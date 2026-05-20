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
the boot runs to ~1.4M retired instructions.

### 9. MOVEM didn't set dc_is_long; unaligned .L reads broke — FIXED
The MOVEM-step issue path in `rtl/m68k_core.v` set `dc_req_r`,
`dc_we`, `dc_addr`, `dc_be` but forgot to drive `dc_is_long`.
The bus's unaligned-.L read assembly (`{mem[idx][15:0],
mem[idx+1][31:16]}` when `addr[1:0] == 2'b10`) only fires when
`granted_is_long_q` is set, so MOVEM-loads from a stack whose
top was word-aligned-but-not-mod-4 returned `mem[idx]` verbatim
— the wrong half — and pulled garbage into the restored
registers.

In Kickstart 1.3's InitResident driver at `$F80E48`:

```
LINK A5, #-$0E          ; A5 = $3E4, SP = $3D6  (SP[1:0] = 10)
...
MOVEM.L D2/A5, -(SP)    ; saves A5 = $3E4 to mem[$3CA]
... scan body ...
MOVEM.L (SP)+, D2/A5    ; should restore A5 = $3E4
```

The save wrote `$3E4` to `mem[$3CA]` correctly via the bus's
existing unaligned-.L *write* path.  The restore, lacking
`dc_is_long`, read `mem[$F2]` directly (`mem[$F2] = mem[$3C8] |
mem[$3C9] | mem[$3CA] | mem[$3CB]`) and pulled the *high* word
of the longword starting four bytes earlier — i.e. the *D2*
slot.  A5 ended up as garbage like `$FFFB0000` / `$FFFFFFFB`,
and the eventual `UNLK A5` at `$F80E58` tried to read
`mem[$FFFFFFFB]` (odd address) and triggered an address-error
trap with PC garbage.

Fix in `rtl/m68k_core.v`: assign `dc_is_long <= (ex_size ==
SZ_L)` inside the MOVEM-step issue path.  Regression:
`tests/t102_movem_unaligned_l.s` does the LINK A5,#-$0E + 4-reg
MOVEM round-trip on an unaligned stack and verifies every
register's value round-trips intact.

With this fix Kickstart's InitResident scan completes cleanly,
the driver's UNLK A5 lands on the proper saved A5, and boot
runs to ~42M retired instructions before hitting a new failure
(BAD-PC from JT slot `$00004024` to `$038F_0305`).

### 10. RTE/RTR PC pop missed dc_is_long — FIXED (preventive)
The RTE/RTR state machine (`S_RTE_POP_SR` → `S_RTE_POP_PC`) sets
`dc_req_r` / `dc_we` / `dc_addr` / `dc_be` but didn't drive
`dc_is_long`.  The PC pop is .L from `SP+2` (SP was .L-aligned,
SR pop bumped it by 2), so the read address is always
word-aligned-but-not-mod-4 — the same unaligned-.L case the
MOVEM fix above addresses.  Without `dc_is_long` set the bus
returned `mem[idx]` verbatim (= the wrong half) and the redirect
landed in garbage on the first exception that came through RTE.

Fix in `rtl/m68k_core.v`: clear `dc_is_long` for the SR pop (it's
.W) and set it for the PC pop (always unaligned .L).  No new
regression test — `tests/t96_irq_savereg.s` already exercises
the RTE path but didn't trip the bug because its supervisor
stack happened to stay .L-aligned at RTE entry.

### 11. Slow/Expansion-RAM probe writes aliased into vectors — FIXED
Kickstart's memory probe writes patterns at $00C0_xxxx /
$00D0_xxxx (the slow/expansion RAM regions) and reads them back
to size the RAM.  Our bus uses `mem_idx = addr[18:2]`, which
silently truncates the upper address bits — so a write to
$00C0_000A landed at `mem[$2]` (vector 2) and a write to
$00C0_000E landed at `mem[$3]` (vector 3).  Kickstart's probe
saw consistent write/read (because both sides aliased) and
concluded slow RAM was present — but the writes had already
clobbered the exception vector table.  Vector 3 (address error)
ended up holding `$038F_0305`, so every address-error trap
landed in garbage and the boot dropped into an exception storm.

Fix in `rtl/m68k_bus.v` (gated on `+define+KICKSTART_BOOT` so
bench tests that intentionally use high addresses as fake RAM
still alias): writes to `addr >= (MEM_WORDS << 2)` are dropped,
and reads return 0.  Existing chipset / CIA / ROM / autoconfig
slaves run first in the if-else chain so they're unaffected;
the guard only fires when the access has fallen through to the
main-RAM default and the address is outside the populated
window.

With this fix Kickstart progresses through chip-RAM init,
exception-vector setup, and into the strap.lib boot-device
search.  Boot now runs to ~5.3M retired before tripping a new
BAD-PC: RTS at $F80BE6 pops `$0B0C_00F8` from the supervisor
stack at $FFE0 — i.e. the saved return address is still
corrupt.  Next thing to chase.

### 12. D-cache returned aligned longword for unaligned .L reads — FIXED
The cache stores aligned 32-bit longwords indexed by
`addr[9:2]`.  Its hit check ignored `addr[1:0]`, so an unaligned
.L read (`addr[1:0]==10`) of a previously-cached line returned
the aligned longword at `(addr & ~3)` — the wrong four bytes.
The bus's read-side unaligned-.L assembly path
(`{mem[idx][15:0], mem[idx+1][31:16]}`) was never invoked for
cache hits.

Kickstart 1.3 trips this on its first task switch.  The
supervisor stack at $FFE8 gets cached during normal access.
The task setup pushes PC.L unaligned at $FFEA and SR.W aligned
at $FFE8, leaving `mem[$3FFA]` = `$00F8_00F8` (SR high half ||
PC high half).  When `RTE` later pops PC.L from $FFEA, the
cache hits and returns the *aligned* longword $00F800F8
instead of the spliced PC.  RTE redirects to garbage.

The same bug also bites the cached fill: if a miss brought in
an unaligned .L's spliced result and stored it as the aligned
line, the next read at the aligned index would see corrupted
data.

Fix in `rtl/m68k_cache.v`:
- `comb_read_hit` now gates on `!is_unaligned_long` (where
  `is_unaligned_long = cpu_is_long && cpu_addr[1:0]==10`), so
  unaligned .L hits force a bus access.
- The miss-issue path also sets `saved_io <= is_io ||
  is_unaligned_long` so the spliced bus response is *not*
  cached (it doesn't correspond to any aligned mem line).

Regression: `tests/t103_cache_unaligned_l.s` primes the cache
with an aligned read, writes an unaligned .L, and verifies
both aligned and unaligned reads return the right bytes.

### 14. MOVE-FROM-SR to memory was a silent no-op — FIXED
Our `K_MOVESR` direction=0 (MOVE-FROM-SR) handler only emitted a
write-back when `dst_mode == EA_DREG`.  Memory destinations (-(An),
(An)+, (An), d16(An), etc.) just dropped on the floor: no memory write,
no An update.  On real 68000 these are valid data-alterable EAs.

Two bugs, both in `rtl/m68k_core.v`:
a) `id_rb_idx`'s case statement defaulted K_MOVESR's `id_rb_idx` to
   `4'd0` (= D0).  For MOVE.W SR, -(An) the EX stage needs ex_rb = An
   so the predec computes `An - 2`.  With the default, dst_ea was
   computed from D0 and the write landed in random low memory.  Fix:
   include K_MOVESR in the K_MOVE/K_MOVEA case so `id_rb_idx =
   id_dst_base_idx`.
b) The K_MOVESR `direction == 0` handler had no `else if (dst_is_mem)`
   branch.  Added one that sets `want_mem/want_we/want_addr/want_be/
   want_wdata` exactly like K_MOVE's .W store, with `dst_an_update`
   committed in the S_STORE case (matching K_MOVE's pattern).

Kickstart 1.3 hits this in strap.lib's transition-to-supervisor
sequence:
```
ORI.W #$2000, SR     ; supervisor (or trap to vec 8 if user)
PEA   <ret>           ; push synthetic PC
MOVE  SR, -(A7)       ; push synthetic SR
JMP   (A5)            ; dispatch
```
With `MOVE SR, -(A7)` a no-op, A7 didn't decrement past PEA's push,
so subsequent supervisor code (Switch / scheduler) read the synthetic
RTE frame from the wrong offset.  The first task switch's RTE popped
PC = $0C3400F8 (high half of the PEA'd $00F80C34 spliced with the high
half of the next pushed return $00F827C2) and crashed.

Regression: `tests/t104_movefromsr_mem.s` snapshots SR into Dn then
into memory via -(An) and (An)+, verifies both stores match and the
An updates.

### 15. SR-write transition to user mode didn't swap A7 — FIXED
On real 68000, A7 is a dual-register pair: SSP when S=1, USP when
S=0.  Whenever S toggles, A7's visible value must swap between
SSP and the stashed USP (`usp_shadow` in our impl).

Our impl handled the swap on `RTE` cleanly (line 2884), but the
"direct" supervisor→user transitions via SR writes
(`ANDI/ORI/EORI #imm, SR`, `MOVE.W src, SR`) only saved the current
A7 (=SSP) into `usp_shadow` and never loaded the prior USP back
into A7 — A7 silently kept the supervisor pointer.  A comment at
the swap site flagged this as deferred ("use RTE for clean U/S
transitions").

Kickstart 1.3 hits this directly at `$F807CE`: `ANDI.W #$0000, SR`
drops the running supervisor code into user mode.  Without the
swap, A7 stayed at SSP = $10000.  User code then pushed BSR
returns and locals onto what was actually the supervisor stack,
silently corrupting whatever the supervisor code stashed there.
The first priv-vio trap from this "user" mode pushed its frame on
top, and Switch's MOVEM-save then overlapped with the BSR push
two frames up.  The eventual RTS popped garbage and crashed.

Fix in `rtl/m68k_core.v`: in the combinational planning block for
both `K_ALUI` to SR and `K_MOVESR` direction=1, when the new
`sr_data_c[`SR_S]` is 0 and current `sr_s` is 1, drive
`wb_aux_we_c=1 / wb_aux_idx_c=15 / wb_aux_data_c=usp_shadow` so the
regfile commits A7 := old USP at the same settle.  The existing
sequential block still saves the outgoing A7 (=SSP) into
`usp_shadow`.

Regression: `tests/t105_sr_user_swap.s` sets USP, clears S via ANDI
to SR, verifies A7 swapped, pushes a sentinel onto the user stack,
confirms the supervisor-stack sentinel is untouched, then takes a
TRAP back to supervisor and re-verifies A7.

With this fix Kickstart's boot reaches ~17M+ retired instructions
in 100M cycles with no BAD-PC.

### Where the boot now spends its time
Profiling at 1B cycles (169M retired): the boot sits entirely
inside a memory-rotate / display-update loop at
`$F86EBE`–`$F86F9A`, dispatched from `$F81168` (a PC-relative
jump-table dispatch driven by a byte stream at `(A1)`).  The inner
body is an 8x-unrolled `MOVE.L (A5)+, (A6)` block fanning out to
five destination pointers (`A0`–`A4`), interleaved with `ROL.W
(An)` rotates on those buffers and a `DBF D3` inner-loop counter.
The outer counter at `8(A7)` runs `BNE.W $F86EBE` for up to 65 535
iterations; in 1B cycles we complete 322 of them.

The shape — five parallel write streams + per-buffer rotate +
two-level nested counter — is consistent with the Kickstart
**bitplane "scroll the prompt"** routine that animates the
floppy-insert screen.  No CPU bug; this is real work that takes
a long time in our slow Verilator simulation.

Reaching this routine without any BAD-PC, illegal-instruction trap,
or stack corruption confirms the strap.lib → exec.library →
display setup path is now sound end-to-end.  Further progress
requires a real boot device (an MFM trackdisk image) so Kickstart
exits the prompt loop into bootblock execution.

### Open: Task's saved PC computed as $0C3400F8 (pre-MOVE-FROM-SR fix)
With all the above, Kickstart now runs to ~2.4M retired but
the first task switch's RTE pops PC = $0C3400F8 — a slow-RAM
address with no backing memory.  The task setup wrote this
value to the task struct's saved-PC slot at $5D5E.

### 13. Real 68000 has a 24-bit external address bus — FIXED
Kickstart 1.3 stores APtrs with non-zero top bytes (flag bits
packed into bits 24..31) and assumes the bus ignores them, so
$0C34_xxxx and $0034_xxxx resolve to the same physical chip-RAM
longword.  Our bus passed the full 32-bit address through to
every decoder, which (combined with the read-side range check)
turned every "flagged" APtr into a garbage read of 0.

Fix in `rtl/m68k_bus.v`: at the unflatten step, mask each port's
incoming address to 24 bits (`addr[g] = {8'd0, addr_flat[32*g
+: 24]}`) before the bus's chipset / ROM / CIA / memory decoders
see it.  Required moving `IRQ_REG_ADDR` from $FFFF_FFFC (which
folds onto the last ROM longword and broke the ROM checksum) to
$00E9_FFFC (an empty autoconfig slot under KICKSTART_BOOT) and
to $00FF_FFFC (the masked form of $FFFF_FFFC) for tests, so
`t15_irq` still works with `$FFFC.W` (sign-extends to
$FFFFFFFC, masked to $00FFFFFC).

With the mask, the RTE that earlier landed in `$0C34_00F8`
garbage now lands in chip-RAM `$0034_00F8` where the first task's
code actually lives.  Boot runs through strap.lib and into a
long polling loop at $F83B8E-$F83D0C reading SERDATR and
checking CIA-A bits — looks like the "insert disk" prompt or a
keyboard wait.  146M retired in 500M cycles without crashing,
no DSKLEN write yet.

### Investigation: SERDATR poll loop is in strap.lib, not the keyboard wait
Tracing the hot loop at $F83B98-$F83BCA + $F83CF0-$F83D22 shows
this is strap.lib's serial-receive polling, called recursively
from the priv-vio handler at $F84020+ that fires when the boot
task hits `MOVE A0, USP` at $F80796 in user mode.

The flow:
1. Cold reset → vector init → exec init → MakeTask("boot")
2. RTE to boot task at $F800F8 with `SR = $00F8` (user mode)
3. Boot-task runs in user mode through $F800F8..$F80795
4. At $F80796 `MOVE A0, USP` triggers a priv-vio trap
5. Vector 8 handler chains into strap.lib at $F83BF6
6. Strap.lib's polling enters $F83CF0 + nested calls to
   $F83B66 / $F83B8E (SERDATR poll + 600K-iter delay)
7. ...

The polling at $F83CF0 reads SERDATR, checks D1 byte against
$1B and $AF, and loops indefinitely if neither matches.  Tried:
- SERDATR low byte = $1B: function exits → caller cold-reboots
- SERDATR low byte = $AF: same outcome
- Force-take BEQ at $F83CEC / BNE at $F83D0C: same outcome
- Run for 2 billion cycles: no DSKLEN, just spinning in poll

The polling appears to be checking for **serial-boot stimulus**
(a remote-debugger protocol byte) rather than disk insertion.
Without an actual serial peer feeding it, the loop is bounded
only by D7 (an outer counter we haven't traced), which appears
to be very large.

Even when the poll is force-exited, Kickstart's task-switch RTE
lands on `$0C34_00F8` which the 24-bit address mask resolves to
chip-RAM `$0034_00F8`.  But that location is empty (we never
populated it with task code), so execution walks through zeros
forever.

The real fix needs the *task code itself* to be at
`$003400F8`.  Real Kickstart's MakeTask populates this region
during AddTask, but our trace shows no writes to `$003400F8`.
Either Kickstart's AddTask is being skipped or the address is
computed differently.  Diagnosis needs:
- Trace AddTask invocations and their `task_PC` argument
- Identify which exec.library function builds the trackdisk
  task struct
- Verify the chain that feeds it the boot-task entry PC

**Confirmed AddTask never fires.**  Tracing the JT slot for AddTask
(`A6 - $11A` = `$4292`) shows zero visits in 50M cycles of boot.
Kickstart 1.3's first task is created via some other path -- a custom
exec-init sequence that doesn't go through the standard AddTask LVO.
The saved task context at `$5D62`-`$5D5E` is built by direct memory
writes from the $F8146C `MOVE.L (A7)+, -(A5)` loop, populating the
struct from values already on the supervisor stack.

The value-chain question becomes: which earlier function pushed
`$0C3400F8` onto the supervisor stack so $F8146C could pop it into
the task's PC slot?  That's the next thread to pull, requiring
trace-back through the strap.lib privilege-violation handler chain
to find the exception-frame source.

8 commits worth of CPU/bus fixes have removed every earlier crash;
the remaining work is Kickstart-side data-flow analysis to find where
that specific PC value gets onto the supervisor stack.

### Open: AllocMem starts allocating from low-mem $20+
Tracing shows AllocMem returning $20, $38, $50, $68, $80, $98,
$B0, $C8 — each 24 bytes apart, starting from $20.  $20 is
vector 8 (privilege violation).  Kickstart's per-resident node
fill then writes ln_Pri/ln_Name/etc. to those low-mem offsets,
clobbering Kickstart's own vector table.  Eventually the saved
A5 frame pointer in the MOVEM-spill area at mem[$3CA] also gets
overwritten (to $FFFB_0000 and later $FFFF_FFFB), so when the
InitResident driver's UNLK A5 runs at $F80E58 it pops A5 =
$FFFFFFFB and the following MOVEM/RTS land in $038F_xxxx garbage.

Root cause likely on our side: chip-RAM memory probe runs in
Kickstart's early init and uses our `mem[]` (which returns 0 for
unwritten cells and accepts every write).  The probe seems to be
reporting a MemHeader whose `mh_Lower` includes the vector
area.  Possibilities to investigate next: (a) Kickstart's probe
relies on a write-fails-at-top-of-chip behavior we don't emulate,
so it computes the wrong free bounds; (b) `mh_Lower` got
truncated somewhere in our exec init.

Under `+define+KICKSTART_BOOT` the sim now returns $FF for the
Zorro II autoconfig type byte ($E80000) so Kickstart doesn't
AddMemList() the phantom 8MB FastRAM card (we don't back the
assigned region with memory anyway).  Standalone autoconfig
test still passes because it doesn't define KICKSTART_BOOT.

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
