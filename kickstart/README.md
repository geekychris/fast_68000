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

### 3. Library jump-table called before init — FIXED
Two intertwined CPU/bus bugs were silently corrupting Kickstart's
library jump-table construction at `$F81C7C` (`MOVE.L A3, -(A0)` /
`MOVE.W #$4EF9, -(A0)` loop) and the subsequent `JSR -$27C(A6)` at
`$F81CB0`:

a) **JSR/JMP d16(An) ignored the displacement.**  Our K_JMP / K_JSR
   computed `take_branch_target_c = ex_ra` (= raw `An`).  For source
   modes other than absolute, that drops the `d16/d8` displacement
   entirely.  Every `JSR -N(A6)` style library call therefore
   jumped to `A6` itself instead of `A6-N`.  Fix
   (`rtl/m68k_core.v`): use `src_ea` which is computed correctly for
   all EA modes.

b) **Unaligned `.L` writes clobbered the wrong `mem[]` entry.**  The
   jump-table loop alternates `MOVE.L A3, -(A0)` (4-byte longword)
   and `MOVE.W #$4EF9, -(A0)` (2-byte opcode), so the longword
   writes land at `A0` values where `A0[1:0] == 10` -- a word-
   aligned but not longword-aligned address.  Our bus indexed
   `mem[]` by `addr[18:2]`, dropping bits 1:0; an unaligned `.L`
   write therefore overwrote the entire `mem[mem_idx]` longword
   with the four wdata bytes, blowing away the `$4EF9` opcode just
   written 2 bytes above and leaving the next mem[] entry untouched.
   Real 68000 silicon expands this internally to two consecutive
   word transactions.  Fix (`rtl/m68k_bus.v`): detect `be=1111 &&
   addr[1:0]=10`, split the write across `mem[mem_idx][15:0]` and
   `mem[mem_idx+1][31:24:16]`.

With both fixes the loop builds the 151-entry exec.library jump
table correctly, `JSR -$27C(A6)` at `$F81CB0` now lands at
`$00004130` (the right JMP slot), JMPs to `$00F80CF6` (an exec
function), RTSs back to `$F81CB4`, and Kickstart continues with
real exec.library init.

### 5. Unaligned .L stack reads (RTS pop) — FIXED
Some Kickstart routines `LINK A5, #-$E` (subtracts 14, not a
multiple of 4), leaving A7 word-aligned-but-not-mod-4.  Subsequent
`BSR` push + `MOVEM` save/restore + `RTS` pop reads the saved PC
from `(A7)` with `A7[1:0] = 10`; without unaligned-.L support the
read returns the longword *before* the intended one and boot jumps
to a bogus PC like `$03E400F8`.

Fix: add a `cpu_is_long` side-band signal plumbed CPU → cache →
bus.  The CPU sets it to `(ex_size == SZ_L)` at the `dc_req` commit;
the bus latches it as `granted_is_long_q` and uses
`(granted_is_long_q && granted_addr_q[1:0] == 2'b10)` as the
unaligned-.L assembly condition.  Required setting `size = SZ_L`
on K_RTS / K_RTE / K_RTR in the decoder (they had defaulted to
SZ_W since they don't carry a size in the encoding).

Result: Kickstart now boots through 1.4B+ instructions of real
exec.library init code -- previously it stalled at ~503K
instructions when the first unaligned RTS pop landed.  All 82
regression tests pass, including the new t96_irq_savereg case.

### 6. Resident-tag scan loop ran forever — FIXED
Two follow-on bugs were keeping the boot in the resident-tag scan
even after unaligned-.L reads worked for RAM:

a) **Unaligned .L read of the resident-list terminator at `$F8037E`
   returned the wrong bytes.**  The terminator is the longword
   `$FFFFFFFF` at `$F8037E` (i.e. addr[1:0] = 10, straddling
   `rom[$DF]` and `rom[$E0]`).  The bus's unaligned-.L assembly
   path was only wired into the `mem[]` (RAM) return -- the
   `granted_is_rom_q ? granted_rom_data_q` short-circuit above it
   captured only `rom[rom_idx]` (the aligned half), so the read
   returned `$0000FFFF` instead of `$FFFFFFFF`.  The list-walker at
   `$F80E70` checks `CMP.L #-1, D1 ; BEQ exit`, never saw -1, and
   walked off the end of the in-ROM list into low-RAM zeros.
   Fix in `rtl/m68k_bus.v`: when `is_long[winner] &&
   addr[winner][1:0] == 2'b10`, capture
   `{rom[rom_idx][15:0], rom[rom_idx+1][31:16]}` into
   `granted_rom_data_q` instead.

b) **Second LED-blink timing loop at `$F84038` / `$F84044`.**  The
   post-init init-OS code re-blinks the floppy LED via a BSET/BCLR
   pair driven by inner DBF instructions, identical pattern to the
   power-on probe at `$F8043C` / `$F8044A` that we already short-
   circuit under `KICKSTART_FAST_BOOT`.  Added the two new PCs to
   the same skip.

### Status: Kickstart boots
With all of the above, Kickstart now runs to completion: ROM
checksum → vector init → memory probe → exec.library setup → JT
build → InitResident scan completes → screen blank + COLOR00 loop →
JMP into the cold-reboot epilogue (`$F80E06-$F80E18`) → `RESET`
opcode → `JMP (A0)` back to `$F800D0`.  The boot then cycles
through this reset path repeatedly (~26 times in 100M cycles)
because there is no bootable floppy / hard disk to find.  Total
unique PCs visited: 544.

### Next: provide a boot device
To progress past the reset loop, the system needs to expose a
working DSKBLK / trackdisk DMA path that returns a valid MFM
boot block (`DOS\0` magic + checksum + bootblock code) so
Kickstart's trackdisk.device picks it up and JSRs the bootblock.
The bus already has the MFM DMA stub; the missing piece is a
real Workbench-style MFM image generator (or fake DOS\0 short
boot stub) loaded via `DISK_HEXFILE`.

### 7. 16-bit chipset reads landed in the wrong half — FIXED
Every chipset MOVE.W `<reg>`, Dn comes through the 32-bit bus as
a long-aligned read.  The CPU's MOVE.W extraction picks
`dc_rdata[31:16]` when the access addr[1] is 0 and `[15:0]` when
it is 1 (the long-aligned bus access straddles two words and the
.W lands in whichever half the address selects).  Earlier the bus
put every 16-bit chipset value (SERDATR, POTGOR, ADKCONR, VPOSR,
the JOY/POTxDAT pairs, DMACONR, VHPOSR) in the low half of the
response unconditionally.  Registers at odd-word offsets
(POTGOR/POTxDAT/JOY0DAT/VHPOSR/DMACONR) accidentally worked
because addr[1] was 1; registers at even-word offsets (SERDATR
$DFF018, ADKCONR $DFF010, JOY1DAT $DFF00C, VPOSR $DFF004) read as
$0000 because the CPU took the high half.

Fix in `rtl/m68k_bus.v`: condition both `agnus_resp_w` and the
`granted_is_paula_ro_q` resp on `granted_addr_q[1]` so the value
lands in `[31:16]` when addr[1]=0 and `[15:0]` when addr[1]=1.
Regression: `tests/t100_chip_word_half.s` does MOVE.W $DFF018
(addr[1]=0) and MOVE.W $DFF016 (addr[1]=1) and checks both halves.

With this fix, Kickstart's SERDATR self-test at `$F84048`
(`AND.B #$7F, D0 ; CMP.B #$7F, D0 ; DBLS / BEQ`) actually sees
$7F (when SERDATR_VAL = $607F) and takes the BEQ to $F83BF6
(strap.lib continuation) instead of falling through to the
direct cold-reboot path at $F80452.

### 8. MOVEM ra-port poisoned the next instruction — FIXED
The CPU has only three regfile read ports; the `ra` port is
shared between the ID stage's source-register read and MOVEM's
"current register being pushed" value (the data the bus needs
when MOVEM is doing a register-to-memory write).  The mux
`rf_ra_idx_eff = movem_active ? movem_curr_reg_full : id_ra_idx`
left the MOVEM-current register selected on the cycle MOVEM
retired — which is exactly when the ID→EX boundary advances and
the next instruction's `ex_ra` latches.  So any instruction
immediately after MOVEM whose source was a register *not in the
MOVEM mask* silently captured one of the just-pushed MOVEM
registers' value instead.

The failure mode in Kickstart 1.3:

```
MOVEM.L D2/D3, -(SP)          ; inner MOVEM in AllocMem at $F81ECE
MOVE.L  D0, D3                 ; should set D3 = AllocMem size arg
```

`ex_ra` for the MOVE captured the just-saved D3 value (= 0)
rather than D0 (= 18).  AllocMem's inner function tested
`MOVE.L D0,D3 ; BEQ` (treating D3 as the size argument), saw
zero, took the zero-size early-return path, and returned with D0
unchanged.  The caller then stored D0 (=18, the requested size)
as the "allocated pointer".  Later `FreeMem(18, 18)` walked the
MemList looking for a MemHeader owning address 18, fell off the
end, and `Alert($0100000F = AN_MemoryInsane)`.

Fix in `rtl/m68k_core.v`: change the mux to
`movem_active && !is_settled_after_movem` so the ra port hands
back to `id_ra_idx` on the retirement cycle.  Reads are
combinational in the regfile, so the latch on the next clock
edge captures the correct source register value.

Regression: `tests/t101_movem_ra_hazard.s` reproduces the
exact MOVEM+MOVE pattern from AllocMem and verifies the
following MOVE reads its operand correctly.

With this fix Kickstart's InitResident finds all 8 ROM-resident
modules (exec, expansion, ramlib, audio, console, trackdisk,
gameport, keyboard at $F800B2/$F841DE/$F84CBE/$FA8C72/$FB9EFE/
$FBC6E6/$FC2486/$FC9A96), AllocMem returns valid pointers, and
the boot runs to ~1.4M retired instructions before jumping into
an apparent garbage region around `$038F_Axxx` (= Z2 FastRAM
area).  That's the next thing to chase — probably an early
resident's Init function reading bogus values from our autoconfig
or FastRAM stub.

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
