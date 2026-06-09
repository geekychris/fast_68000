#!/usr/bin/env python3
"""Structured slow-RAM diff between two snapshots.

Prints every 4-byte longword where the two snapshots differ.
Used to compare our Verilog Amiga sim against FS-UAE-no-UAEFS for
the boing-disk investigation.

Usage:
    tools/slow_ram_diff.py OURS.bin FSU.bin [--start 0x00C00000] [--end 0x00C10000]

Tags each diff with a guess at what it might be:
  ROM_PTR    - value is in $FCxxxx-$FFFFFF range (ROM function pointer)
  SLOW_PTR   - value is in $C00000-$C7FFFF range (slow RAM ptr)
  CHIP_PTR   - value is in $0-$7FFFF range (chip RAM ptr)
  STRING?    - ASCII printable bytes
  ZERO       - all zeros
  SMALL_INT  - small integer (likely a count or flag)
  OTHER      - other value
"""

import argparse
import struct
import sys


def classify(val: int) -> str:
    if val == 0:
        return 'ZERO'
    if 0xFC0000 <= val <= 0xFFFFFF:
        return 'ROM_PTR'
    if 0x00C00000 <= val <= 0x00C7FFFF:
        return 'SLOW_PTR'
    if 0 < val < 0x80000:
        return 'CHIP_PTR'
    if val < 0x10000:
        return 'SMALL_INT'
    # Check if it looks like an ASCII string (4 printable ASCII bytes)
    bs = val.to_bytes(4, 'big')
    if all(32 <= b < 127 for b in bs):
        return f'STRING? "{bs.decode("latin-1")}"'
    return 'OTHER'


def main():
    p = argparse.ArgumentParser()
    p.add_argument('ours')
    p.add_argument('fsu')
    p.add_argument('--start', type=lambda s: int(s, 0), default=0x00C00000)
    p.add_argument('--end',   type=lambda s: int(s, 0), default=0x00C80000)
    p.add_argument('--ours-base', type=lambda s: int(s, 0), default=0x00C00000)
    p.add_argument('--fsu-base',  type=lambda s: int(s, 0), default=0x00C00000)
    args = p.parse_args()

    ours = open(args.ours, 'rb').read()
    fsu  = open(args.fsu, 'rb').read()

    print(f'OURS: {args.ours} ({len(ours)} bytes)')
    print(f'FSU:  {args.fsu} ({len(fsu)} bytes)')
    print()

    # Walk longword-aligned addresses in the requested range.
    diffs = 0
    matches = 0
    for addr in range(args.start, args.end, 4):
        oof = addr - args.ours_base
        fof = addr - args.fsu_base
        if oof < 0 or oof+4 > len(ours): continue
        if fof < 0 or fof+4 > len(fsu): continue
        o = struct.unpack('>I', ours[oof:oof+4])[0]
        f = struct.unpack('>I', fsu[fof:fof+4])[0]
        if o == f:
            matches += 1
            continue
        diffs += 1
        print(f'  ${addr:08X}: OURS=${o:08X}  FSU=${f:08X}'
              f'   [{classify(o):12s} vs {classify(f)}]')

    print()
    print(f'Total: {diffs} differing longwords, {matches} matching')


if __name__ == '__main__':
    main()
