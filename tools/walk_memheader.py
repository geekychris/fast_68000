#!/usr/bin/env python3
"""walk_memheader.py — walk the Amiga chip MemHeader at $400 and dump
its free-list.  Also checks whether a probe address falls inside an
allocated or free region.

Usage:
    python3 tools/walk_memheader.py <chipram.bin> [--probe ADDR]
"""
import argparse, struct, sys


def u32(b, o):
    return struct.unpack('>I', b[o:o+4])[0]


def u16(b, o):
    return struct.unpack('>H', b[o:o+2])[0]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('chipram')
    ap.add_argument('--mh-addr', type=lambda s: int(s, 16), default=0x400,
                    help='MemHeader address (default $400)')
    ap.add_argument('--probe', type=lambda s: int(s, 16), default=None,
                    help='check whether address is free / allocated')
    args = ap.parse_args()

    mem = open(args.chipram, 'rb').read()
    if len(mem) < 0x80000:
        print(f'WARNING: chipram only {len(mem)} bytes', file=sys.stderr)

    mh = args.mh_addr
    print(f'=== MemHeader at ${mh:08X} ===')
    ln_succ = u32(mem, mh + 0)
    ln_pred = u32(mem, mh + 4)
    ln_type = mem[mh + 8]
    ln_pri = struct.unpack('b', mem[mh+9:mh+10])[0]
    ln_name = u32(mem, mh + 10)
    mh_attrs = u16(mem, mh + 14)
    mh_first = u32(mem, mh + 16)
    mh_lower = u32(mem, mh + 20)
    mh_upper = u32(mem, mh + 24)
    mh_free = u32(mem, mh + 28)

    print(f'  ln_Succ       = ${ln_succ:08X}')
    print(f'  ln_Pred       = ${ln_pred:08X}')
    print(f'  ln_Type       = ${ln_type:02X}')
    print(f'  ln_Pri        = {ln_pri}')
    print(f'  ln_Name       = ${ln_name:08X}')
    print(f'  mh_Attributes = ${mh_attrs:04X}')
    print(f'  mh_First      = ${mh_first:08X}')
    print(f'  mh_Lower      = ${mh_lower:08X}')
    print(f'  mh_Upper      = ${mh_upper:08X}')
    print(f'  mh_Free       = ${mh_free:08X} ({mh_free} bytes)')

    print()
    print(f'=== Free chunk chain ===')
    chunks = []
    cur = mh_first
    total = 0
    safety = 0
    while cur != 0 and safety < 1000:
        if cur < mh_lower or cur >= mh_upper:
            print(f'  ${cur:08X}: OUT OF RANGE (mh_Lower=${mh_lower:08X} '
                  f'mh_Upper=${mh_upper:08X})')
            break
        if cur >= len(mem):
            print(f'  ${cur:08X}: BEYOND DUMP SIZE ${len(mem):08X}')
            break
        mc_next = u32(mem, cur)
        mc_bytes = u32(mem, cur + 4)
        end = cur + mc_bytes
        print(f'  chunk @ ${cur:08X}  size=${mc_bytes:08X} ({mc_bytes:>6d} bytes)  '
              f'covers ${cur:08X}..${end-1:08X}  next=${mc_next:08X}')
        chunks.append((cur, mc_bytes, end))
        total += mc_bytes
        cur = mc_next
        safety += 1
    if safety >= 1000:
        print(f'  WARN: chain >1000 chunks, aborting (probably corrupt)')
    print(f'  Total free: ${total:08X} ({total} bytes)')
    print(f'  mh_Free says: ${mh_free:08X} ({mh_free} bytes)')
    if total != mh_free:
        print(f'  *** mismatch: walker={total} vs mh_Free={mh_free} ***')

    if args.probe is not None:
        print()
        print(f'=== Probe ${args.probe:08X} ===')
        if args.probe < mh_lower or args.probe >= mh_upper:
            print(f'  not inside heap (mh_Lower=${mh_lower:08X} '
                  f'mh_Upper=${mh_upper:08X})')
            return
        in_free = False
        for start, size, end in chunks:
            if start <= args.probe < end:
                in_free = True
                print(f'  ${args.probe:08X} is FREE')
                print(f'    inside chunk @ ${start:08X} size=${size:08X} '
                      f'(offset +${args.probe-start:X})')
                break
        if not in_free:
            print(f'  ${args.probe:08X} is ALLOCATED')
            # Find the surrounding chunk gap
            prev_end = mh_lower
            for start, size, end in chunks:
                if prev_end <= args.probe < start:
                    print(f'    inside allocated region '
                          f'${prev_end:08X}..${start-1:08X} '
                          f'(size ${start-prev_end:X})')
                    break
                prev_end = end
            else:
                print(f'    inside allocated region '
                      f'${prev_end:08X}..${mh_upper-1:08X} (tail)')


if __name__ == '__main__':
    main()
