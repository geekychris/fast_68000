; Smoke test: addq.l #imm, <abs.l>.
;
; If addq with absolute-long destination isn't decoded, the memory
; location stays 0 and the test halts $BA01.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        move.l  #0, $00010000
        addq.l  #1, $00010000
        addq.l  #1, $00010000
        addq.l  #1, $00010000
        move.l  $00010000, D0
        cmpi.l  #3, D0
        bne     fail
        stop    #0
fail:   stop    #$ba01
