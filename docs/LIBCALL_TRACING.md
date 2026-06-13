# K1.3 library-call tracing

A reusable instrumentation layer for spotting which AmigaOS library
functions a workload hits, with arguments and (optionally) return
values.  Designed to make "what is the OS actually doing right now?"
a one-command answer.

This is the framework that broke the boing-disk LoadSeg investigation
open — every time you find yourself with a hot ROM PC and want to know
"is this AllocMem? OpenLibrary? Wait?", this is the tool.

## When to reach for this

- You see a hot K1.3 ROM PC in `PC_HISTOGRAM` output and want to know
  what function it lives in.
- You're trying to figure out why a program hangs — is its task in
  `Wait()`?  On what signal?  Did anyone `Signal()` it?
- You suspect a load failure — did `AllocMem` succeed?  How many bytes?
  What flags?  Did it get `FreeMem`'d a moment later?
- You're tracking message-port traffic — `PutMsg` covers virtually
  every packet-based DOS call.

## Architecture

Three pieces:

1. `rtl/k13_libcall_probes.v` — Verilog probes, one per LVO, each
   gated by its own `+define`.  All are no-ops unless enabled.
2. `rtl/m68k_core.v` — `\`include`s the probes file inside the
   `is_settled` PC-classification block.  Probes have `u_rf.regs[]`
   and `retired` / `ex_pc` in scope.
3. `tools/decode_libcall_trace.py` — consumes `[LIBCALL]` lines from
   `run.log`, prints a per-function call count + most-common args.
   Supports `--filter <fn>` to dump every call to a specific function
   and `--first-of <fn>` to dump only the first occurrence.

## Enabling probes

Each probe is gated by `+define+LIBCALL_<NAME>_TRACE`.  The defines are:

| Define                          | Function       | LVO   | K1.3 JT addr |
|---------------------------------|----------------|-------|--------------|
| `LIBCALL_OPENLIB_TRACE`         | OpenLibrary    | -552  | `$C0004E`    |
| `LIBCALL_ALLOCMEM_TRACE`        | AllocMem       | -198  | `$C001B0`    |
| `LIBCALL_FREEMEM_TRACE`         | FreeMem        | -210  | `$C001A4`    |
| `LIBCALL_WAIT_TRACE`            | Wait           | -318  | `$C00138`    |
| `LIBCALL_SIGNAL_TRACE`          | Signal         | -324  | `$C00132`    |
| `LIBCALL_FINDTASK_TRACE`        | FindTask       | -294  | `$C00150`    |
| `LIBCALL_FORBID_TRACE`          | Forbid         | -132  | `$C001F2`    |
| `LIBCALL_PERMIT_TRACE`          | Permit         | -138  | `$C001EC`    |
| `LIBCALL_PUTMSG_TRACE`          | PutMsg         | -366  | `$C00108`    |
| `LIBCALL_ALL`                   | (enable everything above) | | |

Pass them via `EXTRA_VERI_DEFS`:

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_ALLOCMEM_TRACE +define+LIBCALL_FREEMEM_TRACE' \
  make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
```

Or enable everything at once:

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_ALL' \
  make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
```

## Reading the output

Probe lines look like:

```
[LIBCALL] r=2560484 AllocMem(size=6728, flags=h00000002)  A6=00c00276
[LIBCALL] r=2560910 FreeMem(ptr=00051670, size=6728)  A6=00c00276
[LIBCALL] r=2561200 Wait(mask=h00000010)  task=00c05128
```

Decode with:

```sh
# summary across all calls
python3 tools/decode_libcall_trace.py build_kick_boot/run.log

# every AllocMem call in detail
python3 tools/decode_libcall_trace.py build_kick_boot/run.log --filter AllocMem

# just confirm a function fired (or didn't)
python3 tools/decode_libcall_trace.py build_kick_boot/run.log --first-of LoadSeg
```

## Common investigation recipes

**Did the program actually load?**

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_ALLOCMEM_TRACE' \
  make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
python3 tools/decode_libcall_trace.py build_kick_boot/run.log --filter AllocMem
```

LoadSeg allocates one block per hunk.  If boing has 28 hunks, expect
28 `AllocMem` calls starting around `r=<launch time>`.  None means
LoadSeg never even started reading the file's hunk header.  Some-but-
not-28 means LoadSeg failed midway — look for matching `FreeMem`
calls right after (they're the cleanup of the partial allocation).

**What's the task waiting for?**

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_WAIT_TRACE +define+LIBCALL_SIGNAL_TRACE' \
  make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
```

Look at the `Wait(mask=...)` and `Signal(task=..., mask=...)` lines.
If a task is `Wait`ing on `$00000010` (DOSSIG_PKT) and no `Signal`
ever delivers `$10` to that task, you've found a missed wake-up.

**What library is being opened next?**

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_OPENLIB_TRACE' make test-kickstart-boot ADFFILE=...
```

The `name_ptr` argument is a chip-RAM string pointer.  After the run,
look at the chip-RAM dump at that address to read the library name as
ASCII.  This is also how you capture a library's runtime base for
adding follow-up probes — D0 holds `LibBase` on RTS.

**Capturing DOSBase for dos.library probes**

dos.library is not in ROM, so its JT addresses depend on where K1.3
loaded it.  Capture the base from a first run:

```sh
EXTRA_VERI_DEFS='+define+LIBCALL_OPENLIB_TRACE' make test-kickstart-boot ADFFILE=...
grep -A1 'OpenLibrary.*name_ptr=' build_kick_boot/run.log | head -10
```

For each `OpenLibrary` hit, peek at chip RAM at `name_ptr` to find the
ASCII name.  The matching D0 in the next-cycle PC sample tells you
`LibBase`.  Once you know DOSBase, add a dos probe to
`rtl/k13_libcall_probes.v`.  See the `LIBCALL_DOS_LOADSEG_TRACE`
template at the bottom of that file.

## Adding a new exec LVO

1. Look up the function in the exec.library FD file (e.g.
   <https://github.com/cnvogelg/amitools/blob/master/data/fd/exec_lib.fd>)
   or the Amiga ROM Kernel Reference Manual.
2. JT address = `$C00276 + LVO` (negative LVO subtracts).
3. Add a new ``ifdef LIBCALL_<NAME>_TRACE`` block to
   `rtl/k13_libcall_probes.v` mirroring the existing ones.  Use
   `u_rf.regs[0]..[7]` for D0..D7 and `u_rf.regs[8]..[15]` for A0..A7.
4. Add a row to the table in this file and in the probes-file header
   comment.
5. Update `tools/decode_libcall_trace.py` if the function's args are
   special enough to deserve a custom formatter (most don't —
   `name=val` decoding works for everything).

## Adding a non-exec library

dos.library / intuition.library / graphics.library bases change every
boot.  Two ways to deal with this:

- **Hardcoded after a probing run.**  Use `LIBCALL_OPENLIB_TRACE` to
  capture the base from a representative run, then add a `\`define
  DOSBASE_VALUE 32'h00cN_NNNN` line and the matching probe block in
  `rtl/k13_libcall_probes.v`.  See the `LIBCALL_DOS_LOADSEG_TRACE`
  example at the bottom of that file.
- **Indirect, via OpenLibrary's return.**  If you only care about
  *whether* a library function fires (not *which call* it is), probe
  the implementation address directly (it's in ROM for resident
  libraries).  Disassemble the ROM at the JT entry to find the impl
  PC, then add a probe at that PC.

## Cost

Each probe is one comparator and one `$display` per matching cycle —
negligible at design-time and ~free at sim-time when the probe doesn't
fire.  When a hot probe (e.g. `LIBCALL_SIGNAL_TRACE` during heavy
scheduler activity) fires often, sim slows ~5-10× due to printf
volume.  Enable just what you need.
