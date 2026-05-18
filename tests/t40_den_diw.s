; Display Window (DIWSTRT/DIWSTOP).
; Plane 0 all-ones -> playfield = COLOR01 (red) over the full 256x192 buffer
; if DIW were unrestricted.  Set DIWSTRT=(64,32), DIWSTOP=(192,160).
;   Inside window (x in [64,192), y in [32,160)) -> red ($E0).
;   Outside window                                 -> COLOR00 = blue ($03).
; Sample pixels at the four corners + one outside.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- fill plane 0 with all-ones ----
        move.l  #$00005000, A0
        move.l  #1536, D1
fill_p: move.l  #$FFFFFFFF, (A0)+
        subq.l  #1, D1
        bne     fill_p

        ; ---- Denise: nplanes=1, palette ----
        move.l  #$00001000, $00FE0100   ; BPLCON0
        move.l  #0,         $00FE0104   ; BPLCON1
        move.l  #0,         $00FE0108   ; BPLCON2
        move.l  #$00005000, $00FE0110   ; BPL1PT
        move.l  #0,         $00FE0128   ; BPL1MOD
        move.l  #$0000000F, $00FE0180   ; COLOR00 = blue
        move.l  #$00000F00, $00FE0184   ; COLOR01 = red

        ; ---- DIW: start (64, 32), stop (192, 160) ----
        move.l  #$00200040, $00FE010C   ; DIWSTRT: Y_start=$20=32, X_start=$40=64
        move.l  #$00A000C0, $00FE0174   ; DIWSTOP: Y_stop=$A0=160, X_stop=$C0=192

        ; ---- trigger ----
        move.l  #1, $00FE0140
w1:     move.l  $00FE0144, D2
        andi.l  #1, D2
        bne     w1

        ; ---- check pixel (64, 32) inside top-left corner: $10000 + 32*256 + 64 = $12040 ----
        move.l  #0, D3
        move.b  $00012040, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_a

        ; ---- check pixel (191, 159) inside bottom-right: $10000 + 159*256 + 191 = $19FBF ----
        move.l  #0, D3
        move.b  $00019FBF, D3
        andi.l  #$FF, D3
        cmpi.l  #$E0, D3
        bne     fail_b

        ; ---- check pixel (63, 32) just outside left edge: $1203F ----
        move.l  #0, D3
        move.b  $0001203F, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_c

        ; ---- check pixel (192, 32) just outside right edge: $12040 + 192 = $12100 ----
        move.l  #0, D3
        move.b  $00012100, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_d

        ; ---- check pixel (64, 31) just above top: $10000 + 31*256 + 64 = $11F40 ----
        move.l  #0, D3
        move.b  $00011F40, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_e

        ; ---- check pixel (64, 160) just below bottom: $10000 + 160*256 + 64 = $1A040 ----
        move.l  #0, D3
        move.b  $0001A040, D3
        andi.l  #$FF, D3
        cmpi.l  #$03, D3
        bne     fail_f

        stop    #0

fail_a: stop #$D101
fail_b: stop #$D102
fail_c: stop #$D103
fail_d: stop #$D104
fail_e: stop #$D105
fail_f: stop #$D106
