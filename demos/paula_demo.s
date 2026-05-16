; Paula demo: play a 441 Hz square wave on channel 0.
;
; Sample buffer at $5000: 100 8-bit signed samples (50 positive then
; 50 negative).  AUD0LEN = 50 words = 100 bytes.  AUD0PER = 100 sim
; cycles per sample.  With the harness sampling Paula at the same
; rate (CYCLES_PER_AUDIO = 100) we get 44.1 kHz sample output, so
; the 100-sample cycle period gives 441 Hz.

        .org $400

start:
        ; Build the sample buffer at $00005000 (25 longs = 100 bytes).
        move.l  #$00005000, A0
        moveq   #11, D0                ; 12 positive longs
pos:    move.l  #$7F7F7F7F, (A0)+
        dbra    D0, pos
        move.l  #$7F7F8080, (A0)+      ; transition long
        moveq   #11, D0                ; 12 negative longs
neg:    move.l  #$80808080, (A0)+
        dbra    D0, neg

        ; Program Paula channel 0.
        move.l  #$00005000, $00FE0210  ; AUD0LC
        move.l  #50,         $00FE0214 ; AUD0LEN (words)
        move.l  #100,        $00FE0218 ; AUD0PER (sim clocks/sample)
        move.l  #50,         $00FE021C ; AUD0VOL (out of 64)

        ; Enable channel 0.
        move.l  #$00000001, $00FE0200  ; AUDENA bit 0

forever:
        bra     forever
