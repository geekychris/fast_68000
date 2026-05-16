// Verilator C++ testbench harness for the 68K-mini multi-core SoC.
//
// Usage:
//   sim_main <max_cycles>
//
// Reads program.hex from cwd (loaded by $readmemh inside m68k_bus.v).
// Runs until every core halts or until max_cycles. Prints one line per core
// with cycles, retired instructions, IPC, and halt code. Returns 0 on success
// (every core halted with code 0), otherwise the first non-zero halt code.

#include <verilated.h>
#include "Vm68k_top.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    uint64_t max_cycles = 200000;
    if (argc >= 2) max_cycles = std::strtoull(argv[1], nullptr, 0);

    Vm68k_top* top = new Vm68k_top;

    top->clk = 0;
    top->rst_n = 0;
    for (int i = 0; i < 8; i++) {
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
    }
    top->rst_n = 1;

#ifndef NUM_CORES
#define NUM_CORES 2
#endif
    const int n_cores = NUM_CORES;

    auto all_halted = [&]() {
        for (int c = 0; c < n_cores; c++) {
            if (((top->halted >> c) & 1) == 0) return false;
        }
        return true;
    };

    uint64_t cycle = 0;
    while (cycle < max_cycles && !all_halted()) {
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
        cycle++;
    }

    bool halted_all = all_halted();
    int rc = 0;

    printf("[sim] cycles=%llu halted_mask=0x%x\n",
           (unsigned long long)cycle, (unsigned)top->halted);

    // The retired_flat output is 32*N_CORES bits packed. Verilator emits the
    // smallest-fitting C type; for up to 4 cores (128 bits), it's a WData
    // array. Cast to uint32_t* and index per core.
    const uint32_t* retired_arr = reinterpret_cast<const uint32_t*>(&top->retired_flat);
    for (int c = 0; c < n_cores; c++) {
        uint16_t code = 0;
        if (n_cores <= 2) code = (top->halt_code_flat >> (16 * c)) & 0xFFFF;
        else {
            const uint32_t* hcf = reinterpret_cast<const uint32_t*>(&top->halt_code_flat);
            code = (hcf[c / 2] >> (16 * (c % 2))) & 0xFFFF;
        }
        uint32_t retired = retired_arr[c];
        bool h = ((top->halted >> c) & 1) != 0;
        double ipc = (cycle == 0) ? 0.0 : (double)retired / (double)cycle;
        printf("[sim] core%d halted=%d code=0x%04x retired=%u ipc=%.3f\n",
               c, h, code, (unsigned)retired, ipc);
        if (!h) rc = rc ? rc : 0xFFFF;
        else if (code != 0 && rc == 0) rc = code;
    }
    if (!halted_all) {
        printf("[sim] TIMEOUT after %llu cycles\n", (unsigned long long)cycle);
        if (rc == 0) rc = 0xFFFE;
    }
    delete top;
    return rc;
}
