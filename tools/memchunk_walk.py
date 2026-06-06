#!/usr/bin/env python3
"""Walk the exec.library MemHeader free-list chain in a chip-RAM dump.

For diagnosing CHIP-RAM free-list corruption (per
project_boing_chip_freelist.md): given a chip-RAM .bin and a slow-RAM
.bin, walks the MemHeader list at ExecBase+0x142, then walks each
MemHeader's MH_FIRST → MC_NEXT chain, and prints what's reachable vs
what MH_FREE says should be reachable.

Optionally compares two snapshots to find which chunks went MISSING
between them — that's the orphaning instant.

Usage:
    memchunk_walk.py --chipram /tmp/boing_chip.bin --slowram /tmp/boing_slow.bin
    memchunk_walk.py --diff   /tmp/before.bin /tmp/after.bin --slowram /tmp/slow.bin
"""

import argparse
import sys


def load(path):
    return open(path, 'rb').read()


def read_u32(chip, slow, addr):
    if addr < 0x80000:
        return int.from_bytes(chip[addr:addr+4], 'big')
    elif 0xC00000 <= addr < 0xC00000 + len(slow):
        off = addr - 0xC00000
        return int.from_bytes(slow[off:off+4], 'big')
    return None


def walk_memheaders(chip, slow):
    """Returns [(mh_addr, attrib, mh_first, mh_lower, mh_upper, mh_free), ...]"""
    exec_base = read_u32(chip, slow, 4)
    mh_list_head = exec_base + 0x142
    head = read_u32(chip, slow, mh_list_head)
    results = []
    seen = set()
    node = head
    while node and node not in seen and node != 0:
        seen.add(node)
        succ   = read_u32(chip, slow, node)
        attrib = int.from_bytes(
            chip[node+0xE:node+0x10] if node < 0x80000
            else slow[node-0xC00000+0xE:node-0xC00000+0x10],
            'big')
        first  = read_u32(chip, slow, node + 0x10)
        lower  = read_u32(chip, slow, node + 0x14)
        upper  = read_u32(chip, slow, node + 0x18)
        free   = read_u32(chip, slow, node + 0x1C)
        results.append((node, attrib, first, lower, upper, free))
        if succ == node or succ == 0:
            break
        node = succ
    return results


def walk_chain(chip, slow, first, max_chunks=2000):
    """Returns [(addr, mc_next, mc_bytes), ...] for free chunks reachable from `first`."""
    chunks = []
    seen = set()
    cur = first
    while cur and cur not in seen and len(chunks) < max_chunks:
        seen.add(cur)
        nxt = read_u32(chip, slow, cur)
        sz  = read_u32(chip, slow, cur + 4)
        if nxt is None or sz is None:
            break
        # Sanity: a chunk header should have plausible MC_NEXT (0 or in valid memory)
        # and plausible MC_BYTES (multiple of 8, less than 2 MB).
        if sz > 2*1024*1024 or sz % 8 != 0:
            chunks.append((cur, nxt, sz, 'CORRUPT'))
            break
        chunks.append((cur, nxt, sz, 'ok'))
        if nxt == 0:
            break
        cur = nxt
    return chunks


def dump_state(label, chip, slow):
    print(f'\n=== {label} ===')
    headers = walk_memheaders(chip, slow)
    chip_chain = None
    for mh, attrib, first, lower, upper, free in headers:
        kind = ('CHIP' if attrib & 0x02 else
                'FAST' if attrib & 0x04 else f'?{attrib:04X}')
        print(f'\n  MemHeader ${mh:08X}: attrib=${attrib:04X} ({kind}) '
              f'lower=${lower:08X} upper=${upper:08X} MH_FREE={free}')
        chain = walk_chain(chip, slow, first)
        total = sum(c[2] for c in chain if c[3] == 'ok')
        print(f'  Chain (MH_FIRST=${first:08X}): {len(chain)} chunks, '
              f'reachable={total} bytes')
        for i, (addr, nxt, sz, st) in enumerate(chain):
            if i < 10 or i >= len(chain) - 3:
                marker = ' ← CORRUPT' if st != 'ok' else ''
                print(f'      ${addr:08X}  size={sz:8d}  next=${nxt:08X}{marker}')
            elif i == 10:
                print(f'      ... {len(chain) - 13} more ...')
        if total != free:
            delta = free - total
            print(f'  ⚠  ORPHANED: MH_FREE={free} but reachable={total} '
                  f'(delta={delta} bytes lost from chain)')
        if kind == 'CHIP':
            chip_chain = chain
    return chip_chain or []


def diff_chains(a_chain, b_chain):
    """Print chunks that disappeared between snapshot A and B."""
    a_addrs = {c[0] for c in a_chain}
    b_addrs = {c[0] for c in b_chain}
    gone = a_addrs - b_addrs
    arrived = b_addrs - a_addrs
    print(f'\n  Chunks present in A but not in B (i.e. consumed or orphaned):'
          f' {len(gone)}')
    a_by_addr = {c[0]: c for c in a_chain}
    for addr in sorted(gone):
        c = a_by_addr[addr]
        print(f'    ${addr:08X}  size={c[2]}')
    print(f'\n  Chunks present in B but not in A (newly freed):'
          f' {len(arrived)}')
    b_by_addr = {c[0]: c for c in b_chain}
    for addr in sorted(arrived):
        c = b_by_addr[addr]
        print(f'    ${addr:08X}  size={c[2]}')


def main():
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--chipram', help='chip-RAM .bin')
    ap.add_argument('--slowram', help='slow-RAM .bin', required=True)
    ap.add_argument('--diff', nargs=2, metavar=('BEFORE', 'AFTER'),
                    help='diff two chip-RAM snapshots')
    args = ap.parse_args()

    slow = load(args.slowram)

    if args.diff:
        a = load(args.diff[0])
        b = load(args.diff[1])
        a_chain = dump_state(f'BEFORE  ({args.diff[0]})', a, slow)
        b_chain = dump_state(f'AFTER   ({args.diff[1]})', b, slow)
        diff_chains(a_chain, b_chain)
    elif args.chipram:
        chip = load(args.chipram)
        dump_state(args.chipram, chip, slow)
    else:
        ap.print_help()
        sys.exit(2)


if __name__ == '__main__':
    main()
