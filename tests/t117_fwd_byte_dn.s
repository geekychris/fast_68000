; EX->ID forwarding network must apply size-masking for Dn writebacks.
;
; Background: the regfile's sized_write() preserves the upper bits of
; Dn when wb_size is SZ_B/SZ_W.  But the forwarder bypasses the
; regfile when a downstream instruction reads Dn the same cycle Dn is
; being written, and the original forwarder returned wb_wdata
; unmasked -- losing the upper 24 bits on a byte-write hazard.
;
; Surfaced in Kickstart 1.3 boot inside InitStruct's word-copy entry
; sequence at $F81182-$F8118A:
;
;   $F81182: MOVE.L A1, D1     ; D1 = $00FC243A
;   $F81184: ADDQ.L #1, D1     ; D1 = $00FC243B
;   $F81186: AND.B #$FE, D1    ; D1 should be $00FC243A
;   $F8118A: MOVEA.L D1, A1    ; <-- reads D1 with EX->ID forward
;
; t116 already verifies AND.B preserves upper bits when followed by a
; CMPI, because the regfile merge has settled before CMPI reads D1.
; This test forces the hazard: AND.B (or any byte-Dn op) immediately
; followed by an instruction that reads Dn, with no separator that
; lets the writeback retire first.
;
; Pass = halt 0.  Failures:
;   $BAD1 = MOVEA.L Dn,An after AND.B mis-forwarded
;   $BAD2 = MOVE.L Dn,Dm after ANDI.B mis-forwarded
;   $BAD3 = MOVEA.L Dn,An after ORI.B mis-forwarded
;   $BAD4 = MOVE.L Dn,Dm after ADDI.B mis-forwarded
;   $BAD5 = .W-size forwarding hazard (ANDI.W followed by MOVE.L)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. AND.B (via the Kickstart $C23C encoding) then MOVEA.L ---
        move.l  #$00FC243B, D1
        ; AND.B #$FE, D1
        .word   $C23C
        .word   $00FE
        move.l D1, A1               ; reads D1 with forwarding hazard
        move.l  A1, D0
        cmpi.l  #$00FC243A, D0
        bne     fail1

        ; --- 2. ANDI.B then immediate MOVE.L ---
        move.l  #$11223344, D2
        andi.b  #$F0, D2
        move.l  D2, D3
        cmpi.l  #$11223340, D3
        bne     fail2

        ; --- 3. ORI.B then MOVEA.L ---
        move.l  #$AABBCC00, D2
        ori.b   #$0F, D2
        move.l D2, A2
        move.l  A2, D0
        cmpi.l  #$AABBCC0F, D0
        bne     fail3

        ; --- 4. ADDI.B then MOVE.L ---
        move.l  #$77665544, D2
        addi.b  #$01, D2
        move.l  D2, D4
        cmpi.l  #$77665545, D4
        bne     fail4

        ; --- 5. ANDI.W then MOVE.L (word-size forwarding) ---
        move.l  #$DEADBEEF, D2
        andi.w  #$F0F0, D2
        move.l  D2, D5
        cmpi.l  #$DEADB0E0, D5
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
