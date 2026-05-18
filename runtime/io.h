/* io.h — memory-mailbox console for fast_68000 C demos.
 *
 * The simulator's testbench (tb/sim_main.cpp) polls a small ring
 * buffer in main memory and prints any new characters to stdout.
 * Layout (chosen to live above per-core stacks but below 64 KB):
 *
 *   0x0000F000   head index  (uint32, CPU writes; host reads)
 *   0x0000F004   tail index  (uint32, host writes; CPU reads — unused)
 *   0x0000F008   per-core lock byte (TAS), so output from different
 *                cores doesn't interleave mid-byte
 *   0x0000F010   start of 256-byte ring (head & 0xFF indexes a slot)
 *
 * No RTL hooks are needed: the testbench reads memory via the
 * fb_peek port, which bypasses the cache and sees write-through
 * stores immediately.
 *
 * Bytes are interpreted as ASCII.  A non-printable byte ends the
 * stream for the host's purposes (so embed e.g. 0xFF to signal end);
 * the regression harness usually just dumps everything until halt.
 */
#ifndef FAST68K_IO_H
#define FAST68K_IO_H

#include <stdint.h>
#include "sync.h"

#define CONSOLE_HEAD_ADDR  0x0000F000u
#define CONSOLE_TAIL_ADDR  0x0000F004u
#define CONSOLE_LOCK_ADDR  0x0000F008u
#define CONSOLE_RING_ADDR  0x0000F010u
#define CONSOLE_RING_MASK  0xFFu       /* 256-byte ring */

static inline void putc_locked(char c)
{
    volatile uint32_t *head_p = (uint32_t *)CONSOLE_HEAD_ADDR;
    volatile uint8_t  *ring   = (uint8_t  *)CONSOLE_RING_ADDR;
    uint32_t h = *head_p;
    ring[h & CONSOLE_RING_MASK] = (uint8_t)c;
    *head_p = h + 1u;
}

/* Single character. Takes the console lock so multi-core writes don't
   interleave at the byte level. */
static inline void putc(char c)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
    putc_locked(c);
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}

/* Whole string under one acquire so a line stays together. */
static inline void puts_(const char *s)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
    while (*s) putc_locked(*s++);
    putc_locked('\n');
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}

/* Same as puts_ but without the trailing newline. */
static inline void print(const char *s)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
    while (*s) putc_locked(*s++);
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}

/* Lock-once-per-line helpers.  Call console_begin(), emit any
 * combination of print_locked/putc_locked/putu_locked/puth_locked,
 * then console_end().  No other core can interleave in between. */
static inline void console_begin(void)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}
static inline void console_end(void)
{
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}
static inline void print_locked(const char *s)
{
    while (*s) putc_locked(*s++);
}
static inline void putu_locked(uint32_t v)
{
    if (v == 0) { putc_locked('0'); return; }
    uint32_t p = 1;
    while (p <= 1000000000u) {
        uint32_t np = p * 10u;
        if (np > v) break;
        p = np;
    }
    while (p > 0) {
        uint32_t d = 0;
        while (v >= p) { v -= p; d++; }
        putc_locked((char)('0' + d));
        if (p == 1) break;
        uint32_t np = 1;
        while (np * 10u < p) np *= 10u;
        p = np;
    }
}
/* Print one nibble (no prefix, no lock). */
static inline void putn_locked(uint32_t d)
{
    d &= 0xFu;
    putc_locked((char)(d < 10 ? '0' + d : 'a' + (d - 10)));
}

/* Print uint32_t as 8-nibble hex with "0x" prefix.  Fixed width =
 * 8 hex chars so we don't need a stack buffer or backwards walk. */
static inline void puth_locked(uint32_t v)
{
    putc_locked('0'); putc_locked('x');
    putn_locked(v >> 28);
    putn_locked(v >> 24);
    putn_locked(v >> 20);
    putn_locked(v >> 16);
    putn_locked(v >> 12);
    putn_locked(v >>  8);
    putn_locked(v >>  4);
    putn_locked(v);
}

/* Print an unsigned int in decimal (no padding).  Avoids libgcc's
 * 32-bit __udivsi3 (which wedges) and avoids reading any pre-built
 * pow10 array (an earlier attempt missed digits when the array lived
 * in .rodata — root cause unclear, possibly a cache/snoop edge case).
 * Strategy: walk a single uint32_t power down by /10 each step.
 *
 * Range: 0..0xFFFFFFFF.
 */
static inline void putu(uint32_t v)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
    if (v == 0) {
        putc_locked('0');
        tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
        return;
    }
    /* Find the highest pow10 <= v by multiplying up.  Constant
     * cap at 1e9 ensures we don't overflow uint32_t (max ~4.29e9). */
    uint32_t p = 1;
    while (p <= 1000000000u) {
        uint32_t np = p * 10u;
        if (np > v) break;
        p = np;
    }
    /* Now p is the highest pow10 <= v.  Walk down printing each digit. */
    while (p > 0) {
        uint32_t d = 0;
        while (v >= p) { v -= p; d++; }
        putc_locked((char)('0' + d));
        if (p == 1) break;
        /* p /= 10 without __udivsi3 — recompute by counting from 1. */
        uint32_t np = 1;
        while (np * 10u < p) np *= 10u;
        p = np;
    }
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}

/* Print an unsigned int in 0x-prefixed lowercase hex, fixed 8 nibbles. */
static inline void puth(uint32_t v)
{
    tas_lock((tas_lock_t *)CONSOLE_LOCK_ADDR);
    puth_locked(v);
    tas_unlock((tas_lock_t *)CONSOLE_LOCK_ADDR);
}

#endif /* FAST68K_IO_H */
