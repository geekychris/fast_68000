// Amiga-Copper-inspired display-list coprocessor (Phase 2).
//
// Reads a memory-resident program (the "Copper list") and executes simple
// MOVE / WAIT / HALT instructions on its own bus master port.  Used to
// chain blitter operations and reprogram chip registers without CPU
// involvement.
//
// Differences from the real Amiga Copper (intentional):
//   - 32-bit-friendly instruction encoding: each instruction is 8 bytes
//     (two longs) instead of the Amiga's two 16-bit words.
//   - Raster WAIT compares against Denise's `row_idx` only (no horizontal
//     position).  Sufficient for per-scanline palette/register swaps.
//
// Instruction encoding (per 8-byte slot):
//
//   long 0 = target address (32-bit byte address)
//   long 1 = data (32-bit, written to target on MOVE; Y position for
//                  raster WAIT/SKIP; ignored otherwise)
//
//   Special target values:
//       $FFFF_FFFF : HALT - stop the Copper, COP_BUSY <- 0.
//       $FFFF_FFFE : WAIT-BLITTER - block until blitter goes busy then
//                    idle.
//       $FFFF_FFFD : SKIP-IF-BLITTER-BUSY - skip the next instruction
//                    (8 bytes) if the blitter is currently busy.
//       $FFFF_FFFC : WAIT-RASTER - block until vbeam_i >= data_q[15:0].
//                    Used to synchronise to Denise's scanline so a
//                    subsequent MOVE rewrites a Denise register before
//                    the rasterizer reaches the relevant rows.
//       $FFFF_FFFB : SKIP-IF-RASTER-PAST - skip the next instruction if
//                    vbeam_i >= data_q[15:0].
//       (everything else) : MOVE - write long 1 to the target address.
//
//   MOVE writes are 32-bit-wide bus transactions with BE = 4'b1111.
//   For 16-bit blitter half-registers, the receiver simply latches the
//   full 32 bits (low 16 are usually the relevant value in our blitter
//   register-set, which is already 32-bit-wide).
//
// Register map (CPU-facing slave port at $00FE_0040..$00FE_007F):
//
//   $00FE_0040 COP1LC   RW   32-bit byte address of Copper list 1
//   $00FE_0044 COPJMP1  W    write any value to (re)start the Copper at COP1LC
//   $00FE_0048 COPSTAT  RO   bit 0 = COP_BUSY
//   $00FE_004C COP2LC   RW   32-bit byte address of Copper list 2
//   $00FE_0050 COPJMP2  W    write any value to (re)start the Copper at COP2LC



module copper (
    input  wire        clk,
    input  wire        rst_n,

    // CPU slave port
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [5:0]  slv_addr,    // byte offset $40..$7F (we see $00..$3F via the mask)
    input  wire [3:0]  slv_be,
    input  wire [31:0] slv_wdata,
    output reg  [31:0] slv_rdata,

    // Bus master (read instructions + execute MOVE writes)
    output wire        mst_req,
    output reg         mst_we,
    output reg  [31:0] mst_addr,
    output reg  [31:0] mst_wdata,
    output reg  [3:0]  mst_be,
    input  wire        mst_ack,
    input  wire [31:0] mst_rdata,

    // Live blitter-busy signal so we can implement WAIT.
    input  wire        blt_busy_i,

    // Current Denise raster row (valid while Denise is rasterising; held
    // at its last value when idle).  Lets the Copper synchronise to the
    // bitplane pipeline so it can rewrite Denise registers per-scanline.
    input  wire [15:0] vbeam_i
);
    // ---------------- Registers programmed by the CPU ----------------
    reg [31:0] cop1lc;
    reg [31:0] cop2lc;
    reg        cop_busy;

    // ---------------- Internal state ---------------------------------
    localparam S_IDLE       = 4'd0;
    localparam S_FETCH_T    = 4'd1;   // fetch instruction word 0 (target)
    localparam S_FETCH_D    = 4'd2;   // fetch instruction word 1 (data)
    localparam S_DECODE     = 4'd3;
    localparam S_WRITE      = 4'd4;   // MOVE: write data to target
    localparam S_WAIT_RISE  = 4'd5;   // WAIT-BLITTER: blitter must go busy first
    localparam S_WAIT_FALL  = 4'd6;   // WAIT-BLITTER: ...then idle
    localparam S_HALT       = 4'd7;
    localparam S_WAIT_VBEAM = 4'd8;   // WAIT-RASTER: vbeam_i >= data_q[15:0]

    reg [3:0]  state;
    reg [31:0] pc;
    reg [31:0] target_q;
    reg [31:0] data_q;

    // Master-request gating: drop combinationally on ack so the arbiter
    // sees one request per transaction.
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

    // ---------------- CPU slave reads --------------------------------
    always @* begin
        case (slv_addr[4:2])
            3'd0: slv_rdata = cop1lc;             // $40 COP1LC
            3'd1: slv_rdata = 32'd0;              // $44 COPJMP1 (write-only)
            3'd2: slv_rdata = {31'd0, cop_busy};  // $48 COPSTAT
            3'd3: slv_rdata = cop2lc;             // $4C COP2LC
            3'd4: slv_rdata = 32'd0;              // $50 COPJMP2 (write-only)
            default: slv_rdata = 32'd0;
        endcase
    end

    // ---------------- Main always block ------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cop1lc    <= 32'd0;
            cop2lc    <= 32'd0;
            cop_busy  <= 1'b0;
            state     <= S_IDLE;
            pc        <= 32'd0;
            target_q  <= 32'd0;
            data_q    <= 32'd0;
            mst_req_r <= 1'b0;
            mst_we    <= 1'b0;
            mst_addr  <= 32'd0;
            mst_wdata <= 32'd0;
            mst_be    <= 4'b0000;
        end else begin
            // -------- CPU slave writes --------
            if (slv_req && slv_we) begin
                case (slv_addr[4:2])
                    3'd0: cop1lc <= slv_wdata;        // COP1LC
                    3'd1: begin                        // COPJMP1: strobe-start at COP1LC
                        if (!cop_busy) begin
                            pc       <= cop1lc;
                            cop_busy <= 1'b1;
                            state    <= S_FETCH_T;
                        end
                    end
                    3'd3: cop2lc <= slv_wdata;        // COP2LC
                    3'd4: begin                        // COPJMP2: strobe-start at COP2LC
                        if (!cop_busy) begin
                            pc       <= cop2lc;
                            cop_busy <= 1'b1;
                            state    <= S_FETCH_T;
                        end
                    end
                    default: ;
                endcase
            end

            // -------- Copper state machine --------
            case (state)
                S_IDLE: begin
                    mst_req_r <= 1'b0;
                end

                S_FETCH_T: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b0;
                    mst_addr  <= pc;
                    mst_be    <= 4'b1111;
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        target_q  <= mst_rdata;
                        pc        <= pc + 32'd4;
                        state     <= S_FETCH_D;
                    end
                end

                S_FETCH_D: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b0;
                    mst_addr  <= pc;
                    mst_be    <= 4'b1111;
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        data_q    <= mst_rdata;
                        pc        <= pc + 32'd4;
                        state     <= S_DECODE;
                    end
                end

                S_DECODE: begin
                    if (target_q == 32'hFFFF_FFFF) begin
                        state <= S_HALT;
                    end else if (target_q == 32'hFFFF_FFFE) begin
                        state <= S_WAIT_RISE;
                    end else if (target_q == 32'hFFFF_FFFD) begin
                        // SKIP-IF-BLITTER-BUSY: if busy, skip next 8 bytes.
                        if (blt_busy_i) pc <= pc + 32'd8;
                        state <= S_FETCH_T;
                    end else if (target_q == 32'hFFFF_FFFC) begin
                        // WAIT-RASTER: pause until vbeam_i reaches data_q[15:0].
                        state <= S_WAIT_VBEAM;
                    end else if (target_q == 32'hFFFF_FFFB) begin
                        // SKIP-IF-RASTER-PAST-Y: skip next 8 bytes if
                        // vbeam_i is already at/past the target Y.
                        if (vbeam_i >= data_q[15:0]) pc <= pc + 32'd8;
                        state <= S_FETCH_T;
                    end else begin
                        state <= S_WRITE;
                    end
                end

                S_WRITE: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b1;
                    mst_addr  <= target_q;
                    mst_wdata <= data_q;
                    mst_be    <= 4'b1111;
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        state     <= S_FETCH_T;
                    end
                end

                // WAIT semantics: ensure we observe blt_busy going high
                // (so the previous MOVE-to-BLTSIZE has been latched) and
                // then going low.
                S_WAIT_RISE: begin
                    if (blt_busy_i) state <= S_WAIT_FALL;
                end

                S_WAIT_FALL: begin
                    if (!blt_busy_i) state <= S_FETCH_T;
                end

                S_WAIT_VBEAM: begin
                    if (vbeam_i >= data_q[15:0]) state <= S_FETCH_T;
                end

                S_HALT: begin
                    cop_busy <= 1'b0;
                    state    <= S_IDLE;
                end

                default: state <= S_IDLE;
            endcase
        end
    end

endmodule
