// Amiga-Paula-inspired 4-voice audio engine (Phase 4).
//
// Four independent audio channels, each fetching 8-bit signed samples
// from memory via DMA and emitting at a programmable period.  Each
// channel has its own volume.  Stereo output: channels 0 + 3 -> L,
// channels 1 + 2 -> R (canonical Amiga routing).
//
// Per channel:
//   AUDxLC  : 32-bit byte address of the sample buffer (word-aligned).
//   AUDxLEN : 16-bit word count (each word = 2 mono 8-bit samples).
//             When the channel reaches LEN words, it reloads from AUDxLC.
//   AUDxPER : 16-bit period (system clocks between samples).
//   AUDxVOL : 7-bit volume (0..64).
//
// AUDENA (DMACONP-style enable register) bits 0..3 turn each channel on.
//
// Output: audio_l_o / audio_r_o, signed 16-bit, updated combinationally
// to the current mixed level.  The simulator harness samples these at
// SDL_audio's callback rate (44.1 kHz).
//
// Register page: $00FE_0200..$00FE_02FF.

`include "m68k_defs.vh"

module m68k_paula (
    input  wire        clk,
    input  wire        rst_n,

    // Slave port
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [7:0]  slv_addr,
    input  wire [3:0]  slv_be,
    input  wire [31:0] slv_wdata,
    output reg  [31:0] slv_rdata,

    // Master port (DMA fetch)
    output wire        mst_req,
    output reg         mst_we,
    output reg  [31:0] mst_addr,
    output reg  [31:0] mst_wdata,
    output reg  [3:0]  mst_be,
    input  wire        mst_ack,
    input  wire [31:0] mst_rdata,

    // Audio output: signed 16-bit stereo, updated continuously.
    output reg signed [15:0] audio_l_o,
    output reg signed [15:0] audio_r_o
);
    // --------------- Registers ---------------
    reg [3:0]  audena;                 // channel enables
    reg [31:0] aud_lc   [0:3];
    reg [15:0] aud_len  [0:3];
    reg [15:0] aud_per  [0:3];
    reg [6:0]  aud_vol  [0:3];
    reg        aud_oneshot [0:3];      // AUDxLEN bit 31: don't auto-loop

    // --------------- Per-channel state ---------------
    reg [31:0] ch_cur_addr   [0:3];    // running fetch address
    reg [15:0] ch_words_left [0:3];    // remaining words in current buffer pass
    reg [15:0] ch_word_q     [0:3];    // last fetched 16-bit word
    reg        ch_word_valid [0:3];    // 1 = ch_word_q holds a usable word
    reg        ch_byte_idx   [0:3];    // 0 = high byte, 1 = low byte
    reg [15:0] ch_per_cnt    [0:3];    // counts down to 0
    reg signed [7:0] ch_sample [0:3];  // current emitted sample

    // --------------- Master-request gating ---------------
    reg mst_req_r;
    assign mst_req = mst_req_r && !mst_ack;

    // --------------- Slave reads ---------------
    always @* begin
        slv_rdata = 32'd0;
        case (slv_addr[7:0])
            8'h00: slv_rdata = {28'd0, audena};
            8'h10: slv_rdata = aud_lc[0];
            8'h14: slv_rdata = {16'd0, aud_len[0]};
            8'h18: slv_rdata = {16'd0, aud_per[0]};
            8'h1C: slv_rdata = {25'd0, aud_vol[0]};
            8'h20: slv_rdata = aud_lc[1];
            8'h24: slv_rdata = {16'd0, aud_len[1]};
            8'h28: slv_rdata = {16'd0, aud_per[1]};
            8'h2C: slv_rdata = {25'd0, aud_vol[1]};
            8'h30: slv_rdata = aud_lc[2];
            8'h34: slv_rdata = {16'd0, aud_len[2]};
            8'h38: slv_rdata = {16'd0, aud_per[2]};
            8'h3C: slv_rdata = {25'd0, aud_vol[2]};
            8'h40: slv_rdata = aud_lc[3];
            8'h44: slv_rdata = {16'd0, aud_len[3]};
            8'h48: slv_rdata = {16'd0, aud_per[3]};
            8'h4C: slv_rdata = {25'd0, aud_vol[3]};
            default: slv_rdata = 32'd0;
        endcase
    end

    // --------------- DMA fetch arbiter ---------------
    // At most one channel fetches at a time.  We round-robin between
    // channels that have "need new word" pending.
    reg [1:0] fetch_ch;
    wire [3:0] need_word = {
        audena[3] && !ch_word_valid[3],
        audena[2] && !ch_word_valid[2],
        audena[1] && !ch_word_valid[1],
        audena[0] && !ch_word_valid[0]
    };
    wire fetch_active = |need_word;

    // --------------- Mixing ---------------
    // For each enabled channel: signed sample × volume (0..64). Combine.
    // Output is signed 16-bit, stereo: ch0+ch3 = L, ch1+ch2 = R.
    wire signed [15:0] ch_mixed [0:3];
    genvar gci;
    generate
        for (gci = 0; gci < 4; gci = gci + 1) begin : g_mix
            assign ch_mixed[gci] = audena[gci]
                ? ($signed(ch_sample[gci]) * $signed({1'b0, aud_vol[gci]}))
                : 16'sd0;
        end
    endgenerate

    always @* begin
        audio_l_o = ch_mixed[0] + ch_mixed[3];
        audio_r_o = ch_mixed[1] + ch_mixed[2];
    end

    // --------------- Main always block ---------------
    integer i;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            audena <= 4'd0;
            for (i = 0; i < 4; i = i + 1) begin
                aud_lc[i]  <= 32'd0;
                aud_len[i] <= 16'd0;
                aud_per[i] <= 16'd1;
                aud_vol[i] <= 7'd0;
                aud_oneshot[i] <= 1'b0;
                ch_cur_addr[i]   <= 32'd0;
                ch_words_left[i] <= 16'd0;
                ch_word_q[i]     <= 16'd0;
                ch_word_valid[i] <= 1'b0;
                ch_byte_idx[i]   <= 1'b0;
                ch_per_cnt[i]    <= 16'd0;
                ch_sample[i]     <= 8'sd0;
            end
            fetch_ch <= 2'd0;
            mst_req_r <= 1'b0;
            mst_we    <= 1'b0;
            mst_addr  <= 32'd0;
            mst_wdata <= 32'd0;
            mst_be    <= 4'b0000;
        end else begin
            // ---- slave writes ----
            if (slv_req && slv_we) begin
                case (slv_addr[7:0])
                    8'h00: audena <= slv_wdata[3:0];
                    8'h10: aud_lc[0]  <= slv_wdata;
                    8'h14: begin aud_len[0] <= slv_wdata[15:0]; aud_oneshot[0] <= slv_wdata[31]; end
                    8'h18: aud_per[0] <= slv_wdata[15:0];
                    8'h1C: aud_vol[0] <= slv_wdata[6:0];
                    8'h20: aud_lc[1]  <= slv_wdata;
                    8'h24: begin aud_len[1] <= slv_wdata[15:0]; aud_oneshot[1] <= slv_wdata[31]; end
                    8'h28: aud_per[1] <= slv_wdata[15:0];
                    8'h2C: aud_vol[1] <= slv_wdata[6:0];
                    8'h30: aud_lc[2]  <= slv_wdata;
                    8'h34: begin aud_len[2] <= slv_wdata[15:0]; aud_oneshot[2] <= slv_wdata[31]; end
                    8'h38: aud_per[2] <= slv_wdata[15:0];
                    8'h3C: aud_vol[2] <= slv_wdata[6:0];
                    8'h40: aud_lc[3]  <= slv_wdata;
                    8'h44: begin aud_len[3] <= slv_wdata[15:0]; aud_oneshot[3] <= slv_wdata[31]; end
                    8'h48: aud_per[3] <= slv_wdata[15:0];
                    8'h4C: aud_vol[3] <= slv_wdata[6:0];
                    default: ;
                endcase
            end

            // ---- Initialise channel state when DMA enable bit is set ----
            // Whenever audena[i] toggles low->high we reset that channel's
            // running state to point at the start of the buffer.
            for (i = 0; i < 4; i = i + 1) begin
                if (slv_req && slv_we && slv_addr == 8'h00) begin
                    if (slv_wdata[i] && !audena[i]) begin
                        ch_cur_addr[i]   <= aud_lc[i];
                        ch_words_left[i] <= aud_len[i];
                        ch_word_q[i]     <= 16'd0;
                        ch_word_valid[i] <= 1'b0;
                        ch_byte_idx[i]   <= 1'b0;
                        ch_per_cnt[i]    <= aud_per[i];
                        ch_sample[i]     <= 8'sd0;
                    end
                end
            end

            // ---- DMA fetch logic ----
            // Issue one read at a time.  Round-robin between channels
            // that need a fresh word.
            if (!mst_req_r && fetch_active) begin
                // Find the next channel needing a word, starting from fetch_ch.
                if      (need_word[fetch_ch]) begin
                    mst_req_r <= 1'b1;
                    mst_we    <= 1'b0;
                    mst_addr  <= ch_cur_addr[fetch_ch];
                    mst_be    <= 4'b1111;
                end else if (need_word[(fetch_ch + 2'd1) & 2'b11]) begin
                    fetch_ch <= (fetch_ch + 2'd1) & 2'b11;
                end else if (need_word[(fetch_ch + 2'd2) & 2'b11]) begin
                    fetch_ch <= (fetch_ch + 2'd2) & 2'b11;
                end else if (need_word[(fetch_ch + 2'd3) & 2'b11]) begin
                    fetch_ch <= (fetch_ch + 2'd3) & 2'b11;
                end
            end
            if (mst_req_r && mst_ack) begin
                mst_req_r <= 1'b0;
                // Pick the half-word for this fetch address.
                ch_word_q[fetch_ch] <= ch_cur_addr[fetch_ch][1]
                    ? mst_rdata[15:0]
                    : mst_rdata[31:16];
                ch_word_valid[fetch_ch] <= 1'b1;
                // Advance address; reload at end of buffer unless this
                // channel is in one-shot mode.
                if (ch_words_left[fetch_ch] == 16'd1) begin
                    if (aud_oneshot[fetch_ch]) begin
                        // One-shot: disable this channel cleanly.
                        audena[fetch_ch] <= 1'b0;
                    end else begin
                        // Loop: reload pointer + length.
                        ch_cur_addr[fetch_ch]   <= aud_lc[fetch_ch];
                        ch_words_left[fetch_ch] <= aud_len[fetch_ch];
                    end
                end else begin
                    ch_cur_addr[fetch_ch]   <= ch_cur_addr[fetch_ch] + 32'd2;
                    ch_words_left[fetch_ch] <= ch_words_left[fetch_ch] - 16'd1;
                end
                // Advance round-robin pointer.
                fetch_ch <= (fetch_ch + 2'd1) & 2'b11;
            end

            // ---- Period counters: emit next sample when one reaches 0 ----
            for (i = 0; i < 4; i = i + 1) begin
                if (audena[i]) begin
                    if (ch_per_cnt[i] == 16'd0) begin
                        // Time to emit the next sample.
                        if (ch_word_valid[i]) begin
                            if (ch_byte_idx[i] == 1'b0) begin
                                // Emit high byte first.
                                ch_sample[i] <= ch_word_q[i][15:8];
                                ch_byte_idx[i] <= 1'b1;
                            end else begin
                                // Emit low byte; word now consumed.
                                ch_sample[i] <= ch_word_q[i][7:0];
                                ch_byte_idx[i] <= 1'b0;
                                ch_word_valid[i] <= 1'b0;  // need next word
                            end
                            ch_per_cnt[i] <= aud_per[i];
                        end
                        // else: stall until word arrives via DMA
                    end else begin
                        ch_per_cnt[i] <= ch_per_cnt[i] - 16'd1;
                    end
                end else begin
                    ch_sample[i] <= 8'sd0;
                end
            end
        end
    end

endmodule
