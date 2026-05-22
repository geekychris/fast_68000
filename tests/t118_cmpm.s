; CMPM.B (Ay)+, (Ax)+ -- post-increment memory compare.
;
; Used by Kickstart 1.3's FindName / FindResource / OpenResource at
; $F81A56:
;   $F81A50: A0 = candidate.ln_Name
;   $F81A54: A1 = D1 (target name)
;   $F81A56: CMPM.B (A0)+, (A1)+   (opcode $B308 = CMPM.B (A0)+, (A1)+)
;   $F81A58: BNE next_node          ; mismatch -> try next
;   $F81A5A: TST.B $-1(A0)          ; was the byte zero (end of string)?
;   $F81A5E: BNE $F81A56             ; if no, keep comparing
;
; If CMPM isn't decoded, the boot can't find resources by name, and
; library inits that OpenResource("ciab.resource") see NULL.
;
; Note: $B308 currently falls into the decoder's EOR.B Dn,<ea> pattern
; (`1011_???_1??_???_???`), which mismatches because EOR.B with An
; direct destination isn't a valid m68k op.  CMPM has bits[5:3] = 001
; AND src/dst are both *post-increment* memory pointers; the encoding
; reuses mode 001 to signal that.
;
; Test: lay two identical 4-byte strings in memory, run CMPM.B repeatedly,
; check Z=1 on each match, then point one ptr at a different string,
; verify Z=0 on mismatch.  Also exercise CMPM.W / CMPM.L.
;
; Pass = halt 0.  Failures:
;   $BAD1 = CMPM.B Z flag wrong on match
;   $BAD2 = CMPM.B A0/A1 not incremented after match
;   $BAD3 = CMPM.B Z flag wrong on mismatch
;   $BAD4 = CMPM.W Z wrong on match
;   $BAD5 = CMPM.L Z wrong on match

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Lay down two identical 4-byte strings.
        move.l  #$DEADBEEF, $00002000
        move.l  #$DEADBEEF, $00002004
        ; And a third that mismatches.
        move.l  #$BAADCAFE, $00002008

        ; --- 1. CMPM.B match ---
        lea     $00002000, A0
        lea     $00002004, A1
        ; CMPM.B (A0)+, (A1)+
        .word   $B308
        bne     fail1

        ; A0 should be $2001, A1 should be $2005.
        move.l  A0, D0
        cmpi.l  #$00002001, D0
        bne     fail2
        move.l  A1, D0
        cmpi.l  #$00002005, D0
        bne     fail2

        ; --- 2. CMPM.B mismatch ---
        lea     $00002000, A0
        lea     $00002008, A1               ; points at $BA...
        .word   $B308
        beq     fail3

        ; --- 3. CMPM.W match ---
        lea     $00002000, A0
        lea     $00002004, A1
        ; CMPM.W (A0)+, (A1)+ = $B348
        .word   $B348
        bne     fail4

        ; --- 4. CMPM.L match ---
        lea     $00002000, A0
        lea     $00002004, A1
        ; CMPM.L (A0)+, (A1)+ = $B388
        .word   $B388
        bne     fail5

        stop    #0

fail1:  stop    #$BAD1
fail2:  stop    #$BAD2
fail3:  stop    #$BAD3
fail4:  stop    #$BAD4
fail5:  stop    #$BAD5
