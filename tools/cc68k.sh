#!/usr/bin/env bash
# cc68k.sh — compile C/asm sources for fast_68000 inside the cc68k Docker
# image and emit a $readmemh-compatible program.hex.
#
# Usage:
#   tools/cc68k.sh -o out/myprog.hex demos/c/foo.c [more.c ...]
#
# Flags:
#   -o FILE       Output hex path (required).
#   --mem-words N $readmemh array length (default 65536 = 256 KB).
#   --base ADDR   Text base (default 0x400).
#   --image NAME  Docker image tag (default fast68k-cc:latest).
#   --no-build    Skip building the image (assumes it exists).
#   --keep        Keep the temporary build dir on success.
#
# Picks up runtime/crt0.S and runtime/m68k.ld automatically.
# All file paths must be inside the repo root (passed verbatim to docker -v).

set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
img="fast68k-cc:latest"
mem_words=65536
base=0x400
out=""
no_build=0
keep=0
srcs=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        -o)            out="$2"; shift 2;;
        --mem-words)   mem_words="$2"; shift 2;;
        --base)        base="$2"; shift 2;;
        --image)       img="$2"; shift 2;;
        --no-build)    no_build=1; shift;;
        --keep)        keep=1; shift;;
        -h|--help)     sed -n '2,18p' "${BASH_SOURCE[0]}"; exit 0;;
        --)            shift; while [[ $# -gt 0 ]]; do srcs+=("$1"); shift; done;;
        -*)            echo "unknown flag: $1" >&2; exit 2;;
        *)             srcs+=("$1"); shift;;
    esac
done

if [[ -z "$out" || ${#srcs[@]} -eq 0 ]]; then
    sed -n '2,18p' "${BASH_SOURCE[0]}"
    exit 2
fi

if [[ "$no_build" -eq 0 ]]; then
    docker build -q -t "$img" "$here/tools/cc68k" >/dev/null
fi

# Stage sources into a build dir under build/cc68k/ so paths inside the
# container are simple and predictable.
work="$here/build/cc68k/$(basename "$out" .hex).work"
rm -rf "$work"
mkdir -p "$work"
cp "$here/runtime/crt0.S" "$here/tools/cc68k/m68k.ld" "$work/"
cp "$here/tools/cc68k/bin2hex.py" "$work/"

# Copy each source into work/ flattened.  Header dependencies get the same
# treatment: anything from runtime/ is mirrored into work/.
mkdir -p "$work/src"
for src in "${srcs[@]}"; do
    cp "$src" "$work/src/"
done
cp -r "$here/runtime/." "$work/inc/"

# Build command.  CFLAGS keep us on the 68000 ISA and out of libc/libgcc.
docker run --rm \
    -v "$work":/work \
    -w /work \
    "$img" sh -c '
set -e
CFLAGS="-m68000 -Os -fomit-frame-pointer -ffreestanding -fno-builtin \
        -fno-common -fno-stack-protector -nostdlib -nostartfiles \
        -Wall -Wno-builtin-declaration-mismatch -I inc"
LDFLAGS="-T m68k.ld -nostdlib -nostartfiles -Wl,--build-id=none -static"

cobjs=""
for c in src/*.c src/*.S; do
    [ -e "$c" ] || continue
    base=$(basename "$c")
    obj=${base%.*}.o
    "$CROSS-gcc" $CFLAGS -c "$c" -o "$obj"
    cobjs="$cobjs $obj"
done

# crt0 goes first so _start lands at .text base.
"$CROSS-gcc" $CFLAGS -c crt0.S -o crt0.o

"$CROSS-gcc" $CFLAGS crt0.o $cobjs $LDFLAGS -lgcc -o prog.elf
"$CROSS-objcopy" -O binary prog.elf prog.bin
python3 bin2hex.py prog.bin prog.hex --base '"$base"' --mem-words '"$mem_words"'
'

cp "$work/prog.hex" "$out"
if [[ "$keep" -eq 0 ]]; then
    rm -rf "$work"
fi
echo "cc68k: wrote $out"
