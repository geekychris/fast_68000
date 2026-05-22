; MOVE between two d8(An, Xn) effective addresses with different
; index registers.  Our core has 3 regfile read ports (ra, rb, rc),
; with rc muxed to whichever EA is using EA_IDX.  When BOTH src
; and dst use EA_IDX with different Xn registers, only the src
; xreg goes through rc -- the dst EA then used the src xreg too,
; computing the wrong destination address.
;
; Surfaced by the K1.3 boot wall at r=1313466.  Code at $FDB956 is
;   MOVE.W $1C(A2, D1.L), $00(A0, D0.L)
; -- src uses D1 as index, dst uses D0.  Our core silently used D1
; for the dst calc too, writing the source word to A0+D1 instead
; of A0+D0.  That landed on the privilege-violation vector at $20,
; corrupting the Exec.Supervisor() handler pointer.
;
; Pass = halt 0.  Failures:
;   $BAD1 = dst index reg used was src's, not dst's (the K1.3 case)
;   $BAD2 = src index reg got corrupted by dst's xreg

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- The K1.3 case: MOVE.W d8(A2, D1.L), d8(A0, D0.L) ---
        ;   D1 = src xreg = $40
        ;   D0 = dst xreg = $80
        ;   A2 = src An base = $1000
        ;   A0 = dst An base = $2000
        ;   src EA = $1000 + $40 + $0 (disp) = $1040
        ;   dst EA = $2000 + $80 + $0 (disp) = $2080
        ; Prime memory at $1040 with $CAFE so the move-target byte
        ; pattern is unambiguous.
        move.l  #$00001000, A2
        move.l  #$00002000, A0
        move.l  #$00000040, D1
        move.l  #$00000080, D0
        move.w  #$CAFE, $1040.w           ; source word at $1040

        ; Pre-fill *both* potential destinations so a wrong-EA write
        ; shows up as a clobbered sentinel rather than a quiet success.
        move.w  #$1111, $2080.w           ; correct dst = $2080
        move.w  #$2222, $2040.w           ; would-be wrong dst = $2040
                                          ; (= A0 + D1 = $2000+$40)

        ; MOVE.W d8(A2, D1.L), d8(A0, D0.L) -- src index reg = D1,
        ; dst index reg = D0.  Both EAs use mode 6.
        move.w  0(A2, D1.L), 0(A0, D0.L)

        ; Verify $2080 holds $CAFE (the just-written source).
        move.w  $2080.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000CAFE, D2
        bne     fail1

        ; Verify $2040 still holds $2222 (the would-be wrong dst).
        move.w  $2040.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00002222, D2
        bne     fail1

        ; Source $1040 should also still hold $CAFE.
        move.w  $1040.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000CAFE, D2
        bne     fail2

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
