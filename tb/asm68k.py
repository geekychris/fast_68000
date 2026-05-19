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
    EA_DREG, EA_AREG, EA_AIND, EA_AINC, EA_ADEC, EA_DISP, EA_INDEXED, EA_EXT = 0, 1, 2, 3, 4, 5, 6, 7
    EA7_ABSW, EA7_ABSL, EA7_IMM, EA7_PCREL, EA7_PCIDX = 0, 1, 4, 2, 3

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
        # Allow underscores as digit separators (e.g. $00FE_0400, 1_000).
        s = s.replace('_', '')
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
        # disp(Ar) — 16-bit displacement addressing
        m = re.fullmatch(r'(-?(?:\$[0-9a-fA-F]+|0x[0-9a-fA-F]+|\d+))\(A([0-7])\)', tok, re.I)
        if m:
            disp = self.parse_int(m.group(1)) & 0xFFFF
            an = int(m.group(2))
            return (self.EA_DISP, an, [disp])
        # d8(An, Xn.size)  -- indexed addressing, mode 6
        m = re.fullmatch(
            r'(-?(?:\$[0-9a-fA-F]+|0x[0-9a-fA-F]+|\d+))?\s*\(\s*A([0-7])\s*,\s*([DA])([0-7])\.([wlWL])\s*\)',
            tok)
        if m:
            disp = (self.parse_int(m.group(1)) & 0xFF) if m.group(1) else 0
            an = int(m.group(2))
            xkind = m.group(3).upper()
            xreg = int(m.group(4))
            xL = 1 if m.group(5).lower() == 'l' else 0
            ext = ((1 if xkind == 'A' else 0) << 15) | (xreg << 12) | (xL << 11) | disp
            return (self.EA_INDEXED, an, [ext])
        # d16(PC) — PC-relative with 16-bit displacement.  Resolved in pass 2.
        m = re.fullmatch(r'([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*PC\s*\)', tok, re.I)
        if m:
            return (self.EA_EXT, self.EA7_PCREL, [('pcrel16', m.group(1))])
        # d8(PC, Xn.size) — PC-relative indexed.  Resolved in pass 2.
        m = re.fullmatch(
            r'([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*PC\s*,\s*([DA])([0-7])\.([wlWL])\s*\)',
            tok, re.I)
        if m:
            xkind = m.group(2).upper()
            xreg = int(m.group(3))
            xL = 1 if m.group(4).lower() == 'l' else 0
            ext_hdr = ((1 if xkind == 'A' else 0) << 15) | (xreg << 12) | (xL << 11)
            return (self.EA_EXT, self.EA7_PCIDX, [('pcrel8', m.group(1), ext_hdr)])
        # (Ar)
        m = re.fullmatch(r'\(A([0-7])\)', tok, re.I)
        if m: return (self.EA_AIND, int(m.group(1)), [])
        # Dr
        m = re.fullmatch(r'D([0-7])', tok, re.I)
        if m: return (self.EA_DREG, int(m.group(1)), [])
        # Ar
        m = re.fullmatch(r'A([0-7])', tok, re.I)
        if m: return (self.EA_AREG, int(m.group(1)), [])
        # #imm.L (literal or label reference, resolved in pass 2)
        if tok.startswith('#'):
            inner = tok[1:].strip()
            if re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', inner):
                # Label as immediate: emit two zero halfwords, patched later.
                return (self.EA_EXT, self.EA7_IMM, [('lbl_hi', inner), ('lbl_lo', inner)])
            v = self.parse_int(inner) & 0xFFFFFFFF
            return (self.EA_EXT, self.EA7_IMM, [(v >> 16) & 0xFFFF, v & 0xFFFF])
        # abs.w
        m = re.fullmatch(r'\$([0-9a-fA-F_]+)\.w', tok, re.I)
        if m:
            v = int(m.group(1).replace('_',''), 16) & 0xFFFF
            return (self.EA_EXT, self.EA7_ABSW, [v])
        # abs.L
        m = re.fullmatch(r'\$([0-9a-fA-F_]+)', tok)
        if m:
            v = int(m.group(1).replace('_',''), 16) & 0xFFFFFFFF
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
            elif isinstance(w, tuple) and w[0] == 'pcrel16':
                # PC-relative 16-bit displacement.  Real 68k PC for this
                # access = the address of this extension word.
                self.fixups.append(('pcrel16', self.pc, w[1]))
                self.emit(0)
            elif isinstance(w, tuple) and w[0] == 'pcrel8':
                # PC-relative 8-bit displacement embedded in the indexed
                # extension word; high 8 bits already encoded in w[2].
                self.fixups.append(('pcrel8', self.pc, w[1], w[2]))
                self.emit(0)
            else:
                self.emit(w)

    def emit_ea_ext_sized(self, ea, size_char):
        # Like emit_ea_ext but for sized MOVE: an immediate source emits 1 word
        # for .b/.w (the low halfword of the value) and 2 words for .l.
        mode, reg, words = ea
        is_imm = (mode == self.EA_EXT and reg == self.EA7_IMM)
        if is_imm and size_char in ('b', 'w'):
            if len(words) == 2 and isinstance(words[0], tuple):
                raise SyntaxError("label as .b/.w immediate not supported")
            lo = words[1] if len(words) == 2 else words[0]
            self.emit(lo)
            return
        self.emit_ea_ext(ea)

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
        label_name = None
        if m:
            label_name = m.group(1)
            line = m.group(2).strip()
            self.labels[label_name] = self.pc
            if not line: return
        # Directives
        if line.startswith('.org'):
            self.pc = self.parse_int(line.split()[1])
            return
        if line.startswith('.word'):
            for tok in [t.strip() for t in line[5:].split(',')]:
                self.emit(self.parse_int(tok))
            return
        if line.startswith('.align'):
            n = self.parse_int(line.split()[1])
            while self.pc % n:
                self.emit(0)
            return
        if line.startswith('.long'):
            # Auto-align to 4-byte boundary so .long values are long-aligned.
            # Bump any labels that were set at the pre-aligned PC up to the
            # aligned PC (so `label:` on the line above resolves to the
            # actual data position, not the padding bytes).
            old_pc = self.pc
            while self.pc % 4:
                self.emit(0)
            if self.pc != old_pc:
                for lbl, lpc in self.labels.items():
                    if lpc == old_pc:
                        self.labels[lbl] = self.pc
            for tok in [t.strip() for t in line[5:].split(',')]:
                try:
                    v = self.parse_int(tok) & 0xFFFFFFFF
                    self.emit((v >> 16) & 0xFFFF)
                    self.emit(v & 0xFFFF)
                except ValueError:
                    # Treat as a label reference, patched in pass 2.
                    self.fixups.append(('abs32_word', self.pc,     'lbl_hi', tok))
                    self.fixups.append(('abs32_word', self.pc + 2, 'lbl_lo', tok))
                    self.emit(0)
                    self.emit(0)
            return
        # Mnemonic
        mnem, _, rest = line.partition(' ')
        mnem = mnem.lower()
        operands = self.parse_operands(rest)

        if mnem == 'nop':
            self.emit(0x4E71)
        elif mnem == 'rts':
            self.emit(0x4E75)
        elif mnem == 'rte':
            self.emit(0x4E73)
        elif mnem == 'rtr':
            self.emit(0x4E77)
        elif mnem == 'trapv':
            self.emit(0x4E76)
        elif mnem == 'trap':
            n = self.parse_int(operands[0]) & 0xF
            self.emit(0x4E40 | n)
        elif mnem in ('movem.l', 'movem.w'):
            sz = 1 if mnem == 'movem.l' else 0
            op0 = operands[0].strip()
            op1 = operands[1].strip()
            # Distinguish reg-list vs EA by looking for ()/$/# in the EA side.
            if '(' in op0 or op0.startswith('$') or op0.startswith('#'):
                direction = 1   # mem -> regs
                ea = self.parse_ea(op0)
                regs = self._parse_reg_list(op1)
            else:
                direction = 0   # regs -> mem
                regs = self._parse_reg_list(op0)
                ea = self.parse_ea(op1)
            predec = (ea[0] == self.EA_ADEC)
            mask = 0
            for kind, idx in regs:
                reg_idx_full = idx + (8 if kind == 'A' else 0)
                bit_pos = (15 - reg_idx_full) if predec else reg_idx_full
                mask |= (1 << bit_pos)
            op = (0b0100 << 12) | (1 << 11) | (direction << 10) | \
                 (1 << 7) | (sz << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit(mask & 0xFFFF)
            self.emit_ea_ext(ea)
        elif mnem == 'chk' or mnem == 'chk.w':
            # 0100_ddd_110_mm_rrr
            ea = self.parse_ea(operands[0])
            dn = self.reg(operands[1], 'D')
            op = (0b0100 << 12) | (dn << 9) | (0b110 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem in ('move.l', 'move.w') and len(operands) == 2 and \
              (operands[0].strip().lower() == 'usp' or operands[1].strip().lower() == 'usp'):
            # MOVE An,USP  (0100_1110_0110_0aaa = 0x4E60 | An)
            # MOVE USP,An  (0100_1110_0110_1aaa = 0x4E68 | An)
            if operands[1].strip().lower() == 'usp':
                an = self.reg(operands[0], 'A')
                self.emit(0x4E60 | an)
            else:
                an = self.reg(operands[1], 'A')
                self.emit(0x4E68 | an)
        elif mnem == 'andi' or mnem == 'ori' or mnem == 'eori':
            # ANDI/ORI/EORI to CCR or SR -- no size suffix.
            # 0000_xxxx_0011_1100 = to CCR (.B), 0000_xxxx_0111_1100 = to SR (.W)
            fam = {'ori':0b0000,'andi':0b0010,'eori':0b1010}[mnem]
            imm = self.parse_int(operands[0])
            dst = operands[1].strip().lower()
            if dst == 'ccr':
                self.emit((0b0000 << 12) | (fam << 8) | 0b0011_1100)
                self.emit(imm & 0xFF)
            elif dst == 'sr':
                self.emit((0b0000 << 12) | (fam << 8) | 0b0111_1100)
                self.emit(imm & 0xFFFF)
            else:
                raise SyntaxError(f"{mnem} expects CCR or SR as dest, got {dst!r}")
        elif mnem in ('move.w',) and len(operands) == 2 and \
              (operands[0].strip().lower() in ('sr','ccr') or operands[1].strip().lower() in ('sr','ccr')):
            # MOVE from SR: 0100_0000_11_mm_rrr (W)
            # MOVE to CCR:  0100_0100_11_mm_rrr (W src; low byte used)
            # MOVE to SR:   0100_0110_11_mm_rrr (W, priv)
            src = operands[0].strip()
            dst = operands[1].strip().lower()
            if src.lower() == 'sr':
                ea = self.parse_ea(operands[1])
                self.emit((0b0100_0000 << 8) | (0b11 << 6) | (ea[0] << 3) | ea[1])
                self.emit_ea_ext_sized(ea, 'w')
            elif dst == 'ccr':
                ea = self.parse_ea(operands[0])
                self.emit((0b0100_0100 << 8) | (0b11 << 6) | (ea[0] << 3) | ea[1])
                self.emit_ea_ext_sized(ea, 'w')
            elif dst == 'sr':
                ea = self.parse_ea(operands[0])
                self.emit((0b0100_0110 << 8) | (0b11 << 6) | (ea[0] << 3) | ea[1])
                self.emit_ea_ext_sized(ea, 'w')
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
        elif mnem in ('move.l', 'move.w', 'move.b'):
            src = self.parse_ea(operands[0])
            dst = self.parse_ea(operands[1])
            # MOVE size field in bits 13:12: 01 = byte, 11 = word, 10 = long.
            size_bits = {'move.b': 0b01, 'move.w': 0b11, 'move.l': 0b10}[mnem]
            op = (0b00 << 14) | (size_bits << 12) | (dst[1] << 9) | (dst[0] << 6) | (src[0] << 3) | src[1]
            self.emit(op)
            # Size-aware emission of source extension words.  parse_ea always
            # generates two halfwords for an immediate (.L view); for .B/.W we
            # need just one (the low half).
            self.emit_ea_ext_sized(src, mnem[-1])
            self.emit_ea_ext_sized(dst, mnem[-1])
        elif mnem in ('add.l', 'sub.l', 'and.l', 'or.l', 'cmp.l'):
            # If the destination is an An register, real assemblers auto-promote
            # ADD/SUB/CMP to ADDA/SUBA/CMPA.  AND/OR have no An-dest form.
            promoted = None
            if mnem in ('add.l','sub.l','cmp.l') and \
                  re.fullmatch(r'A[0-7]', operands[1].strip(), re.I):
                promoted = {'add.l':'adda.l','sub.l':'suba.l','cmp.l':'cmpa.l'}[mnem]
            if promoted:
                src = self.parse_ea(operands[0])
                an  = self.reg(operands[1], 'A')
                top4 = {'adda.l': 0b1101, 'suba.l': 0b1001, 'cmpa.l': 0b1011}[promoted]
                op = (top4 << 12) | (an << 9) | (0b111 << 6) | (src[0] << 3) | src[1]
                self.emit(op)
                self.emit_ea_ext(src)
            else:
                src = self.parse_ea(operands[0])
                dn = self.reg(operands[1], 'D')
                tab = {'add.l': 0b1101, 'sub.l': 0b1001, 'and.l': 0b1100, 'or.l': 0b1000, 'cmp.l': 0b1011}
                op = (tab[mnem] << 12) | (dn << 9) | (0b010 << 6) | (src[0] << 3) | src[1]
                self.emit(op)
                self.emit_ea_ext(src)
        elif mnem in ('adda.l', 'suba.l', 'cmpa.l', 'adda.w', 'suba.w', 'cmpa.w'):
            # ADDA/SUBA/CMPA Dn-like-source -> An.  Encoding:
            #   ADDA.W = 1101 ddd 011 mmm rrr ; ADDA.L = 1101 ddd 111 mmm rrr
            #   SUBA.W = 1001 ddd 011 mmm rrr ; SUBA.L = 1001 ddd 111 mmm rrr
            #   CMPA.W = 1011 ddd 011 mmm rrr ; CMPA.L = 1011 ddd 111 mmm rrr
            src = self.parse_ea(operands[0])
            an  = self.reg(operands[1], 'A')
            family, _, sz = mnem.partition('.')
            top4 = {'adda': 0b1101, 'suba': 0b1001, 'cmpa': 0b1011}[family]
            opm  = 0b111 if sz == 'l' else 0b011
            op = (top4 << 12) | (an << 9) | (opm << 6) | (src[0] << 3) | src[1]
            self.emit(op)
            self.emit_ea_ext_sized(src, sz)
        elif mnem == 'eor.l':
            # EOR Dn,Dm only — opmode 110
            dn = self.reg(operands[0], 'D')
            dm = self.reg(operands[1], 'D')
            op = (0b1011 << 12) | (dn << 9) | (0b110 << 6) | (0b000 << 3) | dm
            self.emit(op)
        elif mnem in ('addx.b', 'addx.w', 'addx.l',
                      'subx.b', 'subx.w', 'subx.l'):
            # ADDX/SUBX Dy, Dx: register form only (1101/1001_xxx_1ss_000_yyy).
            family, _, sz = mnem.partition('.')
            top4 = 0b1101 if family == 'addx' else 0b1001
            ss   = {'b':0b00, 'w':0b01, 'l':0b10}[sz]
            dy = self.reg(operands[0], 'D')
            dx = self.reg(operands[1], 'D')
            op = (top4 << 12) | (dx << 9) | (1 << 8) | (ss << 6) | (0 << 3) | dy
            self.emit(op)
        elif mnem in ('negx.b', 'negx.w', 'negx.l'):
            # NEGX <ea>: 0100_0000_ss_mm_rrr.  Currently we accept any EA the
            # generic parser produces, but only Dn is implemented in the core.
            _, _, sz = mnem.partition('.')
            ss = {'b':0b00, 'w':0b01, 'l':0b10}[sz]
            ea = self.parse_ea(operands[0])
            op = (0b0100_0000 << 8) | (ss << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext_sized(ea, sz)
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
        elif mnem in ('clr.b', 'clr.w', 'clr.l',
                       'neg.b', 'neg.w', 'neg.l',
                       'not.b', 'not.w', 'not.l',
                       'tst.b', 'tst.w', 'tst.l'):
            op_family, _, sz_str = mnem.partition('.')
            ss = {'b': 0b00, 'w': 0b01, 'l': 0b10}[sz_str]
            mid = {'clr': 0b0010, 'neg': 0b0100, 'not': 0b0110, 'tst': 0b1010}[op_family]
            ea = self.parse_ea(operands[0])
            op = (0b0100 << 12) | (mid << 8) | (ss << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem in ('ext.w', 'ext.l'):
            dn = self.reg(operands[0], 'D')
            ss = 0b10 if mnem == 'ext.w' else 0b11   # 10 = ext byte→word, 11 = ext word→long
            op = (0b0100_1000 << 8) | (ss << 6) | dn
            self.emit(op)
        elif mnem == 'swap':
            dn = self.reg(operands[0], 'D')
            op = (0b0100_1000_0100_0 << 3) | dn
            self.emit(op)
        elif mnem == 'exg':
            a = operands[0].strip().upper()
            b = operands[1].strip().upper()
            ma = re.fullmatch(r'([DA])([0-7])', a)
            mb = re.fullmatch(r'([DA])([0-7])', b)
            if not ma or not mb:
                raise SyntaxError(f"exg expects two registers, got {operands!r}")
            ka, ra = ma.group(1), int(ma.group(2))
            kb, rb = mb.group(1), int(mb.group(2))
            if ka == 'D' and kb == 'D':
                # EXG Dx,Dy: 1100 xxx 1 01000 yyy → 0xC140 base
                op = 0xC140 | (ra << 9) | rb
            elif ka == 'A' and kb == 'A':
                op = 0xC148 | (ra << 9) | rb
            elif ka == 'D' and kb == 'A':
                op = 0xC188 | (ra << 9) | rb
            else:   # exg A, D — canonical form swaps to D, A
                op = 0xC188 | (rb << 9) | ra
            self.emit(op)
        elif mnem in ('addi.b', 'addi.w', 'addi.l',
                       'subi.b', 'subi.w', 'subi.l',
                       'andi.b', 'andi.w', 'andi.l',
                       'ori.b',  'ori.w',  'ori.l',
                       'eori.b', 'eori.w', 'eori.l',
                       'cmpi.b', 'cmpi.w', 'cmpi.l'):
            family, _, sz_str = mnem.partition('.')
            # ANDI/ORI/EORI to CCR is always .B; to SR is always .W.  Real
            # assemblers accept any size suffix when the dst is CCR/SR.
            if family in ('andi', 'ori', 'eori') and len(operands) == 2:
                dst = operands[1].strip().lower()
                if dst in ('ccr', 'sr'):
                    fam = {'ori':0b0000,'andi':0b0010,'eori':0b1010}[family]
                    imm = self.parse_int(operands[0])
                    if dst == 'ccr':
                        self.emit((0b0000 << 12) | (fam << 8) | 0b0011_1100)
                        self.emit(imm & 0xFF)
                    else:
                        self.emit((0b0000 << 12) | (fam << 8) | 0b0111_1100)
                        self.emit(imm & 0xFFFF)
                    return
            ss = {'b': 0b00, 'w': 0b01, 'l': 0b10}[sz_str]
            fam = {'ori':0b0000,'andi':0b0010,'subi':0b0100,'addi':0b0110,'eori':0b1010,'cmpi':0b1100}[family]
            imm = self.parse_int(operands[0])
            ea = self.parse_ea(operands[1])
            op = (0b0000 << 12) | (fam << 8) | (ss << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            if sz_str == 'l':
                self.emit((imm >> 16) & 0xFFFF)
                self.emit(imm & 0xFFFF)
            else:
                self.emit(imm & 0xFFFF)
            self.emit_ea_ext(ea)
        elif mnem.startswith(('asl.', 'asr.', 'lsl.', 'lsr.',
                              'rol.', 'ror.', 'roxl.', 'roxr.')):
            family, _, sz_str = mnem.partition('.')
            ss = {'b': 0b00, 'w': 0b01, 'l': 0b10}[sz_str]
            d = 1 if family.endswith('l') else 0  # left if endswith 'l'
            # type: 00=AS, 01=LS, 10=ROX, 11=RO
            ttab = {'as': 0b00, 'ls': 0b01, 'rox': 0b10, 'ro': 0b11}
            base = family[:-1] if family.endswith('l') else family[:-1]   # strip trailing l/r
            tt = ttab[base]
            # First operand: count (immediate or Dn). Second: Dn being shifted.
            cnt = operands[0].strip()
            dn  = self.reg(operands[1], 'D')
            if cnt.startswith('#'):
                imm = self.parse_int(cnt)
                ccc = 0 if imm == 8 else (imm & 7)
                i = 0
            else:
                ccc = self.reg(cnt, 'D')
                i = 1
            op = (0b1110 << 12) | (ccc << 9) | (d << 8) | (ss << 6) | (i << 5) | (tt << 3) | dn
            self.emit(op)
        elif mnem.startswith('db') and (mnem[2:] in self.CC or mnem == 'dbra'):
            cc = self.CC['f'] if mnem == 'dbra' else self.CC[mnem[2:]]
            dn = self.reg(operands[0], 'D')
            target = operands[1]
            # 0101_cccc_1100_1rrr + disp16
            op = (0b0101 << 12) | (cc << 8) | (0b11001 << 3) | dn
            self.emit(op)
            self.fixups.append(('dbcc_disp', self.pc, target))
            self.emit(0)
        elif mnem == 'link':
            # link Aa, #disp16
            an = self.reg(operands[0], 'A')
            imm = self.parse_int(operands[1])
            op = (0b0100_1110_0101_0_000) | an
            self.emit(op)
            self.emit(imm & 0xFFFF)
        elif mnem == 'unlk':
            an = self.reg(operands[0], 'A')
            op = (0b0100_1110_0101_1_000) | an
            self.emit(op)
        elif mnem == 'pea':
            ea = self.parse_ea(operands[0])
            op = (0b0100_1000_01 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem in ('mulu.w', 'muls.w', 'divu.w', 'divs.w'):
            # 1100_ddd_011_mmrrr (MULU), 1100_ddd_111_mmrrr (MULS),
            # 1000_ddd_011_mmrrr (DIVU), 1000_ddd_111_mmrrr (DIVS)
            ea = self.parse_ea(operands[0])
            dn = self.reg(operands[1], 'D')
            base = 0b1100 if mnem.startswith('mul') else 0b1000
            # 'mulu'/'divu' -> unsigned (sub=011); 'muls'/'divs' -> signed (sub=111).
            # Use mnem[3] (the 'u' or 's' character) to distinguish.
            sub  = 0b011 if mnem[3] == 'u' else 0b111
            op = (base << 12) | (dn << 9) | (sub << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem.startswith('s') and len(mnem) >= 2 and mnem[1:] in self.CC:
            # Scc <ea>: 0101_cccc_11_mm_rrr
            cc = self.CC[mnem[1:]]
            ea = self.parse_ea(operands[0])
            op = (0b0101 << 12) | (cc << 8) | (0b11 << 6) | (ea[0] << 3) | ea[1]
            self.emit(op)
            self.emit_ea_ext(ea)
        elif mnem in ('btst', 'bchg', 'bclr', 'bset'):
            # Two forms: static (#imm, ea) or dynamic (Dn, ea).
            xx = {'btst':0b00,'bchg':0b01,'bclr':0b10,'bset':0b11}[mnem]
            src = operands[0].strip()
            ea = self.parse_ea(operands[1])
            if src.startswith('#'):
                # Static: 0000_1000_xx_mm_rrr + #imm extension word.
                bitn = self.parse_int(src) & 0xFF
                op = (0b0000_1000 << 8) | (xx << 6) | (ea[0] << 3) | ea[1]
                self.emit(op)
                self.emit(bitn & 0xFFFF)
                self.emit_ea_ext(ea)
            else:
                # Dynamic: 0000_ddd_1xx_mm_rrr
                dn = self.reg(src, 'D')
                op = (0b0000 << 12) | (dn << 9) | (1 << 8) | (xx << 6) | (ea[0] << 3) | ea[1]
                self.emit(op)
                self.emit_ea_ext(ea)
        elif mnem == 'bra' or mnem == 'bsr' or (mnem.startswith('b') and mnem[1:] in self.CC):
            if mnem == 'bra': cc = self.CC['t']
            elif mnem == 'bsr': cc = self.CC['f']
            else: cc = self.CC[mnem[1:]]
            target = operands[0]
            # If target is a backward reference that fits in disp8, emit 1 word.
            # Otherwise, emit 2 words (opcode|0x00 + disp16 extension).
            tgt = self.labels.get(target)
            if tgt is None:
                tgt = None  # forward reference - try short literal only
                try:
                    tgt = self.parse_int(target)
                except Exception:
                    tgt = None
            if tgt is not None:
                disp = tgt - (self.pc + 2)
                if -128 <= disp <= 127 and disp != 0:
                    word = (0b0110 << 12) | (cc << 8) | (disp & 0xFF)
                    self.emit(word)
                else:
                    word = (0b0110 << 12) | (cc << 8) | 0x00
                    self.emit(word)
                    self.emit(disp & 0xFFFF)
            else:
                # Forward reference to a label: always emit disp16 form.
                self.fixups.append(('branch16', self.pc, target, cc))
                self.emit((0b0110 << 12) | (cc << 8) | 0x00)
                self.emit(0)  # placeholder for disp16
        else:
            raise SyntaxError(f"unknown mnemonic: {mnem!r}")

    def _parse_reg_list(self, tok):
        """Parse a register list like 'D0/D1/D7/A0-A2' into a set of (kind, idx)."""
        regs = set()
        for part in tok.split('/'):
            p = part.strip().upper()
            m = re.fullmatch(r'([DA])(\d)-([DA])(\d)', p)
            if m:
                k1, i1, k2, i2 = m.group(1), int(m.group(2)), m.group(3), int(m.group(4))
                if k1 != k2:
                    raise SyntaxError(f"bad reg range {p!r}")
                lo, hi = min(i1, i2), max(i1, i2)
                for i in range(lo, hi + 1):
                    regs.add((k1, i))
                continue
            m = re.fullmatch(r'([DA])(\d)', p)
            if not m:
                raise SyntaxError(f"bad reg list element {p!r}")
            regs.add((m.group(1), int(m.group(2))))
        return regs

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
            if fix[0] == 'branch16':
                # Forward-ref branch: we emitted opcode|0x00 at pc, disp16 at pc+2.
                _, pc, target, cc = fix
                tgt = self.labels[target] if target in self.labels else self.parse_int(target)
                disp = tgt - (pc + 2)
                if disp < -32768 or disp > 32767:
                    raise SyntaxError(f"branch disp16 out of range to {target}")
                self.words[words_by_pc[pc + 2]] = (pc + 2, disp & 0xFFFF)
            elif fix[0] == 'abs32_word':
                _, pc, which, label = fix
                addr = self.labels[label] if label in self.labels else self.parse_int(label)
                addr &= 0xFFFFFFFF
                half = (addr >> 16) if which == 'lbl_hi' else (addr & 0xFFFF)
                self.words[words_by_pc[pc]] = (pc, half & 0xFFFF)
            elif fix[0] == 'dbcc_disp':
                # DBcc target = pc_of_opcode + 2 + sign_ext(disp16).
                # We saved pc = address of the displacement word = pc_of_opcode + 2.
                _, pc, target = fix
                tgt = self.labels[target] if target in self.labels else self.parse_int(target)
                disp = tgt - pc
                if disp < -32768 or disp > 32767:
                    raise SyntaxError(f"dbcc disp16 out of range to {target}")
                self.words[words_by_pc[pc]] = (pc, disp & 0xFFFF)
            elif fix[0] == 'pcrel16':
                # PC-relative 16-bit displacement; the 68k PC during the
                # access points at the extension-word address (pc).
                _, pc, target = fix
                tgt = self.labels[target] if target in self.labels else self.parse_int(target)
                disp = tgt - pc
                if disp < -32768 or disp > 32767:
                    raise SyntaxError(f"pcrel16 out of range to {target}")
                self.words[words_by_pc[pc]] = (pc, disp & 0xFFFF)
            elif fix[0] == 'pcrel8':
                _, pc, target, ext_hdr = fix
                tgt = self.labels[target] if target in self.labels else self.parse_int(target)
                disp = tgt - pc
                if disp < -128 or disp > 127:
                    raise SyntaxError(f"pcrel8 out of range to {target}")
                self.words[words_by_pc[pc]] = (pc, ext_hdr | (disp & 0xFF))

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
    args = sys.argv[1:]
    mem_words = 16384
    while args and args[0].startswith('--'):
        flag = args.pop(0)
        if flag == '--mem-words':
            mem_words = int(args.pop(0), 0)
        else:
            print(f"unknown flag: {flag}", file=sys.stderr)
            sys.exit(2)
    if len(args) != 2:
        print("usage: asm68k.py [--mem-words N] <in.s> <out.hex>", file=sys.stderr)
        sys.exit(1)
    with open(args[0]) as f:
        src = f.read()
    a = Asm()
    a.assemble(src)
    with open(args[1], 'w') as f:
        f.write(a.to_hex(mem_words))

if __name__ == '__main__':
    main()
