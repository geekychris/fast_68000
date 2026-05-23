; Blitter K1.3-style "shift B into D" smoke test.
;
; K1.3 trackdisk's in-place MFM decode programs:
;   BLTCON0 = $05CC = USEB|USED, LF=$CC, ASH=0
;     -> LF=$CC means "output = B" regardless of A,C
;     -> bit pattern (USEA=0, USEB=1, USEC=0, USED=1)
;   BLTCON1 = $0005 = (LINE=1, FCI=1, BSH=0)  -- area-mode flags overlap
;     ...for AREA mode LINE bit is 0; K1.3's $0005 actually sets some
;     of the line-mode-only bits; we treat the run as a normal area blit
;     since USEA=0 forces non-line operation in real Amiga
;   BLTBPT = src
;   BLTDPT = src - 6 (shift backwards by 6 bytes, in-place)
;   BLTSIZE = 1 row * 14 words = $004E
;
; Expected behaviour: bytes [src..src+27] copy verbatim to
; [src-6..src+21] (overlapping copy, processed in ascending order
; since DESC=0).  After the blit, the destination range holds the
; source's original values; the overlapping region [src..src+21] gets
; clobbered with shifted data.
;
; This test stages a known source pattern and verifies the destination
; matches a hand-computed expected pattern.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Stage source pattern at $00006000 + 6 = $00006006 (so dst
        ; starts at $00006000).  14 words = 28 bytes.
        move.l  #$11112222, $00006006
        move.l  #$33334444, $0000600A
        move.l  #$55556666, $0000600E
        move.l  #$77778888, $00006012
        move.l  #$9999AAAA, $00006016
        move.l  #$BBBBCCCC, $0000601A
        move.l  #$DDDDEEEE, $0000601E

        ; Pre-fill dest region with $A5A5 so we can detect leaks.
        move.l  #$A5A5A5A5, $00006000

        ; Program blitter via canonical $DFF040+ window.
        move.w  #$05CC, $00DFF040          ; BLTCON0: USEB|USED|LF=$CC|ASH=0
        move.w  #$0000, $00DFF042          ; BLTCON1: area mode, BSH=0
        move.w  #$FFFF, $00DFF044          ; BLTAFWM (ignored, USEA=0)
        move.w  #$FFFF, $00DFF046          ; BLTALWM
        move.w  #$0000, $00DFF04C          ; BLTBPTH
        move.w  #$6006, $00DFF04E          ; BLTBPTL = src
        move.w  #$0000, $00DFF054          ; BLTDPTH
        move.w  #$6000, $00DFF056          ; BLTDPTL = src-6
        move.w  #$0000, $00DFF062          ; BLTBMOD
        move.w  #$0000, $00DFF066          ; BLTDMOD
        move.w  #$004E, $00DFF058          ; BLTSIZE = 1 row * 14 words

        ; Wait for completion.
wait:   move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     wait

        ; Blit semantics with BSH=0, LF=$CC ("output = B"), no shift:
        ; the barrel shifter joins {prev, cur} and outputs prev's
        ; contribution at the chosen shift offset.  At shift=0 the
        ; output is just B's current word (no leakage from prev).
        ;
        ; So the 14 dst words at $6000..$601B should equal the 14 src
        ; words at $6006..$6021 verbatim.  We staged 7 longwords
        ; ($11112222 .. $DDDDEEEE) = 28 bytes = 14 words at $6006.
        ;
        ; Verify exact expected pattern at the dst:
        move.l  $00006000, D1
        cmpi.l  #$11112222, D1
        bne     fail_w01
        move.l  $00006004, D1
        cmpi.l  #$33334444, D1
        bne     fail_w23
        move.l  $00006008, D1
        cmpi.l  #$55556666, D1
        bne     fail_w45
        move.l  $0000600C, D1
        cmpi.l  #$77778888, D1
        bne     fail_w67
        move.l  $00006010, D1
        cmpi.l  #$9999AAAA, D1
        bne     fail_w89
        move.l  $00006014, D1
        cmpi.l  #$BBBBCCCC, D1
        bne     fail_wab
        move.l  $00006018, D1
        cmpi.l  #$DDDDEEEE, D1
        bne     fail_wcd

        stop    #0

fail_w01: stop #$bd80
fail_w23: stop #$bd81
fail_w45: stop #$bd82
fail_w67: stop #$bd83
fail_w89: stop #$bd84
fail_wab: stop #$bd85
fail_wcd: stop #$bd86
