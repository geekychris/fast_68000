; Copper-rainbow + HAM6 demo.
;
; The bitplane data is a uniform HAM black-to-red modify-R gradient
; repeated on every scanline.  Without the Copper, every row would
; show the same gradient because HAM resets prev_color to COLOR00 at
; the start of each scanline.
;
; The Copper list issues a WAIT-RASTER + MOVE COLOR00 pair every six
; scanlines, so each band of 6 rows starts HAM's prev_color from a
; different base.  Combined, the screen shows 32 base colors x 16 R
; gradient steps = ~500 distinct shades from a single bitplane.
;
; Memory layout:
;   $00050000  per-plane row templates (6 x 32 bytes)
;   $00060000  plane 0 buffer  ($1800 bytes each)
;   $00061800  plane 1
;   $00063000  plane 2
;   $00064800  plane 3
;   $00066000  plane 4
;   $00067800  plane 5
;   $00070000  Copper list (32 entries, 8 bytes each = 256 bytes)

        .org $400

start:
        ; ---- palette: COLOR00 set per-band by Copper; here we seed
        ;      band 0's base = COLOR00 = $F00 (red) so the first band's
        ;      modify-R gradient is "red to red" (degenerate -- shows
        ;      the base color).  The Copper rewrites this every 6 rows.
        move.l  #$00000F00, $00FE0180

        ; ---- per-plane templates for the HAM modify-R gradient ----
        ; Pixel 0 = $00 (mode 00, palette[0] -> reset to COLOR00).
        ; Pixels 1..255 = $20 | (x/16) (mode 10 modify-R, data 0..15).

        ; Plane 0 (data bit 0): 16 px off, 16 px on, repeating.
        move.l  #$0000FFFF, $00050000
        move.l  #$0000FFFF, $00050004
        move.l  #$0000FFFF, $00050008
        move.l  #$0000FFFF, $0005000C
        move.l  #$0000FFFF, $00050010
        move.l  #$0000FFFF, $00050014
        move.l  #$0000FFFF, $00050018
        move.l  #$0000FFFF, $0005001C

        ; Plane 1 (data bit 1)
        move.l  #$00000000, $00050020
        move.l  #$FFFFFFFF, $00050024
        move.l  #$00000000, $00050028
        move.l  #$FFFFFFFF, $0005002C
        move.l  #$00000000, $00050030
        move.l  #$FFFFFFFF, $00050034
        move.l  #$00000000, $00050038
        move.l  #$FFFFFFFF, $0005003C

        ; Plane 2 (data bit 2)
        move.l  #$00000000, $00050040
        move.l  #$00000000, $00050044
        move.l  #$FFFFFFFF, $00050048
        move.l  #$FFFFFFFF, $0005004C
        move.l  #$00000000, $00050050
        move.l  #$00000000, $00050054
        move.l  #$FFFFFFFF, $00050058
        move.l  #$FFFFFFFF, $0005005C

        ; Plane 3 (data bit 3)
        move.l  #$00000000, $00050060
        move.l  #$00000000, $00050064
        move.l  #$00000000, $00050068
        move.l  #$00000000, $0005006C
        move.l  #$FFFFFFFF, $00050070
        move.l  #$FFFFFFFF, $00050074
        move.l  #$FFFFFFFF, $00050078
        move.l  #$FFFFFFFF, $0005007C

        ; Plane 4 (mode bit 4 = 0 for mode 10): all zeros.
        move.l  #0, $00050080
        move.l  #0, $00050084
        move.l  #0, $00050088
        move.l  #0, $0005008C
        move.l  #0, $00050090
        move.l  #0, $00050094
        move.l  #0, $00050098
        move.l  #0, $0005009C

        ; Plane 5 (mode bit 5 = 1 for mode 10; pixel 0 has value 0).
        move.l  #$7FFFFFFF, $000500A0
        move.l  #$FFFFFFFF, $000500A4
        move.l  #$FFFFFFFF, $000500A8
        move.l  #$FFFFFFFF, $000500AC
        move.l  #$FFFFFFFF, $000500B0
        move.l  #$FFFFFFFF, $000500B4
        move.l  #$FFFFFFFF, $000500B8
        move.l  #$FFFFFFFF, $000500BC

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

        ; ---- build Copper list at $70000 ----
        ; 31 (WAIT, MOVE) pairs plus a HALT.  WAIT-RASTER target = $FFFFFFFC.
        ; MOVE writes COLOR00 ($00FE0180); the data field is the new RGB12.

        move.l  #$00070000, A0
        move.l  #color_table, A1
        moveq   #0, D1                  ; band index, 1..31
cl_loop:
        addq.l  #1, D1
        cmpi.l  #32, D1
        beq     cl_halt

        ; WAIT-RASTER for row (D1 * 6 - 1)
        move.l  #$FFFFFFFC, (A0)+
        move.l  D1, D2
        move.l  D2, D3
        lsl.l   #2, D3                  ; D1*4
        add.l   D2, D3                  ; D1*5
        add.l   D2, D3                  ; D1*6
        subq.l  #1, D3
        move.l  D3, (A0)+

        ; MOVE COLOR00 = color_table[D1]
        move.l  #$00FE0180, (A0)+
        ; color_table is .long[], so index D1 * 4
        move.l  D1, D2
        lsl.l   #2, D2
        move.l  #color_table, A2
        adda.l  D2, A2
        move.l  (A2), (A0)+

        bra     cl_loop
cl_halt:
        move.l  #$FFFFFFFF, (A0)+
        move.l  #0, (A0)+

        ; ---- configure Denise: BPLCON0 = nplanes=6 | HAM ----
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

        ; ---- start Copper at $70000 ----
        move.l  #$00070000, $00FE0040   ; COP1LC
        move.l  #1,          $00FE0044  ; COPJMP1

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
wait:   move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait

        stop    #0

; ============================================================
; fill_plane(D0 = plane index):
;   Blits the 32-byte template at $50000 + D0*32 over 192 rows
;   (16 words/row), rewinding the source pointer each row.
; ============================================================
fill_plane:
fp_w0:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w0

        move.l  D0, D1
        lsl.l   #5, D1
        addi.l  #$00050000, D1          ; src = template

        move.l  D0, D2
        moveq   #11, D3
        lsl.l   D3, D2                  ; D0 * $800
        move.l  D0, D3
        moveq   #12, D4
        lsl.l   D4, D3                  ; D0 * $1000
        add.l   D3, D2                  ; D0 * $1800
        addi.l  #$00060000, D2          ; dst = plane buffer

        move.l  #$F0000009, $00FE0000   ; LF=$F0 (A), USEA|USED
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  D1,         $00FE000C
        move.l  D2,         $00FE0018
        move.l  #-32,       $00FE001C   ; rewind APT each row
        move.l  #0,         $00FE0028
        move.l  #$00003010, $00FE0038   ; (192<<6)|16

fp_w1:  move.l  $00FE003C, D1
        andi.l  #1, D1
        bne     fp_w1
        rts

; ============================================================
; color_table: 32 hand-picked colors that walk around the hue wheel.
; ============================================================
color_table:
        .long  $00000F00       ; band  0 used as initial palette (see top)
        .long  $00000F40       ; band  1
        .long  $00000F80       ; band  2 orange
        .long  $00000FC0       ; band  3
        .long  $00000FF0       ; band  4 yellow
        .long  $00000CF0       ; band  5
        .long  $000008F0       ; band  6 lime
        .long  $000004F0       ; band  7
        .long  $000000F0       ; band  8 green
        .long  $000000F4       ; band  9
        .long  $000000F8       ; band 10 turquoise
        .long  $000000FC       ; band 11
        .long  $000000FF       ; band 12 cyan
        .long  $000000CF       ; band 13
        .long  $0000008F       ; band 14 sky blue
        .long  $0000004F       ; band 15
        .long  $0000000F       ; band 16 blue
        .long  $0000040F       ; band 17
        .long  $0000080F       ; band 18 purple
        .long  $00000C0F       ; band 19
        .long  $00000F0F       ; band 20 magenta
        .long  $00000F0C       ; band 21
        .long  $00000F08       ; band 22 pink
        .long  $00000F04       ; band 23
        .long  $00000F00       ; band 24 red
        .long  $00000F22       ; band 25 light pink
        .long  $00000F44       ; band 26
        .long  $00000F66       ; band 27
        .long  $00000F88       ; band 28 salmon
        .long  $00000FAA       ; band 29
        .long  $00000FCC       ; band 30
        .long  $00000FFF       ; band 31 white
