# romwack/ — lifting K1.3's ROMWack out of ROM and reimplementing it

This directory contains everything related to ROMWack: the Commodore
serial-port debugger that lives inside Kickstart 1.3.  Two complete
artefacts live here side by side:

1.  **The original**, sliced out of `kickstart/kick_13.bin` and made
    relocatable to any address.
2.  **A from-scratch reimplementation** in 68k assembly, ~1.2 KB,
    PC-relative, with a documented extension hook so you can graft on
    your own commands without rebuilding the rest.

The full design and reimplementation guide is in [`design.md`](design.md).
This README is a quick-start.

---

## Contents

```
romwack/
├── README.md         ← you are here
├── design.md         ← architecture, conventions, extension recipes
├── wack.s            ← from-scratch reimplementation (GAS m68k syntax)
├── extract.py        ← slice ROMWack out of the K1.3 ROM image
├── relocate.py       ← move either binary to a new base address
├── deps.py           ← report external dependencies of the extract
├── build.py          ← assemble + link wack.s (Docker cross-toolchain)
└── build/            ← output dir (gitignored by convention)
```

All four scripts are expected to be run from the **repo root**:

```
python3 romwack/extract.py kickstart/kick_13.bin
python3 romwack/relocate.py romwack/build/extracted.bin --target 0x80000 --out X.bin
python3 romwack/deps.py kickstart/kick_13.bin --manifest romwack/build/extracted.json
python3 romwack/build.py
```

---

## Quick start: extract & relocate the original ROMWack

Slice ROMWack and its CPU-detection helper out of K1.3:

```
python3 romwack/extract.py kickstart/kick_13.bin
```

writes

- `romwack/build/extracted.bin` — 12 226 B of code + data
- `romwack/build/extracted.json` — manifest (entry-point map + 51 relocs)
- `romwack/build/extracted.txt` — human-readable report

The 51 relocations are every absolute-long reference inside the
extracted slice; hardware MMIO (`$DFF018/30/32/9A/9C`), low-RAM
vectors and ExecBase live outside the slice and are not touched.

Move the blob to any base:

```
python3 romwack/relocate.py romwack/build/extracted.bin \
    --target 0x70000 --out romwack/build/extracted_70000.bin
```

The relocator prints the new address of every named entry point so
you can jump in:

```
$00071BDE  printf
$00071CEE  serper_init
$00071E54  romwack_main      ← typical entry
$00072EEA  cmd_dispatch_head
...
```

To check what (if anything) still reaches outside the extracted
window:

```
python3 romwack/deps.py kickstart/kick_13.bin \
    --manifest romwack/build/extracted.json
```

With the default extraction ranges the only remaining external
reference is `JMP $FFE2(A6)` — exec's `Supervisor` LVO call, which
is dynamic (resolved via A6=ExecBase at run time) and is the
standard Amiga path to flip into supervisor mode.

---

## Quick start: build the small reimplementation

Requires Docker; pulls in the `fast68k-cc:latest` image (the same
toolchain `tools/cc68k.sh` uses).  The first run builds the image.

```
python3 romwack/build.py
```

writes

- `romwack/build/wack.bin` — 1208 B flat binary, linked at `$00070000`
- `romwack/build/wack.json` — manifest (entry points, 11 relocs)

Relocate just like the extracted version — the file format is the
same:

```
python3 romwack/relocate.py romwack/build/wack.bin \
    --target 0x80000 --out romwack/build/wack_80000.bin
```

### Pre-conditions to invoke `wack_entry`

- Supervisor mode (we `MOVE.W SR,-(SP)` on entry — `RTR` pops the
  full word).
- About 80 bytes of stack headroom for the save frame.
- A serial terminal attached at 9600 8N1.
- Interrupts **should** be masked by the caller — the monitor polls
  Paula `SERDATR` directly.

From a supervisor-mode caller, just JSR to the relocated address:

```assembly
    jsr     wack_entry              | absolute, post-relocation
```

From user mode (typical CLI invocation), flip via `exec.Supervisor()`:

```assembly
    lea     wack_entry, a5
    move.l  4.w, a6
    jsr     -30(a6)                 | exec.Supervisor: A5 -> sup. func
```

The monitor prints

```
wack v1
> 
```

and accepts one of the built-in commands.

### Built-in commands

| Command | Args               | Action |
|---------|--------------------|---|
| `r`     | —                  | Dump saved D0..D7 / A0..A6 / SR / PC |
| `d`     | `<addr> [count]`   | Hex-dump `count` bytes from `addr` (default 64) |
| `g`     | `[addr]`           | Resume; optional new PC overrides the saved one |
| `?`     | —                  | List all command names (built-in + user) |

All numbers are parsed as hex (no `$` or `0x` prefix needed).

---

## Extending the reimplementation

The dispatch list is a singly-linked list of 14-byte nodes:

```
+0  next     long   | pointer to next node, 0 = end
+4  name     long   | pointer to NUL-terminated command name
+8  flags    word   | reserved (currently $0001)
+A  handler  long   | JSR (An) target; D0=0 to continue, D0=-1 to exit
```

The monitor walks `builtin_head` first, then whatever is stored in
the long `user_cmd_head`.  To install your own commands, build your
own linked list and write its head address into `user_cmd_head`:

```assembly
    | --- assume wack.bin has been copied to $80000
    | --- user_cmd_head is at offset $464 within the blob
    move.l  #my_node, $00080464

| -------------------------------------------------------------------
| Custom command: "hello" - prints a greeting and continues.
| -------------------------------------------------------------------
my_node:
    dc.l    0                            | next = end of user list
    dc.l    my_name
    dc.w    $0001
    dc.l    my_handler

my_name:    dc.b   'hello',0
    .balign 2

my_handler:
    | A0 = command-line args (post-name, ws skipped)
    | A5 = save-area base (registers / SR / PC of the caller)
    lea     greeting(pc), a0
    bsr     print_string                 | link against wack.s for the name
    moveq   #0, d0                       | 0 = continue prompt
    rts

greeting:   dc.b   'hi from a user cmd', $0d, 0
    .balign 2
```

Handler entry conventions:

| Register | Contents |
|----------|----------|
| `A0` | start of command-line args, after the command name and any leading whitespace |
| `A5` | base of the save frame — `SAV_D0(A5)` through `SAV_PC(A5)` |
| `SP` | inside the monitor's stack; free to PEA/PUSH as long as you balance |

Handler return convention:

- `D0 = 0` — return to the prompt
- `D0 = -1` — exit the monitor (RTR back to caller with original SR/PC)

To resume at a *different* PC, write to `SAV_PC(A5)` first, then
return `-1`.  That is exactly what the built-in `g <addr>` does.

A handler can call any of the monitor's internal helpers; the
simplest pattern is to link your extension into the same blob as
`wack.s` so the names resolve directly.  If you need to call them
from a separately-loaded blob, read the relocated addresses out of
`romwack/build/wack.json`.

---

## Layout reference (wack.bin at link-base `$70000`)

| Symbol | Address |
|---|---|
| `wack_entry`     | `$70000` |
| `serial_init`    | `$7004A` |
| `getc_block`     | `$7005C` |
| `putc`           | `$70078` |
| `print_string`   | `$700B8` |
| `print_hex_byte` | `$700D0` |
| `print_hex_word` | `$70108` |
| `print_hex_long` | `$70116` |
| `read_line`      | `$7013C` |
| `skip_ws`        | `$701A6` |
| `parse_hex`      | `$701BE` |
| `str_eq`         | `$70218` |
| `dispatch`       | `$7024E` |
| `walk_list`      | `$7028A` |
| `cmd_regs`       | `$702B8` |
| `cmd_dump`       | `$70362` |
| `cmd_go`         | `$703BE` |
| `cmd_help`       | `$703D0` |
| `banner`         | `$70408` |
| `prompt`         | `$70412` |
| `builtin_head`   | `$7042C` |
| `user_cmd_head`  | `$70464` |
| `linebuf`        | `$70468` |

All shift by `target - $70000` after relocation; the actual values
land in the `entries` map of `wack.json`.

---

## Where to read next

- [`design.md`](design.md) — full design document covering ROM layout
  of the original, save-state conventions, serial protocol, the
  printf formatter, command dispatch, and a reimplementation guide.
- [`wack.s`](wack.s) — the from-scratch source.  About 600 lines
  including comments; the actual code is ~1.2 KB.
