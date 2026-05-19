; Address-error exception (vector 3) on misaligned word/long access.
;
; Issues a .w read from an odd address; the CPU must trap into the
; address-error handler (vector at $0C), bumping a sentinel counter.
; Repeat with a .l read from a non-mod-4 address.
;
; Coverage limitation: only detects accesses issued via the central
; S_RUN want_mem path.  MOVEM, RTE/RTR, exception stack pushes and TAS
; go through other dispatch paths and don't trap yet.

        .org $C                           ; vector 3 = address error
        .long addr_err_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        move.l  #0, $00010000             ; handler-entry counter

        ; ---- Misaligned word read ----
        move.l  #$00010001, A0            ; odd address
        move.w  (A0), D1                  ; <-- triggers VEC_ADDR_ERROR

        ; ---- Misaligned long read at ODD address ----
        move.l  #$00010003, A0            ; addr bit 0 = 1
        move.l  (A0), D1                  ; <-- triggers VEC_ADDR_ERROR

        ; ---- Misaligned word write ----
        move.l  #$00010005, A0            ; odd address
        move.w  #$1234, (A0)              ; <-- triggers VEC_ADDR_ERROR

        ; Counter should be 3.
        move.l  $00010000, D2
        cmpi.l  #3, D2
        bne     fail_count

        ; Confirm aligned accesses DON'T trap.
        ;   - long-aligned (mod-4) and word-aligned-but-not-mod-4 (.L at $02)
        ;     must both work; real 68000 only traps on odd addresses.
        move.l  #$00010008, A0
        move.l  $00010008, D1             ; .L at mod-4 — no trap
        move.l  $00010002, D1             ; .L at even-but-not-mod-4 — no trap
        move.w  #$5678, $00010010         ; .W at even — no trap
        move.l  $00010000, D2
        cmpi.l  #3, D2                    ; counter unchanged
        bne     fail_extra
        stop    #0

fail_count:   stop #$AE72
fail_extra:   stop #$AE73

; ---- Address-error handler ----
; Increments the counter at $00010000 and returns.  The real 68000
; (and our impl) builds a Group-0 stack frame: 8 extra bytes below
; SR (SSW + faulted addr + IR).  RTE itself pops only the canonical
; SR + PC, so we must drop those 8 bytes off A7 before RTE.  Our impl
; saves PC_next (after the faulting instruction) so we don't need to
; bump the saved PC.
addr_err_handler:
        addq.l  #1, D7                    ; sentinel
        move.l  $00010000, D0
        addq.l  #1, D0
        move.l  D0, $00010000
        ; Drop the 8 extra Group-0 bytes off A7 before RTE.
        addq.l  #8, A7
        rte
