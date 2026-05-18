; Exception dispatch: TRAP #N, TRAPV, CHK, divide-by-zero, illegal,
; and the vectored return via RTE.
;
; The 68000's exception model is what makes AmigaOS syscalls work:
; A6 -= ExecBase offset, JSR (A6) for LVO, or trap #N for fast path.

        .org $400

vectors:
        .org $80
        .long trap0_handler     ; vector 32 = TRAP #0
        .org $84
        .long trap1_handler     ; vector 33 = TRAP #1
        .org $14
        .long div0_handler      ; vector 5 = divide by zero
        .org $18
        .long chk_handler       ; vector 6 = CHK
        .org $1C
        .long trapv_handler     ; vector 7 = TRAPV
        .org $10
        .long illegal_handler   ; vector 4 = illegal instruction

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Test 1: TRAP #0 dispatches and returns ----
        moveq   #0, D7
        trap    #0
        cmpi.l  #$11, D7
        bne     fail_1

        ; ---- Test 2: TRAP #1 dispatches to a different vector ----
        moveq   #0, D7
        trap    #1
        cmpi.l  #$22, D7
        bne     fail_2

        ; ---- Test 3: Divide by zero exception ----
        moveq   #0, D7
        move.w  #1, D1
        move.w  #100, D2
        divu.w  D1, D2          ; D1 = divisor = 1; D2 unchanged on success
        ; Now force div-by-zero:
        moveq   #0, D7
        move.w  #0, D1
        move.w  #100, D2
        divu.w  D1, D2          ; should fire VEC_DIV_ZERO
        cmpi.l  #$55, D7
        bne     fail_3

        ; ---- Test 4: TRAPV with V flag set ----
        moveq   #0, D7
        ; Force overflow then trapv.
        move.l  #$7FFFFFFF, D0
        add.l   #1, D0          ; produces $80000000, V=1
        trapv
        cmpi.l  #$88, D7
        bne     fail_4

        ; ---- Test 5: TRAPV with V=0 does NOT trap ----
        moveq   #0, D7
        move.l  #0, D0
        add.l   #1, D0          ; V=0
        trapv                   ; should fall through
        cmpi.l  #0, D7
        bne     fail_5

        stop    #0

; ============================================================
; Handlers
; ============================================================
trap0_handler:
        move.l  #$11, D7
        rte

trap1_handler:
        move.l  #$22, D7
        rte

div0_handler:
        move.l  #$55, D7
        rte

trapv_handler:
        move.l  #$88, D7
        rte

chk_handler:
        move.l  #$CC, D7
        rte

illegal_handler:
        move.l  #$AD, D7
        rte

fail_1: stop #$D601
fail_2: stop #$D602
fail_3: stop #$D603
fail_4: stop #$D604
fail_5: stop #$D605
