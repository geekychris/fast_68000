#!/usr/bin/env python3
"""Convert a flat 68k binary (objcopy -O binary output) into the same
$readmemh format that tb/asm68k.py emits: one 32-bit word per line,
big-endian, indexed by (byte_addr >> 2).  The binary is assumed to be
loaded at --base (default 0x400); the head of memory ($0..base) is
zero-filled.

Usage:
    bin2hex.py <in.bin> <out.hex> [--base 0x400] [--mem-words 65536]
"""

import argparse
import struct
import sys


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("infile")
    ap.add_argument("outfile")
    ap.add_argument("--base", type=lambda s: int(s, 0), default=0x400)
    ap.add_argument("--mem-words", type=lambda s: int(s, 0), default=65536)
    args = ap.parse_args()

    if args.base % 4 != 0:
        print(f"base 0x{args.base:x} must be 4-byte aligned", file=sys.stderr)
        return 1

    with open(args.infile, "rb") as f:
        data = f.read()
    # Pad to multiple of 4 bytes so we always emit whole words.
    if len(data) % 4 != 0:
        data += b"\x00" * (4 - (len(data) % 4))

    base_word = args.base >> 2
    payload_words = len(data) // 4
    if base_word + payload_words > args.mem_words:
        print(
            f"image (base=0x{args.base:x}, size={len(data)} bytes) "
            f"overflows --mem-words=0x{args.mem_words:x} words "
            f"(= 0x{args.mem_words * 4:x} bytes)",
            file=sys.stderr,
        )
        return 1

    mem = [0] * args.mem_words
    for i in range(payload_words):
        (w,) = struct.unpack(">I", data[i * 4 : i * 4 + 4])
        mem[base_word + i] = w

    with open(args.outfile, "w") as f:
        f.write("\n".join(f"{w:08x}" for w in mem) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
