; t142 -- CIA-A TOD increments on VBL pulse, not fixed prescaler.
;
; Real Amiga wires CIA-A's TOD counter to Agnus VSYNC (50 Hz PAL /
; 60 Hz NTSC).  K1.3's timer.device reads CIA-A TOD on every VBL to
; compute elapsed time and complete pending TR_ADDREQUESTs.  Our
; previous model ticked TOD every 1024 host clocks — about 70x faster
; than VBL — which broke timer.device's math and left input.device
; wedged in its initial Wait() forever.  CLI never unblocked.
;
; This test:
;   1. Reads CIA-A TOD (3 bytes: TODHI/TODMID/TODLOW at $BFEA01/$BFE901/$BFE801)
;   2. Spins long enough for at least one VBL (~71K host clocks).
;   3. Re-reads TOD.
;   4. Verifies TOD has advanced by at least 1 and at most ~5
;      (a normal CIA-A TOD on VBL increments once per VBL).
;
; Without the fix, TOD would increment ~70 times in this window.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- First, run long enough for at least one VBL so the CIA
        ;      switches into external-tick mode (the prescaler fallback
        ;      ticks until the first VBL, which would otherwise dominate
        ;      our delta measurement below).  VBL period is ~71K host
        ;      cycles; spin >100K to be safe.
        move.l #100000, D0
warmup:
        subq.l #1, D0
        bne    warmup

        ; ---- Read TOD #1.  CIA-A TODHI latches all three bytes on read
        ;      so we must read HI, MID, LOW in that order.
        ;
        ; CIA-A register byte addresses:
        ;   $BFE801  TODLOW
        ;   $BFE901  TODMID
        ;   $BFEA01  TODHI
        moveq  #0, D1
        move.b $00BFEA01, D1            ; D1 = $000000HH
        lsl.l  #8, D1                    ; D1 = $0000HH00
        move.b $00BFE901, D1            ; D1 = $0000HHMM
        lsl.l  #8, D1                    ; D1 = $00HHMM00
        move.b $00BFE801, D1            ; D1 = $00HHMMLL = TOD #1

        ; ---- Spin ~150K host cycles (covers >1 VBL at our 70K-cycle frame).
        move.l #50000, D0
spin1:
        subq.l #1, D0
        bne    spin1

        ; ---- Read TOD #2.
        moveq  #0, D2
        move.b $00BFEA01, D2
        lsl.l  #8, D2
        move.b $00BFE901, D2
        lsl.l  #8, D2
        move.b $00BFE801, D2            ; D2 = TOD #2

        ; ---- TOD must have advanced.
        cmp.l  D1, D2
        beq    fail_no_tick             ; no advance at all

        ; ---- TOD must NOT have advanced by more than ~5 (3 VBLs of
        ;      slack; real CIA-A TOD ticks at 50/60 Hz).  Previously we
        ;      had a 1024-cycle prescaler that incremented TOD ~70x per
        ;      VBL, so a 150K spin saw a TOD delta of ~150 — fail.
        move.l D2, D3
        sub.l  D1, D3
        cmpi.l #10, D3
        bhi    fail_too_fast

        stop   #0

fail_no_tick:   stop #$c200
fail_too_fast:  stop #$c201
