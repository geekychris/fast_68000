#!/usr/bin/env python3
"""fsuae_cosim_bp.py — BP-with-skip aligned cosim of FS-UAE vs Verilator.

Replaces fsuae_cosim.py's step-count alignment (which fails because
FS-UAE's /v1/step?n=N counts cycles, not instructions).

For each checkpoint PC in our Verilator [Cosim] trace, this counts how
many times that PC was visited up to retired=R, sets BP-with-skip on
FS-UAE so it stops at the (count+1)-th hit, dumps CPU regs, compares.

Workflow:
  1. Parse Verilator [Cosim] trace, build:
       checkpoints[(pc, hit_count)] = (retired, regs)
     where hit_count is the 1-indexed visit number to that PC.
  2. Launch FS-UAE, reset.
  3. For each (pc, hit_count) in chronological retired order:
       a. Set BP at pc with skip=(hit_count-1).
       b. Resume FS-UAE, wait for pause.
       c. Get CPU regs.
       d. Compare with our checkpoint regs.
       e. Clear BP, continue.
  4. Report first divergence.

Usage:
  tools/fsuae_cosim_bp.py --veri-log build_kick_boot/run.log \\
    --port 8765 \\
    --fsuae-binary /tmp/fsuae-src/fs-uae \\
    --fsuae-config "/Users/chris/Documents/FS-UAE/Configurations/WB13-K13-A500-Reference.fs-uae" \\
    [--checkpoints 'unique' | 'sample:N']

  --checkpoints unique  → pick every PC visited exactly ONCE in the trace
                          (deterministic boot landmarks)
  --checkpoints sample:N → pick N evenly-spaced checkpoints
"""

import argparse
import json
import os
import socket
import subprocess
import sys
import time
import urllib.request
from collections import Counter


def call(method, path, port=8765, timeout=30):
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


def cpu_state(port):
    r = call("GET", "/v1/cpu", port)
    out = {"pc": int(r["pc"], 16), "sr": int(r["sr"], 16)}
    for i in range(8):
        out[f"d{i}"] = int(r[f"d{i}"], 16)
        out[f"a{i}"] = int(r[f"a{i}"], 16)
    return out


def parse_veri_cosim(log_path):
    """Yield (retired, pc, regs_dict) for each [Cosim] line."""
    with open(log_path) as f:
        for line in f:
            if not line.startswith("[Cosim] "):
                continue
            parts = line.strip().split()
            if len(parts) < 21:
                continue
            try:
                retired = int(parts[1])
            except ValueError:
                continue
            pc = int(parts[2], 16)
            regs = {"pc": pc, "sr": int(parts[20], 16)}
            for i in range(8):
                regs[f"d{i}"] = int(parts[4 + i], 16)
                regs[f"a{i}"] = int(parts[12 + i], 16)
            yield retired, pc, regs


def diff_regs(a, b):
    out = []
    for field in ["pc"] + [f"d{i}" for i in range(8)] + [f"a{i}" for i in range(8)]:
        if a[field] != b[field]:
            out.append((field, a[field], b[field]))
    # FS-UAE's SR sometimes drops S/T/I bits; compare CCR only.
    if (a["sr"] & 0x1F) != (b["sr"] & 0x1F):
        out.append(("sr_ccr", a["sr"] & 0x1F, b["sr"] & 0x1F))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--veri-log", required=True)
    ap.add_argument("--fsuae-binary", required=True)
    ap.add_argument("--fsuae-config", required=True)
    ap.add_argument("--port", type=int, default=8765)
    ap.add_argument("--checkpoints", default="unique",
                    help="'unique' (PCs visited exactly once) "
                         "or 'sample:N' (N evenly-spaced)")
    ap.add_argument("--max-cps", type=int, default=20,
                    help="cap number of checkpoints")
    ap.add_argument("--out", default="/tmp/fsuae_cosim_bp.json")
    args = ap.parse_args()

    print(f"[veri] parsing {args.veri_log}...")
    pc_counts = Counter()
    cps = []  # list of (retired, pc, hit_count, regs)
    for retired, pc, regs in parse_veri_cosim(args.veri_log):
        pc_counts[pc] += 1
        cps.append((retired, pc, pc_counts[pc], regs))
    print(f"[veri] {len(cps)} [Cosim] entries, {len(pc_counts)} unique PCs")

    # Pick checkpoints
    if args.checkpoints == "unique":
        unique_pcs = {pc for pc, c in pc_counts.items() if c == 1}
        chosen = [(r, pc, hc, regs) for (r, pc, hc, regs) in cps if pc in unique_pcs]
        # Cap and evenly subsample
        if len(chosen) > args.max_cps:
            step = len(chosen) // args.max_cps
            chosen = chosen[::step][:args.max_cps]
    elif args.checkpoints == "rom-unique":
        # PCs visited exactly once AND in Kickstart ROM ($F80000-$FFFFFF).
        # ROM PCs are guaranteed identical on FSU and us; slow-RAM PCs
        # are shifted by FSU's exec-patch injection (+$D38).
        unique_pcs = {pc for pc, c in pc_counts.items() if c == 1}
        chosen = [(r, pc, hc, regs) for (r, pc, hc, regs) in cps
                  if pc in unique_pcs and 0xF80000 <= pc <= 0xFFFFFF]
        if len(chosen) > args.max_cps:
            step = len(chosen) // args.max_cps
            chosen = chosen[::step][:args.max_cps]
    elif args.checkpoints.startswith("sample:"):
        n = int(args.checkpoints.split(":")[1])
        if n >= len(cps):
            chosen = cps
        else:
            step = len(cps) // n
            chosen = cps[::step][:n]
    else:
        print(f"[veri] unknown --checkpoints '{args.checkpoints}'", file=sys.stderr)
        return 2

    print(f"[veri] chose {len(chosen)} checkpoints (max-cps={args.max_cps})")
    print(f"[veri] first 5: " + ", ".join(
        f"r={r} pc=0x{pc:06X} hit={hc}" for r, pc, hc, _ in chosen[:5]))

    # Launch FS-UAE
    subprocess.run(["pkill", "-f", args.fsuae_binary], stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    env = os.environ.copy()
    env["FSUAE_RPC_PORT"] = str(args.port)
    env["FSUAE_RPC_PAUSE_AT_BOOT"] = "1"
    log = open("/tmp/fsuae_cosim_bp.log", "w")
    proc = subprocess.Popen(
        [args.fsuae_binary, args.fsuae_config],
        stdin=subprocess.DEVNULL, stdout=log, stderr=log, env=env)
    print(f"[fsuae] launched (PID {proc.pid})")
    if not wait_port(args.port):
        print("[fsuae] no RPC", file=sys.stderr)
        return 2
    if not wait_paused(args.port, timeout=5):
        call("POST", "/v1/pause", args.port)

    call("POST", "/v1/breakpoints/clear", args.port)
    call("POST", "/v1/reset?hard=1", args.port)
    if not wait_paused(args.port, timeout=5):
        print("[fsuae] reset didn't pause", file=sys.stderr)
    print(f"[fsuae] post-reset PC=0x{cpu_state(args.port)['pc']:08X}")

    trace = []
    first_diverge = None
    for i, (retired, pc, hit_count, veri_regs) in enumerate(chosen):
        # Set BP with skip=hit_count-1 (1st hit = skip 0).
        call("POST", "/v1/breakpoints/clear", args.port)
        call("POST", f"/v1/breakpoints?addr=0x{pc:X}&skip={hit_count-1}&oneshot=1", args.port)
        call("POST", "/v1/resume", args.port)
        if not wait_paused(args.port, timeout=60):
            print(f"[cp #{i}] BP at 0x{pc:X} hit#{hit_count} never fired", file=sys.stderr)
            break
        fsu_regs = cpu_state(args.port)
        diffs = diff_regs(fsu_regs, veri_regs)
        status = "MATCH" if not diffs else "DIVERGE"
        print(f"[cp #{i}] r={retired:8d} pc=0x{pc:06X} hit#{hit_count} {status}")
        if diffs:
            for f, fv, vv in diffs[:6]:
                print(f"    {f:5} FSU=0x{fv:08X} VERI=0x{vv:08X}")
            if first_diverge is None:
                first_diverge = (retired, pc, hit_count, diffs)
        trace.append({"retired": retired, "pc": pc, "hit": hit_count,
                      "fsu": fsu_regs, "veri": veri_regs,
                      "diffs": [(f, fv, vv) for f, fv, vv in diffs]})

    with open(args.out, "w") as f:
        json.dump({"first_diverge": first_diverge, "trace": trace}, f, indent=2,
                  default=str)
    print(f"\n[done] wrote {args.out}")
    if first_diverge:
        r, pc, hc, diffs = first_diverge
        print(f"[done] first divergence at r={r} pc=0x{pc:06X} hit#{hc}")
    else:
        print(f"[done] no divergence across {len(trace)} checkpoints")

    proc.terminate()
    return 0 if not first_diverge else 1


if __name__ == "__main__":
    sys.exit(main())
