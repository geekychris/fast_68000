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
    input  wire        cpu_is_long,
    output wire        cpu_ack,
    output wire [31:0] cpu_rdata,

    output wire        bus_req,
    output reg         bus_we,
    output reg         bus_lock,
    output reg  [31:0] bus_addr,
    output reg  [31:0] bus_wdata,
    output reg  [3:0]  bus_be,
    output reg         bus_is_long,
    input  wire        bus_grant,
    input  wire        bus_resp_valid,
    input  wire [31:0] bus_resp_data,

    input  wire                snoop_valid,
    input  wire [31:0]         snoop_addr,
    input  wire [ID_BITS-1:0]  snoop_src_id,

    // Cache-wide invalidate.  Used at reset and on OVL clear so the
    // pre-OVL ROM-overlay reads aren't returned as stale data once
    // the low-memory address space switches to RAM.
    input  wire                inval_all_i
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
    // Unaligned .L reads (cpu_is_long && cpu_addr[1:0]==10) must also go
    // to the bus -- a hit here would return the aligned longword at
    // (cpu_addr & ~3) rather than the spliced {low half, next high half}
    // the unaligned access needs.  Without this guard an RTE PC pop that
    // landed at SP[1:0]==10 (every RTE after the .W SR pop) could pull a
    // stale cached longword and redirect to garbage.
    wire is_unaligned_long = cpu_is_long && (cpu_addr[1:0] == 2'b10);
    wire comb_read_hit = (state == S_IDLE) && cpu_req && !cpu_we && !cpu_lock &&
                         !is_io && !is_unaligned_long && hit;

    // Combinational response while we're holding the BUS_WAIT state and the
    // bus has just returned. The state machine transitions back to S_IDLE on
    // the next clock edge, so this signal pulses for exactly one cycle and
    // the IF stage drops cpu_req before the cache becomes ready again.
    wire bus_resp_now = (state == S_BUS_WAIT) && bus_resp_valid;

    assign cpu_ack   = comb_read_hit || bus_resp_now;
    assign cpu_rdata = comb_read_hit ? data[idx] :
                       bus_resp_now ? bus_resp_data : 32'd0;

`ifdef KICKSTART_BOOT_TRACE
    // Trace any read of vector $6C (vec=27 level-3 IRQ).  Diagnosing
    // why the read returns 0 after the watchdog kicks at r=4436670.
    always @(posedge clk) if (rst_n && cpu_req && !cpu_we &&
                              (cpu_addr == 32'h0000_006C ||
                               cpu_addr == 32'h0000_002C)) begin
        $display("[CACHE-VEC] addr=%h hit=%b state=%d data[idx]=%h bus_resp_valid=%b bus_resp_data=%h ack=%b rdata=%h saved_idx=%h",
            cpu_addr, hit, state, data[idx], bus_resp_valid, bus_resp_data,
            cpu_ack, cpu_rdata, saved_idx);
    end
`endif

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
            bus_is_long <= 1'b0;
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
            if (inval_all_i) begin
                for (i = 0; i < NUM_LINES; i = i + 1)
                    valids[i] <= 1'b0;
            end

            case (state)
                S_IDLE: begin
                    if (cpu_req && cpu_we && WRITABLE) begin
                        // Writes to I/O regions also bypass cache fills
                        // (we update existing cached lines on hit, but
                        // never allocate for I/O addresses).
                        //
                        // Unaligned .L write (cpu_be == 4'b1111 &&
                        // cpu_addr[1:0] == 2'b10) straddles two mem[] entries
                        // on the bus side.  Our merge_be assumes aligned
                        // layout and would smear all 4 bytes of wdata across
                        // mem[idx], leaving the cache inconsistent with the
                        // (correctly split) bus state.  Force the line(s) to
                        // be invalidated so any subsequent read takes the
                        // bus path and gets the right bytes.
                        if (hit && !is_io && !((cpu_be == 4'b1111) && (cpu_addr[1:0] == 2'b10)))
                            data[idx] <= merge_be(data[idx], cpu_wdata, cpu_be);
                        if (hit && (cpu_be == 4'b1111) && (cpu_addr[1:0] == 2'b10))
                            valids[idx] <= 1'b0;
                        // Unaligned .L write also modifies the *next* mem[]
                        // entry on the bus.  Invalidate that cache line too
                        // (unconditionally, regardless of tag) so any cached
                        // aligned read of (cpu_addr & ~3) + 4 takes the bus
                        // path and sees the second half of the just-written
                        // longword.  Surfaced by the Musashi cosim at K1.3
                        // boot $FC15DA (MOVE.L D1, -(A0) unaligned): a prior
                        // aligned read of $1C84 was cached, then the
                        // unaligned write at $1C82 silently updated the
                        // second half of $1C84 in mem[], and the next read
                        // hit the stale cache.
                        if ((cpu_be == 4'b1111) && (cpu_addr[1:0] == 2'b10))
                            valids[idx + 1'b1] <= 1'b0;
                        bus_req_r <= 1'b1;
                        bus_we  <= 1'b1;
                        bus_lock <= cpu_lock;
                        bus_addr <= cpu_addr;
                        bus_wdata <= cpu_wdata;
                        bus_be <= cpu_be;
                        bus_is_long <= cpu_is_long;
                        saved_we <= 1'b1;
                        saved_io <= 1'b0;
                        saved_idx <= idx;
                        saved_tag <= tag_in;
                        state <= S_BUS_WAIT;
                    end else if (cpu_req && !cpu_we && (!hit || cpu_lock || is_io || is_unaligned_long)) begin
                        // Miss, locked read, I/O read, or unaligned .L read.
                        // I/O reads always go to the bus and never fill the
                        // cache so the CPU sees current device-register state.
                        // Unaligned .L reads need the bus's split-assembly
                        // and the spliced result must NOT be cached -- it
                        // doesn't correspond to any aligned mem[] longword,
                        // so caching it would corrupt the next read at the
                        // same aligned index.
                        bus_req_r <= 1'b1;
                        bus_we  <= 1'b0;
                        bus_lock <= cpu_lock;
                        bus_addr <= cpu_addr;
                        bus_be <= 4'b1111;
                        bus_is_long <= cpu_is_long;
                        saved_we <= 1'b0;
                        saved_idx <= idx;
                        saved_tag <= tag_in;
                        // Suppress fill for I/O reads and unaligned .L reads.
                        saved_io <= is_io || is_unaligned_long;
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
