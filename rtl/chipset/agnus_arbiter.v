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
`else
    wire is_refresh_slot = 1'b0;
    wire is_disk_slot    = 1'b0;
    wire is_audio_slot   = 1'b0;
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
