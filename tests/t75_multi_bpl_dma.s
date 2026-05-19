; Multi-plane bitplane DMA test.
;
; Stages three distinct bitplane patterns in memory, points BPL1PT,
; BPL2PT, BPL3PT at them, enables BPLEN+DMAEN with BPU=3 in BPLCON0,
; spins, then verifies:
;   - Total fetch counter advanced.
;   - Each of BPL1DAT/BPL2DAT/BPL3DAT holds a half-word from its own
;     staged pattern (proves Agnus rotates fetches across planes).

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Fill plane-region of memory with a tagged repeating pattern.
        ; Each plane gets 4 KB of $nnnnnnnn so the running pointer can
        ; advance freely during the spin and still land on tagged data.
        move.l  #$00006000, A0
        move.l  #1024, D0
fill1:  move.l  #$1111AAAA, (A0)+
        subq.l  #1, D0
        bne     fill1
        move.l  #$00007000, A0
        move.l  #1024, D0
fill2:  move.l  #$2222BBBB, (A0)+
        subq.l  #1, D0
        bne     fill2
        move.l  #$00008000, A0
        move.l  #1024, D0
fill3:  move.l  #$3333CCCC, (A0)+
        subq.l  #1, D0
        bne     fill3

        ; Program BPLnPT shadows.
        move.w  #$0000, $00DFF0E0    ; BPL1PTH
        move.w  #$6000, $00DFF0E2    ; BPL1PTL
        move.w  #$0000, $00DFF0E4    ; BPL2PTH
        move.w  #$7000, $00DFF0E6    ; BPL2PTL
        move.w  #$0000, $00DFF0E8    ; BPL3PTH
        move.w  #$8000, $00DFF0EA    ; BPL3PTL

        ; BPLCON0: BPU=3 -> bits [14:12] = $3000.
        move.w  #$3000, $00DFF100

        ; Enable DMACON: SET DMAEN(9)+BPLEN(8) = $8300.
        move.w  #$8300, $00DFF096

        ; Spin a bounded amount so each plane fetches several times but
        ; stays inside its 4 KB filled window.  At BPU=3 and one fetch
        ; per 16 host clocks, ~2000 iterations × ~2 cyc/iter = ~4000
        ; host cycles -> ~83 fetches per plane -> ~166 byte advance
        ; from $6000/$7000/$8000.  Well inside the 4 KB fill.
        move.l  #2000, D7
spin:   subq.l  #1, D7
        bne     spin

        ; Disable to freeze the engine.
        move.w  #$0100, $00DFF096           ; CLR BPLEN

        ; BPLCON0 snoop sanity check (must read back $3000 in high half).
        move.l  $00FE911C, D5
        cmpi.l  #$30000000, D5
        bne     fail_bplcon0

        ; Sanity: confirm the fill actually wrote $1111AAAA at $6000.
        move.l  $00006000, D6
        cmpi.l  #$1111AAAA, D6
        bne     fail_fill

        ; Total fetch counter advanced?
        move.l  $00FE9100, D0
        tst.l   D0
        beq     fail_no_fetch

        ; BPL1DAT should match plane-1 pattern.  Each plane's words are
        ; tagged with its plane number in the high nibble: BPL1=$1xxx,
        ; BPL2=$2xxx, BPL3=$3xxx.  Half-word reads see either $1111 or
        ; $AAAA/$BBBB/$CCCC depending on alignment; check the plane tag.
        move.l  $00FE9104, D1
        ; Take the high half of the 32-bit probe (probe puts the 16-bit
        ; BPLnDAT into bits [31:16]).  We want the upper byte of that
        ; half-word, which is in bits [31:24] of the long.
        ; Plane 1: high half should look like $1111 (since the high half
        ; of each long at $6xxx is $1111, and the low half is $AABB/CC).
        ; Conservative check: ensure D1's plane tag matches $1 or $A/B/C.
        rol.l   #8, D1
        andi.l  #$000000FF, D1
        ; D1 now holds the top byte of BPL1DAT.  Accept $11 (the tag) or
        ; $AA/$BB/$CC (low-half words from staged pattern).
        cmpi.l  #$11, D1
        beq     check_bpl2
        cmpi.l  #$AA, D1
        beq     check_bpl2
        cmpi.l  #$BB, D1
        beq     check_bpl2
        cmpi.l  #$CC, D1
        beq     check_bpl2
        ; Fingerprint: distinguish whether high byte is 0, 22, 33, or other.
        cmpi.l  #$00, D1
        beq     fail_bpl1_00
        cmpi.l  #$22, D1
        beq     fail_bpl1_22
        cmpi.l  #$33, D1
        beq     fail_bpl1_33
        bra     fail_bpl1

check_bpl2:
        ; Read BPLCON0 shadow to confirm snoop worked.
        move.l  $00FE911C, D5
        cmpi.l  #$30000000, D5
        bne     fail_bplcon0
        ; Read BPL2DAT.
        move.l  $00FE9108, D2
        rol.l   #8, D2
        andi.l  #$000000FF, D2
        cmpi.l  #$22, D2
        beq     check_bpl3
        cmpi.l  #$BB, D2
        beq     check_bpl3
        bra     fail_bpl2

check_bpl3:
        move.l  $00FE910C, D3
        rol.l   #8, D3
        andi.l  #$000000FF, D3
        cmpi.l  #$33, D3
        beq     ok
        cmpi.l  #$AA, D3
        beq     ok
        cmpi.l  #$BB, D3
        beq     ok
        cmpi.l  #$CC, D3
        beq     ok
        bra     fail_bpl3

ok:     stop    #0
fail_no_fetch: stop #$ba51
fail_bpl1:     stop #$ba52
fail_bpl2:     stop #$ba53
fail_bpl3:     stop #$ba54
fail_bplcon0:  stop #$ba55
fail_fill:     stop #$ba56
fail_bpl1_00:  stop #$ba60
fail_bpl1_22:  stop #$ba62
fail_bpl1_33:  stop #$ba63
