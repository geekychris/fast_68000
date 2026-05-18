; Two back-to-back move.l (A0)+, (A1)+ without dbra.  Verify the
; FIRST iter's data is intact after the second runs.
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        move.l  #src_data, A0
        move.l  #$00006000, A1
        move.l  (A0)+, (A1)+
        move.l  (A0)+, (A1)+

        move.l  $00006000, D1
        cmpi.l  #$11111111, D1
        bne     fail_p0
        move.l  $00006004, D1
        cmpi.l  #$22222222, D1
        bne     fail_p1
        stop    #0

fail_p0: stop #$F800
fail_p1: stop #$F801

src_data:
        .long $11111111
        .long $22222222
