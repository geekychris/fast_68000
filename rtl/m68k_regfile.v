// Unified 16-entry register file: D0..D7, A0..A7. 32-bit each.
// Two read ports, one write port. Combinational read.
// Writeback to A7 uses port A; SP adjustments from CALL/RET/auto-inc use a
// separate dedicated update port to avoid contention with the normal WB path.
module m68k_regfile #(
    parameter [3:0] RESET_A7_IDX = 4'd15
)(
    input  wire        clk,
    input  wire        rst_n,
    input  wire [31:0] reset_a7,

    // Read ports. Index high bit = 0 -> Dn, 1 -> An.
    input  wire [3:0]  ra_idx,
    output wire [31:0] ra_data,
    input  wire [3:0]  rb_idx,
    output wire [31:0] rb_data,
    input  wire [3:0]  rc_idx,
    output wire [31:0] rc_data,

    // Write port (instruction writeback).
    input  wire        we,
    input  wire [3:0]  w_idx,
    input  wire [31:0] w_data,

    // Auxiliary write port (auto-inc/dec, SP push/pop). Same-cycle priority
    // is `we` over `aux_we` if they target the same index — primary WB wins.
    input  wire        aux_we,
    input  wire [3:0]  aux_idx,
    input  wire [31:0] aux_data
);
    reg [31:0] regs [0:15];
    integer i;

    assign ra_data = regs[ra_idx];
    assign rb_data = regs[rb_idx];
    assign rc_data = regs[rc_idx];

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < 16; i = i + 1) regs[i] <= 32'd0;
            regs[RESET_A7_IDX] <= reset_a7;
        end else begin
            // Primary WB wins on conflict.
            if (aux_we && !(we && aux_idx == w_idx))
                regs[aux_idx] <= aux_data;
            if (we)
                regs[w_idx] <= w_data;
        end
    end
endmodule
