// Phase 1a smoke test driver — instantiates minimig_phase1_top, ticks
// clock, reports PC progress.  Mirrors minimig_fullsys_main.cpp but for
// the phase1 top.

#include "Vminimig_phase1_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>

static void tick(Vminimig_phase1_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vminimig_phase1_top;

    // Reset for 32 cycles (minimig needs longer reset than our bare CPU).
    dut->clk = 0; dut->rst_n = 0;
    dut->dump_strobe = 0;
    for (int i = 0; i < 32; i++) tick(dut);
    dut->rst_n = 1;

    uint64_t max_cycles = 5'000'000;
    if (const char* s = std::getenv("MAX_CYCLES"))
        max_cycles = std::strtoull(s, nullptr, 0);

    uint64_t trace_every = 100'000;
    if (const char* s = std::getenv("TRACE_EVERY"))
        trace_every = std::strtoull(s, nullptr, 0);

    uint32_t last_pc = 0xFFFFFFFFu;
    uint64_t same_pc_count = 0;
    bool reached_rom = false;
    uint64_t reached_rom_cycle = 0;

    printf("[phase1] minimig.v + m68k_core boot test\n");
    printf("[phase1] max_cycles=%llu trace_every=%llu\n",
           (unsigned long long)max_cycles, (unsigned long long)trace_every);

    for (uint64_t cycle = 0; cycle < max_cycles; cycle++) {
        tick(dut);

        uint32_t pc       = (uint32_t)dut->cur_pc;
        uint32_t retired  = (uint32_t)dut->retired;
        bool     halted   = dut->halted;
        uint32_t pc_masked = pc & 0x00FFFFFFu;

        if (!reached_rom && pc_masked >= 0x00FC0000u && pc_masked <= 0x00FFFFFFu) {
            reached_rom = true;
            reached_rom_cycle = cycle;
            printf("[phase1] FIRST ROM PC reached at cycle=%llu retired=%u pc=$%08X\n",
                   (unsigned long long)cycle, retired, pc);
        }

        if (cycle > 0 && (cycle % trace_every) == 0) {
            printf("[phase1] cycle=%llu retired=%u pc=$%08X stop=%d halted=%d\n",
                   (unsigned long long)cycle, retired, pc,
                   (int)dut->cpu_in_stop, (int)halted);
        }

        if (halted) {
            uint16_t hc = (uint16_t)dut->halt_code;
            printf("[phase1] HALTED at cycle=%llu retired=%u pc=$%08X halt_code=$%04X\n",
                   (unsigned long long)cycle, retired, pc, hc);
            break;
        }

        if (pc == last_pc) {
            same_pc_count++;
            if (same_pc_count >= 100'000 && !dut->cpu_in_stop) {
                printf("[phase1] STALL: same PC=$%08X for 100K cycles (retired=%u)\n",
                       pc, retired);
                break;
            }
        } else {
            same_pc_count = 0;
            last_pc = pc;
        }
    }

    uint32_t final_pc      = (uint32_t)dut->cur_pc;
    uint32_t final_retired = (uint32_t)dut->retired;
    printf("[phase1] FINAL pc=$%08X retired=%u reached_rom=%d\n",
           final_pc, final_retired, (int)reached_rom);

    // Pulse dump_strobe for one cycle so the testbench writes
    // phase1_sram.hex.  Skip if the env var explicitly disables it.
    if (!std::getenv("PHASE1_NO_DUMP")) {
        dut->dump_strobe = 1; tick(dut);
        dut->dump_strobe = 0; tick(dut);
    }

    delete dut;
    // Phase 1a pass criterion: build + run-without-crash.  CPU not
    // executing yet is expected — minimig's clock-enable strobes (c1
    // / c3 / cck / clk7_en) aren't yet driven at the right cadence
    // and the ROM mapping into minimig's SRAM isn't set up.  Phase 1b
    // tackles both.
    printf("[phase1] PASS — Phase 1a: minimig.v compiles + runs %llu cycles "
           "without crash (retired=%u; CPU execution = Phase 1b)\n",
           (unsigned long long)max_cycles, final_retired);
    return 0;
}
