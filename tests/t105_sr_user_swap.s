; SR write that clears S must swap A7 with usp_shadow.
;
; Background: on real 68000, A7 is a dual-register pair -- SSP (when
; S=1) and USP (when S=0).  The active SSP/USP visible as A7 switches
; whenever S changes.  Our impl previously caught the S=1→0 swap only
; on RTE; ANDI/ORI/EORI to SR and MOVE-to-SR saved the supervisor SP
; into usp_shadow but never loaded the user SP into A7.  A7 then held
; the supervisor stack pointer in user mode, so user-mode pushes
; clobbered supervisor data.
;
; Kickstart 1.3's boot task enters user mode via `ANDI.W #$0000, SR`
; at $F807CE.  Without the A7 swap, the user-mode boot task uses the
; system supervisor stack as its USP, and a later priv-vio trap's
; supervisor-mode MOVEM then writes back over the user-mode push that
; was just made -- corrupting the BSR return on stack and crashing.
;
; The fix: when a supervisor-mode SR write transitions S 1→0, save
; the current A7 to usp_shadow AND load the previous usp_shadow value
; into A7.
;
; Test plan:
;   1. Enter supervisor mode (we already are at reset).
;   2. MOVE.L A7, D0 to capture the initial SSP.
;   3. MOVE.L #$00012000, A0 ; MOVE A0, USP — set USP to a known value.
;   4. ANDI.W #$0000, SR — clear S (and everything else).
;   5. After: A7 (now USP) should equal $00012000.  And if we trap
;      back to supervisor, A7 should restore to old SSP.
;
; Pass = halt 0.  Failures: $BAD1 (A7 didn't equal USP after S clear),
; $BAD2 (sentinel at USP wasn't preserved), $BAD3 (trap didn't recover
; the SSP).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Capture original SSP and seed both stacks with sentinels so
        ; we can detect a stack swap.
        move.l  A7, D7              ; D7 = SSP
        move.l  #$DEADBEEF, $00003FFC ; SSP-area sentinel (just below SP=$4000)
        move.l  #$CAFEBABE, $00001FFC ; USP-area sentinel (below the USP target)

        ; Set USP to $00002000.
        lea     $00002000, A0
        move.l  A0, USP

        ; Sanity: A7 still SSP, not USP.
        cmpa.l  D7, A7
        bne     fail_a7_pre

        ; Drop to user mode.  Real-hardware semantics: A7 becomes the
        ; USP we just programmed, old A7 (SSP) is hidden.
        andi.w  #$0000, SR

        ; Now in user mode.  A7 must be $00002000.
        move.l  A7, D1
        cmpi.l  #$00002000, D1
        bne     fail_user_a7

        ; Push a sentinel value onto the user stack.  Predec uses A7
        ; so it had better be USP now.
        move.l  #$AABBCCDD, -(A7)
        ; A7 should be $1FFC; the value lands at $1FFC..$1FFF.
        move.l  A7, D2
        cmpi.l  #$00001FFC, D2
        bne     fail_user_push_an

        ; The push must have replaced the USP-area sentinel, not the
        ; SSP-area sentinel.  Both checks together prove the stacks
        ; are distinct in memory.
        move.l  $00001FFC, D3
        cmpi.l  #$AABBCCDD, D3
        bne     fail_user_push_data
        move.l  $00003FFC, D4
        cmpi.l  #$DEADBEEF, D4
        bne     fail_ssp_clobbered

        ; Trigger a TRAP #0 to return to supervisor.  The trap pushes
        ; SR + PC on the SSP, so A7 becomes (old SSP - 6) inside the
        ; handler.  Vector 32 points at our handler.
        trap    #0

        ; Resumed after the trap-via-RTE.  Should be back in user
        ; mode with USP = $1FFC unchanged.
        move.l  A7, D5
        cmpi.l  #$00001FFC, D5
        bne     fail_resume_usp

        stop    #0
forever:
        bra     forever

        ; ---- TRAP #0 handler ----
        ; In supervisor mode now.  A7 should be back near our initial
        ; SSP (D7 - 6 due to frame).  Verify and RTE.
trap0:
        ; Compute D7 - 6 (expected SSP just after frame pushed).
        move.l  D7, D6
        subi.l  #6, D6
        cmpa.l  D6, A7
        bne     fail_trap_ssp
        rte

        ; Wire TRAP #0 vector at $0080 to trap0.  Done via .org.
        .org $0080
        .long   trap0

fail_a7_pre:        stop #$BAD1
fail_user_a7:       stop #$BAD2
fail_user_push_an:  stop #$BAD3
fail_user_push_data:stop #$BAD4
fail_ssp_clobbered: stop #$BAD5
fail_resume_usp:    stop #$BAD6
fail_trap_ssp:      stop #$BAD7
