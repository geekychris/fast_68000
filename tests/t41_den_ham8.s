; HAM8 (8-plane HAM): BPLCON0 = HAM=1, BPU3=1.
;
; HAM8 encoding:
;   pv[0] (plane 0 = BPL1) -> control bit 0
;   pv[1] (plane 1 = BPL2) -> control bit 1
;   pv[2..7] (BPL3..BPL8)  -> 6-bit data
;
;   ctl 00: palette lookup, index = data.
;
; Plane 0..7 buffers spaced $2000 apart at $20000+ (each plane needs
; 256/16*192 = 3072 words = 6144 bytes; $2000 leaves room).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- zero all 8 plane buffers ($20000..$2FFFF, 64 KB) ----
        move.l  #$00020000, A0
        move.l  #16384, D1
zp:     move.l  #0, (A0)+
        subq.l  #1, D1
        bne     zp

        ; Plane 2 (BPL3 = HAM8 data bit 0) row 0 word 0 = $8000 ->
        ; pixel (0,0) data = 1, ctl = 00 -> palette[1].
        move.l  #$80000000, $00024000

        ; ---- palette ----
        move.l  #$000000F0, $00FE0180    ; COLOR00 = green
        move.l  #$00000F00, $00FE0184    ; COLOR01 = red

        ; ---- Denise: BPLCON0 = HAM=1, BPU3=1 ($0810) ----
        move.l  #$00000810, $00FE0100
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108

        ; Bitplane pointers (planes 0..7 at $20000+i*$2000)
        move.l  #$00020000, $00FE0110
        move.l  #$00022000, $00FE0114
        move.l  #$00024000, $00FE0118    ; BPL3PT (data bit 0)
        move.l  #$00026000, $00FE011C
        move.l  #$00028000, $00FE0120
        move.l  #$0002A000, $00FE0124
        move.l  #$0002C000, $00FE0300    ; BPL7PT (bank 1)
        move.l  #$0002E000, $00FE0304    ; BPL8PT (bank 1)
        move.l  #0,         $00FE0128
        move.l  #0,         $00FE012C

        ; ---- trigger Denise ----
        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ---- check pixel (0,0) -> palette[1] = red ($E0) ----
        move.l  #0, D3
        move.b  $00010000, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_a

        ; ---- check pixel (1,0) -> all planes zero -> palette[0] = green ($1C) ----
        move.l  #0, D3
        move.b  $00010001, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_b

        ; ---- check pixel (0,1) -> row 1 all-zero -> green ----
        move.l  #0, D3
        move.b  $00010100, D3
        andi.l  #$FF, D3
        cmpi.l  #$1C, D3
        bne     fail_c

        stop    #0

fail_a: stop #$D810
fail_b: stop #$D811
fail_c: stop #$D812
