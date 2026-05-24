`include "m68k_defs.vh"

// Multi-core top-level. Wires N_CORES cores, each with its own I and D
// cache, to a shared bus + memory. Bus ports are numbered 2*i (I-cache)
// and 2*i+1 (D-cache) for core i.
module m68k_top #(
    parameter N_CORES    = 2,
    parameter USE_CACHE  = 1,           // 1 = real L1 caches, 0 = bus passthrough
    parameter MEM_WORDS  = 16384,
    parameter FB_W       = 256,         // chunky framebuffer width (256 lores, 512 hires)
    parameter MEM_HEXFILE = "program.hex",
    parameter ROM_WORDS  = 1,            // size of the $F80000+ ROM in 32-bit words
    parameter MEM_HEXFILE_ROM = "",      // optional Kickstart-style ROM hex
    parameter OVL_RESET  = 1'b0,         // 1 -> CIA-A /OVL active at reset (Amiga-faithful)
    parameter DISK_WORDS = 1,            // memory-backed disk image at $00400000
    parameter DISK_HEXFILE = ""          // optional disk image hex (one .adf etc.)
)(
    input  wire clk,
    input  wire rst_n,
    output wire [N_CORES-1:0]    halted,
    output wire [16*N_CORES-1:0] halt_code_flat,
    output wire [32*N_CORES-1:0] retired_flat,

    // Framebuffer peek for the simulator harness. Drive fb_peek_addr with a
    // word-aligned byte address; fb_peek_data returns the 32-bit word.
    input  wire [31:0] fb_peek_addr,
    output wire [31:0] fb_peek_data,

    // Live Paula audio output for the simulator harness.
    output wire signed [15:0] audio_l,
    output wire signed [15:0] audio_r
);
    // Two ports per core (I-cache + D-cache) plus one each for the blitter,
    // Copper, Denise, and Paula masters.
    localparam N_PORTS  = 2 * N_CORES + 4;
    localparam BLT_PORT = 2 * N_CORES;       // blitter master
    localparam COP_PORT = 2 * N_CORES + 1;   // Copper master
    localparam DEN_PORT = 2 * N_CORES + 2;   // Denise master
    localparam PAU_PORT = 2 * N_CORES + 3;   // Paula master
    localparam PID_BITS = $clog2(N_PORTS > 1 ? N_PORTS : 2);

    // Per-core cache <-> bus.
    wire [N_PORTS-1:0]    p_req;
    wire [N_PORTS-1:0]    p_we;
    wire [N_PORTS-1:0]    p_lock;
    wire [32*N_PORTS-1:0] p_addr;
    wire [32*N_PORTS-1:0] p_wdata;
    wire [4*N_PORTS-1:0]  p_be;
    wire [N_PORTS-1:0]    p_is_long;
    wire [N_PORTS-1:0]    p_grant;     // unused by caches but routed for completeness
    wire [N_PORTS-1:0]    p_resp_valid;
    wire [31:0]           p_resp_data;

    wire                  snoop_valid;
    wire [31:0]           snoop_addr;
    wire [PID_BITS-1:0]   snoop_src_id;
    // The bus drives a memory-mapped IRQ register at $FFFF_FFFC (legacy
    // test-bench interface used by t15_irq).  Paula's INTREQ/INTENA priority
    // chain drives `paula_irq_level`.  The CPU should see the higher of the
    // two.
    wire [2:0]            bus_irq_level;
    wire [2:0]            irq_level = (bus_irq_level > paula_irq_level)
                                      ? bus_irq_level : paula_irq_level;

    // Blitter slave wires.
    wire        blt_slv_req;
    wire        blt_slv_we;
    wire [5:0]  blt_slv_addr;
    wire [3:0]  blt_slv_be;
    wire [31:0] blt_slv_wdata;
    wire [31:0] blt_slv_rdata;

    // Copper slave wires.
    wire        cop_slv_req;
    wire        cop_slv_we;
    wire [5:0]  cop_slv_addr;
    wire [3:0]  cop_slv_be;
    wire [31:0] cop_slv_wdata;
    wire [31:0] cop_slv_rdata;

    // Denise slave wires.
    wire        den_slv_req;
    wire        den_slv_we;
    wire [8:0]  den_slv_addr;
    wire [3:0]  den_slv_be;
    wire [31:0] den_slv_wdata;
    wire [31:0] den_slv_rdata;

    // Paula slave wires.
    wire        pau_slv_req;
    wire        pau_slv_we;
    wire [7:0]  pau_slv_addr;
    wire [3:0]  pau_slv_be;
    wire [31:0] pau_slv_wdata;
    wire [31:0] pau_slv_rdata;

    // CIA-A / CIA-B slave wires (8-bit registers).
    wire        cia_a_slv_req;
    wire        cia_a_slv_we;
    wire [3:0]  cia_a_slv_addr;
    wire [7:0]  cia_a_slv_wdata;
    wire [7:0]  cia_a_slv_rdata;
    wire        cia_b_slv_req;
    wire        cia_b_slv_we;
    wire [3:0]  cia_b_slv_addr;
    wire [7:0]  cia_b_slv_wdata;
    wire [7:0]  cia_b_slv_rdata;
    wire        cia_a_int;
    wire        cia_b_int;
    // CIA-A port A drives /OVL on bit 0 (active low).  The bus clears its
    // OVL latch when CIA-A actively drives bit 0 low.
    wire [7:0]  cia_a_pa_out;
    wire [7:0]  cia_a_pa_oe;
    wire        ovl_clr = cia_a_pa_oe[0] & ~cia_a_pa_out[0];
    // Keyboard byte injection.  Bus exposes a write port at $00FE9000
    // that pulses cia_a_kbd_wr for one cycle with the written byte.
    wire        cia_a_kbd_wr;
    wire [7:0]  cia_a_kbd_byte;

    // Blitter busy signal exposed to the Copper (for WAIT).
    wire        blt_busy;
    wire        blt_bzero;
    // One-cycle pulse on each blit completion.  Not currently routed
    // into an interrupt controller, but exposed so test/demo code or a
    // future Paula-style int controller can observe it.
    wire        blt_int;

    // Forward-declare floppy-drive state inputs to the bus.  These are
    // driven by the CIA-B step-pulse-tracking block further down — putting
    // the wire declarations here lets us use them in the m68k_bus
    // instantiation that follows.
    wire [7:0]  disk_track_idx;

    // Bitplane shadow + auto-active wires from the bus into Denise.
    // m68k_bus snoops canonical BPLnPT and BPL1/2MOD chipset writes and
    // exposes them here for Denise's auto-rasterisation on VBL.
    wire [31:0] bus_bpl_pt0;
    wire [31:0] bus_bpl_pt1;
    wire [31:0] bus_bpl_pt2;
    wire [31:0] bus_bpl_pt3;
    wire [31:0] bus_bpl_pt4;
    wire [31:0] bus_bpl_pt5;
    wire [15:0] bus_bpl_mod1;
    wire [15:0] bus_bpl_mod2;
    wire        bus_den_auto_active;
    wire [6*32-1:0] den_auto_bpl_pt_flat = {bus_bpl_pt5, bus_bpl_pt4,
                                            bus_bpl_pt3, bus_bpl_pt2,
                                            bus_bpl_pt1, bus_bpl_pt0};

    m68k_bus #(
        .N_PORTS        (N_PORTS),
        .MEM_WORDS      (MEM_WORDS),
        .MEM_HEXFILE    (MEM_HEXFILE),
        .ROM_WORDS      (ROM_WORDS),
        .MEM_HEXFILE_ROM(MEM_HEXFILE_ROM),
        .OVL_RESET      (OVL_RESET),
        .DISK_WORDS     (DISK_WORDS),
        .DISK_HEXFILE   (DISK_HEXFILE)
    ) u_bus (
        .clk         (clk),
        .rst_n       (rst_n),
        .req         (p_req),
        .we          (p_we),
        .lock        (p_lock),
        .addr_flat   (p_addr),
        .wdata_flat  (p_wdata),
        .be_flat     (p_be),
        .is_long_flat(p_is_long),
        .grant       (p_grant),
        .resp_valid  (p_resp_valid),
        .resp_data   (p_resp_data),
        .snoop_valid (snoop_valid),
        .snoop_addr  (snoop_addr),
        .snoop_src_id(snoop_src_id),
        .irq_level   (bus_irq_level),
        .fb_peek_addr(fb_peek_addr),
        .fb_peek_data(fb_peek_data),
        .blt_slv_req (blt_slv_req),
        .blt_slv_we  (blt_slv_we),
        .blt_slv_addr(blt_slv_addr),
        .blt_slv_be  (blt_slv_be),
        .blt_slv_wdata(blt_slv_wdata),
        .blt_slv_rdata(blt_slv_rdata),
        .blt_busy_i  (blt_busy),
        .blt_bzero_i (blt_bzero),
        .cop_slv_req (cop_slv_req),
        .cop_slv_we  (cop_slv_we),
        .cop_slv_addr(cop_slv_addr),
        .cop_slv_be  (cop_slv_be),
        .cop_slv_wdata(cop_slv_wdata),
        .cop_slv_rdata(cop_slv_rdata),
        .den_slv_req (den_slv_req),
        .den_slv_we  (den_slv_we),
        .den_slv_addr(den_slv_addr),
        .den_slv_be  (den_slv_be),
        .den_slv_wdata(den_slv_wdata),
        .den_slv_rdata(den_slv_rdata),
        .pau_slv_req (pau_slv_req),
        .pau_slv_we  (pau_slv_we),
        .pau_slv_addr(pau_slv_addr),
        .pau_slv_be  (pau_slv_be),
        .pau_slv_wdata(pau_slv_wdata),
        .pau_slv_rdata(pau_slv_rdata),
        .cia_a_slv_req  (cia_a_slv_req),
        .cia_a_slv_we   (cia_a_slv_we),
        .cia_a_slv_addr (cia_a_slv_addr),
        .cia_a_slv_wdata(cia_a_slv_wdata),
        .cia_a_slv_rdata(cia_a_slv_rdata),
        .cia_b_slv_req  (cia_b_slv_req),
        .cia_b_slv_we   (cia_b_slv_we),
        .cia_b_slv_addr (cia_b_slv_addr),
        .cia_b_slv_wdata(cia_b_slv_wdata),
        .cia_b_slv_rdata(cia_b_slv_rdata),
        .ovl_clr_i      (ovl_clr),
        .kbd_inject_wr  (cia_a_kbd_wr),
        .kbd_inject_byte(cia_a_kbd_byte),
        .vblank_pulse_o (vblank_pulse),
        .dskblk_pulse_o (dskblk_pulse),
        .dsksyn_pulse_o (dsksyn_pulse),
        .ovl_clear_pulse_o (ovl_clear_pulse),
        .disk_track_i   (disk_track_idx),
        .bpl_pt0_o      (bus_bpl_pt0),
        .bpl_pt1_o      (bus_bpl_pt1),
        .bpl_pt2_o      (bus_bpl_pt2),
        .bpl_pt3_o      (bus_bpl_pt3),
        .bpl_pt4_o      (bus_bpl_pt4),
        .bpl_pt5_o      (bus_bpl_pt5),
        .bpl_mod1_o     (bus_bpl_mod1),
        .bpl_mod2_o     (bus_bpl_mod2),
        .den_auto_active_o (bus_den_auto_active)
    );

    // CIA-A and CIA-B.  Tick every bus cycle for now (10x real Amiga rate)
    // -- when we split into chip-clock vs CPU-clock domains we'll gate this.
    // CIA-A PRA inputs reflect floppy "disk present, ready, at track 0":
    //   bit 7: /FIRE  (joy/mouse button) -- not pressed -> 1
    //   bit 6: /SEL2  (extra select)     -- 1 (unused)
    //   bit 5: /DSKRDY  (active low)     -- 0 (drive ready / spinning)
    //   bit 4: /DSKTRACK0 (active low)   -- 0 (at track 0 cylinder)
    //   bit 3: /DSKWRPRO  (active low)   -- 1 (NOT write-protected -> high)
    //   bit 5: /DSKRDY  (driven low by drive once motor is up).  Now
    //                   driven by `fl_dskrdy_low` (gated on motor on
    //                   for ~16K cycles), declared below alongside
    //                   the CIA-B PB step detector.
    //   bit 2: /DSKCHANGE -- starts 0 (active, "disk just changed"
    //                   since boot), goes 1 after the first /STEP
    //                   pulse from CIA-B PB0, like real hw.
    //   bit 1: /OVL     -- driven by CIA, not input
    //   bit 0: /LED     -- driven by CIA, not input
    cia u_cia_a (
        .clk      (clk),
        .rst_n    (rst_n),
        .slv_req  (cia_a_slv_req),
        .slv_we   (cia_a_slv_we),
        .slv_addr (cia_a_slv_addr),
        .slv_wdata(cia_a_slv_wdata),
        .slv_rdata(cia_a_slv_rdata),
        .tick     (1'b1),
        .kbd_wr   (cia_a_kbd_wr),
        .kbd_byte (cia_a_kbd_byte),
        // PRA input bit composition (active-low signals):
        //   [7]=FIRE=1, [6]=SEL2=1, [5]=DSKRDY (motor-gated, low when ready),
        //   [4]=DSKTRACK0 (low when cur_cyl == 0),
        //   [3]=DSKWRPRO=1 (write-protect inactive),
        //   [2]=DSKCHANGE (cleared after CIA-B drives a step),
        //   [1:0] driven by the CIA, not the peripheral.
        .pa_in    ({2'b11, !fl_dskrdy_low, fl_track0_low, 1'b1,
                    dskchange_cleared, 2'b00}),
        .pb_in    (8'd0),
        .pa_out   (cia_a_pa_out),
        .pa_oe    (cia_a_pa_oe),
        .pb_out   (),
        .pb_oe    (),
        .int_o    (cia_a_int)
    );

    // CIA-B PB drives the floppy interface lines: PB7 /MOTOR, PB6..PB3
    // /SEL3..SEL0, PB2 /SIDE, PB1 /DIRECTION, PB0 /STEP.  Expose pb_out
    // so we can model real-hw side effects (motor → DSKRDY, step →
    // DSKCHANGE clear) on the CIA-A PRA input.
    wire [7:0] cia_b_pb_out;
    cia u_cia_b (
        .clk      (clk),
        .rst_n    (rst_n),
        .slv_req  (cia_b_slv_req),
        .slv_we   (cia_b_slv_we),
        .slv_addr (cia_b_slv_addr),
        .slv_wdata(cia_b_slv_wdata),
        .slv_rdata(cia_b_slv_rdata),
        .tick     (1'b1),
        .kbd_wr   (1'b0),
        .kbd_byte (8'd0),
        .pa_in    (8'd0),
        .pb_in    (8'd0),
        .pa_out   (),
        .pa_oe    (),
        .pb_out   (cia_b_pb_out),
        .pb_oe    (),
        .int_o    (cia_b_int)
    );

    // ----- Simulated floppy drive state -----------------------------
    // Real Amiga floppy lines wired to CIA-A PRA inputs:
    //   PRA[5] = /DSKRDY    (low when motor spun up and disk present)
    //   PRA[4] = /DSKTRACK0 (low while head over cylinder 0)
    //   PRA[2] = /DSKCHANGE (low while a disk-change is pending; the
    //                       drive clears it high after the head steps)
    // Our previous model hard-wired these to "ready, at T0, change
    // pending" which sufficed for early-boot but stalled trackdisk
    // before it would issue a step (it kept seeing DSKRDY=0 without
    // having turned the motor on, decided the drive was bogus).
    //
    // Now: track CIA-B PB7 (/MOTOR) to gate DSKRDY, and look for
    // /STEP rising edges (PB0 0→1) with a drive selected (any
    // PB6..PB3 low) to clear DSKCHANGE.  Spin-up delay is a token
    // 16-bit counter -- enough for K1.3 to observe a transition,
    // not so long the regression tests slow to a crawl.
    reg pb0_step_prev;
    reg dskchange_cleared;
    reg [15:0] motor_on_counter;       // counts cycles since motor cmd on
    reg [6:0]  cur_cyl;                // 0..79 (current physical cylinder)
    reg        cur_side;               // 0 or 1 (head select, /SIDE from PB2)
    wire motor_on_now = !cia_b_pb_out[7];                  // /MOTOR active-low
    wire any_drive_selected = !(&cia_b_pb_out[6:3]);       // any /SEL low
    // After 16K cycles of motor-on with a drive selected, declare drive
    // ready.  Threshold picked small for sim throughput; real hw is ~0.5s.
    wire fl_dskrdy_low = (motor_on_counter >= 16'd16384);  // DSKRDY active-low
    // /DSKTRACK0 is active-low: 0 when head is at cylinder 0, 1 otherwise.
    wire fl_track0_low = (cur_cyl != 7'd0);
    // ADF track index: cyl*2 + side.  0..159.  Floppy DMA in the bus uses
    // this to fetch from the correct MFM-encoded track within disk[].
    assign disk_track_idx = {1'b0, cur_cyl} + {1'b0, cur_cyl} + {7'd0, cur_side};
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pb0_step_prev     <= 1'b1;
            dskchange_cleared <= 1'b0;
            motor_on_counter  <= 16'd0;
            cur_cyl           <= 7'd0;
            cur_side          <= 1'b0;
        end else begin
            pb0_step_prev <= cia_b_pb_out[0];
            // /STEP rising edge with a drive selected = head step.  Real
            // Amiga: /DIR (PB1) = 1 → step *in* toward higher cyl, 0 → step
            // *out* toward cyl 0.  Take the step on the rising edge of /STEP
            // (PB0 0→1), since trackdisk asserts /STEP low then high.
            if (cia_b_pb_out[0] && !pb0_step_prev && any_drive_selected) begin
                dskchange_cleared <= 1'b1;
                if (cia_b_pb_out[1]) begin
                    // /DIR = 1: step in (toward cyl 79).
                    if (cur_cyl != 7'd79) cur_cyl <= cur_cyl + 7'd1;
                end else begin
                    // /DIR = 0: step out (toward cyl 0).
                    if (cur_cyl != 7'd0) cur_cyl <= cur_cyl - 7'd1;
                end
            end
            // /SIDE follows PB2 continuously, INVERTED.  The line is
            // active-low: PB2=1 (default) selects head 0, PB2=0 selects
            // head 1.  Wired uninverted, K1.3 trackdisk's default PRB
            // wrote our bootblock read as track 1 instead of track 0.
            cur_side <= !cia_b_pb_out[2];
            // Motor spin-up counter.
            if (motor_on_now && any_drive_selected) begin
                if (motor_on_counter != 16'hFFFF)
                    motor_on_counter <= motor_on_counter + 16'd1;
            end else begin
                motor_on_counter <= 16'd0;
            end
        end
    end

    // Blitter instance.  Its master port plugs into the bus at index
    // BLT_PORT (the slot past all CPU I/D ports).
    wire        blt_mst_req;
    wire        blt_mst_we;
    wire [31:0] blt_mst_addr;
    wire [31:0] blt_mst_wdata;
    wire [3:0]  blt_mst_be;
    wire        blt_mst_ack;
    wire [31:0] blt_mst_rdata;

    blitter u_blt (
        .clk          (clk),
        .rst_n        (rst_n),
        .slv_req      (blt_slv_req),
        .slv_we       (blt_slv_we),
        .slv_addr     (blt_slv_addr),
        .slv_be       (blt_slv_be),
        .slv_wdata    (blt_slv_wdata),
        .slv_rdata    (blt_slv_rdata),
        .slv_ack      (),               // unused: bus handles ack timing
        .mst_req      (blt_mst_req),
        .mst_we       (blt_mst_we),
        .mst_addr     (blt_mst_addr),
        .mst_wdata    (blt_mst_wdata),
        .mst_be       (blt_mst_be),
        .mst_ack      (blt_mst_ack),
        .mst_rdata    (blt_mst_rdata),
        .busy_o       (blt_busy),
        .bzero_o      (blt_bzero),
        .int_o        (blt_int)
    );

    // Wire the blitter master into the arbiter at BLT_PORT.
    assign p_req  [BLT_PORT]                  = blt_mst_req;
    assign p_we   [BLT_PORT]                  = blt_mst_we;
    assign p_lock [BLT_PORT]                  = 1'b0;
    assign p_addr [32*BLT_PORT +: 32]         = blt_mst_addr;
    assign p_wdata[32*BLT_PORT +: 32]         = blt_mst_wdata;
    assign p_be   [4*BLT_PORT  +: 4]          = blt_mst_be;
    assign blt_mst_ack   = p_resp_valid[BLT_PORT];
    assign blt_mst_rdata = p_resp_data;

    // Copper master wires.
    wire        cop_mst_req;
    wire        cop_mst_we;
    wire [31:0] cop_mst_addr;
    wire [31:0] cop_mst_wdata;
    wire [3:0]  cop_mst_be;
    wire        cop_mst_ack;
    wire [31:0] cop_mst_rdata;

    copper u_cop (
        .clk        (clk),
        .rst_n      (rst_n),
        .slv_req    (cop_slv_req),
        .slv_we     (cop_slv_we),
        .slv_addr   (cop_slv_addr),
        .slv_be     (cop_slv_be),
        .slv_wdata  (cop_slv_wdata),
        .slv_rdata  (cop_slv_rdata),
        .mst_req    (cop_mst_req),
        .mst_we     (cop_mst_we),
        .mst_addr   (cop_mst_addr),
        .mst_wdata  (cop_mst_wdata),
        .mst_be     (cop_mst_be),
        .mst_ack    (cop_mst_ack),
        .mst_rdata  (cop_mst_rdata),
        .blt_busy_i (blt_busy),
        .vbeam_i    (vbeam)
    );

    // Wire the Copper master into the arbiter at COP_PORT.
    assign p_req  [COP_PORT]                  = cop_mst_req;
    assign p_we   [COP_PORT]                  = cop_mst_we;
    assign p_lock [COP_PORT]                  = 1'b0;
    assign p_addr [32*COP_PORT +: 32]         = cop_mst_addr;
    assign p_wdata[32*COP_PORT +: 32]         = cop_mst_wdata;
    assign p_be   [4*COP_PORT  +: 4]          = cop_mst_be;
    assign cop_mst_ack   = p_resp_valid[COP_PORT];
    assign cop_mst_rdata = p_resp_data;

    // Live Denise raster row, fed into the Copper's vbeam_i for
    // raster-aligned MOVEs.
    wire [15:0] vbeam;

    // Denise master wires.
    wire        den_mst_req;
    wire        den_mst_we;
    wire [31:0] den_mst_addr;
    wire [31:0] den_mst_wdata;
    wire [3:0]  den_mst_be;
    wire        den_mst_ack;
    wire [31:0] den_mst_rdata;

    denise #(.FB_W(FB_W)) u_den (
        .clk        (clk),
        .rst_n      (rst_n),
        .slv_req    (den_slv_req),
        .slv_we     (den_slv_we),
        .slv_addr   (den_slv_addr),
        .slv_be     (den_slv_be),
        .slv_wdata  (den_slv_wdata),
        .slv_rdata  (den_slv_rdata),
        .mst_req    (den_mst_req),
        .mst_we     (den_mst_we),
        .mst_addr   (den_mst_addr),
        .mst_wdata  (den_mst_wdata),
        .mst_be     (den_mst_be),
        .mst_ack    (den_mst_ack),
        .mst_rdata  (den_mst_rdata),
        .vbeam_o    (vbeam),
        .auto_kick_i    (vblank_pulse),
        .auto_active_i  (bus_den_auto_active),
        .auto_bpl_pt_flat_i(den_auto_bpl_pt_flat),
        .auto_bpl_mod1_i(bus_bpl_mod1),
        .auto_bpl_mod2_i(bus_bpl_mod2)
    );

    assign p_req  [DEN_PORT]                  = den_mst_req;
    assign p_we   [DEN_PORT]                  = den_mst_we;
    assign p_lock [DEN_PORT]                  = 1'b0;
    assign p_addr [32*DEN_PORT +: 32]         = den_mst_addr;
    assign p_wdata[32*DEN_PORT +: 32]         = den_mst_wdata;
    assign p_be   [4*DEN_PORT  +: 4]          = den_mst_be;
    assign den_mst_ack   = p_resp_valid[DEN_PORT];
    assign den_mst_rdata = p_resp_data;

    // Paula master wires.
    wire        pau_mst_req;
    wire        pau_mst_we;
    wire [31:0] pau_mst_addr;
    wire [31:0] pau_mst_wdata;
    wire [3:0]  pau_mst_be;
    wire        pau_mst_ack;
    wire [31:0] pau_mst_rdata;

    wire [2:0] paula_irq_level;
    wire       vblank_pulse;
    wire       dskblk_pulse;
    wire       dsksyn_pulse;
    wire       ovl_clear_pulse;

    paula u_pau (
        .clk        (clk),
        .rst_n      (rst_n),
        .slv_req    (pau_slv_req),
        .slv_we     (pau_slv_we),
        .slv_addr   (pau_slv_addr),
        .slv_be     (pau_slv_be),
        .slv_wdata  (pau_slv_wdata),
        .slv_rdata  (pau_slv_rdata),
        .mst_req    (pau_mst_req),
        .mst_we     (pau_mst_we),
        .mst_addr   (pau_mst_addr),
        .mst_wdata  (pau_mst_wdata),
        .mst_be     (pau_mst_be),
        .mst_ack    (pau_mst_ack),
        .mst_rdata  (pau_mst_rdata),
        .audio_l_o  (audio_l),
        .audio_r_o  (audio_r),
        .cia_a_int_i (cia_a_int),
        .cia_b_int_i (cia_b_int),
        .blt_int_i   (blt_int),
        .cop_int_i   (1'b0),           // copper int not wired yet
        .vblank_int_i(vblank_pulse),
        .dskblk_int_i(dskblk_pulse),
        .dsksyn_int_i(dsksyn_pulse),
        .irq_level_o (paula_irq_level)
    );

    assign p_req  [PAU_PORT]                  = pau_mst_req;
    assign p_we   [PAU_PORT]                  = pau_mst_we;
    assign p_lock [PAU_PORT]                  = 1'b0;
    assign p_addr [32*PAU_PORT +: 32]         = pau_mst_addr;
    assign p_wdata[32*PAU_PORT +: 32]         = pau_mst_wdata;
    assign p_be   [4*PAU_PORT  +: 4]          = pau_mst_be;
    assign pau_mst_ack   = p_resp_valid[PAU_PORT];
    assign pau_mst_rdata = p_resp_data;

    genvar gi;
    generate
        for (gi = 0; gi < N_CORES; gi = gi + 1) begin : g_core
            // I-cache port = gi*2, D-cache port = gi*2+1.
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

            if (USE_CACHE) begin : g_cache
                m68k_cache #(
                    .NUM_LINES (256),
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2),
                    .WRITABLE  (0)
                ) u_ic (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (ic_req),
                    .cpu_we      (1'b0),
                    .cpu_lock    (1'b0),
                    .cpu_addr    (ic_addr),
                    .cpu_wdata   (32'd0),
                    .cpu_be      (4'b1111),
                    .cpu_is_long (1'b0),
                    .cpu_ack     (ic_ack),
                    .cpu_rdata   (ic_rdata),
                    .bus_req     (p_req[gi*2]),
                    .bus_we      (p_we[gi*2]),
                    .bus_lock    (p_lock[gi*2]),
                    .bus_addr    (p_addr[32*(gi*2) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2) +: 32]),
                    .bus_be      (p_be[4*(gi*2) +: 4]),
                    .bus_is_long (p_is_long[gi*2]),
                    .bus_grant   (p_grant[gi*2]),
                    .bus_resp_valid(p_resp_valid[gi*2]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id),
                    .inval_all_i (ovl_clear_pulse)
                );

                m68k_cache #(
                    .NUM_LINES (256),
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2 + 1),
                    .WRITABLE  (1)
                ) u_dc (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (dc_req),
                    .cpu_we      (dc_we),
                    .cpu_lock    (dc_lock),
                    .cpu_addr    (dc_addr),
                    .cpu_wdata   (dc_wdata),
                    .cpu_be      (dc_be),
                    .cpu_is_long (dc_is_long),
                    .cpu_ack     (dc_ack),
                    .cpu_rdata   (dc_rdata),
                    .bus_req     (p_req[gi*2+1]),
                    .bus_we      (p_we[gi*2+1]),
                    .bus_lock    (p_lock[gi*2+1]),
                    .bus_addr    (p_addr[32*(gi*2+1) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2+1) +: 32]),
                    .bus_be      (p_be[4*(gi*2+1) +: 4]),
                    .bus_is_long (p_is_long[gi*2+1]),
                    .bus_grant   (p_grant[gi*2+1]),
                    .bus_resp_valid(p_resp_valid[gi*2+1]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id),
                    .inval_all_i (ovl_clear_pulse)
                );
            end else begin : g_passthrough
                m68k_passthrough #(
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2),
                    .WRITABLE  (0)
                ) u_ic (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (ic_req),
                    .cpu_we      (1'b0),
                    .cpu_lock    (1'b0),
                    .cpu_addr    (ic_addr),
                    .cpu_wdata   (32'd0),
                    .cpu_be      (4'b1111),
                    .cpu_is_long (1'b0),
                    .cpu_ack     (ic_ack),
                    .cpu_rdata   (ic_rdata),
                    .bus_req     (p_req[gi*2]),
                    .bus_we      (p_we[gi*2]),
                    .bus_lock    (p_lock[gi*2]),
                    .bus_addr    (p_addr[32*(gi*2) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2) +: 32]),
                    .bus_be      (p_be[4*(gi*2) +: 4]),
                    .bus_is_long (p_is_long[gi*2]),
                    .bus_grant   (p_grant[gi*2]),
                    .bus_resp_valid(p_resp_valid[gi*2]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id),
                    .inval_all_i (ovl_clear_pulse)
                );

                m68k_passthrough #(
                    .ID_BITS   (PID_BITS),
                    .CORE_ID   (gi*2 + 1),
                    .WRITABLE  (1)
                ) u_dc (
                    .clk         (clk),
                    .rst_n       (rst_n),
                    .cpu_req     (dc_req),
                    .cpu_we      (dc_we),
                    .cpu_lock    (dc_lock),
                    .cpu_addr    (dc_addr),
                    .cpu_wdata   (dc_wdata),
                    .cpu_be      (dc_be),
                    .cpu_is_long (dc_is_long),
                    .cpu_ack     (dc_ack),
                    .cpu_rdata   (dc_rdata),
                    .bus_req     (p_req[gi*2+1]),
                    .bus_we      (p_we[gi*2+1]),
                    .bus_lock    (p_lock[gi*2+1]),
                    .bus_addr    (p_addr[32*(gi*2+1) +: 32]),
                    .bus_wdata   (p_wdata[32*(gi*2+1) +: 32]),
                    .bus_be      (p_be[4*(gi*2+1) +: 4]),
                    .bus_is_long (p_is_long[gi*2+1]),
                    .bus_grant   (p_grant[gi*2+1]),
                    .bus_resp_valid(p_resp_valid[gi*2+1]),
                    .bus_resp_data (p_resp_data),
                    .snoop_valid (snoop_valid),
                    .snoop_addr  (snoop_addr),
                    .snoop_src_id(snoop_src_id),
                    .inval_all_i (ovl_clear_pulse)
                );
            end

            // Per-core reset SP: read from memory[gi] (low word of mem[gi]).
            // We expose it via the bus model by a "reset_a7" wire computed by
            // peeking memory at startup. Simplest: derive from a parameter.
            // We give each core its own stack: 0x8000, 0x9000, 0xA000, 0xB000.
            wire [31:0] reset_a7 = 32'h0000_4000 + (gi * 32'h0000_1000);

            m68k_core #(
                .CORE_ID(gi[3:0])
            ) u_core (
                .clk        (clk),
                .rst_n      (rst_n),
                .reset_a7   (reset_a7),
                .ipl_i      (irq_level),
                .ic_req     (ic_req),
                .ic_we      (ic_we),
                .ic_lock    (ic_lock),
                .ic_addr    (ic_addr),
                .ic_wdata   (ic_wdata),
                .ic_be      (ic_be),
                .ic_ack     (ic_ack),
                .ic_rdata   (ic_rdata),
                .dc_req     (dc_req),
                .dc_we      (dc_we),
                .dc_lock    (dc_lock),
                .dc_addr    (dc_addr),
                .dc_wdata   (dc_wdata),
                .dc_be      (dc_be),
                .dc_is_long (dc_is_long),
                .dc_ack     (dc_ack),
                .dc_rdata   (dc_rdata),
                .halted     (halted[gi]),
                .halt_code  (halt_code_flat[16*gi +: 16]),
                .retired    (retired_flat[32*gi +: 32])
            );
        end
    endgenerate
endmodule
