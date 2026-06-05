"""Unit tests for tools/blt_trace.py reference-blitter model.

Verifies the Python LF-combine + mask logic matches the canonical
real-Amiga semantics that our Verilog blitter implements
(per WB13_DEBUG_JOURNAL §54 + §55c).
"""

import os
import sys
import unittest

# Make tools/ importable
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'tools'))

import blt_trace  # noqa: E402


class TestLFCombine(unittest.TestCase):
    """Verify the 8-bit LF minterm combiner."""

    def test_lf_zero_clears(self):
        # LF=$00 → D=0 regardless of inputs (full clear)
        self.assertEqual(blt_trace.lf_combine(0x00, 0xFFFF, 0xFFFF, 0xFFFF), 0)

    def test_lf_ff_sets(self):
        # LF=$FF → D=1 regardless (full set)
        self.assertEqual(blt_trace.lf_combine(0xFF, 0x0000, 0x0000, 0x0000), 0xFFFF)

    def test_lf_ca_intuition_mask(self):
        # LF=$CA = $11001010 → D = (A & B) | (~A & B & C) | (~A & ~B & C)
        # equivalently:  if A then B else C
        # With A=$FF00 (top byte set), B=$F0F0, C=$AAAA:
        # - bits 15-8 (A=1):  D = B[15:8]  = $F0
        # - bits 7-0  (A=0):  D = C[7:0]   = $AA
        # → D = $F0AA
        self.assertEqual(
            blt_trace.lf_combine(0xCA, 0xFF00, 0xF0F0, 0xAAAA),
            0xF0AA)

    def test_lf_ea_recfill_border(self):
        # LF=$EA = D = (A & B) | (A & C) | (B & C) — RectFill border style
        # $EA = 11101010
        # With A=$FFFF, B=$FFFF, C=anything → D should be $FFFF (every term where
        # A=B=1 has D=1; bit 6 of LF (1,1,0)=1, bit 7 (1,1,1)=1)
        self.assertEqual(blt_trace.lf_combine(0xEA, 0xFFFF, 0xFFFF, 0x0000), 0xFFFF)
        self.assertEqual(blt_trace.lf_combine(0xEA, 0xFFFF, 0xFFFF, 0xFFFF), 0xFFFF)
        # With A=0, B=$FFFF, D=C (since bit 2 (0,1,0)=0, bit 3 (0,1,1)=1)
        self.assertEqual(blt_trace.lf_combine(0xEA, 0x0000, 0xFFFF, 0x5555), 0x5555)


class TestPredictWord(unittest.TestCase):
    """Verify per-word D prediction includes BLTAFWM/BLTALWM masking,
    matching the §55c fix in our Verilog blitter."""

    def _rec(self, **kw):
        defaults = dict(
            log_line=0, bltcon=0xEA000103, bltsize=0x0068,
            adat=0xFFFF, bdat=0xFFFF, cdat=0x0000,
            afwm=0xFFFF, alwm=0xFFFF,
        )
        defaults.update(kw)
        return blt_trace.BlitRecord(**defaults)

    def test_unmasked_full_white(self):
        # LF=$EA + A=$FFFF + B=$FFFF + USE_A=0 USE_C=1
        # Word 0 of a 40-word fill → D = $FFFF
        rec = self._rec()
        d = blt_trace.predict_word(rec, cur_word=5, c_word=0x0000)
        self.assertEqual(d, 0xFFFF)

    def test_afwm_corner_mask(self):
        # The §55c case: WB1.3 bottom-border blit has afwm=$3FFF
        # → first word's A gets masked to $3FFF → top 2 bits of word 0
        #   take the (A=0,B=1) branch = D = C bits at those positions.
        # With C=0, D = $3FFF (top 2 bits 0, rest 1).
        rec = self._rec(afwm=0x3FFF, alwm=0xFFFC)
        # cur_word=0 → is_first → A masked to $3FFF
        d0 = blt_trace.predict_word(rec, cur_word=0, c_word=0x0000)
        self.assertEqual(d0, 0x3FFF)
        # cur_word=39 → is_last → A masked to $FFFC
        d_last = blt_trace.predict_word(rec, cur_word=39, c_word=0x0000)
        self.assertEqual(d_last, 0xFFFC)
        # cur_word=10 → middle, no mask → A unchanged = $FFFF
        d_mid = blt_trace.predict_word(rec, cur_word=10, c_word=0x0000)
        self.assertEqual(d_mid, 0xFFFF)


class TestBlitRecord(unittest.TestCase):
    """Verify BlitRecord property decoding matches the internal bltcon layout."""

    def test_decode_canonical_intuition_blit(self):
        # bltcon=$EA000103: LF=$EA, USE_C+USE_D, copy mode
        rec = blt_trace.BlitRecord(log_line=0, bltcon=0xEA000103, bltsize=0x0068)
        self.assertEqual(rec.lf, 0xEA)
        self.assertEqual(rec.use_a, 0)
        self.assertEqual(rec.use_b, 0)
        self.assertEqual(rec.use_c, 1)
        self.assertEqual(rec.use_d, 1)
        self.assertEqual(rec.line, 0)
        self.assertEqual(rec.height, 1)
        self.assertEqual(rec.width, 40)  # $68 & 0x3F = $28 = 40

    def test_decode_line_mode(self):
        # bltcon with bit 11 set = LINE mode
        rec = blt_trace.BlitRecord(log_line=0, bltcon=0xCA000F0B, bltsize=0x0202)
        self.assertEqual(rec.use_a, 1)
        self.assertEqual(rec.use_b, 0)
        self.assertEqual(rec.use_c, 1)
        self.assertEqual(rec.use_d, 1)
        self.assertEqual(rec.line, 1)


class TestRangeParsing(unittest.TestCase):
    """Verify the address-range parser handles common idioms."""

    def test_hex_with_prefix(self):
        self.assertEqual(blt_trace.parse_int('0xFF38'), 0xFF38)

    def test_hex_with_dollar(self):
        self.assertEqual(blt_trace.parse_int('$FF38'), 0xFF38)

    def test_hex_bare(self):
        self.assertEqual(blt_trace.parse_int('FF38'), 0xFF38)

    def test_range_dotdot(self):
        lo, hi = blt_trace.parse_range('0x60c8..0x9f48')
        self.assertEqual((lo, hi), (0x60C8, 0x9F48))


if __name__ == '__main__':
    unittest.main()
