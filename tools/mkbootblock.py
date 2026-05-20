#!/usr/bin/env python3
"""Build a minimal Amiga DOS bootblock (1024 bytes) the sim can use to
prove Kickstart actually reached and called the disk's boot code.

Amiga DOS 1.x bootblock layout (first 1024 bytes of an ADF):
    +0    'DOS' + flag byte ($00 = OFS, $01 = FFS, ...)
    +4    32-bit boot-checksum (additive with end-around carry; the
          whole 1024-byte block must sum to $FFFFFFFF)
    +8    32-bit rootblock pointer (880 for DD floppies)
    +12   boot code -- Kickstart's strap.lib calls JSR (A0) where A0 is
          this offset, in user mode, with A1 pointing at the trackdisk
          IORequest and A6 = ExecBase.

This generator emits a bootblock whose code does:
    move.l  #$CAFE_BABE, $00050000   ; magic marker our sim can spot
    moveq   #-1, D0                  ; boot failure (Kickstart won't
                                     ; then try to use a fake dos.lib)
    rts                              ; return to Kickstart

The actual ADF is 880 KB (160 tracks * 11 sectors * 512 bytes), but we
zero-fill everything past the bootblock since trackdisk only DMAs and
decodes one track at a time.

Usage:
    mkbootblock.py <out.adf>
"""
from __future__ import annotations

import argparse
import sys


BOOTBLOCK_BYTES = 1024
TRACK_BYTES = 11 * 512
TRACKS_PER_DISK = 160
DISK_BYTES = TRACK_BYTES * TRACKS_PER_DISK  # 880 KB


def boot_checksum(block: bytes) -> int:
    """Compute the Amiga DOS bootblock checksum.

    The whole 1024-byte block, summed as 32-bit big-endian longwords
    with end-around carry, must equal $FFFFFFFF.  We compute the sum
    with the checksum field (bytes 4..7) zeroed, then return
    ~sum (mod 2^32) so callers can write it back into bytes 4..7.
    """
    assert len(block) == BOOTBLOCK_BYTES
    s = 0
    for i in range(0, BOOTBLOCK_BYTES, 4):
        v = int.from_bytes(block[i : i + 4], "big")
        s += v
        if s > 0xFFFFFFFF:
            s = (s + 1) & 0xFFFFFFFF  # end-around carry
    return (~s) & 0xFFFFFFFF


def build_bootblock() -> bytes:
    """Assemble the 1024-byte bootblock by hand-encoding the 68k ops.

    Code (offset +12):
        $23FC DEAD BEEF 0005 0000   move.l #$CAFEBABE, $00050000
        $70FF                       moveq #-1, D0
        $4E75                       rts
    (24 bytes including imm + absL operand; rest padded to 1024.)
    """
    bb = bytearray(BOOTBLOCK_BYTES)

    # Magic + flag.  'DOS' + $00 = OFS-compatible bootblock.
    bb[0:4] = b"DOS\x00"

    # Checksum placeholder (filled in after).
    bb[4:8] = b"\x00\x00\x00\x00"

    # Root block pointer (880 for DD).
    bb[8:12] = (880).to_bytes(4, "big")

    # Code at offset 12.
    code = bytes.fromhex(
        # move.l #$CAFEBABE, $00050000
        "23FC" "CAFEBABE" "00050000"
        # moveq #-1, D0
        "70FF"
        # rts
        "4E75"
    )
    bb[12 : 12 + len(code)] = code

    # Compute and install the checksum.
    chk = boot_checksum(bytes(bb))
    bb[4:8] = chk.to_bytes(4, "big")

    # Verify.
    s = 0
    for i in range(0, BOOTBLOCK_BYTES, 4):
        v = int.from_bytes(bb[i : i + 4], "big")
        s += v
        if s > 0xFFFFFFFF:
            s = (s + 1) & 0xFFFFFFFF
    assert s == 0xFFFFFFFF, f"bootblock checksum failed: {s:08x}"

    return bytes(bb)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("outfile", help="path to write the ADF image")
    ap.add_argument(
        "--full",
        action="store_true",
        help="emit a full 880 KB ADF (default: just one 5632-byte track)",
    )
    args = ap.parse_args()

    bb = build_bootblock()
    # The bootblock occupies sectors 0+1 (= first 1024 B of track 0).
    # Pad sector data 2..10 of track 0 with zero.  trackdisk decodes
    # whole tracks so we need 11 sectors worth in the ADF (even if
    # the extra ones are blank).
    track0 = bb + b"\x00" * (TRACK_BYTES - BOOTBLOCK_BYTES)

    if args.full:
        # Pad to 880 KB with zeros for other tracks.
        adf = track0 + b"\x00" * (DISK_BYTES - TRACK_BYTES)
    else:
        # Just track 0 is enough for boot.
        adf = track0

    with open(args.outfile, "wb") as f:
        f.write(adf)

    print(
        f"wrote {len(adf)} bytes to {args.outfile} "
        f"(bootblock checksum installed; {len(adf) // TRACK_BYTES} track(s))"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
