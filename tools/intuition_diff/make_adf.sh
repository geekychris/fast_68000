#!/bin/bash
# make_adf.sh — inject a built Amiga executable into a copy of wb13.adf
# and rewrite S/Startup-Sequence so the boot lands directly in the test
# binary.  Output goes to kickstart/<stem>.adf.
#
# Usage:  tools/intuition_diff/make_adf.sh [stem]    (default stem: screen_open_test)
#
# The startup-sequence is intentionally minimal — no SetPatch, no cd c:,
# no extra echos.  We want OpenScreen to run before any of WB1.3's
# preferences/Workbench code touches the chipset.
set -eu
STEM="${1:-screen_open_test}"
DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$DIR/../.." && pwd)"
EXE="$DIR/$STEM"
WB="$ROOT/kickstart/wb13.adf"
OUT="$ROOT/kickstart/${STEM}.adf"

if [ ! -f "$EXE" ]; then
    echo "missing exe: $EXE  (run build.sh first)"
    exit 1
fi
if [ ! -f "$WB" ]; then
    echo "missing wb13.adf at $WB"
    exit 1
fi
if ! command -v xdftool >/dev/null 2>&1; then
    echo "amitools not installed - pip install amitools"
    exit 1
fi

cp "$WB" "$OUT"

# Minimal startup: just run the test binary.  endcli stops the CLI so
# the binary doesn't keep getting interrupted by Read() polls.
STARTUP=$(mktemp)
printf '%s\n%s\n%s\n' \
    'echo "*N>>> SCREEN-OPEN-TEST <<<*N"' \
    "$STEM" \
    'endcli >NIL:' > "$STARTUP"

xdftool "$OUT" delete S/Startup-Sequence >/dev/null 2>&1 || true
xdftool "$OUT" write "$STARTUP" S/Startup-Sequence 2>&1 | tail -1
xdftool "$OUT" delete "C/$STEM" >/dev/null 2>&1 || true
xdftool "$OUT" write "$EXE" "C/$STEM" 2>&1 | tail -1
xdftool "$OUT" info 2>&1 | grep -E "free|used" || true
echo "Built $OUT"
rm -f "$STARTUP"
