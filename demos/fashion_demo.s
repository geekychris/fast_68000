; HAM6 "Fashion-style" showcase demo.
;
; Renders a 256x192 HAM6 image with three vertically stacked smooth
; gradient bands meant to evoke the Amiga "Fashion" promo image
; without actually shipping its bitmap.  Each band gets its own
; horizontal R/G/B sweep so the screen is filled with ~thousands of
; distinct colours from a single bitplane render — exactly what HAM
; was invented for on the Amiga.
;
; Band layout (vertical):
;   y = 0..47    "sky"      blue -> pink horizon
;   y = 48..95   "skin"     peach -> rose, with a darker oval region
;   y = 96..143  "dress"    burgundy / red gradient with shading
;   y = 144..191 "shadow"   purple/brown falloff
;
; HAM6 encoding (per pixel byte):
;   $00..$0F  : load palette[idx]            (reset previous colour)
;   $10..$1F  : modify B = data              (hold R, G)
;   $20..$2F  : modify R = data              (hold G, B)
;   $30..$3F  : modify G = data              (hold R, B)
;
; The encoder writes one row at a time:
;   pixel 0    : palette[0]               (always reset to COLOR00)
;   pixel 1    : modify R from band table (sets the row's red level)
;   pixel 2    : modify G from band table (sets row's green level)
;   pixel 3    : modify B from band table (sets row's blue level)
;   pixels 4..255: modify B in a sweep (0..15..0..15..) with periodic
;                  G nudges to add cool / warm horizontal variation.
;
; Memory layout (well clear of the 1-bpp overlay at $20000 and the
; chunky framebuffer at $10000 used by sim_main.cpp):
;   $00040000   per-row pixel-value buffer (256 bytes)
;   $00060000   plane 0 bitmap  ($1800 bytes per plane = 192 rows x 32 B)
;   $00061800   plane 1
;   $00063000   plane 2
;   $00064800   plane 3
;   $00066000   plane 4
;   $00067800   plane 5

        .org $400

start:
        ; COLOR00 = deep navy; every row's HAM walk starts from here.
        move.l  #$00000003, $00FE0180

        moveq   #0, D7                      ; D7 = Y
row_loop:
        bsr     generate_row
        bsr     pack_row
        addq.l  #1, D7
        cmpi.l  #192, D7
        bne     row_loop

        ; Denise: BPU=6, HAM6.  BPLCON0 [14:12]=6, [11]=1.
        move.l  #$00006800, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108

        move.l  #$00060000, $00FE0110       ; BPL1PT
        move.l  #$00061800, $00FE0114
        move.l  #$00063000, $00FE0118
        move.l  #$00064800, $00FE011C
        move.l  #$00066000, $00FE0120
        move.l  #$00067800, $00FE0124
        move.l  #0,         $00FE0128
        move.l  #0,         $00FE012C

        move.l  #1, $00FE0140               ; DENRUN
wait_den:
        move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait_den
        stop    #0

; ============================================================
; generate_row(D7 = Y):
;   Writes 256 HAM6 pixel bytes to $00040000.
;
;   Lookups:
;     band_R, band_G, band_B from the row's "band" (Y / 48).
;     Each pixel's modify-B sweeps 0..15..0..15 so each row contains
;     a smooth blue-axis gradient inside the band's base R/G.
;     Every 16 pixels we nudge G by ±1 to add hue shift across the row.
; ============================================================
generate_row:
        move.l  #$00040000, A0

        ; band index = Y >> 6  -> 0..2 (last band reuses 2)
        move.l  D7, D0
        lsr.l   #6, D0
        cmpi.l  #3, D0
        blt     bd_ok
        moveq   #3, D0
bd_ok:
        ; Indices 0..3 into band tables (4 longs each).
        lsl.l   #2, D0                      ; *4 for .long table
        move.l  #band_R_tab, A1
        adda.l  D0, A1
        move.l  (A1), D5
        andi.l  #$0F, D5                    ; D5 = base R (0..15)

        move.l  #band_G_tab, A1
        adda.l  D0, A1
        move.l  (A1), D6
        andi.l  #$0F, D6                    ; D6 = base G

        move.l  #band_B_tab, A1
        adda.l  D0, A1
        move.l  (A1), D2
        andi.l  #$0F, D2                    ; D2 = base B

        ; pixel 0: palette[0] = COLOR00 (reset)
        clr.b   (A0)+

        ; pixel 1: modify R = base_R
        move.b  D5, D1
        andi.b  #$0F, D1
        ori.b   #$20, D1
        move.b  D1, (A0)+

        ; pixel 2: modify G = base_G
        move.b  D6, D1
        andi.b  #$0F, D1
        ori.b   #$30, D1
        move.b  D1, (A0)+

        ; pixel 3: modify B = base_B
        move.b  D2, D1
        andi.b  #$0F, D1
        ori.b   #$10, D1
        move.b  D1, (A0)+

        ; pixels 4..255: sweep B (and nudge G every 16 px).
        ; Use D3 = pos (0..251), D4 = remaining (252).
        moveq   #0, D3
        move.l  #252, D4
gr_lp:
        move.l  D3, D0
        andi.l  #15, D0                     ; in-segment (0..15)
        beq     gr_nudgeG                   ; first pixel of each 16-pixel group

        ; modify B = ((pos & 15) - 1 + base_B) wrapped to 0..15.
        subq.l  #1, D0
        add.l   D2, D0
        andi.l  #15, D0
        ori.b   #$10, D0
        move.b  D0, (A0)+
        bra     gr_step

gr_nudgeG:
        ; Each group bumps G by 1 within 0..15.
        move.l  D3, D1
        lsr.l   #4, D1                      ; segment idx 0..15
        add.l   D6, D1
        andi.l  #15, D1
        ori.b   #$30, D1
        move.b  D1, (A0)+

gr_step:
        addq.l  #1, D3
        subq.l  #1, D4
        bne     gr_lp
        rts

; ============================================================
; pack_row(D7 = Y):  same logic as ham_demo.s pack_row.
; ============================================================
pack_row:
        moveq   #0, D6
pr_plane:
        move.l  D6, D0
        moveq   #11, D1
        lsl.l   D1, D0
        move.l  D6, D1
        moveq   #12, D2
        lsl.l   D2, D1
        add.l   D1, D0
        move.l  D7, D1
        lsl.l   #5, D1
        add.l   D1, D0
        addi.l  #$00060000, D0
        move.l  D0, A1

        move.l  #$00040000, A0
        moveq   #32, D5
pr_byte:
        moveq   #0, D4
        moveq   #7, D3
pr_bit:
        move.b  (A0)+, D0
        andi.l  #$FF, D0
        move.l  D0, D1
        lsr.l   D6, D1
        andi.l  #1, D1
        beq     pr_skip
        moveq   #1, D2
        lsl.l   D3, D2
        or.l    D2, D4
pr_skip:
        subq.l  #1, D3
        bpl     pr_bit

        move.b  D4, (A1)+
        subq.l  #1, D5
        bne     pr_byte

        addq.l  #1, D6
        cmpi.l  #6, D6
        bne     pr_plane
        rts

; ============================================================
; Band tables: indexed by band = Y/64 (0..3).  Values are 4-bit
; HAM data fields (0..15).
; ============================================================
band_R_tab:
        .long 1
        .long 13
        .long 14
        .long 6
band_G_tab:
        .long 3
        .long 9
        .long 3
        .long 2
band_B_tab:
        .long 12
        .long 7
        .long 4
        .long 5
