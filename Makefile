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
TESTS  := $(filter-out $(TESTS_DIR)/t61_ovl.s,$(wildcard $(TESTS_DIR)/*.s))
BENCHES:= $(wildcard $(BENCH_DIR)/*.s)

N_CORES   ?= 2
MEM_WORDS ?= 65536
BUILD     ?= build

.PHONY: all build test test-ovl test-all bench clean demo demo-fb demo-os demo-blt demo-cop demo-den demo-pau demo-poly demo-spr demo-morph demo-ham demo-coprainbow demo-showcase demo-hires fetch-musashi musashi crosscheck fetch-fx68k fx68k crosscheck-fx68k crosscheck-all demos-c demos-c-build cc68k-image

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
    $(TESTS_DIR)/t57_exg_pcidx.s

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
