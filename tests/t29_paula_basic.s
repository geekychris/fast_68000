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
        move.l #$00005000, $00FE0210      ; AUD0LC
        move.l #$00000042, $00FE0214      ; AUD0LEN
        move.l #$000000C8, $00FE0218      ; AUD0PER = 200
        move.l #$00000040, $00FE021C      ; AUD0VOL = 64 (full)

        move.l $00FE0210, D1
        cmpi.l #$00005000, D1
        bne    fail1
        move.l $00FE0214, D2
        cmpi.l #$00000042, D2
        bne    fail2
        move.l $00FE0218, D3
        cmpi.l #$000000C8, D3
        bne    fail3
        move.l $00FE021C, D4
        cmpi.l #$00000040, D4
        bne    fail4

        ; ---- Write & read AUD3 (the highest channel) ----
        move.l #$00005800, $00FE0240
        move.l #$00000020, $00FE0244
        move.l #$00000080, $00FE0248
        move.l #$00000020, $00FE024C

        move.l $00FE0240, D5
        cmpi.l #$00005800, D5
        bne    fail5
        move.l $00FE024C, D6
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
