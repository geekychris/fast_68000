; IRQ handler save/restore via -(A7) / (A7)+ in supervisor mode.
;
; Force one BLIT interrupt (we can SET INTREQ[6] from CPU, INTENA gates).
; Handler increments memory counter using a saved register.

        .org $6C                          ; INT3 vector
        .long int3_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        andi.w  #$F8FF, SR

        move.l  #$11111111, D6              ; sentinel in D6

        move.l  #0, $00010000               ; counter

        ; Enable INTEN + BLIT.
        move.w  #$C040, $00FE029A

        ; Trigger BLIT manually by setting INTREQ[6].
        move.w  #$C040, $00FE029C

        ; Give CPU a chance to take the IRQ.
        nop
        nop
        nop
        nop
        nop

        ; D6 must still be $11111111 (handler must save/restore).
        cmpi.l  #$11111111, D6
        bne     fail_d6

        ; Counter must have advanced.
        move.l  $00010000, D0
        tst.l   D0
        beq     fail_no_irq

        stop    #0

fail_d6:     stop #$ba96
fail_no_irq: stop #$ba97

int3_handler:
        move.l  D6, -(A7)
        move.l  $00010000, D6
        addq.l  #1, D6
        move.l  D6, $00010000
        move.w  #$0040, $00FE029C           ; clear INTREQ[6]
        move.l  (A7)+, D6
        rte
