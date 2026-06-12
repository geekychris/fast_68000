// Bus adapter — translates m68k_core's 32-bit master interface
// (ic_*/dc_* request/ack/rdata pattern) into a 68000-style 16-bit
// external bus (_AS/_UDS/_LDS/RW + _DTACK).
//
// This is the interface minimig.v's chipset stack expects.  In our
// Phase 0 testbench the external side connects to a 68000-protocol
// memory model; in Phase 1 it'll connect to minimig.v itself.
//
// Bus protocol per transaction (synchronous model):
//   T0  : addr/rw/uds/lds driven; as_n asserted (low)
//   T1+ : memory sees as_n, drives dtack_n low + rdata (for reads)
//   Tn  : wrapper sees dtack_n=0; latches data; raises CPU ack;
//         drops as_n / uds_n / lds_n / dtack handshake
//   Tn+1: cycle returns to IDLE.
//
// For .L (32-bit) transactions we issue two consecutive word
// transactions, high half first (matches 68000 byte ordering).
//
// Arbitration: dc takes priority over ic (matches m68k_bus semantics).
//
// `dc_be[3:0]` from m68k_core covers the four bytes of the longword:
//   be[3] = bits 31:24 (= UDS lane of the high word)
//   be[2] = bits 23:16 (= LDS lane of the high word)
//   be[1] = bits 15:8  (= UDS lane of the low  word)
//   be[0] = bits 7:0   (= LDS lane of the low  word)
//
// For word transactions we map each pair (be[3:2] → first word UDS/LDS,
// be[1:0] → second word UDS/LDS).  For byte and aligned-word accesses
// only one of the two transactions usually has both strobes asserted.

module m68k_to_amiga_bus (
    input  wire        clk,
    input  wire        rst_n,

    // ---------------- CPU side (m68k_core ports) ----------------
    input  wire        ic_req,
    input  wire [31:0] ic_addr,
    input  wire [3:0]  ic_be,
    output reg         ic_ack,
    output reg  [31:0] ic_rdata,

    input  wire        dc_req,
    input  wire        dc_we,
    input  wire [31:0] dc_addr,
    input  wire [31:0] dc_wdata,
    input  wire [3:0]  dc_be,
    input  wire        dc_is_long,
    output reg         dc_ack,
    output reg  [31:0] dc_rdata,

    // ---------------- 68000-style external bus ----------------
    output reg  [23:1] addr_o,
    output reg  [15:0] data_o,
    input  wire [15:0] data_i,
    output reg         as_n,      // address strobe       (active low)
    output reg         uds_n,     // upper data strobe    (active low)
    output reg         lds_n,     // lower data strobe    (active low)
    output reg         rw,        // 1 = read, 0 = write
    input  wire        dtack_n    // data acknowledge     (active low)
);
    // MIN_AS_CYCLES: minimum host cycles to hold AS asserted before
    // releasing on DTACK.  Real 68000 holds AS for at least 4 clock
    // periods (S0..S7); our default of 1 is much shorter — fine when
    // talking to memory models that respond combinationally on AS
    // (e.g. our m68k_bus.v).  When the target is minimig.v's
    // clk7_en-gated m68k_bridge, we must hold AS for several host
    // cycles so the bridge's l_dtack latch can capture _ta_n=0.
    // Override at instantiation: `.MIN_AS_CYCLES(16)` for minimig.
    parameter MIN_AS_CYCLES = 1;

    // ---------------- State machine ----------------
    localparam S_IDLE   = 3'd0;
    localparam S_HI     = 3'd1;   // drive high-half word
    localparam S_HI_W   = 3'd2;   // wait for dtack on high half
    localparam S_LO     = 3'd3;   // drive low-half word
    localparam S_LO_W   = 3'd4;   // wait for dtack on low half
    localparam S_ACK    = 3'd5;   // assert CPU ack for one cycle

    reg [2:0]  state;
    reg        owner_dc;          // 1 = serving dc, 0 = ic
    reg        is_long;
    reg        is_write;
    reg [31:0] xact_addr;
    reg [31:0] xact_wdata;
    reg [3:0]  xact_be;
    reg [15:0] hi_rdata;          // latched high half for .L reads
    // Counter that runs while AS is asserted (S_HI_W, S_LO_W).  Bus
    // cycle completes when DTACK is low AND counter >= MIN_AS_CYCLES.
    reg [7:0]  as_hold_cnt;

    // ---------------- Arbitration: pick next requester ----------------
    wire pick_dc = dc_req;
    wire pick_ic = ic_req && !dc_req;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state    <= S_IDLE;
            owner_dc <= 1'b0;
            is_long  <= 1'b0;
            is_write <= 1'b0;
            ic_ack   <= 1'b0;
            dc_ack   <= 1'b0;
            ic_rdata <= 32'd0;
            dc_rdata <= 32'd0;
            addr_o   <= 23'd0;
            data_o   <= 16'd0;
            as_n     <= 1'b1;
            uds_n    <= 1'b1;
            lds_n    <= 1'b1;
            rw       <= 1'b1;
            hi_rdata <= 16'd0;
        end else begin
            ic_ack <= 1'b0;
            dc_ack <= 1'b0;

            case (state)
                // -------- IDLE: arbitrate next requester --------
                S_IDLE: begin
                    as_n  <= 1'b1; uds_n <= 1'b1; lds_n <= 1'b1; rw <= 1'b1;
                    if (pick_dc) begin
                        owner_dc   <= 1'b1;
                        is_long    <= dc_is_long;
                        is_write   <= dc_we;
                        xact_addr  <= dc_addr;
                        xact_wdata <= dc_wdata;
                        xact_be    <= dc_be;
                        state      <= S_HI;
                    end else if (pick_ic) begin
                        owner_dc   <= 1'b0;
                        is_long    <= 1'b1;       // I-fetch is always .L
                        is_write   <= 1'b0;
                        xact_addr  <= ic_addr;
                        xact_wdata <= 32'd0;
                        xact_be    <= ic_be;      // typically 1111
                        state      <= S_HI;
                    end
                end

                // -------- HI: drive high-half word --------
                S_HI: begin
                    addr_o <= xact_addr[23:1];
                    rw     <= ~is_write;
                    uds_n  <= ~xact_be[3];
                    lds_n  <= ~xact_be[2];
                    data_o <= xact_wdata[31:16];
                    as_n   <= 1'b0;
                    as_hold_cnt <= 8'd0;
                    state  <= S_HI_W;
                end

                // -------- HI_W: wait for DTACK on high half --------
                S_HI_W: begin
                    if (as_hold_cnt < MIN_AS_CYCLES) as_hold_cnt <= as_hold_cnt + 8'd1;
                    if (!dtack_n && as_hold_cnt >= MIN_AS_CYCLES) begin
                        if (!is_write) hi_rdata <= data_i;
                        // Drop strobes; move on.
                        as_n  <= 1'b1; uds_n <= 1'b1; lds_n <= 1'b1;
                        if (is_long) state <= S_LO;
                        else         state <= S_ACK;
                    end
                end

                // -------- LO: drive low-half word --------
                S_LO: begin
                    addr_o <= (xact_addr[23:1] + 23'd1);  // next word
                    rw     <= ~is_write;
                    uds_n  <= ~xact_be[1];
                    lds_n  <= ~xact_be[0];
                    data_o <= xact_wdata[15:0];
                    as_n   <= 1'b0;
                    as_hold_cnt <= 8'd0;
                    state  <= S_LO_W;
                end

                // -------- LO_W: wait for DTACK on low half --------
                S_LO_W: begin
                    if (as_hold_cnt < MIN_AS_CYCLES) as_hold_cnt <= as_hold_cnt + 8'd1;
                    if (!dtack_n && as_hold_cnt >= MIN_AS_CYCLES) begin
                        as_n <= 1'b1; uds_n <= 1'b1; lds_n <= 1'b1;
                        state <= S_ACK;
                        // For .L reads we combine hi_rdata + data_i here.
                    end
                end

                // -------- ACK: hand result back to CPU --------
                S_ACK: begin
                    if (owner_dc) begin
                        dc_ack <= 1'b1;
                        if (is_long) dc_rdata <= {hi_rdata, data_i};
                        else         dc_rdata <= {16'd0, hi_rdata};
                    end else begin
                        ic_ack <= 1'b1;
                        // I-fetch always .L
                        ic_rdata <= {hi_rdata, data_i};
                    end
                    state <= S_IDLE;
                end

                default: state <= S_IDLE;
            endcase
        end
    end

endmodule
