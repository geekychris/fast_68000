; t111 — DMA-write cache-snoop coherence.
;
; Reproduces a class of bug seen during K1.3 trackdisk bring-up: the
; bus's DMA writes bypass the CPU's bus master port, and the D-cache
; was never notified of those writes.  Result: the CPU would read a
; stale cached value and miss the DMA-updated content.
;
; Test path:
;   1. CPU stores a sentinel ($11111111) at $9000 and reads it back so
;      the D-cache caches the sentinel for that address.
;   2. DMA-copy sector 0 from disk[] to $9000 (overwrites with
;      $CAFEBEEF, the value adf2mfm puts at disk[0]).
;   3. CPU reads $9000 again.  Without the cache-snoop fix, this read
;      returns the cached sentinel and the test FAILs.  With the fix,
;      the snoop bus picks up the DMA write and the cache line is
;      invalidated -- the read sees the DMA's $CAFEBEEF.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Stage sentinel at $9000 + first few longwords so the D-cache
        ; line covers the DMA destination prefix.
        move.l  #$11111111, $00009000
        move.l  #$22222222, $00009004
        move.l  #$33333333, $00009008
        move.l  #$44444444, $0000900C

        ; Read them back -- forces the cache to fill with the sentinel.
        ; (Verilator's D-cache is direct-mapped; the read after the
        ; write is what actually loads the line.)
        move.l  $00009000, D5
        cmpi.l  #$11111111, D5
        bne     fail_seed
        move.l  $00009004, D6
        cmpi.l  #$22222222, D6
        bne     fail_seed
        move.l  $00009008, D7
        cmpi.l  #$33333333, D7
        bne     fail_seed
        move.l  $0000900C, D4
        cmpi.l  #$44444444, D4
        bne     fail_seed

        ; Program block-device DMA: copy sector 0 (= disk[0..127] longs
        ; = first 512 bytes of disk image) to $9000.  Disk image is
        ; tests/disk_test.hex (loaded via DISK_HEXFILE).
        move.l  #0,          $00FE8000   ; BLKSRC: sector 0
        move.l  #$00009000,  $00FE8004   ; BLKDST
        move.l  #1,          $00FE8008   ; BLKCNT: 1 sector
        move.l  #1,          $00FE800C   ; BLKCMD: start

        ; Wait for BLKCMD to clear (busy -> idle).
wait_dma:
        move.l  $00FE800C, D0
        tst.l   D0
        bne     wait_dma

        ; Re-read $9000 and check it matches disk[0] = $CAFEBEEF.  If
        ; the snoop bus isn't broadcasting DMA writes, the read returns
        ; the stale cached sentinel $11111111 and we fail.
        move.l  $00009000, D1
        cmpi.l  #$CAFEBEEF, D1
        bne     fail_long0

        move.l  $00009004, D2
        cmpi.l  #$DEADCAFE, D2
        bne     fail_long1

        move.l  $00009008, D3
        cmpi.l  #$12345678, D3
        bne     fail_long2

        ; Pass.
        stop    #0
forever_pass:
        bra     forever_pass

fail_seed:
        stop    #$BAD1
fail_long0:
        stop    #$BAD2
fail_long1:
        stop    #$BAD3
fail_long2:
        stop    #$BAD4
