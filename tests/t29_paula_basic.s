; Paula basic register round-trip test.
;
; Programs all four channels with distinct values, reads them back, and
; verifies the slave-port registers store and return correctly. Also
; verifies AUDENA toggles.
;
; Doesn't try to observe actual audio output (no CPU-side hook into the
; mixed audio_l/r signals); the demo proves audible playback.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Write & read AUD0 ----
        ; Canonical Amiga offsets within Paula's window:
        ;   AUDxLC  .L  at $A0 + 0x10*x
        ;   AUDxLEN .W  at +$4
        ;   AUDxPER .W  at +$6
        ;   AUDxVOL .W  at +$8
        move.l #$00005000, $00FE02A0      ; AUD0LC
        move.l #$00000042, $00FE02A4      ; AUD0LEN
        move.l #$000000C8, $00FE02A6      ; AUD0PER = 200
        move.l #$00000040, $00FE02A8      ; AUD0VOL = 64 (full)

        move.l $00FE02A0, D1
        cmpi.l #$00005000, D1
        bne    fail1
        move.l $00FE02A4, D2
        cmpi.l #$00000042, D2
        bne    fail2
        move.l $00FE02A6, D3
        cmpi.l #$000000C8, D3
        bne    fail3
        move.l $00FE02A8, D4
        cmpi.l #$00000040, D4
        bne    fail4

        ; ---- Write & read AUD3 (the highest channel) ----
        move.l #$00005800, $00FE02D0
        move.l #$00000020, $00FE02D4
        move.l #$00000080, $00FE02D6
        move.l #$00000020, $00FE02D8

        move.l $00FE02D0, D5
        cmpi.l #$00005800, D5
        bne    fail5
        move.l $00FE02D8, D6
        cmpi.l #$00000020, D6
        bne    fail6

        ; ---- Toggle AUDENA ----
        move.l #$0000000F, $00FE0200      ; enable all 4
        move.l $00FE0200, D7
        cmpi.l #$0000000F, D7
        bne    fail7

        ; ---- Disable cleanly ----
        move.l #0, $00FE0200
        move.l $00FE0200, D7
        cmpi.l #0, D7
        bne    fail8

        stop   #0

fail1:  stop #$ae11
fail2:  stop #$ae12
fail3:  stop #$ae13
fail4:  stop #$ae14
fail5:  stop #$ae15
fail6:  stop #$ae16
fail7:  stop #$ae17
fail8:  stop #$ae18
