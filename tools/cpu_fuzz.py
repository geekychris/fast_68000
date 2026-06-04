#!/usr/bin/env python3
"""cpu_fuzz.py — differential CPU fuzz tester (Verilator vs Musashi).

Generates targeted random 68000 test programs and runs each through both
our Verilator core and Musashi (the reference 68000 emulator).  Reports
every test where halt codes differ — that's a real CPU divergence bug.

Why this exists: most CPU bugs found in the WB1.3-boot push were
"silent memory-destination writes that don't happen" (tasks #6, #15,
#22, #27, #41, #43, etc.).  Hand-written directed tests catch known
bugs but miss the long tail.  Differential fuzzing systematically
covers the (instruction × addressing-mode × operand-pattern) space.

Workflow:
    1. Pick an instruction template and addressing-mode template.
    2. Substitute random operand values.
    3. Wrap with a self-check: write a known value via the instr,
       read it back, branch to PASS/FAIL accordingly.
    4. STOP #1 on PASS, STOP #2..N on different FAIL paths.
    5. Build the test once with asm68k, run on Verilator + Musashi,
       compare exit codes.

If both halt with the same STOP code, the instruction agrees on this
operand pattern.  Different codes = bug to investigate.

Usage:
    # Quick smoke test (50 random fuzz cases):
    tools/cpu_fuzz.py --count 50

    # Heavier coverage (500 cases, only memory-destination writes):
    tools/cpu_fuzz.py --count 500 --class mem-dst

    # Re-run a specific seed for repro:
    tools/cpu_fuzz.py --seed 12345 --count 1

    # Save divergent tests for manual inspection:
    tools/cpu_fuzz.py --count 500 --save-divergent /tmp/cpu_fuzz_bugs/

Reusable: any new bug class can be added by editing INSTR_TEMPLATES
and ADDR_TEMPLATES.  The fuzzer doesn't care what's under test as
long as it produces a STOP code.
"""

import argparse
import os
import random
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
TB_DIR = REPO / "tb"
BUILD = REPO / "build"
MUSASHI_BUILD = REPO / "build_musashi"
TMP_DIR = Path("/tmp/cpu_fuzz")


# Addressing-mode templates.  Each is a (asm-syntax, setup-code) pair.
# setup-code runs BEFORE the instruction under test to ensure the
# operand resolves to a safe RAM address.
#
# SAFE RAM range we use: $1000..$2FFF (well below the test code at $400
# and ROM-shadow region at $F80000, with room for indexed addressing).
SIZE_BYTES = {"b": 1, "w": 2, "l": 4}

# Each entry: syntax string, setup(size_bytes) -> str, ea address actually touched.
ADDR_TEMPLATES = {
    "abs_w": {
        "syntax": "$1800.w",
        "setup": lambda nb: "",
        "ea_addr": 0x1800,
    },
    "abs_l": {
        "syntax": "$00001800",
        "setup": lambda nb: "",
        "ea_addr": 0x1800,
    },
    "an_ind": {
        "syntax": "(A0)",
        "setup": lambda nb: "        lea     $1800, A0\n",
        "ea_addr": 0x1800,
    },
    "an_plus": {
        # (A0)+ touches $1800 with A0 starting at $1800.
        "syntax": "(A0)+",
        "setup": lambda nb: "        lea     $1800, A0\n",
        "ea_addr": 0x1800,
    },
    "an_minus": {
        # -(A0) touches $1800 with A0 starting at $1800+size.
        "syntax": "-(A0)",
        "setup": lambda nb: f"        lea     ${0x1800 + nb:04X}, A0\n",
        "ea_addr": 0x1800,
    },
    "d16_an": {
        "syntax": "$10(A0)",
        "setup": lambda nb: "        lea     $17F0, A0\n",  # $17F0+$10 = $1800
        "ea_addr": 0x1800,
    },
    "d8_an_xn": {
        "syntax": "$04(A0,D1.L)",
        "setup": lambda nb: "        lea     $17F0, A0\n        moveq   #$0C, D1\n",
        # A0=$17F0, D1=$0C, d8=$04, ea = $17F0 + $0C + $04 = $1800
        "ea_addr": 0x1800,
    },
}


# Instruction templates.  Each is a function (size, addr_template) ->
# (asm-snippet, expected-rd-after-test).
#
# The asm-snippet ASSUMES:
#   - Initial value at ea = 0
#   - D0 holds the "source value" we're storing
#   - The test then reads ea back and compares against expected
#
# `expected_value` describes what should be at `ea` after the
# instruction executes.  The wrapper code below verifies and STOPs.


def gen_move_dst(size, src_reg, src_val, addr_tpl):
    """MOVE.{B,W,L} src_reg, <ea> — store src_reg to ea."""
    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    expected = src_val & mask
    return f"        move.{size}  {src_reg}, {addr_tpl['syntax']}\n", expected


def gen_clr_dst(size, src_reg, src_val, addr_tpl):
    """CLR.{B,W,L} <ea> — clears ea."""
    return f"        clr.{size}   {addr_tpl['syntax']}\n", 0


def gen_addi_dst(size, src_reg, src_val, addr_tpl):
    """ADDI.{B,W,L} #imm, <ea> — adds imm to ea (which is initially 0).
    Result at ea should equal imm."""
    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    expected = src_val & mask
    if size == "l":
        imm = f"#${src_val:08X}"
    elif size == "w":
        imm = f"#${src_val & 0xFFFF:04X}"
    else:
        imm = f"#${src_val & 0xFF:02X}"
    return f"        addi.{size}  {imm}, {addr_tpl['syntax']}\n", expected


def gen_andi_dst(size, src_reg, src_val, addr_tpl):
    """ANDI #imm, <ea> — ea AND imm.  ea starts as $FFFFFFFF; expected = imm."""
    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    expected = src_val & mask
    if size == "l":
        imm = f"#${src_val:08X}"
    elif size == "w":
        imm = f"#${src_val & 0xFFFF:04X}"
    else:
        imm = f"#${src_val & 0xFF:02X}"
    return f"        andi.{size}  {imm}, {addr_tpl['syntax']}\n", expected, 0xFFFFFFFF


def gen_addq_dst(size, src_reg, src_val, addr_tpl):
    """ADDQ #imm, <ea> — imm in 1..8.  Result = ea + imm."""
    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    q = (src_val % 8) + 1  # imm in 1..8
    expected = q & mask
    return f"        addq.{size}  #{q}, {addr_tpl['syntax']}\n", expected


def gen_bset_dst(size, src_reg, src_val, addr_tpl):
    """BSET #N, <ea> — set bit N (0..7 on byte memory dst).  ea starts at 0."""
    bit = src_val % 8
    expected = 1 << bit
    return f"        bset    #{bit}, {addr_tpl['syntax']}\n", expected


def gen_neg_dst(size, src_reg, src_val, addr_tpl):
    """NEG.{B,W,L} <ea> — negate ea.  Requires pre-load of src_val into ea."""
    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    expected = (-src_val) & mask
    return f"        neg.{size}   {addr_tpl['syntax']}\n", expected, src_val


def gen_scc_dst(size, src_reg, src_val, addr_tpl):
    """ST <ea> — store $FF to byte ea (always TRUE)."""
    # Scc is always byte size
    return f"        st       {addr_tpl['syntax']}\n", 0xFF


INSTRS = {
    "mem-dst": [
        ("move.b D7, ea",  gen_move_dst,  ("b", "D7"),   None),
        ("move.w D7, ea",  gen_move_dst,  ("w", "D7"),   None),
        ("move.l D7, ea",  gen_move_dst,  ("l", "D7"),   None),
        ("clr.b ea",       gen_clr_dst,   ("b", "D7"),   0xFFFFFFFF),  # pre-init = $FFFFFFFF
        ("clr.w ea",       gen_clr_dst,   ("w", "D7"),   0xFFFFFFFF),
        ("clr.l ea",       gen_clr_dst,   ("l", "D7"),   0xFFFFFFFF),
        ("addi.l ea",      gen_addi_dst,  ("l", "D7"),   0x00000000),
        ("addi.w ea",      gen_addi_dst,  ("w", "D7"),   0x00000000),
        ("addi.b ea",      gen_addi_dst,  ("b", "D7"),   0x00000000),
        ("addq.l ea",      gen_addq_dst,  ("l", "D7"),   0x00000000),
        ("bset.b ea",      gen_bset_dst,  ("b", "D7"),   0x00000000),
        ("neg.l ea",       gen_neg_dst,   ("l", "D7"),   None),
        ("st ea",          gen_scc_dst,   ("b", "D7"),   0x00000000),
    ],
}


# Source-side addressing templates: parallel to ADDR_TEMPLATES but use A2 instead
# of A0 and reference $2000 (separate from destination $1800).  This avoids
# An-aliasing between the source and destination EAs when both vary.
ADDR_TEMPLATES_SRC = {
    "abs_w_src": {
        "syntax": "$2000.w",
        "setup": lambda nb: "",
        "ea_addr": 0x2000,
    },
    "abs_l_src": {
        "syntax": "$00002000",
        "setup": lambda nb: "",
        "ea_addr": 0x2000,
    },
    "a2_ind": {
        "syntax": "(A2)",
        "setup": lambda nb: "        lea     $2000, A2\n",
        "ea_addr": 0x2000,
    },
    "a2_plus": {
        "syntax": "(A2)+",
        "setup": lambda nb: "        lea     $2000, A2\n",
        "ea_addr": 0x2000,
    },
    "a2_minus": {
        "syntax": "-(A2)",
        "setup": lambda nb: f"        lea     ${0x2000 + nb:04X}, A2\n",
        "ea_addr": 0x2000,
    },
    "d16_a2": {
        "syntax": "$10(A2)",
        "setup": lambda nb: "        lea     $1FF0, A2\n",
        "ea_addr": 0x2000,
    },
    "d8_a2_xn": {
        "syntax": "$04(A2,D3.L)",
        "setup": lambda nb: "        lea     $1FF0, A2\n        moveq   #$0C, D3\n",
        "ea_addr": 0x2000,
    },
}


def emit_test(test_id, instr_label, instr_gen, args, pre_init, addr_tpl, src_val):
    """Emit a single fuzz test ASM and return (asm_text, expected_value).

    The test:
      - Pre-initializes ea with `pre_init` (0 or 0xFFFFFFFF or src_val).
      - Sets D7 = src_val.
      - Runs addr_tpl setup (e.g., load A0 with operand base).
      - Runs the instruction under test.
      - Reads ea back, compares to expected.
      - STOPs with #1 on PASS, #2..N on FAIL.
    """
    size = args[0]
    src_reg = args[1]
    result = instr_gen(size, src_reg, src_val, addr_tpl)
    if len(result) == 2:
        instr_asm, expected = result
        ea_pre = pre_init if pre_init is not None else 0x00000000
    else:
        instr_asm, expected, ea_pre = result

    ea_addr = addr_tpl["ea_addr"]

    mask = {"b": 0xFF, "w": 0xFFFF, "l": 0xFFFFFFFF}[size]
    expected_masked = expected & mask

    asm = f"""; fuzz test {test_id}: {instr_label} | addr={addr_tpl['syntax']} | src=${src_val:08X}
        .org $400
start:
        ; Pre-init ea = ${ea_pre:08X} (always longword preset)
        move.l  #${ea_pre:08X}, ${ea_addr:04X}.w

        ; Source value in D7
        move.l  #${src_val:08X}, D7

        ; Addr-mode setup
{addr_tpl['setup'](SIZE_BYTES[size])}
        ; INSTRUCTION UNDER TEST
{instr_asm}
        ; Check result: read back with matching size into a cleared D6
        moveq   #0, D6
        move.{size}  ${ea_addr:04X}.w, D6
        cmp.l   #${expected_masked:08X}, D6
        bne     fail
        stop    #1                   ; PASS

fail:
        stop    #2                   ; FAIL (ea content != expected)
"""

    return asm, expected_masked


def emit_msd_test(test_id, size, src_tpl, dst_tpl, src_val):
    """Emit one MOVE.{size} <ea_src>, <ea_dst> mem-to-mem test.

    Pre-init source with src_val and dst with $DEADBEEF (so a missing
    write is detectable).  Run MOVE.  Verify dst-readback == src low bits.
    """
    nb = SIZE_BYTES[size]
    src_addr = src_tpl["ea_addr"]
    dst_addr = dst_tpl["ea_addr"]
    # Pre-init wrote src_val as a longword at src_addr.  Big-endian: a
    # move.b reads the high byte; move.w reads the high word; move.l the long.
    if size == "b":
        expected = (src_val >> 24) & 0xFF
    elif size == "w":
        expected = (src_val >> 16) & 0xFFFF
    else:
        expected = src_val & 0xFFFFFFFF

    asm = f"""; mem-src-dst test {test_id}: move.{size} {src_tpl['syntax']}, {dst_tpl['syntax']} | src=${src_val:08X}
        .org $400
start:
        ; Pre-init source $({src_addr:04X}) = ${src_val:08X} (long write)
        move.l  #${src_val:08X}, ${src_addr:04X}.w
        ; Pre-init dest $({dst_addr:04X}) = $DEADBEEF (so missing write shows up)
        move.l  #$DEADBEEF, ${dst_addr:04X}.w

        ; Addr-mode setup — source first then dest (independent An regs)
{src_tpl['setup'](nb)}{dst_tpl['setup'](nb)}
        ; INSTRUCTION UNDER TEST
        move.{size}  {src_tpl['syntax']}, {dst_tpl['syntax']}

        ; Check dst at the stored ea_addr, size-matched, into a cleared D6
        moveq   #0, D6
        move.{size}  ${dst_addr:04X}.w, D6
        cmp.l   #${expected:08X}, D6
        bne     fail
        stop    #1                   ; PASS
fail:
        stop    #2                   ; FAIL
"""
    return asm, expected


def assemble(asm_text, hex_path, log_path):
    """Run asm68k.py on the asm text, produce a .hex file.  Return success."""
    asm_path = hex_path.with_suffix(".s")
    asm_path.write_text(asm_text)
    try:
        r = subprocess.run(
            [sys.executable, str(TB_DIR / "asm68k.py"), str(asm_path), str(hex_path)],
            capture_output=True, text=True, timeout=10,
        )
        log_path.write_text(r.stdout + r.stderr)
        return r.returncode == 0
    except subprocess.TimeoutExpired:
        log_path.write_text("asm68k timeout\n")
        return False


def run_veri(hex_path, log_path, timeout=10):
    """Run the hex through our Verilator binary.  Return STOP code (from exit code)."""
    # Copy hex to build/program.hex, run Vm68k_top, return exit code.
    bin_path = BUILD / "Vm68k_top"
    if not bin_path.exists():
        raise FileNotFoundError(f"{bin_path} not built — run `make build` first")
    prog = BUILD / "program.hex"
    prog.write_bytes(hex_path.read_bytes())
    try:
        r = subprocess.run(
            [str(bin_path), "1000000"],
            cwd=str(BUILD), capture_output=True, text=True, timeout=timeout,
        )
        log_path.write_text(r.stdout + r.stderr)
        return r.returncode
    except subprocess.TimeoutExpired:
        log_path.write_text("Verilator timeout\n")
        return -1


def run_musashi(hex_path, log_path, timeout=10):
    """Run the hex through Musashi.  Return STOP code."""
    bin_path = MUSASHI_BUILD / "musashi_sim"
    if not bin_path.exists():
        raise FileNotFoundError(f"{bin_path} not built — run `make musashi` first")
    prog = MUSASHI_BUILD / "program.hex"
    prog.write_bytes(hex_path.read_bytes())
    try:
        r = subprocess.run(
            [str(bin_path), "program.hex", "1000000"],
            cwd=str(MUSASHI_BUILD), capture_output=True, text=True, timeout=timeout,
        )
        log_path.write_text(r.stdout + r.stderr)
        return r.returncode
    except subprocess.TimeoutExpired:
        log_path.write_text("Musashi timeout\n")
        return -1


def fuzz_one_msd(test_id, rng, work_dir):
    """Generate one mem→mem MOVE fuzz test."""
    size = rng.choice(["b", "w", "l"])
    src_tpl_name, src_tpl = rng.choice(list(ADDR_TEMPLATES_SRC.items()))
    dst_tpl_name, dst_tpl = rng.choice(list(ADDR_TEMPLATES.items()))
    src_val = rng.randint(0, 0xFFFFFFFF)

    asm_text, expected = emit_msd_test(test_id, size, src_tpl, dst_tpl, src_val)

    hex_path = work_dir / f"t{test_id:05d}.hex"
    log_path = work_dir / f"t{test_id:05d}.log"
    instr_label = f"move.{size} src,dst"
    addr_label = f"{src_tpl_name}→{dst_tpl_name}"

    if not assemble(asm_text, hex_path, log_path):
        return {"id": test_id, "status": "asm_fail",
                "instr": instr_label, "addr": addr_label, "src": src_val,
                "asm": asm_text}

    veri_rc = run_veri(hex_path, work_dir / f"t{test_id:05d}.veri.log")
    mus_rc  = run_musashi(hex_path, work_dir / f"t{test_id:05d}.mus.log")

    if veri_rc == 1 and mus_rc == 1:
        status = "match_pass"
    elif veri_rc == 2 and mus_rc == 2:
        status = "match_both_fail"
    elif veri_rc != mus_rc:
        status = "diverge"
    else:
        status = "match"

    return {
        "id": test_id, "status": status,
        "instr": instr_label, "addr": addr_label, "src": src_val,
        "veri_rc": veri_rc, "mus_rc": mus_rc, "asm": asm_text,
        "hex_path": str(hex_path),
    }


def fuzz_one(test_id, instrs, addr_tpls, rng, work_dir):
    """Generate one fuzz test, run on both, compare.  Return dict of results."""
    instr_label, instr_gen, args, pre_init = rng.choice(instrs)
    addr_tpl_name, addr_tpl = rng.choice(list(addr_tpls.items()))
    src_val = rng.randint(0, 0xFFFFFFFF)

    asm_text, expected = emit_test(
        test_id, instr_label, instr_gen, args, pre_init, addr_tpl, src_val)

    hex_path = work_dir / f"t{test_id:05d}.hex"
    log_path = work_dir / f"t{test_id:05d}.log"

    if not assemble(asm_text, hex_path, log_path):
        return {"id": test_id, "status": "asm_fail",
                "instr": instr_label, "addr": addr_tpl_name, "src": src_val,
                "asm": asm_text}

    veri_rc = run_veri(hex_path, work_dir / f"t{test_id:05d}.veri.log")
    mus_rc  = run_musashi(hex_path, work_dir / f"t{test_id:05d}.mus.log")

    status = "match" if veri_rc == mus_rc else "diverge"
    # Both running and both at #1 (=PASS) is the strongest signal
    if veri_rc == 1 and mus_rc == 1:
        status = "match_pass"
    elif veri_rc == 2 and mus_rc == 2:
        # both consider the instruction broken in same way — usually means
        # our test setup is wrong, not a real bug
        status = "match_both_fail"
    elif veri_rc != mus_rc:
        status = "diverge"

    return {
        "id": test_id, "status": status,
        "instr": instr_label, "addr": addr_tpl_name, "src": src_val,
        "veri_rc": veri_rc, "mus_rc": mus_rc, "asm": asm_text,
        "hex_path": str(hex_path),
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", type=int, default=50,
                    help="number of fuzz tests to run")
    ap.add_argument("--seed", type=int, default=None,
                    help="RNG seed (omit for time-based)")
    ap.add_argument("--class", dest="cls", default="mem-dst",
                    choices=list(INSTRS.keys()) + ["mem-src-dst"],
                    help="instruction class to fuzz")
    ap.add_argument("--work-dir", default=str(TMP_DIR),
                    help="where to put generated tests")
    ap.add_argument("--save-divergent", default=None,
                    help="copy divergent tests here for manual inspection")
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    rng = random.Random(args.seed if args.seed is not None else None)
    seed_info = f"seed={args.seed}" if args.seed is not None else "seed=time-based"

    work_dir = Path(args.work_dir)
    work_dir.mkdir(parents=True, exist_ok=True)

    if args.cls == "mem-src-dst":
        instrs = None
        addr_tpls = None
        n_combos = len(ADDR_TEMPLATES_SRC) * len(ADDR_TEMPLATES) * 3  # 3 sizes
        print(f"[cpu_fuzz] {seed_info} class={args.cls} count={args.count}")
        print(f"[cpu_fuzz] {len(ADDR_TEMPLATES_SRC)} src × {len(ADDR_TEMPLATES)} dst × 3 sizes = {n_combos} combinations")
    else:
        instrs = INSTRS[args.cls]
        addr_tpls = ADDR_TEMPLATES
        print(f"[cpu_fuzz] {seed_info} class={args.cls} count={args.count}")
        print(f"[cpu_fuzz] {len(instrs)} instr templates × {len(addr_tpls)} "
              f"addr templates = {len(instrs)*len(addr_tpls)} combinations")
    print(f"[cpu_fuzz] work dir: {work_dir}")
    print()

    pass_count, fail_count, asm_fail = 0, 0, 0
    divergent = []
    asm_fails_seen = set()

    for i in range(args.count):
        if args.cls == "mem-src-dst":
            r = fuzz_one_msd(i, rng, work_dir)
        else:
            r = fuzz_one(i, instrs, addr_tpls, rng, work_dir)

        if r["status"] == "match_pass":
            pass_count += 1
            if args.verbose:
                print(f"  [{i:>5d}] PASS  {r['instr']:<18s} {r['addr']:<12s} src=${r['src']:08X}")
        elif r["status"] == "match_both_fail":
            # both diverged from expected, probably test bug
            asm_fail += 1
            key = (r["instr"], r["addr"])
            if key not in asm_fails_seen:
                asm_fails_seen.add(key)
                print(f"  [{i:>5d}] both_fail (test/expected bug?) {r['instr']:<18s} {r['addr']:<12s}")
        elif r["status"] == "asm_fail":
            asm_fail += 1
            key = (r["instr"], r["addr"])
            if key not in asm_fails_seen:
                asm_fails_seen.add(key)
                print(f"  [{i:>5d}] asm_fail (template bug) {r['instr']:<18s} {r['addr']:<12s}")
        else:  # diverge — real bug!
            fail_count += 1
            divergent.append(r)
            print(f"  [{i:>5d}] ⚠ DIVERGE {r['instr']:<18s} {r['addr']:<12s} "
                  f"src=${r['src']:08X} veri={r['veri_rc']} mus={r['mus_rc']}")

    print()
    print(f"[cpu_fuzz] {pass_count} match-pass, {fail_count} DIVERGE, "
          f"{asm_fail} asm-fail/both-fail (of {args.count} total)")

    if divergent and args.save_divergent:
        save_dir = Path(args.save_divergent)
        save_dir.mkdir(parents=True, exist_ok=True)
        for r in divergent:
            test_dir = save_dir / f"test_{r['id']:05d}_{r['instr'].replace('.', '_').replace(' ', '_')}"
            test_dir.mkdir(parents=True, exist_ok=True)
            (test_dir / "test.s").write_text(r["asm"])
            (test_dir / "info.txt").write_text(
                f"instr: {r['instr']}\naddr: {r['addr']}\n"
                f"src: ${r['src']:08X}\nveri_rc: {r['veri_rc']}\nmus_rc: {r['mus_rc']}\n")
        print(f"[cpu_fuzz] saved {len(divergent)} divergent tests to {save_dir}")

    return 1 if divergent else 0


if __name__ == "__main__":
    sys.exit(main())
