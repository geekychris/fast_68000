// MiniMig blitter vs our blitter cross-validation harness.
//
// Drives both blitters with identical register sequences (BLTCON0/1,
// pointers, modulos, BLTSIZE), then ticks the clock until both
// signal !busy.  Reads back memory from each and prints any
// mismatches.
//
// Exits 0 on full match, non-zero on mismatch.

#include "Vminimig_blt_xcheck_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

static void tick(Vminimig_blt_xcheck_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

static void init_mem(Vminimig_blt_xcheck_top* dut, uint16_t word_addr, uint16_t value) {
    dut->init_we    = 1;
    dut->init_addr  = word_addr;
    dut->init_wdata = value;
    tick(dut);
    dut->init_we    = 0;
}

static void reg_w(Vminimig_blt_xcheck_top* dut, uint8_t reg, uint16_t val) {
    dut->reg_we    = 1;
    dut->reg_addr  = reg;
    dut->reg_wdata = val;
    tick(dut);
    dut->reg_we    = 0;
}

static uint16_t read_mm(Vminimig_blt_xcheck_top* dut, uint16_t word_addr) {
    dut->read_addr = word_addr;
    dut->eval();
    return dut->mm_read_data;
}
static uint16_t read_our(Vminimig_blt_xcheck_top* dut, uint16_t word_addr) {
    dut->read_addr = word_addr;
    dut->eval();
    return dut->our_read_data;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vminimig_blt_xcheck_top;

    // Reset.
    dut->rst = 1; dut->clk = 0;
    dut->reg_we = 0; dut->init_we = 0;
    dut->reg_addr = 0; dut->reg_wdata = 0;
    dut->init_addr = 0; dut->init_wdata = 0;
    dut->read_addr = 0;
    for (int i = 0; i < 8; i++) tick(dut);
    dut->rst = 0;
    for (int i = 0; i < 8; i++) tick(dut);

    // -------- Pre-load memory --------
    // Set up a simple test pattern: word $1000 + i at addresses $1000..$100F.
    // This is the "source" area for channel B.
    for (int i = 0; i < 16; i++) {
        init_mem(dut, 0x1000 + i, 0x1000 + i);
    }
    // Pre-fill dest area with $A5A5 to detect leaks.
    for (int i = 0; i < 16; i++) {
        init_mem(dut, 0x0FFC + i, 0xA5A5);
    }

    // -------- Program blitter --------
    // Simple "shift B by -4 bytes into D" operation:
    //   BLTCON0 = $0500 = USEB|USED, LF=$00 (output = always 0), ASH=0
    //   Wait — $00 LF gives all zeros.  Use LF=$CC (output = B).
    //   BLTCON0 = $05CC.
    //   BLTCON1 = $0000.
    //   BLTBPT = $2000 (= word $1000).
    //   BLTDPT = $1FFC (= word $0FFE -- shifted -4 bytes).
    //   BLTSIZE: 1 row x 8 words = (1<<6)|8 = $48.
    //
    // Expected: dst[0FFE..1005] = src[1000..1007] (8 words copied with
    // -4 byte offset).
    reg_w(dut, 0x40, 0x05CC);   // BLTCON0
    reg_w(dut, 0x42, 0x0000);   // BLTCON1
    reg_w(dut, 0x44, 0xFFFF);   // BLTAFWM
    reg_w(dut, 0x46, 0xFFFF);   // BLTALWM
    reg_w(dut, 0x4C, 0x0000);   // BLTBPTH
    reg_w(dut, 0x4E, 0x2000);   // BLTBPTL
    reg_w(dut, 0x54, 0x0000);   // BLTDPTH
    reg_w(dut, 0x56, 0x1FFC);   // BLTDPTL
    reg_w(dut, 0x62, 0x0000);   // BLTBMOD
    reg_w(dut, 0x66, 0x0000);   // BLTDMOD
    reg_w(dut, 0x58, 0x0048);   // BLTSIZE (height=1, width=8) -- triggers!

    // -------- Run until both done --------
    int max_cycles = 2000;
    for (int i = 0; i < max_cycles; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 8) {
            printf("Both blitters idle after %d cycles\n", i);
            break;
        }
    }

    // -------- Compare memory --------
    int mismatches = 0;
    int matches = 0;
    // Check the dst region + a bit of margin.
    for (uint16_t w = 0x0FFC; w <= 0x1010; w++) {
        uint16_t mm = read_mm(dut, w);
        uint16_t our = read_our(dut, w);
        if (mm != our) {
            if (mismatches < 16)
                printf("MISMATCH at word $%04X: mm=$%04X our=$%04X\n",
                       w, mm, our);
            mismatches++;
        } else {
            matches++;
        }
    }

    printf("Match: %d, Mismatch: %d\n", matches, mismatches);
    delete dut;
    return mismatches ? 1 : 0;
}
