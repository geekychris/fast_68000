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

    uint32_t host_tail = 0;
    uint64_t cycle = 0;
    while (cycle < max_cycles && !all_halted()) {
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
        cycle++;
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
    uint32_t k13_cop1lc = 0;
    if (const char* s = std::getenv("RENDER_K13_COP1LC")) {
        k13_cop1lc = uint32_t(std::strtoul(s, nullptr, 0));
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
    while (!quit && cycle < max_cycles && !all_halted()) {
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
