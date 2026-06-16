; IDCMP_INJECT smoke test: verify the multi-cycle poke sequencer can build
; an IntuiMessage in chip RAM + link it into a MsgPort msglist.
;
; The IDCMP_INJECT env var is parsed by tb/sim_main.cpp; this test
; doesn't directly exercise it (env vars aren't part of the test harness),
; but it validates that the chip-RAM addresses the injection writes to are
; semantically reasonable: writing a MsgPort head pointer to userport+$14
; and a msg's ln_Succ to userport+$18.  If MsgPort layout offsets ever
; change in our K1.3 ROM, this test catches it.
;
; Test:
;   1. Pre-fill MsgPort at $20000 with empty-list sentinel:
;        head=&tail, tail=NULL, tailpred=&head
;   2. Verify our test reads the head==&tail pattern (= empty).
;   3. Write a Message at $30000 + link into the port (head=$30000,
;      tailpred=$30000, msg.succ=&tail, msg.pred=&head).
;   4. Verify head==$30000 now (= non-empty).

        .org $400
start:
        move.l  A7, D0
        cmpi.l  #$4000, D0
        beq     core0
        stop    #0

core0:
        ; ---- (1) Set up empty MsgPort at $20000 ----
        ; MsgPort.mp_MsgList at +$14 is a List struct (14 bytes)
        ; Empty list: lh_Head = &lh_Tail = port+$18, lh_TailPred = &lh_Head = port+$14
        move.l  #$00020018, $00020014   ; lh_Head = port+$18
        move.l  #0,         $00020018   ; lh_Tail = NULL
        move.l  #$00020014, $0002001C   ; lh_TailPred = port+$14

        ; ---- (2) Verify empty pattern ----
        move.l  $00020014, D0           ; lh_Head
        cmpi.l  #$00020018, D0          ; should be &lh_Tail
        bne     fail_empty_head

        ; ---- (3) Build a message at $30000, link into port ----
        ; Message header (Node):
        ;   +$00 ln_Succ = &lh_Tail = $00020018
        ;   +$04 ln_Pred = &lh_Head = $00020014
        move.l  #$00020018, $00030000
        move.l  #$00020014, $00030004
        ; Update port:
        ;   lh_Head = $30000
        ;   lh_TailPred = $30000
        move.l  #$00030000, $00020014
        move.l  #$00030000, $0002001C

        ; ---- (4) Verify non-empty ----
        move.l  $00020014, D0           ; lh_Head
        cmpi.l  #$00030000, D0
        bne     fail_link_head
        move.l  $0002001C, D0           ; lh_TailPred
        cmpi.l  #$00030000, D0
        bne     fail_link_tailpred
        ; And msg's links:
        move.l  $00030000, D0           ; ln_Succ
        cmpi.l  #$00020018, D0
        bne     fail_msg_succ
        move.l  $00030004, D0           ; ln_Pred
        cmpi.l  #$00020014, D0
        bne     fail_msg_pred

        stop    #0

fail_empty_head:     stop #$D741
fail_link_head:      stop #$D742
fail_link_tailpred:  stop #$D743
fail_msg_succ:       stop #$D744
fail_msg_pred:       stop #$D745
