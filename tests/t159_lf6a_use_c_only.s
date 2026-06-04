; LF=$6A USE_C=1 USE_D=1 — the second-most-common Workbench icon-area
; blit form (per WB13_DEBUG_JOURNAL §50a).
;
; LF=$6A = 0110_1010 minterms.  With A=BLTADAT_pre=$FFFF (USE_A=0
; default per §38) and B=BLTBDAT_pre=$FFFF (USE_B=0 default), every
; bit position has A=B=1.  LF[(1,1,0)] = bit 6 = 1, LF[(1,1,1)] =
; bit 7 = 0.  So output = NOT C.
;
; Test: set C = $5555, expect D = $AAAA.
;
; This sister test to t158 confirms our blitter handles both forms
; WB1.3 uses in the icon area.  If both pass, the icon-graphic gap
; in the WB1.3 boot is definitively upstream (Workbench's draw
; sequence) and not in the blitter (§51).

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Seed C source --------
        move.w  #$5555, $2000.W

        bsr     wait_blt_idle

        ; -------- Ensure BLTxDAT presets are $FFFF (the default) --------
        move.w  #$FFFF, $74(A0)         ; BLTADAT
        move.w  #$FFFF, $72(A0)         ; BLTBDAT
        move.w  #$0000, $70(A0)         ; BLTCDAT (irrelevant — USE_C=1)

        ; -------- Configure the blit --------
        ; BLTCON0:
        ;   ASH=0, USE-A=0, USE-B=0, USE-C=1, USE-D=1, LF=$6A
        ;   = (0 << 12) | (0 << 11) | (0 << 10) | (1 << 9) | (1 << 8) | $6A
        ;   = $036A
        move.w  #$036A, $40(A0)         ; BLTCON0
        move.w  #$0000, $42(A0)         ; BLTCON1
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        move.w  #$0000, $64(A0)         ; BLTAMOD
        move.w  #$0000, $66(A0)         ; BLTBMOD
        move.w  #$0000, $62(A0)         ; BLTCMOD
        move.w  #$0000, $60(A0)         ; BLTDMOD

        ; BLTCPT = $2000
        move.w  #$0000, $48(A0)
        move.w  #$2000, $4A(A0)
        ; BLTDPT = $3000
        move.w  #$0000, $54(A0)
        move.w  #$3000, $56(A0)

        ; Pre-clear dest
        move.w  #$DEAD, $3000.W

        ; BLTSIZE: 1 row × 1 word
        move.w  #$0041, $58(A0)

        bsr     wait_blt_idle

        ; -------- Verify D == NOT C = $AAAA --------
        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        move.w  D2, D3
        cmpi.l  #$0000AAAA, D2
        bne     fail

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0
        btst    #14, D0
        bne     wait_blt_idle
        rts

fail:
        move.w  D3, D0
        stop    #1
