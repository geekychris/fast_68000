// Canonical-Amiga Copper (S41 rewrite).
//
// Two-word instruction encoding, exactly as on real Agnus.  Each
// instruction is 4 bytes = two 16-bit words IR1, IR2, stored in chip
// RAM big-endian-word-addressed.  A single 32-bit aligned bus read at
// `pc` yields `{IR1, IR2}` because chip RAM is BE: the byte sequence
// (b0 b1 b2 b3) at address `pc` is read as 32'h{b0,b1,b2,b3}, with
// IR1 = high half ([31:16]) and IR2 = low half ([15:0]).
//
// Instruction decode:
//   IR1[0] = 0  MOVE
//     reg  = IR1[8:1] & 0x1FE          (16-bit chipset register offset)
//     data = IR2                        (16-bit value)
//     -> bus write of `IR2` to address $00DF_F000 | reg (be=4'b0011).
//        The bus arbiter already decodes $DFF000..$DFF1FF for the
//        chipset.  Special registers handled internally without a
//        master cycle:
//          $088 COPJMP1   -> PC <- cop1lc, restart S_FETCH
//          $08A COPJMP2   -> PC <- cop2lc, restart S_FETCH
//          $08C COPINS    -> accept and discard (no-op)
//          $08E COPCON    -> accept and discard (no-op)
//
//   IR1[0] = 1, IR2[0] = 0  WAIT
//     vp_target = IR1[15:8]
//     hp_target = IR1[7:1]
//     vp_mask   = IR2[14:8]
//     hp_mask   = IR2[6:1]
//     Stall until ((vbeam_v & vp_mask) >= vp_target)
//                 AND ((vbeam_h & hp_mask) >= hp_target).
//     End-of-list sentinel: $FFFE, $FFFE (vp=$FF h=$7F mask=$7F/$3F)
//     can never be reached -> Copper stalls until reset/COPJMP.
//
//   IR1[0] = 1, IR2[0] = 1  SKIP
//     Same compare as WAIT.  If the condition is currently TRUE
//     (raster already past the target position), skip the next 4
//     bytes (advance PC by 4 instead of just continuing).
//
// vbeam_i carries the Denise raster row (V) only -- there is no Agnus
// H-position signal in this codebase yet.  The H comparison in WAIT/
// SKIP is therefore treated as ALWAYS SATISFIED for now: the V
// comparison alone decides.  This is sufficient for every line-grain
// raster trick a Copper list does, and matches the simplification
// already used by `tools/render_k13_screen.py`.  TODO: wire a real H
// counter (e.g. from Agnus when it lands) and tighten the compare.
//
// Slave port (legacy harness compat, kept verbatim) at
// $00FE_0040..$00FE_007F:
//   $00FE_0040 COP1LC   RW   list-1 byte address
//   $00FE_0044 COPJMP1  W    strobe to (re)start at COP1LC
//   $00FE_0048 COPSTAT  RO   bit 0 = COP_BUSY
//   $00FE_004C COP2LC   RW   list-2 byte address
//   $00FE_0050 COPJMP2  W    strobe to (re)start at COP2LC
//
// The bus translates canonical $DFF080..$DFF08A CPU writes into the
// equivalent slave-port writes (see m68k_bus.v cop_xlat_*), so user
// code can program the Copper either way.

module copper (
    input  wire        clk,
    input  wire        rst_n,

    // CPU slave port
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [5:0]  slv_addr,    // byte offset $40..$7F (we see $00..$3F via the mask)
    input  wire [3:0]  slv_be,
    input  wire [31:0] slv_wdata,
    output reg  [31:0] slv_rdata,

    // Bus master (read instructions + execute MOVE writes)
    output wire        mst_req,
    output reg         mst_we,
    output reg  [31:0] mst_addr,
    output reg  [31:0] mst_wdata,
    output reg  [3:0]  mst_be,
    input  wire        mst_ack,
    input  wire [31:0] mst_rdata,

    // Live blitter-busy signal (unused under canonical encoding but
    // kept on the port for legacy harness compat).
    input  wire        blt_busy_i,

    // Current Denise raster row.  Carries V only; H is unavailable in
    // this codebase, so WAIT/SKIP treat the H comparator as always
    // satisfied (see header comment).
    input  wire [15:0] vbeam_i,
    input  wire        auto_kick_i,
    input  wire        auto_active_i,
    // COPCON.CDANG: 0 = Copper restricted to chip regs $80+ (default OCS);
    // 1 = gate opens to $40+ so Copper-driven blitter setup works.  $00-$3E
    // is always blocked at the chipset level.
    input  wire        cdang_i
);
    // ---------------- Registers programmed by the CPU ----------------
    reg [31:0] cop1lc;
    reg [31:0] cop2lc;
    reg        cop_busy;

    // Silence Verilator unused-input warning on blt_busy_i.  Canonical
    // Copper does not consult the blitter (WAIT-BLITTER no longer
    // exists in this encoding); the port stays for harness symmetry.
    wire _unused_blt_busy = blt_busy_i;

    // ---------------- Internal state ---------------------------------
    localparam S_IDLE       = 3'd0;
    localparam S_FETCH      = 3'd1;   // single 32-bit fetch -> {IR1, IR2}
    localparam S_DECODE     = 3'd2;
    localparam S_WRITE      = 3'd3;   // MOVE: bus write of IR2 to $DFFxxx
    localparam S_WAIT_RAS   = 3'd4;   // WAIT: stall until raster compare
    localparam S_HALT       = 3'd5;

    reg [2:0]  state;
    reg [31:0] pc;
    reg [15:0] ir1_q;
    reg [15:0] ir2_q;
    reg        auto_kick_d;

    // Master-request gating: drop combinationally on ack so the arbiter
    // sees one request per transaction.
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

    // ---------------- WAIT / SKIP compare helpers --------------------
    wire [7:0] vp_target = ir1_q[15:8];
    wire [6:0] hp_target = ir1_q[7:1];
    wire [6:0] vp_mask   = ir2_q[14:8];   // [14:8] = 7 bits
    wire [5:0] hp_mask   = ir2_q[6:1];    // [6:1]  = 6 bits
    wire       skip_bit  = ir2_q[0];      // 1 = SKIP, 0 = WAIT

    // V-only compare: ((vbeam_v & vp_mask) >= vp_target).  Take vbeam_i's
    // low 8 bits as the current V (Denise row counter); mask with the
    // 7-bit vp_mask zero-extended to 8 bits (HRM uses VP=8 bits; high
    // bit is always enabled in real Agnus comparator, which matches
    // masking with 0xFF in our simplified model when vp_mask has the
    // high bit implicitly enabled.  We follow CopperSim and ignore the
    // mask's high-bit subtlety; using only the 7 low mask bits is fine
    // for end-of-list ($FF,$FE) and for any V threshold under 128).
    wire [7:0] vbeam_v = vbeam_i[7:0];
    wire [7:0] vp_mask_ext = {1'b1, vp_mask};   // top bit always enabled
    wire       v_match    = ((vbeam_v & vp_mask_ext) >= vp_target);
    // H comparator stubbed as always satisfied (see header comment).
    wire       h_match    = 1'b1;
    // Suppress Verilator unused-bit warning on hp_target / hp_mask:
    wire _unused_hp = |{hp_target, hp_mask};
    wire       raster_match = v_match && h_match;

    // ---------------- IR1 decode --------------------------------------
    wire       is_move = !ir1_q[0];
    wire [8:0] mv_reg  = {ir1_q[8:1], 1'b0};   // even byte offset 0..$1FE

    // Special-register handling for MOVE.  These reload PC or noop
    // without issuing any bus master write.
    wire is_copjmp1 = is_move && (mv_reg == 9'h088);
    wire is_copjmp2 = is_move && (mv_reg == 9'h08A);
    wire is_copins  = is_move && (mv_reg == 9'h08C);
    wire is_copcon  = is_move && (mv_reg == 9'h08E);

    // Silence Verilator unused-input warning on slv_be (legacy port
    // accepted byte enables but the canonical Copper treats all slave
    // accesses as long-word).
    wire _unused_slv_be = |slv_be;

    // ---------------- CPU slave reads --------------------------------
    always @* begin
        case (slv_addr[4:2])
            3'd0: slv_rdata = cop1lc;             // $40 COP1LC
            3'd1: slv_rdata = 32'd0;              // $44 COPJMP1 (write-only)
            3'd2: slv_rdata = {31'd0, cop_busy};  // $48 COPSTAT
            3'd3: slv_rdata = cop2lc;             // $4C COP2LC
            3'd4: slv_rdata = 32'd0;              // $50 COPJMP2 (write-only)
            default: slv_rdata = 32'd0;
        endcase
    end

    // ---------------- Main always block ------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cop1lc    <= 32'd0;
            cop2lc    <= 32'd0;
            cop_busy  <= 1'b0;
            state     <= S_IDLE;
            pc        <= 32'd0;
            ir1_q     <= 16'd0;
            ir2_q     <= 16'd0;
            mst_req_r <= 1'b0;
            mst_we    <= 1'b0;
            mst_addr  <= 32'd0;
            mst_wdata <= 32'd0;
            mst_be    <= 4'b0000;
            auto_kick_d <= 1'b0;
        end else begin
            auto_kick_d <= auto_kick_i;
            if (auto_kick_i && !auto_kick_d && auto_active_i && !cop_busy) begin
                pc       <= cop1lc;
                cop_busy <= 1'b1;
                state    <= S_FETCH;
            end

            // -------- CPU slave writes --------
            if (slv_req && slv_we) begin
                case (slv_addr[4:2])
                    3'd0: cop1lc <= slv_wdata;        // COP1LC
                    3'd1: begin                        // COPJMP1: strobe-start
                        if (!cop_busy) begin
                            pc       <= cop1lc;
                            cop_busy <= 1'b1;
                            state    <= S_FETCH;
                        end
                    end
                    3'd3: cop2lc <= slv_wdata;        // COP2LC
                    3'd4: begin                        // COPJMP2: strobe-start
                        if (!cop_busy) begin
                            pc       <= cop2lc;
                            cop_busy <= 1'b1;
                            state    <= S_FETCH;
                        end
                    end
                    default: ;
                endcase
            end

            // -------- Copper state machine --------
            case (state)
                S_IDLE: begin
                    mst_req_r <= 1'b0;
                end

                S_FETCH: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b0;
                    mst_addr  <= pc;
                    mst_be    <= 4'b1111;
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        ir1_q     <= mst_rdata[31:16];
                        ir2_q     <= mst_rdata[15:0];
                        pc        <= pc + 32'd4;
                        state     <= S_DECODE;
                    end
                end

                S_DECODE: begin
                    if (is_move) begin
                        if (is_copjmp1) begin
                            pc    <= cop1lc;
                            state <= S_FETCH;
                        end else if (is_copjmp2) begin
                            pc    <= cop2lc;
                            state <= S_FETCH;
                        end else if (is_copins || is_copcon) begin
                            // Discard silently; advance.
                            state <= S_FETCH;
                        end else if ((mv_reg < 9'h040) ||
                                     ((mv_reg < 9'h080) && !cdang_i)) begin
                            // Copper register-access protection.
                            //   * $00-$3E: always blocked (disk DMA,
                            //     serial, INTREQ, COPxxx, vector regs).
                            //   * $40-$7E: blocked when COPCON.CDANG=0
                            //     (default OCS); allowed when CDANG=1
                            //     so software can program the blitter
                            //     from the Copper (t24_cop_chain etc.).
                            //   * $80+: always allowed (Denise: BPLnPT,
                            //     BPLCON, SPRnPT/POS/CTL/DATA, COLOR).
                            //
                            // Without this gate, a Copper that walks past
                            // a valid list into chip-RAM code finds opcodes
                            // like $4EF9 (JMP) and interprets them as MOVEs
                            // to DSKLEN ($DFF024) / BLTDPTL ($DFF056) /
                            // etc., clearing DMAEN and corrupting in-
                            // progress disk reads + blits.  See
                            // project_copper_autorestart.md.
                            state <= S_FETCH;
                        end else begin
                            // Real MOVE: emit canonical $DFFxxx bus write.
                            state <= S_WRITE;
                        end
                    end else if (skip_bit) begin
                        // SKIP: if raster already past target, skip next
                        // instruction (4 more bytes).
                        if (raster_match) pc <= pc + 32'd4;
                        state <= S_FETCH;
                    end else if (ir1_q == 16'hFFFF && ir2_q == 16'hFFFE) begin
                        // Canonical end-of-list sentinel: WAIT vp=$FF.
                        // The condition can never be satisfied since vbeam
                        // maxes at FB_H-1 (192 << 255), so on real Agnus
                        // the Copper stalls here until VBLANK restart.  In
                        // our test harness we clear cop_busy so the CPU
                        // polling COPSTAT can proceed.  This is a strict
                        // superset of real semantics: the dead-end WAIT
                        // really would block forever anyway.
                        state <= S_HALT;
                    end else begin
                        // WAIT
                        state <= S_WAIT_RAS;
                    end
                end

                S_WRITE: begin
                    // Canonical Amiga chipset registers are 16-bit, packed
                    // 2/long.  $DFFxx0/$DFFxx4/$DFFxx8/$DFFxxC are the high
                    // halves of their long, $DFFxx2/$DFFxx6/$DFFxxA/$DFFxxE
                    // the low halves.  Place IR2 in the matching half and
                    // set BE accordingly so we don't clobber the other 16-
                    // bit register sharing the same long.
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b1;
                    mst_addr  <= 32'h00DF_F000 | {23'd0, mv_reg};
                    if (mv_reg[1]) begin
                        // Low half of long: $DFFxx2/$DFFxx6/$DFFxxA/$DFFxxE.
                        mst_wdata <= {16'd0, ir2_q};
                        mst_be    <= 4'b0011;
                    end else begin
                        // High half of long: $DFFxx0/$DFFxx4/$DFFxx8/$DFFxxC.
                        mst_wdata <= {ir2_q, 16'd0};
                        mst_be    <= 4'b1100;
                    end
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        state     <= S_FETCH;
                    end
                end

                S_WAIT_RAS: begin
                    if (raster_match) state <= S_FETCH;
                end

                S_HALT: begin
                    cop_busy <= 1'b0;
                    state    <= S_IDLE;
                end

                default: state <= S_IDLE;
            endcase
        end
    end

endmodule
