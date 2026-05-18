; Paula INTREQ / INTENA interrupt controller test.
;
; Verifies:
;   * INTENA write SET/CLR semantics (bit 15 of value = SET, else CLR)
;   * INTREQ write SET/CLR semantics
;   * INTREQR / INTENAR read mirrors at $1C / $1E
;   * Priority-encoded IPL: setting BLIT (bit 6) + INTEN (bit 14) raises
;     irq_level to 3, the CPU autovector-dispatches via vector 27 (INT3),
;     and the handler clears the source.
;
; Paula register window in the local layout is $00FE_0200..$00FE_02FF.

        .org $6C                          ; vector 27 = INT3 (level 3)
        .long int3_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Lower the CPU IRQ mask so anything >= level 1 is accepted.
        ; SR bits 10:8 = I[2:0].  Clear them.
        andi.w  #$F8FF, SR

        ; ---- Test 1: INTENA SET/CLR + INTENAR read ----
        ; Enable INTEN (bit 14) by writing $C000 to INTENA (bit15=1 SET, bit14).
        move.w  #$C000, $00FE029A         ; INTENA SET bit 14 (INTEN)
        ; Read .L at $1C = {INTENAR, INTREQR}.
        move.l  $00FE021C, D0
        andi.l  #$FFFF0000, D0
        cmpi.l  #$40000000, D0            ; only bit 14 set in upper half
        bne     fail_1

        ; Clear INTEN.
        move.w  #$4000, $00FE029A         ; CLR bit 14
        move.l  $00FE021C, D0             ; INTENAR : INTREQR
        andi.l  #$40000000, D0
        bne     fail_2

        ; ---- Test 2: INTREQ SET/CLR + INTREQR read ----
        ; Set bit 6 (BLIT) in INTREQ.
        move.w  #$C040, $00FE029C         ; INTREQ SET bit 6
        move.l  $00FE021C, D0             ; INTENAR : INTREQR
        andi.l  #$00000040, D0
        cmpi.l  #$00000040, D0
        bne     fail_3
        ; Clear bit 6.
        move.w  #$0040, $00FE029C         ; INTREQ CLR bit 6
        move.l  $00FE021C, D0             ; INTENAR : INTREQR
        andi.l  #$00000040, D0
        bne     fail_4

        ; ---- Test 3: priority-encoded IPL fires INT3 ----
        moveq   #0, D7
        ; Enable BLIT (bit 6) in INTENA, and master enable INTEN (bit 14).
        move.w  #$C040, $00FE029A         ; INTENA SET bits 14 + 6
        ; Trigger BLIT pending.  IPL should rise to 3 → INT3 vector at $6C.
        move.w  #$C040, $00FE029C         ; INTREQ SET bit 6
        ; Give the CPU a few cycles to dispatch.
        nop
        nop
        nop
        nop
        cmpi.l  #$BB, D7
        bne     fail_5

        stop    #0

int3_handler:
        ; Mark we got here and clear the pending bit so we don't re-enter.
        move.l  #$BB, D7
        move.w  #$0040, $00FE029C         ; INTREQ CLR bit 6
        rte

fail_1: stop #$DC11
fail_2: stop #$DC12
fail_3: stop #$DC13
fail_4: stop #$DC14
fail_5: stop #$DC15
