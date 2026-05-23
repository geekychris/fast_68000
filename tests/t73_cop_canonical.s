; Canonical Amiga Copper address test (canonical 2-word encoding).
;
; Verifies that writes to $DFF080..$DFF08B reach the internal copper
; engine (not just the chipset shadow) and run a real Copper program.
;
; Copper list at $6800 (4 bytes per instruction):
;   inst 0: MOVE COLOR ($DFF180) <- $0ABC
;   inst 1: MOVE COLOR ($DFF184) <- $0DEF
;   inst 2: WAIT VP=$FF (end-of-list)
;
; CPU programs COP1LCH/L through $DFF080/$DFF082, strobes COPJMP1 at
; $DFF088, polls legacy COPSTAT, and verifies the writes.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Build the Copper list.
        move.l #$01800ABC, $00006800      ; MOVE $DFF180 <- $0ABC
        move.l #$01840DEF, $00006804      ; MOVE $DFF184 <- $0DEF
        move.l #$FFFFFFFE, $00006808      ; end-of-list (WAIT VP=$FF)

        ; Point COP1LC at the list through canonical 16-bit halves.
        move.w #$0000, $00DFF080          ; COP1LCH
        move.w #$6800, $00DFF082          ; COP1LCL
        ; Strobe via canonical COPJMP1 ($DFF088).
        move.w #$0001, $00DFF088

        ; Wait for the Copper to halt (poll legacy COPSTAT).
wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

        ; Verify via Denise long-read.
        move.l $00DFF180, D1
        cmpi.l #$0ABC0000, D1
        bne    fail1
        move.l $00DFF184, D2
        cmpi.l #$0DEF0000, D2
        bne    fail2

        ; Shadow round-trip on COP1LCL.  $DFF080 is in is_shadow_reg so a
        ; 32-bit read returns {chip_regs[$80], chip_regs[$82]} = {0, $6800}.
        move.l $00DFF080, D3
        cmpi.l #$00006800, D3
        bne    fail3

        stop    #0

fail1:  stop    #$cc71
fail2:  stop    #$cc72
fail3:  stop    #$cc73
