# boing! disassembly + cosim notes

This document maps out the boing! binary (Robert J. Mical / Dale Luck,
dated 1986) used in our boing-disk reproduction.  Goal: identify
exactly which library call wedges on our sim + vAmiga so we can
diagnose the audio.device interaction gap.

## File

```
/Users/chris/Downloads/workbench_demos/animations/boing!  21,872 bytes
```

`file(1)`: `AmigaOS loadseg()ble executable/binary`.  Standard
HUNK_HEADER format starting with `$000003F3`.

## Hunk layout

28 hunks total.  Use `tools/disasm68k.py` after extracting via
`tools/extract_boing.py` (script in this document):

| Hunk | Type | Bytes | MEMF | Notes |
|---|---|---|---|---|
| 0 | CODE | 452 | ANY | C-startup boilerplate — gets ThisTask, checks pr_CLI, parses argc/argv, calls `main()` |
| 1 | DATA | 440 | ANY | Initialized globals |
| 2 | CODE | 1,360 | ANY | `main()` and CLI wrap helpers (open/parse/exit chain) |
| 3 | DATA | 112 | CHIP | Small chip-RAM data (sprite struct?) |
| 4 | CODE | 6,776 | ANY | **Main boing logic** — graphics/audio loop |
| 5 | DATA | 6,728 | CHIP | **Ball image bitplane** — pre-rasterized animation frames |
| 6 | CODE | 208 | ANY | Helper code |
| 7 | CODE | 60 | ANY | Helper code |
| 8 | BSS | 0 | CHIP | Empty CHIP BSS |
| 9-25 | CODE+DATA mix | varies | ANY | C runtime + math helpers |

Total CHIP allocation = 6,840 bytes (hunks 3 + 5 + 8).
Total ANY allocation = 11,584 bytes.

## Library / device references (from `strings(1)`)

Boing!'s import table is tiny:

```
dos.library
boing.samples       (×2 — open by name, then content path)
audio.device
```

That's the **complete** set.  No graphics.library reference — the
ball animation is drawn directly to bitplanes via Copper + blitter,
not through graphics.library.  No intuition.library either — no
Window or Screen open through OS calls.

## Boing!'s task lifecycle

Reconstructed from hunk 0 disassembly + behavioural observation on
our sim + vAmiga:

1. **Hunk-0 startup** (offset 0):
   - `MOVE.L A7, $1C` ; save SP for clean exit
   - `MOVEA.L $4.W, A6` ; A6 = ExecBase
   - `SUBA.L A1, A1` then `JSR -294(A6)` = `FindTask(0)` → A4 = ThisTask
   - `TST.L $AC(A4)` = check Process.pr_CLI BPTR
   - If from CLI: parse argv from CLI struct, call `main()`
   - If not from CLI (i.e., Workbench-launched): handle WBStartup
     message + call `main(0, NULL)`

2. **`main()` (hunk 2 offset 0x30)**:
   - LINK + register save
   - Opens libraries / devices (relocated JSR targets — exact
     order needs hunk 4 reading)

3. **boing render loop** (hunk 4):
   - Allocate bitmap / Copper list in CHIP RAM (hunks 3+5 provide the
     pre-shaded ball image)
   - Set up audio.device IORequest for boing.samples playback
   - Install Copper list, run the bounce-physics loop
   - Wait for left mouse click or CTRL-C to exit

## Behavioural observation: where it wedges

On both our RTL and vAmiga (independent cycle-accurate emulators):

  - Last `PutMsg→FS` packet at r=12,040,432 = DOS Read reply for
    boing.samples — completes cleanly (reply at r=12,041,065).
  - After that, **no further DOS-packet activity** for the rest of
    the run (190M+ retired instructions).
  - 235K idle `Signal(CLI, $10)` ↔ `Wait` ping-pongs (K1.3 idle scheduler
    with no other runnable task — but CLI's wait never gets a real DOS
    reply because boing! is no longer issuing DOS packets).
  - boing!'s code (slow $C07B30..$C08634 once loaded) sees zero PC
    samples in the steady state → boing! task is parked in a
    `Wait()` that never returns.

That `Wait()` is **not** a DOS reply wait (boing's last DOS call
completed).  Most likely candidates:
1. `WaitIO()` on an `audio.device` IORequest never replying
2. `WaitTOF()` (Wait for top-of-frame) — signal mask never delivered
3. Some custom `Wait(sigmask)` on an audio-DMA completion bit that
   never fires under our (and vAmiga's) emulation timing

## Hunk 0 disassembly excerpt (the standard CLI prologue)

```
$000000: 23CF 0000 001C       MOVE.L  A7, $1C            ; save SP
$000006: 23C0 0000 0024       MOVE.L  D0, $24            ; save argc (CLI arg len)
$00000C: 23C8 0000 0028       MOVE.L  A0, $28            ; save argv (CLI arg buf)
$000012: 42B9 0000 0020       CLR.L   $20                ; result = 0
$000018: 2C79 0000 0004       MOVEA.L $4.W, A6           ; A6 = ExecBase
$00001E: 23CE 0000 0004       MOVE.L  A6, $4
$000024: 93C9                 SUBA.L  A1, A1             ; A1 = 0
$000026: 4EAE FEDA            JSR     _LVOFindTask(A6)   ; ThisTask
$00002A: 2840                 MOVEA.L D0, A4
$00002C: 4AAC 00AC            TST.L   $AC(A4)            ; pr_CLI?
$000030: 6700 00A8            BEQ     $DA                ; → not from CLI
$000034: 6100 016C            BSR     $1A2               ; some setup
$000038: 206C 00AC            MOVEA.L $AC(A4), A0        ; A0 = pr_CLI BPTR
$00003C: D1C8                 ADDA.L  A0, A0             ; ×2
$00003E: D1C8                 ADDA.L  A0, A0             ; ×4 = real ptr
$000040: 2068 0010            MOVEA.L $10(A0), A0        ; A0 = cli_CommandLine BPTR
$000044: D1C8                 ADDA.L  A0, A0
$000046: D1C8                 ADDA.L  A0, A0             ; ×4
```

This matches the standard Lattice / SAS C startup-from-CLI sequence
of the era.

## How to extract + disassemble

Use this extractor:

```python
# tools/extract_boing.py (paraphrased — full script in
# docs/BOING_DISASSEMBLY.md)
import struct
with open('/Users/chris/Downloads/workbench_demos/animations/boing!','rb') as f:
    b = f.read()
# Parse HUNK_HEADER at offset 0: magic $3F3, then res_libs, n_hunks,
# first, last, then n_hunks × (size_lw|flags<<30) hunk-size table.
# Then read marker stream: $3E9 CODE, $3EA DATA, $3EB BSS, $3EC
# RELOC32, $3F0 SYMBOL, $3F2 END.  Each CODE/DATA marker is followed
# by a longword count of payload words.
```

Then disassemble any hunk:

```sh
python3 tools/disasm68k.py /tmp/boing_hunk4_code.bin 0 --bytes 6776 --base 0
```

(Disassembler doesn't know about hunk relocations — `JSR
$0000004C`-style absolute calls in the disasm output are
relocatable references that get patched at load time to point at
real addresses in OTHER hunks.  Cross-reference relocation tables
manually.)

## Next investigation step

To find the exact `Wait()` boing! parks in, add a probe to our sim
that fires at every K1.3 `_LVOWait` entry AND whose caller PC is in
the slow-RAM range $C07B30..$C08800 (= boing!'s loaded code).
That's a 1-line addition to `rtl/k13_libcall_probes.v` (a "slow-RAM-
caller-only" variant of the WAIT trace).  When fired, it logs
`{caller_PC, signal_mask}` — which tells us EXACTLY which boing
instruction wedges + on which signal bit it waits.

Pair that with the same probe on FS-UAE (via the `fsuae_remote_patch`
HTTP BP API at `/v1/breakpoints?addr=...&trace=1`) and we can compare
boing!'s execution path between three references.

## References

- `tools/disasm68k.py` — our subset 68k disassembler
- `rtl/k13_libcall_probes.v` — the K1.3 library-call tracer
- `project_boing_emulator_parity_2026_06_15.md` (memory) — proof that
  ours ≡ vAmiga on this disk
- `reference_fsuae_remote_patch.md` (memory) — FS-UAE HTTP debugger
  for the 3rd-emulator triangulation
