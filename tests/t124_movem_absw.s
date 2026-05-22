; MOVEM regs ↔ abs.W must use the absolute-short address, not An.
;
; The K_MOVEM EA setup had a `default: movem_addr <= ex_ra` that
; fired for any EA mode the case didn't handle -- including EA_EXT
; (abs.W / abs.L / PC-rel).  K1.3 at $FC2FF0 does
;   MOVEM.L D0-D7/A0-A7, $0180.W
; to clear a 64-byte exec save area.  Our CPU collapsed that EA
; to ex_ra (which still holds whatever An source value the regfile
; muxed in) and the MOVEM wrote 16 longs starting at that random
; address -- in K1.3 boot that landed on top of ExecBase at $4 with
; $FFFFFFFF, which then steered the warm-reset trampoline at
; $FC05FC (MOVEA.L $4.W, A0 ; JMP (A0)) to PC=$FFFFFFFF.  PC ran
; away through low memory until it eventually hit a $FFFF word and
; trapped at chip-RAM $11FAC.
;
; Test: MOVEM.L D0-D3, $1000.W (reg→mem) writes four longs
; starting at $1000 (NOT at some register's contents).  Then
; MOVEM.L $1000.W, D4-D7 (mem→reg) loads them back.  Verify both
; directions hit the right address.
;
; Pass = halt 0.  Failures:
;   $BAD1 = D0 didn't write to $1000
;   $BAD2 = D1 didn't write to $1004
;   $BAD3 = D2 didn't write to $1008
;   $BAD4 = D3 didn't write to $100C
;   $BAD5 = mem→reg D4 didn't load $1000

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Pre-fill the destination region so we'd see if writes miss.
        move.l  #$FFFFFFFF, $1000.w
        move.l  #$FFFFFFFF, $00001004
        move.l  #$FFFFFFFF, $00001008
        move.l  #$FFFFFFFF, $0000100C

        ; Load D0-D3 with distinct values, and stuff random crap
        ; into A0-A6 so we know movem_addr isn't grabbing ex_ra.
        move.l  #$AAAA0001, D0
        move.l  #$BBBB0002, D1
        move.l  #$CCCC0003, D2
        move.l  #$DDDD0004, D3
        move.l  #$DEADBEEF, A0
        move.l  #$CAFEBABE, A1
        move.l  #$FEEDFACE, A2

        ; MOVEM.L D0-D3, $1000.W -- writes 4 longs starting at $1000.
        movem.l D0-D3, $1000.w

        ; Verify each long landed at the right address.
        move.l  $1000.w, D0
        cmpi.l  #$AAAA0001, D0
        bne     fail1
        move.l  $00001004, D0
        cmpi.l  #$BBBB0002, D0
        bne     fail2
        move.l  $00001008, D0
        cmpi.l  #$CCCC0003, D0
        bne     fail3
        move.l  $0000100C, D0
        cmpi.l  #$DDDD0004, D0
        bne     fail4

        ; mem→reg direction: MOVEM.L $1000.W, D4-D7.
        movem.l $1000.w, D4-D7
        cmpi.l  #$AAAA0001, D4
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
