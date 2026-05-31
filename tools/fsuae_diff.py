#!/usr/bin/env python3
"""fsuae_diff.py — bisect divergence between FS-UAE and our Verilator.

Captures FS-UAE chip RAM at a specified PC using the fsuae_remote_patch
RPC, runs our Verilator boot to the same PC via CHIPRAM_SNAP_PCS, and
diffs the two chip RAM images.  Reports the first byte that disagrees
and a context window around it.

Usage:
    tools/fsuae_diff.py --pc 0xFC0240 [--label after-zclr]
                        [--fsuae-binary /tmp/fsuae-src/fs-uae]
                        [--fsuae-config ~/Documents/FS-UAE/.../WB13-K13-A500-Reference.fs-uae]
                        [--port 8765]
                        [--snap-dir /tmp/fsuae_diff]

What this proves:
  - FS-UAE is a known-good Amiga reference.  Our Verilator must produce
    identical chip RAM state at the same boot PC.  Any divergence is an
    emulation bug (CPU, chipset, or memory map).
"""

import argparse
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import time
import urllib.request


def call(method, path, port=8765, timeout=5):
    req = urllib.request.Request(f"http://127.0.0.1:{port}{path}", method=method)
    return json.load(urllib.request.urlopen(req, timeout=timeout))


def wait_port(port, timeout=10):
    end = time.time() + timeout
    while time.time() < end:
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.5):
                return True
        except Exception:
            time.sleep(0.1)
    return False


def wait_paused(port, timeout=30):
    end = time.time() + timeout
    while time.time() < end:
        try:
            if call("GET", "/v1/state", port)["state"] == "paused":
                return True
        except Exception:
            pass
        time.sleep(0.05)
    return False


def fsuae_capture(pc, fsuae_binary, config, port, snap_dir):
    """Boot FS-UAE, hit BP at pc, dump chip RAM 0..$80000 + registers."""
    chip_path = os.path.join(snap_dir, f"fsuae_chip_pc{pc:06X}.bin")
    regs_path = os.path.join(snap_dir, f"fsuae_regs_pc{pc:06X}.json")

    # Kill any existing instance
    subprocess.run(["pkill", "-f", fsuae_binary], stderr=subprocess.DEVNULL)
    time.sleep(0.5)

    env = os.environ.copy()
    env["FSUAE_RPC_PORT"] = str(port)
    env["FSUAE_RPC_PAUSE_AT_BOOT"] = "1"
    log = open(os.path.join(snap_dir, "fsuae.log"), "w")
    proc = subprocess.Popen(
        [fsuae_binary, config],
        stdin=subprocess.DEVNULL, stdout=log, stderr=log, env=env)
    print(f"[fsuae] launched (PID {proc.pid}), waiting for RPC...")
    if not wait_port(port):
        print("[fsuae] failed to come up; check log", file=sys.stderr)
        proc.terminate()
        return None, None

    # Should already be paused at boot
    if not wait_paused(port, timeout=5):
        call("POST", "/v1/pause", port)
        time.sleep(0.1)

    print(f"[fsuae] installing BP at 0x{pc:08X}")
    call("POST", "/v1/breakpoints/clear", port)
    call("POST", f"/v1/breakpoints?addr=0x{pc:X}", port)
    print(f"[fsuae] resetting...")
    call("POST", "/v1/reset?hard=1", port)
    call("POST", "/v1/resume", port)

    print(f"[fsuae] waiting for BP to fire (up to 30 s)...")
    if not wait_paused(port, timeout=30):
        print("[fsuae] BP never fired; PC may not be reachable on this boot path",
              file=sys.stderr)
        return None, None

    cpu = call("GET", "/v1/cpu", port)
    actual_pc = int(cpu["pc"], 16)
    print(f"[fsuae] paused at PC=0x{actual_pc:08X} (expected 0x{pc:08X})")
    if actual_pc != pc:
        print(f"[fsuae] WARN: paused at different PC — BP fired but CPU advanced",
              file=sys.stderr)

    print(f"[fsuae] dumping chip RAM (0..$80000 in 64 KB chunks)...")
    chip = bytearray()
    for off in range(0, 0x80000, 0x10000):
        r = call("GET", f"/v1/mem?addr=0x{off:X}&len=65536", port)
        chip.extend(bytes.fromhex(r["hex"]))
    with open(chip_path, "wb") as f:
        f.write(chip)
    print(f"[fsuae] wrote {chip_path} ({len(chip)} bytes)")

    # Also dump slow RAM ($C00000-$C7FFFF = 512 KB) since the K1.3
    # boot-bringup writes a *lot* of state there.
    slow_path = os.path.join(snap_dir, f"fsuae_slow_pc{pc:06X}.bin")
    print(f"[fsuae] dumping slow RAM ($C00000-$C7FFFF)...")
    slow = bytearray()
    for off in range(0xC00000, 0xC80000, 0x10000):
        r = call("GET", f"/v1/mem?addr=0x{off:X}&len=65536", port)
        slow.extend(bytes.fromhex(r["hex"]))
    with open(slow_path, "wb") as f:
        f.write(slow)
    print(f"[fsuae] wrote {slow_path} ({len(slow)} bytes)")

    # Custom chipset state too
    custom_path = os.path.join(snap_dir, f"fsuae_custom_pc{pc:06X}.json")
    custom = call("GET", "/v1/custom", port)
    with open(custom_path, "w") as f:
        json.dump(custom, f, indent=2)

    with open(regs_path, "w") as f:
        json.dump(cpu, f, indent=2)
    print(f"[fsuae] wrote {regs_path}")

    proc.terminate()
    return chip_path, regs_path


def verilator_capture(pc, snap_dir):
    """Run Verilator boot with CHIPRAM_SNAP at pc, return path to snap."""
    # We need the cosim_window or test_kickstart_boot binary already built.
    # Easier path: invoke the same Makefile target that runs the boot.
    veri_dir = os.path.abspath("build_cosim_window")
    binary = os.path.abspath(os.path.join(veri_dir, "Vm68k_top"))
    if not os.path.exists(binary):
        print(f"[veri] {binary} not found; build it with: make cosim-window",
              file=sys.stderr)
        return None

    # Clear any prior snap output
    out_dir = os.path.join(snap_dir, "veri_snaps")
    os.makedirs(out_dir, exist_ok=True)
    for f in os.listdir(out_dir):
        if f.endswith(".bin"):
            os.remove(os.path.join(out_dir, f))

    env = os.environ.copy()
    env["CHIPRAM_SNAP_PCS"] = f"{pc:06X}:diff"
    env["CHIPRAM_SNAP_DIR"] = os.path.abspath(out_dir)
    env["CHIPRAM_SNAP_LIMIT"] = "1"   # only first hit
    env["CHIPRAM_SNAP_HALT"] = "1"    # exit sim as soon as snap fires
    # Bound simulation
    cycles = "50000000"
    log_path = os.path.join(snap_dir, "veri.log")
    log = open(log_path, "w")
    print(f"[veri] running {binary} for up to {cycles} cycles, snapping at 0x{pc:X}...")
    # Run from the build dir because Vm68k_top opens rom.hex/disk.hex by
    # relative path.
    proc = subprocess.run(["./Vm68k_top", cycles], cwd=veri_dir,
                          stdout=log, stderr=log, env=env, timeout=600)
    print(f"[veri] exited rc={proc.returncode}")

    # Find the snap file
    matches = [f for f in os.listdir(out_dir) if f.startswith("snap_") and f.endswith(".bin")]
    if not matches:
        print(f"[veri] no snap produced — PC=0x{pc:X} not reached", file=sys.stderr)
        print(f"[veri] log tail:", file=sys.stderr)
        with open(log_path) as f:
            for line in f.readlines()[-20:]:
                sys.stderr.write("  " + line)
        return None
    snap_path = os.path.join(out_dir, matches[0])
    print(f"[veri] snap at {snap_path}")
    return snap_path


def diff_chip(fsuae_bin, veri_bin):
    """Compare two chip RAM binaries.  Returns (first_diff_offset, hunks)."""
    with open(fsuae_bin, "rb") as f: a = f.read()
    with open(veri_bin, "rb") as f: b = f.read()
    if len(a) != len(b):
        print(f"[diff] size mismatch: fsuae={len(a)} veri={len(b)}")
    n = min(len(a), len(b))
    first = None
    hunks = []
    in_hunk = False
    hunk_start = 0
    for i in range(n):
        if a[i] != b[i]:
            if first is None:
                first = i
            if not in_hunk:
                hunk_start = i
                in_hunk = True
        else:
            if in_hunk and (i - hunk_start) > 0:
                # close hunk if 16 same bytes in a row
                # (simple heuristic — coalesce nearby diffs)
                if i - hunk_start > 0:
                    pass
                # only close if we've seen 16 identical bytes
                run_ok = True
                for j in range(max(0, i - 16), i):
                    if j < n and a[j] != b[j]:
                        run_ok = False
                        break
                if run_ok and (i - hunk_start) > 0:
                    hunks.append((hunk_start, i))
                    in_hunk = False
    if in_hunk:
        hunks.append((hunk_start, n))
    return first, hunks, a, b


def print_hunk(a, b, start, end, max_lines=4):
    """Print a hex diff of bytes a[start..end] vs b[start..end]."""
    end = min(end, start + max_lines * 16)
    for off in range(start & ~0xF, end, 16):
        ha = " ".join(f"{a[off+i]:02x}" if a[off+i] != b[off+i] else f"\033[2m{a[off+i]:02x}\033[0m"
                      for i in range(16) if off+i < len(a))
        hb = " ".join(f"{b[off+i]:02x}" if a[off+i] != b[off+i] else f"\033[2m{b[off+i]:02x}\033[0m"
                      for i in range(16) if off+i < len(b))
        print(f"  ${off:06X}  FSUAE  {ha}")
        print(f"           VERI   {hb}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pc", required=True, help="hex PC to break at (e.g. 0xFC0240)")
    ap.add_argument("--label", default=None)
    ap.add_argument("--fsuae-binary", default="/tmp/fsuae-src/fs-uae")
    ap.add_argument("--fsuae-config",
        default=os.path.expanduser(
            "~/Documents/FS-UAE/Configurations/WB13-K13-A500-Reference.fs-uae"))
    ap.add_argument("--port", type=int, default=8765)
    ap.add_argument("--snap-dir", default="/tmp/fsuae_diff")
    ap.add_argument("--skip-fsuae", action="store_true",
                    help="reuse existing FS-UAE capture")
    ap.add_argument("--skip-veri", action="store_true",
                    help="reuse existing Verilator snap")
    args = ap.parse_args()

    pc = int(args.pc, 16) if args.pc.lower().startswith("0x") else int(args.pc, 16)
    os.makedirs(args.snap_dir, exist_ok=True)

    if not args.skip_fsuae:
        if not os.path.isfile(args.fsuae_binary):
            print(f"FS-UAE binary not found: {args.fsuae_binary}", file=sys.stderr)
            print("Build via: cd ~/code/claude_world/fsuae_remote_patch && ./build.sh",
                  file=sys.stderr)
            return 1
        fsuae_chip, fsuae_regs = fsuae_capture(
            pc, args.fsuae_binary, args.fsuae_config, args.port, args.snap_dir)
        if not fsuae_chip:
            return 2
    else:
        fsuae_chip = os.path.join(args.snap_dir, f"fsuae_chip_pc{pc:06X}.bin")
        fsuae_regs = os.path.join(args.snap_dir, f"fsuae_regs_pc{pc:06X}.json")
        print(f"[fsuae] reusing {fsuae_chip}")

    if not args.skip_veri:
        veri_chip = verilator_capture(pc, args.snap_dir)
        if not veri_chip:
            return 3
    else:
        veri_chip = os.path.join(args.snap_dir, "veri_snaps")
        files = [f for f in os.listdir(veri_chip) if f.endswith(".bin")]
        veri_chip = os.path.join(veri_chip, files[0]) if files else None
        if not veri_chip:
            print("[veri] no existing snap", file=sys.stderr)
            return 3
        print(f"[veri] reusing {veri_chip}")

    print()
    print("=== chip RAM diff ===")
    first, hunks, a, b = diff_chip(fsuae_chip, veri_chip)
    if first is None:
        print("  ZERO DIFFERENCES — emulators agree on chip RAM at this PC")
    else:
        print(f"  first divergence at offset $0x{first:06X}")
        print(f"  total hunks: {len(hunks)}")
        print()
        for i, (start, end) in enumerate(hunks[:5]):
            print(f"--- hunk {i+1}: $0x{start:06X}..$0x{end:06X} ({end-start} bytes) ---")
            print_hunk(a, b, start, end)
            print()
        if len(hunks) > 5:
            print(f"... and {len(hunks) - 5} more hunks")

    if fsuae_regs and os.path.isfile(fsuae_regs):
        with open(fsuae_regs) as f:
            cpu = json.load(f)
        print("=== FS-UAE CPU regs at PC ===")
        for k in ["pc", "sr", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
                  "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "usp", "isp"]:
            print(f"  {k.upper():3s} = {cpu.get(k, '???')}")

    # Look at the IRQ vector slots ($C0-$FC) — the smoking gun
    print()
    print("=== chip $C0-$FC (IRQ vectors 48-63) ===")
    print("  FS-UAE:")
    for i in range(16):
        off = 0xC0 + i * 4
        v = int.from_bytes(a[off:off+4], "big")
        print(f"    vec {i+48:2d} @ $0x{off:04X}: 0x{v:08X}")
    print("  VERI:")
    for i in range(16):
        off = 0xC0 + i * 4
        v = int.from_bytes(b[off:off+4], "big")
        print(f"    vec {i+48:2d} @ $0x{off:04X}: 0x{v:08X}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
