; MOVEM .L on a word-aligned-but-not-mod-4 stack must read the right value.
;
; When LINK An, #imm yields SP[1:0] == 10 (e.g. LINK with an odd-word
; displacement) and the function then does MOVEM.L Dn/Am, -(SP) the
; pushed registers land on a stack whose top is unaligned-.L.  Each
; MOVEM-step issues a long-aligned bus read at SP+0, SP+4, ... which
; straddles two mem[] entries on real 68000 silicon -- two consecutive
; word reads, with low half from bytes 2..3 of mem[idx] and high half
; from bytes 0..1 of mem[idx+1].
;
; The bus's unaligned-.L assembly only fires when dc_is_long is set,
; but the MOVEM-issue path forgot to drive it, so MOVEM-load returned
; mem[idx] verbatim (= the wrong half) and registers got restored to
; garbage.  Kickstart 1.3's InitResident driver hits this via
; MOVEM.L D2/A5, -(SP) ; ... ; MOVEM.L (SP)+, D2/A5 at $F80E88/$F80EBC
; with A5=$3E4 reaching back as $FFFB0000 after restore.
;
; This test reproduces the pattern: drive SP unaligned via LINK with
; #-$0E displacement and run MOVEM.L round-trip through -(SP) / (SP)+.
;
; Pass = halt 0.  Fail = stop $BADn for the register that lost its value.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Seed every register we'll touch with a recognisable pattern.
        move.l  #$11111111, D2
        move.l  #$22222222, D3
        move.l  #$33333333, A2
        move.l  #$44444444, A3
        move.l  #$55555555, A5

        ; LINK A5, #-$0E pushes A5 (4 bytes) then SP -= $0E.  With the
        ; entry SP at $4000, post-LINK SP = $4000 - 4 - $E = $3FEE, so
        ; SP[1:0] = 10 -- exactly the unaligned-.L stack alignment that
        ; trips MOVEM-load.
        link    A5, #-$0E

        ; The MOVEM-store path uses the same -(SP) decrement and so
        ; also writes unaligned long words.
        movem.l D2/D3/A2/A3, -(A7)

        ; Clobber the registers so a missed restore is visible.
        move.l  #0, D2
        move.l  #0, D3
        move.l  #0, A2
        move.l  #0, A3

        ; Restore via the unaligned (SP)+ path.  Without dc_is_long set
        ; during MOVEM, each read returned the wrong 16 bits and the
        ; registers landed on garbage.
        movem.l (A7)+, D2/D3/A2/A3

        cmpi.l  #$11111111, D2
        bne     fail_d2
        cmpi.l  #$22222222, D3
        bne     fail_d3
        cmpa.l  #$33333333, A2
        bne     fail_a2
        cmpa.l  #$44444444, A3
        bne     fail_a3

        unlk    A5
        cmpa.l  #$55555555, A5
        bne     fail_a5

        stop    #0
forever: bra forever

fail_d2: stop #$BAD2
fail_d3: stop #$BAD3
fail_a2: stop #$BAA2
fail_a3: stop #$BAA3
fail_a5: stop #$BAA5
