; Canonical Amiga Blitter address test.
;
; Verifies that writes to $DFF040..$DFF066 reach the internal blitter
; engine (not just the chipset shadow) and perform a real blit.
;
; Programs a 4-word A->D copy via the canonical 16-bit register layout:
;   BLTCON0=$09F0  (USEA|USED, LF=$F0=A, ASH=0)
;   BLTCON1=$0000  (BSH=0, no fill, no line, no descend)
;   BLTAFWM=$FFFF  BLTALWM=$FFFF
;   BLTAPTH/L = $0000_4000
;   BLTDPTH/L = $0000_5000
;   BLTAMOD = BLTDMOD = 0
;   BLTSIZE = (1<<6)|4 = $44       ; height=1, width=4 words
;
; Source @ $4000: $1234ABCD $DEADBEEF.
; Dest   @ $5000: should equal source after the blit.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Stage source words.
        move.l #$1234ABCD, $00004000
        move.l #$DEADBEEF, $00004004
        ; Clear dest.
        move.l #0, $00005000
        move.l #0, $00005004

        ; Program blitter through canonical $DFF040+ window.
        move.w #$09F0, $00DFF040          ; BLTCON0: USEA|USED|LF=A
        move.w #$0000, $00DFF042          ; BLTCON1: no flags
        move.w #$FFFF, $00DFF044          ; BLTAFWM
        move.w #$FFFF, $00DFF046          ; BLTALWM
        move.w #$0000, $00DFF050          ; BLTAPTH
        move.w #$4000, $00DFF052          ; BLTAPTL
        move.w #$0000, $00DFF054          ; BLTDPTH
        move.w #$5000, $00DFF056          ; BLTDPTL
        move.w #$0000, $00DFF064          ; BLTAMOD
        move.w #$0000, $00DFF066          ; BLTDMOD
        move.w #$0044, $00DFF058          ; BLTSIZE -- triggers blit

        ; Wait for blit to finish (poll legacy BUSY status -- the
        ; canonical BLTSIZE read at $DFF002 (DMACONR bit 14) would be
        ; the Amiga way, but our legacy BLTSTAT at $FE003C is the same
        ; engine state).
wait:   move.l $00FE003C, D0
        andi.l #1, D0
        bne    wait

        ; Verify destination matches source.
        move.l $00005000, D1
        cmpi.l #$1234ABCD, D1
        bne    fail1
        move.l $00005004, D2
        cmpi.l #$DEADBEEF, D2
        bne    fail2

        ; Confirm the shadow round-trip still works (Kickstart-style
        ; readback of one of the canonical Blitter regs).
        move.l $00DFF040, D3
        andi.l #$FFFF0000, D3
        cmpi.l #$09F00000, D3
        bne    fail3

        stop    #0

fail1:  stop    #$bd71
fail2:  stop    #$bd72
fail3:  stop    #$bd73
