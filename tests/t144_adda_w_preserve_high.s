; Regression: ADDA.W must NOT clobber the high half of the
; destination An.
;
; 68k semantics: ADDA.W <ea>, An sign-extends the source word to
; long and adds to the FULL 32-bit An.  Our size-aware ALU was
; masking BOTH operands to the op size before adding, which silently
; zeroed the high half of An when source was small and An.W had no
; sign bit.  Surfaced by the WB1.3 deadlock at r=4058118: intuition's
; gadget-template-init helper at $FD643A did:
;     move.l  #$00FD6062, A3
;     move.l  A3, A0
;     move.l  A0, A1
;     adda.w  D0, A1     ; D0 = 0
; A1 ended up as $00006062 instead of $00FD6062, so the subsequent
; 11-long MOVE.L (A1)+, (A0)+ copied chip-RAM garbage into the
; per-Screen gadget templates.  Every system gadget then had
; GadgetType = 0, intuition's cleanup misinterpreted minterms as a
; graphics pointer, ObtainSemaphore was eventually called on
; $CACACB12 ($CACACACA + $48) -> Wait(bit 4) forever.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Set A1 to a value with the high byte clearly set
        ; ($00FD6062 -- the exact value that exposed the bug).
        move.l  #$00FD6062, A1
        ; D0 = 0 sign-extends to $00000000.  ADDA.W with src=0
        ; should leave A1 unchanged.
        moveq   #0, D0
        adda.w  D0, A1
        cmpa.l  #$00FD6062, A1
        bne     fail_zero

        ; Positive source: A1 = $00FD0000, D0 = $00000100.
        ; signext($0100) = $00000100; A1 + $100 = $00FD0100.
        move.l  #$00FD0000, A1
        move.l  #$00000100, D0
        adda.w  D0, A1
        cmpa.l  #$00FD0100, A1
        bne     fail_pos

        ; Negative source: A1 = $00FD0100, D0.W = $FFFF (-1).
        ; signext($FFFF) = $FFFFFFFF; A1 + (-1) = $00FD00FF.
        move.l  #$00FD0100, A1
        move.l  #$0000FFFF, D0
        adda.w  D0, A1
        cmpa.l  #$00FD00FF, A1
        bne     fail_neg

        ; SUBA.W with src=0: A1 = $00FD6062, A1 stays $00FD6062.
        move.l  #$00FD6062, A1
        moveq   #0, D0
        suba.w  D0, A1
        cmpa.l  #$00FD6062, A1
        bne     fail_suba

        ; SUBA.W with positive source: A1 = $00FD0100, D0 = $0001.
        ; A1 - 1 = $00FD00FF.
        move.l  #$00FD0100, A1
        move.l  #$00000001, D0
        suba.w  D0, A1
        cmpa.l  #$00FD00FF, A1
        bne     fail_suba_pos

        ; Immediate-source form: ADDA.W #imm, An.  Same semantics.
        move.l  #$00FD6062, A1
        adda.w  #0, A1
        cmpa.l  #$00FD6062, A1
        bne     fail_imm

        stop    #0

fail_zero:    stop  #$BA01
fail_pos:     stop  #$BA02
fail_neg:     stop  #$BA03
fail_suba:    stop  #$BA04
fail_suba_pos:stop  #$BA05
fail_imm:     stop  #$BA06
