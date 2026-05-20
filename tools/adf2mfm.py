#!/usr/bin/env python3
"""Convert an Amiga ADF disk image into MFM-encoded track data.

Emits the Amiga-floppy disk byte stream that trackdisk.device's
software MFM decoder consumes after Paula DMAs raw disk bytes to RAM.

ADF layout: 80 cylinders * 2 sides = 160 tracks, 11 sectors/track,
512 B/sector => 901,120 bytes (880 KB).

Per-sector disk layout (raw disk bytes, MSB first):
    2 sync gap bytes ($00 each, MFM-expand to $AAAA)
    2 sync words ($4489 $4489 - 4 bytes; raw on disk, NOT MFM encoded)
    4 bytes  header info  (format / track / sector / sectors-to-end)
    16 bytes sector label (zero for OS sectors)
    4 bytes  header checksum
    4 bytes  data   checksum
    512 bytes sector data

The 4-byte info / 16-byte label / checksums / 512-byte data areas are
all "odd/even half" encoded.  For each 32-bit longword L:
    odd_half  = (L >> 1) & 0x55555555     # bits L[1], L[3], ..., L[31]
                                          # placed at positions 0, 2, ..., 30
    even_half = L & 0x55555555            # bits L[0], L[2], ..., L[30]

These half-density bytes have data at every other bit position; the
remaining (clock) positions are normally filled in by the MFM encoder
following the rule "clock = NOT (prev_data | next_data)".  Our
simulator DMA delivers raw bytes verbatim and Kickstart's trackdisk
extracts data bits at the same positions, so we leave the clock bits
at zero.  A real floppy controller's PLL would refuse to lock onto
this stream, but our simulator has no PLL.

For a longword buffer of N longs, the disk layout writes ALL the odd
halves first (4*N bytes), then ALL the even halves (4*N bytes), for
a total of 8*N disk bytes / 2*N raw bytes.

Header & data checksums are computed AFTER the half-split: the
checksum is the XOR of every 4-byte odd-half and every 4-byte
even-half, then masked with 0x55555555.

Usage:
    adf2mfm.py <in.adf> <out.hex> [--mem-words N] [--track N]
"""

import argparse
import struct
import sys

SECTORS_PER_TRACK = 11
BYTES_PER_SECTOR = 512
TRACKS_PER_DISK = 160
SECTOR_LABEL_BYTES = 16

# Per-sector disk-byte budget:
#   2 gap + 4 sync + 8 (header info) + 32 (label) + 8 (hdr chk)
#   + 8 (data chk) + 1024 (data) = 1086 bytes
SECTOR_DISK_BYTES = 1086

# Pre-sync MFM-encoded gap.  Two raw $00 bytes -> $AA $AA $AA $AA on
# disk (MFM expansion: each 0 data bit gets a 1 clock bit).  We emit
# the pre-expanded form.
GAP_RAW = bytes([0xAA, 0xAA])


def odd_even_halves(buf: bytes) -> tuple[bytes, bytes]:
    """For each 32-bit longword in `buf`, compute its odd and even half.

    Returns (odd_stream, even_stream) where each stream has the same
    length as `buf`.  The disk layout writes odd_stream first then
    even_stream, for a total of 2 * len(buf) disk bytes.
    """
    assert len(buf) % 4 == 0, len(buf)
    odd_stream = bytearray()
    even_stream = bytearray()
    for i in range(0, len(buf), 4):
        lw = int.from_bytes(buf[i : i + 4], "big")
        odd  = (lw >> 1) & 0x55555555
        even = lw & 0x55555555
        odd_stream.extend(odd.to_bytes(4, "big"))
        even_stream.extend(even.to_bytes(4, "big"))
    return bytes(odd_stream), bytes(even_stream)


def amiga_checksum(odd_then_even: bytes) -> int:
    """XOR every 4-byte longword in the (already-split) odd|even stream
    and mask with 0x55555555 -- the Amiga sector checksum convention."""
    assert len(odd_then_even) % 4 == 0, len(odd_then_even)
    chk = 0
    for i in range(0, len(odd_then_even), 4):
        chk ^= int.from_bytes(odd_then_even[i : i + 4], "big")
    return chk & 0x55555555


def encode_sector(track: int, sector: int, data: bytes) -> bytes:
    """Encode one 512-byte Amiga sector into its 1086-byte disk
    representation (excluding inter-sector gap)."""
    assert len(data) == BYTES_PER_SECTOR
    out = bytearray()

    # Pre-sync gap (2 bytes of $AA after MFM expansion).
    out.extend(GAP_RAW)

    # Sync words: two $4489 on disk, as raw bytes.  Trackdisk's PLL
    # locks here.  We emit them verbatim.
    out.extend(b"\x44\x89\x44\x89")

    # Header info longword: $FF (format) / track / sector / sectors-left.
    sectors_to_end = SECTORS_PER_TRACK - sector
    info = (0xFF << 24) | (track << 16) | (sector << 8) | sectors_to_end
    info_raw = info.to_bytes(4, "big")

    # Sector label: zeroes for OS sectors.
    label_raw = b"\x00" * SECTOR_LABEL_BYTES

    # Header = info (4 bytes = 1 long) + label (16 bytes = 4 longs).
    header_raw = info_raw + label_raw
    hdr_odd, hdr_even = odd_even_halves(header_raw)
    out.extend(hdr_odd)
    out.extend(hdr_even)

    # Header checksum = XOR of header odd+even halves, masked.
    hdr_chk = amiga_checksum(hdr_odd + hdr_even)
    hdr_chk_odd, hdr_chk_even = odd_even_halves(hdr_chk.to_bytes(4, "big"))
    out.extend(hdr_chk_odd)
    out.extend(hdr_chk_even)

    # Data interleave & checksum.
    data_odd, data_even = odd_even_halves(data)
    data_chk = amiga_checksum(data_odd + data_even)
    data_chk_odd, data_chk_even = odd_even_halves(data_chk.to_bytes(4, "big"))
    out.extend(data_chk_odd)
    out.extend(data_chk_even)

    # Data area.
    out.extend(data_odd)
    out.extend(data_even)

    assert len(out) == SECTOR_DISK_BYTES, (len(out), SECTOR_DISK_BYTES)
    return bytes(out)


def encode_track(track: int, sectors: list[bytes]) -> bytes:
    assert len(sectors) == SECTORS_PER_TRACK
    out = bytearray()
    for s_idx, sec in enumerate(sectors):
        out.extend(encode_sector(track, s_idx, sec))
    # Inter-track gap: extra $AA bytes so the next track's pre-sync gap
    # has slack.  Keep it small to fit the disk image.
    out.extend(b"\xAA" * 16)
    return bytes(out)


def encode_disk(adf: bytes, tracks_to_emit: list[int]) -> bytes:
    track_bytes = SECTORS_PER_TRACK * BYTES_PER_SECTOR
    out = bytearray()
    for t in tracks_to_emit:
        sectors = [
            adf[t * track_bytes + s * BYTES_PER_SECTOR : t * track_bytes + (s + 1) * BYTES_PER_SECTOR]
            for s in range(SECTORS_PER_TRACK)
        ]
        out.extend(encode_track(t, sectors))
    return bytes(out)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("infile")
    ap.add_argument("outfile")
    ap.add_argument("--mem-words", type=lambda s: int(s, 0), default=262144,
                    help="number of 32-bit words in output hex (default 256K)")
    ap.add_argument("--track", type=int, default=None,
                    help="encode only this one track (0..159); default is all")
    args = ap.parse_args()

    with open(args.infile, "rb") as f:
        adf = f.read()

    track_bytes = SECTORS_PER_TRACK * BYTES_PER_SECTOR
    n_tracks = len(adf) // track_bytes
    if args.track is not None:
        tracks = [args.track]
    else:
        tracks = list(range(n_tracks))

    mfm = encode_disk(adf, tracks)

    if len(mfm) % 4 != 0:
        mfm += b"\x00" * (4 - (len(mfm) % 4))
    n_words = len(mfm) // 4
    if n_words > args.mem_words:
        print(f"encoded MFM is {len(mfm)} bytes ({n_words} words) but "
              f"--mem-words is {args.mem_words}", file=sys.stderr)
        return 1

    mem = [0] * args.mem_words
    for i in range(n_words):
        (mem[i],) = struct.unpack(">I", mfm[i * 4 : i * 4 + 4])

    with open(args.outfile, "w") as f:
        f.write("\n".join(f"{w:08x}" for w in mem) + "\n")
    print(f"encoded {len(tracks)} track(s) = {len(mfm)} disk bytes = "
          f"{n_words} 32-bit words", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
