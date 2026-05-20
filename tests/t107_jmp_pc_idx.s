; JMP $d8(PC, Xn) -- the Kickstart 1.3 bytecode-dispatcher form.
;
; Background: addressing mode 7/reg 3 (= EA_EXT / EA7_PCIDX) is
; "PC plus 8-bit signed displacement plus indexed register".  The
; index register Xn is named by the brief extension word that
; follows the opcode, and it can be ANY D0..D7/A0..A7.  Our impl
; previously routed the rc regfile port to Xn only for EA_IDX
; (mode 6, d8(An, Xn)).  For mode 7/reg 3 the rc port defaulted
; to A7 -- so JMP / MOVE / etc. with d8(PC, Xn) used A7's value
; instead of the actual Xn.  Kickstart 1.3's bytecode dispatch
; at $F81168 (`JMP $2C(PC, D1.W)`) tripped this: it read A7 (a
; supervisor stack address in low chip RAM) instead of D1 and
; jumped to a wildly wrong target (~$F86EEE = a 65K-iter bitplane
; render loop) instead of one of the dispatcher case handlers.
;
; Test:
;   - Set D1 = 6 (= 3 entries forward in a 16-bit jump table).
;   - Set A7 = $4000 (= where we are at reset).
;   - Execute JMP $tab-PC(PC, D1.W).  If A7 is wrongly used as
;     the index, target = PC+disp+$4000 which is far away.  If
;     D1 is correctly used as the index, target = PC+disp+6 which
;     hits the third long in our local table.
;
; Pass = halt 0.  Failures: $BAD1 (landed at the A7-relative spot)
; or $BAD2 (jumped to anywhere unexpected).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Place each candidate target as a unique stop code; if the
        ; JMP lands on the "right" one we fall through to halt 0.
        ; The brief-ext encoding for the JMP is hand-checked:
        ; d8 = signed byte, Xn = D1.W.  We use a label "tab" and
        ; let the assembler compute (tab - (jmp_pc + 2)) as the d8.
        moveq   #6, D1                 ; index into table-of-4
        jmp     tab(PC, D1.W)          ; (mode 7, reg 3 -- d8(PC,Xn))

        ; Targets via the table.  Each is a different JMP.
tab:
        ; D1=0 -> first target (BAD1 — only reached if Xn=0)
        jmp     case0
        ; D1=6 -> second target (skip the JMP above + reach this one)
        jmp     case1
        ; D1=12 -> third target
        jmp     case_a7_used

case0:
        stop    #$BAD1                 ; landed at index 0 (Xn=0?)
case1:
        ; This is the EXPECTED case (D1=6).
        stop    #0
case_a7_used:
        stop    #$BAD2                 ; would happen if Xn=A7 routed
                                       ; wrongly to here at the right disp
forever:
        bra     forever
