; Canonical Amiga Copper address test.
;
; Verifies that writes to $DFF080..$DFF08B reach the internal copper
; engine (not just the chipset shadow) and run a real Copper program.
;
; Copper list at $6800 (8 bytes per instruction):
;   inst 0: MOVE $DEADBEEF -> $00003800
;   inst 1: MOVE $CAFEBABE -> $00003804
;   inst 2: HALT
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
        move.l #$00003800, $00006800
        move.l #$DEADBEEF, $00006804
        move.l #$00003804, $00006808
        move.l #$CAFEBABE, $0000680C
        move.l #$FFFFFFFF, $00006810      ; HALT
        move.l #0,         $00006814

        ; Clear destination words.
        move.l #0, $00003800
        move.l #0, $00003804

        ; Point COP1LC at the list through canonical 16-bit halves.
        move.w #$0000, $00DFF080          ; COP1LCH
        move.w #$6800, $00DFF082          ; COP1LCL
        ; Strobe via canonical COPJMP1 ($DFF088).
        move.w #$0001, $00DFF088

        ; Wait for the Copper to halt (poll legacy COPSTAT).
wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

        move.l $00003800, D1
        cmpi.l #$DEADBEEF, D1
        bne    fail1
        move.l $00003804, D2
        cmpi.l #$CAFEBABE, D2
        bne    fail2

        ; Shadow round-trip on COP1LCL.
        move.l $00DFF080, D3
        cmpi.l #$00006800, D3
        bne    fail3

        stop    #0

fail1:  stop    #$cc71
fail2:  stop    #$cc72
fail3:  stop    #$cc73
