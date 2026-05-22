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
    // never ran.  CHIPRAM_DUMP=path dumps the first 256 KB of chip RAM.
    if (const char* path = std::getenv("CHIPRAM_DUMP")) {
        std::FILE* f = std::fopen(path, "wb");
        if (f) {
            for (uint32_t addr = 0; addr < 0x40000; addr += 4) {
                uint32_t w = mem_peek_word(top, addr);
                uint8_t b[4] = {
                    (uint8_t)(w >> 24), (uint8_t)(w >> 16),
                    (uint8_t)(w >> 8),  (uint8_t)w};
                std::fwrite(b, 1, 4, f);
            }
            std::fclose(f);
            printf("[sim] dumped chip RAM to %s (256 KB)\n", path);
        }
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

    using clk_t = std::chrono::steady_clock;
    auto next_render = clk_t::now();

    uint64_t cycle = 0;
    bool quit = false;
    while (!quit && cycle < max_cycles && !all_halted()) {
        // Step many simulated cycles per wall iteration so the CPU makes
        // visible progress between renders. Tune CYCLES_PER_BATCH to balance
        // smoothness vs. simulator throughput.
        const int CYCLES_PER_BATCH = 2000;
        int audio_cycle_acc = 0;
        for (int b = 0; b < CYCLES_PER_BATCH && cycle < max_cycles && !all_halted(); b++) {
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

        // Pump SDL events.
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) quit = true;
        }

        // Throttle render to ~30 FPS.
        auto now = clk_t::now();
        if (now < next_render) continue;
        next_render = now + std::chrono::milliseconds(WALL_FRAME_MS);

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
