# Path to Running Amiga OS on This Core

Target: boot a real Kickstart ROM (1.3 or 3.1), reach the Workbench prompt,
and run trackdisk-style software. Final delivery: synthesise to FPGA with
the CPU clocked well above the original 7.09 MHz.

## What works today

- 68000 user-level instruction set: MOVE, ALU, shifts, bit ops, branches,
  MOVEM, LINK/UNLK, JSR/RTS, exceptions (TRAP, TRAPV, CHK, illegal,
  divide-by-zero, priv-violation, IRQ dispatch via the bus's `irq_level`
  sideband).
- All standard 68000 addressing modes including `d8(An,Xn)` and PC-relative.
- Mem-to-mem MOVE with `(An)+,(An)+` and `-(An),-(An)` (recent fix).
- D-cache + I-cache with snooping; bus arbiter with N CPU + chipset masters.
- Chipset: Blitter (copy, line, fill, all logic ops, descending, BZERO,
  INT pulse), Copper (MOVE, WAIT-RASTER, WAIT-BLITTER, SKIP, two lists),
  Denise (1–8 bitplanes, HAM6/HAM8, EHB, DPF, 1bpp + 2bpp sprites,
  attached pairs, DIW, BPLCON1 scroll, BPLCON2 priority, hires), Paula
  (4-channel audio).
- 55-test regression suite covering CPU, chipset, addressing modes,
  stack frames, MOVEM patterns, traps, SR/CCR.

## What's missing for Kickstart boot

The minimum viable set, in dependency order:

### 1. Memory map rework (mostly bus rewiring, ~1 day)

Real Amiga layout:

| Range                         | Purpose                                |
|-------------------------------|----------------------------------------|
| `$00_0000` – `$1F_FFFF`        | Chip RAM (2 MB, accessible to chipset) |
| `$20_0000` – `$9F_FFFF`        | Fast RAM / Zorro II                    |
| `$BF_D000` – `$BF_DFFF`        | CIA-B (even byte addresses)            |
| `$BF_E001` – `$BF_EFFF`        | CIA-A (odd byte addresses)             |
| `$DF_F000` – `$DF_FFFF`        | Custom chip registers (mirror)         |
| `$E8_0000` – `$EF_FFFF`        | Zorro II autoconfig                    |
| `$F8_0000` – `$FF_FFFF`        | Kickstart ROM (512 KB)                 |

The current bus puts custom registers at `$00FE_0000` and main RAM at
`$00_0000+`. We can keep our internal layout but ALSO mirror the Amiga
addresses, OR just relocate to Amiga-canonical addresses. Recommend the
latter so Kickstart's hard-coded pointers work as-is.

**Tasks:**
- Move BLT/COP/DEN/PAU register windows to `$DF_F000+` aliases.
- Add Kickstart ROM region at `$F8_0000–$FF_FFFF`. Boot vectors at
  `$0`/`$4` initially read from ROM via OVL remap (CIA-A PRA bit 0).
- Bus passthrough vs cache: ROM is read-only, cache it freely; chip
  registers must stay uncached.

### 2. CIA chips (2 × 8520 model, ~1–2 days)

The two CIAs are the OS's contact with the world: timers for task
scheduling, keyboard scancodes, mouse buttons, disk, serial, parallel,
OVL/LED control.

Each CIA has 16 registers (8-bit), accessed at addresses 256 bytes apart
in real hardware. Critical registers for boot:

| CIA-A                          | CIA-B                          |
|--------------------------------|--------------------------------|
| `PRA`: kbd, mouse btns, OVL    | `PRA`: serial, parallel, disk  |
| `TA/TB`: timers                | `TA/TB`: timers                |
| `TOD`: time-of-day             | `TOD`: time-of-day             |
| `ICR`: interrupt control       | `ICR`: interrupt control       |
| `CRA/CRB`: timer control       | `CRA/CRB`: timer control       |

**Bare-minimum CIA model:**
- 16 byte-sized registers per CIA.
- Two 16-bit decrementing timers with reload, one-shot/continuous modes,
  IRQ-on-underflow.
- 24-bit TOD counter (ticks at 50/60 Hz from chipset vsync).
- ICR with mask/source register pair.
- Output interrupt line tied to the 68k's IRQ controller (CIA-A → INT2,
  CIA-B → INT6 in real Amiga; we'll mirror).

Defer for v2: serial UART shift register, parallel handshake, the SDR
register, full TOD alarm.

**File:** new `rtl/chipset/cia.v` instantiated twice. Each with its own
slave window in the bus.

### 3. Interrupt controller (~0.5 days)

Currently we have a single `irq_level` sideband. Real Amiga has Paula's
`INTREQ` (pending) and `INTENA` (enable) registers at `$DFF09A/9C`. The
chipset masters set their bits; the controller ORs enabled bits into a
3-bit level for the 68k.

| Bit | Source                  | Vector                |
|-----|-------------------------|-----------------------|
| 0   | TBE (serial)            | INT1                  |
| 1   | DSKBLK (disk block)     | INT1                  |
| 2   | SOFT                    | INT1                  |
| 3   | CIA-A                   | INT2                  |
| 4   | COPER (Copper)          | INT3                  |
| 5   | VERTB (vblank)          | INT3                  |
| 6   | BLIT (blitter done)     | INT3                  |
| 7   | AUD0                    | INT4                  |
| 8   | AUD1                    | INT4                  |
| 9   | AUD2                    | INT4                  |
| 10  | AUD3                    | INT4                  |
| 11  | RBF (serial)            | INT5                  |
| 12  | DSKSYN                  | INT5                  |
| 13  | EXTER (CIA-B + Zorro)   | INT6                  |
| 14  | INTEN (master enable)   | —                     |
| 15  | SET / CLR control       | —                     |

**Tasks:**
- Add `INTREQ`/`INTENA` regs to Paula (or a new `int.v` module).
- Route blitter `int_o`, Copper `int` (need to add), Denise vblank,
  Paula audio interrupts, CIA-A/CIA-B IRQ outputs into INTREQ.
- Combine into `irq_level` driven onto the bus.
- The CPU's IRQ vector dispatch already handles autovectored interrupts.

### 4. ROM loader (~half day)

Need to load a Kickstart `.rom` blob into our memory at the right
address before reset. Two options:

1. Convert the .rom file to .hex and prepend zeros up to $F80000. Big.
2. Make the bus's memory init use TWO files: a small RAM image and the
   ROM, with `$readmemh` reading each into the right region. Cleaner.

Recommend option 2. Add `MEM_HEXFILE_ROM` parameter loaded into the
$F80000+ region.

**OVL handling:** at reset, the vectors at `$0`/`$4` actually read from
ROM. This is done by CIA-A PRA bit 0 (OVL=1 after reset). The bus
arbiter, when OVL=1, redirects reads in the `$0_0000–$0_7FFFF` range to
`$F8_0000–$FF_FFFF`. The bootstrap sets OVL=0 once vectors are copied to
RAM.

### 5. CPU compatibility gaps (~1–2 days)

The remaining 68000 instruction-set gaps after this session:

| Missing                                       | Use case                          |
|-----------------------------------------------|-----------------------------------|
| ABCD / SBCD / NBCD                            | BCD arithmetic (rare)             |
| ADDX / SUBX / NEGX                            | Multi-precision math              |
| Memory-destination BSET/BCLR/BCHG/BTST        | Chipset register bit-twiddling    |
| Bus error (group 0 exception, vector 2)       | Unmapped memory accesses          |
| Address error (group 0, vector 3)             | Misaligned 32-bit reads           |
| RESET instruction                             | Sometimes used by kernel          |
| MOVEP                                         | 6800-compatibility (rare)         |

**Most-important for OS:** mem-destination bit ops, ADDX/SUBX, and bus
error. Address error is debatable — we currently return the
cache-aligned word for a misaligned access, which is enough to keep
software running unless it intentionally tests the trap.

### 6. Display output for FPGA (~1 day for HDMI/VGA)

We currently render to a chunky framebuffer that the testbench scans via
`fb_peek`. On FPGA:

- Add a `rgb_out` module that scans the chunky FB at HDMI/VGA timing.
- HDMI: TMDS encoder, 25 MHz pixel clock for 640×480@60.
- For Amiga's native 320×200/640×400, scale to 640×480 or 720×480.

The chunky FB → video conversion is a much simpler module than the
chipset itself, since all the work is already in Denise.

### 7. Input devices (~1 day for testbench, ~3 days for FPGA)

- Keyboard: USB HID → CIA-A serial shift register protocol.
- Mouse: USB HID → JOY0DAT register + CIA button bits.

For the simulator: stdin → CIA-A scancodes.

### 8. Floppy / disk (~1 day for stubs, multi-day for proper)

For initial boot from a self-bootable in-memory image:

- Map a `.adf` file into memory at e.g. `$30_0000`.
- Stub `trackdisk.device` to read from this region.

For real floppy emulation: Paula's disk controller plus the MFM
encoding. Significant complexity — defer.

### 9. Clock-domain crossing for FPGA (~2 days)

The CPU can run as fast as the FPGA fabric allows. The chipset must
stay locked to 7.09 MHz (PAL) or 7.16 MHz (NTSC) for timing-sensitive
software (Copper waits, sprite positions, raster effects).

**Approach:**
- `clk_cpu`: PLL-generated 50–100 MHz (or higher).
- `clk_chip`: 7.09 MHz, derived from the same PLL.
- All chipset modules clocked on `clk_chip`. The CPU's master bus port
  on `clk_cpu`.
- Bus arbiter has clock-crossing FIFOs at the chipset/CPU boundary.
- The L1 caches absorb most of the rate mismatch — CPU runs full-speed
  on cache hits, only blocks on miss.

**Compatibility:** Kickstart and 99% of Amiga software is robust to a
faster CPU as long as the chipset registers stay at 7.09 MHz timing.
Demos that count CPU cycles for raster effects will break — that's
fundamental to "fast 68k" Amigas like the A4000/040, so users expect it.

### 10. Zorro II/III (~1 day for stub, much longer for real autoconfig)

For first boot we don't need real Zorro. Just give the CPU a chunk of
"fast RAM" in the `$20_0000+` range that Kickstart will auto-detect via
the AddMemList scan. Defer full autoconfig.

---

## Phased plan

**Phase A — bring-up (target: Kickstart "hand" prompt)**

1. Memory-map rework + ROM loader.
2. CIA stub (timers + interrupt mask; defer keyboard).
3. Interrupt controller w/ vblank from Denise.
4. ADDX/SUBX + mem-dst bit ops.
5. Load Kickstart 1.3 ROM. Boot. Reach the floppy prompt.

**Phase B — interaction (target: type at Workbench)**

6. Keyboard via CIA-A serial.
7. Mouse via CIA-A + JOY0DAT.
8. Display output via SDL (already done) / HDMI for FPGA.
9. Boot from in-memory ADF.

**Phase C — FPGA bring-up**

10. Clock-domain split.
11. HDMI/VGA video output.
12. USB HID input bridge.
13. SD card → floppy/HDD emulation.
14. Optimise CPU for higher fmax (pipeline tweaks, register renaming).

**Phase D — polish**

15. Zorro II autoconfig (for proper fast-RAM detection).
16. Full Paula disk controller (real ADF boot).
17. Audio output to FPGA's onboard DAC/HDMI.
18. Bus-error / address-error exception support for cleaner crash
    handling.

---

## Test/diagnostics strategy

- Keep the existing 55-test regression as the baseline. Every new
  feature lands with its own test.
- Add a `bench-amiga` target that does a partial ROM-execute trace:
  load Kickstart, run for N cycles, dump observed instruction
  addresses and detect divergence from a reference 68k emulator
  (Musashi, WinUAE) for the same input.
- Snapshotting: when Kickstart hangs, dump CPU regs + relevant chip
  regs + last 256 PCs to a file for postmortem.
