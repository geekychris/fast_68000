; Blitter fill-mode test.
;
; Input pattern in plane A (1 word):  $8001  (bit 15 and bit 0 set)
;
; Inclusive fill (IFE): every bit between (and including) two set bits
; in scan order is set.  $8001 -> $FFFF.
;
; Exclusive fill (EFE): every bit strictly between two set bits is set
; (endpoints clear).
;
; Ascending mode (DESC=0) scans bit 15 first within each word so the
; carry propagates left-to-right across multi-word rows.  For $8001:
;   bit 15: in=1, out=fc(0)=0, fc->1
;   bits 14..1: in=0, out=fc(1)=1
;   bit 0: in=1, out=fc(1)=1, fc->0
; EFE result: bit 15 clear, bits 14..0 set = $7FFF.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Source A at $2000 = single word $8001 in high half of long.
        move.l #$80010000, $00002000

        ; ===== Part 1: IFE (inclusive fill) =====
        ;   BLTCON: LF=$F0 (A), USEA|USED, IFE=1
        ;   bits: [31:24]=F0, [23:20]=0 (ASH), [14]=1 IFE, [3:0]=9
        ;   = $F0_00_40_09
        move.l #$F0004009, $00FE0000
        move.l #$0000FFFF, $00FE0004
        move.l #$0000FFFF, $00FE0008
        move.l #$00002000, $00FE000C
        move.l #$00003000, $00FE0018
        move.l #0,         $00FE001C
        move.l #0,         $00FE0028
        ; height=1, width=1 -> (1 << 6) | 1 = $41
        move.l #$00000041, $00FE0038

w1:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w1

        ; Dest high half should be $FFFF, low half unchanged ($0000).
        move.l $00003000, D1
        cmpi.l #$FFFF0000, D1
        bne    fail1

        ; ===== Part 2: EFE (exclusive fill) =====
        ;   BLTCON: LF=$F0, USEA|USED, EFE=1
        ;   = $F0_00_20_09
        move.l #0,         $00003000
        move.l #$F0002009, $00FE0000
        move.l #$00002000, $00FE000C
        move.l #$00003000, $00FE0018
        move.l #$00000041, $00FE0038

w2:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w2

        ; EFE expected = $7FFF (see header comment).
        move.l $00003000, D2
        cmpi.l #$7FFF0000, D2
        bne    fail2

        stop   #0

fail1:  stop #$bd61
fail2:  stop #$bd62
