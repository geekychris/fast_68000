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

    // Decoder kinds (mirrored from m68k_decoder.v).
    localparam K_BAD   = 4'd0;
    localparam K_NOP   = 4'd1;
    localparam K_RTS   = 4'd2;
    localparam K_STOP  = 4'd3;
    localparam K_JMP   = 4'd4;
    localparam K_JSR   = 4'd5;
    localparam K_LEA   = 4'd6;
    localparam K_MOVEQ = 4'd7;
    localparam K_MOVE  = 4'd8;
    localparam K_ALUQ  = 4'd9;
    localparam K_ALU   = 4'd10;
    localparam K_TAS   = 4'd11;
    localparam K_BCC   = 4'd12;

    // EX state.
    localparam S_RUN   = 2'd0;
    localparam S_LOAD  = 2'd1;
    localparam S_STORE = 2'd2;
    localparam S_TASW  = 2'd3;

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
    function [3:0] reg_idx_of_ea;
        input [2:0] m;
        input [2:0] r;
        begin
            case (m)
                `EA_DREG: reg_idx_of_ea = {1'b0, r};
                `EA_AREG: reg_idx_of_ea = {1'b1, r};
                `EA_AIND, `EA_AINC, `EA_ADEC: reg_idx_of_ea = {1'b1, r};
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

    wire [3:0]  dec_kind;
    wire [3:0]  dec_alu_op;
    wire [2:0]  dec_src_mode, dec_src_reg, dec_dst_mode, dec_dst_reg, dec_reg_idx;
    wire        dec_reg_is_a;
    wire [31:0] dec_imm;
    wire [3:0]  dec_cc;
    wire [1:0]  dec_src_ext_words, dec_dst_ext_words;
    wire        dec_needs_disp_word;

    m68k_decoder u_dec_if (
        .opcode        (effective_op),
        .kind          (dec_kind),
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
        .needs_disp_word(dec_needs_disp_word)
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
    wire [3:0] idec_kind;
    wire [3:0] idec_alu_op;
    wire [2:0] idec_src_mode, idec_src_reg, idec_dst_mode, idec_dst_reg, idec_reg_idx;
    wire       idec_reg_is_a;
    wire [31:0] idec_imm;
    wire [3:0]  idec_cc;
    wire [1:0]  idec_src_ext_words, idec_dst_ext_words;
    wire        idec_needs_disp_word;

    m68k_decoder u_dec_id (
        .opcode        (id_op),
        .kind          (idec_kind),
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
        .needs_disp_word(idec_needs_disp_word)
    );

    wire [31:0] id_branch_imm = idec_needs_disp_word
        ? {{16{id_ext[0][15]}}, id_ext[0]}
        : idec_imm;

    wire [31:0] id_src_imm32 = (idec_src_ext_words == 2'd1)
        ? {{16{id_ext[0][15]}}, id_ext[0]}
        : {id_ext[0], id_ext[1]};

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
            K_ALU, K_LEA, K_MOVEQ: id_rb_idx = id_reg_idx_full;
            K_MOVE:                id_rb_idx = id_dst_base_idx;
            default:               id_rb_idx = 4'd0;
        endcase
    end
    wire [3:0] id_rc_idx = 4'd15;  // A7 / SP always available on third port

    wire [31:0] rf_ra_data, rf_rb_data, rf_rc_data;

    // Writeback signals (driven combinationally by EX stage below).
    reg         wb_we;
    reg  [3:0]  wb_widx;
    reg  [31:0] wb_wdata;
    reg         wb_aux_we;
    reg  [3:0]  wb_aux_idx;
    reg  [31:0] wb_aux_data;

    m68k_regfile u_rf (
        .clk      (clk),
        .rst_n    (rst_n),
        .reset_a7 (reset_a7),
        .ra_idx   (id_ra_idx),
        .ra_data  (rf_ra_data),
        .rb_idx   (id_rb_idx),
        .rb_data  (rf_rb_data),
        .rc_idx   (id_rc_idx),
        .rc_data  (rf_rc_data),
        .we       (wb_we),
        .w_idx    (wb_widx),
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
    reg  [3:0]  ex_kind;
    reg  [3:0]  ex_alu_op;
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
        end else if (redirect_valid) begin
            ex_valid <= 1'b0;
        end else if (!stall) begin
            ex_valid       <= id_valid;
            ex_pc          <= id_pc;
            ex_pc_next     <= id_pc_next;
            ex_kind        <= idec_kind;
            ex_alu_op      <= idec_alu_op;
            ex_src_mode    <= idec_src_mode;
            ex_src_reg     <= idec_src_reg;
            ex_dst_mode    <= idec_dst_mode;
            ex_dst_reg     <= idec_dst_reg;
            ex_src_imm32   <= id_src_imm32;
            ex_dst_imm32   <= id_dst_imm32;
            ex_branch_imm  <= id_branch_imm;
            ex_imm_raw     <= idec_imm;
            ex_cc          <= idec_cc;
            ex_ra          <= id_ra_fwd;
            ex_rb          <= id_rb_fwd;
            ex_sp          <= id_sp_fwd;
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
                src_an_next = ex_ra + 32'd4;
                src_an_update = 1'b1;
            end
            `EA_ADEC: begin
                src_ea = ex_ra - 32'd4;
                src_needs_mem = 1'b1;
                src_an_next = ex_ra - 32'd4;
                src_an_update = 1'b1;
            end
            `EA_EXT: begin
                case (ex_src_reg)
                    `EA7_ABSW, `EA7_ABSL: begin src_ea = ex_src_imm32; src_needs_mem = 1'b1; end
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
                dst_an_next = ex_rb + 32'd4;
                dst_an_update = 1'b1;
            end
            `EA_ADEC: begin
                dst_ea = ex_rb - 32'd4;
                dst_is_mem = 1'b1;
                dst_an_next = ex_rb - 32'd4;
                dst_an_update = 1'b1;
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

    reg cc_n, cc_z, cc_v, cc_c;
    reg [1:0] ex_state;

    reg [31:0] ex_tas_word;  // word read in TAS phase 0

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

    reg        wb_aux_we_c;
    reg [3:0]  wb_aux_idx_c;
    reg [31:0] wb_aux_data_c;

    reg        cc_we_c;
    reg        cc_n_c, cc_z_c, cc_v_c, cc_c_c;

    reg        take_branch_c;
    reg [31:0] take_branch_target_c;

    reg [31:0] alu_a, alu_b;
    reg [3:0]  alu_op_c;
    wire [31:0] alu_y;
    wire alu_n, alu_z, alu_v, alu_c;

    m68k_alu u_alu (
        .op     (alu_op_c),
        .a      (alu_a),
        .b      (alu_b),
        .y      (alu_y),
        .flag_n (alu_n),
        .flag_z (alu_z),
        .flag_v (alu_v),
        .flag_c (alu_c)
    );

    // dc_req is held until we observe dc_ack at a posedge; the cache's
    // internal state machine ignores cpu_req while it's serving a miss/write.
    reg dc_req_r;
    assign dc_req = dc_req_r && !halted;

    // Settlement conditions: when the current EX-stage instruction completes
    // its work this cycle, downstream effects (regfile writeback, CCR commit,
    // branch redirect) take effect and the pipeline can advance.
    wire is_settled_in_run     = (ex_state == S_RUN) && !want_mem && ex_valid && !halted;
    wire is_settled_after_load = (ex_state == S_LOAD)  && dc_ack &&
                                 ex_valid && !halted && (ex_kind != K_TAS);
    wire is_settled_after_store= (ex_state == S_STORE) && dc_ack && ex_valid && !halted;
    wire is_settled_after_tasw = (ex_state == S_TASW)  && dc_ack && ex_valid && !halted;
    wire is_settled = is_settled_in_run || is_settled_after_load ||
                      is_settled_after_store || is_settled_after_tasw;

    assign stall = ex_valid && !halted && !is_settled;

    // Branch redirect (combinational). For K_BCC we only redirect on a
    // mispredict; for non-Bcc branches (JMP/JSR/RTS) there's no prediction so
    // we always redirect on take. STOP freezes the IF stage at the STOP PC.
    wire stop_now      = ex_valid && (ex_kind == K_STOP) && (ex_state == S_RUN) && !halted;
    wire bcc_mispred   = is_settled && (ex_kind == K_BCC) && (ex_predicted_taken != take_branch_c);
    wire other_branch  = is_settled && (ex_kind != K_BCC) && take_branch_c;
    assign redirect_valid = bcc_mispred || other_branch || stop_now;
    assign redirect_pc    = stop_now ? ex_pc :
                            (ex_kind == K_BCC)
                              ? (take_branch_c ? (ex_pc + 32'd2 + ex_branch_imm) : ex_pc_next)
                              : take_branch_target_c;

    // Big planning block — computes wb_*_c, cc_*_c, want_mem etc. based on
    // ex_state and ex_kind.
    always @* begin
        want_mem = 1'b0; want_we = 1'b0; want_lock = 1'b0;
        want_addr = 32'd0; want_wdata = 32'd0; want_be = 4'b1111;
        wb_main_we_c = 1'b0; wb_main_idx_c = 4'd0; wb_main_data_c = 32'd0;
        wb_aux_we_c  = 1'b0; wb_aux_idx_c  = 4'd0; wb_aux_data_c  = 32'd0;
        cc_we_c = 1'b0;
        cc_n_c = 1'b0; cc_z_c = 1'b0; cc_v_c = 1'b0; cc_c_c = 1'b0;
        take_branch_c = 1'b0;
        take_branch_target_c = 32'd0;
        alu_a = 32'd0; alu_b = 32'd0;
        alu_op_c = `ALU_MOV;

        if (!ex_valid || halted) ;
        else case (ex_state)
            S_RUN: begin
                case (ex_kind)
                    K_NOP, K_STOP: ;
                    K_JMP: begin
                        take_branch_c = 1'b1;
                        take_branch_target_c = (ex_src_mode == `EA_EXT) ? ex_src_imm32 : ex_ra;
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
                        wb_main_we_c = 1'b1;
                        wb_main_idx_c = ex_reg_idx_full;
                        wb_main_data_c = (ex_src_mode == `EA_EXT) ? ex_src_imm32 : src_ea;
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
                            want_wdata = src_operand;
                            want_be = 4'b1111;
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
                            if (ex_alu_op != `ALU_CMP) begin
                                wb_main_we_c = 1'b1;
                                wb_main_idx_c = ex_reg_idx_full;
                                wb_main_data_c = alu_y;
                            end
                        end
                    end
                    K_ALUQ: begin
                        alu_op_c = ex_alu_op;
                        alu_a = src_operand;
                        alu_b = ex_imm_raw;
                        cc_we_c = 1'b1;
                        cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v; cc_c_c = alu_c;
                        if (ex_src_mode == `EA_DREG || ex_src_mode == `EA_AREG) begin
                            wb_main_we_c = 1'b1;
                            wb_main_idx_c = (ex_src_mode == `EA_AREG) ? {1'b1, ex_src_reg}
                                                                      : {1'b0, ex_src_reg};
                            wb_main_data_c = alu_y;
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
                    end
                    default: ;
                endcase
            end
            S_LOAD: begin
                case (ex_kind)
                    K_MOVE: begin
                        wb_main_we_c = 1'b1;
                        wb_main_idx_c = (ex_dst_mode == `EA_AREG) ? {1'b1, ex_dst_reg}
                                                                  : {1'b0, ex_dst_reg};
                        wb_main_data_c = dc_rdata;
                        cc_we_c = (ex_dst_mode == `EA_DREG);
                        cc_n_c = dc_rdata[31];
                        cc_z_c = (dc_rdata == 32'd0);
                        if (src_an_update) begin
                            wb_aux_we_c = 1'b1;
                            wb_aux_idx_c = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end
                    K_ALU: begin
                        alu_op_c = ex_alu_op;
                        alu_a = ex_rb;
                        alu_b = dc_rdata;
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
                    default: ;
                endcase
            end
            S_STORE: begin
                case (ex_kind)
                    K_MOVE: begin
                        cc_we_c = 1'b1;
                        cc_n_c = src_operand[31];
                        cc_z_c = (src_operand == 32'd0);
                        if (dst_an_update) begin
                            wb_aux_we_c = 1'b1;
                            wb_aux_idx_c = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end
                    end
                    K_JSR: begin
                        wb_aux_we_c = 1'b1;
                        wb_aux_idx_c = 4'd15;
                        wb_aux_data_c = ex_sp - 32'd4;
                        take_branch_c = 1'b1;
                        take_branch_target_c = (ex_src_mode == `EA_EXT) ? ex_src_imm32 : ex_ra;
                    end
                    K_BCC: begin
                        // BSR completion.
                        wb_aux_we_c = 1'b1;
                        wb_aux_idx_c = 4'd15;
                        wb_aux_data_c = ex_sp - 32'd4;
                        take_branch_c = 1'b1;
                        take_branch_target_c = ex_pc + 32'd2 + ex_branch_imm;
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
    end

    // Drive wb_* combinationally.
    always @* begin
        wb_we       = 1'b0;
        wb_widx     = 4'd0;
        wb_wdata    = 32'd0;
        wb_aux_we   = 1'b0;
        wb_aux_idx  = 4'd0;
        wb_aux_data = 32'd0;
        if (is_settled) begin
            wb_we       = wb_main_we_c;
            wb_widx     = wb_main_idx_c;
            wb_wdata    = wb_main_data_c;
            wb_aux_we   = wb_aux_we_c;
            wb_aux_idx  = wb_aux_idx_c;
            wb_aux_data = wb_aux_data_c;
        end
    end

    // Sequential state updates (CCR commit, branch redirect, ex_state, dc_*).
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cc_n <= 1'b0; cc_z <= 1'b0; cc_v <= 1'b0; cc_c <= 1'b0;
            ex_state <= S_RUN;
            ex_tas_word <= 32'd0;
            dc_req_r <= 1'b0;
            dc_we <= 1'b0; dc_lock <= 1'b0;
            dc_addr <= 32'd0; dc_wdata <= 32'd0; dc_be <= 4'b1111;
            halted <= 1'b0;
            halt_code <= 16'd0;
            retired <= 32'd0;
        end else begin
            // Count instructions that complete each cycle. STOP doesn't count.
            if (is_settled && ex_kind != K_STOP) retired <= retired + 32'd1;

            if (is_settled && cc_we_c) begin
                cc_n <= cc_n_c; cc_z <= cc_z_c; cc_v <= cc_v_c; cc_c <= cc_c_c;
            end

            // STOP halts.
            if (ex_valid && ex_kind == K_STOP && ex_state == S_RUN && !halted) begin
                halted <= 1'b1;
                halt_code <= ex_imm_raw[15:0];
            end

            case (ex_state)
                S_RUN: begin
                    if (ex_valid && !halted && want_mem) begin
                        dc_req_r <= 1'b1;
                        dc_we <= want_we;
                        dc_lock <= want_lock;
                        dc_addr <= want_addr;
                        dc_wdata <= want_wdata;
                        dc_be <= want_be;
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
                default: ex_state <= S_RUN;
            endcase
        end
    end
endmodule
