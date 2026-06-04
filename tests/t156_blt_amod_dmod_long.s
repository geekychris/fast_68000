; MOVE.L to BLTCMOD ($DFF060) must commit both BLTCMOD and BLTBMOD
; ($DFF062).  MOVE.L to BLTAMOD ($DFF064) must commit both BLTAMOD
; and BLTDMOD ($DFF066).
;
; K1.3 graphics.library at PC $FC5CFE does:
;     MOVE.L $0C(A1), D0
;     MOVE.L D0, $0064(A0)        ; $DFF064 — BLTAMOD high, BLTDMOD low
;     MOVE.W D0, $0060(A0)        ; $DFF060 — BLTCMOD only
;
; The intent is to set BLTAMOD = high16(D0), BLTDMOD = low16(D0) in
; ONE instruction.  Pre-fix, our chipset translated the longword as a
; single 16-bit write to BLTAMOD and DROPPED the low half intended for
; BLTDMOD, leaving BLTDMOD with its stale value from a previous blit.
;
; At WB1.3 boot r=4.2M this stale BLTDMOD was -90 (carried over from
; an earlier graphics.library blit at PC $FC7C68).  The bitmap-clear
; blit issued by graphics at $FC5E08 inherited the -90, causing the
; destination pointer to walk DOWN 12 bytes per row through 187 rows,
; marching out of the bitmap allocation and into K1.3's BCPL DOS pool
; data at chip $5E40.  Corrupting $5E40 (which held a DOS list-node
; pointer field) propagated to a "Volume Workbench1.3 has a read/write
; error" requester at r=4,444,593, wedging WB1.3 boot.
;
; This test pre-seeds the partner mod regs with sentinels, then writes
; longwords to BLTCMOD ($DFF060) and BLTAMOD ($DFF064), and verifies
; that both halves of each longword landed correctly.

        .org $400
start:
        move.l  #$00DFF000, A0          ; A0 = custom chipset base

        ; -------- Test 1: BLTAMOD + BLTDMOD via MOVE.L $DFF064 --------
        ; Pre-seed both with non-zero sentinels
        move.w  #$1234, $64(A0)         ; BLTAMOD = $1234
        move.w  #$FFA6, $66(A0)         ; BLTDMOD = $FFA6  (= -90)
        ; Verify seed
        move.w  $64(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00001234, D2
        bne     fail_amod_seed
        move.w  $66(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000FFA6, D2
        bne     fail_dmod_seed

        ; MOVE.L #$DEADBEEF, $DFF064 — high=$DEAD->BLTAMOD,
        ; low=$BEEF->BLTDMOD
        move.l  #$DEADBEEF, $64(A0)

        ; Verify BLTAMOD
        move.w  $64(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000DEAD, D2
        bne     fail_amod_split
        ; Verify BLTDMOD — this is the one that was broken
        move.w  $66(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BEEF, D2
        bne     fail_dmod_split

        ; -------- Test 2: K1.3 actual sequence — $00000002 to $DFF064 --------
        ; Pre-seed BLTDMOD with the stale -90 K1.3 sees in WB1.3 boot
        move.w  #$FFA6, $66(A0)         ; BLTDMOD = -90
        ; K1.3 writes longword $00000002 — intends BLTAMOD=0,
        ; BLTDMOD=$0002.
        move.l  #$00000002, $64(A0)
        ; Verify BLTAMOD = 0
        move.w  $64(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000000, D2
        bne     fail_k13_amod
        ; Verify BLTDMOD = 2 — pre-fix this stayed at $FFA6 (the bug)
        move.w  $66(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000002, D2
        bne     fail_k13_dmod

        ; -------- Test 3: BLTCMOD + BLTBMOD via MOVE.L $DFF060 --------
        ; Pre-seed
        move.w  #$5555, $60(A0)         ; BLTCMOD = $5555
        move.w  #$AAAA, $62(A0)         ; BLTBMOD = $AAAA
        ; MOVE.L #$CAFEBABE — high=$CAFE->BLTCMOD, low=$BABE->BLTBMOD
        move.l  #$CAFEBABE, $60(A0)
        ; Verify BLTCMOD
        move.w  $60(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000CAFE, D2
        bne     fail_cmod_split
        ; Verify BLTBMOD — broken pre-fix
        move.w  $62(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$0000BABE, D2
        bne     fail_bmod_split

        ; -------- Test 4: 16-bit writes still independent --------
        ; After the .L writes, individual .W writes must NOT cross-fire.
        move.w  #$0001, $64(A0)
        move.w  #$0002, $66(A0)
        move.w  $64(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000001, D2
        bne     fail_w_amod
        move.w  $66(A0), D2
        andi.l  #$0000FFFF, D2
        cmpi.l  #$00000002, D2
        bne     fail_w_dmod

        stop    #0                       ; pass

fail_amod_seed:   stop   #1
fail_dmod_seed:   stop   #2
fail_amod_split:  stop   #3
fail_dmod_split:  stop   #4
fail_k13_amod:    stop   #5
fail_k13_dmod:    stop   #6
fail_cmod_split:  stop   #7
fail_bmod_split:  stop   #8
fail_w_amod:      stop   #9
fail_w_dmod:      stop   #10
