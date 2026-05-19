// Bus passthrough with the same external interface as m68k_cache.v but no
// storage. Every CPU request becomes a bus request. Used to build a
// "basic 68000"-style reference SoC with no cache speedup.
//
// Like the cache, cpu_ack and cpu_rdata are driven combinationally during the
// bus-response cycle so the CPU sees the same handshake style on both.
module m68k_passthrough #(
    parameter NUM_LINES = 256,             // ignored
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
    input  wire [ID_BITS-1:0]  snoop_src_id,

    // No-op for passthrough (no storage to invalidate). Present so the
    // module interface matches m68k_cache.
    input  wire                inval_all_i
);
    localparam S_IDLE = 1'b0;
    localparam S_WAIT = 1'b1;
    reg state;

    reg bus_req_r;
    assign bus_req = bus_req_r && !bus_resp_valid;

    /* verilator lint_off UNUSED */
    wire _unused = &{1'b0, bus_grant, snoop_valid, snoop_addr, snoop_src_id, inval_all_i, 1'b0};
    /* verilator lint_on UNUSED */

    wire bus_resp_now = (state == S_WAIT) && bus_resp_valid;
    assign cpu_ack   = bus_resp_now;
    assign cpu_rdata = bus_resp_now ? bus_resp_data : 32'd0;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= S_IDLE;
            bus_req_r <= 1'b0;
            bus_we <= 1'b0;
            bus_lock <= 1'b0;
            bus_addr <= 32'd0;
            bus_wdata <= 32'd0;
            bus_be <= 4'b0;
        end else begin
            case (state)
                S_IDLE: begin
                    if (cpu_req) begin
                        bus_req_r <= 1'b1;
                        bus_we <= cpu_we && WRITABLE[0];
                        bus_lock <= cpu_lock;
                        bus_addr <= cpu_addr;
                        bus_wdata <= cpu_wdata;
                        bus_be <= cpu_be;
                        state <= S_WAIT;
                    end
                end
                S_WAIT: begin
                    if (bus_resp_valid) begin
                        bus_req_r <= 1'b0;
                        bus_we <= 1'b0;
                        bus_lock <= 1'b0;
                        state <= S_IDLE;
                    end
                end
                default: state <= S_IDLE;
            endcase
        end
    end
endmodule
