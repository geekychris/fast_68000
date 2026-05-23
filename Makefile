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

RTL_SRCS := $(RTL_DIR)/m68k_alu.v \
            $(RTL_DIR)/m68k_regfile.v \
            $(RTL_DIR)/m68k_decoder.v \
            $(RTL_DIR)/m68k_cache.v \
            $(RTL_DIR)/m68k_passthrough.v \
            $(RTL_DIR)/m68k_bus.v \
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

TESTS  := $(filter-out $(TESTS_DIR)/t61_ovl.s $(TESTS_DIR)/t63_boot_rom.s $(TESTS_DIR)/t65_blockdev.s $(TESTS_DIR)/t66_boot_rom_ext.s $(TESTS_DIR)/t68_floppy_dsklen.s $(TESTS_DIR)/t69_fake_kickstart.s,$(wildcard $(TESTS_DIR)/*.s))
# Multicore tests (t06_multicore, t07_coherence) need N_CORES >= 2.
ifeq ($(N_CORES),1)
TESTS := $(filter-out $(TESTS_DIR)/t06_multicore.s $(TESTS_DIR)/t07_coherence.s,$(TESTS))
endif
BENCHES:= $(wildcard $(BENCH_DIR)/*.s)

.PHONY: all build test test-ovl test-all test-boot-rom test-boot-rom-ext test-blockdev test-floppy test-boot-rom-bin test-fake-kickstart test-kickstart-boot bench clean demo demo-fb demo-os demo-blt demo-cop demo-den demo-pau demo-poly demo-spr demo-morph demo-ham demo-coprainbow demo-showcase demo-hires demo-kickstart demo-fashion fetch-musashi musashi crosscheck fetch-fx68k fx68k crosscheck-fx68k crosscheck-all demos-c demos-c-build cc68k-image fetch-minimig minimig crosscheck-minimig

all: test

build: $(BUILD)/Vm68k_top

# Generic build rule: BUILD, N_CORES, USE_CACHE, MEM_WORDS, WITH_SDL2 control the output.
# Optional ROM_WORDS / ROM_HEXFILE / OVL_RESET configure the Kickstart-ROM
# region and reset-time OVL latch.
$(BUILD)/Vm68k_top: $(RTL_SRCS) $(TB_DIR)/sim_main.cpp
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
	    $(RTL_SRCS) $(TB_DIR)/sim_main.cpp

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

# All tests including special-build OVL test.
test-all: test test-ovl

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
# and a tiny ROM hex pre-loaded into rom[], then runs tests/t61_ovl.s
# against it.  Not part of the default `make test` because the OVL=1 reset
# state would break tests that boot directly at PC=$400 with low-RAM reads.
test-ovl:
	@mkdir -p build_ovl
	cp tests/rom_test.hex build_ovl/rom_test.hex
	@$(MAKE) --no-print-directory build BUILD=build_ovl N_CORES=$(N_CORES) USE_CACHE=1 \
	    MEM_WORDS=65536 ROM_WORDS=16 ROM_HEXFILE=rom_test.hex OVL_RESET=1 \
	    >build_ovl/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t61_ovl.s build_ovl/program.hex
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
	    VERI_DEFS='+define+KICKSTART_BOOT +define+KICKSTART_FAST_BOOT +define+KICKSTART_BOOT_TRACE $(EXTRA_VERI_DEFS)' \
	    >build_kick_boot/_build.log 2>&1
	$(PYTHON) $(TB_DIR)/asm68k.py tests/t63_boot_rom.s build_kick_boot/program.hex
	@echo "Running Kickstart + bootblock disk for up to $${ROMCYCLES:-1500000000} cycles..."
	@(cd build_kick_boot && ./Vm68k_top $${ROMCYCLES:-1500000000}) > build_kick_boot/run.log 2>&1; \
	rc=$$?; \
	if grep -q '\[BOOTBLOCK\]' build_kick_boot/run.log; then \
	    echo "PASS test-kickstart-boot (bootblock executed)"; \
	    grep -E '\[OVL\]|\[DSKLEN\]|\[BOOTBLOCK\]' build_kick_boot/run.log; \
	    tail -3 build_kick_boot/run.log; \
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

demo: demo-os

clean:
	rm -rf build build_fast build_slow build_demo build_ovl build_hires build_fast.log build_slow.log
