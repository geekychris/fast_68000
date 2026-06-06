# Full-system minimig boot in Verilator — design plan

**Status**: planning doc (task #170, spec-only).  Nothing has been built
against this plan yet.

## Why

We already have piecewise minimig comparisons:
- `crosscheck-minimig-blt` — our blitter vs `agnus_blitter`
- `crosscheck-minimig-cop` — our copper vs `agnus_copper` (MOVE-sequence)

These have been load-bearing diagnostically (the blitter USE_A=0 fix,
the BLTAFWM/BLTALWM longword-split fix).  But they only catch decode-
level regressions in individual modules.

For the remaining long-standing walls — the trackdisk-validator-
rejection at far cylinders (cyl 53, cyl 62 / boing.samples) and any
future heap-layout-divergence — the bug is most likely in **bus
arbitration / DMA slot timing** rather than any single module's decode.
That class of bug is invisible to per-module xchecks.

A **full-system minimig boot in Verilator** gives us a reference where:
- The chipset is real-hardware-validated (FPGA-deployed for years)
- The bus arbitration is true cycle-accurate
- The disk path uses minimig's `paula_floppy.v` (which is closer to real
  Paula than FS-UAE's high-level emulation)
- All comparisons happen at the same K1.3 ROM execution moments → easy
  side-by-side state diffs

The goal is **not** to replace our chipset with minimig's.  It's to
have a SECOND known-good reference (besides FS-UAE-no-UAEFS) that
shares more semantics with real-A500 hardware than FS-UAE does.

## What minimig provides

`external/minimig/rtl/minimig/` — 60 Verilog files, ~16K LOC total.
The relevant subset:

| File | Role |
|---|---|
| `minimig.v` | Top-level integration of all chipset modules + memory controllers + IO mux.  997 LOC. |
| `agnus.v` | DMA scheduler + slot arbitration.  Hosts blitter/copper/sprite/disk DMA channels. |
| `agnus_blitter*.v` | Already cross-checked. |
| `agnus_copper.v` | Already cross-checked (Phase 1). |
| `agnus_diskdma.v` | Disk DMA channel.  Handshakes with `paula_floppy`. |
| `agnus_bitplanedma.v` | Bitplane DMA slots. |
| `agnus_audiodma.v` | Audio DMA channels (AUD0..AUD3). |
| `agnus_spritedma.v` | Sprite DMA fetch. |
| `agnus_refresh.v` | Refresh slots. |
| `agnus_beamcounter.v` | VPOS/HPOS counters. |
| `paula.v` | Paula top.  Hosts interrupt logic, audio, disk control. |
| `paula_floppy.v` | Floppy controller (DSKLEN/DSKSYNC/etc.). |
| `paula_floppy_fifo.v` | MFM bit-stream FIFO between SPI source and DMA path. |
| `denise.v` | Bitplane shifter + sprite mux + color resolver.  Outputs RGB. |
| `denise_*.v` | Per-feature submodules (HAM, dual-playfield, etc.). |
| `ciaa.v`, `ciab.v` | CIA chips — keyboard, mouse, timers, parallel/serial, disk control. |
| `gary.v` | Address decoder.  Routes CPU/Agnus accesses to chip RAM / slow RAM / ROM / chipset. |
| `amber.v` | Scan-doubler.  Skip — we don't need video output. |
| `userio.v`, `osd.v` | OSD menu.  Skip. |
| `cart.v`, `akiko.v` | Action Replay / CD32.  Skip. |

**No 68000 CPU core in Verilog** — minimig uses `TG68` (VHDL,
`rtl/tg68k/`).  We can't drive TG68 through Verilator directly.

## Integration approach

### Top-level: our `m68k_core.v` + minimig chipset

Our existing 68000 core has been validated through `cpu_fuzz.py` (4000+
random cases clean vs Musashi) and the cosim infra.  Using it as the
CPU side cleanly separates concerns:

- **If full-system boot diverges from ours**: the bug is in our chipset
  (since the CPU is identical).
- **If full-system boot diverges from FS-UAE-no-UAEFS but matches
  ours**: the bug is shared between us and minimig — i.e. a real-Amiga
  semantic that FS-UAE handles differently.
- **If full-system boot matches FS-UAE-no-UAEFS but diverges from
  ours**: our chipset's slot-arbitration or DMA-timing model differs
  in a way that matters for K1.3 trackdisk.

The third case is the interesting one for the cyl-51 wall.

### Bus-interface impedance match

Our `m68k_core` outputs a 32-bit master bus with `mst_req` / `mst_we` /
`mst_addr` / `mst_be` / `mst_wdata` / `mst_ack` / `mst_rdata`.

Minimig's `minimig.v` exposes a 68000-style bus:
```
cpu_address [23:1]  // word address
cpu_data    [15:0]  // CPU→bus
cpudata_in  [15:0]  // bus→CPU
_cpu_as              // address strobe (active low)
_cpu_uds / _cpu_lds  // upper/lower data strobe (active low)
cpu_r_w              // 1=read, 0=write
_cpu_dtack           // bus acknowledge (active low)
_cpu_ipl[2:0]        // interrupt level (active low)
```

A small wrapper translates:
- 32-bit `mst_addr[23:1]` → minimig's `cpu_address[23:1]`
- `mst_we && mst_be != 0` → `_cpu_as=0`, drives `_cpu_uds/_cpu_lds` from `mst_be[3:2]/[1:0]`
- `mst_we=0 && mst_req=1` → read transaction, same UDS/LDS pattern
- minimig's `_cpu_dtack=0` → our `mst_ack=1`
- `_cpu_ipl[2:0]` → our `core_intena_ipl`

Conservative timing: hold our master signals stable until `_cpu_dtack`
asserts.  Matches what our core does for slow chip-RAM accesses already.

### Memory

Minimig drives a 16-bit SRAM bus (`ram_data` / `ram_address` / `_ram_we`).
For Verilator we provide an in-process memory model:
- 512 KiB chip RAM at `$00_0000-$07_FFFF`
- 512 KiB slow RAM at `$C0_0000-$C7_FFFF`
- 512 KiB ROM at `$F8_0000-$FF_FFFF` (loaded from `kick_13.bin`)
- OVL bit: at boot, mirror ROM at `$00_0000` until first `OVL=0` write

Identical to our existing memory model.  Should drop in.

### Disk / MFM

This is the **tricky part**.  Minimig's `paula_floppy.v` reads MFM data
from an SPI interface (intended for a microcontroller-driven SD card
on real hardware).  Ports:
```
_scs / sdi / sdo / sck       // SPI signals
direct_scs / direct_sdi      // alt direct-from-SD path
```

Two options:

**Option A (simpler): replace `paula_floppy.v` with our trackdisk**.
Our `rtl/chipset/paula.v` already handles DSKLEN/DSKSYNC/DSKBYTR and
sources MFM data from a pre-loaded `disk.hex`.  We can swap in our
trackdisk while keeping all of minimig's other Paula bits.  This loses
the "minimig's disk path is the reference" benefit but unblocks
booting fast.

**Option B (better long-term): write a Verilator SPI-master testbench
peripheral** that exposes our ADF data through the SPI signals
minimig's `paula_floppy.v` expects.  Needs reverse-engineering the SPI
protocol minimig defines for its MCU-driven disk path
(`paula_floppy_fifo.v` is the buffer side; the SPI command set is
documented in minimig source comments).  ~200-400 LOC of Verilator C++
peripheral logic.

**Recommendation**: start with Option A to get *something* booting,
then upgrade to Option B once the boot reaches a useful steady state.

### Skipped modules

The following minimig modules aren't needed for boot-up to a comparable
state and can be either stubbed or omitted:

| Module | Why skipped |
|---|---|
| `amber.v` | Scan-doubler.  We capture Denise pre-amber output for comparison. |
| `userio.v`, `osd.v`, `osd_*.v` | On-screen-display menu.  Pure UX feature. |
| `cart.v`, `akiko.v` | Action Replay / CD32.  Not on A500. |
| `gayle.v`, `gayle_fifo.v` | IDE controller (Gayle).  A1200/CD32 only. |
| `clock/*.v`, `mist/*.v`, `soc/*.v` | FPGA-board glue + PLL.  We drive clocks from the testbench. |
| `denise_colortable_ram_mf.v`, `denise_hamgenerator_ram_mf.v` | FPGA-vendor RAM macros.  Replace with simple inferred RAM. |

## What we wire and how

```
                                 ┌──────────────────────────────────┐
                                 │  tb/minimig_fullsys_top.sv       │
                                 │                                  │
   ┌──────────────┐   bus     ┌──┴──────────┐   chipset bus  ┌───────┴──┐
   │ m68k_core.v  │──────────►│  cpu-bus    │───────────────►│ minimig  │
   │ (our CPU)    │           │  wrapper    │                │ chipset  │
   └──────────────┘           └─────────────┘                │ subset   │
                                     ▲                       └───┬──────┘
                                     │                           │
                              ┌──────┴────┐                ┌─────▼─────┐
                              │ chip/slow │                │  Denise   │
                              │  /ROM mem │                │  RGB out  │
                              └───────────┘                └─────┬─────┘
                                                                 │
                                                          render_screen.py
                                  ┌─────────────────────┐
                                  │ tb/minimig_fullsys_ │
                                  │      main.cpp       │
                                  │ - load kick.bin     │
                                  │ - load ADF as MFM   │
                                  │ - tick clock        │
                                  │ - capture state     │
                                  └─────────────────────┘
```

## Phase plan

### Phase 0 — Smoke test (1-2 sessions)

- Set up `tb/minimig_fullsys_top.sv` with stub minimig chipset (just
  enough to respond to CPU bus cycles with NOPs / zeros).
- Wire our `m68k_core.v` to the wrapper bus.
- Load K1.3 ROM, no disk.
- Goal: verify the CPU executes ROM code through the wrapper bus
  without bus protocol violations.  Should reach the first DSKLEN
  write (~r=200K) and stall waiting for disk.

### Phase 1 — Chipset register access (1-2 sessions)

- Replace the chipset stub with real `minimig.v` instantiation, with
  unused modules stubbed or excluded.
- Pull in `agnus.v`, `paula.v` (without floppy), `denise.v`, `ciaa.v`,
  `ciab.v`, `gary.v`.
- Verify chipset register reads/writes work: K1.3 should reach the same
  `[STOP] PC=00fc0f90` idle state our sim does.

### Phase 2 — Disk subsystem (2-3 sessions)

- Option A path: substitute our `paula_floppy.v` for minimig's.
- Verify K1.3 trackdisk loads the bootblock.
- Compare DSKLEN-event timing against our sim and FS-UAE.

### Phase 3 — Bitplane DMA + Denise readout (1-2 sessions)

- Capture Denise's pixel output per frame.
- Cross-check against our `render_k13_screen.py` output for the same
  WB1.3 boot state.

### Phase 4 — Comparison rig (1 session)

- Add per-cycle DMACON/INTREQ/BPLCON0 snapshots from minimig.
- Diff against our sim's snapshots at matched retired-instruction
  counts.
- First target divergence: track 125 (cyl 62 head 1, boing.samples) —
  does minimig's `paula_floppy` deliver MFM that K1.3 accepts?  If yes,
  the bug is in our trackdisk/MFM-decode path.  If no, the bug is
  upstream in K1.3's expectation of disk timing.

## Risk and what could go wrong

- **TG68 substitution mismatch**: minimig's chipset was developed
  assuming TG68 bus semantics.  Our CPU may issue cycles in patterns
  that minimig's bus arbiter doesn't expect (e.g. back-to-back without
  the proper `_cpu_as` gap).  Mitigation: add the same conservative
  timing model used for our slow chip-RAM accesses.

- **Clock-enable model differs**: minimig uses `clk7_en` /
  `clk7n_en` / `c1` / `c3` / `cck` derived from a 28.37 MHz master
  clock to model the actual Agnus timing slots.  We need to generate
  these enables correctly in the testbench (~50 LOC).

- **paula_floppy SPI protocol** (Option B): proprietary minimig
  microcontroller protocol.  Need to dig into MIST/MiSTer firmware
  source to understand the byte stream.  Risk: weeks of reverse-engineering
  for unclear payoff if Option A works.

- **Verilator-compile time**: 16K LOC of minimig + our 8K LOC + harness
  ≈ 30-60s build.  Reasonable.

## Open questions for next session

- Do we actually want Phase 4 to be a *live* per-cycle differ, or a
  *snapshot* differ at boot-state milestones (RESINIT, DSKLEN edges,
  idle Wait)?  Snapshot is simpler and probably enough for the cyl-51
  diagnosis.
- Is there a TG68→Verilog netlist somewhere (yosys + ghdl)?  Would
  remove the CPU-substitution variable from the equation.  Worth
  ~1 session investigation if Phase 2 reveals a CPU-bus-protocol
  issue.

## Cost estimate

Conservative session budget for full Phase 0-4:
- Phase 0: 1-2 sessions
- Phase 1: 2 sessions
- Phase 2: 3 sessions (Option A); 5-8 sessions (Option B)
- Phase 3: 2 sessions
- Phase 4: 1 session

Total: **~9 sessions for Option A** (functional boot + snapshot
comparison), **~13 sessions** for the full Option B path.  Recommend
committing to Option A first, deciding on Option B after Phase 2
results.
