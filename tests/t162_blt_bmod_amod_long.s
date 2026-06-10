; MOVE.L to BLTBMOD ($DFF062) must commit both BLTBMOD and BLTAMOD
; ($DFF064).  Mirrors t156 (BLTAMOD/BLTDMOD long-write) and t156's
; BLTCMOD/BLTBMOD half.
;
; K1.3 trackdisk at PC $FEAF5C does:
;     LEA  $0062(A0), A1            ; A1 = $DFF062
;     MOVE.L D0, (A1)+              ; .L to $DFF062 (BLTBMOD + BLTAMOD)
;     MOVE.W D0, (A1)+              ; .W to $DFF066 (BLTDMOD)
;
; Pre-fix, our chipset adapter translated the .L as a single 16-bit
; write to BLTBMOD and DROPPED the low half intended for BLTAMOD —
; BLTAMOD kept the stale value from a prior graphics line-draw
; (e.g. $FF2E from PC $FC56F0 Bresenham).  Real effect: the 15th
; MFM-decode blit on boing-disk track 125 sector 0 inherited that
; stale AMOD, walking A pointer between rows by -210 bytes instead
; of 0.  The decoded OFS block at chip $1574 came out as garbage,
; failing the K1.3 BCPL OFS checksum and firing a "Volume X has a
; read/write error" AutoRequest.
;
; Test strategy: run a 2-row 1-word blit with USE_A=1, USE_D=1,
; LF=$F0 (D=A).  Source row 0 at $8000 holds $1234, source row 1
; at $8002 holds $5678.  BLTAMOD is pre-seeded to $FF2E (the bug
; sentinel), THEN cleared via MOVE.L #0 to $DFF062.  If the .L
; split landed, AMOD=0 and the blit decodes correctly:
;   dst[0] = $1234 (row 0)   dst[1] = $5678 (row 1, AMOD=0 advance)
; If the .L split DROPPED — AMOD=$FF2E (-210), row 1 A pointer
; lands at $8002 - 210 = $7F30 which is uninitialised → wrong dst[1].

        .org $400
start:
        ; Source: 2 words at $8000, $8002.
        move.w  #$1234, $00008000
        move.w  #$5678, $00008002
        ; Pre-clear destination at $9000, $9002.
        move.w  #$0000, $00009000
        move.w  #$0000, $00009002
        ; Also seed $7F30 with a sentinel — if AMOD=-210 bug fires, that's
        ; where the A read for row 1 lands and we'd see this value.
        move.w  #$DEAD, $00007F30

        ; Wait for any prior blit (BBUSY in DMACONR bit 14).
wait1:  btst    #6, $00DFF002
        bne     wait1

        ; ---- Pre-seed BLTAMOD with the stale Bresenham sentinel ----
        move.w  #$FF2E, $00DFF064        ; BLTAMOD = -210
        ; ---- The K1.3 trackdisk sequence: clear BMOD+AMOD via .L ----
        move.l  #$00000000, $00DFF062    ; .L should hit BOTH BLTBMOD and BLTAMOD
        ; (Don't write AMOD again — that would mask the bug.)
        move.w  #$0000, $00DFF066        ; BLTDMOD = 0 (always reset)
        move.w  #$0000, $00DFF060        ; BLTCMOD = 0

        ; BLTCON0 = $09F0: USE=$9 (USE_A, USE_D), LF=$F0 (D=A)
        ; BLTCON1 = $0000: no DESC, no LINE.
        move.l  #$09F00000, $00DFF040
        move.l  #$FFFFFFFF, $00DFF044    ; BLTAFWM = BLTALWM = $FFFF

        ; Pointers
        move.l  #$00008000, $00DFF050    ; BLTAPT
        move.l  #$00009000, $00DFF054    ; BLTDPT

        ; BLTSIZE = (height << 6) | width = (2 << 6) | 1 = $0081
        move.w  #$0081, $00DFF058

wait2:  btst    #6, $00DFF002
        bne     wait2

        ; Verify both rows.
        move.w  $00009000, D0
        cmpi.w  #$1234, D0
        bne     fail_row0
        move.w  $00009002, D0
        cmpi.w  #$5678, D0
        bne     fail_row1

        stop    #0

fail_row0:
        ; Should never see this — row 0 doesn't depend on AMOD.
        stop    #1
fail_row1:
        ; If D0 == $DEAD, AMOD wasn't cleared (= -210, pointer landed at $7F30).
        cmpi.w  #$DEAD, D0
        beq     fail_amod_stuck
        ; If D0 is otherwise garbage, also a bug but unexpected variant.
        stop    #2
fail_amod_stuck:
        ; The actual bug we fixed — AMOD stayed at $FF2E after the .L write.
        stop    #3
