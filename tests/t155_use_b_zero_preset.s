; Real Amiga BLTCON0 USE-B=0 means B-channel is sourced from BLTBDAT
; preset register (default $FFFF, software-settable via $DFF072).
;
; Pre-fix our blitter hardwired B=0 when USE-B=0.  For LF=$2A USE-A+C+D
; blits (which K1.3 graphics.library text drawing emits) this changed
; the output formula:
;   Real Amiga:  output = (~A) & C
;   Pre-fix us:  output = C  (because B=0 changes which minterms contribute)
;
; This test verifies real-Amiga semantics: set BLTBDAT preset to $FFFF,
; USE-B=0, LF=$2A, USE-A+C+D, with known A and C source data; check
; output matches (~A) & C.
;
; Found 2026-06-02 via WB1.3 boot debugging at chip $2D24 corruption.
; See docs/WB13_DEBUG_JOURNAL.md §22.

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Seed source and dest memory --------
        ; A source at chip $1000 = $5555 (all 0101 pattern)
        move.w  #$5555, $1000.W
        ; C/D source at chip $2000 = $AAFF (mix pattern)
        move.w  #$AAFF, $2000.W

        ; Wait for blitter idle
        bsr     wait_blt_idle

        ; -------- Preset BLTBDAT to $FFFF (default but explicit) --------
        move.w  #$FFFF, $72(A0)         ; BLTBDAT

        ; -------- Configure the blit --------
        ; BLTCON0:
        ;   ASH=0 (no shift), USE-A=1, USE-B=0, USE-C=1, USE-D=1, LF=$2A
        ;   = (0 << 12) | (1 << 11) | (0 << 10) | (1 << 9) | (1 << 8) | $2A
        ;   = $0B2A
        move.w  #$0B2A, $40(A0)         ; BLTCON0
        move.w  #$0000, $42(A0)         ; BLTCON1 (no LINE, no DESC)
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        move.w  #$0000, $64(A0)         ; BLTAMOD
        move.w  #$0000, $60(A0)         ; BLTCMOD
        move.w  #$0000, $66(A0)         ; BLTDMOD

        ; BLTAPT = $1000
        move.w  #$0000, $50(A0)         ; BLTAPTH
        move.w  #$1000, $52(A0)         ; BLTAPTL
        ; BLTCPT = $2000
        move.w  #$0000, $48(A0)         ; BLTCPTH
        move.w  #$2000, $4A(A0)         ; BLTCPTL
        ; BLTDPT = $3000 (clean dest area)
        move.w  #$0000, $54(A0)         ; BLTDPTH
        move.w  #$3000, $56(A0)         ; BLTDPTL

        ; Pre-clear the destination so we can read it back
        move.w  #$DEAD, $3000.W

        ; BLTSIZE: 1 row × 1 word = $0041
        move.w  #$0041, $58(A0)         ; BLTSIZE -- starts the blit

        ; Wait for blit done
        bsr     wait_blt_idle

        ; -------- Verify output --------
        ; Expected: (~A) & C = (~$5555) & $AAFF = $AAAA & $AAFF = $AAAA
        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        ; Encode actual value into stop code for diagnostics
        move.w  D2, D3
        ; Real Amiga: B = BLTBDAT preset = $FFFF; output = (~A) & C = $AAAA
        cmpi.l  #$0000AAAA, D2
        bne     fail_output_actual

        ; -------- Verify BLTBDAT preset is independent of USE-B=0 --------
        ; Change BLTBDAT to $0F0F and re-run with same A/C
        move.w  #$0F0F, $72(A0)         ; BLTBDAT = $0F0F
        move.w  #$DEAD, $3000.W         ; reset dest
        ; Re-set BLTAPT (auto-advanced after prior blit)
        move.w  #$0000, $50(A0)
        move.w  #$1000, $52(A0)
        move.w  #$0000, $48(A0)
        move.w  #$2000, $4A(A0)
        move.w  #$0000, $54(A0)
        move.w  #$3000, $56(A0)
        move.w  #$0041, $58(A0)         ; trigger
        bsr     wait_blt_idle

        ; With B=$0F0F: LF=$2A picks minterms by ABC.  $5555&$0F0F=$0505
        ; For each bit, output = m1 + m3 + m5 contributions based on A,B,C.
        ; This is complex per-bit; expected pre-computed = $A0AF.
        ;   bit pos 15: A=0 B=0 C=1 → m1=1, output bit = 1
        ;   bit pos 14: A=1 B=0 C=0 → m4=0, output bit = 0
        ;   bit pos 13: A=0 B=0 C=1 → m1=1, output bit = 1
        ;   bit pos 12: A=1 B=0 C=0 → m4=0, output bit = 0
        ;   bit pos 11: A=0 B=1 C=1 → m3=1, output bit = 1
        ;   bit pos 10: A=1 B=1 C=0 → m6=0, output bit = 0
        ;   bit pos  9: A=0 B=1 C=1 → m3=1, output bit = 1
        ;   bit pos  8: A=1 B=1 C=0 → m6=0, output bit = 0
        ; high byte = 10101010 = $AA
        ;   bit pos  7: A=0 B=0 C=1 → m1=1, output bit = 1
        ;   bit pos  6: A=1 B=0 C=1 → m5=1, output bit = 1
        ;   bit pos  5: A=0 B=0 C=1 → m1=1, output bit = 1
        ;   bit pos  4: A=1 B=0 C=1 → m5=1, output bit = 1
        ;   bit pos  3: A=0 B=1 C=1 → m3=1, output bit = 1
        ;   bit pos  2: A=1 B=1 C=1 → m7=0, output bit = 0
        ;   bit pos  1: A=0 B=1 C=1 → m3=1, output bit = 1
        ;   bit pos  0: A=1 B=1 C=1 → m7=0, output bit = 0
        ; low byte = 11111010 = $FA
        ; expected = $AAFA
        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000AAFA, D2
        bne     fail_preset2

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

fail_output_actual:
        ; pass the actual blit output as the stop code so we can see it
        ; in the simulator log (lower 16 bits = actual blit result)
        move.w  D3, D0
        stop    #1
fail_output:    stop   #3
fail_preset2:   stop   #2
