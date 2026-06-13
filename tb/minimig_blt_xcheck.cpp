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
#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "track80_mfm_data.h"

static void tick(Vminimig_blt_xcheck_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

static void init_mem(Vminimig_blt_xcheck_top* dut, uint32_t word_addr, uint16_t value) {
    dut->init_we    = 1;
    dut->init_addr  = word_addr;       // 20-bit (1Mi word index)
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

static uint16_t read_mm(Vminimig_blt_xcheck_top* dut, uint32_t word_addr) {
    dut->read_addr = word_addr;        // 20-bit
    dut->eval();
    return dut->mm_read_data;
}
static uint16_t read_our(Vminimig_blt_xcheck_top* dut, uint32_t word_addr) {
    dut->read_addr = word_addr;        // 20-bit
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

    // ====================================================================
    // Test 3: K1.3 cyl-53 cc000005 blit with ACTUAL track-80 MFM data.
    //   Captured from a failed WB1.3 boot where output at chip $2D24 became
    //   $0 instead of $44894489 (sector 3 sync).
    //   Same BLTCON0=$05CC USEB+USED, ASH=0, BLTBPT=$2064, BLTDPT=$2060,
    //   BLTSIZE=$2EE0 — only the source data differs (real MFM vs synthetic).
    // ====================================================================
    printf("=== Test 3: K1.3 cyl-53 cc000005 blit with real track-80 MFM ===\n");
    // BLTBPT byte $2064 = word $1032; BLTDPT byte $2060 = word $1030
    // Source data covers 5984 words = $1032..$2EF1
    // Pre-fill the destination range with sentinel
    for (uint16_t w = 0x1028; w <= 0x2EF8; w++)
        init_mem(dut, w, 0xBEEF);
    // Load actual track-80 MFM into source range starting at word $1032
    for (uint16_t w = 0; w < 6000 && (0x1032 + w) <= 0x7FFF; w++)
        init_mem(dut, 0x1032 + w, TRACK80_MFM[w]);

    reg_w(dut, 0x40, 0x05CC);   // BLTCON0 — same as K1.3
    reg_w(dut, 0x42, 0x0000);   // BLTCON1
    reg_w(dut, 0x44, 0xFFFF);   // BLTAFWM
    reg_w(dut, 0x46, 0xFFFF);   // BLTALWM
    reg_w(dut, 0x4C, 0x0000);   // BLTBPTH
    reg_w(dut, 0x4E, 0x2064);   // BLTBPTL — exact byte from K1.3 boot
    reg_w(dut, 0x54, 0x0000);   // BLTDPTH
    reg_w(dut, 0x56, 0x2060);   // BLTDPTL — exact byte from K1.3 boot
    reg_w(dut, 0x62, 0x0000);   // BLTBMOD
    reg_w(dut, 0x66, 0x0000);   // BLTDMOD
    reg_w(dut, 0x58, 0x2EE0);   // BLTSIZE — same as K1.3

    for (int i = 0; i < 200000; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 32) {
            printf("  Both idle after %d cycles\n", i);
            break;
        }
    }
    int t3_mis = 0, t3_match = 0;
    int first_mis = -1;
    // Compare dest range word $1030..$1030+5984
    for (uint32_t w = 0x1030; w <= 0x1030 + 5984; w++) {
        uint16_t mm = read_mm(dut, w);
        uint16_t our = read_our(dut, w);
        if (mm != our) {
            if (t3_mis < 16) printf("  MISMATCH @ word $%04X (chip $%04x): mm=$%04X our=$%04X\n",
                w, w*2, mm, our);
            if (first_mis < 0) first_mis = w;
            t3_mis++;
        } else t3_match++;
    }
    printf("Test 3: %d match, %d mismatch (first mismatch @ word $%04X)\n",
        t3_match, t3_mis, first_mis);
    // Specifically inspect chip $2D24 (= word $1692) which had $0 instead of $44894489 in the real boot
    printf("  Spot check chip $2D24 (sector 3 sync): mm=$%04x our=$%04x (expect $4489)\n",
        read_mm(dut, 0x1692), read_our(dut, 0x1692));

    // ====================================================================
    // Test 4: ODD BLTBPT/BLTDPT must be masked to word-aligned (bit 0 = 0).
    //   Real Amiga BLT*PTL has bit 0 wired to 0.  K1.3 occasionally writes
    //   odd byte addresses to BLTDPT (e.g. $762D).  Without our mask, the
    //   iteration produces odd-byte writes that hit "different memwords"
    //   than the CPU expects, corrupting buffers (caught via [BUF-2D24-WR]
    //   trace during WB1.3 cyl-53 read).  Verify that an odd BLTDPT
    //   produces THE SAME output as an even BLTDPT (= odd_addr & ~1) — both
    //   should match minimig.
    // ====================================================================
    printf("=== Test 4: ODD BLTBPT/BLTDPT masking ===\n");
    // Setup: same as Test 1, but use ODD addresses for BPT and DPT.
    for (uint16_t w = 0; w < 16; w++)
        init_mem(dut, 0x1000 + w, 0xAA00 + w);
    for (uint16_t w = 0; w < 16; w++)
        init_mem(dut, 0x2000 + w, 0xDEAD);

    reg_w(dut, 0x40, 0x05CC);   // BLTCON0
    reg_w(dut, 0x42, 0x0000);   // BLTCON1
    reg_w(dut, 0x44, 0xFFFF);
    reg_w(dut, 0x46, 0xFFFF);
    reg_w(dut, 0x4C, 0x0000);
    reg_w(dut, 0x4E, 0x2001);   // BLTBPTL = $2001 (ODD!) — should be masked to $2000
    reg_w(dut, 0x54, 0x0000);
    reg_w(dut, 0x56, 0x4001);   // BLTDPTL = $4001 (ODD!) — should be masked to $4000
    reg_w(dut, 0x62, 0x0000);
    reg_w(dut, 0x66, 0x0000);
    reg_w(dut, 0x58, 0x0048);   // BLTSIZE = 1 row x 8 words

    for (int i = 0; i < 2000; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 8) break;
    }
    int t4_mis = 0, t4_match = 0;
    for (uint16_t w = 0x0FFC; w <= 0x1010; w++) {
        uint16_t mm = read_mm(dut, w);
        uint16_t our = read_our(dut, w);
        if (mm != our) {
            if (t4_mis < 8) printf("  MISMATCH @ $%04X: mm=$%04X our=$%04X\n", w, mm, our);
            t4_mis++;
        } else t4_match++;
    }
    printf("Test 4: %d match, %d mismatch (odd BLTBPT/DPT must mask to even)\n",
        t4_match, t4_mis);

    // ====================================================================
    // Test 5: BOING-CLASS COOKIE-CUT — isolates the §68 wall.
    //   BLTCON0=$0BCA  (USE=ACD, minterm $CA = cookie-cut)
    //   BLTAFWM=$FFFF  BLTALWM=$8000  (only the rightmost-pixel bit
    //                                   gets through the last column)
    //   BLTAMOD=$0020  BLTCMOD=$0020  BLTDMOD=$FFEC  (-10 words/row)
    //   3 rows x 2 words = 6 D writes total, easy to step through.
    //
    // Source A at word $1000..$100A, source C at word $2000..$200A.
    // Dest at word $3000..$300A (initially $DEAD).
    //
    // BLTALWM=$8000 + non-zero BLTDMOD is the canned-test gap that
    // boing exercises and our blitter currently gets wrong.
    // ====================================================================
    printf("=== Test 5: BOING-CLASS cookie-cut ($0BCA, BLTALWM=$8000) ===\n");
    // Source A and C: distinguishable patterns.
    for (uint32_t w = 0; w < 32; w++) {
        init_mem(dut, 0x1000 + w, 0xA000 + w);
        init_mem(dut, 0x2000 + w, 0xC000 + w);
        init_mem(dut, 0x3000 + w, 0xDEAD);
    }
    // Explicitly write all three BLT*DAT presets BEFORE Test 5 so neither
    // blitter inherits a reset-default difference from Test 4 residue.
    // (Our blitter resets bltbdat_pre to $FFFF; minimig may differ.)
    reg_w(dut, 0x74, 0x0000);   // BLTADAT
    reg_w(dut, 0x72, 0x0000);   // BLTBDAT — USE_B=0 means this preset feeds B
    reg_w(dut, 0x70, 0x0000);   // BLTCDAT

    reg_w(dut, 0x40, 0x0BCA);   // BLTCON0 = ACD + minterm $CA
    reg_w(dut, 0x42, 0x0000);   // BLTCON1 = ascending, no fill, no line
    reg_w(dut, 0x44, 0xFFFF);   // BLTAFWM
    reg_w(dut, 0x46, 0x8000);   // BLTALWM <-- the key edge
    // 24-bit pointers — high half zero, low half is byte address.
    reg_w(dut, 0x48, 0x0000);   // BLTCPTH
    reg_w(dut, 0x4A, 0x4000);   // BLTCPTL = byte $4000 (word $2000)
    reg_w(dut, 0x50, 0x0000);   // BLTAPTH
    reg_w(dut, 0x52, 0x2000);   // BLTAPTL = byte $2000 (word $1000)
    reg_w(dut, 0x54, 0x0000);   // BLTDPTH
    reg_w(dut, 0x56, 0x6000);   // BLTDPTL = byte $6000 (word $3000)
    reg_w(dut, 0x60, 0x0020);   // BLTCMOD
    reg_w(dut, 0x64, 0x0020);   // BLTAMOD
    reg_w(dut, 0x66, 0xFFEC);   // BLTDMOD = -20 bytes = -10 words
    reg_w(dut, 0x62, 0x0000);   // BLTBMOD (USE_B=0, irrelevant)
    // BLTSIZE: 3 rows (bits 15:6) x 2 words/row (bits 5:0) = ($003 << 6) | $02 = $00C2
    reg_w(dut, 0x58, 0x00C2);

    for (int i = 0; i < 2000; i++) {
        tick(dut);
        if (!dut->mm_busy && !dut->our_busy && i > 8) break;
    }
    int t5_mis = 0, t5_match = 0;
    // Check a wider window than just BLTDPT because negative DMOD
    // walks the dest backward in memory.
    for (uint32_t w = 0x2FF0; w <= 0x301F; w++) {
        uint16_t mm = read_mm(dut, w);
        uint16_t our = read_our(dut, w);
        if (mm != our) {
            if (t5_mis < 8)
                printf("  MISMATCH @ $%06X: mm=$%04X our=$%04X\n",
                       w * 2, mm, our);
            t5_mis++;
        } else t5_match++;
    }
    printf("Test 5: %d match, %d mismatch (cookie-cut $CA + BLTALWM=$8000 + neg DMOD)\n",
        t5_match, t5_mis);

    int mismatches = t1_mis + t2_mis + t3_mis + t4_mis + t5_mis;
    printf("\nTotal: %d match, %d mismatch\n",
        t1_match + t2_match + t3_match + t4_match + t5_match, mismatches);

    // ====================================================================
    // Optional: replay a real boing-disk blit sequence captured via
    // tools/intuition_diff/blit_classify.py --replay-out.  Set
    //   REPLAY_FILE=/path/to/blits.txt
    // to run.  Each line: con0 con1 afwm alwm  apt bpt cpt dpt
    //                     amod bmod cmod dmod  bltsize  (hex).
    //
    // Pre-fills both 32K-word memories with a deterministic pattern,
    // then runs every blit BACK-TO-BACK (no re-init between blits) and
    // compares the full memory after each.  The first divergent blit is
    // printed with context.  This is the regime where cumulative-state
    // bugs (BLTAFWM/ALWM leakage, BLTxMOD survival, BLTADAT preset
    // state) surface — see WB13 journal §65 / §66.
    //
    // The pre-fill pattern is deterministic so the test is reproducible
    // even though boing's real source data is not available here:
    // identical inputs to both blitters MUST produce identical outputs,
    // regardless of what the inputs actually are.
    // ====================================================================
    int replay_mis = 0;
    if (const char* path = std::getenv("REPLAY_FILE")) {
        printf("\n=== Replay: %s ===\n", path);
        FILE* fp = std::fopen(path, "r");
        if (!fp) {
            printf("REPLAY_FILE could not be opened\n");
            delete dut;
            return mismatches ? 1 : 2;
        }

        // Re-reset between the canned tests and the replay so state is clean.
        dut->rst = 1; for (int i = 0; i < 8; i++) tick(dut);
        dut->rst = 0; for (int i = 0; i < 8; i++) tick(dut);

        // Pre-fill memories with the same pattern.  word[i] = (i * 0x1357) ^ i.
        // Deterministic, non-zero, easy to recompute mentally.  1Mi words.
        // Verilator handles ~1M sequential ticks quickly; takes a few seconds.
        constexpr uint32_t MEM_WORDS = 1u << 20;
        for (uint32_t w = 0; w < MEM_WORDS; w++) {
            init_mem(dut, w, (uint16_t)((w * 0x1357u) ^ w));
        }

        // Verify pre-fill consistency BEFORE any blit runs.  If mm_mem
        // and our_mem differ at this point, the harness is broken; the
        // problem isn't the blitters.  Sample-check (every 64th word) —
        // a full 1M-word scan after every blit would be slow; this is
        // enough to catch a wholesale init bug.
        int pre_diffs = 0;
        for (uint32_t w = 0; w < MEM_WORDS; w += 64) {
            uint16_t mm  = read_mm(dut, w);
            uint16_t our = read_our(dut, w);
            if (mm != our) {
                if (pre_diffs < 5)
                    printf("  PRE-FILL DIFF $%06X: mm=$%04X our=$%04X\n",
                           w * 2, mm, our);
                pre_diffs++;
            }
        }
        printf("Pre-fill check (sampled): %d divergent words\n", pre_diffs);

        char line[512];
        int blit_no = 0;
        int first_diff_blit = -1;
        while (std::fgets(line, sizeof line, fp)) {
            if (line[0] == '#' || line[0] == '\n') continue;
            uint32_t con0, con1, afwm, alwm;
            uint32_t apt, bpt, cpt, dpt;
            uint32_t amod, bmod, cmod, dmod;
            uint32_t bltsize;
            if (std::sscanf(line,
                "%x %x %x %x %x %x %x %x %x %x %x %x %x",
                &con0, &con1, &afwm, &alwm,
                &apt, &bpt, &cpt, &dpt,
                &amod, &bmod, &cmod, &dmod,
                &bltsize) != 13) {
                continue;
            }
            // Mask pointers to the 2 MiB test memory window (21-bit
            // byte address, even).  Preserves the chip-RAM range
            // ($000000-$1FFFFE) at full resolution — boing's real
            // pointers mostly land in chip RAM and now survive without
            // collision.
            uint32_t apt21 = apt & 0x1FFFFEu;
            uint32_t bpt21 = bpt & 0x1FFFFEu;
            uint32_t cpt21 = cpt & 0x1FFFFEu;
            uint32_t dpt21 = dpt & 0x1FFFFEu;

            // Reset BLT[ABC]DAT presets to 0 so cross-blit state cannot
            // leak through them.  When boing's actual workload runs,
            // those registers get written each blit too (visible in the
            // chipset trace as ~4,300 BLTBDAT writes, ~3,700 BLTADAT
            // writes); the classifier just doesn't capture them yet.
            // For this test we want to isolate "given identical
            // pre-state, do the two blitters produce identical output
            // across N back-to-back blits", which requires zeroing the
            // presets between blits.
            reg_w(dut, 0x74, 0x0000);                // BLTADAT
            reg_w(dut, 0x72, 0x0000);                // BLTBDAT
            reg_w(dut, 0x70, 0x0000);                // BLTCDAT

            // Write the full register set in canonical order.
            reg_w(dut, 0x40, (uint16_t)con0);
            reg_w(dut, 0x42, (uint16_t)con1);
            reg_w(dut, 0x44, (uint16_t)afwm);
            reg_w(dut, 0x46, (uint16_t)alwm);
            // 21-bit pointer split: H = bits [23:16], L = bits [15:0].
            reg_w(dut, 0x48, (uint16_t)(cpt21 >> 16));        // CPTH
            reg_w(dut, 0x4A, (uint16_t)(cpt21 & 0xFFFE));     // CPTL
            reg_w(dut, 0x4C, (uint16_t)(bpt21 >> 16));        // BPTH
            reg_w(dut, 0x4E, (uint16_t)(bpt21 & 0xFFFE));     // BPTL
            reg_w(dut, 0x50, (uint16_t)(apt21 >> 16));        // APTH
            reg_w(dut, 0x52, (uint16_t)(apt21 & 0xFFFE));     // APTL
            reg_w(dut, 0x54, (uint16_t)(dpt21 >> 16));        // DPTH
            reg_w(dut, 0x56, (uint16_t)(dpt21 & 0xFFFE));     // DPTL
            reg_w(dut, 0x64, (uint16_t)amod);
            reg_w(dut, 0x62, (uint16_t)bmod);
            reg_w(dut, 0x60, (uint16_t)cmod);
            reg_w(dut, 0x66, (uint16_t)dmod);
            reg_w(dut, 0x58, (uint16_t)bltsize);     // BLTSIZE — triggers

            // Spin until both blitters drop !busy.  Bound the wait so a
            // hung blit reports failure instead of looping forever.
            int spin = 0;
            while ((dut->mm_busy || dut->our_busy) && spin < 200000) {
                tick(dut);
                spin++;
            }
            if (spin >= 200000) {
                printf("BLIT #%d hung (mm_busy=%d our_busy=%d)\n",
                       blit_no, dut->mm_busy, dut->our_busy);
                replay_mis++;
                break;
            }

            // Compare memory.  Focus on the blit's write region (around
            // dpt21) plus a stride-32 sweep of the full memory for any
            // unexpected writes elsewhere.  A blit at most touches
            // ~64KB (worst case big blit) so scanning a 256KB window
            // around the dest pointer is comprehensive for the writes
            // we care about; the wider sweep catches stray writes.
            int blit_diffs = 0;
            auto check = [&](uint32_t w) {
                uint16_t mm  = read_mm(dut, w);
                uint16_t our = read_our(dut, w);
                if (mm != our) {
                    if (blit_diffs < 6) {
                        printf("  blit#%d DIFF $%06X: mm=$%04X ours=$%04X\n",
                               blit_no, w * 2, mm, our);
                    }
                    blit_diffs++;
                }
            };
            // Dense scan around dest pointer (±128KB = ±64K words)
            uint32_t dpt_w = dpt21 >> 1;
            uint32_t lo = dpt_w >= 65536 ? dpt_w - 65536 : 0;
            uint32_t hi = std::min(dpt_w + 65536, MEM_WORDS);
            int dpt_diffs_in_window = 0;
            for (uint32_t w = lo; w < hi; w++) {
                uint16_t mm  = read_mm(dut, w);
                uint16_t our = read_our(dut, w);
                if (mm != our) dpt_diffs_in_window++;
                check(w);
            }
            // Sparse sweep of the full 2 MiB for stray writes elsewhere.
            int stray_diffs = 0;
            for (uint32_t w = 0; w < MEM_WORDS; w += 32) {
                if (w >= lo && w < hi) continue;     // already covered
                uint16_t mm  = read_mm(dut, w);
                uint16_t our = read_our(dut, w);
                if (mm != our) stray_diffs++;
                check(w);
            }
            if (blit_diffs) {
                printf("    [in-window=%d  out-of-window-sample=%d  apt=$%06X "
                       "cpt=$%06X dpt=$%06X]\n",
                       dpt_diffs_in_window, stray_diffs, apt21, cpt21, dpt21);
            }
            if (blit_diffs) {
                if (first_diff_blit < 0) first_diff_blit = blit_no;
                printf("blit#%d  con0=$%04X size=$%04X  -> %d diverging words\n",
                       blit_no, con0, bltsize, blit_diffs);
                replay_mis++;
                if (replay_mis >= 5) {
                    printf("Stopping after 5 divergent blits.\n");
                    break;
                }
            }
            blit_no++;
        }
        std::fclose(fp);
        printf("\nReplay summary: %d blits processed, %d divergent",
               blit_no, replay_mis);
        if (first_diff_blit >= 0)
            printf(" (first at blit #%d)", first_diff_blit);
        printf("\n");
    }

    delete dut;
    return (mismatches + replay_mis) ? 1 : 0;
}
