"""
make_background_pcx.py - Generate a plain dark background PCX for the main menu.

Replaces ui_art/hf_title.pcx from TH4data.mor with a neutral dark background,
so the th_logo2.CEL overlay (our mod logo) is the only thing shown.

Output: res/thdata/ui_art/hf_title.pcx  (640x480, 8-bit PCX)
"""

import struct
import os

W, H = 640, 480
OUT = "res/thdata/ui_art/hf_title.pcx"

# Background color: very dark grey (near-black, avoids pure 0,0,0 which may be
# treated as transparent by the game's palette).
BG_R, BG_G, BG_B = 8, 8, 12   # dark blueish black
BG_IDX = 1                      # palette index to use (0 is often special)

# ── PCX header (128 bytes) ──────────────────────────────────────────────────
def make_header(w, h):
    hdr = bytearray(128)
    hdr[0]  = 0x0A        # manufacturer
    hdr[1]  = 0x05        # version 3.0
    hdr[2]  = 0x01        # RLE encoding
    hdr[3]  = 0x08        # 8 bits per pixel
    struct.pack_into('<HHHH', hdr, 4, 0, 0, w - 1, h - 1)   # xmin/ymin/xmax/ymax
    struct.pack_into('<HH',   hdr, 12, 72, 72)               # dpi
    hdr[65] = 1           # 1 color plane
    struct.pack_into('<H', hdr, 66, w)                       # bytes per line (must be even)
    struct.pack_into('<H', hdr, 68, 1)                       # palette: color
    return bytes(hdr)

# ── RLE-encode one scanline ─────────────────────────────────────────────────
def rle_row(row):
    data = bytearray()
    x, n = 0, len(row)
    while x < n:
        v = row[x]
        run = 1
        while x + run < n and row[x + run] == v and run < 63:
            run += 1
        if run > 1 or (v & 0xC0) == 0xC0:
            data.append(0xC0 | run)
        data.append(v)
        x += run
    return bytes(data)

# ── 256-colour palette (769 bytes: 0x0C + 256*RGB) ──────────────────────────
def make_palette():
    pal = bytearray(769)
    pal[0] = 0x0C       # extended palette marker
    # index 0 = black (keep as pure black)
    pal[1], pal[2], pal[3] = 0, 0, 0
    # index BG_IDX = our background colour
    pal[1 + BG_IDX*3]     = BG_R
    pal[1 + BG_IDX*3 + 1] = BG_G
    pal[1 + BG_IDX*3 + 2] = BG_B
    return bytes(pal)

def main():
    row = bytes([BG_IDX] * W)
    image = bytearray()
    for _ in range(H):
        image += rle_row(row)

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "wb") as f:
        f.write(make_header(W, H))
        f.write(image)
        f.write(make_palette())

    print(f"Written: {OUT}  ({os.path.getsize(OUT):,} bytes, {W}x{H})")
    print("Place build/ui_art/hf_title.pcx in your game directory to override TH4data.mor.")

if __name__ == "__main__":
    main()
