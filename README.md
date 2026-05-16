# 68K-mini in Verilog

A subset of the Motorola 68000 implemented in Verilog and verified with
Verilator. The design includes a 3-stage pipeline with operand forwarding,
per-core instruction and data caches with snoop-based invalidation, and a
round-robin shared-bus arbiter that supports atomic locked transactions —
enough to demonstrate a small multi-core SMP system.

## Layout

```
rtl/
  m68k_defs.vh    constants
  m68k_alu.v      32-bit ALU with NZVC flags
  m68k_regfile.v  D0..D7, A0..A7 (3 read ports, 1 write + 1 aux write)
  m68k_decoder.v  16-bit opcode -> control bundle
  m68k_cache.v    direct-mapped, write-through, snoop-invalidating cache
  m68k_bus.v      round-robin arbiter, big-endian memory, lock support
  m68k_core.v     3-stage pipelined core (IF | ID | EX/MEM/WB)
  m68k_top.v     N_CORES wired together with caches and shared bus
tb/
  sim_main.cpp    Verilator C++ harness
  asm68k.py       tiny 68K-mini assembler -> $readmemh-ready hex
tests/
  t01..t07        regression suite, see below
Makefile          build + run-all
ISA.md            instruction-set spec
```

## Architectural optimizations

* **Pipelining** — three stages (IF / ID / EX). The IF stage assembles
  multi-word instructions (up to four extension words) from the I-cache.
  The EX stage runs the ALU, memory accesses, and writebacks. Result
  forwarding from the EX-stage writeback to the ID-stage register reads
  removes the back-to-back RAW hazard.
* **Per-core L1 caches** — separate I and D caches, direct-mapped, 1 word
  per line, 256 sets. Write-through, no-write-allocate.
* **Snoop coherence** — every accepted bus write is broadcast on a snoop
  bus; other caches invalidate matching lines.
* **Locked RMW (TAS)** — the bus arbiter pins the bus to the lock holder
  across the second transaction so test-and-set is atomic against all
  other ports.
* **Multi-core** — `N_CORES` is a parameter. Tested at 1, 2, and 4 cores.
  Each core gets a private stack (SP at 0x4000, 0x5000, 0x6000, ...).

## Running

```
make test                 # default 2 cores, builds & runs every test
make bench                # builds two 1-core sims (cache on/off) and prints a
                          # comparison table against canonical 68000 cycles
make N_CORES=4 build      # rebuild for 4 cores
```

## Benchmarks

`make bench` produces, for every `bench/*.s` program, a table comparing:

* `68k_ref`  – sum of canonical M68000PRM instruction execution times
  (baked into each .s file as a `; basic_68000_cycles=N` header)
* `fast`     – cycles on the cached, pipelined build (`USE_CACHE=1`)
* `slow`     – cycles on the same pipelined core with caches replaced by a
  bus passthrough (`USE_CACHE=0`)
* `cpi_*`    – cycles per retired instruction on each platform
* `vs_68k`   – speedup of our fast build over a basic 68000
* `vs_slow`  – cache-only speedup (isolates the cache contribution)

Sample output (Verilator 5.048, M2 Mac):

```
benchmark   retired  68k_ref cpi_ref    fast cpi_fast    slow cpi_slow  vs_68k  vs_slow
fib             105      718    6.84     133     1.27     338     3.22   5.40x    2.54x
jsr             304     3334   10.97     934     3.07    1835     6.04   3.57x    1.96x
memcopy         458     4286    9.36     888     1.94    1669     3.64   4.83x    1.88x
sum             404     4034    9.99     632     1.56    1835     4.54   6.38x    2.90x
```

The headline number — `vs_68k` — went from 1.4–1.6× in the initial design up to
**3.6–6.4×** after three frontend changes: combinational L1 cache hits,
streaming IF (latch + dispatch + start next fetch in a single cycle), and
BTFNT static branch prediction (backwards taken, forwards not-taken).

The benchmark workloads:

| name     | what it stresses                                         |
|----------|----------------------------------------------------------|
| sum      | tight ALU loop: ADD + ADDQ + CMP + Bcc                   |
| fib      | iterative Fibonacci, all register-resident               |
| memcopy  | streaming loads/stores through `(An)+`                   |
| jsr      | JSR abs.L + RTS overhead, 50 subroutine calls            |

## Tests

| test          | covers                                       |
|---------------|----------------------------------------------|
| t01_basic     | reset, NOP, STOP                             |
| t02_arith     | MOVEQ, ADD, SUB, CMP+BEQ, AND                |
| t03_loop      | ADDQ, conditional branch loop                |
| t04_memory    | (An)+, -(An), per-core branch on SP          |
| t05_call      | BSR/RTS, JSR abs.L, recursion + stack        |
| t06_multicore | TAS-protected shared counter, two cores      |
| t07_coherence | producer/consumer ping-pong via shared mem   |

See `ISA.md` for the supported instruction set and addressing modes.
