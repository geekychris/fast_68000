// Unit test for agnus_arbiter.v Phases B/C/D slot reservation.
//
// Verifies that under +define+SLOT_ACCURATE_AGNUS the arbiter denies
// grants on:
//   - Phase B: refresh cycles hpos = 0, 2, 4, 6 (every line)
//   - Phase C: disk cycles hpos = 7, 9, 11 (when dsk_active)
//   - Phase D: audio cycles hpos = 13 (AUD0EN), 15 (AUD1EN),
//                            17 (AUD2EN), 19 (AUD3EN)
//
// Also verifies that off-slot cycles still grant the requesting port
// via the underlying round-robin (Phases A/B+ behavior preserved).
//
// Exit 0 on full pass, nonzero on first mismatch.

#include "Vagnus_arbiter_xcheck_top.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>

static int failures = 0;

static void tick(Vagnus_arbiter_xcheck_top* dut) {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

// Drive inputs, settle combinational `winner_valid`, then snapshot.
// Phase E inputs default to "no bitplane DMA" so legacy tests keep
// working unmodified.
static void drive(Vagnus_arbiter_xcheck_top* dut,
                  uint16_t hpos, bool dsk_active, uint8_t audn_en,
                  uint8_t req, uint8_t lock) {
    dut->hpos       = hpos;
    dut->dsk_active = dsk_active ? 1 : 0;
    dut->audn_en    = audn_en & 0xF;
    dut->req        = req & 0xF;
    dut->lock       = lock & 0xF;
    dut->bpl_active = 0;
    dut->vpos       = 0;
    dut->bpu        = 0;
    dut->hires      = 0;
    dut->ddfstrt    = 0;
    dut->ddfstop    = 0;
    dut->eval();
}

// Drive with bitplane DMA active.
static void drive_bpl(Vagnus_arbiter_xcheck_top* dut,
                      uint16_t hpos, uint16_t vpos,
                      uint8_t bpu, bool hires,
                      uint8_t ddfstrt, uint8_t ddfstop,
                      uint8_t req) {
    dut->hpos       = hpos;
    dut->dsk_active = 0;
    dut->audn_en    = 0;
    dut->req        = req & 0xF;
    dut->lock       = 0;
    dut->bpl_active = 1;
    dut->vpos       = vpos;
    dut->bpu        = bpu;
    dut->hires      = hires ? 1 : 0;
    dut->ddfstrt    = ddfstrt;
    dut->ddfstop    = ddfstop;
    dut->eval();
}

static void check_eq(const char* label, int expected, int actual) {
    if (expected != actual) {
        printf("FAIL: %s — expected=%d actual=%d\n", label, expected, actual);
        failures++;
    }
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vagnus_arbiter_xcheck_top;

    // Reset
    dut->clk = 0; dut->rst_n = 0;
    dut->req = 0; dut->lock = 0; dut->hpos = 0;
    dut->dsk_active = 0; dut->audn_en = 0;
    dut->eval();
    tick(dut); tick(dut);
    dut->rst_n = 1;
    tick(dut);

    // ---------------- Phase B: refresh slot reservation ----------------
    // Port 0 (CPU) constantly requests.  At hpos in {0, 2, 4, 6}, the
    // arbiter must deny.  At all other early-line cycles (1, 3, 5, 7
    // when disk inactive), the request must be granted.
    printf("Phase B: refresh slots reject CPU req at hpos {0,2,4,6}\n");
    for (uint16_t h = 0; h < 8; h++) {
        drive(dut, h, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
        bool is_refresh = !(h & 1) && (h < 8);  // hpos in {0,2,4,6}
        int expected_valid = is_refresh ? 0 : 1;
        char lbl[64];
        snprintf(lbl, sizeof(lbl), "refresh hpos=%u valid", h);
        check_eq(lbl, expected_valid, (int)dut->winner_valid);
        if (expected_valid) {
            snprintf(lbl, sizeof(lbl), "refresh hpos=%u winner", h);
            check_eq(lbl, 0, (int)dut->winner);
        }
    }

    // ---------------- Phase C: disk slot reservation -------------------
    // With dsk_active=1, hpos in {7, 9, 11} must deny CPU even though
    // refresh isn't in effect there.  With dsk_active=0, those slots
    // are free.
    printf("Phase C: disk slots reject CPU req at hpos {7,9,11}\n");
    for (uint16_t h = 7; h <= 12; h++) {
        // Skip refresh hpos {2, 4, 6} ... actually 7..12 won't include them.
        drive(dut, h, /*dsk*/true, /*aud*/0, /*req*/0b0001, /*lock*/0);
        bool is_disk = (h == 7) || (h == 9) || (h == 11);
        int expected_valid = is_disk ? 0 : 1;
        char lbl[64];
        snprintf(lbl, sizeof(lbl), "disk_active hpos=%u valid", h);
        check_eq(lbl, expected_valid, (int)dut->winner_valid);
    }

    printf("Phase C: with dsk_active=0, hpos {7,9,11} are free\n");
    uint16_t disk_hposes[] = {7, 9, 11};
    for (uint16_t h : disk_hposes) {
        drive(dut, h, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
        char lbl[64];
        snprintf(lbl, sizeof(lbl), "disk_idle hpos=%u valid", h);
        check_eq(lbl, 1, (int)dut->winner_valid);
    }

    // ---------------- Phase D: audio slot reservation ------------------
    // With audn_en[N]=1, hpos = 13 + 2*N is denied to CPU.
    // With audn_en=0, those cycles are free.
    printf("Phase D: audio slots reject CPU req at hpos 13/15/17/19\n");
    struct AudioSlot { int hpos; uint8_t ch_bit; };
    AudioSlot slots[] = {{13, 0x1}, {15, 0x2}, {17, 0x4}, {19, 0x8}};
    for (auto& s : slots) {
        // Channel enabled → slot reserved
        drive(dut, s.hpos, /*dsk*/false, s.ch_bit, /*req*/0b0001, /*lock*/0);
        char lbl[64];
        snprintf(lbl, sizeof(lbl), "audio hpos=%d ch=$%X valid",
                 s.hpos, s.ch_bit);
        check_eq(lbl, 0, (int)dut->winner_valid);

        // Channel disabled → slot free
        drive(dut, s.hpos, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
        snprintf(lbl, sizeof(lbl), "audio_dis hpos=%d valid", s.hpos);
        check_eq(lbl, 1, (int)dut->winner_valid);

        // Other channel enabled → THIS slot is still free
        uint8_t other_ch = (s.ch_bit == 0x1) ? 0x2 : 0x1;
        drive(dut, s.hpos, /*dsk*/false, other_ch, /*req*/0b0001, /*lock*/0);
        snprintf(lbl, sizeof(lbl), "audio_otherCh hpos=%d valid", s.hpos);
        check_eq(lbl, 1, (int)dut->winner_valid);
    }

    // ---------------- Phase F (partial): ECLK slot --------------------
    // hpos = 224 ($E0) is reserved unconditionally for E-clock CIA
    // access.  Adjacent cycles (223, 225) must remain free.
    printf("Phase F: ECLK slot rejects CPU req at hpos 224 only\n");
    drive(dut, 224, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
    check_eq("eclk hpos=224 valid", 0, (int)dut->winner_valid);
    drive(dut, 223, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
    check_eq("eclk_adj hpos=223 valid", 1, (int)dut->winner_valid);
    drive(dut, 225, /*dsk*/false, /*aud*/0, /*req*/0b0001, /*lock*/0);
    check_eq("eclk_adj hpos=225 valid", 1, (int)dut->winner_valid);

    // ---------------- Free cycle still grants ------------------------
    // hpos = 100 (mid-line, no reservation) — CPU req must be granted.
    printf("Sanity: hpos=100 free cycle grants CPU\n");
    drive(dut, 100, /*dsk*/false, /*aud*/0xF, /*req*/0b0001, /*lock*/0);
    check_eq("free hpos=100 valid", 1, (int)dut->winner_valid);
    check_eq("free hpos=100 winner", 0, (int)dut->winner);

    // ---------------- Phase E (coarse): bitplane slot reservation -----
    // With bpl_active=1, bpu=1, ddfstrt=$38, ddfstop=$D0 (lowres
    // typical), vpos=100 (visible line), hpos in the fetch window:
    //   - Reserved cycles: hpos[2:0] == 4 (BPU=1 pattern)
    //   - Other even cycles: free
    //   - Odd cycles: free
    // Outside the fetch window (e.g. hpos=300), free regardless.
    printf("Phase E: BPU=1 lowres reserves hpos[2:0]==4 in fetch window\n");
    {
        const uint8_t ddfstrt = 0x38;
        const uint8_t ddfstop = 0xD0;
        // ddfstrt_h = $38<<1 = $70 = 112; ddfstop_h = $D0<<1 = $1A0 = 416.
        // Pick hpos=120 (in window).  Test hpos[2:0] = 0..7.
        for (int p = 0; p < 8; p++) {
            uint16_t h = 112 + p;
            drive_bpl(dut, h, /*vpos*/100, /*bpu*/1, /*hires*/false,
                      ddfstrt, ddfstop, /*req*/0b0001);
            bool reserved = (p == 4);
            int expected = reserved ? 0 : 1;
            char lbl[64];
            snprintf(lbl, sizeof(lbl), "bpu1 fetch hpos=%u (p=%d) valid",
                     h, p);
            check_eq(lbl, expected, (int)dut->winner_valid);
        }
        // Outside fetch window: always free.
        drive_bpl(dut, 60, 100, 1, false, ddfstrt, ddfstop, 0b0001);
        check_eq("bpu1 pre-fetch hpos=60 valid", 1,
                 (int)dut->winner_valid);
        // Non-visible line: always free.
        drive_bpl(dut, 116, 0, 1, false, ddfstrt, ddfstop, 0b0001);
        check_eq("bpu1 vblank vpos=0 valid", 1,
                 (int)dut->winner_valid);
    }

    // Phase E: BPU=2 reserves hpos[2:0] in {2, 6}; BPU=4 reserves all
    // even cycles (0, 2, 4, 6).
    printf("Phase E: BPU=4 lowres reserves all even cycles in window\n");
    for (int p = 0; p < 8; p++) {
        uint16_t h = 112 + p;
        drive_bpl(dut, h, 100, 4, false, 0x38, 0xD0, 0b0001);
        bool reserved = ((p & 1) == 0);  // even
        int expected = reserved ? 0 : 1;
        char lbl[64];
        snprintf(lbl, sizeof(lbl), "bpu4 fetch hpos=%u (p=%d) valid",
                 h, p);
        check_eq(lbl, expected, (int)dut->winner_valid);
    }

    // ---------------- Round-robin still works across free cycles ----
    // Drive two ports at hpos=100 — round-robin fairness implies the
    // winner alternates over consecutive clocks.
    printf("Round-robin: two ports alternate winning on free cycles\n");
    int wins[2] = {0, 0};
    for (int i = 0; i < 6; i++) {
        drive(dut, 100, false, 0, 0b0011, 0);  // ports 0+1 request
        wins[dut->winner & 1]++;
        tick(dut);
    }
    // Both ports should win at least once over 6 cycles.
    if (wins[0] == 0 || wins[1] == 0) {
        printf("FAIL: round-robin starved a port — wins=[%d,%d]\n",
               wins[0], wins[1]);
        failures++;
    }

    // ---------------- Final result -----------------------------------
    delete dut;
    if (failures == 0) {
        printf("PASS agnus_arbiter_xcheck (all slot reservation rules verified)\n");
        return 0;
    } else {
        printf("FAIL agnus_arbiter_xcheck (%d mismatches)\n", failures);
        return 1;
    }
}
