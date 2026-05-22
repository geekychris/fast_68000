; 68000 has a 24-bit external address bus -- the upper 8 bits of
; a 32-bit PC are silently dropped on every bus fetch.  Code that
; loads e.g. $7800090E into PC (via RTE/JMP/RTS/vector fetch) on
; real hardware lands at $0000090E.
;
; Surfaced by K1.3 boot at r=1313466: Exec writes function pointers
; for the Supervisor() priv-vio handler that have garbage (or flag
; bits) in the top byte.  Our core was driving the full 32-bit
; redirect address to the bus, going off-map.
;
; Pass = halt 0.  Failures:
;   $BAD1 = JMP to $78000_NNN didn't land at $00000_NNN
;   $BAD2 = RTS popping high-bit-set PC didn't truncate

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; Stage a routine at $1000 that just sets D5 and halts success.
        ; Encoded inline as raw words:
        ;   $1000  MOVE.L  #$DEADBEEF, D5       (4 words: 2A3C DEAD BEEF)
        ;   $1006  STOP    #0                   (2 words: 4E72 0000)
        ; The .L immediate has order high-word, low-word in memory.
        move.l  #$2A3CDEAD, $1000      ; first word = $2A3C, second = $DEAD
        move.l  #$BEEF4E72, $1004      ; third = $BEEF, fourth = $4E72
        move.w  #$0000, $1008          ; STOP imm word

        ; --- 1. RTS with a high-bit-set PC ---
        ; Push $78001000 (= $00001000 on the 24-bit bus) and RTS.
        move.l  A7, A0                  ; save current SSP
        move.l  #$78001000, -(A7)
        rts                              ; should land at $1000

        ; If RTS truncated correctly we'd halt at $1006 (STOP).  If we
        ; got back here, something is wrong (RTS didn't land at $1000).
        stop    #$BAD1
