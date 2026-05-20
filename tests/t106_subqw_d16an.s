; SUBQ.W #1, d16(An) -- the form Kickstart 1.3's render-init outer
; loop uses to decrement its counter ($F86F92 = SUBQ.W #1, $8(A7)).
;
; If SUBQ.W with d16(An) destination doesn't actually commit the
; decrement, the loop counter stays put, the BNE.W after never sees
; zero, and the boot spins forever inside the render init.
;
; Tests both .W placements: word at a long-aligned displacement
; (high half of the long) and word at a word-aligned-but-not-long-
; aligned displacement (low half of the long).
;
; Pass = halt 0.  Failures: $BAD1 (high-half counter wrong),
; $BAD2 (low-half counter wrong), $BAD3 (sentinel below clobbered),
; $BAD4 (sentinel above clobbered).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        lea     $00002000, A0
        ; Set sentinels.
        move.l  #$AABBCCDD, $00001FFC
        move.l  #$DEADBEEF, $00002010

        ; --- High-half .W counter at A0+8 (= $2008) ---
        ; Word at $2008..$2009 = high half of long at $2008.
        ; Put $0080 there + leave $200A..$200B as a 'magic' sentinel.
        move.w  #$0080, $00002008    ; counter = 128
        move.w  #$F00F, $0000200A    ; magic, must stay intact

        ; Decrement 5 times.
        subq.w  #1, $8(A0)
        subq.w  #1, $8(A0)
        subq.w  #1, $8(A0)
        subq.w  #1, $8(A0)
        subq.w  #1, $8(A0)

        move.w  $00002008, D1
        andi.l  #$FFFF, D1
        cmpi.l  #$007B, D1               ; 128 - 5 = 123 = $7B
        bne     fail_high
        move.w  $0000200A, D2
        andi.l  #$FFFF, D2
        cmpi.l  #$F00F, D2               ; magic must survive
        bne     fail_high                ; if magic dies, .W write was wide

        ; --- Low-half .W counter at A0+10 (= $200A) ---
        ; Word at $200A..$200B = low half of long at $2008.
        move.w  #$0080, $0000200A        ; counter = 128
        move.w  #$BEEF, $00002008        ; magic, must stay intact

        subq.w  #1, $A(A0)
        subq.w  #1, $A(A0)
        subq.w  #1, $A(A0)
        subq.w  #1, $A(A0)
        subq.w  #1, $A(A0)

        move.w  $0000200A, D3
        andi.l  #$FFFF, D3
        cmpi.l  #$007B, D3
        bne     fail_low
        move.w  $00002008, D4
        andi.l  #$FFFF, D4
        cmpi.l  #$BEEF, D4
        bne     fail_low

        ; Sentinels above + below must be intact.
        move.l  $00001FFC, D5
        cmpi.l  #$AABBCCDD, D5
        bne     fail_below
        move.l  $00002010, D6
        cmpi.l  #$DEADBEEF, D6
        bne     fail_above

        stop    #0
forever:
        bra     forever
fail_high:  stop #$BAD1
fail_low:   stop #$BAD2
fail_below: stop #$BAD3
fail_above: stop #$BAD4
