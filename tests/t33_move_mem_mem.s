; Regression for memory-to-memory MOVE.
;
; Without the corresponding CPU fix `move.l (abs).L, (abs).L` (and its
; .W / .B siblings) would do the load but silently corrupt a data
; register instead of storing to memory.

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
        beq     fail_l_aaa
        cmpi.l  #0, D1
        beq     fail_l_zero
        cmpi.l  #$00003000, D1
        beq     fail_l_srcaddr
        cmpi.l  #$00003010, D1
        beq     fail_l_dstaddr
        cmpi.l  #$DEADBEEF, D1     ; once more
        bne     fail_l_other
ok_l:

        move.w  #$1234, $00003020
        move.w  #$5678, $00003022
        move.w  $00003020, $00003022
        move.l  $00003020, D2
        cmpi.l  #$12341234, D2
        bne     fail_w

        move.b  #$AB, $00003030
        move.b  #$CD, $00003031
        move.b  $00003030, $00003031
        move.l  $00003030, D3
        andi.l  #$FFFF0000, D3
        cmpi.l  #$ABAB0000, D3
        bne     fail_b

        stop    #0

fail_l:           stop #$BB01
fail_w:           stop #$BB02
fail_b:           stop #$BB03
fail_l_aaa:       stop #$BBAA
fail_l_zero:      stop #$BB00
fail_l_srcaddr:   stop #$BB30
fail_l_dstaddr:   stop #$BB31
fail_l_other:     stop #$BBFF
