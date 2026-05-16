; Initialize a 32-long buffer at $2000, then copy it to $3000, then verify.
; Stresses (An)+ memory streaming on both source and destination.
;
; Basic-68000 cycles:
;   prologue: LEA(12)+LEA(12)+MOVEQ(4)+MOVEQ(4)+MOVE(4)         = 36
;   init loop (32 iters): MOVE.L Dn,(An)+(12)+ADDQ(8)+SUBQ(8)+
;     Bcc taken(10) = 38 ; last iter (not taken) = 36
;     total init body = 31*38 + 36                              = 1214
;   intra setup: MOVEQ(4)                                       =   4
;   copy loop (32 iters): MOVE.L (An)+,Dn(12)+MOVE.L Dn,(An)+(12)
;     +SUBQ(8)+Bcc taken(10) = 42 ; last = 40                   = 31*42 + 40 = 1342
;   verify loop (32 iters): MOVE.L (An)+,Dn(12)+CMP.L Dn,Dm(6)
;     +Bcc not taken(8)+ADDQ(8)+SUBQ(8)+Bcc taken(10) = 52      = 31*52 + 50 = 1662
;     (this loop verifies each element equals an expected value)
;   intra setup: 3 * MOVEQ + LEA                                =  4*3 + 12 = 24
;   epilogue: STOP                                              =   4
;   total                                                       = 36+1214+4+1342+24+1662+4 = 4286
;
; basic_68000_cycles=4286
        .org $400

        lea $2000, A0          ; init pointer
        lea $3000, A1          ; copy dest
        moveq #32, D0          ; iteration count
        moveq #1, D1           ; running value
        move.l A0, A2          ; A2 = saved init start
init_loop:
        move.l D1, (A0)+
        addq.l #1, D1
        subq.l #1, D0
        bne init_loop

        moveq #32, D0
        ; A2 still points at $2000 (init start); reuse as source for copy.
        move.l A2, A0
copy_loop:
        move.l (A0)+, D2
        move.l D2, (A1)+
        subq.l #1, D0
        bne copy_loop

        ; Verify the destination buffer.
        moveq #32, D0
        moveq #1, D1
        lea $3000, A0
verify_loop:
        move.l (A0)+, D2
        cmp.l D2, D1
        bne fail
        addq.l #1, D1
        subq.l #1, D0
        bne verify_loop

        stop #0
fail:
        stop #$dead
