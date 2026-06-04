# WB1.3 boot — visual progress log

Each PNG is `tools/render_k13_screen.py` rendering a chip-RAM snapshot
captured by the Verilator sim at some point during the Kickstart 1.3
+ Workbench 1.3 boot sequence, walking the live Copper list to recover
the active bitplane configuration. FS-UAE reference shots come from
the patched FS-UAE save-state pipeline (`tools/fsuae_state.py`),
rendered through the **same** Python tool — so anything different
between two renders is a chip-RAM difference, not a rendering
difference.

Filenames: `YYYYMMDD_HHMMSS_<label>.png`.

---

## Reference target

What the same Python renderer produces from the patched FS-UAE
save-state at WB1.3 idle. This is what "pristine" looks like:

![FS-UAE reference](20260604_102517_fsuae_reference_640.png)

Solid white CLI title bar with "AmigaDOS" text, full banner
("Copyright ©1987 Commodore-Amiga, Inc.", "Release 1.3", "A500/A2000
U.K. Workbench disk. Release 1.3 version 34.20", "Monday 01-Jun-26
10:42:45"), CLI prompt, mouse cursor, disk icons top-right and
bottom-right.

---

## Timeline

### 2026-06-01 — first attempts

![](20260601_104544_verilog_wb13_idle.png)

Our sim at the matched cycle. BPL1 mostly empty — only sparse glyph
fragments visible. Diagnosis: bitplane content not laid out
correctly, no title bar.

![](20260601_110352_verilog_wb13_fresh.png)

Fresh boot, same era. Confirms partial-render isn't a snapshot timing
artifact.

![](20260601_223405_verilog_wb13_postblitfix.png)

After early blitter fidelity fixes (bit-0 mask + USE-B=BLTBDAT
preset). Boot reaches further, rendering still degraded.

### 2026-06-02 — validator era

![](20260602_235012_verilog_wb13_post_validator_probes.png)

Sim with trackdisk-validator probes wired in. Same broken render but
instrumentation captures why K1.3 rejects cyl-53.

### 2026-06-03 — dialog wall

![](20260603_104112_verilog_wb13_system_request_dialog.png)

Boot wall reached: K1.3 raises **"Volume Workbench1.3 has a read/write
error"** AutoRequest dialog. CPU exonerated by cosim — root cause
traced upstream to a stuck `$DFF064 / $DFF060` chipset write.

### 2026-06-03 — BLTAMOD/BLTCMOD longword-split fix breaks the wall

![](20260603_224007_verilog_wb13_post_blt_long_fix.png)

**Boot wall broken.** Chipset adapter now splits `MOVE.L` writes to
`$DFF064` / `$DFF060` into both BLTAMOD+BLTDMOD / BLTCMOD+BLTBMOD
halves (same bug class as the earlier BLTAFWM/BLTALWM fix). Boot runs
from `r=4.4M` (stuck on dialog) → `r=183M+`.

![](20260603_225139_verilog_wb13_workbench_open.png)

Workbench Screen+window structures now present at chip $C0BBB8 with
`Title="Workbench" 640x245`, matching FS-UAE's final idle state.

![](20260603_233333_verilog_wb13_render_fixed.png)

Render tool fixed to use **max-BPU-during-frame** rather than
end-of-frame BPLCON0. K1.3 uses BPLCON0=$A302 (BPU=2 HIRES) for the
visible scanlines and $0302 (BPU=0) for blanking; the renderer was
sampling the wrong one.

![](20260603_234252_verilog_wb13_clean_render.png)

Clean render with the fixed BPU selection. Real Workbench bitplane
content visible for the first time.

![](20260603_234353_compare_fsuae_vs_verilog.png)

Side-by-side: FS-UAE reference vs our sim, same boot snapshot, same
tool.

![](20260603_235712_verilog_wb13_post_click.png)

After `MOUSE_AUTO_CLICK` injection at WB-idle. No visible change —
Workbench Backdrop sits on top of CLI; click alone doesn't
depth-arrange.

### 2026-06-04 — sprite renderer fix → mouse cursor visible

![](20260604_093118_verilog_wb13_cop1_walked.png)

Render driven by walking COP1LC=$0420 (the K1.3 boot Copper list)
instead of the auto-detected secondary list. Correctly resolves
`SPR0PT=$00C80` etc. — sprite still not drawn because of a renderer
bug.

![](20260604_093830_wb13_render_with_sprite.png)

First render after fixing `try_render_sprite` (the bounds check used
`len(chip)` which returned the tuple length 2, not buffer size).

![](20260604_093923_wb13_full_screen_with_cursor.png)

**First time the Workbench mouse arrow appears in any rendered
output.** Cursor at lo-res (127, 43), classic Amiga red arrow.
Stippled top bar still indicates a depth-arrangement issue.

### 2026-06-04 — current state (autodetected Intuition Copper list)

![](20260604_102517_wb13_autodetect_640.png)

Fresh boot through `make wb-screenshot`, renderer auto-detects
Intuition's installed Copper list at $100C8. Shows: title bar pattern
(stippled), Workbench disk icons + RAM Disk icons + trash can up top
right, render gaps below. Mouse cursor not present at this snapshot
point.

![](20260604_102517_wb13_autodetect_320.png)

Same chip-RAM at 320×200 render. The icon area is more visually
present in the smaller render because the bitplane data clips at byte
40 per row instead of 80 — useful for inspection, not a real
"pristine" display.

---

## What's left vs FS-UAE reference

Side-by-side at the same idle point:

| Our sim                                                | FS-UAE reference                                       |
| ------------------------------------------------------ | ------------------------------------------------------ |
| ![](20260604_102517_wb13_autodetect_640.png) | ![](20260604_102517_fsuae_reference_640.png) |

### Structural state: identical to FS-UAE

`tools/dump_intui_windows.py` (extended this session to also walk
slow-RAM titles) finds the same `Window @ $C05E90 Title='AmigaDOS'
Size=640×200 Flags=$00023007 UserPort=$00000000` in both systems.
Workbench Backdrop window at $C0BBB8, icon.library structures, all
present. The CLI Window struct exists and matches FS-UAE byte-for-byte
in the fields that matter.

### Visible gap: Intuition's title-bar/text render never ran

The chip RAM **does not contain** the pixels that should be there:

- Zero runs of 6+ bytes of `$FF` anywhere in our chip RAM. FS-UAE has
  437 runs of 8+ bytes of `$FF` (the CLI title bar background and the
  inside of every solid-text-glyph row).
- BPL1 nonzero coverage: ours 922 bytes (5.8%) vs FS-UAE 1908 bytes
  (11.9%). The ~1000-byte difference is exactly the "Copyright 1987"
  banner block + "Monday 01-Jun-26 10:42:45" date line.
- Top 10 rows are `$2A AA $2A AA …` (50% stipple, Intuition's Screen
  depth-bar fill pattern) instead of FS-UAE's `$FF FF` (the CLI
  window's title bar background rectangle that Intuition's window-open
  routine would have filled).

So Intuition never executed the title-bar background fill / glyph
blit for the CLI window even though the Window struct is fully
initialized. The boot reaches Workbench-idle with all OS data
structures wired correctly, but the visible pixels for the CLI window
were never painted into the screen bitmap. This is the actual next
boot wall.

The icons + RAM DISK area in our top-right matches FS-UAE — that's
Workbench drawing its disk icons (BCPL routine succeeded).
MOUSE_AUTO_CLICK was tried to activate the CLI window; the 8-bit
quadrature counter wraps gracefully so 624 px is reachable, but
clicking a struct whose pixels were never painted doesn't bring
content back — Intuition has no refresh path that re-emits the title
bar glyphs out of thin air.
