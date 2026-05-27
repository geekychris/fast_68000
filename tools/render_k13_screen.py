#!/usr/bin/env python3
"""Render Kickstart 1.3's screen by externally simulating its Copper list.

K1.3 writes a Copper list to chip RAM at boot, enables DMACON BPLEN +
COPEN, and expects the Copper to fire each VBLANK and program Agnus/
Denise registers (bitplane pointers, palette, sprites).  Our Verilog
Copper uses a different (32-bit-target) encoding so it cannot execute
K1.3's real-Amiga Copper list directly.

This script parses the chipram.bin dump produced by CHIPRAM_DUMP=
and walks the Copper list interpretively to find the final state of
the Agnus/Denise registers.  Then it renders the bitplane data (and
sprite 0, the disk-prompt animation) into a PNG so we can see what
K1.3 intended to display.

Usage:
    python3 tools/render_k13_screen.py [--chipram FILE] [--out FILE]
                                       [--cop1lc ADDR] [--bplcon0 VAL]
                                       [--width 320] [--height 200]
"""
import argparse
import struct
from PIL import Image


# Default Workbench palette (4-color) used by K1.3 when COLOR00-03 aren't
# explicitly set by the Copper list (the boot Copper list doesn't set
# them; Intuition.OpenScreen would, but that hasn't run yet).
DEFAULT_PAL_4 = [
    (0xAA, 0xAA, 0xFF),  # COLOR00: light blue (Workbench background)
    (0xFF, 0xFF, 0xFF),  # COLOR01: white (text)
    (0x00, 0x00, 0x00),  # COLOR02: black
    (0xFF, 0x88, 0x22),  # COLOR03: orange
]


# Sprite-palette default: COLOR17-19 for sprite 0/1, etc.
def amiga_color_to_rgb(c12):
    """$0RGB 4-bit -> 8-bit RGB."""
    r = (c12 >> 8) & 0xF
    g = (c12 >> 4) & 0xF
    b = c12 & 0xF
    # Replicate each nibble to fill 8 bits.
    return ((r << 4) | r, (g << 4) | g, (b << 4) | b)


_SLOW_BASE = 0x00C00000
_SLOW_SIZE = 0x00080000


def _addr_to_buf(chip, addr):
    """Return (buf, offset) for a given byte address, picking chip vs slow.

    If `chip` is a bytes-like object: legacy mode, only chip ram available
    (address must be < 0x80000).  If `chip` is a tuple (chip_bytes,
    slow_bytes), slow RAM is also accessible at $C00000-$C7FFFF.
    """
    if isinstance(chip, tuple):
        c, s = chip
        if _SLOW_BASE <= addr < _SLOW_BASE + _SLOW_SIZE:
            return s, addr - _SLOW_BASE
        return c, addr
    return chip, addr


def fetch_word(chip, addr):
    buf, off = _addr_to_buf(chip, addr)
    if off + 1 >= len(buf):
        return 0
    return (buf[off] << 8) | buf[off + 1]


def fetch_long(chip, addr):
    buf, off = _addr_to_buf(chip, addr)
    if off + 3 >= len(buf):
        return 0
    return ((buf[off] << 24) | (buf[off + 1] << 16) |
            (buf[off + 2] << 8) | buf[off + 3])


# Amiga chipset register name table for diagnostic dump.
REG_NAMES = {
    0x080: 'COP1LCH', 0x082: 'COP1LCL', 0x084: 'COP2LCH', 0x086: 'COP2LCL',
    0x088: 'COPJMP1', 0x08A: 'COPJMP2', 0x08C: 'COPINS',
    0x08E: 'DIWSTRT', 0x090: 'DIWSTOP',
    0x092: 'DDFSTRT', 0x094: 'DDFSTOP', 0x096: 'DMACON',
    0x09A: 'INTENA',  0x09C: 'INTREQ',  0x09E: 'ADKCON',
    0x0E0: 'BPL1PTH', 0x0E2: 'BPL1PTL', 0x0E4: 'BPL2PTH', 0x0E6: 'BPL2PTL',
    0x0E8: 'BPL3PTH', 0x0EA: 'BPL3PTL', 0x0EC: 'BPL4PTH', 0x0EE: 'BPL4PTL',
    0x0F0: 'BPL5PTH', 0x0F2: 'BPL5PTL',
    0x100: 'BPLCON0', 0x102: 'BPLCON1', 0x104: 'BPLCON2',
    0x106: 'BPLCON3', 0x108: 'BPL1MOD', 0x10A: 'BPL2MOD',
    0x110: 'BPL1DAT', 0x112: 'BPL2DAT', 0x114: 'BPL3DAT', 0x116: 'BPL4DAT',
    0x120: 'SPR0PTH', 0x122: 'SPR0PTL', 0x124: 'SPR1PTH', 0x126: 'SPR1PTL',
    0x128: 'SPR2PTH', 0x12A: 'SPR2PTL', 0x12C: 'SPR3PTH', 0x12E: 'SPR3PTL',
    0x130: 'SPR4PTH', 0x132: 'SPR4PTL', 0x134: 'SPR5PTH', 0x136: 'SPR5PTL',
    0x138: 'SPR6PTH', 0x13A: 'SPR6PTL', 0x13C: 'SPR7PTH', 0x13E: 'SPR7PTL',
}


def reg_name(off):
    if 0x180 <= off <= 0x1BE and (off & 1) == 0:
        return f"COLOR{(off-0x180)//2:02X}"
    return REG_NAMES.get(off, f"?${off:03X}")


def _pc_mask(addr):
    # If pointing into slow RAM, keep the full 24-bit address; otherwise
    # mask to 19-bit chip range so legacy callers still work.
    if _SLOW_BASE <= addr < _SLOW_BASE + _SLOW_SIZE:
        return addr & 0xFFFFFF
    return addr & 0xFFFFF


class CopperSim:
    """Walk Kickstart's Copper list to find final Agnus/Denise state."""

    def __init__(self, chip, cop1lc):
        # chip can be raw chip-RAM bytes (legacy) or a (chip, slow) tuple
        # exposing both regions for the new slow-RAM-backed boot path.
        self.chip = chip
        self.pc = _pc_mask(cop1lc)
        self.regs = {}              # offset (0..0x1FE) -> 16-bit value
        self.log = []
        # Simulate to a target raster position: end of frame, both VP=ff HP=7f.
        self.vp = 0xFF
        self.hp = 0x7F

    def step(self):
        """Decode one instruction, return False if we should stop."""
        # Bounds-check fetch_word handles slow vs chip routing.
        target = fetch_word(self.chip, self.pc)
        data   = fetch_word(self.chip, self.pc + 2)
        if target == 0 and data == 0 and len(self.log) > 1:
            # Likely end-of-list padding.
            return False
        self.pc += 4
        if target & 1:
            # WAIT or SKIP
            tv = (target >> 8) & 0xFF
            th = (target >> 1) & 0x7F
            sk = data & 1
            ve = (data >> 8) & 0x7F
            he = (data >> 1) & 0x7F
            kind = "SKIP" if sk else "WAIT"
            self.log.append(f"  ${self.pc-4:05x}: {kind} V=${tv:02X} H=${th:02X}")
            # SKIP: skip next inst if vp>=tv (within enable mask) and
            # similar for hp.  We treat the "end of list" terminator
            # WAIT $FFFF as a stop.
            if tv == 0xFF and th == 0x7F and not sk:
                return False
            if sk:
                # Conditional skip; we always at end-of-frame so V>=ANY
                # is true, advance past next inst.
                if self.vp >= tv:
                    self.pc += 4
            # WAIT: skip ahead virtually -- since we're simulating at
            # end-of-frame, all WAITs pass through immediately.
            return True
        else:
            # MOVE: target[8:1] = reg offset (0..510 step 2)
            reg = target & 0x1FE
            self.regs[reg] = data
            self.log.append(f"  ${self.pc-4:05x}: MOVE #{data:04X} -> "
                            f"{reg_name(reg)} (${reg:03X})")
            # COPJMP2: jump to COP2LC instead.
            if reg == 0x08A:
                next_pc = ((self.regs.get(0x084, 0) << 16) |
                           self.regs.get(0x086, 0))
                self.pc = _pc_mask(next_pc)
            return True

    def run(self, limit=10000):
        for _ in range(limit):
            if not self.step():
                break

    def bpl_pt(self, plane_idx):
        h = self.regs.get(0xE0 + plane_idx * 4, 0)
        l = self.regs.get(0xE2 + plane_idx * 4, 0)
        return ((h << 16) | l) & 0xFFFFFF

    def spr_pt(self, idx):
        h = self.regs.get(0x120 + idx * 4, 0)
        l = self.regs.get(0x122 + idx * 4, 0)
        return ((h << 16) | l) & 0xFFFFFF

    def palette(self):
        pal = list(DEFAULT_PAL_4) + [(0, 0, 0)] * 28
        for i in range(32):
            v = self.regs.get(0x180 + i * 2, None)
            if v is not None:
                pal[i] = amiga_color_to_rgb(v)
        return pal


def render(chip, cop1lc, width=320, height=200, out='k13_screen.png',
           cop2lc=None):
    sim = CopperSim(chip, cop1lc)
    if cop2lc:
        sim.regs[0x084] = (cop2lc >> 16) & 0xFFFF
        sim.regs[0x086] = cop2lc & 0xFFFF
    sim.run()

    print("Copper trace:")
    for line in sim.log:
        print(line)

    bplcon0 = sim.regs.get(0x100, 0)
    bpu = (bplcon0 >> 12) & 7
    print(f"\nBPLCON0=${bplcon0:04X}  BPU={bpu}")
    pal = sim.palette()
    print(f"Palette: {pal[:4]}")
    for i in range(min(6, max(1, bpu))):
        print(f"  BPL{i+1}PT = ${sim.bpl_pt(i):05X}")
    for i in range(8):
        p = sim.spr_pt(i)
        if p:
            print(f"  SPR{i}PT = ${p:05X}")

    # Render bitplanes.
    if bpu == 0:
        bpu = 1  # render at least 2-color background so we can overlay sprites
    bpr = width // 8
    img = Image.new('RGB', (width, height), pal[0])
    pixels = img.load()
    def read_bpl_byte(pt, plane_idx, y, byte_idx):
        addr = pt + y * bpr + byte_idx
        buf, off = _addr_to_buf(chip, addr)
        if off < 0 or off >= len(buf):
            return 0
        return buf[off]

    plane_pt = [sim.bpl_pt(p) for p in range(bpu)]
    for y in range(height):
        for byte_idx in range(bpr):
            for bit in range(8):
                x = byte_idx * 8 + bit
                idx = 0
                for p, pt in enumerate(plane_pt):
                    if pt == 0:
                        continue
                    b = read_bpl_byte(pt, p, y, byte_idx)
                    if b & (0x80 >> bit):
                        idx |= (1 << p)
                pixels[x, y] = pal[idx]

    # Overlay sprite 0 if present and enabled.
    spr0 = sim.spr_pt(0)
    if spr0:
        try_render_sprite(img, chip, spr0, sim, idx=0, palette=pal)

    # Scale up 2x for visibility.
    img = img.resize((width * 2, height * 2), Image.NEAREST)
    img.save(out)
    print(f"\nSaved {out}")
    return img


def try_render_sprite(img, chip, sprpt, sim, idx, palette):
    """Render an Amiga hardware sprite into the image."""
    pixels = img.load()
    # Sprite layout: each sprite has a chain of (POS, CTL, [data pairs..])
    # POS word: HSTART_lo (8 bits low) + VSTART_lo (8 bits hi)
    # CTL word: VSTOP_lo (8 hi) + VSTART_hi/VSTOP_hi/HSTART_hi/ATTACH (8 lo)
    p = sprpt
    safety = 64
    while safety > 0:
        pos = fetch_word(chip, p)
        ctl = fetch_word(chip, p + 2)
        vstart = ((pos >> 8) & 0xFF) | ((ctl & 0x4) << 6)
        vstop  = ((ctl >> 8) & 0xFF) | ((ctl & 0x2) << 7)
        hstart = ((pos & 0xFF) << 1) | (ctl & 0x1)
        if vstart == 0 and vstop == 0:
            return  # disabled / end-of-sprite-list
        if vstop <= vstart:
            return
        height = vstop - vstart
        p += 4
        # Sprite color palette: SPR0 uses COLOR17-19 (idx*2+17).
        spr_pal = [None,
                   palette[17 + idx * 2],
                   palette[18 + idx * 2],
                   palette[19 + idx * 2]]
        for row in range(height):
            if p + 4 > len(chip):
                return
            w0 = fetch_word(chip, p)
            w1 = fetch_word(chip, p + 2)
            p += 4
            y = vstart + row
            if y >= img.size[1] // 1:
                continue
            for bit in range(16):
                px = (1 if (w0 & (0x8000 >> bit)) else 0) | \
                     (2 if (w1 & (0x8000 >> bit)) else 0)
                if px == 0:
                    continue
                # Sprite x is in lo-res pixels (320 px screen has H=0..159 *2).
                x = hstart * 1 // 1 + bit
                if 0 <= x < img.size[0] and 0 <= y < img.size[1]:
                    pixels[x, y] = spr_pal[px] or (255, 0, 255)
        safety -= 1


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--chipram', default='build_kick_boot/chipram.bin')
    ap.add_argument('--slowram', default=None,
                    help='Optional Agnus slow-RAM dump (\$C00000-\$C7FFFF). '
                         'When set, the Copper list and bitplanes can live in '
                         'slow RAM (the post-26f2f86 K1.3 boot layout).')
    ap.add_argument('--out',     default='build_kick_boot/k13_render.png')
    ap.add_argument('--cop1lc',  default='0x2368',
                    help='Address of K1.3 Copper list. Defaults to \$2368 '
                         '(chip-RAM layout); with --slowram set, try '
                         '\$00C00276 for the slow-RAM boot copper list.')
    ap.add_argument('--cop2lc',  default='0x23bc')
    ap.add_argument('--width',   type=int, default=320)
    ap.add_argument('--height',  type=int, default=200)
    args = ap.parse_args()

    with open(args.chipram, 'rb') as f:
        chip = f.read()
    if args.slowram:
        with open(args.slowram, 'rb') as f:
            slow = f.read()
        mem = (chip, slow)
    else:
        mem = chip

    cop1lc = int(args.cop1lc, 0)
    cop2lc = int(args.cop2lc, 0)
    print(f"Rendering K1.3 screen: chipram={args.chipram} "
          f"slowram={args.slowram} cop1lc=${cop1lc:X}")
    render(mem, cop1lc, args.width, args.height, args.out, cop2lc=cop2lc)


if __name__ == '__main__':
    main()
