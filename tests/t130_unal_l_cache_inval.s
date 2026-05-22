; Verify unaligned .L write at $X (X & 3 == 2) actually writes
; all 4 bytes, including the high half at X+2.
;
; Surfaced by the Musashi cosim at K1.3 boot $FC15DA:
;   MOVE.L D1, -(A0)   ; A0=$1C86 -> $1C82, writes D1
; -- then later ADD.W -(A0), D1 read word at $1C84 and got 0
; instead of the low half of the written value.
;
; Pass = halt 0.  Failures:
;   $BAD1 = unaligned .L write didn't update word at X+2
;   $BAD2 = unaligned .L write didn't update word at X
;   $BAD3 = predec MOVE.L unaligned write missed second half
;   $BAD4 = adjacent untouched bytes were clobbered

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. Unaligned .L immediate write at $1002 ---
        move.l  #$AAAAAAAA, $1000
        move.l  #$BBBBBBBB, $1004
        move.l  #$DEADBEEF, $00001002    ; unaligned .L (abs.L)
        move.w  $1004.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BEEF, D2
        bne     fail1
        move.w  $1002.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000DEAD, D2
        bne     fail2
        ; Adjacent: $1000..$1001 still $AAAA, $1006..$1007 still $BBBB
        move.w  $1000.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000AAAA, D2
        bne     fail4
        move.w  $1006.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BBBB, D2
        bne     fail4

        ; --- 2. PREDEC form MOVE.L D1, -(A0) - the K1.3 case ---
        move.l  #$AAAAAAAA, $1000
        move.l  #$BBBBBBBB, $1004
        move.l  #$12345678, D1
        move.l  #$00001006, A0
        move.l  D1, -(A0)                 ; A0 -= 4 -> $1002, writes D1
        cmpa.l  #$00001002, A0
        bne     fail3
        move.w  $1004.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00005678, D2
        bne     fail3
        move.w  $1002.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00001234, D2
        bne     fail3

        ; --- 3. Predec form at a higher address ($1C82) -- exact K1.3 ---
        move.l  #$AAAAAAAA, $1C80
        move.l  #$BBBBBBBB, $1C84
        move.l  #$00FC52F0, D1
        move.l  #$00001C86, A0
        move.l  D1, -(A0)
        cmpa.l  #$00001C82, A0
        bne     fail3
        move.w  $1C84.w, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$000052F0, D2
        bne     fail3

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
