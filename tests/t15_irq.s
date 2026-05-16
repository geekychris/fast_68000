; Exercise interrupts via the memory-mapped IRQ register at $FFFC.
;
; Writing N (1..7) to $FFFC asserts IPL=N. The CPU takes an autovector
; interrupt at the next instruction boundary; vector = 24 + N.
; The handler must write 0 to $FFFC before RTE to clear the IRQ source,
; or it will re-fire after RTE restores sr_i.
;
        .org $64                ; vector 25 = IRQ at IPL 1
        .long irq_handler

        .org $400

        ;------------------------------------------------------------
        ; Initial state: sr_i = 7 (all masked by reset). Lower the mask
        ; so IRQ level 1 can interrupt us. ANDI to SR with $F8FF clears
        ; the I bits.
        ;------------------------------------------------------------
        andi  #$F8FF, SR           ; sr_i := 0

        ;------------------------------------------------------------
        ; Trigger the IRQ by writing 1 to $FFFC.
        ;------------------------------------------------------------
        move.l #0, D7
        move.l #1, D0
        move.l D0, $FFFC.W         ; assert IPL=1
        ; The interrupt may not be taken until the NEXT instruction
        ; boundary. Execute a few NOPs and then check D7.
        nop
        nop
        nop
        cmpi.l #$0000_0042, D7
        beq   ok1
        stop  #$1001               ; D7 wasn't 0x42 — IRQ didn't fire

ok1:
        ;------------------------------------------------------------
        ; Trigger another IRQ to confirm the path is re-entrant.
        ;------------------------------------------------------------
        move.l #0, D7
        move.l #1, D0
        move.l D0, $FFFC.W
        nop
        nop
        cmpi.l #$0000_0042, D7
        beq   ok2
        stop  #$1002

ok2:
        stop  #0

irq_handler:
        move.l #$0000_0042, D7     ; full 32-bit so no sign extension surprises
        ; Clear the IRQ source before RTE.
        move.l #0, D0
        move.l D0, $FFFC.W
        rte
