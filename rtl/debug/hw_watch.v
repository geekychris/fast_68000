// Reusable address-range watchpoint.
//
// Instantiate at any port where you'd otherwise hand-write
//   `if (addr >= LO && addr <= HI && we) $display("[LABEL] ...")`.
//
// Parameters
//   LABEL        - short tag printed on every hit; pick something
//                  unique per call site (e.g., "VEC-1B", "C2CC-WR").
//   ADDR_LO/HI   - inclusive byte-address range to match.  Set LO==HI
//                  for an exact-address watchpoint.
//   MATCH_WE     - 1 = fire on writes.
//   MATCH_RE     - 1 = fire on reads.
//   STOP_ON_HIT  - 1 = $finish after first matching hit (cheap
//                  substitute for a real CPU breakpoint).
//   PRINT_DATA   - 1 = include wdata/be in the print (off for read
//                  watchpoints where wdata is meaningless).
//
// Inputs are the "natural" master/slave port wires.  Pass 0 for
// fields the caller doesn't have (e.g., src_id, pc, retired).
//
// hit_o pulses high for one cycle on a match — useful to OR into a
// single halt line for a future debug controller.  Several
// watchpoints can share one halt-aggregator.
//
// Future-hardware shape: if this ever tapes out, the parameters
// become JTAG-writable registers, MATCH_WE/RE become per-watch
// runtime bits, and hit_o aggregates into an event FIFO.  The
// in-sim shape mirrors that — same wires, same semantics.

module hw_watch #(
    parameter        LABEL       = "WATCH",
    parameter [31:0] ADDR_LO     = 32'h0000_0000,
    parameter [31:0] ADDR_HI     = 32'h0000_0000,
    parameter        MATCH_WE    = 1,
    parameter        MATCH_RE    = 0,
    parameter        STOP_ON_HIT = 0,
    parameter        PRINT_DATA  = 1
) (
    input  wire        clk,
    input  wire        rst_n,
    input  wire        valid,
    input  wire        we,
    input  wire [31:0] addr,
    input  wire [31:0] wdata,
    input  wire [3:0]  be,
    input  wire [7:0]  src_id,
    input  wire [31:0] pc,
    input  wire [31:0] retired,
    output reg         hit_o
);
    wire in_range = (addr >= ADDR_LO) && (addr <= ADDR_HI);
    wire dir_ok   = (we && (MATCH_WE != 0)) || (!we && (MATCH_RE != 0));
    wire hit_w    = valid && in_range && dir_ok;

    always @(posedge clk) begin
        if (!rst_n) hit_o <= 1'b0;
        else begin
            hit_o <= hit_w;
            if (hit_w) begin
                if (PRINT_DATA != 0)
                    $display("[%s] r=%0d src=%0d pc=%h we=%b addr=%h wdata=%h be=%b",
                        LABEL, retired, src_id, pc, we, addr, wdata, be);
                else
                    $display("[%s] r=%0d src=%0d pc=%h re addr=%h",
                        LABEL, retired, src_id, pc, addr);
                if (STOP_ON_HIT != 0) $finish;
            end
        end
    end
endmodule
