; Exercise CHK and RTR.
;
        .org $18                ; vector 6 = CHK
        .long chk_handler

        .org $400

        ;------------------------------------------------------------
        ; CHK with Dn in bounds: should NOT trap.
        ;------------------------------------------------------------
        moveq #0, D7
        move.l #5, D0
        move.l #10, D1
        chk   D1, D0               ; 5 <= 10 and >= 0, no trap
        cmpi.l #0, D7
        bne   fail

        ;------------------------------------------------------------
        ; CHK out of bounds (Dn > bound): trap to vec 6.
        ;------------------------------------------------------------
        moveq #0, D7
        move.l #20, D0
        move.l #10, D1
        chk   D1, D0               ; 20 > 10, should trap
        ; After RTE, D7 should be set.
        cmpi.l #$AA, D7
        bne   fail

        ;------------------------------------------------------------
        ; CHK with negative Dn: trap.
        ;------------------------------------------------------------
        moveq #0, D7
        move.l #$FFFF, D0          ; -1 in low word
        move.l #10, D1
        chk   D1, D0               ; -1 < 0, should trap
        cmpi.l #$AA, D7
        bne   fail

        ;------------------------------------------------------------
        ; RTR: build a frame with CCR (Z=1) and a return PC. RTR should
        ; restore CCR and jump to the PC, but not change supervisor mode.
        ;------------------------------------------------------------
        move.l #after_rtr, -(A7)   ; push PC
        move.l #$0000_0004, -(A7)  ; push CCR=Z=1 (low byte = 0x04)
        rtr                         ; pop CCR + PC, jump

        ; If RTR doesn't work, we keep falling through:
        stop  #$BAD8

after_rtr:
        ; Verify Z=1 (CCR was restored).
        beq   ok_rtr
        stop  #$BAD9
ok_rtr:
        stop  #0

fail:
        stop  #$BADA

chk_handler:
        move.l #$0000_00AA, D7   ; full 32-bit so no sign-extension
        rte
