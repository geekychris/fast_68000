; Group-0 stack-frame layout: address error builds the 14-byte
; M68000 frame.  Layout from SP at handler entry:
;
;   SP +  0   SSW (special status word; bit 0 = R/W)
;   SP +  2   fault address (32-bit)
;   SP +  6   instruction register (the faulting opcode word)
;   SP +  8   SR
;   SP + 10   PC (post-instruction; our convention)
;
; The handler captures all five fields into known memory locations
; so the test body can verify them.

        .org $C
        .long addr_err_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Trigger an address error via a misaligned .w read.
        move.l  #$00020001, A0           ; odd address
        move.w  (A0), D1                  ; <-- traps, handler captures frame

        ; ---- Verify the captured frame ----

        ; SSW low byte should have bit 0 set (read, R/W = 1).
        move.l  $00020010, D0
        andi.l  #$1, D0
        cmpi.l  #$1, D0
        bne     fail_ssw

        ; Fault address should be $00020001.
        move.l  $00020014, D0
        cmpi.l  #$00020001, D0
        bne     fail_addr

        ; IR should be nonzero (it's the opcode word of move.w (A0),D1).
        move.l  $00020018, D0
        andi.l  #$0000FFFF, D0
        beq     fail_ir

        ; SR low byte: S=1 (we're in supervisor mode at exception entry).
        ; SR is bit 13 of saved SR (= $2000).
        move.l  $0002001C, D0
        andi.l  #$2000, D0
        cmpi.l  #$2000, D0
        bne     fail_sr

        ; Saved PC should be at or just past the move.w instruction (we
        ; save PC_next, so the resume point is the next instruction).
        ; We don't pin the exact PC, just check it's a sensible address.
        move.l  $00020020, D0
        cmpi.l  #$400, D0
        bls     fail_pc                  ; PC <= $400 -> bad

        stop    #0

fail_ssw:  stop #$F501
fail_addr: stop #$F502
fail_ir:   stop #$F503
fail_sr:   stop #$F504
fail_pc:   stop #$F505

; ---- Address-error handler ----
; Captures the Group-0 frame fields into memory at $00020010..$00020023:
;   $00020010: SSW
;   $00020014: fault addr
;   $00020018: IR
;   $0002001C: SR
;   $00020020: PC
addr_err_handler:
        ; SSW at (A7+0)
        move.w  (A7), D0
        ext.l   D0
        andi.l  #$FFFF, D0
        move.l  D0, $00020010

        ; fault addr at (A7+2)
        move.l  2(A7), D0
        move.l  D0, $00020014

        ; IR at (A7+6)
        move.w  6(A7), D0
        andi.l  #$FFFF, D0
        move.l  D0, $00020018

        ; SR at (A7+8)
        move.w  8(A7), D0
        andi.l  #$FFFF, D0
        move.l  D0, $0002001C

        ; PC at (A7+10)
        move.l  10(A7), D0
        move.l  D0, $00020020

        ; Drop Group-0 extras (8 bytes), then RTE pops SR+PC.
        addq.l  #8, A7
        rte
