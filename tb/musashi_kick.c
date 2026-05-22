/* Kickstart-boot cosim harness for Musashi.
 *
 * Boots a 256 KB Kickstart 1.x ROM (loaded at $FC0000 + mirrored at
 * $F80000) and emits a per-instruction trace: retired index, PC,
 * opcode, D0..D7, A0..A7, SR.  The format matches what the Verilator
 * sim emits with KICKSTART_COSIM_TRACE so the two logs can be diff'd
 * to find the first instruction where Musashi disagrees with us --
 * that's a CPU bug.
 *
 * Chipset reads are stubbed to match the constants the Verilator bus
 * returns: SERDATR=$307F, POTGOR=$FFFF, ADKCONR/DMACONR/INTREQR/etc=0.
 * VPOSR/VHPOSR increment per fake cycle.  The set is intentionally
 * minimal -- if Musashi diverges on a chipset read we haven't covered,
 * we'll see the read in the trace and extend the stub.
 */

#include "m68k.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MEM_SIZE 0x1000000   /* 16 MiB flat */
#define ROM_BASE 0x00FC0000
#define ROM_MIRROR 0x00F80000
#define ROM_SIZE 0x40000     /* 256 KB K1.3 */
/* Match Verilator's kickstart-boot build: MEM_WORDS=131072 long-words
 * = 512 KB of chip RAM at $0-$7FFFF.  Writes at higher addresses are
 * dropped on the bus side; we mirror that here. */
#define CHIP_RAM_SIZE 0x80000

static uint8_t mem[MEM_SIZE];
static uint8_t rom[ROM_SIZE];

/* Tiny chipset shadow -- write-tracking for the registers Musashi reads
 * back via the *R mirrors ($DFF002 DMACONR, $DFF01C INTENAR, etc.).
 * Everything else returns the constant the Verilator bus returns. */
static uint16_t dmacon, intena, intreq, adkcon;
static uint64_t vpos_counter;     /* tick per cycle ~PAL-ish */

static int trace_enabled = 0;
static FILE* trace_fp = NULL;
static int wtrace_enabled = 0;
static FILE* wtrace_fp = NULL;
static uint64_t retired_count = 0;

/* Helper: byte-addressed BE read of mem[]. */
static uint32_t mem_read8(uint32_t addr) {
    return mem[addr & (MEM_SIZE - 1)];
}
static uint32_t mem_read16(uint32_t addr) {
    addr &= MEM_SIZE - 1;
    return ((uint32_t)mem[addr] << 8) | mem[(addr + 1) & (MEM_SIZE - 1)];
}

/* Chipset reads.  Address range $DFF000-$DFFFFF.  Returns 0xFFFFFFFF
 * if not handled (caller falls back to mem[] read). */
static uint32_t chipset_read16(uint32_t addr) {
    uint32_t reg = addr & 0x1FE;
    /* VPOSR: bit 0 of high byte = V8.  Low byte = V7..V0.  V increments
     * every ~226 cycles (one scanline).  PAL frame = 313 lines. */
    if (reg == 0x004) {  /* VPOSR */
        uint32_t v = (vpos_counter / 226) % 313;
        uint32_t lof = (vpos_counter / (226 * 313)) & 1;
        return (lof << 15) | ((v >> 8) & 1);
    }
    if (reg == 0x006) {  /* VHPOSR */
        uint32_t v = (vpos_counter / 226) % 313;
        uint32_t h = (vpos_counter % 226);
        return ((v & 0xFF) << 8) | (h & 0xFF);
    }
    if (reg == 0x002) return dmacon & 0x7FFF;        /* DMACONR */
    if (reg == 0x01C) return intena & 0x7FFF;        /* INTENAR */
    if (reg == 0x01E) return intreq & 0x7FFF;        /* INTREQR */
    if (reg == 0x010) return adkcon;                 /* ADKCONR */
    if (reg == 0x018) return 0x307F;                 /* SERDATR */
    if (reg == 0x016) return 0xFFFF;                 /* POTGOR  */
    if (reg == 0x012 || reg == 0x014) return 0;      /* POT0DAT / POT1DAT */
    if (reg == 0x00A || reg == 0x00C) return 0;      /* JOY0DAT / JOY1DAT */
    if (reg == 0x000) return 0xFFFF;                 /* BLTDDAT */
    if (reg == 0x008) return 0;                      /* DSKDATR */
    return 0;  /* default: unmapped chipset reads return 0 */
}

/* Chipset writes update the shadows for SET/CLR-style registers. */
static void chipset_write16(uint32_t addr, uint16_t val) {
    uint32_t reg = addr & 0x1FE;
    if (reg == 0x096) {  /* DMACON: bit 15 SET, else CLR */
        if (val & 0x8000) dmacon |= (val & 0x7FFF);
        else              dmacon &= ~(val & 0x7FFF);
    } else if (reg == 0x09A) {  /* INTENA */
        if (val & 0x8000) intena |= (val & 0x7FFF);
        else              intena &= ~(val & 0x7FFF);
    } else if (reg == 0x09C) {  /* INTREQ */
        if (val & 0x8000) intreq |= (val & 0x7FFF);
        else              intreq &= ~(val & 0x7FFF);
    } else if (reg == 0x09E) {  /* ADKCON */
        if (val & 0x8000) adkcon |= (val & 0x7FFF);
        else              adkcon &= ~(val & 0x7FFF);
    }
    /* Other writes are dropped (we don't simulate display/DMA effects). */
}

/* Memory regions mirroring our Verilator bus exactly:
 *   $000000-$03FFFF : 256 KB chip RAM (matches MEM_WORDS=65536)
 *   $BF0000-$BFFFFF : CIA-A/B (return $FF, idle)
 *   $DFF000-$DFFFFF : custom chipset
 *   $E80000-$E8FFFF : autoconfig (return $FF, no board)
 *   $F80000-$FFFFFF : 256 KB Kickstart ROM (mirrored)
 *   everything else : reads return 0, writes dropped (matches our
 *                     bus's "unmapped fallthrough returns $0").
 *
 * Without the unmapped-write drop, Musashi diverges from Verilator
 * the moment Kickstart probes memory above 256 KB (e.g. the FAST-RAM
 * sizing scan that walks into the autoconfig region) -- Musashi
 * keeps the data, Verilator forgets it, and the next compare-back
 * picks different flags. */
static uint32_t handled_read(uint32_t addr, int size) {
    addr &= 0x00FFFFFF;
    /* Chipset $DFF000-$DFFFFF */
    if (addr >= 0x00DFF000 && addr < 0x00E00000) {
        uint32_t aligned = addr & ~1u;
        uint32_t w = chipset_read16(aligned);
        if (size == 2) return w;
        if (size == 1) return (addr & 1) ? (w & 0xFF) : ((w >> 8) & 0xFF);
        uint32_t lo = chipset_read16(aligned + 2);
        return (w << 16) | lo;
    }
    if (addr >= 0x00F80000 && addr <= 0x00FFFFFF) {
        uint32_t off = (addr - 0x00F80000) & (ROM_SIZE - 1);
        if (size == 1) return rom[off];
        if (size == 2) return ((uint32_t)rom[off] << 8) | rom[off + 1];
        return ((uint32_t)rom[off] << 24) |
               ((uint32_t)rom[(off + 1) & (ROM_SIZE - 1)] << 16) |
               ((uint32_t)rom[(off + 2) & (ROM_SIZE - 1)] << 8) |
               (uint32_t)rom[(off + 3) & (ROM_SIZE - 1)];
    }
    if (addr >= 0x00BF0000 && addr < 0x00C00000) {
        /* CIA region.  Mirror Verilator's chipset/cia.v:
         *   - CIA-A is at $BFEx01 (bit-13 set in $BFE000-$BFEFFF).
         *     pa_in = 8'b1100_1000 = $C8; pra=0, ddra=0 at reset.
         *     PRA read = (pra & ddra) | (pa_in & ~ddra) = $C8.
         *   - CIA-B is at $BFDx00 (bit-12 clear).  pa_in = 8'd0,
         *     so PRA read = 0.
         *   - All other registers reset to 0.
         * Register index from addr[11:8] (every 256-byte stride).
         * Reads at unmapped offsets float to $FF on real hw; we use
         * $FF only when the address is *outside* a CIA register stride
         * to keep Kickstart's autoconfig sizing happy. */
        int is_ciaa = (addr & 0x00F00000) == 0x00B00000 &&
                      (addr & 0x0000F000) == 0x0000E000;
        int is_ciab = (addr & 0x00F00000) == 0x00B00000 &&
                      (addr & 0x0000F000) == 0x0000D000;
        int reg = (addr >> 8) & 0xF;
        uint8_t b;
        if (is_ciaa && reg == 0)       b = 0xC8;  /* PRA  */
        else if (is_ciab && reg == 0)  b = 0x00;  /* PRA  */
        else if (is_ciaa || is_ciab)   b = 0x00;  /* other CIA regs */
        else                            b = 0xFF;  /* unmapped */
        if (size == 1) return b;
        if (size == 2) return ((uint32_t)b << 8) | b;
        return ((uint32_t)b << 24) | ((uint32_t)b << 16) | ((uint32_t)b << 8) | b;
    }
    if (addr >= 0x00E80000 && addr < 0x00E90000) {
        /* Mirror Verilator's autoconfig stub: only the type byte at
         * offset $00 returns $FF ("no card here"); all other autoconfig
         * register offsets read as $00.  Kickstart 1.3's autoconfig
         * walker reads multiple offsets and the K-LSR.B-mask checksum
         * loop diverges if we lie about offset $04. */
        uint32_t off = addr & 0xFF;
        uint8_t b = (off == 0x00) ? 0xFF : 0x00;
        if (size == 1) return b;
        if (size == 2) {
            uint32_t off2 = (addr + 1) & 0xFF;
            uint8_t b2 = (off2 == 0x00) ? 0xFF : 0x00;
            return ((uint32_t)b << 8) | b2;
        }
        /* size == 4: 4 bytes from the autoconfig stub */
        uint32_t v = 0;
        for (int i = 0; i < 4; i++) {
            uint32_t o = (addr + i) & 0xFF;
            uint8_t bi = (o == 0x00) ? 0xFF : 0x00;
            v = (v << 8) | bi;
        }
        return v;
    }
    /* Chip RAM: only the populated 256 KB matches Verilator.  Higher
     * chip-RAM addresses ($040000-$BEFFFF, $C00000-$DFEFFF) read as 0
     * on our bus, so they must read as 0 here too. */
    if (addr < CHIP_RAM_SIZE) {
        if (size == 1) return mem[addr];
        if (size == 2) return ((uint32_t)mem[addr] << 8) |
                              mem[(addr + 1) & (MEM_SIZE - 1)];
        return ((uint32_t)mem[addr] << 24) |
               ((uint32_t)mem[(addr + 1) & (MEM_SIZE - 1)] << 16) |
               ((uint32_t)mem[(addr + 2) & (MEM_SIZE - 1)] << 8) |
               mem[(addr + 3) & (MEM_SIZE - 1)];
    }
    /* Unmapped: matches Verilator's "(granted_addr_q >= MEM_WORDS<<2) ? 0". */
    return 0;
}

static void handled_write(uint32_t addr, uint32_t val, int size) {
    addr &= 0x00FFFFFF;
    if (wtrace_enabled) {
        /* Format matches Verilator: `<retired> <addr> <size> <val>`.
         * Writes occur during instruction N's execution, but our
         * instr_hook already incremented retired_count to N+1 before
         * the write callback fires -- subtract one to align with
         * Verilator's "retired index of the writing instruction". */
        if (addr < CHIP_RAM_SIZE)
            fprintf(wtrace_fp, "%llu %06x %d %0*x\n",
                    (unsigned long long)(retired_count - 1), addr, size,
                    size * 2, val & (size == 1 ? 0xFFu : size == 2 ? 0xFFFFu : 0xFFFFFFFFu));
    }
    if (addr >= 0x00DFF000 && addr < 0x00E00000) {
        if (size == 2) {
            chipset_write16(addr & ~1u, val & 0xFFFF);
        } else if (size == 4) {
            chipset_write16(addr & ~1u, (val >> 16) & 0xFFFF);
            chipset_write16((addr & ~1u) + 2, val & 0xFFFF);
        }
        return;
    }
    if (addr >= 0x00F80000 && addr <= 0x00FFFFFF) return;  /* ROM read-only */
    /* Only writes inside the populated 256 KB chip RAM stick; everything
     * else (FAST-RAM sizing scans, expansion ROM probes, etc.) is dropped
     * the same way Verilator drops them. */
    if (addr + (uint32_t)size > CHIP_RAM_SIZE) return;
    if (size == 1) { mem[addr] = val & 0xFF; return; }
    if (size == 2) {
        mem[addr]                       = (val >> 8) & 0xFF;
        mem[(addr + 1) & (MEM_SIZE - 1)] = val & 0xFF;
        return;
    }
    mem[addr]                       = (val >> 24) & 0xFF;
    mem[(addr + 1) & (MEM_SIZE - 1)] = (val >> 16) & 0xFF;
    mem[(addr + 2) & (MEM_SIZE - 1)] = (val >> 8)  & 0xFF;
    mem[(addr + 3) & (MEM_SIZE - 1)] = val & 0xFF;
}

/* Musashi callbacks. */
unsigned int m68k_read_memory_8(unsigned int addr)  { return handled_read(addr, 1); }
unsigned int m68k_read_memory_16(unsigned int addr) { return handled_read(addr, 2); }
unsigned int m68k_read_memory_32(unsigned int addr) { return handled_read(addr, 4); }
void m68k_write_memory_8 (unsigned int addr, unsigned int v) { handled_write(addr, v, 1); }
void m68k_write_memory_16(unsigned int addr, unsigned int v) { handled_write(addr, v, 2); }
void m68k_write_memory_32(unsigned int addr, unsigned int v) { handled_write(addr, v, 4); }
unsigned int m68k_read_disassembler_8 (unsigned int a) { return m68k_read_memory_8(a); }
unsigned int m68k_read_disassembler_16(unsigned int a) { return m68k_read_memory_16(a); }
unsigned int m68k_read_disassembler_32(unsigned int a) { return m68k_read_memory_32(a); }

static void instr_hook(unsigned int pc) {
    if (!trace_enabled) return;
    uint32_t op = m68k_read_memory_16(pc);
    fprintf(trace_fp, "%llu %06x %04x", (unsigned long long)retired_count, pc, op);
    for (int i = 0; i < 8; i++)
        fprintf(trace_fp, " %08x", m68k_get_reg(NULL, M68K_REG_D0 + i));
    for (int i = 0; i < 8; i++)
        fprintf(trace_fp, " %08x", m68k_get_reg(NULL, M68K_REG_A0 + i));
    fprintf(trace_fp, " %04x\n", m68k_get_reg(NULL, M68K_REG_SR) & 0xFFFF);
    retired_count++;
}

static int load_rom(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return -1;
    size_t n = fread(rom, 1, ROM_SIZE, f);
    fclose(f);
    return (n == ROM_SIZE) ? 0 : -1;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s rom.bin [max_instructions] [trace.log]\n", argv[0]);
        return 1;
    }
    if (load_rom(argv[1]) < 0) {
        fprintf(stderr, "musashi-kick: failed to load %s\n", argv[1]);
        return 1;
    }
    long max_instr = (argc >= 3) ? strtol(argv[2], NULL, 10) : 5000000L;
    const char* trace_path = (argc >= 4) ? argv[3] : NULL;
    if (trace_path) {
        trace_fp = fopen(trace_path, "w");
        if (!trace_fp) {
            fprintf(stderr, "failed to open trace file %s\n", trace_path);
            return 1;
        }
        trace_enabled = 1;
    }
    const char* wtrace_path = (argc >= 5) ? argv[4] : NULL;
    if (wtrace_path) {
        wtrace_fp = fopen(wtrace_path, "w");
        if (!wtrace_fp) {
            fprintf(stderr, "failed to open write-trace file %s\n", wtrace_path);
            return 1;
        }
        wtrace_enabled = 1;
    }

    /* Match Verilator's reset semantics exactly:
     *   - chip RAM at $0..$7 stays zero (real Amiga: OVL=1 maps ROM
     *     there at reset, but once K1.3 has cleared OVL via CIA-A the
     *     space becomes uninitialised chip RAM = 0 on a cold boot);
     *   - the CPU's initial SSP and PC are forced to match our
     *     Verilator harness (SSP = $00004000, PC = $00FC00D2) so the
     *     first retired instruction (LEA $40000, A7) operates from
     *     the same state on both sides.
     *
     * Musashi's m68k_pulse_reset reads SSP from mem[$0] and PC from
     * mem[$4]; since those are zero we override afterwards via
     * m68k_set_reg.  Without this, Musashi would jump to PC=0 and
     * fault immediately. */
    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000);
    m68k_set_instr_hook_callback(instr_hook);
    m68k_pulse_reset();
    m68k_set_reg(M68K_REG_PC, 0x00FC00D2);
    m68k_set_reg(M68K_REG_SP, 0x00004000);
    /* Clear the X flag so SR matches Verilator's reset SR = $2700
     * (supervisor mode, IRQ mask = 7, T=0, C/V/Z/N/X all clear). */
    m68k_set_reg(M68K_REG_SR, 0x2700);

    long total = 0;
    while ((long)retired_count < max_instr) {
        int ran = m68k_execute(1000);
        if (ran <= 0) break;
        total += ran;
        vpos_counter += ran;
    }

    if (trace_fp) fclose(trace_fp);
    if (wtrace_fp) fclose(wtrace_fp);
    fprintf(stderr, "[musashi-kick] retired=%llu cycles=%ld\n",
            (unsigned long long)retired_count, total);
    return 0;
}
