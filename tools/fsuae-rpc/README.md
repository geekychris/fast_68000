# FS-UAE RPC patch — moved

This directory is a **stale v1 snapshot** of the FS-UAE control-surface
patch.  Active development has moved to a dedicated project:

> **<https://github.com/geekychris/fsuae_remote_patch>**
> (working tree at `/Users/chris/code/claude_world/fsuae_remote_patch`)

The dedicated project is many versions ahead:

- WebSocket event stream + embedded web UI
- PC breakpoints + watchpoints (with value-match + must-change filters)
- Zero-cost trace breakpoints (`trace=1&out=PATH`) for high-volume
  divergence-bisection
- Single-step + memory write + state load
- In-process GDB Remote Serial Protocol stub
- `FSUAE_NO_UAEFS=1` env gate (suppress KS12_BOOT_HACK for
  apples-to-apples chipset comparisons)
- Symbol-aware disassembly with embedded `.fd` annotations
- Windows compile-shim
- Optional MCP server (`tools/mcp_fsuae.py`)

## Use the dedicated project

```sh
git clone git@github.com:geekychris/fsuae_remote_patch.git
cd fsuae_remote_patch
./build.sh
./run.sh ~/Documents/FS-UAE/Configurations/MyAmiga.fs-uae
```

## What's still here

The files in this directory are a frozen v1 snapshot kept for
historical reference (the original commit message references this
path).  Don't add to them — work in the dedicated repo instead.

| File | Status |
|---|---|
| `claude_rpc.cpp` | v1 — superseded by `fsuae_remote_patch/fsuae_rpc.cpp` |
| `0001-claude-rpc-hook.patch` | v1 — superseded by `fsuae_remote_patch/patches/0001-fsuae-rpc-hook.patch` |
| `build.sh` | v1 — superseded by `fsuae_remote_patch/build.sh` |
