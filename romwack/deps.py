#!/usr/bin/env python3
"""Report ROMWack's external dependencies.

Disassemble the extracted ROMWack code ranges and list every BSR / JSR /
BRA / Bcc / JMP whose target falls OUTSIDE the extracted window.  These
are the helpers ROMWack borrows from the rest of Kickstart -- exec
private routines, the cold-boot entry, etc.

Static targets only: things like `JSR (A2)`, `JSR $FFE2(A6)` (exec LVO
call), `JMP (A0)` are dynamic and cannot be resolved from the
disassembly alone.  We report them under a separate heading so you
know they exist.

Usage:
    python3 tools/romwack_deps.py kickstart/kick_13.bin \\
        --manifest build/romwack.json \\
        > build/romwack_deps.txt
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

# Match disasm68k output lines like:
#   "  $FC2200:  6118                     BSR     $FC221A"
# and pull out (addr, mnemonic, target_hex).  The trailing `$` anchors
# the match so we don't accidentally pick up forms like
# `JMP $FFE2(A6)` -- those are register-indirect, not static jumps.
BRANCH_RE = re.compile(
    r'^\s*\$([0-9A-Fa-f]+):\s+[0-9A-Fa-f ]+?\s+'
    r'(BSR|BRA|JSR|JMP|BEQ|BNE|BLT|BLE|BGT|BGE|BHI|BLS|BCC|BCS|BVC|BVS|BPL|BMI|DBRA|DBEQ|DBNE|DBLT|DBLE|DBGT|DBGE|DBHI|DBLS|DBCC|DBCS|DBVC|DBVS|DBPL|DBMI|DBT|DBF)'
    r'\s+(?:[DA]\d,\s*)?\$([0-9A-Fa-f]+)\s*$'
)

# Dynamic JSR/JMP forms we should still surface so the reader knows they exist.
DYNAMIC_RE = re.compile(
    r'^\s*\$([0-9A-Fa-f]+):\s+[0-9A-Fa-f ]+?\s+'
    r'(JSR|JMP)\s+(\([AD]\d\)|\$[-0-9A-Fa-f]+\([AD]\d\)|[^$\s][^\n]*)$'
)


def _default_disasm() -> str:
    """Find tools/disasm68k.py relative to this script's parent directory."""
    here = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.dirname(here)
    return os.path.join(repo_root, 'tools', 'disasm68k.py')


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('rom')
    ap.add_argument('--manifest', required=True,
                    help='extract-tool manifest .json (gives ranges & rombase)')
    ap.add_argument('--rombase', default=None,
                    help='override rombase (else from manifest)')
    ap.add_argument('--disasm', default=None,
                    help='path to tools/disasm68k.py (default: alongside this script)')
    args = ap.parse_args()

    if args.disasm is None:
        args.disasm = _default_disasm()

    meta = json.load(open(args.manifest))
    rombase = int(args.rombase, 0) if args.rombase else meta['rombase']
    ranges = [(r['lo'], r['hi'], r['label'], r['mode']) for r in meta['ranges']]

    def in_window(addr: int) -> bool:
        return any(lo <= addr < hi for lo, hi, *_ in ranges)

    external_targets: dict[int, list[tuple[int, str]]] = defaultdict(list)
    dynamic_calls: list[tuple[int, str]] = []

    for lo, hi, _label, mode in ranges:
        if mode != 'code':
            continue
        nbytes = hi - lo
        cmd = ['python3', args.disasm, args.rom,
               f'0x{lo:08X}', '--bytes', str(nbytes),
               '--base', f'0x{rombase:X}']
        out = subprocess.run(cmd, capture_output=True, text=True, check=True).stdout
        for line in out.splitlines():
            m = BRANCH_RE.match(line)
            if m:
                src = int(m.group(1), 16)
                mnem = m.group(2)
                tgt = int(m.group(3), 16)
                if not in_window(tgt):
                    external_targets[tgt].append((src, mnem))
                continue
            m = DYNAMIC_RE.match(line)
            if m:
                # Only flag JSR/JMP through registers/indirect — these are
                # what we actually mean by "dynamic call".
                src = int(m.group(1), 16)
                rest = line.split(None, 2)[2].rstrip() if len(line.split(None, 2)) > 2 else line
                if '(A' in rest or '(D' in rest:
                    dynamic_calls.append((src, rest))

    # Report
    print(f'ROMWack external dependencies (manifest: {args.manifest})')
    print(f'Window:')
    for lo, hi, lbl, mode in ranges:
        print(f'  ${lo:08X}-${hi:08X}  {lbl:<14} [{mode}]')
    print()

    print(f'External static call/branch targets ({len(external_targets)}):')
    for tgt in sorted(external_targets):
        callers = external_targets[tgt]
        print(f'  ${tgt:08X}   ({len(callers)} caller{"s" if len(callers)!=1 else ""})')
        for src, mnem in callers:
            print(f'      from ${src:08X}  {mnem}')
    print()

    print(f'Dynamic / indirect calls ({len(dynamic_calls)}):')
    for src, text in dynamic_calls:
        print(f'  ${src:08X}  {text}')

    return 0


if __name__ == '__main__':
    sys.exit(main())
