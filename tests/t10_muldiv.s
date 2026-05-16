; Exercise MULU.W / MULS.W / DIVU.W / DIVS.W and bit ops (BTST/BCHG/BCLR/BSET)
; and Scc.
;
        .org $400

        ;------------------------------------------------------------
        ; MULU.W: unsigned 16x16 -> 32. 0x1234 * 0x5678 = 0x06260060.
        ;------------------------------------------------------------
        move.l #$1234, D0
        move.l #$5678, D1
        mulu.w D1, D0          ; D0 = D0[15:0] * D1[15:0]
        cmpi.l #$06260060, D0
        bne   fail

        ;------------------------------------------------------------
        ; MULS.W: signed. (-3) * (-5) = 15.
        ;------------------------------------------------------------
        move.l #$FFFD, D2      ; -3 in low word
        move.l #$FFFB, D3      ; -5 in low word
        muls.w D3, D2
        cmpi.l #15, D2
        bne   fail

        ; MULS.W: 30000 * 30000 = 0x35a4_e900 (= 900_000_000)
        move.l #30000, D4
        move.l #30000, D5
        muls.w D5, D4
        cmpi.l #900000000, D4
        bne   fail

        ;------------------------------------------------------------
        ; DIVU.W: 100 / 7 -> quot=14, rem=2.  Result = (rem<<16)|quot.
        ;------------------------------------------------------------
        move.l #100, D0
        move.l #7,   D1
        divu.w D1, D0
        cmpi.l #$0002_000E, D0    ; rem=2, quot=14 (0x0E)
        bne   fail

        ;------------------------------------------------------------
        ; DIVS.W: -100 / 7 -> quot=-14 (signed), rem=-2 (truncated).
        ;------------------------------------------------------------
        move.l #-100, D2
        move.l #7,    D3
        divs.w D3, D2
        ; quot = -14 = 0xFFF2, rem = -2 = 0xFFFE
        cmpi.l #$FFFE_FFF2, D2
        bne   fail

        ;------------------------------------------------------------
        ; BTST/BSET/BCLR/BCHG on Dn.
        ;------------------------------------------------------------
        moveq #0, D4              ; D4 = 0
        bset  #5, D4              ; D4 = 0x20 ; Z reflects pre-set bit (was 0 -> Z=1)
        cmpi.l #$20, D4
        bne   fail
        bset  #5, D4              ; bit already set: Z=0, D4 unchanged
        cmpi.l #$20, D4
        bne   fail
        bchg  #5, D4              ; toggle off: D4 = 0
        cmpi.l #0, D4
        bne   fail
        bset  #0, D4
        bset  #2, D4
        bset  #4, D4              ; D4 = 0x15
        cmpi.l #$15, D4
        bne   fail
        bclr  #2, D4              ; D4 = 0x11
        cmpi.l #$11, D4
        bne   fail

        ;------------------------------------------------------------
        ; BTST sets Z based on whether the bit is 0 (then Z=1).
        ;------------------------------------------------------------
        btst  #4, D4              ; bit 4 of 0x11 = 1 -> Z=0
        beq   fail
        btst  #1, D4              ; bit 1 of 0x11 = 0 -> Z=1
        bne   fail

        ;------------------------------------------------------------
        ; Dynamic BSET: bit number in Dn.
        ;------------------------------------------------------------
        moveq #0, D5
        moveq #10, D6
        bset  D6, D5              ; set bit 10 of D5
        cmpi.l #$400, D5
        bne   fail

        ;------------------------------------------------------------
        ; Scc: SEQ when Z is set.
        ;------------------------------------------------------------
        moveq #0, D0              ; CCR Z = 1 from move
        cmpi.l #0, D0             ; ensure Z=1
        seq   D7                  ; D7.B := 0xFF
        andi.l #$FF, D7           ; isolate byte
        cmpi.l #$FF, D7
        bne   fail
        moveq #1, D0
        cmpi.l #0, D0             ; Z=0
        seq   D7                  ; D7.B := 0x00 (low byte)
        andi.l #$FF, D7
        cmpi.l #$00, D7
        bne   fail

        ; Pass
        stop  #0
fail:
        stop  #$BAD3
