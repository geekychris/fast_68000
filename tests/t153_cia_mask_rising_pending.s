; CIA ICR mask-rising-edge with pending already set must fire int_o.
;
; Reproduces the K1.3 keyboard-self-test race that stalls WB1.3 boot:
;   - The keyboard delivers a scancode via SP BEFORE exec writes the
;     ICR mask to enable bit 3 (SP).  This sets icr_pending[3] while
;     icr_mask[3] is still 0, so the old per-source rising-edge logic
;     (int_o only fires on pending 0->1) latches no edge.
;   - Then exec writes #$88 (set mask bit 3) to ICR.
;   - The corrected logic also fires on mask 0->1 while pending is
;     already 1.  Without it, int_o stays low forever and L2 never
;     fires, so AddICRVector for the keyboard handler never runs.
;
; This test sets up exactly that ordering and asserts the handler runs.

        .org $68                        ; vector 26 = IRQ-2 autovector
        .long kbd_handler

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        move.l  #0, $00030000           ; handler counter
        move.l  #0, $00030004           ; received byte

        andi.w  #$F8FF, SR              ; unmask CPU IRQs

        ; Paula INTENA: INTEN (14) + PORTS (3) — IRQ delivery enabled.
        move.w  #$C008, $00DFF09A

        ; CIA-A mask is 0 at reset.  Inject the byte FIRST so that
        ; icr_pending[3] becomes set while the mask is still 0.
        ; (No interrupt should fire on this edge — mask gates it.)
        move.b  #$5A, $00FE9000

        ; Burn a few cycles so the pending bit settles in CIA.
        moveq   #20, D3
settle:
        subq.l  #1, D3
        bne     settle

        ; Confirm no IRQ has been serviced yet (mask still 0).
        move.l  $00030000, D0
        cmpi.l  #0, D0
        bne     fail_early_irq

        ; NOW enable SP mask.  This is the mask 0->1 rising edge.  With
        ; the fix, int_o fires this same cycle because pending[3]==1.
        move.b  #$88, $00BFED01

        ; Spin until handler runs (or timeout).
        moveq   #0, D3
wait1:
        addq.l  #1, D3
        cmpi.l  #$10000, D3
        bge     fail_no_irq
        move.l  $00030000, D0
        cmpi.l  #1, D0
        bne     wait1

        ; Verify the received byte.
        move.l  $00030004, D0
        cmpi.l  #$5A, D0
        bne     fail_byte

        stop    #0

fail_early_irq: stop #$D701
fail_no_irq:    stop #$D702
fail_byte:      stop #$D703

; ---- IRQ 2 (PORTS) handler ----
kbd_handler:
        move.l  D0, -(A7)
        move.l  D1, -(A7)

        ; Read ICR to clear pending bits.
        move.b  $00BFED01, D0

        ; Read scancode from SDR.
        move.b  $00BFEC01, D1
        andi.l  #$FF, D1

        addq.l  #1, $00030000
        move.l  D1, $00030004

        ; Clear INTREQ.PORTS.
        move.w  #$0008, $00DFF09C

        move.l  (A7)+, D1
        move.l  (A7)+, D0
        rte
