// Cross-validation top for MiniMig blitter vs our blitter.
//
// Hosts both blitters side-by-side with separate 64 KiB memory buffers
// (mm_mem, our_mem).  The C++ harness drives identical register-write
// sequences into both, ticks the clock until both signal !busy, then
// reads back the memories and diffs.
//
// Memory layout: 32K words (= 64 KiB) addressed by 15-bit word index.
// Both buffers are pre-loaded with the SAME initial pattern by the
// harness via init_we/init_addr/init_wdata before the blit starts.

module minimig_blt_xcheck_top (
    input  wire        clk,
    input  wire        rst,

    // Common register-write interface (drives both blitters).
    // reg_addr is the 8-bit Amiga chipset offset (e.g. $40 for BLTCON0).
    // For our blitter slave port, we translate this to the internal
    // [5:0] address (= reg_addr - $40, with the layout matching
    // our blitter.v: $00=BLTCON, $04=BLTAFWM, $0C=BLTAPT, $10=BLTBPT,
    // $14=BLTCPT, $18=BLTDPT, $1C=BLTAMOD, $20=BLTBMOD, $24=BLTCMOD,
    // $28=BLTDMOD, $38=BLTSIZE).
    input  wire        reg_we,
    input  wire [8:0]  reg_addr,         // canonical Amiga offset (0..$7E)
    input  wire [15:0] reg_wdata,        // 16-bit value

    // Memory pre-load interface.
    input  wire        init_we,
    input  wire [14:0] init_addr,        // word address (15 bits => 32K words)
    input  wire [15:0] init_wdata,

    // Memory readback after blit.
    input  wire [14:0] read_addr,
    output wire [15:0] mm_read_data,
    output wire [15:0] our_read_data,

    // Status: both !busy?
    output wire        mm_busy,
    output wire        our_busy
);

    // ---------------- Memories (one per DUT) ----------------
    reg [15:0] mm_mem  [0:32767];
    reg [15:0] our_mem [0:32767];

    // ---------------- MiniMig blitter ----------------
    wire        mm_reqdma;
    wire        mm_we_o;
    wire [15:0] mm_data_out;
    wire [20:1] mm_addr_out;
    wire [8:1]  mm_reg_addr_out;
    wire        mm_zero, mm_int3;
    reg  [15:0] mm_data_in_mux;

    // Multiplex data_in: register writes when reg_we, memory data otherwise.
    always @* begin
        if (reg_we)
            mm_data_in_mux = reg_wdata;
        else
            mm_data_in_mux = mm_mem[mm_addr_out[15:1]];
    end

    agnus_blitter mm_blt (
        .clk            (clk),
        .clk7_en        (1'b1),
        .reset          (rst),
        .ecs            (1'b0),
        .clkena         (1'b1),
        .enadma         (1'b1),
        .reqdma         (mm_reqdma),
        .ackdma         (mm_reqdma),    // instant-ack
        .we             (mm_we_o),
        .zero           (mm_zero),
        .busy           (mm_busy),
        .int3           (mm_int3),
        .data_in        (mm_data_in_mux),
        .data_out       (mm_data_out),
        .reg_address_in (reg_we ? reg_addr[8:1] : mm_reg_addr_out),
        .address_out    (mm_addr_out),
        .reg_address_out(mm_reg_addr_out)
    );

    // MiniMig memory write on D-channel ack + we.
    always @(posedge clk) begin
        if (init_we) mm_mem[init_addr] <= init_wdata;
        else if (mm_reqdma && mm_we_o)
            mm_mem[mm_addr_out[15:1]] <= mm_data_out;
    end

    // ---------------- Our blitter ----------------
    // Translate canonical reg_addr (0..$3E for BLT regs) into our
    // blitter's [5:0] internal layout.  Matches the m68k_bus.v
    // translator.
    //
    // BLTCON is one 32-bit register in our blitter, but the Amiga
    // chipset exposes it as two 16-bit halves (BLTCON0 at $40,
    // BLTCON1 at $42).  We maintain staging copies of both halves
    // and ALWAYS push the fully-composed 32-bit value on either
    // write, so a $42 write does not clobber BLTCON0 fields
    // (and vice versa).
    reg [15:0] bltcon0_q;
    reg [15:0] bltcon1_q;
    always @(posedge clk) begin
        if (rst) begin
            bltcon0_q <= 16'd0;
            bltcon1_q <= 16'd0;
        end else if (reg_we) begin
            if (reg_addr[7:0] == 8'h40) bltcon0_q <= reg_wdata;
            if (reg_addr[7:0] == 8'h42) bltcon1_q <= reg_wdata;
        end
    end

    // "Effective" BLTCON0/1: use the value being written this cycle for
    // the matching half, and the staged value for the other half.
    wire [15:0] eff_b0 = (reg_we && reg_addr[7:0] == 8'h40) ? reg_wdata : bltcon0_q;
    wire [15:0] eff_b1 = (reg_we && reg_addr[7:0] == 8'h42) ? reg_wdata : bltcon1_q;

    reg [5:0]  our_slv_addr;
    reg [31:0] our_slv_wdata;
    reg        our_slv_req;
    always @* begin
        our_slv_req   = reg_we;
        our_slv_addr  = 6'h00;
        our_slv_wdata = 32'd0;
        case (reg_addr[7:0])
            8'h40, 8'h42: begin
                // Compose full BLTCON from staged + current half.
                our_slv_addr  = 6'h00;
                our_slv_wdata = {
                    eff_b0[7:0],        // LF       -> [31:24]
                    eff_b0[15:12],      // ASH      -> [23:20]
                    eff_b1[15:12],      // BSH      -> [19:16]
                    eff_b1[1],          // DESC     -> [15]
                    eff_b1[3],          // IFE      -> [14]
                    eff_b1[4],          // EFE      -> [13]
                    eff_b1[2],          // FCI      -> [12]
                    eff_b1[0],          // LINE     -> [11]
                    eff_b1[3:1],        // SUD/SUL/AUL -> [10:8]
                    4'd0,               // reserved -> [7:4]
                    eff_b0[11:8]        // USEA-D   -> [3:0]
                };
            end
            8'h44: begin our_slv_addr = 6'h04; our_slv_wdata = {16'd0, reg_wdata}; end // BLTAFWM
            8'h46: begin our_slv_addr = 6'h08; our_slv_wdata = {16'd0, reg_wdata}; end // BLTALWM
            8'h50: begin our_slv_addr = 6'h0C; our_slv_wdata = {reg_wdata, 16'd0}; end // BLTAPTH
            8'h52: begin our_slv_addr = 6'h0C; our_slv_wdata = {16'd0, reg_wdata}; end // BLTAPTL
            8'h4C: begin our_slv_addr = 6'h10; our_slv_wdata = {reg_wdata, 16'd0}; end // BLTBPTH
            8'h4E: begin our_slv_addr = 6'h10; our_slv_wdata = {16'd0, reg_wdata}; end // BLTBPTL
            8'h48: begin our_slv_addr = 6'h14; our_slv_wdata = {reg_wdata, 16'd0}; end // BLTCPTH
            8'h4A: begin our_slv_addr = 6'h14; our_slv_wdata = {16'd0, reg_wdata}; end // BLTCPTL
            8'h54: begin our_slv_addr = 6'h18; our_slv_wdata = {reg_wdata, 16'd0}; end // BLTDPTH
            8'h56: begin our_slv_addr = 6'h18; our_slv_wdata = {16'd0, reg_wdata}; end // BLTDPTL
            8'h64: begin our_slv_addr = 6'h1C; our_slv_wdata = {16'd0, reg_wdata}; end // BLTAMOD
            8'h62: begin our_slv_addr = 6'h20; our_slv_wdata = {16'd0, reg_wdata}; end // BLTBMOD
            8'h60: begin our_slv_addr = 6'h24; our_slv_wdata = {16'd0, reg_wdata}; end // BLTCMOD
            8'h66: begin our_slv_addr = 6'h28; our_slv_wdata = {16'd0, reg_wdata}; end // BLTDMOD
            8'h58: begin our_slv_addr = 6'h38; our_slv_wdata = {16'd0, reg_wdata}; end // BLTSIZE
            default: our_slv_req = 1'b0;
        endcase
    end

    wire        our_mst_req;
    wire        our_mst_we;
    wire [31:0] our_mst_addr;
    wire [3:0]  our_mst_be;
    wire [31:0] our_mst_wdata;
    wire        our_busy_q;
    assign our_busy = our_busy_q;

    // mst_rdata: drive from our_mem on read requests.  The blitter
    // uses pick_half(word, addr[1]): addr[1]=0 -> high half [31:16],
    // addr[1]=1 -> low half [15:0].  Place the 16-bit word in the
    // matching half so the read decode lines up with the real bus.
    reg [31:0] our_mst_rdata;
    always @* begin
        if (our_mst_addr[1])
            our_mst_rdata = {16'd0, our_mem[our_mst_addr[15:1]]};
        else
            our_mst_rdata = {our_mem[our_mst_addr[15:1]], 16'd0};
    end

    // 1-cycle-latency ack to avoid the combinational loop that arises
    // from feeding our_mst_req directly back as the ack (blitter gates
    // its mst_req combinationally with !mst_ack).  Functionally this
    // matches a fast bus arbiter that acknowledges in the next cycle.
    reg our_mst_ack_q;
    always @(posedge clk) begin
        if (rst) our_mst_ack_q <= 1'b0;
        else     our_mst_ack_q <= our_mst_req;
    end

    blitter our_blt (
        .clk        (clk),
        .rst_n      (~rst),
        .slv_req    (our_slv_req),
        .slv_we     (our_slv_req),
        .slv_addr   (our_slv_addr),
        .slv_be     (4'b1111),
        .slv_wdata  (our_slv_wdata),
        .slv_rdata  (),                  // unused
        .mst_req    (our_mst_req),
        .mst_we     (our_mst_we),
        .mst_addr   (our_mst_addr),
        .mst_wdata  (our_mst_wdata),
        .mst_be     (our_mst_be),
        .mst_ack    (our_mst_ack_q),     // 1-cycle latency
        .mst_rdata  (our_mst_rdata),
        .busy_o     (our_busy_q),
        .int_o      ()
    );

    // Our blitter memory write: take the half-word from mst_wdata
    // selected by mst_be (high or low half).
    reg [31:0] our_write_count;
    always @(posedge clk) begin
        if (our_write_count == 32'hFFFFFFFF) our_write_count <= 0;
        if (init_we) our_mem[init_addr] <= init_wdata;
        else if (our_mst_req && our_mst_we) begin
            if (our_write_count < 6)
                $display("[OUR_WR_%0d] addr=%h be=%b wdata=%h",
                    our_write_count, our_mst_addr, our_mst_be, our_mst_wdata);
            our_write_count <= our_write_count + 1;
            if (our_mst_be == 4'b1100)
                our_mem[our_mst_addr[15:1]] <= our_mst_wdata[31:16];
            else if (our_mst_be == 4'b0011)
                our_mem[our_mst_addr[15:1]] <= our_mst_wdata[15:0];
        end
    end
    reg [31:0] mm_write_count;
    always @(posedge clk) begin
        if (mm_write_count == 32'hFFFFFFFF) mm_write_count <= 0;
        if (!init_we && mm_reqdma && mm_we_o) begin
            if (mm_write_count < 6)
                $display("[MM_WR_%0d]  addr=%h data=%h",
                    mm_write_count, {mm_addr_out, 1'b0}, mm_data_out);
            mm_write_count <= mm_write_count + 1;
        end
    end

    assign mm_read_data  = mm_mem[read_addr];
    assign our_read_data = our_mem[read_addr];

endmodule
