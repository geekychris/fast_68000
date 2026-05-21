; MOVEM.L reglist, d8(An, Xn.W) -- reg-to-mem with d8(An,Xn) destination.
;
; Surfaced in Kickstart 1.3 boot at $F817C0:
;   MOVEM.L D1/A1, $54(A6, D3.W)
;
; Bug had two parts:
;   1. MOVEM init's case statement only handled (An), (An)+, -(An) and
;      d16(An) as source modes.  d8(An,Xn) fell through to the default
;      branch which used `ex_ra` (= An) and ignored both the d8
;      displacement AND the index register.
;   2. The third regfile read port (rc) is repurposed during ID to read
;      the EA_IDX brief extension's Xn register, but the routing logic
;      was indexing `id_ext[0]` to find the brief extension.  For MOVEM
;      `id_ext[0]` is the register mask -- the brief extension lives at
;      `id_ext[1]`.  So ex_sp held a wrong-register value (the one whose
;      index matched the mask word's high nibble) rather than the
;      intended Xn.
;
; Together the two bugs collapsed `$54(A6, D3.W)` to plain `A6` and
; IntVects[3].iv_Data was never set, so cia.resource's AddIntServer
; later enqueued onto a NULL list head and crashed the boot.
;
; Test: store D1=$11111111 / A1=$22222222 to $40(A0, D3.W) with
; A0=$2000 and D3=$10.  Verify writes land at $2050/$2054 and nothing
; else moves.
;
; Pass = halt 0.  Failures:
;   $BAD1 = $2050 wrong (D1 not stored)
;   $BAD2 = $2054 wrong (A1 not stored)
;   $BAD3 = neighbour $204C clobbered
;   $BAD4 = neighbour $2058 clobbered
;   $BAD5 = mem-to-reg readback (D2) wrong
;   $BAD6 = mem-to-reg readback (A2) wrong

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Pre-fill memory with sentinels.
        move.l  #$DEADBEE0, $0000204C
        move.l  #$CAFEBAB0, $00002050
        move.l  #$CAFEBAB1, $00002054
        move.l  #$DEADBEE1, $00002058

        ; Set up regs.
        lea     $00002000, A0
        move.l  #$00000010, D3
        move.l  #$11111111, D1
        move.l  #$22222222, A1

        ; MOVEM.L D1/A1, $40(A0, D3.W)
        ; Expected EA = A0 + D3 + $40 = $2000 + $10 + $40 = $2050.
        movem.l D1/A1, $40(A0, D3.W)

        ; Check writes.
        cmpi.l  #$11111111, $00002050
        bne     fail1
        cmpi.l  #$22222222, $00002054
        bne     fail2

        ; Check neighbours untouched.
        cmpi.l  #$DEADBEE0, $0000204C
        bne     fail3
        cmpi.l  #$DEADBEE1, $00002058
        bne     fail4

        ; Mem-to-reg direction.  Pre-fill D2/A2 with junk, then
        ; MOVEM.L $40(A0, D3.W), D2/A2 should reload them from $2050/$2054.
        move.l  #$DEAD0000, D2
        move.l  #$DEAD0001, A2

        movem.l $40(A0, D3.W), D2/A2

        cmpi.l  #$11111111, D2
        bne     fail5

        move.l  A2, D0
        cmpi.l  #$22222222, D0
        bne     fail6

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
fail6:  stop    #$BAD6
