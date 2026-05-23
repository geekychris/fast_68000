; t202 -- canonical Copper WAIT + SKIP test.
;
; Copper list at $00006000 (canonical 2-word encoding):
;   inst 0: WAIT  VP=$00 mask=$7F/$3F     ($00017F7E)
;             -- compare always true (vbeam_v >= 0); falls straight through.
;   inst 1: MOVE  $DFF180 <- $0AAA       ($01800AAA)
;             -- expected to execute.
;   inst 2: SKIP  VP=$00 mask=$7F/$3F     ($00017F7F)
;             -- compare true => skip next instruction.
;   inst 3: MOVE  $DFF184 <- $0BBB       ($01840BBB)
;             -- expected to be SKIPPED.
;   inst 4: MOVE  $DFF188 <- $0CCC       ($01880CCC)
;             -- expected to execute (next after the skipped one).
;   inst 5: end-of-list  WAIT VP=$FF      ($FFFFFFFE)
;
; Final state expected (long reads, high-half writes):
;   $DFF180 -> $0AAA_0000
;   $DFF184 -> $0000_0000   (untouched -- start state)
;   $DFF188 -> $0CCC_0000

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Build the Copper list.
        move.l #$00017F7E, $00006000   ; WAIT VP=0 (passes immediately)
        move.l #$01800AAA, $00006004   ; MOVE $DFF180 <- $0AAA
        move.l #$00017F7F, $00006008   ; SKIP VP=0 (raster past => skip)
        move.l #$01840BBB, $0000600C   ; MOVE $DFF184 <- $0BBB (skipped)
        move.l #$01880CCC, $00006010   ; MOVE $DFF188 <- $0CCC
        move.l #$FFFFFFFE, $00006014   ; end-of-list

        ; Start Copper.
        move.l #$00006000, $00FE0040
        move.l #1,         $00FE0044
wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

        ; Verify final state.
        move.l $00DFF180, D1
        cmpi.l #$0AAA0000, D1
        bne    fail1
        move.l $00DFF184, D2
        cmpi.l #$00000000, D2          ; should NOT have been written
        bne    fail_skipped
        move.l $00DFF188, D3
        cmpi.l #$0CCC0000, D3
        bne    fail3

        stop   #0

fail1:        stop  #$c211
fail_skipped: stop  #$c212
fail3:        stop  #$c213
