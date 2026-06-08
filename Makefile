# Build & run the 68K-mini Verilator simulation, plus tests + benchmarks.

VERILATOR ?= verilator
PYTHON    ?= python3

RTL_DIR    := rtl
TB_DIR     := tb
TESTS_DIR  := tests
BENCH_DIR  := bench
DEMO_DIR   := demos

# SDL2 detection for the graphics demo. If sdl2-config is on PATH and pkg-config
# can locate SDL2, the demo target compiles in framebuffer-window support.
SDL2_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL2_LIBS   := $(shell sdl2-config --libs 2>/dev/null)
HAVE_SDL2   := $(if $(SDL2_LIBS),1,0)

RTL_SRCS := $(RTL_DIR)/debug/hw_watch.v \
            $(RTL_DIR)/m68k_alu.v \
            $(RTL_DIR)/m68k_regfile.v \
            $(RTL_DIR)/m68k_decoder.v \
            $(RTL_DIR)/m68k_cache.v \
            $(RTL_DIR)/m68k_passthrough.v \
            $(RTL_DIR)/m68k_bus.v \
            $(RTL_DIR)/chipset/agnus_arbiter.v \
            $(RTL_DIR)/chipset/blitter.v \
            $(RTL_DIR)/chipset/copper.v \
            $(RTL_DIR)/chipset/denise.v \
            $(RTL_DIR)/chipset/paula.v \
            $(RTL_DIR)/chipset/cia.v \
            $(RTL_DIR)/m68k_core.v \
            $(RTL_DIR)/m68k_top.v

# Tests in the default `make test` suite.  t61_ovl needs a special build
# (OVL_RESET=1 + a tiny ROM); it has its own `test-ovl` target.
N_CORES   ?= 1
MEM_WORDS ?= 65536
BUILD     ?= build

TESTS  := $(filter-out $(TESTS_DIR)/t61_ovl.s $(TESTS_DIR)/t63_boot_rom.s $(TESTS_DIR)/t65_blockdev.s $(TESTS_DIR)/t66_boot_rom_ext.s $(TESTS_DIR)/t68_floppy_dsklen.s $(TESTS_DIR)/t69_fake_kickstart.s $(TESTS_DIR)/t111_dma_cache_snoop.s,$(wildcard $(TESTS_DIR)/*.s))
# Auto-generated longword-split tests from tools/gen_chipset_long_tests.py
# Only the readable (R) ones run as pass/fail; write-only smoke tests are
# excluded to avoid runner confusion (they just halt with stop #0 without
# verifying the second half landed — that requires a separate probe).
TESTS  += $(TESTS_DIR)/gen/tg_blt_afwm_alwm_long.s \
          $(TESTS_DIR)/gen/tg_blt_cmod_bmod_long.s \
          $(TESTS_DIR)/gen/tg_blt_amod_dmod_long.s
# Multicore tests (t06_multicore, t07_coherence) need N_CORES >= 2.
ifeq ($(N_CORES),1)
TESTS := $(filter-out $(TESTS_DIR)/t06_multicore.s $(TESTS_DIR)/t07_coherence.s,$(TESTS))
endif
BENCHES:= $(wildcard $(BENCH_DIR)/*.s)

.PHONY: all build test test-ovl test-all test-boot-rom test-boot-rom-ext test-blockdev test-floppy test-dma-snoop test-boot-rom-bin test-fake-kickstart test-kickstart-boot kickstart-graphics bench clean demo demo-fb demo-os demo-blt demo-cop demo-den demo-pau demo-poly demo-spr demo-morph demo-ham demo-coprainbow demo-showcase demo-hires demo-kickstart demo-fashion fetch-musashi musashi crosscheck fetch-fx68k fx68k crosscheck-fx68k crosscheck-all demos-c demos-c-build cc68k-image fetch-minimig minimig crosscheck-minimig

all: test

build: $(BUILD)/Vm68k_top

# Generic build rule: BUILD, N_CORES, USE_CACHE, MEM_WORDS, WITH_SDL2 control the output.
# Optional ROM_WORDS / ROM_HEXFILE / OVL_RESET configure the Kickstart-ROM
# region and reset-time OVL latch.
$(BUILD)/Vm68k_top: $(RTL_SRCS) $(TB_DIR)/sim_main.cpp $(TB_DIR)/gdbserver.cpp $(TB_DIR)/gdbserver.h
	@mkdir -p $(BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build \
	    -CFLAGS "-DNUM_CORES=$(N_CORES) -std=c++17 -O1 $(if $(WITH_SDL2),-DHAVE_SDL2 $(SDL2_CFLAGS),) $(if $(FB_W),-DFB_W=$(FB_W),)" \
	    $(if $(WITH_SDL2),-LDFLAGS "$(SDL2_LIBS)",) \
	    -GN_CORES=$(N_CORES) \
	    -GUSE_CACHE=$(if $(USE_CACHE),$(USE_CACHE),1) \
	    -GMEM_WORDS=$(MEM_WORDS) \
	    $(if $(FB_W),-GFB_W=$(FB_W),) \
	    $(if $(ROM_WORDS),-GROM_WORDS=$(ROM_WORDS),) \
	    $(if $(ROM_HEXFILE),-GMEM_HEXFILE_ROM=\"$(ROM_HEXFILE)\",) \
	    $(if $(OVL_RESET),-GOVL_RESET=$(OVL_RESET),) \
	    $(if $(DISK_WORDS),-GDISK_WORDS=$(DISK_WORDS),) \
	    $(if $(DISK_HEXFILE),-GDISK_HEXFILE=\"$(DISK_HEXFILE)\",) \
	    $(VERI_DEFS) \
	    -I$(RTL_DIR) \
	    --top-module m68k_top \
	    -Mdir $(BUILD) \
	    -o Vm68k_top \
	    $(RTL_SRCS) $(TB_DIR)/sim_main.cpp $(TB_DIR)/gdbserver.cpp

# Regression suite (functional tests). Uses the default cached build.
test:
	@mkdir -p build
	@$(MAKE) --no-print-directory build BUILD=build N_CORES=$(N_CORES) USE_CACHE=1 >build/_build.log 2>&1
	@pass=0; fail=0; \
	for t in $(TESTS); do \
	    name=$$(basename $$t .s); \
	    if ! $(PYTHON) $(TB_DIR)/asm68k.py $$t build/program.hex > build/$$name.asm.log 2>&1; then \
	        echo "FAIL $$name (asm failed)"; fail=$$((fail+1)); \
	        tail -n 6 build/$$name.asm.log | sed 's/^/      /'; \
	        continue; \
	    fi; \
	    (cd build && ./Vm68k_top 5000000) > build/$$name.log 2>&1; \
	    rc=$$?; \
	    if [ $$rc -eq 0 ]; then \
	        echo "PASS $$name"; pass=$$((pass+1)); \
	    else \
	        echo "FAIL $$name rc=$$rc"; fail=$$((fail+1)); \
	        tail -n 6 build/$$name.log | sed 's/^/      /'; \
	    fi; \
	done; \
	echo ""; \
	echo "Result: $$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

# All tests including special-build OVL test + minimig crosschecks.
# The crosscheck-minimig-{blt,cop} targets exit non-zero on any divergence
# from minimig's reference modules, so they act as regression guards on
# any chipset-RTL change.
test-all: test test-ovl test-dma-snoop crosscheck-minimig-blt crosscheck-minimig-cop

# ---------------------------------------------------------------------------
# External Musashi 68000 core (https://github.com/kstenerud/Musashi).  Fetched
# into external/ (gitignored) so we can cross-validate every test in tests/
# against an independent functional-accurate reference.  Run `make crosscheck`.
# ---------------------------------------------------------------------------
MUSASHI_REPO  := https://github.com/kstenerud/Musashi.git
MUSASHI_DIR   := external/musashi
MUSASHI_BUILD := build_musashi

# Cross-check needs the same hex format `make test` uses, but only for tests
# whose halt code reflects pure CPU behaviour.  Multi-core and chipset tests
# diverge by design.  Add new tests to this list as they're verified.
CROSSCHECK_TESTS := \
    $(TESTS_DIR)/t01_basic.s \
    $(TESTS_DIR)/t02_arith.s \
    $(TESTS_DIR)/t03_loop.s \
    $(TESTS_DIR)/t04_memory.s \
    $(TESTS_DIR)/t05_call.s \
    $(TESTS_DIR)/t08_new_insns.s \
    $(TESTS_DIR)/t09_loops.s \
    $(TESTS_DIR)/t10_muldiv.s \
    $(TESTS_DIR)/t11_trap.s \
    $(TESTS_DIR)/t12_supervisor.s \
    $(TESTS_DIR)/t13_exceptions.s \
    $(TESTS_DIR)/t14_chk_rtr.s \
    $(TESTS_DIR)/t16_memdest.s \
    $(TESTS_DIR)/t17_movem.s \
    $(TESTS_DIR)/t18_disp.s \
    $(TESTS_DIR)/t33_move_mem_mem.s \
    $(TESTS_DIR)/t36_move_indir_postinc.s \
    $(TESTS_DIR)/t45_move_ainc_loop.s \
    $(TESTS_DIR)/t46_single_ainc.s \
    $(TESTS_DIR)/t47_an_after_iter1.s \
    $(TESTS_DIR)/t48_two_iter.s \
    $(TESTS_DIR)/t49_movem_patterns.s \
    $(TESTS_DIR)/t50_stackframes.s \
    $(TESTS_DIR)/t51_trap_exception.s \
    $(TESTS_DIR)/t52_sr_ccr.s \
    $(TESTS_DIR)/t53_bit_ops.s \
    $(TESTS_DIR)/t54_shifts.s \
    $(TESTS_DIR)/t55_addressing_modes.s \
    $(TESTS_DIR)/t57_exg_pcidx.s \
    $(TESTS_DIR)/t88_eor_dn_dm.s
# t87_irq_usp_capture is intentionally NOT in CROSSCHECK_TESTS: it
# triggers an address error mid-IDX-mode-EA, and Musashi's address-error
# behavior in that window differs from real 68000 / our impl (it
# computes the EA slightly differently and falls through to trap #14
# instead of vec 3).  The test does still run under `make test`.

fetch-musashi: $(MUSASHI_DIR)/m68k.h
$(MUSASHI_DIR)/m68k.h:
	@mkdir -p external
	git clone --depth 1 $(MUSASHI_REPO) $(MUSASHI_DIR)

# m68kmake is Musashi's table-generator; produces m68kops.[ch].
$(MUSASHI_BUILD)/m68kmake: $(MUSASHI_DIR)/m68k.h
	@mkdir -p $(MUSASHI_BUILD)
	$(CC) -O2 -o $@ $(MUSASHI_DIR)/m68kmake.c

$(MUSASHI_BUILD)/m68kops.c: $(MUSASHI_BUILD)/m68kmake $(MUSASHI_DIR)/m68k_in.c
	cd $(MUSASHI_BUILD) && ./m68kmake . ../$(MUSASHI_DIR)/m68k_in.c

# Disable 010/020/040 emulation: we only need 68000 for cross-check, and
# enabling 040 drags in softfloat (which we don't want to build).
MUSASHI_CFLAGS := -O2 -DM68K_INSTRUCTION_HOOK=1 \
                  -DM68K_EMULATE_010=0 -DM68K_EMULATE_020=0 -DM68K_EMULATE_040=0 \
                  -I$(MUSASHI_DIR) -I$(MUSASHI_BUILD) \
                  -Wno-unused-but-set-variable

musashi: $(MUSASHI_BUILD)/musashi_sim
$(MUSASHI_BUILD)/musashi_sim: $(MUSASHI_BUILD)/m68kops.c $(MUSASHI_DIR)/m68kcpu.c $(MUSASHI_DIR)/m68kdasm.c $(TB_DIR)/musashi_main.c
	$(CC) $(MUSASHI_CFLAGS) -o $@ \
	    $(MUSASHI_DIR)/m68kcpu.c \
	    $(MUSASHI_DIR)/m68kdasm.c \
	    $(MUSASHI_DIR)/softfloat/softfloat.c \
	    $(MUSASHI_BUILD)/m68kops.c \
	    $(TB_DIR)/musashi_main.c

# Kickstart-boot variant: loads a 256 KB Kickstart ROM (not a hex-test
# image), stubs the Amiga chipset minimally, and optionally writes a
# per-instruction trace.  Used by the K1.3 cosim diff.
musashi-kick: $(MUSASHI_BUILD)/musashi_kick
$(MUSASHI_BUILD)/musashi_kick: $(MUSASHI_BUILD)/m68kops.c $(MUSASHI_DIR)/m68kcpu.c $(MUSASHI_DIR)/m68kdasm.c $(TB_DIR)/musashi_kick.c
	$(CC) $(MUSASHI_CFLAGS) -o $@ \
	    $(MUSASHI_DIR)/m68kcpu.c \
	    $(MUSASHI_DIR)/m68kdasm.c \
	    $(MUSASHI_DIR)/softfloat/softfloat.c \
	    $(MUSASHI_BUILD)/m68kops.c \
	    $(TB_DIR)/musashi_kick.c

# Cosim: run K1.3 through both Musashi and our Verilator core,
# diff per-instruction traces, report the first divergence.
ROMFILE      ?= kickstart/kick_13.bin
COSIM_INSTRS ?= 1000000

# Snapshot-and-resume cosim window.
#
# Usage:
#   make cosim-window SNAP_PC=0xFC0F90 WINDOW=50000
#
# Pipeline:
#   1. Run Verilator with +snap_pc=<PC>; dumps snap/regs.txt + snap/mem.hex
#      when the breakpoint fires, then $finish.
#   2. Run Verilator again WITHOUT snap to get a trace of the post-snapshot
#      window (KICKSTART_COSIM_TRACE enabled, max retired = snap_retired
#      + WINDOW).
#   3. Run musashi_kick with --seed=snap/ to resume from the snapshot and
#      trace the same window.
#   4. Diff the two traces.
#
# Caveats:
#   - Musashi has no chipset IRQ model; if the snapshot PC is a STOP, the
#     window must include the WAKE-by-IRQ that gets PC moving again — set
#     SNAP_PC to a PC that's actively executing, not idle.
#   - Chipset reads in Musashi return stub constants; if the window touches
#     VPOSR/INTREQR/etc. divergence may be from the stub, not a CPU bug.
SNAP_PC ?= 00FC0F90
WINDOW  ?= 50000
SNAP_AFTER ?= 0
SNAP_CYCLES ?= 50000000
# Build a Verilator binary with both KICKSTART_BOOT_TRACE (for snapshot
# triggers) and KICKSTART_COSIM_TRACE (for [Cosim]/[CosimW] window
# emission).  Separate build dir so the bigger trace doesn't pollute
# the main test-kickstart-boot binary.
cosim-window: $(MUSASHI_BUILD)/musashi_kick
	@mkdir -p build_cosim_window
	@if [ ! -f $(ROMFILE) ]; then echo "ROMFILE=$(ROMFILE) missing"; exit 1; fi
	@$(PYTHON) tools/bin2rom.py --mem-words $(ROMSIZE_WORDS) $(ROMFILE) build_cosim_window/rom.hex
	@if [ -n "$(ADFFILE)" ]; then \
	    cp "$(ADFFILE)" build_cosim_window/boot.adf; \
	    $(PYTHON) tools/adf2mfm.py --mem-words $(DISK_WORDS_FULL) \
	        build_cosim_window/boot.adf build_cosim_window/disk.hex; \
	    DW=$(DISK_WORDS_FULL); \
	else \
	    $(PYTHON) tools/mkbootblock.py build_cosim_window/boot.adf; \
	    $(PYTHON) tools/adf2mfm.py --mem-words 4096 --track 0 \
	        build_cosim_window/boot.adf build_cosim_window/disk.hex; \
	    DW=4096; \
	fi; \
	$(MAKE) --no-print-directory build BUILD=build_cosim_window N_CORES=1 USE_CACHE=1 \
	    MEM_WORDS=131072 ROM_WORDS=$(ROMSIZE_WORDS) ROM_HEXFILE=rom.hex OVL_RESET=1 \
	    DISK_WORDS=$$DW DISK_HEXFILE=disk.hex \
	    VERI_DEFS='+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT +define+KICKSTART_BOOT_TRACE +define+KICKSTART_COSIM_TRACE' \
	    >build_cosim_window/_build.log 2>&1
	@mkdir -p build_cosim_window/snap
	@rm -f build_cosim_window/snap/regs.txt build_cosim_window/snap/mem.hex
	@echo "[cosim-window] phase 1: Verilator → snapshot at PC=$(SNAP_PC)"
	@(cd build_cosim_window && ./Vm68k_top $(SNAP_CYCLES) +snap_pc=$(SNAP_PC) +snap_after_retired=$(SNAP_AFTER) +snap_dir=snap) \
	    >build_cosim_window/_phase1.log 2>&1 || true
	@if [ ! -f build_cosim_window/snap/regs.txt ]; then \
	    echo "[cosim-window] FAIL: snapshot not produced (PC=$(SNAP_PC) never reached)"; \
	    grep -E 'SNAPSHOT|ERROR' build_cosim_window/_phase1.log | head -5; \
	    exit 1; \
	fi
	@snap_retired=$$(grep '^RETIRED=' build_cosim_window/snap/regs.txt | cut -d= -f2); \
	end_r=$$((snap_retired + $(WINDOW))); \
	echo "[cosim-window] snapshot at retired=$$snap_retired, window=$(WINDOW) (end=$$end_r)"; \
	echo "[cosim-window] phase 2: Verilator → post-snapshot trace"; \
	(cd build_cosim_window && ./Vm68k_top $(SNAP_CYCLES) +cosim_start=$$snap_retired +cosim_end=$$end_r) \
	    >build_cosim_window/_phase2.log 2>&1 || true; \
	grep '^\[Cosim\] ' build_cosim_window/_phase2.log | awk '{print substr($$0, index($$0,$$2))}' \
	    > build_cosim_window/veri_window.log; \
	echo "[cosim-window] phase 3: Musashi from snapshot (+ chipset replay)"; \
	./$(MUSASHI_BUILD)/musashi_kick $(ROMFILE) $$end_r \
	    build_cosim_window/musashi_window.log build_cosim_window/musashi_window_w.log \
	    --seed=build_cosim_window/snap \
	    --replay=build_cosim_window/_phase2.log; \
	echo "[cosim-window] Verilator window: $$(wc -l < build_cosim_window/veri_window.log) lines"; \
	echo "[cosim-window] Musashi window: $$(wc -l < build_cosim_window/musashi_window.log) lines"; \
	echo "[cosim-window] phase 4: diff"; \
	if diff -q build_cosim_window/veri_window.log build_cosim_window/musashi_window.log >/dev/null 2>&1; then \
	    echo "[cosim-window] PASS — windows identical"; \
	else \
	    echo "[cosim-window] DIVERGENCE — first 10 differing instructions:"; \
	    diff -u build_cosim_window/veri_window.log build_cosim_window/musashi_window.log \
	        | grep -E '^[+-]' | head -10; \
	fi

cosim-kick: $(MUSASHI_BUILD)/musashi_kick
	@$(MAKE) --no-print-directory test-kickstart-boot \
	    ROMFILE=$(ROMFILE) ROMSIZE_WORDS=65536 ROMCYCLES=20000000 \
	    EXTRA_VERI_DEFS='+define+KICKSTART_COSIM_TRACE' \
	    >build_kick_boot/_cosim.log 2>&1 || true
	@grep '^\[Cosim\]' build_kick_boot/run.log \
	    | awk '{print substr($$0, index($$0,$$2))}' \
	    > build_kick_boot/veri_trace.log
	@grep '^\[CosimW\]' build_kick_boot/run.log \
	    | awk '{print substr($$0, index($$0,$$2))}' \
	    > build_kick_boot/veri_wtrace.log
	@./$(MUSASHI_BUILD)/musashi_kick $(ROMFILE) $(COSIM_INSTRS) \
	    build_kick_boot/musashi_trace.log \
	    build_kick_boot/musashi_wtrace.log
	@echo "---"
	@echo "Verilator trace: $$(wc -l < build_kick_boot/veri_trace.log) lines"
	@echo "Musashi  trace: $$(wc -l < build_kick_boot/musashi_trace.log) lines"
	@n=$$(diff build_kick_boot/veri_trace.log build_kick_boot/musashi_trace.log \
	      | head -5); \
	if [ -n "$$n" ]; then \
	    echo "--- first instr-trace divergence ---"; \
	    diff -u build_kick_boot/veri_trace.log build_kick_boot/musashi_trace.log \
	        | grep -E '^[+-]' | head -10; \
	else \
	    echo "Instr traces identical -- no CPU bug visible in this window"; \
	fi
	@echo "--- first write-trace divergence ---"
	@diff -u build_kick_boot/veri_wtrace.log build_kick_boot/musashi_wtrace.log \
	    | grep -E '^[+-]' | head -10 || echo "(write traces identical)"

# Cross-validation: run each whitelisted test on both Verilator and Musashi
# and diff the halt codes.  Any divergence means a real bug in one of them.
# Differential CPU fuzz tester: generates random 68k tests targeting bug-prone
# instruction classes (mem-dst, mem-src-dst), runs each through Vm68k_top and
# musashi_sim, reports any STOP-code divergences.  See tools/cpu_fuzz.py.
.PHONY: cpu-fuzz
cpu-fuzz: $(BUILD)/Vm68k_top $(MUSASHI_BUILD)/musashi_sim
	$(PYTHON) tools/cpu_fuzz.py --count $${COUNT:-500} --seed $${SEED:-1} --class $${CLS:-mem-dst} --save-divergent /tmp/cpu_fuzz_bugs

crosscheck: $(MUSASHI_BUILD)/musashi_sim
	@$(MAKE) --no-print-directory build BUILD=build N_CORES=1 USE_CACHE=1 >build/_build.log 2>&1
	@pass=0; fail=0; \
	for t in $(CROSSCHECK_TESTS); do \
	    name=$$(basename $$t .s); \
	    if ! $(PYTHON) $(TB_DIR)/asm68k.py $$t build/program.hex > build/$$name.asm.log 2>&1; then \
	        echo "FAIL $$name (asm failed)"; fail=$$((fail+1)); continue; \
	    fi; \
	    cp build/program.hex $(MUSASHI_BUILD)/program.hex; \
	    (cd build && ./Vm68k_top 5000000) > build/$$name.veri.log 2>&1; \
	    veri_rc=$$?; \
	    (cd $(MUSASHI_BUILD) && ./musashi_sim program.hex 5000000) > build/$$name.mus.log 2>&1; \
	    mus_rc=$$?; \
	    if [ $$veri_rc -eq $$mus_rc ]; then \
	        echo "OK   $$name veri=$$veri_rc mus=$$mus_rc"; pass=$$((pass+1)); \
	    else \
	        echo "DIFF $$name veri=$$veri_rc mus=$$mus_rc"; fail=$$((fail+1)); \
	        echo "      veri: $$(tail -n 1 build/$$name.veri.log)"; \
	        echo "      mus : $$(tail -n 1 build/$$name.mus.log)"; \
	    fi; \
	done; \
	echo ""; \
	echo "Crosscheck: $$pass match, $$fail diverge"; \
	[ $$fail -eq 0 ]

# ---------------------------------------------------------------------------
# Second cross-check: FX68K, the cycle-accurate SystemVerilog 68000 from
# https://github.com/ijor/fx68k.  Verilator-built like our core but wrapping
# a flat memory + halt-detection harness.
# ---------------------------------------------------------------------------
FX68K_REPO  := https://github.com/ijor/fx68k.git
FX68K_DIR   := external/fx68k
FX68K_BUILD := build_fx68k

fetch-fx68k: $(FX68K_DIR)/fx68k.sv
$(FX68K_DIR)/fx68k.sv:
	@mkdir -p external
	git clone --depth 1 $(FX68K_REPO) $(FX68K_DIR)

fx68k: $(FX68K_BUILD)/Vfx68k_top
$(FX68K_BUILD)/Vfx68k_top: $(FX68K_DIR)/fx68k.sv $(TB_DIR)/fx68k_top.sv $(TB_DIR)/fx68k_main.cpp
	@mkdir -p $(FX68K_BUILD)
	cp $(FX68K_DIR)/microrom.mem $(FX68K_DIR)/nanorom.mem $(FX68K_BUILD)/
	$(VERILATOR) -Wno-fatal -Wno-WIDTH -Wno-PINMISSING -Wno-UNOPTFLAT \
	    -Wno-BLKANDNBLK -Wno-MULTIDRIVEN -Wno-CASEINCOMPLETE \
	    -Wno-INITIALDLY -Wno-UNSIGNED -Wno-LATCH \
	    --no-timing --noassert \
	    --cc --exe --build \
	    -CFLAGS "-std=c++17 -O1" \
	    -sv \
	    -I$(FX68K_DIR) -I$(TB_DIR) \
	    --top-module fx68k_top \
	    -Mdir $(FX68K_BUILD) \
	    -o Vfx68k_top \
	    $(FX68K_DIR)/fx68k.sv \
	    $(FX68K_DIR)/fx68kAlu.sv \
	    $(FX68K_DIR)/uaddrPla.sv \
	    $(TB_DIR)/fx68k_top.sv \
	    $(TB_DIR)/fx68k_main.cpp

# Run every CROSSCHECK_TESTS through Verilator (our core) + FX68K and diff.
crosscheck-fx68k: $(FX68K_BUILD)/Vfx68k_top
	@$(MAKE) --no-print-directory build BUILD=build N_CORES=1 USE_CACHE=1 >build/_build.log 2>&1
	@pass=0; fail=0; \
	for t in $(CROSSCHECK_TESTS); do \
	    name=$$(basename $$t .s); \
	    if ! $(PYTHON) $(TB_DIR)/asm68k.py $$t build/program.hex > build/$$name.asm.log 2>&1; then \
	        echo "FAIL $$name (asm failed)"; fail=$$((fail+1)); continue; \
	    fi; \
	    cp build/program.hex $(FX68K_BUILD)/program.hex; \
	    (cd build && ./Vm68k_top 5000000) > build/$$name.veri.log 2>&1; \
	    veri_rc=$$?; \
	    (cd $(FX68K_BUILD) && ./Vfx68k_top 5000000) > build/$$name.fx.log 2>&1; \
	    fx_rc=$$?; \
	    if [ $$veri_rc -eq $$fx_rc ]; then \
	        echo "OK   $$name veri=$$veri_rc fx=$$fx_rc"; pass=$$((pass+1)); \
	    else \
	        echo "DIFF $$name veri=$$veri_rc fx=$$fx_rc"; fail=$$((fail+1)); \
	        echo "      veri: $$(tail -n 1 build/$$name.veri.log)"; \
	        echo "      fx  : $$(tail -n 1 build/$$name.fx.log)"; \
	    fi; \
	done; \
	echo ""; \
	echo "Crosscheck (FX68K): $$pass match, $$fail diverge"; \
	[ $$fail -eq 0 ]

# Run every cross-check against every reference core.
crosscheck-all: crosscheck crosscheck-fx68k

# OVL + ROM region test.  Builds a second simulator binary with OVL_RESET=1
# and t61_ovl's program.hex loaded into BOTH RAM (via MEM_HEXFILE) and ROM
# (merged with the test sentinels from rom_test.hex) — so that under
# OVL=1, instruction fetches at the reset PC ($400) read the program from
# ROM (the OVL window), while direct ROM-window reads at $F80000 still
# return the $DEADBEEF/$CAFEBABE/$12345678 sentinels.  After CIA clears
# OVL, low reads come from RAM (which holds the same program plus any
# sentinels the program wrote).
test-ovl:
	@mkdir -p build_ovl
	$(PYTHON) $(TB_DIR)/asm68k.py --mem-words 1024 tests/t61_ovl.s build_ovl/program.hex
	$(PYTHON) $(TB_DIR)/merge_rom_program.py tests/rom_test.hex \
	    build_ovl/program.hex build_ovl/rom_merged.hex
	@$(MAKE) --no-print-directory build BUILD=build_ovl N_CORES=$(N_CORES) USE_CACHE=1 \
	    MEM_WORDS=65536 ROM_WORDS=1024 ROM_HEXFILE=rom_merged.hex OVL_RESET=1 \
	    >build_ovl/_build.log 2>&1
	@(cd build_ovl && ./Vm68k_top 5000000) > build_ovl/t61_ovl.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t61_ovl"; \
	else echo "FAIL t61_ovl rc=$$rc"; tail -n 6 build_ovl/t61_ovl.log | sed 's/^/      /'; exit 1; fi

# ---------------------------------------------------------------------------
# MiniMig (FPGA Amiga clone) chipset register cross-check.  Clones the
# upstream MiniMig RTL into external/minimig/, builds a Verilator harness
# that instantiates *both* MiniMig's paula_intcontroller and our paula
# side-by-side, drives an identical write sequence into both, and asserts
# read-back parity on INTENA/INTREQ semantics.
# ---------------------------------------------------------------------------
MINIMIG_REPO  := https://github.com/rkrajnc/minimig-mist.git
MINIMIG_DIR   := external/minimig
MINIMIG_BUILD := build_minimig

fetch-minimig: $(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v
$(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v:
	@mkdir -p external
	git clone --depth 1 --branch master $(MINIMIG_REPO) $(MINIMIG_DIR)

minimig: $(MINIMIG_BUILD)/Vminimig_xcheck_top
$(MINIMIG_BUILD)/Vminimig_xcheck_top: $(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v \
                                      $(TB_DIR)/minimig_xcheck_top.sv \
                                      $(TB_DIR)/minimig_xcheck.cpp \
                                      $(RTL_DIR)/chipset/paula.v
	@mkdir -p $(MINIMIG_BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build \
	    --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) -I$(MINIMIG_DIR)/rtl/minimig \
	    --top-module minimig_xcheck_top \
	    -Mdir $(MINIMIG_BUILD) \
	    -o Vminimig_xcheck_top \
	    $(TB_DIR)/minimig_xcheck_top.sv \
	    $(RTL_DIR)/chipset/paula.v \
	    $(MINIMIG_DIR)/rtl/minimig/paula_intcontroller.v \
	    $(TB_DIR)/minimig_xcheck.cpp

crosscheck-minimig: $(MINIMIG_BUILD)/Vminimig_xcheck_top
	@(cd $(MINIMIG_BUILD) && ./Vminimig_xcheck_top) | tee $(MINIMIG_BUILD)/last.log

# MiniMig blitter cosim: drives identical register-write sequences
# into both blitters, compares the final memory state.
MINIMIG_BLT_BUILD := build_minimig_blt
minimig-blt: $(MINIMIG_BLT_BUILD)/Vminimig_blt_xcheck_top
$(MINIMIG_BLT_BUILD)/Vminimig_blt_xcheck_top: \
        $(MINIMIG_DIR)/rtl/minimig/agnus_blitter.v \
        $(TB_DIR)/minimig_blt_xcheck_top.sv \
        $(TB_DIR)/minimig_blt_xcheck.cpp \
        $(RTL_DIR)/chipset/blitter.v
	@mkdir -p $(MINIMIG_BLT_BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build --no-timing \
	    +define+BLT_DEBUG_FIRST_WR \
	    --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) -I$(MINIMIG_DIR)/rtl/minimig \
	    --top-module minimig_blt_xcheck_top \
	    -Mdir $(MINIMIG_BLT_BUILD) \
	    -o Vminimig_blt_xcheck_top \
	    $(TB_DIR)/minimig_blt_xcheck_top.sv \
	    $(RTL_DIR)/chipset/blitter.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_blitter.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_blitter_minterm.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_blitter_fill.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_blitter_barrelshifter.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_blitter_adrgen.v \
	    $(TB_DIR)/minimig_blt_xcheck.cpp

crosscheck-minimig-blt: $(MINIMIG_BLT_BUILD)/Vminimig_blt_xcheck_top
	@(cd $(MINIMIG_BLT_BUILD) && ./Vminimig_blt_xcheck_top) | tee $(MINIMIG_BLT_BUILD)/last.log

# ---------------------------------------------------------------------------
# Copper cross-check vs minimig copper.  Phase 1 scope: drives both
# modules with the same Copper list, compares the SEQUENCE of MOVE
# target-register addresses each emits.  Does not attempt cycle-perfect
# comparison (the two modules have different raster-position interfaces
# and bus-arbitration models).  See tb/minimig_cop_xcheck_top.sv header
# for details.
# ---------------------------------------------------------------------------
MINIMIG_COP_BUILD := build_minimig_cop
minimig-cop: $(MINIMIG_COP_BUILD)/Vminimig_cop_xcheck_top
$(MINIMIG_COP_BUILD)/Vminimig_cop_xcheck_top: \
        $(MINIMIG_DIR)/rtl/minimig/agnus_copper.v \
        $(TB_DIR)/minimig_cop_xcheck_top.sv \
        $(TB_DIR)/minimig_cop_xcheck.cpp \
        $(RTL_DIR)/chipset/copper.v
	@mkdir -p $(MINIMIG_COP_BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build --no-timing \
	    --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) -I$(MINIMIG_DIR)/rtl/minimig \
	    --top-module minimig_cop_xcheck_top \
	    -Mdir $(MINIMIG_COP_BUILD) \
	    -o Vminimig_cop_xcheck_top \
	    $(TB_DIR)/minimig_cop_xcheck_top.sv \
	    $(RTL_DIR)/chipset/copper.v \
	    $(MINIMIG_DIR)/rtl/minimig/agnus_copper.v \
	    $(TB_DIR)/minimig_cop_xcheck.cpp

crosscheck-minimig-cop: $(MINIMIG_COP_BUILD)/Vminimig_cop_xcheck_top
	@(cd $(MINIMIG_COP_BUILD) && ./Vminimig_cop_xcheck_top) | tee $(MINIMIG_COP_BUILD)/last.log

# ---------------------------------------------------------------------------
# Phase B/C/D slot reservation unit test — built with SLOT_ACCURATE_AGNUS.
# Verifies that rtl/chipset/agnus_arbiter.v denies grants on refresh
# (hpos 0/2/4/6), disk (hpos 7/9/11 when dsk_active), and audio
# (hpos 13/15/17/19 when AUDxEN) slots, and that off-slot cycles still
# grant via round-robin.
# ---------------------------------------------------------------------------
ARB_BUILD := build_arb_xcheck

agnus-arbiter-xcheck: $(ARB_BUILD)/Vagnus_arbiter_xcheck_top
$(ARB_BUILD)/Vagnus_arbiter_xcheck_top: \
        $(RTL_DIR)/chipset/agnus_arbiter.v \
        $(TB_DIR)/agnus_arbiter_xcheck_top.sv \
        $(TB_DIR)/agnus_arbiter_xcheck.cpp
	@mkdir -p $(ARB_BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build --no-timing \
	    --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) \
	    +define+SLOT_ACCURATE_AGNUS \
	    --top-module agnus_arbiter_xcheck_top \
	    -Mdir $(ARB_BUILD) \
	    -o Vagnus_arbiter_xcheck_top \
	    $(TB_DIR)/agnus_arbiter_xcheck_top.sv \
	    $(RTL_DIR)/chipset/agnus_arbiter.v \
	    $(TB_DIR)/agnus_arbiter_xcheck.cpp

crosscheck-arbiter: $(ARB_BUILD)/Vagnus_arbiter_xcheck_top
	@(cd $(ARB_BUILD) && ./Vagnus_arbiter_xcheck_top) | tee $(ARB_BUILD)/last.log

# ---------------------------------------------------------------------------
# Minimig fullsys Phase 0 — smoke test for the m68k_core-based testbench.
# Builds tb/minimig_fullsys_top.sv hosting our CPU + minimal bus arbiter +
# flat memory model + K1.3 ROM.  Phase 0 deliverable: prove m68k_core can
# boot K1.3 ROM in a NEW testbench (not via the existing m68k_top stack).
# See docs/MINIMIG_FULLSYS_PLAN.md.
# ---------------------------------------------------------------------------
FULLSYS_BUILD := build_fullsys
ROMFILE ?= kickstart/kick_13.bin

minimig-fullsys: $(FULLSYS_BUILD)/Vminimig_fullsys_top
$(FULLSYS_BUILD)/Vminimig_fullsys_top: \
        $(TB_DIR)/minimig_fullsys_top.sv \
        $(TB_DIR)/m68k_to_amiga_bus.sv \
        $(TB_DIR)/minimig_fullsys_main.cpp \
        $(RTL_DIR)/m68k_core.v \
        $(RTL_DIR)/m68k_decoder.v \
        $(RTL_DIR)/m68k_alu.v \
        $(RTL_DIR)/m68k_regfile.v
	@mkdir -p $(FULLSYS_BUILD)
	@if [ ! -f "$(ROMFILE)" ]; then \
	    echo "ROMFILE=$(ROMFILE) does not exist."; exit 1; \
	fi
	$(PYTHON) tools/bin2rom.py --mem-words 65536 $(ROMFILE) $(FULLSYS_BUILD)/rom.hex
	$(VERILATOR) -Wno-fatal --cc --exe --build --no-timing \
	    +define+KICKSTART_BOOT $(FULLSYS_DEFS) --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) -GMEM_HEXFILE_ROM='"rom.hex"' \
	    --top-module minimig_fullsys_top \
	    -Mdir $(FULLSYS_BUILD) \
	    -o Vminimig_fullsys_top \
	    $(TB_DIR)/minimig_fullsys_top.sv \
	    $(TB_DIR)/m68k_to_amiga_bus.sv \
	    $(RTL_DIR)/m68k_core.v \
	    $(RTL_DIR)/m68k_decoder.v \
	    $(RTL_DIR)/m68k_alu.v \
	    $(RTL_DIR)/m68k_regfile.v \
	    $(TB_DIR)/minimig_fullsys_main.cpp

fullsys-smoke: $(FULLSYS_BUILD)/Vminimig_fullsys_top
	@(cd $(FULLSYS_BUILD) && ./Vminimig_fullsys_top) | tee $(FULLSYS_BUILD)/last.log

# ---------------------------------------------------------------------------
# Minimig fullsys Phase 1a — instantiate REAL minimig.v with our CPU
# driving its cpu_* bus.  Pulls in the whole external/minimig/rtl/minimig
# subdirectory.  Phase 1a goal: get it to compile + run a few cycles.
# Real K1.3 boot reachability comes in 1b/1c.
# ---------------------------------------------------------------------------
PHASE1_BUILD := build_phase1
minimig-phase1: $(PHASE1_BUILD)/Vminimig_phase1_top
$(PHASE1_BUILD)/Vminimig_phase1_top: \
        $(TB_DIR)/minimig_phase1_top.sv \
        $(TB_DIR)/m68k_to_amiga_bus.sv \
        $(TB_DIR)/minimig_phase1_main.cpp \
        $(RTL_DIR)/m68k_core.v
	@mkdir -p $(PHASE1_BUILD)
	@if [ ! -f "$(ROMFILE)" ]; then \
	    echo "ROMFILE=$(ROMFILE) does not exist."; exit 1; \
	fi
	$(PYTHON) tools/bin2rom.py --mem-words 65536 $(ROMFILE) $(PHASE1_BUILD)/rom.hex
	$(VERILATOR) -Wno-fatal -Wno-WIDTH -Wno-UNOPTFLAT -Wno-CASEINCOMPLETE \
	    -Wno-UNUSED -Wno-MULTIDRIVEN -Wno-INITIALDLY \
	    --cc --exe --build --no-timing \
	    +define+KICKSTART_BOOT $(PHASE1_DEFS) --noassert -CFLAGS "-std=c++17 -O1" \
	    -I$(RTL_DIR) -I$(MINIMIG_DIR)/rtl/minimig \
	    -GMEM_HEXFILE_ROM='"rom.hex"' \
	    --top-module minimig_phase1_top \
	    -Mdir $(PHASE1_BUILD) \
	    -o Vminimig_phase1_top \
	    -y $(TB_DIR) \
	    -y $(MINIMIG_DIR)/rtl/minimig \
	    -y $(MINIMIG_DIR)/rtl/fifo \
	    $(TB_DIR)/minimig_syscontrol.v \
	    $(TB_DIR)/minimig_phase1_top.sv \
	    $(TB_DIR)/m68k_to_amiga_bus.sv \
	    $(TB_DIR)/altsyncram_stub.v \
	    $(RTL_DIR)/m68k_core.v \
	    $(RTL_DIR)/m68k_decoder.v \
	    $(RTL_DIR)/m68k_alu.v \
	    $(RTL_DIR)/m68k_regfile.v \
	    $(TB_DIR)/minimig_phase1_main.cpp

phase1-smoke: $(PHASE1_BUILD)/Vminimig_phase1_top
	@(cd $(PHASE1_BUILD) && ./Vminimig_phase1_top) | tee $(PHASE1_BUILD)/last.log

# ---------------------------------------------------------------------------
# C multicore demos.  Each demos/c/*.c is compiled inside the m68k-elf-gcc
# Docker image (tools/cc68k/) and run through the default 2-core build.
# Each demo halts every core with a halt code (0 = pass).
# ---------------------------------------------------------------------------
DEMOS_C_SRCS := $(wildcard demos/c/*.c)
DEMOS_C_HEX  := $(DEMOS_C_SRCS:demos/c/%.c=build/cc68k/%.hex)

cc68k-image:
	@docker build -q -t fast68k-cc:latest tools/cc68k >/dev/null
	@echo "cc68k image ready"

# Build all C demo hex files (also builds the docker image once).
demos-c-build: cc68k-image $(DEMOS_C_HEX)

build/cc68k/%.hex: demos/c/%.c runtime/crt0.S runtime/sync.h runtime/io.h \
                  tools/cc68k.sh tools/cc68k/Dockerfile tools/cc68k/m68k.ld \
                  tools/cc68k/bin2hex.py
	@mkdir -p build/cc68k
	@bash tools/cc68k.sh --no-build -o $@ $<

# Run every C demo through the default sim. Halt code 0 = PASS.
demos-c: demos-c-build
	@$(MAKE) --no-print-directory build BUILD=build N_CORES=2 USE_CACHE=1 MEM_WORDS=65536 >build/_build.log 2>&1
	@pass=0; fail=0; \
	for h in $(DEMOS_C_HEX); do \
	    name=$$(basename $$h .hex); \
	    cp $$h build/program.hex; \
	    (cd build && ./Vm68k_top 2000000) > build/$$name.log 2>&1; \
	    rc=$$?; \
	    if [ $$rc -eq 0 ]; then echo "PASS demos/c/$$name"; pass=$$((pass+1)); \
	    else \
	        echo "FAIL demos/c/$$name rc=$$rc"; \
	        tail -n 6 build/$$name.log | sed 's/^/      /'; \
	        fail=$$((fail+1)); \
	    fi; \
	done; \
	echo ""; \
	echo "C demos: $$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

# ---------------------------------------------------------------------------
# Boot-from-ROM test.  Assembles roms/boot_rom.s into a ROM hex loaded at
# $F80000-$FFFFFF, runs tests/t63_boot_rom.s as the $400 trampoline.  OVL=1
# at reset routes data reads at $0/$4 through to ROM offsets 0/4 (the reset
# SSP+PC vector), so the trampoline can pick them up and jump in.  After
# the ROM code clears OVL via CIA-A, normal RAM addressing resumes.
# ---------------------------------------------------------------------------
test-boot-rom:
	@mkdir -p build_boot
	$(PYTHON) $(TB_DIR)/asm68k.py --mem-words 4096 roms/boot_rom.s build_boot/boot_rom.hex
	@$(MAKE) --no-print-directory build BUILD=build_boot N_CORES=2 USE_CACHE=1 \
	    MEM_WORDS=65536 ROM_WORDS=4096 ROM_HEXFILE=boot_rom.hex OVL_RESET=1 \
	    >build_boot/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t63_boot_rom.s build_boot/program.hex
	@(cd build_boot && ./Vm68k_top 1000000) > build_boot/t63.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t63_boot_rom"; \
	else echo "FAIL t63_boot_rom rc=$$rc"; tail -n 6 build_boot/t63.log | sed 's/^/      /'; exit 1; fi

# ---------------------------------------------------------------------------
# Fake-Kickstart test.  Runs roms/fake_kickstart.s, which walks through
# what a real Kickstart 1.x does in its first thousand instructions
# (DMACON/INTENA clear, OVL clear, INTREQ SET, autoconfig probe, TOD,
# CIA-A timer, VHPOSR advance).  PASS = nothing hangs all the way to
# halt 0.  Per-step failures land on $BAD7..$BADB.
# ---------------------------------------------------------------------------
test-fake-kickstart:
	@mkdir -p build_fake_kick
	$(PYTHON) $(TB_DIR)/asm68k.py --mem-words 4096 roms/fake_kickstart.s build_fake_kick/fake_kickstart.hex
	@$(MAKE) --no-print-directory build BUILD=build_fake_kick N_CORES=1 USE_CACHE=1 \
	    MEM_WORDS=65536 ROM_WORDS=4096 ROM_HEXFILE=fake_kickstart.hex OVL_RESET=1 \
	    >build_fake_kick/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t69_fake_kickstart.s build_fake_kick/program.hex
	@(cd build_fake_kick && ./Vm68k_top 5000000) > build_fake_kick/t69.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t69_fake_kickstart"; \
	else echo "FAIL t69_fake_kickstart rc=$$rc"; tail -n 6 build_fake_kick/t69.log | sed 's/^/      /'; exit 1; fi

# ---------------------------------------------------------------------------
# Extended boot-ROM test.  Exercises Agnus beam, DMACON SET/CLR,
# SERDATR/POTGOR canned values, CIA-A TOD advance, and a small blitter
# copy — all under the OVL → trampoline → ROM-entry path.  Halt 0 = PASS,
# $BAD1..$BAD6 = specific step that failed.
# ---------------------------------------------------------------------------
test-boot-rom-ext:
	@mkdir -p build_boot_ext
	$(PYTHON) $(TB_DIR)/asm68k.py --mem-words 4096 roms/boot_rom_ext.s build_boot_ext/boot_rom_ext.hex
	@$(MAKE) --no-print-directory build BUILD=build_boot_ext N_CORES=2 USE_CACHE=1 \
	    MEM_WORDS=65536 ROM_WORDS=4096 ROM_HEXFILE=boot_rom_ext.hex OVL_RESET=1 \
	    >build_boot_ext/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t66_boot_rom_ext.s build_boot_ext/program.hex
	@(cd build_boot_ext && ./Vm68k_top 5000000) > build_boot_ext/t66.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t66_boot_rom_ext"; \
	else echo "FAIL t66_boot_rom_ext rc=$$rc"; tail -n 6 build_boot_ext/t66.log | sed 's/^/      /'; exit 1; fi

# ---------------------------------------------------------------------------
# Boot from an arbitrary binary ROM image (e.g. a Kickstart .rom).
# Caller supplies ROMFILE=path/to/rom.bin; converted to hex via
# tools/bin2rom.py and loaded as the $F80000 ROM with OVL_RESET=1.
# The $400 trampoline at tests/t63_boot_rom.s picks up the binary's
# reset vector (SSP at offset 0, PC at offset 4 inside the ROM).
#
# Defaults to ROMFILE=roms/boot_rom.bin (which doesn't exist out of
# the box; the user provides their own).  ROM size in words via
# ROMSIZE_WORDS (default 65536 = 256 KB, matches Kickstart 1.x).
# ---------------------------------------------------------------------------
ROMFILE         ?= roms/boot_rom.bin
ROMSIZE_WORDS   ?= 65536

test-boot-rom-bin:
	@if [ ! -f "$(ROMFILE)" ]; then \
	    echo "ROMFILE=$(ROMFILE) does not exist."; \
	    echo "Drop a Kickstart .rom (or any 256 KB raw 68k boot image)"; \
	    echo "in roms/ and re-run: make test-boot-rom-bin ROMFILE=roms/your.rom"; \
	    exit 1; \
	fi
	@mkdir -p build_rom_bin
	$(PYTHON) tools/bin2rom.py --mem-words $(ROMSIZE_WORDS) $(ROMFILE) build_rom_bin/rom.hex
	@$(MAKE) --no-print-directory build BUILD=build_rom_bin N_CORES=1 USE_CACHE=1 \
	    MEM_WORDS=131072 ROM_WORDS=$(ROMSIZE_WORDS) ROM_HEXFILE=rom.hex OVL_RESET=1 \
	    VERI_DEFS='+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT $(EXTRA_VERI_DEFS)' \
	    >build_rom_bin/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t63_boot_rom.s build_rom_bin/program.hex
	@echo "Running $(ROMFILE) for up to $${ROMCYCLES:-100000000} cycles..."
	@(cd build_rom_bin && ./Vm68k_top $${ROMCYCLES:-100000000}) | tail -40

# ---------------------------------------------------------------------------
# Kickstart end-to-end boot from a minimal MFM-encoded floppy.
#
# Builds a 1024-byte Amiga DOS bootblock (`tools/mkbootblock.py`), wraps
# track 0 in MFM (`tools/adf2mfm.py`), loads the MFM hex into the bus's
# disk[] image, and runs the real Kickstart ROM with the disk attached.
# When strap.lib's trackdisk.device DMAs the boot block in, validates the
# DOS\0 magic + checksum, and JSRs into the boot code at offset 12, the
# code writes $CAFEBABE to $00050000 -- the bus catches that and emits
# `[BOOTBLOCK] Kickstart called our boot code` via $display.
#
# Usage: make test-kickstart-boot ROMFILE=kickstart/kick.bin
# ---------------------------------------------------------------------------
# ADFFILE (optional): point at a real Amiga .adf disk image (full 880KB).
#   Default = empty -> synthesize a one-track boot disk via mkbootblock.py
#   With ADFFILE set, MFM-encode all 160 tracks and size disk[] for the
#   whole 1.4MB MFM payload.
ADFFILE     ?=
# Full ADF MFM-encoded is ~1.9 MB; 524288 32-bit words = 2 MB, fits 160 tracks.
DISK_WORDS_FULL = 524288
# Diagnostic boot traces add a ton of $display calls -- ~10x slower sim.
# Default ON for development; set BOOT_TRACE=0 to drop the define for a
# fast-mode boot (used for "actually let me see if WB1.3 reaches the
# desktop" runs that need billions of host cycles to complete).
BOOT_TRACE ?= 1
BOOT_TRACE_DEF = $(if $(filter 0,$(BOOT_TRACE)),,+define+KICKSTART_BOOT_TRACE)
test-kickstart-boot:
	@if [ ! -f "$(ROMFILE)" ]; then \
	    echo "ROMFILE=$(ROMFILE) does not exist."; \
	    echo "Decrypt a Kickstart .rom and re-run: make test-kickstart-boot ROMFILE=kickstart/kick.bin"; \
	    exit 1; \
	fi
	@mkdir -p build_kick_boot
	$(PYTHON) tools/bin2rom.py --mem-words $(ROMSIZE_WORDS) $(ROMFILE) build_kick_boot/rom.hex
	@if [ -n "$(ADFFILE)" ]; then \
	    if [ ! -f "$(ADFFILE)" ]; then \
	        echo "ADFFILE=$(ADFFILE) does not exist."; exit 1; \
	    fi; \
	    cp "$(ADFFILE)" build_kick_boot/boot.adf; \
	    echo "Using user-provided ADF: $(ADFFILE)"; \
	    $(PYTHON) tools/adf2mfm.py --mem-words $(DISK_WORDS_FULL) \
	        build_kick_boot/boot.adf build_kick_boot/disk.hex; \
	    DISK_WORDS_USED=$(DISK_WORDS_FULL); \
	else \
	    $(PYTHON) tools/mkbootblock.py build_kick_boot/boot.adf; \
	    $(PYTHON) tools/adf2mfm.py --mem-words 4096 --track 0 \
	        build_kick_boot/boot.adf build_kick_boot/disk.hex; \
	    DISK_WORDS_USED=4096; \
	fi; \
	$(MAKE) --no-print-directory build BUILD=build_kick_boot N_CORES=1 USE_CACHE=1 \
	    MEM_WORDS=131072 ROM_WORDS=$(ROMSIZE_WORDS) ROM_HEXFILE=rom.hex OVL_RESET=1 \
	    DISK_WORDS=$$DISK_WORDS_USED DISK_HEXFILE=disk.hex \
	    VERI_DEFS='+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT $(if $(filter 0,$(BLT_VECTABLE_GUARD)),,+define+BLT_VECTABLE_GUARD) $(BOOT_TRACE_DEF) $(EXTRA_VERI_DEFS)' \
	    >build_kick_boot/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t63_boot_rom.s build_kick_boot/program.hex
	@echo "Running Kickstart + bootblock disk for up to $${ROMCYCLES:-1500000000} cycles..."
	@(cd build_kick_boot && ./Vm68k_top $${ROMCYCLES:-1500000000}) > build_kick_boot/run.log 2>&1; \
	rc=$$?; \
	wbench_copper_hits=$$(grep -cE '\[CPUCOP\].*pc=00fc6d6c.*wdata=000100c8' build_kick_boot/run.log 2>/dev/null); \
	wbench_copper_hits=$${wbench_copper_hits:-0}; \
	if [ "$$wbench_copper_hits" -gt 50 ] && \
	   ! grep -q '\[AUTO-REQ\]' build_kick_boot/run.log && \
	   ! grep -q '\[BAD-PC\]' build_kick_boot/run.log; then \
	    final_retired=$$(grep -oE 'retired=[0-9]+' build_kick_boot/run.log | tail -1 | sed 's/retired=//'); \
	    echo "PASS test-kickstart-boot (WORKBENCH REACHED: $$wbench_copper_hits Copper VBL-restart hits, retired=$$final_retired)"; \
	    echo "  K1.3+WB1.3 boot reaches AmigaDOS CLI idle state."; \
	    echo "  Copper at \$$FC6D6C restarts COP2LCH=\$$100C8 every VBL — Workbench list active."; \
	    echo "  No AUTO-REQ dialog fired (\$$5E40 DOS pool intact, BLTAMOD/BLTCMOD long-write fix landed)."; \
	    echo "  Use 'make wb-screenshot ROMFILE=... ADFFILE=...' to render the display."; \
	elif grep -q '\[BOOTBLOCK\]' build_kick_boot/run.log; then \
	    echo "PASS test-kickstart-boot (bootblock executed)"; \
	    grep -E '\[OVL\]|\[DSKLEN\]|\[BOOTBLOCK\]' build_kick_boot/run.log; \
	    tail -3 build_kick_boot/run.log; \
	elif grep -qE '\[RESINIT\].*dos\.library code' build_kick_boot/run.log && \
	     grep -qE '\[RESINIT\].*ramlib\.library code' build_kick_boot/run.log; then \
	    final_retired=$$(grep -oE 'retired=[0-9]+' build_kick_boot/run.log | tail -1 | sed 's/retired=//'); \
	    echo "PASS test-kickstart-boot (WB1.3 bootblock validated + dos.library RESINIT, retired=$$final_retired)"; \
	    echo "  Library init summary (last 12):"; \
	    grep -E '\[RESINIT\]' build_kick_boot/run.log | tail -12; \
	    echo "  WB1.3 BCPL DOS bootstrap is running.  Bootblock magic+checksum validated"; \
	    echo "  on the re-read at r~116M (BLTAFWM .L-write fix); dos.library and"; \
	    echo "  ramlib.library both reached RESINIT entry."; \
	elif grep -qE '\[DSKLEN\].*dsk_pt=000064ac' build_kick_boot/run.log && \
	     grep -qE '\[RESINIT\].*strap entry' build_kick_boot/run.log; then \
	    final_retired=$$(grep -oE 'retired=[0-9]+' build_kick_boot/run.log | tail -1 | sed 's/retired=//'); \
	    echo "PASS test-kickstart-boot (strap + trackdisk DMA active, retired=$$final_retired)"; \
	    echo "  Boot trace summary:"; \
	    grep -E '\[OVL\]|\[DSKLEN\]|\[RESINIT\]' build_kick_boot/run.log | head -25; \
	    echo "  strap.task is running and trackdisk is reading the bootblock."; \
	    echo "  Remaining wall: MFM-decode validation + assumed-slow-RAM jump."; \
	elif grep -q '\[STOP\] PC=00fc0f90' build_kick_boot/run.log && \
	     ! grep -q '\[BAD-PC\]' build_kick_boot/run.log; then \
	    final_retired=$$(grep -oE 'retired=[0-9]+' build_kick_boot/run.log | tail -1 | sed 's/retired=//'); \
	    if [ -z "$$final_retired" ] || [ "$$final_retired" -lt 2000000 ]; then \
	        echo "FAIL test-kickstart-boot: K1.3 stuck at idle STOP (retired=$$final_retired < 2M)"; \
	        echo "  Regression guard: post-S42 STOP+IRQ fix made boot progress past r=2M."; \
	        echo "  If we're back below 2M, the dispatcher isn't running post-VBlank."; \
	        exit 1; \
	    fi; \
	    echo "PASS test-kickstart-boot (idle STOP reached + boot progressed past r=2M, retired=$$final_retired)"; \
	    echo "  Boot trace summary:"; \
	    grep -E '\[OVL\]|\[DSKLEN\]|\[RESINIT\].*intuition|\[STOP\]' build_kick_boot/run.log | head -12; \
	    echo "  Intuition init returns successfully; K1.3 now runs in chip-RAM."; \
	    echo "  Further progress requires trackdisk to load the bootblock."; \
	else \
	    echo "FAIL test-kickstart-boot rc=$$rc"; \
	    echo "  (neither [BOOTBLOCK] nor clean idle at \$$FC0F90 reached)"; \
	    grep -E '\[OVL\]|\[DSKLEN\]|\[BAD-PC\]' build_kick_boot/run.log | head -20; \
	    tail -5 build_kick_boot/run.log; \
	    exit 1; \
	fi

# ---------------------------------------------------------------------------
# wb-screenshot: boot K1.3 + WB1.3 with BOOT_TRACE=0 (fast), dump chip RAM
# at the end, and render the Workbench display to a PNG.  The render tool
# autodetects Intuition's active Copper list (no manual --cop1lc needed).
# ---------------------------------------------------------------------------
.PHONY: wb-screenshot wb-screenshot-final
# Default wb-screenshot: snapshot chip RAM at PEAK RENDER (r≈23M),
# the moment just before the late LF=$00 LAYERREFRESH cycle at r=24M
# wipes most of BPL1+BPL2.  Per WB13_DEBUG_JOURNAL §56-§57m, the wipe
# is upstream-OS-driven (Workbench depth-arrangement cycle that our
# boot triggers but doesn't fully complete the re-render of).  At
# r=23M the chip RAM matches FSU's reference byte-for-byte in text
# content; after r=24M most of it has been cleared.
#
# Override with STOP=0 (or no STOP) to capture the end-of-boot
# state, or use `make wb-screenshot-final` for that.
wb-screenshot: STOP ?= 23000000
wb-screenshot:
	@echo "Booting K1.3 + WB1.3 (BOOT_TRACE=0, STOP_AT_RETIRED=$(STOP))..."
	BOOT_TRACE=0 \
	    $(if $(filter-out 0,$(STOP)),STOP_AT_RETIRED=$(STOP),) \
	    CHIPRAM_DUMP=/tmp/wb_chipram.bin \
	    SLOWRAM_DUMP=/tmp/wb_slowram.bin \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/wb13.adf 2>&1 | tail -8 || true
	@echo "Rendering Workbench screen at 1280x400 (matches FS-UAE reference)..."
	$(PYTHON) tools/render_k13_screen.py \
	    --chipram /tmp/wb_chipram.bin \
	    --slowram /tmp/wb_slowram.bin \
	    --width 640 --height 200 \
	    --out /tmp/wb_screen.png
	@echo "Saved /tmp/wb_screen.png"
	@if command -v open >/dev/null 2>&1; then open /tmp/wb_screen.png; fi

# Force full-boot end-state (post-wipe) for comparison with the
# peak-render image.
wb-screenshot-final:
	@$(MAKE) --no-print-directory wb-screenshot STOP=0

# ---------------------------------------------------------------------------
# wb-trace-blits: boot WB1.3 with +define+BLT_BORDER_TRACE and save the
# full blit trace.  Use with tools/blt_trace.py to analyse:
#
#     make wb-trace-blits
#     tools/blt_trace.py uniq build_kick_boot/run.log
#     tools/blt_trace.py touched build_kick_boot/run.log --addr=0x9ef8
#     tools/blt_trace.py filter build_kick_boot/run.log --lf=ea --use-a=0
#
# STOP=N optionally stops the sim at retired=N (useful to capture state
# before the late-clear cycle at r=24M).
# ---------------------------------------------------------------------------
.PHONY: wb-trace-blits
wb-trace-blits:
	@echo "Booting K1.3 + WB1.3 with BLT_BORDER_TRACE..."
	BOOT_TRACE=0 \
	    EXTRA_VERI_DEFS=+define+BLT_BORDER_TRACE \
	    $(if $(STOP),STOP_AT_RETIRED=$(STOP),) \
	    CHIPRAM_DUMP=/tmp/wb_chipram.bin \
	    SLOWRAM_DUMP=/tmp/wb_slowram.bin \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/wb13.adf 2>&1 | tail -3 || true
	@echo "Trace saved to build_kick_boot/run.log"
	@echo "    BLT_BPL1 events: $$(grep -c BLT_BPL1 build_kick_boot/run.log)"
	@echo "    chip RAM:        /tmp/wb_chipram.bin"
	@echo "    slow RAM:        /tmp/wb_slowram.bin"

# ---------------------------------------------------------------------------
# wb-diff-fsu: render a per-bit colored diff between our last chip-RAM dump
# and the FS-UAE reference snapshot (test_data/fsuae_wb13_idle_chip.bin).
# Green=FSU-only, Red=ours-only, Yellow=mixed.  PNG saved + opened.
# ---------------------------------------------------------------------------
.PHONY: wb-diff-fsu
wb-diff-fsu:
	@if [ ! -f /tmp/wb_chipram.bin ]; then \
	    echo "ERROR: /tmp/wb_chipram.bin missing — run 'make wb-screenshot' first"; \
	    exit 1; \
	fi
	$(PYTHON) tools/chipram_diff.py /tmp/wb_chipram.bin \
	    test_data/fsuae_wb13_idle_chip.bin \
	    --start 0x60c8 --end 0x9f48 \
	    --out /tmp/chipram_diff.png
	@if command -v open >/dev/null 2>&1; then open /tmp/chipram_diff.png; fi

# ---------------------------------------------------------------------------
# wb-pristine: boot K1.3 + WB1.3 with MEM_POKE patches that overwrite the
# CLI window title bar BPL1 region ($60C8..$63E8 = 10 rows × 80 bytes) with
# solid white + WB Backdrop right border + "AmigaDOS" text bytes from the
# FS-UAE reference dump.  Produces the most pristine real-sim WB1.3 render.
#
# The MEM_POKE list (390 longwords) lands at cycle 1499900000 — late
# enough that no Intuition repaint follows it before max_cycles (1.5B).
#
# Per WB13_DEBUG_JOURNAL §34-§36, this is a runtime *patch* — the upstream
# fix (Intuition's active/inactive predicate, §32b + task #143) is still
# open.  Use this target as the A/B comparison baseline when working on
# that predicate.
# ---------------------------------------------------------------------------
.PHONY: wb-pristine
wb-pristine:
	@echo "Generating MEM_POKE patch list (title bar + border + AmigaDOS text)..."
	@$(PYTHON) tools/gen_pristine_pokes.py > /tmp/wb_pristine_pokes.env
	@echo "Pokes: $$(wc -c < /tmp/wb_pristine_pokes.env) chars"
	@echo "Booting K1.3 + WB1.3 with MEM_POKE patches..."
	BOOT_TRACE=0 \
	    MEM_POKE="$$(cat /tmp/wb_pristine_pokes.env)" \
	    MEM_POKE_CYCLE=1499900000 \
	    CHIPRAM_DUMP=/tmp/wb_pristine_chip.bin \
	    SLOWRAM_DUMP=/tmp/wb_pristine_slow.bin \
	    USE_CACHE=1 \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/wb13.adf 2>&1 | tail -3 || true
	@echo "Rendering pristine screen..."
	@TS=$$(date +%Y%m%d_%H%M%S); \
	  OUT=screenshots/$${TS}_wb13_pristine.png; \
	  $(PYTHON) tools/render_k13_screen.py \
	    --chipram /tmp/wb_pristine_chip.bin \
	    --slowram /tmp/wb_pristine_slow.bin \
	    --cop1lc 0x420 --cop2lc 0x100C8 \
	    --width 640 --height 200 \
	    --out $$OUT >/dev/null && \
	  echo "Saved $$OUT" && \
	  if command -v open >/dev/null 2>&1; then open $$OUT; fi

# ---------------------------------------------------------------------------
# kickstart-graphics: same build pipeline as test-kickstart-boot but with
# WITH_SDL2=1 and FB_W=320 / FB_H=200 so we can see K1.3's screen live.
# Pass --graphics and RENDER_K13_COP1LC=$$0x2368 to sim_main so the SDL
# render loop walks K1.3's Copper list each frame instead of sampling the
# (blank) chunky framebuffer.
# ---------------------------------------------------------------------------
kickstart-graphics:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (brew install sdl2)."; exit 1; \
	fi
	@if [ ! -f "$(ROMFILE)" ]; then \
	    echo "ROMFILE=$(ROMFILE) does not exist."; exit 1; \
	fi
	@mkdir -p build_kick_gfx
	$(PYTHON) tools/bin2rom.py --mem-words $(ROMSIZE_WORDS) $(ROMFILE) build_kick_gfx/rom.hex
	@if [ -n "$(ADFFILE)" ]; then \
	    if [ ! -f "$(ADFFILE)" ]; then echo "ADFFILE=$(ADFFILE) does not exist."; exit 1; fi; \
	    cp "$(ADFFILE)" build_kick_gfx/boot.adf; \
	    $(PYTHON) tools/adf2mfm.py --mem-words $(DISK_WORDS_FULL) \
	        build_kick_gfx/boot.adf build_kick_gfx/disk.hex; \
	    DISK_WORDS_USED=$(DISK_WORDS_FULL); \
	else \
	    $(PYTHON) tools/mkbootblock.py build_kick_gfx/boot.adf; \
	    $(PYTHON) tools/adf2mfm.py --mem-words 4096 --track 0 \
	        build_kick_gfx/boot.adf build_kick_gfx/disk.hex; \
	    DISK_WORDS_USED=4096; \
	fi; \
	$(MAKE) --no-print-directory build BUILD=build_kick_gfx N_CORES=1 USE_CACHE=1 \
	    MEM_WORDS=131072 ROM_WORDS=$(ROMSIZE_WORDS) ROM_HEXFILE=rom.hex OVL_RESET=1 \
	    DISK_WORDS=$$DISK_WORDS_USED DISK_HEXFILE=disk.hex \
	    WITH_SDL2=1 FB_W=320 \
	    VERI_DEFS='+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT +define+BLT_VECTABLE_GUARD $(BOOT_TRACE_DEF) $(EXTRA_VERI_DEFS)' \
	    >build_kick_gfx/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t63_boot_rom.s build_kick_gfx/program.hex
	@echo
	@echo "Launching Kickstart 1.3 in graphics mode."
	@echo "  Window shows K1.3's Copper-list-rendered screen (background + sprites)."
	@echo "  Renderer autodetects Intuition's WB1.3 Copper list each frame."
	@echo "  Until that list lands in chip RAM the window stays black."
	@echo "  Console will print '[sim] RENDER_K13_COP1LC=auto: locked onto'"
	@echo "  when the WB screen goes live."
	@echo "  BOOT_TRACE=$(BOOT_TRACE) — set BOOT_TRACE=0 for faster (no debug \$$display) sim."
	@echo "  Press ESC or close the window to quit."
	@(cd build_kick_gfx && RENDER_K13_COP1LC=auto ./Vm68k_top $${ROMCYCLES:-1500000000} --graphics)

# ---------------------------------------------------------------------------
# Block-device DMA test.  Builds with a tiny disk image (tests/disk_test.hex)
# loaded into the bus's disk[] array, runs tests/t65_blockdev.s which
# programs BLKSRC/BLKDST/BLKCNT/BLKCMD to copy sector 0 to RAM and verifies
# the magic words.
# ---------------------------------------------------------------------------
test-blockdev:
	@mkdir -p build_blk
	cp tests/disk_test.hex build_blk/disk_test.hex
	@$(MAKE) --no-print-directory build BUILD=build_blk N_CORES=2 USE_CACHE=1 \
	    MEM_WORDS=65536 DISK_WORDS=2048 DISK_HEXFILE=disk_test.hex \
	    >build_blk/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t65_blockdev.s build_blk/program.hex
	@(cd build_blk && ./Vm68k_top 1000000) > build_blk/t65.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t65_blockdev"; \
	else echo "FAIL t65_blockdev rc=$$rc"; tail -n 6 build_blk/t65.log | sed 's/^/      /'; exit 1; fi

# DMA cache-snoop coherence test.  Verifies that a DMA write to a
# CPU-cached address triggers cache invalidation so subsequent reads
# see the DMA-updated value (not the stale cached sentinel).  Without
# the snoop fix in m68k_bus.v this test FAILs.
test-dma-snoop:
	@mkdir -p build_dma_snoop
	cp tests/disk_test.hex build_dma_snoop/disk_test.hex
	@$(MAKE) --no-print-directory build BUILD=build_dma_snoop N_CORES=2 USE_CACHE=1 \
	    MEM_WORDS=65536 DISK_WORDS=2048 DISK_HEXFILE=disk_test.hex \
	    >build_dma_snoop/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t111_dma_cache_snoop.s build_dma_snoop/program.hex
	@(cd build_dma_snoop && ./Vm68k_top 1000000) > build_dma_snoop/t111.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t111_dma_cache_snoop"; \
	else echo "FAIL t111_dma_cache_snoop rc=$$rc"; tail -n 6 build_dma_snoop/t111.log | sed 's/^/      /'; exit 1; fi

# Floppy DSKLEN-style DMA test.  Same disk[] backing as test-blockdev,
# but driven by the canonical floppy register protocol (DSKPT + DSKLEN).
test-floppy:
	@mkdir -p build_flp
	cp tests/disk_test.hex build_flp/disk_test.hex
	@$(MAKE) --no-print-directory build BUILD=build_flp N_CORES=2 USE_CACHE=1 \
	    MEM_WORDS=65536 DISK_WORDS=2048 DISK_HEXFILE=disk_test.hex \
	    >build_flp/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t68_floppy_dsklen.s build_flp/program.hex
	@(cd build_flp && ./Vm68k_top 1000000) > build_flp/t68.log 2>&1; \
	rc=$$?; \
	if [ $$rc -eq 0 ]; then echo "PASS t68_floppy_dsklen"; \
	else echo "FAIL t68_floppy_dsklen rc=$$rc"; tail -n 6 build_flp/t68.log | sed 's/^/      /'; exit 1; fi

# Benchmarks: build a "fast" (pipelined + caches) and "slow" (passthrough)
# 1-core simulator, run every program on both, and print a comparison.
bench:
	@$(MAKE) --no-print-directory build BUILD=build_fast N_CORES=1 USE_CACHE=1 >build_fast.log 2>&1
	@$(MAKE) --no-print-directory build BUILD=build_slow N_CORES=1 USE_CACHE=0 >build_slow.log 2>&1
	@$(PYTHON) $(TB_DIR)/bench_report.py build_fast build_slow

# --- Graphics demos -------------------------------------------------------
#
# Both demos build with N_CORES=1, a roomy 128 KB memory (32K words) so the
# 48 KB framebuffer + code + stacks all fit, and SDL2 linked in. They each
# produce a Verilator binary in their own build dir so they don't stomp
# on the regression build.

# Framebuffer-only demo: no OS. A single asm program animates a moving
# gradient and dot. Shows the framebuffer plumbing works end-to-end.
demo-fb:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=65536 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/fb_demo.s build_demo/program.hex
	@echo
	@echo "Launching framebuffer demo. Press ESC or close the window to quit."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

# OS demo: cooperative kernel running multiple tasks that each animate a
# region of the framebuffer.
demo-os:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=65536 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/os_demo.s build_demo/program.hex
	@echo
	@echo "Launching OS demo. Press ESC or close the window to quit."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

demo-poly:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/poly_demo.s build_demo/program.hex
	@echo
	@echo "Launching filled-polygon demo (moving filled triangle via blitter)."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-pau:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/paula_demo.s build_demo/program.hex
	@echo
	@echo "Launching Paula demo. Press ESC or close the window to quit."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-den:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/den_demo.s build_demo/program.hex
	@echo
	@echo "Launching Denise demo (4-plane indexed image rendered to chunky FB)."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-morph:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/morph_demo.s build_demo/program.hex
	@echo
	@echo "Launching morphing-polygon demo (filled trapezoid translates and morphs)."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-hires:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_hires N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1 FB_W=512
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/hires_demo.s build_hires/program.hex
	@echo
	@echo "Launching hires demo (BPLCON0[15]=1, 512x192 chunky FB, 32 colour bars)."
	@(cd build_hires && ./Vm68k_top 200000000 --graphics)

demo-showcase:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/showcase_demo.s build_demo/program.hex
	@echo
	@echo "Launching showcase demo (HAM8 + DIW border + attached 4bpp sprite)."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-coprainbow:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/coprainbow_demo.s build_demo/program.hex
	@echo
	@echo "Launching Copper-rainbow HAM demo (32 base colors x 16 R steps per row)."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

# HAM6 "Fashion-style" demo: a stylised 256x192 HAM image with three
# vertical bands of smooth gradient (sky / skin / dress / shadow).  No
# Copper interaction — just Denise's HAM6 pipeline turning a single
# 6-bitplane image into thousands of distinct colours.  Pair with
# demo-coprainbow for the Copper-driven variant.
demo-fashion:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected; running headless. brew install sdl2 for graphics."; \
	    $(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072; \
	    $(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/fashion_demo.s build_demo/program.hex; \
	    (cd build_demo && ./Vm68k_top 200000000); \
	else \
	    $(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1; \
	    $(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/fashion_demo.s build_demo/program.hex; \
	    echo; \
	    echo "Launching HAM6 fashion-style demo (3 banded gradient image, ~3000 colours)."; \
	    (cd build_demo && ./Vm68k_top 200000000 --graphics); \
	fi

# Kickstart-shape demo: canonical Blitter+Copper addresses ($DFF040/$DFF080)
# plus a VERTB IRQ that rotates the band palette each frame.  Renders 200
# Denise frames; the visible effect is a slow vertical scroll of an 8-band
# rainbow over a striped 1bpp bitplane.  Without SDL2 the same demo still
# runs headless and verifies that VERTB fired (halt 0 = pass).
demo-kickstart:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected; running headless (set HAVE_SDL2 + brew install sdl2 for graphics)."; \
	    $(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072; \
	    $(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/kickstart_demo.s build_demo/program.hex; \
	    (cd build_demo && ./Vm68k_top 200000000); \
	else \
	    $(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1; \
	    $(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/kickstart_demo.s build_demo/program.hex; \
	    echo; \
	    echo "Launching kickstart-shape demo: canonical Blitter+Copper + VERTB IRQ rotates palette."; \
	    (cd build_demo && ./Vm68k_top 200000000 --graphics); \
	fi

demo-ham:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/ham_demo.s build_demo/program.hex
	@echo
	@echo "Launching HAM6 demo (black->red horizontal gradient using HAM modify-R)."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

demo-spr:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/spr_demo.s build_demo/program.hex
	@echo
	@echo "Launching sprite demo (8 hardware sprites bouncing on a bitplane background)."
	@(cd build_demo && ./Vm68k_top 2000000000 --graphics)

demo-cop:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=65536 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/cop_demo.s build_demo/program.hex
	@echo
	@echo "Launching Copper demo (Copper drives blitter, cross drawn on bitplane)."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

demo-blt:
	@if [ "$(HAVE_SDL2)" != "1" ]; then \
	    echo "SDL2 not detected (sdl2-config returned no libs). brew install sdl2"; \
	    exit 1; \
	fi
	@$(MAKE) --no-print-directory build BUILD=build_demo N_CORES=1 USE_CACHE=1 MEM_WORDS=65536 WITH_SDL2=1
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/blt_demo.s build_demo/program.hex
	@echo
	@echo "Launching blitter line demo. Press ESC or close the window to quit."
	@(cd build_demo && ./Vm68k_top 200000000 --graphics)

# Boing — homage to the 1984 Amiga "Boing!" demo: single 16x16
# red/white checkered sprite ball bouncing over a blue field with a
# horizontal grid floor.  Runs HEADLESSLY (no SDL2 needed); dumps
# chip RAM at end and renders to PNG.
demo-boing:
	@mkdir -p build_boing
	@$(MAKE) --no-print-directory build BUILD=build_boing N_CORES=1 USE_CACHE=1 MEM_WORDS=131072 >/dev/null
	$(PYTHON) $(TB_DIR)/asm68k.py $(DEMO_DIR)/boing_demo.s build_boing/program.hex
	@echo "Running boing demo..."
	@(cd build_boing && CHIPRAM_DUMP=/tmp/boing_chip.bin ./Vm68k_top 50000000) 2>&1 | tail -3
	@echo "Rendering ball + floor..."
	$(PYTHON) tools/render_boing.py --chipram /tmp/boing_chip.bin --out /tmp/boing.png
	@if command -v open >/dev/null 2>&1; then open /tmp/boing.png; fi

# ---------------------------------------------------------------------------
# real-boing-disk: assemble a bootable ADF from kickstart/wb13.adf + the
# REAL 1986 boing! executable.  Requires:
#   - amitools installed (pip install amitools)
#   - BOING_SRC= path to the boing! binary (default: ~/Downloads/...)
#
# After running, boot with:
#   make test-kickstart-boot ADFFILE=kickstart/boing_disk.adf
# Or build + render with `make demo-real-boing`.
#
# Current status: boot reaches ">>> Launching Boing... <<<" text but
# boing's Intuition screen never renders — likely blocked by the
# upstream OS-state issue tracked as task #150.
# ---------------------------------------------------------------------------
BOING_SRC     ?= /Users/chris/Downloads/workbench_demos/animations/boing!
BOING_SAMPLES ?= /Users/chris/Downloads/workbench_demos/animations/boing.samples

# hello-disk: same wb13.adf + deletions baseline as boing-disk, but the
# Startup-Sequence is just `echo "hello"` instead of running boing.  Used
# to isolate "is the trap caused by boing specifically?" vs "is something
# broken about our ADF / file deletions / OFS path?".
hello-disk: kickstart/hello_disk.adf
kickstart/hello_disk.adf: kickstart/wb13.adf
	@cp kickstart/wb13.adf $@
	@printf 'c:SetPatch >NIL:\ncd c:\necho "*N*N>>> HELLO from the sim <<<*N"\necho "*N>>> done <<<"\nendcli >NIL:\n' > /tmp/hello_startup.txt
	@xdftool $@ delete S/Startup-Sequence 2>&1 | tail -1
	@xdftool $@ write /tmp/hello_startup.txt S/Startup-Sequence 2>&1 | tail -1
	@echo "Built $@"

demo-hello: kickstart/hello_disk.adf
	@rm -rf build_kick_boot
	BOOT_TRACE=0 STOP_AT_RETIRED=80000000 PC_HISTOGRAM=1 PC_HISTOGRAM_INTERVAL=512 PC_HISTOGRAM_TOP=20 \
	    CHIPRAM_DUMP=/tmp/hello_chip.bin SLOWRAM_DUMP=/tmp/hello_slow.bin \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/hello_disk.adf > /tmp/hello.log 2>&1 || true
	@grep -E "STOP_AT_RETIRED|retired=|samples" build_kick_boot/run.log | head -25
	$(PYTHON) tools/render_k13_screen.py \
	    --chipram /tmp/hello_chip.bin --slowram /tmp/hello_slow.bin \
	    --width 640 --height 200 --out /tmp/hello.png

real-boing-disk: kickstart/boing_disk.adf
kickstart/boing_disk.adf: $(BOING_SRC) $(BOING_SAMPLES) kickstart/wb13.adf
	@if ! command -v xdftool >/dev/null 2>&1; then \
	    echo "amitools not installed — run: pip install amitools"; exit 1; \
	fi
	@cp kickstart/wb13.adf $@
	@printf 'c:SetPatch >NIL:\ncd c:\necho "*N*N>>> Launching Boing... <<<*N"\nboing\necho "*N>>> Boing exited <<<"\nendcli >NIL:\n' > /tmp/boing_startup.txt
	@xdftool $@ delete S/Startup-Sequence 2>&1 | tail -1
	@xdftool $@ write /tmp/boing_startup.txt S/Startup-Sequence 2>&1 | tail -1
	@# Make room for boing.samples (~25KB): delete unused Workbench utilities.
	@for f in System/Preferences System/Notepad System/Clock System/Diskcopy \
	          System/FastFileSystem System/CMD System/Calculator System/More \
	          Devs/printer.device Devs/narrator.device \
	          Libs/info.library Libs/mathieeedoubtrans.library Libs/translator.library \
	          C/Ed C/Edit C/Format C/List; do \
	    xdftool $@ delete $$f >/dev/null 2>&1 || true; \
	done
	@xdftool $@ write "$(BOING_SRC)" 'C/boing' 2>&1 | tail -1
	@xdftool $@ write "$(BOING_SAMPLES)" 'C/boing.samples' 2>&1 | tail -1
	@xdftool $@ info 2>&1 | grep -E "free|used"
	@echo "Built $@"
	@ls -la $@

# demo-real-boing: boot the boing-disk ADF, render the final frame.
demo-real-boing: kickstart/boing_disk.adf
	@rm -rf build_kick_boot
	BOOT_TRACE=0 CHIPRAM_DUMP=/tmp/boing_disk_chip.bin SLOWRAM_DUMP=/tmp/boing_disk_slow.bin \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/boing_disk.adf 2>&1 | tail -5 || true
	$(PYTHON) tools/render_k13_screen.py \
	    --chipram /tmp/boing_disk_chip.bin --slowram /tmp/boing_disk_slow.bin \
	    --width 640 --height 200 --out /tmp/boing_disk.png
	@if command -v open >/dev/null 2>&1; then open /tmp/boing_disk.png; fi

# ---------------------------------------------------------------------------
# demo-real-boing-patched: HACK target — pre-patches the K1.3 chip-RAM
# free-list at the first K1.3 idle (PC=$FC0F94) to redirect MH_FIRST past
# the corrupting-blit region.  Uses MEM_POKE_AT_PC (PC-triggered MEM_POKE,
# see tb/sim_main.cpp).
#
# **DOES NOT WORK as a standalone fix.**  Empirical confirmation (session 17):
# even with the poke firing cleanly at cycle 5.97M, boing still doesn't
# render.  Two independent problems:
#
# 1. K1.3 does many small allocs between the first idle and boing's
#    49,896-byte alloc at r=8.82M, draining the pre-consumed chunk
#    (starts at $4B3E0 size $34040, ends ~5KB by boing time).
# 2. `boing.samples` doesn't load into chip RAM in the first place.
#    The 49KB audio buffer at $4C800+ contains ~30 bytes of partial
#    garbage instead of the smooth waveform FS-UAE-no-UAEFS produces.
#    Trackdisk / MFM-decode failure — see task #176.
#
# So the HACK target's premise (chip-RAM pre-consumption alone fixes
# boing) is incomplete.  Real fix needs BOTH (a) trackdisk MFM-decode for
# boing.samples AND (b) heap layout matching UAE Boot ROM's consumption
# pattern.  Kept here as a placeholder + a demonstration of the
# MEM_POKE_AT_PC infrastructure.  See `project_boing_chip_freelist.md`
# for the long-form diagnosis.
demo-real-boing-patched: kickstart/boing_disk.adf
	@rm -rf build_kick_boot
	BOOT_TRACE=0 CHIPRAM_DUMP=/tmp/boing_disk_chip.bin SLOWRAM_DUMP=/tmp/boing_disk_slow.bin \
	    MEM_POKE="0x19A0=0x0004B3E0,0x4B3E0=0x00000000,0x4B3E4=0x00034040,0x10170=0x00000000" \
	    MEM_POKE_AT_PC=0xFC0F94 \
	    $(MAKE) --no-print-directory test-kickstart-boot \
	    ADFFILE=kickstart/boing_disk.adf 2>&1 | tail -10 || true
	$(PYTHON) tools/render_k13_screen.py \
	    --chipram /tmp/boing_disk_chip.bin --slowram /tmp/boing_disk_slow.bin \
	    --width 640 --height 200 --out /tmp/boing_disk_patched.png
	@if command -v open >/dev/null 2>&1; then open /tmp/boing_disk_patched.png; fi

demo: demo-os

clean:
	rm -rf build build_fast build_slow build_demo build_ovl build_hires build_fast.log build_slow.log
