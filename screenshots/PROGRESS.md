# WB1.3 boot — visual progress log

Each PNG is a `tools/render_k13_screen.py` rendering of a chip-RAM
snapshot captured by the Verilator sim at some point during the
Kickstart 1.3 + Workbench 1.3 boot sequence, walking the live Copper
list to recover the active bitplane configuration. FS-UAE shots come
from the patched FS-UAE save-state pipeline (`tools/fsuae_state.py`).

Filenames are `YYYYMMDD_HHMMSS_<label>.png`.

## Timeline

### 2026-06-01 — reference + first attempts

| Shot | What it shows |
| --- | --- |
| `20260601_104544_fsuae_wb13_idle.png` | **Reference**: FS-UAE at WB1.3 idle. Solid blue desktop, title bar with disk icons. The target rendering. |
| `20260601_104544_verilog_wb13_idle.png` | Our sim at the matched cycle: chip-RAM snapshot rendered to screen. BPL1 mostly empty — only sparse glyph fragments visible. Diagnosis: bitplane content not laid out correctly, no title bar. |
| `20260601_110352_verilog_wb13_fresh.png` | Fresh boot, same era. Confirms the partial-render isn't a snapshot timing artifact. |
| `20260601_223405_verilog_wb13_postblitfix.png` | After early blitter fidelity fixes (bit-0 mask + USE-B=BLTBDAT preset). Boot reaches further but rendering still degraded. |

### 2026-06-02/03 — validator/dialog era

| Shot | What it shows |
| --- | --- |
| `20260602_235012_verilog_wb13_post_validator_probes.png` | Sim with trackdisk-validator probes wired in. Same broken render but underlying instrumentation captures why K1.3 rejects cyl-53. |
| `20260603_104112_verilog_wb13_system_request_dialog.png` | Boot wall reached: K1.3 raises the **"Volume Workbench1.3 has a read/write error"** AutoRequest dialog. CPU exonerated by cosim — root cause traced upstream to a stuck `$DFF064 / $DFF060` write. |

### 2026-06-03 — BLTAMOD/BLTCMOD longword-split fix

| Shot | What it shows |
| --- | --- |
| `20260603_224007_verilog_wb13_post_blt_long_fix.png` | **Boot wall broken.** Chipset adapter now splits `MOVE.L` writes to `$DFF064` / `$DFF060` into both BLTAMOD+BLTDMOD / BLTCMOD+BLTBMOD halves (same bug class as the earlier BLTAFWM/BLTALWM fix). Boot runs from `r=4.4M` (stuck on dialog) → `r=183M+`. |
| `20260603_225139_verilog_wb13_workbench_open.png` | Workbench Screen+window structures now present at chip $C0BBB8 with `Title="Workbench" 640x245`, matching FS-UAE's final idle state. |
| `20260603_233333_verilog_wb13_render_fixed.png` | Render tool fixed to use **max-BPU-during-frame** rather than end-of-frame BPLCON0. K1.3 uses BPLCON0=$A302 (BPU=2 HIRES) for the visible scanlines and $0302 (BPU=0) for blanking; the renderer was sampling the wrong one. |
| `20260603_234252_verilog_wb13_clean_render.png` | Clean render with the fixed BPU selection. Real Workbench bitplane content visible for the first time. |
| `20260603_234353_compare_fsuae_vs_verilog.png` | Side-by-side: FS-UAE reference vs our sim, same boot snapshot, rendered with the same tool. Tops match; lower half still differs (downstream blit ordering). |
| `20260603_235712_verilog_wb13_post_click.png` | After `MOUSE_AUTO_CLICK` injection at WB-idle. No visible change in the render — Workbench Backdrop sits on top of CLI; click alone doesn't depth-arrange. |

### 2026-06-04 — sprite renderer fix → mouse cursor visible

| Shot | What it shows |
| --- | --- |
| `20260604_093118_verilog_wb13_cop1_walked.png` | Render driven by walking COP1LC=$0420 (the real Workbench Copper list) instead of the auto-detected secondary list. Now correctly resolves `SPR0PT=$00C80` etc. — but no sprite still drawn. |
| `20260604_093830_wb13_render_with_sprite.png` | First render after fixing `try_render_sprite`. The `len(chip) > p+4` bounds check returned the tuple length (2) instead of the byte buffer size, killing every sprite row before it was fetched. Replaced with `_addr_to_buf()` lookup. |
| `20260604_093923_wb13_full_screen_with_cursor.png` | **First time the Workbench mouse arrow appears in any rendered output.** Cursor at lo-res (127, 43), classic Amiga red arrow. Title bar / disk icons visible top-right. Lower half of the screen still shows render gaps — next milestone. |

## What's currently visible (and what's not)

`20260604_093923_wb13_full_screen_with_cursor.png` is the latest state.
Visible:

- Workbench-blue backdrop (COLOR00 = $005A).
- Top-row stippled pattern + two disk-icon ghosts on the right edge.
- Red mouse arrow at lo-res (127, 43).

Still missing / corrupt:

- Most of the lower bitplane area is empty blue (no rendered icon
  area or window borders).
- Top-row title bar is stippled rather than solid — possible BPL2
  contribution missing, or DDFSTRT/STOP horizontal alignment off.
- No `Workbench1.3` disk-icon label glyphs.

These remaining gaps map to bitplane-rendering work, not CPU/blitter
correctness — the cosim and chip-RAM-vs-FS-UAE compare both show the
right *bytes* are present, so the next round is about how Denise +
Agnus actually composite them across the frame.
