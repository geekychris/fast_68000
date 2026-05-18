; OVL low-memory ROM remap test.
;
; Requires the bus to be built with OVL_RESET=1 and rom_test.hex loaded
; into the ROM (`make test-ovl`).  At reset OVL=1, so reads at $0-$7FFFF
; come from the ROM (rom_test.hex starts with $DEADBEEF $CAFEBABE
; $12345678).  After CIA-A drives /OVL low, low reads come from RAM.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; While OVL is active, $00000000 reads from rom[0] = $DEADBEEF.
        move.l  $00000000, D0
        cmpi.l  #$DEADBEEF, D0
        bne     fail_1
        move.l  $00000004, D0
        cmpi.l  #$CAFEBABE, D0
        bne     fail_2

        ; Also $F80000 maps to rom[0] (canonical direct ROM read).
        move.l  $00F80000, D0
        cmpi.l  #$DEADBEEF, D0
        bne     fail_3

        ; Set a sentinel in low RAM before disabling OVL.  Writes are NOT
        ; affected by OVL; they always go to RAM.
        move.l  #$AABB_CCDD, $00000000
        move.l  #$1122_3344, $00000004

        ; Clear OVL: CIA-A DDRA bit 0 = 1 (output), PRA bit 0 = 0 (low).
        ; Use canonical CIA-A addresses.  DDRA = register 2 at $BFE201.
        move.b  $00BFE001, D0          ; read current PRA
        andi.b  #$FE, D0               ; clear bit 0
        move.b  D0, $00BFE001          ; write back: PRA bit 0 = 0
        move.b  #$01, $00BFE201        ; DDRA bit 0 = 1 (output)
        ; The bus latches /OVL cleared on the next ovl_clr_i pulse.

        ; A few NOPs to let the latch propagate.
        nop
        nop
        nop

        ; Now $0 should read main RAM (the sentinel we wrote earlier).
        move.l  $00000000, D0
        cmpi.l  #$AABB_CCDD, D0
        bne     fail_4
        move.l  $00000004, D0
        cmpi.l  #$1122_3344, D0
        bne     fail_5

        ; Direct ROM reads at $F80000 are unaffected by OVL.
        move.l  $00F80000, D0
        cmpi.l  #$DEADBEEF, D0
        bne     fail_6

        stop    #0

fail_1: stop #$DF01
fail_2: stop #$DF02
fail_3: stop #$DF03
fail_4: stop #$DF04
fail_5: stop #$DF05
fail_6: stop #$DF06
