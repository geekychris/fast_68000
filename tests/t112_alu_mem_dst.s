; K_ALU direction=1: ADD/SUB/AND/OR/EOR Dn, <ea> with memory destination.
;
; The decoder set direction=opcode[8] for the 1xxx ALU family, but the
; K_ALU executor only ever handled direction=0 (Dn destination, mem
; source).  Direction=1 (Dn source, mem destination) was a silent
; no-op -- the load was scheduled but no write-back, no CCR update.
;
; Surfaced in Kickstart 1.3 cold-boot via `SUB.L D0, $001C(A0)` at
; $F81E04 -- the allocator's mh_Free decrement.  Without it, mh_Free
; stayed pinned at its initial value and downstream cold-boot code
; mis-counted free memory.
;
; The asm68k tool only supports the <ea>, Dn form of ALU ops, so we
; encode the Dn, <ea> form via .word:
;   SUB.L D0, 0(A0)  = $91A8 $0000
;   ADD.L D0, 0(A0)  = $D1A8 $0000
;   AND.W D1, $2(A0) = $C368 $0002
;   OR.B  D2, $3(A0) = $8528 $0003
;
; Pass = halt 0.  Failures:
;   $BAD1 = SUB.L Dn, mem didn't decrement memory
;   $BAD2 = SUB.L CCR result wrong (N/C flag)
;   $BAD3 = ADD.L Dn, mem didn't write
;   $BAD4 = AND.W Dn, mem didn't write (word lane at offset 2)
;   $BAD5 = OR.B Dn, mem didn't write (byte lane at offset 3)
;   $BAD6 = sibling longword clobbered

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        lea     $00002000, A0
        move.l  #$0007BFE0, $00002000      ; mh_Free-like value
        move.l  #$DEADBEEF, $00001FFC       ; sibling below
        move.l  #$CAFEBABE, $00002004       ; sibling above

        ; D0 = $50; SUB.L D0, 0(A0) -> mem becomes $7BF90.
        move.l  #$00000050, D0
        .word   $91A8, $0000                 ; SUB.L D0, $0(A0)

        cmpi.l  #$0007BF90, $00002000
        bne     fail1
        cmpi.l  #$DEADBEEF, $00001FFC
        bne     fail6
        cmpi.l  #$CAFEBABE, $00002004
        bne     fail6

        ; Repeat to check CCR: $7BF90 - $50 = $7BF40, positive, no borrow.
        .word   $91A8, $0000                 ; SUB.L D0, $0(A0)
        bmi     fail2
        bcs     fail2

        ; ADD.L D0, 0(A0): mem $7BF40 + $10 (we'll reset D0)
        move.l  #$00000010, D0
        .word   $D1A8, $0000                 ; ADD.L D0, $0(A0)
        cmpi.l  #$0007BF50, $00002000
        bne     fail3

        ; AND.W D1, $2(A0):  word at offset 2 = $BF50.
        ; D1 low word = $F0F0.  $BF50 & $F0F0 = $B050.
        ; Long becomes $0007_B050.
        move.l  #$0000F0F0, D1
        .word   $C368, $0002                 ; AND.W D1, $2(A0)
        cmpi.l  #$0007B050, $00002000
        bne     fail4

        ; OR.B D2, $3(A0): byte at offset 3 = $50.
        ; D2 low byte = $0F.  $50 | $0F = $5F.
        ; Long becomes $0007_B05F.
        move.l  #$0000000F, D2
        .word   $8528, $0003                 ; OR.B D2, $3(A0)
        cmpi.l  #$0007B05F, $00002000
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
fail6:  stop    #$BAD6
