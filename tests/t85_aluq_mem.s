; ADDQ/SUBQ to memory: previously the executor only handled Dn/An
; destinations, so addq.l #1, $00010000 was a silent no-op.  Fix shares
; the K_ALUI mem-dst RMW path.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- 1: ADDQ.L #1, abs.l (the original bug repro) ----
        move.l  #0, $00010000
        addq.l  #1, $00010000
        addq.l  #1, $00010000
        addq.l  #1, $00010000
        move.l  $00010000, D0
        cmpi.l  #3, D0
        bne     fail_1

        ; ---- 2: SUBQ.L #2, abs.l ----
        move.l  #$00000010, $00010000
        subq.l  #2, $00010000
        subq.l  #2, $00010000
        move.l  $00010000, D0
        cmpi.l  #$0000000C, D0
        bne     fail_2

        ; ---- 3: ADDQ.W #imm, abs.l ----
        move.l  #$AABBCC00, $00010004
        addq.w  #5, $00010006           ; word at offset 2 of long
        move.l  $00010004, D0
        cmpi.l  #$AABBCC05, D0
        bne     fail_3

        ; ---- 4: ADDQ.B #imm, abs.l ----
        move.l  #$11223340, $00010008
        addq.b  #3, $0001000B
        move.l  $00010008, D0
        cmpi.l  #$11223343, D0
        bne     fail_4

        ; ---- 5: ADDQ.L #imm, (An) ----
        move.l  #$00000005, $0001000C
        move.l  #$0001000C, A0
        addq.l  #4, (A0)
        move.l  $0001000C, D0
        cmpi.l  #$00000009, D0
        bne     fail_5

        ; ---- 6: SUBQ.L #imm, d(An) ----
        move.l  #$00000020, $00010010
        move.l  #$00010000, A1
        subq.l  #5, 16(A1)              ; addr = $10010
        move.l  $00010010, D0
        cmpi.l  #$0000001B, D0
        bne     fail_6

        ; ---- 7: ADDQ.L #8, abs.l (the "#0 means 8" special case) ----
        move.l  #$00000000, $00010014
        addq.l  #8, $00010014
        move.l  $00010014, D0
        cmpi.l  #$00000008, D0
        bne     fail_7

        ; ---- 8: ADDQ sets X (chains into ADDX) ----
        move.l  #$FFFFFFFF, $00010018
        andi.w  #$ffee, CCR              ; clear X
        addq.l  #1, $00010018             ; result 0, X=1
        move.w  SR, D0
        andi.l  #$10, D0
        cmpi.l  #$10, D0
        bne     fail_8
        move.l  $00010018, D0
        cmpi.l  #$00000000, D0
        bne     fail_8

        stop    #0

fail_1: stop #$A201
fail_2: stop #$A202
fail_3: stop #$A203
fail_4: stop #$A204
fail_5: stop #$A205
fail_6: stop #$A206
fail_7: stop #$A207
fail_8: stop #$A208
