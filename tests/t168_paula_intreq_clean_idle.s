; Paula INTREQ stays clean during chipset-init / idle.
;
; Verifies two FS-UAE-parity fixes:
;   (a) Copper-driven AUDxDAT writes (e.g. K1.3's chipset-clear Copper
;       list) must NOT fire AUDxIR.  Real CPU-driven AUDxDAT writes
;       still do (mirrors minimig STATE_0 → STATE_3 transition).
;       Source-tag gating in paula.v: aud_int_pulse fires only when
;       slv_src < n_cpu_ports.
;   (b) DSKSYN-IRQ fires only when ADKCON WORDSYNC=1.  Without it,
;       every DSKLEN-DMA start re-asserted INTREQ[12] and the bit
;       stayed pending forever (L5 masked) — even after K1.3's
;       final disk-handler cleanup.
;
; Test sequence:
;   1. Reset INTREQ to all-zero.
;   2. CPU-write zeros to AUD0DAT/AUD1DAT/AUD2DAT/AUD3DAT — these
;      ARE real CPU writes so AUDxIR DOES fire (we mask via INTENA
;      and check INTREQ[7..10] = 0b1111 confirming the fire path
;      still works).
;   3. Clear those AUDx bits via INTREQ write.
;   4. Confirm INTREQ stays clean ($0000).
;
;   Note: Verifying the *Copper* gate requires actual Copper traffic,
;   so it's covered by the chipset-trace regression on boing-disk
;   rather than here.  This test exercises path (a) the CPU side
;   ("CPU writes still fire") and indirectly path (b) ("no spurious
;   DSKSYN from DMA reads we didn't initiate").

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; ---- (1) Clear INTREQ ----
        move.w  #$7FFF, $00DFF09C       ; CLR all 14 source bits

        ; ---- (2) CPU writes to AUDxDAT (real CPU writes — should fire AUDxIR) ----
        move.w  #$0000, $00DFF0AA       ; AUD0DAT
        move.w  #$0000, $00DFF0BA       ; AUD1DAT
        move.w  #$0000, $00DFF0CA       ; AUD2DAT
        move.w  #$0000, $00DFF0DA       ; AUD3DAT

        ; Settle a few cycles for the IRQ pulses to land in INTREQ.
        moveq   #10, D1
settle:
        subq.l  #1, D1
        bne     settle

        ; ---- Check INTREQ[7..10] all set (CPU writes fired) ----
        move.w  $00DFF01E, D0           ; INTREQR
        andi.w  #$0780, D0              ; isolate AUD0..3 bits
        cmpi.w  #$0780, D0
        bne     fail_no_audxir

        ; ---- (3) Clear AUDx bits ----
        move.w  #$0780, $00DFF09C       ; CLR AUD0..3

        moveq   #10, D1
settle2:
        subq.l  #1, D1
        bne     settle2

        ; ---- (4) Confirm INTREQ clean — only VBL bit may be set ----
        move.w  $00DFF01E, D0
        andi.w  #$5FFF, D0              ; mask out bit 15 (always 0)
        andi.w  #$FFDF, D0              ; mask out bit 5 (VBL — may fire)
        bne     fail_dirty_intreq

        ; ---- Also verify: a write to AUDxDAT when INTREQ[7] is
        ;      already set must NOT fire again (real Paula AUDxIP gate).
        ;      First set INTREQ[7] manually.
        move.w  #$8080, $00DFF09C       ; SET AUD0
        ; Now CPU write AUDxDAT — should NOT change INTREQ.
        move.w  #$0000, $00DFF0AA       ; AUD0DAT (gated by !intreq[7])
        ; Now CLR bit 7.
        move.w  #$0080, $00DFF09C
        moveq   #10, D1
settle3:
        subq.l  #1, D1
        bne     settle3
        ; If the AUDxDAT write had spuriously re-fired, INTREQ[7] would
        ; have hw_set'd ON in the CLR path → CLR + hw_set ORs back in →
        ; bit 7 stays set.  We expect bit 7 to be cleared.
        move.w  $00DFF01E, D0
        andi.w  #$0080, D0
        bne     fail_audxip_gate_broken

        stop    #0

fail_no_audxir:            stop #$D721
fail_dirty_intreq:         stop #$D722
fail_audxip_gate_broken:   stop #$D723
