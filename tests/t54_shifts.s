; Shift and rotate instructions on Dn.
; ASL/ASR (arithmetic), LSL/LSR (logical), ROL/ROR (rotate),
; ROXL/ROXR (rotate through X).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- ASL.L #1, Dn doubles ----
        move.l  #$00000040, D0
        asl.l   #1, D0
        cmpi.l  #$00000080, D0
        bne     fail_1

        ; ---- ASR.L #1, Dn (signed right shift) ----
        move.l  #$80000000, D0
        asr.l   #1, D0
        cmpi.l  #$C0000000, D0       ; sign-extended
        bne     fail_2

        ; ---- LSR.L #1, Dn (unsigned right shift) ----
        move.l  #$80000000, D0
        lsr.l   #1, D0
        cmpi.l  #$40000000, D0
        bne     fail_3

        ; ---- LSL.L by Dn (dynamic count) ----
        move.l  #$00000001, D0
        moveq   #16, D1
        lsl.l   D1, D0
        cmpi.l  #$00010000, D0
        bne     fail_4

        ; ---- ROL.L #4 rotates left ----
        move.l  #$80000001, D0
        rol.l   #4, D0
        cmpi.l  #$00000018, D0
        bne     fail_5

        ; ---- ROR.L #4 rotates right ----
        move.l  #$00000018, D0
        ror.l   #4, D0
        cmpi.l  #$80000001, D0
        bne     fail_6

        ; ---- ASL.W #1 sets V if sign changes ----
        move.l  #$00004000, D0
        asl.w   #1, D0
        ; Result = $00008000, V should be set
        move.w  SR, D1               ; 68000: read SR; low byte = CCR
        andi.w  #$0002, D1           ; V flag
        cmpi.w  #$0002, D1
        bne     fail_7

        ; ---- ROXL with X flag ----
        andi.w  #$F0, CCR            ; clear X
        ori.w   #$10, CCR            ; set X (bit 4 of CCR)
        move.l  #$00000000, D0
        roxl.l  #1, D0
        ; X=1 rotates in; result = 1
        cmpi.l  #$00000001, D0
        bne     fail_8

        ; ---- ROR.B preserves upper bytes ----
        move.l  #$AABBCC81, D0
        ror.b   #1, D0
        ; Only low byte rotated: $81 → $C0 (1000_0001 → 1100_0000)
        cmpi.l  #$AABBCCC0, D0
        bne     fail_9

        stop    #0

fail_1: stop #$D901
fail_2: stop #$D902
fail_3: stop #$D903
fail_4: stop #$D904
fail_5: stop #$D905
fail_6: stop #$D906
fail_7: stop #$D907
fail_8: stop #$D908
fail_9: stop #$D909
