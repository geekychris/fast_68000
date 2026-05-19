# Kickstart boot attempt — current state

## What works
- ROM decryption: `tools/rom_decrypt.py` strips the `AMIROMTYPE1` header
  and XOR-decrypts an AmiKit-style ROM against `rom.key`.
- Boot flow: `make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072`
  runs the t63 trampoline, which reads SSP from `$0` and PC from `$4` (via
  OVL=1 ROM overlay) and jumps to `$00F800D2` (the Kickstart entry).
- STOP semantics: with `+define+KICKSTART_BOOT` STOP loads imm into SR and
  waits for an IRQ (instead of halting the sim).  Test default is preserved.

## Current wall
Kickstart runs 12,399 instructions, then PC ends up at `$42A8040D` — an
address outside both RAM ($00000-$1FFFFF) and ROM ($F80000-$FFFFFF).
Memory at that address reads as zero from the bus, so the CPU decodes
`$0000` as a STOP #0 forever.

No redirect to an address with bit 30 set was traced during the run,
which means PC reached `$42A8040D` either via:
  - sequential IF advancement after an earlier redirect that *was* in
    range but landed in unmapped territory (would have read as 0s, so
    PC advances through "NOP NOP" garbage until hitting the STOP-shaped
    word) — most likely
  - a JMP/JSR/RTS target read from corrupted memory

To debug from here:
  - Build with `+define+KICKSTART_BOOT_TRACE` to print every STOP and
    bit-30-set redirect.
  - Add a tracer that fires whenever a JMP/JSR/RTS computes a target
    outside ROM/RAM.  That likely finds the actual point of
    corruption.

## Files touched
- `tools/rom_decrypt.py` — new
- `kickstart/kick.bin` — generated (gitignored)
- `rtl/m68k_core.v` — KICKSTART_BOOT wait-for-IRQ STOP semantics
- `Makefile` — VERI_DEFS hook so the build can `+define`
