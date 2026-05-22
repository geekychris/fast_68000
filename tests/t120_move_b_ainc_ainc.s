; MOVE.B (A0)+, (A1)+ -- both src and dst use AINC.
;
; Surfaced in Kickstart 1.3's OldOpenLibrary string builder at
; $F81AF0-$F81AF2:
;   MOVE.B  (A0)+, (A1)+    ; copy byte
;   BNE     $F81AF0           ; loop until 0
;
; In our boot the loop writes EVERY byte to the same address (dst A1
; never advances), so the search string ends up with only the LAST
; byte of "utility" then ".library" smashed on top -- breaks the
; FindResident name lookup graphics.library depends on.
;
; Test: copy 8 bytes from $2000 to $3000 via MOVE.B (A0)+, (A1)+,
; verify each destination byte got its source byte (not all the same).
;
; Pass = halt 0.  Failures:
;   $BAD1 = byte at $3000 wrong
;   $BAD2 = byte at $3001 wrong
;   $BAD3 = byte at $3007 wrong
;   $BAD4 = A0 didn't advance to $2008
;   $BAD5 = A1 didn't advance to $3008

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Source: 8 distinct bytes at $2000..$2007.
        move.l  #$11223344, $00002000
        move.l  #$55667788, $00002004

        ; Destination: pre-fill with $00 to catch only-overwrite-position-0.
        move.l  #$00000000, $00003000
        move.l  #$00000000, $00003004

        lea     $00002000, A0
        lea     $00003000, A1

        ; Copy 8 bytes via MOVE.B (A0)+, (A1)+.
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+
        move.b  (A0)+, (A1)+

        ; Verify $3000 = $11.
        move.b  $00003000, D0
        andi.l  #$FF, D0
        cmpi.l  #$11, D0
        bne     fail1

        ; Verify $3001 = $22.
        move.b  $00003001, D0
        andi.l  #$FF, D0
        cmpi.l  #$22, D0
        bne     fail2

        ; Verify $3007 = $88.
        move.b  $00003007, D0
        andi.l  #$FF, D0
        cmpi.l  #$88, D0
        bne     fail3

        ; Verify A0 = $00002008.
        move.l  A0, D0
        cmpi.l  #$00002008, D0
        bne     fail4

        ; Verify A1 = $00003008.
        move.l  A1, D0
        cmpi.l  #$00003008, D0
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
