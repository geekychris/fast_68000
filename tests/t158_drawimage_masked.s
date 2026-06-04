; Workbench-style DrawImage blit: LF=$CA USE_A+B+C+D.
; LF=$CA = D = if A then B else C — "draw B where A masks 1, else
; preserve C".  This is the canonical Intuition / graphics.library
; selector-form blit used for foreground-with-mask drawing — what
; we observed in WB1.3 traces (bltcon ending in $CAxxxx).
;
; Why this test exists (WB13_DEBUG_JOURNAL §49):
; After §38 fixed USE_A=0 LF=$CA, WB1.3's title bar drew correctly,
; but disk-icon graphics remained as solid white outlines (no
; floppy-disk drawing inside).  Hypothesis: another blit form
; Workbench uses for icon-image drawing has a separate bug.  This
; test exercises the *most common* such form in isolation so we
; can pinpoint blitter vs upstream gaps.
;
; Setup:
;   A source (\$1000) = mask        = $0F0F (low nibble of each byte = 1)
;   B source (\$2000) = foreground  = $5555 (50% horizontal stripe)
;   C/D dest (\$3000) = background  = $AAAA (50% horizontal stripe shifted)
;
; Expected per LF=$E2:
;   D = (A & B) | (~A & C)
;     = ($0F0F & $5555) | ($F0F0 & $AAAA)
;     = $0505 | $A0A0
;     = $A5A5
;
; If our blitter outputs $A5A5, LF=$E2 works correctly with USE_A+B+C+D
; and Workbench's icon-image draw must be failing for a non-blitter
; reason (icon source data not loaded, wrong dest address, etc.).

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Seed source and dest memory --------
        move.w  #$0F0F, $1000.W         ; A source = mask
        move.w  #$5555, $2000.W         ; B source = foreground
        move.w  #$AAAA, $3000.W         ; C/D dest = background

        bsr     wait_blt_idle

        ; -------- Make sure DAT presets don't leak in --------
        move.w  #$DEAD, $74(A0)         ; BLTADAT (pre) — shouldn't matter
        move.w  #$DEAD, $72(A0)         ; BLTBDAT (pre)
        move.w  #$DEAD, $70(A0)         ; BLTCDAT (pre)

        ; -------- Configure the blit --------
        ; BLTCON0:
        ;   ASH=0, USE-A=1, USE-B=1, USE-C=1, USE-D=1, LF=$CA
        ;   = (0 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8) | $CA
        ;   = $0FCA
        move.w  #$0FCA, $40(A0)         ; BLTCON0
        move.w  #$0000, $42(A0)         ; BLTCON1
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        move.w  #$0000, $64(A0)         ; BLTAMOD
        move.w  #$0000, $66(A0)         ; BLTBMOD
        move.w  #$0000, $62(A0)         ; BLTCMOD
        move.w  #$0000, $60(A0)         ; BLTDMOD

        ; BLTAPT = $1000
        move.w  #$0000, $50(A0)
        move.w  #$1000, $52(A0)
        ; BLTBPT = $2000
        move.w  #$0000, $4C(A0)
        move.w  #$2000, $4E(A0)
        ; BLTCPT = $3000
        move.w  #$0000, $48(A0)
        move.w  #$3000, $4A(A0)
        ; BLTDPT = $3000 (same as C — in-place blend)
        move.w  #$0000, $54(A0)
        move.w  #$3000, $56(A0)

        ; BLTSIZE: 1 row × 1 word = $0041
        move.w  #$0041, $58(A0)         ; trigger

        bsr     wait_blt_idle

        ; -------- Verify D == $A5A5 --------
        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        move.w  D2, D3                  ; save for diagnostic
        cmpi.l  #$0000A5A5, D2
        bne     fail_masked

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

fail_masked:
        ; pass actual D as stop code so we can see it in the sim log
        move.w  D3, D0
        stop    #1
