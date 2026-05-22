`include "m68k_defs.vh"

// Unified 16-entry register file: D0..D7 (idx 0..7), A0..A7 (idx 8..15).
// 3 combinational read ports, 1 main write port (size-aware for Dn), 1
// auxiliary write port (always full long, used for An updates and SP push/pop).
//
// Writes to Dn (idx[3] == 0) are masked to the operand size: SZ_B writes
// only the low byte, SZ_W writes the low 16 bits, SZ_L replaces the whole
// register. Writes to An (idx[3] == 1) are always 32-bit per M68000 spec.
module m68k_regfile #(
    parameter [3:0] RESET_A7_IDX = 4'd15
)(
    input  wire        clk,
    input  wire        rst_n,
    input  wire [31:0] reset_a7,

    input  wire [3:0]  ra_idx,
    output wire [31:0] ra_data,
    input  wire [3:0]  rb_idx,
    output wire [31:0] rb_data,
    input  wire [3:0]  rc_idx,
    output wire [31:0] rc_data,
    // Fourth read port: dst-index register for `d8(An, Xn)` destinations.
    // rc is busy carrying src's xreg (or fallback A7) so when *both* src and
    // dst use EA_IDX with different Xn registers, rd provides the dst Xn.
    input  wire [3:0]  rd_idx,
    output wire [31:0] rd_data,

    input  wire        we,
    input  wire [3:0]  w_idx,
    input  wire [1:0]  w_size,
    input  wire [31:0] w_data,

    input  wire        aux_we,
    input  wire [3:0]  aux_idx,
    input  wire [31:0] aux_data
);
    reg [31:0] regs [0:15];
    integer i;

    assign ra_data = regs[ra_idx];
    assign rb_data = regs[rb_idx];
    assign rc_data = regs[rc_idx];
    assign rd_data = regs[rd_idx];

    function [31:0] sized_write;
        input [31:0] old_val, new_val;
        input [1:0]  sz;
        input        is_areg;
        begin
            if (is_areg) sized_write = new_val;
            else case (sz)
                `SZ_B: sized_write = {old_val[31:8], new_val[7:0]};
                `SZ_W: sized_write = {old_val[31:16], new_val[15:0]};
                default: sized_write = new_val;
            endcase
        end
    endfunction

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < 16; i = i + 1) regs[i] <= 32'd0;
            regs[RESET_A7_IDX] <= reset_a7;
        end else begin
            if (aux_we && !(we && aux_idx == w_idx))
                regs[aux_idx] <= aux_data;
            if (we)
                regs[w_idx] <= sized_write(regs[w_idx], w_data, w_size, w_idx[3]);
        end
    end
endmodule
