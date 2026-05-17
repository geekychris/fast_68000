; Blitter line-drawing demo.
;
; Animates a star pattern by repeatedly drawing line segments from the
; screen center (128, 96) to points on a circle of radius 60, with the
; angle advancing one step per frame.  No clear -> the lines accumulate
; into a sunburst pattern; every 64 frames we wipe the bitplane.
;
; Bitplane:     $00020000  (256x192 / 8 = 6144 bytes)
; Scratch:      $00030000..$0003001F  (line parameters + intermediates)
; Sine table:   embedded after the code

; ------- offsets into scratch area used by draw_line -------
;   $30000  x0       $30004  y0
;   $30008  x1       $3000C  y1
;   $30010  BLTAPT   $30014  BLTAMOD
;   $30018  BLTBMOD  $3001C  BLTSIZE

; ------- blitter register helpers -------
;   $FE0000 BLTCON   $FE0004 BLTAFWM  $FE0008 BLTALWM
;   $FE000C APT      $FE0010 BPT      $FE0014 CPT      $FE0018 DPT
;   $FE001C AMOD     $FE0020 BMOD     $FE0024 CMOD     $FE0028 DMOD
;   $FE002C ADAT     $FE0030 BDAT     $FE0034 CDAT
;   $FE0038 SIZE     $FE003C STAT

        .org $400

start:
        ; clear bitplane once at startup
        bsr     clear_bp

        moveq   #0, D7              ; frame counter

main_loop:
        ; ---- every 64 frames, clear the bitplane ----
        move.l  D7, D0
        andi.l  #63, D0
        bne     no_clear
        bsr     clear_bp
no_clear:

        ; ---- pick a sine-table entry: idx = D7 mod 16 ----
        move.l  D7, D0
        andi.l  #15, D0
        ; A0 = sin_table + D0*4
        lsl.l   #2, D0
        move.l  #sin_table, A0
        adda.l  D0, A0

        ; (dx, dy) read from table as two 16-bit signed values.
        move.w  (A0)+, D1           ; dx (signed)
        ext.l   D1
        move.w  (A0)+, D2           ; dy
        ext.l   D2

        ; (x1, y1) = (128 + dx, 96 + dy)
        addi.l  #128, D1
        addi.l  #96, D2

        ; store line endpoints in scratch
        move.l  #128, $00030000     ; x0
        move.l  #96,  $00030004     ; y0
        move.l  D1,   $00030008     ; x1
        move.l  D2,   $0003000C     ; y1

        ; draw_line clobbers every register including D7 (it reuses D7 as
        ; the dy-sign register).  Stash the frame counter in scratch so
        ; the outer loop can advance it after the call.
        move.l  D7, $00030020
        bsr     draw_line
        move.l  $00030020, D7

        ; advance frame
        addq.l  #1, D7

        ; pacing delay
        move.l  #1500, D0
pace:   subq.l  #1, D0
        bne     pace

        bra     main_loop

; ============================================================
; clear_bp: blit zeros across the full bitplane via D-only blit.
;   192 rows x 16 words = 3072 word writes.
;   LF=0, USED=1 only (no A/B/C reads).
; ============================================================
clear_bp:
        ; wait for blitter idle
cbp_w0: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w0

        move.l  #$00000001, $00FE0000   ; BLTCON: LF=0, USED only
        move.l  #$00020000, $00FE0018   ; BLTDPT = bitplane base
        move.l  #0,         $00FE0028   ; BLTDMOD = 0
        ; BLTSIZE: height=192, width=16 -> (192 << 6) | 16 = $3010
        move.l  #$00003010, $00FE0038

cbp_w1: move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     cbp_w1
        rts

; ============================================================
; draw_line: draw a Bresenham line into the bitplane.
;   Inputs at $30000..$3000C: x0, y0, x1, y1 (32-bit, signed in principle).
;   Uses D0-D7, A0-A2 (caller may not rely on any register surviving).
; ============================================================
draw_line:
dl_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w0

        ; ---- dx = x1 - x0, sx = sign(dx), then dx = |dx| ----
        move.l  $00030008, D2
        sub.l   $00030000, D2       ; D2 = signed dx
        moveq   #0, D6
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6              ; D6 = sx
dl_dx_done:

        ; ---- dy = y1 - y0, sy ----
        move.l  $0003000C, D3
        sub.l   $00030004, D3
        moveq   #0, D7
        tst.l   D3
        bpl     dl_dy_done
        neg.l   D3
        moveq   #1, D7              ; D7 = sy
dl_dy_done:

        ; ---- determine dominance ----
        ; if dx >= dy: X-dominant. AUL=0. max=dx,min=dy. SUD=sx, SUL=sy.
        ; else:        Y-dominant. AUL=1. max=dy,min=dx. SUD=sy, SUL=sx.
        cmp.l   D3, D2
        bge     dl_xdom
        ; Y dominant
        move.l  D3, D4              ; D4 = max = dy
        move.l  D2, D5              ; D5 = min = dx
        ; swap D6/D7 (no EXG in our assembler)
        move.l  D6, D0
        move.l  D7, D6
        move.l  D0, D7              ; D6=sy=SUD, D7=sx=SUL
        moveq   #1, D2              ; AUL = 1 (Y-dom)
        bra     dl_dom_done
dl_xdom:
        move.l  D2, D4              ; max = dx
        move.l  D3, D5              ; min = dy
        moveq   #0, D2              ; AUL = 0
dl_dom_done:
        ; D2 = AUL, D4 = max, D5 = min, D6 = SUD, D7 = SUL

        ; ---- pixel address: A1 = $20000 + y0*32 + (x0/16)*2 ----
        move.l  $00030004, D0
        lsl.l   #5, D0              ; y0 * 32
        move.l  $00030000, D1
        lsr.l   #4, D1              ; x0 / 16
        lsl.l   #1, D1              ; * 2
        add.l   D1, D0
        addi.l  #$00020000, D0
        move.l  D0, A1

        ; ---- BLTAPT = 4*min - 2*max ----
        move.l  D5, D0
        lsl.l   #2, D0              ; 4*min
        move.l  D4, D1
        lsl.l   #1, D1              ; 2*max
        sub.l   D1, D0
        move.l  D0, $00030010       ; stash BLTAPT

        ; ---- BLTAMOD = 4*(min - max), signed ----
        move.l  D5, D1
        sub.l   D4, D1
        lsl.l   #2, D1
        move.l  D1, $00030014

        ; ---- BLTBMOD = 4*min ----
        move.l  D5, D1
        lsl.l   #2, D1
        move.l  D1, $00030018

        ; ---- BLTSIZE = ((max+1) << 6) | 2 ----
        move.l  D4, D1
        addq.l  #1, D1
        lsl.l   #6, D1
        ori.l   #2, D1
        move.l  D1, $0003001C

        ; ---- build BLTCON ----
        ;   bits 31..24 = LF      = $CA
        ;   bits 23..20 = ASH     = x0 mod 16
        ;   bit  11     = LINE    = 1
        ;   bits 10..8  = octant  = {SUD, SUL, AUL} = D6,D7,D2
        ;   bits 3..0   = channels = $F
        move.l  $00030000, D0
        andi.l  #15, D0
        moveq   #20, D1
        lsl.l   D1, D0              ; ASH at bits [23:20]
        ori.l   #$CA000000, D0
        ori.l   #$0000080F, D0
        ; octant
        move.l  D6, D1
        lsl.l   #2, D1              ; SUD at bit 2
        move.l  D7, D3
        lsl.l   #1, D3              ; SUL at bit 1
        or.l    D3, D1
        or.l    D2, D1              ; AUL at bit 0
        moveq   #8, D3
        lsl.l   D3, D1
        or.l    D1, D0              ; merge octant at bits [10:8]

        ; ---- write blitter registers (reg-to-mem only) ----
        move.l  D0,         $00FE0000   ; BLTCON
        move.l  #$0000FFFF, $00FE0004   ; BLTAFWM
        move.l  #$0000FFFF, $00FE0008   ; BLTALWM
        move.l  $00030010, D1
        move.l  D1,         $00FE000C   ; BLTAPT
        move.l  #0,         $00FE0010   ; BLTBPT (unused)
        move.l  A1, D1
        move.l  D1,         $00FE0014   ; BLTCPT
        move.l  D1,         $00FE0018   ; BLTDPT
        move.l  $00030014, D1
        move.l  D1,         $00FE001C   ; BLTAMOD
        move.l  $00030018, D1
        move.l  D1,         $00FE0020   ; BLTBMOD
        move.l  #32,        $00FE0024   ; BLTCMOD = bytes/row
        move.l  #32,        $00FE0028   ; BLTDMOD
        move.l  #$0000FFFF, $00FE0030   ; BLTBDAT = solid line pattern
        move.l  $0003001C, D1
        move.l  D1,         $00FE0038   ; BLTSIZE -> START

dl_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w1
        rts

; ============================================================
; sin_table: 16 entries of (dx.w, dy.w) for points around a circle of
; radius 60 at 16 angles (22.5 degrees apart).
; ============================================================
sin_table:
        .word  60,    0           ; 0
        .word  55,   23           ; 22.5
        .word  42,   42           ; 45
        .word  23,   55           ; 67.5
        .word   0,   60           ; 90
        .word -23,   55           ; 112.5
        .word -42,   42           ; 135
        .word -55,   23           ; 157.5
        .word -60,    0           ; 180
        .word -55,  -23           ; 202.5
        .word -42,  -42           ; 225
        .word -23,  -55           ; 247.5
        .word   0,  -60           ; 270
        .word  23,  -55           ; 292.5
        .word  42,  -42           ; 315
        .word  55,  -23           ; 337.5
