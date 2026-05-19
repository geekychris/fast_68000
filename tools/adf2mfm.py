#!/usr/bin/env python3
"""Convert an Amiga ADF disk image into MFM-encoded track data.

This is the minimig-style encode path: the host (= this script) takes
the raw 512-byte sectors from an ADF and emits the full MFM bit stream
the way real floppy hardware would after sector encoding (sync words,
sector headers, data area, checksums).  Paula's side then treats the
result as opaque MFM bytes and DMAs them to memory; trackdisk.device
on the running Amiga decodes them back to the original raw bytes via
its existing MFM-decode routine.

ADF layout:
    11 sectors per track (DD)
    2 surfaces (sides) * 80 tracks/side = 160 tracks
    512 bytes per sector
    Total = 901,120 bytes = 880 KB

MFM-encoded Amiga sector (per sector, after sync):
    2x SYNC ($AAAA $4489 $4489) -- 6 bytes after the first sync gap
    header info long: format / track / sector / sectors-to-end-of-write
    16 long  sector label (zero in OS sectors)
    1 long   header checksum
    1 long   data checksum
    512 bytes (=128 longs) data area
The encoded sector body is "interleaved odd/even" -- all the odd bits
of each long come first, then all the even bits.  The MFM encode
itself doubles the bit count and inserts clock bits so consecutive
data bits don't run together.

This script emits one $readmemh-style 32-bit-per-line hex file that the
testbench loads into a host-side MFM buffer.  An RTL-side FIFO consumes
it byte-by-byte when DSKLEN DMA starts, feeding raw MFM bytes to memory.

Usage:
    adf2mfm.py <in.adf> <out.hex> [--mem-words N] [--track N]

If --track is supplied, only that single track is encoded (useful for
tests).  Otherwise the full disk image is encoded.

Status: stub.  Currently emits a placeholder that's good enough to
sanity-check the testbench plumbing; the actual MFM encoding will land
when wiring the FIFO into the bus.  Not yet used by any test target.
"""

import argparse
import struct
import sys

# MFM clock: insert a clock bit between data bits if the surrounding data
# bits are both zero.  Each byte expands to 16 MFM bits = 2 bytes.
def mfm_encode_byte(b: int, last_bit: int) -> tuple[int, int]:
    """Encode one byte as 16 MFM bits.  Returns (16-bit value, new last bit)."""
    out = 0
    for i in range(7, -1, -1):
        data_bit = (b >> i) & 1
        clock = 1 if (last_bit == 0 and data_bit == 0) else 0
        out = (out << 2) | (clock << 1) | data_bit
        last_bit = data_bit
    return out & 0xFFFF, last_bit


def mfm_encode_bytes(data: bytes) -> bytes:
    """MFM-encode a sequence of bytes.  Returns 2x-length encoded bytes."""
    out = bytearray()
    last_bit = 0
    for b in data:
        word, last_bit = mfm_encode_byte(b, last_bit)
        out.append((word >> 8) & 0xFF)
        out.append(word & 0xFF)
    return bytes(out)


SECTORS_PER_TRACK = 11
BYTES_PER_SECTOR = 512


def encode_sector(track: int, sector: int, data: bytes) -> bytes:
    """Encode a single 512-byte sector into MFM bytes following the Amiga
    floppy-format convention.  Sector header:

        info = (0xFF << 24) | (track << 16) | (sector << 8) | (11 - sector)
        16-long sector label = zeroes
        header checksum   = XOR of odd-bit-stream-then-even-bit-stream halves
        data checksum     = XOR over the encoded data area

    The header info / label area is "interleaved odd/even": all the odd
    bits of every long come first, then all the even bits.  Same for the
    512-byte data area.  After this scrambling each half is MFM-encoded.

    This is a stub returning a zero-filled placeholder for now; landing
    the real encoder is part of the disk-FIFO integration TODO.
    """
    assert len(data) == BYTES_PER_SECTOR
    # Placeholder: 540 bytes raw (= 1080 bytes MFM) per Amiga sector.
    return b"\x00" * 1080


def encode_track(track: int, sectors: list[bytes]) -> bytes:
    out = bytearray()
    for s_idx, sec in enumerate(sectors):
        out.extend(encode_sector(track, s_idx, sec))
    return bytes(out)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("infile")
    ap.add_argument("outfile")
    ap.add_argument("--mem-words", type=lambda s: int(s, 0), default=65536)
    ap.add_argument("--track", type=int, default=None,
                    help="encode only this one track (0..159); default is all")
    args = ap.parse_args()

    with open(args.infile, "rb") as f:
        data = f.read()

    track_bytes = SECTORS_PER_TRACK * BYTES_PER_SECTOR
    n_tracks = len(data) // track_bytes
    if args.track is not None:
        tracks = [args.track]
    else:
        tracks = list(range(n_tracks))

    out = bytearray()
    for t in tracks:
        sectors = [data[t * track_bytes + s * BYTES_PER_SECTOR :
                        t * track_bytes + (s + 1) * BYTES_PER_SECTOR]
                   for s in range(SECTORS_PER_TRACK)]
        out.extend(encode_track(t, sectors))

    # Pad and pack into 32-bit big-endian words.
    if len(out) % 4 != 0:
        out += b"\x00" * (4 - (len(out) % 4))
    n_words = len(out) // 4
    if n_words > args.mem_words:
        print(f"encoded MFM is {len(out)} bytes ({n_words} words) but "
              f"--mem-words is {args.mem_words}", file=sys.stderr)
        return 1

    mem = [0] * args.mem_words
    for i in range(n_words):
        (mem[i],) = struct.unpack(">I", out[i * 4 : i * 4 + 4])

    with open(args.outfile, "w") as f:
        f.write("\n".join(f"{w:08x}" for w in mem) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
