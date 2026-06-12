| open_window_test.s — second test stub in the differential harness.
|
| Same chain as screen_open_test but goes one library call further:
|     AllocMem -> OpenLibrary -> OpenScreen -> OpenWindow -> snapshot.
|
| If screen_open_test passes on our sim (it does, status=3 at commit
| fc5ac88) but THIS stub fails or produces a different Window
| fingerprint than FS-UAE, the boing bug is in OpenWindow / Layers /
| graphics.library — not in OpenScreen.
|
| Marker block layout (extends screen_open_test's):
|   +0   sentinel 'TEST'
|   +4   IntuitionBase
|   +8   Screen pointer
|   +12  status (1=OL fail, 2=OS fail, 3=screen ok / no OW yet,
|                4=OW fail, 5=OW ok)
|   +16  Screen.LeftEdge.W | TopEdge.W
|   +20  Screen.Width.W | Height.W
|   +24  Screen.ViewPort first long
|   +28  DMACONR.W | BPLCON0.W
|   +32  BitMap.BytesPerRow.W | Rows.W
|   +36  BitMap.Flags.B | Depth.B | pad.W
|   +40  BitMap.Planes[0]
|   +44  BitMap.Planes[1]
|   --- new fields for OpenWindow ---
|   +48  Window pointer
|   +52  Window.LeftEdge.W | TopEdge.W
|   +56  Window.Width.W | Height.W
|   +60  Window.Flags
|   +64  Window.IDCMPFlags
|   +68  Window.RPort pointer
|   +72  Window.UserPort pointer
|   +76  Window.WScreen pointer  (should == Screen pointer above)

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
	move.l	#0x00010002,%d1            | MEMF_CHIP | MEMF_CLEAR
	jsr	LVO_AllocMem(%a6)
	tst.l	%d0
	beq	hard_fail
	move.l	%d0,MARKER_POINTER
	movea.l	%d0,%a5

	move.l	#MAGIC_TEST,(%a5)
	clr.l	12(%a5)

	| ----- OpenLibrary("intuition.library", 0) -----
	lea	intname(%pc),%a1
	moveq	#0,%d0
	jsr	LVO_OpenLibrary(%a6)
	move.l	%d0,4(%a5)
	tst.l	%d0
	beq	fail_lib
	movea.l	%d0,%a6                    | IntuitionBase

	| ----- Push NewScreen, OpenScreen -----
	lea	ns_template(%pc),%a0
	movem.l	(%a0),%d0-%d7
	movem.l	%d0-%d7,-(%sp)
	movea.l	%sp,%a0
	jsr	LVO_OpenScreen(%a6)
	lea	32(%sp),%sp
	move.l	%d0,8(%a5)
	tst.l	%d0
	beq	fail_screen

	movea.l	%d0,%a4                    | A4 = Screen (preserved across OpenWindow)
	move.l	%d0,12(%a5)                | mark "screen open, attempting window"
	move.l	#3,12(%a5)

	| Snapshot Screen + BitMap fields BEFORE OpenWindow (so even if
	| OpenWindow trashes Screen state we have a baseline)
	move.l	8(%a4),16(%a5)             | LeftEdge|TopEdge
	move.l	12(%a4),20(%a5)            | Width|Height
	move.l	0x20(%a4),24(%a5)          | ViewPort first long
	move.w	DFF002,%d0
	swap	%d0
	move.w	DFF100,%d0
	move.l	%d0,28(%a5)
	movea.l	0x54+4(%a4),%a1            | A1 = RastPort.BitMap
	move.l	(%a1),32(%a5)
	move.l	4(%a1),36(%a5)
	move.l	8(%a1),40(%a5)
	move.l	12(%a1),44(%a5)

	| ----- Patch NewWindow.Screen = our Screen pointer -----
	lea	nw_screen_slot(%pc),%a0
	move.l	%a4,(%a0)

	| ----- Push NewWindow, OpenWindow -----
	| NewWindow is 48 bytes = 12 longs.  movem.l reg-list,-(SP) writes
	| registers in reverse order, so to land NewWindow bytes 0..47 in
	| ascending address order we MUST push the high half (bytes 32..47)
	| FIRST and the low half (bytes 0..31) SECOND — the second push
	| lands at the lower address.
	lea	nw_template(%pc),%a0
	movem.l	32(%a0),%d0-%d3            | high half: bytes 32..47
	movem.l	%d0-%d3,-(%sp)
	movem.l	(%a0),%d0-%d7              | low half: bytes 0..31
	movem.l	%d0-%d7,-(%sp)
	movea.l	%sp,%a0                    | A0 = NewWindow on stack

	jsr	LVO_OpenWindow(%a6)
	lea	48(%sp),%sp
	move.l	%d0,48(%a5)                | +48 Window ptr
	tst.l	%d0
	beq	fail_window

	movea.l	%d0,%a3                    | A3 = Window
	| struct Window offsets:
	|   +4  LeftEdge.W +6 TopEdge.W
	|   +8  Width.W   +10 Height.W
	|   +24 Flags
	|   +50 RPort
	|   +82 IDCMPFlags
	|   +86 UserPort
	|   +46 WScreen
	move.l	4(%a3),52(%a5)             | LeftEdge|TopEdge
	move.l	8(%a3),56(%a5)             | Width|Height
	move.l	24(%a3),60(%a5)            | Flags
	move.l	82(%a3),64(%a5)            | IDCMPFlags
	move.l	50(%a3),68(%a5)            | RPort
	move.l	86(%a3),72(%a5)            | UserPort
	move.l	46(%a3),76(%a5)            | WScreen

	move.l	#5,12(%a5)                 | status = window OK
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
	.word	0, 0                       | LeftEdge, TopEdge
	.word	320, 200                   | Width, Height
	.word	5                          | Depth
	.byte	0, 1                       | DetailPen, BlockPen
	.word	0                          | ViewModes
	.word	15                         | Type = CUSTOMSCREEN
	.long	0                          | Font
	.long	ns_title                   | DefaultTitle
	.long	0                          | Gadgets
	.long	0                          | CustomBitMap
ns_title:
	.ascii	"OpenWindow-DIFF-screen\0"
	.balign	2

| struct NewWindow {
|   WORD LeftEdge, TopEdge;
|   WORD Width, Height;
|   UBYTE DetailPen, BlockPen;
|   ULONG IDCMPFlags;
|   ULONG Flags;
|   struct Gadget *FirstGadget;
|   struct Image *CheckMark;
|   UBYTE *Title;
|   struct Screen *Screen;
|   struct BitMap *BitMap;
|   WORD MinWidth, MinHeight, MaxWidth, MaxHeight;
|   WORD Type;
| };  -- 48 bytes
nw_template:
	.word	10, 10                     | LeftEdge, TopEdge (small inset)
	.word	300, 180                   | Width, Height
	.byte	0, 1                       | DetailPen, BlockPen
	.long	0                          | IDCMPFlags = 0 (no events)
	.long	0x00001000                 | Flags = ACTIVATE only
	.long	0                          | FirstGadget
	.long	0                          | CheckMark
	.long	nw_title                   | Title
nw_screen_slot:
	.long	0                          | Screen — patched at runtime
	.long	0                          | BitMap
	.word	0, 0                       | MinWidth, MinHeight
	.word	0, 0                       | MaxWidth, MaxHeight
	.word	15                         | Type = CUSTOMSCREEN
nw_title:
	.ascii	"OpenWindow-DIFF-window\0"
	.balign	2

	.end	_start
