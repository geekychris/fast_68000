; Blitter line-drawing tests (Bresenham, Amiga octants).
;
;   Part 1: single-pixel sanity at (x=3, y=0).
;           Expected D @ $3000 high half = $1000  (bit 12 set).
;
;   Part 2: 4-pixel horizontal line (0,0) to (3,0).
;           Expected D @ $3000 high half = $F000  (bits 15..12 set).
;
;   Part 3: 4-pixel diagonal (0,0) to (3,3) using BLTCMOD=2 so a Y-step
;           advances by one 16-bit word (= one row of a 1-word-wide
;           virtual scanline).  Expected:
;             D @ $3000 high half = $8000  (pixel x=0, y=0 -> bit 15)
;             D @ $3002 high half = $4000  (x=1, y=1 -> bit 14)
;             D @ $3004 high half = $2000  (x=2, y=2 -> bit 13)
;             D @ $3006 high half = $1000  (x=3, y=3 -> bit 12)
;
; All three parts share one BLTCON template (LF=$CA, line mode, USEA|USEB|
; USEC|USED).  Each part reprograms BLTAPT/CPT/DPT/SIZE.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Part 1: single pixel at x=3 ----
        move.l #0,         $00003000
        move.l #$CA30080F, $00FE0000   ; LF=$CA, ASH=3, LINE=1, channels $F
        move.l #$0000FFFF, $00FE0004
        move.l #$0000FFFF, $00FE0008
        move.l #$FFFFFFFA, $00FE000C
        move.l #0,         $00FE0010
        move.l #$00003000, $00FE0014
        move.l #$00003000, $00FE0018
        move.l #0,         $00FE001C
        move.l #0,         $00FE0020
        move.l #32,        $00FE0024
        move.l #32,        $00FE0028
        move.l #$0000FFFF, $00FE0030   ; BLTBDAT = solid pattern
        move.l #$00000042, $00FE0038   ; height=1 px, width=2

w1:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w1
        move.l $00003000, D1
        cmpi.l #$10000000, D1
        bne    fail1

        ; ---- Part 2: 4-pixel horizontal line ----
        move.l #0,         $00003000
        move.l #$CA00080F, $00FE0000   ; ASH=0 this time
        move.l #$FFFFFFFA, $00FE000C
        move.l #0,         $00FE0010
        move.l #$00003000, $00FE0014
        move.l #$00003000, $00FE0018
        move.l #$FFFFFFF4, $00FE001C
        move.l #0,         $00FE0020
        move.l #32,        $00FE0024
        move.l #32,        $00FE0028
        move.l #$00000102, $00FE0038   ; height=4, width=2

w2:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w2
        move.l $00003000, D2
        cmpi.l #$F0000000, D2
        bne    fail2

        ; ---- Part 3: 4-pixel diagonal (0,0)->(3,3) ----
        ; dx = dy = 3. accumulator init = 4*dy - 2*dx = 12-6 = 6 (positive,
        ; so the very first step is dom+sub; then APT += AMOD = 4*(dy-dx) = 0.
        ; APT stays 6 throughout, so every step is dom+sub).  Octant 0:
        ; dominant=X+, subordinate=Y+.
        ;
        ; We use a 2-byte scanline stride (BLTCMOD=BLTDMOD=2) so each Y step
        ; lands on the next word.
        move.l #0,         $00003000
        move.l #0,         $00003004
        move.l #$CA00080F, $00FE0000
        move.l #$00000006, $00FE000C   ; BLTAPT = +6
        move.l #0,         $00FE0010
        move.l #$00003000, $00FE0014
        move.l #$00003000, $00FE0018
        move.l #0,         $00FE001C   ; BLTAMOD = 0
        move.l #0,         $00FE0020   ; BLTBMOD = 0
        move.l #2,         $00FE0024   ; BLTCMOD = 2 bytes per "row"
        move.l #2,         $00FE0028   ; BLTDMOD = 2
        move.l #$00000102, $00FE0038   ; height=4, width=2

w3:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w3

        ; Verify each row's high half independently.
        move.w $00003000, D3
        cmpi.w #$8000, D3
        bne    fail3a
        move.w $00003002, D4
        cmpi.w #$4000, D4
        bne    fail3b
        move.w $00003004, D5
        cmpi.w #$2000, D5
        bne    fail3c
        move.w $00003006, D6
        cmpi.w #$1000, D6
        bne    fail3d

        stop   #0

fail1:  stop #$bd41
fail2:  stop #$bd42
fail3a: stop #$bd43
fail3b: stop #$bd44
fail3c: stop #$bd45
fail3d: stop #$bd46
