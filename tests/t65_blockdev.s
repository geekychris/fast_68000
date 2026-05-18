; Block-device test.  Programs BLKSRC/BLKDST/BLKCNT/BLKCMD to copy 1
; sector (512 bytes = 128 longs) from disk image sector 0 to RAM at
; $00009000, then verifies the first long matches the magic value
; the testbench pre-loaded at disk[0] = $CAFEBEEF.
;
; The disk image hex is loaded into the bus's disk[] array via the
; DISK_HEXFILE parameter at build time (see make test-blockdev).
        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Program the block-device control regs.
        move.l  #0,            $00FE8000   ; BLKSRC: sector 0
        move.l  #$00009000,    $00FE8004   ; BLKDST: 36KB into RAM
        move.l  #1,            $00FE8008   ; BLKCNT: 1 sector
        move.l  #1,            $00FE800C   ; BLKCMD: start

        ; Poll BLKCMD until it reads 0 (busy clears).
wait_done:
        move.l  $00FE800C, D0
        tst.l   D0
        bne     wait_done

        ; Check first long at $9000 matches what the disk image contained
        ; at sector 0 offset 0.  Test fixture: $CAFEBEEF.
        move.l  $00009000, D1
        cmpi.l  #$CAFEBEEF, D1
        bne     fail_long0

        ; Second long should be $DEADCAFE.
        move.l  $00009004, D2
        cmpi.l  #$DEADCAFE, D2
        bne     fail_long1

        stop    #0
forever:
        bra     forever
fail_long0: stop #$BAD1
fail_long1: stop #$BAD2
