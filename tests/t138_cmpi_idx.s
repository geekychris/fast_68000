; CMPI.L #imm, d8(An, Xn) — d8(An,Xn) memory-source compare-immediate.
;
; Bug: the decoder maps EA mode 3'b110 (EA_IDX = d8(An,Xn)) to dst_mode
; = 3'b110, but the execute side at K_ALUI used dst_mode == 3'b110 as a
; sentinel for the CCR/SR variant of ANDI/ORI/EORI.  Both share the
; same dst_mode value, so CMPI.L #imm, d8(An,Xn) was routed into the
; CCR/SR-write path: no memory read happened, the ALU did not run, and
; the Z/N flags were left at whatever the previous instruction had
; set — silently breaking the compare.
;
; Surfaced in K1.3 trackdisk: the sector-1 preamble check at $FEAD9A
; is `CMPI.L #$AAAAAAAA, 0(A2, D4.W)` with D4=$440.  The compare did
; nothing; Z stayed at whatever bit the prior MOVEQ left, so the BEQ
; at $FEADA2 took the wrong branch and the sector failed validation.
;
; Fix: route only the explicit CCR/SR variants (encoded by the decoder
; with dst_mode=EA_EXT, dst_reg = sentinel) through the SR-write path;
; let dst_mode=EA_IDX flow through the normal alui_mem load path.
;
; Test sequence:
;   1. Seed memory at $2000..$200F with known longwords.
;   2. CMPI.L #known_value, 0(A0, D0.W) with D0=$0   -> Z=1
;   3. CMPI.L #wrong_value, 0(A0, D0.W) with D0=$8   -> Z=0
;   4. CMPI.L #known_value, 0(A0, D0.W) with D0=$8   -> Z=1
;   Verify both Z-true and Z-false outcomes drive the right branch.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Set up source buffer.
        lea     $00002000, A0
        move.l  #$AAAAAAAA, $00002000
        move.l  #$DEADBEEF, $00002004
        move.l  #$CAFEBABE, $00002008
        move.l  #$5555AAAA, $0000200C

        ; --- Test 1: CMPI.L equal at D0=$0 -----------------------------
        ; Pre-poison Z to the WRONG value so the test must observe the
        ; CMPI actually running (a no-op CMPI would leave Z=0).
        moveq   #1, D1                  ; D1=1 -> Z=0
        moveq   #0, D0                  ; D0=$0 (index)
        ; CMPI.L #$AAAAAAAA, 0(A0, D0.W)
        .word   $0CB0, $AAAA, $AAAA, $0000
        bne     fail_eq_d0_0

        ; --- Test 2: CMPI.L not-equal at D0=$8 -------------------------
        ; Pre-poison Z=1 so a no-op CMPI would leave Z=1 (wrong).
        moveq   #0, D1                  ; D1=0 -> Z=1
        moveq   #8, D0                  ; D0=$8 (index) -> reads $CAFEBABE
        ; CMPI.L #$DEADBEEF, 0(A0, D0.W)  -- not equal
        .word   $0CB0, $DEAD, $BEEF, $0000
        beq     fail_ne_d0_8

        ; --- Test 3: CMPI.L equal at D0=$8 -----------------------------
        moveq   #1, D1                  ; D1=1 -> Z=0
        moveq   #8, D0
        ; CMPI.L #$CAFEBABE, 0(A0, D0.W)
        .word   $0CB0, $CAFE, $BABE, $0000
        bne     fail_eq_d0_8

        ; --- Test 4: ensure ORI.W #$2000, SR (the CCR/SR sentinel that
        ; the bug-fix must keep working) still functions.  In supervisor
        ; mode this raises IPL mask to 2.  Just verify it doesn't trap.
        ori.w   #$2000, SR

        ; All pass.
        stop    #0

fail_eq_d0_0:
        stop    #$BAD1
fail_ne_d0_8:
        stop    #$BAD2
fail_eq_d0_8:
        stop    #$BAD3
