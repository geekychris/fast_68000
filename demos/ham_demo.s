; HAM6 demo: a 2D color field designed to hit close to all 4096
; colors that HAM6 can produce.
;
; HAM (Hold-And-Modify) lets a 6-plane bitmap encode 4096 distinct
; RGB values.  Each pixel's [5:4] selects the mode and [3:0] the data:
;   00  : load palette[data]              (16-color lookup, prev_color reset)
;   01  : keep R/G, set B = data          (modify B)
;   10  : keep G/B, set R = data          (modify R)
;   11  : keep R/B, set G = data          (modify G)
; "Previous pixel" resets to COLOR00 at the start of each scanline.
;
; Per-row layout:
;   pixel 0      : value $00 -> palette[0] = black (reset)
;   pixel 1      : modify R = (Y / 12)  -> 16 distinct R values over 192 rows
;   pixels 2..255: "snake walk" through (G, B):
;       16 pixels modify B = 0..15      (hold G)
;       1  pixel  modify G += 1
;       16 pixels modify B = 0..15      (hold new G)
;       ...
;   Each row visits 15 G values x 16 B values = 240 colors.
;   Over 16 R levels: ~3840 distinct colors (close to HAM6's 4096 max).
;
; Memory layout:
;   $00040000  pixel-value buffer for the current row (256 bytes)
;   $00060000  plane 0 buffer  ($1800 bytes)
;   $00061800  plane 1
;   $00063000  plane 2
;   $00064800  plane 3
;   $00066000  plane 4
;   $00067800  plane 5

        .org $400

start:
        ; ---- palette: COLOR00 = black ----
        move.l  #$00000000, $00FE0180

        ; ---- generate plane data row-by-row ----
        moveq   #0, D7                  ; D7 = current row Y
row_loop:
        ; --- generate pixel-value buffer at $40000 for row D7 ---
        bsr     generate_row

        ; --- pack pixel-value buffer into all 6 plane rows for Y = D7 ---
        bsr     pack_row

        addq.l  #1, D7
        cmpi.l  #192, D7
        bne     row_loop

        ; ---- configure Denise: BPLCON0 = nplanes=6 (110) | HAM (bit 11) ----
        move.l  #$00006800, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108

        move.l  #$00060000, $00FE0110   ; BPL1PT (plane 0)
        move.l  #$00061800, $00FE0114
        move.l  #$00063000, $00FE0118
        move.l  #$00064800, $00FE011C
        move.l  #$00066000, $00FE0120
        move.l  #$00067800, $00FE0124
        move.l  #0,         $00FE0128
        move.l  #0,         $00FE012C

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
wait:   move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait

        stop    #0

; ============================================================
; generate_row(D7 = Y):
;   Fills $00040000..$000400FF with 256 pixel values for row Y.
;   Uses Y >> 4 for the R level (12 levels over 192 rows) and
;   16-pixel-per-segment snake walk for the (G, B) sweep, so no
;   divu instructions in the hot loop.
; ============================================================
generate_row:
        ; row_R = Y >> 4 (0..11)
        move.l  D7, D0
        lsr.l   #4, D0
        ; D0 = row_R

        move.l  #$00040000, A0

        ; pixel 0 = $00 (reset to COLOR00)
        clr.b   (A0)+
        ; pixel 1 = $20 | row_R  (mode 10: modify R)
        move.b  D0, D1
        ori.b   #$20, D1
        move.b  D1, (A0)+

        ; pixels 2..255: snake walk through (G, B)
        ;   pos = x - 2  (range 0..253)
        ;   in_seg = pos & 15
        ;   segment = pos >> 4
        ;   if in_seg == 0: modify G to segment
        ;   else          : modify B to (in_seg - 1)
        moveq   #0, D2                  ; pos
        move.l  #254, D3                ; remaining
gr_loop:
        move.l  D2, D5
        andi.l  #15, D5                 ; D5 = in_seg
        beq     gr_mod_G

        ; modify B:  value = $10 | (in_seg - 1)
        subq.l  #1, D5
        ori.b   #$10, D5
        move.b  D5, (A0)+
        bra     gr_next

gr_mod_G:
        ; modify G:  value = $30 | segment
        move.l  D2, D4
        lsr.l   #4, D4
        ori.b   #$30, D4
        move.b  D4, (A0)+

gr_next:
        addq.l  #1, D2
        subq.l  #1, D3
        bne     gr_loop
        rts

; ============================================================
; pack_row(D7 = Y):
;   Reads pixel values from $40000 and writes plane bytes for row Y
;   to all 6 plane buffers.
; ============================================================
pack_row:
        moveq   #0, D6                  ; D6 = plane index
pr_plane:
        ; Compute plane_p row addr = $60000 + plane*$1800 + Y*32
        move.l  D6, D0
        moveq   #11, D1
        lsl.l   D1, D0                  ; D0 = plane * $800
        move.l  D6, D1
        moveq   #12, D2
        lsl.l   D2, D1                  ; D1 = plane * $1000
        add.l   D1, D0                  ; D0 = plane * $1800
        move.l  D7, D1
        lsl.l   #5, D1                  ; D1 = Y * 32
        add.l   D1, D0
        addi.l  #$00060000, D0
        move.l  D0, A1                  ; A1 = dst plane row addr

        move.l  #$00040000, A0          ; A0 = pixel buf
        moveq   #32, D5                 ; byte count
pr_byte:
        moveq   #0, D4                  ; accumulator
        moveq   #7, D3                  ; bit_pos (MSB first)
pr_bit:
        move.b  (A0)+, D0
        andi.l  #$FF, D0
        ; bit = (D0 >> plane) & 1
        move.l  D0, D1
        lsr.l   D6, D1
        andi.l  #1, D1
        beq     pr_skip
        ; set bit (D3) of D4
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
