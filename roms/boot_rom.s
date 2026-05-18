; boot_rom.s — Kickstart-style boot ROM image.
;
; Loaded into the bus model's rom[] array at $F80000-$FFFFFF.  With
; OVL_RESET=1, the first two longs at ROM offset 0/4 are visible at
; $0/$4 as the 68000 reset vector (SSP + PC).
;
; The companion test (tests/t63_boot_rom.s) lives in mem[] at $400 and
; is a tiny trampoline that reads $0/$4 (which OVL routes to ROM) and
; jumps to the entry PC — letting the boot code actually run from ROM.
;
; This ROM does what a real Kickstart would do in its very first
; instructions:
;   1. set up an exception vector base
;   2. write to a chipset register so we have a side-effect to observe
;   3. clear OVL via CIA-A PRA (the canonical Amiga way)
;   4. read a chipset register back to confirm it stuck
;   5. halt with code 0 (pass) or $BADx (fail at known step)
;
; All addresses below ROM are absolute references to chipset MMIO;
; intra-ROM control flow is PC-relative so the image is position-
; independent (the linker / asm68k doesn't know we live at $F80000).

        .org $0

reset_ssp:
        .long $00010000        ; SSP (will be loaded into A7 by trampoline)
reset_pc:
        .long $00F80008        ; entry PC = ROM base + 8 = first instruction below

boot_entry:
        ; --- 1. drop a magic value into a known RAM cell so the test
        ;        can confirm we got here ---
        move.l  #$C0FFEE00, $00008000

        ; --- 2. write Paula's master interrupt enable (INTENA) ---
        ; INTENA = $DFF09A. Bit 15 = SET/CLR (1 = set), bit 14 = INTEN.
        ; Writing #$C000 sets the INTEN master enable.
        move.w  #$C000, $00DFF09A

        ; --- 3. clear OVL via CIA-A canonical addresses ---
        move.b  $00BFE001, D0
        andi.b  #$FE, D0
        move.b  D0, $00BFE001         ; PRA bit 0 = 0
        move.b  #$01, $00BFE201        ; DDRA bit 0 = 1 (output)

        ; --- 4. let the OVL latch propagate (a handful of cycles) ---
        nop
        nop
        nop

        ; --- 5. confirm we can read mem[$8000] back as the magic value ---
        move.l  $00008000, D0
        cmpi.l  #$C0FFEE00, D0
        bne     fail_magic

        ; --- 6. check Paula INTENAR reads back the bit we set ---
        ; INTENAR is the read-mirror of INTENA at $DFF01C (low word
        ; of the combined {INTENAR, INTREQR} long).
        move.l  $00DFF01C, D0
        ; INTEN bit (14) should be set in the low 16 bits of d0.
        andi.l  #$4000, D0
        beq     fail_intena

        ; --- 7. pass ---
        stop    #0
forever:
        bra     forever

fail_magic:
        stop    #$BAD1
        bra     fail_magic

fail_intena:
        stop    #$BAD2
        bra     fail_intena
