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
    output reg  [31:0] retired,      // instructions retired since reset
    // High while CPU is parked on a STOP instruction waiting for an IRQ.
    // External logic (Agnus beam counter, CIA tick) may use this to
    // fast-forward sim time during long idle stretches without changing
    // observable behavior — the CPU only resumes when an IRQ redirects PC.
    output wire        cpu_in_stop,
    // Current EX-stage PC, exposed so the simulator harness can trigger
    // chipram snapshots at specific instructions.  Pure observation -
    // no functional impact.  See DIAGNOSTICS.md "CHIPRAM_SNAP_PCS".
    output wire [31:0] cur_pc,
    // ---- gdbserver debug interface (pure observation) ----
    // Full register file flattened: regs[0]=D0..regs[7]=D7, regs[8]=A0..
    // regs[15]=A7.  Each entry is 32 bits, ordered LSB-first in the bus.
    output wire [16*32-1:0] dbg_regs_flat,
    // Status register (T,S,I[2:0],X,N,Z,V,C — packed into 16 bits as
    // sr_now is built).
    output wire [15:0]      dbg_sr
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
    localparam K_CMPM    = 6'd46;

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
    // CMPM second-memory-load: first load (at Ay) completes in S_LOAD, then
    // we issue a second load at Ax and finish in this state.
    localparam S_CMPM_LOAD2    = 4'd15;

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

`ifdef KICKSTART_COSIM_TRACE
    // Cosim trace window bounds (set via +cosim_start=N +cosim_end=N
    // plusargs).  Default: cover the whole run.
    reg [31:0] cosim_start_r;
    reg [31:0] cosim_end_r;
    reg [31:0] cosim_last_retired;   // dedupe filter — emit [Cosim] only on
                                     // retired-counter advance, not every
                                     // settled cycle (STOP idles otherwise
                                     // produce one [Cosim] per clock).
    initial begin
        if (!$value$plusargs("cosim_start=%d", cosim_start_r)) cosim_start_r = 32'd0;
        if (!$value$plusargs("cosim_end=%d",   cosim_end_r))   cosim_end_r   = 32'hFFFFFFFF;
        cosim_last_retired = 32'hFFFFFFFF;
    end
`endif

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
    // Real 68000 has only 24 address lines, so any PC carried past
    // \$00FFFFFE wraps to \$00000000 on the bus.  Mask the sequential
    // / Bcc predicted target the same way: without this, a ROM-end
    // JMP wraps to PC=\$01000000 internally, every subsequent
    // sequential add keeps bit 24 set, and the CPU walks chip-RAM-
    // mirror addresses while the bus correctly truncates to chip RAM.
    // The bus arbiter already masks bits 24..31 (m68k_bus.v unflat),
    // but the CPU-side PC register accumulates the bad high bits and
    // pushes them as the return address on exception traps, so RTE
    // pops back into the same garbage forever.  See also the redirect
    // path at line ~372 which already applies this mask.
    wire [31:0] if_fetch_target_raw = predict_taken_w ? predicted_target_w
                                                      : next_seq_pc;
    wire [31:0] if_fetch_target    = {8'd0, if_fetch_target_raw[23:0]};

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
            // Trace any redirect to an address outside RAM ($00000-$1FFFFF),
            // ROM ($F80000-$FFFFFF), or Agnus trapdoor slow RAM
            // ($C00000-$C7FFFF).
            if (redirect_pc != ex_pc) begin
                if (!(redirect_pc[31:21] == 11'b0) &&            // not 0x000000-0x1FFFFF
                    !(redirect_pc[31:20] == 12'hF80) &&          // not 0xF80000-0xF8FFFF
                    !(redirect_pc[31:19] == 13'b1111_1000_0000_1) && // 0xF90000-0xF97FFF
                    !(redirect_pc >= 32'h00F80000 &&
                      redirect_pc <= 32'h00FFFFFF) &&
                    !(redirect_pc >= 32'h00C00000 &&
                      redirect_pc <  32'h00C80000))
                    $display("[BAD-PC] from=%h to=%h retired=%d kind=%d sp=%h op=%h ra=%h rb=%h",
                        ex_pc, redirect_pc, retired, ex_kind, rf_rc_data,
                        ex_opcode, ex_ra, ex_rb);
                // Also trace any redirect landing in the vector table
                // ($0-$3FF) — that's "control transferred to data" which
                // is virtually always wrong outside of exception entry.
                if (redirect_pc < 32'h0000_0400)
                    $display("[VEC-EXEC] from=%h to=%h retired=%d kind=%d sp=%h",
                        ex_pc, redirect_pc, retired, ex_kind, rf_rc_data);
            end
`endif
            // A speculative fetch may still be in flight to the cache. Mark
            // it for drain so the eventual ack is discarded before we start
            // the redirected fetch.
            // 68000 has a 24-bit external address bus (A1..A23) -- PC is
            // 32 bits internally but the upper 8 bits are ignored on every
            // bus fetch.  K1.3 stores function pointers with garbage in the
            // top byte (e.g., vector $20 = $7800090E should fetch from $90E
            // on real hardware).  Mask redirect targets here so downstream
            // fetch, decode, exception-frame-push etc. all use the same
            // 24-bit address.
            if_pc <= {8'd0, redirect_pc[23:0]};
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
            K_MOVE, K_MOVEA, K_MOVESR, K_CMPM:   id_rb_idx = id_dst_base_idx;
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
    // For MOVEM with EA_IDX, the first extension word is the register mask
    // and the brief extension lives at id_ext[1].  All other instructions
    // place the brief extension as the first (or only) ext word.
    wire [15:0] id_src_ext0 = (idec_kind == K_MOVEM) ? id_ext[1] : id_ext[0];
    wire [15:0] id_dst_ext0 = id_ext[idec_src_ext_words];
    wire [3:0]  id_src_xreg = {id_src_ext0[15], id_src_ext0[14:12]};
    wire [3:0]  id_dst_xreg = {id_dst_ext0[15], id_dst_ext0[14:12]};
    wire [3:0] id_rc_idx = id_src_is_idx ? id_src_xreg
                         : id_dst_is_idx ? id_dst_xreg
                         : 4'd15;
    // Fourth read port: dst xreg.  rc carries src xreg (or fallback A7); when
    // an instruction needs BOTH src and dst xregs (MOVE.W between two
    // EA_IDX modes with different Xn), rd carries the dst xreg.  Defaults
    // to A7 when dst isn't EA_IDX -- harmless because dst_ea only consults
    // ex_xreg_dst for EA_IDX destinations.
    wire [3:0] id_rd_idx = id_dst_is_idx ? id_dst_xreg : 4'd15;

    wire [31:0] rf_ra_data, rf_rb_data, rf_rc_data, rf_rd_data;

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
        .rd_idx   (id_rd_idx),
        .rd_data  (rf_rd_data),
        .we       (wb_we),
        .w_idx    (wb_widx),
        .w_size   (wb_size),
        .w_data   (wb_wdata),
        .aux_we   (wb_aux_we),
        .aux_idx  (wb_aux_idx),
        .aux_data (wb_aux_data)
    );

    // Forwarding network (EX → ID).  When the main writeback is going to
    // a D-register with a byte- or word-sized op, the regfile's own
    // sized_write() merges only the low bits of wb_wdata with the
    // preserved upper bits of the existing register value.  The forwarder
    // must mirror that merge here -- otherwise an instruction that reads
    // Dn the same cycle Dn is being byte-written sees wb_wdata directly
    // (low byte in place, upper 24 zero) and loses the upper bits.
    //
    // Surfaced in Kickstart 1.3 InitStruct's word-copy entry sequence:
    //   MOVE.L A1, D1 ; ADDQ.L #1, D1 ; AND.B #$FE, D1 ; MOVEA.L D1, A1
    // The MOVEA.L immediately following AND.B forwarded the unmerged
    // byte-write, so A1 ended up at $0000003A instead of $00FC243A and
    // InitStruct copied source bytes from the chip-RAM vector table over
    // its destination, smashing the heap free chain and triggering a
    // downstream double-AllocMem of a live Resource node.
    //
    // For An destinations the regfile always writes 32 bits, so no merge
    // is needed there.
    function [31:0] fwd;
        input [3:0]  idx;
        input [31:0] base;
        begin
            if (wb_we && wb_widx == idx) begin
                if (idx[3])
                    fwd = wb_wdata;
                else case (wb_size)
                    `SZ_B: fwd = {base[31:8],  wb_wdata[7:0]};
                    `SZ_W: fwd = {base[31:16], wb_wdata[15:0]};
                    default: fwd = wb_wdata;
                endcase
            end else if (wb_aux_we && wb_aux_idx == idx)
                fwd = wb_aux_data;
            else
                fwd = base;
        end
    endfunction

    wire [31:0] id_ra_fwd = fwd(id_ra_idx, rf_ra_data);
    wire [31:0] id_rb_fwd = fwd(id_rb_idx, rf_rb_data);
    wire [31:0] id_sp_fwd = fwd(id_rc_idx, rf_rc_data);
    // True A7 read (post-forwarding) used by stack ops that *also*
    // mux the rc port to an index register (e.g. PEA d8(An,Xn)).
    // For those instructions ex_sp is the index, not the stack
    // pointer, so we need a separate path to the current A7.
    wire [31:0] current_a7 = fwd(4'd15, u_rf.regs[15]);
    // Dst-xreg forwarded value (for MOVE between two EA_IDX modes).
    wire [31:0] id_xreg_dst_fwd = fwd(id_rd_idx, rf_rd_data);

    // ====================================================================
    //  ID/EX register.
    // ====================================================================
    reg         ex_valid;
    reg  [31:0] ex_pc;
    reg  [31:0] ex_pc_next;

`ifdef FF4D1C_RINGBUF
    // PC ring buffer: captures the last 16 retired PCs.  Used by the
    // probe that fires at $FF4D1C entry (boing-disk AUTO-REQ path) to
    // dump the caller chain that decided "show File System error dialog".
    // See project_boing_filesystem_error_handler.md.
    reg [3:0]   ring_head;
    reg [31:0]  pc_ring [0:15];
`endif
`ifdef A3_CHANGE_TO_FFC5A0
    // Tracks the prior A3 value so we can detect transitions to
    // $00FFC5A0.  Used by the probe that captures the exact instruction
    // that sets A3 to the boing-disk divergent dispatch target.
    reg [31:0]  a3_prev;
`endif
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
    // Latched A7 -- always carries the real stack pointer, even when
    // ex_sp is muxed to an index register for d8(An, Xn) operands.
    reg  [31:0] ex_a7;
    // Latched dst-index register for d8(An, Xn) destinations.  Needed for
    // MOVE between two EA_IDX EAs (e.g., K1.3 $FDB956 MOVE.W d8(A2,D1.L),
    // d8(A0,D0.L)): ex_sp carries the src xreg, ex_xreg_dst carries the
    // dst xreg, so both EA computations get the right index.
    reg  [31:0] ex_xreg_dst;
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
            ex_a7 <= 32'd0;
            ex_xreg_dst <= 32'd0;
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
            ex_a7          <= current_a7;
            ex_xreg_dst    <= id_xreg_dst_fwd;
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
                // Same as src EA_IDX, but uses ex_rb (dst An base) and
                // ex_xreg_dst (dst's xreg).  Previously this read ex_sp,
                // which works only when src is NOT EA_IDX (rc falls back
                // to dst's xreg in that case).  For MOVE between two
                // EA_IDX modes (e.g., K1.3 $FDB956 MOVE.W d8(A2, D1.L),
                // d8(A0, D0.L)), ex_sp carries the *src* xreg -- using
                // it for the dst would compute A0+D1 instead of A0+D0
                // and corrupt the priv-vio vector table (the K1.3 wall
                // at r=1313466 that gated booting past Exec startup).
                dst_ea = ex_rb
                       + (ex_dst_imm32[11] ? ex_xreg_dst
                                           : {{16{ex_xreg_dst[15]}},
                                              ex_xreg_dst[15:0]})
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
    // Latched PC of the last STOP instruction we logged, so [STOP]
    // $display fires once per fresh STOP rather than every clock cycle
    // the CPU sits halted at the same STOP PC.
    reg [31:0] stop_logged_pc;
    reg        dos_init_seen;
    // Previous value of A5 register, for the [A5-CHANGE] edge trace
    // (used in the WB1.3 corrupt-semaphore investigation).
    reg [31:0] a5_last_r;
    // Edge tracker for [USPTINY]: prevents re-firing while user-mode A7
    // sits below $200 (e.g. a Forbid/Permit nested loop that keeps SP small).
    reg        user_a7_was_tiny;
    reg        super_a7_was_tiny;
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

    // CMPM (Ay)+, (Ax)+ saves the first load's data here while the second
    // load is in flight.  Size-extracted (byte/word/long aligned).
    reg [31:0] cmpm_ay_data;

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
    // Step size: 4 bytes per register for MOVEM.L, 2 for MOVEM.W.  Surfaced
    // by the K1.3 boot wall at $FC6BD0 (MOVEM.W D6/D7, -(SP)): our core
    // always advanced SP by 4 per register, smashing 4 extra bytes on the
    // task stack and the subsequent RTS popped $0 from $18D0.
    wire [31:0] movem_step_bytes = (ex_size == `SZ_L) ? 32'd4 : 32'd2;
    wire [31:0] movem_step_addr = movem_predec ? (movem_addr - movem_step_bytes)
                                                : movem_addr;
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
    // ANDI/ORI/EORI #imm,CCR/SR variants come out of the decoder with
    // dst_mode = EA_EXT and dst_reg = 3'b110 (CCR) or 3'b111 (SR) — both
    // unused mode-7 sub-encodings, used here as sentinels.  Without this
    // discriminator the SR-write path swallowed legitimate ALUI loads to
    // EA_IDX (3'b110) — see tests/t138_cmpi_idx.s.
    wire dst_is_sr_ccr   = (ex_dst_mode == `EA_EXT) &&
                           (ex_dst_reg == 3'b110 || ex_dst_reg == 3'b111);
    wire alui_mem        = ((ex_kind == K_ALUI) && (ex_dst_mode != `EA_DREG) &&
                            !dst_is_sr_ccr) ||
                           ((ex_kind == K_ALUQ) && (ex_src_mode != `EA_DREG) &&
                            (ex_src_mode != `EA_AREG));
    wire alui_mem_writes = alui_mem && (ex_alu_op != `ALU_CMP);
    // K_ALU with direction=1 (Dn -> <ea>): the EA lives in src_* (decoder
    // convention) and is the destination.  Full RMW since the only K_ALU ops
    // that allow direction=1 are ADD/SUB/AND/OR/EOR (CMP is direction=0 only).
    wire alu_mem_dst        = (ex_kind == K_ALU) && (ex_direction == 1'b1) &&
                              (ex_src_mode != `EA_DREG) &&
                              (ex_src_mode != `EA_AREG);
    wire alu_mem_dst_writes = alu_mem_dst;  // never CMP
    // Memory single-bit shift (ASR/ASL/LSR/LSL/ROXR/ROXL/ROR/ROL on .W EA).
    // Decoder routes via K_SHIFT with src_mode = EA mode (not EA_DREG).
    wire shift_mem        = (ex_kind == K_SHIFT) &&
                            (ex_src_mode != `EA_DREG);
    wire shift_mem_writes = shift_mem;
    wire is_settled_after_load = (ex_state == S_LOAD)  && dc_ack &&
                                 ex_valid && !halted && (ex_kind != K_TAS) &&
                                 !(load_starts_rmw && (ex_src_mode != `EA_DREG)) &&
                                 !bit_mem_writes &&
                                 !alui_mem_writes &&
                                 !alu_mem_dst_writes &&
                                 !shift_mem_writes &&
                                 (ex_kind != K_CMPM) &&
                                 !(ex_kind == K_MOVE && dst_is_mem);
    wire is_settled_after_store= (ex_state == S_STORE) && dc_ack && ex_valid && !halted;
    wire is_settled_after_tasw = (ex_state == S_TASW)  && dc_ack && ex_valid && !halted;
    wire is_settled_after_exc  = (ex_state == S_EXC_FETCH) && dc_ack && ex_valid && !halted;
    wire is_settled_after_rte  = (ex_state == S_RTE_POP_PC) && dc_ack && ex_valid && !halted;
    wire is_settled_after_rmw  = (ex_state == S_RMW_W) && dc_ack && ex_valid && !halted;
    // CMPM second load completes here; CCR + Ax post-inc are committed by
    // the combinational S_CMPM_LOAD2 planning, instruction settles.
    wire is_settled_after_cmpm = (ex_state == S_CMPM_LOAD2) && dc_ack &&
                                 ex_valid && !halted;
    // MOVEM settles only when the mask is fully exhausted AND we are not
    // mid-transaction (so the final An commit happens cleanly).
    wire is_settled_after_movem = movem_active && !movem_busy &&
                                  (movem_mask == 16'd0) && ex_valid && !halted;
    wire is_settled = is_settled_in_run || is_settled_after_load ||
                      is_settled_after_store || is_settled_after_tasw ||
                      is_settled_after_exc || is_settled_after_rte ||
                      is_settled_after_rmw || is_settled_after_movem ||
                      is_settled_after_cmpm;

    assign stall = ex_valid && !halted && !is_settled;

    // Branch redirect (combinational). For K_BCC we only redirect on a
    // mispredict; for non-Bcc branches (JMP/JSR/RTS) there's no prediction so
    // we always redirect on take. STOP freezes the IF stage at the STOP PC.
    wire stop_now      = ex_valid && (ex_kind == K_STOP) && (ex_state == S_RUN) && !halted;
    // External fast-forward hint: stays high for as long as the CPU is
    // parked on a STOP waiting for an IRQ.  Once stop_now fires the EX
    // pipeline freezes (no further commits) until a redirect / IRQ
    // arrives.  We approximate "in STOP" as: most recently executed
    // instruction was STOP, EX is in S_RUN, and the CPU isn't halted.
    reg cpu_in_stop_r;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)                       cpu_in_stop_r <= 1'b0;
        else if (stop_now)                cpu_in_stop_r <= 1'b1;
        else if (redirect_valid)          cpu_in_stop_r <= 1'b0;
    end
    assign cpu_in_stop = cpu_in_stop_r;
    assign cur_pc      = ex_pc;

    // ---- gdbserver: expose D0..A7 and SR for external inspection ----
    genvar __dbgi;
    generate
        for (__dbgi = 0; __dbgi < 16; __dbgi = __dbgi + 1) begin : gdbg_regs
            assign dbg_regs_flat[32*__dbgi +: 32] = u_rf.regs[__dbgi];
        end
    endgenerate
    assign dbg_sr = sr_now;
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
                        // Use ex_a7 (the real A7) -- for JSR d8(An, Xn) the
                        // rc port carries Xn so ex_sp would be the index, not
                        // SP.  Same bug class as K_PEA fixed in S32.
                        want_mem = 1'b1; want_we = 1'b1;
                        want_addr = ex_a7 - 32'd4;
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
                        if (alu_mem_dst) begin
                            // ADD/SUB/AND/OR/EOR Dn, <ea> with mem dst.
                            // Read mem, ALU op (mem ALU Dn), write back: full
                            // RMW.  S_LOAD ack captures alu_y + CCR snapshot
                            // and transitions to S_RMW_W (same path as
                            // K_ALUI mem-dest).
                            want_mem  = 1'b1;
                            want_we   = 1'b0;
                            want_lock = 1'b1;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                            if (src_an_update) begin
                                // (An)+/-(An) on dst: commit new An at the
                                // S_RMW_W settle so it doesn't shadow.
                            end
                        end else if (src_needs_mem) begin
                            want_mem = 1'b1; want_we = 1'b0;
                            want_addr = src_ea;
                            want_be = 4'b1111;
                        end else begin
                            alu_b = src_operand;
                            // ADDA/SUBA leave all CCR bits unchanged
                            // (per m68k spec).  Detect by An-destination
                            // (ex_reg_idx_full[3]) plus ALU op = ADD/SUB.
                            // CMPA *does* update N/Z/V/C (X unchanged), so
                            // alu_op == CMP still falls through to the
                            // normal CCR commit.  Surfaced by the Musashi
                            // cosim at K1.3 boot $FC01D2 (SUBA.W #$FD8A, A6)
                            // -- our core was clobbering N/Z/C and the
                            // very next instruction took the wrong branch.
                            //
                            // ADDA/SUBA semantics (per the 68k spec): the
                            // operation is ALWAYS 32-bit on the destination
                            // An.  For ADDA.W / SUBA.W, the *source* is
                            // sign-extended from word to long; the *full*
                            // 32-bit An is then added/subtracted.  Our
                            // size-aware ALU masks BOTH operands to the
                            // op size, which zeroes A1's high byte on
                            // e.g. `ADDA.W D0, A1` with A1 = $00FD6062 and
                            // D0 = 0 -- result wrongly becomes $00006062.
                            // Fix: when An is the destination of ADD/SUB,
                            // force alu_size_c = SZ_L and sign-extend the
                            // source from W to L (if size was .W).  Surfaced
                            // by the WB1.3 deadlock at r=4058118: intuition
                            // template-init helper ($FD643A) uses ADDA.W
                            // D0,A1 to add an offset to a ROM ptr; high
                            // byte was being silently dropped, so the
                            // 11-long template copy ran from chip-RAM
                            // garbage instead of from ROM at $FD6062.
                            if (ex_reg_idx_full[3] &&
                                (ex_alu_op == `ALU_ADD ||
                                 ex_alu_op == `ALU_SUB)) begin
                                alu_size_c = `SZ_L;
                                alu_b = (ex_size == `SZ_W)
                                        ? {{16{src_operand[15]}},
                                           src_operand[15:0]}
                                        : src_operand;
                            end
                            if (!(ex_reg_idx_full[3] &&
                                  (ex_alu_op == `ALU_ADD ||
                                   ex_alu_op == `ALU_SUB))) begin
                                cc_we_c = 1'b1;
                                cc_n_c = alu_n; cc_z_c = alu_z;
                                cc_v_c = alu_v; cc_c_c = alu_c;
                                // ADD/SUB write X; CMP doesn't (X kept
                                // unchanged for both CMP and CMPA).
                                cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                            (ex_alu_op == `ALU_SUB);
                                cc_x_c = alu_x;
                            end
                            if (ex_alu_op != `ALU_CMP) begin
                                wb_main_we_c = 1'b1;
                                // EOR.L Dn,Dm (the only direction=1 K_ALU
                                // op that allows EA_DREG) writes back to
                                // the EA's data reg, not the reg-field.
                                // K1.3 trackdisk header-checksum loop
                                // ($FEACAC: EOR.L D0,D6) silently dropped
                                // every XOR step pre-fix.
                                wb_main_idx_c = (ex_direction == 1'b1 &&
                                                 ex_src_mode == `EA_DREG)
                                                ? {1'b0, ex_src_reg}
                                                : ex_reg_idx_full;
                                wb_main_data_c = alu_y;
                                // ADDA/SUBA always write the full 32-bit
                                // An -- override the per-instruction size
                                // so the wb size matches the ALU size we
                                // just forced to SZ_L.
                                if (ex_reg_idx_full[3] &&
                                    (ex_alu_op == `ALU_ADD ||
                                     ex_alu_op == `ALU_SUB)) begin
                                    wb_main_size_c = `SZ_L;
                                end
                            end
                        end
                    end
                    // CMPM (Ay)+, (Ax)+ : two sequential memory reads then CMP.
                    // First load at Ay (= ex_ra, since src_mode=AINC src_reg=Ay).
                    // S_LOAD ack saves the first byte/word/long, kicks off the
                    // second load at Ax, and transitions to S_CMPM_LOAD2.
                    K_CMPM: begin
                        want_mem  = 1'b1;
                        want_we   = 1'b0;
                        want_addr = src_ea;
                        want_be   = 4'b1111;
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
                        // 68k spec: ADDQ/SUBQ to An is ALWAYS a longword
                        // operation, regardless of the encoded size.  Without
                        // this, ADDQ.W #4, A7 (opcode $584F, used by K1.3 to
                        // pop stack args at $FE0A50) masked the ALU to the
                        // low byte and zero-cleared the high 3 bytes of A7.
                        // After fix, SP = $C071E0 + 4 = $C071E4 (preserved).
                        // Before fix, SP became $000000E4.  Caught WB1.3 FS
                        // task stack corruption at r=4M.
                        alu_size_c = (ex_src_mode == `EA_AREG) ? `SZ_L : ex_size;
                        if (ex_src_mode == `EA_DREG || ex_src_mode == `EA_AREG) begin
                            // ADDQ/SUBQ #imm, An: do NOT update CCR (per
                            // 68k spec, like ADDA/SUBA).  Surfaced by the
                            // Musashi cosim at K1.3 boot $FC19A8:
                            //   ADDQ.B  #8, A2
                            // -- Verilator was clearing X (and updating
                            // N/Z/V/C); Musashi correctly left CCR alone.
                            // ADDQ/SUBQ to Dn does update all CCR bits.
                            if (ex_src_mode != `EA_AREG) begin
                                cc_we_c = 1'b1;
                                cc_n_c = alu_n; cc_z_c = alu_z;
                                cc_v_c = alu_v; cc_c_c = alu_c;
                                cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                            (ex_alu_op == `ALU_SUB);
                                cc_x_c = alu_x;
                            end
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
                        // dst_mode/dst_reg.  CCR/SR variants of ANDI/ORI/EORI
                        // come out of the decoder with dst_mode=EA_EXT and
                        // dst_reg = 3'b110 (CCR) / 3'b111 (SR) — sentinel
                        // values that do not collide with EA_IDX.
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
                        end else if (dst_is_sr_ccr) begin
                            // ANDI/ORI/EORI to CCR (dst_reg=3'b110) or SR
                            // (dst_reg=3'b111).  SR variant is privileged.
                            if (ex_dst_reg == 3'b111 && !sr_s) begin
                                exc_launch_c   = 1'b1;
                                exc_vector_c   = 8'd`VEC_PRIV_VIO;
                                exc_saved_pc_c = ex_pc;
                            end else begin
                                sr_we_c = 1'b1;
                                if (ex_dst_reg == 3'b110) begin
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
                        if (ex_src_mode == `EA_DREG) begin
                            cc_we_c    = 1'b1;
                            cc_x_we_c  = (alu_shamt_c != 6'd0);
                            cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v;
                            cc_c_c = alu_c; cc_x_c = alu_x;
                            wb_main_we_c   = 1'b1;
                            wb_main_idx_c  = {1'b0, ex_src_reg};
                            wb_main_data_c = alu_y;
                        end else begin
                            // Memory shift (single-bit, .W only).  RMW:
                            // read 16-bit word, shift, write back, update
                            // CCR.  The current decoder routes src_mode to
                            // the EA mode (e.g. 5 for d16(An)).  Without
                            // this branch the EA's An-source register
                            // collided with a D-register via
                            // `wb_main_idx_c = {1'b0, ex_src_reg}`, causing
                            // the WB1.3 boot to corrupt D1 on LSR.W $10(A1)
                            // at $FC7C86 — found via cosim diff.  Issue
                            // the memory read; capture happens in S_LOAD.
                            want_mem  = 1'b1;
                            want_we   = 1'b0;
                            want_lock = 1'b1;
                            want_addr = src_ea;
                            want_be   = 4'b1111;
                        end
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
                        // Use ex_a7 (not ex_sp) -- for PEA d8(An, Xn) ex_sp
                        // is muxed to Xn via the rc port, so the actual A7
                        // would otherwise be lost.  Surfaced in K1.3:
                        //   $FD6620: PEA $0(A2, D2.L)
                        //   $FD6624: JSR $00FE0358
                        // pushed to (D2-4) instead of (A7-4), then updated
                        // A7 = D2-4 = $FFFFFFFC (D2 was 0).
                        want_mem  = 1'b1; want_we = 1'b1;
                        want_addr = ex_a7 - 32'd4;
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
                //
                // EXCEPTION for K_STOP: STOP loads SR and waits in S_RUN
                // forever until an IRQ arrives.  The SR load is idempotent
                // (we re-do it every cycle the CPU is parked in STOP), so by
                // the time the IRQ check fires, STOP has effectively
                // "executed."  We must save ex_pc_next (= post-STOP PC) so
                // RTE returns to the instruction after STOP -- not back to
                // STOP itself, which would re-park the CPU and never let
                // the dispatcher at $FC0F94 run.  This is the K1.3 Exec
                // idle loop's wakeup path: STOP $2000 -> IRQ -> handler ->
                // RTE -> BRA back to TaskReady dispatcher.  Without this
                // fix the CPU oscillates between STOP and a brief handler
                // run and never re-dispatches a READY task.
                //
                // Only fires if no other exception is already being launched.
                if (!exc_launch_c && (ipl_i > sr_i)) begin
                    exc_launch_c   = 1'b1;
                    exc_is_irq_c   = 1'b1;
                    exc_vector_c   = 8'd24 + {5'd0, ipl_i};
                    exc_saved_pc_c = (ex_kind == K_STOP) ? ex_pc_next : ex_pc;
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
            // Final cycle of RTE/RTR: write A7 := working_sp + 4 (final SP
            // after both pops; working_sp has been advanced by +2 for the SR
            // pop already, the +4 PC-pop bump is sequential and only commits
            // at the next posedge, so the planning block has to add it here).
            // For RTE returning to user mode the swap with usp_shadow
            // happens; RTR never changes supervisor mode.  Surfaced by the
            // Musashi cosim at K1.3 boot $FC1FC4 RTE -- Verilator was
            // committing original+2, dropping 4 bytes of stack on every RTE,
            // and the very next RTS popped the saved CCR word as its PC.
            S_RTE_POP_PC: begin
                if (dc_ack) begin
                    wb_aux_we_c   = 1'b1;
                    wb_aux_idx_c  = 4'd15;
                    wb_aux_data_c = ((ex_kind == K_RTE) &&
                                     (ex_exc_saved_sr[`SR_S] == 1'b0))
                                    ? usp_shadow
                                    : (working_sp + 32'd4);
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
                    // MOVEM.W loads sign-extend the word into the full 32-bit
                    // register (per spec); MOVEM.L loads write the full long.
                    wb_main_we_c   = 1'b1;
                    wb_main_idx_c  = movem_curr_reg_full;
                    if (ex_size == `SZ_L)
                        wb_main_data_c = dc_rdata;
                    else
                        wb_main_data_c = dc_addr[1]
                            ? {{16{dc_rdata[15]}}, dc_rdata[15:0]}
                            : {{16{dc_rdata[31]}}, dc_rdata[31:16]};
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
                    end else if (ex_kind == K_SHIFT) begin
                        // Memory single-bit shift: CCR was captured at
                        // S_LOAD ack from the ALU snapshot.  Commit all 5
                        // flags — memory shift always updates X/C from
                        // the shifted-out bit, N/Z from the result.
                        cc_we_c   = 1'b1;
                        cc_x_we_c = 1'b1;
                        cc_n_c = ex_alui_n_q;
                        cc_z_c = ex_alui_z_q;
                        cc_v_c = ex_alui_v_q;
                        cc_c_c = ex_alui_c_q;
                        cc_x_c = ex_alui_x_q;
                        // Memory-shift addressing modes that update An
                        // (postinc/predec) are not part of the
                        // single-bit-memshift EA set (modes 2..7 except
                        // 0/1), but cover them for completeness.
                        if (src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end else if (ex_kind == K_ALUI || ex_kind == K_ALUQ ||
                                 ex_kind == K_ALU) begin
                        // Mem-dest ADDI/SUBI/ANDI/ORI/EORI + ADDQ/SUBQ +
                        // ADD/SUB/AND/OR/EOR Dn,<ea>.  N/Z/V/C/X were captured
                        // at S_LOAD ack (ex_alui_*_q) from the in-flight ALU
                        // output; by S_RMW_W the live alu_y is stale.
                        cc_we_c   = 1'b1;
                        cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                    (ex_alu_op == `ALU_SUB);
                        cc_n_c = ex_alui_n_q;
                        cc_z_c = ex_alui_z_q;
                        cc_v_c = ex_alui_v_q;
                        cc_c_c = ex_alui_c_q;
                        cc_x_c = ex_alui_x_q;
                        // K_ALUI tracks the dst An; K_ALUQ and K_ALU
                        // (direction=1) use src An -- the EA is in src_*.
                        if (ex_kind == K_ALUI && dst_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_dst_reg};
                            wb_aux_data_c = dst_an_next;
                        end else if ((ex_kind == K_ALUQ || ex_kind == K_ALU) &&
                                     src_an_update) begin
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
                    K_CMPM: begin
                        // First load (at Ay) just arrived.  No commits here
                        // because the instruction hasn't settled yet (we
                        // chain a second load and finish at S_CMPM_LOAD2).
                        // Ay/Ax post-increments are committed at the
                        // S_CMPM_LOAD2 settle to mirror the K_MOVE
                        // mem-to-mem deferred-commit pattern.
                    end
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
                    K_SHIFT: begin
                        // Memory single-bit shift: alu_b = the loaded word
                        // (lane-extracted), shamt = 1.  Result + CCR
                        // snapshot captured in the sequential block;
                        // S_RMW_W writes the shifted word back.
                        alu_op_c    = ex_alu_op;
                        alu_size_c  = `SZ_W;  // memory shift is always .W
                        alu_b       = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                         : dc_rdata[31:16]};
                        alu_shamt_c = 6'd1;
                    end
                    K_ALU: begin
                        // dc_rdata is the 32-bit aligned read; the ALU operand
                        // must be the size-appropriate field positioned at the
                        // requested address.  For byte/word reads at odd byte
                        // or odd word offsets, the wanted lane is not in
                        // dc_rdata[7:0] / dc_rdata[15:0].
                        // For direction=1 (mem dst), operand roles are swapped:
                        //   mem op Dn -> result.  alu_a = mem, alu_b = Dn.
                        // The sequential block captures alu_y + CCR snapshot
                        // and transitions to S_RMW_W (like K_ALUI mem-dst).
                        alu_op_c = ex_alu_op;
                        alu_size_c = ex_size;
                        if (alu_mem_dst) begin
                            alu_b = ex_rb;
                            case (ex_size)
                                `SZ_B: alu_a = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                                `SZ_W: alu_a = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                                  : dc_rdata[31:16]};
                                default: alu_a = dc_rdata;
                            endcase
                            // CCR commit happens at S_RMW_W settle.
                        end else begin
                            alu_a = ex_rb;
                            case (ex_size)
                                `SZ_B: alu_b = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                                `SZ_W: alu_b = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                                  : dc_rdata[31:16]};
                                default: alu_b = dc_rdata;
                            endcase
                            cc_we_c = 1'b1;
                            cc_n_c = alu_n; cc_z_c = alu_z; cc_v_c = alu_v; cc_c_c = alu_c;
                            // X flag: ADD/SUB write X (= C); CMP keeps X
                            // unchanged. The register-source K_ALU path
                            // (EX-stage) already does this; the mem-source
                            // path (this S_LOAD settle) was missing it,
                            // surfaced by the Musashi cosim at K1.3 boot
                            // $FC14F8: ADD.W -(A0), D1 inside the checksum
                            // loop -- Verilator left X stale while Musashi
                            // tracked C, and a downstream ADDX picked up
                            // the wrong X.
                            cc_x_we_c = (ex_alu_op == `ALU_ADD) ||
                                        (ex_alu_op == `ALU_SUB);
                            cc_x_c = alu_x;
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
                        // Commit predec/postinc An update at settle.  Without
                        // this, TST.B (A0)+ never advances A0 — K1.3's strlen
                        // helper at $FE4AD0 loops 65535 times until D0.w wraps,
                        // stalling FS task's packet handling for ~275K cycles.
                        if (src_an_update) begin
                            wb_aux_we_c = 1'b1;
                            wb_aux_idx_c = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
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
                        // CCR: N/Z based on the byte/word/long that was
                        // actually moved.  For register-source moves
                        // src_operand carries the value.  For memory-source
                        // moves src_operand is 0 (it's only set for D-reg/
                        // A-reg sources); recover the moved byte/word from
                        // dc_wdata (the chained store's wdata, which the
                        // S_LOAD K_MOVE sequential block already positioned
                        // into the right lane at dst_ea[1:0]).
                        //
                        // Without this, Kickstart 1.3's OldOpenLibrary
                        // string builder at $F81AF0
                        //   MOVE.B (A0)+, (A1)+ ; BNE
                        // saw Z=1 (= "src_operand=0") on every byte and the
                        // BNE never branched -- the loop exited after one
                        // iteration, the search name landed as ".library"
                        // (with the prefix smashed), FindResident missed
                        // every resident, and graphics.library stashed 0
                        // in its $1A0 field.  Downstream `JSR $FFE2(0)`
                        // walked off into garbage memory.
                        cc_we_c = 1'b1;
                        if (src_needs_mem) begin
                            case (ex_size)
                                `SZ_B: begin
                                    cc_n_c = byte_at(dc_wdata, dc_addr[1:0])[7];
                                    cc_z_c = (byte_at(dc_wdata, dc_addr[1:0]) == 8'd0);
                                end
                                `SZ_W: begin
                                    cc_n_c = dc_addr[1] ? dc_wdata[15]
                                                        : dc_wdata[31];
                                    cc_z_c = dc_addr[1] ? (dc_wdata[15:0] == 16'd0)
                                                        : (dc_wdata[31:16] == 16'd0);
                                end
                                default: begin
                                    cc_n_c = dc_wdata[31];
                                    cc_z_c = (dc_wdata == 32'd0);
                                end
                            endcase
                        end else begin
                            // Register-source: src_operand has the right
                            // value, but it's full-width.  Mask to size
                            // for CCR.
                            case (ex_size)
                                `SZ_B: begin
                                    cc_n_c = src_operand[7];
                                    cc_z_c = (src_operand[7:0] == 8'd0);
                                end
                                `SZ_W: begin
                                    cc_n_c = src_operand[15];
                                    cc_z_c = (src_operand[15:0] == 16'd0);
                                end
                                default: begin
                                    cc_n_c = src_operand[31];
                                    cc_z_c = (src_operand == 32'd0);
                                end
                            endcase
                        end
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
                        // ex_a7 (real A7), not ex_sp -- see EX-stage comment.
                        wb_aux_data_c = ex_a7 - 32'd4;
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
                        // ex_a7 (not ex_sp) -- see EX-stage comment.
                        wb_aux_data_c = ex_a7 - 32'd4;
                    end
                    K_CLR: begin
                        // Memory CLR: write done; commit CCR.
                        cc_we_c = 1'b1;
                        cc_z_c = 1'b1;
                        cc_n_c = 1'b0; cc_v_c = 1'b0; cc_c_c = 1'b0;
                        // CLR.L -(SP) / (An)+ -- replay the An predec/postinc
                        // update at settle time.  The S_RUN handler also
                        // sets wb_aux_we_c, but wb_aux_we only propagates
                        // when is_settled, which doesn't fire in S_RUN for
                        // memory destinations.  Without this, two
                        // back-to-back CLR.L -(SP) both write to the same
                        // address (the first SP decrement is lost), as
                        // surfaced in K1.3 InitResident's MakeLibrary call
                        // setup at $FCABD6.
                        if (src_an_update) begin
                            wb_aux_we_c   = 1'b1;
                            wb_aux_idx_c  = {1'b1, ex_src_reg};
                            wb_aux_data_c = src_an_next;
                        end
                    end
                    default: ;
                endcase
            end
            S_TASW: begin
                cc_we_c = 1'b1;
                cc_n_c = byte_at(ex_tas_word, dc_addr[1:0])[7];
                cc_z_c = (byte_at(ex_tas_word, dc_addr[1:0]) == 8'd0);
            end
            S_CMPM_LOAD2: begin
                if (ex_kind == K_CMPM) begin
                    // Second load (at Ax) just arrived.  Compute CMP =
                    // Ax_data - Ay_data, set CCR, commit BOTH Ay and Ax
                    // post-increments.  Main writeback carries Ay (= ex_src_reg)
                    // and aux carries Ax (= ex_dst_reg).
                    alu_op_c   = `ALU_CMP;
                    alu_size_c = ex_size;
                    alu_b      = cmpm_ay_data;   // source (Ay)
                    case (ex_size)
                        `SZ_B: alu_a = {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                        `SZ_W: alu_a = {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                          : dc_rdata[31:16]};
                        default: alu_a = dc_rdata;
                    endcase
                    cc_we_c = 1'b1;
                    cc_n_c = alu_n; cc_z_c = alu_z;
                    cc_v_c = alu_v; cc_c_c = alu_c;
                    // CMP doesn't touch X.
                    if (src_an_update) begin
                        wb_main_we_c   = 1'b1;
                        wb_main_idx_c  = {1'b1, ex_src_reg};
                        wb_main_size_c = `SZ_L;        // An writes are 32-bit
                        wb_main_data_c = src_an_next;
                    end
                    if (dst_an_update) begin
                        wb_aux_we_c   = 1'b1;
                        wb_aux_idx_c  = {1'b1, ex_dst_reg};
                        wb_aux_data_c = dst_an_next;
                    end
                end
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
            cmpm_ay_data <= 32'd0;
            dc_req_r <= 1'b0;
            dc_we <= 1'b0; dc_lock <= 1'b0;
            dc_addr <= 32'd0; dc_wdata <= 32'd0; dc_be <= 4'b1111;
            dc_is_long <= 1'b0;
            halted <= 1'b0;
            halt_code <= 16'd0;
            retired <= 32'd0;
            stop_logged_pc <= 32'hFFFF_FFFF;
            a5_last_r <= 32'd0;
            user_a7_was_tiny <= 1'b0;
            super_a7_was_tiny <= 1'b0;
            dos_init_seen <= 1'b0;
        end else begin
            // Count instructions that complete each cycle. STOP doesn't count.
            if (is_settled && ex_kind != K_STOP) retired <= retired + 32'd1;
`ifdef KICKSTART_BOOT_PC_TRACE
            if (is_settled && ex_kind != K_STOP)
                $display("[PC] r=%d pc=%h kind=%d", retired, ex_pc, ex_kind);
`endif
`ifdef CLI_TITLE_CPU_WR_TRACE
            // Trace any CPU memory write whose dest lands in CLI window's
            // title bar BPL1 region ($60C8..$63E8).  Identifies which K1.3
            // ROM routine writes the $2AAA pattern there (since the
            // BLT_CLI_TITLE_TRACE probe in the blitter saw 0 hits).
            if (dc_we &&
                dc_addr >= 32'h0000_60C8 && dc_addr <= 32'h0000_63E8) begin
                $display("[CLI_TITLE_CPU_WR] r=%d pc=%h addr=%h wdata=%h be=%b is_long=%b",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be, dc_is_long);
            end
`endif
`ifdef BLT_DAT_2AAA_PC_TRACE
            // CPU-side companion to bus-side [BLT_DAT_2AAA]: fires on the
            // CPU write to BLTADAT/BLTBDAT/BLTCDAT with $2AAA, printing PC.
            // Note: dc_addr is the longword-aligned address, so $DFF070
            // matches both 16-bit halves; we filter on the wdata word
            // matching $2AAA in either half.
            if (dc_we &&
                (dc_addr == 32'h00DF_F070 || dc_addr == 32'h00DF_F074) &&
                (dc_wdata[31:16] == 16'h2AAA ||
                 dc_wdata[15:0]  == 16'h2AAA)) begin
                $display("[BLT_DAT_2AAA_PC] r=%d pc=%h dc_addr=%h dc_wdata=%h dc_be=%b",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be);
            end
`endif
`ifdef BLT_TRIGGER_PC_TRACE
            // Catch every CPU write to BLTSIZE (\$DFF058 = blit trigger).
            // Prints PC + the size word so we can correlate which K1.3
            // ROM routine issues each blit; cross-references with
            // BLT_START_TITLE/BANNER (which print bltcon and bltdpt) by
            // matching the size word.  Per WB13_DEBUG_JOURNAL §41 — used
            // to find which Intuition routine issues the late LF=\$00
            // banner clear.
            if (dc_we && dc_addr == 32'h00DF_F058) begin
                $display("[BLT_TRIGGER] r=%d pc=%h dc_wdata=%h dc_be=%b",
                    retired, ex_pc, dc_wdata, dc_be);
            end
`endif
`ifdef BOING_PC_SAMPLE
            // Sample the PC every ~512K retired in the boing-disk
            // post-AUTO-REQ window to identify what code the CPU is
            // executing — was hoping boing! task does Intuition
            // OpenScreen but maybe is stuck in SERDATR poll or other.
            if (is_settled && retired[18:0] == 19'd0 && retired >= 32'd30_000_000)
                $display("[PC-SAMPLE] r=%0d pc=%h sp=%h A6=%h",
                    retired, ex_pc, u_rf.regs[15], u_rf.regs[14]);
`endif
`ifdef ADDTASK_TRACE
            // Catch every call to exec.library AddTask ($FC1C84 in K1.3
            // ROM).  A1 points to the new Task struct; A1+10 is
            // tc_Node.ln_Name (pointer to ASCII name).  Tracking these
            // identifies whether boing! ever got CreateProc'd.  If
            // boing.samples decode succeeded but boing! task never
            // appears, AddTask was not called → CreateProc upstream
            // failed.
            if (is_settled && ex_pc == 32'h00fc_1c84)
                $display("[ADDTASK] r=%0d task=%h name_ptr=mem[A1+10] A0=%h A1=%h",
                    retired, u_rf.regs[9], u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef BOING_FLAG_WR
            // Catch any write to chip \$10256 — boing!'s main-loop wait
            // flag.  If the flag never gets set, boing! main loop spins
            // forever at \$C09762.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00010254 && dc_addr <= 32'h00010259)
                $display("[BOING_FLAG] r=%0d pc=%h addr=%h wdata=%h",
                    retired, ex_pc, dc_addr, dc_wdata);
`endif
`ifdef BOING_SLOW_PC_CATCH
            // boing!'s code lives at slow \$C07984+ (per cli_Module BPTR
            // \$301E61) and dos.library code clusters at \$C09D44+/\$C0A130+.
            // Catch any PC in \$C04000..\$C0AFFF range (BCPL CLI + loaded
            // user code) every 4K retired.
            if (is_settled && retired >= 32'd5_000_000 &&
                ex_pc >= 32'h00c0_4000 && ex_pc <= 32'h00c0_afff &&
                retired[11:0] == 12'd0)
                $display("[SLOW-USER-PC] r=%0d pc=%h sp=%h A6=%h A4=%h",
                    retired, ex_pc, u_rf.regs[15], u_rf.regs[14], u_rf.regs[12]);
`endif
`ifdef BOING_CHIP_PC_CATCH
            // Catch any execution in chip RAM range that's NOT in the
            // K1.3 BCPL low-area ($0..$3FF vector table) or the CLI's
            // BCPL operand fetches in slow RAM.  Boing! runs from
            // somewhere in chip $1000..$5FFFF after LoadSeg.
            if (is_settled && retired >= 32'd5_000_000 &&
                ex_pc >= 32'h00000400 && ex_pc < 32'h00060000 &&
                retired[12:0] == 13'd0)  // sample 1 in every 8K
                $display("[CHIP-PC] r=%0d pc=%h sp=%h A6=%h",
                    retired, ex_pc, u_rf.regs[15], u_rf.regs[14]);
`endif
`ifdef BANNER_CLEAR_PC_STACK_TRACE
            // Targeted: when the per-plane banner clear loop trigger fires
            // (PC=\$FE301E per WB13_DEBUG_JOURNAL §42), dump all 16
            // registers + A7 pointer.  Caller is one return-frame deep;
            // mem[A7] is the return PC of the loop helper, mem[A7+4..] is
            // the parent's saved PC.  Per task #145.
            if (is_settled && ex_pc == 32'h00fe_301e && retired >= 32'd24000000) begin
                $display("[BANNER_CLEAR_STACK] r=%d pc=%h", retired, ex_pc);
                $display("  D0=%h D1=%h D2=%h D3=%h D4=%h D5=%h D6=%h D7=%h",
                    u_rf.regs[0],  u_rf.regs[1],  u_rf.regs[2],  u_rf.regs[3],
                    u_rf.regs[4],  u_rf.regs[5],  u_rf.regs[6],  u_rf.regs[7]);
                $display("  A0=%h A1=%h A2=%h A3=%h A4=%h A5=%h A6=%h A7=%h",
                    u_rf.regs[8],  u_rf.regs[9],  u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[12], u_rf.regs[13], u_rf.regs[14], u_rf.regs[15]);
            end
`endif
`ifdef LATE_CLEAR_STACK_TRACE
            // Trace the late LF=$00 full-BPL1 clear at PC=$FCB39A
            // (per WB13_DEBUG_JOURNAL §55 — issued twice, the second
            // erases the bottom border drawn at r=4M).  Dump regs to
            // identify which Intuition/Workbench routine retriggers it.
            if (is_settled && ex_pc == 32'h00fc_b39a && retired >= 32'd20000000) begin
                $display("[LATE_CLEAR_STACK] r=%d pc=%h", retired, ex_pc);
                $display("  D0=%h D1=%h D2=%h D3=%h D4=%h D5=%h D6=%h D7=%h",
                    u_rf.regs[0],  u_rf.regs[1],  u_rf.regs[2],  u_rf.regs[3],
                    u_rf.regs[4],  u_rf.regs[5],  u_rf.regs[6],  u_rf.regs[7]);
                $display("  A0=%h A1=%h A2=%h A3=%h A4=%h A5=%h A6=%h A7=%h",
                    u_rf.regs[8],  u_rf.regs[9],  u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[12], u_rf.regs[13], u_rf.regs[14], u_rf.regs[15]);
            end
`endif
`ifdef BLT_PTR_WR_TRACE
            // Trace every CPU write to BLTAPT ($DFF050/$DFF052) so we
            // can find the routine computing $0280FF38 (the bad
            // pointer for the WB Backdrop left-border blit per §57).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F050 && dc_addr <= 32'h00DF_F053)
                $display("[BLTAPT-WR] r=%d pc=%h addr=%h wdata=%h be=%b D0=%h A0=%h A1=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef FE3014_PROBE
            // Print A4/D0/D4 right BEFORE the dangerous read at $FE3014
            // (MOVE.L $08(A4, D0.W), D0).  Identifies the actual source
            // struct address `A4 + 8 + sign_extend(D0[15:0])` that the
            // K1.3 layer-refresh code reads to get bltdpt.  See
            // project_boing_post_validator_chain.md for context.
            if (is_settled && ex_pc == 32'h00fe_3014)
                $display("[FE3014] r=%0d A4=%h D0=%h D4=%h  → addr=A4+8+D0.W=%h",
                    retired, u_rf.regs[12], u_rf.regs[0], u_rf.regs[4],
                    u_rf.regs[12] + 32'd8 +
                    {{16{u_rf.regs[0][15]}}, u_rf.regs[0][15:0]});
`endif
`ifdef BPLPTR_C01418_WR_TRACE
            // Trace every write to BitMap.Planes[0..7] at slow $C01418-$C01437.
            // Investigation of the boing-disk rogue blit at $FE301A
            // (project_boing_post_validator_chain.md): the K1.3 LAYER
            // refresh path reads BitMap.Planes[idx] from $C01418+idx*4
            // and writes it to BLTDPT.  At r=11153359 the read returned
            // the corrupted value $6438.  Find who wrote $6438 there.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_1418 && dc_addr < 32'h00C0_1438)
                $display("[BPLPTR-WR] r=%0d pc=%h addr=%h wdata=%h be=%b A0=%h A1=%h D0=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[0]);
`endif
`ifdef FF4D0C_VALUE_WATCH
            // Watch CPU writes that COULD be part of pushing $00FF4D0C
            // onto the stack — the deferred-call address that BCPL DOS
            // code stashes before calling an inner helper.  RTS at slow
            // $C050E4 pops this value and lands at AutoRequest entry
            // $FF4D0C.
            //
            // ONLY catches:
            //   - Full .L writes (be=1111) of exact value $00FF4D0C
            //   - 16-bit writes (be=0011) of the low half $4D0C (rare
            //     pattern; usually paired with be=1100 high-half write)
            // The high-half writes ($00FF) are too common to filter
            // on alone — there are millions of them in exec routines.
            if (dc_req_r && dc_we && dc_ack &&
                ((dc_be == 4'b1111 && dc_wdata == 32'h00FF_4D0C) ||
                 (dc_be == 4'b0011 && dc_wdata[15:0] == 16'h4D0C)))
                $display("[FF4D0C-WR] r=%0d pc=%h addr=%h wdata=%h be=%b A0=%h A1=%h A5=%h A6=%h A7=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[8], u_rf.regs[9],
                    u_rf.regs[13], u_rf.regs[14], u_rf.regs[15]);
`endif
`ifdef C00EBC_WR_TRACE
            // Watch writes to slow $C00EBC — the struct slot that
            // MOVEM at $FF4128 loads into A3.  Per the boing-disk
            // investigation, at r=11133232 this slot held $FFC5A0
            // (the AutoRequest dispatch target).  At AUTO-REQ time
            // it holds $FFC434 in our sim vs $FFC412 in FS-UAE.
            // Find who writes $FFC5A0 to this slot.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C00EB8 && dc_addr < 32'h00C00EC4)
                $display("[C00EBC-WR] r=%0d pc=%h addr=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef BLT_BOING_TRACE
            // When CPU writes the upper word of BLTDPT (\$DFF054) with
            // a value that puts destination inside boing's buffer
            // (\$11C38..\$1E0E0), log the write so we can see the blit's
            // destination address.  Then we can also log BLTAPT etc.
            // via the existing chipset writes that fire just before
            // each blit trigger.
            //
            // We track only the .L writes to \$DFF054 (BLTDPT high half)
            // since the high half determines the chip-RAM bank.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00DF_F054 && dc_be == 4'b1111 &&
                dc_wdata >= 32'h0001_1C38 && dc_wdata <= 32'h0001_E0E0)
                $display("[BLT-BOING-DPT] r=%0d pc=%h BLTDPT=%h",
                    retired, ex_pc, dc_wdata);
            // Also catch all writes to BLTAPT (\$DFF050) so we can
            // correlate the source pointer with subsequent BLTDPT.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00DF_F050 && dc_be == 4'b1111)
                $display("[BLT-BOING-APT] r=%0d pc=%h BLTAPT=%h",
                    retired, ex_pc, dc_wdata);
            // Catch BLTSIZE writes (\$DFF058 — triggers the blit) to
            // see when each blit fires.  Log size + the regs at the moment.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00DF_F058)
                $display("[BLT-BOING-SIZE] r=%0d pc=%h BLTSIZE=%h be=%b",
                    retired, ex_pc, dc_wdata, dc_be);
`endif
`ifdef FFC422_PROBE
            // Probe at \$FFC422 TST.L D1 — captures D1 just after the
            // 2nd dispatch returns.  If D1==0, BEQ \$FFC43A takes and
            // skips the failing 3rd dispatch.  If D1!=0, the 3rd
            // dispatch runs and causes AUTO-REQ.
            //
            // The handler called was at A4=mem[A2+\$2B0]=\$FFED98 +
            // any LEA advance from the 1st dispatch.  We expect D1=0
            // on real K1.3 / bisect-3, but get D1!=0 on bisect-4.
            if (is_settled && ex_pc == 32'h00FF_C422)
                $display("[FFC422-TST] r=%0d D1=%h A1=%h A2=%h A4=%h (D1==0 → skip fail path)",
                    retired, u_rf.regs[1], u_rf.regs[9],
                    u_rf.regs[10], u_rf.regs[12]);
            // Also probe BEFORE the 2nd dispatch at \$FFC41A so we see
            // D1's INPUT to the handler — vs \$FFC422 which captures
            // the handler's RETURN.  If both inputs are identical
            // across passing/failing runs but the output differs, the
            // handler chain executed something different.
            if (is_settled && ex_pc == 32'h00FF_C41A)
                $display("[FFC41A-PRE] r=%0d A1=%h A2=%h D1=%h D2=%h (in: BCPL packet pre-dispatch)",
                    retired, u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[1], u_rf.regs[2]);
`endif
`ifdef MOVEM_AUDIT
            // Audit \$FF413E MOVEM.L D1,D2,D3,D4, (A1) — check that ALL
            // 4 longs (16 bytes) get written.  If our MOVEM is buggy
            // and writes only 3 longs at certain addressing modes, we'd
            // see only 3 writes here per MOVEM execution.
            if (dc_req_r && dc_we && dc_ack && ex_pc == 32'h00FF_413E)
                $display("[MOVEM-WR] r=%0d addr=%h wdata=%h be=%b D1=%h D2=%h D3=%h D4=%h",
                    retired, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[1], u_rf.regs[2], u_rf.regs[3], u_rf.regs[4]);
            // Audit $FF4136 MOVEM.L A1,A3,A4, $F4(A1,D0.L) — save step.
            // $F4 = -12 signed.  Save addr = A1_in + (-12) + D0.
            // For inner dispatch (A1=$C00EC4, D0=$14): save addr =
            // $C00ED0.  This conflicts with the subsequent input MOVEM
            // at $FF413E which writes D1..D4 to $C00ED8..$C00EE7 — but
            // the save and input regions overlap at $C00ED0..$C00EDB.
            // If our MOVEM addressing is wrong, the save could go to
            // an unrelated address.
            if (dc_req_r && dc_we && dc_ack && ex_pc == 32'h00FF_4136)
                $display("[SAVE-MOVEM-WR] r=%0d addr=%h wdata=%h be=%b D0=%h A1=%h A3=%h A4=%h",
                    retired, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[9], u_rf.regs[11], u_rf.regs[12]);
`endif
`ifdef DOIO_PROBE
            // [DoIO] at PC=\$FFD3FC: just before the BCPL wrapper for
            // exec.library:DoIO (LVO -456 = \$FFFFFE38).
            // Through \$FF46E8 wrapper:
            //   A1 = IORequest <-- mem[(BCPL_A1)+\$10]
            // So we want to capture A1 AT \$FF46F8 (which loads A1 = mem[BCPL_A1+\$10]).
            // But simpler to probe AT the DoIO JSR site \$FF4700 with A1 fully loaded.
            if (is_settled && ex_pc == 32'h00FF_4700)
                $display("[DoIO-CALL] r=%0d A1=%h (IORequest) A0=%h A2=%h D0=%h D1=%h",
                    retired,
                    u_rf.regs[9], u_rf.regs[8], u_rf.regs[10],
                    u_rf.regs[0], u_rf.regs[1]);
`endif
`ifdef OPENDEVICE_PROBE
            // [OD] at PC=\$FFD42A: just before the BCPL wrapper for
            // exec.library:OpenDevice (LVO -456 = \$FFFFFE38).
            // Through \$FF46E8 wrapper, the args become:
            //   A0 = device name ptr   <-- BCPL D4 (= u_rf.regs[4])
            //   D0 = unit number       <-- BCPL D2 (= u_rf.regs[2])
            //   D1 = flags             <-- BCPL D3 (= u_rf.regs[3])
            //   A1 = IORequest         <-- mem[A1+\$10]
            // Capturing these tells us WHICH device is being opened.
            if (is_settled && ex_pc == 32'h00FF_D42A)
                $display("[OPENDEV] r=%0d A1=%h name_ptr=D4=%h unit=D2=%h flags=D3=%h",
                    retired, u_rf.regs[9],
                    u_rf.regs[4], u_rf.regs[2], u_rf.regs[3]);
            // Also probe just AFTER the OpenDevice JSR returns
            // (\$FFD432) to capture the result code in D0.
            if (is_settled && ex_pc == 32'h00FF_D432)
                $display("[OPENDEV-RET] r=%0d D0=%h D1=%h (OpenDevice ret: 0=success, nonzero=error)",
                    retired, u_rf.regs[0], u_rf.regs[1]);
`endif
`ifdef C00C9C_WR_TRACE
            // Probe writes to slow \$C00C9C — chip-RAM buffer pointer
            // for K1.3 BCPL DOS file headers.  FS-UAE WB1.3 has
            // \$00010A0C here.  Our sim has \$00001574 (low chip RAM)
            // which is the root divergence causing graphics line-draws
            // to scribble OFS file-header memory.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00C0_0C9C)
                $display("[C00C9C-WR] r=%0d pc=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h",
                    retired, ex_pc, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef C00EAC_WR_TRACE
            // Probe writes to slow $C00EAC — at $FFC3A2, K1.3 reads
            // mem[$C00EAC] as a BPTR index, shifts <<2, then uses it
            // to index a chip-RAM struct.  The result becomes $55D
            // (= chip $1574) for the failing checksum case.
            // Find what writes the failing-case index value.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00C0_0EAC)
                $display("[C00EAC-WR] r=%0d pc=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h",
                    retired, ex_pc, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef C00EB0_WR_TRACE
            // Probe writes to slow $C00EB0 — the BPTR slot that ends up
            // fed to checksum routine $FFED98 as input D1.  At $FFC416,
            // K1.3 loads D1 = mem[$0004(A1)] where A1=$C00EAC →
            // D1 = mem[$C00EB0].
            //
            // Per project_boing_blitter_mfm_decode_bug.md, D1 = $55D
            // makes the checksum routine read chip $1574 (= $55D << 2)
            // which is K1.3 graphics scratch, producing garbage sum.
            // Identify who writes $55D to slow $C00EB0.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr == 32'h00C0_0EB0)
                $display("[C00EB0-WR] r=%0d pc=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h",
                    retired, ex_pc, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef C04A88_WR_TRACE
            // Probe writes to slow $C04A88..$C04A97 — K1.3 trackdisk
            // IORequest struct at A1=$C04A82.  In our sim these slots
            // are populated with BCPL DOS struct pointers
            // ($C0040C, $C00A80, $C00CA8, $C04730) — FS-UAE has all
            // zeros.  The destination buffer pointer fed to the
            // MFM-decode blit at $FEA992 is read from mem[A1+12]
            // = $C04A8E and resolves to chip $1574 in our sim.
            //
            // Identify the K1.3 ROM PC that writes those non-zero
            // pointers — that is the upstream divergence causing
            // the MFM-decode to land in the wrong chip-RAM region.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_4A88 && dc_addr <= 32'h00C0_4A97)
                $display("[C04A88-WR] r=%0d pc=%h addr=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef CALLER_OF_D2EQ0
            // Capture caller of the BCPL dispatcher at $FF4134 when
            // the dispatch will store 0 at $C00EC8 (mem[A1+$4] at the
            // failing predicate $FFC57A).
            //
            // The dispatcher's MOVEM at $FF413E writes D1,D2,D3,D4 to
            // (A1) where A1 = caller_A1 + D0.  When the resulting A1
            // = $C00EC4 (so that A1+4 = $C00EC8) AND D2 = 0, we've
            // identified THE failing dispatch.
            //
            // Capture all caller-side regs + the BCPL return PC stored
            // at A3 (which $FF4134's MOVEA.L (A7)+,A3 just popped).
            // A3 IS the BCPL return PC (the instruction after the
            // caller's `JSR (A5)`).
            if (is_settled && ex_pc == 32'h00FF_413E &&
                u_rf.regs[9] == 32'h00C0_0EC4 && u_rf.regs[2] == 32'h0000_0000)
                $display("[D2EQ0-CALLER] r=%0d ret_PC=A3=%h  D0=%h D1=%h D2=%h D3=%h D4=%h  A0=%h A2=%h A4=%h A6=%h A7=%h",
                    retired,
                    u_rf.regs[11],  // A3 = popped return PC
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                    u_rf.regs[3], u_rf.regs[4],
                    u_rf.regs[8], u_rf.regs[10], u_rf.regs[12],
                    u_rf.regs[14], u_rf.regs[15]);
`endif
`ifdef FFC57E_PROBE
            // [DOS-ERR-PREDICATE] at $FFC57E:
            //   $FFC578: MOVEQ #29, D2
            //   $FFC57A: CMP.L $4(A1), D2
            //   $FFC57E: BNE $FFC58C  ; if mem[A1+$4] != 29 → load $128 (err 296)
            // Capture A1 and the value of mem[A1+$4] (which is what the
            // packet handler returned in the Res1 slot) plus all regs to
            // identify the predicate input that decides "read/write error".
            // Probe at $FFC57A (CMP) so D2 is set and we see A1.
            if (is_settled && ex_pc == 32'h00FF_C57A)
                $display("[FFC57A-CMP] r=%0d A1=%h D2=%h sp=%h A0=%h A2=%h A4=%h A6=%h D0=%h D1=%h",
                    retired,
                    u_rf.regs[9], u_rf.regs[2], u_rf.regs[15],
                    u_rf.regs[8], u_rf.regs[10], u_rf.regs[12], u_rf.regs[14],
                    u_rf.regs[0], u_rf.regs[1]);
            // Also probe at the BRANCH point: $FFC57E reached, then either
            // $FFC582 (D1=$120 path) or $FFC58C (D1=$128 err 296 path).
            if (is_settled && ex_pc == 32'h00FF_C58C)
                $display("[FFC58C-ERR296] r=%0d A1=%h (mem[A1+\$4]!=29 → DOS err 296)",
                    retired, u_rf.regs[9]);
            if (is_settled && ex_pc == 32'h00FF_C582)
                $display("[FFC582-ERR288] r=%0d A1=%h (mem[A1+\$4]==29 → DOS err 288)",
                    retired, u_rf.regs[9]);
`endif
`ifdef BOING_BUFFER_WR_TRACE
            // Watch CPU writes to the boing.samples destination buffer
            // area at chip $10000-$1F000.  Per the 16th-session boing
            // investigation, boing's task allocates 49896 bytes at
            // chip $11C38, intended to hold the audio sample data.
            // At AUTO-REQ time the buffer is all zeros.
            //
            // If DOS Read() actually copies sector data into the buffer,
            // we should see hundreds of CPU writes here during the
            // r=9M..11M window.  If we see ZERO writes, the copy never
            // happened — failure is before the FFS-to-buffer step.
            //
            // Filtered to events at r > 8.5M (post-boing-alloc moment)
            // to skip pre-alloc noise.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0001_0000 && dc_addr < 32'h0001_F000 &&
                retired >= 32'd8500000) begin
                $display("[BOING-BUF-WR] r=%0d pc=%h addr=%h wdata=%h be=%b",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be);
            end
`endif
`ifdef C00ED0_WR_TRACE
            // Wider version of C00EBC_WR_TRACE covering the real A3
            // source at $C00ED0 plus surrounding dispatch-table slots
            // $C00ECC..$C00F08.  Per the corrected MOVEM-source analysis
            // (project_boing_filesystem_error_handler.md final update),
            // A3 = mem[$C00ED0] at the divergent dispatch.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C00ECC && dc_addr < 32'h00C00F0C)
                $display("[C00ED0-WR] r=%0d pc=%h addr=%h wdata=%h be=%b D0=%h D1=%h D2=%h A0=%h A1=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                    u_rf.regs[8], u_rf.regs[9]);
`endif
`ifdef A3_CHANGE_TO_FFC5A0
            // Catch the exact instruction that sets A3 = $FFC5A0.  Track
            // A3 from the previous cycle; when A3 transitions from
            // anything to $00FFC5A0, log the PC + all regs.
            //
            // Per `project_boing_findtask_returns_rom.md`, the deduction
            // "A3 = FindTask + $5C" was falsified.  $FFC5A0 doesn't
            // appear as a stored longword anywhere.  So A3 must be
            // computed via LEA d(An), A3 or similar.  This probe finds
            // the PC where the computation happens.
            if (is_settled && u_rf.regs[11] == 32'h00FFC5A0 &&
                a3_prev != 32'h00FFC5A0) begin
                $display("[A3-EQ-FFC5A0] r=%0d pc=%h prev_A3=%h",
                    retired, ex_pc, a3_prev);
                $display("  D0=%h D1=%h D2=%h D3=%h D4=%h D5=%h",
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                    u_rf.regs[3], u_rf.regs[4], u_rf.regs[5]);
                $display("  A0=%h A1=%h A2=%h A4=%h A5=%h A6=%h A7=%h",
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[12], u_rf.regs[13], u_rf.regs[14],
                    u_rf.regs[15]);
            end
            if (is_settled) a3_prev <= u_rf.regs[11];
`endif
`ifdef FF474A_FINDTASK_TRACE
            // Capture A6 (= ExecBase) and the FindTask return path
            // right at the JSR FindTask call at $FF474A.  Per the
            // 10-hop boing investigation (project_boing_findtask_
            // returns_rom.md), deduced ThisTask = $FFC50C at this
            // moment, which feeds into the divergent dispatch chain.
            //
            // At PC=$FF474A, A6=ExecBase has just been loaded.  We
            // log A6 + A1 + D0 (FindTask args).  The actual ThisTask
            // value (= mem[A6+$114]) needs a separate readback after
            // the JSR — captured at $FF474E (post-JSR PC, where D0 =
            // FindTask return value).
            if (is_settled && ex_pc == 32'h00ff_474a) begin
                $display("[FF474A-PRE] r=%0d A6=%h (= ExecBase) A1=%h D0=%h SP=%h",
                    retired, u_rf.regs[14], u_rf.regs[9],
                    u_rf.regs[0], u_rf.regs[15]);
            end
            if (is_settled && ex_pc == 32'h00ff_474e) begin
                $display("[FF474A-POST] r=%0d D0=%h (= FindTask result = ThisTask)",
                    retired, u_rf.regs[0]);
            end
`endif
`ifdef FF4134_D1EQ3_TRACE
            // Catch the $FF4134 dispatcher entry where D1 == 3.  That's
            // the call that ends up writing $00000003 to $C00ED8 via the
            // MOVEM at $FF413E.  Dispatcher fires ~1693+ times in
            // ordinary operation, so filter to the specific bad case.
            //
            // At $FF4134, top of stack is the JSR-pushed return PC =
            // caller's "next instruction" PC.  $FF4134 itself begins
            // with MOVEA.L (SP)+, A3 which pops that return PC into A3,
            // but the POP hasn't happened yet when ex_pc == $FF4134.
            if (is_settled && ex_pc == 32'h00ff_4134 &&
                u_rf.regs[1] == 32'h00000003) begin
                $display("[FF4134-D1EQ3] r=%0d D1=3 — caller frame:",
                         retired);
                $display("  D0=%h D1=%h D2=%h D3=%h D4=%h",
                         u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                         u_rf.regs[3], u_rf.regs[4]);
                $display("  A0=%h A1=%h A2=%h A3=%h A4=%h A5=%h A6=%h A7=%h",
                         u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                         u_rf.regs[11], u_rf.regs[12], u_rf.regs[13],
                         u_rf.regs[14], u_rf.regs[15]);
                $display("  ring (last 16 PCs leading to dispatcher entry):");
                $display("    %h %h %h %h",
                         pc_ring[ring_head + 4'd1],
                         pc_ring[ring_head + 4'd2],
                         pc_ring[ring_head + 4'd3],
                         pc_ring[ring_head + 4'd4]);
                $display("    %h %h %h %h",
                         pc_ring[ring_head + 4'd5],
                         pc_ring[ring_head + 4'd6],
                         pc_ring[ring_head + 4'd7],
                         pc_ring[ring_head + 4'd8]);
                $display("    %h %h %h %h",
                         pc_ring[ring_head + 4'd9],
                         pc_ring[ring_head + 4'd10],
                         pc_ring[ring_head + 4'd11],
                         pc_ring[ring_head + 4'd12]);
                $display("    %h %h %h %h",
                         pc_ring[ring_head + 4'd13],
                         pc_ring[ring_head + 4'd14],
                         pc_ring[ring_head + 4'd15],
                         pc_ring[ring_head]);
            end
`endif
`ifdef C00ED8_WR_TRACE
            // Watch every CPU write to slow $C00ED8 (the DOS-packet
            // struct field0 whose value triggers the divergent boing-
            // disk AutoRequest path).  Our sim writes $00000003 here;
            // FS-UAE has $0030031D.  Find the responsible instruction.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0ED8 && dc_addr < 32'h00C0_0EE0)
                $display("[C00ED8-WR] r=%0d pc=%h addr=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h A6=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[14]);
            // Also watch slow $C00EC4..$C00ECF — the predicate input
            // (mem[A1+$4] at $FFC57A where A1=$C00EC4).  This is what
            // gets compared to 29; if !=29 → DOS error 296 dialog.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0EC4 && dc_addr < 32'h00C0_0ED0)
                $display("[C00EC4-WR] r=%0d pc=%h addr=%h wdata=%h be=%b D0=%h D1=%h A0=%h A1=%h A6=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[14]);
`endif
`ifdef FF5364_PROBE
            // Capture A2 at $FF5364: MOVEA.L $FF7C(A2), A4.  The struct
            // field at offset -$84 from A2 holds the function pointer
            // that gets dispatched (eventually leading to AutoRequest).
            // Compare A2 + the loaded value to FS-UAE-no-UAEFS at the
            // same boot moment to find the upstream divergence.
            if (is_settled && ex_pc == 32'h00ff_5364) begin
                $display("[FF5364] r=%0d A2=%h  D0=%h D1=%h D2=%h A0=%h A1=%h A5=%h A6=%h A7=%h",
                    retired,
                    u_rf.regs[10],  // A2
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                    u_rf.regs[8], u_rf.regs[9],
                    u_rf.regs[13], u_rf.regs[14], u_rf.regs[15]);
            end
            // Same instruction, but capture A4 just AFTER it loaded —
            // we'll see what value was at mem[A2 + $FF7C].  $FF5368 is
            // the next PC ($FF5364 is 4 bytes), so probe there.
            if (is_settled && ex_pc == 32'h00ff_5368) begin
                $display("[FF5368] r=%0d A4=%h (loaded from A2+\$FF7C)",
                    retired, u_rf.regs[12]);
            end
            // Capture caller of the $FF5354 routine entry: dump ring +
            // registers + 4 words of stack so we identify the upstream
            // dispatcher selection that landed us here.
            if (is_settled && ex_pc == 32'h00ff_5354) begin
                $display("[FF5354-ENTRY] r=%0d A1=%h A2=%h A5=%h A6=%h A7=%h",
                    retired,
                    u_rf.regs[9], u_rf.regs[10], u_rf.regs[13],
                    u_rf.regs[14], u_rf.regs[15]);
                $display("  ring (last 16 retired PCs leading to entry):");
                $display("    [-15..-12]=%h %h %h %h",
                         pc_ring[ring_head + 4'd1],
                         pc_ring[ring_head + 4'd2],
                         pc_ring[ring_head + 4'd3],
                         pc_ring[ring_head + 4'd4]);
                $display("    [-11..-8] =%h %h %h %h",
                         pc_ring[ring_head + 4'd5],
                         pc_ring[ring_head + 4'd6],
                         pc_ring[ring_head + 4'd7],
                         pc_ring[ring_head + 4'd8]);
                $display("    [-7..-4]  =%h %h %h %h",
                         pc_ring[ring_head + 4'd9],
                         pc_ring[ring_head + 4'd10],
                         pc_ring[ring_head + 4'd11],
                         pc_ring[ring_head + 4'd12]);
                $display("    [-3..-0]  =%h %h %h %h",
                         pc_ring[ring_head + 4'd13],
                         pc_ring[ring_head + 4'd14],
                         pc_ring[ring_head + 4'd15],
                         pc_ring[ring_head]);
            end
`endif
`ifdef FF4D1C_RINGBUF
            // Dump ring buffer + log SP/mem[SP] for $FF4D00 entry too
            // — that's the routine's ACTUAL caller, since $FF4D02 is the
            // first instruction.  Distinct from $FF4D1C trigger which
            // captures the chain inside the routine.
            if (is_settled && ex_pc == 32'h00ff_4d02) begin
                $display("[FF4D02-ENTRY] r=%0d FF4D00 routine entered — SP=%h A0=%h A1=%h D0=%h",
                    retired, u_rf.regs[15],
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[0]);
                $display("  ring (last 16 PCs leading to entry):");
                $display("    [-15]=%h [-14]=%h [-13]=%h [-12]=%h",
                         pc_ring[ring_head + 4'd1],
                         pc_ring[ring_head + 4'd2],
                         pc_ring[ring_head + 4'd3],
                         pc_ring[ring_head + 4'd4]);
                $display("    [-11]=%h [-10]=%h [-9]=%h [-8]=%h",
                         pc_ring[ring_head + 4'd5],
                         pc_ring[ring_head + 4'd6],
                         pc_ring[ring_head + 4'd7],
                         pc_ring[ring_head + 4'd8]);
                $display("    [-7]=%h  [-6]=%h  [-5]=%h [-4]=%h",
                         pc_ring[ring_head + 4'd9],
                         pc_ring[ring_head + 4'd10],
                         pc_ring[ring_head + 4'd11],
                         pc_ring[ring_head + 4'd12]);
                $display("    [-3]=%h  [-2]=%h  [-1]=%h [-0]=%h",
                         pc_ring[ring_head + 4'd13],
                         pc_ring[ring_head + 4'd14],
                         pc_ring[ring_head + 4'd15],
                         pc_ring[ring_head]);
            end
            // PC ring buffer: capture the last 16 PCs, dump them when PC
            // reaches $FF4D1C (boing-disk AUTO-REQ entry).  Used to find
            // the BCPL/DOS caller chain that decides "show error dialog".
            // See project_boing_filesystem_error_handler.md for the
            // forensics context — $FF4D0C/$FF4D1C has zero static callers
            // in K1.3 ROM, so the entry must come via runtime call.
            if (is_settled && ex_pc == 32'h00ff_4d1c) begin
                $display("[FF4D1C-RING] r=%0d hit AUTO-REQ entry — last 16 PCs:",
                         retired);
                $display("  [-15]=%h [-14]=%h [-13]=%h [-12]=%h",
                         pc_ring[ring_head + 4'd1],
                         pc_ring[ring_head + 4'd2],
                         pc_ring[ring_head + 4'd3],
                         pc_ring[ring_head + 4'd4]);
                $display("  [-11]=%h [-10]=%h [-9]=%h [-8]=%h",
                         pc_ring[ring_head + 4'd5],
                         pc_ring[ring_head + 4'd6],
                         pc_ring[ring_head + 4'd7],
                         pc_ring[ring_head + 4'd8]);
                $display("  [-7]=%h  [-6]=%h  [-5]=%h [-4]=%h",
                         pc_ring[ring_head + 4'd9],
                         pc_ring[ring_head + 4'd10],
                         pc_ring[ring_head + 4'd11],
                         pc_ring[ring_head + 4'd12]);
                $display("  [-3]=%h  [-2]=%h  [-1]=%h [-0]=%h",
                         pc_ring[ring_head + 4'd13],
                         pc_ring[ring_head + 4'd14],
                         pc_ring[ring_head + 4'd15],
                         pc_ring[ring_head]);
                $display("  SP=%h", u_rf.regs[15]);
            end
            // Update the ring on every settled instruction retire.
            if (is_settled) begin
                ring_head <= ring_head + 4'd1;
                pc_ring[ring_head + 4'd1] <= ex_pc;
            end
`endif
`ifdef SLOW_C07138_WR_TRACE
            // Trace every write to slow RAM $C07138 — the struct field
            // that supplies the bad BLTAPT high-half $0280 (per §57e).
            // Identifies the routine that initializes this field correctly.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_7138 && dc_addr <= 32'h00C0_713B)
                $display("[SLOW_C07138-WR] r=%d pc=%h addr=%h wdata=%h be=%b A1=%h",
                    retired, ex_pc, dc_addr, dc_wdata, dc_be, u_rf.regs[9]);
`endif
`ifdef KICKSTART_TRACKDISK_TRACE
            // Wider window trace: K1.3 trackdisk decoder PCs during the
            // validation window.  Open r=2.0M..5.0M after the snoop fix
            // shifted retired counts.
            if (is_settled && ex_kind != K_STOP &&
                retired >= 32'd2000000 && retired <= 32'd20000000 &&
                ex_pc >= 32'h00fe_ac00 && ex_pc <= 32'h00fe_b000)
                $display("[TRKPC] r=%d pc=%h kind=%d", retired, ex_pc, ex_kind);
            // Trace each CMP.L (A1)+,D1 in $FEABCC's pattern-search inner
            // loop.  Shows D1 (= 4 bytes from buffer at A0) and A1's prior
            // value (i.e. the pattern entry being compared this iter).
            if (is_settled && ex_pc == 32'h00fe_ac06)
                $display("[SYNCSCAN] r=%d D1=%h A1=%h D0=%h A0=%h",
                    retired, u_rf.regs[1], u_rf.regs[9], u_rf.regs[0], u_rf.regs[8]);
            // Trace successful exit at $FEAC10 (match found).
            if (is_settled && ex_pc == 32'h00fe_ac10)
                $display("[SYNCSCAN] r=%d MATCH A0=%h", retired, u_rf.regs[8]);
            // Trace $FEABCC entry to see what scan range was set up.
            if (is_settled && ex_pc == 32'h00fe_abcc)
                $display("[SYNCSCAN] r=%d $FEABCC ENTER A0=%h D0=%h", retired, u_rf.regs[8], u_rf.regs[0]);
            // hdr_chk compare site ($FEACF8: CMP.L D0,D6)
            if (is_settled && ex_pc == 32'h00fe_acf8)
                $display("[HDRCHK] r=%d t=%0t D6=%h D0=%h (expect equal)", retired, $time, u_rf.regs[6], u_rf.regs[0]);
            // D2==0 path: hdr_chk pair read at $FEACB2/$FEACB6.
            if (is_settled && ex_pc == 32'h00fe_acb2)
                $display("[HDRCHK_RD] r=%d t=%0t D2=0 path: pre hdr_chk_odd read A0=%h A4=%h", retired, $time, u_rf.regs[8], u_rf.regs[12]);
            if (is_settled && ex_pc == 32'h00fe_acb6)
                $display("[HDRCHK_RD] r=%d D2=0 path: post-odd A0=%h", retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00fe_acba)
                $display("[HDRCHK_RD] r=%d D2=0 path: post-even A0=%h", retired, u_rf.regs[8]);
            // $FEAA0E entry — combines odd/even pair: D0 = (odd << 1) | even.
            if (is_settled && ex_pc == 32'h00fe_aa0e)
                $display("[MFM_COMBINE] r=%d ENTER A0=%h", retired, u_rf.regs[8]);
            // After MOVE.L (A0)+, D0 at $FEAA0E (so next PC = $FEAA10)
            if (is_settled && ex_pc == 32'h00fe_aa10)
                $display("[MFM_COMBINE] r=%d post-load-odd D0=%h A0=%h", retired, u_rf.regs[0], u_rf.regs[8]);
            // After MOVE.L (A0)+, D1 at $FEAA10 (so next PC = $FEAA12)
            if (is_settled && ex_pc == 32'h00fe_aa12)
                $display("[MFM_COMBINE] r=%d post-load-even D0=%h D1=%h", retired, u_rf.regs[0], u_rf.regs[1]);
            // After OR.L D1, D0 at $FEAA20 (so next PC = $FEAA22 RTS)
            if (is_settled && ex_pc == 32'h00fe_aa22)
                $display("[MFM_COMBINE] r=%d RTS D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00fe_acfa)
                $display("[HDRCHK] r=%d post-CMP CC bits (ccr next)", retired);
            // Format byte check ($FEAD10: BNE if format != $FF)
            if (is_settled && ex_pc == 32'h00fe_ad08)
                $display("[INFO] r=%d info decoded D3=%h", retired, u_rf.regs[3]);
            // Track number check ($FEAD1C: BNE if track != $4B(A3))
            if (is_settled && ex_pc == 32'h00fe_ad18)
                $display("[TRACK] r=%d cmp D0=%h vs $4B(A3) (A3=%h)", retired, u_rf.regs[0], u_rf.regs[11]);
            // Error path ($FEAE6C) — TDERR_BadHdrSum or similar
            if (is_settled && ex_pc == 32'h00fe_ae6c)
                $display("[BADSEC] r=%d error path D0=%h D3=%h D6=%h A0=%h", retired, u_rf.regs[0], u_rf.regs[3], u_rf.regs[6], u_rf.regs[8]);
            // Other validator error paths (all set D0=TDERR_* and BRA to $FEAE64)
            if (is_settled && ex_pc == 32'h00fe_ae68)
                $display("[TDERR21] r=%d NoSync A0=%h A2=%h D2=%h", retired, u_rf.regs[8], u_rf.regs[10], u_rf.regs[2]);
            if (is_settled && ex_pc == 32'h00fe_ae70)
                $display("[TDERR22] r=%d BadSecPreamble A0=%h A2=%h", retired, u_rf.regs[8], u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00fe_ae74)
                $display("[TDERR23] r=%d D0=23 A0=%h A2=%h", retired, u_rf.regs[8], u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00fe_ae78)
                $display("[TDERR24] r=%d D0=24 A0=%h A2=%h", retired, u_rf.regs[8], u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00fe_ae7c)
                $display("[TDERR25] r=%d BadSecSum A0=%h A2=%h", retired, u_rf.regs[8], u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00fe_ae80)
                $display("[TDERR26] r=%d TooFewSecs A0=%h A2=%h", retired, u_rf.regs[8], u_rf.regs[10]);
            // strap.task bootblock-load checks at $FE85A0..$FE85AA.
            if (is_settled && ex_pc == 32'h00fe_85a0)
                $display("[STRAP] r=%d post DoIO Read: D0=%h (io_Error)", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00fe_85a4)
                $display("[STRAP] r=%d about to read (A4): A4=%h", retired, u_rf.regs[12]);
            if (is_settled && ex_pc == 32'h00fe_85a6)
                $display("[STRAP] r=%d D0=%h (= first long of bootblock buffer)", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00fe_85aa)
                $display("[STRAP] r=%d post-CMP magic check (expect DOS\\0=$444F5300)", retired);
            if (is_settled && ex_pc == 32'h00fe_85ac)
                $display("[STRAP] r=%d magic check PASSED -- proceed with checksum/JSR", retired);
            if (is_settled && ex_pc == 32'h00fe_8600)
                $display("[STRAP] r=%d ERROR path entered (read failed or magic mismatch) D0=%h", retired, u_rf.regs[0]);
            // Trace OpenLibrary's strlen loop ($FC52B4 / $FC52BA): see what
            // string pointer it was handed.  K1.3's strlen reads from A0.
            // For WB1.3 bootblock the expected ptr is the "dos.library"
            // string inside the bootblock buffer (~$55FE).
            if (is_settled && ex_pc == 32'h00fc_52b4)
                $display("[STRLEN] r=%d ENTER A0=%h A1=%h D0=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00fc_52c4)
                $display("[STRLEN] r=%d EXIT D0=%h", retired, u_rf.regs[0]);
            // strap.task post-bootblock dispatch ($FE85CA = TST.L D0 after
            // bootblock RTS).  Bootblock returns D0=0 for success (A0 =
            // dos.library Resident's rt_Init pointer).  $FE85F0 = success
            // path (JSR $FE8C9C).  $FE85F8 = post-JSR continuation.
            if (is_settled && ex_pc == 32'h00fe_85ca)
                $display("[STRAP] r=%d post-JSR bootblock returned D0=%h A0=%h",
                    retired, u_rf.regs[0], u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00fe_85f0)
                $display("[STRAP] r=%d success branch entered A0=%h",
                    retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00fe_8c9c)
                $display("[STRAP] r=%d JSR \\$FE8C9C entered A0=%h", retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00fe_85f8)
                $display("[STRAP] r=%d post-JSR-FE8C9C continuation D0=%h A0=%h",
                    retired, u_rf.regs[0], u_rf.regs[8]);
            // strap.task's final JMP into dos.library's rt_Init (post-cleanup).
            // $FE86EC = JMP (A0).  A0 carries the rt_Init from the bootblock.
            if (is_settled && ex_pc == 32'h00fe_86ec)
                $display("[STRAP] r=%d JMP into dos.library rt_Init A0=%h",
                    retired, u_rf.regs[8]);
            // Reached dos.library rt_Init code at \$FF3E94 (or whatever
            // rt_Init the bootblock surfaced).  Catch any execution in the
            // ROM Resident's init region.
            if (is_settled && ex_pc >= 32'h00ff_3e62 &&
                ex_pc <= 32'h00ff_3fff && !dos_init_seen) begin
                $display("[DOSINIT] r=%d ENTER pc=%h A0=%h D0=%h",
                    retired, ex_pc, u_rf.regs[8], u_rf.regs[0]);
                dos_init_seen <= 1'b1;
            end
            // BadSecID branches: 3 checks after MFM data decode (format
            // byte, track byte, sector byte).  Trace each check + the
            // post-MFM-combine info to see which sector test fails.
            if (is_settled && ex_pc == 32'h00fe_addc)
                $display("[SECID] r=%d post-combine info D0=%h A0=%h D4=%h D5=%h A2=%h", retired, u_rf.regs[0], u_rf.regs[8], u_rf.regs[4], u_rf.regs[5], u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00fe_ade6)
                $display("[SECID] r=%d format-check BNE D0=%h D4=%h D5=%h (expect top byte=$FF)", retired, u_rf.regs[0], u_rf.regs[4], u_rf.regs[5]);
            if (is_settled && ex_pc == 32'h00fe_adee)
                $display("[SECID] r=%d format OK; track-check D1=%h vs $4B(A3=%h)", retired, u_rf.regs[1], u_rf.regs[11]);
            if (is_settled && ex_pc == 32'h00fe_adf6)
                $display("[SECID] r=%d track OK; sector-check D1=%h vs D5=%h", retired, u_rf.regs[1], u_rf.regs[5]);
            if (is_settled && ex_pc == 32'h00fe_ae00)
                $display("[SECID] r=%d ALL checks passed (sector data accepted) D5=%h", retired, u_rf.regs[5]);
            if (is_settled && ex_pc == 32'h00fe_ae74)
                $display("[SECID] r=%d BadSecID error -- one of format/track/sector checks failed", retired);
            // Preamble check: $FEAD9A reads buffer for MFM gap pattern.
            if (is_settled && ex_pc == 32'h00fe_ad9a)
                $display("[PREAMBLE] r=%d preamble check A2=%h D4=%h (read \$00(A2,D4) for \$AAAA pattern)", retired, u_rf.regs[10], u_rf.regs[4]);
            // Trace CPU read result for the CMPI.L access to (A2,D4).
            // Widen range to catch any nearby reads.
            if (dc_req_r && dc_ack &&
                dc_addr >= 32'h0000_68E0 && dc_addr <= 32'h0000_68FB)
                $display("[CPU_RD] r=%d pc=%h we=%b addr=%h dc_rdata=%h", retired, ex_pc, dc_we, dc_addr, dc_rdata);
            if (is_settled && ex_pc == 32'h00fe_adac)
                $display("[PREAMBLE] r=%d after \$AAAAAAAA check (dc_rdata=%h) -- if BNE -> BadSecPreamble", retired, dc_rdata);
            // Trace EVERY CMPI.L at \$FEAD9A and \$FEADA4 with the read value.
            if (is_settled && (ex_pc == 32'h00fe_ada2 || ex_pc == 32'h00fe_adac))
                $display("[PREAMBLE_RD] r=%d pc=%h dc_addr=%h dc_rdata=%h", retired, ex_pc, dc_addr, dc_rdata);
            if (is_settled && ex_pc == 32'h00fe_adb0)
                $display("[PREAMBLE] r=%d preamble OK -- checking sync next", retired);
            if (is_settled && ex_pc == 32'h00fe_ae70)
                $display("[PREAMBLE] r=%d BadSecPreamble error path entered", retired);
            // Trace CPU writes to the local-var slots $5DC8 / $5DCC where the
            // hdr_chk pair is staged before $FEAA0E.  Identifies any code
            // path that scribbles over the local between $FEACB6 and the
            // MFM-combine read.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_5DC8 && dc_addr <= 32'h0000_5DCF)
                $display("[HDRCHK_LOC_PC] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch CPU writes that hit the disk-buffer hdr_chk pair area
            // ($64DC..$64E7).  Find whoever clobbers it between DMA refills.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_64DC && dc_addr <= 32'h0000_64E7)
                $display("[HDRCHK_BUF_PC] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
`endif
            // [BAD-BLTDPT] CPU writes to BLTDPT ($DFF054/$DFF056) with a
            // destination value in low chip RAM (under \$10000).  Captures
            // the PC + value + register state so we can find which
            // routine in our system is computing the bogus destinations
            // that land at chip \$C2xx (corrupting the task stack at
            // \$C2CC).  Always-on (independent of any BOOT_TRACE define).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F054 && dc_addr <= 32'h00DF_F057 &&
                dc_wdata < 32'h0001_0000)
                $display("[BAD-BLTDPT] r=%d pc=%h wdata=%h A0=%h A1=%h A3=%h A4=%h D5=%h D6=%h D7=%h",
                    retired, ex_pc, dc_wdata,
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[11], u_rf.regs[12],
                    u_rf.regs[5], u_rf.regs[6], u_rf.regs[7]);
            // [MH-ATTRS] FS-UAE-confirmed: PC \$FC1A38 is
            //   `MOVE.W D1, \$0E(A0)`
            // — the exec.AddMemHeader helper that writes mh_Attributes.
            // On FS-UAE, the chip-RAM call has D1=\$0003
            // (MEMF_PUBLIC|MEMF_CHIP).  Our system ends up with the chip
            // header at \$00000400 carrying attrs=\$0000, which makes
            // AllocMem(MEMF_CHIP) fall through to slow RAM and breaks the
            // entire bitmap-allocation/task-stack layout (project
            // wb13_displays).  Capture D1 + A0 to see if the caller is
            // passing the wrong value, or if MOVE.W is dropping the
            // store.  Always-on.
            if (is_settled && ex_pc == 32'h00fc_1a38)
                $display("[MH-ATTRS] r=%d enter A0=%h D1=%h D2=%h",
                    retired, u_rf.regs[8], u_rf.regs[1], u_rf.regs[2]);
            // [BLT-SETUP] Capture register state at the FS-UAE-confirmed
            // graphics.library blit-setup loop body at PC \$FC5DF2 /
            // \$FC5E04 — the routine that writes BLTAPT/BLTCPT/BLTDPT/
            // BLTSIZE.  On FS-UAE this loop runs with *A3 = \$60C8 /
            // \$B0C8 (correct screen Planes[]).  On our system this PC
            // may not be reached at all — in which case our boot uses a
            // different drawing routine that we then need to find.
            if (is_settled && ex_pc == 32'h00fc_5df2)
                $display("[BLT-SETUP] r=%d enter loop A1=%h A2=%h A3=%h D0=%h D4=%h",
                    retired, u_rf.regs[9], u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[0], u_rf.regs[4]);
            if (is_settled && ex_pc == 32'h00fc_5e04)
                $display("[BLT-SETUP] r=%d post-A4-compute A4=%h A3=%h D4=%h",
                    retired, u_rf.regs[12], u_rf.regs[11], u_rf.regs[4]);
`ifdef KICKSTART_BOOT_TRACE
            // Edge-triggered watch for user-mode A7 dipping below $200.
            // Catches tasks running with a smashed stack pointer.  Kept
            // after the rc_idx exception-capture bug fix as a guardrail.
            if (is_settled && !sr_s && ex_kind != K_STOP &&
                u_rf.regs[15] < 32'h0000_0200 && !user_a7_was_tiny) begin
                $display("[USPTINY] r=%d pc=%h opcode=%h kind=%d A7=%h",
                    retired, ex_pc, ex_opcode, ex_kind, u_rf.regs[15]);
                user_a7_was_tiny <= 1'b1;
            end
            if (is_settled && !sr_s && ex_kind != K_STOP &&
                u_rf.regs[15] >= 32'h0000_0200 && user_a7_was_tiny) begin
                user_a7_was_tiny <= 1'b0;
            end
            // Same for supervisor mode (SSP).  If SSP dips below $200
            // an exception frame will land on top of the vector table —
            // that's the post-TOD-fix WB1.3 boot wall.
            if (is_settled && sr_s && ex_kind != K_STOP &&
                u_rf.regs[15] < 32'h0000_0200 && !super_a7_was_tiny) begin
                $display("[SSPTINY] r=%d pc=%h opcode=%h kind=%d SSP=%h",
                    retired, ex_pc, ex_opcode, ex_kind, u_rf.regs[15]);
                super_a7_was_tiny <= 1'b1;
            end
            if (is_settled && sr_s && ex_kind != K_STOP &&
                u_rf.regs[15] >= 32'h0000_0200 && super_a7_was_tiny) begin
                super_a7_was_tiny <= 1'b0;
            end
`endif
`ifdef KICKSTART_BOOT_TRACE
            // STOP logs once when it first settles -- without the edge
            // detect, K_STOP's is_settled stays high every cycle of the
            // halt and floods the log (76M [STOP] lines in a 200M-cycle
            // run).  `stop_logged_pc` remembers the last PC that fired
            // the trace; we re-fire only when PC changes (= entered a
            // new STOP).
            if (ex_kind == K_STOP && is_settled && ex_pc != stop_logged_pc) begin
                $display("[STOP] r=%d pc=%h imm=%h",
                    retired, ex_pc, ex_src_imm32[15:0]);
                stop_logged_pc <= ex_pc;
            end
            // Sample A5 each cycle so the [A5-CHANGE] edge trace below
            // sees the previous value.
            a5_last_r <= u_rf.regs[13];
            // [OPENSCREEN]: trace entry to intuition.library OpenScreen.
            // LVO entry at $FDFCE8 (JT target from $C03D24 - 198).  Body
            // at $FDC91A.  WB1.3 boot reaches r=4.4M idle with BPLCON0=0
            // — if this trace never fires, no task requested a screen.
            // If it fires but BPLCON0 stays 0, OpenScreen is failing
            // internally (AllocBitMap returning NULL? bitmap dst in bad
            // region?).
            if (is_settled && ex_pc == 32'h00fd_fce8)
                $display("[OPENSCREEN-LVO] r=%d entry sp=%h a0=%h a6=%h",
                    retired, u_rf.regs[15], u_rf.regs[8], u_rf.regs[14]);
            // graphics.library functions (correct LVO entry points).
            if (is_settled && ex_pc == 32'h00fc_5b40)
                $display("[GFX-MAKEVPORT] r=%d a0=%h a1=%h", retired, u_rf.regs[8], u_rf.regs[9]);
            if (is_settled && ex_pc == 32'h00fc_5b4c)
                $display("[GFX-MRGCOP] r=%d a1=%h a6=%h", retired, u_rf.regs[9], u_rf.regs[14]);
            // MrgCop's actual body (LVO wrapper at $FC5B4C calls $FCD3CC).
            if (is_settled && ex_pc == 32'h00fc_d3cc)
                $display("[GFX-MRGCOP-BODY] r=%d sp=%h", retired, u_rf.regs[15]);
            // MrgCop call sites (none in Intuition OpenScreen body).
            if (is_settled && ex_pc == 32'h00fe_8898)
                $display("[MRGCOP-CS] r=%d at $FE8898", retired);
            if (is_settled && ex_pc == 32'h00fe_8976)
                $display("[MRGCOP-CS] r=%d at $FE8976", retired);
            if (is_settled && ex_pc == 32'h00ff_355e)
                $display("[MRGCOP-CS] r=%d at $FF355E", retired);
            if (is_settled && ex_pc == 32'h00ff_3c1e)
                $display("[MRGCOP-CS] r=%d at $FF3C1E", retired);
            if (is_settled && ex_pc == 32'h00fc_637c)
                $display("[GFX-LOADVIEW] r=%d a1=%h a6=%h",
                    retired, u_rf.regs[9], u_rf.regs[14]);
            if (is_settled && ex_pc == 32'h00fc_65ec)
                $display("[GFX-INITVIEW] r=%d a1=%h", retired, u_rf.regs[9]);
            // After LoadView assembles + writes COP1LC, the next COP1LC
            // write should reflect View.LOFCprList. Trace the value just
            // before COP1LC write.
            if (is_settled && ex_pc == 32'h00fd_c91a)
                $display("[OPENSCREEN-BODY] r=%d entry sp=%h a0=%h",
                    retired, u_rf.regs[15], u_rf.regs[8]);
            // OpenScreen body's RTS at $FDCDB8 — capture return value (D0).
            if (is_settled && ex_pc == 32'h00fd_cdb8)
                $display("[OPENSCREEN-EXIT] r=%d D0=%h a0=%h",
                    retired, u_rf.regs[0], u_rf.regs[8]);
            // Also trace the various other exits inside OpenScreen body to
            // see if any error-exit path fires before the main RTS.
            if (is_settled && ex_pc == 32'h00fd_cda0)
                $display("[OPENSCREEN-EXIT-PATH] r=%d at $FDCDA0", retired);
            // [OPENSCREEN-RETURN]: trace return into the LVO wrapper at
            // $FDFCF0 (after the JSR $FDC91A returns).  If RTS at $FDCDB8
            // fired with D0=0 (failure), $FDFCF0 also gets D0=0.
            if (is_settled && ex_pc == 32'h00fd_fcf0)
                $display("[OPENSCREEN-LVO-RET] r=%d D0=%h",
                    retired, u_rf.regs[0]);
            // [STRSCAN]: trace $FE4ADC strlen helper used by WB1.3 FS task.
            // After CLI's PUTMSG to FS port at \$C00ADC, FS dequeues the
            // packet and ends up scanning a string for >275K cycles before
            // PutMsg'ing to ciaa.resource (which is NOT a port).  Capture A0
            // at $FE4AD0 entry to find what string is being scanned, and
            // D0 at exit to count chars consumed.
            if (is_settled && ex_pc == 32'h00fe_4adc)
                $display("[STRSCAN-IN] r=%d sp=%h",
                    retired, u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00fe_4ad0)
                $display("[STRSCAN-AD0] r=%d A0=%h",
                    retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00fe_4ada)
                $display("[STRSCAN-OUT] r=%d D0=%h", retired, u_rf.regs[0]);
            // Trace CPU writes to Copper-list-pointer regs (COP1LC / COP2LC).
            // Useful to see when (if) Intuition.OpenScreen installs a new
            // screen Copper list, which on a successful boot screen would
            // appear at $DFF080 after Kickstart's initial boot list.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F080 && dc_addr <= 32'h00DF_F087)
                $display("[CPUCOP] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [CPUINT]: CPU writes to INTENA ($DFF09A) or INTREQ ($DFF09C).
            // Captures the PC of every Disable/Enable so we can identify
            // which call site is leaking the +3% CLR/SET imbalance that
            // wedges WB1.3 boot at the trackdisk Wait($400).
            if (dc_req_r && dc_we && dc_ack &&
                (dc_addr == 32'h00DF_F098 || dc_addr == 32'h00DF_F09A ||
                 dc_addr == 32'h00DF_F09C))
                $display("[CPUINT] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [CPUBLT]: CPU writes to the canonical blitter pointer
            // regs $DFF048..$DFF057.  Captures the PC of every BLTAPT/BPT/
            // CPT/DPT write so we can find who set bltdpt=$01FF (the
            // value that makes the blitter walk through the vector table).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F048 && dc_addr <= 32'h00DF_F057)
                $display("[CPUBLT] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [SPRPT-WR]: ANY CPU write to $DFF120..$DFF13F (8 sprite
            // pointer pairs).  Tells us whether Intuition's sprite
            // cursor renderer ever fires.  WB1.3 idle shows Workbench
            // with no mouse cursor — confirm SPR*PT are never set.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F120 && dc_addr <= 32'h00DF_F13F)
                $display("[SPRPT-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);

            // [BLTMOD-WR]: ALL CPU writes to BLTAMOD/BMOD/CMOD/DMOD
            // ($DFF060..$DFF067).  Filtered to the window around the
            // corrupting blit at r=4,196,883 to keep the log small.
            // Captures PC + wdata so we can trace which write set
            // bltdmod=-90 for the corrupting blit.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F060 && dc_addr <= 32'h00DF_F067 &&
                retired >= 32'd4_180_000 && retired <= 32'd4_200_000)
                $display("[BLTMOD-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
`ifdef BLTAMOD_FF2E_TRACE
            // [BLTAMOD-FF2E]: catch the CPU write that puts $FF2E or
            // sign-extended $FFFFFF2E into BLTAMOD ($DFF064).
            // Boing-disk: blit #15 to chip $1574 has amod=$FFFFFF2E
            // while blits 1-14 had amod=0 — find who set it.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F060 && dc_addr <= 32'h00DF_F067 &&
                (dc_wdata[15:0] == 16'hFF2E ||
                 dc_wdata[31:16] == 16'hFF2E))
                $display("[BLTAMOD-FF2E] r=%d pc=%h addr=%h be=%b wdata=%h A0=%h A1=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata,
                    u_rf.regs[8], u_rf.regs[9]);
            // Also catch all BLTAMOD/BLTDMOD writes in the window of
            // r=10_500_000..10_520_000 (15-blit failing window).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00DF_F060 && dc_addr <= 32'h00DF_F067 &&
                retired >= 32'd10_500_000 && retired <= 32'd10_520_000)
                $display("[BLTMOD-WIN] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
`endif
            // [BLT-DST-SRC]: probe registers right after the MOVEM.L $8(A1)
            // at $FEA97A that loads D0/D1/A5 from the struct.  This is the
            // graphics primitive that computes BLTDPT = D1 + $200 - 1.
            // Dumping A1 + the post-MOVEM values lets us see whether the
            // struct field really holds 0 (legitimate K1.3 behaviour),
            // or whether the bus read returned 0 erroneously.
            if (is_settled && (ex_pc == 32'h00fe_a980 ||
                               ex_pc == 32'h00fe_a988 ||
                               ex_pc == 32'h00fe_a98e))
                $display("[BLT-DST-SRC] r=%d pc=%h D0=%h D1=%h A1=%h A5=%h",
                    retired, ex_pc, u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[9], u_rf.regs[13]);
            // [STKSET-906]: dump regs at the outer graphics-prep entry
            // $FEA906 — that function's caller passes A0; A0 then flows
            // through to $FEA932 → MOVEM save at $FEA936.  Comparing A0
            // across calls finds the one where it became 0 (the bug
            // origin upstream).
            if (is_settled && ex_pc == 32'h00fe_a906)
                $display("[STKSET-906] r=%d A0=%h A1=%h A2=%h A6=%h SP=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[14], u_rf.regs[15]);
            // [STKSET-932]: same at $FEA932 (the LINK A2,#-$1E that
            // creates the frame which $FEA936 then saves into).  Includes
            // D0 (= what gets saved at $C04A8E by MOVEM at $FEA936) so we
            // can see directly whether the caller passed D0=0 or whether
            // D0 was clobbered between MOVE.L #imm,D0 and the BSR.
            if (is_settled && ex_pc == 32'h00fe_a932)
                $display("[STKSET-932] r=%d D0=%h A0=%h A1=%h A2=%h A6=%h SP=%h ret=%h",
                    retired,
                    u_rf.regs[0], u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[14], u_rf.regs[15], u_rf.regs[15]);
            // [LSR-FC7C86]: detection probe at PC=$FC7C86 (the LSR.W
            // $10(A1) site flagged by the original K_SHIFT investigation).
            // If our boot path never reaches this PC, K_SHIFT fix doesn't
            // help.  Dumps D1 so we can compare to expected vs corrupted.
            if (is_settled && ex_pc == 32'h00fc_7c86)
                $display("[LSR-FC7C86] r=%d D1=%h A1=%h",
                    retired, u_rf.regs[1], u_rf.regs[9]);
            // [STKSET-970]: same at $FEA970 (the reader — A1 source of
            // the struct).
            if (is_settled && ex_pc == 32'h00fe_a970)
                $display("[STKSET-970] r=%d A0=%h A1=%h A2=%h A6=%h SP=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[14], u_rf.regs[15]);
            // [PC-TRACE-4M]: dump PC + A0 for the 50 instructions
            // leading up to the bad-blit $FEA932 entry at retired
            // ≈ 4436232.  Pinpoints the exact CALL site that passes
            // A0=0 instead of a Y-coord.
            if (is_settled && retired >= 32'd4436180 && retired <= 32'd4436240)
                $display("[PC-TRACE-4M] r=%d pc=%h opcode=%h A0=%h A1=%h A3=%h A4=%h",
                    retired, ex_pc, ex_opcode, u_rf.regs[8], u_rf.regs[9],
                    u_rf.regs[11], u_rf.regs[12]);
            // [PC-TRACE-4305K]: trace around the bad MOVE.L D3, $28(A3)
            // at r=4305781.  Shows what set D3=0 (vs reasonable Y in
            // good iterations).
            if (is_settled && retired >= 32'd4305740 && retired <= 32'd4305790)
                $display("[PC-TRACE-4305K] r=%d pc=%h opcode=%h D3=%h A1=%h A3=%h",
                    retired, ex_pc, ex_opcode, u_rf.regs[3], u_rf.regs[9], u_rf.regs[11]);
            // [FFC4EE-CHK]: at the indexed read `MOVE.L (d8,A0,D2.L),D3`
            // dump A0 + D2 + effective addr so we can find the BCPL
            // table whose entry is 0.  Disp = $14 = +20.
            if (is_settled && ex_pc == 32'h00ff_c4ee)
                $display("[FFC4EE-CHK] r=%d A0=%h D2=%h EA=%h",
                    retired, u_rf.regs[8], u_rf.regs[2],
                    u_rf.regs[8] + 32'h14 + u_rf.regs[2]);
            // [5E40-WR]: every write to mem[$5E40] (the linked-list
            // slot that gets nulled).  Identifies who broke the chain
            // (was $5E44 at r=2737456, is $0 at r=4305766).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_5E3E && dc_addr <= 32'h0000_5E43)
                $display("[5E40-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [CHIP-MC-CPU-WR]: capture CPU writes to chip $19A0..$19A7
            // — the corrupting write at r=11255412 that zeros MC_NEXT
            // of the free-list chunk and severs the chain.  PC + reg
            // state tells us which exec.AllocMem/FreeMem code path
            // mis-handles the split.  See project_boing_chip_freelist.md.
            // (ALLOC_TRACE moved below — was inside KICKSTART_BOOT_TRACE)

`ifdef CHIP_MC_WATCH
            // Broadened: any chip-RAM write in $1990..$19BF window.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_1990 && dc_addr <= 32'h0000_19BF)
                $display("[CHIP-MC-CPU-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h A0=%h A1=%h A2=%h A6=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata,
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[10], u_rf.regs[14]);
            // Sanity: dump EVERY write retiring in the corruption
            // window so we can see what the CPU was doing.  Use the
            // bus-side dc_we; if 0 hits, our dc_* signal interpretation
            // is wrong.
            if (dc_req_r && dc_we && dc_ack &&
                retired >= 32'd11254000 && retired <= 32'd11256000)
                $display("[CHIP-MC-NARROW] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Also instrument the BUS side — sometimes the cache
            // shadows dc_we for already-cached writes.  cur_pc_q is
            // the CPU's currently-executing PC; this fires on the
            // bus_we pulse, capturing the PC at that wall-clock cycle.
            if (retired >= 32'd11254000 && retired <= 32'd11256000)
                $display("[CHIP-MC-PCDUMP] r=%d pc=%h opcode=%h ex_kind=%d is_settled=%b dc_req_r=%b dc_we=%b dc_ack=%b dc_addr=%h",
                    retired, ex_pc, ex_opcode, ex_kind, is_settled,
                    dc_req_r, dc_we, dc_ack, dc_addr);
`endif
            // [Y-FIELD-CHK]: at every $FEA548 (MOVEA.L $56(A3), A0)
            // dump A3 + (A3+$56) so we can see whose struct field is
            // zero in the bad case.
            if (is_settled && ex_pc == 32'h00fe_a548)
                $display("[Y-FIELD-CHK] r=%d A3=%h (A3+$56=%h)",
                    retired, u_rf.regs[11], u_rf.regs[11] + 32'h56);
            // [Y-FIELD-WR]: catch every CPU write to the persistent
            // Y-coord field at $C04786 (long; covers $C04784..$C04787
            // for safety since the .L write might land on $C04784 or
            // straddle).  Identifies who zeroed it before r=4436229.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_4784 && dc_addr <= 32'h00C0_4789)
                $display("[Y-FIELD-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [STRUCT-CHK-3B4]: dump A1 (and mem[A1+$28] later) at the
            // entry of the function at $FEA3B4 that writes Y-coord.
            if (is_settled && ex_pc == 32'h00fe_a3b4)
                $display("[STRUCT-CHK-3B4] r=%d A1=%h A0=%h",
                    retired, u_rf.regs[9], u_rf.regs[8]);
            // [STRUCT-CHK-3CA]: dump A2 at the WRITE instruction so we
            // see exactly what (A2+$28) source address is.  This will
            // tell us which struct field is zero.
            if (is_settled && ex_pc == 32'h00fe_a3ca)
                $display("[STRUCT-CHK-3CA] r=%d A2=%h A3=%h",
                    retired, u_rf.regs[10], u_rf.regs[11]);
            // [C00C9C-WR]: trace every write to $C00C9C — the Y source
            // (= mem[A2+$28] when A2=$C00C74) that was zeroed before
            // the bad blit. Find who wrote 0 here.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0C9A && dc_addr <= 32'h00C0_0C9F)
                $display("[C00C9C-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [WBPC-WR-C04A8E]: any write to the long at $C04A8E (the
            // "D1 source" struct field that loaded 0 right before the bad
            // blit).  Print PC + value so we can find the bug source.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_4A8C && dc_addr <= 32'h00C0_4A91)
                $display("[WBPC-WR-C04A8E] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch CPU writes to ThisTask.tc_SigRecvd = $198A (within $1970+$1A..$198D)
            // Bit 31 (SIGF_ABORT) becoming set explains why the dispatcher
            // refuses to wake the READY task.  Catch the offending write and
            // its PC so we can find who's ORing $80000000 in.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_1988 && dc_addr <= 32'h0000_198C)
                $display("[SIGREC] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // WB1.3 boot: SSP corruption — trace any write to chip RAM
            // $1940-$1960 (the post-TOD-fix stack region where the bad
            // return-PC RTS pops $0 at $194C).  See project_wb13_cli_wait.md.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_1800 && dc_addr <= 32'h0000_1960)
                $display("[STKW] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Post-TST-fix new wall: RTS at $FE0A52 pops $0.  First fire
            // SP=$71E0, second SP=$6058 at r=4M.  Watch a wider stack
            // region $6000-$7FFF.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_6000 && dc_addr <= 32'h0000_7FFF)
                $display("[STK6058] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch writes to the Copper list buffer at chip-RAM $100C8.
            // GfxBase+$32 = $000100C8 (LOFCprList).  Workbench should
            // assemble a valid Copper list there but our dump shows
            // garbage ($FFFFFFFF entries).  If no writes fire to this
            // region, MakeView/MrgCop never runs.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_100C8 && dc_addr <= 32'h0000_103C8)
                $display("[CPRLIST-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch writes to the BOOT Copper list at chip-RAM $420..$470.
            // Whoever assembles this is the V33 Copper-list assembler;
            // the same routine should re-assemble after OpenScreen but
            // doesn't (resulting in BPLCON0=0 / no Workbench display).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_0420 && dc_addr <= 32'h0000_0470)
                $display("[BOOTCOP-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch writes to slow-RAM $C05C40..$C05E40 (Workbench
            // Screen ViewPort.DspIns Copper list).  Whoever writes
            // here IS the V33 Copper-list assembler for new ViewPorts.
            // The same routine should also write to the COMBINED View
            // list at $100C8 — find out why it doesn't.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_5C40 && dc_addr <= 32'h00C0_5E40)
                $display("[WBCOP-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // Watch writes to GfxBase+$30..$37 (slow RAM $C01E4E..$C01E55).
            // $32(GfxBase) = LOFCprList — the View Copper list pointer
            // that the VBL handler ($FC6D6C) reads to write COP1LC.
            // Find what stores $000100C8 there.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_1E4E && dc_addr <= 32'h00C0_1E60)
                $display("[GFX-VBL-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [FE0A52]: log every entry into the RTS at $FE0A52 with all
            // registers.  PC trail before this tells us who set SP=$71E0.
            if (is_settled && ex_pc == 32'h00fe_0a52)
                $display("[FE0A52] r=%d sp=%h ssp=%h sr_s=%b d0=%h d1=%h a0=%h a3=%h",
                    retired, u_rf.regs[15], usp_shadow, sr_s,
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[11]);
            // Also trace entry to $FE0A4A so we see what called the function
            // that RTS-pops $0.
            if (is_settled && ex_pc == 32'h00fe_0a4a)
                $display("[FE0A4A] r=%d sp=%h ssp=%h sr_s=%b a0=%h",
                    retired, u_rf.regs[15], usp_shadow, sr_s, u_rf.regs[8]);
            // [KBD-SP] / [KBD-CMD9]: trace keyboard.device's SP IRQ handler
            // and its cmd-9 (KBD_READMATRIX) handler.  Used to confirm whether
            // the queued IO at chipram $4468 (input.device's READMATRIX
            // request) ever gets processed when a kbd byte arrives.  See
            // project_wb13_cli_wait.md.
            if (is_settled && ex_pc == 32'h00FE_5278)
                $display("[KBD-SP] r=%d entered SP handler",
                    retired);
            if (is_settled && ex_pc == 32'h00FE_5120)
                $display("[KBD-CMD9] r=%d entered KBD_READMATRIX handler A1=%h",
                    retired, u_rf.regs[9]);
            // [WAIT-CALL]: trace every entry into Exec's Wait at $FC1F0C
            // with the wait mask (D0) and the caller's return PC (top of
            // SSP/USP).  Used to find which Wait call site has D0=$00000010
            // — the mask input.device.task is parked on.
            if (is_settled && ex_pc == 32'h00FC_1F0C)
                $display("[WAIT-CALL] r=%d mask=%h",
                    retired, u_rf.regs[0]);
            // [SIGNAL-CALL]: trace every entry into Signal at $FC1E84 with
            // target task A1 and mask D0.  Used to find who Signals
            // input.device.task with what bit.  Capture pre-fetched word
            // at SSP/USP as the caller's likely return PC.
            if (is_settled && ex_pc == 32'h00FC_1E84)
                $display("[SIGNAL-CALL] r=%d task=%h mask=%h sp=%h",
                    retired, u_rf.regs[9], u_rf.regs[0], u_rf.regs[15]);
            // [ALLOCSIG-ENTRY] / [ALLOCSIG-EXIT]: AllocSignal at $FC2000.
            // Bug investigation: FSU's "File System" task has tc_SigAlloc=$0000FFFF
            // (no bit 31), ours has $8000FFFF — our path called AllocSignal($FF)
            // and got 31, FSU's didn't.  Find the call site responsible.
            if (is_settled && ex_pc == 32'h00FC_2000)
                $display("[ALLOCSIG-ENTRY] r=%d req=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FC_2036)
                $display("[ALLOCSIG-EXIT] r=%d ret=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[15]);
            // [FREESIG] FreeSignal at $FC2038 — tracks the bit being freed.
            if (is_settled && ex_pc == 32'h00FC_2038)
                $display("[FREESIG] r=%d bit=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[15]);
            // [FS-PORT-WR]: every write to the File System task's MsgPort
            // header at slow $C00ADC..$C00AEC (mp_LN + mp_Flags + mp_SigBit
            // + mp_SigTask).  Identifies when mp_SigBit changes between
            // 31 (the AllocSignal result that sets Wait mask = $80000000)
            // and 8 (the value at idle).  PC capture nails the instruction.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0ADC && dc_addr <= 32'h00C0_0AEC)
                $display("[FS-PORT-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [BCPL-WAITPKT]: every entry to BCPL DOS handler's
            // waitpkt loop at K1.3 ROM $FDE3D0.  Captures A2 (and
            // A2+$56 = port-pointer field) so we can identify WHICH
            // port the handler reads mp_SigBit from.  Prior FS-PORT-WR
            // run shows FS task's pr_MsgPort mp_SigBit=$08, but bug
            // Wait mask=$80000000=1<<31 — so handler must be reading
            // a DIFFERENT (auxiliary) port with mp_SigBit=31.
            if (is_settled && ex_pc == 32'h00FD_E3D0)
                $display("[BCPL-WAITPKT] r=%d A2=%h",
                    retired, u_rf.regs[10]);
            // [REQ-BUILDER]: entry to K1.3 ROM $FF4CE8 — DOS error
            // requester builder.  Called when BCPL DOS wants to put
            // up an error dialog ("Volume X has a read/write error",
            // "Disk full", etc).  A1 = pointer to BPTR list of strings
            // to fill in the requester body.  Capturing A1 + ThisTask
            // tells us which error path called it.  ThisTask + $94 =
            // pr_Result2 (the DOS error code).
            if (is_settled && ex_pc == 32'h00FF_4CE8)
                $display("[REQ-BUILDER] r=%d A1=%h A6=%h sp=%h",
                    retired, u_rf.regs[9], u_rf.regs[14], u_rf.regs[15]);
            // [AUTO-REQ]: just before the actual AutoRequest call at
            // $FF4DB4.  By here the requester struct is fully built;
            // A0 = body IText (already populated), A1=pos/A2=neg.
            if (is_settled && ex_pc == 32'h00FF_4DB4)
                $display("[AUTO-REQ] r=%d body=%h pos=%h neg=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[10], u_rf.regs[15]);
            // [DLG-BUILDER]: trace multiple candidate entry points to
            // the K1.3 DOS error-dialog routine that ends at AutoRequest
            // ($FF4DB4).  We probe $FF4CE8 (full prelude) and several
            // mid-routine PCs to identify which entry path the dialog
            // takes.  Captures A1/A3/A4/A6/SP/D-args.
            if (is_settled &&
                (ex_pc == 32'h00FF_4CE8 ||  // full prelude entry
                 ex_pc == 32'h00FF_4D08 ||  // just before AllocMem JSR
                 ex_pc == 32'h00FF_4D1C ||  // just before FindTask JSR
                 ex_pc == 32'h00FF_4D28 ||  // just before OpenLibrary JSR
                 ex_pc == 32'h00FF_4D32 ||  // post-OpenLibrary entry
                 ex_pc == 32'h00FF_4D40 ||  // BCPL-string-iter entry
                 ex_pc == 32'h00FF_4DAA))   // pre-AutoRequest setup
                $display("[DLG-PC] r=%d pc=%h A1=%h A3=%h A4=%h A6=%h sp=%h d0=%h d1=%h",
                    retired, ex_pc,
                    u_rf.regs[9], u_rf.regs[11], u_rf.regs[12], u_rf.regs[14],
                    u_rf.regs[15],
                    u_rf.regs[0], u_rf.regs[1]);
            // [DLG-CALLER]: catch BSR or JSR landing at any of these
            // by recording every PC change in a narrow window before
            // the dialog fires.  Track the LAST CHANGE INTO $FF4Dxx
            // range to find the caller PC.
            if (is_settled &&
                ex_pc >= 32'h00FF_4D20 && ex_pc <= 32'h00FF_4D32 &&
                retired >= 4444460 && retired <= 4444475)
                $display("[DLG-TRACE] r=%d pc=%h",
                    retired, ex_pc);
            // [BPTR-STRUCT-WR]: catch CPU writes to the 3-BPTR struct at
            // slow RAM $C00F2C..$C00F37 (the dialog template that becomes
            // input to $FF4D08).  The CPU instruction that writes these
            // BPTRs is the BCPL DOS function that built the error msg.
            // Log ex_pc + the address + write data so we can find the
            // BCPL "ErrorReport"-style routine.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0F2C && dc_addr <= 32'h00C0_0F37)
                $display("[BPTR-STRUCT-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata[31:0]);
            // [SRC-TPL-WR]: catch writes to the source template at
            // $C00EE8..$C00EF3.  Those BPTRs are copied to the dialog
            // struct by $FF532E/$FF5334/$FF533A.  The writer is the
            // BCPL DOS routine that picked the read/write error msg.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_0EE8 && dc_addr <= 32'h00C0_0EF3)
                $display("[SRC-TPL-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata[31:0]);
            // [FF4506-ENTRY]: BCPL handler at $FF4506.  At r=4307668 it
            // received D3=0 and wrote 0 to $C00C9C, triggering the
            // downstream chain to the dialog.  Probe captures all args
            // (D1..D4, A1) plus A6 (BCPL next-instr ptr, = caller).
            if (is_settled && ex_pc == 32'h00FF_4506)
                $display("[FF4506-ENTRY] r=%d D1=%h D2=%h D3=%h D4=%h A1=%h A6=%h",
                    retired, u_rf.regs[1], u_rf.regs[2], u_rf.regs[3],
                    u_rf.regs[4], u_rf.regs[9], u_rf.regs[14]);
            // [Y-ZERO-WR]: focused on CPU writes that ZERO the Y-coord
            // field at $C04786.  Captures EX_PC + EX_KIND so we can
            // identify the specific instruction.  The existing
            // Y-FIELD-WR probe is wider but might miss byte/word
            // sized variants.  Trigger on ANY write with wdata=0 that
            // touches $C04786..$C04789.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h00C0_4784 && dc_addr <= 32'h00C0_4789 &&
                dc_wdata[31:0] == 32'h0000_0000)
                $display("[Y-ZERO-WR] r=%d pc=%h kind=%d addr=%h be=%b",
                    retired, ex_pc, ex_kind, dc_addr, dc_be);
            // [Y-FIELD-RD]: capture all reads from $C04786 (the Y-coord
            // field).  If $C04786 holds $1958 but A0 ends up 0 at
            // $FEA932 entry, that's a MOVEA.L bug.  If $C04786 reads
            // back 0, something cleared it after the last Y-FIELD-WR.
            if (dc_req_r && !dc_we && dc_ack &&
                dc_addr >= 32'h00C0_4784 && dc_addr <= 32'h00C0_4789)
                $display("[Y-FIELD-RD] r=%d pc=%h kind=%d addr=%h rdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_rdata);
            // [FEA920-ENTRY]: capture A2 + SP at K1.3 ROM $FEA920
            // (a function that's part of the trackdisk MFM-decode path).
            // BAD-BLTDPT event at $FEA992 was traced back to A2=0
            // entering this routine.  Find which caller passes A2=0.
            if (is_settled && ex_pc == 32'h00FE_A920)
                $display("[FEA920-ENTRY] r=%d A2=%h A1=%h D0=%h sp=%h",
                    retired, u_rf.regs[10], u_rf.regs[9], u_rf.regs[0], u_rf.regs[15]);
            // [FEA926-PRECALL]: capture A0 right before BSR $FEA932
            // where A0 is the value that ends up at $C04A8E.
            if (is_settled && ex_pc == 32'h00FE_A926)
                $display("[FEA926-PRECALL] r=%d A0=%h A2=%h",
                    retired, u_rf.regs[8], u_rf.regs[10]);
            // [ALLOCMEM-ENTRY] / [ALLOCMEM-EXIT]: trace every call.
            // Exec.library AllocMem on K1.3 patches itself into slow RAM
            // at $C05088 after init (the JT at $C001B0 = LVO -$C6 jumps
            // there).  D0 = size requested, D1 = MEMF_* flags.  Exit at
            // RTS with D0 = returned address.  Cosim found a $4E88 byte
            // shift between our slow-RAM ptrs and FSU's at r=2881025;
            // this probe lets us identify which AllocMem call returns
            // a different address than FSU's.
            if (is_settled && ex_pc == 32'h00C0_5088)
                $display("[ALLOCMEM-ENTRY] r=%d size=%h flags=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[1], u_rf.regs[15]);
            // Slow-RAM AllocMem returns through a chain; can't easily
            // capture exit PC.  Instead trace ALL slow-RAM JMPs $C0xxxx
            // via PC tracking.  Skip for now — rely on entry probe.

            // [ERR-DIALOG]: entry to BCPL DOS error-display wrapper
            // at $FDFE32.  This wrapper saves A0..A3/D0..D3 then JSRs
            // $FDE368 (the actual display routine which eventually
            // calls AutoRequest).  Capturing all 8 caller-passed regs
            // tells us EXACTLY what the BCPL DOS handler is asking
            // the user about (volume name BPTR, error code, etc.).
            // SP at entry = caller's return-PC location, so *(SP)
            // identifies the BCPL DOS callsite.
            if (is_settled && ex_pc == 32'h00FD_FE32)
                $display("[ERR-DIALOG] r=%d A0=%h A1=%h A2=%h A3=%h D0=%h D1=%h D2=%h D3=%h sp=%h",
                    retired,
                    u_rf.regs[8],  u_rf.regs[9],  u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[0],  u_rf.regs[1],  u_rf.regs[2],  u_rf.regs[3],
                    u_rf.regs[15]);
            // [TD-BEGIO]: trace every entry into trackdisk.device.BeginIO
            // at $FE9C3E with the IORequest pointer (A1).  Used to find
            // out whether the chip-RAM FileHandler task is actually
            // submitting fresh TD_READ requests or just pinging
            // trackdisk with no new work.  After cd83a77, post-strap
            // WB1.3 boot enters a trackdisk<->FS signal loop that does
            // NOT produce new DSKLEN writes -- this trace tells us why.
            if (is_settled && ex_pc == 32'h00FE_9C3E)
                $display("[TD-BEGIO] r=%d A1=%h sp=%h",
                    retired, u_rf.regs[9], u_rf.regs[15]);
            // [TD-READ-ENTRY]: trackdisk.device CMD_READ/WRITE handler at
            // $FEA3B4.  A1=IORequest, A3 (loaded from A1's io_Unit) will
            // be set up here.  Log A1 + the io_Offset/io_Length values to
            // see what FS is asking for at this exact entry.
            if (is_settled && ex_pc == 32'h00FE_A3B4)
                $display("[TD-READ-ENTRY] r=%d A1=%h",
                    retired, u_rf.regs[9]);
            // [TD-OFFSET]: helper $FE9E02 is called from $FEA3D4 with
            // D0 = io_Offset (the block address FS is reading).  Logs
            // D0 to reveal whether FS reads different blocks (making
            // progress through the FS walk) or hammers the same block
            // (stuck verifying root over and over).
            if (is_settled && ex_pc == 32'h00FE_9E02)
                $display("[TD-OFFSET] r=%d D0=%h", retired, u_rf.regs[0]);
            // [TD-READ-OFFSET-BAD]: BMI target after offset-to-track conv
            // returns negative.  Means io_Offset (or end-offset) is off
            // the disk.
            if (is_settled && ex_pc == 32'h00FE_A5AA)
                $display("[TD-READ-OFFSET-BAD] r=%d A1=%h",
                    retired, u_rf.regs[9]);
            // [TD-READ-EXIT]: common error-exit at $FEA5A0 where io_Error
            // has been set to some specific TDERR_ value.
            if (is_settled && ex_pc == 32'h00FE_A5A0)
                $display("[TD-READ-EXIT] r=%d A1=%h",
                    retired, u_rf.regs[9]);
            // [TD-DISKCHANGED]: at $FEA416 trackdisk sets io_Error = $1D
            // (TDERR_DiskChanged) and falls through to $FEA5A0.  If this
            // fires, the BTST #1, $40(A3) at $FEA40E saw a non-zero bit.
            if (is_settled && ex_pc == 32'h00FE_A416)
                $display("[TD-DISKCHANGED] r=%d", retired);
            // [TD-D0-ERROR]: at $FEA44C trackdisk does MOVE.B D0, $1F(A1)
            // — writes D0 (low byte) into io_Error.  Logging D0 reveals
            // exactly which TDERR_* code FS is being told the read failed
            // with.  D0 = 21 NoSecHdr, 22 BadSecPreamble, 23 BadSecID,
            //       24 BadHdrSum, 25 BadSecSum, 26 TooFewSecs ...
            if (is_settled && ex_pc == 32'h00FE_A44C)
                $display("[TD-D0-ERROR] r=%d D0=%h", retired, u_rf.regs[0]);
            // [TD-D0-ERROR2]: at $FEA47C trackdisk does the SAME io_Error
            // store (third error-setting path).  D0 here came from BSR
            // $FEA61E + a comparison check on the unit's media buffer.
            if (is_settled && ex_pc == 32'h00FE_A47C)
                $display("[TD-D0-ERROR2] r=%d D0=%h", retired, u_rf.regs[0]);
            // [TD-GETMSG-RET]: trackdisk's main-loop body at $FEA3AC TSTs
            // D0 (the message returned by GetMsg).  If D0=0, the wake was
            // spurious (no actual packet on the port) and the loop spins
            // back to Wait.  Logging this distinguishes "stuck spinning on
            // empty port" from "processing a packet but failing validation
            // and retrying".
            if (is_settled && ex_pc == 32'h00FE_A3AC)
                $display("[TD-GETMSG-RET] r=%d D0=%h", retired, u_rf.regs[0]);
            // [TD-MAINLOOP]: the outer trackdisk-task entry that calls the
            // Wait+GetMsg sub-loop at $FEA38A and then dispatches.  Fires
            // once per command processed.
            if (is_settled && ex_pc == 32'h00FE_A316)
                $display("[TD-MAINLOOP] r=%d A3=%h", retired, u_rf.regs[11]);
            // [TD-READ-EPILOGUE]: the MOVEM.L (SP)+ at $FEA5B6 — actual
            // last instruction of the read handler before RTS.  Catches
            // ANY exit path (including ones that bypass $FEA5A0).
            if (is_settled && ex_pc == 32'h00FE_A5B6)
                $display("[TD-READ-EPILOGUE] r=%d", retired);
            // [TD-PCWATCH]: bisect milestones inside the read handler.
            // If the read enters at $FEA3B4 but never reaches $FEA5A0
            // (the success epilogue), one of these PCs tells us how far.
            if (is_settled && ex_pc == 32'h00FE_A3F8)
                $display("[TD-A3F8] r=%d", retired);   // entry to bit-2 BTST
            if (is_settled && ex_pc == 32'h00FE_A40E)
                $display("[TD-A40E] r=%d", retired);   // entry to bit-1 BTST
            if (is_settled && ex_pc == 32'h00FE_A414)
                $display("[TD-A414] r=%d", retired);   // entry to BEQ
            if (is_settled && ex_pc == 32'h00FE_A420)
                $display("[TD-A420] r=%d D0=%h", retired, u_rf.regs[0]);   // past disk-changed
            if (is_settled && ex_pc == 32'h00FE_A42E)
                $display("[TD-A42E] r=%d", retired);   // after $FEA5BC ret
            if (is_settled && ex_pc == 32'h00FE_A454)
                $display("[TD-A454] r=%d", retired);   // alternate good
            if (is_settled && ex_pc == 32'h00FE_A464)
                $display("[TD-A464] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A484)
                $display("[TD-A484] r=%d", retired);   // write/continue
            if (is_settled && ex_pc == 32'h00FE_A5BC)
                $display("[TD-CALL-A5BC] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A61E)
                $display("[TD-CALL-A61E] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A5D2)
                $display("[TD-CALL-A5D2] r=%d", retired);
            if (is_settled && ex_pc == 32'h00FE_A5D8)
                $display("[TD-CALL-A5D8] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A510)
                $display("[TD-A510] r=%d", retired);   // BNE.W target from $FEA494
            // Inside-$FEA61E milestones — bisect the cache-miss disk-read path:
            if (is_settled && ex_pc == 32'h00FE_A0E2)
                $display("[TD-CALL-A0E2] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A05A)
                $display("[TD-CALL-A05A] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A1A4)
                $display("[TD-CALL-A1A4] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_AC62)
                $display("[TD-CALL-AC62] r=%d", retired);
            // Read 15 path past $FEA61E body — milestones inside.
            if (is_settled && ex_pc == 32'h00FE_A636)
                $display("[TD-A636] r=%d D0=%h", retired, u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FE_A64E)
                $display("[TD-A64E] r=%d", retired);
            if (is_settled && ex_pc == 32'h00FE_A678)
                $display("[TD-A678] r=%d", retired);
            // Track Wait() calls with the calling SP so we can pinpoint
            // whether it's trackdisk task or FS task that's parked.
            if (is_settled && ex_pc == 32'h00FC_1F0C)
                $display("[WAIT-CALL-SP] r=%d mask=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[15]);
            // [PUTMSG]: trace every entry into PutMsg (LVO -$16E body
            // at $FC1B70).  A0 = port, A1 = message.  Confirms whether
            // the FileHandler-task signal storm includes real packet
            // submissions or just spurious Signal calls.
            if (is_settled && ex_pc == 32'h00FC_1B70)
                $display("[PUTMSG] r=%d port=%h msg=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[15]);
            // [REPLYMSG]: LVO -$174, body at $FC1BEA.  A1 = message.
            if (is_settled && ex_pc == 32'h00FC_1BEA)
                $display("[REPLYMSG] r=%d msg=%h sp=%h",
                    retired, u_rf.regs[9], u_rf.regs[15]);
            // [DOS-ADDTAIL]: dos.library bypasses exec PutMsg.  It enqueues
            // packets via two BCPL wrappers that call AddTail directly:
            //   $FF364C : 2-arg wrapper, A0=list, A1=msg already loaded
            //   $FF3920 : 4-arg wrapper, A1=msg, A0 from caller, D0/D1=args
            // Tracing these reveals dos's own packet flow that PUTMSG misses.
            // The Initial-CLI wait diagnosis depends on knowing whether dos
            // ever enqueues to port $A264 (Initial CLI's pr_MsgPort).
            if (is_settled && ex_pc == 32'h00FF_364C)
                $display("[DOS-ADDTAIL1] r=%d A0=%h A1=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FF_3920)
                $display("[DOS-ADDTAIL2] r=%d A0=%h A1=%h D0=%h D1=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[9],
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[15]);
            // [DOS-INIT-PC]: dos.library rt_Init entry + key milestones.
            // Tracking whether dos init reaches each phase, and what its
            // GetMsg call at $FF400A receives — if any.  The GetMsg is
            // the suspected gate where dos awaits the bootblock setup
            // packet that would tell Initial CLI to run startup-sequence.
            if (is_settled && ex_pc == 32'h00FF_3E94)
                $display("[DOS-RT-INIT] r=%d sp=%h", retired, u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FF_3FEC)
                $display("[DOS-CREATEPROC] r=%d D7=%h sp=%h",
                    retired, u_rf.regs[7], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FF_400A)
                $display("[DOS-GETMSG] r=%d A0=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FF_400E)
                $display("[DOS-GETMSG-RET] r=%d D0=%h sp=%h",
                    retired, u_rf.regs[0], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FF_4014)
                $display("[DOS-ALERT] r=%d D7=%h sp=%h",
                    retired, u_rf.regs[7], u_rf.regs[15]);
            // [OBTSEM]: ObtainSemaphore-like primitive at $FC2DF0 — increments
            // a counter at $2C(A0), AddTails ThisTask onto a wait queue at
            // $10(A0), then Wait($10).  Input.device is parked in this Wait
            // post-r=4M.  Capturing A0 reveals which semaphore is held by
            // whom and blocking the chain.
            if (is_settled && ex_pc == 32'h00FC_2DF0)
                $display("[OBTSEM] r=%d A0=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[15]);
            // [RELSEM]: matching Release-side primitive at $FC2E40 —
            // decrements counters, signals next waiter.  Counts of this
            // tell us if anyone EVER releases the semaphore input.device
            // is waiting on.
            if (is_settled && ex_pc == 32'h00FC_2E40)
                $display("[RELSEM] r=%d A0=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[15]);
            // [SEM-WAIT]: trace specifically the Wait($10) call at
            // $FC2E2A inside ObtainSemaphore (the bit-4 blocking point).
            if (is_settled && ex_pc == 32'h00FC_2E2A)
                $display("[SEM-WAIT] r=%d A0=%h sp=%h",
                    retired, u_rf.regs[8], u_rf.regs[15]);
            // [CACA-WR]: trace memory writes carrying $CACA in the high half
            // of wdata — catches the K1.3 table-copy routine at $FC6BE0 that
            // copies $CA-bytes into chip-RAM (intentionally; the table at
            // $FC6BB2 has $CA at offsets +$0B and +$0F).  These writes
            // deposit $CACACACA into chip-RAM struct fields that are then
            // misread by intuition.library at $FD550E and cause the
            // ObtainSemaphore deadlock.  Narrow filter so the trace is
            // signal not noise.
            if (dc_req_r && dc_we && dc_ack &&
                dc_wdata[31:16] == 16'hCACA)
                $display("[CACA-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [INTU-COPY]: capture A3 (and A2) at the intuition.library
            // copy site at $FD550E that propagates $CACACACA from A3+$7C
            // into A2+$10.  A3 reveals what struct holds the uninitialized
            // field; the upstream emulator-bug is in whatever should
            // initialize A3+$7C before this point.
            if (is_settled && ex_pc == 32'h00FD_550E)
                $display("[INTU-COPY] r=%d A2=%h A3=%h src=%h dst=%h sp=%h",
                    retired, u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[11] + 32'h7C,
                    u_rf.regs[10] + 32'h10, u_rf.regs[15]);
            // [INTU-COPY-EARLIER]: also fire on entry to the surrounding
            // function so we see A3 at the function-entry point, before
            // any local munging that the disassembler chokes on.
            // $FD5480 is reached via fall-through from $FD547E in the
            // function starting at $FD5416 (the intuition cleanup that
            // tests bit-12 of D3 to decide minterms-copy vs JSR
            // $FD4A6A pointer-resolve).  We log D3 to confirm bit-12
            // state at the decision point.
            if (is_settled && ex_pc == 32'h00FD_5480)
                $display("[INTU-FN-ENTRY] r=%d A2=%h A3=%h D3=%h sp=%h",
                    retired, u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[3], u_rf.regs[15]);
            // [INTU-D1] log A1 at $FD5446 so we can see the struct
            // pointer whose +$10 field selects the path.
            if (is_settled && ex_pc == 32'h00FD_5446)
                $display("[INTU-D1] r=%d A1=%h D3=%h sp=%h",
                    retired, u_rf.regs[9], u_rf.regs[3], u_rf.regs[15]);
            // [INTU-STRUCT-WR]: catch ALL writes (any size) to the struct
            // at $BE80..$BF20 (the intuition object A3 was pointing at).
            // Especially interested in the field at offset +$7C (= $BEFC)
            // which holds $CACACACA at boot wall.  Logs PC + addr + wdata.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_BE80 && dc_addr <= 32'h0000_BF20)
                $display("[INTU-STRUCT-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [A5-CHANGE]: track every change to A5 (= u_rf.regs[13]) during
            // the deadlock window r=4057000..4060000.  CON's BCPL code in
            // this window loads A5 with $CACACACA somewhere, then computes
            // A5+$48 → corrupt semaphore.  Pin down the exact instruction.
            if (is_settled && retired >= 32'd4057000 && retired <= 32'd4060000
                && u_rf.regs[13] != a5_last_r)
                $display("[A5-CHANGE] r=%d pc=%h new_a5=%h old_a5=%h",
                    retired, ex_pc, u_rf.regs[13], a5_last_r);
            // [A5-BAD]: fires the first time A5 becomes $082800FC.  Drop
            // is_settled so we catch the change even when written by an
            // exception save/restore that doesn't go through a normal
            // instruction-settle.
            if (u_rf.regs[13] == 32'h0828_00FC
                && a5_last_r != 32'h0828_00FC)
                $display("[A5-BAD] r=%d pc=%h new_a5=%h prev_a5=%h op=%h ra=%h rb=%h",
                    retired, ex_pc, u_rf.regs[13], a5_last_r,
                    ex_opcode, ex_ra, ex_rb);
            // [A5-WIN] log every A5 change in the 100K-retired window
            // before the BAD-PC at r=4203012.  We need a wider view to
            // see if A5 came from a long-load, a MOVEM, or an exception
            // frame restore (RTE).
            if (retired >= 32'd4100000 && retired <= 32'd4203100
                && u_rf.regs[13] != a5_last_r)
                $display("[A5-WIN] r=%d pc=%h new_a5=%h old_a5=%h op=%h",
                    retired, ex_pc, u_rf.regs[13], a5_last_r, ex_opcode);
            // [ROMWACK]: fires when K1.3 enters the ROMWack serial-debug
            // monitor command loop at \$FC2440.  Logs all D/A registers
            // and SP so we can see what triggered it (Alert code in
            // some register, return PC on stack, etc.).
            if (is_settled && ex_pc == 32'h00FC_2440)
                $display("[ROMWACK] r=%d sp=%h d0=%h d1=%h d2=%h a0=%h a1=%h a4=%h a5=%h a6=%h",
                    retired, u_rf.regs[15],
                    u_rf.regs[0], u_rf.regs[1], u_rf.regs[2],
                    u_rf.regs[8], u_rf.regs[9],
                    u_rf.regs[12], u_rf.regs[13], u_rf.regs[14]);
            // [GURU-PC10E4]: the WB1.3 Guru saved-PC is $000010E4 (= File
            // System MsgPort area, not code).  Catch any redirect to that
            // address (which BAD-PC misses since it's in chip-RAM range).
            // Logs the FROM PC + the instruction op so we can identify
            // the bad RTS/JSR/JMP.
            if (redirect_valid && redirect_pc == 32'h0000_10E4 &&
                redirect_pc != ex_pc)
                $display("[GURU-PC10E4] from=%h to=%h retired=%d kind=%d sp=%h op=%h ra=%h rb=%h",
                    ex_pc, redirect_pc, retired, ex_kind, rf_rc_data,
                    ex_opcode, ex_ra, ex_rb);
            // Also: trace any CHIP-RAM redirect below the standard 64KB
            // code region ($0..$FFFF) -- those are likely jumps into
            // vector tables, struct fields, MsgPorts, etc.  Excludes
            // legitimate exception vectors (= jumping to vec-handler
            // address in ROM-range, not low chip RAM).
            if (redirect_valid && redirect_pc < 32'h0000_8000 &&
                redirect_pc >= 32'h0000_0400 &&
                redirect_pc != ex_pc)
                $display("[CHIPRAM-LOW-PC] from=%h to=%h retired=%d kind=%d sp=%h op=%h",
                    ex_pc, redirect_pc, retired, ex_kind, rf_rc_data,
                    ex_opcode);
            // [5D80-WR] watch CPU writes to the MOVEM-source area
            // $5D80..$5E00 in the 100K-retired window before the
            // BAD-PC at r=4203012.  The MOVEM.L d16(PC) at $5D82
            // reads $082800FC from this area into A5; we want to find
            // who wrote that long there.  If no CPU writes show up,
            // the data came in via disk DMA (off-CPU-bus) and the
            // issue is upstream in MFM decode / disk pointer setup.
            if (dc_req_r && dc_we && dc_ack
                && dc_addr >= 32'h0000_5D00 && dc_addr <= 32'h0000_5E00)
                $display("[5D80-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [SIGWAIT-WR]: any write to input.device.task ($3342) +
            // $16..$19 (tc_SigWait).  Helps find who's writing $10.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_3358 && dc_addr <= 32'h0000_335B)
                $display("[SIGWAIT-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [WB-FLAGS-WR]: any write to the struct at $C128 (the one
            // whose +$10 word selects intuition's cleanup path).  Field
            // at +$10 lives at $C138..$C13B; we widen the watch slightly
            // to catch the Flags long-write at $C136 (since +$0E is a UL
            // Flags field if this is a Window struct).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_C128 && dc_addr <= 32'h0000_C140)
                $display("[WB-FLAGS-WR] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [INTU-MKGADGET]: log intuition_base (= A6) at the entry
            // to the gadget-template factory $FDE8E0.  Also log A0 at
            // $FDE91A right before the 11-long template copy, so we
            // know the template's source address — that's where the
            // GadgetType field originates.
            if (is_settled && ex_pc == 32'h00FD_E8E0)
                $display("[INTU-MKGADGET] r=%d A6_base=%h sp=%h",
                    retired, u_rf.regs[14], u_rf.regs[15]);
            if (is_settled && ex_pc == 32'h00FD_E91A)
                $display("[INTU-MKGADGET-TPL] r=%d A0_tpl=%h A2_dst=%h D3=%h D4=%h",
                    retired, u_rf.regs[8], u_rf.regs[10],
                    u_rf.regs[3], u_rf.regs[4]);
            // [INTU-TPL-COPY] inside helper $FD643A at the MOVE.L (A1)+,(A0)+
            // copy loop ($FD64AC).  Logs A1 (source) and A0 (dest) so we
            // can verify A1 points to ROM template data ($FD6062 +
            // computed offset).  If A1 points wrong → MULU bug.  If A1
            // is right but chipram dest has zeros → MOVE.L-postinc bug.
            if (is_settled && ex_pc == 32'h00FD_64AC)
                $display("[INTU-TPL-COPY] r=%d A0=%h A1=%h D0=%h",
                    retired, u_rf.regs[8], u_rf.regs[9], u_rf.regs[0]);
            // [VEC-WR] any CPU write to chip-RAM vector \$10-\$13 (the
            // ILLEGAL trap address).  Post-ADDA.W fix, WB1.3 boot hits
            // an ILLEGAL-loop at r=4.2M because vector \$10 ends up
            // pointing at \$FC00B6 (exec's Resident matchword \$4AFC,
            // which is itself an ILLEGAL opcode).  We need to find what
            // wrote that bad value.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_0008 && dc_addr <= 32'h0000_002F)
                $display("[VEC-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // Also trace writes to chip RAM $0..$1F so we can see what
            // set $14 to $F1DD that traps in the LINEF storm.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr <= 32'h0000_0017 && dc_addr >= 32'h0000_0000)
                $display("[CHIPLOW-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // Trace writes to the full vector table area $0..$FF and
            // catch any CPU write that NULLs out a vector — specifically
            // vec=27 ($6C) and vec=11 ($2C), both of which read NULL when
            // the watchdog forces an IRQ at r=4436670.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_0060 && dc_addr <= 32'h0000_006F)
                $display("[VEC27-WR] r=%d pc=%h addr=%h be=%b wdata=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata);
            // [C2CC-WR] any CPU write to supervisor-stack slot \$C2CC..\$C2CF.
            // This is the slot the bad RTS at \$FCF104 pops a NULL from
            // (VEC-EXEC kind=2 sp=\$C2CC to=\$0 at r=4.2M, vec=4 ILLEGAL).
            // We want to find the JSR/BSR that pushed onto this slot --
            // if no writes fire, the slot was NEVER written (= caller
            // pushed garbage from uninit SP).  If writes fire with PC=0,
            // the caller computed/pushed NULL.  Otherwise, something
            // corrupted it later.
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_C2CC && dc_addr <= 32'h0000_C2CF)
                $display("[C2CC-WR] r=%d pc=%h addr=%h be=%b wdata=%h sp=%h",
                    retired, ex_pc, dc_addr, dc_be, dc_wdata, u_rf.regs[15]);
            // [FCF104-PEEK] log register state at the bad RTS *before* it
            // fires.  Confirms what mem[SP] reads.  Note: u_rf.regs[15]
            // is the live A7 register file value at this cycle.
            if (is_settled && ex_pc == 32'h00FC_F104)
                $display("[FCF104-PEEK] r=%d sp=%h d0=%h d1=%h a0=%h a1=%h a2=%h a6=%h",
                    retired, u_rf.regs[15],
                    u_rf.regs[0], u_rf.regs[1],
                    u_rf.regs[8], u_rf.regs[9], u_rf.regs[10],
                    u_rf.regs[14]);
            // Bisect the A1=$00006062-instead-of-$00FD6062 bug.
            // $FD644A loads A3 = #$00FD6062 via MOVEA.L #imm.L.
            // $FD649A copies A3 to A0 via MOVEA.L A3,A0.
            // $FD64A4 copies A0 to A1 via MOVEA.L A0,A1.
            // $FD64A6 adds D0.W to A1 via ADDA.W.
            // Log A-reg values at each of these PCs.
            if (is_settled && ex_pc == 32'h00FD_644A)
                $display("[BISECT-A3-LOAD] r=%d A3_before=%h", retired, u_rf.regs[11]);
            if (is_settled && ex_pc == 32'h00FD_6450)
                $display("[BISECT-A3-AFTER] r=%d A3_after=%h", retired, u_rf.regs[11]);
            if (is_settled && ex_pc == 32'h00FD_649A)
                $display("[BISECT-A3-USE] r=%d A3=%h A0_before=%h", retired, u_rf.regs[11], u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00FD_64A4)
                $display("[BISECT-A0] r=%d A0=%h A1_before=%h", retired, u_rf.regs[8], u_rf.regs[9]);
            if (is_settled && ex_pc == 32'h00FD_64A6)
                $display("[BISECT-A1] r=%d A1=%h D0=%h", retired, u_rf.regs[9], u_rf.regs[0]);
            if (is_settled && ex_pc == 32'h00FD_64A8)
                $display("[BISECT-A1-FINAL] r=%d A1=%h", retired, u_rf.regs[9]);
            // [STRAP-BOOT]: strap reached its bootblock-validate path.
            // At $FE85A4-AA strap checks (A4) == 'DOS\0' (the long-word
            // expected at $FE841C in ROM).  At $FE85C6 strap calls the
            // bootblock entry via JSR $0C(A4) — the moment WB1.3's boot
            // code starts running in chip-RAM.
            if (is_settled && ex_pc == 32'h00FE_85A4)
                $display("[STRAP-BOOT] r=%d sec0 magic check ENTERED A4=%h",
                    retired, u_rf.regs[12]);
            if (is_settled && ex_pc == 32'h00FE_85AC)
                $display("[STRAP-BOOT] r=%d sec0 magic OK (DOS), checking sum",
                    retired);
            if (is_settled && ex_pc == 32'h00FE_85C2)
                $display("[STRAP-BOOT] r=%d checksum OK, about to JSR bootblock entry",
                    retired);
            if (is_settled && ex_pc == 32'h00FE_85C6)
                $display("[STRAP-BOOT] r=%d JSR $0C(A4)=%h -- BOOTBLOCK ENTRY",
                    retired, u_rf.regs[12] + 32'h0C);
            if (is_settled && ex_pc == 32'h00FE_8600)
                $display("[STRAP-BOOT] r=%d bootblock validation FAILED, retry counter A2=%h",
                    retired, u_rf.regs[10]);
            if (is_settled && ex_pc == 32'h00FE_85CA)
                $display("[STRAP-BOOT] r=%d bootblock returned, D0=%h A0=%h",
                    retired, u_rf.regs[0], u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00FE_85F0)
                $display("[STRAP-BOOT] r=%d bootblock SUCCESS path entered, A0=%h",
                    retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00FE_8C9C)
                $display("[STRAP-BOOT] r=%d entered FE8C9C (boot-after-success) A0=%h",
                    retired, u_rf.regs[8]);
            if (is_settled && ex_pc == 32'h00FE_85F8)
                $display("[STRAP-BOOT] r=%d returned from FE8C9C",
                    retired);
            if (is_settled && ex_pc == 32'h00FE_86B4)
                $display("[STRAP-BOOT] r=%d strap reaching final/cleanup A3=%h",
                    retired, u_rf.regs[11]);
            // Trace bootblock entry execution at chipram $55E4
            if (is_settled && ex_pc == 32'h0000_55E4)
                $display("[STRAP-BOOT] r=%d ENTERED bootblock entry at $55E4",
                    retired);
            if (is_settled && ex_pc >= 32'h0000_5400 && ex_pc <= 32'h0000_5600 && (retired[5:0] == 6'd0))
                $display("[BB-PC] r=%d pc=%h",
                    retired, ex_pc);
            // [DOS-INIT]: trace dos.library rt_Init entry at $FF3E94 (where
            // strap JMPs after success).  Sample PC every 256 retired through
            // dos.library's init range so we can see how far it gets.
            if (is_settled && ex_pc == 32'h00FF_3E94)
                $display("[DOS-INIT] r=%d ENTERED dos.library rt_Init", retired);
            if (is_settled && ex_pc >= 32'h00FF_3000 && ex_pc <= 32'h00FF_FFFF && (retired[7:0] == 8'd0))
                $display("[DOS-PC] r=%d pc=%h",
                    retired, ex_pc);
            // Stack-balance probe at timer.device.Init's call to AddDevice.
            // Log SP at:
            //   $FE8F80 — just before JSR $FE50(A6) to AddDevice
            //   $FE8F88 — just after  JSR return (= entry of exit MOVEM)
            //   $FC0690 — AddDevice entry
            //   $FC069C — AddDevice exit RTS
            //   $FC0694 — just before BSR $FC16BE
            //   $FC0698 — after BSR returns (= before JSR SumLibrary)
            if (is_settled && (ex_pc == 32'h00FE_8F80 ||
                               ex_pc == 32'h00FE_8F88 ||
                               ex_pc == 32'h00FC_0690 ||
                               ex_pc == 32'h00FC_069C ||
                               ex_pc == 32'h00FC_0694 ||
                               ex_pc == 32'h00FC_0698))
                $display("[STK-PC] r=%d pc=%h SP=%h",
                    retired, ex_pc, u_rf.regs[15]);
            // Trace every Signal() entry to find who passes $80000000 as the
            // signal mask.  Signal is at $FC1E84.  Log A1 (task ptr) and D0 (mask).
            if (is_settled && ex_pc == 32'h00fc_1e84)
                $display("[SIG] r=%d Signal ENTER A1=%h D0=%h SP=%h",
                    retired, u_rf.regs[9], u_rf.regs[0], u_rf.regs[15]);
            // Trace every PC in the window leading up to the Signal($1970, $80000000)
            // call at r~1296136 so we can see who's calling.
            if (is_settled && retired >= 32'd1296100 && retired <= 32'd1296140)
                $display("[PRESIG] r=%d pc=%h opcode=%h D0=%h A1=%h",
                    retired, ex_pc, ex_opcode, u_rf.regs[0], u_rf.regs[9]);
            // Watch writes to byte $1909 = MP_SIGBIT of a critical MsgPort whose
            // value is read at $FC1BCA before Signal is called.  If MP_SIGBIT is
            // 31 ($1F), Signal mask becomes $80000000 (SIGF_DOS reserved).
            if (dc_req_r && dc_we && dc_ack &&
                dc_addr >= 32'h0000_1908 && dc_addr <= 32'h0000_190B)
                $display("[SIGBIT] r=%d pc=%h kind=%d addr=%h be=%b wdata=%h",
                    retired, ex_pc, ex_kind, dc_addr, dc_be, dc_wdata);
            // [RESINIT] log when execution enters each known K1.3 Resident
            // init function.  For libraries/devices, rt_Init points to an
            // auto-init *table*; the actual init code is at table[12].
            // For NT_TASK residents (workbench.task), rt_Init IS the task
            // entry point.  Addresses extracted from the K1.3 ROM via
            // tools/find_residents (Python).
            if (is_settled && ex_kind != K_STOP) begin
                case (ex_pc)
                    // -- exec & per-resident init entry points --
                    32'h00fc00d2: $display("[RESINIT] r=%d exec.library", retired);
                    32'h00fc4c56: $display("[RESINIT] r=%d expansion.library code", retired);
                    32'h00fc4840: $display("[RESINIT] r=%d disk.resource code", retired);
                    32'h00fe4314: $display("[RESINIT] r=%d misc.resource code", retired);
                    32'h00fe45cc: $display("[RESINIT] r=%d ramlib.library code", retired);
                    32'h00fc3586: $display("[RESINIT] r=%d audio.device code", retired);
                    32'h00fd3db6: $display("[RESINIT] r=%d intuition.library code", retired);
                    32'h00fc3128: $display("[RESINIT] r=%d alert.hook code", retired);
                    32'h00fe3eac: $display("[RESINIT] r=%d mathffp.library code", retired);
                    32'h00feb496: $display("[RESINIT] r=%d workbench.task entry", retired);
                    32'h00ff3e94: $display("[RESINIT] r=%d dos.library code", retired);
                    32'h00feb0a8: $display("[RESINIT] r=%d romboot.library code", retired);
                    32'h00fe8444: $display("[RESINIT] r=%d strap entry", retired);
                    32'h00fcaba2: $display("[RESINIT] r=%d graphics.library code", retired);
                    32'h00fe0a2c: $display("[RESINIT] r=%d layers.library code", retired);
                    32'h00fe97be: $display("[RESINIT] r=%d trackdisk.device code", retired);
                    32'h00fc45e0: $display("[RESINIT] r=%d cia.resource code", retired);
                    32'h00fe4424: $display("[RESINIT] r=%d potgo.resource code", retired);
                    32'h00fe7f24: $display("[RESINIT] r=%d keymap.resource code", retired);
                    32'h00fe4f44: $display("[RESINIT] r=%d keyboard.device code", retired);
                    32'h00fe53b0: $display("[RESINIT] r=%d gameport.device code", retired);
                    32'h00fe5ad0: $display("[RESINIT] r=%d input.device code", retired);
                    32'h00fe66e4: $display("[RESINIT] r=%d console.device code", retired);
                    32'h00fe8df4: $display("[RESINIT] r=%d timer.device code", retired);
                    // -- intuition init internals (to find where it bails) --
                    32'h00fd3dc0: $display("[INTU] r=%d wrapper post-JSR (MOVE.L A6,D0) A6=%h", retired, u_rf.regs[14]);
                    32'h00fd3dc4: $display("[INTU] r=%d wrapper RTS A6=%h D0=%h", retired, u_rf.regs[14], u_rf.regs[0]);
                    32'h00fd68d8: $display("[INTU] r=%d $FD68B4 RTS A6=%h", retired, u_rf.regs[14]);
                    32'h00fd68a4: $display("[INTU] r=%d $FD65E4 RTS (intuition init body) A6=%h", retired, u_rf.regs[14]);
                    // post-JSR PCs inside $FD65E4 (returned from each JSR target)
                    32'h00fd6604: $display("[INTU] r=%d post $FE0090 NewList #1", retired);
                    32'h00fd662a: $display("[INTU] r=%d post $FE0358", retired);
                    32'h00fd664e: $display("[INTU] r=%d post $FE0208", retired);
                    32'h00fd6678: $display("[INTU] r=%d post $FE0340", retired);
                    32'h00fd668c: $display("[INTU] r=%d post $FE01A0", retired);
                    32'h00fd669c: $display("[INTU] r=%d post $FE0340 #2", retired);
                    32'h00fd66b0: $display("[INTU] r=%d post $FE01A0 #2", retired);
                    32'h00fd66c2: $display("[INTU] r=%d post $FE01DC", retired);
                    32'h00fd66d0: $display("[INTU] r=%d post $FE05B4", retired);
                    32'h00fd66dc: $display("[INTU] r=%d post $FE01DC #2", retired);
                    32'h00fd66ea: $display("[INTU] r=%d post $FE0438", retired);
                    32'h00fd66f4: $display("[INTU] r=%d post $FE0438 #2", retired);
                    32'h00fd66fa: $display("[INTU] r=%d post $FD65A8 (BSR)", retired);
                    32'h00fd671a: $display("[INTU] r=%d post $FDA76E (deep init)", retired);
                    32'h00fd6790: $display("[INTU] r=%d post $FD651C (BSR)", retired);
                    32'h00fd67a2: $display("[INTU] r=%d post $FD6562 (BSR)", retired);
                    32'h00fd67c0: $display("[INTU] r=%d post $FD643A (BSR)", retired);
                    32'h00fd6846: $display("[INTU] r=%d post $FE0584", retired);
                    32'h00fd6872: $display("[INTU] r=%d post $FE01DC #3", retired);
                    32'h00fd6884: $display("[INTU] r=%d post $FDB580", retired);
                    32'h00fd6894: $display("[INTU] r=%d post $FDB5DA", retired);
                    // -- inside $FDB5DA (called from $FD688E) --
                    32'h00fdb5da: $display("[INTU] r=%d $FDB5DA ENTER D0=%h D2=%h D3=%h A2=%h", retired, u_rf.regs[0], u_rf.regs[2], u_rf.regs[3], u_rf.regs[10]);
                    32'h00fdb5fa: $display("[INTU] r=%d $FDB5DA post $FDB62A", retired);
                    32'h00fdb608: $display("[INTU] r=%d $FDB5DA post $FDB67A", retired);
                    32'h00fdb61e: $display("[INTU] r=%d $FDB5DA post $FD7F2C", retired);
                    32'h00fdb628: $display("[INTU] r=%d $FDB5DA RTS  D0=%h", retired, u_rf.regs[0]);
                    // -- inside $FDB62A ($FDB5DA helper) --
                    32'h00fdb62a: $display("[INTU] r=%d $FDB62A ENTER", retired);
                    // -- inside $FDB67A --
                    32'h00fdb67a: $display("[INTU] r=%d $FDB67A ENTER A6=%h", retired, u_rf.regs[14]);
                    32'h00fdb6ba: $display("[INTU] r=%d $FDB67A post $FE03F8 D0=%h", retired, u_rf.regs[0]);
                    32'h00fdb6cc: $display("[INTU] r=%d $FDB67A at BNE($FDB718) D0=%h", retired, u_rf.regs[0]);
                    32'h00fdb71e: $display("[INTU] r=%d $FDB67A post $FDE600 D0=%h", retired, u_rf.regs[0]);
                    32'h00fdb76e: $display("[INTU] r=%d $FDB67A at $FDB76E", retired);
                    32'h00fdb7c8: $display("[INTU] r=%d $FDB67A at JSR $FDB868", retired);
                    32'h00fdb7ce: $display("[INTU] r=%d $FDB67A post $FDB868", retired);
                    32'h00fdb7f8: $display("[INTU] r=%d $FDB67A at JSR $FDB914", retired);
                    32'h00fdb7fe: $display("[INTU] r=%d $FDB67A post $FDB914 / at JSR $FDC70A", retired);
                    32'h00fdb804: $display("[INTU] r=%d $FDB67A post $FDC70A / at JSR $FD8556", retired);
                    32'h00fdb80a: $display("[INTU] r=%d $FDB67A post $FD8556", retired);
                    32'h00fdb812: $display("[INTU] r=%d $FDB67A at BEQ ($FDB862)", retired);
                    32'h00fdb820: $display("[INTU] r=%d $FDB67A at JSR $FDD6C0", retired);
                    32'h00fdb83c: $display("[INTU] r=%d $FDB67A at JSR $FE0318 #1", retired);
                    32'h00fdb858: $display("[INTU] r=%d $FDB67A at JSR $FE0318 #2", retired);
                    32'h00fdb862: $display("[INTU] r=%d $FDB67A at MOVEM (epilogue)", retired);
                    32'h00fdb866: $display("[INTU] r=%d $FDB67A RTS  A6=%h D0=%h", retired, u_rf.regs[14], u_rf.regs[0]);
                    32'h00fde600: $display("[INTU] r=%d $FDE600 ENTER A6=%h", retired, u_rf.regs[14]);
                    32'h00fde624: $display("[INTU] r=%d $FDE600 RTS  D0=%h", retired, u_rf.regs[0]);
                    // -- inside $FDC70A (called from $FDB7FE) --
                    32'h00fdc70a: $display("[INTU] r=%d $FDC70A ENTER A6=%h", retired, u_rf.regs[14]);
                    32'h00fdc718: $display("[INTU] r=%d $FDC70A post ObtainSem(4)", retired);
                    32'h00fdc71e: $display("[INTU] r=%d $FDC70A post $FDC8AC", retired);
                    32'h00fdc73c: $display("[INTU] r=%d $FDC70A post list-walk loop", retired);
                    32'h00fdc746: $display("[INTU] r=%d $FDC70A post ReleaseSem(4)", retired);
                    32'h00fdc74c: $display("[INTU] r=%d $FDC70A post $FDC754", retired);
                    32'h00fdc752: $display("[INTU] r=%d $FDC70A RTS", retired);
                    32'h00fdc754: $display("[INTU] r=%d $FDC754 ENTER (sem #5 wrapper)", retired);
                    32'h00fdc764: $display("[INTU] r=%d $FDC754 post $FDC7E6 (about to $FDC778)", retired);
                    32'h00fdc76a: $display("[INTU] r=%d $FDC754 post $FDC778", retired);
                    32'h00fdc776: $display("[INTU] r=%d $FDC754 RTS", retired);
                    32'h00fdc7e6: $display("[INTU] r=%d $FDC7E6 ENTER", retired);
                    32'h00fdc778: $display("[INTU] r=%d $FDC778 ENTER A6=%h", retired, u_rf.regs[14]);
                    32'h00fdc788: $display("[INTU] r=%d $FDC778 post ObtainSem(4)", retired);
                    32'h00fdc7a2: $display("[INTU] r=%d $FDC778 post $FE0460", retired);
                    32'h00fdc7ac: $display("[INTU] r=%d $FDC778 post $FE0488", retired);
                    32'h00fdc7b2: $display("[INTU] r=%d $FDC778 post $FE04F4", retired);
                    32'h00fdc7c2: $display("[INTU] r=%d $FDC778 post DMACON writes", retired);
                    32'h00fdc7ca: $display("[INTU] r=%d $FDC778 post $FE06E4 #1", retired);
                    32'h00fdc7d2: $display("[INTU] r=%d $FDC778 post $FE06E4 #2", retired);
                    32'h00fdc7dc: $display("[INTU] r=%d $FDC778 post ReleaseSem(4)", retired);
                    32'h00fdc7e4: $display("[INTU] r=%d $FDC778 RTS", retired);
                    // gfx LVO wrappers in Intuition
                    32'h00fe0460: $display("[INTU] r=%d $FE0460 wrapper ENTER", retired);
                    32'h00fe0488: $display("[INTU] r=%d $FE0488 wrapper ENTER", retired);
                    32'h00fe04f4: $display("[INTU] r=%d $FE04F4 wrapper ENTER", retired);
                    32'h00fe06e4: $display("[INTU] r=%d $FE06E4 wrapper ENTER", retired);
                    // vec=8 trap source (Supervisor() call site)
                    32'h00fc08e6: $display("[INTU] r=%d at $FC08E6 (ORI.W #2000,SR)", retired);
                    // -- ObtainSemaphore at $FC2DF0 --
                    32'h00fc2df0: $display("[INTU] r=%d ObtainSem ENTER A0=%h ThisTask=$1970", retired, u_rf.regs[8]);
                    32'h00fc2e36: $display("[INTU] r=%d ObtainSem succeeded (recursive or first)", retired);
                    32'h00fc2e3e: $display("[INTU] r=%d ObtainSem RTS", retired);
                    32'h00fc2e10: $display("[INTU] r=%d ObtainSem WAIT path (BLOCKING)", retired);
                    32'h00fc2e24: $display("[INTU] r=%d ObtainSem BSR $FC1624 (AddTail to wait list)", retired);
                    32'h00fc2e2a: $display("[INTU] r=%d ObtainSem about to call Wait #16", retired);
                    // $FE03F8 wrapper: load A6 from $64(A6), then JSR $FFB8(A6)
                    32'h00fe03f8: $display("[INTU] r=%d $FE03F8 wrapper ENTER A6=%h", retired, u_rf.regs[14]);
                    32'h00fe0406: $display("[INTU] r=%d $FE03F8 post JSR FFB8(A6) D0=%h", retired, u_rf.regs[0]);
                    default: ;
                endcase
            end
            // Exception launches happen in S_RUN with exc_launch_c set --
            // that gates run_launches_exc and disables is_settled_in_run,
            // so is_settled and exc_launch_c are mutually exclusive.  Use
            // the raw S_RUN gate to trace launches.
            if (ex_state == S_RUN && ex_valid && !halted && exc_launch_c)
                $display("[EXC] r=%d pc=%h opcode=%h kind=%d vec=%d",
                    retired, ex_pc, ex_opcode, ex_kind, exc_vector_c);
            if (is_settled && retired < 32'd20)
                $display("[Early] r=%d pc=%h opcode=%h",
                    retired, ex_pc, ex_opcode);
`endif
`ifdef KICKSTART_COSIM_TRACE
            // Per-instruction state dump for the Musashi cosim diff.
            // Format: `[Cosim] <retired> <pc> <opcode> <D0..D7> <A0..A7> <SR>`
            //
            // Window-gated: when `+cosim_start=N` / `+cosim_end=N` are set
            // on the command line, the trace fires only for retired in
            // [N, M].  Lets `make cosim-window` capture a focused trace
            // matching the snapshot+resume Musashi trace.
            if (is_settled && retired != cosim_last_retired &&
                retired >= cosim_start_r && retired <= cosim_end_r) begin
                $display("[Cosim] %0d %06h %04h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %08h %04h",
                    retired, ex_pc, ex_opcode,
                    u_rf.regs[0],  u_rf.regs[1],  u_rf.regs[2],  u_rf.regs[3],
                    u_rf.regs[4],  u_rf.regs[5],  u_rf.regs[6],  u_rf.regs[7],
                    u_rf.regs[8],  u_rf.regs[9],  u_rf.regs[10], u_rf.regs[11],
                    u_rf.regs[12], u_rf.regs[13], u_rf.regs[14], u_rf.regs[15],
                    {1'b0, sr_t, 1'b0, sr_s, 2'b0, sr_i, 3'b0,
                     cc_x, cc_n, cc_z, cc_v, cc_c});
                cosim_last_retired <= retired;
                if (retired == cosim_end_r) $finish;
            end
            // Memory-write trace.  Fires when a write completes (dc_ack
            // with dc_we=1) on chip RAM (< 512 KB) OR slow RAM
            // ($C00000..$C7FFFF).  Format:
            //   [CosimW] <retired> <addr> <size> <data>
            // Size pulled from dc_be (which is what the bus actually
            // commits): 1111 = 4 bytes long-write, 1100/0011 = 2 bytes
            // word-write, single bit = 1 byte.  dc_is_long is only set
            // for true .L MOVEs; BSR/JSR/exception pushes use be=1111
            // without is_long but still write four bytes.
            if (dc_req_r && dc_we && dc_ack &&
                (dc_addr < 32'h0008_0000 ||
                 (dc_addr >= 32'h00C0_0000 && dc_addr < 32'h00C8_0000)) &&
                retired >= cosim_start_r && retired <= cosim_end_r) begin
                if (dc_be == 4'b1111)
                    $display("[CosimW] %0d %06h 4 %08h", retired, dc_addr, dc_wdata);
                else if (dc_be == 4'b1100)
                    $display("[CosimW] %0d %06h 2 %04h", retired, dc_addr, dc_wdata[31:16]);
                else if (dc_be == 4'b0011)
                    $display("[CosimW] %0d %06h 2 %04h", retired, dc_addr | 32'd2, dc_wdata[15:0]);
                else if (dc_be == 4'b1000)
                    $display("[CosimW] %0d %06h 1 %02h", retired, dc_addr, dc_wdata[31:24]);
                else if (dc_be == 4'b0100)
                    $display("[CosimW] %0d %06h 1 %02h", retired, dc_addr | 32'd1, dc_wdata[23:16]);
                else if (dc_be == 4'b0010)
                    $display("[CosimW] %0d %06h 1 %02h", retired, dc_addr | 32'd2, dc_wdata[15:8]);
                else if (dc_be == 4'b0001)
                    $display("[CosimW] %0d %06h 1 %02h", retired, dc_addr | 32'd3, dc_wdata[7:0]);
            end
            // [CosimR]: chipset reads inside the window.  Captures every
            // value the CPU saw at custom-chip ($DFF000..$DFFFFF), CIA
            // ($BFD000..$BFEFFF), or autoconfig ($E80000..$E8FFFF)
            // addresses.  Musashi resume replays these so the post-
            // snapshot path is deterministic regardless of Musashi's
            // chipset stubs.
            //   [CosimR] <retired> <addr> <size> <value>
            if (dc_req_r && !dc_we && dc_ack &&
                retired >= cosim_start_r && retired <= cosim_end_r &&
                ((dc_addr >= 32'h00DF_F000 && dc_addr <= 32'h00DF_FFFF) ||
                 (dc_addr >= 32'h00BF_D000 && dc_addr <= 32'h00BF_FFFF) ||
                 (dc_addr >= 32'h00E8_0000 && dc_addr <= 32'h00E8_FFFF))) begin
                if (dc_be == 4'b1111)
                    $display("[CosimR] %0d %06h 4 %08h", retired, dc_addr, dc_rdata);
                else if (dc_be == 4'b1100)
                    $display("[CosimR] %0d %06h 2 %04h", retired, dc_addr, dc_rdata[31:16]);
                else if (dc_be == 4'b0011)
                    $display("[CosimR] %0d %06h 2 %04h", retired, dc_addr | 32'd2, dc_rdata[15:0]);
                else if (dc_be == 4'b1000)
                    $display("[CosimR] %0d %06h 1 %02h", retired, dc_addr, dc_rdata[31:24]);
                else if (dc_be == 4'b0100)
                    $display("[CosimR] %0d %06h 1 %02h", retired, dc_addr | 32'd1, dc_rdata[23:16]);
                else if (dc_be == 4'b0010)
                    $display("[CosimR] %0d %06h 1 %02h", retired, dc_addr | 32'd2, dc_rdata[15:8]);
                else if (dc_be == 4'b0001)
                    $display("[CosimR] %0d %06h 1 %02h", retired, dc_addr | 32'd3, dc_rdata[7:0]);
            end
            // [CosimI]: IRQ events inside the window.  Auto-vectored
            // 68000 IRQs land at vec=25..31 (level 1..7).  Trap
            // exceptions (vec<25) come from CPU state Musashi reproduces
            // on its own, so we only replay IRQs.  Musashi's
            // `--replay=` consumer triggers `m68k_set_irq(level)` at
            // the matching retired count.
            //   [CosimI] <retired> <vec> <level>
            if (ex_state == S_RUN && ex_valid && !halted && exc_launch_c &&
                retired >= cosim_start_r && retired <= cosim_end_r &&
                exc_vector_c >= 8'd25 && exc_vector_c <= 8'd31) begin
                $display("[CosimI] %0d %0d %0d",
                    retired, exc_vector_c, exc_vector_c - 8'd24);
            end
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
`ifdef KICKSTART_BOOT_TRACE
            // Watch USP being assigned a small value — likely stack overflow
            // or dispatch into a task with garbage tc_SPReg.  $200 picks up
            // anything close to the $00000008 we've been chasing.
            if (is_settled && usp_we_c && usp_data_c < 32'h0000_0200)
                $display("[USPLOW] r=%d pc=%h usp_new=%h kind=%d (via MOVE An,USP)",
                    retired, ex_pc, usp_data_c, ex_kind);
            // Also watch the SR-write S->U path that captures live A7.
            if (is_settled && sr_we_c && sr_s &&
                sr_data_c[`SR_S] == 1'b0 && rf_rc_data < 32'h0000_0200)
                $display("[USPLOW2] r=%d pc=%h usp_new=%h (via MOVE-to-SR S->U)",
                    retired, ex_pc, rf_rc_data);
`endif

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
                // Only log on first-touch -- this block fires every cycle
                // STOP is in S_RUN, which would flood the log otherwise.
                if (ex_pc != stop_logged_pc)
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
                        // MOVEM: capture mask + initial address.
                        //
                        // Layout of ex_src_imm32 depends on src_ext_words:
                        //   ext_words=1 (predec/AIND/postinc):
                        //     ex_src_imm32 = sign_ext(mask) -- mask at [15:0]
                        //   ext_words>=2 (d16(An)/d8(An,Xn)/abs/pcrel):
                        //     ex_src_imm32 = {mask, disp} -- mask at [31:16],
                        //     disp at [15:0]
                        // Detect by ex_src_mode (since the predec/AIND/AINC
                        // modes are exactly the ones with ext_words=1 for
                        // MOVEM).
                        //
                        // Mask bits map to registers differently for predec:
                        //   predec:    mask[0]=A7, ..., mask[15]=D0
                        //   other:     mask[0]=D0, ..., mask[15]=A7
                        movem_mask    <= ((ex_src_mode == `EA_AIND) ||
                                          (ex_src_mode == `EA_AINC) ||
                                          (ex_src_mode == `EA_ADEC))
                                         ? ex_src_imm32[15:0]
                                         : ex_src_imm32[31:16];
                        movem_idx     <= 5'd0;
                        movem_dir     <= ex_direction;
                        movem_predec  <= (ex_src_mode == `EA_ADEC);
                        movem_an_idx  <= ex_src_reg;
                        movem_an_update <= (ex_src_mode == `EA_ADEC) ||
                                           (ex_src_mode == `EA_AINC);
                        // Initial address: for predec the tracked An starts
                        // at the original An (each step uses An-4 then
                        // decrements).  For d16(An), add the sign-extended
                        // displacement (which lives at ex_src_imm32[15:0]
                        // because the mask sits in the high half).  For
                        // (An)/(An)+, just An.  For d8(An,Xn), add the brief
                        // extension's index (ex_sp routed via the IDX mux)
                        // and 8-bit displacement -- Kickstart 1.3's exec
                        // init uses MOVEM.L D1/A1, $54(A6, D3.W) at $F817C0
                        // to initialise IntVects[3,5,4,13,15].
                        //
                        // For EA_EXT/EA7_ABSW: ex_src_imm32 holds {mask,
                        // abs.W} so the abs.W is in the low 16 bits and
                        // sign-extended forms the absolute address.
                        // Surfaced in K1.3 at $FC2FF0:
                        //   MOVEM.L D0-D7/A0-A7, $0180.W
                        // which clears 64 bytes starting at $180 (the
                        // exec vector save area).  Without this case the
                        // MOVEM collapsed to addr = ex_ra (= some register
                        // contents) and clobbered random low memory
                        // including ExecBase at $4, so the warm-reset
                        // trampoline at $FC05FC then JMPed through a
                        // garbage ExecBase to $FFFFFFFF.
                        case (ex_src_mode)
                            `EA_ADEC:
                                movem_addr <= ex_ra;
                            `EA_DISP:
                                movem_addr <= ex_ra +
                                    {{16{ex_src_imm32[15]}}, ex_src_imm32[15:0]};
                            `EA_IDX:
                                movem_addr <= ex_ra
                                    + (ex_src_imm32[11] ? ex_sp
                                        : {{16{ex_sp[15]}}, ex_sp[15:0]})
                                    + {{24{ex_src_imm32[7]}}, ex_src_imm32[7:0]};
                            `EA_EXT: begin
                                // ABSW: low 16 of ex_src_imm32, sign-ext.
                                // PCDISP: ex_pc + 4 + sign-ext(d16).  The +4
                                //   accounts for MOVEM's two extension words
                                //   (mask at opcode+2, d16 at opcode+4); the
                                //   68k convention is that d16(PC) is relative
                                //   to the address of the d16 word itself.
                                //   Surfaced by the WB1.3 boot wall at
                                //   chip-RAM $5D82: `MOVEM.L $FFC4(PC), A2/A5/A6`
                                //   used to fall into the `else movem_addr <=
                                //   ex_ra` branch, dropping A2/A5/A6 to a
                                //   random source ($26 area = trap-vector
                                //   table) instead of the intended $5D4A.
                                //   A5 then held $082800FC and the next JSR (A5)
                                //   started the BAD-PC cascade.
                                // ABSL/PCIDX still untouched (the ABSL case
                                //   needs the third ext word that doesn't
                                //   survive into ex_src_imm32 -- known
                                //   limitation; K1.3 doesn't use ABSL MOVEM).
                                if (ex_src_reg == `EA7_ABSW)
                                    movem_addr <=
                                        {{16{ex_src_imm32[15]}}, ex_src_imm32[15:0]};
                                else if (ex_src_reg == `EA7_PCDISP)
                                    movem_addr <= ex_pc + 32'd4 +
                                        {{16{ex_src_imm32[15]}}, ex_src_imm32[15:0]};
                                else
                                    movem_addr <= ex_ra;
                            end
                            default:
                                movem_addr <= ex_ra;
                        endcase
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
                        end else if (alui_mem_writes || alu_mem_dst_writes) begin
                            // Mem-dest ADDI/SUBI/ANDI/ORI/EORI (K_ALUI),
                            // ADDQ/SUBQ <ea> (K_ALUQ), or ADD/SUB/AND/OR/EOR
                            // Dn,<ea> (K_ALU direction=1).  alu_y has the new
                            // value (computed in S_LOAD planning).  Capture
                            // both result and CCR snapshot; S_RMW_W settle
                            // commits the writeback + CCR.
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
                        end else if (shift_mem_writes) begin
                            // K_SHIFT memory destination — capture the
                            // shifted word + CCR snapshot, issue the
                            // 16-bit write-back (memory shifts are .W).
                            ex_tas_word <= alu_y;
                            ex_alui_n_q <= alu_n;
                            ex_alui_z_q <= alu_z;
                            ex_alui_v_q <= alu_v;
                            ex_alui_c_q <= alu_c;
                            ex_alui_x_q <= alu_x;
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b1;
                            dc_be    <= be_for_word(dc_addr[1]);
                            dc_wdata <= word_into_word(alu_y[15:0], dc_addr[1]);
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
                        end else if (ex_kind == K_CMPM) begin
                            // CMPM (Ay)+, (Ax)+: capture the first load's
                            // size-extracted data and issue the second load
                            // at Ax.  The compare + CCR commit happens at
                            // S_CMPM_LOAD2 ack.  Ay post-inc has already
                            // been driven via wb_aux above (combinational).
                            case (ex_size)
                                `SZ_B: cmpm_ay_data <=
                                    {24'd0, byte_at(dc_rdata, dc_addr[1:0])};
                                `SZ_W: cmpm_ay_data <=
                                    {16'd0, dc_addr[1] ? dc_rdata[15:0]
                                                       : dc_rdata[31:16]};
                                default: cmpm_ay_data <= dc_rdata;
                            endcase
                            dc_req_r <= 1'b1;
                            dc_we    <= 1'b0;
                            dc_lock  <= 1'b0;
                            dc_addr  <= dst_ea;
                            dc_be    <= 4'b1111;
                            dc_is_long <= (ex_size == `SZ_L);
                            ex_state <= S_CMPM_LOAD2;
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
                S_CMPM_LOAD2: begin
                    if (dc_ack) begin
                        dc_req_r <= 1'b0;
                        dc_lock  <= 1'b0;
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
                            if (movem_predec) movem_addr <= movem_addr - movem_step_bytes;
                            else              movem_addr <= movem_addr + movem_step_bytes;
`ifdef KICKSTART_BOOT_TRACE
                            if (ex_pc == 32'h00FE_8F88 || ex_pc == 32'h00FE_8DF4)
                                $display("[MOVEM-X] r=%d pc=%h step ack idx=%d mask=%h addr=%h dir=%b",
                                    retired, ex_pc, movem_idx, movem_mask, movem_addr, movem_dir);
`endif
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
                            // MOVEM.L is a long-aligned transfer in size but
                            // the SP at entry can be word-aligned-not-mod-4
                            // (e.g. after the InitResident driver's LINK A5,
                            // #-$0E leaves SP at $3D6 and the inner -(SP)
                            // pushes hit $3CA/$3C6).  We need dc_is_long set
                            // so the bus's unaligned-.L assembly path fires
                            // on the read response; otherwise MOVEM-load
                            // reads return mem[idx] verbatim (= the wrong
                            // half) and registers get restored to garbage.
                            // MOVEM.W uses word byte enables selected by
                            // the step address's bit 1.
                            if (ex_size == `SZ_L) begin
                                dc_be      <= 4'b1111;
                                dc_is_long <= 1'b1;
                                if (movem_dir == 1'b0) dc_wdata <= rf_ra_data;
                            end else begin
                                dc_be      <= be_for_word(movem_step_addr[1]);
                                dc_is_long <= 1'b0;
                                if (movem_dir == 1'b0)
                                    dc_wdata <= word_into_word(rf_ra_data[15:0],
                                                               movem_step_addr[1]);
                            end
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
                        // immediately below PC, total 6 bytes.  Byte enables
                        // depend on the alignment of (working_sp - 2) within
                        // the 4-byte mem[] word:
                        //   addr[1]=0 (= ends in $0/$4/$8/$C):  high half of
                        //     mem[idx] -> be=4'b1100, wdata = SR << 16
                        //   addr[1]=1 (= ends in $2/$6/$A/$E):  low half of
                        //     mem[idx] -> be=4'b0011, wdata = SR
                        // Previously we unconditionally used be=0011 + low
                        // half, which silently overwrote bytes 2-3 of the
                        // just-pushed PC whenever working_sp-2 had addr[1]=0.
                        // Symptom: K1.3 boot wall at r=2080000 -- RTE popped
                        // $001C08F4 instead of the pushed $00FC08F4 (high
                        // byte $FC replaced by SR's low byte $1C).  This
                        // alignment scenario happens whenever pre-exception
                        // SSP[1:0]=10 (16-bit aligned but not 32-bit), which
                        // recurs once every 6 bytes of nested exception
                        // pushes.
                        working_sp <= working_sp - 32'd2;
                        dc_req_r <= 1'b1;
                        dc_we    <= 1'b1;
                        dc_addr  <= working_sp - 32'd2;
                        if ((working_sp - 32'd2) & 32'd2) begin
                            dc_wdata <= {16'd0, ex_exc_saved_sr};
                            dc_be    <= 4'b0011;
                        end else begin
                            dc_wdata <= {ex_exc_saved_sr, 16'd0};
                            dc_be    <= 4'b1100;
                        end
                        ex_state <= S_EXC_PUSH_SR;
                    end
                end
                S_EXC_PUSH_SR: begin
                    if (dc_ack) begin
                        // Enforce supervisor mode (matches old behavior).
                        // Capture USP (= live A7 when we entered the exception
                        // from user mode) into usp_shadow.  Read A7 directly
                        // via the regfile -- rf_rc_data routes through
                        // id_rc_idx which is decoded from the *preempted*
                        // instruction and points at its src/dst Xn register
                        // when that instruction used IDX mode (e.g. MOVE.B
                        // ..,d8(An,Dn) preempted at $FC6BF6 by a level-3 IRQ
                        // captured D0=$0 instead of A7=$188A, propagating
                        // a corrupt USP=$0 into the next dispatch and pulling
                        // the saved-PC pop into top-of-ROM garbage).
                        if (ex_exc_was_user) usp_shadow <= u_rf.regs[15];
`ifdef KICKSTART_BOOT_TRACE
                        if (ex_exc_was_user && u_rf.regs[15] < 32'h0000_0200)
                            $display("[USPLOW3] r=%d pc=%h usp_captured=%h (via EXC from user)",
                                retired, ex_pc, u_rf.regs[15]);
`endif
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
`ifdef KICKSTART_BOOT_TRACE
                        $display("[VEC] r=%d vec=%d addr=%h rdata=%h",
                            retired, ex_exc_vector, dc_addr, dc_rdata);
`endif
                        ex_state <= S_RUN;
                    end
                end
                S_RTE_POP_SR: begin
                    if (dc_ack) begin
                        // Bus returns the full 32-bit aligned word containing
                        // the SR.  SR.W lives at bytes 0-1 (addr[1]=0,
                        // high half = dc_rdata[31:16]) OR at bytes 2-3
                        // (addr[1]=1, low half = dc_rdata[15:0]) depending
                        // on which half of the aligned word working_sp
                        // points into.  Previously we assumed [1:0]=10 and
                        // always read [15:0]; this was wrong whenever SSP
                        // entered the exception .L-not-mod-4-aligned (which
                        // happens once every 6 bytes of nested exception
                        // depth -- pre-S42 it never came up because the
                        // dispatcher couldn't progress past STOP).
                        if (working_sp & 32'd2) begin
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
                        end else begin
                            ex_exc_saved_sr <= dc_rdata[31:16];
                            if (ex_kind == K_RTE) begin
                                sr_t <= dc_rdata[16+`SR_T];
                                sr_s <= dc_rdata[16+`SR_S];
                                sr_i <= dc_rdata[16+10:16+8];
                            end
                            cc_x <= dc_rdata[16+`SR_X];
                            cc_n <= dc_rdata[16+`SR_N];
                            cc_z <= dc_rdata[16+`SR_Z];
                            cc_v <= dc_rdata[16+`SR_V];
                            cc_c <= dc_rdata[16+`SR_C];
                        end
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
