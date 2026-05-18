; Blitter BZERO bit (BLTSTAT bit 1).
; - Blit that writes all-zero D output should leave BZERO=1.
; - Blit that writes any non-zero D should clear BZERO=0.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ----- Test 1: all-zero source -> BZERO should stay 1 -----
        ; Fill source $00060000..$00060007 with 0.
        move.l  #0, $00060000
        move.l  #0, $00060004
        ; Destination $00060100 dirty.
        move.l  #$DEADBEEF, $00060100
        move.l  #$DEADBEEF, $00060104

        ; BLTCON: copy A to D, no shifts, USEA|USED, LF = $F0 (D = A).
        move.l  #$F0000009, $00FE0000
        move.l  #$0000FFFF, $00FE0004
        move.l  #$0000FFFF, $00FE0008
        move.l  #$00060000, $00FE000C   ; APT
        move.l  #$00060100, $00FE0018   ; DPT
        move.l  #0,         $00FE001C   ; AMOD
        move.l  #0,         $00FE0028   ; DMOD
        move.l  #$00000042, $00FE0038   ; (1<<6)|2 -> 1 row, 2 words
w1:     move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     w1
        ; BZERO = BLTSTAT bit 1
        move.l  $00FE003C, D0
        andi.l  #2, D0
        beq     fail_z1                 ; expected BZERO=1

        ; ----- Test 2: non-zero source -> BZERO should go 0 -----
        move.l  #$DEADBEEF, $00060000
        move.l  #$F0000009, $00FE0000
        move.l  #$00060000, $00FE000C
        move.l  #$00060100, $00FE0018
        move.l  #$00000042, $00FE0038
w2:     move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     w2
        move.l  $00FE003C, D0
        andi.l  #2, D0
        bne     fail_nz                 ; expected BZERO=0

        stop    #0

fail_z1: stop #$BB1A
fail_nz: stop #$BB1B
