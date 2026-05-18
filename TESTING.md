# Testing the 68000 Core

This project has four layers of automated testing.  Every change should
pass at least the first two; cross-validation should be run before any
release or FPGA bring-up.

## 1. Default regression suite — `make test`

Runs every `tests/tNN_*.s` through our Verilator-built `Vm68k_top` and
checks the halt code (the immediate of the final `STOP #imm`).  A halt
code of 0 means PASS; any other value means a specific assertion failed.

```
make test
```

60 tests cover:

- **CPU**: arithmetic, loops, addressing modes, MOVEM patterns, exceptions
  (TRAP, TRAPV, CHK, divide-by-zero), stack frames (LINK/UNLK/JSR/RTS/
  RTR), supervisor mode, SR/CCR manipulation, bit ops, shifts/rotates,
  EXG (D-D, A-A, D-A), PC-relative and PC-indexed addressing.
- **Chipset**: Blitter (copy, line, fill, descending, BZERO, BLTSTAT
  interrupt), Copper (basic + 2-list), Denise (HAM6/HAM8, EHB, DPF,
  DIW, BPLCON1/2, hires, 1bpp + 2bpp sprites, attached pairs, sprite/
  playfield priority), Paula (audio + INTREQ/INTENA priority encoder).
- **CIA**: 8520 timers (one-shot + continuous), ICR read-and-clear,
  port A/B with DDR, canonical `$BFE001`/`$BFD000` aliasing.
- **Memory map**: canonical chipset aliases at `$DFFxxx`, ROM region
  at `$F80000+`, OVL remap (separate build — see below).

### Adding a test

1. Write `tests/tNN_*.s` (the assembler is in `tb/asm68k.py`).
2. End with `stop #0` for success and `stop #$BADx` for assertion
   failures (the runner reports the immediate as the halt code).
3. `make test` picks it up via `wildcard tests/*.s`.

**Important:** every bug fix should land with a test that would have
caught it.  See the user-feedback memory on this in the project notes.

## 2. OVL test — `make test-ovl`

The `OVL_RESET=1` configuration boots with Amiga's reset-time low-memory
remap active (reads at `$0-$7FFFF` come from a small ROM hex file).
That's not the regression-suite default because all the other tests
boot directly at PC=$400 and want the low-RAM reads working.  `test-ovl`
builds a separate `build_ovl/Vm68k_top` with `OVL_RESET=1` plus a tiny
`tests/rom_test.hex`, and runs `tests/t61_ovl.s` which checks that:

- Low reads come from ROM while OVL is active.
- CIA-A PRA bit 0 driven low clears OVL.
- Direct ROM reads at `$F80000+` are unaffected.

```
make test-ovl
```

`make test-all` runs both the default regression and the OVL test.

## 3. Cross-validation against Musashi — `make crosscheck`

[Musashi](https://github.com/kstenerud/Musashi) is a C-based
functional-accurate 68000 emulator used by MAME and many other
projects.  Treating it as the oracle, we run each whitelisted test
through both our core and Musashi and assert the halt codes match
byte-for-byte.

```
make fetch-musashi    # one-time: clones into external/musashi/ (gitignored)
make crosscheck       # builds Musashi + harness, runs every CROSSCHECK_TEST
```

**Result**: 29/29 match.  Our 68000 is bit-identical to Musashi for the
CROSSCHECK_TESTS set, which covers every CPU-only test (chipset/CIA
tests are excluded by design — Musashi has no chipset).

The CROSSCHECK_TESTS list is in the Makefile; add tests as they become
CPU-only / chipset-independent.

**Real bug this caught**: t14_chk_rtr exposed that our exception stack
frame was 8 bytes (PC + 32-bit-padded SR) instead of the canonical 6
bytes (PC + 16-bit SR).  Fixed in m68k_core.v's S_EXC_PUSH_SR /
S_RTE_POP_SR states.  See `git log` for the diff.

## 4. Cross-validation against FX68K — `make crosscheck-fx68k`

[FX68K](https://github.com/ijor/fx68k) is the cycle-accurate
SystemVerilog 68000 by Jorge Cwik used in the MiSTer Amiga core.  We
wrap it in `tb/fx68k_top.sv` (a flat 16 MiB memory + STOP-immediate
detection) and Verilator-build it as a separate `Vfx68k_top`.

```
make fetch-fx68k          # one-time: clones into external/fx68k/ (gitignored)
make crosscheck-fx68k     # builds Vfx68k_top + runs every CROSSCHECK_TEST
```

**Result**: 29/29 match.  Our core is also bit-identical to FX68K for
every CROSSCHECK_TEST — the same set of CPU-only tests we run against
Musashi.

Two harness details that mattered:

1. **DTACK must wait for UDSn/LDSn**, not just ASn.  Real 68000 only
   asserts the data strobes a phi after ASn, and a slave that drops
   DTACKn before the strobes are valid races the CPU's byte-lane
   capture.  Tests that did word/long memory I/O failed until the
   harness gated DTACKn on `(!UDSn || !LDSn)`.
2. **STOP detection has to tolerate prefetch**.  FX68K speculatively
   prefetches one or two words past the current instruction, so a
   `4E72` (STOP opcode) byte in the prefetched stream is not
   necessarily an executing STOP — an earlier `RTR`/`JMP` may
   redirect PC.  The harness now captures every `4E72`+next-word
   pair as a candidate STOP-imm, then commits the most recent
   candidate when the CPU genuinely halts (either `oHALTEDn` drops
   for a valid STOP, or `prog_fetches` stops advancing for >4000
   host cycles, which catches privilege-violation-trapped STOPs
   that fall into an uninitialised vector loop).

## 5. C multicore demos — `make demos-c`

Demos in `demos/c/` are short C programs that exercise the multicore
programming model end-to-end:

- `00_smoke.c` — every core prints "hi core N" and halts.
- `01_parallel_sum.c` — split a 512-element array across N cores,
  per-core partial sums into padded slots, sense-reversing barrier,
  core 0 reduces and asserts the total against `N*(N+1)/2`.
- `02_prodcons.c` — single-producer / single-consumer ring on the
  pattern from MULTICORE.md.  Core 0 produces N items, core 1 consumes
  and verifies each.
- `03_amiga_audio_blt.c` — two cores driving the Amiga chipset in
  parallel: core 0 programs Paula audio channel 0, core 1 runs a
  blitter copy to memory, both check side effects.

Compiled by `m68k-linux-gnu-gcc` from the `fast68k-cc` Docker image
(`tools/cc68k/Dockerfile`), linked against the freestanding runtime in
`runtime/` (crt0 + sync/io primitives), then converted from binary to
the `$readmemh` format with `tools/cc68k/bin2hex.py`.

```
make demos-c           # build the docker image, compile and run every demo
```

Halt-code convention is the same as for the assembly tests: 0 = PASS,
non-zero = FAIL.  The harness asserts halt 0 for every demo.

## 6. Boot-from-ROM end-to-end — `make test-boot-rom`

Assembles `roms/boot_rom.s` into a hex loaded at $F80000 (with the
68000 reset vector at offsets 0/4 inside the ROM) and runs
`tests/t63_boot_rom.s` as a $400-resident trampoline that picks up
the SSP+PC from $0/$4 (OVL routes those reads to ROM) and jumps into
the ROM image.  The ROM code then writes Paula INTENA, clears OVL
via CIA-A, reads memory back, and halts with 0.

Proves the boot chain: OVL → reset vector fetch → trampoline → ROM
entry → chipset MMIO → live RAM after OVL clear.

```
make test-boot-rom
```

### 6b. Extended boot-ROM test — `make test-boot-rom-ext`

Same trampoline mechanism as `test-boot-rom`, but the ROM image walks
a wider slice of the chipset before halting: VHPOSR beam advance,
DMACON SET/CLR via the Agnus block, SERDATR/POTGOR canned values,
CIA-A TOD counter advance, and a 4-word blitter copy with BLTSTAT
poll.  Any failure halts with $BAD1..$BAD6 so the failing step is
identifiable.

```
make test-boot-rom-ext
```

### 6c. Boot from an arbitrary binary ROM — `make test-boot-rom-bin`

Same trampoline + ROM-region build, but loads a user-supplied
binary file (e.g. a Kickstart .rom).  No legal binary is checked in.

```
make test-boot-rom-bin ROMFILE=roms/my_kickstart.rom ROMSIZE_WORDS=65536
```

`tools/bin2rom.py` converts the binary to the bus's $readmemh
format, padded to ROMSIZE_WORDS (= 256 KB for Kickstart 1.x).  Runs
the sim for 10 M cycles and dumps the tail of stdout / [sim]
output — so even when the ROM doesn't reach a clean halt, you can
see how far it got and which chipset registers it last touched.

## 7. Memory-backed block device — `make test-blockdev`

Builds the sim with a tiny disk image (`tests/disk_test.hex`) loaded
into the bus's `disk[]` array (parameter `DISK_HEXFILE`), runs
`tests/t65_blockdev.s` which programs the four block-device control
registers at $00FE_8000 (BLKSRC / BLKDST / BLKCNT / BLKCMD) and
verifies that a single 512-byte sector DMAs to RAM correctly.

The block-device interface stands in for an A590-style hardfile or
SD-card autoconfig device.  CPU programs sector + dest + count, sets
the go bit, polls until BLKCMD reads back 0.  Hex-loadable from any
.adf / hardfile image.

```
make test-blockdev
```

## 8. Chipset register cross-check vs MiniMig — `make crosscheck-minimig`

Clones the MiniMig (FPGA Amiga clone) RTL into `external/minimig/`
and builds a Verilator harness that instantiates *both* MiniMig's
`paula_intcontroller` and our `paula` side-by-side, drives an
identical sequence of INTENA/INTREQ writes into both, and asserts
read-back parity on INTENAR/INTREQR.

```
make fetch-minimig          # one-time clone
make crosscheck-minimig     # build + run
```

Result: PASS with 2 documented soft divergences.

The soft divergences both come from MiniMig keeping bit 14 of
INTREQ as a writable storage cell that ORs into the INTREQR
read-back; the Amiga HRM reserves bit 14 of INTREQ and our paula
masks it off.  These are real differences in chipset semantics —
the cross-check finds them automatically and surfaces them as
expected-divergence lines in the output, not failures.

## 9. All cross-validation — `make crosscheck-all`

Runs both `crosscheck` (Musashi) and `crosscheck-fx68k` back-to-back.

## External references — NOT checked in

`external/musashi/` and `external/fx68k/` are both gitignored.  The
Makefile fetches them on demand via `git clone --depth 1`.  Anyone
checking out the repo can run `make crosscheck-all` and the references
will be fetched automatically the first time.

`build_musashi/` and `build_fx68k/` are also gitignored — they contain
the Verilator/host-compiled binaries for each reference.

## Halt-code convention

Every test ends with `stop #imm` where:

- `imm == 0` → test passed.
- `imm != 0` → test failed; the immediate identifies which assertion
  fired (typically `$BADx`, `$Dxxx`, `$Exxx`).

The host-side wrapper exits with `(imm & 0xFF)` so shell scripts can
test exit code.  Bit 0 of the exit code is OR'd to 1 when `(imm & 0xFF)
== 0 && imm != 0`, so a halt code like `$BB00` doesn't get reported as
"success" by mistake.

## Build matrix

| Target               | Binary                       | Boot configuration                        |
|----------------------|------------------------------|-------------------------------------------|
| `make build`         | `build/Vm68k_top`            | Default: OVL off, no ROM, full chipset    |
| `make test`          | (uses build/)                | All `tests/*.s` except `t61_ovl.s`        |
| `make test-ovl`      | `build_ovl/Vm68k_top`        | OVL_RESET=1, MEM_HEXFILE_ROM=rom_test.hex |
| `make musashi`       | `build_musashi/musashi_sim`  | Musashi reference                         |
| `make fx68k`         | `build_fx68k/Vfx68k_top`     | FX68K reference                           |
| `make demo-*`        | `build_demo/Vm68k_top`       | SDL2-linked, large MEM_WORDS              |
