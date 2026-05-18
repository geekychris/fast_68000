; Coverage for fixes that the existing suite didn't reach:
;   - EXG A,A and EXG D,A (the D,D form is covered by t52)
;   - d8(PC, Xn.size) PC-relative indexed addressing
;   - ADD #imm,An auto-promoted to ADDA by the assembler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- EXG A,A ----
        move.l  #$AAAA0000, A0
        move.l  #$BBBB0000, A1
        exg     A0, A1
        move.l  A0, D0
        cmpi.l  #$BBBB0000, D0
        bne     fail_1
        move.l  A1, D0
        cmpi.l  #$AAAA0000, D0
        bne     fail_2

        ; ---- EXG D,A (mixed) ----
        move.l  #$DEAD0001, D2
        move.l  #$CAFE0001, A2
        exg     D2, A2
        cmpi.l  #$CAFE0001, D2
        bne     fail_3
        move.l  A2, D0
        cmpi.l  #$DEAD0001, D0
        bne     fail_4

        ; ---- d8(PC, Xn.W) PC-relative indexed ----
        ; Fetch pc_table[D0] where each entry is a long.
        moveq   #4, D0                  ; index into table (one long = 4 bytes)
        lea     pc_table(pc), A0
        move.l  0(A0, D0.w), D1
        cmpi.l  #$22222222, D1
        bne     fail_5

        ; ---- d8(PC, An.L) ----
        moveq   #0, D0
        move.l  #8, A3
        lea     pc_table(pc), A0
        move.l  0(A0, A3.l), D1
        cmpi.l  #$33333333, D1
        bne     fail_6

        stop    #0

pc_table:
        .long $11111111
        .long $22222222
        .long $33333333

fail_1: stop #$DB01
fail_2: stop #$DB02
fail_3: stop #$DB03
fail_4: stop #$DB04
fail_5: stop #$DB05
fail_6: stop #$DB06
