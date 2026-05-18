; Showcase demo: exercises HAM8 + DIW + 2bpp attached sprite + sprite priority.
;
; - HAM8 background: 8 bitplanes laid out so each pixel x's HAM8 control
;   bits are 00 (palette lookup) and data bits encode x mod 64.  Combined
;   with a 64-entry rainbow palette this paints 4 rainbow bars across each
;   row.
; - Display window (DIWSTRT/DIWSTOP): inset by 32 pixels on the sides and
;   16 rows top/bottom, so the outer border shows COLOR00 (dark blue).
; - Attached sprite pair (sprites 0+1 attached, 2bpp each -> 4bpp / 15
;   colours + transparent).  Drawn as a small 16x16 multi-colour icon.
;   The sprite slides horizontally each frame.
; - Sprite-vs-playfield priority: PF1P/PF2P = 4 (sprites in front).
;
; Memory layout:
;   $20000  BPL1   ; 8 bitplane buffers, $1800 bytes each
;   $21800  BPL2
;   $23000  BPL3 (data bit 0)
;   $24800  BPL4 (data bit 1)
;   $26000  BPL5 (data bit 2)
;   $27800  BPL6 (data bit 3)
;   $29000  BPL7 (data bit 4)
;   $2A800  BPL8 (data bit 5)
;   $30000  Sprite 0 data (16 rows * 4 bytes = 64 bytes)
;   $30040  Sprite 1 data
;   $40000  Per-plane row templates (32 bytes each x 8 planes)

        .org $400

start:
        ; ---- per-plane row templates at $40000 ----
        ; Each template is 32 bytes (16 words) covering one row.
        ; Use blitter later to clone across 192 rows.

        ; BPL1 / BPL2 / control bits = 0 -> ctl=00 (palette lookup).
        move.l  #0, $00040000           ; BPL1 template at $40000
        move.l  #0, $00040004
        move.l  #0, $00040008
        move.l  #0, $0004000C
        move.l  #0, $00040010
        move.l  #0, $00040014
        move.l  #0, $00040018
        move.l  #0, $0004001C

        move.l  #0, $00040020           ; BPL2 template at $40020
        move.l  #0, $00040024
        move.l  #0, $00040028
        move.l  #0, $0004002C
        move.l  #0, $00040030
        move.l  #0, $00040034
        move.l  #0, $00040038
        move.l  #0, $0004003C

        ; BPL3: data bit 0 = pixel_x & 1 -> word = $5555 every word.
        move.l  #$55555555, $00040040
        move.l  #$55555555, $00040044
        move.l  #$55555555, $00040048
        move.l  #$55555555, $0004004C
        move.l  #$55555555, $00040050
        move.l  #$55555555, $00040054
        move.l  #$55555555, $00040058
        move.l  #$55555555, $0004005C

        ; BPL4: word = $3333.
        move.l  #$33333333, $00040060
        move.l  #$33333333, $00040064
        move.l  #$33333333, $00040068
        move.l  #$33333333, $0004006C
        move.l  #$33333333, $00040070
        move.l  #$33333333, $00040074
        move.l  #$33333333, $00040078
        move.l  #$33333333, $0004007C

        ; BPL5: word = $0F0F.
        move.l  #$0F0F0F0F, $00040080
        move.l  #$0F0F0F0F, $00040084
        move.l  #$0F0F0F0F, $00040088
        move.l  #$0F0F0F0F, $0004008C
        move.l  #$0F0F0F0F, $00040090
        move.l  #$0F0F0F0F, $00040094
        move.l  #$0F0F0F0F, $00040098
        move.l  #$0F0F0F0F, $0004009C

        ; BPL6: word = $00FF.
        move.l  #$00FF00FF, $000400A0
        move.l  #$00FF00FF, $000400A4
        move.l  #$00FF00FF, $000400A8
        move.l  #$00FF00FF, $000400AC
        move.l  #$00FF00FF, $000400B0
        move.l  #$00FF00FF, $000400B4
        move.l  #$00FF00FF, $000400B8
        move.l  #$00FF00FF, $000400BC

        ; BPL7: bit 4 of palette index = (pixel_x >> 4) & 1.
        ; Each 16-pixel word is uniform: $0000 then $FFFF, alternating.
        move.l  #$0000FFFF, $000400C0
        move.l  #$0000FFFF, $000400C4
        move.l  #$0000FFFF, $000400C8
        move.l  #$0000FFFF, $000400CC
        move.l  #$0000FFFF, $000400D0
        move.l  #$0000FFFF, $000400D4
        move.l  #$0000FFFF, $000400D8
        move.l  #$0000FFFF, $000400DC

        ; BPL8: bit 5 of palette index = (pixel_x >> 5) & 1. Each word
        ; uniform, alternating in pairs ($0000 $0000 $FFFF $FFFF ...).
        move.l  #$00000000, $000400E0
        move.l  #$FFFFFFFF, $000400E4
        move.l  #$00000000, $000400E8
        move.l  #$FFFFFFFF, $000400EC
        move.l  #$00000000, $000400F0
        move.l  #$FFFFFFFF, $000400F4
        move.l  #$00000000, $000400F8
        move.l  #$FFFFFFFF, $000400FC

        ; ---- fill each plane buffer with 192 copies of its template ----
        moveq   #0, D0
        bsr     fill_plane
        moveq   #1, D0
        bsr     fill_plane
        moveq   #2, D0
        bsr     fill_plane
        moveq   #3, D0
        bsr     fill_plane
        moveq   #4, D0
        bsr     fill_plane
        moveq   #5, D0
        bsr     fill_plane
        moveq   #6, D0
        bsr     fill_plane
        moveq   #7, D0
        bsr     fill_plane

        ; ---- 64-entry rainbow palette (Amiga 12-bit colors) ----
        move.l  #palette_table, A0
        move.l  #$00FE0180, A1          ; colors[0..31] at bank 0 $180..$1FC
        moveq   #31, D0
pal0:   move.l  (A0)+, (A1)+
        dbra    D0, pal0
        ; colors[32..63] at bank 1 $380..$3FC (-> $00FE0380..$00FE03FC)
        move.l  #$00FE0380, A1
        moveq   #31, D0
pal1:   move.l  (A0)+, (A1)+
        dbra    D0, pal1

        ; ---- Sprite 0 data at $30000 (2bpp, 16 rows -> 16 * 4 = 64 bytes) ----
        ; Layout: one 32-bit word per row = {data_a[15:0], data_b[15:0]}.
        ; Draw a colourful 16x16 icon.  Both sprites combined (ATT mode)
        ; give 4 bits per pixel -> 16 colour values selecting from
        ; colors[base..base+15].
        move.l  #sprite0_data, A0
        move.l  #$00030000, A1
        moveq   #15, D0
sp0:    move.l  (A0)+, (A1)+
        dbra    D0, sp0

        move.l  #sprite1_data, A0
        move.l  #$00030040, A1
        moveq   #15, D0
sp1:    move.l  (A0)+, (A1)+
        dbra    D0, sp1

        ; ---- Denise: BPLCON0 = HAM=1, BPU3=1 ($0810) ----
        move.l  #$00000810, $00FE0100
        move.l  #0,         $00FE0104   ; BPLCON1 (no scroll)
        move.l  #$00000024, $00FE0108   ; BPLCON2: PF1P=PF2P=4 (sprites win)

        ; Bitplane pointers (planes 0..7 at $20000 + i*$1800)
        move.l  #$00020000, $00FE0110
        move.l  #$00021800, $00FE0114
        move.l  #$00023000, $00FE0118
        move.l  #$00024800, $00FE011C
        move.l  #$00026000, $00FE0120
        move.l  #$00027800, $00FE0124
        move.l  #$00029000, $00FE0300   ; BPL7PT bank 1
        move.l  #$0002A800, $00FE0304   ; BPL8PT bank 1
        move.l  #0, $00FE0128
        move.l  #0, $00FE012C

        ; ---- Display window: inset 32 left/right, 16 top/bottom.
        ;       Window covers (32,16) .. (224,176)
        move.l  #$00100020, $00FE010C   ; DIWSTRT = Y=$10=16, X=$20=32
        move.l  #$00B000E0, $00FE0174   ; DIWSTOP = Y=$B0=176, X=$E0=224

        ; ---- Sprites: 0 = even, 1 = odd ATT-attached partner ----
        ; SPR0CTL: Y=80 (in DIW), X=initial 80, H=16, BPP2=1, ATT=0, base=8
        ;          = ($50 << 24) | ($50 << 16) | ($10 << 8) | $48
        ;          = $50500148
        move.l  #$00030000, $00FE0130   ; SPR0PT
        move.l  #$50500148, $00FE0134
        ; SPR1CTL: Y=80, X=80, H=16, BPP2=1, ATT=1, base=8
        ;          = $50500148 | $80 = $505001C8
        move.l  #$00030040, $00FE0138   ; SPR1PT
        move.l  #$505001C8, $00FE013C
        move.l  #$00000003, $00FE0170   ; SPRENA = sprites 0 + 1

        ; ---- animation loop: bounce sprite X ----
        moveq   #0, D7                  ; frame counter

main_loop:
        ; Compute X = abs((frame mod 384) - 192) + 16
        move.l  D7, D0
        andi.l  #$1FF, D0               ; D0 = frame mod 512
        cmpi.l  #256, D0
        blt     no_bounce
        move.l  #511, D1
        sub.l   D0, D1
        move.l  D1, D0
no_bounce:
        ; D0 now in [0..255].  Clamp to [16..208] so 16-px sprite fits in window.
        cmpi.l  #208, D0
        ble     x_clip
        move.l  #208, D0
x_clip:
        ; Build SPR0CTL = Y(80)<<24 | X<<16 | H(16)<<8 | BPP2|base8 = $48
        move.l  #$50000000, D1          ; Y<<24 (80=$50)
        move.l  D0, D2
        lsl.l   #8, D2
        lsl.l   #8, D2                  ; X<<16
        or.l    D2, D1
        ori.l   #$00001048, D1          ; H<<8 | BPP2 | base
        move.l  D1, $00FE0134

        ; SPR1CTL = same Y/X/H but ATT set: $48 | $80 = $C8
        move.l  D1, D2
        ori.l   #$000000C8, D2
        andi.l  #$FFFFFFF8, D2          ; clear low 3 bits then re-add color
        ori.l   #$000000C8, D2
        move.l  D2, $00FE013C

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
wait:   move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait

        ; pacing
        move.l  #150000, D0
pace:   subq.l  #1, D0
        bne     pace

        addq.l  #1, D7
        bra     main_loop


; ============================================================
; fill_plane(D0 = plane index 0..7)
;   Blits the 32-byte template at $40000 + D0*32 across 192 rows
;   of 16 words to plane buffer $20000 + D0*$1800.
; ============================================================
fill_plane:
        ; Save D7 (frame counter) around the call.
        move.l  D7, $00007FFC

fp_w0:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w0

        ; src = $40000 + D0 * 32
        move.l  D0, D1
        lsl.l   #5, D1
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

        ; Blitter: BLTCON LF=$F0 (D=A), USEA|USED, no shift, ascending.
        move.l  #$F0000009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  D1,         $00FE000C   ; BLTAPT
        move.l  D2,         $00FE0018   ; BLTDPT
        move.l  #-32,       $00FE001C   ; BLTAMOD rewinds APT each row
        move.l  #0,         $00FE0028
        move.l  #$00003010, $00FE0038   ; (192<<6) | 16 words

fp_w1:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w1

        move.l  $00007FFC, D7
        rts


; ============================================================
; 64-entry rainbow palette.  Walks H around hue cycle, then varies S/V.
; ============================================================
palette_table:
        ; Row 0: bright primary/secondary rainbow (16 colours).
        .long  $00000000      ; COLOR00 = black (visible as outer border)
        .long  $00000F00      ; red
        .long  $00000F80      ; orange
        .long  $00000FF0      ; yellow
        .long  $00000CF0      ; lime
        .long  $000008F0      ; green
        .long  $000000F0      ; deep green
        .long  $000000F8      ; teal
        .long  $000000FF      ; cyan
        .long  $0000008F      ; sky blue
        .long  $0000000F      ; blue
        .long  $0000080F      ; purple
        .long  $00000F0F      ; magenta
        .long  $00000F08      ; pink
        .long  $00000F04      ; salmon
        .long  $00000F0C      ; rose
        ; Row 1: pastels.
        .long  $00000FAA
        .long  $00000FCA
        .long  $00000FEA
        .long  $00000EFA
        .long  $00000AFA
        .long  $000008FA
        .long  $000006FE
        .long  $000004FE
        .long  $000002FE
        .long  $000000FE
        .long  $00000AAF
        .long  $00000CCF
        .long  $00000EEF
        .long  $00000FCF
        .long  $00000FAF
        .long  $00000F8F
        ; Row 2: dark shades.
        .long  $00000800
        .long  $00000840
        .long  $00000880
        .long  $000008C0
        .long  $000008F0
        .long  $000004F0
        .long  $000000F4
        .long  $000000F8
        .long  $000000FC
        .long  $0000008F
        .long  $0000004F
        .long  $0000000F
        .long  $0000040F
        .long  $0000080F
        .long  $0000080C
        .long  $00000808
        ; Row 3: greyscale + extras.
        .long  $00000111
        .long  $00000222
        .long  $00000333
        .long  $00000444
        .long  $00000555
        .long  $00000666
        .long  $00000777
        .long  $00000888
        .long  $00000999
        .long  $00000AAA
        .long  $00000BBB
        .long  $00000CCC
        .long  $00000DDD
        .long  $00000EEE
        .long  $00000FFF
        .long  $00000FFA       ; warm white

; ============================================================
; Sprite 0 + Sprite 1 data (16 rows, attached pair forming 4bpp icon).
; Each row = 32-bit word {data_a[15:0], data_b[15:0]}.
;
; Even sprite (S0) holds the low 2 plane bits; odd (S1) holds the high 2.
; Combined pixel = {S1.b, S1.a, S0.b, S0.a} -> 4-bit index into
; colors[8..23] (base = 8).
;
; We'll draw a 16x16 "comet" with a bright core and trail.
; For simplicity: rows 0-1 transparent, rows 2-13 a colorful blob,
; rows 14-15 transparent.
; ============================================================
sprite0_data:
        ; Row 0: transparent
        .long  $00000000
        ; Row 1: transparent
        .long  $00000000
        ; Row 2: thin top edge - center 4 pixels color 1 (S0.a=1)
        .long  $07E00000
        ; Row 3: wider top - center 8 px color 1
        .long  $0FF00000
        ; Row 4-11: solid block, vary S0 bits to make colors
        .long  $1FF80000      ; S0.a=1FF8, S0.b=0
        .long  $3FFC0000
        .long  $7FFE0000
        .long  $FFFF0000
        .long  $FFFF0000
        .long  $7FFE0000
        .long  $3FFC0000
        .long  $1FF80000
        ; Row 12-13: tail
        .long  $0FF00000
        .long  $07E00000
        ; Row 14-15: transparent
        .long  $00000000
        .long  $00000000

sprite1_data:
        ; Row 0-1: transparent
        .long  $00000000
        .long  $00000000
        ; Row 2: S1.a center-most 2 pixels
        .long  $03000000
        ; Row 3: S1.a 4 pixels in center
        .long  $07800000
        ; Row 4-11: S1 bits make some inner pixels darker/lighter
        .long  $0FC00FF0
        .long  $1FE01FF8
        .long  $3FF03FFC
        .long  $7FF87FFE
        .long  $7FF87FFE
        .long  $3FF03FFC
        .long  $1FE01FF8
        .long  $0FC00FF0
        .long  $07800000
        .long  $03000000
        .long  $00000000
        .long  $00000000
