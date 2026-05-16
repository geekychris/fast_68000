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
// Sprites, hardware scroll, hires, HAM8, and genlock are not implemented
// in this phase (see DENISE.md for the deferred list).
//
// Pixels: leftmost pixel of a word = bit 15 (Amiga convention). Plane i's
// bit contributes the i-th bit of the pixel value (plane 0 = LSB).
//
// Triggering: write any value to DENRUN. Denise rasterizes the full
// 256x192 framebuffer and sets DENSTAT.BUSY low when done.

`include "m68k_defs.vh"

module m68k_denise #(
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
    input  wire [7:0]  slv_addr,
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
    input  wire [31:0] mst_rdata
);
    // ---------------- Registers programmed by the CPU ----------------
    reg [31:0] bplcon0;
    reg [31:0] bplcon1;
    reg [31:0] bplcon2;
    reg [31:0] bpl_pt   [0:5];
    reg [31:0] bpl_mod  [0:1];   // BPL1MOD (odd-plane stride bonus), BPL2MOD (even)
    reg [31:0] colors   [0:31];
    reg        den_busy;

    // BPLCON0 decode.
    wire [2:0] nplanes = bplcon0[14:12];
    wire       ham_en  = bplcon0[11];
    wire       dpf_en  = bplcon0[10];
    // EHB is the implicit fall-through when nplanes=6 and neither HAM nor DPF.
    wire       ehb_en  = (nplanes == 3'd6) && !ham_en && !dpf_en;

    // BPLCON1 decode: horizontal scroll per playfield (4 bits each).
    //   PF1H = BPLCON1[3:0] (applied to planes 0, 2, 4 - "odd" 1-indexed)
    //   PF2H = BPLCON1[7:4] (applied to planes 1, 3, 5 - "even" 1-indexed)
    wire [3:0] pf1h = bplcon1[3:0];
    wire [3:0] pf2h = bplcon1[7:4];

    // BPLCON2: PF2 priority over PF1 when bit 6 is set.
    wire       pf2_pri = bplcon2[6];

    // ---------------- mst_req gating --------------------------------
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

    // ---------------- Slave reads -----------------------------------
    integer rk;
    always @* begin
        slv_rdata = 32'd0;
        case (slv_addr[7:0])
            8'h00: slv_rdata = bplcon0;
            8'h04: slv_rdata = bplcon1;
            8'h08: slv_rdata = bplcon2;
            8'h10: slv_rdata = bpl_pt[0];
            8'h14: slv_rdata = bpl_pt[1];
            8'h18: slv_rdata = bpl_pt[2];
            8'h1C: slv_rdata = bpl_pt[3];
            8'h20: slv_rdata = bpl_pt[4];
            8'h24: slv_rdata = bpl_pt[5];
            8'h28: slv_rdata = bpl_mod[0];
            8'h2C: slv_rdata = bpl_mod[1];
            8'h40: slv_rdata = 32'd0;                 // DENRUN write-only
            8'h44: slv_rdata = {31'd0, den_busy};     // DENSTAT
            default: begin
                if (slv_addr[7] == 1'b1) begin
                    // $80..$FF = COLOR00..COLOR1F (32 entries x 4 bytes).
                    slv_rdata = colors[slv_addr[6:2]];
                end
            end
        endcase
    end

    // ---------------- Bitplane fetch / pixel proc state machine ----
    // Per row we have FB_W/16 = 16 word-groups.  For each word-group we
    // read up to 6 bitplane words from memory, then sequentially process
    // 16 pixels, then write 4 longs (16 chunky bytes) to the framebuffer.
    localparam S_IDLE     = 5'd0;
    localparam S_ROW_INIT = 5'd1;
    localparam S_WG_INIT  = 5'd2;
    localparam S_FETCH_P  = 5'd3;
    localparam S_PROC     = 5'd4;
    localparam S_WRITE    = 5'd5;
    localparam S_NEXT_WG  = 5'd6;
    localparam S_ROW_END  = 5'd7;
    localparam S_DONE     = 5'd8;

    reg [4:0]  state;

    // Per-row state.
    reg [15:0] row_idx;             // 0..FB_H-1
    reg [31:0] row_bpl_pt [0:5];    // current row pointers (modified by reads)
    reg [11:0] row_prev_color;      // HAM prev-color, init to COLOR00 each row

    // Per-word-group state.
    reg [4:0]  wg_idx;              // 0..(FB_W/16 - 1)
    reg [2:0]  pi_idx;              // current plane being fetched (0..5)
    reg [15:0] plane_word [0:5];    // latched 16-bit word per plane
    reg [15:0] plane_prev [0:5];    // previous word per plane (for scroll)

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

    // Compose the 6-bit pixel value from the latched plane words, with
    // per-playfield horizontal scroll applied.  PF1 (odd planes 0,2,4)
    // uses PF1H; PF2 (even planes 1,3,5) uses PF2H.
    function [5:0] pixel_value;
        input [3:0] pix;
        integer pli;
        reg [5:0] v;
        reg [15:0] scrolled;
        reg [3:0] sh;
        begin
            v = 6'd0;
            for (pli = 0; pli < 6; pli = pli + 1) begin
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
            bpl_pt[0] <= 32'd0;
            bpl_pt[1] <= 32'd0;
            bpl_pt[2] <= 32'd0;
            bpl_pt[3] <= 32'd0;
            bpl_pt[4] <= 32'd0;
            bpl_pt[5] <= 32'd0;
            bpl_mod[0] <= 32'd0;
            bpl_mod[1] <= 32'd0;
            for (i = 0; i < 32; i = i + 1) colors[i] <= 32'd0;
            den_busy <= 1'b0;

            state <= S_IDLE;
            row_idx <= 16'd0;
            row_bpl_pt[0] <= 32'd0;
            row_bpl_pt[1] <= 32'd0;
            row_bpl_pt[2] <= 32'd0;
            row_bpl_pt[3] <= 32'd0;
            row_bpl_pt[4] <= 32'd0;
            row_bpl_pt[5] <= 32'd0;
            row_prev_color <= 12'd0;

            wg_idx  <= 5'd0;
            pi_idx  <= 3'd0;
            for (i = 0; i < 6; i = i + 1) plane_word[i] <= 16'd0;
            for (i = 0; i < 6; i = i + 1) plane_prev[i] <= 16'd0;
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
                case (slv_addr[7:0])
                    8'h00: bplcon0   <= slv_wdata;
                    8'h04: bplcon1   <= slv_wdata;
                    8'h08: bplcon2   <= slv_wdata;
                    8'h10: bpl_pt[0] <= slv_wdata;
                    8'h14: bpl_pt[1] <= slv_wdata;
                    8'h18: bpl_pt[2] <= slv_wdata;
                    8'h1C: bpl_pt[3] <= slv_wdata;
                    8'h20: bpl_pt[4] <= slv_wdata;
                    8'h24: bpl_pt[5] <= slv_wdata;
                    8'h28: bpl_mod[0] <= slv_wdata;
                    8'h2C: bpl_mod[1] <= slv_wdata;
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
                            fb_write_ptr  <= FB_BASE;
                        end
                    end
                    default: begin
                        if (slv_addr[7] == 1'b1)
                            colors[slv_addr[6:2]] <= slv_wdata;
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
                    state <= S_WG_INIT;
                end

                S_WG_INIT: begin
                    wg_idx <= 5'd0;
                    pi_idx <= 3'd0;
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
                            pi_idx <= pi_idx + 3'd1;
                        end
                    end
                end

                S_PROC: begin : proc_block
                    reg [5:0]  pv;
                    reg [11:0] cur_color;
                    reg [1:0]  ham_ctrl;
                    reg [3:0]  ham_data;
                    reg [2:0]  pf1_idx;
                    reg [2:0]  pf2_idx;
                    reg [3:0]  dpf_idx;
                    reg [4:0]  palette_idx;
                    reg [11:0] palette_color;
                    reg [7:0]  rgb332;

                    pv = pixel_value(px_idx);

                    if (ham_en && (nplanes == 3'd6)) begin
                        ham_ctrl = pv[5:4];
                        ham_data = pv[3:0];
                        case (ham_ctrl)
                            2'b00: cur_color = colors[{1'b0, ham_data}][11:0];  // use palette[0..15]
                            2'b01: cur_color = {row_prev_color[11:4], ham_data};            // modify B
                            2'b10: cur_color = {ham_data, row_prev_color[7:0]};             // modify R
                            2'b11: cur_color = {row_prev_color[11:8], ham_data, row_prev_color[3:0]}; // modify G
                        endcase
                        row_prev_color <= cur_color;
                    end else if (ehb_en) begin
                        // 6-plane EHB: bit 5 = half-brite flag, low 5 bits = palette index.
                        palette_color = colors[pv[4:0]][11:0];
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
                        cur_color = colors[{1'b0, dpf_idx}][11:0];
                    end else begin
                        // Plain indexed.  Truncate pv to log2(palette size) bits, capped at 5.
                        palette_idx = {2'd0, pv[2:0]};
                        if (nplanes >= 3'd4) palette_idx[3] = pv[3];
                        if (nplanes >= 3'd5) palette_idx[4] = pv[4];
                        cur_color = colors[palette_idx][11:0];
                    end

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
                    if (wg_idx == 5'd15) begin
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
                        wg_idx <= wg_idx + 5'd1;
                        pi_idx <= 3'd0;
                        px_idx <= 4'd0;
                        pixel_buf <= 128'd0;
                        state <= S_FETCH_P;
                    end
                end

                S_ROW_END: begin
                    // Apply per-row modulos.  BPL1MOD is added to all odd
                    // planes (0-indexed 0, 2, 4); BPL2MOD to even (1, 3, 5).
                    row_bpl_pt[0] <= row_bpl_pt[0] + bpl_mod[0];
                    row_bpl_pt[2] <= row_bpl_pt[2] + bpl_mod[0];
                    row_bpl_pt[4] <= row_bpl_pt[4] + bpl_mod[0];
                    row_bpl_pt[1] <= row_bpl_pt[1] + bpl_mod[1];
                    row_bpl_pt[3] <= row_bpl_pt[3] + bpl_mod[1];
                    row_bpl_pt[5] <= row_bpl_pt[5] + bpl_mod[1];
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
