# 68K-mini ISA

A subset of the Motorola 68000 user-mode ISA used by this implementation.
32-bit datapath; all arithmetic is .L (long) only.

## Registers
- `D0..D7` — 32-bit data registers
- `A0..A7` — 32-bit address registers (A7 is the stack pointer)
- `PC`     — 32-bit program counter
- `CCR`    — condition codes: N, Z, V, C

## Addressing modes (mode:reg, 6 bits)
| mode | reg   | meaning             |
|------|-------|---------------------|
| 000  | r     | Dr                  |
| 001  | r     | Ar                  |
| 010  | r     | (Ar)                |
| 011  | r     | (Ar)+               |
| 100  | r     | -(Ar)               |
| 111  | 000   | abs.W (1 ext word)  |
| 111  | 001   | abs.L (2 ext words) |
| 111  | 100   | #imm.L (2 ext words) |

## Instructions

All opcodes are 16 bits, MSB-first. Extension words follow as required by the
addressing mode(s).

| mnemonic | encoding              | semantics                              |
|----------|-----------------------|----------------------------------------|
| NOP      | 0100_1110_0111_0001   | no operation                           |
| RTS      | 0100_1110_0111_0101   | PC = (SP)+; SP+=4                       |
| STOP     | 0100_1110_0111_0010 + imm16 | halt CPU; testbench reads imm16 as exit status |
| JMP      | 0100_1110_11_mm_rrr   | PC = EA                                 |
| JSR      | 0100_1110_10_mm_rrr   | -(SP)=PC; PC=EA                         |
| LEA      | 0100_aaa_111_mm_rrr   | Aa = EA                                 |
| MOVEQ    | 0111_ddd_0_iiiiiiii   | Dd = sign_ext(imm8)                     |
| MOVE.L   | 0010_DDDmmm_MMMrrr    | dst_ea = src_ea; bits[11:9]=destReg, [8:6]=destMode |
| ADDQ.L   | 0101_iii_010_mm_rrr   | EA += (i==0?8:i)                        |
| SUBQ.L   | 0101_iii_110_mm_rrr   | EA -= (i==0?8:i)                        |
| ADD.L    | 1101_ddd_010_mm_sss   | Dd = Dd + EA                            |
| ADD.L    | 1101_ddd_110_mm_sss   | EA = EA + Dd                            |
| SUB.L    | 1001_ddd_010_mm_sss   | Dd = Dd - EA                            |
| AND.L    | 1100_ddd_010_mm_sss   | Dd = Dd & EA                            |
| OR.L     | 1000_ddd_010_mm_sss   | Dd = Dd \| EA                           |
| EOR.L    | 1011_ddd_110_mm_sss   | EA = EA ^ Dd                            |
| CMP.L    | 1011_ddd_010_mm_sss   | flags from Dd - EA                      |
| TAS      | 0100_1010_11_mm_rrr   | atomic byte test-and-set on EA (low byte) |
| Bcc      | 0110_cccc_dddddddd    | conditional branch; if d8==0 use next word as disp16 |
| BRA      | 0110_0000_dddddddd    | unconditional branch                    |
| BSR      | 0110_0001_dddddddd    | -(SP)=PC_next; PC += disp               |

## Pipeline
5 stages: IF, ID, EX, MEM, WB. Hazards resolved by forwarding (EX→EX, MEM→EX);
load-use stalls one cycle. Branches predicted not-taken; misprediction flushes
IF and ID (2-cycle penalty).

## Memory subsystem
- Per-core L1 I-cache and D-cache: 256 lines × 4 bytes, direct-mapped.
- D-cache is write-through, no-write-allocate. Other cores snoop the bus and
  invalidate matching lines on writes.
- Round-robin arbiter on the shared memory bus. `TAS` raises `bus_lock` so the
  RMW completes atomically.

## Reset and exit
- All cores start at PC = `0x0000_0400`. Stack pointers come from per-core
  reset table at `0x0000_0000 + core_id*4`.
- A `STOP #imm` halts that core; the testbench inspects the immediate as the
  exit code. Convention: 0x0000 = success, anything else = failing test ID.
