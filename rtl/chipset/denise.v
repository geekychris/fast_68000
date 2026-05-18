// Amiga-Denise-inspired bitplane rasterizer (Phase 3).
//
// Reads 1-6 planar bitplanes from memory, combines them per pixel via the
// configured display mode, looks up a 12-bit RGB color from a 32-entry
// palette, and writes the result as 8 bpp RGB332 bytes into the chunky
// framebuffer at $00010000.
//
// Supported display modes:
//   * INDEXED   - 1 to 6 planes, palette lookup.
//   * HAM6      - 6 planes, hold-and-modify (4 control modes per pixel).
//                 prev_color resets to COLOR00 at the start of each row.
//   * EHB       - 6 planes, !HAM, !DPF: bit 5 of pixel = half-brite.
//   * DPF       - dual playfield: odd planes form PF1, even form PF2.
//                 Priority controlled by BPLCON2[6] (PF2 in front when set).
//
// Hardware scroll (BPLCON1) and 1bpp sprites are implemented.  Hires,
// HAM8, and genlock are not implemented (see DENISE.md).
//
// Sprites: 8 hardware sprites, 16 pixels wide.  Each can be 1bpp (one
// monochrome plane) or 2bpp (two planes -> 4 colours, value 0 = transparent).
// Pairs of sprites (0+1, 2+3, 4+5, 6+7) can be ATTACHED to form one
// 4bpp sprite (16 colours, value 0 = transparent).  Per sprite:
//   SPRxPT   ($30+8i)  32-bit byte pointer to sprite data.
//                      1bpp layout: one 16-bit data_a word per row,
//                                   packed two-per-long.
//                      2bpp layout: one 32-bit row containing
//                                   {data_a[15:0], data_b[15:0]}
//                                   (high half = A, low half = B).
//                      ATTACHED layout: each member sprite is 2bpp.
//                                   Pixel = {oddA[i], oddB[i], evenA[i], evenB[i]}.
//   SPRxCTL  ($34+8i)  {YPOS[31:24], XPOS[23:16], HEIGHT[15:8],
//                       ATT[7], BPP2[6], reserved[5], COLOR_IDX[4:0]}
//                       YPOS/XPOS: 8-bit screen coordinates (0..255)
//                       HEIGHT:    8-bit row count
//                       ATT:       1 = attach to even-numbered partner
//                                  (only meaningful on odd sprites 1/3/5/7)
//                       BPP2:      1 = 2-bit-per-pixel layout
//                       COLOR_IDX: base palette index.  Sprite pixel value V
//                                  (1..3 in 2bpp; 1..15 attached) selects
//                                  colors[COLOR_IDX + V]; V = 0 is transparent.
//                                  In 1bpp mode, only V = 1 is meaningful and
//                                  it selects colors[COLOR_IDX].
// Global enable: SPRENA ($70), bit i = enable for sprite i.
// Priority: lower sprite number wins.  Sprites composite OVER the
// bitplane image (sprite bit=1 means opaque, bit=0 means transparent).
//
// Pixels: leftmost pixel of a word = bit 15 (Amiga convention). Plane i's
// bit contributes the i-th bit of the pixel value (plane 0 = LSB).
//
// Triggering: write any value to DENRUN. Denise rasterizes the full
// 256x192 framebuffer and sets DENSTAT.BUSY low when done.



module denise #(
    parameter FB_BASE = 32'h0001_0000,
    parameter FB_W    = 256,
    parameter FB_H    = 192
)(
    input  wire        clk,
    input  wire        rst_n,

    // Slave (CPU/Copper register port).  8-bit slave address (256 bytes
    // of register space).
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [8:0]  slv_addr,        // bank bit [8] = 1 for $300..$3FF window
    input  wire [3:0]  slv_be,
    input  wire [31:0] slv_wdata,
    output reg  [31:0] slv_rdata,

    // Master (bitplane reads + chunky FB writes).
    output wire        mst_req,
    output reg         mst_we,
    output reg  [31:0] mst_addr,
    output reg  [31:0] mst_wdata,
    output reg  [3:0]  mst_be,
    input  wire        mst_ack,
    input  wire [31:0] mst_rdata,

    // Live raster position (the row currently being rasterised).  Held
    // at its last value when idle.  Exported so the Copper can wait on
    // a specific scanline via WAIT-RASTER.
    output wire [15:0] vbeam_o
);
    // ---------------- Registers programmed by the CPU ----------------
    reg [31:0] bplcon0;
    reg [31:0] bplcon1;
    reg [31:0] bplcon2;
    reg [31:0] diwstrt;          // {Y_start[15:0], X_start[15:0]}, inclusive
    reg [31:0] diwstop;          // {Y_stop[15:0],  X_stop[15:0]},  exclusive
    wire [15:0] diw_x0 = diwstrt[15:0];
    wire [15:0] diw_y0 = diwstrt[31:16];
    wire [15:0] diw_x1 = diwstop[15:0];
    wire [15:0] diw_y1 = diwstop[31:16];
    reg [31:0] bpl_pt   [0:7];   // BPL1PT..BPL8PT (planes 6,7 used only in HAM8)
    reg [31:0] bpl_mod  [0:1];   // BPL1MOD (odd-plane stride bonus), BPL2MOD (even)
    reg [31:0] colors   [0:63];  // colors[0..31] standard; [32..63] HAM8 base only
    reg [31:0] spr_pt   [0:7];
    reg [31:0] spr_ctl  [0:7];
    reg [7:0]  sprena;
    reg        den_busy;

    // BPLCON0 decode.
    //   [14:12]   = BPU[2:0] (planes 0..7)
    //   [11]      = HAM enable
    //   [10]      = DPF enable
    //   [4]       = BPU[3] — when set with HAM, selects HAM8 (8 planes, 6
    //               bits of data + 2 control bits, 64-entry palette base).
    wire [2:0] nplanes_lo = bplcon0[14:12];
    wire       bpu_hi     = bplcon0[4];
    wire [3:0] nplanes    = bpu_hi ? 4'd8 : {1'b0, nplanes_lo};
    wire       ham_en     = bplcon0[11];
    wire       dpf_en     = bplcon0[10];
    wire       ham8_en    = ham_en && bpu_hi;          // HAM8: 8-plane HAM
    wire       ham6_en    = ham_en && !bpu_hi && (nplanes_lo == 3'd6);
    // EHB is the implicit fall-through when nplanes=6 and neither HAM nor DPF.
    wire       ehb_en     = (nplanes == 4'd6) && !ham_en && !dpf_en;
    // BPLCON0[15] = HIRES: 2x word-groups per row (32 instead of 16),
    // producing FB_W = 512 chunky pixels per row.  Demo / test code must
    // build with FB_W >= 512 for hires output to be visible end-to-end.
    wire       hires_en   = bplcon0[15];
    wire [4:0] wg_last    = hires_en ? 5'd31 : 5'd15;

    // BPLCON1 decode: horizontal scroll per playfield (4 bits each).
    //   PF1H = BPLCON1[3:0] (applied to planes 0, 2, 4 - "odd" 1-indexed)
    //   PF2H = BPLCON1[7:4] (applied to planes 1, 3, 5 - "even" 1-indexed)
    wire [3:0] pf1h = bplcon1[3:0];
    wire [3:0] pf2h = bplcon1[7:4];

    // BPLCON2: bit 6 = PF2-in-front-of-PF1 (DPF mode);
    //          [5:3] = PF1P (playfield-1 vs. sprite-pair priority threshold);
    //          [2:0] = PF2P (playfield-2 vs. sprite-pair priority threshold).
    // A sprite-pair with level L is in front of playfield X iff L < PFxP.
    // PFxP == 0 puts the playfield in front of every sprite; PFxP == 4+
    // (the reset value of zero-extended bits) puts it behind every sprite.
    wire       pf2_pri = bplcon2[6];
    wire [2:0] pf1p    = bplcon2[5:3];
    wire [2:0] pf2p    = bplcon2[2:0];

    // ---------------- mst_req gating --------------------------------
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

    // ---------------- Slave reads -----------------------------------
    integer rk;
    always @* begin
        slv_rdata = 32'd0;
        if (slv_addr[8]) begin
            // ---- Bank 1: $300..$3FF (HAM8 extension) ----
            case (slv_addr[7:0])
                8'h00: slv_rdata = bpl_pt[6];          // BPL7PT
                8'h04: slv_rdata = bpl_pt[7];          // BPL8PT
                default: begin
                    if (slv_addr[7] == 1'b1) begin
                        // $380..$3FF maps colors[32..63].
                        slv_rdata = colors[{1'b1, slv_addr[6:2]}];
                    end
                end
            endcase
        end else
        case (slv_addr[7:0])
            8'h00: slv_rdata = bplcon0;
            8'h04: slv_rdata = bplcon1;
            8'h08: slv_rdata = bplcon2;
            8'h0C: slv_rdata = diwstrt;
            8'h74: slv_rdata = diwstop;
            8'h10: slv_rdata = bpl_pt[0];
            8'h14: slv_rdata = bpl_pt[1];
            8'h18: slv_rdata = bpl_pt[2];
            8'h1C: slv_rdata = bpl_pt[3];
            8'h20: slv_rdata = bpl_pt[4];
            8'h24: slv_rdata = bpl_pt[5];
            8'h28: slv_rdata = bpl_mod[0];
            8'h2C: slv_rdata = bpl_mod[1];
            // Sprite register file at $30..$6F.
            8'h30: slv_rdata = spr_pt[0];
            8'h34: slv_rdata = spr_ctl[0];
            8'h38: slv_rdata = spr_pt[1];
            8'h3C: slv_rdata = spr_ctl[1];
            8'h40: slv_rdata = 32'd0;                 // DENRUN write-only
            8'h44: slv_rdata = {31'd0, den_busy};     // DENSTAT
            8'h48: slv_rdata = spr_pt[2];
            8'h4C: slv_rdata = spr_ctl[2];
            8'h50: slv_rdata = spr_pt[3];
            8'h54: slv_rdata = spr_ctl[3];
            8'h58: slv_rdata = spr_pt[4];
            8'h5C: slv_rdata = spr_ctl[4];
            8'h60: slv_rdata = spr_pt[5];
            8'h64: slv_rdata = spr_ctl[5];
            8'h68: slv_rdata = spr_pt[6];
            8'h6C: slv_rdata = spr_ctl[6];
            8'h70: slv_rdata = {24'd0, sprena};
            8'h78: slv_rdata = spr_pt[7];
            8'h7C: slv_rdata = spr_ctl[7];
            default: begin
                if (slv_addr[7] == 1'b1) begin
                    // $80..$FF = COLOR00..COLOR1F (32 entries x 4 bytes).
                    slv_rdata = colors[{1'b0, slv_addr[6:2]}];
                end
            end
        endcase
    end

    // ---------------- Bitplane fetch / pixel proc state machine ----
    // Per row we have FB_W/16 = 16 word-groups.  For each word-group we
    // read up to 6 bitplane words from memory, then sequentially process
    // 16 pixels, then write 4 longs (16 chunky bytes) to the framebuffer.
    localparam S_IDLE      = 5'd0;
    localparam S_ROW_INIT  = 5'd1;
    localparam S_WG_INIT   = 5'd2;
    localparam S_FETCH_P   = 5'd3;
    localparam S_PROC      = 5'd4;
    localparam S_WRITE     = 5'd5;
    localparam S_NEXT_WG   = 5'd6;
    localparam S_ROW_END   = 5'd7;
    localparam S_DONE      = 5'd8;
    localparam S_SPR_PREP  = 5'd9;
    localparam S_SPR_FETCH = 5'd10;

    reg [4:0]  state;

    // Per-row state.
    reg [15:0] row_idx;             // 0..FB_H-1
    reg [31:0] row_bpl_pt [0:7];    // current row pointers (modified by reads)
    reg [11:0] row_prev_color;      // HAM prev-color, init to COLOR00 each row

    // Expose the current raster row to whoever consumes the chipset
    // (the Copper wires this to its WAIT-RASTER comparator).
    assign vbeam_o = row_idx;

    // Per-word-group state.
    reg [4:0]  wg_idx;              // 0..(FB_W/16 - 1)
    reg [3:0]  pi_idx;              // current plane being fetched (0..7)
    reg [15:0] plane_word [0:7];    // latched 16-bit word per plane
    reg [15:0] plane_prev [0:7];    // previous word per plane (for scroll)

    // Per-sprite running state.
    reg [31:0] spr_row_pt [0:7];    // running fetch pointer per sprite
    reg [15:0] spr_word_a [0:7];    // current row's data_a (low plane)
    reg [15:0] spr_word_b [0:7];    // current row's data_b (high plane); zero in 1bpp
    reg [7:0]  spr_active;          // bitmask: sprite i visible this row
    reg [3:0]  spr_idx;             // sprite walker during S_SPR_FETCH (0..8)

    // Per-pixel state.
    reg [3:0]  px_idx;              // 0..15 within the word-group
    // pixel_buf is a 128-bit register storing 16 RGB332 bytes packed
    // as { px[0], px[1], ..., px[15] } where px[0] is the leftmost pixel.
    reg [127:0] pixel_buf;

    // FB write state.
    reg [1:0]  wr_long_idx;         // 0..3 (we write 4 longs per word-group)
    reg [31:0] fb_write_ptr;        // running pointer into the chunky FB

    // ---------------- Functions / helpers ---------------------------
    // Convert a 12-bit Amiga color (4R, 4G, 4B) into RGB332.
    function [7:0] color12_to_rgb332;
        input [11:0] c12;
        reg [3:0]   r, g, b;
        begin
            r = c12[11:8];
            g = c12[7:4];
            b = c12[3:0];
            // R332 takes the top 3 bits of R4, G332 top 3 of G4, B332 top 2 of B4.
            color12_to_rgb332 = {r[3:1], g[3:1], b[3:2]};
        end
    endfunction

    // Apply half-brite (shift each channel right by 1).
    function [11:0] color12_halfbrite;
        input [11:0] c12;
        reg [3:0]   r, g, b;
        begin
            r = c12[11:8] >> 1;
            g = c12[7:4]  >> 1;
            b = c12[3:0]  >> 1;
            color12_halfbrite = {r, g, b};
        end
    endfunction

    // Extract bit pix_idx of word_data (bit 15 = leftmost pixel = px=0).
    function bit_at;
        input [15:0] word_data;
        input [3:0]  pix;
        begin
            bit_at = word_data[4'd15 - pix];
        end
    endfunction

    // Apply horizontal scroll to the current plane word, taking high
    // bits from the previous word.  scroll=0 means no shift.  The
    // bitmap visually moves RIGHT by `scroll` pixels.
    function [15:0] scrolled_word;
        input [15:0] prev_w;
        input [15:0] cur_w;
        input [3:0]  scroll;
        reg [31:0] joined;
        begin
            joined = {prev_w, cur_w};
            // result = bits [15+scroll : scroll]
            scrolled_word = joined[15+scroll -: 16];
        end
    endfunction

    // Sprite compositing.  Returns 9'b{hit, sprite_idx[2:0], color_idx[4:0]};
    // hit=1 means an opaque sprite pixel exists at the given X.  Sprite 0
    // has highest priority (returned first if multiple sprites overlap).
    // sprite_idx is the index of the winning sprite; callers can derive
    // the sprite-pair level (idx >> 1) for BPLCON2 priority comparisons.
    //
    // Handles 1bpp (data_b == 0), 2bpp, and ATTACHED pair mode.  When the
    // odd sprite of a pair has ATT set we consider the pair as a single
    // 4-bit (16-colour) sprite indexed by the odd sprite's base palette.
    function [8:0] sprite_pixel;
        input [8:0] screen_x;
        integer si;
        reg [8:0] sx;
        reg done;
        reg [3:0] bp;
        reg [4:0] cidx;
        reg [3:0] sv;            // sprite pixel value (1bpp:0..1, 2bpp:0..3, ATT:0..15)
        reg       att;           // odd partner has ATT bit set?
        reg       bpp2;
        reg       skip;
        begin
            sprite_pixel = 9'd0;
            done = 1'b0;
            for (si = 0; si < 8; si = si + 1) begin
                // The even sprite of an attached pair is "consumed" by the
                // odd sprite's compositing path; skip standalone hit-check.
                att  = spr_ctl[si | 1][7] && spr_ctl[si | 1][6];  // partner's ATT && BPP2
                skip = (si[0] == 1'b0) && att;
                bpp2 = spr_ctl[si][6];
                if (!done && spr_active[si] && !skip) begin
                    // Sprite X is 8-bit in SPRxCTL[23:16]; zero-extend so a
                    // sprite placed at X < 256 still composites under hires.
                    sx = {1'b0, spr_ctl[si][23:16]};
                    if (screen_x >= sx && screen_x < (sx + 9'd16)) begin
                        bp   = 4'd15 - (screen_x[3:0] - sx[3:0]);
                        cidx = spr_ctl[si][4:0];
                        if (si[0] && att) begin
                            // ATT: combine even partner's two planes with odd's two.
                            sv = {spr_word_b[si][bp], spr_word_a[si][bp],
                                  spr_word_b[si & 3'b110][bp], spr_word_a[si & 3'b110][bp]};
                        end else if (bpp2) begin
                            sv = {2'd0, spr_word_b[si][bp], spr_word_a[si][bp]};
                        end else begin
                            sv = {3'd0, spr_word_a[si][bp]};
                        end
                        if (sv != 4'd0) begin
                            // 1bpp: V=1 -> base.  2bpp: base+V.  ATT: base+V.
                            sprite_pixel = {1'b1, si[2:0],
                                            bpp2 ? (cidx + {1'b0, sv}) : cidx};
                            done = 1'b1;
                        end
                    end
                end
            end
        end
    endfunction

    // Compose the 6-bit pixel value from the latched plane words, with
    // per-playfield horizontal scroll applied.  PF1 (odd planes 0,2,4)
    // uses PF1H; PF2 (even planes 1,3,5) uses PF2H.
    function [7:0] pixel_value;
        input [3:0] pix;
        integer pli;
        reg [7:0] v;
        reg [15:0] scrolled;
        reg [3:0] sh;
        begin
            v = 8'd0;
            for (pli = 0; pli < 8; pli = pli + 1) begin
                if (pli < nplanes) begin
                    sh = (pli[0]) ? pf2h : pf1h;
                    scrolled = scrolled_word(plane_prev[pli], plane_word[pli], sh);
                    v[pli] = scrolled[4'd15 - pix];
                end
            end
            pixel_value = v;
        end
    endfunction

    // ---------------- Main always block -----------------------------
    integer i;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            bplcon0 <= 32'd0;
            bplcon1 <= 32'd0;
            bplcon2 <= 32'd0;
            // Default DIW covers the entire rendered area (both lores 256
            // and hires 512 widths).  Software that wants a narrower
            // window must program DIWSTRT/DIWSTOP explicitly.
            diwstrt <= 32'd0;
            diwstop <= 32'hFFFF_FFFF;
            for (i = 0; i < 8; i = i + 1) bpl_pt[i] <= 32'd0;
            bpl_mod[0] <= 32'd0;
            bpl_mod[1] <= 32'd0;
            for (i = 0; i < 64; i = i + 1) colors[i] <= 32'd0;
            for (i = 0; i < 8;  i = i + 1) begin
                spr_pt[i]    <= 32'd0;
                spr_ctl[i]   <= 32'd0;
                spr_row_pt[i] <= 32'd0;
                spr_word_a[i] <= 16'd0;
                spr_word_b[i] <= 16'd0;
            end
            sprena      <= 8'd0;
            spr_active  <= 8'd0;
            spr_idx     <= 4'd0;
            den_busy <= 1'b0;

            state <= S_IDLE;
            row_idx <= 16'd0;
            for (i = 0; i < 8; i = i + 1) row_bpl_pt[i] <= 32'd0;
            row_prev_color <= 12'd0;

            wg_idx  <= 5'd0;
            pi_idx  <= 4'd0;
            for (i = 0; i < 8; i = i + 1) plane_word[i] <= 16'd0;
            for (i = 0; i < 8; i = i + 1) plane_prev[i] <= 16'd0;
            px_idx <= 4'd0;
            pixel_buf <= 128'd0;
            wr_long_idx <= 2'd0;
            fb_write_ptr <= 32'd0;

            mst_req_r <= 1'b0;
            mst_we <= 1'b0;
            mst_addr <= 32'd0;
            mst_wdata <= 32'd0;
            mst_be <= 4'b0000;
        end else begin
            // -------- Slave writes --------
            if (slv_req && slv_we) begin
                if (slv_addr[8]) begin
                    // ---- Bank 1 writes ($300..$3FF) ----
                    case (slv_addr[7:0])
                        8'h00: bpl_pt[6] <= slv_wdata;
                        8'h04: bpl_pt[7] <= slv_wdata;
                        default: begin
                            if (slv_addr[7] == 1'b1)
                                colors[{1'b1, slv_addr[6:2]}] <= slv_wdata;
                        end
                    endcase
                end else
                case (slv_addr[7:0])
                    8'h00: bplcon0   <= slv_wdata;
                    8'h04: bplcon1   <= slv_wdata;
                    8'h08: bplcon2   <= slv_wdata;
                    8'h0C: diwstrt   <= slv_wdata;
                    8'h74: diwstop   <= slv_wdata;
                    8'h10: bpl_pt[0] <= slv_wdata;
                    8'h14: bpl_pt[1] <= slv_wdata;
                    8'h18: bpl_pt[2] <= slv_wdata;
                    8'h1C: bpl_pt[3] <= slv_wdata;
                    8'h20: bpl_pt[4] <= slv_wdata;
                    8'h24: bpl_pt[5] <= slv_wdata;
                    8'h28: bpl_mod[0] <= slv_wdata;
                    8'h2C: bpl_mod[1] <= slv_wdata;
                    8'h30: spr_pt[0]  <= slv_wdata;
                    8'h34: spr_ctl[0] <= slv_wdata;
                    8'h38: spr_pt[1]  <= slv_wdata;
                    8'h3C: spr_ctl[1] <= slv_wdata;
                    8'h40: begin
                        // DENRUN: kick rasterization.
                        if (!den_busy) begin
                            den_busy <= 1'b1;
                            state    <= S_ROW_INIT;
                            row_idx  <= 16'd0;
                            row_bpl_pt[0] <= bpl_pt[0];
                            row_bpl_pt[1] <= bpl_pt[1];
                            row_bpl_pt[2] <= bpl_pt[2];
                            row_bpl_pt[3] <= bpl_pt[3];
                            row_bpl_pt[4] <= bpl_pt[4];
                            row_bpl_pt[5] <= bpl_pt[5];
                            row_bpl_pt[6] <= bpl_pt[6];
                            row_bpl_pt[7] <= bpl_pt[7];
                            // Initialise sprite running pointers.
                            spr_row_pt[0] <= spr_pt[0];
                            spr_row_pt[1] <= spr_pt[1];
                            spr_row_pt[2] <= spr_pt[2];
                            spr_row_pt[3] <= spr_pt[3];
                            spr_row_pt[4] <= spr_pt[4];
                            spr_row_pt[5] <= spr_pt[5];
                            spr_row_pt[6] <= spr_pt[6];
                            spr_row_pt[7] <= spr_pt[7];
                            fb_write_ptr  <= FB_BASE;
                        end
                    end
                    8'h48: spr_pt[2]  <= slv_wdata;
                    8'h4C: spr_ctl[2] <= slv_wdata;
                    8'h50: spr_pt[3]  <= slv_wdata;
                    8'h54: spr_ctl[3] <= slv_wdata;
                    8'h58: spr_pt[4]  <= slv_wdata;
                    8'h5C: spr_ctl[4] <= slv_wdata;
                    8'h60: spr_pt[5]  <= slv_wdata;
                    8'h64: spr_ctl[5] <= slv_wdata;
                    8'h68: spr_pt[6]  <= slv_wdata;
                    8'h6C: spr_ctl[6] <= slv_wdata;
                    8'h70: sprena     <= slv_wdata[7:0];
                    8'h78: spr_pt[7]  <= slv_wdata;
                    8'h7C: spr_ctl[7] <= slv_wdata;
                    default: begin
                        if (slv_addr[7] == 1'b1)
                            colors[{1'b0, slv_addr[6:2]}] <= slv_wdata;
                    end
                endcase
            end

            // -------- State machine --------
            case (state)
                S_IDLE: begin
                    mst_req_r <= 1'b0;
                end

                S_ROW_INIT: begin
                    // Reset HAM prev color and per-plane scroll-prev at row start.
                    row_prev_color <= colors[0][11:0];
                    plane_prev[0] <= 16'd0;
                    plane_prev[1] <= 16'd0;
                    plane_prev[2] <= 16'd0;
                    plane_prev[3] <= 16'd0;
                    plane_prev[4] <= 16'd0;
                    plane_prev[5] <= 16'd0;
                    plane_prev[6] <= 16'd0;
                    plane_prev[7] <= 16'd0;
                    state <= S_SPR_PREP;
                end

                S_SPR_PREP: begin : spr_prep_block
                    // Decide which sprites overlap this row.  An enabled
                    // sprite is active when YPOS <= row < YPOS+HEIGHT and
                    // HEIGHT > 0.  All comparisons are 16-bit unsigned.
                    integer s;
                    reg [15:0] yp, hh, yend;
                    reg [7:0]  mask;
                    mask = 8'd0;
                    for (s = 0; s < 8; s = s + 1) begin
                        yp   = {8'd0, spr_ctl[s][31:24]};
                        hh   = {8'd0, spr_ctl[s][15:8]};
                        yend = yp + hh;
                        if (sprena[s] && (hh != 16'd0) &&
                            (row_idx >= yp) && (row_idx < yend)) begin
                            mask[s] = 1'b1;
                        end
                        // Clear stale sprite data so inactive slots don't
                        // bleed last row's pixels into the compositor.
                        if (!mask[s]) begin
                            spr_word_a[s] <= 16'd0;
                            spr_word_b[s] <= 16'd0;
                        end
                    end
                    spr_active <= mask;
                    spr_idx    <= 4'd0;
                    state      <= S_SPR_FETCH;
                end

                S_SPR_FETCH: begin
                    // Walk through sprites 0..7. For each active one, fetch
                    // its current row word(s).
                    //   1bpp: one 16-bit half-word -> data_a, data_b = 0,
                    //         pointer += 2.
                    //   2bpp: one 32-bit word -> {data_a, data_b}, pointer += 4.
                    if (spr_idx == 4'd8) begin
                        mst_req_r <= 1'b0;
                        state     <= S_WG_INIT;
                    end else if (!spr_active[spr_idx[2:0]]) begin
                        spr_idx <= spr_idx + 4'd1;
                    end else begin
                        mst_req_r <= 1'b1;
                        mst_we    <= 1'b0;
                        mst_addr  <= spr_row_pt[spr_idx[2:0]];
                        mst_be    <= 4'b1111;
                        if (mst_ack) begin
                            mst_req_r <= 1'b0;
                            if (spr_ctl[spr_idx[2:0]][6]) begin
                                // 2bpp: full 32-bit row, A in high half, B in low half.
                                spr_word_a[spr_idx[2:0]] <= mst_rdata[31:16];
                                spr_word_b[spr_idx[2:0]] <= mst_rdata[15:0];
                                spr_row_pt[spr_idx[2:0]] <= spr_row_pt[spr_idx[2:0]] + 32'd4;
                            end else begin
                                // 1bpp legacy: 16-bit half-word, data_b = 0.
                                spr_word_a[spr_idx[2:0]] <= spr_row_pt[spr_idx[2:0]][1]
                                    ? mst_rdata[15:0]
                                    : mst_rdata[31:16];
                                spr_word_b[spr_idx[2:0]] <= 16'd0;
                                spr_row_pt[spr_idx[2:0]] <= spr_row_pt[spr_idx[2:0]] + 32'd2;
                            end
                            spr_idx <= spr_idx + 4'd1;
                        end
                    end
                end

                S_WG_INIT: begin
                    wg_idx <= 5'd0;
                    pi_idx <= 4'd0;
                    px_idx <= 4'd0;
                    pixel_buf <= 128'd0;
                    state <= S_FETCH_P;
                end

                S_FETCH_P: begin
                    if (pi_idx >= nplanes) begin
                        // All needed planes fetched: process pixels.
                        px_idx <= 4'd0;
                        state  <= S_PROC;
                        mst_req_r <= 1'b0;
                    end else begin
                        mst_req_r <= 1'b1;
                        mst_we    <= 1'b0;
                        mst_addr  <= row_bpl_pt[pi_idx];
                        mst_be    <= 4'b1111;
                        if (mst_ack) begin
                            mst_req_r <= 1'b0;
                            // Latch the half-word at the relevant address.
                            plane_word[pi_idx] <= row_bpl_pt[pi_idx][1]
                                ? mst_rdata[15:0]
                                : mst_rdata[31:16];
                            row_bpl_pt[pi_idx] <= row_bpl_pt[pi_idx] + 32'd2;
                            pi_idx <= pi_idx + 4'd1;
                        end
                    end
                end

                S_PROC: begin : proc_block
                    reg [7:0]  pv;
                    reg [11:0] cur_color;
                    reg [1:0]  ham_ctrl;
                    reg [3:0]  ham_data;        // HAM6
                    reg [5:0]  ham8_data;       // HAM8 (6-bit channel data)
                    reg [3:0]  ham8_data4;      // ham8_data quantised to 4-bit
                    reg [2:0]  pf1_idx;
                    reg [2:0]  pf2_idx;
                    reg [3:0]  dpf_idx;
                    reg [5:0]  palette_idx;
                    reg [11:0] palette_color;
                    reg [7:0]  rgb332;
                    reg [8:0]  spr_hit;     // {hit, sprite_idx[2:0], color_idx[4:0]}
                    reg [1:0]  spr_pair;    // pair level (idx >> 1)
                    reg        spr_front;   // sprite wins over playfield?
                    reg        pf_is_bg;    // current pixel is the playfield background
                    reg [8:0]  screen_x;
                    reg        in_diw;      // (x,y) inside the display window

                    pv = pixel_value(px_idx);
                    screen_x = {wg_idx[4:0], px_idx};
                    spr_hit  = sprite_pixel(screen_x);
                    spr_pair = spr_hit[7:6];
                    in_diw   = ({7'd0, screen_x} >= diw_x0) && ({7'd0, screen_x} < diw_x1) &&
                               (row_idx        >= diw_y0) && (row_idx        < diw_y1);
                    // Outside the display window the playfield reverts to
                    // background (COLOR00); sprites still composite over it.
                    if (!in_diw) pv = 8'd0;

                    if (ham8_en) begin
                        // HAM8: 6 data bits + 2 control bits.  Control selects
                        // palette base (64-entry) or modify-R/G/B with 6-bit
                        // precision; we quantise the new channel to 4 bits.
                        ham_ctrl  = pv[1:0];                 // BPL1/BPL2 = control
                        ham8_data = pv[7:2];                 // BPL3..BPL8 = data
                        ham8_data4 = ham8_data[5:2];         // top 4 bits -> 4-bit ch
                        case (ham_ctrl)
                            2'b00: cur_color = colors[ham8_data][11:0];
                            2'b01: cur_color = {row_prev_color[11:4], ham8_data4};
                            2'b10: cur_color = {ham8_data4, row_prev_color[7:0]};
                            2'b11: cur_color = {row_prev_color[11:8], ham8_data4, row_prev_color[3:0]};
                        endcase
                        row_prev_color <= cur_color;
                    end else if (ham6_en) begin
                        ham_ctrl = pv[5:4];
                        ham_data = pv[3:0];
                        case (ham_ctrl)
                            2'b00: cur_color = colors[{2'b00, ham_data}][11:0];  // palette[0..15]
                            2'b01: cur_color = {row_prev_color[11:4], ham_data};
                            2'b10: cur_color = {ham_data, row_prev_color[7:0]};
                            2'b11: cur_color = {row_prev_color[11:8], ham_data, row_prev_color[3:0]};
                        endcase
                        row_prev_color <= cur_color;
                    end else if (ehb_en) begin
                        // 6-plane EHB: bit 5 = half-brite flag, low 5 bits = palette index.
                        palette_color = colors[{1'b0, pv[4:0]}][11:0];
                        cur_color = pv[5] ? color12_halfbrite(palette_color) : palette_color;
                    end else if (dpf_en) begin
                        // Dual playfield: planes 0,2,4 -> PF1 (low 3 bits),
                        // planes 1,3,5 -> PF2.  pv is pre-masked by nplanes.
                        pf1_idx = {pv[4], pv[2], pv[0]};
                        pf2_idx = {pv[5], pv[3], pv[1]};
                        // Determine which PF is in front and visible.
                        if (pf2_pri) begin
                            // PF2 in front
                            if (pf2_idx != 3'd0)      dpf_idx = {1'b1, pf2_idx};      // PF2 colors 8..15
                            else if (pf1_idx != 3'd0) dpf_idx = {1'b0, pf1_idx};      // PF1 colors 0..7
                            else                      dpf_idx = 4'd0;
                        end else begin
                            // PF1 in front (default)
                            if (pf1_idx != 3'd0)      dpf_idx = {1'b0, pf1_idx};
                            else if (pf2_idx != 3'd0) dpf_idx = {1'b1, pf2_idx};
                            else                      dpf_idx = 4'd0;
                        end
                        cur_color = colors[{2'b00, dpf_idx}][11:0];
                    end else begin
                        // Plain indexed.  Build the palette index from the
                        // active plane bits; index width follows nplanes (max 5).
                        palette_idx = {3'd0, pv[2:0]};
                        if (nplanes >= 4'd4) palette_idx[3] = pv[3];
                        if (nplanes >= 4'd5) palette_idx[4] = pv[4];
                        cur_color = colors[palette_idx][11:0];
                    end

                    // Sprite vs. playfield priority.
                    //   BPLCON2[5:3] = PF1P, BPLCON2[2:0] = PF2P.
                    //   A sprite-pair with level L wins over playfield X iff
                    //   L < PFxP.  PFxP == 0 puts the playfield in front of
                    //   all sprites; PFxP >= 4 puts it behind all sprites
                    //   (the historical default).  In non-DPF modes only
                    //   PF1P is consulted; in DPF mode we apply each
                    //   playfield's threshold independently and let the
                    //   front-most playfield (per pf2_pri) take precedence
                    //   only when no sprite would beat it.
                    pf_is_bg = dpf_en ? (dpf_idx == 4'd0)
                                      : (pv == 6'd0);
                    spr_front = spr_hit[8] && ({1'b0, spr_pair} < pf1p);
                    if (spr_hit[8] && (pf_is_bg || spr_front))
                        cur_color = colors[{1'b0, spr_hit[4:0]}][11:0];

                    rgb332 = color12_to_rgb332(cur_color);
                    // Pack into pixel_buf at slot px_idx (leftmost = slot 0).
                    case (px_idx)
                        4'd0:  pixel_buf[127:120] <= rgb332;
                        4'd1:  pixel_buf[119:112] <= rgb332;
                        4'd2:  pixel_buf[111:104] <= rgb332;
                        4'd3:  pixel_buf[103:96]  <= rgb332;
                        4'd4:  pixel_buf[95:88]   <= rgb332;
                        4'd5:  pixel_buf[87:80]   <= rgb332;
                        4'd6:  pixel_buf[79:72]   <= rgb332;
                        4'd7:  pixel_buf[71:64]   <= rgb332;
                        4'd8:  pixel_buf[63:56]   <= rgb332;
                        4'd9:  pixel_buf[55:48]   <= rgb332;
                        4'd10: pixel_buf[47:40]   <= rgb332;
                        4'd11: pixel_buf[39:32]   <= rgb332;
                        4'd12: pixel_buf[31:24]   <= rgb332;
                        4'd13: pixel_buf[23:16]   <= rgb332;
                        4'd14: pixel_buf[15:8]    <= rgb332;
                        4'd15: pixel_buf[7:0]     <= rgb332;
                    endcase
                    if (px_idx == 4'd15) begin
                        wr_long_idx <= 2'd0;
                        state       <= S_WRITE;
                    end else begin
                        px_idx <= px_idx + 4'd1;
                    end
                end

                S_WRITE: begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b1;
                    mst_addr  <= fb_write_ptr;
                    mst_be    <= 4'b1111;
                    case (wr_long_idx)
                        2'd0: mst_wdata <= pixel_buf[127:96];
                        2'd1: mst_wdata <= pixel_buf[95:64];
                        2'd2: mst_wdata <= pixel_buf[63:32];
                        2'd3: mst_wdata <= pixel_buf[31:0];
                    endcase
                    if (mst_ack) begin
                        mst_req_r <= 1'b0;
                        fb_write_ptr <= fb_write_ptr + 32'd4;
                        if (wr_long_idx == 2'd3) begin
                            state <= S_NEXT_WG;
                        end else begin
                            wr_long_idx <= wr_long_idx + 2'd1;
                        end
                    end
                end

                S_NEXT_WG: begin
                    if (wg_idx == wg_last) begin
                        state <= S_ROW_END;
                    end else begin
                        // The word we just consumed becomes the "prev" word
                        // for the next WG's scroll calculation.
                        plane_prev[0] <= plane_word[0];
                        plane_prev[1] <= plane_word[1];
                        plane_prev[2] <= plane_word[2];
                        plane_prev[3] <= plane_word[3];
                        plane_prev[4] <= plane_word[4];
                        plane_prev[5] <= plane_word[5];
                        plane_prev[6] <= plane_word[6];
                        plane_prev[7] <= plane_word[7];
                        wg_idx <= wg_idx + 5'd1;
                        pi_idx <= 4'd0;
                        px_idx <= 4'd0;
                        pixel_buf <= 128'd0;
                        state <= S_FETCH_P;
                    end
                end

                S_ROW_END: begin
                    // Apply per-row modulos.  BPL1MOD is added to all odd
                    // 1-indexed planes (0-indexed 0, 2, 4, 6); BPL2MOD to
                    // even (1, 3, 5, 7).
                    row_bpl_pt[0] <= row_bpl_pt[0] + bpl_mod[0];
                    row_bpl_pt[2] <= row_bpl_pt[2] + bpl_mod[0];
                    row_bpl_pt[4] <= row_bpl_pt[4] + bpl_mod[0];
                    row_bpl_pt[6] <= row_bpl_pt[6] + bpl_mod[0];
                    row_bpl_pt[1] <= row_bpl_pt[1] + bpl_mod[1];
                    row_bpl_pt[3] <= row_bpl_pt[3] + bpl_mod[1];
                    row_bpl_pt[5] <= row_bpl_pt[5] + bpl_mod[1];
                    row_bpl_pt[7] <= row_bpl_pt[7] + bpl_mod[1];
                    if (row_idx == FB_H[15:0] - 16'd1) begin
                        state <= S_DONE;
                    end else begin
                        row_idx <= row_idx + 16'd1;
                        state   <= S_ROW_INIT;
                    end
                end

                S_DONE: begin
                    den_busy <= 1'b0;
                    state    <= S_IDLE;
                end

                default: state <= S_IDLE;
            endcase
        end
    end

endmodule
