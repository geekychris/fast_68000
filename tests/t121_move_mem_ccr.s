; MOVE.B/.W/.L with memory destination must set CCR.N/Z from the moved
; value (not from a stale 0 in src_operand).
;
; Surfaced in Kickstart 1.3's OldOpenLibrary at $F81AF0:
;   MOVE.B (A0)+, (A1)+
;   BNE    $F81AF0
; With our buggy CCR (Z=1 even on non-zero byte), the BNE never branched
; and the search-name builder copied only the FIRST byte.  Downstream
; FindResident missed every resident and graphics.library stored 0 in
; its $1A0 field, leading to a JSR $FFE2(0) garbage-PC trap.
;
; Test: source memory holds 'u' = $75 at $2000.  MOVE.B (A0)+, (A1)+ to
; copy it.  Z must be 0 afterwards (BNE should be taken).
;
; Pass = halt 0.  Failures:
;   $BAD1 = Z=1 after MOVE.B with non-zero source byte
;   $BAD2 = Z=0 after MOVE.B with zero source byte
;   $BAD3 = N wrong after MOVE.B with negative byte ($FF)
;   $BAD4 = Z wrong after MOVE.W mem-to-mem with non-zero value
;   $BAD5 = Z wrong after MOVE.L mem-to-mem with non-zero value

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. MOVE.B mem-to-mem with non-zero byte ---
        move.l  #$75000000, $00002000      ; byte at $2000 = $75 ('u')
        move.l  #$00000000, $00003000
        lea     $00002000, A0
        lea     $00003000, A1
        move.b  (A0)+, (A1)+                ; should set Z=0
        bne     ok1                          ; if Z=0, branch taken (correct)
        bra     fail1                        ; Z=1, bug

ok1:
        ; --- 2. MOVE.B mem-to-mem with zero byte ---
        move.l  #$00000000, $00002000
        move.l  #$FF000000, $00003000
        lea     $00002000, A0
        lea     $00003000, A1
        move.b  (A0)+, (A1)+                ; should set Z=1
        beq     ok2
        bra     fail2

ok2:
        ; --- 3. MOVE.B mem-to-mem with negative byte ($FF) ---
        move.l  #$FF000000, $00002000
        move.l  #$00000000, $00003000
        lea     $00002000, A0
        lea     $00003000, A1
        move.b  (A0)+, (A1)+                ; should set N=1
        bmi     ok3
        bra     fail3

ok3:
        ; --- 4. MOVE.W mem-to-mem with non-zero ---
        move.l  #$12340000, $00002000
        move.l  #$00000000, $00003000
        lea     $00002000, A0
        lea     $00003000, A1
        move.w  (A0)+, (A1)+
        bne     ok4
        bra     fail4

ok4:
        ; --- 5. MOVE.L mem-to-mem with non-zero ---
        move.l  #$11223344, $00002000
        move.l  #$00000000, $00003000
        lea     $00002000, A0
        lea     $00003000, A1
        move.l  (A0)+, (A1)+
        bne     ok5
        bra     fail5

ok5:
        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
