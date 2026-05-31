# WB1.3 debugging journal — techniques, observations, dead ends

A retrospective on the WB1.3 boot-to-Workbench-display work across
commits f923bc7 (Denise FB gate), 0bb2abd (Copper-self bus-xlat mask),
efb8e29 (CPU 24-bit PC mask + Copper COPxLC block + VBL force-restart),
c2aa5dd (first WB render captured), and e71b002 (sim_main chdir).

This is half "what I found" and half "techniques worth reusing." Skip
ahead to **§7** if you only want the latter.

## §1. The starting state

After many sessions of fix-by-fix progress, K1.3 V33 was reaching
Intuition init and idling at the standard idle STOP. Visible end-state:
`bpl_fetches=0 dmaconr=0000 bplcon0=0 — no Workbench display.` The
test target passed because it only verified "idle STOP at $FC0F90 +
retired > 2M." Everything past that — Workbench Screen open, bitplanes
drawn, etc. — was an open question.

I had a memory note saying "MrgCop fires 0 times" and "Workbench
Copper at $100C8 never populated." Both turned out to be wrong, but
in interesting ways.

## §2. First diagnosis — and the first wrong conclusion

**Symptom:** chip-RAM dump at `$100C8` (the K1.3 LOFCprList) was full
of `$FFFFFFFF` instead of a valid Copper list. The dump tool showed
the area was being repeatedly overwritten with all-ones.

**Trace I added:** a chip-RAM watchpoint in `m68k_core.v`:
```verilog
if (dc_req_r && dc_we && dc_ack &&
    dc_addr >= 32'h0000_100C8 && dc_addr <= 32'h0000_103C8)
    $display("[CPRLIST-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
        retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
```

**What it showed:** `$FFFFFFFF` writes at `pc=$FC17A6`, every few
hundred thousand cycles.

**My first interpretation:** something in the K1.3 graphics enable-
interrupt wrapper (which lives at `$FC17xx`) was writing `$FFFFFFFF`
to the Copper buffer.

**Why that was wrong:** I disassembled `$FC17xx` and confirmed the
code is `MOVE.W #$C000, $00DFF09A` (re-enable INTENA master). It
*cannot* write to chip-RAM $10170 — the destination is $DFF09A.

The PC field in the trace was *not* the PC of the writing instruction.
It was whichever instruction the CPU happened to be executing when the
bus delivered the write `ack`. The actual writer was some other bus
master.

**Lesson 1:** When a CPU-side trace fires for an event that does not
match the instruction at `ex_pc`, suspect a different bus master.
The `ex_pc` field is sampled at bus-ack time, not at issue time.

## §3. The bus-level trace — and finding the real culprit

To identify *which* bus master, I added a tracer in the bus arbiter
that prints the **winner port id**, not the CPU PC:

```verilog
if (addr[winner] >= 32'h0000_10460 &&
    addr[winner] <= 32'h0000_10500 && we[winner])
    $display("[10468-BUS-WR] t=%0t port=%0d addr=%h be=%b wdata=%h",
        $time, winner, addr[winner], be[winner], wdata[winner]);
```

The port id maps to: 0,1 = CPU I/D cache, 2 = BLT, 3 = COP, 4 = DEN,
5 = PAU.

**What it showed:** `port=4` (Denise) writing `$FFFFFFFF` patterns to
chip-RAM `$10460+`.

**That was the smoking gun.** Denise is the display chip; it should
**read** bitplane data, not write to chip RAM. But our Denise module
has a "chunky framebuffer" output path for the SDL2 demo harness, and
the framebuffer base address was `FB_BASE = $00010000`. With a 256×192
chunky output, that occupies `$10000..$1BFFF` — overlapping K1.3's
LOFCprList at `$100C8`.

**Fix (f923bc7):** under `KICKSTART_BOOT`, gate Denise's `S_WRITE`
state to advance without issuing the bus write. The chunky FB is
sim-harness scaffolding; under K1.3 we walk the live Copper via
`render_k13_screen.py` instead.

**Lesson 2:** When a coprocessor master writes to a region someone
else "owns," check the design parameters. The FB_BASE override
mattered because K1.3 *chose* chip-RAM `$100C8` for LOFCprList
allocation, but Denise *also* chose `$10000` for its scratch — and
in a real Amiga, Denise has no scratch chip-RAM. Real-Amiga modeling
is only as faithful as the parameters you give it.

## §4. The second wall — Copper runaway corruption

After the Denise fix, `$100C8` started getting populated with valid
Copper instructions… but K1.3 still didn't display Workbench. The
sim ended with `bpl_fetches=0`.

I added Copper state-machine traces gated on `+define+COP_TRACE`:

```verilog
if (auto_kick_i && !auto_kick_d && auto_active_i && !cop_busy)
    $display("[COP-KICK] auto-kick cop1lc=%h cop2lc=%h", cop1lc, cop2lc);
if (mst_ack) $display("[COP-FETCH] pc=%h ir1=%h ir2=%h", pc, ...);
if (is_copjmp2) $display("[COP-JMP2] reload pc=%h", cop2lc);
```

**What I observed:**
1. First COP-KICK fires at r=120K with `cop1lc=$0420, cop2lc=$0`.
2. The boot Copper at `$0420` runs through SPRnPT initialisation,
   reaches `WAIT V=$0C` at `$0468`, satisfies it (vbeam=$A1 > $0C).
3. At `$046C`: `008A 0000` decodes as `MOVE COPJMP2=$0` — strobes
   COPJMP2 → reload PC from `cop2lc=$0`.
4. Copper now walks chip-RAM zeros from `$0` onward. Each `$0000 0000`
   is `MOVE BLTDDAT $0` — blocked by our `mv_reg < $40` gate, so the
   Copper just advances PC by 4.
5. After ~33K instructions of zeros, PC reaches chip-RAM somewhere
   around `$11000+` where there's actual non-zero data.
6. That data happens to decode as `MOVE COP1LCH=$0140`.
7. Our slave-write xlat combined `$0140` (high half) with
   `canon_cop1lc[15:0]=$0420` and wrote `$01400420` to `cop1lc`.
8. **From this point onward, every VBL auto-restart reloads PC from
   `$01400420`** — past chip RAM end. The arbiter returns 0 for
   non-existent memory, so Copper eats zeros forever and never
   re-enters the boot list at `$0420`. K1.3's VBL handler writes
   `COP2LC=$100C8` (LOFCprList) every frame, but Copper can never
   reach the `COPJMP2` strobe that would jump there.

**The runaway corrupted itself.** The key word is `canon_cop1lc[15:0]`:
the xlat reconstructs the 32-bit value from the half-word the Copper
wrote *plus* the cached half from the last CPU write. The Copper-write
should only set 19 bits (real OCS Agnus has 19-line COP1LCH/L latches),
but our xlat used the full 16-bit half.

**Fix attempts that broke things:**
- **Slave-write mask in copper.v:** masked `cop1lc/cop2lc` to 19 bits
  on every slave write. Broke K1.3 because the CPU reads `COP1LC` back
  via the slave port and compares; masked read-back failed K1.3's
  validity check, branched into bad code, ended up in a Line-F loop
  at PC=`$BFD400`.
- **Force-restart at every VBL (regardless of `cop_busy`):** also
  broke K1.3 in a similar Line-F loop. Suspected cop_busy bit being
  polled somewhere.
- **HALT on out-of-range fetch:** also broke K1.3, in a third
  different way.

All three "obvious" fixes failed because they all changed behavior
visible to the CPU side (read-back, busy bit, COPSTAT). Many sessions
worth of confusion ensued.

## §5. Isolated unit test — `tests/t150_cop_jmp_self_modify.s`

I stopped trying fixes against K1.3 and built a **single-instruction-
level reproduction** of the runaway:

```
boot Copper at $500: MOVE COPJMP2=0
cop2lc points at $1000: MOVE COP1LCH=$0140, MOVE COP1LCL=$0420, end
```

Then enable DMACON.COPEN+DMAEN, spin for one VBL, check whether
`cop1lc` retained its upper bits.

```
move.l  $00FE0040, D2                 ; slave-port read of cop1lc
andi.l  #$FFF80000, D2                 ; mask to bits 31..19
bne     fail_runaway                   ; non-zero = corruption survived
```

Without the fix the test halts with `stop $C152`. With the fix it
halts with `stop 0`.

**Lesson 3:** The moment you can reproduce a bug in five lines of
assembler that runs in 1M cycles, the cost of iterating drops by
100×. K1.3 boot takes 5+ minutes to rebuild and run, and the failure
mode (Line-F loop, blank screen) is a downstream symptom. The unit
test was definitive — no "but maybe it's a side-effect of X." The
fix had to make the test pass and not break the regression suite.

## §6. The correct fix — `0bb2abd`

With the unit test in place, the right fix surfaced quickly: do the
mask at the **bus xlat layer**, gated on the **winner port**, not at
the slave-write site:

```verilog
wire        winner_is_cop = (winner == (N_PORTS - 3));  // = COP_PORT
wire [15:0] cop_self_high_masked = winner_is_cop
                                   ? {13'd0, amiga_wdata_half[2:0]}
                                   : amiga_wdata_half;
```

This masks COP1LCH/COP2LCH writes to 19 bits **only when the Copper
master is writing**, and leaves CPU MOVE.L writes (which use the
`is_long && be==4'b1111` 32-bit path) untouched.

**Subtle gotcha that bit me on the first attempt:** the `canon_cop1lc`
shadow on the bus side also needs the same mask — otherwise the next
COPxLCL slave write reconstructs the full 32-bit value with the
poisoned upper half:

```verilog
3'd0: if (is_long[winner] && be[winner] == 4'b1111)
            canon_cop1lc <= wdata[winner];
      else canon_cop1lc[31:16] <= cop_self_high_masked;
```

Without the canon-shadow mask, the unit test still failed.

**Lesson 4:** When a fix splits state across two registers (the
authoritative one and a shadow), apply the same invariant to both.
"The shadow is just a cache" is comforting until the cache becomes
the source of truth on the next write.

## §7. Techniques worth keeping

A. **Trace at the bus layer, not just the CPU.** `ex_pc` in a CPU
   trace is meaningless for events that originate in a coprocessor.
   When a write doesn't match the instruction at `ex_pc`, immediately
   add a bus-level trace printing the `winner` port id. The next
   diagnosis becomes obvious.

B. **Build unit-test reproductions for any bug whose K1.3 manifestation
   has multiple layers of symptom.** Even five lines of assembler
   that halts on success or `$BADn` on failure beats a 5-minute K1.3
   boot whose only end-state signal is "blank screen."

C. **Distinguish "writes happen" from "writes land."** Use multiple
   complementary traces:
   - `[CPRLIST-WR]` (CPU-side, `dc_req_r && dc_we && dc_ack`)
   - `[10468-BUS-WR]` (bus-arbiter side, `winner_valid && we[winner]`)
   - chip-RAM dump (`CHIPRAM_DUMP=…`) — the final state
   If the three diverge, you have a hint where the diverge happens.

D. **Keep multiple decoy hypotheses on the table at once.** When my
   "first-bus-write-at-PC-$FC17A6" diagnosis was wrong, I tried four
   different interpretations of the instruction stream before
   abandoning the CPU-PC theory and adding a bus tracer. Sticking to
   a wrong hypothesis is the expensive failure mode.

E. **The `$display` PC may not be the PC that issued the write.**
   In our pipeline, `ex_pc` updates as the pipeline advances. A bus
   ack arriving on cycle N latches a watcher with whatever `ex_pc`
   holds on cycle N — which may be the instruction *after* the one
   that issued the write. Don't trust `ex_pc` in a trace condition;
   verify against the disassembly.

F. **For 32-bit-host vs 24-bit-target traps: mask everywhere.** Real
   68000 has 24 address lines; PC bit 24+ should always be zero. The
   bus arbiter masks (commit 56238c5), but the CPU's internal `if_pc`
   register can still accumulate bit 24+ across ROM-end wraps. If you
   see PCs like `$014B1C62` in any trace, something accumulated
   bit 24 somewhere; mask there too.

G. **Always rebuild from clean (`rm -rf build_kick_boot/`) when a fix
   doesn't seem to work.** Verilator's `USE_CACHE=1` mode silently
   reuses old object files in some scenarios. Several times today I
   "confirmed" a fix worked because a stale binary was running. The
   way to know: `stat build_kick_boot/Vm68k_top` vs `git log -1
   <changed-file>`.

H. **Verify CWD assumptions.** Verilator's `$readmemh` resolves paths
   relative to the **current working directory**, not the binary's
   directory. If the binary can't find its hex files, Verilator prints
   a `%Warning` but does **not abort** — the sim runs with all-zero
   memory and the CPU executes NOP-of-zero opcodes forever. This bit
   me hard: I spent half a session "investigating non-determinism"
   before realising that `build_kick_boot/Vm68k_top` invoked from the
   project root was running with no ROM loaded. The fix (e71b002) is
   one `chdir(dirname(argv[0]))` at the top of `main()`.

I. **A "passing" test isn't proof of correctness.** Our
   `test-kickstart-boot` target only checks for the idle STOP at
   `$FC0F90` + `retired > 2M`. With zero ROM loaded, K1.3 of course
   never reaches STOP — but a watchful eye would have noticed that
   `make` reported the test as failed. I missed it because I was
   reading `build_kick_boot/run.log` instead of the Makefile's
   `PASS/FAIL` line.

J. **Don't trust sim_main telemetry without checking how it reads
   state.** `[sim] Agnus: bpl_fetches=0 bplcon0=0` was misleading me
   for hours — the read path uses `fb_peek_data` which only handles
   chip and slow RAM, not the `$00FE_9100`-style probe registers.
   The real chipset BPLCON0 *was* getting `$A302` (verified via
   Copper trace), it just wasn't visible through `fb_peek`. Knowing
   which registers your readout can and can't see is critical.

## §8. The current "almost works" state

After e71b002, with a clean build:
- 139/139 regression tests pass.
- `make test-kickstart-boot` passes.
- K1.3 boots through resident-init for all libraries.
- Strap runs at r=2.4M, validates `DOS\0` magic, JSRs to real
  bootblock entry at chip-RAM `$1564`, returns success.
- dos.library / ramlib.library RT_INIT runs.
- Intuition's `OpenScreen` at r=4.0M opens the Workbench Screen,
  returns valid Screen pointer `$C01358`.
- 3 calls to `MakeVPort`.
- Workbench Copper at `$100C8` contains the assembled list:
  WAIT V=$2B, palette load, BPLCON0=$0302, DDFSTRT/STOP/MOD,
  BPL1PT=$60C8, BPL2PT=$B0C8, WAIT V=$2C, **BPLCON0=$A302** (HIRES
  interlaced 2-bitplane).
- The Copper executes this list ~15,000 times across a 50M-cycle
  trace. `BPLCON0=$A302` is written each iteration.
- 57 DSKLEN events fire (trackdisk is actually reading disk).
- `render_k13_screen.py` produces `docs/wb13_first_screen.png` —
  the actual K1.3 Workbench Screen container, classic blue (`$005A`).

But the bitplanes themselves are still essentially empty (2 non-zero
bytes in plane 1, 0 in plane 2). All 149 blits that target the
bitplane region after r=4M use minterm `LF=$00` (clear-to-zero).
None of them are *drawing* blits.

So Intuition allocates and clears the BitMap, sets up the Copper,
and then… something interrupts the chain that would normally call
`PrintIText` / `RefreshGadgets` / `DrawImage` to put title-bar text
and gadget pixels on the bitmap.

That's where task #90 sits now.

## §9. Open hypothesis for the bitplane wall

Most-likely explanation (memory note from earlier sessions): the
Workbench task isn't calling its drawing routines because it's
waiting for an input event that never arrives. Our SDL2 doesn't
inject mouse/keyboard activity for the test path; trackdisk reads
the disk but doesn't necessarily signal Workbench-task in a way that
triggers redraw.

Alternative: the `Layers` library's damage-region machinery isn't
running. Layers manages screen redraw; if `LayersBase` ops never
fire, no draw happens.

Both would explain the symptom: 1068 `Signal()` calls between tasks
(they're alive and running event loops), but no drawing blits ever
issued.

## §10b. Task-list forensics — finding the parked CLI

After establishing that Intuition opens the Screen successfully and
the Copper executes the Workbench list but no real drawing ever
happens, the next question was "what tasks are alive and what are
they waiting on?"

**Technique A: walk slow-RAM for task structs.** A task struct
starts with a `Node`: `+0..+7 ln_Succ/ln_Pred (pointers), +8 ln_Type,
+9 ln_Pri, +0xA ln_Name (ptr)`. Scan slow-RAM (`$C00000..$C7FFFF`)
two bytes at a time. If `d[off+8] == 1` (NT_TASK) or `13` (NT_PROCESS)
and `d[off+0xA..0xD]` is a plausible pointer (into ROM `$FC0000..` or
back into slow RAM), try to read the string at that pointer. Any string
of printable ASCII is almost certainly a real task name.

This walked 70 unique task-like structs in the dump and identified:
- "File System" at `$C00A80`
- "Initial CLI" at `$C05128`
- "CON" at `$C06778`
- "input.device" (name garbled — ROM string)
- Plus many false positives (random data that happened to start with
  `01` followed by what could be a pointer).

**Technique B: classify blits by minterm to detect "drawing vs
clearing."** `LF == $00` produces zero output for all (A,B,C)
combinations — a clear blit. `LF == $FF` is fill. Anything else is
real drawing. Of the 149 blits targeting the bitplane region after
r=4M, every single one used `LF=$00`. **None of them were drawing
blits.** That's a direct, unambiguous "the system isn't drawing" —
no Workbench-task event-loop tracing needed.

**Technique C: read each parked task's `tc_SigWait` and `tc_SigRecvd`
to find the deadlock.** A task in WAIT state with `tc_SigWait` set
and `tc_SigRecvd == 0` is waiting for a signal that hasn't arrived.
If you can trace who would normally send that signal, and that sender
is also parked, you've found a deadlock or a missing wake.

Reading Initial CLI's struct directly from the dump:
```
ln_Type      = $0D  (NT_PROCESS — DOS process, not raw exec task)
tc_SigWait   = $00000100  (= SIGB_DOS, bit 8)
tc_SigRecvd  = $00000000
```

**Diagnosis:** Initial CLI is parked at `WaitPkt()` (or `Wait(SIGF_DOS)`)
waiting for a DOS reply that never arrives. Without that reply it
never reads the startup-sequence script, never runs `LoadWB`,
workbench.task never starts, no desktop drawing ever happens.

The same wall was hit in an earlier session (task #84) where it was
marked completed; the fix at that point moved the wall forward but
the underlying DOS-message-passing chain (CLI ↔ File System ↔ CON)
still has a missing reply somewhere.

**Refinement after counting Signal(task,mask) pairs across the full
run:**

```
606 → task=$C026E2 mask=$40000000    (input.device VBL ticks)
239 → task=$C0485E mask=$00000400    (timer task)
 91 → task=$C00A80 mask=$00000100    (File System SIGF_DOS)
 48 → task=$C0485E mask=$00000200
 35 → task=$C0485E mask=$00000100
 21 → task=$C05128 mask=$00000100    (Initial CLI  SIGF_DOS)
  9 → task=$C06778 mask=$00000100    (CON          SIGF_DOS)
```

All three DOS handlers (File System, Initial CLI, CON) **do** receive
SIGF_DOS — the PutMsg → Signal chain is functional in our sim. They
each run an event loop:

1. `Wait(SIGF_DOS)` → blocks until packet arrives.
2. `GetMsg(port)` → pull packet.
3. Process packet.
4. `ReplyMsg(packet)` or `PutMsg(next-task-port, ...)`.
5. Loop.

The ratio (91 / 21 / 9 packets) is roughly what you'd expect: File
System is busiest (all I/O goes through it), CLI medium (parsing
startup script), CON least (just I/O).

**The wall is finer than "no signal arrives":** every signal-recipient
task processes its incoming packets, then drains to the WAIT state
with `tc_SigRecvd=0`. The chain stalls at *one specific packet that
never gets sent.* The candidate sequence:

1. CLI sends `Open("S:Startup-Sequence", MODE_OLDFILE)` to File System.
2. File System processes Open, replies success (?) to CLI.
3. CLI receives reply, sends `Read(file, buffer, length)`.
4. File System processes Read…

…and somewhere in step 4, either the disk read returns wrong data,
the FS metadata doesn't match what File System expects, or our
trackdisk's MFM-decoded blocks have a hash/checksum mismatch that
File System catches.

The 57 DSKLEN events fire (disk reads happen), but it's possible
the *content* of those reads doesn't survive File System's
validation, so File System never sends back a successful Read reply
and CLI is parked forever.

Verifying that hypothesis would need a packet-level trace of
`PutMsg` / `GetMsg` calls in DOS, plus correlating each packet's
`dp_Type` and `dp_Res1` against the next task that handles it.
Significant DOS-internals work; deferred.

## §10. Suggested next steps

1. Hook a trace at Workbench task's main event loop entry (need to
   disassemble K1.3 V33 to find it) and confirm it's been entered.
2. Hook `Layers.library` entry points (DrawGadget, RefreshGadgets,
   PrintIText) and see whether any of them ever fire.
3. If none fire, the question is "what's blocking the chain that
   leads to them?" — likely a task waiting on a signal that's
   never sent.

Without those hooks, "why doesn't the title bar draw?" is the kind
of question that takes a full session of K1.3 disassembly to answer.

## §11. Cosim window — eliminating the CPU as a hypothesis

After two sessions of suspecting our MOVEM at `$FEA936` (the visible
write of `0` to `$C04A8E` right before the bad blit), we finally
proved the CPU is byte-identical to Musashi by running a 100K-instruction
cosim window over the corruption point:

```
make cosim-window SNAP_PC=00FEA932 SNAP_AFTER=4437000 WINDOW=100000 \
     ADFFILE=kickstart/wb13.adf ROMFILE=kickstart/kick_13.bin
```

Result: **2 diff hunks in 100,001 lines**, both at MOVEM-load
instructions, both immediately re-converging on the next line. The
divergences are a trace-emission-semantics artifact (Verilator's
`is_settled_after_movem` fires at end-of-MOVEM = post-load state;
Musashi's `instr_hook` fires at start-of-instruction = pre-load state).
Same retired count, different snapshot point in the multi-cycle
instruction. By the next single-cycle instruction, both CPUs show
identical state.

Two sessions of "MOVEM is the bug" diagnosis dissolved in 30 minutes
of cosim. The previous diagnoses had assumed the `MOVEM.L D0/A0/A1,
$FFEA(A2)` was predec mode (so D0 would land at $C04A8E). The actual
instruction is `48EA 0301 FFEA` = d16(An) mode; D0 lands at $C04A8A
and A0 lands at $C04A8E. A0 was already 0 entering the routine.

**Technique:** when stuck on a multi-session "CPU bug" hypothesis,
the cosim window is the definitive test. Use a SNAP_PC that's actively
executing (not a STOP), and a window large enough to cover the
suspected divergence. The two MOVEM-load hunks always appear in
diffs — they're trace-format artifacts and can be ignored.

## §12. FS-UAE state diff — the real ground truth

The cosim cleared the CPU but didn't tell us *why* K1.3 takes a
different path on us. The next breakthrough: FS-UAE has a save state
(`.uss`) format that captures full chip RAM + slow RAM + CPU regs +
chipset registers. Booted WB1.3 in FS-UAE 3.2.35 with the exact same
hardware config (A500 + 68000 + 512 KB chip + 512 KB slow + our
unencrypted K1.3 ROM + WB1.3 ADF), got to the idle desktop, saved
state.

Wrote `tools/fsuae_state.py` to parse the .uss:
- FOURCC chunk walker
- `CRAM`/`BRAM` chunks are zlib-compressed (8-byte header: flags + size, then `78 9c`-prefixed zlib)
- `CPU ` chunk has a +4-byte offset between FLAGS and D0 vs the textbook `save_cpu()` order in `fsuae-src/newcpu.cpp` (discovered empirically by searching for known register $C0040C)
- `CHIP` chunk has the same +4 prefix before chipset_mask; chipset_mask at +$04, BLTDDAT at +$08, DMACONR at +$0A, INTENAR at +$24, INTREQR at +$26

Output is `regs.txt` + `mem.hex` + `slow.hex` in the format our existing
`cosim-window` snap directory uses.

Side-by-side regs at IDLE STOP showed a striking pattern:

| reg | matches? |
|---|---|
| A0/A3/A4/A6/A7 + INTENA | ✓ exactly |
| D2-D7, A1/A2/A5, USP, INTREQ, DMACON, SR | divergent |

The MATCHING values are the long-lived system pointers (ExecBase,
IntVect base, library bases). The DIVERGING values are working state
(task pointers, BCPL globals, stack frames). That alone is a sharp
clue: K1.3 has taken a different control-flow path through IRQ
servicing or task dispatch.

## §13. The smoking gun — IRQ dispatcher never installed

Two specific findings nailed the upstream cause:

**1. DMACON BPLEN+SPREN bits.** Ours $02D0, FS-UAE $03F0. The
divergent bits are bit 8 (BPLEN = bitplane DMA) and bit 5 (SPREN =
sprite DMA). FS-UAE has Workbench's display DMA active; we don't.
A grep through our 30 M-cycle trace for any CPU write to `$DFF096`
with the BPLEN bit set returns **zero matches**. Our K1.3 never even
*attempts* to turn on bitplane DMA.

**2. Chip RAM $C0-$FF and slow RAM $C096DC.** A diff of chip RAM
between us and FS-UAE shows 96.3% of longwords match. The first
divergent region is $C0-$FF (the 68k user-defined interrupt vector
table area, vectors 48-63). On FS-UAE, this holds 16 consecutive
pointers `$00C096DC, $00C096DE, $00C096E0, ... $00C096FA` — each 2
bytes apart. On us, all 16 entries are `$00000000`.

Looking at slow RAM $C096DC on FS-UAE:

```
$C096dc: $6120 $611e  ; BSR.B +$20, BSR.B +$1E
$C096e0: $611c $611a  ; BSR.B +$1C, BSR.B +$1A
... (continuing pattern, each BSR.B with decrementing displacement)
$C096fa: $6102        ; BSR.B +$02 (last entry)
$C096fc: $4e71        ; NOP (end marker)
```

These ARE 68k instructions. A series of `BSR.B +disp` with
decrementing displacement — classic 16-entry IRQ-server dispatcher
table. Each chip-RAM $C0+4N pointer points at the Nth `BSR.B`,
which jumps to a common handler tail.

On us, slow RAM $C096DC area is all zeros. K1.3 never builds the
dispatcher.

Added the `[INTVECS-WR]` hw_watch to `rtl/m68k_bus.v` for chip RAM
$C0-$FF. Confirmed: our K1.3 issues exactly 16 writes to that range
at boot start, all `wdata=$0` (exec.library zero-clear), then never
writes back. Zero writes to slow RAM $C096DC ever.

**The K1.3 IRQ-server dispatcher install code path is entirely
skipped on our system.** That's why BPLEN never enables (the IRQ-driven
graphics.library / intuition.library bootstrap can't progress past
the point where it needs the dispatcher), and consequently why K1.3
hits the fallback branch that issues the corrupting line-blit.

This unifies several previously-deferred mysteries:
- Task #51 (framebuffer black, BPLEN=0)
- Task #56 (Intuition screen never opens)
- Task #87 (Copper not executing Workbench list)
- The corrupting blit chain ending at $5E40

They were all downstream symptoms of one upstream cause: the IRQ
dispatcher install routine is gated by a check that fails on our
emulator and silently bypasses the install. Until that check is
identified and unblocked, none of the downstream issues can be
fixed at their actual source.

## §14. Updated techniques (additions to §7)

K. **Real-emulator state diff is the bisection of last resort.** When
   internal traces don't pinpoint a divergence, dump full state from
   a known-good emulator (FS-UAE/WinUAE save state is the easiest)
   and diff against ours at a comparable moment. Most chip RAM will
   match; the small divergent regions are where the K1.3 state
   machine took a different branch. The smallest, earliest divergence
   (e.g. chip $C0-$FF) is often more telling than the biggest one
   (e.g. the 14 KB BCPL-scratch difference).

L. **Save-state binary formats are rarely documented; reverse them
   empirically.** When `tools/fsuae_state.py` first ran with the
   textbook `save_cpu()` offsets, the parsed register values were all
   off by one slot. The fix was to grep the raw chunk bytes for a
   known register value (`$00C0040C` for A0) and back-calculate the
   offset. The +4 prefix this revealed is a FS-UAE 3.x quirk that
   isn't in the source comments. Once one offset is known, the rest
   follow by structure.

M. **A line-mode blit's address generation depends on more than
   bltdpt + bltdmod.** When we saw 80 blitter writes to $5E40 on
   our system and zero on FS-UAE for the same K1.3 ROM, the
   first instinct was "blitter address bug." Wrong: the cosim and
   the FS-UAE state proved the blit *parameters* differ between
   us and FS-UAE — K1.3 never issues this blit on FS-UAE at all.
   The blitter is correct; K1.3 is taking a different branch.

N. **Workarounds buy diagnostic time but mask the real problem.**
   `+define+BLT_VECTABLE_GUARD` neutralizes the LINEF storm from the
   secondary bad blit at $1FF, letting WB1.3 boot reach productive
   idle. That's *valuable* — without it, half our trace is LINEF
   churn. But every time we say "the boot reaches productive idle
   now" we should be ready to say "yes, with this specific symptom
   masked, and the actual cause still pending." The guard helps us
   see further, not work better.

O. **D-pipeline (1-cycle delay) is real Amiga blitter behavior we
   were missing.** FS-UAE's `blitter_dofast()` (lines 546-563)
   latches the D destination pointer into a local, then issues the
   actual bus write on the NEXT inner-loop iteration. Final word's
   write deferred to post-loop. We didn't model this. Now landed
   in `rtl/chipset/blitter.v` via `d_pipe_valid` + `d_pipe_addr`
   + `d_pipe_data` registers; 141/141 unit tests still pass. The
   pipeline didn't fix the $5E40 corruption (source and dest in
   that specific blit don't overlap in a way the pipeline timing
   affects), but it's correct behavior we should have had from the
   start.

## §15. Suggested next steps (revising §10)

The §10 list of UI-side hooks is now superseded by the upstream
finding. The right next move:

1. **Disassemble K1.3 ROM for the IRQ-dispatcher install routine.**
   Search for `MOVE.L An, $C0.L`-style writes (= installs into the
   chip-RAM $C0-$FF table) or for the AllocMem-then-write-BSRs
   routine that creates the dispatcher trampoline at slow RAM
   $C096DC. The install instruction's PC reveals which code path
   is being skipped.

2. **Identify the gate.** The install routine is almost certainly
   conditional on a chipset probe, a resource RESINIT call, or a
   memory probe. Whatever returns a different value on us vs FS-UAE
   is the actual bug to fix.

3. **Possible quick-bypass test:** synthetically pre-populate chip
   $C0-$FF + slow $C096DC at boot via a forced write before K1.3
   runs (similar to how OVL_RESET=1 seeds reset vectors). If the
   downstream Workbench-init then proceeds and BPLEN gets enabled,
   the dispatcher being absent IS the root cause. If not, there's
   another gate we still haven't found.

The pieces of the puzzle now fit. The exact ROM disassembly is the
work that's left.

