// Bus arbiter — Phase A of #4 (slot-accurate Agnus arbiter).
//
// This is the SAME round-robin priority arbiter that currently lives
// inline in `rtl/m68k_bus.v:776-803, 2113-2130`, extracted into its
// own module without behavior change.  Phase A's goal is purely
// refactoring so subsequent phases (B-F per docs/SLOT_ACCURATE_AGNUS_PLAN.md)
// can replace this with a slot-position-aware version without
// disturbing the surrounding bus logic.
//
// Semantics:
//   - Combinational `winner` selection from `req[]`, rotated by `rrobin`.
//   - `lock[]` per-port marks "this txn needs a lock" (RMW or
//     unaligned .L second-half).  Lock is single-cycle: granted once,
//     then released on the next winning grant.
//   - On grant, rrobin advances to (winner+1) mod N_PORTS UNLESS a
//     lock is being entered, in which case rrobin stays put so the
//     same port re-wins on the next cycle.

module agnus_arbiter #(
    parameter N_PORTS  = 4,
    parameter PID_BITS = 2,
    // Phase C: ID of the disk DMA port (Paula master in m68k_top
    // layout, = N_PORTS - 1 for the K1.3 build).
    parameter DSK_PORT_ID = 3
) (
    input  wire                       clk,
    input  wire                       rst_n,
    input  wire [N_PORTS-1:0]         req,
    input  wire [N_PORTS-1:0]         lock,
    // Beam position from Agnus.  Used by Phase B+ to deny grants on
    // refresh slots (and later disk/audio/sprite/bitplane slots).  In
    // Phase A this was unused.  Tie to 0 if the caller doesn't have
    // it yet — refresh reservation is gated by SLOT_ACCURATE_AGNUS.
    input  wire [9:0]                 hpos,
    // Phase C: disk DMA is active (DSKEN + DMAEN + DSKLEN != 0).
    // When high, cycles 7/9/11 of each line are reserved for the disk
    // port.  When low, those cycles are free.
    input  wire                       dsk_active,
    // Phase D: per-channel audio enables (DMACON bits 0-3).  Gated by
    // DMAEN (caller responsibility — pass 0 if master DMAEN is off).
    // When `audN_en`, cycle (13 + 2*N) is reserved for that audio
    // channel.  The simulated audio path lives inside paula.v and
    // doesn't use a port number here, same idea as disk DMA:
    // reservation simply denies grants without granting them to
    // another port.
    //
    // Sprite slot reservation is deferred to Phase E because it
    // requires per-sprite vstart/vstop state to be fidelity-correct.
    // SPREN alone over-reserves: real Agnus only fetches sprites on
    // lines where a sprite is active, but our previous attempt to
    // approximate via "visible lines" still over-reserved by ~10× for
    // typical OS use (1-sprite mouse pointer).
    input  wire [3:0]                 audn_en,
    // Phase E (coarse approximation): bitplane DMA reservation.
    // `bpl_active` is BPLEN & DMAEN & BPU>0 (caller responsibility);
    // `vpos`, `bpu`, and DDFSTRT/DDFSTOP shadows let the arbiter deny
    // CPU on visible lines during the bitplane fetch window.  Coarse
    // because we don't model the exact slot-within-8-cycle-group
    // pattern, just "every Nth cycle" where N is derived from BPU
    // and HIRES.  Phase E proper would model the precise pattern.
    input  wire                       bpl_active,
    input  wire [9:0]                 vpos,
    input  wire [2:0]                 bpu,
    input  wire                       hires,
    input  wire [7:0]                 ddfstrt,
    input  wire [7:0]                 ddfstop,
    output reg  [PID_BITS-1:0]        winner,
    output reg                        winner_valid,
    output reg  [N_PORTS-1:0]         grant
);

    // ---------------- Refresh-slot reservation (Phase B) ----------------
    // Real Agnus reserves cycles 0, 2, 4, 6 of every line for DRAM
    // refresh.  CPU + DMA channels are denied during those cycles.
    // Our memory model doesn't need refresh, but reserving the slots
    // shifts CPU timing to match real-hardware cadence.  Gated behind
    // a `define so Phase A and earlier callers see no change.
`ifdef SLOT_ACCURATE_AGNUS
    // Real Agnus refreshes DRAM on cycles 0, 2, 4, 6 (4 even cycles
    // per line, per HRM appendix C).  Earlier draft reserved hpos 0..7
    // which is 2× the correct slot count; fixed here.
    wire is_refresh_slot = !hpos[0] && (hpos[9:1] < 9'd4);
    // Phase C: disk DMA owns cycles 7, 9, 11 (one CCK each) when active.
    wire is_disk_slot = dsk_active &&
                        ( (hpos[9:0] == 10'd7)  ||
                          (hpos[9:0] == 10'd9)  ||
                          (hpos[9:0] == 10'd11) );
    // Phase D: audio DMA owns cycles 13/15/17/19 (per HRM appendix C).
    // AUDx fetch slot only reserved when channel enabled.
    wire is_audio_slot = (audn_en[0] && (hpos[9:0] == 10'd13)) ||
                         (audn_en[1] && (hpos[9:0] == 10'd15)) ||
                         (audn_en[2] && (hpos[9:0] == 10'd17)) ||
                         (audn_en[3] && (hpos[9:0] == 10'd19));
    // Phase F (partial): E-clock peripheral slot at hpos $E0 = 224.
    // Real Agnus reserves this cycle for 6800-bus-compatible peripheral
    // access (CIAs, anything on the E-clock).  Single slot per line.
    // Independent of any DMACON bit — reservation is always-on under
    // SLOT_ACCURATE_AGNUS.
    wire is_eclk_slot = (hpos[9:0] == 10'd224);

    // Phase E (coarse): bitplane DMA reservation during visible-line
    // active fetch window.  Real Agnus reserves N slots in each
    // 8-cycle group within DDFSTRT..DDFSTOP, where N depends on BPU
    // and HIRES.  We approximate:
    //   - visible line if 26 <= vpos <= 308 (PAL)
    //   - active fetch window if (ddfstrt*2) <= hpos <= (ddfstop*2)+8
    //   - reservation pattern by BPU/HIRES based on hpos[2:0]
    // This is coarser than HRM appendix C but gives the dominant
    // CPU-stall effect (~40% loss during display) that determines
    // K1.3 boot pacing.
    wire is_visible_line = (vpos >= 10'd26) && (vpos < 10'd309);
    wire [9:0] ddfstrt_h = {2'd0, ddfstrt[7:0]} << 1;  // DDFSTRT in raw hpos
    wire [9:0] ddfstop_h = {2'd0, ddfstop[7:0]} << 1;
    wire in_fetch_window = (hpos >= ddfstrt_h) && (hpos <= ddfstop_h + 10'd8);
    // Per-cycle reservation pattern in lowres (8-cycle group):
    //   BPU=1: hpos[2:0] == 4 reserved
    //   BPU=2: hpos[2:0] in {2,6} reserved
    //   BPU=3: hpos[2:0] in {2,4,6} reserved
    //   BPU=4: hpos[2:0] in {0,2,4,6} reserved (every even cycle)
    //   BPU=5+: same as BPU=4 (lowres caps at 4 planes in HRM)
    // Hires (which we don't reach in K1.3) would use a tighter 4-cycle
    // group pattern — same coarse model, halved interval.
    wire [2:0] hs = hires ? {1'b0, hpos[1:0]} : hpos[2:0];
    wire bpl_pattern = !hs[0] &&  // always on an even cycle
                       ( (bpu >= 3'd4) ? 1'b1 :
                         (bpu == 3'd3) ? (hs[2:1] != 2'b00) :
                         (bpu == 3'd2) ? (hs[2:1] == 2'b01 || hs[2:1] == 2'b11) :
                         (bpu == 3'd1) ? (hs[2:1] == 2'b10) :
                                         1'b0 );
    wire is_bitplane_slot = bpl_active && is_visible_line &&
                            in_fetch_window && bpl_pattern;
`else
    wire is_refresh_slot   = 1'b0;
    wire is_disk_slot      = 1'b0;
    wire is_audio_slot     = 1'b0;
    wire is_eclk_slot      = 1'b0;
    wire is_bitplane_slot  = 1'b0;
`endif

    // ---------------- Internal state ----------------
    reg                  lock_pending;
    reg [PID_BITS-1:0]   lock_holder;
    reg [PID_BITS-1:0]   rrobin;

    // ---------------- Combinational winner selection ----------------
    integer            k;
    reg [PID_BITS:0]   cand_ext;     // one extra bit so + cannot wrap silently
    reg [PID_BITS-1:0] cand;

    always @* begin
        winner = {PID_BITS{1'b0}};
        winner_valid = 1'b0;
        cand = {PID_BITS{1'b0}};
        cand_ext = {(PID_BITS+1){1'b0}};
        if (is_refresh_slot) begin
            // Phase B: deny all grants on refresh slots regardless of
            // pending lock.  A lock_holder will re-win as soon as the
            // next non-refresh cycle arrives.
            winner = {PID_BITS{1'b0}};
            winner_valid = 1'b0;
        end else if (is_disk_slot) begin
            // Phase C: disk DMA owns this slot.  In our impl the disk
            // DMA state machine lives inside m68k_bus.v (not behind a
            // port), so we simply reserve the slot from every
            // requester.  Real effect: CPU+DMA pause at these cycles
            // when disk DMA is active, shifting CPU timing relative
            // to the internal MFM-decode state machine.  That's the
            // hypothesis test for the cyl-53 trackdisk-rejection wall.
            winner = {PID_BITS{1'b0}};
            winner_valid = 1'b0;
        end else if (is_audio_slot) begin
            // Phase D: audio DMA reservation.  Like disk, audio
            // doesn't sit behind a bus port in our chipset, so we
            // simply deny all requesters.  CPU+blitter+copper pause
            // on these cycles when the corresponding AUDxEN+DMAEN is
            // high, recreating real-Agnus stalls.
            winner = {PID_BITS{1'b0}};
            winner_valid = 1'b0;
        end else if (is_eclk_slot) begin
            // Phase F (partial): E-clock peripheral access slot at
            // hpos $E0.  Reserved unconditionally — like refresh, it
            // happens every line.  Denies all requesters; the slot
            // gives the CIAs an exclusive bus moment matching real
            // 6800-compatible cycle behavior.
            winner = {PID_BITS{1'b0}};
            winner_valid = 1'b0;
        end else if (is_bitplane_slot) begin
            // Phase E (coarse): bitplane DMA reservation.  When BPLEN
            // is on and we're inside the visible display window, the
            // bitplane fetcher owns a fraction of cycles (varies with
            // BPU / HIRES).  Approximated by hpos[2:0]-pattern match
            // against BPU.  See is_bitplane_slot definition above.
            winner = {PID_BITS{1'b0}};
            winner_valid = 1'b0;
        end else if (lock_pending) begin
            winner = lock_holder;
            winner_valid = req[lock_holder];
        end else begin
            for (k = 0; k < N_PORTS; k = k + 1) begin
                cand_ext = {1'b0, rrobin} + k[PID_BITS:0];
                cand = (cand_ext >= N_PORTS[PID_BITS:0])
                       ? cand_ext[PID_BITS-1:0] - N_PORTS[PID_BITS-1:0]
                       : cand_ext[PID_BITS-1:0];
                if (!winner_valid && req[cand]) begin
                    winner = cand;
                    winner_valid = 1'b1;
                end
            end
        end
    end

    // ---------------- Grant output ----------------
    integer pp;
    always @* begin
        grant = {N_PORTS{1'b0}};
        if (winner_valid) grant[winner] = 1'b1;
        pp = 0;  // unused, silences VL2008 strictness
    end

    // ---------------- State updates ----------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            lock_pending <= 1'b0;
            lock_holder  <= {PID_BITS{1'b0}};
            rrobin       <= {PID_BITS{1'b0}};
        end else begin
            // Round-robin advance.
            if (winner_valid && !lock_pending) begin
                if (winner == (N_PORTS-1))
                    rrobin <= {PID_BITS{1'b0}};
                else
                    rrobin <= winner + 1'b1;
            end

            // Lock update.
            if (winner_valid) begin
                if (lock_pending) begin
                    // Granted second txn of an RMW; release lock.
                    lock_pending <= 1'b0;
                end else if (lock[winner]) begin
                    lock_pending <= 1'b1;
                    lock_holder  <= winner;
                end
            end
        end
    end

endmodule
