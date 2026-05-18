#!/usr/bin/env python3
"""Convert an arbitrary binary ROM image (e.g. a Kickstart .rom or any
other flat 68k boot image) into the $readmemh format the bus loads via
the DISK_HEXFILE / MEM_HEXFILE_ROM parameter.

The output is one 32-bit big-endian word per line, indexed by
(byte_offset >> 2).  Padding to --mem-words words; if the input is
larger, raises.

Usage:
    bin2rom.py <in.rom> <out.hex> [--mem-words N]
"""

import argparse
import struct
import sys


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("infile")
    ap.add_argument("outfile")
    ap.add_argument("--mem-words", type=lambda s: int(s, 0), default=65536)
    args = ap.parse_args()

    with open(args.infile, "rb") as f:
        data = f.read()
    if len(data) % 4 != 0:
        data += b"\x00" * (4 - (len(data) % 4))

    n_words = len(data) // 4
    if n_words > args.mem_words:
        print(
            f"binary is {len(data)} bytes (= {n_words} words) but "
            f"--mem-words is {args.mem_words}",
            file=sys.stderr,
        )
        return 1

    mem = [0] * args.mem_words
    for i in range(n_words):
        (mem[i],) = struct.unpack(">I", data[i * 4 : i * 4 + 4])

    with open(args.outfile, "w") as f:
        f.write("\n".join(f"{w:08x}" for w in mem) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
