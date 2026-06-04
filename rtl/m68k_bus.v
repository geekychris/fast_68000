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
    parameter OVL_RESET = 1'b0,
    // Optional memory-backed block device (think SD-card / hardfile).
    // When DISK_WORDS > 0 the bus exposes a read-only disk image at
    // $00400000..$00400000 + DISK_WORDS*4 - 1 and a 4-register block-
    // device control port at $00FE_8000 (BLKSRC / BLKDST / BLKCNT / BLKCMD).
    // Writing 1 to BLKCMD streams BLKCNT*512 bytes from disk[BLKSRC*512]
    // to mem[BLKDST] one word per host clock; BLKCMD reads back 0 when
    // the transfer is done.
    parameter DISK_WORDS = 1,
    parameter DISK_HEXFILE = ""
)(
    input  wire                 clk,
    input  wire                 rst_n,

    input  wire [N_PORTS-1:0]      req,
    input  wire [N_PORTS-1:0]      we,
    input  wire [N_PORTS-1:0]      lock,
    input  wire [32*N_PORTS-1:0]   addr_flat,
    input  wire [32*N_PORTS-1:0]   wdata_flat,
    input  wire [4*N_PORTS-1:0]    be_flat,
    input  wire [N_PORTS-1:0]      is_long_flat,
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

    // Memory-poke port. Drive mem_poke_strobe high for one host clock
    // to write mem_poke_data into mem[] at word address
    // mem_poke_addr[AIDX_BITS+1:2].  Diagnostic-only — the simulator
    // uses this to patch K1.3 task state (e.g. force input.device's
    // tc_SigWait to include the keyboard-event bit so a stalled boot
    // can advance past CLI WAIT).  In synthesis tie strobe low.
    input  wire                           mem_poke_strobe,
    input  wire [31:0]                    mem_poke_addr,
    input  wire [31:0]                    mem_poke_data,

    // Cosim trace inputs (used only by `[CosimMW]` emission inside
    // KICKSTART_COSIM_TRACE).  Driven by m68k_top from the CPU's
    // retired counter and cosim window plusargs.
    input  wire [31:0]                    cosim_retired_i,
    input  wire                           cosim_in_window_i,

    // Blitter slave interface.  Writes/reads from BLT_BASE..BLT_BASE+0x3F
    // are diverted to this port instead of main memory.  The blitter module
    // sits on a separate master port for its own memory R/W; the slave
    // port is purely the CPU programming interface.
    output reg                            blt_slv_req,
    output reg                            blt_slv_we,
    output reg  [5:0]                     blt_slv_addr,
    output reg  [3:0]                     blt_slv_be,
    output reg                            blt_slv_is_long,
    output reg  [31:0]                    blt_slv_wdata,
    input  wire [31:0]                    blt_slv_rdata,

    // Blitter live busy/bzero status.  Real Amiga reflects these in
    // DMACONR (bit 14 = BBUSY, bit 13 = BZERO).  Used by K1.3 WAITBLIT.
    input  wire                           blt_busy_i,
    input  wire                           blt_bzero_i,

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
    input  wire                           ovl_clr_i,
    // Keyboard byte injection (testbench-driven).
    output reg                            kbd_inject_wr,
    output reg  [7:0]                     kbd_inject_byte,

    // Mouse state from the harness.  mouse_x_in / mouse_y_in are 8-bit
    // wrap-around quadrature counters; they appear directly as the low /
    // high bytes of JOY0DAT.  mouse_btn_r_in is 1 while the right mouse
    // button is held — drives POTGOR bit 10 low (the right-port right-
    // button source on real Amiga).
    input  wire [7:0]                     mouse_x_in,
    input  wire [7:0]                     mouse_y_in,
    input  wire                           mouse_btn_r_in,

    // Vertical-blank pulse from Agnus beam counter (one host clock high
    // per frame, on the cycle (agnus_h, agnus_v) = (LAST_H, LAST_V)).
    // DSKBLK pulse: high for one cycle when the block-DMA engine finishes
    // (blk_busy 1->0).  Wired into Paula's dskblk_int_i so the floppy
    // completion interrupt (INTREQ bit 1, IPL 1) fires when DSKLEN-style
    // DMA completes.
    // Wire into Paula's vblank_int_i so the system-tick interrupt (VERTB,
    // INTREQ bit 5, IPL 3) fires once per frame.
    output wire                           vblank_pulse_o,
    output wire                           dskblk_pulse_o,
    // Bitplane DMA shadow outputs for Denise auto-rasterisation.
    // BPLnPT pointers come from the chipset shadow chip_regs[$E0..$F6],
    // BPL1MOD/BPL2MOD are snooped directly off canonical writes to
    // $DFF108 / $DFF10A (Denise's slave dropped those silently).  Denise
    // uses these as the source for `row_bpl_pt` and `bpl_mod` when it
    // auto-kicks on `vblank_pulse_o`.
    output wire [31:0]                    bpl_pt0_o,
    output wire [31:0]                    bpl_pt1_o,
    output wire [31:0]                    bpl_pt2_o,
    output wire [31:0]                    bpl_pt3_o,
    output wire [31:0]                    bpl_pt4_o,
    output wire [31:0]                    bpl_pt5_o,
    output wire [15:0]                    bpl_mod1_o,
    output wire [15:0]                    bpl_mod2_o,
    output wire                           den_auto_active_o,
    output wire                           cop_auto_active_o,
    // COPCON.CDANG: bit 1 of $DFF02E.  When 0 (default OCS) the Copper
    // is restricted to chip registers $80+; when 1 the gate opens to
    // $40+ so Copper-driven blitter setup works.  $00-$3E is always
    // blocked (disk/serial/INTREQ).
    output wire                           cop_cdang_o,
    // One-cycle pulse fired at the start of every disk DMA read.  Our DMA
    // is "minimig-style" memcpy and skips the real bit-stream PLL/sync
    // search; this pulse stands in for the DSKSYN event Paula would raise
    // when DSKSYNC matched.  K1.3 trackdisk waits on DSKSYN to start
    // processing the buffered MFM track.
    output wire                           dsksyn_pulse_o,
    // One-cycle pulse on OVL 1 -> 0 transition.  Wired to every cache's
    // inval_all input so cached pre-OVL ROM-overlay reads at $00000-$7FFFF
    // are invalidated when low memory switches to RAM.
    output wire                           ovl_clear_pulse_o,
    // Current floppy ADF track index (cyl*2 + side), 0..159.  Used to
    // offset the MFM-DMA read source within disk[].  Driven by the step-
    // pulse-tracking block in m68k_top.v; defaults to 0 so test benches
    // that don't wire this in still get track 0 (the legacy behaviour).
    input  wire [7:0]                     disk_track_i,
    // High whenever all CPU cores are parked on STOP waiting for IRQs.
    // When asserted, the Agnus beam counter advances by SIM_FF_STRIDE
    // cycles per host clock instead of 1, making VBL pulses fire faster
    // (and via the CIA tick prescaler, CIA Timer A/B count faster too).
    // The OS doesn't see the speedup: its time base (CIA-A TOD on VBL)
    // is preserved relative to the OS-visible event rate.  Off by default
    // when SIM_FF_STRIDE=1.
    input  wire                           sim_ff_active
);
    // How many beam-cycles per host clock when fast-forward is active.
    // 64 = ~64x faster VBL firing.  Pure sim performance hack; observable
    // CPU behavior unchanged because the CPU is parked on STOP.
    localparam [9:0] SIM_FF_STRIDE = 10'd64;
`ifdef KICKSTART_BOOT
    // Under Kickstart the address-bus mask in `unflat` drops bits 24..31,
    // so $FFFF_FFFC (the test-bench IRQ register slot) folds to
    // $00FF_FFFC which is inside the Kickstart ROM window.  Move it to
    // the empty autoconfig slot at $00E9_FFFC which Kickstart never reads.
    localparam [31:0] IRQ_REG_ADDR  = 32'h00E9_FFFC;
`else
    // Test-bench IRQ register.  $FFFC.W sign-extends to $FFFF_FFFC inside
    // the CPU; the 24-bit mask folds that to $00FF_FFFC, so compare
    // against the masked form.
    localparam [31:0] IRQ_REG_ADDR  = 32'h00FF_FFFC;
`endif
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
    // Canonical Amiga chipset window aliasing.  Paula/Denise are 1:1.
    // Blitter ($DFF040-$DFF07F) and Copper ($DFF080-$DFF08E) need
    // register-offset and bit-layout translation; the canonical writes
    // are bit-translated into our internal 32-bit register slave at the
    // muxing layer below.  Reads on the canonical Blitter/Copper window
    // fall through to the chipset shadow (chip_regs[]) so software can
    // round-trip 16-bit Amiga values.
    localparam [31:0] PAU_AMIGA_BASE = 32'h00DF_F000;
    localparam [31:0] PAU_AMIGA_END  = 32'h00DF_F0FF;
    localparam [31:0] DEN_AMIGA_BASE = 32'h00DF_F100;
    localparam [31:0] DEN_AMIGA_END  = 32'h00DF_F1FF;
    localparam [31:0] DEN2_AMIGA_BASE= 32'h00DF_F300;
    localparam [31:0] DEN2_AMIGA_END = 32'h00DF_F3FF;
    localparam [31:0] BLT_AMIGA_BASE = 32'h00DF_F040;
    localparam [31:0] BLT_AMIGA_END  = 32'h00DF_F07F;
    localparam [31:0] COP_AMIGA_BASE = 32'h00DF_F080;
    localparam [31:0] COP_AMIGA_END  = 32'h00DF_F08F;
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

    // Framebuffer / chipram peek.  Routes to slow-RAM array when the
    // peek address lands in Agnus trapdoor range $C00000-$C7FFFF, so
    // sim-side chipram dumps and tools can also see slow-RAM contents
    // (Exec structures live there once K1.3 prefers slow RAM for non-
    // chip allocations).
    assign fb_peek_data =
        (fb_peek_addr >= 32'h00C0_0000 && fb_peek_addr < 32'h00C8_0000)
            ? slowmem[(fb_peek_addr - 32'h00C0_0000) >> 2]
            : mem[fb_peek_addr[AIDX_BITS+1:2]];

    // Memory-poke port (diagnostic — see comment near port declaration).
    always @(posedge clk) begin
        if (mem_poke_strobe)
            mem[mem_poke_addr[AIDX_BITS+1:2]] <= mem_poke_data;
    end
    integer mi;
    initial begin
        for (mi = 0; mi < MEM_WORDS; mi = mi + 1) mem[mi] = 32'd0;
        $readmemh(MEM_HEXFILE, mem);
        for (mi = 0; mi < ROM_WORDS; mi = mi + 1) rom[mi] = 32'd0;
        if (MEM_HEXFILE_ROM != "")
            $readmemh(MEM_HEXFILE_ROM, rom);
        for (mi = 0; mi < DISK_WORDS; mi = mi + 1) disk[mi] = 32'd0;
        if (DISK_HEXFILE != "")
            $readmemh(DISK_HEXFILE, disk);
    end

    // ----- Disk image (read-only) ----------------------------------
    // 32-bit words; address $00400000 + word_idx*4.
    localparam DISK_IDX_BITS = $clog2(DISK_WORDS > 1 ? DISK_WORDS : 2);
    reg [31:0] disk [0:DISK_WORDS-1];
    localparam [31:0] DISK_BASE = 32'h0040_0000;
    localparam [31:0] DISK_END  = 32'h0040_0000 + (DISK_WORDS << 2) - 1;

    // ----- Block-device control registers --------------------------
    localparam [31:0] BLK_BASE  = 32'h00FE_8000;
    localparam [31:0] BLKSRC_AD = 32'h00FE_8000;   // source sector # (32-bit)
    localparam [31:0] BLKDST_AD = 32'h00FE_8004;   // dest mem byte addr (32-bit)
    localparam [31:0] BLKCNT_AD = 32'h00FE_8008;   // # sectors (32-bit; sector = 512 B)
    localparam [31:0] BLKCMD_AD = 32'h00FE_800C;   // write 1 = go; read 0 when done
    reg [31:0] blk_src;     // sector index
    reg [31:0] blk_dst;     // destination byte addr in mem[]
    reg [31:0] blk_cnt;     // sectors remaining
    reg        blk_busy;
    reg [31:0] blk_cur_off; // current disk word offset
`ifdef HDRCHK_WATCH
    reg [31:0] cyc_count;   // free-running cycle counter for trace ordering
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) cyc_count <= 32'd0;
        else        cyc_count <= cyc_count + 32'd1;
    end
`endif
    reg [31:0] blk_cur_dst; // current dest byte addr
    reg        blk_busy_last;
    // When the DSKLEN-DMA path triggers it, blk_count_in_bytes is set to
    // dsk_len_words << 1 and the byte-mode termination check below
    // uses (blk_dst + blk_count_in_bytes) instead of (blk_dst + blk_cnt*512).
    // Lets us serve trackdisk's per-word DMA length verbatim rather than
    // re-quantizing to sectors.
    reg        blk_byte_mode;
    reg [31:0] blk_count_in_bytes;
    // One-cycle pulse on the busy 1->0 transition (DMA completion).
    assign dskblk_pulse_o = blk_busy_last && !blk_busy;
    // One-cycle pulse on the busy 0->1 transition (DMA start), gated to
    // DSKLEN-driven disk reads only (not BLK* shadow-DMA writes).
    // K1.3 sets DSKSYNC=$4489 and ADKCON WORDSYNC=1, then expects DSKSYN
    // when sync is locked.  We don't model bit-stream PLLs, so we raise it
    // at the moment DMA starts: the trackdisk buffer's first MFM word is
    // already $4489.
    assign dsksyn_pulse_o = !blk_busy_last && blk_busy && blk_byte_mode;

    // OVL latch: set on reset, cleared the first time CIA-A drives /OVL low.
    reg ovl_active;
    reg ovl_just_cleared;   // one-cycle pulse on OVL 1->0
    assign ovl_clear_pulse_o = ovl_just_cleared;

    // ----- Canonical Blitter/Copper shadow registers ----------------
    // Hold the most recent 16-bit canonical write so the next write to
    // the other half of a 32-bit value can compose the full word and
    // re-emit it to the internal slave port (which is 32-bit-wide).
    reg [15:0] canon_bltcon0;
    reg [15:0] canon_bltcon1;
    reg [31:0] canon_bltapt;
    reg [31:0] canon_bltbpt;
    reg [31:0] canon_bltcpt;
    reg [31:0] canon_bltdpt;
    reg [31:0] canon_cop1lc;
    reg [31:0] canon_cop2lc;

    // ----- Agnus stub (minimal) ---------------------------------------
    // Real Agnus owns $DFF000-$DFF03F (DMA + beam) and shares $DFF080-$DFF09F
    // with the copper/interrupt block.  This stub implements just the four
    // registers needed for early-boot ROM code:
    //   $DFF002  DMACONR  RO  -- DMA control read mirror
    //   $DFF004  VPOSR    RO  -- {LOF, ID2..ID0, V8, 8'd0} | top bits of beam V
    //   $DFF006  VHPOSR   RO  -- {V[7:0], H[7:0]}
    //   $DFF096  DMACON   WO  -- SET/CLR semantics, same shape as INTENA
    // The beam counter increments every host clock; H wraps at 226 (227-cycle
    // PAL line), V wraps at 311 (312-line PAL frame).  No relationship to
    // wall-clock NTSC/PAL — this is just monotonic for boot-time polling.
    localparam [9:0]  AGNUS_H_LAST = 10'd226;
    localparam [9:0]  AGNUS_V_LAST = 10'd311;
    reg [9:0] agnus_h;
    reg [9:0] agnus_v;
    reg [15:0] dmacon;

    // VERTB pulse: high for one cycle each frame, when the beam counter
    // reaches the bottom-right of the display.  With strided advance
    // (SIM_FF_STRIDE) the counter can skip past the exact (LAST_H,LAST_V)
    // coordinates without ever landing on them — so detect frame end by
    // "next advance would wrap the line AND we're on the last line".
    wire beam_line_wraps_next = (agnus_h + beam_stride > AGNUS_H_LAST);
    assign vblank_pulse_o = beam_line_wraps_next && (agnus_v == AGNUS_V_LAST);
    // Beam tick.  Increment H every clock; on wrap, increment V; on V wrap,
    // back to 0.  In a real Amiga the H tick is one bus cycle (8 master
    // clocks); here we tick once per system clock so the counter advances
    // visibly under cycle-accurate cross-check.
    // Effective stride per clock — 1 normally, SIM_FF_STRIDE while all
    // CPU cores are STOPped (fast-forward through long idle gaps).
    wire [9:0] beam_stride = sim_ff_active ? SIM_FF_STRIDE : 10'd1;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            agnus_h <= 10'd0;
            agnus_v <= 10'd0;
            dmacon  <= 16'd0;
        end else begin
            // Generic advance: agnus_h += stride, wrap at AGNUS_H_LAST,
            // bump agnus_v on wrap.  Stride is clamped so wrap detection
            // works even when stride > distance-to-wrap (the next clock
            // picks up the remainder via the same logic).  Stride is
            // tuned (64) so a full line wraps in 4 clocks max; an entire
            // frame is ~1100 clocks instead of ~71K.
            if (agnus_h + beam_stride > AGNUS_H_LAST) begin
                agnus_h <= 10'd0;
                if (agnus_v == AGNUS_V_LAST) agnus_v <= 10'd0;
                else                          agnus_v <= agnus_v + 10'd1;
            end else begin
                agnus_h <= agnus_h + beam_stride;
            end
        end
    end
    localparam [31:0] DMACONR_ADDR = 32'h00DF_F002;
    localparam [31:0] VPOSR_ADDR   = 32'h00DF_F004;
    localparam [31:0] VHPOSR_ADDR  = 32'h00DF_F006;
    localparam [31:0] DMACON_ADDR  = 32'h00DF_F096;

    // ----- Floppy DSKLEN-style DMA -----------------------------------
    // Real Amiga floppy DMA: write DSKPT (32-bit byte addr), write DSKLEN
    // (length + bit15 DMAEN) twice (the second write actually starts the
    // transfer in real hw).  We accept the first write as a "start" too.
    // Source is disk[] starting at offset (blk_src << 9), i.e. we re-use
    // the block-device BLKSRC register at $00FE_8000 to set the disk
    // position.  On completion we set INTREQ DSKBLK (bit 1) via an
    // internal pulse fed into Paula's irq input wires (TODO; today we
    // only set the busy flag and the boot ROM polls).
    localparam [31:0] DSKPTH_ADDR  = 32'h00DF_F020;   // high half of DSKPT
    localparam [31:0] DSKPTL_ADDR  = 32'h00DF_F022;   // low half
    localparam [31:0] DSKLEN_ADDR  = 32'h00DF_F024;   // length + DMAEN bit15
    reg [31:0] dsk_pt;
    reg [15:0] dsk_len;

    // ----- Paula peripheral input stubs (read-only) -----------------
    // Real Kickstart pokes SERDATR (bit 14 RBF = "receive buffer full",
    // bit 13 TBE = "transmit buffer empty", bit 12 TSRE = "transmit
    // shift register empty") and POTGOR (joystick / mouse / paddle
    // button bits, active-low) during early init.  We pin SERDATR to
    // "idle, no RX data" and POTGOR to "all buttons up, all pots
    // floating high" so Kickstart's loops don't trip on garbage.
    localparam [31:0] SERDATR_ADDR = 32'h00DF_F018;   // serial data + status
    localparam [31:0] POTGOR_ADDR  = 32'h00DF_F016;   // pot/joy buttons + pull-ups
    localparam [31:0] ADKCONR_ADDR = 32'h00DF_F010;   // audio/disk control read
    localparam [31:0] POT0DAT_ADDR = 32'h00DF_F012;   // pot 0 sample
    localparam [31:0] POT1DAT_ADDR = 32'h00DF_F014;   // pot 1 sample
    localparam [31:0] JOY0DAT_ADDR = 32'h00DF_F00A;
    localparam [31:0] JOY1DAT_ADDR = 32'h00DF_F00C;
    // SERDATR idle on real Amiga: serial RX line floats high so the
    // receive buffer reads as $FF (or $7F if 7-bit mode).  Kickstart
    // 1.3's serial self-test at $FC30DA does:
    //   MOVE.W $DFF018, D0 ; ANDI.B #$7F, D0 ; CMPI.B #$7F, D0
    //   DBEQ D1, -50       ; BMI Alert
    // DBEQ exits on EQ (cc=EQ) -- so the test expects the low 7 bits
    // to read as $7F so EQ is true on the first iteration, then BMI
    // (testing N from the CMPI) sees N=0 and falls through.  A low
    // byte of $00 (the previous $3000 setting) makes CMP set N=1,
    // DBEQ loops until D1 underflows, then BMI fires and triggers
    // ColdReboot.
    //
    // Bits 13 (TBE) and 12 (TSRE) signal transmit-buffer / transmit-
    // shift-register empty; we keep them set.  RBF (bit 14) stays 0
    // so the Romwack serial poll at $F83B98 doesn't think a byte has
    // arrived.
    localparam [15:0] SERDATR_VAL  = 16'h307F;
    // POTGOR / JOY0DAT values are now dynamic (depend on mouse_*_in).
    // The "VAL" parameters below are the no-input defaults; the actual
    // mux uses live wires further down.
    localparam [15:0] POTGOR_VAL   = 16'hFFFF;
    localparam [15:0] POT_DAT_VAL  = 16'h0000;
    localparam [15:0] JOY_DAT_VAL  = 16'h0000;
    localparam [15:0] ADKCONR_VAL  = 16'h0000;
    // Live POTGOR view: start from POTGOR_VAL and clear bits 10 and 14
    // (right buttons of port 1 / port 0) when the harness reports a
    // right-mouse-button press.
    wire [15:0] potgor_live = POTGOR_VAL
                              & ~({1'b0, mouse_btn_r_in, 3'b000,
                                   mouse_btn_r_in, 10'b00_0000_0000});
    // Live JOY0DAT view: { vert_count[7:0], horz_count[7:0] }.  Mouse
    // position is plumbed through as a raw 8-bit quadrature counter
    // (workbench's intuition.library reads JOY0DAT every VBL and
    // subtracts to get the delta).
    wire [15:0] joy0dat_live = {mouse_y_in, mouse_x_in};

    // ----- Disk-status read registers (DSKBYTR / DSKDATR / DSKSYNC) ----
    // K1.3's trackdisk.device polls DSKBYTR for DMAON to confirm the
    // floppy DMA is in progress.  Our previous model returned $0 for
    // these addresses (default unmapped), so K1.3 saw DMAON=0 always and
    // bailed out of its wait-for-DMA loop early.
    //
    // DSKBYTR ($DFF01A):
    //   bit 15: DSKBYT     (1 when byte ready, clears on read)
    //   bit 14: DMAON      (1 when disk DMA enabled and running)
    //   bit 13: DSKWRITE   (0 = read, 1 = write)
    //   bit 12: WORDEQUAL  (1 when last word == DSKSYNC)
    //   bits 7-0: last data byte from disk
    //
    // We track DMAON = blk_busy (set during the DMA copy) so K1.3 sees a
    // genuine "DMA in progress" then "DMA done" transition.
    //
    // DSKDATR ($DFF008): just returns the last disk word data; we keep
    // it 0 since the data is delivered via DMA to chip RAM anyway.
    //
    // DSKSYNC ($DFF07E): K1.3 writes the sync pattern (typically $4489)
    // and reads it back to confirm; track the last write so reads match.
    localparam [31:0] DSKBYTR_ADDR = 32'h00DF_F01A;
    localparam [31:0] DSKDATR_ADDR = 32'h00DF_F008;
    localparam [31:0] DSKSYNC_ADDR = 32'h00DF_F07E;
    localparam [31:0] ADKCON_ADDR  = 32'h00DF_F09E;
    reg [15:0] dsksync;  // sync pattern set by CPU writes
    reg [14:0] adkcon;   // ADKCON.  Only bit 10 (WORDSYNC) is consulted today.
    wire       adkcon_wordsync = adkcon[10];

    // ----- Agnus bitplane / control register STORAGE ---------------
    // Kickstart writes a lot of bitplane setup state before kicking
    // the system: BPLxPTH/L (bitplane DMA pointers), BPLxMOD (modulo),
    // BPLCON0..3 (control), DIWSTRT/DIWSTOP, DDFSTRT/DDFSTOP, COLOR00..31,
    // COPxLCH/L (copper list pointers).  Our Denise reads bitplanes
    // through its own master port today; for compat we just *store* the
    // values so reads back match writes (Kickstart sometimes re-reads).
    //
    // Storage layout: 9-bit byte-address space ($000..$1FF) inside the
    // $DFF000..$DFF1FF Paula+Denise window.  We carve out the storage
    // ranges below and shadow the writes into chip_regs[].
    //
    // Ranges shadowed:
    //   $DFF080..$DFF08F     COPxLCH/L (8 bytes per pointer)
    //   $DFF08E              COPJMP1/2 (handled as RW storage)
    //   $DFF092..$DFF093     DDFSTRT/DDFSTOP
    //   $DFF08A..$DFF08D     DSKLEN family (no DMA, just storage)
    //   $DFF0E0..$DFF0F7     BPL1PT..BPL6PT  (32-bit each)
    //   $DFF100..$DFF107     BPLCON0..BPLCON3
    //   $DFF108..$DFF10B     BPL1MOD/BPL2MOD
    //   $DFF180..$DFF1BF     COLOR00..COLOR31
    //
    // We let the existing Denise/Copper slaves continue to own their
    // canonical ranges where they have real implementations; this
    // storage region is for the rest.
    reg [15:0] chip_regs [0:511];
    integer ci;
    initial begin
        for (ci = 0; ci < 512; ci = ci + 1) chip_regs[ci] = 16'd0;
    end
    wire [8:0] chip_idx = addr[winner][8:0];
    // The "shadow" set is anything in $DFF000-$DFF1FF that isn't already
    // owned by Agnus/Paula-RO/Paula-amiga/Denise-amiga slaves.  Easier:
    // explicitly carve out the COPxLCH/L, BPLxPT, BPLCONx, BPLxMOD,
    // DDF/DIW ranges.
    // Shadow region: ONLY chipset addresses that no other slave already
    // owns.  Carefully excludes:
    //   $DFF096 / $DFF09A / $DFF09C / $DFF09E   (Agnus + Paula)
    //   $DFF100-$DFF1FF / $DFF300-$DFF3FF      (Denise)
    //   $DFF000-$DFF07F (mostly)                (DMA reads from Paula)
    // Includes:
    //   $DFF020-$DFF02E   (DSK*)         storage-only
    //   $DFF080-$DFF08F   (COP1/2LCH/L, COPJMP1/2, COPCON)
    //   $DFF090-$DFF094   (DIWSTRT/STOP, DDFSTRT)
    //   $DFF098 (CLXCON)
    //   $DFF0E0-$DFF0FF   (BPLxPT)
    // DSKPTH/L/DSKLEN are handled by their own logic below; carve them
    // out of the shadow region so writes hit the floppy state machine.
    wire is_dsk_reg = (addr[winner] == DSKPTH_ADDR) ||
                      (addr[winner] == DSKPTL_ADDR) ||
                      (addr[winner] == DSKLEN_ADDR) ||
                      (addr[winner] == DSKSYNC_ADDR) ||
                      (addr[winner] == ADKCON_ADDR);
    wire is_shadow_reg =
        ((addr[winner] >= 32'h00DF_F020 && addr[winner] <= 32'h00DF_F02E) ||
         (addr[winner] >= 32'h00DF_F040 && addr[winner] <= 32'h00DF_F07F) ||
         (addr[winner] >= 32'h00DF_F080 && addr[winner] <= 32'h00DF_F094) ||
         (addr[winner] == 32'h00DF_F098)                                  ||
         (addr[winner] >= 32'h00DF_F0E0 && addr[winner] <= 32'h00DF_F0FF))
        && !is_dsk_reg;

    // ----- Zorro II autoconfig: simulated 8MB FAST RAM card --------
    // Real Amiga probes $E80000..$E8007F for autoconfig boards.  Each
    // 8-bit board register is exposed as two nibble reads (high at the
    // base offset, low at +$100, both placed in the [7:4] of the byte).
    // We greatly simplify: reads at the base offsets return the FULL
    // byte (high nibble in [7:4]), and the +$100 mirror also works.
    // Kickstart's expansion.library starts by reading the type byte at
    // $E80000 -- $FF means "no card", anything else means a card to
    // configure.  Our type byte is $E0:
    //   bits 7..6 = 11  Zorro II
    //   bit  5    =  1  MEMLIST (this card is RAM)
    //   bits 4..3 =  0  no diag ROM, no chain
    //   bits 2..0 =  0  size = 8 MB
    // After Kickstart configures the card by writing the base address
    // (high nibble at $E80048, low at $E8004A) the card "shuts up"
    // (type byte reads back as $FF).  We don't actually back the
    // assigned region with RAM; this is the autoconfig handshake only.
    localparam [31:0] AUTOCONFIG_BASE = 32'h00E8_0000;
    localparam [31:0] AUTOCONFIG_END  = 32'h00E8_FFFF;
`ifdef KICKSTART_BOOT
    // Under Kickstart we don't actually back the autoconfig-assigned
    // region with memory, so advertising the card as Z2 RAM gets
    // Kickstart to AddMemList() a phantom 8MB region; subsequent
    // AllocMem returns pointers there and writes/reads silently corrupt.
    // Returning $FF says "no card here" so Kickstart skips the slot.
    localparam [7:0]  AUTOCONFIG_TYPE = 8'hFF;
`else
    localparam [7:0]  AUTOCONFIG_TYPE = 8'hE0;
`endif
    reg        autoconfig_shutup;
    reg [7:0]  autoconfig_base_hi;
    reg [7:0]  autoconfig_base_lo;

    // ----- Agnus trapdoor SLOW RAM at $00C00000-$00C7FFFF -----------
    // A500/A2000 ECS-Agnus exposes 512 KB of "slow RAM" (trapdoor or
    // side expansion) at this range.  It's DMA-capable through Agnus
    // (same arbitration as chip RAM, just slower), so blitter, disk,
    // and bitplane DMA must also see it.  K1.3 detects it via its
    // memory probe and AddMemList's it as MEMF_CHIP|MEMF_PUBLIC.  The
    // first-fit allocator then has more options when placing task
    // stacks vs screen buffers, removing the K1.3 boot CON stack vs
    // Workbench BitMap overlap that causes the WB1.3 ROMWACK at r=4.9M.
    localparam SLOW_WORDS = 131072;            // 512 KB / 4 = 128 K longs
    localparam [31:0] SLOW_BASE = 32'h00C0_0000;
    localparam [31:0] SLOW_END  = SLOW_BASE + (SLOW_WORDS << 2); // $00C8_0000
    reg [31:0] slowmem [0:SLOW_WORDS-1];
    integer msl;
    initial begin
        for (msl = 0; msl < SLOW_WORDS; msl = msl + 1) slowmem[msl] = 32'd0;
    end
    // is_slow used for bus arbiter (CPU + chipset master) reads/writes.
    wire is_slow = (addr[winner] >= SLOW_BASE) && (addr[winner] < SLOW_END);
    wire [16:0] slow_idx = (addr[winner] - SLOW_BASE) >> 2;
    // Helper for DMA paths that index by raw byte address (disk, BPL,
    // sprite, copper).  These bypass the arbiter and write/read mem[]
    // directly, so they must do their own slow-vs-chip selection.
    function [16:0] slow_idx_of;
        input [31:0] a;
        slow_idx_of = (a - SLOW_BASE) >> 2;
    endfunction
    function is_slow_addr;
        input [31:0] a;
        is_slow_addr = (a >= SLOW_BASE) && (a < SLOW_END);
    endfunction

    // ----- Keyboard byte injection register -------------------------
    // CPU writes a scan-code byte to $00FE_9000.  Pulses kbd_inject_wr
    // for one cycle with the byte; CIA-A latches it into SDR and fires
    // its SP interrupt source.
    localparam [31:0] KBD_INJECT_ADDR = 32'h00FE_9000;

    // ----- Agnus bitplane DMA engine --------------------------------
    // Multi-plane bitplane DMA.  Active when DMACON BPLEN (bit 8) AND
    // DMACON DMAEN (bit 9) are both set AND BPLCON0[14:12] (BPU) > 0.
    //
    // Each BPL_FETCH_DIV host clocks, one half-word is fetched from
    // mem[bpl_pt[plane_idx]] into bpl_dat[plane_idx], plane_idx is
    // advanced (mod BPU), and the corresponding running pointer is
    // incremented by 2.  Pointer pre-load: when BPLEN transitions 0→1
    // (or BPLCON0 BPU changes from 0 to non-zero), all six running
    // pointers are reloaded from the chipset shadow BPLnPT slots.
    //
    // Limitations vs real Agnus:
    //   - No DDFSTRT/DDFSTOP horizontal gating; fetches happen any time
    //     the engine is enabled.
    //   - No per-line modulo (BPLnMOD lives in the Denise window which
    //     we don't snoop here); pointers run straight through memory.
    //   - No slot-accurate interleaving vs blitter / copper / CPU.
    //
    // Probes (host bus side):
    //   $00FE_9100   total fetch counter (32-bit)
    //   $00FE_9104   BPL1DAT  (16-bit value in high half of long)
    //   $00FE_9108   BPL2DAT
    //   $00FE_910C   BPL3DAT
    //   $00FE_9110   BPL4DAT
    //   $00FE_9114   BPL5DAT
    //   $00FE_9118   BPL6DAT
    //   $00FE_911C   BPLCON0 shadow (debug)
    localparam [31:0] BPL_FETCH_COUNTER_ADDR = 32'h00FE_9100;
    localparam [31:0] BPL1DAT_PROBE_ADDR     = 32'h00FE_9104;
    localparam [31:0] BPL2DAT_PROBE_ADDR     = 32'h00FE_9108;
    localparam [31:0] BPL3DAT_PROBE_ADDR     = 32'h00FE_910C;
    localparam [31:0] BPL4DAT_PROBE_ADDR     = 32'h00FE_9110;
    localparam [31:0] BPL5DAT_PROBE_ADDR     = 32'h00FE_9114;
    localparam [31:0] BPL6DAT_PROBE_ADDR     = 32'h00FE_9118;
    localparam [31:0] BPLCON0_PROBE_ADDR     = 32'h00FE_911C;
    localparam [9:0]  BPL_FETCH_DIV = 10'd16;     // fetch every 16 host clk
    reg [31:0] bpl_pt   [0:5];        // live per-plane running pointers
    reg [15:0] bpl_dat  [0:5];        // last fetched word per plane
    reg [31:0] bpl_fetches;
    reg [9:0]  bpl_fetch_tick;
    reg [2:0]  bpl_plane_idx;         // which plane to fetch next (0..5)
    reg [15:0] bplcon0_shadow;        // snooped from writes to $DFF100
    // BPLnMOD shadows.  Real Denise puts BPLnMOD inside its own register
    // file but our slave's offset layout differs from canonical Agnus
    // ($108/$10A); canonical writes get routed to den_slv where they
    // map to BPLCON2 / unused slots and are dropped.  Mirror them here
    // so the auto-rasterise path picks up the user's modulo intent.
    reg [15:0] bpl1mod_shadow;        // snooped from writes to $DFF108
    reg [15:0] bpl2mod_shadow;        // snooped from writes to $DFF10A
    reg        bpl_was_active;        // last cycle's (BPLEN & DMAEN & BPU>0)
    reg [9:0]  agnus_v_prev;
    wire [2:0] bpu = bplcon0_shadow[14:12];
    wire       bpl_active_now = dmacon[8] && dmacon[9] && (bpu != 3'd0);

    // Bitplane shadow outputs to Denise.
    assign bpl_pt0_o = {chip_regs[9'h0E0], chip_regs[9'h0E2]};
    assign bpl_pt1_o = {chip_regs[9'h0E4], chip_regs[9'h0E6]};
    assign bpl_pt2_o = {chip_regs[9'h0E8], chip_regs[9'h0EA]};
    assign bpl_pt3_o = {chip_regs[9'h0EC], chip_regs[9'h0EE]};
    assign bpl_pt4_o = {chip_regs[9'h0F0], chip_regs[9'h0F2]};
    assign bpl_pt5_o = {chip_regs[9'h0F4], chip_regs[9'h0F6]};
    assign bpl_mod1_o = bpl1mod_shadow;
    assign bpl_mod2_o = bpl2mod_shadow;
    assign den_auto_active_o = bpl_active_now;
    assign cop_auto_active_o = dmacon[9] && dmacon[7];
    // $DFF02E.CDANG lives at bit 1 of the 16-bit COPCON.  Canonical
    // 16-bit writes land in either chip_regs[$02C] (be=$C, addr[1]=0
    // -> high half stored in chip_regs[$2C]) or chip_regs[$02E]
    // (addr[1]=1, low half stored in chip_regs[$2E]).  Real Agnus uses
    // a single 16-bit register; mirror that by ORing both shadows so
    // either form lands the bit.
    assign cop_cdang_o = chip_regs[9'h02C][1] | chip_regs[9'h02E][1];

    // Unflatten inputs.
    wire [31:0] addr  [0:N_PORTS-1];
    wire [31:0] wdata [0:N_PORTS-1];
    wire [3:0]  be    [0:N_PORTS-1];
    wire        is_long [0:N_PORTS-1];
    genvar g;
    generate
        for (g = 0; g < N_PORTS; g = g + 1) begin : unflat
            // Real 68000 has a 24-bit external address bus -- bits 24..31
            // of any 32-bit register are not driven at the pins.  Kickstart
            // 1.3 stores pointers with non-zero top bytes (e.g. flag bits
            // packed into the high byte of an APtr) and assumes
            // $0C34_xxxx resolves to chip-RAM $0034_xxxx.  Mask here so
            // the bus, every decoder, and the cache-fed bus_addr all see
            // the same 24-bit-wrapped address.
            assign addr[g]    = {8'd0, addr_flat[32*g +: 24]};
            assign wdata[g]   = wdata_flat[32*g +: 32];
            assign be[g]      = be_flat[4*g +: 4];
            assign is_long[g] = is_long_flat[g];
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
    reg                 granted_is_long_q;
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
    reg                 granted_is_slow_q;
    reg [16:0]          granted_slow_idx_q;

    // OVL low-memory remap: when active, reads at $0-$7FFFF land in ROM
    // instead of main memory.  Computed on the raw access address so we can
    // route the read to the rom[] array below.
    wire ovl_remap = ovl_active && !we[winner] &&
                     (addr[winner] < 32'h0008_0000);

    wire [AIDX_BITS-1:0] mem_idx = addr[winner][AIDX_BITS+1:2];
    wire is_irq_reg = (addr[winner] == IRQ_REG_ADDR);
    wire is_disk_read = (addr[winner] >= DISK_BASE) && (addr[winner] <= DISK_END);
    wire [DISK_IDX_BITS-1:0] disk_idx =
        addr[winner][DISK_IDX_BITS+1:2] - DISK_BASE[DISK_IDX_BITS+1:2];
    wire is_blk_reg = LEGACY_CHIPSET && (
                      (addr[winner] == BLKSRC_AD) ||
                      (addr[winner] == BLKDST_AD) ||
                      (addr[winner] == BLKCNT_AD) ||
                      (addr[winner] == BLKCMD_AD));
    // The legacy chipset region ($00FE_0000-$00FE_05FF) was this codebase's
    // pre-canonical-Amiga convention.  It collides with the Kickstart ROM
    // window ($00F8_0000-$00FF_FFFF) any time the loaded ROM either
    //   - is large enough to physically occupy $FE0000 (≥ ~384 KB), OR
    //   - is a 256 KB ROM whose mirror at $FC0000-$FFFFFF covers $FE0000.
    // K1.3 sits in the 256 KB camp: an explicit `JSR $00FE0358` in the
    // boot path lands on a valid ROM-mirror function at offset $20358;
    // with legacy chipset active, the legacy blitter slave at $FE0xxx
    // intercepts the read and returns 0, so the JSR target is garbage.
    //
    // Disable the legacy window for any "real" Kickstart-sized ROM
    // (>= 16 K words = 64 KB).  Test programs use smaller ROMs (≤ 4 K
    // words) so they continue to see the legacy demos region.
    localparam LEGACY_CHIPSET = (ROM_WORDS < 16384);

    wire is_blt_legacy = LEGACY_CHIPSET && (addr[winner] >= BLT_BASE) && (addr[winner] <= BLT_END);
    wire is_blt_amiga  = (addr[winner] >= BLT_AMIGA_BASE) && (addr[winner] <= BLT_AMIGA_END);
    wire is_blt_reg    = is_blt_legacy | is_blt_amiga;
    wire is_cop_legacy = LEGACY_CHIPSET && (addr[winner] >= COP_BASE) && (addr[winner] <= COP_END);
    wire is_cop_amiga  = (addr[winner] >= COP_AMIGA_BASE) && (addr[winner] <= COP_AMIGA_END);
    wire is_cop_reg    = is_cop_legacy | is_cop_amiga;
    wire is_den_legacy = LEGACY_CHIPSET &&
                         (((addr[winner] >= DEN_BASE)  && (addr[winner] <= DEN_END)) ||
                          ((addr[winner] >= DEN2_BASE) && (addr[winner] <= DEN2_END)));
    wire is_den_amiga  = ((addr[winner] >= DEN_AMIGA_BASE)  && (addr[winner] <= DEN_AMIGA_END)) ||
                         ((addr[winner] >= DEN2_AMIGA_BASE) && (addr[winner] <= DEN2_AMIGA_END));
    wire is_den_reg = is_den_legacy | is_den_amiga;
    wire den_bank   = ((addr[winner] >= DEN2_BASE)       && (addr[winner] <= DEN2_END)) ||
                      ((addr[winner] >= DEN2_AMIGA_BASE) && (addr[winner] <= DEN2_AMIGA_END));
    wire is_pau_legacy = LEGACY_CHIPSET && (addr[winner] >= PAU_BASE) && (addr[winner] <= PAU_END);
    // Agnus stub steals four addresses out of the Paula window.  Strip them
    // here so they don't reach the Paula slave.
    wire is_agnus_reg  = (addr[winner] == DMACONR_ADDR) ||
                          (addr[winner] == VPOSR_ADDR)   ||
                          (addr[winner] == VHPOSR_ADDR)  ||
                          (addr[winner] == DMACON_ADDR);
    // Read-only stubs (SERDATR / POTGOR / etc.) — return canned values
    // suitable for early Kickstart probes.  Writes are silently dropped.
    wire is_paula_ro_reg = (addr[winner] == SERDATR_ADDR) ||
                           (addr[winner] == POTGOR_ADDR)  ||
                           (addr[winner] == ADKCONR_ADDR) ||
                           (addr[winner] == POT0DAT_ADDR) ||
                           (addr[winner] == POT1DAT_ADDR) ||
                           (addr[winner] == JOY0DAT_ADDR) ||
                           (addr[winner] == JOY1DAT_ADDR) ||
                           (addr[winner] == DSKBYTR_ADDR) ||
                           (addr[winner] == DSKDATR_ADDR) ||
                           (addr[winner] == DSKSYNC_ADDR);
    wire is_pau_amiga  = (addr[winner] >= PAU_AMIGA_BASE) && (addr[winner] <= PAU_AMIGA_END)
                         && !is_agnus_reg && !is_paula_ro_reg && !is_shadow_reg;
    wire is_autoconfig_reg = (addr[winner] >= AUTOCONFIG_BASE) &&
                              (addr[winner] <= AUTOCONFIG_END);
    wire is_kbd_inject_reg = LEGACY_CHIPSET && (addr[winner] == KBD_INJECT_ADDR);
    wire is_bpl_probe_reg = LEGACY_CHIPSET && (
                             (addr[winner] == BPL_FETCH_COUNTER_ADDR) ||
                             (addr[winner] == BPL1DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPL2DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPL3DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPL4DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPL5DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPL6DAT_PROBE_ADDR)    ||
                             (addr[winner] == BPLCON0_PROBE_ADDR));
    wire is_pau_reg  = is_pau_legacy | is_pau_amiga;
    wire is_ciaa_legacy = LEGACY_CHIPSET && (addr[winner] >= CIAA_BASE) && (addr[winner] <= CIAA_END);
    wire is_ciab_legacy = LEGACY_CHIPSET && (addr[winner] >= CIAB_BASE) && (addr[winner] <= CIAB_END);
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

    // ----- Canonical-write translation (Blitter / Copper) -------------
    // 16-bit Amiga write data sits in the high or low half of the 32-bit
    // bus word depending on byte address bit 1 (big-endian: addr+0 in
    // bits[31:16], addr+2 in bits[15:0]).
    wire        amiga_high_half  = (addr[winner][1] == 1'b0);
    wire [15:0] amiga_wdata_half = amiga_high_half ? wdata[winner][31:16]
                                                   : wdata[winner][15:0];
    // Canonical register index within the 64-byte Blitter / 16-byte
    // Copper window (1 reg = 2 bytes).
    wire [4:0]  blt_amiga_reg    = addr[winner][5:1];
    wire [2:0]  cop_amiga_reg    = addr[winner][3:1];

    // Translate canonical Blitter write to internal slave (32-bit, 16
    // long-aligned regs).  Internal layout (per blitter.v):
    //   $00 BLTCON   $04 BLTAFWM  $08 BLTALWM
    //   $0C BLTAPT   $10 BLTBPT   $14 BLTCPT   $18 BLTDPT
    //   $1C BLTAMOD  $20 BLTBMOD  $24 BLTCMOD  $28 BLTDMOD
    //   $2C BLTADAT  $30 BLTBDAT  $34 BLTCDAT
    //   $38 BLTSIZE (trigger)     $3C BLTSTAT (RO)
    reg [5:0]  blt_xlat_addr;
    reg [31:0] blt_xlat_wdata;
    reg        blt_xlat_valid;
    always @* begin
        blt_xlat_addr  = 6'h00;
        blt_xlat_wdata = 32'd0;
        blt_xlat_valid = 1'b0;
        case (blt_amiga_reg)
            5'd0: begin                       // BLTCON0 ($DFF040)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h00;
                blt_xlat_wdata = {
                    amiga_wdata_half[7:0],       // LF       [31:24]
                    amiga_wdata_half[15:12],     // ASH      [23:20]
                    canon_bltcon1[15:12],        // BSH      [19:16]
                    canon_bltcon1[1],            // DESC     [15]
                    canon_bltcon1[3],            // IFE      [14]
                    canon_bltcon1[4],            // EFE      [13]
                    canon_bltcon1[2],            // FCI      [12]
                    canon_bltcon1[0],            // LINE     [11]
                    canon_bltcon1[3:1],          // oct      [10:8]
                    4'd0,                        //          [7:4]
                    amiga_wdata_half[11:8]       // USEA-D   [3:0]
                };
            end
            5'd1: begin                       // BLTCON1 ($DFF042)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h00;
                blt_xlat_wdata = {
                    canon_bltcon0[7:0],
                    canon_bltcon0[15:12],
                    amiga_wdata_half[15:12],
                    amiga_wdata_half[1],
                    amiga_wdata_half[3],
                    amiga_wdata_half[4],
                    amiga_wdata_half[2],
                    amiga_wdata_half[0],
                    amiga_wdata_half[3:1],
                    4'd0,
                    canon_bltcon0[11:8]
                };
            end
            5'd2: begin                       // BLTAFWM (.L writes commit BLTALWM too)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h04;
                // K1.3 trackdisk does MOVE.L #$FFFFFFFF, $DFF044 to
                // set both BLTAFWM and BLTALWM in one instruction.
                // Without passing the full 32-bit wdata, BLTALWM retains
                // its stale value from the previous blit ($FFFC was
                // observed at the WB1.3 bootblock re-read at r=116M),
                // causing single-bit corruption in MFM decode.
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {16'd0, amiga_wdata_half};
            end
            5'd3: begin                       // BLTALWM
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h08;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            // For the 32-bit BLT pointers (BLTAPT/BPT/CPT/DPT), a single
            // MOVE.L #val, $DFFnnn touches the H register address but
            // carries BOTH halves in wdata (high half at [31:16] = the H
            // reg, low half at [15:0] = the L reg).  We must commit the
            // FULL wdata to the slave -- otherwise the L half is lost
            // and the pointer reads as $XXXX_0000 with the L half zero.
            // K1.3 boot caught this: blitter clobbered ExecBase at $4
            // because BLTDPT was effectively $0 after MOVE.L sets, and
            // the blitter walked from BLTDPT=$0 / $2 / $4 / ...
            5'd4: begin                       // BLTCPTH (.L writes commit full)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h14;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {amiga_wdata_half, canon_bltcpt[15:0]};
            end
            5'd5: begin                       // BLTCPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h14;
                blt_xlat_wdata = {canon_bltcpt[31:16], amiga_wdata_half};
            end
            5'd6: begin                       // BLTBPTH (.L writes commit full)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h10;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {amiga_wdata_half, canon_bltbpt[15:0]};
            end
            5'd7: begin                       // BLTBPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h10;
                blt_xlat_wdata = {canon_bltbpt[31:16], amiga_wdata_half};
            end
            5'd8: begin                       // BLTAPTH (.L writes commit full)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h0C;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {amiga_wdata_half, canon_bltapt[15:0]};
            end
            5'd9: begin                       // BLTAPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h0C;
                blt_xlat_wdata = {canon_bltapt[31:16], amiga_wdata_half};
            end
            5'd10: begin                      // BLTDPTH (.L writes commit full)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h18;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {amiga_wdata_half, canon_bltdpt[15:0]};
            end
            5'd11: begin                      // BLTDPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h18;
                blt_xlat_wdata = {canon_bltdpt[31:16], amiga_wdata_half};
            end
            5'd12: begin                      // BLTSIZE -- starts blit
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h38;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            5'd16: begin                      // BLTCMOD ($DFF060)
                // .L writes also commit BLTBMOD ($DFF062), same pattern as
                // the BLTAFWM/BLTALWM split at 5'd2.  K1.3 graphics.library
                // does MOVE.L to $DFF060 to set BOTH BLTCMOD and BLTBMOD.
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h24;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd17: begin                      // BLTBMOD ($DFF062)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h20;
                blt_xlat_wdata = {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd18: begin                      // BLTAMOD ($DFF064)
                // .L writes also commit BLTDMOD ($DFF066), same pattern
                // as BLTAFWM/BLTALWM split.  K1.3 graphics.library at
                // PC $FC5CFE does `MOVE.L D0, $64(A0)` to set BOTH BLTAMOD
                // and BLTDMOD in one instruction.  Without this split,
                // BLTDMOD retains its stale value from the previous blit;
                // for the WB1.3 boot wall, a previous blit had BLTDMOD=-90
                // and the corrupting bitmap blit inherited it, walking the
                // dst pointer DOWN 12 bytes/row through K1.3's BCPL DOS
                // pool data at $5E40.  See project_wb13_dialog_chain.md.
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h1C;
                blt_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd19: begin                      // BLTDMOD ($DFF066)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h28;
                blt_xlat_wdata = {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd24: begin                      // BLTCDAT ($DFF070)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h34;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            5'd25: begin                      // BLTBDAT ($DFF072)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h30;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            5'd26: begin                      // BLTADAT ($DFF074)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h2C;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            // BLTCON0L/BLTSIZV/BLTSIZH (ECS) and reserved slots drop.
            default: ;
        endcase
    end

    // Translate canonical Copper write to internal slave (32-bit).
    // Internal copper.v register layout:
    //   $00 COP1LC   $04 COPJMP1 (strobe)  $08 COPSTAT (RO)
    //   $0C COP2LC   $10 COPJMP2 (strobe)
    reg [5:0]  cop_xlat_addr;
    reg [31:0] cop_xlat_wdata;
    reg        cop_xlat_valid;
    // OCS Agnus only has 19 address lines for COP1LC/COP2LC; bits 31:19
    // of any value written to COP1LCH / COP2LCH are silently dropped.
    // When the Copper master itself executes a "MOVE COP1LCH=$xxxx"
    // instruction (a 16-bit half-word write), we must mirror that
    // truncation, otherwise a runaway Copper that walks past the end
    // of a list into chip-RAM garbage and decodes a stray $xxxx as
    // COP1LCH ends up with cop1lc pointing past chip RAM — where the
    // arbiter returns zeros forever — permanently parking the Copper.
    // CPU MOVE.L writes to $DFF080/$DFF084 fall through `is_long &&
    // be==1111` and use the full 32-bit wdata, so this gate only
    // catches the 16-bit half-word path used by Copper-self writes
    // and a hypothetical CPU MOVE.W (which K1.3 never does to these).
    // The COP_MASTER_PORT computation mirrors m68k_top's localparam
    // COP_PORT = 2*N_CORES + 1; with N_PORTS = 2*N_CORES + 4 (4 chipset
    // masters), that is also N_PORTS - 3.
    wire        winner_is_cop = (winner == (N_PORTS - 3));
    wire [15:0] cop_self_high_masked = winner_is_cop
                                       ? {13'd0, amiga_wdata_half[2:0]}
                                       : amiga_wdata_half;
    always @* begin
        cop_xlat_addr  = 6'h00;
        cop_xlat_wdata = 32'd0;
        cop_xlat_valid = 1'b0;
        case (cop_amiga_reg)
            // COP1LC/COP2LC are 32-bit pointers; MOVE.L to the H address
            // carries BOTH halves in wdata.  Without picking up the low
            // half, the L reg is silently zeroed.  Mirror the BLT pointer
            // fix at lines 808-855.
            3'd0: begin                       // COP1LCH ($DFF080)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h00;
                cop_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {cop_self_high_masked, canon_cop1lc[15:0]};
            end
            3'd1: begin                       // COP1LCL ($DFF082)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h00;
                cop_xlat_wdata = {canon_cop1lc[31:16], amiga_wdata_half};
            end
            3'd2: begin                       // COP2LCH ($DFF084)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h0C;
`ifdef HACK_COP1LC_TO_WBSCREEN
                // Diagnostic hack: K1.3 VBL handler at $FC6D6C writes
                // COP2LC=$000100C8 every frame, but the buffer at
                // $100C8 was never assembled with valid Copper
                // instructions (MrgCop never runs).  Substitute
                // $00C05C40 — the Workbench Screen's VP_DspIns Copper
                // list which DOES have valid BPL/BPLCON0 setup.
                cop_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? (wdata[winner] == 32'h0001_00C8
                                    ? 32'h00C0_5C40
                                    : wdata[winner])
                                 : {cop_self_high_masked, canon_cop2lc[15:0]};
`else
                cop_xlat_wdata = (is_long[winner] && be[winner] == 4'b1111)
                                 ? wdata[winner]
                                 : {cop_self_high_masked, canon_cop2lc[15:0]};
`endif
            end
            3'd3: begin                       // COP2LCL ($DFF086)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h0C;
                cop_xlat_wdata = {canon_cop2lc[31:16], amiga_wdata_half};
            end
            3'd4: begin                       // COPJMP1 ($DFF088)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h04;
                cop_xlat_wdata = 32'd0;
            end
            3'd5: begin                       // COPJMP2 ($DFF08A)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h10;
                cop_xlat_wdata = 32'd0;
            end
            // COPINS ($DFF08C) / COPCON ($DFF08E): drop silently.
            default: ;
        endcase
    end

    // Combinational slave ports.  When the winning access hits the blitter
    // or Copper register space, we route the request to the appropriate
    // slv_* and prevent the memory write.  Canonical writes are translated
    // (bit-layout and address) before reaching the slave; legacy writes are
    // passed through unmodified.  Reads return slv_rdata via the delayed-
    // response path (legacy reads only -- canonical reads fall through to
    // the chipset shadow chip_regs[]).
    always @* begin
        if (is_blt_amiga) begin
            blt_slv_req     = winner_valid && we[winner] && blt_xlat_valid;
            blt_slv_we      = winner_valid && we[winner] && blt_xlat_valid;
            blt_slv_addr    = blt_xlat_addr;
            blt_slv_be      = 4'b1111;
            // is_long signals "CPU did a 32-bit write that spans two 16-bit
            // chipset regs" — only valid for the Amiga $DFFnnn path where
            // adjacent regs (BLTAFWM/BLTALWM, BLTAPTH/BLTAPTL, etc.) live
            // at adjacent addresses.  The legacy path uses 32-bit-aligned
            // internal regs where the CPU's .L write targets a single reg.
            blt_slv_is_long = is_long[winner] && (be[winner] == 4'b1111);
            blt_slv_wdata   = blt_xlat_wdata;
        end else begin
            blt_slv_req     = winner_valid && is_blt_legacy;
            blt_slv_we      = winner_valid && is_blt_legacy && we[winner];
            blt_slv_addr    = addr[winner][5:0];
            blt_slv_be      = be[winner];
            blt_slv_is_long = 1'b0;   // legacy regs are 32-bit-aligned standalone
            blt_slv_wdata   = wdata[winner];
        end
    end
`ifdef KICKSTART_BOOT_TRACE
    // Trace every write to the blitter slave port targeting any of the
    // pointer regs (offsets $0C BLTAPT, $10 BLTBPT, $14 BLTCPT, $18 BLTDPT).
    always @(posedge clk) if (rst_n && blt_slv_req && blt_slv_we &&
                              (blt_slv_addr[5:2] == 4'h3 ||
                               blt_slv_addr[5:2] == 4'h4 ||
                               blt_slv_addr[5:2] == 4'h5 ||
                               blt_slv_addr[5:2] == 4'h6)) begin
        $display("[BLT-SLV-PTR-WR] port=%0d via=%s slv_addr=%h wdata=%h be=%b raw_addr=%h",
            winner, is_blt_amiga ? "AMIGA" : "LEGACY",
            blt_slv_addr, blt_slv_wdata, blt_slv_be, addr[winner]);
    end
`endif
    always @* begin
        if (is_cop_amiga) begin
            cop_slv_req   = winner_valid && we[winner] && cop_xlat_valid;
            cop_slv_we    = winner_valid && we[winner] && cop_xlat_valid;
            cop_slv_addr  = cop_xlat_addr;
            cop_slv_be    = 4'b1111;
            cop_slv_wdata = cop_xlat_wdata;
        end else begin
            cop_slv_req   = winner_valid && is_cop_legacy;
            cop_slv_we    = winner_valid && is_cop_legacy && we[winner];
            cop_slv_addr  = addr[winner][5:0];
            cop_slv_be    = be[winner];
            cop_slv_wdata = wdata[winner];
        end
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
            granted_is_long_q <= 1'b0;
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
            granted_is_slow_q  <= 1'b0;
            granted_slow_idx_q <= 17'd0;
            ovl_active <= OVL_RESET;
            ovl_just_cleared <= 1'b0;
            irq_level <= 3'd0;
            blk_src   <= 32'd0;
            blk_dst   <= 32'd0;
            blk_cnt   <= 32'd0;
            blk_busy  <= 1'b0;
            blk_busy_last <= 1'b0;
            blk_cur_off <= 32'd0;
            blk_cur_dst <= 32'd0;
            blk_byte_mode <= 1'b0;
            blk_count_in_bytes <= 32'd0;
            dsk_pt    <= 32'd0;
            dsk_len   <= 16'd0;
            dsksync   <= 16'h4489;  // power-on default per Amiga HRM
            adkcon    <= 15'h0000;
            autoconfig_shutup  <= 1'b0;
            autoconfig_base_hi <= 8'd0;
            autoconfig_base_lo <= 8'd0;
            kbd_inject_wr   <= 1'b0;
            kbd_inject_byte <= 8'd0;
            for (mi = 0; mi < 6; mi = mi + 1) begin
                bpl_pt[mi]  <= 32'd0;
                bpl_dat[mi] <= 16'd0;
            end
            bpl_fetches     <= 32'd0;
            bpl_fetch_tick  <= 10'd0;
            bpl_plane_idx   <= 3'd0;
            bplcon0_shadow  <= 16'd0;
            bpl1mod_shadow  <= 16'd0;
            bpl2mod_shadow  <= 16'd0;
            bpl_was_active  <= 1'b0;
            agnus_v_prev    <= 10'd0;
            canon_bltcon0   <= 16'd0;
            canon_bltcon1   <= 16'd0;
            canon_bltapt    <= 32'd0;
            canon_bltbpt    <= 32'd0;
            canon_bltcpt    <= 32'd0;
            canon_bltdpt    <= 32'd0;
            canon_cop1lc    <= 32'd0;
            canon_cop2lc    <= 32'd0;
        end else begin
            // Default keyboard inject pulse to 0 each cycle; the write
            // path below asserts it for one cycle when triggered.
            kbd_inject_wr <= 1'b0;

            // ----- Agnus bitplane DMA (multi-plane) ------------------
            // On a 0->1 transition of "engine active" (BPLEN & DMAEN &
            // BPU>0), reload all six running pointers from the BPLnPT
            // chipset shadow.  Then, every BPL_FETCH_DIV host clocks,
            // fetch one half-word for plane bpl_plane_idx, advance that
            // plane's pointer, and rotate plane_idx through 0..BPU-1.
            agnus_v_prev   <= agnus_v;
            bpl_was_active <= bpl_active_now;
            if (bpl_active_now && !bpl_was_active) begin
                // Pre-load running pointers from chip_regs[$E0..$F6].
                // BPLnPT high half lives at $E0+(n-1)*4, low half at +2.
                bpl_pt[0] <= {chip_regs[9'h0E0], chip_regs[9'h0E2]};
                bpl_pt[1] <= {chip_regs[9'h0E4], chip_regs[9'h0E6]};
                bpl_pt[2] <= {chip_regs[9'h0E8], chip_regs[9'h0EA]};
                bpl_pt[3] <= {chip_regs[9'h0EC], chip_regs[9'h0EE]};
                bpl_pt[4] <= {chip_regs[9'h0F0], chip_regs[9'h0F2]};
                bpl_pt[5] <= {chip_regs[9'h0F4], chip_regs[9'h0F6]};
                bpl_fetch_tick <= 10'd0;
                bpl_plane_idx  <= 3'd0;
            end else if (bpl_active_now) begin
                if (bpl_fetch_tick == BPL_FETCH_DIV - 1) begin
                    bpl_fetch_tick <= 10'd0;
                    // Fetch one half-word for the current plane.  Sources
                    // from chip RAM or Agnus slow-RAM mirror depending on
                    // where bpl_pt points.
                    if (is_slow_addr(bpl_pt[bpl_plane_idx])) begin
                        bpl_dat[bpl_plane_idx] <=
                            bpl_pt[bpl_plane_idx][1] == 1'b0
                                ? slowmem[slow_idx_of(bpl_pt[bpl_plane_idx])][31:16]
                                : slowmem[slow_idx_of(bpl_pt[bpl_plane_idx])][15:0];
                    end else if (bpl_pt[bpl_plane_idx][1] == 1'b0) begin
                        bpl_dat[bpl_plane_idx] <=
                            mem[bpl_pt[bpl_plane_idx][AIDX_BITS+1:2]][31:16];
                    end else begin
                        bpl_dat[bpl_plane_idx] <=
                            mem[bpl_pt[bpl_plane_idx][AIDX_BITS+1:2]][15:0];
                    end
                    bpl_pt[bpl_plane_idx] <= bpl_pt[bpl_plane_idx] + 32'd2;
                    bpl_fetches <= bpl_fetches + 32'd1;
                    // Rotate plane index mod BPU.
                    if (bpl_plane_idx + 3'd1 >= bpu) bpl_plane_idx <= 3'd0;
                    else                            bpl_plane_idx <= bpl_plane_idx + 3'd1;
                end else begin
                    bpl_fetch_tick <= bpl_fetch_tick + 10'd1;
                end
            end

            // Snoop BPLCON0 writes (at canonical $DFF100) so the DMA
            // engine sees the BPU bits.  Denise owns the same address
`ifdef BLT_DAT_2AAA_TRACE
            // Trace any CPU write to a blitter data register where data
            // half == $2AAA (Intuition's inactive-window 50% gray fill
            // pattern, per WB13_DEBUG_JOURNAL §29).
            //   $DFF070 = BLTCDAT
            //   $DFF072 = BLTBDAT
            //   $DFF074 = BLTADAT
            // Whoever writes $2AAA here picked "draw inactive" — finding
            // their K1.3 ROM PC is the next step toward identifying the
            // active/inactive predicate.
            if (winner_valid && we[winner] &&
                (addr[winner] == 32'h00DF_F070 ||
                 addr[winner] == 32'h00DF_F072 ||
                 addr[winner] == 32'h00DF_F074) &&
                (wdata[winner][31:16] == 16'h2AAA ||
                 wdata[winner][15:0]  == 16'h2AAA)) begin
                $display("[BLT_DAT_2AAA] port=%0d addr=%h wdata=%h be=%b is_long=%b",
                    winner, addr[winner], wdata[winner], be[winner], is_long[winner]);
            end
`endif
            // for its own register file; this snoop is in parallel.
            if (winner_valid && we[winner] && (addr[winner] == 32'h00DF_F100)) begin
                // 16-bit BPLCON0 lives in the high half of the long
                // at $DFF100 (addr[1]==0 -> high half).
                if (be[winner][3] | be[winner][2])
                    bplcon0_shadow <= wdata[winner][31:16];
                else
                    bplcon0_shadow <= wdata[winner][15:0];
            end
            // Snoop BPL1MOD ($DFF108, addr[1]==0 -> high half) and
            // BPL2MOD ($DFF10A, addr[1]==1 -> low half).  Both live in
            // the same aligned long word at $DFF108.  Long-aligned 32-bit
            // writes pack BPL1MOD into [31:16] and BPL2MOD into [15:0].
            if (winner_valid && we[winner] && (addr[winner] == 32'h00DF_F108)) begin
                if (be[winner][3] | be[winner][2])
                    bpl1mod_shadow <= wdata[winner][31:16];
                if (be[winner][1] | be[winner][0])
                    bpl2mod_shadow <= wdata[winner][15:0];
            end else if (winner_valid && we[winner] && (addr[winner] == 32'h00DF_F10A)) begin
                // 16-bit-only write to BPL2MOD; the bus presents it in
                // the low half of the long word.
                if (be[winner][1] | be[winner][0])
                    bpl2mod_shadow <= wdata[winner][15:0];
                else if (be[winner][3] | be[winner][2])
                    bpl2mod_shadow <= wdata[winner][31:16];
            end
            // ----- Canonical Blitter/Copper shadow updates ------------
            // The slave-port mux already composed the new 32-bit value
            // for the internal blitter/copper this cycle (using the OLD
            // canon_blt*/canon_cop* values).  Update the shadow now so
            // the NEXT canonical write to the partner half of the same
            // pointer/control sees the new piece.
            if (winner_valid && we[winner] && is_blt_amiga) begin
                case (blt_amiga_reg)
                    5'd0:  canon_bltcon0       <= amiga_wdata_half;
                    5'd1:  canon_bltcon1       <= amiga_wdata_half;
                    // 32-bit pointer writes: when CPU does MOVE.L to the H
                    // address, capture BOTH halves from wdata so a follow-up
                    // .W write to the partner half doesn't re-zero the L.
                    5'd4:  if (is_long[winner] && be[winner] == 4'b1111)
                                canon_bltcpt <= wdata[winner];
                           else canon_bltcpt[31:16] <= amiga_wdata_half;
                    5'd5:  canon_bltcpt[15:0]  <= amiga_wdata_half;
                    5'd6:  if (is_long[winner] && be[winner] == 4'b1111)
                                canon_bltbpt <= wdata[winner];
                           else canon_bltbpt[31:16] <= amiga_wdata_half;
                    5'd7:  canon_bltbpt[15:0]  <= amiga_wdata_half;
                    5'd8:  if (is_long[winner] && be[winner] == 4'b1111)
                                canon_bltapt <= wdata[winner];
                           else canon_bltapt[31:16] <= amiga_wdata_half;
                    5'd9:  canon_bltapt[15:0]  <= amiga_wdata_half;
                    5'd10: if (is_long[winner] && be[winner] == 4'b1111)
                                canon_bltdpt <= wdata[winner];
                           else canon_bltdpt[31:16] <= amiga_wdata_half;
                    5'd11: canon_bltdpt[15:0]  <= amiga_wdata_half;
                    default: ;
                endcase
`ifdef KICKSTART_BOOT_TRACE
                // Trace any CPU/master write to a BLT canonical register —
                // diagnoses where bltdpt=$01FF (and other oddly-aligned
                // pointers) come from for the blit that corrupts mem[$1B].
                if (blt_amiga_reg == 5'd10 || blt_amiga_reg == 5'd11)
                    $display("[BLT-CANON-DPT-WR] port=%0d reg=%0d is_long=%b be=%b wdata=%h half=%h canon_dpt(after)=%h",
                        winner, blt_amiga_reg, is_long[winner],
                        be[winner], wdata[winner], amiga_wdata_half,
                        (blt_amiga_reg == 5'd10)
                            ? ((is_long[winner] && be[winner] == 4'b1111)
                               ? wdata[winner]
                               : {amiga_wdata_half, canon_bltdpt[15:0]})
                            : {canon_bltdpt[31:16], amiga_wdata_half});
`endif
            end
            if (winner_valid && we[winner] && is_cop_amiga) begin
                // Mirror the xlat-side mask: a Copper-master write to
                // COP1LCH/COP2LCH must also store the 19-bit-truncated
                // value in the canon shadow.  Otherwise the next half-
                // word write to COP1LCL (which reads canon_cop1lc[31:16]
                // back to reconstruct the full 32-bit address) re-
                // contaminates the slave with the dropped upper bits.
                case (cop_amiga_reg)
                    3'd0: if (is_long[winner] && be[winner] == 4'b1111)
                                canon_cop1lc <= wdata[winner];
                          else canon_cop1lc[31:16] <= cop_self_high_masked;
                    3'd1: canon_cop1lc[15:0]  <= amiga_wdata_half;
                    3'd2: if (is_long[winner] && be[winner] == 4'b1111)
                                canon_cop2lc <= wdata[winner];
                          else canon_cop2lc[31:16] <= cop_self_high_masked;
                    3'd3: canon_cop2lc[15:0]  <= amiga_wdata_half;
                    default: ;
                endcase
            end

            // Block-device DMA tick: while busy, copy one disk word to mem
            // per clock.  Two cycles of pre-arbiter access is fine — we
            // bypass the bus arbiter entirely (this is a "ghost" master that
            // writes mem[] directly, simulating an off-CPU controller).
            if (blk_busy) begin
                if ((blk_byte_mode && blk_cur_dst >= blk_dst + blk_count_in_bytes) ||
                    (!blk_byte_mode && blk_cur_dst >= blk_dst + (blk_cnt << 9))) begin
                    blk_busy <= 1'b0;
                    blk_byte_mode <= 1'b0;
                end else if (is_slow_addr(blk_cur_dst)) begin
                    // Disk DMA into Agnus trapdoor slow RAM range.
                    slowmem[slow_idx_of(blk_cur_dst)] <= disk[blk_cur_off[DISK_IDX_BITS+1:2]];
                end else begin
                    mem[blk_cur_dst[AIDX_BITS+1:2]] <= disk[blk_cur_off[DISK_IDX_BITS+1:2]];
`ifdef KICKSTART_BOOT_TRACE
                    // Trace disk DMA writes near the vector table ($0..$FF)
                    // — disk DMA bypasses the bus arbiter so VEC-BUS-WR
                    // misses these.  We need to know if disk DMA is what
                    // clears mem[$1B] to $0 after the legit write.
                    if (blk_cur_dst < 32'h0000_0100)
                        $display("[DMA-LOW-WR] dst=%h off=%h data=%h",
                            blk_cur_dst, blk_cur_off,
                            disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
                    // [BB-DEC-DMA-WR]: catch disk DMA writes to the bootblock
                    // decoded-magic location $20A0..$20A3.  Counterpart to the
                    // arbiter-side BB-DEC-WR probe — disk DMA bypasses the
                    // arbiter.
                    if (blk_cur_dst >= 32'h0000_20A0 && blk_cur_dst <= 32'h0000_20A3)
                        $display("[BB-DEC-DMA-WR] dst=%h off=%h data=%h",
                            blk_cur_dst, blk_cur_off,
                            disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
`endif
`ifdef HDRCHK_WATCH
                    if (blk_cur_dst >= 32'h0000_64A8 && blk_cur_dst <= 32'h0000_64E7)
                        $display("[HDRCHK_WR] cyc=%0d DMA dst=%h off=%h data=%h",
                            cyc_count, blk_cur_dst, blk_cur_off, disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
                    if (blk_cur_dst >= 32'h0000_68E0 && blk_cur_dst <= 32'h0000_68FB)
                        $display("[SEC1_WR] cyc=%0d DMA dst=%h off=%h data=%h",
                            cyc_count, blk_cur_dst, blk_cur_off, disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
`endif
`ifdef KICKSTART_BOOT_TRACE
                    // [CACA-WR-DMA]: counterpart to the CPU CACA-WR trace
                    // in m68k_core.v.  Disk DMA writes bypass the CPU's
                    // dc_we path, so $CACA fills coming from MFM-decoded
                    // disk data wouldn't otherwise show up.  Catch any
                    // longword OR either half containing $CACA.
                    if (disk[blk_cur_off[DISK_IDX_BITS+1:2]][31:16] == 16'hCACA ||
                        disk[blk_cur_off[DISK_IDX_BITS+1:2]][15:0]  == 16'hCACA)
                        $display("[CACA-WR-DMA] dst=%h off=%h data=%h",
                            blk_cur_dst, blk_cur_off,
                            disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
`endif
                    blk_cur_off <= blk_cur_off + 32'd4;
                    blk_cur_dst <= blk_cur_dst + 32'd4;
                end
            end
            blk_busy_last <= blk_busy;
            // Writes to the IRQ register update irq_level (sticky) and do NOT
            // commit to main memory. Writes to the blitter register region
            // are handled by the combinational slave port (no memory write).
            // Writes anywhere else update memory.
            if (winner_valid && we[winner] && is_kbd_inject_reg) begin
                // Pick the byte in whatever lane the CPU drove; for a
                // canonical move.b dest the byte sits in be[0]..be[3]
                // depending on address.  Use the first non-zero lane.
                if (be[winner][3]) kbd_inject_byte <= wdata[winner][31:24];
                else if (be[winner][2]) kbd_inject_byte <= wdata[winner][23:16];
                else if (be[winner][1]) kbd_inject_byte <= wdata[winner][15:8];
                else if (be[winner][0]) kbd_inject_byte <= wdata[winner][7:0];
                kbd_inject_wr <= 1'b1;
            end else if (winner_valid && we[winner] && is_irq_reg) begin
                if (be[winner][0]) irq_level <= wdata[winner][2:0];
            end else if (winner_valid && we[winner] && is_dsk_reg) begin
                // Floppy DSKPT/DSKLEN writes.  The CPU presents (addr, be,
                // is_long, wdata) where:
                //   - aligned long at $DFF020: addr=$DFF020 be=1111 is_long=1,
                //     wdata = full 32-bit ptr -> set DSKPT to wdata.
                //   - word at $DFF020 (high half): addr=$DFF020 be=1100,
                //     wdata[31:16] = the 16-bit value -> DSKPTH.
                //   - word at $DFF022 (low half of $DFF020 long): addr=$DFF020
                //     be=0011 wdata[15:0] = value -> DSKPTL.
                //   - word at $DFF022 (direct addr): addr=$DFF022 be=0011
                //     wdata[15:0] = value -> DSKPTL.
                //   - unaligned long at $DFF022: addr=$DFF022 be=1111
                //     is_long=1.  Spans DSKPTL ($DFF022,23) +
                //     DSKLEN ($DFF024,25).  Bytes: $DFF022 <- wdata[31:24],
                //     $DFF023 <- wdata[23:16], $DFF024 <- wdata[15:8],
                //     $DFF025 <- wdata[7:0].  So DSKPTL <- wdata[31:16],
                //     DSKLEN <- wdata[15:0].
                //   - word at $DFF024 (DSKLEN, high half of long $DFF024):
                //     addr=$DFF024 be=1100 wdata[31:16] = value -> DSKLEN.
                //     (Some emitters use be=0011 with wdata[15:0]; handle
                //     that too.)
                //
                // We trigger floppy DMA on writes that put a non-zero
                // value in DSKLEN with bit 15 set.
                if (addr[winner] == DSKPTH_ADDR) begin
                    if (is_long[winner] && be[winner] == 4'b1111) begin
                        // Aligned MOVE.L Dn, $DFF020 -- full DSKPT.
                        dsk_pt <= wdata[winner];
                    end else if (be[winner] == 4'b1100) begin
                        // MOVE.W Dn, $DFF020 -- DSKPTH only.
                        dsk_pt[31:16] <= wdata[winner][31:16];
                    end else if (be[winner] == 4'b0011) begin
                        // MOVE.W Dn, $DFF022 emitted with long-aligned addr.
                        // wdata[15:0] goes to bytes $DFF022,$DFF023 = DSKPTL.
                        dsk_pt[15:0] <= wdata[winner][15:0];
                    end
                end else if (addr[winner] == DSKPTL_ADDR) begin
                    if (is_long[winner] && be[winner] == 4'b1111) begin
                        // Unaligned MOVE.L Dn, $DFF022 -- spans DSKPTL +
                        // DSKLEN.  Bytes: 022,023 <- wdata[31:16] (DSKPTL),
                        // 024,025 <- wdata[15:0] (DSKLEN).  If this DSKLEN
                        // write has DMAEN (bit 31 = wdata[31] for old high
                        // half, but here it's wdata[15]), kick off DMA.
                        dsk_pt[15:0] <= wdata[winner][31:16];
                        dsk_len      <= wdata[winner][15:0];
                        if (wdata[winner][15] && !blk_busy) begin
                            blk_busy    <= 1'b1;
                            blk_byte_mode <= 1'b1;
                            // WORDSYNC=1: skip $AAAA gap AND the $4489 sync
                            // word so the buffer starts at post-sync data
                            // (matching real Amiga PLL behaviour where DMA
                            // consumes the sync word for alignment and
                            // doesn't write it to memory).  adf2mfm emits
                            // 4 bytes gap ($AAAAAAAA) + 4 bytes sync
                            // ($44894489) before sector data = 8 bytes total.
                            blk_cur_off <= adkcon_wordsync ? 32'd2 : 32'd0;
                            // Combine DSKPTH (already set) + new DSKPTL.
                            blk_cur_dst <= {dsk_pt[31:16], wdata[winner][31:16]};
                            blk_dst     <= {dsk_pt[31:16], wdata[winner][31:16]};
                            blk_count_in_bytes <= {17'd0, wdata[winner][13:0]} << 1;
                        end
                    end else if (be[winner] == 4'b0011 || be[winner] == 4'b1111) begin
                        // Word write at $DFF022 (either lane-form).
                        dsk_pt[15:0] <= wdata[winner][15:0];
                    end else if (be[winner] == 4'b1100) begin
                        // Word at $DFF022 with high-lane form (unusual).
                        dsk_pt[15:0] <= wdata[winner][31:16];
                    end
                end else if (addr[winner] == DSKLEN_ADDR) begin
`ifdef KICKSTART_BOOT_TRACE
                    $display("[DSKLEN] addr=%h be=%b is_long=%b wdata=%h dsk_pt=%h blk_busy=%b track=%0d",
                        addr[winner], be[winner], is_long[winner],
                        wdata[winner], dsk_pt, blk_busy, disk_track_i);
`endif
                    // DSKLEN is at the high half of its longword (addr[1]=0).
                    // K1.3 trackdisk's "arm" sequence writes DSKLEN twice
                    // ($4000 to disarm, $4000 again, $9CBE to arm, $9CBE
                    // again to fire).  The first $9CBE sets DMAEN=1 from 0
                    // and starts DMA; subsequent writes with DMAEN=0 must
                    // STOP the previous DMA so the next $9CBE pair starts
                    // a fresh one (otherwise the prior DMA keeps running
                    // and clobbers buffer modifications K1.3 made between
                    // sectors).
                    if (is_long[winner] && be[winner] == 4'b1111) begin
                        // Aligned MOVE.L Dn, $DFF024 -- DSKLEN in high half
                        // (wdata[31:16]), $DFF026 (DSKDAT) in low half.
                        // Trigger DMA on DMAEN bit set in DSKLEN value.
                        // disk[] is laid out by adf2mfm.py as concatenated
                        // MFM tracks, 11984 bytes each (= 11 sectors * 1088
                        // + 16 inter-track gap).  Offset by the current head
                        // position so dos.library actually gets the track it
                        // asked for instead of always seeing track 0.
                        dsk_len <= wdata[winner][31:16];
                        if (wdata[winner][31] && !blk_busy) begin
                            blk_busy    <= 1'b1;
                            blk_byte_mode <= 1'b1;
                            blk_cur_off <= (adkcon_wordsync ? 32'd2 : 32'd0)
                                         + (32'd11984 * disk_track_i);
                            blk_cur_dst <= dsk_pt;
                            blk_dst     <= dsk_pt;
                            blk_count_in_bytes <= {17'd0, wdata[winner][29:16]} << 1;
                        end else if (!wdata[winner][31]) begin
                            // DMAEN cleared -- abort any running DMA.
                            blk_busy <= 1'b0;
                            blk_byte_mode <= 1'b0;
                        end
                    end else if (be[winner] == 4'b1100) begin
                        dsk_len <= wdata[winner][31:16];
                        if (wdata[winner][31] && !blk_busy) begin
                            blk_busy    <= 1'b1;
                            blk_byte_mode <= 1'b1;
                            blk_cur_off <= (adkcon_wordsync ? 32'd2 : 32'd0)
                                         + (32'd11984 * disk_track_i);
                            blk_cur_dst <= dsk_pt;
                            blk_dst     <= dsk_pt;
                            blk_count_in_bytes <= {17'd0, wdata[winner][29:16]} << 1;
                        end else if (!wdata[winner][31]) begin
                            blk_busy <= 1'b0;
                            blk_byte_mode <= 1'b0;
                        end
                    end else if (be[winner] == 4'b0011) begin
                        // Some emitters send low-lane form.
                        dsk_len <= wdata[winner][15:0];
                        if (wdata[winner][15] && !blk_busy) begin
                            blk_busy    <= 1'b1;
                            blk_byte_mode <= 1'b1;
                            blk_cur_off <= (adkcon_wordsync ? 32'd2 : 32'd0)
                                         + (32'd11984 * disk_track_i);
                            blk_cur_dst <= dsk_pt;
                            blk_dst     <= dsk_pt;
                            blk_count_in_bytes <= {17'd0, wdata[winner][13:0]} << 1;
                        end
                    end
                end else if (addr[winner] == DSKSYNC_ADDR) begin
                    // CPU sets the sync pattern (typically $4489).
                    // Word write -- 16 bits in high half if addr[1]=0,
                    // low half if addr[1]=1.  $DFF07E has addr[1]=1.
                    if (be[winner] == 4'b0011)        dsksync <= wdata[winner][15:0];
                    else if (be[winner] == 4'b1100)   dsksync <= wdata[winner][31:16];
                end else if (addr[winner] == ADKCON_ADDR) begin
                    // ADKCON write: bit 15 = SET (1) / CLR (0), bits 14:0 = mask.
                    // We only model bit 10 (WORDSYNC) -- when set, disk DMA
                    // skips the leading $AAAA gap so the buffer starts at
                    // the first $4489 sync (matching real-hw PLL behavior).
                    // ADKCON lives at $DFF09E (addr[1]=1), so the word is
                    // typically in the low half (be=0011).
                    if (be[winner] == 4'b0011) begin
                        if (wdata[winner][15]) adkcon <= adkcon |  wdata[winner][14:0];
                        else                   adkcon <= adkcon & ~wdata[winner][14:0];
                    end else if (be[winner] == 4'b1100) begin
                        if (wdata[winner][31]) adkcon <= adkcon |  wdata[winner][30:16];
                        else                   adkcon <= adkcon & ~wdata[winner][30:16];
                    end
`ifdef KICKSTART_BOOT_TRACE
                    $display("[ADKCON] addr=%h be=%b wdata=%h", addr[winner], be[winner], wdata[winner]);
`endif
                end
            end else if (winner_valid && we[winner] && is_blk_reg) begin
                // Block-device control register write.  BLKCMD with bit 0
                // set starts the DMA from disk[] -> mem[].
                case (addr[winner])
                    BLKSRC_AD: blk_src <= wdata[winner];
                    BLKDST_AD: blk_dst <= wdata[winner];
                    BLKCNT_AD: blk_cnt <= wdata[winner];
                    BLKCMD_AD: if (wdata[winner][0] && !blk_busy) begin
                        blk_busy    <= 1'b1;
                        blk_cur_off <= blk_src << 9;   // sector * 512
                        blk_cur_dst <= blk_dst;
                    end
                    default: ;
                endcase
            end else if (winner_valid && we[winner] && is_paula_ro_reg) begin
                // Paula read-only stubs swallow writes silently.
            end else if (winner_valid && we[winner] && is_autoconfig_reg) begin
                // Autoconfig configure or shutup write.  Kickstart writes
                // the base-address nibbles at $48 / $4A, then the card
                // "moves" to the new region and stops responding at $E80000.
                // Writes to $4C are an explicit "shutup" (skip this card).
                case (addr[winner][7:0])
                    8'h48: begin
                        // High nibble of base address.  Word write at $48
                        // places the byte in the high lane (be[3]) since
                        // $48 is long-aligned.
                        if (be[winner][3]) autoconfig_base_hi <= wdata[winner][31:24];
                        autoconfig_shutup <= 1'b1;     // configure ends shutup
                    end
                    8'h4A: begin
                        if (be[winner][1]) autoconfig_base_lo <= wdata[winner][15:8];
                        autoconfig_shutup <= 1'b1;
                    end
                    8'h4C: autoconfig_shutup <= 1'b1;
                    default: ;
                endcase
            end else if (winner_valid && we[winner] && is_shadow_reg) begin
                // Shadow-store any chipset register Kickstart writes that
                // we don't have a functional slave for.  16-bit writes
                // come from one or the other half of the 32-bit wdata;
                // for a long write, store BOTH halves into consecutive
                // chip_reg slots.
                if (be[winner][3] | be[winner][2])
                    chip_regs[{chip_idx[8:2], 2'b00}] <= wdata[winner][31:16];
                if (be[winner][1] | be[winner][0])
                    chip_regs[{chip_idx[8:2], 2'b10}] <= wdata[winner][15:0];
`ifdef KICKSTART_BOOT_TRACE
                // Trace bitplane pointer + Copper-list writes that bring
                // up Kickstart's display.  CPU-side cop-list writes are
                // captured separately in m68k_core via [CPUCOP] which
                // gives PC, so we keep this trace narrow.
                if (addr[winner] >= 32'h00DF_F0E0 && addr[winner] <= 32'h00DF_F0FF)
                    $display("[BPLPT] addr=%h be=%b wdata=%h",
                        addr[winner], be[winner], wdata[winner]);
`endif
            end else if (winner_valid && we[winner] && is_agnus_reg) begin
                // DMACON write: bit 15 = SET (1) / CLR (0), bits 14..0 = mask.
                if (addr[winner] == DMACON_ADDR) begin
                    if (wdata[winner][15])
                        dmacon <= dmacon | {1'b0, wdata[winner][14:0]};
                    else
                        dmacon <= dmacon & ~{1'b0, wdata[winner][14:0]};
`ifdef KICKSTART_BOOT_TRACE
                    $display("[DMACON] addr=%h be=%b wdata=%h cur=%h",
                        addr[winner], be[winner], wdata[winner], dmacon);
`endif
                end
                // DMACONR/VPOSR/VHPOSR are read-only; writes silently drop.
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
            end else if (winner_valid && we[winner] && is_disk_read) begin
                // Disk image is read-only; silently drop the write.
            end else if (winner_valid && we[winner] && is_slow) begin
                // Slow RAM write -- bus arbiter path.  Mirror chip-RAM's
                // unaligned-.L handling: addr[1:0]==10 with be=1111 spans
                // two longword slots (the 68000 issues two consecutive
                // word writes for a misaligned .L).  Without this the
                // .L write packs all 4 bytes into a single slot, which
                // corrupts both halves of the actual destination range.
                if (be[winner] == 4'b1111 && addr[winner][1:0] == 2'b10) begin
                    slowmem[slow_idx][15:8]      <= wdata[winner][31:24];
                    slowmem[slow_idx][7:0]       <= wdata[winner][23:16];
                    slowmem[slow_idx + 1][31:24] <= wdata[winner][15:8];
                    slowmem[slow_idx + 1][23:16] <= wdata[winner][7:0];
                end else begin
                    if (be[winner][3]) slowmem[slow_idx][31:24] <= wdata[winner][31:24];
                    if (be[winner][2]) slowmem[slow_idx][23:16] <= wdata[winner][23:16];
                    if (be[winner][1]) slowmem[slow_idx][15:8]  <= wdata[winner][15:8];
                    if (be[winner][0]) slowmem[slow_idx][7:0]   <= wdata[winner][7:0];
                end
`ifdef KICKSTART_BOOT_TRACE
                // Diagnostic: log slow-RAM writes near $C001EC where the
                // WB1.3 boot is hitting BAD-PC via JSR -$8A(A6).  Helps
                // verify K1.3 actually writes the expected JMP trampoline.
                if (addr[winner] >= 32'h00C0_01E0 && addr[winner] <= 32'h00C0_01F0)
                    $display("[SLOW-WR] addr=%h be=%b wdata=%h port=%0d",
                        addr[winner], be[winner], wdata[winner], winner);
`endif
`ifdef ACTIVE_SCREEN_SLOT_TRACE
                // Trace any slow-RAM write landing at $C0605C..$C06063 — the
                // IntuitionBase (ActiveWindow, ActiveScreen) slot pair per
                // WB13_DEBUG_JOURNAL §30.  Tells us:
                //   (a) does anything ever write to the ActiveScreen slot
                //       at $C06060? (we end up with NULL there)
                //   (b) if so, what value + what writer?
                if (addr[winner] >= 32'h00C0_605C && addr[winner] <= 32'h00C0_6063) begin
                    $display("[ACT_SLOT_WR] port=%0d addr=%h be=%b wdata=%h is_long=%b",
                        winner, addr[winner], be[winner], wdata[winner], is_long[winner]);
                end
`endif
`ifdef KICKSTART_BOOT
            end else if (winner_valid && we[winner] &&
                        (addr[winner] >= (MEM_WORDS << 2))) begin
                // Under Kickstart the address space includes slow/expansion
                // regions ($00C0_xxxx, $00D0_xxxx) that Kickstart's memory
                // probe touches; without this guard those writes alias back
                // into mem[] via the truncated mem_idx and silently corrupt
                // the exception vector table (vector 3 at $0C ended up
                // holding $038F_0305, so every address-error trap landed in
                // garbage).  Bench tests deliberately use high addresses as
                // "fake RAM" via the alias, so the guard is gated on
                // KICKSTART_BOOT only.  Slow RAM at $C00000 falls through
                // to the is_slow clause above and never reaches this drop.
`endif
            end else if (winner_valid && we[winner]) begin
`ifdef KICKSTART_BOOT_TRACE
                // The fake DOS bootblock writes $CAFEBABE to $00050000 to
                // prove Kickstart's strap.lib actually JSR'd into our boot
                // code.  Fire a one-shot $display when it lands.
                if (addr[winner] == 32'h0005_0000 &&
                    wdata[winner] == 32'hCAFE_BABE && be[winner] == 4'b1111)
                    $display("[BOOTBLOCK] Kickstart called our boot code: wrote %h to %h",
                        wdata[winner], addr[winner]);
`endif
                // Unaligned .L write (addr[1:0]==10, all four bytes enabled)
                // straddles two mem[] entries on real 68000 silicon -- two
                // consecutive word writes, low half to bytes 2..3 of the
                // longword starting at (addr & ~3), high half to bytes 0..1
                // of the next longword.  Without this case the entire 32-bit
                // wdata clobbered a single mem[] entry, silently corrupting
                // Kickstart's library jump-table writes at MOVE.L A3, -(A0)
                // for every other entry.
                if (be[winner] == 4'b1111 && addr[winner][1:0] == 2'b10) begin
                    mem[mem_idx][15:8]      <= wdata[winner][31:24];
                    mem[mem_idx][7:0]       <= wdata[winner][23:16];
                    mem[mem_idx + 1][31:24] <= wdata[winner][15:8];
                    mem[mem_idx + 1][23:16] <= wdata[winner][7:0];
                end else begin
                    if (be[winner][3]) mem[mem_idx][31:24] <= wdata[winner][31:24];
                    if (be[winner][2]) mem[mem_idx][23:16] <= wdata[winner][23:16];
                    if (be[winner][1]) mem[mem_idx][15:8]  <= wdata[winner][15:8];
                    if (be[winner][0]) mem[mem_idx][7:0]   <= wdata[winner][7:0];
                end
`ifdef KICKSTART_BOOT_TRACE
                // Trace any write that lands at chip RAM idx=$1B
                // (vector table $6C-$6F).  IMPORTANT: an unaligned-.L
                // write at addr=$6A has mem_idx=$1A but ALSO writes
                // mem[$1B] via the second-slot path on line 1850-1851.
                // Catch BOTH paths.
                if (mem_idx == 'h1B || mem_idx == 'h0B ||
                    (mem_idx == 'h1A && be[winner] == 4'b1111 &&
                     addr[winner][1:0] == 2'b10) ||
                    (mem_idx == 'h0A && be[winner] == 4'b1111 &&
                     addr[winner][1:0] == 2'b10) ||
                    addr[winner] == 32'h0000_006C ||
                    addr[winner] == 32'h0000_002C ||
                    addr[winner] == 32'h0000_006E ||
                    addr[winner] == 32'h0000_002E) begin
                    $display("[VEC-BUS-WR] port=%d addr=%h mem_idx=%h be=%b wdata=%h is_long=%b",
                        winner, addr[winner], mem_idx, be[winner],
                        wdata[winner], is_long[winner]);
                end
`endif
`ifdef CLI_TITLE_BUS_WR_TRACE
                // Trace any bus arbiter write that lands in the CLI window's
                // title bar BPL1 region (mem_idx $1832..$18FA = bytes $60C8..
                // $63E8).  Fires for any winner (CPU port 0/1, blitter,
                // DMA), so we'll definitively see who writes the $2AAA
                // pattern.  Same probe pattern as [VEC-BUS-WR] above but
                // for a different memory range.
                if (mem_idx >= 'h1832 && mem_idx <= 'h18FA) begin
                    $display("[CLI_TITLE_BUS_WR] port=%d addr=%h mem_idx=%h be=%b wdata=%h is_long=%b",
                        winner, addr[winner], mem_idx, be[winner],
                        wdata[winner], is_long[winner]);
                end
`endif
                // [C2CC-BUS-WR] watchpoint moved to a hw_watch instance
                // outside this always block — see u_w_c2cc below.  Same
                // semantics (catches ALL bus writers — CPU, blitter, DMA
                // — to chip RAM $C2AC..$C2D0).
`ifdef KICKSTART_BOOT_TRACE
                // [5E40-BUS-WR]: catch any bus-side write touching the
                // linked-list slot at $5E40 (mem[$1797]/mem[$1798]).
                // Includes unaligned .L straddles via the second-slot
                // path on lines 1850-1851.
                if (mem_idx == 17'h1798 || mem_idx == 17'h1797 ||
                    (mem_idx == 17'h1796 && be[winner] == 4'b1111 &&
                     addr[winner][1:0] == 2'b10) ||
                    addr[winner] == 32'h0000_5E40 ||
                    addr[winner] == 32'h0000_5E42)
                    $display("[5E40-BUS-WR] r=%d port=%d addr=%h mem_idx=%h be=%b wdata=%h",
                        cosim_retired_i, winner, addr[winner], mem_idx, be[winner], wdata[winner]);
`endif
`ifdef HDRCHK_WATCH
                // Watch writes that touch \$64DC..\$64E7 (hdr_chk + data_chk
                // pair in the K1.3 disk-decode buffer).  Helps pin down what
                // clobbers the buffer between trackdisk validation passes.
                if (addr[winner] >= 32'h0000_64A8 && addr[winner] <= 32'h0000_64E7) begin
                    $display("[HDRCHK_WR] cyc=%0d port=%0d addr=%h be=%b wdata=%h blk_busy=%b",
                        cyc_count, winner, addr[winner], be[winner], wdata[winner], blk_busy);
                end
                if (addr[winner] >= 32'h0000_68E0 && addr[winner] <= 32'h0000_68FB) begin
                    $display("[SEC1_WR] cyc=%0d port=%0d addr=%h be=%b wdata=%h blk_busy=%b",
                        cyc_count, winner, addr[winner], be[winner], wdata[winner], blk_busy);
                end
                // Watch writes to local-var slots $5DC8/$5DCC where the
                // hdr_chk pair is copied to before $FEAA0E.
                if (addr[winner] >= 32'h0000_5DC0 && addr[winner] <= 32'h0000_5DCF) begin
                    $display("[HDRCHK_LOC] cyc=%0d port=%0d addr=%h be=%b wdata=%h",
                        cyc_count, winner, addr[winner], be[winner], wdata[winner]);
                end
`endif
            end

            // Latch for delayed response.
            granted_valid_q <= winner_valid;
            granted_port_q  <= winner;
            granted_we_q    <= winner_valid && we[winner];
            granted_is_long_q <= is_long[winner];
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
            granted_is_slow_q  <= winner_valid && !we[winner] && is_slow;
            granted_slow_idx_q <= slow_idx;
            if (winner_valid && !we[winner] && is_rom_access) begin
                // Unaligned .L read at addr[1:0]=10 spans two rom[] entries;
                // assemble the same way the RAM path does (low half of
                // rom[idx], high half of rom[idx+1]).  Without this an
                // unaligned .L read of $FFFFFFFF terminator at e.g.
                // $F8037E returned only the aligned half ($0000FFFF) and
                // Kickstart's resident-list walker never saw -1, scanning
                // until the cycle budget ran out.
                if (is_long[winner] && addr[winner][1:0] == 2'b10)
                    granted_rom_data_q <=
                        {rom[rom_idx][15:0], rom[rom_idx + 1'b1][31:16]};
                else
                    granted_rom_data_q <= rom[rom_idx];
            end

            // OVL clears the first time CIA-A drives the /OVL line low.
            if (ovl_clr_i && ovl_active) begin
                ovl_active <= 1'b0;
                ovl_just_cleared <= 1'b1;
`ifdef KICKSTART_BOOT_TRACE
                $display("[OVL] cleared");
`endif
            end else begin
                ovl_just_cleared <= 1'b0;
            end

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
    // Latch which Agnus register the granted access was for, so we can
    // route the read response one cycle later from the live counters.
    reg granted_is_agnus_q;
    reg [1:0] granted_agnus_sel_q;   // 0 = DMACONR, 1 = VPOSR, 2 = VHPOSR
    reg granted_is_disk_q;
    reg [31:0] granted_disk_data_q;
    reg granted_is_blk_q;
    reg [1:0] granted_blk_sel_q;     // 0 SRC 1 DST 2 CNT 3 CMD
    reg granted_is_paula_ro_q;
    reg [15:0] granted_paula_ro_val_q;
    reg granted_is_autoconfig_q;
    reg granted_is_shadow_q;
    reg [15:0] granted_shadow_hi_q;   // high half (long-aligned word)
    reg [15:0] granted_shadow_lo_q;   // low half (next word)
    reg granted_is_bpl_probe_q;
    reg [3:0] granted_bpl_probe_sel_q;   // 0..7 = fetch_cnt, BPL1..6 DAT, BPLCON0
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            granted_is_agnus_q  <= 1'b0;
            granted_agnus_sel_q <= 2'd0;
            granted_is_disk_q   <= 1'b0;
            granted_disk_data_q <= 32'd0;
            granted_is_blk_q    <= 1'b0;
            granted_blk_sel_q   <= 2'd0;
            granted_is_paula_ro_q  <= 1'b0;
            granted_paula_ro_val_q <= 16'd0;
            granted_is_autoconfig_q <= 1'b0;
            granted_is_shadow_q     <= 1'b0;
            granted_shadow_hi_q     <= 16'd0;
            granted_shadow_lo_q     <= 16'd0;
            granted_is_bpl_probe_q  <= 1'b0;
            granted_bpl_probe_sel_q <= 4'd0;
        end else begin
            granted_is_agnus_q  <= winner_valid && !we[winner] && is_agnus_reg
                                   && (addr[winner] != DMACON_ADDR);
            granted_agnus_sel_q <= (addr[winner] == VHPOSR_ADDR) ? 2'd2
                                 : (addr[winner] == VPOSR_ADDR)  ? 2'd1
                                 : 2'd0;
            granted_is_disk_q   <= winner_valid && !we[winner] && is_disk_read;
            granted_disk_data_q <= disk[disk_idx];
            granted_is_blk_q    <= winner_valid && !we[winner] && is_blk_reg;
            granted_blk_sel_q   <= (addr[winner] == BLKDST_AD) ? 2'd1
                                 : (addr[winner] == BLKCNT_AD) ? 2'd2
                                 : (addr[winner] == BLKCMD_AD) ? 2'd3
                                 : 2'd0;
            granted_is_autoconfig_q <= winner_valid && !we[winner] && is_autoconfig_reg;
            granted_is_shadow_q     <= winner_valid && !we[winner] && is_shadow_reg;
            // For a 32-bit read at long-aligned address X, return
            // {chip_regs[X], chip_regs[X+2]}.  For a 16-bit read at
            // an odd-word offset (chip_idx[1]=1), the value is in the
            // low half of the 32-bit result.  Compute both halves and
            // pick later when forming resp_data.
            granted_shadow_hi_q     <= chip_regs[{chip_idx[8:2], 2'b00}];
            granted_shadow_lo_q     <= chip_regs[{chip_idx[8:2], 2'b10}];
            granted_is_bpl_probe_q  <= winner_valid && !we[winner] && is_bpl_probe_reg;
            granted_bpl_probe_sel_q <= (addr[winner] == BPL1DAT_PROBE_ADDR) ? 4'd1
                                     : (addr[winner] == BPL2DAT_PROBE_ADDR) ? 4'd2
                                     : (addr[winner] == BPL3DAT_PROBE_ADDR) ? 4'd3
                                     : (addr[winner] == BPL4DAT_PROBE_ADDR) ? 4'd4
                                     : (addr[winner] == BPL5DAT_PROBE_ADDR) ? 4'd5
                                     : (addr[winner] == BPL6DAT_PROBE_ADDR) ? 4'd6
                                     : (addr[winner] == BPLCON0_PROBE_ADDR) ? 4'd7
                                     : 4'd0;
            granted_is_paula_ro_q  <= winner_valid && !we[winner] && is_paula_ro_reg;
            granted_paula_ro_val_q <=
                (addr[winner] == SERDATR_ADDR) ? SERDATR_VAL
              : (addr[winner] == POTGOR_ADDR)  ? potgor_live
              : (addr[winner] == ADKCONR_ADDR) ? ADKCONR_VAL
              : (addr[winner] == POT0DAT_ADDR) ? POT_DAT_VAL
              : (addr[winner] == POT1DAT_ADDR) ? POT_DAT_VAL
              : (addr[winner] == JOY0DAT_ADDR) ? joy0dat_live
              : (addr[winner] == JOY1DAT_ADDR) ? JOY_DAT_VAL
              // DSKBYTR: bit 14 = DMAON tracks blk_busy.  Bit 15
              // (DSKBYT = byte ready) we leave 0; K1.3 doesn't poll
              // it under DMA mode.  Last data byte 0.
              : (addr[winner] == DSKBYTR_ADDR) ? {1'b0, blk_busy, 14'd0}
              // DSKDATR: returns last disk word.  We don't track it
              // (data flows via DMA), so return 0.
              : (addr[winner] == DSKDATR_ADDR) ? 16'd0
              // DSKSYNC: returns the last value the CPU wrote
              // (typically $4489).
              : (addr[winner] == DSKSYNC_ADDR) ? dsksync
              :                                  JOY_DAT_VAL;
        end
    end
    wire [31:0] blk_resp_w = (granted_blk_sel_q == 2'd0) ? blk_src
                           : (granted_blk_sel_q == 2'd1) ? blk_dst
                           : (granted_blk_sel_q == 2'd2) ? blk_cnt
                           : {31'd0, blk_busy};
    // 16-bit chipset reads land in either the high or low half of the
    // 32-bit bus word depending on the access address — long-aligned
    // accesses with addr[1]=0 expect the value in [31:16]; odd-word
    // accesses (addr[1]=1) expect it in [15:0].  The CPU's MOVE.W
    // extraction takes the appropriate half based on dc_addr[1], so
    // place the 16-bit value accordingly.
    wire [15:0] agnus_val_w  = (granted_agnus_sel_q == 2'd2)
                                   ? {agnus_v[7:0], agnus_h[7:0]}  // VHPOSR $DFF006 (addr[1]=1)
                             : (granted_agnus_sel_q == 2'd1)
                                   // VPOSR $DFF004: bit 0 of the word = V8
                                   // (the high bit of the 9-bit vertical
                                   // position).  Kickstart's beam-wait code
                                   // does `MOVE.L $DFF004, D0; ASR.L #8, D0;
                                   // AND.L #$7FF, D0` and expects bit 8 of
                                   // D0 after the shift = bit 0 of VPOSR_lo
                                   // byte = V8.  We previously put V8 in
                                   // VPOSR_hi byte bit 0 which left the
                                   // 9-bit V always reading 0..255 (low
                                   // byte only); the loop comparing against
                                   // 256 never exited.
                                   ? {15'd0, agnus_v[8]}           // VPOSR  $DFF004 (addr[1]=0)
                                   // DMACONR $DFF002 (addr[1]=1).  Real Amiga
                                   // semantics: bit 14 = BBUSY (live blitter
                                   // busy status), bit 13 = BZERO, bits 9..0
                                   // copy DMACON.  CPU's WAITBLIT polls
                                   // BBUSY, so we must overlay it on bit 14
                                   // rather than returning the written
                                   // BLTPRI value.  Bit 13 = BZERO (set when
                                   // last blit wrote no non-zero D-words).
                                   : {dmacon[15], blt_busy_i, blt_bzero_i, dmacon[12:0]};
    // 32-bit accesses at $DFF004 read VPOSR (high word) + VHPOSR (low word)
    // as a single longword.  Kickstart's vbeam wait loop at $F88F7C does
    // `MOVE.L $DFF004, D0 ; ASR.L #8, D0 ; AND.L #$7FF, D0` and needs the
    // V[7:0] byte (which lives in VHPOSR's high byte) in the LOW half.
    // Without this, the low half returns 0 and the loop never sees V in
    // the [20..160] window.
    wire [15:0] vhposr_pair_w = {agnus_v[7:0], agnus_h[7:0]};
    wire [31:0] agnus_resp_w = granted_addr_q[1] ? {16'd0, agnus_val_w}
                             : (granted_agnus_sel_q == 2'd1)
                                   ? {agnus_val_w, vhposr_pair_w} // VPOSR.L
                                   : {agnus_val_w, 16'd0};

    // Autoconfig read response.  Before shutup, byte $00 returns the
    // type byte in the high lane; everything else returns 0.  After
    // shutup (= after a $48/$4A/$4C write) every read returns $FFFFFFFF
    // ("no card here") so Kickstart's scan moves on.
    reg [31:0] autoconfig_resp_q;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) autoconfig_resp_q <= 32'hFFFFFFFF;
        else if (winner_valid && !we[winner] && is_autoconfig_reg) begin
            if (autoconfig_shutup)
                autoconfig_resp_q <= 32'hFFFFFFFF;
            else case (addr[winner][7:0])
                8'h00: autoconfig_resp_q <= {AUTOCONFIG_TYPE, 24'd0};
                default: autoconfig_resp_q <= 32'd0;
            endcase
        end
    end

    always @* begin
        resp_valid = {N_PORTS{1'b0}};
        resp_data  = granted_is_irq_q       ? {29'd0, irq_level}
                   : granted_is_autoconfig_q ? autoconfig_resp_q
                   : granted_is_bpl_probe_q  ?
                       ( (granted_bpl_probe_sel_q == 4'd1) ? {bpl_dat[0], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd2) ? {bpl_dat[1], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd3) ? {bpl_dat[2], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd4) ? {bpl_dat[3], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd5) ? {bpl_dat[4], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd6) ? {bpl_dat[5], 16'd0}
                       : (granted_bpl_probe_sel_q == 4'd7) ? {bplcon0_shadow, 16'd0}
                       :                                     bpl_fetches )
                   : granted_is_shadow_q    ? {granted_shadow_hi_q, granted_shadow_lo_q}
                   : granted_is_agnus_q     ? agnus_resp_w
                   : granted_is_paula_ro_q  ?
                       (granted_addr_q[1] ? {16'd0, granted_paula_ro_val_q}
                                          : {granted_paula_ro_val_q, 16'd0})
                   : granted_is_disk_q      ? granted_disk_data_q
                   : granted_is_blk_q       ? blk_resp_w
                   : granted_is_blt_q       ? granted_blt_rdata_q
                   : granted_is_cop_q   ? granted_cop_rdata_q
                   : granted_is_den_q   ? granted_den_rdata_q
                   : granted_is_pau_q   ? granted_pau_rdata_q
                   : granted_is_ciaa_q  ? ciaa_resp_w
                   : granted_is_ciab_q  ? ciab_resp_w
                   : granted_is_rom_q   ? granted_rom_data_q
                   // Slow RAM: assemble unaligned .L from two slots
                   // exactly like the chip-RAM path below, otherwise
                   // K1.3's MOVE.L immediate at odd-aligned trampoline
                   // addresses returns only the head longword.
                   : granted_is_slow_q  ?
                       ((granted_is_long_q && granted_addr_q[1:0] == 2'b10)
                            ? {slowmem[granted_slow_idx_q][15:0],
                               slowmem[granted_slow_idx_q + 1][31:16]}
                            : slowmem[granted_slow_idx_q])
`ifdef KICKSTART_BOOT
                   // Under Kickstart, drop reads of unmapped addresses (above
                   // the populated mem[] window) to $0 so Kickstart's chip-
                   // RAM probe sees consistent "no memory here" (matching the
                   // write-side drop) rather than aliased mem[] data.  Real
                   // hardware would float here.  Slow RAM at $C00000 is
                   // captured by granted_is_slow_q above.
                   : (granted_addr_q >= (MEM_WORDS << 2)) ? 32'h0000_0000
`endif
                   // Unaligned .L read: caller signals genuine .L via
                   // granted_is_long_q.  Assemble from low half of mem[idx]
                   // and high half of mem[idx+1] -- matches the 68000's
                   // internal two-word read for unaligned .L.
                   : (granted_is_long_q && granted_addr_q[1:0] == 2'b10)
                       ? {mem[granted_idx_q][15:0], mem[granted_idx_q + 1][31:16]}
                       : mem[granted_idx_q];
        if (granted_valid_q) resp_valid[granted_port_q] = 1'b1;
    end
`ifdef KICKSTART_BOOT_TRACE
    // Trace bus responses for vector $6C reads to diagnose the watchdog
    // race that returns $0 instead of the correct $FC0D14.
    always @(posedge clk) if (rst_n && granted_valid_q &&
                              (granted_addr_q == 32'h0000_006C ||
                               granted_addr_q == 32'h0000_002C)) begin
        $display("[BUS-VEC] port=%d addr=%h idx=%h resp_data=%h mem[idx]=%h is_slow=%b is_rom=%b",
            granted_port_q, granted_addr_q, granted_idx_q, resp_data,
            mem[granted_idx_q], granted_is_slow_q, granted_is_rom_q);
    end
    // Per-cycle change detector for mem[$1B] — fires when its value
    // differs from the previous cycle.  This catches ANY hidden write
    // path that clears it to $0 (or any other change).
    reg [31:0] mem_1b_prev;
    always @(posedge clk) begin
        if (!rst_n) mem_1b_prev <= 32'd0;
        else begin
            if (mem[17'h1B] != mem_1b_prev) begin
                $display("[MEM-1B-CHG] prev=%h new=%h winner=%d we=%b addr=%h wdata=%h be=%b",
                    mem_1b_prev, mem[17'h1B],
                    winner, we[winner], addr[winner], wdata[winner], be[winner]);
            end
            mem_1b_prev <= mem[17'h1B];
        end
    end

`ifdef CLI_TITLE_MEM_CHG_TRACE
    // Per-cycle change detector for the first longword of the CLI title bar
    // (mem[$60C8/4] = mem[$1832]).  Fires whenever the longword value
    // changes from the previous cycle and prints the winning bus master.
    // Used in tandem with BLT_CLI_TITLE_TRACE and CLI_TITLE_CPU_WR_TRACE:
    // if all three say "0 hits", but mem[$1832] still contains $2AAA_2AAA
    // at boot end, then some hidden write path is responsible.
    reg [31:0] mem_60c8_prev;
    reg [31:0] mem_60cc_prev;
    reg [31:0] mem_60d0_prev;
    always @(posedge clk) begin
        if (!rst_n) begin
            mem_60c8_prev <= 32'd0;
            mem_60cc_prev <= 32'd0;
            mem_60d0_prev <= 32'd0;
        end else begin
            if (mem[17'h1832] != mem_60c8_prev) begin
                $display("[CLI_TITLE_MEM_CHG] addr=$60C8 prev=%h new=%h winner=%d we=%b w_addr=%h w_data=%h w_be=%b",
                    mem_60c8_prev, mem[17'h1832],
                    winner, we[winner], addr[winner], wdata[winner], be[winner]);
            end
            if (mem[17'h1833] != mem_60cc_prev) begin
                $display("[CLI_TITLE_MEM_CHG] addr=$60CC prev=%h new=%h winner=%d we=%b w_addr=%h w_data=%h w_be=%b",
                    mem_60cc_prev, mem[17'h1833],
                    winner, we[winner], addr[winner], wdata[winner], be[winner]);
            end
            if (mem[17'h1834] != mem_60d0_prev) begin
                $display("[CLI_TITLE_MEM_CHG] addr=$60D0 prev=%h new=%h winner=%d we=%b w_addr=%h w_data=%h w_be=%b",
                    mem_60d0_prev, mem[17'h1834],
                    winner, we[winner], addr[winner], wdata[winner], be[winner]);
            end
            mem_60c8_prev <= mem[17'h1832];
            mem_60cc_prev <= mem[17'h1833];
            mem_60d0_prev <= mem[17'h1834];
        end
    end
`endif

    // [CosimMW]: non-CPU memory writes (blitter, copper, denise, paula)
    // and disk-DMA-bypass writes, emitted only inside the cosim window.
    // Format: `[CosimMW] <retired> <addr> <size> <value>`
    // The CPU's own writes are already captured by [CosimW] in
    // m68k_core.v; we skip port 1 (CPU 0 D-cache) here so we don't
    // double-count.
    always @(posedge clk) if (rst_n && cosim_in_window_i &&
                              winner_valid && we[winner] &&
                              winner != {{(PID_BITS-1){1'b0}}, 1'b1}) begin
        if (be[winner] == 4'b1111)
            $display("[CosimMW] %0d %06h 4 %08h",
                cosim_retired_i, addr[winner], wdata[winner]);
        else if (be[winner] == 4'b1100)
            $display("[CosimMW] %0d %06h 2 %04h",
                cosim_retired_i, addr[winner], wdata[winner][31:16]);
        else if (be[winner] == 4'b0011)
            $display("[CosimMW] %0d %06h 2 %04h",
                cosim_retired_i, addr[winner] | 32'd2, wdata[winner][15:0]);
    end

    // [CosimMW] for disk-DMA bypass (writes that don't go through the bus
    // arbiter).  Emits during blk_busy active steps for BOTH chip-RAM
    // (mem[]) and slow-RAM (slowmem[]) destinations — the trackdisk
    // load can write to either depending on bootblock placement.
    wire dma_committing_now = blk_busy &&
        !((blk_byte_mode && blk_cur_dst >= blk_dst + blk_count_in_bytes) ||
          (!blk_byte_mode && blk_cur_dst >= blk_dst + (blk_cnt << 9)));
    always @(posedge clk) if (rst_n && cosim_in_window_i && dma_committing_now) begin
        $display("[CosimMW] %0d %06h 4 %08h",
            cosim_retired_i, blk_cur_dst,
            disk[blk_cur_off[DISK_IDX_BITS+1:2]]);
    end
`endif

    // ----------------------------------------------------------------
    // hw_watch instances at the bus arbiter port.
    //
    // Pattern: each instantiation replaces what would otherwise be a
    // hand-written `if (addr >= LO && addr <= HI && we) $display(...)`
    // inside the mem-write always block.  Drives at the bus master
    // outputs so every CPU/blitter/copper/denise/paula write is seen.
    // Disk DMA bypasses the arbiter and needs its own watch on
    // blk_cur_dst (see u_w_dma_* below).
    //
    // Adding a new bus watchpoint:
    //   1. Pick a unique LABEL.
    //   2. Set ADDR_LO/HI (use LO==HI for exact match).
    //   3. Decide MATCH_WE / MATCH_RE.
    //   4. Instantiate below.  No new always block required.
    //
    // For HALT-ON-HIT, set STOP_ON_HIT=1 — cheap substitute for a CPU
    // breakpoint until a real debug controller exists.
    wire bus_committing = winner_valid;
    wire bus_we_now     = winner_valid && we[winner];
    wire [7:0] bus_src  = {{(8 - PID_BITS){1'b0}}, winner};

    // [C2CC-BUS-WR]: chip RAM $C2AC..$C2D0 (32-byte slot around the bad-
    // RTS return PC at $FCF104).  Catches all bus writers — CPU, blitter,
    // DMA — at the actual mem[] commit.  (DMA does not flow through this
    // path; if you also want DMA writes, add a separate hw_watch wired
    // to the disk DMA destination address — see u_w_dma_low_mem below.)
    hw_watch #(
        .LABEL      ("C2CC-BUS-WR"),
        .ADDR_LO    (32'h0000_C2AC),
        .ADDR_HI    (32'h0000_C2D0),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_c2cc (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BB-DEC-WR]: chip RAM $20A0..$20A3, the bootblock-magic byte location
    // in the decoded-sector buffer.  WB1.3 boot at r=116M sees the byte
    // 1 at $20A1 = $4D instead of $4F (one-bit corruption), causing the
    // bootblock magic check at $FE85AA to fail.  This probe identifies who
    // is writing to this location between the MFM decode blit and the
    // sector-copy blit.
    hw_watch #(
        .LABEL      ("BB-DEC-WR"),
        .ADDR_LO    (32'h0000_20A0),
        .ADDR_HI    (32'h0000_20A3),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_bb_dec (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [5E40-BUS-WR]: chip RAM $5E40..$5E47, the BCPL DOS table entry
    // that gets zeroed between r=2.5M and r=4.3M of WB1.3 boot.  CPU
    // writes are captured by [5E40-WR] in m68k_core.v; this bus-arbiter
    // watch ALSO captures blitter/copper/DMA writes, identifying the
    // non-CPU writer that the existing probe missed.  src_id shows the
    // arbiter port (0=CPU0 I, 1=CPU0 D, 2=blitter, 3=copper, etc.).
    hw_watch #(
        .LABEL      ("5E40-BUS-WR"),
        .ADDR_LO    (32'h0000_5E3C),
        .ADDR_HI    (32'h0000_5E47),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_5e40_bus (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (cosim_retired_i),
        .hit_o   ()
    );

    // [MH-ATTRS-WR]: every write to chip $40E — the mh_Attributes
    // field of the chip-RAM MemHeader at chip $400.  FS-UAE writes
    // $0003 (MEMF_PUBLIC|MEMF_CHIP) here at PC=$FC1A38.  Our dump
    // shows $0000 — either the write doesn't land, or something else
    // clears it.  Always-on, captures src/value.
    hw_watch #(
        .LABEL      ("MH-ATTRS-WR"),
        .ADDR_LO    (32'h0000_040E),
        .ADDR_HI    (32'h0000_040F),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_mh_attrs (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BUF-2D24-WR]: every write to chip $2D24..$2D27 — the corrupt
    // longword in the WB1.3 cyl-53 MFM-decode buffer.  Our chip dump
    // consistently shows $0 here; FS-UAE shows $44894489 (sector 3
    // sync).  Blitter unit test passes for the cc000005 blit in
    // isolation so this is a bus-arbitration / DMA-race / CPU-write
    // interaction in the real boot.  Logs PC + src + wdata.
    hw_watch #(
        .LABEL      ("BUF-2D24-WR"),
        .ADDR_LO    (32'h0000_2D24),
        .ADDR_HI    (32'h0000_2D27),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_buf_2d24 (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BLTDPT-WR]: every write to BLTDPT ($DFF054).  Captures the moment
    // the CPU programs the destination of the next blit.  Used to find
    // which graphics.library routine is computing destinations in our
    // boot that land at chip $C2xx (corrupting the task stack).
    hw_watch #(
        .LABEL      ("BLTDPT-WR"),
        .ADDR_LO    (32'h00DF_F054),
        .ADDR_HI    (32'h00DF_F057),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_bltdpt (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [DMACON-WR]: every CPU/Copper write to DMACON ($DFF096).  Each
    // write is SET/CLR semantics: bit 15 = mode, bits 0-14 = mask.
    // Use the distribution of writes to confirm BPLEN+DMAEN are
    // simultaneously enabled while the Workbench Copper list is also
    // setting BPU>0 in BPLCON0.
    hw_watch #(
        .LABEL      ("DMACON-WR"),
        .ADDR_LO    (32'h00DF_F096),
        .ADDR_HI    (32'h00DF_F097),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_dmacon (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BPLCON0-WR]: every write to BPLCON0 ($DFF100), CPU or Copper.
    // If Workbench renders, Intuition's Copper list MOVEs $A302 to
    // BPLCON0 once per frame (after BPU-up transition).  If we see
    // zero writes, the Copper isn't executing Intuition's list.
    hw_watch #(
        .LABEL      ("BPLCON0-WR"),
        .ADDR_LO    (32'h00DF_F100),
        .ADDR_HI    (32'h00DF_F101),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_bplcon0 (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [YFLD-BUS-WR]: ANY bus write (CPU or DMA) to slow $C04784..$C04789
    // (the Y-coord field that gets zeroed by an unidentified writer).
    // The Y-FIELD-WR probe in m68k_core only catches CPU writes via
    // dc_we — DMA writes (blitter/copper/disk) go through different
    // signal paths but all show up here in the bus arbiter.
    hw_watch #(
        .LABEL      ("YFLD-BUS-WR"),
        .ADDR_LO    (32'h00C0_4780),
        .ADDR_HI    (32'h00C0_4790),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_yfld_bus (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [FS-SIG-WR]: writes into the File System task's signal-state
    // fields at slow $C00A92..$C00A9F (covers tc_SigAlloc, tc_SigWait,
    // tc_SigRecvd, tc_SigExcept).  At r=177M idle the FS task has
    // tc_SigWait=$80000000 (CTRL_C only) but tc_SigRecvd=$00000100
    // (port sigbit already raised) — scheduler never wakes it because
    // sigWait excludes the port bit.  Trace what wrote that to see if
    // Wait() got the wrong mask or if memory got corrupted.
    hw_watch #(
        .LABEL      ("FS-SIG-WR"),
        .ADDR_LO    (32'h00C0_0A92),
        .ADDR_HI    (32'h00C0_0A9F),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_fs_sig (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BPL1-AREA-WR]: every write into the Workbench BPL1 region at
    // chip $60C8..$88C8 (10 KB = 2-plane HIRES bitplane).  If WB is
    // actually drawing into the screen, we should see thousands of
    // blitter writes here.  If we see zero, the drawing pipeline
    // (graphics.library blits, layers.library drawing, console
    // putchar) never reaches the bitmap.
    hw_watch #(
        .LABEL      ("BPL1-AREA-WR"),
        .ADDR_LO    (32'h0000_60C8),
        .ADDR_HI    (32'h0000_88C8),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_bpl1_area (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [BPL1PT-WR]: every write to BPL1PTH/L ($DFF0E0-3).  Used same way
    // as BPLCON0-WR to track Copper-driven bitplane setup.
    hw_watch #(
        .LABEL      ("BPL1PT-WR"),
        .ADDR_LO    (32'h00DF_F0E0),
        .ADDR_HI    (32'h00DF_F0E3),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_bpl1pt (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [COP-LC-WR]: every CPU write to COP1LC/COP1LCH/COP1LCL/COP2LC.
    // Covers $DFF080..$DFF087.  FS-UAE WP tracing shows K1.3 activates
    // Workbench by chaining boot list -> COPJMP2 -> COP2LC at PC
    // $FC6D6C every VBL.  If our system never writes COP2LC with the
    // Intuition list address, Agnus dead-ends at the COPJMP2 and the
    // Workbench display never goes live.
    hw_watch #(
        .LABEL      ("COP-LC-WR"),
        .ADDR_LO    (32'h00DF_F080),
        .ADDR_HI    (32'h00DF_F087),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_cop_lc (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [INTVECS-WR]: chip RAM $C0..$FF — K1.3's IntVects[] array (16
    // pointers, one per IRQ source).  FS-UAE save state shows this
    // populated with 16 consecutive slow-RAM pointers $00C096DC,
    // $00C096DE, ... pointing into a BSR.B dispatcher trampoline at
    // $C096DC.  Ours is all zeros — K1.3 never sets up the IntVects.
    // Capture any writer to find which K1.3 init path is being skipped.
    hw_watch #(
        .LABEL      ("INTVECS-WR"),
        .ADDR_LO    (32'h0000_00C0),
        .ADDR_HI    (32'h0000_00FF),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_intvecs (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [LIB-LIST-WR]: chip RAM $0410..$043F — covers the exec library
    // MemList Node header at $0410-$041F and the start of the library
    // dispatch table at $0420-$043F.  The fsuae_diff.py tool revealed
    // (journal §17/18) that at PC=$FC0CE2 (autovec L2 first hit), FS-UAE
    // has chip $042C = $01220478 while our system has $042C = $01220C80.
    // Same overall structure but ONE pointer in the table differs.  The
    // FS-UAE write that produces $0478 doesn't happen on our side.
    // Capture every write to this region to compare against the
    // FS-UAE trace.
    hw_watch #(
        .LABEL      ("LIB-LIST-WR"),
        .ADDR_LO    (32'h0000_0410),
        .ADDR_HI    (32'h0000_043F),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_lib_list (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

    // [RELOC-IMG-WR]: chip RAM $10EB0..$10F1F — the relocatable image
    // that K1.3 CopyMems to slow $C094D4+ to build the IRQ-dispatcher
    // install routine.  On FS-UAE this region gets populated during
    // exec init; on us it stays zero (project_wb13_intvecs_skipped.md).
    // Any write here means the upstream producer ran — log it.
    hw_watch #(
        .LABEL      ("RELOC-IMG-WR"),
        .ADDR_LO    (32'h0001_0EB0),
        .ADDR_HI    (32'h0001_0F1F),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_reloc_img (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (bus_we_now),
        .we      (1'b1),
        .addr    (addr[winner]),
        .wdata   (wdata[winner]),
        .be      (be[winner]),
        .src_id  (bus_src),
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );

`ifdef KICKSTART_BOOT_TRACE
    // [BB-BLT-SRC]: snapshot of chip RAM at $20A0 + $22A0 (the sector
    // copy blit's A and B source starts) at every BLTSIZE write.  Lets
    // us see the actual blit input — independent of any subsequent DMA
    // refill that BB-VAL's src@20A0 captures later.
    wire bb_blt_start_wr = winner_valid && we[winner] &&
                           (addr[winner] == 32'h00DF_F058);
    always @(posedge clk) begin
        if (rst_n && bb_blt_start_wr) begin
            $display("[BB-BLT-SRC] bltsize=%h be=%b mem[20A0]=%h mem[22A0]=%h",
                wdata[winner],
                be[winner],
                mem[14'd2088],     // $20A0 / 4 = $828
                mem[14'd2216]);    // $22A0 / 4 = $8A8
        end
    end
`endif

    // [DMA-LOW-MEM]: disk DMA destination < $100 — the disk DMA path
    // (mem[blk_cur_dst[..]] <= disk[..]) bypasses the arbiter.  Existing
    // [DMA-LOW-WR] trace at line ~1500 already covers this gated by
    // KICKSTART_BOOT_TRACE; this hw_watch is the same idea but as a
    // standalone instance.  Wired to blk_cur_dst with blk_busy as valid.
`ifdef KICKSTART_BOOT_TRACE
    wire dma_committing = blk_busy &&
        !((blk_byte_mode && blk_cur_dst >= blk_dst + blk_count_in_bytes) ||
          (!blk_byte_mode && blk_cur_dst >= blk_dst + (blk_cnt << 9))) &&
        !is_slow_addr(blk_cur_dst);
    hw_watch #(
        .LABEL      ("DMA-LOW-MEM"),
        .ADDR_LO    (32'h0000_0000),
        .ADDR_HI    (32'h0000_00FF),
        .MATCH_WE   (1),
        .MATCH_RE   (0)
    ) u_w_dma_low_mem (
        .clk     (clk),
        .rst_n   (rst_n),
        .valid   (dma_committing),
        .we      (1'b1),
        .addr    (blk_cur_dst),
        .wdata   (disk[blk_cur_off[DISK_IDX_BITS+1:2]]),
        .be      (4'b1111),
        .src_id  (8'hFF),                   // DMA pseudo-port
        .pc      (32'd0),
        .retired (32'd0),
        .hit_o   ()
    );
`endif

    // DMA writes bypass the bus arbiter (mem[] is written directly at the
    // top of the always block), so they don't naturally show up on the
    // snoop bus.  That means D-caches won't invalidate stale entries
    // covering the DMA destination range -- the CPU later reads pre-DMA
    // cached values and validation fails.  Register the DMA write
    // address one cycle delayed (matches granted_*_q timing) and OR
    // into the snoop output.
    reg        dma_snoop_we_q;
    reg [31:0] dma_snoop_addr_q;
    wire dma_writing_now = blk_busy &&
        !((blk_byte_mode && blk_cur_dst >= blk_dst + blk_count_in_bytes) ||
          (!blk_byte_mode && blk_cur_dst >= blk_dst + (blk_cnt << 9)));
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            dma_snoop_we_q   <= 1'b0;
            dma_snoop_addr_q <= 32'd0;
        end else begin
            dma_snoop_we_q   <= dma_writing_now;
            dma_snoop_addr_q <= blk_cur_dst;
        end
    end

    // Snoop broadcast: every successful write goes out 1 cycle later.
    // (Suppress for blitter-register writes since those don't touch memory.)
    // DMA writes take priority -- both can fire in the same cycle, but in
    // practice CPU writes during DMA are rare since the CPU is usually
    // polling DSKBYTR waiting for completion.
    wire arb_snoop_valid = granted_valid_q && granted_we_q
                           && !granted_blt_we_q && !granted_cop_we_q
                           && !granted_den_we_q && !granted_pau_we_q
                           && !granted_ciaa_we_q && !granted_ciab_we_q;
    always @* begin
        if (dma_snoop_we_q) begin
            snoop_valid  = 1'b1;
            snoop_addr   = dma_snoop_addr_q;
            // Use BLT_PORT-like src_id (not equal to any cache CORE_ID
            // 0..2*N_CORES-1) so every D-cache invalidates.
            snoop_src_id = {$clog2(N_PORTS){1'b1}};
        end else begin
            snoop_valid  = arb_snoop_valid;
            snoop_addr   = granted_addr_q;
            snoop_src_id = granted_port_q;
        end
    end
`ifdef HDRCHK_WATCH
    always @(posedge clk) begin
        if (snoop_valid && snoop_addr >= 32'h0000_68E0 && snoop_addr <= 32'h0000_68FB)
            $display("[SNOOP_SEC1] cyc=%0d snoop_addr=%h src_id=%0d",
                cyc_count, snoop_addr, snoop_src_id);
    end
`endif
endmodule
