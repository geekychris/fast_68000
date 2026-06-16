; Floppy DSKLEN DMA -> DSKSYN interrupt (INTREQ bit 12, IPL 5).
;
; Our DMA is "minimig-style" memcpy with no PLL; the bus fires a
; one-cycle dsksyn pulse at the moment the DMA starts, modelling the
; real "sync word matched" event Paula would raise.  This test arms
; INTENA bit 12 (DSKSYN), kicks off a DSKLEN read, and verifies the
; level-5 autovector handler runs exactly once.

        .org $74                        ; level-5 autovec (29 * 4 = $74)
        .long dsksyn_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Zero sentinel + dest.
        move.l  #0, $00020000
        move.l  #0, $00020100
        move.l  #0, $00020104

        ; Allow IPL 5.  SR.IPL field is bits 8-10; force them all to 0.
        andi.w  #$F8FF, SR

        ; INTENA: SET (bit15) + INTEN (bit14) + DSKSYN (bit12).
        move.w  #$D000, $00DFF09A

        ; ADKCON: SET (bit15) + WORDSYNC (bit10) — required for Paula
        ; to fire DSKSYN-IRQ on sync match (or DMA start in our
        ; simplified model).
        move.w  #$8400, $00DFF09E

        ; DSKSYNC = $4489 (already the power-on default, but be explicit).
        move.w  #$4489, $00DFF07E

        ; BLKSRC = 0 (sector 0).
        move.l  #0, $00FE_8000
        ; DSKPT = $00020100.
        move.w  #$0002, $00DFF020
        move.w  #$0100, $00DFF022
        ; DSKLEN: DMAEN + word count 4.
        move.w  #$8004, $00DFF024

        moveq   #0, D3
wait_loop:
        addq.l  #1, D3
        cmpi.l  #$10000, D3
        bge     fail_timeout
        move.l  $00020000, D2
        beq     wait_loop

        cmpi.l  #1, D2
        bne     fail_count

        stop    #0

fail_count:    stop #$D101
fail_timeout:  stop #$D102

dsksyn_handler:
        move.l  D0, -(A7)
        move.l  $00020000, D0
        addq.l  #1, D0
        move.l  D0, $00020000
        ; CLR INTREQ.DSKSYN.
        move.w  #$1000, $00DFF09C
        move.l  (A7)+, D0
        rte
