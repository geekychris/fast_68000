; SR / CCR read/write semantics.
;
; Real-world usage in Kickstart: ORI/ANDI to SR for masking interrupts,
; MOVE SR/CCR to inspect state, EXG/SWAP for register juggling.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Test 1: MOVE SR, Dn reads SR ----
        ; At reset we're in supervisor mode (S=1).  S is bit 13 of SR.
        move.w  SR, D0
        andi.w  #$2000, D0
        cmpi.w  #$2000, D0
        bne     fail_1

        ; ---- Test 2: ORI to SR sets I bits (interrupt mask) ----
        move.w  SR, D1            ; save original
        ori.w   #$0700, SR        ; mask all interrupts
        move.w  SR, D0
        andi.w  #$0700, D0
        cmpi.w  #$0700, D0
        bne     fail_2

        ; ---- Test 3: ANDI to SR clears I bits ----
        andi.w  #$F8FF, SR        ; clear I[2:0]
        move.w  SR, D0
        andi.w  #$0700, D0
        cmpi.w  #$0000, D0
        bne     fail_3
        ; Restore.
        move.w  D1, SR

        ; ---- Test 4: ANDI to CCR clears flags ----
        ; First set V and C via arithmetic.
        move.l  #$7FFFFFFF, D0
        add.l   #1, D0            ; sets V
        andi.w  #$F0, CCR         ; clear bits 0-3 of CCR (X,N,Z,V,C)
        move.w  SR, D0           ; 68000 has no MOVE-from-CCR; SR low byte = CCR
        andi.w  #$001F, D0
        cmpi.w  #0, D0
        bne     fail_4

        ; ---- Test 5: ORI to CCR sets specific flags ----
        andi.w  #$F0, CCR
        ori.w   #$05, CCR         ; set X (4) and C (0)? bits: 4=X, 3=N, 2=Z, 1=V, 0=C
        move.w  SR, D0           ; 68000 has no MOVE-from-CCR; SR low byte = CCR
        andi.w  #$001F, D0
        cmpi.w  #$05, D0
        bne     fail_5

        ; ---- Test 6: MOVE #imm, CCR writes the low 5 bits ----
        move.w  #$001F, CCR
        move.w  SR, D0           ; 68000 has no MOVE-from-CCR; SR low byte = CCR
        andi.w  #$001F, D0
        cmpi.w  #$001F, D0
        bne     fail_6

        ; ---- Test 7: EXG swaps two registers ----
        move.l  #$AAAA0001, D0
        move.l  #$BBBB0002, D1
        exg     D0, D1
        cmpi.l  #$BBBB0002, D0
        bne     fail_7
        cmpi.l  #$AAAA0001, D1
        bne     fail_7

        ; ---- Test 8: SWAP swaps high and low halves of Dn ----
        move.l  #$12345678, D0
        swap    D0
        cmpi.l  #$56781234, D0
        bne     fail_8

        ; ---- Test 9: EXT.W sign-extends byte to word ----
        move.l  #$0000_00FF, D0
        ext.w   D0
        cmpi.l  #$0000_FFFF, D0
        bne     fail_9

        ; ---- Test 10: EXT.L sign-extends word to long ----
        move.l  #$0000_FFFF, D0
        ext.l   D0
        cmpi.l  #$FFFFFFFF, D0
        bne     fail_10

        stop    #0

fail_1:  stop #$D701
fail_2:  stop #$D702
fail_3:  stop #$D703
fail_4:  stop #$D704
fail_5:  stop #$D705
fail_6:  stop #$D706
fail_7:  stop #$D707
fail_8:  stop #$D708
fail_9:  stop #$D709
fail_10: stop #$D710
