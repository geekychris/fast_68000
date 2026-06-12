# intuition_diff — minimal-program differential harness

Boing-disk stalls in `boing!`'s post-OpenWindow polling loop; whether the
root cause is in OpenScreen, OpenWindow, graphics.library, the blitter or
elsewhere upstream is hard to bisect by running boing itself (100M
instructions of OS code between launch and symptom).  This harness
replaces boing's startup-sequence with a 100-instruction assembly stub
that does *just* one thing — call `OpenScreen` and record the result in a
fixed marker block — so we can compare two runs (ours vs. FS-UAE)
byte-for-byte.

## Files

- `screen_open_test.s` — vasm/GNU-as 68k source: AllocMem(marker),
  OpenLibrary("intuition.library"), build NewScreen, OpenScreen, write
  fingerprint to marker block, spin.
- `build.sh STEM` — assemble + link via `/opt/amiga/bin/m68k-amigaos-{as,ld}`
  to a real AmigaOS hunkexe.
- `make_adf.sh STEM` — copy `kickstart/wb13.adf` → `kickstart/<STEM>.adf`,
  inject the test binary and a one-line Startup-Sequence that runs it.
- `diff_dumps.py ours.bin [fsuae.bin]` — locate the marker block in each
  chip-RAM dump and print the field-by-field diff.

## End-to-end run

```sh
# Build + inject + boot on our sim
make screen-open-test

# (Manually) boot the same ADF on FS-UAE through claude_rpc, snapshot
# chip RAM after the marker shows up, save to /tmp/screen_open_fsuae.bin

# Compare
python3 tools/intuition_diff/diff_dumps.py /tmp/screen_open_chip.bin /tmp/screen_open_fsuae.bin
```

## Marker block layout (64 bytes)

| Offset | Size | Field             | Notes                                |
|--------|------|-------------------|--------------------------------------|
| +0     | 4    | sentinel          | `$54455354` = 'TEST'                 |
| +4     | 4    | IntuitionBase     | 0 if OpenLibrary failed              |
| +8     | 4    | Screen pointer    | 0 if OpenScreen failed               |
| +12    | 4    | status            | 0=in-progress, 1=OL fail, 2=Screen fail, 3=OK |
| +16    | 4    | LeftEdge\|TopEdge | from Screen struct                   |
| +20    | 4    | Width\|Height     | from Screen struct                   |
| +24    | 4    | ViewPort first L  | first long of Screen.ViewPort        |
| +28    | 4    | DMACONR\|BPLCON0  | chipset register-file readback       |

The marker block's address is published at chip RAM `$00000C00` (a
guaranteed-free slot in the system-reserved low memory).

If `AllocMem(MEMF_CHIP|MEMF_CLEAR)` fails, the program falls into a
hard-fail path that writes the sentinel + status=255 directly to
`$00000C00..$00000C07` so we still see a breadcrumb.

## Adding new test programs

The harness is intentionally generic: any 100-instruction stub that
follows the same marker-block convention (sentinel at +0, status at +12,
fields after) plugs in.  Likely follow-ups:

- `open_window_test.s` — OpenScreen → OpenWindow → record Window fields
- `refresh_test.s` — OpenScreen → fill rect → snapshot bitplane bytes
- `task_create_test.s` — CreateProc + check resulting Task struct

`build.sh` and `make_adf.sh` both take the stem as `$1`, so a new test is
one `.s` file + a new `make` target.
