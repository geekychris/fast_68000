; Exercise the new Tier 3 instruction kinds: CLR/TST/NEG/NOT, EXT, SWAP,
; ADDI/SUBI/CMPI immediate forms.
        .org $400

        ; clr.l D0   -> D0 = 0, flags Z=1
        moveq #-1, D0
        clr.l D0
        cmpi.l #0, D0
        bne   fail

        ; addi.l #100, D0  -> D0 = 100
        addi.l #100, D0
        cmpi.l #100, D0
        bne   fail

        ; subi.l #50, D0   -> D0 = 50
        subi.l #50, D0
        cmpi.l #50, D0
        bne   fail

        ; neg.l D0  -> D0 = -50
        neg.l D0
        cmpi.l #-50, D0
        bne   fail

        ; not.l D0  -> D0 = ~(-50) = 49
        not.l D0
        cmpi.l #49, D0
        bne   fail

        ; swap halves: D0 high <-> low. 49 = 0x00000031, swap = 0x00310000
        swap D0
        cmpi.l #$00310000, D0
        bne   fail

        ; ext.w / ext.l: sign-extend
        moveq #-1, D1            ; D1 = 0xFFFFFFFF
        moveq #$7F, D2           ; D2 = 0x0000007F
        ext.w D2                 ; sign-ext byte 0x7F -> word 0x007F (still 0x7F)
        cmpi.l #$0000007F, D2
        bne   fail
        moveq #-2, D3            ; D3 = 0xFFFFFFFE
        ext.w D3                 ; byte 0xFE sign-extends to 0xFFFE in low word
        cmpi.l #$FFFFFFFE, D3
        bne   fail

        ; All pass
        stop  #0
fail:
        stop  #$BAD1
