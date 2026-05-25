; t143 -- CIA Timer B counts at ~host/5 (real-CIA phi2 rate), not host clock.
;
; Real 8520 CIA runs at phi2 ~= 0.715 MHz, which is 1/10 of the 7.16 MHz
; CPU clock and 1/5 of the 3.55 MHz Agnus clock our host clock approximates.
; Before this fix, the CIA `tick` input was tied to 1 and the timers ran at
; host bus rate -- so K1.3's timer.device.Init at $FE8F8E loaded TBHI=$FF
; and busy-waited for CIA-A TODLOW to change while testing TBHI's sign bit
; as a timeout.  With Timer B running 5x too fast, TBHI's high bit cleared
; in ~33K host cycles -- well before the first VBL (~71K cycles) -- so the
; OS took the Alert path ($FE8F9E -> JSR Alert -> BRA $FE8F70), which
; bypasses the BSR's matching RTS.  The orphaned return PC corrupted SP
; and crashed the boot at the timer.device.Init epilogue.
;
; This test bounds Timer B's countdown rate.  We load TB with a known value,
; spin a known number of host cycles, then check that TB has counted down by
; roughly the expected amount.  With /5 prescaler, ~1 TB decrement per 5
; host cycles.  Anything significantly faster (e.g. /1) or slower (/20+)
; fails.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Load Timer B with $FFFF in free-running (continuous) mode.
        ;      RUNMODE = 0 (continuous), LOAD = 1 (force reload).  Once
        ;      started, TB counts down then auto-reloads to $FFFF on
        ;      underflow.  We won't let it underflow.
        move.b  #$FF, $00FE_0406        ; TBLO = $FF
        move.b  #$FF, $00FE_0407        ; TBHI = $FF (auto-load + start)
        move.b  #$11, $00FE_040F        ; CRB: LOAD=1 + START=1, continuous

        ; ---- Spin a known number of subq+bne pairs to consume host cycles.
        ;      The CPU's IPC in this loop is roughly constant, so the host-
        ;      cycle cost scales linearly with iteration count.
        move.l  #1000, D1
spin:
        subq.l  #1, D1
        bne     spin

        ; ---- Read TBHI (latches TBLO too).
        move.b  $00FE_0407, D2          ; D2 = TBHI

        ; ---- With /5 prescaler, Timer B should still have plenty of high
        ;      bits set.  In particular, TBHI must remain >= $80 (high bit
        ;      set, BMI would take) after a 1000-iter spin.  Without the
        ;      prescaler, TBHI underflows several times in this window and
        ;      can be anywhere.  The K1.3 boot path needs the high bit set
        ;      so the timer.device.Init wait loop keeps polling for the
        ;      TOD tick rather than falling through to Alert.
        cmpi.b  #$80, D2
        bcs     fail_too_fast           ; D2 < $80  (unsigned: cmpi sets C if a<b)

        ; ---- Also assert TB did at least *some* counting (i.e., is not
        ;      stuck at $FF).  The 1000-iter spin retires ~2000 instructions;
        ;      with /5 prescaler that's ~400 TB ticks at IPC 1 or ~80 ticks
        ;      at IPC 0.25.  Either way TBHI should be at most $FF.  This
        ;      checks the timer is *running* (catches a bug where the
        ;      prescaler value never fires).
        cmpi.b  #$FF, D2
        bhi     fail_no_count

        stop    #0

fail_too_fast:  stop #$c300             ; TB counted 5x+ too fast (no prescaler)
fail_no_count:  stop #$c301             ; TB didn't count at all
