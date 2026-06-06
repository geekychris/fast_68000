# Boot Pipeline + Porting Guide

How a Verilog-simulated 68000 boots Amiga Kickstart 1.3 from an ADF
floppy image, how to extend the boot path to other media (hard disks,
ROMdisks, network), and what's involved in moving the design from a
Verilator host onto a Gowin FPGA — including using their HyperRAM IP
for chip + slow RAM.

This is the "tour" doc. For low-level bug-hunting context see
`WB13_DEBUG_JOURNAL.md`.

---

## 1. The 30-second pitch

Boot a real, byte-original 1987 Amiga Kickstart 1.3 ROM, mounted to
real, byte-original Workbench 1.3 floppy bits (with a custom
Startup-Sequence and a 1986 Boing! binary injected via an AmigaDOS
filesystem tool), through our 68000 + chipset + Paula DMA, with **no
shortcuts**:

```
$ make real-boing-disk     # 1 produce a bootable ADF with boing
$ make demo-real-boing     # 2 boot it on the Verilator sim
```

There's no fake "Loader" task; no host file-injection at boot. The
sim's `trackdisk.device` calls into Paula's DMA the same way real
Agnus + Paula chips do, reads MFM-encoded floppy bits the same way a
real Amiga reads `$4489` sync words, blitter-decodes the odd/even
half-words, and hands the resulting `LoadSeg` data to `dos.library`
running unmodified in the K1.3 ROM. The only "host" interaction is
the SDL keyboard/mouse stream (or `MOUSE_AUTO_CLICK` env var) that
emulates a typing user.

This fidelity is what makes the project useful as an RTL fuzzer for
Amiga-class designs: any RTL bug shows up as a real-Amiga-software
malfunction (and we can diff against FS-UAE running the same disk
image to bisect).

---

## 2. From `make` to running code

### 2.1 Build-time pipeline

```
  kickstart/kick_13.bin   ← unmodified 256 KB K1.3 ROM (exec 34.2, 1987)
            │
            ▼
  tools/bin2rom.py        ← 4-bytes-per-line $readmemh format
            │
            ▼
  build_kick_boot/rom.hex
```

```
  kickstart/wb13.adf      ← 901,120 B OFS floppy image
            │ ...inject S/Startup-Sequence + C/boing + C/boing.samples
            │ ...via amitools xdftool
            ▼
  kickstart/boing_disk.adf
            │
            ▼
  tools/adf2mfm.py        ← MFM-encode with $4489 sync, odd/even halves,
            │              clock-bit fill
            ▼
  build_kick_boot/disk.hex   (~1.9 MB raw disk stream as $readmemh)
```

`tools/asm68k.py tests/t63_boot_rom.s` produces a tiny
`program.hex` that fills the chip-RAM `mem[]` array with what we'd see
just after reset — almost entirely zeros for the Kickstart path; the
real program loads from ROM.

`make build` runs Verilator on `rtl/m68k_top.v` plus the chipset
modules, with `+define+KICKSTART_BOOT` and (optionally)
`+define+KICKSTART_FAST_BOOT` to skip the LED-blink delay loops.
Output: `build_kick_boot/Vm68k_top`.

### 2.2 Reset

Power-on (cycle 0) of the simulated 68000 sees `OVL=1`: the bus
arbiter maps the lower 256 KB of address space to ROM contents, so a
fetch from `$00000000`–`$00000007` returns the K1.3 reset SP/PC pair
(`$00040000` / `$00FC00D2`). The CPU latches these into A7 and PC,
exits reset, and starts executing in ROM.

Within ~50 K cycles the ROM has written `$0000` to `$BFE001` (CIA-A
PRA bit 0 = OVL clear), so the bus arbiter switches the lower 256 KB
back to chip RAM. From this point onwards reads from `$0–$3FF` return
the live exception-vector table, exec writes the cold-capture vector
at `$04`, and we look like a standard Amiga 500 from K1.3's POV.

### 2.3 RAM layout

```
$000000  - $07FFFF   CHIP RAM (512 KB, A500 trapdoor-less)
$0BE000  - $0BEFFF   CIA-A (chip-side mirror)
$0BFE00  - $0BFEFF   CIA-A (real address)
$0BFD00  - $0BFDFF   CIA-B (real address)
$0BFD000 - $0BFDFFF  CIA-B (chip-side mirror)
$0C00000 - $0C7FFFF  SLOW (trapdoor) RAM, 512 KB — emulates the
                     A500 trapdoor expansion. K1.3 calls this "FAST"
                     in its memory list, technically misleading on
                     real silicon but software-equivalent.
$0DFF000 - $0DFFFFF  Chipset registers (Agnus / Paula / Denise /
                     Copper / Blitter / Bitplane DMA, etc).
$0F80000 - $0FFFFFF  Kickstart ROM (256 KB).
$00400000- variable  DISK image (read-only, MFM bitstream).
$00FE8000- $0FE800F  Block-device control window (bonus, see §3.2).
```

This is the canonical Amiga 500 map plus three sim-only additions:

1. **Disk image at `$00400000`**: holds the `adf2mfm.py` output.
   Paula reads from here when `DSKLEN` DMA starts. There is no
   physical disk drive; we don't model spinning, head stepping
   timing, write protect, or motor on/off. K1.3 trackdisk treats this
   memory as if it were the read FIFO downstream of a real Paula
   `DSKBYT` register.

2. **Block-device control at `$00FE8000`**: a four-register pseudo-
   chipset slot that's a fast "skip the MFM round-trip and just bulk-
   copy raw sectors into chip RAM" shortcut. This is **not** used by
   the K1.3 boot path — it's reserved for non-Kickstart demos and the
   future hard-disk extension (§4.2).

3. **`+define+KICKSTART_FAST_BOOT`**: a build flag that patches the
   K1.3 ROM's LED-blink and "wait 60 frames before reading" timing
   loops to single iterations. Cosmetic only — saves ~10 seconds of
   wall clock during regression.

### 2.4 Bootblock load (what trackdisk does)

Inside K1.3, the boot sequence after exec-init is:

```
ROM init → trackdisk.device init → DSKLEN/DSKSYNC programmed →
Paula DMA: 11 sectors of cyl 0 → MFM-decode blit → bootblock magic
check → JSR into bootblock code at chip $0C00.
```

What our hardware does for that path:

```
trackdisk writes DSKPTH/DSKPTL = $C18 (or whatever buffer)
trackdisk writes DSKLEN = $9800 (0x4800 words × 2 = read enable)
Paula's disk DMA engine (rtl/chipset/paula_disk.v) is triggered,
walks the bitstream at $00400000+, watches for $4489 $4489 sync
pattern, then DMAs the following raw bytes into chip RAM.
On DMA completion, Paula raises INTREQ bit 1 (DSKBLK),
trackdisk's IRQ handler queues a blitter pass that runs the standard
Amiga MFM odd/even-half re-interleave + sector-checksum verify.
Bootblock magic check + Disk*Valid latch — boot continues into chip RAM.
```

Every step happens in the K1.3 ROM, unmodified. The only "us" code
is the chipset RTL emulating Paula's DMA + the MFM bitstream we
loaded at `$00400000`.

---

## 3. The ADF translation, in detail

### 3.1 Why MFM?

Two reasons. **Fidelity**: a real Amiga floppy delivers an MFM bit
stream, and `trackdisk.device` does its own bit-level sync search,
DMA, and per-sector checksum verification. If the sim hands K1.3
already-decoded sector bytes, we'd skip the blitter MFM decode (used
constantly by every disk read), the DSKSYNC IRQ path (catches DMA
overruns), and the per-sector header checksum (catches single-bit
flips that pass sync but reject sector data). Three classes of bug
would silently never fire in our sim. With full MFM, every read
exercises those paths.

**Cross-checking**: the bitstream we hand K1.3 is *literally* the
same bitstream FS-UAE hands its 68000 emulator and that minimig hands
its FPGA. Two RTL bugs that we caught the long way through cosim
diff (the `BLTAMOD/BLTCMOD` longword split, the
`BLTAFWM/BLTALWM` mask straddling, etc.) would have been invisible
if we'd shortcut the MFM. They showed up because the bug landed in
the actual disk-validation blits that 1.3 trackdisk runs.

### 3.2 The ADF → MFM map

An ADF (Amiga Disk File) is the raw 880 KB image of one floppy: 80
cylinders × 2 sides × 11 sectors × 512 B. AmigaDOS calls each
512-byte block a "sector"; on disk, each sector is wrapped in a
self-describing header:

```
Per-sector disk layout (raw disk bytes, MSB-first):
  2 sync gap bytes ($00 each, MFM-expand to $AAAA)
  2 sync words ($4489 $4489 — written directly, NOT MFM-encoded)
  4 bytes  header info  (format / track / sector / sectors-to-end)
  16 bytes sector label (zero for OS sectors)
  4 bytes  header checksum
  4 bytes  data   checksum
  512 bytes sector data
```

The 4-byte info, 16-byte label, both checksums, and 512-byte data
sections are stored as "odd/even halves":

```
For each 32-bit longword L,
  odd_half  = (L >> 1) & 0x55555555     # bits at positions L[1,3,...,31]
                                        # placed at positions 0, 2, ..., 30
  even_half =  L      & 0x55555555      # bits at positions L[0,2,...,30]
```

This is what Paula's bit-shifter produces in hardware when it MFM-
encodes one byte: the data bit at even positions, a clock bit
between each. K1.3's MFM-decode blit extracts only the data bits
(the `$5555_5555 ∧ ...` combine) and discards the clock bits.

For a N-longword buffer, the disk layout is "all odd halves first,
then all even halves", for total 8N raw bytes = 2N decoded bytes.

`tools/adf2mfm.py` walks the ADF, emits per-track headers in the
above format, fills the clock bits according to the MFM rule
`clock = NOT (prev_data | curr_data)` (cosmetic — K1.3 ignores
them, but matters for hardware-faithful read-back diff), and writes
the result as `$readmemh`-style hex.

The end product is 1,917,440 bytes (160 tracks × 11 sectors ×
1,088 raw bytes/sector + gap and overhead). That's exactly what a
real Amiga floppy delivers per side.

### 3.3 The OFS filesystem manipulation

`amitools` (a Python package — `pip install amitools`) provides
`xdftool`, an OFS/FFS-aware tool that can read, write, delete, and
inject files into an ADF without unpacking it to host filesystem
first. We use it to:

```
make real-boing-disk:
  cp kickstart/wb13.adf  kickstart/boing_disk.adf
  delete S/Startup-Sequence    ← replace the WB1.3 default
  write  /tmp/boing_startup.txt → S/Startup-Sequence
  delete 17 unused WB tools     ← make room for boing.samples
  write  ~/Downloads/.../boing! → C/boing
  write  ~/Downloads/.../boing.samples → C/boing.samples
```

The disk metadata (block bitmap, dir entries, file system header)
gets updated correctly because we let amitools do it. Our sim then
MFM-encodes the result, K1.3's OFS reads it without any custom
patch, and `Initial CLI` executes `boing` exactly as it would on a
real Amiga.

---

## 4. Extending the boot media

### 4.1 Adding a second floppy (DF1:)

K1.3 supports up to 4 drives. The bus already exposes `DSKDRIVE0`-
selected DMA via the `DSEL` line. Adding DF1:

1. Wire a second `paula_disk.v` instance keyed off `DSEL=1`.
2. Add a second `$00500000` disk image slot in `m68k_bus.v` (mirroring
   the existing `DISK_BASE`).
3. Extend `Makefile`'s `test-kickstart-boot` rule to accept
   `ADFFILE_DF1=…` and run `adf2mfm.py` twice.
4. The K1.3 ROM does the rest — `INFO DF1:` will autodetect.

Estimated effort: a half day of RTL + Makefile, plus one new test
that boots `DF0:Workbench1.3` and reads files off `DF1:Workbench1.3-`
`extras`.

### 4.2 Hard-disk image (the path of least resistance)

There are two viable styles:

**Style A: "RDB" Rigid Disk Block** — the real-Amiga way.
On a real Amiga, the hard disk has a Rigid Disk Block (RDB) header at
LBA 0–15 that lists FileSystem partitions. K1.3's `expansion.library`
detects the RDB at boot, looks up an RDSK partition with the
`AUTOMOUNT` flag, and mounts it via `trackdisk-compatible.device` —
typically `scsi.device`. Implementing this means:

1. **Hardware**: add a SCSI-style block-device master to `rtl/m68k_bus.v`.
   We already have most of this — `BLK_BASE = $00FE8000` exposes
   four registers: source sector, dest mem byte addr, sector count,
   command. The existing `paula_disk.v` doesn't use it; the block
   slot was added for non-Kickstart demos. To make it boot-from-HD,
   it needs:
   - A read-after-DMA IRQ that real Amiga drivers expect (cheap).
   - A "device-type" register read (1 byte) returning $00 for hard
     disk, $80 for floppy.
2. **ROM extension**: K1.3 doesn't include a hard-disk driver. You
   need a small Module file that's either:
   - Burned into a Kickstart ROM extension at `$F00000-$F80000` (the
     `KSPATCH`/2MB-ROM slot), OR
   - Loaded from a 'boot ROM' file (an option ROM in a real Amiga
     expansion slot), OR
   - Run from RAM after a tiny ROM-boot stub that reads `expansion.rom`
     from offset 0 of the HD.
3. **Software**: ship a HardFile RDB at LBA 0 + an OFS or FFS
   partition. `amitools` has `xdfblk` for RDB manipulation.

Estimated effort: 1–2 days for the hardware (RDB reader fits in
~200 lines of Verilog using the existing `blk_*` infrastructure),
1 day to find or write a minimal `scsi.device` substitute that K1.3's
expansion.library can probe.

**Style B: "ROMdisk" — much simpler**.
Inject a fake floppy in chip RAM at a fixed location, mount it as
`RAM:` via a custom bootblock. We already do something like this for
`tests/t63_boot_rom.s` (which loads a small program at chip $400 and
runs it). To boot **AmigaDOS** from a chip-RAM image:

1. Generate an ADF with `xdftool`, populate it with your binary +
   Startup-Sequence as today.
2. At build time, load the ADF into chip RAM at `$50000` via the
   `MEM_HEXFILE` slot (we already do this for the boing demo's
   "load samples into chip RAM" path).
3. Patch the K1.3 boot path to skip floppy DMA and read sector
   directly from chip RAM. Easiest: provide an option-ROM at
   `$F00000` whose autoconfig handshake registers a `ramdrive.device`
   with the BootMenu's resident-module list.

Style B avoids touching trackdisk at all but only works because the
sim can pre-populate RAM at build time. It's not portable to real
hardware (no boot ROM on FPGA without flash).

### 4.3 Network boot (TFTP)

Long shot but interesting: replace the `paula_disk.v` master with a
TCP/UDP socket that the host opens at sim launch. Boot stub in ROM
extension pulls `boot.adf` over TFTP, drops it in chip RAM, and runs
the Style B path above. Useful for development workflows where the
ADF changes every minute and you don't want to rebuild Verilator
each time.

### 4.4 The pragmatic shortlist

If you want to expand what runs:

| You want…                      | Path of least resistance |
|--------------------------------|--------------------------|
| Different K1.3 demo            | Replace `BOING_SRC` in Makefile, re-run `make real-boing-disk` |
| K1.3 boot from RAM (no MFM)    | Style B ROMdisk |
| K2.0 boot                      | Replace `kick_13.bin` with `kick_205.bin`; chipset RTL extensions may be needed (ECS) |
| WB1.3 desktop with multiple WB apps | Bigger ADF (multi-floppy or HD style A) |
| Real AmigaOS 3.1               | HD style A + scsi.device + 2 MB ROM slot |

---

## 5. Porting to a Gowin FPGA

### 5.1 What lives where on Verilator vs. FPGA

| Component               | Verilator                | Gowin FPGA target                |
|-------------------------|--------------------------|----------------------------------|
| CPU + caches            | `rtl/m68k_*.v`           | Same RTL, synthesizes ~9–12 K LE |
| Chipset (Agnus/Paula/Denise/Copper/Blitter) | `rtl/chipset/*.v` | Same RTL, synthesizes ~6–8 K LE |
| Chip + slow RAM          | `reg [31:0] mem[…]`      | External HyperRAM via Gowin IP   |
| ROM                      | `reg [31:0] rom[…]`      | SPI flash → on-chip BRAM         |
| Disk image (MFM stream)  | `reg [31:0] disk[…]`     | SD-card-backed SPI memory        |
| Framebuffer              | `fb_peek_*` → SDL window | Gowin VGA / HDMI core + DDR FIFO |
| Keyboard / mouse         | SDL events               | PS/2 or USB HID core             |

The CPU and chipset RTL are **already FPGA-targeted**. They use
no `$readmemh` outside the optional `MEM_HEXFILE` boot-image
slot (which the FPGA target ignores), no `$display`-as-stimulus,
no behavioral primitives. Everything that's instrumentation lives
under `+define+CHIP_MC_WATCH`-style guards, so a synthesizable build
sets those undefined.

The heavy lifting for an FPGA port is the **storage** problem:
- ROM (256 KB) fits trivially in any Gowin part's BRAM.
- Chip RAM (512 KB) does **not** fit in BRAM of most Gowin parts.
  The Tang Primer 25K has ~144 KB BRAM total, the Tang Mega 138K has
  ~408 KB — neither covers chip + slow + framebuffer.
- Slow RAM (512 KB) same story.
- Framebuffer (640×400×4bpp = ~125 KB) again.

You need external memory. The Gowin
[HyperRAM IP](https://www.gowinsemi.com/en/support/ip_detail/128/)
is the right primitive for this. Cypress / Infineon HyperRAM is a
DDR-style 8-bit serial interface that gives you 8 MB or 16 MB in a
small package, ~100 MB/s sustained bandwidth, ~30 ns first-word
latency.

### 5.2 HyperRAM integration plan

Today our `m68k_bus.v` exposes a 32-bit-wide synchronous SRAM
abstraction: `addr, wdata, be, we, req → ack, rdata`, single-cycle
latency. To swap that for HyperRAM:

1. **Add an arbiter slave**: `hyperram_slave.v` wraps the Gowin IP's
   `cmd / addr / data / valid / ready` interface and accepts the
   same `req/we/addr/wdata/be → rdata/ack` shape the rest of our
   buses expect. The Gowin IP handles refresh, command framing, and
   the HyperBus protocol.
2. **Memory-map carving**: in `m68k_bus.v`, where today the lookup
   table decodes `addr ∈ [0, $80000) → mem[]`, instead it routes
   those reads/writes to the `hyperram_slave`. Same for slow RAM
   `addr ∈ [$C00000, $C80000)`.
3. **Latency tolerance**: HyperRAM round-trip is ~5–10 cycles at
   100 MHz, not 1. Our `m68k_cache.v` already tolerates multi-cycle
   bus latency (`S_BUS_WAIT` state pumps until `bus_resp_valid`).
   The blitter, copper, and disk DMA masters all have the same
   `req/grant/resp_valid` handshake and tolerate latency — they were
   written to model real Agnus DMA arbitration where each access is
   a horizontal-blank slot. The biggest concern is **bitplane DMA
   throughput**: at 7.16 MHz pixel clock with 4 bitplanes, Agnus
   wants ~1.8 MB/s sustained from chip RAM. HyperRAM gives 30+ MB/s
   peak so even with cache misses we'd be fine, but the bandwidth
   has to be **shared** with CPU + blitter + copper. The FPGA
   arbiter wants a slot scheduler matching Agnus's per-line plan.
4. **Power-on**: the Gowin HyperRAM IP needs a config phase before
   first access. Drive the CPU reset asserted until the IP's
   `init_done` goes high.
5. **Cold-boot ROM load**: on FPGA, copy ROM from SPI flash to
   on-chip BRAM at startup, or memory-map it directly via Gowin's
   SPI flash continuous-read mode. The K1.3 ROM is only read, never
   written by software, so even SPI's ~50 ns access is acceptable
   (with a small read cache).

### 5.3 Disk image on FPGA

On Verilator the disk MFM stream sits in a 1.9 MB `reg [31:0] disk[]`
array — fine for sim, way too big for any Gowin BRAM. Two practical
options:

1. **SD card via SPI**. SDC card → SPI master → small FIFO →
   Paula DMA. Each track is ~12 KB; you can fetch a track on every
   step-and-seek event with a 50 ms budget that real floppies needed
   anyway. K1.3 will not notice.

2. **HyperRAM region**. Allocate a 2 MB window of HyperRAM for the
   "disk image", load it at FPGA bringup from SD card, and have
   Paula DMA point at it. Trades SD card complexity for HyperRAM
   bandwidth contention with chip RAM.

The Tang Mega 138K has both an SD slot and HyperRAM headers on the
dev board — option 2 is cleanest.

### 5.4 Output: video and audio

The Verilator framebuffer `fb_peek_*` interface (in `m68k_top.v`) is
a synchronous read of the current Denise pixel state, polled by SDL
each frame. On FPGA, replace SDL with a VGA / HDMI core:

```
Denise + Copper → pixel stream → Gowin VGA/HDMI IP → DVI connector
```

Gowin ships a DVI IP that takes 8-bit RGB and produces a TMDS stream;
the 480×400 Amiga PAL/NTSC resolution upscales cleanly to 720p with
a 2× line repeat.

Paula audio: today the sim has 4 audio DMA channels but only writes
sample values to a register that no host listens to. On FPGA, wire
each channel into a Gowin I²S DAC IP or a simple PWM output through
an analog filter.

### 5.5 Keyboard + mouse

The chipset already implements CIA-A keyboard handshake (`SP`/`CNT`
clocking, FLAG IRQ, scan-code injection). On Verilator the host SDL
event loop drives `ext_kbd_wr`/`ext_kbd_byte`; on FPGA you replace
that with a PS/2 receiver + small state machine that translates
PS/2 scan codes to Amiga rawkey codes and pumps them in via the same
`ext_kbd_wr` signal.

Mouse: CIA-A POTGOR + JOY0DAT already model the Amiga 2-axis quad
counter. PS/2 mouse → quadrature generator → same `mouse_x_count` /
`mouse_y_count` / `mouse_btn_*` wires.

### 5.6 Realistic Gowin targets

| Part | LUTs / LEs | BRAM | Notes |
|------|------------|------|-------|
| Tang Nano 9K (GW1NR-9) | 8.6 K | 26 KB | Probably too small even with HyperRAM — barely fits CPU |
| Tang Primer 25K (GW2A-LV18) | 23 K | 72 KB | Fits CPU + caches comfortably; chipset is tight |
| Tang Mega 138K (GW5AST-138) | 138 K | 408 KB | Comfortable. ROM + small caches all on-chip; HyperRAM for everything else. Has the right peripheral mix (HDMI, SD, USB, HyperRAM headers). |

The Tang Mega 138K is the right target. Comparable to a high-end
Amiga 1200's main FPGA cost on minimig-style hobby projects.

### 5.7 Build flow for Gowin

Today the Verilator build pipeline is `make build BUILD=build_…`.
For Gowin you'd add a parallel toolchain:

1. `make synth-gowin` — runs Gowin EDA / yosys synthesis on `rtl/`,
   produces `.fs` bitstream.
2. Constraints file pins HyperRAM, SD, HDMI, PS/2, push buttons.
3. `make program-gowin` — uses `openFPGALoader` to flash the
   bitstream to the Tang Mega.

Both Yosys + nextpnr-gowin (open-source) and Gowin's own EDA suite
target these parts. Yosys + nextpnr is the lower-friction path for
hobby work; Gowin EDA is mandatory if you want to use their HyperRAM
or DDR IP cores (they ship as encrypted blobs that only the vendor
toolchain accepts).

### 5.8 What's NOT ready to port today

- **CDTV/CD32 IDE controllers**: not modeled.
- **AGA chipset registers** (A1200/A4000): not modeled. K1.3 boot
  doesn't need them, but K3.1+ does.
- **Mouse acceleration timing**: our quadrature generator emits
  step pulses ratiometrically; real Amiga drivers infer velocity
  from inter-step timing. Acceptable for boot but not for slick
  desktop interaction.
- **Real-time-clock (battery-backed CIA-B TOD)**: TOD register
  semantics are emulated but the FPGA target needs a host RTC chip
  (or NTP at startup) to seed it.
- **Floppy write**: disk image is read-only. Adding write would
  require a real SD-card or HyperRAM-backed image and a way for
  K1.3 to communicate "the disk is now dirty, please flush" — which
  on a Verilator sim is fine to dump to a file at exit, on FPGA
  requires power-fail-safe block-aligned writes through Paula's DMA
  going backwards. Doable; not done.

---

## 6. Pointers into the code

The map from this doc to source:

| Concept                         | File / function |
|---------------------------------|-----------------|
| Reset + OVL                     | `rtl/m68k_bus.v` § `OVL_RESET`, `ovl_clr` |
| ROM region map                  | `rtl/m68k_bus.v` § `ROM_END`, `rom[…]` |
| Chip RAM                        | `rtl/m68k_bus.v` § `mem[…]` |
| Slow RAM                        | `rtl/m68k_bus.v` § `SLOW_BASE`, `slow[…]` |
| Bus arbiter                     | `rtl/m68k_bus.v` § `winner`, `p_req/we/addr` |
| Paula disk DMA                  | `rtl/chipset/paula_disk.v` |
| ADF→MFM encoder                 | `tools/adf2mfm.py` |
| ROM `.bin`→`.hex` translator    | `tools/bin2rom.py` |
| Boot test launcher              | `Makefile` § `test-kickstart-boot:` |
| Boing demo wrapper              | `Makefile` § `real-boing-disk:`, `demo-real-boing:` |
| Hello (minimal) baseline        | `Makefile` § `hello-disk:`, `demo-hello:` |
| Screen renderer                 | `tools/render_k13_screen.py` |
| Chipset shadow regs for render  | `rtl/m68k_top.v` § `fb_peek_*` |
| Bus-side watchpoints            | `rtl/m68k_bus.v` § `hw_watch` instances |
| CPU-side instrumented probes    | `rtl/m68k_core.v` § `$display([…])` blocks |
| Sim harness                     | `tb/sim_main.cpp` |
| Verilator build rule            | `Makefile` § `build:` |

---

## 7. Where this is most useful

Three audiences:

**Amiga-curious software people**: this is the most accurate Amiga
emulator that lives in a single Git checkout (no encrypted ROM
blobs, no closed-source chipset libraries; you supply K1.3 ROM and
WB1.3 ADF). Boot the real bits, watch them run instruction-by-
instruction, single-step through `trackdisk.device` in the included
gdbserver, snapshot RAM at any retired count, render the framebuffer
to PNG.

**RTL designers chasing chipset corner cases**: every disk read, every
blit, every Copper restart, every Agnus DMA slot is observable. We
caught about 15 RTL bugs that minimig + WinUAE missed by running the
same K1.3 + WB1.3 boot side-by-side and diffing memory snapshots.
See `WB13_DEBUG_JOURNAL.md`.

**FPGA-on-a-budget Amiga rebuilders**: the design is portable to
sub-$200 Gowin dev boards using their HyperRAM IP for external RAM.
This doc's §5 sketches the integration; nothing in the RTL prevents
synthesis today, only the Verilator-specific I/O glue does (SDL,
`$readmemh`, etc.) and those are isolated behind a single top-level
module.
