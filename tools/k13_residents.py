#!/usr/bin/env python3
"""Scan a K1.3 ROM binary for AmigaOS Resident structures.

Each AmigaOS library / device / resource exposed by Kickstart has a
Resident struct laid out in ROM:

    +0  uint16  rt_MatchWord    = $4AFC (RTC_MATCHWORD)
    +2  uint32  rt_MatchTag     = pointer back to rt_MatchWord (self-pointer)
    +6  uint32  rt_EndSkip      = end of resident block
    +A  uint8   rt_Flags
    +B  uint8   rt_Version
    +C  uint8   rt_Type         (NT_LIBRARY=9, NT_DEVICE=3, NT_RESOURCE=8, NT_TASK=1)
    +D  int8    rt_Pri
    +E  uint32  rt_Name         (pointer to NUL-terminated C string)
    +12 uint32  rt_IdString     (pointer to NUL-terminated C string)
    +16 uint32  rt_Init         (Init code or pointer to InitStruct)

We scan ROM for $4AFC, verify rt_MatchTag points back to the match-word,
then decode the metadata.  Useful for identifying library/device entry
points when chasing K1.3 boot stalls.

Usage:
    python3 tools/k13_residents.py kickstart/kick_13.bin
        [--rombase 0xFC0000]   # default for 256K K1.3 ROM
        [--name input]          # filter by name substring (case-insens.)
"""
from __future__ import annotations

import argparse
import sys

NT_NAMES = {
    0:  'NT_UNKNOWN',
    1:  'NT_TASK',
    2:  'NT_INTERRUPT',
    3:  'NT_DEVICE',
    4:  'NT_MSGPORT',
    5:  'NT_MESSAGE',
    6:  'NT_FREEMSG',
    7:  'NT_REPLYMSG',
    8:  'NT_RESOURCE',
    9:  'NT_LIBRARY',
    10: 'NT_MEMORY',
    11: 'NT_SOFTINT',
    12: 'NT_FONT',
    13: 'NT_PROCESS',
    14: 'NT_SEMAPHORE',
    15: 'NT_SIGNALSEM',
    16: 'NT_BOOTNODE',
    17: 'NT_KICKMEM',
}


def be32(buf: bytes, off: int) -> int:
    return int.from_bytes(buf[off:off+4], 'big')


def be16(buf: bytes, off: int) -> int:
    return int.from_bytes(buf[off:off+2], 'big')


def read_cstr(rom: bytes, base: int, ptr: int, max_len: int = 80) -> str:
    """Read a NUL-terminated string from ROM at absolute address `ptr`."""
    off = ptr - base
    if off < 0 or off >= len(rom):
        return f'<ptr ${ptr:08X} out of ROM>'
    s = bytearray()
    while off < len(rom) and rom[off] != 0 and len(s) < max_len:
        s.append(rom[off])
        off += 1
    try:
        return s.decode('latin-1').strip()
    except Exception:
        return repr(bytes(s))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('rom')
    ap.add_argument('--rombase', default='0xFC0000')
    ap.add_argument('--name', default=None,
                    help='filter to residents whose name contains this string')
    args = ap.parse_args()

    rombase = int(args.rombase, 0)
    with open(args.rom, 'rb') as f:
        rom = f.read()
    romend = rombase + len(rom)

    print(f'Scanning {args.rom} ({len(rom)} bytes) at base ${rombase:08X}')
    print()
    print(f'{"ADDR":>10}  {"TYPE":<13} {"PRI":>3}  {"VER":>3} '
          f'{"INIT":>10}  NAME (IDSTRING)')
    print('-' * 78)

    count = 0
    off = 0
    while off + 0x1A <= len(rom):
        if be16(rom, off) == 0x4AFC:
            tag = be32(rom, off + 2)
            # The MatchTag must be a self-pointer (rombase + off).
            if tag == rombase + off:
                end_skip = be32(rom, off + 6)
                flags = rom[off + 0x0A]
                version = rom[off + 0x0B]
                rtype = rom[off + 0x0C]
                pri = rom[off + 0x0D]
                if pri >= 128:
                    pri -= 256
                name_ptr = be32(rom, off + 0x0E)
                idstr_ptr = be32(rom, off + 0x12)
                init = be32(rom, off + 0x16)

                name = read_cstr(rom, rombase, name_ptr)
                idstr = read_cstr(rom, rombase, idstr_ptr)

                if args.name and args.name.lower() not in name.lower():
                    off += 2
                    continue

                tn = NT_NAMES.get(rtype, f'NT_${rtype:02X}')
                print(f'${rombase + off:08X}  {tn:<13} {pri:>3}  {version:>3} '
                      f'${init:08X}  {name}  ({idstr})')
                count += 1
                # Skip past this resident — at least to the end of the struct.
                off = max(off + 0x1A, off + 2)
                continue
        off += 2

    print()
    print(f'Found {count} resident structures.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
