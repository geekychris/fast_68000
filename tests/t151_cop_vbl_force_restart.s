; t151 -- Copper auto-restart at VBL must NOT be gated on !cop_busy.
;
; K1.3's boot Copper at chip-RAM $0420 ends with MOVE COPJMP2=0, which
; jumps to whatever cop2lc currently holds.  The K1.3 VBL handler at
; $FC6D6C writes COP2LC = LOFCprList every frame, but the first write
; lands as COP2LC=0 (the LOFCprList field is uninitialised this early
; in boot).  Our Copper then walks chip-RAM zeros from $0 forever and
; cop_busy never clears -- without force-restart, no subsequent VBL
; can resync the Copper to cop1lc=$420 even after the CPU has written
; a valid Workbench Copper list at LOFCprList.
;
; This test reproduces the failure in isolation:
;   - boot Copper at $500 contains a single MOVE COPJMP2=0 + WAIT-
;     forever sentinel, so the Copper jumps to cop2lc and *stalls*
;     forever (cop_busy stuck high).
;   - cop2lc points at a do-nothing list that does NOT halt; the
;     Copper just walks zeros (mst_req keeps cycling but no useful
;     MOVEs happen).
;   - We sleep two VBL periods.
;   - We reprogram COP1LC to a *new* list that writes COLOR00=$0F00
;     and ends.
;   - With the gate `!cop_busy` blocking the next VBL kick, the new
;     list never runs and COLOR00 stays at its pre-staged value.
;   - With the force-restart, the next VBL reloads PC from cop1lc and
;     the new list runs.
;
; Pass = halt 0.  Fail = stop $C161 (COLOR00 didn't get $0F00).

        .org $400

start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Pre-stage COLOR00 with a sentinel.
        move.l  #$01800000, $00DFF180        ; long write puts $0000 in
                                              ; the COLOR00 slot

        ; cop2lc target at $1000: just a WAIT-forever end-of-list.
        ; A real runaway would walk garbage zeros; the WAIT-end is
        ; equivalent for the test (Copper sets cop_busy and parks).
        move.l  #$FFFFFFFE, $00001000

        ; "Boot" Copper at $500: jump to cop2lc and stay parked.
        move.l  #$008A0000, $00000500        ; MOVE COPJMP2=0
        move.l  #$FFFFFFFE, $00000504        ; defensive end-of-list

        ; Program COP2LC and COP1LC + enable DMACON.COPEN+DMAEN.
        move.l  #$00001000, $00DFF084        ; COP2LC = parked list
        move.l  #$00000500, $00DFF080        ; COP1LC = boot list
        move.w  #$8280,     $00DFF096        ; SET | DMAEN | COPEN

        ; Wait long enough for at least one VBL auto-kick + run + park.
        move.l  #$00020000, D0
spin1:
        subq.l  #1, D0
        bne     spin1

        ; Now swap COP1LC to a list that writes COLOR00=$0F00.
        move.l  #$01800F00, $00006000
        move.l  #$FFFFFFFE, $00006004
        move.l  #$00006000, $00DFF080        ; COP1LC = new list

        ; Wait long enough for at least one more VBL.
        move.l  #$00020000, D0
spin2:
        subq.l  #1, D0
        bne     spin2

        ; Verify the new COP1LC list ran -- COLOR00 should now be $0F00.
        move.l  $00DFF180, D1
        andi.l  #$FFFF0000, D1
        cmpi.l  #$0F000000, D1
        bne     fail_restart

        stop    #0

fail_restart:  stop #$C161
