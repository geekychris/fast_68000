; Iterative Fibonacci. Computes fib(20) = 6765. Register-only ALU.
;
; Basic-68000 cycles:
;   prologue: 3 * MOVEQ                                        =  12
;   per iteration (20 total): MOVE.L Dn,Dm(4)+MOVE.L Dn,Dm(4)
;     +ADD.L Dn,Dm(8)+SUBQ.L(8)+Bcc taken(10)                  =  34
;   19 taken + 1 not taken (-2 cycles on the last)             = 19*34 + 32 = 678
;   epilogue: CMP.L #imm(14)+Bcc taken(10)+STOP(4)             =  28
;   total                                                      = 718
;
; basic_68000_cycles=718
        .org $400
        moveq #20, D0          ; iteration count
        moveq #0, D1           ; a
        moveq #1, D2           ; b
fib_loop:
        move.l D1, D3          ; tmp = a
        move.l D2, D1          ; a = b
        add.l D3, D2           ; b = b + tmp
        subq.l #1, D0
        bne fib_loop
        cmp.l #6765, D1
        beq pass
        stop #$dead
pass:
        stop #0
