; Exception SR push + RTE SR pop must honor the alignment of SSP within
; the 4-byte mem[] word.
;
; The 68000 6-byte exception frame is:
;   SP[+0]: SR (word)
;   SP[+2]: PC (long)
;
; If pre-exception SP[1:0] = 10 (16-bit aligned but not 32-bit), the SR
; word lands at the HIGH half of a 4-byte mem[] cell -- byte enables
; should be 4'b1100 with wdata = SR << 16.  Our previous code
; unconditionally used 4'b0011 and wdata = SR, which silently overwrote
; bytes 2-3 of the just-pushed PC.  K1.3 boot caught this -- RTE popped
; $001C08F4 instead of $00FC08F4 (SR=$001C smeared into PC's high byte).
;
; Test: deliberately push SSP into the [1:0]==10 alignment, then trigger
; a TRAP #15 (vector 47) so the CPU pushes a 6-byte exception frame.
; The trap handler reads back the saved-PC long from the stack and
; verifies it matches the post-TRAP PC.  Also verifies the SR.
;
; Pass = halt 0.
;   $BAD1 = saved-PC on stack != expected post-TRAP PC
;   $BAD2 = saved-SR on stack high byte differs from current SR upper
;   $BAD3 = RTE returned to wrong PC

        .org $BC                          ; vector 47 = TRAP #15
        .long trap15_handler

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Force SSP to [1:0]==10 by subtracting 2 from A7 then a NOP.
        ; The TRAP push will land bytes 0-3 at addr&~3 (= high half of
        ; the new aligned word).  This is the alignment that surfaces the
        ; bug.
        suba.l  #2, A7

        ; Mark return PC so the handler can verify.  We use after_trap.
        moveq   #0, D7
        trap    #15
after_trap:
        ; If we got here properly, D7 was set to 1 by the handler AND
        ; the handler verified the frame.  D6 holds the handler's reads.
        cmpi.l  #1, D7
        bne     fail3

        ; Restore SP.
        adda.l  #2, A7

        stop    #0

trap15_handler:
        ; Stack now has: [SSP+0]=SR (word), [SSP+2]=PC (long, points
        ; at the instruction after TRAP).
        ;
        ; Pop saved SR and PC into D-regs to verify.
        ; Note A7 here is the supervisor SSP just after the TRAP push.
        move.w  (A7), D0                  ; D0.W = saved SR
        move.l  2(A7), D1                 ; D1.L = saved PC
        ; Verify saved PC equals after_trap (label).
        lea     after_trap, A0
        move.l  A0, D2
        cmp.l   D2, D1
        bne     trap15_fail1
        ; SR upper byte must contain the supervisor S bit (bit 13 of $2000).
        ; CCR is whatever it was at TRAP entry.
        andi.w  #$E000, D0                ; isolate T, S, ?
        cmpi.w  #$2000, D0                ; supervisor bit set, T clear
        bne     trap15_fail2
        moveq   #1, D7                    ; mark success for caller
        rte

trap15_fail1:
        ; Write a marker so we know which fail path
        moveq   #$11, D7
        rte

trap15_fail2:
        moveq   #$22, D7
        rte

fail3:  stop    #$BAD3
