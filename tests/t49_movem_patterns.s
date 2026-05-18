; MOVEM patterns used by Amiga code prologue/epilogue.
;
; Test 1: full save/restore via -(SP) / (SP)+
; Test 2: partial save (D2-D7/A2-A6 — typical C calling convention)
; Test 3: load via abs.L source
; Test 4: store via abs.L destination
; Test 5: predec on dst preserves An semantics (mask reversed)
;
; A handful of these patterns hit the same code paths Kickstart uses
; on EVERY routine entry/exit.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Test 1: full save/restore
        moveq   #1, D0
        moveq   #2, D1
        moveq   #3, D2
        moveq   #4, D3
        moveq   #5, D4
        moveq   #6, D5
        moveq   #7, D6
        move.l  #$BEEF1234, D7
        move.l  #$00001100, A1
        move.l  #$00001200, A2
        move.l  #$00001300, A3
        move.l  #$00001400, A4
        move.l  #$00001500, A5
        move.l  #$00001600, A6
        movem.l D0-D7/A1-A6, -(A7)

        ; Clobber
        moveq   #0, D0
        moveq   #0, D1
        moveq   #0, D2
        moveq   #0, D3
        moveq   #0, D4
        moveq   #0, D5
        moveq   #0, D6
        moveq   #0, D7
        move.l  #0, A1
        move.l  #0, A2
        move.l  #0, A3
        move.l  #0, A4
        move.l  #0, A5
        move.l  #0, A6

        movem.l (A7)+, D0-D7/A1-A6

        cmpi.l  #1, D0
        bne     fail_1
        cmpi.l  #2, D1
        bne     fail_1
        cmpi.l  #3, D2
        bne     fail_1
        cmpi.l  #7, D6
        bne     fail_1
        cmpi.l  #$BEEF1234, D7
        bne     fail_1
        move.l  A1, D0
        cmpi.l  #$00001100, D0
        bne     fail_1
        move.l  A6, D0
        cmpi.l  #$00001600, D0
        bne     fail_1

        ; Test 2: partial save (D2/D3/D5 only)
        move.l  #$DEAD0002, D2
        move.l  #$DEAD0003, D3
        move.l  #$DEAD0005, D5
        movem.l D2/D3/D5, -(A7)
        moveq   #0, D2
        moveq   #0, D3
        moveq   #0, D5
        movem.l (A7)+, D2/D3/D5
        cmpi.l  #$DEAD0002, D2
        bne     fail_2
        cmpi.l  #$DEAD0003, D3
        bne     fail_2
        cmpi.l  #$DEAD0005, D5
        bne     fail_2

        ; Test 3: MOVEM.L from (An) — the common AmigaOS form.  (Abs.L
        ; source/dest for MOVEM is a separate CPU gap; tracked in the OS
        ; plan.)
        move.l  #$11111111, $00008000
        move.l  #$22222222, $00008004
        move.l  #$33333333, $00008008
        move.l  #$44444444, $0000800C
        move.l  #$00008000, A1
        movem.l (A1), D0/D1/D2/D3
        cmpi.l  #$11111111, D0
        bne     fail_3
        cmpi.l  #$22222222, D1
        bne     fail_3
        cmpi.l  #$33333333, D2
        bne     fail_3
        cmpi.l  #$44444444, D3
        bne     fail_3

        ; Test 4: MOVEM.L to (An), then read back.
        move.l  #$AAAAAAAA, D4
        move.l  #$BBBBBBBB, D5
        move.l  #$00008100, A1
        movem.l D4/D5, (A1)
        move.l  $00008100, D0
        cmpi.l  #$AAAAAAAA, D0
        bne     fail_4
        move.l  $00008104, D0
        cmpi.l  #$BBBBBBBB, D0
        bne     fail_4

        ; Test 5: -(A7) store reverses mask -- saved order is
        ; (A6, A5, ..., A0, D7, ..., D0) in memory (low->high addresses).
        ; Inspect the saved values via direct mem reads.
        moveq   #$11, D0
        moveq   #$77, D7
        move.l  #$12345678, A6
        ; Save SP into an An so we can index it as a base for read-back.
        move.l  A7, A4
        movem.l D0/D7/A6, -(A7)
        ; Frame layout (low -> high addresses): D0, D7, A6.  MOVEM predec
        ; writes the *last* register in the list first (so saving A7-12 = D0,
        ; A7-8 = D7, A7-4 = A6).
        move.l  -12(A4), D1
        cmpi.l  #$11, D1
        bne     fail_5
        move.l  -8(A4), D1
        cmpi.l  #$77, D1
        bne     fail_5
        move.l  -4(A4), D1
        cmpi.l  #$12345678, D1
        bne     fail_5
        ; Pop and discard.
        movem.l (A7)+, D0/D7/A6

        stop    #0

fail_1: stop #$D451
fail_2: stop #$D452
fail_3: stop #$D453
fail_4: stop #$D454
fail_5: stop #$D455
