; DSKSYN-IRQ fires only when ADKCON WORDSYNC=1.
;
; Real Paula gates DSKSYN-IRQ on ADKCON bit 10 (WORDSYNC).  Without
; this gate, every DSKLEN-DMA start re-asserts INTREQ[12], leading to
; stuck DSKSYN bits across multiple disk reads.
;
; Test:
;   Phase 1 — WORDSYNC OFF: kick a DSKLEN read, confirm INTREQ[12] NOT set.
;   Phase 2 — WORDSYNC ON:  kick a DSKLEN read, confirm INTREQ[12] IS set.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Clear INTREQ.
        move.w  #$7FFF, $00DFF09C

        ; DSKSYNC = $4489 (Amiga MFM sync word).
        move.w  #$4489, $00DFF07E

        ; ADKCON CLR all bits (no WORDSYNC).
        move.w  #$7FFF, $00DFF09E

        ; ---- Phase 1: DSKLEN read with WORDSYNC=0 ----
        ; BLKSRC = 0 (sector 0).
        move.l  #0, $00FE_8000
        ; DSKPT = $00020100.
        move.w  #$0002, $00DFF020
        move.w  #$0100, $00DFF022
        ; DSKLEN: DMAEN + 4 words.
        move.w  #$8004, $00DFF024

        ; Let DMA + any spurious DSKSYN settle.
        moveq   #50, D1
spin1:
        subq.l  #1, D1
        bne     spin1

        ; INTREQ[12] must NOT be set.
        move.w  $00DFF01E, D0
        andi.w  #$1000, D0
        bne     fail_phase1_dsksyn_fired

        ; ---- Phase 2: DSKLEN read with WORDSYNC=1 ----
        move.w  #$8400, $00DFF09E       ; ADKCON SET WORDSYNC
        ; New DSKLEN read.
        move.w  #$0002, $00DFF020
        move.w  #$0200, $00DFF022       ; different dest
        move.w  #$8004, $00DFF024

        moveq   #50, D1
spin2:
        subq.l  #1, D1
        bne     spin2

        ; INTREQ[12] MUST be set.
        move.w  $00DFF01E, D0
        andi.w  #$1000, D0
        beq     fail_phase2_no_dsksyn

        stop    #0

fail_phase1_dsksyn_fired:  stop #$D731
fail_phase2_no_dsksyn:     stop #$D732
