; boot_rom_ext.s — extended Kickstart-style stub.
;
; Loaded at $F80000 with reset SSP/PC at offsets 0/4.  Companion
; test (tests/t66_boot_rom_ext.s) is the same $400 trampoline.
;
; Walks the chipset more thoroughly than the basic boot_rom.s:
;
;   1. Land in ROM via OVL+trampoline.
;   2. Clear OVL via CIA-A.
;   3. Read Agnus VHPOSR a few times; assert beam advanced.
;   4. Set DMACON BPLEN|DMAEN via SET, read back via DMACONR.
;   5. Read SERDATR (should be $6000 = TSRE | TBE).
;   6. Read POTGOR (should be $FFFF).
;   7. Read CIA-A TOD twice; assert it advanced.
;   8. Run a 4-word blitter copy in RAM, verify destination.
;   9. Halt 0.
;
; Any step that fails halts with $BAD1..$BAD9.

        .org $0

reset_ssp:
        .long $00080000
reset_pc:
        .long $00F80008

boot_entry:
        ; 1. Magic into low RAM to show we got here.
        move.l  #$BADC0FFE, $00008000

        ; 2. Clear OVL via CIA-A.
        move.b  $00BFE001, D0
        andi.b  #$FE, D0
        move.b  D0, $00BFE001
        move.b  #$01, $00BFE201
        nop
        nop
        nop

        ; 3. VHPOSR advances.
        move.l  $00DFF006, D0           ; t0
        ; burn a few cycles
        moveq   #16, D7
spin1:  subq.l  #1, D7
        bne     spin1
        move.l  $00DFF006, D1           ; t1
        cmp.l   D0, D1
        beq     fail_vhpos

        ; 4. DMACON SET: BPLEN|DMAEN ($0300) with bit 15 = SET = $8300.
        move.w  #$8300, $00DFF096
        move.l  $00DFF002, D2
        andi.l  #$0300, D2
        cmpi.l  #$0300, D2
        bne     fail_dmacon

        ; 5. SERDATR canned value.
        move.l  $00DFF018, D3
        andi.l  #$0000FFFF, D3
        cmpi.l  #$6000, D3
        bne     fail_serdatr

        ; 6. POTGOR canned value.
        move.l  $00DFF016, D4
        andi.l  #$0000FFFF, D4
        cmpi.l  #$FFFF, D4
        bne     fail_potgor

        ; 7. CIA-A TOD advances.  TOD high byte at $BFE801, mid $BFE901,
        ; low $BFEA01 with the canonical 256-byte stride.  Read low byte
        ; twice with a delay in between.
        moveq   #0, D5
        moveq   #0, D6
        move.b  $00BFE801, D5           ; TOD-LO sample 0
        moveq   #100, D7
spin2:  subq.l  #1, D7
        bne     spin2
        move.b  $00BFE801, D6           ; TOD-LO sample 1
        cmp.l   D5, D6
        beq     fail_tod

        ; 8. Blitter copy: 4 words from $00006000 to $00006100.
        move.l  #$11112222, $00006000
        move.l  #$33334444, $00006004
        move.l  #$0, $00006100
        move.l  #$0, $00006104
        move.l  #$F0000009, $00FE0000   ; BLTCON: LF=$F0, USEA|USED
        move.l  #$0000FFFF, $00FE0004   ; BLTAFWM = all ones
        move.l  #$0000FFFF, $00FE0008   ; BLTALWM = all ones
        move.l  #$00006000, $00FE000C   ; BLTAPT
        move.l  #$00006100, $00FE0018   ; BLTDPT
        move.l  #0,         $00FE001C   ; BLTAMOD
        move.l  #0,         $00FE0028   ; BLTDMOD
        move.l  #$00000044, $00FE0038   ; BLTSIZE: 1 row x 4 words
blt_wait:
        move.l  $00FE003C, D7
        andi.l  #1, D7
        bne     blt_wait
        ; Verify
        move.l  $00006100, D7
        cmpi.l  #$11112222, D7
        bne     fail_blt
        move.l  $00006104, D7
        cmpi.l  #$33334444, D7
        bne     fail_blt

        ; 9. Pass.
        stop    #0
forever:
        bra     forever

fail_vhpos:   stop #$BAD1
              bra  fail_vhpos
fail_dmacon:  stop #$BAD2
              bra  fail_dmacon
fail_serdatr: stop #$BAD3
              bra  fail_serdatr
fail_potgor:  stop #$BAD4
              bra  fail_potgor
fail_tod:     stop #$BAD5
              bra  fail_tod
fail_blt:     stop #$BAD6
              bra  fail_blt
