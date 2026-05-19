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

### 1. Checksum diverges from $FFFFFFFF around iter ~100K — FIXED
Root cause: the legacy chipset / block-device / keyboard-inject /
bitplane-probe testbench registers at `$00FE_xxxx` ($00FE_0000-$00FE_05FF,
$00FE_8000-$00FE_800C, $00FE_9000, $00FE_9100+) overlap with the
512 KB Kickstart ROM region ($00F8_0000-$00FF_FFFF).  When the
checksum walked past `$F80000+$60000`, its `add.l (A0)+, D5` reads
were routed to the testbench peripherals (which return 0) instead of
the real ROM bytes.  Different sub-windows aliased at different
offsets, so the divergence point varied between runs.

Fix in `rtl/m68k_bus.v`: a `localparam LEGACY_CHIPSET =
(ROM_WORDS <= 98304)` gates every testbench-only `is_*_legacy`,
`is_blk_reg`, `is_kbd_inject_reg`, and `is_bpl_probe_reg` decode.
For ROM_WORDS large enough to extend into `$FE_xxxx` the testbench
registers stay disabled so ROM wins.  Canonical Amiga chipset at
`$00DF_F000+` is unaffected.

`KICKSTART_FAST_BOOT`'s skip of the post-checksum BNE at `$F801AA`
is no longer needed and has been removed.

### 3. Library jump-table called before init — open
Past the vector init and memory probe ($4000..$80000), Kickstart sets
up a library base in `A6=$4628` and then calls `JSR -$27C(A6)` at
`$F81CB0`.  Target `$000043AC` is in uninitialized low RAM, so the
CPU executes ~130K `ORI #imm,Dn` pseudo-instructions worth of zeros,
eventually trips a CHK at `$00080006`, vectors to the default handler
($F80416), LED-blink, RESET, restart.  Either Kickstart's exec
library hasn't laid down its jump vectors yet by this point, or A6
is bogus.  Needs more digging into the boot sequence
`$F80238-$F81CB0`.

### 4. Address-error too strict — FIXED
The CPU was trapping `.L` accesses on any address with `bits[1:0] !=
00`.  Real 68000 only traps when bit 0 is 1 (odd byte); even-but-not-
mod-4 addresses are legal `.L` accesses (the CPU does two consecutive
word reads).  Kickstart's `ADD.L $26(A6), D1` at `$F801EE` (A6=0,
EA=$26) was incorrectly trapping to vector 3 which routed to the
default LED-blink handler.  Fixed in `rtl/m68k_core.v` by relaxing
the `.L` check to only test bit 0.  `tests/t77_addr_error.s` updated
to match real 68k semantics.

### 2. Vector-table readback fails — FIXED
After OVL clears, the vector-init at `$F801B2` writes 46 vectors to
`$0008..$00C0` via `move.l A1, (A0)+`, then reads them back at
`$F801C8` via `cmpa.l -(A0), A1`.  Root cause: when OVL=1 the cache
filled lines at low-memory tags with the ROM data routed via the
OVL shadow.  After CIA-A clears OVL those cached lines became
stale (the same line index now belongs to RAM addresses with a
different tag, but the post-write reads under a SHORT word index
could match the previous tag).  Fix (`m68k_bus.v` +
`m68k_cache.v` + `m68k_top.v`): the bus exports a one-cycle
`ovl_clear_pulse_o` on the OVL 1→0 edge; both the I- and D-cache
of every core take that as `inval_all_i` and clear all `valids[]`.
Verified end-to-end by Kickstart boot: the readback BNE at
`$F801CA` no longer trips and execution falls through to the early
exec.library init at `$F801D2`.

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
