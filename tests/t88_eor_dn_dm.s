; EOR.L Dn, Dm with direction=1 + EA_DREG.
;
; The K1.3 trackdisk header-checksum loop runs EOR.L D0,D6 in a tight
; loop ($FEACAC).  Pre-fix the K_ALU writeback went to ex_reg_idx_full
; (the source Dn from bits 11:9) instead of the EA's data register
; (Dm from bits 2:0).  Result: D6 never accumulated and every disk
; sector failed checksum validation, causing the strap.task to loop
; on a "no boot disk" error.
;
; This test runs the equivalent XOR loop in isolation: build D6 by
; EOR'ing in a sequence of longwords from a small constant buffer.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; D6 = 0 (accumulator).
        moveq   #0, D6

        ; D0 = $55000005, EOR into D6  -> D6 = $55000005
        move.l  #$55000005, D0
        eor.l   D0, D6
        cmpi.l  #$55000005, D6
        bne     fail1

        ; D0 = $55000001, EOR into D6  -> D6 = $00000004
        move.l  #$55000001, D0
        eor.l   D0, D6
        cmpi.l  #$00000004, D6
        bne     fail2

        ; D0 = $00000000, EOR into D6  -> D6 = $00000004 (unchanged)
        moveq   #0, D0
        eor.l   D0, D6
        cmpi.l  #$00000004, D6
        bne     fail3

        ; EOR with non-zero in BOTH directions: ensure the *destination*
        ; (Dm = D6) is updated, not the source (Dn = D0).
        move.l  #$DEADBEEF, D0
        move.l  #$12345678, D6
        eor.l   D0, D6                     ; D6 = $DEADBEEF ^ $12345678 = $CC99E897
        cmpi.l  #$CC99E897, D6
        bne     fail4
        ; D0 must NOT have changed.
        cmpi.l  #$DEADBEEF, D0
        bne     fail5

        stop    #0

fail1:  stop    #$D101
fail2:  stop    #$D102
fail3:  stop    #$D103
fail4:  stop    #$D104
fail5:  stop    #$D105
