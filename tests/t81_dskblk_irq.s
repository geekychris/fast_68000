; Floppy DSKLEN DMA -> DSKBLK interrupt (INTREQ bit 1, IPL 1).
;
; Programs a DSKLEN block transfer from disk[] (or rather, from the
; block-device source register pointing at the pre-loaded disk image)
; into RAM, enables INTENA.INTEN | INTENA.DSKBLK, and verifies the
; handler runs exactly once via a sentinel counter.
;
; The disk[] array contents are determined by DISK_HEXFILE; this test
; doesn't care about the payload, just that DMA completes and the
; interrupt fires.

        .org $64                        ; auto-vector IRQ 1 (vector 25 = $19 * 4 = $64)
        .long dskblk_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Zero the handler counter and dest buffer.
        move.l  #0, $00020000
        move.l  #0, $00020100
        move.l  #0, $00020104
        move.l  #0, $00020108

        ; Set sup-mode IPL mask = 0 so IPL 1 (DSKBLK) gets through.
        andi.w  #$F8FF, SR

        ; INTENA: INTEN (bit 14) + DSKBLK (bit 1) -> SET both via bit 15 form.
        move.w  #$C002, $00DFF09A        ; SET-bit 15 + INTEN(14) + DSKBLK(1)

        ; Program block-device source (sector 0).
        move.l  #0, $00FE_8000           ; BLKSRC = 0
        ; Set DSKPT = $00020100 (dest buffer).
        move.w  #$0002, $00DFF020        ; DSKPTH
        move.w  #$0100, $00DFF022        ; DSKPTL
        ; DSKLEN: bit 15 = DMAEN, bits 13:0 = word count. 4 words = 8 bytes.
        move.w  #$8004, $00DFF024

        ; Wait for the handler to run.  Poll the sentinel.
        moveq   #0, D3                   ; loop budget
wait_loop:
        addq.l  #1, D3
        cmpi.l  #$10000, D3
        bge     fail_timeout
        move.l  $00020000, D2
        beq     wait_loop                ; counter still 0 -> spin

        ; Counter should be exactly 1.
        cmpi.l  #1, D2
        bne     fail_count

        stop    #0

fail_count:    stop #$D101
fail_timeout:  stop #$D102

; ---- DSKBLK handler ----
; Bump sentinel + clear the pending INTREQ.DSKBLK so it doesn't refire.
dskblk_handler:
        move.l  D0, -(A7)
        move.l  $00020000, D0
        addq.l  #1, D0
        move.l  D0, $00020000
        ; INTREQ: CLR-bit 15 = 0 -> CLR mode; bit 1 = DSKBLK.
        move.w  #$0002, $00DFF09C
        move.l  (A7)+, D0
        rte
