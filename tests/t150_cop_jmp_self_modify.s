; t150 -- Copper runaway when COPJMP target contains MOVE COP1LCH.
;
; K1.3 V33 boot can reach the boot-Copper list at chip-RAM $0420 before
; graphics.library has populated the LOFCprList target.  The boot list
; ends with MOVE COPJMP2 = 0, which jumps to whatever cop2lc currently
; holds (often $0 at that point).  The Copper then walks zeros until
; it picks up a non-zero word that happens to decode as a MOVE
; COP1LCH=$xxxx instruction.  Without protection, our cop1lc latch
; takes the full 32-bit xlat result ({$xxxx, canon_cop1lc[15:0]}) and
; lands at a chip-RAM address past the end of installed memory.  Every
; subsequent VBL auto-restart reloads PC from that bogus address,
; permanently parking the Copper in unreachable memory.
;
; Real OCS Agnus has only 19 address lines for COP1LC/COP2LC, so a
; Copper "MOVE COP1LCH=$0140" gets masked to $00 in the high half and
; the resulting cop1lc still lands in chip RAM.  The runaway self-heals
; on the next VBL.  We must do the same.
;
; This test reproduces the scenario in isolation:
;   - boot Copper at $0400: MOVE COLOR00=$0F00; MOVE COPJMP2=0
;   - cop2lc points at $1000 which contains the "poison":
;       MOVE COP1LCH = $0140   ($00800140)
;       MOVE COP1LCL = $0420   ($00820420)
;       end-of-list            ($FFFEFFFE)
;   - after enabling DMACON.COPEN+DMAEN we wait for at least one VBL
;     auto-restart, then probe COP1LC via its slave-port read at
;     $00FE0040.  Expected (with the fix): cop1lc retains its low-19-bit
;     value, i.e. $00000420.  Without the fix: cop1lc would be
;     $01400420 and chip RAM at the high end stays untouched.
;
; Note: this test currently FAILS on baseline (commit f923bc7) because
; no Copper-self-write protection exists yet.  Once the fix lands the
; test should pass.
;
;   Pass = halt 0.
;   Fail = stop $C151 -- COLOR00 didn't get the boot-list write
;        = stop $C152 -- COP1LC was corrupted past 19-bit range

        .org $400

start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Pre-stage COLOR00.
        move.l  #$00000000, $00DFF180

        ; --- boot Copper at $0500 (use $500 not $400 to avoid stepping
        ;     on this code at $400).  Two MOVEs then COPJMP2.
        move.l  #$01800F00, $00000500       ; MOVE COLOR00=$0F00
        move.l  #$008A0000, $00000504       ; MOVE COPJMP2=0
        move.l  #$FFFFFFFE, $00000508       ; defensive end-of-list

        ; --- poison list at $1000.  MOVE COP1LCH=$0140 + MOVE COP1LCL=$0420
        ;     + end.  These are exactly the writes a runaway Copper would
        ;     do if it walked into garbage that decoded this way.
        move.l  #$00800140, $00001000       ; MOVE COP1LCH=$0140
        move.l  #$00820420, $00001004       ; MOVE COP1LCL=$0420
        move.l  #$FFFFFFFE, $00001008       ; end-of-list

        ; --- program COP2LC = $1000 first via canonical port.
        move.l  #$00001000, $00DFF084

        ; --- program COP1LC = $500 + enable DMACON.COPEN+DMAEN.
        move.l  #$00000500, $00DFF080
        move.w  #$8280,     $00DFF096       ; SET | DMAEN | COPEN

        ; Spin long enough for an auto-restart (need ~71K cycles).
        move.l  #$00020000, D0
spin:
        subq.l  #1, D0
        bne     spin

        ; Verify COLOR00 got the $0F00 from the boot list.
        move.l  $00DFF180, D1
        andi.l  #$FFFF0000, D1
        cmpi.l  #$0F000000, D1
        bne     fail_color

        ; Read back COP1LC via slave port at $00FE0040 and check that
        ; bits 23..19 are zero (Agnus's 19-bit chip-RAM address space).
        ; Without the fix cop1lc would be $01400420.
        move.l  $00FE0040, D2
        andi.l  #$FFF80000, D2              ; mask to bits 31..19
        bne     fail_runaway

        stop    #0

fail_color:    stop #$C151
fail_runaway:  stop #$C152
