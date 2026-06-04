#!/usr/bin/env python3
"""fsuae_validator_session.py — capture FS-UAE state at K1.3 trackdisk
validator's BSR return point ($FEAC80).

The validator at $FEAC62 calls a pattern-scan helper at $FEABCC which
returns A0 = pointer-to-found-pattern or A0 = $FFFFFFFF (-1) on failure.
At $FEAC80 the validator runs `CMPA.L #-1, A0` followed by `BEQ` to
the failure path at $FEAE68.

Captures:
  - CPU regs at $FEAC80 (so we see A0 directly)
  - Chip RAM in the scan range [A2-2 .. A2+$ABE] = K1.3 MFM buffer
  - Slow RAM around A3 (trackdisk task struct)
  - DSKSYNC + DSKLEN + ADKCON chipset state

Compare against our Verilator boot at same PC to find:
  1. Is the buffer being scanned identical between us and FSU?
  2. Does our scan helper at $FEABCC return -1 while FSU returns a ptr?
  3. If buffers differ, what specifically differs?

Usage:
  tools/fsuae_validator_session.py \\
    --fsuae-binary /tmp/fsuae-src/fs-uae \\
    --fsuae-config ".../WB13-K13-A500-Reference.fs-uae" \\
    --out /tmp/fsuae_validator/
"""

import argparse
import json
import os
import socket
import subprocess
import sys
import time
import urllib.request


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


def wait_paused(port, timeout=60):
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


def read_mem(port, addr, length):
    """Read N bytes from FSU memory, return as bytes."""
    out = bytearray()
    chunk = 4096
    pos = 0
    while pos < length:
        n = min(chunk, length - pos)
        r = call("GET", f"/v1/mem?addr=0x{addr+pos:X}&len={n}", port)
        out.extend(bytes.fromhex(r["hex"]))
        pos += n
    return bytes(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--fsuae-binary", required=True)
    ap.add_argument("--fsuae-config", required=True)
    ap.add_argument("--port", type=int, default=8765)
    ap.add_argument("--out", default="/tmp/fsuae_validator/")
    ap.add_argument("--bp-pc", default="0xFEAC80",
                    help="BP at this PC (default $FEAC80 = right after BSR returns)")
    ap.add_argument("--retry-skip", type=int, default=0,
                    help="skip N hits before capturing (each hit = one validator pass)")
    args = ap.parse_args()

    os.makedirs(args.out, exist_ok=True)
    bp_pc = int(args.bp_pc, 16)

    # Launch FSU
    subprocess.run(["pkill", "-f", args.fsuae_binary], stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    env = os.environ.copy()
    env["FSUAE_RPC_PORT"] = str(args.port)
    env["FSUAE_RPC_PAUSE_AT_BOOT"] = "1"
    log_path = os.path.join(args.out, "fsuae.log")
    log = open(log_path, "w")
    proc = subprocess.Popen(
        [args.fsuae_binary, args.fsuae_config],
        stdin=subprocess.DEVNULL, stdout=log, stderr=log, env=env)
    print(f"[fsuae] launched (PID {proc.pid})")
    if not wait_port(args.port):
        print("[fsuae] no RPC; check log", file=sys.stderr)
        return 2

    if not wait_paused(args.port, timeout=5):
        call("POST", "/v1/pause", args.port)
        time.sleep(0.2)

    call("POST", "/v1/breakpoints/clear", args.port)
    call("POST", "/v1/reset?hard=1", args.port)
    if not wait_paused(args.port, timeout=10):
        print("[fsuae] reset didn't leave it paused", file=sys.stderr)

    # Set BP at $FEAC80 with skip=N
    print(f"[fsuae] installing BP at 0x{bp_pc:08X} (skip={args.retry_skip})")
    call("POST",
         f"/v1/breakpoints?addr=0x{bp_pc:X}&skip={args.retry_skip}&oneshot=0",
         args.port)
    call("POST", "/v1/resume", args.port)

    print(f"[fsuae] waiting for BP to fire (up to 120 s)...")
    if not wait_paused(args.port, timeout=120):
        print(f"[fsuae] BP at $FEAC80 never fired", file=sys.stderr)
        proc.terminate()
        return 2

    # Capture state at the BP hit
    cpu = cpu_state(args.port)
    print(f"[fsuae] paused at PC=0x{cpu['pc']:08X}")
    print(f"  D0=0x{cpu['d0']:08X} A0=0x{cpu['a0']:08X} A2=0x{cpu['a2']:08X} A3=0x{cpu['a3']:08X}")

    # Save regs
    with open(os.path.join(args.out, f"regs_pc{bp_pc:06X}.json"), "w") as f:
        json.dump(cpu, f, indent=2)

    # Dump the buffer being scanned: [A2-2 .. A2-2 + $ABC + $40] (extra context)
    scan_start = cpu['a2'] - 2
    scan_len = 0xABC + 0x40
    print(f"[fsuae] dumping scan buffer chip [${scan_start:08X}..${scan_start+scan_len:08X}]")
    buf = read_mem(args.port, scan_start, scan_len)
    with open(os.path.join(args.out, f"scanbuf_pc{bp_pc:06X}.bin"), "wb") as f:
        f.write(buf)

    # Dump trackdisk task struct context: [A3-$10 .. A3+$80]
    if cpu['a3'] >= 0x10:
        print(f"[fsuae] dumping A3 context [${cpu['a3']-0x10:08X}..${cpu['a3']+0x80:08X}]")
        a3_ctx = read_mem(args.port, cpu['a3'] - 0x10, 0x90)
        with open(os.path.join(args.out, f"a3_ctx_pc{bp_pc:06X}.bin"), "wb") as f:
            f.write(a3_ctx)

    # Dump chipset state (DSKSYNC, DSKLEN, ADKCON)
    if call("GET", "/v1/custom", args.port):
        custom = call("GET", "/v1/custom", args.port)
        with open(os.path.join(args.out, f"custom_pc{bp_pc:06X}.json"), "w") as f:
            json.dump(custom, f, indent=2)

    # Print verdict
    if cpu['a0'] == 0xFFFFFFFF:
        print(f"\n[verdict] A0 = -1: scan FAILED on FSU at this BP hit")
        print(f"          (this is unexpected — FSU should succeed)")
    else:
        print(f"\n[verdict] A0 = 0x{cpu['a0']:08X}: scan SUCCEEDED on FSU")
        print(f"          (validator continues to next sector check)")
        # Show what's at A0 (the matched pattern)
        match_data = read_mem(args.port, cpu['a0'], 16)
        print(f"          mem[A0..A0+16] = {match_data.hex()}")

    print(f"\n[done] state saved to {args.out}")
    proc.terminate()
    return 0


if __name__ == "__main__":
    sys.exit(main())
