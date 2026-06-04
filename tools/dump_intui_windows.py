#!/usr/bin/env python3
"""dump_intui_windows.py — forensic dump of all Intuition Windows.

Scans chip + slow RAM for Intuition Window structs and prints each one's
title, position, flags, and Requester body IText.  Useful when our
bitplane renderer doesn't show the dialog cleanly (separate bug) but
the dialog IS logically present in chip RAM.

Usage:
    python3 tools/dump_intui_windows.py \\
        --chipram /tmp/wb_chipram.bin \\
        --slowram /tmp/wb_slowram.bin \\
        --rom kickstart/kick_13.bin
"""

import argparse
import struct

SLOW_BASE = 0x00C00000
ROM_BASE  = 0x00FC0000


def make_reader(chip, slow, rom):
    def read(addr, n):
        if 0 <= addr < len(chip) and addr + n <= len(chip):
            return chip[addr:addr + n]
        if SLOW_BASE <= addr < SLOW_BASE + len(slow) and addr + n <= SLOW_BASE + len(slow):
            return slow[addr - SLOW_BASE:addr - SLOW_BASE + n]
        if ROM_BASE <= addr < ROM_BASE + len(rom) and addr + n <= ROM_BASE + len(rom):
            return rom[addr - ROM_BASE:addr - ROM_BASE + n]
        return None

    def r8(a):
        b = read(a, 1)
        return b[0] if b else None

    def r16(a):
        b = read(a, 2)
        return struct.unpack('>H', b)[0] if b else None

    def r32(a):
        b = read(a, 4)
        return struct.unpack('>I', b)[0] if b else None

    def rstr(a, maxlen=80):
        s = b''
        while len(s) < maxlen:
            c = r8(a + len(s))
            if c is None or c == 0:
                break
            if c < 0x20 or c > 0x7E:
                return None
            s += bytes([c])
        return s.decode('ascii', errors='replace') if s else None

    return read, r8, r16, r32, rstr


def dump_window(addr, r8, r16, r32, rstr):
    """Decode a Window struct at addr and print its details."""
    title_ptr = r32(addr + 0x20)
    width = r16(addr + 0x08)
    height = r16(addr + 0x0A)
    flags = r32(addr + 0x18)
    title = rstr(title_ptr) if title_ptr else None
    if not title:
        return False

    # wd_UserPort at offset $56 (verified empirically vs $FDE3D0 BCPL handler)
    user_port = r32(addr + 0x56)

    first_req = r32(addr + 0x24)

    print(f"\n--- Window @ ${addr:08X} ---")
    print(f"  Title:     {title!r}")
    print(f"  Size:      {width}×{height}")
    print(f"  Flags:     ${flags:08X}")
    print(f"  UserPort:  ${user_port:08X}")
    if user_port:
        mp_sigbit = r8(user_port + 0x0F)
        mp_sigtask = r32(user_port + 0x10)
        mp_name_ptr = r32(user_port + 0x0A)
        mp_name = rstr(mp_name_ptr) if mp_name_ptr else None
        print(f"    mp_LN.Name:  {mp_name!r}")
        if mp_sigbit is None or mp_sigtask is None:
            print(f"    (UserPort unmapped or out-of-range — skipping signal fields)")
        else:
            print(f"    mp_SigBit:   ${mp_sigbit:02X} ({mp_sigbit})  ← Wait mask = 1<<{mp_sigbit} = ${1<<mp_sigbit:08X}")
            print(f"    mp_SigTask:  ${mp_sigtask:08X}")
            # Check if msg list is empty
            head = r32(user_port + 0x14)
            tail_pred = r32(user_port + 0x1C)
            if head is not None and tail_pred is not None:
                empty = head == user_port + 0x18 and tail_pred == user_port + 0x14
                print(f"    msgs pending: {'NO (empty)' if empty else 'YES'}")

    if first_req:
        print(f"  FirstRequest: ${first_req:08X}")
        # Requester struct: ReqText at +$18
        req_text = r32(first_req + 0x18)
        print(f"    ReqText IText chain (the body text):")
        while req_text:
            t_str_ptr = r32(req_text + 0x0C)
            t_str = rstr(t_str_ptr) if t_str_ptr else None
            left = r16(req_text + 0x04)
            top = r16(req_text + 0x06)
            if t_str_ptr is None or left is None or top is None:
                break
            print(f"      @({left:>3},{top:>3}) [{t_str_ptr:08X}] {t_str!r}")
            req_text = r32(req_text + 0x10)  # NextText
            if req_text == 0:
                break

        # ReqGadget for button labels
        req_gadget = r32(first_req + 0x10)
        if req_gadget:
            print(f"    Gadgets (buttons):")
            while req_gadget:
                g_text_ptr = r32(req_gadget + 0x1A)
                if g_text_ptr:
                    g_str_ptr = r32(g_text_ptr + 0x0C)
                    g_str = rstr(g_str_ptr) if g_str_ptr else None
                    print(f"      [{g_str!r}]")
                req_gadget = r32(req_gadget + 0x00)
                if req_gadget == 0:
                    break

    return True


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--chipram', default='/tmp/wb_chipram.bin')
    ap.add_argument('--slowram', default='/tmp/wb_slowram.bin')
    ap.add_argument('--rom',      default='/Users/chris/code/claude_world/68000/kickstart/kick_13.bin')
    args = ap.parse_args()

    chip = open(args.chipram, 'rb').read()
    slow = open(args.slowram, 'rb').read()
    rom = open(args.rom, 'rb').read()
    _, r8, r16, r32, rstr = make_reader(chip, slow, rom)

    # Walk every possible Intuition Window struct address.  Heuristic:
    # we scan chip+slow RAM for word-aligned 4-byte values pointing to
    # a string in ROM (typical Window.Title).  Any address whose +$20
    # field points to a non-empty ASCII string in $FC0000..$FFFFFF
    # is a candidate.
    candidates = []
    for buf_name, buf, base in [('chip', chip, 0), ('slow', slow, SLOW_BASE)]:
        for off in range(0, len(buf) - 0x60, 2):
            t = struct.unpack('>I', buf[off + 0x20:off + 0x24])[0]
            # Window.Title can live in ROM (Kickstart-supplied titles like
            # "Workbench") or in slow RAM (titles from disk-loaded DOS
            # commands like CON: "AmigaDOS").  Accept both.
            in_rom  = ROM_BASE <= t < ROM_BASE + len(rom)
            in_slow = SLOW_BASE <= t < SLOW_BASE + len(slow)
            if in_rom or in_slow:
                s = rstr(t)
                if s and len(s) >= 3 and len(s) <= 40:
                    # Verify width/height look like reasonable Intuition values
                    w = struct.unpack('>H', buf[off + 8:off + 10])[0]
                    h = struct.unpack('>H', buf[off + 10:off + 12])[0]
                    if 50 <= w <= 800 and 30 <= h <= 400:
                        candidates.append(base + off)

    print(f"Scanning chip + slow RAM for Intuition Windows...")
    print(f"Found {len(candidates)} candidate window struct(s):")
    for a in candidates:
        dump_window(a, r8, r16, r32, rstr)


if __name__ == '__main__':
    main()
