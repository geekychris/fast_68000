# fast_68000: Design Document

A pipelined, cached, multi-core implementation of a Motorola 68000 instruction
set subset, written in Verilog and verified with Verilator. Built to exhibit
the classic CPU microarchitecture optimizations — pipelining with forwarding,
private L1 caches with snoop-based coherence, atomic locked RMW operations
for inter-core synchronisation, and static branch prediction — and to quantify
their impact against the published 68000 timing table.

Final headline numbers on this design:

```
benchmark   retired  68k_ref cpi_ref    fast cpi_fast    slow cpi_slow  vs_68k  vs_slow
fib             105      718    6.84     133     1.27     338     3.22   5.40x    2.54x
jsr             304     3334   10.97     934     3.07    1835     6.04   3.57x    1.96x
memcopy         458     4286    9.36     888     1.94    1669     3.64   4.83x    1.88x
sum             404     4034    9.99     632     1.56    1835     4.54   6.38x    2.90x
```

`vs_68k` is the ratio of canonical M68000 PRM cycles to our cycles for the
same instruction stream. The cached + pipelined build finishes the same
benchmark programs in **3.6×–6.4× fewer clock cycles** than a textbook 68000
would. `vs_slow` isolates the cache contribution by comparing against a
no-cache build of the same pipeline.

---

## Table of contents

1. [Goals and non-goals](#goals-and-non-goals)
2. [ISA: 68K-mini](#isa-68k-mini)
3. [System architecture](#system-architecture)
4. [Pipeline microarchitecture](#pipeline-microarchitecture)
5. [Memory subsystem](#memory-subsystem)
6. [Multi-core, coherence, atomics](#multi-core-coherence-atomics)
7. [Optimizations](#optimizations)
8. [Module reference](#module-reference)
9. [Verification methodology](#verification-methodology)
10. [Benchmarks](#benchmarks)
11. [Build and run](#build-and-run)
12. [Known limitations](#known-limitations)
13. [Future work](#future-work)
14. [File map](#file-map)

---

## Goals and non-goals

**Goals.**

- Faithfully implement enough of the M68000 ISA to run interesting hand-written
  programs (arithmetic, loops, subroutines, memory streaming, inter-core
  synchronisation via TAS).
- Pipeline the core with hazard forwarding and a static branch predictor.
- Build a coherent memory hierarchy with per-core L1 caches, write-through
  with snoop-invalidate, and atomic locked transactions.
- Scale to N cores parameterically.
- Verify with Verilator and a small regression + benchmark suite, with a
  reference comparison against the canonical 68000 cycle counts.

**Explicit non-goals.**

- Cycle-accurate emulation of the real 68000. We pick a subset of the ISA and
  measure clock cycles on our own implementation, then compare against PRM
  per-instruction cycle counts.
- Full ISA coverage. No .B / .W sizes, no displacement/indexed addressing
  modes, no DBcc, no MOVEM, no supervisor mode or exceptions.
- Hardware synthesis. Designed for clarity in simulation; gate-level timing
  was not analysed. Real silicon would need further work (clock-domain
  consideration, retiming, ASIC/FPGA targeting).

---

## ISA: 68K-mini

A user-mode subset of the M68000 with a 32-bit datapath. Every arithmetic
operation is long-word (.L). The full opcode encoding is documented in
`ISA.md`; here is a summary.

### Registers

| name      | description                                                       |
|-----------|-------------------------------------------------------------------|
| D0..D7    | 32-bit data registers                                             |
| A0..A7    | 32-bit address registers; A7 is the stack pointer (SP)            |
| PC        | 32-bit program counter                                            |
| CCR       | condition codes: N, Z, V, C                                       |

The register file (`m68k_regfile.v`) is a 16-entry flat array indexed by a
4-bit number with bit 3 selecting data (0) vs. address (1). It exposes three
read ports (source, destination/Dn-operand, SP) and one main write port plus
one auxiliary write port. The auxiliary port commits address-register updates
from auto-increment/decrement and SP push/pop in the same cycle as the main
result writeback.

### Addressing modes

| mode | reg | meaning                                                       |
|------|-----|---------------------------------------------------------------|
| 000  | r   | Dr (data register direct)                                     |
| 001  | r   | Ar (address register direct)                                  |
| 010  | r   | (Ar) — address-register indirect                              |
| 011  | r   | (Ar)+ — postincrement                                         |
| 100  | r   | -(Ar) — predecrement                                          |
| 111  | 000 | abs.W (sign-extended 16-bit absolute, 1 extension word)       |
| 111  | 001 | abs.L (32-bit absolute, 2 extension words)                    |
| 111  | 100 | #imm.L (32-bit immediate, 2 extension words)                  |

### Instructions

| mnemonic | semantics                                                       |
|----------|-----------------------------------------------------------------|
| NOP      | no operation                                                    |
| RTS      | pop PC from stack                                               |
| STOP #imm| halt this core; imm16 is the exit code                          |
| JMP ea   | PC = ea                                                         |
| JSR ea   | push PC_next, then PC = ea                                      |
| LEA ea,An| An = ea                                                         |
| MOVEQ #i,Dn | sign-extend imm8 into Dn                                     |
| MOVE.L src,dst | copy long; both EAs can be memory or register             |
| ADD.L / SUB.L / AND.L / OR.L / CMP.L src,Dn | Dn = Dn op src        |
| EOR.L Dn,Dm | reg-to-reg only in this subset                              |
| ADDQ.L / SUBQ.L #i,ea | immediate 1..8 (0 encodes 8)                       |
| TAS ea   | atomic test-and-set of the low byte of `ea`                     |
| BRA / BSR disp | unconditional / call (8- or 16-bit displacement)          |
| Bcc disp | conditional branch using CCR; 8- or 16-bit displacement         |

Condition codes follow the standard M68000 encoding (HI, LS, CC, CS, NE, EQ,
VC, VS, PL, MI, GE, LT, GT, LE).

### Memory model

- Byte-addressable, big-endian. Byte address `A` lives in
  `mem[A>>2][31:24] / [23:16] / [15:8] / [7:0]` for `A & 3 = 0/1/2/3`.
- 64 KB by default (`MEM_WORDS = 16384`).
- Memory is initialised at simulation start from `program.hex` via
  `$readmemh`; the assembler `tb/asm68k.py` produces this file.

### Reset model

- All cores start at PC = `0x00000400` (`RESET_PC`).
- Each core's reset SP is computed from its `CORE_ID` parameter:
  core 0 → `0x4000`, core 1 → `0x5000`, core 2 → `0x6000`, etc.
  Test programs read SP to differentiate cores running the same code.

---

## System architecture

```
                                    ┌──────────────────────────────┐
                                    │       m68k_top.v             │
                                    │                              │
   per core i ∈ [0, N_CORES):       │                              │
   ┌──────────────┐                 │                              │
   │  m68k_core   │                 │                              │
   │   (3-stage)  │                 │                              │
   └─┬──────────┬─┘                 │                              │
     │ ic_*     │ dc_*              │                              │
     ▼          ▼                   │                              │
   ┌──────┐  ┌──────┐               │                              │
   │  IC$ │  │  DC$ │  m68k_cache   │                              │
   │   /  │  │      │  (or pass-    │                              │
   │  pass│  │      │   through)    │                              │
   └──┬───┘  └──┬───┘               │                              │
      │bus      │bus                │                              │
      └────┬────┘                   │                              │
           ▼                        │                              │
        ┌───────────────────────────┴─────────────────────────────┐│
        │                  m68k_bus.v                              ││
        │  round-robin arbiter, lock tracking, snoop broadcast,    ││
        │  big-endian 32-bit-wide memory ($readmemh program.hex)   ││
        └───────────────────────────────────────────────────────────│
                                                                    │
                                    └────────────────────────────────┘
```

Every core has a private L1 instruction cache and L1 data cache. Both caches
share the system bus, on which the arbiter implements round-robin priority
with a lock-pending override for atomic RMW. Each accepted write transaction
is broadcast on a snoop output so the other caches invalidate matching lines.

The `USE_CACHE` parameter on `m68k_top` swaps each cache instance for an
`m68k_passthrough` that has the same interface but no storage, used for
performance comparisons.

---

## Pipeline microarchitecture

Three logical stages: **IF**, **ID**, and **EX** (with EX absorbing memory
access and writeback for simplicity).

```
     ┌──────────────────┐     ┌──────────────┐     ┌─────────────────────┐
     │       IF         │     │     ID       │     │       EX            │
     │                  │     │              │     │ ALU                 │
     │ - issue I-cache  │     │ - decode     │ ──► │ EA compute          │
     │ - latch opcode + │ ──► │ - regfile rd │     │ D-cache R/W         │
     │   ext words      │     │ - BTFNT pred │     │ branch resolve      │
     │ - dispatch       │     │ - forwarding │     │ writeback (comb)    │
     │ - speculative    │     │              │     │ CCR commit          │
     │   fetch on pred  │     │              │     │                     │
     └─────────┬────────┘     └──────┬───────┘     └──────────┬──────────┘
               │ if_dispatch_w       │ id_valid               │ ex_valid
               │ if_op, if_ext,      │ id_op, id_ext, etc.    │ writeback
               │ predict_taken_w     │ + forwarded operands   │
               ▼                     ▼                        ▼
            IF/ID register        ID/EX register           regfile / CCR
```

### IF stage

Responsible for assembling each instruction (one opcode + up to four
extension words for `MOVE.L abs.L,abs.L`-style worst cases) and dispatching
it to ID along with a static branch prediction.

State (registered):

- `if_pc`: PC of the opcode currently being fetched.
- `if_op`: most recently latched 16-bit opcode.
- `if_ext[0..3]`: extension words 1..4.
- `if_words_done`: number of words already latched into the buffer.
- `if_busy`: a cache fetch is in flight on the I-cache port.
- `if_slot`: which slot the current in-flight word will fill.
- `if_fetch_addr`: byte address of the in-flight fetch (used to pick the
  correct half-word from the 32-bit cache response).
- `if_drain`: a fetch was outstanding when a redirect arrived; the next ack
  must be discarded before issuing the redirected fetch.

A combinational decoder runs on an **effective opcode**: if the IF stage is
latching the opcode word this cycle, the decoder sees the just-arrived
half-word from the cache directly, not the registered `if_op`. This lets the
IF compute `total_words` and the branch displacement in the same cycle the
opcode arrives, so it can latch, dispatch, and start the next fetch all in
one posedge.

### ID stage

Combinational decode (`m68k_decoder` instance #2, on the latched `id_op`),
register read with single-cycle EX→ID forwarding, and immediate / EA
extraction.

The forwarding network is a single `fwd(idx, base)` helper that prefers the
main writeback bus, then the aux writeback bus, then the regfile output.
Because EX drives writeback signals **combinationally** when an instruction
settles, a dependent next instruction in ID sees the new value in the same
cycle and never stalls for a RAW hazard on a register-only operation.

The IF/ID register also carries `id_predicted_taken` — the BTFNT prediction
made at IF dispatch — so EX can detect mispredicts later.

### EX stage

Implements an explicit state machine per in-flight instruction:

- **S_RUN** — single-cycle ops (MOVEQ, MOVE reg-reg, ALU reg-reg, MOVEQ,
  Bcc, JMP). Settles in one cycle and writes back the result combinationally.
- **S_LOAD** — memory load in flight (MOVE-from-mem, ADD-from-mem, RTS, TAS
  phase 0). Settles when `dc_ack` arrives.
- **S_STORE** — memory store in flight (MOVE-to-mem, JSR push, BSR push).
- **S_TASW** — TAS phase 1: writes the test-and-set byte back to memory.

`is_settled` is the union of "did one of those states just complete?". When
asserted, all of the instruction's side effects fire on the same posedge:
register writeback, CCR commit, retired-counter increment, and the branch
redirect signal. `stall = ex_valid && !halted && !is_settled` freezes the
upstream pipeline registers.

### Hazard handling and forwarding

Because EX writes back combinationally and ID reads with the forwarding mux
already wired to those combinational signals, RAW hazards on a single
register operand resolve without any stall.

Memory operations are handled inside the EX state machine; the pipeline
stalls naturally as long as the cache is in-flight, because `is_settled`
won't fire until `dc_ack`.

### Branch prediction and mispredict recovery

The IF stage runs BTFNT statically:

- `BRA` (Bcc with cc = T): predicted taken always.
- `BSR` (Bcc with cc = F): predicted taken always.
- Conditional Bcc with displacement sign bit set: predicted taken (backward
  branch — classic loop case).
- All others (including conditional forward branches): predicted not-taken.

The prediction redirects IF to the predicted target on the same posedge as
the dispatch. The predicted-taken bit rides through ID and ID/EX so EX can
compare against the actual outcome (computed once the CCR has been read).

Mispredict resolution:

- `redirect_valid` is **combinational** in EX, driven from `is_settled &&
  (BCC mispredict OR non-Bcc branch taken OR STOP)`. Making it combinational
  is critical: with the streaming-IF speculative path, a wrong-path
  instruction can otherwise enter EX in the same cycle that the branch
  resolves. The combinational redirect lets the ID/EX pipeline register
  observe `ex_valid <= 0` on that same posedge.

Mispredict penalty is 2 cycles structurally (one for IF/ID flush, one for
the new IF fetch to land in ID), assuming the new target hits the I-cache.

### Pipeline summary

| stage | latency on cache hit, no hazard | bottlenecks                    |
|-------|---------------------------------|--------------------------------|
| IF    | 1 cycle per opcode word         | I-cache miss, extension words  |
| ID    | combinational                   | none in this design            |
| EX    | 1 cycle for register-only ops   | D-cache miss, TAS RMW (2 txns) |

Tight register-only loops with BTFNT-correct branches achieve sustained
**1 instruction per cycle** (see `sum.s` and `fib.s`).

---

## Memory subsystem

### Cache organisation

Per-core L1 I-cache and L1 D-cache, both instantiated from `m68k_cache.v`.

- **Direct-mapped**, 256 lines × 4 bytes = 1 KB per cache.
- **Write-through** (D-cache only; the I-cache is read-only).
- **No-write-allocate** on a write miss (the line is not pulled in; the bus
  takes the write and that's it).
- **Combinational hit response** on reads: `cpu_ack` and `cpu_rdata` fall out
  in the same cycle as `cpu_req`. The cache state machine only transitions
  on miss or write.

### Cache transactions

For reads:

- If `state == S_IDLE && cpu_req && !cpu_we && hit && !cpu_lock`, the cache
  returns `cpu_rdata = data[idx]` and `cpu_ack = 1` combinationally. No state
  change. This is the steady-state fast path.
- If the line misses (or `cpu_lock = 1`, see TAS), the cache asserts `bus_req`
  and moves to `S_BUS_WAIT`. When `bus_resp_valid` arrives, the line is
  filled, `cpu_ack` pulses combinationally for one cycle, and the state
  returns to `S_IDLE`.

For writes (D-cache only):

- The cache always transitions to `S_BUS_WAIT` and issues a bus write,
  regardless of hit or miss. On hit, it also updates `data[idx]` locally.
- `cpu_ack` pulses combinationally when the bus response arrives.

### One-grant-per-transaction invariant

A subtle but important detail: `bus_req` is held over two cycles in any
standard register-transfer-level design — once while the request is
in flight on the bus, and once during the response cycle. That would make
the arbiter grant the same port twice for one logical transaction, which
breaks the lock-pending counter used for TAS atomicity.

The fix in this design: `bus_req` is a wire driven by
`bus_req_r && !bus_resp_valid`. The instant the bus response arrives, the
request signal drops combinationally and the arbiter sees a clean
one-grant-per-transaction protocol. The same fix is applied to
`m68k_passthrough.v`.

### Snoop coherence

The bus broadcasts every accepted write on a snoop output one cycle later.
Each cache compares the snoop address against its tag array; if it hits
and the snoop source is not this cache's own `CORE_ID`, the line's valid
bit is cleared. Self-modifying code is supported "for free" because an
I-cache snoops the D-cache.

Note: this design uses an invalidation-only protocol (no MESI states),
write-through (so memory is always authoritative), and no write merging.
Coherence is correct but not optimal for write-heavy workloads.

---

## Multi-core, coherence, atomics

### Bus arbiter

Round-robin priority with a lock-pending override. The arbiter has
`N_PORTS = 2 * N_CORES` ports (I-cache and D-cache per core). Each cycle:

```
if (lock_pending)              winner = lock_holder
else if any port has req=1     winner = round-robin pick starting at rrobin
else                           no winner this cycle
```

On a granted request:

- The arbiter latches the request fields (`addr`, `wdata`, `be`, `we`,
  `lock`) one cycle later for the memory write/read.
- It advances the round-robin pointer past the winner (unless locked).
- If the granted request was locked (`lock[winner] = 1`) and we were not
  already lock-pending, the arbiter pins the bus to this port for one
  additional transaction. The next grant from the same port releases the
  lock.

Memory writes commit at the end of the granted cycle. Reads return data
one cycle later via `resp_valid` and `resp_data`.

### TAS atomicity

The 68000 `TAS` is a byte-level read-modify-write that sets the high bit:
read byte, set Z from the original value, write `byte | 0x80` back, all
atomically against other bus masters.

In this design, TAS executes as two consecutive locked bus transactions:

1. **Phase 0** — locked read of the full 32-bit word containing the target
   byte. The core asserts `cpu_lock = 1` so the D-cache bypasses any hit
   in its own line and goes to the bus. The arbiter grants and sets
   `lock_pending = 1, lock_holder = this_dcache`.
2. **Phase 1** — locked byte write: `byte | 0x80` to the same address with
   `cpu_be` set to the right byte lane. The arbiter sees `lock_pending` and
   grants this port unconditionally. The lock releases when this transaction
   completes.

While `lock_pending` is set, no other port can win the bus, so neither
another core's TAS nor an unrelated write can interleave with the RMW. The
CCR (N, Z) is computed from the original byte value (latched in phase 0)
and committed when phase 1 completes.

### Snoop and multi-core data sharing

The `t06_multicore` test exercises this end-to-end: two cores compete on
a TAS-protected shared counter and each performs five increments. Both
cores then poll the counter until it reaches 10 and STOP with code 0.
`t07_coherence` exercises the producer/consumer pattern where one core
writes a value and the other observes the snoop invalidation and reads
the fresh data from memory.

---

## Optimizations

Three frontend changes took `vs_68k` from 1.4×–1.6× to 3.6×–6.4× without
adding any backend complexity.

### 1. Combinational L1 cache hit

**Before.** The cache used a fully registered handshake: `cpu_req` in
cycle T, `cpu_ack` registered in cycle T+1. Even a hit cost 2 cycles
end-to-end.

**After.** On a read hit while idle and unlocked, the cache drives
`cpu_ack` and `cpu_rdata` combinationally:

```verilog
wire comb_read_hit = (state == S_IDLE) && cpu_req && !cpu_we && !cpu_lock && hit;
wire bus_resp_now  = (state == S_BUS_WAIT) && bus_resp_valid;
assign cpu_ack   = comb_read_hit || bus_resp_now;
assign cpu_rdata = comb_read_hit ? data[idx] :
                   bus_resp_now  ? bus_resp_data : 32'd0;
```

The gate on `!cpu_lock` is important — locked reads (TAS phase 0) must
always go through the bus so the arbiter sees the lock and serialises
the RMW. Without this gate, a cached TAS read would silently break
atomicity.

The cache also gained a fix to the bus protocol — see "One-grant-per-
transaction invariant" above.

### 2. Streaming IF (latch + dispatch + start next fetch in one cycle)

**Before.** IF was a fetch → wait-for-ack → latch → wait → dispatch
chain. With a 2-cycle cache, a simple instruction took 4 cycles from
issue to entering ID.

**After.** Every cycle that a fetch's ack arrives, the IF stage:

1. Latches the just-arrived half-word into the right slot.
2. Runs a combinational decoder on the **effective opcode** — the
   just-arrived word if this is the opcode latch, otherwise the
   registered `if_op`.
3. Combinationally computes `total_words` and `would_be_complete`.
4. If the instruction is complete and downstream is not stalled, drives
   `if_dispatch_w = 1` for ID/EX to latch, advances `if_pc` to the
   predicted next PC, and issues the next opcode fetch with the new
   `ic_addr` — all in the same posedge.

Steady state on tight loops: 1 instruction per cycle, fully overlapping
fetch and execute.

The matching ID/EX register has an `else if (!stall) ... id_op <=
effective_op` so the new opcode value reaches ID without an extra cycle.

### 3. BTFNT static branch predictor

At IF dispatch time, the predictor inspects the just-decoded opcode and
displacement:

```verilog
wire is_bcc_uncond   = is_bcc && (cc == CC_T || cc == CC_F);
wire is_bcc_cond     = is_bcc && !is_bcc_uncond;
wire predict_taken_w = is_bcc_uncond || (is_bcc_cond && disp32[31]);
```

If predicted taken, IF redirects to `pc + 2 + disp` in the same cycle as
the dispatch — the speculative fetch starts immediately on the predicted
path. The prediction bit rides through the pipeline; EX compares it
against `take_branch_c` (the resolved outcome) and only asserts
`redirect_valid` on a mispredict.

Result: tight loops mispredict only once (on the loop exit). For `sum`,
that's 1 mispredict out of 100 backward branches.

### 4. Combinational mispredict redirect (correctness fix)

With combinational dispatch, the wrong-path instruction could otherwise
make it into EX before a registered redirect signal fired. So
`redirect_valid` and `redirect_pc` are declared as wires and driven
combinationally from EX:

```verilog
wire bcc_mispred  = is_settled && (ex_kind == K_BCC) &&
                    (ex_predicted_taken != take_branch_c);
wire other_branch = is_settled && (ex_kind != K_BCC) && take_branch_c;
wire stop_now     = ex_valid && (ex_kind == K_STOP) && (ex_state == S_RUN) && !halted;
assign redirect_valid = bcc_mispred || other_branch || stop_now;
```

The ID/EX register reads this wire in the same posedge it computes its
next value, so a mispredicting branch wipes the in-flight wrong-path
instruction before it enters EX.

### 5. IF drain on redirect

If a speculative fetch is in flight to the I-cache when a branch
mispredicts in EX, the cache will still return that response a few
cycles later. The IF stage must not latch that response as the
redirected opcode (it was fetched from the predicted-but-wrong target).

A single-bit `if_drain` flag, set on a redirect when a fetch was
outstanding and cleared when the next ack arrives, gates `is_latching`:

```verilog
wire is_latching = if_busy && ic_ack && !if_drain;
```

The catch-all "start a new fetch" branch is similarly gated on
`!if_drain`, so the new redirected fetch only starts after the stale
in-flight response has been observed and discarded.

### Quantified contributions

The benchmark column `vs_slow` isolates what the caches alone are
buying (same pipeline both sides, the "slow" build has the bus
passthrough instead of caches). The difference between `vs_slow` and
`vs_68k` is roughly the pipelining + BTFNT contribution.

| benchmark | cpi initial | cpi final | speedup from start |
|-----------|-------------|-----------|---------------------|
| sum       | 6.54        | 1.56      | 4.19×               |
| fib       | 5.03        | 1.27      | 3.96×               |
| memcopy   | 5.77        | 1.94      | 2.97×               |
| jsr       | 8.05        | 3.07      | 2.62×               |

`jsr` improves least because every JSR/RTS still pays a flush (those
targets are not predicted statically in this design).

---

## Module reference

### `rtl/m68k_defs.vh`

Macros for ALU op encodings (`ALU_ADD`, `ALU_SUB`, …), addressing-mode
constants (`EA_DREG`, `EA_AIND`, …), and branch condition codes
(`CC_HI`, `CC_EQ`, …). Also defines `RESET_PC = 0x0000_0400`.

### `rtl/m68k_alu.v`

Combinational 32-bit ALU. Inputs: 4-bit `op`, two 32-bit operands.
Outputs: 32-bit `y` and four flag bits (N, Z, V, C) computed per the
M68000 specification. Handles `MOV/PASSB`, `ADD`, `SUB`, `AND`, `OR`,
`EOR`, `CMP` (sub without writeback), and `TAS` (returns operand | 0x80
with flags from the original byte).

### `rtl/m68k_regfile.v`

16 × 32-bit register file: indices 0..7 are D0..D7, 8..15 are A0..A7.

- 3 combinational read ports (ra, rb, rc) used for the source EA base,
  the Dn-operand-or-MOVE-dst-base, and SP respectively.
- 1 main write port + 1 aux write port. On a same-cycle conflict, the
  main port wins.
- Reset initialises A7 (idx 15) from the per-core `reset_a7` input.

### `rtl/m68k_decoder.v`

Pure-combinational instruction decoder. Input: 16-bit opcode. Outputs:
instruction `kind` enum, ALU op selector, source and destination EA
fields, branch condition, sign-extended immediate, and the number of
extension words on each side.

Used twice in the core: once in IF (on the streaming "effective opcode")
to compute `total_words` and the branch prediction, and once in ID for
the full operand-selection decode.

### `rtl/m68k_cache.v`

Direct-mapped L1 cache with combinational read hit, write-through,
snoop-invalidate, and lock-aware bypass. Parameters:

- `NUM_LINES` (default 256)
- `ID_BITS` and `CORE_ID` (used by the snoop logic to ignore own writes)
- `WRITABLE` (0 for I-cache, 1 for D-cache)

### `rtl/m68k_passthrough.v`

Pin-compatible alternative to `m68k_cache.v` with no storage. Every CPU
request becomes a bus request. Used when `m68k_top` is built with
`USE_CACHE = 0` to construct the "slow" reference build for benchmarks.

### `rtl/m68k_bus.v`

Round-robin shared bus arbiter with a register-array memory underneath.

- `N_PORTS` requesters, picked combinationally using a rotating priority.
- Memory has `MEM_WORDS` 32-bit entries with per-byte enables.
- Memory is initialised at sim start by `$readmemh(MEM_HEXFILE, mem)`
  (default `"program.hex"`).
- Lock support: on a granted locked request, the bus pins itself to the
  same port for one additional grant.
- Snoop broadcast: every committed write is republished one cycle later
  on `snoop_valid`/`snoop_addr`/`snoop_src_id` so peer caches can
  invalidate.

### `rtl/m68k_core.v`

The pipelined core. Organised into commented sections:

1. **IF stage** — combinational streaming logic + state registers.
2. **IF/ID register** — combinational-dispatch latch.
3. **ID stage** — combinational decoder, regfile reads, forwarding mux.
4. **ID/EX register** — latched operands and prediction bit.
5. **EX stage** — EA computation, ALU, memory state machine, branch
   resolution, writeback, CCR commit, halt logic, retired counter.

Exposes I-cache and D-cache ports, halt status, halt code, and a 32-bit
`retired` instruction counter for benchmarking.

### `rtl/m68k_top.v`

Wires `N_CORES` cores, their per-core I/D caches (or passthroughs), the
shared bus arbiter, and the snoop broadcast. Flattens halt and retired
status into wide output buses for the testbench. Parameters:

- `N_CORES`, `USE_CACHE`, `MEM_WORDS`, `MEM_HEXFILE`.

---

## Verification methodology

### Toolchain

- **Verilator 5.x** simulates the design.
- **`tb/asm68k.py`** is a single-file Python assembler that recognises the
  68K-mini subset and emits a `program.hex` file suitable for `$readmemh`.
  It supports `.org`, `.word`, `.long`, labels, and signed 8/16-bit branch
  displacements.
- **`tb/sim_main.cpp`** is the Verilator C++ harness. It pulses the clock,
  releases reset, runs until every core halts or a cycle cap is reached,
  and prints per-core cycles, retired instructions, IPC, and halt code.
- **`tb/bench_report.py`** runs every `bench/*.s` on both the cached and
  no-cache builds, parses the simulator output, and prints the comparison
  table.

### Test format

Test programs are written in 68K-mini assembly and live in `tests/` (regression)
or `bench/` (performance). Each test ends with `STOP #0` for pass and
`STOP #<nonzero>` for a specific failure code. The harness inspects the
halt code; a 0 means the test passed.

### Regression suite

| test            | covers                                                    |
|-----------------|-----------------------------------------------------------|
| `t01_basic`     | reset, NOP, STOP                                          |
| `t02_arith`     | MOVEQ, ADD, SUB, CMP+BEQ, AND                             |
| `t03_loop`      | ADDQ, signed-conditional branch loop                       |
| `t04_memory`    | (An)+, -(An), per-core branch by SP                       |
| `t05_call`      | BSR/RTS, JSR abs.L, recursion + stack                     |
| `t06_multicore` | TAS-protected shared counter, two cores                   |
| `t07_coherence` | producer/consumer ping-pong via shared memory + snoop     |

All seven pass under both `USE_CACHE = 1` (cached build) and the no-cache
build, with both `N_CORES = 1` and `N_CORES = 2`.

### Benchmark methodology

Each `bench/*.s` file carries a `; basic_68000_cycles=N` header where `N`
is the sum of the canonical M68000 PRM instruction execution times for
the dynamic instruction trace of that program. For example, the `sum`
benchmark's prologue is `2 × MOVEQ = 8 cycles`, then 99 taken iterations
of `ADD.L Dn,Dm(8) + ADDQ.L(8) + CMPI.L(14) + Bcc-T(10) = 40 cycles`,
plus a last not-taken iteration at 38, plus a 28-cycle epilogue. Total
4034 cycles.

`bench_report.py` runs each benchmark on both `build_fast` (cached +
pipelined) and `build_slow` (passthrough) and prints:

- `retired`  — instructions retired on our core (same on both builds).
- `68k_ref`  — canonical M68000 cycles for the same trace.
- `cpi_ref`  — `68k_ref / retired`.
- `fast`, `slow` — our cycle counts.
- `cpi_fast`, `cpi_slow` — our CPI on each build.
- `vs_68k`  — `68k_ref / fast`. Headline speedup.
- `vs_slow` — `slow / fast`. Cache contribution alone.

---

## Benchmarks

| name      | what it stresses                                          |
|-----------|-----------------------------------------------------------|
| `sum`     | tight ALU loop: ADD + ADDQ + CMP + Bcc                    |
| `fib`     | iterative Fibonacci, all register-resident                |
| `memcopy` | streaming loads/stores through `(An)+`                    |
| `jsr`     | JSR abs.L + RTS overhead, 50 subroutine calls             |

Each is verified to compute the correct answer (the program halts with
code 0 on success and a nonzero code on failure), and the reference
cycle count is hand-derived in the header comment.

---

## Build and run

### Prerequisites

- **Verilator 5.x** in `$PATH` (`verilator --version` should report ≥ 5.0).
- **Python 3** for the assembler and the bench runner.
- A C++17 compiler reachable by Verilator (clang or g++).

### Make targets

```
make test                 # default 2 cores, builds & runs every regression test
make bench                # builds two 1-core sims (cache on/off) and prints the
                          # comparison table against canonical 68000 cycles
make N_CORES=4 build      # rebuild the multi-core sim for 4 cores
make clean                # nuke build/ build_fast/ build_slow/ and the logs
```

### Configuration knobs

- `N_CORES` (Makefile variable, default 2). Controls how many cores
  `m68k_top` instantiates. Tested at 1, 2, and 4.
- `USE_CACHE` (Makefile variable, default 1). When 0, `m68k_top` uses
  `m68k_passthrough` shims instead of the L1 caches. Used by the `bench`
  target to construct the "slow" reference build.
- `BUILD` (Makefile variable, default `build`). Sets the Verilator output
  directory; the `bench` target uses `build_fast` and `build_slow`.

### How to read a successful sim run

```
$ ./build/Vm68k_top 200000
[sim] cycles=305 halted_mask=0x3
[sim] core0 halted=1 code=0x0000 retired=44 ipc=0.144
[sim] core1 halted=1 code=0x0000 retired=44 ipc=0.144
```

- `halted_mask`: bit per core; all bits set means every core reached its
  STOP. The harness exits 0 only in that case with every halt code = 0.
- `code=0xNNNN`: the immediate operand of the STOP that halted this core.
  By convention, 0 is pass, anything else is a specific failure marker
  the program chose.
- `retired`: instructions completed by this core, excluding the final
  STOP. `ipc = retired / cycles`.

The harness's exit code is the first nonzero halt code, or `0xFFFE` on
timeout, or 0 on full success.

### Running a single test or benchmark by hand

```
# assemble
python3 tb/asm68k.py tests/t03_loop.s build/program.hex

# run
(cd build && ./Vm68k_top 50000)
```

### Adding a new test

1. Write a `.s` file in `tests/` or `bench/` ending with `STOP #0` on
   pass and a chosen-immediate STOP on each failure path.
2. For a benchmark, add a `; basic_68000_cycles=N` header line with the
   hand-derived reference count.
3. `make test` (or `make bench`) will pick it up automatically — the
   Makefile globs both directories.

---

## ISA implementation status

This implementation is being expanded toward a complete 68000. The
foundation pieces — size-aware datapath (.B/.W/.L), all 8 addressing
modes except indexed (`d8(An,Xn)`/`d8(PC,Xn)`), and an X-flag-tracking
ALU — are in place. Many instruction kinds beyond the original subset
are decoded but not yet fully executed; see the table below.

### Decoded and executed

| category                         | instructions / variants                                          |
|----------------------------------|------------------------------------------------------------------|
| data movement                    | MOVE.{B,W,L}, MOVEA.{W,L} (with sign-ext for .W), MOVEQ           |
| integer arithmetic (reg dest)    | ADD, SUB, AND, OR, EOR, CMP (.B/.W/.L); ADDA/SUBA/CMPA            |
| immediate arithmetic (Dn dest)   | ADDI, SUBI, ANDI, ORI, EORI, CMPI                                 |
| immediate quick                  | ADDQ, SUBQ                                                        |
| single-operand (Dn dest)         | CLR, TST, NEG, NOT                                                |
| sign-extend / swap               | EXT.W, EXT.L, SWAP                                                |
| register exchange                | EXG (D-D, A-A, D-A)                                               |
| program control                  | Bcc, BRA, BSR, JMP, JSR, RTS, NOP, STOP                            |
| address calculation              | LEA                                                               |
| atomic                           | TAS                                                               |

### Decoded but NOT yet executed

Decoder maps these correctly; the EX stage either handles only register
destinations or does not yet implement them at all. They are present so
the assembler can emit them and the pipeline can dispatch them — but
they will produce incorrect results until each EX case is filled in.

| category                                | what works / what's missing                                |
|-----------------------------------------|------------------------------------------------------------|
| ALUI / CLR / TST / NEG / NOT to memory  | needs read-modify-write state (not yet wired)              |
| LINK / UNLK / PEA                       | decoder only; EX stub                                      |
| MOVEM                                   | decoder only; needs multi-cycle reg-mask iterator           |
| Shifts and rotates (ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR) | ALU has the math; EX integration not yet wired |
| Bit ops (BTST/BCHG/BCLR/BSET)           | decoder only; EX stub                                      |
| DBcc, Scc, CHK                          | decoder only; EX stub                                      |
| MULU / MULS / DIVU / DIVS               | decoder only; EX stub                                      |
| MOVE to/from SR/CCR, MOVE USP           | decoder only; needs SR register and supervisor mode         |
| TRAP, TRAPV, RTE, RTR, ILLEGAL          | decoder only; needs exception model                         |

### Not yet decoded

- `d8(An,Xn)` and `d8(PC,Xn)` addressing modes — require a fourth
  regfile read port for the index register.
- ABCD, SBCD, NBCD (BCD operations) — niche, skipped on purpose.
- ADDX, SUBX, NEGX (multiprecision extend) — skipped on purpose.
- MOVEP (move peripheral byte sequence) — skipped on purpose.
- RESET instruction (hardware-specific) — skipped on purpose.
- Trace exception, bus error / address error — skipped on purpose.

### Architectural pieces still TODO for a real 68000

- **Supervisor mode**: dual stack pointers (USP/SSP), S bit in SR,
  privilege violation traps. None of this is wired up.
- **Status register**: only the CCR low byte (N, Z, V, C, X) is tracked;
  the upper byte (T, S, I0..I2) does not exist.
- **Exception model**: no vector table at memory address 0, no
  exception stack frame, no exception entry / RTE round-trip.
- **Interrupts**: no IPL inputs to the core, no interrupt acknowledge
  bus cycle.
- **Reset vector from memory**: SP and PC are loaded from parameters,
  not from `mem[0]` and `mem[4]`.

## Other implementation choices

- **No floating point.**
- **64 KB physical address space.** `MEM_WORDS = 16384`, expandable by
  parameter.
- **No real timing/closure work.** Combinational paths through the cache
  hit, branch redirect, and EX writeback are long; on real silicon, you
  would want to retime several of these.
- **JSR/RTS unpredicted.** The branch predictor is BTFNT for Bcc only.
  Indirect branches still cause a 2-cycle flush.
- **Direct-mapped, 1-word-line caches.** No spatial locality benefit
  beyond the natural 32-bit word; no associativity (conflict misses are
  possible if a program straddles two addresses with the same index).
- **TAS only.** No CAS, LL/SC, or LOCK prefix; multi-core synchronisation
  primitives are limited to test-and-set.

---

## Future work

The frontend changes already eat most of the easy speedup. The next
tier of wins, in roughly increasing complexity:

1. **Wider fetch + prefetch buffer** — the cache returns 32 bits per
   access but the IF currently throws half of it away. Buffering both
   half-words and pre-decoding the second would push the front-end
   toward 2 instructions/cycle.
2. **Bigger cache lines** (e.g. 16 B with critical-word-first fill) —
   amortise bus round-trips and gain spatial locality on streaming
   loads.
3. **Branch target buffer + 2-bit saturating-counter predictor** —
   accurate for irregular branches; also predicts JSR/RTS targets.
4. **Hit-under-miss D-cache (MSHRs)** — let the pipeline keep running
   past an outstanding load if the next instruction doesn't depend on
   the result.
5. **Macro-op fusion of `CMP + Bcc`** — the dominant idiom in our loops;
   fusing it into a single EX op halves the per-iteration cycle count.
6. **Single-cycle multiplier / shifter ALU** — repeated-addition
   multiplication is a real cost in `square` and similar programs.
7. **Out-of-order or scoreboarded issue** — would shed the load-use
   penalty entirely for unrelated downstream instructions.
8. **Superscalar issue** — dual-issue independent instructions from a
   small prefetch buffer.

The largest wins lie outside the architecture entirely (better
algorithms, vector data paths, SMT) and would be a different project.

---

## File map

```
fast_68000/
├── DESIGN.md                  (this file)
├── ISA.md                     instruction encoding reference
├── README.md                  quickstart, sample output
├── Makefile                   build + run targets
│
├── rtl/                       synthesisable Verilog
│   ├── m68k_defs.vh           shared macros
│   ├── m68k_alu.v             32-bit ALU + flags
│   ├── m68k_regfile.v         16-entry, 3R/2W register file
│   ├── m68k_decoder.v         16-bit opcode → control bundle
│   ├── m68k_cache.v           direct-mapped, write-through, snooping L1
│   ├── m68k_passthrough.v     pin-compatible no-cache shim (USE_CACHE=0)
│   ├── m68k_bus.v             arbiter + memory + snoop
│   ├── m68k_core.v            3-stage pipelined core
│   └── m68k_top.v             N-core SoC top
│
├── tb/                        testbench infrastructure
│   ├── asm68k.py              68K-mini assembler → program.hex
│   ├── sim_main.cpp           Verilator C++ harness
│   └── bench_report.py        fast vs. slow vs. 68000-PRM table
│
├── tests/                     regression programs (.s)
│   └── t01_basic.s … t07_coherence.s
│
└── bench/                     benchmark programs with reference cycle counts
    ├── fib.s
    ├── jsr.s
    ├── memcopy.s
    └── sum.s
```
