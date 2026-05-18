; Stack frame manipulation: LINK / UNLK / JSR / RTS / BSR / PEA.
; AmigaDOS-style: nested function calls with frame pointer in A6 (or A5).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Test 1: simple JSR / RTS ----
        moveq   #0, D7
        jsr     fn_add42
        cmpi.l  #42, D7
        bne     fail_1

        ; ---- Test 2: BSR (relative branch to subroutine) ----
        moveq   #0, D7
        bsr     fn_add42
        cmpi.l  #42, D7
        bne     fail_2

        ; ---- Test 3: nested JSR/JSR/RTS/RTS ----
        moveq   #0, D7
        jsr     fn_outer
        cmpi.l  #100, D7
        bne     fail_3

        ; ---- Test 4: LINK / UNLK ----
        ; Save SP, call fn_link_test, verify SP restored.
        move.l  A7, D6
        jsr     fn_link_test
        cmpi.l  #$DEAD0001, D7
        bne     fail_4
        move.l  A7, D5
        cmp.l   D6, D5
        bne     fail_4_sp     ; SP should be back to pre-call value

        ; ---- Test 5: PEA pushes effective address ----
        move.l  A7, D6
        pea     $0001ABCD       ; push absolute address as a long
        move.l  (A7)+, D0
        cmpi.l  #$0001ABCD, D0
        bne     fail_5
        cmp.l   D6, A7
        bne     fail_5_sp

        ; ---- Test 6: deep recursion (countdown via stack) ----
        moveq   #5, D7
        jsr     fn_countdown
        cmpi.l  #0, D7
        bne     fail_6

        stop    #0

; ============================================================
; Subroutines
; ============================================================
fn_add42:
        addi.l  #42, D7
        rts

fn_outer:
        jsr     fn_inner
        addi.l  #50, D7
        rts

fn_inner:
        moveq   #50, D7
        rts

; LINK A5, #-8: save A5, A5 <- SP-4, SP <- A5-8.
; Allocates 8 bytes of locals.  Writes a sentinel into them and reads back.
fn_link_test:
        link    A5, #-8
        ; A5 now points to the saved A5 in the frame; locals at -8(A5)..-1(A5).
        move.l  #$DEAD0001, D7      ; return value placeholder
        move.l  #$F00DCAFE, -8(A5)
        ; Read back to make sure -8(A5) addressing works.
        move.l  -8(A5), D0
        cmpi.l  #$F00DCAFE, D0
        bne     fail_link_local
        unlk    A5
        rts
fail_link_local:
        move.l  #$BAD, D7
        unlk    A5
        rts

; Decrement D7 by 1 and call again if D7 > 0.  Stops at D7==0.
fn_countdown:
        cmpi.l  #0, D7
        beq     cd_done
        subq.l  #1, D7
        jsr     fn_countdown
cd_done:
        rts

fail_1:     stop #$D501
fail_2:     stop #$D502
fail_3:     stop #$D503
fail_4:     stop #$D504
fail_4_sp:  stop #$D514
fail_5:     stop #$D505
fail_5_sp:  stop #$D515
fail_6:     stop #$D506
