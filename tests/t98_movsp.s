; Smoke test: move.l Dn, -(A7) and move.l (A7)+, Dn (predec/postinc A7).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        ; Set D6 to a sentinel, push, clobber, pop, verify.
        move.l  #$DEADBEEF, D6
        move.l  D6, -(A7)
        move.l  #0, D6
        move.l  (A7)+, D6
        cmpi.l  #$DEADBEEF, D6
        bne     fail
        stop    #0
fail:   stop    #$ba98
