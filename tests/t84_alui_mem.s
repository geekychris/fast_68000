; K_ALUI mem dst: ADDI/SUBI/ANDI/ORI/EORI/CMPI with memory destination.
;
; Previously these were no-ops in the core ("ALUI to memory: would need
; RMW; not implemented" comment in m68k_core.v).  Kickstart uses them
; extensively (e.g. counters in chip registers, ANDI to status bytes,
; CMPI on memory probe values during autoconfig scan).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- 1: ADDI.L #imm, abs.l ----
        move.l  #$00000010, $00010000
        addi.l  #$00000005, $00010000
        move.l  $00010000, D0
        cmpi.l  #$00000015, D0
        bne     fail_1

        ; ---- 2: SUBI.L #imm, abs.l ----
        move.l  #$00000020, $00010000
        subi.l  #$00000007, $00010000
        move.l  $00010000, D0
        cmpi.l  #$00000019, D0
        bne     fail_2

        ; ---- 3: ANDI.L #imm, abs.l ----
        move.l  #$DEADBEEF, $00010000
        andi.l  #$0F0F0F0F, $00010000
        move.l  $00010000, D0
        cmpi.l  #$0E0D0E0F, D0
        bne     fail_3

        ; ---- 4: ORI.L #imm, abs.l ----
        move.l  #$00000000, $00010000
        ori.l   #$AABBCCDD, $00010000
        move.l  $00010000, D0
        cmpi.l  #$AABBCCDD, D0
        bne     fail_4

        ; ---- 5: EORI.L #imm, abs.l ----
        move.l  #$FFFFFFFF, $00010000
        eori.l  #$0F0F0F0F, $00010000
        move.l  $00010000, D0
        cmpi.l  #$F0F0F0F0, D0
        bne     fail_5

        ; ---- 6: CMPI.L #imm, abs.l (read-only, sets Z if match) ----
        move.l  #$00001234, $00010000
        cmpi.l  #$00001234, $00010000
        bne     fail_6                  ; Z expected
        cmpi.l  #$00001234, $00010000   ; idempotent
        bne     fail_6
        cmpi.l  #$00001235, $00010000
        beq     fail_6                  ; mismatch -> Z=0

        ; ---- 7: ADDI.W word mem ----
        move.l  #$11220034, $00010004
        addi.w  #$0011, $00010006       ; word at byte-3-of-long position
        move.l  $00010004, D0
        cmpi.l  #$11220045, D0
        bne     fail_7

        ; ---- 8: ADDI.B byte mem (LS byte of long) ----
        move.l  #$11220034, $00010008
        addi.b  #$11, $0001000B
        move.l  $00010008, D0
        cmpi.l  #$11220045, D0
        bne     fail_8

        ; ---- 9: ADDI.L on (An) ----
        move.l  #$00000003, $0001000C
        move.l  #$0001000C, A0
        addi.l  #$00000002, (A0)
        move.l  $0001000C, D0
        cmpi.l  #$00000005, D0
        bne     fail_9

        ; ---- 10: CMPI.W with mismatch sets N=1 when result < 0 ----
        move.l  #$00000005, $00010010
        cmpi.w  #$0010, $00010012       ; 5 - $10 = -$0B, N=1
        bpl     fail_10                  ; N=1 expected

        ; ---- 11: ADDI sets X (so ADDX would chain) ----
        ; Use a value that produces a carry-out.
        move.l  #$FFFFFFFF, $00010014
        andi.w  #$ffee, CCR              ; clear X
        addi.l  #$00000001, $00010014    ; result 0, X=1
        move.l  $00010014, D0
        cmpi.l  #$00000000, D0
        bne     fail_11
        move.w  SR, D0
        andi.l  #$10, D0
        cmpi.l  #$10, D0
        bne     fail_11

        stop    #0

fail_1:  stop #$A101
fail_2:  stop #$A102
fail_3:  stop #$A103
fail_4:  stop #$A104
fail_5:  stop #$A105
fail_6:  stop #$A106
fail_7:  stop #$A107
fail_8:  stop #$A108
fail_9:  stop #$A109
fail_10: stop #$A10A
fail_11: stop #$A10B
