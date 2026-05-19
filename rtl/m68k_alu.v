`include "m68k_defs.vh"

// Size-aware 32-bit ALU. Inputs are always 32-bit; only the low byte / word
// of the inputs (and result) is "real" for SZ_B / SZ_W operations, but the
// caller writes only the low part of the destination register so the upper
// bits don't get mangled.
//
// Outputs the new NZVCX flag values. Caller decides which flags actually get
// written to CCR (per M68000 spec — some ops don't touch X, MOVE/MOVEQ clear
// V and C, CMP doesn't touch X, etc.).
module m68k_alu (
    input  wire [4:0]  op,
    input  wire [1:0]  size,        // SZ_B / SZ_W / SZ_L
    input  wire        x_in,        // current X flag (for ADDX/SUBX/ROXL/ROXR/NEGX)
    input  wire [31:0] a,
    input  wire [31:0] b,
    input  wire [5:0]  shamt,       // shift count, 1..63 (modulo 64)
    output reg  [31:0] y,
    output reg         flag_n,
    output reg         flag_z,
    output reg         flag_v,
    output reg         flag_c,
    output reg         flag_x
);
    // Width masks / sign-bit positions.
    wire [31:0] mask     = (size == `SZ_B) ? 32'h0000_00FF :
                           (size == `SZ_W) ? 32'h0000_FFFF : 32'hFFFF_FFFF;
    wire [5:0]  msb_pos  = (size == `SZ_B) ? 6'd7  :
                           (size == `SZ_W) ? 6'd15 : 6'd31;
    wire [31:0] msb_mask = (size == `SZ_B) ? 32'h0000_0080 :
                           (size == `SZ_W) ? 32'h0000_8000 : 32'h8000_0000;

    wire [31:0] a_sized = a & mask;
    wire [31:0] b_sized = b & mask;
    wire        a_neg   = (a_sized & msb_mask) != 0;
    wire        b_neg   = (b_sized & msb_mask) != 0;

    reg  [32:0] sum;
    reg  [31:0] sub_res;
    reg         res_neg;
    reg  [31:0] shift_val;
    reg  [5:0]  shift_n;
    reg  [31:0] shift_y;
    reg         shift_c, shift_x_out;
    reg         shift_v;

    integer i;

    always @* begin
        // Defaults.
        y = 32'd0;
        flag_n = 1'b0;
        flag_z = 1'b0;
        flag_v = 1'b0;
        flag_c = 1'b0;
        flag_x = x_in;
        sum = 33'd0;
        sub_res = 32'd0;
        res_neg = 1'b0;
        shift_val = 32'd0;
        shift_n   = 6'd0;
        shift_y   = 32'd0;
        shift_c   = 1'b0;
        shift_x_out = x_in;
        shift_v   = 1'b0;

        case (op)
            `ALU_MOV, `ALU_PASSB: begin
                y = b_sized;
                flag_n = b_neg;
                flag_z = (b_sized == 32'd0);
                // V, C cleared. X untouched (MOVE doesn't touch X).
            end
            `ALU_ADD: begin
                sum = {1'b0, a_sized} + {1'b0, b_sized};
                y = sum[31:0] & mask;
                res_neg = (y & msb_mask) != 0;
                flag_n = res_neg;
                flag_z = (y == 32'd0);
                // Carry from sized add: bit msb_pos+1 of sum.
                flag_c = sum[msb_pos+1];
                flag_x = sum[msb_pos+1];
                flag_v = (a_neg == b_neg) && (res_neg != a_neg);
            end
            `ALU_SUB, `ALU_CMP: begin
                sum = {1'b0, a_sized} - {1'b0, b_sized};
                sub_res = sum[31:0] & mask;
                y = (op == `ALU_CMP) ? a : sub_res;
                res_neg = (sub_res & msb_mask) != 0;
                flag_n = res_neg;
                flag_z = (sub_res == 32'd0);
                flag_c = sum[msb_pos+1];
                flag_v = (a_neg != b_neg) && (res_neg != a_neg);
                if (op != `ALU_CMP) flag_x = sum[msb_pos+1];
            end
            `ALU_AND: begin
                y = a_sized & b_sized;
                flag_n = (y & msb_mask) != 0;
                flag_z = (y == 32'd0);
            end
            `ALU_OR: begin
                y = a_sized | b_sized;
                flag_n = (y & msb_mask) != 0;
                flag_z = (y == 32'd0);
            end
            `ALU_EOR: begin
                y = a_sized ^ b_sized;
                flag_n = (y & msb_mask) != 0;
                flag_z = (y == 32'd0);
            end
            `ALU_NEG: begin
                sum = 33'd0 - {1'b0, b_sized};
                y = sum[31:0] & mask;
                flag_n = (y & msb_mask) != 0;
                flag_z = (y == 32'd0);
                flag_c = (b_sized != 0);
                flag_x = (b_sized != 0);
                flag_v = b_neg && ((y & msb_mask) != 0);
            end
            // ADDX/SUBX/NEGX: like ADD/SUB/NEG but include X.  Z is
            // returned as the raw "result==0" bit; the caller ANDs it
            // with the previous Z (68k Z-preserve semantics).
            `ALU_ADDX: begin
                sum = {1'b0, a_sized} + {1'b0, b_sized} + {32'd0, x_in};
                y = sum[31:0] & mask;
                res_neg = (y & msb_mask) != 0;
                flag_n = res_neg;
                flag_z = (y == 32'd0);
                flag_c = sum[msb_pos+1];
                flag_x = sum[msb_pos+1];
                flag_v = (a_neg == b_neg) && (res_neg != a_neg);
            end
            `ALU_SUBX: begin
                sum = {1'b0, a_sized} - {1'b0, b_sized} - {32'd0, x_in};
                sub_res = sum[31:0] & mask;
                y = sub_res;
                res_neg = (sub_res & msb_mask) != 0;
                flag_n = res_neg;
                flag_z = (sub_res == 32'd0);
                flag_c = sum[msb_pos+1];
                flag_x = sum[msb_pos+1];
                flag_v = (a_neg != b_neg) && (res_neg != a_neg);
            end
            `ALU_NEGX: begin
                sum = 33'd0 - {1'b0, b_sized} - {32'd0, x_in};
                y = sum[31:0] & mask;
                res_neg = (y & msb_mask) != 0;
                flag_n = res_neg;
                flag_z = (y == 32'd0);
                flag_c = sum[msb_pos+1];
                flag_x = sum[msb_pos+1];
                flag_v = b_neg && res_neg;
            end
            `ALU_NOT: begin
                y = (~b_sized) & mask;
                flag_n = (y & msb_mask) != 0;
                flag_z = (y == 32'd0);
            end
            `ALU_CLR: begin
                y = 32'd0;
                flag_z = 1'b1;
                // N, V, C cleared. X untouched.
            end
            `ALU_TST: begin
                y = b_sized;
                flag_n = b_neg;
                flag_z = (b_sized == 32'd0);
            end
            `ALU_EXT_W: begin
                // Sign-extend byte→word: take low byte of b, sign-ext into bits [15:8].
                y = {b[31:16], {8{b[7]}}, b[7:0]};
                flag_n = b[7];
                flag_z = (b[7:0] == 8'd0);
            end
            `ALU_EXT_L: begin
                // Sign-extend word→long.
                y = {{16{b[15]}}, b[15:0]};
                flag_n = b[15];
                flag_z = (b[15:0] == 16'd0);
            end
            `ALU_SWAP: begin
                y = {b[15:0], b[31:16]};
                flag_n = b[15];
                flag_z = (y == 32'd0);
            end
            `ALU_TAS: begin
                // Test-and-set: flags from B (byte), result is B | 0x80.
                y = b | 32'h0000_0080;
                flag_n = b[7];
                flag_z = (b[7:0] == 8'd0);
            end
            `ALU_ASL, `ALU_LSL, `ALU_ROL, `ALU_ROXL: begin
                shift_val = b_sized;
                shift_n   = shamt[5:0];
                shift_y   = shift_val;
                shift_c   = flag_c;
                shift_x_out = x_in;
                shift_v   = 1'b0;
                for (i = 0; i < 64; i = i + 1) begin
                    if (i < shift_n) begin
                        // Shift one bit left.
                        case (op)
                            `ALU_ASL, `ALU_LSL: begin
                                shift_c = (shift_y & msb_mask) != 0;
                                shift_x_out = shift_c;
                                shift_y = ((shift_y << 1)) & mask;
                                if (op == `ALU_ASL) begin
                                    // V set if MSB changed at any time during shift
                                    if (((shift_y ^ ((shift_y >> 1) | (shift_c ? msb_mask : 32'd0))) & msb_mask) != 0)
                                        shift_v = 1'b1;
                                end
                            end
                            `ALU_ROL: begin
                                shift_c = (shift_y & msb_mask) != 0;
                                shift_y = ((shift_y << 1) | (shift_c ? 32'd1 : 32'd0)) & mask;
                            end
                            `ALU_ROXL: begin
                                shift_c = (shift_y & msb_mask) != 0;
                                shift_y = ((shift_y << 1) | (shift_x_out ? 32'd1 : 32'd0)) & mask;
                                shift_x_out = shift_c;
                            end
                            default: ;
                        endcase
                    end
                end
                y = (b & ~mask) | (shift_y & mask);
                flag_n = (shift_y & msb_mask) != 0;
                flag_z = ((shift_y & mask) == 32'd0);
                flag_c = (shift_n == 0) ? 1'b0 : shift_c;
                flag_v = (op == `ALU_ASL) ? shift_v : 1'b0;
                if (op == `ALU_ROL) flag_x = x_in;
                else                flag_x = (shift_n == 0) ? x_in : shift_x_out;
            end
            `ALU_ASR, `ALU_LSR, `ALU_ROR, `ALU_ROXR: begin
                shift_val = b_sized;
                shift_n   = shamt[5:0];
                shift_y   = shift_val;
                shift_c   = flag_c;
                shift_x_out = x_in;
                for (i = 0; i < 64; i = i + 1) begin
                    if (i < shift_n) begin
                        case (op)
                            `ALU_ASR: begin
                                shift_c = shift_y[0];
                                shift_x_out = shift_c;
                                // Arithmetic: replicate MSB.
                                shift_y = ((shift_y >> 1) |
                                           ((shift_y & msb_mask) ? msb_mask : 32'd0)) & mask;
                            end
                            `ALU_LSR: begin
                                shift_c = shift_y[0];
                                shift_x_out = shift_c;
                                shift_y = (shift_y >> 1) & mask;
                            end
                            `ALU_ROR: begin
                                shift_c = shift_y[0];
                                shift_y = ((shift_y >> 1) | (shift_c ? msb_mask : 32'd0)) & mask;
                            end
                            `ALU_ROXR: begin
                                shift_c = shift_y[0];
                                shift_y = ((shift_y >> 1) | (shift_x_out ? msb_mask : 32'd0)) & mask;
                                shift_x_out = shift_c;
                            end
                            default: ;
                        endcase
                    end
                end
                y = (b & ~mask) | (shift_y & mask);
                flag_n = (shift_y & msb_mask) != 0;
                flag_z = ((shift_y & mask) == 32'd0);
                flag_c = (shift_n == 0) ? 1'b0 : shift_c;
                flag_v = 1'b0;  // ASR/LSR/ROR/ROXR clear V
                if (op == `ALU_ROR) flag_x = x_in;
                else                flag_x = (shift_n == 0) ? x_in : shift_x_out;
            end
            default: begin
                y = 32'd0;
            end
        endcase
    end
endmodule
