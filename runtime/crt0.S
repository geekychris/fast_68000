/* crt0.S — fast_68000 multi-core C runtime entry.
 *
 * Hardware resets every core to PC=$400 and SP=$4000+core_id*$1000
 * (see MULTICORE.md).  Core 0 zeros .bss then publishes a ready flag
 * the other cores spin on, after which everyone calls main(core_id).
 * On return from main the core does `stop #0` (success).  To halt
 * with a different code (a test failure) call `STOP_WITH(0xBAD1)`
 * from C — the macro emits a literal `stop #imm` since `stop`'s
 * immediate is encoded in the instruction word.
 */

    .equ    BSS_READY_ADDR, 0x00003FF0

    .text
    .globl  _start
_start:
    /* d7 := core_id = (sp - 0x4000) >> 12 */
    move.l  %sp, %d7
    sub.l   #0x4000, %d7
    moveq   #12, %d6
    lsr.l   %d6, %d7

    tst.l   %d7
    bne     wait_bss_ready

    /* core 0: zero .bss */
    lea     __bss_start, %a0
    lea     __bss_end, %a1
    move.l  %a1, %d0
    sub.l   %a0, %d0
    beq     bss_done
    moveq   #0, %d1
bss_loop:
    move.b  %d1, (%a0)+
    subq.l  #1, %d0
    bne     bss_loop
bss_done:
    move.l  #1, BSS_READY_ADDR
    bra     call_main

wait_bss_ready:
    tst.l   BSS_READY_ADDR
    beq     wait_bss_ready

call_main:
    /* SysV m68k ABI: arg on stack. */
    move.l  %d7, -(%sp)
    jsr     main
    addq.l  #4, %sp

    stop    #0
    bra     .-2
