`include "m68k_defs.vh"

// Multi-core top-level. Wires N_CORES cores, each with its own I and D
// cache, to a shared bus + memory. Bus ports are numbered 2*i (I-cache)
// and 2*i+1 (D-cache) for core i.
module m68k_top #(
    parameter N_CORES    = 2,
    parameter USE_CACHE  = 1,           // 1 = real L1 caches, 0 = bus passthrough
    parameter MEM_WORDS  = 16384,
    parameter MEM_HEXFILE = "program.hex"
)(
    input  wire clk,
    input  wire rst_n,
    output wire [N_CORES-1:0]    halted,
    output wire [16*N_CORES-1:0] halt_code_flat,
    output wire [32*N_CORES-1:0] retired_flat,

    // Framebuffer peek for the simulator harness. Drive fb_peek_addr with a
    // word-aligned byte address; fb_peek_data returns the 32-bit word.
    input  wire [31:0] fb_peek_addr,
    output wire [31:0] fb_peek_data
);
    // Two ports per core (I-cache + D-cache) plus one for the blitter master.
    localparam N_PORTS = 2 * N_CORES + 1;
    localparam BLT_PORT = 2 * N_CORES;       // index of the blitter master port
    localparam PID_BITS = $clog2(N_PORTS > 1 ? N_PORTS : 2);

    // Per-core cache <-> bus.
    wire [N_PORTS-1:0]    p_req;
    wire [N_PORTS-1:0]    p_we;
    wire [N_PORTS-1:0]    p_lock;
    wire [32*N_PORTS-1:0] p_addr;
    wire [32*N_PORTS-1:0] p_wdata;
    wire [4*N_PORTS-1:0]  p_be;
    wire [N_PORTS-1:0]    p_grant;     // unused by caches but routed for completeness
    wire [N_PORTS-1:0]    p_resp_valid;
    wire [31:0]           p_resp_data;

    wire                  snoop_valid;
    wire [31:0]           snoop_addr;
    wire [PID_BITS-1:0]   snoop_src_id;
    wire [2:0]            irq_level;

    // Blitter slave wires.
    wire        blt_slv_req;
    wire        blt_slv_we;
    wire [5:0]  blt_slv_addr;
    wire [3:0]  blt_slv_be;
    wire [31:0] blt_slv_wdata;
    wire [31:0] blt_slv_rdata;

    m68k_bus #(
        .N_PORTS    (N_PORTS),
        .MEM_WORDS  (MEM_WORDS),
        .MEM_HEXFILE(MEM_HEXFILE)
    ) u_bus (
        .clk         (clk),
        .rst_n       (rst_n),
        .req         (p_req),
        .we          (p_we),
        .lock        (p_lock),
        .addr_flat   (p_addr),
        .wdata_flat  (p_wdata),
        .be_flat     (p_be),
        .grant       (p_grant),
        .resp_valid  (p_resp_valid),
        .resp_data   (p_resp_data),
        .snoop_valid (snoop_valid),
        .snoop_addr  (snoop_addr),
        .snoop_src_id(snoop_src_id),
        .irq_level   (irq_level),
        .fb_peek_addr(fb_peek_addr),
        .fb_peek_data(fb_peek_data),
        .blt_slv_req (blt_slv_req),
        .blt_slv_we  (blt_slv_we),
        .blt_slv_addr(blt_slv_addr),
        .blt_slv_be  (blt_slv_be),
        .blt_slv_wdata(blt_slv_wdata),
        .blt_slv_rdata(blt_slv_rdata)
    );

    // Blitter instance.  Its master port plugs into the bus at index
    // BLT_PORT (the slot past all CPU I/D ports).
    wire        blt_mst_req;
    wire        blt_mst_we;
    wire [31:0] blt_mst_addr;
    wire [31:0] blt_mst_wdata;
    wire [3:0]  blt_mst_be;
    wire        blt_mst_ack;
    wire [31:0] blt_mst_rdata;

    m68k_blitter u_blt (
        .clk          (clk),
        .rst_n        (rst_n),
        .slv_req      (blt_slv_req),
        .slv_we       (blt_slv_we),
        .slv_addr     (blt_slv_addr),
        .slv_be       (blt_slv_be),
        .slv_wdata    (blt_slv_wdata),
        .slv_rdata    (blt_slv_rdata),
        .slv_ack      (),               // unused: bus handles ack timing
        .mst_req      (blt_mst_req),
        .mst_we       (blt_mst_we),
        .mst_addr     (blt_mst_addr),
        .mst_wdata    (blt_mst_wdata),
        .mst_be       (blt_mst_be),
        .mst_ack      (blt_mst_ack),
        .mst_rdata    (blt_mst_rdata)
    );

    // Wire the blitter master into the arbiter at BLT_PORT.
    assign p_req  [BLT_PORT]                  = blt_mst_req;
    assign p_we   [BLT_PORT]                  = blt_mst_we;
    assign p_lock [BLT_PORT]                  = 1'b0;
    assign p_addr [32*BLT_PORT +: 32]         = blt_mst_addr;
    assign p_wdata[32*BLT_PORT +: 32]         = blt_mst_wdata;
    assign p_be   [4*BLT_PORT  +: 4]          = blt_mst_be;
    assign blt_mst_ack   = p_resp_valid[BLT_PORT];
    assign blt_mst_rdata = p_resp_data;

    genvar gi;
    generate
        for (gi = 0; gi < N_CORES; gi = gi + 1) begin : g_core
            // I-cache port = gi*2, D-cache port = gi*2+1.
            wire        ic_req, ic_we, ic_lock;
            wire [31:0] ic_addr, ic_wdata;
            wire [3:0]  ic_be;
            wire        ic_ack;
            wire [31:0] ic_rdata;

            wire        dc_req, dc_we, dc_lock;
            wire [31:0] dc_addr, dc_wdata;
            wire [3:0]  dc_be;
            wire        dc_ack;
            wire [31:0] dc_rdata;

            if (USE_CACHE) begin : g_cache
                m68k_cache #(
                    .NUM_LINES (256),
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2),
                    .WRITABLE  (0)
                ) u_ic (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (ic_req),
                    .cpu_we      (1'b0),
                    .cpu_lock    (1'b0),
                    .cpu_addr    (ic_addr),
                    .cpu_wdata   (32'd0),
                    .cpu_be      (4'b1111),
                    .cpu_ack     (ic_ack),
                    .cpu_rdata   (ic_rdata),
                    .bus_req     (p_req[gi*2]),
                    .bus_we      (p_we[gi*2]),
                    .bus_lock    (p_lock[gi*2]),
                    .bus_addr    (p_addr[32*(gi*2) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2) +: 32]),
                    .bus_be      (p_be[4*(gi*2) +: 4]),
                    .bus_grant   (p_grant[gi*2]),
                    .bus_resp_valid(p_resp_valid[gi*2]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id)
                );

                m68k_cache #(
                    .NUM_LINES (256),
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2 + 1),
                    .WRITABLE  (1)
                ) u_dc (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (dc_req),
                    .cpu_we      (dc_we),
                    .cpu_lock    (dc_lock),
                    .cpu_addr    (dc_addr),
                    .cpu_wdata   (dc_wdata),
                    .cpu_be      (dc_be),
                    .cpu_ack     (dc_ack),
                    .cpu_rdata   (dc_rdata),
                    .bus_req     (p_req[gi*2+1]),
                    .bus_we      (p_we[gi*2+1]),
                    .bus_lock    (p_lock[gi*2+1]),
                    .bus_addr    (p_addr[32*(gi*2+1) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2+1) +: 32]),
                    .bus_be      (p_be[4*(gi*2+1) +: 4]),
                    .bus_grant   (p_grant[gi*2+1]),
                    .bus_resp_valid(p_resp_valid[gi*2+1]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id)
                );
            end else begin : g_passthrough
                m68k_passthrough #(
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2),
                    .WRITABLE  (0)
                ) u_ic (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (ic_req),
                    .cpu_we      (1'b0),
                    .cpu_lock    (1'b0),
                    .cpu_addr    (ic_addr),
                    .cpu_wdata   (32'd0),
                    .cpu_be      (4'b1111),
                    .cpu_ack     (ic_ack),
                    .cpu_rdata   (ic_rdata),
                    .bus_req     (p_req[gi*2]),
                    .bus_we      (p_we[gi*2]),
                    .bus_lock    (p_lock[gi*2]),
                    .bus_addr    (p_addr[32*(gi*2) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2) +: 32]),
                    .bus_be      (p_be[4*(gi*2) +: 4]),
                    .bus_grant   (p_grant[gi*2]),
                    .bus_resp_valid(p_resp_valid[gi*2]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id)
                );

                m68k_passthrough #(
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2 + 1),
                    .WRITABLE  (1)
                ) u_dc (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (dc_req),
                    .cpu_we      (dc_we),
                    .cpu_lock    (dc_lock),
                    .cpu_addr    (dc_addr),
                    .cpu_wdata   (dc_wdata),
                    .cpu_be      (dc_be),
                    .cpu_ack     (dc_ack),
                    .cpu_rdata   (dc_rdata),
                    .bus_req     (p_req[gi*2+1]),
                    .bus_we      (p_we[gi*2+1]),
                    .bus_lock    (p_lock[gi*2+1]),
                    .bus_addr    (p_addr[32*(gi*2+1) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2+1) +: 32]),
                    .bus_be      (p_be[4*(gi*2+1) +: 4]),
                    .bus_grant   (p_grant[gi*2+1]),
                    .bus_resp_valid(p_resp_valid[gi*2+1]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id)
                );
            end

            // Per-core reset SP: read from memory[gi] (low word of mem[gi]).
            // We expose it via the bus model by a "reset_a7" wire computed by
            // peeking memory at startup. Simplest: derive from a parameter.
            // We give each core its own stack: 0x8000, 0x9000, 0xA000, 0xB000.
            wire [31:0] reset_a7 = 32'h0000_4000 + (gi * 32'h0000_1000);

            m68k_core #(
                .CORE_ID(gi[3:0])
            ) u_core (
                .clk        (clk),
                .rst_n      (rst_n),
                .reset_a7   (reset_a7),
                .ipl_i      (irq_level),
                .ic_req     (ic_req),
                .ic_we      (ic_we),
                .ic_lock    (ic_lock),
                .ic_addr    (ic_addr),
                .ic_wdata   (ic_wdata),
                .ic_be      (ic_be),
                .ic_ack     (ic_ack),
                .ic_rdata   (ic_rdata),
                .dc_req     (dc_req),
                .dc_we      (dc_we),
                .dc_lock    (dc_lock),
                .dc_addr    (dc_addr),
                .dc_wdata   (dc_wdata),
                .dc_be      (dc_be),
                .dc_ack     (dc_ack),
                .dc_rdata   (dc_rdata),
                .halted     (halted[gi]),
                .halt_code  (halt_code_flat[16*gi +: 16]),
                .retired    (retired_flat[32*gi +: 32])
            );
        end
    endgenerate
endmodule
