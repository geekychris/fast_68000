; Denise sprite test: nplanes=1 background + one sprite overlay.
;
; Background: nplanes=1, COLOR00 = blue, COLOR01 = red. Plane 0 buffer is
;   left empty (all zeros) -> background is solid blue everywhere.
;
; Sprite 0:
;   X = 50, Y = 30, height = 4
;   color index = 7 (palette entry 7 -> green)
;   data (one 16-bit word per row, MSB = leftmost pixel):
;     row 0:  $FFFF (all 16 pixels on)
;     row 1:  $0000 (nothing)
;     row 2:  $8001 (only leftmost and rightmost on)
;     row 3:  $FFFF
;
; Assertions:
;   pixel (50, 30) -> green   (sprite row 0, bit 15)
;   pixel (65, 30) -> green   (sprite row 0, bit 0)
;   pixel (50, 31) -> blue    (sprite row 1, bit 15 = 0)
;   pixel (50, 32) -> green   (sprite row 2, bit 15)
;   pixel (60, 32) -> blue    (sprite row 2, middle bits = 0)
;   pixel (49, 30) -> blue    (just outside sprite, left edge)
;   pixel (66, 30) -> blue    (just outside sprite, right edge)
;   pixel (50, 34) -> blue    (just past sprite height)

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- pre-fill chunky FB with sentinel to detect non-runs ----
        move.l  #$00010000, A0
        move.l  #12288, D1
fill0:  move.l  #$5A5A5A5A, (A0)+
        subq.l  #1, D1
        bne     fill0

        ; ---- zero plane 0 ----
        move.l  #$00005000, A0
        move.l  #1536, D1
zero0:  move.l  #0, (A0)+
        subq.l  #1, D1
        bne     zero0

        ; ---- write sprite data at $00006000 ----
        ;   row 0 = $FFFF, row 1 = $0000  ->  long $FFFF0000
        ;   row 2 = $8001, row 3 = $FFFF  ->  long $8001FFFF
        move.l  #$FFFF0000, $00006000
        move.l  #$8001FFFF, $00006004

        ; ---- Denise registers ----
        ; BPLCON0 = nplanes=1 -> $1000
        move.l  #$00001000, $00FE0100
        move.l  #0,         $00FE0104     ; BPLCON1
        move.l  #0,         $00FE0108     ; BPLCON2
        move.l  #$00005000, $00FE0110     ; BPL1PT
        move.l  #0,         $00FE0128     ; BPL1MOD

        ; Palette: COLOR00 = blue ($00F), COLOR01 = red ($F00),
        ; COLOR07 = green ($0F0)
        move.l  #$0000000F, $00FE0180
        move.l  #$00000F00, $00FE0184
        move.l  #$000000F0, $00FE019C     ; COLOR07 ($80 + 7*4 = $9C)

        ; Sprite 0: SPRxPT = $6000, SPRxCTL = Y=30, X=50, H=4, COLOR=7
        ;   ctl = (30<<24) | (50<<16) | (4<<8) | 7
        ;       =  $1E_32_04_07
        move.l  #$00006000, $00FE0130     ; SPR0PT
        move.l  #$1E320407, $00FE0134     ; SPR0CTL

        ; SPRENA: enable sprite 0 only
        move.l  #$00000001, $00FE0170

        ; Trigger Denise
        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ============================================================
        ; Verify pixels.  FB[y*256 + x] is the byte at $10000 + y*256 + x.
        ;
        ; Expected RGB332 values:
        ;   blue  ($00F) -> $03
        ;   red   ($F00) -> $E0
        ;   green ($0F0) -> $1C
        ; ============================================================

        ; (50, 30) -> green: $10000 + 30*256 + 50 = $11E32
        move.l  #0, D3
        move.b  $00011E32, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_a

        ; (65, 30) -> green: $10000 + 30*256 + 65 = $11E41
        move.l  #0, D3
        move.b  $00011E41, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_b

        ; (50, 31) -> blue (sprite row 1 = $0000): $10000 + 31*256 + 50 = $11F32
        move.l  #0, D3
        move.b  $00011F32, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_c

        ; (50, 32) -> green (sprite row 2 = $8001, bit 15): $10000 + 32*256 + 50 = $12032
        move.l  #0, D3
        move.b  $00012032, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_d

        ; (60, 32) -> blue (sprite row 2 middle): $12032 + 10 = $1203C
        move.l  #0, D3
        move.b  $0001203C, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_e

        ; (49, 30) -> blue (just outside sprite left edge): $11E31
        move.l  #0, D3
        move.b  $00011E31, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_f

        ; (66, 30) -> blue (just outside sprite right edge): $11E42
        move.l  #0, D3
        move.b  $00011E42, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_g

        ; (50, 34) -> blue (past sprite height): $10000 + 34*256 + 50 = $12232
        move.l  #0, D3
        move.b  $00012232, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_h

        stop    #0

fail_a: stop #$de41
fail_b: stop #$de42
fail_c: stop #$de43
fail_d: stop #$de44
fail_e: stop #$de45
fail_f: stop #$de46
fail_g: stop #$de47
fail_h: stop #$de48
