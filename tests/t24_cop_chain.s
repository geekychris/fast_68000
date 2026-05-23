; Copper chains the canonical-Amiga blitter (S41 rewrite).
;
; Copper writes the canonical blitter register file at $DFF040..$DFF066
; via canonical 16-bit MOVEs, then ends.  Writing BLTSIZE ($DFF058)
; kicks the blit.  CPU polls Copper-done then blitter-done and
; verifies the A->D copy.
;
; Source @ $2000: $1234ABCD $DEADBEEF.
; Dest   @ $3000: should match source after the blit.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- source / dest setup ----
        move.l #$1234ABCD, $00002000
        move.l #$DEADBEEF, $00002004
        move.l #0,         $00003000
        move.l #0,         $00003004

        ; ---- Copper list at $6000 (canonical encoding, 4 bytes / insn).
        ;   IR1 = high word of each .L = register offset (even); IR1[0]=0
        ;   means MOVE.  IR2 = low word = 16-bit data.
        ;     BLTCON0  = $DFF040 <- $09F0   (USEA|USED|LF=A)
        ;     BLTCON1  = $DFF042 <- $0000
        ;     BLTAFWM  = $DFF044 <- $FFFF
        ;     BLTALWM  = $DFF046 <- $FFFF
        ;     BLTAPTH  = $DFF050 <- $0000
        ;     BLTAPTL  = $DFF052 <- $2000
        ;     BLTDPTH  = $DFF054 <- $0000
        ;     BLTDPTL  = $DFF056 <- $3000
        ;     BLTAMOD  = $DFF064 <- $0000
        ;     BLTDMOD  = $DFF066 <- $0000
        ;     BLTSIZE  = $DFF058 <- $0044   (height=1, width=4 -- starts the blit)
        ;     end-of-list = WAIT VP=$FF
        move.l #$004009F0, $00006000      ; BLTCON0
        move.l #$00420000, $00006004      ; BLTCON1
        move.l #$0044FFFF, $00006008      ; BLTAFWM
        move.l #$0046FFFF, $0000600C      ; BLTALWM
        move.l #$00500000, $00006010      ; BLTAPTH
        move.l #$00522000, $00006014      ; BLTAPTL
        move.l #$00540000, $00006018      ; BLTDPTH
        move.l #$00563000, $0000601C      ; BLTDPTL
        move.l #$00640000, $00006020      ; BLTAMOD
        move.l #$00660000, $00006024      ; BLTDMOD
        move.l #$00580044, $00006028      ; BLTSIZE (kicks blit)
        move.l #$FFFFFFFE, $0000602C      ; end-of-list

        ; ---- start the Copper ----
        move.l #$00006000, $00FE0040      ; COP1LC
        move.l #1,         $00FE0044      ; COPJMP1

        ; Wait for the Copper to finish issuing the writes.
wait_cop:
        move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait_cop

        ; Now wait for the blit itself to complete (legacy BLTSTAT).
wait_blt:
        move.l $00FE003C, D0
        andi.l #1, D0
        bne    wait_blt

        ; ---- verify ----
        move.l $00003000, D1
        cmpi.l #$1234ABCD, D1
        bne    fail1
        move.l $00003004, D2
        cmpi.l #$DEADBEEF, D2
        bne    fail2
        stop   #0

fail1:  stop   #$cd21
fail2:  stop   #$cd22
