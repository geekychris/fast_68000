# Cosim against MiniMig

We keep a small framework for running our chipset RTL **side-by-side**
with the upstream MiniMig FPGA Amiga clone (the `rkrajnc/minimig-mist`
repo). The idea: drive **identical stimuli** into both DUTs in a single
Verilator binary, then compare their externally-visible behaviour
(register reads, memory contents, IRQ levels, ...). Any divergence is a
candidate bug in our RTL; MiniMig is a long-debugged reference design
running real hardware.

This doc explains the pattern, the two cosims that already exist, and
how to add a new one.

## When to reach for cosim

Cosim is most valuable when:

* A boot-time investigation has narrowed the bug to *one chipset
  module* (e.g. "Paula INTREQ semantics" or "Blitter memory write
  pattern") and you can express the suspected behaviour as a sequence
  of register writes + observable reads.
* You're refactoring one of our chipset modules and want a fast
  regression that catches register-level behaviour drift.
* You're chasing a difference where Musashi-cosim doesn't help because
  the divergence is below the CPU bus, in the chipset.

Cosim is *not* the right tool when:

* The bug is in CPU instruction emulation — use Musashi cosim
  (`make crosscheck`) instead.
* The bug is in disk DMA / MFM decode — use `tools/adf2mfm.py` and
  byte-compare disk dumps.
* You need to validate timing accuracy across slot-accurate Agnus
  arbitration — MiniMig has its own quirks there and isn't a strict
  reference.

## The existing cosims

| Target | What it compares | Where the harness lives |
|---|---|---|
| `make crosscheck-minimig` | Paula INTENA / INTREQ / INTENAR / INTREQR semantics | `tb/minimig_xcheck_top.sv`, `tb/minimig_xcheck.cpp` |
| `make crosscheck-minimig-blt` | Blitter memory writes for a fixed sequence of `BLTCON0/1`, pointer, modulo, and `BLTSIZE` programs | `tb/minimig_blt_xcheck_top.sv`, `tb/minimig_blt_xcheck.cpp` |
| `make cosim-window SNAP_PC=...` | 68k CPU lockstep vs Musashi across a snapshot window | `tools/musashi_kick`, `tools/cosim_diff.py` |
| `python3 tools/fsuae_state.py …` | Full chip+slow+CPU+chipset state vs an FS-UAE save state | `tools/fsuae_state.py` (no harness — diff via `cmp`) |

The first two prove individual sub-modules byte-match.  `cosim-window`
proves the CPU runs the same code path Musashi does over a window
(useful for ruling the CPU out of a multi-session corruption hunt —
see WB13_DEBUG_JOURNAL.md §11).  The FS-UAE state diff is the
end-to-end ground truth: same K1.3 ROM + WB1.3 ADF on a software
emulator that's been hardened over decades against real hardware
behavior.  Diff our snapshot at any matched PC against an FS-UAE
save state and the long-lived pointers will match while the
working state diverges, pinpointing which K1.3 data structures
took a different branch.  WB13_DEBUG_JOURNAL.md §12–13 walks the
full discovery path.

Both pass at the time of writing. The Paula one has 2 documented soft
differences around `INTREQ` bit-14 writability (MiniMig keeps it
writable, HRM-correct behaviour reserves it).

**Known cosim-window trace-format quirk:** for MOVEM-load instructions
the Verilator and Musashi traces differ by one row at the MOVEM
line itself (Verilator emits post-load state, Musashi emits
pre-load).  Both converge on the next single-cycle instruction.
Always 2 diff hunks per MOVEM-load in the window — they're
emission-timing artifacts, not CPU divergences.  Ignore them.

## How to fetch MiniMig

```sh
make fetch-minimig
```

This shallow-clones `https://github.com/rkrajnc/minimig-mist.git` into
`external/minimig/`. We import only what we need (currently
`paula_intcontroller.v` and the `agnus_blitter*.v` files).

We do **not** modify the imported MiniMig files. If something needs to
change to make a cosim build, write a wrapper or a SystemVerilog `bind`
instead.

## The cosim pattern, top to bottom

A cosim has three pieces:

1. **A SystemVerilog top module** under `tb/` that instantiates
   *both* DUTs and exposes a stimulus port (`reg_addr`, `wdata`,
   `we`, ...) plus two read-back ports (`mm_*` and `our_*`).
2. **A C++ harness** under `tb/` that, via Verilator's generated
   wrapper, drives a fixed sequence of stimuli and asserts equality.
   Hard mismatches exit non-zero; documented soft differences are
   counted but tolerated.
3. **Makefile targets**: a build target that compiles the harness and
   a `crosscheck-foo` target that runs it.

### 1. The SystemVerilog top (`tb/minimig_<foo>_xcheck_top.sv`)

Conceptually:

```systemverilog
module minimig_<foo>_xcheck_top (
    input  wire        clk,
    input  wire        rst,
    // ...stimulus signals...
    output wire [15:0] mm_rdata,
    output wire [15:0] our_rdata
);

    // MiniMig module instance — pins wired to the same stimulus,
    // unused source pins tied off.
    <minimig_module> mm_dut ( ... );

    // Our module instance — its slv/master bus is adapted to match
    // the same conceptual interface.
    <our_module>     our_dut ( ... );

endmodule
```

The non-obvious part is **interface adaptation**. MiniMig and our
RTL don't use the same bus shape. For the Paula intcontroller:

* MiniMig: 16-bit data, 8-bit (word-addressed) register address, no
  byte enables.
* Ours: 32-bit data with byte enables, full byte-addressed register
  offset.

So `tb/minimig_xcheck_top.sv` (lines 64–86) does the mapping:

```systemverilog
wire [7:0] our_word_addr = {reg_addr[8:1], 1'b0};      // word→byte
wire       upper_half    = (our_word_addr[1:0] == 2'b00);
wire [3:0] our_be        = upper_half ? 4'b1100 : 4'b0011;
always @* begin
    our_slv_wdata = upper_half ? {wdata, 16'h0000}
                               : {16'h0000, wdata};
end
// On read, pick the matching half:
assign our_rdata = upper_half ? our_slv_rdata[31:16]
                              : our_slv_rdata[15:0];
```

When adding a new cosim, expect to spend most of your time on this
adaptation layer.

### 2. The C++ harness (`tb/minimig_<foo>_xcheck.cpp`)

A small Verilator-driven main that:

* Resets both DUTs.
* Walks a hard-coded sequence of `(op, address, data)` tuples.
* For writes: drives `we=1`, `addr`, `wdata`, ticks one cycle.
* For reads: drives `we=0`, ticks, samples `mm_rdata` and `our_rdata`,
  compares.

`tb/minimig_xcheck.cpp` uses a simple per-step struct:

```cpp
struct Step { const char* name; uint16_t addr; uint16_t data; bool is_write; };
```

The driver tolerates **documented soft differences** by masking the
known-divergent bits before comparing:

```cpp
bool match_raw    = (mm == ours);
bool match_masked = ((mm & ~0x4000) == (ours & ~0x4000));
const char* mark =
    match_raw    ? "OK"
  : match_masked ? "OK (bit14-INTREQ writable diff)"
                 : "MISMATCH";
```

Hard mismatches are counted and cause a non-zero exit. The summary line
prints `PASS` / `FAIL` plus mismatch counts, suitable for grepping in CI.

### 3. The Makefile targets

`Makefile` lines 340–402 define the existing two. The pattern is:

```make
MINIMIG_DIR   := external/minimig
MINIMIG_BUILD := build_minimig

$(MINIMIG_BUILD)/Vminimig_xcheck_top: tb/minimig_xcheck_top.sv \
                                       tb/minimig_xcheck.cpp \
                                       rtl/chipset/paula.v \
                                       $(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v
    @mkdir -p $(MINIMIG_BUILD)
    $(VERILATOR) -Wno-fatal --cc --exe --build \
        --noassert -CFLAGS "-std=c++17 -O1" \
        -I$(RTL_DIR) -I$(MINIMIG_DIR)/rtl/minimig \
        --top-module minimig_xcheck_top \
        -Mdir $(MINIMIG_BUILD) \
        -o Vminimig_xcheck_top \
        tb/minimig_xcheck_top.sv \
        rtl/chipset/paula.v \
        $(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v \
        tb/minimig_xcheck.cpp

crosscheck-minimig: $(MINIMIG_BUILD)/Vminimig_xcheck_top
    @(cd $(MINIMIG_BUILD) && ./Vminimig_xcheck_top) | tee $(MINIMIG_BUILD)/last.log
```

Always use a fresh `build_<name>/` directory for each cosim — Verilator
caches per-build-directory, and mixing modules across directories
breaks the cache.

## Adding a new cosim — concrete steps

Suppose you want to cosim **agnus_beamcounter** (the H/V beam counter).

1. **Identify the MiniMig file**: `external/minimig/rtl/minimig/agnus_beamcounter.v`. Read its port list and behaviour.
2. **Write the adapter top**:
   `tb/minimig_beam_xcheck_top.sv` instantiates both. If our beam-counter
   logic lives inside `m68k_bus.v`, you may need to expose it as a
   standalone module first, or instantiate the relevant slice via a
   `bind`.
3. **Decide what to compare**:
   * For combinational state machines: drive a clock and a few control
     signals, sample outputs after each tick.
   * For memory-touching modules (like the blitter): give both DUTs a
     parallel scratch RAM, run the operation, then `read_mm()` /
     `read_our()` for each word and compare.
4. **Write the C++ harness**:
   `tb/minimig_beam_xcheck.cpp`. Reset, drive a fixed sequence, sample
   outputs, count mismatches, exit non-zero on hard mismatch.
5. **Add a Makefile target**: copy the `crosscheck-minimig` block and
   adjust paths.
6. **Document expected soft diffs** in the harness (`match_masked`
   pattern). Both real Amiga and HRM are sometimes ambiguous; if our
   choice and MiniMig's choice are both defensible, declare it a soft
   diff and move on.

## What to capture and how to analyse

The harness already prints a per-step line like:

```
  R $01c -> mm=$4008 ours=$4008  OK  (check INTENAR)
  ...
  W $09c <- $c060  (set BLT|VERTB)
  R $01e -> mm=$4060 ours=$0060  OK (bit14-INTREQ writable diff)
```

That's usually enough for debugging. For deeper analysis:

* **Dump internal state on mismatch**: if you have access to internal
  signals via Verilator's `--public` flag or `verilator_lint_off`
  pragmas, print them when the first mismatch fires. Mirror MiniMig's
  internal state too if accessible.
* **Bisect the stimulus sequence**: when something fails, halve the
  step list to find the minimal-reproducing sequence.
* **Tie off vs leave-dangling**: when wiring up the adapter, *tie off
  all unused source pins on both DUTs identically* (both to `1'b0`
  or both to `1'b1`). Asymmetric tieoff produces false mismatches.

## Limitations

* **Cosim only catches divergence on the COMPARED interface.** If our
  Paula's IRQ-level output (`irq_level_o`) is wrong but the INTENAR
  readback is right, the existing Paula cosim won't catch it.
  Cosim is a checkpoint, not a proof.
* **MiniMig is not bit-perfect Amiga.** It's a community-built FPGA
  clone, very accurate in practice but with documented deviations
  (the INTREQ bit-14 case). Don't blindly chase MiniMig's behaviour
  if it contradicts the HRM.
* **Cosim doesn't catch timing-relative bugs** between modules.
  Agnus-DMA-vs-CPU contention, for example, won't show up in a
  beam-counter cosim alone.
* **The MiniMig clone is shallow-cloned.** Don't depend on commit-
  specific behaviour; pin a known revision if you do.

## Files at a glance

```
external/minimig/                  # shallow clone of minimig-mist
external/minimig/rtl/minimig/      # the source HDL we cosim against
tb/minimig_xcheck_top.sv           # Paula intcontroller adapter
tb/minimig_xcheck.cpp              # Paula harness
tb/minimig_blt_xcheck_top.sv       # Blitter adapter
tb/minimig_blt_xcheck.cpp          # Blitter harness
build_minimig/                     # Paula cosim build dir
build_minimig_blt/                 # Blitter cosim build dir
Makefile                           # crosscheck-minimig{,-blt} targets
```

## See also

* `BLITTER.md` — our blitter design notes; the blitter cosim is what
  caught the `+4-byte offset` bug we fixed against MiniMig's known-
  good output.
* `PAULA.md` — Paula design + INTENA/INTREQ semantics.
* `TESTING.md` — broader testing strategy; cosim is one of several
  layers.
