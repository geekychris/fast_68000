; Memory-destination CLR and TST: write zero / set CCR from memory operand.
;
        .org $400

        ;------------------------------------------------------------
        ; Write a non-zero pattern to memory, then CLR it.
        ;------------------------------------------------------------
        lea   $2000, A0
        move.l #$DEADBEEF, (A0)
        clr.l (A0)                  ; mem[A0] := 0
        ; Verify by loading back into D0 and comparing.
        move.l (A0), D0
        cmpi.l #0, D0
        bne   fail
        ; CLR.L should leave Z=1 in CCR.
        beq   ok1
        bra   fail
ok1:
        ;------------------------------------------------------------
        ; CLR.W only clears the low 16 bits of the longword at the EA.
        ; (Actually CLR.W clears the WORD at the EA; if the EA is word-
        ; aligned at offset 0, that's the high half of a longword.)
        ;------------------------------------------------------------
        move.l #$AABBCCDD, (A0)
        clr.w (A0)                  ; clears bytes 0..1 (high half)
        move.l (A0), D0
        cmpi.l #$0000_CCDD, D0
        bne   fail

        ;------------------------------------------------------------
        ; CLR.B clears just one byte.
        ;------------------------------------------------------------
        move.l #$11223344, (A0)
        clr.b (A0)                  ; clears byte 0 (high byte)
        move.l (A0), D0
        cmpi.l #$0022_3344, D0
        bne   fail

        ;------------------------------------------------------------
        ; TST.L on memory: set N/Z from the loaded value.
        ;------------------------------------------------------------
        move.l #0, (A0)
        tst.l (A0)
        bne   fail                  ; Z should be set since (A0) = 0

        move.l #$80000000, (A0)
        tst.l (A0)
        bpl   fail                  ; N should be set

        ;------------------------------------------------------------
        ; TST.B on memory: only the byte at the EA.
        ;------------------------------------------------------------
        move.l #$00FF0000, (A0)
        tst.b (A0)                  ; byte 0 = 0x00 -> Z=1
        bne   fail
        ; (d8(An) addressing not yet supported in our assembler;
        ; the per-byte alignment test would go here.)

        ;------------------------------------------------------------
        ; NEG.L on memory: read, negate, write back, set CCR.
        ;------------------------------------------------------------
        move.l #100, (A0)
        neg.l (A0)
        move.l (A0), D0
        cmpi.l #-100, D0
        bne   fail

        ;------------------------------------------------------------
        ; NOT.L on memory: invert all bits.
        ;------------------------------------------------------------
        move.l #$5A5A_5A5A, (A0)
        not.l (A0)
        move.l (A0), D0
        cmpi.l #$A5A5_A5A5, D0
        bne   fail

        ;------------------------------------------------------------
        ; NEG.B on memory: only the byte at the EA.
        ;------------------------------------------------------------
        move.l #$10_AA_BB_CC, (A0)
        neg.b (A0)                  ; byte 0 (0x10) becomes 0xF0
        move.l (A0), D0
        cmpi.l #$F0_AA_BB_CC, D0
        bne   fail

        stop  #0
fail:
        stop  #$BADC
