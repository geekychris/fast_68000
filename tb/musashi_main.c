/* Cross-validation harness: runs a program.hex through Musashi and reports
 * the STOP halt code in the same format / exit code as our Verilator sim.
 * The aim is bit-identical halt codes between the two cores for every
 * test in tests/.
 */
#include "m68k.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MEM_SIZE 0x1000000   /* 16 MiB flat */

static uint8_t mem[MEM_SIZE];
static int halted = 0;
static unsigned int halt_code = 0;
static unsigned int halt_pc = 0;

/* Memory callbacks (big-endian byte order). */
unsigned int m68k_read_memory_8(unsigned int addr) {
    addr &= MEM_SIZE - 1;
    return mem[addr];
}
unsigned int m68k_read_memory_16(unsigned int addr) {
    addr &= MEM_SIZE - 1;
    return ((unsigned int)mem[addr] << 8) | mem[(addr + 1) & (MEM_SIZE - 1)];
}
unsigned int m68k_read_memory_32(unsigned int addr) {
    return ((unsigned int)m68k_read_memory_16(addr) << 16) |
            (unsigned int)m68k_read_memory_16(addr + 2);
}
void m68k_write_memory_8(unsigned int addr, unsigned int value) {
    addr &= MEM_SIZE - 1;
    mem[addr] = value & 0xFF;
}
void m68k_write_memory_16(unsigned int addr, unsigned int value) {
    addr &= MEM_SIZE - 1;
    mem[addr]                       = (value >> 8) & 0xFF;
    mem[(addr + 1) & (MEM_SIZE - 1)] = value & 0xFF;
}
void m68k_write_memory_32(unsigned int addr, unsigned int value) {
    m68k_write_memory_16(addr, (value >> 16) & 0xFFFF);
    m68k_write_memory_16(addr + 2, value & 0xFFFF);
}
unsigned int m68k_read_disassembler_8(unsigned int addr)  { return m68k_read_memory_8(addr); }
unsigned int m68k_read_disassembler_16(unsigned int addr) { return m68k_read_memory_16(addr); }
unsigned int m68k_read_disassembler_32(unsigned int addr) { return m68k_read_memory_32(addr); }


/* Instruction hook fires before each instruction.  Catch STOP #imm and
 * stash the immediate, then end the timeslice so the main loop sees us
 * halted before the real STOP enters the CPU's halted state. */
static void instr_hook(unsigned int pc) {
    unsigned int op = m68k_read_memory_16(pc);
    if (op == 0x4E72) {
        halt_code = m68k_read_memory_16(pc + 2);
        halt_pc = pc;
        halted = 1;
        m68k_end_timeslice();
    }
}

/* Load a $readmemh-style hex file: one 32-bit word per line, MSB-first,
 * starting at byte 0.  Used by our test bench. */
static int load_hex(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[64];
    int idx = 0;
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\0' || line[0] == '\n' || line[0] == '#') continue;
        unsigned long word = strtoul(line, NULL, 16);
        if ((unsigned long)(idx + 1) * 4 > MEM_SIZE) break;
        m68k_write_memory_32((unsigned int)(idx * 4), (unsigned int)word);
        idx++;
    }
    fclose(f);
    return idx;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s program.hex [max_cycles]\n", argv[0]);
        return 1;
    }
    long max_cycles = (argc >= 3) ? strtol(argv[2], NULL, 10) : 5000000L;

    memset(mem, 0, sizeof(mem));
    if (load_hex(argv[1]) < 0) {
        fprintf(stderr, "musashi: failed to load %s\n", argv[1]);
        return 1;
    }

    /* Match the Verilator harness's reset state: SSP=$4000, PC=$400.  Our
     * Verilog hardware bootstraps those directly (no vector fetch), so the
     * test hex files don't write them. */
    m68k_write_memory_32(0, 0x00004000);
    m68k_write_memory_32(4, 0x00000400);

    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000);
    m68k_set_instr_hook_callback(instr_hook);
    m68k_pulse_reset();

    long total_cycles = 0;
    while (!halted && total_cycles < max_cycles) {
        int ran = m68k_execute(1000);
        if (ran <= 0) break;
        total_cycles += ran;
    }

    if (halted) {
        printf("[musashi] halt code=0x%04x PC=0x%06x cycles=%ld\n",
               halt_code & 0xFFFF, halt_pc, total_cycles);
        int rc = halt_code & 0xFF;
        if (rc == 0 && halt_code != 0) rc = 1;
        return rc;
    }
    printf("[musashi] TIMEOUT cycles=%ld\n", total_cycles);
    return 255;
}
