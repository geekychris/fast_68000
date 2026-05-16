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
            $(RTL_DIR)/m68k_blitter.v \
            $(RTL_DIR)/m68k_copper.v \
            $(RTL_DIR)/m68k_denise.v \
            $(RTL_DIR)/m68k_paula.v \
            $(RTL_DIR)/m68k_core.v \
            $(RTL_DIR)/m68k_top.v

TESTS  := $(wildcard $(TESTS_DIR)/*.s)
BENCHES:= $(wildcard $(BENCH_DIR)/*.s)

N_CORES   ?= 2
MEM_WORDS ?= 65536
BUILD     ?= build

.PHONY: all build test bench clean demo demo-fb demo-os demo-blt demo-cop demo-den demo-pau demo-poly demo-spr

all: test

build: $(BUILD)/Vm68k_top

# Generic build rule: BUILD, N_CORES, USE_CACHE, MEM_WORDS, WITH_SDL2 control the output.
$(BUILD)/Vm68k_top: $(RTL_SRCS) $(TB_DIR)/sim_main.cpp
	@mkdir -p $(BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build \
	    -CFLAGS "-DNUM_CORES=$(N_CORES) -std=c++17 -O1 $(if $(WITH_SDL2),-DHAVE_SDL2 $(SDL2_CFLAGS),)" \
	    $(if $(WITH_SDL2),-LDFLAGS "$(SDL2_LIBS)",) \
	    -GN_CORES=$(N_CORES) \
	    -GUSE_CACHE=$(if $(USE_CACHE),$(USE_CACHE),1) \
	    -GMEM_WORDS=$(MEM_WORDS) \
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
	    $(PYTHON) $(TB_DIR)/asm68k.py $$t build/program.hex; \
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
	rm -rf build build_fast build_slow build_demo build_fast.log build_slow.log
