#!/usr/bin/env python3
"""Walk an Exec MsgPort + its message list from a chipram dump.

Usage:
    python3 tools/port_walk.py chipram.bin <port_addr>

Prints port fields (Flags, SigBit, SigTask, list head/tail/tailpred) and
walks the mp_MsgList chain (mp_MsgList at port+$14 per <exec/ports.h>).

For each Message, prints node fields + mn_Length, mn_ReplyPort, the
following few longwords (usually the IORequest body), and the IORequest's
io_Command / io_Error.
"""
import sys

if len(sys.argv) < 3:
    print(f'usage: {sys.argv[0]} chipram.bin <port_addr_hex>',
          file=sys.stderr)
    sys.exit(2)

with open(sys.argv[1], 'rb') as f:
    chip = f.read()
port = int(sys.argv[2], 0)


def L(addr):
    if addr + 4 > len(chip):
        return 0
    return int.from_bytes(chip[addr:addr+4], 'big')


def W(addr):
    if addr + 2 > len(chip):
        return 0
    return int.from_bytes(chip[addr:addr+2], 'big')


def B(addr):
    if addr >= len(chip):
        return 0
    return chip[addr]


def cstr(addr, n=48):
    s = b''
    for i in range(n):
        if addr + i >= len(chip):
            break
        c = chip[addr+i:addr+i+1]
        if c == b'\x00':
            break
        s += c
    try:
        return s.decode('latin-1')
    except Exception:
        return repr(s)


# struct MsgPort offsets:
#   $00 ln_Succ / $04 ln_Pred / $08 ln_Type ($04=PORT) / $09 ln_Pri
#   $0A ln_Name (L)
#   $0E mp_Flags / $0F mp_SigBit / $10 mp_SigTask (L) / $14 mp_MsgList (LH)
#   list layout: lh_Head($14), lh_Tail($18=NULL), lh_TailPred($1C)
name_ptr = L(port + 0x0A)
flags    = B(port + 0x0E)
sigbit   = B(port + 0x0F)
sigtask  = L(port + 0x10)
mlh_head = L(port + 0x14)
mlh_tail = L(port + 0x18)
mlh_tpred= L(port + 0x1C)

name = cstr(name_ptr) if name_ptr else '<no name>'
print(f'Port ${port:08X}  "{name}"')
print(f'  ln_Type=${B(port+8):02X} ln_Pri={B(port+9)}')
print(f'  mp_Flags=${flags:02X} mp_SigBit={sigbit} ($1<<{sigbit}=${1<<sigbit:08X}) '
      f'mp_SigTask=${sigtask:08X}')
print(f'  mp_MsgList: head=${mlh_head:08X} tail=${mlh_tail:08X} '
      f'tailpred=${mlh_tpred:08X}')
empty = (mlh_head == port + 0x18)  # head points at the sentinel tail
print(f'  empty? {"yes" if empty else "no"}')

# Walk the list.
seen = set()
cur = mlh_head
i = 0
while cur and cur != port + 0x18 and cur not in seen:
    seen.add(cur)
    # struct Message:
    #   $00 ln_Succ / $04 ln_Pred / $08 ln_Type ($05=MSG / $06=FREEMSG / $07=REPLYMSG)
    #   $0E mn_ReplyPort / $12 mn_Length
    msg_succ  = L(cur)
    msg_type  = B(cur + 8)
    msg_rport = L(cur + 0x0E)
    msg_len   = W(cur + 0x12)
    print(f'  Msg#{i:2d} ${cur:08X} type=${msg_type:02X} '
          f'reply=${msg_rport:08X} len={msg_len}')
    # If it's an IORequest, $14 io_Device, $18 io_Unit, $1C io_Command,
    # $1E io_Flags, $1F io_Error.
    if msg_len >= 0x20:
        io_device = L(cur + 0x14)
        io_unit   = L(cur + 0x18)
        io_cmd    = W(cur + 0x1C)
        io_flags  = B(cur + 0x1E)
        io_err    = B(cur + 0x1F)
        print(f'      io_Device=${io_device:08X} io_Unit=${io_unit:08X} '
              f'io_Command={io_cmd} io_Flags=${io_flags:02X} '
              f'io_Error={io_err}')
    cur = msg_succ
    i += 1
    if i > 64:
        print('  ...too many, stopping')
        break

print(f'  total messages: {i}')
