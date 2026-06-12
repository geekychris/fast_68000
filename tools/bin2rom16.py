#!/usr/bin/env python3
"""Convert a ROM binary to 16-bit-per-line hex for $readmemh into a
16-bit-wide memory array (e.g. minimig fullsys phase1's 2 MiB SRAM).

This is the 16-bit-word companion to tools/bin2rom.py (which emits
32-bit longwords for our 32-bit-wide rom[] arrays in m68k_top).

Usage:
    python3 tools/bin2rom16.py kickstart/kick_13.bin out.hex
"""
import argparse
import struct
import sys


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("infile")
    ap.add_argument("outfile")
    args = ap.parse_args()

    with open(args.infile, "rb") as f:
        data = f.read()
    if len(data) % 2 != 0:
        data += b"\x00"

    n_words = len(data) // 2

    with open(args.outfile, "w") as f:
        for i in range(n_words):
            (w,) = struct.unpack(">H", data[i * 2 : i * 2 + 2])
            f.write(f"{w:04x}\n")

    print(f"wrote {n_words} 16-bit words to {args.outfile}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
