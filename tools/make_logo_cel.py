"""
make_logo_cel.py - Convert docs/logo.png to Diablo CEL format for the main menu.

The menu logo is loaded from Data\th_logo2.CEL via Storm's file system.
CEL format: palette-indexed 8-bit, RLE encoded, rendered BOTTOM-TO-TOP.
The game's menu palette (MenuPal from dx_utility.cpp) maps index i -> RGB color.

Usage:
    python tools/make_logo_cel.py
Output:
    res/thdata/Data/th_logo2.CEL
"""

import struct
import re
import os
from PIL import Image

# --- Config ---
LOGO_SRC = "docs/logo.png"
CEL_OUT = "res/thdata/Data/th_logo2.CEL"
CEL_WIDTH = 430  # hardcoded width in MenuEngine.cpp
NUM_FRAMES = 16  # LogoFrameCount cycles 1..16
# Pixels with R+G+B <= this are rendered transparent (let menu BG show through)
TRANSPARENT_THRESHOLD = 45


# ---------------------------------------------------------------------------
# 1. Extract MenuPal from dx_utility.cpp (256 x RGBX entries)
# ---------------------------------------------------------------------------
def parse_menu_pal(src_path="src/dx_utility.cpp"):
    with open(src_path, "r") as f:
        content = f.read()
    m = re.search(r"uchar MenuPal\[1024\]\s*=\s*\{(.*?)\};", content, re.DOTALL)
    if not m:
        raise ValueError("MenuPal not found in dx_utility.cpp")
    nums = [int(v, 0) for v in re.findall(r"0[xX][0-9A-Fa-f]+|\d+", m.group(1))]
    palette = [(nums[i * 4], nums[i * 4 + 1], nums[i * 4 + 2]) for i in range(256)]
    return palette  # palette[i] = (R, G, B) for CEL pixel value i


# ---------------------------------------------------------------------------
# 2. Encode one scanline to CEL RLE
#    Positive byte N  -> next N bytes are literal palette indices
#    Negative byte -N -> skip N pixels (transparent)
# ---------------------------------------------------------------------------
def encode_row(pixels, palette):
    data = bytearray()
    x, W = 0, len(pixels)
    while x < W:
        px = pixels[x]
        if px == 0:  # transparent
            end = x
            while end < W and pixels[end] == 0:
                end += 1
            count = end - x
            while count > 0:
                run = min(count, 127)
                data.append((256 - run) & 0xFF)
                count -= run
            x = end
        else:
            end = x
            while end < W and pixels[end] != 0:
                end += 1
            indices = pixels[x:end]
            i = 0
            while i < len(indices):
                run = min(len(indices) - i, 127)
                data.append(run)
                data.extend(indices[i : i + run])
                i += run
            x = end
    return bytes(data)


# ---------------------------------------------------------------------------
# 3. Build full 16-frame CEL
# ---------------------------------------------------------------------------
def build_cel(frame_bytes, num_frames=NUM_FRAMES):
    header_ints = num_frames + 2
    header_size = header_ints * 4
    frame_size = len(frame_bytes)

    hdr = bytearray()
    hdr += struct.pack("<I", num_frames)
    for i in range(num_frames):
        hdr += struct.pack("<I", header_size + i * frame_size)
    hdr += struct.pack("<I", header_size + num_frames * frame_size)

    return bytes(hdr) + frame_bytes * num_frames


# ---------------------------------------------------------------------------
# 4. Main
# ---------------------------------------------------------------------------
def main():
    palette = parse_menu_pal()

    # Create PIL palette image for quantization
    pal_img = Image.new("P", (256, 1))
    pal_data = []
    for r, g, b in palette:
        pal_data.extend([r, g, b])
    pal_img.putpalette(pal_data)

    img = Image.open(LOGO_SRC).convert("RGBA")
    W, H = img.size
    new_h = round(H * CEL_WIDTH / W)
    img = img.resize((CEL_WIDTH, new_h), Image.LANCZOS)
    print(f"Resized logo: {CEL_WIDTH}x{new_h}")

    # Make dark pixels transparent
    def is_dark(r, g, b, a):
        return r + g + b <= TRANSPARENT_THRESHOLD or a < 128

    # Convert to palette mode using the game's palette
    img_rgb = img.convert("RGB")
    img_p = Image.new("P", (CEL_WIDTH, new_h))
    img_p.putpalette(pal_data)

    # Quantize using dither
    result = img_rgb.quantize(palette=img_p, dither=Image.FLOYDSTEINBERG)

    # Apply transparency mask
    alpha = img.split()[3] if img.mode == "RGBA" else None
    pixels = list(result.getdata())

    # Mark transparent pixels as index 0
    if alpha:
        pixels = [
            0
            if is_dark(
                *img_rgb.getpixel((x % CEL_WIDTH, x // CEL_WIDTH)),
                alpha.getpixel((x % CEL_WIDTH, x // CEL_WIDTH)),
            )
            else p
            for x, p in enumerate(pixels)
        ]
    else:
        pixels = [
            0
            if sum(img_rgb.getpixel((x % CEL_WIDTH, x // CEL_WIDTH)))
            <= TRANSPARENT_THRESHOLD
            else p
            for x, p in enumerate(pixels)
        ]

    # Convert to rows (top to bottom for encoding, will reverse later)
    rows = [pixels[y * CEL_WIDTH : (y + 1) * CEL_WIDTH] for y in range(new_h)]

    # Encode rows BOTTOM-TO-TOP
    frame = bytearray()
    for y in range(new_h - 1, -1, -1):
        frame += encode_row(rows[y], palette)

    cel = build_cel(bytes(frame))

    os.makedirs(os.path.dirname(CEL_OUT), exist_ok=True)
    with open(CEL_OUT, "wb") as f:
        f.write(cel)

    print(f"Written: {CEL_OUT}  ({len(cel):,} bytes, {NUM_FRAMES} frames)")
    print(f"Frame data: {len(frame):,} bytes per frame")
    print()
    print("Next step: ensure 'res/thdata/Data/th_logo2.CEL' is packaged into the")
    print("mod's MPQ archive or placed as a loose file in your game Data\\ directory.")


if __name__ == "__main__":
    main()
