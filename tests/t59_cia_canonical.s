; CIA-A and CIA-B routing via Amiga-canonical addresses.
;
; Real Amiga: CIA-A at $BFE001 (registers 256 bytes apart, odd byte addresses),
; CIA-B at $BFD000 (even byte addresses).  Our bus accepts these aliases as
; well as the legacy $00FE_04xx / $00FE_05xx addresses.  Verify that a write
; to one alias is visible at the corresponding register through the other.

        .org $400
start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        ; DDRA = all-output on CIA-A so reads of PRA return our written value.
        move.b  #$FF, $00FE_0402
        ; Write PRA via canonical CIA-A address $BFE001 (register 0).
        move.b  #$A5, $00BF_E001
        ; Read back via the legacy alias.
        move.b  $00FE_0400, D0
        andi.l  #$FF, D0
        cmpi.l  #$A5, D0
        bne     fail_1
        ; Read back via canonical too.
        move.b  $00BF_E001, D0
        andi.l  #$FF, D0
        cmpi.l  #$A5, D0
        bne     fail_2

        ; CRA via canonical $BFEF01 (register $F = $0F).  Real Amiga's CIA-A
        ; spaces registers 256 bytes apart, so register N lives at $BFE001 +
        ; 256*N.  Register $F (CRB) is at $BFEF01.
        move.b  #$25, $00BF_EF01            ; arbitrary CRB value
        move.b  $00FE_040F, D0
        andi.l  #$FF, D0
        cmpi.l  #$25, D0
        bne     fail_3

        ; CIA-B at canonical $BFD000.  Set DDRA, write PRA, read back.
        move.b  #$FF, $00FE_0502            ; DDRA on CIA-B (legacy alias)
        move.b  #$5A, $00BF_D000            ; PRA on CIA-B (canonical)
        move.b  $00FE_0500, D0
        andi.l  #$FF, D0
        cmpi.l  #$5A, D0
        bne     fail_4

        stop    #0

fail_1: stop #$DD01
fail_2: stop #$DD02
fail_3: stop #$DD03
fail_4: stop #$DD04
