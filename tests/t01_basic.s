; Simplest possible test: every core executes NOP then STOP #0.
        .org $400
        nop
        nop
        stop #0
