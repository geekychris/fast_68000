; Filled polygon demo.
;
; Each frame:
;   1. Clear the bitplane via blitter (LF=0, USED-only).
;   2. Draw three edges of a triangle as Bresenham lines via blitter
;      LINE mode.
;   3. Run an Inclusive Fill (IFE) blit over the bitplane to flood the
;      interior between the edges on each scanline.
;   4. Pace and loop.
;
; The triangle translates horizontally by a small amount each frame
; using a precomputed offset table, producing a polygon that slides
; back and forth across the screen.  All scratch slots are loaded
; into and out of D registers (no mem-to-mem moves) so blitter
; programming uses only reg-to-mem and immediate-to-mem stores.

        .org $400

start:
        moveq   #0, D7                  ; frame counter

main_loop:
        ; ---- compute X offset = bounce_table[frame_count & 63] ----
        move.l  D7, D0
        andi.l  #63, D0
        lsl.l   #2, D0
        move.l  #bounce_table, A0
        adda.l  D0, A0
        move.l  (A0), D6                ; D6 = signed X offset

        ; ---- vertices ----
        ; Apex is 2 pixels wide so the apex scanline has an EVEN edge-crossing
        ; count (1+1 = 2), keeping the IFE fill carry balanced.
        ;   A1 = (cx-1, 66)   apex-left
        ;   A2 = (cx,   66)   apex-right
        ;   B  = (cx-40, 126) bottom-left
        ;   C  = (cx+40, 126) bottom-right
        ; We do NOT rasterize the bottom horizontal edge (B->C) -- the IFE
        ; fill creates the bottom by filling between the two sloped edges.
        move.l  #128, D0
        add.l   D6, D0                  ; cx_actual

        ; A1.x, A1.y
        move.l  D0, D5
        subq.l  #1, D5                  ; A1.x = cx-1
        move.l  D5, $00030020
        move.l  #66, $00030024          ; A1.y = 66
        ; A2.x, A2.y
        move.l  D0, $00030028           ; A2.x = cx
        move.l  #66, $0003002C
        ; B.x, B.y
        move.l  D0, D1
        subi.l  #40, D1
        move.l  D1, $00030030
        move.l  #126, $00030034
        ; C.x, C.y
        move.l  D0, D2
        addi.l  #40, D2
        move.l  D2, $00030038
        move.l  #126, $0003003C

        ; ---- clear bitplane ----
        bsr     clear_bp

        ; ---- draw 2 sloped edges ----
        ; edge A1 -> B (left side)
        move.l  $00030020, D0
        move.l  $00030024, D1
        move.l  $00030030, D2
        move.l  $00030034, D3
        bsr     draw_line

        ; edge A2 -> C (right side)
        move.l  $00030028, D0
        move.l  $0003002C, D1
        move.l  $00030038, D2
        move.l  $0003003C, D3
        bsr     draw_line

        ; ---- fill ----
        bsr     fill_bitplane

        ; ---- frame pacing ----
        addq.l  #1, D7
        move.l  #200000, D0
pace:   subq.l  #1, D0
        bne     pace
        bra     main_loop

; ============================================================
; clear_bp: zero the bitplane.  192 rows x 16 words.
; ============================================================
clear_bp:
cbp_w0: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w0
        move.l  #$00000001, $00FE0000   ; BLTCON: LF=0, USED only
        move.l  #$00020000, $00FE0018   ; BLTDPT
        move.l  #0,         $00FE0028   ; BLTDMOD
        move.l  #$00003010, $00FE0038   ; BLTSIZE (192<<6) | 16
cbp_w1: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w1
        rts

; ============================================================
; fill_bitplane: IFE blit covering the whole bitplane (A=D=$20000).
; ============================================================
fill_bitplane:
fb_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w0
        move.l  #$F0004009, $00FE0000   ; LF=$F0, IFE=1, USEA|USED
        move.l  #$0000FFFF, $00FE0004   ; AFWM
        move.l  #$0000FFFF, $00FE0008   ; ALWM
        move.l  #$00020000, $00FE000C   ; BLTAPT
        move.l  #$00020000, $00FE0018   ; BLTDPT
        move.l  #0,         $00FE001C   ; AMOD
        move.l  #0,         $00FE0028   ; DMOD
        move.l  #$00003010, $00FE0038
fb_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w1
        rts

; ============================================================
; draw_line: line from (D0,D1) to (D2,D3).  Clobbers D0-D7, A0-A2.
; All inputs in registers; all blitter writes are reg-to-mem.
; ============================================================
draw_line:
dl_w0:  move.l  $00FE003C, D4
        andi.l  #1, D4
        bne     dl_w0

        ; D0=x0, D1=y0, D2=x1, D3=y1.  Preserve x0 and y0 in scratch
        ; slots so we can rebuild the pixel address and the ASH after
        ; the Bresenham math clobbers registers.
        move.l  D0, $00030000           ; x0
        move.l  D1, $00030004           ; y0

        ; dx = x1 - x0; sx
        sub.l   D0, D2                  ; D2 = signed dx
        moveq   #0, D6                  ; D6 = sx
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6
dl_dx_done:

        ; dy = y1 - y0; sy
        sub.l   D1, D3                  ; D3 = signed dy
        moveq   #0, D7                  ; D7 = sy
        tst.l   D3
        bpl     dl_dy_done
        neg.l   D3
        moveq   #1, D7
dl_dy_done:

        ; Dominant axis: if |dx| >= |dy| -> X-dom (AUL=0); else Y-dom.
        cmp.l   D3, D2
        bge     dl_xdom
        ; Y dominant
        move.l  D3, D4                  ; max = dy
        move.l  D2, D5                  ; min = dx
        ; swap sx <-> sy so D6 holds SUD (dom sign) and D7 holds SUL.
        move.l  D6, D0
        move.l  D7, D6
        move.l  D0, D7
        moveq   #1, D2                  ; AUL = 1
        bra     dl_dom_done
dl_xdom:
        move.l  D2, D4                  ; max = dx
        move.l  D3, D5                  ; min = dy
        moveq   #0, D2                  ; AUL = 0
dl_dom_done:
        ; D2=AUL, D4=max, D5=min, D6=SUD, D7=SUL.

        ; pixel addr = $20000 + y0*32 + (x0/16)*2
        move.l  $00030004, D0           ; y0
        lsl.l   #5, D0
        move.l  $00030000, D1           ; x0
        lsr.l   #4, D1
        lsl.l   #1, D1
        add.l   D1, D0
        addi.l  #$00020000, D0
        move.l  D0, A1

        ; BLTAPT = 4*min - 2*max -> stash in scratch + D for later write
        move.l  D5, D0
        lsl.l   #2, D0
        move.l  D4, D1
        lsl.l   #1, D1
        sub.l   D1, D0
        move.l  D0, $00030010           ; BLTAPT

        ; BLTAMOD = 4*(min - max)
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

        ; ---- build BLTCON in D0 ----
        move.l  $00030000, D0           ; x0
        andi.l  #15, D0                 ; x0 mod 16
        moveq   #20, D1
        lsl.l   D1, D0                  ; ASH at bits [23:20]
        ori.l   #$CA000000, D0          ; LF=$CA
        ori.l   #$0000080F, D0          ; LINE=1, chan_en=$F
        move.l  D6, D1
        lsl.l   #2, D1                  ; SUD<<2
        move.l  D7, D3
        lsl.l   #1, D3                  ; SUL<<1
        or.l    D3, D1
        or.l    D2, D1                  ; AUL
        moveq   #8, D3
        lsl.l   D3, D1                  ; octant at bits [10:8]
        or.l    D1, D0                  ; final BLTCON in D0

        ; ---- write blitter regs (all reg-to-mem) ----
        move.l  D0,         $00FE0000   ; BLTCON
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  $00030010, D1
        move.l  D1,         $00FE000C   ; BLTAPT
        move.l  #0,         $00FE0010
        move.l  A1, D1
        move.l  D1,         $00FE0014   ; BLTCPT
        move.l  D1,         $00FE0018   ; BLTDPT
        move.l  $00030014, D1
        move.l  D1,         $00FE001C
        move.l  $00030018, D1
        move.l  D1,         $00FE0020
        move.l  #32,        $00FE0024
        move.l  #32,        $00FE0028
        move.l  #$0000FFFF, $00FE0030
        move.l  $0003001C, D1
        move.l  D1,         $00FE0038   ; START

dl_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w1
        rts

; ============================================================
; bounce_table: 64 entries of signed X-offset producing a smooth
; left-right bounce over 64 frames.
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
