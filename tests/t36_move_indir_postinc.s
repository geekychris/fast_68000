        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        move.l  #$DEADBEEF, $00003000
        move.l  #$AAAAAAAA, $00003010
        move.l  $00003000, $00003010
        move.l  $00003010, D1
        cmpi.l  #$DEADBEEF, D1
        beq     ok_l
        cmpi.l  #$AAAAAAAA, D1
        beq     was_aaa
        cmpi.l  #0, D1
        beq     was_zero
        cmpi.l  #$00003000, D1
        beq     was_3000
        cmpi.l  #$00003010, D1
        beq     was_3010
        cmpi.l  #$DEADBEEF, D1
        bne     fail_other
ok_l:
        stop    #0

was_aaa:    stop #$F6AA
was_zero:   stop #$F60E
was_3000:   stop #$F630
was_3010:   stop #$F631
fail_other: stop #$F6FF
