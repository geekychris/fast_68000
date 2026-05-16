; Denise demo: 4-plane indexed-color image with 16-entry palette.
;
; Each row of plane k holds k+1 nested bit patterns producing a colorful
; gradient/checker visible in the SDL window.  After setting up the
; bitplanes and palette, the CPU triggers Denise once and re-triggers
; every frame so the live render reflects ongoing animation.
;
; Layout:
;   Plane 0: $00020000  $FF00 every word  ("highest bit toggles per word")
;   Plane 1: $00021800  $F0F0 every word  ("every nibble")
;   Plane 2: $00023000  $CCCC every word  ("every 2 bits")
;   Plane 3: $00024800  $AAAA every word  ("every other bit")
;
; The XOR of these gives a 16-color repeating pattern across each row.
; All rows are identical, so the screen shows 16 vertical bands wide.

        .org $400

start:
        ; ---- fill plane 0..3 ----
        bsr     fill_plane0
        bsr     fill_plane1
        bsr     fill_plane2
        bsr     fill_plane3

        ; ---- palette: 16 colors spanning the spectrum ----
        ;   Each COLORxx is a 12-bit RGB value in the low 12 bits of a long.
        move.l  #$00000000, $00FE0180          ; COLOR00 = black
        move.l  #$00000F00, $00FE0184          ; COLOR01 = red
        move.l  #$000000F0, $00FE0188          ; COLOR02 = green
        move.l  #$0000000F, $00FE018C          ; COLOR03 = blue
        move.l  #$00000FF0, $00FE0190          ; COLOR04 = yellow
        move.l  #$00000F0F, $00FE0194          ; COLOR05 = magenta
        move.l  #$000000FF, $00FE0198          ; COLOR06 = cyan
        move.l  #$00000FFF, $00FE019C          ; COLOR07 = white

        move.l  #$00000F80, $00FE01A0          ; COLOR08 = orange
        move.l  #$00000FF8, $00FE01A4          ; COLOR09 = pale yellow
        move.l  #$00000F88, $00FE01A8          ; COLOR0A = pink
        move.l  #$0000088F, $00FE01AC          ; COLOR0B = pale blue
        move.l  #$00000080, $00FE01B0          ; COLOR0C = brown-green
        move.l  #$0000080F, $00FE01B4          ; COLOR0D = teal
        move.l  #$00000800, $00FE01B8          ; COLOR0E = dark red
        move.l  #$00000888, $00FE01BC          ; COLOR0F = mid gray

        ; ---- BPLCON0 = 4 planes, no HAM, no DPF ----
        move.l  #$00004000, $00FE0100          ; bits [14:12] = 100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108

        move.l  #$00020000, $00FE0110
        move.l  #$00021800, $00FE0114
        move.l  #$00023000, $00FE0118
        move.l  #$00024800, $00FE011C
        move.l  #0,         $00FE0128
        move.l  #0,         $00FE012C

main_loop:
        move.l  #1, $00FE0140                  ; DENRUN
wait:   move.l  $00FE0144, D0                  ; DENSTAT
        andi.l  #1, D0
        bne     wait
        ; pacing delay
        move.l  #50000, D0
pace:   subq.l  #1, D0
        bne     pace
        bra     main_loop

; -------------------------------------------------------------------
; fill_planeN: writes a constant 32-bit pattern to all 1536 longs of
;              the corresponding bitplane region.
; -------------------------------------------------------------------
fill_plane0:
        move.l  #$00020000, A0
        move.l  #1536, D0
        move.l  #$FF00FF00, D1
        bra     fill_common

fill_plane1:
        move.l  #$00021800, A0
        move.l  #1536, D0
        move.l  #$F0F0F0F0, D1
        bra     fill_common

fill_plane2:
        move.l  #$00023000, A0
        move.l  #1536, D0
        move.l  #$CCCCCCCC, D1
        bra     fill_common

fill_plane3:
        move.l  #$00024800, A0
        move.l  #1536, D0
        move.l  #$AAAAAAAA, D1
        bra     fill_common

fill_common:
        move.l  D1, (A0)+
        subq.l  #1, D0
        bne     fill_common
        rts
