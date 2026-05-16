; Subroutine call/return overhead. Calls a tiny "add D1 to D0" 50 times.
;
; Basic-68000 cycles:
;   prologue:                       2*MOVEQ                          =     8
;   per iter (50): MOVEQ(4)+JSR abs.L(20)+ADD.L Dn,Dm(8)+RTS(16)
;     +SUBQ(8)+Bcc-T(10)                                              = 66
;   49 taken + 1 not taken (-2):                                      = 49*66+64 = 3298
;   epilogue: CMP.L #imm,Dn(14)+Bcc-T(10)+STOP(4)                     =    28
;   total                                                             = 3334
;
; basic_68000_cycles=3334
        .org $400
        moveq #50, D7
        moveq #0, D0
call_loop:
        moveq #1, D1
        jsr add_d1
        subq.l #1, D7
        bne call_loop
        cmp.l #50, D0
        beq pass
        stop #$dead
pass:
        stop #0

add_d1:
        add.l D1, D0
        rts
