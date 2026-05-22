; K_ALU with memory source must commit X flag (for ADD/SUB).
;
; Surfaced by the Musashi cosim at K1.3 boot $FC14F8 (the ROM
; checksum loop):
;   ADD.W -(A0), D1
;   DBF   D0, $FC14F8
; -- Verilator updated N/Z/V/C but left X stale.  Musashi
; correctly tracked X = carry-out of the ADD.  The K_ALU
; register-source path (EX-stage) was already committing X
; for ADD/SUB; only the memory-source path (S_LOAD K_ALU
; settle) was missing cc_x_we_c / cc_x_c.
;
; Pass = halt 0.  Failures:
;   $BAD1 = ADD.L -(A0), Dn didn't set X on carry-out
;   $BAD2 = SUB.L -(A0), Dn didn't set X on borrow
;   $BAD3 = downstream ADDX didn't see the carry-in we set
;   $BAD4 = ADD.L (A0), Dn with no carry left X set
;   $BAD5 = AND.L mem,Dn touched X

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. ADD.L -(A0), Dn with carry-out should set X ---
        move.l  #$A0000000, $00001000
        move.l  #$00001004, A0
        move.l  #$80000000, D1
        andi    #0, CCR              ; clear all CCR (X=0, C=0, etc.)
        add.l   -(A0), D1            ; D1 = $80000000 + $A0000000 -> carry-out
        bcc     fail1                ; C must be set
        ; Confirm X via ROXL #1 (rotate through X). ROXL.B #1 D2:
        ;   D2 was 0; X=1 rotates in -> D2 low byte = $01
        moveq   #0, D2
        roxl.b  #1, D2
        cmpi.b  #$01, D2
        bne     fail1

        ; --- 2. SUB.L -(A0), Dn with borrow should set X ---
        move.l  #$F0000000, $00001000
        move.l  #$00001004, A0
        move.l  #$00000001, D1
        andi    #0, CCR
        sub.l   -(A0), D1            ; 1 - $F0000000 = borrow
        bcc     fail2
        moveq   #0, D2
        roxl.b  #1, D2
        cmpi.b  #$01, D2
        bne     fail2

        ; --- 3. ADDX picks up X set by ADD.L -(A0), Dn ---
        move.l  #$A0000000, $00001000
        move.l  #$00001004, A0
        move.l  #$80000000, D1
        andi    #0, CCR
        add.l   -(A0), D1            ; X=1
        moveq   #0, D2
        moveq   #0, D3
        addx.b  D2, D3               ; D3 = 0 + 0 + X = 1
        cmpi.b  #$01, D3
        bne     fail3

        ; --- 4. ADD.L (A0), Dn with NO carry must CLEAR X ---
        move.l  #$00000001, $00001000
        move.l  #$00001000, A0
        move.l  #$00000001, D1
        andi    #0, CCR
        ori     #$10, CCR            ; pre-set X=1
        add.l   (A0), D1             ; 1 + 1 = 2, no carry: X must clear
        bcs     fail4
        moveq   #0, D2
        roxl.b  #1, D2               ; should rotate in 0
        cmpi.b  #$00, D2
        bne     fail4

        ; --- 5. AND.L mem,Dn must NOT touch X ---
        move.l  #$FFFFFFFF, $00001000
        move.l  #$00001000, A0
        move.l  #$0000FFFF, D1
        andi    #0, CCR
        ori     #$10, CCR            ; X=1, C=0
        and.l   (A0), D1             ; result $0000FFFF; X must remain 1
        moveq   #0, D2
        roxl.b  #1, D2               ; X must still be 1 -> D2 = $01
        cmpi.b  #$01, D2
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
