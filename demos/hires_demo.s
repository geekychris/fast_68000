; Hires demo (BPLCON0[15] = 1).
;
; 4-plane hires display (32-entry palette) showing a 512-wide
; gradient bar that you couldn't fit horizontally in lores.
;
; Each pixel x in [0, 511] has palette index = (x >> 4) & 31 ->
; 32 colour bars, each 16 pixels wide.  At 512 across, the
; on-screen result is a full rainbow spanning the entire row.
;
; Build/run with:  make demo-hires
; (this target builds the sim with FB_W=512 so the SDL renderer
;  shows the full hires display).
;
; Memory layout:
;   $00020000  BPL1   ($1800 bytes; hires row stride 64 -> 64*192 = 12288)
;   $00021800  BPL2
;   $00023000  BPL3
;   $00024800  BPL4
;   $00040000  per-plane row templates (64 bytes each)

        .org $400

start:
        ; ---- per-plane row templates (64 bytes each) ----
        ; pixel x's palette index = (x >> 4) & 31.  Each 16-pixel
        ; word group of the row corresponds to one palette index;
        ; within a word group all 16 bits are identical (the bit-
        ; layout encodes the index bit-by-bit across the 4 planes).

        ; BPL1: bit 0 of (x>>4) -> alternating words 0, $FFFF, 0, $FFFF, ...
        ;       (32 words = 64 bytes per row, but plane is 1bpp so the
        ;        word = $FFFF or $0000 to set all 16 pixels of that wg.)
        move.l  #$0000FFFF, $00040000
        move.l  #$0000FFFF, $00040004
        move.l  #$0000FFFF, $00040008
        move.l  #$0000FFFF, $0004000C
        move.l  #$0000FFFF, $00040010
        move.l  #$0000FFFF, $00040014
        move.l  #$0000FFFF, $00040018
        move.l  #$0000FFFF, $0004001C
        move.l  #$0000FFFF, $00040020
        move.l  #$0000FFFF, $00040024
        move.l  #$0000FFFF, $00040028
        move.l  #$0000FFFF, $0004002C
        move.l  #$0000FFFF, $00040030
        move.l  #$0000FFFF, $00040034
        move.l  #$0000FFFF, $00040038
        move.l  #$0000FFFF, $0004003C

        ; BPL2: bit 1 of (x>>4) -> period 2 in word groups.
        ;       words 00 11 00 11 ... = $0000 $0000 $FFFF $FFFF
        move.l  #$00000000, $00040040
        move.l  #$FFFFFFFF, $00040044
        move.l  #$00000000, $00040048
        move.l  #$FFFFFFFF, $0004004C
        move.l  #$00000000, $00040050
        move.l  #$FFFFFFFF, $00040054
        move.l  #$00000000, $00040058
        move.l  #$FFFFFFFF, $0004005C
        move.l  #$00000000, $00040060
        move.l  #$FFFFFFFF, $00040064
        move.l  #$00000000, $00040068
        move.l  #$FFFFFFFF, $0004006C
        move.l  #$00000000, $00040070
        move.l  #$FFFFFFFF, $00040074
        move.l  #$00000000, $00040078
        move.l  #$FFFFFFFF, $0004007C

        ; BPL3: bit 2 of (x>>4) -> period 4. words 0000 1111 0000 1111
        move.l  #$00000000, $00040080
        move.l  #$00000000, $00040084
        move.l  #$FFFFFFFF, $00040088
        move.l  #$FFFFFFFF, $0004008C
        move.l  #$00000000, $00040090
        move.l  #$00000000, $00040094
        move.l  #$FFFFFFFF, $00040098
        move.l  #$FFFFFFFF, $0004009C
        move.l  #$00000000, $000400A0
        move.l  #$00000000, $000400A4
        move.l  #$FFFFFFFF, $000400A8
        move.l  #$FFFFFFFF, $000400AC
        move.l  #$00000000, $000400B0
        move.l  #$00000000, $000400B4
        move.l  #$FFFFFFFF, $000400B8
        move.l  #$FFFFFFFF, $000400BC

        ; BPL4: bit 3 of (x>>4) -> period 8. 8 words 0 then 8 words 1.
        move.l  #$00000000, $000400C0
        move.l  #$00000000, $000400C4
        move.l  #$00000000, $000400C8
        move.l  #$00000000, $000400CC
        move.l  #$FFFFFFFF, $000400D0
        move.l  #$FFFFFFFF, $000400D4
        move.l  #$FFFFFFFF, $000400D8
        move.l  #$FFFFFFFF, $000400DC
        move.l  #$00000000, $000400E0
        move.l  #$00000000, $000400E4
        move.l  #$00000000, $000400E8
        move.l  #$00000000, $000400EC
        move.l  #$FFFFFFFF, $000400F0
        move.l  #$FFFFFFFF, $000400F4
        move.l  #$FFFFFFFF, $000400F8
        move.l  #$FFFFFFFF, $000400FC

        ; ---- We need plane bit 4 of (x>>4) to cover the upper 16 palette
        ;      indices.  4-plane only goes to 16 palette indices.  Use
        ;      EHB mode to get 32 colours, or 5 planes... we'll do 5 planes.
        ;      Actually for simplicity stick with 4 planes -> 16 colour
        ;      bars, each repeating twice across the 32-bar layout.

        ; ---- fill each plane buffer with 192 copies of its template ----
        moveq   #0, D0
        bsr     fill_plane
        moveq   #1, D0
        bsr     fill_plane
        moveq   #2, D0
        bsr     fill_plane
        moveq   #3, D0
        bsr     fill_plane

        ; ---- palette: 16 distinct hues ----
        move.l  #$00000000, $00FE0180   ; COLOR00 black
        move.l  #$00000F00, $00FE0184   ; red
        move.l  #$00000F80, $00FE0188   ; orange
        move.l  #$00000FF0, $00FE018C   ; yellow
        move.l  #$00000CF0, $00FE0190   ; lime
        move.l  #$000008F0, $00FE0194   ; green
        move.l  #$000004F8, $00FE0198   ; teal
        move.l  #$000000FF, $00FE019C   ; cyan
        move.l  #$000000CF, $00FE01A0   ; sky
        move.l  #$0000008F, $00FE01A4   ; light blue
        move.l  #$0000000F, $00FE01A8   ; blue
        move.l  #$0000080F, $00FE01AC   ; purple
        move.l  #$00000F0F, $00FE01B0   ; magenta
        move.l  #$00000F08, $00FE01B4   ; pink
        move.l  #$00000F04, $00FE01B8   ; salmon
        move.l  #$00000FFF, $00FE01BC   ; white

        ; ---- BPLCON0 = HIRES | nplanes=4 = $9000 | $4000 = $D000 ----
        move.l  #$0000D000, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108
        move.l  #$00020000, $00FE0110
        move.l  #$00021800, $00FE0114
        move.l  #$00023000, $00FE0118
        move.l  #$00024800, $00FE011C
        move.l  #0,         $00FE0128
        move.l  #0,         $00FE012C

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
wait:   move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait

        stop    #0

; ============================================================
; fill_plane(D0 = plane index): blits the 64-byte template at $40000 + D0*64
; into the plane buffer at $20000 + D0*$1800 over 192 rows of 32 words each.
; ============================================================
fill_plane:
fp_w0:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w0

        ; src = $40000 + D0 * 64
        move.l  D0, D1
        lsl.l   #6, D1
        addi.l  #$00040000, D1

        ; dst = $20000 + D0 * $1800
        move.l  D0, D2
        moveq   #11, D3
        lsl.l   D3, D2                  ; D0 * $800
        move.l  D0, D3
        moveq   #12, D4
        lsl.l   D4, D3                  ; D0 * $1000
        add.l   D3, D2                  ; D0 * $1800
        addi.l  #$00020000, D2

        move.l  #$F0000009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  D1,         $00FE000C
        move.l  D2,         $00FE0018
        move.l  #-64,       $00FE001C   ; rewind APT by 64 each row
        move.l  #0,         $00FE0028
        move.l  #$00003020, $00FE0038   ; (192<<6) | 32 words/row

fp_w1:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w1
        rts
