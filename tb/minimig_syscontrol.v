// Testbench override of external/minimig/rtl/minimig/minimig_syscontrol.v (V word above is verboten).
//
// Original gates `reset` low only after 4 `sof` (start-of-frame) pulses
// arrive on the `cnt` input.  In the FPGA target, `sof` is generated
// by Agnus's beam counter once clocks are running.  In our Verilator
// harness, the chain from clock-enable → Agnus → sof to syscontrol
// hasn't been bring-up'd yet, so the original syscontrol holds `reset`
// asserted forever and the SRAM bridge never starts cycling.
//
// This bypass instead deasserts `reset` after a fixed delay from
// `mrst` going low, independent of cnt pulses.  Lets us get to the
// next Phase 1b blocker (SRAM bridge protocol / ROM mapping) without
// needing Agnus's beam counter to be running correctly first.
//
// Picked up before minimig's copy via `-y tb` ordered first on the
// command line (V-word verboten).

module minimig_syscontrol
(
    input  wire clk,
    input  wire clk7_en,
    input  wire cnt,        // unused
    input  wire mrst,       // master/user reset input
    output wire reset       // global synchronous system reset (active high)
);

    reg [2:0] smrst;
    reg [3:0] rst_cnt;

    reg reset_was;
    always @(posedge clk) begin
        if (clk7_en) begin
            smrst <= {smrst[1:0], mrst};
            if (smrst[2])              // mrst held → hold reset asserted
                rst_cnt <= 4'd0;
            else if (rst_cnt != 4'd15) // count up until saturated
                rst_cnt <= rst_cnt + 4'd1;
        end
`ifdef PHASE1_PROBE
        if (reset_was != (rst_cnt != 4'd15)) begin
            $display("[probe-sysctrl-override] reset transition: %b -> %b (mrst=%b smrst=%b cnt=%0d)",
                reset_was, (rst_cnt != 4'd15), mrst, smrst, rst_cnt);
        end
        reset_was <= (rst_cnt != 4'd15);
`endif
    end

    assign reset = (rst_cnt != 4'd15);

    wire _unused = &{cnt, 1'b0};

endmodule
