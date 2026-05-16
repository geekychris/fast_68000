; Denise indexed-color rasterizer test.
;
; Part 1: nplanes=0 fills the entire chunky FB with COLOR00 (red).
; Part 2: nplanes=1 with a $80000001 pattern at plane 0 row 0:
;         pixel 0 -> COLOR01 (red), pixels 1..30 -> COLOR00 (blue),
;         pixel 31 -> COLOR01 (red), pixels 32+ -> COLOR00 (blue).
;
; All assertions probe individual bytes in the chunky framebuffer at $10000.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ====================================================
        ; Part 1: blank screen test (nplanes=0)
        ; ====================================================
        ; Pre-fill chunky FB with $5A to detect "Denise didn't run".
        move.l #$00010000, A0
        move.l #12288, D1           ; 49152/4 longs
fill0:  move.l #$5A5A5A5A, (A0)+
        subq.l #1, D1
        bne    fill0

        ; BPLCON0 = 0 (nplanes=0, no HAM, no DPF)
        move.l #0,         $00FE0100
        move.l #0,         $00FE0104    ; BPLCON1
        move.l #0,         $00FE0108    ; BPLCON2

        ; COLOR00 = $F00 (red)
        move.l #$00000F00, $00FE0180

        ; Trigger Denise.
        move.l #1, $00FE0140

w1:     move.l $00FE0144, D2
        andi.l #1, D2
        bne    w1

        ; Verify a few bytes are $E0 (red in RGB332).
        ; (FB[0] = pixel 0,0)
        move.l #0, D3
        move.b $00010000, D3
        andi.l #$FF, D3
        cmpi.l #$E0, D3
        bne    fail1a
        ; FB[last byte of row 0] = pixel 255,0
        move.l #0, D3
        move.b $000100FF, D3
        andi.l #$FF, D3
        cmpi.l #$E0, D3
        bne    fail1b
        ; FB[last long] = bottom-right corner area
        move.l $0001BFFC, D4
        cmpi.l #$E0E0E0E0, D4
        bne    fail1c

        ; ====================================================
        ; Part 2: single-plane indexed
        ; ====================================================
        ; Re-fill chunky FB with $5A to detect "Denise didn't run again".
        move.l #$00010000, A0
        move.l #12288, D1
fill1:  move.l #$5A5A5A5A, (A0)+
        subq.l #1, D1
        bne    fill1

        ; Zero plane 0 (need 6144 bytes at $5000..$67FF).
        move.l #$00005000, A0
        move.l #1536, D1            ; 6144/4 longs
zero0:  move.l #0, (A0)+
        subq.l #1, D1
        bne    zero0

        ; Write row 0 word 0 = $8000 (pixel 0 set), word 1 = $0001 (pixel 31 set).
        move.l #$80000001, $00005000

        ; BPLCON0 = nplanes=1 -> bits [14:12] = 001 -> $1000
        move.l #$00001000, $00FE0100

        ; BPL1PT = $5000
        move.l #$00005000, $00FE0110

        ; BPL1MOD = 0
        move.l #0, $00FE0128

        ; COLOR00 = $00F (blue), COLOR01 = $F00 (red)
        move.l #$0000000F, $00FE0180
        move.l #$00000F00, $00FE0184

        ; Trigger
        move.l #1, $00FE0140

w2:     move.l $00FE0144, D2
        andi.l #1, D2
        bne    w2

        ; Verify pixel (0,0) = red (high byte of $10000 = $E0).
        move.l #0, D3
        move.b $00010000, D3
        andi.l #$FF, D3
        cmpi.l #$E0, D3
        bne    fail2a

        ; Verify pixel (1,0) = blue ($03).
        move.l #0, D3
        move.b $00010001, D3
        andi.l #$FF, D3
        cmpi.l #$03, D3
        bne    fail2b

        ; Verify pixel (31,0) = red.
        move.l #0, D3
        move.b $0001001F, D3
        andi.l #$FF, D3
        cmpi.l #$E0, D3
        bne    fail2c

        ; Verify pixel (32,0) = blue.
        move.l #0, D3
        move.b $00010020, D3
        andi.l #$FF, D3
        cmpi.l #$03, D3
        bne    fail2d

        ; Verify pixel (0,1) = blue (row 1 is all zeros in plane 0).
        move.l #0, D3
        move.b $00010100, D3
        andi.l #$FF, D3
        cmpi.l #$03, D3
        bne    fail2e

        stop   #0

fail1a: stop #$de11
fail1b: stop #$de12
fail1c: stop #$de13
fail2a: stop #$de21
fail2b: stop #$de22
fail2c: stop #$de23
fail2d: stop #$de24
fail2e: stop #$de25
