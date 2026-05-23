; Exception-entry USP capture: address-error from a user-mode IDX-dest
; MOVE.B at an odd address.  Verifies that S_EXC_PUSH_SR saves the
; *actual* user A7 (USP) into usp_shadow rather than rf_rc_data, which
; routes through id_rc_idx (= the preempted instruction's IDX-mode Xn,
; not A7).
;
; Pre-fix bug: K1.3 strap.task ran with A7=$188A; a level-3 IRQ
; preempted MOVE.B at $FC6BF6 (Xn=D1, value $0), saving D1 into
; usp_shadow instead of A7.  RTE then bounced user with USP=$0,
; and K1.3's LaunchPoint propagated the corrupt SP forward into
; a self-perpetuating cycle that ended in a Guru.
;
; Test plan (deterministic, no real IRQ needed):
;   - Enter user mode with A7 = $00010F00 and D1 = $DEADBEEF.
;   - Execute MOVE.B src,(d8,A1,D1.W) where the resulting address is
;     odd and EA is computed via IDX mode (id_rc_idx = D1, NOT 15).
;     The CPU raises an address error (vec 3, group-0) -- which uses
;     the same usp_shadow capture path as a real IRQ.
;   - Address-error handler reads USP via MOVE USP,A0 and stashes it.
;   - The pre-fix code would store $DEADBEEF (= D1) instead of
;     $00010F00 (= A7).

        .org $C                         ; vector 3 = address error
        .long addr_err_handler
        .org $84                        ; vector 33 = TRAP #1
        .long trap1_handler
        .org $B8                        ; vector 46 = TRAP #14
        .long fail_handler14

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        move.l  #0, $00020100           ; sentinel slot
        move.l  #0, $00020104           ; sentinel slot 2 (A7 readback)

        ; usp_shadow := $00010F00 via MOVE A0,USP ($4E60).
        move.l  #$00010F00, A0
        .word   $4E60

        ; A1 base, D1 sentinel.  We want the EA = A1 + D1.W + d8 to be
        ; odd; D1.W = $BEEF (sign-extended -16657) and we add d8=1.
        ; Address-error path triggers for MOVE.B writes too if the EA
        ; ends up odd?  Actually MOVE.B doesn't check alignment.  Use
        ; MOVE.W (or .L) instead -- those DO check alignment.
        ;
        ; MOVE.W src,(d8,A1,D1.W): src is a constant; dst EA computed.
        move.l  #$00020000, A1
        move.l  #$DEADBEEF, D1

        ; Drop into user mode.  A7 will become USP=$00010F00.
        move.w  #$0000, SR

        ; Trigger address error: MOVE.W D0,1(A1,D1.W).
        ; D1.W = $BEEF, sign-extended W to L = $FFFFBEEF (= -16657).
        ; EA = A1 + $FFFFBEEF + 1 = $00020000 + $FFFFBEEF + 1 = $0001BEF0
        ; which is even (last nibble 0) -- not enough.  Use d8 = 3 to
        ; force an odd EA: $00020000 + $FFFFBEEF + 3 = $0001BEF2 -- still
        ; even.  D1.W = $BEEF, mode 6 sign-extends to long: $FFFFBEEF.
        ; To get odd, displacement+xreg sum must be odd.  Use d8 = 0:
        ; EA = $00020000 + $FFFFBEEF + 0 = $0001BEEF -- odd!
        move.w  D0, 0(A1, D1.W)
        ; Should not reach here.
        trap    #14

        .org $500
addr_err_handler:
        ; A bus/address error is a Group-0 frame: 14-byte frame.  We
        ; just need to read USP from usp_shadow; then STOP cleanly.
        move.l  A0, -(A7)
        .word   $4E68                   ; MOVE USP,A0
        move.l  A0, $00020100           ; saved USP

        ; Compare to expected.
        move.l  $00020100, D2
        cmpi.l  #$00010F00, D2
        bne     fail_in_handler

        stop    #0

fail_in_handler:
        ; Distinguish bug fingerprint: D1 was $DEADBEEF, so pre-fix
        ; usp_shadow would equal $0000BEEF (D1 sign-extended as word
        ; in the rc port? Actually rf_rc_data reads regs[id_rc_idx]
        ; as a full 32-bit longword, so it's $DEADBEEF).  We don't
        ; distinguish here -- just stop with a unique code.
        stop    #$D101

trap1_handler:
        stop    #0
fail_handler14:
        stop    #$D102
