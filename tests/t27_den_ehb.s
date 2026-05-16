; Denise EHB (Extra Half-Brite) test.
;
; With 6 planes + !HAM + !DPF, bit 5 of each pixel value selects
; half-intensity.  Bits [4:0] = palette index (0..31, of which we use 0..15).
;
; Setup pixels in row 0:
;   pixel 0: V=1   (bit 5 = 0, low = 1)  -> palette[1] = $888 -> rgb332 $92
;   pixel 1: V=33  (bit 5 = 1, low = 1)  -> half-brite of $888 = $444 -> $49
;   pixel 2..15: V=0                     -> palette[0] = $000 -> $00
;
; Long at FB[$10000] should be $92490000.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Plane patterns for pixels 0 and 1:
        ;   Plane 0 bit 15 (pixel 0) = 1, bit 14 (pixel 1) = 1 -> word 0 = $C000.
        ;   Plane 5 bit 14 (pixel 1) = 1                       -> word 0 = $4000.
        ; All other planes: 0.
        move.l #$C0000000, $00020000    ; plane 0 row 0 word 0 = $C000
        move.l #$40000000, $00027800    ; plane 5 row 0 word 0 = $4000

        ; BPLCON0 = nplanes=6, no HAM, no DPF -> bits [14:12] = 110 -> $6000
        move.l #$00006000, $00FE0100
        move.l #0,         $00FE0104
        move.l #0,         $00FE0108

        ; Plane pointers
        move.l #$00020000, $00FE0110
        move.l #$00021800, $00FE0114
        move.l #$00023000, $00FE0118
        move.l #$00024800, $00FE011C
        move.l #$00026000, $00FE0120
        move.l #$00027800, $00FE0124
        move.l #0, $00FE0128
        move.l #0, $00FE012C

        ; Palette: COLOR00=$000 black, COLOR01=$888 mid-gray
        move.l #0,         $00FE0180
        move.l #$00000888, $00FE0184

        ; Kick
        move.l #1, $00FE0140

wait:   move.l $00FE0144, D2
        andi.l #1, D2
        bne    wait

        ; Verify long at FB[$10000].
        move.l $00010000, D3
        cmpi.l #$92490000, D3
        bne    fail
        stop   #0

fail:   stop #$de41
