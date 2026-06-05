#!/usr/bin/env python3
"""Blit-trace analyzer for the 68k Verilator sim.

Parses `[BLT_BPL1]`, `[BLT_START]`, `[BLT_TRIGGER]`, and
`[BLT_WR_*]` lines from a boot-trace log and provides:
  parse     — extract structured records from a log
  filter    — show blits matching predicates
  touched   — find blits whose D-write spans a chip address
  simulate  — compute predicted D output for a blit (LF + masks)
  histo     — destination-range histogram
  uniq      — count unique register-tuples

Used by WB1.3 visual-debug investigations (see
docs/WB13_DEBUG_JOURNAL §54-§57).  Generate trace with:

    rm -rf build_kick_boot
    BOOT_TRACE=0 EXTRA_VERI_DEFS=+define+BLT_BORDER_TRACE \\
        CHIPRAM_DUMP=/tmp/wb_chipram.bin \\
        make test-kickstart-boot ADFFILE=kickstart/wb13.adf

Then:
    tools/blt_trace.py filter build_kick_boot/run.log --lf=ea --use-a=0
    tools/blt_trace.py touched build_kick_boot/run.log --addr=0x9ef8
    tools/blt_trace.py simulate --bltcon=0xea000103 \\
        --bltsize=0x0068 --c-data=0x0000 --afwm=0x3fff --alwm=0xfffc
"""

import argparse
import json
import re
import sys
from dataclasses import dataclass, asdict, field
from typing import Optional


# ────────────────────────────────────────────────────────────────────
# Trace record types
# ────────────────────────────────────────────────────────────────────

@dataclass
class BlitRecord:
    """One BLT_BPL1 / BLT_START trace line."""
    log_line: int
    bltcon: int
    bltapt: int = 0
    bltbpt: int = 0
    bltcpt: int = 0
    bltdpt: int = 0
    bltsize: int = 0       # raw 16-bit BLTSIZE value
    bltamod: int = 0
    bltbmod: int = 0
    bltcmod: int = 0
    bltdmod: int = 0
    adat: int = 0          # BLTADAT_pre
    bdat: int = 0          # BLTBDAT_pre
    cdat: int = 0          # BLTCDAT_pre
    afwm: int = 0xFFFF
    alwm: int = 0xFFFF
    extras: dict = field(default_factory=dict)

    # --- Decoded BLTCON fields (Verilog internal layout)
    @property
    def lf(self):       return (self.bltcon >> 24) & 0xFF
    @property
    def ash(self):      return (self.bltcon >> 20) & 0xF
    @property
    def bsh(self):      return (self.bltcon >> 16) & 0xF
    @property
    def desc(self):     return (self.bltcon >> 15) & 1
    @property
    def ife(self):      return (self.bltcon >> 14) & 1
    @property
    def efe(self):      return (self.bltcon >> 13) & 1
    @property
    def fci(self):      return (self.bltcon >> 12) & 1
    @property
    def line(self):     return (self.bltcon >> 11) & 1
    @property
    def use_a(self):    return (self.bltcon >> 3) & 1
    @property
    def use_b(self):    return (self.bltcon >> 2) & 1
    @property
    def use_c(self):    return (self.bltcon >> 1) & 1
    @property
    def use_d(self):    return (self.bltcon >> 0) & 1

    @property
    def height(self):
        """BLTSIZE high field: rows in copy mode, pixels in line mode."""
        return (self.bltsize >> 6) & 0x3FF or 1024

    @property
    def width(self):
        """BLTSIZE low field: words per row.  0 = 64 (Amiga quirk)."""
        w = self.bltsize & 0x3F
        return w if w else 64

    @property
    def d_byte_span(self):
        """For copy mode: byte range the D-channel writes over
        (low_addr, high_addr_exclusive).  Heuristic — does not
        account for descending mode or bltdmod-driven skips."""
        if self.line:
            # Line mode: D walks per-pixel, hard to characterise
            # as a single span.  Return [bltdpt, bltdpt+1] as a stub.
            return (self.bltdpt, self.bltdpt + 2)
        rows = self.height
        per_row = self.width * 2  # word stride bytes
        if self.bltdmod:
            # Pointer advances width*2 + bltdmod per row
            total_bytes_per_row = per_row + (self.bltdmod & 0xFFFFFFFF)
            if total_bytes_per_row > 0x80000000:  # signed negative
                total_bytes_per_row -= 0x100000000
            span_end = self.bltdpt + rows * total_bytes_per_row
        else:
            span_end = self.bltdpt + rows * per_row
        return (min(self.bltdpt, span_end), max(self.bltdpt, span_end))


# ────────────────────────────────────────────────────────────────────
# Trace log parser
# ────────────────────────────────────────────────────────────────────

# Regex for [BLT_BPL1] / [BLT_START] lines — captures all "key=value"
# pairs.  We accept both formats by being flexible about which keys
# appear.
_FIELD_RE = re.compile(r'(\w+)=([0-9a-fA-F]+)')


def parse_trace(log_path: str, pattern: str = 'BLT_BPL1') -> list:
    """Parse all matching trace lines.  Returns list of BlitRecord."""
    records = []
    with open(log_path, 'r', encoding='utf-8', errors='replace') as f:
        for lineno, line in enumerate(f, 1):
            if pattern not in line:
                continue
            fields = dict(_FIELD_RE.findall(line))
            if 'bltcon' not in fields:
                continue
            kwargs = {'log_line': lineno}
            # Standard fields
            for k in ('bltcon', 'bltapt', 'bltbpt', 'bltcpt', 'bltdpt',
                      'bltamod', 'bltbmod', 'bltcmod', 'bltdmod',
                      'adat', 'bdat', 'cdat', 'afwm', 'alwm'):
                if k in fields:
                    kwargs[k] = int(fields[k], 16)
            if 'bltsize' in fields:
                kwargs['bltsize'] = int(fields['bltsize'], 16)
            # Stash any extras (e.g. cur_word, cur_row from BLT_WR_*)
            extras = {k: v for k, v in fields.items()
                      if k not in kwargs and k != 'addr'}
            kwargs['extras'] = extras
            records.append(BlitRecord(**kwargs))
    return records


# ────────────────────────────────────────────────────────────────────
# Filtering predicates
# ────────────────────────────────────────────────────────────────────

def parse_int(s: str) -> int:
    """Accept 0xff / $ff / ff (assumed hex if not 0x-prefixed)."""
    s = s.lstrip('$')
    if s.startswith('0x') or s.startswith('0X'):
        return int(s, 16)
    try:
        return int(s, 16)
    except ValueError:
        return int(s)


def parse_range(s: str) -> tuple:
    """'lo..hi' or 'lo:hi' → (lo, hi)."""
    sep = '..' if '..' in s else ':'
    lo, hi = s.split(sep, 1)
    return (parse_int(lo), parse_int(hi))


def matches(rec: BlitRecord, **preds) -> bool:
    """Generic predicate matcher.  Each kwarg is an attribute or
    a decoded property of BlitRecord."""
    for key, val in preds.items():
        if val is None:
            continue
        if hasattr(rec, key):
            actual = getattr(rec, key)
            if isinstance(val, tuple) and len(val) == 2:
                lo, hi = val
                if not (lo <= actual <= hi):
                    return False
            else:
                if actual != val:
                    return False
        else:
            return False
    return True


# ────────────────────────────────────────────────────────────────────
# Blitter reference: predict D output for a single copy-mode blit
# ────────────────────────────────────────────────────────────────────

def lf_combine(lf: int, a: int, b: int, c: int) -> int:
    """Apply 8-bit LF minterm bit-wise to 16-bit channels."""
    out = 0
    for bit in range(16):
        ab = (a >> bit) & 1
        bb = (b >> bit) & 1
        cb = (c >> bit) & 1
        idx = (ab << 2) | (bb << 1) | cb
        if (lf >> idx) & 1:
            out |= (1 << bit)
    return out


def predict_word(rec: BlitRecord, cur_word: int, c_word: int = 0,
                 a_word: Optional[int] = None,
                 b_word: Optional[int] = None) -> int:
    """Predict the D output for ONE word of a copy-mode blit.

    Args:
        cur_word: 0 .. width-1 (for first/last mask application)
        c_word: actual C-channel value read from memory (or
                BLTCDAT_pre if USE_C=0)
        a_word: actual A-channel value (defaults to BLTADAT_pre)
        b_word: actual B-channel value (defaults to BLTBDAT_pre)

    Returns the predicted 16-bit D output.  Does NOT model:
        - barrel shifts (ASH/BSH != 0)
        - descending mode
        - fill modes
        - the D-write pipeline delay
    """
    a = a_word if a_word is not None else rec.adat
    b = b_word if b_word is not None else rec.bdat

    # Apply BLTAFWM/BLTALWM to A channel (real-Amiga: ALWAYS,
    # regardless of USE_A — see WB13_DEBUG_JOURNAL §55c).
    is_first = (cur_word == 0)
    is_last = (cur_word == rec.width - 1)
    if is_first:
        a &= rec.afwm
    if is_last:
        a &= rec.alwm

    if not rec.use_c:
        c_word = rec.cdat

    return lf_combine(rec.lf, a & 0xFFFF, b & 0xFFFF, c_word & 0xFFFF)


# ────────────────────────────────────────────────────────────────────
# CLI subcommands
# ────────────────────────────────────────────────────────────────────

def cmd_parse(args):
    """Dump the parsed trace as JSON or CSV."""
    records = parse_trace(args.log)
    if args.format == 'json':
        out = [dict(asdict(r), lf=r.lf, height=r.height, width=r.width)
               for r in records]
        json.dump(out, sys.stdout, indent=2, default=lambda o: f'${o:08X}'
                  if isinstance(o, int) and o > 9 else o)
        print()
    else:
        print('line,bltcon,bltdpt,bltsize,lf,height,width,'
              'use_a,use_b,use_c,use_d,line_mode,afwm,alwm')
        for r in records:
            print(f'{r.log_line},{r.bltcon:08x},{r.bltdpt:08x},'
                  f'{r.bltsize:04x},{r.lf:02x},{r.height},{r.width},'
                  f'{r.use_a},{r.use_b},{r.use_c},{r.use_d},{r.line},'
                  f'{r.afwm:04x},{r.alwm:04x}')


def cmd_filter(args):
    """Show blits matching predicates."""
    records = parse_trace(args.log)
    preds = {}
    if args.bltcon is not None:
        preds['bltcon'] = parse_int(args.bltcon)
    if args.lf is not None:
        preds['lf'] = parse_int(args.lf)
    if args.dest is not None:
        preds['bltdpt'] = parse_range(args.dest)
    if args.use_a is not None:
        preds['use_a'] = args.use_a
    if args.use_b is not None:
        preds['use_b'] = args.use_b
    if args.use_c is not None:
        preds['use_c'] = args.use_c
    if args.use_d is not None:
        preds['use_d'] = args.use_d
    if args.line is not None:
        preds['line'] = args.line
    if args.afwm is not None:
        preds['afwm'] = parse_int(args.afwm)
    if args.alwm is not None:
        preds['alwm'] = parse_int(args.alwm)

    matches_list = [r for r in records if matches(r, **preds)]

    if not matches_list:
        print(f'No blits match filter (of {len(records)} total)')
        return

    print(f'{len(matches_list)} of {len(records)} blits match:')
    print(f'{"line":>7}  {"bltcon":>8} {"bltdpt":>8} {"size":>4}  '
          f'lf  use h×w   {"afwm":>4} {"alwm":>4}')
    for r in matches_list[:args.limit]:
        usage = (f'{"A" if r.use_a else "-"}{"B" if r.use_b else "-"}'
                 f'{"C" if r.use_c else "-"}{"D" if r.use_d else "-"}')
        mode = 'LIN' if r.line else 'CPY'
        print(f'{r.log_line:>7}  {r.bltcon:08x} {r.bltdpt:08x} {r.bltsize:04x}  '
              f'{r.lf:02x}  {usage} {r.height:3}×{r.width:<2} {mode}  '
              f'{r.afwm:04x} {r.alwm:04x}')
    if len(matches_list) > args.limit:
        print(f'... +{len(matches_list) - args.limit} more')


def cmd_touched(args):
    """Find blits whose D-channel writes might touch a chip address."""
    records = parse_trace(args.log)
    addr = parse_int(args.addr)
    touching = []
    for r in records:
        lo, hi = r.d_byte_span
        if lo <= addr < hi:
            touching.append(r)

    print(f'{len(touching)} blits may touch ${addr:08X} '
          f'(of {len(records)} total):')
    print(f'{"line":>7}  {"bltcon":>8} {"dpt":>8} {"size":>4} {"lf":>2} '
          f'h×w   span')
    for r in touching[:args.limit]:
        lo, hi = r.d_byte_span
        print(f'{r.log_line:>7}  {r.bltcon:08x} {r.bltdpt:08x} '
              f'{r.bltsize:04x} {r.lf:02x} '
              f'{r.height:3}×{r.width:<2}  ${lo:06x}-${hi:06x}')
    if len(touching) > args.limit:
        print(f'... +{len(touching) - args.limit} more')


def cmd_simulate(args):
    """Compute predicted D for a hand-constructed blit setup.
    Useful for replaying a single blit in isolation without rebuilding."""
    rec = BlitRecord(
        log_line=0,
        bltcon=parse_int(args.bltcon),
        bltsize=parse_int(args.bltsize),
        adat=parse_int(args.adat) if args.adat else 0xFFFF,
        bdat=parse_int(args.bdat) if args.bdat else 0xFFFF,
        cdat=parse_int(args.cdat) if args.cdat else 0,
        afwm=parse_int(args.afwm) if args.afwm else 0xFFFF,
        alwm=parse_int(args.alwm) if args.alwm else 0xFFFF,
    )
    c_words = []
    if args.c_data:
        c_words = [parse_int(s) for s in args.c_data.split(',')]
    print(f'Blit:  bltcon=${rec.bltcon:08X} (LF=${rec.lf:02X} '
          f'USE_A={rec.use_a} USE_B={rec.use_b} USE_C={rec.use_c} '
          f'USE_D={rec.use_d})')
    print(f'       size={rec.height}×{rec.width}  afwm=${rec.afwm:04X}  '
          f'alwm=${rec.alwm:04X}')
    print(f'       adat=${rec.adat:04X}  bdat=${rec.bdat:04X}  '
          f'cdat=${rec.cdat:04X}')
    print()
    print(f'Predicted D output per word (1 row):')
    for w in range(rec.width):
        c = c_words[w] if w < len(c_words) else 0
        d = predict_word(rec, w, c)
        print(f'  word {w:2}: C=${c:04X}  →  D=${d:04X}')


def cmd_histo(args):
    """Histogram destination ranges."""
    records = parse_trace(args.log)
    buckets = {}
    bucket_size = parse_int(args.bucket) if args.bucket else 0x100
    for r in records:
        b = (r.bltdpt // bucket_size) * bucket_size
        buckets[b] = buckets.get(b, 0) + 1
    for b in sorted(buckets):
        bar = '#' * min(buckets[b], 60)
        print(f'  ${b:08X}: {buckets[b]:5}  {bar}')


def cmd_uniq(args):
    """Count unique (bltcon, afwm, alwm) tuples."""
    records = parse_trace(args.log)
    tuples = {}
    for r in records:
        key = (r.bltcon, r.afwm, r.alwm)
        tuples[key] = tuples.get(key, 0) + 1
    print(f'{len(tuples)} unique (bltcon, afwm, alwm) tuples '
          f'(of {len(records)} blits):')
    for (con, afwm, alwm), n in sorted(tuples.items(), key=lambda x: -x[1]):
        lf = (con >> 24) & 0xFF
        usage = ''
        for bit, name in [(3, 'A'), (2, 'B'), (1, 'C'), (0, 'D')]:
            usage += name if (con >> bit) & 1 else '-'
        print(f'  ×{n:4}  bltcon=${con:08X} lf=${lf:02X} use={usage}  '
              f'afwm=${afwm:04X} alwm=${alwm:04X}')


# ────────────────────────────────────────────────────────────────────
# Main
# ────────────────────────────────────────────────────────────────────

def main():
    p = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest='cmd', required=True)

    pp = sub.add_parser('parse', help='dump trace as CSV/JSON')
    pp.add_argument('log')
    pp.add_argument('--format', choices=['csv', 'json'], default='csv')
    pp.set_defaults(func=cmd_parse)

    pf = sub.add_parser('filter', help='show blits matching predicates')
    pf.add_argument('log')
    pf.add_argument('--bltcon', help='match bltcon value (hex)')
    pf.add_argument('--lf', help='match LF byte (hex)')
    pf.add_argument('--dest', help='match dest range "lo..hi"')
    pf.add_argument('--afwm', help='match AFWM value (hex)')
    pf.add_argument('--alwm', help='match ALWM value (hex)')
    pf.add_argument('--use-a', type=int, choices=[0, 1])
    pf.add_argument('--use-b', type=int, choices=[0, 1])
    pf.add_argument('--use-c', type=int, choices=[0, 1])
    pf.add_argument('--use-d', type=int, choices=[0, 1])
    pf.add_argument('--line', type=int, choices=[0, 1],
                    help='1 = line-mode only, 0 = copy-mode only')
    pf.add_argument('--limit', type=int, default=20)
    pf.set_defaults(func=cmd_filter)

    pt = sub.add_parser('touched',
        help='find blits whose D-write may span an address')
    pt.add_argument('log')
    pt.add_argument('--addr', required=True, help='chip address')
    pt.add_argument('--limit', type=int, default=20)
    pt.set_defaults(func=cmd_touched)

    ps = sub.add_parser('simulate',
        help='predict D output for a single blit setup')
    ps.add_argument('--bltcon', required=True)
    ps.add_argument('--bltsize', required=True)
    ps.add_argument('--adat')
    ps.add_argument('--bdat')
    ps.add_argument('--cdat')
    ps.add_argument('--afwm')
    ps.add_argument('--alwm')
    ps.add_argument('--c-data',
        help='comma-separated C-channel word values (hex)')
    ps.set_defaults(func=cmd_simulate)

    ph = sub.add_parser('histo', help='destination histogram')
    ph.add_argument('log')
    ph.add_argument('--bucket', default='0x100',
                    help='bucket size (default 0x100)')
    ph.set_defaults(func=cmd_histo)

    pu = sub.add_parser('uniq',
        help='unique (bltcon, afwm, alwm) tuples')
    pu.add_argument('log')
    pu.set_defaults(func=cmd_uniq)

    args = p.parse_args()
    args.func(args)


if __name__ == '__main__':
    main()
