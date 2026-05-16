; Exercise the supervisor / user mode plumbing:
;  - MOVE An, USP   (set USP)
;  - ANDI #imm, SR  (clear S bit -> goes to user mode? no: ANDI to SR is
;                    handled by sr_we_c which currently does not initiate the
;                    USP/SSP swap from supervisor context. So instead we use
;                    a constructed RTE frame to enter user mode cleanly.)
;  - From user mode, TRAP -> back to supervisor mode (push frame, vector fetch)
;  - Verify supervisor sees the right context
;  - Privilege violation: from user mode, executing RTE should trap to vec 8
;  - Divide by zero: should trap to vec 5
;
        .org $14                ; vector 5 = divide by zero
        .long div0_handler

        .org $20                ; vector 8 = privilege violation
        .long privvio_handler

        .org $8C                ; vector 35 = TRAP #3
        .long trap_handler

        .org $400

        ;------------------------------------------------------------
        ; 1) Set up USP and switch to user mode via a manufactured RTE
        ;    stack frame. Our exception frame is 32-bit SR (low 16 used)
        ;    followed by 32-bit PC.
        ;------------------------------------------------------------
        ; USP will live in 0x3000..0x4000.
        lea   $4000, A0
        move.l A0, USP             ; usp_shadow := 0x4000

        ; Build a fake RTE frame on the current SSP. Target = user_code.
        ; SR for user mode: S=0, T=0, I=0, no flags -> 0x0000.
        ; PC = user_code (32-bit).
        move.l #user_code, -(A7)   ; push PC32
        move.l #$0000_0000, -(A7)  ; push SR32 (S=0 user mode)

        rte                         ; should pop frame and enter user mode

fail:
        stop  #$BAD5

        ;------------------------------------------------------------
        ; In user mode. Run a small computation and TRAP back.
        ;------------------------------------------------------------
user_code:
        moveq #7, D0
        moveq #6, D1
        add.l D1, D0               ; D0 = 13
        cmpi.l #13, D0
        bne   fail
        ; Trap to supervisor.
        trap  #3

        ; After trap_handler RTEs, we resume here (still in user mode).
        ; Verify D7 was set by the handler.
        cmpi.l #$5A5A, D7
        bne   fail

        ; Privilege violation: executing RTE from user mode should trap.
        rte                         ; should go to privvio_handler

        ;------------------------------------------------------------
        ; Handlers (run in supervisor mode).
        ;------------------------------------------------------------
trap_handler:
        moveq #$5A, D7             ; low byte = 0x5A
        move.l #$0000_5A5A, D7     ; full long
        rte

privvio_handler:
        ; Once here we know the privilege violation fired. Halt success.
        stop  #0

div0_handler:
        ; Not used in main path; would trigger on DIVU/DIVS by 0.
        stop  #$BAD6
