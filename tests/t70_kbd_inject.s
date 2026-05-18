; Keyboard byte injection via $00FE9000 -> CIA-A SDR + ICR bit 3.
;
; Sequence:
;   1. Inject scan code $42 via the magic address.
;   2. A few NOPs so the byte propagates to CIA-A.
;   3. Read CIA-A ICR (at $00BFED01).  Bit 7 = IR (any pending),
;      bit 3 = SP source.  Also the read CLEARS pending.
;   4. Confirm bit 3 was set.
;   5. Read CIA-A SDR at $00BFEC01.  Should equal $42.
;   6. Halt 0.
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0
core0:
        ; Inject the scan code.
        move.b  #$42, $00FE9000

        ; Burn a few cycles for the byte to land.
        nop
        nop
        nop
        nop

        ; Read ICR — must have bit 3 set (SP source pending).
        move.b  $00BFED01, D0
        andi.l  #$00000008, D0
        beq     fail_no_int

        ; Read SDR — should be the injected $42.
        move.b  $00BFEC01, D1
        andi.l  #$000000FF, D1
        cmpi.l  #$42, D1
        bne     fail_sdr

        stop    #0
forever: bra forever
fail_no_int: stop #$BAD1
fail_sdr:    stop #$BAD2
