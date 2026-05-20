; MOVE-FROM-SR to memory destinations (e.g. -(An)) must actually do
; the store and update the An.
;
; Background: real 68000's MOVE-FROM-SR accepts any data-alterable EA,
; including memory destinations -(An), (An)+, (An), d16(An), etc.
; Our K_MOVESR handler originally only supported the Dn destination
; path -- memory destinations were a silent no-op, leaving An unchanged
; and never writing SR to memory.
;
; Kickstart 1.3's privilege-violation handler uses the sequence:
;     ORI.W #$2000, SR        ; enter supervisor
;     PEA <ret>               ; push synthetic PC for later RTE
;     MOVE SR, -(A7)          ; push synthetic SR for later RTE
;     JMP (A5)                ; call into scheduler / dispatcher
; If MOVE SR, -(A7) doesn't decrement A7 *and* doesn't write SR, the
; subsequent dispatcher sees a misaligned/wrong RTE frame and the boot
; reads garbage when it eventually pops it.
;
; Verification strategy: do MOVE.W SR, Dn and MOVE.W SR, mem back-to-back
; with no intervening flag-changing op, so both readings see the same
; SR.  Compare the stored memory value to Dn.
;
; Pass = halt 0.  Failures: $BAD1 (predec An wrong), $BAD2 (predec
; memory mismatch), $BAD3 (postinc An wrong), $BAD4 (postinc memory
; mismatch), $BAD5 (sentinel overrun).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Seed buffer + sentinel before any flag-changing ops we plan
        ; to keep undisturbed during the back-to-back SR captures.
        move.l  #$DEADBEEF, $00001FFC
        move.l  #$CAFEBABE, $00002000

        ; ---- Predec test: capture SR into D5 then push to -(A0) ----
        lea     $2000, A0
        move.w  SR, D5           ; reference
        move.w  SR, -(A0)        ; under-test write; same SR (no flag ops between)
        andi.l  #$FFFF, D5
        cmpa.l  #$00001FFE, A0
        bne     fail_predec_an
        move.w  $00001FFE, D1
        andi.l  #$FFFF, D1
        cmp.l   D5, D1
        bne     fail_predec_mem

        ; ---- Postinc test: capture SR into D6 then push to (A0)+ ----
        ; Clobber the slot first so we can tell it was rewritten.
        move.w  #$1234, $00001FFE
        lea     $00001FFE, A0
        move.w  SR, D6           ; reference
        move.w  SR, (A0)+        ; under-test write
        andi.l  #$FFFF, D6
        cmpa.l  #$00002000, A0
        bne     fail_postinc_an
        move.w  $00001FFE, D2
        andi.l  #$FFFF, D2
        cmp.l   D6, D2
        bne     fail_postinc_mem

        ; Sentinel above the buffer should be intact.
        move.l  $00002000, D3
        cmpi.l  #$CAFEBABE, D3
        bne     fail_overrun

        stop    #0
forever:
        bra     forever
fail_predec_an:  stop #$BAD1
fail_predec_mem: stop #$BAD2
fail_postinc_an: stop #$BAD3
fail_postinc_mem:stop #$BAD4
fail_overrun:    stop #$BAD5
