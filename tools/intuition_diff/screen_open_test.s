| screen_open_test.s — minimal differential test for Intuition OpenScreen.
|
| PURPOSE
| -------
| Boing-disk is stuck in `boing!`'s post-OpenWindow polling loop.  We do
| not yet know whether the bug is in OpenScreen, OpenWindow, or somewhere
| upstream (graphics.library, blitter setup, exec).  This program isolates
| OpenScreen by running it from a fresh CLI with no DOS/BCPL/Workbench
| interference, then writes a marker block to chip RAM so the harness can
| diff two runs (ours vs. FS-UAE) byte-for-byte.
|
| FLOW
| ----
|   1. AllocMem(64, MEMF_CHIP|MEMF_CLEAR) -> marker block in chip RAM
|   2. Write the *pointer* to a fixed chip-RAM slot ($00000C00) so the
|      harness can locate the marker without scanning.
|   3. OpenLibrary("intuition.library", 0) -> IntuitionBase
|   4. Build a NewScreen on the stack: 320x200, 5 bitplanes, CUSTOMSCREEN,
|      ViewModes=0.  This matches boing's screen geometry closely enough
|      to exercise the same graphics.library code paths.
|   5. OpenScreen(NewScreen) -> Screen pointer
|   6. Capture a fingerprint of the Screen + chipset state into the marker:
|          +0  $54455354 'TEST'    sentinel
|          +4  IntuitionBase
|          +8  Screen pointer
|          +12 Status (1=OL fail, 2=Screen fail, 3=OK)
|          +16 Screen.LeftEdge.W | TopEdge.W
|          +20 Screen.Width.W | Height.W
|          +24 first long of Screen.ViewPort (bitplane / RasInfo state)
|          +28 DMACONR.W | BPLCON0 register-file readback.W
|   7. Spin forever.  Sim times out and dumps chip RAM.
|
| GNU-as syntax (m68k-amigaos-as) so we can produce a real hunkexe via the
| installed amiga-gcc toolchain; vasm's hunk module is not built into the
| binary on this machine.

	.equ	ExecBase,        0x00000004
	.equ	LVO_OpenLibrary, -552
	.equ	LVO_AllocMem,    -198
	.equ	LVO_OpenScreen,  -198

	.equ	DFF002,          0x00DFF002
	.equ	DFF100,          0x00DFF100

	.equ	MARKER_POINTER,  0x00000C00
	.equ	MARKER_SIZE,     128
	.equ	MAGIC_TEST,      0x54455354

	.section .text,"ax"
	.globl	_start
_start:
	| ----- AllocMem(MARKER_SIZE, MEMF_CHIP | MEMF_CLEAR) -----
	movea.l	ExecBase,%a6
	move.l	#MARKER_SIZE,%d0
	move.l	#0x00010002,%d1            | MEMF_CHIP (bit1) | MEMF_CLEAR (bit16)
	jsr	LVO_AllocMem(%a6)
	tst.l	%d0
	beq	hard_fail
	move.l	%d0,MARKER_POINTER         | publish marker ptr
	movea.l	%d0,%a5                    | A5 = marker block

	move.l	#MAGIC_TEST,(%a5)          | +0  sentinel
	clr.l	12(%a5)                    | +12 status = 0 (in-progress)

	| ----- OpenLibrary("intuition.library", 0) -----
	lea	intname(%pc),%a1
	moveq	#0,%d0
	jsr	LVO_OpenLibrary(%a6)
	move.l	%d0,4(%a5)                 | +4  IntuitionBase (or NULL)
	tst.l	%d0
	beq	fail_lib
	movea.l	%d0,%a6                    | A6 = IntuitionBase

	| ----- Push NewScreen onto stack -----
	lea	ns_template(%pc),%a0
	movem.l	(%a0),%d0-%d7              | load 32 bytes of template
	movem.l	%d0-%d7,-(%sp)             | push as NewScreen
	movea.l	%sp,%a0                    | A0 = NewScreen on stack

	| ----- OpenScreen(NewScreen) -----
	jsr	LVO_OpenScreen(%a6)
	lea	32(%sp),%sp                | pop NewScreen
	move.l	%d0,8(%a5)                 | +8  Screen pointer
	tst.l	%d0
	beq	fail_screen

	movea.l	%d0,%a0                    | A0 = Screen
	| struct Screen offsets:
	|   +0  NextScreen
	|   +4  FirstWindow
	|   +8  LeftEdge.W, +10 TopEdge.W
	|   +12 Width.W,    +14 Height.W
	|   +16 MouseY.W,   +18 MouseX.W
	|   +20 Flags.W
	|   +22 Title (APTR)
	|   +26 DefaultTitle (APTR)
	|   +30 BarHeight.UB +31 BarVBorder.UB
	|   +32 BarHBorder.UB +33 MenuVBorder.UB
	|   ... ViewPort embedded at offset $20+something ...
	move.l	8(%a0),16(%a5)             | LeftEdge|TopEdge (signed)
	move.l	12(%a0),20(%a5)            | Width|Height
	| ViewPort is embedded in Screen starting at offset $20.
	| First long of ViewPort = struct ViewPort {.Next, .ColorMap, ...}
	move.l	0x20(%a0),24(%a5)          | first long of ViewPort (Next)

	move.w	DFF002,%d0
	swap	%d0
	move.w	DFF100,%d0
	move.l	%d0,28(%a5)                | DMACONR|BPLCON0

	| Capture BitMap pointer + BPL1 address so we can see what graphics
	| got allocated.  Screen.RastPort is at offset $54; RastPort.BitMap
	| is at offset +4.  BitMap.Planes[0] is at offset +8.
	movea.l	0x54+4(%a0),%a1            | A1 = RastPort.BitMap
	move.l	(%a1),32(%a5)              | BitMap.BytesPerRow.W|Rows.W
	move.l	4(%a1),36(%a5)             | BitMap.Flags.B|Depth.B|pad.W
	move.l	8(%a1),40(%a5)             | BitMap.Planes[0] (chip-RAM bitplane ptr)
	move.l	12(%a1),44(%a5)            | BitMap.Planes[1]

	move.l	#3,12(%a5)                 | status = SUCCESS
	bra	spin

fail_lib:
	move.l	#1,12(%a5)
	bra	spin

fail_screen:
	move.l	#2,12(%a5)

spin:
	bra.s	spin

hard_fail:
	move.l	#MAGIC_TEST,MARKER_POINTER       | fallback sentinel at C00
	move.l	#255,MARKER_POINTER+4
	bra.s	hard_fail

	.balign	2
intname:
	.ascii	"intuition.library\0"
	.balign	2

| struct NewScreen { WORD LeftEdge, TopEdge, Width, Height, Depth;
|                    UBYTE DetailPen, BlockPen;
|                    WORD ViewModes, Type;
|                    APTR Font, DefaultTitle, Gadgets, CustomBitMap; };
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
	.ascii	"OpenScreen-DIFF\0"
	.balign	2

	.end	_start
