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
    wire is_refresh_slot = (hpos[8:1] == 8'd0) ||   // hpos = 0 or 1
                           (hpos[8:1] == 8'd1) ||   // hpos = 2 or 3
                           (hpos[8:1] == 8'd2) ||   // hpos = 4 or 5
                           (hpos[8:1] == 8'd3);     // hpos = 6 or 7
    // Phase C: disk DMA owns cycles 7, 9, 11 (one CCK each) when active.
    wire is_disk_slot = dsk_active &&
                        ( (hpos[9:0] == 10'd7)  ||
                          (hpos[9:0] == 10'd9)  ||
                          (hpos[9:0] == 10'd11) );
`else
    wire is_refresh_slot = 1'b0;
    wire is_disk_slot    = 1'b0;
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
