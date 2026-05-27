; Slow-RAM (Agnus trapdoor) at $C00000-$C7FFFF must support:
;   1. Aligned .L write + readback at $C00000-range
;   2. Unaligned .L write at $C00xxx (addr[1:0]==10) splits across slots
;   3. Unaligned .L read assembles correctly across slots
;   4. Byte write + read with correct byte-enable
;   5. Reads from un-written slow-RAM return $0 (zero-initialized)
;
; This protects the K1.3 boot fix: K1.3 writes trampolines as
; "MOVE.L #imm, $C001EE" (addr[1:0]=10, be=1111) and reads JMP-abs.l
; immediates at unaligned slow-RAM addresses.  Without the unaligned-.L
; split/assemble path in m68k_bus.v's slow-RAM mux, those writes stuffed
; all 4 bytes into ONE slot.  Boot crashed with LINEF.
;
; Pass = halt 0.  Fail = stop $BADn.

        .org $400
start:
        ; Single-core test guard: core1 idles.
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; --- 1. Aligned .L write/read in slow RAM ---
        move.l  #$12345678, $00C00000
        move.l  $00C00000, D0
        cmpi.l  #$12345678, D0
        bne     fail_aligned

        ; Far-end aligned access at $C7FFFC (last aligned long).
        move.l  #$DEADBEEF, $00C7FFFC
        move.l  $00C7FFFC, D1
        cmpi.l  #$DEADBEEF, D1
        bne     fail_far

        ; --- 2. Unaligned .L write at $C00002 (addr[1:0]=10) ---
        ; Should split across slots [$C00000] and [$C00004].
        ; After write: slot0 = $1234 1234 (low half), slot1 = $5678 ????
        ; (high half).  Pre-existing slot0 = $12345678; slot1 = $0 (clean).
        move.l  #$00000000, $00C00000
        move.l  #$00000000, $00C00004
        move.l  #$AABBCCDD, $00C00002
        ; Aligned read of slot0: should be $0000 AABB.
        move.l  $00C00000, D2
        cmpi.l  #$0000AABB, D2
        bne     fail_unal_w_lo
        ; Aligned read of slot1: should be $CCDD 0000.
        move.l  $00C00004, D3
        cmpi.l  #$CCDD0000, D3
        bne     fail_unal_w_hi

        ; --- 3. Unaligned .L read at $C00002 ---
        ; Should return bytes 2,3 of slot0 || bytes 0,1 of slot1.
        ; = $AABB || $CCDD = $AABBCCDD.
        move.l  $00C00002, D4
        cmpi.l  #$AABBCCDD, D4
        bne     fail_unal_r

        ; --- 4. Byte write/read ---
        move.l  #$00000000, $00C00010
        move.b  #$5A, $00C00010
        move.b  $00C00010, D5
        andi.l  #$FF, D5
        cmpi.l  #$5A, D5
        bne     fail_byte
        ; Other bytes in the same slot must still be zero.
        move.l  $00C00010, D6
        cmpi.l  #$5A000000, D6
        bne     fail_byte_other

        ; --- 5. Untouched slow-RAM returns $0 ---
        move.l  $00C40000, D7
        cmpi.l  #$00000000, D7
        bne     fail_zero

        stop    #0
forever: bra forever

fail_aligned:    stop #$BAD1
fail_far:        stop #$BAD2
fail_unal_w_lo:  stop #$BAD3
fail_unal_w_hi:  stop #$BAD4
fail_unal_r:     stop #$BAD5
fail_byte:       stop #$BAD6
fail_byte_other: stop #$BAD7
fail_zero:       stop #$BAD8
