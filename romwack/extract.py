#!/usr/bin/env python3
"""Extract the ROMWack serial debugger from a Kickstart 1.3 ROM.

ROMWack is the 9600-baud monitor that K1.3's `exec.Debug()` (and the
Guru/Alert path) drop into.  Its code lives at fixed ROM addresses
inside the 256 KB Kickstart image at $FC0000-$FFFFFF; this tool slices
out the relevant code + data ranges and records every absolute-long
reference inside that slice so a sibling relocator can move it.

Outputs:
    <out>.bin   - raw extracted bytes (concatenation of selected
                  ranges, with the gap between them filled in-place
                  from ROM to keep offsets simple)
    <out>.json  - manifest: src_base, length, named entry points,
                  relocation list
    <out>.txt   - human-readable report (entries, relocs w/ context)

Relocations recorded are only the absolute-long operands of:
    LEA  abs.l, An       (opcodes $41F9..$4FF9)
    PEA  abs.l           ($4879)
    JSR  abs.l           ($4EB9)
    JMP  abs.l           ($4EF9)
    MOVE.L #imm32, <ea>  ($2X3C/$2XBC/$2XFC/$2X7C... with src=#imm32)

Hardware addresses (Paula $DFFxxx, CIA $BFExxx, low-RAM vectors,
ExecBase pointer at $00000004) all fall OUTSIDE the extracted window
and are therefore left alone naturally.
"""
from __future__ import annotations

import argparse
import json
import os
import sys
from dataclasses import dataclass, field, asdict


# --- known entry points inside ROMWack (K1.3 v34.5) ----------------------

ENTRIES = {
    'cpu_detect':         0xFC0546,  # returns 0=68000, !0 on 68010+/FPU
    'romwack_main':       0xFC239A,  # MOVEM-all entry from Alert path
    'romwack_trap_setup': 0xFC2336,  # install $42 and $190 vectors + SERPER
    'banner_string':      0xFC232C,  # "\nrom-wack\0"
    'serper_init':        0xFC2234,  # write SERPER=$0174 (9600 baud)
    'getc':               0xFC223E,  # poll SERDATR, return -1 if empty
    'getc_block':         0xFC225E,  # block until char
    'putc':               0xFC2266,  # write SERDAT, CR->CRLF
    'print_string':       0xFC22B4,  # A0 = NUL-terminated string
    'print_hex':          0xFC22CA,  # D0 = value, D1 = digit count
    'printf':             0xFC2124,  # A0=fmt, A2=putc-vector
    'powers10_table':     0xFC20C0,  # 1e9, 1e8, ..., 1, 0
    'strlen':             0xFC2048,
    'upcase_char':        0xFC2D84,  # 'a'..'z' -> 'A'..'Z' in D0
    'parse_hex':          0xFC2D38,  # parse a hex/decimal number from string
    'unknown_symbol_msg': 0xFC2D26,  # "\nunknown symbol\0"
    'cmd_names':          0xFC33E2,  # "alter\0boot\0clear\0..."
    'cmd_dispatch_head':  0xFC3430,  # linked list of cmd nodes
}

# Default extraction ranges -- each window has a scan mode:
#   'code'    -> scan for opcode forms whose operand is one abs.l
#   'data_l'  -> treat every word-aligned 4-byte value as a possible pointer
#   'none'    -> do not scan (e.g. raw string tables)
# All ranges are concatenated into a single contiguous blob keeping
# offsets = (orig_addr - min_lo); gaps between non-contiguous ranges are
# filled with the ROM bytes that live there (dead weight but harmless).
#
# The 'cpu_detect' range pulls in the small helper at $FC0546 that
# ROMWack's main entry calls to probe for 68010+ / FPU; including it
# means the relocated copy doesn't fall back into ROM for that probe.
DEFAULT_RANGES = [
    (0xFC0546, 0xFC0592, 'cpu_detect',   'code'),
    (0xFC2048, 0xFC2DA6, 'code',         'code'),
    (0xFC33E2, 0xFC3430, 'cmd_names',    'none'),
    (0xFC3430, 0xFC3508, 'cmd_dispatch', 'data_l'),
]


# --- opcode patterns containing a 32-bit absolute operand ----------------

def _build_absl_opcodes() -> dict[int, str]:
    """Map opcode-word -> mnemonic for instructions whose operand is
    one 32-bit absolute address starting at offset +2 from the opcode."""
    m: dict[int, str] = {
        0x4879: 'PEA',
        0x4EB9: 'JSR',
        0x4EF9: 'JMP',
    }
    # LEA abs.l, An -> $41F9..$4FF9 step $200
    for an in range(8):
        m[0x41F9 | (an << 9)] = f'LEA.A{an}'
    return m


def _build_move_l_imm_opcodes() -> set[int]:
    """MOVE.L or MOVEA.L with #imm32 source.

    Both share the bit layout `0010 dst_reg(3) dst_mode(3) 111100`; dst_mode=1
    is MOVEA.L (legal for L size, illegal for B).  The 4-byte immediate
    follows the opcode word."""
    s: set[int] = set()
    for dst_reg in range(8):
        for dst_mode in range(8):
            s.add(0x2000 | (dst_reg << 9) | (dst_mode << 6) | 0x3C)
    return s


ABSL_OPCODES = _build_absl_opcodes()
MOVE_L_IMM_OPCODES = _build_move_l_imm_opcodes()


# --- tiny BE helpers -----------------------------------------------------

def be32(buf: bytes, off: int) -> int:
    return int.from_bytes(buf[off:off + 4], 'big')

def be16(buf: bytes, off: int) -> int:
    return int.from_bytes(buf[off:off + 2], 'big')


# --- core ----------------------------------------------------------------

@dataclass
class Reloc:
    offset_in_blob: int
    src_addr: int   # absolute ROM addr of the 4 bytes being relocated
    target: int     # absolute ROM addr that the 4 bytes encode
    kind: str       # mnemonic ('JSR', 'LEA.A0', etc.)


def in_ranges(addr: int, ranges) -> bool:
    return any(lo <= addr < hi for lo, hi, *_ in ranges)


def scan(rom: bytes, rombase: int, ranges):
    min_lo = min(r[0] for r in ranges)
    max_hi = max(r[1] for r in ranges)
    blob_len = max_hi - min_lo
    blob = bytearray(rom[min_lo - rombase:max_hi - rombase])

    relocs: list[Reloc] = []
    seen: set[int] = set()  # offset_in_blob -- dedupe between scans

    for lo, hi, _label, mode in ranges:
        if mode == 'none':
            continue

        if mode == 'code':
            addr = lo
            while addr + 6 <= hi:
                off = addr - rombase
                opcode = be16(rom, off)
                kind = ABSL_OPCODES.get(opcode)
                if kind is None and opcode in MOVE_L_IMM_OPCODES:
                    kind = f'MOVE.L#${opcode:04X}'
                if kind is not None:
                    v = be32(rom, off + 2)
                    if in_ranges(v, ranges):
                        rel_off = (addr + 2) - min_lo
                        if rel_off not in seen:
                            seen.add(rel_off)
                            relocs.append(Reloc(
                                offset_in_blob=rel_off,
                                src_addr=addr + 2,
                                target=v,
                                kind=kind,
                            ))
                addr += 2

        elif mode == 'data_l':
            # Scan word-aligned positions (stride 2) and emit a reloc when
            # the 4 bytes there encode an address inside our extracted
            # window.  Stride 2 (not 4) is needed because some records
            # in K1.3 ROMWack -- e.g. the dispatch-list node -- are 14
            # bytes long and put pointer fields at offsets 0, 4 and 10.
            for addr in range(lo, hi - 3, 2):
                v = be32(rom, addr - rombase)
                if in_ranges(v, ranges):
                    rel_off = addr - min_lo
                    if rel_off not in seen:
                        seen.add(rel_off)
                        relocs.append(Reloc(
                            offset_in_blob=rel_off,
                            src_addr=addr,
                            target=v,
                            kind='DATA.L',
                        ))
        else:
            raise ValueError(f'unknown scan mode: {mode!r}')

    relocs.sort(key=lambda r: r.offset_in_blob)
    return blob, blob_len, min_lo, relocs


def main() -> int:
    ap = argparse.ArgumentParser(description='Extract ROMWack from K1.3 ROM.')
    ap.add_argument('rom', help='path to kick_13.bin (256 KB)')
    ap.add_argument('--rombase', default='0xFC0000')
    ap.add_argument('--out', default='romwack/build/extracted',
                    help='output prefix; writes <out>.bin/.json/.txt')
    ap.add_argument('--range', action='append', default=None,
                    metavar='LO:HI:LABEL:MODE',
                    help='override default extraction range; hex addresses; '
                         'MODE in {code,data_l,none}; repeatable')
    args = ap.parse_args()

    rombase = int(args.rombase, 0)
    with open(args.rom, 'rb') as f:
        rom = f.read()

    if args.range:
        ranges = []
        for r in args.range:
            parts = r.split(':')
            if len(parts) != 4:
                raise SystemExit(f'--range expects LO:HI:LABEL:MODE, got {r!r}')
            lo, hi, lbl, mode = parts
            ranges.append((int(lo, 0), int(hi, 0), lbl, mode))
    else:
        ranges = DEFAULT_RANGES

    blob, blob_len, base_addr, relocs = scan(rom, rombase, ranges)

    out_dir = os.path.dirname(args.out)
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
    bin_path = args.out + '.bin'
    json_path = args.out + '.json'
    txt_path = args.out + '.txt'

    with open(bin_path, 'wb') as f:
        f.write(blob)

    meta = {
        'src_rom': args.rom,
        'rombase': rombase,
        'src_base': base_addr,
        'blob_length': blob_len,
        'ranges': [{'lo': lo, 'hi': hi, 'label': lbl, 'mode': mode}
                   for lo, hi, lbl, mode in ranges],
        'entries': {k: v for k, v in ENTRIES.items() if in_ranges(v, ranges)},
        'relocs': [asdict(r) for r in relocs],
    }
    with open(json_path, 'w') as f:
        json.dump(meta, f, indent=2)

    with open(txt_path, 'w') as f:
        f.write('ROMWack extraction\n')
        f.write(f'  src       : {args.rom}\n')
        f.write(f'  src_base  : ${base_addr:08X}\n')
        f.write(f'  blob_len  : {blob_len} bytes (${blob_len:X})\n\n')
        f.write('Ranges:\n')
        for lo, hi, lbl, mode in ranges:
            f.write(f'  ${lo:08X}-${hi:08X}  {lbl:<14} [{mode}]  ({hi - lo} bytes)\n')
        f.write('\nNamed entry points (offsets are within blob):\n')
        for k, v in meta['entries'].items():
            f.write(f'  ${v:08X}  +${v - base_addr:04X}  {k}\n')
        f.write(f'\nRelocations ({len(relocs)}):\n')
        for r in relocs:
            f.write(f'  +${r.offset_in_blob:04X}  @${r.src_addr:08X}  '
                    f'{r.kind:<10} -> ${r.target:08X}\n')

    print(f'wrote {bin_path}  ({blob_len} bytes)')
    print(f'wrote {json_path} ({len(relocs)} relocs)')
    print(f'wrote {txt_path}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
