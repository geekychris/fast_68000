; t134 passed but the cosim divergence at r=525688 shows MOVE.L A2,$002C.W
; clobbering A2.  The difference might be that $002C is in the exception
; vector table (low memory) vs t134's $102C in regular chip RAM, or it
; might be context-specific.
;
; Test: replicate the K1.3 $FC0584/$FC0588 sequence exactly, writing
; vector $10 and $2C with arbitrary register values.  Verify An stays
; intact.  Run in supervisor mode (the test runner is already supervisor)
; so writing to $10/$2C should be allowed.
;
; Pass = halt 0.  Failures:
;   $BAD1 = vector $10 didn't receive A0's value
;   $BAD2 = A0 clobbered by MOVE.L A0, $0010.W
;   $BAD3 = vector $2C didn't receive A2's value
;   $BAD4 = A2 clobbered by MOVE.L A2, $002C.W
;   $BAD5 = A3 clobbered (control)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Save the vectors we're about to clobber so we don't crash later.
        move.l  $0010.w, D1
        move.l  $002c.w, D2

        ; K1.3's actual values at the cosim divergence point.
        move.l  #$FC05B4, A0
        move.l  #$FC05B4, A2
        move.l  #$FEED0030, A3

        ; The exact K1.3 idiom at $FC0584 / $FC0588.
        move.l  A0, $0010.w
        move.l  A2, $002c.w

        ; Verify stores landed.
        move.l  $0010.w, D0
        cmpi.l  #$FC05B4, D0
        bne     fail1
        move.l  $002c.w, D0
        cmpi.l  #$FC05B4, D0
        bne     fail3

        ; Verify the source registers weren't clobbered.
        cmpa.l  #$FC05B4, A0
        bne     fail2
        cmpa.l  #$FC05B4, A2
        bne     fail4
        cmpa.l  #$FEED0030, A3
        bne     fail5

        ; Restore the original vectors.
        move.l  D1, $0010.w
        move.l  D2, $002c.w

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
