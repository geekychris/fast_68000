// MiniMig copper vs our copper cross-validation harness.
//
// Phase 1: pre-load both modules with the same Copper list, run for N
// clock cycles, then compare the sequence of MOVE-target chipset
// register offsets each side captured.
//
// Exits 0 on full match for all tests, non-zero on mismatch.

#include "Vminimig_cop_xcheck_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>

static void tick(Vminimig_cop_xcheck_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

static void init_mem(Vminimig_cop_xcheck_top* dut, uint16_t word_addr, uint16_t value) {
    dut->init_we    = 1;
    dut->init_addr  = word_addr;
    dut->init_wdata = value;
    tick(dut);
    dut->init_we    = 0;
}

// Build a Copper-list MOVE instruction: writes `value` to chipset
// register at offset `reg_off` (must be even).  IR1 = reg_off | 1<<0=0
// (MOVE form); IR2 = value.  Returns the two 16-bit words.
static void copper_move(uint16_t reg_off, uint16_t value,
                        uint16_t* ir1, uint16_t* ir2) {
    // IR1: bits [8:1] = reg_off >> 1.  Bit 0 = 0 (MOVE).
    *ir1 = (reg_off & 0x1FE);
    *ir2 = value;
}

// WAIT instruction: stall until raster reaches (VP, HP) with the given
// masks.  IR1 = (VP << 8) | (HP << 1) | 1; IR2 = (VPmask << 8) |
// (HPmask << 1) | 0.  We use VPmask=$7F, HPmask=$3F by default for a
// loose "wait until line VP" semantic.
static void copper_wait(uint8_t vp, uint8_t hp,
                        uint16_t* ir1, uint16_t* ir2) {
    *ir1 = ((uint16_t)vp << 8) | ((uint16_t)(hp & 0x7F) << 1) | 1;
    *ir2 = ((uint16_t)0x7F << 8) | ((uint16_t)(0x3F) << 1) | 0;
}

// SKIP instruction: if the raster has already passed (VP, HP), skip the
// next 4 bytes (one instruction).  Same encoding as WAIT but with the
// SKIP bit (IR2[0]) set.
static void copper_skip(uint8_t vp, uint8_t hp,
                        uint16_t* ir1, uint16_t* ir2) {
    *ir1 = ((uint16_t)vp << 8) | ((uint16_t)(hp & 0x7F) << 1) | 1;
    *ir2 = ((uint16_t)0x7F << 8) | ((uint16_t)(0x3F) << 1) | 1;
}

static void copper_wait_end(uint16_t* ir1, uint16_t* ir2) {
    // Unreachable end-of-list sentinel: IR1=$FFFF (WAIT/SKIP bit set),
    // IR2=$FFFE (WAIT — low bit clear, not SKIP).  Decodes as
    // WAIT(VP=$FF, HP=$7F, VP_mask=$7F, HP_mask=$3F): v_match needs
    // (vbeam & $7F) >= $FF, impossible since LHS max is $7F.
    *ir1 = 0xFFFF;
    *ir2 = 0xFFFE;
}

// Run a test: pre-load chip RAM with a Copper list at byte 0,
// kick both coppers, tick for `cycles`, then compare captured MOVEs.
struct CopperEntry {
    enum Kind { MOVE, WAIT, SKIP } kind = MOVE;
    uint16_t reg_off = 0;   // MOVE: chipset offset
    uint16_t value   = 0;   // MOVE: 16-bit value to write
    uint8_t  vp      = 0;   // WAIT/SKIP: vertical target
    uint8_t  hp      = 0;   // WAIT/SKIP: horizontal target
    bool     skipped = false; // MOVE: true if expected to be skipped by a preceding SKIP
    static CopperEntry mov(uint16_t r, uint16_t v) {
        CopperEntry e; e.kind = MOVE; e.reg_off = r; e.value = v; return e;
    }
    static CopperEntry mov_skipped(uint16_t r, uint16_t v) {
        CopperEntry e; e.kind = MOVE; e.reg_off = r; e.value = v;
        e.skipped = true; return e;
    }
    static CopperEntry wait(uint8_t v, uint8_t h) {
        CopperEntry e; e.kind = WAIT; e.vp = v; e.hp = h; return e;
    }
    static CopperEntry skip(uint8_t v, uint8_t h) {
        CopperEntry e; e.kind = SKIP; e.vp = v; e.hp = h; return e;
    }
};

static int run_test(Vminimig_cop_xcheck_top* dut,
                    const char* name,
                    const std::vector<CopperEntry>& list,
                    int max_cycles) {
    printf("=== %s ===\n", name);

    // Reset.  Hold long enough to flush any leftover state from a
    // previous test in this DUT instance.
    dut->rst = 1; dut->clk = 0;
    dut->init_we = 0; dut->cop1lc_we = 0;
    dut->vpos_in = 0; dut->hpos_in = 0; dut->sof_in = 0;
    dut->mm_pop = 0; dut->our_pop = 0;
    for (int i = 0; i < 64; i++) tick(dut);
    dut->rst = 0;
    for (int i = 0; i < 8; i++) tick(dut);

    // Pre-load chip RAM with the Copper list at word 0.
    uint16_t word = 0;
    for (auto& e : list) {
        uint16_t ir1, ir2;
        if (e.kind == CopperEntry::MOVE)
            copper_move(e.reg_off, e.value, &ir1, &ir2);
        else if (e.kind == CopperEntry::WAIT)
            copper_wait(e.vp, e.hp, &ir1, &ir2);
        else
            copper_skip(e.vp, e.hp, &ir1, &ir2);
        init_mem(dut, word++, ir1);
        init_mem(dut, word++, ir2);
    }
    // End-of-list sentinel.
    {
        uint16_t ir1, ir2;
        copper_wait_end(&ir1, &ir2);
        init_mem(dut, word++, ir1);
        init_mem(dut, word++, ir2);
    }

    // Program COP1LC = 0 in both coppers.
    dut->cop1lc_w  = 0;
    dut->cop1lc_we = 1;
    tick(dut);
    dut->cop1lc_we = 0;
    for (int i = 0; i < 4; i++) tick(dut);

    // Hold SOF high for several cycles.  Minimig's copjmp1 → strobe1 →
    // PC-reload chain needs several clk_ena (hpos[0]) cycles to settle.
    // A single-cycle pulse is sometimes enough on a fresh DUT but not
    // when there's stale state from a prior test in the same run.
    dut->sof_in = 1;
    for (int i = 0; i < 16; i++) {
        dut->hpos_in = i;  // ensure clk_ena toggles
        tick(dut);
    }
    dut->sof_in = 0;

    // Advance raster: hpos every cycle, vpos every 227 cycles.
    int hpos = 0, vpos = 0;
    for (int i = 0; i < max_cycles; i++) {
        dut->hpos_in = hpos;
        dut->vpos_in = vpos;
        tick(dut);
        hpos = (hpos + 1) % 227;
        if (hpos == 0) vpos = (vpos + 1) % 313;
    }

    // Drain captured MOVEs.
    std::vector<uint8_t> mm_seq, our_seq;
    while (dut->mm_count > 0) {
        mm_seq.push_back(dut->mm_head_reg);
        dut->mm_pop = 1; tick(dut); dut->mm_pop = 0;
        dut->eval();
    }
    while (dut->our_count > 0) {
        our_seq.push_back(dut->our_head_reg);
        dut->our_pop = 1; tick(dut); dut->our_pop = 0;
        dut->eval();
    }

    // Build the EXPECTED MOVE-target sequence (ignore WAIT/SKIP entries,
    // and exclude MOVEs marked as skipped — those are expected to be
    // consumed by a preceding SKIP).
    std::vector<uint8_t> expected;
    for (auto& e : list) {
        if (e.kind == CopperEntry::MOVE && !e.skipped)
            expected.push_back(e.reg_off & 0xFE);
    }
    printf("  Expected MOVE targets (from list):\n   ");
    for (auto b : expected) printf(" $%02X", b);
    printf("\n");
    printf("  Minimig captured (%zu):\n   ", mm_seq.size());
    for (auto b : mm_seq) printf(" $%02X", b);
    printf("\n");
    printf("  Ours captured    (%zu):\n   ", our_seq.size());
    for (auto b : our_seq) printf(" $%02X", b);
    printf("\n");

    // Compare expected sequence against each side.
    // We accept a PREFIX match: the harness may run longer than the
    // copper takes to halt on the sentinel, so trailing repeats from
    // restart cycles are tolerated.
    int mm_ok = 1, our_ok = 1;
    for (size_t i = 0; i < expected.size(); i++) {
        uint8_t want = expected[i];
        if (i >= mm_seq.size() || mm_seq[i] != want) {
            printf("  Minimig MISMATCH at %zu: want=$%02X got=%s\n",
                   i, want,
                   i >= mm_seq.size() ? "(none)" :
                   (std::string("$") +
                    std::string(1, "0123456789ABCDEF"[mm_seq[i]>>4]) +
                    std::string(1, "0123456789ABCDEF"[mm_seq[i]&0xF])).c_str());
            mm_ok = 0;
            break;
        }
        if (i >= our_seq.size() || our_seq[i] != want) {
            printf("  Ours MISMATCH at %zu: want=$%02X got=%s\n",
                   i, want,
                   i >= our_seq.size() ? "(none)" :
                   (std::string("$") +
                    std::string(1, "0123456789ABCDEF"[our_seq[i]>>4]) +
                    std::string(1, "0123456789ABCDEF"[our_seq[i]&0xF])).c_str());
            our_ok = 0;
            break;
        }
    }

    if (mm_ok && our_ok) {
        printf("  PASS\n");
        return 0;
    }
    printf("  FAIL: mm_ok=%d our_ok=%d\n", mm_ok, our_ok);
    return 1;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vminimig_cop_xcheck_top;
    int fails = 0;

    // -----------------------------------------------------------------
    // Test 1: minimal 3-MOVE list.  Write to BPLCON0/COLOR00/COLOR01.
    // No WAIT — just the end-of-list sentinel.
    // -----------------------------------------------------------------
    fails += run_test(dut, "Test 1: 3 MOVEs (BPLCON0, COLOR00, COLOR01)",
        {
            CopperEntry::mov(0x100, 0xA200),   // BPLCON0
            CopperEntry::mov(0x180, 0x0F0F),   // COLOR00
            CopperEntry::mov(0x182, 0x0FF0),   // COLOR01
        },
        4000);

    // -----------------------------------------------------------------
    // Test 2: longer list — 8 MOVEs to consecutive COLORxx registers.
    // -----------------------------------------------------------------
    {
        std::vector<CopperEntry> list;
        for (int i = 0; i < 8; i++)
            list.push_back(CopperEntry::mov(0x180 + 2*i, 0x100 * i));
        fails += run_test(dut, "Test 2: 8 MOVEs to COLOR00..COLOR07",
                          list, 8000);
    }

    // -----------------------------------------------------------------
    // Test 3: MOVEs gated by WAITs.  Verifies the WAIT instruction
    // pauses the copper until the raster reaches the target VP.
    //
    // The harness advances vpos every 227 cycles, so with 8000 cycles
    // vbeam reaches ~35.  WAIT VP=5 and VP=20 will both eventually
    // succeed and the post-WAIT MOVEs will fire.
    // -----------------------------------------------------------------
    fails += run_test(dut, "Test 3: MOVE / WAIT(VP=5) / MOVE / WAIT(VP=20) / MOVE",
        {
            CopperEntry::mov(0x100, 0xA200),  // BPLCON0
            CopperEntry::wait(5, 0),
            CopperEntry::mov(0x180, 0x0F00),  // COLOR00
            CopperEntry::wait(20, 0),
            CopperEntry::mov(0x182, 0x000F),  // COLOR01
        },
        8000);

    // -----------------------------------------------------------------
    // Test 4: SKIP that succeeds (always true with vp_target=0).  The
    // MOVE immediately after the SKIP is consumed (not executed).
    // Expected MOVE sequence: BPLCON0, COLOR01 (the middle COLOR00 is
    // skipped).
    // -----------------------------------------------------------------
    fails += run_test(dut, "Test 4: MOVE / SKIP(VP=0, always true) / MOVE_skipped / MOVE",
        {
            CopperEntry::mov(0x100, 0xA200),         // BPLCON0
            CopperEntry::skip(0, 0),
            CopperEntry::mov_skipped(0x180, 0x0F00), // COLOR00 — skipped
            CopperEntry::mov(0x182, 0x000F),         // COLOR01
        },
        4000);

    // -----------------------------------------------------------------
    // Test 5: SKIP that does NOT trigger (vp_target=$F0 > raster at
    // start).  All MOVEs execute, including the one after the SKIP.
    // Expected: BPLCON0, COLOR00, COLOR01.
    // -----------------------------------------------------------------
    fails += run_test(dut, "Test 5: MOVE / SKIP(VP=$F0, never satisfied early) / MOVE / MOVE",
        {
            CopperEntry::mov(0x100, 0xA200),  // BPLCON0
            CopperEntry::skip(0xF0, 0),       // raster hasn't reached $F0 yet
            CopperEntry::mov(0x180, 0x0F00),  // COLOR00 — should execute
            CopperEntry::mov(0x182, 0x000F),  // COLOR01
        },
        2000);  // short run, vbeam stays well below $F0

    delete dut;

    if (fails == 0) {
        printf("\nALL PASS\n");
        return 0;
    }
    printf("\n%d test(s) FAILED\n", fails);
    return 1;
}
