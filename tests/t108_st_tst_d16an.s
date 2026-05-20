; ST $d16(An) + TST.B $d16(An) -- the fail-flag pattern in
; Kickstart 1.3's library-create wrapper at $F8086A / $F808A4.
;
; If ST writes the wrong byte or TST reads the wrong byte (or the
; same memory location is interpreted differently between the two),
; Kickstart's "MakeLibrary failed" check falls through to a null-
; pointer JSR.
;
; Pass = halt 0.  Failures:
;   $BAD1 = ST didn't set the byte to $FF
;   $BAD2 = neighbouring bytes got clobbered
;   $BAD3 = TST.B saw zero after ST
;   $BAD4 = CLR.B didn't zero the byte
;   $BAD5 = TST.B saw non-zero after CLR

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        lea     $00002000, A2

        ; Pre-fill the struct slot at $1F(A2) and its neighbours
        ; with distinct sentinels.
        move.l  #$AABBCCDD, $00001FFC      ; below
        move.l  #$11223344, $00002000      ; word at $2000
        move.l  #$55667788, $00002004
        move.l  #$99AABBCC, $0000201C      ; longword that contains $1F
        move.l  #$DDEEFF00, $00002020      ; just above

        ; ---- ST $1F(A2) ----  (= byte $1F = mem[$201F])
        st      $1F(A2)

        ; Read back the byte at $201F and verify it's $FF.
        move.b  $0000201F, D1
        andi.l  #$FF, D1
        cmpi.l  #$FF, D1
        bne     fail_st

        ; Neighbours should be intact except $201F.
        move.l  $0000201C, D2
        andi.l  #$FFFFFF00, D2
        cmpi.l  #$99AABB00, D2
        bne     fail_neighbour
        move.l  $00002020, D3
        cmpi.l  #$DDEEFF00, D3
        bne     fail_neighbour

        ; ---- TST.B $1F(A2) with byte = $FF ----  should NOT set Z.
        tst.b   $1F(A2)
        beq     fail_tst_set        ; Z set means TST saw zero

        ; ---- CLR.B $1F(A2) ----  should zero just that byte.
        clr.b   $1F(A2)
        move.b  $0000201F, D4
        andi.l  #$FF, D4
        cmpi.l  #$00, D4
        bne     fail_clr

        ; ---- TST.B $1F(A2) with byte = 0 ----  should set Z.
        tst.b   $1F(A2)
        bne     fail_tst_clr

        stop    #0
forever:
        bra     forever
fail_st:        stop #$BAD1
fail_neighbour: stop #$BAD2
fail_tst_set:   stop #$BAD3
fail_clr:       stop #$BAD4
fail_tst_clr:   stop #$BAD5
