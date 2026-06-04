; Real Amiga BLTCON0 USE-A=0 means A-channel is sourced from BLTADAT
; preset register (default $FFFF, software-settable via $DFF074).
;
; Pre-fix our blitter hardwired A=0 when USE-A=0.  Mirror of the
; pre-fix USE-B=0 bug landed in t155.  For LF=$CA USE-C+D blits
; (which Intuition's title-bar fill emits with bltcon=$CA000003),
; the bug changed the output:
;   Real Amiga (A=$FFFF):  D = (A AND B) OR (NOT_A AND C) = $FFFF
;                          (since A AND B = $FFFF for every bit)
;   Pre-fix us (A=$0000):  D = (0 AND B) OR ($FFFF AND C) = C
;                          (whatever was at BLTCPT)
;
; In the K1.3+WB1.3 boot, Intuition fills the CLI title bar with
; LF=$CA expecting solid white ($FFFF) but our buggy A=0 caused the
; output to fall through to whatever C had at $60C8 from earlier
; (stale) blits — which happened to be $2AAA bits left over from
; MFM-decode buffer reads.  That's why the title bar rendered as
; the inactive-window 50% stipple instead of solid active white.
;
; Found 2026-06-04 via WB13_DEBUG_JOURNAL §38.  See also t155
; for the USE-B=0 equivalent.

        .org $400
start:
        move.l  #$00DFF000, A0          ; chipset base

        ; -------- Seed source and dest memory --------
        ; C source at chip $2000 = $5A5A (so we can detect A=0 fallthrough)
        move.w  #$5A5A, $2000.W

        ; Wait for blitter idle
        bsr     wait_blt_idle

        ; -------- Preset BLTADAT to $FFFF (default-ish but explicit) --------
        move.w  #$FFFF, $74(A0)         ; BLTADAT
        ; BLTBDAT also $FFFF — needed for the LF=$CA "A AND B" branch
        move.w  #$FFFF, $72(A0)         ; BLTBDAT

        ; -------- Configure blit --------
        ; BLTCON0:
        ;   ASH=0, USE-A=0, USE-B=0, USE-C=1, USE-D=1, LF=$CA
        ;   = (0<<12) | (0<<11) | (0<<10) | (1<<9) | (1<<8) | $CA
        ;   = $0000 | $0000 | $0000 | $0200 | $0100 | $CA
        ;   = $03CA
        move.w  #$03CA, $40(A0)         ; BLTCON0
        move.w  #$0000, $42(A0)         ; BLTCON1
        move.w  #$FFFF, $44(A0)         ; BLTAFWM
        move.w  #$FFFF, $46(A0)         ; BLTALWM
        move.w  #$0000, $64(A0)         ; BLTAMOD
        move.w  #$0000, $66(A0)         ; BLTBMOD
        move.w  #$0000, $62(A0)         ; BLTCMOD
        move.w  #$0000, $60(A0)         ; BLTDMOD

        ; BLTCPT = $2000
        move.w  #$0000, $48(A0)         ; BLTCPTH
        move.w  #$2000, $4A(A0)         ; BLTCPTL
        ; BLTDPT = $3000
        move.w  #$0000, $54(A0)         ; BLTDPTH
        move.w  #$3000, $56(A0)         ; BLTDPTL

        ; Pre-fill dest with sentinel so we can detect the write happens
        move.w  #$DEAD, $3000.W

        ; BLTSIZE: 1 row × 1 word = $0041
        move.w  #$0041, $58(A0)         ; trigger blit

        bsr     wait_blt_idle

        ; -------- Verify --------
        ; Real Amiga with A=$FFFF, B=$FFFF: D = $FFFF always
        ; Pre-fix bug with A=$0:           D = C = $5A5A
        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        move.w  D2, D3                  ; save actual for diagnostic
        cmpi.l  #$0000FFFF, D2
        bne     fail_first

        ; -------- Second test: BLTADAT = $5555, BLTBDAT = $FFFF --------
        ; Real Amiga: A=$5555, B=$FFFF, C=$5A5A
        ;   A AND B = $5555 AND $FFFF = $5555
        ;   NOT_A AND C = $AAAA AND $5A5A = $0A0A
        ;   D = $5555 OR $0A0A = $5F5F
        ; Pre-fix bug:
        ;   A=0 forced → D = $0 OR ($FFFF AND $5A5A) = $5A5A
        move.w  #$5555, $74(A0)         ; BLTADAT = $5555
        ; BLTBDAT stays $FFFF
        ; Reset C source (unchanged) + clear D
        move.w  #$DEAD, $3000.W
        ; Re-set BLTCPT/BLTDPT (auto-advanced)
        move.w  #$0000, $48(A0)
        move.w  #$2000, $4A(A0)
        move.w  #$0000, $54(A0)
        move.w  #$3000, $56(A0)
        move.w  #$0041, $58(A0)         ; trigger

        bsr     wait_blt_idle

        move.w  $3000.W, D2
        andi.l  #$0000FFFF, D2
        move.w  D2, D4                  ; save actual for diagnostic
        cmpi.l  #$00005F5F, D2
        bne     fail_second

        stop    #0                       ; PASS

; ----------------------------------------------------------------
wait_blt_idle:
        move.w  $02(A0), D0             ; DMACONR
        btst    #14, D0                 ; BBUSY
        bne     wait_blt_idle
        rts

fail_first:
        ; D3 = actual D output (pre-fix bug: $5A5A; real Amiga: $FFFF)
        move.w  D3, D0
        stop    #1

fail_second:
        move.w  D4, D0
        stop    #2
