; Sum integers 1..100. Stresses tight register-only ALU loop.
;
; Basic-68000 cycles (from M68000PRM Instruction Execution Times):
;   prologue:   2 * MOVEQ                                  =     8
;   99 taken iters:  ADD.L Dn,Dm(8) + ADDQ.L(8) + CMPI.L(14) + Bcc-T(10) = 40
;       (real CMPI.L is encoded with imm; we use CMP.L #imm,Dn which the
;        68000 PRM lists at 14 cycles for the long-immediate destination)
;   last iter (Bcc not taken):                       8+8+14+ 8 = 38
;   epilogue:   CMP.L #imm,Dn(14) + Bcc-T(10) + STOP(4)    =    28
;   total = 8 + 99*40 + 38 + 28                            =  4034
;
; basic_68000_cycles=4034
        .org $400
        moveq #0, D0
        moveq #1, D1
sum_loop:
        add.l D1, D0
        addq.l #1, D1
        cmp.l #101, D1
        blt sum_loop
        cmp.l #5050, D0
        beq pass
        stop #$dead
pass:
        stop #0
