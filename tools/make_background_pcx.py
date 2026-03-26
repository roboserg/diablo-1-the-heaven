"""
make_background_pcx.py - Convert docs/logo.png to a 640x480 8-bit PCX for the main menu background.

Output: res/thdata/ui_art/mainmenu.pcx  (640x480, 8-bit PCX)
        res/thdata/ui_art/swmmenu.pcx   (same, used for shareware/demo mode)

Usage:
    python tools/make_background_pcx.py
"""

import struct
import os
from PIL import Image

LOGO_SRC = "docs/logo.png"
OUTS = [
    "res/thdata/ui_art/mainmenu.pcx",
    "res/thdata/ui_art/swmmenu.pcx",
]
W, H = 640, 480

# ── PCX header (128 bytes) ──────────────────────────────────────────────────
def make_header(w, h):
    hdr = bytearray(128)
    hdr[0]  = 0x0A
    hdr[1]  = 0x05
    hdr[2]  = 0x01
    hdr[3]  = 0x08
    struct.pack_into('<HHHH', hdr, 4, 0, 0, w - 1, h - 1)
    struct.pack_into('<HH',   hdr, 12, 72, 72)
    hdr[65] = 1
    struct.pack_into('<H', hdr, 66, w)
    struct.pack_into('<H', hdr, 68, 1)
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

# ── Build 640x480 canvas with logo centered ─────────────────────────────────
def build_canvas():
    logo = Image.open(LOGO_SRC).convert("RGBA")
    lw, lh = logo.size

    # Scale to fit within 640x480, maintaining aspect ratio
    scale = min(W / lw, H / lh)
    new_w = round(lw * scale)
    new_h = round(lh * scale)
    logo = logo.resize((new_w, new_h), Image.LANCZOS)

    # Composite onto black background
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 255))
    x_off = (W - new_w) // 2
    y_off = (H - new_h) // 2
    canvas.paste(logo, (x_off, y_off), logo)

    # Quantize to 256 colors
    canvas_rgb = canvas.convert("RGB")
    quantized = canvas_rgb.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
    return quantized

def main():
    img = build_canvas()
    palette_bytes = img.getpalette()  # 768 bytes: R,G,B * 256
    pixels = list(img.getdata())

    # Build PCX body
    body = bytearray()
    for row_y in range(H):
        row = pixels[row_y * W : row_y * W + W]
        body += rle_row(row)

    # PCX extended palette: 0x0C + 256*RGB
    pal = bytearray([0x0C])
    pal += bytearray(palette_bytes[:768])

    os.makedirs("res/thdata/ui_art", exist_ok=True)
    for out in OUTS:
        with open(out, "wb") as f:
            f.write(make_header(W, H))
            f.write(body)
            f.write(pal)
        print(f"Written: {out}  ({os.path.getsize(out):,} bytes)")

    print(f"Logo scaled to {img.size[0]}x{img.size[1]} and centered on 640x480 black background.")
    print("These files override mainmenu.pcx / swmmenu.pcx from TH4data.mor.")

if __name__ == "__main__":
    main()
