#!/usr/bin/env python3
"""fsuae_find_writer.py — find the first instruction on FS-UAE that
writes to a chip RAM address.

Usage:
    tools/fsuae_find_writer.py --addr 0x10EB0 [--size 8] [--n 5]
                               [--fsuae-binary /tmp/fsuae-src/fs-uae]
                               [--config <config>] [--port 8765]

Boots FS-UAE paused, installs a watchpoint on [addr..addr+size) with
mustchange=1 (so only state-changing writes fire it), hard-resets,
resumes, and reports the trigger PC + register + disasm context.

Hitting "next" N times by re-resuming after each hit walks the first
N writers, useful for understanding the producer chain.
"""

import argparse
import json
import os
import socket
import subprocess
import sys
import time
import urllib.request


def call(method, path, port=8765, timeout=5):
    req = urllib.request.Request(f"http://127.0.0.1:{port}{path}", method=method)
    with urllib.request.urlopen(req, timeout=timeout) as r:
        return json.load(r)


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


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--addr", type=lambda s: int(s, 0), required=True,
                   help="chip RAM address to watch (e.g. 0x10EB0)")
    p.add_argument("--size", type=lambda s: int(s, 0), default=4,
                   help="bytes to watch (default 4)")
    p.add_argument("--n", type=int, default=3,
                   help="number of consecutive WP hits to capture (default 3)")
    p.add_argument("--fsuae-binary", default="/tmp/fsuae-src/fs-uae")
    p.add_argument("--config",
                   default=os.path.expanduser(
                       "~/Documents/FS-UAE/Configurations/"
                       "WB13-K13-A500-Reference.fs-uae"))
    p.add_argument("--port", type=int, default=8765)
    p.add_argument("--timeout", type=int, default=60,
                   help="seconds to wait for each WP hit (default 60)")
    args = p.parse_args()

    subprocess.run(["pkill", "-f", "fs-uae"], stderr=subprocess.DEVNULL)
    time.sleep(0.5)

    env = os.environ.copy()
    env["FSUAE_RPC_PORT"] = str(args.port)
    env["FSUAE_RPC_PAUSE_AT_BOOT"] = "1"
    log = open("/tmp/fsuae-find-writer.log", "w")
    proc = subprocess.Popen(
        [args.fsuae_binary, args.config],
        stdin=subprocess.DEVNULL, stdout=log, stderr=log, env=env)
    print(f"[fsuae] launched (PID {proc.pid})")
    try:
        if not wait_port(args.port):
            print("fs-uae RPC did not come up", file=sys.stderr); return 1
        if not wait_paused(args.port, timeout=5):
            call("POST", "/v1/pause", args.port); time.sleep(0.1)

        # WP: rwi=2 (write only), mustchange=1
        call("POST", "/v1/breakpoints/clear", args.port)
        call("POST", "/v1/watchpoints/clear", args.port)
        call("POST",
             f"/v1/watchpoints?addr=0x{args.addr:X}&size={args.size}"
             f"&rwi=W&mustchange=1", args.port)
        print(f"[fsuae] WP installed on $%X..$%X (size %d, mustchange=1)" %
              (args.addr, args.addr + args.size - 1, args.size))

        call("POST", "/v1/reset?hard=1", args.port)
        time.sleep(0.3)
        call("POST", "/v1/resume", args.port)

        for hit in range(1, args.n + 1):
            print(f"\n[fsuae] waiting for hit #{hit} ...")
            if not wait_paused(args.port, timeout=args.timeout):
                print(f"[fsuae] no WP hit within {args.timeout}s — stopping")
                break

            cpu = call("GET", "/v1/cpu", args.port)
            wpinfo = call("GET", "/v1/watchpoints/last", args.port)
            pc = int(cpu["pc"], 16)
            # Disasm 6 instructions ending at PC (rough back-walk = 18 bytes)
            disasm = call("GET",
                          f"/v1/disasm?addr=0x{max(0, pc-12):X}&count=8",
                          args.port)
            mem = call("GET",
                       f"/v1/mem?addr=0x{args.addr:X}&len={args.size}",
                       args.port)
            print(f"  PC=0x{pc:08X}   wp.addr=0x{int(wpinfo.get('addr','0'),16):X}"
                  f"  wp.pc=0x{int(wpinfo.get('pc','0'),16):X}"
                  f"  wp.value=0x{int(wpinfo.get('value','0'),16):X}")
            print(f"  mem@$%X = %s" % (args.addr, mem["hex"]))
            print(f"  Regs:")
            for k in ("d0","d1","d2","d3","d4","d5","d6","d7"):
                print(f"    {k} = {cpu.get(k,'?')}")
            for k in ("a0","a1","a2","a3","a4","a5","a6","a7"):
                print(f"    {k} = {cpu.get(k,'?')}")
            print(f"    sr = {cpu.get('sr','?')}")
            print(f"  Disasm around PC:")
            lines = disasm.get("lines", [])
            for line in lines:
                if isinstance(line, str):
                    print(f"    {line}")
                    continue
                addr = line.get("addr", "0") if isinstance(line, dict) else "0"
                try:
                    line_addr = int(addr, 16) if isinstance(addr, str) else addr
                except Exception:
                    line_addr = 0
                mark = "  *" if line_addr == pc else "   "
                hexs = line.get("hex","") if isinstance(line, dict) else ""
                text = line.get("text","") if isinstance(line, dict) else ""
                print(f"    {mark}{addr}: {hexs.ljust(20)} {text}")

            # Resume to find next writer
            if hit < args.n:
                call("POST", "/v1/resume", args.port)
                time.sleep(0.1)
    finally:
        try:
            proc.terminate()
            proc.wait(timeout=3)
        except Exception:
            proc.kill()

    return 0


if __name__ == "__main__":
    sys.exit(main())
