# Writing multi-core code for fast_68000

This document describes how to write programs that exploit the multi-core
capability of this design. It covers the programming model the hardware
exposes, the primitives you have to build on, and worked examples in both
68000 assembly (which the included assembler can build directly) and C
(illustrative, presuming an `m68k-elf-gcc`-style toolchain producing code
within the implemented ISA subset — see the toolchain note at the end).

The runnable examples here are pure assembly. The C versions show what the
same algorithms look like in a structured language; they are not built or
tested by `make test`.

## Table of contents

1. [Programming model](#programming-model)
2. [Identifying the running core](#identifying-the-running-core)
3. [Mutual exclusion via TAS](#mutual-exclusion-via-tas)
4. [Producer / consumer with shared memory](#producer--consumer-with-shared-memory)
5. [Barriers](#barriers)
6. [Building higher-level primitives](#building-higher-level-primitives)
7. [Inter-core signaling via the IRQ register](#inter-core-signaling-via-the-irq-register)
8. [Performance pitfalls](#performance-pitfalls)
9. [Worked example: N-way parallel sum reduction](#worked-example-n-way-parallel-sum-reduction)
10. [Toolchain note](#toolchain-note)

---

## Programming model

The system is **shared-memory SMP** with **N identical cores**, all running
from a single program image, all reset to the same `PC = 0x00000400`. The
following points are decisions the hardware makes for you:

- **One image, many cores.** There is no per-core boot vector. Every core
  starts at `0x400` with the same instruction stream. Differentiate cores
  by their stack pointer (see below).
- **Coherent caches.** Each core has private 1 KB L1 instruction and data
  caches. Writes are broadcast on a snoop bus; peer caches invalidate
  matching lines. **You do not need to flush or invalidate caches by hand.**
  A value written by core A is visible to core B on B's next access.
- **Write-through, no write merging.** Every store goes to memory. The
  cache is invalidation-only (no MESI states).
- **One atomic primitive: TAS.** No CAS, no LL/SC, no LOCK prefix. The bus
  arbiter pins the bus to the lock holder across TAS's read-modify-write,
  so TAS is atomic against all other cores. Everything else — mutexes,
  semaphores, barriers, queues — is built on TAS.
- **No memory-fence instruction.** You don't need one for cross-core
  visibility (snoop handles that). You do need to order your *own* writes
  correctly when one write advertises the readiness of another.
- **Flat 32-bit address space, no MMU.** Every core can read and write
  every address, including other cores' stacks. Convention only.

The memory layout reset by hardware is:

| range            | use                                              |
|------------------|--------------------------------------------------|
| `0x000000..0x3FF`| exception vector table (256 vectors × 4 bytes)   |
| `0x000400..`     | code (`.org $400` is the conventional start)     |
| `0x004000`       | reset SP of core 0                               |
| `0x005000`       | reset SP of core 1                               |
| `0x004000 + N*0x1000` | reset SP of core N                          |
| `0xFFFFFFFC`     | memory-mapped IRQ register (also `$FFFC.W`)      |

Stacks grow downward, so each core gets a 4 KB stack between its reset SP
and the next core's reset SP. If you need bigger stacks, point them
elsewhere with an early `move.l #...,A7`.

---

## Identifying the running core

Because the reset SP differs by core, the canonical idiom is to read A7
and key off it. SP for core N is `0x4000 + N * 0x1000`, so the core ID
is `(SP - 0x4000) >> 12`.

### Assembly

The most readable form is a compare tree:

```asm
        ; Branch by core ID.
        move.l  A7, D7
        cmp.l   #$4000, D7
        beq     core0
        cmp.l   #$5000, D7
        beq     core1
        cmp.l   #$6000, D7
        beq     core2
        bra     core3
```

Or compute the numeric ID directly into D7:

```asm
        ; D7 := core_id  (0..N-1)
        move.l  A7, D7
        subi.l  #$4000, D7
        moveq   #12, D6
        lsr.l   D6, D7              ; D7 = (SP - 0x4000) >> 12
```

The shift uses a register count because the 68000's immediate shift count
maxes at 8 and 12 doesn't fit. You can also do two shifts: `lsr.l #8, D7`
followed by `lsr.l #4, D7`.

### C

A tiny inline-asm wrapper. The `volatile` is important — the compiler must
not hoist this across function boundaries.

```c
#include <stdint.h>

static inline uint32_t core_id(void)
{
    uint32_t sp;
    __asm__ volatile ("move.l %%a7,%0" : "=d"(sp));
    return (sp - 0x4000u) >> 12;
}

void main(void)
{
    if (core_id() == 0) {
        run_core0();
    } else {
        run_other_cores(core_id());
    }
}
```

---

## Mutual exclusion via TAS

`TAS ea` is the only atomic operation. It:

1. Reads the byte at `ea`.
2. Sets condition codes from the original value (`Z = 1` if old byte was 0,
   `N = 1` if old bit 7 was 1).
3. Writes back the byte with bit 7 set (`old | 0x80`).

All three steps execute as one bus transaction pair, locked by the arbiter
against every other port.

**Convention.** A lock is one byte. `0x00` = unlocked, anything else =
locked. `TAS` sets bit 7, so after a successful acquire the byte is
`0x80`. To release, write `0x00`.

### Assembly: the spin lock

This is the pattern used in `tests/t06_multicore.s` and is the simplest
correct mutex you can build on this hardware.

```asm
; ---- acquire ----
; A0 = address of the lock byte
acq:    tas     (A0)
        bne     acq              ; old value was nonzero -> retry

; ---- critical section ----
        ...

; ---- release ----
        moveq   #0, D0
        move.b  D0, (A0)         ; write 0x00 to the lock byte
```

`TAS` returns Z=1 only when the old byte was zero, i.e., when we
successfully transitioned 0 → 0x80. `BNE` retries on any other outcome.

### Assembly: acquire with backoff

A tight TAS spin issues one locked bus transaction *per spin*, which
blocks every other core from making progress. For contended locks, back
off after the first miss:

```asm
; A0 = lock address
acq:    tas     (A0)
        beq     have_lock
        ; back off: spin in cache, no bus traffic
        moveq   #16, D0
spin:   subq.l  #1, D0
        bne     spin
        bra     acq
have_lock:
        ...
```

The 16-cycle spin is a magic number — tune it to the typical contention
pattern. For low contention, use a smaller spin; for high contention, use
exponential backoff (double the spin count each time).

### C: lock and unlock

```c
#include <stdint.h>

typedef volatile uint8_t tas_lock_t;

#define TAS_LOCK_INIT 0

static inline void tas_lock(tas_lock_t *p)
{
    __asm__ volatile (
        "1: tas %0\n"
        "   bne 1b\n"
        : "+m"(*p)
        :
        : "cc"
    );
}

static inline void tas_unlock(tas_lock_t *p)
{
    *p = 0;
    /* The volatile store is enough.  Write-through + snoop guarantees
       other cores see the release on their next read.  The 68000 has
       no fence instruction and none is needed here: the in-order
       pipeline plus bus serialization means a peer's acquire after
       our release is bus-ordered with respect to anything we wrote
       during the critical section. */
}
```

Note: the inline asm uses GCC's `1:` / `1b` local labels (b = backward).
Any m68k-targeting GCC accepts this syntax.

### C: critical section

```c
static tas_lock_t g_counter_lock = TAS_LOCK_INIT;
static volatile uint32_t g_counter;

void increment_shared_counter(void)
{
    tas_lock(&g_counter_lock);
    g_counter++;
    tas_unlock(&g_counter_lock);
}
```

---

## Producer / consumer with shared memory

When the synchronization pattern is single-writer-then-single-reader you
don't need a lock — a flag word in shared memory is enough. This is the
pattern in `tests/t07_coherence.s`.

The hardware guarantees you need:

- Coherence: a write by core A is visible to core B on B's next read of
  the same address (snoop invalidates B's cached copy).
- In-order writes from a single core: this design's bus is FIFO from a
  given port, so writes from one core to memory appear in program order
  to every other core. You can therefore use one write to advertise the
  validity of another, as long as the producer writes the data first and
  the flag second, and the consumer reads the flag first and the data
  second.

### Assembly

```asm
; Shared layout:
;   $3000  data (long)
;   $3008  flag (long): 0 = idle, 1 = data ready, 2 = consumed

; --- producer (core 0) ---
        move.l  #42, D0
        move.l  D0, $3000             ; write data first
        moveq   #1, D0
        move.l  D0, $3008             ; then advertise it

prod_wait:
        move.l  $3008, D1             ; wait for consumer
        cmp.l   #2, D1
        bne     prod_wait
        ; consumer is done

; --- consumer (core 1) ---
cons_wait:
        move.l  $3008, D1             ; wait for flag = 1
        cmp.l   #1, D1
        bne     cons_wait

        move.l  $3000, D0             ; read data (snoop guarantees freshness)
        ; ... process D0 ...

        moveq   #2, D1
        move.l  D1, $3008             ; advertise consumption
```

### C

```c
#include <stdint.h>

struct slot {
    volatile uint32_t data;
    uint32_t          pad[1];   /* keep flag in a different 4-byte word */
    volatile uint32_t flag;     /* 0 idle, 1 data ready, 2 consumed */
};

static struct slot g_slot;

void producer(void)
{
    g_slot.data = compute_value();
    g_slot.flag = 1;
    while (g_slot.flag != 2) { /* spin */ }
}

void consumer(void)
{
    while (g_slot.flag != 1) { /* spin */ }
    uint32_t v = g_slot.data;
    process(v);
    g_slot.flag = 2;
}
```

The `pad` field is not strictly necessary because the cache line is one
word (4 bytes); `data` and `flag` already land in different lines if they
are independent 4-byte ints. But padding makes the intent explicit and is
the safe default in case the cache line size changes (e.g. a future
upgrade to 16 B lines).

---

## Barriers

A barrier is the basic "wait for everyone to reach this point" primitive.
On this hardware you build one from a TAS-protected counter plus a spin.

### Assembly: single-use barrier

This barrier can be passed once. Reset it before the next phase. See
the next subsection for a reusable form.

```asm
; Inputs:
;   A0 = lock byte address
;   A1 = counter long address
;   D0 = N (number of cores)
;
; On entry, *A1 == initial_count (caller-zeroed before any core enters).
; On exit from this routine on each core, every core has passed.
barrier:
        ; increment counter under lock
b_acq:  tas     (A0)
        bne     b_acq
        move.l  (A1), D1
        addq.l  #1, D1
        move.l  D1, (A1)
        moveq   #0, D2
        move.b  D2, (A0)

        ; spin until counter >= N
b_wait: move.l  (A1), D1
        cmp.l   D0, D1
        bcs     b_wait              ; while counter < N (unsigned)
        rts
```

### Assembly: sense-reversing barrier (reusable)

For repeated barriers, use a "sense bit" that flips on each pass. Each
core remembers the sense it expects this round.

The `d16(An)` addressing mode lets us reach all three slots of the
barrier struct through one A2.

```asm
; A2 points to a 12-byte barrier struct:
;   $0  lock byte (in low byte of long)
;   $4  counter long
;   $8  sense long
;
; D0 = N (cores).  D7 = expected sense (per-core register, init to 0).

sense_barrier:
        eori.l  #1, D7                ; flip expected sense for next round
sb_acq: tas     (A2)
        bne     sb_acq

        ; increment counter
        move.l  4(A2), D1
        addq.l  #1, D1
        move.l  D1, 4(A2)

        cmp.l   D0, D1                ; last arriver?
        bne     sb_not_last

        ; last arriver: reset counter, flip sense, release lock
        moveq   #0, D2
        move.l  D2, 4(A2)
        move.l  D7, 8(A2)            ; publish new sense
        move.b  D2, (A2)             ; release lock
        rts

sb_not_last:
        moveq   #0, D2
        move.b  D2, (A2)             ; release lock
        ; spin until sense == D7
sb_wait:
        move.l  8(A2), D1
        cmp.l   D7, D1
        bne     sb_wait
        rts
```

### C: sense-reversing barrier

```c
#include <stdint.h>

struct barrier {
    volatile uint8_t  lock;
    uint8_t           pad0[3];
    volatile uint32_t count;
    volatile uint32_t sense;
};

static struct barrier g_bar = { 0, {0,0,0}, 0, 0 };

void barrier_wait(struct barrier *b, uint32_t n, uint32_t *my_sense)
{
    uint32_t s = *my_sense ^ 1u;
    *my_sense = s;

    tas_lock((tas_lock_t *)&b->lock);
    b->count++;
    if (b->count == n) {
        b->count = 0;
        b->sense = s;
        tas_unlock((tas_lock_t *)&b->lock);
    } else {
        tas_unlock((tas_lock_t *)&b->lock);
        while (b->sense != s) { /* spin */ }
    }
}
```

---

## Building higher-level primitives

### CAS (compare-and-swap)

The 68000 has no native CAS in this subset. Emulate it under a TAS lock.

```c
static inline int cas32(volatile uint32_t *p, uint32_t expected, uint32_t new_val,
                        tas_lock_t *lock)
{
    tas_lock(lock);
    int ok = (*p == expected);
    if (ok) *p = new_val;
    tas_unlock(lock);
    return ok;
}
```

For very fine-grained atomics it is often cheaper to give every protected
word its own dedicated lock byte (placed in a different cache word) than
to share one big lock for many words.

### Semaphores

```c
struct sem {
    volatile uint32_t count;
    tas_lock_t        lock;
};

void sem_wait(struct sem *s)
{
    for (;;) {
        tas_lock(&s->lock);
        if (s->count > 0) {
            s->count--;
            tas_unlock(&s->lock);
            return;
        }
        tas_unlock(&s->lock);
        /* back off briefly to avoid bus thrash */
        for (volatile int i = 0; i < 16; i++) { }
    }
}

void sem_post(struct sem *s)
{
    tas_lock(&s->lock);
    s->count++;
    tas_unlock(&s->lock);
}
```

### Single-producer, single-consumer queue

A power-of-two ring buffer with separate head and tail words is lock-free
between exactly one producer and one consumer. The producer owns `tail`;
the consumer owns `head`. Each reads the other's index but never writes
it. Put `head` and `tail` in different cache words (different 4-byte
aligned long words) so a producer-side store does not invalidate the
consumer's cached `head` and vice-versa.

```c
#define Q_LOG2 4
#define Q_SIZE (1u << Q_LOG2)
#define Q_MASK (Q_SIZE - 1u)

struct queue {
    volatile uint32_t head;          /* consumer writes */
    uint32_t          pad0[7];       /* push tail to its own line */
    volatile uint32_t tail;          /* producer writes */
    uint32_t          pad1[7];
    volatile uint32_t slots[Q_SIZE];
};

int q_push(struct queue *q, uint32_t v)
{
    uint32_t t = q->tail;
    uint32_t next = (t + 1u) & Q_MASK;
    if (next == q->head) return 0;   /* full */
    q->slots[t] = v;                  /* publish slot */
    q->tail     = next;               /* then advance tail */
    return 1;
}

int q_pop(struct queue *q, uint32_t *out)
{
    uint32_t h = q->head;
    if (h == q->tail) return 0;       /* empty */
    *out = q->slots[h];               /* read slot (snoop-fresh) */
    q->head = (h + 1u) & Q_MASK;
    return 1;
}
```

This is correct because:

- The producer writes `slots[t]` before advancing `tail`. Bus FIFO order
  means the consumer can never see the new `tail` without also seeing the
  new slot.
- The consumer reads `slots[h]` before advancing `head`. The producer
  can never overwrite `slots[h]` before the consumer's `head` advance is
  visible.

For MPMC (multiple producers / multiple consumers) you need either two
TAS locks (one for `head`, one for `tail`) or a single global queue lock.

---

## Inter-core signaling via the IRQ register

`$FFFFFFFC` (also reachable as `$FFFC.W` in absolute-short addressing) is
a memory-mapped IRQ-level register. Writing `N` (1..7) latches the IRQ
level seen by *every* core's `ipl_i[2:0]` pin. Each core, at its next
instruction boundary, checks `ipl_i > sr_i` and takes an autovector
exception with vector `24 + N` if so. The handler must clear the source
(write 0 to the register) before `RTE`, or the IRQ re-fires immediately.

This is a coarse mechanism: it's a global broadcast, and any core whose
IRQ mask is low enough will take it. There is no per-core directed IPI.

### Asm: receiver setup

```asm
        .org    $64                  ; vector 25 = IRQ at IPL 1
        .long   irq_handler

        .org    $400
        andi    #$F8FF, SR           ; lower IRQ mask: sr_i := 0
        ; ... normal code; an IRQ at level 1+ will be taken now ...

irq_handler:
        ; do whatever the signal means
        move.l  #1, $5000            ; some shared "I got it" flag
        ; clear the IRQ source
        move.l  #0, D0
        move.l  D0, $FFFC.W
        rte
```

### Asm: sender

```asm
; Any core can post a signal at IPL=N (N ∈ 1..7) by writing N to $FFFC:
        move.l  #1, D0
        move.l  D0, $FFFC.W
```

Note that the *sender* core will also see this IRQ if its own `sr_i`
allows it. If you want to signal "the others, not me," raise the
sender's mask first:

```asm
        ori     #$0700, SR           ; sr_i := 7 (mask everything)
        move.l  #1, D0
        move.l  D0, $FFFC.W           ; only other cores will fire
        ; ... do work ...
        andi    #$F8FF, SR           ; lower mask again
```

### C

```c
#include <stdint.h>

static volatile uint32_t * const IRQ_REG = (uint32_t *)0xFFFFFFFCu;

static inline void irq_post(uint32_t level)
{
    *IRQ_REG = level;
}

static inline void irq_clear(void)
{
    *IRQ_REG = 0;
}

static inline void sr_mask_all(void)
{
    __asm__ volatile ("ori #0x0700,%%sr" ::: "cc");
}

static inline void sr_unmask(void)
{
    __asm__ volatile ("andi #0xF8FF,%%sr" ::: "cc");
}
```

`ori #..., SR` and `andi #..., SR` are privileged, so this only works
from supervisor mode. After reset every core is in supervisor mode
(S=1, I=7), so this is the default; an explicit transition to user mode
with the canonical `move A7,USP; move ... ,SR` sequence is only needed
if you want it.

---

## Performance pitfalls

### False sharing

The L1 cache line is **one 32-bit word (4 bytes)**. Two cores writing
different bytes of the same 4-byte word will trash each other's caches
on every store. Two cores writing adjacent 4-byte words will not — they
land in different lines.

**Rule of thumb.** Align every per-core counter, flag, or queue index to
a 4-byte boundary and keep them in separate 4-byte words. For per-core
arrays, give each core a 4-byte-aligned slot.

```c
struct partial {
    volatile uint32_t value;
    uint32_t pad[3];          /* keep adjacent partials far apart;
                                 16-byte padding is safe even if the
                                 cache later moves to a larger line */
};

static struct partial g_partials[N];
```

### Direct-mapped conflict misses

The caches are direct-mapped with 256 lines. Addresses that are 1 KB
apart (`addr mod 1024 == same`) compete for the same line. A program
that hot-loops on two addresses 1 KB apart will see 100% misses.

**Rule of thumb.** If you stride large stretches of memory, vary the
base addresses by something *other* than 1 KB. For per-core data
blocks, pick `core_base = 0x10000 + core_id * 0x100` or similar
non-power-of-two strides.

### Tight TAS spinning

Every iteration of `tas (a0); bne 1b` is one locked bus transaction
pair. While one core spins, the arbiter pins the bus to it across two
cycles. With N cores all spinning, the bus achieves zero useful
forward progress.

**Rule of thumb.** Spin in a small cache-resident delay loop between
TAS attempts. The simple loop in [Assembly: acquire with backoff]
above is enough for most cases.

### Bus contention on streaming workloads

The bus has one slot per cycle. With 2N cache ports (I + D per core)
competing, a workload where every core is constantly missing both
caches saturates at 1/N of single-core throughput, even though every
core is logically independent.

**Rule of thumb.** Keep hot data in L1. The cache is small (1 KB), so
your hot working set per core should fit in roughly 256 long-words for
the data side. Above that, expect linear degradation in throughput as
cores collide on the bus.

---

## Worked example: N-way parallel sum reduction

Compute `sum(array[0..K-1])` across N cores. Each core handles a
contiguous slice and writes its partial to a per-core slot. After a
barrier, core 0 sums the partials and stops with the total in D0.

### Shared layout

```
$1000    array[0..K-1] of long      (K longs; K assumed multiple of N)
$2000    partials[0..N-1] of long, each padded to 16 bytes apart
$3000    barrier struct: lock byte at +0, count at +4, sense at +8
$3010    per-core "expected sense" memory backing for return-from-barrier
```

### Assembly (tested-style; targets N=4 cores)

```asm
        .org    $400

start:
        ; ---- compute core_id into D7 ----
        move.l  A7, D0
        subi.l  #$4000, D0
        moveq   #12, D1
        lsr.l   D1, D0              ; D0 = core_id (0..3)
        move.l  D0, D7              ; keep in D7

        ; ---- compute slice bounds ----
        ; K = 64 (example), slice_size = 16 per core
        move.l  #16, D6
        move.l  D7, D5
        muls.w  D6, D5              ; D5 = core_id * 16  (start index)
        ; address = $1000 + D5*4
        lsl.l   #2, D5
        move.l  #$1000, A0
        adda.l  D5, A0              ; A0 -> start of our slice
        move.l  D6, D5              ; D5 = element count = 16

        ; ---- sum the slice into D0 ----
        moveq   #0, D0
sum_lp:
        add.l   (A0)+, D0
        subq.l  #1, D5
        bne     sum_lp

        ; ---- write partial to $2000 + core_id*16 ----
        move.l  D7, D5
        lsl.l   #4, D5              ; *16 for padding
        move.l  #$2000, A1
        adda.l  D5, A1
        move.l  D0, (A1)

        ; ---- if core 0, initialise barrier counter/sense ----
        tst.l   D7
        bne     past_init
        moveq   #0, D0
        move.l  D0, $3000           ; lock = 0
        move.l  D0, $3004           ; count = 0
        move.l  D0, $3008           ; sense = 0
past_init:
        ; Give everyone a beat to see the zeroed barrier (very crude;
        ; a real program would use a separate boot barrier).
        moveq   #100, D6
init_w: subq.l  #1, D6
        bne     init_w

        ; ---- barrier wait ----
        move.l  #$3000, A2
        moveq   #4, D6              ; N = 4
        moveq   #0, D4              ; expected sense, starts at 0
        bsr     barrier_wait

        ; ---- core 0 reduces partials, others stop ----
        tst.l   D7
        bne     done

        moveq   #0, D0
        move.l  #$2000, A1
        moveq   #4, D5
red_lp: add.l   (A1), D0
        adda.l  #16, A1
        subq.l  #1, D5
        bne     red_lp
        ; D0 now holds the total
        ; (In a real test you would CMP it against the expected value.)

done:
        stop    #0

; ---- barrier_wait ----
; A2 = barrier struct ; D6 = N ; D4 = expected sense (caller-managed)
barrier_wait:
        eori.l  #1, D4
bw_acq: tas     (A2)
        bne     bw_acq
        move.l  4(A2), D1
        addq.l  #1, D1
        move.l  D1, 4(A2)
        cmp.l   D6, D1
        bne     bw_not_last
        moveq   #0, D2
        move.l  D2, 4(A2)           ; reset count
        move.l  D4, 8(A2)           ; publish sense
        move.b  D2, (A2)            ; release lock
        rts
bw_not_last:
        moveq   #0, D2
        move.b  D2, (A2)            ; release lock
bw_wait:
        move.l  8(A2), D1
        cmp.l   D4, D1
        bne     bw_wait
        rts

        ; place array & partials by hand for the example
        .org    $1000
        ; 64 longs of test data — fill as you wish
        .long   1, 2, 3, 4, 5, 6, 7, 8
        .long   9, 10, 11, 12, 13, 14, 15, 16
        ; ... etc through 64 elements ...
```

The expected sum for the integers 1..64 is 2080. A real test would
compare D0 against that and stop with a non-zero code on mismatch.

### C equivalent

```c
#include <stdint.h>

#define N          4u
#define K          64u
#define SLICE      (K / N)

static volatile uint32_t array[K];        /* lives at 0x1000 */
struct partial_slot {                      /* lives at 0x2000 */
    volatile uint32_t value;
    uint32_t pad[3];
};
static struct partial_slot partials[N];

struct barrier {
    volatile uint8_t  lock;
    uint8_t           pad0[3];
    volatile uint32_t count;
    volatile uint32_t sense;
};
static struct barrier g_bar = { 0, {0,0,0}, 0, 0 };

static inline uint32_t core_id(void)
{
    uint32_t sp;
    __asm__ volatile ("move.l %%a7,%0" : "=d"(sp));
    return (sp - 0x4000u) >> 12;
}

void barrier_wait(struct barrier *b, uint32_t n, uint32_t *my_sense)
{
    uint32_t s = *my_sense ^ 1u;
    *my_sense = s;
    tas_lock((tas_lock_t *)&b->lock);
    b->count++;
    if (b->count == n) {
        b->count = 0;
        b->sense = s;
        tas_unlock((tas_lock_t *)&b->lock);
    } else {
        tas_unlock((tas_lock_t *)&b->lock);
        while (b->sense != s) { }
    }
}

void main(void)
{
    uint32_t me     = core_id();
    uint32_t base   = me * SLICE;
    uint32_t sense  = 0;   /* per-core, lives in stack/regs */

    uint32_t sum = 0;
    for (uint32_t i = 0; i < SLICE; i++) {
        sum += array[base + i];
    }
    partials[me].value = sum;

    barrier_wait(&g_bar, N, &sense);

    if (me == 0) {
        uint32_t total = 0;
        for (uint32_t i = 0; i < N; i++) total += partials[i].value;
        finalize(total);
    }
    stop_core(0);
}
```

The per-core sense bit lives on each core's stack as a local variable.
This works because the stack is private (each core's reset SP puts it
in its own 4 KB region) — no thread-local storage support needed.

---

## Toolchain note

There is **no C compiler integrated with this repo**. The test suite and
benchmarks are written in 68000 assembly and built with the in-tree
single-file assembler at `tb/asm68k.py`.

The C examples in this document are illustrative. They assume a hypothetical
`m68k-elf-gcc`-style cross compiler producing code that:

- Uses only the implemented ISA subset (see DESIGN.md § ISA implementation
  status; in particular, **no `d8(An,Xn)` indexed mode** and **no `MULU.L`
  / 32-bit multiply**).
- Targets the byte ordering and reset model documented above.
- Uses inline assembly via GCC's `__asm__ volatile (...)` for TAS and SR
  access.

To actually run C, you would need to:

1. Build or fetch `m68k-elf-gcc` with `-mcpu=68000`.
2. Restrict the code generator (or carefully audit emitted code) to the
   instructions this core implements — the standard 68000 backend
   produces a small number of opcodes this core does not decode (e.g.,
   `MOVEP`, indexed addressing) and would need either an alternative
   backend setting or hand-patched output for those cases.
3. Convert the linked ELF to the `$readmemh` format the simulator
   expects. The simplest path is `m68k-elf-objcopy -O binary` followed by
   a 4-bytes-per-line hex dumper.

Until such a toolchain is wired in, the canonical way to write programs
for this core is the included assembler. Every pattern in this document
has an assembly form, and the assembly is what the regression tests
build and run.
