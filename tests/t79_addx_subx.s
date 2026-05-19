; ADDX / SUBX / NEGX register-form: multi-precision arithmetic via X.
;
; Verifies:
;   - ADDX.L Dy, Dx adds Dy + X
;   - SUBX.L Dy, Dx subtracts Dy + X
;   - NEGX.L Dn negates with X
;   - Z is preserved when result is zero (cleared otherwise)
;   - .B / .W size variants only touch the relevant low bits
;   - X carries/borrows across two-long-wide adds and subs.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- 1: ADDX.L with X=0 ----
        andi.w  #$ffee, CCR              ; clear X
        move.l  #$00000005, D0
        move.l  #$00000003, D1
        addx.l  D0, D1                   ; D1 = 3 + 5 + 0 = 8
        cmpi.l  #$00000008, D1
        bne     fail_1

        ; ---- 2: ADDX.L with X=1 ----
        ori.w   #$10, CCR                ; X=1
        move.l  #$00000005, D0
        move.l  #$00000003, D1
        addx.l  D0, D1                   ; D1 = 3 + 5 + 1 = 9
        cmpi.l  #$00000009, D1
        bne     fail_2

        ; ---- 3: ADDX.L sets X on carry-out ----
        andi.w  #$ffee, CCR
        move.l  #$FFFFFFFF, D0
        move.l  #$00000001, D1
        addx.l  D0, D1                   ; result = 0, X=1
        cmpi.l  #$00000000, D1
        bne     fail_3
        move.w  SR, D2
        andi.w  #$10, D2
        cmpi.w  #$10, D2
        bne     fail_3

        ; ---- 4: ADDX Z preserve: nonzero result clears Z ----
        ori.w   #$04, CCR                ; Z=1
        andi.w  #$ffeb, CCR              ; X=0
        move.l  #$00000001, D0
        move.l  #$00000001, D1
        addx.l  D0, D1                   ; result = 2 -> Z must clear
        move.w  SR, D2
        andi.w  #$04, D2
        bne     fail_4

        ; ---- 5: ADDX Z preserve: zero result keeps Z ----
        ori.w   #$04, CCR                ; Z=1
        andi.w  #$ffeb, CCR              ; X=0
        moveq   #0, D0
        moveq   #0, D1
        addx.l  D0, D1                   ; result = 0 -> Z remains 1
        move.w  SR, D2
        andi.w  #$04, D2
        beq     fail_5

        ; ---- 6: SUBX.L with X=0 ----
        andi.w  #$ffee, CCR
        move.l  #$00000010, D0
        move.l  #$00000020, D1
        subx.l  D0, D1                   ; D1 = 0x20 - 0x10 - 0 = 0x10
        cmpi.l  #$00000010, D1
        bne     fail_6

        ; ---- 7: SUBX.L with X=1 ----
        ori.w   #$10, CCR
        move.l  #$00000010, D0
        move.l  #$00000020, D1
        subx.l  D0, D1                   ; D1 = 0x20 - 0x10 - 1 = 0x0F
        cmpi.l  #$0000000F, D1
        bne     fail_7

        ; ---- 8: NEGX.L Dn ----
        andi.w  #$ffee, CCR
        move.l  #$00000005, D0
        negx.l  D0                       ; 0 - 5 - 0 = $FFFFFFFB
        cmpi.l  #$FFFFFFFB, D0
        bne     fail_8
        ori.w   #$10, CCR
        move.l  #$00000005, D0
        negx.l  D0                       ; 0 - 5 - 1 = $FFFFFFFA
        cmpi.l  #$FFFFFFFA, D0
        bne     fail_8

        ; ---- 9: 64-bit add via ADD.L + ADDX.L ----
        andi.w  #$ffee, CCR
        move.l  #$FFFFFFFF, D0           ; lo A
        move.l  #$00000001, D1           ; hi A
        move.l  #$00000001, D2           ; lo B
        move.l  #$00000000, D3           ; hi B
        add.l   D2, D0                   ; lo = 0, X=1
        addx.l  D3, D1                   ; hi = 1 + 0 + 1 = 2
        cmpi.l  #$00000000, D0
        bne     fail_9
        cmpi.l  #$00000002, D1
        bne     fail_9

        ; ---- 10: 64-bit sub via SUB.L + SUBX.L ----
        andi.w  #$ffee, CCR
        move.l  #$00000000, D0           ; lo A
        move.l  #$00000002, D1           ; hi A
        move.l  #$00000001, D2           ; lo B
        move.l  #$00000000, D3           ; hi B
        sub.l   D2, D0                   ; lo = $FFFFFFFF, X=1
        subx.l  D3, D1                   ; hi = 2 - 0 - 1 = 1
        cmpi.l  #$FFFFFFFF, D0
        bne     fail_10
        cmpi.l  #$00000001, D1
        bne     fail_10

        ; ---- 11: ADDX.B only touches LS byte ----
        andi.w  #$ffee, CCR
        move.l  #$AABBCC11, D0
        move.l  #$11223301, D1
        addx.b  D0, D1                   ; 01 + 11 = 12 in LSB
        cmpi.l  #$11223312, D1
        bne     fail_11

        ; ---- 12: ADDX.W only touches LS word ----
        andi.w  #$ffee, CCR
        move.l  #$AABBCC11, D0
        move.l  #$11220034, D1
        addx.w  D0, D1                   ; LS word: 0x0034 + 0xCC11 = 0xCC45
        cmpi.l  #$1122CC45, D1
        bne     fail_12

        stop    #0

fail_1:  stop #$A101
fail_2:  stop #$A102
fail_3:  stop #$A103
fail_4:  stop #$A104
fail_5:  stop #$A105
fail_6:  stop #$A106
fail_7:  stop #$A107
fail_8:  stop #$A108
fail_9:  stop #$A109
fail_10: stop #$A10A
fail_11: stop #$A10B
fail_12: stop #$A10C
