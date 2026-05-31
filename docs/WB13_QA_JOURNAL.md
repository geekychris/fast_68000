# WB1.3 / K1.3 boot investigation — Q&A log

Companion to `WB13_DEBUG_JOURNAL.md` (the chronological work log).  This
file captures the *questions* Chris asked along the way and the
answers that came out of investigation — useful as a fast index when
the same kind of question comes up again in a future session.

Each entry follows: **Q** (verbatim or close-to-verbatim) → **A**
(the answer after investigation) → **trail** (where the evidence
lives now, so the answer can be re-verified).

---

## Q1 — "Is this related to our 'fast 68k'? If we use another implementation of the 68k will that help?"

**A — No.** Our 68k Verilog core is byte-identical to Musashi (the
reference 68k interpreter) across a 100K-instruction window covering
the corruption point.  Switching CPU implementations will produce the
same wall.

**Trail.**
- `make cosim-window SNAP_PC=00FEA932 SNAP_AFTER=4437000 WINDOW=100000`
- Result: 2 diff hunks in 100,001 lines, both MOVEM-load trace-format
  artifacts (Verilator emits post-state, Musashi pre-state).  Both
  re-converge on the next single-cycle instruction.  CPUs functionally
  identical.
- See `WB13_DEBUG_JOURNAL.md` §11 and memory item
  `project_wb13_cpu_exonerated.md`.

---

## Q2 — "Why so many trackdisk issues?  Is this because all of the booting of the OS is from trackdisk or something more fundamental?"

**A — Both, and the cosim sharpened the diagnosis.**  Trackdisk is on
the critical path for everything after the bootblock (Workbench reads
all libraries / fonts / s/startup-sequence via trackdisk), so *any*
trackdisk bug surfaces as a boot wall.  But our trackdisk
implementation is also wide:
- MFM encoding (`tools/adf2mfm.py`)
- Floppy DMA + DSKLEN arming (`rtl/m68k_bus.v`)
- DSKSYNC + DSKBLK irq (`rtl/m68k_bus.v` / `rtl/chipset/paula.v`)
- CIA-B step/dir/side line decoding (`rtl/m68k_top.v`)
- ADKCON WORDSYNC handling

Each had a bug at some point.  And K1.3's trackdisk is unforgiving:
it polls, retries, validates, has cache state.  Single byte mismatch
in MFM, single missed step pulse, off-by-one DMA length — and the
validation chain rejects.

Now that the cosim has eliminated the CPU and the FS-UAE diff has
identified the upstream cause (K1.3 IRQ-dispatcher install skipped,
see Q14), the trackdisk-side issues are probably symptoms of K1.3
taking the wrong control-flow path before trackdisk ever finishes
its init.  Fix the install gate and the trackdisk symptoms likely
recede on their own.

**Trail.**  `WB13_DEBUG_JOURNAL.md` §13, memory item
`project_wb13_intvecs_skipped.md`, task #98.

---

## Q3 — "go ahead do the cosim"

**A — Done.**  100K-instruction Musashi cosim across the bad-blit
window proved the CPU is byte-identical to Musashi.  This eliminated
two sessions' worth of "MOVEM CPU bug" hypotheses.

**Trail.**  Q1, plus the actual cosim output at
`build_cosim_window/veri_window.log` and `musashi_window.log`.

---

## Q4 — "well we don't have a real amiga so its minimig comparison time"

**A — Limited.**  Our minimig integration is sub-module cross-checks
only (paula_intcontroller, blitter) for byte-matching against the
upstream FPGA Amiga implementation.  Bringing up the *full* minimig
system in Verilator is feasible but a 1-3 day project (top-level
wrapper, MiST-board peripheral stubs, ROM seed, etc.).

What we did instead: use FS-UAE save states as the reference
(software emulator that's been hardened over decades against real
hardware).  See Q7.

**Trail.**  `COSIM.md` "The existing cosims" + Q7 below.

---

## Q5 — "yes run screen_walk"

**A — Done.**  `tools/screen_walk.py` against the chip-RAM snapshot
walked the Workbench Screen struct successfully.  Confirmed the
BitMap is at $C01410 with Planes[0]=$60C8, Planes[1]=$B0C8 — 2-plane,
640×256 PAL Workbench.  The corrupting blit's destination at $5E40 is
in the BCPL DOS data area just *below* the bitplane.  Initially
suggested a memory-allocation overlap; later disproved by Q7's FS-UAE
diff (FS-UAE has the same memory layout but doesn't corrupt $5E40).

**Trail.**  `WB13_DEBUG_JOURNAL.md` §9 (was wrong) + §13 (correct).

---

## Q6 — "This seems a very niche issue.  Especially when real amigas also can have much faster 68030 and up microprocessors.  Go ahead and investigate."

**A — Niche framing was wrong** (turned out via Q7's FS-UAE diff that
the bug is in our K1.3 init path, not in memory layout).  But the
investigation you authorized produced the actual root cause: the K1.3
IRQ-server dispatcher install is skipped on us.

**Trail.**  Q7 + Q14.

---

## Q7 — "well we have fsuae installed.  Here is an example but Please copy the configs I dont want you to trash the one I have fs-uae ~/Documents/FS-UAE/Configurations/AmiKit-Debug.fs-uae"

**A — Created `~/Documents/FS-UAE/Configurations/WB13-K13-A500-Reference.fs-uae`** that matches our emulator's setup (A500 + 68000 + 512 KB
chip + 512 KB slow + our K1.3 ROM + WB1.3 ADF).  Staged the ROM and
ADF into FS-UAE's standard directories.  Left your AmiKit-Debug
config untouched.

**Trail.**  `~/Documents/FS-UAE/Configurations/` (new file alongside
the existing one).

---

## Q8 — "I saved state.  Not sure where it stored it"

**A — `~/Documents/FS-UAE/Save States/WB13-K13-A500-Reference/Saved
State 1.uss`** (FS-UAE default location, named after the config).

**Trail.**  `find ~/Documents/FS-UAE -name "*.uss"` returned that
single match.

---

## Q9 — "would looking at the source to fsuae also help?  it is here https://github.com/FrodeSolheim/fs-uae"

**A — Yes substantially.**  Sparse-cloned to `/tmp/fsuae-src/` (no
history blobs).  The `blitter.cpp` file at the repo root is the
reference 68k blitter implementation.  Key finding from reading it:
the **D-channel is pipelined one word behind the C read** (lines
546-563 of `blitter_dofast()`).  Our blitter wrote D in the same
cycle as the C read; landed the pipeline this session.

The blitter source didn't directly fix the $5E40 corruption (Q11 's
investigation showed source and dest don't overlap in a way the
pipeline timing affects), but it's correct Amiga behavior we should
have had.  More importantly the FS-UAE save state format (parsed by
`tools/fsuae_state.py`) opened up the comparison work in Q12-Q14.

**Trail.**  `/tmp/fsuae-src/blitter.cpp` lines 481-580.  `BLITTER.md`
"D-write pipeline" section.

---

## Q10 — "yes" (re: attempt the blitter D-pipeline fix)

**A — Done, but didn't fix the corruption.**  D-pipeline landed in
`rtl/chipset/blitter.v`.  141/141 unit tests pass.  WB1.3 boot
unchanged (still corrupts $5E40 — proved analytically that the
specific blit pattern has non-overlapping source/dest after the
first row, so pipeline timing doesn't affect which addresses get
written).  The pipeline is correct Amiga behavior regardless.

**Trail.**  `BLITTER.md`, task #96 (completed).

---

## Q11 — "do a and b" (re: D-pipeline + FS-UAE divergence-bisection)

**A — Both done.**  D-pipeline as above.  FS-UAE → Verilator snapshot
converter (`tools/fsuae_state.py`) parses the .uss format
(CRAM/BRAM/CPU/CHIP chunks; FS-UAE 3.x has a +4 offset before D0 in
CPU chunk vs the textbook layout — discovered empirically by searching
for known register $C0040C).  Side-by-side reg comparison at the
idle STOP showed A0/A3/A4/A6/A7 match exactly, A1/A2/A5/D2-D7 diverge
= K1.3 takes different control-flow paths.

**Trail.**  `tools/fsuae_state.py`, memory items
`project_wb13_fsuae_reference.md` and `project_wb13_fsuae_state_tool.md`,
tasks #96 and #97.

---

## Q12 — "continue" (after Q11)

**A — Continued into chipset register diff.**  Extended
`fsuae_state.py` to extract DMACON / INTENA / INTREQ from the CHIP
chunk.  Discovered:
- DMACON: ours $02D0 vs FS-UAE $03F0 → BPLEN(8) + SPREN(5) bits
  differ.  Our K1.3 *never* enables bitplane DMA.
- Zero CPU writes to $DFF096 with BPLEN set in entire trace.

**Trail.**  `tools/fsuae_state.py` updated, task #98 created.

---

## Q13 — "continue checking cosim background task bdnnr4616 and analyze who zeroed mem[$5E40]"

**A — The cosim task was hung on diff'ing a 2M-line file; killed it
and used the simpler approach of adding a bus-arbiter `[5E40-BUS-WR]`
probe.**  Caught the actual corrupting blit:
`bltcon=$0A000003 bltdpt=$6438 bltsize=$2EE7` walking through $5E40
on rows 128-133.  This was the line-blit identified in the original
`project_wb13_linef_storm.md` analysis but with the bltcon decode
corrected (LF=$00 not $0A; copy mode not line mode).

**Trail.**  `[5E40-BUS-WR]` probe in `rtl/m68k_bus.v`,
`WB13_DEBUG_JOURNAL.md` §13.

---

## Q14 — "Did you update all the docs that you are keeping a journal of your progress, your new debugging techniques, issues found and resolved."

**A — Yes, updated five docs (+422 lines total).**
- `docs/WB13_DEBUG_JOURNAL.md` §11-§15 (+201)
- `kickstart/JOURNAL.md` S45 (+101)
- `DIAGNOSTICS.md` new `tools/fsuae_state.py` section + probe
  entries (+51)
- `BLITTER.md` D-pipeline + BLT_VECTABLE_GUARD sections (+49)
- `COSIM.md` cosim-window + FS-UAE state diff entries (+21)

All cross-referenced.  Memory items synchronized.  Future investigators
can land on any doc and follow the trail.

**Trail.**  Commit `28ee464 more tools` (the docs) and `7d1930d more
tools` (the code).

---

## Q15 — "go ahead" (re: K1.3 ROM walk for the IRQ-dispatcher install)

**A — Found `LEA $00C0.W, A0` exactly once in K1.3 ROM, at $FC022C.**
The surrounding code at $FC0220-$FC0236 is the exec chip-RAM
zero-clear (matches our 16 zero-writes to $C0-$FC at boot start).
The actual install routine that POPULATES $C0-$FC with dispatcher
pointers isn't a `MOVE.L _, $00C0.W`-style sequence — K1.3 builds the
dispatcher dynamically.  No `MOVE.W #$6120, Dn` (the obvious build-loop
constant) and no static handler signature in ROM either.

The install is somewhere later in boot, probably in a resource init
(cia.resource? graphics.library?).  Next attempt: bisect by saving
FS-UAE state at progressively later moments and watching for the
chip-$C0-$FC transition.

**Trail.**  task #98 updated with disassembly findings,
`project_wb13_intvecs_skipped.md`.

---

## Q16 — "btw how do you control fs-uae so that you can save state every 1m cycles?"

**A — There's no built-in "save every N cycles" API**, but several
practical options:

1. **F12 menu (manual)** — what you used; no cycle precision.
2. **`console_debugger = 1` in config** — Pause + `` ` `` drops into a
   UAE debugger console with `f <pc>` (breakpoint), `g` (continue),
   `M <addr> <count>` (memory dump), `S` (save state).
3. **Your `serial_port = tcp://0.0.0.0:2345`** — if your MCP setup can
   send debugger commands over that TCP port, scriptable.
4. **FS-UAE Lua scripting** — `script` config option, sparse docs.
5. **AppleScript / macOS UI automation** — press F12 at intervals,
   fragile.

**Most practical for our chip-$C0-$FC bisection:** PC breakpoints at
each K1.3 RESINIT entry, save state per fire.  The install probably
happens during a specific resource's init; bracketing each RESINIT
and walking forward will pinpoint the one.

**Trail.**  This file's parent answer + ongoing task #98.

---

## Q17 — "I have asked a lot of questions on the way could you make sure you have a journal of Chris's questions and your answers to them"

**A — This file.**  Will be appended to in future sessions whenever a
substantive question comes in.

**Convention for future entries:**
- Q numbered sequentially
- Verbatim or near-verbatim user quote
- Answer in 1-3 paragraphs
- "Trail" link to where the evidence lives (memory item, commit,
  journal section, code file).
- Edit prior entries only to add a "**Update (date):** ..." note if a
  later investigation invalidates the answer.

---

## Themes that came up multiple times

- **"Is this our CPU?"** (Q1, Q3) → No.  Cosim definitive.
- **"Is this niche / will another CPU help?"** (Q1, Q6) → No.  K1.3
  takes a wrong code path; CPU choice doesn't change that.
- **"How do we get ground truth?"** (Q4, Q7, Q9, Q16) → Software
  emulator save states (FS-UAE) + `tools/fsuae_state.py` parser.
- **"Did you update the docs?"** (Q14, Q17) → Always update
  `WB13_DEBUG_JOURNAL.md`, `kickstart/JOURNAL.md`, the relevant
  topical doc (BLITTER/COSIM/DIAGNOSTICS), and the memory items.
  Now also this Q&A log.
