; TST.B (An)+ / TST.W (An)+ / TST.L (An)+ must post-increment An.
;
; CPU bug found while diagnosing WB1.3 boot stall: K_TST's S_LOAD settle
; path forgot to commit src_an_update.  Result: TST.B (A0)+ read the
; byte and set N/Z, but never advanced A0.  K1.3's strlen helper at
; \$FE4AD0 (used by FS task to process the bootstrap packet) ran 65535
; iterations against a static address until D0.w wrapped, stalling FS
; packet handling for ~275K cycles and corrupting the boot.
;
; Same fix is needed for TST.B (An)+ in chip RAM, slow RAM, and across
; .B/.W/.L sizes.  This test covers all combinations.
;
; Pass = halt 0.  Fail = stop \$BADn.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. TST.B (A0)+ on CHIP RAM ---
        move.l  #$00000000, $00001000
        lea     $00001000, A0
        .word   $4A18                  ; TST.B (A0)+
        beq     tst_b_chip_z_ok
        stop    #$BAD1
tst_b_chip_z_ok:
        cmpa.l  #$00001001, A0          ; A0 must advance by 1
        bne     fail_b_chip_inc

        ; --- 2. TST.B (A0)+ on SLOW RAM ---
        move.l  #$00000000, $00C00100
        lea     $00C00100, A0
        .word   $4A18                  ; TST.B (A0)+
        beq     tst_b_slow_z_ok
        stop    #$BAD2
tst_b_slow_z_ok:
        cmpa.l  #$00C00101, A0
        bne     fail_b_slow_inc

        ; --- 3. TST.W (A0)+ on CHIP RAM ---
        move.l  #$00000000, $00001100
        lea     $00001100, A0
        .word   $4A58                  ; TST.W (A0)+
        beq     tst_w_chip_z_ok
        stop    #$BAD3
tst_w_chip_z_ok:
        cmpa.l  #$00001102, A0          ; A0 must advance by 2
        bne     fail_w_chip_inc

        ; --- 4. TST.L (A0)+ on CHIP RAM ---
        move.l  #$00000000, $00001200
        lea     $00001200, A0
        .word   $4A98                  ; TST.L (A0)+
        beq     tst_l_chip_z_ok
        stop    #$BAD4
tst_l_chip_z_ok:
        cmpa.l  #$00001204, A0          ; A0 must advance by 4
        bne     fail_l_chip_inc

        ; --- 5. TST.B -(A0) on CHIP RAM (predec, also needs An update) ---
        move.l  #$00000000, $00001300
        lea     $00001301, A0          ; will become \$1300 after predec
        .word   $4A20                  ; TST.B -(A0)
        beq     tst_b_dec_z_ok
        stop    #$BAD5
tst_b_dec_z_ok:
        cmpa.l  #$00001300, A0          ; A0 must decrement by 1
        bne     fail_b_dec

        ; --- 6. The actual K1.3 strlen pattern: scan slow RAM for \$0 ---
        ; Write "abc\0xyz" to slow RAM.
        move.l  #$61626300, $00C00200   ; 'a','b','c',\$00
        move.l  #$78797A00, $00C00204   ; 'x','y','z',\$00
        lea     $00C00200, A0
        moveq   #-1, D0
strlen_loop:
        .word   $4A18                  ; TST.B (A0)+
        .word   $57C8, $FFFC           ; DBEQ D0, strlen_loop
        ; Loop should exit after 4 iters (a,b,c,\$0 - Z set on \$0).
        ; D0.w decremented 3 times (a, b, c each set Z=0): D0.w = \$FFFC.
        ; A0 = \$C00204 (past the \$0).
        cmpa.l  #$00C00204, A0
        bne     fail_strlen_a0
        cmpi.l  #$FFFFFFFC, D0
        bne     fail_strlen_d0

        stop    #0

fail_b_chip_inc:  stop #$BADA
fail_b_slow_inc:  stop #$BADB
fail_w_chip_inc:  stop #$BADC
fail_l_chip_inc:  stop #$BADD
fail_b_dec:       stop #$BADE
fail_strlen_a0:   stop #$BAD8
fail_strlen_d0:   stop #$BAD9
