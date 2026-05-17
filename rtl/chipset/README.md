# Chipset

Amiga-inspired chipset IP cores.  Each module is bus-agnostic - it
exposes a simple `req / we / addr / be / wdata / rdata / ack` slave
port for register access and (where applicable) the same handshake on
its master port for DMA.

These modules have no dependency on the 68000 core or its bus
arbiter.  They are wired up to this project's CPU via
`rtl/m68k_bus.v`, but the same modules could be dropped into any
SoC by wrapping the ports in Wishbone, AXI-Lite, or whatever
interconnect the host system uses.

| Module    | Purpose                                                |
| --------- | ------------------------------------------------------ |
| `blitter` | 4-channel bit-block engine: copy/logic/line-draw/fill. |
| `copper`  | Display-list coprocessor: MOVE/WAIT/SKIP/HALT.         |
| `denise`  | Bitplane rasterizer: 1-6 planes, HAM6, EHB, DPF, scroll, 8 sprites. |
| `paula`   | 4-voice DMA audio engine.                              |
