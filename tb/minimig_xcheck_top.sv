// Cross-validation top-level for MiniMig vs our Paula.
//
// Hosts two DUTs side-by-side:
//
//   1. mm_int : the official MiniMig paula_intcontroller from
//               external/minimig/rtl/minimig/paula_intcontroller.v
//   2. ours_int : a thin wrapper around our paula.v that exposes the
//                 same {address, write-data, read-data} contract on the
//                 INTENA / INTREQ / INTENAR / INTREQR registers.
//
// The C++ harness (tb/minimig_xcheck.cpp) drives a sequence of register
// writes into both at once and checks that INTENAR | INTREQR reads back
// identical from each.
//
// We tie off all interrupt source pins (rxint/txint/vblint/etc.) on
// both DUTs since the test is comparing only the SET/CLR register
// semantics, not the live IRQ wiring.

module minimig_xcheck_top (
    input  wire        clk,
    input  wire        rst,
    input  wire        we,             // 1 = write, 0 = read
    input  wire [8:1]  reg_addr,       // [8:1] of register address
    input  wire [15:0] wdata,
    output wire [15:0] mm_rdata,
    output wire [15:0] our_rdata
);

    // ---------------- MiniMig ----------------
    wire [2:0] mm_ipl_unused;
    wire [3:0] mm_audpen_unused;
    wire       mm_rbfmirror_unused;

    paula_intcontroller mm_int (
        .clk            (clk),
        .clk7_en        (1'b1),
        .reset          (rst),
        .reg_address_in (reg_addr),
        .data_in        (wdata),
        .data_out       (mm_rdata),
        .rxint          (1'b0),
        .txint          (1'b0),
        .vblint         (1'b0),
        .int2           (1'b0),
        .int3           (1'b0),
        .int6           (1'b0),
        .blckint        (1'b0),
        .syncint        (1'b0),
        .audint         (4'b0),
        .audpen         (mm_audpen_unused),
        .rbfmirror      (mm_rbfmirror_unused),
        ._ipl           (mm_ipl_unused)
    );

    // ---------------- Ours ----------------
    // Our paula's slv interface is byte-aligned and 32-bit; map the
    // MiniMig 8-bit-aligned 16-bit register interface onto it.
    //
    // MiniMig reg_addr_in is [8:1] (so 7 bits = 128 distinct word
    // addresses, byte address = reg_addr*2).  Our paula expects an
    // 8-bit byte offset.  reg_addr_in == 8'h4D (= INTENA's [8:1]) maps
    // to byte offset 8'h9A.  We left-shift by 1 to get the byte
    // address.
    wire [7:0] our_slv_addr = {reg_addr[8:1], 1'b0};
    reg  [31:0] our_slv_wdata;
    wire [31:0] our_slv_rdata;
    wire        our_slv_req = 1'b1;

    // The MiniMig int controller takes 16-bit data in low half; our
    // Paula reads its writes via slv_be byte enables.  For 16-bit
    // writes we put the value in the *high* half (high addr byte =
    // [31:24], for big-endian word access at even address) and use
    // be[3:2] = 11.
    //
    // Our paula's slv_addr is the FULL byte address (it matches
    // 8'h9A, 8'h9C, 8'h1C, 8'h1E directly).  Pass it verbatim from
    // MiniMig's reg_addr.  For a 16-bit word at offset N our bus
    // packs the data either in the high half (N % 4 == 0) or the
    // low half (N % 4 == 2).
    wire [7:0] our_word_addr = {reg_addr[8:1], 1'b0};
    wire       upper_half    = (our_word_addr[1:0] == 2'b00);
    wire [3:0] our_be        = upper_half ? 4'b1100 : 4'b0011;
    always @* begin
        our_slv_wdata = upper_half ? {wdata, 16'h0000}
                                   : {16'h0000, wdata};
    end

    // Audio output / IRQ unused for this cross-check.
    wire signed [15:0] our_aud_l, our_aud_r;
    wire        [2:0]  our_ipl;

    paula our_int (
        .clk          (clk),
        .rst_n        (~rst),
        .slv_req      (1'b1),
        .slv_we       (we),
        .slv_addr     (our_word_addr),
        .slv_be       (our_be),
        .slv_wdata    (our_slv_wdata),
        .slv_rdata    (our_slv_rdata),
        .mst_req      (),
        .mst_we       (),
        .mst_addr     (),
        .mst_wdata    (),
        .mst_be       (),
        .mst_ack      (1'b0),
        .mst_rdata    (32'd0),
        .audio_l_o    (our_aud_l),
        .audio_r_o    (our_aud_r),
        .cia_a_int_i  (1'b0),
        .cia_b_int_i  (1'b0),
        .blt_int_i    (1'b0),
        .cop_int_i    (1'b0),
        .vblank_int_i (1'b0),
        .irq_level_o  (our_ipl)
    );
    // For read of INTENAR/INTREQR ($1C/$1E in MiniMig's [8:1] view),
    // our paula word at offset 0x1C holds {INTENAR, INTREQR}.  Both
    // halves are byte-packed; MiniMig combines them as INTENAR|INTREQR.
    // Pick the half corresponding to reg_addr.
    assign our_rdata = upper_half ? our_slv_rdata[31:16]
                                  : our_slv_rdata[15:0];

endmodule
