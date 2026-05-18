/* Linker script for fast_68000 C programs.
 *
 * Memory layout matches MULTICORE.md and tests/* convention:
 *   0x00000000 .. 0x000003FF   exception vector table (256 vectors)
 *   0x00000400 .. 0x00003FFF   code + rodata + data + bss
 *   0x00004000 .. 0x00007FFF   per-core stacks (reset SP for core N
 *                              is 0x4000 + N*0x1000, set by hardware)
 *   0x00010000 ..              user data, console mailbox, etc.
 *
 * Output is plain ELF; the cc68k.sh wrapper strips it with objcopy
 * -O binary and bin2hex.py packs it to the $readmemh format.
 *
 * We do NOT emit the reset SSP/PC into the binary itself — m68k_bus.v
 * resets SP and PC by hardware (per MULTICORE.md), so byte addresses
 * 0..7 in the binary are zero and that's fine.
 */

ENTRY(_start)

MEMORY {
    LOMEM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00004000
}

SECTIONS
{
    .vectors 0x00000000 : {
        KEEP(*(.vectors))
        . = 0x400;
    } > LOMEM

    .text 0x00000400 : {
        *(.text._start)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > LOMEM

    .data : {
        *(.data*)
        . = ALIGN(4);
    } > LOMEM

    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > LOMEM

    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) *(.gnu*) }
}
