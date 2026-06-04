#!/usr/bin/env python3
"""Generate the MEM_POKE env-var string for `make wb-pristine`.

Per WB13_DEBUG_JOURNAL §34-§36, the WB1.3 boot reaches a state where
the CLI window's title bar BPL1 region is filled with the inactive
50% stipple ($2AAA) instead of the active solid ($FF).  This script
generates a 390-entry MEM_POKE list that overwrites the title bar
region with:
  - $FF (solid white) for rows 0-9 of BPL1
  - 'AmigaDOS' glyph bytes from the FS-UAE reference dump
  - WB Backdrop right border ($03 $C0 at byte 78-79 of every row)

Stdout is the comma-separated `addr=value` list ready to feed to the
MEM_POKE env var.

Per WB13_DEBUG_JOURNAL §35b, this is a runtime patch.  The real fix
to Intuition's active/inactive predicate is still open (task #143).
"""

import os
import sys

BPL1 = 0x60C8
NUM_ROWS = 200
ROW_BYTES = 80
TITLE_ROWS = 10

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FSU_CHIP = os.path.join(ROOT, 'test_data', 'fsuae_wb13_idle_chip.bin')


def build_target_bytes():
    fsu = open(FSU_CHIP, 'rb').read()
    mem = bytearray(NUM_ROWS * ROW_BYTES)
    # (1) Title bar rows 0-9 to $FF
    for i in range(TITLE_ROWS * ROW_BYTES):
        mem[i] = 0xFF
    # (2) Right border at bytes 78-79 of every row
    for row in range(NUM_ROWS):
        mem[row * ROW_BYTES + 78] = 0x03
        mem[row * ROW_BYTES + 79] = 0xC0
    # (3) 'AmigaDOS' text from FSU
    for row in range(1, 8):
        for col in range(10):
            mem[row * ROW_BYTES + col] = fsu[BPL1 + row * ROW_BYTES + col]
    return bytes(mem)


def main():
    target = build_target_bytes()
    pokes = []
    for off in range(0, NUM_ROWS * ROW_BYTES, 4):
        word = (target[off]   << 24) | (target[off+1] << 16) | \
               (target[off+2] <<  8) |  target[off+3]
        if word != 0:
            pokes.append((BPL1 + off, word))
    sys.stdout.write(','.join(f'0x{a:X}=0x{v:08X}' for a, v in pokes))


if __name__ == '__main__':
    main()
