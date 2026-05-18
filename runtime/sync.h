/* sync.h — TAS spinlock, sense-reversing barrier, IPI for fast_68000.
 *
 * Every primitive here is documented (with assembly equivalents and
 * rationale) in MULTICORE.md.  The headers exist so demos can spell
 * the same patterns in idiomatic C.
 */
#ifndef FAST68K_SYNC_H
#define FAST68K_SYNC_H

#include <stdint.h>

/* --------- core_id ---------------------------------------------- */
static inline uint32_t core_id(void)
{
    uint32_t sp;
    __asm__ volatile ("move.l %%sp,%0" : "=d"(sp));
    return (sp - 0x4000u) >> 12;
}

/* --------- halt with a literal immediate ------------------------ *
 * `stop` encodes its immediate in the instruction word so `imm`
 * MUST be a compile-time constant.  Test halt codes are always
 * literals so this is fine.
 */
#define STOP_WITH(imm) do {                              \
    __asm__ volatile ("1: stop %0\n bra 1b\n"            \
                      : : "i"((uint16_t)(imm)));         \
    __builtin_unreachable();                             \
} while (0)

/* --------- TAS spinlock ----------------------------------------- *
 * Lock byte is `0x00` = unlocked, `0x80` = locked.  TAS sets bit 7
 * atomically; we retry while the prior value was non-zero.
 */
typedef volatile uint8_t tas_lock_t;
#define TAS_LOCK_INIT 0

static inline void tas_lock(tas_lock_t *p)
{
    __asm__ volatile (
        "0: tas %0\n"
        "   bne 0b\n"
        : "+m"(*p) : : "cc"
    );
}

static inline void tas_unlock(tas_lock_t *p)
{
    *p = 0;
}

/* --------- sense-reversing barrier ------------------------------ */
struct barrier {
    volatile uint8_t  lock;
    uint8_t           _pad[3];
    volatile uint32_t count;
    volatile uint32_t sense;
};
#define BARRIER_INIT { 0, {0,0,0}, 0, 0 }

static inline void barrier_wait(struct barrier *b, uint32_t n,
                                uint32_t *my_sense)
{
    uint32_t s = *my_sense ^ 1u;
    *my_sense = s;
    tas_lock((tas_lock_t *)&b->lock);
    uint32_t c = b->count + 1;
    b->count = c;
    if (c == n) {
        b->count = 0;
        b->sense = s;
        tas_unlock((tas_lock_t *)&b->lock);
    } else {
        tas_unlock((tas_lock_t *)&b->lock);
        while (b->sense != s) { /* spin */ }
    }
}

/* --------- IRQ / IPI register at $FFFF_FFFC --------------------- */
static volatile uint32_t * const IRQ_REG = (uint32_t *)0xFFFFFFFCu;

static inline void irq_post(uint32_t level)  { *IRQ_REG = level; }
static inline void irq_clear(void)           { *IRQ_REG = 0; }

static inline void sr_mask_all(void)
{
    __asm__ volatile ("ori.w #0x0700,%%sr" ::: "cc");
}
static inline void sr_unmask(void)
{
    __asm__ volatile ("andi.w #0xF8FF,%%sr" ::: "cc");
}

#endif /* FAST68K_SYNC_H */
