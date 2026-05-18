// Cross-validation top-level: wraps FX68K (cycle-accurate Verilog 68000
// from https://github.com/ijor/fx68k) with a flat 16 MiB memory model and
// the same halt-code-reporting protocol as our Verilator sim_main.cpp.
//
// FX68K's external bus is the real 68000 asynchronous protocol (ASn/UDSn/
// LDSn/DTACKn/etc.).  We assert DTACKn one phi cycle after ASn drops to
// model an instantaneous-response memory.  STOP detection: track the last
// two program-space fetches; when FX68K asserts oHALTEDn (the CPU stopped),
// the most recent program fetch IS the STOP immediate.

module fx68k_top #(
    parameter MEM_HEXFILE = "program.hex"
)(
    input  wire        clk,
    input  wire        rst_n,
    output reg         halted,
    output reg [15:0]  halt_code,
    output reg [31:0]  cycle_count,
    output reg [31:0]  prog_fetches,
    output reg [31:0]  bus_cycles,
    output reg         priv_vio_seen,
    output wire        dbg_oHALTEDn,
    output wire        dbg_extReset,
    output wire [31:0] dbg_mem_at_400,
    output wire [31:0] dbg_last_eab,
    output wire [15:0] dbg_last_fetch
);
    // Defined further down once mem[] / last_fetch exist; assign here once
    // visible.
    // -----------------------------------------------------------------
    // Memory: 16 MiB, byte-addressed, big-endian.  Hex file format is
    // identical to what `tb/asm68k.py` already emits (32-bit MSB-first
    // words, one per line, packed into a 32-bit-word array).
    // -----------------------------------------------------------------
    localparam MEM_WORDS = 32'h0040_0000;   // 16 MiB / 4 = 4 Mwords
    reg [31:0] mem [0:MEM_WORDS-1];
    integer mi;
    initial begin
        for (mi = 0; mi < MEM_WORDS; mi = mi + 1) mem[mi] = 32'd0;
        $readmemh(MEM_HEXFILE, mem);
        // Inject reset SSP at $0 and reset PC at $4 to match our Verilator
        // harness (reset_a7 = $4000, RESET_PC = $400).  Our hex files
        // don't write these because our hardware skips the vector fetch.
        mem[0] = 32'h0000_4000;
        mem[1] = 32'h0000_0400;
    end

    // -----------------------------------------------------------------
    // Phi clock-enable generation: one full FX68K bus cycle takes two
    // clk edges (PHI1 then PHI2).  We alternate enPhi1/enPhi2 every
    // clk so the synchronous-internal model runs at half the host clock.
    // -----------------------------------------------------------------
    reg phi_state;
    wire enPhi1 = ~phi_state;
    wire enPhi2 =  phi_state;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) phi_state <= 1'b0;
        else        phi_state <= ~phi_state;
    end

    // -----------------------------------------------------------------
    // Reset & pwrUp staging.  FX68K wants pwrUp held for at least one
    // edge alongside extReset; release pwrUp first, then extReset.
    // -----------------------------------------------------------------
    // Hold pwrUp for ~32 host clocks (= 16 phi cycles) and extReset for
    // ~256 clocks so the microcode and bus interface fully settle before
    // we start servicing bus accesses.
    reg [9:0] rst_ctr;
    wire pwrUp_w    = (rst_ctr < 10'd32);
    wire extReset_w = (rst_ctr < 10'd256);
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) rst_ctr <= 10'd0;
        else if (rst_ctr < 10'd1023) rst_ctr <= rst_ctr + 10'd1;
    end

    // -----------------------------------------------------------------
    // FX68K instantiation.
    // -----------------------------------------------------------------
    wire        eRWn, ASn, LDSn, UDSn;
    wire        E_unused, VMAn_unused;
    wire        FC0, FC1, FC2;
    wire        BGn_unused, oRESETn, oHALTEDn;
    assign dbg_oHALTEDn = oHALTEDn;
    assign dbg_extReset = extReset_w;
    // Reach into mem[] from a stable index for inspection.
    assign dbg_mem_at_400 = mem[32'h100];
    reg [31:0] last_eab_r;
    always @(posedge clk) if (!ASn && ASn_q) last_eab_r <= {eab, 1'b0};
    assign dbg_last_eab = last_eab_r;
    assign dbg_last_fetch = last_fetch;
    wire [23:1] eab;
    wire [15:0] oEdb;
    reg  [15:0] iEdb_r;
    reg         DTACKn_r;

    fx68k u_cpu (
        .clk        (clk),
        .HALTn      (1'b1),
        .extReset   (extReset_w),
        .pwrUp      (pwrUp_w),
        .enPhi1     (enPhi1),
        .enPhi2     (enPhi2),
        .eRWn       (eRWn),
        .ASn        (ASn),
        .LDSn       (LDSn),
        .UDSn       (UDSn),
        .E          (E_unused),
        .VMAn       (VMAn_unused),
        .FC0        (FC0),
        .FC1        (FC1),
        .FC2        (FC2),
        .BGn        (BGn_unused),
        .oRESETn    (oRESETn),
        .oHALTEDn   (oHALTEDn),
        .DTACKn     (DTACKn_r),
        .VPAn       (1'b1),
        .BERRn      (1'b1),
        .BRn        (1'b1),
        .BGACKn     (1'b1),
        .IPL0n      (1'b1),
        .IPL1n      (1'b1),
        .IPL2n      (1'b1),
        .iEdb       (iEdb_r),
        .oEdb       (oEdb),
        .eab        (eab)
    );

    // -----------------------------------------------------------------
    // Bus servicing.  When ASn is asserted (low), decode the access and:
    //   - For reads: place the requested data on iEdb, then assert DTACKn.
    //   - For writes: capture oEdb into memory, then assert DTACKn.
    // We assert DTACKn one clock after seeing ASn=0 to model a 1-wait
    // synchronous memory.
    // -----------------------------------------------------------------
    wire [23:0] full_addr = {eab, 1'b0};
    wire [21:0] word_idx  = full_addr[23:2];   // 32-bit word index
    wire        addr_odd  = full_addr[1];      // 1 -> word in low half

    // Read data view: pick the 16-bit half of the 32-bit memory word
    // selected by addr[1].
    wire [15:0] mem_word_hi = mem[word_idx][31:16];
    wire [15:0] mem_word_lo = mem[word_idx][15:0];
    wire [15:0] read_word   = addr_odd ? mem_word_lo : mem_word_hi;

    reg ASn_q;
    reg [15:0] last_fetch;
    reg [15:0] prev_fetch;
    // STOP opcode is 0x4E72 and is followed by a 16-bit immediate that
    // becomes the new SR.  We treat that immediate as the halt code,
    // matching our Verilator harness's convention.  Detecting via the
    // opcode pattern (rather than oHALTEDn) lets us also catch `stop #0`,
    // which FX68K decodes as a privilege violation because it clears the
    // S bit and our test harness still wants to report rc=0.
    reg stop_imm_pending;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ASn_q            <= 1'b1;
            DTACKn_r         <= 1'b1;
            iEdb_r           <= 16'd0;
            last_fetch       <= 16'd0;
            prev_fetch       <= 16'd0;
            halted           <= 1'b0;
            halt_code        <= 16'd0;
            cycle_count      <= 32'd0;
            prog_fetches     <= 32'd0;
            bus_cycles       <= 32'd0;
            stop_imm_pending <= 1'b0;
            priv_vio_seen    <= 1'b0;
        end else begin
            ASn_q <= ASn;
            cycle_count <= cycle_count + 32'd1;
            // Privilege violation handler vector lives at $20.  CPU
            // hitting that vector (in supervisor data or CPU space)
            // means our previous STOP just trapped (typical pattern
            // for `stop #0`: SR has S=0, so STOP immediately re-enters
            // supervisor mode via vec 8).
            if (!ASn && eRWn && ({eab, 1'b0} == 24'h000020 ||
                                 {eab, 1'b0} == 24'h000022))
                priv_vio_seen <= 1'b1;
            // Count any bus cycle completion (rising edge of ASn).
            if (ASn && !ASn_q) bus_cycles <= bus_cycles + 32'd1;
            // Drop DTACKn one cycle after UDSn or LDSn drops (whichever is
            // sooner -- a real 68000 only releases DTACK once the strobes
            // are valid).  Raise it when ASn returns high.
            if (!ASn && DTACKn_r && (!UDSn || !LDSn)) begin
                DTACKn_r <= 1'b0;
                if (eRWn) begin
                    // Read.  Driver supplies the requested half-word; the
                    // CPU only consumes bytes whose UDSn/LDSn is asserted.
                    iEdb_r <= read_word;
                    // Capture program-space reads here so the recorded
                    // word is the one we actually delivered (avoids an
                    // off-by-one with eab changing on T4 of the cycle).
                    if (FC1 && !FC0) begin
                        prev_fetch   <= last_fetch;
                        last_fetch   <= read_word;
                        prog_fetches <= prog_fetches + 32'd1;
                        // Don't set halted here -- FX68K does some
                        // speculative prefetching, so a 4E72 in the
                        // program stream may belong to a STOP that's
                        // never actually executed (e.g. when an
                        // earlier RTR redirects PC).  Instead capture
                        // the LATEST plausible STOP-imm candidate and
                        // let the C++ harness commit it when it sees
                        // the CPU genuinely halt (oHALTEDn) or idle
                        // (no new program fetches for many cycles).
                        if (stop_imm_pending) begin
                            halt_code        <= read_word;
                            stop_imm_pending <= 1'b0;
                        end else if (read_word == 16'h4E72) begin
                            stop_imm_pending <= 1'b1;
                        end
                    end
                end else begin
                    // Write.  UDSn enables upper byte (even address),
                    // LDSn enables lower byte (odd address) of the 16-bit
                    // word at eab.
                    if (!UDSn) begin
                        if (addr_odd)
                            mem[word_idx][15:8] <= oEdb[15:8];
                        else
                            mem[word_idx][31:24] <= oEdb[15:8];
                    end
                    if (!LDSn) begin
                        if (addr_odd)
                            mem[word_idx][7:0] <= oEdb[7:0];
                        else
                            mem[word_idx][23:16] <= oEdb[7:0];
                    end
                end
            end
            if (ASn) DTACKn_r <= 1'b1;
        end
    end

endmodule
