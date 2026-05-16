; Copper chains the blitter: Copper writes all the blitter registers
; including BLTSIZE, then WAITs for blitter completion, then HALTs.
; The CPU only sets up the source data, kicks the Copper, and polls
; COPSTAT.  The Copper handles the whole blit.
;
; Source @ $2000: $1234ABCD $DEADBEEF (4 words)
; Dest   @ $3000
; After Copper + blit: $3000 should match the source.

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

        ; ---- Copper list at $6000: 11 MOVEs + WAIT + HALT = 13 insns = 104 bytes ----
        ;   Programs the blitter for a simple A->D copy of 4 words,
        ;   then BLTSIZE write to kick it off, then WAIT-for-blitter, then HALT.
        move.l #$00FE0000, $00006000    ; target = BLTCON
        move.l #$F0000009, $00006004    ; data: LF=$F0 (A), USEA|USED
        move.l #$00FE0004, $00006008    ; target = BLTAFWM
        move.l #$0000FFFF, $0000600C    ; data
        move.l #$00FE0008, $00006010    ; target = BLTALWM
        move.l #$0000FFFF, $00006014    ; data
        move.l #$00FE000C, $00006018    ; BLTAPT
        move.l #$00002000, $0000601C    ; = source
        move.l #$00FE0018, $00006020    ; BLTDPT
        move.l #$00003000, $00006024    ; = dest
        move.l #$00FE001C, $00006028    ; BLTAMOD
        move.l #0,         $0000602C
        move.l #$00FE0028, $00006030    ; BLTDMOD
        move.l #0,         $00006034
        move.l #$00FE0038, $00006038    ; BLTSIZE  (writing this starts the blit)
        move.l #$00000044, $0000603C    ; height=1 width=4 -> $44
        move.l #$FFFFFFFE, $00006040    ; WAIT for blitter not-busy
        move.l #0,         $00006044
        move.l #$FFFFFFFF, $00006048    ; HALT
        move.l #0,         $0000604C

        ; ---- start the Copper ----
        move.l #$00006000, $00FE0040    ; COP1LC
        move.l #1,         $00FE0044    ; COPJMP1

wait:   move.l $00FE0048, D0
        andi.l #1, D0
        bne    wait

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
