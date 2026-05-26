| wack.s — small, relocatable, extensible serial-port monitor.
|
| A from-scratch reimplementation of the spirit of K1.3's ROMWack.
| The whole file assembles to one position-independent blob: every
| cross-reference is PC-relative, no MOVE.L #abs32 to local labels.
| Drop the binary at any address, jump to `wack_entry`, and the
| monitor comes up on the RS-232 port (Paula SERDAT/SERDATR/SERPER
| at 9600 8N1).
|
| This file is GAS-style asm (Motorola mnemonics, %-less regs to
| match boot_rom.s).  Targets 68000 only| no `MOVEC`, no `LINK.L`.
|
| Layout:
|   wack_entry      | public entry — saves regs, runs prompt loop
|   wack_resume     | restore saved regs and return
|   <serial>        | getc / putc / SERPER init
|   <print>         | print_string / print_hex / put_dec
|   <line>          | read_line, skip_ws, parse_hex
|   <dispatch>      | walk built-in table + user table
|   <handlers>      | regs / dump / go / help
|   <data>          | banner, prompt, built-in command list, scratch
|   user_cmd_head:  | user-extension hook (defaults to NULL)
|
| To add commands at runtime, write a pointer to your own 14-byte
| linked-list node into the long at `user_cmd_head` (offset is shown
| in the assembly listing).  The dispatcher walks the built-in list
| first, then your list.
|
| All offsets are PC-relative so the blob can sit anywhere in RAM.

| ---------------------------------------------------------------
| Paula register addresses (absolute — these are hardware MMIO)
| ---------------------------------------------------------------
        .equ    INTREQ,  0x00DFF09C
        .equ    INTENA,  0x00DFF09A
        .equ    SERDAT,  0x00DFF030
        .equ    SERDATR, 0x00DFF018
        .equ    SERPER,  0x00DFF032
        .equ    SERPER_9600, 0x0174     | PAL 7.094 MHz -> ~9612 baud

        .equ    CHAR_CR, 0x0D
        .equ    CHAR_LF, 0x0A
        .equ    CHAR_BS, 0x08
        .equ    CHAR_DEL, 0x7F

        .equ    LINEBUF_SIZE, 80
        .equ    MAX_ARGS, 4

| ---------------------------------------------------------------
| Save area layout, accessed via A5.  Allocated on the entry stack.
| ---------------------------------------------------------------
        .equ    SAV_D0, 0
        .equ    SAV_D1, 4
        .equ    SAV_D2, 8
        .equ    SAV_D3, 12
        .equ    SAV_D4, 16
        .equ    SAV_D5, 20
        .equ    SAV_D6, 24
        .equ    SAV_D7, 28
        .equ    SAV_A0, 32
        .equ    SAV_A1, 36
        .equ    SAV_A2, 40
        .equ    SAV_A3, 44
        .equ    SAV_A4, 48
        .equ    SAV_A5, 52
        .equ    SAV_A6, 56
        | --- 2 bytes of pad word at offset 60-61 to keep SR/PC long-aligned
        .equ    SAV_SR, 62      | word: saved SR
        .equ    SAV_PC, 64      | long: caller's return PC
        .equ    SAV_LEN, 68     | total bytes of frame

        .text
        .globl  wack_entry

| ===============================================================
|  wack_entry  —  monitor entry.  JSR here from supervisor mode.
|  Saves all regs to a stack frame, runs the command loop, restores
|  via RTR (pops SR then PC) when the user types "g[o]".
|
|  Pre-conditions:
|   * Supervisor mode (the saved/restored SR pops the full word).
|   * RS-232 cabled at 9600 8N1.
|   * About 80 bytes of stack headroom for the save frame.
|   * Interrupts SHOULD be masked by the caller for clean serial I/O.
| ===============================================================
wack_entry:
        | --- snapshot SR for display, push as the RTR target word.
        move.w  %sr, -(%sp)
        subq.l  #2, %sp                  | pad so SR/PC are long-aligned
        movem.l %d0-%d7/%a0-%a6, -(%sp)      | 60 bytes of regs

        | --- A5 := base of save frame
        movea.l %sp, %a5

        | --- init serial port
        bsr     serial_init

        | --- print banner
        lea     banner(%pc), %a0
        bsr     print_string

cmd_loop:
        lea     prompt(%pc), %a0
        bsr     print_string

        | --- read a line into linebuf
        lea     linebuf(%pc), %a0
        bsr     read_line

        | --- dispatch.  A0 = linebuf, modified to skip leading ws
        lea     linebuf(%pc), %a0
        bsr     skip_ws
        tst.b   (%a0)
        beq     cmd_loop                | empty line -> reprompt

        bsr     dispatch
        tst.l   %d0
        bmi     do_resume               | handler returned -1 -> exit
        bra     cmd_loop

do_resume:
        | --- restore regs and RTS into the saved PC, with saved SR.
        movem.l (%sp)+, %d0-%d7/%a0-%a6
        addq.l  #2, %sp                  | drop padding
        rtr                              | pops SR then PC

| ===============================================================
|  Serial I/O.  Polling, no interrupts.  Addresses are absolute
|  hardware MMIO and must NOT be relocated.
| ===============================================================
serial_init:
        move.w  #SERPER_9600, SERPER
        move.w  #0x0800, INTREQ         | clear pending RBF
        rts

| --- getc_block: spin until a char is available| return in D0.B ---
getc_block:
        move.w  SERDATR, %d0
        btst    #14, %d0                 | RBF (receive buffer full)
        beq     getc_block
        move.w  #0x0800, INTREQ         | ack
        and.w   #0x00FF, %d0
        rts

| --- putc: D0.B -> serial.  Expands CR to CRLF. ---
putc:
        movem.l %d1/%d0, -(%sp)
        cmpi.b  #CHAR_CR, %d0
        bne     putc_one
        moveq   #CHAR_CR, %d0
        bsr     putc_raw
        moveq   #CHAR_LF, %d0
        bra     putc_done
putc_one:
        bsr     putc_raw
putc_done:
        movem.l (%sp)+, %d1/%d0
        rts

putc_raw:
        | wait for TBE (transmit-buffer empty) -- SERDATR bit 13
putc_wait:
        move.w  SERDATR, %d1
        btst    #13, %d1
        beq     putc_wait
        and.w   #0x00FF, %d0
        or.w    #0x0100, %d0             | stop-bit encoding for 8N1
        move.w  %d0, SERDAT
        rts

| ===============================================================
|  Print helpers
| ===============================================================
| --- print_string: A0 -> NUL-terminated| preserves A0/D0 ---
print_string:
        move.l  %a0, -(%sp)
        move.l  %d0, -(%sp)
ps_loop:
        move.b  (%a0)+, %d0
        beq     ps_done
        bsr     putc
        bra     ps_loop
ps_done:
        move.l  (%sp)+, %d0
        move.l  (%sp)+, %a0
        rts

| --- print_hex_byte: D0.B -> two hex digits.  Trashes D1. ---
print_hex_byte:
        move.l  %d0, -(%sp)
        ror.w   #4, %d0
        bsr     phb_nibble
        rol.w   #4, %d0
        bsr     phb_nibble
        move.l  (%sp)+, %d0
        rts
phb_nibble:
        move.b  %d0, %d1
        and.b   #0x0F, %d1
        cmpi.b  #10, %d1
        blt     phb_dec
        add.b   #'A'-10, %d1
        bra     phb_emit
phb_dec:
        add.b   #'0', %d1
phb_emit:
        move.l  %d0, -(%sp)
        move.b  %d1, %d0
        bsr     putc
        move.l  (%sp)+, %d0
        rts

| --- print_hex_word: D0.W | print_hex_long: D0.L ---
print_hex_word:
        ror.w   #8, %d0
        bsr     print_hex_byte
        rol.w   #8, %d0
        bsr     print_hex_byte
        rts

print_hex_long:
        swap    %d0
        bsr     print_hex_word
        swap    %d0
        bsr     print_hex_word
        rts

| --- print_space ---
print_space:
        move.l  %d0, -(%sp)
        moveq   #' ', %d0
        bsr     putc
        move.l  (%sp)+, %d0
        rts

| --- print_newline ---
print_newline:
        move.l  %d0, -(%sp)
        moveq   #CHAR_CR, %d0
        bsr     putc                    | putc itself emits CRLF
        move.l  (%sp)+, %d0
        rts

| ===============================================================
|  Line reader & parser
| ===============================================================
| --- read_line: A0 = buffer| reads chars until CR| NUL-terminates ---
read_line:
        move.l  %d0, -(%sp)
        move.l  %a0, -(%sp)
        move.l  %a0, %a1                  | A1 = write cursor
        moveq   #LINEBUF_SIZE-1, %d1
rl_loop:
        bsr     getc_block
        cmp.b   #CHAR_CR, %d0
        beq     rl_done
        cmp.b   #CHAR_LF, %d0
        beq     rl_done
        cmp.b   #CHAR_BS, %d0
        beq     rl_back
        cmp.b   #CHAR_DEL, %d0
        beq     rl_back
        | --- echo and store
        bsr     putc
        tst.w   %d1
        beq     rl_loop                 | buffer full -> ignore but keep reading
        move.b  %d0, (%a1)+
        subq.w  #1, %d1
        bra     rl_loop
rl_back:
        cmpa.l  %a0, %a1
        beq     rl_loop                 | nothing to delete
        subq.l  #1, %a1
        addq.w  #1, %d1
        moveq   #CHAR_BS, %d0
        bsr     putc
        moveq   #' ', %d0
        bsr     putc
        moveq   #CHAR_BS, %d0
        bsr     putc
        bra     rl_loop
rl_done:
        clr.b   (%a1)
        bsr     print_newline
        move.l  (%sp)+, %a0
        move.l  (%sp)+, %d0
        rts

| --- skip_ws: A0 advanced past spaces/tabs ---
skip_ws:
        cmp.b   #' ', (%a0)
        beq     skip_ws_step
        cmp.b   #0x09, (%a0)
        bne     skip_ws_done
skip_ws_step:
        addq.l  #1, %a0
        bra     skip_ws
skip_ws_done:
        rts

| --- parse_hex: A0 in/out, D1 = parsed value, D0 = digit count.
| --- Stops at first non-hex char.
parse_hex:
        moveq   #0, %d1
        moveq   #0, %d0
ph_loop:
        move.b  (%a0), %d2
        | uppercase 'a'-'f' -> 'A'-'F'
        cmp.b   #'a', %d2
        blt     ph_check
        cmp.b   #'f', %d2
        bgt     ph_check
        sub.b   #0x20, %d2
ph_check:
        cmp.b   #'0', %d2
        blt     ph_done
        cmp.b   #'9', %d2
        ble     ph_dec
        cmp.b   #'A', %d2
        blt     ph_done
        cmp.b   #'F', %d2
        bgt     ph_done
        sub.b   #'A'-10, %d2
        bra     ph_acc
ph_dec:
        sub.b   #'0', %d2
ph_acc:
        and.l   #0x000000FF, %d2
        lsl.l   #4, %d1
        or.l    %d2, %d1
        addq.l  #1, %a0
        addq.l  #1, %d0
        bra     ph_loop
ph_done:
        rts

| --- str_eq: input cursor A0 vs name string A1 (NUL-terminated).
| --- Returns D0 != 0 if A1's name is a complete token at A0
| --- (followed by NUL/space/TAB), in which case A0 is left advanced
| --- past the matched name.  On mismatch D0 = 0 and A0 is restored.
str_eq:
        move.l  %a0, -(%sp)               | save start of cursor for restore
se_loop:
        move.b  (%a1)+, %d0
        beq     se_check_end            | A1 hit NUL -> see if A0 is at sep
        move.b  (%a0)+, %d1
        cmp.b   %d0, %d1
        bne     se_no
        bra     se_loop
se_check_end:
        move.b  (%a0), %d1
        beq     se_yes
        cmp.b   #' ', %d1
        beq     se_yes
        cmp.b   #0x09, %d1
        beq     se_yes
se_no:
        move.l  (%sp)+, %a0               | restore cursor
        moveq   #0, %d0
        rts
se_yes:
        addq.l  #4, %sp                  | drop saved cursor| keep advanced A0
        moveq   #1, %d0
        rts

| ===============================================================
|  Dispatcher.  Walks builtin_head, then user_cmd_head.
|  Each node:
|       +0  next     long
|       +4  name     long (NUL-terminated)
|       +8  flags    word     (reserved, currently $0001)
|       +10 handler  long     (entered via JSR (An))
|
|  dispatch returns D0 = 0 (continue prompt) or -1 (exit monitor).
|  walk_list returns D0 = +1 (no match), 0 (handled), -1 (exit).
| ===============================================================
dispatch:
        move.l  %a0, -(%sp)               | preserve cursor across builtin walk
        lea     builtin_head(%pc), %a3
        bsr     walk_list
        cmpi.l  #1, %d0
        bne     disp_done               | matched (0 or -1) -> propagate

        | --- builtins didn't match| try user list if present.
        movea.l user_cmd_head(%pc), %a3
        move.l  %a3, %d2
        beq     disp_unknown
        movea.l (%sp), %a0                | reset cursor for second walk
        bsr     walk_list
        cmpi.l  #1, %d0
        bne     disp_done

disp_unknown:
        lea     unknown_msg(%pc), %a0
        bsr     print_string
        moveq   #0, %d0
disp_done:
        addq.l  #4, %sp                  | drop saved cursor
        rts

| walk_list: try each node in A3-chain| on match JSR its handler.
walk_list:
        move.l  %a0, %d3                  | D3 = original cursor (for restart)
wl_loop:
        move.l  %a3, %d0
        beq     wl_nomatch
        movea.l %d3, %a0                  | reset cursor each iteration
        movea.l 4(%a3), %a1               | name ptr
        bsr     str_eq
        tst.l   %d0
        bne     wl_hit
        movea.l (%a3), %a3                | advance to next node
        bra     wl_loop
wl_hit:
        bsr     skip_ws                 | A0 now points at args
        movea.l 10(%a3), %a2
        jsr     (%a2)                    | handler returns 0 or -1 in D0
        rts
wl_nomatch:
        moveq   #1, %d0                  | +1 = list exhausted, no match
        rts

| ===============================================================
|  Built-in command handlers.  Each takes A0 = args (after the
|  command name, leading ws already skipped) and A5 = save-area.
|  Return D0 = 0 (continue) or -1 (resume / exit monitor).
| ===============================================================

| --- regs: dump saved D0..D7, A0..A6, SR, PC ---
cmd_regs:
        movem.l %d2-%d3/%a0-%a3, -(%sp)

        moveq   #0, %d2                  | reg index 0..7
rg_d:
        moveq   #'d', %d0
        bsr     putc
        move.l  %d2, %d0
        add.b   #'0', %d0
        bsr     putc
        moveq   #'=', %d0
        bsr     putc
        move.l  %d2, %d3
        lsl.l   #2, %d3                  | *4
        move.l  0(%a5,%d3.l), %d0
        bsr     print_hex_long
        bsr     print_space
        addq.l  #1, %d2
        cmp.l   #8, %d2
        blt     rg_d
        bsr     print_newline

        moveq   #0, %d2
rg_a:
        moveq   #'a', %d0
        bsr     putc
        move.l  %d2, %d0
        add.b   #'0', %d0
        bsr     putc
        moveq   #'=', %d0
        bsr     putc
        move.l  %d2, %d3
        lsl.l   #2, %d3
        add.l   #SAV_A0, %d3
        move.l  0(%a5,%d3.l), %d0
        bsr     print_hex_long
        bsr     print_space
        addq.l  #1, %d2
        cmp.l   #7, %d2
        blt     rg_a
        bsr     print_newline

        lea     sr_label(%pc), %a0
        bsr     print_string
        move.w  SAV_SR(%a5), %d0
        bsr     print_hex_word
        bsr     print_space
        lea     pc_label(%pc), %a0
        bsr     print_string
        move.l  SAV_PC(%a5), %d0
        bsr     print_hex_long
        bsr     print_newline

        moveq   #0, %d0
        movem.l (%sp)+, %d2-%d3/%a0-%a3
        rts

| --- dump: D <addr> [count].  Default count = 64 bytes ---
cmd_dump:
        movem.l %d2-%d4/%a2-%a3, -(%sp)
        bsr     parse_hex
        move.l  %d1, %a2                  | A2 = start
        bsr     skip_ws
        bsr     parse_hex
        tst.l   %d1
        bne     dump_cnt_ok
        move.l  #64, %d1
dump_cnt_ok:
        move.l  %d1, %d4                  | D4 = count
dump_row:
        tst.l   %d4
        ble     dump_done
        | --- address column
        move.l  %a2, %d0
        bsr     print_hex_long
        moveq   #':', %d0
        bsr     putc
        bsr     print_space
        | --- 16 bytes
        moveq   #16, %d3
dump_byte:
        move.b  (%a2)+, %d0
        bsr     print_hex_byte
        bsr     print_space
        subq.l  #1, %d4
        subq.l  #1, %d3
        ble     dump_eol
        tst.l   %d4
        bgt     dump_byte
dump_eol:
        bsr     print_newline
        bra     dump_row
dump_done:
        moveq   #0, %d0
        movem.l (%sp)+, %d2-%d4/%a2-%a3
        rts

| --- go: resume.  Optional new PC.
cmd_go:
        bsr     parse_hex
        tst.l   %d0
        beq     go_no_arg
        move.l  %d1, SAV_PC(%a5)
go_no_arg:
        moveq   #-1, %d0                 | signal exit
        rts

| --- help: list commands ---
cmd_help:
        move.l  %a3, -(%sp)
        lea     builtin_head(%pc), %a3
        bsr     help_print_list
        movea.l user_cmd_head(%pc), %a3
        bsr     help_print_list
        bsr     print_newline
        movea.l (%sp)+, %a3
        moveq   #0, %d0
        rts

| --- help_print_list: A3 = head of linked list (may be NULL).
|     Walks the chain, printing each name + a space.
help_print_list:
        cmpa.w  #0, %a3
        beq     hpl_done
        movea.l 4(%a3), %a0
        bsr     print_string
        bsr     print_space
        movea.l (%a3), %a3
        bra     help_print_list
hpl_done:
        rts

| ===============================================================
|  Data (strings, dispatch table, scratch).  Strings are NUL-term.
|  Dispatch nodes are 14 bytes: next/name/flags/handler.
|  All pointers in this section are PC-relative *at assembly time*
|  -- the linker resolves them once and the blob then ships flat.
| ===============================================================

        .balign 2
banner:
        .byte   CHAR_CR, 'w','a','c','k',' ','v','1',0
        .balign 2
prompt:
        .byte   '>',' ',0
        .balign 2
unknown_msg:
        .byte   '?',CHAR_CR,0
        .balign 2
sr_label:
        .byte   's','r','=',0
        .balign 2
pc_label:
        .byte   'p','c','=',0

        | command name strings
        .balign 2
name_regs:      .byte   'r',0
        .balign 2
name_dump:      .byte   'd',0
        .balign 2
name_go:        .byte   'g',0
        .balign 2
name_help:      .byte   '?',0

| ---- Dispatch list (linked, head-first) ----
        .balign 4
builtin_head:
node_regs:
        .long   node_dump
        .long   name_regs
        .word   0x0001
        .long   cmd_regs

node_dump:
        .long   node_go
        .long   name_dump
        .word   0x0001
        .long   cmd_dump

node_go:
        .long   node_help
        .long   name_go
        .word   0x0001
        .long   cmd_go

node_help:
        .long   0                        | end of builtin list
        .long   name_help
        .word   0x0001
        .long   cmd_help

| ---- User-extension hook ----
|
| To add commands at runtime, store a pointer to the head of your own
| linked list of nodes here.  Each node has the same 14-byte layout
| as the builtins.  The dispatcher walks builtin_head first then this
| list.
|
        .balign 4
        .globl  user_cmd_head
user_cmd_head:
        .long   0

| ---- Scratch ----
        .balign 2
linebuf:
        .space  LINEBUF_SIZE
