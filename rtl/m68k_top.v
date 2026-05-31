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
    // Per-core EX-stage PC, exposed so the simulator harness can trigger
    // chipram snapshots when execution hits a configured PC.  Single-core
    // kickstart-boot only inspects core 0.  See DIAGNOSTICS.md.
    output wire [32*N_CORES-1:0] core_pc_flat,

    // Framebuffer peek for the simulator harness. Drive fb_peek_addr with a
    // word-aligned byte address; fb_peek_data returns the 32-bit word.
    input  wire [31:0] fb_peek_addr,
    output wire [31:0] fb_peek_data,

    // Diagnostic memory-poke port (sim_main writes chip RAM directly).
    input  wire        mem_poke_strobe,
    input  wire [31:0] mem_poke_addr,
    input  wire [31:0] mem_poke_data,

    // Live Paula audio output for the simulator harness.
    output wire signed [15:0] audio_l,
    output wire signed [15:0] audio_r,

    // External input injection from the harness (SDL keyboard + mouse).
    // These are sampled by the chipset RTL and emulate a real Amiga
    // keyboard/mouse plugged into the CIA / Paula ports.
    //
    //   ext_kbd_wr     : pulse high for one host clock to deliver a scan
    //                    code byte to CIA-A (same path as $00FE9000).
    //   ext_kbd_byte   : 8-bit scancode (Amiga raw key + bit7=key-up).
    //   mouse_x_count  : 8-bit horizontal quadrature counter (wraps).
    //                    Exposed as the low byte of JOY0DAT.
    //   mouse_y_count  : 8-bit vertical quadrature counter.  Exposed as
    //                    the high byte of JOY0DAT.
    //   mouse_btn_l    : 1 while the left mouse button is held; drives
    //                    /FIR0 (CIA-A PRA bit 6) low.
    //   mouse_btn_r    : 1 while the right button is held; drives the
    //                    POTGOR right-button bit low.
    //
    // All default to "no input" so headless / regression runs are
    // unaffected (the harness ties them to 0 when no SDL is present).
    input  wire        ext_kbd_wr,
    input  wire [7:0]  ext_kbd_byte,
    input  wire [7:0]  mouse_x_count,
    input  wire [7:0]  mouse_y_count,
    input  wire        mouse_btn_l,
    input  wire        mouse_btn_r
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

    // Cosim window signals driven into the bus and chipset modules so
    // their non-CPU memory writes can emit `[CosimMW]` traces aligned
    // with the CPU's retired counter.  Default to 0/off in non-cosim
    // builds — see `+cosim_start=` / `+cosim_end=` plusargs (m68k_core.v).
    wire [31:0] cosim_retired_w = retired_flat[31:0];
`ifdef KICKSTART_COSIM_TRACE
    wire cosim_in_window_w =
        (cosim_retired_w >= g_core[0].u_core.cosim_start_r) &&
        (cosim_retired_w <= g_core[0].u_core.cosim_end_r);
`else
    wire cosim_in_window_w = 1'b0;
`endif
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

    // Per-core "CPU is parked on STOP" hint.  Used by the Agnus beam
    // counter (and CIA tick prescaler) to fast-forward sim time during
    // long idle stretches without changing observable CPU behavior —
    // when all cores are STOPped, the only thing that matters is when
    // the next IRQ arrives, so we run the beam ahead at N×.
    wire [N_CORES-1:0]    cpu_in_stop_flat;
    wire                  all_cores_in_stop = &cpu_in_stop_flat;

    // Blitter slave wires.
    wire        blt_slv_req;
    wire        blt_slv_we;
    wire [5:0]  blt_slv_addr;
    wire [3:0]  blt_slv_be;
    wire        blt_slv_is_long;
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
    // The simulator harness can ALSO inject scancodes via ext_kbd_wr /
    // ext_kbd_byte (no CPU bus cycle required); both sources OR into the
    // CIA-A serial-receive path.
    wire        cia_a_kbd_wr_bus;
    wire [7:0]  cia_a_kbd_byte_bus;
    wire        cia_a_kbd_wr   = cia_a_kbd_wr_bus | ext_kbd_wr;
    wire [7:0]  cia_a_kbd_byte = ext_kbd_wr ? ext_kbd_byte
                                            : cia_a_kbd_byte_bus;

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
    wire        bus_cop_auto_active;
    wire        bus_cop_cdang;
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
        .mem_poke_strobe(mem_poke_strobe),
        .mem_poke_addr  (mem_poke_addr),
        .mem_poke_data  (mem_poke_data),
        .cosim_retired_i (cosim_retired_w),
        .cosim_in_window_i (cosim_in_window_w),
        .fb_peek_data(fb_peek_data),
        .blt_slv_req (blt_slv_req),
        .blt_slv_we  (blt_slv_we),
        .blt_slv_addr(blt_slv_addr),
        .blt_slv_be  (blt_slv_be),
        .blt_slv_is_long(blt_slv_is_long),
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
        .kbd_inject_wr  (cia_a_kbd_wr_bus),
        .kbd_inject_byte(cia_a_kbd_byte_bus),
        .mouse_x_in     (mouse_x_count),
        .mouse_y_in     (mouse_y_count),
        .mouse_btn_r_in (mouse_btn_r),
        .vblank_pulse_o (vblank_pulse),
        .dskblk_pulse_o (dskblk_pulse),
        .dsksyn_pulse_o (dsksyn_pulse),
        .ovl_clear_pulse_o (ovl_clear_pulse),
        .disk_track_i   (disk_track_idx),
        .sim_ff_active  (all_cores_in_stop),
        .bpl_pt0_o      (bus_bpl_pt0),
        .bpl_pt1_o      (bus_bpl_pt1),
        .bpl_pt2_o      (bus_bpl_pt2),
        .bpl_pt3_o      (bus_bpl_pt3),
        .bpl_pt4_o      (bus_bpl_pt4),
        .bpl_pt5_o      (bus_bpl_pt5),
        .bpl_mod1_o     (bus_bpl_mod1),
        .bpl_mod2_o     (bus_bpl_mod2),
        .den_auto_active_o (bus_den_auto_active),
        .cop_auto_active_o (bus_cop_auto_active),
        .cop_cdang_o       (bus_cop_cdang)
    );

    // CIA-A and CIA-B.  Tick every bus cycle for now (10x real Amiga rate)
    // -- when we split into chip-clock vs CPU-clock domains we'll gate this.
    // CIA tick prescaler.  Real CIAs (8520) run at phi2 ≈ 0.715 MHz,
    // 1/10 of the 7.16 MHz CPU clock.  Our host clock approximates Agnus
    // (3.55 MHz), so the CIA tick should pulse once every ~5 host cycles
    // (Agnus / 5 ≈ 0.71 MHz).  Without this prescaler, Timer A/B count
    // 5x too fast: K1.3's timer.device.Init at $FE8F8E loads TBHI=$FF
    // and busy-waits for CIA-A TODLOW to change while testing TBHI's
    // sign bit as a timeout.  With Timer B running at host speed, TBHI's
    // high bit clears in ~33K host cycles — *before* the first VBL
    // (~71K cycles) gives TODLOW a chance to tick — so the OS takes the
    // Alert path ($FE8F9E -> JSR Alert -> BRA $FE8F70).  That branch
    // bypasses the BSR's matching RTS, orphaning the BSR return PC on
    // the stack -> SP off by 4 -> exit MOVEM at $FE8F88 reads from the
    // wrong slot -> RTS pops $0 -> CPU executes vector table data ->
    // exception storm at PC=$BFD100.  See project_wb13_cli_wait.md.
    reg [2:0] cia_tick_div;
    // During sim fast-forward (all cores STOPped), tick every host clock
    // instead of every 5th — keeps CIA Timer A/B counting at the same
    // rate relative to the Agnus beam (which also fast-forwards 64x).
    // OS-visible timing (TOD-on-VBL, Timer-B-elapsed-vs-VBL) is preserved.
    wire cia_tick = all_cores_in_stop ? 1'b1 : (cia_tick_div == 3'd4);
    always @(posedge clk) begin
        if (!rst_n)                    cia_tick_div <= 3'd0;
        else if (all_cores_in_stop)    cia_tick_div <= 3'd0;
        else if (cia_tick_div == 3'd4) cia_tick_div <= 3'd0;
        else                           cia_tick_div <= cia_tick_div + 3'd1;
    end

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
        .tick     (cia_tick),
        .kbd_wr   (cia_a_kbd_wr),
        .kbd_byte (cia_a_kbd_byte),
        // PRA input bit composition (active-low signals):
        //   [7]=/FIR0  (mouse port 0 button — low while pressed),
        //   [6]=/FIR1  (mouse port 1 button — low while pressed),
        //   [5]=DSKRDY (motor-gated, low when ready),
        //   [4]=DSKTRACK0 (low when cur_cyl == 0),
        //   [3]=DSKWRPRO=1 (write-protect inactive),
        //   [2]=DSKCHANGE (cleared after CIA-B drives a step),
        //   [1:0] driven by the CIA, not the peripheral.
        // mouse_btn_l drives both port-0 and port-1 fire low because we
        // don't know which port Workbench is configured for; either way
        // the left button gets observed.
        .pa_in    ({~mouse_btn_l, ~mouse_btn_l, !fl_dskrdy_low, fl_track0_low, 1'b1,
                    dskchange_cleared, 2'b00}),
        .pb_in    (8'd0),
        .pa_out   (cia_a_pa_out),
        .pa_oe    (cia_a_pa_oe),
        .pb_out   (),
        .pb_oe    (),
        .tod_tick_i (vblank_pulse),
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
        .tick     (cia_tick),
        .kbd_wr   (1'b0),
        .kbd_byte (8'd0),
        .pa_in    (8'd0),
        .pb_in    (8'd0),
        .pa_out   (),
        .pa_oe    (),
        .pb_out   (cia_b_pb_out),
        .pb_oe    (),
        .tod_tick_i (1'b0),
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
                // /DIR is active-low.  Amiga HRM: high (PB1=1) = step
                // OUT toward cyl 0 (lower-numbered cylinders); low
                // (PB1=0) = step IN toward cyl 79 (higher-numbered
                // cylinders).  Without this polarity correct, trackdisk's
                // seek-to-track-80 issued 80 "step OUT" pulses (which
                // clamped at cyl 0) and never reached the rootblock —
                // every WB1.3 disk read came back as track 0 data.
                if (cia_b_pb_out[1]) begin
                    if (cur_cyl != 7'd0) cur_cyl <= cur_cyl - 7'd1;
`ifdef KICKSTART_BOOT_TRACE
                    $display("[STEP] out cur_cyl=%0d->%0d", cur_cyl, cur_cyl-1);
`endif
                end else begin
                    if (cur_cyl != 7'd79) cur_cyl <= cur_cyl + 7'd1;
`ifdef KICKSTART_BOOT_TRACE
                    $display("[STEP] in cur_cyl=%0d->%0d", cur_cyl, cur_cyl+1);
`endif
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
        .slv_is_long  (blt_slv_is_long),
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
        .vbeam_i    (vbeam),
        .auto_kick_i   (vblank_pulse),
        .auto_active_i (bus_cop_auto_active),
        .cdang_i       (bus_cop_cdang)
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
        .cpu_in_stop_i (all_cores_in_stop),
        .irq_level_o (paula_irq_level),
        .watchdog_kick_o (watchdog_kick)
    );

    // Combine OVL-clear pulse + watchdog-kick pulse for cache invalidation.
    // The watchdog kick is needed because the IRQ-entry sequence after a
    // STOP exit can race with stale bus_resp_data; invalidating the cache
    // forces a fresh bus read for the vector.
    wire watchdog_kick;
    wire cache_inval = ovl_clear_pulse | watchdog_kick;

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
                    .inval_all_i (cache_inval)
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
                    .inval_all_i (cache_inval)
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
                    .inval_all_i (cache_inval)
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
                    .inval_all_i (cache_inval)
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
                .retired    (retired_flat[32*gi +: 32]),
                .cpu_in_stop(cpu_in_stop_flat[gi]),
                .cur_pc     (core_pc_flat[32*gi +: 32])
            );
        end
    endgenerate

    // ----------------------------------------------------------------
    // Snapshot-at-PC breakpoint.
    //
    // Set +snap_pc=0xFEA970 +snap_dir=build_kick_boot/snap on the
    // Vm68k_top command line.  When core 0's executing PC reaches
    // snap_pc (and core 0 is in the "is_settled" commit cycle), we
    // dump:
    //   <dir>/regs.txt   — D0..D7 / A0..A7 / PC / SR / USP
    //   <dir>/mem.hex    — chip RAM (one 32-bit longword per line)
    //   <dir>/slow.hex   — slow/trapdoor RAM
    //   <dir>/chip.txt   — chipset shadow regs (intena/intreq/dmacon/vpos)
    // then call $finish so the snapshot files reflect a quiescent state.
    //
    // The snapshot is the "switch ON cosim" trigger from the user's
    // mental model: re-launch musashi_kick with the snapshot dir to
    // resume from this point.  See `make cosim-window` (TODO).
`ifdef KICKSTART_BOOT_TRACE
    reg [31:0] snap_pc;
    reg [31:0] snap_after_retired;
    reg        snap_armed;
    reg [255*8-1:0] snap_dir;
    initial begin
        if (!$value$plusargs("snap_pc=%h", snap_pc)) snap_pc = 32'hFFFF_FFFF;
        if (!$value$plusargs("snap_after_retired=%d", snap_after_retired))
            snap_after_retired = 32'd0;
        if (!$value$plusargs("snap_dir=%s", snap_dir)) snap_dir = "snap";
        snap_armed = 1'b1;
    end
    integer snap_fp;
    always @(posedge clk) begin
        if (rst_n && snap_armed &&
            snap_pc != 32'hFFFF_FFFF &&
            retired_flat[31:0] >= snap_after_retired &&
            g_core[0].u_core.is_settled &&
            g_core[0].u_core.ex_pc == snap_pc) begin
            $display("=== SNAPSHOT @ PC=%h retired=%0d ===",
                g_core[0].u_core.ex_pc, retired_flat[31:0]);
            // Write machine-readable regs.txt (one KEY=HEX per line).
            // Consumers: tools/snap_to_musashi.py, musashi_kick.c --seed.
            snap_fp = $fopen({snap_dir, "/regs.txt"}, "w");
            $fwrite(snap_fp, "PC=%h\n", g_core[0].u_core.ex_pc);
            $fwrite(snap_fp, "SR=%h\n", g_core[0].u_core.sr_now);
            $fwrite(snap_fp, "USP=%h\n", g_core[0].u_core.usp_shadow);
            $fwrite(snap_fp, "D0=%h\n", g_core[0].u_core.u_rf.regs[0]);
            $fwrite(snap_fp, "D1=%h\n", g_core[0].u_core.u_rf.regs[1]);
            $fwrite(snap_fp, "D2=%h\n", g_core[0].u_core.u_rf.regs[2]);
            $fwrite(snap_fp, "D3=%h\n", g_core[0].u_core.u_rf.regs[3]);
            $fwrite(snap_fp, "D4=%h\n", g_core[0].u_core.u_rf.regs[4]);
            $fwrite(snap_fp, "D5=%h\n", g_core[0].u_core.u_rf.regs[5]);
            $fwrite(snap_fp, "D6=%h\n", g_core[0].u_core.u_rf.regs[6]);
            $fwrite(snap_fp, "D7=%h\n", g_core[0].u_core.u_rf.regs[7]);
            $fwrite(snap_fp, "A0=%h\n", g_core[0].u_core.u_rf.regs[8]);
            $fwrite(snap_fp, "A1=%h\n", g_core[0].u_core.u_rf.regs[9]);
            $fwrite(snap_fp, "A2=%h\n", g_core[0].u_core.u_rf.regs[10]);
            $fwrite(snap_fp, "A3=%h\n", g_core[0].u_core.u_rf.regs[11]);
            $fwrite(snap_fp, "A4=%h\n", g_core[0].u_core.u_rf.regs[12]);
            $fwrite(snap_fp, "A5=%h\n", g_core[0].u_core.u_rf.regs[13]);
            $fwrite(snap_fp, "A6=%h\n", g_core[0].u_core.u_rf.regs[14]);
            $fwrite(snap_fp, "A7=%h\n", g_core[0].u_core.u_rf.regs[15]);
            $fwrite(snap_fp, "INTENA=%h\n", u_pau.intena);
            $fwrite(snap_fp, "INTREQ=%h\n", u_pau.intreq);
            $fwrite(snap_fp, "DMACON=%h\n", u_bus.dmacon);
            $fwrite(snap_fp, "RETIRED=%0d\n", retired_flat[31:0]);
            $fclose(snap_fp);
            // Also dump to stdout for at-a-glance inspection.
            $display("[SNAP] PC=%h SR=%h USP=%h",
                g_core[0].u_core.ex_pc,
                g_core[0].u_core.sr_now,
                g_core[0].u_core.usp_shadow);
            $display("[SNAP] D0..D7 A0..A7 + chipset written to regs.txt");
            // Dump chip RAM and slow RAM via $writememh.  The Musashi
            // resume needs both — graphics primitives and most Exec
            // structures live in slow RAM, so cosim diverges immediately
            // without it.
            $writememh({snap_dir, "/mem.hex"}, u_bus.mem);
            $writememh({snap_dir, "/slow.hex"}, u_bus.slowmem);
            $display("[SNAP] mem.hex (chip) + slow.hex written");
            snap_armed <= 1'b0;
            $finish;
        end
    end

    // [TD-BEGIO-DEEP]: at trackdisk.device.BeginIO entry ($FE9C3E), log
    // io_Command (byte at $1D(A1)) and the return-PC sitting at the top
    // of SP.  Distinguishes "spinning on same command from same caller"
    // from "different commands queued from different sites".  A1 and SP
    // are in slow RAM at boot ($C00000..$C7FFFF range).
    wire [31:0] td_a1 = g_core[0].u_core.u_rf.regs[9];
    wire [31:0] td_sp = g_core[0].u_core.u_rf.regs[15];
    wire        td_a1_in_slow = (td_a1[23:20] == 4'hC);
    wire        td_sp_in_slow = (td_sp[23:20] == 4'hC);
    wire [31:0] td_ioreq_w    = td_a1_in_slow
                                ? u_bus.slowmem[(td_a1 + 32'h1D - 32'h00C0_0000) >> 2]
                                : 32'd0;
    wire [1:0]  td_cmd_baddr  = (td_a1[1:0] + 2'd1);  // ($1D & 3) = 1 + a1[1:0]
    reg  [7:0]  td_cmd_b;
    always @* begin
        case (td_cmd_baddr)
            2'd0: td_cmd_b = td_ioreq_w[31:24];
            2'd1: td_cmd_b = td_ioreq_w[23:16];
            2'd2: td_cmd_b = td_ioreq_w[15:8];
            default: td_cmd_b = td_ioreq_w[7:0];
        endcase
    end
    // Longword at SP and at SP+12 (caller-of-DoIO return PC: BeginIO's
    // return + pushed A6 + pushed A1 = 12-byte frame above SP).  SP may
    // be word-aligned but not longword-aligned (SP[1:0] == 2'b10 is
    // common after odd-pair pushes), in which case the longword straddles
    // two slowmem slots: high half lives in the low half of slot[idx],
    // low half in the high half of slot[idx+1].
    wire [31:0] td_sp_idx0 = u_bus.slowmem[(td_sp - 32'h00C0_0000) >> 2];
    wire [31:0] td_sp_idx1 = u_bus.slowmem[((td_sp - 32'h00C0_0000) >> 2) + 1];
    wire [31:0] td_sp_top  = !td_sp_in_slow ? 32'd0 :
                             (td_sp[1:0] == 2'b10)
                             ? {td_sp_idx0[15:0], td_sp_idx1[31:16]}
                             : td_sp_idx0;
    wire [31:0] td_sp12    = td_sp + 32'd12;
    wire [31:0] td_sp12_idx0 = u_bus.slowmem[(td_sp12 - 32'h00C0_0000) >> 2];
    wire [31:0] td_sp12_idx1 = u_bus.slowmem[((td_sp12 - 32'h00C0_0000) >> 2) + 1];
    wire [31:0] td_caller   = !td_sp_in_slow ? 32'd0 :
                              (td_sp12[1:0] == 2'b10)
                              ? {td_sp12_idx0[15:0], td_sp12_idx1[31:16]}
                              : td_sp12_idx0;
    reg [31:0] td_last_r;
    initial td_last_r = 32'hFFFF_FFFF;
    always @(posedge clk) begin
        if (rst_n &&
            g_core[0].u_core.is_settled &&
            g_core[0].u_core.ex_pc == 32'h00FE_9C3E &&
            (retired_flat[31:0] != td_last_r)) begin
            $display("[TD-BEGIO-DEEP] r=%0d A1=%h sp=%h cmd=%h retpc=%h caller=%h",
                retired_flat[31:0], td_a1, td_sp, td_cmd_b, td_sp_top, td_caller);
            td_last_r <= retired_flat[31:0];
        end
    end

    // [BB-VAL]: bootblock validation at $FE85A0/$FE85AA/$FE85C0.
    // Three failure branches into the TD_CHANGENUM wait loop at $FE8600.
    // Log which branch fires + D0 (io_Error or checksum), A4 (buffer ptr).
    wire [31:0] bb_a4 = g_core[0].u_core.u_rf.regs[12];
    wire [31:0] bb_d0 = g_core[0].u_core.u_rf.regs[0];
    wire        bb_a4_in_slow = (bb_a4[23:20] == 4'hC);
    wire        bb_a4_in_chip = (bb_a4 < 32'h0008_0000);
    wire [31:0] bb_a4_word0_slow = u_bus.slowmem[(bb_a4 - 32'h00C0_0000) >> 2];
    wire [31:0] bb_a4_word0_chip = u_bus.mem[bb_a4[18:2]];
    wire [31:0] bb_a4_word0 = bb_a4_in_slow ? bb_a4_word0_slow
                            : bb_a4_in_chip ? bb_a4_word0_chip
                            : 32'd0;
    reg [31:0] bb_last_r;
    initial bb_last_r = 32'hFFFF_FFFF;
    // Source buffer at $20A0 in chip RAM (the decoded-sector start) —
    // dump alongside BB-VAL traces so we can tell if the corruption came
    // from a bad copy (dst != src) or a bad source (src already wrong).
    // Index = $20A0 / 4 = $828 = 2088.
    wire [31:0] bb_src_20a0 = u_bus.mem[2088];

    always @(posedge clk) begin
        if (rst_n && g_core[0].u_core.is_settled &&
            (retired_flat[31:0] != bb_last_r)) begin
            if (g_core[0].u_core.ex_pc == 32'h00FE_85A2) begin
                $display("[BB-VAL] r=%0d at $FE85A2 IO_ERR D0=%h A4=%h *A4=%h src@20A0=%h",
                    retired_flat[31:0], bb_d0, bb_a4, bb_a4_word0, bb_src_20a0);
                bb_last_r <= retired_flat[31:0];
            end
            if (g_core[0].u_core.ex_pc == 32'h00FE_85AA) begin
                $display("[BB-VAL] r=%0d at $FE85AA MAGIC D0=%h A4=%h *A4=%h src@20A0=%h (want $444F5300)",
                    retired_flat[31:0], bb_d0, bb_a4, bb_a4_word0, bb_src_20a0);
                bb_last_r <= retired_flat[31:0];
            end
            if (g_core[0].u_core.ex_pc == 32'h00FE_85C0) begin
                $display("[BB-VAL] r=%0d at $FE85C0 CHECKSUM D0=%h A4=%h *A4=%h src@20A0=%h",
                    retired_flat[31:0], bb_d0, bb_a4, bb_a4_word0, bb_src_20a0);
                bb_last_r <= retired_flat[31:0];
            end
        end
    end
`endif
endmodule
