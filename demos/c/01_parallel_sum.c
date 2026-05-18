/* 01_parallel_sum.c — split summing of 1..N across all cores, then
 * core 0 reduces and checks the total.  Uses the sense-reversing
 * barrier from sync.h.
 *
 * Pass:   stop #0
 * Fail:   stop #$BAD1  (reduction mismatch)
 *
 * Tuning: ARRAY_LEN must be a multiple of NCORES.  ARRAY_LEN=512 with
 * NCORES=2 is small enough to fit alongside .text/.bss/console in the
 * default 64 KB build but big enough that the per-core sum dominates
 * coordination cost.
 */
#include "sync.h"
#include "io.h"

#ifndef NCORES
#define NCORES 2u
#endif
#define ARRAY_LEN 512u

/* Array lives in .data so all cores see the same initial values. */
static volatile uint32_t array[ARRAY_LEN];

/* Each core writes its partial into its own padded slot to avoid
 * false sharing on the 4-byte cache line.                          */
struct partial { volatile uint32_t value; uint32_t pad[3]; };
static struct partial partials[NCORES];

static struct barrier g_bar = BARRIER_INIT;

int main(int core_)
{
    uint32_t core = (uint32_t)core_;
    uint32_t sense = 0;

    /* Core 0 seeds the array (1..ARRAY_LEN).  Other cores wait at the
     * first barrier for this to publish.                              */
    if (core == 0) {
        for (uint32_t i = 0; i < ARRAY_LEN; i++) array[i] = i + 1u;
    }
    barrier_wait(&g_bar, NCORES, &sense);

    /* Compute partial sum of this core's slice. */
    uint32_t slice = ARRAY_LEN / NCORES;
    uint32_t base  = core * slice;
    uint32_t sum   = 0;
    for (uint32_t i = 0; i < slice; i++) sum += array[base + i];
    partials[core].value = sum;

    barrier_wait(&g_bar, NCORES, &sense);

    if (core == 0) {
        uint32_t total = 0;
        for (uint32_t i = 0; i < NCORES; i++) total += partials[i].value;
        uint32_t expected = (ARRAY_LEN * (ARRAY_LEN + 1u)) / 2u;
        print("parallel_sum: total=");
        putu(total);
        print(" expected=");
        putu(expected);
        putc('\n');
        if (total != expected) STOP_WITH(0xBAD1);
    }
    return 0;
}
