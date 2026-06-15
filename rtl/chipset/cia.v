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

    // Keyboard (or any other serial) byte inject.  Assert kbd_wr for
    // one cycle along with kbd_byte; the byte lands in SDR and bit 3
    // of ICR (SP-flag = "serial complete") goes pending, raising the
    // CIA interrupt.  Reading SDR (register $C) clears the pending bit
    // via the normal ICR-read-clear path.
    input  wire        kbd_wr,
    input  wire [7:0]  kbd_byte,

    // Port A / B inputs (peripheral side).
    input  wire [7:0]  pa_in,
    input  wire [7:0]  pb_in,

    // Port A / B outputs (driven by PRA/PRB & DDR).
    output wire [7:0]  pa_out,
    output wire [7:0]  pa_oe,            // 1 = driven by this CIA
    output wire [7:0]  pb_out,
    output wire [7:0]  pb_oe,

    // TOD increment trigger.  Real CIA-A's TOD is wired to Agnus VSYNC
    // (50 Hz PAL / 60 Hz NTSC).  Drive this with the system VBL pulse so
    // K1.3's timer.device VBL handler sees TOD advance by exactly 1 per
    // VBL — without it, our previous 1024-cycle prescaler ticked TOD
    // ~70x faster than VBL and timer.device's request-completion math
    // never resolved, leaving input.device wedged in WAIT and CLI never
    // unblocking.  When tod_tick_i is tied off in synth, the legacy
    // prescaler still ticks TOD as a fallback.
    input  wire        tod_tick_i,

    // Combined IRQ output: high for one host clock when any enabled
    // pending bit transitions 0->1 (per-source rising-edge pulse).
    //
    // Real CIA's INT pin is level-sensitive while pending bits remain
    // set, but downstream Paula uses *edge* detection (cia_a_int_i &
    // ~cia_a_int_last).  With a pure level output a TA underflow would
    // raise int_o, and a subsequent SP byte arrival (icr_pending[3]
    // <= 1) wouldn't change int_o because it was already high -- so
    // Paula never latched a fresh PORTS IRQ and CLI/keyboard.device
    // never saw the keystroke.  Pulsing per source restores the
    // expected "new event = new IRQ" semantics regardless of whether
    // the prior handler read ICR.  See project_wb13_cli_wait.md.
    output reg         int_o
);
    // ---------------- Register storage ------------------
    reg [7:0]  pra, prb, ddra, ddrb;
    reg [7:0]  ta_lo_reload, ta_hi_reload;
    reg [7:0]  tb_lo_reload, tb_hi_reload;
    reg [15:0] ta_cnt, tb_cnt;
    reg [7:0]  cra, crb;
    // Serial data register.  Receives kbd_byte; reads clear the SP
    // interrupt source via the icr_read_clear path.
    reg [7:0]  sdr;
    reg [4:0]  icr_pending;          // bits 0=TA, 1=TB, 3=SP
    reg [4:0]  icr_mask;

    // ---------------- TOD: 24-bit time-of-day -----------------------
    // Real Amiga drives TOD from Agnus VSYNC (50 Hz PAL / 60 Hz NTSC).
    // We approximate it with a fixed prescaler so any code polling TOD
    // sees it advance.  TOD_PRESCALER=1024 gives plenty of resolution
    // for boot-time clock reads without slowing things down.
    reg [23:0] tod;
    reg [9:0]  tod_prescale;
    reg        tod_tick_seen;
    // 8520 CIA TOD semantics (per the chip's datasheet):
    //   - Writing TODHI stops the TOD counter (real 8520 quirk; staging mode).
    //   - Writing TODMID is buffered.
    //   - Writing TODLO restarts the counter and atomically commits the
    //     {TODHI, TODMID, TODLO} buffer to the live TOD register (or to
    //     the ALARM register when CRB[7]=1).
    //   - Reading TODHI latches all three TOD bytes into a side register;
    //     subsequent reads of TODMID/TODLO return the latched values until
    //     the matching TODLO read releases the latch.  This avoids the
    //     read-tear hazard while TOD is incrementing.
    //   - When live TOD matches the ALARM register, the CIA asserts
    //     ICR bit 2 (ALARM source).
    // Previously our impl ignored TOD writes (silently dropped), didn't
    // latch on read, and didn't generate ALARM interrupts.  K1.3
    // timer.device uses CIA-A TOD ALARM for EClock sub-second wakeups —
    // missing alarms shift the interrupt pattern and downstream
    // allocation order.
    reg [23:0] tod_alarm;
    reg [23:0] tod_writebuf;   // staged {TODHI, TODMID, TODLO}
    reg        tod_writepend;  // 1 = TODHI/MID written; awaiting TODLO commit
    reg        tod_stopped;    // TODHI write halts counter until TODLO commit
    reg [23:0] tod_latch;
    reg        tod_latched;
    reg [23:0] tod_prev;       // for alarm-match edge detection

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

    // Interrupt line — LEVEL-TRIGGERED (mirrors minimig cia_int.v line 65):
    //
    //   irq = |(icr_mask & icr_pending)
    //
    // Stays HIGH while any unmasked pending bit is set; goes LOW when
    // the CPU clears icr_pending via ICR read (or when mask gets cleared).
    //
    // Replaces an earlier pulse-on-rising-edge implementation that fired
    // int_o for ONE cycle on (pending|mask) rising edges.  That pulse
    // could be missed by CPU's interrupt sampling if the CPU happened
    // to be deep in a multi-cycle instruction at the moment of the pulse;
    // the symptom in Turrican post-fire was the INT2 keyboard handler
    // never reading icr_pending[3] as set even though kbd_wr asserted
    // it.  Level-triggered behaviour matches both real CIA silicon and
    // the minimig reference; the rising-edge cases (keyboard-byte-
    // arrives-before-mask, timer-underflow-after-mask) are naturally
    // covered because the AND product stays high until cleared.
    always @* begin
        int_o = |(icr_mask & icr_pending);
    end

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
            4'h8: slv_rdata = tod_latched ? tod_latch[7:0]   : tod[7:0];   // TOD low (unlatches)
            4'h9: slv_rdata = tod_latched ? tod_latch[15:8]  : tod[15:8];  // TOD mid
            4'hA: slv_rdata = tod_latched ? tod_latch[23:16] : tod[23:16]; // TOD hi (latches on read)
            4'hB: slv_rdata = 8'd0;
            4'hC: slv_rdata = sdr;
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
            tod_tick_seen <= 1'b0;
            tod_alarm     <= 24'd0;
            tod_writebuf  <= 24'd0;
            tod_writepend <= 1'b0;
            tod_stopped   <= 1'b0;
            tod_latch     <= 24'd0;
            tod_latched   <= 1'b0;
            tod_prev      <= 24'd0;
            sdr <= 8'd0;
        end else begin
            // Serial byte arrives from external (keyboard scancode).
            if (kbd_wr) begin
                sdr <= kbd_byte;
                icr_pending[3] <= 1'b1;       // SP interrupt source
`ifdef KICKSTART_BOOT_TRACE
                $display("[CIA-KBD] kbd_wr=1 byte=%h icr_mask=%h icr_pending_before=%h",
                    kbd_byte, icr_mask, icr_pending);
`endif
            end
            // TOD increment.  Once any external VSYNC tick arrives we
            // switch to "external mode" permanently and ignore the
            // prescaler (otherwise the prescaler would also overflow
            // ~70 times per VBL and re-introduce the fast-TOD bug).
            // Until the first tick, the prescaler ticks TOD as a
            // fallback so harnesses that don't drive tod_tick_i still
            // see TOD advance.  Counter is gated on !tod_stopped so
            // writes-in-progress (TODHI without matching TODLO commit)
            // pause the counter exactly like the real 8520.
            tod_prev <= tod;
            if (!tod_stopped) begin
                if (tod_tick_i) begin
                    tod                <= tod_tick_seen ? (tod + 24'd1) : 24'd1;
                    tod_prescale       <= 10'd0;
                    tod_tick_seen      <= 1'b1;
                end else if (!tod_tick_seen) begin
                    if (tod_prescale == 10'd1023) begin
                        tod_prescale <= 10'd0;
                        tod          <= tod + 24'd1;
                    end else begin
                        tod_prescale <= tod_prescale + 10'd1;
                    end
                end
            end
            // ALARM match: when TOD transitions INTO equality with alarm
            // (edge), assert ICR bit 2 (the ALARM source).  Edge check
            // avoids re-asserting every cycle the values stay equal.
            // Alarm value 0 isn't special on real silicon, but it's the
            // power-on default — we still need to fire if K1.3 set
            // alarm=0 deliberately.
            if (tod == tod_alarm && tod_prev != tod_alarm)
                icr_pending[2] <= 1'b1;
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
                    // TOD writes — real 8520 semantics:
                    //   * Write TODHI ($A) stops the counter, stages the hi byte.
                    //   * Write TODMID ($9) stages the mid byte.
                    //   * Write TODLO ($8) atomically commits {hi:mid:lo}
                    //     into either TOD (CRB[7]=0) or ALARM (CRB[7]=1),
                    //     then restarts the counter.
                    4'h8: begin
                        if (crb[7]) begin
                            tod_alarm <= {tod_writebuf[23:8], slv_wdata};
                        end else begin
                            tod <= {tod_writebuf[23:8], slv_wdata};
                        end
                        tod_writepend <= 1'b0;
                        tod_stopped   <= 1'b0;
                    end
                    4'h9: tod_writebuf[15:8]  <= slv_wdata;
                    4'hA: begin
                        tod_writebuf[23:16] <= slv_wdata;
                        // Only stop the live counter when targeting TOD
                        // (CRB[7]=0); writing the ALARM should not pause TOD.
                        if (!crb[7]) tod_stopped <= 1'b1;
                        tod_writepend <= 1'b1;
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
            // ---- TOD read-latch: $A latches, $8 unlatches ----
            // Real 8520 latches all three TOD bytes when TODHI is read,
            // and releases the latch when TODLO is read.  This gives the
            // CPU a stable snapshot across the three byte reads, even if
            // TOD increments in between.
            if (slv_req && !slv_we && slv_addr == 4'hA) begin
                tod_latch   <= tod;
                tod_latched <= 1'b1;
            end else if (slv_req && !slv_we && slv_addr == 4'h8) begin
                tod_latched <= 1'b0;
            end

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
