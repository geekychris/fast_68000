; OR.L Dn, (An) memory-destination is exec.library Signal()'s critical
; instruction at \$FC1E96.  Signal does:
;   LEA tc_SigRecvd(A1), A0       ; A0 = &task.tc_SigRecvd
;   OR.L D0, (A0)                 ; tc_SigRecvd |= mask
;
; This test verifies OR.L Dn,(An) sets the right bits in memory, both
; for chip-RAM and for slow-RAM (Agnus trapdoor) addresses.  Slow-RAM
; coverage matters because all task structs (incl. tc_SigRecvd) live
; in slow RAM after commit 26f2f86.
;
; Pass = halt 0.  Fail = stop \$BADn.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. Chip RAM: OR.L D0, (A0) ---
        lea     $2000, A0
        move.l  #$00FF00FF, (A0)
        move.l  #$AB00CD00, D0
        .word   $8190                ; OR.L D0, (A0)
        move.l  (A0), D1
        cmpi.l  #$ABFFCDFF, D1
        bne     fail_chip_or

        ; --- 2. Slow RAM: OR.L D0, (A0) — the actual Signal() pattern ---
        ; Use \$C00A98 (longword-aligned) to start clean.
        lea     $00C00A98, A0      ; .L-aligned slow-RAM addr
        move.l  #$00000000, (A0)    ; start at zero
        move.l  #$00000100, D0      ; signal mask = bit 8
        .word   $8190                ; OR.L D0, (A0) — SigRecvd |= mask
        move.l  (A0), D1
        cmpi.l  #$00000100, D1
        bne     fail_slow_set

        ; OR again with a different mask — should accumulate
        move.l  #$80000000, D0
        .word   $8190                ; OR.L D0, (A0)
        move.l  (A0), D2
        cmpi.l  #$80000100, D2
        bne     fail_slow_accum

        ; OR with \$0 should not change anything
        move.l  #$00000000, D0
        .word   $8190                ; OR.L D0, (A0)
        move.l  (A0), D3
        cmpi.l  #$80000100, D3
        bne     fail_slow_zero

        ; --- 3. OR.L at unaligned slow-RAM dst (addr[1:0]=10) ---
        ; This is the actual Signal() pattern: tc_SigRecvd is at task+\$1A,
        ; which makes addr[1:0]=10 when task base is .L-aligned (.L-aligned
        ; task + \$1A = unaligned long).  Spans two slow-RAM slots.
        ; FS task at \$C00A80 → tc_SigRecvd at \$C00A9A (unaligned long).
        lea     $00C00A9A, A0      ; addr[1:0] = 10 — unaligned long
        move.l  #$00000000, $00C00A98  ; clear lower slot
        move.l  #$00000000, $00C00A9C  ; clear upper slot — span clean
        move.l  #$00000100, D0
        .word   $8190                ; OR.L D0, (A0)
        move.l  (A0), D4
        cmpi.l  #$00000100, D4
        bne     fail_slow_unal

        ; OR another mask — should accumulate at unaligned dst
        move.l  #$80000000, D0
        .word   $8190                ; OR.L D0, (A0)
        move.l  (A0), D5
        cmpi.l  #$80000100, D5
        bne     fail_slow_unal_accum

        stop    #0
forever: bra forever

fail_chip_or:        stop #$BAD1
fail_slow_set:       stop #$BAD2
fail_slow_accum:     stop #$BAD3
fail_slow_zero:      stop #$BAD4
fail_slow_unal:      stop #$BAD5
fail_slow_unal_accum: stop #$BAD6
