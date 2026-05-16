// Round-robin shared bus arbiter + memory model.
//
// One transaction per cycle. The granted port sees grant=1 in cycle T; memory
// drives resp_valid + resp_data in cycle T+1 to that same port. Other ports
// can be granted while this one's response is in flight (write transactions
// don't stall further grants).
//
// Lock semantics: when a port wins a grant with `lock=1`, the arbiter pins
// the bus to that port for one additional transaction (which is expected to
// arrive without `lock` set). Any other requester is starved until the lock
// clears. Used to implement TAS as an atomic two-transaction RMW.
//
// All accepted writes are broadcast on the snoop output (1-cycle delayed) so
// every cache can invalidate matching lines.
module m68k_bus #(
    parameter N_PORTS  = 4,
    parameter MEM_WORDS = 16384,         // 64 KB
    parameter MEM_HEXFILE = "program.hex"
)(
    input  wire                 clk,
    input  wire                 rst_n,

    input  wire [N_PORTS-1:0]      req,
    input  wire [N_PORTS-1:0]      we,
    input  wire [N_PORTS-1:0]      lock,
    input  wire [32*N_PORTS-1:0]   addr_flat,
    input  wire [32*N_PORTS-1:0]   wdata_flat,
    input  wire [4*N_PORTS-1:0]    be_flat,
    output reg  [N_PORTS-1:0]      grant,
    output reg  [N_PORTS-1:0]      resp_valid,
    output reg  [31:0]             resp_data,

    output reg                            snoop_valid,
    output reg  [31:0]                    snoop_addr,
    output reg  [$clog2(N_PORTS)-1:0]     snoop_src_id,

    // Memory-mapped IRQ controller: writes to address $FFFF_FFFC (i.e.
    // $FFFC.W with the 68000's sign-extension of absolute-short addresses)
    // latch the low 3 bits of wdata into irq_level (sticky; clear by
    // writing 0).
    output reg  [2:0]                     irq_level,

    // Framebuffer peek port. Combinational read-only access to the memory
    // array. Used by the simulator harness to render a window without
    // intruding on CPU bus traffic. In synthesis you would tie fb_peek_addr
    // off and ignore fb_peek_data, or remove this port entirely.
    input  wire [31:0]                    fb_peek_addr,
    output wire [31:0]                    fb_peek_data,

    // Blitter slave interface.  Writes/reads from BLT_BASE..BLT_BASE+0x3F
    // are diverted to this port instead of main memory.  The blitter module
    // sits on a separate master port (the highest port index) for its own
    // memory R/W; the slave port is purely the CPU programming interface.
    output reg                            blt_slv_req,
    output reg                            blt_slv_we,
    output reg  [5:0]                     blt_slv_addr,
    output reg  [3:0]                     blt_slv_be,
    output reg  [31:0]                    blt_slv_wdata,
    input  wire [31:0]                    blt_slv_rdata
);
    localparam [31:0] IRQ_REG_ADDR  = 32'hFFFF_FFFC;
    localparam [31:0] BLT_BASE      = 32'h00FE_0000;
    localparam [31:0] BLT_END       = 32'h00FE_003F;
    localparam PID_BITS = $clog2(N_PORTS);
    localparam AIDX_BITS = $clog2(MEM_WORDS);

    // Memory.
    reg [31:0] mem [0:MEM_WORDS-1];

    // Framebuffer peek (word index = fb_peek_addr[AIDX_BITS+1:2]).
    assign fb_peek_data = mem[fb_peek_addr[AIDX_BITS+1:2]];
    integer mi;
    initial begin
        for (mi = 0; mi < MEM_WORDS; mi = mi + 1) mem[mi] = 32'd0;
        $readmemh(MEM_HEXFILE, mem);
    end

    // Unflatten inputs.
    wire [31:0] addr  [0:N_PORTS-1];
    wire [31:0] wdata [0:N_PORTS-1];
    wire [3:0]  be    [0:N_PORTS-1];
    genvar g;
    generate
        for (g = 0; g < N_PORTS; g = g + 1) begin : unflat
            assign addr[g]  = addr_flat[32*g +: 32];
            assign wdata[g] = wdata_flat[32*g +: 32];
            assign be[g]    = be_flat[4*g +: 4];
        end
    endgenerate

    // Lock tracking.
    reg               lock_pending;
    reg [PID_BITS-1:0] lock_holder;

    // Round-robin pointer.
    reg [PID_BITS-1:0] rrobin;

    // Choose winner combinationally.
    reg [PID_BITS-1:0] winner;
    reg                winner_valid;
    integer            k;
    reg [PID_BITS:0]   cand_ext;     // one extra bit so + cannot wrap silently
    reg [PID_BITS-1:0] cand;

    always @* begin
        winner = {PID_BITS{1'b0}};
        winner_valid = 1'b0;
        cand = {PID_BITS{1'b0}};
        cand_ext = {(PID_BITS+1){1'b0}};
        if (lock_pending) begin
            winner = lock_holder;
            winner_valid = req[lock_holder];
        end else begin
            for (k = 0; k < N_PORTS; k = k + 1) begin
                cand_ext = {1'b0, rrobin} + k[PID_BITS:0];
                cand = (cand_ext >= N_PORTS[PID_BITS:0])
                       ? cand_ext[PID_BITS-1:0] - N_PORTS[PID_BITS-1:0]
                       : cand_ext[PID_BITS-1:0];
                if (!winner_valid && req[cand]) begin
                    winner = cand;
                    winner_valid = 1'b1;
                end
            end
        end
    end

    // Grant signals.
    integer pp;
    always @* begin
        grant = {N_PORTS{1'b0}};
        if (winner_valid) grant[winner] = 1'b1;
    end

    // Response routing: 1-cycle delay.
    reg                 granted_valid_q;
    reg [PID_BITS-1:0]  granted_port_q;
    reg                 granted_we_q;
    reg [AIDX_BITS-1:0] granted_idx_q;
    reg [31:0]          granted_addr_q;
    reg                 granted_is_irq_q;
    reg                 granted_is_blt_q;
    reg                 granted_blt_we_q;
    reg [31:0]          granted_blt_rdata_q;

    wire [AIDX_BITS-1:0] mem_idx = addr[winner][AIDX_BITS+1:2];
    wire is_irq_reg = (addr[winner] == IRQ_REG_ADDR);
    wire is_blt_reg = (addr[winner] >= BLT_BASE) && (addr[winner] <= BLT_END);

    // Combinational slave port to the blitter.  When the winning bus access
    // hits the blitter register space, we route the request to blt_slv_*
    // and prevent the memory write.  Reads return blt_slv_rdata via the
    // delayed-response path.
    always @* begin
        blt_slv_req   = winner_valid && is_blt_reg;
        blt_slv_we    = winner_valid && is_blt_reg && we[winner];
        blt_slv_addr  = addr[winner][5:0];
        blt_slv_be    = be[winner];
        blt_slv_wdata = wdata[winner];
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            lock_pending <= 1'b0;
            lock_holder  <= {PID_BITS{1'b0}};
            rrobin       <= {PID_BITS{1'b0}};
            granted_valid_q <= 1'b0;
            granted_port_q  <= {PID_BITS{1'b0}};
            granted_we_q    <= 1'b0;
            granted_idx_q   <= {AIDX_BITS{1'b0}};
            granted_addr_q  <= 32'd0;
            granted_is_irq_q <= 1'b0;
            granted_is_blt_q <= 1'b0;
            granted_blt_we_q <= 1'b0;
            granted_blt_rdata_q <= 32'd0;
            irq_level <= 3'd0;
        end else begin
            // Writes to the IRQ register update irq_level (sticky) and do NOT
            // commit to main memory. Writes to the blitter register region
            // are handled by the combinational slave port (no memory write).
            // Writes anywhere else update memory.
            if (winner_valid && we[winner] && is_irq_reg) begin
                if (be[winner][0]) irq_level <= wdata[winner][2:0];
            end else if (winner_valid && we[winner] && is_blt_reg) begin
                // Handled via blt_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner]) begin
                if (be[winner][3]) mem[mem_idx][31:24] <= wdata[winner][31:24];
                if (be[winner][2]) mem[mem_idx][23:16] <= wdata[winner][23:16];
                if (be[winner][1]) mem[mem_idx][15:8]  <= wdata[winner][15:8];
                if (be[winner][0]) mem[mem_idx][7:0]   <= wdata[winner][7:0];
            end

            // Latch for delayed response.
            granted_valid_q <= winner_valid;
            granted_port_q  <= winner;
            granted_we_q    <= winner_valid && we[winner];
            granted_idx_q   <= mem_idx;
            granted_addr_q  <= addr[winner];
            granted_is_irq_q <= winner_valid && is_irq_reg;
            granted_is_blt_q <= winner_valid && is_blt_reg;
            granted_blt_we_q <= winner_valid && is_blt_reg && we[winner];
            // Latch the blitter's combinational read result on the grant
            // cycle so the delayed response sees stable data.
            if (winner_valid && is_blt_reg && !we[winner])
                granted_blt_rdata_q <= blt_slv_rdata;

            // Round-robin advance.
            if (winner_valid && !lock_pending) begin
                if (winner == (N_PORTS-1))
                    rrobin <= {PID_BITS{1'b0}};
                else
                    rrobin <= winner + 1'b1;
            end

            // Lock update.
            if (winner_valid) begin
                if (lock_pending) begin
                    // Granted second txn of an RMW; release lock.
                    lock_pending <= 1'b0;
                end else if (lock[winner]) begin
                    lock_pending <= 1'b1;
                    lock_holder  <= winner;
                end
            end
        end
    end

    // Output response (1 cycle after grant). Reads of the IRQ register return
    // {29'b0, irq_level}; reads of the blitter region return blt_slv_rdata;
    // everything else reads from main memory.
    always @* begin
        resp_valid = {N_PORTS{1'b0}};
        resp_data  = granted_is_irq_q ? {29'd0, irq_level}
                   : granted_is_blt_q ? granted_blt_rdata_q
                   : mem[granted_idx_q];
        if (granted_valid_q) resp_valid[granted_port_q] = 1'b1;
    end

    // Snoop broadcast: every successful write goes out 1 cycle later.
    // (Suppress for blitter-register writes since those don't touch memory.)
    always @* begin
        snoop_valid  = granted_valid_q && granted_we_q && !granted_blt_we_q;
        snoop_addr   = granted_addr_q;
        snoop_src_id = granted_port_q;
    end
endmodule
