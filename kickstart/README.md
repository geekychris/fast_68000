# Kickstart boot on the sim

## What works
- ROM decryption: `tools/rom_decrypt.py` strips the `AMIROMTYPE1` header
  and XORs the encrypted ROM against `rom.key`.
- Boot flow:
  ```
  python3 tools/rom_decrypt.py kick.rom rom.key kickstart/kick.bin
  make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072
  ```
- The boot-rom-bin target builds with `+define+KICKSTART_BOOT`, which:
  - Sets RESET_PC to `$00F800D2` (Kickstart's first instruction; the
    trampoline at `$0400` is shadowed by OVL=1 so the CPU just starts
    at the ROM entry).
  - Changes STOP semantics from "halt the sim" to "load imm into SR
    and wait for an IRQ" (real 68k behavior).

## Current progress
8M+ instructions of real Kickstart code run successfully.  Trajectory:

  - `$F800D2` — entry; sets SSP via LEA, initial register load.
  - `$F800E2` — ROM-checksum loop: 32-bit add of all 131,072 ROM longs
    into D5 with carry-in via ADDQ, then DBF inner+outer (~500K
    instructions).  Runs to completion.
  - `$F80100` onward — checksum compare, branches into setup code.
  - `$F80434`/`$F80442` — power-on LED blink loop.  Sets/clears bit 1
    of CIA-A PRA (`$00BFE001`) inside two DBF loops with D0 = $FFFF
    then $3FFF inner counts, and an outer D1 = 10.  Each call to the
    blink routine is ~1.8M instructions; Kickstart appears to call it
    several times during stage-1 init.

At 100M cycles we get through ~16M instructions and Kickstart is
still iterating through its early init (mostly the LED blinks).

## Debug knobs
- `+define+KICKSTART_BOOT_PC_TRACE` — `$display` every retired PC + kind.
  Useful but produces millions of lines.
- `+define+KICKSTART_BOOT_TRACE`    — `$display` STOP events.

Override the cycle budget with `ROMCYCLES`:
```
make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072 \
    ROMCYCLES=500000000
```

## Next steps
- Speed: at our current ~6 cycles/instr, real Kickstart's full init
  needs hundreds of millions of cycles in the sim.  Either run longer
  or (eventually) patch the LED-blink loops to skip the inner spins.
- Once init completes, expansion.library will probe autoconfig --
  our 8MB FAST RAM card should bind.  Then exec.library init runs,
  then trackdisk.device wants to boot from the disk image.

## Files touched
- `tools/rom_decrypt.py` — AMIROMTYPE1 XOR decrypt
- `kickstart/kick.bin`   — generated (gitignored)
- `rtl/m68k_core.v`      — KICKSTART_BOOT wait-for-IRQ STOP semantics
- `rtl/m68k_defs.vh`     — RESET_PC = $F800D2 under KICKSTART_BOOT
- `Makefile`             — VERI_DEFS hook + ROMCYCLES override
