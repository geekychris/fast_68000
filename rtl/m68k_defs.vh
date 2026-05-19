// 68K common definitions.
`ifndef M68K_DEFS_VH
`define M68K_DEFS_VH

// ----------------------------------------------------------------------------
//  Operand sizes
// ----------------------------------------------------------------------------
`define SZ_B   2'b00
`define SZ_W   2'b01
`define SZ_L   2'b10

// ----------------------------------------------------------------------------
//  ALU op encodings (internal microcode).
// ----------------------------------------------------------------------------
`define ALU_MOV   5'd0
`define ALU_ADD   5'd1
`define ALU_SUB   5'd2
`define ALU_AND   5'd3
`define ALU_OR    5'd4
`define ALU_EOR   5'd5
`define ALU_CMP   5'd6   // sub but no writeback
`define ALU_TAS   5'd7   // pass B|0x80 with flags from byte
`define ALU_PASSB 5'd8   // pass operand B (MOVE / MOVEQ / LEA)
`define ALU_NEG   5'd9
`define ALU_NOT   5'd10
`define ALU_CLR   5'd11
`define ALU_TST   5'd12  // pass A, set N/Z, V/C=0
`define ALU_EXT_W 5'd13  // sign-extend byte→word
`define ALU_EXT_L 5'd14  // sign-extend word→long
`define ALU_SWAP  5'd15  // swap halves of a long
// Shifts/rotates (modifier passed in as count, direction = bit in ctrl)
`define ALU_ASL   5'd16
`define ALU_ASR   5'd17
`define ALU_LSL   5'd18
`define ALU_LSR   5'd19
`define ALU_ROL   5'd20
`define ALU_ROR   5'd21
`define ALU_ROXL  5'd22
`define ALU_ROXR  5'd23
`define ALU_ADDX  5'd24
`define ALU_SUBX  5'd25
`define ALU_NEGX  5'd26

// ----------------------------------------------------------------------------
//  Effective-address modes
// ----------------------------------------------------------------------------
`define EA_DREG    3'd0  // Dn
`define EA_AREG    3'd1  // An
`define EA_AIND    3'd2  // (An)
`define EA_AINC    3'd3  // (An)+
`define EA_ADEC    3'd4  // -(An)
`define EA_DISP    3'd5  // d16(An)
`define EA_IDX     3'd6  // d8(An,Xn)
`define EA_EXT     3'd7  // 111: extension-word modes

// Mode-7 sub-encodings (reg field):
`define EA7_ABSW   3'd0
`define EA7_ABSL   3'd1
`define EA7_PCDISP 3'd2  // d16(PC)
`define EA7_PCIDX  3'd3  // d8(PC,Xn)
`define EA7_IMM    3'd4

// ----------------------------------------------------------------------------
//  Branch condition codes (Bcc cc field)
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
//  Bus op codes
// ----------------------------------------------------------------------------
`define BUS_READ   2'd0
`define BUS_WRITE  2'd1
`define BUS_TAS    2'd2

// ----------------------------------------------------------------------------
//  SR / CCR bit positions
// ----------------------------------------------------------------------------
//   SR[15] T  - trace
//   SR[13] S  - supervisor
//   SR[10:8] I - interrupt mask
//   SR[4] X   - extend
//   SR[3] N   - negative
//   SR[2] Z   - zero
//   SR[1] V   - overflow
//   SR[0] C   - carry
`define SR_T   15
`define SR_S   13
`define SR_X   4
`define SR_N   3
`define SR_Z   2
`define SR_V   1
`define SR_C   0

// ----------------------------------------------------------------------------
//  Exception vector numbers (offset = vector * 4)
// ----------------------------------------------------------------------------
`define VEC_RESET_SP    0
`define VEC_RESET_PC    1
`define VEC_BUS_ERROR   2
`define VEC_ADDR_ERROR  3
`define VEC_ILLEGAL     4
`define VEC_DIV_ZERO    5
`define VEC_CHK         6
`define VEC_TRAPV       7
`define VEC_PRIV_VIO    8
`define VEC_TRACE       9
`define VEC_LINEA      10
`define VEC_LINEF      11
`define VEC_SPURIOUS   24
`define VEC_AUTO_BASE  25  // 25..31 = autovector IRQ 1..7
`define VEC_TRAP_BASE  32  // 32..47 = TRAP #0..#15

// ----------------------------------------------------------------------------
//  Reset and boot
// ----------------------------------------------------------------------------
`define RESET_PC   32'h0000_0400

`endif
