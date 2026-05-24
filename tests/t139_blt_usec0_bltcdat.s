; t139 — Blitter: USEC=0 must use BLTCDAT as the constant C input, not 0.
;
; Real Amiga / minimig: when USEC=0 the C-channel DMA fetch is skipped,
; but BLTCDAT is still routed into the minterm generator as a constant.
; Our blitter was forcing C=0 when USEC=0, which silently corrupts any
; blit whose LF actually consults C — most notably trackdisk's MFM
; data-decode blit (LF=$D8 with BLTCDAT=$5555 as the bit-interleave mask).
;
; K1.3 strap.task post-DoIO check fails ($000A0200 at $55D8 instead of
; 'DOS\0'=$444F5300) because the decoded sector-0 data is the wrong
; mux of (A, B): we always picked A and ignored B entirely.
;
; Test: program a tiny blit with LF=$D8 (the classic mux pattern
; D = (C AND B) OR (NOT C AND A), i.e. C=1 → B, C=0 → A), USEC=0,
; BLTCDAT=$F0F0 preloaded.  A=$AAAA, B=$5555.  Expected D = $5A5A:
;   bits where BLTCDAT=1 (high nibble of each byte) -> from B = $5555 -> $5
;   bits where BLTCDAT=0 (low nibble of each byte)  -> from A = $AAAA -> $A
;   per byte: high nibble $5, low nibble $A -> $5A.  Two bytes -> $5A5A.
;
; With the bug (C forced to 0): D = A = $AAAA.
;
; Pass = halt 0.  Fail $BAD1 = wrong word at $9000.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Seed source words: A at $8000, B at $8100.
        move.w  #$AAAA, $00008000
        move.w  #$5555, $00008100

        ; Pre-clear destination so we can detect any spurious write.
        move.w  #$0000, $00009000

        ; Wait for any prior blit to finish (BBUSY).
wait1:  btst    #6, $00DFF002
        bne     wait1

        ; Program canonical Amiga blitter registers.
        ; BLTCON0 = $0DD8: bits 15-12=ASH=$0, bits 11-8=USE=$D (USEA=1,
        ; USEB=1, USEC=0, USED=1), bits 7-0=LF=$D8.
        ;   LF $D8 = 1101_1000.  Index = {A,B,C} (A MSB):
        ;     C=0 bits (idx 0,2,4,6) -> D8>>{0,2,4,6} = 0,0,1,1 = A
        ;     C=1 bits (idx 1,3,5,7) -> D8>>{1,3,5,7} = 0,1,0,1 = B
        ;   So D[i] = C[i] ? B[i] : A[i] — the canonical 3-input mux.
        ; BLTCON1 = $0000: no DESC, no LINE
        move.l  #$0DD80000, $00DFF040    ; BLTCON0/1
        move.l  #$FFFFFFFF, $00DFF044    ; BLTAFWM/BLTALWM (no edge mask)

        ; BLTCDAT = $F0F0 (the mux selector).
        move.w  #$F0F0, $00DFF070

        ; Pointers: A at $8000, B at $8100, D at $9000.
        move.l  #$00008000, $00DFF050    ; BLTAPTH/L
        move.l  #$00008100, $00DFF04C    ; BLTBPTH/L
        move.l  #$00009000, $00DFF054    ; BLTDPTH/L

        ; Modulos = 0 (single-word blit).
        move.w  #$0000, $00DFF060        ; BLTAMOD
        move.w  #$0000, $00DFF062        ; BLTBMOD
        move.w  #$0000, $00DFF066        ; BLTDMOD

        ; Kick: BLTSIZE = height=1, width=1 -> $0041.
        move.w  #$0041, $00DFF058

        ; Wait for blit to finish.
wait2:  btst    #6, $00DFF002
        bne     wait2

        ; Verify D at $9000.
        move.w  $00009000, D0
        cmpi.w  #$5A5A, D0
        bne     fail_d

        stop    #0

fail_d:
        ; Halt with the actual value in the failure code so the run.log
        ; preserves the diagnostic.  D0 already holds the wrong word.
        ; To distinguish bug-state ($AAAA from C=0) vs other failures,
        ; halt with the actual value.
        move.w  D0, D1
        ; If D0 == $AAAA, the bug is present (C forced to 0, D=A).
        cmpi.w  #$AAAA, D0
        beq     fail_aaaa
        ; If D0 == $5555, opposite bug (D=B).
        cmpi.w  #$5555, D0
        beq     fail_5555
        stop    #$BAD1
fail_aaaa:
        stop    #$BAD2
fail_5555:
        stop    #$BAD3
