#!/usr/bin/env python3
"""Convert an FS-UAE save state (.uss) into our Verilator snapshot format.

Outputs three files compatible with build_cosim_window/snap/:
    regs.txt   — CPU regs + SR + chipset regs (INTENA/INTREQ/DMACON)
    mem.hex    — 512KB chip RAM, one 32-bit longword per line in hex
    slow.hex   — 512KB slow RAM (BRAM chunk), same format

Lets us compare an FS-UAE-captured A500/K1.3/WB1.3 state byte-for-byte
against our Verilator snapshot at the same boot moment, and feed FS-UAE
state into the existing musashi_kick `--seed=<dir>` flow for divergence
bisection.

Usage:
    python3 tools/fsuae_state.py "<path to .uss>" <output dir>
"""
import struct
import sys
import zlib
from pathlib import Path


def parse_chunks(data):
    """Yield (chunk_id, offset, size_field, payload) for every chunk.

    UAE save-state chunks are 4-byte aligned, but the chunk size field
    is byte-exact — so after consuming a chunk we round up to the next
    4-byte boundary before reading the next chunk header.  Without
    rounding, we'd stop at chunks that don't happen to end on a
    longword boundary (e.g. KEYB which has size=27).
    """
    i = 0
    while i < len(data) - 8:
        cid = data[i:i+4]
        if not all(32 <= b < 127 for b in cid):
            i += 1
            continue
        size = struct.unpack('>I', data[i+4:i+8])[0]
        if size <= 8 or i + size > len(data):
            i += 1
            continue
        yield cid.decode('ascii'), i, size, data[i+8:i+size]
        # Advance to next 4-byte aligned position past this chunk.
        i = (i + size + 3) & ~3


def inflate_mem_chunk(payload):
    """UAE memory chunks: 4-byte flags, 4-byte uncompressed size, payload.

    Flags bit 0 = zlib-compressed.  When uncompressed, the usize field
    is unreliable (FS-UAE 3.2.35 CRAM sets it to 0, BRAM to garbage),
    so we just return everything past the 8-byte header.
    """
    flags = struct.unpack('>I', payload[0:4])[0]
    if flags & 1:
        return zlib.decompress(payload[8:])
    # Uncompressed — usize is unreliable; return all remaining bytes.
    return payload[8:]


def find_chunk(chunks, cid):
    for c in chunks:
        if c[0] == cid:
            return c
    return None


def parse_cpu(payload):
    """FS-UAE 3.x CPU chunk layout (verified against version 3.3.0 save
    state at PC=$FC0F94 with A0=$C0040C / A2=$FDFF50 / A6=$C00276):

        +$00  MODEL          u32  (0 = 68000)
        +$04  FLAGS          u32  (cap bits + addr_space_24)
        +$08  ?               u32  (unknown extra field — version?)
        +$0C  D0..D7         8 × u32
        +$2C  A0..A6         7 × u32
        +$48  PC             u32
        +$4C  IRC + IR       u32  (two u16 packed)
        +$50  USP            u32
        +$54  ISP / SSP      u32  (= A7 when supervisor)
        +$58  SR (high)      u32  (SR is upper 16 bits)
    """
    regs = {}
    for i in range(8):
        regs[f'D{i}'] = struct.unpack('>I', payload[0x0C + 4*i:0x0C + 4*i + 4])[0]
    for i in range(7):
        regs[f'A{i}'] = struct.unpack('>I', payload[0x2C + 4*i:0x2C + 4*i + 4])[0]
    regs['PC']  = struct.unpack('>I', payload[0x48:0x4C])[0]
    regs['USP'] = struct.unpack('>I', payload[0x50:0x54])[0]
    regs['ISP'] = struct.unpack('>I', payload[0x54:0x58])[0]
    # A7 is whichever stack is active — ISP in supervisor mode, USP in user.
    sr_field = struct.unpack('>I', payload[0x58:0x5C])[0]
    regs['SR'] = (sr_field >> 16) & 0xFFFF
    sup = (regs['SR'] >> 13) & 1
    regs['A7'] = regs['ISP'] if sup else regs['USP']
    return regs


def write_hex(out_path, data, longword_count):
    """Emit one 32-bit hex per line, padded out to longword_count entries."""
    with open(out_path, 'w') as f:
        for i in range(longword_count):
            if 4*i + 4 <= len(data):
                v = struct.unpack('>I', data[4*i:4*i+4])[0]
            else:
                v = 0
            f.write(f'{v:08x}\n')


def parse_custom(payload):
    """CHIP chunk (= save_custom() output in custom.cpp).  Has a 4-byte
    chunk-format header before chipset_mask, then the chipset register
    map starting at +$04.  Verified empirically by finding the known
    INTENAR value ($602C) at +$24 (= INTENAR position from save_custom
    + 4-byte prefix).

    Layout (offsets within CHIP payload):
        +$04  chipset_mask | 0x100
        +$08  BLTDDAT  (UWORD)
        +$0A  DMACONR
        +$0C  VPOSR
        +$0E  VHPOSR
        ...
        +$24  INTENAR
        +$26  INTREQR
        +$28  DSKPT  (ULONG)
        +$2C  DSKLEN
    """
    return {
        'DMACON': struct.unpack('>H', payload[0x0A:0x0C])[0],
        'VPOSR':  struct.unpack('>H', payload[0x0C:0x0E])[0],
        'INTENA': struct.unpack('>H', payload[0x24:0x26])[0],
        'INTREQ': struct.unpack('>H', payload[0x26:0x28])[0],
        'DSKPT':  struct.unpack('>I', payload[0x28:0x2C])[0],
        'DSKLEN': struct.unpack('>H', payload[0x2C:0x2E])[0],
    }


def main():
    if len(sys.argv) < 3:
        print('usage: fsuae_state.py <input.uss> <output_dir>', file=sys.stderr)
        return 2

    uss_path = sys.argv[1]
    out_dir  = Path(sys.argv[2])
    out_dir.mkdir(parents=True, exist_ok=True)

    with open(uss_path, 'rb') as f:
        data = f.read()

    chunks = list(parse_chunks(data))
    cram = find_chunk(chunks, 'CRAM')
    bram = find_chunk(chunks, 'BRAM')
    cpu  = find_chunk(chunks, 'CPU ')
    chip_chunk = find_chunk(chunks, 'CHIP')
    if cram is None or cpu is None:
        print('error: required chunk missing (need CRAM and CPU)', file=sys.stderr)
        return 1

    chip = inflate_mem_chunk(cram[3])
    slow = inflate_mem_chunk(bram[3]) if bram else b'\x00' * 524288
    regs = parse_cpu(cpu[3])
    custom = parse_custom(chip_chunk[3]) if chip_chunk else {}

    write_hex(out_dir / 'mem.hex', chip, 131072)
    write_hex(out_dir / 'slow.hex', slow, 131072)

    with open(out_dir / 'regs.txt', 'w') as f:
        f.write(f"PC={regs['PC']:08x}\n")
        f.write(f"SR={regs['SR']:04x}\n")
        f.write(f"USP={regs['USP']:08x}\n")
        for i in range(8):
            f.write(f"D{i}={regs[f'D{i}']:08x}\n")
        for i in range(7):
            f.write(f"A{i}={regs[f'A{i}']:08x}\n")
        f.write(f"A7={regs['A7']:08x}\n")
        # Match the existing regs.txt format used by Verilator+musashi_kick.
        f.write(f"INTENA={custom.get('INTENA', 0):04x}\n")
        f.write(f"INTREQ={custom.get('INTREQ', 0):04x}\n")
        f.write(f"DMACON={custom.get('DMACON', 0):04x}\n")
        f.write(f"RETIRED=0\n")

    print(f"wrote {out_dir}/regs.txt + mem.hex + slow.hex")
    print(f"  CPU: PC={regs['PC']:08x} A7={regs['A7']:08x} SR={regs['SR']:04x}")
    print(f"  CHIP: {len(chip)} bytes  SLOW: {len(slow)} bytes")
    return 0


if __name__ == '__main__':
    sys.exit(main())
