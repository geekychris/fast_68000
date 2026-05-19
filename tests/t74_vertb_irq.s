; VERTB interrupt wiring test.
;
; Agnus's beam counter wraps every 227*312 = 70824 host clocks; on the
; cycle (h=last,v=last) the bus drives vblank_pulse_o high, which feeds
; Paula's vblank_int_i.  Paula edge-detects and sets INTREQ bit 5 (VERTB).
; With INTEN+VERTB enabled in INTENA, IPL rises to 3 and the CPU
; dispatches INT3.
;
; The handler counts entries in D7; the main code waits up to a frame's
; worth of cycles and verifies D7 >= 1.

        .org $6C                          ; vector 27 = INT3 (level 3)
        .long int3_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Lower the CPU IRQ mask.
        andi.w  #$F8FF, SR

        ; Arm: INTENA = INTEN(bit14) | VERTB(bit5) -> $C020 SET.
        move.w  #$C020, $00FE029A

        ; Wait for the VBLANK pulse.  Each frame is ~71k clocks; a busy
        ; loop of 80000 iters dwarfs that.  D7 starts 0 and the handler
        ; raises it to $51.  If D7 stays 0, the IRQ never fired.
        moveq   #0, D7
        move.l  #80000, D0
wait:   nop
        subq.l  #1, D0
        bne     wait

        cmpi.l  #$51, D7
        bne     fail
        stop    #0

int3_handler:
        ; Clear VERTB pending so we don't re-enter on the next clock.
        move.w  #$0020, $00FE029C         ; INTREQ CLR bit 5
        move.l  #$51, D7
        rte

fail:   stop    #$be51
