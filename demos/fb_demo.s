; Framebuffer demo: animated XOR pattern, no OS.
;
; The framebuffer lives at byte address $0001_0000, 256 wide x 192 tall,
; 8 bits per pixel in RGB332 format (RRRGGGBB).  Byte at (x,y) is at
; $0001_0000 + y*256 + x.
;
; This program writes a moving XOR pattern: color(x,y) = (x + frame) ^ y.
; A small bouncing white square moves across the screen on top.

        .org $400

start:
        moveq  #0, D7                ; D7 = frame counter

frame_loop:
        ; ---- redraw background: byte = (x + frame) ^ y ----
        move.l #$00010000, A0        ; A0 = FB_BASE
        moveq  #0, D1                ; D1 = y
yloop:
        moveq  #0, D2                ; D2 = x
xloop:
        move.l D2, D3
        add.l  D7, D3
        eor.l  D1, D3
        move.b D3, (A0)+             ; pixel byte (low 8 bits of D3)
        addq.l #1, D2
        cmpi.l #256, D2              ; FB width
        bne    xloop
        addq.l #1, D1
        cmpi.l #192, D1              ; FB height
        bne    yloop

        ; ---- bouncing dot ----
        ; Bounce x in [8, 247]: span 240. Use phase = frame mod 240.
        move.l D7, D0
        andi.l #$00000FFF, D0        ; cap phase so divu fits a 16-bit divisor
        move.l D0, D4
        divu.w #240, D4              ; rem in upper 16 bits after instruction
        swap   D4
        andi.l #$0000FFFF, D4        ; D4 = phase mod 240
        addi.l #8, D4                ; D4 = cx in [8, 247]

        ; Bounce y in [8, 183]: span 176.
        move.l D0, D5
        divu.w #176, D5
        swap   D5
        andi.l #$0000FFFF, D5
        addi.l #8, D5                ; D5 = cy

        ; pixel address = FB_BASE + cy*256 + cx
        move.l D5, D6
        lsl.l  #8, D6                ; cy * 256
        add.l  D4, D6
        move.l #$00010000, A0
        adda.l D6, A0

        ; Draw a 4-pixel-wide x 4-pixel-tall white block (write 4 bytes per row).
        moveq  #4, D2
dotrow:
        move.l #$FFFFFFFF, (A0)      ; 4 white pixels
        adda.l #256, A0              ; next row
        subq.l #1, D2
        bne    dotrow

        ; ---- advance frame ----
        addq.l #1, D7

        ; light pacing delay
        move.l #500, D5
pace:   subq.l #1, D5
        bne    pace

        bra    frame_loop
