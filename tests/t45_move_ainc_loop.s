; Tight `move.l (A0)+, (A1)+` loop.  Verify the dst A1 advances each
; iteration so that 4 distinct longs land at 4 distinct addresses.
;
; Also exercises -(An) on dst symmetrically.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- AINC -> AINC loop (4 iters) ----
        move.l  #src_data, A0
        move.l  #$00006000, A1
        moveq   #3, D0
loop_p: move.l  (A0)+, (A1)+
        dbra    D0, loop_p

        ; Expect mem[$6000..$600F] = $11111111 $22222222 $33333333 $AABBCCDD.
        move.l  $00006000, D1
        cmpi.l  #$11111111, D1
        beq     p0_ok
        cmpi.l  #$00000000, D1
        beq     p0_zero
        cmpi.l  #$22222222, D1
        beq     p0_22
        cmpi.l  #$33333333, D1
        beq     p0_33
        cmpi.l  #$AABBCCDD, D1
        beq     p0_last
        stop    #$F4FF
p0_zero: stop #$F400
p0_22:   stop #$F422
p0_33:   stop #$F433
p0_last: stop #$F4AA
p0_ok:
        move.l  $00006004, D1
        cmpi.l  #$22222222, D1
        bne     fail_p1
        move.l  $00006008, D1
        cmpi.l  #$33333333, D1
        bne     fail_p2
        move.l  $0000600C, D1
        cmpi.l  #$AABBCCDD, D1
        bne     fail_p3

        ; ---- ADEC -> ADEC loop ----
        ; Pre-decrement: each move.l uses (Ax) AFTER decrementing Ax by 4.
        ; A0 starts at src_data + 16 (one past the table), A1 at $00007010.
        move.l  #src_data, A0
        adda.l  #16, A0
        move.l  #$00007010, A1
        moveq   #3, D0
loop_m: move.l  -(A0), -(A1)
        dbra    D0, loop_m

        ; After 4 iters, A0 should be back at src_data, A1 at $7000.
        ; mem[$7000..$700C] should be $11111111 $22222222 $33333333 $AABBCCDD
        ; (same order as src_data, but written right-to-left).
        move.l  $00007000, D1
        cmpi.l  #$11111111, D1
        bne     fail_m0
        move.l  $00007004, D1
        cmpi.l  #$22222222, D1
        bne     fail_m1
        move.l  $00007008, D1
        cmpi.l  #$33333333, D1
        bne     fail_m2
        move.l  $0000700C, D1
        cmpi.l  #$AABBCCDD, D1
        bne     fail_m3

        stop    #0

fail_p0: stop #$F4D0
fail_p1: stop #$F4D1
fail_p2: stop #$F4D2
fail_p3: stop #$F4D3
fail_m0: stop #$F4E0
fail_m1: stop #$F4E1
fail_m2: stop #$F4E2
fail_m3: stop #$F4E3

src_data:
        .long $11111111
        .long $22222222
        .long $33333333
        .long $AABBCCDD
