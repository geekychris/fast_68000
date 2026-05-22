; MOVEM.W must advance the EA by 2 bytes per register, not 4,
; and write/read only the low word of each register.
;
; Our K_MOVEM iterator was hard-coded to advance the address by
; 4 bytes per register and to write/read all 4 bytes (be=1111),
; regardless of size.  Surfaced by the K1.3 boot wall at
; $FC6BD0:
;   MOVEM.W  D6/D7, -(SP)     ; mask $0300 = 2 registers
;   ... function body ...
;   MOVE.W   (SP)+, D6
;   MOVE.W   (SP)+, D7
;   RTS
; -- Verilator pushed 4 words (8 bytes) for the 2-register MOVEM.W
; instead of 2 words (4 bytes), leaving 4 extra bytes of garbage on
; the stack.  The matching MOVE.W pops were correct (2 words = 4
; bytes), so SP ended up 4 bytes too low when RTS ran -- RTS popped
; $0 from $18D0 (a slot the function never wrote) and jumped to
; PC=0, walking through the vector table as code.
;
; Pass = halt 0.  Failures:
;   $BAD1 = MOVEM.W predec didn't decrement An by 2 per register
;   $BAD2 = MOVEM.W postinc didn't increment An by 2 per register
;   $BAD3 = MOVEM.W store didn't write the low word of each reg
;   $BAD4 = MOVEM.W load didn't sign-extend the word into the long

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. MOVEM.W D6/D7, -(SP) must advance SP by exactly 4 ---
        move.l  #$00003000, A0
        ; Pre-fill memory below A0 with sentinels so any extra-byte
        ; stomp shows up loudly.
        move.l  #$AAAAAAAA, $2FF8
        move.l  #$BBBBBBBB, $2FFC
        move.l  #$11112222, D6
        move.l  #$33334444, D7
        movem.w D6/D7, -(A0)
        ; A0 must have decremented by exactly 4
        cmpa.l  #$00002FFC, A0
        bne     fail1
        ; Bytes at $2FFC..$2FFD should be the low word of D6 = $2222
        move.w  $2FFC, D0
        andi.l  #$0000FFFF, D0
        cmpi.l  #$00002222, D0
        bne     fail3
        ; Bytes at $2FFE..$2FFF should be the low word of D7 = $4444
        move.w  $2FFE, D0
        andi.l  #$0000FFFF, D0
        cmpi.l  #$00004444, D0
        bne     fail3
        ; Adjacent byte at $2FFA..$2FFB should still be $AAAA
        move.w  $2FFA, D0
        andi.l  #$0000FFFF, D0
        cmpi.l  #$0000AAAA, D0
        bne     fail3

        ; --- 2. MOVEM.W (A0)+, D2/D3 must advance A0 by 4 (2 regs *.W) ---
        ;   and sign-extend each word into the full Dn.
        move.l  #$00002FFC, A0
        ; Write $FFFF and $0001 in successive words so we can check
        ; sign extension.
        move.w  #$FFFF, $2FFC
        move.w  #$0001, $2FFE
        moveq   #0, D2
        moveq   #0, D3
        movem.w (A0)+, D2/D3
        cmpa.l  #$00003000, A0           ; A0 += 4
        bne     fail2
        cmpi.l  #$FFFFFFFF, D2           ; D2 = sign-extended $FFFF
        bne     fail4
        cmpi.l  #$00000001, D3           ; D3 = sign-extended $0001
        bne     fail4

        ; --- 3. MOVEM.W with longer mask (4 registers) advances by 8 ---
        move.l  #$00003000, A0
        move.l  #$11110000, D0
        move.l  #$22220000, D1
        move.l  #$33330000, D2
        move.l  #$44440000, D3
        movem.w D0/D1/D2/D3, -(A0)
        cmpa.l  #$00002FF8, A0           ; A0 -= 8 (4 regs * 2 bytes)
        bne     fail1
        ; In predec MOVEM, registers are stored high-to-low; the
        ; LAST written (= LOWEST address) is D0 (the lowest reg).
        ; So memory layout from $2FF8 up is: D0.w, D1.w, D2.w, D3.w.
        ; (= word at $2FF8 is D0's low half = 0, $2FFA is D1's = 0,
        ;    $2FFC is D2's = 0, $2FFE is D3's = 0.)
        move.w  $2FF8, D5
        andi.l  #$0000FFFF, D5
        cmpi.l  #$00000000, D5
        bne     fail3

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
