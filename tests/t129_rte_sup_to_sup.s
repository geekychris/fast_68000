; RTE staying in supervisor mode must advance A7 by 6
; (2 bytes SR + 4 bytes PC), not 2.
;
; The K_RTE handler committed wb_aux_data_c = working_sp at
; the S_RTE_POP_PC ack -- but working_sp had only been bumped
; by +2 for the SR pop; the +4 PC-pop bump was sequential and
; hadn't fired yet, so A7 ended up at original+2 instead of
; original+6.  4 bytes of stack remained "live" after the RTE.
;
; Existing t12_supervisor RTEs from supervisor to USER mode,
; which takes the usp_shadow branch and bypasses the bug.
; This test stays supervisor->supervisor (manufactured frame
; with SR.S=1 in saved SR).
;
; Pass = halt 0.  Failures:
;   $BAD1 = A7 not advanced by 6 after sup->sup RTE
;   $BAD2 = sentinel below new A7 was corrupted

        .org $80                ; vector 32 = TRAP #0
        .long trap0_handler

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- Manufactured sup->sup RTE frame ---
        ;   stack (low addr -> high addr):
        ;     SR  (2 bytes)
        ;     PC  (4 bytes)
        ;   Build it by pushing PC first, then SR.
        ; Target PC = after_rte; SR = $2700 (supervisor, IPL=7, no flags)
        move.l  #after_rte, -(A7)        ; push PC
        move.w  #$2700, -(A7)            ; push SR (S=1 -> stay supervisor)
        ; Sentinel below the new A7 to catch over-pop.
        move.l  A7, A3                    ; A7 now points at SR word
        move.l  A3, D7                    ; remember pre-RTE A7
        addq.l  #6, D7                    ; expected post-RTE A7

        rte                               ; sup->sup; A7 should += 6

after_rte:
        move.l  A7, D0
        cmp.l   D7, D0
        bne     fail1

        ; --- Real exception path: TRAP #0 lands in supervisor;
        ;     its RTE returns to user-land code here in supervisor
        ;     mode (we start supervisor and never leave) ---
        move.l  A7, D6                    ; capture A7 before TRAP
        trap    #0
        move.l  A7, D0
        cmp.l   D6, D0                    ; A7 must be back where it was
        bne     fail2

        stop    #0

trap0_handler:
        ; Modify D7 to verify handler ran.
        move.l  #$DEAD0007, D7
        rte                               ; sup->sup; A7 must += 6

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
