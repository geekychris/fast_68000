; Exercise TRAP / RTE.
;
; Layout: we install a handler for TRAP #3 (vector 35, byte offset 35*4 = 140
; = $8C). At reset we boot at $400 with SSP at $4000 (set by m68k_top harness).
;
; The handler will write a sentinel into D7 and then RTE. The main code TRAPs,
; verifies D7 was set, and stops.
;
        .org $8C                ; vector 35 = TRAP #3
        .long handler

        .org $400
        moveq #0, D7

        ; Install handler -- not needed; .long above already populated $8C.
        ; (kept here for clarity; the .org block writes the vector at link time)

        trap  #3                ; should push PC+SR, jump to handler

        ; After RTE we resume here. D7 should be $42.
        cmpi.l #$42, D7
        bne   fail

        ; A second TRAP returns again.
        moveq #0, D7
        trap  #3
        cmpi.l #$42, D7
        bne   fail

        stop  #0

handler:
        moveq #$42, D7
        rte

fail:
        stop  #$BAD4
