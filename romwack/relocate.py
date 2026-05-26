#!/usr/bin/env python3
"""Relocate an extracted ROMWack blob to a new memory address.

Takes the `<prefix>.bin` produced by romwack_extract.py and the matching
`<prefix>.json` manifest, then patches each recorded absolute-long
reference so it points to the new base.

Paula MMIO ($DFFxxx), CIA registers ($BFExxx), ExecBase ($00000004) and
all other off-window absolute addresses live OUTSIDE the extracted
window and therefore have no relocation entry -- they remain pointing at
their original hardware/ROM locations, which is what we want.

Usage:
    python3 tools/romwack_relocate.py build/romwack.bin \\
        --target 0x70000 --out build/romwack_at_70000.bin

After relocation, the printed entry-point list tells you where each
named routine landed; jump there from your own code to invoke ROMWack.
"""
from __future__ import annotations

import argparse
import json
import sys


def main() -> int:
    ap = argparse.ArgumentParser(description='Relocate extracted ROMWack to a new base.')
    ap.add_argument('blob', help='path to extracted .bin')
    ap.add_argument('--meta', default=None,
                    help='manifest .json (defaults to <blob>.json)')
    ap.add_argument('--target', required=True,
                    help='new base address (hex or dec)')
    ap.add_argument('--out', required=True, help='output .bin path')
    ap.add_argument('--quiet', action='store_true')
    args = ap.parse_args()

    target = int(args.target, 0)
    meta_path = args.meta
    if meta_path is None:
        if args.blob.endswith('.bin'):
            meta_path = args.blob[:-4] + '.json'
        else:
            meta_path = args.blob + '.json'

    with open(args.blob, 'rb') as f:
        blob = bytearray(f.read())
    with open(meta_path, 'r') as f:
        meta = json.load(f)

    src_base = meta['src_base']
    delta = target - src_base
    relocs = meta['relocs']

    for r in relocs:
        off = r['offset_in_blob']
        expected = r['target']
        cur = int.from_bytes(blob[off:off + 4], 'big')
        if cur != expected:
            raise SystemExit(
                f'reloc mismatch at +${off:04X}: blob has ${cur:08X}, '
                f'manifest expects ${expected:08X} (blob/manifest out of sync?)')
        new_val = (expected + delta) & 0xFFFFFFFF
        blob[off:off + 4] = new_val.to_bytes(4, 'big')

    with open(args.out, 'wb') as f:
        f.write(blob)

    if not args.quiet:
        print(f'patched {len(relocs)} relocations '
              f'(src ${src_base:08X} -> target ${target:08X})')
        print(f'wrote {args.out}  ({len(blob)} bytes)')
        if meta.get('entries'):
            print('entry points at new base:')
            for k, v in meta['entries'].items():
                print(f'  ${v + delta:08X}  {k}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
