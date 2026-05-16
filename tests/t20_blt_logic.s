; Blitter logic-function test.
;
; Verifies the 8-bit minterm combiner by computing (A OR B) and (A AND B)
; on two 2-word patterns, with C=0.
;
;   A @ $2000 = $00FF, $F0F0
;   B @ $2100 = $0F0F, $FF00
;   D @ $3000: A OR  B  -> $0FFF, $FFF0
;   D @ $3100: A AND B  -> $000F, $F000

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- prepare source A and B ----
        move.l #$00FFF0F0, $00002000   ; A: word 0 = $00FF, word 1 = $F0F0
        move.l #$0F0FFF00, $00002100   ; B: word 0 = $0F0F, word 1 = $FF00

        ; ---- clear destinations ----
        move.l #0, $00003000
        move.l #0, $00003100

        ; ====================================
        ; Blit 1: D = A OR B.
        ; LF for "A OR B":
        ;   out=1 iff a=1 OR b=1
        ;   index=000 (a=0,b=0,c=0): 0
        ;   index=001 (a=0,b=0,c=1): 0
        ;   index=010 (a=0,b=1,c=0): 1
        ;   index=011 (a=0,b=1,c=1): 1
        ;   index=100 (a=1,b=0,c=0): 1
        ;   index=101 (a=1,b=0,c=1): 1
        ;   index=110 (a=1,b=1,c=0): 1
        ;   index=111 (a=1,b=1,c=1): 1
        ;   LF = $FC
        ; channels: USEA|USEB|USED = $0B
        ; ====================================
        move.l #$FC00000D, $00FE0000   ; BLTCON
        move.l #$0000FFFF, $00FE0004
        move.l #$0000FFFF, $00FE0008
        move.l #$00002000, $00FE000C   ; APT
        move.l #$00002100, $00FE0010   ; BPT
        move.l #$00003000, $00FE0018   ; DPT
        move.l #0, $00FE001C
        move.l #0, $00FE0020
        move.l #0, $00FE0028
        ; height=1, width=2 -> $44 / 6 ... (1<<6)|2 = $42
        move.l #$00000042, $00FE0038

w1:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w1

        move.l $00003000, D1
        cmpi.l #$0FFFFFF0, D1
        bne    fail1

        ; ====================================
        ; Blit 2: D = A AND B.
        ;   LF = $C0 (out=1 only when a=1 AND b=1)
        ; ====================================
        move.l #$C000000D, $00FE0000
        move.l #$00002000, $00FE000C
        move.l #$00002100, $00FE0010
        move.l #$00003100, $00FE0018
        move.l #$00000042, $00FE0038

w2:     move.l $00FE003C, D0
        andi.l #1, D0
        bne    w2

        move.l $00003100, D2
        cmpi.l #$000FF000, D2
        bne    fail2

        stop   #0

fail1:  stop   #$bd21
fail2:  stop   #$bd22
