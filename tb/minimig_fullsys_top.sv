// Minimig-fullsys integration harness — Phase 0.5.
//
// Phase 0 (commit 992d283) wired m68k_core directly to a flat memory
// model via an internal arbiter.  Phase 0.5 inserts the 68000-style
// bus wrapper (m68k_to_amiga_bus.sv) between the CPU and memory, so
// the memory model now speaks 68000 protocol (_AS/_UDS/_LDS/_DTACK).
// This is the interface minimig.v's chipset stack expects; once it's
// validated end-to-end, Phase 1 can replace the flat memory with
// real minimig.v modules with no further bus changes.
//
// Address map (unchanged from Phase 0):
//   $00_0000 - $07_FFFF  chip RAM (512 KiB), or ROM mirror if OVL=1
//   $BF_E001             CIA-A PRA — write here clears OVL
//   $C0_0000 - $C7_FFFF  slow/trapdoor RAM (512 KiB)
//   $DF_F000 - $DF_F1FF  chipset stub (reads $00, writes ignored)
//   $F8_0000 - $FF_FFFF  ROM (mirrored every 256 KiB)
//   everything else      reads $00, writes ignored

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
    // CPU instance.  KICKSTART_BOOT must be defined at compile time.
    // ------------------------------------------------------------------
    wire        ic_req, ic_we, ic_lock;
    wire [31:0] ic_addr, ic_wdata;
    wire [3:0]  ic_be;
    wire        ic_ack;
    wire [31:0] ic_rdata;

    wire        dc_req, dc_we, dc_lock;
    wire [31:0] dc_addr, dc_wdata;
    wire [3:0]  dc_be;
    wire        dc_is_long;
    wire        dc_ack;
    wire [31:0] dc_rdata;

    m68k_core u_cpu (
        .clk           (clk),
        .rst_n         (rst_n),
        .reset_a7      (32'h0000_4000),
        .ipl_i         (3'd0),
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
    // 68000-style bus signals (between wrapper and memory model).
    // ------------------------------------------------------------------
    wire [23:1] bus_addr;
    wire [15:0] bus_wdata;
    reg  [15:0] bus_rdata;
    wire        bus_as_n, bus_uds_n, bus_lds_n, bus_rw;
    reg         bus_dtack_n;

    m68k_to_amiga_bus u_bus_adapter (
        .clk        (clk),
        .rst_n      (rst_n),
        .ic_req     (ic_req),
        .ic_addr    (ic_addr),
        .ic_be      (ic_be),
        .ic_ack     (ic_ack),
        .ic_rdata   (ic_rdata),
        .dc_req     (dc_req),
        .dc_we      (dc_we),
        .dc_addr    (dc_addr),
        .dc_wdata   (dc_wdata),
        .dc_be      (dc_be),
        .dc_is_long (dc_is_long),
        .dc_ack     (dc_ack),
        .dc_rdata   (dc_rdata),
        .addr_o     (bus_addr),
        .data_o     (bus_wdata),
        .data_i     (bus_rdata),
        .as_n       (bus_as_n),
        .uds_n      (bus_uds_n),
        .lds_n      (bus_lds_n),
        .rw         (bus_rw),
        .dtack_n    (bus_dtack_n)
    );

    // ------------------------------------------------------------------
    // 68000-protocol memory model.  Watches as_n falling edge; decodes
    // address; provides rdata + dtack low; deasserts when as_n rises.
    // 32-bit word memories with 16-bit external port; bus_addr[1] picks
    // high vs low word in each longword.
    // ------------------------------------------------------------------
    reg [31:0] chip_ram [0:131071];
    reg [31:0] slow_ram [0:131071];
    reg [31:0] rom      [0:65535];

    parameter MEM_HEXFILE_ROM = "rom.hex";
    initial begin
        if (MEM_HEXFILE_ROM != "") $readmemh(MEM_HEXFILE_ROM, rom);
    end

    reg ovl_active;
    initial ovl_active = 1'b1;

    // Build a 24-bit byte address from the bus's word address.
    wire [23:0] byte_addr = {bus_addr, 1'b0};
    wire [16:0] lw_idx    = byte_addr[18:2];     // 0..131071 longword idx (chip/slow)
    wire [15:0] rom_idx   = byte_addr[17:2];     // 0..65535 longword idx (ROM)
    wire        high_half = ~byte_addr[1];        // 1 = high word of LW (byte 0,1)

    // Pick the right longword from memory.
    function [31:0] mem_read_lw(input [23:0] a);
        begin
            if      (a >= 24'hF8_0000)             mem_read_lw = rom[rom_idx];
            else if (ovl_active && a < 24'h08_0000) mem_read_lw = rom[rom_idx];
            else if (a < 24'h08_0000)              mem_read_lw = chip_ram[lw_idx];
            else if (a >= 24'hC0_0000 && a < 24'hC8_0000) mem_read_lw = slow_ram[lw_idx];
            else                                   mem_read_lw = 32'd0;
        end
    endfunction

    // Combinational rdata + dtack.  When as_n goes low, the memory's
    // response is immediately available (1-cycle latency from the
    // bus-wrapper's S_HI / S_LO state, which is the next clock edge).
    always @* begin
        if (!bus_as_n) begin
            bus_rdata   = high_half ? mem_read_lw(byte_addr)[31:16]
                                    : mem_read_lw(byte_addr)[15:0];
            bus_dtack_n = 1'b0;
        end else begin
            bus_rdata   = 16'h0000;
            bus_dtack_n = 1'b1;
        end
    end

    // Write side: latch on as_n low + !rw, with strobes determining
    // which bytes to update.
    always @(posedge clk) begin
        if (!bus_as_n && !bus_rw) begin
            // CIA-A PRA write clears OVL.
            if ({byte_addr[23:3], 3'b000} == 24'hBF_E000) begin
                if (ovl_active) $display("[fullsys] OVL cleared at retired=%0d pc=%h",
                                          retired, cur_pc);
                ovl_active <= 1'b0;
            end
            // Chip RAM
            if (byte_addr < 24'h08_0000) begin
                if (high_half) begin
                    if (!bus_uds_n) chip_ram[lw_idx][31:24] <= bus_wdata[15:8];
                    if (!bus_lds_n) chip_ram[lw_idx][23:16] <= bus_wdata[7:0];
                end else begin
                    if (!bus_uds_n) chip_ram[lw_idx][15:8]  <= bus_wdata[15:8];
                    if (!bus_lds_n) chip_ram[lw_idx][7:0]   <= bus_wdata[7:0];
                end
            end else if (byte_addr >= 24'hC0_0000 && byte_addr < 24'hC8_0000) begin
                if (high_half) begin
                    if (!bus_uds_n) slow_ram[lw_idx][31:24] <= bus_wdata[15:8];
                    if (!bus_lds_n) slow_ram[lw_idx][23:16] <= bus_wdata[7:0];
                end else begin
                    if (!bus_uds_n) slow_ram[lw_idx][15:8]  <= bus_wdata[15:8];
                    if (!bus_lds_n) slow_ram[lw_idx][7:0]   <= bus_wdata[7:0];
                end
            end
        end
    end

`ifdef FULLSYS_BUS_TRACE
    reg as_n_d;
    always @(posedge clk) begin
        as_n_d <= bus_as_n;
        // Print on as_n falling edge (start of transaction).
        if (as_n_d && !bus_as_n) begin
            $display("[bus68k] %s addr=%h uds=%b lds=%b wdata=%h rdata=%h pc=%h r=%0d",
                bus_rw ? "RD" : "WR",
                byte_addr, ~bus_uds_n, ~bus_lds_n,
                bus_wdata, bus_rdata, cur_pc, retired);
        end
    end
`endif

    wire _unused = &{ic_we, ic_lock, ic_wdata, dc_lock, 1'b0};

endmodule
