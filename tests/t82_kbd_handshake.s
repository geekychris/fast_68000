; Keyboard scan-code byte arrival -> CIA-A SP interrupt -> Paula INTREQ.PORTS
; -> IPL 2 -> auto-vector $68 (vec 26) handler -> read SDR + handshake.
;
; Verifies the full chain end-to-end and that a second injected byte
; fires the handler again after the handshake.  The handshake itself
; is modeled as CRA[6] write (SPMODE = 1 then 0); our CIA latches the
; bits but doesn't otherwise act on them -- enough to keep Kickstart's
; keyboard.device happy.

        .org $68                        ; vector 26 = IRQ-2 autovector
        .long kbd_handler

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; Zero handler counter + byte log.
        move.l  #0, $00030000           ; counter
        move.l  #0, $00030004           ; first byte
        move.l  #0, $00030008           ; second byte

        ; Unmask CPU IRQs.
        andi.w  #$F8FF, SR

        ; CIA-A ICR: enable SP source (bit 3).  Bit 7 = SET.
        move.b  #$88, $00BFED01

        ; Paula INTENA: INTEN (14) + PORTS (3).
        move.w  #$C008, $00DFF09A

        ; Inject first byte.
        move.b  #$5A, $00FE9000

        ; Spin until counter goes to 1.
        moveq   #0, D3
wait1:
        addq.l  #1, D3
        cmpi.l  #$10000, D3
        bge     fail_timeout
        move.l  $00030000, D0
        cmpi.l  #1, D0
        bne     wait1

        ; Inject second byte.
        move.b  #$7B, $00FE9000

        ; Spin until counter goes to 2.
        moveq   #0, D3
wait2:
        addq.l  #1, D3
        cmpi.l  #$10000, D3
        bge     fail_timeout
        move.l  $00030000, D0
        cmpi.l  #2, D0
        bne     wait2

        ; Verify the bytes.
        move.l  $00030004, D0
        cmpi.l  #$5A, D0
        bne     fail_byte1
        move.l  $00030008, D0
        cmpi.l  #$7B, D0
        bne     fail_byte2

        stop    #0

fail_timeout: stop #$D801
fail_byte1:   stop #$D802
fail_byte2:   stop #$D803

; ---- IRQ 2 (PORTS) handler ----
kbd_handler:
        move.l  D0, -(A7)
        move.l  D1, -(A7)

        ; Read CIA-A ICR to clear pending.
        move.b  $00BFED01, D0

        ; Read scan code from SDR.
        move.b  $00BFEC01, D1
        andi.l  #$FF, D1

        ; Bump counter; route first/second byte to slots.
        move.l  $00030000, D0
        addq.l  #1, D0
        move.l  D0, $00030000

        cmpi.l  #1, D0
        bne     not_first
        move.l  D1, $00030004
        bra     handshake
not_first:
        move.l  D1, $00030008
handshake:
        ; SPMODE = 1, then = 0.  Kickstart's "I received your byte" pattern.
        move.b  #$40, $00BFEE01
        nop
        nop
        move.b  #$00, $00BFEE01

        ; Clear INTREQ.PORTS.
        move.w  #$0008, $00DFF09C

        move.l  (A7)+, D1
        move.l  (A7)+, D0
        rte
