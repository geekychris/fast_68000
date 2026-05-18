; boot_rom.s — Amiga-style boot ROM.
;
; Loaded into the bus rom[] array which is mapped at $F80000-$FFFFFF.
; With OVL=1 at reset, the CPU's IF fetches at $400 route through OVL
; to rom[$100] = ROM offset $400.  We therefore put the boot trampoline
; AT offset $400 (where the CPU starts), and the canonical Amiga reset
; vectors at offsets 0/4 (so the trampoline can read SSP/PC from $0/$4
; via OVL data reads).
;
; Flow:
;   1. CPU resets at PC=$400, OVL=1.
;   2. IF at $400 -> rom[$100] = the .org $400 trampoline below.
;   3. Trampoline reads $0/$4 (OVL routes them to rom[0/4]), getting
;      SSP and entry PC.  Jumps to entry PC.
;   4. Entry PC = $F80008 -> rom[2] = boot_entry below.
;   5. Boot code: clear OVL, exercise chipset, halt 0.
;
; Halt codes:
;   $0000 = PASS
;   $BAD1 = magic value didn't round-trip through RAM
;   $BAD2 = INTENAR didn't reflect INTENA write

        .org $0

reset_ssp:
        .long $00080000                ; canonical SSP (will load into A7)
reset_pc:
        .long $00F80008                ; entry PC = ROM base + 8 = boot_entry

boot_entry:
        ; --- core gating: only core 0 (whatever A7 happens to be after
        ; the trampoline loaded it) continues; other cores park.  After
        ; the trampoline, A7 = the reset SSP = $80000 on every core.
        ; To differentiate cores we'd need a hardware ID register, which
        ; we don't have yet, so for now we just run on every core (they
        ; all do the same thing serially under the bus arbiter).

        ; 1. drop a magic value into known RAM so we can prove we got
        ;    past OVL clear and into normal RAM addressing.
        move.l  #$C0FFEE00, $00008000

        ; 2. set Paula INTENA's master-enable (INTEN=bit14) via SET.
        move.w  #$C000, $00DFF09A

        ; 3. clear OVL via CIA-A canonical addresses.
        move.b  $00BFE001, D0
        andi.b  #$FE, D0
        move.b  D0, $00BFE001
        move.b  #$01, $00BFE201

        ; 4. let the latch propagate (write to chip register isn't
        ;    visible to mem reads until next bus cycle).
        nop
        nop
        nop

        ; 5. magic value round-trip from RAM (now OVL is off).
        move.l  $00008000, D0
        cmpi.l  #$C0FFEE00, D0
        bne     fail_magic

        ; 6. confirm INTENAR reflects the INTEN bit.  The long-read at
        ; $DFF01C returns {INTENAR, INTREQR}; INTEN (bit 14) lives in the
        ; high 16 bits.
        move.l  $00DFF01C, D0
        andi.l  #$40000000, D0
        beq     fail_intena

        stop    #0
forever:
        bra     forever

fail_magic:
        stop    #$BAD1
        bra     fail_magic

fail_intena:
        stop    #$BAD2
        bra     fail_intena

;------------------------------------------------------------------
; Reset-vector trampoline.  Placed at ROM offset $400 so that the CPU's
; first IF fetches (PC=$400, OVL=1 -> rom[$100]) hit this code.
; The trampoline does what a 68000 in a real Amiga would do during its
; reset-vector fetch: read SSP from $0, PC from $4, then jump.
;------------------------------------------------------------------
        .org $400
reset_trampoline:
        move.l  $00000000, A7         ; A7 = ROM[0] (via OVL on data read)
        move.l  $00000004, A0         ; A0 = ROM[4]
        jmp     (A0)                  ; transfer to boot_entry
trampoline_dead:
        stop    #$BADF
        bra     trampoline_dead
