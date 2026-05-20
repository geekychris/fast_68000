; MOVEA.W (d16,An), An -- word source from memory at the high half
; of a 32-bit aligned longword.
;
; The CPU was using raw dc_rdata[15:0] for K_MOVEA.W mem source, which
; only holds the wanted word when dc_addr[1]==1 (odd-word offset).  For
; addresses with dc_addr[1]==0 (even-word offset, e.g. offsets 0, 4, 8,
; ...) the wanted word lives at dc_rdata[31:16]; the buggy path
; loaded (and sign-extended) the WRONG half, putting garbage into An.
;
; Same shape as the t110 CMP.B bug, but for K_MOVEA's word form.
; MOVEA is everywhere -- pointer derefs through struct slots all
; route through this path -- so this is plausibly the cause of much
; downstream Kickstart-boot chaos.
;
; Pass = halt 0.  Failures:
;   $BAD1 = MOVEA.W at offset 0 (high half of long) returned wrong value
;   $BAD2 = MOVEA.W at offset 2 (low half) returned wrong value
;   $BAD3 = sign extension at offset 0 wrong (negative source)
;   $BAD4 = sign extension at offset 2 wrong (negative source)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        lea     $00002000, A1

        ; Long at $2000 = $7FFF_8001.
        ;   Word at offset 0 (dc_addr=$2000, dc_addr[1]=0): $7FFF (= +32767, positive)
        ;   Word at offset 2 (dc_addr=$2002, dc_addr[1]=1): $8001 (= -32767, negative)
        ; Sign-extended .W loads:
        ;   offset 0 -> A2 = $00007FFF
        ;   offset 2 -> A2 = $FFFF8001
        move.l  #$7FFF8001, $00002000

        ; MOVEA.W $0(A1), A2 -- expect A2 = $00007FFF
        .word $3469, $0000      ; MOVEA.W $0(A1), A2
        move.l  A2, D0
        cmpi.l  #$00007FFF, D0
        bne     fail1

        ; MOVEA.W $2(A1), A2 -- expect A2 = $FFFF8001
        .word $3469, $0002      ; MOVEA.W $2(A1), A2
        move.l  A2, D0
        cmpi.l  #$FFFF8001, D0
        bne     fail2

        ; Swap the long: $8000_7000.  Sign-extension flips.
        ;   offset 0: word $8000 -> A2 = $FFFF8000 (negative)
        ;   offset 2: word $7000 -> A2 = $00007000 (positive)
        move.l  #$80007000, $00002000

        .word $3469, $0000      ; MOVEA.W $0(A1), A2
        move.l  A2, D0
        cmpi.l  #$FFFF8000, D0
        bne     fail3

        .word $3469, $0002      ; MOVEA.W $2(A1), A2
        move.l  A2, D0
        cmpi.l  #$00007000, D0
        bne     fail4

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
