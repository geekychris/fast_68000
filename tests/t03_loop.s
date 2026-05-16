; Loop: sum integers 1..10, expect 55 in D0.
        .org $400
        moveq #0, D0          ; accumulator
        moveq #1, D1          ; counter
loop:
        add.l D1, D0
        addq.l #1, D1
        cmp.l #11, D1
        blt   loop
        cmp.l #55, D0
        beq   pass
        stop  #$dead
pass:
        stop  #0
