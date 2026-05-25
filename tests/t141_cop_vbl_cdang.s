; t141 -- Copper VBL auto-restart + OCS CDANG register-access protection.
;
; K1.3 / WB1.3 program COP1LC then enable DMACON.COPEN+DMAEN and rely
; on Agnus restarting the Copper from cop1lc at every vertical blank.
; Without that auto-restart our Copper sat in S_IDLE and K1.3's
; BPLnPT / SPRnPT setup never reached the chipset.
;
; Once running, the Copper has to obey OCS register-access protection
; or a Copper that walks off a valid list and into chip-RAM code
; interprets opcodes (e.g. $4EF9 = JMP) as MOVEs to DSKLEN ($DFF024)
; and BLTDPTL ($DFF056), corrupting in-progress disk reads + blits:
;
;   * $00-$3E (disk DMA, serial, INTREQ, vectors)  always blocked.
;   * $40-$7E (blitter)  blocked when COPCON.CDANG=0; allowed when =1.
;   * $80+                always allowed.
;
; This test does NOT touch any legacy $00FE0040..$00FE007F address
; because that range maps to the Copper/Blitter legacy slave: a
; spurious write to $FE0044 would land on COPJMP1 ($FE0044, slv[4:2]=1)
; and start the Copper before we wanted it.  All chipset accesses use
; the canonical $DFFxxx port.

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; ---- pre-stage COLOR00 via canonical long write so we can
        ;      tell if the Copper's MOVE later landed.
        move.l #$00000000, $00DFF180

        ; ---- Copper list at $6000:
        ;   MOVE $0F00 -> COLOR00 ($DFF180)            allowed ($180 >= $80)
        ;   MOVE $DEAD -> BLTAPTH ($DFF050)            blocked under CDANG=0
        ;   MOVE $4EF9 -> DSKLEN  ($DFF024)            blocked unconditionally
        ;   WAIT VP=$FF                                 end-of-list
        move.l #$01800F00, $00006000
        move.l #$0050DEAD, $00006004
        move.l #$00244EF9, $00006008
        move.l #$FFFFFFFE, $0000600C

        ; ---- program COP1LC via canonical $DFF080, then enable
        ;      DMACON.COPEN+DMAEN.  CDANG stays 0 (default).
        move.l #$00006000, $00DFF080
        move.w #$8280,     $00DFF096        ; SET | DMAEN | COPEN

        ; ---- spin long enough for at least one VBL auto-kick.
        ;      Our Agnus VBL fires every ~71K host clocks; spin for many.
        move.l #$00020000, D0
spin1:
        subq.l #1, D0
        bne    spin1

        ; ---- verify the auto-restart fired and the COLOR write landed.
        ;      Canonical write was even-aligned (be=$C, addr[1]=0), so
        ;      legacy storage keeps slv_wdata in bits [31:16].
        move.l $00DFF180, D1
        andi.l #$FFFF0000, D1
        cmpi.l #$0F000000, D1
        bne    fail1

        ; ---- enable CDANG=1 via canonical COPCON write so the Copper
        ;      can program the blitter from a list.  Stop the running
        ;      Copper first by clearing DMACON.COPEN.
        move.w #$0080,     $00DFF096        ; CLR COPEN (no SET bit)

        ; Re-arm with a new list at $6100 that pokes BLTAFWM ($DFF044)
        ; from the Copper.  Under CDANG=1 this $40-$7E write is allowed.
        move.l #$00440BEE, $00006100        ; MOVE $0BEE -> BLTAFWM
        move.l #$FFFFFFFE, $00006104

        move.l #$00006100, $00DFF080        ; COP1LC = list B
        move.w #$0002,     $00DFF02E        ; COPCON.CDANG = 1
        move.w #$8280,     $00DFF096        ; SET | DMAEN | COPEN

        move.l #$00020000, D0
spin2:
        subq.l #1, D0
        bne    spin2

        ; ---- verify CDANG=1 unblocked BLTAFWM write.  Blitter's
        ;      legacy slave puts BLTAFWM at $FE0004 (slv_addr=$04).
        ;      The canonical-to-internal xlat for BLTAFWM stores the
        ;      16-bit value in the low half of the 32-bit slot
        ;      ({16'd0, val}); legacy long-read returns $00000BEE.
        move.l $00FE0004, D1
        cmpi.l #$00000BEE, D1
        bne    fail2

        stop   #0

fail1:  stop   #$c141      ; COLOR00 write didn't land -- auto-restart broken
fail2:  stop   #$c142      ; BLTAFWM write didn't land -- CDANG=1 not honoured
