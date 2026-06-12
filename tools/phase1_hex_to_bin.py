#!/usr/bin/env python3
"""Convert $writememh output from minimig fullsys phase1 testbench to
raw chip+slow binary files for diffing against our m68k_top stack.

minimig fullsys SRAM layout (per tb/minimig_phase1_top.sv):
    sram[0x00000..0x3FFFF]  = chip RAM (512 KiB at $0..$7FFFF)
    sram[0x40000..0x7FFFF]  = slow RAM (512 KiB at $80000..$FFFFF)
    sram[0x180000..0x1BFFFF] = ROM (256 KiB at $FC0000..$FFFFFF)

Each SRAM entry is 16 bits, big-endian on the m68k bus.  $writememh
writes one 16-bit value per line as 4 hex chars.

Output files:
    --chip OUT.bin  → 512 KiB chip RAM dump (matches CHIPRAM_DUMP fmt)
    --slow OUT.bin  → 512 KiB slow RAM dump (matches SLOWRAM_DUMP fmt)

Usage:
    python3 tools/phase1_hex_to_bin.py build_phase1/phase1_sram.hex \\
        --chip /tmp/minimig_chip.bin --slow /tmp/minimig_slow.bin
"""
import argparse
import sys


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("hexfile")
    ap.add_argument("--chip", required=True, help="output: chip-RAM bin")
    ap.add_argument("--slow", required=True, help="output: slow-RAM bin")
    args = ap.parse_args()

    with open(args.hexfile) as f:
        words = []
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue
            words.append(int(line, 16) & 0xFFFF)

    print(f"loaded {len(words)} 16-bit words from {args.hexfile}")
    if len(words) < 0x80000:
        print(f"WARNING: only {len(words)} words, expected at least {0x80000:#x}")

    # Chip RAM: words[0..0x40000) = 256K words = 512 KiB
    chip_words = words[0:0x40000]
    chip_bytes = bytearray()
    for w in chip_words:
        chip_bytes.append((w >> 8) & 0xFF)
        chip_bytes.append(w & 0xFF)
    with open(args.chip, "wb") as f:
        f.write(chip_bytes)
    print(f"wrote {len(chip_bytes)} bytes to {args.chip}")

    # Slow RAM: words[0x40000..0x80000) = 256K words = 512 KiB
    slow_words = words[0x40000:0x80000]
    slow_bytes = bytearray()
    for w in slow_words:
        slow_bytes.append((w >> 8) & 0xFF)
        slow_bytes.append(w & 0xFF)
    with open(args.slow, "wb") as f:
        f.write(slow_bytes)
    print(f"wrote {len(slow_bytes)} bytes to {args.slow}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
