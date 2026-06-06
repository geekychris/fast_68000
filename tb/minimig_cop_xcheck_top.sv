// Cross-validation top for MiniMig copper vs our copper.
//
// PHASE 1 scope: compares the SEQUENCE of MOVE target-register addresses
// each module emits when executing a shared Copper list.
//
// Why phase 1 only: the two coppers have very different beam-position
// inputs (minimig has H+V via vpos/hpos; ours has V-only via vbeam_i)
// and different bus-arbitration models (minimig is Agnus-arbitrated
// with reqdma/ackdma; ours uses a 32-bit master port).  Cycle-perfect
// comparison isn't possible without re-implementing one against the
// other.  But for tests that don't depend on H position — pure MOVE
// chains terminated by a guaranteed-impossible WAIT($FFFE,$FFFE)
// sentinel — both should walk the list in the same order and emit
// MOVEs to the same chipset register addresses in the same order.
// That's what this harness verifies.
//
// Memory layout: 32K words (= 64 KiB) of synthetic chip RAM, shared
// pre-load via init_we.  Both modules see the same Copper list at
// address 0.  Output capture: ring buffer of register-address bytes
// emitted by each side; the harness reads them out after both halt.

module minimig_cop_xcheck_top (
    input  wire        clk,
    input  wire        rst,

    // Memory pre-load.
    input  wire        init_we,
    input  wire [14:0] init_addr,        // word address
    input  wire [15:0] init_wdata,

    // Copper list pointer programming.  Drive cop1lc_w with the
    // BYTE-address of the list, then pulse cop1lc_we for one cycle to
    // load both halves into each copper's location register.
    input  wire        cop1lc_we,
    input  wire [20:0] cop1lc_w,         // byte address (bit 0 ignored)

    // Raster timing.  vpos increments slowly (~1 per row-time worth of
    // cycles); hpos increments per cycle modulo 227.  Used only by
    // minimig — our copper ignores hpos.  vpos drives our vbeam_i too.
    input  wire [7:0]  vpos_in,
    input  wire [8:0]  hpos_in,
    // sof = start of frame.  Pulse high for 1+ cycles to kick both
    // coppers from COP1LC (minimig: latches into copjmp1; ours:
    // auto_kick_i+auto_active_i triggers the restart).
    input  wire        sof_in,

    // Move-write capture readout.  Bit-banged sequential read: pulse
    // mm_pop / our_pop to advance to the next captured byte.  An empty
    // queue returns 8'hFF.
    input  wire        mm_pop,
    input  wire        our_pop,
    output wire [8:0]  mm_count,
    output wire [8:0]  our_count,
    output wire [7:0]  mm_head_reg,
    output wire [7:0]  our_head_reg
);

    // ---------------- Shared synthetic chip RAM ----------------
    reg [15:0] mem [0:32767];

    always @(posedge clk) begin
        if (init_we) mem[init_addr] <= init_wdata;
    end

    // ===================================================================
    // MiniMig copper
    // ===================================================================
    wire        mm_reqdma;
    wire [8:1]  mm_reg_addr_out;
    wire [20:1] mm_address_out;

    // Programming state machine: when cop1lc_we pulses, drive two
    // sequential register writes ($080 = COP1LCH, $082 = COP1LCL)
    // into the minimig copper via its reg_address_in / data_in.
    reg [1:0]  mm_prog_state;
    reg [20:0] mm_prog_lc;
    always @(posedge clk) begin
        if (rst) mm_prog_state <= 2'd0;
        else if (cop1lc_we) begin
            mm_prog_state <= 2'd1;
            mm_prog_lc    <= cop1lc_w;
        end else if (mm_prog_state == 2'd1) mm_prog_state <= 2'd2;
        else if (mm_prog_state == 2'd2) mm_prog_state <= 2'd0;
    end

    // reg_address_in / data_in mux:
    //   state 1: drive COP1LCH ($080 >> 1) + high 5 bits of cop1lc
    //   state 2: drive COP1LCL ($082 >> 1) + low 16 bits of cop1lc
    //   default: echo reg_address_out back so copper sees COPINS
    //            during its own FETCH slots and latches IR1/IR2.
    //            data_in = mem[address_out] (the fetched word).
    reg [8:1]  mm_reg_addr_in;
    reg [15:0] mm_data_in;
    always @* begin
        case (mm_prog_state)
            2'd1: begin
                mm_reg_addr_in = 8'h80 >> 1;
                mm_data_in     = {11'd0, mm_prog_lc[20:16]};
            end
            2'd2: begin
                mm_reg_addr_in = 8'h82 >> 1;
                mm_data_in     = mm_prog_lc[15:0];
            end
            default: begin
                mm_reg_addr_in = mm_reg_addr_out;
                mm_data_in     = mem[mm_address_out[15:1]];
            end
        endcase
    end

    agnus_copper mm_cop (
        .clk            (clk),
        .clk7_en        (1'b1),
        .reset          (rst),
        .ecs            (1'b0),
        .reqdma         (mm_reqdma),
        .ackdma         (mm_reqdma),    // instant ack
        .enadma         (1'b1),
        .sof            (sof_in),
        .blit_busy      (1'b0),
        .vpos           (vpos_in),
        .hpos           (hpos_in),
        .data_in        (mm_data_in),
        .reg_address_in (mm_reg_addr_in),
        .reg_address_out(mm_reg_addr_out),
        .address_out    (mm_address_out)
    );

    // MOVE-EXECUTE capture for minimig.  Heuristic: reg_address_out
    // transitions to a value that is NOT COPINS ($08C >> 1 = $46),
    // NOT $FF (idle marker = $7F when >>1), and NOT one of the LC/JMP
    // programming addresses.  Capture on the rising edge of such a
    // value to avoid double-counting.
    //
    // Minimig presents reg_address_out as 8 bits 8:1 (= chipset offset
    // [8:1]).  The actual byte address is {reg_address_out, 1'b0}.
    // COPINS chipset offset = $08C, observed at reg_address_out[8:1] as
    // $46 (= $08C >> 1).  Dummy cycles present $FF.
    wire mm_reg_is_copins = (mm_reg_addr_out == 8'h46);
    wire mm_reg_is_idle   = (mm_reg_addr_out == 8'hFF);
    wire mm_is_move_exec  = !mm_reg_is_copins && !mm_reg_is_idle;
    reg  [8:1] mm_reg_addr_out_d;
    reg  [7:0] mm_queue [0:255];
    reg  [7:0] mm_head, mm_tail;
    assign mm_count    = {1'b0, (mm_tail - mm_head)};
    assign mm_head_reg = mm_queue[mm_head];
    always @(posedge clk) begin
        if (rst) begin
            mm_head <= 0; mm_tail <= 0;
            mm_reg_addr_out_d <= 0;
        end else begin
            mm_reg_addr_out_d <= mm_reg_addr_out;
            if (mm_is_move_exec && (mm_reg_addr_out != mm_reg_addr_out_d)) begin
                mm_queue[mm_tail] <= {mm_reg_addr_out, 1'b0};
                mm_tail <= mm_tail + 1;
            end
            if (mm_pop && mm_head != mm_tail) mm_head <= mm_head + 1;
        end
    end

    // ===================================================================
    // Our copper
    // ===================================================================
    wire        our_mst_req;
    wire        our_mst_we;
    wire [31:0] our_mst_addr;
    wire [31:0] our_mst_wdata;
    wire [3:0]  our_mst_be;
    reg         our_mst_ack_q;
    reg  [31:0] our_mst_rdata;

    // Our copper does aligned 32-bit fetches with mem[addr]=IR1 in
    // high half, mem[addr+2]=IR2 in low half.
    always @* begin
        our_mst_rdata = {mem[our_mst_addr[15:1]], mem[(our_mst_addr[15:1]) + 1]};
    end

    always @(posedge clk) begin
        if (rst) our_mst_ack_q <= 1'b0;
        else     our_mst_ack_q <= our_mst_req;
    end

    // Slave-port program write for COP1LC.  Two cycles after cop1lc_we
    // are needed because the slave port is registered.
    reg        our_slv_req;
    reg        our_slv_we;
    reg  [5:0] our_slv_addr;
    reg [31:0] our_slv_wdata;
    reg  [1:0] our_prog_state;
    always @(posedge clk) begin
        if (rst) begin
            our_prog_state <= 0; our_slv_req <= 0; our_slv_we <= 0;
        end else begin
            our_slv_req <= 0; our_slv_we <= 0;
            if (cop1lc_we) our_prog_state <= 2'd1;
            case (our_prog_state)
                2'd1: begin
                    our_slv_req    <= 1;
                    our_slv_we     <= 1;
                    our_slv_addr   <= 6'h00;            // COP1LC at $40
                    our_slv_wdata  <= {11'd0, cop1lc_w};
                    our_prog_state <= 0;
                end
                default: ;
            endcase
        end
    end

    copper our_cop (
        .clk           (clk),
        .rst_n         (~rst),
        .slv_req       (our_slv_req),
        .slv_we        (our_slv_we),
        .slv_addr      (our_slv_addr),
        .slv_be        (4'b1111),
        .slv_wdata     (our_slv_wdata),
        .slv_rdata     (),
        .mst_req       (our_mst_req),
        .mst_we        (our_mst_we),
        .mst_addr      (our_mst_addr),
        .mst_wdata     (our_mst_wdata),
        .mst_be        (our_mst_be),
        .mst_ack       (our_mst_ack_q),
        .mst_rdata     (our_mst_rdata),
        .blt_busy_i    (1'b0),
        .vbeam_i       ({8'd0, vpos_in}),
        .auto_kick_i   (sof_in),
        .auto_active_i (1'b1),
        .cdang_i       (1'b1)
    );

    // Our MOVE captures: master-write transactions in the $DFFxxx range.
    // Capture on the FIRST cycle the request is asserted (when
    // !was_req) to avoid double-counting across multi-cycle handshakes.
    wire our_is_move = our_mst_req && our_mst_we
                    && (our_mst_addr[23:12] == 12'hDFF);
    reg  [7:0] our_queue [0:255];
    reg  [7:0] our_head, our_tail;
    reg        our_was_move;
    assign our_count    = {1'b0, (our_tail - our_head)};
    assign our_head_reg = our_queue[our_head];
    always @(posedge clk) begin
        if (rst) begin our_head <= 0; our_tail <= 0; our_was_move <= 0; end
        else begin
            if (our_is_move && !our_was_move) begin
                our_queue[our_tail] <= our_mst_addr[7:0];
                our_tail <= our_tail + 1;
            end
            our_was_move <= our_is_move;
            if (our_pop && our_head != our_tail) our_head <= our_head + 1;
        end
    end

endmodule
