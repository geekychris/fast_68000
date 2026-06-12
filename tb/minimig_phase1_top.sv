// Minimig fullsys Phase 1: instantiate REAL minimig.v with our
// m68k_core driving its CPU bus.
//
// Phase 1a goal: get the integration to compile + run with all
// minimig sub-modules pulled in.  Boot may not progress past Phase
// 0.5's chipset-stub limit, but compiling proves the port-mapping
// surface is correct.
//
// All "uninteresting" minimig ports (PS2, SPI, video, audio, joystick,
// OSD) are tied off to safe defaults.  We drive only what's needed
// for K1.3 to execute: CPU bus, SRAM bus (for chip RAM + ROM), clocks,
// reset.

module minimig_phase1_top (
    input  wire        clk,            // ~28 MHz (we drive 1:1 for now)
    input  wire        rst_n,
    output wire [31:0] cur_pc,
    output wire [31:0] retired,
    output wire        halted,
    output wire [15:0] halt_code,
    output wire        cpu_in_stop,
    input  wire        dump_strobe  // 0→1 edge: write sram to phase1_sram.hex
);

    parameter MEM_HEXFILE_ROM = "rom.hex";

    // ---------------- CPU ----------------
    wire        ic_req, ic_we, ic_lock;
    wire [31:0] ic_addr, ic_wdata;
    wire [3:0]  ic_be;
    wire        ic_ack;
    wire [31:0] ic_rdata;

    wire        dc_req, dc_we, dc_lock;
    wire [31:0] dc_addr, dc_wdata;
    wire [3:0]  dc_be;
    wire        dc_is_long;
    wire        dc_ack;
    wire [31:0] dc_rdata;

    // Convert minimig's [2:0]_cpu_ipl (active low) to our [2:0] ipl_i
    // (active high).
    wire [2:0] cpu_ipl_n;
    wire [2:0] ipl_i_eff = ~cpu_ipl_n;

    m68k_core u_cpu (
        .clk           (clk),
        .rst_n         (rst_n),
        .reset_a7      (32'h0000_4000),
        .ipl_i         (ipl_i_eff),
        .ic_req        (ic_req),
        .ic_we         (ic_we),
        .ic_lock       (ic_lock),
        .ic_addr       (ic_addr),
        .ic_wdata      (ic_wdata),
        .ic_be         (ic_be),
        .ic_ack        (ic_ack),
        .ic_rdata      (ic_rdata),
        .dc_req        (dc_req),
        .dc_we         (dc_we),
        .dc_lock       (dc_lock),
        .dc_addr       (dc_addr),
        .dc_wdata      (dc_wdata),
        .dc_be         (dc_be),
        .dc_is_long    (dc_is_long),
        .dc_ack        (dc_ack),
        .dc_rdata      (dc_rdata),
        .halted        (halted),
        .halt_code     (halt_code),
        .retired       (retired),
        .cpu_in_stop   (cpu_in_stop),
        .cur_pc        (cur_pc),
        .dbg_regs_flat (),
        .dbg_sr        ()
    );

    // ---------------- Bus adapter (CPU → 68000-style) ----------------
    wire [23:1] cpu_address;
    wire [15:0] cpudata_in;     // CPU → minimig
    wire [15:0] cpu_data;       // minimig → CPU
    wire        cpu_as_n;
    wire        cpu_uds_n;
    wire        cpu_lds_n;
    wire        cpu_rw;
    wire        cpu_dtack_n;

    m68k_to_amiga_bus u_bus_adapter (
        .clk        (clk),
        .rst_n      (rst_n),
        .ic_req     (ic_req),
        .ic_addr    (ic_addr),
        .ic_be      (ic_be),
        .ic_ack     (ic_ack),
        .ic_rdata   (ic_rdata),
        .dc_req     (dc_req),
        .dc_we      (dc_we),
        .dc_addr    (dc_addr),
        .dc_wdata   (dc_wdata),
        .dc_be      (dc_be),
        .dc_is_long (dc_is_long),
        .dc_ack     (dc_ack),
        .dc_rdata   (dc_rdata),
        .addr_o     (cpu_address),
        .data_o     (cpudata_in),
        .data_i     (cpu_data),
        .as_n       (cpu_as_n),
        .uds_n      (cpu_uds_n),
        .lds_n      (cpu_lds_n),
        .rw         (cpu_rw),
        .dtack_n    (cpu_dtack_n)
    );

    // ---------------- Clock enables (mirrors external/minimig/rtl/clock/amiga_clk.v) -----
    // Real Amiga: 28.37516 MHz master, 7 MHz CPU, c1/c3 quadrature phases.
    // We treat our `clk` as the 28 MHz clock; clk7_en pulses every 4
    // cycles (28/4 = 7 MHz), clk7n_en is 180° out of phase.
    reg [1:0] clk7_cnt;
    reg       clk7_en_r, clk7n_en_r;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            clk7_cnt    <= 2'b10;
            clk7_en_r   <= 1'b1;
            clk7n_en_r  <= 1'b1;
        end else begin
            clk7_cnt    <= clk7_cnt + 2'b01;
            clk7_en_r   <= (clk7_cnt == 2'b00);
            clk7n_en_r  <= (clk7_cnt == 2'b10);
        end
    end
    wire clk7_en   = clk7_en_r;
    wire clk7n_en  = clk7n_en_r;

    // c3 = clk_7 delayed 1 cycle.  c1 = ~c3 delayed 1 cycle.
    wire clk_7 = clk7_cnt[1];
    reg  c3_r, c1_r;
    always @(posedge clk) begin
        c3_r <= clk_7;
        c1_r <= ~c3_r;
    end
    wire c1 = c1_r;
    wire c3 = c3_r;

    // cck: 3.54 MHz (~half of 7 MHz).  Toggled every clk7_en pulse.
    reg cck_r;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)            cck_r <= 1'b0;
        else if (clk7_en_r)    cck_r <= ~cck_r;
    end
    wire cck = cck_r;

    // eclk: 1/10 of CLK (ECLK enable pattern from amiga_clk.v).
    // Cycles through 10 states with one position hot.
    reg [9:0] eclk_r;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)         eclk_r <= 10'b1000000000;
        else if (clk7_en_r) eclk_r <= {eclk_r[8:0], eclk_r[9]};
    end
    wire [9:0] eclk = eclk_r;

    // ---------------- SRAM model ----------------
    // minimig drives [21:1] ram_address.  A 2 MiB address space, with
    // chip RAM in low region and ROM mapped near the top by minimig's
    // bank mapper.  Phase 1a just gives it 2 MiB of flat memory and
    // pre-loads ROM at the top quadrant where the bank mapper expects it.
    wire [15:0] ram_data;        // minimig → SRAM
    wire [15:0] ramdata_in;      // SRAM → minimig
    wire [21:1] ram_address;
    wire        ram_bhe_n, ram_ble_n, ram_we_n, ram_oe_n;

    reg [15:0] sram [0:2097151];  // 2 MiB / 2 = 1Mi words
    initial begin
        if (MEM_HEXFILE_ROM != "")
            $readmemh(MEM_HEXFILE_ROM, sram, 21'h180000);  // load ROM at $300000-$37FFFF (word addr)
    end

    // SRAM read/write logic.
    reg [15:0] sram_rdata_q;
    always @(posedge clk) begin
        if (!ram_oe_n && ram_we_n) begin
            sram_rdata_q <= sram[ram_address];
        end
        if (!ram_we_n) begin
            if (!ram_bhe_n) sram[ram_address][15:8] <= ram_data[15:8];
            if (!ram_ble_n) sram[ram_address][7:0]  <= ram_data[7:0];
        end
    end
    assign ramdata_in = sram_rdata_q;

    // ---------------- minimig.v ----------------
    minimig u_minimig (
        // m68k pins
        .cpu_address    (cpu_address),
        .cpu_data       (cpu_data),
        .cpudata_in     (cpudata_in),
        ._cpu_ipl       (cpu_ipl_n),
        ._cpu_as        (cpu_as_n),
        ._cpu_uds       (cpu_uds_n),
        ._cpu_lds       (cpu_lds_n),
        .cpu_r_w        (cpu_rw),
        ._cpu_dtack     (cpu_dtack_n),
        ._cpu_reset     (),
        ._cpu_reset_in  (rst_n),
        .cpu_vbr        (32'd0),
        .ovr            (),

        // sram pins
        .ram_data       (ram_data),
        .ramdata_in     (ramdata_in),
        .ram_address    (ram_address),
        ._ram_bhe       (ram_bhe_n),
        ._ram_ble       (ram_ble_n),
        ._ram_we        (ram_we_n),
        ._ram_oe        (ram_oe_n),
        .chip48         (48'd0),

        // system pins
        .rst_ext        (~rst_n),
        .rst_out        (),
        .clk            (clk),
        .clk7_en        (clk7_en),
        .clk7n_en       (clk7n_en),
        .c1             (c1),
        .c3             (c3),
        .cck            (cck),
        .eclk           (eclk),

        // rs232 — tie off
        .rxd            (1'b1),
        .txd            (),
        .cts            (1'b1),
        .rts            (),

        // I/O — tie off
        ._joy1          (8'hFF),
        ._joy2          (8'hFF),
        .mouse_btn1     (1'b1),
        .mouse_btn2     (1'b1),
        .mouse_btn      (3'b111),
        .kbd_mouse_strobe (1'b0),
        .kms_level      (1'b0),
        .kbd_mouse_type (2'b00),
        .kbd_mouse_data (8'h00),
        ._15khz         (1'b1),
        .pwrled         (),
        .msdat          (),
        .msclk          (),
        .kbddat         (),
        .kbdclk         (),

        // SPI — tie off (no disk for Phase 1a)
        ._scs           (3'b111),
        .direct_sdi     (1'b0),
        .sdi            (1'b0),
        .sdo            (),
        .sck            (1'b0),

        // video — unused outputs
        ._hsync         (),
        ._vsync         (),
        .red            (),
        .green          (),
        .blue           (),

        // audio — unused outputs
        .left           (),
        .right          (),
        .ldata          (),
        .rdata          (),

        // user i/o — unused outputs
        .cpu_config     (),
        .memcfg         (),
        .turbochipram   (),
        .turbokick      (),
        .init_b         (),
        .fifo_full      (),
        .trackdisp      (),
        .secdisp        (),
        .floppy_fwr     (),
        .floppy_frd     (),
        .hd_fwr         (),
        .hd_frd         ()
    );

    // Per-cycle probe for bus activity — first few AS-falling events.
    // Enable with +define+PHASE1_PROBE.  Diagnostic: confirms CPU does
    // assert AS at $FC00D2 exactly once, then waits forever because
    // minimig never asserts _cpu_dtack (= _ta_n).  Next diagnostic
    // pass needs to bisect the dtack chain — likely syscontrol holding
    // sram_bridge in reset, or _ta_n gated by an unprogrammed setting.
`ifdef PHASE1_PROBE
    reg [31:0] probe_count;
    reg [31:0] dtack_count;
    reg cpu_as_n_d;
    reg cpu_dtack_n_d;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            probe_count <= 0;
            dtack_count <= 0;
            cpu_as_n_d  <= 1'b1;
            cpu_dtack_n_d <= 1'b1;
        end else begin
            cpu_as_n_d <= cpu_as_n;
            cpu_dtack_n_d <= cpu_dtack_n;
            if (cpu_as_n_d && !cpu_as_n && probe_count < 20) begin
                $display("[probe] AS falling: addr=%h rw=%b uds=%b lds=%b cpudata_in=%h",
                    {cpu_address, 1'b0}, cpu_rw, ~cpu_uds_n, ~cpu_lds_n, cpudata_in);
                probe_count <= probe_count + 1;
            end
            if (cpu_dtack_n_d && !cpu_dtack_n && dtack_count < 20) begin
                $display("[probe] DTACK falling at cycle (addr=%h rdata=%h)",
                    {cpu_address, 1'b0}, cpu_data);
                dtack_count <= dtack_count + 1;
            end
            // Phase 1b: when CPU AS is low at $FC00D2 (or any addr in
            // the ROM range $F8_0000..$FF_FFFF), capture what minimig
            // outputs on its SRAM bridge.  This tells us the SRAM
            // offset minimig actually generates so we know where to
            // load ROM in our test SRAM.
            if (!cpu_as_n && probe_count < 10 &&
                {cpu_address, 1'b0} >= 24'hF8_0000) begin
                $display("[probe-sram] cpu=%h -> ram_addr_word=%h (byte=%h) oe=%b we=%b bhe=%b ble=%b",
                    {cpu_address, 1'b0}, ram_address,
                    {ram_address, 1'b0},
                    ~ram_oe_n, ~ram_we_n, ~ram_bhe_n, ~ram_ble_n);
            end
        end
    end
    // Phase 1b deeper trace: dump the m68k_bridge enable-chain signals
    // every cycle for the first 200 cycles after reset to see which
    // gate is stuck.
    reg [9:0] sig_cnt;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) sig_cnt <= 10'd0;
        else if (sig_cnt < 10'd200) begin
            sig_cnt <= sig_cnt + 10'd1;
            $display("[sig %0d] _as=%b _dtack=%b cck=%b clk7_en=%b clk_7=%b c1=%b c3=%b cpu_addr=%h",
                sig_cnt, cpu_as_n, cpu_dtack_n, cck, clk7_en, clk_7, c1, c3,
                {cpu_address, 1'b0});
        end
    end
    // Phase 1b deepest trace: hierarchical access into u_minimig internal
    // signals.  Shows the bridge enable chain + downstream cpu_rd/bank,
    // so we can identify which gate is wrong.  Note: u_minimig.CPU1 is
    // the m68k_bridge instance (per minimig.v:770); BMAP1 is the
    // bank mapper.
    always @(posedge clk or negedge rst_n) begin
        if (rst_n && sig_cnt < 10'd60) begin
            $display("[deep %0d] cpuhlt=%b cpurst=%b dbr=%b dbs=%b nrdy=%b l_as=%b l_dtack=%b enable=%b _ta_n=%b cpu_rd=%b sel_kick=%b bank=%h",
                sig_cnt,
                u_minimig.cpuhlt,
                u_minimig.cpurst,
                u_minimig.dbr,
                u_minimig.dbs,
                u_minimig.gayle_nrdy,
                u_minimig.CPU1.l_as,
                u_minimig.CPU1.l_dtack,
                u_minimig.CPU1.enable,
                u_minimig.CPU1._ta_n,
                u_minimig.cpu_rd,
                u_minimig.sel_kick,
                u_minimig.bank);
        end
    end
`endif

    wire _unused = &{ic_we, ic_lock, ic_wdata, dc_lock, 1'b0};

    // On rising edge of dump_strobe write the SRAM contents (in
    // 16-bit-per-line $writememh format) to phase1_sram.hex.  Driven
    // from the C++ harness at end-of-run for chip-RAM diff vs our
    // m68k_top stack.  sram[0..262143] covers the 512 KiB chip RAM
    // range (word offsets 0..$3FFFF).  Slow RAM at SRAM offset $80000
    // covers the next 512 KiB.
    reg dump_strobe_d;
    always @(posedge clk) begin
        dump_strobe_d <= dump_strobe;
        if (dump_strobe && !dump_strobe_d) begin
            $writememh("phase1_sram.hex", sram, 21'h00000, 21'h17FFFF);
            $display("[phase1] dumped sram to phase1_sram.hex (1.5 MiB / 768K words)");
        end
    end

    // Phase 1b unblocker: minimig's user_io keeps cpuhlt=1 and
    // cpurst=1 by default until the OSD/host SPI bus releases them.
    // Our test harness has no SPI driver, so they stay asserted
    // forever — preventing the m68k_bridge from ever latching the
    // CPU's AS strobe and thus DTACK never asserts.
    // Force them low to let the CPU run.  This is intrusive but
    // semantically correct: real-hardware minimig releases cpuhlt
    // shortly after FPGA configuration; we're simulating that
    // already-released state from t=0.
    initial begin
        force u_minimig.cpuhlt = 1'b0;
        force u_minimig.cpurst = 1'b0;
    end

endmodule
