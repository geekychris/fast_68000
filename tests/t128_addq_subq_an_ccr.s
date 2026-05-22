; ADDQ / SUBQ on An must NOT modify any CCR bits.
;
; Per the 68000 spec: ADDQ #imm,An and SUBQ #imm,An operate
; on the full 32 bits of An regardless of the encoded size,
; and "do not affect the condition codes" (same as ADDA/SUBA).
;
; The K_ALUQ executor was unconditionally committing N/Z/V/C
; and the X flag even for An destinations.  Surfaced by the
; Musashi cosim at K1.3 boot $FC19A8:
;   ADDQ.B  #8, A2
; -- Verilator went from CCR=$10 (X=1) to CCR=$00 (all clear);
; Musashi correctly left CCR=$10 alone.  Downstream NEGX/ADDX
; that depend on X took different paths.
;
; ADDQ/SUBQ #imm,Dn does update CCR, so the Dn case still
; tests positively here as the regression for the working path.
;
; Pass = halt 0.  Failures:
;   $BAD1 = ADDQ #imm,An clobbered CCR
;   $BAD2 = SUBQ #imm,An clobbered CCR
;   $BAD3 = ADDQ #imm,Dn DIDN'T set Z when result was 0
;   $BAD4 = ADDQ #imm,An didn't compute the right result

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. ADDQ #8, A2 must not touch CCR ---
        move.l  #$00FC04DC, A2
        andi    #0, CCR              ; clear CCR
        ori     #$1D, CCR            ; prime X|N|Z|C (V=0)
        addq.l  #8, A2               ; A2 = $00FC04E4, CCR untouched
        beq     ok1                  ; Z=1 should still hold
        bra     fail1
ok1:
        bmi     ok1b                 ; N=1 should still hold
        bra     fail1
ok1b:
        bcs     ok1c                 ; C=1 should still hold
        bra     fail1
ok1c:
        ; (skip CMPA -- it would clobber CCR; trust later check via D0)
        move.l  A2, D0
        cmpi.l  #$00FC04E4, D0
        bne     fail4                ; result must be A2 + 8

        ; --- 2. SUBQ #4, A3 must not touch CCR ---
        move.l  #$00001000, A3
        andi    #0, CCR
        ori     #$1D, CCR            ; reprime CCR (CMPI above cleared it)
        subq.l  #4, A3               ; A3 = $00000FFC
        beq     ok2
        bra     fail2
ok2:
        bmi     ok2b
        bra     fail2
ok2b:
        bcs     ok2c
        bra     fail2
ok2c:
        move.l  A3, D0
        cmpi.l  #$00000FFC, D0
        bne     fail4

        ; --- 3. Positive control: ADDQ on Dn DOES set CCR ---
        ; Clear CCR, then add 1 to -1 -> 0 with carry.  Z must be set,
        ; C must be set, X must be set.
        andi    #0, CCR
        move.l  #$FFFFFFFF, D2
        addq.l  #1, D2               ; D2 = 0, sets Z=1, C=1, X=1
        bne     fail3                ; Z must be set
        bcc     fail3                ; C must be set

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
