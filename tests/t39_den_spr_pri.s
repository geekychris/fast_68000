; Sprite-vs-playfield priority via BPLCON2[5:3] (PF1P).
;
; Background plane 0 is all-ones -> every playfield pixel is COLOR01 (red).
; Sprite 0 is placed at (50, 30), 4 rows of $FFFF (solid 16-px green bar).
;
; Run 1: BPLCON2 = 0  -> PF1P = 0, sprite-pair 0 NOT < 0, playfield wins.
;        Pixel (50,30) should stay red.
; Run 2: BPLCON2 = $24 (PF1P=4) -> sprite pair 0 < 4, sprite wins.
;        Pixel (50,30) should be green.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- fill plane 0 with all-ones (so playfield colour = COLOR01 everywhere) ----
        move.l  #$00005000, A0
        move.l  #1536, D1
fill_p: move.l  #$FFFFFFFF, (A0)+
        subq.l  #1, D1
        bne     fill_p

        ; ---- sprite data: 4 rows of $FFFF at $6000 (rows 0/1 pack into one long, 2/3 next) ----
        move.l  #$FFFFFFFF, $00006000
        move.l  #$FFFFFFFF, $00006004

        ; ---- common Denise config: nplanes=1 ----
        move.l  #$00001000, $00FE0100   ; BPLCON0 = nplanes=1
        move.l  #0,         $00FE0104   ; BPLCON1
        move.l  #$00005000, $00FE0110   ; BPL1PT
        move.l  #0,         $00FE0128   ; BPL1MOD
        ; Palette: COLOR00 = blue ($00F), COLOR01 = red ($F00), COLOR07 = green ($0F0)
        move.l  #$0000000F, $00FE0180
        move.l  #$00000F00, $00FE0184
        move.l  #$000000F0, $00FE019C
        ; Sprite 0: at (50, 30), height 4, color 7
        move.l  #$00006000, $00FE0130
        move.l  #$1E320407, $00FE0134
        move.l  #$00000001, $00FE0170   ; SPRENA = sprite 0

        ; ---- Run 1: BPLCON2 = 0 -> playfield wins ----
        move.l  #0, $00FE0108
        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1
        ; Check pixel (50,30) = $11E32 should be red ($E0)
        move.l  #0, D3
        move.b  $00011E32, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_pf

        ; ---- Run 2: BPLCON2 = $24 (PF1P=4) -> sprite wins ----
        move.l  #$00000024, $00FE0108
        move.l  #1, $00FE0140
w2:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w2
        ; Check pixel (50,30) = $11E32 should be green ($1C)
        move.l  #0, D3
        move.b  $00011E32, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_spr

        stop    #0

fail_pf:  stop #$D8A1
fail_spr: stop #$D8A2
