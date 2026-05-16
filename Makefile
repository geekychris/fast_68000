# Build & run the 68K-mini Verilator simulation, plus tests + benchmarks.

VERILATOR ?= verilator
PYTHON    ?= python3

RTL_DIR   := rtl
TB_DIR    := tb
TESTS_DIR := tests
BENCH_DIR := bench

RTL_SRCS := $(RTL_DIR)/m68k_alu.v \
            $(RTL_DIR)/m68k_regfile.v \
            $(RTL_DIR)/m68k_decoder.v \
            $(RTL_DIR)/m68k_cache.v \
            $(RTL_DIR)/m68k_passthrough.v \
            $(RTL_DIR)/m68k_bus.v \
            $(RTL_DIR)/m68k_core.v \
            $(RTL_DIR)/m68k_top.v

TESTS  := $(wildcard $(TESTS_DIR)/*.s)
BENCHES:= $(wildcard $(BENCH_DIR)/*.s)

N_CORES ?= 2
BUILD ?= build

.PHONY: all build test bench clean

all: test

build: $(BUILD)/Vm68k_top

# Generic build rule: BUILD, N_CORES and USE_CACHE control the output.
$(BUILD)/Vm68k_top: $(RTL_SRCS) $(TB_DIR)/sim_main.cpp
	@mkdir -p $(BUILD)
	$(VERILATOR) -Wno-fatal --cc --exe --build \
	    -CFLAGS "-DNUM_CORES=$(N_CORES) -std=c++17 -O1" \
	    -GN_CORES=$(N_CORES) \
	    -GUSE_CACHE=$(if $(USE_CACHE),$(USE_CACHE),1) \
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
	    (cd build && ./Vm68k_top 200000) > build/$$name.log 2>&1; \
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

clean:
	rm -rf build build_fast build_slow build_fast.log build_slow.log
