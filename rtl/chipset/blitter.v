// Amiga-blitter-inspired hardware accelerator.
//
// Clean-room design. Programming model and semantics follow the canonical
// Amiga blitter (HRM chapter 6): four channels A/B/C/D over 16-bit words,
// 8-bit minterm Logic Function combiner, barrel shifts on A and B, first-
// and last-word masks on A, per-channel modulo at the end of each row,
// and Bresenham line mode using A as the DDA accumulator.
//
// Differences from the Amiga blitter (intentional, to fit our 32-bit bus
// and our 1-clock-domain simulation model):
//   - Registers are 32-bit (Amiga has 16-bit register pairs PTH/PTL etc.).
//   - Word stride is still 2 bytes (we still operate on 16-bit words).
//   - Pointers are full 32-bit byte addresses (Amiga is 21-bit).
//   - Fill mode (inclusive/exclusive) is implemented.
//   - Descending mode (DESC) is implemented: pointers decrement, modulos
//     subtract, and the A/B barrel shifts read {cur, prev} >> ASH so the
//     bit alignment matches a right-to-left row traversal.
//
// Slave interface: CPU writes/reads of the register region get routed here
// by the bus arbiter (slv_we / slv_addr / slv_wdata / slv_rdata).  See
// m68k_bus.v for the address decode.
//
// Master interface: when running a blit, the blitter issues memory read
// and write transactions on its own bus port (mst_req / mst_we / mst_addr
// / mst_wdata / mst_ack / mst_rdata).
//
// Register map (offsets from BLT_BASE = $00FE_0000):
//   $00  BLTCON   {LF[7:0], ASH[3:0], BSH[3:0], 4'b0, LINE, oct[2:0], chan_en[7:0]}
//                  LF       = 8-bit minterm function
//                  ASH/BSH  = barrel shift for A/B (0..15)
//                  LINE     = 1: line mode, 0: copy mode
//                  oct      = {SUD, SUL, AUL}
//                  chan_en  = {4'b0, USEA, USEB, USEC, USED}
//   $04  BLTAFWM  low 16 bits = first-word mask for A
//   $08  BLTALWM  low 16 bits = last-word mask for A
//   $0C  BLTAPTH  A pointer (byte address; must be word-aligned)
//   $10  BLTBPTH  B pointer
//   $14  BLTCPTH  C pointer
//   $18  BLTDPTH  D pointer
//   $1C  BLTAMOD  modulo A (signed; added to APT at end of each row)
//   $20  BLTBMOD  modulo B
//   $24  BLTCMOD  modulo C
//   $28  BLTDMOD  modulo D
//   $2C  BLTADAT  A data preset (low 16 bits used; ignored in copy mode)
//   $30  BLTBDAT  B data preset (line pattern in line mode)
//   $34  BLTCDAT  C data preset (ignored in copy mode)
//   $38  BLTSIZE  [21:6] = height (number of rows / pixels in line mode)
//                  [5:0]  = width-in-words (max 63; 0 is treated as 64).
//                  *Writing this register starts a blit.*
//   $3C  BLTSTAT  RO: bit 0 = BUSY.  Bit 1 = BZERO (set when the entire
//                  D-channel image written by the most recent blit was
//                  all-zero; cleared as soon as any non-zero word is
//                  written, and reset to 1 at the start of each blit).
//
// Interrupt: int_o pulses high for one cycle the cycle the blit finishes
// (transition out of S_DONE).  Wire it to your interrupt controller of
// choice; it is *not* required for the Copper's WAIT-BLITTER, which uses
// busy_o.



module blitter (
    input  wire        clk,
    input  wire        rst_n,

    // Slave interface (CPU register access).
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [5:0]  slv_addr,    // byte offset 0..0x3F
    input  wire [3:0]  slv_be,      // byte enables
    input  wire        slv_is_long, // CPU did a .L write that spans this reg + next
    input  wire [31:0] slv_wdata,
    output reg  [31:0] slv_rdata,
    output reg         slv_ack,

    // Master interface (bus arbiter port).  mst_req is held in mst_req_r
    // and gated combinationally with !mst_ack so the arbiter sees exactly
    // one request per logical transaction (mirrors the trick used in
    // m68k_cache.v and m68k_passthrough.v).
    output wire        mst_req,
    output reg         mst_we,
    output reg  [31:0] mst_addr,
    output reg  [31:0] mst_wdata,
    output reg  [3:0]  mst_be,
    input  wire        mst_ack,
    input  wire [31:0] mst_rdata,

    // Live busy flag for the Copper (or any other consumer) to sample.
    output wire        busy_o,

    // Live BZERO flag: 1 = no non-zero D-word written by the current /
    // most recent blit.  Cleared as soon as a non-zero word is committed,
    // and reset to 1 at every BLTSIZE write.  Real Amiga reflects this in
    // DMACONR bit 13.
    output wire        bzero_o,

    // One-cycle pulse on blit completion (rising edge of !busy_o).
    output reg         int_o
);
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

`ifdef BLT_MST_TRACE_2D24
    // Catch every blitter master write attempt to $2D24..$2D27 — what
    // actually appears on the bus.  Compares against state-machine
    // d_pipe and S_LWRD probes to find which path is responsible
    // for $0 writes that don't appear in those probes.
    always @(posedge clk) begin
        if (mst_req_r && mst_we &&
            mst_addr >= 32'h0000_2D24 && mst_addr <= 32'h0000_2D27) begin
            $display("[BLT_MST_2D24] addr=%h be=%b wdata=%h ack=%b state=%0d cur_word=%0d cur_row=%0d bltcon=%h bltdpt=%h",
                mst_addr, mst_be, mst_wdata, mst_ack, state,
                cur_word, cur_row, bltcon, bltdpt);
        end
    end
`endif

`ifdef BLT_CLI_TITLE_TRACE
    // Trace every blitter master write whose destination lands in the
    // CLI window's title bar region in BPL1 ($60C8..$63E8 inclusive).
    // Used to find out whether Intuition's title-bar fill blit ever
    // issues against CLI Window @ $C05E90 in the WB1.3 boot.
    always @(posedge clk) begin
        if (mst_req_r && mst_we &&
            mst_addr >= 32'h0000_60C8 && mst_addr <= 32'h0000_63E8) begin
            $display("[BLT_CLI_TITLE] addr=%h be=%b wdata=%h state=%0d cur_word=%0d cur_row=%0d bltcon=%h bltdpt=%h",
                mst_addr, mst_be, mst_wdata, state,
                cur_word, cur_row, bltcon, bltdpt);
        end
    end
`endif

    // ----- Register file -----
    reg [31:0] bltcon;
    reg [31:0] bltafwm;
    reg [31:0] bltalwm;
    reg [31:0] bltapt;
    reg [31:0] bltbpt;
    reg [31:0] bltcpt;
    reg [31:0] bltdpt;
    reg [31:0] bltamod;
    reg [31:0] bltbmod;
    reg [31:0] bltcmod;
    reg [31:0] bltdmod;
    reg [31:0] bltadat_pre;
    reg [31:0] bltbdat_pre;
    reg [31:0] bltcdat_pre;
    reg [15:0] blt_width;       // words per row (after BLTSIZE decode)
    reg [15:0] blt_height;      // rows
    reg        blt_busy;
    reg        blt_bzero;       // tracks "no non-zero D-word written yet"
    assign busy_o = blt_busy;
    assign bzero_o = blt_bzero;

    // D-write pipeline (mirrors FS-UAE/real-Amiga 1-cycle delay).  The
    // blitter's D channel is pipelined one word behind the C read: the
    // FIRST D-iteration of a blit only LATCHES the dst address and the
    // computed D value; subsequent iterations write the previously-latched
    // pair to memory BEFORE latching the new one.  At blit end the last
    // pending pair is flushed in S_DONE.  See blitter_dofast() in FS-UAE's
    // blitter.cpp (lines 546-563 + 574-575).  Without this delay our
    // emulated blitter writes D in the same cycle as it reads C, which
    // diverges from real-Amiga timing.  Documented in
    // project_wb13_fsuae_reference.md.
    reg        d_pipe_valid;
    reg [31:0] d_pipe_addr;
    reg [15:0] d_pipe_data;

    // Decoded BLTCON fields.
    wire [7:0]  lf       = bltcon[31:24];
    wire [3:0]  ash      = bltcon[23:20];
    wire [3:0]  bsh      = bltcon[19:16];
    wire        desc     = bltcon[15];       // descending mode
    wire        ife      = bltcon[14];       // inclusive fill enable
    wire        efe      = bltcon[13];       // exclusive fill enable
    wire        fci      = bltcon[12];       // fill carry-in (per-row reset value)
    wire        line     = bltcon[11];
    wire        sud      = bltcon[10];
    wire        sul      = bltcon[9];
    wire        aul      = bltcon[8];
    wire        use_a    = bltcon[3];
    wire        use_b    = bltcon[2];
    wire        use_c    = bltcon[1];
    wire        use_d    = bltcon[0];

    // ----- Slave (register) port -----
    // Combinational read; writes commit on posedge below.
    always @* begin
        case (slv_addr[5:2])
            4'h0: slv_rdata = bltcon;
            4'h1: slv_rdata = bltafwm;
            4'h2: slv_rdata = bltalwm;
            4'h3: slv_rdata = bltapt;
            4'h4: slv_rdata = bltbpt;
            4'h5: slv_rdata = bltcpt;
            4'h6: slv_rdata = bltdpt;
            4'h7: slv_rdata = bltamod;
            4'h8: slv_rdata = bltbmod;
            4'h9: slv_rdata = bltcmod;
            4'hA: slv_rdata = bltdmod;
            4'hB: slv_rdata = bltadat_pre;
            4'hC: slv_rdata = bltbdat_pre;
            4'hD: slv_rdata = bltcdat_pre;
            4'hE: slv_rdata = {6'd0, blt_height, blt_width[5:0], 4'd0};
            4'hF: slv_rdata = {30'd0, blt_bzero, blt_busy};
            default: slv_rdata = 32'd0;
        endcase
        // Single-cycle ack for register accesses.
        slv_ack = slv_req;
    end

    // ----- State machine -----
    // Copy mode iterates over (height * width) words.
    //   For each word we issue (in order, only if the corresponding channel
    //   is enabled): an A read, a B read, a C read, and a D write.
    //
    // Line mode iterates over `height` pixels (Amiga line mode forces
    //   width = 2; here we just use height as the pixel count).
    //   For each pixel: C read, D write.  In between, we update the
    //   Bresenham accumulator (BLTAPT) and the pointers per octant.
    localparam S_IDLE   = 4'd0;
    localparam S_RDA    = 4'd1;
    localparam S_RDB    = 4'd2;
    localparam S_RDC    = 4'd3;
    localparam S_WRD    = 4'd4;
    localparam S_ROW    = 4'd5;   // end-of-row: apply modulos
    localparam S_DONE   = 4'd6;
    localparam S_LRDC   = 4'd7;   // line mode: read C
    localparam S_LWRD   = 4'd8;   // line mode: write D
    localparam S_LSTEP  = 4'd9;   // line mode: step pointers and accumulator

    reg [3:0]  state;
    reg [15:0] cur_word;          // 0..width-1 within the row
    reg [15:0] cur_row;           // 0..height-1
    reg [15:0] a_prev_word;       // for barrel shifting
    reg [15:0] b_prev_word;
    reg [15:0] a_cur_word_q;      // latched A read result
    reg [15:0] b_cur_word_q;
    reg [15:0] c_cur_word_q;

    // Bus-side: we always emit word reads/writes.  Memory is 32-bit wide,
    // so we select half-word with byte enables based on addr[1].
    // For a read, the response is the full 32-bit word; we pick the half.
    // For a write, we use byte enables to write only 2 bytes.
    function [15:0] pick_half;
        input [31:0] word;
        input        odd;    // 1 = byte address bits 1=1 (low half)
        begin
            pick_half = odd ? word[15:0] : word[31:16];
        end
    endfunction

    function [3:0] half_be;
        input odd;
        begin
            // Big-endian: byte 0 of long is at byte addr +0 (bits 31:24);
            // a 16-bit word at addr+0 occupies bytes 0,1 (BE=4'b1100);
            // a 16-bit word at addr+2 occupies bytes 2,3 (BE=4'b0011).
            half_be = odd ? 4'b0011 : 4'b1100;
        end
    endfunction

    function [31:0] put_half;
        input [15:0] half;
        input        odd;
        begin
            put_half = odd ? {16'd0, half} : {half, 16'd0};
        end
    endfunction

    // Apply ASH-bit barrel shift.
    //   Ascending : joined = {prev, cur}; out = joined[15+sh -: 16]
    //               = (prev << 16 | cur) >> sh, low 16.
    //               -> right shift; vacated MSBs filled from prev's LSBs.
    //   Descending: joined = {cur, prev}; out = joined[31-sh -: 16]
    //               = (cur << sh) | (prev >> (16-sh)).
    //               -> left shift;  vacated LSBs filled from prev's MSBs.
    //   In both cases prev_w is the word read in the PREVIOUS iteration.
    function [15:0] shift_a;
        input [15:0] prev_w;
        input [15:0] cur_w;
        input [3:0]  sh;
        input        do_desc;
        reg   [31:0] joined;
        begin
            if (do_desc) begin
                joined  = {cur_w, prev_w};
                shift_a = joined[31-sh -: 16];
            end else begin
                joined  = {prev_w, cur_w};
                shift_a = joined[15+sh -: 16];
            end
        end
    endfunction

    function [15:0] shift_b;
        input [15:0] prev_w;
        input [15:0] cur_w;
        input [3:0]  sh;
        input        do_desc;
        reg   [31:0] joined;
        begin
            if (do_desc) begin
                joined  = {cur_w, prev_w};
                shift_b = joined[31-sh -: 16];
            end else begin
                joined  = {prev_w, cur_w};
                shift_b = joined[15+sh -: 16];
            end
        end
    endfunction

    // 8-bit minterm Boolean combiner.  For each output bit i:
    //   out[i] = LF[ {A[i], B[i], C[i]} ]
    function [15:0] combine;
        input [7:0]  flf;
        input [15:0] av;
        input [15:0] bv;
        input [15:0] cv;
        integer i;
        begin
            for (i = 0; i < 16; i = i + 1)
                combine[i] = flf[{av[i], bv[i], cv[i]}];
        end
    endfunction

    // Apply Amiga-style fill across a 16-bit word, returning {new_fill_carry,
    // filled_word}.  The carry has to walk consistently with the row scan
    // direction so it propagates correctly across word boundaries:
    //
    //   Ascending  (DESC=0): words processed low-addr -> high-addr.
    //                        Within each word, bit 15 (leftmost pixel)
    //                        first, bit 0 last.  Scan direction is visually
    //                        left-to-right across the row.
    //   Descending (DESC=1): words processed high-addr -> low-addr.
    //                        Within each word, bit 0 (rightmost pixel)
    //                        first, bit 15 last.  Scan direction is
    //                        visually right-to-left.
    //
    //   IFE (inclusive): out = in OR carry; carry ^= in
    //   EFE (exclusive): out = carry;       carry ^= in
    function [16:0] apply_fill;
        input [15:0] din;
        input        fc_in;
        input        do_ife;
        input        do_efe;
        input        do_desc;
        integer i, bi;
        reg [15:0] dout;
        reg        fc;
        reg        bit_in;
        begin
            fc = fc_in;
            dout = 16'd0;
            for (i = 0; i < 16; i = i + 1) begin
                bi = do_desc ? i : (15 - i);
                bit_in = din[bi];
                if (do_efe) begin
                    dout[bi] = fc;
                end else if (do_ife) begin
                    dout[bi] = bit_in | fc;
                end else begin
                    dout[bi] = bit_in;
                end
                fc = fc ^ bit_in;
            end
            apply_fill = {fc, dout};
        end
    endfunction

    // Line mode bit-position tracking.  Initial value = ASH at start of blit;
    // updated each pixel based on octant.
    reg [3:0]  line_pos;          // 0..15, bit index within the current word
    reg [15:0] line_pattern_pos;  // rotating B pattern shift (0..15)

    // Fill mode per-row state.  Initialised to FCI at the start of each row.
    reg        fill_carry;

    // Slave write commits.
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            bltcon       <= 32'd0;
            bltafwm      <= 32'hFFFF;
            bltalwm      <= 32'hFFFF;
            bltapt       <= 32'd0;
            bltbpt       <= 32'd0;
            bltcpt       <= 32'd0;
            bltdpt       <= 32'd0;
            bltamod      <= 32'd0;
            bltbmod      <= 32'd0;
            bltcmod      <= 32'd0;
            bltdmod      <= 32'd0;
            bltadat_pre  <= 32'd0;
            bltbdat_pre  <= 32'hFFFF;     // default: solid line pattern
            bltcdat_pre  <= 32'd0;
            blt_width    <= 16'd0;
            blt_height   <= 16'd0;
            blt_busy     <= 1'b0;
            blt_bzero    <= 1'b1;
            int_o        <= 1'b0;

            state          <= S_IDLE;
            cur_word       <= 16'd0;
            cur_row        <= 16'd0;
            a_prev_word    <= 16'd0;
            b_prev_word    <= 16'd0;
            a_cur_word_q   <= 16'd0;
            b_cur_word_q   <= 16'd0;
            c_cur_word_q   <= 16'd0;
            line_pos       <= 4'd0;
            line_pattern_pos <= 16'd0;
            fill_carry     <= 1'b0;

            mst_req_r <= 1'b0;
            mst_we    <= 1'b0;
            mst_addr  <= 32'd0;
            mst_wdata <= 32'd0;
            mst_be    <= 4'b0000;

            d_pipe_valid <= 1'b0;
            d_pipe_addr  <= 32'd0;
            d_pipe_data  <= 16'd0;
        end else begin
            // Default: int_o is a single-cycle pulse; clear it every cycle
            // unless S_DONE sets it this cycle.
            int_o <= 1'b0;

            // -------- Slave writes (CPU programs registers) --------
            if (slv_req && slv_we) begin
                case (slv_addr[5:2])
                    4'h0: bltcon       <= slv_wdata;
                    4'h1: begin
                        // For longword CPU writes (`slv_is_long`), split
                        // slv_wdata: high half is BLTAFWM ($DFF044), low
                        // half is BLTALWM ($DFF046).  K1.3 trackdisk does
                        // MOVE.L #$FFFFFFFF, $DFF044 as a single instruction
                        // to set both masks; without this split, BLTALWM
                        // retained its stale value and caused single-bit
                        // corruption in MFM decode.  See tests/t152.
                        if (slv_is_long) begin
                            bltafwm <= {16'd0, slv_wdata[31:16]};
                            bltalwm <= {16'd0, slv_wdata[15:0]};
                        end else begin
                            bltafwm <= slv_wdata;
                        end
                    end
                    4'h2: bltalwm      <= slv_wdata;
                    // Real Amiga BLTxPTL is wired with bit 0 = 0 (word-align),
                    // so any CPU write of an odd byte address is silently
                    // masked.  Our blitter must do the same — otherwise an
                    // odd BLT*PT propagates through the iteration loop and
                    // produces writes at odd byte addresses that land in
                    // different mem[] longwords than the CPU expected.
                    // Found via [BUF-2D24-WR]+[BPL1-AREA-WR] trace where
                    // K1.3 set bltdpt=$762D (odd) and the resulting
                    // descending blit corrupted chip $2D24 (WB1.3 cyl-53
                    // sector 3 sync).
                    4'h3: bltapt       <= slv_wdata & 32'hFFFF_FFFE;
                    4'h4: bltbpt       <= slv_wdata & 32'hFFFF_FFFE;
                    4'h5: bltcpt       <= slv_wdata & 32'hFFFF_FFFE;
                    4'h6: begin
`ifdef HDRCHK_WATCH
                        if (blt_busy)
                            $display("[BLT_REG_WR] BLTDPT mid-blit! old=%h new=%h cur_word=%0d cur_row=%0d",
                                bltdpt, slv_wdata, cur_word, cur_row);
`endif
                        bltdpt       <= slv_wdata & 32'hFFFF_FFFE;
                    end
                    // BLTAMOD .L from CPU (MOVE.L to $DFF064) commits both
                    // BLTAMOD (high 16, sign-ext) AND BLTDMOD (low 16,
                    // sign-ext) — same pattern as BLTAFWM/BLTALWM split.
                    // Bus adapter sets slv_is_long when CPU did a 32-bit
                    // write to the canonical BLTAMOD address.
                    4'h7: begin
                        if (slv_is_long) begin
                            bltamod <= {{16{slv_wdata[31]}}, slv_wdata[31:16]};
                            bltdmod <= {{16{slv_wdata[15]}}, slv_wdata[15:0]};
                        end else begin
                            bltamod <= slv_wdata;
                        end
                    end
                    4'h8: bltbmod      <= slv_wdata;
                    // BLTCMOD .L from CPU (MOVE.L to $DFF060) commits both
                    // BLTCMOD (high 16, sign-ext) AND BLTBMOD (low 16,
                    // sign-ext).
                    4'h9: begin
                        if (slv_is_long) begin
                            bltcmod <= {{16{slv_wdata[31]}}, slv_wdata[31:16]};
                            bltbmod <= {{16{slv_wdata[15]}}, slv_wdata[15:0]};
                        end else begin
                            bltcmod <= slv_wdata;
                        end
                    end
                    4'hA: bltdmod      <= slv_wdata;
                    4'hB: bltadat_pre  <= slv_wdata;
                    4'hC: bltbdat_pre  <= slv_wdata;
                    4'hD: bltcdat_pre  <= slv_wdata;
                    4'hE: begin
                        // Start a blit.  BLTSIZE write only triggers if not busy.
                        if (!blt_busy) begin
`ifdef BLT_START_TITLE_TRACE
                            // Filtered version of [BLT_START] that only fires
                            // for blits whose dest pointer lands in the CLI
                            // title bar BPL1 region ($60C8..$63E8).  Surfaces
                            // bltapt / bltbpt / bltcpt — those tell us where
                            // the AreaPtrn stipple pattern lives in memory
                            // (per WB13_DEBUG_JOURNAL §32).
                            if (bltdpt[23:0] >= 24'h00_60C8 &&
                                bltdpt[23:0] <= 24'h00_63E8) begin
                                $display("[BLT_START_TITLE] bltcon=%h bltapt=%h bltbpt=%h bltcpt=%h bltdpt=%h bltsize=%h bltamod=%h bltbmod=%h bltcmod=%h bltdmod=%h adat_pre=%h bdat_pre=%h cdat_pre=%h afwm=%h alwm=%h",
                                    bltcon, bltapt, bltbpt, bltcpt, bltdpt, slv_wdata[15:0],
                                    bltamod, bltbmod, bltcmod, bltdmod,
                                    bltadat_pre[15:0], bltbdat_pre[15:0], bltcdat_pre[15:0],
                                    bltafwm[15:0], bltalwm[15:0]);
                            end
`endif
`ifdef BLT_START_BANNER_TRACE
                            // Same shape but for the CLI banner text area:
                            // BPL1 rows 11..32 = $6168..$7158.  Tells us
                            // whether Intuition's Text() ever issues glyph
                            // blits for the CLI "Release 1.3 ..." banner.
                            if (bltdpt[23:0] >= 24'h00_6168 &&
                                bltdpt[23:0] <= 24'h00_7158) begin
                                $display("[BLT_START_BANNER] bltcon=%h bltapt=%h bltbpt=%h bltcpt=%h bltdpt=%h bltsize=%h adat_pre=%h bdat_pre=%h cdat_pre=%h afwm=%h alwm=%h",
                                    bltcon, bltapt, bltbpt, bltcpt, bltdpt, slv_wdata[15:0],
                                    bltadat_pre[15:0], bltbdat_pre[15:0], bltcdat_pre[15:0],
                                    bltafwm[15:0], bltalwm[15:0]);
                            end
`endif
`ifdef BLT_START_ICON_TRACE
                            // Same shape but for the Workbench disk-icon
                            // area: BPL1 rows 17..30 = $6618..$6A78.
                            // Per §48a: investigate why icons render as
                            // solid white outlines instead of detailed
                            // graphics — find which blits hit this range
                            // and what their LF/USE/source data is.
                            if (bltdpt[23:0] >= 24'h00_6618 &&
                                bltdpt[23:0] <= 24'h00_6A78) begin
                                $display("[BLT_START_ICON] bltcon=%h bltapt=%h bltbpt=%h bltcpt=%h bltdpt=%h bltsize=%h adat_pre=%h bdat_pre=%h cdat_pre=%h afwm=%h alwm=%h",
                                    bltcon, bltapt, bltbpt, bltcpt, bltdpt, slv_wdata[15:0],
                                    bltadat_pre[15:0], bltbdat_pre[15:0], bltcdat_pre[15:0],
                                    bltafwm[15:0], bltalwm[15:0]);
                            end
`endif
`ifdef KICKSTART_BOOT_TRACE
                            $display("[BLT_START] bltcon=%h bltapt=%h bltbpt=%h bltcpt=%h bltdpt=%h bltsize=%h bltamod=%h bltbmod=%h bltcmod=%h bltdmod=%h",
                                bltcon, bltapt, bltbpt, bltcpt, bltdpt, slv_wdata[15:0],
                                bltamod, bltbmod, bltcmod, bltdmod);
                            // [BLT_DAT_PRE]: track *_dat_pre values at every
                            // BLT_START — when USE_X=0 the LF combine uses
                            // *_dat_pre[15:0] as the missing channel.  Stale
                            // values across blits could explain non-det
                            // outputs on sector copies (WB1.3 boot, r=116M).
                            // bltafwm/bltalwm are critical: applied to the
                            // first/last word of A in apply_a_masks; if these
                            // change across blits, the masked output differs.
                            $display("[BLT_DAT_PRE] bltadat=%h bltbdat=%h bltcdat=%h afwm=%h alwm=%h",
                                bltadat_pre[15:0], bltbdat_pre[15:0], bltcdat_pre[15:0],
                                bltafwm[15:0], bltalwm[15:0]);
`endif
                            blt_height <= slv_wdata[21:6];
                            // width 0 (= 64) is a real-Amiga quirk; we treat 0 as 64.
                            blt_width  <= (slv_wdata[5:0] == 6'd0)
                                          ? 16'd64
                                          : {10'd0, slv_wdata[5:0]};
                            blt_busy   <= 1'b1;
                            blt_bzero  <= 1'b1;
                            d_pipe_valid <= 1'b0; // restart D pipeline per blit
                            cur_word   <= 16'd0;
                            cur_row    <= 16'd0;
                            a_prev_word <= 16'd0;
                            b_prev_word <= 16'd0;
                            // Amiga semantics: pixel x of a scanline lives in
                            // bit (15 - (x mod 16)) of word (x/16).  ASH =
                            // (start_x mod 16); initial bit index = 15 - ASH.
                            line_pos    <= 4'd15 - bltcon[23:20];
                            line_pattern_pos <= 16'd0;
                            // Fill mode starts each row with carry = FCI.
                            fill_carry  <= bltcon[12];
                            state <= bltcon[11] ? S_LRDC : (bltcon[3] ? S_RDA :
                                                            bltcon[2] ? S_RDB :
                                                            bltcon[1] ? S_RDC : S_WRD);
                        end
                    end
                    default: ;
                endcase
            end

            // -------- Master-side state machine --------
            case (state)
                S_IDLE: begin
                    mst_req_r <= 1'b0;
                end

                // -------- Copy mode --------
                S_RDA: begin
                    if (use_a) begin
                        mst_req_r <= 1'b1;
                        mst_we    <= 1'b0;
                        mst_addr  <= bltapt;
                        mst_be    <= 4'b1111;
                        if (mst_ack) begin
                            mst_req_r <= 1'b0;
`ifdef BLT_A_TRACE_2AC
                            // Log every A read during the corrupting blit
                            // shape (bltcon=$2AC0000B).  Compares A source
                            // address + returned data — finds why output
                            // forces $0 at the iteration that hits $2D24.
                            if (bltcon[31:0] == 32'h2AC0_000B)
                                $display("[BLT_A_RD_2AC] bltapt=%h mst_rdata=%h pick=%h cur_word=%0d cur_row=%0d bltdpt=%h",
                                    bltapt, mst_rdata,
                                    pick_half(mst_rdata, bltapt[1]),
                                    cur_word, cur_row, bltdpt);
`endif
                            // Pick the 16-bit half and apply masks.
                            a_cur_word_q <= apply_a_masks(
                                pick_half(mst_rdata, bltapt[1]),
                                cur_word == 16'd0,
                                cur_word == (blt_width - 16'd1));
                            bltapt <= bltapt + (desc ? -32'sd2 : 32'sd2);
                            state  <= use_b ? S_RDB : (use_c ? S_RDC : S_WRD);
                        end
                    end else begin
                        // USE_A=0: per real-Amiga / minimig semantics, the
                        // A-channel feeds BLTADAT (its CPU-pre-loaded value)
                        // into the LF minterm generator as a constant.  Our
                        // earlier impl set a_cur_word_q <= 0, which broke
                        // any LF that consults A (e.g. LF=\$CA in Intuition's
                        // title-bar fill, which expects A=\$FFFF for solid
                        // output).  Mirror of the t155 USE_B=0 fix in S_RDB.
                        // Per WB13_DEBUG_JOURNAL §38.
                        a_cur_word_q <= bltadat_pre[15:0];
                        state <= use_b ? S_RDB : (use_c ? S_RDC : S_WRD);
                    end
                end
                S_RDB: begin
                    if (use_b) begin
                        mst_req_r <= 1'b1;
                        mst_we    <= 1'b0;
                        mst_addr  <= bltbpt;
                        mst_be    <= 4'b1111;
                        if (mst_ack) begin
                            mst_req_r <= 1'b0;
                            b_cur_word_q <= pick_half(mst_rdata, bltbpt[1]);
                            bltbpt <= bltbpt + (desc ? -32'sd2 : 32'sd2);
                            state  <= use_c ? S_RDC : S_WRD;
`ifdef BLT_RD_TRACE_2D24
                            // Log B-reads from the cyl-53 sector-3 source
                            // region.  Compares blitter's received data
                            // against bus-side memory state.
                            if (bltbpt >= 32'h0000_2D20 && bltbpt <= 32'h0000_2D34)
                                $display("[BLT_B_RD_2D24] bltbpt=%h mst_rdata=%h pick=%h cur_word=%0d cur_row=%0d bltcon=%h",
                                    bltbpt, mst_rdata, pick_half(mst_rdata, bltbpt[1]), cur_word, cur_row, bltcon);
`endif
`ifdef HDRCHK_WATCH
                            if (bltbpt >= 32'h0000_64B0 && bltbpt <= 32'h0000_64BB)
                                $display("[BLT_RDB] bltbpt=%h mst_rdata=%h cur_word=%0d cur_row=%0d", bltbpt, mst_rdata, cur_word, cur_row);
`endif
                        end
                    end else begin
                        // USE-B=0: when entered from S_IDLE, treat B as
                        // BLTBDAT preset (real Amiga semantics — see the
                        // `use_b ? b_cur_word_q : bltbdat_pre[15:0]` arm
                        // in the combine() call below).  Keeping
                        // b_cur_word_q at 0 here is harmless since the
                        // combine arm overrides it; we still need the
                        // state transition.
                        state <= use_c ? S_RDC : S_WRD;
                    end
                end
                S_RDC: begin
                    if (use_c) begin
                        mst_req_r <= 1'b1;
                        mst_we    <= 1'b0;
                        mst_addr  <= bltcpt;
                        mst_be    <= 4'b1111;
                        if (mst_ack) begin
                            mst_req_r <= 1'b0;
                            c_cur_word_q <= pick_half(mst_rdata, bltcpt[1]);
                            bltcpt <= bltcpt + (desc ? -32'sd2 : 32'sd2);
                            state  <= S_WRD;
                        end
                    end else begin
                        c_cur_word_q <= 16'd0;
                        state <= S_WRD;
                    end
                end
                S_WRD: begin : wrd_block
                    reg [15:0] combined_w;
                    reg [16:0] filled;          // {new_fill_carry, filled_word}
                    reg [15:0] final_w;

                    // When USEC=0, the C-channel DMA fetch is skipped and
                    // S_RDC is never entered, so c_cur_word_q would carry a
                    // stale value (or 0).  Real Amiga / minimig feed BLTCDAT
                    // into the minterm generator as a constant in that case —
                    // critical for LFs that consult C (e.g. LF=$D8 with
                    // BLTCDAT bit-mask in trackdisk's MFM data decoder).
                    // See tests/t139_blt_usec0_bltcdat.s.
                    // When USE_A=0, the A-channel feeds BLTADAT preset into
                    // the LF minterm generator as a constant (real-Amiga /
                    // minimig semantics, mirror of the USE_B=0 arm just
                    // below).  Without this, a_cur_word_q and a_prev_word
                    // carry stale values from a prior blit and Intuition's
                    // title-bar fill (LF=$CA USE_A=0 USE_C=1 USE_D=1, which
                    // wants A=$FFFF for solid output) writes whatever C
                    // had instead — producing the $2AAA inactive stipple
                    // we observed in WB1.3 boots.  See WB13_DEBUG_JOURNAL
                    // §38 + tests/t157_use_a_zero_preset.s.
                    // For both A and B: when USE_X=0 the channel's prev and
                    // cur words should both default to BLTxDAT_pre.  Real
                    // Amiga / minimig semantics — the prev-word fallback
                    // matters only when ASH/BSH > 0 (the barrel shifter
                    // combines prev|cur into a 32-bit window before
                    // extracting the shifted 16-bit result).  For ASH/BSH=0,
                    // shift_x returns just cur_w, so the prev fallback is
                    // harmless.  See WB13_DEBUG_JOURNAL §38 + t157 (A) and
                    // t155 (B).
                    combined_w = combine(lf,
                                         shift_a(
                                            use_a ? a_prev_word   : bltadat_pre[15:0],
                                            use_a ? a_cur_word_q  : bltadat_pre[15:0],
                                            ash, desc),
                                         shift_b(
                                            use_b ? b_prev_word   : bltbdat_pre[15:0],
                                            use_b ? b_cur_word_q  : bltbdat_pre[15:0],
                                            bsh, desc),
                                         use_c ? c_cur_word_q : bltcdat_pre[15:0]);
                    filled  = apply_fill(combined_w, fill_carry, ife, efe, desc);
                    final_w = filled[15:0];

                    if (use_d) begin : use_d_block
                        // D-write pipeline: write the PREVIOUSLY-latched
                        // pair, then latch the current pair.  Matches
                        // FS-UAE/real-Amiga semantics where D is delayed
                        // one word behind the A/B/C reads.
                        reg should_advance;
                        should_advance = 1'b0;
`ifdef BLT_VECTABLE_GUARD
                        if (d_pipe_valid && d_pipe_addr < 32'h0000_0400) begin
                            // Pending pair targets the vector table —
                            // refuse the bus write but still rotate the
                            // pipeline so the blit completes normally.
                            if (cur_word == 16'd1 && cur_row == 16'd0)
                                $display("[BLT_GUARD] refusing blit dst=%h bltcon=%h size=%0dx%0d",
                                    d_pipe_addr, bltcon, blt_width, blt_height);
                            d_pipe_addr <= bltdpt;
                            d_pipe_data <= final_w;
                            d_pipe_valid <= 1'b1;
                            should_advance = 1'b1;
                        end else
`endif
`ifdef BLT_WR_TRACE_2D24
                        // Catch any blitter copy-mode write to $2D24..$2D27.
                        if (d_pipe_valid &&
                            d_pipe_addr >= 32'h0000_2D24 &&
                            d_pipe_addr <= 32'h0000_2D27)
                            $display("[BLT_WR_2D24_COPY] addr=%h data=%h state=%0d cur_word=%0d cur_row=%0d bltcon=%h bltdpt=%h bltbpt=%h",
                                d_pipe_addr, d_pipe_data, state, cur_word, cur_row, bltcon, bltdpt, bltbpt);
`endif
// BLT_MFM_BUFFER_GUARD copy-mode arm REMOVED: the MFM-decode blit
// (BLTCON0=$CC USE=B+D, dst=$2060) legitimately writes into chip
// $2000-$5FFF as part of K1.3 trackdisk's sector-decode pipeline.
// Blocking those writes broke disk reads (chip $2D24 stayed at gap
// pattern $AAAAAAAA, boot reached r=231M in a retry loop).  See
// docs/WB13_DEBUG_JOURNAL.md.  Keep the line-mode guard in S_LWRD
// because the corrupting blit (PC=$FC5772) IS line mode and walks
// off-bitmap deliberately.
                        if (d_pipe_valid) begin
                            mst_req_r <= 1'b1;
                            mst_we    <= 1'b1;
                            mst_addr  <= d_pipe_addr;
                            mst_be    <= half_be(d_pipe_addr[1]);
                            mst_wdata <= put_half(d_pipe_data, d_pipe_addr[1]);
                            if (mst_ack) begin
`ifdef BLT_DEBUG_FIRST_WR
                                if (cur_row == 16'd0 && cur_word < 16'd4)
                                    $display("[BLT_WRD] bltdpt=%h bltbpt=%h final_w=%h b_cur=%h b_prev=%h cur_word=%0d cur_row=%0d",
                                        d_pipe_addr, bltbpt, d_pipe_data, b_cur_word_q, b_prev_word, cur_word, cur_row);
`endif
`ifdef HDRCHK_WATCH
                                if (d_pipe_addr >= 32'h0000_64B0 && d_pipe_addr <= 32'h0000_64BB)
                                    $display("[BLT_WR] bltdpt=%h bltbpt=%h bltcon=%h final_w=%h b_cur=%h cur_word=%0d cur_row=%0d width=%0d",
                                        d_pipe_addr, bltbpt, bltcon, d_pipe_data, b_cur_word_q, cur_word, cur_row, blt_width);
                                if (d_pipe_addr >= 32'h0000_68E0 && d_pipe_addr <= 32'h0000_68FB)
                                    $display("[BLT_SEC1_WR] bltdpt=%h bltbpt=%h final_w=%h b_cur=%h cur_word=%0d cur_row=%0d",
                                        d_pipe_addr, bltbpt, d_pipe_data, b_cur_word_q, cur_word, cur_row);
`endif
                                mst_req_r <= 1'b0;
                                if (d_pipe_data != 16'd0) blt_bzero <= 1'b0;
                                d_pipe_addr <= bltdpt;
                                d_pipe_data <= final_w;
                                // d_pipe_valid stays 1
                                should_advance = 1'b1;
                            end
                        end else begin
                            // First word of blit — just latch, no bus.
                            d_pipe_valid <= 1'b1;
                            d_pipe_addr  <= bltdpt;
                            d_pipe_data  <= final_w;
                            should_advance = 1'b1;
                        end

                        if (should_advance) begin
                            a_prev_word <= a_cur_word_q;
                            b_prev_word <= b_cur_word_q;
                            fill_carry  <= filled[16];
                            bltdpt <= bltdpt + (desc ? -32'sd2 : 32'sd2);
                            if (cur_word == (blt_width - 16'd1)) begin
                                cur_word <= 16'd0;
                                a_prev_word <= 16'd0;
                                b_prev_word <= 16'd0;
                                state <= S_ROW;
                            end else begin
                                cur_word <= cur_word + 16'd1;
                                state <= use_a ? S_RDA :
                                          use_b ? S_RDB :
                                          use_c ? S_RDC : S_WRD;
                            end
                        end
                    end else begin
                        a_prev_word <= a_cur_word_q;
                        b_prev_word <= b_cur_word_q;
                        fill_carry  <= filled[16];
                        if (cur_word == (blt_width - 16'd1)) begin
                            cur_word <= 16'd0;
                            a_prev_word <= 16'd0;
                            b_prev_word <= 16'd0;
                            state <= S_ROW;
                        end else begin
                            cur_word <= cur_word + 16'd1;
                            state <= use_a ? S_RDA :
                                      use_b ? S_RDB :
                                      use_c ? S_RDC : S_WRD;
                        end
                    end
                end
                S_ROW: begin
                    // Apply per-channel modulo at end of row.  Descending
                    // mode subtracts the modulo (so the pointer continues
                    // walking down-and-left through memory).
                    if (use_a) bltapt <= bltapt + (desc ? -bltamod : bltamod);
                    if (use_b) bltbpt <= bltbpt + (desc ? -bltbmod : bltbmod);
                    if (use_c) bltcpt <= bltcpt + (desc ? -bltcmod : bltcmod);
                    if (use_d) bltdpt <= bltdpt + (desc ? -bltdmod : bltdmod);
                    // [BLT-ROW] log all four pointers at row-end if any
                    // of them is in the C2CC corruption zone.  Helps
                    // explain the row-to-row drift in WB1.3 boot.
                    if (bltdpt >= 32'h0000_C200 && bltdpt <= 32'h0000_C400)
                        $display("[BLT-ROW] row=%0d bltapt=%h bltbpt=%h bltcpt=%h bltdpt=%h desc=%b amod=%h bmod=%h cmod=%h dmod=%h",
                            cur_row, bltapt, bltbpt, bltcpt, bltdpt, desc,
                            bltamod, bltbmod, bltcmod, bltdmod);
                    // Reset the fill carry for the next row.
                    fill_carry <= fci;
                    if (cur_row == (blt_height - 16'd1)) begin
                        state    <= S_DONE;
                    end else begin
                        cur_row <= cur_row + 16'd1;
                        state   <= use_a ? S_RDA :
                                    use_b ? S_RDB :
                                    use_c ? S_RDC : S_WRD;
                    end
                end
                S_DONE: begin
                    // Flush the last pending D-pipeline pair before
                    // declaring the blit done.  Mirrors FS-UAE's final
                    // post-loop blit_chipmem_agnus_wput() at line 574.
                    if (use_d && d_pipe_valid) begin
`ifdef BLT_VECTABLE_GUARD
                        if (d_pipe_addr < 32'h0000_0400) begin
                            // Last pending word lands in the vector table —
                            // drop the write but complete the blit.
                            d_pipe_valid <= 1'b0;
                            blt_busy <= 1'b0;
                            int_o    <= 1'b1;
`ifdef HDRCHK_WATCH
                            $display("[BLT_DONE] bltcon=%h", bltcon);
`endif
                            state <= S_IDLE;
                        end else
`endif
                        begin
                            mst_req_r <= 1'b1;
                            mst_we    <= 1'b1;
                            mst_addr  <= d_pipe_addr;
                            mst_be    <= half_be(d_pipe_addr[1]);
                            mst_wdata <= put_half(d_pipe_data, d_pipe_addr[1]);
                            if (mst_ack) begin
                                mst_req_r <= 1'b0;
                                if (d_pipe_data != 16'd0) blt_bzero <= 1'b0;
                                d_pipe_valid <= 1'b0;
                                blt_busy <= 1'b0;
                                int_o    <= 1'b1;
`ifdef HDRCHK_WATCH
                                $display("[BLT_DONE] bltcon=%h", bltcon);
`endif
                                state <= S_IDLE;
                            end
                        end
                    end else begin
                        blt_busy <= 1'b0;
                        int_o    <= 1'b1;        // one-cycle pulse
`ifdef HDRCHK_WATCH
                        $display("[BLT_DONE] bltcon=%h", bltcon);
`endif
                        state    <= S_IDLE;
                    end
                end

                // -------- Line mode --------
                S_LRDC: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b0;
                    mst_addr  <= bltcpt;
                    mst_be    <= 4'b1111;
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        c_cur_word_q <= pick_half(mst_rdata, bltcpt[1]);
                        state <= S_LWRD;
                    end
                end
                S_LWRD: begin : lwrd_block
                    reg [15:0] lw;
                    lw = line_combine();
`ifdef BLT_WR_TRACE_2D24
                    if (bltdpt >= 32'h0000_2D24 && bltdpt <= 32'h0000_2D27)
                        $display("[BLT_WR_2D24_LINE] addr=%h lw=%h cur_row=%0d bltcon=%h bltapt=%h bltcpt=%h",
                            bltdpt, lw, cur_row, bltcon, bltapt, bltcpt);
`endif
`ifdef BLT_MFM_BUFFER_GUARD
                    if (bltdpt >= 32'h0000_2000 && bltdpt < 32'h0000_6000) begin
                        // LINE-draw descended into trackdisk MFM buffer
                        // range (chip $2000-$5FFF).  Skip the bus write
                        // but advance Bresenham so the loop completes.
                        // See docs/WB13_DEBUG_JOURNAL.md.
                        $display("[BLT_MFM_GUARD_LINE] dst=%h bltcon=%h row=%0d",
                            bltdpt, bltcon, cur_row);
                        mst_req_r <= 1'b0;
                        state <= S_LSTEP;
                    end else
`endif
                    begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b1;
                    mst_addr  <= bltdpt;
                    mst_be    <= half_be(bltdpt[1]);
                    mst_wdata <= put_half(lw, bltdpt[1]);
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        if (lw != 16'd0) blt_bzero <= 1'b0;
                        state <= S_LSTEP;
                    end
                    end
                end
                S_LSTEP: begin
                    // Bresenham step.  Compute the combined byte-pointer and
                    // bit-position deltas in temp regs, then commit once.
                    //
                    //   APT < 0  : step dominant axis only; APT += BMOD
                    //   APT >= 0 : step both axes;          APT += AMOD
                    //
                    //   AUL = 0  : X is dominant, Y is subordinate
                    //   AUL = 1  : Y is dominant, X is subordinate
                    //   SUD      : sign of dominant step (0 = +, 1 = -)
                    //   SUL      : sign of subordinate step
                    //
                    //   X+  : line_pos -= 1; if wrapped (was 0), bptr += 2
                    //   X-  : line_pos += 1; if wrapped (was 15), bptr -= 2
                    //   Y+  : bptr += BLT?MOD
                    //   Y-  : bptr -= BLT?MOD
                    begin : line_step_blk
                        reg        step_sub;
                        reg        do_x, do_y;
                        reg        x_sign, y_sign;       // 0 = +, 1 = -
                        reg [4:0]  new_pos_ext;          // 5 bits to detect wrap
                        reg signed [31:0] cptr_d, dptr_d;
                        reg [3:0]  next_pos;

                        step_sub = ~bltapt[31];

                        // Decide axis activation per octant.  Internal
                        // convention: aul=0 → X dominant, aul=1 → Y dominant.
                        // The m68k_bus.v translation INVERTS the real-Amiga
                        // BLTCON1[4] = AUL bit (which has the opposite sense:
                        // 1 = X dominant per FS-UAE blitter.cpp).  Tests that
                        // write the blitter's internal format directly
                        // (t22_blt_line) use this convention as-is.
                        if (!aul) begin
                            do_x   = 1'b1;
                            do_y   = step_sub;
                            x_sign = sud;
                            y_sign = sul;
                        end else begin
                            do_y   = 1'b1;
                            do_x   = step_sub;
                            y_sign = sud;
                            x_sign = sul;
                        end

                        // Pointer delta from Y step.
                        cptr_d = 32'd0;
                        dptr_d = 32'd0;
                        if (do_y) begin
                            cptr_d = y_sign ? -bltcmod : bltcmod;
                            dptr_d = y_sign ? -bltdmod : bltdmod;
                        end

                        // X step: rotate line_pos; on wrap, also adjust ptr.
                        next_pos = line_pos;
                        if (do_x) begin
                            if (!x_sign) begin
                                // +X: bit position moves toward LSB
                                if (line_pos == 4'd0) begin
                                    next_pos = 4'd15;
                                    cptr_d   = cptr_d + 32'd2;
                                    dptr_d   = dptr_d + 32'd2;
                                end else begin
                                    next_pos = line_pos - 4'd1;
                                end
                            end else begin
                                // -X: toward MSB
                                if (line_pos == 4'd15) begin
                                    next_pos = 4'd0;
                                    cptr_d   = cptr_d - 32'd2;
                                    dptr_d   = dptr_d - 32'd2;
                                end else begin
                                    next_pos = line_pos + 4'd1;
                                end
                            end
                        end

                        // Commit deltas.
                        line_pos <= next_pos;
                        bltcpt   <= bltcpt + cptr_d;
                        bltdpt   <= bltdpt + dptr_d;
                        bltapt   <= bltapt + (bltapt[31] ? bltbmod : bltamod);
                    end

                    // Rotate pattern position (line texture).
                    line_pattern_pos <= line_pattern_pos + 16'd1;
                    if (cur_row == (blt_height - 16'd1)) begin
                        state <= S_DONE;
                    end else begin
                        cur_row <= cur_row + 16'd1;
                        state   <= S_LRDC;
                    end
                end

                default: state <= S_IDLE;
            endcase
        end
    end

    // ----- Pure-function helpers (declared via tasks to fit synthesizable
    // Verilog and keep the state machine readable) -----

    function [15:0] apply_a_masks;
        input [15:0] raw;
        input        is_first;
        input        is_last;
        reg   [15:0] masked;
        begin
            masked = raw;
            if (is_first) masked = masked & bltafwm[15:0];
            if (is_last)  masked = masked & bltalwm[15:0];
            apply_a_masks = masked;
        end
    endfunction

    // Line-mode combine: form a single-bit pixel mask at line_pos, AND with
    // the rotated line pattern bit from BLTBDAT, then run the minterm.
    function [15:0] line_combine;
        reg [15:0] pixel_mask;
        reg        pattern_bit;
        reg [15:0] a_word;
        reg [15:0] b_word;
        reg        bit_idx;
        begin
            pixel_mask = 16'd1 << line_pos;
            // BLTBDAT serves as a 16-bit repeating pattern, rotated by
            // line_pattern_pos.  Pick bit (15 - (line_pattern_pos & 15)).
            pattern_bit = bltbdat_pre[15 - (line_pattern_pos[3:0])];
            a_word = pixel_mask;
            b_word = pattern_bit ? 16'hFFFF : 16'h0000;
            line_combine = combine(lf, a_word, b_word, c_cur_word_q);
        end
    endfunction

endmodule
