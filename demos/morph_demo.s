; Morphing filled-polygon demo.
;
; Renders a 4-sided convex shape (a trapezoid with horizontal top and
; bottom edges) whose vertices each animate independently using a
; precomputed bounce table.  The top center, bottom center, top Y,
; and bottom Y all pull from the same bounce table with different
; phase offsets, so the polygon translates and changes shape every
; frame.
;
; Layout:
;   - Top edge:    row TOP_Y, from top_left to top_right
;                  (fixed width 20 pixels - even count for IFE parity)
;   - Bottom edge: row BOTTOM_Y, from bot_left to bot_right
;                  (fixed width 30 pixels - even count for IFE parity)
;   - Left edge:   bot_left -> top_left (sloped)
;   - Right edge:  top_right -> bot_right (sloped)
;
; Both horizontal edges have an even pixel count and the sloped edges
; contribute exactly 2 pixels per middle scanline, so every scanline
; has an even edge-crossing count and the IFE fill works cleanly.
;
; Scratch memory map (avoid plane buffer at $20000..$217FF):
;   $30000..$3000F  draw_line input: x0, y0, x1, y1
;   $30010..$3001F  draw_line internal scratch
;   $30020          top_left
;   $30024          top_right
;   $30028          TOP_Y
;   $3002C          bot_left
;   $30030          bot_right
;   $30034          BOTTOM_Y
;   $30040          frame counter (draw_line clobbers D7)

        .org $400

start:
        ; frame counter at $30040 initialised to 0
        move.l  #0, $00030040

main_loop:
        ; ---- compute four phase indices ----
        ; phase = frame & 63
        move.l  $00030040, D7

        ; --- top center bounce: bounce_table[phase] / 2 ---
        ; Halve the X range so sloped edges stay Y-dominant (|dx|<=~30
        ; vs |dy|>=~70) -- prevents IFE parity leak via Bresenham
        ; putting multiple pixels per scanline on a shallow edge.
        move.l  D7, D0
        andi.l  #63, D0
        lsl.l   #2, D0
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D1                ; signed offset in [-60, 60]
        asr.l   #1, D1                  ; / 2 -> [-30, 30]
        addi.l  #128, D1                ; top_cx
        ; top_left = top_cx - 10, top_right = top_cx + 9
        move.l  D1, D2
        subi.l  #10, D2
        move.l  D2, $00030020           ; top_left
        addi.l  #19, D2                 ; top_left + 19 = top_right
        move.l  D2, $00030024

        ; --- top Y bounce: bounce[(phase + 16) & 63] / 4 + 60 ---
        move.l  D7, D0
        addi.l  #16, D0
        andi.l  #63, D0
        lsl.l   #2, D0
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D1
        asr.l   #2, D1                  ; / 4  (signed)
        addi.l  #60, D1                 ; TOP_Y in [45, 75]
        move.l  D1, $00030028

        ; --- bottom center bounce: bounce[(phase + 32) & 63] / 2 ---
        move.l  D7, D0
        addi.l  #32, D0
        andi.l  #63, D0
        lsl.l   #2, D0
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D1
        asr.l   #1, D1                  ; / 2 -> [-30, 30]
        addi.l  #128, D1
        move.l  D1, D2
        subi.l  #15, D2
        move.l  D2, $0003002C           ; bot_left
        addi.l  #29, D2
        move.l  D2, $00030030           ; bot_right

        ; --- bottom Y bounce: bounce[(phase + 48) & 63] / 4 + 140 ---
        move.l  D7, D0
        addi.l  #48, D0
        andi.l  #63, D0
        lsl.l   #2, D0
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D1
        asr.l   #2, D1
        addi.l  #140, D1                ; BOTTOM_Y in [125, 155]
        move.l  D1, $00030034

        ; ---- clear bitplane ----
        bsr     clear_bp

        ; ---- draw 4 edges ----
        ; V0 = (top_left,  TOP_Y)
        ; V1 = (top_right, TOP_Y)
        ; V2 = (bot_right, BOTTOM_Y)
        ; V3 = (bot_left,  BOTTOM_Y)

        ; edge V0 -> V1 (top, horizontal)
        move.l  $00030020, D0
        move.l  $00030028, D1
        move.l  $00030024, D2
        move.l  $00030028, D3
        bsr     draw_line

        ; edge V1 -> V2 (right, sloped)
        move.l  $00030024, D0
        move.l  $00030028, D1
        move.l  $00030030, D2
        move.l  $00030034, D3
        bsr     draw_line

        ; edge V2 -> V3 (bottom, horizontal)
        move.l  $00030030, D0
        move.l  $00030034, D1
        move.l  $0003002C, D2
        move.l  $00030034, D3
        bsr     draw_line

        ; edge V3 -> V0 (left, sloped)
        move.l  $0003002C, D0
        move.l  $00030034, D1
        move.l  $00030020, D2
        move.l  $00030028, D3
        bsr     draw_line

        ; ---- fill ----
        bsr     fill_bitplane

        ; ---- advance frame and pace ----
        move.l  $00030040, D0
        addq.l  #1, D0
        move.l  D0, $00030040

        ; ---- advance frame and pace ----
        move.l  $00030040, D0
        addq.l  #1, D0
        move.l  D0, $00030040

        move.l  #500000, D0
pace:   subq.l  #1, D0
        bne     pace
        bra     main_loop

; ============================================================
clear_bp:
cbp_w0: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w0
        move.l  #$00000001, $00FE0000
        move.l  #$00020000, $00FE0018
        move.l  #0,         $00FE0028
        move.l  #$00003010, $00FE0038
cbp_w1: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w1
        rts

fill_bitplane:
fb_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w0
        move.l  #$F0004009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  #$00020000, $00FE000C
        move.l  #$00020000, $00FE0018
        move.l  #0,         $00FE001C
        move.l  #0,         $00FE0028
        move.l  #$00003010, $00FE0038
fb_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w1
        rts

; ============================================================
; draw_line: line from (D0,D1) to (D2,D3) into the bitplane.
; ============================================================
draw_line:
dl_w0:  move.l  $00FE003C, D4
        andi.l  #1, D4
        bne     dl_w0
        move.l  D0, $00030000
        move.l  D1, $00030004
        sub.l   D0, D2
        moveq   #0, D6
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6
dl_dx_done:
        sub.l   D1, D3
        moveq   #0, D7
        tst.l   D3
        bpl     dl_dy_done
        neg.l   D3
        moveq   #1, D7
dl_dy_done:
        cmp.l   D3, D2
        bge     dl_xdom
        move.l  D3, D4
        move.l  D2, D5
        move.l  D6, D0
        move.l  D7, D6
        move.l  D0, D7
        moveq   #1, D2
        bra     dl_dom_done
dl_xdom:
        move.l  D2, D4
        move.l  D3, D5
        moveq   #0, D2
dl_dom_done:
        move.l  $00030004, D0
        lsl.l   #5, D0
        move.l  $00030000, D1
        lsr.l   #4, D1
        lsl.l   #1, D1
        add.l   D1, D0
        addi.l  #$00020000, D0
        move.l  D0, A1
        move.l  D5, D0
        lsl.l   #2, D0
        move.l  D4, D1
        lsl.l   #1, D1
        sub.l   D1, D0
        move.l  D0, $00030010
        move.l  D5, D1
        sub.l   D4, D1
        lsl.l   #2, D1
        move.l  D1, $00030014
        move.l  D5, D1
        lsl.l   #2, D1
        move.l  D1, $00030018
        move.l  D4, D1
        addq.l  #1, D1
        lsl.l   #6, D1
        ori.l   #2, D1
        move.l  D1, $0003001C
        move.l  $00030000, D0
        andi.l  #15, D0
        moveq   #20, D1
        lsl.l   D1, D0
        ori.l   #$CA000000, D0
        ori.l   #$0000080F, D0
        move.l  D6, D1
        lsl.l   #2, D1
        move.l  D7, D3
        lsl.l   #1, D3
        or.l    D3, D1
        or.l    D2, D1
        moveq   #8, D3
        lsl.l   D3, D1
        or.l    D1, D0
        move.l  D0,         $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  $00030010, D1
        move.l  D1,         $00FE000C
        move.l  #0,         $00FE0010
        move.l  A1, D1
        move.l  D1,         $00FE0014
        move.l  D1,         $00FE0018
        move.l  $00030014, D1
        move.l  D1,         $00FE001C
        move.l  $00030018, D1
        move.l  D1,         $00FE0020
        move.l  #32,        $00FE0024
        move.l  #32,        $00FE0028
        move.l  #$0000FFFF, $00FE0030
        move.l  $0003001C, D1
        move.l  D1,         $00FE0038
dl_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w1
        rts

; ============================================================
bounce_table:
        .long  0,    4,    8,   12,   16,   20,   24,   28
        .long 32,   36,   40,   44,   48,   52,   56,   60
        .long 60,   56,   52,   48,   44,   40,   36,   32
        .long 28,   24,   20,   16,   12,    8,    4,    0
        .long -4,  -8,  -12,  -16,  -20,  -24,  -28,  -32
        .long -36, -40, -44,  -48,  -52,  -56,  -60,  -60
        .long -60, -56, -52,  -48,  -44,  -40,  -36,  -32
        .long -28, -24, -20,  -16,  -12,   -8,   -4,    0
