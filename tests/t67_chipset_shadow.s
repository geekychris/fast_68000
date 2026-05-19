; Chipset shadow + Zorro autoconfig tests.
;
; Verifies:
;   - BPL1PTH ($DFF0E0) round-trips a 16-bit value.
;   - DDFSTRT ($DFF092) round-trips.
;   - DIWSTRT ($DFF08E) round-trips.
;   - COP1LCH ($DFF080) round-trips.
;   - $E80000 autoconfig exposes our simulated FAST RAM card (type $E0).
;
; Pass = halt 0.  Failures: $BAD1..$BAD5.
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; BPL1PTH at $DFF0E0 (16-bit).  Write $1234, read back.
        move.w  #$1234, $00DFF0E0
        move.l  $00DFF0E0, D0
        andi.l  #$FFFF0000, D0
        cmpi.l  #$12340000, D0
        bne     fail_bpl

        ; COP1LCH at $DFF080.  Write $ABCD, read back.
        move.w  #$ABCD, $00DFF080
        move.l  $00DFF080, D1
        andi.l  #$FFFF0000, D1
        cmpi.l  #$ABCD0000, D1
        bne     fail_cop

        ; DDFSTRT at $DFF092.  Write $0038, read back.
        move.w  #$0038, $00DFF092
        move.l  $00DFF090, D2          ; long-read picks up DIWSTOP|DDFSTRT
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000038, D2
        bne     fail_ddf

        ; Zorro autoconfig: $E80000 returns the simulated FAST RAM type
        ; byte ($E0) in the high lane.  Card stays visible until a
        ; configure write at $E80048 -- see t83_autoconfig for the
        ; full handshake.
        move.l  $00E80000, D3
        andi.l  #$FF000000, D3
        cmpi.l  #$E0000000, D3
        bne     fail_acf

        stop    #0
forever: bra forever
fail_bpl: stop #$BAD1
fail_cop: stop #$BAD2
fail_ddf: stop #$BAD3
fail_acf: stop #$BAD4
