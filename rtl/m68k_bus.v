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
    input  wire                           ovl_clr_i,
    // Keyboard byte injection (testbench-driven).
    output reg                            kbd_inject_wr,
    output reg  [7:0]                     kbd_inject_byte,

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
    // One-cycle pulse on OVL 1 -> 0 transition.  Wired to every cache's
    // inval_all input so cached pre-OVL ROM-overlay reads at $00000-$7FFFF
    // are invalidated when low memory switches to RAM.
    output wire                           ovl_clear_pulse_o
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

    // Framebuffer peek (word index = fb_peek_addr[AIDX_BITS+1:2]).
    assign fb_peek_data = mem[fb_peek_addr[AIDX_BITS+1:2]];
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
    reg [31:0] blk_cur_dst; // current dest byte addr
    reg        blk_busy_last;
    // One-cycle pulse on the busy 1->0 transition (DMA completion).
    assign dskblk_pulse_o = blk_busy_last && !blk_busy;

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

    // VERTB pulse: high for one cycle each frame, just before agnus_v
    // wraps to 0.  Paula edge-detects on the rising edge to set INTREQ[5].
    assign vblank_pulse_o = (agnus_h == AGNUS_H_LAST) && (agnus_v == AGNUS_V_LAST);
    // Beam tick.  Increment H every clock; on wrap, increment V; on V wrap,
    // back to 0.  In a real Amiga the H tick is one bus cycle (8 master
    // clocks); here we tick once per system clock so the counter advances
    // visibly under cycle-accurate cross-check.
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            agnus_h <= 10'd0;
            agnus_v <= 10'd0;
            dmacon  <= 16'd0;
        end else begin
            if (agnus_h == AGNUS_H_LAST) begin
                agnus_h <= 10'd0;
                if (agnus_v == AGNUS_V_LAST) agnus_v <= 10'd0;
                else                          agnus_v <= agnus_v + 10'd1;
            end else begin
                agnus_h <= agnus_h + 10'd1;
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
    // Real Kickstart pokes SERDATR (bit 13 TBE = "transmit empty";
    // bit 14 TSRE = "transmit shift register empty") and POTGOR
    // (joystick / mouse / paddle button bits, active-low) during
    // early init.  We pin SERDATR to "idle, no data" and POTGOR to
    // "all buttons up, all pots floating high" so Kickstart's loops
    // don't trip on garbage.
    localparam [31:0] SERDATR_ADDR = 32'h00DF_F018;   // serial data + status
    localparam [31:0] POTGOR_ADDR  = 32'h00DF_F016;   // pot/joy buttons + pull-ups
    localparam [31:0] ADKCONR_ADDR = 32'h00DF_F010;   // audio/disk control read
    localparam [31:0] POT0DAT_ADDR = 32'h00DF_F012;   // pot 0 sample
    localparam [31:0] POT1DAT_ADDR = 32'h00DF_F014;   // pot 1 sample
    localparam [31:0] JOY0DAT_ADDR = 32'h00DF_F00A;
    localparam [31:0] JOY1DAT_ADDR = 32'h00DF_F00C;
    localparam [15:0] SERDATR_VAL  = 16'h6000;        // TSRE | TBE
    localparam [15:0] POTGOR_VAL   = 16'hFFFF;
    localparam [15:0] POT_DAT_VAL  = 16'h0000;
    localparam [15:0] JOY_DAT_VAL  = 16'h0000;
    localparam [15:0] ADKCONR_VAL  = 16'h0000;

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
                      (addr[winner] == DSKLEN_ADDR);
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
    localparam [7:0]  AUTOCONFIG_TYPE = 8'hE0;
    reg        autoconfig_shutup;
    reg [7:0]  autoconfig_base_hi;
    reg [7:0]  autoconfig_base_lo;

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
    reg        bpl_was_active;        // last cycle's (BPLEN & DMAEN & BPU>0)
    reg [9:0]  agnus_v_prev;
    wire [2:0] bpu = bplcon0_shadow[14:12];
    wire       bpl_active_now = dmacon[8] && dmacon[9] && (bpu != 3'd0);

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
    // (which lives at $00F8_0000-$00FF_FFFF) whenever the loaded ROM is
    // larger than ~$60000 bytes — for a real 512 KB Kickstart, reads at
    // $00FE_xxxx hit a blitter/copper/etc. slave that masks the ROM data,
    // breaking the ROM checksum the moment it walks past $F80000+$60000.
    // Disable the legacy window when the ROM extends into it; canonical
    // Amiga chipset at $00DF_F000+ stays active regardless.
    localparam LEGACY_CHIPSET = (ROM_WORDS <= 98304);

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
                           (addr[winner] == JOY1DAT_ADDR);
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
            5'd2: begin                       // BLTAFWM
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h04;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            5'd3: begin                       // BLTALWM
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h08;
                blt_xlat_wdata = {16'd0, amiga_wdata_half};
            end
            5'd4: begin                       // BLTCPTH
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h14;
                blt_xlat_wdata = {amiga_wdata_half, canon_bltcpt[15:0]};
            end
            5'd5: begin                       // BLTCPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h14;
                blt_xlat_wdata = {canon_bltcpt[31:16], amiga_wdata_half};
            end
            5'd6: begin                       // BLTBPTH
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h10;
                blt_xlat_wdata = {amiga_wdata_half, canon_bltbpt[15:0]};
            end
            5'd7: begin                       // BLTBPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h10;
                blt_xlat_wdata = {canon_bltbpt[31:16], amiga_wdata_half};
            end
            5'd8: begin                       // BLTAPTH
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h0C;
                blt_xlat_wdata = {amiga_wdata_half, canon_bltapt[15:0]};
            end
            5'd9: begin                       // BLTAPTL
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h0C;
                blt_xlat_wdata = {canon_bltapt[31:16], amiga_wdata_half};
            end
            5'd10: begin                      // BLTDPTH
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h18;
                blt_xlat_wdata = {amiga_wdata_half, canon_bltdpt[15:0]};
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
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h24;
                blt_xlat_wdata = {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd17: begin                      // BLTBMOD ($DFF062)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h20;
                blt_xlat_wdata = {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
            end
            5'd18: begin                      // BLTAMOD ($DFF064)
                blt_xlat_valid = 1'b1;
                blt_xlat_addr  = 6'h1C;
                blt_xlat_wdata = {{16{amiga_wdata_half[15]}}, amiga_wdata_half};
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
    always @* begin
        cop_xlat_addr  = 6'h00;
        cop_xlat_wdata = 32'd0;
        cop_xlat_valid = 1'b0;
        case (cop_amiga_reg)
            3'd0: begin                       // COP1LCH ($DFF080)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h00;
                cop_xlat_wdata = {amiga_wdata_half, canon_cop1lc[15:0]};
            end
            3'd1: begin                       // COP1LCL ($DFF082)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h00;
                cop_xlat_wdata = {canon_cop1lc[31:16], amiga_wdata_half};
            end
            3'd2: begin                       // COP2LCH ($DFF084)
                cop_xlat_valid = 1'b1;
                cop_xlat_addr  = 6'h0C;
                cop_xlat_wdata = {amiga_wdata_half, canon_cop2lc[15:0]};
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
            blt_slv_req   = winner_valid && we[winner] && blt_xlat_valid;
            blt_slv_we    = winner_valid && we[winner] && blt_xlat_valid;
            blt_slv_addr  = blt_xlat_addr;
            blt_slv_be    = 4'b1111;
            blt_slv_wdata = blt_xlat_wdata;
        end else begin
            blt_slv_req   = winner_valid && is_blt_legacy;
            blt_slv_we    = winner_valid && is_blt_legacy && we[winner];
            blt_slv_addr  = addr[winner][5:0];
            blt_slv_be    = be[winner];
            blt_slv_wdata = wdata[winner];
        end
    end
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
            ovl_just_cleared <= 1'b0;
            irq_level <= 3'd0;
            blk_src   <= 32'd0;
            blk_dst   <= 32'd0;
            blk_cnt   <= 32'd0;
            blk_busy  <= 1'b0;
            blk_busy_last <= 1'b0;
            blk_cur_off <= 32'd0;
            blk_cur_dst <= 32'd0;
            dsk_pt    <= 32'd0;
            dsk_len   <= 16'd0;
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
                    // Fetch one half-word for the current plane.
                    if (bpl_pt[bpl_plane_idx][1] == 1'b0) begin
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
            // for its own register file; this snoop is in parallel.
            if (winner_valid && we[winner] && (addr[winner] == 32'h00DF_F100)) begin
                // 16-bit BPLCON0 lives in the high half of the long
                // at $DFF100 (addr[1]==0 -> high half).
                if (be[winner][3] | be[winner][2])
                    bplcon0_shadow <= wdata[winner][31:16];
                else
                    bplcon0_shadow <= wdata[winner][15:0];
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
                    5'd4:  canon_bltcpt[31:16] <= amiga_wdata_half;
                    5'd5:  canon_bltcpt[15:0]  <= amiga_wdata_half;
                    5'd6:  canon_bltbpt[31:16] <= amiga_wdata_half;
                    5'd7:  canon_bltbpt[15:0]  <= amiga_wdata_half;
                    5'd8:  canon_bltapt[31:16] <= amiga_wdata_half;
                    5'd9:  canon_bltapt[15:0]  <= amiga_wdata_half;
                    5'd10: canon_bltdpt[31:16] <= amiga_wdata_half;
                    5'd11: canon_bltdpt[15:0]  <= amiga_wdata_half;
                    default: ;
                endcase
            end
            if (winner_valid && we[winner] && is_cop_amiga) begin
                case (cop_amiga_reg)
                    3'd0: canon_cop1lc[31:16] <= amiga_wdata_half;
                    3'd1: canon_cop1lc[15:0]  <= amiga_wdata_half;
                    3'd2: canon_cop2lc[31:16] <= amiga_wdata_half;
                    3'd3: canon_cop2lc[15:0]  <= amiga_wdata_half;
                    default: ;
                endcase
            end

            // Block-device DMA tick: while busy, copy one disk word to mem
            // per clock.  Two cycles of pre-arbiter access is fine — we
            // bypass the bus arbiter entirely (this is a "ghost" master that
            // writes mem[] directly, simulating an off-CPU controller).
            if (blk_busy) begin
                if (blk_cur_dst >= blk_dst + (blk_cnt << 9)) begin
                    blk_busy <= 1'b0;
                end else begin
                    mem[blk_cur_dst[AIDX_BITS+1:2]] <= disk[blk_cur_off[DISK_IDX_BITS+1:2]];
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
                // Floppy DSKPT/DSKLEN writes.  Most accesses are 16-bit,
                // landing in either high or low half of the 32-bit word.
                // DSKPTH at $20 (word-aligned long): top 16 bits of DSKPT.
                // DSKPTL at $22: bottom 16 bits.  DSKLEN at $24 (high half
                // of long $24): length + DMAEN bit.
                if (addr[winner] == DSKPTH_ADDR) begin
                    // Upper half of 32-bit DSKPT
                    dsk_pt[31:16] <= wdata[winner][31:16];
                end else if (addr[winner] == DSKPTL_ADDR) begin
                    dsk_pt[15:0]  <= wdata[winner][15:0];
                end else if (addr[winner] == DSKLEN_ADDR) begin
                    // Word write at $24 — value in high half of 32-bit
                    // word (since $24 is long-aligned, addr[1]=0).
                    dsk_len <= wdata[winner][31:16];
                    // DMAEN bit 15: start transfer.  Re-use block-DMA
                    // engine: source = blk_src*512, dest = dsk_pt,
                    // count = (dsk_len & $3FFF) words = dsk_len*2 bytes.
                    if (wdata[winner][31] && !blk_busy) begin
                        blk_busy    <= 1'b1;
                        blk_cur_off <= blk_src << 9;          // sector*512
                        blk_cur_dst <= dsk_pt;
                        // Use dsk_len (in words) as the transfer length
                        // in bytes/4 longs.  Set blk_cnt such that
                        // blk_dst + (blk_cnt << 9) = dsk_pt + dsk_len*2:
                        blk_dst     <= dsk_pt;
                        blk_cnt     <= {16'd0, wdata[winner][29:16]} >> 8;
                    end
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
            end else if (winner_valid && we[winner] && is_agnus_reg) begin
                // DMACON write: bit 15 = SET (1) / CLR (0), bits 14..0 = mask.
                if (addr[winner] == DMACON_ADDR) begin
                    if (wdata[winner][15])
                        dmacon <= dmacon | {1'b0, wdata[winner][14:0]};
                    else
                        dmacon <= dmacon & ~{1'b0, wdata[winner][14:0]};
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
            end else if (winner_valid && we[winner]) begin
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
              : (addr[winner] == POTGOR_ADDR)  ? POTGOR_VAL
              : (addr[winner] == ADKCONR_ADDR) ? ADKCONR_VAL
              : (addr[winner] == POT0DAT_ADDR) ? POT_DAT_VAL
              : (addr[winner] == POT1DAT_ADDR) ? POT_DAT_VAL
              : (addr[winner] == JOY0DAT_ADDR) ? JOY_DAT_VAL
              :                                  JOY_DAT_VAL;
        end
    end
    wire [31:0] blk_resp_w = (granted_blk_sel_q == 2'd0) ? blk_src
                           : (granted_blk_sel_q == 2'd1) ? blk_dst
                           : (granted_blk_sel_q == 2'd2) ? blk_cnt
                           : {31'd0, blk_busy};
    wire [31:0] agnus_resp_w = (granted_agnus_sel_q == 2'd2)
                                  // VHPOSR: {V[7:0], H[7:0]} in low 16
                                  ? {16'd0, agnus_v[7:0], agnus_h[7:0]}
                              : (granted_agnus_sel_q == 2'd1)
                                  // VPOSR: bit 0 of high byte = V[8]
                                  ? {16'd0, 7'd0, agnus_v[8], 8'd0}
                                  // DMACONR
                                  : {16'd0, dmacon};

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
                   : granted_is_paula_ro_q  ? {16'd0, granted_paula_ro_val_q}
                   : granted_is_disk_q      ? granted_disk_data_q
                   : granted_is_blk_q       ? blk_resp_w
                   : granted_is_blt_q       ? granted_blt_rdata_q
                   : granted_is_cop_q   ? granted_cop_rdata_q
                   : granted_is_den_q   ? granted_den_rdata_q
                   : granted_is_pau_q   ? granted_pau_rdata_q
                   : granted_is_ciaa_q  ? ciaa_resp_w
                   : granted_is_ciab_q  ? ciab_resp_w
                   : granted_is_rom_q   ? granted_rom_data_q
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
