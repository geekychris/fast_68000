/* 02_prodcons.c — single-producer / single-consumer lock-free ring.
 * Core 0 produces N items, core 1 consumes them and checks each one
 * is value*value (so we know data crossed the bus intact).  The ring
 * uses the canonical SPSC pattern from MULTICORE.md: producer owns
 * tail, consumer owns head, head/tail in separate cache words so a
 * store to one doesn't snoop-invalidate the other.
 *
 * Pass:   stop #0
 * Fail:   stop #$BAD2  (value mismatch)
 */
#include "sync.h"
#include "io.h"

#define N_ITEMS  64u
#define Q_LOG2   4u
#define Q_SIZE   (1u << Q_LOG2)
#define Q_MASK   (Q_SIZE - 1u)

struct queue {
    volatile uint32_t head;
    uint32_t          _pad0[7];
    volatile uint32_t tail;
    uint32_t          _pad1[7];
    volatile uint32_t slots[Q_SIZE];
};
static struct queue g_q;

static volatile uint32_t producer_done;

int main(int core_)
{
    uint32_t core = (uint32_t)core_;

    if (core == 0) {
        for (uint32_t i = 0; i < N_ITEMS; i++) {
            uint32_t v = i * i;
            /* spin until there's space */
            for (;;) {
                uint32_t t = g_q.tail;
                uint32_t next = (t + 1u) & Q_MASK;
                if (next != g_q.head) {
                    g_q.slots[t] = v;
                    g_q.tail = next;
                    break;
                }
                /* full; back off briefly */
                for (volatile int k = 0; k < 8; k++) {}
            }
        }
        producer_done = 1;
        print("producer done\n");
        return 0;
    }
    if (core == 1) {
        uint32_t expected = 0;
        uint32_t got = 0;
        while (got < N_ITEMS) {
            uint32_t h = g_q.head;
            if (h == g_q.tail) {
                if (producer_done && h == g_q.tail) continue;
                /* spin */
                for (volatile int k = 0; k < 8; k++) {}
                continue;
            }
            uint32_t v = g_q.slots[h];
            g_q.head = (h + 1u) & Q_MASK;
            if (v != expected * expected) STOP_WITH(0xBAD2);
            expected++;
            got++;
        }
        print("consumer got ");
        putu(got);
        putc('\n');
        return 0;
    }
    return 0;
}
