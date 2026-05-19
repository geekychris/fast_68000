; Zorro II autoconfig: simulated 8MB FAST RAM card.
;
; Sequence:
;   1. Read $E80000.  Type byte should be $E0 (Z2 RAM, 8MB).
;   2. Write the base nibbles at $E80048 / $E8004A (Kickstart-style
;      "configure" handshake).
;   3. Read $E80000 again.  The card has shut up, so the type byte
;      should now be $FF (no card here).
;
; Only the upper-byte lane carries the type byte in our model
; (matching real hardware where each autoconfig "register" is exposed
; as a nibble at the addressed byte's high lane).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- Phase 1: detect the card ----
        move.l  $00E80000, D0
        andi.l  #$FF000000, D0
        cmpi.l  #$E0000000, D0
        bne     fail_pre

        ; ---- Phase 2: configure the card ----
        ; Write the base high nibble at $48 with $20 in the high lane.
        ; Word write at $48 (long-aligned) puts the value in byte 0
        ; (be[3]) when written via a word access addressed at $48.
        move.w  #$2000, $00E80048
        ; Base low at $4A.  $4A is offset 2 within the long-aligned word,
        ; so the value lands in be[1] when written as a word.
        move.w  #$0000, $00E8004A

        ; ---- Phase 3: card has shut up ----
        move.l  $00E80000, D0
        cmpi.l  #$FFFFFFFF, D0
        bne     fail_post

        ; Reads at other autoconfig addresses also return $FF post-shutup.
        move.l  $00E80004, D0
        cmpi.l  #$FFFFFFFF, D0
        bne     fail_post

        stop    #0

fail_pre:  stop #$AC01
fail_post: stop #$AC02
