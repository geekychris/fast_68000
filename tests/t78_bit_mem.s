; Memory-destination bit ops: BSET / BCLR / BCHG / BTST .B with mem dst.
;
; Verifies the read-modify-write path: byte at EA is loaded, bit n
; (mod 8) is set/cleared/toggled/tested, CCR Z reflects the PRE-modify
; value of that bit, and (for BSET/BCLR/BCHG) the modified byte is
; written back.  Covers static and dynamic forms across abs.l, (An),
; d(An) addressing.
;
; Note: comparisons go via a Dn snapshot because CMPI to memory is
; currently a no-op in this core (tracked separately).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- 1: BSET static, abs.l, bit was 0 -> Z=1 ----
        move.l  #$00000000, $00010000
        bset    #2, $00010003           ; flips bit 2 of LS byte
        bne     fail_1                  ; Z should be 1 (pre-bit=0)
        move.l  $00010000, D0
        cmpi.l  #$00000004, D0
        bne     fail_1

        ; ---- 2: BSET same bit again: was 1 -> Z=0, value unchanged ----
        bset    #2, $00010003
        beq     fail_2                  ; Z=0
        move.l  $00010000, D0
        cmpi.l  #$00000004, D0
        bne     fail_2

        ; ---- 3: BCLR static, abs.l ----
        move.l  #$000000FF, $00010000
        bclr    #3, $00010003           ; clear bit 3 of LS byte
        beq     fail_3                  ; Z=0 (bit was 1)
        move.l  $00010000, D0
        cmpi.l  #$000000F7, D0
        bne     fail_3
        bclr    #3, $00010003           ; already clear
        bne     fail_3                  ; Z=1
        move.l  $00010000, D0
        cmpi.l  #$000000F7, D0
        bne     fail_3

        ; ---- 4: BCHG static, abs.l ----
        move.l  #$00000080, $00010000
        bchg    #7, $00010003
        beq     fail_4                  ; Z=0 (was 1)
        move.l  $00010000, D0
        cmpi.l  #$00000000, D0
        bne     fail_4
        bchg    #7, $00010003
        bne     fail_4                  ; Z=1 (was 0)
        move.l  $00010000, D0
        cmpi.l  #$00000080, D0
        bne     fail_4

        ; ---- 5: BTST static, abs.l, no write ----
        move.l  #$00000040, $00010000
        btst    #6, $00010003
        beq     fail_5                  ; Z=0
        move.l  $00010000, D0
        cmpi.l  #$00000040, D0
        bne     fail_5
        btst    #5, $00010003
        bne     fail_5                  ; Z=1
        move.l  $00010000, D0
        cmpi.l  #$00000040, D0
        bne     fail_5

        ; ---- 6: BSET (An) ----
        move.l  #$00000000, $00010004
        move.l  #$00010007, A0
        bset    #1, (A0)
        bne     fail_6
        move.l  $00010004, D0
        cmpi.l  #$00000002, D0
        bne     fail_6

        ; ---- 7: BSET d(An) ----
        move.l  #$00000000, $00010008
        move.l  #$00010000, A1
        bset    #4, 11(A1)
        bne     fail_7
        move.l  $00010008, D0
        cmpi.l  #$00000010, D0
        bne     fail_7

        ; ---- 8: BSET dynamic (bit count in Dn), abs.l ----
        move.l  #$00000000, $0001000C
        moveq   #6, D2
        bset    D2, $0001000F
        bne     fail_8
        move.l  $0001000C, D0
        cmpi.l  #$00000040, D0
        bne     fail_8

        ; ---- 9: Static bit# mod 8 (#10 -> bit 2) ----
        move.l  #$00000000, $00010010
        bset    #10, $00010013
        bne     fail_9
        move.l  $00010010, D0
        cmpi.l  #$00000004, D0
        bne     fail_9

        ; ---- 10: Dynamic bit# mod 8 (D=17 -> bit 1) ----
        move.l  #$00000000, $00010014
        moveq   #17, D3
        bset    D3, $00010017
        bne     fail_10
        move.l  $00010014, D0
        cmpi.l  #$00000002, D0
        bne     fail_10

        ; ---- 11: MS byte and adjacent byte ----
        move.l  #$00000000, $00010018
        bset    #0, $00010018          ; MS byte
        bne     fail_11
        bset    #0, $00010019          ; next byte
        bne     fail_11
        move.l  $00010018, D0
        cmpi.l  #$01010000, D0
        bne     fail_11

        ; ---- 12: BCLR dynamic ----
        move.l  #$000000FF, $0001001C
        moveq   #3, D4
        bclr    D4, $0001001F
        beq     fail_12                ; Z=0
        move.l  $0001001C, D0
        cmpi.l  #$000000F7, D0
        bne     fail_12

        stop    #0

fail_1:  stop #$B701
fail_2:  stop #$B702
fail_3:  stop #$B703
fail_4:  stop #$B704
fail_5:  stop #$B705
fail_6:  stop #$B706
fail_7:  stop #$B707
fail_8:  stop #$B708
fail_9:  stop #$B709
fail_10: stop #$B70A
fail_11: stop #$B70B
fail_12: stop #$B70C
