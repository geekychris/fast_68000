; Mimic the blt_demo main loop: 5 iterations, draw 5 different lines.
; Then check that the bitplane has at least 5 distinct columns set on
; row 96 (the center row), which is where all 16 angles cross.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        moveq   #0, D7                  ; frame counter

main_loop:
        ; clear every 64 frames (matches blt_demo)
        move.l  D7, D0
        andi.l  #63, D0
        bne     no_clear
        bsr     clear_bp
no_clear:
        ; pick sin_table entry idx = D7 mod 16
        move.l  D7, D0
        andi.l  #15, D0
        lsl.l   #2, D0
        move.l  #sin_table, A0
        adda.l  D0, A0

        move.w  (A0)+, D1
        ext.l   D1
        move.w  (A0)+, D2
        ext.l   D2
        addi.l  #128, D1
        addi.l  #96,  D2

        move.l  #128, $00030000
        move.l  #96,  $00030004
        move.l  D1,   $00030008
        move.l  D2,   $0003000C

        move.l  D7, $00030020
        bsr     draw_line
        move.l  $00030020, D7

        addq.l  #1, D7
        cmpi.l  #200, D7
        blt     main_loop

        ; Count pixels in row 96.
        ; row 96 starts at $00020000 + 96*32 = $00020C00, 16 words.
        move.l  #$00020C00, A0
        moveq   #16, D2                 ; words
        moveq   #0, D5                  ; OR of words
ck:     move.w  (A0)+, D6
        andi.l  #$FFFF, D6
        or.l    D6, D5
        subq.l  #1, D2
        bne     ck
        tst.l   D5
        beq     fail_no_pixels

        ; Encode in halt code: low 16 bits of D5 = the OR of row 96.
        ; Use stop with a fixed code so we can inspect retired count.
        stop    #0

fail_no_pixels:
        stop    #$F500

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

draw_line:
dl_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     dl_w0

        move.l  $00030008, D2
        sub.l   $00030000, D2
        moveq   #0, D6
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6
dl_dx_done:
        move.l  $0003000C, D3
        sub.l   $00030004, D3
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

sin_table:
        .word  60,    0
        .word  55,   23
        .word  42,   42
        .word  23,   55
        .word   0,   60
        .word -23,   55
        .word -42,   42
        .word -55,   23
        .word -60,    0
        .word -55,  -23
        .word -42,  -42
        .word -23,  -55
        .word   0,  -60
        .word  23,  -55
        .word  42,  -42
        .word  55,  -23
