`include "m68k_defs.vh"

// Purely combinational 68K-mini decoder.
//
// Takes a 16-bit opcode and produces:
//   - The kind of instruction
//   - Source/destination EA fields
//   - The "register operand" index (for ADD/SUB/etc — the Dn side)
//   - ALU op, immediate, branch condition
//   - Number of extension words needed (for the IF state machine)
//
// Unknown opcodes set kind = K_BAD.
module m68k_decoder (
    input  wire [15:0] opcode,
    output reg  [3:0]  kind,
    output reg  [3:0]  alu_op,
    output reg  [2:0]  src_mode,
    output reg  [2:0]  src_reg,
    output reg  [2:0]  dst_mode,
    output reg  [2:0]  dst_reg,
    output reg  [2:0]  reg_idx,     // Dn/An index for the register operand
    output reg         reg_is_a,    // is reg_idx an An?
    output reg  [31:0] imm,         // for MOVEQ/ADDQ/SUBQ/Bcc disp8
    output reg  [3:0]  cc,
    output reg  [1:0]  src_ext_words,
    output reg  [1:0]  dst_ext_words,
    output reg         needs_disp_word // Bcc with disp8 == 0
);

    // Instruction kinds.
    localparam K_BAD   = 4'd0;
    localparam K_NOP   = 4'd1;
    localparam K_RTS   = 4'd2;
    localparam K_STOP  = 4'd3;
    localparam K_JMP   = 4'd4;
    localparam K_JSR   = 4'd5;
    localparam K_LEA   = 4'd6;
    localparam K_MOVEQ = 4'd7;
    localparam K_MOVE  = 4'd8;
    localparam K_ALUQ  = 4'd9;  // ADDQ/SUBQ
    localparam K_ALU   = 4'd10; // ADD/SUB/AND/OR/EOR/CMP
    localparam K_TAS   = 4'd11;
    localparam K_BCC   = 4'd12;

    // Local helpers.
    function [1:0] ea_ext;
        input [2:0] m;
        input [2:0] r;
        begin
            case (m)
                `EA_EXT: begin
                    case (r)
                        `EA7_ABSW: ea_ext = 2'd1;
                        `EA7_ABSL: ea_ext = 2'd2;
                        `EA7_IMM:  ea_ext = 2'd2; // long immediate
                        default:   ea_ext = 2'd0;
                    endcase
                end
                default: ea_ext = 2'd0;
            endcase
        end
    endfunction

    wire [2:0] hi  = opcode[15:13];
    wire [2:0] r9  = opcode[11:9];
    wire [2:0] m6  = opcode[8:6];
    wire [2:0] m3  = opcode[5:3];
    wire [2:0] r0  = opcode[2:0];
    wire [5:0] src_ea = {m3, r0};
    wire [5:0] dst_ea = {m6, r9};

    reg [31:0] imm_movq;
    reg [31:0] imm_addq;
    reg [31:0] imm_disp;

    always @* begin
        // Defaults.
        kind = K_BAD;
        alu_op = `ALU_MOV;
        src_mode = m3;
        src_reg  = r0;
        dst_mode = m6;
        dst_reg  = r9;
        reg_idx  = r9;
        reg_is_a = 1'b0;
        imm      = 32'd0;
        cc       = 4'd0;
        src_ext_words = 2'd0;
        dst_ext_words = 2'd0;
        needs_disp_word = 1'b0;

        // Sign-extended immediates.
        imm_movq = {{24{opcode[7]}}, opcode[7:0]};
        imm_addq = (opcode[11:9] == 3'd0) ? 32'd8 : {29'd0, opcode[11:9]};
        imm_disp = {{24{opcode[7]}}, opcode[7:0]};

        // Privileged-form opcodes (4E series).
        if (opcode == 16'h4E71)      kind = K_NOP;
        else if (opcode == 16'h4E75) kind = K_RTS;
        else if (opcode == 16'h4E72) begin
            kind = K_STOP;
            src_ext_words = 2'd1;
        end
        // JMP / JSR : 0100_1110_1x_mm_rrr
        else if (opcode[15:7] == 9'b0100_1110_1) begin
            kind = opcode[6] ? K_JMP : K_JSR;
            src_mode = m3;
            src_reg  = r0;
            src_ext_words = ea_ext(m3, r0);
        end
        // LEA: 0100_aaa_111_mm_rrr  -> dst An = src EA
        else if (opcode[15:12] == 4'b0100 && m6 == 3'b111) begin
            kind = K_LEA;
            reg_idx = r9;
            reg_is_a = 1'b1;
            alu_op = `ALU_PASSB;
            src_ext_words = ea_ext(m3, r0);
        end
        // TAS: 0100_1010_11_mm_rrr
        else if (opcode[15:6] == 10'b0100_1010_11) begin
            kind = K_TAS;
            src_mode = m3;
            src_reg  = r0;
            alu_op = `ALU_TAS;
            src_ext_words = ea_ext(m3, r0);
        end
        // MOVEQ: 0111_rrr_0_iiiiiiii
        else if (opcode[15:12] == 4'b0111 && opcode[8] == 1'b0) begin
            kind = K_MOVEQ;
            reg_idx = r9;
            imm = imm_movq;
            alu_op = `ALU_PASSB;
        end
        // MOVE.L: 00_10_DDDmmm_MMMrrr  (size field is bits 13:12; 10 = long)
        else if (opcode[15:12] == 4'b0010) begin
            kind = K_MOVE;
            alu_op = `ALU_PASSB;
            src_mode = m3;
            src_reg  = r0;
            dst_mode = m6;
            dst_reg  = r9;
            src_ext_words = ea_ext(m3, r0);
            dst_ext_words = ea_ext(m6, r9);
        end
        // ADDQ / SUBQ: 0101_iii_x_10_mm_rrr  (size 10 = long; bit 8 = 0 ADDQ, 1 SUBQ)
        else if (opcode[15:12] == 4'b0101 && opcode[7:6] == 2'b10) begin
            kind = K_ALUQ;
            alu_op = opcode[8] ? `ALU_SUB : `ALU_ADD;
            imm = imm_addq;
            src_mode = m3;
            src_reg  = r0;
            src_ext_words = ea_ext(m3, r0);
        end
        // Bcc: 0110_cccc_dddddddd
        else if (opcode[15:12] == 4'b0110) begin
            kind = K_BCC;
            cc = opcode[11:8];
            imm = imm_disp;
            if (opcode[7:0] == 8'd0) begin
                needs_disp_word = 1'b1;
                src_ext_words = 2'd1;
            end
        end
        // ADD/SUB/AND/OR/EOR/CMP : <opfamily>_rrr_<opmode>_mm_sss
        // opmode 010 = long EA->Dn  ; opmode 110 = long Dn->EA (only EOR / store-add)
        else if (opcode[7:6] == 2'b10 &&
                 (opcode[15:12] == 4'b1101 ||  // ADD
                  opcode[15:12] == 4'b1001 ||  // SUB
                  opcode[15:12] == 4'b1100 ||  // AND
                  opcode[15:12] == 4'b1000 ||  // OR
                  opcode[15:12] == 4'b1011)) begin
            kind = K_ALU;
            reg_idx = r9;
            src_mode = m3;
            src_reg  = r0;
            src_ext_words = ea_ext(m3, r0);
            case (opcode[15:12])
                4'b1101: alu_op = `ALU_ADD;
                4'b1001: alu_op = `ALU_SUB;
                4'b1100: alu_op = `ALU_AND;
                4'b1000: alu_op = `ALU_OR;
                4'b1011: alu_op = opcode[8] ? `ALU_EOR : `ALU_CMP;
                default: alu_op = `ALU_MOV;
            endcase
        end
    end

    // Expose kind constants via parameters (no SV).
    // Users can match on the numeric values directly.
endmodule
