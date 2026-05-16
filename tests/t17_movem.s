; Exercise MOVEM.L with the canonical prologue/epilogue patterns:
;   MOVEM.L  reglist, -(SP)    ; save
;   MOVEM.L  (SP)+, reglist    ; restore
;
        .org $400

        ;------------------------------------------------------------
        ; Set up some distinctive register values.
        ;------------------------------------------------------------
        move.l #$AAAA_0001, D0
        move.l #$AAAA_0002, D1
        move.l #$AAAA_0003, D2
        move.l #$AAAA_0007, D7
        move.l #$AAAA_0010, A0
        move.l #$AAAA_0014, A4

        ;------------------------------------------------------------
        ; Save these regs on the stack via MOVEM.L reglist, -(SP).
        ;------------------------------------------------------------
        move.l A7, D6              ; snapshot SP before save

        movem.l D0/D1/D2/D7/A0/A4, -(A7)

        ; SP should have decreased by 6 longs = 24 bytes.
        move.l A7, D5
        sub.l  D5, D6              ; D6 := old_sp - new_sp = 24
        cmpi.l #24, D6
        bne   fail

        ;------------------------------------------------------------
        ; Clobber all the registers — then restore via (A7)+.
        ;------------------------------------------------------------
        moveq #0, D0
        moveq #0, D1
        moveq #0, D2
        moveq #0, D7
        move.l #0, A0
        move.l #0, A4

        movem.l (A7)+, D0/D1/D2/D7/A0/A4

        ;------------------------------------------------------------
        ; Verify each register is back to its saved value.
        ;------------------------------------------------------------
        cmpi.l #$AAAA_0001, D0
        bne   fail
        cmpi.l #$AAAA_0002, D1
        bne   fail
        cmpi.l #$AAAA_0003, D2
        bne   fail
        cmpi.l #$AAAA_0007, D7
        bne   fail
        move.l A0, D3
        cmpi.l #$AAAA_0010, D3
        bne   fail
        move.l A4, D3
        cmpi.l #$AAAA_0014, D3
        bne   fail

        ;------------------------------------------------------------
        ; SP should now be back to its original value.
        ;------------------------------------------------------------
        move.l A7, D4
        cmpi.l #0, D6              ; we already overwrote D6 above; recompute
        ; Actually just verify directly:
        ; D5 holds old_sp - 24 (after first MOVEM). After restore SP=old_sp.
        ; So current A7 - D5 = 24.
        sub.l  D5, D4
        cmpi.l #24, D4
        bne   fail

        stop  #0
fail:
        stop  #$BADD
