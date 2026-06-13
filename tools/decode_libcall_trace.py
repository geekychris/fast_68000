#!/usr/bin/env python3
"""Decode [LIBCALL] lines from a sim run.log into a human-readable
summary.

Counts each library function call, tallies common arguments (memory
sizes, signal masks, etc.) and prints a histogram so you can see what
the system is doing at a glance.

Designed to be paired with `rtl/k13_libcall_probes.v` — that file
emits one [LIBCALL] line per call.  This tool consumes those lines.

Usage:
    decode_libcall_trace.py build_kick_boot/run.log
    decode_libcall_trace.py build_kick_boot/run.log --filter AllocMem
    decode_libcall_trace.py build_kick_boot/run.log --first-of LoadSeg

By default prints a summary (call counts + top arguments).  Use
--filter to dump every line for a specific function, --first-of to dump
only the first occurrence (handy for "did Loadseg ever fire").
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, defaultdict


LINE_RE = re.compile(
    r"\[LIBCALL\]\s+r=(?P<r>\d+)\s+(?P<fn>\w+)\((?P<args>[^)]*)\)(?P<rest>.*)$"
)


def parse_args_field(args_str: str) -> dict[str, str]:
    """Split 'size=128, flags=h0002' → {'size':'128', 'flags':'h0002'}."""
    out = {}
    for part in args_str.split(","):
        part = part.strip()
        if "=" not in part:
            continue
        k, v = part.split("=", 1)
        out[k.strip()] = v.strip()
    return out


def summarize(log_path: str, fn_filter: str | None,
              first_of: str | None) -> None:
    by_fn: dict[str, list[tuple[int, dict[str, str], str]]] = defaultdict(list)
    with open(log_path) as f:
        for line in f:
            m = LINE_RE.search(line)
            if not m:
                continue
            r = int(m.group("r"))
            fn = m.group("fn")
            args = parse_args_field(m.group("args"))
            rest = m.group("rest").strip()
            by_fn[fn].append((r, args, rest))

    if not by_fn:
        print("no [LIBCALL] lines found — were probes enabled?")
        print("  add +define+LIBCALL_ALL (or specific LIBCALL_*_TRACE) to "
              "VERI_DEFS, rebuild, re-run")
        return

    if first_of:
        rows = by_fn.get(first_of, [])
        if not rows:
            print(f"{first_of}: never called")
            return
        r, args, rest = rows[0]
        print(f"first {first_of}() at r={r}:  {args}  {rest}")
        return

    if fn_filter:
        rows = by_fn.get(fn_filter, [])
        if not rows:
            print(f"{fn_filter}: never called")
            return
        print(f"# {fn_filter}: {len(rows)} calls")
        for r, args, rest in rows:
            args_str = ", ".join(f"{k}={v}" for k, v in args.items())
            print(f"  r={r:>10}  {args_str}  {rest}")
        return

    # Full summary
    print(f"{'function':<16}  {'#calls':>8}  top args")
    print("-" * 60)
    for fn in sorted(by_fn.keys()):
        rows = by_fn[fn]
        count = len(rows)
        # Compute the most common value per arg key
        arg_counters: dict[str, Counter] = defaultdict(Counter)
        for _, args, _ in rows:
            for k, v in args.items():
                arg_counters[k][v] += 1
        top_args_parts = []
        for k, counter in arg_counters.items():
            most_common = counter.most_common(1)[0]
            top_args_parts.append(f"{k}={most_common[0]}×{most_common[1]}")
        print(f"  {fn:<14}  {count:>8}  {', '.join(top_args_parts)}")
    print()
    print(f"(use --filter <fn> to dump all calls; --first-of <fn> "
          f"for just the first)")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("logfile")
    ap.add_argument("--filter",
                    help="dump every call to this function")
    ap.add_argument("--first-of",
                    help="dump only the first call to this function")
    args = ap.parse_args()
    summarize(args.logfile, args.filter, args.first_of)


if __name__ == "__main__":
    main()
