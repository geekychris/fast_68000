#!/usr/bin/env python3
"""m68k disassembler for ROM exploration.

This is NOT a full 68000 disassembler -- it handles the subset of
opcodes that show up while reading Kickstart 1.3, with a focus on
producing readable output for tracing register-load chains rather
than perfect Motorola syntax.  Coverage is added when new opcodes
turn up; unknown words print as `DC.W $xxxx`.

Usage:
    disasm68k.py <bin> <addr> [--bytes N] [--base 0xF80000]

`<bin>` is a raw ROM (e.g. `kickstart/kick.bin`).  `<addr>` is the
program-address of the first byte to disassemble (defaults to the
ROM base).  `--bytes` controls how many bytes to walk (default 64).
"""
from __future__ import annotations

import argparse
import struct
import sys

ROM_BASE_DEFAULT = 0xF80000

# ---- helpers --------------------------------------------------------------

def signed_byte(b: int) -> int:
    return b - 256 if b & 0x80 else b


def signed_word(w: int) -> int:
    return w - 0x10000 if w & 0x8000 else w


def signed_long(l: int) -> int:
    return l - 0x100000000 if l & 0x80000000 else l


# ---- EA decoder ----------------------------------------------------------

def decode_ea(mode: int, reg: int, size: str, fetcher) -> tuple[str, int]:
    """Return (text, extra_words_consumed).

    `fetcher(i)` returns the i'th extra word past the opcode.
    """
    if mode == 0:
        return f"D{reg}", 0
    if mode == 1:
        return f"A{reg}", 0
    if mode == 2:
        return f"(A{reg})", 0
    if mode == 3:
        return f"(A{reg})+", 0
    if mode == 4:
        return f"-(A{reg})", 0
    if mode == 5:
        d = signed_word(fetcher(0))
        return f"${d & 0xFFFF:04X}(A{reg})", 1
    if mode == 6:
        ext = fetcher(0)
        xreg = (ext >> 12) & 7
        is_a = (ext >> 15) & 1
        is_long = (ext >> 11) & 1
        d8 = signed_byte(ext & 0xFF)
        x = f"{'A' if is_a else 'D'}{xreg}.{'L' if is_long else 'W'}"
        return f"${d8 & 0xFF:02X}(A{reg},{x})", 1
    if mode == 7:
        if reg == 0:        # abs.W
            a = signed_word(fetcher(0)) & 0xFFFFFFFF
            return f"${a & 0xFFFF:04X}.W", 1
        if reg == 1:        # abs.L
            hi = fetcher(0)
            lo = fetcher(1)
            return f"${(hi << 16) | lo:08X}", 2
        if reg == 2:        # d16(PC)
            d = signed_word(fetcher(0))
            return f"${d & 0xFFFF:04X}(PC)", 1
        if reg == 3:        # d8(PC, Xn)
            ext = fetcher(0)
            xreg = (ext >> 12) & 7
            is_a = (ext >> 15) & 1
            is_long = (ext >> 11) & 1
            d8 = signed_byte(ext & 0xFF)
            x = f"{'A' if is_a else 'D'}{xreg}.{'L' if is_long else 'W'}"
            return f"${d8 & 0xFF:02X}(PC,{x})", 1
        if reg == 4:        # immediate
            if size == "B":
                imm = fetcher(0) & 0xFF
                return f"#${imm:02X}", 1
            if size == "W":
                imm = fetcher(0) & 0xFFFF
                return f"#${imm:04X}", 1
            if size == "L":
                hi = fetcher(0); lo = fetcher(1)
                return f"#${(hi << 16) | lo:08X}", 2
    return f"<?mode={mode}/reg={reg}>", 0


SIZES = {0: "B", 1: "W", 2: "L"}
SIZE_BYTES = {"B": 1, "W": 2, "L": 4}
MOVE_SIZES = {1: "B", 3: "W", 2: "L"}
CC_NAMES = ["T", "F", "HI", "LS", "CC", "CS", "NE", "EQ",
            "VC", "VS", "PL", "MI", "GE", "LT", "GT", "LE"]


# ---- per-opcode handlers -------------------------------------------------

def disasm_one(words: list[int], pc: int) -> tuple[str, int]:
    """Disassemble a single instruction starting at `words[0]`.

    Returns (text, n_words_consumed).
    """
    op = words[0]

    def fetch(i):
        return words[i + 1]

    # ---- MOVE / MOVEA ----
    if (op & 0xC000) == 0x0000 and (op & 0x3000) != 0:
        size_field = (op >> 12) & 3
        if size_field in MOVE_SIZES:
            size = MOVE_SIZES[size_field]
            src_mode = (op >> 3) & 7
            src_reg  = op & 7
            dst_mode = (op >> 6) & 7
            dst_reg  = (op >> 9) & 7
            # Source first.
            src, ns = decode_ea(src_mode, src_reg, size, fetch)
            # Destination follows extension words.
            def fetch2(i):
                return words[i + 1 + ns]
            dst, nd = decode_ea(dst_mode, dst_reg, size, fetch2)
            mnem = "MOVEA" if dst_mode == 1 else "MOVE"
            return f"{mnem}.{size}  {src}, {dst}", 1 + ns + nd

    # ---- BSR / BRA / Bcc ----
    if (op & 0xF000) == 0x6000:
        cc = (op >> 8) & 0xF
        disp8 = op & 0xFF
        if disp8 == 0:
            disp = signed_word(fetch(0))
            tgt = pc + 2 + disp
            extra = 1
        elif disp8 == 0xFF:
            hi = fetch(0); lo = fetch(1)
            disp = signed_long((hi << 16) | lo)
            tgt = pc + 2 + disp
            extra = 2
        else:
            disp = signed_byte(disp8)
            tgt = pc + 2 + disp
            extra = 0
        if cc == 0:
            return f"BRA     ${tgt:06X}", 1 + extra
        if cc == 1:
            return f"BSR     ${tgt:06X}", 1 + extra
        return f"B{CC_NAMES[cc]:<3} ${tgt:06X}", 1 + extra

    # ---- DBcc Dn, label ----
    if (op & 0xF0F8) == 0x50C8:
        cc = (op >> 8) & 0xF
        reg = op & 7
        disp = signed_word(fetch(0))
        tgt = pc + 2 + disp
        cc_nm = "RA" if cc == 1 else CC_NAMES[cc]
        return f"DB{cc_nm:<3} D{reg}, ${tgt:06X}", 2

    # ---- Scc <ea> ----
    if (op & 0xF0C0) == 0x50C0:
        cc = (op >> 8) & 0xF
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "B", fetch)
        return f"S{CC_NAMES[cc]:<3} {ea}", 1 + n

    # ---- ADDQ / SUBQ ----
    if (op & 0xF000) == 0x5000:
        imm = (op >> 9) & 7
        if imm == 0: imm = 8
        is_subq = (op >> 8) & 1
        size_field = (op >> 6) & 3
        if size_field in SIZES:
            size = SIZES[size_field]
            mode = (op >> 3) & 7
            reg = op & 7
            ea, n = decode_ea(mode, reg, size, fetch)
            mnem = "SUBQ" if is_subq else "ADDQ"
            return f"{mnem}.{size}  #{imm}, {ea}", 1 + n

    # ---- MOVEQ ----
    if (op & 0xF100) == 0x7000:
        reg = (op >> 9) & 7
        v = signed_byte(op & 0xFF)
        return f"MOVEQ   #{v}, D{reg}", 1

    # ---- LEA ----
    if (op & 0xF1C0) == 0x41C0:
        dst_a = (op >> 9) & 7
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "L", fetch)
        return f"LEA     {ea}, A{dst_a}", 1 + n

    # ---- PEA ----
    if (op & 0xFFC0) == 0x4840:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "L", fetch)
        return f"PEA     {ea}", 1 + n

    # ---- JMP / JSR ----
    if (op & 0xFFC0) == 0x4EC0:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "L", fetch)
        return f"JMP     {ea}", 1 + n
    if (op & 0xFFC0) == 0x4E80:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "L", fetch)
        return f"JSR     {ea}", 1 + n

    # ---- RTS / RTE / RTR / RTD / NOP / TRAP / STOP ----
    if op == 0x4E75: return "RTS", 1
    if op == 0x4E73: return "RTE", 1
    if op == 0x4E77: return "RTR", 1
    if op == 0x4E71: return "NOP", 1
    if op == 0x4E70: return "RESET", 1
    if (op & 0xFFF0) == 0x4E40:
        return f"TRAP    #{op & 0xF}", 1
    if op == 0x4E72:
        return f"STOP    #${fetch(0):04X}", 2

    # ---- LINK / UNLK ----
    if (op & 0xFFF8) == 0x4E50:
        reg = op & 7
        disp = signed_word(fetch(0))
        return f"LINK    A{reg}, #${disp & 0xFFFF:04X}", 2
    if (op & 0xFFF8) == 0x4E58:
        reg = op & 7
        return f"UNLK    A{reg}", 1

    # ---- MOVE USP / MOVE-from-SR / MOVE-to-SR / MOVE-to-CCR ----
    if (op & 0xFFF0) == 0x4E60:
        # 4E60..4E67 = MOVE An, USP    4E68..4E6F = MOVE USP, An
        reg = op & 7
        if op & 8:
            return f"MOVE    USP, A{reg}", 1
        return f"MOVE    A{reg}, USP", 1
    if (op & 0xFFC0) == 0x40C0:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "W", fetch)
        return f"MOVE    SR, {ea}", 1 + n
    if (op & 0xFFC0) == 0x46C0:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "W", fetch)
        return f"MOVE    {ea}, SR", 1 + n
    if (op & 0xFFC0) == 0x44C0:
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, "W", fetch)
        return f"MOVE    {ea}, CCR", 1 + n

    # ---- SWAP / EXT (must come before MOVEM — they share the high
    # bits of the MOVEM mask but use EA mode 0 (Dn), which MOVEM does
    # not allow).
    if (op & 0xFFF8) == 0x4840:
        return f"SWAP    D{op & 7}", 1
    if (op & 0xFFB8) == 0x4880:
        size = "L" if op & 0x40 else "W"
        return f"EXT.{size}   D{op & 7}", 1

    # ---- MOVEM (only for EA modes that MOVEM actually supports —
    # mode 0 / 1 are EXT/SWAP, handled just above).
    if (op & 0xFB80) == 0x4880 and ((op >> 3) & 7) not in (0, 1):
        dir_ = (op >> 10) & 1     # 0 = reg→mem, 1 = mem→reg
        is_long = (op >> 6) & 1
        size = "L" if is_long else "W"
        mode = (op >> 3) & 7
        reg = op & 7
        mask = fetch(0)
        # decode reglist into a readable string
        if mode == 4:
            # predec: bit0 = A7, ..., bit15 = D0
            order = [f"D{i}" for i in range(8)][::-1] + \
                    [f"A{i}" for i in range(8)][::-1]
        else:
            # postinc / other: bit0 = D0, ..., bit15 = A7
            order = [f"D{i}" for i in range(8)] + [f"A{i}" for i in range(8)]
        regs = ",".join(order[i] for i in range(16) if mask & (1 << i))
        ea, n = decode_ea(mode, reg, size, lambda i: words[i + 2])
        if dir_:
            return f"MOVEM.{size} {ea}, {regs}", 2 + n
        return f"MOVEM.{size} {regs}, {ea}", 2 + n

    # ---- ORI / ANDI / SUBI / ADDI / EORI / CMPI ----
    # 0000_oop_ss_mmmrrr   where oop = which op (000=ORI, 001=ANDI,
    # 010=SUBI, 011=ADDI, 101=EORI, 110=CMPI; 100 is the static-bit-op
    # family BTST/BCHG/BCLR/BSET handled below).  The destination-
    # special form 0000_oop_0_011_1100 is "to CCR" (0000_oop_0_111_1100
    # = "to SR").
    # Mask 0xF100 keeps op_field (bits 11-9) free so CMPI / EORI
    # (op_field 5 and 6, both of which have bit 11 set) also match.
    if (op & 0xF100) == 0x0000:
        op_field = (op >> 9) & 7
        names = {0: "ORI", 1: "ANDI", 2: "SUBI", 3: "ADDI",
                 5: "EORI", 6: "CMPI"}
        if op_field in (0, 1, 2, 3, 5, 6):
            size_field = (op >> 6) & 3
            size = SIZES.get(size_field, "?")
            # immediate first
            if size == "L":
                imm = (fetch(0) << 16) | fetch(1)
                ns = 2
                imm_text = f"#${imm:08X}"
            elif size == "W":
                imm = fetch(0)
                ns = 1
                imm_text = f"#${imm:04X}"
            else:
                imm = fetch(0) & 0xFF
                ns = 1
                imm_text = f"#${imm:02X}"
            mode = (op >> 3) & 7
            reg = op & 7
            if mode == 7 and reg == 4 and size in ("B", "W"):
                dst_special = "CCR" if size == "B" else "SR"
                return f"{names[op_field]}.{size} {imm_text}, {dst_special}", 1 + ns
            ea, n = decode_ea(mode, reg, size,
                              lambda i: words[i + 1 + ns])
            return f"{names[op_field]}.{size} {imm_text}, {ea}", 1 + ns + n
        if op_field == 4:
            # Static bit-op (BTST/BCHG/BCLR/BSET #imm, <ea>).
            #   0000_100_oo_mmm_rrr  where oo selects the operation; the
            #   bit number is the low byte of the next word.  Size is
            #   .L if EA is Dn, else .B.
            bit_op = (op >> 6) & 3
            mnem = ["BTST", "BCHG", "BCLR", "BSET"][bit_op]
            mode = (op >> 3) & 7
            reg = op & 7
            bit_num = fetch(0) & 0xFF
            size = "L" if mode == 0 else "B"
            ea, n = decode_ea(mode, reg, size, lambda i: words[i + 2])
            return f"{mnem}.{size}  #{bit_num}, {ea}", 2 + n

    # ---- Dynamic bit-op: BTST/BCHG/BCLR/BSET Dn, <ea> -------------------
    # 0000_DDD_1_oo_mmm_rrr.  bit 8 = 1 distinguishes this from the
    # immediate-source ALU family above.
    if (op & 0xF100) == 0x0100:
        src = (op >> 9) & 7
        bit_op = (op >> 6) & 3
        mnem = ["BTST", "BCHG", "BCLR", "BSET"][bit_op]
        mode = (op >> 3) & 7
        reg = op & 7
        size = "L" if mode == 0 else "B"
        ea, n = decode_ea(mode, reg, size, fetch)
        return f"{mnem}.{size}  D{src}, {ea}", 1 + n

    # ---- ALU register form: ADD/SUB/AND/OR/EOR/CMP with Dn ----
    # 1xxx_rrr_ooo_mmm_RRR : opcode bits 14-12 select ADD/SUB/AND/OR/EOR/CMP.
    family_codes = {
        0b1101: "ADD", 0b1001: "SUB", 0b1100: "AND", 0b1000: "OR",
        0b1011: "CMP/EOR", 0b1011: "CMP", 0b1011: "CMP",  # CMP/CMPA, EOR shared
    }
    fam = (op >> 12) & 0xF
    if fam in (0xD, 0x9, 0xC, 0x8, 0xB):
        reg = (op >> 9) & 7
        opmode = (op >> 6) & 7
        mode = (op >> 3) & 7
        srcreg = op & 7
        # opmode 0/1/2: size B/W/L, EA -> Dn  (or Dn EA for ADDX/SUBX subset)
        # opmode 4/5/6: size B/W/L, Dn -> EA
        # opmode 3 (size W) / 7 (size L): An direct (ADDA/SUBA/CMPA)
        size_map = {0: "B", 1: "W", 2: "L", 4: "B", 5: "W", 6: "L"}
        if opmode in size_map:
            size = size_map[opmode]
            ea, n = decode_ea(mode, srcreg, size, fetch)
            if fam == 0xD: mnem = "ADD"
            elif fam == 0x9: mnem = "SUB"
            elif fam == 0xC: mnem = "AND"
            elif fam == 0x8: mnem = "OR"
            else: mnem = "CMP" if opmode in (0, 1, 2) else "EOR"
            if opmode in (0, 1, 2):
                return f"{mnem}.{size}  {ea}, D{reg}", 1 + n
            return f"{mnem}.{size}  D{reg}, {ea}", 1 + n
        if opmode in (3, 7):
            size = "W" if opmode == 3 else "L"
            ea, n = decode_ea(mode, srcreg, size, fetch)
            mnem = {0xD: "ADDA", 0x9: "SUBA", 0xB: "CMPA"}.get(fam)
            if mnem:
                return f"{mnem}.{size} {ea}, A{reg}", 1 + n

    # ---- ROL/ROR/LSL/LSR/ASL/ASR/ROXL/ROXR -------------------------------
    if (op & 0xF000) == 0xE000:
        if (op & 0x00C0) == 0x00C0:
            # memory shift .W, EA in mode/reg
            mnems = {0: "ASR", 1: "ASL", 2: "LSR", 3: "LSL",
                     4: "ROXR", 5: "ROXL", 6: "ROR", 7: "ROL"}
            idx = (op >> 9) & 7
            mode = (op >> 3) & 7
            reg = op & 7
            ea, n = decode_ea(mode, reg, "W", fetch)
            return f"{mnems[idx]}.W {ea}", 1 + n
        # register shift: by immediate or by Dn
        ir = (op >> 5) & 1
        size_field = (op >> 6) & 3
        size = SIZES.get(size_field, "?")
        idx = (op >> 3) & 7
        reg = (op >> 9) & 7
        dst = op & 7
        # mnems indexed by the 3-bit op (bits 4-3 of opcode + direction bit 8)
        op_bits = ((op >> 3) & 0x6) | ((op >> 8) & 1)
        # cleaner: just look up by (left/right + family bits 4-3)
        family = (op >> 3) & 3
        is_left = (op >> 8) & 1
        fam_mnem = {0: "AS", 1: "LS", 2: "ROX", 3: "RO"}[family]
        return f"{fam_mnem}{'L' if is_left else 'R'}.{size} " \
               f"{'D' + str(reg) if ir else '#' + str((reg or 8))}, D{dst}", 1

    # ---- TST ----
    if (op & 0xFF00) == 0x4A00:
        size_field = (op >> 6) & 3
        size = SIZES.get(size_field, "?")
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, size, fetch)
        return f"TST.{size}   {ea}", 1 + n

    # ---- CLR ----
    if (op & 0xFF00) == 0x4200:
        size_field = (op >> 6) & 3
        size = SIZES.get(size_field, "?")
        mode = (op >> 3) & 7
        reg = op & 7
        ea, n = decode_ea(mode, reg, size, fetch)
        return f"CLR.{size}   {ea}", 1 + n

    # ---- Fallback ----
    return f"DC.W    ${op:04X}", 1


# ---- driver --------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("binfile")
    ap.add_argument("addr", type=lambda s: int(s, 0), nargs="?",
                    default=ROM_BASE_DEFAULT)
    ap.add_argument("--bytes", type=lambda s: int(s, 0), default=64)
    ap.add_argument("--base", type=lambda s: int(s, 0), default=ROM_BASE_DEFAULT,
                    help="virtual address of binfile[0] (default $F80000)")
    args = ap.parse_args()

    with open(args.binfile, "rb") as f:
        blob = f.read()

    start = args.addr
    end   = start + args.bytes
    if start < args.base or end > args.base + len(blob):
        print(f"address range out of file: ${start:06X}..${end:06X} "
              f"(file covers ${args.base:06X}..${args.base + len(blob):06X})",
              file=sys.stderr)
        return 1

    pc = start
    while pc < end:
        # Fetch up to 5 words for the worst-case operand combo.
        offs = pc - args.base
        words = []
        for i in range(5):
            o = offs + i * 2
            if o + 1 >= len(blob): break
            words.append(struct.unpack(">H", blob[o:o + 2])[0])
        if not words:
            break
        text, nw = disasm_one(words, pc)
        raw = " ".join(f"{w:04X}" for w in words[:nw])
        print(f"  ${pc:06X}:  {raw:<24} {text}")
        pc += nw * 2

    return 0


if __name__ == "__main__":
    sys.exit(main())
