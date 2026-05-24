#!/usr/bin/env python3
"""Merge a small rom_test.hex (sentinels at the front) with an asm68k.py
program.hex.  The resulting hex is loaded into the ROM region by
$readmemh so that, under OVL_RESET=1, instruction fetches at the test
program's reset PC ($400) read the program code from ROM — and direct
ROM-window reads at $F80000 / $F80004 still return the test sentinels.

Layout of the output:
  - lines 0..N-1: contents of rom_test.hex (the sentinels)
  - lines N..255: zeros (pad up to byte offset $400 = word $100)
  - lines 256..  : the assembled program (asm68k .org $400 starts here)

Usage:
  merge_rom_program.py <rom_test.hex> <program.hex> <out.hex>
"""
import sys


def main():
    if len(sys.argv) != 4:
        print("usage: merge_rom_program.py <rom_test.hex> <program.hex> <out.hex>", file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1]) as f:
        sentinels = [line.strip() for line in f if line.strip()]
    with open(sys.argv[2]) as f:
        program = [line.strip() for line in f if line.strip()]

    merged = list(program)
    if len(merged) < 256:
        merged.extend(['00000000'] * (256 - len(merged)))
    merged[:len(sentinels)] = sentinels

    with open(sys.argv[3], 'w') as f:
        f.write('\n'.join(merged) + '\n')


if __name__ == '__main__':
    main()
