; Boing-style demo: single 16×16 red/white checkered sprite ball bouncing
; in a parabolic arc against a blue backdrop with a grid floor pattern.
;
; A homage to the legendary 1984 Amiga "Boing!" demo by RJ Mical and Dale
; Luck.  Ours is a single-sprite cartoon; no 3D, no sound (this turn).
;
; Display layout: 320×200 lo-res, 1 bitplane (for the floor grid) + the
; checkered ball as hardware sprite 0.  Background = COLOR00 (blue).
;
; The ball's animation comes from updating SPR0POS/SPR0CTL each frame in
; the main loop (no Copper magic — just busy-loop poll on VPOSR to know
; when to step the animation).

        .org $400

start:
        ; ---- zero bitplane 0 (8000 bytes at $5000..$6F40) ----
        move.l  #$00005000, A0
        move.l  #2000, D0
zero_bpl:
        clr.l   (A0)+
        subq.l  #1, D0
        bne     zero_bpl

        ; ---- draw the floor grid in BPL0 (lower half of screen) ----
        ; Horizontal grid lines every 8 rows in the lower 100 rows.
        ; Each row is 40 bytes wide (320 / 8).
        ; Start at row 100: $5000 + 100*40 = $5000 + 4000 = $5FA0
        move.l  #$00005FA0, A0
        move.l  #12, D0                          ; 12 horizontal lines
hgrid_loop:
        ; Fill 40 bytes with $FF (solid white horizontal line)
        moveq   #9, D1                           ; 10 longwords = 40 bytes
hgrid_row:
        move.l  #$FFFFFFFF, (A0)+
        dbra    D1, hgrid_row
        ; Advance A0 by 7 more rows (= 280 bytes) to next grid line.
        ; Already advanced by 40 bytes via the writes above.
        adda.l  #280, A0
        dbra    D0, hgrid_loop

        ; ---- write the 16×16 checkered ball sprite at $7000 ----
        ; Each sprite "line" is 4 bytes: { pos byte, ctl byte (only line 0),
        ; data word 0 = colorA bit, data word 1 = colorB bit }
        ; For lines 1..H: { pad, pad, dataA, dataB }
        ; (We use the typical "data only" pattern after line 0.)
        ;
        ; Ball pattern: alternating 4×4 colored blocks (color A vs color B).
        ; Rows 0-3: pattern AAAA BBBB AAAA BBBB
        ; Rows 4-7: pattern BBBB AAAA BBBB AAAA
        ; Rows 8-11: AAAA BBBB AAAA BBBB
        ; Rows 12-15: BBBB AAAA BBBB AAAA
        ;
        ; Data layout per row: DATAA word, then DATAB word
        ; If DATAA bit set & DATAB clear: color 1 (red)
        ; If DATAA clear & DATAB set:     color 2 (white)
        ; If both clear:                  color 0 (transparent backdrop)

        move.l  #$00007000, A0
        ; Line 0: SPR0POS = $4080 (vstart=64, hstart=128), CTL = $D880
        ; Line 0 encoding: { vstart_low byte, hstart>>1 byte, datA, datB }
        ; For 16x16 sprite at (128, 64): VSTART=64 ($40), VSTOP=80 ($50),
        ; HSTART=128 ($80, hstart>>1 = $40 — but actually HSTART is
        ; encoded as upper 8 bits of HSTART; for HSTART=128, byte = $40.
        ; Per Amiga HRM:
        ;   SPR0POS.h = (vstart & 0xFF)        <- vstart low 8 bits
        ;   SPR0POS.l = ((hstart >> 1) & 0xFF) <- hstart bits 8:1
        ;   SPR0CTL.h = (vstop & 0xFF)
        ;   SPR0CTL.l = (vstart8 << 2) | (vstop8 << 1) | (hstart_bit0)
        ; vstart=64 ($40), vstop=80 ($50), hstart=128 ($80)
        ;   POS = $4040  (high=vstart_low=64, low=hstart>>1=64)
        ;   CTL = $5000  (high=vstop_low=80, low=ctl bits = 0)

        move.w  #$4040, (A0)+                    ; SPR0POS
        move.w  #$5000, (A0)+                    ; SPR0CTL
        ; Rows 0-15: data word A, data word B
        ; Pattern: each row is 16 bits.  Checkered 4x4 blocks.
        ; A=$F0F0 means "color1 in cols 0-3,8-11" → top-left, top-right blocks
        ; B=$0F0F means "color2 in cols 4-7,12-15"
        ; Row 0..3 use this pattern
        move.l  #$F0F00F0F, (A0)+               ; row 0 (A, B)
        move.l  #$F0F00F0F, (A0)+               ; row 1
        move.l  #$F0F00F0F, (A0)+               ; row 2
        move.l  #$F0F00F0F, (A0)+               ; row 3
        ; Row 4..7: invert
        move.l  #$0F0FF0F0, (A0)+               ; row 4
        move.l  #$0F0FF0F0, (A0)+               ; row 5
        move.l  #$0F0FF0F0, (A0)+               ; row 6
        move.l  #$0F0FF0F0, (A0)+               ; row 7
        ; Row 8..11: same as 0..3
        move.l  #$F0F00F0F, (A0)+               ; row 8
        move.l  #$F0F00F0F, (A0)+               ; row 9
        move.l  #$F0F00F0F, (A0)+               ; row 10
        move.l  #$F0F00F0F, (A0)+               ; row 11
        ; Row 12..15: invert again
        move.l  #$0F0FF0F0, (A0)+               ; row 12
        move.l  #$0F0FF0F0, (A0)+               ; row 13
        move.l  #$0F0FF0F0, (A0)+               ; row 14
        move.l  #$0F0FF0F0, (A0)+               ; row 15
        ; Terminator (end of sprite data)
        move.l  #$00000000, (A0)+

        ; ---- palette ----
        ; COLOR00 = blue backdrop ($00F)
        move.l  #$0000000F, $00FE0180
        ; BPL color 1 = bright cyan-white (floor grid)
        move.l  #$00000FFF, $00FE0184
        ; SPR0 color 1 (= COLOR17 / $00FE01A2) = red
        move.l  #$00000F00, $00FE01A2
        ; SPR0 color 2 (= COLOR18 / $00FE01A4) = white
        move.l  #$00000FFF, $00FE01A4
        ; SPR0 color 3 (= COLOR19 / $00FE01A6) = unused → grey
        move.l  #$00000999, $00FE01A6

        ; ---- BPLCON0 = 1 plane, lo-res, sprites enabled ----
        move.l  #$00001000, $00FE0100           ; BPU=1
        move.l  #0,         $00FE0104           ; BPLCON1
        move.l  #0,         $00FE0108           ; BPL1MOD
        move.l  #$00005000, $00FE0110           ; BPL1PT = $5000
        move.l  #0,         $00FE0128           ; BPL2MOD

        ; ---- All 8 sprite pointers point to our sprite data ----
        move.l  #$00007000, $00FE0120           ; SPR0PT
        ; Other sprites disabled (= point at empty data)
        move.l  #$00007020, $00FE0124           ; SPR1PT (terminator slot)
        move.l  #$00007020, $00FE0130           ; SPR2..7 share terminator
        move.l  #$00007020, $00FE0134
        move.l  #$00007020, $00FE0138
        move.l  #$00007020, $00FE013C
        move.l  #$00007020, $00FE0140
        move.l  #$00007020, $00FE0144

        ; ---- animation loop ----
        ; The ball follows a parabolic bounce: x increments linearly,
        ; y = base + amplitude × |sin(phase)|.
        ;
        ; Implement with integer phase counter (0..255) and a small
        ; sin-like table lookup (or just a triangle wave).

        moveq   #0, D7                          ; frame counter
main_loop:
        ; phase_x = (frame >> 1) & 255      → 0..255 looping
        ; phase_y = (frame >> 0) & 63       → 0..63 (faster cycle)

        ; X position: bounce between 32 and 256
        ;   tri_x(phase) = abs((phase & 511) - 256) - 128 + 64  → 0..128 range
        move.l  D7, D0
        andi.l  #$1FF, D0                       ; phase mod 512
        subi.l  #256, D0
        tst.l   D0
        bge     x_pos_done
        neg.l   D0
x_pos_done:
        ; D0 is now 0..255.  Map to screen X 64..256.
        ; Simpler: just use D0/2 + 64 → 64..191
        lsr.l   #1, D0
        addi.l  #64, D0                         ; D0 = sprite X
        ; Convert to SPR0POS encoding:
        ;   POS.high = vstart & 0xFF (Y position)
        ;   POS.low  = (X >> 1) & 0xFF
        ; We compute Y separately.

        ; Y position: bounce between 50 (top) and 150 (floor)
        ;   tri_y(phase) = | (phase & 127) - 64 |
        move.l  D7, D1
        andi.l  #$7F, D1                        ; 0..127
        subi.l  #64, D1
        tst.l   D1
        bge     y_pos_done
        neg.l   D1
y_pos_done:
        ; D1 is now 0..64.  Map to 80 .. 144 (bouncing in screen middle).
        addi.l  #80, D1                         ; D1 = sprite Y (vstart)

        ; Compute SPR0POS = (Y << 8) | (X >> 1)
        ; Need only low 8 bits of Y and (X>>1).
        lsl.l   #8, D1                          ; Y in high byte
        move.l  D0, D2
        lsr.l   #1, D2
        andi.l  #$FF, D2
        or.l    D2, D1                          ; D1 = POS

        move.w  D1, $00FE0140                   ; SPR0POS (legacy)

        ; SPR0CTL: VSTOP = VSTART + 16
        ;          CTL.l = (VSTART_bit8 << 2) | (VSTOP_bit8 << 1) | hstart_bit0
        ; For Y in 80..144 (all below 256) VSTART_bit8 = 0.
        ; VSTOP = VSTART + 16 ≤ 160 → bit8 = 0.
        ; hstart_bit0 = X & 1.
        move.l  D0, D3
        andi.l  #1, D3                           ; hstart_bit0
        ; Compute VSTOP byte
        move.l  D1, D2
        lsr.l   #8, D2                           ; D2 = VSTART_lo (was Y)
        addi.l  #16, D2
        lsl.l   #8, D2                           ; VSTOP in high byte of CTL
        or.l    D3, D2                           ; OR in hstart_bit0
        move.w  D2, $00FE0142                   ; SPR0CTL

        ; Increment frame counter.
        addq.l  #1, D7

        ; Busy-wait so the animation isn't infinity-fast.
        ; Simple inner loop ≈ 200 instructions / frame.
        move.l  #2000, D2
delay_loop:
        subq.l  #1, D2
        bne     delay_loop

        ; Halt criterion: after some frames, stop so test is deterministic.
        cmpi.l  #240, D7                        ; ~4 seconds @ 60fps
        bge     end_demo

        bra     main_loop

end_demo:
        stop    #0
