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
#define FB_BASE 0x00010000u   // byte address of the framebuffer
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

static int run_regression(Vm68k_top* top, uint64_t max_cycles, int n_cores) {
    auto all_halted = [&]() {
        for (int c = 0; c < n_cores; c++) {
            if (((top->halted >> c) & 1) == 0) return false;
        }
        return true;
    };

    uint64_t cycle = 0;
    while (cycle < max_cycles && !all_halted()) {
        top->clk = 0; top->eval();
        top->clk = 1; top->eval();
        cycle++;
    }

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
        else if (code != 0 && rc == 0) rc = code;
    }
    if (!halted_all) {
        printf("[sim] TIMEOUT after %llu cycles\n", (unsigned long long)cycle);
        if (rc == 0) rc = 0xFFFE;
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

static int run_graphics(Vm68k_top* top, uint64_t max_cycles, int n_cores) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 2;
    }

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
        for (int b = 0; b < CYCLES_PER_BATCH && cycle < max_cycles && !all_halted(); b++) {
            top->clk = 0; top->eval();
            top->clk = 1; top->eval();
            cycle++;
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

        // Sample the framebuffer.
        for (int y = 0; y < FB_H; y++) {
            for (int x = 0; x < FB_W; x++) {
                uint32_t byte_addr = FB_BASE + uint32_t(y) * FB_W + uint32_t(x);
                uint8_t p = uint8_t(read_fb_byte(top, byte_addr));
                pixel_buf[y * FB_W + x] = palette[p];
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

    // On halt, leave the final frame on screen until the user closes the window.
    if (all_halted() && !quit) {
        // Final render of the halt state.
        for (int y = 0; y < FB_H; y++) {
            for (int x = 0; x < FB_W; x++) {
                uint32_t byte_addr = FB_BASE + uint32_t(y) * FB_W + uint32_t(x);
                uint8_t p = uint8_t(read_fb_byte(top, byte_addr));
                pixel_buf[y * FB_W + x] = palette[p];
            }
        }
        SDL_UpdateTexture(tex, nullptr, pixel_buf, FB_W * 4);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, nullptr, nullptr);
        SDL_RenderPresent(ren);
        SDL_SetWindowTitle(win, "fast_68000 framebuffer (halted) — press ESC to quit");

        while (!quit) {
            SDL_Event e;
            while (SDL_WaitEvent(&e)) {
                if (e.type == SDL_QUIT) { quit = true; break; }
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) { quit = true; break; }
            }
        }
    }

    delete[] pixel_buf;
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
