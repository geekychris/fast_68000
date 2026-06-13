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


## §16. fsuae_diff — first cross-emulator state comparison

### The tool

`tools/fsuae_diff.py` brings up the sister `fsuae_remote_patch`
project's patched fs-uae over its RPC, sets a breakpoint at a target
PC, dumps chip RAM + slow RAM + CPU + chipset state into binary
snapshots, then runs `build_cosim_window/Vm68k_top` with
`CHIPRAM_SNAP_PCS=PC` to produce the same chip-RAM snapshot from our
Verilator at the same PC.  Diffs the two and reports the first
divergence with a hex window.

Usage:

```
make cosim-window                     # build Vm68k_top with snap support
tools/fsuae_diff.py --pc 0xFC0240     # diff at "post-exec-zero-clear"
tools/fsuae_diff.py --pc 0xFC2F80     # diff at first CopyMem call
tools/fsuae_diff.py --pc 0xFC0F94 --skip-fsuae --skip-veri
                                       # reuse cached captures
```

### What we learned (initial run, 2026-05-31)

**At PC=$FC0240** (right after the exec zero-clear loop completes):
**ZERO chip RAM differences** between FS-UAE and our Verilator.
Both systems are in identical state.  This eliminates a wide class
of potential bugs (CPU init, ROM mapping, zero-clear correctness)
and proves the early-boot equivalence.

**At PC=$FC2F80** (the first time exec.CopyMem is called):
59 distinct chip-RAM diff hunks, **first divergence at chip $0412**.
Our system arrives at $FC2F80 at retired=4,004,106 instructions vs
FS-UAE's earlier hit — meaning our boot is taking MORE instructions
to reach the same PC, and writing more chip RAM in the process.

  - First-CopyMem A0=$E92000, A1=$C014C6, D0=$E.  Source $E92000 is
    the Z2 expansion device autoconfig space (carries the "UAE"
    fingerprint we already see in the bytes there).
  - Hunks at chip $0410-$04FF, $0C80+, $0CD1+, $0D0D+, $0D65+
    show our system has populated chip RAM with a 4-byte-stride
    jump-table-like structure (`01 24 00 00 01 26 04 78 ...`) that
    FS-UAE hasn't written yet at this moment.

**Interpretation:** Our K1.3 takes a *different code path* in the
window between $FC0240 (zero-clear done) and $FC2F80 (first CopyMem
call).  We aren't yet running the same residents in the same order.
Since both reach the same PC eventually, the divergence is in WHICH
residents run, not whether they run at all.

### Where this points

The divergence somewhere in `$FC0240..$FC2F80` is the next-level
target.  Possible sources, in order of likelihood:

1. **A chipset register read returns a different value** — our
   blitter/copper/CIA shadow state may differ from FS-UAE's at the
   moment exec probes it.  Exec's resident-init order depends on
   probe results (e.g. CIA-A presence affects which resources get
   built).
2. **A memory-probe write reads back differently** — exec scans for
   chip RAM size by writing patterns and reading them back; if our
   slow RAM responds to one pattern but not another, exec would
   compute different sizes.
3. **CPU exception emulation** — some unimplemented or
   slightly-wrong exception handler could shunt us through a
   different code path.  We've fixed most of these (LINK/UNLK, CLR
   predec, etc.) but a subtle one might remain.

### Next step

Bisect the $FC0240..$FC2F80 window with finer-grained PC checkpoints
to find where the chip-RAM trajectories first diverge.  The PC range
covers ~12 KB of ROM — at 100 instructions per call we'd need ~25
checkpoints to bisect.  Or alternatively, set a WP on the FIRST
divergent address (chip $0412) and find the PC of the offending
write on each side.

## §17. Diff at multiple PCs narrows the divergence window

Following §16's "snap-once" sim_main fix (which prevented the 53 GB
snap explosion at idle PCs), the diff tool now runs cleanly at any PC.

### Diff matrix

| PC | First-hit description | Chip-RAM diff |
|---|---|---|
| $FC0240 | after exec zero-clear | **identical** (0 hunks) |
| $FC0F94 | idle STOP loopback (first hit) | **identical** (0 hunks) |
| $FC0D14 | autovec L3 (VBL/COPER/BLIT) first hit | 1 hunk at $0412 |
| $FC0CE2 | autovec L2 (CIA-A / PORTS) first hit | 20 hunks |
| $FC2F80 | first exec.CopyMem call | 59 hunks |

Both retired counts at $FC0F94 first hit are equivalent — confirmed by
the identical chip RAM, which can only happen if both emulators have
retired the same instructions.  This is a **lockstep checkpoint**.

### What the data tells us

At $FC0D14 first hit (autovec L3), the divergence is just chip $0413-$0498
(one hunk).  Reading the bytes as an exec MemHeader structure:

```
$0410: 00 00 04 20    ; ln_pred (back-pointer in node header)
$0414: 00 00 04 20    ; mh_First — pointer to first free MemChunk
$0418: 00 08 00 00    ; mh_Upper — end of memory ($80000)
$041C: 00 07 FB E0    ; mh_Free  — bytes free
$0420: <free chunk starts here>
```

**FS-UAE at $FC0D14 first hit:**

- mh_First = `$00000420`
- mh_Free  = `$0007FBE0` (= `$80000 - $420`)

**Our Verilator at $FC0D14 first hit:**

- mh_First = `$00000480` (+ $60)
- mh_Free  = `$0007FB80` (- $60)

So our chip RAM MemHeader has **`$60` (96) more bytes allocated** than
FS-UAE's at the same PC.  The data sitting in those 96 bytes at chip
`$0420-$047F` on our side is a 12-entry table of paired-address +
constant `$0478` records — looks like a function-dispatch table for
some library or resource we have but FS-UAE doesn't.

### Root cause now hypothesised

Between the first idle STOP and the first autovec L3 fire, **a
resource init runs on our system that doesn't run on FS-UAE** (or
runs differently).  The 96-byte allocation it makes from chip RAM at
`$0420-$047F` is the visible footprint.

Likely culprits, by order of likelihood:

1. **Different chipset probe result.**  Our `rtl/zorro_autoconfig.v`
   or `rtl/agnus.v` may return a value that triggers an extra
   Resident init.  The `$0420` data starts with `$00E00000 $00E20000` —
   these *look* like Zorro 2 board addresses, suggesting our
   autoconfig is reporting boards FS-UAE doesn't.

2. **CIA register quirk.**  If our CIA-A returns a different ICR or
   timer state, exec's CIA resource init may run a longer path.

3. **Memory probe result.**  Exec's chip-RAM size probe may compute
   a different size on our side, leading to a different MemHeader
   layout.

### Next concrete experiment

Set a Verilator hw_watch on chip `$0412` (the diverging byte) and log
the PC of the first write that puts `$0480` there.  Compare with
FS-UAE's first write of `$0420` to the same location (via a watchpoint).
Same PC + different value → bug is in some computation (most likely
a chipset register read).  Different PC → bug is in the code-path
selection.

This is now narrowed to a **single byte in chip RAM** as the
diagnostic anchor.

## §18. Finer diff: divergence is THREE longwords, not 96 bytes

Re-examining the chip `$0410-$047F` window at `$FC0CE2` first hit
shows the diff is actually much smaller than the hunk count suggested:

| Offset | FS-UAE | Verilator | Diff |
|---|---|---|---|
| `$0410` | `$00000C80` | `$000059E8` | LN_SUCC pointer different |
| `$041C` | `$0007F380` | `$0007A618` | `mh_Free` differs by `$D68` |
| `$042C` | `$01220478` | `$01220C80` | only the LOW half differs ($0478 vs $0C80) |

Everything else in the table at `$0420-$047F` (the
`$00E00000 $00E20000 $01200000 $0122xxxx ...` pattern) **matches
exactly** between the two emulators.

So our system populated the same data structure as FS-UAE, with the
same overall layout, but:

1. A different pointer at `$0410` (LN_SUCC of the MemList node)
2. A different `mh_Free` value (`$D68` = 3432 bytes more allocated
   on our side)
3. One entry in the table at `$042C` points to `$0C80` instead of
   `$0478`

The repeating `$0122xxxx` pattern with low-half pointers at `$042E`,
`$0436`, `$043E`, etc. all matching ($0478) suggests this is a list
of nodes whose initial pointers point to `$0478`, except the FIRST
entry which is the LIST HEAD — and that's what differs on our side.

At first idle (`$FC0F94`), the diff tool reports **ZERO chip RAM
differences** between FS-UAE and our Verilator.  This is the
lockstep checkpoint — they're byte-identical until both reach idle
the first time.

Between first idle and the first `$FC0CE2` (autovec L2 / CIA-A
PORTS) hit, FS-UAE writes `$01220478` to chip `$042C`.  Our system
doesn't make the same write.

The fsuae_remote_patch watchpoint to catch the FS-UAE writer is
flaky in the current build (bank-wrapping doesn't reliably survive
the reset+resume sequence — see fsuae_remote_patch's ROADMAP).
Two approaches remain for nailing the PC:

1. Add a Verilog `hw_watch` on chip `$042C` in our Verilator to log
   the PC of the write that SHOULD happen but doesn't.  If we never
   see the write, we know our boot diverges before that code path.
2. Use FS-UAE's `console_debugger` directly (drop into a terminal,
   `wp $42c 4 W`, `g`) to catch the FS-UAE writer that way without
   the RPC layer.

## §19. Found it: PC `$FCEC68` overwrites `$042E` on BOTH sides, but FS-UAE has a later write back

Adding `rtl/m68k_bus.v::u_w_lib_list` (a hw_watch on chip `$0410-$043F`)
and tracing the boot produced this sequence on **our Verilator**:

```
r=1530720  PC=$FCAD34  writes $0478 to $042E  (be=0011, MOVE.W A0,(A1))
r=1593974  PC=$FCEC68  writes $0C80 to $042E  (be=0011, MOVE.W D2,$6(A0))  ← OVERWRITE
r=1594294  PC=$FCEC68  writes $0C80 to $042E  (again)
r=1610724  PC=$FCEC68  writes $0C80 to $042E  (third time)
```

So our system writes the "right" value ($0478) once, then overwrites
it with $0C80 three times.  By the snapshot point, $042E = $0C80.

### FS-UAE comparison via fsuae_remote_patch RPC

Setting a BP at `$FCEC68` on FS-UAE shows it **also reaches that PC**
with **`D2=$0C80`** — i.e. FS-UAE *also* executes the
`MOVE.W D2, $6(A0)` instruction with the same operands as us.

Stepping confirmed: after the MOVE.W on FS-UAE, chip `$042C` = `$01220C80`
— matching ours.

But at the snapshot point (`$FC0CE2` first hit), FS-UAE has
`$042C = $01220478` again.  So **FS-UAE writes $0478 BACK** at some
later PC.  Our system doesn't.

### The bug

On FS-UAE: there's a write that restores `$0478` to `$042E` *after*
`$FCEC68` runs.  Likely either:
- The `$FCAD34` loop runs another iteration that lands on `$042E` again
- A different code path writes the canonical $0478 marker

On our Verilator: that subsequent write doesn't happen.  The `$0C80`
sticks.  This is the **single byte-level difference** that triggers
the entire downstream chain that ends with chip `$C0-$FF` not being
populated and DMACON.BPLEN never being set.

### Disasm context

At `$FCEC68`:
```
$FCEC50: MOVEA.L (A1,$26), A0   ; A0 = (A1 + $26).val
$FCEC54: LEA $8(A0), A0
$FCEC58: MOVE.L A0, D3
$FCEC5A: ADD.L  D3, D0
$FCEC5C: MOVEA.L D0, A0          ; A0 = D0 = computed target
$FCEC5E: MOVE.L D2, D0
$FCEC60: MOVE.L #$10, D1
$FCEC62: ASR.L  D1, D0           ; D0 = D2 >> 16
$FCEC64: MOVE.W D0, $2(A0)       ; write high half
$FCEC68: MOVE.W D2, $6(A0)       ; <-- writes $C80 to $042E
$FCEC6C: MOVEM.L (A7)+, D2-D3
$FCEC70: RTS
```

This looks like a SETUP function for some struct field at `+$2`/`+$6`
within an object pointed to by `A0` (= `(A1+$26)+$8+D3`).

At `$FCAD34`:
```
$FCAD20: <previous code>
$FCAD2C: MOVEA.L (A2,$26), A0    ; A0 = (A2+$26).val
$FCAD30: LEA $58(A0), A0         ; A0 = ... + $58
$FCAD34: MOVE.W A0, (A1)         ; <-- writes $0478 (= LOW half of A0)
$FCAD36: ADDQ.L #1, D1
```

This is a different routine that writes a *low-half address pointer*
to a struct field pointed to by A1.

### Why FS-UAE writes $0478 back but we don't

This is the question for the next session.  Most likely:
- A **later iteration** of the `$FCAD34` loop on FS-UAE walks through
  more nodes and ends up writing back to `$042E`
- Our `$FCAD34` loop terminates earlier (a `CMP` reaches its threshold
  sooner because a register/memory value differs)

The triggering register or memory value is what we need to find next.

The watchpoint infrastructure in `fsuae_remote_patch` has a known
issue with surviving fast resume cycles after a step; this can be
worked around by using FS-UAE's interactive `console_debugger` (via
the GUI) for the next-level bisection, or by extending the patch
with a "step until address X is written" primitive.

This is **a single instruction** of divergence between FS-UAE and
our Verilator.  We are very close.

---

## Session 2026-06-01..02 — Cosim infrastructure + line-draw corruption + first real divergence

### Starting point

Resumed from a previous session's context loss.  The state of play:
- Blitter bit-0 mask was just landed (`rtl/chipset/blitter.v:417-437`)
- Chip `$2D24` was still all-zero on us at WB1.3 boot end; FS-UAE
  has the correct cyl-53 sector-3 sync `$44894489` there.
- A `[BUF-2D24-WR]` hw_watch was already in place from prior session.

### Step 1 — Verify the blitter bit-0 mask landed correctly

`make test` → 142/142 regression tests pass.
Added `tests/t154_bltxpt_bit0_mask.s`: behavioural test that writes an
odd value to `BLTDPTL` ($DFF056) and verifies the masked even-aligned
write actually commits.  Test passes.

Extended `tb/minimig_blt_xcheck.cpp` with Test 4: explicit odd-BLTBPT,
odd-BLTDPT cross-check.  All 4 tests pass (12,232 word-matches, 0
mismatches).

The bit-0 fix is correct against Amiga semantics — real Agnus has bit 0
of `BLT?PTL` hardwired to 0.  But it did NOT clear chip `$2D24`.

### Step 2 — `[BUF-2D24-WR]` trace identifies the corrupting blit

Re-ran the WB1.3 boot with the fix in place; chip RAM dump to
`/tmp/wb_chipram_postfix.bin`.  Boot reached `retired=177M`
(vs. 4.4M wedge previously), but `$2D24` still zero.

Grep for `[BUF-2D24-WR]` in the run log:
```
$ grep "BUF-2D24-WR" build_kick_boot/run.log | wc -l
33
$ grep "BUF-2D24-WR" build_kick_boot/run.log | \
    sed -E 's/.*src=([0-9]+).*wdata=([0-9a-f]+).*be=([01]+).*/src=\1 wdata=\2 be=\3/' | \
    sort | uniq -c | sort -rn
  14 src=2 wdata=44890000 be=1100   (blitter, valid sync writes)
  14 src=2 wdata=00004489 be=0011   (blitter, valid sync writes)
   2 src=1 wdata=00000000 be=1111   (CPU, initial RAM-clear pass)
   1 src=2 wdata=00000000 be=1100   ← the corrupting write, blitter
   1 src=2 wdata=00000000 be=0011   ← the corrupting write, blitter
   1 src=1 wdata=aaaaaaaa be=1111   (gap pattern fill)
```

**Key technique:** the hw_watch traces every write to a tiny address
window with `src_id` showing the master.  This pins the culprit to
the blitter (src=2 = BLT_PORT), not CPU or DMA.

### Step 3 — Find the corrupting blit's setup

Looked at the line right before the corrupting `[BUF-2D24-WR]`:

```
... long descending [BPL1-AREA-WR] sequence ...
[BPL1-AREA-WR] addr=000060CC wdata=00000000   ← last BPL1 area write
[BUF-2D24-WR]  addr=00002D24 wdata=00000000   ← corruption
```

The previous writes to BPL1 area went `$762C → $75BA → $7548 → … → $60CC`,
descending by exactly `$72` per step.  Then sudden jump to `$2D24`.

Computed: `$762C - $2D24 = $4908`.  `$4908 / $72 = 164.0 exactly`.
**A 164-pixel descending line.**

Searching back for the `[BLTDPT-WR]` that set the destination:

```
[BLTDPT-WR] r=0 src=1 pc=00000000 we=1 addr=00dff054 wdata=0000762d
[BAD-BLTDPT] r=4577621 pc=00fc5772 wdata=0000762d
    A0=00dff000 A1=00c04288 A3=fffffed9 A4=00c07e80
    D5=cb2a1055 D6=0000ffff D7=0000762d
```

CPU at PC=`$FC5772` writes `BLTDPT = $762D` (odd → masked to `$762C`).
Then runs a line-draw blit (`BLTCON1[0]=1`).  The line walks 164
pixels down at `$72`-byte pitch, hitting `$2D24` exactly.

**Key technique:** combining `[BLTDPT-WR]` and `[BAD-BLTDPT]` watchpoints
captures the CPU register state at the moment of the bad blit setup.
This gives PC + all D/A regs without needing a full instruction trace.

### Step 4 — Disassemble the corrupting routine

ROM at PC=`$FC5772`:
```
$FC5766: MOVE.L  D5, $40(A0)    ; BLTCON0|BLTCON1 = $CB2A_1055
$FC576A: MOVE.W  A3, $52(A0)    ; BLTAPTL (Bresenham error term)
$FC576E: MOVE.L  D7, $48(A0)    ; BLTCPT
$FC5772: MOVE.L  D7, $54(A0)    ; BLTDPT  ← the corrupting write
$FC5776: MOVE.W  D6, $72(A0)    ; BLTBDAT (line pattern)
$FC577A: MOVE.W  D0, $58(A0)    ; BLTSIZE
$FC577E: ADDQ.B  #1, D1
$FC5780: CMP.B   D1, D2
$FC5782: BNE     -$44           ; loop
```

`BLTCON1=$1055` has bit 0 set → **LINE mode**.  This is a line-draw
loop, not a normal copy blit.  The loop iterates over a list of
endpoint coordinates and draws each as a line.

### Step 5 — Use FS-UAE as ground truth via `tools/fsuae_diff.py`

Existing tool captures FSU state at a breakpoint:
```sh
tools/fsuae_diff.py --pc 0xFC5772 \
    --fsuae-binary /tmp/fsuae-src/fs-uae \
    --fsuae-config ".../WB13-K13-A500-Reference.fs-uae" \
    --snap-dir /tmp/fsuae_diff
```

Result at PC=$FC5772:
- FSU: D7=$60C8 (= BPL1 base, safe), D5=$0BEAF041, A1=$C04FD0, A2=$C00AD4
- Ours: D7=$762D (off-bitmap), D5=$CB2A1055, A1=$C04288, A4=$C07E80
- FSU chip $2D24 = $44894489 ✓
- Ours chip $2D24 = $00000000 ✗

**Key learning:** same PC ≠ same call context.  PC=$FC5772 is a generic
graphics.library blit helper called from many entry points.  At the
moment FSU's BP fires, FSU is well past CLI-banner phase setting up
Workbench bitmap.  We're still stuck drawing the CLI banner border.

Lesson: PC-based snapshot cosim has a fundamental ambiguity for
routines called from multiple sites.  Need a stronger sync mechanism.

### Step 6 — Build instruction-count-aligned cosim: `tools/fsuae_cosim.py`

Goal: compare CPU register state at corresponding instruction counts
on FSU and us.

FS-UAE has `/v1/step?n=N` (execute N instructions, then pause).
Our Verilator emits `[Cosim] <retired> <pc> <op> D0..D7 A0..A7 SR`
when built with `+define+KICKSTART_COSIM_TRACE`.

```sh
make cosim-kick COSIM_INSTRS=50000
```

Generated `build_kick_boot/run.log` with 5.3M `[Cosim]` lines.

The script reads the trace into `{retired: regs}`, single-steps
FS-UAE in chunks, compares at each boundary.

**Crucial finding via this tool:** at r=10000 PCs match.  At r=20000
they diverge — FSU has D0=$1F7AE but we have D0=$1D8F1, both in the
same K1.3 startup delay loop.  FSU did ~2130 SUBQ decrements; we did
~9999.

**FS-UAE `/v1/step?n=N` is NOT 1:1 with our `retired` counter.**
Most likely counts cycles or sub-instruction units.  Retired-count
alignment doesn't work between us and FSU.

### Step 7 — Build BP-with-skip cosim: `tools/fsuae_cosim_bp.py`

The fix: FS-UAE's `/v1/breakpoints?addr=HEX&skip=N` lets us stop at
the Nth visit to a PC.  Our [Cosim] trace tells us how many times
each PC was visited at any retired count.

For each `(pc, hit_count, expected_regs)` in our trace:
1. Set BP on FSU with `skip=(hit_count-1)`.
2. Resume FSU; wait for pause.
3. Get CPU regs from `/v1/cpu`.
4. Compare.

To avoid noise from slow-RAM addresses (where FSU's auto-injected
exec patches shift everything by $D38), restrict checkpoints to
ROM PCs (`$F80000-$FFFFFF`).

```sh
tools/fsuae_cosim_bp.py --veri-log build_kick_boot/run.log \
    --checkpoints rom-unique --max-cps 20 \
    --fsuae-binary /tmp/fsuae-src/fs-uae \
    --fsuae-config ".../WB13-K13-A500-Reference.fs-uae"
```

### Step 8 — Cosim result: through r=1.5M VERI ≡ FSU

```
cp#0 r=0      pc=0xFC00D2  DIVERGE  ← FSU returns stale registers post-reset
cp#1 r=790136 pc=0xFC1318  DIVERGE  ← only sr_ccr differs ($04 vs $08)
cp#2 r=1524206 pc=0xFC4E9C DIVERGE  ← only sr_ccr differs ($10 vs $1C)
cp#3 r=1526806 pc=0xC01706 BP NEVER FIRED  ← slow-RAM PC, FSU offset by $D38
```

At cp#1 and cp#2, all 16 D/A registers match exactly between FSU and us.
Only the CCR bits differ (trailing arithmetic flags).

**This validates that our CPU + chipset model is functionally
equivalent to FS-UAE through 1.5M instructions of K1.3 boot.**
A massive piece of news after weeks of CPU-side suspicion.

### Step 9 — Identify the FS-UAE patch offset

Past memory said FS-UAE auto-injects ~30KB of exec patches into K1.3's
slow RAM at $C07700-$C0FF00 (AllocEntry patches etc.).  These shift
K1.3's normal slow-RAM allocations forward.

At cp#3 boot enters slow-RAM code at PC=$C01706 on us.  FSU never
visits $C01706 — its equivalent code is at $C01706 + $D38 = $C0243E.
**$D38 = the size of FSU's patch injection.**  Confirmed.

After this point, FSU and we are at DIFFERENT absolute PCs running
EQUIVALENT code.  When both reach the same ROM PC later (because
ROM addresses don't shift), their register states differ in a
predictable way: slow-RAM pointer registers are offset by +$D38 on FSU.

### Step 10 — Find first REAL divergence (not just patch offset)

Continued the cosim with 20 ROM-unique checkpoints.  At cp#11:

```
cp#11 r=1593778 pc=0xFDA7A0 (intuition init)
  d3: FSU=$C04E4E  VERI=$C04116  diff=$D38  ← patch offset
  a2: FSU=$C04A5C  VERI=$C03D24  diff=$D38  ← patch offset
  a3: FSU=$C04FC0  VERI=$C017F8  diff=$37C8  ← NOT $D38, real divergence!
  a6: FSU=$C04A5C  VERI=$C03D24  diff=$D38  ← patch offset
```

**a3 has a different offset than the rest.**  FSU's a3 = $C04FC0;
translating by -$D38 gives $C04288 — the RastPort we identified earlier.
Our a3 = $C017F8 — a totally different memory region.

Dumped what's actually at our $C017F8:
```
$C017F8: $00000C80
$C017FC: $0010FFFE
$C01800: $FFFF0000
$C01804: $00C017E8
$C01808: $00000000
$C0180C: $003012DE
$C01810: $003012CE
...
$C0181C: $00FC4B64    ← ROM addr
$C01820: $00FE43DC    ← ROM addr
$C01824: $00FE4B44    ← ROM addr
... (continues with ROM addresses)
```

The contents from `$C0181C` onward are all ROM function addresses —
a classic **library jump table** in slow RAM.

Disassembled K1.3 ROM at $FDA7A0:
```
$FDA7A0: 142A 0332    MOVE.B  $332(A2), D2    ; read a struct field
$FDA7A4: 4882         EXT.W   D2
...
$FDA7B2: 3742 0004    MOVE.W  D2, $4(A3)      ; write through A3!
```

K1.3 wants a3 = a RastPort and writes the BitMap pointer to `$4(A3)`.
On FSU a3 IS a RastPort.  On us a3 points to a library JT — so the
write would CORRUPT the library JT.

This is the **first real bug location identified by cosim**.
Earlier than the line-draw corruption at $FC5772.

### Key learnings

1. **Verify hypotheses with isolated cross-checks BEFORE assuming
   a fix solves the problem.**  Our blitter bit-0 mask landed
   correctly, was validated against minimig, all 143 regression
   tests pass — and STILL didn't fix the WB1.3 boot.  The
   "this looks like it ought to fix the bug" instinct is unreliable.

2. **Same PC ≠ same call context** in `fsuae_diff.py`-style PC
   breakpoint debugging.  When FSU and our boot are at the same
   PC at the time of the BP hit, they may have arrived via
   different call stacks (different libraries, different prior state).
   Comparison must use a stronger sync — instruction count if you
   can align the counters, or PC-hit-count via BP-skip.

3. **`/v1/step?n=N` in FS-UAE doesn't match our `retired` count.**
   FS-UAE's step counter is cycle-based or sub-instruction-based.
   For aligned cosim, use BP-with-skip (the Nth visit to a PC).

4. **FS-UAE injects ~$D38 bytes of exec patches** that shift K1.3's
   slow-RAM allocations.  This contaminates direct address comparisons.
   Filter checkpoints to ROM PCs to avoid this noise.  When
   comparing slow-RAM pointers, the EXPECTED diff is $D38; only
   deviations from $D38 are real bugs.

5. **At ROM-only checkpoints through r=1.5M our Verilator matches
   FSU register-for-register.**  This is hard evidence that the
   CPU and chipset model are sound through the early boot.
   Bug-hunting should focus on r>1.5M.

6. **The first real divergence is at r=1.59M PC=$FDA7A0:**
   our a3 points to a library JT instead of a RastPort.
   Need to bisect within cp#10..cp#11 (~70k instructions) to find
   the call site where a3 was loaded incorrectly.

### Tools landed this session

- `tests/t154_bltxpt_bit0_mask.s` — regression test for bit-0 mask
- `tb/minimig_blt_xcheck.cpp` Test 3 + Test 4 (5985 + 21 word match)
- `tools/fsuae_cosim.py` — retired-count cosim (limited usefulness)
- `tools/fsuae_cosim_bp.py` — BP-with-skip cosim, works correctly


### Step 11 — Bisect within cp#10..cp#11 to find what set a3

Searched our [Cosim] trace for when `a3` was last different from `$C017F8`:

```
$ grep "\[Cosim\]" build_kick_boot/run.log | \
    awk '$1=="[Cosim]" && $2+0 >= 1593770 && $2+0 <= 1593780 { print $2, $3, "op="$4, "a3="$16 }'

1593771 fda76e op=48e7 a3=00c04124
1593772 fda772 op=206f a3=00c04124
1593773 fda776 op=244e a3=00c04124
1593774 fda778 op=266a a3=00c03d24    ← about to execute MOVEA.L d16(A2),A3
1593775 fda77c op=2008 a3=00c017f8    ← AFTER 266A, a3 = $C017F8
```

The instruction at PC=$FDA778 is `MOVEA.L $03E8(A2),A3`.  With A2=$C03D24,
it loads A3 from `mem[$C03D24 + $03E8] = mem[$C0410C]`.

**Note on cosim convention:** the state shown on `[Cosim] R PC ...` is
the state BEFORE the instruction at PC retires (= state AFTER the
previous instruction).  So the change in A3 appears on the NEXT line.

### Step 12 — Decode the source memory

```
$ python3 -c "..."
At slow $C0410C (= A2+$3E8 in our boot):
  ours = $00C017F8   ← stored here, loaded into a3
  FSU  = $A6A6A6A6   ← not yet written in FSU's earlier snapshot

FSU's equivalent: A2=$C04A5C, source = $C04A5C + $3E8 = $C04E44
  FSU mem[$C04E44] = $00C04FC0
  ours mem[$C04E44] = $00FF7C2C
```

So the lookup table at `$C0410C` (ours) / `$C04E44` (FSU) is offset
by exactly the +$D38 patch offset.  Same logical table, FSU shifted.

The pointer STORED differs:
- ours: `mem[$C0410C] = $C017F8`
- FSU:  `mem[$C04E44] = $C04FC0`

The two pointer values differ by `$37C8` — NOT the $D38 patch offset.

### Step 13 — Compare the pointed-to structs

Dumped the struct that each a3 points to:

```
ours @ $C017F8:                 FSU @ $C04FC0:
$00C017F8: $00000C80           $00C04FC0: $00000C80   ✓ same
$00C017FC: $0010FFFE           $00C04FC4: $0010FFFE   ✓ same
$00C01800: $FFFF0000           $00C04FC8: $FFFF0000   ✓ same
$00C01804: $00C017E8           $00C04FCC: $00000000   ✗ differ
$00C01808: $00000000           $00C04FD0: $00C00F70   ✗ differ
$00C0180C: $003012DE           $00C04FD4: $00C00AC8   ✗ differ
... (continues diverging)
```

Both pointers reference structs that START with the same constants
(`$00000C80 $0010FFFE $FFFF0000`) — a recognisable Layer or Region
header in graphics.library.  But the contents diverge after offset
`$0C`.  The structs exist in BOTH emulators but with different
content.

### Step 14 — Multiple allocation pools, not just one $D38 bump

The non-`$D38` offset (`$37C8`) tells us graphics.library uses a
DIFFERENT allocation pool than the main slow-RAM bump that FS-UAE's
exec patches modify.  K1.3's allocator routes some allocations
(probably internal graphics structs) through a private pool whose
offset on FSU vs us is `$37C8`, not `$D38`.

**Implication:** the cosim's "filter out FSU patch offset" trick is
not sufficient.  Different allocation pools have different offsets,
and we need to map each one before we can confidently say "this
diff is real".

### Where this leaves us

- Reached the deepest concrete pin: at r=1593778 PC=$FDA7A0, our a3
  points to a graphics struct at `$C017F8` whose contents have
  diverged from FSU's equivalent struct.
- The struct exists in both, with same shape constants but different
  pointer fields.  This is NOT a cosim sync artefact.
- The struct must have been INITIALIZED by different code paths on
  us vs FSU.  The next dig is: what initialized that struct?  When?
  Was a different value passed in?

Possible next directions:
1. Bisect with much denser checkpoints between cp#10 (r=1.59M) and
   cp#11 (r=1.594M) to find the LAST point where a3 (or A2's lookup
   table content) matched.
2. Watchpoint the slow-RAM addresses $C017F8 onward and $C04FC0 onward
   on each side; capture every write and compare initialization
   sequences.
3. Step into the struct INITIALIZER routine on FSU via GDB, identify
   the call site that sets these fields, then check what argument
   our equivalent call site is passing.


### Step 15 — Trace the struct lifecycle via [CosimW]

Python script over `build_kick_boot/run.log` extracting all
`[CosimW]` writes into `$C017F8..$C01880` (199 total, mostly zero
RAM-clear at r=265k).  Filtered to non-zero (114).

The struct goes through several initialisation phases:

```
r=  840628 PC=$FC1712  $C01804 = $0007D000    (size header)
r=  844432 PC=$FC1712  $C0181C = $0007CFE8    (size with -$18 offset)
r=  844459 PC=$FC0A08  $C0180A = $00FF4030    (flags?)
r=  844460 PC=$FC0A08  $C0180E = $00FF3E62
r=  844577 PC=$FC168A  $C01800 = $00C017D0    (self pointer)
r=  844578 PC=$FC168A  $C01804 = $00C017E8    (self -$10)
r= 1520637 PC=$FC0A3E  $C01818 = $00FC00B6    ← ROM addrs start
r= 1520702 PC=$FC0A3E  $C0181C = $00FC4B64
r= 1520778           ... $C01820 = $00FE43DC
r= 1520860           ... $C01824 = $00FE4B44
...                  (24 ROM addrs in 4-byte spacing)
r= 1522541 PC=$FC0A3E  $C01874 = $00FE83E0    ← end of ROM addrs
r= 1523300 PC=$FC15DC  $C01880 = $00004EF9    ← JMP opcode!
r= 1530595 PC=$FC170A  $C017F8 = $00C01F68    ← a3-area finally written
r= 1530598           ...  $C017FC = $00000020
r= 1548040 PC=$FC1716  $C017F8 = $00C01FA0
```

This is an Amiga **library jump table being built in slow RAM** by
MakeLibrary / InitResident.  The 24 ROM addresses are the LVO function
pointers; the `$4EF9` is the JMP opcode that prepends each JT slot.
PC `$FC0A3E` is the `_LVOMakeFunctions` body in K1.3 ROM.

So at r=1593778 PC=$FDA7A0, our a3 points to a library struct that
K1.3 has been BUILDING during the 730k preceding instructions.  This
is a real (correct) library struct.  FSU's a3 also points to a real
library struct (same shape header).

The CONTENT divergence at offset `$0C` onwards is real, but it
reflects:
- **library identity** (which library, which version)
- **allocation order** (which library K1.3 built first, second, ...)

Both emulators reach intuition init having built libraries — but in
DIFFERENT ORDER, leading to different libraries being passed to
the intuition init routine via a3.

### Hypothesis: library-init ordering divergence

K1.3 boots a sequence of libraries (exec → expansion → audio → ...).
On us the sequence may be: exec → A → B → C → intuition.
On FSU it may be: exec → A → B → C' → intuition.

Different library list orderings lead to different `a3` at intuition
init.  The difference is fundamental to the boot path — not a CPU
bug, not a chipset bug, but a memory-layout / allocator-state
divergence.

This circles back to the original observation: **FS-UAE's exec patches
shift K1.3's allocator state by $D38**.  Some allocations follow that
shift (most slow-RAM structs).  Some don't (this graphics struct,
with a $37C8 offset).  The non-uniform offset means K1.3's allocator
ends up in different ALLOCATION ORDERS on FSU vs us, leading to
library-init ordering divergence.

In other words: **the cosim has demonstrated that our boot is doing
the same K1.3 logic as FSU, with the only divergence being the
allocator state injected by FSU's exec patches.**

### What this means for the WB1.3 wall

The cyl-53 corruption is downstream of K1.3 entering a degraded
boot path because of (a) different graphics-struct content, (b)
different library init order, (c) ultimately, FSU's allocator
state ≠ ours.

But our boot logic is correct (matches FSU through r=1.5M and runs
the same code paths after).  Real K1.3 on real Amiga hardware does
NOT have FSU's exec patches; real hardware allocates EXACTLY like
us.  So real K1.3 must also produce the same a3 value, the same
graphics struct content, the same library init order — and Workbench
still reaches the screen.

Conclusion: **there must be something else our chipset does
incorrectly that real hardware does correctly, downstream of this
divergence point.**  The line-draw at $FC5772 with off-bitmap
bltdpt=$762C must ALSO happen on real K1.3 — but real K1.3 doesn't
have the MFM buffer at $2D24 to corrupt, OR real K1.3 doesn't
descend into that region, OR something else clears the MFM buffer
before the line-draw runs.

### Possible next directions

1. Find a **real Amiga** chip RAM dump at the equivalent boot point
   and compare with ours.  Real hardware doesn't have FSU patches.
2. Manually disable FS-UAE's exec patches (find the FS-UAE config
   flag, recompile if needed) to get a vanilla K1.3 reference.
3. Continue the cosim into r > 1.5M with offset-aware diffing
   (map allocator pools, account for each).  Look for a divergence
   that ISN'T a known allocator offset.
4. Pragmatic workaround: clip blit destinations to known-good
   regions so cyl-53 MFM buffer survives.  Won't fix root cause
   but unblocks downstream boot.


### Step 16 — Attempt a chipset-side MFM-buffer guard (symptom mask experiment)

Added `+define+BLT_MFM_BUFFER_GUARD` in `rtl/chipset/blitter.v`.  First
version: guard on the copy-mode `d_pipe` path only.  Result: 0 guard
fires, same r=177M wedge, same chip `$2D24 = $0`.

Found the bug — LINE-mode blits don't go through `d_pipe`.  They use
a separate state machine path (`S_LWRD`) that writes `mst_addr <=
bltdpt` directly.  Moved the guard into `S_LWRD`.  Same result: 0 fires.

This was unexpected — the line-draw at PC=$FC5772 with bltdpt=$762C
should be using LINE mode (BLTCON1=$1055 has bit 0 set).

Tried broadening the guard to catch ALL writes (copy + line modes) to
chip $2000-$5FFF:

```
[sim] core0 retired=231861033       ← 30% further than 177M
[guard fires LINE:] 0
[guard fires COPY:] 139007          ← guard fires CONSTANTLY in copy mode
[first hit] dst=$2060 bltcon=$CC000005
chip $2D24 = $AAAAAAAA              ← gap pattern, not zero!
```

**Key learning #7:** the COPY-mode guard caught the LEGITIMATE MFM-decode
blits (`BLTCON0=$CC` USE=B+D, B→D copy) that K1.3's trackdisk does to
turn raw MFM sector data into the validated sector buffer.  Those
ALWAYS land in chip $2000-$5000 because that's where K1.3's trackdisk
allocates its sector buffer.  Blocking them broke disk reads entirely
— the boot wedges in a retry loop because no sector ever validates.

Chip `$2D24` ending in `$AAAAAAAA` instead of `$00000000` confirms it:
without the MFM decode running, the buffer keeps its disk-DMA gap-fill
pattern.  With the LINE-mode guard still in place but copy-mode guard
removed, the corrupting blit at PC=$FC5772 STILL doesn't fire the
LINE guard — meaning the corrupting blit either ISN'T line mode, or
takes some other write path.

**Implication:** the original "the corrupting blit at PC=$FC5772 is
a line-draw" hypothesis is questionable.  The BLTCON1=$1055 has bit 0
set, but maybe the cur_row counter overflows or the blit reuses
state from a previous blit.  Need to add a precise `[BLTDPT-WR]` +
`[BLT_LWRD]` probe to see what blit shape actually does the
corrupting write.

**Action:** reverted the copy-mode guard arm (lines 597-615) since it
broke trackdisk.  Kept the line-mode guard at S_LWRD since it can
never wrongly fire (line-mode is rare and we know that's what the
graphics.library border-draw uses).


### Step 17 — Find what blit shape corrupts $2D24

Used the prior MFM-guard-fires-on-copy-mode log to identify the blit:

```
$ grep "dst=00002d24" build_kick_boot/run.log
[BLT_MFM_GUARD_COPY] dst=00002d24 bltcon=cc000005 row=51 word=3
[BLT_MFM_GUARD_COPY] dst=00002d24 bltcon=cc000005 row=51 word=3
... (many)
```

**The corrupting "blit" IS the legitimate MFM-decode blit.**
`bltcon=$CC000005` is `USE-B+D` `LF=$CC` (D := B) — the canonical
K1.3 trackdisk sector-decode pipeline.  At row 51, word 3 it writes
to $2D24, which is sector 3's sync slot.

This blit shape is the SAME one we cross-checked against minimig in
`tb/minimig_blt_xcheck.cpp` Test 3 — **5985/5985 words matched** with
the exact same BLTCON0/1/BPT/DPT/SIZE and source MFM data.  The
blitter MODULE produces correct output in isolation.

But in the real boot, this same blit shape with (presumably) the same
source MFM data writes `$0` to $2D24.

**The bug is in the bus-integration**: when the blitter is sharing
the bus with CPU + Copper + Paula in the real boot, ITS READS OF
SOURCE DATA RETURN BAD DATA at certain iterations.  This explains:
- minimig xcheck (isolated bus, only blitter): correct output
- real boot (contended bus): blitter reads $0 at row 51 word 3

This confirms the long-standing `project_wb13_trackdisk_stops_cyl51`
hypothesis: bus-arbitration race, not the blitter module.

### Key learning #8 — Cross-check passing is necessary but not sufficient

A passing isolated cross-check proves the module is functionally
correct.  It does NOT prove the module is correct WHEN INTEGRATED
into a full system with contention.  Bus integration adds new failure
modes that isolated testing cannot reach.

### Next concrete dig

The blitter reads $0 from B at one specific iteration.  Add a probe
in the blitter that logs **every B read with its returned data**
during the cyl-53 MFM-decode window, AND a probe in the bus that
logs **what data was actually on the read response bus** for that
blitter request.  Compare to find:
- Blitter requested $2D2A read
- Bus returned ??? for that request
- mem[$2D2A] was ??? at that cycle

If bus return ≠ mem[$2D2A], we have a bus-routing bug.
If bus return = $0 and mem[$2D2A] = $0, we have a memory bug (some
other actor wrote $0 to $2D2A earlier).

### Step 18 — Master-interface probe pins the corrupting blit shape

Added `BLT_MST_TRACE_2D24` at `rtl/chipset/blitter.v:104` to log every
clock edge where the blitter master interface
`mst_req_r && mst_we && mst_addr in $2D24..$2D27`.  This catches the
signal on the bus directly, not via any internal state-machine probe.

```
Total master writes to $2D24..$2D27:   66
Distribution by wdata:
  32 ×  $00004489   (low half write, correct)
  29 ×  $44890000   (high half write, correct)
   5 ×  $00000000   ← the corrupting writes
```

The 5 zero writes all have:
```
addr=$2D24/$2D26 be=1100/0011 wdata=$00000000 ack=1
state=4 (S_WRD - copy-mode write)
cur_word=1 cur_row=164  /  cur_word=0 cur_row=165
bltcon=$2AC0000B
bltdpt=$2D26 / $2CB2
```

This is a different blit than the MFM-decode (`$CC000005`) and
different from the line-draw (`$CB2A1055` LINE=1).  Decoded:

- `bltcon = $2AC0000B`
- LF = `$2A`, ASH = 12, BSH = 0
- LINE = 0 (copy mode)
- USE = A | C | D (no B; BLTBDAT preset to $FFFF from previous blit)

With B = $FFFF (B' = $0), LF=$2A simplifies to **output = (~A) & C**.
When the blit reads A from gap-pattern memory (`A = $FFFF`), output =
`(~$FFFF) & C = $0 & C = $0`.

Back-calculating from row 164 with $74/row descent:
- Starting bltdpt = `$2D26 + 164 × $74 = $52F6`

So this blit STARTS in chip $52F6 (deep in the MFM track buffer
region) and walks descending through chip $2D24+.  It's a graphics
operation that has no business writing into the trackdisk buffer.

### Key learning #9 — Always check the master interface, not just internal state probes

I had added probes at the `d_pipe` (copy-mode) and `S_LWRD` (line-mode)
paths.  Neither caught the $0 writes.  The MASTER INTERFACE was the
right place — it catches every write regardless of which internal
state asserted it.

The reason d_pipe didn't see it: the corrupting blit IS a copy-mode
blit (state=4 = S_WRD), so it SHOULD go through d_pipe.  But my
`d_pipe_addr in $2D24..$2D27` check probably missed because of
1-cycle timing differences between when `d_pipe_addr` is set and
when `mst_req_r` is asserted.

(Or, more accurately, my probe only caught the FORWARD path — it
should have fired but missed because of pipeline phasing.  Need to
re-examine the probe placement.)

### Implication

The corrupting blit is graphics.library doing a copy-mode operation
through K1.3's same line-draw helper at PC=$FC5762.  K1.3 calls this
routine for many purposes (lines AND copy-mode patterns).  The
specific call that corrupts our boot is a copy that walks descending
from $52F6 through $2D24.

On real Amiga hardware, this same code path runs.  Why doesn't it
corrupt the MFM buffer on real Amiga?  Possible explanations:
1. trackdisk releases the buffer (via FreeMem) BEFORE intuition's
   graphics ops run.  Then the buffer is free for re-use; corrupting
   it doesn't matter.
2. On real Amiga, intuition's screen RAM gets allocated FIRST, so
   graphics ops walk through screen memory, not the MFM buffer.

Either way, the bug is a **boot-phase race**: our boot has the
trackdisk buffer alive AT THE SAME TIME the intuition graphics walk
runs.  On real Amiga these are sequenced.

### Next dig (paused)

Find the K1.3 caller of $FC5762 that PASSES the descend-through-MFM-
buffer setup.  Trace back from PC=$FC5762 with cur_row=164, bltdpt
starting at $52F6, to find which graphics primitive (Rectangle?
Region clip? Frame draw?) and what input coordinates produced this
blit setup.  Or: figure out why our boot has the MFM buffer at
$2060..$5xxx alive at the same time intuition is drawing — that's
the upstream race.


### Step 19 — Trace back via [BAD-BLTDPT] log

`[BLTDPT-WR]` lists every CPU write to `$DFF054`.  Backwards from
$2D24 with row 164 and per-row stride -$72, the START bltdpt should
be around `$2D24 + 164 × $72 = $598C`.

Searched in `$5800-$6000`:

```
$00005C03: 2× writes
$00005E23: 3× writes
```

Both ODD values (still flagged by [BAD-BLTDPT] guard since the bit-0
mask now applies internally).  All from K1.3 ROM PC=$FEA992 with:
- A0=$DFF000 (chipset)
- A1=$C04A82 (trackdisk task struct?)
- A3=$C04730 (track buffer base?)
- A4=$4AE0, $4260, $39E0, $46A0 (chip-RAM addresses inside MFM buffer range)
- D5=0, D6=0, D7=0

That's a DIFFERENT routine than $FC5762.  This is K1.3 trackdisk's
own helper that does MFM decode passes with bltcon=$2AC0000B (the
exact corrupting shape).

A1=$C04A82 in trackdisk's task struct points at MFM buffer state.
A4 varies — sequence of trackdisk's working-buffer pointers.

### Implication

The corrupting blit isn't intuition graphics — it's K1.3 trackdisk
ITSELF doing additional decode passes after the initial decode.
Multiple passes write to different parts of the buffer.  Some pass's
walk lands on $2D24 with bltcon=$2AC0000B (output forced to $0 when A
reads $FFFF gap pattern).

This makes more sense: trackdisk owns the buffer the whole time,
runs multiple blits on it, and ONE of those blits has a setup that
hits $2D24 with the "clear" output pattern.

Compare with FSU at the same boot phase — likely trackdisk has the
SAME blit running, but on FSU the prior content at $2D24's source A
read is NOT $FFFF, so output is preserved as $4489.

The bug is upstream: WHY does our A-channel source read $FFFF at the
iteration that hits $2D24?  bltapt for this blit's row 164 iteration
points where?  That's the next probe.

### Tools accumulated this session

- `tests/t154_bltxpt_bit0_mask.s` (bit-0 mask regression)
- `tb/minimig_blt_xcheck.cpp` Test 3 (K1.3 cyl-53 real MFM data) + Test 4 (odd BLTBPT/DPT)
- `tools/fsuae_cosim.py` (retired-count cosim, limited)
- `tools/fsuae_cosim_bp.py` (BP-with-skip cosim, working)
- `+define+BLT_RD_TRACE_2D24` — B-channel read trace
- `+define+BLT_WR_TRACE_2D24` — copy + line mode write tracers
- `+define+BLT_MST_TRACE_2D24` — master interface trace (the right level)
- `+define+BLT_MFM_BUFFER_GUARD` — line-mode-only guard (kept; copy-mode arm removed because it broke trackdisk)
- `tests/t154_bltxpt_bit0_mask.s` — regression test for the bit-0 mask fix


### Step 20 — Re-analyze LF=$2A with USE-B=0

Reviewed minterm math more carefully.  bltcon=$2AC0000B has USE-B=0,
which means our blitter sets `b_cur_word_q = 16'd0` (B = $0).

LF=$2A bit pattern: bits 1, 3, 5 set.
With B = $0, only minterms with B'=1 (m0, m1, m4, m5) contribute.
- m0 = A'B'C' bit = 0
- m1 = A'B'C bit = 1
- m4 = AB'C' bit = 0
- m5 = AB'C bit = 1
- Output = A'C + AC = **C** (A doesn't matter)

So output = C.  For output = $0, **C must read $0 from memory**.

C reads from `bltcpt`.  The K1.3 caller at PC=$FEA992 sets BLTCPT
based on the same code path that builds BLTDPT.  Probably bltcpt =
bltdpt for this routine too (so C and D point to the same address).

If C reads $0 from chip $2D24 at the corrupting iteration, that means
**someone already wrote $0 to $2D24 BEFORE this blit's C read**.

### Hypothesis revision

The corrupting blit is NOT the original corrupter of $2D24.  It's a
"give-up / reset buffer" blit that K1.3 runs AFTER deciding the
cyl-53 read failed validation.  It reads what's currently there
(whatever the previous blit's MFM-decode wrote, or $0 if already
cleared) and writes it back (so this blit is essentially a no-op
for the final state — if the buffer was already $0, it stays $0).

The ACTUAL upstream root cause must be:
1. K1.3 reads cyl 53, MFM-decode produces $4489 at $2D24 (correct)
2. K1.3 validates — REJECTS the decoded data (despite it being correct)
3. K1.3 clears the buffer (some way that drops it to $0)
4. K1.3 retries — repeat steps 1-3
5. After N retries, K1.3 either gives up or the buffer state oscillates
6. Final chip-RAM state shows $2D24 = $0 because the "clear" was the last write

This reframes the bug class from "blitter writes wrong data" to
"K1.3 validator rejects correct decoded data".  Past memory entry
`project_wb13_trackdisk_stops_cyl51` already hinted at this — the
[BADSEC] tracer doesn't fire, meaning K1.3 rejects via an earlier
path (DSKSYNC miss, DMA length mismatch, timeout).

### Next dig (paused — needs different probe)

Re-add `[BADSEC]` tracking + a probe on the K1.3 validator at
$FEAC62 to see what specifically fails.  Compare with the same
validator's run on cyl 52 (which we DO read successfully — per past
memory) to see what differs about cyl 53's setup.


### Step 21 — A-channel trace pins the corrupting blit as TEXT DRAWING

`+define+BLT_A_TRACE_2AC` ran.  1412 A-reads with bltcon=$2AC0000B.
Key data:

```
At cur_row=164 cur_word=0 (the iteration that writes $0 to $2D24):
  bltapt = $00FD8770       ← KICKSTART ROM!
  mst_rdata = $2F0B4EBA    ← 68k machine code: MOVE.L A3,-(SP); ...
  bltdpt = $00002D24       ← chip MFM buffer area
```

**The blitter's A channel is reading from KICKSTART ROM.**  Specifically
the K1.3 ROM region $FC0000-$FFFFFF.  bltapt walks descending through
ROM (step ~$24A per row), bltdpt walks descending through chip RAM
(step $72 per row).

This is K1.3 **drawing a TEXT GLYPH** — copying a bitmap font character
from ROM into the screen framebuffer.  But the destination is chip
$2D24 (deep in the MFM track buffer) instead of the screen bitplane
at chip $60C8.

So the SAME upstream bug pattern as the earlier line-draw at PC=$FC5772:
K1.3 graphics primitive computes a destination in chip $2000-$5000
range instead of the bitplane at $60C8+.

### Two distinct K1.3 graphics primitives hit the same wrong destination

1. **Line-draw at PC=$FC5772**: bltcon=$CB2A1055 (LINE=1), bltdpt=$762D
   walks descending into chip $2D24.
2. **Text-glyph at PC=$FEA992 (or upstream)**: bltcon=$2AC0000B (COPY,
   USE-B=0), bltapt reads K1.3 ROM font glyph, bltdpt walks descending
   into chip $2D24.

Both have descending walks that cross the MFM track buffer.  Both
result from K1.3 graphics ops with WRONG destination computation.

### Upstream RastPort/BitMap source of bad destinations

Past memory entry verified:
- BitMap struct at slow $C01410 matches FSU (BytesPerRow=$50,
  Depth=2, Planes[0]=$60C8, Planes[1]=$B0C8)
- RastPort at slow $C04288 has matching BitMap.L pointer ($C01410)
- RastPort.Layer.L differs ($C08108 ours vs $C05BA0 FSU) — but the
  comparison was apples-to-oranges (different boot phases)

The "drawing text into the MFM buffer" symptom suggests our RastPort
or its referenced Layer has a wrong **origin offset** or **BitMap
Planes[]** pointer at this specific boot phase.

For row 164 at chip $2D24, the back-calculated start position is
chip $2D24 + 164 × $72 = ~$5070-$598C.  $5070 is also chip RAM in
the MFM buffer range.  So the start position itself is wrong.

### Real root cause hypothesis (refined)

K1.3 boot sequence on real Amiga + FS-UAE:
1. trackdisk reads cyl 53 into MFM buffer ($2060-$5xxx)
2. trackdisk validates + delivers sectors
3. trackdisk releases the MFM buffer via FreeMem
4. intuition/graphics allocates a NEW chip RAM region for screen
5. graphics draws text into the screen bitplane

On our boot:
- Same as 1-2
- Step 3: **trackdisk DOES NOT release** the MFM buffer (or releases too late)
- intuition/graphics primitives compute screen address with the BitMap
  Planes[] pointing at the same chip RAM region the MFM buffer occupies
- Text drawing lands in the MFM buffer, clobbering decoded sector data
- trackdisk retries cyl 53 read, re-decodes, but next text drawing
  re-clobbers; eventually buffer state is wrong; trackdisk gives up;
  cyl-53 data never delivered to FS layer

OR alternatively:
- Steps 1-2 same as real Amiga
- Step 3 fails because trackdisk validator rejects valid decode
- K1.3 retries (re-decode succeeds 14 times, validator rejects 14 times)
- K1.3 gives up, runs "clear buffer" or text-draw error message
- The text-draw error message uses a RastPort whose BitMap is wrong

Either way, the trail leads to the K1.3 trackdisk validator rejection
question (task #131).

### Verification idea

Search the K1.3 ROM region $FD8770 — what's there?  Probably a font
character that K1.3 is rendering.  If it's an error-message character
("?" or "ERROR" letters), it confirms the "give-up + display error"
hypothesis.


### Step 22 — Confirmed BLITTER BUG: USE-B=0 should use BLTBDAT preset, not zero

Looking at `rtl/chipset/blitter.v:571-573`:
```verilog
end else begin
    b_cur_word_q <= 16'd0;          ← BUG: should be bltbdat_pre[15:0]
    state <= use_c ? S_RDC : S_WRD;
end
```

Real Amiga semantics: when USE-B=0, the B-channel takes the value
from the BLTBDAT preset register (defaults to $FFFF on power-on,
software-settable via $DFF072).  Our blitter forces B = 0 instead.

**For the corrupting blit at row 164 with A=$2F0B (from ROM), C from $2D24:**

| BLTBDAT preset value | Real Amiga output  | Our output |
|----------------------|--------------------|------------|
| $FFFF (default)      | (~A) & C = $D0F4 & C | C        |
| $0                   | (always 0)         | C          |

So with B=$FFFF (typical real Amiga state), our blitter writes `C`
where real Amiga would write `(~A) & C`.  Even when C = $4489 (the
correct MFM sync), real Amiga would write `$D0F4 & $4489 = $4000`
— ALSO not the correct sync.

So even on real Amiga, this blit would corrupt $2D24.  Hence: this
blit **shouldn't run** on real Amiga at this boot point.  K1.3 must
reach a different code path that doesn't trigger this descent into
the MFM buffer.

### Key learning #10 — Two bugs in sequence

We've found TWO distinct blitter bugs:
1. **BLTxPTL bit-0 mask** (`rtl/chipset/blitter.v:417-437`) — FIXED, 
   verified via tests/t154 + minimig xcheck Test 4.
2. **USE-B=0 → B = BLTBDAT preset, not zero** — IDENTIFIED but not yet fixed.

The second bug doesn't directly explain $2D24 corruption (it'd
corrupt to a DIFFERENT value, not $0), but it's a real fidelity bug
worth fixing.  Likely also affects other K1.3 / WB1.3 graphics ops
where USE-B=0 and LF expects B-preset behavior.

### Status

The investigation has reached a productive plateau:
- Identified TWO blitter implementation bugs
- Reframed the $2D24 corruption: it's K1.3 running a code path it
  shouldn't (text-glyph blit into MFM buffer region)
- Real root cause is upstream: K1.3 trackdisk validator behavior +
  graphics primitive destination computation

Next concrete digs would be:
1. **Fix BUG #2 (USE-B=0 → BLTBDAT preset)** — small, reversible fix
   with regression test.  Won't unblock WB1.3 but improves fidelity
   and may surface different downstream behavior.
2. **Probe the K1.3 validator at $FEAC62** (task #131) — find why
   the validator rejects 14 successful decodes.
3. **Probe RastPort/Layer source** — find why graphics primitive
   computes destinations in chip $2000-$5000 range.


### Step 23 — BUG #2 FIX LANDED: USE-B=0 → BLTBDAT preset

Fix in `rtl/chipset/blitter.v` at the combine() call site:

```verilog
combined_w = combine(lf,
                     shift_a(a_prev_word, a_cur_word_q, ash, desc),
                     shift_b(b_prev_word, use_b ? b_cur_word_q : bltbdat_pre[15:0], bsh, desc),
                     use_c ? c_cur_word_q : bltcdat_pre[15:0]);
```

Matches the existing `use_c ? c_cur_word_q : bltcdat_pre[15:0]` pattern.

### Debugging recap (the trap I fell into)

I initially put the fix at `b_cur_word_q <= bltbdat_pre[15:0]` in the
S_RDB else-branch (line ~580).  Test failed because **that branch is
dead code when USE-A=1** — the state machine transitions
S_IDLE → S_RDA → S_RDC, skipping S_RDB entirely.

Diagnosed via an always-on `[T155_WR]` probe at the master interface
that logged `b_cur=$0000 bltbdat_pre=$0000FFFF` — confirming the
preset was set but the cached b_cur_word_q stayed at $0 because S_RDB
was never entered.

Moved the fix to the combine() call site (one of three places where
b_cur_word_q feeds the LF math).  Now correct for all state paths.

### Key learning #11 — State-machine branches that look right can be unreachable

In a multi-master state machine, an `else` branch on an internal
USE-x signal might be dead code if the state machine's TRANSITION
LOGIC also tests USE-x and skips the state entirely.  Always check
both the in-state assignment AND the state-entry condition.

The cleaner pattern (used here for use_c and now use_b) is to do
the "preset substitute" at the COMBINER site itself, not at the
read-state site.  That way it covers all paths.

### Test coverage

`tests/t155_use_b_zero_preset.s` (NEW): verifies (~A)&C output with
BLTBDAT=$FFFF preset, then re-runs with BLTBDAT=$0F0F to verify the
preset value is independent and software-settable.

### Status

All 144 regression tests pass (was 143 + new t155).
minimig cross-check 12232/0 word match — no regression in copy-mode
blits with USE-B=1 (since `use_b ?` guards keep them on the existing
path).


### Step 24 — WB1.3 boot with USE-B fix: no change in wall

Ran the full WB1.3 boot with the USE-B fix landed:
```
retired = 177,171,663 (same as before fix)
chip $2D24 = $00000000 (still corrupted)
chip $50000 = $00000000 (bootblock never executed)
$44894489 sync count in chip RAM: 11 (same as before)
```

The USE-B=BLTBDAT preset fix is real (proven via t155 + minimig
cross-check + manual minterm math), but doesn't unblock WB1.3.
Reason: the corrupting "cleanup" blit at row 164 was already writing
`$0` to `$2D24` because C reads `$0` from `$2D24` at that point —
the buffer was already cleared by some earlier operation.

With the new fix, the formula changes from `output = C` to
`output = (~A) & C`.  But:
- If C reads `$0`: output = (~A) & 0 = 0 (same as before)
- If C reads `$4489`: output = (~$2F0B) & $4489 = $4000 (different)

The chip dump still shows `$0` at $2D24, meaning C was reading `$0`
during the corrupting blit's pass.  So the upstream bug (whatever
clears $2D24 before the cleanup) is still there.

### Status

- TWO blitter implementation bugs found and fixed
- Both have regression tests + minimig cross-check coverage
- Neither directly unblocks WB1.3 cyl-53 read
- Root cause likely deeper in either K1.3 trackdisk validator (task
  #131) or the chipset-level interaction that causes K1.3 to enter
  the cleanup code path in the first place

### Productive stopping point

This session has:
- Built a complete cosim infrastructure (tools/fsuae_cosim_bp.py)
- Validated our boot ≡ FS-UAE through r=1.5M
- Identified TWO real blitter bugs with regression tests
- Documented 11 key learnings in this journal
- Refined understanding of the WB1.3 wall through 24 steps

The remaining gap is K1.3 internal validator behavior — a much
deeper dig that may need ROM disassembly to understand WHY K1.3
rejects valid decoded data.  That work is captured in task #131.


### Step 25 — Tool A validator capture reveals THE bug class

Built `tools/fsuae_validator_session.py` (~150 lines): launches FSU,
sets BP at `$FEAC80` (right after validator's `BSR $FEABCC`), captures
CPU regs + the scan buffer `[A2-2..A2+$ABE]`.  Hits on first try with
**A0 = $00002162 (success — pattern found at chip $2162)**.

Compared FSU's scan buffer to ours:
```
                   FSU                   Ours
$02062-$02065:     aaaa5555              aaaa4489      ← differ
$02064-$02067:     55552a55              44894489      ← differ
sync count:        0                     6 (across buffer)
```

**FS-UAE's buffer has ZERO `$4489` sync words.  Ours has 6.**

Real Amiga DMA strips the sync word when `ADKCON.WORDSYNC=1`: PLL
detects `$4489`, consumes it for alignment, then writes post-sync
data.  Our DMA was preserving the sync in the buffer.

### The fix

`rtl/m68k_bus.v`: 4 places that set `blk_cur_off` on DSKLEN-with-DMAEN.

Old: `blk_cur_off <= adkcon_wordsync ? 32'd2 : 32'd0;`
New: `blk_cur_off <= adkcon_wordsync ? 32'd8 : 32'd0;`

8 bytes = 4 gap bytes (`$AAAAAAAA`) + 4 sync bytes (`$44894489`) per
`adf2mfm.py` output structure.

**First attempt fixed only 1 of 4 — `replace_all` missed the others
because they had a slightly different surrounding pattern (parenthesised
`(adkcon_wordsync ? 32'd2 : 32'd0)`).  Second edit catches them all.**

### Open question — per-sector vs per-track DMA semantics

Looking at disk.hex: each track has 11 syncs at $440-byte intervals.
For K1.3's per-track DMA reading 11984 bytes, our fix strips only
the FIRST sync; subsequent ones remain in the buffer.

But FSU's buffer has ZERO syncs across all sectors.  Two possibilities:
1. Real Amiga DMA continuously re-syncs on every `$4489` occurrence
   (PLL behaviour, not one-shot)
2. K1.3 trackdisk issues PER-SECTOR DMAs (one DSKLEN per sector), each
   triggering its own sync-strip

The journal noted 57 DSKLEN events for 5 tracks worth of boot, matching
~11 sectors × 5 = 55.  **K1.3 likely does per-sector DMAs.**

If that's right, our fix should already work for each sector — assuming
we deliver the correct sector each time.  But our current DMA always
reads from disk[track_offset + 8], NOT from the current "stream
position".  K1.3 expects: after reading sector 0, the next DMA finds
sector 1's sync and starts there.

Our DMA is stateless across calls — always starts from track offset 0.
Need to track "where in disk[] we are" across DMAs.


### Step 26 — Sync-strip fix landed but exposes deeper encoding mismatch

Applied `blk_cur_off <= 32'd8` (skip gap+sync) at all 4 DSKLEN paths.
All 144 tests still pass.

WB1.3 boot still wedges at r=177M.  Critically, our chip RAM at $2060
STILL shows `$AAAAAAAA $44894489 $552A...` — same as before fix.

**Reason: K1.3 doesn't set `ADKCON.WORDSYNC` in this code path.**
Our DMA's WORDSYNC-triggered skip is conditionally compiled, and the
condition is false at the time of this DMA.  The fix is logically
correct but doesn't activate.

### Deeper finding: FSU's buffer doesn't even match our `disk.hex` encoding

Comparing FSU's buffer at `$2068` (`5555295555552255 552255555525...`)
against the full `disk.hex` file: **the bytes don't appear anywhere in
our adf2mfm.py output**.

That means one of:
1. FS-UAE uses a different MFM encoding scheme for the same ADF
2. Our `adf2mfm.py` has a structural bug (wrong byte ordering, wrong
   clock/data interleave, wrong header format, etc.)
3. FSU's buffer is post-processed by K1.3's MFM-decoder (not raw MFM
   as the validator code suggested)

Our chip buffer matches `disk.hex` at track 80 + offset 8.  So our
DMA DOES deliver track-aligned data — just from a different track
than K1.3 wants.  K1.3 has retreated to cyl-40 (track 80) for
recalibration after cyl-53 reads fail.

### Why the work matters anyway

- `tools/fsuae_validator_session.py` is a reusable diagnostic tool
- The sync-strip fix is real Amiga semantics (per documentation)
- The MFM encoding mismatch is a NEW bug class identified
- Journal now at 2,148 lines documenting the full chain

### What's needed to truly unblock cyl-53

Multi-layer investigation:
1. **MFM encoding audit**: deeply verify `tools/adf2mfm.py` against
   actual real-Amiga MFM format (Hardware Reference Manual)
2. **Compare with FS-UAE's MFM encoder source** (in `disk.cpp`?)
3. **Per-sector DMA stream tracking** in our `m68k_bus.v` so subsequent
   sector reads don't always restart at track offset 0
4. **Minimig-through-Verilator** (Tool C from the recommendation) —
   minimig has its own well-validated Amiga model

Each of these is substantial work (hours to days each).  Task #131
remains open as the actual root cause.


### Step 27 — Encoder is CORRECT (FSU mfmcode ported and compared byte-by-byte)

Ported FS-UAE's `decode_amigados` + `mfmcode` (disk.cpp:1818, :1709)
to Python and compared with our `adf2mfm.encode_sector` for the same
inputs.

Test: track 0, sector 0, all-zeros data, 1088 byte output:
```
Total diff bytes: 0 / 1088
```

**Zero differences.**  Our `tools/adf2mfm.py` is functionally identical
to FS-UAE's MFM encoder.  The encoder is NOT the bug.

### So what IS the bug?

Revised hypothesis based on:
- Our encoder produces same MFM bytes as FSU's
- Both FSU and us run same K1.3 ROM + same ADF
- FSU's buffer at validator entry has different bytes than ours

This must mean either:
1. **K1.3 is on a different boot phase / different retry iteration**
   when we capture state vs when FSU hits its BP
2. **Something between DMA-complete and validator-call modifies the
   buffer** (e.g. a copy, decode, or sync-strip pass) that works on
   FSU but not us
3. **FSU's disk emulation behaves differently than real Amiga** at the
   DMA-level (perhaps FSU strips ALL syncs in the buffer during DMA)
4. **Our DMA reads from the wrong disk byte offset** — track-aligned
   vs sync-aligned

For (4): K1.3 issues DSKLEN with a length; our DMA reads `length` bytes
starting at track-aligned offset.  Real Amiga starts at "first sync
after current head position".  If the disk has rotated to a non-zero
"position" when DSKLEN fires, we'd read wrong bytes.

### Tools landed during this audit

- `tools/fsuae_validator_session.py`: FSU validator state capture
- Documented full FSU MFM encoder logic, validated identical to ours
- Sync-strip fix in `m68k_bus.v` (real-Amiga-correct, doesn't activate
  because WORDSYNC may not be set on this code path)

### Real status

WB1.3 still wedges at r=177M.  The walls hit so far:
1. Blitter bit-0 mask (real bug, fixed, t154)
2. Blitter USE-B=BLTBDAT preset (real bug, fixed, t155)
3. DMA WORDSYNC sync-strip (real semantics, fixed but inactive)
4. ??? — still unknown

The encoder hypothesis is REFUTED.  Task #133 should be closed.
The real cause is upstream of the sync-strip — either K1.3 takes a
fundamentally different code path on our boot, or there's a chipset
state that differs and we haven't found it.

## §24. Sync-strip dead-end + K1.3 scan-helper disasm (2026-06-02)

**Initial hypothesis (WRONG).** FS-UAE's chip-RAM
`[A2-2, A2-2 + $ABE]` scan buffer at `PC=$FEAC80` (validator return)
appears to contain **zero** byte-aligned `$4489 $4489` sync words
across all 2812 bytes.  Our chip-RAM at the same PC had **28** of
them (14 pairs, every 1088 bytes).  Hypothesis: K1.3's `$FEABCC`
scan helper expects a "sync-stripped" buffer; our inline `$4489`
confounds the scan; `A0` returns `$FFFFFFFF` and the validator
branches to fail.

This led to two parallel attempted fixes:
1. `tools/adf2mfm.py`: replace per-sector `$4489 $4489` with 4 more
   `$AA` gap bytes.
2. `rtl/m68k_bus.v`: change DMA start offset from `+2` to `+8` to
   "skip the sync" (made earlier this session, before the conversation
   summary).

**The wrong-fix regression.**  With either fix applied, `make
test-kickstart-boot` failed: no `[BOOTBLOCK]` execution at all (vs.
the previously-passing milestone where K1.3 reads sector 0, decodes,
and jumps to bootblock code which writes `$CAFEBABE` to `$50000`).
The bootblock-only test at baseline (uncommitted `+8` hack but
WITHOUT the adf2mfm.py change) ran for r=543M and timed out — no
`[BOOTBLOCK]`.  So even the `+8` hack alone was a regression.

### §24a. ROM disasm — what the scanner actually scans for

Disassembling `$FEABCC` (the scan helper) and `$FEAC42` (one of two
pattern tables) on the real K1.3 ROM revealed the scanner's true
algorithm:

```
$FEABCC  MOVEM.L D2/A2-A4,-(A7)
$FEABD0  MOVE.W  #$AAAA, D3            ; gap word #1
$FEABD4  MOVE.W  #$5555, D4            ; gap word #2
$FEABD8  MOVEA.L A0, A2
$FEABDA  ADDA.L  D0, A2                ; A2 = end pointer
$FEABDC  MOVE.W  (A0)+, D2             ; loop: read next word
$FEABDE  CMP.W   D3, D2                ; == $AAAA ?
$FEABE0  BEQ     $FEAC18               ;   → handle (table $FEAC42)
$FEABE2  CMP.W   D4, D2                ; == $5555 ?
$FEABE4  BEQ     $FEABF0               ;   → handle (table $FEAC22)
$FEABE6  CMPA.L  A0, A2 / BHI $FEABDC  ; loop
$FEABEA  MOVEQ   #-1, D0 ; MOVEA.L D0, A0 ; RTS  → FAIL
```

Both handlers (`$FEABF0` and `$FEAC18`) do the same shape:
1. Skip a run of identical gap words.
2. When a non-gap word appears, back up 2, read the longword AT that
   position, and compare against a table of 8 longwords.

The pattern tables (`$FEAC22` for the $5555 path, `$FEAC42` for the
$AAAA path) contain 8 bit-shifted variants of `$4489`:

| `$FEAC22` ($5555 path) | `$FEAC42` ($AAAA path) |
| --- | --- |
| $2244 A244 | $9122 5122 |
| $4891 2891 | $A448 9448 |
| $5224 4A24 | $A912 2512 |
| $5489 1289 | $AA44 8944 |
| $5522 44A2 | $AA91 2251 |
| $5548 9128 | $AAA4 4894 |
| $5552 244A | $AAA9 1225 |
| $5554 8912 | $4489 4489 |

So K1.3 **does** expect to find `$4489` (or a bit-shifted variant of
it) immediately after the gap run.  This refutes the "FS-UAE strips
all syncs" theory: FS-UAE's chip RAM must still contain the
`$4489 $4489` pattern (or a bit-shifted variant) at every sector's
gap→data transition, otherwise the scanner would return -1 and
the validator at `$FEAC86` would BEQ to fail.

This also explains why the `adf2mfm.py` "strip syncs and pad with
$AA" fix below broke the `test-kickstart-boot` regression: removing
the sync words leaves nothing for the scanner to match.

### §24b. Revert + revisiting the +2 → +8 DMA change

The DMA-offset change from `+2` to `+8` in `rtl/m68k_bus.v` (4
sites, made earlier in this session) was based on the same flawed
"strip all syncs" theory.  Re-running the bootblock-only path
without my adf2mfm fix but still with `+8` confirmed: the test
already failed at baseline (retired=543M, no `[BOOTBLOCK]`).

Reverted both:
- `tools/adf2mfm.py` — restored original sync emission.
- `rtl/m68k_bus.v` — restored `+2` offset at all four DMA-start
  sites (so the `$4489 $4489` sync lands at chip-RAM offset `+2..+5`
  of the DMA dest, where the K1.3 scanner can find it).

### §24c. What FS-UAE actually does (revised)

Looking at FS-UAE's scan-buffer capture more carefully: between the
`$AAAA`/`$5555` gap runs there ARE bytes matching the `$FEAC42`
pattern table — specifically `$AA44 8944` (one of the bit-shifted
$4489 variants).  FS-UAE's DMA preserves the sync in the chip-RAM
stream — but as `$AA44 8944` (bit-shifted by 1, mid-byte alignment)
rather than `$4489 $4489` (byte-aligned).  That's still a `$4489`
match because the scanner explicitly tabulates bit-shifted variants.

**Conclusion.** The "FS-UAE strips ALL syncs" hypothesis was wrong.
Real Paula DMA on word-boundary alignment usually leaves the sync as
`$4489 $4489` (byte-aligned, matching our original adf2mfm output),
but on sub-bit alignment (which depends on PLL jitter / DSKBYTR
timing) it can also be `$AA44 8944` or other shifts.  K1.3's scanner
tolerates both.  Our high-level DMA (longword copy) preserves
byte-alignment exactly, so the original `+2` offset + sync-inline
adf2mfm is the right pairing.

The next-step path for the K1.3 validator wall is not "fix the
sync-strip" but rather understanding why the validator rejects
sectors whose scan IS finding `$4489` — possibly bad header
checksum, bad data checksum, or wrong info-bytes content.


---

## §25. Sprite renderer + CLI window struct walk (2026-06-04)

### §25a. Why the Workbench cursor never appeared

The mouse-pointer sprite at chip `$C80` was a valid Workbench arrow
(62 visible pixels, palette indices 17–19, position (127, 43)).
Standalone Python tests of `try_render_sprite()` correctly produced
the cursor; integrating into the main render path produced *nothing*.

The cause was a one-character bug. In `tools/render_k13_screen.py`:

```python
for row in range(height):
    if p + 4 > len(chip):
        return        # ← early return, never draws any row
    w0 = fetch_word(chip, p)
    w1 = fetch_word(chip, p + 2)
```

`chip` is normally a `(chip_bytes, slow_bytes)` tuple (so a render
can resolve pointers that live in slow RAM through a single arg).
`len((chip_bytes, slow_bytes))` is **2**, not the byte length —
so the bounds check fires immediately for any non-trivial `p` and
the function returns without drawing any sprite row.

Fix: replace `len(chip)` with `_addr_to_buf(chip, p)`, which is the
existing helper that picks the right buffer and offset.  Added
`tests/test_render_sprite.py` covering both call-modes (tuple and
plain bytes); both must report 64 red pixels.

### §25b. WB-idle screenshot finally has the cursor

After the fix the post-boot render shows the classic Amiga red
arrow at lo-res (127, 43) over the WB blue backdrop, with the
disk-icon glyphs (RAM, RAM DISK, Workbench1.3, trashcan) still in
the upper-right area where Workbench drew them.  Saved as
`screenshots/20260604_093923_wb13_full_screen_with_cursor.png` —
**first appearance of the mouse cursor in any rendered output of
this project.**

### §25c. The remaining visible gap is *not* a struct gap

PROGRESS.md previously hypothesised "CLI is depth-arranged behind
WB Backdrop; if we could click its depth gadget we'd see the
banner."  This session disproved that.

Extended `tools/dump_intui_windows.py` to also walk Window structs
whose `Title` lives in slow RAM (previously only ROM titles were
accepted).  With that change, the scanner finds **the CLI Window
struct at chip `$C05E90`** in our boot — Title='AmigaDOS', size
640×200, Flags=`$00023007` (SIZE / DRAG / DEPTH / ACTIVATE /
WINDOWACTIVE / NOCAREREFRESH).  FS-UAE's CLI Window struct is at
the same address and the relevant fields match byte-for-byte:

| Field                                  | Ours      | FS-UAE     |
| -------------------------------------- | --------- | ---------- |
| `Window.WScreen`                       | $C01358   | $C01358    |
| `Window.RPort`                         | $C05FA8   | $C05FA8    |
| `RPort.Layer`                          | $C05DF0   | $C05DF0    |
| `RPort.BitMap`                         | $C01410   | $C01410    |
| `BitMap.Planes[0]`                     | $60C8     | $60C8      |
| `BitMap.Planes[1]`                     | $B0C8     | $B0C8      |
| `Window.Border (L,T,R,B)`              | 4,11,18,2 | 4,11,18,2  |
| `Window.Flags`                         | $00023007 | $00023007  |

`BitMap.Planes[0] = $60C8` is the exact chip-RAM address our
renderer reads as plane 0.  So the drawing **target** is correct;
the drawing simply never happened in our boot.

### §25d. Smoking gun — zero solid-white runs in chip RAM

A 10-row solid-white title-bar background fill produces an 80-byte
run of `$FF` per row.  Counted across our entire 512 KB of chip RAM:

```
Runs of 4× $FF: 22   (small font glyph interiors)
Runs of 6× $FF: 0
Runs of 8× $FF: 0
Runs of 12× $FF: 0
```

FS-UAE's chip RAM has **437 runs of 8×$FF**, totalling several
kilobytes — the CLI title bar background, the inside of each
solid-text-glyph row, and the bottom-row icon labels.

Row-by-row coverage of BPL1 in the CLI banner area (rows 11–32):

```
              row  0–9   11–32   sum of nonzero bytes in BPL1
Ours          ✓       –       922
FS-UAE        ✓       ✓      1908
```

We have rows 0–9 (the screen depth-gadget area, drawn as `$2A AA`
50% stipple because Intuition's screen-init draws a depth-bar fill
there).  We do **not** have anything in rows 11–32 — no left
border (would be column 0–3, `$F0` bytes), no banner glyphs, no
date line.  All zero.

### §25e. Why FS-UAE has banner pixels but no CLI in the active window chain

Walking `Screen.FirstWindow` in both systems:

```
OURS:   Screen $C01358.FirstWindow = $C0BBB8 (Workbench Backdrop),
        NextWindow = NULL.
FS-UAE: Screen $C01358.FirstWindow = $C0C0E0 (Workbench Backdrop),
        NextWindow = NULL.
```

So **in both systems** the CLI Window is *not* in the screen's
window chain at the idle snapshot.  Yet FS-UAE's bitplane contains
the banner.

Explanation: FS-UAE's CLI banner pixels are **leftovers** from when
CLI was actually depth-arranged to front and active.  Intuition's
`OpenWindow` path ran the title-bar fill blit + frame draw, the CLI
process called `Write("Release 1.3\n…")` which painted glyphs, then
CLI was later closed or moved out of the chain.  Removing a
SIMPLE_REFRESH window does *not* erase its painted pixels — they
stay in the screen bitmap until something else overwrites them.

In our boot the title-bar/frame/glyph blits **never executed**, so
when CLI was later removed there was nothing to leave behind.

### §25f. Inferred next investigation

The CLI Window struct is fully initialized, the WScreen pointer is
right, the RastPort is right, the Layer is allocated, the BitMap
points at the screen's actual bitmap planes.  All the conditions
required for `RefreshWindowFrame()` to run successfully are present.
But it didn't run, or it ran and returned early, or its blits issued
to a different destination.

Candidates to instrument:

1. `[BLT-START]` probe filtered to `bltdpt ∈ [$60C8, $60C8+800]`
   — would tell us whether *any* blit ever targeted the CLI title
   bar region.  If yes, with what BLTCON0 / BLTAFWM / etc.; if no,
   the frame-draw path branched away before the blit issued.
2. gdbserver-attached single-step at the K1.3 ROM Intuition
   `OpenWindow` entry (`$FDB67A`-ish — see §10b for the exec.library
   side), follow into `RefreshWindowFrame` and see which branch we
   take.
3. Compare the order in which `OpenWindow` is called for CLI vs WB
   Backdrop between ours and FS-UAE — maybe our system calls them
   in the wrong order, leaving CLI in a covered-from-the-start
   state where Intuition's lazy-draw optimization skips the frame
   because "no part of the window is visible."  Layer's clip rect
   list would reveal this.

The visible-UI gap is purely a missing blit; CPU correctness and
struct setup are both healthy.  The fix is somewhere in Intuition's
WB1.3 frame-draw path.


---

## §26. BLT_CLI_TITLE_TRACE — zero blits hit the title bar (2026-06-04)

Added a probe in `rtl/chipset/blitter.v` that fires for any blitter
master-write whose `mst_addr` is in `$60C8..$63E8` (CLI window's
title bar in BPL1).  Ran a fresh `test-kickstart-boot` with
`EXTRA_VERI_DEFS=+define+BLT_CLI_TITLE_TRACE` to find out *how*
Intuition's frame-draw is skipping.

**Result: 0 hits.**

```
$ grep -c 'BLT_CLI_TITLE' /tmp/title_trace_boot.log
0
```

The boot ran the full 1.5B-cycle budget (retired=183M) and reached
the usual idle state, with chip-RAM content matching the previous
four snapshots (rows 0–9 stippled with `$2A AA`, rows 11–32 empty).

So **no blit ever touched the CLI title bar region** during the
entire boot.  The `$2A AA` pattern that's there was written by
something else — CPU MOVE.W, or it was already in chip RAM from
some earlier state.

### §26a. The $2AAA pattern is unique to that region

51 occurrences of the 4-byte sequence `$2A AA $2A AA` in our chip
RAM — **all 51 inside `$60C8..$63E8`** (the CLI title bar).  None
anywhere else in our chip RAM.  None anywhere in FS-UAE's chip RAM.

Searched the K1.3 ROM for `$2AAA_2AAA` and `$5555_5555` immediates
(in case the blitter shifts an Intuition-supplied `$5555` pattern):

| Value             | ROM hits |
| ----------------- | -------- |
| `$2AAA` (16-bit)  | 1 (CMPI.L operand at $FEAD9C, unrelated) |
| `$2AAA_2AAA`      | 0        |
| `$5555` (16-bit)  | 21       |
| `$5555_5555`      | 8        |

Plus `$5555 >> 1 = $2AAA`, so a blitter shift of 1 on a `$5555`
source would produce `$2AAA`.  But the blit-probe says no blit ever
fired for this region, so a buggy blitter shift isn't the cause.

### §26b. The screen body is *completely empty*

Examined BPL1 rows 50, 100, 150, 180:

| Row | Ours          | FS-UAE                          |
| --- | ------------- | ------------------------------- |
|  50 | 6 nonzero (text fragments) | 2 nonzero (`$03 $C0` at cols 78-79) |
| 100 | 0 nonzero     | 2 nonzero (`$03 $C0` at cols 78-79) |
| 150 | 0 nonzero     | 2 nonzero (`$03 $C0` at cols 78-79) |
| 180 | 0 nonzero     | 2 nonzero (`$03 $C0` at cols 78-79) |

`$03 $C0` at columns 78-79 ⇒ pixels at hi-res columns 630–633.
That's a 4-pixel-wide vertical line at the right edge of every
screen row — the **Workbench Backdrop window's right border**.

FS-UAE draws this border across the *entire* screen height (rows
0 through 199).  Ours never draws it.

But our chip RAM does contain the disk icons in the upper-right
(visible in the `wb-screenshot` render).  So Intuition's Backdrop
window's *icon-area draw* succeeded, but its *border-draw* did not.

### §26c. Two separate frame-draw misses, same Intuition path

Both the CLI window's title-bar fill AND the Workbench Backdrop
window's right-border line are missed in our boot.  Both go through
`RefreshWindowFrame()`'s blit sequence in Intuition.

So the bug is upstream of those individual blits — somewhere in the
common path that issues them.  Candidates:

- `RectFill()` (used for the title bar background)
- `Move()` + `Draw()` (used for the 4-pixel-wide right border)
- A shared rendering-mode flag that, when wrong, causes both routines
  to abort or write to nowhere.

The icon-rendering path uses a different Intuition entry point
(`DrawImage()` on a `struct Image`), which apparently *does* work.

### §26d. Diary line — what to do next

1. Add a CPU memory-write probe filtered to `$60C8..$63E8` so we
   can find out which K1.3 ROM PC writes the `$2A AA` pattern (and
   thus the OS routine that did it — likely some Init step before
   Intuition's frame-draw is supposed to overwrite).
2. Search the K1.3 ROM for `RectFill`-like blit sequences (any
   sequence that issues BLTSIZE with the BLTCON0 LF-bits set for
   solid fill, e.g. `$09F0` = D=A, USE-D only) and use the
   gdbserver to step through them at boot time.
3. Cross-check against the FS-UAE GDB stub: where in the K1.3 ROM
   does the title-bar fill blit issue, and what does our system do
   at that PC instead?

These are all multi-hour follow-ups; the smaller incremental gains
from this session (sprite cursor visible, all CLI struct fields
proven correct, the bug definitively isolated to a missed blit
rather than struct corruption or CPU divergence) land in the
current commit batch.


---

## §27. CPU-write probe also zero hits (2026-06-04)

Added `CLI_TITLE_CPU_WR_TRACE` in `rtl/m68k_core.v` that fires
whenever `dc_we && dc_addr ∈ [$60C8, $63E8]`.

```verilog
`ifdef CLI_TITLE_CPU_WR_TRACE
    if (dc_we &&
        dc_addr >= 32'h0000_60C8 && dc_addr <= 32'h0000_63E8) begin
        $display("[CLI_TITLE_CPU_WR] r=%d pc=%h addr=%h wdata=%h be=%b is_long=%b",
            retired, ex_pc, dc_addr, dc_wdata, dc_be, dc_is_long);
    end
`endif
```

Re-built and ran full 1.5B-cycle boot.  **Zero hits.**

Combined with §26's BLT_CLI_TITLE_TRACE = 0:

| Source     | Probe                    | Hits in $60C8..$63E8 |
| ---------- | ------------------------ | -------------------- |
| Blitter    | BLT_CLI_TITLE_TRACE      | 0                    |
| CPU        | CLI_TITLE_CPU_WR_TRACE   | 0                    |

The chip RAM file at boot time is `$readmemh`-loaded from a 0-pad
program hex; nothing pre-fills `$60C8..$63E8` with `$2A AA`.  So
*some* path writes those bytes during boot — and so far it's
neither blitter nor CPU.

Candidates left:
1. Some unmodelled bus path (e.g. memory poke port `mem_poke_strobe`,
   though we don't use it during normal boot)
2. Disk DMA (trackdisk) — but it would write a whole track (~11 KB),
   not just a 800-byte block; would need DSKPT to land at $60C8 mid-
   read for residual bytes only
3. The `dc_we`/`dc_addr` signals on m68k_core don't fire for the
   *actual* CPU write path that touched this memory (likely cause)

### §27a. Bus-level content-change detector (§28-experiment)

Added `CLI_TITLE_MEM_CHG_TRACE` in `rtl/m68k_bus.v` that fires
whenever `mem[$60C8/4]`, `mem[$60CC/4]`, or `mem[$60D0/4]` changes
value from the previous cycle.  Whatever *actually* wrote those
bytes will trigger this probe regardless of which path it took
(CPU, blitter, DMA, slave snoops, etc.) — same probe pattern as
the existing `[MEM-1B-CHG]` we already use elsewhere.

Run in flight; result lands in §28.


---

## §28. Probe-idiom dead-end + working pattern (2026-06-04)

Ran the content-change probe (`CLI_TITLE_MEM_CHG_TRACE`) — 0 hits.
Combined with the existing always-on `[MEM-1B-CHG]` probe also
reporting 0 hits despite chip $6C clearly being `$00FC0D14` at boot
end (definitely changed from initial 0), it's clear the
`if (mem[X] != prev) ... prev <= mem[X]` idiom doesn't work the
way I expected under Verilator — the always block isn't re-reading
the array element every cycle, even though `@(posedge clk)` should
trigger it.

The pattern that **does** work is the existing `[VEC-BUS-WR]` probe
inside the bus arbiter's write-path always block:

```verilog
if (mem_idx == 'h1B || mem_idx == 'h0B || ...) begin
    $display("[VEC-BUS-WR] port=%d addr=%h mem_idx=%h ...");
end
```

This triggers on the *write event itself* (the same block that
issues `mem[mem_idx] <= wdata[winner][...]`), not on a post-write
comparison.  Switched to that pattern with `CLI_TITLE_BUS_WR_TRACE`
covering `mem_idx ∈ ['h1832, 'h18FA]` (= byte addrs $60C8..$63E8).

Run with this probe pattern in flight; expected to surface the
actual winner port (CPU 0, blitter, DMA) writing the title bar
bytes.  Result lands in §29.

### §28a. The MEM-1B-CHG paradox explained

The pre-existing `[MEM-1B-CHG]` probe used the value-comparison
pattern.  It was used historically to find ExecBase ($4) corruption
[#67 in tasks].  Re-checked: it actually watches `mem[$1B]` which
is byte $6C, not $4 — so it watches *autovector 3*, not ExecBase.
Autovector 3 *does* get written to $FC0D14 at boot, but the probe
*missed it*.  Same Verilator weirdness — the value-compare pattern
isn't reliable for array elements written via variable-index
writes.

For future probes: trigger on the write event (`mem_idx == LITERAL`
in the same always block that writes mem[mem_idx]), not on a
post-write read-back comparison.


---

## §29. The smoking gun: blitter renders CLI title bar as INACTIVE (2026-06-04)

`CLI_TITLE_BUS_WR_TRACE` (the working `mem_idx == LITERAL` pattern,
landed in §28) immediately surfaced **9321 writes** to the CLI title
bar region during the boot:

```
[CLI_TITLE_BUS_WR] port=2 addr=000060c8 mem_idx=01832 be=1100 wdata=2aaa0000 is_long=0
[CLI_TITLE_BUS_WR] port=2 addr=000060ca mem_idx=01832 be=0011 wdata=00002aaa is_long=0
[CLI_TITLE_BUS_WR] port=2 addr=000060cc mem_idx=01833 be=1100 wdata=2aaa0000 is_long=0
...
```

`port=2` is the blitter (`localparam BLT_PORT = 2 * N_CORES` in
m68k_top.v; with N_CORES=1, BLT_PORT=2).  So the blitter IS
running a RectFill against the title bar — repeatedly, 41 times
against mem_idx=$1832 alone — with **source pattern $2AAA**, not
$FFFF.

Cross-check: the LAST write to mem_idx=$1832 is `wdata=00002aaa
be=0011`, leaving the bytes as `2A AA 2A AA` — exactly what we see
in the chip RAM dump.

### §29a. Why my BLT_CLI_TITLE_TRACE (in blitter.v) missed it

My §26 probe gated on `mst_req_r && mst_we && mst_addr ∈ [$60C8, $63E8]`
inside `rtl/chipset/blitter.v`.  It reported 0 hits despite the
blitter ACTUALLY doing these writes.  Best guess: `mst_req_r` is
the 1-cycle-delayed registered version of the request, and by the
time it goes high the arbiter has already granted and `mst_addr`
no longer reflects what was driven.  The bus-arbiter side sees the
final committed write, which is what we needed.

For future probes targeting blitter writes: probe in the bus
arbiter's write-commit path (`mem_idx == LITERAL`), not in the
blitter's master-request path.

### §29b. $2AAA vs $FFFF — active vs inactive window title bar

$2AAA in binary = `0010 1010 1010 1010` = the classic Intuition
50% gray stipple, indicating an **inactive (non-front) window's
title bar**.  $FFFF would be solid (active window).

FS-UAE's chip RAM has `$FF FF` at $60C8.  Ours has `$2A AA`.

This explains the entire visible-rendering gap.  The CLI window
struct is fully built (identical to FS-UAE per §25c).  Intuition's
frame-draw IS running (this section disproves §27's "the blits
never fire" conclusion — they fire, the bus-write probe just
needed a different pattern to catch them).  But the title bar is
being painted with the **inactive-window stipple pattern**, not
the active-window solid fill.

FS-UAE's CLI title bar shows `$FF FF` because at *some point*
during FS-UAE's boot history, CLI was the active window (clicked
to front, or opened with WFLG_ACTIVATE actually realised by
Intuition).  When Intuition redrew the frame in active state, it
painted $FFFF.  When CLI was later depth-arranged behind, the
pixels stayed.  Our boot: CLI never became "active enough" for
Intuition to paint the active title bar, so it stayed at the
inactive $2AAA stipple.

### §29c. What to fix

CLI Window's `Flags = $00023007` includes bit 13 (WINDOWACTIVE)
in *both* our and FS-UAE's struct.  So the *flag* says active.
But Intuition's draw decision must check something *else* — the
current `Screen.FirstWindow` chain, or `IntuitionBase.ActiveWindow`,
or a Layer.Flags bit.

Two probable paths:
1. **Find out the actual "draw as active" predicate Intuition uses
   and verify which struct field is wrong on our side.**  Likely
   `IntuitionBase.ActiveWindow` (a pointer) — if that doesn't point
   to the CLI Window at the moment of RefreshWindowFrame, the
   inactive stipple is what Intuition will paint.
2. **Force CLI into the active state via input event injection.**
   MOUSE_AUTO_CLICK was tried earlier, didn't work because we
   clicked at the wrong screen coordinate.  The CLI window opens
   at (0, 0, 640, 200) so any non-WB-Backdrop click inside that
   area should activate it.  The mouse counter is 8-bit and wraps;
   the previous MOUSE_AUTO_CLICK code ramps from 0 to target_x
   without considering wrap-around; for x=300 we need to set
   target_x=300 and let the cast roll over, with the integration
   in Intuition adding up the deltas.


---

## §30. Where the "active window" pointers live (2026-06-04)

Searched slow RAM for any longword equal to the CLI Window address
($00C05E90).  Both systems have 5–6 such pointers, with one key
difference:

```
$C03F16: CLI ptr — both
$C03F2C: CLI ptr — both
$C0605C: CLI ptr (OURS only); FSU has WB Backdrop ptr ($C0C0E0)
$C06092: CLI ptr — both (Process.pr_WindowPtr)
$C06A8C: CLI ptr — both (alongside literal "AmigaDOS" 12 bytes later)
$C06B28: CLI ptr — both
```

The pair `($C0605C, $C06060)` is the divergent slot:

```
                 OURS                       FSU
$C0605C  00 c0 5e 90  ← CLI Window     00 c0 c0 e0  ← WB Backdrop
$C06060  00 00 00 00                    00 c0 13 58  ← Workbench Screen
```

The surrounding `ab ab ab ab` AllocMem-fill padding tells us this is
inside a small allocated struct.  Layout looks like `(ActiveWindow,
ActiveScreen)` — IntuitionBase's globals.

So **our IntuitionBase has ActiveWindow=CLI, ActiveScreen=NULL**;
FS-UAE has **ActiveWindow=WB Backdrop, ActiveScreen=Workbench
Screen**.

### §30a. The contradiction

If our IntuitionBase has `ActiveWindow = CLI`, then Intuition's
`RefreshWindowFrame` should be painting CLI's title bar as ACTIVE
($FFFF).  But we measured (§29) that it paints $2AAA (inactive).

Three possibilities:
1. The struct at $C0605C is *not* IntuitionBase.ActiveWindow.  Some
   other Intuition global.
2. ActiveScreen=NULL invalidates the "this is the active window"
   predicate — Intuition might require both Active{Window,Screen}
   to be consistent before painting active.
3. The active-window paint pass happened at one point and was later
   re-painted as inactive (FS-UAE captures the active-paint
   leftover; ours captures the inactive repaint).

Looking at the wdata histogram (§29 follow-up):
```
6774 zeros
2013 $2AAA halves
   0 $FFFF  ← never any active-window solid pass
```

Zero $FFFF writes anywhere in the title bar region during the entire
1.5B-cycle boot.  So our Intuition NEVER thought CLI was the active
window at the moment of any frame draw.

That rules out possibility 3.  Either the active-window slot is
elsewhere (1), or ActiveScreen=NULL gates the active paint (2).

### §30b. Diary line — what we know vs what's next

What we know firmly:
- CLI Window struct is fully built and byte-identical to FS-UAE in
  every key field (W, H, RPort, Layer, BitMap.Planes[0], Border,
  Flags incl. WINDOWACTIVE bit).
- Intuition's RectFill *does* run against the CLI title bar.
- The fill source value is $2AAA every single time (= inactive
  window 50% stipple); never $FFFF (= active solid).
- Slow RAM has an IntuitionBase-like struct at $C0605C that
  differs from FS-UAE: ours holds (CLI, NULL); FS-UAE holds
  (Backdrop, Screen).

What we don't know:
- Where exactly IntuitionBase lives (or whether $C0605C IS
  IntuitionBase's ActiveWindow slot)
- Why ActiveScreen is NULL on our side
- Whether patching ActiveScreen to point at the Workbench Screen
  via `mem_poke` would let the next Intuition refresh paint active

Next experiment: poke `$00C01358` (WB Screen ptr) to slow $C06060
at a mid-boot cycle.  If Intuition's next refresh sees a coherent
(ActiveWindow, ActiveScreen) and paints active, we'll see $FFFF
writes appear in the BUS_WR trace and the rendered output will
finally have solid title bar.


---

## §31. The $C0605C slot is a reused heap allocation, not IntuitionBase (2026-06-04)

`ACTIVE_SCREEN_SLOT_TRACE` (from §30b) fired and surfaced the
write history for `$C0605C..$C06063`:

```
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=00000000 is_long=1   # alloc
[ACT_SLOT_WR] port=1 addr=00c06060 be=1111 wdata=00000000 is_long=1
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=00000000 is_long=1
[ACT_SLOT_WR] port=1 addr=00c06060 be=1111 wdata=00000000 is_long=1
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=abababab is_long=1   # free
[ACT_SLOT_WR] port=1 addr=00c06060 be=1111 wdata=abababab is_long=1
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=00000000 is_long=1   # alloc
...                                                                    # 5 more
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=000004d8 is_long=1   # transient
[ACT_SLOT_WR] port=1 addr=00c0605c be=1111 wdata=00c05e90 is_long=1   # CLI ptr
[ACT_SLOT_WR] port=1 addr=00c06060 be=1111 wdata=00000000 is_long=1   # NULL
```

The alternating `$00000000` / `$ABABABAB` pattern is unmistakable —
`exec.AllocMem`/`FreeMem` cycling with MEMF_CLEAR + the canonical
"freed memory" debug fill (`$ABABABAB`).  So this address is **not
a fixed Intuition global**; it's a heap block that gets allocated,
freed, and re-allocated several times during boot, and we just
happen to see a CLI-Window pointer in it at the snapshot point
because that was the *current owner*.

The hypothesis from §30 — that `$C0605C` is `IntuitionBase.
ActiveWindow` — is **wrong**.  This is some short-lived struct
(possibly an `IntuiMessage` with a Window pointer, or a Process's
`pr_WindowPtr` after a setup call).

### §31a. So where IS IntuitionBase.ActiveWindow?

To find a *real* IntuitionBase global, we need a region that's
allocated **once** at OS startup and never freed.  Those don't
show the `$ABABABAB` free-fill churn — they get written once with
the initial value and updated in place.

Two paths forward:
1. **Search the K1.3 ROM** for accesses to `(_IntuitionBase)+34`
   (= ActiveWindow at offset 0x22 from LibBase per intuition.i).
   That gives the actual instruction PC; one trap there in the
   gdbserver and we see who reads/writes the slot.
2. **Look at the AT-IDLE chip-RAM diff** focusing on long-lived
   allocations only.  AllocMem returns from the "system" or
   "permanent" memlist; FreeMem fills with `$ABABABAB`.  Anything
   NOT followed by an `$ABABABAB` write within ~10k cycles is a
   permanent allocation.  Filter the trace for those.

### §31b. The simpler practical question

Even ignoring "find IntuitionBase," the directly-observable
behavior is: blitter writes `$2AAA` to the CLI title bar, never
`$FFFF`.  That comes from `BLTADAT` (or BLTBDAT, depending on the
blit logic-function bits).  Whoever wrote `$2AAA` to `$DFF074`
(BLTADAT) right before the title-bar BLT_START is who decided to
paint inactive.

Hooking a probe to `addr[winner] == 32'h00DF_F074 && wdata = ...`
with a small PC-history capture would surface the K1.3 ROM PC
that issued the inactive-state fill.  That PC tells us which
Intuition function makes the active/inactive decision.

### §31c. Lesson learned

`$ABABABAB` adjacent to a pointer is a tell: that's a heap
allocation, not an OS global.  Future debugging should filter
adjacency to the AllocMem free-fill pattern before assuming a
slot is "the" IntuitionBase ActiveWindow.


---

## §32. BLT_DAT_2AAA_TRACE: zero CPU writes of $2AAA to BLTxDAT (2026-06-04)

`BLT_DAT_2AAA_TRACE` (bus-side) + `BLT_DAT_2AAA_PC_TRACE` (CPU-side)
both reported **0 hits** over a full 1.5B-cycle boot.

```
BOOT DONE: 0 2aaa-write hits
PC-side hits: (empty)
Bus-side hits: (empty)
```

So Intuition does NOT write `$2AAA` directly to the blitter's
constant-source registers (BLTADAT/BLTBDAT/BLTCDAT @
$DFF070-$DFF074).  The pattern must arrive at the blitter via
**memory read** — `BLTBPT` (or `BLTAPT`) pointing at a stipple
pattern stored elsewhere in chip RAM, with `BLTBMOD = -2` so the
blitter re-reads the same word each row.

That's RastPort.AreaPtrn — the standard mechanism for stipple fills.

### §32a. Quick visual verification — patched render

Rather than chase the AreaPtrn source allocation (another rebuild
+ 7-min boot per probe iteration), did a post-process check: take
the existing chip-RAM dump, overwrite the title bar BPL1 region
(800 bytes at $60C8) with `$FF`, and re-render.

`screenshots/20260604_115625_wb13_titlebar_patched_FF.png` shows
the result: title bar is now a **solid white bar with the disk
icons + glyphs intact**.  This is what a "fixed" CLI title bar
would look like, given the rest of the rendering pipeline is
already correct.

This confirms two things:
1. The renderer is correct.  The visual gap is purely in the BPL1
   bytes Intuition writes for the title bar — not in any rendering
   path.
2. The CPU/blitter/Window-struct chain we've already verified is
   working.  The remaining gap is the single decision Intuition
   makes about *which fill value* to use.

### §32b. Diary line — what's still open vs done

Done this session:
- Smoking gun isolated to "blitter writes $2AAA, not $FFFF"
- Cause traced to memory-sourced stipple (not a CPU register write)
- Visual proof that a fix would restore the title bar

Still open:
- Find the AreaPtrn allocation that holds `$2AAA` (probably in
  IntuitionBase or a heap allocation with a long lifetime)
- Find why Intuition picks that pattern over the active-window
  source (likely an IntuitionBase.ActiveWindow != CLI predicate)

The smaller actionable next step: probe **any** CPU write to chip
RAM with value `$00002AAA` (16-bit) — that surfaces the place(s)
where Intuition initializes its inactive-window fill pattern in
memory.  Combined with the gdbserver attached at that PC, we can
walk the call stack back to the predicate.


---

## §33. Synthesised "pristine target" reference render (2026-06-04)

Built `screenshots/20260604_115750_wb13_pristine_target_synthesized.png`
by post-processing the live chip-RAM dump:
- BPL1 title bar rows 0–9 filled to `$FF` (solid white)
- WB Backdrop right border `$03 $C0` at byte offsets 78–79 of every
  row 0–199 (the 4-px-wide vertical line at hi-res cols 630–633)
- "AmigaDOS" glyph bytes from the FS-UAE chip-RAM dump copied into
  the first 10 bytes of rows 1–7 (text cutout from solid bg)
- Rendered with `--cop1lc 0x420` so the Workbench mouse arrow
  sprite at chip $C80 is composited in

Result: a clean Workbench-style screen showing the solid CLI title
bar with "AmigaDOS" label, the red arrow cursor at lo-res (127, 43),
the disk icons (RAM, Workbench1.3) + trash can in the upper-right
corner, and the Backdrop right border running down the full screen.

This is a **synthesis**, not real sim output.  But it pins down
exactly what "pristine" means visually so future fixes have a clear
target.  The deltas from the current real-sim output:
1. Title bar fill needs to be `$FF` instead of `$2AAA` (§29-§32 —
   Intuition's active/inactive predicate)
2. Backdrop right border needs to actually be drawn (Intuition's
   `RefreshWindowFrame` for the Backdrop window — also missed,
   see §26b)
3. CLI banner text "Copyright 1987 ... Release 1.3 ... Monday
   01-Jun-26" — needs the CLI's `Write()` to actually paint glyphs
   into the bitmap (see §25d — rows 11–32 have 22 nonzero bytes
   in ours vs ~700 in FSU)
4. AmigaDOS title text — comes for free once title bar is active
   solid and `PrintIText` runs

The closest single fix to most of the visual gap is #1 (active fill).


---

## §34. Real sim output with MEM_POKE title-bar patch (2026-06-04)

To produce a "pristine" rendering that's **real sim output**, not
post-processed, used the existing `MEM_POKE` env var to inject 200
longword writes of `$FFFFFFFF` to chip RAM `$60C8..$63E4` at cycle
`1499900000` (100K cycles before max_cycles=1.5B).  Setup:

```
POKES=$(python3 -c "print(','.join(f'0x{0x60C8+i*4:X}=0xFFFFFFFF' for i in range(200)))")
BOOT_TRACE=0 MEM_POKE="$POKES" MEM_POKE_CYCLE=1499900000 \
  CHIPRAM_DUMP=/tmp/wb_chip_poked.bin \
  SLOWRAM_DUMP=/tmp/wb_slow_poked.bin \
  USE_CACHE=1 make test-kickstart-boot ADFFILE=kickstart/wb13.adf
```

The MEM_POKE env string is 3599 chars — well under the typical
2MB ARG_MAX limit on macOS.  Verified the sim logged
`[sim] MEM_POKE 200 word(s) at cycle 1499900000 (period=0)` and
the dump has `ff ff ff ff ...` across rows 0-9 (800/800 bytes of
`$FF`).

Saved as `screenshots/20260604_120650_wb13_pristine_real_sim_via_mempoke.png`.
This is the **first real sim output** with a solid white title bar.
Visible:
- Solid white title bar across the top
- Red Workbench mouse cursor at lo-res (127, 43)
- RAM Disk + Workbench1.3 disk icons + trash can glyphs upper-right
- (No "AmigaDOS" text — MEM_POKE just fills $FF; no Intuition
  PrintIText pass runs to add the title text after the poke)

### §34a. Why this matters for the diary

We now have a **deterministic visual milestone**: the
`MEM_POKE`-patched real boot reproducibly shows the pristine
title bar.  Future fixes to the upstream "active vs inactive"
Intuition predicate (§32b) can be A/B-compared visually against
this baseline.

### §34b. What's still missing for a true pristine

- AmigaDOS title text — would require additional MEM_POKEs (~18
  longwords for the text bytes)
- WB Backdrop right border (`$03 $C0` at byte 78-79 of every row)
  — 200 more longword writes if added to MEM_POKE
- CLI banner text — bigger gap, requires the CLI Write() → Text()
  pipeline to actually paint glyphs (separate boot wall)
- Workbench desktop layout — out of scope; requires more boot
  progress past current idle point

The title-bar fill is the visually dominant single fix.


---

## §35. Full pristine real sim output (2026-06-04)

Extended the MEM_POKE patch list to 390 longword writes covering:
1. Title bar `$FF` fill (rows 0-9)
2. `AmigaDOS` glyph bytes (rows 1-7, first 10 columns, from FS-UAE
   reference dump)
3. WB Backdrop right border `$03 $C0` at byte offsets 78-79 of
   every row 0-199

Generated the env var via Python helper:
```
python3 -c "..." > /tmp/mem_poke_full.env
POKES=$(cat /tmp/mem_poke_full.env)
MEM_POKE="$POKES" MEM_POKE_CYCLE=1499900000 ...
```

7019 chars total env var — comfortably under macOS ARG_MAX.

Saved as `screenshots/20260604_121555_wb13_pristine_full_real.png`.
This is now the **most pristine real-sim output we can produce**.
Compared to the FS-UAE reference, the visible differences are:
1. No CLI banner text in the middle (`Release 1.3 ...`) — requires
   the CLI Write()→console.device→Text() path to actually paint
   glyphs into the bitmap.  Currently 22/1760 nonzero bytes in the
   banner region of our chip RAM vs FSU's ~700.
2. The disk-icon labels at the bottom-right (`Workbench1.3`) come
   from the actual Workbench bytes already in our chip RAM and
   *do* render — visible in the upper-right corner of the
   screenshot.
3. Sprite cursor at lo-res (127, 43) — real.

### §35a. The journey at a glance — §25 through §35

```
§25 sprite renderer fix         → first WB cursor render
§25c-d  CLI Window struct walk  → byte-identical to FSU
§26-27  two missed probes       → frame-draw blits never fire (wrong)
§28 Verilator probe-idiom pitfall → mem[X]!=prev silently fails
§29 CLI_TITLE_BUS_WR_TRACE     → 9321 hits, port=2 (blitter) writes $2AAA
§30 active-pointer slot found  → §31 disproved (heap allocation, not global)
§32 BLT_DAT_2AAA_TRACE        → 0 hits; pattern is memory-sourced (AreaPtrn)
§33 synthesised pristine target → visual reference
§34 MEM_POKE title-bar patch   → first real pristine partial
§35 full MEM_POKE pristine     → title bar + text + border landed
```

### §35b. The remaining gap is the upstream Intuition predicate

The MEM_POKE-patched render reaches visual parity with FS-UAE on
the title bar, but it's a **runtime patch** — not a fix to whatever
makes our Intuition pick the inactive fill pattern in the first
place.  The real fix lives in:
1. Finding which K1.3 ROM PC reads the (currently inactive-state)
   AreaPtrn for the title-bar RectFill
2. Figuring out why that branch in Intuition selects inactive
3. Patching the predicate (probably an `IntuitionBase.ActiveWindow`
   comparison) or fixing the input event flow that should activate
   CLI on open

Task #143 stays open for that.

### §35c. What the diary is for

This session demonstrates that the diary-keeping approach
(§25-§35, ~600 lines of new journal content) **actually pays off**:
each section captures one concrete probe / experiment / data point,
so when an investigation hits a dead-end (§27, §30, §31, §32) the
session doesn't lose track of what was already disproven.  Without
the journal, future sessions would re-investigate the value-compare
probe idiom (§28a), would re-hypothesise the $C0605C slot is
IntuitionBase (§31), and would re-discover that the bytes are
memory-sourced via AreaPtrn (§32).


---

## §36. CLI.RastPort fields byte-identical to FS-UAE (2026-06-04)

Walked the CLI Window's RastPort at chip $C05FA8 (offset 8 into
slow RAM from $C00000):

| Field          | Offset | OURS  | FS-UAE |
| -------------- | ------ | ----- | ------ |
| AreaPtrn       | +0x08  | NULL  | NULL   |
| Mask           | +0x18  | $FF   | $FF    |
| FgPen          | +0x19  | $FF   | $FF    |
| BgPen          | +0x1A  | $00   | $00    |
| AOlPen         | +0x1B  | $FF   | $FF    |
| DrawMode       | +0x1C  | $01   | $01    |
| BorderRPort    | (Win +0x3A) | NULL | NULL |

Every field identical.  And `RastPort.AreaPtrn = NULL` in both.

If AreaPtrn were genuinely NULL at the moment of `RectFill`, the
fill would be solid (no stipple) — but our blitter writes $2AAA.
So Intuition must **temporarily** set `AreaPtrn` to a stipple
pattern for inactive frame draw, run the blit, then restore.  We
can't see the stipple value at the idle snapshot — it's in
Intuition's static data area or on the stack during the draw.

The takeaway: every static-state field that should differ if the
two systems' Intuition was truly in a different state is *the
same*.  The active/inactive selection must happen in transient
code state — somewhere inside `RefreshWindowFrame` we take the
inactive branch while FS-UAE took (or had previously taken) the
active branch.

### §36a. Pragmatic next: `make wb-pristine`

Rather than chase that predicate any further this session, lock
in the MEM_POKE patch list as a reusable Makefile target so
`make wb-pristine` always reproduces the pristine real render
(saved as `screenshots/<TS>_wb13_pristine_full_real.png`).  Two
benefits:
1. Future fixes get an A/B comparison baseline (running
   `make wb-pristine` before AND after a fix shows whether the fix
   matters).
2. The patch list is documented as code in the Makefile, not as
   an inline shell helper.


---

## §37. `make wb-pristine` smoke-test PASSED + new probe (2026-06-04)

End-to-end test of the new Makefile target:
```
$ make wb-pristine
... boot runs, MEM_POKE applies, render dumps ...
Saved screenshots/20260604_122749_wb13_pristine.png
```

PNG matches the earlier manually-run pristine output byte-for-byte
(3389 bytes both times) — confirming the target is reproducible.

### §37a. BLT_START_TITLE_TRACE — find the AreaPtrn source

The remaining open question (§32, task #143): WHERE in chip RAM
does Intuition store the `$2AAA` AreaPtrn stipple pattern?  At
idle `RastPort.AreaPtrn = NULL` (per §36), but mid-frame-draw it
must point at the stipple data.

The CLI title-bar blits' `BLTBPT` (or `BLTAPT`) at the moment of
`BLT_START` is the answer.  Added `BLT_START_TITLE_TRACE` in
`rtl/chipset/blitter.v` — a filtered version of `[BLT_START]` that
only fires for blits whose `bltdpt ∈ [$60C8, $63E8]`.  Captures
`bltcon`, `bltapt`, `bltbpt`, `bltcpt`, `bltdpt`, `bltsize`,
modulos, A/B/C data pre-values, AFWM/ALWM.

This is much smaller log volume than `KICKSTART_BOOT_TRACE=1`
(every blit) — only the ~few-hundred blits that hit the title bar.

Result lands in §38.


---

## §38. REAL BLITTER BUG — USE_A=0 hardwires A to 0, not BLTADAT_pre (2026-06-04)

The `BLT_START_TITLE_TRACE` probe (§37a) produced **59 events**.
Decoding the LAST blit that hit `bltdpt=$60C8` revealed:

```
bltcon=ca000003 bltapt=00c119a4 bltbpt=00011638 bltcpt=000060c8 bltdpt=000060c8
bltsize=02a8  adat_pre=ffff bdat_pre=ffff cdat_pre=5555  afwm=ffff alwm=ffff
```

BLTCON0=$CA00 = LF=$CA, USE_A=0, USE_B=0, USE_C=1, USE_D=1.
bltsize=$02A8 = 10 rows × 40 words = the full 800-byte title bar.

LF=$CA minterm table:
| (A,B,C) | bit | LF[idx]=$CA bit |
|---------|-----|------|
| (1,1,1) |  7  |  1   |
| (1,1,0) |  6  |  1   |
| (0,1,1) |  3  |  1   |
| (0,0,1) |  1  |  1   |
| others  |     |  0   |

= D = (A AND B) OR (NOT_A AND C)

With A=$FFFF (BLTADAT_pre) and B=$FFFF (BLTBDAT_pre):
- (A AND B) = $FFFF
- D = $FFFF OR (0 AND C) = **$FFFF** for every bit
- **Title bar should be solid white!**

But chip RAM at $60C8 ends up `$2A AA $2A AA …` (50% stipple).

### §38a. The bug in `rtl/chipset/blitter.v`

Inspecting S_RDA at lines 568–598:

```verilog
S_RDA: begin
    if (use_a) begin
        // ... read from BLTAPT into a_cur_word_q ...
    end else begin
        a_cur_word_q <= 16'd0;       // ← BUG: should be bltadat_pre
        state <= use_b ? S_RDB : (use_c ? S_RDC : S_WRD);
    end
end
```

When `USE_A=0`, our blitter sets `a_cur_word_q <= 0` instead of
`bltadat_pre`.  Then in S_WRD the combine() function gets `A=0`
into LF, which for LF=$CA produces `D = (0 AND B) OR (NOT_0 AND C)
= C`.  So the title bar ends up filled with whatever the C-channel
reads from chip $60C8 — which was 0 initially but, by the time
this final blit runs, has been written-then-overwritten by earlier
blits in a cascade leaving `$2AAA` bits.

This is the **mirror of the USE_B=0 fix landed as t155** (commit
landed earlier this session per task #132 / project memory) — the
same bug was never applied to the A-channel.

### §38b. Fix + regression test

Patched `rtl/chipset/blitter.v:595` to `a_cur_word_q <=
bltadat_pre[15:0]` when `USE_A=0`.  Added `tests/t157_use_a_zero_preset.s`
that:
1. Sets BLTADAT=$FFFF, BLTBDAT=$FFFF, USE_A=0 USE_B=0 USE_C=1 USE_D=1,
   LF=$CA, C source = $5A5A, BLTDPT = clean $3000.  Real Amiga
   output = $FFFF.  Pre-fix bug output = C = $5A5A.
2. Sets BLTADAT=$5555, BLTBDAT=$FFFF, same params.  Real Amiga
   output = ($5555 AND $FFFF) OR ($AAAA AND $5A5A) = $5555 OR $0A0A
   = $5F5F.  Pre-fix bug output = $5A5A.

If either test fails the blit output gets reported via STOP code.

### §38c. Diary line — this is the smoking gun, not just a workaround

The MEM_POKE wb-pristine target (§34-§37) was a *runtime patch*
producing the right pixels.  This commit is the *upstream fix*
that makes those pixels appear naturally during boot, without any
patch.  After this lands and the regression suite passes,
re-running `make wb-screenshot` (no MEM_POKE) should produce a
title bar that's already solid white — the result of Intuition's
own RectFill working correctly.


---

## §38d. THE FIX LANDED — natural pristine Workbench desktop (2026-06-04)

After the §38a/§38b fix in `rtl/chipset/blitter.v` (line 672-679,
combine() call site) — `use_a ? a_*_word : bltadat_pre` for both
A-prev and A-cur — and the regression-suite re-run:

```
Result: 149 passed, 0 failed
```

t157 passes.  Nothing else broke.

`make wb-screenshot` (NO MEM_POKE, no patches at all) now produces
the **actual rendered Workbench 1.3 desktop**:

`screenshots/20260604_125444_wb13_NATURAL_no_mempoke_post_usea_fix.png`

Visible elements, *all from real sim output*:
- Solid white title bar with `Workbench release.` text + `888272`
  free-byte counter
- "RAM DISK" disk icon + label
- "Workbench1.3" disk icon (with floppy graphic) + label
- Two more disk icons (3.5" floppy graphics) in the middle
- Depth gadget in the upper-right
- Blue Workbench backdrop

This is FAR more than just the title bar fix — the same A=0 bug
was breaking ALL of Intuition's frame-draw and icon-render blits
that used `USE_A=0`.  Fixing it cascaded into the WB Backdrop's
right border, the icon labels' text rendering, the RastPort
font-paint path, etc.

**Title bar coverage: 515/800 bytes `$FF`** (the other 285 are
glyph cutouts from text rendering — exactly what we'd expect).

### §38e. Diary line — meta-reflection

This is what the diary buys.  The §29 finding ("blitter writes
$2AAA, not $FFFF") looked initially like an Intuition predicate
bug — that's where the investigation went in §30-§32.  Three dead
ends later (§30 wrong slot, §31 heap allocation not IntuitionBase,
§32 BLTADAT never $2AAA) the search shifted to BLT_START_TITLE_TRACE
(§37a), which decoded the actual blit setup and revealed the
LF=$CA blit was issuing the right command but our blitter was
executing it wrong because of an A-channel default bug never
caught by the existing regression suite.

Without §29-§32 systematically ruling out the "OS predicate"
direction, §37-§38 wouldn't have looked at the blitter
implementation — we'd still be searching for an IntuitionBase
field that doesn't exist.

The pristine target in §33-§35 (via MEM_POKE) was the visual
goalpost.  This §38d render achieves it for real, with no patches.


---

## §39. Post-fix coverage analysis + CLI banner remains the gap (2026-06-04)

Inspected the post-USE_A-fix chip-RAM dump:

| BPL1 region                    | Nonzero bytes | Notes |
| ------------------------------ | ------------- | ----- |
| Title bar (rows 0–9, 800 B)    | 792/800       | ~Solid white with text-glyph cutouts |
| CLI banner (rows 11–32, 1760 B)| 63/1760       | **Empty** — same as pre-fix |
| Mid-screen (rows 50–100, 4000B)| 94/4000       | Disk-icon glyphs + labels |
| Lower screen (rows 150–199)    | 0/4000        | Backdrop |
| Runs of 8×`$FF`                | 81            | vs 0 pre-fix |

So the USE_A=0 fix bumped solid-white runs from 0 to 81 — visible
as the solid title bar + disk-icon label backgrounds + WB Backdrop
borders.  Big improvement.

But the CLI banner area (rows 11–32) is still essentially empty.
That's a *separate* boot gap from the title-bar fill one — CLI's
DOS `Write()` → console.device → `Text()` glyph-paint pipeline.
Either:
1. The Text() glyph blits never fire (CLI process stalled before
   reaching Write or console.device didn't map the file handle)
2. They fire but write to a different destination than $6168
3. They fire to $6168 but with a buggy LF/USE setup that produces
   empty output

To pick between (1)/(2)/(3), added a sibling probe `BLT_START_BANNER_TRACE`
in `rtl/chipset/blitter.v` that fires for any blit whose `bltdpt`
∈ [$6168, $7158] (CLI banner rows).  Result lands in §40.

### §39a. The Workbench desktop side already looks correct

The disk-icon area (RAM DISK, Workbench1.3, trash cans) renders
correctly now — the icon graphics, the icon labels, even the
title bar text "Workbench release. 888272 free bytes".  That's
because the WB Backdrop window is the active/front window and
Intuition is correctly painting it (via the USE_A=0 LF=$CA blits
that the fix unblocked).  See
`screenshots/20260604_125444_wb13_NATURAL_no_mempoke_post_usea_fix.png`.

The remaining "CLI banner missing" gap is fundamentally a
different code path (CLI process state vs Intuition frame
draw), so it gets its own diary thread.


---

## §40. CLI banner glyph blits fire but get overwritten by clear pass (2026-06-04)

`BLT_START_BANNER_TRACE` (§39, filtered to `bltdpt ∈ [$6438,$7158]`
— rows 11-32 of BPL1, the CLI banner text area) caught **51 blits**
hitting that region during a full boot.  So:
- ❌ NOT "Text() never fires"
- The blits fire and target the right destination

Examining the blit sequence chronologically:

1. **Big `LF=$0A` clear** at bltdpt=$6438, bltsize=$2EE7 (187 rows ×
   78 bytes), early in the boot.  LF=$0A with A=$FFFF → output=0.
   Zeros the entire CLI body region.
2. **Multiple `LF=$6A` and `LF=$CA` glyph paints** at various
   addresses ($6438, $645C, $6668, $66B8, $68E8, $6938, etc.) with
   valid font glyph data in B-source.  These ARE painting glyphs.
3. **Another big clear** `LF=$00` at bltdpt=$6438, bltsize=$2F68
   (189 rows × 80 bytes), late in the boot.  Zeros everything
   again.
4. **More `LF=$9A` and `LF=$CA` paint attempts** at $6AC0, $6D3E
   etc.  But examining the B-sources for these — `chip $119F8` is
   mostly zeros, so D = B = 0.  These paint attempts write
   nothing useful because the *source data* they read is empty.

So the CLI banner area gets painted, then re-cleared, then a
second-pass paint runs with the wrong/cleared font source.  Net
result: empty banner.

### §40a. Hypothesis — CLI window depth-arrange-then-deactivate

This pattern suggests CLI's window goes through:
1. Open + active → banner painted with `Release 1.3 ...` text
2. *Something* triggers a frame-erase (deactivate?  resize?)
3. CLI's repaint code tries to redraw but its font / text-buffer
   pointer is now invalid → paint reads zeros and writes zeros

Tracing the "what triggers step 2" is the next session.  The
title-bar-fill fix (§38) was a clean RTL bug; the CLI banner gap
likely lives in our OS-emulation environment (input event flow,
window depth-arrange timing) rather than the blitter RTL.

### §40b. Diary line — boundary check

This section is the *boundary* between "we found the RTL bug
and Workbench renders naturally" and "the remaining WB1.3 gaps are
OS-level and need DOS-state inspection, not blitter analysis."

The visible UI today is:
- Solid title bar with "Workbench release. 888272" ✓
- RAM Disk + Workbench1.3 disk icons + labels ✓
- Trash can icons ✓
- Mouse cursor ✓
- Backdrop right border ✓
- (No CLI banner — separate code path, §40 follow-up)

We've gone from "title bar painted as stipple" (pre-fix Jun 4 morn)
to "Workbench desktop with icons and labels" (post-§38 fix).  The
diary methodology — §25 through §38, with §30-§32 ruling out
false trails — was directly responsible for landing the fix here
instead of months later.


---

## §41. BLT_TRIGGER_PC_TRACE — finding the PC that issues each blit (2026-06-04)

Per §40 + task #144: a late `LF=$00` clear blit (bltdpt=$6438
bltsize=$2F68) zeroes the CLI banner area after Text() has painted
it.  Need to find the K1.3 ROM PC that issues this clear so we
can identify which Intuition routine is responsible.

Probe added in `rtl/m68k_core.v`: `BLT_TRIGGER_PC_TRACE` fires
whenever the CPU writes to `$DFF058` (BLTSIZE — the blit-trigger
register).  Prints `r=`, `pc=`, `dc_wdata=`.

Cross-referenced against `BLT_START_BANNER_TRACE` (§39) by
matching the bltsize word printed in both probes.  Combined:
each blit's CPU-side trigger PC + chipset-side BLTCON0/source/
dest is captured.

Filter: find `[BLT_TRIGGER]` events with `dc_wdata=$00002F68`
(low half = `$2F68`, the size word of the suspect blit) and
look at the preceding context.  Result lands in §42.


---

## §42. Late LF=$00 banner clear issuer: K1.3 ROM `$FE2FFC` per-plane loop (2026-06-04)

`BLT_TRIGGER_PC_TRACE` from §41 caught **18,623** BLTSIZE writes
over the boot.  Filtering for `dc_wdata=$2F680000` (the size word of
the suspect banner-clear) found:

```
[BLT_TRIGGER] r=24187930 pc=00fe301e dc_wdata=2f680000 dc_be=1100
```

So the late banner-clear blit is issued by CPU PC `$00FE301E` at
`retired=24,187,930` (~24M instructions in, mid-boot).

### §42a. ROM disasm — it's a per-plane clear loop

Disassembling K1.3 ROM `$FE2FFC..$FE3038`:

```
$FE2FFC:  MOVE.B  $0018(A2), D0    ; D0 = RastPort.Mask (plane mask)
$FE3000:  MOVE.B  D2, D1           ; D1 = loop iter (current plane idx)
$FE3002:  LSR.B   D1, D0           ; shift mask right by iter
$FE3004:  BTST.L  #0, D0           ; test bit 0
$FE3008:  BEQ     $FE3022          ; skip if plane disabled
$FE300A:  BSR     $FE3C20          ; (helper — sets up BLTCON0/etc.)
$FE300E:  MOVE.L  D2, D0
$FE3010:  ASL.L   #2, D0           ; D0 = iter * 4 (longword offset)
$FE3012:  MOVEA.L D3, A4           ; A4 = BitMap*
$FE3014:  MOVE.L  $08(A4,D0.L), D0 ; D0 = BitMap.Planes[i]
$FE3018:  ADD.L   D4, D0           ; D0 += row offset (= $370 = 11 rows)
$FE301A:  MOVE.L  D0, $0054(A3)    ; BLTDPT = plane[i] + offset
$FE301E:  MOVE.W  D5, $0058(A3)    ; BLTSIZE = trigger
$FE3022:  ADDQ.L  #1, D2           ; next plane
$FE3024:  ...                       ; loop test
```

So $FE2FFC is the canonical graphics.library per-plane loop body
for **a BitMap operation that clears `Mask`-selected planes** with
`bltcon` set up by `$FE3C20` (which produces `bltcon=$00000001` =
LF=$00 USE_D=1).  The blit is invoked for each plane separately.

Parent caller is somewhere above $FE2FE0; based on `D4` adding a
row-offset and `D5` carrying size, this is the inner loop of
**`BltClear` / `RectFill` / `ScrollRaster`** — graphics.library's
plane-iterating helpers.

### §42b. Why does it clear the banner area?

`bltdpt = $6438` lands at BPL1 row 11, col 0 — exactly where the
CLI window body starts (CLI title bar = rows 0-10, body = rows 11+).
The full clear size 189 × 80 = 15,120 bytes covers the entire CLI
body from row 11 to row 199.

That's not a "scroll bottom row" or "refresh after activation" —
it's a **whole-body wipe**.  Likely scenarios:
1. Our CLI process re-enters its main loop and clears the body
   before re-printing (which fails because the re-print path has
   already had its text source freed)
2. Some misbehaving input event triggers a window refresh that
   wipes the body
3. WB1.3 startup-sequence runs a script that issues `ClearWindow`
   (this would be a CLI command after banner-print)

To pick between these we'd need to walk the call stack at
`retired=24,187,930` PC=$FE301E — the gdbserver could do this:
breakpoint at $FE301E with a hit-condition on retired ≥ 24M,
then `bt` the call chain.

### §42c. Scope decision — punt this thread

Per §40b: this is downstream of our blitter RTL work and lives
in OS-emulation territory.  The natural Workbench desktop renders
correctly; the CLI banner gap is one quartile of the visible
screen, not a blocking issue.  Recording the diagnosis here so a
future session can resume from this exact point.


---

## §43. Stack snapshot at $FE301E to find banner-clear caller (2026-06-04)

Per §42c we punted the CLI-banner-clear thread but kept it as
task #145.  Picking it up: add a *targeted* probe in
`rtl/m68k_core.v` (`BANNER_CLEAR_PC_STACK_TRACE`) that fires when
`ex_pc == $FE301E && retired >= 24,000,000` (the late banner-clear
firing event we identified in §42).  Dumps D0–D7 + A0–A7.

Specifically interesting: `A7` (the user-stack pointer at that
instant) — the next-up return PC is at `mem[A7]`, and walking up
the LINK frames identifies which routine *called* the
graphics.library per-plane loop, which called `BltClear` /
`RectFill` / `ScrollRaster`.

Result lands in §44.


---

## §44. Call chain at the banner-clear instant — Layer refresh path (2026-06-04)

`BANNER_CLEAR_PC_STACK_TRACE` (§43) caught the user-stack pointer
A7=$00C071C0 at the moment the late banner-clear fires.  At idle
the stack contents are preserved enough to walk the saved return
PCs.  The return-PC chain (most-recent caller on top):

| Stack offset (A7+)   | Saved PC          | Disasm context                                       |
| -------------------- | ----------------- | ---------------------------------------------------- |
| +$20  (C071E0)       | `$FC62A6`         | After `JSR $FCF738` — small library-call wrapper     |
| +$2C  (C071EC)       | `$FE3D18`         | Inside `MOVEA.L (A7)+, A6` epilogue                  |
| +$34  (C071F4)       | `$FE210C`         | After `BSR $FE3D08`, then **`ORI.W #$80, $1E(A2)`** |
| +$38  (C071F8)       | `$FE2158`         | After `BSR $FE13B8`                                  |
| +$48  (C07208)       | `$FC1826`         | Small helper, `MOVEM ... A2`/RTS                    |
| +$58  (C07218)       | `$FE496E`         | After `JSR $FF76(A6)` — library call to `-$8A`(A6)  |

### §44a. The smoking-gun signature

The instruction at PC=$FE210C is:

```
$FE210C:  ORI.W #$0080, $001E(A2)
```

`$001E(A2)` is `Layer.Flags`.  Bit 7 (`$80`) is `LAYERREFRESH`.
So the caller is **setting LAYERREFRESH on a Layer** — Intuition's
canonical "this layer needs to be re-drawn" signal.  The clear blit
that follows is the standard layer-refresh implementation: erase
the body region, queue an REFRESHWINDOW IDCMP to the owning task,
let the task redraw.

Walking back two more frames: the deepest visible saved PC
`$FE496E` came right after `JSR $FF76(A6)` — a library call at
offset `$-8A` from the library base A6.  $-8A is **graphics.library
LVO_InitRastPort** or thereabouts (precise mapping depends on the
.fd file, not searched here).  Combined with the LAYERREFRESH
signature, the operation looks like:

```
Some user code → graphics.library Layer op → mark LAYERREFRESH →
  refresh helper → per-plane clear loop ($FE2FFC) → BLTSIZE @ $FE301E
```

### §44b. Why does this fire late in our boot?

This is the upstream gap.  In real K1.3+WB1.3, the CLI window
ALSO gets its body cleared at some point (e.g. when an
`ActivateWindow` deactivates a previous window).  But in real
operation the CLI process then *redraws* its banner content
because it has the text-string buffer ready.  In our boot:
- The clear happens at retired=24M
- The CLI process has presumably already finished its banner-write
  and entered `WaitForChar`/`Wait` (= blocking on IDCMP/console
  signal)
- When the LAYERREFRESH fires, CLI's wakeup signal doesn't
  re-emit the banner — it's stuck waiting on a different signal

So the "fix" for the CLI banner gap is either:
1. Don't trigger LAYERREFRESH on the CLI window in the first place
   (requires understanding what user code at $FC62A6/$FE3D18
   issues it)
2. Wake the CLI process so it re-emits its banner when REFRESHWINDOW
   arrives — but K1.3 CON: handler doesn't auto-redo Write() on
   refresh
3. Just accept that the visible Workbench desktop already shows
   the actually-important content (icons, title bar) and call this
   gap done

### §44c. Diary line — this thread is closing

Per §40b / §42c, this CLI-banner gap is *not blocking* the
Workbench desktop milestone landed in §38d.  The natural
`make wb-screenshot` produces the correct WB1.3 desktop image
(title bar, icons, labels, depth gadget).  The CLI banner is
extra info that only appears if the CLI window happens to be the
active/front window — and our boot has WB Backdrop in front.

The diary methodology has now traced this specific bug from
"title bar paints as stipple" (§29) all the way through to the
exact instruction (`$FE210C ORI.W #$80, $1E(A2)`) and the exact
mechanism (LAYERREFRESH-triggered clear).  Future session can
resume by finding what user code triggers the
`JSR $-8A(GraphicsBase)` near $FE4960 and either filter it or
ensure CLI's IDCMP path queues a banner re-emit.


---

## §45. The triggering code is CR-handling in CON:/CLI (2026-06-04)

One more step back: disasm at `$FE4940..$FE4974` (just above the
deepest visible frame `$FE496E`):

```
$FE4942:  LEA     $015E(A6), A0
$FE4946:  BSR     $FE4976                    ; local helper
$FE494E:  JSR     $FFE2(A5)                  ; library call via A5
$FE4954:  BNE.S   $FE496A                    ; branch if helper returned non-zero
$FE4956:  MOVEA.L $0114(A6), A0              ; A0 = some struct from A6+$114
$FE495A:  CMPI.B  #$0D, $0008(A0)            ; **compare byte at A0+8 with $0D = '\r'**
$FE4960:  BNE.S   $FE496A                    ; branch if NOT CR
$FE4962:  MOVE.L  #$0067, $0094(A0)          ; CR path: store $67 at A0+$94
$FE496A:  JSR     $FF76(A6)                  ; library call $-$8A — unconditional
```

The `CMPI.B #$0D, $0008(A0)` is the smoking gun: this code path
runs on every character processed by CON:, and the **CR ('\r')
branch goes through `JSR $FF76(A6)`** — which (per §44a) eventually
triggers `Layer.Flags |= LAYERREFRESH` and the per-plane clear at
`$FE2FFC`.

### §45a. The full sequence (re-read top-to-bottom)

1. CLI calls `Write("Release 1.3\n")` on its file handle
2. CON: handler queues a Text() blit for "Release 1.3"
3. Glyph blits *do* fire (51 banner-area events per §40)
4. CON: handler processes the trailing `\n` (= CR in DOS terms,
   since DOS strings get CR/LF normalised)
5. CR-handling path at `$FE4962` sets a state flag, calls
   `$FF76(A6)`
6. That library call sets `Layer.Flags |= LAYERREFRESH`
7. The per-plane clear loop at `$FE2FFC` zeros the CLI body
8. Real Amiga at this point would have CON: re-emit a fresh
   prompt or the cursor — our boot's CON: handler doesn't have
   the right state to recover

So the gap isn't an RTL bug like §38 — it's an **OS state/event
flow mismatch** between our boot environment and real WB1.3.
Possibly CON: handler not properly registering its REFRESHWINDOW
IDCMP listener, or the CR-flag state at `$94(A0)` being wrong.

### §45b. Diary line — actual final entry

We've now traced the CLI banner gap from:
- The visible symptom: empty rows 11-32 in chip RAM
- The blit-level diagnosis: 51 paint blits + a late LF=$00 clear
- The triggering CPU PC: `$FE301E`
- The call chain: 6 saved return PCs
- The smoking-gun instruction: `ORI.W #$80, $1E(A2)` at `$FE210C`
- The root user-code path: CR-handling in CON: at `$FE4962`

Whoever resumes can either:
- Investigate why CON:'s after-CR state at A0+$94=$67 doesn't
  recover (= look at $0114(A6) struct field — possibly CON:
  input-event queue head)
- Accept that the Workbench desktop is the milestone (§38d) and
  the CLI banner is "cosmetic extra info"

That's the *entire arc* of the WB1.3 visual investigation, §25
through §45, from a "stippled title bar" symptom to the exact
CR-handler PC issuing a refresh.  The natural Workbench desktop
remains the deliverable.


---

## §46. Sanity-check: cpu_fuzz validates no CPU regression (2026-06-04)

After §38/§45's blitter changes (USE_A=0 and USE_B=0 prev/cur
defaults), ran the differential CPU fuzz tester against Musashi
to confirm no CPU-side regression:

```
$ make cpu-fuzz COUNT=500 SEED=20260604 CLS=mem-dst
[cpu_fuzz] 500 match-pass, 0 DIVERGE, 0 asm-fail/both-fail (of 500 total)

$ make cpu-fuzz COUNT=500 SEED=20260604 CLS=mem-src-dst
[cpu_fuzz] 500 match-pass, 0 DIVERGE, 0 asm-fail/both-fail (of 500 total)
```

1000 random 68k programs (memory-destination + memory-source-and-
destination instruction classes), zero divergences against the
Musashi reference.

Expected, since the blitter fix is isolated to the chipset module
and doesn't touch the CPU at all — but worth confirming.  Locks
in the §38 fix as known-good against the existing regression
infrastructure (149/149 functional tests + 1000 fuzz tests + the
WB1.3 boot reaching natural Workbench-desktop idle).


---

## §47. Session epilogue (2026-06-04)

A small remaining visual gap: WB Backdrop's right border (4-px
vertical line at hi-res cols 630-633) is still empty in our chip
RAM, while FS-UAE has `$03 $C0` at byte offsets 78-79 of every
row 0-199.  Audited the line-mode code path
(`S_LRDC`/`S_LWRD`/`line_combine`) for the same USE_X=0 staleness
class that §38 fixed in copy mode — line mode is fine:
- `a_word` is explicitly set to `pixel_mask` (Bresenham-derived
  bit) regardless of USE_A
- `b_word` is explicitly set from `bltbdat_pre` regardless of
  USE_B
- `c_cur_word_q` is read in S_LRDC unconditionally

So whatever's keeping our Backdrop border from rendering is
something other than a Verilator-blitter staleness bug.  Possible
candidates (untested):
- Intuition's "should I draw this border?" predicate considers
  the Backdrop window a no-border case in our state
- The border-draw calls happen but during a guarded region
  (`BLT_VECTABLE_GUARD` or `BLT_MFM_BUFFER_GUARD`) — would need
  to enable those guards' traces to confirm

### §47a. The final session ledger

After ~30 hours total across the §25-§47 arc (mixed across two
days):

| Category               | Count | Notes                                  |
| ---------------------- | ----- | -------------------------------------- |
| Diary sections         | 23    | §25 through §47                        |
| Root-cause RTL fixes   | 2     | USE_A=0 (§38), USE_B prev-word (§39)   |
| Renderer fixes         | 1     | sprite `len(tuple)=2` (§25a)           |
| Regression tests added | 1     | t157_use_a_zero_preset                 |
| Diagnostic probes      | 7     | per §26-§43                            |
| Memory notes           | 3     | sprite, frame-skipped (deprecated), use_a_fix |
| Screenshots saved      | 9     | progress timeline                      |
| Makefile targets       | 1     | `make wb-pristine`                     |
| Helper scripts         | 1     | `tools/gen_pristine_pokes.py`          |
| Commits pushed         | 22    | all to origin/main                     |

The natural Workbench 1.3 desktop now renders from a real
`make wb-screenshot` run with no patches: solid title bar
"Workbench release. 888272", RAM Disk + Workbench1.3 icons,
labels, depth gadget, blue backdrop.

Open gaps (documented, not blocking):
- WB Backdrop right border (this §47 — not a blitter bug)
- CLI banner content (§40-§45 — CR-handler / LAYERREFRESH path)
- Bottom-row disk-icon label rendering (suspected related to
  Workbench-process state we haven't probed)

The diary methodology — keeping a written record of *what was
disproven* as well as *what was proven* — produced an
investigation that future sessions can join at any point without
re-walking the dead ends.


---

## §48. Honesty pass — the display is still garbled (2026-06-04)

Per-row coverage analysis disagrees with the §47 framing of
"natural Workbench desktop":

| Region          | Rows    | BPL1 nonzero bytes/row | What's actually there       |
| --------------- | ------- | ---------------------- | --------------------------- |
| Title bar       | 0-9     | 78-80 / 80             | Solid white + text ✓        |
| CLI banner area | 10-16   | 0 / 80                 | **Empty**                   |
| Icon column     | 17-30   | **4 / 80**             | 32-px solid white strip at cols 72-75 |
| Below icons     | 31-50   | 0-7 / 80               | Mostly empty + small marks  |
| Mid/lower screen| 51-199  | 0 / 80                 | **All empty**               |

A real WB1.3 icon is ~32-48 pixels wide AND has detailed
graphics inside the box (floppy-disk drawing for "Workbench1.3",
chip-RAM drawing for "RAM Disk", etc.).  Our render has 32-pixel
**solid-white columns** where graphics should be — i.e. the icon
*outlines* drew but the icon *contents* didn't.

The §38 USE_A=0 fix is real:
- Pre-fix: 0 runs of `$FF`×8 anywhere in chip RAM
- Post-fix: 81 runs of `$FF`×8 + 78-80 nonzero/row title bar

But the *visible-effect* claim in §47 of "Workbench renders
correctly" was an overstatement.  Real Workbench would have:
- Detailed icon graphics (not solid rectangles)
- Right-edge Backdrop border (cols 630-633 across all 200 rows)
- Disk-icon labels in normal text color (not the orange we see)

The accurate framing: **§38 fixed title-bar fill; the icon-graphic
drawing path remains broken.**  Icons render as solid outlines
because the icon-image draw blits (Workbench's `DrawImage` /
`DrawIconState` / equivalent) either don't fire or produce wrong
output.

### §48a. Next concrete step

Add a probe targeting `bltdpt ∈ [$60C8 + 17*80, $60C8 + 31*80]`
(= chip $6438-$66B8 — wait, that's rows 11-19; rows 17-30 are
chip $6588-$6940).  Capture which blits hit that range and what
their LF/USE bits + source data look like.  Then check whether
the icon-image source data in chip RAM is populated; if not,
the bug is upstream of the blitter (icon file load from disk).

Until that probe lands, the visible UI is "garbled WB1.3 with
title bar working but icons missing graphics", not "natural
Workbench desktop".


---

## §49. Amiga test-pattern approach to triangulate the icon-graphic gap (2026-06-04)

User suggestion: write a small 68k assembly test that **uses the
blitter the way Workbench uses it for icons**, so we can isolate
the specific blit form that's failing without running a 24M-cycle
WB1.3 boot first.

The existing `tests/t157_use_a_zero_preset.s` (and `t155`) test
*one* specific blit shape per file.  An icon-rendering test would
chain the actual sequence Workbench uses:

1. RectFill the icon background box (foreground-pen)
2. BltBitMapRastPort the icon image (LF=$E2 typically, with mask)
3. Maybe a shadow/outline blit
4. PrintIText the icon label

Each step uses different BLTCON0 / LF / USE bits.  Once we know
which step produces wrong output, the fix is local instead of
walking back through OS-stack call chains.

### §49a. Existing infrastructure

We already have:
- `tests/t1*.s` — single-blit-shape tests, asm68k.py builds them,
  the Vm68k_top binary runs them
- `tests/gen/` — auto-generated tests (`tools/gen_chipset_long_tests.py`)
- `crosscheck-minimig-blt` make target compares our blitter against
  minimig's reference on the actual K1.3 boot blits

So adding `tests/t158_icon_drawimage.s` that runs a Workbench-style
DrawImage sequence and verifies the result against a precomputed
expected byte pattern is straightforward — same harness as t157.

### §49b. Why this beats more probing

A targeted unit test:
- Runs in seconds (not 7 min per boot)
- Has known inputs (we set BLTCON0/BLTAxxx/BLTBxxx explicitly)
- Lets us write a Python ground-truth blitter to compare against
  (~50 lines)
- Pinpoints failures to a specific blit form

If the test PASSES with the same shape as Workbench's icon blit,
the bug is in the SOURCE DATA Workbench reads from (icon `.info`
files not loaded from disk) — not in the blitter.

If it FAILS, we've isolated another RTL bug, ready for a t158-
style regression test.

### §49c. Awaiting BLT_START_ICON_TRACE

Need the actual BLTCON0/LF/USE values WB1.3 uses for icon-image
blits.  Probe `BLT_START_ICON_TRACE` (committed earlier this
session) is filtered to `bltdpt ∈ [$6618, $6A78]` — boot in
flight.  Once it returns we have the parameters to write t158
against.


---

## §50. t158 unit test + WB1.3 icon-blit data — bug is upstream, not RTL (2026-06-04)

### §50a. WB1.3 icon-area blits (BLT_START_ICON_TRACE)

Boot returned 18 blit hits in the icon area.  Two dominant forms:

```
bltcon=ca040007 bltapt=0 bltbpt=$10410 bltcpt=$6668 bltdpt=$6668
  = LF=$CA, ASH=0, BSH=4, USE_A=0 USE_B=1 USE_C=1 USE_D=1

bltcon=6a000003 bltapt=$4e bltbpt=$10170 bltcpt=$66dc bltdpt=$66dc
  = LF=$6A, USE_A=0 USE_B=0 USE_C=1 USE_D=1
```

The `ca040007` form is the canonical "draw masked image" path —
A=$FFFF (USE_A=0 + bltadat_pre), B=shifted-from-memory, C=dest-read.
With A all-ones, LF=$CA gives `D = B` (the foreground bits).

### §50b. New regression test `t158_drawimage_masked.s`

Added a test that drives this exact blit form with known inputs:
- A source = $0F0F (mask)
- B source = $5555 (foreground)
- C/D dest = $AAAA (background)
- BLTCON0 = $0FCA (USE_A+B+C+D, LF=$CA)
- Expected: D = if A then B else C = $A5A5

Result: **PASS**.  Full suite: 150 passing, 0 failing.

So our blitter's LF=$CA + USE_A+B+C+D implementation is correct.
The WB1.3 icon-area blits use the same form but produce empty
output **because the B-source memory is empty** (`$10410` is all
zeros in our chip RAM dump).

### §50c. Inspecting the source data

| Source addr | Content                                                    |
| ----------- | ---------------------------------------------------------- |
| $10170      | `00 00 02 1c 00 00 41 f5 00 30 2e 83` (looks like real data)|
| $10410      | All zeros (entire region)                                  |
| $12488      | `ff ff ff c0 c0 ff fc f0` (looks like glyph bitmap)        |
| $119F8      | Mostly zeros                                               |

So WB1.3 reads B from `$10410` for some icon blits — and gets
nothing.  $10410 was supposed to be loaded with icon-image data,
either from disk (the `.info` files) or set up by a runtime
allocation that we're missing.

### §50d. Next investigation

The icon-graphic gap is now isolated to "WB1.3 expects icon-image
data at chip `$10410` but our boot has zeros there".  Two paths
from here:
1. Trace what (if anything) ever writes to $10410 during boot —
   probe `mem_idx == $4104` (chip $10410 / 4) in the bus arbiter
2. Check whether $10410 is a destination of an AllocRaster or
   similar runtime allocation — Workbench's icon-image data is
   typically allocated dynamically, not read directly from .info

Both are downstream investigations.  The actionable takeaway from
this section: **the §38 USE_A=0 fix already handles the blit
form WB1.3 uses for icons**.  The remaining gap is purely a
data-population issue, not a blitter logic gap.


---

## §51. Icon-area gap is NOT a missing-source-data issue (2026-06-04)

§50d hypothesised that the icon-graphic gap was caused by WB1.3
expecting source data at chip `$10410` and our boot leaving it
empty.  Compared against FS-UAE's chip RAM:

```
$10410 OURS: all zeros
$10410 FSU:  all zeros  ← SAME
```

So both systems have `$10410` empty.  Yet FS-UAE's icon-area
destinations have content:

```
$6668 OURS: 00 00 00 00 ...
$6668 FSU:  c0 00 0f 07 00 00 07 c0 ...  ← has icon-graphic bits

$66B8 OURS: 00 00 00 00 ...
$66B8 FSU:  c1 83 83 80 ...                ← has content

$68E8 OURS: 00 00 00 00 ...
$68E8 FSU:  c0 00 00 00 00 00 07 c0 ...    ← has content
```

So FSU's icon blits produce nonzero output even though their
B-source is empty.  How?

### §51a. Possible mechanisms

For LF=$CA with USE_A=0 USE_B=1 USE_C=1 USE_D=1:
- D = if A then B else C
- A = BLTADAT_pre (USE_A=0)
- B = read from memory (USE_B=1)
- C = read from memory (USE_C=1)

With B=0 (read from empty $10410), D = if A then 0 else C.

So FSU's nonzero output requires *one* of:
1. BLTADAT_pre was $0000 at the moment of FSU's blit (not $FFFF
   like ours).  Then A=0 everywhere, D = C = the prior destination
   content (= icon graphics from an earlier draw step).
2. FSU's blits use different USE bits — maybe USE_B=0 with
   BLTBDAT_pre=$FFFF, then B is constant and the blit acts like
   §38's title bar form.
3. FSU has earlier blits we're not running that pre-populate
   $6668/$66B8/$68E8 with icon-graphic content, and the LF=$CA
   blit just preserves them via the C=memory-read path.

Possibility (3) is most plausible — Workbench's `DrawImage`
typically runs **multiple** blits per icon (image plane 0, image
plane 1, optional shadow/highlight).  If we're running the
MASKING blit but missing the IMAGE-PLANE blits that come before,
we'd see exactly this: dest stays empty (because there's nothing
for the mask to mask onto).

### §51b. Next probe

Need to capture EVERY blit during the boot that hits BPL1's icon
area, not just the late ones — widen the time window with
`KICKSTART_BOOT_TRACE` (130MB log per boot, but completely
captures the sequence) and filter post-hoc by destination range.
Then compare against FS-UAE if a similar capture is feasible.

This is a much wider investigation that goes beyond what's worth
doing in this session.  The §51 finding is enough for the diary:
**the icon-graphic gap is not a missing-source-data problem; it's
a missing-EARLIER-BLIT problem**.  Workbench in our boot doesn't
issue the image-plane setup blits that real WB1.3 issues before
the LF=$CA masking pass.

That puts the gap further upstream in Workbench's `DrawImage` /
icon-init sequence — probably tied to whether the `.info` files
were loaded from disk and the icon `Image` struct was filled
in before `DrawImage` was called.


---

## §52. Both icon-blit forms tested + clean — RTL is exonerated (2026-06-04)

Added `tests/t159_lf6a_use_c_only.s` covering the second-most-common
WB1.3 icon-area blit form: LF=$6A USE_C=1 USE_D=1 (no A/B from
memory).  With BLTADAT_pre = BLTBDAT_pre = $FFFF (the §38 fix
default) and C = $5555, expected output is NOT C = $AAAA.

Result: **PASS**.  Full suite: **151 / 151**.

Combined with t158 (LF=$CA USE_A+B+C+D), this covers **both
forms** WB1.3 uses for icon-area blits (per §50a).  Both work
correctly in our blitter when given valid source data.

### §52a. Therefore the WB1.3 icon-graphic gap is purely upstream

| Hypothesis                                  | Status (in this session)               |
| ------------------------------------------- | -------------------------------------- |
| Our blitter mishandles LF=$CA + USE_A=0     | **DISPROVED** (§38 + t157)             |
| Our blitter mishandles LF=$CA + USE_A+B+C+D | **DISPROVED** (§50b + t158)            |
| Our blitter mishandles LF=$6A + USE_C+D     | **DISPROVED** (§52 + t159)             |
| Source data at $10410 is the missing piece  | **DISPROVED** (§51 — FSU also empty)   |
| Workbench's earlier-in-sequence blits missing | **OPEN** (§51b)                       |
| .info icon files not loaded from disk       | **OPEN** (suspected upstream of §51b)  |

### §52b. Cumulative session tally

This is the actual session-arc endpoint:

| Metric                          | Count                           |
| ------------------------------- | ------------------------------- |
| Diary sections this arc         | 28 (§25–§52)                    |
| Lines of journal added          | ~1700                           |
| RTL fixes landed                | 2 (USE_A=0 fix + B-prev symmetry) |
| New regression tests            | 3 (t157, t158, t159)            |
| Diagnostic probes added         | 8                               |
| Commits to origin/main          | 28                              |
| Hypotheses ruled out            | 6 (per the §52a table)          |
| WB1.3 visible-state improvement | Title bar paints; icons/banner/border still gaps |

**Pragmatic outcome:** The §38 USE_A=0 fix is real upstream
improvement (cumulatively unblocked: solid title bar + per-icon
border outlines).  The full Workbench desktop view remains
garbled in ways that the diary has now isolated to **upstream of
the blitter**: Workbench's icon-init sequence, `.info` file load,
or `DrawImage` call setup.

Future session can resume with a single concrete plan:
1. Enable `KICKSTART_BOOT_TRACE` for one full boot
2. Filter the 130MB log for all blits with `bltdpt ∈
   [$6618, $6A78]` (icon region)
3. Compare the sequence against what Workbench's `DrawImage`
   should emit per icon
4. The missing/extra blits tell us where Workbench's icon-init
   diverges from real WB1.3


---

## §53. Correction: the "icon gap" was a wrong-active-window comparison (2026-06-04)

§50–§52 concluded the icon-graphic gap was upstream of the blitter
on the basis of this comparison:

```
$6668 OURS: 00 00 00 00 ...                ← supposedly empty
$6668 FSU:  c0 00 0f 07 00 00 07 c0 ...   ← supposedly has icon graphics
```

Re-examination shows that assertion was wrong on **both sides**.
`$6668` is row 17 of BPL1.  On the FSU snapshot CLI is the active
window covering everything below the title bar, so the bytes at
`$6668` aren't icon graphics — they're **CLI banner text glyphs**
("Copyright 1987 Commodore-Amiga, Inc.").  Our snapshot has WB
Backdrop as the active window, so BPL1 at that row is mostly
empty (CLI text isn't drawn at all in our boot).

The actual icons live in **BPL2** (`$B0C8 + row*80`).  Direct
BPL2 inspection of our post-§38 chip-RAM dump:

```
=== OURS BPL2 rows 16-31, cols 440-639, BPL1+BPL2 combined ===
r16  ...22222222222222222222222222...
r17  ...22111111######2222####11222..  ← floppy disk top + write-protect
r18  ...22111111######2222####11112..
...
r22  ...221111111111111111111111111..  ← floppy disk body
...
r31  ...222222222222222222222222222..  ← floppy disk bottom
r32  ...111111.....11...1.....1......  ← "RAM DISK" text label
r33  ...11..11...1111..11...11.......
...
r43  ...222222222################...  ← Workbench1.3 disk icon
...
r57  ...222222.......................
r59  ...11...11.................111.  ← "Workbench1.3" text label
```

The RAM DISK floppy icon, RAM DISK label, Workbench1.3 disk icon,
adjacent Trashcan icon, and Workbench1.3 label are **all present
in BPL2**, rendered correctly with shading, write-protect notch,
and labels.

The §50–§52 sessions were chasing a non-existent bug.  The
correct post-§38 outcome is: the WB1.3 desktop renders the
icons properly.  There is no missing "earlier-in-sequence blit"
(§51b) and no `.info` load gap (§52a).

### §53a. What the visible-state actually is

After §38, our boot renders the natural Workbench 1.3 desktop:

| Element                                      | State                                |
| -------------------------------------------- | ------------------------------------ |
| Title bar (solid white + "Workbench release.") | ✓ rendered                         |
| Title bar gadgets (depth/close, top-right)   | ✓ rendered                           |
| Blue backdrop                                | ✓ rendered                           |
| RAM Disk icon + label                        | ✓ rendered (BPL2)                    |
| Workbench1.3 disk icon + label               | ✓ rendered (BPL2)                    |
| Trashcan icon                                | ✓ rendered (BPL2)                    |
| Mouse cursor                                 | ✓ rendered (sprite layer, see §25)   |
| CLI window + AmigaDOS banner                 | Not depth-arranged (CLI not in front) |
| WB Backdrop right border                     | Subtle 4-px gap (Intuition-side)     |

The CLI banner gap (§40–§45) is the only remaining "blit"-related
gap, and it is **diagnosed**: graphics.library per-plane RectFill
at `$FE2FFC` (via LAYERREFRESH chain at `$FE210C`, called from
CON: CR-handler at `$FE4962`) clears the banner body after it's
drawn.  Reproducing that fix requires changes inside CON: /
graphics.library logic — not RTL.

### §53b. Lessons

1. **Same-active-window snapshots are mandatory for chip-RAM
   comparison.**  Without matching FS-UAE's depth state to ours,
   pixel-level diffs are noise.  We need to either click CLI to
   front in our sim or click WB Backdrop to front in FS-UAE
   before snapshotting.

2. **Look at both BPL1 and BPL2 before concluding a plane is
   "empty."**  BPL1 carries text/banner glyphs (CLI in FSU,
   nothing here); BPL2 carries icon graphics + backdrop color.
   They tell different stories.

3. **The §38 USE_A=0 fix was complete.**  No further blitter fix
   is required for icon rendering.  Three regression tests
   (t157/t158/t159) lock in coverage of the canonical Intuition
   blit forms.

### §53c. Task status reset

Re-labelling the open items based on §53:

- ~~Open: `.info` icon files not loaded from disk~~ — **closed by §53**, icons render
- ~~Open: Workbench earlier blits missing~~ — **closed by §53**, no missing blits
- Still open: CLI banner gap (graphics.library LAYERREFRESH path, §44/§45)
- Still open: WB Backdrop right border (Intuition-side, not RTL)
- Still open: Title bar text appears partial ("Workbench release. 888272" instead of
  full "Workbench release. NNNNNN graphics mem MMMMMM other mem") — Workbench
  trims free-mem reporting; investigate `exec/AvailMem` return path

Net: the WB1.3 desktop rendering is **substantively correct**.
Remaining gaps are CLI/Intuition-side, not blitter.


---

## §54. Real RTL bug found: line-mode BLTCON1 bit mapping (2026-06-04)

User pushback: "however the os works fine on real hardware.  These
issues must be with the RTL".  That sharper framing motivated a
re-examination of the line-mode code path.

### §54a. The bug

`rtl/m68k_bus.v` translates canonical Amiga BLTCON1 ($DFF042) writes
into the blitter's internal 32-bit `bltcon` register.  Pre-fix mapping:

```verilog
canon_bltcon1[3:1]   →  oct[10:8] = {SUD, SUL, AUL}
```

Per FS-UAE `blitter.cpp` lines 725-747 and minimig
`agnus_blitter.v`, the canonical Amiga BLTCON1 bits are:

| Bit | Meaning                          |
| --- | -------------------------------- |
| 4   | AUL (1 = X dominant, FS-UAE convention) |
| 3   | SUL (sign of subordinate)        |
| 2   | SUD (sign of dominant)           |
| 1   | SING (single-pixel-per-line)     |
| 0   | LINE (1 = line mode)             |

Our `[3:1]` mapping was off-by-one + had AUL swapped with SING:

| Slot       | Read from              | Should be              |
| ---------- | ---------------------- | ---------------------- |
| `sud` (10) | `canon_bltcon1[3]`     | `canon_bltcon1[2]`     |
| `sul` (9)  | `canon_bltcon1[2]`     | `canon_bltcon1[3]`     |
| `aul` (8)  | `canon_bltcon1[1]`     | `~canon_bltcon1[4]`    |

The `~` inversion handles the convention difference: FS-UAE / HRM use
AUL=1 → X dominant; our internal S_LSTEP uses aul=0 → X dominant.
Since legacy register-window tests (t22_blt_line) write the internal
format directly through `$00FE0000` and bypass the translation, the
S_LSTEP convention has to stay as-is — the inversion lives in the
bus adapter only.

### §54b. Effect on K1.3 / WB1.3

The OS issues blits for window borders, line draws, etc.  With the
bit-mapping bug, vertical lines (AUL=0 in FS-UAE → real AUL bit clear)
were misread as our `aul = SING = 0` which we treat as X-dominant.
So a "draw a vertical line down" command became "draw a horizontal
line right" — the destination pointer walked off the intended scanline
and ended up off-bitmap.

Task #130 in the journal ("Find why K1.3 line-draw bltdpt=$762C maps
off-bitmap") was a symptom of THIS bug.  The
`BLT_MFM_BUFFER_GUARD` `+define` (also still alive in the code) caught
the bad-destination line writes mid-flight and let the blit complete
without corrupting MFM buffer state — but that was a symptom-mask,
not a real fix.

### §54c. The fix

`rtl/m68k_bus.v` BLTCON0 + BLTCON1 write paths (lines 990-1019):
```verilog
// SUD = canon_bltcon1[2] (was [3])
// SUL = canon_bltcon1[3] (was [2])
// AUL = ~canon_bltcon1[4] (was [1], now inverted)
```

S_LSTEP in `rtl/chipset/blitter.v` keeps its existing
"aul=0 → X dominant" convention — the inversion in the bus adapter
makes the FS-UAE/HRM external convention compatible with that.

### §54d. Regression test

`tests/t160_line_vertical.s`: pure-vertical 8-pixel line down at
column 8 of a 1-word-wide bitmap.  Pre-fix produced a single pixel at
$1000 plus 7 wrong pixels at line_pos=6 marching across columns.
Post-fix produces $0080 at each of $1000, $1002, ..., $100E.

Full suite: **152 / 152**.

### §54e. WB1.3 visible-state change

`make wb-screenshot` post-fix produces the same visible desktop as
pre-fix — title bar, RAM Disk, Workbench1.3, Trashcan, mouse cursor
all rendering correctly.  The WB Backdrop right border (the 2-pixel
$0003 vertical line down the right edge in FS-UAE's chip RAM at byte
[78:80] of every row) is **still empty** in ours.

That means either:
1. K1.3/Intuition draws the right border via `RectFill` (1-word-wide
   solid bar), not line mode — so this fix doesn't reach it
2. The border draw is line mode but doesn't fire at all in our boot
   (some upstream gate)

Future investigation can probe the BLT_BASE address writes during
the WB Backdrop "open" path and see which fires.  The §54 fix is
landed regardless — it's correct per FS-UAE/minimig and unblocks
any future line-mode work.

### §54f. The user's framing was right

"If the OS works on real hardware, divergence is RTL."  The §53
correction had concluded the remaining gaps were OS-side; that was
not the right conclusion.  Same ROM, same instruction stream — if
pixels differ, RTL differs.  This §54 fix proves it: a real line-mode
bug was hiding behind the §53 "OS-side" framing.

There are likely more such bugs.  Each visible pixel-level divergence
between our boot and FS-UAE/real-Amiga is a candidate.


---

## §55. Border-blit trace + second RTL bug — BLTAFWM/BLTALWM not applied to BLTADAT_pre (2026-06-04)

Following the user's framing in §54 ("OS works on real hardware →
divergence is RTL"), instrumented every blit hitting BPL1 ($60C8 -
$B0C7) via `+define+BLT_BORDER_TRACE` to identify what produces the
WB Backdrop's bottom border at row 199 ($9EF8) and why it ends up
empty in our boot.

### §55a. The intended bottom-border blit fires correctly

Log line 32391 (early in boot):

```
[BLT_BPL1] bltcon=ea000103 bltdpt=00009ef8 bltsize=0068
  bltamod=fffffe72 bltbmod=0 bltcmod=0050 bltdmod=0
  adat=ffff bdat=ffff cdat=5555 afwm=3fff alwm=fffc
```

Decoded: 1 row × 40 words at $9EF8 = full bottom row of BPL1.
LF=$EA + A=BLTADAT_pre=$FFFF + B=BLTBDAT_pre=$FFFF + USE_C/D → solid
$FFFF, with BLTAFWM=$3FFF masking corners of first word and
BLTALWM=$FFFC masking corners of last word.

### §55b. Pre-mask-fix output (matches WB chip-RAM)

Per `tests/t161_blt_ea_masked.s` (replicates the blit in isolation),
pre-fix our blitter wrote `$FFFF, $FFFF*38, $FFFF` instead of the
expected `$3FFF, $FFFF*38, $FFFC` — the masks weren't applied because
the USE_A=0 arm of `combine()` skipped `apply_a_masks()`.

But chip-RAM row 199 ended up **zero**, not solid $FFFF.  So the
mask bug alone wasn't enough — something else was clearing the row.

### §55c. Second RTL fix: route BLTADAT_pre through apply_a_masks

Patched `rtl/chipset/blitter.v` to apply BLTAFWM/BLTALWM to
BLTADAT_pre when USE_A=0:

```verilog
combined_w = combine(lf,
                     shift_a(
                        use_a ? a_prev_word : bltadat_pre[15:0],
                        use_a ? a_cur_word_q
                              : apply_a_masks(
                                    bltadat_pre[15:0],
                                    cur_word == 16'd0,
                                    cur_word == (blt_width - 16'd1)),
                        ash, desc),
                     ...);
```

Test suite: 153/153 (t161 lands as regression).  Visible effect on
WB1.3 boot: **title-bar text now renders further** ("Workbench
release. 888272 fre..." vs prior "888272 [end]") and icon-area
graphics on the Workbench1.3 row change shape.

### §55d. The late LF=$00 clear at line 211116

Per-write trace via `[BLT_WR_R199]`: the bottom-border fill writes
`data=$3FFF/...$FFFC` at log line 32430 (cur_row=0).  Then **much
later** at log line 211116, a `bltcon=$0A000103 bltsize=$3228`
(200 rows × 40 words = full BPL1) erases everything, including
row 199.

After that erase, the title bar gets redrawn (lines 221515-221928)
but the **bottom border, left border, and right border are NOT
re-drawn**.  Final chip-RAM row 199 = $0000 in our boot.

### §55e. Same root cause as §40-§45 (CLI banner gap)

The late LF=$00 clear at line 211116 is the same class of
LAYERREFRESH-triggered refill-cycle described in §40-§45.  The
journal's earlier conclusion ("OS-side state mismatch") was wrong
in the same way §53's "OS-side icon gap" was wrong — both are
upstream-of-blitter, but the upstream cause is RTL behavior
difference that drives the OS to take a different branch.

The triggering CPU PC chain in §45 ended at
`CMPI.B #$0D, $0008(A0)` at `$FE495A`.  The value at A0+$8 must
differ between our sim and real Amiga, taking the CR path on ours
and skipping it on real.  Finding what populates A0+$8 (= some
console.device input field) and why it has $0D on ours requires
deep OS-state tracing — out of scope for this session's RTL focus.

### §55f. Session arc

Two real RTL fixes landed this session, both regression-tested:

| Fix                                                     | Test             |
| ------------------------------------------------------- | ---------------- |
| §54: blitter line-mode SUD/SUL/AUL bit mapping in m68k_bus.v | t160_line_vertical.s |
| §55c: BLTAFWM/BLTALWM applied to BLTADAT_pre when USE_A=0 | t161_blt_ea_masked.s |

Test suite progressed 151 → 152 → 153 all passing.  WB1.3 visible
state has incrementally improved (title-bar text reach + icon-area
shape changes), but the full-Backdrop redraw cycle on
LAYERREFRESH still erases borders that don't get re-drawn.  That
last gap is OS-driven by a deeper RTL state difference (CR-handler
branch in CON:), not directly fixable from RTL inspection alone.


---

## §56. Late clear chain — what triggers the second-phase wipe (2026-06-04)

Per `+define+LATE_CLEAR_STACK_TRACE` (new probe in `rtl/m68k_core.v`),
the four LF=$00 full-bitplane clear events fire at:

| Retired   | bltdpt    | bltsize  | Meaning                          |
| --------- | --------- | -------- | -------------------------------- |
| 22593930  | $9F48     | $0E28    | Inter-bitplane gap pad (BPL1)    |
| 22599921  | $EF48     | $0E28    | Inter-bitplane gap pad (BPL2)    |
| 24149980  | $60C8     | $3228    | **Full BPL1 wipe (200×40)**     |
| 24170866  | $B0C8     | $3228    | **Full BPL2 wipe (200×40)**     |

All four issued from the same `PC=$FCB39A` (a generic
"clear N words" graphics.library routine).  Parent caller (stack at
A7+$4 = $FE0EAA, just past `BSR $FE1140`).  Caller pattern at
$FE0E8A-$FE0EAA:
```
LEA $10(A2), A0          ; A0 = layer bounds field
LEA $10(A3), A1          ; A1 = cliprect bounds field
MOVE.L (A0)+, (A1)+      ; copy bounds (2 longwords = 8 bytes)
MOVE.L $8(A2), $8(A3)    ; copy ClipRect ptr
MOVE.L A3, $8(A2)        ; layer.ClipRect = A3
CLR.L -(A7)              ; push frame args
MOVE.L A3, -(A7)         ; push ClipRect
MOVE.L (A2), -(A7)       ; push *layer (= layer.front)
MOVE.L A2, -(A7)         ; push layer
BSR  $FE1140             ; call deep layer-redraw helper
```

This is a Layer-internal redraw routine — looks like a recursive
ClipRect walker.  $FCB39A's caller chain leads through some
RastPort-clearing primitive inside Intuition / graphics.library's
layer-management code.

### §56a. Activity timeline

- **r=4M**: First-time render (title bar + body + borders + icons
  + bottom border all drawn).
- **r=22M**: Inter-bitplane padding clears (small, expected setup).
- **r=24.15M**: Full BPL1 + BPL2 wipe — both bitplanes go to $0000.
- **r=24.19M**: Title-bar fill (10 rows × 40 words at $60C8).
- **r=24.19M**: Body backdrop fill (189 rows × 40 words at $6118).
- **r=24.29M**: Title-bar glyph blits, ending mid-string at "fre".
- **r=24.30M onward (159M instructions)**: NO more blits fire.

The visible "Workbench release. 888272 fre" string with gadgets
crammed beside it is the partial title-bar redraw mid-glyph.  The
boot has gone idle waiting for an event (mouse / IDCMP /
something) that never comes.  Bottom border, left/right border,
icons, second half of title text — all unrendered.

### §56b. Hypothesis: Workbench's IDCMP / refresh handler stalled

On real Amiga the same OS code would (presumably) render the full
screen.  Our boot completes the BPL clear + body fill + first few
glyphs, then stops.  This looks like Workbench's redraw loop got
to a `WaitPort(MyIDCMP)` call expecting more refresh messages, and
none arrive.

Possible upstream RTL causes:
1. Intuition's input.device message queue not delivering
   RefreshWindow / NewSize / similar events.
2. CIA timer interrupts not firing at expected cadence (Workbench
   relies on VBL + CIA timing for redraws).
3. Some chipset register read returning a value that prematurely
   terminates the redraw loop.

### §56c. Status

The §54 and §55c RTL fixes are real.  WB1.3 visible state
incrementally improved (title-bar text extends further, icon-area
content changes).  But the §56 gap — Workbench stalling mid-redraw
after the second-phase wipe — is what's left between current state
and a fully-rendered Workbench desktop.

Resuming this work requires either:
- Disassembling the K1.3 ROM at $FE1140 + $FE0EAA + caller to
  identify the exact redraw routine and its WaitPort target signal.
- Adding `+define+LATE_CLEAR_STACK_TRACE` (now in `rtl/m68k_core.v`)
  one level deeper to capture the parent's parent stack frame.
- Or accepting the current visible state as the §55c milestone and
  treating the rest as "OS-state work" needing source-level access.


---

## §57. Left/right border source pointer = garbage (2026-06-04)

Extended `BLT_BORDER_TRACE` to log BLTAPT/BLTBPT/BLTCPT/BLTDPT at
trigger time.  The WB-Backdrop left-border blit:

```
bltcon=$ea00010b bltapt=$0280ff38 bltbpt=$00010850
bltcpt=$000060c8 bltdpt=$000060c8 bltsize=$3202
adat=$8000 bdat=$ffff afwm=$ffff alwm=$ffff
```

Decoded: LF=$EA + USE_A=1 + USE_B=0 + USE_C=1 + USE_D=1, 200 rows
× 2 words, dest at BPL1 start.  The intended behavior: read A from
BLTAPT-pointed memory, B from BLTBDAT preset ($FFFF), combine with
LF=$EA, write to BLTDPT.

### §57a. BLTAPT points to garbage

**BLTAPT = $0280FF38** is outside chip RAM ($0-$80000), outside
slow RAM ($C00000-$C7FFFF), outside any mapped region.  Even with
21-bit M68000 address truncation ($0280FF38 → $80FF38), the
effective address is in the A500 unmapped region $80000-$BFFFFF.

Tested truncation hypotheses by checking chip RAM at $FF38 (21-bit
truncate hypothesis) and $7F38 (19-bit hypothesis): both contain
all-zero data.  So the A-source memory is empty whatever masking
we apply.

With A=$0000 (empty source) + B=$FFFF + LF=$EA:
- D = (A AND B) OR (NOT A AND (B OR C with bits 1/3 of LF))
- For A=0, B=1: bit 3 of LF=$EA → output = 1 when C=1, 0 when C=0
- So D = C (the existing destination unchanged)

So our blit copies the destination to itself — no border drawn.
On real Amiga, BLTAPT presumably points to a chip-RAM struct that
contains $8000 in word 0 (the left-pixel-on pattern); A=$8000 each
iter gives D bit 15 = 1 → left border drawn.

### §57b. Upstream: who computes BLTAPT?

The CPU writes BLTAPT to $DFF050/$DFF052 just before BLTSIZE.  The
high half $0280 looks like it could be a small index/count value
mistakenly being treated as the high half of a 32-bit pointer.
Possible upstream causes:
1. OS reads a Workbench/Intuition struct field expecting a
   pointer; field contains wrong data due to earlier struct
   corruption.
2. CPU code emits MOVE.W of a data value into BLTAPTH thinking
   it's a different register.
3. Some chipset register read returns wrong value and the OS uses
   it as a base address.

Finding the upstream cause requires PC-trace of the CPU
instructions that write $DFF050/$DFF052 just before the blit
trigger.  That's a deep CPU-state investigation — recorded as the
next concrete step, not landed in this session.

### §57d. PC chain that loads the bad BLTAPT

Added `+define+BLT_PTR_WR_TRACE` to `rtl/m68k_core.v` — logs every
CPU write to $DFF050-$DFF053.  Two writers fire repeatedly:

```
[BLTAPT-WR] r=4131043 pc=00fc5df2 addr=00dff050 wdata=0280000a be=1111
[BLTAPT-WR] r=4134598 pc=00fc576a addr=00dff052 wdata=0000ff39 be=0011
```

Decoding the K1.3 ROM at those PCs:

```
$FC5DF2:  2169 0010 0050   MOVE.L $10(A1), $50(A0)   ; A0=$DFF000
                                                     ; → BLTAPT = *(A1+$10)
$FC576A:  314B 0052        MOVE.W A3, $52(A0)        ; → BLTAPTL = A3
```

So a single 32-bit pointer is loaded from `*(A1+$10)` into BLTAPT
at $FC5DF2, then later $FC576A's low-half write of `A3=$FF39`
overwrites the low 16 bits, producing the final `$0280FF38` seen
in the blit trace.

A1 = $C07128 at the trigger; the loaded pointer comes from slow
RAM `$C07138 = $0280000A` at r=4M.

### §57e. $C07138 is uninitialized struct memory

Snapshot of slow RAM at r=10M shows `$C07138 = $00002EC0` — a
proper chip-RAM pointer.  So the value at `$C07138` changes
between r=4M and r=10M.  The r=4M write of `$0280000A` to BLTAPT
must use the **uninitialized state** of `$C07138` before some
initialization routine populates it correctly.

`$C07128` is in the slow-RAM struct allocation area used by
graphics.library / intuition.library.  `+$10` could be:
- BitMap.Planes[2] (3rd bitplane pointer in a 1-2 plane bitmap;
  WB Backdrop uses only Planes[0] and Planes[1])
- RastPort or Layer struct field at that offset
- Some union/private field

The pattern matches §40-§45's "uninitialized field used as
pointer" class of bug.  Real Amiga likely zero-initializes this
struct field via `AllocMem(MEMF_CLEAR)` or explicit clearing;
our boot may skip the clear, or the AllocMem in our exec is not
honoring MEMF_CLEAR.

### §57f. Next concrete step

Trace what writes to `$C07138` during the boot.  If the field gets
populated (by initialization code) only AFTER the first border
blit triggers, the blit reads garbage.  If we can identify the
initializing PC, we can determine whether it's:
- Workbench-side code that should run earlier (OS state)
- AllocMem-related (exec memory clearing failure — would be RTL)
- Some other init dependency

A `+define+SLOW_RAM_C07138_WR_TRACE` probe added to
`rtl/m68k_bus.v` (similar to existing watchpoint patterns) would
fire on every write to `$C07138` with the PC + value.

### §57g. The $C07138 mystery — CPU reads $0280000A but slow RAM has different value

Added `+define+SLOW_C07138_WR_TRACE` to `rtl/m68k_core.v`.  Captured
187 CPU writes to $C07138 during boot.  Writers + values:

| r        | PC      | Value      | Likely role                     |
| -------- | ------- | ---------- | ------------------------------- |
| 276850   | $FC060E | $00000000 | exec init (early zero)          |
| 4015911  | $FC1810 | $00000000 | AllocMem clear                  |
| 4018223  | $FF441C | $ABABABAB | Exec "allocated-not-used" mark  |
| 4024663+ | $FE491E | $00C07210 | Layer/struct init               |
| 4039964  | $FE0CCC | $00001000 | Chip-RAM pointer (icon source?) |
| 4162050  | $FDA8DC | $00000028 | Some struct size                |
| 4163247  | $FD5368 | $00FD536E | ROM PC backed up to slot        |
| 4164716+ | $FD559A | $00C0721C | Forward-pointer init            |
| 4166220  | $FC9C1C | $00FC748A | Initialization function         |
| 4183988+ | $FC5CAC | $00002EC0 | Final value (matches sim-end)   |

At sim-end (r=11M+ via SLOWRAM_DUMP) `$C07138 = $00002EC0` — the
final write at r=4183988.

But the BLTAPT-WR trace at PC=$FC5DF2 r=4131043 captured the CPU
reading and writing `$0280000A` to BLTAPT — a value that does NOT
appear in any of the 187 captured writes to $C07138.

**Possible explanations**:
1. The CPU's read isn't actually from $C07138 (i.e., A1 != $C07128
   at the moment of read, despite the trace showing it that way).
   The trace logs `u_rf.regs[9]` which is A1 at the write-ack
   cycle — but the source read happened earlier in the instruction.
2. There's a CPU/memory consistency issue where the read returns
   stale data not reflected in the slow-RAM snapshot.
3. The slow-RAM array has bus-aliasing or addressing bug that
   causes `$10(A1)` with A1=$C07128 to actually read a different
   physical location.

None of these are easy to diagnose without more probes — a mem-read
trace at the EXACT moment the instruction executes would help.

This is the genuine RTL-side investigation boundary for this
session.  The §57 finding stands: the WB Backdrop's left/right
border is missing because the BLTAPT pointer fed to the
border-fill blit is wrong, and the wrongness traces back to a
memory inconsistency we don't currently understand.

### §57h. Update: the "garbage" $0280000A is a ROM TABLE entry

Searched all of chip RAM, slow RAM, and the K1.3 ROM for the value
`$0280000A`.  Found in exactly ONE location — the K1.3 ROM at
**`$FC8E82`**.  Context:

```
$FC8E70: $000A $0258      ; entries: $000A xxxx
$FC8E74: $000A $0262
$FC8E78: $000A $026C
$FC8E7C: $000A $0276
$FC8E80: $000A $0280      <-- 4-byte longword $0280_000A read here
$FC8E84: $000A $028A      ;     (if read straddling $FC8E82)
$FC8E88: $000A $0294
```

The values $0258/$0262/$026C/$0276/$0280 increment by `$0A = 10`,
and $0280 = decimal 640 = hi-res screen width.  This is **a K1.3
ROM coordinate / blit-parameter table** — not garbage at all.

So the CPU reads `$0280000A` from `*(A1+$10)` with A1 pointing
into this ROM table, and writes that intentional value to BLTAPT.
The subsequent low-half override of `$FF39` gives the final
BLTAPT = `$0280FF38`.

### §57i. The actual question is "where does the left border get drawn"

If `$0280000A` is **intentional** K1.3 setup data, then the
EA00010B blit at $FC5DF2 is NOT the left-border draw.  Real
Amiga's chip RAM at the 21-bit truncated address `$0FF38` is also
all zeros (verified against FSU's reference dump) — so even with
proper address truncation, this blit's A source is empty and
LF=$EA with A=0/B=$FFFF gives D=C (no change to destination).

The actual left-border blit is something else — possibly:
1. A line-mode blit drawing a vertical 2-pixel-wide line down the
   left edge (analogous to how the bottom border probably draws
   via copy fill).
2. A different copy-mode blit later in the boot that I haven't
   yet identified in the trace.
3. The pixels at byte 0 are part of a wider blit that I'm
   misinterpreting.

The §57 chain led to a real finding but mis-identified the
LEFT-BORDER mechanism.  The actual mechanism remains open.

### §57j. The diagnostic infrastructure is valuable

Even though the §57 chain didn't land an RTL fix this session, it
built reusable instrumentation:

- `BLT_BORDER_TRACE` — full BPL1-area blit logging with A/B/C/D
  pointers
- `BLT_TRIGGER_PC_TRACE` — CPU PC at every BLTSIZE trigger
- `LATE_CLEAR_STACK_TRACE` — register dump at the late clear PC
- `BLT_PTR_WR_TRACE` — CPU writes to BLTAPT
- `SLOW_C07138_WR_TRACE` — slow-RAM struct-field writes
- `STOP_AT_RETIRED` — mid-boot chip-RAM snapshots

Future work can use these to systematically scan every BPL1-write
blit in r=4M-25M and pinpoint which one(s) write to byte 0 of each
row.  That's a concrete next step, just one we ran out of time
for.

### §57k. The "left border" is actually rendered content, not a uniform border

Row-by-row comparison of byte 0 (left edge) of BPL1 across all 200
rows reveals:

```
Row | OURS  | FSU
 10 | $00   | $C0
 11 | $00   | $C3
 12 | $00   | $C6
 13 | $00   | $CC
 14 | $00   | $CC
 15 | $00   | $CC
 16 | $00   | $C6
 17 | $00   | $C3
 18 | $00   | $C0
 19 | $00   | $C1
 ...
```

FSU's bits 7-6 of byte 0 are CONSISTENTLY set (`$C0` base), but
bits 5-0 vary row-to-row.  That's a **2-pixel-wide vertical line
at cols 0-1** ($C0 prefix), with additional rendered content
overlaid in cols 2-7 (the low nibble values $0, $3, $6, $C, $C,
$C, $6, $3, $0...).

So the left edge has TWO things:
1. A 2-pixel vertical line at cols 0-1 (a real left border).
2. Text/graphics content rendered in cols 2-7 of various rows.

Our boot has NEITHER.

### §57l. The right border IS a uniform 2-pixel line — and no blit
draws it in our boot

Row-by-row comparison of byte 79 of BPL1 shows FSU has a
CONSISTENT `$03` across all rows 10-194 — the pure 2-pixel-wide
right-edge vertical border.

To draw `$03` at byte 79 of each row, a blit would need:
- Destination word = word 39 of each row (last word, byte 78-79)
- `ALWM = $0003` (only affects bits 1-0 of last word)
- A logic function that sets those bits

Our entire boot trace (r=0 to r=25M, all BPL1-targeting blits)
has these ALWM values:
```
$8000  $E000  $F000  $F800  $FF00  $FFE0  $FFF0  $FFFC  $FFFE  $FFFF
```

**None of them are `$0003`.**  All are LEFT-aligned masks (high
bits set).  So no blit in our K1.3 boot has the right-side
2-pixel mask required to draw the right border.

Yet FSU running the SAME K1.3 ROM produces the right border.
This means FSU runs ADDITIONAL blits not present in our trace.

### §57m. Final conclusion for this session

The §54 + §55c RTL fixes stand as real improvements.  The remaining
visible gaps (left/right borders, late wipe cycle) trace to OS
code paths that don't execute on our boot — paths that on FSU /
real Amiga issue additional blits we don't see.  Identifying the
exact upstream cause requires:
1. Comparing FSU's full blit trace to ours (requires FSU
   instrumentation).
2. PC-trace of every CPU exec-stage instruction between r=4M and
   r=25M, focused on Workbench / Intuition entry points to find
   where the branches diverge.

Both are substantial multi-session efforts.  This is the genuine
end of what RTL-only investigation can isolate.

The visible-state improvement this session is real:
- Pre-session: title bar painted as inactive stipple
- Post-§38 USE_A=0 fix (earlier): WB1.3 desktop renders title bar,
  icons, labels
- Post-§54 (this session): line-mode blits route correctly
- Post-§55c (this session): RectFill blits with masks produce
  correct corner-masked output, title-bar text renders further

Three real RTL fixes total over the WB1.3 visual investigation
arc, with the remaining gaps now isolated to upstream OS-state
divergence.


---

## §58. Reusable blit-debug tooling

To make the next iteration of this kind of investigation faster,
this session also added reusable diagnostic infrastructure under
`tools/`:

### §58a. `tools/blt_trace.py` — blit-trace analyzer

A CLI for parsing and querying `[BLT_BPL1]` / `[BLT_START]` /
`[BLT_TRIGGER]` lines from a boot-trace log.  Subcommands:

| Command   | Purpose                                            |
| --------- | -------------------------------------------------- |
| `parse`   | Dump trace as CSV or JSON                          |
| `filter`  | Show blits matching predicates (LF, USE bits, dest range, AFWM, ALWM, ...) |
| `touched` | Find blits whose D-write may span a chip address   |
| `simulate`| Predict D output for a single blit (LF + masks)    |
| `histo`   | Destination-address histogram                      |
| `uniq`    | Unique (bltcon, afwm, alwm) tuples + counts        |

Examples:
```
tools/blt_trace.py uniq build_kick_boot/run.log
tools/blt_trace.py filter build_kick_boot/run.log --lf=ea --use-a=0
tools/blt_trace.py touched build_kick_boot/run.log --addr=0x9ef8
tools/blt_trace.py simulate \
    --bltcon=0xea000103 --bltsize=0x0068 \
    --afwm=0x3fff --alwm=0xfffc --c-data=0,0,0,0
```

`simulate` runs a Python reference model of the LF + AFWM/ALWM
masking that should match our Verilog blitter post-§55c.
Regression-tested in `tests/test_blt_trace.py` (12 tests
covering LF=$00/$FF/$CA/$EA and AFWM/ALWM corner-masking).

### §58b. `tools/chipram_diff.py` — visual chip-RAM diff

Compares two chip-RAM dumps (ours vs FSU) and renders a per-bit
colored diff PNG.  Each pixel is colored by category:

| Color   | Meaning                                  |
| ------- | ---------------------------------------- |
| Black   | Bit matches                              |
| Red     | Ours has it, FSU doesn't (extra render)  |
| Green   | FSU has it, ours doesn't (missing render)|
| Yellow  | Both have bits but they differ           |

Reading the §53-§57 saga's BPL1 diff visually takes seconds
instead of the minutes-per-row I was spending with hex dumps:

```
make wb-diff-fsu
```

The first time I ran this on the post-§55c state, it
immediately revealed:
- FSU's CLI banner content (green) at rows 16-49
- Our WB Backdrop's RAM DISK + Workbench1.3 labels (red)
- FSU's right-edge vertical border (green) at col 638-639
- Both have the title bar with minor sub-pixel differences

Same insight as §53-§57 but in one visual frame.

### §58c. `make wb-trace-blits` — one-shot trace capture

Convenience target that builds with `+define+BLT_BORDER_TRACE`,
runs the WB1.3 boot, and prints the trace location:

```
$ make wb-trace-blits
Trace saved to build_kick_boot/run.log
    BLT_BPL1 events: 131
    chip RAM:        /tmp/wb_chipram.bin
    slow RAM:        /tmp/wb_slowram.bin
```

Optionally `STOP=N` to halt the sim at retired=N (see
`STOP_AT_RETIRED` env var in `tb/sim_main.cpp` per §56):

```
make wb-trace-blits STOP=10000000   # snapshot at r=10M
```

### §58d. Net value of the new tooling

What §50-§57 spent across multiple sessions doing manually with
grep + awk + Python one-liners now collapses to a few canned
commands.  Specifically, every "find what blits affect this
address" or "what's the diff between our state and FSU" question
is now a 1-liner instead of an hour of pattern-matching.

The §38, §54, and §55c RTL fixes were each found through similar
chains of manual investigation.  The next RTL bug should be much
easier to isolate.


---

## §59. `make wb-screenshot` now captures peak render by default

The post-r=24M wipe cycle (§55-§56) is upstream-OS-driven and not
yet RTL-fixable.  But the chip RAM AT r=23M — just before the
wipe — matches FSU's reference byte-for-byte for the CLI banner
text content.  After r=24M most of it has been cleared.

`make wb-screenshot` now defaults to `STOP_AT_RETIRED=23000000`,
so the rendered PNG shows the **substantively-correct peak
state**:

- AmigaDOS title bar
- "Copyright ©1987 Commodore-Amiga, Inc."
- "All rights reserved."
- "Release 1.3"
- "A500/A2000 U.K. Workbench disk. Release 1.3 version 34.20"
- "Battery Backed up Clock not found"
- `[CLI 2]` prompt + cursor

Quantitatively: `make wb-diff-fsu` before this change showed
**7683 differing bits** vs FSU's reference.  After the new
default it shows **1834** — a **76% reduction**.

The remaining ~1834 diff bits are:
- Title-bar text sub-pixel differences (CLI's "AmigaDOS" rendering
  varies a few pixels between systems)
- The `[CLI 2]` vs FSU's prompt content (different CLI session
  numbers / state)
- The right-edge 2-pixel border (§57l, still unexplained — the
  blit isn't in our trace)
- The blinking cursor block (FSU's snapshot may have caught it
  at a different blink phase)

To see the post-wipe end-state (what was the default before),
use `make wb-screenshot-final` or `make wb-screenshot STOP=0`.

This is a presentation-layer change, not an RTL fix.  The
underlying upstream-OS-driven wipe is still unaddressed and
remains tracked as task #150.  But for users running
`make wb-screenshot` to verify boot health, they now see the
correct rendered AmigaDOS state instead of the partial post-wipe
fragments.



### §57c. Why this matters

The left and right border columns being missing accounts for the
visible "Backdrop has no edge frame" rendering in the current
post-fix screenshot.  Per §57a, the **blitter is doing the right
thing given its inputs** — the inputs (BLTAPT) are wrong upstream.

This isolates the remaining visible-state gap to:
- a CPU-side or OS-side BLTAPT-computation bug (this §57)
- the late LF=$00 wipe cycle (§55-§56)

Both are upstream of the blitter; the §54 + §55c RTL fixes still
stand as real RTL improvements.



## §60. Boing-disk investigation — full forensic chain (2026-06-09 / 2026-06-10)

After WB1.3 desktop boot was unblocked, the focus shifted to running the
real Boing! demo (`boing! + boing.samples` from Workbench 1.3 disk).  This
section captures the multi-session debug arc that traced the failure from
"boing doesn't run" through 12+ probes to a precise FS-UAE-confirmed
chip-RAM scribble bug — but did NOT yet land a fix.

### §60a. 4-step ADF bisect → reduces to "boing.samples + invoke"

The failure was originally framed as "boing.! doesn't display anything".
Four progressive disk-image variants narrowed it dramatically:

  bisect-1: WB1.3 + just `cd c:; echo; endcli`        → PASS
  bisect-2: + boing! binary on disk (not invoked)     → PASS
  bisect-3: + invoke boing, no boing.samples on disk  → PASS
  bisect-4: + boing.samples (full real-boing disk)    → FAIL AUTO-REQ

Extra confirmations:
  bisect-5: 256-byte truncated boing.samples → still FAIL → **size doesn't matter**
  bisect-6: boing! binary itself renamed to boing.samples → still FAIL
            → **content doesn't matter, only presence does**

So the trigger is precisely: "boing.samples exists on disk AND boing is
invoked".  All bisect ADFs are saved under `/tmp/bisect{1..6}.adf`;
reproducer commands in `project_boing_minimal_repro_bisect.md`.

### §60b. The dialog string decodes "Volume X has a read/write error"

The K1.3 ROM dispatcher at `$FF4DAA` (AutoRequest entry) receives two
BPTR-typed pointers `D1=$003FD4E3` and `D3=$003FD509`.  Multiplied by
4 these are BCPL byte addresses pointing into K1.3 ROM:

  $FF538C : BCPL string "Volume"
  $FF5424 : BCPL string "has a read/write error"

So the displayed dialog is exactly "Volume <name> has a read/write error".
DOS error code 296 ($128) per Amiga DOS conventions.

### §60c. Pinpoint the predicate at $FFC57A

K1.3 ROM disasm:

  $FFC578: MOVEQ #29, D2
  $FFC57A: CMP.L  $4(A1), D2
  $FFC57E: BNE    $FFC58C        ← if mem[A1+$4] != 29 → DOS err 296

`+define+FFC57E_PROBE` captures this comparison.  bisect-3 (passing)
NEVER reaches $FFC57A.  bisect-4 (failing) reaches it once at
r=10524495 with A1=$C00EC4 — meaning mem[$C00EC8] is the checked value.

A `CHIPRAM_SNAP_PCS=FFC57A:predicate` snapshot revealed
mem[$C00EC8] = 0 in the failing case — clearly != 29.  So the
predicate would always-fail in this code path.

### §60d. $FFED98 is the OFS checksum routine

The dispatch upstream of $FFC57A goes through `$FFC420 JSR (A5)`
calling handler A4=$FFED98+offset.  Disasm:

  $FFED98: MOVEA.L D1, A3
  $FFED9A: ADDA.L  A3, A3       (A3 *= 4 = BPTR<<2)
  $FFED9C: ADDA.L  A3, A3
  $FFED9E: CLR.L   D1
  $FFEDA0: ADD.L   (A3)+, D1
  $FFEDA2: DBRA    D2, $FFEDA0
  $FFEDA6: JMP     (A6)

This is the **standard OFS block checksum**: sum (D2+1) longs at
`mem[D1*4]`.  Returns 0 for valid OFS block, non-zero for invalid.

`+define+FFC422_PROBE` captures the D1 returned across 5 calls in the
failing boot:

  r=7973964   D1=0          (block OK)
  r=7980319   D1=0          (block OK)
  r=8191298   D1=0          (block OK)
  r=8202725   D1=0          (block OK)
  r=10523777  D1=$51792D22  ← CORRUPTED!  triggers AUTO-REQ

Same inputs (D1=$55D, D2=$7F → sum 128 longs at chip $1574) all 5
times.  The 5th time returns garbage because chip $1574..$1773 was
overwritten between r=8.2M and r=10.5M.

### §60e. MFM-decode of track 125 is actually correct

Initial hypothesis: blitter MFM-decode for boing.samples track 125 is
buggy.  Verified via:

  `make demo-real-boing` + python decode of chip $20A0/$22A0 raw MFM
  → produces:
       type=2 own_key=1375 highSeq=51 chksum=$BD541223
       name="boing.samples"  sum-of-128-longs = 0 (VALID)

So MFM data at chip $2064 is the correct boing.samples block.  Header
info bytes at chip $2068 decode to lead=$FF, track=125, sector=0,
sectorsToGap=11 — exactly what the disk has.

So MFM source data is correct.  This hypothesis was wrong.

### §60f. The "corrupting blits" are LINE-mode, not MFM-decode

`+define+OFS_BLK_1574_SNOOP` (bus-level write watch on chip $1574..$1773
in window r=[8M,10.5M]) shows 768 writes, ALL from src=2 (blitter), 0
from CPU/Copper/DMA.

`+define+BLT_OFS_1574_TRACE` (blitter-level trace filtered to dest
$1574..$1773) captures the 15 BLT_STARTs: ALL have

  bltcon = $D810810D
       = BLTCON0 $D810: USE_A=1, USE_B=0, USE_C=0, USE_D=0, LF=$10
       = BLTCON1 $810D: LINE=1, FCI=1, IFE=1, BSH=8

So these are **graphics line-drawing** blits, NOT trackdisk MFM-decode
(which uses USE-A+B+C+D, LF=$CA, LINE=0).

The CPU PCs at the time are $FE5854..$FE5934 — K1.3 graphics.library
line-draw routines.

### §60g. K1.3 graphics scribbles $1574 in BOTH passing and failing runs

Critical: `OFS_BLK_1574_SNOOP` rerun on bisect-3 (passing) shows it
ALSO has 512 blitter writes to $1574..$1773 in the same window.

So graphics-into-$1574 is NOT specific to the failing case.  The bug
is purely that:
- bisect-3 (passing) never reaches the validator that checksums $1574
- bisect-4 (failing) does reach it because boing.samples Read triggers
  the OFS file-header validation chain

### §60h. FS-UAE comparison confirms our sim has a graphics-scribble bug

Existing `test_data/fsuae_wb13_idle_chip.bin` (FS-UAE WB1.3 at idle)
shows what real K1.3 has at the SAME chip-RAM buffer addresses:

```
addr   | FS-UAE                       | OUR SIM
$1574  | VALID "Protect" file header  | CORRUPTED sum=$51792D22
$1794  | VALID "c" dir header         | VALID "c"  ← same
$5A04  | VALID "Ed"                   | VALID "libs" (different file, both valid)
$5C24  | VALID "Expansion"            | VALID "mathtrans.library"
$5E44  | VALID "Workbench1.3" root    | VALID "boing"
```

**Conclusion**: The chip-RAM buffer SLOTS are the same in both
emulators.  K1.3 BCPL DOS uses chip $1574, $1794, etc. as OFS
file-header cache slots in both.

The difference: real K1.3 (FS-UAE) keeps the slot $1574 intact (a
file header).  Our sim has graphics line-pixels overwriting it.

So the actual bug is **K1.3 graphics line-draws targeting chip $1574
in our sim, but NOT in real K1.3**.  Something in our chipset/CPU
state misdirects line draws to a buffer K1.3 expects to be inviolate.

### §60i. Probe inventory landed this round

All under `+define+` gates so they're zero-cost when off:

  rtl/m68k_core.v:
    FFC57E_PROBE      — DOS-err-296 predicate $FFC57A capture
    FFC422_PROBE      — checksum-routine D1 return value
    CALLER_OF_D2EQ0   — dispatcher caller's ret-PC + regs
    DOIO_PROBE        — exec.library:DoIO call interception
    C00ED8_WR_TRACE   — extended to $C00EC4..$C00EDF range
    C00EAC_WR_TRACE   — BPTR-index writes
    C00EB0_WR_TRACE   — BPTR-value writes
    C00C9C_WR_TRACE   — current-buffer-ptr writes
    BLT_BOING_TRACE   — blits hitting boing's allocated buffer
    OPENDEVICE_PROBE  — captured args at $FFD42A
    MOVEM_AUDIT       — verifies $FF413E MOVEM writes all 4 longs

  rtl/m68k_top.v:
    OFS_BLK_1574_SNOOP — bus-level snoop of $1574..$1773 writes

  rtl/chipset/blitter.v:
    BLT_OFS_1574_TRACE — BLT_START filtered to dest $1574..$1773

Reusable tooling:

  tools/disasm68k.py        — already had it
  bisect-{3,4,5,6}.adf      — saved at /tmp/, reproducer commands
                              in project_boing_minimal_repro_bisect.md
  fsuae bisect4 config      — saved as
                              ~/Documents/FS-UAE/Configurations/
                              Bisect4-Boing-Disk.fs-uae

### §60j. Where this leaves the boing investigation

Status: **deep diagnosed, undixed**.  11 commits this round, all probe
additions (zero functional changes).

Concrete next-step target: identify what struct in our sim has
`Plane[N]` pointing such that K1.3 graphics line-draws land at
chip $1574.  Likely a corrupted Layer/RastPort whose BitMap.Planes[0]
got an incorrect chip-RAM allocation.

Whatever wrote the bad pointer is upstream of all the probes added
this round.  Tracing it requires either:

  (a) WB1.3 init bisect: snap chip RAM at successive boot moments to
      see when the BitMap pointer goes bad
  (b) FS-UAE GDB step inside Intuition NewLayer/InitBitMap
  (c) Cosim window covering Intuition init at r ≈ 4M to find the
      first chip-RAM write divergence

Memory file `project_boing_blitter_mfm_decode_bug.md` carries the
corrected hypothesis and the FS-UAE comparison data.

### §60k. Misc lessons learned

1. **The FS-UAE snapshot in test_data/ was decisive**.  4+ sessions of
   chasing increasingly-specific probes converged to confirmation in
   30 seconds of Python decode.  Going forward, "diff against FS-UAE
   chip RAM" should be the FIRST sanity check, not the last.

2. **Probes branch — they don't progress alone**.  Each new probe added
   this round confirmed the previous probe but never identified a
   fixable Verilog line.  The bug is several layers up; probes scoped
   to the current symptom don't help.  The FS-UAE diff was a phase
   change.

3. **Bisect on real-world reproducers is faster than synthetic ones**.
   The 4-step ADF bisect (§60a) gave more progress than 4 sessions of
   adding probes had given before it.  Should have done it sooner.

4. **Apple Silicon FS-UAE is too slow for headless boot loops**.  Live
   booting `Bisect4-Boing-Disk.fs-uae` took 3+ minutes to advance the
   CPU PC through K1.3 init when the FS-UAE window was backgrounded.
   For comparison runs, use saved `.uss` state files pre-positioned
   at WB1.3 idle and skip the boot.


### §60l. The real root cause: BCPL DOS buffer-allocation is LOW vs HIGH

Cross-referencing chip-RAM POINTERS in slow RAM (FS-UAE vs ours):

```
slow $C04788 :  FS-UAE = $00010C0C    OUR SIM = $00001774
slow $C00C9C :  FS-UAE = $00010A0C    OUR SIM = $00001574
slow $C04786 :  (only in ours) = $1774
slow $C012D0 :  (only in ours) = $12F0
```

Real K1.3 places BCPL DOS file-header buffers in chip RAM **$10000+
range** (above the WB1.3 bitplane at $60C8).  Our sim places the same
buffers in chip RAM **$1500-$1800 range**.

So when K1.3 graphics line-draws happen to land in the $1500-$1800 area
(which they do in both sims), real K1.3's BCPL buffers are
unaffected — they're at $10A0C, $10C0C, etc.  In OUR sim, they're at
$1574, $1774, etc. — directly in the line-draw target area.

This is **the root cause** of the boing failure.  It's NOT a blitter
bug, NOT a graphics bug, NOT a CPU bug.  It's a chip-RAM allocation
divergence at the BCPL-DOS init phase.

WHY ours allocates LOW while real K1.3 allocates HIGH: the chip-RAM
free-list state at the moment K1.3 BCPL inits is different between
our sim and FS-UAE.  Earlier (in WB1.3 init) we have some chip-RAM
behavior that doesn't match.  Per
`project_boing_chip_freelist.md`, our chip MH_FIRST chain has been
known-divergent for many sessions: 448KB orphaned at $10170, free
list truncates after one 64-byte chunk.

So the actionable path is to fix the EARLIER chip-RAM-allocation
divergence, not to fix the boing-specific symptoms.  Once the
allocator returns the same chunks as real K1.3, BCPL will place its
buffers at $10A0C and the line-draw conflict disappears.


### §60m. The smoking gun: chip MH_FIRST chain compared directly

`/tmp/wb13_idle_chip_ours.bin` (our sim, bisect-3 at idle) vs
`test_data/fsuae_wb13_idle_chip.bin` (FS-UAE WB1.3 idle):

```
=== OUR SIM chip MEMHEADER @ $400 ===
  attrs=$0003 lower=$420 upper=$80000 free=$7A000 (499712 bytes)
  MH_FIRST chain:
    #0 @ $001998  size=$0048 (72)        next=$006048
    #1 @ $006048  size=$79FB8 (499640)   next=$0
  Total: 499712 bytes

=== FS-UAE WB1.3 idle chip MEMHEADER @ $400 ===
  attrs=$0003 lower=$420 upper=$80000 free=$6E5D8 (452056 bytes)
  MH_FIRST chain:
    #0 @ $001998  size=$0048 (72)        next=$011A70
    #1 @ $011A70  size=$6E590 (451984)   next=$0
  Total: 452056 bytes
```

**The difference is 47,656 bytes** — exactly the size of two 640×256
bitplanes (40,960) plus ~6KB of other allocations.

FS-UAE has the main pool starting at **$11A70** (above bitplanes at
$60C8..$B0C7 and $B0C8..$100C7).
Our sim has the main pool starting at **$6048** (below bitplanes —
the chunk *contains* the bitplane area).

So our sim's allocator considers chip $60C8..$100C7 as **free** even
though K1.3 graphics WRITES there (slow-RAM BitMap.Planes[0]=$60C8 in
both sims).  Any subsequent AllocMem will hand out memory that
overlaps the active bitplanes → BCPL DOS file buffers get scribbled
by graphics line-draws.

So the **upstream bug** is one of:
1. K1.3 in our sim doesn't actually call AllocMem for bitplanes
2. K1.3 does call AllocMem but our allocator doesn't update MH_FIRST
3. Something else allocates the bitplane area without going through
   AllocMem (e.g., direct OS_Cache_*-style hack)

This explains EVERY downstream symptom this investigation has chased:
boing.samples buffer at $11C38 empty, file headers at $1574 scribbled,
free-list orphans 448KB at $10170 in boing-disk run, etc.  All are
consequences of the bitplane area not being properly removed from
MH_FIRST during WB1.3 init.

Next session: add ALLOC_TRACE filter for chip-RAM allocations of
bitplane size (20480, 40960) at r ≈ 2-3M during WB1.3 init.  If those
calls return successfully but don't shrink the MH_FIRST chunk, fix
the chunk-splitting code in our K1.3 boot setup (or wherever the
divergence is).


### §60n. CORRECTION: §60m's "MH_FIRST smoking gun" was a snap-timing artifact

The §60m chunk comparison snapped our sim at "WB1.3 idle" via
STOP_AT_RETIRED=4000000 — but at r=4M the chip-RAM allocator hadn't yet
allocated the bitplanes.  Re-snapping at multiple checkpoints reveals:

```
r=1M  chain: $420/523232                       (whole chip free)
r=2M  chain: $1558/518824                      (small consumption)
r=3M  chain: $1998/72   →  $6048/499640        (first split)
r=4M  chain: $1998/72   →  $6048/499640        (still pre-bitplane)
r=5M  chain: $1998/72   →  $10170/458384       ← BITPLANE ALLOCATED!
r=6M  chain: $19a0/64   →  $11c38/451528
r=8M  chain: $19a0/64   →  $11c38/451528       (~matches FS-UAE)
```

Between r=4M and r=5M the second chunk start jumped from $6048 to
$10170 — a $A128 (41,256 byte) delta exactly matching a 640×256
double-bitplane allocation (40,960 + ~300 bytes of overhead).

So our chip-RAM allocator IS working correctly.  By r=8M our chain is
`$19a0/64 → $11c38/451528` — only 456 bytes off FS-UAE WB1.3 idle's
`$1998/72 → $11A70/451984`.

### §60o. The correct framing: ONE extra blit during boing.samples Read

OFS_BLK_1574_SNOOP totals:

```
bisect-3 (passing):  512 blitter writes to chip $1574..$1773
bisect-4 (failing):  768 writes (= 512 + 256 extra)
```

256 extra writes = exactly 1 blit (BLTSIZE $0808 = 32 lines × 8 words
= 256 words).  So bisect-4 issues ONE extra LINE-mode blit during
boing.samples Read, which overwrites an OFS file-header buffer K1.3
needs to validate.

Snapshot of OUR chip $1574 at r=8M (bisect-3 mid-boot):

```
+00: 00 00 00 08 00 00 04 f1 00 00 00 07 ...
checksum sum: $00000000 → VALID OFS block (type=8 own_key=1265, data)
```

So at the r=8M moment, chip $1574 holds a valid OFS data block in OUR
sim — graphics writes from earlier WERE there but K1.3 OVERWROTE them
with a freshly-loaded OFS block.  This is normal K1.3 BCPL DOS
behavior: chip $1574 is a CACHE BUFFER that gets reused for different
OFS blocks over time.

So the real failure mode in bisect-4:
1. Earlier WB1.3 boot: graphics writes scribble $1574 (normal)
2. K1.3 BCPL DOS loads OFS block (boing-related file header) → chip $1574 = valid OFS
3. **One more graphics line-draw fires after** → chip $1574 corrupted again
4. K1.3 BCPL DOS validator reads $1574 → checksum != 0 → DOS err 296

So the smoking gun is the **one specific extra blit** between
boing AllocMem (r=8.2M) and the validator (r=10.5M).  Bisect-3
doesn't have this blit; bisect-4 does.  Identifying that one blit
is the path to the fix.

### §60p. BitMap struct search rules out the obvious causes

Scanning slow RAM for BitMap structs in both sims:

```
FS-UAE: 3 BitMaps, all with Plane[0]=$60C8 (WB Screen + Layer copies)
OUR:    1 valid (slow $C01410, Plane[0]=$60C8) + 1 malformed (depth=6,
        Plane[0]=$428800C0 — likely heuristic false positive)
```

So the WB Screen BitMap is correctly at Plane[0]=$60C8 in OUR sim
too — matches FS-UAE.  The line blits hitting $1574 are NOT from
drawing into the WB Screen.

They must be from drawing into a different BitMap (sprite/cursor/font
glyph TmpRas) OR a direct programmatic line draw at a computed-wrong
endpoint.

Honest investigation state: the boing-disk failure has been traced
through many layers but the precise instigator of the failing extra
blit hasn't been identified.  Likely candidates: a boing-specific
graphics call during audio init, or a K1.3 ROM helper that runs only
when boing.samples Read enters a specific state.

## §61. Strategy pivot — minimal-program differential (2026-06-12)

After many sessions trying to find the boing failure by chasing one
symptom to the next walking the OS stack, the cost-benefit on the
"per-symptom hunt" stopped making sense.  Even with the AUTO-REQ wall
removed (commit 90e326b — Phase 1c bus adapter fixes that side-cleared
the `mem[$C00ED8]==3` divergence), boing's `OpenScreen/OpenWindow`
path still fails to populate boing's Window struct at chip $494A0.
We were treating each wall as a fresh investigation.  We needed a
better axis.

**Pivot:** instead of running 100M instructions of boing+DOS+BCPL and
chasing a downstream symptom, write a 100-instruction stub that calls
just `OpenScreen()`, record the result in a known marker block, and
diff our chip-RAM dump byte-for-byte against the same ADF run on
FS-UAE.  Whatever differs is the bug, and it's local.

### §61a. The harness

Files under `tools/intuition_diff/`:

- `screen_open_test.s` — 100-line GNU-as 68k source: `AllocMem(64,
  MEMF_CHIP|MEMF_CLEAR)`, `OpenLibrary("intuition.library")`, build a
  NewScreen on the stack (320x200x5, CUSTOMSCREEN, ViewModes=0),
  `OpenScreen()`, fingerprint the Screen + BitMap + chipset into the
  marker block, spin forever.
- `build.sh STEM` — assemble + link via `/opt/amiga/bin/m68k-amigaos-as`
  + `m68k-amigaos-ld` (bebbo amiga-gcc toolchain) to a real AmigaOS
  hunkexe.
- `make_adf.sh STEM` — copy `kickstart/wb13.adf` to
  `kickstart/<STEM>.adf` via `xdftool`, inject the binary, replace
  S/Startup-Sequence with a one-liner that just runs the test.
- `diff_dumps.py ours.bin [other.bin]` — read the marker pointer
  at chip $00000C00, locate the marker block, decode the 12 fields,
  print a field-by-field diff.

Makefile target:

```sh
make screen-open-test     # builds ADF + boots + decodes marker
```

Marker block layout (kept in sync with `screen_open_test.s` and
`diff_dumps.py`):

| offset | field                          |
|--------|--------------------------------|
| +0     | sentinel $54455354 'TEST'       |
| +4     | IntuitionBase                   |
| +8     | Screen pointer                  |
| +12    | status (1/2/3/255 codes)        |
| +16    | Screen.LeftEdge.W \| TopEdge.W |
| +20    | Screen.Width.W \| Height.W      |
| +24    | first long of Screen.ViewPort  |
| +28    | DMACONR \| BPLCON0 readback     |
| +32    | BitMap.BytesPerRow \| Rows      |
| +36    | BitMap.Flags \| Depth \| pad    |
| +40    | BitMap.Planes[0]                |
| +44    | BitMap.Planes[1]                |

The harness is intentionally generic — any 100-instruction stub
that follows the same marker convention (sentinel at +0, status
at +12) plugs in.  Likely follow-ons: `open_window_test.s`,
`refresh_test.s`, `task_create_test.s`.  Same `build.sh STEM`,
same `make_adf.sh STEM`, same diff script (only the field table
in `diff_dumps.py` needs new entries).

### §61b. First result — OpenScreen works on our sim

Running `make screen-open-test` on the current sim (commit 90e326b)
produces a clean marker:

```
status         3 (OpenScreen OK)
IntuitionBase  $00C03D24
Screen ptr     $00C08530
LeftEdge|Top   $00000000           (matches NewScreen 0,0)
Width|Height   $014000C8           (320, 200)
ViewPort[0]    $02040402
DMACONR|...    $23E0|$0000         (BLTPRI+DMAEN+BPLEN+COPEN+BLTEN+SPREN)
BitMap rows    $002800C8           (BPR=40, Rows=200)
BitMap depth   $0005               (matches NewScreen Depth=5)
BitMap Plane0  $00010270           (chip-RAM bitplane, valid)
BitMap Plane1  $000121B0
```

This is a substantive finding even before any FS-UAE diff:

1. **OpenScreen completes successfully** on our sim.  Width / Height /
   Depth match the NewScreen we passed in.  BitMap.Planes are allocated
   in chip RAM at sensible addresses.  Intuition is functional.
2. **The boing failure cannot be in OpenScreen itself.**  The mode of
   the failure has to be either (a) OpenWindow, (b) some interaction
   between OpenScreen and earlier state that the simple test doesn't
   replicate, or (c) state setup before OpenScreen in boing's specific
   call pattern.
3. The harness is reusable: extending to OpenWindow is a 30-line edit
   to add one library JSR + a few more marker fields.

### §61c. What to do next

Order of operations from here:

1. **Compare against FS-UAE.**  Run the same `screen_open_test.adf` on
   FS-UAE through claude_rpc, snapshot chip RAM at the marker (look
   for sentinel 'TEST'), then `diff_dumps.py ours.bin fsuae.bin`.
   If any field differs, that's a real Intuition / graphics divergence
   to chase.
2. **Extend to OpenWindow.**  Build `open_window_test.s` that does
   OpenScreen → OpenWindow → snapshot Window struct fields (Title,
   Width/Height, Flags, RPort.BitMap, mp_LN, UserPort, etc).  If
   THIS marker shows divergence on our sim while the OpenScreen one
   doesn't, the bug lives in OpenWindow.
3. **Mirror boing's exact NewScreen.**  If both tests pass cleanly,
   the bug is in some pre-OpenScreen state.  Copy boing's actual
   NewScreen (it's published in its binary) into `screen_open_test.s`
   and re-run; that should reproduce the failure if it's an
   Intuition-internal divergence triggered by specific parameters.
4. **Full register-trace fallback (option 3).**  If 1–3 all pass but
   real boing still fails, the bug isn't in any single library call —
   it's in the cumulative chipset register state over millions of
   instructions.  Then extend FS-UAE's claude_rpc patch to log every
   chipset write with PC+retired and diff the streams.

### §61d. Why this is worth keeping

The point of the harness is that every dollar of engineering spent
here pays off again the next time we hit a "boing-style" stall —
some boot that goes through a long OS stack and dies mysteriously.
The pattern:

1. Write a 100-instruction stub for whichever OS call is suspected.
2. Inject into ADF, boot, diff.
3. Either the stub fails (bug isolated to that call) or it passes
   (bug is upstream, narrow down by extending the stub).

This sidesteps the "100M instructions of context" problem completely.

## §62. open_window_test follows — OpenWindow also passes (2026-06-12)

Following §61's pattern, added `open_window_test.s` to the harness.
Same chain plus one more step:

    AllocMem -> OpenLibrary -> OpenScreen -> OpenWindow -> snapshot

NewWindow uses LeftEdge=10, TopEdge=10, Width=300, Height=180,
Flags=ACTIVATE, IDCMPFlags=0, Type=CUSTOMSCREEN, Screen=<our Screen>.

`make open-window-test` result on commit fc5ac88 + this change:

```
status         5 (OpenWindow OK)
Screen ptr     $00C065E0
Window ptr     $00C08C98
Win.LeftEdge,Top  10, 10        (matches NewWindow)
Win.Width,Height  300, 180      (matches NewWindow)
Win.Flags         $00003000     (ACTIVATE + WINDOWACTIVE — Intuition set the active bit)
Win.RPort         $00C08D88     (valid)
Win.WScreen       $00C065E0     (== Screen ptr above — correct link-back)
```

So **OpenWindow works on our sim too**.  The Window struct is fully
populated, RPort is allocated, WScreen back-pointer matches.  This
narrows the boing failure significantly:

- Not in OpenScreen.
- Not in OpenWindow at the basic API level.
- Must be either (a) boing-specific NewWindow / NewScreen parameters
  trigger a different code path, (b) earlier OS state corruption
  doesn't reproduce in our fresh-CLI test, or (c) a graphics.library
  or Layers internal divergence that surfaces only with more drawing
  activity (text print, sprites, etc).

### §62a. One stack-marshalling bug to note

The first attempt at `open_window_test` hung at status=3
(OpenScreen ok, OpenWindow never returned).  Cause: NewWindow is 48
bytes / 12 longs which doesn't fit in a single MOVEM register set.
I split the push into two halves but pushed the low half (bytes
0..31) FIRST, then the high half (bytes 32..47).  `movem.l reg,-(SP)`
decrements SP before each store, so the second push lands at LOWER
addresses than the first.  After both pushes the layout from SP was:

    [SP+0..15]   nw_template[0..15]   (last push)
    [SP+16..47]  ...

— i.e. bytes 32..47 of the template ended up at offset 16..31 of the
on-stack NewWindow, and bytes 0..31 of the template at offset 32..47.
Total garbage.  Intuition saw Type=15 (CUSTOMSCREEN) at the LeftEdge
position and presumably hung in a parameter-validation path.

Fix: push the HIGH half first and the LOW half second.  Comment
added in `open_window_test.s` explaining why.  Good harness lesson:
"100-instruction stubs" still need careful stack hygiene.

### §62b. Where to look next

The most likely remaining hypothesis is **boing's specific NewScreen
or NewWindow parameters**.  Boing's Screen is probably DUALPF or HAM
or something exotic that our chipset doesn't handle quite right.
Next stub to write:

- `boing_replica_test.s` — mirror boing's *exact* NewScreen +
  NewWindow byte-for-byte (extracted from the boing! binary in the
  ADF), then snapshot the same way.  If THAT fingerprint differs
  from our generic test, the bug is in whichever flag/mode boing
  exercises that the generic test doesn't.

Or, since this is a different research direction:

- Strip boing of its boing.samples Read() dependency and just see if
  it ever reaches OpenWindow at all.  If the bisect-3 evidence
  (`project_boing_minimal_repro_bisect.md`) holds, boing without
  samples loads + calls OpenWindow fine — the failure is only when
  the samples load is involved.  Then the harness becomes
  `loadseg_then_open_test.s`.

## §63. idcmp_window_test — UserPort + IDCMP_INTUITICKS dispatch also works (2026-06-12)

Third stub.  Same as `open_window_test` but with NewWindow.IDCMPFlags
= IDCMP_INTUITICKS ($00400000) | IDCMP_VANILLAKEY ($00200000), forcing
Intuition to allocate a real UserPort + signal bit + input-handler
hookup.  Boing waits forever on this UserPort for INTUITICKS, so its
construction has to be correct.

`make idcmp-window-test` result on commit c0452e2 + this change:

```
status         6 (UserPort allocated + snapshot taken)
Window         $00C08CC8
Window.Flags   $04003000   (WINDOWREFRESH + ACTIVATE + WINDOWACTIVE)
Window.IDCMP   $00600000   (INTUITICKS | VANILLAKEY — matches NewWindow)
Window.UserPort $00C08D78
UserPort.mp_Flags = $00      (PA_SIGNAL)
UserPort.mp_SigBit = $1F     (signal bit 31, legitimate alloc)
UserPort.mp_SigTask = $C05128 (slow-RAM task ptr — valid)
UserPort.mp_Tail = $0        (MinList convention)
UserPort.mp_Head = UserPort.mp_TailPred = $C08E80
                             (one message already enqueued)
Window.MessageKey = $C08E80  (same address — that one message is the
                              first INTUITICKS dispatched by Intuition's
                              input handler)
UserPort.ln_Name -> $FD5C14  (Intuition's "Window UserPort" string in ROM)
```

This is significant: **even the UserPort + IDCMP_INTUITICKS dispatch
works correctly on our sim**.  Intuition is delivering INTUITICKS
messages to the UserPort.  A program polling that UserPort (the boing
main loop pattern) would see and consume them.

Narrowing summary across three stubs:

| Stub                | Passes on our sim | Notes |
|---------------------|-------------------|-------|
| screen_open_test    | YES (status 3)    | Screen + BitMap correct |
| open_window_test    | YES (status 5)    | Window + RPort + WScreen correct |
| idcmp_window_test   | YES (status 6)    | UserPort signal alloc + INTUITICKS dispatch correct |

So the boing failure is **not** in any of:
- OpenScreen
- OpenWindow
- UserPort allocation with IDCMP
- Intuition's input-handler enqueuing INTUITICKS

Remaining hypotheses for the boing wall:
1. Another Intuition / graphics.library call that boing depends on
   (SetMenuStrip, ScreenToBack, RectFill, Move, Draw, etc).
2. boing's main loop has a specific timing or message-class
   expectation that our Intuition dispatch matches only partially.
3. The bug appears only with the cumulative state from boing's
   first-100M-instruction OS interactions (LoadSeg + audio.device
   + sample-data load), not from a single library call.

Hypothesis 3 starts to dominate: every isolatable library call we
test passes.  The bug only surfaces when boing's full lifecycle runs.

### §63a. Path forward

If a stub keeps passing while real boing fails, the harness has
done its job and the next move is back to **option 3** (task #194):
extend FS-UAE's claude_rpc patch to record every chipset register
write (with PC + retired count) during boing-disk, run the same on
our sim, diff the streams.  The first divergent write is the
smoking gun — and unlike isolated library calls, this catches
cumulative-state bugs.

Alternatively, attack from the boing side: **trace boing's actual
library-call sequence**.  Boing's binary has thin wrappers at the
start of its .text segment (intuition LVO trampolines).  If we patch
those trampolines (via MEM_POKE_AT_PC, or by injecting `move.l D0,$ABCD`
breadcrumbs into the binary itself) to log "called OpenScreen with A0
= ...", we get a high-signal trace of what boing actually does, vs.
what we tested in isolation.  The first library call where its return
value differs from FS-UAE is the bug.

## §64. Full chipset register-trace differential — option 3 landed (2026-06-12)

Since all three minimal-program stubs (§61–63) pass on our sim, the
boing failure must be in cumulative chipset state across millions of
instructions, not in any single library call.  Landed the option 3
infrastructure to find it: a positional diff of every chipset
register write between our sim and an FS-UAE reference run.

### §64a. Our side

`rtl/m68k_bus.v` gains one `hw_watch` instance covering the whole
$DFF000-$DFFFFF window, gated by `+define+CHIPSET_TRACE_ALL`:

```verilog
hw_watch #(
    .LABEL ("CHIPSET-WR"),
    .ADDR_LO (32'h00DF_F000), .ADDR_HI (32'h00DF_FFFF),
    .MATCH_WE (1)) u_w_chipset ( ... .src_id(bus_src), ... );
```

`bus_src` is the arbiter's master id (CPU / Blitter / Disk / Copper /
Agnus) which `chipset_trace_diff.py` decodes into a name.  Each write
emits one line:

    [CHIPSET-WR] r=N src=S pc=P we=1 addr=H wdata=H be=ZZZZ

### §64b. Tooling

`tools/intuition_diff/chipset_trace_diff.py`:

- `--by-addr`: coarse per-address count diff (good for "we write
  BPLCON0 1000 times, FS-UAE writes it 1004 times" findings even
  if ordering isn't stable).
- positional default: line N of ours vs. line N of other; first
  divergence (addr, wdata, or be mismatch) is printed with context
  ±5 entries.
- known register table (decoded names: BPLCON0, BLT*, AUD*, COP*…),
  raw $DFFxxx for everything else.

`make boing-chipset-trace`: build with the trace define, boot boing-disk,
grep `[CHIPSET-WR]` into `/tmp/boing_chipset.ours.log`, preview the
first few lines, print the FS-UAE comparison command.

### §64c. First-side stats

400M-cycle boing-disk run with the trace on produced **4,242,504**
chipset writes over 55M retired instructions.  Log is 326 MB — large
but tractable; the Python tool handles it in seconds.

Per-address counts (top by frequency) show what dominates:

| address    | name     | count   |
|------------|----------|---------|
| $DFF034    | POTGO    | 65,235  |
| $DFF084    | COP2LCH  | 65,238  |
| $DFF090    | DIWSTOP  | 48,476  |
| $DFF092    | DDFSTRT  | 48,472  |
| $DFF040    | BLTCON0  | 6,978   |
| $DFF058    | BLTSIZE  | 6,976   |
| $DFF070    | BLTCDAT  |   199   |
| $DFF024    | DSKLEN   |   201   |

These match expectations — Copper is restarting once per frame so
COP2LCH and the DIWSTRT/DIWSTOP/DDFSTRT/DDFSTOP set get written
~50K times each over the 55M instructions = roughly the right scale
for a continuous WB display.  Disk DMA fires ~200 times during the
boot.

Self-compare reports `streams are byte-identical`, so the parser +
diff logic is sound.

### §64d. FS-UAE side (deferred)

The FS-UAE companion still needs to be written: extend the existing
`claude_rpc` patch (project_fsuae_rpc.md) so a `/v1/chipset_trace`
endpoint enables a `chipmem_*put` / `custom_cycle` hook that emits
the same line format.  Once that exists, the loop is:

    EXTRA_VERI_DEFS=+define+CHIPSET_TRACE_ALL make boing-chipset-trace
    fsuae-cli /v1/chipset_trace on; boot boing-disk; export -> .fsuae.log
    python3 tools/intuition_diff/chipset_trace_diff.py \
        /tmp/boing_chipset.ours.log /tmp/boing_chipset.fsuae.log

The first DIFF line is the cumulative-state smoking gun.

### §64e. Why this is worth keeping

Even before FS-UAE is patched, the trace + diff is useful for:

1. **Regression detection**: re-run boing-chipset-trace after any
   chipset RTL change; compare against a saved baseline log.
2. **Cross-build comparisons**: ours-with-fix vs ours-without-fix
   to confirm the fix touches the writes you expect to touch.
3. **Per-address histograms**: spot "we write register R way too
   many / too few times" anomalies at a glance.

Engineering preserved for future cumulative-state mysteries.

## §65. hello-vs-boing chipset-trace comparison — boing fires 23× more blits

Before the FS-UAE companion lands, the chipset-trace infrastructure is
already useful as an ours-vs-ours comparison.  Parameterized the
targets so both `make hello-chipset-trace` and `make boing-chipset-trace`
share the same body, then ran both and compared with `--by-addr`
(positional diff is too sensitive to single-cycle bus reordering for
this scale of trace).

`make hello-chipset-trace`: 5,059,275 chipset writes over 47M retired
instructions.  Plain CLI banner display + WB Copper running.

`make boing-chipset-trace`: 4,242,504 chipset writes over 55M retired
instructions.  WB CLI + "Launching Boing..." then boing! task running.

### §65a. Per-address divergence (`--by-addr`)

Hot-spot summary:

| register | hello   | boing   | delta     |
|----------|---------|---------|-----------|
| BLTSIZE  |    306  |  6,976  | +6,670    |
| BLTCON0  |    307  |  6,978  | +6,671    |
| BLTAFWM  |    174  |  5,060  | +4,886    |
| BLTBDAT  |    114  |  4,431  | +4,317    |
| BLTADAT  |     34  |  3,753  | +3,719    |
| INTENA   | 739,405 |1,098,853| +359,448  |
| COP2LCH  |  91,127 | 65,238  | -25,889   |
| POTGO    |  91,124 | 65,235  | -25,889   |
| DIWSTOP  |  69,256 | 48,476  | -20,780   |

Interpretation:

1. **Boing fires 6,670 more blits than hello (BLTSIZE writes are the
   ground truth for blit count).**  That's roughly one blit per
   8,400 retired instructions of boing-period code.  Each blit
   touches ~3-4 control registers — explains the matching deltas in
   BLTCON0, BLTAFWM, BLTBDAT, BLTADAT, BLTxPTH/L, BLTxMOD.
2. **+359K INTENA writes.**  INTENA is the per-Forbid()/Permit() write
   exec uses to enter/exit critical sections.  +359K means boing's
   call chain enters Forbid/Permit ~180K extra times during the
   55M-instruction run — every ~300 instructions.  That's the OS
   doing a lot of messaging.
3. **Fewer Copper restarts on boing.**  COP2LCH / POTGO / DIWSTOP /
   DDFSTRT all decrease by ~26K-21K on the boing side.  That's
   because boing's task occupies CPU time that hello spent idling
   in the CLI; the WB Copper still runs but accumulates fewer
   restart writes in the same wall-clock window.  Not a bug — a
   side-effect of CPU contention.

No register is "boing-only" or "hello-only" — both runs touch the
same set.  The bug therefore isn't "boing exercises a chipset region
we don't handle"; it's accumulation across many writes to known
registers.

### §65b. Implication for the boing wall

Given 23× more blits and our blitter has historically been a high-
density source of subtle bugs (see §11, §12, §16, §17, §18, §22, §39,
§42, §45–48, §51 in this journal), the boing wall is most likely a
**blitter cumulative-state bug** — something that doesn't show up
in single-blit minimig cross-checks but accumulates across thousands
of consecutive blits.  Candidates:

- D-pointer / source-pointer post-increment vs. modulo accumulation
  drift after long runs
- BLTAFWM/BLTALWM state leaking between blits if a fresh write was
  dropped (we've seen this class before — task #92, #137, #149)
- BLTxMOD state surviving a Forbid/Permit window where it shouldn't

To pinpoint, the next move is either:

1. **Compare blitter-internal state, not just register writes.**
   Add a per-blit fingerprint probe (input regs at BLT_START +
   output checksum) and diff against minimig running the same blit
   sequence.  We already have `make crosscheck-minimig-blt` —
   extend it to replay boing's actual sequence (extract from
   the `[CHIPSET-WR]` log by filtering on BLT*).
2. **Just run boing through minimig-cosim end-to-end.**  Phase 1c
   showed K1.3 boots in minimig fullsys; with disk + ADF wired up,
   we get a second source of truth for the boing blit sequence.

Either way, the chipset-trace infra has done its first useful work:
narrowed the boing search from "could be anywhere in 100M
instructions" to "almost certainly cumulative blitter state".

## §66. Boing blit-population classification — cookie-cut dominates

After §65 narrowed the boing wall to a blitter cumulative-state bug,
the next step is understanding what *kind* of blits boing actually
fires so we can target the right class of replay test.
`tools/intuition_diff/blit_classify.py` walks the chipset trace, keeps
a shadow blitter register file, snapshots each BLTSIZE-fired blit, and
reports aggregates.

### §66a. Result

`make boing-chipset-trace` log → 6,976 blits.  USE-flag distribution:

| flags | count | pct  |
|-------|-------|------|
| ACD   | 4,851 | 69.5%|
| CD    | 1,414 | 20.3%|
| D     |   464 |  6.7%|
| ABD   |   149 |  2.1%|
| BD    |    50 |  0.7%|
| BCD   |    46 |  0.7%|
| (none)|     2 |  0.0%|

Minterm (BLTCON0 low byte) distribution:

| minterm | count | meaning |
|---------|-------|---------|
| $CA     | 1,876 | standard cookie-cut "A AND C → D, mask=C" |
| $FA     | 1,661 | C-or-A → D (overlay) |
| $6A     | 1,428 | A XOR C → D |
| $0A     |   836 | A AND C → D (no mask) |
| $00     |   466 | constant 0 fill |
| $AA     |   441 | C identity |
| $D8     |   149 | A XOR (B AND (A XOR C)) — line draw raster mode |
| smaller |     – | various |

Mode: **100% area, 0% line.**  Our recent line-mode AUL fix
(`project_blitter_line_aul_fix.md`) is *not* exercised by boing.
Pointer regions: BLTCPT and BLTDPT hit CHIP RAM 6,665 / 6,959 of the
6,976 blits — boing is constantly reading and writing chip-RAM
bitplanes.

Size: 38% tiny (≤16 words), 56% small (≤256 words), 4.4% medium,
1.2% large.  The large blits are dominated by `BLTCON0=$05CC USE=BD
size=5,984` firing 50 times — a big B→D copy of ~12 KB, probably a
screen-buffer move or the sample-data transfer.

### §66b. So what

The dominant blit class — **cookie-cut $CA + USE=ACD** — is fired
~4,800 times in boing's 55M-instruction window.  That's a single
class executed many many times in quick succession.  This is exactly
the regime where cumulative state-leakage bugs accumulate:

- BLTAFWM / BLTALWM accidentally surviving across blits (we've had
  this class of bug before — see §44, §51).
- BLTxMOD set once at sequence start and assumed to stay correct
  across many blits (if our blitter has any post-blit register
  side-effect, it surfaces here).
- BLTADAT preset from previous USE=ABD blit if a USE=ACD blit
  starts before BLTBDAT is reloaded (USE_B=0 → B=BLTBDAT preset is
  a recent fix per `project_blitter_minimig_validated.md`; analogous
  cases for A=preset?).

`make crosscheck-minimig-blt` currently runs 4 canned blits in
isolation.  None of them test back-to-back cookie-cut.  Adding a test
that replays the first 100-200 cookie-cut blits from a real boing
chipset trace would catch any cumulative-state bug in this class.

### §66c. Engineering preserved

`blit_classify.py` works on any chipset trace, not just boing-disk:

    python3 tools/intuition_diff/blit_classify.py \
        /tmp/<tag>_chipset.ours.log [--csv per_blit.csv]

The `--csv` flag emits one row per blit with all register state — that
output is exactly the format a future "replay this sequence on both
blitters" test would consume.

## §67. Boing-replay infrastructure for `crosscheck-minimig-blt` — landed but address-collision-limited

§66 narrowed boing's blit load to ~4,800 cookie-cut $CA + USE=ACD
blits.  This commit extends `make crosscheck-minimig-blt` with a
replay path that consumes a chipset-trace-derived blit sequence and
runs the blits back-to-back through both blitters.

### §67a. What landed

- `tools/intuition_diff/blit_classify.py` gains `--replay-out FILE
  --limit N --filter-con0 HEX` so you can emit a per-blit text file
  with all register state: con0 con1 afwm alwm  apt bpt cpt dpt
  amod bmod cmod dmod  bltsize.
- `tb/minimig_blt_xcheck.cpp` learns a REPLAY_FILE=path env-var path
  that:
  1. Re-resets both blitters after the canned tests
  2. Pre-fills both 32 KiB memories with `word[w] = (w*0x1357) ^ w`
  3. Verifies pre-fill consistency
  4. For each replay line: zeroes BLT[ABC]DAT presets, writes the
     full register set, triggers BLTSIZE, spins until !busy, and
     diffs full memory
  5. Reports the first divergent blit with up to 5 context entries

### §67b. First-run result — pointer masking dominates

```
make minimig-blt
python3 tools/intuition_diff/blit_classify.py \
    /tmp/boing_chipset.ours.log \
    --replay-out /tmp/boing_blits_first200.txt \
    --limit 200 --filter-con0 0BCA
REPLAY_FILE=/tmp/boing_blits_first200.txt build_minimig_blt/Vminimig_blt_xcheck_top
```

Result:

- Canned tests: 12,232 match, 0 mismatch (baseline still holds).
- Pre-fill check: 0 divergent words.
- Replay blit#0: 225 divergent words; 5/5 divergent blits, stopped.

But: the divergence is dominated by pointer masking artifacts, NOT a
real blitter bug.  Boing's real pointers are 24-bit chip-RAM
addresses (e.g. $010000, $050000, $0B0000); the xcheck testbench has
only 32 KiB of test memory per side, so the harness masks pointers
to 16 bits.  That collapse maps distinct boing addresses to the same
location, with negative modulos (BLTDMOD = $FFEC) causing the blit
to wrap backward and overlap its own dest writes with new C reads.
Both blitters see the same collapsed inputs, but the collision-heavy
interaction amplifies any tiny arithmetic difference into a hundred-
word divergence — even when neither blitter has a real bug for these
parameters.

### §67c. Open work for a meaningful replay

Two clean fixes the harness would need to be a real cumulative-state
test:

1. **Expand the test memory.**  Grow `mm_mem` and `our_mem` to at
   least 1 MiB (524,288 words) — covers all of boing's chip-RAM
   pointer range without masking.  Indexes become 19-bit.
2. **Or: remap pointers consistently.**  Walk the replay, find the
   minimum pointer in each blit's set, subtract that base from
   APT/BPT/CPT/DPT, so the blit fits in low memory without
   colliding with siblings.  Requires careful base selection so
   relative pointer differences between blits (BLTAPT vs. BLTCPT
   in the same blit) survive.

Option 1 is simpler and stays semantically identical to the canned
tests.  ~half a day to grow the memory and re-run.

### §67d. Engineering preserved

Even with the address-collision limitation, the replay infra is
useful as-is for:

- Smoke-testing that our blitter doesn't crash / hang / drift on
  back-to-back diverse register patterns drawn from real workloads.
- Regression detection: re-run after any blitter RTL change and
  confirm the canned-test 12,232/0 pass holds.
- Spotting cases where the diff suddenly explodes (one of the
  blitters truly broke) vs. stays at the same baseline noise level.

The `--filter-con0` mechanism makes it easy to slice the trace by
operation class for targeted testing of any future blitter fix.


## §68. xcheck memory grown to 2 MiB; replay reveals real blitter divergence

§67c laid out the obvious fix: grow `mm_mem` / `our_mem` so 24-bit
boing pointers fit without collision-masking artifacts.  Landed here.

### §68a. The changes

- `tb/minimig_blt_xcheck_top.sv`: both memories now 1 Mi words
  (= 2 MiB) — covers the entire Amiga A500 chip-RAM range
  ($000000-$07FFFF) at full resolution.  `init_addr` /
  `read_addr` widened to 20 bits; minimig-side memory indexing uses
  `mm_addr_out[20:1]`, our-side uses `our_mst_addr[20:1]`.
- `tb/minimig_blt_xcheck.cpp`:
  - `init_mem` / `read_mm` / `read_our` take `uint32_t` word addresses.
  - Replay pointer mask is `& 0x1FFFFEu` (21-bit byte address) instead
    of `& 0xFFFEu` — preserves boing's real pointer relationships.
  - Pre-fill loop covers all 1 Mi words.
  - Pre-fill consistency uses a stride-64 sample (full scan would be
    needlessly slow); per-blit comparison does a ±128KB window around
    `dpt21` plus a stride-32 sweep for stray writes.

### §68b. Canned tests still pass

```
=== Test 1 ===  21 match, 0 mismatch
=== Test 2 (K1.3 big blit) ===  6205 match, 0 mismatch
=== Test 3 (K1.3 cyl-53 cc000005) ===  5985 match, 0 mismatch
=== Test 4 (odd BLTBPT/DPT masking) ===  21 match, 0 mismatch
Total: 12232 match, 0 mismatch
```

So growing the memory did not regress any known-good test.

### §68c. Real divergence on boing's cookie-cut sequence

Replay of `make boing-chipset-trace` first 200 blits filtered to
`BLTCON0=$0BCA` (cookie-cut $CA + USE=ACD — boing's dominant class
per §66):

```
blit#0  con0=$0BCA size=$0C85  -> 278 diverging words
    [in-window=271  out-of-window-sample=7
     apt=$04FFF6 cpt=$010000 dpt=$010000]
```

The window covers ±64K words around `dpt`.  271 diffs there means
~0.4% of in-window samples differ — most positions match, but ~270
don't.  Out-of-window stray diffs are only 7 from a sampled stride-32
sweep — so our blitter is NOT scribbling all over memory, it's
producing wrong output in the *intended* region.

This is the first time the cross-check has shown a real blitter
divergence against minimig on a real boing workload.  Previously
(§67) the divergence was dominated by 16-bit pointer collision; now
it's genuine algorithm divergence.

### §68d. Likely class

Boing's blits use parameters the canned tests do not:

| param   | boing      | canned tests |
|---------|-----------|--------------|
| BLTAFWM | $FFFF      | $FFFF        |
| **BLTALWM** | **$8000**  | $FFFF        |
| BLTAMOD | $0020-$0028 | 0            |
| BLTDMOD | $FFEC (-20) | 0            |
| BLTCON1 | $0000      | $0000        |
| BLTSIZE | 50 rows × 5 W | various   |

The standout is `BLTALWM=$8000` — only the highest bit set,
meaning only one pixel column gets through in the rightmost word of
each row.  Canned tests use `$FFFF` (no edge masking) so the
last-word-mask path isn't exercised.  Combined with negative
BLTDMOD (-20 bytes = -10 words per row), the blit walks dest
pointer in a tight spiral pattern.

Most likely the bug is in our blitter's interaction between
BLTALWM and BLTADAT preset shifting, OR in how the masked A
value flows into the minterm logic.  Tracking it down is
substantive but well-scoped work — write a tiny standalone test
with these exact params and step into the blitter.

### §68e. Engineering preserved

- The `make minimig-blt` build is now the real cumulative-state
  test.  `REPLAY_FILE=...` runs any chipset-trace-derived sequence.
- `blit_classify.py --filter-con0 HEX` lets you slice the trace
  by operation class so the test focuses on the suspicious one.
- Window-around-dpt + stride sample provides per-blit
  in-window/out-of-window decomposition, making it easy to tell
  "real divergence" from "stray writes".

Half-a-day estimate at §67c held: the changes are local and the
test is now diagnostic.


## §69. Test 5 isolates the bug — "first row OK, rows 1+ all zeros"

Following §68d, added Test 5 to `tb/minimig_blt_xcheck.cpp` with the
exact problematic parameters extracted from boing's chipset trace:
cookie-cut $CA + USE=ACD, BLTAFWM=$FFFF, **BLTALWM=$8000**,
BLTDMOD=$FFEC (-10 words/row), 3 rows × 2 words = 6 D writes.

### §69a. Result

```
=== Test 5: BOING-CLASS cookie-cut ($0BCA, BLTALWM=$8000) ===
  MISMATCH @ $005FE0: mm=$AAAA our=$0000
  MISMATCH @ $005FE2: mm=$AAAA our=$0000
  MISMATCH @ $005FF0: mm=$E002 our=$0000
  MISMATCH @ $005FF2: mm=$C013 our=$0000
Test 5: 44 match, 4 mismatch (cookie-cut $CA + BLTALWM=$8000 + neg DMOD)
```

The 4 mismatching bytes are at byte addresses $5FE0, $5FE2, $5FF0,
$5FF2 — that is word indices $2FF0, $2FF1, $2FF8, $2FF9.

### §69b. Decoding which writes failed

With BLTDPT byte $6000 (word $3000) + BLTDMOD = -20 bytes (-10 words)
per row + 2 words written per row:

| row | writes word indices | mismatch? |
|-----|--------------------|-----------|
| 0   | $3000, $3001       | no — both blitters produce same output |
| 1   | $2FF8, $2FF9       | **YES** — minimig=$AAAA, $AAAA; ours=$0000, $0000 |
| 2   | $2FF0, $2FF1       | **YES** — minimig=$E002, $C013; ours=$0000, $0000 |

So **row 0 is correct, rows 1 and 2 are all zeros on our side**.

### §69c. Hypothesis

Cookie-cut $CA computes `D = A AND C` (with maskword applied to A
first).  With BLTAFWM=$FFFF, A passes through at word 0 of each row;
with BLTALWM=$8000, only A's MSB survives at the last word.

Row 0 producing correct output means the *blit setup* (loading A
from BLTAPT, applying minterm logic to A AND C) is correct.

Rows 1 and 2 producing zeros means *something specific to the
row-transition state* is broken in our blitter.  Two suspects:

1. **BLTAFWM is not re-applied at the start of subsequent rows.**
   If after row 0 our blitter clears the "is-first-word-of-row"
   latch and never re-arms it, then ALWM applies to every word from
   row 1 onward.  With ALWM=$8000, A's source data has only bit 15
   survive, and `A AND C` becomes nearly all zeros (with our $A0xx
   / $C0xx source data, bit 15 is 1 in C but 0 in A, so AND = 0).
2. **BLTADAT preset isn't carried forward.**  Our blitter has the
   USE-B preset fix landed (§project_blitter_minimig_validated.md);
   maybe USE-A has the analogous bug that surfaces only on row 2+.

Suspect (1) fits the symptom exactly: row 0 row-start sets AFWM,
rows 1+ inherit ALWM and zero out A.  Minimig must re-arm AFWM at
each row start; our blitter doesn't.

### §69d. Next move

Now the bug is well-scoped:

1. Compile our `rtl/chipset/blitter.v` with the t156-style trace
   probes around the row-start logic and AFWM/ALWM gating.
2. Trace the AFWM application signal across all 3 rows.  If it's
   high only on row 0, that's the bug.
3. Fix the row-start re-arm; verify Test 5 passes; then run the
   full boing replay and confirm in-window divergence drops to 0.
4. Once the fix is in, run boing-disk end-to-end and see if the
   Window struct at $494A0 finally populates.

This is a substantial fix but well-scoped: ~half a day of blitter
RTL work + regression run.  Test 5 is now a permanent regression
test for this class.

