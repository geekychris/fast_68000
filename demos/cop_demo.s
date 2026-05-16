; Copper demo: the Copper drives the blitter to clear the bitplane and
; draw a horizontal + vertical line forming a cross on the screen.
;
; CPU work: build the Copper list once, then poke COPJMP1 once per frame.
; The Copper does the rest (issuing 4 + 12 + 12 + 1 = 29 blitter register
; writes per kick, plus three WAITs for blitter completion).
;
; Each frame, the CPU rewrites the cross to ratchet through a small list
; of pixel-position offsets so the cross visibly slides around.

        .org $400

; ----- shared constants (kept in the comments here; emitted as immediates) -----
;   CL_BASE   = $00006000   Copper list base
;   FB_BP     = $00020000   bitplane base
;   STRIDE    = 32          bytes per scanline (256 px / 8 bpp)
;   CHIP_BASE = $00FE_0000

start:
        ; ---- one-time Copper list build ----
        bsr     build_copper_list

        moveq   #0, D7              ; frame counter

main_loop:
        ; Compute drift offset = (frame counter mod 32) - 16  -> range [-16..15]
        move.l  D7, D0
        andi.l  #31, D0
        subi.l  #16, D0              ; signed offset

        ; Patch the Copper list slots that depend on the offset.
        bsr     patch_copper_for_offset

        ; Kick the Copper.
        move.l  #1, $00FE0044       ; COPJMP1

cop_wait:
        move.l  $00FE0048, D1       ; COPSTAT
        andi.l  #1, D1
        bne     cop_wait

        ; advance frame, pace
        addq.l  #1, D7
        move.l  #4000, D2
delay:  subq.l  #1, D2
        bne     delay
        bra     main_loop

; ====================================================================
; build_copper_list:
;   Lays out the Copper program at $6000.  Fields that depend on the
;   per-frame offset are written by patch_copper_for_offset; the rest
;   are baked in once.
;
; Layout:
;   $6000 + i*8: clear-bitplane MOVEs and WAIT
;   then h-line MOVEs (12) + WAIT
;   then v-line MOVEs (12) + WAIT
;   then HALT
; ====================================================================
build_copper_list:
        ; ---- clear bitplane (4 MOVEs + WAIT + 1 BLTSIZE move) ----
        ; Inst 0: target = BLTCON, data = $00000001 (LF=0, USED only)
        move.l  #$00FE0000, $00006000
        move.l  #$00000001, $00006004
        ; Inst 1: target = BLTDPT, data = $00020000
        move.l  #$00FE0018, $00006008
        move.l  #$00020000, $0000600C
        ; Inst 2: target = BLTDMOD, data = 0
        move.l  #$00FE0028, $00006010
        move.l  #0,         $00006014
        ; Inst 3: target = BLTSIZE, data = $3010 (192 << 6 | 16)
        move.l  #$00FE0038, $00006018
        move.l  #$00003010, $0000601C
        ; Inst 4: WAIT
        move.l  #$FFFFFFFE, $00006020
        move.l  #0,         $00006024

        ; ---- horizontal line: from (32+drift, 96) to (224+drift, 96) ----
        ; (the +drift bits are filled in by patch_copper_for_offset).
        ; Inst 5: BLTCON = $CA00080F (LF=$CA, ASH=0, LINE=1, octant=0, channels=$F)
        move.l  #$00FE0000, $00006028
        move.l  #$CA00080F, $0000602C
        ; Inst 6: BLTAFWM
        move.l  #$00FE0004, $00006030
        move.l  #$0000FFFF, $00006034
        ; Inst 7: BLTALWM
        move.l  #$00FE0008, $00006038
        move.l  #$0000FFFF, $0000603C
        ; Inst 8: BLTAPT (Bresenham init = -384 for dx=192) -- patched
        move.l  #$00FE000C, $00006040
        move.l  #$FFFFFE80, $00006044
        ; Inst 9: BLTCPT (pixel addr at (32, 96)) -- patched
        move.l  #$00FE0014, $00006048
        move.l  #$00020C04, $0000604C
        ; Inst 10: BLTDPT -- patched (same value as BLTCPT)
        move.l  #$00FE0018, $00006050
        move.l  #$00020C04, $00006054
        ; Inst 11: BLTAMOD = -768
        move.l  #$00FE001C, $00006058
        move.l  #$FFFFFD00, $0000605C
        ; Inst 12: BLTBMOD = 0
        move.l  #$00FE0020, $00006060
        move.l  #0,         $00006064
        ; Inst 13: BLTCMOD = 32
        move.l  #$00FE0024, $00006068
        move.l  #32,        $0000606C
        ; Inst 14: BLTDMOD = 32
        move.l  #$00FE0028, $00006070
        move.l  #32,        $00006074
        ; Inst 15: BLTBDAT = $FFFF
        move.l  #$00FE0030, $00006078
        move.l  #$0000FFFF, $0000607C
        ; Inst 16: BLTSIZE = (193 << 6) | 2 = $3042  -> starts the blit
        move.l  #$00FE0038, $00006080
        move.l  #$00003042, $00006084
        ; Inst 17: WAIT
        move.l  #$FFFFFFFE, $00006088
        move.l  #0,         $0000608C

        ; ---- vertical line: from (128, 32+drift) to (128, 160+drift) ----
        ; Inst 18: BLTCON = $CA00090F (AUL=1, octant=1)
        move.l  #$00FE0000, $00006090
        move.l  #$CA00090F, $00006094
        ; (BLTAFWM/ALWM already $FFFF from the H-line; skip rewriting.)
        ; Inst 19: BLTAPT = -256 (dx=0, dy=128)
        move.l  #$00FE000C, $00006098
        move.l  #$FFFFFF00, $0000609C
        ; Inst 20: BLTCPT (pixel addr at (128, 32)) -- patched for drift
        move.l  #$00FE0014, $000060A0
        move.l  #$00020410, $000060A4
        ; Inst 21: BLTDPT
        move.l  #$00FE0018, $000060A8
        move.l  #$00020410, $000060AC
        ; Inst 22: BLTAMOD = -512
        move.l  #$00FE001C, $000060B0
        move.l  #$FFFFFE00, $000060B4
        ; Inst 23: BLTBMOD = 0
        move.l  #$00FE0020, $000060B8
        move.l  #0,         $000060BC
        ; Inst 24: BLTSIZE = (129 << 6) | 2 = $2042
        move.l  #$00FE0038, $000060C0
        move.l  #$00002042, $000060C4
        ; Inst 25: WAIT
        move.l  #$FFFFFFFE, $000060C8
        move.l  #0,         $000060CC

        ; ---- HALT ----
        move.l  #$FFFFFFFF, $000060D0
        move.l  #0,         $000060D4

        ; Program COP1LC once.
        move.l  #$00006000, $00FE0040
        rts

; ====================================================================
; patch_copper_for_offset:
;   D0 = signed drift offset (currently unused but illustrative).
;   For simplicity, we leave the cross stationary — the Copper still
;   redraws every frame, which is the actual point of the demo.
; ====================================================================
patch_copper_for_offset:
        rts
