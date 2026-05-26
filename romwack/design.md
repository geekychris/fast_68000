# ROMWack: design, extraction, reimplementation

This document describes the small serial-port debugger that lives
inside Kickstart 1.3, how it is invoked, how the code is laid out,
and how to lift it out of ROM into a relocatable, extensible form.

The reference target is the 256 KB A500 K1.3 ROM (`exec 34.2 (28 Oct
1987)`, file `kickstart/kick_13.bin`).  All addresses are absolute in
the running Amiga's memory map; the K1.3 image is mapped at
`$00FC0000-$00FFFFFF`.

Everything described here ships under `romwack/`:

```
romwack/
├── README.md         quick-start
├── design.md         this file
├── wack.s            from-scratch reimplementation
├── extract.py        slice ROMWack out of K1.3 ROM
├── relocate.py       move either binary to a new base
├── deps.py           external dependency report
├── build.py          assemble + link wack.s
└── build/            outputs
```

Sections 1-4 below cover what ROMWack *is*, where it lives in the
K1.3 ROM, and how the conventions that the code depends on actually
work.  Section 5 onwards covers lifting it out, relocating it,
extending it, and (in section 7) the reimplementation in
[`wack.s`](wack.s).

---

## 1. What ROMWack is

ROMWack is a stand-alone serial-port monitor: a register dump, a
memory dump/alter, a list/symbol facility, and a "go" command, with
no screen UI and no system services beyond the things it talks to
itself.  All I/O goes over the Amiga's RS-232 connector (Paula
`SERPER`/`SERDAT`/`SERDATR`) at 9600 8N1.

It was Commodore's last-resort debugger for situations where the
graphics subsystem or operating system was too broken to put a Guru
on the screen.  It is invoked in three ways:

- **`exec.Debug()`** (LVO -114) — the public entry.  In K1.3 the LVO
  shim at `$FC7308` saves A0/A1 and jumps to `$FD2A9E`, which is the
  high-level dispatcher; that dispatcher in turn drops into the
  monitor body when a debugger is requested.  *Note:* `Debug()` is
  declared as a placeholder in the Amiga ROM Kernel Reference, and
  programs that want to enter the monitor reliably use the Alert path
  below instead.

- **Alert / Guru path** — when `exec.Alert()` is called with the
  *dead-end* bit set and the alert handler in the ROM cannot recover,
  the kernel enters ROMWack's main entry at `$FC239A`.  This is the
  path most users see in practice: a Guru that drops you to a wack
  prompt over RS-232.

- **CLI command** — the (third-party) `RomWack` shell command is a
  tiny stub that calls `exec.Debug()`.  Anything else can do the
  same: a startup-sequence binary, a tool icon, an input-handler.

The serial connection is required.  Without a terminal on RS-232 a
ROMWack session is invisible.

---

## 2. Code layout in K1.3 ROM

ROMWack is not contiguous.  The relevant runs are:

| Range | Size | Contents |
| --- | --- | --- |
| `$FC0546-$FC0592` | 76 B | CPU/FPU probe (`MOVEC`/F-line trap test) |
| `$FC2048-$FC2DA6` | 3422 B | All ROMWack code: serial I/O, printf, parser, command bodies, entry trampolines |
| `$FC33E2-$FC3430` | 78 B | Command-name strings (`alter\0boot\0clear\0…`) |
| `$FC3430-$FC3508` | 216 B | Linked-list dispatch table (15 nodes × 14 B + 4 B terminator) |

The big gap between `$FC2DA6` and `$FC33E2` is occupied by the
`audio.device` resident and unrelated kernel code.

### Named entry points

(Offsets are within the extracted blob produced by
`romwack_extract.py`; address is the original ROM location.)

| Address | Symbol | Purpose |
| --- | --- | --- |
| `$FC0546` | `cpu_detect` | Returns 0 for 68000, non-zero for 68010+/FPU |
| `$FC2048` | `strlen` | A0 in, length-1 returned in D2 |
| `$FC20C0` | `powers10_table` | $3B9ACA00 (1e9), $05F5E100, ... 10, 0 |
| `$FC2124` | `printf` | A0=fmt, A2=putc-vector, varargs on stack |
| `$FC2234` | `serper_init` | `move.w #$0174, $DFF032` (9600 baud) |
| `$FC223E` | `getc` | Returns -1 in D0 if no char, else char in low byte |
| `$FC225E` | `getc_block` | Spins on `getc` until a char arrives |
| `$FC2266` | `putc` | D0 in; expands `$0D` -> CR LF |
| `$FC22B4` | `print_string` | A0 = NUL-terminated string |
| `$FC22CA` | `print_hex` | D0 = value, D1 = digit count |
| `$FC232C` | `banner_string` | `"\nrom-wack\0"` |
| `$FC2336` | `trap_setup` | Hooks vectors `$0042` / `$0190`, calls SERPER init |
| `$FC239A` | `romwack_main` | Save-all-regs entry from Alert path |
| `$FC2D26` | `unknown_symbol_msg` | `"\nunknown symbol\0"` |
| `$FC2D38` | `parse_hex` | Read hex/dec number from string |
| `$FC2D84` | `upcase_char` | `'a'-'z'` -> `'A'-'Z'` in D0 |
| `$FC33E2` | `cmd_names` | Concatenated command-name C strings |
| `$FC3430` | `cmd_dispatch_head` | First node of the linked dispatch list |

---

## 3. Architectural conventions

ROMWack uses a few unusual conventions that you must respect if you
re-enter the monitor or splice into it from another address.

### 3.1 The save-state buffer at `$0200`

On entry the monitor sets `A6 = $0200` and uses `A6` as the base of a
fixed-layout scratch area in low RAM.  Selected offsets:

| Offset from A6 | Use |
| --- | --- |
| `$0000` | Saved D0..D7, A0..A6 (15 longs, MOVEM order) |
| `$003C` | Saved SP at entry |
| `$0084` | Pointer to the caller's task/exception context |
| `$00EA` | Saved INTENAR (`$DFF01C` at entry) |
| `$0114` | Cached `ExecBase` -> `ThisTask` pointer |
| `$0126`, `$0127` | Forbid/Disable nest counters used while wack is alive |
| `$FF90` (-$70) | Address $0190 — vector 100 patch slot |

In other words, the monitor pretends `$0200` is the start of its own
TCB-shaped block.  Hardware MMIO offsets like `$DFF01C` are reached
as `$00EA(A6)` because `$0200 + $EA - $1000` ... no, simpler: the
field at `$EA(A6) = $02EA` is a *cached copy* of `$DFF01C`, written
once during entry.

> If you relocate ROMWack to a new address, **don't touch the `$0200`
> assumption**.  Either reserve that low-RAM page (page-aligned, 256 B
> safe) or rewrite the entry to use a different fixed base.

### 3.2 Exception vectors `$0042` and `$0190`

`trap_setup` (`$FC2336`) writes:
- `$0042 = entry_point_A`
- `$0190 = entry_point_B`

`$0042` is *not* aligned to the standard 4-byte vector slot; the
monitor is using offsets within the `A6=$200` buffer, so writing to
`$42(A6)` lands at absolute `$0242`.  This is internal scratch, not a
real CPU vector.

`$FF90(A6) = $0190` *is* a real vector slot — vector 100, in the
"user-defined interrupt" range.  ROMWack overloads it as its
re-entry hook for whatever installed it.

### 3.3 Calling `exec.Supervisor()`

At `$FC2370` ROMWack does `JMP $FFE2(A6)`.  When the code is reached
via the CLI path (`A6 = ExecBase`), `-$1E(A6)` is exec LVO `Supervisor`,
which flips into supervisor mode and runs a routine pointed at by the
register convention.  This is the *only* genuine external dependency
in the monitor body: everything else is either self-contained or
talks straight to Paula registers.

### 3.4 Serial port

`$DFF032 SERPER = $0174` sets 9600 baud on a PAL 7.094 MHz `E_CLOCK`
(`$0174 + 1 = 373` → ~9612 baud).  NTSC is close enough that K1.3
works on both.

- `getc`: read `$DFF018 SERDATR`, check the RBF bit, write
  `$0800` to `$DFF09C INTREQ` to clear, return char in D0 (or -1).
- `putc`: write D0.B to `$DFF030 SERDAT` with the standard
  "data | $100" stop-bit encoding (the `0100` bit set as the start
  of stop-bit), then spin until TBE.
- CR translation: `putc` of `$0D` emits `$0D 0A`.

No interrupts, no flow control, no `DBR` handshaking.  If you stuff
the input faster than the monitor can read you will lose chars.

### 3.5 `printf` formatter (`$FC2124`)

Mini-printf with a custom putc vector in `A2` and the format string
in `A0`.  Arguments are word-aligned on the caller's stack, popped
oldest-first.  Supported conversions (from the format strings found
in the ROM):

| Spec | Effect |
| --- | --- |
| `%06lx` | 32-bit hex, 6 digits, zero-padded |
| `%04x`  | 16-bit hex, 4 digits |
| `%-2ld`, `%-4ld` | left-justified signed decimal, width `N` |
| `%s` | NUL-terminated C string |
| `%%` | literal `%` |

Width modifiers `-` (left-justify), digit count, and the `l` (long)
qualifier are honoured; precision is not.  The dec/hex helpers at
`$FC207E` and `$FC20E8` consume the powers-of-ten table at
`$FC20C0`.

### 3.6 Number parser (`$FC2D38`)

Eats whitespace, accepts hex digits `0-9 a-f A-F` directly (no `$` or
`0x` prefix needed — every number is a hex), stops at the first
non-hex char.  Returns value in D1, length in D2.

### 3.7 Command dispatch

The list lives at `cmd_dispatch_head = $FC3430`.  Each node is **14
bytes**:

```
+0  next     long   ; pointer to next node, 0 terminates the list
+4  name     long   ; pointer to NUL-terminated command name string
+8  flags    word   ; observed values: 0x0001 (no per-cmd flags used)
+A  handler  long   ; absolute address jumped to with JSR (Ax)
```

When the user types a token at the prompt, the monitor walks the
list, compares against `name`, and `JSR`s the matching `handler`
with the rest of the input line accessible through the save-state
buffer.

Built-in commands and their handlers:

| Name | Handler | Notes |
| --- | --- | --- |
| `alter` | `$FC26AC` | edit memory word/byte |
| `boot`  | `$FC05F0` | jump into cold-boot path (outside extracted window) |
| `clear` | `$FC28DE` | clear memory range |
| `fill`  | `$FC2A74` | fill memory range with value |
| `find`  | `$FC29FC` | search for byte pattern |
| `go`    | `$FC2474` | resume at PC (with optional new PC) |
| `ig`    | `$FC05F0` | also lands in cold-boot helper; "ignore"/abort path |
| `limit` | `$FC29EE` | set search/dump end address |
| `list`  | `$FC26EE` | list memory contents |
| `regs`  | `$FC2822` | dump saved registers |
| `reset` | `$FC28EE` | hard reset |
| `resume`| `$FC247E` | resume from breakpoint |
| `set`   | `$FC290C` | set memory range |
| `show`  | `$FC2946` | show variable / symbol |
| `user`  | `$FC24EE` | switch to user-stack view |

> *Caveat*: the handlers for `boot` and `ig` both point at
> `$FC05F0`, which is **outside** the extracted window.  In a
> relocated copy these two commands will still call back into ROM,
> which is what you want on a normally-booted machine.

---

## 4. Lifting ROMWack out of ROM

The `romwack/extract.py` script slices the four ranges above
out of `kick_13.bin` and writes a relocatable blob plus a manifest
listing every absolute-long reference inside the slice.

```
python3 romwack/extract.py kickstart/kick_13.bin    # default out: romwack/build/extracted
```

Produces:

- `romwack/build/extracted.bin` — 12 226 B raw bytes
- `romwack/build/extracted.json` — manifest with `src_base`, length, named
  entry-point map, and 51 relocation records
- `romwack/build/extracted.txt` — human-readable summary

To relocate to a new address:

```
python3 romwack/relocate.py romwack/build/extracted.bin \
    --target 0x70000 --out romwack/build/extracted_70000.bin
```

The 51 relocation records cover every absolute-long operand of
`JSR/JMP/PEA/LEA abs.l` and `MOVE.L #imm32, …` inside the code range,
plus every long-aligned pointer in the dispatch list.

What is **not** patched, on purpose:
- Paula MMIO (`$DFF018/30/32/9A/9C`) — addresses outside the
  extracted window, so no relocation entry exists for them.
- Vectors `$0042` / `$0190` and the `A6=$0200` buffer base — these
  *values* (`$42`, `$190`, `$200`) appear as **immediates**, not as
  pointers, and the scanner only relocates pointers landing inside
  the extracted ranges.
- The "boot"/"ig" handler `$FC05F0` — outside the extracted window,
  intentionally left pointing into ROM.

To list every dependency that crosses the window:

```
python3 romwack/deps.py kickstart/kick_13.bin \
    --manifest romwack/build/extracted.json
```

With the default ranges this prints zero external static targets
and six dynamic (register-indirect) calls, all of which are internal
function-pointer invocations except `JMP $FFE2(A6)` at `$FC2370`,
which is exec's `Supervisor` LVO call.

---

## 5. Invoking a relocated copy

After loading the relocated bytes into RAM at your chosen target,
jump to the relocated address of `romwack_main` (the `.txt` report
lists where every entry ended up).  Typical sequence from caller
code:

```
    move.l  4.w,a6              ; need ExecBase in A6 for Supervisor JMP
    lea     romwack_main(pc),a0 ; or absolute, depending on link
    jmp     (a0)
```

…or simpler if you don't care about the Supervisor path and you've
preserved the save buffer:

```
    jsr     romwack_main(pc)
```

The monitor will then init Paula's SERPER, print the `rom-wack`
banner over RS-232, and present its prompt.

**Pre-conditions to keep in mind:**

- The low-RAM page at `$0200-$02FF` must be writeable and unused by
  the rest of your program.  ROMWack stomps it.
- Vector slots `$0042` and `$0190` will be overwritten.
- Interrupts will be disabled while the monitor is active.
- A serial terminal must be attached.

---

## 6. Extending a relocated copy with new commands

The dispatch list is a *singly-linked* list pointed at by the global
`cmd_dispatch_head = $FC3430` (post-relocation address found in the
`.json` manifest).  A new command is a 14-byte record matching the
node layout above, inserted anywhere by adjusting `next` pointers.

Easiest pattern: **prepend** a custom node, since nothing else
points at the original head.

```assembly
; New node, 14 bytes
my_node:
    dc.l    cmd_dispatch_head           ; +0  next  -> first builtin
    dc.l    my_name                     ; +4  name
    dc.w    $0001                       ; +8  flags
    dc.l    my_handler                  ; +A  handler

my_name:    dc.b 'mycmd',0
    even

my_handler:
    ; D0..D7/A0..A7 from the user are visible at $00(A6) onward,
    ; where A6 = $0200 on entry. Print via the existing helpers:
    lea     mymsg(pc),a0
    jsr     print_string                ; resolved at link time
    rts

mymsg:      dc.b 'hello from mycmd',$0a,0
    even
```

Then, immediately after relocating the blob, patch the head pointer.
Easiest is to *not* patch `cmd_dispatch_head` itself but to install a
new head somewhere your loader controls; the monitor walks whichever
list it is pointed at.

To patch the head in place: the address of the head pointer is *not*
stored — the head IS the location.  Specifically the code at
`$FC2BAE` does `LEA cmd_dispatch_head, A3` to start the walk, so
overwriting that LEA's operand (4 bytes after the opcode) with the
address of your custom head node lets you redirect the walk.  In the
relocated blob, that operand is in the manifest as the
`@$00FC2BAE LEA.A3 -> $00FC3430` reloc.

A cleaner alternative is to fork the source: see section 7 below.

---

## 7. Reimplementing ROMWack from scratch

[`wack.s`](wack.s) is exactly such a reimplementation: a from-scratch
serial monitor in 68k assembly, ~1.2 KB assembled, fully
PC-relative aside from 11 absolute long pointers in the built-in
dispatch list.  It is structured to make extension as easy as
storing a pointer into `user_cmd_head` (no source modification
needed — see "Adding your own commands" in
[`README.md`](README.md)).

To build it:

```
python3 romwack/build.py
```

Output:
- `romwack/build/wack.bin` — flat binary, linked at `$00070000`
- `romwack/build/wack.json` — manifest compatible with
  `romwack/relocate.py` (same format as the extracted-original
  manifest)

Then relocate exactly like the extract:

```
python3 romwack/relocate.py romwack/build/wack.bin \
    --target 0xADDR --out romwack/build/wack_ADDR.bin
```

The build pipeline uses the `fast68k-cc:latest` docker image shared
with `tools/cc68k.sh`; the first run will build it (~1 minute) and
cache it locally.

### Functional skeleton

If you want to reimplement from the ground up instead of starting
from `wack.s`, here is the architecture in shape:

```
romwack:
    ; 1. Save all registers to a known location.
    movem.l d0-d7/a0-a6,-(sp)

    ; 2. Init serial.
    move.w  #$0174,$dff032          ; 9600 baud
    move.w  #$0800,$dff09c          ; clear RBF latch

    ; 3. Print banner.
    lea     banner(pc),a0
    bsr     print_string

prompt:
    ; 4. Print prompt, read line.
    moveq   #'>',d0
    bsr     putc
    lea     linebuf(pc),a0
    bsr     read_line

    ; 5. Tokenise, walk dispatch list, JSR matching handler.
    bsr     dispatch

    bra     prompt
```

### Staying position-independent

A relocatable build avoids all `MOVE.L #abs32` / `LEA abs.l` forms
and stays PC-relative.  The original ROM uses absolute references
because it lives at fixed addresses; a fresh reimplementation can
use `LEA label(PC), An` everywhere and ship as one position-
independent blob.  Specifically:

- Replace every `LEA abs.l, An` with `LEA label(PC), An`.
- Replace every `MOVE.L #abs32, ea` with `LEA label(PC), an; MOVE.L an, ea`.
- Keep `JSR/BSR` PC-relative (they already are, except for the few
  abs.l forms which become `BSR.W` or `LEA + JSR (An)`).
- Reach Paula registers via a single `LEA $DFF000, An` once at entry
  and use `xx(An)` everywhere — slightly more compact than
  `MOVE.W #imm, $DFFxxx.L` per access.

Even a complete reimplementation (15 commands, printf, hex parser)
will sit in well under 2 KB if you skip the `%-Nld` precision logic
that the original spends ~200 B on.

---

## 8. Quick reference

All commands assume the repo root as cwd.

```
# Extract the original ROMWack out of K1.3 ROM
python3 romwack/extract.py kickstart/kick_13.bin
    # -> romwack/build/extracted.bin / .json / .txt

# Build the from-scratch reimplementation
python3 romwack/build.py
    # -> romwack/build/wack.bin / .json   (requires docker)

# Relocate either binary to a chosen base
python3 romwack/relocate.py romwack/build/extracted.bin \
    --target 0x70000 --out romwack/build/extracted_70000.bin

python3 romwack/relocate.py romwack/build/wack.bin \
    --target 0x80000 --out romwack/build/wack_80000.bin

# Inspect dependencies leaving the extracted window
python3 romwack/deps.py kickstart/kick_13.bin \
    --manifest romwack/build/extracted.json
```

The relocated binary is callable at `<target> + offset` for each
entry in the manifest's `entries` map.  For the extracted original,
the minimum requirement to run is a writeable low-RAM page at
`$0200` and a serial terminal at 9600 8N1; the reimplementation
needs ~80 bytes of stack instead of the `$0200` page.
