; CLR.L -(An) must commit the An predec to the regfile.
;
; The S_RUN K_CLR handler set wb_aux_we_c for the An update, but
; wb_aux_we only propagates to the regfile when is_settled fires --
; and is_settled doesn't fire in S_RUN for memory-destination CLR
; (it transitions to S_STORE first).  S_STORE K_CLR didn't replay
; the wb_aux assignment, so the An predec was silently dropped.
;
; Net effect: two back-to-back CLR.L -(SP) both wrote to the same
; address, and subsequent pushes saw a stale SP.  Surfaced in
; Kickstart 1.3's MakeLibrary call setup at $FCABD6 where two
; CLR.L -(SP) zero the (size, init_func) slots before the JSR -- our
; CPU collapsed both onto one slot and the trailing PEAs ended up
; one slot too high, so MakeLibrary read garbage args and JSR'd
; through a $01B8 ROM-offset that flew PC into chip-RAM low memory.
;
; Test:
;   1. PEA two distinguishing values onto stack so we know what
;      should be at each slot.
;   2. Do two consecutive CLR.L -(SP).
;   3. Verify the four longs at the expected SP positions are the
;      pushed PEA values + the two zeros, not collapsed.
;
; Pass = halt 0.  Failures:
;   $BAD1 = SP didn't decrement by 8 across two CLR.Ls
;   $BAD2 = first CLR.L slot (top of stack after two CLRs) not zero
;   $BAD3 = second CLR.L slot (SP+4) not zero
;   $BAD4 = first PEA value clobbered (CLR overwrote it)
;   $BAD5 = second PEA value clobbered

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Position A7 at a known location so we can verify.
        move.l  #$00003000, A7
        ; Pre-fill the stack region so we can see if writes happen.
        move.l  #$11111111, $00002FFC
        move.l  #$22222222, $00002FF8
        move.l  #$33333333, $00002FF4
        move.l  #$44444444, $00002FF0

        ; Push two distinguishing PEAs first.
        pea     $AAAA5555
        pea     $5555AAAA
        ; A7 should now be $00002FF8 ($3000 - 4 - 4).

        ; Two consecutive CLR.L -(SP).
        clr.l   -(A7)
        clr.l   -(A7)
        ; A7 should now be $00002FF0.

        ; Verify A7 = $00002FF0.
        move.l  A7, D0
        cmpi.l  #$00002FF0, D0
        bne     fail1

        ; Verify the zeros: $00002FF0 and $00002FF4.
        move.l  $00002FF0, D0
        bne     fail2
        move.l  $00002FF4, D0
        bne     fail3

        ; Verify PEA values still at $00002FF8 and $00002FFC.
        move.l  $00002FF8, D0
        cmpi.l  #$5555AAAA, D0
        bne     fail4
        move.l  $00002FFC, D0
        cmpi.l  #$AAAA5555, D0
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
