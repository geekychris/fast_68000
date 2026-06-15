; CIA timer fidelity: bit-4 (LOAD strobe) readback + THI-while-stopped reload.
;
; Two real-silicon CIA semantics tested here (mirrored from minimig
; cia_timera.v):
;
;   (a) CRA bit 4 is a write-only strobe that triggers a counter reload.
;       It must read back as 0; otherwise software reading CRA back and
;       writing the same value would re-trigger the LOAD on every CRA
;       write, scrambling timer state.
;
;   (b) Writing TAHI while the timer is *stopped* (CRA[0]=0) reloads
;       the counter from the {TAHI,TALO} latch — even in continuous
;       mode.  Without this, a "stop, set new period, start" sequence
;       would start counting from whatever stale value was in the
;       counter, not the new period.
;
; Same checks for timer B (CRB / TBLO / TBHI).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; ---- Test (a): CRA bit 4 LOAD strobe reads back as 0 ----
        ; Write CRA = $10 (just LOAD strobe, START=0, RUNMODE=0).
        move.b  #$10, $00FE040E
        move.b  $00FE040E, D0
        andi.l  #$FF, D0
        cmpi.l  #$00, D0                ; bit 4 should NOT read back
        bne     fail_1

        ; Write CRA = $19 (LOAD + RUNMODE + START), readback should
        ; have LOAD masked: $09.
        move.b  #$19, $00FE040E
        move.b  $00FE040E, D0
        andi.l  #$FF, D0
        cmpi.l  #$09, D0
        bne     fail_2

        ; Same for CRB.
        move.b  #$10, $00FE040F
        move.b  $00FE040F, D0
        andi.l  #$FF, D0
        cmpi.l  #$00, D0
        bne     fail_3

        move.b  #$19, $00FE040F
        move.b  $00FE040F, D0
        andi.l  #$FF, D0
        cmpi.l  #$09, D0
        bne     fail_4

        ; ---- Test (b): THI write while stopped reloads counter ----
        ; First, make sure timer A is stopped.
        move.b  #$00, $00FE040E         ; CRA = 0 (stopped)

        ; Set TALO/TAHI = $1234 by writing each byte while stopped.
        ; The TAHI write should reload the live counter to $1234 even
        ; without LOAD strobe.
        move.b  #$34, $00FE0404         ; TALO = $34
        move.b  #$12, $00FE0405         ; TAHI = $12 → reload triggered
        move.b  $00FE0404, D0           ; read TALO (live count)
        andi.l  #$FF, D0
        cmpi.l  #$34, D0
        bne     fail_5
        move.b  $00FE0405, D0           ; read TAHI
        andi.l  #$FF, D0
        cmpi.l  #$12, D0
        bne     fail_6

        ; Same for timer B.
        move.b  #$00, $00FE040F         ; CRB = 0 (stopped)
        move.b  #$78, $00FE0406         ; TBLO = $78
        move.b  #$56, $00FE0407         ; TBHI = $56
        move.b  $00FE0406, D0
        andi.l  #$FF, D0
        cmpi.l  #$78, D0
        bne     fail_7
        move.b  $00FE0407, D0
        andi.l  #$FF, D0
        cmpi.l  #$56, D0
        bne     fail_8

        stop    #0

fail_1: stop #$D641
fail_2: stop #$D642
fail_3: stop #$D643
fail_4: stop #$D644
fail_5: stop #$D645
fail_6: stop #$D646
fail_7: stop #$D647
fail_8: stop #$D648
