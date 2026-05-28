; ADDQ.<size> #n, An / SUBQ.<size> #n, An must use FULL longword.
;
; Per 68k spec, when ADDQ/SUBQ destination is an address register,
; the operation is ALWAYS performed on the full 32 bits, regardless
; of the encoded size.  CCR is NOT affected.
;
; CPU bug: our K_ALUQ handler passed ex_size to the ALU.  The ALU
; masked the result to byte/word.  Writing the masked result back to
; An zero-extended above the masked region, so ADDQ.W #4, A7 with
; A7=\$C071E0 produced A7=\$000000E4.
;
; K1.3 uses ADDQ.B #4, A7 (opcode \$584F) at \$FE0A50 to pop a stack
; arg.  Under the bug, SP \$C071E0 became \$71E0 — the next RTS read
; the wrong stack location (zero from chip RAM that had never been
; pushed to) and jumped to PC=\$0, killing WB1.3 FS task at r=4M.
;
; Pass = halt 0.  Fail = stop \$BADn.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. ADDQ.W #4, A0 with high bits set ---
        lea     $00C071E0, A0
        addq.w  #4, A0
        cmpa.l  #$00C071E4, A0      ; full 32-bit add — high bits preserved
        bne     fail_addq_w_a0

        ; --- 2. ADDQ.B #4, A7 — the exact K1.3 pattern at \$FE0A50 ---
        ; Swap to a high-address SP, run the instruction, verify
        ; high byte preserved.
        move.l  A7, D7              ; save real SP
        lea     $00C00400, A7       ; SP to slow-RAM addr
        .word   $584F                ; ADDQ.B #4, A7 (the K1.3 instruction)
        move.l  A7, D6              ; read back SP
        move.l  D7, A7              ; restore real SP
        cmpi.l  #$00C00404, D6
        bne     fail_addq_b_a7

        ; --- 3. ADDQ.W #1, A0 — verify .W also longword on An ---
        lea     $00C04000, A0
        addq.w  #1, A0
        cmpa.l  #$00C04001, A0
        bne     fail_addq_w_a0_2

        ; --- 4. SUBQ.W #2, A0 — sub also longword on An ---
        lea     $00C04010, A0
        subq.w  #2, A0
        cmpa.l  #$00C0400E, A0
        bne     fail_subq_w_a0

        ; --- 5. ADDQ #4 must NOT update CCR when dst is An ---
        ; Set Z flag (via TST #0), then ADDQ to An should leave Z=1.
        moveq   #0, D0
        tst.l   D0                  ; Z=1
        lea     $00C00500, A0
        addq.w  #4, A0              ; must NOT touch CCR
        bne     fail_ccr            ; Z should still be set

        ; --- 6. ADDQ.B (size bits 00) #4, A0 — also longword on An ---
        ; Opcode \$5808: data=100(=4), dir=0, size=00(=B), mode=001, reg=000
        lea     $00FF1234, A0
        .word   $5808                ; ADDQ.B #4, A0
        cmpa.l  #$00FF1238, A0
        bne     fail_addq_b_a0

        stop    #0

fail_addq_w_a0:    stop #$BAD1
fail_addq_b_a7:    stop #$BAD2
fail_addq_w_a0_2:  stop #$BAD3
fail_subq_w_a0:    stop #$BAD4
fail_ccr:          stop #$BAD5
fail_addq_b_a0:    stop #$BAD6
