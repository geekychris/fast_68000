; Copper list-2 (canonical 2-word encoding).
;
; Run list 1 (one MOVE then end-of-list), then start list 2 (one MOVE
; then end-of-list); each writes a distinct chipset register.
;
; Each instruction is one .L: high word = IR1, low word = IR2.
;   list 1: MOVE COLOR  ($DFF188) <- $0111   ; then WAIT VP=$FF (end)
;   list 2: MOVE COLOR  ($DFF18C) <- $0222   ; then WAIT VP=$FF (end)

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; --- build list 1 at $00050000 ---
        move.l  #$01880111, $00050000      ; MOVE $DFF188 <- $0111
        move.l  #$FFFFFFFE, $00050004      ; end-of-list

        ; --- build list 2 at $00050100 ---
        move.l  #$018C0222, $00050100      ; MOVE $DFF18C <- $0222
        move.l  #$FFFFFFFE, $00050104      ; end-of-list

        ; --- COP1LC = $50000, then COPJMP1 ---
        move.l  #$00050000, $00FE0040
        move.l  #1,          $00FE0044
wait1:
        move.l  $00FE0048, D1
        andi.l  #1, D1
        bne     wait1

        ; --- COP2LC = $50100, then COPJMP2 ---
        move.l  #$00050100, $00FE004C
        move.l  #1,          $00FE0050
wait2:
        move.l  $00FE0048, D1
        andi.l  #1, D1
        bne     wait2

        ; --- verify both register slots.  Copper writes the high half
        ; --- of the long (be=4'b1100), so long-read returns {IR2, 0}.
        move.l  $00DFF188, D1
        cmpi.l  #$01110000, D1
        bne     fail_l1
        move.l  $00DFF18C, D1
        cmpi.l  #$02220000, D1
        bne     fail_l2

        stop    #0

fail_l1: stop #$CC11
fail_l2: stop #$CC22
