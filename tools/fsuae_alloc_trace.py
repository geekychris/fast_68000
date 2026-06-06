#!/usr/bin/env python3
"""Trace exec.Allocate calls in FS-UAE via the RPC breakpoint API.

Sets a breakpoint at $FC16D8 (Allocate entry).  Each time FS-UAE pauses
on the breakpoint, the script reads D0+A0+SP+stack-top (caller PC),
then steps off and resumes.  Emits one ALLOC line per hit; matches the
format of our Verilator ALLOC_TRACE so logs can be diffed.

Polls /v1/cpu — if PC matches the BP, we're paused there; capture and
advance.  Uses /v1/step?n=1 to advance one instruction past the BP, then
/v1/resume to continue.

Usage:
    fsuae_alloc_trace.py [--port 8765] [--max-hits 1000] [--bp 0xFC16D8]
                         [--out /tmp/fsuae_allocs.txt]
"""

import argparse
import json
import sys
import time
import urllib.request


def call(port, method, path):
    url = f'http://127.0.0.1:{port}{path}'
    req = urllib.request.Request(url, method=method)
    try:
        return json.loads(urllib.request.urlopen(req, timeout=5).read())
    except Exception as e:
        return {'ok': False, 'err': str(e)}


def get(port, path):  return call(port, 'GET',  path)
def post(port, path): return call(port, 'POST', path)


def read_mem(port, addr, n):
    url = f'http://127.0.0.1:{port}/v1/mem?addr=0x{addr:x}&len={n}'
    r = json.loads(urllib.request.urlopen(url, timeout=5).read())
    return bytes.fromhex(r['hex'])


def main():
    ap = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--port', type=int, default=8765)
    ap.add_argument('--max-hits', type=int, default=1000)
    ap.add_argument('--bp', default='0xFC16D8',
                    help='Breakpoint addr (Allocate entry; %(default)s)')
    ap.add_argument('--out', default='/tmp/fsuae_allocs.txt')
    args = ap.parse_args()

    bp_addr = int(args.bp, 16)

    post(args.port, '/v1/breakpoints/clear')
    r = post(args.port, f'/v1/breakpoints?addr={args.bp}&skip=0&oneshot=0')
    if not r.get('ok'):
        print(f'failed to set BP: {r}')
        return 1
    print(f'[FSU-ALLOC] BP at {args.bp}', flush=True)

    # If FS-UAE is currently paused on the BP, capture immediately.
    # Otherwise resume and wait for first hit.
    post(args.port, '/v1/resume')

    hits = 0
    out = open(args.out, 'w', buffering=1)  # line-buffered
    start_t = time.time()

    try:
        while hits < args.max_hits:
            # Poll for paused-at-BP state.
            time.sleep(0.005)
            cpu = get(args.port, '/v1/cpu')
            if not cpu.get('ok'):
                continue
            pc = int(cpu['pc'], 16)
            if pc != bp_addr:
                continue

            # We're at the BP.  Capture.
            d0 = int(cpu['d0'], 16)
            a0 = int(cpu['a0'], 16)
            a7 = int(cpu['a7'], 16)
            try:
                ret = int.from_bytes(read_mem(args.port, a7, 4), 'big')
            except Exception:
                ret = 0
            hits += 1
            line = (f'[ALLOC] hit={hits} D0={d0:d} ${d0:08X} '
                    f'A0=${a0:08X} SP=${a7:08X} ret=${ret:08X}')
            print(line, flush=True)
            out.write(line + '\n')

            # Step one instruction past the BP and resume.
            post(args.port, '/v1/step?n=1')
            # /v1/step pauses after stepping; resume.
            post(args.port, '/v1/resume')
    except KeyboardInterrupt:
        print(f'\n[FSU-ALLOC] interrupted at {hits} hits')

    elapsed = time.time() - start_t
    print(f'[FSU-ALLOC] {hits} hits in {elapsed:.1f}s '
          f'({hits/max(elapsed,0.001):.1f} hits/s)')
    out.close()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
