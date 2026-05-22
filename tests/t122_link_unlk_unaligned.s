; LINK An / UNLK An at a frame-pointer address with addr[1:0] = 2'b10.
;
; On a 68000 the LINK long-write and UNLK long-read must use the
; bus's unaligned-.L assembly path (low half of mem[idx], high half of
; mem[idx+1]).  Our decoder used to leave LINK/UNLK at the default
; SZ_W, so the read path returned only mem[idx] and zero-extended the
; rest -- UNLK restored An = 0 instead of the value LINK had saved.
;
; This surfaced in Kickstart 1.3 vsprintf at $F82128 (LINK A6, #-16)
; with A7 at $7FEEE pre-LINK: the save landed at $7FEEA (addr[1:0]=10),
; UNLK at $F82140 read back $00000000, A6 = 0 propagated to the next
; JSR -$228(A6) and trapped to $FFFFFDD8.
;
; Test: position A7 so LINK A6 writes to addr[1:0] = 2'b10, save a
; non-zero A6, call into a subroutine that builds a frame, return,
; then verify A6 came back intact.
;
; Pass = halt 0.  Failures:
;   $BAD1 = saved A6 not restored after UNLK (unaligned-.L path bug)
;   $BAD2 = A7 not restored to pre-LINK value
;   $BAD3 = nested LINK/UNLK breaks An restore
;   $BAD4 = aligned LINK/UNLK still works (sanity)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; ----- 1. Force A7 unaligned-long alignment ($7FEE)
        ;        and call a routine that does LINK A6, #-16. -----
        move.l  #$12345678, A6              ; A6 has a recognizable value
        move.l  #$00007FEE, A7              ; A7 mod 4 = 2 -> LINK writes
                                            ;  to $7FEA (addr[1:0]=10)
        bsr     fn1
        ; After fn1 returns, A6 should be $12345678 again.
        move.l  A6, D0
        cmpi.l  #$12345678, D0
        bne     fail1
        ; And A7 should be back at $7FEE.
        move.l  A7, D0
        cmpi.l  #$00007FEE, D0
        bne     fail2

        ; ----- 2. Aligned LINK/UNLK (sanity).  A7 = $7FF0,
        ;        LINK writes to $7FEC (addr[1:0]=00). -----
        move.l  #$AABBCCDD, A6
        move.l  #$00007FF0, A7
        bsr     fn2
        move.l  A6, D0
        cmpi.l  #$AABBCCDD, D0
        bne     fail4

        ; ----- 3. Nested LINK -- both An saves must round-trip. -----
        move.l  #$DEADBEEF, A5
        move.l  #$CAFEBABE, A6
        move.l  #$00007FFE, A7              ; mod 4 = 2 -> unaligned LINK
        bsr     fn3
        move.l  A6, D0
        cmpi.l  #$CAFEBABE, D0
        bne     fail3
        move.l  A5, D0
        cmpi.l  #$DEADBEEF, D0
        bne     fail3

        stop    #0

;------------------------------------------------------------
; fn1: LINK A6, #-16 ; ... ; UNLK A6 ; RTS
;------------------------------------------------------------
fn1:
        link    A6, #-16
        ; Touch the frame so the compiler can't optimize away.
        move.l  #$11111111, -4(A6)
        move.l  #$22222222, -8(A6)
        unlk    A6
        rts

;------------------------------------------------------------
; fn2: aligned LINK (A7 originally at long boundary).
;------------------------------------------------------------
fn2:
        link    A6, #-8
        move.l  #$33333333, -4(A6)
        unlk    A6
        rts

;------------------------------------------------------------
; fn3: nested LINK A5 inside LINK A6.
;------------------------------------------------------------
fn3:
        link    A6, #-8
        link    A5, #-8
        move.l  #$44444444, -4(A5)
        move.l  #$55555555, -4(A6)
        unlk    A5
        unlk    A6
        rts

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
