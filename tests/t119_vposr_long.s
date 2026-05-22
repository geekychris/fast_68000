; MOVE.L $DFF004 must return {VPOSR, VHPOSR} as one longword.
;
; Surfaced in Kickstart 1.3 boot at $F88F7C:
;   MOVE.L  $00DFF004, D0      ; read VPOSR.L
;   ASR.L   #8, D0              ; shift V[7:0] into low byte
;   AND.L   #$7FF, D0           ; keep low 11 bits of V
;   RTS
;
; Callers ($F8C5A0, $F8C5B0) wait for the current raster line to be in
; [20..160] (the visible-display band) before continuing.
;
; Earlier the Agnus response for $DFF004 was {VPOSR_word, 16'd0}, so a
; .L read produced 0 in the low half, ASR.L #8 left D0=0, and the
; BLT-against-20 loop spun forever.
;
; Test approach: spin reading VPOSR.L; the V counter (advanced by Agnus
; every line, ~227 sysclks) MUST eventually cycle through a non-zero
; value in the low word (= VHPOSR contents).  Bail out as soon as we
; see a non-zero low half.  If the bus still ignores the long-read,
; the loop hits its iteration cap and fails.
;
; Pass = halt 0.  Failures:
;   $BAD1 = MOVE.L $DFF004 still returned low word == 0 after the cap
;   $BAD2 = low half changed but ASR/AND path produces a stuck 0
;           (would indicate the bus returned a value but it doesn't
;           carry V[7:0] in the right byte)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. Spin until low half of VPOSR.L is non-zero ---
        move.l  #200000, D2          ; iteration cap
spin1:
        move.l  $00DFF004, D0
        andi.l  #$0000FFFF, D0
        bne     got_low
        subq.l  #1, D2
        bne     spin1
        bra     fail1
got_low:

        ; --- 2. Spin until ASR/AND path produces V in [20..160] ---
        move.l  #200000, D2
spin2:
        move.l  $00DFF004, D0
        asr.l   #8, D0
        andi.l  #$7FF, D0
        cmpi.l  #20, D0
        blt     keep_spin
        cmpi.l  #160, D0
        ble     ok
keep_spin:
        subq.l  #1, D2
        bne     spin2
        bra     fail2

ok:
        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
