; Filled polygon demo.
;
; Each frame:
;   1. Clear the bitplane.
;   2. Draw a triangle's three edges as Bresenham lines via blitter LINE mode.
;   3. Run an Inclusive Fill blit (IFE) over the bitplane to flood the
;      interior between the edges on each scanline.
;   4. Pace and loop.
;
; The triangle translates horizontally by a small amount each frame using
; a precomputed offset table, producing a polygon that slides back and
; forth across the screen.
;
; Bitplane:    $00020000 (256x192 / 8 = 6144 bytes)
; Scratch for line-draw params: $00030000..$0003001F
; Triangle vertex scratch: $00030020..$00030037 (3 vertices * 2 longs = 24 bytes)

        .org $400

start:
        moveq   #0, D7                  ; frame counter

main_loop:
        ; ---- compute X offset = bounce_table[frame_count & 63] ----
        move.l  D7, D0
        andi.l  #63, D0
        lsl.l   #2, D0                  ; * 4 bytes/entry
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D6                ; D6 = signed X offset

        ; ---- vertex setup (cx + offset, cy) ----
        ; Triangle:
        ;   A = (cx, cy-30)
        ;   B = (cx-40, cy+30)
        ;   C = (cx+40, cy+30)
        ; cx = 128, cy = 96
        move.l  #128, D0
        add.l   D6, D0                  ; cx_actual = 128 + offset
        ; Store as triangle vertices.
        ; A.x, A.y
        move.l  D0, $00030020
        move.l  #66,  $00030024         ; A.y = 96 - 30
        ; B.x, B.y
        move.l  D0, D1
        subi.l  #40, D1
        move.l  D1, $00030028
        move.l  #126, $0003002C         ; B.y = 96 + 30
        ; C.x, C.y
        move.l  D0, D2
        addi.l  #40, D2
        move.l  D2, $00030030
        move.l  #126, $00030034

        ; ---- clear the bitplane via blitter (LF=0, USED-only) ----
        bsr     clear_bp

        ; ---- draw edges A->B, B->C, C->A ----
        ; edge A->B
        move.l  $00030020, $00030000    ; x0 = A.x
        move.l  $00030024, $00030004    ; y0 = A.y
        move.l  $00030028, $00030008    ; x1 = B.x
        move.l  $0003002C, $0003000C    ; y1 = B.y
        bsr     draw_line

        ; edge B->C
        move.l  $00030028, $00030000
        move.l  $0003002C, $00030004
        move.l  $00030030, $00030008
        move.l  $00030034, $0003000C
        bsr     draw_line

        ; edge C->A
        move.l  $00030030, $00030000
        move.l  $00030034, $00030004
        move.l  $00030020, $00030008
        move.l  $00030024, $0003000C
        bsr     draw_line

        ; ---- fill (IFE copy A=$20000 -> D=$20000) ----
        bsr     fill_bitplane

        ; ---- advance frame, pace, loop ----
        addq.l  #1, D7
        move.l  #1500, D0
pace:   subq.l  #1, D0
        bne     pace
        bra     main_loop

; ============================================================
; clear_bp: blit zeros across the bitplane.  192 rows x 16 words.
; ============================================================
clear_bp:
cbp_w0: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w0
        move.l  #$00000001, $00FE0000   ; BLTCON: LF=0, USED only
        move.l  #$00020000, $00FE0018   ; BLTDPT
        move.l  #0,         $00FE0028   ; BLTDMOD
        move.l  #$00003010, $00FE0038   ; BLTSIZE (192 << 6) | 16
cbp_w1: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w1
        rts

; ============================================================
; fill_bitplane: A=D=$20000, LF=$F0 (copy A->D), IFE=1.
; Walks every word of every row and applies the inclusive fill.
; ============================================================
fill_bitplane:
fb_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w0
        ; BLTCON: LF=$F0, ASH=0, BSH=0, IFE=1, line=0, USEA|USED ($9)
        ;   [31:24]=F0, [14]=1, [3:0]=9 -> $F000_4009
        move.l  #$F0004009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  #$00020000, $00FE000C   ; BLTAPT
        move.l  #$00020000, $00FE0018   ; BLTDPT
        move.l  #0,         $00FE001C   ; BLTAMOD
        move.l  #0,         $00FE0028
        ; height=192, width=16 -> $3010
        move.l  #$00003010, $00FE0038
fb_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w1
        rts

; ============================================================
; draw_line: inputs in scratch at $30000..$3000C (x0,y0,x1,y1).
; Clobbers D0-D7, A0-A2.
; ============================================================
draw_line:
dl_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w0

        ; dx = x1 - x0, sx, abs(dx)
        move.l  $00030008, D2
        sub.l   $00030000, D2
        moveq   #0, D6
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6
dl_dx_done:

        ; dy
        move.l  $0003000C, D3
        sub.l   $00030004, D3
        moveq   #0, D7
        tst.l   D3
        bpl     dl_dy_done
        neg.l   D3
        moveq   #1, D7
dl_dy_done:

        ; dominance
        cmp.l   D3, D2
        bge     dl_xdom
        ; Y dom
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
        ; D2 = AUL, D4 = max, D5 = min, D6 = SUD, D7 = SUL

        ; pixel addr = $20000 + y0*32 + (x0/16)*2
        move.l  $00030004, D0
        lsl.l   #5, D0
        move.l  $00030000, D1
        lsr.l   #4, D1
        lsl.l   #1, D1
        add.l   D1, D0
        addi.l  #$00020000, D0
        move.l  D0, A1

        ; BLTAPT = 4*min - 2*max
        move.l  D5, D0
        lsl.l   #2, D0
        move.l  D4, D1
        lsl.l   #1, D1
        sub.l   D1, D0
        move.l  D0, $00030010
        ; BLTAMOD = 4*(min-max)
        move.l  D5, D1
        sub.l   D4, D1
        lsl.l   #2, D1
        move.l  D1, $00030014
        ; BLTBMOD = 4*min
        move.l  D5, D1
        lsl.l   #2, D1
        move.l  D1, $00030018
        ; BLTSIZE = ((max+1) << 6) | 2
        move.l  D4, D1
        addq.l  #1, D1
        lsl.l   #6, D1
        ori.l   #2, D1
        move.l  D1, $0003001C

        ; BLTCON
        move.l  $00030000, D0
        andi.l  #15, D0
        moveq   #20, D1
        lsl.l   D1, D0                  ; ASH
        ori.l   #$CA000000, D0          ; LF
        ori.l   #$0000080F, D0          ; LINE + channels
        move.l  D6, D1
        lsl.l   #2, D1
        move.l  D7, D3
        lsl.l   #1, D3
        or.l    D3, D1
        or.l    D2, D1
        moveq   #8, D3
        lsl.l   D3, D1
        or.l    D1, D0                  ; octant bits

        move.l  D0,         $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  $00030010,  $00FE000C
        move.l  #0,         $00FE0010
        move.l  A1, D0
        move.l  D0,         $00FE0014
        move.l  D0,         $00FE0018
        move.l  $00030014,  $00FE001C
        move.l  $00030018,  $00FE0020
        move.l  #32,        $00FE0024
        move.l  #32,        $00FE0028
        move.l  #$0000FFFF, $00FE0030
        move.l  $0003001C,  $00FE0038

dl_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w1
        rts

; ============================================================
; bounce_table: 64 entries of signed X-offset in [-60, 60], producing
; a smooth left-right bounce over 64 frames.
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
