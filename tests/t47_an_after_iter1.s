; After a single move.l (A0)+, (A1)+, verify A0 += 4, A1 += 4, and mem
; written.  This is the building block of a loop.
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        move.l  #$00005000, A0
        move.l  #$00006000, A1
        ; Pre-seed mem[$5000] = $11111111
        move.l  #$11111111, $00005000
        move.l  #$DEADBEEF, $00005004

        move.l  (A0)+, (A1)+

        ; Check A0 advanced by 4.
        move.l  A0, D2
        cmpi.l  #$00005004, D2
        bne     fail_a0

        ; Check A1 advanced by 4.
        move.l  A1, D2
        cmpi.l  #$00006004, D2
        bne     fail_a1

        ; Check mem[$6000] has $11111111.
        move.l  $00006000, D2
        cmpi.l  #$11111111, D2
        bne     fail_m

        stop    #0
fail_a0: stop #$F7A0
fail_a1: stop #$F7A1
fail_m:  stop #$F7D0
