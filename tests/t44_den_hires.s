; Hires mode (BPLCON0[15] = 1).
;
; nplanes=1, plane 0 row stride = 64 bytes (32 wgs * 2 bytes/word).
; Each row's bytes 0..31 are zero (left half of screen: blue).
; Each row's bytes 32..63 are $FF (right half of screen: red).
;
; Palette: COLOR00 = blue ($03), COLOR01 = red ($E0).
;
; Hires FB stride = 32 wg * 16 bytes/wg = 512 bytes/row starting at $10000:
;   pixel (0,   0) at $10000 -> blue
;   pixel (255, 0) at $100FF -> blue
;   pixel (256, 0) at $10100 -> red
;   pixel (511, 0) at $101FF -> red
;   pixel (0,   1) at $10200 -> blue

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        move.l  #$00020000, A0
        move.l  #3072, D1
zp:     move.l  #0, (A0)+
        subq.l  #1, D1
        bne     zp

        ; Fill bytes 32..63 of each 64-byte plane row with $FF.
        move.l  #$00020020, A0
        move.l  #192, D1
row_lp: move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        move.l  #$FFFFFFFF, (A0)+
        adda.l  #32, A0
        subq.l  #1, D1
        bne     row_lp

        ; Denise: BPLCON0 = HIRES | nplanes=1 = $9000
        move.l  #$00009000, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108
        move.l  #$00020000, $00FE0110
        move.l  #0,         $00FE0128
        move.l  #$0000000F, $00FE0180   ; COLOR00 = blue
        move.l  #$00000F00, $00FE0184   ; COLOR01 = red

        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ---- check pixel (0, 0) at $10000: blue ($03) ----
        move.l  #0, D3
        move.b  $00010000, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_a

        ; ---- check pixel (255, 0) at $100FF: blue ----
        move.l  #0, D3
        move.b  $000100FF, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_b

        ; ---- check pixel (256, 0) at $10100: red ($E0) (only present in hires) ----
        move.l  #0, D3
        move.b  $00010100, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_c

        ; ---- check pixel (511, 0) at $101FF: red ----
        move.l  #0, D3
        move.b  $000101FF, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_d

        ; ---- check pixel (0, 1) at $10200: blue (row 1, hires stride 512) ----
        move.l  #0, D3
        move.b  $00010200, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_e

        stop    #0

fail_a: stop #$D900
fail_b: stop #$D901
fail_c: stop #$D902
fail_d: stop #$D903
fail_e: stop #$D904
