; 8520 CIA model smoke test.
; CIA-A is mapped at $00FE_0400; CIA-B at $00FE_0500.  Each has 16
; byte-sized registers:
;   $00 PRA   $01 PRB   $02 DDRA  $03 DDRB
;   $04 TALO  $05 TAHI  $06 TBLO  $07 TBHI
;   $0D ICR   $0E CRA   $0F CRB

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Test 1: write and read back DDRA / PRA on CIA-A ----
        move.b  #$FF, $00FE_0402         ; DDRA = all-output
        move.b  #$5A, $00FE_0400         ; PRA = $5A
        move.b  $00FE_0402, D0
        andi.l  #$FF, D0
        cmpi.l  #$FF, D0
        bne     fail_1
        move.b  $00FE_0400, D0
        andi.l  #$FF, D0
        cmpi.l  #$5A, D0
        bne     fail_2

        ; ---- Test 2: timer-A one-shot underflow sets ICR pending bit 0 ----
        ; One-shot mode (CRA bit 3 = 1) stops the timer on underflow so the
        ; ICR read-clear isn't racing further underflows.
        move.b  #$81, $00FE_040D         ; ICR: enable TA underflow (mask bit 0)
        move.b  #4,    $00FE_0404        ; TALO = 4
        move.b  #0,    $00FE_0405        ; TAHI (reload = 4)
        move.b  #$19,  $00FE_040E        ; CRA: RUNMODE=1 (one-shot) + LOAD + START
        ; Spin to let the timer underflow.
        moveq   #50, D1
spin1:  subq.l  #1, D1
        bne     spin1
        move.b  $00FE_040D, D0
        andi.l  #$1, D0
        cmpi.l  #$1, D0
        bne     fail_3
        ; Second read sees 0 because the first read cleared pending.
        move.b  $00FE_040D, D0
        andi.l  #$1, D0
        bne     fail_4

        ; ---- Test 3: timer-B one-shot stops itself ----
        move.b  #$82, $00FE_040D         ; ICR: enable TB underflow (bit 1)
        move.b  #3,    $00FE_0406        ; TBLO
        move.b  #0,    $00FE_0407        ; TBHI
        move.b  #$19,  $00FE_040F        ; CRB: RUNMODE=1 (bit 3) + LOAD + START
        ; Wait for TB underflow.
        moveq   #100, D1
poll_tb:
        move.b  $00FE_040D, D0
        andi.l  #$2, D0
        bne     tb_fired
        subq.l  #1, D1
        bne     poll_tb
        bra     fail_5
tb_fired:
        ; Check CRB[0] is back to 0 (one-shot stopped the timer).
        move.b  $00FE_040F, D0
        andi.l  #$1, D0
        bne     fail_6

        ; ---- Test 4: CIA-A and CIA-B are independent ----
        move.b  #$FF, $00FE_0502         ; CIA-B DDRA = all-output
        move.b  #$AA, $00FE_0400         ; CIA-A PRA
        move.b  #$55, $00FE_0500         ; CIA-B PRA (separate device)
        move.b  $00FE_0400, D0
        andi.l  #$FF, D0
        cmpi.l  #$AA, D0
        bne     fail_7
        move.b  $00FE_0500, D0
        andi.l  #$FF, D0
        cmpi.l  #$55, D0
        bne     fail_8

        stop    #0

fail_1: stop #$DA01
fail_2: stop #$DA02
fail_3: stop #$DA03
fail_4: stop #$DA04
fail_5: stop #$DA05
fail_6: stop #$DA06
fail_7: stop #$DA07
fail_8: stop #$DA08
