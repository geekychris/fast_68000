; Copper list-2: COP2LC + COPJMP2 should start the Copper at the second
; list pointer.  Run list 1 (one MOVE then HALT), then start list 2
; (one MOVE then HALT) and check both wrote their distinct sentinels.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; --- build list 1 at $00050000: MOVE $00080000 <- $11111111 ; HALT ---
        move.l  #$00080000, $00050000
        move.l  #$11111111, $00050004
        move.l  #$FFFFFFFF, $00050008
        move.l  #$00000000, $0005000C

        ; --- build list 2 at $00050100: MOVE $00080004 <- $22222222 ; HALT ---
        move.l  #$00080004, $00050100
        move.l  #$22222222, $00050104
        move.l  #$FFFFFFFF, $00050108
        move.l  #$00000000, $0005010C

        ; --- zero sentinels ---
        move.l  #0, $00080000
        move.l  #0, $00080004

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

        ; --- verify both sentinels ---
        move.l  $00080000, D1
        cmpi.l  #$11111111, D1
        bne     fail_l1
        move.l  $00080004, D1
        cmpi.l  #$22222222, D1
        bne     fail_l2

        stop    #0

fail_l1: stop #$CC11
fail_l2: stop #$CC22
