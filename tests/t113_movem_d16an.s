; MOVEM.L $d16(An), reglist -- mem-to-reg with a d16 displacement.
;
; The CPU's MOVEM init at $F81276 was loading movem_addr from `ex_ra`
; directly (= An), ignoring the d16 displacement.  So
; `MOVEM.L $0090(A6), A1/A5` was reading from A6 + 0 / A6 + 4 instead
; of A6 + $90 / A6 + $94.
;
; Surfaced in Kickstart 1.3 boot: exec uses a per-library init dispatch
; table at ExecBase + $84..$A3 (pairs of (lib_base, init_fn)).  The
; broken MOVEM read ExecBase[0..7] instead of the target slot --
; A5 ended up pointing at garbage (chip-RAM $6378), JMP (A5) landed
; on a $4D14 opcode (invalid on 68000), ILLEGAL trap fired, boot
; trapped to Alert($80000008 = AN_BogusExcpt).
;
; Test: lay out a 16-byte source struct at $2080 with sentinel
; values, MOVEM.L $80(A0), D1/D3/A2/A5 into the register list, verify
; each register got its corresponding slot.  Also verifies that A0
; itself is NOT updated (d16(An) is a control mode, not predec/
; postinc).
;
; Pass = halt 0.  Failures:
;   $BAD1 = D1 wrong
;   $BAD2 = D3 wrong
;   $BAD3 = A2 wrong
;   $BAD4 = A5 wrong
;   $BAD5 = A0 was modified (shouldn't be)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Base pointer.  $80(A0) = $2080.
        lea     $00002000, A0

        ; Lay down sentinels at $2080..$208F.
        move.l  #$11111111, $00002080      ; -> D1
        move.l  #$33333333, $00002084      ; -> D3
        move.l  #$AAAAAAAA, $00002088      ; -> A2
        move.l  #$55555555, $0000208C      ; -> A5

        ; Pre-fill registers with bogus values so the test catches
        ; "did MOVEM write at all".
        move.l  #$DEAD0000, D1
        move.l  #$DEAD0001, D3
        move.l  #$DEAD0002, A2
        move.l  #$DEAD0003, A5

        ; MOVEM.L $80(A0), D1/D3/A2/A5
        movem.l $80(A0), D1/D3/A2/A5

        cmpi.l  #$11111111, D1
        bne     fail1
        cmpi.l  #$33333333, D3
        bne     fail2

        move.l  A2, D0
        cmpi.l  #$AAAAAAAA, D0
        bne     fail3

        move.l  A5, D0
        cmpi.l  #$55555555, D0
        bne     fail4

        move.l  A0, D0
        cmpi.l  #$00002000, D0
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
