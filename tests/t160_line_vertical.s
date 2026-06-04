; Blitter LINE mode: vertical line down — exercises the AUL bit
; mapping in BLTCON1 that Intuition uses for window borders.
;
; Per FS-UAE blitter.cpp (lines 725-747) + minimig agnus_blitter.v:
;   BLTCON1 bit 4 = AUL  (1 = X dominant, 0 = Y dominant)
;   BLTCON1 bit 3 = SUL  (sign of subordinate)
;   BLTCON1 bit 2 = SUD  (sign of dominant)
;   BLTCON1 bit 1 = SING (single-pixel-per-line)
;   BLTCON1 bit 0 = LINE (must be 1)
;
; Pre-fix m68k_bus.v mapped canon_bltcon1[3:1] → {sud, sul, aul}
; internally, which read SUL as SUD, SUD as SUL, and SING as AUL —
; off-by-one bit positions plus AUL convention flipped vs FS-UAE.
;
; This test: draw a vertical 8-pixel line going down at column 8
; of a 1-word-wide bitmap (dest $1000, one word per row).
; Expected output: each of 8 rows has bit 7 (= column 8 within
; bits 15-0 of the row word) set.
;
;   Real Amiga: $0080 in each of $1000, $1002, $1004, ..., $100E.
;   Pre-fix:   AUL bit-mapping bug → blitter steps X instead of Y →
;               horizontal line in row 0 across 8 columns instead.

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Clear destination (8 rows × 1 word = $10 bytes) --------
        move.l  #$00000000, $1000.W
        move.l  #$00000000, $1004.W
        move.l  #$00000000, $1008.W
        move.l  #$00000000, $100C.W

        bsr     wait_blt_idle

        ; -------- Configure LINE-mode blit --------
        ; Vertical line from (col=8, row=0) to (col=8, row=7).
        ;   dx=0, dy=7, Y is dominant (dy > dx).
        ;
        ; Bresenham init (graphics.library DrawLine convention):
        ;   When dy >= dx:
        ;     BLTAPT  = 4*dx - 2*dy = -14 = $FFFFFFF2
        ;     BLTAMOD = 4*(dx-dy)   = -28 = $FFFFFFE4
        ;     BLTBMOD = 4*dx        =   0
        ;   This keeps BLTAPT negative throughout (since 4*dx = 0 ≤ 0),
        ;   so the subordinate (X) axis never steps — pure vertical line.
        ;   BLTCMOD = BLTDMOD = bytes per row = 2 (1 word per row)

        ; BLTCON0: ASH=8 (pixel column within word), USE_A=1, USE_C=1,
        ;          USE_D=1, LF=$CA  (D = if A then B else C)
        ;   = (8<<12) | (1<<11) | 0 | (1<<9) | (1<<8) | $CA
        ;   = $8000 | $0800 | $0300 | $00CA
        ;   = $8BCA
        move.w  #$8BCA, $40(A0)         ; BLTCON0

        ; BLTCON1: BSH=8, AUL=0 (Y dominant), SUD=0 (+Y), SUL=0 (+X),
        ;          SING=0, LINE=1
        ;   Bit 4 = AUL = 0
        ;   Bit 3 = SUL = 0
        ;   Bit 2 = SUD = 0
        ;   Bit 1 = SING = 0
        ;   Bit 0 = LINE = 1
        ;   = (8 << 12) | $0001 = $8001
        move.w  #$8001, $42(A0)         ; BLTCON1

        ; Masks
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM

        ; BLTBDAT = $FFFF (solid line pattern; no gaps)
        move.w  #$FFFF, $72(A0)

        ; BLTAPT = $FFFFFFF2 (= -14)
        move.w  #$FFFF, $50(A0)         ; BLTAPTH
        move.w  #$FFF2, $52(A0)         ; BLTAPTL
        ; BLTBPT = 0 (unused since USE_B=0)
        move.w  #$0000, $4C(A0)
        move.w  #$0000, $4E(A0)
        ; BLTCPT = $1000 (source = dest = the bitmap)
        move.w  #$0000, $48(A0)
        move.w  #$1000, $4A(A0)
        ; BLTDPT = $1000
        move.w  #$0000, $54(A0)
        move.w  #$1000, $56(A0)

        ; Register order on Amiga:
        ;   $60 BLTCMOD  $62 BLTBMOD  $64 BLTAMOD  $66 BLTDMOD
        move.w  #$0002, $60(A0)         ; BLTCMOD = 2 (1 word per row)
        move.w  #$0000, $62(A0)         ; BLTBMOD = 0 (4*dx, dx=0)
        move.w  #$FFE4, $64(A0)         ; BLTAMOD = -28 (4*(dx-dy))
        move.w  #$0002, $66(A0)         ; BLTDMOD = 2

        ; BLTSIZE: height = 8 pixels, width = 2 (line mode forces 1 word)
        ;   = (8 << 6) | 2 = $0202
        move.w  #$0202, $58(A0)         ; trigger

        bsr     wait_blt_idle

        ; -------- Verify each row has bit 7 set (column 8 of 1-word row) --------
        ; Expected: $0080 at each of $1000, $1002, $1004, $1006, $1008,
        ;           $100A, $100C, $100E.

        move.w  $1000.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r0
        move.w  $1002.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r1
        move.w  $1004.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r2
        move.w  $1006.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r3
        move.w  $1008.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r4
        move.w  $100A.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r5
        move.w  $100C.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r6
        move.w  $100E.W, D0
        cmpi.w  #$0080, D0
        bne     fail_r7

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

; Fail with stop code = row index where the comparison failed.
; D0 carries actual word read (low 16 bits) for diagnostic.
fail_r0:
        move.w  $1000.W, D0
        stop    #$8000          ; row 0 fail (high bit = diagnostic)
fail_r1:
        move.w  $1002.W, D0
        stop    #$8001
fail_r2:
        move.w  $1004.W, D0
        stop    #$8002
fail_r3:
        move.w  $1006.W, D0
        stop    #$8003
fail_r4:
        move.w  $1008.W, D0
        stop    #$8004
fail_r5:
        move.w  $100A.W, D0
        stop    #$8005
fail_r6:
        move.w  $100C.W, D0
        stop    #$8006
fail_r7:
        move.w  $100E.W, D0
        stop    #$8007
