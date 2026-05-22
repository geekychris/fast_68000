; PEA d8(An, Xn): push must go to (A7-4) and update A7 -= 4, not via Xn.
;
; The K_PEA execution path computed `want_addr = ex_sp - 4` and
; `wb_aux_data = ex_sp - 4`.  For EA_IDX (`d8(An, Xn)`) the rc port is
; muxed to Xn, so ex_sp held the *index register value*, not A7.  PEA
; then pushed to (Xn - 4) and set A7 = Xn - 4 -- if Xn happened to be 0
; A7 became $FFFFFFFC, and every subsequent push/RTS walked off into
; unmapped memory.  Surfaced in K1.3 at $FD6620:
;   PEA   $0(A2, D2.L)
;   JSR   $00FE0358
; -- when D2 had just been cleared, A7 jumped from $1900 to $FFFFFFFC
; and the JSR target's MOVE.L A1, (A1) ended up reading garbage from
; the ROM-end and tripping vec=3.
;
; Test: PEA d8(A1, D0.L) with D0 = 0 and A1 = something specific.
; Verify (a) A7 decremented by exactly 4, (b) the pushed value is the
; EA (A1+D0+d8), (c) prior stack contents are intact, (d) PEA without
; index (d16(An) variant) still works.
;
; Pass = halt 0.  Failures:
;   $BAD1 = A7 not A7-4 after PEA d8(An,Xn) with Xn=0
;   $BAD2 = pushed value is not the EA
;   $BAD3 = prior stack contents corrupted
;   $BAD4 = PEA d16(An) (mode 5) regressed

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Set up a known stack with sentinel above.
        move.l  #$00003000, A7
        move.l  #$DEADBEEF, $00002FFC     ; sentinel just above current SP
        move.l  #$CAFEBABE, $00002FF8     ; another sentinel

        ; Set An / Xn so the EA is computable.
        move.l  #$00004000, A1
        moveq   #0, D0                     ; index = 0
        moveq   #0, D2                     ; another zero index
        move.l  #$11111111, $00002FFC      ; re-prime sentinel
        move.l  #$22222222, $00002FF8

        ; PEA $0(A1, D0.L) -- EA = A1 + D0 = $4000.  Should push $4000
        ; to $00002FFC and set A7 = $00002FFC.
        pea     0(A1, D0.l)

        ; Verify A7 = $00002FFC (A7 decremented by exactly 4).
        move.l  A7, D0
        cmpi.l  #$00002FFC, D0
        bne     fail1

        ; Verify the pushed value at $00002FFC is the EA = $4000.
        move.l  $00002FFC, D0
        cmpi.l  #$00004000, D0
        bne     fail2

        ; Verify the next-lower sentinel at $2FF8 is still intact.
        move.l  $00002FF8, D0
        cmpi.l  #$22222222, D0
        bne     fail3

        ; --- Sanity: PEA d16(An) (mode 5) still works ---
        move.l  #$00003000, A7
        move.l  #$BEEF0001, $00002FFC
        pea     8(A1)                      ; EA = A1 + 8 = $00004008
        move.l  A7, D0
        cmpi.l  #$00002FFC, D0
        bne     fail4
        move.l  $00002FFC, D0
        cmpi.l  #$00004008, D0
        bne     fail4

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
