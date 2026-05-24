; t112 — DMACONR returns live BBUSY (bit 14), not the CPU-written BLTPRI.
;
; Real Amiga semantics: DMACON write at $DFF096 latches the CPU's DMA-
; enable bits (including bit 14 = BLTPRI for blitter priority).  But
; DMACONR read at $DFF002 returns DIFFERENT semantics in bit 14: it
; reflects the LIVE blitter busy flag (BBUSY), not BLTPRI.
;
; K1.3 trackdisk's WAITBLIT polls DMACONR bit 14 expecting BBUSY.  If
; we returned BLTPRI instead, the CPU thinks the blitter is idle and
; writes BLTDPT for the next blit mid-PREVIOUS-blit -- corrupting the
; running blit's output.
;
; Test:
;   1. Verify DMACONR bit 14 = 0 when blitter is idle.
;   2. Start a blit (write BLTSIZE).
;   3. Verify DMACONR bit 14 = 1 (BBUSY) while the blit is running.
;   4. Wait for the blit to finish (poll BBUSY → 0).
;   5. Verify DMACONR bit 14 = 0 after the blit completes.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Step 1: idle DMACONR.bit14 = 0.
        move.w  $00DFF002, D0
        andi.w  #$4000, D0
        bne     fail_idle

        ; Step 2: set up a real (non-zero-size) blit.  Use a simple
        ; B->D copy of 4 words from $9000 to $9100.
        ; Seed source.
        move.l  #$11111111, $00009000
        move.l  #$22222222, $00009004

        ; Program blitter via canonical Amiga addresses.
        ; BLTCON0 = $05CC: USEB+USED, LF=$CC (D=B), ASH=0
        ; BLTCON1 = $0000: no DESC, no LINE, BSH=0
        move.l  #$05CC0000, $00DFF040    ; BLTCON0/1
        move.l  #$FFFFFFFF, $00DFF044    ; BLTAFWM/BLTALWM (no mask)
        move.l  #$00009000, $00DFF04C    ; BLTBPTH/L
        move.l  #$00009100, $00DFF054    ; BLTDPTH/L
        move.w  #$0000, $00DFF062         ; BLTBMOD
        move.w  #$0000, $00DFF066         ; BLTDMOD

        ; Kick off the blit: BLTSIZE = height=1, width=4 (= 1 row x 4 words)
        move.w  #$0044, $00DFF058         ; BLTSIZE = $0044

        ; Step 3: immediately read DMACONR -- BBUSY must be 1.
        move.w  $00DFF002, D1
        andi.w  #$4000, D1
        beq     fail_busy_not_set

        ; Step 4: wait for blit to finish (poll BBUSY).
wait_blit:
        btst    #6, $00DFF002             ; bit 14 of D8 == bit 6 of high byte
        bne     wait_blit

        ; Step 5: DMACONR.bit14 = 0 after blit.
        move.w  $00DFF002, D2
        andi.w  #$4000, D2
        bne     fail_busy_not_cleared

        ; All pass.
        stop    #0
forever_pass:
        bra     forever_pass

fail_idle:
        stop    #$BAD1
fail_busy_not_set:
        stop    #$BAD2
fail_busy_not_cleared:
        stop    #$BAD3
