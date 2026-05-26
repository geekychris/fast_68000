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
| `[WB-FLAGS-WR]` | any write in chip-RAM `$C128..$C140` (intuition Gadget struct) |
| `[INTU-STRUCT-WR]` | any write in `$BE80..$BF20` (Workbench Screen area) |
| `[SIGWAIT-WR]` | any write to input.device's tc_SigWait field |

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

## 3. Working with the ROM + ADF images

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
