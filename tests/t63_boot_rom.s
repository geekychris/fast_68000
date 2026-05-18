; t63_boot_rom.s — boot-from-ROM trampoline.
;
; Sits in mem[$400] (the hardware reset PC).  At reset OVL=1, so any
; data read at $0/$4 transparently reads from rom[0]/rom[4] — which
; hold the Amiga-canonical reset vector (SSP and PC).
;
; We load the vector and jump into ROM, exactly like the 68000 itself
; would if it actually executed a reset-vector fetch.  Once in ROM the
; real boot code (roms/boot_rom.s) runs; it eventually does its own
; STOP #imm and halt code makes its way back here.
;
; Core 0 only; other cores park immediately so they don't race the
; OVL machinery.

        .org $400

start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Read reset SSP from $0 (= rom[0] via OVL) and load into A7.
        move.l  $00000000, A7
        ; Read entry PC from $4 (= rom[4] via OVL).
        move.l  $00000004, A0
        ; Jump into ROM.  From here on we are running the boot ROM.
        jmp     (A0)

        ; Never reached.  If the jump fell through somehow we trap.
forever_bad:
        stop    #$BADF
        bra     forever_bad
