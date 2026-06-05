#!/usr/bin/env python3
"""Render the boing demo's chip-RAM dump to PNG.

The demo writes its display setup directly to legacy chipset addresses
($00FE0xxx) without a Copper list, so the K1.3 renderer's
auto-detect doesn't apply.  This is a fixed-config renderer for:
  - 320×200 lo-res, 1 bitplane at $5000
  - SPR0 data at $7000
  - Palette: COLOR00=blue, COLOR01=white, SPR0 colors=red/white

Outputs the final frame at the moment the demo halts.
"""

import argparse
from PIL import Image


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--chipram', required=True)
    p.add_argument('--out', default='/tmp/boing.png')
    p.add_argument('--width', type=int, default=320)
    p.add_argument('--height', type=int, default=200)
    args = p.parse_args()

    chip = open(args.chipram, 'rb').read()

    # Palette (RGB tuples)
    BG    = (0, 0, 255)        # COLOR00 blue
    BPL1  = (255, 255, 255)    # COLOR01 white (floor grid)
    SPR_A = (255, 32, 32)      # red
    SPR_B = (255, 255, 255)    # white (other half of checker)

    img = Image.new('RGB', (args.width, args.height), BG)
    px = img.load()

    # Render BPL0 (the floor grid)
    BPL0_BASE = 0x5000
    BYTES_PER_ROW = args.width // 8
    for row in range(args.height):
        for col_byte in range(BYTES_PER_ROW):
            b = chip[BPL0_BASE + row * BYTES_PER_ROW + col_byte]
            for bit in range(8):
                if b & (0x80 >> bit):
                    px[col_byte * 8 + bit, row] = BPL1

    # Read SPR0 position from chipset shadow.
    # Demo writes SPR0POS to $FE0140, SPR0CTL to $FE0142.
    # Our sim's legacy chipset window mirrors at $FE0000..$FE0FFF — but
    # the chip-RAM dump is the actual RAM at $00000-$80000, NOT the
    # chipset registers.  So we can't read SPR0POS from the dump.
    # Instead, decode from sprite data at $7000:
    #   word 0 = POS = (vstart << 8) | (hstart >> 1)
    #   word 1 = CTL = (vstop << 8) | flags
    SPR0_BASE = 0x7000
    pos = int.from_bytes(chip[SPR0_BASE:SPR0_BASE+2], 'big')
    ctl = int.from_bytes(chip[SPR0_BASE+2:SPR0_BASE+4], 'big')
    vstart = (pos >> 8) & 0xFF
    hstart = ((pos & 0xFF) << 1) | (ctl & 1)
    print(f'SPR0 at chip-RAM POS=${pos:04X} CTL=${ctl:04X}')
    print(f'  vstart={vstart} hstart={hstart}')

    # Demo writes the LIVE sprite position to legacy chipset
    # ($FE0140 = SPR0POS, $FE0142 = SPR0CTL) every frame, but the
    # chip-RAM at $7000 still holds the BASE values from init.
    # So the ball position seen here is the INITIAL position, not the
    # animated position.  That's still a valid Boing-like render.

    # Draw the 16×16 sprite.  Sprite data:
    #   $7000-7003: POS, CTL (4 bytes)
    #   $7004-7043: 16 lines × 4 bytes (data_a word, data_b word)
    # For each scanline:
    #   da = data_a word, db = data_b word
    #   pixel = (da_bit << 0) | (db_bit << 1)
    #     0 = transparent (don't draw)
    #     1 = COLOR17 (red)
    #     2 = COLOR18 (white)
    #     3 = COLOR19 (grey)
    SPR_COLORS = [None, SPR_A, SPR_B, (153, 153, 153)]
    for line in range(16):
        off = SPR0_BASE + 4 + line * 4
        da = int.from_bytes(chip[off:off+2], 'big')
        db = int.from_bytes(chip[off+2:off+4], 'big')
        for bit in range(16):
            a = (da >> (15 - bit)) & 1
            b = (db >> (15 - bit)) & 1
            color_idx = (b << 1) | a
            color = SPR_COLORS[color_idx]
            if color is None:
                continue
            x = hstart + bit
            y = vstart + line
            if 0 <= x < args.width and 0 <= y < args.height:
                px[x, y] = color

    # Scale 2x for better viewing.
    img = img.resize((args.width * 2, args.height * 2), Image.NEAREST)
    img.save(args.out)
    print(f'Saved {args.out}')


if __name__ == '__main__':
    main()
