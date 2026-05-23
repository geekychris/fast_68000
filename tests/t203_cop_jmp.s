; t203 -- canonical Copper internal-jump test (MOVE -> COPJMP2).
;
; list 1 at $00050000:
;   MOVE  $DFF180 (COLOR00) <- $0AAA       ; ($01800AAA)
;   MOVE  $DFF08A (COPJMP2) <- $0000       ; ($008A0000) -- restarts at COP2LC
;
; list 2 at $00050100:
;   MOVE  $DFF184 (next COLOR slot) <- $0BBB ; ($01840BBB)
;   end-of-list                              ; ($FFFFFFFE)
;
; The MOVE to COPJMP2 in list 1 reloads PC from cop2lc internally
; (no bus master cycle) and resumes fetch at list 2.  Both colors
; should end up set in Denise.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; --- build list 1 ---
        move.l #$01800AAA, $00050000     ; MOVE COLOR00 <- $0AAA
        move.l #$008A0000, $00050004     ; MOVE COPJMP2 <- 0 (jump to list 2)

        ; --- build list 2 ---
        move.l #$01840BBB, $00050100     ; MOVE COLOR@$184 <- $0BBB
        move.l #$FFFFFFFE, $00050104     ; end-of-list

        ; --- program COP2LC first so the COPJMP2 reload sees the right addr.
        move.l #$00050100, $00FE004C     ; COP2LC

        ; --- program COP1LC and strobe COPJMP1 ---
        move.l #$00050000, $00FE0040
        move.l #1,         $00FE0044

wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

        ; --- verify both colors set ---
        move.l $00DFF180, D1
        cmpi.l #$0AAA0000, D1
        bne    fail1
        move.l $00DFF184, D2
        cmpi.l #$0BBB0000, D2
        bne    fail2

        stop   #0

fail1:  stop   #$c231
fail2:  stop   #$c232
