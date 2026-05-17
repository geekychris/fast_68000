; Denise sprite demo: 8 hardware sprites bouncing across the screen.
;
; Each sprite shares the same 16x16 ball-shape data at $00007000..$0000701F.
; Sprites are stacked vertically with each one moving at a different
; horizontal velocity, producing a row of bouncing balls in 8 colors over a
; solid-blue background.
;
; Background: nplanes=1, plane 0 all zeros, COLOR00 = blue.
; Sprite palette: COLOR08..COLOR0F = eight distinct hues.

        .org $400

start:
        ; ---- zero plane 0 (6144 bytes at $00005000..$000067FF) ----
        move.l  #$00005000, A0
        move.l  #1536, D0
zp:     move.l  #0, (A0)+
        subq.l  #1, D0
        bne     zp

        ; ---- write the 16x16 ball sprite at $00007000 ----
        ;   16 words; packed two-per-long.
        move.l  #$00000FF0, $00007000          ; row 0 = $0000, row 1 = $0FF0
        move.l  #$3FFC7FFE, $00007004          ; row 2 = $3FFC, row 3 = $7FFE
        move.l  #$FFFFFFFF, $00007008          ; row 4, row 5
        move.l  #$FFFFFFFF, $0000700C          ; row 6, row 7
        move.l  #$FFFFFFFF, $00007010          ; row 8, row 9
        move.l  #$FFFFFFFF, $00007014          ; row 10, row 11
        move.l  #$7FFE3FFC, $00007018          ; row 12 = $7FFE, row 13 = $3FFC
        move.l  #$0FF00000, $0000701C          ; row 14 = $0FF0, row 15 = $0000

        ; ---- palette ----
        move.l  #$0000000F, $00FE0180          ; COLOR00 blue (background)
        move.l  #$00000FFF, $00FE01A0          ; COLOR08 white
        move.l  #$00000F00, $00FE01A4          ; COLOR09 red
        move.l  #$000000F0, $00FE01A8          ; COLOR0A green
        move.l  #$00000FF0, $00FE01AC          ; COLOR0B yellow
        move.l  #$00000F0F, $00FE01B0          ; COLOR0C magenta
        move.l  #$000000FF, $00FE01B4          ; COLOR0D cyan
        move.l  #$00000F80, $00FE01B8          ; COLOR0E orange
        move.l  #$00000F88, $00FE01BC          ; COLOR0F pink

        ; ---- BPLCON0 = 1 plane ----
        move.l  #$00001000, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108
        move.l  #$00005000, $00FE0110
        move.l  #0,         $00FE0128

        ; ---- All sprites point to the same data, height = 16 ----
        move.l  #$00007000, $00FE0130          ; SPR0PT
        move.l  #$00007000, $00FE0138          ; SPR1PT
        move.l  #$00007000, $00FE0148          ; SPR2PT
        move.l  #$00007000, $00FE0150          ; SPR3PT
        move.l  #$00007000, $00FE0158          ; SPR4PT
        move.l  #$00007000, $00FE0160          ; SPR5PT
        move.l  #$00007000, $00FE0168          ; SPR6PT
        move.l  #$00007000, $00FE0178          ; SPR7PT

        ; ---- enable all 8 sprites ----
        move.l  #$000000FF, $00FE0170

        moveq   #0, D7                          ; frame counter

main_loop:
        ; For each sprite i, compute X = abs((frame + 32*i) mod 480 - 240) - i*?
        ; Simpler: X bounces 0..240 using a triangle wave on (frame * speed_i).
        ; Each sprite gets a different speed and a different Y row.
        ;
        ; Loop unrolled per sprite.
        moveq   #0, D6                          ; sprite index
spr_loop:
        ; phase = (frame * (D6+1)) & 511
        move.l  D7, D0
        addq.l  #1, D6                          ; speed = i+1
        ; multiply: D0 *= D6 via repeated addition (small numbers)
        move.l  D0, D1
        move.l  D6, D2
        moveq   #0, D3                          ; product
mul_lp: tst.l   D2
        beq     mul_done
        add.l   D1, D3
        subq.l  #1, D2
        bra     mul_lp
mul_done:
        andi.l  #511, D3
        ; triangle: if D3 >= 256 -> 511-D3 else D3
        cmpi.l  #256, D3
        blt     tri_ok
        move.l  #511, D4
        sub.l   D3, D4
        move.l  D4, D3
tri_ok:
        ; D3 is now in [0..255], use as X offset clamped.
        ; Cap at 240 (sprite is 16 wide, screen is 256).
        cmpi.l  #240, D3
        ble     x_ok
        move.l  #240, D3
x_ok:
        subq.l  #1, D6                          ; restore i
        ; Y position: i * 22 (so 8 sprites fit in 0..192)
        move.l  D6, D4
        lsl.l   #1, D4                          ; D4 = i*2
        move.l  D6, D5
        lsl.l   #4, D5                          ; D5 = i*16
        add.l   D5, D4                          ; D4 = i*18
        addq.l  #5, D4                          ; D4 = i*18 + 5 -> 5,23,41,...
        ; Build SPRxCTL = (Y<<24) | (X<<16) | (16<<8) | color
        lsl.l   #8, D4                          ; D4 = Y<<8
        lsl.l   #8, D4                          ; D4 = Y<<16 ... oops
        lsl.l   #8, D4                          ; D4 = Y<<24
        lsr.l   #8, D4                          ; back to Y<<16... let's restart
        ; Easier: rebuild from scratch.
        move.l  D6, D4
        lsl.l   #1, D4
        move.l  D6, D5
        lsl.l   #4, D5
        add.l   D5, D4
        addq.l  #5, D4                          ; D4 = Y
        moveq   #24, D5
        lsl.l   D5, D4                          ; D4 = Y<<24
        ; X<<16
        move.l  D3, D5
        moveq   #16, D2
        lsl.l   D2, D5
        or.l    D5, D4
        ; height<<8 = 16<<8 = $1000
        ori.l   #$00001000, D4
        ; color = $08 + i
        move.l  D6, D5
        addi.l  #8, D5
        or.l    D5, D4

        ; Write SPRxCTL according to D6.
        cmpi.l  #0, D6
        beq     wctl0
        cmpi.l  #1, D6
        beq     wctl1
        cmpi.l  #2, D6
        beq     wctl2
        cmpi.l  #3, D6
        beq     wctl3
        cmpi.l  #4, D6
        beq     wctl4
        cmpi.l  #5, D6
        beq     wctl5
        cmpi.l  #6, D6
        beq     wctl6
        bra     wctl7
wctl0:  move.l  D4, $00FE0134
        bra     wctl_done
wctl1:  move.l  D4, $00FE013C
        bra     wctl_done
wctl2:  move.l  D4, $00FE014C
        bra     wctl_done
wctl3:  move.l  D4, $00FE0154
        bra     wctl_done
wctl4:  move.l  D4, $00FE015C
        bra     wctl_done
wctl5:  move.l  D4, $00FE0164
        bra     wctl_done
wctl6:  move.l  D4, $00FE016C
        bra     wctl_done
wctl7:  move.l  D4, $00FE017C
wctl_done:
        addq.l  #1, D6
        cmpi.l  #8, D6
        blt     spr_loop

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
wait:   move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait

        ; pacing
        move.l  #200000, D0
pace:   subq.l  #1, D0
        bne     pace

        addq.l  #1, D7
        bra     main_loop
