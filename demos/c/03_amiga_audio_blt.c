/* 03_amiga_audio_blt.c — two cores drive the Amiga chipset in parallel.
 *
 * Core 0 programs Paula's audio channel 0 (sample buffer + length +
 * period + volume + DMA enable), then reads back AUD0LC to make sure
 * the register file accepted the write.
 *
 * Core 1 sets up a copy-mode blitter fill of a small rectangle in
 * memory.  When it polls BLTSTAT and sees BUSY=0, it scans the
 * destination to confirm every word matches the fill pattern.
 *
 * Halt codes:
 *   $0000 = both cores OK
 *   $BAD3 = core 0: AUD0LC readback mismatch
 *   $BAD4 = core 1: blit destination did not contain the fill
 *
 * Pure side-effect demo: no console hex print, so we don't depend on
 * any of the more fiddly io.h helpers.  A one-line status banner per
 * core lands in the console for human read-back.
 */
#include "sync.h"
#include "io.h"

#define PAU_BASE          0x00DFF000u
#define AUDENA            (*(volatile uint32_t *)(PAU_BASE + 0x000))
#define AUD0LC            (*(volatile uint32_t *)(PAU_BASE + 0x0A0))
#define AUD0LEN           (*(volatile uint32_t *)(PAU_BASE + 0x0A4))
#define AUD0PER           (*(volatile uint32_t *)(PAU_BASE + 0x0A6))
#define AUD0VOL           (*(volatile uint32_t *)(PAU_BASE + 0x0A8))

#define BLT_BASE          0x00FE0000u
#define BLTCON            (*(volatile uint32_t *)(BLT_BASE + 0x00))
#define BLTAFWM           (*(volatile uint32_t *)(BLT_BASE + 0x04))
#define BLTALWM           (*(volatile uint32_t *)(BLT_BASE + 0x08))
#define BLTAPTH           (*(volatile uint32_t *)(BLT_BASE + 0x0C))
#define BLTDPTH           (*(volatile uint32_t *)(BLT_BASE + 0x18))
#define BLTAMOD           (*(volatile uint32_t *)(BLT_BASE + 0x1C))
#define BLTDMOD           (*(volatile uint32_t *)(BLT_BASE + 0x28))
#define BLTSIZE           (*(volatile uint32_t *)(BLT_BASE + 0x38))
#define BLTSTAT           (*(volatile uint32_t *)(BLT_BASE + 0x3C))

#define AUDIO_BUF_ADDR    0x00010000u
#define AUDIO_BUF_WORDS   16u
#define BLT_DEST_ADDR     0x00014000u
#define BLT_FILL          0x0000AA55u

static struct barrier g_bar = BARRIER_INIT;
static volatile uint32_t status[2];

int main(int core_)
{
    uint32_t core  = (uint32_t)core_;
    uint32_t sense = 0;

    if (core == 0) {
        volatile uint16_t *sample = (uint16_t *)AUDIO_BUF_ADDR;
        for (uint32_t i = 0; i < AUDIO_BUF_WORDS; i++)
            sample[i] = (i & 1) ? 0x4040 : 0xC0C0;

        AUD0LC  = AUDIO_BUF_ADDR;
        AUD0LEN = AUDIO_BUF_WORDS;
        AUD0PER = 0x100u;
        AUD0VOL = 0x40u;
        AUDENA  = 0x1u;

        uint32_t lc = AUD0LC;
        status[0] = (lc == AUDIO_BUF_ADDR) ? 0u : 0xBAD3u;
        puts_(status[0] == 0 ? "c0: paula OK" : "c0: paula FAIL");
    } else if (core == 1) {
        /* Build a 16-word source filled with the pattern, then copy
         * it to BLT_DEST_ADDR with a single-row blit (height=1, width=16). */
        volatile uint16_t *src = (uint16_t *)0x00012000u;
        for (uint32_t i = 0; i < 16u; i++) src[i] = (uint16_t)BLT_FILL;

        volatile uint32_t *dst = (uint32_t *)BLT_DEST_ADDR;
        for (uint32_t i = 0; i < 16u; i++) dst[i] = 0;

        BLTAFWM = 0x0000FFFFu;
        BLTALWM = 0x0000FFFFu;
        BLTAPTH = 0x00012000u;
        BLTDPTH = BLT_DEST_ADDR;
        BLTAMOD = 0;
        BLTDMOD = 0;
        BLTCON  = ((uint32_t)0xF0u << 24) | (1u << 3) | (1u << 0);
        /* height=1 in bits 21:6 = 0x40; width=16 in bits 5:0 = 0x10. */
        BLTSIZE = (1u << 6) | 16u;

        for (int t = 0; t < 10000; t++) {
            if ((BLTSTAT & 1u) == 0) break;
        }

        uint32_t bad = 0;
        volatile uint16_t *p = (uint16_t *)BLT_DEST_ADDR;
        for (uint32_t i = 0; i < 16u; i++) {
            if (p[i] != (uint16_t)BLT_FILL) { bad = 1; break; }
        }
        status[1] = bad ? 0xBAD4u : 0u;
        puts_(status[1] == 0 ? "c1: blitter OK" : "c1: blitter FAIL");
    }

    barrier_wait(&g_bar, 2u, &sense);

    /* Core 0 reduces the final halt code. */
    if (core == 0) {
        if (status[0] == 0xBAD3u) STOP_WITH(0xBAD3);
        if (status[1] == 0xBAD4u) STOP_WITH(0xBAD4);
    }
    return 0;
}
