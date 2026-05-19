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

        ; ---- Misaligned long read ----
        move.l  #$00010002, A0            ; addr[1:0] = 10 (not mod 4)
        move.l  (A0), D1                  ; <-- triggers VEC_ADDR_ERROR

        ; ---- Misaligned word write ----
        move.l  #$00010003, A0            ; odd address
        move.w  #$1234, (A0)              ; <-- triggers VEC_ADDR_ERROR

        ; Counter should be 3.
        move.l  $00010000, D2
        cmpi.l  #3, D2
        bne     fail_count

        ; Confirm aligned accesses DON'T trap.
        move.l  #$00010008, A0            ; long-aligned
        move.l  $00010008, D1             ; (read; no trap)
        move.w  #$5678, $00010010         ; word at even addr (no trap)
        move.l  $00010000, D2
        cmpi.l  #3, D2                    ; counter unchanged
        bne     fail_extra
        stop    #0

fail_count:   stop #$AE72
fail_extra:   stop #$AE73

; ---- Address-error handler ----
; Increments the counter at $00010000 and returns.  Note that on the
; real 68000 the Group-0 stack frame has 4 extra words above the SR;
; our minimal impl uses the same short 6-byte frame as other
; exceptions, so plain `rte` works.  When we land the full Group-0
; frame, the handler will need to adjust A7 by 8 first.
addr_err_handler:
        addq.l  #1, D7                    ; D7 = arbitrary sentinel
        move.l  $00010000, D0
        addq.l  #1, D0
        move.l  D0, $00010000
        rte
