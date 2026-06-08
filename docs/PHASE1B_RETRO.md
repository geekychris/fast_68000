# Phase 1b retrospective — minimig.v DTACK chain

Session arc this conversation hit a wall at **minimig.v never asserts
`_cpu_dtack`** for the K1.3 reset-vector fetch.  Below: complete map of
what was tried, what works, what doesn't, and where the next focused
session should start.

## What's confirmed working (commits 992d283 → 4acd22d)

- `m68k_core` boots K1.3 ROM through a bare bus arbiter (Phase 0, commit
  `992d283`).  262K instructions retired before hitting the chipset
  stub wall — proves the CPU is reusable outside `m68k_top`.
- 68000-style bus wrapper translates the CPU's 32-bit `ic_*`/`dc_*`
  ports into `_AS`/`_UDS`/`_LDS`/`_DTACK` protocol (Phase 0.5, commit
  `bd62134`).  416K instructions retired through the wrapper — proves
  the bus protocol layer is correct.
- `minimig.v` + all 60 sub-modules + `altsyncram` stub + `sync_fifo`
  build cleanly through Verilator (Phase 1a, commit `7a2b65c`).
- Clock-enable cadence (`clk7_en` / `clk7n_en` / `c1` / `c3` / `cck` /
  `eclk`) mirrors `external/minimig/rtl/clock/amiga_clk.v` exactly
  (commit `abba5d8`).

## What's confirmed broken

**Symptom**: `make phase1-smoke` runs 5M cycles with `retired=0`.  CPU
never executes a single instruction through minimig.

**Probe output** (`PHASE1_DEFS=+define+PHASE1_PROBE`):
```
[probe] AS falling: addr=fc00d2 rw=1 uds=1 lds=1 cpudata_in=0000
... no DTACK falling event in 5M cycles ...
```

The CPU asserts `_AS` once at the K1.3 RESET_PC (`$FC00D2`) for an
instruction fetch with both `_UDS` and `_LDS` asserted (word read).
That's a correct, well-formed 68000 bus cycle.  Minimig never returns
`_cpu_dtack` low, so the wrapper waits forever.

## Dtack chain inside minimig

Walking the `_cpu_dtack` chain back from minimig.v's CPU port:

```
minimig.v _cpu_dtack ← minimig_m68k_bridge._dtack
                       _dtack = _ta_n              (line 211)
                       _ta_n asserts when:           (line 198-205)
                         clk7n_en pulse
                       && !l_as
                       && cck
                       && (   (!vpa && !(dbr && dbs))
                            || (vpa && vma && eclk[8]) )
                       && !nrdy
```

Each of those signals:

| Signal | Source | Likely state |
|---|---|---|
| `clk7n_en` | testbench drives | ✓ pulses every 4 cycles |
| `l_as` | bridge samples `_as` on `clk7_en` | unknown — needs probe |
| `cck` | testbench drives | ✓ toggles every clk7_en |
| `vpa` | bridge decode of address | $FC0000 is NOT VPA (only CIA $BFExxx is), so vpa=0 |
| `dbr` / `dbs` | Agnus blitter DMA | should be 0 (no blit pending) |
| `nrdy` | gayle.v (IDE) | `pio_in & sel_fifo & fifo_empty` — should be 0 (no IDE access) |

Most likely first culprit: **`l_as` never goes low** because
something gates the sync flop, or because halt=1 makes l_as sample
`!host_cs` (which is 1) instead of `_as` (which is 0).

## Things tried this session that DIDN'T move the needle

1. **Override `minimig_syscontrol`** via `tb/minimig_syscontrol.v` +
   `-y tb` in Verilator's library path (commit `6c553ec`).  My
   override deasserts `reset` from a fixed delay after `mrst` low,
   bypassing the original's dependence on Agnus's `sof` pulses.  No
   change to `retired=0`.  Either the override didn't take effect
   (file-precedence issue) or syscontrol's reset wasn't the gate.
2. **Tying off all minimig board-level inputs** (SPI, PS2, joystick,
   `_15khz`, etc.) to safe defaults.  Verified no crash from any
   unconnected port, but no boot progress either.

## Next-session attack plan (in priority order)

1. **Confirm syscontrol override took effect**: probe the `reset`
   signal coming out of `CONTROL1` in minimig.v.  If it's still
   asserted (high), the override file isn't being picked up — switch
   to a different override mechanism (e.g., `-Imodules tb` or directly
   patching minimig.v's body).
2. **Probe `l_as`, `l_dtack`, `halt`, `nrdy` from outside the bridge**:
   `Verilator --public` on those signals, then read them from the C++
   harness.  Or add `$display`-on-change inside `minimig_m68k_bridge.v`
   (one-line patch).  Identify which signal in the dtack chain is
   stuck.
3. **Try driving `_cpu_reset_in` differently**: minimig's spec might
   want it ACTIVE LOW at the moment of valid bus cycles, ACTIVE HIGH
   during init.  Current wiring `._cpu_reset_in(rst_n)` matches the
   underscore-prefix convention (active low) but worth double-checking
   against the spec in `minimig.v`'s header comment.
4. **Re-examine clock cadence vs minimig's expectations**: walk
   `amiga_clk.v`'s c1/c3 timing diagram (the ASCII art in the
   comments) and confirm our cadence matches exactly.  An off-by-one
   phase in `c1` could starve the chip-RAM SRAM bridge.
5. **If 1-4 don't reveal it**: stand up a Verilator VCD dump
   (`--trace`), feed it into GTKWave, and visually walk the signals
   from AS falling forward.

## Why this is multi-session work

minimig.v + sub-modules total ~16K LOC of hand-tuned FPGA timing.
There are several "obvious" enable conditions and a number of subtle
ones, and the chain from `_as` to `_dtack` runs through 3-4 modules
gated by different combinations of clock enables.  Verilator-level
debugging needs either signal-by-signal probing (Verilator `--public`
+ printfs, slow but methodical) or VCD + GTKWave (fast but requires
setup).  Either approach is naturally an N-hour focused session, not
a series of conversational "continue" beats.

## What's reusable from this session for the future try

- `tb/minimig_phase1_top.sv` — complete CPU+minimig wiring, all
  board-level ports tied off
- `tb/m68k_to_amiga_bus.sv` — Phase 0.5 bus wrapper, validated through
  the bare-memory smoke test
- `tb/altsyncram_stub.v` — Altera megafunction shim
- `tb/minimig_syscontrol.v` — syscontrol override (effectiveness TBD)
- `+define+PHASE1_PROBE` — AS+DTACK falling-edge logger
- `make minimig-phase1` / `make phase1-smoke` — build + run targets
- This doc — Phase 1b knowledge map
