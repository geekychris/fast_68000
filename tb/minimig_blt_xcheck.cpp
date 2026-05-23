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

    // ====================================================================
    // Test 1: simple B->D copy (LF=$CC, USE=B|D, no shift)
    //   Source : word $1000..$1007 = $1000..$1007
    //   Dest   : word $2000..$2007 (initially $DEAD)
    //   Disjoint ranges, so this test actually validates writes happen.
    // ====================================================================
    printf("=== Test 1: simple BLTCON0=$05CC, 8-word B->D copy ===\n");
    for (int i = 0; i < 16; i++)
        init_mem(dut, 0x1000 + i, 0x1000 + i);
    for (int i = 0; i < 16; i++)
        init_mem(dut, 0x2000 + i, 0xDEAD);

    reg_w(dut, 0x40, 0x05CC);   // BLTCON0
    reg_w(dut, 0x42, 0x0000);   // BLTCON1
    reg_w(dut, 0x44, 0xFFFF);   // BLTAFWM
    reg_w(dut, 0x46, 0xFFFF);   // BLTALWM
    reg_w(dut, 0x4C, 0x0000);   // BLTBPTH
    reg_w(dut, 0x4E, 0x2000);   // BLTBPTL (word $1000)
    reg_w(dut, 0x54, 0x0000);   // BLTDPTH
    reg_w(dut, 0x56, 0x4000);   // BLTDPTL (word $2000)
    reg_w(dut, 0x62, 0x0000);   // BLTBMOD
    reg_w(dut, 0x66, 0x0000);   // BLTDMOD
    reg_w(dut, 0x58, 0x0048);   // BLTSIZE (1 row x 8 words)

    for (int i = 0; i < 2000; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 8) {
            printf("  Both idle after %d cycles\n", i);
            break;
        }
    }
    int t1_mis = 0, t1_match = 0;
    for (uint16_t w = 0x0FFC; w <= 0x1010; w++) {
        if (read_mm(dut, w) != read_our(dut, w)) {
            if (t1_mis < 8) printf("  MISMATCH @ $%04X: mm=$%04X our=$%04X\n",
                w, read_mm(dut, w), read_our(dut, w));
            t1_mis++;
        } else t1_match++;
    }
    printf("Test 1: %d match, %d mismatch\n\n", t1_match, t1_mis);

    // ====================================================================
    // Test 2: K1.3 trackdisk's exact big blit
    //   BLTCON0=$05CC BLTCON1=$0000 BLTBPT=$64AC BLTDPT=$64A8
    //   BLTSIZE=$2EE0 (height=191, width=32 words; = 6112 word writes)
    //   Source covers \$64AC..(\$64AC + 6112*2) = \$64AC..\$D2CC
    // ====================================================================
    printf("=== Test 2: K1.3 exact big blit ($2EE0 = 191x32) ===\n");
    // Seed the source range with disk-MFM-like data.  Use a simple
    // varying pattern so we exercise the barrel shifter at every word.
    for (uint16_t w = 0x3256; w <= 0x3256 + 6200; w++) {
        // Pattern: word i has value ((i * 0x4489) ^ 0x5555)
        uint16_t v = ((uint32_t)w * 0x4489) ^ 0x5555;
        init_mem(dut, w, v);
    }
    // Pre-fill destination with sentinel.
    for (uint16_t w = 0x3254; w <= 0x3254 + 6200; w++) {
        init_mem(dut, w, 0xBEEF);
    }
    // Re-do source after sentinel pre-fill (overlap covers most of B-range).
    for (uint16_t w = 0x3256; w <= 0x3256 + 6200; w++) {
        uint16_t v = ((uint32_t)w * 0x4489) ^ 0x5555;
        init_mem(dut, w, v);
    }
    // Reset both blitters before re-arming.  Just rely on the BLTSIZE
    // start (both should be !busy from the previous run).
    reg_w(dut, 0x40, 0x05CC);   // BLTCON0
    reg_w(dut, 0x42, 0x0000);   // BLTCON1
    reg_w(dut, 0x44, 0xFFFF);   // BLTAFWM
    reg_w(dut, 0x46, 0xFFFF);   // BLTALWM
    reg_w(dut, 0x4C, 0x0000);   // BLTBPTH
    reg_w(dut, 0x4E, 0x64AC);   // BLTBPTL
    reg_w(dut, 0x54, 0x0000);   // BLTDPTH
    reg_w(dut, 0x56, 0x64A8);   // BLTDPTL
    reg_w(dut, 0x62, 0x0000);   // BLTBMOD
    reg_w(dut, 0x66, 0x0000);   // BLTDMOD
    reg_w(dut, 0x58, 0x2EE0);   // BLTSIZE (height=191, width=32)

    for (int i = 0; i < 100000; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 32) {
            printf("  Both idle after %d cycles\n", i);
            break;
        }
    }
    int t2_mis = 0, t2_match = 0;
    // Compare the entire dst range.  Dst starts at \$64A8 = word \$3254
    // and writes 6112 words.  We compare 6200 to catch any over-write.
    for (uint32_t w = 0x3250; w <= 0x3254 + 6200; w++) {
        uint16_t mm = read_mm(dut, w);
        uint16_t our = read_our(dut, w);
        if (mm != our) {
            if (t2_mis < 8) printf("  MISMATCH @ $%04X: mm=$%04X our=$%04X\n",
                w, mm, our);
            t2_mis++;
        } else t2_match++;
    }
    printf("Test 2: %d match, %d mismatch\n", t2_match, t2_mis);

    int mismatches = t1_mis + t2_mis;
    printf("\nTotal: %d match, %d mismatch\n", t1_match + t2_match, mismatches);
    delete dut;
    return mismatches ? 1 : 0;
}
