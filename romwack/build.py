#!/usr/bin/env python3
"""Build the small standalone Wack monitor (romwack/wack.s).

Runs the m68k-linux-gnu cross-toolchain inside the `fast68k-cc` docker
image (the same one used by tools/cc68k.sh) to assemble + link
romwack/wack.s and emits a flat binary plus a manifest of relocations
compatible with romwack/relocate.py.

Run from the repo root:

    python3 romwack/build.py

Output (default):
    romwack/build/wack.bin    - flat 68k binary linked at LINK_BASE
    romwack/build/wack.json   - manifest (src_base, length, entries, relocs)

Then relocate to any target with:
    python3 romwack/relocate.py romwack/build/wack.bin \\
        --target 0xADDR --out romwack/build/wack_ADDR.bin
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import struct
import subprocess
import sys


# Defaults assume the script is invoked from the repo root.
LINK_BASE_DEFAULT = 0x00070000   # absolute link base; relocator can move it later
IMAGE_TAG = 'fast68k-cc:latest'
DOCKER_BUILD_CTX = 'tools/cc68k'        # shared with tools/cc68k.sh
DEFAULT_SRC = 'romwack/wack.s'
DEFAULT_OUT_PREFIX = 'romwack/build/wack'


def run_docker(workdir: str, cmd: str) -> None:
    full = ['docker', 'run', '--rm',
            '-v', f'{os.path.abspath(workdir)}:/work',
            '-w', '/work', IMAGE_TAG, 'sh', '-c', cmd]
    subprocess.run(full, check=True)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('--src', default=DEFAULT_SRC)
    ap.add_argument('--out-prefix', default=DEFAULT_OUT_PREFIX)
    ap.add_argument('--link-base', default=hex(LINK_BASE_DEFAULT))
    ap.add_argument('--no-build-image', action='store_true')
    args = ap.parse_args()

    link_base = int(args.link_base, 0)
    src = args.src
    if not os.path.isfile(src):
        raise SystemExit(f'source not found: {src}')

    out_dir = os.path.dirname(args.out_prefix) or '.'
    work = os.path.join(out_dir, 'work')
    os.makedirs(work, exist_ok=True)
    shutil.copy(src, os.path.join(work, 'wack.s'))

    if not args.no_build_image:
        subprocess.run(['docker', 'build', '-q', '-t', IMAGE_TAG, DOCKER_BUILD_CTX],
                       check=True, stdout=subprocess.DEVNULL)

    # Assemble, link at a known absolute base, dump flat binary, list relocations.
    cmd = f'''
        set -e
        m68k-linux-gnu-as -m68000 wack.s -o wack.o
        m68k-linux-gnu-ld -Ttext=0x{link_base:X} wack.o -o wack.elf
        m68k-linux-gnu-objcopy -O binary wack.elf wack.bin
        m68k-linux-gnu-objdump -r wack.o > wack.relocs.txt
        m68k-linux-gnu-nm wack.elf > wack.syms.txt
        m68k-linux-gnu-objdump -h wack.elf > wack.sections.txt
    '''
    run_docker(work, cmd)

    # Read built artefacts.
    bin_path = os.path.join(work, 'wack.bin')
    syms_path = os.path.join(work, 'wack.syms.txt')
    sections_path = os.path.join(work, 'wack.sections.txt')

    blob = open(bin_path, 'rb').read()
    blob_len = len(blob)

    # Parse .text VMA from objdump -h
    text_vma = link_base
    for line in open(sections_path):
        parts = line.split()
        if len(parts) >= 4 and parts[1] == '.text':
            text_vma = int(parts[3], 16)
            break

    # Parse symbol table for named entry points.
    KNOWN = {
        'wack_entry', 'user_cmd_head', 'cmd_regs', 'cmd_dump',
        'cmd_go', 'cmd_help', 'serial_init', 'getc_block', 'putc',
        'print_string', 'print_hex_long', 'print_hex_word', 'print_hex_byte',
        'read_line', 'skip_ws', 'parse_hex', 'str_eq', 'dispatch',
        'walk_list', 'builtin_head', 'banner', 'prompt', 'linebuf',
    }
    entries = {}
    for line in open(syms_path):
        parts = line.split()
        if len(parts) >= 3 and parts[2] in KNOWN:
            entries[parts[2]] = int(parts[0], 16)

    # Build relocation list from the .text section relocations in the
    # already-linked binary.  An easier route: rescan the flat blob for
    # any long-aligned 4-byte value that points into [link_base,
    # link_base+blob_len) -- because we've linked with absolute
    # addresses, every R_68K_32 reloc resolves to such a value.  This
    # catches exactly the 12 dispatch-list pointers and nothing else.
    # Read the ELF reloc table directly so we get the precise set without
    # heuristics.  We list R_68K_32 records inside .text; their offsets
    # within the .text section are the offsets within the flat binary.
    relocs_path = os.path.join(work, 'wack.relocs.txt')
    relocs = []
    in_text = False
    for line in open(relocs_path):
        if 'RELOCATION RECORDS FOR [.text]' in line:
            in_text = True
            continue
        if in_text and line.strip().startswith('RELOCATION'):
            in_text = False
        if not in_text:
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        if parts[1] != 'R_68K_32':
            continue
        try:
            off = int(parts[0], 16)
        except ValueError:
            continue
        if off >= blob_len:
            continue
        v = int.from_bytes(blob[off:off + 4], 'big')
        relocs.append({
            'offset_in_blob': off,
            'src_addr': text_vma + off,
            'target': v,
            'kind': 'DATA.L',
        })

    # Copy artefacts to final destinations.
    final_bin = args.out_prefix + '.bin'
    final_json = args.out_prefix + '.json'
    shutil.copy(bin_path, final_bin)

    meta = {
        'src': src,
        'src_base': text_vma,
        'blob_length': blob_len,
        'link_base': text_vma,
        'ranges': [{
            'lo': text_vma,
            'hi': text_vma + blob_len,
            'label': 'wack',
            'mode': 'mixed',
        }],
        'entries': entries,
        'relocs': relocs,
    }
    with open(final_json, 'w') as f:
        json.dump(meta, f, indent=2)

    print(f'built {final_bin}  ({blob_len} bytes)')
    print(f'manifest {final_json}  ({len(relocs)} relocs)')
    print('entries:')
    for k, v in sorted(entries.items(), key=lambda kv: kv[1]):
        print(f'  ${v:08X}  {k}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
