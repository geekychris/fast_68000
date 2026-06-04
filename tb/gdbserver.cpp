// GDB Remote Serial Protocol server.  See gdbserver.h.

#include "gdbserver.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Vm68k_top.h"

// External helper from sim_main.cpp for memory read.  Returns 32-bit
// word at the given byte address (word-aligned).  We use it byte-wise.
extern uint32_t mem_peek_word(Vm68k_top* top, uint32_t addr);

// External helper for memory write.  The bus has a mem_poke_strobe input;
// we synthesize a single-cycle write here.
static void mem_poke_word(Vm68k_top* top, uint32_t addr, uint32_t value) {
    top->mem_poke_addr   = addr;
    top->mem_poke_data   = value;
    top->mem_poke_strobe = 1;
    top->clk = 0; top->eval();
    top->clk = 1; top->eval();
    top->mem_poke_strobe = 0;
}

GdbServer::GdbServer() = default;

GdbServer::~GdbServer() {
    if (client_fd_ >= 0) close(client_fd_);
    if (listen_fd_ >= 0) close(listen_fd_);
}

bool GdbServer::init_from_env() {
    const char* env = std::getenv("GDB_PORT");
    if (!env) return false;
    port_ = std::atoi(env);
    if (port_ <= 0) port_ = 2159;

    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) {
        std::perror("[gdb] socket");
        return false;
    }
    int yes = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons((uint16_t)port_);
    if (bind(listen_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::perror("[gdb] bind");
        close(listen_fd_);
        listen_fd_ = -1;
        return false;
    }
    if (listen(listen_fd_, 1) < 0) {
        std::perror("[gdb] listen");
        close(listen_fd_);
        listen_fd_ = -1;
        return false;
    }
    enabled_ = true;
    std::printf("[gdb] listening on tcp:127.0.0.1:%d "
                "(target remote :%d from m68k-gdb)\n",
                port_, port_);
    std::fflush(stdout);
    return true;
}

void GdbServer::wait_for_client_if_enabled() {
    if (!enabled_ || client_fd_ >= 0) return;
    std::printf("[gdb] waiting for client connect...\n");
    std::fflush(stdout);
    sockaddr_in caddr{};
    socklen_t clen = sizeof(caddr);
    client_fd_ = accept(listen_fd_, (sockaddr*)&caddr, &clen);
    if (client_fd_ < 0) {
        std::perror("[gdb] accept");
        enabled_ = false;
        return;
    }
    int yes = 1;
    setsockopt(client_fd_, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    // Non-blocking so we can poll-then-step.
    int flags = fcntl(client_fd_, F_GETFL, 0);
    fcntl(client_fd_, F_SETFL, flags | O_NONBLOCK);
    std::printf("[gdb] client connected.  Sim starts halted at PC=$%06X.\n",
                /* dummy until first read */ 0);
    std::fflush(stdout);
    halted_ = true;
    first_stop_sent_ = false;
}

// ---- Packet I/O ----

bool GdbServer::read_packet(std::string& out) {
    // Top up rx_buf_ from the socket.
    uint8_t buf[1024];
    while (true) {
        ssize_t n = recv(client_fd_, buf, sizeof(buf), 0);
        if (n > 0) {
            rx_buf_.append((const char*)buf, (size_t)n);
        } else if (n == 0) {
            // EOF
            close(client_fd_);
            client_fd_ = -1;
            enabled_ = false;
            std::printf("[gdb] client disconnected.\n");
            return false;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            if (errno == EINTR) continue;
            std::perror("[gdb] recv");
            return false;
        }
    }

    // Parse one packet:  $<payload>#<2-hex-digit checksum>
    while (!rx_buf_.empty()) {
        if (rx_buf_[0] == '+' || rx_buf_[0] == '-') {
            rx_buf_.erase(0, 1);
            continue;
        }
        if (rx_buf_[0] == 0x03) {
            // Ctrl-C from gdb = interrupt request.
            rx_buf_.erase(0, 1);
            halted_ = true;
            send_signal(2);  // SIGINT
            continue;
        }
        if (rx_buf_[0] != '$') {
            // Skip junk.
            rx_buf_.erase(0, 1);
            continue;
        }
        size_t hash = rx_buf_.find('#');
        if (hash == std::string::npos || hash + 2 >= rx_buf_.size()) {
            return false;  // not complete yet
        }
        out = rx_buf_.substr(1, hash - 1);
        rx_buf_.erase(0, hash + 3);
        // ACK with '+' (we don't verify checksums for v1).
        const char ack = '+';
        ::send(client_fd_, &ack, 1, 0);
        return true;
    }
    return false;
}

void GdbServer::send_packet(const std::string& payload) {
    uint8_t cs = 0;
    for (char c : payload) cs = (uint8_t)(cs + (uint8_t)c);
    char buf[16];
    std::snprintf(buf, sizeof(buf), "$%s#%02x", "", cs);
    std::string out = "$";
    out += payload;
    out += '#';
    char hex[3];
    std::snprintf(hex, sizeof(hex), "%02x", cs);
    out += hex;
    ::send(client_fd_, out.data(), out.size(), 0);
}

void GdbServer::send_signal(int sig) {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "S%02x", sig);
    send_packet(buf);
}

// ---- Register access via Verilog dbg_regs_flat / dbg_sr_flat / core_pc_flat ----

uint32_t GdbServer::read_reg(Vm68k_top* top, int idx) {
    if (idx >= 0 && idx <= 15) {
        // D0..D7, A0..A7 - 16 × 32-bit values flattened into dbg_regs_flat.
        const uint32_t* regs = reinterpret_cast<const uint32_t*>(&top->dbg_regs_flat);
        return regs[idx];
    }
    if (idx == 16) {
        // PS / SR.  Pack as 32-bit zero-extended.
        const uint16_t* sr = reinterpret_cast<const uint16_t*>(&top->dbg_sr_flat);
        return (uint32_t)sr[0];
    }
    if (idx == 17) {
        const uint32_t* pc = reinterpret_cast<const uint32_t*>(&top->core_pc_flat);
        return pc[0];
    }
    return 0;
}

void GdbServer::write_reg(Vm68k_top* /*top*/, int /*idx*/, uint32_t /*v*/) {
    // Writing to internal regs isn't wired through yet (would need a back-
    // door write port).  GDB's `G`/`P` commands will silently no-op.  For
    // v1 this is fine — most debugging is read + memory write.
}

// ---- Hex helpers ----

std::string GdbServer::hex_encode_bytes(const uint8_t* p, size_t n) {
    static const char digs[] = "0123456789abcdef";
    std::string s;
    s.reserve(n * 2);
    for (size_t i = 0; i < n; i++) {
        s.push_back(digs[p[i] >> 4]);
        s.push_back(digs[p[i] & 0xF]);
    }
    return s;
}

std::vector<uint8_t> GdbServer::hex_decode_bytes(const std::string& s) {
    std::vector<uint8_t> out;
    auto nibble = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    for (size_t i = 0; i + 1 < s.size(); i += 2) {
        int hi = nibble(s[i]), lo = nibble(s[i+1]);
        if (hi < 0 || lo < 0) break;
        out.push_back((uint8_t)((hi << 4) | lo));
    }
    return out;
}

std::string GdbServer::be32_hex(uint32_t v) {
    uint8_t b[4] = {
        (uint8_t)((v >> 24) & 0xFF),
        (uint8_t)((v >> 16) & 0xFF),
        (uint8_t)((v >>  8) & 0xFF),
        (uint8_t)( v        & 0xFF)
    };
    return hex_encode_bytes(b, 4);
}

std::string GdbServer::be16_hex(uint16_t v) {
    uint8_t b[2] = { (uint8_t)((v >> 8) & 0xFF), (uint8_t)(v & 0xFF) };
    return hex_encode_bytes(b, 2);
}

// ---- Packet handlers ----

void GdbServer::handle_packet(Vm68k_top* top, const std::string& pkt) {
    if (pkt.empty()) { send_empty(); return; }
    switch (pkt[0]) {
        case '?':  send_signal(5);  break;  // SIGTRAP at startup
        case 'H':  send_ok();       break;  // thread select — single thread
        case 'g':  handle_g(top);   break;
        case 'G':  handle_G(top, pkt);  break;
        case 'p':  handle_p(top, pkt);  break;
        case 'P':  handle_P(top, pkt);  break;
        case 'm':  handle_m(top, pkt);  break;
        case 'M':  handle_M(top, pkt);  break;
        case 'c':  halted_ = false; step_remaining_ = 0; break;  // no reply until next stop
        case 's':  halted_ = false; step_remaining_ = 1; break;
        case 'Z':  handle_Z(pkt);   break;
        case 'z':  handle_z(pkt);   break;
        case 'q':  handle_q(top, pkt); break;
        case 'v':  handle_v(top, pkt); break;
        case 'D':  send_ok(); halted_ = false; enabled_ = false; break;
        case 'k':  exit(0);
        default:   send_empty(); break;
    }
}

void GdbServer::handle_q(Vm68k_top* /*top*/, const std::string& pkt) {
    if (pkt.rfind("qSupported", 0) == 0) {
        send_packet("PacketSize=1000;swbreak+;hwbreak+");
        return;
    }
    if (pkt == "qC") { send_packet("QC1"); return; }
    if (pkt.rfind("qfThreadInfo", 0) == 0) { send_packet("m1"); return; }
    if (pkt.rfind("qsThreadInfo", 0) == 0) { send_packet("l"); return; }
    if (pkt.rfind("qAttached", 0) == 0) { send_packet("1"); return; }
    if (pkt.rfind("qSymbol", 0) == 0) { send_ok(); return; }
    if (pkt.rfind("qTStatus", 0) == 0) { send_packet("T0"); return; }
    send_empty();
}

void GdbServer::handle_v(Vm68k_top* /*top*/, const std::string& pkt) {
    if (pkt.rfind("vMustReplyEmpty", 0) == 0) { send_empty(); return; }
    if (pkt.rfind("vCont?", 0) == 0) { send_packet("vCont;c;s"); return; }
    if (pkt.rfind("vCont;c", 0) == 0) { halted_ = false; step_remaining_ = 0; return; }
    if (pkt.rfind("vCont;s", 0) == 0) { halted_ = false; step_remaining_ = 1; return; }
    send_empty();
}

void GdbServer::handle_g(Vm68k_top* top) {
    // GDB m68k expects 18 × 4-byte regs: D0..D7, A0..A7, PS, PC.
    std::string s;
    s.reserve(18 * 8);
    for (int i = 0; i <= 15; i++) s += be32_hex(read_reg(top, i));
    s += be32_hex(read_reg(top, 16));  // PS
    s += be32_hex(read_reg(top, 17));  // PC
    send_packet(s);
}

void GdbServer::handle_G(Vm68k_top* /*top*/, const std::string& /*pkt*/) {
    // No back-door writes yet.
    send_ok();
}

void GdbServer::handle_p(Vm68k_top* top, const std::string& pkt) {
    int idx = (int)std::strtol(pkt.c_str() + 1, nullptr, 16);
    if (idx < 0 || idx > 17) { send_empty(); return; }
    send_packet(be32_hex(read_reg(top, idx)));
}

void GdbServer::handle_P(Vm68k_top* /*top*/, const std::string& /*pkt*/) {
    send_ok();
}

void GdbServer::handle_m(Vm68k_top* top, const std::string& pkt) {
    // "m addr,length"
    char* end = nullptr;
    uint32_t addr = (uint32_t)std::strtoul(pkt.c_str() + 1, &end, 16);
    if (!end || *end != ',') { send_packet("E01"); return; }
    uint32_t len = (uint32_t)std::strtoul(end + 1, nullptr, 16);
    if (len == 0 || len > 4096) { send_packet("E02"); return; }
    std::vector<uint8_t> bytes(len);
    for (uint32_t i = 0; i < len; i++) {
        uint32_t a = addr + i;
        uint32_t w = mem_peek_word(top, a & ~3u);
        int shift = 24 - 8 * (a & 3);
        bytes[i] = (uint8_t)((w >> shift) & 0xFF);
    }
    send_packet(hex_encode_bytes(bytes.data(), bytes.size()));
}

void GdbServer::handle_M(Vm68k_top* top, const std::string& pkt) {
    // "M addr,length:hexdata"
    char* end = nullptr;
    uint32_t addr = (uint32_t)std::strtoul(pkt.c_str() + 1, &end, 16);
    if (!end || *end != ',') { send_packet("E01"); return; }
    uint32_t len = (uint32_t)std::strtoul(end + 1, &end, 16);
    if (!end || *end != ':') { send_packet("E02"); return; }
    auto bytes = hex_decode_bytes(end + 1);
    if (bytes.size() < len) { send_packet("E03"); return; }
    // Memory write: longword-at-a-time via mem_poke.  For partial-word
    // writes we read-modify-write.
    for (uint32_t i = 0; i < len; ) {
        uint32_t a = addr + i;
        uint32_t off = a & 3;
        if (off == 0 && (len - i) >= 4) {
            uint32_t w = ((uint32_t)bytes[i]   << 24) |
                         ((uint32_t)bytes[i+1] << 16) |
                         ((uint32_t)bytes[i+2] <<  8) |
                          (uint32_t)bytes[i+3];
            mem_poke_word(top, a, w);
            i += 4;
        } else {
            uint32_t aligned = a & ~3u;
            uint32_t w = mem_peek_word(top, aligned);
            int shift = 24 - 8 * off;
            w = (w & ~(0xFFu << shift)) | ((uint32_t)bytes[i] << shift);
            mem_poke_word(top, aligned, w);
            i += 1;
        }
    }
    send_ok();
}

void GdbServer::handle_Z(const std::string& pkt) {
    // "Z type,addr,kind"
    if (pkt.size() < 6) { send_empty(); return; }
    int type = pkt[1] - '0';
    char* end = nullptr;
    uint32_t addr = (uint32_t)std::strtoul(pkt.c_str() + 3, &end, 16);
    if (!end || *end != ',') { send_packet("E01"); return; }
    uint32_t kind = (uint32_t)std::strtoul(end + 1, nullptr, 16);
    if (type == 0 || type == 1) {
        bps_.push_back(addr);
        send_ok();
        return;
    }
    if (type == 2 || type == 3 || type == 4) {
        wps_.push_back({addr, kind ? kind : 4u, type});
        send_ok();
        return;
    }
    send_empty();
}

void GdbServer::handle_z(const std::string& pkt) {
    if (pkt.size() < 6) { send_empty(); return; }
    int type = pkt[1] - '0';
    char* end = nullptr;
    uint32_t addr = (uint32_t)std::strtoul(pkt.c_str() + 3, &end, 16);
    if (type == 0 || type == 1) {
        for (auto it = bps_.begin(); it != bps_.end(); ++it) {
            if (*it == addr) { bps_.erase(it); break; }
        }
        send_ok();
        return;
    }
    if (type >= 2 && type <= 4) {
        for (auto it = wps_.begin(); it != wps_.end(); ++it) {
            if (it->addr == addr && it->kind == type) {
                wps_.erase(it); break;
            }
        }
        send_ok();
        return;
    }
    send_empty();
}

// ---- Main tick ----

bool GdbServer::tick(Vm68k_top* top, uint64_t /*cycle*/) {
    if (!enabled_) return true;
    if (client_fd_ < 0) return true;

    // On first call after connect, send the initial stop notification
    // so GDB knows we're halted at the entry point.
    if (!first_stop_sent_) {
        send_signal(5);  // SIGTRAP — "stopped at start"
        first_stop_sent_ = true;
    }

    // Drain pending packets.
    std::string pkt;
    while (read_packet(pkt)) {
        handle_packet(top, pkt);
    }
    return !halted_;
}

void GdbServer::notify_step(Vm68k_top* top, uint64_t /*cycle*/) {
    if (!enabled_ || halted_) return;
    // Check breakpoints against current PC.
    const uint32_t* pc_arr = reinterpret_cast<const uint32_t*>(&top->core_pc_flat);
    uint32_t pc = pc_arr[0] & 0x00FFFFFFu;
    for (uint32_t bp : bps_) {
        if ((bp & 0x00FFFFFFu) == pc) {
            halted_ = true;
            send_signal(5);  // SIGTRAP
            return;
        }
    }
    // Single-step?
    if (step_remaining_ > 0) {
        step_remaining_--;
        if (step_remaining_ == 0) {
            halted_ = true;
            send_signal(5);
        }
    }
}
