// GDB Remote Serial Protocol (RSP) server for the Verilator 68k sim.
//
// Listens on TCP $GDB_PORT (default 2159) at sim start.  Accepts ONE
// client connection.  Once connected, the sim runs under debugger
// control: `c` resumes, `s` single-steps, breakpoints halt.
//
// Spec: https://sourceware.org/gdb/current/onlinedocs/gdb/Remote-Protocol.html
//
// Wire it in sim_main.cpp's run_regression() like so:
//
//     GdbServer gdb;
//     if (!gdb.init_from_env()) {  // checks $GDB_PORT, opens listener
//         /* no debugger; run normally */
//     }
//     gdb.wait_for_client_if_enabled();
//     ...
//     while (cycle < max_cycles) {
//         if (gdb.enabled() && !gdb.tick(top, cycle)) {
//             // Halted by debugger; gdb.tick will block until resumed
//             continue;
//         }
//         top->clk = 0; top->eval();
//         top->clk = 1; top->eval();
//         cycle++;
//         gdb.notify_step(top, cycle);  // checks breakpoints
//     }
//
// Single-threaded design: no threads, no locks.  We poll the socket
// non-blockingly at the start of each batch and process all pending
// packets before stepping the clock.  Halted state blocks the main
// loop until the debugger issues `c`/`s`/etc.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Vm68k_top;  // forward

class GdbServer {
public:
    GdbServer();
    ~GdbServer();

    // Reads $GDB_PORT (decimal).  Opens listener if set.  Returns true
    // iff the debugger is enabled (listener opened).
    bool init_from_env();

    // True iff a debugger session is configured (env was set).
    bool enabled() const { return enabled_; }

    // If enabled and no client yet, accept blocking.  Prints status.
    void wait_for_client_if_enabled();

    // Called from the sim loop at the top of every cycle batch.  Drains
    // pending packets from the socket and processes them.  Returns true
    // if the sim should advance the clock this cycle, false if it must
    // stay halted (block).  When false is returned, sim_main should
    // re-call tick() in a tight loop without ticking the clock.
    bool tick(Vm68k_top* top, uint64_t cycle);

    // Called after every clock advance.  Checks breakpoints / watchpoints
    // against the new state and may transition us to halted.
    void notify_step(Vm68k_top* top, uint64_t cycle);

private:
    bool        enabled_ = false;
    int         listen_fd_ = -1;
    int         client_fd_ = -1;
    int         port_ = 2159;

    // Debugger control state.
    bool        halted_ = true;     // start halted; debugger says `c` to run
    int64_t     step_remaining_ = 0;  // >0 = single-stepping; runs N more cycles
    bool        first_stop_sent_ = false;

    // Breakpoints (sw bkpt = halt when ex_pc matches).
    std::vector<uint32_t> bps_;
    // Watchpoints: (addr, kind) where kind: 2=write, 3=read, 4=access.
    struct Watch {
        uint32_t addr;
        uint32_t len;
        int      kind;  // 2/3/4
    };
    std::vector<Watch> wps_;

    // Packet I/O.
    std::string rx_buf_;
    bool read_packet(std::string& out);   // returns true if a packet was extracted
    void send_packet(const std::string& payload);
    void send_ok() { send_packet("OK"); }
    void send_empty() { send_packet(""); }
    void send_signal(int sig);  // T05 etc.

    // Packet dispatch.
    void handle_packet(Vm68k_top* top, const std::string& pkt);
    void handle_q(Vm68k_top* top, const std::string& pkt);
    void handle_v(Vm68k_top* top, const std::string& pkt);
    void handle_g(Vm68k_top* top);
    void handle_G(Vm68k_top* top, const std::string& pkt);
    void handle_p(Vm68k_top* top, const std::string& pkt);
    void handle_P(Vm68k_top* top, const std::string& pkt);
    void handle_m(Vm68k_top* top, const std::string& pkt);
    void handle_M(Vm68k_top* top, const std::string& pkt);
    void handle_Z(const std::string& pkt);
    void handle_z(const std::string& pkt);

    // Helpers.
    static std::string hex_encode_bytes(const uint8_t* p, size_t n);
    static std::vector<uint8_t> hex_decode_bytes(const std::string& s);
    static std::string be32_hex(uint32_t v);
    static std::string be16_hex(uint16_t v);

    // 68k register access via Verilator-exposed signals.
    uint32_t read_reg(Vm68k_top* top, int idx);   // 0..7=D, 8..15=A, 16=SR(32-bit cast), 17=PC
    void     write_reg(Vm68k_top* top, int idx, uint32_t v);
};
