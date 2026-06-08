// Unit-test harness for rtl/chipset/agnus_arbiter.v.
//
// Wraps the arbiter with simple top-level ports so a C++ harness in
// tb/agnus_arbiter_xcheck.cpp can drive req/lock/hpos/dsk_active/
// audn_en and read back winner/winner_valid/grant.  Compiled with
// SLOT_ACCURATE_AGNUS active so Phases B/C/D slot reservation paths
// are exercised.
//
// Lives under tb/ alongside the minimig cross-check harnesses so the
// project's existing Verilator build pattern works unchanged.

module agnus_arbiter_xcheck_top #(
    parameter N_PORTS  = 4,
    parameter PID_BITS = 2,
    parameter DSK_PORT_ID = 3
) (
    input  wire                       clk,
    input  wire                       rst_n,
    input  wire [N_PORTS-1:0]         req,
    input  wire [N_PORTS-1:0]         lock,
    input  wire [9:0]                 hpos,
    input  wire                       dsk_active,
    input  wire [3:0]                 audn_en,
    output wire [PID_BITS-1:0]        winner,
    output wire                       winner_valid,
    output wire [N_PORTS-1:0]         grant
);
    agnus_arbiter #(
        .N_PORTS     (N_PORTS),
        .PID_BITS    (PID_BITS),
        .DSK_PORT_ID (DSK_PORT_ID)
    ) u_arb (
        .clk          (clk),
        .rst_n        (rst_n),
        .req          (req),
        .lock         (lock),
        .hpos         (hpos),
        .dsk_active   (dsk_active),
        .audn_en      (audn_en),
        .winner       (winner),
        .winner_valid (winner_valid),
        .grant        (grant)
    );
endmodule
