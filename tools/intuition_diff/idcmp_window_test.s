| idcmp_window_test.s — third stub in the differential harness.
|
| Per docs/WB13_DEBUG_JOURNAL.md §62, OpenScreen and OpenWindow both
| pass on our sim with trivial parameters.  But boing waits forever on
| IDCMP_INTUITICKS messages on its Window's UserPort.  When boing
| stalls, the prior memory note (project_boing_render_blocked.md)
| reported the Window struct at chip $494A0 was *all zeros* and
| UserPort at slow $C0C618 was malformed.  That suggests Intuition
| allocates the UserPort differently when IDCMPFlags are non-zero.
|
| This stub exercises that path: same as open_window_test, but with
| IDCMPFlags = IDCMP_INTUITICKS ($00400000) + IDCMP_VANILLAKEY
| ($00200000), forcing Intuition to allocate a real UserPort.  We
| snapshot the UserPort MsgPort layout so a diff can spot whether our
| Intuition's UserPort-construction code path produces a valid struct.
|
| Marker block layout: same as open_window_test (offsets 0..79) PLUS
|   +80  UserPort.mp_Flags.B | mp_SigBit.B | pad.W
|   +84  UserPort.mp_SigTask
|   +88  UserPort.mp_MsgList.mlh_Head
|   +92  UserPort.mp_MsgList.mlh_Tail
|   +96  UserPort.mp_MsgList.mlh_TailPred
|   +100 UserPort.mp_LN.ln_Name (pointer)
|   +104 IDCMPFlags read back from Window
|   +108 Window.MessageKey
|
| A valid MsgPort has mp_Flags=PA_SIGNAL (=0), mp_SigBit != 0,
| mp_SigTask = boing task ptr, MsgList head/tail/tailpred forming a
| proper minlist (head -> tail's address-of-tail; tail = 0;
| tailpred -> head's address-of-head).

	.equ	ExecBase,        0x00000004
	.equ	LVO_OpenLibrary, -552
	.equ	LVO_AllocMem,    -198
	.equ	LVO_OpenScreen,  -198
	.equ	LVO_OpenWindow,  -204

	.equ	DFF002,          0x00DFF002
	.equ	DFF100,          0x00DFF100

	.equ	MARKER_POINTER,  0x00000C00
	.equ	MARKER_SIZE,     128
	.equ	MAGIC_TEST,      0x54455354

	.section .text,"ax"
	.globl	_start
_start:
	movea.l	ExecBase,%a6
	move.l	#MARKER_SIZE,%d0
	move.l	#0x00010002,%d1
	jsr	LVO_AllocMem(%a6)
	tst.l	%d0
	beq	hard_fail
	move.l	%d0,MARKER_POINTER
	movea.l	%d0,%a5

	move.l	#MAGIC_TEST,(%a5)
	clr.l	12(%a5)

	lea	intname(%pc),%a1
	moveq	#0,%d0
	jsr	LVO_OpenLibrary(%a6)
	move.l	%d0,4(%a5)
	tst.l	%d0
	beq	fail_lib
	movea.l	%d0,%a6

	| OpenScreen
	lea	ns_template(%pc),%a0
	movem.l	(%a0),%d0-%d7
	movem.l	%d0-%d7,-(%sp)
	movea.l	%sp,%a0
	jsr	LVO_OpenScreen(%a6)
	lea	32(%sp),%sp
	move.l	%d0,8(%a5)
	tst.l	%d0
	beq	fail_screen
	movea.l	%d0,%a4
	move.l	#3,12(%a5)

	| Screen snapshot
	move.l	8(%a4),16(%a5)
	move.l	12(%a4),20(%a5)
	move.l	0x20(%a4),24(%a5)
	move.w	DFF002,%d0
	swap	%d0
	move.w	DFF100,%d0
	move.l	%d0,28(%a5)
	movea.l	0x54+4(%a4),%a1
	move.l	(%a1),32(%a5)
	move.l	4(%a1),36(%a5)
	move.l	8(%a1),40(%a5)
	move.l	12(%a1),44(%a5)

	| Patch NewWindow.Screen
	lea	nw_screen_slot(%pc),%a0
	move.l	%a4,(%a0)

	| OpenWindow: push high half first, low half second
	lea	nw_template(%pc),%a0
	movem.l	32(%a0),%d0-%d3
	movem.l	%d0-%d3,-(%sp)
	movem.l	(%a0),%d0-%d7
	movem.l	%d0-%d7,-(%sp)
	movea.l	%sp,%a0
	jsr	LVO_OpenWindow(%a6)
	lea	48(%sp),%sp
	move.l	%d0,48(%a5)
	tst.l	%d0
	beq	fail_window

	movea.l	%d0,%a3                    | A3 = Window
	move.l	4(%a3),52(%a5)
	move.l	8(%a3),56(%a5)
	move.l	24(%a3),60(%a5)
	move.l	82(%a3),64(%a5)             | IDCMPFlags
	move.l	50(%a3),68(%a5)             | RPort
	move.l	86(%a3),72(%a5)             | UserPort (this is what we care about)
	move.l	46(%a3),76(%a5)             | WScreen

	| Snapshot UserPort (MsgPort struct, 34 bytes)
	movea.l	86(%a3),%a2                | A2 = UserPort
	move.l	%a2,%d0
	beq	no_userport                 | NULL UserPort = silent failure path

	| struct MsgPort layout:
	|   +0   ln_Succ
	|   +4   ln_Pred
	|   +8   ln_Type.B (NT_MSGPORT=4)
	|   +9   ln_Pri.B
	|   +10  ln_Name (APTR)
	|   +14  mp_Flags.B    (PA_SIGNAL=0, PA_SOFTINT=1, PA_IGNORE=2)
	|   +15  mp_SigBit.B   (signal bit number, 0..31)
	|   +16  mp_SigTask    (APTR)
	|   +20  mp_MsgList.mlh_Head
	|   +24  mp_MsgList.mlh_Tail  (always NULL in a valid MinList)
	|   +28  mp_MsgList.mlh_TailPred
	move.l	14(%a2),80(%a5)             | flags | sigbit | first 2 bytes of SigTask hi
	move.l	16(%a2),84(%a5)             | SigTask
	move.l	20(%a2),88(%a5)             | mlh_Head
	move.l	24(%a2),92(%a5)             | mlh_Tail
	move.l	28(%a2),96(%a5)             | mlh_TailPred
	move.l	10(%a2),100(%a5)            | ln_Name pointer
	move.l	82(%a3),104(%a5)            | re-read IDCMPFlags (should match what we asked for)
	move.l	94(%a3),108(%a5)            | MessageKey

	move.l	#6,12(%a5)                  | status 6 = UserPort allocated, snapshot taken
	bra	spin

no_userport:
	move.l	#7,12(%a5)                  | status 7 = OpenWindow OK but UserPort NULL
	bra	spin

fail_lib:
	move.l	#1,12(%a5)
	bra	spin
fail_screen:
	move.l	#2,12(%a5)
	bra	spin
fail_window:
	move.l	#4,12(%a5)
spin:
	bra.s	spin

hard_fail:
	move.l	#MAGIC_TEST,MARKER_POINTER
	move.l	#255,MARKER_POINTER+4
	bra.s	hard_fail

	.balign	2
intname:
	.ascii	"intuition.library\0"
	.balign	2

ns_template:
	.word	0, 0
	.word	320, 200
	.word	5
	.byte	0, 1
	.word	0
	.word	15                         | CUSTOMSCREEN
	.long	0
	.long	ns_title
	.long	0
	.long	0
ns_title:
	.ascii	"IDCMP-test-screen\0"
	.balign	2

| NewWindow with IDCMP_INTUITICKS | IDCMP_VANILLAKEY so Intuition
| allocates a real UserPort and runs its input-handler dispatch.
nw_template:
	.word	10, 10
	.word	300, 180
	.byte	0, 1
	.long	0x00600000                 | IDCMPFlags = INTUITICKS($400000) | VANILLAKEY($200000)
	.long	0x00001000                 | Flags = ACTIVATE
	.long	0
	.long	0
	.long	nw_title
nw_screen_slot:
	.long	0
	.long	0
	.word	0, 0, 0, 0
	.word	15                         | CUSTOMSCREEN
nw_title:
	.ascii	"IDCMP-test-window\0"
	.balign	2

	.end	_start
