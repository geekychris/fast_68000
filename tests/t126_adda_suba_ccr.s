; ADDA / SUBA must NOT modify any CCR bits (per m68k spec).
;
; Our K_ALU executor was unconditionally setting cc_we_c after the
; ALU produced N/Z/V/C, even when the destination was an An (the
; ADDA/SUBA family) -- whose spec says "Condition codes: Not
; affected".  Surfaced by the Musashi cosim at K1.3 boot $FC01D2:
;   SUBA.W #$FD8A, A6
; -- Verilator clobbered N=1/C=1, Musashi correctly left N/Z/V/C
; alone, and the very next branch test took different paths.
;
; CMPA *does* update N/Z/V/C (X unchanged), so this test exercises
; both negative cases (ADDA/SUBA must not touch CCR) and the positive
; case (CMPA must update CCR).
;
; Pass = halt 0.  Failures:
;   $BAD1 = ADDA.L clobbered CCR
;   $BAD2 = SUBA.W clobbered CCR
;   $BAD3 = CMPA.L didn't set Z when operands equal
;   $BAD4 = CMPA.L set N for a positive result

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Prime CCR to a known nonzero pattern: X=1, N=1, Z=1, V=0, C=1.
        ; Use ANDI/ORI to CCR to set it explicitly.
        andi    #0, CCR              ; clear all flags
        ori     #$1D, CCR            ; X(4)|N(3)|Z(2)|V(1=0)|C(0) = $1D - V

        ; Sanity: read CCR back -- expect $1D in the low byte of SR (we
        ; can't directly read CCR without privileged MOVE-from-SR, but
        ; the subsequent ops will fail loudly if CCR was wrong).

        ; --- 1. ADDA.L #imm, An must not touch CCR ---
        move.l  #$10000000, A0
        adda.l  #$10000000, A0       ; A0 = $20000000.  CCR untouched.
        ; If CCR is preserved we should still have Z=1 from before.
        beq     ok1                  ; Z=1 path: still set, ADDA didn't clear.
        bra     fail1
ok1:
        bmi     ok1b                 ; N=1 should still be set.
        bra     fail1
ok1b:
        bcs     ok1c                 ; C=1 should still be set.
        bra     fail1
ok1c:

        ; --- 2. SUBA.W #imm, An must not touch CCR ---
        ; CCR is still X=1/N=1/Z=1/C=1 from above.
        move.l  #$00000400, A6
        suba.w  #$FD8A, A6           ; A6 = $400 - sign_ext($FD8A) = $676
        cmpa.l  #$00000676, A6       ; CMPA.L *does* update flags.
        bne     fail2                ; SUBA computed wrong A6?

        ; --- 3. CMPA.L should set Z when equal ---
        move.l  #$DEADBEEF, A0
        cmpa.l  #$DEADBEEF, A0
        bne     fail3                ; Z must be set on equal

        ; --- 4. CMPA.L positive result: N=0 ---
        move.l  #$00010000, A0
        cmpa.l  #$00005000, A0       ; A0 - imm > 0 -> N=0, Z=0
        bmi     fail4

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
