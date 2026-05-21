; AND.B #imm, Dn -- imm-to-Dn byte AND must preserve upper 24 bits.
;
; Surfaced in Kickstart 1.3 boot inside InitStruct's word-copy entry
; sequence at $F8118A:
;
;   $F81182: MOVE.L A1, D1     ; D1 = $00FC243A
;   $F81184: ADDQ.L #1, D1     ; D1 = $00FC243B
;   $F81186: AND.B #$FE, D1    ; should mask low byte only -> $00FC243A
;   $F8118A: MOVEA.L D1, A1    ; A1 = D1
;
; If AND.B #imm, Dn clobbers the upper 24 bits of Dn, A1 ends up at
; $0000003A and InitStruct reads source data from low chip-RAM
; (vector table) instead of the ROM data table.  That smashes the
; AllocMem'd library buffer with garbage, corrupts the heap free
; chain, and AllocMem later double-allocates an in-use Resource node.
;
; Encoded as AND.B #imm, Dn (opcode $C0/$C2/$C4/.. 3C XX):
;   bits 15:12 = 1100 (AND family)
;   bits 11:9  = Dn   (dst reg)
;   bits 8:6   = 000  (.B, EA->Dn direction)
;   bits 5:0   = 111 100 (mode 7 / reg 4 = #imm)
;
; assemblers usually emit ANDI.B for this; we want to exercise the
; AND-with-immediate encoding the Kickstart ROM uses, so encode by
; hand with .word.
;
; Test: D1 = $00FC243B, AND.B #$FE, D1; verify D1 = $00FC243A.
; Repeat for ANDI.B / ORI.B / EORI.B etc. as a quick sanity sweep --
; the bug is suspected in K_ALU only, but the test catches any size-
; size-propagation regression in any byte-to-Dn op.
;
; Pass = halt 0.  Failures:
;   $BAD1 = AND.B #imm, D1 cleared upper bits
;   $BAD2 = ANDI.B #imm, D1 cleared upper bits
;   $BAD3 = ORI.B #imm, D1 cleared upper bits
;   $BAD4 = EORI.B #imm, D1 cleared upper bits
;   $BAD5 = ADDI.B #imm, D1 cleared upper bits
;   $BAD6 = SUBI.B #imm, D1 cleared upper bits

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. AND.B #$FE, D1 (the actual Kickstart encoding) ---
        move.l  #$00FC243B, D1
        ; AND.B #$FE, D1 = $C23C 00FE
        .word   $C23C
        .word   $00FE
        cmpi.l  #$00FC243A, D1
        bne     fail1

        ; --- 2. ANDI.B #$FE, D1 (standard ANDI encoding) ---
        move.l  #$00FC243B, D1
        andi.b  #$FE, D1
        cmpi.l  #$00FC243A, D1
        bne     fail2

        ; --- 3. ORI.B #$80, D1 ---
        move.l  #$00FC2400, D1
        ori.b   #$80, D1
        cmpi.l  #$00FC2480, D1
        bne     fail3

        ; --- 4. EORI.B #$0F, D1 ---
        move.l  #$00FC2400, D1
        eori.b  #$0F, D1
        cmpi.l  #$00FC240F, D1
        bne     fail4

        ; --- 5. ADDI.B #1, D1 (byte add, no propagation into upper) ---
        move.l  #$00FC2400, D1
        addi.b  #1, D1
        cmpi.l  #$00FC2401, D1
        bne     fail5

        ; --- 6. SUBI.B #1, D1 ---
        move.l  #$00FC2401, D1
        subi.b  #1, D1
        cmpi.l  #$00FC2400, D1
        bne     fail6

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
fail6:  stop    #$BAD6
