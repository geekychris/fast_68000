#!/usr/bin/env python3
"""Decrypt an AmiKit-style "AMIROMTYPE1" encrypted Kickstart ROM.

File layout:
    bytes 0..10   "AMIROMTYPE1" header (11 bytes)
    bytes 11..N   encrypted ROM data, XORed with rom.key (repeating key)

The output is a raw flat ROM binary suitable for feeding into
tools/bin2rom.py.

Usage:
    rom_decrypt.py <kick.rom> <rom.key> <out.rom>
"""

import sys


def main() -> int:
    if len(sys.argv) != 4:
        print("usage: rom_decrypt.py <kick.rom> <rom.key> <out.rom>",
              file=sys.stderr)
        return 1

    rom_path, key_path, out_path = sys.argv[1], sys.argv[2], sys.argv[3]

    with open(rom_path, "rb") as f:
        blob = f.read()
    with open(key_path, "rb") as f:
        key = f.read()

    header = b"AMIROMTYPE1"
    if not blob.startswith(header):
        print(f"{rom_path}: missing AMIROMTYPE1 magic", file=sys.stderr)
        return 1

    enc = blob[len(header):]
    if len(key) == 0:
        print(f"{key_path}: empty key", file=sys.stderr)
        return 1

    out = bytearray(len(enc))
    for i, b in enumerate(enc):
        out[i] = b ^ key[i % len(key)]

    with open(out_path, "wb") as f:
        f.write(out)

    print(f"decrypted {len(out)} bytes -> {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
