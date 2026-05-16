; Denise HAM6 test.
;
; Sets up six bitplanes with very small pixel data at the top-left of
; row 0, exercising each of HAM's four control modes:
;
;   Pixel 0: V = 000001  (control=00, data=$1)  USE COLOR01
;   Pixel 1: V = 110001  (control=11, data=$1)  modify G to $1
;   Pixel 2: V = 100001  (control=10, data=$1)  modify R to $1
;   Pixel 3: V = 010001  (control=01, data=$1)  modify B to $1
;
; With COLOR00 = $000 and COLOR01 = $789:
;   pixel 0: cur = $789  rgb332 = $72  prev = $789
;   pixel 1: cur = $719  rgb332 = $62  prev = $719
;   pixel 2: cur = $119  rgb332 = $02  prev = $119
;   pixel 3: cur = $111  rgb332 = $00  prev = $111
;
; Long at FB[$10000] should be $72620200.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Plane word 0 settings to encode pixels 0..3 across all 6 planes.
        ;   plane 0 word 0 = $F000  (pixels 0..3 all have bit 0 = 1)
        ;   plane 1..3 word 0 = $0000
        ;   plane 4 word 0 = $5000  (pixels 1 and 3 have bit 4 = 1)
        ;   plane 5 word 0 = $6000  (pixels 1 and 2 have bit 5 = 1)
        move.l #$F0000000, $00020000   ; plane 0
        ; plane 1..3 already zero
        move.l #$50000000, $00026000   ; plane 4
        move.l #$60000000, $00027800   ; plane 5

        ; BPLCON0 = nplanes=6, HAM=1 -> bits [14:12]=110, bit 11=1 -> $6800
        move.l #$00006800, $00FE0100
        move.l #0,         $00FE0104
        move.l #0,         $00FE0108

        ; Bitplane pointers
        move.l #$00020000, $00FE0110   ; BPL1PT (plane 0)
        move.l #$00021800, $00FE0114   ; BPL2PT (plane 1)
        move.l #$00023000, $00FE0118   ; BPL3PT (plane 2)
        move.l #$00024800, $00FE011C   ; BPL4PT (plane 3)
        move.l #$00026000, $00FE0120   ; BPL5PT (plane 4)
        move.l #$00027800, $00FE0124   ; BPL6PT (plane 5)

        move.l #0, $00FE0128           ; BPL1MOD
        move.l #0, $00FE012C           ; BPL2MOD

        ; Palette: COLOR00=$000, COLOR01=$789, rest left default (0).
        move.l #0,         $00FE0180
        move.l #$00000789, $00FE0184

        ; Trigger Denise.
        move.l #1, $00FE0140

wait:   move.l $00FE0144, D2
        andi.l #1, D2
        bne    wait

        ; Verify pixels 0..3 in the first long of the chunky FB.
        move.l $00010000, D3
        cmpi.l #$72620200, D3
        bne    fail1

        ; Pixel 4 should fall back to USE palette[0] (V=000000) -> $000 -> $00.
        ; The whole next long should be zero.
        move.l $00010004, D4
        cmpi.l #0, D4
        bne    fail2

        ; Row 1 pixel 0: HAM prev resets to COLOR00 ($000) at the start of
        ; each row. Plane data there is all 0 -> V=0 -> USE palette[0] -> $00.
        move.l $00010100, D5
        cmpi.l #0, D5
        bne    fail3

        stop   #0

fail1:  stop #$de31
fail2:  stop #$de32
fail3:  stop #$de33
