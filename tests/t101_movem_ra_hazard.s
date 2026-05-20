; MOVEM -(A7) ra-port leakage hazard.
;
; The CPU multiplexes regfile read port A between the ID stage's source
; register and MOVEM's "current reg being pushed" while MOVEM is mid-flight.
; If the mux still points at MOVEM's last reg on the cycle MOVEM retires,
; the immediately-following instruction's ex_ra latches the wrong value.
;
; This was caught by Kickstart 1.3's AllocMem:
;   MOVEM.L D0/D1, -(A7)
;   BSR.S   inner
; inner:
;   MOVEM.L D2/D3, -(A7)
;   MOVE.L  D0, D3      <-- reads 0 instead of original D0
; Since D0 held the requested allocation size, the inner function thought
; size==0, took the early-return path, and returned with D0 unchanged --
; so the caller treated the size value (18) as the returned pointer.
;
; This test reproduces the hazard with a tight MOVEM-then-MOVE sequence
; and verifies D3 sees D0's value.  Pass = halt 0.  Fail = stop $BAD1.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Seed registers with distinct values so the hazard is visible.
        move.l  #$12345678, D0
        move.l  #$00000000, D3        ; ensure D3 starts clean
        move.l  #$00000000, D2

        ; The bug trigger: MOVEM.L D2/D3, -(A7) immediately followed by
        ; a MOVE.L Dn, Dm whose source is not in the MOVEM mask.
        movem.l D2/D3, -(A7)
        move.l  D0, D3

        cmpi.l  #$12345678, D3
        bne     fail

        ; Sanity: same pattern from a BSR (matches the AllocMem chain --
        ; outer MOVEM, BSR, inner MOVEM, then MOVE.L Dn).
        movem.l D0/D1, -(A7)
        bsr     inner
        addq.l  #8, A7

        cmpi.l  #$12345678, D3
        bne     fail

        stop    #0

inner:
        movem.l D2/D3, -(A7)
        move.l  D0, D3
        cmpi.l  #$12345678, D3
        bne     fail
        addq.l  #8, A7
        rts

fail:   stop    #$BAD1
forever: bra forever
