// Minimig-fullsys integration harness — Phase 0.
//
// Goal: prove our `m68k_core` can boot K1.3 ROM in a NEW testbench
// (not via the existing `m68k_top` / `m68k_bus` stack), using only a
// minimal ic/dc bus arbiter and a flat memory model.
//
// What this validates:
// - m68k_core is reusable outside `m68k_top` (no hidden dependency on
//   the chipset-aware m68k_bus arbiter).
// - The ic/dc port semantics (req/ack/rdata) are well-defined enough to
//   drive any memory backend that handles the handshake correctly.
//
// What this DOESN'T validate (Phase 0.5+):
// - 68000-style external bus protocol (_AS/_UDS/_LDS/_DTACK).  That
//   layer goes between the arbiter and the memory, and is what would
//   connect to minimig.v.  Deferred.
// - Real minimig chipset.  This top uses a stub (chipset reads return
//   $00, writes ignored).
//
// Address map:
//   $00_0000 - $07_FFFF  chip RAM (512 KiB), or ROM mirror if OVL=1
//   $BF_E001             CIA-A PRA — write here clears OVL (Amiga-faithful)
//   $C0_0000 - $C7_FFFF  slow/trapdoor RAM (512 KiB)
//   $DF_F000 - $DF_F1FF  chipset stub: reads $0000, writes ignored
//   $F8_0000 - $FF_FFFF  ROM (mirrored every 256 KiB; K1.3 is 256 KiB)
//   everything else      reads $0000, writes ignored
//
// Use:
//   make minimig-fullsys ROMFILE=kickstart/kick_13.bin
//   make fullsys-smoke   ROMFILE=kickstart/kick_13.bin

module minimig_fullsys_top (
    input  wire        clk,
    input  wire        rst_n,
    output wire [31:0] cur_pc,
    output wire [31:0] retired,
    output wire        halted,
    output wire [15:0] halt_code,
    output wire        cpu_in_stop
);

    // ------------------------------------------------------------------
    // CPU instance.  KICKSTART_BOOT must be `define+'d at the compile
    // line so m68k_defs.vh sets RESET_PC=$FC00D2.
    // ------------------------------------------------------------------
    wire        ic_req, ic_we, ic_lock;
    wire [31:0] ic_addr, ic_wdata;
    wire [3:0]  ic_be;
    reg         ic_ack;
    reg  [31:0] ic_rdata;

    wire        dc_req, dc_we, dc_lock;
    wire [31:0] dc_addr, dc_wdata;
    wire [3:0]  dc_be;
    wire        dc_is_long;
    reg         dc_ack;
    reg  [31:0] dc_rdata;

    m68k_core u_cpu (
        .clk           (clk),
        .rst_n         (rst_n),
        .reset_a7      (32'h0000_4000),  // matches m68k_top default
        .ipl_i         (3'd0),            // no interrupts in Phase 0
        .ic_req        (ic_req),
        .ic_we         (ic_we),
        .ic_lock       (ic_lock),
        .ic_addr       (ic_addr),
        .ic_wdata      (ic_wdata),
        .ic_be         (ic_be),
        .ic_ack        (ic_ack),
        .ic_rdata      (ic_rdata),
        .dc_req        (dc_req),
        .dc_we         (dc_we),
        .dc_lock       (dc_lock),
        .dc_addr       (dc_addr),
        .dc_wdata      (dc_wdata),
        .dc_be         (dc_be),
        .dc_is_long    (dc_is_long),
        .dc_ack        (dc_ack),
        .dc_rdata      (dc_rdata),
        .halted        (halted),
        .halt_code     (halt_code),
        .retired       (retired),
        .cpu_in_stop   (cpu_in_stop),
        .cur_pc        (cur_pc),
        .dbg_regs_flat (),
        .dbg_sr        ()
    );

    // ------------------------------------------------------------------
    // Memory: 32-bit-word indexed to match bin2rom.py's output format
    // (one 8-hex-digit longword per line).  Matches the m68k_bus.v
    // convention so we can re-use the same rom.hex from existing
    // builds.
    // ------------------------------------------------------------------
    reg [31:0] chip_ram [0:131071];  // 512 KiB / 4 = 128K longwords
    reg [31:0] slow_ram [0:131071];
    reg [31:0] rom      [0:65535];   // 256 KiB / 4 = 64K longwords

    parameter MEM_HEXFILE_ROM = "rom.hex";
    initial begin
        if (MEM_HEXFILE_ROM != "") $readmemh(MEM_HEXFILE_ROM, rom);
    end

    // OVL: at reset, ROM is mirrored at $00_0000 so the CPU can read
    // the reset vector ($0 = SP, $4 = PC).  Cleared by the first write
    // to CIA-A PRA at $BF_E001 (Amiga-faithful).
    reg ovl_active;
    initial ovl_active = 1'b1;

    // ------------------------------------------------------------------
    // Bus arbiter: dc wins on conflict (matches m68k_bus semantics).
    // 1-cycle response.  ic takes the next slot.
    // ------------------------------------------------------------------
    reg        bus_owner_dc;        // 1 = serving dc, 0 = serving ic
    reg        bus_busy;            // 1 = transaction in progress

    wire        cur_req   = bus_owner_dc ? dc_req   : ic_req;
    wire        cur_we    = bus_owner_dc ? dc_we    : 1'b0;
    wire [31:0] cur_addr  = bus_owner_dc ? dc_addr  : ic_addr;
    wire [31:0] cur_wdata = bus_owner_dc ? dc_wdata : 32'd0;
    wire [3:0]  cur_be    = bus_owner_dc ? dc_be    : 4'b1111;

    // ------------------------------------------------------------------
    // Address decode + memory read/write.
    // Read latency: 1 cycle (registered ack + rdata).
    // ------------------------------------------------------------------
    function [31:0] read_mem(input [31:0] addr);
        reg [31:0] a;
        reg [15:0] rom_idx;
        reg [16:0] ram_idx;
        reg [31:0] data;
        begin
            a       = addr & 32'h00FF_FFFC;   // longword-align
            rom_idx = a[17:2];                // 64K-longword wrap (256 KiB)
            ram_idx = a[18:2];                // 128K-longword wrap (512 KiB)
            // ROM region ($F8_0000+) — mirrored every 256 KiB.
            if (a >= 32'h00F8_0000)             data = rom[rom_idx];
            // Chip RAM with OVL: read ROM
            else if (ovl_active && a < 32'h0008_0000) data = rom[rom_idx];
            // Chip RAM normal
            else if (a < 32'h0008_0000)         data = chip_ram[ram_idx];
            // Slow RAM
            else if (a >= 32'h00C0_0000 && a < 32'h00C8_0000)
                                                data = slow_ram[ram_idx];
            // Chipset / IO stub
            else                                data = 32'h0000_0000;
            read_mem = data;
        end
    endfunction

    task write_mem;
        input [31:0] addr;
        input [3:0]  be;
        input [31:0] wdata;
        reg   [31:0] a;
        reg   [16:0] ram_idx;
        reg   [31:0] cur;
        reg   [31:0] merged;
        begin
            a       = addr & 32'h00FF_FFFC;
            ram_idx = a[18:2];
            // CIA-A PRA write at $BF_E001 — clear OVL.
            if ((addr & 32'h00FF_FFF8) == 32'h00BF_E000) begin
                if (ovl_active) $display("[fullsys] OVL cleared at retired=%0d pc=%h",
                                          retired, cur_pc);
                ovl_active <= 1'b0;
            end
            // Chip RAM (writes always go to chip RAM regardless of OVL).
            if (a < 32'h0008_0000) begin
                cur    = chip_ram[ram_idx];
                merged = {be[3] ? wdata[31:24] : cur[31:24],
                          be[2] ? wdata[23:16] : cur[23:16],
                          be[1] ? wdata[15:8]  : cur[15:8],
                          be[0] ? wdata[7:0]   : cur[7:0]};
                chip_ram[ram_idx] <= merged;
            end else if (a >= 32'h00C0_0000 && a < 32'h00C8_0000) begin
                cur    = slow_ram[ram_idx];
                merged = {be[3] ? wdata[31:24] : cur[31:24],
                          be[2] ? wdata[23:16] : cur[23:16],
                          be[1] ? wdata[15:8]  : cur[15:8],
                          be[0] ? wdata[7:0]   : cur[7:0]};
                slow_ram[ram_idx] <= merged;
            end
            // ROM writes silently ignored.
        end
    endtask

    // ------------------------------------------------------------------
    // Arbiter FSM + ack generation.
    // ------------------------------------------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ic_ack       <= 1'b0;
            dc_ack       <= 1'b0;
            bus_owner_dc <= 1'b0;
            bus_busy     <= 1'b0;
            ovl_active   <= 1'b1;
        end else begin
            ic_ack <= 1'b0;
            dc_ack <= 1'b0;

            if (!bus_busy) begin
                // Pick next requester: dc wins on conflict.
                if (dc_req) begin
                    bus_owner_dc <= 1'b1;
                    bus_busy     <= 1'b1;
                end else if (ic_req) begin
                    bus_owner_dc <= 1'b0;
                    bus_busy     <= 1'b1;
                end
            end else begin
                // Serve the current owner: 1-cycle response.
                if (cur_we) write_mem(cur_addr, cur_be, cur_wdata);
                if (bus_owner_dc) begin
                    dc_ack    <= 1'b1;
                    dc_rdata  <= read_mem(cur_addr);
                end else begin
                    ic_ack    <= 1'b1;
                    ic_rdata  <= read_mem(cur_addr);
                end
                bus_busy <= 1'b0;
`ifdef FULLSYS_BUS_TRACE
                if (bus_owner_dc)
                    $display("[bus] DC %s addr=%h wdata=%h be=%b rdata=%h pc=%h r=%0d",
                        cur_we ? "WR " : "RD ",
                        cur_addr, cur_wdata, cur_be, read_mem(cur_addr), cur_pc, retired);
                else
                    $display("[bus] IC RD addr=%h rdata=%h pc=%h r=%0d",
                        cur_addr, read_mem(cur_addr), cur_pc, retired);
`endif
            end
        end
    end

    // Silence unused-signal warnings.
    wire _unused = &{ic_we, ic_lock, ic_wdata, ic_be, dc_lock, dc_is_long, 1'b0};

endmodule
