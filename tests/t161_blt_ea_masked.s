; Reproduces the WB Backdrop bottom-border blit from K1.3+WB1.3 boot
; (BLT_BPL1 log at line 32272 of build_kick_boot/run.log).
;
; Setup decoded from BLT_BORDER_TRACE:
;   bltcon=$EA000103
;     LF=$EA, ASH=0, BSH=0, COPY mode,
;     USE_A=0, USE_B=0, USE_C=1, USE_D=1
;   bltsize=$0068 = (1 row << 6) | 40 words = 1 × 40
;   adat=$FFFF (BLTADAT preset)
;   bdat=$FFFF (BLTBDAT preset)
;   afwm=$3FFF (bits 15:14 of A-first-word masked off)
;   alwm=$FFFC (bits 1:0  of A-last-word masked off)
;
; LF=$EA truth table (bit i = D when (A,B,C) encode value i):
;   $EA = 1110_1010 -> (0,0,1)=1, (0,1,1)=1, (1,0,1)=1, (1,1,0)=1, (1,1,1)=1
;   With B=$FFFF: A bit=1 -> D bit=1; A bit=0 -> D bit = C bit
;   So D = A | (~A & C)
;
; With A_first = $FFFF & afwm = $3FFF, A_last = $FFFF & alwm = $FFFC,
; A_middle = $FFFF (no mask).  Dest C content pre-fill = $0000.
;
; Expected after blit:
;   word 0  (first):  D = $3FFF | (~$3FFF & 0) = $3FFF
;   word 1..38:       D = $FFFF | (~$FFFF & 0) = $FFFF
;   word 39 (last):   D = $FFFC | (~$FFFC & 0) = $FFFC
;
; This test triggers an exact replay.  Pre-fix our blitter writes
; row 199 of WB1.3 with $0000 instead of the expected solid-white-with-
; masked-corners pattern — diagnosed in WB13_DEBUG_JOURNAL §55.

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Pre-clear the destination row (40 words at $1000) --------
        lea     $1000.W, A1
        moveq   #39, D0
clr_loop:
        clr.w   (A1)+
        dbra    D0, clr_loop

        ; -------- Wait for blitter idle, then program registers --------
        bsr     wait_blt_idle

        ; BLTADAT = $FFFF (A preset)
        move.w  #$FFFF, $74(A0)
        ; BLTBDAT = $FFFF (B preset)
        move.w  #$FFFF, $72(A0)
        ; BLTCDAT = $0000 (irrelevant, USE_C=1)
        move.w  #$0000, $70(A0)

        ; BLTCON0/1
        move.w  #$03EA, $40(A0)         ; BLTCON0: ASH=0, USE_C=1, USE_D=1, LF=$EA
        move.w  #$0000, $42(A0)         ; BLTCON1: copy mode, no flags

        ; BLTAFWM = $3FFF, BLTALWM = $FFFC
        move.w  #$3FFF, $44(A0)
        move.w  #$FFFC, $46(A0)

        ; BLTCPT = BLTDPT = $1000 (single-row destination)
        move.w  #$0000, $48(A0)         ; BLTCPTH
        move.w  #$1000, $4A(A0)         ; BLTCPTL
        move.w  #$0000, $54(A0)         ; BLTDPTH
        move.w  #$1000, $56(A0)         ; BLTDPTL

        ; Modulos (all 0 — single-row blit, no row transition)
        move.w  #$0000, $60(A0)         ; BLTCMOD
        move.w  #$0000, $62(A0)         ; BLTBMOD
        move.w  #$0000, $64(A0)         ; BLTAMOD
        move.w  #$0000, $66(A0)         ; BLTDMOD

        ; BLTSIZE = (1 << 6) | 40 = $0068 — TRIGGER
        move.w  #$0068, $58(A0)

        bsr     wait_blt_idle

        ; -------- Verify --------
        ; word 0: $3FFF
        move.w  $1000.W, D0
        cmpi.w  #$3FFF, D0
        bne     fail_first

        ; word 1..38: $FFFF
        lea     $1002.W, A1
        moveq   #37, D1
verify_mid:
        move.w  (A1)+, D0
        cmpi.w  #$FFFF, D0
        bne     fail_mid
        dbra    D1, verify_mid

        ; word 39: $FFFC
        move.w  $104E.W, D0
        cmpi.w  #$FFFC, D0
        bne     fail_last

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0
        btst    #14, D0
        bne     wait_blt_idle
        rts

fail_first:
        move.w  $1000.W, D0
        stop    #$0001
fail_mid:
        ; D0 already holds the failing word
        stop    #$0002
fail_last:
        move.w  $104E.W, D0
        stop    #$0003
