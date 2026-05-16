; Exercise d16(An) addressing.
;
        .org $400

        lea   $2000, A0
        ; Populate a small struct-like layout at A0:
        ;   +0:  $11111111
        ;   +4:  $22222222
        ;   +8:  $33333333
        ;   -4:  $FFFFFFFF (just below the base, for negative-disp test)
        move.l #$11111111, (A0)
        move.l #$22222222, 4(A0)
        move.l #$33333333, 8(A0)
        move.l #$FFFFFFFF, -4(A0)

        ; Read them back via d16(An) and verify.
        move.l (A0), D0
        cmpi.l #$11111111, D0
        bne   fail
        move.l 4(A0), D0
        cmpi.l #$22222222, D0
        bne   fail
        move.l 8(A0), D0
        cmpi.l #$33333333, D0
        bne   fail
        move.l -4(A0), D0
        cmpi.l #$FFFFFFFF, D0
        bne   fail

        ; ADD.L src d16(An) into a Dn.
        moveq #1, D1
        add.l 4(A0), D1            ; D1 = 1 + $22222222 = $22222223
        cmpi.l #$22222223, D1
        bne   fail

        ; CLR.L on d16(An) clears that slot only.
        clr.l 4(A0)
        move.l (A0), D2
        cmpi.l #$11111111, D2
        bne   fail
        move.l 4(A0), D2
        cmpi.l #0, D2
        bne   fail
        move.l 8(A0), D2
        cmpi.l #$33333333, D2
        bne   fail

        stop  #0
fail:
        stop  #$BADE
