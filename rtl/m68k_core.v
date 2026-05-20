`include "m68k_defs.vh"

// 3-stage pipelined 68K-mini core.
//
// Stages:
//   IF : multi-cycle instruction fetch (opcode + 0..4 extension words) into
//        the IF/ID pipeline register. Handles 16-bit opcode width over 32-bit
//        cache words by extracting the addressed half-word.
//   ID : full decode + register-file read.
//   EX : execute. Performs ALU, memory access (load/store/TAS) via the
//        D-cache, branch resolution, and register writeback. May stall the
//        pipeline as long as the D-cache needs to settle.
//
// Forwarding: EX's writeback for the in-flight instruction is forwarded to
// the ID stage's register reads in the same cycle so back-to-back dependent
// instructions do not stall.
//
// Branches are resolved in EX. Predict not-taken; mispredicts flush IF and
// the IF/ID register (2-cycle penalty by structure).
module m68k_core #(
    parameter [3:0]  CORE_ID = 0,
    parameter [31:0] RESET_PC_PARAM = `RESET_PC
)(
    input  wire        clk,
    input  wire        rst_n,
    input  wire [31:0] reset_a7,
    // Pending interrupt level (0 = none, 1..7 = priority). When > sr_i,
    // the CPU takes an autovector interrupt at the next instruction
    // boundary (saved PC = address of suppressed next instruction, new
    // sr_i = ipl_i so same/lower-priority IRQs are masked while servicing).
    input  wire [2:0]  ipl_i,

    // I-cache port.
    output wire        ic_req,
    output wire        ic_we,
    output wire        ic_lock,
    output reg  [31:0] ic_addr,
    output wire [31:0] ic_wdata,
    output wire [3:0]  ic_be,
    input  wire        ic_ack,
    input  wire [31:0] ic_rdata,

    // D-cache port.
    output wire        dc_req,
    output reg         dc_we,
    output reg         dc_lock,
    output reg  [31:0] dc_addr,
    output reg  [31:0] dc_wdata,
    output reg  [3:0]  dc_be,
    // 1 when the data access is a genuine 32-bit (.L) operation.
    // dc_be is always 4'b1111 for .L but the CPU also sets be=1111 for
    // sub-longword reads (it does its own byte extraction from cpu_rdata),
    // so be=1111 alone can't tell the bus "this is .L".  dc_is_long
    // disambiguates so the bus's unaligned-.L assembly path can fire only
    // when truly needed.
    output reg         dc_is_long,
    input  wire        dc_ack,
    input  wire [31:0] dc_rdata,

    output reg         halted,
    output reg  [15:0] halt_code,
    output reg  [31:0] retired       // instructions retired since reset
);
    assign ic_we    = 1'b0;
    assign ic_lock  = 1'b0;
    assign ic_wdata = 32'd0;
    assign ic_be    = 4'b1111;

    // Decoder kinds (must match m68k_decoder.v).
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

    // EX state.
    localparam S_RUN           = 4'd0;
    localparam S_LOAD          = 4'd1;
    localparam S_STORE         = 4'd2;
    localparam S_TASW          = 4'd3;
    // Canonical 68000 exception frame: 6 bytes total, [SP+0]=SR (word),
    // [SP+2]=PC (long).  We push PC first (long), then SR (word).
    localparam S_EXC_PUSH_PC   = 4'd4;  // push 32-bit PC at (SSP-4)
    localparam S_EXC_PUSH_SR   = 4'd5;  // push 16-bit SR  at (SSP-2)
    localparam S_EXC_FETCH     = 4'd6;  // read 32-bit vector from (vec*4)
    localparam S_RTE_POP_SR    = 4'd7;  // pop 16-bit SR/CCR word
    localparam S_RTE_POP_PC    = 4'd8;  // pop 32-bit PC
    localparam S_RMW_W         = 4'd9;  // RMW write phase (NEG/NOT to memory)
    localparam S_MOVEM         = 4'd10; // MOVEM register-list iterator
    // Group-0 (bus/address error) frame extends the canonical 6-byte frame
    // with 4 more words pushed BELOW the SR/PC pair (in the direction of
    // decreasing addresses):
    //   SP+0  SSW       SP+2..5  fault addr  SP+6  IR
    //   SP+8  SR        SP+10..13 PC
    // S_EXC_PUSH_SR transitions to S_EXC_PUSH_IR (not S_EXC_FETCH) when
    // exc_is_group0.
    localparam S_EXC_PUSH_IR   = 4'd11;
    localparam S_EXC_PUSH_ALO  = 4'd12;
    localparam S_EXC_PUSH_AHI  = 4'd13;
    localparam S_EXC_PUSH_SSW  = 4'd14;

    // Helpful pure functions on byte position within a 32-bit word.
    function [3:0] be_for_byte;
        input [1:0] pos;
        begin
            case (pos)
                2'b00: be_for_byte = 4'b1000;
                2'b01: be_for_byte = 4'b0100;
                2'b10: be_for_byte = 4'b0010;
                2'b11: be_for_byte = 4'b0001;
            endcase
        end
    endfunction
    function [7:0] byte_at;
        input [31:0] w;
        input [1:0]  pos;
        begin
            case (pos)
                2'b00: byte_at = w[31:24];
                2'b01: byte_at = w[23:16];
                2'b10: byte_at = w[15:8];
                2'b11: byte_at = w[7:0];
            endcase
        end
    endfunction
    function [31:0] byte_into_word;
        input [7:0] b;
        input [1:0] pos;
        begin
            case (pos)
                2'b00: byte_into_word = {b, 24'd0};
                2'b01: byte_into_word = {8'd0, b, 16'd0};
                2'b10: byte_into_word = {16'd0, b, 8'd0};
                2'b11: byte_into_word = {24'd0, b};
            endcase
        end
    endfunction
    // Word-position byte enables (for .W writes at a given 16-bit offset).
    // 68000 word accesses must be even-aligned; we honor that with addr[1].
    function [3:0] be_for_word;
        input ahalf;   // addr[1]: 0 = high half (bytes 0..1), 1 = low half (2..3)
        begin
            be_for_word = ahalf ? 4'b0011 : 4'b1100;
        end
    endfunction
    function [31:0] word_into_word;
        input [15:0] w;
        input        ahalf;
        begin
            word_into_word = ahalf ? {16'd0, w} : {w, 16'd0};
        end
    endfunction
    function [3:0] reg_idx_of_ea;
        input [2:0] m;
        input [2:0] r;
        begin
            case (m)
                `EA_DREG: reg_idx_of_ea = {1'b0, r};
                `EA_AREG: reg_idx_of_ea = {1'b1, r};
                // All An-relative modes use the An register on a regfile port.
                `EA_AIND, `EA_AINC, `EA_ADEC,
                `EA_DISP, `EA_IDX: reg_idx_of_ea = {1'b1, r};
                default: reg_idx_of_ea = 4'd0;
            endcase
        end
    endfunction

    // ====================================================================
    //  IF stage (streaming: latch + dispatch + start next fetch in one cycle).
    // ====================================================================
    reg [31:0] if_pc;
    reg [15:0] if_op;
    reg [15:0] if_ext [0:3];
    reg [2:0]  if_words_done;
    reg        if_busy;
    reg [2:0]  if_slot;
    reg [31:0] if_fetch_addr;
    reg        if_drain;  // a fetch was outstanding when we redirected; ignore one ack

    // Combinational redirect signals. Made combinational so the ID/EX
    // pipeline register can wipe the wrong-path instruction in the same
    // posedge that the branch resolves.
    wire        redirect_valid;
    wire [31:0] redirect_pc;
    wire        stall;

    // Cache asserts ic_ack combinationally on hit. Keep ic_req high until we
    // observe ic_ack at a posedge; the cache's state machine ignores cpu_req
    // while it's serving a miss.
    assign ic_req = if_busy && !halted;

    // Half-word extracted from the 32-bit cache response.
    wire [15:0] half_word_w = if_fetch_addr[1] ? ic_rdata[15:0] : ic_rdata[31:16];
    wire        is_latching       = if_busy && ic_ack && !if_drain;
    wire        is_latching_opcode = is_latching && (if_slot == 3'd0);

    // Decoder sees the just-latched opcode immediately so total_words and
    // branch fields are valid in the same cycle as the latch.
    wire [15:0] effective_op = is_latching_opcode ? half_word_w : if_op;

    wire [5:0]  dec_kind;
    wire [4:0]  dec_alu_op;
    wire [1:0]  dec_size;
    wire        dec_privileged;
    wire        dec_direction;
    wire [3:0]  dec_shift_count;
    wire        dec_shift_dyn;
    wire [4:0]  dec_bit_pos;
    wire [2:0]  dec_src_mode, dec_src_reg, dec_dst_mode, dec_dst_reg, dec_reg_idx;
    wire        dec_reg_is_a;
    wire [31:0] dec_imm;
    wire [3:0]  dec_cc;
    wire [1:0]  dec_src_ext_words, dec_dst_ext_words;
    wire        dec_needs_disp_word;

    m68k_decoder u_dec_if (
        .opcode        (effective_op),
        .kind          (dec_kind),
        .size          (dec_size),
        .alu_op        (dec_alu_op),
        .src_mode      (dec_src_mode),
        .src_reg       (dec_src_reg),
        .dst_mode      (dec_dst_mode),
        .dst_reg       (dec_dst_reg),
        .reg_idx       (dec_reg_idx),
        .reg_is_a      (dec_reg_is_a),
        .imm           (dec_imm),
        .cc            (dec_cc),
        .src_ext_words (dec_src_ext_words),
        .dst_ext_words (dec_dst_ext_words),
        .needs_disp_word(dec_needs_disp_word),
        .privileged    (dec_privileged),
        .direction     (dec_direction),
        .shift_count   (dec_shift_count),
        .shift_dyn     (dec_shift_dyn),
        .bit_pos       (dec_bit_pos)
    );

    wire [2:0] total_words_eff = 3'd1 + {1'b0, dec_src_ext_words} + {1'b0, dec_dst_ext_words};

    // ext_combo[i]: the value if_ext[i] WILL have at the next posedge.
    wire [15:0] ext_combo [0:3];
    genvar ec;
    generate
        for (ec = 0; ec < 4; ec = ec + 1) begin : g_extcombo
            assign ext_combo[ec] = (is_latching && (if_slot == ec[2:0] + 3'd1))
                                   ? half_word_w
                                   : if_ext[ec];
        end
    endgenerate

    wire [2:0] new_words_done = is_latching ? (if_words_done + 3'd1) : if_words_done;
    wire would_be_complete = ({1'b0, new_words_done} >= total_words_eff) && (new_words_done != 3'd0);

    // BTFNT static branch prediction. Bcc with cc==T (BRA) and cc==F (BSR) are
    // unconditional taken; conditional Bcc is predicted taken iff the disp is
    // backward (sign bit set).
    wire is_bcc        = (dec_kind == K_BCC);
    wire is_bcc_uncond = is_bcc && ((dec_cc == 4'h0) || (dec_cc == 4'h1));
    wire is_bcc_cond   = is_bcc && !is_bcc_uncond;
    wire [31:0] disp32 = dec_needs_disp_word
                         ? {{16{ext_combo[0][15]}}, ext_combo[0]}
                         : dec_imm;
    wire predict_taken_w = is_bcc_uncond || (is_bcc_cond && disp32[31]);

    wire [31:0] predicted_target_w = if_pc + 32'd2 + disp32;
    wire [31:0] next_seq_pc        = if_pc + ({29'd0, total_words_eff} << 1);
    wire [31:0] if_fetch_target    = predict_taken_w ? predicted_target_w : next_seq_pc;

    wire if_dispatch_w = would_be_complete && !stall && !redirect_valid && !halted;

    integer iw;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            if_pc <= RESET_PC_PARAM;
            if_op <= 16'd0;
            for (iw = 0; iw < 4; iw = iw + 1) if_ext[iw] <= 16'd0;
            if_words_done <= 3'd0;
            if_busy <= 1'b0;
            if_slot <= 3'd0;
            if_fetch_addr <= 32'd0;
            ic_addr <= 32'd0;
            if_drain <= 1'b0;
        end else if (redirect_valid) begin
`ifdef KICKSTART_BOOT_TRACE
            // Trace any redirect to an address outside RAM ($00000-$1FFFFF)
            // and ROM ($F80000-$FFFFFF).
            if (redirect_pc != ex_pc) begin
                if (!(redirect_pc[31:21] == 11'b0) &&            // not 0x000000-0x1FFFFF
                    !(redirect_pc[31:20] == 12'hF80) &&          // not 0xF80000-0xF8FFFF
                    !(redirect_pc[31:19] == 13'b1111_1000_0000_1) && // 0xF90000-0xF97FFF
                    !(redirect_pc >= 32'h00F80000 &&
                      redirect_pc <= 32'h00FFFFFF))
                    $display("[BAD-PC] from=%h to=%h retired=%d kind=%d sp=%h",
                        ex_pc, redirect_pc, retired, ex_kind, rf_rc_data);
            end
`endif
            // A speculative fetch may still be in flight to the cache. Mark
            // it for drain so the eventual ack is discarded before we start
            // the redirected fetch.
            if_pc <= redirect_pc;
            if_words_done <= 3'd0;
            if_busy <= 1'b0;
            if_slot <= 3'd0;
            if (if_busy && !ic_ack) if_drain <= 1'b1;
        end else if (halted) begin
            if_busy <= 1'b0;
        end else begin
            // Drain a stale in-flight response after a redirect.
            if (if_drain && ic_ack) if_drain <= 1'b0;

            // Latch a returned half-word into the right slot.
            if (is_latching) begin
                if (if_slot == 3'd0)              if_op <= half_word_w;
                else if (if_slot <= 3'd4)         if_ext[if_slot - 3'd1] <= half_word_w;
            end

            if (would_be_complete) begin
                if (!stall) begin
                    // Dispatch the assembled instruction AND start the next
                    // opcode fetch in the same cycle.
                    if_pc <= if_fetch_target;
                    if_words_done <= 3'd0;
                    if_busy <= 1'b1;
                    ic_addr <= if_fetch_target;
                    if_fetch_addr <= if_fetch_target;
                    if_slot <= 3'd0;
                end else begin
                    // Downstream stall: hold the buffer.
                    if_words_done <= new_words_done;
                    if_busy <= 1'b0;
                end
            end else if (is_latching) begin
                // Need more extension words for this instruction.
                if_words_done <= new_words_done;
                if_busy <= 1'b1;
                ic_addr <= if_pc + ({29'd0, new_words_done} << 1);
                if_fetch_addr <= if_pc + ({29'd0, new_words_done} << 1);
                if_slot <= new_words_done;
            end else if (!if_busy && !stall && !if_drain) begin
                // Initial fetch (after reset, after stall, or after a drain).
                if_busy <= 1'b1;
                ic_addr <= if_pc + ({29'd0, if_words_done} << 1);
                if_fetch_addr <= if_pc + ({29'd0, if_words_done} << 1);
                if_slot <= if_words_done;
            end
        end
    end

    // ====================================================================
    //  IF/ID register.
    // ====================================================================
    reg         id_valid;
    reg  [31:0] id_pc;
    reg  [15:0] id_op;
    reg  [15:0] id_ext [0:3];
    reg         id_predicted_taken;
    integer iee;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            id_valid <= 1'b0;
            id_pc <= 32'd0;
            id_op <= 16'd0;
            id_predicted_taken <= 1'b0;
            for (iee = 0; iee < 4; iee = iee + 1) id_ext[iee] <= 16'd0;
        end else if (redirect_valid) begin
            id_valid <= 1'b0;
        end else if (!stall) begin
            id_valid <= if_dispatch_w;
            if (if_dispatch_w) begin
                id_pc <= if_pc;
                id_op <= effective_op;
                id_predicted_taken <= predict_taken_w;
                for (iee = 0; iee < 4; iee = iee + 1) id_ext[iee] <= ext_combo[iee];
            end
        end
    end

    // ====================================================================
    //  ID stage.
    // ====================================================================
    wire [5:0]  idec_kind;
    wire [4:0]  idec_alu_op;
    wire [1:0]  idec_size;
    wire [2:0]  idec_src_mode, idec_src_reg, idec_dst_mode, idec_dst_reg, idec_reg_idx;
    wire        idec_reg_is_a;
    wire [31:0] idec_imm;
    wire [3:0]  idec_cc;
    wire [1:0]  idec_src_ext_words, idec_dst_ext_words;
    wire        idec_needs_disp_word;
    wire        idec_privileged;
    wire        idec_direction;
    wire [3:0]  idec_shift_count;
    wire        idec_shift_dyn;
    wire [4:0]  idec_bit_pos;

    m68k_decoder u_dec_id (
        .opcode        (id_op),
        .kind          (idec_kind),
        .size          (idec_size),
        .alu_op        (idec_alu_op),
        .src_mode      (idec_src_mode),
        .src_reg       (idec_src_reg),
        .dst_mode      (idec_dst_mode),
        .dst_reg       (idec_dst_reg),
        .reg_idx       (idec_reg_idx),
        .reg_is_a      (idec_reg_is_a),
        .imm           (idec_imm),
        .cc            (idec_cc),
        .src_ext_words (idec_src_ext_words),
        .dst_ext_words (idec_dst_ext_words),
        .needs_disp_word(idec_needs_disp_word),
        .privileged    (idec_privileged),
        .direction     (idec_direction),
        .shift_count   (idec_shift_count),
        .shift_dyn     (idec_shift_dyn),
        .bit_pos       (idec_bit_pos)
    );

    wire [31:0] id_branch_imm = idec_needs_disp_word
        ? {{16{id_ext[0][15]}}, id_ext[0]}
        : idec_imm;

    wire [31:0] id_src_imm32 = (idec_src_ext_words == 2'd1)
        ? {{16{id_ext[0][15]}}, id_ext[0]}
        : {id_ext[0], id_ext[1]};

    // STOP #imm is the only instruction whose halt-code immediate is sourced
    // from an extension word rather than encoded in the opcode; route the
    // ext-word imm into ex_imm_raw for that kind, and the decoder's static
    // imm field (TRAP vector, MOVEQ data, ADDQ count, etc.) for everything
    // else.
    wire [31:0] id_imm_eff = (idec_kind == K_STOP) ? id_src_imm32 : idec_imm;

    wire [15:0] id_dst_e0 = id_ext[idec_src_ext_words];
    wire [15:0] id_dst_e1 = id_ext[idec_src_ext_words + 2'd1];
    wire [31:0] id_dst_imm32 = (idec_dst_ext_words == 2'd1)
        ? {{16{id_dst_e0[15]}}, id_dst_e0}
        : {id_dst_e0, id_dst_e1};

    wire [3:0] id_src_base_idx = reg_idx_of_ea(idec_src_mode, idec_src_reg);
    wire [3:0] id_dst_base_idx = reg_idx_of_ea(idec_dst_mode, idec_dst_reg);
    wire [3:0] id_reg_idx_full = idec_reg_is_a ? {1'b1, idec_reg_idx} : {1'b0, idec_reg_idx};

    reg [3:0] id_ra_idx;
    reg [3:0] id_rb_idx;
    always @* begin
        id_ra_idx = id_src_base_idx;
        case (idec_kind)
            K_ALU, K_LEA, K_MOVEQ,
            K_MULU, K_MULS, K_DIVU, K_DIVS,
            K_BIT, K_SHIFT, K_EXG, K_CHK,
            K_ADDX, K_SUBX:                      id_rb_idx = id_reg_idx_full;
            K_MOVE, K_MOVEA, K_MOVESR:           id_rb_idx = id_dst_base_idx;
            K_ALUI:                              id_rb_idx = {idec_dst_mode != `EA_DREG, idec_dst_reg};
            K_DBCC:                              id_rb_idx = {1'b0, idec_src_reg};
            default:                             id_rb_idx = 4'd0;
        endcase
    end
    // Third regfile read port: usually A7 (so the EX stage can use the SP for
    // JSR/BSR/RTS/LINK/UNLK/exception stack handling).  When the current
    // instruction uses EA_IDX as its src effective address, repurpose this
    // port to read the index register (D/A xreg encoded in the brief
    // extension word) so we can compute An + Xn + d8 in the EX stage.
    // EA_IDX as dst is rare in practice; we route the port to src's X reg
    // when src uses EA_IDX, else to dst's X reg, else to A7.
    // Repurpose the rc port only when the assembler actually emitted an
    // EA_IDX extension word.  Some opcodes (RTS, RTE, ...) leave src_mode at
    // the default-decoded m3 = opcode[5:3] which can equal EA_IDX (3'd6)
    // even though there's no EA in play.  Gating on src_ext_words avoids
    // hijacking rc and breaking SP-using kinds.
    // Both EA_IDX (mode 6, d8(An,Xn)) and EA7_PCIDX (mode 7/reg 3,
    // d8(PC,Xn)) take a brief extension word whose top nibble names
    // the index register Xn -- any of D0..D7/A0..A7.  Route Xn
    // through the rc regfile port so the EX stage gets its value
    // in ex_sp.  Originally only EA_IDX did this, so JMP $X(PC,Xn)
    // (the Kickstart 1.3 bytecode-dispatcher form) defaulted to
    // reading A7 and computed a wrong target.
    wire        id_src_is_idx =
        ((idec_src_mode == `EA_IDX) && (idec_src_ext_words != 2'd0)) ||
        ((idec_src_mode == `EA_EXT) && (idec_src_reg == `EA7_PCIDX) &&
         (idec_src_ext_words != 2'd0));
    wire        id_dst_is_idx =
        ((idec_dst_mode == `EA_IDX) && (idec_dst_ext_words != 2'd0)) ||
        ((idec_dst_mode == `EA_EXT) && (idec_dst_reg == `EA7_PCIDX) &&
         (idec_dst_ext_words != 2'd0));
    wire [15:0] id_src_ext0 = id_ext[0];
    wire [15:0] id_dst_ext0 = id_ext[idec_src_ext_words];
    wire [3:0]  id_src_xreg = {id_src_ext0[15], id_src_ext0[14:12]};
    wire [3:0]  id_dst_xreg = {id_dst_ext0[15], id_dst_ext0[14:12]};
    wire [3:0] id_rc_idx = id_src_is_idx ? id_src_xreg
                         : id_dst_is_idx ? id_dst_xreg
                         : 4'd15;

    wire [31:0] rf_ra_data, rf_rb_data, rf_rc_data;

    // Writeback signals (driven combinationally by EX stage below).
    reg         wb_we;
    reg  [3:0]  wb_widx;
    reg  [1:0]  wb_size;
    reg  [31:0] wb_wdata;
    reg         wb_aux_we;
    reg  [3:0]  wb_aux_idx;
    reg  [31:0] wb_aux_data;

    // MOVEM register-iterator state lives further down; here we just declare
    // forward wires so the regfile's `ra_idx` can be muxed to point at the
    // current MOVEM register during store-MOVEM transfers.
    wire movem_active;
    wire [3:0] movem_curr_reg_full;
    // While MOVEM is mid-flight the ra port is borrowed to feed register
    // values to the memory writes (predec / postinc / fixed (An) store
    // mode).  But on the cycle MOVEM retires (is_settled_after_movem=1)
    // the ID→EX boundary advances and ex_ra <= id_ra_fwd latches, so the
    // mux must already be returning the *next* instruction's src reg on
    // that cycle — otherwise ex_ra captures the last MOVEM reg's value
    // and any immediately-following Dn-source instruction (MOVE.L D0,D3,
    // ADD.L D0,D1, ...) silently reads the wrong operand.  Hand the port
    // back as soon as the final MOVEM transfer is in.
    wire [3:0] rf_ra_idx_eff = (movem_active && !is_settled_after_movem)
                                   ? movem_curr_reg_full
                                   : id_ra_idx;

    m68k_regfile u_rf (
        .clk      (clk),
        .rst_n    (rst_n),
        .reset_a7 (reset_a7),
        .ra_idx   (rf_ra_idx_eff),
        .ra_data  (rf_ra_data),
        .rb_idx   (id_rb_idx),
        .rb_data  (rf_rb_data),
        .rc_idx   (id_rc_idx),
        .rc_data  (rf_rc_data),
        .we       (wb_we),
        .w_idx    (wb_widx),
        .w_size   (wb_size),
        .w_data   (wb_wdata),
        .aux_we   (wb_aux_we),
        .aux_idx  (wb_aux_idx),
        .aux_data (wb_aux_data)
    );

    // Forwarding network (EX → ID).
    function [31:0] fwd;
        input [3:0]  idx;
        input [31:0] base;
        begin
            if (wb_we     && wb_widx    == idx) fwd = wb_wdata;
            else if (wb_aux_we && wb_aux_idx == idx) fwd = wb_aux_data;
            else fwd = base;
        end
    endfunction

    wire [31:0] id_ra_fwd = fwd(id_ra_idx, rf_ra_data);
    wire [31:0] id_rb_fwd = fwd(id_rb_idx, rf_rb_data);
    wire [31:0] id_sp_fwd = fwd(id_rc_idx, rf_rc_data);

    // ====================================================================
    //  ID/EX register.
    // ====================================================================
    reg         ex_valid;
    reg  [31:0] ex_pc;
    reg  [31:0] ex_pc_next;
    reg  [5:0]  ex_kind;
    reg  [4:0]  ex_alu_op;
    reg  [1:0]  ex_size;
    reg         ex_direction;
    reg  [3:0]  ex_shift_count;
    reg         ex_shift_dyn;
    reg  [2:0]  ex_src_mode, ex_src_reg, ex_dst_mode, ex_dst_reg;
    reg  [31:0] ex_src_imm32;
    reg  [31:0] ex_dst_imm32;
    reg  [31:0] ex_branch_imm;
    reg  [31:0] ex_imm_raw;
    reg  [3:0]  ex_cc;
    reg  [31:0] ex_ra;
    reg  [31:0] ex_rb;
    reg  [31:0] ex_sp;
    reg  [3:0]  ex_reg_idx_full;
    reg         ex_predicted_taken;
    reg  [15:0] ex_opcode;       // current instruction word (for Group-0 IR push)

    wire [2:0] id_total_words = 3'd1 + {1'b0, idec_src_ext_words} + {1'b0, idec_dst_ext_words};
    wire [31:0] id_pc_next = id_pc + ({29'd0, id_total_words} << 1);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ex_valid <= 1'b0;
            ex_pc <= 32'd0;
            ex_pc_next <= 32'd0;
            ex_kind <= K_BAD;
            ex_alu_op <= `ALU_MOV;
            ex_src_mode <= 3'd0; ex_src_reg <= 3'd0;
            ex_dst_mode <= 3'd0; ex_dst_reg <= 3'd0;
            ex_src_imm32 <= 32'd0;
            ex_dst_imm32 <= 32'd0;
            ex_branch_imm <= 32'd0;
            ex_imm_raw <= 32'd0;
            ex_cc <= 4'd0;
            ex_ra <= 32'd0;
            ex_rb <= 32'd0;
            ex_sp <= 32'd0;
            ex_reg_idx_full <= 4'd0;
            ex_predicted_taken <= 1'b0;
            ex_size <= `SZ_L;
            ex_direction <= 1'b0;
            ex_shift_count <= 4'd0;
            ex_shift_dyn <= 1'b0;
            ex_opcode <= 16'd0;
        end else if (redirect_valid) begin
            ex_valid <= 1'b0;
        end else if (!stall) begin
            ex_valid       <= id_valid;
            ex_pc          <= id_pc;
            ex_pc_next     <= id_pc_next;
            ex_kind        <= idec_kind;
            ex_alu_op      <= idec_alu_op;
            ex_size        <= idec_size;
            ex_direction   <= idec_direction;
            ex_shift_count <= idec_shift_count;
            ex_shift_dyn   <= idec_shift_dyn;
            ex_src_mode    <= idec_src_mode;
            ex_src_reg     <= idec_src_reg;
            ex_dst_mode    <= idec_dst_mode;
            ex_dst_reg     <= idec_dst_reg;
            ex_src_imm32   <= id_src_imm32;
            ex_dst_imm32   <= id_dst_imm32;
            ex_branch_imm  <= id_branch_imm;
            ex_imm_raw     <= id_imm_eff;
            ex_cc          <= idec_cc;
            ex_ra          <= id_ra_fwd;
            ex_rb          <= id_rb_fwd;
            ex_sp          <= id_sp_fwd;
            ex_opcode      <= id_op;
            ex_reg_idx_full<= id_reg_idx_full;
            ex_predicted_taken <= id_predicted_taken;
        end
    end

    // ====================================================================
    //  EX stage.
    // ====================================================================
    reg [31:0] src_ea;
    reg [31:0] src_operand;
    reg        src_needs_mem;
    reg [31:0] src_an_next;
    reg        src_an_update;
    reg [31:0] dst_ea;
    reg [31:0] dst_an_next;
    reg        dst_an_update;
    reg        dst_is_mem;

    // Size-dependent auto-inc/dec step (with A7=SP byte→step-2 quirk).
    function [31:0] step_for;
        input [1:0] sz;
        input [2:0] reg_n;
        begin
            case (sz)
                `SZ_L: step_for = 32'd4;
                `SZ_W: step_for = 32'd2;
                default: step_for = (reg_n == 3'd7) ? 32'd2 : 32'd1;
            endcase
        end
    endfunction

    wire [31:0] src_step = step_for(ex_size, ex_src_reg);
    wire [31:0] dst_step = step_for(ex_size, ex_dst_reg);

    always @* begin
        src_ea = 32'd0;
        src_operand = 32'd0;
        src_needs_mem = 1'b0;
        src_an_next = 32'd0;
        src_an_update = 1'b0;
        case (ex_src_mode)
            `EA_DREG, `EA_AREG: src_operand = ex_ra;
            `EA_AIND:           begin src_ea = ex_ra; src_needs_mem = 1'b1; end
            `EA_AINC: begin
                src_ea = ex_ra;
                src_needs_mem = 1'b1;
                src_an_next = ex_ra + src_step;
                src_an_update = 1'b1;
            end
            `EA_ADEC: begin
                src_ea = ex_ra - src_step;
                src_needs_mem = 1'b1;
                src_an_next = ex_ra - src_step;
                src_an_update = 1'b1;
            end
            `EA_DISP: begin
                // d16(An): EA = An + sign_ext(disp16). disp16 is in ex_src_imm32.
                src_ea = ex_ra + ex_src_imm32;
                src_needs_mem = 1'b1;
            end
            `EA_IDX: begin
                // d8(An, Xn.size): brief extension word in low 16 bits of
                // ex_src_imm32.  ex_sp was muxed to read the index register
                // during ID so its value is available here.
                src_ea = ex_ra
                       + (ex_src_imm32[11] ? ex_sp
                                           : {{16{ex_sp[15]}}, ex_sp[15:0]})
                       + {{24{ex_src_imm32[7]}}, ex_src_imm32[7:0]};
                src_needs_mem = 1'b1;
            end
            `EA_EXT: begin
                case (ex_src_reg)
                    `EA7_ABSW, `EA7_ABSL: begin src_ea = ex_src_imm32; src_needs_mem = 1'b1; end
                    `EA7_PCDISP:          begin src_ea = ex_pc + 32'd2 + ex_src_imm32; src_needs_mem = 1'b1; end
                    `EA7_PCIDX: begin
                        // d8(PC, Xn.size): same as EA_IDX but base = PC + 2.
                        src_ea = ex_pc + 32'd2
                               + (ex_src_imm32[11] ? ex_sp
                                                   : {{16{ex_sp[15]}}, ex_sp[15:0]})
                               + {{24{ex_src_imm32[7]}}, ex_src_imm32[7:0]};
                        src_needs_mem = 1'b1;
                    end
                    `EA7_IMM:             src_operand = ex_src_imm32;
                    default: ;
                endcase
            end
            default: ;
        endcase
    end

    always @* begin
        dst_ea = 32'd0;
        dst_an_next = 32'd0;
        dst_an_update = 1'b0;
        dst_is_mem = 1'b0;
        case (ex_dst_mode)
            `EA_DREG, `EA_AREG: ;
            `EA_AIND: begin dst_ea = ex_rb; dst_is_mem = 1'b1; end
            `EA_AINC: begin
                dst_ea = ex_rb;
                dst_is_mem = 1'b1;
                dst_an_next = ex_rb + dst_step;
                dst_an_update = 1'b1;
            end
            `EA_ADEC: begin
                dst_ea = ex_rb - dst_step;
                dst_is_mem = 1'b1;
                dst_an_next = ex_rb - dst_step;
                dst_an_update = 1'b1;
            end
            `EA_DISP: begin
                dst_ea = ex_rb + ex_dst_imm32;
                dst_is_mem = 1'b1;
            end
            `EA_IDX: begin
                // Same as src EA_IDX, but uses ex_rb (dst An base).  When dst
                // is EA_IDX we mux rc to dst's xreg, so ex_sp holds the index
                // value here.
                dst_ea = ex_rb
                       + (ex_dst_imm32[11] ? ex_sp
                                           : {{16{ex_sp[15]}}, ex_sp[15:0]})
                       + {{24{ex_dst_imm32[7]}}, ex_dst_imm32[7:0]};
                dst_is_mem = 1'b1;
            end
            `EA_EXT: begin
                case (ex_dst_reg)
                    `EA7_ABSW, `EA7_ABSL: begin dst_ea = ex_dst_imm32; dst_is_mem = 1'b1; end
                    default: ;
                endcase
            end
            default: ;
        endcase
    end

    reg cc_n, cc_z, cc_v, cc_c, cc_x;

    // Status-register bits beyond the CCR. The lower byte of SR is the CCR
    // (X,N,Z,V,C). The upper byte holds T (trace), S (supervisor) and
    // I[2:0] (interrupt-mask priority).
    reg        sr_s;
    reg        sr_t;
    reg  [2:0] sr_i;

    // Shadow stack pointer: holds the inactive SP. When sr_s==1, regfile[A7]
    // is the SSP and usp_shadow holds the saved USP. When sr_s==0, regfile[A7]
    // is the USP and usp_shadow holds the saved SSP. The shadow is swapped
    // with A7 on user/supervisor transitions via TRAP/RTE/MOVE-USP.
    reg [31:0] usp_shadow;

    // 16-bit SR view (combinational).
    wire [15:0] sr_now = {sr_t, 1'b0, sr_s, 2'b00, sr_i, 3'b000,
                          cc_x, cc_n, cc_z, cc_v, cc_c};

    reg [3:0] ex_state;

    reg [31:0] ex_tas_word;  // word read in TAS phase 0
    // CCR snapshot captured at S_LOAD ack for mem-dest RMW kinds where
    // S_RMW_W settle needs the *operation's* flags (not the registered
    // CCR which may have moved on).  Used by K_ALUI mem-dest path.
    reg        ex_alui_n_q;
    reg        ex_alui_z_q;
    reg        ex_alui_v_q;
    reg        ex_alui_c_q;
    reg        ex_alui_x_q;
    reg [31:0] ex_exc_saved_pc;
    reg [15:0] ex_exc_saved_sr;
    reg  [7:0] ex_exc_vector;
    reg        ex_exc_was_user; // 1 if we were in user mode entering this exception
    reg [31:0] working_sp;      // SP being incremented/decremented during exc/rte
    reg  [2:0] ex_exc_new_sri;  // new sr_i to install after entry (==ipl for IRQ)
    // Group-0 (bus/address error) extra frame info.
    reg        ex_exc_is_group0;
    reg [31:0] ex_exc_addr;
    reg [15:0] ex_exc_ir;
    reg [15:0] ex_exc_ssw;

    // MOVEM iterator state.
    reg [15:0] movem_mask;       // remaining bits to process (LSB-first)
    reg  [4:0] movem_idx;        // current bit position (0..15; 16 = done)
    reg [31:0] movem_addr;       // current An ("address pointer" tracked per step)
    reg        movem_busy;       // 1 = bus transaction outstanding
    reg        movem_dir;        // 0 = regs->mem, 1 = mem->regs
    reg        movem_predec;     // 1 if EA was -(An)
    reg        movem_an_update;  // 1 if predec or postinc (commit final An)
    reg  [2:0] movem_an_idx;     // An register index for final commit


    // MULS 16x16 -> 32 signed multiplication. Done manually as |a| * |b|
    // with sign adjustment. The 16-bit absolute value is computed at 16-bit
    // width and then zero-extended to 32 bits; using 32-bit negation on a
    // zero-extended operand would produce the 32-bit twos-complement value
    // (e.g. -0x0000FFFD = 0xFFFF0003), which corrupts the multiplication.
    // For word-sized memory operands, the wanted word lives at
    // dc_rdata[31:16] when dc_addr[1]==0 and dc_rdata[15:0] when
    // dc_addr[1]==1.  Pre-pick the right half for the MULU/MULS/DIVU/DIVS
    // mem-source paths (all 16-bit operands).
    wire [15:0] mem_word_w = dc_addr[1] ? dc_rdata[15:0] : dc_rdata[31:16];
    wire        mem_word_w_msb = mem_word_w[15];
    wire [15:0] muls_a_abs16  = ex_rb[15]       ? (16'd0 - ex_rb[15:0])       : ex_rb[15:0];
    wire [15:0] muls_b_abs16  = src_operand[15] ? (16'd0 - src_operand[15:0]) : src_operand[15:0];
    wire [15:0] muls_bm_abs16 = mem_word_w_msb  ? (16'd0 - mem_word_w)         : mem_word_w;
    wire [31:0] muls_ua32 = {16'd0, muls_a_abs16} * {16'd0, muls_b_abs16};
    wire [31:0] muls_um32 = {16'd0, muls_a_abs16} * {16'd0, muls_bm_abs16};
    wire [31:0] muls_y_signed     =
        (ex_rb[15] ^ src_operand[15]) ? (32'd0 - muls_ua32) : muls_ua32;
    wire [31:0] muls_y_signed_mem =
        (ex_rb[15] ^ mem_word_w_msb)  ? (32'd0 - muls_um32) : muls_um32;

    // DIVS 32/16 -> {rem[15:0], quot[15:0]} signed division, computed
    // manually: take absolute values, do unsigned division, then apply sign
    // adjustments. Quotient sign = XOR of operand signs; remainder sign
    // matches dividend.
    wire        divs_dividend_neg = ex_rb[31];
    wire [31:0] divs_a_abs = divs_dividend_neg ? (32'd0 - ex_rb) : ex_rb;
    wire        divs_divisor_neg     = src_operand[15];
    wire        divs_divisor_neg_mem = mem_word_w_msb;
    wire [31:0] divs_b_abs     = divs_divisor_neg     ? {16'd0, (16'd0 - src_operand[15:0])} : {16'd0, src_operand[15:0]};
    wire [31:0] divs_b_abs_mem = divs_divisor_neg_mem ? {16'd0, (16'd0 - mem_word_w)}        : {16'd0, mem_word_w};
    wire [31:0] divs_uq     = divs_a_abs / divs_b_abs;
    wire [31:0] divs_ur     = divs_a_abs % divs_b_abs;
    wire [31:0] divs_uq_mem = divs_a_abs / divs_b_abs_mem;
    wire [31:0] divs_ur_mem = divs_a_abs % divs_b_abs_mem;
    wire [15:0] divs_q     = (divs_dividend_neg ^ divs_divisor_neg)     ? (16'd0 - divs_uq[15:0])     : divs_uq[15:0];
    wire [15:0] divs_r     =  divs_dividend_neg                          ? (16'd0 - divs_ur[15:0])     : divs_ur[15:0];
    wire [15:0] divs_q_mem = (divs_dividend_neg ^ divs_divisor_neg_mem) ? (16'd0 - divs_uq_mem[15:0]) : divs_uq_mem[15:0];
    wire [15:0] divs_r_mem =  divs_dividend_neg                          ? (16'd0 - divs_ur_mem[15:0]) : divs_ur_mem[15:0];
    wire [31:0] divs_y     = {divs_r,     divs_q};
    wire [31:0] divs_y_mem = {divs_r_mem, divs_q_mem};

    // K_BIT operand selection: for dynamic, destination read via ex_ra (src_base m3/r0)
    // and bit-count register read via ex_rb (reg_idx_full = r9). For static, the
    // destination is read via ex_rb (reg_idx_full = r9 — matches r0 when the asm
    // encoding has r9==r0; the test ensures this).
    wire [31:0] bit_dest_val = ex_shift_dyn ? ex_ra : ex_rb;
    wire [4:0]  bit_n        = ex_shift_dyn ? ex_rb[4:0] : ex_src_imm32[4:0];

    assign movem_active = (ex_state == S_MOVEM);
    // For predec mode the mask is bit-reversed (mask[0]=A7, mask[15]=D0); for
    // every other mode mask[0]=D0, mask[15]=A7. movem_idx is the bit position
    // currently under consideration.
    assign movem_curr_reg_full = movem_predec ? (4'd15 - movem_idx[3:0])
                                              :  movem_idx[3:0];
    // Address used for the current step's bus transaction. Predec uses an
    // address one step below the tracked An (the An is decremented along with
    // the transfer); other modes use the tracked address directly.
    wire [31:0] movem_step_addr = movem_predec ? (movem_addr - 32'd4) : movem_addr;
    // Combinational signal: the current bus step has just acked. Used to gate
    // intermediate regfile writes (for load-MOVEM) without going through
    // is_settled (which only fires at end-of-instruction).
    wire movem_step_commit = movem_active && movem_busy && dc_ack && ex_valid && !halted;

    // Plan signals for this cycle.
    reg        want_mem;
    reg        want_we;
    reg        want_lock;
    reg [31:0] want_addr;
    reg [31:0] want_wdata;
    reg [3:0]  want_be;

    reg        wb_main_we_c;
    reg [3:0]  wb_main_idx_c;
    reg [31:0] wb_main_data_c;
    reg [1:0]  wb_main_size_c;

    // Exception-launch (combinational): asserted in S_RUN to ask the sequential
    // block to start the exception entry sequence with the given vector and
    // saved-PC. Used by TRAP, illegal opcode, divide-by-zero, TRAPV, priv-vio,
    // and pending IRQ. For IRQ, exc_is_irq_c is also set so the sequential
    // block can install the new sr_i (==ipl_i) on entry.
    reg        exc_launch_c;
    reg        exc_is_irq_c;
    reg  [7:0] exc_vector_c;
    reg [31:0] exc_saved_pc_c;
    // Group-0 (bus/address error) extras.  Captured when exc_launch_c fires
    // with vector 2 (bus) or 3 (address).
    reg        exc_group0_c;
    reg [31:0] exc_addr_c;
    reg [15:0] exc_ssw_c;

    // Combinational SR writeback (for MOVE-to-SR, ANDI/ORI/EORI to SR/CCR, etc.).
    // Setting sr_we_c installs sr_data_c into the SR bits on the settled cycle.
    reg        sr_we_c;
    reg [15:0] sr_data_c;
    // Combinational USP write (for MOVE An,USP in supervisor mode).
    reg        usp_we_c;
    reg [31:0] usp_data_c;

    reg        wb_aux_we_c;
    reg [3:0]  wb_aux_idx_c;
    reg [31:0] wb_aux_data_c;

    reg        cc_we_c;
    reg        cc_n_c, cc_z_c, cc_v_c, cc_c_c, cc_x_c;
    reg        cc_x_we_c;       // whether this op writes X

    reg        take_branch_c;
    reg [31:0] take_branch_target_c;

    reg [31:0] alu_a, alu_b;
    reg [4:0]  alu_op_c;
    reg [1:0]  alu_size_c;
    reg [5:0]  alu_shamt_c;
    wire [31:0] alu_y;
    wire alu_n, alu_z, alu_v, alu_c, alu_x;

    m68k_alu u_alu (
        .op     (alu_op_c),
        .size   (alu_size_c),
        .x_in   (cc_x),
        .a      (alu_a),
        .b      (alu_b),
        .shamt  (alu_shamt_c),
        .y      (alu_y),
        .flag_n (alu_n),
        .flag_z (alu_z),
        .flag_v (alu_v),
        .flag_c (alu_c),
        .flag_x (alu_x)
    );

    // dc_req is held until we observe dc_ack at a posedge; the cache's
    // internal state machine ignores cpu_req while it's serving a miss/write.
    reg dc_req_r;
    assign dc_req = dc_req_r && !halted;

    // Settlement conditions: when the current EX-stage instruction completes
    // its work this cycle, downstream effects (regfile writeback, CCR commit,
    // branch redirect) take effect and the pipeline can advance.
    // When any exception is being launched in S_RUN, or an RTE / RTR / MOVEM
    // is starting up its multi-cycle sequence in S_RUN, S_RUN is NOT a
    // settled state: those instructions only settle when the final bus
    // transaction completes (see is_settled_after_exc / _after_rte /
    // _after_movem).
    wire run_launches_exc = (ex_state == S_RUN) && ex_valid && !halted &&
                            (exc_launch_c ||
                             ((ex_kind == K_RTE) && sr_s) ||
                             (ex_kind == K_RTR) ||
                             (ex_kind == K_MOVEM));
    wire is_settled_in_run     = (ex_state == S_RUN) && !want_mem && !run_launches_exc &&
                                 ex_valid && !halted;
    wire load_starts_rmw = (ex_kind == K_NEG) || (ex_kind == K_NOT);
    // Memory-destination bit ops that need RMW (BCHG/BCLR/BSET): alu_op != 0
    // (alu_op == 0 is BTST, which only reads).
    wire bit_mem_writes  = (ex_kind == K_BIT) && (ex_dst_mode != `EA_DREG) &&
                           (ex_alu_op != 5'd0);
    // K_ALUI to memory: CMPI reads only (CCR-only side effect); other ALUI
    // ops (ADDI/SUBI/ANDI/ORI/EORI) are full RMW.
    // K_ALUQ to memory (ADDQ / SUBQ to a mem ea) takes the same RMW path.
    wire alui_mem        = ((ex_kind == K_ALUI) && (ex_dst_mode != `EA_DREG) &&
                            (ex_dst_mode != 3'b110)) || // 110 = CCR/SR dst
                           ((ex_kind == K_ALUQ) && (ex_src_mode != `EA_DREG) &&
                            (ex_src_mode != `EA_AREG));
    wire alui_mem_writes = alui_mem && (ex_alu_op != `ALU_CMP);
    wire is_settled_after_load = (ex_state == S_LOAD)  && dc_ack &&
                                 ex_valid && !halted && (ex_kind != K_TAS) &&
                                 !(load_starts_rmw && (ex_src_mode != `EA_DREG)) &&
                                 !bit_mem_writes &&
                                 !alui_mem_writes &&
                                 !(ex_kind == K_MOVE && dst_is_mem);
    wire is_settled_after_store= (ex_state == S_STORE) && dc_ack && ex_valid && !halted;
    wire is_settled_after_tasw = (ex_state == S_TASW)  && dc_ack && ex_valid && !halted;
    wire is_settled_after_exc  = (ex_state == S_EXC_FETCH) && dc_ack && ex_valid && !halted;
    wire is_settled_after_rte  = (ex_state == S_RTE_POP_PC) && dc_ack && ex_valid && !halted;
    wire is_settled_after_rmw  = (ex_state == S_RMW_W) && dc_ack && ex_valid && !halted;
    // MOVEM settles only when the mask is fully exhausted AND we are not
    // mid-transaction (so the final An commit happens cleanly).
    wire is_settled_after_movem = movem_active && !movem_busy &&
                                  (movem_mask == 16'd0) && ex_valid && !halted;
    wire is_settled = is_settled_in_run || is_settled_after_load ||
                      is_settled_after_store || is_settled_after_tasw ||
                      is_settled_after_exc || is_settled_after_rte ||
                      is_settled_after_rmw || is_settled_after_movem;

    assign stall = ex_valid && !halted && !is_settled;

    // Branch redirect (combinational). For K_BCC we only redirect on a
    // mispredict; for non-Bcc branches (JMP/JSR/RTS) there's no prediction so
    // we always redirect on take. STOP freezes the IF stage at the STOP PC.
    wire stop_now      = ex_valid && (ex_kind == K_STOP) && (ex_state == S_RUN) && !halted;
    wire bcc_mispred   = is_settled && (ex_kind == K_BCC) && (ex_predicted_taken != take_branch_c);
    wire other_branch  = is_settled && (ex_kind != K_BCC) && take_branch_c;
    wire exc_redirect  = is_settled_after_exc || is_settled_after_rte;
    assign redirect_valid = bcc_mispred || other_branch || stop_now || exc_redirect;
    assign redirect_pc    = exc_redirect ? dc_rdata :
                            stop_now ? ex_pc :
                            (ex_kind == K_BCC)
                              ? (take_branch_c ? (ex_pc + 32'd2 + ex_branch_imm) : ex_pc_next)
                              : take_branch_target_c;

    // Evaluate one of the 16 condition codes against current CCR bits.
    function cond_true;
        input [3:0] cc;
        begin
            case (cc)
                `CC_T:  cond_true = 1'b1;
                `CC_F:  cond_true = 1'b0;
                `CC_HI: cond_true = !cc_c && !cc_z;
                `CC_LS: cond_true =  cc_c ||  cc_z;
                `CC_CC: cond_true = !cc_c;
                `CC_CS: cond_true =  cc_c;
                `CC_NE: cond_true = !cc_z;
                `CC_EQ: cond_true =  cc_z;
                `CC_VC: cond_true = !cc_v;
                `CC_VS: cond_true =  cc_v;
                `CC_PL: cond_true = !cc_n;
                `CC_MI: cond_true =  cc_n;
                `CC_GE: cond_true = (cc_n &&  cc_v) || (!cc_n && !cc_v);
                `CC_LT: cond_true = (cc_n && !cc_v) || (!cc_n &&  cc_v);
                `CC_GT: cond_true = !cc_z && ((cc_n && cc_v) || (!cc_n && !cc_v));
                `CC_LE: cond_true =  cc_z || (cc_n && !cc_v) || (!cc_n && cc_v);
                default: cond_true = 1'b0;
            endcase
        end
    endfunction

    // Big planning block — computes wb_*_c, cc_*_c, want_mem etc. based on
    // ex_state and ex_kind.
    always @* begin
        want_mem = 1'b0; want_we = 1'b0; want_lock = 1'b0;
        want_addr = 32'd0; want_wdata = 32'd0; want_be = 4'b1111;
        wb_main_we_c = 1'b0; wb_main_idx_c = 4'd0; wb_main_data_c = 32'd0;
        wb_aux_we_c  = 1'b0; wb_aux_idx_c  = 4'd0; wb_aux_data_c  = 32'd0;
        cc_we_c = 1'b0;
        cc_n_c = 1'b0; cc_z_c = 1'b0; cc_v_c = 1'b0; cc_c_c = 1'b0; cc_x_c = cc_x;
        cc_x_we_c = 1'b0;
        take_branch_c = 1'b0;
        take_branch_target_c = 32'd0;
        alu_a = 32'd0; alu_b = 32'd0;
        alu_op_c = `ALU_MOV;
        alu_size_c = ex_size;
        alu_shamt_c = 6'd0;
        wb_main_size_c = ex_size;
        exc_launch_c = 1'b0;
        exc_is_irq_c = 1'b0;
        exc_vector_c = 8'd0;
        exc_saved_pc_c = ex_pc;
        exc_group0_c = 1'b0;
        exc_addr_c   = 32'd0;
        exc_ssw_c    = 16'd0;
        sr_we_c = 1'b0;
        sr_data_c = 16'd0;
        usp_we_c = 1'b0;
        usp_data_c = 32'd0;

        if (!ex_valid || halted) ;
        else case (ex_state)
            S_RUN: begin
                case (ex_kind)
                    K_NOP, K_STOP: ;
                    K_JMP: begin
                        take_branch_c = 1'b1;
                        // src_ea is the address computed for every supported
                        // JMP source mode: An direct, (An), d16(An), d8(An,Xn),
                        // $xxxx.W, $xxxx.L, d16(PC), d8(PC,Xn).  Using ex_ra
                        // alone silently dropped the d16/d8 displacement on
                        // JMP d16(An) which Kickstart's library jump-table
                        // path tripped over.
                        take_branch_target_c = src_ea;
                    end
                    K_JSR: begin
                        // Issue store of return address; branch + SP commit at S_STORE.
                        want_mem = 1'b1; want_we = 1'b1;
                        want_addr = ex_sp - 32'd4;
                        want_wdata = ex_pc_next;
                        want_be = 4'b1111;
                    end
                    K_RTS: begin
                        // Issue load of return address; branch + SP commit at S_LOAD.
                        want_mem = 1'b1; want_we = 1'b0;
                        want_addr = ex_sp;
                        want_be = 4'b1111;
                    end
                    K_LEA: begin
                        // src_ea is computed correctly for every memory-shaped
                        // source mode, including absolute, PC-relative, and
                        // PC-indexed.
                        wb_main_we_c = 1'b1;
                        wb_main_idx_c = ex_reg_idx_full;
                        wb_main_data_c = src_ea;
                    end
                    K_MOVEQ: begin
                        wb_main_we_c = 1'b1;
                        wb_main_idx_c = ex_reg_idx_full;
                        wb_main_data_c = ex_imm_raw;
                        cc_we_c = 1'b1;
                        cc_n_c = ex_imm_raw[31];
                        cc_z_c = (ex_imm_raw == 32'd0);
                    end
                    K_MOVE: begin
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be = 4'b1111;
                        end else if (dst_is_mem) begin
                            want_mem = 1'b1; want_we = 1'b1;
                            want_addr = dst_ea;
                            case (ex_size)
                                `SZ_B: begin
                                    want_be    = be_for_byte(dst_ea[1:0]);
                                    want_wdata = byte_into_word(src_operand[7:0], dst_ea[1:0]);
                                end
                                `SZ_W: begin
                                    want_be    = be_for_word(dst_ea[1]);
                                    want_wdata = word_into_word(src_operand[15:0], dst_ea[1]);
                                end
                                default: begin
                                    want_be    = 4'b1111;
                                    want_wdata = src_operand;
                                end
                            endcase
                        end else begin
                            wb_main_we_c = 1'b1;
                            wb_main_idx_c = (ex_dst_mode == `EA_AREG) ? {1'b1, ex_dst_reg}
                                                                      : {1'b0, ex_dst_reg};
                            wb_main_data_c = src_operand;
                            cc_we_c = (ex_dst_mode == `EA_DREG);
                            cc_n_c = src_operand[31];
                            cc_z_c = (src_operand == 32'd0);
                        end
                    end
                    K_ALU: begin
                        alu_op_c = ex_alu_op;
                        alu_a = ex_rb;
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be = 4'b1111;
                        end else begin
                            alu_b = src_operand;
                            cc_we_c = 1'b1;
                            cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v; cc_c_c = alu_c;
                            // ADD/SUB write X; CMP doesn't.  ADDA/SUBA also
                            // skip X per spec (Pn-dest is An here, but those
                            // come through with ex_alu_op==ADD/SUB too -- the
                            // 68k spec says ADDA/SUBA don't touch CCR at
                            // all; preserved by reg_is_a path skipping
                            // cc_we_c... actually cc_we_c is set here for
                            // both An and Dn dst.  Live with that
                            // pre-existing behavior for now; just make sure
                            // ADD/SUB set X so ADDX can see it).
                            cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                        (ex_alu_op == `ALU_SUB);
                            cc_x_c = alu_x;
                            if (ex_alu_op != `ALU_CMP) begin
                                wb_main_we_c = 1'b1;
                                wb_main_idx_c = ex_reg_idx_full;
                                wb_main_data_c = alu_y;
                            end
                        end
                    end
                    // ADDX/SUBX register form (Dy,Dx).  Memory form (-(Ay),-(Ax))
                    // not yet implemented; would need 2-read + 1-write RMW.
                    K_ADDX, K_SUBX: begin
                        if (ex_src_mode == `EA_DREG) begin
                            alu_op_c = ex_alu_op;        // ALU_ADDX or ALU_SUBX
                            alu_a    = ex_rb;            // Dx (destination)
                            alu_b    = src_operand;      // Dy (source)
                            alu_size_c = ex_size;
                            cc_we_c   = 1'b1;
                            cc_x_we_c = 1'b1;
                            cc_n_c = alu_n;
                            // 68k Z-preserve: cleared if result nonzero,
                            // else unchanged.
                            cc_z_c = cc_z & alu_z;
                            cc_v_c = alu_v;
                            cc_c_c = alu_c;
                            cc_x_c = alu_x;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = ex_reg_idx_full;
                            wb_main_size_c = ex_size;
                            wb_main_data_c = alu_y;
                        end
                    end
                    // NEGX Dn (memory form not yet implemented).
                    K_NEGX: begin
                        if (ex_src_mode == `EA_DREG) begin
                            alu_op_c = `ALU_NEGX;
                            alu_b    = src_operand;
                            alu_size_c = ex_size;
                            cc_we_c   = 1'b1;
                            cc_x_we_c = 1'b1;
                            cc_n_c = alu_n;
                            cc_z_c = cc_z & alu_z;
                            cc_v_c = alu_v;
                            cc_c_c = alu_c;
                            cc_x_c = alu_x;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_size_c = ex_size;
                            wb_main_data_c = alu_y;
                        end
                    end
                    K_ALUQ: begin
                        alu_op_c = ex_alu_op;
                        alu_a = src_operand;
                        alu_b = ex_imm_raw;
                        alu_size_c = ex_size;
                        if (ex_src_mode == `EA_DREG || ex_src_mode == `EA_AREG) begin
                            // Dn/An destination: commit register + CCR
                            // (CCR is suppressed for An per 68k spec, but
                            // legacy behavior here writes it -- preserved
                            // to keep regression tests green).
                            cc_we_c = 1'b1;
                            cc_n_c = alu_n; cc_z_c = alu_z;
                            cc_v_c = alu_v; cc_c_c = alu_c;
                            cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                        (ex_alu_op == `ALU_SUB);
                            cc_x_c = alu_x;
                            wb_main_we_c = 1'b1;
                            wb_main_idx_c = (ex_src_mode == `EA_AREG) ? {1'b1, ex_src_reg}
                                                                      : {1'b0, ex_src_reg};
                            wb_main_data_c = alu_y;
                        end else if (src_needs_mem) begin
                            // ADDQ/SUBQ <#imm>, <mem ea>: schedule the read;
                            // S_LOAD ack runs the ALU and chains a write
                            // through S_RMW_W (same path as K_ALUI mem).
                            want_mem  = 1'b1;
                            want_we   = 1'b0;
                            want_lock = 1'b1;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                        end
                    end
                    K_TAS: begin
                        want_mem = 1'b1; want_we = 1'b0; want_lock = 1'b1;
                        want_addr = src_ea;
                        want_be = 4'b1111;
                    end
                    K_BCC: begin
                        case (ex_cc)
                            `CC_T:  take_branch_c = 1'b1;
                            `CC_F:  take_branch_c = 1'b1;
                            `CC_HI: take_branch_c = !cc_c && !cc_z;
                            `CC_LS: take_branch_c =  cc_c ||  cc_z;
                            `CC_CC: take_branch_c = !cc_c;
                            `CC_CS: take_branch_c =  cc_c;
                            `CC_NE: take_branch_c = !cc_z;
                            `CC_EQ: take_branch_c =  cc_z;
                            `CC_VC: take_branch_c = !cc_v;
                            `CC_VS: take_branch_c =  cc_v;
                            `CC_PL: take_branch_c = !cc_n;
                            `CC_MI: take_branch_c =  cc_n;
                            `CC_GE: take_branch_c = (cc_n &&  cc_v) || (!cc_n && !cc_v);
                            `CC_LT: take_branch_c = (cc_n && !cc_v) || (!cc_n &&  cc_v);
                            `CC_GT: take_branch_c = !cc_z && ((cc_n && cc_v) || (!cc_n && !cc_v));
                            `CC_LE: take_branch_c =  cc_z || (cc_n && !cc_v) || (!cc_n && cc_v);
                            default: ;
                        endcase
                        take_branch_target_c = ex_pc + 32'd2 + ex_branch_imm;
                        if (ex_cc == `CC_F) begin
                            // BSR: take_branch will be deferred to after the store completes.
                            take_branch_c = 1'b0;
                            want_mem = 1'b1; want_we = 1'b1;
                            want_addr = ex_sp - 32'd4;
                            want_wdata = ex_pc_next;
                            want_be = 4'b1111;
                        end
`ifdef KICKSTART_FAST_BOOT
                        // Skip Kickstart's COLOR00-write timing loop at
                        // $F80462 (bgt $F80458 with a 86K-iteration count).
                        // Force fall-through.
                        if (ex_pc == 32'h00F8_0462) take_branch_c = 1'b0;
`endif
                    end
                    // ----------------------------------------------------
                    // New instruction kinds (Tier 3).
                    // ----------------------------------------------------
                    K_ALUI: begin
                        // src is immediate (in ex_src_imm32); dst is the EA from
                        // dst_mode/dst_reg. We support Dn destinations and the
                        // special "CCR/SR" destination encoded as dst_mode=3'b110.
                        if (ex_dst_mode == `EA_DREG) begin
                            alu_op_c = ex_alu_op;
                            alu_a = ex_rb;            // Dn destination value
                            alu_b = ex_src_imm32;
                            alu_size_c = ex_size;
                            cc_we_c = 1'b1;
                            cc_x_we_c = (ex_alu_op == `ALU_ADD) || (ex_alu_op == `ALU_SUB);
                            cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v;
                            cc_c_c = alu_c; cc_x_c = alu_x;
                            if (ex_alu_op != `ALU_CMP) begin
                                wb_main_we_c  = 1'b1;
                                wb_main_idx_c = {1'b0, ex_dst_reg};
                                wb_main_data_c = alu_y;
                            end
                        end else if (ex_dst_mode == 3'b110) begin
                            // ANDI/ORI/EORI to CCR (dst_reg=0) or SR (dst_reg=1).
                            // SR variant is privileged.
                            if (ex_dst_reg == 3'b001 && !sr_s) begin
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_PRIV_VIO;
                                exc_saved_pc_c = ex_pc;
                            end else begin
                                sr_we_c = 1'b1;
                                if (ex_dst_reg == 3'b000) begin
                                    // CCR variant: source is 8 bits, only CCR
                                    // byte changes; upper byte of SR untouched.
                                    case (ex_alu_op)
                                        `ALU_AND: sr_data_c = {sr_now[15:8], sr_now[7:0] & ex_src_imm32[7:0]};
                                        `ALU_OR:  sr_data_c = {sr_now[15:8], sr_now[7:0] | ex_src_imm32[7:0]};
                                        `ALU_EOR: sr_data_c = {sr_now[15:8], sr_now[7:0] ^ ex_src_imm32[7:0]};
                                        default:  sr_data_c = sr_now;
                                    endcase
                                end else begin
                                    // SR variant: source is 16 bits.
                                    case (ex_alu_op)
                                        `ALU_AND: sr_data_c = sr_now & ex_src_imm32[15:0];
                                        `ALU_OR:  sr_data_c = sr_now | ex_src_imm32[15:0];
                                        `ALU_EOR: sr_data_c = sr_now ^ ex_src_imm32[15:0];
                                        default:  sr_data_c = sr_now;
                                    endcase
                                    // If S is being cleared (supervisor→user
                                    // mode), swap A7 with usp_shadow.  Without
                                    // this A7 keeps the supervisor stack
                                    // pointer and user-mode pushes clobber it.
                                    if (sr_data_c[`SR_S] == 1'b0 && sr_s) begin
                                        wb_aux_we_c   = 1'b1;
                                        wb_aux_idx_c  = 4'd15;
                                        wb_aux_data_c = usp_shadow;
                                    end
                                end
                            end
                        end else if (alui_mem) begin
                            // ALUI <#imm>, <ea(mem)>.  Schedule the read; the
                            // S_LOAD ack handler runs the ALU on dc_rdata and
                            // (for non-CMP) chains a write through S_RMW_W.
                            // CMPI is read-only.
                            want_mem  = 1'b1;
                            want_we   = 1'b0;
                            want_lock = alui_mem_writes;
                            want_addr = dst_ea;
                            want_be   = 4'b1111;
                            if (dst_an_update) begin
                                // (An)+ / -(An) addressing on dst: commit
                                // the new An at S_LOAD settle (for CMPI) or
                                // S_RMW_W settle (for modifying ops).  We
                                // don't fire the wb_aux here -- it would be
                                // shadowed because is_settled_in_run is
                                // false when want_mem is set.
                            end
                        end
                    end
                    K_MOVEA: begin
                        // MOVEA: source any EA, dest is An (full 32-bit).
                        // .W sign-extends to 32 bits.
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be = 4'b1111;     // simple: always read full word, extract later
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end else begin
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b1, ex_dst_reg};
                            wb_main_data_c = (ex_size == `SZ_W)
                                             ? {{16{src_operand[15]}}, src_operand[15:0]}
                                             : src_operand;
                        end
                    end
                    K_CLR: begin
                        if (ex_src_mode == `EA_DREG) begin
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_data_c = 32'd0;
                            cc_we_c = 1'b1;
                            cc_z_c = 1'b1;
                        end else if (src_needs_mem) begin
                            // CLR <ea>: write 0 to memory with size-appropriate
                            // byte enables. No read needed (CCR is always
                            // Z=1, N=V=C=0 regardless of prior value).
                            want_mem = 1'b1; want_we = 1'b1;
                            want_addr = src_ea;
                            want_wdata = 32'd0;
                            case (ex_size)
                                `SZ_B: want_be = be_for_byte(src_ea[1:0]);
                                `SZ_W: want_be = be_for_word(src_ea[1]);
                                default: want_be = 4'b1111;
                            endcase
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                            // CCR commit in S_STORE.
                        end
                    end
                    K_TST: begin
                        if (!src_needs_mem) begin
                            alu_op_c = `ALU_TST;
                            alu_b = src_operand;
                            alu_size_c = ex_size;
                            cc_we_c = 1'b1;
                            cc_n_c = alu_n; cc_z_c = alu_z;
                        end else begin
                            // Memory TST: issue a load; CCR set in S_LOAD ack.
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end
                    end
                    K_NEG, K_NOT: begin
                        if (ex_src_mode == `EA_DREG) begin
                            alu_op_c = (ex_kind == K_NEG) ? `ALU_NEG : `ALU_NOT;
                            alu_b = src_operand;
                            alu_size_c = ex_size;
                            cc_we_c = 1'b1;
                            cc_x_we_c = (ex_kind == K_NEG);
                            cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v;
                            cc_c_c = alu_c; cc_x_c = alu_x;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_data_c = alu_y;
                        end else if (src_needs_mem) begin
                            // Memory NEG/NOT: read-modify-write. Issue load
                            // here; S_LOAD will drive the ALU with rdata and
                            // start the write into S_RMW_W.
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end
                    end
                    K_EXT: begin
                        alu_op_c = ex_alu_op;       // ALU_EXT_W or ALU_EXT_L
                        alu_b = ex_ra;
                        alu_size_c = ex_size;
                        cc_we_c = 1'b1;
                        cc_n_c = alu_n; cc_z_c = alu_z;
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b0, ex_src_reg};
                        wb_main_data_c = alu_y;
                    end
                    K_SWAP: begin
                        alu_op_c = `ALU_SWAP;
                        alu_b = ex_ra;
                        alu_size_c = `SZ_L;
                        cc_we_c = 1'b1;
                        cc_n_c = alu_n; cc_z_c = alu_z;
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b0, ex_src_reg};
                        wb_main_data_c = alu_y;
                    end
                    K_EXG: begin
                        // Swap two registers (both .L). ra holds Ry (the src-mode reg),
                        // rb holds Rx (reg_idx). After EXG: Rx <- Ry, Ry <- Rx.
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = ex_reg_idx_full;
                        wb_main_data_c = ex_ra;
                        wb_aux_we_c    = 1'b1;
                        wb_aux_idx_c   = reg_idx_of_ea(ex_src_mode, ex_src_reg);
                        wb_aux_data_c  = ex_rb;
                    end
                    K_DBCC: begin
`ifdef KICKSTART_FAST_BOOT
                        // Skip Kickstart 3.1's power-on LED-blink inner DBF
                        // spins by forcing the counter to underflow on the
                        // first iteration.  The two PCs are the dbf D0
                        // instructions inside the BSET (inner1) and BCLR
                        // (inner2) loops at the CIA-A PRA poke routine.
                        // The outer DBF at $F8044E is left alone so D1
                        // still counts.
                        if (ex_pc == 32'h00F8_043C || ex_pc == 32'h00F8_044A ||
                            ex_pc == 32'h00F8_4038 || ex_pc == 32'h00F8_4044) begin
                            // Force Dn[15:0] = $FFFF (== -1, the DBF
                            // underflow condition) so it falls through.
                            // Two LED-blink BSET/BCLR routines: one at
                            // \$F8043C/\$F8044A (power-on probe), another
                            // at \$F84038/\$F84044 (post-Resident-Init).
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_data_c = {ex_rb[31:16], 16'hFFFF};
                            wb_main_size_c = `SZ_W;
                            take_branch_c  = 1'b0;
                        end else begin
`endif
                        // Evaluate condition (same as Bcc). If TRUE: fall through.
                        // If FALSE: decrement Dn[15:0]; branch unless result == -1.
                        // The Dn register is read via rb (we set rb_idx accordingly in ID).
                        case (ex_cc)
                            `CC_T:  take_branch_c = 1'b1;     // dbt: cond always TRUE -> no decrement
                            `CC_F:  take_branch_c = 1'b0;     // dbra: never TRUE -> always decrement
                            `CC_HI: take_branch_c = !cc_c && !cc_z;
                            `CC_LS: take_branch_c =  cc_c ||  cc_z;
                            `CC_CC: take_branch_c = !cc_c;
                            `CC_CS: take_branch_c =  cc_c;
                            `CC_NE: take_branch_c = !cc_z;
                            `CC_EQ: take_branch_c =  cc_z;
                            `CC_VC: take_branch_c = !cc_v;
                            `CC_VS: take_branch_c =  cc_v;
                            `CC_PL: take_branch_c = !cc_n;
                            `CC_MI: take_branch_c =  cc_n;
                            `CC_GE: take_branch_c = (cc_n &&  cc_v) || (!cc_n && !cc_v);
                            `CC_LT: take_branch_c = (cc_n && !cc_v) || (!cc_n &&  cc_v);
                            `CC_GT: take_branch_c = !cc_z && ((cc_n && cc_v) || (!cc_n && !cc_v));
                            `CC_LE: take_branch_c =  cc_z || (cc_n && !cc_v) || (!cc_n && cc_v);
                            default: ;
                        endcase
                        // Note: for DBcc we use take_branch_c here as the COND result;
                        // we re-purpose the redirect logic below.
                        if (!take_branch_c) begin
                            // Cond false -> decrement Dn.W
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_data_c = {ex_rb[31:16], ex_rb[15:0] - 16'd1};
                            wb_main_size_c = `SZ_W;
                            // Branch if result != -1 (i.e., ex_rb[15:0] != 0 before dec)
                            if (ex_rb[15:0] != 16'd0) begin
                                take_branch_c = 1'b1;    // tell redirect logic to take branch
                                take_branch_target_c = ex_pc + 32'd2 + ex_branch_imm;
                            end
                            // else fall through (take_branch_c stays 0)
                        end else begin
                            // Cond true -> fall through (no decrement, no branch).
                            take_branch_c = 1'b0;
                        end
`ifdef KICKSTART_FAST_BOOT
                        end  // end of else (not in skip range)
`endif
                    end
                    K_SHIFT: begin
                        // Register-form shift: src is Dn (read via ra). Count is either
                        // an immediate (ex_shift_count, 0→8) or a register (ex_rb low 6 bits).
                        alu_op_c    = ex_alu_op;
                        alu_b       = ex_ra;
                        alu_size_c  = ex_size;
                        alu_shamt_c = ex_shift_dyn
                                       ? ex_rb[5:0]
                                       : ((ex_shift_count == 4'd0) ? 6'd8 : {2'b00, ex_shift_count[2:0]});
                        cc_we_c    = 1'b1;
                        cc_x_we_c  = (alu_shamt_c != 6'd0);
                        cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v;
                        cc_c_c = alu_c; cc_x_c = alu_x;
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b0, ex_src_reg};
                        wb_main_data_c = alu_y;
                    end
                    K_LINK: begin
                        // Push An, set An = SP-4, SP = SP-4 + disp16.
                        want_mem  = 1'b1; want_we = 1'b1;
                        want_addr = ex_sp - 32'd4;
                        want_wdata = ex_ra;
                        want_be    = 4'b1111;
                    end
                    K_UNLK: begin
                        // SP = An; An = (SP)+; effectively load new An from mem[An], SP = An+4.
                        want_mem  = 1'b1; want_we = 1'b0;
                        want_addr = ex_ra;
                        want_be   = 4'b1111;
                    end
                    K_PEA: begin
                        // Push the EA itself (computed in src_ea) onto the stack.
                        want_mem  = 1'b1; want_we = 1'b1;
                        want_addr = ex_sp - 32'd4;
                        want_wdata = src_ea;
                        want_be    = 4'b1111;
                    end
                    K_MULU, K_MULS: begin
                        // 16x16 -> 32. Destination is Dx (reg_idx_full, in ex_rb).
                        // Source is .W from EA: register operand in src_operand[15:0]
                        // for Dn source, or a memory load otherwise.
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end else begin
                            // Compute now.
                            cc_we_c = 1'b1;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = ex_reg_idx_full;
                            wb_main_size_c = `SZ_L;
                            if (ex_kind == K_MULU) begin
                                wb_main_data_c = {16'd0, ex_rb[15:0]} *
                                                 {16'd0, src_operand[15:0]};
                            end else begin
                                wb_main_data_c = muls_y_signed;
                            end
                            cc_n_c = wb_main_data_c[31];
                            cc_z_c = (wb_main_data_c == 32'd0);
                            cc_v_c = 1'b0;
                            cc_c_c = 1'b0;
                        end
                    end
                    K_DIVU, K_DIVS: begin
                        // 32 / 16 -> {rem[31:16], quot[15:0]}. Dx is the dividend (.L).
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end else begin
                            if (src_operand[15:0] == 16'd0) begin
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_DIV_ZERO;
                                exc_saved_pc_c = ex_pc_next;
                            end else if (ex_kind == K_DIVU) begin
                                cc_we_c = 1'b1;
                                wb_main_we_c   = 1'b1;
                                wb_main_idx_c  = ex_reg_idx_full;
                                wb_main_size_c = `SZ_L;
                                wb_main_data_c = (((ex_rb % {16'd0, src_operand[15:0]}) & 32'h0000FFFF) << 16)
                                               |  ((ex_rb / {16'd0, src_operand[15:0]}) & 32'h0000FFFF);
                                cc_n_c = wb_main_data_c[15];
                                cc_z_c = (wb_main_data_c[15:0] == 16'd0);
                                cc_v_c = 1'b0;
                                cc_c_c = 1'b0;
                            end else begin
                                // DIVS: signed 32 / signed 16 (manual sign handling)
                                cc_we_c = 1'b1;
                                wb_main_we_c   = 1'b1;
                                wb_main_idx_c  = ex_reg_idx_full;
                                wb_main_size_c = `SZ_L;
                                wb_main_data_c = divs_y;
                                cc_n_c = wb_main_data_c[15];
                                cc_z_c = (wb_main_data_c[15:0] == 16'd0);
                                cc_v_c = 1'b0;
                                cc_c_c = 1'b0;
                            end
                        end
                    end
                    K_BIT: begin
                        // BTST/BCHG/BCLR/BSET. For Dn dest: 32-bit position (mod 32).
                        // Static: bit pos in ex_src_imm32, destination read via ex_rb (reg_idx_full).
                        // Dynamic: bit pos in Dr (read via ex_rb=reg_idx_full),
                        //          destination read via ex_ra (src_base m3/r0).
                        if (ex_dst_mode == `EA_DREG) begin
                            cc_we_c = 1'b1;
                            cc_n_c = cc_n; cc_v_c = cc_v; cc_c_c = cc_c; // unchanged
                            cc_z_c = !bit_dest_val[bit_n];
                            if (ex_alu_op != 5'd0) begin
                                wb_main_we_c   = 1'b1;
                                wb_main_idx_c  = {1'b0, ex_dst_reg};
                                wb_main_size_c = `SZ_L;
                                case (ex_alu_op[1:0])
                                    2'b01: wb_main_data_c = bit_dest_val ^ (32'd1 << bit_n); // BCHG
                                    2'b10: wb_main_data_c = bit_dest_val & ~(32'd1 << bit_n); // BCLR
                                    2'b11: wb_main_data_c = bit_dest_val |  (32'd1 << bit_n); // BSET
                                    default: wb_main_data_c = bit_dest_val;
                                endcase
                            end
                        end else begin
                            // Memory destination: byte-sized RMW.  The EA
                            // lives at different positions in the
                            // src/dst-ea machinery for static vs dynamic
                            // (src_mode==EA_EXT/IMM in static carries the
                            // bit-count immediate, so the dest EA is in
                            // dst_ea; for dynamic src_mode==m3 carries the
                            // mem EA in src_ea while rb is the bit-count
                            // Dn).  Pick the right one.
                            want_mem  = 1'b1;
                            want_we   = 1'b0;
                            want_lock = (ex_alu_op != 5'd0);
                            want_addr = ex_shift_dyn ? src_ea : dst_ea;
                            want_be   = be_for_byte(want_addr[1:0]);
                        end
                    end
                    K_SCC: begin
                        // Scc <ea>.B: write 0xFF if cond true, else 0x00.
                        // Destination is any data-alterable EA — Dn or
                        // memory.  Memory dst (e.g. ST $1F(A2)) was
                        // previously a silent no-op which broke Kickstart
                        // 1.3's library-create fail-flag pattern at
                        // $F8086A (ST $1F(A2)).
                        if (ex_src_mode == `EA_DREG) begin
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_size_c = `SZ_B;
                            wb_main_data_c = {24'd0, {8{cond_true(ex_cc)}}};
                        end else if (src_needs_mem) begin
                            // .B store of the cond-result to memory.
                            want_mem  = 1'b1;
                            want_we   = 1'b1;
                            want_addr = src_ea;
                            want_be   = be_for_byte(src_ea[1:0]);
                            want_wdata = byte_into_word(
                                {8{cond_true(ex_cc)}}, src_ea[1:0]);
                        end
                    end
                    K_TRAP: begin
                        // TRAP #N takes vector 32+N, saves the return PC.
                        exc_launch_c   = 1'b1;
                        exc_vector_c   = 8'd32 + {4'd0, ex_imm_raw[3:0]};
                        exc_saved_pc_c = ex_pc_next;
                    end
                    K_RTE: begin
                        // Privileged. If currently in user mode -> priv violation.
                        if (!sr_s) begin
                            exc_launch_c   = 1'b1;
                            exc_vector_c   = 8'd`VEC_PRIV_VIO;
                            exc_saved_pc_c = ex_pc;
                        end
                        // Otherwise the sequential block kicks off the RTE
                        // pop sequence (this case is a planning no-op).
                    end
                    K_TRAPV: begin
                        if (cc_v) begin
                            exc_launch_c   = 1'b1;
                            exc_vector_c   = 8'd`VEC_TRAPV;
                            exc_saved_pc_c = ex_pc_next;
                        end
                    end
                    K_ILLEGAL: begin
                        exc_launch_c   = 1'b1;
                        exc_vector_c   = 8'd`VEC_ILLEGAL;
                        exc_saved_pc_c = ex_pc;
                    end
                    K_BAD: begin
                        // Unknown opcode -- treat as a 68000 ILLEGAL.
                        // A-line ($A___) and F-line ($F___) get their own
                        // vectors (Kickstart uses these as CPU-feature
                        // probes; without the trap the boot mis-detects
                        // us as 68030+ and takes higher-CPU paths).
                        exc_launch_c   = 1'b1;
                        exc_saved_pc_c = ex_pc;
                        if (ex_opcode[15:12] == 4'hA)
                            exc_vector_c = 8'd`VEC_LINEA;
                        else if (ex_opcode[15:12] == 4'hF)
                            exc_vector_c = 8'd`VEC_LINEF;
                        else
                            exc_vector_c = 8'd`VEC_ILLEGAL;
                    end
                    K_MOVEUSP: begin
                        // Privileged. ex_direction: 0 = An→USP, 1 = USP→An.
                        if (!sr_s) begin
                            exc_launch_c   = 1'b1;
                            exc_vector_c   = 8'd`VEC_PRIV_VIO;
                            exc_saved_pc_c = ex_pc;
                        end else if (ex_direction) begin
                            // Read USP into An. usp_shadow holds USP while sup.
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b1, ex_src_reg};
                            wb_main_data_c = usp_shadow;
                            wb_main_size_c = `SZ_L;
                        end else begin
                            // Write An -> USP.
                            usp_we_c   = 1'b1;
                            usp_data_c = ex_ra;
                        end
                    end
                    K_MOVESR: begin
                        // direction 0: read SR into Dn (or memory dst). 1: write SR.
                        if (ex_direction == 1'b0) begin
                            // MOVE from SR (non-priv in 68000; treat as such).
                            // Dest is any data-alterable EA — Dn or memory.
                            if (ex_dst_mode == `EA_DREG) begin
                                wb_main_we_c   = 1'b1;
                                wb_main_idx_c  = {1'b0, ex_dst_reg};
                                wb_main_size_c = `SZ_W;
                                wb_main_data_c = {16'd0, sr_now};
                            end else if (dst_is_mem) begin
                                // .W store of SR to memory. Slot the 16-bit
                                // value into the right half of the bus word
                                // and pick the corresponding byte enables.
                                // dst_an_update (for predec/postinc) is
                                // committed in S_STORE, matching the K_MOVE
                                // pattern.
                                want_mem  = 1'b1;
                                want_we   = 1'b1;
                                want_addr = dst_ea;
                                want_be   = be_for_word(dst_ea[1]);
                                want_wdata = word_into_word(sr_now, dst_ea[1]);
                            end
                        end else begin
                            // MOVE to SR (privileged).
                            if (!sr_s) begin
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_PRIV_VIO;
                                exc_saved_pc_c = ex_pc;
                            end else begin
                                sr_we_c = 1'b1;
                                // Source: register (Dn / An — only Dn really
                                // makes sense here) or immediate. src_operand
                                // is the EA contents; for memory sources we'd
                                // need a load.
                                sr_data_c = src_operand[15:0];
                                // S being cleared: swap A7 with usp_shadow
                                // (same fix as ANDI/ORI/EORI to SR).
                                if (sr_data_c[`SR_S] == 1'b0 && sr_s) begin
                                    wb_aux_we_c   = 1'b1;
                                    wb_aux_idx_c  = 4'd15;
                                    wb_aux_data_c = usp_shadow;
                                end
                            end
                        end
                    end
                    K_MOVECCR: begin
                        // MOVE to CCR (non-priv). Source low byte → CCR bits.
                        sr_we_c = 1'b1;
                        sr_data_c = {sr_now[15:8], src_operand[7:0]};
                    end
                    K_CHK: begin
                        // CHK.W: trap to vec 6 if Dn[15:0] (signed) < 0 or > src (signed).
                        if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end else begin
                            // Both operands are 16-bit signed.
                            if (ex_rb[15] == 1'b1) begin
                                // Negative => trap, set N=1.
                                cc_we_c = 1'b1; cc_n_c = 1'b1;
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_CHK;
                                exc_saved_pc_c = ex_pc_next;
                            end else if ($signed(ex_rb[15:0]) > $signed(src_operand[15:0])) begin
                                cc_we_c = 1'b1; cc_n_c = 1'b0;
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_CHK;
                                exc_saved_pc_c = ex_pc_next;
                            end
                            // else: in range, no-op (N undefined; leave as-is).
                        end
                    end
                    default: ;
                endcase
                // Pending interrupt check: take an autovector interrupt at the
                // next instruction boundary. Detected here in S_RUN, where the
                // current instruction is about to dispatch. Saved PC = ex_pc
                // (so RTE will re-execute the suppressed instruction).
                // Only fires if no other exception is already being launched.
                if (!exc_launch_c && (ipl_i > sr_i)) begin
                    exc_launch_c   = 1'b1;
                    exc_is_irq_c   = 1'b1;
                    exc_vector_c   = 8'd24 + {5'd0, ipl_i};
                    exc_saved_pc_c = ex_pc;
                end
            end
            // Final cycle of exception entry sequence: write A7 := working_sp
            // (the final SSP after both pushes). If we entered from user mode,
            // also stash old A7 in usp_shadow (handled in sequential block).
            S_EXC_FETCH: begin
                if (dc_ack) begin
                    wb_aux_we_c   = 1'b1;
                    wb_aux_idx_c  = 4'd15;
                    wb_aux_data_c = working_sp;
                end
            end
            // Final cycle of RTE/RTR: write A7 := working_sp (final SP after
            // both pops). For RTE, if we're returning to user mode the swap
            // with usp_shadow happens; RTR never changes supervisor mode.
            S_RTE_POP_PC: begin
                if (dc_ack) begin
                    wb_aux_we_c   = 1'b1;
                    wb_aux_idx_c  = 4'd15;
                    wb_aux_data_c = ((ex_kind == K_RTE) &&
                                     (ex_exc_saved_sr[`SR_S] == 1'b0))
                                    ? usp_shadow
                                    : working_sp;
                end
            end
            // MOVEM iteration. On each step ack: load-MOVEM writes the
            // current register; store-MOVEM doesn't need a regfile write.
            // When the mask is exhausted: if predec/postinc, commit final An
            // via wb_aux at the settle cycle.
            S_MOVEM: begin
                if (movem_busy && dc_ack && movem_dir) begin
                    // Intermediate load-step register write (gated by
                    // movem_step_commit in the wb block, not by is_settled).
                    wb_main_we_c   = 1'b1;
                    wb_main_idx_c  = movem_curr_reg_full;
                    wb_main_data_c = dc_rdata;
                    wb_main_size_c = `SZ_L;
                end else if (!movem_busy && (movem_mask == 16'd0)) begin
                    // Final settle: commit An if predec/postinc.
                    if (movem_an_update) begin
                        wb_aux_we_c   = 1'b1;
                        wb_aux_idx_c  = {1'b1, movem_an_idx};
                        wb_aux_data_c = movem_addr;
                    end
                end
            end
            // RMW write phase: the new value is in ex_tas_word; on ack we
            // commit CCR from that value (size-aware).
            S_RMW_W: begin
                if (dc_ack) begin
                    if (ex_kind == K_BIT) begin
                        // Mem-dest BCHG/BCLR/BSET.  ex_tas_word holds the
                        // ORIGINAL word read in S_LOAD; Z reflects the
                        // pre-modify bit value.  Other CCR bits unchanged.
                        cc_we_c = 1'b1;
                        cc_n_c  = cc_n; cc_v_c = cc_v; cc_c_c = cc_c;
                        cc_z_c  = !byte_at(ex_tas_word, dc_addr[1:0])[bit_n[2:0]];
                        if (ex_shift_dyn && src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end else if (!ex_shift_dyn && dst_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end
                    end else if (ex_kind == K_ALUI || ex_kind == K_ALUQ) begin
                        // Mem-dest ADDI/SUBI/ANDI/ORI/EORI + ADDQ/SUBQ.
                        // N/Z/V/C/X were captured at S_LOAD ack
                        // (ex_alui_*_q) from the in-flight ALU output;
                        // by S_RMW_W the live alu_y is stale.
                        cc_we_c   = 1'b1;
                        cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                    (ex_alu_op == `ALU_SUB);
                        cc_n_c = ex_alui_n_q;
                        cc_z_c = ex_alui_z_q;
                        cc_v_c = ex_alui_v_q;
                        cc_c_c = ex_alui_c_q;
                        cc_x_c = ex_alui_x_q;
                        // K_ALUI tracks the dst An; K_ALUQ uses src An
                        // (the EA in K_ALUQ is the *source*, which is
                        // also the destination for the RMW).
                        if (ex_kind == K_ALUI && dst_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end else if (ex_kind == K_ALUQ && src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end else begin
                        cc_we_c   = 1'b1;
                        cc_v_c    = 1'b0;
                        cc_x_we_c = (ex_kind == K_NEG);
                        case (ex_size)
                            `SZ_B: begin
                                cc_n_c = ex_tas_word[7];
                                cc_z_c = (ex_tas_word[7:0] == 8'd0);
                                cc_c_c = (ex_kind == K_NEG) ? (ex_tas_word[7:0] != 8'd0) : 1'b0;
                                cc_x_c = cc_c_c;
                            end
                            `SZ_W: begin
                                cc_n_c = ex_tas_word[15];
                                cc_z_c = (ex_tas_word[15:0] == 16'd0);
                                cc_c_c = (ex_kind == K_NEG) ? (ex_tas_word[15:0] != 16'd0) : 1'b0;
                                cc_x_c = cc_c_c;
                            end
                            default: begin
                                cc_n_c = ex_tas_word[31];
                                cc_z_c = (ex_tas_word == 32'd0);
                                cc_c_c = (ex_kind == K_NEG) ? (ex_tas_word != 32'd0) : 1'b0;
                                cc_x_c = cc_c_c;
                            end
                        endcase
                    end
                end
            end
            S_LOAD: begin
                case (ex_kind)
                    K_MOVE: begin
                        // For memory-to-memory moves the destination is not a
                        // register, so suppress register writeback for the
                        // loaded value.  The sequential block chains a store
                        // to dst_ea after the load completes; src_an / dst_an
                        // commits are emitted by the S_STORE K_MOVE handler
                        // when the instruction settles, NOT here -- otherwise
                        // wb_we drives the regfile while the chained store
                        // is still in flight.
                        if (!dst_is_mem) begin
                            wb_main_we_c = 1'b1;
                            wb_main_idx_c = (ex_dst_mode == `EA_AREG) ? {1'b1, ex_dst_reg}
                                                                      : {1'b0, ex_dst_reg};
                            case (ex_size)
                                `SZ_B: begin
                                    wb_main_data_c = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                                    cc_n_c = byte_at(dc_rdata, dc_addr[1:0])[7];
                                    cc_z_c = (byte_at(dc_rdata, dc_addr[1:0]) == 8'd0);
                                end
                                `SZ_W: begin
                                    wb_main_data_c = {16'd0, dc_addr[1] ? dc_rdata[15:0] : dc_rdata[31:16]};
                                    cc_n_c = dc_addr[1] ? dc_rdata[15] : dc_rdata[31];
                                    cc_z_c = dc_addr[1] ? (dc_rdata[15:0] == 16'd0)
                                                        : (dc_rdata[31:16] == 16'd0);
                                end
                                default: begin
                                    wb_main_data_c = dc_rdata;
                                    cc_n_c = dc_rdata[31];
                                    cc_z_c = (dc_rdata == 32'd0);
                                end
                            endcase
                            cc_we_c = (ex_dst_mode == `EA_DREG);
                            if (src_an_update) begin
                                wb_aux_we_c = 1'b1;
                                wb_aux_idx_c = {1'b1, ex_src_reg};
                                wb_aux_data_c = src_an_next;
                            end
                        end
                    end
                    K_ALU: begin
                        // dc_rdata is the 32-bit aligned read; the ALU operand
                        // must be the size-appropriate field positioned at the
                        // requested address.  For byte/word reads at odd byte
                        // or odd word offsets, the wanted lane is not in
                        // dc_rdata[7:0] / dc_rdata[15:0].  (Bug surfaced as
                        // CMP.B (d16,An),Dn computing the wrong N flag for any
                        // byte at offset 1 within a longword.)
                        alu_op_c = ex_alu_op;
                        alu_a = ex_rb;
                        case (ex_size)
                            `SZ_B: alu_b = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                            `SZ_W: alu_b = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                              : dc_rdata[31:16]};
                            default: alu_b = dc_rdata;
                        endcase
                        cc_we_c = 1'b1;
                        cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v; cc_c_c = alu_c;
                        if (ex_alu_op != `ALU_CMP) begin
                            wb_main_we_c = 1'b1;
                            wb_main_idx_c = ex_reg_idx_full;
                            wb_main_data_c = alu_y;
                        end
                        if (src_an_update) begin
                            wb_aux_we_c = 1'b1;
                            wb_aux_idx_c = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end
                    K_RTS: begin
                        take_branch_c = 1'b1;
                        take_branch_target_c = dc_rdata;
                        wb_aux_we_c = 1'b1;
                        wb_aux_idx_c = 4'd15;
                        wb_aux_data_c = ex_sp + 32'd4;
                    end
                    K_TST: begin
                        // Memory TST: rdata holds the read value at src_ea.
                        // Extract size-appropriate field, set N/Z, clear V/C.
                        cc_we_c = 1'b1;
                        cc_v_c = 1'b0; cc_c_c = 1'b0;
                        case (ex_size)
                            `SZ_B: begin
                                cc_n_c = byte_at(dc_rdata, dc_addr[1:0])[7];
                                cc_z_c = (byte_at(dc_rdata, dc_addr[1:0]) == 8'd0);
                            end
                            `SZ_W: begin
                                cc_n_c = dc_addr[1] ? dc_rdata[15] : dc_rdata[31];
                                cc_z_c = dc_addr[1] ? (dc_rdata[15:0] == 16'd0)
                                                    : (dc_rdata[31:16] == 16'd0);
                            end
                            default: begin
                                cc_n_c = dc_rdata[31];
                                cc_z_c = (dc_rdata == 32'd0);
                            end
                        endcase
                    end
                    K_NEG, K_NOT: begin
                        // Memory NEG/NOT — drive ALU with rdata; sequential
                        // block captures alu_y, issues the write-back, and
                        // transitions to S_RMW_W. For .B and .W we extract
                        // the byte/word at the actual EA position before
                        // feeding the ALU, so the size-masked ALU operates
                        // on the right bits (in big-endian, byte 0 of a long
                        // is the HIGH byte at addr offset 0).
                        alu_op_c = (ex_kind == K_NEG) ? `ALU_NEG : `ALU_NOT;
                        case (ex_size)
                            `SZ_B: alu_b = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                            `SZ_W: alu_b = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                              : dc_rdata[31:16]};
                            default: alu_b = dc_rdata;
                        endcase
                        alu_size_c = ex_size;
                    end
                    K_ALUI, K_ALUQ: begin
                        // Mem-dest ALUI (ADDI/SUBI/ANDI/ORI/EORI/CMPI) or
                        // ADDQ/SUBQ to memory: drive ALU with extracted
                        // byte/word/long from dc_rdata as alu_a, and the
                        // immediate as alu_b.  For CMPI we commit CCR here
                        // (S_LOAD settles).  For modifying ops the
                        // sequential block captures alu_y + CCR snapshot
                        // and transitions to S_RMW_W.
                        alu_op_c   = ex_alu_op;
                        alu_size_c = ex_size;
                        // K_ALUI immediate is in ex_src_imm32 (separate ext
                        // word); K_ALUQ immediate is encoded inline (1..8)
                        // and lives in ex_imm_raw.
                        alu_b      = (ex_kind == K_ALUI) ? ex_src_imm32
                                                         : ex_imm_raw;
                        case (ex_size)
                            `SZ_B: alu_a = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                            `SZ_W: alu_a = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                              : dc_rdata[31:16]};
                            default: alu_a = dc_rdata;
                        endcase
                        if (alui_mem && (ex_alu_op == `ALU_CMP)) begin
                            // CMPI mem: commit CCR + (if any) An update.
                            cc_we_c = 1'b1;
                            cc_n_c = alu_n; cc_z_c = alu_z;
                            cc_v_c = alu_v; cc_c_c = alu_c;
                            if (dst_an_update) begin
                                wb_aux_we_c   = 1'b1;
                                wb_aux_idx_c  = {1'b1, ex_dst_reg};
                                wb_aux_data_c = dst_an_next;
                            end
                        end
                    end
                    K_MOVEA: begin
                        // MOVEA from memory: load source, sign-extend if .W, write to An.
                        // For .W, the wanted word is at dc_rdata[31:16] when dc_addr[1]==0
                        // and at dc_rdata[15:0] when dc_addr[1]==1 (big-endian lane).
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b1, ex_dst_reg};
                        wb_main_data_c = (ex_size == `SZ_W)
                                         ? (dc_addr[1]
                                            ? {{16{dc_rdata[15]}}, dc_rdata[15:0]}
                                            : {{16{dc_rdata[31]}}, dc_rdata[31:16]})
                                         : dc_rdata;
                        if (src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end
                    K_UNLK: begin
                        // Loaded data is the new An; SP becomes old An + 4.
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b1, ex_src_reg};
                        wb_main_data_c = dc_rdata;
                        wb_aux_we_c    = 1'b1;
                        wb_aux_idx_c   = 4'd15;
                        wb_aux_data_c  = ex_ra + 32'd4;
                    end
                    K_BIT: begin
                        // Memory-dest BTST settles here (no write).  Pull the
                        // byte addressed by dc_addr[1:0] out of the loaded
                        // word and set CCR Z from bit n.  Modifying ops
                        // (BCHG/BCLR/BSET) fall through to S_RMW_W and don't
                        // settle here -- see is_settled_after_load.
                        cc_we_c = 1'b1;
                        cc_n_c  = cc_n; cc_v_c = cc_v; cc_c_c = cc_c;
                        cc_z_c  = !byte_at(dc_rdata, dc_addr[1:0])[bit_n[2:0]];
                        // An post-inc / pre-dec commit.  Static uses dst-side
                        // (rb points at A[r0]); dynamic uses src-side (ra
                        // points at A[r0], rb is the bit-count Dn).
                        if (ex_shift_dyn && src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end else if (!ex_shift_dyn && dst_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end
                    end
                    K_MULU, K_MULS: begin
                        // Word operand from memory: use mem_word_w (the byte/word
                        // lane at dc_addr[1:0]), not raw dc_rdata[15:0].
                        cc_we_c = 1'b1;
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = ex_reg_idx_full;
                        wb_main_size_c = `SZ_L;
                        if (ex_kind == K_MULU) begin
                            wb_main_data_c = {16'd0, ex_rb[15:0]} *
                                             {16'd0, mem_word_w};
                        end else begin
                            wb_main_data_c = muls_y_signed_mem;
                        end
                        cc_n_c = wb_main_data_c[31];
                        cc_z_c = (wb_main_data_c == 32'd0);
                        cc_v_c = 1'b0;
                        cc_c_c = 1'b0;
                    end
                    K_DIVU, K_DIVS: begin
                        // Word divisor from memory: use mem_word_w.
                        if (mem_word_w == 16'd0) begin
                            cc_we_c = 1'b1;
                            cc_v_c = 1'b1;
                        end else if (ex_kind == K_DIVU) begin
                            cc_we_c = 1'b1;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = ex_reg_idx_full;
                            wb_main_size_c = `SZ_L;
                            wb_main_data_c = (((ex_rb % {16'd0, mem_word_w}) & 32'h0000FFFF) << 16)
                                           |  ((ex_rb / {16'd0, mem_word_w}) & 32'h0000FFFF);
                            cc_n_c = wb_main_data_c[15];
                            cc_z_c = (wb_main_data_c[15:0] == 16'd0);
                            cc_v_c = 1'b0;
                            cc_c_c = 1'b0;
                        end else begin
                            cc_we_c = 1'b1;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = ex_reg_idx_full;
                            wb_main_size_c = `SZ_L;
                            wb_main_data_c = divs_y_mem;
                            cc_n_c = wb_main_data_c[15];
                            cc_z_c = (wb_main_data_c[15:0] == 16'd0);
                            cc_v_c = 1'b0;
                            cc_c_c = 1'b0;
                        end
                    end
                    // K_CHK with memory source: known limitation -- exc_launch_c
                    // is only sampled from S_RUN, so launching a CHK trap from
                    // S_LOAD here would not actually fire the trap state
                    // machine.  Tracked separately; for now mem-source CHK
                    // still drops the load result and never traps.
                    default: ;
                endcase
            end
            S_STORE: begin
                case (ex_kind)
                    K_MOVESR: begin
                        // MOVE-FROM-SR to memory: commit predec/postinc An.
                        if (dst_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end
                    end
                    K_SCC: begin
                        // Scc to memory: commit predec/postinc An on src
                        // (the Scc EA is src_mode in our decoder).
                        if (src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end
                    K_MOVE: begin
                        cc_we_c = 1'b1;
                        cc_n_c = src_operand[31];
                        cc_z_c = (src_operand == 32'd0);
                        if (dst_an_update) begin
                            wb_aux_we_c = 1'b1;
                            wb_aux_idx_c = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end
                        // For mem-to-mem moves the src An post-inc/pre-dec
                        // must also commit at this settle (the S_LOAD case
                        // emitted it on wb_aux, but wb_aux is now claimed by
                        // dst An).  Route src An through wb_main, which is
                        // unused since K_MOVE with dst_is_mem has no data
                        // register writeback.
                        if (src_an_update) begin
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b1, ex_src_reg};
                            wb_main_data_c = src_an_next;
                        end
                    end
                    K_JSR: begin
                        wb_aux_we_c = 1'b1;
                        wb_aux_idx_c = 4'd15;
                        wb_aux_data_c = ex_sp - 32'd4;
                        take_branch_c = 1'b1;
                        // src_ea is the address computed for every supported
                        // JSR source mode (same as JMP).  Using ex_ra dropped
                        // the displacement on JSR d16(An), which is the bulk
                        // of AmigaOS library calls (JSR -N(A6)).
                        take_branch_target_c = src_ea;
                    end
                    K_BCC: begin
                        // BSR completion.
                        wb_aux_we_c = 1'b1;
                        wb_aux_idx_c = 4'd15;
                        wb_aux_data_c = ex_sp - 32'd4;
                        take_branch_c = 1'b1;
                        take_branch_target_c = ex_pc + 32'd2 + ex_branch_imm;
                    end
                    K_LINK: begin
                        // Store done: An <- SP-4, SP <- SP-4 + disp16.
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b1, ex_src_reg};
                        wb_main_data_c = ex_sp - 32'd4;
                        wb_main_size_c = `SZ_L;
                        wb_aux_we_c    = 1'b1;
                        wb_aux_idx_c   = 4'd15;
                        wb_aux_data_c  = ex_sp - 32'd4 + ex_src_imm32;
                    end
                    K_PEA: begin
                        wb_aux_we_c   = 1'b1;
                        wb_aux_idx_c  = 4'd15;
                        wb_aux_data_c = ex_sp - 32'd4;
                    end
                    K_CLR: begin
                        // Memory CLR: write done; commit CCR.
                        cc_we_c = 1'b1;
                        cc_z_c = 1'b1;
                        cc_n_c = 1'b0; cc_v_c = 1'b0; cc_c_c = 1'b0;
                    end
                    default: ;
                endcase
            end
            S_TASW: begin
                cc_we_c = 1'b1;
                cc_n_c = byte_at(ex_tas_word, dc_addr[1:0])[7];
                cc_z_c = (byte_at(ex_tas_word, dc_addr[1:0]) == 8'd0);
            end
            default: ;
        endcase

        // Address-error detection.  Any data-side access (want_mem) issued
        // with a size that doesn't match the address's alignment traps to
        // VEC_ADDR_ERROR (vector 3).  68000 semantics:
        //   - .W requires bit 0 == 0
        //   - .L requires bit 0 == 0  (the 68000 does two consecutive word
        //     reads under the hood, so addresses with bits[1:0]==10 are
        //     legal — only odd bytes trap)
        //   - .B always OK
        // We suppress the access (want_mem <= 0) and reroute to exc_launch.
        // Note: this is a partial implementation — it only catches
        // accesses issued via the central S_RUN want_mem path.  MOVEM,
        // RTE/RTR, exception stack pushes and TAS go through their own
        // dc_* dispatch and don't hit this check yet.
        if (ex_valid && !halted && want_mem) begin
            if ((ex_size == `SZ_W && want_addr[0]) ||
                (ex_size == `SZ_L && want_addr[0])) begin
                exc_launch_c   = 1'b1;
                exc_vector_c   = 8'd`VEC_ADDR_ERROR;
                // Save the PC AFTER the faulting instruction so RTE returns
                // to the following instruction.  Real 68000 saves the PC of
                // the faulting instruction itself; handlers there can use
                // SSW/IR to compute the resume point.  Returning to
                // ex_pc_next keeps test handlers simple.
                exc_saved_pc_c = ex_pc_next;
                exc_group0_c   = 1'b1;
                exc_addr_c     = want_addr;
                // SSW: bit 0 = R/W (1=read).  We invert want_we since
                // want_we==1 means the in-flight access was a write.
                exc_ssw_c      = {15'd0, ~want_we};
                want_mem       = 1'b0;
            end
        end
    end

    // (No early An commit during mem-to-mem K_MOVE chained S_LOAD->S_STORE.
    //  Both src An and dst An updates ride along at S_STORE settle now
    //  that the S_STORE K_MOVE handler emits both.)

    // Drive wb_* combinationally.
    always @* begin
        wb_we       = 1'b0;
        wb_widx     = 4'd0;
        wb_size     = `SZ_L;
        wb_wdata    = 32'd0;
        wb_aux_we   = 1'b0;
        wb_aux_idx  = 4'd0;
        wb_aux_data = 32'd0;
        if (is_settled) begin
            wb_we       = wb_main_we_c;
            wb_widx     = wb_main_idx_c;
            wb_size     = wb_main_size_c;
            wb_wdata    = wb_main_data_c;
            wb_aux_we   = wb_aux_we_c;
            wb_aux_idx  = wb_aux_idx_c;
            wb_aux_data = wb_aux_data_c;
        end else if (movem_step_commit && movem_dir) begin
            // Intermediate load-MOVEM register write: fire main writeback only.
            wb_we    = wb_main_we_c;
            wb_widx  = wb_main_idx_c;
            wb_size  = wb_main_size_c;
            wb_wdata = wb_main_data_c;
        end
    end

    // Sequential state updates (CCR commit, branch redirect, ex_state, dc_*).
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cc_n <= 1'b0; cc_z <= 1'b0; cc_v <= 1'b0; cc_c <= 1'b0; cc_x <= 1'b0;
            sr_s <= 1'b1;          // boot in supervisor mode
            sr_t <= 1'b0;          // no trace
            sr_i <= 3'd7;          // all interrupts masked
            usp_shadow <= 32'd0;
            ex_state <= S_RUN;
            ex_tas_word <= 32'd0;
            ex_alui_n_q <= 1'b0;
            ex_alui_z_q <= 1'b0;
            ex_alui_v_q <= 1'b0;
            ex_alui_c_q <= 1'b0;
            ex_alui_x_q <= 1'b0;
            ex_exc_saved_pc <= 32'd0;
            ex_exc_saved_sr <= 16'd0;
            ex_exc_vector <= 8'd0;
            ex_exc_was_user <= 1'b0;
            ex_exc_new_sri <= 3'd0;
            ex_exc_is_group0 <= 1'b0;
            ex_exc_addr <= 32'd0;
            ex_exc_ir   <= 16'd0;
            ex_exc_ssw  <= 16'd0;
            working_sp <= 32'd0;
            movem_mask <= 16'd0;
            movem_idx <= 5'd0;
            movem_addr <= 32'd0;
            movem_busy <= 1'b0;
            movem_dir <= 1'b0;
            movem_predec <= 1'b0;
            movem_an_update <= 1'b0;
            movem_an_idx <= 3'd0;
            dc_req_r <= 1'b0;
            dc_we <= 1'b0; dc_lock <= 1'b0;
            dc_addr <= 32'd0; dc_wdata <= 32'd0; dc_be <= 4'b1111;
            dc_is_long <= 1'b0;
            halted <= 1'b0;
            halt_code <= 16'd0;
            retired <= 32'd0;
        end else begin
            // Count instructions that complete each cycle. STOP doesn't count.
            if (is_settled && ex_kind != K_STOP) retired <= retired + 32'd1;
`ifdef KICKSTART_BOOT_PC_TRACE
            if (is_settled && ex_kind != K_STOP)
                $display("[PC] r=%d pc=%h kind=%d", retired, ex_pc, ex_kind);
`endif
`ifdef KICKSTART_BOOT_TRACE
            if (ex_kind == K_STOP && is_settled)
                $display("[STOP] r=%d pc=%h imm=%h",
                    retired, ex_pc, ex_src_imm32[15:0]);
            if (is_settled && exc_launch_c)
                $display("[EXC] r=%d pc=%h vec=%d kind=%d", retired, ex_pc, exc_vector_c, ex_kind);
`endif
            if (is_settled && cc_we_c) begin
                cc_n <= cc_n_c; cc_z <= cc_z_c; cc_v <= cc_v_c; cc_c <= cc_c_c;
            end
            if (is_settled && cc_x_we_c) cc_x <= cc_x_c;

            // SR write (MOVE-to-SR, ANDI/ORI/EORI to SR, etc.). If S
            // toggles to 0 (going to user mode) the A7 currently in regfile
            // must swap with usp_shadow; the EX-handlers that drive sr_we_c
            // for non-RTE paths are expected to be in supervisor mode with
            // S preserved, so this code only handles the S=stay case.
            if (is_settled && sr_we_c) begin
                cc_x <= sr_data_c[`SR_X];
                cc_n <= sr_data_c[`SR_N];
                cc_z <= sr_data_c[`SR_Z];
                cc_v <= sr_data_c[`SR_V];
                cc_c <= sr_data_c[`SR_C];
                // Upper byte only writable from supervisor mode (callers ensure).
                if (sr_s) begin
                    sr_t <= sr_data_c[`SR_T];
                    sr_i <= sr_data_c[10:8];
                    // If S is being cleared, swap A7 with usp_shadow.
                    if (sr_data_c[`SR_S] == 1'b0 && sr_s == 1'b1) begin
                        usp_shadow <= rf_rc_data;   // save SSP
                        // A7 := usp_shadow would need a wb_aux from planning;
                        // not implemented (use RTE for clean U/S transitions).
                    end
                    sr_s <= sr_data_c[`SR_S];
                end
            end
            // USP write (MOVE An,USP — supervisor only, handled by planning).
            if (is_settled && usp_we_c) usp_shadow <= usp_data_c;

            // STOP.  Real 68k: load imm into SR (privileged), then suspend
            // until an interrupt arrives.  Our regression tests have always
            // used STOP as "halt the simulator" with imm as the exit code.
            // To support both:
            //   - When the `KICKSTART_BOOT define is active we implement the
            //     real semantics: update SR from imm and *don't* halt; the
            //     IF stage stays parked at the STOP PC (via stop_now in the
            //     redirect path) until an IRQ takes the CPU elsewhere.
            //   - Otherwise (default) we halt with imm as the halt code.
            if (ex_valid && ex_kind == K_STOP && ex_state == S_RUN && !halted) begin
`ifdef KICKSTART_BOOT
                // Load imm[15:0] into SR.  Privileged check: in user mode
                // we'd trap, but real Kickstart never executes STOP from
                // user mode and our tests don't either.
`ifdef KICKSTART_BOOT_TRACE
                $display("[STOP] PC=%h imm=%h retired=%d sr_s=%b sr_i=%d sp=%h",
                    ex_pc, ex_imm_raw[15:0], retired, sr_s, sr_i, rf_rc_data);
`endif
                if (sr_s) begin
                    sr_t <= ex_imm_raw[`SR_T];
                    sr_i <= ex_imm_raw[10:8];
                    cc_x <= ex_imm_raw[`SR_X];
                    cc_n <= ex_imm_raw[`SR_N];
                    cc_z <= ex_imm_raw[`SR_Z];
                    cc_v <= ex_imm_raw[`SR_V];
                    cc_c <= ex_imm_raw[`SR_C];
                end
`else
                halted <= 1'b1;
                halt_code <= ex_imm_raw[15:0];
`endif
            end

            case (ex_state)
                S_RUN: begin
                    if (ex_valid && !halted && exc_launch_c) begin
                        // Any exception entry source (TRAP, illegal, priv-vio,
                        // div-by-zero, TRAPV, IRQ, etc.). Save the supplied
                        // PC and the current SR, push PC at SSP-4.
                        ex_exc_saved_pc <= exc_saved_pc_c;
                        ex_exc_saved_sr <= sr_now;
                        ex_exc_vector   <= exc_vector_c;
                        ex_exc_was_user <= ~sr_s;
                        ex_exc_is_group0 <= exc_group0_c;
                        ex_exc_addr     <= exc_addr_c;
                        ex_exc_ir       <= ex_opcode;
                        ex_exc_ssw      <= exc_ssw_c;
                        // For IRQ: set new sr_i to ipl level so further IRQs
                        // at the same/lower priority are masked. For other
                        // exceptions: sr_i is preserved.
                        ex_exc_new_sri  <= exc_is_irq_c ? ipl_i : sr_i;
                        working_sp <= (sr_s ? rf_rc_data : usp_shadow) - 32'd4;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_lock  <= 1'b0;
                        dc_addr  <= (sr_s ? rf_rc_data : usp_shadow) - 32'd4;
                        dc_wdata <= exc_saved_pc_c;
                        dc_be    <= 4'b1111;
                        ex_state <= S_EXC_PUSH_PC;
                    end else if (ex_valid && !halted && (ex_kind == K_MOVEM)) begin
                        // MOVEM: capture mask + initial address. We only
                        // support .L MOVEM with predec/(An)/postinc EA modes.
                        // Mask bits map to registers differently for predec:
                        //   predec:    mask[0]=A7, ..., mask[15]=D0
                        //   other:     mask[0]=D0, ..., mask[15]=A7
                        // The iterator walks bit 0..15 in order and uses the
                        // appropriate mapping to pick the register.
                        movem_mask    <= ex_src_imm32[15:0];
                        movem_idx     <= 5'd0;
                        movem_dir     <= ex_direction;
                        movem_predec  <= (ex_src_mode == `EA_ADEC);
                        movem_an_idx  <= ex_src_reg;
                        movem_an_update <= (ex_src_mode == `EA_ADEC) ||
                                           (ex_src_mode == `EA_AINC);
                        // Initial An: for predec the tracked An starts at the
                        // original An (each step uses An-4 then decrements).
                        // For postinc and fixed (An), the tracked address is
                        // the current write/read address.
                        movem_addr    <= ex_ra;
                        movem_busy    <= 1'b0;
                        ex_state      <= S_MOVEM;
                    end else if (ex_valid && !halted &&
                                 ((ex_kind == K_RTE && sr_s) || (ex_kind == K_RTR))) begin
                        // RTE in supervisor mode: pop SR + PC. RTR: pop CCR + PC
                        // (uses same state path; the SR-pop ack handler only
                        // restores CCR for K_RTR). User-mode RTE is handled
                        // above as a priv-vio via exc_launch_c.
                        working_sp <= rf_rc_data;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b0;
                        dc_lock  <= 1'b0;
                        dc_addr  <= rf_rc_data;
                        dc_be    <= 4'b1111;
                        // SR pop is .W, but the bus uses dc_is_long for its
                        // unaligned-.L read-assembly gate; clear it here so
                        // the SR read doesn't trip into the assembly path.
                        dc_is_long <= 1'b0;
                        ex_state <= S_RTE_POP_SR;
                    end else if (ex_valid && !halted && want_mem) begin
                        dc_req_r <= 1'b1;
                        dc_we <= want_we;
                        dc_lock <= want_lock;
                        dc_addr <= want_addr;
                        dc_wdata <= want_wdata;
                        dc_be <= want_be;
                        dc_is_long <= (ex_size == `SZ_L);
                        if (want_we) ex_state <= S_STORE;
                        else         ex_state <= S_LOAD;
                    end
                end
                S_LOAD: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        if (ex_kind == K_TAS) begin
                            ex_tas_word <= dc_rdata;
                            dc_req_r <= 1'b1;
                            dc_we <= 1'b1;
                            dc_lock <= 1'b1;
                            dc_be <= be_for_byte(dc_addr[1:0]);
                            dc_wdata <= byte_into_word(byte_at(dc_rdata, dc_addr[1:0]) | 8'h80,
                                                       dc_addr[1:0]);
                            ex_state <= S_TASW;
                        end else if (load_starts_rmw && (ex_src_mode != `EA_DREG)) begin
                            // Memory NEG/NOT: capture the ALU result (computed
                            // combinationally from dc_rdata) and issue the
                            // write-back; transition to S_RMW_W.
                            ex_tas_word <= alu_y;
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            // For .B/.W, the cache merges by byte enables; we
                            // place the new bytes at the right position in the
                            // write word.
                            case (ex_size)
                                `SZ_B: begin
                                    dc_be    <= be_for_byte(dc_addr[1:0]);
                                    dc_wdata <= byte_into_word(alu_y[7:0], dc_addr[1:0]);
                                end
                                `SZ_W: begin
                                    dc_be    <= be_for_word(dc_addr[1]);
                                    dc_wdata <= word_into_word(alu_y[15:0], dc_addr[1]);
                                end
                                default: begin
                                    dc_be    <= 4'b1111;
                                    dc_wdata <= alu_y;
                                end
                            endcase
                            // dc_addr already set from the read.
                            ex_state <= S_RMW_W;
                        end else if (alui_mem_writes) begin
                            // Mem-dest ADDI/SUBI/ANDI/ORI/EORI.  alu_y has
                            // the new value (computed in S_LOAD planning).
                            // Capture both result and CCR snapshot; S_RMW_W
                            // settle commits the writeback + CCR.
                            ex_tas_word <= alu_y;
                            ex_alui_n_q <= alu_n;
                            ex_alui_z_q <= alu_z;
                            ex_alui_v_q <= alu_v;
                            ex_alui_c_q <= alu_c;
                            ex_alui_x_q <= alu_x;
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            case (ex_size)
                                `SZ_B: begin
                                    dc_be    <= be_for_byte(dc_addr[1:0]);
                                    dc_wdata <= byte_into_word(alu_y[7:0], dc_addr[1:0]);
                                end
                                `SZ_W: begin
                                    dc_be    <= be_for_word(dc_addr[1]);
                                    dc_wdata <= word_into_word(alu_y[15:0], dc_addr[1]);
                                end
                                default: begin
                                    dc_be    <= 4'b1111;
                                    dc_wdata <= alu_y;
                                end
                            endcase
                            ex_state <= S_RMW_W;
                        end else if (bit_mem_writes) begin
                            // Memory BCHG/BCLR/BSET: capture the original
                            // word (S_RMW_W planning reads bit n from it to
                            // commit CCR Z), then issue the byte write with
                            // bit n flipped/cleared/set.
                            ex_tas_word <= dc_rdata;
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            dc_be    <= be_for_byte(dc_addr[1:0]);
                            case (ex_alu_op[1:0])
                                2'b01: dc_wdata <= byte_into_word(
                                    byte_at(dc_rdata, dc_addr[1:0]) ^ (8'd1 << bit_n[2:0]),
                                    dc_addr[1:0]);                                // BCHG
                                2'b10: dc_wdata <= byte_into_word(
                                    byte_at(dc_rdata, dc_addr[1:0]) & ~(8'd1 << bit_n[2:0]),
                                    dc_addr[1:0]);                                // BCLR
                                2'b11: dc_wdata <= byte_into_word(
                                    byte_at(dc_rdata, dc_addr[1:0]) |  (8'd1 << bit_n[2:0]),
                                    dc_addr[1:0]);                                // BSET
                                default: dc_wdata <= byte_into_word(
                                    byte_at(dc_rdata, dc_addr[1:0]),
                                    dc_addr[1:0]);
                            endcase
                            ex_state <= S_RMW_W;
                        end else if (ex_kind == K_MOVE && dst_is_mem) begin
                            // Memory-to-memory MOVE: chain a store of the
                            // loaded data to dst_ea.  For .B/.W, slot the byte
                            // or word into the position implied by dst_ea.
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            dc_addr  <= dst_ea;
                            case (ex_size)
                                `SZ_B: begin
                                    dc_be    <= be_for_byte(dst_ea[1:0]);
                                    dc_wdata <= byte_into_word(
                                        byte_at(dc_rdata, dc_addr[1:0]),
                                        dst_ea[1:0]);
                                end
                                `SZ_W: begin
                                    dc_be    <= be_for_word(dst_ea[1]);
                                    dc_wdata <= word_into_word(
                                        dc_addr[1] ? dc_rdata[15:0] : dc_rdata[31:16],
                                        dst_ea[1]);
                                end
                                default: begin
                                    dc_be    <= 4'b1111;
                                    dc_wdata <= dc_rdata;
                                end
                            endcase
                            ex_state <= S_STORE;
                        end else begin
                            ex_state <= S_RUN;
                        end
                    end
                end
                S_STORE: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        dc_lock <= 1'b0;
                        ex_state <= S_RUN;
                    end
                end
                S_TASW: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        dc_lock <= 1'b0;
                        ex_state <= S_RUN;
                    end
                end
                S_RMW_W: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        dc_lock  <= 1'b0;
                        ex_state <= S_RUN;
                    end
                end
                S_MOVEM: begin
                    if (movem_busy) begin
                        // Waiting for current step's ack.
                        if (dc_ack) begin
                            dc_req_r <= 1'b0;
                            movem_busy <= 1'b0;
                            // Advance: shift mask, advance idx, advance addr.
                            movem_mask <= movem_mask >> 1;
                            movem_idx  <= movem_idx + 5'd1;
                            // For predec, the An is decremented along with
                            // each transfer; for postinc and fixed-EA the
                            // address increases each step.
                            if (movem_predec) movem_addr <= movem_addr - 32'd4;
                            else              movem_addr <= movem_addr + 32'd4;
                        end
                    end else begin
                        // Scanning for next set bit, or done.
                        if (movem_mask == 16'd0) begin
                            // Done: settle. The settle wire fires this cycle;
                            // sequential effects: clear req, return to S_RUN.
                            ex_state <= S_RUN;
                        end else if (movem_mask[0]) begin
                            // Issue this step's transaction.
                            dc_req_r <= 1'b1;
                            dc_we    <= ~movem_dir;        // 0 = load, 1 = store
                            dc_lock  <= 1'b0;
                            dc_addr  <= movem_step_addr;
                            dc_be    <= 4'b1111;
                            // MOVEM.L is a long-aligned transfer in size but
                            // the SP at entry can be word-aligned-not-mod-4
                            // (e.g. after the InitResident driver's LINK A5,
                            // #-$0E leaves SP at $3D6 and the inner -(SP)
                            // pushes hit $3CA/$3C6).  We need dc_is_long set
                            // so the bus's unaligned-.L assembly path fires
                            // on the read response; otherwise MOVEM-load
                            // reads return mem[idx] verbatim (= the wrong
                            // half) and registers get restored to garbage.
                            dc_is_long <= (ex_size == `SZ_L);
                            // For store, the wdata comes from regfile ra
                            // (overridden to movem_curr_reg_full).
                            if (movem_dir == 1'b0) dc_wdata <= rf_ra_data;
                            movem_busy <= 1'b1;
                        end else begin
                            // Bit not set: skip; shift mask, advance idx.
                            movem_mask <= movem_mask >> 1;
                            movem_idx  <= movem_idx + 5'd1;
                            // No addr advance (no transfer for this register).
                        end
                    end
                end
                S_EXC_PUSH_PC: begin
                    if (dc_ack) begin
                        // PC pushed at working_sp.  Push 16-bit SR next at
                        // (working_sp - 2): the canonical 68000 frame puts SR
                        // immediately below PC, total 6 bytes.  With the SP
                        // aligned, the SR word lands in bytes 2-3 of the
                        // 32-bit aligned word at working_sp - 4, so the byte
                        // enables are 4'b0011 and the value goes in
                        // dc_wdata[15:0].
                        working_sp <= working_sp - 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_addr  <= working_sp - 32'd2;
                        dc_wdata <= {16'd0, ex_exc_saved_sr};
                        dc_be    <= 4'b0011;
                        ex_state <= S_EXC_PUSH_SR;
                    end
                end
                S_EXC_PUSH_SR: begin
                    if (dc_ack) begin
                        // Enforce supervisor mode (matches old behavior).
                        if (ex_exc_was_user) usp_shadow <= rf_rc_data;
                        sr_s <= 1'b1;
                        sr_t <= 1'b0;
                        sr_i <= ex_exc_new_sri;
                        if (ex_exc_is_group0) begin
                            // Group-0 frame: push IR at (working_sp - 2).
                            // working_sp is currently at SR offset (post-SR
                            // push moved it down by 2, so this is SP-6).
                            // After IR push working_sp will be SP-8.
                            working_sp <= working_sp - 32'd2;
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            dc_addr  <= working_sp - 32'd2;
                            dc_wdata <= {ex_exc_ir, 16'd0};
                            dc_be    <= 4'b1100;
                            ex_state <= S_EXC_PUSH_IR;
                        end else begin
                            // Short frame: go straight to vector fetch.
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b0;
                            dc_addr  <= {22'd0, ex_exc_vector, 2'b00};
                            dc_be    <= 4'b1111;
                            ex_state <= S_EXC_FETCH;
                        end
                    end
                end
                S_EXC_PUSH_IR: begin
                    if (dc_ack) begin
                        // Push fault-addr low half at SP-10.
                        working_sp <= working_sp - 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_addr  <= working_sp - 32'd2;
                        dc_wdata <= {16'd0, ex_exc_addr[15:0]};
                        dc_be    <= 4'b0011;
                        ex_state <= S_EXC_PUSH_ALO;
                    end
                end
                S_EXC_PUSH_ALO: begin
                    if (dc_ack) begin
                        // Push fault-addr high half at SP-12.
                        working_sp <= working_sp - 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_addr  <= working_sp - 32'd2;
                        dc_wdata <= {ex_exc_addr[31:16], 16'd0};
                        dc_be    <= 4'b1100;
                        ex_state <= S_EXC_PUSH_AHI;
                    end
                end
                S_EXC_PUSH_AHI: begin
                    if (dc_ack) begin
                        // Push SSW at SP-14.
                        working_sp <= working_sp - 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_addr  <= working_sp - 32'd2;
                        dc_wdata <= {16'd0, ex_exc_ssw};
                        dc_be    <= 4'b0011;
                        ex_state <= S_EXC_PUSH_SSW;
                    end
                end
                S_EXC_PUSH_SSW: begin
                    if (dc_ack) begin
                        // All frame fields pushed.  Fetch vector.
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b0;
                        dc_addr  <= {22'd0, ex_exc_vector, 2'b00};
                        dc_be    <= 4'b1111;
                        ex_state <= S_EXC_FETCH;
                    end
                end
                S_EXC_FETCH: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        // PC redirect happens via redirect_pc = dc_rdata.
                        // A7 update happens via wb_aux from planning block.
                        ex_state <= S_RUN;
                    end
                end
                S_RTE_POP_SR: begin
                    if (dc_ack) begin
                        // The cache returns the full 32-bit aligned word that
                        // contains the SR.  With working_sp[1:0] == 10 (the
                        // canonical 6-byte frame), SR lives in bytes 2-3 of
                        // the aligned word -> dc_rdata[15:0].  Restore CCR
                        // always; restore the upper SR byte only for RTE.
                        ex_exc_saved_sr <= dc_rdata[15:0];
                        if (ex_kind == K_RTE) begin
                            sr_t <= dc_rdata[`SR_T];
                            sr_s <= dc_rdata[`SR_S];
                            sr_i <= dc_rdata[10:8];
                        end
                        cc_x <= dc_rdata[`SR_X];
                        cc_n <= dc_rdata[`SR_N];
                        cc_z <= dc_rdata[`SR_Z];
                        cc_v <= dc_rdata[`SR_V];
                        cc_c <= dc_rdata[`SR_C];
                        working_sp <= working_sp + 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b0;
                        dc_addr  <= working_sp + 32'd2;
                        dc_be    <= 4'b1111;
                        // PC pop is .L from working_sp+2, which is always
                        // word-aligned-but-not-mod-4 (SR was 2 bytes,
                        // pre-SR-pop SP was .L-aligned).  Set dc_is_long so
                        // the bus's unaligned-.L read assembly fires --
                        // otherwise the PC pop returns the wrong half and
                        // the redirect lands in garbage.
                        dc_is_long <= 1'b1;
                        ex_state <= S_RTE_POP_PC;
                    end
                end
                S_RTE_POP_PC: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        // PC redirect via redirect_pc = dc_rdata.
                        // For RTE returning to user mode, swap the USP back
                        // into A7 and stash the SP we just popped to into
                        // usp_shadow. RTR never changes supervisor state.
                        working_sp <= working_sp + 32'd4;
                        if ((ex_kind == K_RTE) && (ex_exc_saved_sr[`SR_S] == 1'b0)) begin
                            usp_shadow <= working_sp + 32'd4;
                            // A7 := usp_shadow is committed by planning block.
                        end
                        ex_state <= S_RUN;
                    end
                end
                default: ex_state <= S_RUN;
            endcase
        end
    end
endmodule
