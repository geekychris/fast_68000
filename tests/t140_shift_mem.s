; Memory single-bit shifts: LSR/LSL/ASR/ROR/ROL .W <ea>.
;
; The 68000 encoding `1110_0op_dr_11_ea` is a single-bit shift on a
; 16-bit memory word.  Pre-fix, K_SHIFT routed the shifted result to
; D[An-reg-num] (treating the EA's An as a destination D-reg) and
; never wrote memory.  Surfaced via WB1.3 boot cosim at $FC7C86
; LSR.W $10(A1) — D1 was corrupted from $10005 to $1385C, mem
; unchanged.
;
; This test verifies the read-modify-write path: memory word at
; $disp(A1) gets shifted in place; the D-register named by EA's
; An-reg stays untouched.
;
; The asm68k.py assembler doesn't grok the one-operand memory-shift
; form, so the instructions are emitted as raw words.  Encodings:
;   LSR.W $10(A1) = $E2E9 $0010
;   LSL.W $10(A1) = $E3E9 $0010
;   ASR.W $10(A1) = $E0E9 $0010
;   ROR.W $10(A1) = $E6E9 $0010
;   ROL.W $10(A1) = $E7E9 $0010

        .org $400
start:
        ; ---- 1: LSR.W on $C0FF -> $607F, D1 untouched ----
        move.l  #$00010000, A1
        move.l  #$DEADBEEF, D1
        move.w  #$C0FF, $10(A1)
        .word   $E2E9, $0010            ; LSR.W $10(A1)
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000607F, D2
        bne     fail_1
        cmpi.l  #$DEADBEEF, D1
        bne     fail_1

        ; ---- 2: LSR.W on $0001 -> 0, D1 untouched ----
        move.l  #$AABBCCDD, D1
        move.w  #$0001, $10(A1)
        .word   $E2E9, $0010
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000000, D2
        bne     fail_2
        cmpi.l  #$AABBCCDD, D1
        bne     fail_2

        ; ---- 3: LSL.W on $4000 -> $8000 ----
        move.l  #$11223344, D1
        move.w  #$4000, $10(A1)
        .word   $E3E9, $0010            ; LSL.W $10(A1)
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00008000, D2
        bne     fail_3
        cmpi.l  #$11223344, D1
        bne     fail_3

        ; ---- 4: ASR.W on $8000 keeps sign -> $C000 ----
        move.l  #$DEAD0000, D1
        move.w  #$8000, $10(A1)
        .word   $E0E9, $0010            ; ASR.W $10(A1)
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000C000, D2
        bne     fail_4
        cmpi.l  #$DEAD0000, D1
        bne     fail_4

        ; ---- 5: ROR.W on $0001 -> $8000 ----
        move.l  #$BABEFACE, D1
        move.w  #$0001, $10(A1)
        .word   $E6E9, $0010            ; ROR.W $10(A1)
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00008000, D2
        bne     fail_5
        cmpi.l  #$BABEFACE, D1
        bne     fail_5

        ; ---- 6: ROL.W on $8000 -> $0001 ----
        move.l  #$F00DBEEF, D1
        move.w  #$8000, $10(A1)
        .word   $E7E9, $0010            ; ROL.W $10(A1)
        move.w  $10(A1), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000001, D2
        bne     fail_6
        cmpi.l  #$F00DBEEF, D1
        bne     fail_6

        stop    #0                      ; pass

fail_1: stop   #1
fail_2: stop   #2
fail_3: stop   #3
fail_4: stop   #4
fail_5: stop   #5
fail_6: stop   #6
