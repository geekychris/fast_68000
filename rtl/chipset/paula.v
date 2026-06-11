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



module paula (
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
    output reg signed [15:0] audio_r_o,

    // External interrupt source pins (level-sensitive).  Rising edges latch
    // into INTREQ; the corresponding bits also auto-load from internal audio
    // channel completion (not implemented yet — defer).
    input  wire        cia_a_int_i,    // PORTS  (bit 3,  IPL 2)
    input  wire        cia_b_int_i,    // EXTER  (bit 13, IPL 6)
    input  wire        blt_int_i,      // BLIT   (bit 6,  IPL 3)
    input  wire        cop_int_i,      // COPER  (bit 4,  IPL 3)
    input  wire        vblank_int_i,   // VERTB  (bit 5,  IPL 3)
    input  wire        dskblk_int_i,   // DSKBLK (bit 1,  IPL 1) — floppy DMA done
    input  wire        dsksyn_int_i,   // DSKSYN (bit 12, IPL 5) — disk sync word matched

    // CPU stop indication for the wedge-recovery watchdog (see below).
    input  wire        cpu_in_stop_i,

    // 3-bit IPL output for the CPU's IRQ controller.  Computed from the
    // priority of INTREQ & INTENA bits when INTEN (intena[14]) is set.
    output reg  [2:0]  irq_level_o,

    // One-cycle pulse when the wedge watchdog kicks — wired to the cache
    // inval_all so the IRQ-entry vector read goes to the bus fresh and
    // doesn't latch a stale bus_resp from an earlier transaction race.
    output wire        watchdog_kick_o
);
    // --------------- Registers ---------------
    reg [3:0]  audena;                 // channel enables
    reg [31:0] aud_lc   [0:3];
    reg [15:0] aud_len  [0:3];
    reg [15:0] aud_per  [0:3];
    reg [6:0]  aud_vol  [0:3];
    reg        aud_oneshot [0:3];      // AUDxLEN bit 31: don't auto-loop

    // --------------- Interrupt controller ---------------
    // INTREQ bit assignments (Amiga canonical):
    //   0  TBE      1  DSKBLK    2  SOFT     3  PORTS (CIA-A)
    //   4  COPER    5  VERTB     6  BLIT     7..10 AUD0..3
    //   11 RBF      12 DSKSYN    13 EXTER (CIA-B)
    //   bit 14 (INTREQ) reserved; bit 14 of INTENA is INTEN (master enable).
    reg [13:0] intreq;
    reg [14:0] intena;

    // Edge-detect external pins; the rising-edge OR into INTREQ.
    reg cia_a_int_last, cia_b_int_last, blt_int_last, cop_int_last, vblank_int_last;
    reg dskblk_int_last;
    reg dsksyn_int_last;
    wire cia_a_edge  = cia_a_int_i  & ~cia_a_int_last;
    wire cia_b_edge  = cia_b_int_i  & ~cia_b_int_last;
    wire blt_edge    = blt_int_i    & ~blt_int_last;
    wire cop_edge    = cop_int_i    & ~cop_int_last;
    wire vblank_edge = vblank_int_i & ~vblank_int_last;
    wire dskblk_edge = dskblk_int_i & ~dskblk_int_last;
    wire dsksyn_edge = dsksyn_int_i & ~dsksyn_int_last;
`ifdef PAULA_INTREQ_TRACE
    always @(posedge clk) if (rst_n && dsksyn_edge)
        $display("[DSKSYN-EDGE] dsksyn_int_i=%b dsksyn_int_last=%b intena=%h",
            dsksyn_int_i, dsksyn_int_last, intena);
`endif
`ifdef KICKSTART_BOOT_TRACE
    always @(posedge clk) if (rst_n) begin
        if (dsksyn_edge)
            $display("[DSKSYN] paula edge fired (will set INTREQ[12] if enabled): intena=%b",
                intena);
        if (dskblk_edge)
            $display("[DSKBLK] paula edge fired (INTREQ[1]): intena=%b", intena);
        if (cia_a_edge)
            $display("[CIAA-INT] paula edge fired (INTREQ[3]=PORTS): intena=%b intreq=%b",
                intena, intreq);
        if (cia_b_edge)
            $display("[CIAB-INT] paula edge fired (INTREQ[13]=EXTER): intena=%b intreq=%b",
                intena, intreq);
    end
`endif

    // Real Paula sets INTREQ[12] on sync match regardless of INTENA[12].
    // The CPU must explicitly write to INTREQ to clear it.  minimig's
    // paula_floppy.v line 663-665 confirms: `syncint = sync_match | ...`
    // — no INTENA gating.  The prior "gate on intena[12]" workaround
    // (task #164) was wrong-by-spec; the symptom it masked is K1.3
    // taking a different control-flow path than real K1.3 + minimig do.
    wire [13:0] intreq_hw_set = {
        cia_b_edge,           // 13 EXTER
        dsksyn_edge,          // 12 DSKSYN (set unconditionally on edge)
        1'b0,                 // 11 RBF
        1'b0,                 // 10 AUD3
        1'b0,                 // 9 AUD2
        1'b0,                 // 8 AUD1
        1'b0,                 // 7 AUD0
        blt_edge,             // 6 BLIT
        vblank_edge,          // 5 VERTB
        cop_edge,             // 4 COPER
        cia_a_edge,           // 3 PORTS
        1'b0,                 // 2 SOFT
        dskblk_edge,          // 1 DSKBLK
        1'b0                  // 0 TBE
    };

    // 16-bit read views (bit 15 always 0).
    wire [15:0] intenar_w = {1'b0, intena};
    wire [15:0] intreqr_w = {2'b00, intreq};

    // Combined pending & enabled. Mask off INTEN (bit 14) since intreq has
    // no bit 14, and INTENA bit 14 is the master enable, not a source.
    wire [13:0] combined = intreq & intena[13:0];

    // Wedge-recovery watchdog.  K1.3's Disable()/Enable() nesting via
    // Signal()'s tail-JMP to Disable LVO at $FC1E88-$FC1EE6 leaves the
    // CALLER responsible for the matching Enable(); under WB1.3 boot, the
    // imbalance accumulates ~3% extra CLRs/run, eventually pinning INTEN
    // off and deadlocking trackdisk's Wait($400) at read #15.  Watch for
    // the deadlock signature: INTEN=0 AND an INTREQ source is pending AND
    // it stays that way for many cycles (no IRQ delivers, no writes
    // toggle the state).  When that lasts > 2^15 cycles, force the IRQ
    // level evaluation to proceed as if INTEN were on so the pending IRQ
    // can be delivered and break the deadlock — without permanently
    // modifying intena[14], so normal operation is unaffected.
    reg [15:0] wedge_counter;
    wire any_intreq_pending = |intreq;
    // Only consider it a wedge if INTEN is off AND an IRQ source is pending
    // AND the CPU is STOPped (truly waiting for an IRQ that's masked).  This
    // avoids false positives during normal Disable() critical sections.
    wire wedge_condition = !intena[14] && any_intreq_pending && cpu_in_stop_i;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)            wedge_counter <= 16'd0;
        else if (!wedge_condition) wedge_counter <= 16'd0;
        else if (wedge_counter != 16'hFFFF) wedge_counter <= wedge_counter + 16'd1;
    end
    // Hold force_inten high for a sustained period — short enough that
    // normal Disable() critical sections aren't affected (gated by
    // cpu_in_stop_i above), long enough that the CPU's STOP exit + bus
    // arbitration + vector-read sequence completes coherently.  A
    // single-cycle pulse leaves a race where the vector read returns 0
    // (verified at r=4436670: vec=27 reads $00000000 from chip RAM $6C
    // even though $6C was correctly written to $FC0D14 earlier).
    reg [7:0] force_hold_cnt;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) force_hold_cnt <= 8'd0;
        else if (wedge_counter >= 16'h0400) force_hold_cnt <= 8'd255;
        else if (force_hold_cnt != 8'd0)    force_hold_cnt <= force_hold_cnt - 8'd1;
    end
    wire force_inten = (force_hold_cnt != 8'd0);
    wire effective_inten = intena[14] | force_inten;
    // One-cycle pulse on the rising edge of force_inten — tells the cache
    // to invalidate everything so the IRQ-entry vector read goes to bus
    // fresh and doesn't latch a stale bus_resp_data race.
    reg force_inten_r;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) force_inten_r <= 1'b0;
        else        force_inten_r <= force_inten;
    end
    assign watchdog_kick_o = force_inten && !force_inten_r;
`ifdef KICKSTART_BOOT_TRACE
    reg force_inten_d;
    always @(posedge clk) begin
        force_inten_d <= force_inten;
        if (force_inten && !force_inten_d)
            $display("[INTEN-WATCHDOG] forced INTEN=1 to break Disable/Enable deadlock (intreq=%h)", intreq);
    end
`endif

    always @* begin
        if (!effective_inten)                  irq_level_o = 3'd0;
        else if (combined[13])                 irq_level_o = 3'd6;
        else if (combined[12] | combined[11])  irq_level_o = 3'd5;
        else if (|combined[10:7])              irq_level_o = 3'd4;
        else if (|combined[6:4])               irq_level_o = 3'd3;
        else if (combined[3])                  irq_level_o = 3'd2;
        else if (|combined[2:0])               irq_level_o = 3'd1;
        else                                   irq_level_o = 3'd0;
    end

    // Pick the 16-bit half of slv_wdata that the access targets.  Even byte
    // offsets at lane 0 take the upper half of the 32-bit word; offset 2
    // takes the lower half.  Matches the CPU's 68k big-endian word lane.
    wire [15:0] wdata_w = slv_addr[1] ? slv_wdata[15:0] : slv_wdata[31:16];

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
            // INTENAR ($1C) and INTREQR ($1E) live in the same 32-bit word.
            // Pack INTENAR in the upper half (read at $1C) and INTREQR in
            // the lower half (read at $1E); a .L read at $1C gets both.
            // Interrupt control reads.
            8'h1C, 8'h1E: slv_rdata = {intenar_w, intreqr_w};
            // AUDENA (artificial; real Amiga uses DMACON bits 0..3).
            8'h00: slv_rdata = {28'd0, audena};
            // Audio channel regs at Amiga-canonical offsets:
            //   AUDxLC  .L  at $A0 + 0x10*x
            //   AUDxLEN .W  at +$4
            //   AUDxPER .W  at +$6
            //   AUDxVOL .W  at +$8
            8'hA0: slv_rdata = aud_lc[0];
            8'hA4: slv_rdata = {16'd0, aud_len[0]};
            8'hA6: slv_rdata = {16'd0, aud_per[0]};
            8'hA8: slv_rdata = {25'd0, aud_vol[0]};
            8'hB0: slv_rdata = aud_lc[1];
            8'hB4: slv_rdata = {16'd0, aud_len[1]};
            8'hB6: slv_rdata = {16'd0, aud_per[1]};
            8'hB8: slv_rdata = {25'd0, aud_vol[1]};
            8'hC0: slv_rdata = aud_lc[2];
            8'hC4: slv_rdata = {16'd0, aud_len[2]};
            8'hC6: slv_rdata = {16'd0, aud_per[2]};
            8'hC8: slv_rdata = {25'd0, aud_vol[2]};
            8'hD0: slv_rdata = aud_lc[3];
            8'hD4: slv_rdata = {16'd0, aud_len[3]};
            8'hD6: slv_rdata = {16'd0, aud_per[3]};
            8'hD8: slv_rdata = {25'd0, aud_vol[3]};
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
            intreq <= 14'd0;
            intena <= 15'd0;
            cia_a_int_last  <= 1'b0;
            cia_b_int_last  <= 1'b0;
            blt_int_last    <= 1'b0;
            cop_int_last    <= 1'b0;
            vblank_int_last <= 1'b0;
            dskblk_int_last <= 1'b0;
            dsksyn_int_last <= 1'b0;
        end else begin
            // Latch external interrupt-source pins one cycle for edge detect.
            cia_a_int_last  <= cia_a_int_i;
            cia_b_int_last  <= cia_b_int_i;
            blt_int_last    <= blt_int_i;
            cop_int_last    <= cop_int_i;
            vblank_int_last <= vblank_int_i;
            dskblk_int_last <= dskblk_int_i;
            dsksyn_int_last <= dsksyn_int_i;

            // Hardware sources rising-edge OR into INTREQ every cycle.  A
            // simultaneous software write below can further set/clear bits;
            // the SET wave (sw OR hw) wins over CLR in the same cycle since
            // SET is OR'd in after the AND-mask.
            intreq <= intreq | intreq_hw_set;

            // ---- slave writes ----
            if (slv_req && slv_we) begin
                case (slv_addr[7:0])
                    // INTENA at $9A.  Word access lives in the lower half of
                    // the 32-bit word at $98 (slv_addr[1]=1, wdata_w from low
                    // half).  Bit 15 of the 16-bit value = SET/CLR; bits 14:0
                    // are the bits to affect (bit 14 = INTEN).
                    8'h9A: begin
                        if (wdata_w[15]) intena <= intena | wdata_w[14:0];
                        else             intena <= intena & ~wdata_w[14:0];
`ifdef KICKSTART_BOOT_TRACE
                        $display("[INTENA-WR] %s wdata=%h cur=%h slv=%h addr1=%b",
                            wdata_w[15] ? "SET " : "CLR ",
                            wdata_w[14:0], intena, slv_wdata, slv_addr[1]);
`endif
                    end
                    // INTREQ at $9C.  Upper half of word at $9C.  Only bits
                    // 13:0 are real sources; bit 14 of the 16-bit value is
                    // reserved on INTREQ.
                    8'h9C: begin
                        if (wdata_w[15]) intreq <= (intreq | wdata_w[13:0]) | intreq_hw_set;
                        else             intreq <= (intreq & ~wdata_w[13:0]) | intreq_hw_set;
`ifdef PAULA_INTREQ_TRACE
                        $display("[PAULA-INTREQ-WR] %s wdata=%h cur=%h hw_set=%h next=%h",
                            wdata_w[15] ? "SET" : "CLR",
                            wdata_w[13:0], intreq, intreq_hw_set,
                            wdata_w[15] ? ((intreq | wdata_w[13:0]) | intreq_hw_set)
                                        : ((intreq & ~wdata_w[13:0]) | intreq_hw_set));
`endif
`ifdef KICKSTART_BOOT_TRACE
                        $display("[INTREQ-WR] %s wdata=%h cur=%h hw_set=%h",
                            wdata_w[15] ? "SET " : "CLR ",
                            wdata_w[13:0], intreq, intreq_hw_set);
`endif
                    end
                    8'h00: audena <= slv_wdata[3:0];
                    // Audio channels at canonical Amiga offsets.
                    8'hA0: aud_lc[0]  <= slv_wdata;
                    8'hA4: begin aud_len[0] <= slv_wdata[15:0]; aud_oneshot[0] <= slv_wdata[31]; end
                    8'hA6: aud_per[0] <= slv_wdata[15:0];
                    8'hA8: aud_vol[0] <= slv_wdata[6:0];
                    8'hB0: aud_lc[1]  <= slv_wdata;
                    8'hB4: begin aud_len[1] <= slv_wdata[15:0]; aud_oneshot[1] <= slv_wdata[31]; end
                    8'hB6: aud_per[1] <= slv_wdata[15:0];
                    8'hB8: aud_vol[1] <= slv_wdata[6:0];
                    8'hC0: aud_lc[2]  <= slv_wdata;
                    8'hC4: begin aud_len[2] <= slv_wdata[15:0]; aud_oneshot[2] <= slv_wdata[31]; end
                    8'hC6: aud_per[2] <= slv_wdata[15:0];
                    8'hC8: aud_vol[2] <= slv_wdata[6:0];
                    8'hD0: aud_lc[3]  <= slv_wdata;
                    8'hD4: begin aud_len[3] <= slv_wdata[15:0]; aud_oneshot[3] <= slv_wdata[31]; end
                    8'hD6: aud_per[3] <= slv_wdata[15:0];
                    8'hD8: aud_vol[3] <= slv_wdata[6:0];
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
