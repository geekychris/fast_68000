; 2bpp sprite test.
; Sprite 0 at (50, 30), height 1, BPP2=1, color base = 4.
; Row 0 layout (32 bits = data_a:data_b):
;   data_a = $FF00 (bits 15..8 = 1, bits 7..0 = 0)
;   data_b = $F0F0 (bits 15..12 = 1, 11..8 = 0, 7..4 = 1, 3..0 = 0)
;
;   pixel value v = (data_b_bit << 1) | data_a_bit (per pixel):
;     x=0..3   (bits 15..12): a=1, b=1 -> v=3 -> colors[4+3] = colors[7]
;     x=4..7   (bits 11..8 ): a=1, b=0 -> v=1 -> colors[4+1] = colors[5]
;     x=8..11  (bits  7..4 ): a=0, b=1 -> v=2 -> colors[4+2] = colors[6]
;     x=12..15 (bits  3..0 ): a=0, b=0 -> v=0 -> transparent (background COLOR00)
;
; Background = nplanes=1 + plane 0 zero, so playfield = COLOR00 everywhere.
; Palette: COLOR00=blue, COLOR05=red, COLOR06=green, COLOR07=yellow.

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

        ; ---- 2bpp sprite data: one row, 32-bit word {data_a, data_b} ----
        move.l  #$FF00F0F0, $00021000

        ; ---- Denise: nplanes=1, indexed ----
        move.l  #$00001000, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #$00000024, $00FE0108   ; PF1P=PF2P=4 (sprites in front)
        move.l  #$00020000, $00FE0110
        move.l  #0,         $00FE0128

        ; Palette
        move.l  #$0000000F, $00FE0180   ; COLOR00 = blue
        move.l  #$00000F00, $00FE0194   ; COLOR05 = red    (idx 5 -> $94)
        move.l  #$000000F0, $00FE0198   ; COLOR06 = green
        move.l  #$00000FF0, $00FE019C   ; COLOR07 = yellow

        ; Sprite 0: at (50, 30), height 1, BPP2=1, color base=4.
        ; SPRxCTL = {YPOS, XPOS, HEIGHT, ATT, BPP2, _, COLOR_IDX[4:0]}
        ;         = ($1E << 24) | ($32 << 16) | ($01 << 8) | $44
        ;         = $1E320144
        move.l  #$00021000, $00FE0130
        move.l  #$1E320144, $00FE0134
        move.l  #$00000001, $00FE0170   ; SPRENA

        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ---- check pixel (50, 30): v=3 -> COLOR07 yellow ----
        ;   yellow RGB332: R=G=$E, B=$0 -> binary 111_111_00 = $FC
        move.l  #0, D3
        move.b  $00011E32, D3
        andi.l  #$FF, D3
        cmpi.l  #$FC, D3
        bne     fail_a

        ; ---- check pixel (54, 30): v=1 -> COLOR05 red ($E0) ----
        move.l  #0, D3
        move.b  $00011E36, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_b

        ; ---- check pixel (58, 30): v=2 -> COLOR06 green ($1C) ----
        move.l  #0, D3
        move.b  $00011E3A, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_c

        ; ---- check pixel (62, 30): v=0 -> transparent -> COLOR00 blue ($03) ----
        move.l  #0, D3
        move.b  $00011E3E, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_d

        stop    #0

fail_a: stop #$D820
fail_b: stop #$D821
fail_c: stop #$D822
fail_d: stop #$D823
