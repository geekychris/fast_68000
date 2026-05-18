; fake_kickstart.s — synthetic stub ROM that mirrors what a real
; Kickstart 1.x does in its first thousand instructions.  Goal: prove
; the chipset surface is wide enough that nothing hangs during boot.
;
; Real Kickstart's first phase:
;   1.  Read VHPOSR — confirm Agnus is alive.
;   2.  Disable all DMA via DMACON CLR.
;   3.  Disable all interrupts via INTENA CLR.
;   4.  Clear pending interrupts via INTREQ CLR.
;   5.  Clear OVL via CIA-A.
;   6.  Disable audio channels via AUDENA = 0 (DMACON bits already off).
;   7.  Set Paula INTREQ DSKBLK (test the SET path).
;   8.  Probe Zorro autoconfig at $E80000 — expect $FF (no card).
;   9.  Set up vector base at $0..$3FF (we just leave it zero since
;       we're not actually generating exceptions in this stub).
;  10.  Set INTENA master-enable + a couple specific bits via SET.
;  11.  Read CIA-A TOD-LO, wait, read again, assert advance.
;  12.  Program CIA-A timer A for a 50 Hz tick (continuous mode).
;  13.  Read VHPOSR a second time, assert beam has moved.
;  14.  Halt 0.
;
; Failure halts: $BAD1..$BADC depending on which step fails, plus
; $BADF for the trampoline-fell-through path.

        .org $0
reset_ssp:  .long $00080000
reset_pc:   .long $00F80008

boot_entry:
        ; 1. VHPOSR alive.
        move.l  $00DFF006, D0
        ; just read it; we'll compare to a later sample at step 13.
        move.l  D0, $00007F00          ; stash for later

        ; 2. DMACON CLR everything: write $7FFF (bit 15=0 = CLR, bits 14..0
        ; all set so every channel disabled).
        move.w  #$7FFF, $00DFF096

        ; 3. INTENA CLR everything ($7FFF, bit15=0 = CLR).
        move.w  #$7FFF, $00DFF09A

        ; 4. INTREQ CLR everything.
        move.w  #$7FFF, $00DFF09C

        ; 5. Clear OVL via CIA-A.
        move.b  $00BFE001, D0
        andi.b  #$FE, D0
        move.b  D0, $00BFE001
        move.b  #$01, $00BFE201
        nop
        nop
        nop

        ; 6. AUDENA = 0 (legacy enable register at slv_addr $0 of Paula).
        move.l  #$0, $00DFF000

        ; 7. SET INTREQ DSKBLK (bit 1) via SET: $8000 | $0002 = $8002.
        move.w  #$8002, $00DFF09C
        ; Verify INTREQR low half has bit 1 set.
        move.l  $00DFF01C, D0
        andi.l  #$00000002, D0
        beq     fail_intreq_set

        ; 8. Probe autoconfig: $E80000 should read $FF (no card).
        move.b  $00E80000, D0
        cmpi.b  #$FF, D0
        bne     fail_acf

        ; 9. (skipped — vector table already zero)

        ; 10. Set INTENA master-enable + DSKBLK enable: $8000 | $4000 | $0002
        ;     = $C002.
        move.w  #$C002, $00DFF09A
        move.l  $00DFF01C, D0
        andi.l  #$40000000, D0
        beq     fail_intena

        ; 11. CIA-A TOD advances.
        moveq   #0, D1
        moveq   #0, D2
        move.b  $00BFE801, D1          ; TOD-LO sample 0
        move.l  #4000, D7
tod_wait:
        subq.l  #1, D7
        bne     tod_wait
        move.b  $00BFE801, D2          ; TOD-LO sample 1
        cmp.l   D1, D2
        beq     fail_tod

        ; 12. CIA-A timer A program: reload = $1F40 (8000 = 100 Hz at 800 kHz).
        ;     Write TALO=$40 then TAHI=$1F; start in continuous mode by
        ;     setting CRA bits 0 (START) + clearing bit 3 (RUNMODE=continuous).
        move.b  #$40, $00BFE401        ; TALO
        move.b  #$1F, $00BFE501        ; TAHI
        move.b  #$01, $00BFEE01        ; CRA = START | continuous

        ; 13. VHPOSR has moved.
        move.l  $00007F00, D3          ; original sample
        move.l  $00DFF006, D4          ; new sample
        cmp.l   D3, D4
        beq     fail_vhpos_static

        ; 14. PASS.
        stop    #0
forever:
        bra     forever

fail_intreq_set:  stop #$BAD7
                  bra fail_intreq_set
fail_acf:         stop #$BAD8
                  bra fail_acf
fail_intena:      stop #$BAD9
                  bra fail_intena
fail_tod:         stop #$BADA
                  bra fail_tod
fail_vhpos_static:stop #$BADB
                  bra fail_vhpos_static

        ;-----------------------------------------------------------
        ; Reset trampoline at $400 (where CPU resets with OVL on).
        ;-----------------------------------------------------------
        .org $400
reset_trampoline:
        move.l  $00000000, A7
        move.l  $00000004, A0
        jmp     (A0)
trampoline_dead:
        stop    #$BADF
        bra     trampoline_dead
