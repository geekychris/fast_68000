// Round-robin shared bus arbiter + memory model.
//
// One transaction per cycle. The granted port sees grant=1 in cycle T; memory
// drives resp_valid + resp_data in cycle T+1 to that same port. Other ports
// can be granted while this one's response is in flight (write transactions
// don't stall further grants).
//
// Lock semantics: when a port wins a grant with `lock=1`, the arbiter pins
// the bus to that port for one additional transaction (which is expected to
// arrive without `lock` set). Any other requester is starved until the lock
// clears. Used to implement TAS as an atomic two-transaction RMW.
//
// All accepted writes are broadcast on the snoop output (1-cycle delayed) so
// every cache can invalidate matching lines.
module m68k_bus #(
    parameter N_PORTS  = 4,
    parameter MEM_WORDS = 16384,         // 64 KB
    parameter MEM_HEXFILE = "program.hex",
    // Optional Kickstart-style ROM region.  When ROM_WORDS > 0 the bus
    // exposes a 4*ROM_WORDS-byte read-only window at $F80000+.  An empty
    // MEM_HEXFILE_ROM leaves the ROM zero-filled (still readable, just no
    // useful content).
    parameter ROM_WORDS = 1,
    parameter MEM_HEXFILE_ROM = "",
    // Real Amiga power-up has OVL=1 so the 68000 reset-vector fetch reads
    // ROM instead of RAM.  Our regression tests boot at hardcoded PC=$400
    // (no vector fetch) and don't want low-RAM reads remapped, so OVL
    // defaults off.  A bring-up harness that loads a real Kickstart ROM
    // overrides this to 1.
    parameter OVL_RESET = 1'b0
)(
    input  wire                 clk,
    input  wire                 rst_n,

    input  wire [N_PORTS-1:0]      req,
    input  wire [N_PORTS-1:0]      we,
    input  wire [N_PORTS-1:0]      lock,
    input  wire [32*N_PORTS-1:0]   addr_flat,
    input  wire [32*N_PORTS-1:0]   wdata_flat,
    input  wire [4*N_PORTS-1:0]    be_flat,
    output reg  [N_PORTS-1:0]      grant,
    output reg  [N_PORTS-1:0]      resp_valid,
    output reg  [31:0]             resp_data,

    output reg                            snoop_valid,
    output reg  [31:0]                    snoop_addr,
    output reg  [$clog2(N_PORTS)-1:0]     snoop_src_id,

    // Memory-mapped IRQ controller: writes to address $FFFF_FFFC (i.e.
    // $FFFC.W with the 68000's sign-extension of absolute-short addresses)
    // latch the low 3 bits of wdata into irq_level (sticky; clear by
    // writing 0).
    output reg  [2:0]                     irq_level,

    // Framebuffer peek port. Combinational read-only access to the memory
    // array. Used by the simulator harness to render a window without
    // intruding on CPU bus traffic. In synthesis you would tie fb_peek_addr
    // off and ignore fb_peek_data, or remove this port entirely.
    input  wire [31:0]                    fb_peek_addr,
    output wire [31:0]                    fb_peek_data,

    // Blitter slave interface.  Writes/reads from BLT_BASE..BLT_BASE+0x3F
    // are diverted to this port instead of main memory.  The blitter module
    // sits on a separate master port for its own memory R/W; the slave
    // port is purely the CPU programming interface.
    output reg                            blt_slv_req,
    output reg                            blt_slv_we,
    output reg  [5:0]                     blt_slv_addr,
    output reg  [3:0]                     blt_slv_be,
    output reg  [31:0]                    blt_slv_wdata,
    input  wire [31:0]                    blt_slv_rdata,

    // Copper slave interface ($00FE_0040..$00FE_007F).
    output reg                            cop_slv_req,
    output reg                            cop_slv_we,
    output reg  [5:0]                     cop_slv_addr,
    output reg  [3:0]                     cop_slv_be,
    output reg  [31:0]                    cop_slv_wdata,
    input  wire [31:0]                    cop_slv_rdata,

    // Denise slave interface (two banks: $00FE_0100..$00FE_01FF and
    // $00FE_0300..$00FE_03FF).  Bank 1 ($300) carries bit 8 = 1 in the
    // 9-bit slave address; bank 0 ($100) carries bit 8 = 0.
    output reg                            den_slv_req,
    output reg                            den_slv_we,
    output reg  [8:0]                     den_slv_addr,
    output reg  [3:0]                     den_slv_be,
    output reg  [31:0]                    den_slv_wdata,
    input  wire [31:0]                    den_slv_rdata,

    // Paula slave interface ($00FE_0200..$00FE_02FF).
    output reg                            pau_slv_req,
    output reg                            pau_slv_we,
    output reg  [7:0]                     pau_slv_addr,
    output reg  [3:0]                     pau_slv_be,
    output reg  [31:0]                    pau_slv_wdata,
    input  wire [31:0]                    pau_slv_rdata,

    // CIA-A ($00FE_0400..$00FE_04FF) and CIA-B ($00FE_0500..$00FE_05FF).
    // 8-bit registers placed at 16 consecutive byte addresses (a, a+1, ...);
    // bus extracts the addressed byte lane on writes and packs the
    // 8-bit register response into the correct byte lane on reads.
    output reg                            cia_a_slv_req,
    output reg                            cia_a_slv_we,
    output reg  [3:0]                     cia_a_slv_addr,
    output reg  [7:0]                     cia_a_slv_wdata,
    input  wire [7:0]                     cia_a_slv_rdata,

    output reg                            cia_b_slv_req,
    output reg                            cia_b_slv_we,
    output reg  [3:0]                     cia_b_slv_addr,
    output reg  [7:0]                     cia_b_slv_wdata,
    input  wire [7:0]                     cia_b_slv_rdata,

    // OVL is the Amiga reset-time low-memory mirror.  When `ovl_active`,
    // reads in $0-$7FFFF redirect to $F80000-$FFFFFF (ROM).  Software clears
    // it by driving CIA-A PRA bit 0 low (input on the dedicated /OVL line).
    // We accept that as `ovl_clr_i`; once asserted, OVL stays cleared.
    input  wire                           ovl_clr_i
);
    localparam [31:0] IRQ_REG_ADDR  = 32'hFFFF_FFFC;
    localparam [31:0] BLT_BASE      = 32'h00FE_0000;
    localparam [31:0] BLT_END       = 32'h00FE_003F;
    localparam [31:0] COP_BASE      = 32'h00FE_0040;
    localparam [31:0] COP_END       = 32'h00FE_007F;
    localparam [31:0] DEN_BASE      = 32'h00FE_0100;
    localparam [31:0] DEN_END       = 32'h00FE_01FF;
    localparam [31:0] PAU_BASE      = 32'h00FE_0200;
    localparam [31:0] PAU_END       = 32'h00FE_02FF;
    // Bank-1 Denise window (HAM8 extension: bpl_pt[6..7], colors[32..63]).
    localparam [31:0] DEN2_BASE     = 32'h00FE_0300;
    localparam [31:0] DEN2_END      = 32'h00FE_03FF;
    // 8520 CIA windows (16 byte-sized regs each, byte-dense).  Both the
    // legacy non-canonical aliases ($00FE_04xx / $00FE_05xx) and the
    // Amiga-canonical addresses ($BFE001-style / $BFD000-style) route here.
    // For the canonical addresses we use byte-dense decoding within the
    // page (real Amiga places each register 256 bytes apart; we don't
    // replicate that for now -- the bus accepts addr[3:0] regardless).
    localparam [31:0] CIAA_BASE     = 32'h00FE_0400;
    localparam [31:0] CIAA_END      = 32'h00FE_04FF;
    localparam [31:0] CIAB_BASE     = 32'h00FE_0500;
    localparam [31:0] CIAB_END      = 32'h00FE_05FF;
    localparam [31:0] CIAA_AMIGA_BASE = 32'h00BF_E000;
    localparam [31:0] CIAA_AMIGA_END  = 32'h00BF_EFFF;
    localparam [31:0] CIAB_AMIGA_BASE = 32'h00BF_D000;
    localparam [31:0] CIAB_AMIGA_END  = 32'h00BF_DFFF;
    // Canonical Amiga chipset window aliasing.  Each region below maps
    // 1:1 to our internal slave's register layout (Paula and Denise are
    // already canonical-aligned within their windows).  Blitter
    // ($DFF040-$DFF07F) and Copper ($DFF080-$DFF08F) need register-offset
    // translation because our internal blitter starts at offset 0 with
    // a custom BLTCON layout; they're left as a follow-on.
    localparam [31:0] PAU_AMIGA_BASE = 32'h00DF_F000;
    localparam [31:0] PAU_AMIGA_END  = 32'h00DF_F0FF;
    localparam [31:0] DEN_AMIGA_BASE = 32'h00DF_F100;
    localparam [31:0] DEN_AMIGA_END  = 32'h00DF_F1FF;
    localparam [31:0] DEN2_AMIGA_BASE= 32'h00DF_F300;
    localparam [31:0] DEN2_AMIGA_END = 32'h00DF_F3FF;
    // Kickstart-style ROM region: 32-bit aligned, ROM_WORDS deep, mapped
    // at $F80000+ (also at $FFFFFFxx via the 68000's reset-vector fetch,
    // since OVL remaps low memory to here at reset).
    localparam [31:0] ROM_BASE        = 32'h00F8_0000;
    localparam [31:0] ROM_END         = 32'h00F8_0000 + (ROM_WORDS << 2) - 1;
    localparam PID_BITS = $clog2(N_PORTS);
    localparam AIDX_BITS = $clog2(MEM_WORDS);

    // Memory.
    reg [31:0] mem [0:MEM_WORDS-1];

    // Read-only ROM region (Kickstart-style).  Always at least one word so
    // that synthesis tools don't blow up on a zero-sized array; only
    // meaningful when the test bench provides MEM_HEXFILE_ROM.
    reg [31:0] rom [0:ROM_WORDS-1];
    localparam ROM_IDX_BITS = $clog2(ROM_WORDS > 1 ? ROM_WORDS : 2);

    // Framebuffer peek (word index = fb_peek_addr[AIDX_BITS+1:2]).
    assign fb_peek_data = mem[fb_peek_addr[AIDX_BITS+1:2]];
    integer mi;
    initial begin
        for (mi = 0; mi < MEM_WORDS; mi = mi + 1) mem[mi] = 32'd0;
        $readmemh(MEM_HEXFILE, mem);
        for (mi = 0; mi < ROM_WORDS; mi = mi + 1) rom[mi] = 32'd0;
        if (MEM_HEXFILE_ROM != "")
            $readmemh(MEM_HEXFILE_ROM, rom);
    end

    // OVL latch: set on reset, cleared the first time CIA-A drives /OVL low.
    reg ovl_active;

    // Unflatten inputs.
    wire [31:0] addr  [0:N_PORTS-1];
    wire [31:0] wdata [0:N_PORTS-1];
    wire [3:0]  be    [0:N_PORTS-1];
    genvar g;
    generate
        for (g = 0; g < N_PORTS; g = g + 1) begin : unflat
            assign addr[g]  = addr_flat[32*g +: 32];
            assign wdata[g] = wdata_flat[32*g +: 32];
            assign be[g]    = be_flat[4*g +: 4];
        end
    endgenerate

    // Lock tracking.
    reg               lock_pending;
    reg [PID_BITS-1:0] lock_holder;

    // Round-robin pointer.
    reg [PID_BITS-1:0] rrobin;

    // Choose winner combinationally.
    reg [PID_BITS-1:0] winner;
    reg                winner_valid;
    integer            k;
    reg [PID_BITS:0]   cand_ext;     // one extra bit so + cannot wrap silently
    reg [PID_BITS-1:0] cand;

    always @* begin
        winner = {PID_BITS{1'b0}};
        winner_valid = 1'b0;
        cand = {PID_BITS{1'b0}};
        cand_ext = {(PID_BITS+1){1'b0}};
        if (lock_pending) begin
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

    // Grant signals.
    integer pp;
    always @* begin
        grant = {N_PORTS{1'b0}};
        if (winner_valid) grant[winner] = 1'b1;
    end

    // Response routing: 1-cycle delay.
    reg                 granted_valid_q;
    reg [PID_BITS-1:0]  granted_port_q;
    reg                 granted_we_q;
    reg [AIDX_BITS-1:0] granted_idx_q;
    reg [31:0]          granted_addr_q;
    reg                 granted_is_irq_q;
    reg                 granted_is_blt_q;
    reg                 granted_blt_we_q;
    reg [31:0]          granted_blt_rdata_q;
    reg                 granted_is_cop_q;
    reg                 granted_cop_we_q;
    reg [31:0]          granted_cop_rdata_q;
    reg                 granted_is_den_q;
    reg                 granted_den_we_q;
    reg [31:0]          granted_den_rdata_q;
    reg                 granted_is_pau_q;
    reg                 granted_pau_we_q;
    reg [31:0]          granted_pau_rdata_q;
    reg                 granted_is_ciaa_q;
    reg                 granted_ciaa_we_q;
    reg [7:0]           granted_ciaa_rdata_q;
    reg [1:0]           granted_ciaa_lane_q;
    reg                 granted_is_ciab_q;
    reg                 granted_ciab_we_q;
    reg [7:0]           granted_ciab_rdata_q;
    reg [1:0]           granted_ciab_lane_q;
    reg                 granted_is_rom_q;
    reg [31:0]          granted_rom_data_q;

    // OVL low-memory remap: when active, reads at $0-$7FFFF land in ROM
    // instead of main memory.  Computed on the raw access address so we can
    // route the read to the rom[] array below.
    wire ovl_remap = ovl_active && !we[winner] &&
                     (addr[winner] < 32'h0008_0000);

    wire [AIDX_BITS-1:0] mem_idx = addr[winner][AIDX_BITS+1:2];
    wire is_irq_reg = (addr[winner] == IRQ_REG_ADDR);
    wire is_blt_reg = (addr[winner] >= BLT_BASE) && (addr[winner] <= BLT_END);
    wire is_cop_reg = (addr[winner] >= COP_BASE) && (addr[winner] <= COP_END);
    wire is_den_legacy = ((addr[winner] >= DEN_BASE)  && (addr[winner] <= DEN_END)) ||
                         ((addr[winner] >= DEN2_BASE) && (addr[winner] <= DEN2_END));
    wire is_den_amiga  = ((addr[winner] >= DEN_AMIGA_BASE)  && (addr[winner] <= DEN_AMIGA_END)) ||
                         ((addr[winner] >= DEN2_AMIGA_BASE) && (addr[winner] <= DEN2_AMIGA_END));
    wire is_den_reg = is_den_legacy | is_den_amiga;
    wire den_bank   = ((addr[winner] >= DEN2_BASE)       && (addr[winner] <= DEN2_END)) ||
                      ((addr[winner] >= DEN2_AMIGA_BASE) && (addr[winner] <= DEN2_AMIGA_END));
    wire is_pau_legacy = (addr[winner] >= PAU_BASE) && (addr[winner] <= PAU_END);
    wire is_pau_amiga  = (addr[winner] >= PAU_AMIGA_BASE) && (addr[winner] <= PAU_AMIGA_END);
    wire is_pau_reg  = is_pau_legacy | is_pau_amiga;
    wire is_ciaa_legacy = (addr[winner] >= CIAA_BASE) && (addr[winner] <= CIAA_END);
    wire is_ciab_legacy = (addr[winner] >= CIAB_BASE) && (addr[winner] <= CIAB_END);
    wire is_ciaa_amiga  = (addr[winner] >= CIAA_AMIGA_BASE) && (addr[winner] <= CIAA_AMIGA_END);
    wire is_ciab_amiga  = (addr[winner] >= CIAB_AMIGA_BASE) && (addr[winner] <= CIAB_AMIGA_END);
    wire is_ciaa_reg = is_ciaa_legacy | is_ciaa_amiga;
    wire is_ciab_reg = is_ciab_legacy | is_ciab_amiga;
    // Canonical Amiga CIA address layout has the 16 byte-sized registers
    // 256 bytes apart (addr bits [11:8] = register #).  Our legacy alias
    // keeps the registers byte-dense (addr[3:0] = register #).
    wire [3:0] ciaa_reg_idx = is_ciaa_amiga ? addr[winner][11:8]
                                            : addr[winner][3:0];
    wire [3:0] ciab_reg_idx = is_ciab_amiga ? addr[winner][11:8]
                                            : addr[winner][3:0];
    // Kickstart ROM read region.  The full 512 KB window at $F80000-$FFFFFF
    // is owned by the ROM regardless of ROM_WORDS: reads outside the loaded
    // ROM body return 0, writes anywhere in the window are dropped (so they
    // can't alias back into main RAM via mem_idx).
    wire is_rom_window = (addr[winner] >= 32'h00F8_0000) &&
                         (addr[winner] <= 32'h00FF_FFFF);
    wire is_rom_access = is_rom_window | ovl_remap;
    // Word index within rom[].  Clamped via masking: outside-the-array reads
    // just wrap (the test bench keeps ROM_WORDS large enough for the loaded
    // content).
    wire [31:0] rom_byte_off = ovl_remap ? addr[winner]
                                         : (addr[winner] - 32'h00F8_0000);
    wire [ROM_IDX_BITS-1:0] rom_idx = rom_byte_off[ROM_IDX_BITS+1:2];

    // 8-bit CIA: byte lane within the 32-bit word, derived from addr[1:0].
    //   addr[1:0] = 0 -> bits [31:24]  (BE bit 3)
    //   addr[1:0] = 1 -> bits [23:16]  (BE bit 2)
    //   addr[1:0] = 2 -> bits [15:8]   (BE bit 1)
    //   addr[1:0] = 3 -> bits [7:0]    (BE bit 0)
    wire [1:0] cia_lane = addr[winner][1:0];
    reg  [7:0] cia_wbyte;
    always @* begin
        case (cia_lane)
            2'd0: cia_wbyte = wdata[winner][31:24];
            2'd1: cia_wbyte = wdata[winner][23:16];
            2'd2: cia_wbyte = wdata[winner][15:8];
            default: cia_wbyte = wdata[winner][7:0];
        endcase
    end

    // Combinational slave ports.  When the winning access hits the blitter
    // or Copper register space, we route the request to the appropriate
    // slv_* and prevent the memory write.  Reads return slv_rdata via the
    // delayed-response path.
    always @* begin
        blt_slv_req   = winner_valid && is_blt_reg;
        blt_slv_we    = winner_valid && is_blt_reg && we[winner];
        blt_slv_addr  = addr[winner][5:0];
        blt_slv_be    = be[winner];
        blt_slv_wdata = wdata[winner];
    end
    always @* begin
        cop_slv_req   = winner_valid && is_cop_reg;
        cop_slv_we    = winner_valid && is_cop_reg && we[winner];
        cop_slv_addr  = addr[winner][5:0];
        cop_slv_be    = be[winner];
        cop_slv_wdata = wdata[winner];
    end
    always @* begin
        den_slv_req   = winner_valid && is_den_reg;
        den_slv_we    = winner_valid && is_den_reg && we[winner];
        den_slv_addr  = {den_bank, addr[winner][7:0]};
        den_slv_be    = be[winner];
        den_slv_wdata = wdata[winner];
    end
    always @* begin
        pau_slv_req   = winner_valid && is_pau_reg;
        pau_slv_we    = winner_valid && is_pau_reg && we[winner];
        pau_slv_addr  = addr[winner][7:0];
        pau_slv_be    = be[winner];
        pau_slv_wdata = wdata[winner];
    end
    always @* begin
        cia_a_slv_req   = winner_valid && is_ciaa_reg;
        cia_a_slv_we    = winner_valid && is_ciaa_reg && we[winner];
        cia_a_slv_addr  = ciaa_reg_idx;
        cia_a_slv_wdata = cia_wbyte;
    end
    always @* begin
        cia_b_slv_req   = winner_valid && is_ciab_reg;
        cia_b_slv_we    = winner_valid && is_ciab_reg && we[winner];
        cia_b_slv_addr  = ciab_reg_idx;
        cia_b_slv_wdata = cia_wbyte;
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            lock_pending <= 1'b0;
            lock_holder  <= {PID_BITS{1'b0}};
            rrobin       <= {PID_BITS{1'b0}};
            granted_valid_q <= 1'b0;
            granted_port_q  <= {PID_BITS{1'b0}};
            granted_we_q    <= 1'b0;
            granted_idx_q   <= {AIDX_BITS{1'b0}};
            granted_addr_q  <= 32'd0;
            granted_is_irq_q <= 1'b0;
            granted_is_blt_q <= 1'b0;
            granted_blt_we_q <= 1'b0;
            granted_blt_rdata_q <= 32'd0;
            granted_is_cop_q <= 1'b0;
            granted_cop_we_q <= 1'b0;
            granted_cop_rdata_q <= 32'd0;
            granted_is_den_q <= 1'b0;
            granted_den_we_q <= 1'b0;
            granted_den_rdata_q <= 32'd0;
            granted_is_pau_q <= 1'b0;
            granted_pau_we_q <= 1'b0;
            granted_pau_rdata_q <= 32'd0;
            granted_is_ciaa_q <= 1'b0;
            granted_ciaa_we_q <= 1'b0;
            granted_ciaa_rdata_q <= 8'd0;
            granted_ciaa_lane_q <= 2'd0;
            granted_is_ciab_q <= 1'b0;
            granted_ciab_we_q <= 1'b0;
            granted_ciab_rdata_q <= 8'd0;
            granted_ciab_lane_q <= 2'd0;
            granted_is_rom_q <= 1'b0;
            granted_rom_data_q <= 32'd0;
            ovl_active <= OVL_RESET;
            irq_level <= 3'd0;
        end else begin
            // Writes to the IRQ register update irq_level (sticky) and do NOT
            // commit to main memory. Writes to the blitter register region
            // are handled by the combinational slave port (no memory write).
            // Writes anywhere else update memory.
            if (winner_valid && we[winner] && is_irq_reg) begin
                if (be[winner][0]) irq_level <= wdata[winner][2:0];
            end else if (winner_valid && we[winner] && is_blt_reg) begin
                // Handled via blt_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_cop_reg) begin
                // Handled via cop_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_den_reg) begin
                // Handled via den_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_pau_reg) begin
                // Handled via pau_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_ciaa_reg) begin
                // Handled via cia_a_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_ciab_reg) begin
                // Handled via cia_b_slv_we combinationally; nothing to do here.
            end else if (winner_valid && we[winner] && is_rom_window) begin
                // ROM region is read-only; silently drop the write.
            end else if (winner_valid && we[winner]) begin
                if (be[winner][3]) mem[mem_idx][31:24] <= wdata[winner][31:24];
                if (be[winner][2]) mem[mem_idx][23:16] <= wdata[winner][23:16];
                if (be[winner][1]) mem[mem_idx][15:8]  <= wdata[winner][15:8];
                if (be[winner][0]) mem[mem_idx][7:0]   <= wdata[winner][7:0];
            end

            // Latch for delayed response.
            granted_valid_q <= winner_valid;
            granted_port_q  <= winner;
            granted_we_q    <= winner_valid && we[winner];
            granted_idx_q   <= mem_idx;
            granted_addr_q  <= addr[winner];
            granted_is_irq_q <= winner_valid && is_irq_reg;
            granted_is_blt_q <= winner_valid && is_blt_reg;
            granted_blt_we_q <= winner_valid && is_blt_reg && we[winner];
            granted_is_cop_q <= winner_valid && is_cop_reg;
            granted_cop_we_q <= winner_valid && is_cop_reg && we[winner];
            granted_is_den_q <= winner_valid && is_den_reg;
            granted_den_we_q <= winner_valid && is_den_reg && we[winner];
            granted_is_pau_q <= winner_valid && is_pau_reg;
            granted_pau_we_q <= winner_valid && is_pau_reg && we[winner];
            granted_is_ciaa_q <= winner_valid && is_ciaa_reg;
            granted_ciaa_we_q <= winner_valid && is_ciaa_reg && we[winner];
            granted_ciaa_lane_q <= cia_lane;
            granted_is_ciab_q <= winner_valid && is_ciab_reg;
            granted_ciab_we_q <= winner_valid && is_ciab_reg && we[winner];
            granted_ciab_lane_q <= cia_lane;
            // Latch combinational slave read results on the grant cycle
            // so the delayed response sees stable data.
            if (winner_valid && is_blt_reg && !we[winner])
                granted_blt_rdata_q <= blt_slv_rdata;
            if (winner_valid && is_cop_reg && !we[winner])
                granted_cop_rdata_q <= cop_slv_rdata;
            if (winner_valid && is_den_reg && !we[winner])
                granted_den_rdata_q <= den_slv_rdata;
            if (winner_valid && is_pau_reg && !we[winner])
                granted_pau_rdata_q <= pau_slv_rdata;
            if (winner_valid && is_ciaa_reg && !we[winner])
                granted_ciaa_rdata_q <= cia_a_slv_rdata;
            if (winner_valid && is_ciab_reg && !we[winner])
                granted_ciab_rdata_q <= cia_b_slv_rdata;
            granted_is_rom_q <= winner_valid && !we[winner] && is_rom_access;
            if (winner_valid && !we[winner] && is_rom_access)
                granted_rom_data_q <= rom[rom_idx];

            // OVL clears the first time CIA-A drives the /OVL line low.
            if (ovl_clr_i) ovl_active <= 1'b0;

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

    // CIA 8-bit -> 32-bit byte lane packing for the response word.
    reg [31:0] ciaa_resp_w;
    reg [31:0] ciab_resp_w;
    always @* begin
        case (granted_ciaa_lane_q)
            2'd0: ciaa_resp_w = {granted_ciaa_rdata_q, 24'd0};
            2'd1: ciaa_resp_w = {8'd0, granted_ciaa_rdata_q, 16'd0};
            2'd2: ciaa_resp_w = {16'd0, granted_ciaa_rdata_q, 8'd0};
            default: ciaa_resp_w = {24'd0, granted_ciaa_rdata_q};
        endcase
        case (granted_ciab_lane_q)
            2'd0: ciab_resp_w = {granted_ciab_rdata_q, 24'd0};
            2'd1: ciab_resp_w = {8'd0, granted_ciab_rdata_q, 16'd0};
            2'd2: ciab_resp_w = {16'd0, granted_ciab_rdata_q, 8'd0};
            default: ciab_resp_w = {24'd0, granted_ciab_rdata_q};
        endcase
    end

    // Output response (1 cycle after grant). Reads of the IRQ register return
    // {29'b0, irq_level}; reads of the blitter region return blt_slv_rdata;
    // everything else reads from main memory.
    always @* begin
        resp_valid = {N_PORTS{1'b0}};
        resp_data  = granted_is_irq_q  ? {29'd0, irq_level}
                   : granted_is_blt_q  ? granted_blt_rdata_q
                   : granted_is_cop_q  ? granted_cop_rdata_q
                   : granted_is_den_q  ? granted_den_rdata_q
                   : granted_is_pau_q  ? granted_pau_rdata_q
                   : granted_is_ciaa_q ? ciaa_resp_w
                   : granted_is_ciab_q ? ciab_resp_w
                   : granted_is_rom_q  ? granted_rom_data_q
                   : mem[granted_idx_q];
        if (granted_valid_q) resp_valid[granted_port_q] = 1'b1;
    end

    // Snoop broadcast: every successful write goes out 1 cycle later.
    // (Suppress for blitter-register writes since those don't touch memory.)
    always @* begin
        snoop_valid  = granted_valid_q && granted_we_q
                       && !granted_blt_we_q && !granted_cop_we_q
                       && !granted_den_we_q && !granted_pau_we_q
                       && !granted_ciaa_we_q && !granted_ciab_we_q;
        snoop_addr   = granted_addr_q;
        snoop_src_id = granted_port_q;
    end
endmodule
