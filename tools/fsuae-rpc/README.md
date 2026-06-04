# FS-UAE Claude RPC patch

External HTTP/JSON-RPC control surface for [FS-UAE](https://github.com/FrodeSolheim/fs-uae).
Lets scripts (or Claude via an MCP server) pause/resume the emulator, read
CPU registers and memory, and save state — without GUI interaction.

Built for the K1.3/WB1.3 boot bisection workflow in this repo
(`docs/WB13_DEBUG_JOURNAL.md` §13): we needed to dump chip RAM `$C0-$FF`
at programmable cycle offsets and could not script the GUI debugger.

## Files

- `claude_rpc.cpp` — drop-in source file (~325 lines, no external deps)
- `0001-claude-rpc-hook.patch` — Makefile.am + main.cpp wiring

## Apply

```sh
git clone https://github.com/FrodeSolheim/fs-uae /tmp/fsuae-src
cd /tmp/fsuae-src
git checkout f362278           # base commit verified against
cp /path/to/68000/tools/fsuae-rpc/claude_rpc.cpp .
git apply /path/to/68000/tools/fsuae-rpc/0001-claude-rpc-hook.patch
./bootstrap                    # regenerates Makefile from Makefile.am
./configure
make -j
```

POSIX-only in v1.  Windows builds compile to a no-op stub.

## Run

```sh
FSUAE_RPC_PORT=8765 fs-uae ~/Documents/FS-UAE/Configurations/WB13-K13-A500-Reference.fs-uae
```

The RPC is **off by default**.  Setting `FSUAE_RPC_PORT` enables it; the
server binds `127.0.0.1` only (no remote access).  Stderr will print:

```
[claude-rpc] listening on http://127.0.0.1:8765/
```

## v1 endpoints

| Method | Path                                | Behaviour                          |
| ------ | ----------------------------------- | ---------------------------------- |
| GET    | `/v1/ping`                          | Smoke test                         |
| GET    | `/v1/cpu`                           | PC, SR, D0-D7, A0-A7, USP, ISP     |
| GET    | `/v1/mem?addr=HEX&len=N`            | N bytes from addr (hex string)     |
| POST   | `/v1/pause`                         | Calls `activate_debugger()`        |
| POST   | `/v1/resume`                        | Calls `deactivate_debugger()`      |
| POST   | `/v1/state/save?path=ABS_PATH`      | Saves state to absolute path       |

Responses are JSON: `{"ok":true,...}` or `{"ok":false,"err":"msg"}`.

## Example session

```sh
# Smoke test
curl -s http://127.0.0.1:8765/v1/ping
# {"ok":true,"service":"fs-uae claude-rpc v1"}

# Pause + dump chip RAM $C0-$FF (16 vectors)
curl -sX POST http://127.0.0.1:8765/v1/pause
curl -s "http://127.0.0.1:8765/v1/mem?addr=0xC0&len=64"

# Dump CPU regs
curl -s http://127.0.0.1:8765/v1/cpu

# Snapshot to disk
curl -sX POST "http://127.0.0.1:8765/v1/state/save?path=/tmp/k13-late.uss"

# Resume
curl -sX POST http://127.0.0.1:8765/v1/resume
```

## Bisection workflow

The original motivation — find where K1.3 populates chip RAM `$C0-$FC`
with IRQ-server dispatcher pointers (the install path our Verilator
build never runs).  Sketch:

```sh
boot fs-uae with rpc enabled
for cycles in 1M 2M 5M 10M 20M 50M 100M; do
  sleep_emu_to $cycles    # external wallclock approximation, or
                          # use save-state and reload patterns
  curl POST /v1/pause
  curl GET  /v1/mem?addr=0xC0&len=64 > snap-$cycles.txt
  curl POST /v1/resume
done
```

The first non-zero snapshot brackets the install routine's runtime
window; `curl /v1/cpu` at that pause gives the PC range for ROM
disassembly.

## Safety notes

- Memory + register reads while running may race with the emulation
  thread.  Always `/v1/pause` first for stable values.
- `/v1/state/save` requires an absolute path the FS-UAE process can write to.
- The server runs in a detached pthread; there is no clean-shutdown
  hook on FS-UAE exit (the OS reaps the socket).

## Deferred (v2 ideas)

- WebSocket event stream (notify on breakpoint hit)
- PC breakpoints with auto-pause
- Single-step
- Memory write
- State load
- Windows port
