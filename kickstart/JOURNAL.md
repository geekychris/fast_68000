# Kickstart 1.3 boot bring-up journal

A running log of the reverse-engineering / bring-up work to get the
real Kickstart 1.3 ROM booting on our Verilator-based 68000 sim.
Newest entries on top.  Each entry includes: what we found, how we
found it, and what to try next.  Companion files:

- `kickstart/README.md` — stable summary of fixes + open items.
- `tools/disasm68k.py` — m68k disassembler we use to read ROM.
- `tools/rom_decrypt.py` — strip AMIROMTYPE1 header / XOR with key.
- `tools/mkbootblock.py` — build a minimal Amiga DOS bootblock.
- `tools/adf2mfm.py` — MFM-encode an ADF for our floppy DMA stub.

The boot ROM (`kickstart/kick.bin`, gitignored) is loaded as the
$F80000 ROM image and the boot trampoline at `tests/t63_boot_rom.s`
manually loads SSP / PC from the reset vectors at low chip RAM (which
are ROM-shadowed at OVL=1).  Make target: `make test-kickstart-boot
ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072` (or
`make test-boot-rom-bin ...` for the older no-disk variant).

---

## Current state (TL;DR)

- **93/93** regression tests pass (+`t109_illegal_trap.s`,
  +`t110_cmp_b_d16an.s`).
- Kickstart 1.3 cold boot now correctly init's `exec.library`,
  scans the ROM, builds the full 9-entry ResModules list, and
  starts walking COLDSTART residents -- `keyboard.device`,
  `diag init`, and (post-fix) `strap` all fire.  See S7.
- After `strap` fires Kickstart heads off into wild PC space
  (`[BAD-PC]` triggers with PC in $80xxxxxx / $23xxxxxx).
  Likely strap's init touches kicktag / ColdCapture vectors we
  haven't wired, or the bootstrap task crashes due to a still-
  missing CPU detail.  Next session's target.

CPU fixes landed during the bring-up (all keep regression
tests green; each has a dedicated regression test):

| Bug | Commit | Test |
|---|---|---|
| `MOVE-FROM-SR` to memory was a silent no-op | 6020a09 | `t104_movefromsr_mem.s` |
| SR-write S=1→0 didn't swap A7 with usp_shadow | cc6e450 | `t105_sr_user_swap.s` |
| `SUBQ.W d16(An)` worked, but exposed how `d16(An)` byte writes interact | — | `t106_subqw_d16an.s` |
| `JMP $d8(PC, Xn)` used A7 as the index register | 4d43e26 | `t107_jmp_pc_idx.s` |
| `Scc <ea>` to memory was a silent no-op | e1591ab | `t108_st_tst_d16an.s` |
| K_BAD (unknown opcode) was a silent no-op; decoder ordered TAS before ILLEGAL so $4AFC mis-decoded | 96ce886 | `t109_illegal_trap.s` |
| K_ALU mem source took low byte of dc_rdata instead of byte at dc_addr[1:0] | a1ebc56 | `t110_cmp_b_d16an.s` |

Other infrastructure added:

- `tools/disasm68k.py` -- m68k disassembler for ROM exploration.
  Handles all the opcode families we've encountered so far; new
  ones are added when they show up.  Unknown opcodes fall back to
  `DC.W` so we always make forward progress.
- `tools/mkbootblock.py` -- builds a 1024-byte Amiga DOS bootblock
  with `DOS\0` magic + valid additive checksum + 14 bytes of code
  that writes `$CAFEBABE` to `$00050000` and RTSes.
- `make test-kickstart-boot ROMFILE=kickstart/kick.bin` -- end-to-end
  target: build bootblock, MFM-encode, load into `disk[]`, run
  Kickstart, grep for `[BOOTBLOCK]` in the sim output.
- `rtl/m68k_top.v` -- drives CIA-A `pa_in` bit 2 (`/DSKCHANGE`) low
  so trackdisk would treat the drive as freshly-loaded.

## How to navigate this file

Each session is an `### Sn:` block, newest at the bottom.  Each
opens with a one-line **Goal**, runs through discoveries / hypotheses
/ experiments as bullets, and closes with a `Status:` line.

## Session log

### S2: `JMP $disp(PC, Xn)` was using A7 as the index

Goal: keep pulling on the render-loop thread; find which exec.library
field is feeding `A0..A4` the bad low-chip-RAM pointers.

- Built `tools/disasm68k.py` (handles MOVE, MOVEA, BSR/BRA/Bcc,
  DBcc, Scc, ADDQ/SUBQ, MOVEQ, LEA, PEA, JMP/JSR, RTS/RTE/RTR,
  LINK/UNLK, MOVE USP, MOVE SR/CCR, MOVEM, ORI/ANDI/SUBI/ADDI/EORI/
  CMPI, register-ALU forms, shift family, TST, CLR, SWAP, EXT).
  Unknown opcodes fall back to `DC.W $xxxx` so we always make
  forward progress reading ROM.

- Disassembled `$F810F6..$F811B6` and recognised the surrounding
  routine as a **bytecode dispatcher**: load a byte at `(A1)+`,
  shift+mask into an index, look up a 16-bit signed offset from
  a small table at `$F81196`, then `JMP $2C(PC, D1.W)` to one of
  ~8 case handlers inside the same routine.  All table entries
  are small negative offsets that land back inside the dispatcher
  loop itself.

- Re-checked our earlier trace: `JMP $F81168 -> $F86EEE`.  Computed
  the math: for that target to be reached, D1 at the JMP must
  equal `$5D58`, far beyond what the byte-dispatch could produce
  ($0..$E from the masked shift).

- Added a focused trace: `[JMP1168] ex_rb=00000000 target=00f86eee`.
  ex_rb (= D1 for K_JMP's index register) reads as 0 — so our
  computed target should be $F8116A + $2C + 0 = $F81196.  But the
  CPU emits $F86EEE instead.  Difference: $F86EEE - $F81196 = $5D58.
  At the time of this JMP, A7 was around `$5D58` (we are deep in
  supervisor calls).  Smoking gun: our `d8(PC, Xn)` reads A7
  instead of the brief-extension-specified Xn.

- Root cause in `rtl/m68k_core.v`: `id_rc_idx` is muxed to the Xn
  register only when `idec_src_mode == EA_IDX` (mode 6).  For
  `EA_EXT / EA7_PCIDX` (mode 7 / reg 3 = `d8(PC, Xn)`) the wire
  defaults to A7.  The EA computation at line 740 then reads
  `ex_sp` (= rc port) as Xn — which is A7's value.

- Fix: extend `id_src_is_idx`/`id_dst_is_idx` to also fire when
  `(src_mode == EA_EXT && src_reg == EA7_PCIDX) && ext_words != 0`.
  Same for dst.  Now Xn for `d8(PC, Xn)` is read correctly.

- Regression: 89/89 pass.  Boot: the previous infinite render loop
  is GONE.  Boot now reaches a `STOP #0` at chip-RAM `$040C`
  (= the inside of the test trampoline's `stop #0`) at
  retired = 2,412,037 with `sp=$5D6A`.  No `[BOOTBLOCK]`, no
  `[DSKLEN]` events yet -- the boot apparently RTS'd to a low-RAM
  address that happens to land in the trampoline's STOP.

Status: render-loop block resolved; new blocker is a `JSR -6(A6)`
at `$F808B4` firing with `A6 = 0` after a failed `MakeLibrary`
call.

### S3: `JSR -6(A6)` with A6 = 0 in the MakeLibrary failure path

Goal: figure out why the boot now hits a null-pointer JSR shortly
after the bytecode dispatcher started executing.

- Used the new disassembler to read the routine at
  `$F80866..$F808BA`.  Recognised it as a small library-create
  wrapper:
    ```
    $F8086A: ST   $001F(A2)         ; flag = $FF  (busy/fail-default)
    $F80884: LEA  $015E(A6), A0     ; param for MakeLibrary
    $F8088E: JSR  $FEEC(A6)         ; MakeLibrary  (LVO -276)
    $F80894: MOVEA D0, A0           ; A0 = result
    $F80896: MOVE A0, $14(A2)       ; store result in struct
    $F8089A: BEQ  $F808A0           ; jump to common tail if A0=0
    $F8089C: CLR.B $001F(A2)        ; success: clear the flag
    $F808A0: MOVEM (A7)+, D0,D1     ; restore D0/D1
    $F808A4: TST.B $001F(A2)        ; check flag
    $F808A8: BNE  $F808BA           ; failure: skip the Open() call
    $F808B2: MOVEA A0, A6           ; A6 = library base
    $F808B4: JSR  $FFFA(A6)         ; library's Open() (LVO -6)
    ```
- Traced register state through the routine.  MakeLibrary returns
  `D0 = 0` (failure).  The `BEQ` at `$F8089A` is correctly taken
  → control reaches `$F808A0`.  The `MOVEM` restores D0/D1 from
  the earlier `MOVEM.L A1,A0,-(A7)`.  Then `TST.B $1F(A2)`.

- Expected behaviour: the failure path keeps the flag set to
  `$FF`, so `BNE` takes us to `$F808BA`, skipping the bad JSR.
  In our boot the `BNE` doesn't take, falls through to `$F808B4`,
  and JSR `$FFFA(A6)` with A6=0 jumps off into the vector table
  (PC walks `$FFFFFFFA → 0 → $40A` and hits the trampoline STOP).

- Smoking gun: `ST $1F(A2)` was supposed to seed the flag with
  `$FF`, but at the later TST the same field reads as `0`.  Either
  (a) `A2` changed between the ST and TST (= we're hitting a
  different memory location), (b) something else wrote 0 to the
  flag, or (c) our `ST` / `CLR` / `TST` impl is mis-handling the
  `d16(An)` byte form here.

- Next: trace A2's value at both `$F8086A` and `$F808A4` (they
  should match), and trace the byte-write/read of `$1F(A2)` at
  the bus level so we can confirm whether the issue is the value
  Kickstart writes / reads or our CPU's handling of it.

Status: paused at this point; ROM-side disassembly + targeted
traces will continue in the next session.  90/90 regression tests
remain green.

### S4: `Scc <ea>` to memory was a silent no-op

Goal: figure out why `$1F(A2)` reads zero after `ST $1F(A2)`.

- Wrote `tests/t108_st_tst_d16an.s` -- a focused regression that
  does `ST $1F(A2)`, reads the byte back, expects `$FF`, and also
  verifies neighbouring bytes are untouched + the corresponding
  `CLR.B` / `TST.B` pair behaves.

- First run: `$BAD1` (= byte after `ST` was not `$FF`).  Real CPU
  bug confirmed.  K_SCC in `rtl/m68k_core.v` line 1747 only
  handled `EA_DREG` destinations -- memory destinations dropped
  on the floor, same pattern as the earlier MOVE-FROM-SR fix.

- Fix: extend K_SCC to take the `src_needs_mem` path when src is
  memory (`src` in the decoder == the EA, since Scc's encoding
  puts the EA in `m3/r0`).  Drives `want_mem/want_we/want_addr`
  with a byte-into-word slot + `be_for_byte` byte enable.  Added
  a matching K_SCC case in `S_STORE` for the predec/postinc An
  update.

- Regression: 91/91 pass (t106+t107+t108 all green).

- Kickstart boot: **massive jump**.  Now retires
  **42,819,851** instructions in 50M cycles (IPC = 0.856 vs. the
  previous 0.171), **192,819,851** in 200M cycles (IPC = 0.964 --
  near peak throughput).  No BAD-PC, no priv-vio crashes, the
  CPU is flying through code at the same rate as our regression
  tests.  Sampling PCs shows the boot eventually parks at
  `$F807F4`: `BRA $F807F4` -- Kickstart's **intentional "yellow
  Guru" hang** (sets `COLOR00 = $0F0F` just before, see
  `$F807EC`).  The Alert-hang is reached after a Kickstart-side
  failure check; figuring out which alert (= what failed) needs
  the next session.

Status: boot now runs ~190M instructions cleanly and reaches
Kickstart's intentional Alert hang at `$F807F4`.  All 91
regression tests pass.  Next: identify what fails before the
Alert -- likely a missing exec / graphics check, or a memory
init step that doesn't satisfy Kickstart's expectations.

### S5: Alert at $F807F4 -- it's the "no boot" tail, not a crash

Goal: figure out what causes the yellow-Guru Alert hang.

- Disassembled `$F80700..$F807F4` with `tools/disasm68k.py`.  The
  routine is exec's task-bringup tail:
  - `AllocEntry` to allocate the boot-task memory list.
  - On AllocEntry failure: `Alert($80018001 = AT_DeadEnd |
    AG_NoMemory | AO_ExecLib)`.  In our run AllocEntry succeeds
    (BPL skips the alert), so this isn't where we hit.
  - Build the boot task: `MOVE A0, USP` at $F80796 (this is the
    same instruction that earlier tripped the priv-vio chain in
    the old, broken render loop), task field setup, NewList for
    the task's MemList, AddHead to attach it, RemTask /
    state=`TS_READY`, RemHead, Permit, check CoolCapture, then
    `InitCode(1, 0)` -- run all RTF_COLDSTART residents.
  - After InitCode returns, set `COLOR00 = $0F0F` (yellow Guru
    background) and `BRA $F807F4` -- the intentional "I'm done,
    no boot found" hang.

- LVO map confirmed via the `JSR $FFxx(A6)` displacements:
  `$FFB8 = -72 = InitCode`, `$FF94 = -108 = Alert`, `$FF76 = -138
  = Permit`, `$FEE6 = -282 = RemTask`, `$FF22 = -222 = AllocEntry`.
  Earlier guesses elsewhere were wrong; LVO -72 is **InitCode**
  (not WaitTOF/Alert).

- The boot reaches the Alert tail because InitCode returns
  without anyone JSR'ing into a bootblock.  No `[DSKLEN]` writes
  fire across 100M cycles.  So whichever resident in the
  RTF_COLDSTART chain is supposed to drive trackdisk either
  isn't reached, or its init returns early without scheduling
  a disk read.

- INTENA across the whole run: master + SOFTINT only (`$4004`).
  VERTB IRQ is **never** enabled by Kickstart in this path
  (there's even one explicit `INTREQ CLR $7FFF` at boot that
  clears a queued VERTB).  Hypothesis: the boot expects a
  task-switch via VERTB to hand off to trackdisk, but the IRQ
  isn't enabled until trackdisk.device itself initializes -- and
  trackdisk's init isn't running.

- Tested by gating an `+define+KICKSTART_FORCE_VERTB` hack into
  the Paula INTENA write path (forces bit 5 set whenever INTENA
  master is touched).  Result: with VERTB IRQs firing,
  the vector dispatch immediately jumps to chip RAM addresses
  ($1B, $23, ...), because the vector table at vector 25 / 27
  (autovector for level 3 = VERTB) hasn't been populated by
  Kickstart yet at the time we force-fire it.  Confirmed VERTB
  isn't the right knob here -- the boot really does need
  trackdisk's RT_INIT to run.

- Reverted the FORCE_VERTB hack; only the journal notes remain.

Status: 91/91 regression tests still pass.  Next concrete step:
trace what InitCode iterates -- which residents it visits in
order, and whether trackdisk's resident is in the list.  If it
is, why does its init return without writing DSKLEN?  If it
isn't, our InitResident scan missed it (probably a cache /
unaligned-read issue in the scan).  The earlier README confirmed
trackdisk WAS found by InitResident before; the question is
whether that's still true in the post-Scc-fix execution path.

### S6: InitCode visits only 2 residents -- ResModules under-populated

Goal: figure out why InitCode's RTF_COLDSTART chain returns without
running trackdisk's init.

- Traced the JSR at `$F807E8` (the `JSR $FFB8(A6)` = InitCode call).
  The JT slot in low RAM is at `$0000_4ADC`; its JMP target is the
  real InitCode body at `$00F8_102A`.

- Disassembled InitCode (`tools/disasm68k.py 0xF8102A --bytes 64`):
  it loads `A2 = $12C(A6)` (= `ExecBase->ResModules`, a list of
  resident-tag pointers), walks `(A2)+` reading entries.  Zero
  entry = list terminator (BEQ to RTS).  Positive entry =
  resident-tag pointer; check `RT_VERSION` byte at offset $B
  against D3, check `RT_FLAGS` byte at offset $A AND'd with D2
  (startClass).  If both check, JSR `$FF9A(A6)` = InitResident.
  Negative entry = redirect (`BCLR #$1F`, then `MOVEA.L D0, A2`
  to follow a sub-list).

- Traced `[InitRes] resident=...` at the per-entry InitResident
  call (`$F81056`):
    - resident=`$000043AC`  (a chip-RAM struct)
    - resident=`$00004B24`  (ExecBase itself)
    - resident=`$00004B24`  again (one of the sub-list passes hits
                                  the same pointer)
  **Only 2 unique residents** get cold-start init.  The 8 standard
  Kickstart 1.3 ROM residents that the older README listed
  (exec / expansion / ramlib / audio / console / trackdisk /
  gameport / keyboard at $F800B2 / $F841DE / $F84CBE / $FA8C72 /
  $FB9EFE / $FBC6E6 / $FC2486 / $FC9A96) are **NOT** in the list.

- Traced any JSR/BSR landing in the `$F86E80..$F86F00` range (the
  resident-scan loop body that does `CMP.W #$4AFC, (A4)+`).
  **Zero hits.**  The scan never runs in our boot.

- Searched the whole ROM for `JSR $F86E88` or `BSR $F86E88`
  (the scan-loop's function entry): **no static callers**.
  Either the entry is reached via JSR (An) where An was loaded
  somewhere we didn't catch, or it's reached via the bytecode
  dispatcher's opcode handler that we missed, or it's not
  actually a function entry (just data that the old broken
  PC-IDX path *interpreted* as code).

- Insight: the OLD broken-PC-IDX execution accidentally drove
  the boot through `$F86EEE` (mid-body of the scan loop).
  Memory pointers `A0..A4` ended up walking low chip RAM,
  appending the chip-RAM "residents" we now see ($43AC and
  $4B24).  In the previously-broken path the 8 ROM residents
  the old README mentioned probably came from a DIFFERENT
  execution that ran *more* of the scan (or via the
  Switch-driven path that's now bypassed too).

- Bytecode dispatcher at `$F81138..$F81194` reads its byte
  stream from `A1 = $FBCA88` (a ROM data area).  Dump:
    ```
    A0 08 03 00  80 0A 00 FB C7 64  A0 0E 06 00  90 14 00 28
    90 16 00 01  80 2C 00 FB CA DA  80 30 00 FB CB 12
    ```
  Each "opcode" high nibble selects an action; low nibble +
  next bytes are operands.  $A0 = byte-copy N+1 bytes; $80 =
  word-copy; $90 = some other variant.  The bytes that look
  like 24-bit ROM addresses ($FBC764, $FBCADA, $FBCB12) suggest
  the stream DOES embed pointers -- likely populating exec.library
  jump-tables and small struct fields, NOT the ResModules
  resident list.

- Conclusion: this bytecode stream initialises exec.library
  itself.  Building the *full* ResModules list -- the cold-start
  resident chain -- requires a separate scan step that real
  Kickstart 1.3 does via something other than the bytecode VM,
  and that step isn't being driven in our boot post the
  PC-IDX/Scc fixes.

- The CPU is correct; the missing piece is whatever Kickstart
  function-call chain was supposed to populate ResModules with
  ROM-resident-tag pointers (exec/expansion/.../keyboard).  Best
  next probe: identify the function in the ROM that does
  `FindResident` in a loop and `Enqueue` results into ResModules,
  and figure out who's supposed to call it in our post-fix path.

Status: 91/91 regression tests pass.  Boot still parks on the
Alert tail.  ResModules under-population is documented as the
next concrete blocker.

### S7: ResModules under-population was a CPU bug, not a missing call

Goal: figure out who populates ResModules with the ROM residents
(S6 left this as "no static caller of the scan loop").

- **First fix (K_BAD):** the decoder collapsed every opcode it
  didn't recognise into `K_BAD`, which was a silent no-op.  Real
  68000 raises ILLEGAL (vec 4) for unknowns, with A-line / F-line
  routed to vectors 10 / 11.  Kickstart 1.3 uses $4E7A (MOVEC),
  $F2xx (F-line), $4AFC (hard illegal) as CPU-feature probes.
  Without a trap they all succeed silently and Kickstart picks
  the 68030+ path through ResModules.  Added `K_BAD ->
  ILLEGAL/LINEA/LINEF` exception launch in `rtl/m68k_core.v`.

- **Second fix (decoder ordering):** while testing, $4AFC kept
  decoding as `K_TAS` because the broad TAS pattern
  `16'b0100_1010_11_??_????` matched before the more specific
  ILLEGAL pattern `16'b0100_1010_1111_1100`.  Moved ILLEGAL above
  TAS in `m68k_decoder.v`.  Landed `tests/t109_illegal_trap.s`.

- After both fixes the boot ran 300M cycles without ever
  emitting a STOP (vs the prior Alert-hang STOP at $F807F4).
  Added a temporary `[BRA]` trace of every redirect into
  `$F803xx/$F806xx/$F807xx` -- found the boot DOES take the
  good cold-boot path through `$F80354` (BSR scan; MOVE.L D0,
  $12C(A6)).  So the scan WAS running -- ResModules wasn't
  under-populated, it was just being walked wrong by something
  downstream.

- Traced `[INITRES]` (PC=$F81056 = JSR `$FF9A(A6)` =
  InitResident inside InitCode body).  In the second InitCode
  call (`InitCode(D0=1, D1=0)`) only 3 of 9 residents fired:
  `keyboard.device`, `diag init`, `exec.library` (already done).
  The 4th entry -- `strap` -- did NOT fire.  Strap is the
  trackdisk boot loader; no strap = no DSKLEN = no [BOOTBLOCK].

- Added per-instruction CC trace through the InitCode body and
  caught the bug at `$F81046: CMP.B $0B(A1), D3` where D3=0 and
  $0B(A1) = strap's `RT_VERSION` = $28.  Expected `0 - $28 =
  $D8`, N=1.  Got `cc=N0Z0V0C1`.  N WRONG, so the next `BGT
  $F81036` skipped strap.

- For exec ($F800B2) the same CMP.B ran fine because exec's
  RT_VERSION lives at $F800BD (within the longword $022D0969 at
  $F800BC).  Both the byte at offset 1 ($2D) and the byte at
  offset 3 ($69) are negative-byte values -- the bug was
  masked.  For strap (longword $012800C4 at $F84CC8) offset 1
  = $28 (positive byte after CMP) but offset 3 = $C4
  (positive byte after CMP), so picking the wrong offset
  flipped N=0.

- **Third fix (the real one):** in `m68k_core.v` S_LOAD handler
  for `K_ALU`, alu_b was set to raw `dc_rdata` (a 32-bit
  aligned longword).  The ALU's `mask` logic then took the
  LOW byte (`dc_rdata[7:0]`).  But byte at addr offset N lives
  at bits[(31-8*N):(24-8*N)], not always [7:0].  Replaced with
  `byte_at(dc_rdata, dc_addr[1:0])` for `SZ_B` and the
  matching word slice for `SZ_W` (same shape K_TST / K_NEG /
  K_NOT / K_ALUI already use).  Regression test
  `tests/t110_cmp_b_d16an.s` lays down the exact strap byte
  pattern and verifies CMP.B at each offset.

- Post-fix boot now skips PAST strap-skip and gets further --
  but immediately runs into `[BAD-PC]` events:
    `[BAD-PC] from=80400043 to=23402708 retired=74825891 kind=0`
    `[BAD-PC] from=00f8270c to=80010017 retired=74826111 kind=10`
  Stack pointer at the time is `$FFFFF8EE` -- bottom of 32-bit
  space.  Strap likely jumps into a function that uses
  ColdCapture / KickTag pointers we haven't populated, OR a
  CPU detail still missing.  Next session.

Status: 93/93 tests pass.  Three CPU bugs landed (K_BAD trap,
decoder ILLEGAL/TAS ordering, K_ALU mem-byte alignment).
`strap` now fires; the floppy boot-load chain is no longer
gated on these CPU correctness fixes.  Next blocker is
whatever strap's init reaches into.

### S8: post-K_ALU boot dies in cold-boot AllocMem -- heap reports 507KB free but every chunk is 24 bytes

Goal: chase the `[BAD-PC] from=$F82700` chain from S7 to its
proximate cause.

- `$F82700: MOVEA.L $0114(A6), A1` reads the `ThisTask` slot
  from ExecBase.  At the moment it fires, A6 = **0**, so it
  actually reads the longword at absolute `$0114` (which is
  vector-table noise) and gets `$8001_0001`.  The follow-up
  `MOVE.L D0, $0016(A1)` writes to `$8001_0017` -- a bogus
  high address that bounces back through bus / address-error
  vectors and recursively eats the stack.

- Tracked A6 backwards.  ExecBase is `$43AC` in chip RAM, set
  early in cold boot.  At `$F805E0: MOVE.L A6, $0004.W` we
  see Kickstart write `$43AC` to absolute `$0004` (the
  canonical ExecBase pointer).  That stuck for ~1000 cycles,
  then `[Wr$4] r=1523006 pc=$F817AA wdata=$00000000 be=1111`
  CLEARED `$0004` to 0.

- `$F817AA: CLR.L (A2)` is inside `$F8178A`, which is called
  from the cold-boot main at `$F8072C: BSR $F8178A`.  The
  routine does:
    ```
    $F8178E: MOVEQ #80, D0       ; want 80 bytes
    $F81790: MOVE.L #$10001, D1   ; MEMF_PUBLIC | MEMF_CLEAR
    $F81796: BSR $F81EBC          ; AllocMem-equivalent
    $F8179C: MOVEA.L D0, A2       ; A2 = alloc result, NO null check
    $F8179A: MOVEQ #4, D2
    $F817A4: MOVE.L A2, $0008(A2) ; writes via (A2)
    $F817A8: ADDQ.L #4, A2        ; A2 = 4 (since D0=0 -> A2=0)
    $F817AA: CLR.L (A2)           ; CLEARS $0004 (= ExecBase ptr!)
    ```
  So the proximate cause is that AllocMem returns **0** and
  Kickstart blindly trusts the return value (no `BEQ` check).
  AllocMem failure -> NULL deref -> $0004 clobber -> ExecBase
  ptr lost -> downstream wild PCs.

- Traced AllocMem internals.  At the call:
    - `mh_Free` reads as `$0007_BFE0` = **507,872 bytes free**.
    - The MemHeader at `$00004000` says there's plenty.
  But the free-chunk list walker iterates 11 chunks, every
  single one with `mc_Bytes = $18 = 24`.  D0 (80) > 24, so
  every chunk is rejected.  Walker exhausts and returns 0.

- Traced writes that set `mc_Bytes = $18`.  All come from
  `$F81D28: MOVE.L D0, $0004(A1)` (FreeMem's "insert chunk"
  path), with `D0 = $18` every call.  So something in
  cold-boot is calling FreeMem with `size = 24` over and
  over (>40 times by the failure point), producing tiny
  fragments.  Each chunk's `mc_Next` field claims a small
  free-list of 11 chunks; the other ~507,600 bytes of
  `mh_Free` are stranded somewhere.

- Could not finish identifying *which* code path is the
  FreeMem-of-24-bytes loop.  Two leading suspicions:
    1. The K_ALU mem-byte fix in S7 changed flag results for
       a CMP.B / CMP.W somewhere in MakeLibrary that flips a
       branch -- and now we hit a free-each-slot-individually
       path that didn't run pre-fix (when the boot died at
       the no-boot Alert tail instead).
    2. There's a *separate* CPU correctness gap (still-buggy
       byte op, missing operation, exception-frame layout)
       that turns a single FreeMem(big) into FreeMem(24)x40.

- Tools added this session: `[Wr$4]`, `[ThisTask-W/R/USE]`,
  `[ExpansionInit]`, `[ExecInit]`, `[ExecBaseWrite]`,
  `[BadRead]`, `[Alloc-CALL/RET/CHECK/ATTR/SIZE]`,
  `[Inner-CALL/WALK/CHUNK/LOAD/CMP/FAIL/OK]`,
  `[mh_Free-read]`, `[Wr-18]`, `[Wr-mhFirst]`,
  `[IC_LOOP/IC_ENTRY]`, `[INITRES]`, `[SCAN]`, `[BRA]`,
  `[A6sample]` -- all temporary scopes that have been
  removed; landing only `[EXC]` permanently so next session's
  trace can start small.

Status: 93/93 tests pass.  Boot still loops in recursive
exception storm after AllocMem returns 0.  Next: identify the
FreeMem-of-24-bytes loop in cold boot and figure out whether
it's a CPU bug or a real Kickstart behavior we're feeding
wrong input to.

### S9: the FreeMem(24) "loop" was a misread; sibling lane bugs landed

Goal: identify the FreeMem-of-24 caller hypothesized in S8.

- A/B'd by checking out `96ce886` (just before the K_ALU
  byte/word fix) and re-ran kickstart-boot with a `[Wr-18]` trace
  for any chip-RAM write of `$00000018`.  **27 hits, same PCs**
  (`$F81D28 / $F81D2C`).  The "FreeMem(24)-loop" exists pre-fix
  too -- not introduced by the K_ALU change; the K_ALU fix just
  lets boot reach the consequence (the AllocMem(80) failure).

- Wider trace: `$F81D28: MOVE.L D0, $0004(A1)` writes 24 to a
  chunk's `mc_Bytes`, but it's reached from the allocator's
  *coalesce-or-insert* path inside FreeMem (`$F81CE0..$F81E0E`),
  which has dual roles.  The `D0 = $18` constant comes from
  AllocMem's rounding (`ADDQ.L #7, D0; AND.W #$FFF8, D0` rounds
  size up to a multiple of 8; request of 18 -> 24).

- The caller of every AllocMem(18) hit is **PC = `$F80EFA`**,
  inside the cold-boot resident-list builder:
    ```
    $F80EF6: MOVEQ #0, D1
    $F80EF8: MOVEQ #18, D0          ; size of ListNode + back-ptr
    $F80EFA: JSR   $FF3A(A6)         ; AllocMem(18, 0)
    $F80EFE: TST.L D0; BEQ fail
    $F80F02: MOVEA.L D0, A1          ; new node
    $F80F04: MOVE.B $D(A5), $9(A1)   ; node.Pri = resident.Pri
    $F80F0A: MOVE.L $E(A5), $A(A1)   ; node.Name = resident.Name
    $F80F10: MOVE.L A5, $E(A1)       ; node back-ptr to resident
    $F80F16: BSR   $F81A0A           ; Enqueue
    ```
  This is the ResModules-list builder: 14-byte ListNode +
  4-byte back-pointer per ROM resident.  10-11 allocations is
  the expected number of residents.  **Normal exec behavior, not
  a bug.**

- **The real bug**: `mh_Free` reports `$0007BFE0` = 507,872
  bytes, but the free-chunk walker visits exactly 11 chunks of
  size 24 (= 264 bytes total) before hitting NULL `mc_Next` at
  chunk `$4D08`.  The list is **structurally truncated** --
  more chunks (or one big residue) should account for the
  other 507,608 bytes, but they're unreachable from `mh_First`.

- Hypotheses for the truncation:
  1. Something writes 0 to `(mc_Next)` of chunk `$4D08` (or a
     predecessor), severing the chain.  Could be a library JT
     being built *downward* (`$F81C90: MOVE.L A3, -(A0)`)
     across a free-chunk header.
  2. AllocMem's "consume whole chunk" path (`$F81E10: MOVE.L
     (A1),(A2)`) miscomputes the splice in some edge case.
  3. `mh_Free` is updated independently of `mc_Bytes` (e.g.,
     an allocation that adjusts `mh_Free` without removing the
     chunk from the list), letting the two drift.

- Side investigation: sibling-kind audit (parallel agent) caught
  five more S_LOAD lane bugs of the same shape as the K_ALU fix:
  **K_MOVEA.W, K_MULU, K_MULS, K_DIVU, K_DIVS** all assumed the
  word operand sat in `dc_rdata[15:0]` instead of `[31:16]` when
  `dc_addr[1]==0`.  Fixed via a single `mem_word_w` wire (commit
  `3d582b0`); `tests/t111_movea_w_mem.s` exercises MOVEA.W at
  both word lanes plus sign-extend boundaries.  Also flagged:
  K_CHK with memory source schedules a load but has no S_LOAD
  case -- the bound check, exception launch, and N-flag commit
  never run.  Tried to add one but `exc_launch_c` is only
  sampled from `S_RUN`, so launching a CHK trap from `S_LOAD`
  doesn't fire.  Deferred.

- After the MOVEA / MULU / MULS / DIVU / DIVS fixes,
  kickstart-boot trace is **byte-identical** to pre-fix at the
  failure point: same 11 free chunks, same NULL `mc_Next` at
  `$4D08`, same recursive exception storm.  So the truncation
  isn't caused by any of these lane bugs.

Status: 92/92 tests pass (added t111_movea_w_mem).  Boot wall
unchanged: AllocMem fails because the free-list chain is
truncated at chunk `$4D08`.  Next session: bus-trace 4-byte
writes at `$4D08` (and the other free-chunk header addresses)
to pinpoint what severs the chain.

### S10: K_ALU direction=1 was a silent no-op -- mh_Free never decremented

Goal: chase the `mh_Free` vs reachable-free-list discrepancy.

- Bus-trace writes to `$401C` (= `mh_Free`).  Result: **one
  write only**, at `r=436,272` with value `$0007BFE0` (initial
  heap size).  AllocMem inner does `SUB.L D0, $001C(A0)` at
  `$F81E04` to decrement the counter -- never fires.

- Decoded `$91A8 $001C`:
    - bits[15:12]=1001 (SUB)
    - bits[11:9]=000 (D0)
    - bits[8:6]=110 (.L, **direction=1** = Dn -> ea)
    - bits[5:3]=101 (mode 5 = d16(An))
    - bits[2:0]=000 (A0)
  That's "SUB.L D0, $001C(A0)" with mem destination.  The
  decoder propagates `direction = opcode[8]`, but the K_ALU
  executor at line 1197 only ever handled the direction=0
  case (mem source, Dn destination).  Direction=1 was a silent
  no-op: load scheduled but no writeback, no CCR.  Same
  pattern as the old MOVE-FROM-SR mem-dest and K_ALUI
  mem-dest bugs, just for the K_ALU family.

- Mirrored the K_ALUI mem-dest plumbing:
    - New `alu_mem_dst` / `alu_mem_dst_writes` wires gated on
      `K_ALU && direction==1 && src_mode != EA_DREG/AREG`.
    - K_ALU S_RUN executor schedules the RMW load when
      `alu_mem_dst`.
    - K_ALU S_LOAD ack does the lane-extract (byte_at /
      word-half / long) and swaps operand roles:
      `alu_a = mem`, `alu_b = ex_rb (Dn)` -- so SUB.L D0,mem
      computes `mem - D0` (not `D0 - mem`).
    - Sequential block captures `alu_y` + CCR snapshot
      (`ex_tas_word`, `ex_alui_*_q`) and transitions to
      `S_RMW_W`.
    - S_RMW_W K_ALUI/K_ALUQ commit branch now also includes
      `K_ALU`, with src-An-update routing for predec/postinc
      on the EA.
  Regression `tests/t112_alu_mem_dst.s`: encodes SUB.L /
  ADD.L / AND.W / OR.B Dn,d16(A0) via .word (since asm68k has
  no Dn,<ea> form), verifies memory writeback at each lane
  + neighbour invariants.

- Kickstart 1.3 cold-boot post-fix: **88M retired** in 300M
  cycles (up from 75M), no `[BAD-PC]` chain, crosses a reset
  boundary (two `[OVL] cleared`).  The AllocMem-fail-and-
  clobber loop from S8 is gone.  `mh_Free` trace shows 191
  decrement events now -- the allocator is tracking
  properly.

Status: 93/93 tests pass (added t112_alu_mem_dst).  Boot
progresses past the prior wall but doesn't reach [BOOTBLOCK]
yet; need a fresh trace to see the new state at the reset
boundary.

### S11: post-K_ALU boot wall is K_BAD at chip-RAM $6382 / opcode $4D14

Goal: find where the boot is looping at retired=88M post K_ALU
mem-dst fix.

- Periodic PC sampler (every 4M instr): all samples land in
  `$F83Bxx` / `$F83Dxx`, SP oscillating between `$FF7A` and
  `$FF76`.  Two-byte stack delta = tight inner loop.

- Disassembled the cluster.  `$F83CF0..$F83D00` is the Alert()
  serial poll: send `D7` (alert code) out via SERDATR, read
  reply, retry 9x.  `$F83BCC` / `$F83B8E` are the serial
  send/receive helpers.  Kickstart 1.3 has a "ROMWACK" debug
  monitor that polls the modem serial port during Alert -- this
  is it.

- Hooked PC=`$F83CEA` (alert inner entry): `D7 = $53414421`
  ("SAD!").  That's not the original alert code; it's the magic
  the Alert handler stuffs in D7 for its serial protocol.

- Found a debug bug: the existing `[EXC] r=` trace was gated
  `is_settled && exc_launch_c`, but `exc_launch_c` *gates*
  `run_launches_exc` which disables `is_settled_in_run`.  The
  two signals are mutually exclusive, so [EXC] never fired
  during the entire S7-S10 investigation.  Fixed in `a3cf0e5`
  to use `ex_state == S_RUN && ex_valid && !halted &&
  exc_launch_c`; also dump `ex_opcode`.

- With the corrected trace, the boot fires **4 exceptions**:
    ```
    [EXC] r=    435874 pc=00f80c5e opcode=4e7b kind=K_BAD  vec= 4 (ILLEGAL)
    [EXC] r=   1537276 pc=00f80be8 opcode=007c kind=K_ALUI vec= 8 (PRIV_VIO)
    [EXC] r=   1546964 pc=00f81742 opcode=241f kind=K_MOVE vec=27 (auto-IRQ 3)
    [EXC] r=   1546982 pc=00006382 opcode=4d14 kind=K_BAD  vec= 4 (ILLEGAL)
    ```
  1. `$4E7B` MOVEC -- expected, the CPU-feature probe.
  2. `$007C` (ORI #imm,SR) from user mode -- expected.
  3. Auto-IRQ level 3 -- a fired IRQ from chipset.
  4. **`$4D14` at chip-RAM `$00006382`** -- the wall.  Our K_BAD
     trap fires correctly, ILLEGAL vector handler runs, Alert()
     is called with `$80000008` ("AN_BogusExcpt") in D7.

- `$4D14` decoded:
    ```
    0100_1101_0001_0100
    bits[15:12] = 0100 (MISC/IMM group)
    bits[11:9]  = 110 (Dn=6 / dst An=A6 / ...)
    bits[8:6]   = 100 (NOT a 68000 valid size for $4xxx series)
    bits[5:3]   = 010 (mode 2 = (An))
    bits[2:0]   = 100 (A4)
    ```
  On 68000, `$4xxx` with bits[8:6]=110 is CHK.W, with bits[8:6]=
  111 is LEA.  bits[8:6]=100 is **invalid on 68000**; on 68020+
  it's CHK.L.  So either:
    a. The code at `$6382` is genuinely garbage / data being
       executed as code (wrong jump).
    b. There's a real 68000 instruction at `$4D14` we should
       support but don't decode.
  (b) is unlikely -- the official Motorola docs list `$4D14` as
  not valid on 68000.  (a) means the boot took a bad jump to
  `$6378`, then ran into garbage at `$6382`.

- Lead-up redirect trace:
    ```
    r=1546923: $F8C1AA  -> $00005074 (K_JSR)     -- JSR via JT
    r=1546924: $00005074 -> $F81708 (K_JMP)      -- JT slot's JMP
    ...
    r=1546979: $F81282  -> $00006378 (K_JMP)     -- JMP (A5) into chip RAM
    r=1546982: $00006382 -> $F80AFC (K_BAD trap) -- crash
    ```
  At `$F81282: JMP (A5)`.  A5 was loaded at `$F81276` from
  `$0090(A6)` (ExecBase + $90).  Whatever that slot points to
  is wrong -- it ended up at `$00006378` which has 5 valid
  instructions then garbage at byte 10.

Status: 93/93 tests pass.  Boot wall is now `K_BAD` at chip-RAM
`$6382` with opcode `$4D14`.  The boot took an indirect JMP via
`A5 = $0090(A6)` into chip-RAM code that becomes invalid after
~10 bytes.  Next: figure out what `$0090(A6)` was supposed to
hold -- it's some exec-private function table.  Probably a
library JT slot that wasn't set up correctly.

### S12: MOVEM mem-to-reg with d16(An) was ignoring the displacement

Goal: figure out what $90(A6) is and why A5 ends up = $6378.

- Traced writes to `$51AC..$51B7` (the $90(A6) / $94(A6) slots,
  with ExecBase = $511C).  Result: **only zero writes** at
  `r=1,528,335..1,528,339` from the AllocMem clear-zone code.
  Yet at r=1,546,977 the MOVEM at `$F81276` returned A5=$6378.
  Impossible -- something has to have written non-zero there.

- Traced ALL bus accesses during the MOVEM (`dc_ack` on, not
  `is_settled` -- MOVEM intermediate cycles aren't settled).
  The smoking gun:
    ```
    r=1546976 pc=00f81276 state=a RD addr=0000511c data=00000000
    r=1546976 pc=00f81276 state=a RD addr=00005120 data=00004924
    ```
  The MOVEM read at `$511C` (= A6 + 0) and `$5120` (= A6 + 4)
  **instead of** `$51AC` (= A6 + $90) and `$51B0` (= A6 + $94).
  **The d16 displacement was being ignored.**

- Two bugs at `K_MOVEM` init in `m68k_core.v` line 2666:
  1. `movem_addr <= ex_ra` (= An), without adding the
     displacement.  For d16(An) we want `An + d16`.
  2. The mask read `ex_src_imm32[15:0]` works only for
     ext_words=1 modes (predec / (An) / postinc).  For
     ext_words=2 modes (d16(An), d8(An,Xn), abs, PC-rel), the
     ID stage packs `ex_src_imm32 = {mask, d16}` so mask is in
     `[31:16]` and d16 in `[15:0]`.  The code was reading the
     d16 as the mask.

- Fix (commit `1fa5595`): case-split on `ex_src_mode` in the
  MOVEM init.  For `EA_DISP`: `movem_addr = ex_ra +
  sign_ext(ex_src_imm32[15:0])`, mask = `ex_src_imm32[31:16]`.
  For `EA_ADEC` / `EA_AIND` / `EA_AINC`: keep old behaviour.
  Regression `tests/t113_movem_d16an.s` exercises the
  `MOVEM.L $80(A0), D1/D3/A2/A5` shape used by Kickstart.

- **Kickstart 1.3 boot post-fix retires 74M instr AND writes
  `DSKLEN` for the first time** -- trackdisk is loading the
  bootblock.  Trace:
    ```
    [DSKLEN] wdata=00002708 dsk_pt=00000018
    [DSKLEN] wdata=011829a8 dsk_pt=00000018
    [EXC] r=74019261 pc=08000888 opcode=ffff kind=K_BAD vec=11
    [BAD-PC] from=08000888 to=08000818
    ```
  Fresh wall: DSKPT (disk DMA destination pointer) is `$18` --
  way too low (should be a real RAM buffer).  The disk DMA
  writes bootblock data over the vector table at `$18+`, then
  PC eventually lands at `$08000888` (chip-RAM noise) with an
  F-line opcode (`$FFFF`), our K_BAD trap fires LINEF, vector
  loop hits another bad PC, etc.

  Probably DSKPT isn't being set properly -- the high half
  might be dropped, or our DSKPT register has a similar
  byte/word lane bug.

Status: 94/94 tests pass (added t113_movem_d16an).  Boot now
calls trackdisk and writes DSKLEN -- a major milestone -- but
DSKPT is wrong so the bootblock lands at $18 instead of a real
buffer.  Next: trace DSKPTH/DSKPTL writes and figure out why
the high half is missing.

### S13: bus's DSKPT/DSKLEN dispatch ignored byte-enable (be)

Goal: figure out why DSKPT ends up at $00000018 after trackdisk
sets it up.

- Traced ALL writes to $DFF020..$DFF02F at the CPU's dc port
  and at the bus arbiter side.  The CPU emits patterns like:
    ```
    addr=$DFF020 be=0011 is_long=1 wdata=$00002708
    addr=$DFF022 be=1111 is_long=1 wdata=$00000018
    addr=$DFF024 be=1111 is_long=1 wdata=$011829A8
    ```
  None are simple aligned MOVE.L at $DFF020.  Looking carefully:
    1. The first is `MOVE.W #$2708, $DFF022` -- our CPU
       long-aligns to $DFF020 with `be=0011` (low lane =
       $DFF022,$DFF023).  wdata[15:0] = $2708.
    2. The second is an unaligned `MOVE.L Dn, $DFF022` spanning
       DSKPTL+DSKLEN.  Bytes: 022 <- wdata[31:24]=$00,
       023 <- wdata[23:16]=$00, 024 <- wdata[15:8]=$00,
       025 <- wdata[7:0]=$18.  So DSKPTL=$0000, DSKLEN=$0018.
    3. The third is aligned MOVE.L at DSKLEN.

- The previous bus dispatch was:
    ```
    if (addr == DSKPTH_ADDR) dsk_pt[31:16] <= wdata[31:16];
    else if (addr == DSKPTL_ADDR) dsk_pt[15:0] <= wdata[15:0];
    ```
  Matched by ADDR ONLY and always pulled wdata from the same
  half.  For pattern (1) (addr=$DFF020 be=0011), it took
  wdata[31:16] = $0000 as DSKPTH instead of recognising that
  be=0011 means "low lane = DSKPTL write" and pulling
  wdata[15:0]=$2708 into DSKPTL.

- **Side rabbit-hole**: trace `[cacheLatch]` shows the CPU
  emits these writes with `cpu_addr` having the high byte set
  ($FFDFF02x).  This is the 68000 "24-bit external bus" idiom
  (commit 56238c5 added bus-side mask).  The bus truncates to
  24 bits before matching, so the matching is correct -- it's
  just confusing to read the cacheLatch output.

- **Fix (commit `f3c2448`):** dispatch by `(addr, be, is_long)`:
    - addr=$DFF020 + is_long+be=1111: aligned MOVE.L Dn,$DFF020
      -> full DSKPT <- wdata.
    - addr=$DFF020 + be=1100: MOVE.W high-lane -> DSKPTH.
    - addr=$DFF020 + be=0011: MOVE.W low-lane (= $DFF022) ->
      DSKPTL.
    - addr=$DFF022 + is_long+be=1111: unaligned long ->
      DSKPTL <- wdata[31:16], DSKLEN <- wdata[15:0].
    - addr=$DFF022 + be=0011 or 1111: word write -> DSKPTL.
    - addr=$DFF024 with various (be, is_long): DSKLEN with DMA
      trigger on bit 31/15 (depending on lane).

- **Result:** Kickstart 1.3 boot post-fix sets `dsk_pt` to
  `$00002708` (chip-RAM buffer) -- the disk pointer chain is
  finally healed.  But the boot still TIMEOUTs at the same
  retired=74M with the same BAD-PC at $08000888.  Why?
  Because trackdisk hasn't actually fired DMA yet -- both
  DSKLEN writes we see have bit 15 = 0 (no DMAEN).  The
  bootblock load never starts.  Possibly trackdisk is
  programming a setup sequence (DSKLEN twice + index pulse +
  something else triggers DMA) that we're not faithful to.

- **Debugging dead-end**: tried hooking the CPU's `dc_ack &&
  dc_we && dc_addr in $DFF02x` -- never fired.  Was a chase
  for "where is the CPU emitting these?" but turned out the
  CPU emits them with the high byte set ($FFDFF02x) and my
  filter required the high byte to be 0.  Lesson: when chasing
  bus events, always include the 24-bit mask explicitly when
  comparing CPU-side addresses.

Status: 94/94 tests pass.  Bus byte-enable dispatch fixed.
DSKPT is now correct (chip-RAM buffer at $00002708).  Boot
still hits the same downstream wall -- needs more digging into
strap's actual flow vs what trackdisk usually does to start
DMA.
