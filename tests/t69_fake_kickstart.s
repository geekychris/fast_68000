; Trampoline for the fake-kickstart ROM.  Core 1+ parks; core 0
; would (in principle) jump into ROM — though with OVL=1 the IF
; fetches at $400 are already routed to ROM, so this asm only runs
; on cores that aren't core 0 in builds where we have multiple cores.
        .org $400
start:
        stop   #0
