# Amiga compatibility

A working map of what's implemented on top of the 68k core to let
Amiga-shaped software run.  This is the user-facing companion to
[DESIGN.md](DESIGN.md) (CPU architecture) and [TESTING.md](TESTING.md)
(how each piece is verified).

## Boot path

```mermaid
flowchart LR
    R[CPU reset] --> O["PC=$400<br/>OVL=1<br/>A7=$4000+core*$1000"]
    O -->|IF $400 routed by OVL| T["ROM offset $400:<br/>trampoline"]
    T -->|"move.l $0,A7<br/>move.l $4,A0<br/>jmp (A0)"| V["ROM offset 0/4:<br/>SSP + PC vectors"]
    V --> E["ROM offset 8:<br/>boot_entry"]
    E --> C["CIA-A PRA bit 0<br/>clears OVL"]
    C --> S[normal RAM addressing]
```

The CPU's hardware `RESET_PC` is fixed at `$400` and per-core `A7` is
fixed at `$4000 + core*$1000` — we don't have a microcoded reset-vector
fetch.  Instead, **OVL routes the IF at `$400` to ROM offset `$400`**,
which is where we place a 3-instruction trampoline that does the
canonical 68000 reset-vector sequence: read SSP from `$0`, read PC from
`$4`, jump.  Functionally equivalent to a real Amiga boot — the
difference is software-in-ROM versus microcode-in-CPU.

To boot a real Kickstart binary:

```
make test-boot-rom-bin ROMFILE=path/to/your.rom ROMSIZE_WORDS=65536
```

The harness will load the binary into the ROM region, set `OVL_RESET=1`,
and run for 10 M cycles.  The tail of `[sim]` output tells you what
happened — see "What still hangs Kickstart" below.

## Chipset register coverage

Bus addresses our hardware actually responds to (some functionally,
some as canned read-only stubs, some as plain storage that round-trips):

### Agnus (live, inline in `m68k_bus.v`)

| addr      | reg       | direction | behavior                          |
|-----------|-----------|-----------|-----------------------------------|
| `$DFF002` | DMACONR   | R         | mirror of dmacon                  |
| `$DFF004` | VPOSR     | R         | bit 0 of high byte = agnus_v[8]   |
| `$DFF006` | VHPOSR    | R         | {agnus_v[7:0], agnus_h[7:0]}      |
| `$DFF096` | DMACON    | W         | SET/CLR semantics                 |
| `$DFF0E0` | BPL1PTH   | RW        | shadow, used by bitplane DMA      |
| `$DFF0E2` | BPL1PTL   | RW        | shadow, used by bitplane DMA      |

Beam counters tick every host clock; H wraps at 226 (227-cyc PAL line),
V wraps at 311 (312-line PAL frame).  Not slot-accurate vs real Agnus.

### Paula (live)

| addr      | reg       | behavior                          |
|-----------|-----------|-----------------------------------|
| `$DFF000` | AUDENA    | 4-bit audio DMA enable            |
| `$DFF01C` | INTENAR   | INTENA + INTREQ mirror (combined) |
| `$DFF01E` | INTREQR   | (same word as INTENAR)            |
| `$DFF09A` | INTENA    | SET/CLR, bit 14 = master INTEN    |
| `$DFF09C` | INTREQ    | SET/CLR, bits 13:0 = sources      |
| `$DFF0A0..A8` | AUD0LC/LEN/PER/VOL | channel 0          |
| `$DFF0B0..B8` | AUD1*                              |
| `$DFF0C0..C8` | AUD2*                              |
| `$DFF0D0..D8` | AUD3*                              |
| `$DFF020` | DSKPTH    | floppy DMA destination high       |
| `$DFF022` | DSKPTL    | floppy DMA destination low        |
| `$DFF024` | DSKLEN    | length + bit15 DMAEN -> triggers DMA |

Paula's interrupt priority encoder drives a 3-bit `irq_level` out to the
CPU.  Bit assignments match the canonical Amiga (PORTS at bit 3, BLIT
at bit 6, EXTER at bit 13, audio at bits 7-10).

### Paula RO stubs (canned values)

| addr      | reg     | reads as                          |
|-----------|---------|-----------------------------------|
| `$DFF010` | ADKCONR | 0                                 |
| `$DFF012` | POT0DAT | 0                                 |
| `$DFF014` | POT1DAT | 0                                 |
| `$DFF016` | POTGOR  | `$FFFF` (all buttons up)          |
| `$DFF018` | SERDATR | `$6000` (TSRE \| TBE)             |
| `$DFF00A` | JOY0DAT | 0                                 |
| `$DFF00C` | JOY1DAT | 0                                 |

Enough for Kickstart's early-init probes not to hang.  Joystick /
mouse / paddle / serial input is not wired up.

### Denise (live, via its own module)

`$DFF100-$DFF1FF` bank 0 (BPLCON0..3, BPL1DAT..BPL6DAT, COLOR00..31,
sprite registers, joystick collision), `$DFF300-$DFF3FF` bank 2 (HAM8
extension: extra bitplane pointers and 32 additional colors).

### Blitter (`$00FE_0000-$00FE_003F` + canonical alias)

Live.  Full copy/line/fill/BZERO, ASH/BSH barrel shifts, modulos, LF
minterm, line mode with octant + accumulator state.  Status at
`$00FE_003C` exposes BUSY and BZERO.

### Copper (`$00FE_0040-$00FE_007F`)

Live.  MOVE / WAIT / SKIP instructions, runs from a memory-resident
copper list.  COP1LCH/L and COP2LCH/L live in the shadow region
(`$DFF080-$DFF08F`).

### CIA-A and CIA-B

`$BFE001..$BFEF01` (CIA-A, 256-byte stride) and `$BFD000..$BFDF00`
(CIA-B).  PRA/PRB, DDRA/B, timer A/B (continuous + one-shot, IRQ on
underflow), 24-bit free-running TOD (`$08-$0A`), SDR (`$0C`),
read-and-clear ICR (`$0D`), CRA/CRB.

CIA-A drives `/OVL` on PRA bit 0 → bus clears OVL latch.

CIA-A also accepts keyboard-style serial byte injection: a CPU
write to `$00FE_9000` pulses the bus's `kbd_inject_wr` pin with the
written byte; CIA-A latches it into SDR and raises the SP-source
interrupt.

### Zorro II autoconfig

`$E80000-$E8FFFF` reads return `$FFFFFFFF` — "no autoconfig card
present" sentinel.  Writes drop silently.  Enough for Kickstart's
probe loop to terminate.

### Chipset shadow storage

Writes to these chipset addresses land in a 9-bit-indexed
storage block; reads return what was last written:

```
$DFF020-$DFF02E  DSK* (DSKDAT etc.) — DSKLEN itself is live
$DFF080-$DFF094  COPxLCH/L, COPJMP1/2, COPCON, DIWSTRT/STOP, DDFSTRT
$DFF098          CLXCON
$DFF0E0-$DFF0FF  BPL1..6PT
```

Real Kickstart writes a lot of bitplane setup state during init;
the shadow makes those writes observable on subsequent reads without
needing a functional slave for every register.

### Bitplane DMA (Agnus inline)

When `DMACON[8]` (BPLEN) AND `DMACON[9]` (DMAEN) are both set, Agnus
runs a fetch engine:

- Reloads `bpl1pt` from `chip_regs[BPL1PT]` at start of every scanline
  (detected by `agnus_v` change).
- Every 16 host clocks, fetches one half-word from `mem[bpl1pt]` into
  the internal `BPL1DAT` register and advances `bpl1pt` by 2.

Today only one plane is wired up.  Multi-plane fetch and slot-accurate
arbitration vs blitter/copper/CPU is the next step.

Tests can verify the engine via two MMIO probe addresses:

| addr           | reads as                          |
|----------------|-----------------------------------|
| `$00FE_9100`   | `bpl_fetches` — 32-bit counter    |
| `$00FE_9104`   | `bpl1dat` in high 16 bits         |

### Block-device / hardfile

| addr           | reg          |
|----------------|--------------|
| `$00FE_8000`   | BLKSRC (sector #)                                 |
| `$00FE_8004`   | BLKDST (destination byte addr in main RAM)        |
| `$00FE_8008`   | BLKCNT (number of 512-byte sectors)               |
| `$00FE_800C`   | BLKCMD (write 1 = start; reads 0 when done)       |
| `$00400000+`   | disk image (read-only, loaded from DISK_HEXFILE)  |

Stands in for an A590-style SCSI autoconfig device.  Real Amiga
`trackdisk.device` would talk to the canonical DSKLEN path
(see Paula above) which now uses the same engine under the hood.

## What still hangs Kickstart

The "fake Kickstart" test (`make test-fake-kickstart`) walks 13 steps
of what real Kickstart 1.x does in its first thousand instructions
and currently passes all of them.  Beyond that point real Kickstart
will likely trip on:

1. **Bitplane DMA slot arbitration** — Kickstart sets up bitplane
   pointers and a copper list expecting Agnus to interleave bitplane
   DMA with sprite, audio, blitter, refresh DMA in a fixed cycle
   pattern.  Our Agnus has a fetch counter but no slot scheduler;
   anything that depends on cycle-accurate display timing will
   diverge.
2. **`exec.library` autoconfig of FAST RAM** — we return `$FFFFFFFF`
   from `$E80000` (= "no card") so Kickstart will only see CHIP
   RAM.  Workbench works with CHIP-only but is slower; some
   software refuses to boot without FAST.
3. **Real DSKBYTR + MFM decode** — `trackdisk.device` reads raw MFM
   from `DSKBYTR` and decodes blocks itself.  Our floppy DMA fakes
   the result by copying pre-decoded sector data straight to
   `mem[DSKPT]` — works if you patch `trackdisk.device`, fails for
   stock Kickstart.
4. **Keyboard handshake protocol** — `keyboard.device` does a
   request/acknowledge handshake over CIA-A SP/CNT.  We model only
   the byte-arrival path (raise SP interrupt with the byte in SDR).
5. **No vertical-blank IRQ wiring** — Kickstart hooks `VERTB`
   (Paula INTREQ bit 5) for the system tick.  Our Agnus has beam
   counters but doesn't pulse VERTB when V wraps.

These are progressive: even without fixing any of them you can boot
toward a "Kickstart screen" by feeding the system a patched ROM that
skips the trackdisk and keyboard waits.  Each fix above brings real
Kickstart closer to stock.

## What runs today

- **`make test-fake-kickstart`** — 13-step early-init pass on a
  synthetic Kickstart-shape ROM.
- **`make test-boot-rom-ext`** — VHPOSR + DMACON + SERDATR + POTGOR +
  CIA-A TOD + blitter copy under the OVL → trampoline → ROM path.
- **`make test-floppy`** — sector-DMA via canonical DSKLEN registers.
- **`make crosscheck-minimig`** — register-readback parity vs
  MiniMig's `paula_intcontroller`; 0 hard mismatches, 2 documented
  soft (MiniMig allows bit 14 of INTREQ as writable storage).
- All Amiga chipset demos (`demo-blt`, `demo-cop`, `demo-den`, etc.)
  run from `demos/*.s` against the full chipset.

See [TESTING.md](TESTING.md) for the full target list and per-test
descriptions.
