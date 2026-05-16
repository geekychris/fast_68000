; ALU + branches: compute 5+3, then 8-2, and assert equality.
        .org $400
        moveq #5, D0
        moveq #3, D1
        add.l D1, D0          ; D0 = 8
        moveq #2, D2
        sub.l D2, D0          ; D0 = 6
        cmp.l #6, D0
        beq   pass
        stop  #$dead
pass:
        moveq #0, D3
        and.l D0, D3          ; D3 = 0, flags Z=1
        beq   pass2
        stop  #$beef
pass2:
        stop  #0
