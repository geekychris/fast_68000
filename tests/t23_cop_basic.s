; Copper basic test: Copper writes two values to memory then halts.
;
; Copper list at $00006000 (8 bytes per instruction = 24 bytes for 3 insns):
;   inst 0: write $DEADBEEF to $00003000
;   inst 1: write $CAFEBABE to $00003004
;   inst 2: HALT
;
; CPU programs COP1LC, strobes COPJMP1, polls COPSTAT until 0, verifies
; $3000 and $3004.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- prepare Copper list at $6000 ----
        move.l #$00003000,  $00006000    ; target = $3000
        move.l #$DEADBEEF,  $00006004    ; data
        move.l #$00003004,  $00006008    ; target = $3004
        move.l #$CAFEBABE,  $0000600C    ; data
        move.l #$FFFFFFFF,  $00006010    ; HALT
        move.l #0,          $00006014    ; (unused for HALT)

        ; ---- clear destinations ----
        move.l #0, $00003000
        move.l #0, $00003004

        ; ---- program Copper and start ----
        move.l #$00006000, $00FE0040     ; COP1LC = list start
        move.l #1,         $00FE0044     ; COPJMP1: any write strobes start

wait:   move.l $00FE0048, D0             ; COPSTAT
        andi.l #1, D0
        bne    wait

        ; ---- verify ----
        move.l $00003000, D1
        cmpi.l #$DEADBEEF, D1
        bne    fail1
        move.l $00003004, D2
        cmpi.l #$CAFEBABE, D2
        bne    fail2
        stop   #0

fail1:  stop   #$cd11
fail2:  stop   #$cd12
