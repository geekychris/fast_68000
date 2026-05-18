// MOS 8520 CIA model (subset).
//
// Two timers (A, B) with reload, one-shot + continuous modes, IRQ on
// underflow.  Ports A/B and direction registers (DDR) are exposed as
// simple wires to/from the surrounding chipset (keyboard/mouse on CIA-A,
// floppy/serial on CIA-B in the real Amiga).
//
// Register map (byte offsets within the CIA window):
//   $00  PRA      data register port A     R/W
//   $01  PRB      data register port B     R/W
//   $02  DDRA     direction port A         R/W (1 = output)
//   $03  DDRB     direction port B         R/W
//   $04  TALO     timer A reload low       R/W (write = reload byte)
//   $05  TAHI     timer A reload high      R/W
//   $06  TBLO     timer B reload low       R/W
//   $07  TBHI     timer B reload high      R/W
//   $08-$0A TOD   time-of-day (24-bit)     -- not yet implemented
//   $0B  -       reserved                 -
//   $0C  SDR      serial data             -- not yet implemented
//   $0D  ICR      interrupt control       R = pending+IR; W = mask
//   $0E  CRA      timer A control         R/W
//   $0F  CRB      timer B control         R/W
//
// ICR bits: 0 = TA underflow, 1 = TB underflow, 7 = IR (any pending+masked).
// CRA bits: 0 = START, 3 = RUNMODE (0=continuous, 1=one-shot),
//           4 = LOAD (strobe; reload counter from reload value).
//
// Timer tick: an external `tick` input pulses for each 8520 phi2 (real
// Amiga drives this from the E-clock = chip clock / 10).  Asserting
// `tick` on every cycle effectively runs the timer at our bus clock.
//
// Interrupt output: `int_o` is high while ICR has at least one pending
// bit that the mask permits.  Reading ICR clears the pending bits
// (matches real CIA "read-and-clear" semantics).
//
// In real Amiga the two CIAs are accessed at byte addresses on either
// LDS or UDS, 256 bytes apart per register.  This module just takes a
// 4-bit register index in `slv_addr[3:0]` from whatever decode the bus
// presents.

module cia (
    input  wire        clk,
    input  wire        rst_n,

    // Slave port: 8-bit registers.  slv_addr selects which one.
    input  wire        slv_req,
    input  wire        slv_we,
    input  wire [3:0]  slv_addr,        // register index 0..15
    input  wire [7:0]  slv_wdata,
    output reg  [7:0]  slv_rdata,

    // Timer tick.  Assert for one bus cycle per timer phi.
    input  wire        tick,

    // Port A / B inputs (peripheral side).
    input  wire [7:0]  pa_in,
    input  wire [7:0]  pb_in,

    // Port A / B outputs (driven by PRA/PRB & DDR).
    output wire [7:0]  pa_out,
    output wire [7:0]  pa_oe,            // 1 = driven by this CIA
    output wire [7:0]  pb_out,
    output wire [7:0]  pb_oe,

    // Combined IRQ output (active high while any pending masked bit set).
    output wire        int_o
);
    // ---------------- Register storage ------------------
    reg [7:0]  pra, prb, ddra, ddrb;
    reg [7:0]  ta_lo_reload, ta_hi_reload;
    reg [7:0]  tb_lo_reload, tb_hi_reload;
    reg [15:0] ta_cnt, tb_cnt;
    reg [7:0]  cra, crb;
    reg [4:0]  icr_pending;          // bits 0..4 in real CIA; we use 0,1
    reg [4:0]  icr_mask;             // bits 0..4

    // ---------------- TOD: 24-bit time-of-day -----------------------
    // Real Amiga drives TOD from Agnus VSYNC (50 Hz PAL / 60 Hz NTSC).
    // We approximate it with a fixed prescaler so any code polling TOD
    // sees it advance.  TOD_PRESCALER=1024 gives plenty of resolution
    // for boot-time clock reads without slowing things down.
    reg [23:0] tod;
    reg [9:0]  tod_prescale;

    // Derived control bits.
    wire ta_start    = cra[0];
    wire ta_runmode  = cra[3];       // 0 = continuous, 1 = one-shot
    wire tb_start    = crb[0];
    wire tb_runmode  = crb[3];

    // Port outputs: bit = 1 in DDR -> drive PR. Otherwise, hi-z (we still
    // emit pa_out so any peer reading sees the latched PR value, but the
    // OE mask tells them to ignore those bits).
    assign pa_out = pra;
    assign pa_oe  = ddra;
    assign pb_out = prb;
    assign pb_oe  = ddrb;

    // Interrupt line: any pending bit that the mask enables.
    assign int_o = |(icr_pending & icr_mask);

    // ---------------- Slave reads -----------------------
    always @* begin
        case (slv_addr)
            4'h0: slv_rdata = (pra & ddra) | (pa_in & ~ddra);
            4'h1: slv_rdata = (prb & ddrb) | (pb_in & ~ddrb);
            4'h2: slv_rdata = ddra;
            4'h3: slv_rdata = ddrb;
            4'h4: slv_rdata = ta_cnt[7:0];
            4'h5: slv_rdata = ta_cnt[15:8];
            4'h6: slv_rdata = tb_cnt[7:0];
            4'h7: slv_rdata = tb_cnt[15:8];
            4'h8: slv_rdata = tod[7:0];     // TOD low
            4'h9: slv_rdata = tod[15:8];    // TOD mid
            4'hA: slv_rdata = tod[23:16];   // TOD hi
            4'hB: slv_rdata = 8'd0;
            4'hC: slv_rdata = 8'd0;   // SDR
            4'hD: slv_rdata = {int_o, 2'd0, icr_pending};
            4'hE: slv_rdata = cra;
            4'hF: slv_rdata = crb;
            default: slv_rdata = 8'd0;
        endcase
    end

    // ---------------- Sequential -----------------------
    reg ta_just_underflowed;
    reg tb_just_underflowed;
    reg icr_read_clear;          // strobe: clear pending on read of ICR

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pra <= 8'd0; prb <= 8'd0;
            ddra <= 8'd0; ddrb <= 8'd0;
            ta_lo_reload <= 8'hFF; ta_hi_reload <= 8'hFF;
            tb_lo_reload <= 8'hFF; tb_hi_reload <= 8'hFF;
            ta_cnt <= 16'hFFFF; tb_cnt <= 16'hFFFF;
            cra <= 8'd0; crb <= 8'd0;
            icr_pending <= 5'd0;
            icr_mask <= 5'd0;
            ta_just_underflowed <= 1'b0;
            tb_just_underflowed <= 1'b0;
            icr_read_clear <= 1'b0;
            tod <= 24'd0;
            tod_prescale <= 10'd0;
        end else begin
            // TOD tick (free-running 50 Hz-ish counter).
            if (tod_prescale == 10'd1023) begin
                tod_prescale <= 10'd0;
                tod          <= tod + 24'd1;
            end else begin
                tod_prescale <= tod_prescale + 10'd1;
            end
            ta_just_underflowed <= 1'b0;
            tb_just_underflowed <= 1'b0;
            icr_read_clear <= 1'b0;

            // ---- Slave writes ----
            if (slv_req && slv_we) begin
                case (slv_addr)
                    4'h0: pra <= slv_wdata;
                    4'h1: prb <= slv_wdata;
                    4'h2: ddra <= slv_wdata;
                    4'h3: ddrb <= slv_wdata;
                    4'h4: ta_lo_reload <= slv_wdata;
                    4'h5: begin
                        ta_hi_reload <= slv_wdata;
                        // Writing TAHI in one-shot mode also reloads the counter
                        // and starts the timer (real CIA quirk).
                        if (ta_runmode) begin
                            ta_cnt <= {slv_wdata, ta_lo_reload};
                            cra[0] <= 1'b1;
                        end
                    end
                    4'h6: tb_lo_reload <= slv_wdata;
                    4'h7: begin
                        tb_hi_reload <= slv_wdata;
                        if (tb_runmode) begin
                            tb_cnt <= {slv_wdata, tb_lo_reload};
                            crb[0] <= 1'b1;
                        end
                    end
                    4'hD: begin
                        // ICR write: bit 7 = SET/CLR, bits 4:0 = which bits.
                        if (slv_wdata[7])
                            icr_mask <= icr_mask |  slv_wdata[4:0];
                        else
                            icr_mask <= icr_mask & ~slv_wdata[4:0];
                    end
                    4'hE: begin
                        cra <= slv_wdata;
                        // LOAD strobe: copy reload -> counter.
                        if (slv_wdata[4])
                            ta_cnt <= {ta_hi_reload, ta_lo_reload};
                    end
                    4'hF: begin
                        crb <= slv_wdata;
                        if (slv_wdata[4])
                            tb_cnt <= {tb_hi_reload, tb_lo_reload};
                    end
                    default: ;
                endcase
            end

            // ---- Slave reads: clear pending on ICR read ----
            if (slv_req && !slv_we && slv_addr == 4'hD)
                icr_read_clear <= 1'b1;

            // ---- Timer A countdown ----
            if (ta_start && tick) begin
                if (ta_cnt == 16'd0) begin
                    // Underflow: reload, set IRQ pending, maybe stop (one-shot).
                    ta_cnt <= {ta_hi_reload, ta_lo_reload};
                    ta_just_underflowed <= 1'b1;
                    if (ta_runmode) cra[0] <= 1'b0;     // one-shot stops
                end else begin
                    ta_cnt <= ta_cnt - 16'd1;
                end
            end
            if (ta_just_underflowed)
                icr_pending[0] <= 1'b1;

            // ---- Timer B countdown ----
            // (Real CIA can chain TB to TA underflows; we keep it simple.)
            if (tb_start && tick) begin
                if (tb_cnt == 16'd0) begin
                    tb_cnt <= {tb_hi_reload, tb_lo_reload};
                    tb_just_underflowed <= 1'b1;
                    if (tb_runmode) crb[0] <= 1'b0;
                end else begin
                    tb_cnt <= tb_cnt - 16'd1;
                end
            end
            if (tb_just_underflowed)
                icr_pending[1] <= 1'b1;

            // ICR read clears pending bits (real CIA "read and clear").
            if (icr_read_clear)
                icr_pending <= 5'd0;
        end
    end
endmodule
