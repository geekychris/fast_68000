#!/bin/bash
# build.sh — assemble + link tools/intuition_diff/<name>.s into an Amiga
# hunkexe ready for xdftool injection.  Defaults to screen_open_test;
# accepts a stem as $1 so we can add open_window_test, refresh_test, etc.
# later and share the toolchain wiring.
set -eu
STEM="${1:-screen_open_test}"
DIR="$(cd "$(dirname "$0")" && pwd)"
AS=/opt/amiga/bin/m68k-amigaos-as
LD=/opt/amiga/bin/m68k-amigaos-ld
if [ ! -x "$AS" ] || [ ! -x "$LD" ]; then
    echo "amiga-gcc toolchain missing at /opt/amiga/bin/"
    echo "(needed: m68k-amigaos-as + m68k-amigaos-ld; install via bebbo/amiga-gcc)"
    exit 1
fi
"$AS" -o "$DIR/$STEM.o" "$DIR/$STEM.s"
"$LD" -e _start "$DIR/$STEM.o" -o "$DIR/$STEM"
chmod +x "$DIR/$STEM"
echo "Built $DIR/$STEM ($(wc -c < "$DIR/$STEM") bytes, $(file -b "$DIR/$STEM"))"
