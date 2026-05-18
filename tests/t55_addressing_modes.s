; Addressing-mode coverage — the patterns Amiga code uses constantly:
;   move.l (4).W, A6       ; ExecBase
;   move.l 34(A6), D0      ; struct field via d16(An)
;   move.b table(PC,D0.w), D1   ; lookup via PC-relative indexed
;   jsr   _LVOWaitTOF(A6)  ; library jump table

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- d16(An) addressing ----
        move.l  #$00009000, A0
        move.l  #$12345678, $00009008
        move.l  8(A0), D0
        cmpi.l  #$12345678, D0
        bne     fail_1

        ; ---- d16(An) with negative displacement ----
        move.l  #$DEADBEEF, $00008FFC
        move.l  -4(A0), D0
        cmpi.l  #$DEADBEEF, D0
        bne     fail_2

        ; ---- d16(An) for STORE ----
        move.l  #$CAFEBABE, D0
        move.l  D0, 12(A0)
        move.l  $0000900C, D0
        cmpi.l  #$CAFEBABE, D0
        bne     fail_3

        ; ---- abs.W (sign-extended to 32-bit) ----
        ; abs.W with value $7FFC means address $00007FFC.
        move.l  #$AAAA1111, $00007FFC
        move.l  $7FFC.w, D0
        cmpi.l  #$AAAA1111, D0
        bne     fail_4

        ; ---- d8(An, Dn.W) -- byte displacement + word-sized index ----
        move.l  #$11111111, $00009020
        move.l  #$22222222, $00009024
        move.l  #$33333333, $00009028
        move.l  #$00009020, A0
        move.l  #4, D1
        move.l  0(A0, D1.w), D0
        cmpi.l  #$22222222, D0
        bne     fail_5

        ; ---- d8(An, Dn.L) -- byte displacement + long-sized index ----
        move.l  #8, D1
        move.l  0(A0, D1.l), D0
        cmpi.l  #$33333333, D0
        bne     fail_6

        ; ---- d8(An, An.L) -- index register can be An too ----
        move.l  #4, A2
        move.l  0(A0, A2.l), D0
        cmpi.l  #$22222222, D0
        bne     fail_7

        ; ---- d16(PC) for read-only data ----
        ; pc_lookup is a table after the code.  d16 from current PC.
        lea     pc_lookup(pc), A1
        move.l  (A1), D0
        cmpi.l  #$ABCDEF01, D0
        bne     fail_8

        stop    #0

fail_1: stop #$DA01
fail_2: stop #$DA02
fail_3: stop #$DA03
fail_4: stop #$DA04
fail_5: stop #$DA05
fail_6: stop #$DA06
fail_7: stop #$DA07
fail_8: stop #$DA08

pc_lookup:
        .long $ABCDEF01
        .long $12340000
