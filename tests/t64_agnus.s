; Agnus stub: VHPOSR + DMACON SET/CLR.
; Pass = halt 0.  Failures: $BAD1..$BAD4.
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; 1. Read VHPOSR twice; second read must differ (beam advanced).
        move.l  $00DFF006, D0          ; t0 sample
        move.l  $00DFF006, D1          ; t1 sample (some cycles later)
        cmp.l   D0, D1
        beq     fail_static

        ; 2. DMACON write with SET (bit 15=1) should set bits in DMACONR.
        ;    Set DMAEN (bit 9) + BPLEN (bit 8) = $0300.  With bit 15 set:
        ;    written value = $8300.
        move.w  #$8300, $00DFF096      ; DMACON <- set BPLEN|DMAEN
        move.l  $00DFF002, D2          ; DMACONR readback
        andi.l  #$0300, D2
        cmpi.l  #$0300, D2
        bne     fail_set

        ; 3. DMACON write CLR (bit 15=0) should clear those bits.
        move.w  #$0300, $00DFF096      ; clear BPLEN|DMAEN
        move.l  $00DFF002, D3
        andi.l  #$0300, D3
        cmpi.l  #$0000, D3
        bne     fail_clr

        ; 4. VPOSR (16-bit read) must have 0 in low byte; high V is in bit 0
        ; of the high byte.  MOVE.W from $DFF004 returns only the VPOSR word.
        ; A MOVE.L from $DFF004 returns {VPOSR, VHPOSR} as one long (the
        ; canonical Amiga semantics Kickstart's beam-wait loop relies on),
        ; so the low byte of a .L read carries H[7:0] -- not zero.  Use .W
        ; here to keep the original meaning of "VPOSR low byte = 0".
        move.w  $00DFF004, D4
        andi.l  #$00FF, D4
        cmpi.l  #$0000, D4
        bne     fail_vpos
        stop    #0
forever:
        bra     forever
fail_static: stop #$BAD1
fail_set:    stop #$BAD2
fail_clr:    stop #$BAD3
fail_vpos:   stop #$BAD4
