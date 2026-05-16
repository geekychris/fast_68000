; Blitter shift+mask test.
;
; Source A @ $2000:  $1234  $5678   (long $1234_5678)
; ASH = 4 (barrel-shift A right by 4 bits across two words)
; BLTAFWM = $FF00 (first-word mask)
; BLTALWM = $00FF (last-word mask)
;
; Word 0 (first): raw $1234, after mask = $1200. With prev=0, joined=$00001200;
;                 shifted bits [19:4] = $0120.
; Word 1 (last) : raw $5678, after mask = $0078. With prev (masked) = $1200,
;                 joined=$12000078; shifted bits [19:4] = $0007.
; Expected D @ $3000 (long) = $0120_0007.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        move.l #$12345678, $00002000
        move.l #0,         $00003000

        ; BLTCON: LF=$F0 (A), ASH=4, BSH=0, line=0, USEA|USED ($9)
        move.l #$F0400009, $00FE0000
        move.l #$0000FF00, $00FE0004   ; BLTAFWM
        move.l #$000000FF, $00FE0008   ; BLTALWM
        move.l #$00002000, $00FE000C
        move.l #$00003000, $00FE0018
        move.l #0, $00FE001C
        move.l #0, $00FE0028
        move.l #$00000042, $00FE0038   ; height=1, width=2

wait:   move.l $00FE003C, D0
        andi.l #1, D0
        bne    wait

        move.l $00003000, D1
        cmpi.l #$01200007, D1
        bne    fail
        stop   #0

fail:   stop   #$bd31
