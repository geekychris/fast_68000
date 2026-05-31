# Diagnostics

This doc covers the **runtime investigation toolchain** for boot-stall and
runtime-bug hunts.  Separate from `TESTING.md` (which is about regression
suites) — this is the gear you reach for when something runs but doesn't
behave.

The toolchain has three layers:

1. **Python forensic walkers** — post-mortem inspection of a chipram dump
2. **In-RTL `$display` traces** — guarded by `KICKSTART_BOOT_TRACE`,
   compiled into the sim and surfaced through `run.log`
3. **Disassembly + ADF/ROM inspection** — `tools/disasm68k.py` plus
   ad-hoc Python one-liners against the raw images

## 1. Python forensic walkers (`tools/*.py`)

Each takes a chipram dump (binary file, 1 byte per byte, base = $0).
Generate one with `CHIPRAM_DUMP=path ./Vm68k_top <cycles>`.

### `tools/task_walk.py`

Walks Exec's TaskReady and TaskWait lists from `ExecBase` (read from
chip RAM `$0000_0004`).  Prints each task's name, priority, state,
sigWait/sigRecvd masks, and (with `--deep`) the saved MOVEM frame at
`tc_SPReg`.

```sh
CHIPRAM_DUMP=dump.bin ./Vm68k_top 1500000000
python3 tools/task_walk.py dump.bin           # all tasks
python3 tools/task_walk.py dump.bin --deep    # + register frames
python3 tools/task_walk.py dump.bin 0x1234    # dump one specific task
```

**Use it for:** finding which task is parked, what signal bit it's
waiting on, and (with `--deep`) which function it's stuck inside.

### `tools/port_walk.py`

Walks a MsgPort's message list from a known port address.  Includes
IORequest field decoding so you can see what request a device is
sitting on.

```sh
python3 tools/port_walk.py dump.bin 0x1088    # File System port
```

**Use it for:** seeing what messages are queued at a port the
deadlocked task is/isn't sending to.

### `tools/screen_walk.py`

Walks an Intuition `Screen` struct: ViewPort + RastPort + BitMap, with
inline flags for fields that look uninitialised (`$CACACACA`, all-zero,
`$FFFFFFFF`).

```sh
python3 tools/screen_walk.py dump.bin           # walks $BE80
python3 tools/screen_walk.py dump.bin 0xBE80    # explicit
```

**Use it for:** post-OpenScreen state verification.  Spotted the
`RastPort.minterms[0..7] = $CACACACA` finding that led to the ADDA.W
RTL bug.

### `tools/disasm68k.py`

Partial 68000 disassembler.  Not a full Motorola implementation —
covers the subset that shows up in Kickstart and Workbench code.
Unknown opcodes print as `DC.W $xxxx`.

```sh
python3 tools/disasm68k.py kickstart/kick_13.bin 0xFD5480 --bytes 80 --base 0xFC0000
```

Recent fixes (commit `8838839`):
* Handles `ANDI`/`CMPI`/`EORI` (mask `0xF100` allows op-field 5/6)
* Static + dynamic bit-ops (`BTST`/`BCHG`/`BCLR`/`BSET`)
* Reorders SWAP/EXT before MOVEM (they share the `$4880` prefix)

**Use it for:** quick "what's at this PC" lookups when the trace
points at a ROM address.

### `tools/render_k13_screen.py` + `tools/k13_residents.py`

Render K1.3 chipram into a PPM (post-OpenScreen Copper-list walker)
and list resident structs.  Less frequently used but documented in
the source headers.

### `tools/fsuae_state.py` — FS-UAE save-state diff against our snapshots

When internal traces don't pinpoint *where* K1.3 diverged from the
canonical Amiga boot, fetch a known-good state from a software
emulator and compare byte-for-byte.

```
python3 tools/fsuae_state.py <input.uss> <output_dir>
```

Parses an FS-UAE `.uss` save state (FOURCC chunks; `CRAM`/`BRAM` are
zlib-compressed with a small flag+size header).  Writes the same
`regs.txt` + `mem.hex` + `slow.hex` format our `build_cosim_window/snap/`
uses, plus extracts `DMACON` / `INTENA` / `INTREQ` from the `CHIP`
chunk.

**Two quirks the source comments in `fsuae-src/` don't mention** —
both discovered empirically by searching for known register values:

- `CPU` chunk has +4 bytes between FLAGS and D0 (vs. textbook
  `save_cpu()` in `newcpu.cpp`).  Layout in FS-UAE 3.x: `+$0C` D0,
  `+$2C` A0, `+$48` PC, `+$54` ISP/SSP, `+$58` SR (in high half of
  the longword).
- `CHIP` chunk has the same +4 prefix before `chipset_mask`.  So
  `DMACONR` is at `+$0A`, `INTENAR` at `+$24`, `INTREQR` at `+$26`.

Usage pattern for boot-divergence bisection:

1. Boot WB1.3 in FS-UAE GUI, F12 → Save state at the moment of
   interest (idle desktop is a good choice).
2. `python3 tools/fsuae_state.py "<path>.uss" /tmp/fsuae_snap`
3. Snapshot our Verilator at the matching PC with
   `make cosim-window SNAP_PC=00FC0F90 SNAP_AFTER=N WINDOW=10 ...`
4. `cmp` chip RAM, slow RAM, and `regs.txt`.  The matching
   long-lived pointers (ExecBase, lib bases) confirm the bisection
   is anchored at the same boot moment; the diverging working
   state pinpoints which K1.3 data structures took a different
   branch.

This is what unblocked the WB1.3-corruption investigation —
see WB13_DEBUG_JOURNAL.md §12-§13.  The single most useful diff was
chip RAM `$C0-$FF` (16 zero longwords on us vs. 16 pointers into a
slow-RAM IRQ-dispatcher trampoline on FS-UAE) — which finally
identified K1.3 skipping its IRQ-server install as the upstream
cause of the bitplane-DMA-never-enabled symptom.

### `tools/fsuae_diff.py` — live FS-UAE vs Verilator comparison at any PC

Builds on the [`fsuae_remote_patch`](https://github.com/geekychris/fsuae_remote_patch)
sister project (cloned/built once via `cd ../fsuae_remote_patch && ./build.sh`).
That gives us a controllable FS-UAE instance over HTTP/JSON-RPC.

```sh
make cosim-window                       # build our Verilator boot binary
tools/fsuae_diff.py --pc 0xFC0240       # compare at "after-zclr"
tools/fsuae_diff.py --pc 0xFC2F80       # compare at "first-CopyMem"
tools/fsuae_diff.py --pc PC --skip-fsuae --skip-veri
                                         # reuse cached captures
```

The tool:
1. Boots fs-uae via its RPC, installs a breakpoint at the target PC,
   hard-resets, waits for the BP to fire
2. Dumps FS-UAE's chip RAM (0..$80000), slow RAM ($C00000-$C7FFFF),
   CPU registers, and custom-chip state to files under `/tmp/fsuae_diff/`
3. Runs our `build_cosim_window/Vm68k_top` with
   `CHIPRAM_SNAP_PCS=PC` to dump chip RAM at the same PC
4. Diffs the two chip-RAM binaries, prints first divergence + a
   hex window around it, then dumps the IRQ-vector slots `$C0-$FC`
   side by side

Output is colour-highlighted: bytes the two emulators agree on are
dim, divergent bytes are bright.  Hunk grouping coalesces
near-adjacent differences.

vs the older `fsuae_state.py` approach: `fsuae_state.py` parses an
*offline* `.uss` save state taken by a human in the GUI.
`fsuae_diff.py` is fully scripted — you specify a PC, both
emulators run automatically.  Much faster iteration during
bisection.

Documented in WB13_DEBUG_JOURNAL.md §16.

## 2. In-RTL `$display` traces

All traces in `rtl/m68k_core.v` are wrapped in
``` `ifdef KICKSTART_BOOT_TRACE … `endif ```
and only compile in when `BOOT_TRACE=1` is set on the `make` invocation.
Output goes to `build_kick_boot/run.log`.

Trace conventions (look for these in source):

* **Single-instruction PC trigger.** Filter `is_settled && ex_pc == X`.
  Use when you know exactly which instruction in ROM you care about.
* **Register-value trigger.** Filter `u_rf.regs[Rn] == VALUE`.  Use
  when you want to catch the first time a register reaches a specific
  state — often paired with `last_value != VALUE` to fire on the edge.
* **Window-gated trigger.** Filter `retired >= X && retired <= Y`.
  Use when you've narrowed the bug to a known retired-count window
  and want to log everything inside (a register, a memory address,
  etc.).
* **Memory-write watch.** Filter `dc_req_r && dc_we && dc_ack &&
  dc_addr >= LO && dc_addr <= HI`.  Use to find who wrote to a
  suspect address.

### Existing trace markers (77 unique as of this writing)

Grouped by category — each appears in one or more `$display` strings
in `rtl/m68k_core.v` and shows up in `run.log` as `[NAME] r=…`.

**Boot-state milestones**
| Marker | Fires when |
|---|---|
| `[RESINIT]` | ROM resident-init step starts |
| `[STRAP-BOOT]` | strap reaches bootblock entry |
| `[DOS-INIT]` | dos.library rt_Init runs |
| `[DOSINIT]` | strap → DOS handoff |
| `[OVL]` | OVL bit cleared (chip-RAM mapped at $0) |

**CPU / instruction milestones**
| `[PC]` / `[TRKPC]` | filtered per-instruction PC trace |
| `[STKW]` | stack write |
| `[SSPTINY]` / `[USPLOW]` | SP heading negative / very small |
| `[MOVEM-X]` | MOVEM extension fired |
| `[EXC]` | exception entry |
| `[VEC]` | vector read (during exception) |
| `[VEC-WR]` | CPU write to ILLEGAL-vector range ($10..$13) |
| `[VEC-EXEC]` | redirect into the vector table (always wrong outside of EXC) |
| `[BAD-PC]` | redirect to an address outside RAM/ROM bounds |

**Register tracking**
| `[A5-CHANGE]` / `[A5-WIN]` / `[A5-BAD]` | A5 transitions |
| `[A7]` | A7 transitions |
| `[BISECT-A3-LOAD/USE]`, `[BISECT-A0]`, `[BISECT-A1]`, `[BISECT-A1-FINAL]` | step-by-step register snapshot at a hot-path instruction |

**Memory pattern watches**
| `[CACA-WR]` | any write where high or low half = `$CACA` |
| `[5D80-WR]` | any write in chip-RAM `$5D80..$5E00` |
| `[5E40-WR]` | CPU writes to chip-RAM `$5E3E..$5E43` (BCPL DOS table entry) |
| `[5E40-BUS-WR]` | bus-arbiter watch: ALL writers (CPU + blitter + DMA) to `$5E3C..$5E47`.  Pairs with [5E40-WR] to localise the corrupting writer's `src_id` |
| `[INTVECS-WR]` | bus-arbiter watch on chip-RAM `$C0..$FF` (68k user-defined interrupt vectors / K1.3 IRQ-server `IntVects[]`).  FS-UAE has these populated with pointers into a slow-RAM BSR-trampoline dispatcher; ours stays zero post-boot.  Confirms K1.3 skips the dispatcher-install code path |
| `[WB-FLAGS-WR]` | any write in chip-RAM `$C128..$C140` (intuition Gadget struct) |
| `[INTU-STRUCT-WR]` | any write in `$BE80..$BF20` (Workbench Screen area) |
| `[SIGWAIT-WR]` | any write to input.device's tc_SigWait field |
| `[WBPC-WR-C04A8E]` | CPU writes to slow-RAM `$C04A8C..$C04A91` (LINK frame for the WB1.3 routine at $FEA932) |
| `[PUTMSG-PKT]` | dp_Type capture at `$FC1B70` PutMsg entry — **dp_Type lives at offset $28 from msg start in K1.3/V33, NOT the textbook $1C** |

**Exec primitive entry hooks**
| `[OBTSEM]` | ObtainSemaphore body entry (`$FC2DF0`) |
| `[RELSEM]` | ReleaseSemaphore body entry (`$FC2E40`) |
| `[SEM-WAIT]` | Wait($10) inside ObtainSemaphore contended path |
| `[WAIT-CALL]` / `[SIGNAL-CALL]` | Exec.Wait / Exec.Signal entry |
| `[PUTMSG]` / `[REPLYMSG]` / `[TD-BEGIO]` | exec/dos/device library entry |
| `[DOS-GETMSG]` / `[DOS-CREATEPROC]` / `[DOS-RT-INIT]` / `[DOS-ALERT]` | dos.library entry hooks |
| `[DOS-ADDTAIL]`, `[DOS-ADDTAIL1]`, `[DOS-ADDTAIL2]` | AddTail wrappers |

**Intuition / graphics**
| `[INTU-FN-ENTRY]` | intuition function entry hook |
| `[INTU-COPY]` | intuition `MOVE.L $7C(A3), $10(A2)` (minterm copy) |
| `[INTU-COPY-EARLIER]` | one level up the call chain |
| `[INTU-D1]` | A1 + D3 just before the bit-12 branch |
| `[INTU-MKGADGET]` / `[INTU-MKGADGET-TPL]` | gadget-template factory + copy |
| `[INTU-TPL-COPY]` | helper $FD643A's MOVE.L copy loop |
| `[INTU-STRUCT-WR]` | writes to the Workbench Screen struct |

**Disk / floppy**
| `[BB-PC]` | bootblock PC |
| `[BADSEC]` / `[HDRCHK]` / `[SECID]` | sector validation |
| `[TRACK]` | track-load |
| `[KBD-CMD9]` / `[KBD-SP]` | CIA-A keyboard SP/CNT handshake |
| `[SYNCSCAN]` | MFM sync scan |
| `[STRLEN]` | strlen-style scan |

### Adding a new trace

The pattern (see commits `054f703`, `27cb75c`, `fd504dc` for examples):

```verilog
`ifdef KICKSTART_BOOT_TRACE
    // [MARKER] one-line description of why this fires
    if (is_settled && ex_pc == 32'h00FX_YYYY)
        $display("[MARKER] r=%d pc=%h reg1=%h reg2=%h",
            retired, ex_pc, u_rf.regs[N1], u_rf.regs[N2]);
`endif
```

Build/run cycle (force-rebuild because `USE_CACHE=1` is default):

```sh
rm -rf build_kick_boot
make test-kickstart-boot \
    ROMFILE=kickstart/kick_13.bin ADFFILE=kickstart/wb13.adf \
    ROMCYCLES=30000000 BOOT_TRACE=1
grep '\[MARKER\]' build_kick_boot/run.log
```

A boot to r≈4.7M takes ~30 seconds with trace I/O.

### Trace bisection workflow

When you know an instruction produces a wrong register but don't know
which sub-step, drop multiple `[BISECT-X]` traces at consecutive PCs and
compare the values.  This is how the ADDA.W bug (commit `19da2fc`)
was pinned to a single instruction at `$FD64A6`:

```
[BISECT-A3-AFTER]  A3=$00FD6062   ; MOVEA.L #imm OK
[BISECT-A0]        A0=$00FD6062   ; MOVEA.L A3,A0 OK
[BISECT-A1]        A1=$00FD6062   ; MOVEA.L A0,A1 OK
[BISECT-A1-FINAL]  A1=$00006062   ; ADDA.W D0,A1 — WRONG
```

## 3. PC-triggered chip-RAM snapshots — `CHIPRAM_SNAP_PCS`

`CHIPRAM_DUMP=path` captures chip-RAM only at end of sim, which
is often too late — by the time you see the failure, the relevant
state has been overwritten.  `CHIPRAM_SNAP_PCS` lets you snapshot
**mid-simulation** whenever core 0's EX-stage PC enters one of a
list of configured values.

Each snapshot is a separate 512 KB binary, named
`<DIR>/snap_<seq>_<label>_r<retired>.bin`, so you can diff across
multiple iterations of the same PC.

### Usage

```sh
CHIPRAM_SNAP_PCS=5D82:movem-src,5D90:bad-jsr \
CHIPRAM_SNAP_DIR=/tmp/snaps                  \
./build_kick_boot/Vm68k_top 30000000
```

Env vars:
* `CHIPRAM_SNAP_PCS` — comma-separated entries `pc[:label]`.  `pc`
  is hex (with or without `$`/`0x` prefix), masked to 24 bits to
  match 68000's external address bus.  `label` is an optional
  filename tag; defaults to `pcXXXXXX`.
* `CHIPRAM_SNAP_DIR` — output directory.  Defaults to current dir.

On each fire the sim prints:

```
[sim] CHIPRAM_SNAP /tmp/snaps/snap_3_movem-src_r4203008.bin
      (pc=$005D82 retired=4203008 cycle=16812345)
```

so you can correlate with the `retired=` values shown in trace
markers.

### How it fires (and what to know)

`cur_pc` is exposed at the m68k_core's output and routed through
`m68k_top.core_pc_flat`.  Detection is **edge-triggered** — fires
on the cycle PC transitions INTO a trigger value, then waits for
PC to leave and come back before firing again.

* A long STOP at the trigger PC fires exactly once.
* A multi-cycle instruction at the trigger PC fires exactly once
  (the cycle ex_pc is updated to the trigger value).
* If the only path to the trigger PC is from within the trigger
  itself (a tight self-loop), only the first entry fires.

### Inspecting snapshots

Same Python walkers work on snapshots as on `CHIPRAM_DUMP` outputs
— each snap is a full 512 KB chip RAM image starting at byte 0.

```sh
python3 tools/screen_walk.py /tmp/snaps/snap_3_movem-src_r4203008.bin
python3 tools/task_walk.py   /tmp/snaps/snap_3_movem-src_r4203008.bin
```

For ad-hoc inspection:

```sh
python3 -c "
with open('/tmp/snaps/snap_3_movem-src_r4203008.bin','rb') as f:
    data = f.read()
for addr in (0x5D4A, 0x5D4E, 0x5D52):
    v = int.from_bytes(data[addr:addr+4], 'big')
    print(f'\${addr:08X}: \${v:08X}')
"
```

### When to reach for it

* You have a register that takes a bad value at a specific PC and
  you need to know what's in memory at the moment that instruction
  executes.
* You want to confirm whether a write that "should be there" is in
  chip RAM or only in cache.
* You want to diff chip RAM across two iterations of the same loop
  (snap twice on the same PC, diff with `cmp` or a Python loop).

The feature has no runtime cost when no PCs are configured; with
PCs configured it costs one extra check per cycle (negligible) and
~50 ms per snap fire (the 512 KB read).

## 4. PC histogram — `PC_HISTOGRAM`

The flip side of `CHIPRAM_SNAP_PCS`: when you don't know which PC
to snap at, the histogram tells you where the CPU is spending its
time.

```sh
cd build_kick_boot
PC_HISTOGRAM=1                       \
PC_HISTOGRAM_BUCKET_BITS=4           \
PC_HISTOGRAM_TOP=20                  \
./Vm68k_top 200000000
```

Env vars:
* `PC_HISTOGRAM=1` — enable (default off, zero overhead when off).
* `PC_HISTOGRAM_INTERVAL=N` — sample every N host cycles
  (default `1024`).
* `PC_HISTOGRAM_TOP=K` — show top K buckets (default `30`).
* `PC_HISTOGRAM_BUCKET_BITS=B` — lop off low B bits to group nearby
  PCs (default `0` = exact PC; `4` = per-16-byte region; `8` =
  per-256-byte region).

Output at sim end:

```
[sim] PC_HISTOGRAM total_samples=195312 buckets=1035, showing top 20:
  $FC2250       76644 samples ( 39.24%)
  $FC2240       76643 samples ( 39.24%)
  $FC2260       15332 samples (  7.85%)
  $FC3130        7042 samples (  3.61%)
  ...
```

### When to reach for it

* The boot reaches some state then stops emitting traces — you need
  to know what loop it entered.  (E.g. WB1.3 post-MOVEM-fix went
  silent post-r=4.9M; the histogram revealed 78% of time in K1.3's
  serial-port poll loop.)
* You're working on performance and want to confirm the hot loop
  isn't where you expect.
* You want to verify a fix actually removed a tight loop (compare
  pre/post histograms).

The bucket-bits setting is useful for "what region of code is hot"
vs. "exact instruction is hot."  Set it to 4 to merge adjacent
instructions of a tight loop into one bucket; the count then
roughly reflects time-in-the-function rather than time-at-one-PC.

## 5. Working with the ROM + ADF images

### Searching ROM/ADF for a literal value

```sh
python3 -c "
import re
with open('kickstart/kick_13.bin','rb') as f: rom = f.read()
target = b'\x4E\xAE\xFD\xCC'   # = JSR -$234(A6) = ObtainSemaphore LVO
for m in re.finditer(re.escape(target), rom):
    print(f'\$00{0xFC0000+m.start():06X}')
"
```

Useful for:
* Finding all callers of a library LVO (search for the
  `JSR disp(A6)` byte pattern)
* Confirming a specific 32-bit constant is/isn't in the disk image
  (e.g. validating "this value came from runtime computation, not
  from data")
* Locating struct templates by signature

### `make crosscheck` / `make crosscheck-minimig` / `make crosscheck-fx68k`

Run our RTL against an independent reference (Musashi, MiniMig, or
FX68K).  See `COSIM.md` for the full guide.  Use these when you've
narrowed a bug to a specific CPU instruction or chipset register
behaviour — the cosim will tell you which side is correct.

## 6. The 24-bit address-bus quirk

The 68000 has a 24-bit external bus (A1..A23).  PCs and effective
addresses are computed internally as 32-bit values but only the low
24 bits go onto the bus.  Our redirect-mask logic (`m68k_top.v`
around line 364) enforces this:

```
if_pc <= {8'd0, redirect_pc[23:0]};
```

K1.3 stores function pointers with garbage in the top byte — e.g.
the boot vector at `$08` may contain `$7800090E` and yet correctly
fetch at `$90E`.  So a `BAD-PC from=X to=Y` trace where Y has high
bits set is normal-on-real-Amiga; our sim just shows the unmasked
value for clarity.

If you see a register that is supposed to be a chip-RAM pointer
but has high bits set (like `A5=$082800FC`), that points to a
relocation / pointer-load bug *upstream* of the address bus —
not a bus-masking issue.

## A worked example — the ADDA.W bug

Full chain from "WB1.3 boot hangs at r=4M" to a single-instruction
RTL fix, captured commit-by-commit:

1. `[INTU-FN-ENTRY]` revealed `D3 = $FFFF0000` at the deadlock site.
2. `[INTU-D1]` showed `A1 = $C128` (a Gadget struct address).
3. `[WB-FLAGS-WR]` showed nobody wrote bit 12 or 14 of `$C138`
   (`GadgetType`).
4. `[INTU-MKGADGET]` + `[INTU-MKGADGET-TPL]` traced the call chain
   to a gadget-template factory at `$FDE8E0`.
5. `[INTU-TPL-COPY]` showed `A1 = $00006062` at the copy loop, when
   it should have been `$00FD6062`.
6. `[BISECT-*]` set at 4 consecutive PCs pinned the high-byte loss
   to one specific `ADDA.W D0, A1` at `$FD64A6`.
7. Static read of `m68k_alu.v` confirmed the bug: `a_sized = a & mask`
   masks both operands for SZ_W, zeroing the high half of An.
8. Fix in `m68k_core.v` K_ALU + regression test `t144_adda_w_preserve_high.s`.

Each step took one rebuild+run cycle (~30 sec) and one grep — the
trace ladder is what made the investigation tractable.
