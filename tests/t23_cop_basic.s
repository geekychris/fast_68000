; Copper basic test (canonical 2-word encoding).
;
; Copper list at $00006000 -- canonical Amiga MOVE format: each
; instruction is one 32-bit longword IR1:IR2 (IR1 = high half).
;
;   inst 0: IR1=$0180 IR2=$0ABC  MOVE COLOR00 ($DFF180) <- $0ABC
;   inst 1: IR1=$0184 IR2=$0DEF  MOVE COLOR (at $DFF184) <- $0DEF
;   inst 2: IR1=$FFFF IR2=$FFFE  end-of-list (canonical WAIT VP=$FF)
;
; Our Denise stores colors at 4-byte stride per slot, so writes to
; $DFF180 and $DFF184 land in two independent colors[] entries.  The
; Copper's MOVE places IR2 in the high half of the long (be=4'b1100),
; matching Denise's 32-bit-aligned register file.
;
; CPU programs COP1LC, strobes COPJMP1, polls COPSTAT until 0, then
; verifies that long reads from $DFF180 and $DFF184 reflect the writes.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- prepare Copper list at $6000 ----
        ; Each instruction is one .L (high word = IR1, low word = IR2).
        move.l #$01800ABC,  $00006000    ; MOVE COLOR00 <- $0ABC
        move.l #$01840DEF,  $00006004    ; MOVE COLOR@$184 <- $0DEF
        move.l #$FFFFFFFE,  $00006008    ; end-of-list (WAIT VP=$FF)

        ; ---- program Copper and start ----
        move.l #$00006000, $00FE0040     ; COP1LC = list start
        move.l #1,         $00FE0044     ; COPJMP1: any write strobes start

wait:   move.l $00FE0048, D0             ; COPSTAT
        andi.l #1, D0
        bne    wait

        ; ---- verify via Denise long-read of the 32-bit color slots.
        ; The Copper writes the high half (be=4'b1100), so a long read
        ; returns {IR2, 16'd0}.
        move.l $00DFF180, D1
        cmpi.l #$0ABC0000, D1
        bne    fail1
        move.l $00DFF184, D2
        cmpi.l #$0DEF0000, D2
        bne    fail2
        stop   #0

fail1:  stop   #$cd11
fail2:  stop   #$cd12
