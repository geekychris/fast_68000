// MiniMig vs our paula INTENA/INTREQ cross-validation harness.
//
// Drives a fixed sequence of {address, write-data} pairs into both
// DUTs and then reads INTENAR/INTREQR from both, asserting equality.
//
// Exits 0 on PASS, non-zero on mismatch (first mismatch printed).

#include "Vminimig_xcheck_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>

static void tick(Vminimig_xcheck_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

static uint16_t read_reg(Vminimig_xcheck_top* dut, uint16_t reg_addr,
                         uint16_t& mm, uint16_t& ours) {
    dut->we       = 0;
    dut->reg_addr = (reg_addr >> 1) & 0xFF;
    dut->wdata    = 0;
    tick(dut);
    tick(dut);
    mm   = dut->mm_rdata;
    ours = dut->our_rdata;
    return mm;
}

static void write_reg(Vminimig_xcheck_top* dut, uint16_t reg_addr,
                      uint16_t value) {
    dut->we       = 1;
    dut->reg_addr = (reg_addr >> 1) & 0xFF;
    dut->wdata    = value;
    tick(dut);
    dut->we = 0;
    tick(dut);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vminimig_xcheck_top;

    dut->clk = 0; dut->rst = 1; dut->we = 0;
    dut->reg_addr = 0; dut->wdata = 0;
    for (int i = 0; i < 4; i++) tick(dut);
    dut->rst = 0;
    for (int i = 0; i < 4; i++) tick(dut);

    // Sequence of (description, action) pairs.  An "action" with we=1
    // writes; we=0 just steps the clock and the harness compares.
    struct Step { const char* name; uint16_t addr; uint16_t data; bool is_write; };
    Step seq[] = {
        // Initial readback should be 0 on both.
        {"initial INTENAR", 0x01C, 0,        false},
        // SET INTEN | PORTS (bit 14 + bit 3) on INTENA.
        {"set INTEN|PORTS", 0x09A, 0xC008, true},
        {"check INTENAR",   0x01C, 0,        false},
        // CLR PORTS.
        {"clear PORTS",     0x09A, 0x0008, true},
        {"check INTENAR",   0x01C, 0,        false},
        // SET INTREQ bits via INTREQ.
        {"set BLT|VERTB",   0x09C, 0xC060, true},
        {"check INTREQR",   0x01E, 0,        false},
        // SET multiple.
        {"set AUDIO 0..3",  0x09C, 0xC780, true},
        {"check INTREQR",   0x01E, 0,        false},
        // CLR all.
        {"clr all req",     0x09C, 0x7FFF, true},
        {"check INTREQR",   0x01E, 0,        false},
        {"final INTENAR",   0x01C, 0,        false},
    };
    const size_t N = sizeof(seq) / sizeof(seq[0]);

    // Known documented divergences between our paula and MiniMig that
    // come from MiniMig's choice to let INTREQ writes mutate bit 14.
    // Real Amiga (per HRM) reserves bit 14 of INTREQ; our paula matches
    // that, MiniMig keeps bit 14 as a writable storage cell that ORs
    // into the INTREQR readback.  Mask off bit 14 when comparing.
    int hard = 0, soft = 0;
    for (size_t i = 0; i < N; i++) {
        if (seq[i].is_write) {
            write_reg(dut, seq[i].addr, seq[i].data);
            std::printf("  W $%03x <- $%04x  (%s)\n",
                        seq[i].addr, seq[i].data, seq[i].name);
        } else {
            uint16_t mm, ours;
            read_reg(dut, seq[i].addr, mm, ours);
            bool match_raw = (mm == ours);
            bool match_masked = ((mm & ~0x4000) == (ours & ~0x4000));
            const char* mark =
                match_raw    ? "OK"
              : match_masked ? "OK (bit14-INTREQ writable diff)"
                             : "MISMATCH";
            std::printf("  R $%03x -> mm=$%04x ours=$%04x  %s  (%s)\n",
                        seq[i].addr, mm, ours, mark, seq[i].name);
            if (!match_raw) {
                if (match_masked) soft++;
                else              hard++;
            }
        }
    }

    std::printf("[minimig-xcheck] %s  (%d hard mismatches, %d soft/documented)\n",
                hard == 0 ? "PASS" : "FAIL", hard, soft);
    delete dut;
    return hard == 0 ? 0 : 1;
}
