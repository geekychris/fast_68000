; Regression: MOVEM.L d16(PC), <regs> must compute source as
; (d16-extension-word-address + sign-extended d16), not fall through
; to ex_ra.
;
; Pre-fix the K_MOVEM EA_EXT case only handled EA7_ABSW; EA7_PCDISP
; (mode 7 reg 2) fell into `movem_addr <= ex_ra`.  Surfaced by the
; WB1.3 boot at chip-RAM $5D82 which executes
;   MOVEM.L $FFC4(PC), A2/A5/A6
; and ended up loading from the trap-vector area instead of the
; intended PC-rel table, producing A5=$082800FC and a BAD-PC chain.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$00004000, D0
        beq     core0
        stop    #0

core0:
        ; Sentinel registers so we can tell the MOVEM actually
        ; touched them.
        move.l  #$DEADBEEF, A2
        move.l  #$DEADBEEF, A5
        move.l  #$DEADBEEF, A6

        ; Skip the embedded data table and land on the MOVEM.
        bra     do_movem

src_table:
        .long $11111111   ; -> A2
        .long $22222222   ; -> A5
        .long $33333333   ; -> A6

do_movem:
        ; Hand-emit:  MOVEM.L (src_table - PC_of_d16_word)(PC), A2/A5/A6
        ;   opcode $4CFA  (mem->reg, .L, mode 7 reg 2)
        ;   mask   $6400  (bit10=A2, bit13=A5, bit14=A6;
        ;                   postinc bit order)
        ;   d16    = src_table - addr_of_d16_word.
        ; The minimal assembler resolves `label - .` -- approximated
        ; by emitting a relative label diff.
        ; Layout (after .org $400):
        ;   start..core0 preamble                       @ $400..$40D
        ;   3x move.l #imm, An  (6 bytes each)          @ $40E..$41F
        ;   bra do_movem        (2 bytes)               @ $420..$421
        ;   src_table .long x3  (12 bytes)              @ $422..$42D
        ;   do_movem: .word $4CFA                       @ $42E
        ;             .word $6400  (mask)               @ $430
        ;             .word d16    (d16 word)           @ $432
        ;   d16 word lives at $432; target $422 means
        ;   d16 = $422 - $432 = -$10 = $FFF0 (signed)
        .word $4CFA
        .word $6400
        .word $FFF0

        cmpa.l  #$11111111, A2
        bne     fail_a2
        cmpa.l  #$22222222, A5
        bne     fail_a5
        cmpa.l  #$33333333, A6
        bne     fail_a6
        stop    #0

fail_a2:  stop  #$BA01
fail_a5:  stop  #$BA02
fail_a6:  stop  #$BA03
