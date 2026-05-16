// 68K-mini common definitions.
`ifndef M68K_DEFS_VH
`define M68K_DEFS_VH

// ALU op encodings (internal microcode).
`define ALU_MOV   4'd0
`define ALU_ADD   4'd1
`define ALU_SUB   4'd2
`define ALU_AND   4'd3
`define ALU_OR    4'd4
`define ALU_EOR   4'd5
`define ALU_CMP   4'd6   // sub but no writeback
`define ALU_TAS   4'd7   // returns operand|0x80, sets flags from operand
`define ALU_PASSB 4'd8   // pass operand B (used for MOVE/MOVEQ/LEA)

// Effective-address modes.
`define EA_DREG    3'd0  // Dn
`define EA_AREG    3'd1  // An
`define EA_AIND    3'd2  // (An)
`define EA_AINC    3'd3  // (An)+
`define EA_ADEC    3'd4  // -(An)
`define EA_EXT     3'd7  // 111: extension-word modes

// Mode 7 reg sub-encodings.
`define EA7_ABSW   3'd0
`define EA7_ABSL   3'd1
`define EA7_IMM    3'd4

// Branch condition codes (Bcc cc field).
`define CC_T   4'h0
`define CC_F   4'h1
`define CC_HI  4'h2
`define CC_LS  4'h3
`define CC_CC  4'h4
`define CC_CS  4'h5
`define CC_NE  4'h6
`define CC_EQ  4'h7
`define CC_VC  4'h8
`define CC_VS  4'h9
`define CC_PL  4'hA
`define CC_MI  4'hB
`define CC_GE  4'hC
`define CC_LT  4'hD
`define CC_GT  4'hE
`define CC_LE  4'hF

// Bus op codes.
`define BUS_READ   2'd0
`define BUS_WRITE  2'd1
`define BUS_TAS    2'd2  // locked read-modify-write of low byte

// Reset/boot constants.
`define RESET_PC   32'h0000_0400

`endif
