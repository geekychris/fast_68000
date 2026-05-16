; Denise dual-playfield test.
;
; 2-plane DPF: plane 0 = PF1 (1 bit, 2 colors from palette 0..1),
;              plane 1 = PF2 (1 bit, 2 colors from palette 8..9).
;
; Row 0 setup:
;   pixel 0: PF1=1, PF2=0  -> PF1 visible, palette[1]
;   pixel 1: PF1=0, PF2=1  -> PF2 visible, palette[9]
;   pixel 2: PF1=1, PF2=1  -> default PF1 priority, palette[1]
;   pixel 3: PF1=0, PF2=0  -> palette[0]
;
; Plane bits for each pixel position (bit 15 - px):
;   plane 0 word 0:  pixel 0=1, pixel 2=1, others 0    -> bit 15, 13 set = $A000
;   plane 1 word 0:  pixel 1=1, pixel 2=1, others 0    -> bit 14, 13 set = $6000
;
; Palette:
;   COLOR00 = $000 -> rgb332 $00
;   COLOR01 = $F0F -> R=15->b111, G=0->b000, B=15->b11 = 111_000_11 = $E3
;   COLOR08 = $0F0 -> R=0,G=15,B=0 -> 000_111_00 = $1C
;   COLOR09 = $0FF -> R=0,G=15,B=15 -> 000_111_11 = $1F
;
; Expected long at $10000: $E3 1F E3 00 = $E31FE300.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Plane 0 row 0 word 0 = $A000 (bit 15, 13)
        ; Plane 1 row 0 word 0 = $6000 (bit 14, 13)
        move.l #$A0000000, $00020000
        move.l #$60000000, $00021800

        ; BPLCON0: nplanes=2 (bits [14:12]=010), DPF=1 (bit 10) -> $2400
        move.l #$00002400, $00FE0100
        move.l #0,         $00FE0104
        move.l #0,         $00FE0108            ; BPLCON2 = 0 (PF1 priority)

        move.l #$00020000, $00FE0110
        move.l #$00021800, $00FE0114
        move.l #0, $00FE0128
        move.l #0, $00FE012C

        ; Palette
        move.l #0,         $00FE0180            ; COLOR00 = $000
        move.l #$00000F0F, $00FE0184            ; COLOR01 = $F0F
        move.l #$000000F0, $00FE01A0            ; COLOR08 = $0F0 (offset $20 = 8 entries)
        move.l #$000000FF, $00FE01A4            ; COLOR09 = $0FF

        move.l #1, $00FE0140

w1:     move.l $00FE0144, D2
        andi.l #1, D2
        bne    w1

        move.l $00010000, D3
        cmpi.l #$E31FE300, D3
        bne    fail1

        ; Now flip priority: PF2 in front.  Pixel 2 (where both PFs are set)
        ; should now show PF2.  Expected long: $E3 1F 1F 00 = $E31F1F00.
        move.l #$00000040, $00FE0108            ; BPLCON2 bit 6 = 1 -> PF2 priority
        move.l #1, $00FE0140

w2:     move.l $00FE0144, D2
        andi.l #1, D2
        bne    w2

        move.l $00010000, D3
        cmpi.l #$E31F1F00, D3
        bne    fail2

        stop   #0

fail1:  stop #$de51
fail2:  stop #$de52
