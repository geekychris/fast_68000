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

## Session log

Convention: each session opens with a one-line goal and closes with
a `Status:` line describing what runs.  Within a session, each
discovery / hypothesis / experiment is its own bullet.

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
