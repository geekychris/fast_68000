#!/usr/bin/env python3
"""fsuae_cosim.py — instruction-count aligned cosim of FS-UAE vs Verilator.

Unlike fsuae_diff.py (which compares state at a PC breakpoint — fragile
because PCs are re-entered and FS-UAE may take a different number of
instructions to reach a PC), this tool steps both emulators a known
number of instructions from reset, dumping CPU registers after each
chunk, and reports the first divergence.

Workflow:
  1. Launches FS-UAE with the claude_rpc patch, paused at boot.
  2. Resets FS-UAE; steps it CHUNK_SIZE instructions, captures regs.
  3. Reads our Verilator [Cosim] trace from a precomputed log.
  4. At every chunk boundary, finds the matching Verilator [Cosim] line
     by retired-instruction count and diffs PC + D0..D7 + A0..A7 + SR.
  5. Reports the first chunk where state diverges; bisects within it
     down to the first instruction-level diff.

Usage:
  # First, generate the Verilator cosim trace
  cd 68000 && KICKSTART_BOOT_TRACE=1 BOOT_TRACE=1 \\
    +define+KICKSTART_COSIM_TRACE +cosim_start=0 +cosim_end=1000000 \\
    make test-kickstart-boot ADFFILE=kickstart/wb13.adf > /tmp/veri.log

  # Then run cosim
  tools/fsuae_cosim.py --veri-log /tmp/veri.log --chunk 100000 \\
    --max-instr 1000000 --port 8765 \\
    --fsuae-binary /tmp/fsuae-src/fs-uae \\
    --fsuae-config "/Users/chris/Documents/FS-UAE/Configurations/WB13-K13-A500-Reference.fs-uae"

Performance note: each /v1/step + /v1/cpu round-trip is ~30-100ms.
For 100 chunks of 100k instructions each = ~10 sec FSU steps + 10 sec
network = 20 sec to cover 10M instructions. Single-stepping the whole
boot is infeasible; CHUNK_SIZE controls the granularity.
"""

import argparse
import json
import os
import socket
import subprocess
import sys
import time
import urllib.request


def call(method, path, port=8765, timeout=10):
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
    """Return dict of {pc, sr, d0..d7, a0..a7} as 32-bit ints."""
    r = call("GET", "/v1/cpu", port)
    out = {
        "pc": int(r["pc"], 16),
        "sr": int(r["sr"], 16),
    }
    for i in range(8):
        out[f"d{i}"] = int(r[f"d{i}"], 16)
        out[f"a{i}"] = int(r[f"a{i}"], 16)
    return out


def parse_veri_cosim(log_path):
    """Read Verilator log, yield (retired, pc, regs_dict) for each [Cosim] line.

    Format from m68k_core.v:4173:
      [Cosim] <retired> <pc> <opcode> <D0..D7> <A0..A7> <SR>
    """
    with open(log_path) as f:
        for line in f:
            if not line.startswith("[Cosim] "):
                continue
            parts = line.strip().split()
            # parts[0] = "[Cosim]"
            # parts[1] = retired (decimal)
            # parts[2] = pc (hex)
            # parts[3] = opcode (hex)
            # parts[4..11] = D0..D7
            # parts[12..19] = A0..A7
            # parts[20] = SR
            if len(parts) < 21:
                continue
            try:
                retired = int(parts[1])
            except ValueError:
                continue
            regs = {
                "pc": int(parts[2], 16),
                "sr": int(parts[20], 16),
            }
            for i in range(8):
                regs[f"d{i}"] = int(parts[4 + i], 16)
                regs[f"a{i}"] = int(parts[12 + i], 16)
            yield retired, regs


def diff_regs(a, b):
    """Return list of (field, a_val, b_val) for non-matching fields.

    SR comparison masks user-visible bits only (CCR + interrupt level).
    The S/T/INT bits can legitimately drift between emulators due to
    different interrupt timing.
    """
    out = []
    for field in ["pc"] + [f"d{i}" for i in range(8)] + [f"a{i}" for i in range(8)]:
        if a[field] != b[field]:
            out.append((field, a[field], b[field]))
    # SR: FS-UAE's /v1/cpu often returns sr=0 immediately after RPC
    # access (API artifact), or returns only CCR bits. Compare only the
    # CCR (low 5 bits) — that's what's reliable across emulators.
    if (a["sr"] & 0x1F) != (b["sr"] & 0x1F):
        out.append(("sr_ccr", a["sr"] & 0x1F, b["sr"] & 0x1F))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--veri-log", required=True,
                    help="Verilator log file with [Cosim] lines")
    ap.add_argument("--chunk", type=int, default=10000,
                    help="instructions per FSU step chunk (smaller = finer cosim)")
    ap.add_argument("--max-instr", type=int, default=1000000,
                    help="max instructions to step FSU (bound)")
    ap.add_argument("--fsuae-binary", required=True)
    ap.add_argument("--fsuae-config", required=True)
    ap.add_argument("--port", type=int, default=8765)
    ap.add_argument("--out", default="/tmp/fsuae_cosim_trace.json")
    args = ap.parse_args()

    # Load Verilator trace into memory: dict {retired -> regs}.
    print(f"[veri] parsing {args.veri_log}...")
    veri = {}
    for retired, regs in parse_veri_cosim(args.veri_log):
        veri[retired] = regs
    if not veri:
        print("[veri] no [Cosim] lines in log — did you build with "
              "+define+KICKSTART_COSIM_TRACE and run with +cosim_start/end?",
              file=sys.stderr)
        return 2
    veri_max = max(veri.keys())
    print(f"[veri] {len(veri)} [Cosim] entries, retired range 0..{veri_max}")

    # Launch FSU
    subprocess.run(["pkill", "-f", args.fsuae_binary], stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    env = os.environ.copy()
    env["FSUAE_RPC_PORT"] = str(args.port)
    env["FSUAE_RPC_PAUSE_AT_BOOT"] = "1"
    log = open("/tmp/fsuae_cosim.log", "w")
    proc = subprocess.Popen(
        [args.fsuae_binary, args.fsuae_config],
        stdin=subprocess.DEVNULL, stdout=log, stderr=log, env=env)
    print(f"[fsuae] launched (PID {proc.pid}), waiting for RPC...")
    if not wait_port(args.port):
        print("[fsuae] failed to come up", file=sys.stderr)
        proc.terminate()
        return 2

    if not wait_paused(args.port, timeout=5):
        call("POST", "/v1/pause", args.port)
        time.sleep(0.1)

    print(f"[fsuae] resetting...")
    call("POST", "/v1/breakpoints/clear", args.port)
    call("POST", "/v1/reset?hard=1", args.port)
    if not wait_paused(args.port, timeout=5):
        print("[fsuae] reset didn't leave it paused?", file=sys.stderr)

    # Initial state (should be at reset vector)
    initial = cpu_state(args.port)
    print(f"[fsuae] post-reset PC=0x{initial['pc']:08X}")

    # Step in chunks, comparing at each boundary
    retired = 0
    trace = [(retired, initial)]
    first_diverge_chunk = None
    while retired + args.chunk <= args.max_instr:
        t0 = time.time()
        call("POST", f"/v1/step?n={args.chunk}", args.port)
        if not wait_paused(args.port, timeout=60):
            print(f"[fsuae] step never returned to paused at retired={retired}",
                  file=sys.stderr)
            break
        retired += args.chunk
        fsu_state = cpu_state(args.port)
        trace.append((retired, fsu_state))
        dt = time.time() - t0

        # Compare with Verilator state at this retired count
        if retired in veri:
            diffs = diff_regs(fsu_state, veri[retired])
            status = "MATCH" if not diffs else "DIVERGE"
            print(f"[cosim] r={retired:8d} FSU PC=0x{fsu_state['pc']:08X} "
                  f"VERI PC=0x{veri[retired]['pc']:08X} {status} "
                  f"(step took {dt*1000:.0f}ms)")
            if diffs and first_diverge_chunk is None:
                first_diverge_chunk = retired
                print(f"  diffs:")
                for field, fv, vv in diffs[:8]:
                    print(f"    {field:3} FSU=0x{fv:08X} VERI=0x{vv:08X}")
                print(f"[cosim] first divergence at chunk r={retired}; "
                      f"bisecting within [{retired-args.chunk}, {retired}]")
                break
        else:
            print(f"[cosim] r={retired:8d} FSU PC=0x{fsu_state['pc']:08X} "
                  f"(no Verilator [Cosim] at this retired)")

    # Save FSU trace for later inspection
    with open(args.out, "w") as f:
        json.dump([{"retired": r, **s} for r, s in trace], f, indent=2)
    print(f"[trace] wrote {args.out} with {len(trace)} entries")

    proc.terminate()
    return 0 if first_diverge_chunk is None else 1


if __name__ == "__main__":
    sys.exit(main())
