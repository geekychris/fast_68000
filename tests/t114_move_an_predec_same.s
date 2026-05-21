; MOVE.L An, -(An) -- same register as src and dst-predec.
;
; On 68000: stores the PRE-decremented An value (the OLD An, before
; the decrement happens).  Used in standard Amiga NewList:
;
;   $F80E60: MOVE.L A3, $8(A3)   ; mh_TailPred = A3
;   $F80E64: ADDQ.L #4, A3
;   $F80E66: CLR.L (A3)           ; lh_Tail = 0
;   $F80E68: MOVE.L A3, -(A3)     ; lh_Head = OLD A3 = list_head + 4 = &lh_Tail
;
; On 68010+: stores the POST-decremented value (the NEW An).  That
; would write lh_Head = list_head (= &lh_Head itself), which
; breaks the Amiga empty-list convention.  Then Enqueue's
; `MOVE.L (A0), D0; MOVEA.L D0, A0` follows the corrupted lh_Head
; back to the list_head, eventually wrapping into A0=0 and writing
; into vector-table memory at $0004 -- which clobbers ExecBase
; pointer and crashes the boot.
;
; Test: set A0 to $00002000, MOVE.L A0, -(A0).  Verify:
;   mem[$1FFC] = $00002000   (the PRE-decremented value)
;   A0         = $00001FFC   (decremented)
;
; Pass = halt 0.  Failures:
;   $BAD1 = mem written with wrong value (NEW An = 68010+ semantics)
;   $BAD2 = A0 not properly decremented
;   $BAD3 = neighbour clobbered

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Pre-fill memory.
        move.l  #$DEADBEEF, $00001FFC
        move.l  #$CAFEBABE, $00002000
        move.l  #$BAADF00D, $00002004

        ; Set A0, do the predec-self store.
        lea     $00002000, A0

        ; MOVE.L A0, -(A0) opcode: 0010 dst_reg dst_mode src_mode src_reg
        ;   bits[15:12] = 0010 (MOVE.L)
        ;   bits[11:9]  = 000 (dst_reg = A0)
        ;   bits[8:6]   = 100 (dst_mode = predec)
        ;   bits[5:3]   = 001 (src_mode = An direct)
        ;   bits[2:0]   = 000 (src_reg = A0)
        ; = 0010_000_100_001_000 = $2108
        .word   $2108                   ; MOVE.L A0, -(A0)

        ; Check mem[$1FFC] = $00002000 (pre-dec value of A0)
        cmpi.l  #$00002000, $00001FFC
        bne     fail1

        ; Check A0 = $00001FFC (decremented)
        move.l  A0, D0
        cmpi.l  #$00001FFC, D0
        bne     fail2

        ; Check $00002000 was NOT clobbered (still $CAFEBABE)
        cmpi.l  #$CAFEBABE, $00002000
        bne     fail3

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
