#!/usr/bin/env python3
"""diff_dumps.py — compare two chip-RAM dumps from screen_open_test runs.

Reads the marker pointer at chip $00000C00 in each dump, locates the
marker block (or hard-fail sentinel) and prints the field-by-field diff.
Always exits 0 unless one of the dumps is missing the sentinel entirely.

Usage:
    diff_dumps.py ours.bin fsuae.bin
    diff_dumps.py ours.bin           # single-side inspection, no diff
"""
from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

MAGIC = 0x54455354  # 'TEST'
POINTER_OFFSET = 0x0C00   # chip RAM offset holding the marker block ptr
HARD_FAIL_SENTINEL = (MAGIC, 0xFF)

STATUS_NAMES = {
    0: "in-progress (crashed mid-test)",
    1: "OpenLibrary failed",
    2: "OpenScreen failed",
    3: "OpenScreen OK (no follow-on, or follow-on not yet run)",
    4: "OpenWindow failed",
    5: "OpenWindow OK",
    6: "OpenWindow + UserPort allocated; UserPort snapshot taken",
    7: "OpenWindow OK but UserPort = NULL (silent fail)",
    255: "AllocMem hard-fail (no marker block)",
}

# Field layouts — kept in sync with the corresponding .s files.
# Common screen fields (offsets 0..47) are shared by all stubs.
FIELDS_COMMON = [
    ("magic",         0,  4, "L", "sentinel 'TEST'"),
    ("intuibase",     4,  4, "L", "IntuitionBase"),
    ("screen_ptr",    8,  4, "L", "Screen pointer"),
    ("status",       12,  4, "L", "status code"),
    ("left_top",     16,  4, "L", "Screen.LeftEdge.W|TopEdge.W"),
    ("width_height", 20,  4, "L", "Screen.Width.W|Height.W"),
    ("viewport0",    24,  4, "L", "Screen.ViewPort first long"),
    ("chipset",      28,  4, "L", "DMACONR.W|BPLCON0.W"),
    ("bm_brow_rows", 32,  4, "L", "BitMap.BytesPerRow.W|Rows.W"),
    ("bm_flags_dep", 36,  4, "L", "BitMap.Flags.B|Depth.B|pad.W"),
    ("bm_plane0",    40,  4, "L", "BitMap.Planes[0] (chip-RAM)"),
    ("bm_plane1",    44,  4, "L", "BitMap.Planes[1]"),
]

# Extra fields used by open_window_test.s (offsets 48+).  Decoded only
# when status >= 4 (i.e. OpenWindow was attempted), so a screen-only
# dump doesn't claim zero-Window data is meaningful.
FIELDS_WINDOW = [
    ("win_ptr",      48,  4, "L", "Window pointer"),
    ("win_lt",       52,  4, "L", "Window.LeftEdge.W|TopEdge.W"),
    ("win_wh",       56,  4, "L", "Window.Width.W|Height.W"),
    ("win_flags",    60,  4, "L", "Window.Flags"),
    ("win_idcmp",    64,  4, "L", "Window.IDCMPFlags"),
    ("win_rport",    68,  4, "L", "Window.RPort"),
    ("win_userport", 72,  4, "L", "Window.UserPort"),
    ("win_wscreen",  76,  4, "L", "Window.WScreen (should == screen_ptr)"),
]

# Extra fields used by idcmp_window_test.s (offsets 80+).  Decoded only
# when status >= 6 (UserPort snapshot was taken).
FIELDS_USERPORT = [
    ("mp_flags_sb",  80,  4, "L", "UserPort.mp_Flags.B|mp_SigBit.B|pad.W"),
    ("mp_sigtask",   84,  4, "L", "UserPort.mp_SigTask"),
    ("mp_head",      88,  4, "L", "UserPort.MsgList.mlh_Head"),
    ("mp_tail",      92,  4, "L", "UserPort.MsgList.mlh_Tail (=0 for empty MinList)"),
    ("mp_tailpred",  96,  4, "L", "UserPort.MsgList.mlh_TailPred"),
    ("mp_lnname",   100,  4, "L", "UserPort.ln_Name pointer"),
    ("win_idcmp2", 104,  4, "L", "Window.IDCMPFlags (readback)"),
    ("win_msgkey", 108,  4, "L", "Window.MessageKey"),
]

# Default for marker_block: just the common screen fields.  After
# decode we check `status` and append window fields if the stub got
# that far.
FIELDS = FIELDS_COMMON


def read_marker(blob: bytes) -> dict | None:
    """Locate the marker block from a chip-RAM dump; return field dict."""
    if len(blob) < POINTER_OFFSET + 4:
        return None
    (ptr,) = struct.unpack(">I", blob[POINTER_OFFSET:POINTER_OFFSET + 4])
    if ptr == MAGIC:
        # hard-fail path: marker pointer slot was overwritten with magic
        # and the status sits at $C04
        return {
            "_addr": POINTER_OFFSET,
            "magic": MAGIC,
            "status": struct.unpack(">I", blob[POINTER_OFFSET + 4:POINTER_OFFSET + 8])[0],
            "_path": "hard_fail",
        }
    if ptr < 0x100 or ptr + 32 > len(blob):
        return None
    # Verify sentinel
    sentinel = struct.unpack(">I", blob[ptr:ptr + 4])[0]
    if sentinel != MAGIC:
        return None
    out = {"_addr": ptr, "_path": "marker_block", "_fields": list(FIELDS_COMMON)}
    for name, off, sz, fmt, _desc in FIELDS_COMMON:
        out[name] = struct.unpack(">" + fmt, blob[ptr + off:ptr + off + sz])[0]
    # If status >= 4 the stub attempted OpenWindow; decode the extra fields.
    if out.get("status", 0) >= 4 and ptr + 80 <= len(blob):
        for name, off, sz, fmt, _desc in FIELDS_WINDOW:
            out[name] = struct.unpack(">" + fmt, blob[ptr + off:ptr + off + sz])[0]
        out["_fields"] = list(FIELDS_COMMON) + list(FIELDS_WINDOW)
    # If status >= 6 the stub took a UserPort snapshot.
    if out.get("status", 0) >= 6 and ptr + 112 <= len(blob):
        for name, off, sz, fmt, _desc in FIELDS_USERPORT:
            out[name] = struct.unpack(">" + fmt, blob[ptr + off:ptr + off + sz])[0]
        out["_fields"] = list(FIELDS_COMMON) + list(FIELDS_WINDOW) + list(FIELDS_USERPORT)
    return out


def fmt_field(name: str, val: int) -> str:
    if name == "status":
        return f"{val} ({STATUS_NAMES.get(val, 'unknown')})"
    if name == "magic":
        ascii_repr = "".join(
            chr(b) if 32 <= b < 127 else "."
            for b in val.to_bytes(4, "big")
        )
        return f"${val:08X} ('{ascii_repr}')"
    if name in ("left_top", "width_height"):
        hi = (val >> 16) & 0xFFFF
        lo = val & 0xFFFF
        return f"${val:08X} (hi={hi} lo={lo})"
    if name == "chipset":
        dmaconr = (val >> 16) & 0xFFFF
        bplcon0 = val & 0xFFFF
        return f"${val:08X} (DMACONR=${dmaconr:04X} BPLCON0=${bplcon0:04X})"
    return f"${val:08X}"


def show_marker(label: str, m: dict | None) -> None:
    print(f"== {label} ==")
    if m is None:
        print("  no marker found — sentinel missing from chip RAM dump")
        return
    print(f"  located via {m['_path']} at chip ${m['_addr']:06X}")
    for name, _o, _s, _f, desc in m.get("_fields", FIELDS_COMMON):
        if name not in m:
            continue
        print(f"  {name:<14} {fmt_field(name, m[name]):<40}  {desc}")


def show_diff(a: dict, b: dict, label_a: str, label_b: str) -> int:
    """Print field-by-field diff.  Returns count of differing fields."""
    diffs = 0
    print(f"\n== diff: {label_a} vs {label_b} ==")
    width = max(len(label_a), len(label_b))
    # Use the union of fields present in either marker so a screen-only
    # dump can be diffed against an open-window dump (the extra Window
    # fields show "missing" on the screen side).
    union_field_names: list[str] = []
    seen: set[str] = set()
    for src in (a.get("_fields", FIELDS_COMMON), b.get("_fields", FIELDS_COMMON)):
        for entry in src:
            if entry[0] not in seen:
                union_field_names.append(entry[0])
                seen.add(entry[0])
    field_index = {f[0]: f for f in FIELDS_COMMON + FIELDS_WINDOW + FIELDS_USERPORT}
    for name in union_field_names:
        _, _o, _s, _f, desc = field_index[name]
        va = a.get(name)
        vb = b.get(name)
        if va == vb:
            shown = fmt_field(name, va) if va is not None else "(missing)"
            print(f"  ok   {name:<14} {shown:<40}  {desc}")
        else:
            diffs += 1
            print(f"  DIFF {name:<14} {desc}")
            print(f"    {label_a:<{width}} = {fmt_field(name, va) if va is not None else '(missing)'}")
            print(f"    {label_b:<{width}} = {fmt_field(name, vb) if vb is not None else '(missing)'}")
    return diffs


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("ours", type=Path)
    ap.add_argument("other", type=Path, nargs="?")
    args = ap.parse_args(argv)

    ours_blob = args.ours.read_bytes()
    ours = read_marker(ours_blob)
    show_marker(f"ours [{args.ours}]", ours)

    if ours is None:
        print("\nERROR: ours dump has no marker.  Check that the test binary ran:")
        print("  - did the boot reach C:<stem>?")
        print("  - did AllocMem(MEMF_CHIP) return 0 (-> hard_fail path)?")
        return 1

    if args.other is None:
        return 0

    other_blob = args.other.read_bytes()
    other = read_marker(other_blob)
    show_marker(f"other [{args.other}]", other)
    if other is None:
        print("\nERROR: other dump has no marker.")
        return 1

    diffs = show_diff(ours, other, "ours", "other")
    print(f"\n{diffs} field(s) differ.")
    return 0 if diffs == 0 else 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
