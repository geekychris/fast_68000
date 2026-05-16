; OS demo: a tiny cooperative kernel running three tasks that each animate
; one horizontal stripe of the framebuffer.
;
; Layout (single core, supervisor-mode-only):
;   $0000_0400  code (this file)
;   $0000_4000  core 0 reset stack (kernel)         -> bottom $0000_3FFF
;   $0000_5000  task 0 stack top                    -> bottom $0000_4FFF
;   $0000_6000  task 1 stack top                    -> bottom $0000_5FFF
;   $0000_7000  task 2 stack top                    -> bottom $0000_6FFF
;   $0000_8000  TCB table                           -> 4 entries x 16 bytes
;   $0000_8200  os_curr_task (long, 0..N-1)
;   $0000_8204  os_ntasks (long)
;   $0000_8208  shared frame counter
;   $0000_820C  shared mutex byte
;   $0001_0000  framebuffer (256x192, 1 byte/pixel)
;
; TCB layout per slot (16 bytes):
;   +0: sp     (saved A7 of the task, with return PC on top)
;   +4: state  (0 = free, 1 = ready)
;   +8: reserved
;   +12: reserved
;
; Kernel contract: os_yield clobbers all registers (D0-D7, A0-A6, and A7 by
; design). Tasks must keep persistent state in memory, not in registers,
; across yields. SR is preserved by virtue of staying in supervisor mode
; throughout (no MOVE-to-SR done by the OS).

        .org $400

;------------------------------------------------------------------
; Reset entry
;------------------------------------------------------------------
reset:
        ; Initialise OS state.
        bsr     os_init

        ; Create three tasks.
        moveq   #0, D0
        move.l  #task0, A0
        move.l  #$00005000, A1
        bsr     os_task_create

        moveq   #1, D0
        move.l  #task1, A0
        move.l  #$00006000, A1
        bsr     os_task_create

        moveq   #2, D0
        move.l  #task2, A0
        move.l  #$00007000, A1
        bsr     os_task_create

        ; Hand control over to the scheduler. Never returns.
        bsr     os_start
        stop    #$dead          ; should never reach here

;------------------------------------------------------------------
; os_init: clear TCBs and globals.
;------------------------------------------------------------------
os_init:
        moveq   #0, D0
        move.l  D0, $00008200            ; curr = 0
        move.l  D0, $00008204            ; ntasks = 0
        move.l  D0, $00008208            ; frame counter
        move.l  D0, $0000820C            ; mutex

        ; Zero TCBs 0..3 (16 bytes each = 64 bytes total).
        move.l  #$00008000, A0
        moveq   #15, D1                  ; 16 longs (= 64 bytes)
init_lp:
        move.l  D0, (A0)+
        dbra    D1, init_lp
        rts

;------------------------------------------------------------------
; os_task_create: D0 = idx, A0 = entry PC, A1 = stack top (one past usable).
; Prepares the task's stack so a future yield can RTS into it.
;------------------------------------------------------------------
os_task_create:
        ; Push entry PC onto task's stack: SP -= 4; *SP = entry.
        suba.l  #4, A1
        move.l  A0, (A1)

        ; tcb = $8000 + idx*16
        move.l  D0, D1
        lsl.l   #4, D1
        move.l  #$00008000, A2
        adda.l  D1, A2
        move.l  A1, (A2)                 ; tcb.sp = prepared SP
        move.l  #1, 4(A2)                ; tcb.state = READY

        ; ntasks = max(ntasks, idx+1)
        move.l  $00008204, D2
        addq.l  #1, D0
        cmp.l   D0, D2
        bcc     no_bump                  ; D2 >= D0+1, no bump
        move.l  D0, $00008204
no_bump:
        rts

;------------------------------------------------------------------
; os_start: jump to task 0 by switching to its prepared stack.
;------------------------------------------------------------------
os_start:
        moveq   #0, D0
        move.l  D0, $00008200            ; curr = 0
        move.l  $00008000, A7            ; SP = tcb0.sp (entry PC on top)
        rts                              ; jump to task 0 entry

;------------------------------------------------------------------
; os_yield: save current task's SP; pick next ready task; restore its SP; RTS.
; All caller-saved registers (D0-D7, A0-A6) are clobbered. The task's
; return PC is implicitly on its stack already.
;------------------------------------------------------------------
os_yield:
        ; A0 = current TCB; tcb[curr].sp = A7
        move.l  $00008200, D0
        move.l  D0, D1
        lsl.l   #4, D1
        move.l  #$00008000, A0
        adda.l  D1, A0
        move.l  A7, (A0)                 ; save SP

        move.l  $00008204, D2            ; D2 = ntasks
pick_lp:
        addq.l  #1, D0
        cmp.l   D2, D0
        blt     pick_in
        moveq   #0, D0
pick_in:
        move.l  D0, D1
        lsl.l   #4, D1
        move.l  #$00008000, A0
        adda.l  D1, A0
        move.l  4(A0), D3
        cmp.l   #1, D3
        bne     pick_lp                  ; not ready, keep searching

        move.l  D0, $00008200            ; curr = new
        move.l  (A0), A7                 ; SP = tcb[new].sp
        rts                              ; jump to new task's saved PC

;------------------------------------------------------------------
; Shared mutex around the global frame counter, just to exercise TAS.
;------------------------------------------------------------------
mutex_lock:
        move.l  #$0000820C, A0
mlock_spin:
        tas     (A0)
        beq     mlock_done
        bsr     os_yield                 ; let other tasks run while we wait
        bra     mlock_spin
mlock_done:
        rts

mutex_unlock:
        moveq   #0, D0
        move.b  D0, $0000820C
        rts

;------------------------------------------------------------------
; Task 0: top stripe rows 0..63. Animated horizontal gradient.
;
; Each pixel: byte = ((x + frame) ^ y_in_stripe) ; with high bits dominated
; by the R channel so the stripe is reddish.
;------------------------------------------------------------------
task0:
t0_loop:
        ; bump global frame counter under mutex (purely to exercise TAS)
        bsr     mutex_lock
        move.l  $00008208, D7
        addq.l  #1, D7
        move.l  D7, $00008208
        bsr     mutex_unlock

        ; draw rows 0..63 using local frame counter (D7 = global counter)
        ; pixel address = $00010000 + y*256 + x
        moveq   #0, D1                   ; y in [0..63]
t0_yloop:
        moveq   #0, D2                   ; x
        ; row base = $00010000 + (y<<8)
        move.l  D1, D5
        lsl.l   #8, D5
        move.l  #$00010000, A0
        adda.l  D5, A0
t0_xloop:
        move.l  D2, D3
        add.l   D7, D3
        eor.l   D1, D3
        ori.l   #$E0, D3                 ; force R bits high -> red tint
        move.b  D3, (A0)+
        addq.l  #1, D2
        cmpi.l  #256, D2
        bne     t0_xloop
        addq.l  #1, D1
        cmpi.l  #64, D1
        bne     t0_yloop

        ; yield to let other tasks animate
        bsr     os_yield
        bra     t0_loop

;------------------------------------------------------------------
; Task 1: middle stripe rows 64..127. Moving vertical bars (greenish).
;------------------------------------------------------------------
task1:
t1_loop:
        ; local frame counter persisted in $8210
        move.l  $00008210, D7
        addq.l  #1, D7
        move.l  D7, $00008210

        moveq   #64, D1                  ; y in [64..127]
t1_yloop:
        moveq   #0, D2
        move.l  D1, D5
        lsl.l   #8, D5
        move.l  #$00010000, A0
        adda.l  D5, A0
t1_xloop:
        ; column phase: ((x + frame*2) >> 3) & 7
        move.l  D2, D3
        move.l  D7, D4
        add.l   D4, D3
        add.l   D4, D3                   ; +2*frame
        lsr.l   #3, D3
        andi.l  #7, D3
        ; bake into G bits: shift left 2, OR into byte
        lsl.l   #2, D3                   ; G channel position (bits 4:2)
        move.b  D3, (A0)+
        addq.l  #1, D2
        cmpi.l  #256, D2
        bne     t1_xloop
        addq.l  #1, D1
        cmpi.l  #128, D1
        bne     t1_yloop

        bsr     os_yield
        bra     t1_loop

;------------------------------------------------------------------
; Task 2: bottom stripe rows 128..191. Bouncing diagonal stripes (blueish).
;------------------------------------------------------------------
task2:
t2_loop:
        ; local frame counter at $8214
        move.l  $00008214, D7
        addq.l  #1, D7
        move.l  D7, $00008214

        move.l  #128, D1                 ; y in [128..191]
t2_yloop:
        moveq   #0, D2
        move.l  D1, D5
        lsl.l   #8, D5
        move.l  #$00010000, A0
        adda.l  D5, A0
t2_xloop:
        ; diagonal: ((x + y + frame) >> 2) & 3 -> B channel (bits 1:0)
        move.l  D2, D3
        add.l   D1, D3
        add.l   D7, D3
        lsr.l   #2, D3
        andi.l  #3, D3
        move.b  D3, (A0)+
        addq.l  #1, D2
        cmpi.l  #256, D2
        bne     t2_xloop
        addq.l  #1, D1
        cmpi.l  #192, D1
        bne     t2_yloop

        bsr     os_yield
        bra     t2_loop
