; Smoke test: move.l Dn, <abs.l>.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        move.l  #$CAFE1234, D6
        move.l  D6, $00010000
        move.l  $00010000, D1
        cmpi.l  #$CAFE1234, D1
        bne     fail
        stop    #0
fail:   stop    #$ba97
