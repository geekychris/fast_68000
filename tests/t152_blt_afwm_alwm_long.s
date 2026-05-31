; MOVE.L to BLTAFWM ($DFF044) must commit both BLTAFWM and BLTALWM
; ($DFF046).  K1.3 trackdisk's sector-copy preamble does:
;     MOVEQ.L #-1, D0
;     MOVE.L  D0, $DFF044
; to set both first-word and last-word masks to $FFFF in a single
; instruction.  Pre-fix, our chipset translated this as a single
; 16-bit write to BLTAFWM and dropped the low half intended for
; BLTALWM, leaving BLTALWM with its stale value from the previous
; blit.  At WB1.3 boot r=116M this stale BLTALWM was $FFFC, which
; cleared bit 1 of the last A word in the MFM-decode sector-copy
; blit and flipped byte 1 of the bootblock magic from $4F to $4D
; ('DOS\0' -> 'DMS\0'), wedging the boot at the TD_CHANGENUM poll.
;
; This test pre-seeds BLTALWM=$FFFC, then writes
; MOVE.L #$FFFFFFFF, $DFF044, then reads BLTALWM back.  Pre-fix
; the read returns $FFFC (stale).  Post-fix it returns $FFFF.

        .org $400
start:
        ; -------- Setup --------
        move.l  #$00DFF000, A0          ; A0 = custom chipset base
        ; Pre-seed BLTALWM = $FFFC via a regular 16-bit write
        move.w  #$FFFC, $46(A0)
        ; Read it back to make sure the seed worked
        move.w  $46(A0), D2
        cmpi.l  #$0000FFFC, D2
        bne     fail_seed

        ; -------- The fix under test --------
        ; MOVE.L #$FFFFFFFF, $DFF044 -- one instruction, longword
        ; write spanning BLTAFWM (high half, addr $DFF044) and
        ; BLTALWM (low half, addr $DFF046).
        move.l  #$FFFFFFFF, $44(A0)

        ; -------- Verify both halves --------
        ; BLTAFWM read
        move.w  $44(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000FFFF, D2
        bne     fail_afwm
        ; BLTALWM read -- this is the one that was broken
        move.w  $46(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000FFFF, D2
        bne     fail_alwm

        ; -------- Round-trip with a non-symmetric value --------
        ; Seed both with sentinels to ensure independence
        move.w  #$DEAD, $44(A0)
        move.w  #$BEEF, $46(A0)
        move.w  $44(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000DEAD, D2
        bne     fail_round_afwm
        move.w  $46(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BEEF, D2
        bne     fail_round_alwm

        ; Now MOVE.L $1234_5678 -- the high half goes to BLTAFWM,
        ; low half to BLTALWM
        move.l  #$12345678, $44(A0)
        move.w  $44(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00001234, D2
        bne     fail_split_afwm
        move.w  $46(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00005678, D2
        bne     fail_split_alwm

        stop    #0                       ; pass

fail_seed:        stop   #1
fail_afwm:        stop   #2
fail_alwm:        stop   #3
fail_round_afwm:  stop   #4
fail_round_alwm:  stop   #5
fail_split_afwm:  stop   #6
fail_split_alwm:  stop   #7
