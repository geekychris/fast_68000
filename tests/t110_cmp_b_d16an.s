; CMP.B (d16,An), Dn -- byte source from memory at odd byte offset.
;
; The CPU was passing the raw 32-bit longword fetched from the bus into
; the ALU as alu_b, then relying on the ALU's size-mask to take the low
; byte.  But on a 32-bit aligned read the byte at addr offset 1, 2, 3
; lives in bits[23:16] / [15:8] / [7:0] respectively.  Taking the low
; byte always returned the byte at offset 3 -- the WRONG byte for any
; odd-address CMP.B.
;
; Surfaced as InitCode's resident-class check at $F8104A computing the
; wrong N flag and BGT'ing past `strap`, skipping the Kickstart boot
; loader.
;
; The assembler doesn't have cmp.b; we encode it directly:
;   $B629 0001 = CMP.B $0001(A1), D3
;   $B629 0002 = CMP.B $0002(A1), D3
;   $B629 0003 = CMP.B $0003(A1), D3
;   $B629 0000 = CMP.B $0000(A1), D3
;
; Pass = halt 0.  Failures:
;   $BAD1 = offset 0 (high byte) wrong
;   $BAD2 = offset 1 (= the InitCode case) wrong  <-- the bug
;   $BAD3 = offset 2 wrong
;   $BAD4 = offset 3 wrong

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        lea     $00002000, A1

        ; Long at $2000 = $01_28_00_C4 (exact byte pattern from strap's
        ; RT_FLAGS / RT_VERSION / RT_TYPE / RT_PRI).  Offset 1 = $28.
        move.l  #$012800C4, $00002000

        moveq   #0, D3

        ; CMP.B $0(A1), D3 : 0 - $01 = $FF, expect N=1
        .word $B629, $0000
        bpl     fail0
        ; CMP.B $1(A1), D3 : 0 - $28 = $D8, expect N=1  <-- THE BUG
        .word $B629, $0001
        bpl     fail1
        ; CMP.B $2(A1), D3 : 0 - $00 = $00, expect Z=1
        .word $B629, $0002
        bne     fail2
        ; CMP.B $3(A1), D3 : 0 - $C4 = $3C, expect N=0
        .word $B629, $0003
        bmi     fail3

        stop    #0

fail0:  stop    #$BAD1
fail1:  stop    #$BAD2
fail2:  stop    #$BAD3
fail3:  stop    #$BAD4
