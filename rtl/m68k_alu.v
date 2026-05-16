`include "m68k_defs.vh"

// Combinational 32-bit ALU. Produces result + NZVC flags.
module m68k_alu (
    input  wire [3:0]  op,
    input  wire [31:0] a,
    input  wire [31:0] b,
    output reg  [31:0] y,
    output reg         flag_n,
    output reg         flag_z,
    output reg         flag_v,
    output reg         flag_c
);
    reg [32:0] sum;
    reg signed [31:0] sa, sb;

    always @* begin
        flag_n = 1'b0;
        flag_z = 1'b0;
        flag_v = 1'b0;
        flag_c = 1'b0;
        sa = a;
        sb = b;
        sum = 33'd0;

        case (op)
            `ALU_MOV, `ALU_PASSB: begin
                y = b;
                flag_n = b[31];
                flag_z = (b == 32'd0);
                // V and C cleared per 68K MOVE semantics.
            end
            `ALU_ADD: begin
                sum = {1'b0, a} + {1'b0, b};
                y = sum[31:0];
                flag_n = y[31];
                flag_z = (y == 32'd0);
                flag_c = sum[32];
                flag_v = (a[31] == b[31]) && (y[31] != a[31]);
            end
            `ALU_SUB, `ALU_CMP: begin
                sum = {1'b0, a} - {1'b0, b};
                y = (op == `ALU_CMP) ? a : sum[31:0];
                flag_n = sum[31];
                flag_z = (sum[31:0] == 32'd0);
                flag_c = sum[32];
                flag_v = (a[31] != b[31]) && (sum[31] != a[31]);
            end
            `ALU_AND: begin
                y = a & b;
                flag_n = y[31];
                flag_z = (y == 32'd0);
            end
            `ALU_OR: begin
                y = a | b;
                flag_n = y[31];
                flag_z = (y == 32'd0);
            end
            `ALU_EOR: begin
                y = a ^ b;
                flag_n = y[31];
                flag_z = (y == 32'd0);
            end
            `ALU_TAS: begin
                // Test-and-set: flags from B (the memory byte, zero-extended);
                // result is B with bit7 set (memory write uses only low byte).
                y = b | 32'h0000_0080;
                flag_n = b[7];
                flag_z = (b[7:0] == 8'd0);
            end
            default: begin
                y = 32'd0;
            end
        endcase
    end
endmodule
