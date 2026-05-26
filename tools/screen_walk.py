#!/usr/bin/env python3
"""Walk an Intuition Screen struct from a chipram dump (CHIPRAM_DUMP=).

The Workbench Screen on K1.3 has consistently been seen at $BE80 in
sims that reach the post-bootblock deadlock.  Pass the address as the
second argument to inspect a different Screen.

Prints Screen, its embedded ViewPort, RastPort, and BitMap.  Every
long that reads $CACACACA, $00000000, or otherwise looks uninitialised
is flagged inline so we can tell which fields OpenScreen never wrote.

ROM string pointers (>= $FC0000) are not dereferenced -- their target
isn't in the chipram dump.  Pointers inside the dump are dereferenced
to a short repr (string for STRPTRs, hex bytes for raw).

Usage:
    CHIPRAM_DUMP=dump.bin ./Vm68k_top <cycles>
    python3 tools/screen_walk.py dump.bin           # walks $BE80
    python3 tools/screen_walk.py dump.bin 0xBE80    # explicit addr
"""
import sys

if len(sys.argv) < 2:
    print(f'usage: {sys.argv[0]} chipram_dump.bin [screen_addr]', file=sys.stderr)
    sys.exit(2)

with open(sys.argv[1], 'rb') as f:
    chip = f.read()

SCREEN_ADDR = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0xBE80


def rd_b(addr):
    if addr >= len(chip):
        return 0
    return chip[addr]


def rd_w(addr):
    if addr + 2 > len(chip):
        return 0
    return int.from_bytes(chip[addr:addr+2], 'big')


def rd_l(addr):
    if addr + 4 > len(chip):
        return 0
    return int.from_bytes(chip[addr:addr+4], 'big')


def is_caca(v):
    return v == 0xCACACACA


def is_uninit_long(v):
    """Heuristics: clearly-not-yet-initialised values."""
    return v in (0x00000000, 0xCACACACA, 0xFFFFFFFF) or (v & 0xFFFF0000) == 0xCACA0000


def tag(v):
    if v == 0xCACACACA:
        return '  <UNINIT CACACACA>'
    if v == 0:
        return '  <zero>'
    if (v & 0xFFFF0000) == 0xCACA0000:
        return '  <UNINIT CACA upper>'
    if v == 0xFFFFFFFF:
        return '  <UNINIT FFFFFFFF>'
    return ''


def repr_string(addr, max_len=32):
    if addr == 0 or addr >= 0x1000000:
        return ''
    if addr >= 0xFC0000:
        return f' -> <ROM @${addr:08X}>'
    if addr >= len(chip):
        return f' -> <out-of-dump @${addr:08X}>'
    s = b''
    for i in range(max_len):
        b = chip[addr+i:addr+i+1]
        if b == b'\x00' or not b:
            break
        if not (0x20 <= b[0] < 0x7F):
            # Non-printable -- bail out, probably not a string
            return ''
        s += b
    if not s:
        return ''
    return f' -> "{s.decode("latin-1")}"'


# ---- Screen fields ---------------------------------------------------
print(f'Screen @ ${SCREEN_ADDR:08X}')
print(f'  $00 NextScreen     = ${rd_l(SCREEN_ADDR+0x00):08X}{tag(rd_l(SCREEN_ADDR+0x00))}')
print(f'  $04 FirstWindow    = ${rd_l(SCREEN_ADDR+0x04):08X}{tag(rd_l(SCREEN_ADDR+0x04))}')
print(f'  $08 LeftEdge       = {rd_w(SCREEN_ADDR+0x08):>5d}')
print(f'  $0A TopEdge        = {rd_w(SCREEN_ADDR+0x0A):>5d}')
print(f'  $0C Width          = {rd_w(SCREEN_ADDR+0x0C):>5d}')
print(f'  $0E Height         = {rd_w(SCREEN_ADDR+0x0E):>5d}')
print(f'  $10 MouseY/MouseX  = {rd_w(SCREEN_ADDR+0x10):>5d}, {rd_w(SCREEN_ADDR+0x12):>5d}')
print(f'  $14 Flags          = ${rd_w(SCREEN_ADDR+0x14):04X}')
title_ptr = rd_l(SCREEN_ADDR+0x16)
print(f'  $16 Title          = ${title_ptr:08X}{repr_string(title_ptr)}')
def_title_ptr = rd_l(SCREEN_ADDR+0x1A)
print(f'  $1A DefaultTitle   = ${def_title_ptr:08X}{repr_string(def_title_ptr)}')
print(f'  $1E BarHeight={rd_b(SCREEN_ADDR+0x1E):>3d} BarVBorder={rd_b(SCREEN_ADDR+0x1F):>3d}'
      f' BarHBorder={rd_b(SCREEN_ADDR+0x20):>3d} MenuVBorder={rd_b(SCREEN_ADDR+0x21):>3d}'
      f' MenuHBorder={rd_b(SCREEN_ADDR+0x22):>3d}')
print(f'  $23 WBorTop={rd_b(SCREEN_ADDR+0x23):>3d} WBorLeft={rd_b(SCREEN_ADDR+0x24):>3d}'
      f' WBorRight={rd_b(SCREEN_ADDR+0x25):>3d} WBorBottom={rd_b(SCREEN_ADDR+0x26):>3d}')
print(f'  $28 Font           = ${rd_l(SCREEN_ADDR+0x28):08X}{tag(rd_l(SCREEN_ADDR+0x28))}')

# ---- ViewPort at +$2C ------------------------------------------------
VP = SCREEN_ADDR + 0x2C
print(f'\nViewPort @ ${VP:08X} (Screen + $2C):')
print(f'  $00 Next           = ${rd_l(VP+0x00):08X}{tag(rd_l(VP+0x00))}')
print(f'  $04 ColorMap       = ${rd_l(VP+0x04):08X}{tag(rd_l(VP+0x04))}')
print(f'  $08 DspIns         = ${rd_l(VP+0x08):08X}{tag(rd_l(VP+0x08))}')
print(f'  $0C SprIns         = ${rd_l(VP+0x0C):08X}{tag(rd_l(VP+0x0C))}')
print(f'  $10 ClrIns         = ${rd_l(VP+0x10):08X}{tag(rd_l(VP+0x10))}')
print(f'  $14 UCopIns        = ${rd_l(VP+0x14):08X}{tag(rd_l(VP+0x14))}')
print(f'  $18 DWidth/DHeight = {rd_w(VP+0x18):>5d} x {rd_w(VP+0x1A):>5d}')
print(f'  $1C DxOff/DyOff    = {rd_w(VP+0x1C):>5d}, {rd_w(VP+0x1E):>5d}')
print(f'  $20 Modes          = ${rd_w(VP+0x20):04X}')
print(f'  $22 SprPri/ExtMod  = ${rd_b(VP+0x22):02X} / ${rd_b(VP+0x23):02X}')
print(f'  $24 RasInfo        = ${rd_l(VP+0x24):08X}{tag(rd_l(VP+0x24))}')

# ---- RastPort at +$54 ------------------------------------------------
RP = SCREEN_ADDR + 0x54
print(f'\nRastPort @ ${RP:08X} (Screen + $54):')
print(f'  $00 Layer          = ${rd_l(RP+0x00):08X}{tag(rd_l(RP+0x00))}')
print(f'  $04 BitMap         = ${rd_l(RP+0x04):08X}{tag(rd_l(RP+0x04))}')
print(f'  $08 AreaPtrn       = ${rd_l(RP+0x08):08X}{tag(rd_l(RP+0x08))}')
print(f'  $0C TmpRas         = ${rd_l(RP+0x0C):08X}{tag(rd_l(RP+0x0C))}')
print(f'  $10 AreaInfo       = ${rd_l(RP+0x10):08X}{tag(rd_l(RP+0x10))}')
print(f'  $14 GelsInfo       = ${rd_l(RP+0x14):08X}{tag(rd_l(RP+0x14))}')
print(f'  $18 Mask/FgPen/BgPen/AOlPen = '
      f'${rd_b(RP+0x18):02X} ${rd_b(RP+0x19):02X} ${rd_b(RP+0x1A):02X} ${rd_b(RP+0x1B):02X}')
print(f'  $1C DrawMode/AreaPtSz/linpatcnt = '
      f'${rd_b(RP+0x1C):02X} ${rd_b(RP+0x1D):02X} ${rd_b(RP+0x1E):02X}')
print(f'  $20 Flags/LinePtrn = ${rd_w(RP+0x20):04X} / ${rd_w(RP+0x22):04X}')
print(f'  $24 cp_x/cp_y      = {rd_w(RP+0x24):>5d}, {rd_w(RP+0x26):>5d}')
# minterms is 8 bytes -- our trace observed CACACACA at offset +$7C-7F.
# Default Amiga values: $0C $0A $03 $00 (ABC,ABc,AbC,Abc) + 4 zero bytes.
mt = [rd_b(RP+0x28+i) for i in range(8)]
mt_long_hi = rd_l(RP+0x28)
mt_long_lo = rd_l(RP+0x2C)
default = [0x0C, 0x0A, 0x03, 0x00]
mt_flag = ''
if mt[:4] == default:
    mt_flag = '  <default ABC/ABc/AbC/Abc>'
elif mt_long_hi == 0xCACACACA:
    mt_flag = '  <UNINIT CACACACA -- OpenScreen did NOT populate>'
elif all(b == 0 for b in mt):
    mt_flag = '  <all-zero>'
print(f'  $28 minterms[0..7] = {" ".join(f"${b:02X}" for b in mt)}{mt_flag}')
print(f'      (as 2 longs: ${mt_long_hi:08X} ${mt_long_lo:08X})')
print(f'  $30 PenWidth/PenHeight = {rd_w(RP+0x30):>5d}, {rd_w(RP+0x32):>5d}')
print(f'  $34 Font           = ${rd_l(RP+0x34):08X}{tag(rd_l(RP+0x34))}')
print(f'  $38 AlgoStyle/TxFlags = ${rd_b(RP+0x38):02X} / ${rd_b(RP+0x39):02X}')
print(f'  $3A TxHeight/TxWidth/TxBaseline/TxSpacing = '
      f'{rd_w(RP+0x3A):>5d}, {rd_w(RP+0x3C):>5d}, {rd_w(RP+0x3E):>5d}, {rd_w(RP+0x40):>5d}')

# ---- BitMap pointed to by RastPort.BitMap, OR embedded at Screen+$B8 -
bm_ptr = rd_l(RP+0x04)
if bm_ptr and bm_ptr < 0x1000000 and bm_ptr < len(chip):
    BM = bm_ptr
    print(f'\nBitMap @ ${BM:08X} (RastPort.BitMap):')
else:
    BM = SCREEN_ADDR + 0xB8
    print(f'\nBitMap @ ${BM:08X} (Screen + $B8, RP.BitMap was ${bm_ptr:08X}):')
print(f'  $00 BytesPerRow    = {rd_w(BM+0x00):>5d}')
print(f'  $02 Rows           = {rd_w(BM+0x02):>5d}')
print(f'  $04 Flags          = ${rd_b(BM+0x04):02X}')
print(f'  $05 Depth          = {rd_b(BM+0x05):>3d}')
print(f'  $06 pad            = ${rd_w(BM+0x06):04X}')
for i in range(8):
    p = rd_l(BM+0x08 + 4*i)
    if p == 0 and i >= 2:
        continue
    print(f'  $08 Planes[{i}]     = ${p:08X}{tag(p)}')


# ---- Summary --------------------------------------------------------
print('\n=== summary ===')
uninit_count = 0
for off in (0x00, 0x04, 0x28, RP-SCREEN_ADDR+0x00, RP-SCREEN_ADDR+0x04,
            RP-SCREEN_ADDR+0x28, RP-SCREEN_ADDR+0x2C, RP-SCREEN_ADDR+0x34):
    v = rd_l(SCREEN_ADDR + off)
    if is_uninit_long(v):
        uninit_count += 1
print(f'{uninit_count} critical pointer/field(s) look uninitialised')
if rd_l(RP+0x28) == 0xCACACACA:
    print('** RastPort.minterms[0..3] is $CACACACA -- OpenScreen did NOT')
    print('   complete initialisation.  Workbench OpenScreen was started')
    print('   (Title is set) but the embedded RastPort was never finished.')
