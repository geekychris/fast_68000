; Negative BLTDMOD via MOVE.W must sign-extend through the bus adapter
; and the blitter's slave port, so the row-end pointer arithmetic
; subtracts from BLTDPT instead of walking it FORWARD by 65520+ bytes.
;
; Pre-fix the blitter slave port stored BLT*MOD as
;   bltdmod <= slv_wdata     ; zero-ext: $0000FFFC
; and the row-end S_ROW arm did
;   bltdpt <= bltdpt + bltdmod
; so DPT walked the wrong direction on negative modulos.  Caught via
; `make crosscheck-minimig-blt` Test 5 (BLTDMOD=$FFEC, bltdpt jumped
; $6004 → $15FF0 instead of $5FF0).  Fix at rtl/chipset/blitter.v
; slv_wdata sign-ext for slv_addr 4'h7/8/9/A (mirrors bus adapter).
;
; This test exercises the FULL CPU → bus adapter → blitter path with a
; .W write to BLTDMOD = $FFFC (-4 bytes/row).  A 2-row × 1-word B->D
; copy with BLTBPT = $1000, BLTDPT = $5000 must place row 0 at $5000
; and row 1 at $4FFE (because $5002 + (-4) = $4FFE).  Pre-fix row 1
; landed somewhere around $14FFE, leaving $4FFE at the sentinel.
;
; Discovered 2026-06-12 while bringing Test 5 of `make crosscheck-minimig-blt`
; up to green; the slave-port sign-extension fix is defense-in-depth even
; though the bus adapter already sign-extends — keeps the blitter
; self-consistent for any future bus adapter.

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Seed source words --------
        ; Source at $1000 = $A5A5, $1002 = $5A5A
        move.w  #$A5A5, $1000.W
        move.w  #$5A5A, $1002.W

        ; -------- Seed destination sentinels --------
        ; Row 0 dest = $5000, Row 1 dest = $4FFE
        move.w  #$DEAD, $4FFE.W
        move.w  #$DEAD, $5000.W

        bsr     wait_blt_idle

        ; -------- Configure blit --------
        ; BLTCON0 = $05CC: USE-B + USE-D, LF=$CC (pass-B), ASH=0.
        ; ($5CC bits 11..8 = 0101 = USE_A=0,USE_B=1,USE_C=0,USE_D=1.)
        move.w  #$05CC, $40(A0)         ; BLTCON0
        move.w  #$0000, $42(A0)         ; BLTCON1
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        ; BLTBMOD = 0 (B-pointer end-of-row delta).
        move.w  #$0000, $62(A0)         ; BLTBMOD
        ; BLTDMOD = -4 bytes per row.  THIS IS THE FIX SUBJECT.
        ; .W write must sign-extend to $FFFFFFFC across the bus adapter
        ; AND across the blitter slave port so row-end DPT lands at $4FFE.
        move.w  #$FFFC, $66(A0)         ; BLTDMOD = -4
        ; BLTBPT = $1000
        move.w  #$0000, $4C(A0)         ; BLTBPTH
        move.w  #$1000, $4E(A0)         ; BLTBPTL
        ; BLTDPT = $5000
        move.w  #$0000, $54(A0)         ; BLTDPTH
        move.w  #$5000, $56(A0)         ; BLTDPTL

        ; BLTSIZE: 2 rows × 1 word = (2<<6)|1 = $0081
        move.w  #$0081, $58(A0)

        bsr     wait_blt_idle

        ; -------- Verify --------
        ; Row 0 dest $5000 must hold source word #0 = $A5A5
        move.w  $5000.W, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000A5A5, D2
        bne     fail_row0

        ; Row 1 dest $4FFE must hold source word #1 = $5A5A.
        ; Pre-fix: $4FFE would still be $DEAD because row 1 wrote to
        ; $14FFE somewhere outside the test's check window.
        move.w  $4FFE.W, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00005A5A, D2
        bne     fail_row1

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

fail_row0:        stop   #1
fail_row1:        stop   #2
