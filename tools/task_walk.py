#!/usr/bin/env python3
"""Walk Exec's task lists from a chipram dump (produced by CHIPRAM_DUMP=).

Reads ExecBase from $00000004, then traverses the TaskReady and TaskWait
lists (at ExecBase + 0x196 and + 0x1A4 respectively), printing each task's
name, priority, state, sigWait, and sigRecvd.

Task names that point into ROM ($00FC0000+) cannot be resolved from the
chipram alone -- the script reports the address.  Cross-reference against
`kickstart/kick_13.bin` to find the string (offset = name_ptr - $FC0000
for K1.3, or $F80000 for K1.3 in upper ROM).

Useful for diagnosing K1.3 boot stalls where a specific task is stuck in
WAIT for a signal bit that never arrives.

Usage:
    CHIPRAM_DUMP=dump.bin ./Vm68k_top 1500000000   # generate dump
    python3 tools/task_walk.py dump.bin            # inspect
"""
import sys

if len(sys.argv) < 2:
    print(f'usage: {sys.argv[0]} chipram_dump.bin [task_addr]', file=sys.stderr)
    sys.exit(2)

with open(sys.argv[1], 'rb') as f:
    chip = f.read()


def read_long(addr):
    if addr + 4 > len(chip):
        return 0
    return int.from_bytes(chip[addr:addr+4], 'big')


def read_string(addr, max_len=64):
    if addr >= len(chip):
        return f'<ROM @${addr:08X}>'
    s = b''
    for i in range(max_len):
        if addr + i >= len(chip):
            break
        b = chip[addr+i:addr+i+1]
        if b == b'\x00':
            break
        s += b
    try:
        return s.decode('latin-1')
    except Exception:
        return repr(s)


STATE_NAMES = {0:'invalid', 1:'added', 2:'run', 3:'ready',
               4:'wait', 5:'except', 6:'removed'}


def dump_task(addr):
    succ = read_long(addr)
    pri  = chip[addr+9] if addr+9 < len(chip) else 0
    if pri >= 128:
        pri -= 256
    name_ptr  = read_long(addr + 0x0A)
    state     = chip[addr + 0x0F] if addr + 0x0F < len(chip) else 0
    sig_wait  = read_long(addr + 0x18)
    sig_recvd = read_long(addr + 0x1C)
    sig_alloc = read_long(addr + 0x14)
    name = read_string(name_ptr)
    print(f'  ${addr:08X} pri={pri:>4d} state={STATE_NAMES.get(state, "?"):<8} '
          f'sigWait=${sig_wait:08X} sigRecvd=${sig_recvd:08X} '
          f'sigAlloc=${sig_alloc:08X}  "{name}"')
    return succ


def walk_list(list_addr, label):
    print(f'\n{label} (List @ ${list_addr:08X}):')
    head = read_long(list_addr)
    seen = set()
    while head and (head & ~0xFFFFF) == 0 and head not in seen:
        seen.add(head)
        succ = dump_task(head)
        if succ == 0:
            break
        head = succ


if len(sys.argv) >= 3:
    # Specific task requested.
    dump_task(int(sys.argv[2], 0))
else:
    exec_base = read_long(4)
    print(f'ExecBase = ${exec_base:08X}')
    walk_list(exec_base + 0x196, 'TaskReady')
    walk_list(exec_base + 0x1A4, 'TaskWait')
