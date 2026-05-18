; Kickstart ROM region at $F80000-$FFFFFF: routing-only test.
;
; Without a ROM hex file loaded, rom[] is zero.  Verifies:
;   - Writes to ROM_BASE are silently dropped (no crash, RAM not corrupted).
;   - Reads from ROM_BASE return 0 (the zero-init contents of rom[]).
;
; OVL is off by default in the test build (OVL_RESET=0), so low-RAM reads
; still hit the main mem array.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Read ROM at $F80000 -- expect 0 (no MEM_HEXFILE_ROM loaded).
        move.l  $00F8_0000, D0
        cmpi.l  #0, D0
        bne     fail_1

        ; Write a known sentinel to RAM, then try to clobber it via the
        ; ROM address.  RAM must remain untouched.
        move.l  #$DEAD_BEEF, $0000_9000
        ; The write below targets a ROM address; the bus must drop it.
        move.l  #$1234_5678, $00F8_0010
        move.l  $0000_9000, D0
        cmpi.l  #$DEAD_BEEF, D0
        bne     fail_2

        ; ROM read still 0 after the dropped write.
        move.l  $00F8_0010, D0
        cmpi.l  #0, D0
        bne     fail_3

        stop    #0

fail_1: stop #$DE01
fail_2: stop #$DE02
fail_3: stop #$DE03
