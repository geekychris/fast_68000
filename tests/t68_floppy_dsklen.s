; DSKLEN-driven DMA test.  Uses the floppy register protocol
; (DSKPTH/L at $DFF020/22, DSKLEN at $DFF024) on top of the same
; backing disk[] array as t65_blockdev.  Pass = halt 0.
;
; Sequence:
;   1. Set BLKSRC (sector 0) — our non-canonical "head position".
;   2. Write DSKPT = $00007000 in two halves.
;   3. Write DSKLEN = $8000 | (256 words count in low 14 bits).
;      Bit 15 = DMAEN start.  We program 256 words = 512 bytes = one
;      sector; that matches blk_cnt = 1.
;   4. Spin until BLKCMD reads 0 (transfer done).
;   5. Check that $00007000 holds $CAFEBEEF (sector 0 word 0).
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; 1. Position "head" at sector 0 (BLKSRC = 0).
        move.l  #0,            $00FE8000

        ; 2. DSKPT = $00007000.
        move.w  #$0000, $00DFF020      ; DSKPTH (high half)
        move.w  #$7000, $00DFF022      ; DSKPTL (low half)

        ; 3. DSKLEN start: $8000 (DMAEN) | $0100 = 256 words.
        move.w  #$8100, $00DFF024

        ; 4. Spin on BLKCMD until done.
wait_dma:
        move.l  $00FE800C, D0
        tst.l   D0
        bne     wait_dma

        ; 5. Verify sector 0 word 0.
        move.l  $00007000, D1
        cmpi.l  #$CAFEBEEF, D1
        bne     fail_data

        stop    #0
forever: bra forever
fail_data: stop #$BAD1
