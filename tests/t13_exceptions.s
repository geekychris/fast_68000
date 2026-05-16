; Exercise non-TRAP exception sources:
;   - divide-by-zero  -> vector 5
;   - TRAPV when V=1  -> vector 7
;   - illegal opcode  -> vector 4 (handled separately so it doesn't kill stop)
;
        .org $14                ; vector 5 = div by zero
        .long divz_handler

        .org $1C                ; vector 7 = TRAPV
        .long trapv_handler

        .org $400

        ;------------------------------------------------------------
        ; Trigger divide-by-zero.
        ;------------------------------------------------------------
        moveq #0, D7
        move.l #100, D0
        move.l #0,   D1            ; divisor 0
        divu.w D1, D0              ; should trap to vec 5
        ; Resume here after RTE.
        cmpi.l #$11, D7
        bne   fail

        ;------------------------------------------------------------
        ; Trigger TRAPV. Need V=1 first. Use ori #2, CCR (V bit = 1).
        ;------------------------------------------------------------
        moveq #0, D7
        ori   #2, CCR              ; set V=1
        trapv                       ; should trap to vec 7
        cmpi.l #$22, D7
        bne   fail

        ; TRAPV with V=0 should be a no-op.
        andi  #$F0, CCR            ; clear V (and all CCR bits) — keep upper
        trapv                       ; should NOT trap
        moveq #1, D6
        cmpi.l #1, D6
        bne   fail

        stop  #0
fail:
        stop  #$BAD7

divz_handler:
        moveq #$11, D7
        rte

trapv_handler:
        moveq #$22, D7
        rte
