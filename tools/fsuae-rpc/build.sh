#!/usr/bin/env bash
# Build FS-UAE v3.2.35 from source with the claude_rpc HTTP/JSON-RPC patch.
#
# Idempotent: re-running will reuse an existing clone, reset it cleanly, and
# rebuild.  Pass FSUAE_SRC=/path to override the source tree location.
#
# Outputs the built binary at $FSUAE_SRC/fs-uae and prints its path on success.

set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
FSUAE_SRC="${FSUAE_SRC:-/tmp/fsuae-src}"
FSUAE_TAG="${FSUAE_TAG:-v3.2.35}"
FSUAE_URL="${FSUAE_URL:-https://github.com/FrodeSolheim/fs-uae.git}"
JOBS="${JOBS:-$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)}"

REQUIRED_BREW_PKGS=(
    autoconf automake libtool pkg-config gettext
    glib libpng libmpeg2 openal-soft sdl2 zlib
)

step()  { printf '\n\033[1;36m==> %s\033[0m\n' "$*"; }
info()  { printf '    %s\n' "$*"; }
fail()  { printf '\033[1;31m!! %s\033[0m\n' "$*" >&2; exit 1; }

# -- 1. Check macOS + brew --
[[ "$(uname -s)" == "Darwin" ]] || fail "macOS-only (uname=$(uname -s))"
command -v brew >/dev/null || fail "Homebrew not installed.  See https://brew.sh"

# -- 2. Install brew deps (missing ones only) --
step "Checking brew dependencies"
MISSING=()
for pkg in "${REQUIRED_BREW_PKGS[@]}"; do
    if brew list --formula "$pkg" >/dev/null 2>&1; then
        info "ok: $pkg"
    else
        MISSING+=("$pkg")
    fi
done
if (( ${#MISSING[@]} > 0 )); then
    info "Installing: ${MISSING[*]}"
    brew install "${MISSING[@]}"
fi

# openal-soft is keg-only, expose its pkg-config
export PKG_CONFIG_PATH="$(brew --prefix openal-soft)/lib/pkgconfig:${PKG_CONFIG_PATH:-}"

# -- 3. Clone or update source --
step "Fetching FS-UAE $FSUAE_TAG into $FSUAE_SRC"
if [[ ! -d "$FSUAE_SRC/.git" ]]; then
    git clone "$FSUAE_URL" "$FSUAE_SRC"
else
    info "Reusing existing clone"
    git -C "$FSUAE_SRC" fetch --tags origin
fi

# Reset any prior patch attempts to a clean state at the target tag
git -C "$FSUAE_SRC" reset --hard >/dev/null
git -C "$FSUAE_SRC" clean -fdx --quiet -e claude_rpc.cpp -e src/claude_rpc.cpp
# (clean -e excludes our drop-in from being wiped if re-run)

# Disable sparse-checkout if active (the FrodeSolheim mirror sometimes
# defaults to sparse on first clone)
if git -C "$FSUAE_SRC" sparse-checkout list >/dev/null 2>&1; then
    git -C "$FSUAE_SRC" sparse-checkout disable >/dev/null 2>&1 || true
fi

git -C "$FSUAE_SRC" checkout "$FSUAE_TAG" -- . 2>/dev/null || git -C "$FSUAE_SRC" checkout "$FSUAE_TAG"

# -- 4. Drop in claude_rpc.cpp + apply patch --
step "Applying claude_rpc patch"
cp -f "$HERE/claude_rpc.cpp" "$FSUAE_SRC/src/claude_rpc.cpp"
info "Copied claude_rpc.cpp -> src/"

# `git apply --check` first so we fail fast on stale patches
if git -C "$FSUAE_SRC" apply --check "$HERE/0001-claude-rpc-hook.patch" 2>/dev/null; then
    git -C "$FSUAE_SRC" apply "$HERE/0001-claude-rpc-hook.patch"
    info "Applied 0001-claude-rpc-hook.patch"
else
    fail "Patch does not apply cleanly to $FSUAE_TAG.  Regenerate the patch."
fi

# -- 5. Configure + build --
step "Bootstrap"
( cd "$FSUAE_SRC" && ./bootstrap )

step "Configure"
( cd "$FSUAE_SRC" && ./configure )

step "Build (make -j$JOBS)"
( cd "$FSUAE_SRC" && make -j"$JOBS" )

# -- 6. Smoke test --
step "Smoke-testing built binary"
"$FSUAE_SRC/fs-uae" --version | head -1

step "Done"
echo "Binary: $FSUAE_SRC/fs-uae"
echo "Run with:   FSUAE_RPC_PORT=8765 $FSUAE_SRC/fs-uae <config>"
