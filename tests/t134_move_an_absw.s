; K1.3 cosim caught this at r=525688: MOVE.L A2, $002C.W clobbers A2.
; In our CPU, after this instruction A2 became 0; in Musashi A2 stayed at
; $FC05B4.  K1.3 uses this exact form at $FC0588 to install exception
; vectors (`MOVE.L A2, $002C.W` plants the LINEF emulator entry).
;
; Test: load A2 with a known sentinel, do MOVE.L A2, abs.W, verify A2
; still holds the sentinel afterwards AND that memory got the value too.
; The A0 form right before us ($FC0584 MOVE.L A0, $0010.W) was reported
; as NOT clobbering A0 -- include both forms and check.
;
; Pass = halt 0.  Failures:
;   $BAD1 = memory at $1010 didn't receive A0's value
;   $BAD2 = A0 got clobbered by MOVE.L A0, $1010.W
;   $BAD3 = memory at $102C didn't receive A2's value
;   $BAD4 = A2 got clobbered by MOVE.L A2, $102C.W
;   $BAD5 = A3 got clobbered (regression net)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Load distinct sentinels into A0/A2/A3.
        move.l  #$CAFE0010, A0
        move.l  #$FC05B4, A2          ; K1.3's actual value at the divergence
        move.l  #$DEAD0030, A3

        ; Clear destination cells so we'd notice a missed store.
        move.l  #$00000000, $1010.w
        move.l  #$00000000, $102c.w

        ; The exact K1.3 idiom -- MOVE.L An, abs.W into the exception-
        ; vector table area (here using $1010/$102C instead of $10/$2C
        ; to stay out of the test runner's vector table).
        move.l  A0, $1010.w
        move.l  A2, $102c.w

        ; Verify stores landed.
        move.l  $1010.w, D0
        cmpi.l  #$CAFE0010, D0
        bne     fail1
        move.l  $102c.w, D0
        cmpi.l  #$FC05B4, D0
        bne     fail3

        ; Verify the source registers weren't clobbered.
        cmpa.l  #$CAFE0010, A0
        bne     fail2
        cmpa.l  #$FC05B4, A2
        bne     fail4
        cmpa.l  #$DEAD0030, A3
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
