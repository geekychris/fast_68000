#!/usr/bin/env python3
"""blit_classify.py — analyze blitter use across a chipset trace.

Reads a `[CHIPSET-WR]` log (from +define+CHIPSET_TRACE_ALL), walks every
write, maintains a running "shadow" blitter register file, and emits a
per-BLTSIZE-write record of the blit that just got fired.  Then prints
several aggregates so you can spot patterns boing exercises that hello
doesn't, classes of blits where our blitter has historical bugs, etc.

USAGE
-----
    make boing-chipset-trace          # captures /tmp/boing_chipset.ours.log
    python3 tools/intuition_diff/blit_classify.py /tmp/boing_chipset.ours.log

OUTPUT
------
- count of blits per BLTCON0 (op) value
- count by USE flags (A/B/C/D combinations)
- bltsize histogram (small <16 words, medium, large)
- pointer-source histogram (chip / slow / unmapped)
- top-20 hottest (BLTCON0, BLTSIZE) pairs

Run with `--csv FILE` to emit one CSV row per blit for finer analysis.
"""
from __future__ import annotations

import argparse
import csv
import re
import sys
from collections import Counter
from pathlib import Path

LINE_RE = re.compile(
    r"\[CHIPSET-WR\]\s+r=(?P<retired>\d+)\s+src=(?P<src>\d+)\s+pc=(?P<pc>[0-9a-fA-F]+)"
    r"\s+we=1\s+addr=(?P<addr>[0-9a-fA-F]+)\s+wdata=(?P<wdata>[0-9a-fA-F]+)"
    r"\s+be=(?P<be>[01]{4})"
)

# Blitter register addresses we shadow.  Each register lives at $DFFxxx;
# the BE pattern in our trace shows which 16-bit half got written.
BLT_REGS = {
    0x040: "BLTCON0",  0x042: "BLTCON1",
    0x044: "BLTAFWM",  0x046: "BLTALWM",
    0x048: "BLTCPTH",  0x04A: "BLTCPTL",
    0x04C: "BLTBPTH",  0x04E: "BLTBPTL",
    0x050: "BLTAPTH",  0x052: "BLTAPTL",
    0x054: "BLTDPTH",  0x056: "BLTDPTL",
    0x058: "BLTSIZE",
    0x060: "BLTCMOD",  0x062: "BLTBMOD",
    0x064: "BLTAMOD",  0x066: "BLTDMOD",
    0x070: "BLTCDAT",  0x072: "BLTBDAT", 0x074: "BLTADAT",
}


def parse_lines(path: Path):
    with path.open() as f:
        for line in f:
            m = LINE_RE.search(line)
            if not m:
                continue
            yield (int(m["retired"]),
                   int(m["src"]),
                   int(m["pc"], 16),
                   int(m["addr"], 16),
                   int(m["wdata"], 16),
                   m["be"])


def half_value(wdata: int, be: str, addr_low: int) -> int:
    """Extract the 16-bit value from the 32-bit wdata using be + addr alignment.

    Trace BE format: be[3:0] = bits 31:24, 23:16, 15:8, 7:0 byte enables.
    We see two cases:
      be=1100  -> wrote bytes 31:24+23:16 = high 16 bits.  Address is
                  4-byte aligned.  Value lives in wdata[31:16].
      be=0011  -> wrote bytes 15:8+7:0 = low 16 bits.  Address is
                  4-byte+2.  Value lives in wdata[15:0].
    Anything else (single-byte writes etc) is unusual for chipset.
    """
    if be == "1100":
        return (wdata >> 16) & 0xFFFF
    if be == "0011":
        return wdata & 0xFFFF
    # Fall back: try to pick whichever half is non-zero
    hi = (wdata >> 16) & 0xFFFF
    lo = wdata & 0xFFFF
    return hi if hi != 0 else lo


def ptr_region(value32: int) -> str:
    """Classify a 24-bit pointer by region."""
    addr = value32 & 0x00FFFFFF
    if addr < 0x080000:
        return "CHIP"
    if 0xC00000 <= addr < 0xC80000:
        return "SLOW"
    if 0xDFF000 <= addr < 0xDFF200:
        return "CHIPREG"
    return "OTHER"


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("trace", type=Path)
    ap.add_argument("--csv", type=Path, help="per-blit CSV output")
    args = ap.parse_args(argv)

    shadow = {name: 0 for name in BLT_REGS.values()}
    # Pointers are 32-bit composed of H+L halves, so we track them separately.
    ptrs = {"BLTAPT": 0, "BLTBPT": 0, "BLTCPT": 0, "BLTDPT": 0}

    blits = []
    csv_rows = []

    for retired, src, pc, addr, wdata, be in parse_lines(args.trace):
        local = addr - 0x00DF_F000
        if local not in BLT_REGS:
            continue
        reg_name = BLT_REGS[local]
        value = half_value(wdata, be, local & 3)
        shadow[reg_name] = value

        # Track pointer-pair updates.
        if reg_name.endswith("PTH"):
            base = reg_name[:-3]   # "BLTA"
            ptrs[base + "PT"] = (ptrs[base + "PT"] & 0x0000FFFF) | (value << 16)
        elif reg_name.endswith("PTL"):
            base = reg_name[:-3]
            ptrs[base + "PT"] = (ptrs[base + "PT"] & 0xFFFF0000) | value

        if reg_name == "BLTSIZE":
            # Blit fires.  Snapshot.
            bltsize = value
            words_per_row = bltsize & 0x3F or 64       # 0 means 64
            rows = (bltsize >> 6) & 0x3FF or 1024
            con0 = shadow["BLTCON0"]
            use_a = (con0 >> 11) & 1
            use_b = (con0 >> 10) & 1
            use_c = (con0 >> 9) & 1
            use_d = (con0 >> 8) & 1
            minterm = con0 & 0xFF
            ash = (con0 >> 12) & 0xF
            con1 = shadow["BLTCON1"]
            line = (con1 >> 0) & 1
            fill_xor = (con1 >> 4) & 1
            fill_carry = (con1 >> 3) & 1
            desc = (con1 >> 1) & 1

            blit = {
                "retired": retired,
                "pc": pc,
                "src": src,
                "con0": con0,
                "con1": con1,
                "use": (use_a << 3) | (use_b << 2) | (use_c << 1) | use_d,
                "use_str": "".join(c for c, b in
                                   zip("ABCD",
                                       (use_a, use_b, use_c, use_d)) if b),
                "minterm": minterm,
                "ash": ash,
                "line": line,
                "desc": desc,
                "fill": (fill_xor << 1) | fill_carry,
                "afwm": shadow["BLTAFWM"],
                "alwm": shadow["BLTALWM"],
                "amod": shadow["BLTAMOD"],
                "bmod": shadow["BLTBMOD"],
                "cmod": shadow["BLTCMOD"],
                "dmod": shadow["BLTDMOD"],
                "apt": ptrs["BLTAPT"] & 0x00FFFFFF,
                "bpt": ptrs["BLTBPT"] & 0x00FFFFFF,
                "cpt": ptrs["BLTCPT"] & 0x00FFFFFF,
                "dpt": ptrs["BLTDPT"] & 0x00FFFFFF,
                "words": words_per_row,
                "rows": rows,
            }
            blits.append(blit)

            if args.csv:
                csv_rows.append({k: f"${v:X}" if isinstance(v, int) and k not in
                                 ("retired", "src", "use", "minterm", "ash",
                                  "line", "desc", "fill", "words", "rows")
                                 else v for k, v in blit.items()})

    print(f"Parsed {sum(1 for _ in parse_lines(args.trace)):,} CHIPSET-WR lines")
    print(f"Captured {len(blits):,} BLTSIZE-fired blits\n")

    if not blits:
        return 0

    # ---- USE flag distribution ----
    use_counts = Counter(b["use_str"] or "(none)" for b in blits)
    print("== USE-flag combinations ==")
    print(f"{'flags':<10}  {'count':>8}  {'pct':>6}")
    total = len(blits)
    for use, n in use_counts.most_common():
        print(f"{use:<10}  {n:>8}  {n/total*100:>5.1f}%")
    print()

    # ---- Line vs area blits ----
    n_line = sum(b["line"] for b in blits)
    n_area = total - n_line
    print(f"== mode ==")
    print(f"area mode blits: {n_area:>8}  ({n_area/total*100:.1f}%)")
    print(f"line mode blits: {n_line:>8}  ({n_line/total*100:.1f}%)")
    print()

    # ---- minterm distribution (top 20) ----
    print("== minterm (BLTCON0 low byte) — top 20 ==")
    mt_counts = Counter(f"${b['minterm']:02X}" for b in blits)
    for mt, n in mt_counts.most_common(20):
        print(f"  {mt}  {n:>8}")
    print()

    # ---- size histogram ----
    print("== size histogram (words_per_row × rows) ==")
    size_buckets = Counter()
    for b in blits:
        total_words = b["words"] * b["rows"]
        if total_words <= 16:           bucket = "tiny  (<=16)"
        elif total_words <= 256:        bucket = "small (<=256)"
        elif total_words <= 4096:       bucket = "med   (<=4096)"
        else:                            bucket = "large (>4096)"
        size_buckets[bucket] += 1
    for bucket in ("tiny  (<=16)", "small (<=256)", "med   (<=4096)", "large (>4096)"):
        n = size_buckets[bucket]
        print(f"  {bucket}  {n:>8}  ({n/total*100:.1f}%)")
    print()

    # ---- pointer regions per master role ----
    print("== pointer regions ==")
    for role in ("apt", "bpt", "cpt", "dpt"):
        regs = Counter(ptr_region(b[role]) for b in blits)
        print(f"  BLT{role.upper()}: " +
              " ".join(f"{r}={n}" for r, n in regs.most_common()))
    print()

    # ---- hottest (con0, size) pairs ----
    print("== hottest (BLTCON0, words×rows) pairs — top 20 ==")
    pair_counts = Counter((b["con0"], b["words"] * b["rows"]) for b in blits)
    for (c0, sz), n in pair_counts.most_common(20):
        use_str = "".join(c for i, c in enumerate("ABCD") if c0 & (0x800 >> i))
        print(f"  BLTCON0=${c0:04X} (USE={use_str:<4}, minterm=${c0 & 0xFF:02X})"
              f"  size={sz:<8}  count={n}")
    print()

    if args.csv and csv_rows:
        fieldnames = list(csv_rows[0].keys())
        with args.csv.open("w") as f:
            w = csv.DictWriter(f, fieldnames=fieldnames)
            w.writeheader()
            w.writerows(csv_rows)
        print(f"Wrote per-blit CSV to {args.csv}")

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
