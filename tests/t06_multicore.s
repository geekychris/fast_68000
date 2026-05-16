; Multi-core mutex via TAS: two cores each do 5 increments to a shared
; counter under a TAS-protected lock. Final counter must be 10. Each core
; then spins waiting for the counter to reach 10 before stopping.
;
; Shared addresses:
;   $2000  counter (long)
;   $2004  lock (byte; we use the high byte of the long word at $2004)
        .org $400

        ; Initialize once. Both cores would race here; instead we let core 0
        ; (SP=0x4000) do the init and core 1 (SP=0x5000) skip it.
        move.l A7, D7
        cmp.l #$4000, D7
        bne skip_init

        moveq #0, D0
        move.l D0, $2000      ; counter = 0
        move.l D0, $2004      ; lock = 0
skip_init:

        ; Both cores: do 5 increments under TAS lock.
        lea $2004, A0         ; lock
        lea $2000, A1         ; counter
        moveq #5, D6          ; loop count
work_loop:
acq:
        tas (A0)              ; atomic test-and-set high bit on byte at (A0)
        bne acq               ; if already set, retry

        ; critical section
        move.l (A1), D1
        addq.l #1, D1
        move.l D1, (A1)

        ; release: write 0 to the byte
        moveq #0, D2
        move.l D2, (A0)       ; this writes a long; the high byte gets 0

        subq.l #1, D6
        bne work_loop

        ; Wait for counter to reach 10 (the other core may still be running).
wait_done:
        move.l (A1), D0
        cmp.l #10, D0
        bne wait_done

        stop #0
