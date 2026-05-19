# Kickstart boot on the sim

## What works
- ROM decryption: `tools/rom_decrypt.py` strips the `AMIROMTYPE1`
  header and XORs the encrypted ROM against `rom.key`.
- Boot flow:
  ```
  python3 tools/rom_decrypt.py kick.rom rom.key kickstart/kick.bin
  make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072
  ```
- The `test-boot-rom-bin` target builds with
  `+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT`, which:
  - Sets `RESET_PC` to `$F800D2` (the actual Kickstart entry; the
    trampoline at `$0400` is shadowed by OVL=1 so we just start at
    the ROM entry directly).
  - Changes STOP semantics from "halt the sim" to "load imm into SR
    and wait for IRQ" (real 68k behavior).
  - Short-circuits two known Kickstart timing-loop bodies (the
    BSET/BCLR LED-blink inner DBFs at `$F8043C`/`$F8044A` and the
    COLOR00-write BGT loop at `$F80462`) so each "visible cadence"
    pass takes seconds instead of hours of sim time.
  - Skips the post-checksum BNE at `$F801AA` (see "Known issues").

## Verified end-to-end
- Reset → LEA SSP → enter `$F800D2` cleanly.
- ROM checksum loop runs (~131K longs, with carry-round addition).
- CIA-A PRA/DDRA writes through the bus, OVL clears correctly when
  Kickstart drives PA0 low with DDRA bit 0 = output.  Confirmed via
  `$display` trace: `PRA=00`, `DDRA=03`, `OVL cleared`.
- Power-on LED blink writes succeed on the CIA-A slave.

## Known issues blocking further progress

### 1. Checksum diverges from $FFFFFFFF around iter ~100K
The compute is correct for the first ~95K iterations (verified by
Python re-run with the same ROM bytes), then D5 jumps off by
specific values like `$201FBE1B` and `$E153BE5C` and never recovers.
The divergence happens during the `add.l (A0)+, D5` body with no
intervening exceptions or context switches.  Suspected: a subtle
I-cache vs. D-cache race or bus-arbitration ordering when reading
uncached ROM data while the I-cache is fetching the same loop body
nearby.  Workaround: `KICKSTART_FAST_BOOT` skips the BNE that gates
on this so we proceed past stage 1.

### 2. Vector-table readback fails
After OVL clears, the vector-init at `$F801B2` writes 46 vectors to
`$0008..$00C0` via `move.l A1, (A0)+`, then reads them back at
`$F801C8` via `cmpa.l -(A0), A1`.  The readback comparison fails,
suggesting either the writes don't land in the expected memory cells
or the reads pull from a stale cached value.  Needs investigation.

## Debug knobs
- `+define+KICKSTART_BOOT_PC_TRACE` — `$display` every retired PC + kind
  (millions of lines).
- `+define+KICKSTART_BOOT_TRACE`    — `$display` STOP events.

Override the cycle budget with `ROMCYCLES`:
```
make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072 \
    ROMCYCLES=500000000
```

To trace cache/CIA internals during a boot run, add `EXTRA_VERI_DEFS`:
```
make test-boot-rom-bin ROMFILE=kickstart/kick.bin ROMSIZE_WORDS=131072 \
    EXTRA_VERI_DEFS='+define+KICKSTART_BOOT_PC_TRACE'
```

## Files
- `tools/rom_decrypt.py` — AMIROMTYPE1 XOR decrypt
- `kickstart/kick.bin`   — generated (gitignored)
- `rtl/m68k_core.v`      — KICKSTART_BOOT (STOP) + KICKSTART_FAST_BOOT
                          (LED-blink/COLOR00/post-checksum skips)
- `rtl/m68k_defs.vh`     — RESET_PC = $F800D2 under KICKSTART_BOOT
- `Makefile`             — VERI_DEFS / EXTRA_VERI_DEFS / ROMCYCLES hooks
