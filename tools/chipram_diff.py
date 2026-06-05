#!/usr/bin/env python3
"""Chip RAM diff visualizer for the 68k Verilator sim.

Compares two chip-RAM dumps (typically our sim vs FS-UAE reference)
and renders a PNG showing exactly which bytes differ.  Reuses
tools/render_k13_screen.py's palette/bitplane logic.

Usage:
    tools/chipram_diff.py OURS.bin FSU.bin --out diff.png

    # Limit to BPL1 area:
    tools/chipram_diff.py OURS.bin FSU.bin --start 0x60c8 --end 0xb0c8

The output is a 640×200 image (matching hi-res Workbench layout)
where each pixel is colored based on the per-byte diff:
    black  — bytes match
    red    — ours has bits FSU doesn't  (extra)
    green  — FSU has bits ours doesn't  (missing)
    yellow — both have bits but they differ (mixed)

Falls back to text-mode (one line per row, diff-count) if PIL
isn't available.
"""

import argparse
import sys


def load_dump(path):
    with open(path, 'rb') as f:
        return f.read()


def diff_bytes(o: int, f: int) -> int:
    """Return diff category for two bytes.
    0=match, 1=ours-only, 2=fsu-only, 3=mixed."""
    if o == f:
        return 0
    extra = o & ~f      # bits in ours but not fsu
    missing = f & ~o    # bits in fsu but not ours
    if extra and missing:
        return 3
    elif extra:
        return 1
    elif missing:
        return 2
    return 0


def text_mode(ours, fsu, start, end, bpl_base, row_stride):
    """ASCII summary when PIL is not available."""
    print(f'Chip RAM diff: ${start:06X} - ${end:06X}')
    print(f'  bpl_base=${bpl_base:06X} row_stride={row_stride}')
    print()
    print(f'{"row":>3} {"addr":>7} {"bytes-diff":>10} '
          f'  diff-bytes hex (ours^fsu)')
    total_rows = (end - bpl_base) // row_stride
    for row in range(total_rows):
        a = bpl_base + row * row_stride
        if a < start or a >= end:
            continue
        diff_count = 0
        diff_byte_idx = []
        for i in range(row_stride):
            if ours[a+i] != fsu[a+i]:
                diff_count += 1
                if len(diff_byte_idx) < 8:
                    diff_byte_idx.append((i, ours[a+i] ^ fsu[a+i]))
        if diff_count > 0:
            hex_pairs = ' '.join(f'@{i}=${x:02x}' for i, x in diff_byte_idx)
            print(f'{row:>3} ${a:06X} {diff_count:>10}  {hex_pairs}')


def render_png(ours, fsu, start, end, bpl_base, row_stride, out_path):
    try:
        from PIL import Image
    except ImportError:
        print('PIL not available — falling back to text mode',
              file=sys.stderr)
        return False

    # 640×200 hi-res image, each "pixel" = one bit.
    # We'll output one pixel per BIT of BPL1 (matches normal screen
    # rendering: 80 bytes/row * 8 bits = 640 cols).
    rows = (end - bpl_base) // row_stride
    cols = row_stride * 8
    img = Image.new('RGB', (cols, rows), (0, 0, 0))
    pixels = img.load()

    palette = {
        0: (16, 16, 16),       # match (dim)
        1: (220, 60, 40),      # ours-only (red)
        2: (60, 200, 60),      # fsu-only (green)
        3: (240, 200, 40),     # mixed (yellow)
    }

    diff_total = {1: 0, 2: 0, 3: 0}
    for row in range(rows):
        a = bpl_base + row * row_stride
        for col_byte in range(row_stride):
            o = ours[a + col_byte]
            f = fsu[a + col_byte]
            if o == f:
                continue
            # Per-bit categorization
            for bit in range(8):
                ob = (o >> (7 - bit)) & 1
                fb = (f >> (7 - bit)) & 1
                if ob == fb:
                    continue
                if ob and not fb:
                    cat = 1
                elif fb and not ob:
                    cat = 2
                else:
                    cat = 3
                pixels[col_byte * 8 + bit, row] = palette[cat]
                diff_total[cat] += 1

    img.save(out_path)
    total = sum(diff_total.values())
    print(f'Saved {out_path}  ({cols}×{rows} W×H)')
    print(f'  Diff bits: ours-only={diff_total[1]}  '
          f'fsu-only={diff_total[2]}  mixed={diff_total[3]}  '
          f'total={total}')
    return True


def main():
    p = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('ours', help='our chip-RAM dump (binary)')
    p.add_argument('fsu', help='FS-UAE chip-RAM dump (binary)')
    p.add_argument('--start', default='0x60c8',
                   help='chip address range start (default $60c8 = BPL1 base)')
    p.add_argument('--end', default='0xb0c8',
                   help='chip address range end (default $b0c8 = BPL1 end)')
    p.add_argument('--row-stride', type=int, default=80,
                   help='bytes per row (default 80 = 640px hi-res)')
    p.add_argument('--out', default='/tmp/chipram_diff.png')
    p.add_argument('--text', action='store_true',
                   help='print text-mode summary instead of PNG')
    args = p.parse_args()

    start = int(args.start, 0)
    end = int(args.end, 0)
    bpl_base = start
    ours = load_dump(args.ours)
    fsu = load_dump(args.fsu)
    if len(ours) != len(fsu):
        print(f'WARNING: dumps differ in size ({len(ours)} vs {len(fsu)})',
              file=sys.stderr)

    if args.text:
        text_mode(ours, fsu, start, end, bpl_base, args.row_stride)
    else:
        if not render_png(ours, fsu, start, end, bpl_base,
                          args.row_stride, args.out):
            text_mode(ours, fsu, start, end, bpl_base, args.row_stride)


if __name__ == '__main__':
    main()
