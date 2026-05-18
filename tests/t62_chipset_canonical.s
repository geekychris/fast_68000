; Canonical Amiga chipset address aliasing.
;
; Paula (INTENA/INTREQ/audio) and Denise (BPLCONx/COLORxx) are at the same
; offsets in our window as on a real Amiga, so the bus accepts the
; canonical $DFFxxx addresses as direct aliases of $00FE_02xx / $00FE_01xx.
; Verify each chip's canonical and legacy addresses see the same register.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Paula INTENA via canonical $DFF09A; read back via INTENAR mirror
        ; at both the canonical $DFF01C and the legacy $00FE_021C.
        move.w  #$C040, $00DFF09A         ; SET INTEN + BLIT
        move.l  $00DFF01C, D0             ; canonical INTENAR : INTREQR
        andi.l  #$40400000, D0            ; just the bits we set
        cmpi.l  #$40400000, D0
        bne     fail_1
        move.l  $00FE021C, D0             ; legacy: same register
        andi.l  #$40400000, D0
        cmpi.l  #$40400000, D0
        bne     fail_2
        ; CLR via legacy address, read via canonical.
        move.w  #$4040, $00FE029A         ; CLR INTEN + BLIT (legacy)
        move.l  $00DFF01C, D0
        andi.l  #$40400000, D0
        bne     fail_3

        ; Paula AUD0LC at canonical $DFF0A0.
        move.l  #$0001_2340, $00DFF0A0
        move.l  $00FE02A0, D0             ; read back via legacy
        cmpi.l  #$0001_2340, D0
        bne     fail_4

        ; Denise BPLCON0 at canonical $DFF100 maps to our slv_addr $00.
        ; Just verify the bus routing works -- the actual chipset content
        ; depends on Denise internals (which we don't probe here).  A simple
        ; write/read at COLOR00 ($DFF180 -> $00FE_0180) is the cheapest
        ; chip-touching test.
        move.l  #$0000_0F00, $00DFF180     ; COLOR00 = red
        move.l  $00FE_0180, D0             ; same register via legacy alias
        andi.l  #$0FFF, D0
        cmpi.l  #$0F00, D0
        bne     fail_5

        stop    #0

fail_1: stop #$E001
fail_2: stop #$E002
fail_3: stop #$E003
fail_4: stop #$E004
fail_5: stop #$E005
