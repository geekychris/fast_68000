// Direct-mapped, 1-word-line, write-through cache with combinational read-hit
// response.
//
// Reads:  on hit cpu_ack + cpu_rdata are valid combinationally the same cycle
//         that cpu_req is asserted, so back-to-back hits run at 1 transaction
//         per cycle. On miss, issue a bus read, fill, then ack one cycle after
//         the bus response arrives.
// Writes: always write-through. Update the cache line on hit; issue bus write.
//         cpu_ack pulses one cycle after the bus completes the write.
// Snoop:  other-core writes to a matching line invalidate it (registered).
// Lock:   forwarded to the bus on TAS.
//
// A new request must not be issued while the cache is in S_BUS_WAIT (the IF
// stage observes cpu_ack and naturally won't, but the protocol allows
// holding cpu_req high in the meantime).
module m68k_cache #(
    parameter NUM_LINES = 256,
    parameter ID_BITS   = 2,
    parameter [ID_BITS-1:0] CORE_ID = 0,
    parameter           WRITABLE = 1
)(
    input  wire        clk,
    input  wire        rst_n,

    input  wire        cpu_req,
    input  wire        cpu_we,
    input  wire        cpu_lock,
    input  wire [31:0] cpu_addr,
    input  wire [31:0] cpu_wdata,
    input  wire [3:0]  cpu_be,
    output wire        cpu_ack,
    output wire [31:0] cpu_rdata,

    output wire        bus_req,
    output reg         bus_we,
    output reg         bus_lock,
    output reg  [31:0] bus_addr,
    output reg  [31:0] bus_wdata,
    output reg  [3:0]  bus_be,
    input  wire        bus_grant,
    input  wire        bus_resp_valid,
    input  wire [31:0] bus_resp_data,

    input  wire                snoop_valid,
    input  wire [31:0]         snoop_addr,
    input  wire [ID_BITS-1:0]  snoop_src_id
);
    localparam IDX_BITS = $clog2(NUM_LINES);
    localparam TAG_BITS = 32 - IDX_BITS - 2;

    reg [TAG_BITS-1:0] tags   [0:NUM_LINES-1];
    reg                valids [0:NUM_LINES-1];
    reg [31:0]         data   [0:NUM_LINES-1];

    integer i;

    wire [IDX_BITS-1:0] idx       = cpu_addr[IDX_BITS+1:2];
    wire [TAG_BITS-1:0] tag_in    = cpu_addr[31:IDX_BITS+2];
    wire                hit       = valids[idx] && (tags[idx] == tag_in);

    wire [IDX_BITS-1:0] snoop_idx = snoop_addr[IDX_BITS+1:2];
    wire [TAG_BITS-1:0] snoop_tag = snoop_addr[31:IDX_BITS+2];
    wire                snoop_invalidate = snoop_valid &&
                                           (snoop_src_id != CORE_ID) &&
                                           valids[snoop_idx] &&
                                           (tags[snoop_idx] == snoop_tag);

    localparam S_IDLE     = 1'b0;
    localparam S_BUS_WAIT = 1'b1;
    reg state;

    reg                 saved_we;
    reg                 saved_io;
    reg [IDX_BITS-1:0]  saved_idx;
    reg [TAG_BITS-1:0]  saved_tag;

    // Registered bus_req goes to 0 combinationally when bus_resp_valid arrives,
    // so the arbiter sees exactly one request per transaction (otherwise the
    // request stays asserted into the response cycle and the arbiter grants
    // twice, which breaks the lock-pending counter).
    reg bus_req_r;
    assign bus_req = bus_req_r && !bus_resp_valid;

    // I/O region: any address with [31:24] >= 8'hF0 is treated as
    // uncached (memory-mapped device registers).  This covers the IRQ
    // register at $FFFF_FFFC and the Kickstart ROM at $F8_0000+.  We also
    // cover our legacy chipset window $00FE_xxxx, the canonical Amiga
    // chipset at $00DF_F000-$00DF_FFFF, and the canonical CIA pages at
    // $00BF_E000-$00BF_EFFF and $00BF_D000-$00BF_DFFF -- writes/reads
    // there must always observe the live device state, not a cached copy
    // from a previous access via the alternate address.
    wire is_io = (cpu_addr[31:24] >= 8'hF0) ||
                 (cpu_addr[31:16] == 16'h00FE) ||
                 (cpu_addr[31:12] == 20'h00DFF) ||
                 (cpu_addr[31:12] == 20'h00BFE) ||
                 (cpu_addr[31:12] == 20'h00BFD) ||
                 // Zorro autoconfig window: card-presence + base writes
                 // change state, so caching would mask the post-shutup
                 // $FFFFFFFF response.
                 (cpu_addr[31:16] == 16'h00E8);

    // Combinational read-hit response (no state change).
    // Locked reads (TAS phase 0) always go to the bus so the arbiter sees
    // the lock and serialises against other cores.  I/O reads are also
    // forced to the bus so device-register changes are visible.
    wire comb_read_hit = (state == S_IDLE) && cpu_req && !cpu_we && !cpu_lock && !is_io && hit;

    // Combinational response while we're holding the BUS_WAIT state and the
    // bus has just returned. The state machine transitions back to S_IDLE on
    // the next clock edge, so this signal pulses for exactly one cycle and
    // the IF stage drops cpu_req before the cache becomes ready again.
    wire bus_resp_now = (state == S_BUS_WAIT) && bus_resp_valid;

    assign cpu_ack   = comb_read_hit || bus_resp_now;
    assign cpu_rdata = comb_read_hit ? data[idx] :
                       bus_resp_now ? bus_resp_data : 32'd0;

    function [31:0] merge_be;
        input [31:0] old_w, new_w;
        input [3:0]  be;
        begin
            merge_be[31:24] = be[3] ? new_w[31:24] : old_w[31:24];
            merge_be[23:16] = be[2] ? new_w[23:16] : old_w[23:16];
            merge_be[15:8]  = be[1] ? new_w[15:8]  : old_w[15:8];
            merge_be[7:0]   = be[0] ? new_w[7:0]   : old_w[7:0];
        end
    endfunction

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= S_IDLE;
            bus_req_r <= 1'b0;
            bus_we <= 1'b0;
            bus_lock <= 1'b0;
            bus_addr <= 32'd0;
            bus_wdata <= 32'd0;
            bus_be <= 4'b0;
            saved_we <= 1'b0;
            saved_io <= 1'b0;
            saved_idx <= {IDX_BITS{1'b0}};
            saved_tag <= {TAG_BITS{1'b0}};
            for (i = 0; i < NUM_LINES; i = i + 1) begin
                tags[i]   <= {TAG_BITS{1'b0}};
                valids[i] <= 1'b0;
                data[i]   <= 32'd0;
            end
        end else begin
            if (snoop_invalidate)
                valids[snoop_idx] <= 1'b0;

            case (state)
                S_IDLE: begin
                    if (cpu_req && cpu_we && WRITABLE) begin
                        // Writes to I/O regions also bypass cache fills
                        // (we update existing cached lines on hit, but
                        // never allocate for I/O addresses).
                        if (hit && !is_io)
                            data[idx] <= merge_be(data[idx], cpu_wdata, cpu_be);
                        bus_req_r <= 1'b1;
                        bus_we  <= 1'b1;
                        bus_lock <= cpu_lock;
                        bus_addr <= cpu_addr;
                        bus_wdata <= cpu_wdata;
                        bus_be <= cpu_be;
                        saved_we <= 1'b1;
                        saved_io <= 1'b0;
                        saved_idx <= idx;
                        saved_tag <= tag_in;
                        state <= S_BUS_WAIT;
                    end else if (cpu_req && !cpu_we && (!hit || cpu_lock || is_io)) begin
                        // Miss, locked read, or I/O read.  I/O reads always
                        // go to the bus and never fill the cache so that the
                        // CPU sees current device-register state.
                        bus_req_r <= 1'b1;
                        bus_we  <= 1'b0;
                        bus_lock <= cpu_lock;
                        bus_addr <= cpu_addr;
                        bus_be <= 4'b1111;
                        saved_we <= 1'b0;
                        saved_idx <= idx;
                        saved_tag <= tag_in;
                        // Suppress fill for I/O reads by remembering it was
                        // an I/O access; bypass storing the result on resp.
                        saved_io <= is_io;
                        state <= S_BUS_WAIT;
                    end
                end

                S_BUS_WAIT: begin
                    if (bus_resp_valid) begin
                        if (!saved_we && !saved_io) begin
                            tags[saved_idx]   <= saved_tag;
                            valids[saved_idx] <= 1'b1;
                            data[saved_idx]   <= bus_resp_data;
                        end
                        bus_req_r <= 1'b0;
                        bus_we    <= 1'b0;
                        bus_lock  <= 1'b0;
                        state     <= S_IDLE;
                    end
                end

                default: state <= S_IDLE;
            endcase
        end
    end
endmodule
