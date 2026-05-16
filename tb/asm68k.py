#!/usr/bin/env python3
"""Tiny assembler for the 68K-mini subset.

Reads a single .s file. Emits a .hex file of 32-bit words suitable for
$readmemh. Each line is one big-endian 32-bit word. Programs are placed at
byte address 0x400 by default; use `.org <hex>` to override and `.word <hex>`
to emit raw words at the current PC.

Supported instructions (case-insensitive):

  nop
  rts
  stop  #<imm16>
  jmp   <ea>          ; (Ar) or abs.L
  jsr   <ea>
  lea   <ea>, A<r>
  moveq #<imm8>, D<r>
  move.l <ea>, <ea>   ; long only
  add.l  <ea>, D<r>
  sub.l  <ea>, D<r>
  and.l  <ea>, D<r>
  or.l   <ea>, D<r>
  eor.l  D<r>, D<r>
  cmp.l  <ea>, D<r>
  addq.l #<i>, <ea>
  subq.l #<i>, <ea>
  tas    <ea>
  bra    <label or #disp>
  bsr    <label or #disp>
  b<cc>  <label or #disp>   ; e.g. beq, bne, blt, ble, bgt, bge, bcc, bcs

Addressing modes:
  D<r>            ; data register
  A<r>            ; address register
  (A<r>)          ; indirect
  (A<r>)+         ; postinc
  -(A<r>)         ; predec
  $<hex>          ; absolute long (always 2 ext words for clarity)
  $<hex>.w        ; absolute short
  #$<hex>         ; immediate (long)

Labels:
  identifier:                  ; defines label at current PC
"""
import sys, re, struct

class Asm:
    EA_DREG, EA_AREG, EA_AIND, EA_AINC, EA_ADEC, EA_EXT = 0, 1, 2, 3, 4, 7
    EA7_ABSW, EA7_ABSL, EA7_IMM = 0, 1, 4

    CC = {
        't': 0, 'f': 1, 'hi': 2, 'ls': 3, 'cc': 4, 'cs': 5,
        'ne': 6, 'eq': 7, 'vc': 8, 'vs': 9, 'pl': 10, 'mi': 11,
        'ge': 12, 'lt': 13, 'gt': 14, 'le': 15,
    }

    def __init__(self):
        self.words = []           # list of (address, value16)
        self.labels = {}
        self.fixups = []          # (pc_of_disp_word, label_name, kind)  kind=disp8/disp16/abs32
        self.pc = 0x400

    def parse_int(self, s):
        s = s.strip()
        if s.startswith('#'): s = s[1:]
        neg = False
        if s.startswith('-'):
            neg = True
            s = s[1:]
        if s.startswith('$'):
            v = int(s[1:], 16)
        elif s.startswith('0x'):
            v = int(s, 16)
        else:
            v = int(s, 0)
        return -v if neg else v

    def parse_ea(self, tok):
        tok = tok.strip()
        # -(Ar)
        m = re.fullmatch(r'-\(A([0-7])\)', tok, re.I)
        if m: return (self.EA_ADEC, int(m.group(1)), [])
        # (Ar)+
        m = re.fullmatch(r'\(A([0-7])\)\+', tok, re.I)
        if m: return (self.EA_AINC, int(m.group(1)), [])
        # (Ar)
        m = re.fullmatch(r'\(A([0-7])\)', tok, re.I)
        if m: return (self.EA_AIND, int(m.group(1)), [])
        # Dr
        m = re.fullmatch(r'D([0-7])', tok, re.I)
        if m: return (self.EA_DREG, int(m.group(1)), [])
        # Ar
        m = re.fullmatch(r'A([0-7])', tok, re.I)
        if m: return (self.EA_AREG, int(m.group(1)), [])
        # #imm.L
        if tok.startswith('#'):
            v = self.parse_int(tok[1:]) & 0xFFFFFFFF
            return (self.EA_EXT, self.EA7_IMM, [(v >> 16) & 0xFFFF, v & 0xFFFF])
        # abs.w
        m = re.fullmatch(r'\$([0-9a-fA-F]+)\.w', tok)
        if m:
            v = int(m.group(1), 16) & 0xFFFF
            return (self.EA_EXT, self.EA7_ABSW, [v])
        # abs.L
        m = re.fullmatch(r'\$([0-9a-fA-F]+)', tok)
        if m:
            v = int(m.group(1), 16) & 0xFFFFFFFF
            return (self.EA_EXT, self.EA7_ABSL, [(v >> 16) & 0xFFFF, v & 0xFFFF])
        # Label (resolved later as abs.L). Store as ('lbl', name) markers so
        # emit_ea_ext records the fixup PC at the actual emit site.
        if re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', tok):
            return (self.EA_EXT, self.EA7_ABSL, [('lbl_hi', tok), ('lbl_lo', tok)])
        raise SyntaxError(f"can't parse EA: {tok!r}")

    def emit(self, value):
        self.words.append((self.pc, value & 0xFFFF))
        self.pc += 2

    def emit_ea_ext(self, ea):
        for w in ea[2]:
            if isinstance(w, tuple) and w[0] in ('lbl_hi', 'lbl_lo'):
                self.fixups.append(('abs32_word', self.pc, w[0], w[1]))
                self.emit(0)
            else:
                self.emit(w)

    def parse_operands(self, line):
        # Split on commas not inside parens.
        parts = []
        depth = 0
        buf = ''
        for c in line:
            if c == '(': depth += 1
            elif c == ')': depth -= 1
            if c == ',' and depth == 0:
                parts.append(buf.strip())
                buf = ''
            else:
                buf += c
        if buf.strip(): parts.append(buf.strip())
        return parts

    def assemble_line(self, line):
        line = re.sub(r';.*', '', line).strip()
        if not line:
            return
        # Label
        m = re.match(r'([A-Za-z_][A-Za-z0-9_]*):\s*(.*)', line)
        if m:
            self.labels[m.group(1)] = self.pc
            line = m.group(2).strip()
            if not line: return
        # Directives
        if line.startswith('.org'):
            self.pc = self.parse_int(line.split()[1])
            return
        if line.startswith('.word'):
            for tok in [t.strip() for t in line[5:].split(',')]:
                self.emit(self.parse_int(tok))
            return
        if line.startswith('.long'):
            for tok in [t.strip() for t in line[5:].split(',')]:
                v = self.parse_int(tok) & 0xFFFFFFFF
                self.emit((v >> 16) & 0xFFFF)
                self.emit(v & 0xFFFF)
            return
        # Mnemonic
        mnem, _, rest = line.partition(' ')
        mnem = mnem.lower()
        operands = self.parse_operands(rest)

        if mnem == 'nop':
            self.emit(0x4E71)
        elif mnem == 'rts':
            self.emit(0x4E75)
        elif mnem == 'stop':
            imm = self.parse_int(operands[0]) & 0xFFFF
            self.emit(0x4E72)
            self.emit(imm)
        elif mnem == 'jmp' or mnem == 'jsr':
            ea = self.parse_ea(operands[0])
            opmode = 0b11 if mnem == 'jmp' else 0b10
            op = (0b0100_1110 << 8) | (opmode << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem == 'lea':
            ea = self.parse_ea(operands[0])
            an = self.reg(operands[1], 'A')
            op = (0b0100 << 12) | (an << 9) | (0b111 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem == 'moveq':
            imm = self.parse_int(operands[0])
            dn = self.reg(operands[1], 'D')
            op = (0b0111 << 12) | (dn << 9) | (imm & 0xFF)
            self.emit(op)
        elif mnem == 'move.l':
            src = self.parse_ea(operands[0])
            dst = self.parse_ea(operands[1])
            # Size field = 10 (long), but encoded in bits 13:12 as 10 -> "00_10"
            # opcode = 00_10 DDDmmm MMMrrr
            op = (0b00 << 14) | (0b10 << 12) | (dst[1] << 9) | (dst[0] << 6) | (src[0] << 3) | src[1]
            self.emit(op)
            self.emit_ea_ext(src)
            self.emit_ea_ext(dst)
        elif mnem in ('add.l', 'sub.l', 'and.l', 'or.l', 'cmp.l'):
            src = self.parse_ea(operands[0])
            dn = self.reg(operands[1], 'D')
            tab = {'add.l': 0b1101, 'sub.l': 0b1001, 'and.l': 0b1100, 'or.l': 0b1000, 'cmp.l': 0b1011}
            op = (tab[mnem] << 12) | (dn << 9) | (0b010 << 6) | (src[0] << 3) | src[1]
            self.emit(op)
            self.emit_ea_ext(src)
        elif mnem == 'eor.l':
            # EOR Dn,Dm only — opmode 110
            dn = self.reg(operands[0], 'D')
            dm = self.reg(operands[1], 'D')
            op = (0b1011 << 12) | (dn << 9) | (0b110 << 6) | (0b000 << 3) | dm
            self.emit(op)
        elif mnem == 'addq.l' or mnem == 'subq.l':
            imm = self.parse_int(operands[0])
            ea = self.parse_ea(operands[1])
            iii = 0 if imm == 8 else (imm & 7)
            d = 1 if mnem == 'subq.l' else 0
            op = (0b0101 << 12) | (iii << 9) | (d << 8) | (0b10 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem == 'tas':
            ea = self.parse_ea(operands[0])
            op = (0b0100_1010 << 8) | (0b11 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem == 'bra' or mnem == 'bsr' or (mnem.startswith('b') and mnem[1:] in self.CC):
            if mnem == 'bra': cc = self.CC['t']
            elif mnem == 'bsr': cc = self.CC['f']
            else: cc = self.CC[mnem[1:]]
            target = operands[0]
            # Always emit short form with placeholder; the linker pass picks disp8 or disp16.
            self.fixups.append(('branch', self.pc, target, cc))
            self.emit(0)  # placeholder
        else:
            raise SyntaxError(f"unknown mnemonic: {mnem!r}")

    def reg(self, tok, kind):
        m = re.fullmatch(rf'{kind}([0-7])', tok.strip(), re.I)
        if not m: raise SyntaxError(f"expected {kind} register, got {tok!r}")
        return int(m.group(1))

    def assemble(self, text):
        # Two-pass: first emit placeholders for labels, then patch.
        for line in text.splitlines():
            self.assemble_line(line)
        # Patch branches and EA labels.
        words_by_pc = {pc: i for i, (pc, _) in enumerate(self.words)}
        for fix in self.fixups:
            if fix[0] == 'branch':
                _, pc, target, cc = fix
                tgt = self.labels[target] if target in self.labels else self.parse_int(target)
                disp = tgt - (pc + 2)
                # Try disp8 first.
                if -128 <= disp <= 127 and disp != 0:
                    word = (0b0110 << 12) | (cc << 8) | (disp & 0xFF)
                    self.words[words_by_pc[pc]] = (pc, word & 0xFFFF)
                else:
                    # Use disp16: insert an extension word AFTER the branch.
                    # Need to shift subsequent words by 2 bytes — but we can't easily.
                    # Simpler: pre-allocate disp16 by inserting a dummy ext word. Re-run.
                    raise SyntaxError(f"branch out of disp8 range to {target}; not supported")
            elif fix[0] == 'abs32_word':
                _, pc, which, label = fix
                addr = self.labels[label] if label in self.labels else self.parse_int(label)
                addr &= 0xFFFFFFFF
                half = (addr >> 16) if which == 'lbl_hi' else (addr & 0xFFFF)
                self.words[words_by_pc[pc]] = (pc, half & 0xFFFF)

    def to_hex(self, mem_words):
        """Pack 16-bit half-words into 32-bit big-endian memory words."""
        mem = [0] * mem_words
        for pc, w in self.words:
            widx = pc >> 2
            if (pc & 2) == 0:
                mem[widx] = (mem[widx] & 0x0000FFFF) | ((w & 0xFFFF) << 16)
            else:
                mem[widx] = (mem[widx] & 0xFFFF0000) | (w & 0xFFFF)
        return '\n'.join(f'{x:08x}' for x in mem) + '\n'


def main():
    if len(sys.argv) != 3:
        print("usage: asm68k.py <in.s> <out.hex>", file=sys.stderr)
        sys.exit(1)
    with open(sys.argv[1]) as f:
        src = f.read()
    a = Asm()
    a.assemble(src)
    with open(sys.argv[2], 'w') as f:
        f.write(a.to_hex(16384))

if __name__ == '__main__':
    main()
