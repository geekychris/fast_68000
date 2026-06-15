; CIA timer B chained to timer A (CRB bit 6 = INMODE1) — K1.3
; timer.device uses this for EClock long-period delays.
;
; Setup:
;   - TA = continuous, reload = 4   (underflows every 5 ticks)
;   - TB = continuous, reload = 2   (counts TA underflows, fires after 3)
;   - CRB bit 6 = 1 so TB counts TA underflows instead of phi2.
;
; Expected: TB ICR pending fires after roughly 5*3 = 15 phi2 ticks.
; Before the chain fix, TB never fired because the gating condition
; was unconditional on `tick` (phi2) and bit 6 was ignored.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Mask: enable TB IRQ source (bit 1) only.
        move.b  #$82, $00FE040D         ; bit7=SET, bit1=TB

        ; Configure TA reload = 4 (continuous).
        move.b  #4,    $00FE0404        ; TALO
        move.b  #0,    $00FE0405        ; TAHI

        ; Configure TB reload = 2 (continuous, chain TA underflows).
        move.b  #2,    $00FE0406        ; TBLO
        move.b  #0,    $00FE0407        ; TBHI

        ; CRB = $41: bit 6 = INMODE1 chain, bit 0 = START, RUNMODE = 0.
        move.b  #$41,  $00FE040F

        ; CRA = $01: START only (continuous).
        move.b  #$01,  $00FE040E

        ; Spin for plenty of ticks (>> 15).  We need TB to underflow
        ; once and set ICR bit 1.
        moveq   #100, D1
spin:
        subq.l  #1, D1
        bne     spin

        ; Check ICR pending bit 1 = TB underflow.  (Bit 0 may also be
        ; set since TA is free-running.)
        move.b  $00FE040D, D0
        andi.l  #$2, D0
        cmpi.l  #$2, D0
        bne     fail_no_tb

        stop    #0

fail_no_tb:        stop #$D651
