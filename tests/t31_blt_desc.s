; Blitter descending mode (DESC) test: backwards-walking overlap copy.
;
; Setup at $2000..$200B:
;   $2000  $0001
;   $2002  $0002
;   $2004  $0003
;   $2006  $0004
;   $2008  $0000
;   $200A  $0000
;
; Goal: copy src[$2000..$2007] (4 words: 1,2,3,4) up to dest[$2004..$200B].
; Source and destination overlap at [$2004..$2007], with the dest occupying
; the HIGHER addresses.  Ascending traversal would clobber the source
; before reading it; descending traversal handles this correctly.
;
; After the blit:
;   $2000  $0001  (untouched)
;   $2002  $0002  (untouched)
;   $2004  $0001  (was $0003)
;   $2006  $0002  (was $0004)
;   $2008  $0003
;   $200A  $0004

        .org $400

start:
        move.l A7, D0
        cmpi.l #$4000, D0
        beq    core0
        stop   #0

core0:
        move.l  #$00010002, $00002000
        move.l  #$00030004, $00002004
        move.l  #$00000000, $00002008

        ; BLTCON: LF=$F0 (A-copy), ASH=0, BSH=0, DESC=1, USEA|USED ($9)
        ;   [31:24]=F0, [15]=1 DESC, [3:0]=9 -> $F0008009
        move.l  #$F0008009, $00FE0000
        move.l  #$0000FFFF, $00FE0004        ; BLTAFWM
        move.l  #$0000FFFF, $00FE0008        ; BLTALWM
        ; Start pointers at the LAST word of each region (highest address).
        move.l  #$00002006, $00FE000C        ; BLTAPT = highest src word
        move.l  #$0000200A, $00FE0018        ; BLTDPT = highest dst word
        move.l  #0,         $00FE001C        ; BLTAMOD
        move.l  #0,         $00FE0028        ; BLTDMOD
        ; BLTSIZE: height=1, width=4 -> (1<<6)|4 = $44
        move.l  #$00000044, $00FE0038

wait:   move.l  $00FE003C, D0
        andi.l  #1, D0
        bne     wait

        move.l  $00002000, D1
        cmpi.l  #$00010002, D1                ; source area, unchanged
        bne     fail1
        move.l  $00002004, D2
        cmpi.l  #$00010002, D2                ; overlap region, now = src[0..1]
        bne     fail2
        move.l  $00002008, D3
        cmpi.l  #$00030004, D3                ; high half, now = src[2..3]
        bne     fail3
        stop    #0

fail1:  stop    #$bd31
fail2:  stop    #$bd32
fail3:  stop    #$bd33
