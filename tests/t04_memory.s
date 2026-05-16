; Store + load round-trip + auto-inc/dec to a small buffer at $2000.
; Each core uses its own buffer to avoid contention: core0 at $2000, core1 at $2100.
; We bake one program; each core reads SP to decide which buffer to use.
        .org $400
        ; D7 = SP. Core0 SP = 0x4000, core1 SP = 0x5000.
        move.l A7, D7
        cmp.l #$4000, D7
        beq core0_entry
        ; core1 entry
        lea $2100, A0
        bra after_choose
core0_entry:
        lea $2000, A0
after_choose:
        ; Store 1, 2, 3, 4 starting at (A0) using post-inc.
        move.l A0, A1         ; A1 = original base
        moveq #1, D0
        move.l D0, (A0)+
        moveq #2, D0
        move.l D0, (A0)+
        moveq #3, D0
        move.l D0, (A0)+
        moveq #4, D0
        move.l D0, (A0)+

        ; Now read them back via pre-dec from the current A0 (which points to
        ; the byte just past the last store).
        moveq #0, D1          ; running sum
        move.l -(A0), D2
        add.l D2, D1          ; +4
        move.l -(A0), D2
        add.l D2, D1          ; +3
        move.l -(A0), D2
        add.l D2, D1          ; +2
        move.l -(A0), D2
        add.l D2, D1          ; +1

        cmp.l #10, D1
        beq pass
        stop #$dead
pass:
        stop #0
