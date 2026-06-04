"""Regression: render_k13_screen.try_render_sprite must overlay sprite
pixels onto the bitplane image when chip RAM is passed as a (chip, slow)
tuple.

Previously the row-bounds check used `len(chip)`, which evaluated to 2
(the tuple length) and silently bailed before any sprite row was drawn —
the Workbench mouse cursor never appeared in any rendered screenshot
even though SPR0PT pointed at valid sprite data.

Run: `python3 tests/test_render_sprite.py`
"""

import os
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))

from PIL import Image  # noqa: E402
import render_k13_screen as rks  # noqa: E402


def _put_w(buf, off, val):
    buf[off]     = (val >> 8) & 0xFF
    buf[off + 1] = val & 0xFF


def build_chip_with_arrow_sprite():
    """Return a 64 KB chip buffer with a simple sprite at $C80.

    Sprite control: VSTART=10, VSTOP=18 (8 rows), HSTART=20.
    Color: all '1' pixels (w0 set, w1 clear).
    Each row's w0 = 0xFF00 (8 visible px).  Total visible = 8 * 8 = 64.
    """
    chip = bytearray(0x10000)
    # POS = vstart_lo<<8 | hstart>>1 = 0x0A << 8 | 0x0A = 0x0A0A
    # CTL = vstop_lo<<8 | ... = 0x12 << 8 = 0x1200 (hstart even → no bit0)
    _put_w(chip, 0xC80, 0x0A0A)
    _put_w(chip, 0xC82, 0x1200)
    for row in range(8):
        _put_w(chip, 0xC84 + row * 4, 0xFF00)   # w0 = 8 lit px
        _put_w(chip, 0xC84 + row * 4 + 2, 0)   # w1 = 0
    # End-of-list marker at $CA4.
    _put_w(chip, 0xCA4, 0)
    _put_w(chip, 0xCA6, 0)
    return bytes(chip)


class FakeSim:
    def palette(self):
        # idx 17 (SPR0 color 1) → bright red.
        pal = [(0, 85, 170)] * 32
        pal[17] = (255, 0, 0)
        pal[18] = (0, 255, 0)
        pal[19] = (0, 0, 255)
        return pal


def count_red_pixels(img):
    px = img.load()
    n = 0
    for y in range(img.size[1]):
        for x in range(img.size[0]):
            if px[x, y] == (255, 0, 0):
                n += 1
    return n


def test_tuple_chip_draws_sprite():
    chip = build_chip_with_arrow_sprite()
    slow = bytes(0x80000)
    img = Image.new('RGB', (320, 200), (0, 85, 170))
    pal = FakeSim().palette()
    rks.try_render_sprite(img, (chip, slow), 0xC80, FakeSim(),
                          idx=0, palette=pal)
    n = count_red_pixels(img)
    assert n == 64, (
        f"expected 64 red sprite pixels with (chip, slow) tuple, got {n}. "
        "Did the len(chip)==2 bounds bug regress?"
    )


def test_bytes_chip_still_works():
    """Sanity: legacy single-buffer mode still draws the sprite."""
    chip = build_chip_with_arrow_sprite()
    img = Image.new('RGB', (320, 200), (0, 85, 170))
    pal = FakeSim().palette()
    rks.try_render_sprite(img, chip, 0xC80, FakeSim(),
                          idx=0, palette=pal)
    assert count_red_pixels(img) == 64


if __name__ == '__main__':
    test_tuple_chip_draws_sprite()
    test_bytes_chip_still_works()
    print("PASS: render_k13_screen sprite overlay regression")
