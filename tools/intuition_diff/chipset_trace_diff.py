#!/usr/bin/env python3
"""chipset_trace_diff.py — positional diff of two chipset-write streams.

The companion to `+define+CHIPSET_TRACE_ALL` in rtl/m68k_bus.v: when the
trace probe fires, our sim logs

    [CHIPSET-WR] r=N src=S pc=P we=1 addr=00dffXXX wdata=YYYYYYYY be=ZZZZ

once per chipset write.  This tool reads two such streams (our run.log,
and an equivalent FS-UAE export — see below) and compares them
position-by-position.  The first divergence is the smoking-gun moment
where our sim writes a different register / value than FS-UAE does for
the same boot.

USAGE
-----

    # Generate ours
    rm -rf build_kick_boot
    EXTRA_VERI_DEFS="+define+CHIPSET_TRACE_ALL" BOOT_TRACE=0 \\
        ROMCYCLES=400000000 \\
        make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
    grep -E '^\\[CHIPSET-WR\\]' build_kick_boot/run.log \\
        > /tmp/boing_chipset.ours.log

    # Generate FS-UAE-side (see FS-UAE patch notes below)
    ...  > /tmp/boing_chipset.fsuae.log

    # Compare
    python3 tools/intuition_diff/chipset_trace_diff.py \\
        /tmp/boing_chipset.ours.log /tmp/boing_chipset.fsuae.log

WHAT GETS COMPARED
------------------

Each line is parsed into (addr, wdata, be, src).  retired/pc are
ignored for the diff (they will inevitably differ slightly between
sims) but printed in the context window around a divergence.

The comparison is *positional*: line N of stream A is compared to
line N of stream B.  That works because both sims, given the same
ADF + ROM, should issue chipset writes in the same order.  When the
order diverges (say, our blitter triggers one extra write because
of a state divergence), the diff catches it on the very next entry.

The tool also supports a *coarser* mode (`--by-addr`) where it
groups writes by address and counts them; useful for spotting "we
write BPLCON0 1000 times and FS-UAE writes it 1004 times" kinds of
divergence even when ordering isn't stable.

FS-UAE PATCH NOTES
------------------

FS-UAE already has the claude_rpc patch (commit history references
project_fsuae_rpc.md).  To add chipset-trace support:

  1. Extend the patch so the `/v1/chipset_trace` endpoint enables a
     hook in `custom_cycle()` / `chipmem_wput()` etc., capturing
     (addr, value, be) for every $DFF000-$DFFFFF write.
  2. Export as the same `[CHIPSET-WR] r=N src=S pc=P we=1 addr=H
     wdata=H be=B` line format this tool parses (or a JSON stream
     that this tool can be extended to ingest — see
     parse_line()).
  3. Boot boing-disk in FS-UAE-no-UAEFS with the hook armed.

Until that's done, the tool can be used to compare two of our own
runs (e.g., to verify a fix didn't regress the chipset state).
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, namedtuple
from pathlib import Path
from typing import Iterator

LINE_RE = re.compile(
    r"\[CHIPSET-WR\]\s+"
    r"r=(?P<retired>\d+)\s+"
    r"src=(?P<src>\d+)\s+"
    r"pc=(?P<pc>[0-9a-fA-F]+)\s+"
    r"we=1\s+"
    r"addr=(?P<addr>[0-9a-fA-F]+)\s+"
    r"wdata=(?P<wdata>[0-9a-fA-F]+)\s+"
    r"be=(?P<be>[01]{4})"
)

Entry = namedtuple("Entry", "retired src pc addr wdata be raw")

# Map address -> nominal Amiga chipset register name.  This makes the
# diff output much more readable.  Only common ones; if the diff hits a
# register not in this table, the raw $DFFxxx address is shown.
CHIPSET_REG_NAMES = {
    0x002: "DMACONR",  0x004: "VPOSR",   0x006: "VHPOSR",  0x00A: "JOY0DAT",
    0x00C: "JOY1DAT",  0x00E: "CLXDAT",  0x010: "ADKCONR", 0x012: "POT0DAT",
    0x014: "POT1DAT",  0x016: "POTGOR",  0x018: "SERDATR", 0x01A: "DSKBYTR",
    0x01C: "INTENAR", 0x01E: "INTREQR",
    0x020: "DSKPTH",   0x022: "DSKPTL",  0x024: "DSKLEN",  0x026: "DSKDAT",
    0x028: "REFPTR",   0x02A: "VPOSW",   0x02C: "VHPOSW",  0x02E: "COPCON",
    0x030: "SERDAT",   0x032: "SERPER",  0x034: "POTGO",   0x036: "JOYTEST",
    0x038: "STREQU",   0x03A: "STRVBL",  0x03C: "STRHOR",  0x03E: "STRLONG",
    0x040: "BLTCON0",  0x042: "BLTCON1", 0x044: "BLTAFWM", 0x046: "BLTALWM",
    0x048: "BLTCPTH",  0x04A: "BLTCPTL", 0x04C: "BLTBPTH", 0x04E: "BLTBPTL",
    0x050: "BLTAPTH",  0x052: "BLTAPTL", 0x054: "BLTDPTH", 0x056: "BLTDPTL",
    0x058: "BLTSIZE",  0x060: "BLTCMOD", 0x062: "BLTBMOD", 0x064: "BLTAMOD",
    0x066: "BLTDMOD",  0x070: "BLTCDAT", 0x072: "BLTBDAT", 0x074: "BLTADAT",
    0x07E: "DSKSYNC",
    0x080: "COP1LCH",  0x082: "COP1LCL", 0x084: "COP2LCH", 0x086: "COP2LCL",
    0x088: "COPJMP1",  0x08A: "COPJMP2", 0x08E: "DIWSTRT", 0x090: "DIWSTOP",
    0x092: "DDFSTRT",  0x094: "DDFSTOP",
    0x096: "DMACON",   0x098: "CLXCON",  0x09A: "INTENA",  0x09C: "INTREQ",
    0x09E: "ADKCON",
    0x0A0: "AUD0LCH",  0x0A2: "AUD0LCL", 0x0A4: "AUD0LEN", 0x0A6: "AUD0PER",
    0x0A8: "AUD0VOL",  0x0AA: "AUD0DAT",
    0x0B0: "AUD1LCH",  0x0B2: "AUD1LCL", 0x0B4: "AUD1LEN", 0x0B6: "AUD1PER",
    0x0B8: "AUD1VOL",  0x0BA: "AUD1DAT",
    0x0C0: "AUD2LCH",  0x0C2: "AUD2LCL", 0x0C4: "AUD2LEN", 0x0C6: "AUD2PER",
    0x0C8: "AUD2VOL",  0x0CA: "AUD2DAT",
    0x0D0: "AUD3LCH",  0x0D2: "AUD3LCL", 0x0D4: "AUD3LEN", 0x0D6: "AUD3PER",
    0x0D8: "AUD3VOL",  0x0DA: "AUD3DAT",
    0x0E0: "BPL1PTH",  0x0E2: "BPL1PTL", 0x0E4: "BPL2PTH", 0x0E6: "BPL2PTL",
    0x0E8: "BPL3PTH",  0x0EA: "BPL3PTL", 0x0EC: "BPL4PTH", 0x0EE: "BPL4PTL",
    0x0F0: "BPL5PTH",  0x0F2: "BPL5PTL",
    0x100: "BPLCON0",  0x102: "BPLCON1", 0x104: "BPLCON2", 0x108: "BPL1MOD",
    0x10A: "BPL2MOD",
    0x110: "BPL1DAT",  0x112: "BPL2DAT", 0x114: "BPL3DAT", 0x116: "BPL4DAT",
    0x118: "BPL5DAT",
    0x120: "SPR0PTH",  0x122: "SPR0PTL", 0x124: "SPR1PTH", 0x126: "SPR1PTL",
    0x128: "SPR2PTH",  0x12A: "SPR2PTL", 0x12C: "SPR3PTH", 0x12E: "SPR3PTL",
    0x130: "SPR4PTH",  0x132: "SPR4PTL", 0x134: "SPR5PTH", 0x136: "SPR5PTL",
    0x138: "SPR6PTH",  0x13A: "SPR6PTL", 0x13C: "SPR7PTH", 0x13E: "SPR7PTL",
    0x180: "COLOR00",  0x182: "COLOR01", 0x184: "COLOR02", 0x186: "COLOR03",
}

SRC_NAMES = {0: "CPU", 1: "BLT", 2: "DSK", 3: "COP", 4: "AGNUS"}


def reg_name(addr: int) -> str:
    """Return 'NAME' for known chipset addresses, else $DFFxxx."""
    if 0x00DF_F000 <= addr <= 0x00DF_FFFF:
        local = addr - 0x00DF_F000
        return CHIPSET_REG_NAMES.get(local, f"${addr:08X}")
    return f"${addr:08X}"


def parse_line(line: str) -> Entry | None:
    m = LINE_RE.search(line)
    if not m:
        return None
    return Entry(
        retired=int(m["retired"]),
        src=int(m["src"]),
        pc=int(m["pc"], 16),
        addr=int(m["addr"], 16),
        wdata=int(m["wdata"], 16),
        be=m["be"],
        raw=line.rstrip(),
    )


def load(path: Path) -> list[Entry]:
    out = []
    with path.open() as f:
        for line in f:
            e = parse_line(line)
            if e is not None:
                out.append(e)
    return out


def fmt_entry(e: Entry) -> str:
    src_name = SRC_NAMES.get(e.src, str(e.src))
    return (f"r={e.retired:>10} {src_name:>5} pc=${e.pc:08X} "
            f"{reg_name(e.addr):>10} := ${e.wdata:08X} be={e.be}")


def positional_diff(a: list[Entry], b: list[Entry], context: int) -> int:
    print(f"ours has {len(a):,} entries; other has {len(b):,} entries")
    n = min(len(a), len(b))
    diff_idx = None
    for i in range(n):
        # Equality: address + value + byte enables.  src/pc/retired
        # naturally differ between sims and aren't part of the equality
        # test.
        if (a[i].addr == b[i].addr and a[i].wdata == b[i].wdata
                and a[i].be == b[i].be):
            continue
        diff_idx = i
        break
    if diff_idx is None and len(a) == len(b):
        print("streams are byte-identical (every write matches addr+wdata+be)")
        return 0
    if diff_idx is None:
        diff_idx = n
        print(f"streams match up to entry {n}, then one stream is longer:")
        print(f"  ours next: {fmt_entry(a[n]) if n < len(a) else '(end)'}")
        print(f"  other next: {fmt_entry(b[n]) if n < len(b) else '(end)'}")
        return 1
    print(f"\n=== first divergence at entry {diff_idx} ===")
    print(f"context (entries {max(0, diff_idx - context)}..{diff_idx - 1} — agree):")
    for i in range(max(0, diff_idx - context), diff_idx):
        print(f"  [{i:5}] {fmt_entry(a[i])}")
    print(f"DIFF at entry {diff_idx}:")
    print(f"  ours  : {fmt_entry(a[diff_idx])}")
    print(f"  other : {fmt_entry(b[diff_idx])}")
    print(f"following (entries {diff_idx + 1}..{diff_idx + context}):")
    for i in range(diff_idx + 1, min(len(a), diff_idx + context + 1)):
        same = (i < len(b) and a[i].addr == b[i].addr
                and a[i].wdata == b[i].wdata and a[i].be == b[i].be)
        marker = "    " if same else "DIFF"
        ours_str = fmt_entry(a[i])
        other_str = fmt_entry(b[i]) if i < len(b) else "(end)"
        print(f"  {marker} [{i:5}] ours : {ours_str}")
        if not same:
            print(f"         {' ' * 8} other: {other_str}")
    return 2


def by_address_summary(a: list[Entry], b: list[Entry] | None) -> int:
    """Coarse comparison: count writes per address."""
    ca = Counter(e.addr for e in a)
    print(f"\n== address-write counts ==")
    print(f"{'address':<10}  {'name':<10}  {'ours':>8}", end="")
    if b is not None:
        cb = Counter(e.addr for e in b)
        print(f"  {'other':>8}  {'delta':>8}")
    else:
        cb = Counter()
        print()
    addrs = sorted(set(ca) | set(cb))
    diffs = 0
    for addr in addrs:
        na = ca.get(addr, 0)
        nb = cb.get(addr, 0)
        if b is None:
            print(f"${addr:08X}  {reg_name(addr):<10}  {na:>8}")
        else:
            mark = "  " if na == nb else "**"
            if na != nb:
                diffs += 1
            print(f"${addr:08X}  {reg_name(addr):<10}  {na:>8}  {nb:>8}  "
                  f"{mark}{nb - na:+d}")
    if b is not None:
        print(f"\n{diffs} address(es) differ in write count.")
    return 0


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("ours", type=Path)
    ap.add_argument("other", type=Path, nargs="?")
    ap.add_argument("--context", type=int, default=5,
                    help="entries of context around divergence (default 5)")
    ap.add_argument("--by-addr", action="store_true",
                    help="coarse mode: count writes per address, not positional")
    ap.add_argument("--head", type=int, default=0,
                    help="print first N entries of each stream then exit")
    args = ap.parse_args(argv)

    a = load(args.ours)
    if args.head:
        print(f"== first {args.head} entries of {args.ours} ==")
        for i, e in enumerate(a[:args.head]):
            print(f"  [{i:5}] {fmt_entry(e)}")
        if args.other is not None:
            b = load(args.other)
            print(f"\n== first {args.head} entries of {args.other} ==")
            for i, e in enumerate(b[:args.head]):
                print(f"  [{i:5}] {fmt_entry(e)}")
        return 0

    b = load(args.other) if args.other is not None else None

    if args.by_addr:
        return by_address_summary(a, b)

    if b is None:
        print(f"single-stream mode ({args.ours}, {len(a):,} entries)")
        print("== first 10 entries ==")
        for i, e in enumerate(a[:10]):
            print(f"  [{i:5}] {fmt_entry(e)}")
        print("...")
        print("== last 5 entries ==")
        for i, e in enumerate(a[-5:], start=len(a) - 5):
            print(f"  [{i:5}] {fmt_entry(e)}")
        return 0
    return positional_diff(a, b, args.context)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
