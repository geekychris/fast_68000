; kickstart_demo.s -- exercises the canonical Amiga chipset paths.
;
;   1. Canonical Blitter ($DFF040..$DFF066) -- bit-translated by the
;      bus into the internal blitter engine.  Stamps a 16-px-on /
;      16-px-off vertical-stripe pattern across a 192-row bitplane.
;   2. Canonical Copper ($DFF080..$DFF088) -- COP1LCH/L + COPJMP1.
;      An 8-band Copper list rewrites COLOR00 + COLOR01 every 24
;      scanlines, painting the stripes in 8 different colour pairs.
;   3. VERTB IRQ (Agnus -> Paula INTREQ[5] -> INT3) -- handler
;      increments a frame-counter at $40000.  Demo halts after one
;      Denise render and verifies the counter advanced.
;
; The window shows a "scrolling rainbow" stripe pattern after Denise
; renders.  The VERTB counter is verified at halt (halt 0 = pass).

        .org $6C                          ; INT3 = level 3 autovector
        .long vertb_handler

        .org $400

start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Phase / frame counter at $40000.
        move.l  #0, $00040000

        ; -- canonical Blitter: stamp 16/16 stripes across 192 rows --
        move.l  #$FFFF0000, $00030000
        move.l  #$FFFF0000, $00030004
        move.l  #$FFFF0000, $00030008
        move.l  #$FFFF0000, $0003000C

        move.w  #$09F0, $00DFF040           ; BLTCON0: USEA|USED|LF=A
        move.w  #$0000, $00DFF042           ; BLTCON1: no flags
        move.w  #$FFFF, $00DFF044           ; BLTAFWM
        move.w  #$FFFF, $00DFF046           ; BLTALWM
        move.w  #$0003, $00DFF050           ; BLTAPTH
        move.w  #$0000, $00DFF052           ; BLTAPTL ($00030000)
        move.w  #$0003, $00DFF054           ; BLTDPTH
        move.w  #$0010, $00DFF056           ; BLTDPTL ($00030010)
        move.w  #$FFF0, $00DFF064           ; BLTAMOD -16 (rewind each row)
        move.w  #$0000, $00DFF066           ; BLTDMOD
        move.w  #$2FC8, $00DFF058           ; BLTSIZE: height=191, width=8

wait_blit:
        move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     wait_blit

        ; -- Denise config (BPU=1, BPL1PT=$30000) --
        move.l  #$00001000, $00FE0100       ; BPLCON0: BPU=1
        move.l  #0,         $00FE0104
        move.l  #0,         $00FE0108
        move.l  #$00030000, $00FE0110       ; BPL1PT
        move.l  #$00000000, $00FE0180       ; COLOR00 seed = black
        move.l  #$00000FFF, $00FE0184       ; COLOR01 seed = white

        ; -- build 8-band Copper list at $50000 --
        bsr     build_cop_list

        ; -- arm VERTB IRQ (INT3) --
        andi.w  #$F8FF, SR
        move.w  #$C020, $00FE029A           ; INTENA SET INTEN+VERTB

        ; -- start Copper via canonical $DFF080/$DFF082/$DFF088 --
        move.w  #$0005, $00DFF080           ; COP1LCH
        move.w  #$0000, $00DFF082           ; COP1LCL = $00050000
        move.w  #$0001, $00DFF088           ; COPJMP1

        ; -- trigger Denise (one full rasterisation) --
        move.l  #1, $00FE0140
wait_den:
        move.l  $00FE0144, D0
        andi.l  #1, D0
        bne     wait_den

        ; -- give the simulator time for a few more VERTB pulses --
        move.l  #500000, D7
spin:   subq.l  #1, D7
        bne     spin

        ; -- mask interrupts before reading the counter (prevents
        ;    handler from firing mid-readback) --
        ori.w   #$0700, SR

        ; -- verify VERTB fired (phase counter > 0) --
        move.l  $00040000, D0
        tst.l   D0
        beq     fail_no_vertb

        stop    #0

fail_no_vertb:
        stop    #$DE13

; ====================================================================
; build_cop_list: emits 16 (WAIT, MOVE) pairs at $00050000:
;   8 x (WAIT row=N*24, MOVE $DFF180 (COLOR00) = palette_a[N])
;   8 x (WAIT row=N*24, MOVE $DFF184 (COLOR01) = palette_b[N])
; followed by HALT.
; ====================================================================
build_cop_list:
        move.l  #$00050000, A0
        moveq   #0, D1
bl_loop:
        cmpi.l  #8, D1
        beq     bl_halt

        ; WAIT row = D1 * 24
        move.l  #$FFFFFFFC, (A0)+
        move.l  D1, D2
        lsl.l   #3, D2
        move.l  D2, D3
        add.l   D2, D3
        add.l   D2, D3                      ; D3 = D1 * 24
        move.l  D3, (A0)+

        ; MOVE COLOR00 = palette_a[D1]
        move.l  #$00FE0180, (A0)+
        move.l  D1, D2
        lsl.l   #2, D2
        move.l  #palette_a, A1
        adda.l  D2, A1
        move.l  (A1), (A0)+

        ; second pair: MOVE COLOR01 = palette_b[D1]
        move.l  #$FFFFFFFC, (A0)+
        move.l  D3, (A0)+
        move.l  #$00FE0184, (A0)+
        move.l  D1, D2
        lsl.l   #2, D2
        move.l  #palette_b, A1
        adda.l  D2, A1
        move.l  (A1), (A0)+

        addq.l  #1, D1
        bra     bl_loop
bl_halt:
        move.l  #$FFFFFFFF, (A0)+
        move.l  #0, (A0)+
        rts

; ====================================================================
; VERTB IRQ: increment $40000 (uses D6 saved/restored via stack).
; ====================================================================
vertb_handler:
        move.l  D6, -(A7)
        move.l  $00040000, D6
        addq.l  #1, D6
        move.l  D6, $00040000
        move.w  #$0020, $00FE029C           ; INTREQ CLR VERTB
        move.l  (A7)+, D6
        rte

; ====================================================================
palette_a:
        .long $00000F00       ; red
        .long $00000F80       ; orange
        .long $00000FF0       ; yellow
        .long $000000F0       ; green
        .long $000000FF       ; cyan
        .long $0000000F       ; blue
        .long $00000F0F       ; magenta
        .long $00000FFF       ; white

palette_b:
        .long $00000FFF
        .long $00000F0F
        .long $0000000F
        .long $000000FF
        .long $000000F0
        .long $00000FF0
        .long $00000F80
        .long $00000F00
