; ATTACHED-pair sprite test.
; Sprite 0 (even partner): 2bpp, base color = $00.
;   data_a = $F000, data_b = $0F00
; Sprite 1 (odd, ATT set):  2bpp, base color = $00.
;   data_a = $00F0, data_b = $000F
;
; Combined ATT pixel value v = {S1.b, S1.a, S0.b, S0.a}:
;   x=0..3   (bits 15..12): S1.a=0, S1.b=0, S0.a=1, S0.b=0 -> v = 0001 = 1
;   x=4..7   (bits 11..8 ): S1.a=0, S1.b=0, S0.a=0, S0.b=1 -> v = 0010 = 2
;   x=8..11  (bits  7..4 ): S1.a=1, S1.b=0, S0.a=0, S0.b=0 -> v = 0100 = 4
;   x=12..15 (bits  3..0 ): S1.a=0, S1.b=1, S0.a=0, S0.b=0 -> v = 1000 = 8
; Base color = $00 -> resulting palette index = v.
;
; Palette: COLOR00=blue (transparent placeholder), COLOR01=red, COLOR02=green,
; COLOR04=yellow, COLOR08=magenta.
; Plane 0 is zero -> playfield = COLOR00 -> behind sprites (BPLCON2=$24).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- zero plane 0 ----
        move.l  #$00020000, A0
        move.l  #1536, D1
zp:     move.l  #0, (A0)+
        subq.l  #1, D1
        bne     zp

        ; Sprite 0 data at $21000: one 32-bit row = {$F000, $0F00}
        move.l  #$F0000F00, $00021000
        ; Sprite 1 data at $21100: {$00F0, $000F}
        move.l  #$00F0000F, $00021100

        ; Denise
        move.l  #$00001000, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #$00000024, $00FE0108
        move.l  #$00020000, $00FE0110
        move.l  #0,         $00FE0128

        ; Palette
        move.l  #$0000000F, $00FE0180   ; COLOR00 = blue (transparent + bg)
        move.l  #$00000F00, $00FE0184   ; COLOR01 = red
        move.l  #$000000F0, $00FE0188   ; COLOR02 = green
        move.l  #$00000FF0, $00FE0190   ; COLOR04 = yellow
        move.l  #$00000F0F, $00FE01A0   ; COLOR08 = magenta

        ; Sprite 0: at (50,30), H=1, BPP2=1 (bit 6), no ATT, base=0.
        ;   ctl = $1E_32_01_40 = $1E320140
        move.l  #$00021000, $00FE0130
        move.l  #$1E320140, $00FE0134

        ; Sprite 1: at (50,30), H=1, BPP2=1, ATT=1, base=0.
        ;   ctl = $1E_32_01_C0 = $1E3201C0
        move.l  #$00021100, $00FE0138
        move.l  #$1E3201C0, $00FE013C

        ; SPRENA = sprites 0 and 1
        move.l  #$00000003, $00FE0170

        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ---- check pixel (50,30): v=1 -> red ($E0) ----
        move.l  #0, D3
        move.b  $00011E32, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_a

        ; ---- check pixel (54,30): v=2 -> green ($1C) ----
        move.l  #0, D3
        move.b  $00011E36, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_b

        ; ---- check pixel (58,30): v=4 -> yellow ($FC) ----
        move.l  #0, D3
        move.b  $00011E3A, D3
        andi.l  #$FF, D3
        cmpi.l  #$FC, D3
        bne     fail_c

        ; ---- check pixel (62,30): v=8 -> magenta ($E3) ----
        ;   magenta $F0F -> R=E, G=0, B=3 -> 111_000_11 = $E3
        move.l  #0, D3
        move.b  $00011E3E, D3
        andi.l  #$FF, D3
        cmpi.l  #$E3, D3
        bne     fail_d

        stop    #0

fail_a: stop #$D830
fail_b: stop #$D831
fail_c: stop #$D832
fail_d: stop #$D833
