// Verilator C++ testbench harness for the 68K-mini multi-core SoC.
//
// Two modes:
//
//   Regression (default):
//     sim_main <max_cycles>
//     Runs until every core halts or max_cycles, prints per-core summary,
//     returns 0 on success.
//
//   Graphics demo (compile with -DHAVE_SDL2):
//     sim_main <max_cycles> --graphics
//     Opens a 256x192 SDL2 window (scaled 4x), continuously samples the
//     framebuffer region at FB_BASE every WALL_FRAME_MS milliseconds, and
//     renders it as an RGB332 image. Quits on ESC, window close, or when
//     every core halts. The pixel format is 8 bits per pixel:
//         bits [7:5] = R, bits [4:2] = G, bits [1:0] = B.
//
// Reads program.hex from cwd (loaded by $readmemh inside m68k_bus.v).

#include <verilated.h>
#include "Vm68k_top.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <unistd.h>   // chdir
#include <algorithm>

#ifdef HAVE_SDL2
#include <SDL.h>
#include <chrono>
#endif

#ifndef FB_BASE
#define FB_BASE 0x00010000u   // byte address of the chunky 8 bpp framebuffer
#endif
#ifndef FB_W
#define FB_W 256
#endif
#ifndef FB_H
#define FB_H 192
#endif
#ifndef FB_SCALE
#define FB_SCALE 4
#endif
#ifndef WALL_FRAME_MS
#define WALL_FRAME_MS 33    // ~30 FPS render cap
#endif

// 1bpp bitplane overlay used by the blitter demos.  256x192 bits packed
// 16 bits per word, big-endian (bit 15 of word 0 = pixel (0,0)).  Set bits
// render as white over the chunky framebuffer below.  Total size = 6144
// bytes = 1536 longs.
#ifndef BP_BASE
#define BP_BASE 0x00020000u
#endif
#define BP_BYTES (FB_W * FB_H / 8)

// Audio:  SDL_audio output rate.  Paula's audio_l/r are sampled at this
// rate by the harness's main loop.  Cycles-per-audio-sample = how many
// simulator cycles to run between audio captures.
#ifndef AUDIO_RATE
#define AUDIO_RATE 44100
#endif
#ifndef AUDIO_BUF_SAMPLES
#define AUDIO_BUF_SAMPLES 2048
#endif
#ifndef CYCLES_PER_AUDIO
#define CYCLES_PER_AUDIO 100   // approx. simulator cycles between samples
#endif

static uint32_t read_fb_byte(Vm68k_top* top, uint32_t byte_addr) {
    // Word-aligned peek; pick the byte inside the 32-bit word.  Big-endian:
    // byte address A & 3 == 0 -> [31:24], 1 -> [23:16], 2 -> [15:8], 3 -> [7:0].
    uint32_t word_addr = byte_addr & ~3u;
    top->fb_peek_addr = word_addr;
    top->eval();
    uint32_t w = top->fb_peek_data;
    uint32_t shift = (3u - (byte_addr & 3u)) * 8u;
    return (w >> shift) & 0xFFu;
}

#ifdef HAVE_SDL2
static int run_graphics(Vm68k_top* top, uint64_t max_cycles, int n_cores);
#endif

static int run_regression(Vm68k_top* top, uint64_t max_cycles, int n_cores);

int main(int argc, char** argv) {
    // The Verilog references program.hex / rom.hex / disk.hex via $readmemh,
    // which resolves relative to the current working directory.  Each build
    // directory (build, build_kick_boot, build_demo, etc.) gets its own hex
    // files alongside Vm68k_top.  Invoking the binary from any other dir
    // would silently load all-zero memory (Verilator prints a "%Warning:
    // ... file not found" but does not abort) and the simulation would
    // appear to run K1.3 but the CPU is actually executing NOP-of-zero
    // opcodes forever.  Chdir to argv[0]'s directory so the lookup
    // always finds the right hex files regardless of caller cwd.
    if (argv[0] && argv[0][0]) {
        const char* slash = std::strrchr(argv[0], '/');
        if (slash) {
            std::string bin_dir(argv[0], slash - argv[0]);
            if (chdir(bin_dir.c_str()) != 0) {
                std::fprintf(stderr,
                    "[sim] WARNING: chdir(%s) failed; hex files may not load\n",
                    bin_dir.c_str());
            }
        }
    }
    Verilated::commandArgs(argc, argv);
    uint64_t max_cycles = 200000;
    bool graphics = false;
    if (argc >= 2) max_cycles = std::strtoull(argv[1], nullptr, 0);
    for (int i = 2; i < argc; i++) {
        if (std::strcmp(argv[i], "--graphics") == 0) graphics = true;
    }

    Vm68k_top* top = new Vm68k_top;

    top->clk = 0;
    top->rst_n = 0;
    top->fb_peek_addr = 0;
    // External-input defaults: no mouse motion, no buttons, no kbd.
    // Without these, the inputs are whatever Verilator initialised them
    // to (usually 0, which is benign — but be explicit for clarity).
    top->ext_kbd_wr     = 0;
    top->ext_kbd_byte   = 0;
    top->mouse_x_count  = 0;
    top->mouse_y_count  = 0;
    top->mouse_btn_l    = 0;
    top->mouse_btn_r    = 0;
    top->mem_poke_strobe = 0;
    top->mem_poke_addr   = 0;
    top->mem_poke_data   = 0;
    for (int i = 0; i < 8; i++) {
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
    }
    top->rst_n = 1;

    (void)top->audio_l;  // accessed in graphics path
    (void)top->audio_r;

#ifndef NUM_CORES
#define NUM_CORES 2
#endif
    const int n_cores = NUM_CORES;

    int rc;
#ifdef HAVE_SDL2
    if (graphics) rc = run_graphics(top, max_cycles, n_cores);
    else          rc = run_regression(top, max_cycles, n_cores);
#else
    (void)graphics;
    rc = run_regression(top, max_cycles, n_cores);
#endif
    delete top;
    return rc;
}

// ----- Memory-mailbox console ----------------------------------------------
// C demos (see runtime/io.h) post bytes to a 256-byte ring in main memory:
//   0x0000F000   head index   (CPU writes, monotonic)
//   0x0000F010   ring base    (head & 0xFF indexes into 256 bytes)
// We poll the head index every CONSOLE_POLL cycles and drain any pending
// chars to stdout.  fb_peek bypasses the cache and sees write-through
// stores immediately, so this is race-free with respect to the CPU.
static constexpr uint32_t CONSOLE_HEAD_ADDR = 0x0000F000;
static constexpr uint32_t CONSOLE_RING_ADDR = 0x0000F010;
static constexpr uint32_t CONSOLE_RING_MASK = 0xFFu;
static constexpr uint64_t CONSOLE_POLL      = 32;

static uint32_t mem_peek_word(Vm68k_top* top, uint32_t byte_addr) {
    top->fb_peek_addr = byte_addr & ~3u;
    top->eval();
    return top->fb_peek_data;
}

#ifdef HAVE_SDL2
// ---------------------------------------------------------------------------
// K1.3 chipram → pixel-buffer renderer.
//
// Our Verilog Denise rasterises bitplanes only when BPLEN+DMAEN are on, which
// K1.3 doesn't enable until Intuition.OpenScreen runs (post-Workbench-load).
// Until we get that far, the only way to "see" K1.3's screen is to walk the
// Copper list it staged in chip RAM, extract the palette + sprite pointers +
// bitplane config, and render directly into the SDL pixel buffer.
//
// Mirrors tools/render_k13_screen.py (Python implementation; that one runs as
// a one-shot post-mortem on a chipram dump.  This one updates the SDL window
// every frame).
// ---------------------------------------------------------------------------
static uint16_t chip_word(Vm68k_top* top, uint32_t addr) {
    if (addr & 1) {
        // Unaligned: stitch two adjacent words.
        uint32_t w_lo = mem_peek_word(top, addr & ~1u);
        uint32_t w_hi = mem_peek_word(top, (addr + 1) & ~1u);
        if ((addr & 2) == 0)
            return uint16_t(((w_lo >> 8) & 0xFF) | ((w_hi & 0xFF000000u) >> 16));
        return uint16_t(((w_lo >> 16) & 0xFF) | ((w_hi & 0xFF0000u) >> 8));
    }
    uint32_t w = mem_peek_word(top, addr & ~3u);
    uint32_t shift = ((addr & 2) ? 0 : 16);
    return uint16_t((w >> shift) & 0xFFFFu);
}

static uint32_t amiga4_to_argb(uint16_t c12) {
    uint8_t r = (c12 >> 8) & 0xF;
    uint8_t g = (c12 >> 4) & 0xF;
    uint8_t b =  c12       & 0xF;
    return (0xFFu << 24) |
           uint32_t((r << 4) | r) << 16 |
           uint32_t((g << 4) | g) << 8  |
           uint32_t((b << 4) | b);
}

// Scan chip RAM for an Intuition-style WB1.3 Copper list and return its
// head address (chip byte offset).  The signature is the canonical WB
// palette intro:
//   MOVE \$005A -> COLOR00 ($0180)  =  word stream {$0180, $005A}
//   MOVE \$0FFF -> COLOR01 ($0182)  =  word stream {$0182, $0FFF}
// When that 4-word pattern is found, walk back up to 8 bytes to the
// preceding WAIT (low bit of word 1 = 1, low bit of word 2 = 0) and
// use that as the list head.  Returns 0 if no match.
//
// Mirrors tools/render_k13_screen.py::autodetect_intuition_copper().
static uint32_t k13_autodetect_cop1lc(Vm68k_top* top) {
    // Sweep chip RAM in word-aligned strides.  Chip RAM is 512 KB =
    // 262144 16-bit words.  Cost: ~1M word reads per scan ≈ negligible.
    for (uint32_t a = 0; a + 8 <= 0x80000u; a += 2) {
        if (chip_word(top, a)       == 0x0180 &&
            chip_word(top, a + 2)   == 0x005A &&
            chip_word(top, a + 4)   == 0x0182 &&
            chip_word(top, a + 6)   == 0x0FFF) {
            // Found.  Walk back up to 8 bytes for a preceding WAIT.
            for (int back = 4; back <= 8; back += 4) {
                if (back > (int)a) break;
                uint32_t cand = a - back;
                uint16_t w1 = chip_word(top, cand);
                uint16_t w2 = chip_word(top, cand + 2);
                if ((w1 & 1) && !(w2 & 1)) return cand;
            }
            return a;
        }
    }
    return 0;
}

// Walk the Copper list rooted at cop1lc (chip-RAM byte address); update
// the regs array (16-bit entries, indexed by chip-reg offset/2 in
// $000..$1FE).  Stops at end-of-list (WAIT $FFFF) or instruction-budget.
static void k13_walk_copper(Vm68k_top* top, uint32_t cop1lc,
                            uint16_t regs[256]) {
    uint32_t pc = cop1lc & 0xFFFFFu;
    for (int n = 0; n < 4096; n++) {
        uint16_t ir1 = chip_word(top, pc);
        uint16_t ir2 = chip_word(top, pc + 2);
        if (ir1 == 0 && ir2 == 0) break;
        pc += 4;
        if (ir1 & 1) {
            // WAIT / SKIP.  End-of-list sentinel = WAIT vp=$FF hp=$7F, ir2&1=0.
            uint8_t vp = (ir1 >> 8) & 0xFF;
            uint8_t hp = (ir1 >> 1) & 0x7F;
            if (vp == 0xFF && hp == 0x7F && !(ir2 & 1)) break;
            // SKIP: at end-of-frame all vp/hp comparisons pass, skip next.
            if (ir2 & 1) pc += 4;
        } else {
            // MOVE: ir1[8:1] = chip-reg offset (low 9 bits, byte address).
            uint16_t reg = ir1 & 0x1FE;
            regs[reg >> 1] = ir2;
            // COPJMP2 reloads PC from COP2LC.
            if (reg == 0x08A) {
                uint32_t cop2lc = (uint32_t(regs[0x084 >> 1]) << 16) |
                                  uint32_t(regs[0x086 >> 1]);
                pc = cop2lc & 0xFFFFFu;
            }
            // COPJMP1 reloads from COP1LC.
            if (reg == 0x088) {
                uint32_t cop1 = (uint32_t(regs[0x080 >> 1]) << 16) |
                                uint32_t(regs[0x082 >> 1]);
                pc = cop1 & 0xFFFFFu;
            }
        }
    }
}

// Render an Amiga hardware sprite chain at sprpt onto buf (w*h ARGB).
// idx = 0..7 (determines which COLOR pair: SPRn uses 17 + n*2 .. 19 + n*2).
static void k13_render_sprite(Vm68k_top* top, uint32_t sprpt,
                              const uint16_t regs[256],
                              uint32_t* buf, int w, int h, int idx) {
    if (sprpt == 0) return;
    // Sprite palette: K1.3 doesn't set COLOR17-19 in its boot Copper list
    // (Intuition would, post-OpenScreen).  Fall back to a sensible default
    // so the mouse cursor renders as the conventional white-body/black-
    // outline arrow rather than an all-black blob: 1=white, 2=black,
    // 3=light grey.  Sprites 1..7 are unused by K1.3 in boot state.
    uint16_t c1 = regs[(0x180 + (17 + idx * 2) * 2) >> 1];
    uint16_t c2 = regs[(0x180 + (18 + idx * 2) * 2) >> 1];
    uint16_t c3 = regs[(0x180 + (19 + idx * 2) * 2) >> 1];
    if (c1 == 0 && c2 == 0 && c3 == 0) {
        c1 = 0x0FFF;  // white
        c2 = 0x0000;  // black
        c3 = 0x0BBB;  // light grey
    }
    uint32_t pal[4] = {0, amiga4_to_argb(c1), amiga4_to_argb(c2), amiga4_to_argb(c3)};
    uint32_t p = sprpt;
    for (int safety = 0; safety < 64; safety++) {
        uint16_t pos = chip_word(top, p);
        uint16_t ctl = chip_word(top, p + 2);
        int vstart = ((pos >> 8) & 0xFF) | ((ctl & 0x4) << 6);
        int vstop  = ((ctl >> 8) & 0xFF) | ((ctl & 0x2) << 7);
        int hstart = ((pos & 0xFF) << 1) | (ctl & 0x1);
        if (vstart == 0 && vstop == 0) return;
        if (vstop <= vstart) return;
        int height = vstop - vstart;
        p += 4;
        for (int row = 0; row < height; row++) {
            uint16_t w0 = chip_word(top, p);
            uint16_t w1 = chip_word(top, p + 2);
            p += 4;
            int y = vstart + row;
            if (y >= h) continue;
            for (int bit = 0; bit < 16; bit++) {
                int px = ((w0 & (0x8000 >> bit)) ? 1 : 0) |
                         ((w1 & (0x8000 >> bit)) ? 2 : 0);
                if (!px) continue;
                int x = hstart + bit;
                if (x >= 0 && x < w && y >= 0 && y < h)
                    buf[y * w + x] = pal[px];
            }
        }
    }
}

// Render K1.3's current chipram state into the pixel buffer.  Pixel layout
// is ARGB8888.  buf must have w*h pixels.
static void render_k13_chipram(Vm68k_top* top, uint32_t cop1lc,
                               uint32_t* buf, int w, int h) {
    uint16_t regs[256] = {0};
    k13_walk_copper(top, cop1lc, regs);
    // Background = COLOR00.  K1.3's boot Copper list doesn't set COLOR00
    // explicitly (Intuition would, post-OpenScreen); for the boot prompt
    // state we hard-fill with the canonical Workbench light blue so the
    // user sees the cursor sprite over the expected background.
    uint16_t c00 = regs[0x180 >> 1];
    if (c00 == 0) c00 = 0x0AAF;  // light blue Workbench background
    uint32_t bg = amiga4_to_argb(c00);
    for (int i = 0; i < w * h; i++) buf[i] = bg;
    // Only render sprite 0 (the boot mouse cursor).  Sprites 1..7 in
    // K1.3's idle state all share the same dummy pointer at \$23C0, which
    // is end-of-Copper-list data — interpreting it as sprite POS/CTL
    // gives a bogus 36-row sprite at (0,0).  The Python reference tool
    // (tools/render_k13_screen.py) likewise renders SPR0 only.
    uint32_t sprpt = (uint32_t(regs[0x120 >> 1]) << 16) |
                     uint32_t(regs[0x122 >> 1]);
    sprpt &= 0xFFFFFu;
    if (sprpt) k13_render_sprite(top, sprpt, regs, buf, w, h, 0);
}
#endif  // HAVE_SDL2

static void drain_console(Vm68k_top* top, uint32_t& host_tail) {
    uint32_t head = mem_peek_word(top, CONSOLE_HEAD_ADDR);
    if (host_tail == head) return;
    // OVL-shadowed reads at $0F000 can return arbitrary ROM bytes (e.g.
    // $DEADBEEF) before the CPU has touched the console region. Cap the
    // per-call drain to one ring's worth so an implausibly large head
    // doesn't lock us in a 3.7B-byte fputc loop.
    uint32_t advance = head - host_tail;
    if (advance > (CONSOLE_RING_MASK + 1u)) {
        host_tail = head;
        return;
    }
    if (std::getenv("CONSOLE_DBG"))
        std::fprintf(stderr, "[drain] head=%u tail=%u\n", head, host_tail);
    while (host_tail != head) {
        uint32_t a = CONSOLE_RING_ADDR + (host_tail & CONSOLE_RING_MASK);
        uint32_t w = mem_peek_word(top, a);
        uint32_t shift = (3u - (a & 3u)) * 8u;
        uint8_t  ch = (w >> shift) & 0xFFu;
        if (std::getenv("CONSOLE_DBG"))
            std::fprintf(stderr, "[drain]   t=%u a=0x%x w=0x%08x sh=%u ch=0x%02x '%c'\n",
                         host_tail, a, w, shift, ch, (ch >= 0x20 && ch < 0x7F) ? ch : '?');
        std::fputc(ch, stdout);
        host_tail++;
    }
}

static int run_regression(Vm68k_top* top, uint64_t max_cycles, int n_cores) {
    auto all_halted = [&]() {
        for (int c = 0; c < n_cores; c++) {
            if (((top->halted >> c) & 1) == 0) return false;
        }
        return true;
    };

    // Optional automated keyboard injection.  KBD_AUTO_INJECT contains an
    // ASCII string; we convert each character to an Amiga rawkey and feed
    // it (key-down then key-up) into top->ext_kbd_byte with a one-cycle
    // ext_kbd_wr strobe.  KBD_AUTO_INJECT_START controls how many cycles
    // to wait before injecting the first key (defaults to half of
    // max_cycles -- past the typical K1.3 boot settle point).  Keys are
    // paced at KBD_AUTO_INJECT_PERIOD cycles apart (default 1,000,000
    // host clocks = ~14 simulated VBL frames).  Used to drive WB1.3's CLI
    // past its idle WAIT on console input from the test harness, where
    // there's no SDL window to type into.
    const char* kbd_inject_str = std::getenv("KBD_AUTO_INJECT");
    uint64_t    kbd_inject_start  = 0;
    uint64_t    kbd_inject_period = 1000000;
    if (kbd_inject_str) {
        if (const char* s = std::getenv("KBD_AUTO_INJECT_START"))
            kbd_inject_start = std::strtoull(s, nullptr, 0);
        else
            kbd_inject_start = max_cycles / 2;
        if (const char* s = std::getenv("KBD_AUTO_INJECT_PERIOD"))
            kbd_inject_period = std::strtoull(s, nullptr, 0);
        printf("[sim] KBD_AUTO_INJECT='%s' start=%llu period=%llu\n",
            kbd_inject_str, (unsigned long long)kbd_inject_start,
            (unsigned long long)kbd_inject_period);
    }
    size_t   kbd_idx       = 0;
    bool     kbd_is_keydown = true;  // alternate down/up per key
    uint64_t kbd_next_cycle = kbd_inject_start;

    // Optional one-shot memory poke.  Format:
    //   MEM_POKE="addr=value"          (single 32-bit word write)
    //   MEM_POKE="addr=value,addr=value,..." (multiple)
    //   MEM_POKE_CYCLE=N               (defaults to max_cycles/2)
    //
    // Used to patch K1.3 task state from outside the OS — e.g. set
    // input.device's tc_SigWait to include the keyboard-event bit so
    // a boot stalled in early-init Wait() can advance.  See
    // project_wb13_cli_wait.md.
    std::vector<std::pair<uint32_t, uint32_t>> pokes;
    uint64_t poke_cycle  = max_cycles / 2;
    uint64_t poke_period = 0;  // 0 = one-shot; >0 = re-fire every N cycles
    if (const char* s = std::getenv("MEM_POKE")) {
        const char* p = s;
        while (*p) {
            char* end = nullptr;
            uint32_t addr = (uint32_t)std::strtoul(p, &end, 0);
            if (!end || *end != '=') break;
            p = end + 1;
            uint32_t val = (uint32_t)std::strtoul(p, &end, 0);
            if (!end) break;
            pokes.push_back({addr, val});
            p = end;
            if (*p == ',') p++;
        }
        if (const char* s2 = std::getenv("MEM_POKE_CYCLE"))
            poke_cycle = std::strtoull(s2, nullptr, 0);
        if (const char* s2 = std::getenv("MEM_POKE_PERIOD"))
            poke_period = std::strtoull(s2, nullptr, 0);
        printf("[sim] MEM_POKE %zu word(s) at cycle %llu (period=%llu)\n",
               pokes.size(), (unsigned long long)poke_cycle,
               (unsigned long long)poke_period);
        for (auto& pp : pokes)
            printf("[sim]   $%08X <- $%08X\n", pp.first, pp.second);
    }

    uint32_t host_tail = 0;
    uint64_t cycle = 0;
    size_t   poke_idx = 0;
    uint64_t poke_next_cycle = poke_cycle;

    // CHIPRAM_SNAP_PCS - PC-triggered chip-RAM snapshot list.
    //
    // Env var format: comma-separated entries `pc[:label]`, where pc is
    // a 24-bit hex value (with or without 0x/$) and label is an optional
    // tag used in the dump filename.  When core 0's EX-stage PC enters
    // one of these values, the simulator writes the full 512 KB of chip
    // RAM to `<CHIPRAM_SNAP_DIR>/snap_<retired>_<label-or-pc>.bin` (the
    // directory defaults to the working directory).  Each PC fires
    // multiple times if execution re-enters it — useful for diffing
    // memory across iterations.
    //
    // Example: catch the bad MOVEM source at PC=$5D82, every fire:
    //   CHIPRAM_SNAP_PCS=5D82:movem-src \
    //   CHIPRAM_SNAP_DIR=/tmp/snaps     ./Vm68k_top 30000000
    //
    // The PC compare uses the 24-bit address mask (68000 ignores upper
    // 8 bits) so $00FD5482 and $FFFD5482 both match a configured "fd5482".
    struct SnapTrigger {
        uint32_t pc24;     // 24-bit masked PC value
        std::string label; // filename tag
        int fired = 0;     // # of times we've already snap-dumped
    };
    std::vector<SnapTrigger> snap_triggers;
    std::string snap_dir = ".";
    if (const char* d = std::getenv("CHIPRAM_SNAP_DIR")) snap_dir = d;
    // CHIPRAM_SNAP_LIMIT — max number of times each trigger fires.
    // Default is 1 (snapshot ONCE per PC, on the first arrival).  For
    // long-running idle PCs this avoids the snap explosion that
    // happens when "fire on PC change" sees the loop come back to
    // the trigger every iteration.  Set to 0 for unlimited (old
    // behaviour) or to N to cap per-trigger.
    int snap_limit = 1;
    if (const char* l = std::getenv("CHIPRAM_SNAP_LIMIT")) {
        snap_limit = std::atoi(l);
    }
    // CHIPRAM_SNAP_HALT=1 — stop the simulation after all triggers
    // have fired their limit.  Useful for diff workflows where the
    // snapshot is the only output we care about.
    bool snap_halt = false;
    if (const char* h = std::getenv("CHIPRAM_SNAP_HALT")) {
        snap_halt = (h[0] == '1' || h[0] == 'y' || h[0] == 'Y');
    }
    if (const char* s = std::getenv("CHIPRAM_SNAP_PCS")) {
        const char* p = s;
        while (*p) {
            const char* end = p;
            while (*end && *end != ',') end++;
            std::string ent(p, end);
            size_t colon = ent.find(':');
            std::string pc_str = ent.substr(0, colon);
            std::string label  = (colon == std::string::npos) ? std::string()
                                                              : ent.substr(colon + 1);
            // Accept 0x, $, or bare hex.
            if (!pc_str.empty() && pc_str[0] == '$') pc_str = pc_str.substr(1);
            uint32_t pc = (uint32_t)std::strtoul(pc_str.c_str(), nullptr, 16);
            if (label.empty()) {
                char buf[16]; std::snprintf(buf, sizeof(buf), "pc%06X", pc & 0xFFFFFF);
                label = buf;
            }
            snap_triggers.push_back({pc & 0xFFFFFFu, label});
            printf("[sim] CHIPRAM_SNAP_PCS pc=$%06X label=%s (dir=%s)\n",
                   pc & 0xFFFFFFu, label.c_str(), snap_dir.c_str());
            p = end;
            if (*p == ',') p++;
        }
    }
    uint32_t last_core0_pc = 0xFFFFFFFFu;  // sentinel: never matches a real PC
    int snap_seq = 0;

    // PC_HISTOGRAM - sample core 0's EX-stage PC every N cycles and
    // bucket the counts.  At sim end, print the top hottest buckets.
    //
    // Useful when the boot is technically running but tracing is silent --
    // tells you WHERE the CPU is spending its time when no [MARKER]
    // traces are firing.  Common pattern: a STOP-wait-for-IRQ loop will
    // have one dominant PC; a tight chip-RAM dispatcher loop will have a
    // small cluster of PCs.
    //
    //   PC_HISTOGRAM=1               -- enable (default off)
    //   PC_HISTOGRAM_INTERVAL=N      -- sample every N host cycles
    //                                   (default 1024 -- ~25K samples per
    //                                   second of sim, low overhead)
    //   PC_HISTOGRAM_TOP=K           -- show top K buckets (default 30)
    //   PC_HISTOGRAM_BUCKET_BITS=B   -- group PCs by lopping off the low B
    //                                   bits (default 0 = exact PC).  Set
    //                                   to 4 to bucket per-16-byte region.
    //
    // The histogram is keyed on the 24-bit PC (high byte ignored, matching
    // 68000's external bus).  Output goes to stdout after the regular
    // [sim] summary line at TIMEOUT/halt.
    bool pc_histogram_enabled = false;
    uint64_t pc_histogram_interval = 1024;
    int pc_histogram_top = 30;
    int pc_histogram_bucket_bits = 0;
    if (const char* s = std::getenv("PC_HISTOGRAM"))
        pc_histogram_enabled = std::atoi(s) != 0;
    if (const char* s = std::getenv("PC_HISTOGRAM_INTERVAL"))
        pc_histogram_interval = std::strtoull(s, nullptr, 0);
    if (const char* s = std::getenv("PC_HISTOGRAM_TOP"))
        pc_histogram_top = std::atoi(s);
    if (const char* s = std::getenv("PC_HISTOGRAM_BUCKET_BITS"))
        pc_histogram_bucket_bits = std::atoi(s);
    std::unordered_map<uint32_t, uint64_t> pc_histogram;
    if (pc_histogram_enabled)
        printf("[sim] PC_HISTOGRAM enabled (interval=%llu cycles, "
               "top=%d, bucket_bits=%d)\n",
               (unsigned long long)pc_histogram_interval,
               pc_histogram_top, pc_histogram_bucket_bits);

    while (cycle < max_cycles && !all_halted()) {
        // Default ext_kbd_wr to 0; the injection block below pulses it
        // for exactly one cycle when a scancode is due.
        top->ext_kbd_wr = 0;
        top->mem_poke_strobe = 0;
        if (!pokes.empty() && cycle >= poke_next_cycle) {
            top->mem_poke_addr   = pokes[poke_idx].first;
            top->mem_poke_data   = pokes[poke_idx].second;
            top->mem_poke_strobe = 1;
            poke_idx++;
            if (poke_idx >= pokes.size()) {
                poke_idx = 0;
                if (poke_period > 0) poke_next_cycle += poke_period;
                else                  poke_next_cycle = ~0ULL;  // no more
            }
        }
        if (kbd_inject_str && cycle >= kbd_next_cycle &&
            kbd_inject_str[kbd_idx] != 0) {
            // Translate ASCII -> Amiga rawkey using the SDL helper's
            // table.  For simplicity we hard-code the common subset
            // needed for typing commands: a..z, 0..9, space, enter.
            char ch = kbd_inject_str[kbd_idx];
            uint8_t rk = 0xFF;
            if (ch >= 'a' && ch <= 'z') {
                // Amiga A=$20, B=$35, C=$33, D=$22, E=$12, F=$23,
                // G=$24, H=$25, I=$17, J=$26, K=$27, L=$28, M=$37,
                // N=$36, O=$18, P=$19, Q=$10, R=$13, S=$21, T=$14,
                // U=$16, V=$34, W=$11, X=$32, Y=$15, Z=$31.
                static const uint8_t map[26] = {
                    0x20,0x35,0x33,0x22,0x12,0x23,0x24,0x25,0x17,0x26,
                    0x27,0x28,0x37,0x36,0x18,0x19,0x10,0x13,0x21,0x14,
                    0x16,0x34,0x11,0x32,0x15,0x31};
                rk = map[ch - 'a'];
            } else if (ch == ' ') {
                rk = 0x40;
            } else if (ch == '\n') {
                rk = 0x44;
            } else if (ch >= '0' && ch <= '9') {
                static const uint8_t map[10] = {
                    0x0A,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
                rk = map[ch - '0'];
            }
            if (rk != 0xFF) {
                // Real Amiga keyboard transform: keyboard sends the byte
                // ROL'd by one bit and inverted (~).  Per minimig's
                // ciaa.v "sdr_latch <= ~{keydat[6:0], keydat[7]}", the
                // value that ends up in CIA SDR is ~ROL8(rawkey, 1).
                // K1.3's keyboard.device handler then applies the inverse
                // (rawkey = ROR8(~SDR, 1)) to recover the original rawkey.
                uint8_t up_bit  = kbd_is_keydown ? 0x00 : 0x80;
                uint8_t rk_full = rk | up_bit;
                uint8_t rol     = (uint8_t)((rk_full << 1) | (rk_full >> 7));
                top->ext_kbd_byte = (uint8_t)(~rol);
                top->ext_kbd_wr   = 1;
            }
            if (!kbd_is_keydown) kbd_idx++;
            kbd_is_keydown = !kbd_is_keydown;
            kbd_next_cycle = cycle + kbd_inject_period;
        }
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
        cycle++;

        // CHIPRAM_SNAP: check core 0's EX-stage PC each cycle.  Fire on
        // the rising edge (PC transitions INTO a configured value) so a
        // long STOP at a snap PC doesn't fire thousands of times.
        if (!snap_triggers.empty()) {
            const uint32_t* pc_arr = reinterpret_cast<const uint32_t*>(&top->core_pc_flat);
            uint32_t cur_pc = pc_arr[0] & 0xFFFFFFu;
            if (cur_pc != last_core0_pc) {
                int all_done = 1;
                for (auto& t : snap_triggers) {
                    if (snap_limit > 0 && t.fired >= snap_limit) continue;
                    if (t.pc24 == cur_pc) {
                        const uint32_t* r_arr =
                            reinterpret_cast<const uint32_t*>(&top->retired_flat);
                        char path[1024];
                        std::snprintf(path, sizeof(path),
                            "%s/snap_%d_%s_r%u.bin",
                            snap_dir.c_str(), snap_seq++,
                            t.label.c_str(), r_arr[0]);
                        std::FILE* f = std::fopen(path, "wb");
                        if (f) {
                            for (uint32_t addr = 0; addr < 0x80000; addr += 4) {
                                uint32_t w = mem_peek_word(top, addr);
                                uint8_t b[4] = {
                                    (uint8_t)(w >> 24), (uint8_t)(w >> 16),
                                    (uint8_t)(w >> 8),  (uint8_t)w};
                                std::fwrite(b, 1, 4, f);
                            }
                            std::fclose(f);
                            printf("[sim] CHIPRAM_SNAP %s "
                                   "(pc=$%06X retired=%u cycle=%llu)\n",
                                   path, cur_pc, r_arr[0],
                                   (unsigned long long)cycle);
                        }
                        t.fired++;
                    }
                    if (snap_limit > 0 && t.fired < snap_limit) all_done = 0;
                }
                /* CHIPRAM_SNAP_HALT: when every trigger has fired its
                 * limit, stop the simulation. */
                if (snap_halt && snap_limit > 0 && all_done) {
                    printf("[sim] CHIPRAM_SNAP_HALT — all triggers fired, halting\n");
                    break;
                }
            }
            last_core0_pc = cur_pc;
        }

        // PC_HISTOGRAM sample.  Tap core 0's EX-stage PC at the requested
        // cadence and bucket it.  Skips cycle 0 (PC may still be reset).
        if (pc_histogram_enabled && cycle > 0 &&
            (cycle % pc_histogram_interval) == 0) {
            const uint32_t* pc_arr =
                reinterpret_cast<const uint32_t*>(&top->core_pc_flat);
            uint32_t pc24 = pc_arr[0] & 0xFFFFFFu;
            uint32_t bucket = pc24 & (0xFFFFFFu << pc_histogram_bucket_bits);
            pc_histogram[bucket]++;
        }

        if ((cycle & (CONSOLE_POLL - 1)) == 0) drain_console(top, host_tail);
    }
    drain_console(top, host_tail);
    std::fflush(stdout);

    bool halted_all = all_halted();
    int rc = 0;

    printf("[sim] cycles=%llu halted_mask=0x%x\n",
           (unsigned long long)cycle, (unsigned)top->halted);

    const uint32_t* retired_arr = reinterpret_cast<const uint32_t*>(&top->retired_flat);
    for (int c = 0; c < n_cores; c++) {
        uint16_t code = 0;
        if (n_cores <= 2) code = (top->halt_code_flat >> (16 * c)) & 0xFFFF;
        else {
            const uint32_t* hcf = reinterpret_cast<const uint32_t*>(&top->halt_code_flat);
            code = (hcf[c / 2] >> (16 * (c % 2))) & 0xFFFF;
        }
        uint32_t retired = retired_arr[c];
        bool h = ((top->halted >> c) & 1) != 0;
        double ipc = (cycle == 0) ? 0.0 : (double)retired / (double)cycle;
        printf("[sim] core%d halted=%d code=0x%04x retired=%u ipc=%.3f\n",
               c, h, code, (unsigned)retired, ipc);
        if (!h) rc = rc ? rc : 0xFFFF;
        else if (code != 0 && rc == 0) {
            // Shell exit codes are 8 bits.  Ensure the low byte is non-zero
            // so a halt code like 0xBB00 doesn't masquerade as success.
            rc = (code & 0xFF) ? (int)code : ((int)code | 1);
        }
    }
    if (!halted_all) {
        printf("[sim] TIMEOUT after %llu cycles\n", (unsigned long long)cycle);
        if (rc == 0) rc = 0xFFFE;
    }

    // PC_HISTOGRAM dump.  Sort buckets by hit count, print top N.
    if (pc_histogram_enabled && !pc_histogram.empty()) {
        std::vector<std::pair<uint32_t,uint64_t>> sorted(
            pc_histogram.begin(), pc_histogram.end());
        std::sort(sorted.begin(), sorted.end(),
                  [](const std::pair<uint32_t,uint64_t>& a,
                     const std::pair<uint32_t,uint64_t>& b){
                      return a.second > b.second;
                  });
        uint64_t total = 0;
        for (auto& kv : sorted) total += kv.second;
        int show = pc_histogram_top;
        if ((int)sorted.size() < show) show = (int)sorted.size();
        printf("[sim] PC_HISTOGRAM total_samples=%llu buckets=%zu, "
               "showing top %d:\n",
               (unsigned long long)total, sorted.size(), show);
        for (int i = 0; i < show; i++) {
            double pct = (100.0 * sorted[i].second) / (double)total;
            printf("  $%06X  %10llu samples (%6.2f%%)\n",
                   sorted[i].first,
                   (unsigned long long)sorted[i].second, pct);
        }
    }

    // Optional chip-RAM dump (binary, byte-addressed).  Useful for
    // poking at what Kickstart staged in memory even when bitplane DMA
    // never ran.  CHIPRAM_DUMP=path dumps the first 512 KB of chip RAM
    // (matches the kickstart-boot MEM_WORDS=131072 build).
    if (const char* path = std::getenv("CHIPRAM_DUMP")) {
        std::FILE* f = std::fopen(path, "wb");
        if (f) {
            for (uint32_t addr = 0; addr < 0x80000; addr += 4) {
                uint32_t w = mem_peek_word(top, addr);
                uint8_t b[4] = {
                    (uint8_t)(w >> 24), (uint8_t)(w >> 16),
                    (uint8_t)(w >> 8),  (uint8_t)w};
                std::fwrite(b, 1, 4, f);
            }
            std::fclose(f);
            printf("[sim] dumped chip RAM to %s (512 KB)\n", path);
        }
    }
    // Optional slow-RAM dump (Agnus trapdoor at $C00000-$C7FFFF).  Same
    // format as CHIPRAM_DUMP -- 512 KB of byte-addressed RAM.  K1.3
    // moves ExecBase + task structs into slow RAM when the chip-RAM
    // allocator's pri=-10 loses to slow-RAM's pri=0, so chip-RAM dumps
    // alone aren't enough to inspect the running OS state.
    if (const char* path = std::getenv("SLOWRAM_DUMP")) {
        std::FILE* f = std::fopen(path, "wb");
        if (f) {
            for (uint32_t addr = 0x00C00000u; addr < 0x00C80000u; addr += 4) {
                uint32_t w = mem_peek_word(top, addr);
                uint8_t b[4] = {
                    (uint8_t)(w >> 24), (uint8_t)(w >> 16),
                    (uint8_t)(w >> 8),  (uint8_t)w};
                std::fwrite(b, 1, 4, f);
            }
            std::fclose(f);
            printf("[sim] dumped slow RAM to %s (512 KB)\n", path);
        }
    }
    if (std::getenv("CHIPRAM_DUMP")) {
        // Also dump the Agnus bitplane fetch counter and the last
        // bitplane data, so we can tell whether bitplane DMA actually
        // ran and where it ended up.
        uint32_t bpl_fetches = mem_peek_word(top, 0x00FE9100u);
        uint32_t bpl1dat     = mem_peek_word(top, 0x00FE9104u);
        uint32_t bpl2dat     = mem_peek_word(top, 0x00FE9108u);
        uint32_t bplcon0     = mem_peek_word(top, 0x00FE911Cu);
        // DMACONR ($DFF002) reads the live DMACON value -- bit 8=BPLEN,
        // bit 9=DMAEN.  Both must be high for Agnus to fetch bitplanes.
        uint32_t dmaconr     = mem_peek_word(top, 0x00DFF002u);
        printf("[sim] Agnus: bpl_fetches=%u dmaconr=%04x (BPLEN=%d DMAEN=%d) bplcon0=%08x bpl1dat=%08x\n",
            bpl_fetches, dmaconr >> 16, (dmaconr >> (16 + 8)) & 1,
            (dmaconr >> (16 + 9)) & 1, bplcon0, bpl1dat);
    }

    // Optional framebuffer dump as a PPM (P6) so Kickstart's rendered
    // output can be visualised even in headless regression runs.  The
    // chunky 8 bpp framebuffer lives at $10000, FB_W*FB_H bytes.
    if (const char* path = std::getenv("FB_DUMP_PPM")) {
        const int fb_w = 256, fb_h = 192;
        std::FILE* f = std::fopen(path, "wb");
        if (f) {
            std::fprintf(f, "P6\n%d %d\n255\n", fb_w, fb_h);
            for (int y = 0; y < fb_h; y++) {
                for (int x = 0; x < fb_w; x++) {
                    uint32_t addr = 0x00010000u + uint32_t(y) * fb_w + uint32_t(x);
                    uint32_t w = mem_peek_word(top, addr);
                    uint32_t shift = (3u - (addr & 3u)) * 8u;
                    uint8_t  p = (w >> shift) & 0xFFu;
                    // RGB332 -> RGB888.
                    uint8_t r = (p >> 5) & 0x7;
                    uint8_t g = (p >> 2) & 0x7;
                    uint8_t b = (p     ) & 0x3;
                    uint8_t out[3] = {
                        (uint8_t)((r * 255) / 7),
                        (uint8_t)((g * 255) / 7),
                        (uint8_t)((b * 255) / 3),
                    };
                    std::fwrite(out, 1, 3, f);
                }
            }
            std::fclose(f);
            printf("[sim] dumped framebuffer to %s (%dx%d)\n", path, fb_w, fb_h);
        } else {
            std::fprintf(stderr, "[sim] FB_DUMP_PPM=%s open failed\n", path);
        }
    }

    return rc;
}

#ifdef HAVE_SDL2
// -------- SDL keycode -> Amiga rawkey lookup --------
// Amiga A500 raw scancodes (bits [6:0]).  Bit 7 of the byte sent to the
// keyboard SR sets the key-up flag.  Values from the well-known Commodore
// "Amiga ROM Kernel Reference Manual -- Devices" rawkey table.
//
// We map the most common keys (alphanumerics, punctuation, arrows, function
// keys, modifiers, space, return, backspace, ESC).  Anything not in the
// table is silently dropped — sufficient for driving Workbench.
static uint8_t sdl_keycode_to_amiga_rawkey(SDL_Keycode k) {
    switch (k) {
        // Number row
        case SDLK_BACKQUOTE:   return 0x00;
        case SDLK_1:           return 0x01;
        case SDLK_2:           return 0x02;
        case SDLK_3:           return 0x03;
        case SDLK_4:           return 0x04;
        case SDLK_5:           return 0x05;
        case SDLK_6:           return 0x06;
        case SDLK_7:           return 0x07;
        case SDLK_8:           return 0x08;
        case SDLK_9:           return 0x09;
        case SDLK_0:           return 0x0A;
        case SDLK_MINUS:       return 0x0B;
        case SDLK_EQUALS:      return 0x0C;
        case SDLK_BACKSLASH:   return 0x0D;
        // Top alpha row
        case SDLK_q:           return 0x10;
        case SDLK_w:           return 0x11;
        case SDLK_e:           return 0x12;
        case SDLK_r:           return 0x13;
        case SDLK_t:           return 0x14;
        case SDLK_y:           return 0x15;
        case SDLK_u:           return 0x16;
        case SDLK_i:           return 0x17;
        case SDLK_o:           return 0x18;
        case SDLK_p:           return 0x19;
        case SDLK_LEFTBRACKET: return 0x1A;
        case SDLK_RIGHTBRACKET:return 0x1B;
        // Home row
        case SDLK_a:           return 0x20;
        case SDLK_s:           return 0x21;
        case SDLK_d:           return 0x22;
        case SDLK_f:           return 0x23;
        case SDLK_g:           return 0x24;
        case SDLK_h:           return 0x25;
        case SDLK_j:           return 0x26;
        case SDLK_k:           return 0x27;
        case SDLK_l:           return 0x28;
        case SDLK_SEMICOLON:   return 0x29;
        case SDLK_QUOTE:       return 0x2A;
        // Bottom alpha row
        case SDLK_z:           return 0x31;
        case SDLK_x:           return 0x32;
        case SDLK_c:           return 0x33;
        case SDLK_v:           return 0x34;
        case SDLK_b:           return 0x35;
        case SDLK_n:           return 0x36;
        case SDLK_m:           return 0x37;
        case SDLK_COMMA:       return 0x38;
        case SDLK_PERIOD:      return 0x39;
        case SDLK_SLASH:       return 0x3A;
        // Misc
        case SDLK_SPACE:       return 0x40;
        case SDLK_BACKSPACE:   return 0x41;
        case SDLK_TAB:         return 0x42;
        case SDLK_RETURN:      return 0x44;
        case SDLK_KP_ENTER:    return 0x43;
        case SDLK_DELETE:      return 0x46;
        case SDLK_UP:          return 0x4C;
        case SDLK_DOWN:        return 0x4D;
        case SDLK_RIGHT:       return 0x4E;
        case SDLK_LEFT:        return 0x4F;
        // Function keys
        case SDLK_F1:          return 0x50;
        case SDLK_F2:          return 0x51;
        case SDLK_F3:          return 0x52;
        case SDLK_F4:          return 0x53;
        case SDLK_F5:          return 0x54;
        case SDLK_F6:          return 0x55;
        case SDLK_F7:          return 0x56;
        case SDLK_F8:          return 0x57;
        case SDLK_F9:          return 0x58;
        case SDLK_F10:         return 0x59;
        // Modifiers / qualifiers
        case SDLK_LSHIFT:      return 0x60;
        case SDLK_RSHIFT:      return 0x61;
        case SDLK_CAPSLOCK:    return 0x62;
        case SDLK_LCTRL:
        case SDLK_RCTRL:       return 0x63;
        case SDLK_LALT:        return 0x64;
        case SDLK_RALT:        return 0x65;
        // The Mac uses the GUI ("Cmd") key in the position where the
        // Amiga has the Amiga key — map both to LAMIGA / RAMIGA.
        case SDLK_LGUI:        return 0x66;
        case SDLK_RGUI:        return 0x67;
        default:               return 0xFF;   // sentinel: no mapping
    }
}

// Build a 256-entry RGB332 -> ARGB8888 palette.
static void build_palette(uint32_t* palette) {
    for (int i = 0; i < 256; i++) {
        int r3 = (i >> 5) & 0x7;
        int g3 = (i >> 2) & 0x7;
        int b2 =  i       & 0x3;
        int r = (r3 * 255) / 7;
        int g = (g3 * 255) / 7;
        int b = (b2 * 255) / 3;
        palette[i] = (0xFFu << 24) | (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
    }
}

// --- SDL_audio: lock-free ring buffer fed by the simulator, drained
// by the audio callback. ---
struct AudioRing {
    int16_t* buf;
    int      capacity;          // power of two
    int      mask;
    volatile int wr;            // producer index (sim thread)
    volatile int rd;            // consumer index (audio callback)
};

static AudioRing g_audio_ring;

static void audio_callback(void* udata, Uint8* stream, int len) {
    (void)udata;
    int16_t* out = reinterpret_cast<int16_t*>(stream);
    int n = len / 2; // 16-bit samples
    for (int i = 0; i < n; i++) {
        if (g_audio_ring.rd == g_audio_ring.wr) {
            out[i] = 0; // underrun -> silence
        } else {
            out[i] = g_audio_ring.buf[g_audio_ring.rd];
            g_audio_ring.rd = (g_audio_ring.rd + 1) & g_audio_ring.mask;
        }
    }
}

static int run_graphics(Vm68k_top* top, uint64_t max_cycles, int n_cores) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 2;
    }

    // --- Open audio device ---
    SDL_AudioSpec desired = {};
    desired.freq = AUDIO_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = AUDIO_BUF_SAMPLES;
    desired.callback = audio_callback;
    SDL_AudioSpec obtained;
    SDL_AudioDeviceID audio_dev =
        SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
    if (audio_dev == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice failed: %s (audio disabled)\n", SDL_GetError());
    }

    int ring_cap = 8192;
    g_audio_ring.buf = new int16_t[ring_cap];
    g_audio_ring.capacity = ring_cap;
    g_audio_ring.mask = ring_cap - 1;
    g_audio_ring.wr = 0;
    g_audio_ring.rd = 0;

    if (audio_dev != 0) SDL_PauseAudioDevice(audio_dev, 0);


    SDL_Window* win = SDL_CreateWindow(
        "fast_68000 framebuffer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        FB_W * FB_SCALE, FB_H * FB_SCALE,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 2;
    }
    SDL_RenderSetLogicalSize(ren, FB_W, FB_H);

    SDL_Texture* tex = SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, FB_W, FB_H);

    uint32_t palette[256];
    build_palette(palette);

    auto all_halted = [&]() {
        for (int c = 0; c < n_cores; c++) {
            if (((top->halted >> c) & 1) == 0) return false;
        }
        return true;
    };

    auto pixel_buf = new uint32_t[FB_W * FB_H];

    // K1.3 chipram-render mode: when RENDER_K13_COP1LC is set, ignore the
    // chunky framebuffer and instead walk Kickstart's Copper list each
    // frame to produce the visible screen.  Used by `make kickstart-graphics`.
    //
    // RENDER_K13_COP1LC=auto enables per-frame autodetect of Intuition's
    // active WB1.3 Copper list (by scanning chip RAM for the COLOR00=\$005A
    // + COLOR01=\$0FFF signature).  Until the list appears, the screen
    // stays blank.  This is the right default for live K1.3+WB1.3 boots
    // because Intuition allocates the list wherever — \$2368 is the legacy
    // chip layout and \$100C8 is what we've seen on a recent WB1.3 boot.
    //
    // Any other value (e.g. RENDER_K13_COP1LC=0x100C8) locks the renderer
    // to that fixed address.
    uint32_t k13_cop1lc = 0;
    bool     k13_auto   = false;
    if (const char* s = std::getenv("RENDER_K13_COP1LC")) {
        if (std::strcmp(s, "auto") == 0) {
            k13_auto   = true;
            k13_cop1lc = 0;   // populated each frame
        } else {
            k13_cop1lc = uint32_t(std::strtoul(s, nullptr, 0));
        }
    }

    using clk_t = std::chrono::steady_clock;
    auto next_render = clk_t::now();

    // ---- SDL keyboard / mouse plumbing ----
    // Pending scancode queue (Amiga rawkey bytes, including key-up flag).
    // We drain at most one byte per CYCLES_PER_BATCH so the CIA SR has a
    // full real-Amiga delay between scancodes (real KB sends ~10 KHz).
    static constexpr int   KBD_Q_CAP = 256;
    uint8_t kbd_q[KBD_Q_CAP] = {0};
    int     kbd_q_head = 0;       // producer
    int     kbd_q_tail = 0;       // consumer
    auto kbd_q_push = [&](uint8_t b) {
        int next = (kbd_q_head + 1) & (KBD_Q_CAP - 1);
        if (next == kbd_q_tail) return;       // full -> drop
        kbd_q[kbd_q_head] = b;
        kbd_q_head = next;
    };
    auto kbd_q_pop = [&](uint8_t* out) {
        if (kbd_q_head == kbd_q_tail) return false;
        *out = kbd_q[kbd_q_tail];
        kbd_q_tail = (kbd_q_tail + 1) & (KBD_Q_CAP - 1);
        return true;
    };

    // Mouse state: 8-bit wrap-around quadrature counters, accumulated
    // from SDL_MOUSEMOTION xrel/yrel.  Workbench reads JOY0DAT every VBL
    // and subtracts the previous sample to recover the per-frame delta.
    uint8_t mouse_x_acc = 0;
    uint8_t mouse_y_acc = 0;
    bool    mouse_l_down = false;
    bool    mouse_r_down = false;

    uint64_t cycle = 0;
    bool quit = false;
    // Periodic heartbeat so a long quiet boot (BOOT_TRACE=0 silences all
    // RESINIT / STOP traces) doesn't look hung.
    auto last_heartbeat = clk_t::now();
    while (!quit && cycle < max_cycles && !all_halted()) {
        {
            auto now2 = clk_t::now();
            if (now2 - last_heartbeat > std::chrono::seconds(15)) {
                last_heartbeat = now2;
                const uint32_t* rarr =
                    reinterpret_cast<const uint32_t*>(&top->retired_flat);
                std::printf("[sim] heartbeat: cycle=%llu retired=%u "
                            "k13_cop1lc=$%X\n",
                            (unsigned long long)cycle, rarr[0], k13_cop1lc);
                std::fflush(stdout);
            }
        }
        // Push mouse state every batch.  These are level signals — the bus
        // samples them whenever JOY0DAT / POTGOR is read.  CIA-A's PRA
        // input is similarly level-sensitive (Workbench polls it via the
        // CIA-A interrupt + read sequence).
        top->mouse_x_count = mouse_x_acc;
        top->mouse_y_count = mouse_y_acc;
        top->mouse_btn_l   = mouse_l_down ? 1 : 0;
        top->mouse_btn_r   = mouse_r_down ? 1 : 0;

        // Drain at most one scancode byte per batch (~one rawkey per
        // render frame, ~30 Hz).  Real Amiga KB tops out around 100 Hz
        // raw-key rate so this is the right ballpark.
        uint8_t scancode_to_send = 0;
        bool    have_scancode    = kbd_q_pop(&scancode_to_send);

        // Step many simulated cycles per wall iteration so the CPU makes
        // visible progress between renders. Tune CYCLES_PER_BATCH to balance
        // smoothness vs. simulator throughput.
        const int CYCLES_PER_BATCH = 2000;
        int audio_cycle_acc = 0;
        for (int b = 0; b < CYCLES_PER_BATCH && cycle < max_cycles && !all_halted(); b++) {
            // Pulse ext_kbd_wr for exactly one host clock with the
            // scancode (CIA's posedge clk latches kbd_byte on kbd_wr=1).
            // First sub-cycle of the batch.
            if (have_scancode && b == 0) {
                top->ext_kbd_byte = scancode_to_send;
                top->ext_kbd_wr   = 1;
            } else {
                top->ext_kbd_wr   = 0;
            }
            top->clk = 0; top->eval();
            top->clk = 1; top->eval();
            cycle++;
            audio_cycle_acc++;
            if (audio_cycle_acc >= CYCLES_PER_AUDIO) {
                audio_cycle_acc = 0;
                // Sample Paula's mixed output as interleaved L,R 16-bit.
                int wr   = g_audio_ring.wr;
                int next = (wr + 2) & g_audio_ring.mask;
                if (next != g_audio_ring.rd) {
                    g_audio_ring.buf[wr]               = (int16_t)top->audio_l;
                    g_audio_ring.buf[(wr + 1) & g_audio_ring.mask] = (int16_t)top->audio_r;
                    g_audio_ring.wr = next;
                }
            }
        }

        // Pump SDL events.  Translate keyboard / mouse activity into
        // Amiga-shaped inputs (raw keys queued for CIA-A; mouse
        // accumulators driven into JOY0DAT/POTGOR/CIA-A PRA).
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.repeat) continue;  // don't queue auto-repeat
                uint8_t rk = sdl_keycode_to_amiga_rawkey(e.key.keysym.sym);
                if (rk != 0xFF) kbd_q_push(rk);          // bit7=0 -> key-down
            }
            else if (e.type == SDL_KEYUP) {
                uint8_t rk = sdl_keycode_to_amiga_rawkey(e.key.keysym.sym);
                if (rk != 0xFF) kbd_q_push(rk | 0x80);   // bit7=1 -> key-up
            }
            else if (e.type == SDL_MOUSEMOTION) {
                // SDL gives signed deltas in window pixels.  Real Amiga
                // mouse advances the quadrature counter by 1 per ~125 µm
                // of motion; for screen sanity we just feed pixel deltas
                // 1:1 — Workbench's tracking ratio is configurable in
                // Preferences anyway.
                mouse_x_acc = (uint8_t)(mouse_x_acc + (int8_t)e.motion.xrel);
                mouse_y_acc = (uint8_t)(mouse_y_acc + (int8_t)e.motion.yrel);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT)  mouse_l_down = true;
                if (e.button.button == SDL_BUTTON_RIGHT) mouse_r_down = true;
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT)  mouse_l_down = false;
                if (e.button.button == SDL_BUTTON_RIGHT) mouse_r_down = false;
            }
        }

        // Throttle render to ~30 FPS.
        auto now = clk_t::now();
        if (now < next_render) continue;
        next_render = now + std::chrono::milliseconds(WALL_FRAME_MS);

        // K1.3 mode: walk the Copper list each frame; skip chunky-FB sampling.
        // In autodetect mode, refresh k13_cop1lc each frame.  Once Intuition's
        // list lands in chip RAM (typically at \$100C8 on a real K1.3+WB1.3
        // boot), the scan finds it and the renderer starts producing the
        // Workbench screen.  Until then, the screen stays at whatever the
        // last render produced (initially the SDL clear color).
        if (k13_auto) {
            static uint32_t last_found = 0;
            uint32_t found = k13_autodetect_cop1lc(top);
            if (found) {
                if (found != last_found) {
                    std::printf("[sim] RENDER_K13_COP1LC=auto: locked onto "
                                "Intuition Copper list at $%X\n", found);
                    last_found = found;
                }
                k13_cop1lc = found;
            }
        }
        if (k13_cop1lc) {
            render_k13_chipram(top, k13_cop1lc, pixel_buf, FB_W, FB_H);
            top->fb_peek_addr = 0;
            top->eval();
            SDL_UpdateTexture(tex, nullptr, pixel_buf, FB_W * 4);
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, tex, nullptr, nullptr);
            SDL_RenderPresent(ren);
            // Optional: dump the current ARGB framebuffer to PPM each Nth
            // wall-frame so we can confirm via tooling what's on screen.
            // FB_DUMP_PPM_LIVE=path enables the dump (every ~30 frames ~= 1s).
            if (const char* path = std::getenv("FB_DUMP_PPM_LIVE")) {
                static int frame_n = 0;
                if (++frame_n >= 30) {
                    frame_n = 0;
                    std::FILE* f = std::fopen(path, "wb");
                    if (f) {
                        std::fprintf(f, "P6\n%d %d\n255\n", FB_W, FB_H);
                        for (int i = 0; i < FB_W * FB_H; i++) {
                            uint32_t p = pixel_buf[i];
                            uint8_t out[3] = {
                                (uint8_t)((p >> 16) & 0xFF),
                                (uint8_t)((p >> 8)  & 0xFF),
                                (uint8_t)( p        & 0xFF)};
                            std::fwrite(out, 1, 3, f);
                        }
                        std::fclose(f);
                    }
                }
            }
            continue;
        }

        // Sample the chunky 8 bpp framebuffer.
        for (int y = 0; y < FB_H; y++) {
            for (int x = 0; x < FB_W; x++) {
                uint32_t byte_addr = FB_BASE + uint32_t(y) * FB_W + uint32_t(x);
                uint8_t p = uint8_t(read_fb_byte(top, byte_addr));
                pixel_buf[y * FB_W + x] = palette[p];
            }
        }
        // Overlay the 1bpp bitplane (set bits paint as white).  16 pixels
        // per 16-bit word, MSB = leftmost pixel of the word.
        for (int y = 0; y < FB_H; y++) {
            for (int wx = 0; wx < FB_W / 16; wx++) {
                uint32_t byte_addr = BP_BASE + uint32_t(y) * (FB_W / 8) + uint32_t(wx) * 2;
                uint8_t hi = uint8_t(read_fb_byte(top, byte_addr));
                uint8_t lo = uint8_t(read_fb_byte(top, byte_addr + 1));
                uint16_t w = (uint16_t(hi) << 8) | lo;
                for (int b = 0; b < 16; b++) {
                    if ((w >> (15 - b)) & 1u) {
                        int x = wx * 16 + b;
                        pixel_buf[y * FB_W + x] = 0xFFFFFFFFu;
                    }
                }
            }
        }
        // Restore fb_peek_addr to a neutral value (any read is allowed; we
        // pick 0 so post-eval state is deterministic).
        top->fb_peek_addr = 0;
        top->eval();

        SDL_UpdateTexture(tex, nullptr, pixel_buf, FB_W * 4);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, nullptr, nullptr);
        SDL_RenderPresent(ren);
    }

    // Once the simulator stops advancing (either every core halted or we hit
    // max_cycles), leave the last frame on screen until the user closes the
    // window. This is the convenient behavior for a long-running demo.
    if (!quit) {
        // Final render of the halt state (chunky FB + bitplane overlay).
        for (int y = 0; y < FB_H; y++) {
            for (int x = 0; x < FB_W; x++) {
                uint32_t byte_addr = FB_BASE + uint32_t(y) * FB_W + uint32_t(x);
                uint8_t p = uint8_t(read_fb_byte(top, byte_addr));
                pixel_buf[y * FB_W + x] = palette[p];
            }
        }
        for (int y = 0; y < FB_H; y++) {
            for (int wx = 0; wx < FB_W / 16; wx++) {
                uint32_t byte_addr = BP_BASE + uint32_t(y) * (FB_W / 8) + uint32_t(wx) * 2;
                uint8_t hi = uint8_t(read_fb_byte(top, byte_addr));
                uint8_t lo = uint8_t(read_fb_byte(top, byte_addr + 1));
                uint16_t w = (uint16_t(hi) << 8) | lo;
                for (int b = 0; b < 16; b++) {
                    if ((w >> (15 - b)) & 1u) {
                        int x = wx * 16 + b;
                        pixel_buf[y * FB_W + x] = 0xFFFFFFFFu;
                    }
                }
            }
        }
        SDL_UpdateTexture(tex, nullptr, pixel_buf, FB_W * 4);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, nullptr, nullptr);
        SDL_RenderPresent(ren);
        SDL_SetWindowTitle(win, "fast_68000 framebuffer (stopped) — press ESC to quit");

        while (!quit) {
            SDL_Event e;
            while (SDL_WaitEvent(&e)) {
                if (e.type == SDL_QUIT) { quit = true; break; }
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) { quit = true; break; }
            }
        }
    }

    delete[] pixel_buf;
    if (audio_dev != 0) {
        SDL_PauseAudioDevice(audio_dev, 1);
        SDL_CloseAudioDevice(audio_dev);
    }
    delete[] g_audio_ring.buf;
    g_audio_ring.buf = nullptr;
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    // Print summary like the regression mode for consistency.
    printf("[sim] cycles=%llu halted_mask=0x%x\n",
           (unsigned long long)cycle, (unsigned)top->halted);
    const uint32_t* retired_arr = reinterpret_cast<const uint32_t*>(&top->retired_flat);
    for (int c = 0; c < n_cores; c++) {
        uint16_t code = 0;
        if (n_cores <= 2) code = (top->halt_code_flat >> (16 * c)) & 0xFFFF;
        else {
            const uint32_t* hcf = reinterpret_cast<const uint32_t*>(&top->halt_code_flat);
            code = (hcf[c / 2] >> (16 * (c % 2))) & 0xFFFF;
        }
        uint32_t retired = retired_arr[c];
        bool h = ((top->halted >> c) & 1) != 0;
        printf("[sim] core%d halted=%d code=0x%04x retired=%u\n",
               c, h, code, (unsigned)retired);
    }
    return 0;
}
#endif // HAVE_SDL2
