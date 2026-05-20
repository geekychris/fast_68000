; Unknown opcodes must trap to the right vector.
;
; Real 68000:
;   - $A___      -> Line A emulator,   vector 10 ($28)
;   - $F___      -> Line F emulator,   vector 11 ($2C)
;   - $4AFC      -> ILLEGAL,            vector 4  ($10)
;   - any other unrecognised opcode -> ILLEGAL,  vector 4
;
; Our decoder collapses everything it doesn't recognise into K_BAD.
; Until this fix K_BAD was a silent no-op which let Kickstart 1.3's
; CPU-feature probes ($4E7A/$4E7B/$F2xx/...) succeed silently and
; mis-detect us as 68030+.
;
; Test: install a vec-10 / vec-11 / vec-4 handler, execute one
; opcode of each kind, verify the right vector fired and the handler
; resumed cleanly.  Each handler bumps a counter so we can sanity-
; check there were no double-traps or wrong vectors.
;
; Pass = halt 0.  Failures:
;   $BAD1 = LINE A handler didn't run
;   $BAD2 = LINE F handler didn't run
;   $BAD3 = ILLEGAL ($4E7A) handler didn't run
;   $BAD4 = unknown opcode ($4AFC, the hard-illegal) handler didn't run
;   $BAD5 = handler ran but counter mismatched (= multiple traps)

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Install handlers into the vector table (in supervisor mode
        ; at reset).
        move.l  #linea_h, $28        ; vec 10 = LINE A
        move.l  #linef_h, $2C        ; vec 11 = LINE F
        move.l  #illegal_h, $10      ; vec 4 = ILLEGAL

        clr.l   D5                    ; LINE A count
        clr.l   D6                    ; LINE F count
        clr.l   D7                    ; ILLEGAL count

        ; Execute one $A___ opcode -> should trap to vec 10.
        .word    $A123
        ; Execute one $F___ opcode (no FPU) -> vec 11.
        .word    $F201
        ; Execute MOVEC (68010+) on 68000 -> vec 4.
        .word    $4E7A
        .word    $0002        ; MOVEC requires one extension word
        ; Execute $4AFC (Hard Illegal) -> vec 4.
        .word    $4AFC

        ; All four traps fired, counters should be 1, 1, 2.
        cmpi.l  #1, D5
        bne     fail_linea
        cmpi.l  #1, D6
        bne     fail_linef
        cmpi.l  #2, D7
        bne     fail_illegal_count

        stop    #0
forever:
        bra     forever

; Trap handlers.  Group-1 exception frame on SSP:
;   +0  SR (word)
;   +2  PC (long) -- of the faulting instruction
; Each opcode tested is 2 bytes (MOVEC's 1-word ext is fine -- we
; skip BOTH words by +4 for that one).  Bump the saved PC past the
; faulting opcode so RTE doesn't re-trigger.
linea_h:
        addq.l  #1, D5
        addq.l  #2, 2(A7)            ; skip the 1-word $A123 opcode
        rte
linef_h:
        addq.l  #1, D6
        addq.l  #2, 2(A7)            ; skip the 1-word $F201 opcode
        rte
illegal_h:
        addq.l  #1, D7
        ; MOVEC $4E7A is 2 words; plain $4AFC is 1 word.  Distinguish
        ; by reading the word at the saved-PC: $4E7A or $4E7B = MOVEC.
        move.l  2(A7), A0            ; A0 = saved PC
        move.w  (A0), D0
        andi.w  #$FFFE, D0           ; mask low bit (covers $4E7A/B)
        cmpi.w  #$4E7A, D0
        bne     ill_one_word
        addq.l  #4, 2(A7)            ; skip MOVEC + its ext
        rte
ill_one_word:
        addq.l  #2, 2(A7)             ; skip single-word illegal
        rte

fail_linea:         stop #$BAD1
fail_linef:         stop #$BAD2
fail_illegal_count: stop #$BAD3
