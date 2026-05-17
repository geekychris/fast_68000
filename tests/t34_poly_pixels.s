; Mimic one frame of the polygon demo: clear, three edges, fill.
; Then check that the bitplane has set bits in the triangle interior.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        bsr     clear_bp

        ; Triangle A=(128,66), B=(88,126), C=(168,126)
        moveq   #-128, D0
        andi.l  #$FF, D0          ; just to have a defined zero - actually use immediates below

        ; edge A->B
        move.l  #128, D0
        move.l  #66,  D1
        move.l  #88,  D2
        move.l  #126, D3
        bsr     draw_line

        ; edge B->C
        move.l  #88,  D0
        move.l  #126, D1
        move.l  #168, D2
        move.l  #126, D3
        bsr     draw_line

        ; edge C->A
        move.l  #168, D0
        move.l  #126, D1
        move.l  #128, D2
        move.l  #66,  D3
        bsr     draw_line

        ; After the edges, check that SOME bit is set.
        move.l  #$00020000, A0
        move.l  #1536, D4
        moveq   #0, D5
ck1:    move.l  (A0)+, D6
        or.l    D6, D5
        subq.l  #1, D4
        bne     ck1
        tst.l   D5
        beq     fail_no_edges

        ; Now fill.
        bsr     fill_bitplane

        ; Check a known interior pixel: (128, 96).
        ; byte offset = 96*32 + (128/16)*2 = 3072 + 16 = 3088 = $C10
        ; address = $00020C10
        ; bit-in-word = 15 - (128 mod 16) = 15 - 0 = 15 -> bit mask $8000
        move.w  $00020C10, D0
        andi.w  #$8000, D0
        beq     fail_no_interior

        stop    #0

fail_no_edges:    stop #$70E0
fail_no_interior: stop #$70E1

; ============================================================
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

fill_bitplane:
fb_w0:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w0
        move.l  #$F0004009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  #$00020000, $00FE000C
        move.l  #$00020000, $00FE0018
        move.l  #0,         $00FE001C
        move.l  #0,         $00FE0028
        move.l  #$00003010, $00FE0038
fb_w1:  move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     fb_w1
        rts

draw_line:
dl_w0:  move.l  $00FE003C, D4
        andi.l  #1, D4
        bne     dl_w0
        move.l  D0, $00030000
        move.l  D1, $00030004
        sub.l   D0, D2
        moveq   #0, D6
        tst.l   D2
        bpl     dl_dx_done
        neg.l   D2
        moveq   #1, D6
dl_dx_done:
        sub.l   D1, D3
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
