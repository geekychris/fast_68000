; Blitter copy test: A -> D (LF=A) with no masks, no shifts.
;
; Source @ $2000: two longs (1234ABCD, DEADBEEF) = four 16-bit words.
; Dest   @ $3000: should match source after a 4-word, 1-row blit.
;
; Only core 0 drives the blitter; core 1 halts immediately.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        ; Other cores: just halt OK.
        stop   #0

core0:
        ; ---- prepare source bytes ----
        move.l #$1234ABCD, $00002000
        move.l #$DEADBEEF, $00002004

        ; ---- clear destination ----
        move.l #0, $00003000
        move.l #0, $00003004

        ; ---- program blitter ----
        ; BLTCON: LF=$F0 (A), shifts 0, line=0, use_a+use_d
        ;   [31:24]=F0, [23:20]=0 ASH, [19:16]=0 BSH, [11]=0 LINE,
        ;   [10:8]=0 oct, [3:0]=9 (USEA|USED)
        move.l #$F0000009, $00FE0000
        move.l #$0000FFFF, $00FE0004     ; BLTAFWM
        move.l #$0000FFFF, $00FE0008     ; BLTALWM
        move.l #$00002000, $00FE000C     ; BLTAPT  = source
        move.l #$00003000, $00FE0018     ; BLTDPT  = dest
        move.l #0,         $00FE001C     ; BLTAMOD = 0
        move.l #0,         $00FE0028     ; BLTDMOD = 0

        ; BLTSIZE: height=1 (bits 21:6), width=4 (bits 5:0) -> (1<<6)|4 = $44
        move.l #$00000044, $00FE0038

        ; ---- poll busy ----
wait:   move.l $00FE003C, D0
        andi.l #1, D0
        bne    wait

        ; ---- verify ----
        move.l $00003000, D1
        cmpi.l #$1234ABCD, D1
        bne    fail1
        move.l $00003004, D2
        cmpi.l #$DEADBEEF, D2
        bne    fail2
        stop   #0

fail1:  stop   #$bd11
fail2:  stop   #$bd12
