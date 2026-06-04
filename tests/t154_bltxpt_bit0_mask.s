; Real Amiga BLTxPTL has bit 0 hardwired to 0 (word-aligned). All four
; BLTAPT/BLTBPT/BLTCPT/BLTDPT registers must mask off bit 0 of any
; 16-bit value written to their LOW half.
;
; K1.3 trackdisk MFM-decode preamble at $FEA98E does effectively:
;     MOVE.L  D7, BLTBPT     ; D7 carries a struct pointer
;     MOVE.L  D7, BLTCPT
;     MOVE.L  D7, BLTDPT
; where D7 can be odd (e.g. $762D).  Real Agnus masks bit 0 to give
; $762C; we used to store the odd value as-is.  In descending mode the
; offset accumulation aliased to chip $2D24, corrupting the cyl-53
; sector-3 sync word $4489 and wedging the WB1.3 boot at
; TD_CHANGENUM.  Fix: blitter masks bit 0 on BLTxPT writes.
;
; This test is behavioural: write source/dest, set BLTDPT to an ODD
; address (chip $1001), kick a tiny 1-word D-only blit with all-ones
; pattern, then verify the blit wrote to chip $1000 (even-masked),
; NOT to $1001 (which would be a byte-aligned write that hits $1000
; and $1002 if not masked).

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; Seed dest area: $1000=$DEAD, $1002=$BEEF, $1004=$F00D
        move.w  #$DEAD, $1000.W
        move.w  #$BEEF, $1002.W
        move.w  #$F00D, $1004.W

        ; Wait for blitter not-busy
        bsr     wait_blt_idle

        ; Program a D-only minterm-=0 blit: LF=$00 (write D=0), USE=D,
        ; ASH=0, BSH=0, no shift, no fill, no desc.
        ; BLTCON0 = ($0_00 << 8) | LF=$00  ->  $0_100? actually:
        ;   ASH=4 bits at [15:12]=0, USE-A=0, USE-B=0, USE-C=0,
        ;   USE-D=1, LF=$00 -> word = $0100
        move.w  #$0100, $40(A0)         ; BLTCON0  ASH=0, USE=D, LF=$00
        move.w  #$0000, $42(A0)         ; BLTCON1  no DESC, no LINE
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        move.w  #$0000, $66(A0)         ; BLTDMOD = 0

        ; BLTDPTH = $0000  BLTDPTL = $1001 (odd!)
        move.w  #$0000, $54(A0)         ; BLTDPTH
        move.w  #$1001, $56(A0)         ; BLTDPTL -- bit 0 must be masked

        ; BLTSIZE: rows=1, words=1  -> $0401 in the encoded form
        ; bit layout: height[15:6]=1, width[5:0]=1 -> $0041
        move.w  #$0041, $58(A0)         ; BLTSIZE -- starts the blit

        ; Wait for blit done
        bsr     wait_blt_idle

        ; Expected: $1000 was overwritten to 0 (LF=0 outputs zero).
        ;           $1002 untouched ($BEEF).
        ;           $1004 untouched ($F00D).
        move.w  $1000.W, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000000, D2
        bne     fail_d1000
        move.w  $1002.W, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BEEF, D2
        bne     fail_d1002

        stop    #0                       ; pass

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

fail_d1000:      stop   #1
fail_d1002:      stop   #2
