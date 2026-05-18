; t66_boot_rom_ext.s — trampoline for the extended boot ROM.
; Identical to t63 except it lives in its own build dir to keep the
; ROM hex separate.
        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        move.l  $00000000, A7
        move.l  $00000004, A0
        jmp     (A0)

forever_bad:
        stop    #$BADF
        bra     forever_bad
