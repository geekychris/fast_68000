; Chipset 16-bit register half-positioning.
;
; Background: every chipset MOVE.W <reg>, Dn comes through the 32-bit bus
; as a long-aligned read.  The CPU's MOVE.W extraction takes the high
; half of the response when reg-addr[1]==0 and the low half when
; reg-addr[1]==1.  Earlier the bus put the 16-bit value in the low half
; unconditionally, so MOVE.W from an even-word chipset register (SERDATR,
; ADKCONR, VPOSR, JOY1DAT) silently read $0000.  This test catches the
; regression by reading SERDATR (addr[1]=0) and POTGOR (addr[1]=1) and
; verifying both return their canned values.
;
; SERDATR = $607F (TSRE | TBE | self-test PASS low byte).
; POTGOR  = $FFFF (no buttons pressed, pull-ups high).
;
; Pass = halt 0.  Failures: $BAD1 SERDATR mismatch, $BAD2 POTGOR mismatch.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; 1. MOVE.W from $DFF018 (SERDATR, addr[1]=0).  Without the
        ;    half-positioning fix this returns the high half of the bus
        ;    response which used to be 0.
        move.w  $00DFF018, D0
        andi.l  #$FFFF, D0
        cmpi.l  #$607F, D0
        bne     fail_serdatr

        ; 2. MOVE.W from $DFF016 (POTGOR, addr[1]=1).  Always worked
        ;    because the value used to live in the low half by default;
        ;    keep it as a sanity check that the odd-word path still
        ;    works after the fix.
        move.w  $00DFF016, D1
        andi.l  #$FFFF, D1
        cmpi.l  #$FFFF, D1
        bne     fail_potgor

        stop    #0
forever:
        bra     forever
fail_serdatr: stop #$BAD1
fail_potgor:  stop #$BAD2
