; Subroutine call: square via repeated addition.
; square(n) = sum of n copies of n.
        .org $400
        moveq #7, D0          ; argument
        bsr square
        cmp.l #49, D0
        beq pass
        stop #$dead

pass:
        ; second test: nested JSR
        moveq #3, D0
        jsr factorial          ; D0 = 3! = 6
        cmp.l #6, D0
        beq pass2
        stop #$bad2
pass2:
        stop #0

; D0 = D0 * D0  (positive only)
square:
        move.l D0, D1         ; D1 = original
        move.l D0, D2         ; D2 = counter
        moveq #0, D0          ; accumulator
sq_loop:
        add.l D1, D0
        subq.l #1, D2
        bne sq_loop
        rts

; D0 = D0! (positive only); uses recursion via JSR
factorial:
        cmp.l #1, D0
        bgt fac_recurse
        moveq #1, D0
        rts
fac_recurse:
        move.l D0, -(A7)      ; save n
        subq.l #1, D0
        jsr factorial         ; D0 = (n-1)!
        move.l (A7)+, D1      ; D1 = n
        move.l D0, D2         ; D2 = (n-1)!
        ; D0 = D2 * D1 via repeated add
        moveq #0, D0
fac_mul:
        add.l D2, D0
        subq.l #1, D1
        bne fac_mul
        rts
