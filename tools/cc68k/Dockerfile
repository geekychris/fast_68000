# m68k bare-metal cross-compiler image.
#
# Debian's gcc-m68k-linux-gnu produces ELF binaries targeting Linux/m68k,
# but the compiler itself is fine for bare-metal use as long as we pass
# -nostdlib -nostartfiles -fno-builtin (no libc, no libgcc), provide our
# own crt0, and objcopy -O binary the result so the ELF wrapper is gone.
#
# Pinned to Debian bookworm-slim for reproducibility.

FROM ubuntu:24.04

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        gcc-m68k-linux-gnu \
        binutils-m68k-linux-gnu \
        python3 \
        make \
 && rm -rf /var/lib/apt/lists/*

ENV CROSS=m68k-linux-gnu
WORKDIR /work
