`include "m68k_defs.vh"

// Combinational decoder for the full 68000 user-mode ISA (minus MOVEP,
// ABCD/SBCD/NBCD, ADDX/SUBX/NEGX, RESET, and trace exception).
//
// Decodes a single 16-bit opcode into a control bundle. The IF and ID stages
// of the core instantiate this module; ID uses the full output, IF only uses
// total_words / branch fields.
module m68k_decoder (
    input  wire [15:0] opcode,
    output reg  [5:0]  kind,
    output reg  [1:0]  size,
    output reg  [4:0]  alu_op,
    output reg  [2:0]  src_mode,
    output reg  [2:0]  src_reg,
    output reg  [2:0]  dst_mode,
    output reg  [2:0]  dst_reg,
    output reg  [2:0]  reg_idx,
    output reg         reg_is_a,
    output reg  [31:0] imm,
    output reg  [3:0]  cc,
    output reg  [1:0]  src_ext_words,
    output reg  [1:0]  dst_ext_words,
    output reg         needs_disp_word,
    output reg         privileged,
    output reg         direction,   // for ADD/SUB/AND/OR: 0=EA→Dn, 1=Dn→EA
    output reg  [3:0]  shift_count, // immediate shift count (0..7 means 1..8, see ISA)
    output reg         shift_dyn,   // 1 = count is in Dr, 0 = static
    output reg  [4:0]  bit_pos      // static bit number for bit ops
);
    // Instruction kinds (must match m68k_core.v).
    localparam K_BAD     = 6'd0;
    localparam K_NOP     = 6'd1;
    localparam K_RTS     = 6'd2;
    localparam K_STOP    = 6'd3;
    localparam K_JMP     = 6'd4;
    localparam K_JSR     = 6'd5;
    localparam K_LEA     = 6'd6;
    localparam K_MOVEQ   = 6'd7;
    localparam K_MOVE    = 6'd8;
    localparam K_ALUQ    = 6'd9;
    localparam K_ALU     = 6'd10;
    localparam K_TAS     = 6'd11;
    localparam K_BCC     = 6'd12;
    localparam K_ALUI    = 6'd13;
    localparam K_MOVEA   = 6'd14;
    localparam K_NEG     = 6'd15;
    localparam K_NOT     = 6'd16;
    localparam K_CLR     = 6'd17;
    localparam K_TST     = 6'd18;
    localparam K_EXT     = 6'd19;
    localparam K_SWAP    = 6'd20;
    localparam K_EXG     = 6'd21;
    localparam K_LINK    = 6'd22;
    localparam K_UNLK    = 6'd23;
    localparam K_PEA     = 6'd24;
    localparam K_MOVEM   = 6'd25;
    localparam K_SHIFT   = 6'd26;
    localparam K_BIT     = 6'd27;
    localparam K_DBCC    = 6'd28;
    localparam K_SCC     = 6'd29;
    localparam K_TRAP    = 6'd30;
    localparam K_TRAPV   = 6'd31;
    localparam K_RTE     = 6'd32;
    localparam K_RTR     = 6'd33;
    localparam K_CHK     = 6'd34;
    localparam K_MULU    = 6'd35;
    localparam K_MULS    = 6'd36;
    localparam K_DIVU    = 6'd37;
    localparam K_DIVS    = 6'd38;
    localparam K_MOVESR  = 6'd39;
    localparam K_MOVECCR = 6'd40;
    localparam K_MOVEUSP = 6'd41;
    localparam K_ILLEGAL = 6'd42;
    localparam K_ADDX    = 6'd43;
    localparam K_SUBX    = 6'd44;
    localparam K_NEGX    = 6'd45;

    // Compute extension words for an EA (mode + reg + size).
    function [1:0] ea_ext;
        input [2:0] m;
        input [2:0] r;
        input [1:0] sz;
        begin
            case (m)
                `EA_DISP: ea_ext = 2'd1;     // 16-bit displacement
                `EA_IDX:  ea_ext = 2'd1;     // brief extension word
                `EA_EXT: begin
                    case (r)
                        `EA7_ABSW:   ea_ext = 2'd1;
                        `EA7_ABSL:   ea_ext = 2'd2;
                        `EA7_PCDISP: ea_ext = 2'd1;
                        `EA7_PCIDX:  ea_ext = 2'd1;
                        `EA7_IMM:    ea_ext = (sz == `SZ_L) ? 2'd2 : 2'd1;
                        default:     ea_ext = 2'd0;
                    endcase
                end
                default: ea_ext = 2'd0;
            endcase
        end
    endfunction

    // Common opcode bit-slices.
    wire [2:0] r9  = opcode[11:9];
    wire [2:0] m6  = opcode[8:6];
    wire [2:0] m3  = opcode[5:3];
    wire [2:0] r0  = opcode[2:0];

    // Size from bits[7:6] (most ALU/single-operand instructions).
    function [1:0] size76;
        input [1:0] s;
        begin
            case (s)
                2'b00: size76 = `SZ_B;
                2'b01: size76 = `SZ_W;
                2'b10: size76 = `SZ_L;
                default: size76 = `SZ_W;   // invalid; treat as W
            endcase
        end
    endfunction

    // Size from MOVE bits[13:12] (oddly different).
    function [1:0] size_move;
        input [1:0] s;
        begin
            case (s)
                2'b01: size_move = `SZ_B;
                2'b11: size_move = `SZ_W;
                2'b10: size_move = `SZ_L;
                default: size_move = `SZ_W;
            endcase
        end
    endfunction

    // Immediates we may emit.
    reg [31:0] imm_movq, imm_addq, imm_disp;

    always @* begin
        // Defaults.
        kind            = K_BAD;
        size            = `SZ_W;
        alu_op          = `ALU_MOV;
        src_mode        = m3;
        src_reg         = r0;
        dst_mode        = m6;
        dst_reg         = r9;
        reg_idx         = r9;
        reg_is_a        = 1'b0;
        imm             = 32'd0;
        cc              = 4'd0;
        src_ext_words   = 2'd0;
        dst_ext_words   = 2'd0;
        needs_disp_word = 1'b0;
        privileged      = 1'b0;
        direction       = 1'b0;
        shift_count     = 4'd0;
        shift_dyn       = 1'b0;
        bit_pos         = 5'd0;

        imm_movq = {{24{opcode[7]}}, opcode[7:0]};
        imm_addq = (r9 == 3'd0) ? 32'd8 : {29'd0, r9};
        imm_disp = {{24{opcode[7]}}, opcode[7:0]};

        casez (opcode)
            // ----------------------------------------------------------
            // 0000_*: Immediate / bit ops (ORI, ANDI, SUBI, ADDI, EORI,
            //         CMPI, BTST/BCHG/BCLR/BSET static & dynamic).
            // ----------------------------------------------------------

            // ORI #imm, CCR / SR — special destinations
            16'b0000_0000_0011_1100: begin kind = K_ALUI; alu_op = `ALU_OR;  size = `SZ_B; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b000; src_ext_words = 2'd1; end  // ORI to CCR
            16'b0000_0000_0111_1100: begin kind = K_ALUI; alu_op = `ALU_OR;  size = `SZ_W; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b001; src_ext_words = 2'd1; privileged = 1; end  // ORI to SR
            16'b0000_0010_0011_1100: begin kind = K_ALUI; alu_op = `ALU_AND; size = `SZ_B; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b000; src_ext_words = 2'd1; end  // ANDI to CCR
            16'b0000_0010_0111_1100: begin kind = K_ALUI; alu_op = `ALU_AND; size = `SZ_W; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b001; src_ext_words = 2'd1; privileged = 1; end  // ANDI to SR
            16'b0000_1010_0011_1100: begin kind = K_ALUI; alu_op = `ALU_EOR; size = `SZ_B; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b000; src_ext_words = 2'd1; end  // EORI to CCR
            16'b0000_1010_0111_1100: begin kind = K_ALUI; alu_op = `ALU_EOR; size = `SZ_W; src_mode = `EA_EXT; src_reg = `EA7_IMM; dst_mode = 3'b110; dst_reg = 3'b001; src_ext_words = 2'd1; privileged = 1; end  // EORI to SR

            // ORI/ANDI/SUBI/ADDI/EORI/CMPI #imm, <ea>
            16'b0000_0000_??_??_????,  // ORI
            16'b0000_0010_??_??_????,  // ANDI
            16'b0000_0100_??_??_????,  // SUBI
            16'b0000_0110_??_??_????,  // ADDI
            16'b0000_1010_??_??_????,  // EORI
            16'b0000_1100_??_??_????:  // CMPI
            begin
                kind   = K_ALUI;
                size   = size76(opcode[7:6]);
                case (opcode[11:8])
                    4'b0000: alu_op = `ALU_OR;
                    4'b0010: alu_op = `ALU_AND;
                    4'b0100: alu_op = `ALU_SUB;
                    4'b0110: alu_op = `ALU_ADD;
                    4'b1010: alu_op = `ALU_EOR;
                    4'b1100: alu_op = `ALU_CMP;
                    default: alu_op = `ALU_MOV;
                endcase
                src_mode      = `EA_EXT;
                src_reg       = `EA7_IMM;
                src_ext_words = (size == `SZ_L) ? 2'd2 : 2'd1;
                dst_mode      = m3;
                dst_reg       = r0;
                dst_ext_words = ea_ext(m3, r0, size);
            end

            // Bit ops static: 0000_1000_xx_mm_rrr (+1 ext word)
            // xx: 00=BTST 01=BCHG 10=BCLR 11=BSET
            16'b0000_1000_??_??_????: begin
                kind = K_BIT;
                src_mode = `EA_EXT;
                src_reg  = `EA7_IMM;
                src_ext_words = 2'd1;
                dst_mode = m3;
                dst_reg  = r0;
                dst_ext_words = ea_ext(m3, r0, (m3 == `EA_DREG) ? `SZ_L : `SZ_B);
                size = (m3 == `EA_DREG) ? `SZ_L : `SZ_B;
                alu_op = {3'd0, opcode[7:6]};  // 00..11 selects sub-op
                // For Dn dst the dest value is read via rb (D[r0]); for
                // mem dst rb feeds the EA computation, so route rb to A[r0].
                reg_idx = r0;
                reg_is_a = (m3 != `EA_DREG);
            end

            // Bit ops dynamic: 0000_ddd_1xx_mm_rrr — uses Dd as bit number.
            16'b0000_???_1??_???_???: begin
                kind = K_BIT;
                shift_dyn = 1'b1;
                reg_idx = r9;
                reg_is_a = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                dst_mode = m3;
                dst_reg  = r0;
                src_ext_words = ea_ext(m3, r0, (m3 == `EA_DREG) ? `SZ_L : `SZ_B);
                size = (m3 == `EA_DREG) ? `SZ_L : `SZ_B;
                alu_op = {3'd0, opcode[7:6]};
            end

            // ----------------------------------------------------------
            // 0001/0010/0011: MOVE.B / MOVE.L / MOVE.W
            //                 MOVEA when dst_mode == 001 (An).
            // ----------------------------------------------------------
            16'b00??_???_???_???_???: begin
                if (opcode[13:12] == 2'b01 || opcode[13:12] == 2'b10 || opcode[13:12] == 2'b11) begin
                    size = size_move(opcode[13:12]);
                    src_mode = m3;
                    src_reg  = r0;
                    dst_mode = m6;
                    dst_reg  = r9;
                    src_ext_words = ea_ext(m3, r0, size);
                    dst_ext_words = ea_ext(m6, r9, size);
                    if (m6 == `EA_AREG) begin
                        // MOVEA — destination is An. Only W or L valid.
                        kind = K_MOVEA;
                        alu_op = `ALU_PASSB;
                    end else begin
                        kind = K_MOVE;
                        alu_op = `ALU_PASSB;
                    end
                end
            end

            // ----------------------------------------------------------
            // 0100_*: Misc (LEA, JMP, JSR, NEG, NOT, CLR, TST, TAS, EXT,
            //         SWAP, PEA, LINK, UNLK, MOVE USP, TRAP, RTE, RTR,
            //         NOP, STOP, MOVE-to/from-SR/CCR, MOVEM, CHK).
            // ----------------------------------------------------------

            // MOVE from SR / MOVE to CCR / MOVE to SR must be matched BEFORE
            // CLR/NEG/NOT/TST because those broad patterns include bits 7:6 == 11
            // (which encodes the SR/CCR variants).
            //
            // MOVE from SR: 0100_0000_11_mm_rrr (W)
            16'b0100_0000_11_??_????: begin
                kind = K_MOVESR;
                size = `SZ_W;
                direction = 1'b0;   // 0 = read SR to dst
                dst_mode = m3;
                dst_reg  = r0;
                dst_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            // MOVE to CCR: 0100_0100_11_mm_rrr (W src; only low byte used)
            16'b0100_0100_11_??_????: begin
                kind = K_MOVECCR;
                size = `SZ_W;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            // MOVE to SR: 0100_0110_11_mm_rrr (W, privileged)
            16'b0100_0110_11_??_????: begin
                kind = K_MOVESR;
                size = `SZ_W;
                direction = 1'b1;   // 1 = write SR from src
                privileged = 1'b1;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end

            // NEGX/CLR/NEG/NOT (xx_ss_mm_rrr)
            16'b0100_0000_??_??_????: begin  // NEGX
                kind   = K_NEGX;
                size   = size76(opcode[7:6]);
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
                alu_op = `ALU_NEGX;
            end
            16'b0100_0010_??_??_????: begin  // CLR
                kind = K_CLR;
                size = size76(opcode[7:6]);
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
                alu_op = `ALU_CLR;
            end
            16'b0100_0100_??_??_????: begin  // NEG
                kind = K_NEG;
                size = size76(opcode[7:6]);
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
                alu_op = `ALU_NEG;
            end
            16'b0100_0110_??_??_????: begin  // NOT
                kind = K_NOT;
                size = size76(opcode[7:6]);
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
                alu_op = `ALU_NOT;
            end

            // EXT.W Dn: 0100_1000_10_000_rrr
            16'b0100_1000_10_000_???: begin
                kind = K_EXT;
                size = `SZ_W;
                src_reg = r0;
                alu_op = `ALU_EXT_W;
            end
            // EXT.L Dn: 0100_1000_11_000_rrr
            16'b0100_1000_11_000_???: begin
                kind = K_EXT;
                size = `SZ_L;
                src_reg = r0;
                alu_op = `ALU_EXT_L;
            end

            // SWAP Dn: 0100_1000_01_000_rrr
            16'b0100_1000_0100_0???: begin
                kind = K_SWAP;
                size = `SZ_L;
                src_reg = r0;
                alu_op = `ALU_SWAP;
            end
            // PEA: 0100_1000_01_mm_rrr (excluding mode=000=SWAP)
            16'b0100_1000_01_??_????: begin
                kind = K_PEA;
                size = `SZ_L;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
            end

            // TAS / TST  (0100_1010_xx_mm_rrr)
            16'b0100_1010_11_??_????: begin
                kind = K_TAS;
                src_mode = m3;
                src_reg  = r0;
                size = `SZ_B;
                alu_op = `ALU_TAS;
                src_ext_words = ea_ext(m3, r0, `SZ_B);
            end
            16'b0100_1010_??_??_????: begin
                kind = K_TST;
                size = size76(opcode[7:6]);
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
                alu_op = `ALU_TST;
            end

            // MOVEM regs ↔ memory: 0100_1?00_1?_mm_rrr
            // bit 10 = direction (0 = regs to mem, 1 = mem to regs)
            // bit 6  = size (0 = W, 1 = L)
            16'b0100_1000_1?_??_????,
            16'b0100_1100_1?_??_????: begin
                kind = K_MOVEM;
                size = opcode[6] ? `SZ_L : `SZ_W;
                direction = opcode[10];   // 0 = regs→mem, 1 = mem→regs
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = 2'd1;  // register mask
                if (m3 == `EA_DISP || m3 == `EA_IDX) src_ext_words = 2'd2;
                else if (m3 == `EA_EXT) begin
                    case (r0)
                        `EA7_ABSW: src_ext_words = 2'd2;
                        `EA7_ABSL: src_ext_words = 2'd3;
                        default:   src_ext_words = 2'd2;
                    endcase
                end
            end

            // LINK An, #disp16: 0100_1110_0101_0aaa (+disp16)
            16'b0100_1110_0101_0???: begin
                kind = K_LINK;
                src_reg = r0;
                src_ext_words = 2'd1;
            end
            // UNLK An: 0100_1110_0101_1aaa
            16'b0100_1110_0101_1???: begin
                kind = K_UNLK;
                src_reg = r0;
            end
            // MOVE An, USP: 0100_1110_0110_0aaa
            16'b0100_1110_0110_0???: begin
                kind = K_MOVEUSP;
                direction = 1'b0;
                src_mode = `EA_AREG;
                src_reg = r0;
                privileged = 1'b1;
            end
            // MOVE USP, An: 0100_1110_0110_1aaa
            16'b0100_1110_0110_1???: begin
                kind = K_MOVEUSP;
                direction = 1'b1;
                src_mode = `EA_AREG;
                src_reg = r0;
                privileged = 1'b1;
            end

            // TRAP #v: 0100_1110_0100_vvvv
            16'b0100_1110_0100_????: begin
                kind = K_TRAP;
                imm = {28'd0, opcode[3:0]};
            end

            // Reserved-but-implemented control opcodes (0100_1110_0111_xxxx).
            16'b0100_1110_0111_0001: kind = K_NOP;
            16'b0100_1110_0111_0011: begin kind = K_RTE; privileged = 1'b1; end
            16'b0100_1110_0111_0101: kind = K_RTS;
            16'b0100_1110_0111_0110: kind = K_TRAPV;
            16'b0100_1110_0111_0111: kind = K_RTR;
            16'b0100_1110_0111_0010: begin
                kind = K_STOP;
                src_mode = `EA_EXT;
                src_reg  = `EA7_IMM;
                src_ext_words = 2'd1;
                privileged = 1'b1;
            end

            // JMP / JSR (0100_1110_1x_mm_rrr)
            16'b0100_1110_1?_??_????: begin
                kind = opcode[6] ? K_JMP : K_JSR;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
            end

            // CHK.W: 0100_ddd_110_mm_rrr
            16'b0100_???_110_???_???: begin
                kind = K_CHK;
                size = `SZ_W;
                reg_idx = r9;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            // LEA: 0100_aaa_111_mm_rrr
            16'b0100_???_111_???_???: begin
                kind = K_LEA;
                size = `SZ_L;
                reg_idx = r9;
                reg_is_a = 1'b1;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
                alu_op = `ALU_PASSB;
            end

            // ILLEGAL: 0100_1010_1111_1100
            16'b0100_1010_1111_1100: kind = K_ILLEGAL;

            // ----------------------------------------------------------
            // 0101_*: ADDQ / SUBQ / Scc / DBcc
            // ----------------------------------------------------------
            // DBcc Dn,disp16: 0101_cccc_1100_1rrr
            16'b0101_????_1100_1???: begin
                kind = K_DBCC;
                cc = opcode[11:8];
                src_reg = r0;
                src_ext_words = 2'd1;
                needs_disp_word = 1'b1;  // pull disp16 into id_branch_imm
            end
            // Scc <ea>: 0101_cccc_11_mm_rrr (mode != 001)
            16'b0101_????_11_??_????: begin
                kind = K_SCC;
                cc = opcode[11:8];
                size = `SZ_B;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_B);
            end
            // ADDQ/SUBQ #i, <ea>
            16'b0101_???_???_???_???: begin
                kind = K_ALUQ;
                size = size76(opcode[7:6]);
                alu_op = opcode[8] ? `ALU_SUB : `ALU_ADD;
                imm = imm_addq;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 0110_*: Bcc / BRA / BSR
            // ----------------------------------------------------------
            16'b0110_????_????_????: begin
                kind = K_BCC;
                cc = opcode[11:8];
                imm = imm_disp;
                if (opcode[7:0] == 8'd0) begin
                    needs_disp_word = 1'b1;
                    src_ext_words = 2'd1;
                end
            end

            // ----------------------------------------------------------
            // 0111_*: MOVEQ
            // ----------------------------------------------------------
            16'b0111_???_0_???_?????: begin
                kind = K_MOVEQ;
                size = `SZ_L;
                reg_idx = r9;
                imm = imm_movq;
                alu_op = `ALU_PASSB;
            end

            // ----------------------------------------------------------
            // 1000_*: OR / DIVU.W / DIVS.W
            // ----------------------------------------------------------
            16'b1000_???_011_???_???: begin   // DIVU.W
                kind = K_DIVU;
                size = `SZ_W;
                reg_idx = r9;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1000_???_111_???_???: begin   // DIVS.W
                kind = K_DIVS;
                size = `SZ_W;
                reg_idx = r9;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1000_???_???_???_???: begin   // OR
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_OR;
                reg_idx = r9;
                direction = opcode[8];
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 1001_*: SUB / SUBA
            // ----------------------------------------------------------
            16'b1001_???_011_???_???: begin   // SUBA.W
                kind = K_ALU;
                size = `SZ_W;
                alu_op = `ALU_SUB;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1001_???_111_???_???: begin   // SUBA.L
                kind = K_ALU;
                size = `SZ_L;
                alu_op = `ALU_SUB;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
            end
            // SUBX Dy,Dx / SUBX -(Ay),-(Ax) — direction=1 mm=000/001.
            16'b1001_???_100_000_???,
            16'b1001_???_101_000_???,
            16'b1001_???_110_000_???: begin   // SUBX Dy,Dx
                kind   = K_SUBX;
                size   = size76(opcode[7:6]);
                alu_op = `ALU_SUBX;
                reg_idx = r9;
                src_mode = `EA_DREG;
                src_reg  = r0;
                direction = 1'b0;
            end
            16'b1001_???_100_001_???,
            16'b1001_???_101_001_???,
            16'b1001_???_110_001_???: begin   // SUBX -(Ay),-(Ax)
                kind   = K_SUBX;
                size   = size76(opcode[7:6]);
                alu_op = `ALU_SUBX;
                reg_idx = r9;
                reg_is_a = 1'b1;
                src_mode = `EA_ADEC;
                src_reg  = r0;
                direction = 1'b1;
            end
            16'b1001_???_???_???_???: begin
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_SUB;
                reg_idx = r9;
                direction = opcode[8];
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 1011_*: CMP / CMPA / EOR
            // ----------------------------------------------------------
            16'b1011_???_011_???_???: begin   // CMPA.W
                kind = K_ALU;
                size = `SZ_W;
                alu_op = `ALU_CMP;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1011_???_111_???_???: begin   // CMPA.L
                kind = K_ALU;
                size = `SZ_L;
                alu_op = `ALU_CMP;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
            end
            16'b1011_???_1??_???_???: begin   // EOR
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_EOR;
                reg_idx = r9;
                direction = 1'b1;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end
            16'b1011_???_0??_???_???: begin   // CMP
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_CMP;
                reg_idx = r9;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 1100_*: AND / MULU.W / MULS.W / EXG
            // ----------------------------------------------------------
            16'b1100_???_011_???_???: begin   // MULU.W
                kind = K_MULU;
                size = `SZ_W;
                reg_idx = r9;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1100_???_111_???_???: begin   // MULS.W
                kind = K_MULS;
                size = `SZ_W;
                reg_idx = r9;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            // EXG Dx,Dy: 1100_xxx_1_01000_yyy   (opmode field bits 8:3)
            // EXG Ax,Ay: 1100_xxx_1_01001_yyy
            // EXG Dx,Ay: 1100_xxx_1_10001_yyy
            16'b1100_???_101_000_???,
            16'b1100_???_101_001_???,
            16'b1100_???_110_001_???: begin
                kind = K_EXG;
                size = `SZ_L;
                reg_idx = r9;  // Rx
                src_reg  = r0; // Ry
                case (opcode[8:3])
                    6'b101_000: begin reg_is_a = 1'b0; src_mode = `EA_DREG; end  // both D
                    6'b101_001: begin reg_is_a = 1'b1; src_mode = `EA_AREG; end  // both A
                    6'b110_001: begin reg_is_a = 1'b0; src_mode = `EA_AREG; end  // Dx, Ay
                    default:    begin reg_is_a = 1'b0; src_mode = `EA_DREG; end
                endcase
            end
            16'b1100_???_???_???_???: begin   // AND
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_AND;
                reg_idx = r9;
                direction = opcode[8];
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 1101_*: ADD / ADDA
            // ----------------------------------------------------------
            16'b1101_???_011_???_???: begin   // ADDA.W
                kind = K_ALU;
                size = `SZ_W;
                alu_op = `ALU_ADD;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
            end
            16'b1101_???_111_???_???: begin   // ADDA.L
                kind = K_ALU;
                size = `SZ_L;
                alu_op = `ALU_ADD;
                reg_idx = r9;
                reg_is_a = 1'b1;
                direction = 1'b0;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_L);
            end
            // ADDX Dy, Dx  : 1101_xxx_1ss_000_yyy
            // ADDX -(Ay), -(Ax): 1101_xxx_1ss_001_yyy
            // (Direction=1 with mm=000/001 is ADDX, not ADD-to-mem.)
            16'b1101_???_100_000_???,
            16'b1101_???_101_000_???,
            16'b1101_???_110_000_???: begin   // ADDX Dy,Dx
                kind   = K_ADDX;
                size   = size76(opcode[7:6]);
                alu_op = `ALU_ADDX;
                reg_idx = r9;
                src_mode = `EA_DREG;
                src_reg  = r0;
                direction = 1'b0;
            end
            16'b1101_???_100_001_???,
            16'b1101_???_101_001_???,
            16'b1101_???_110_001_???: begin   // ADDX -(Ay),-(Ax)
                kind   = K_ADDX;
                size   = size76(opcode[7:6]);
                alu_op = `ALU_ADDX;
                reg_idx = r9;
                reg_is_a = 1'b1;
                src_mode = `EA_ADEC;
                src_reg  = r0;
                direction = 1'b1;
            end
            16'b1101_???_???_???_???: begin
                kind = K_ALU;
                size = size76(opcode[7:6]);
                alu_op = `ALU_ADD;
                reg_idx = r9;
                direction = opcode[8];
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, size);
            end

            // ----------------------------------------------------------
            // 1110_*: Shifts / rotates
            // ----------------------------------------------------------
            // Memory single-bit shift: 1110_0??_011_mm_rrr (W only)
            16'b1110_0??_011_???_???: begin
                kind = K_SHIFT;
                size = `SZ_W;
                src_mode = m3;
                src_reg  = r0;
                src_ext_words = ea_ext(m3, r0, `SZ_W);
                direction = opcode[8];
                shift_count = 4'd1;
                case (opcode[10:9])
                    2'b00: alu_op = opcode[8] ? `ALU_ASL : `ALU_ASR;
                    2'b01: alu_op = opcode[8] ? `ALU_LSL : `ALU_LSR;
                    2'b10: alu_op = opcode[8] ? `ALU_ROXL : `ALU_ROXR;
                    2'b11: alu_op = opcode[8] ? `ALU_ROL : `ALU_ROR;
                endcase
            end
            // Register shift: 1110_ccc_d_ss_i_tt_rrr
            16'b1110_???_?_??_?_??_???: begin
                kind = K_SHIFT;
                size = size76(opcode[7:6]);
                src_mode = `EA_DREG;
                src_reg = r0;
                direction = opcode[8];
                shift_dyn = opcode[5];     // 1=count in Dr (r9), 0=count immediate
                shift_count = {1'b0, r9};  // immediate count (0 means 8)
                reg_idx = r9;               // dynamic count register (if shift_dyn=1)
                case (opcode[4:3])
                    2'b00: alu_op = opcode[8] ? `ALU_ASL : `ALU_ASR;
                    2'b01: alu_op = opcode[8] ? `ALU_LSL : `ALU_LSR;
                    2'b10: alu_op = opcode[8] ? `ALU_ROXL : `ALU_ROXR;
                    2'b11: alu_op = opcode[8] ? `ALU_ROL : `ALU_ROR;
                endcase
            end

            default: kind = K_BAD;
        endcase
    end
endmodule
