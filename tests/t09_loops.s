; Exercise Tier-4/5 instructions: shifts, DBcc loops, LINK/UNLK, PEA.
;
; Layout:
;  - The reset vector boots us at $400 with SP at $7F0 (set by m68k_top harness).
;  - We compute 2^4 = 16 via an asl.l loop using DBcc.
;  - We then push a constant via PEA and pop it via "move.l (sp)+, Dn" - actually
;    we read it back with move.l (a7)+, Dn (handled by EA mode 011 on A7).
;  - We also exercise LINK/UNLK around a small "function".
;
        .org $400

        ;------------------------------------------------------------
        ; Test 1: asl.l #1, Dn in a DBRA loop computes 2^4 = 16.
        ; D0 = 1, loop 4 times: D0 <<= 1.  Then D0 must equal 16.
        ;------------------------------------------------------------
        moveq #1, D0
        moveq #3, D1               ; DBcc decrements; runs (count+1) iterations
sh_lp:
        asl.l #1, D0
        dbra  D1, sh_lp
        cmpi.l #16, D0
        bne   fail

        ;------------------------------------------------------------
        ; Test 2: lsr.l #4, Dn divides by 16.
        ;------------------------------------------------------------
        moveq #-1, D2              ; D2 = 0xFFFFFFFF
        lsr.l #4, D2
        cmpi.l #$0FFFFFFF, D2
        bne   fail

        ;------------------------------------------------------------
        ; Test 3: ror.l with dynamic count. Rotate 0x12345678 right by 8
        ;         -> 0x78123456.
        ;------------------------------------------------------------
        move.l #$12345678, D3
        moveq #8, D4
        ror.l D4, D3
        cmpi.l #$78123456, D3
        bne   fail

        ;------------------------------------------------------------
        ; Test 4: PEA pushes the effective address (not the data at the EA).
        ;         pea $1234.W pushes the long $00001234 onto the stack.
        ;         We then pop it via move.l (a7)+, Dn.
        ;------------------------------------------------------------
        pea $1234
        move.l (A7)+, D5
        cmpi.l #$1234, D5
        bne   fail

        ;------------------------------------------------------------
        ; Test 5: LINK A6, #-8; UNLK A6 around a small frame.
        ;         Save SP, LINK should leave SP = old_SP - 4 - 8.
        ;         UNLK should restore SP = old_SP and A6 to its prior value.
        ;------------------------------------------------------------
        move.l A7, D6              ; save initial SP
        move.l A6, D7              ; save initial A6
        link  A6, #-8              ; push A6, A6 := SP, SP := SP + disp
        ; After link: SP = (D6 - 4) + (-8) = D6 - 12.
        move.l A7, D0
        move.l D6, D1
        subi.l #12, D1
        cmpi.l #0, D0              ; (just to set flags around)
        ; verify D0 == D1
        sub.l D1, D0
        cmpi.l #0, D0
        bne   fail
        unlk  A6
        ; After unlk: SP back to D6, A6 back to D7.
        move.l A7, D0
        sub.l  D6, D0
        cmpi.l #0, D0
        bne   fail
        move.l A6, D0
        sub.l  D7, D0
        cmpi.l #0, D0
        bne   fail

        ;------------------------------------------------------------
        ; All passed.
        ;------------------------------------------------------------
        stop  #0
fail:
        stop  #$BAD2
