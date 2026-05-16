#!/usr/bin/env python3
"""Run every .s file in bench/ on the fast and slow simulator builds and
print a comparison table that includes the canonical M68000-PRM-derived
cycle count baked into each benchmark's `basic_68000_cycles=N` header.
"""
import os, re, sys, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASM  = os.path.join(ROOT, 'tb', 'asm68k.py')
BENCH_DIR = os.path.join(ROOT, 'bench')

def run_sim(sim_bin, build_dir, src):
    hexpath = os.path.join(build_dir, 'program.hex')
    subprocess.check_call(['python3', ASM, src, hexpath])
    out = subprocess.check_output(['./Vm68k_top', '2000000'], cwd=build_dir).decode()
    cycles = retired = None
    halted = False
    for line in out.splitlines():
        m = re.search(r'cycles=(\d+) halted_mask=0x([0-9a-f]+)', line)
        if m:
            cycles = int(m.group(1))
            halted = int(m.group(2), 16) != 0
        m = re.search(r'core0 halted=1 code=0x([0-9a-f]+) retired=(\d+) ipc=([\d.]+)', line)
        if m:
            code = int(m.group(1), 16)
            retired = int(m.group(2))
            if code != 0:
                raise RuntimeError(f"benchmark {src} failed: halt code 0x{code:04x}\n{out}")
    if not halted or cycles is None or retired is None:
        raise RuntimeError(f"benchmark {src} did not halt cleanly:\n{out}")
    return cycles, retired

def extract_basic(src):
    with open(src) as f:
        for line in f:
            m = re.search(r'basic_68000_cycles\s*=\s*(\d+)', line)
            if m:
                return int(m.group(1))
    return None

def main():
    if len(sys.argv) != 3:
        print("usage: bench_report.py <fast_build_dir> <slow_build_dir>", file=sys.stderr)
        sys.exit(2)
    fast_dir, slow_dir = sys.argv[1], sys.argv[2]
    fast_bin = os.path.join(fast_dir, 'Vm68k_top')
    slow_bin = os.path.join(slow_dir, 'Vm68k_top')

    results = []
    for src in sorted(os.listdir(BENCH_DIR)):
        if not src.endswith('.s'): continue
        path = os.path.join(BENCH_DIR, src)
        name = src[:-2]
        basic = extract_basic(path)
        fc, fr = run_sim(fast_bin, fast_dir, path)
        sc, sr = run_sim(slow_bin, slow_dir, path)
        results.append((name, basic, fr, fc, sc))

    # Pretty table.
    print()
    print(f"{'benchmark':<10} {'retired':>8} {'68k_ref':>8} {'cpi_ref':>7}"
          f" {'fast':>7} {'cpi_fast':>8} {'slow':>7} {'cpi_slow':>8}"
          f" {'vs_68k':>7} {'vs_slow':>8}")
    print("-" * 92)
    for (name, basic, retired, fc, sc) in results:
        vs_ref = (basic / fc) if (basic and fc) else 0.0
        vs_slow = (sc / fc) if fc else 0.0
        cpi_ref  = (basic / retired) if (basic and retired) else 0.0
        cpi_fast = (fc / retired) if retired else 0.0
        cpi_slow = (sc / retired) if retired else 0.0
        b_str = f"{basic}" if basic is not None else "-"
        print(f"{name:<10} {retired:>8} {b_str:>8} {cpi_ref:>7.2f}"
              f" {fc:>7} {cpi_fast:>8.2f} {sc:>7} {cpi_slow:>8.2f}"
              f" {vs_ref:>6.2f}x {vs_slow:>7.2f}x")
    print()
    print("cpi_*       = cycles per instruction on each platform")
    print("vs_68k      = (basic-68000 reference cycles) / (our cached pipelined cycles)")
    print("vs_slow     = (our no-cache cycles)           / (our cached pipelined cycles)")
    print("68k_ref     = sum of canonical M68000PRM instruction execution times")

if __name__ == '__main__':
    main()
