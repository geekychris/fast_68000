; t201 -- canonical Copper MOVE smoke test.
;
; Writes a 2-word Copper list to chip RAM that MOVEs $0F00 into
; COLOR00 ($DFF180), terminated by the canonical end-of-list marker
; (WAIT VP=$FF).  Strobes COPJMP1 and verifies Denise's COLOR00 slot
; reflects the write.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Copper list at $00006000:
        ;   .L $01800F00   -> MOVE COLOR00 ($DFF180) <- $0F00
        ;   .L $FFFFFFFE   -> WAIT VP=$FF (end-of-list)
        move.l #$01800F00, $00006000
        move.l #$FFFFFFFE, $00006004

        ; Point COP1LC at the list and strobe COPJMP1.
        move.l #$00006000, $00FE0040
        move.l #1,         $00FE0044

wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

        ; Verify: long read of $DFF180 returns {IR2, 16'd0} because the
        ; Copper writes the high half of the long (be=4'b1100).
        move.l $00DFF180, D1
        cmpi.l #$0F000000, D1
        bne    fail1

        stop   #0

fail1:  stop   #$c201
