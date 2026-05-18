// Cross-validation Verilator harness for FX68K.  Same protocol as
// sim_main.cpp: ticks the clock until `halted` goes high, prints the
// halt code in the same format, exits with low byte of halt code.
#include "Vfx68k_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    long max_cycles = 5000000;
    if (argc >= 2) max_cycles = std::strtol(argv[1], nullptr, 10);

    auto* dut = new Vfx68k_top;

    dut->rst_n = 0;
    for (int i = 0; i < 8; i++) {
        dut->clk = 0; dut->eval();
        dut->clk = 1; dut->eval();
    }
    dut->rst_n = 1;

    bool debug = std::getenv("FX68K_TRACE") != nullptr;
    long total = 0;
    // Halt detection: oHALTEDn drop (valid STOP) OR no new program fetch
    // for IDLE_CYCLES (a privilege-violation-trapped STOP eventually loops
    // in the vector handler with no in-test fetches).  We use whatever
    // STOP-imm candidate the SV captured most recently.
    constexpr long IDLE_CYCLES = 4000;
    uint32_t last_fetches = 0;
    long last_progress_cycle = 0;
    while (total < max_cycles) {
        dut->clk = 0; dut->eval();
        dut->clk = 1; dut->eval();
        total++;
        if (!dut->dbg_oHALTEDn) break;             // valid STOP halt
        if (dut->priv_vio_seen) break;             // STOP #imm with S=0 -> priv vio
        if (dut->prog_fetches != last_fetches) {
            last_fetches = dut->prog_fetches;
            last_progress_cycle = total;
        } else if (total - last_progress_cycle > IDLE_CYCLES) {
            break;                                 // CPU stuck in a tight loop
        }
        if (debug && (total % 5000 == 0)) {
            std::printf("  [t=%ld] prog=%u eab=0x%06x lf=0x%04x halt_code=0x%04x\n",
                        total, (unsigned)dut->prog_fetches,
                        (unsigned)dut->dbg_last_eab,
                        (unsigned)dut->dbg_last_fetch & 0xFFFF,
                        (unsigned)dut->halt_code & 0xFFFF);
        }
    }
    dut->halted = 1;

    if (dut->halted) {
        std::printf("[fx68k] halt code=0x%04x cycles=%ld bus_cycles=%u prog_fetches=%u\n",
                    dut->halt_code & 0xFFFF, total,
                    (unsigned)dut->bus_cycles, (unsigned)dut->prog_fetches);
        int rc = dut->halt_code & 0xFF;
        if (rc == 0 && dut->halt_code != 0) rc = 1;
        delete dut;
        return rc;
    }
    std::printf("[fx68k] TIMEOUT cycles=%ld bus_cycles=%u prog_fetches=%u oHALTEDn=%d extReset=%d\n",
                total, (unsigned)dut->bus_cycles, (unsigned)dut->prog_fetches,
                (int)dut->dbg_oHALTEDn, (int)dut->dbg_extReset);
    std::printf("[fx68k]   mem@0x400=0x%08x last_eab=0x%08x last_fetch=0x%04x\n",
                (unsigned)dut->dbg_mem_at_400,
                (unsigned)dut->dbg_last_eab,
                (unsigned)dut->dbg_last_fetch & 0xFFFF);
    delete dut;
    return 255;
}
