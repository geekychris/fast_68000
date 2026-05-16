; Cache coherence: producer/consumer ping-pong via shared memory.
; Core 0 increments value at $3000 then sets flag at $3008 to 1.
; Core 1 spins on flag, reads value, multiplies by 7, stores back at $3004,
; sets flag to 2.
; Core 0 waits for flag=2 then verifies $3004 == 7. Both stop with 0 on pass.
        .org $400
        move.l A7, D7
        cmp.l #$4000, D7
        beq core0_entry
        bra core1_entry

; --- core 0 ---
core0_entry:
        ; Initialize shared region.
        moveq #0, D0
        move.l D0, $3000
        move.l D0, $3004
        move.l D0, $3008

        ; Produce: write 1 to $3000, then 1 to flag.
        moveq #1, D0
        move.l D0, $3000
        move.l D0, $3008

        ; Wait for flag == 2.
c0_wait:
        move.l $3008, D1
        cmp.l #2, D1
        bne c0_wait

        ; Verify $3004 == 7.
        move.l $3004, D2
        cmp.l #7, D2
        beq c0_pass
        stop #$dead
c0_pass:
        stop #0

; --- core 1 ---
core1_entry:
c1_wait:
        move.l $3008, D1
        cmp.l #1, D1
        bne c1_wait

        ; Read value, multiply by 7, store at $3004.
        move.l $3000, D0
        move.l D0, D2
        moveq #6, D3
c1_mul:
        add.l D2, D0
        subq.l #1, D3
        bne c1_mul

        move.l D0, $3004

        ; Set flag to 2.
        moveq #2, D0
        move.l D0, $3008

        stop #0
