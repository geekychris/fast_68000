# Slot-accurate Agnus arbiter — design plan (task #4)

**Status**: planning doc.  Nothing has been built against this plan yet.

## Why

The long-standing trackdisk-validator-rejection wall (cyl-53 / cyl-62
boing.samples, see `project_wb13_trackdisk_stops_cyl51.md` and
`project_boing_samples_track125.md`) is most likely a bus-arbitration
timing bug.  The K1.3 trackdisk validator rejects valid MFM decodes at
far cylinders even though our blitter produces correct `$4489` sync
words.  Cross-validation shows minimig handles the same MFM correctly,
so the difference is in *when* the cycles happen, not *what* the
modules produce.

Our current bus arbiter uses **round-robin priority** (see
`rtl/m68k_bus.v:776-796`).  Real Agnus uses **cycle-position priority**:
each cycle in the 227-cycle PAL line is pre-assigned to a specific DMA
channel by position, with CPU/blitter/copper taking only what's free.

## Current state

`rtl/m68k_bus.v:776-796` — single round-robin loop with one lock
mechanism:

```verilog
always @* begin
    winner = 0;
    if (lock_pending)         winner = lock_holder;
    else for (k = 0; k < N_PORTS; k++) begin
        cand = (rrobin + k) mod N_PORTS;
        if (req[cand]) { winner = cand; break; }
    }
end
```

Ports (per `m68k_bus.v` near line 1188):
- 2 × `CPU_PORT` (ic + dc per core, 1 core in the current K1.3 build)
- `BLT_PORT`   — blitter DMA master
- `COP_PORT`   — copper
- 2 more chipset masters (Paula/disk-DMA, Agnus bitplane DMA)

The arbiter has no concept of slot position.  Any free channel that
asks wins (modulo round-robin fairness).  Compare to real Agnus: a
bitplane DMA cycle MUST happen at its assigned slot; CPU can't preempt
it; blitter has to wait for an odd slot.

## What real Agnus does

PAL line = 227 cycles indexed by `hpos[8:0]`.  Each slot has a
pre-assigned use:

| Cycle range | Use |
|---|---|
| 0-7 (every line) | Refresh DRAM, disk DMA (3 slots if DSKEN), audio DMA (4 slots if AUDxEN) |
| 8-19 (visible lines only) | Sprite DMA (16 slots if SPREN, 2 per sprite × 8 sprites) |
| 20-219 (visible lines only) | Bitplane DMA (up to 60 slots if BPLEN, depends on BPU + hires) |
| 220-226 | Idle / refresh |

**Priority within a slot** (highest first):
1. Refresh (always first, fixed cycles)
2. Disk DMA (only at assigned disk slots)
3. Audio DMA (only at assigned audio slots)
4. Sprite DMA (only at assigned sprite slots)
5. Bitplane DMA (only at assigned bitplane slots)
6. Copper (any free even slot)
7. Blitter (any free even slot)
8. CPU (any free odd slot, except $E0 reserved for ECLK)

CPU only loses cycles when the slot is pre-assigned AND the channel is
active.  With BPLEN=0 + SPREN=0 + AUDxEN=0 + DSKEN=0, CPU gets every
cycle.  With BPLEN=1 + BPU=4 + lowres, CPU loses ~40% of visible-line
cycles.

## Where our current impl falls short

| Channel | Our impl | Real Agnus | Impact |
|---|---|---|---|
| Refresh | Not modeled | Fixed cycles 0,2,4,6 | None functionally (DRAM is sim mem) |
| Disk DMA | Round-robin port | 3 fixed cycles when DSKEN | Trackdisk timing wrong → suspected cyl-53 fix |
| Audio DMA | Round-robin port | 4 fixed cycles when AUDxEN | Audio glitching (not yet visible) |
| Sprite DMA | Round-robin port | 16 fixed cycles when SPREN | Sprite skewing in long lists |
| Bitplane DMA | Approximate per-row burst | Slot-by-slot, gated by BPLCON0 | Display tearing on hires/HAM, fixed in #3 |
| Copper | Round-robin port | Even slots only | Copper instructions timing off |
| Blitter | Round-robin port | Even slots only | Blits faster than real → buffer races |
| CPU | Round-robin port | Odd slots only when DMA contending | CPU runs at "max DMA" rate, not real |

The **trackdisk timing** row is the load-bearing one for the cyl-53
fix.  If our disk DMA happens at the wrong cycle relative to MFM
sync-edge events, K1.3's validator sees correct data at the wrong
moment and rejects it.

## Phased rewrite plan

### Phase A — Refactor without behavior change (1 session)

- Pull the `always @* begin ... winner = ... end` block out of
  `m68k_bus.v` into a new module `rtl/chipset/agnus_arbiter.v`.
- Keep round-robin semantics identical.
- Add a `hpos[8:0]` and `vpos[9:0]` input from the existing beam
  counter (already present in `m68k_bus.v`).
- Verify: 153/153 tests still pass.

### Phase B — Hard-coded refresh slots (1 session)

- At hpos in {0, 2, 4, 6}: reserve slot for "refresh" (deny all
  requesters).  The bus master becomes a no-op consumer (memory still
  works because we don't model DRAM refresh).
- Verify: 153/153 tests still pass.  Slight timing shift in WB1.3
  boot reach is expected and OK.

### Phase C — Disk DMA at assigned slots (2 sessions)

- DSKEN gates 3 cycles per line: hpos in {7, 9, 11} (typical
  positions; verify against HRM appendix C).
- Disk DMA channel only requests at those cycles when DSKEN+DMAEN.
- CPU/blitter denied at those cycles even if disk channel idle.
- Verify: t86_dsksyn_irq + WB1.3 boot reach unchanged.  This is the
  ONE that should unblock cyl-53.

### Phase D — Audio slots [LANDED]

- AUDxEN gates per-channel audio slots at hpos 13/15/17/19.
- Gated by master DMAEN (DMACON bit 9) — caller passes zero when
  DMAEN low so the arbiter doesn't have to re-check.
- Verified: 153/153 tests pass with and without
  `+define+SLOT_ACCURATE_AGNUS`.

Implementation lives in `rtl/chipset/agnus_arbiter.v` `is_audio_slot`
wire + the `audn_en` input port threaded from m68k_bus.v's
`dmacon[3:0]`.

Sprite slot reservation was attempted in a first draft as `is_sprite_slot`
gated by SPREN, but SPREN alone over-reserves: real Agnus only fetches
sprites on lines where a sprite is active (within its vstart/vstop
range).  Even gating on "visible lines" still over-reserves by ~10× for
typical OS usage (1-sprite mouse pointer).  Deferred to Phase E along
with bitplane slots — both need vertical state to be fidelity-correct.

Also fidelity-fixed during this phase: refresh slot reservation was
`hpos[8:1] < 4` (= hpos 0..7, 8 cycles), 2× the real-hardware value.
Per HRM appendix C, refresh occupies cycles 0, 2, 4, 6 only (4 cycles
per line).  Corrected to `!hpos[0] && hpos[9:1] < 4`.

### Phase E — Bitplane slots (2-3 sessions)

- BPLEN + DDFSTRT/DDFSTOP gate bitplane fetch slots, varies by
  BPU/hires.  Existing approximation in `m68k_bus.v` becomes the
  reference; replace with slot-by-slot.
- Verify: WB1.3 render byte-identical to current at idle.

### Phase F — Even/odd slot enforcement for blitter/copper/CPU
       (1-2 sessions, PARTIAL)

- Blitter + Copper request slots numbered even; only granted on
  even-cycle phases.  **Deferred** — needs port-type info threaded to
  the arbiter (which ports are CPU vs blitter/copper).  Skipping for
  now because the existing arbiter already gives blitter/copper fair
  shots via round-robin; the missing parity restriction has mild
  fidelity impact.
- CPU on odd slots.  **Deferred** same reason as above.
- $E0 reserved for ECLK.  **LANDED.**  Single-cycle reservation at
  hpos = 224 in `is_eclk_slot`, denied unconditionally under
  `SLOT_ACCURATE_AGNUS`.  Matches real Agnus's 6800-bus-compatible
  peripheral slot, giving CIAs an exclusive bus moment.
- Verify: cpu_fuzz.py still clean (no instruction-timing bugs).

ECLK reservation is verified by the `crosscheck-arbiter` unit test:
"ECLK slot rejects CPU req at hpos 224 only" — confirms hpos 224
denies grants while adjacent hpos 223/225 stay free.

Remaining Phase F parity-of-port work is logged here for future
sessions when port-type plumbing becomes worthwhile.

## Risk

- **Breaks the existing 153-test suite.**  Each phase needs to be
  guarded behind a `+define+SLOT_ACCURATE_AGNUS` flag during dev, so
  the default behavior is unchanged until the rewrite is complete and
  validated.
- **Performance**: per-cycle decode adds ~10-20% Verilator overhead.
  Acceptable.
- **Subtle CPU timing changes** could expose latent bugs in `m68k_core`'s
  bus-cycle handling.  Mitigate via cpu_fuzz.py regression.

## Cost estimate

Phases A-F: ~10 sessions total (each ~2 hours of focused work).  The
trackdisk-fix payoff lands in Phase C (3rd session).  Phases D-F are
fidelity-improvements without an obvious user-visible win, so could be
deprioritized after C unblocks the cyl-53 wall.

## Cross-validation

Once Phase C lands, the minimig blitter cross-check (`make
crosscheck-minimig-blt`) should still byte-match.  The new disk-DMA
slot timing should let K1.3 accept far-cylinder reads.

If after Phase C the cyl-53 wall is STILL there, the bug isn't bus
timing — it's MFM-encoding fidelity in `tools/adf2mfm.py`.  That
falsifies the bus-timing hypothesis and re-points the investigation.

## Open questions

- Do we need hpos to come from a separate Denise H counter, or does
  the existing `agnus_h` in `m68k_bus.v:397` suffice?  (Probably the
  latter — it already advances per cycle.)
- What's the right unit test for slot-accurate disk DMA?  Probably a
  new `tests/t170_disk_slot_timing.s` that arms a known DSKLEN at a
  known beam position and verifies the read happens at the expected
  cycles.
