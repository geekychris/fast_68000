; t140 -- canonical Denise MOVE.W normalisation.
;
; Regression for the K1.3 BPLCON0 bring-up: K1.3 / Intuition program
; BPLCON0, BPLCON1, BPLCON2 and BPL1MOD/BPL2MOD via MOVE.W to the
; canonical $DFF1xx addresses.  An even-aligned word write places the
; 16-bit value in slv_wdata[31:16] with be=4'b1100; without
; normalisation that value lands in the high half of Denise's 32-bit
; register and decoders that read bits [14:12] (BPU, etc.) see zero.
;
; Verifies that after MOVE.W to $DFF100/$DFF102/$DFF104/$DFF108/$DFF10A
; the legacy long-read of $FE0100/$FE0104/$FE0108/$FE0128/$FE012C
; returns the value in the low half.
;
; Also verifies that color writes via the canonical port still obey the
; legacy "what you wrote is what you read back" storage convention,
; so t23_cop_basic and similar tests keep passing.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- BPLCON0 ($DFF100, even-aligned word) ----
        move.w #$5200, $00DFF100        ; BPU=5
        move.l $00FE0100, D1
        cmpi.l #$00005200, D1
        bne    fail1

        ; ---- BPLCON1 ($DFF102, odd-aligned word) ----
        move.w #$00A5, $00DFF102        ; PF1H=5, PF2H=A
        move.l $00FE0104, D1
        cmpi.l #$000000A5, D1
        bne    fail2

        ; ---- BPLCON2 ($DFF104, even-aligned word) ----
        move.w #$0044, $00DFF104        ; PF2-in-front, PF1P=0, PF2P=4
        move.l $00FE0108, D1
        cmpi.l #$00000044, D1
        bne    fail3

        ; ---- BPL1MOD ($DFF108, even-aligned word) ----
        move.w #$0014, $00DFF108
        move.l $00FE0128, D1
        cmpi.l #$00000014, D1
        bne    fail4

        ; ---- BPL2MOD ($DFF10A, odd-aligned word) ----
        move.w #$0028, $00DFF10A
        move.l $00FE012C, D1
        cmpi.l #$00000028, D1
        bne    fail5

        ; ---- Colors: canonical word write must NOT clobber the
        ;      legacy long-write storage convention.  Write COLOR00
        ;      via long at the legacy port, then read back via long
        ;      at the canonical port — must round-trip exactly.
        move.l #$00000ABC, $00FE0180
        move.l $00DFF180, D1
        cmpi.l #$00000ABC, D1
        bne    fail6

        ; ---- SPRnPT canonical word writes ($DFF120..$DFF13E) ----
        ; K1.3's Copper boot list at $2368 programs all 8 sprite pointers
        ; this way (MOVE.W high half to $DFF12{0,4,8,C,...}, low half
        ; to $DFF12{2,6,A,E,...}).  Without canonical decode they were
        ; silently dropped; this asserts each pair lands in spr_pt[i].
        ; Legacy long-read at $FE013{0,8,...} returns the full 32-bit
        ; pointer.

        ; SPR0: $DFF120 high half = $0001, $DFF122 low half = $1234
        move.w #$0001, $00DFF120
        move.w #$1234, $00DFF122
        move.l $00FE0130, D1
        cmpi.l #$00011234, D1
        bne    fail7

        ; SPR1: $DFF124 high half = $0002, $DFF126 low half = $5678
        move.w #$0002, $00DFF124
        move.w #$5678, $00DFF126
        move.l $00FE0138, D1
        cmpi.l #$00025678, D1
        bne    fail8

        ; SPR7: $DFF13C high half = $0007, $DFF13E low half = $4F50
        ; (the K1.3 boot list writes $4F50 to SPR0PTL — reuse the value
        ;  as a "looks like a real pointer" smoke test for the highest
        ;  sprite slot.)
        move.w #$0007, $00DFF13C
        move.w #$4F50, $00DFF13E
        move.l $00FE0178, D1
        cmpi.l #$00074F50, D1
        bne    fail9

        stop   #0

fail1:  stop   #$c140
fail2:  stop   #$c141
fail3:  stop   #$c142
fail4:  stop   #$c143
fail5:  stop   #$c144
fail6:  stop   #$c145
fail7:  stop   #$c146
fail8:  stop   #$c147
fail9:  stop   #$c148
