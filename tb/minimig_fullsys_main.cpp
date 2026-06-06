// Minimig-fullsys Phase 0 smoke test.
//
// Drives `minimig_fullsys_top` (m68k_core + minimal bus arbiter + flat
// memory model + K1.3 ROM) and watches PC progress.
//
// Pass criterion: retired > 100K and current PC inside ROM
// ($00FC_xxxx range).  That confirms the CPU is executing ROM code
// through the new harness, not the existing m68k_top / m68k_bus stack.
//
// Use:
//   tools/bin2rom.py kickstart/kick_13.bin build_fullsys/rom.hex
//   make minimig-fullsys
//   make fullsys-smoke

#include "Vminimig_fullsys_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>

static void tick(Vminimig_fullsys_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vminimig_fullsys_top;

    // Reset for 16 cycles.
    dut->clk = 0; dut->rst_n = 0;
    for (int i = 0; i < 16; i++) tick(dut);
    dut->rst_n = 1;

    // Max cycle budget — env override.
    uint64_t max_cycles = 5'000'000;
    if (const char* s = std::getenv("MAX_CYCLES"))
        max_cycles = std::strtoull(s, nullptr, 0);

    // Trace cadence: print PC + retired every N cycles.
    uint64_t trace_every = 100'000;
    if (const char* s = std::getenv("TRACE_EVERY"))
        trace_every = std::strtoull(s, nullptr, 0);

    uint32_t last_pc = 0xFFFFFFFFu;
    uint64_t same_pc_count = 0;
    bool reached_rom = false;
    uint64_t reached_rom_cycle = 0;

    printf("[fullsys] Phase 0 smoke test — boot K1.3 ROM through minimal harness\n");
    printf("[fullsys] max_cycles=%llu trace_every=%llu\n",
           (unsigned long long)max_cycles, (unsigned long long)trace_every);

    for (uint64_t cycle = 0; cycle < max_cycles; cycle++) {
        tick(dut);

        uint32_t pc       = (uint32_t)dut->cur_pc;
        uint32_t retired  = (uint32_t)dut->retired;
        bool     halted   = dut->halted;
        uint32_t pc_masked = pc & 0x00FFFFFFu;

        // First time we see PC inside ROM ($FCxxxx) — record it.
        if (!reached_rom && pc_masked >= 0x00FC0000u && pc_masked <= 0x00FFFFFFu) {
            reached_rom = true;
            reached_rom_cycle = cycle;
            printf("[fullsys] FIRST ROM PC reached at cycle=%llu retired=%u pc=$%08X\n",
                   (unsigned long long)cycle, retired, pc);
        }

        if (cycle > 0 && (cycle % trace_every) == 0) {
            printf("[fullsys] cycle=%llu retired=%u pc=$%08X stop=%d halted=%d\n",
                   (unsigned long long)cycle, retired, pc,
                   (int)dut->cpu_in_stop, (int)halted);
        }

        if (halted) {
            uint16_t hc = (uint16_t)dut->halt_code;
            printf("[fullsys] HALTED at cycle=%llu retired=%u pc=$%08X halt_code=$%04X\n",
                   (unsigned long long)cycle, retired, pc, hc);
            break;
        }

        // Detect stall: same PC for 100K cycles (CPU stuck).
        if (pc == last_pc) {
            same_pc_count++;
            if (same_pc_count >= 100'000 && !dut->cpu_in_stop) {
                printf("[fullsys] STALL: same PC=$%08X for 100K cycles (retired=%u)\n",
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
    printf("[fullsys] FINAL pc=$%08X retired=%u reached_rom=%d\n",
           final_pc, final_retired, (int)reached_rom);

    delete dut;

    // Pass: CPU reached ROM execution and retired > 100K instructions.
    if (reached_rom && final_retired >= 100'000) {
        printf("[fullsys] PASS — CPU boots K1.3 ROM through Phase 0 harness "
               "(reached ROM at cycle %llu, retired %u)\n",
               (unsigned long long)reached_rom_cycle, final_retired);
        return 0;
    }
    printf("[fullsys] FAIL — reached_rom=%d retired=%u\n",
           (int)reached_rom, final_retired);
    return 1;
}
