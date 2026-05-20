; Cache must bypass on unaligned .L reads -- bus does the splice.
;
; The D-cache stores 32-bit aligned longwords indexed by addr[9:2].  An
; unaligned .L read at addr X (X[1:0]==10) should return the spliced
; longword: bytes 2..3 of mem[X & ~3] || bytes 0..1 of mem[(X & ~3) + 4].
; The bus's read-side unaligned-.L assembly path does this.
;
; The cache, by default, returns data[idx] on a hit -- which is the
; ALIGNED longword at (X & ~3), the WRONG half for an unaligned access.
; So an unaligned .L read of a previously-cached aligned line silently
; returned the wrong 4 bytes.
;
; Kickstart 1.3 trips this on its first task switch: the supervisor stack
; ($FFE8) gets cached during a normal access, then a fake-RTE frame is
; built by pushing PC.L (unaligned at $FFEA) and SR.W (aligned at $FFE8).
; The cached aligned longword now matches "$00F8" SR | "$00F8" leftover
; halves of the PC push.  When RTE pops PC.L from $FFEA, the cache hits
; and returns the *aligned* longword at $FFE8 ($00F800F8) instead of the
; correctly-spliced PC.  Boot dies on the bad redirect.
;
; This test prepares an aligned longword at $1000, primes the cache by
; reading it, then writes an unaligned longword at $1002 and verifies
; the unaligned .L read returns the correct value.
;
; Pass = halt 0.  Fail = stop $BADn.

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Prepare the buffer.
        move.l  #$AAAAAAAA, $00001000
        move.l  #$BBBBBBBB, $00001004
        move.l  #$CCCCCCCC, $00001008

        ; Prime the cache by reading mem[$1000] aligned.  The cache now
        ; holds data[idx($1000)] = $AAAAAAAA.
        move.l  $00001000, D0

        ; Write an unaligned longword at $1002.  This splits across mem
        ; entries and invalidates the cache line for $1000 (per the
        ; existing unaligned-.L write fix).
        move.l  #$12345678, $00001002

        ; Now read aligned mem[$1000] back -- after the unaligned write,
        ; mem[$1000].W high half (bytes 0,1) should still be $AAAA and
        ; mem[$1000].W low half (bytes 2,3) should be $1234.
        move.l  $00001000, D1
        cmpi.l  #$AAAA1234, D1
        bne     fail_align

        ; Read unaligned .L from $1002.  Should return $12345678 -- the
        ; value we just wrote.
        move.l  $00001002, D2
        cmpi.l  #$12345678, D2
        bne     fail_unalign

        ; Read unaligned .L from $1006 (next unaligned, into mem[$1004]).
        ; mem[$1004] now: high half = bytes 0,1 = $5678 (low half of our
        ; written long), low half = $BBBB (unchanged).
        ; mem[$1008] = $CCCCCCCC.
        ; Unaligned .L at $1006: bytes 2,3 of mem[$1004] || bytes 0,1 of
        ; mem[$1008] = $BBBB || $CCCC = $BBBBCCCC.
        move.l  $00001006, D3
        cmpi.l  #$BBBBCCCC, D3
        bne     fail_unalign2

        stop    #0
forever: bra forever
fail_align:   stop #$BAD1
fail_unalign: stop #$BAD2
fail_unalign2: stop #$BAD3
