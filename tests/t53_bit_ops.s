; Bit operations: BTST / BSET / BCLR / BCHG on registers and memory.
; Used everywhere in chipset register manipulation (DMACON bits etc.).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Test 1: BTST on a Dn ----
        move.l  #$0000_0080, D0
        btst    #7, D0
        bne     ok_btst1          ; Z=0 means bit was set (BTST sets Z=!bit)
        bra     fail_1
ok_btst1:
        btst    #6, D0
        bne     fail_1            ; bit 6 is clear -> Z=1 -> bne should fall through

        ; ---- Test 2: BSET sets a bit and updates Z ----
        moveq   #0, D0
        bset    #5, D0
        cmpi.l  #$0000_0020, D0
        bne     fail_2

        ; ---- Test 3: BCLR clears a bit ----
        move.l  #$FFFF_FFFF, D0
        bclr    #10, D0
        cmpi.l  #$FFFF_FBFF, D0
        bne     fail_3

        ; ---- Test 4: BCHG toggles a bit ----
        move.l  #$0000_0001, D0
        bchg    #0, D0
        cmpi.l  #$0000_0000, D0
        bne     fail_4
        bchg    #0, D0
        cmpi.l  #$0000_0001, D0
        bne     fail_4

        ; (Memory-destination BSET/BCLR/BCHG/BTST are not yet implemented
        ;  in the core -- need RMW path or a separate load.  Tracked
        ;  separately.)

        ; ---- Test 7: BTST dynamic (bit number in Dn) ----
        move.l  #$0000_8000, D0
        moveq   #15, D1
        btst    D1, D0
        beq     fail_7             ; Z=0 expected (bit 15 set)

        ; ---- Test 8: BCHG dynamic ----
        moveq   #0, D0
        moveq   #20, D1
        bchg    D1, D0
        cmpi.l  #$0010_0000, D0
        bne     fail_8

        stop    #0

fail_1: stop #$D801
fail_2: stop #$D802
fail_3: stop #$D803
fail_4: stop #$D804
fail_5: stop #$D805
fail_6: stop #$D806
fail_7: stop #$D807
fail_8: stop #$D808
