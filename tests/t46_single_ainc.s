; Single move.l (A0)+, (A1)+ then read back and check.
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
        nop
        move.l  $00006000, D1
        cmpi.l  #$11111111, D1
        bne     fail_d
        stop    #0
fail_d: stop #$F500
src_data:
        .long $11111111
