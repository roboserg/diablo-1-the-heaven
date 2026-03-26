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
LOGO_SRC  = "docs/logo.png"
CEL_OUT   = "res/thdata/Data/th_logo2.CEL"
CEL_WIDTH = 430          # hardcoded width in MenuEngine.cpp
NUM_FRAMES = 16          # LogoFrameCount cycles 1..16
# Pixels with R+G+B <= this are rendered transparent (let menu BG show through)
TRANSPARENT_THRESHOLD = 30

# ---------------------------------------------------------------------------
# 1. Extract MenuPal from dx_utility.cpp (256 x RGBX entries)
# ---------------------------------------------------------------------------
def parse_menu_pal(src_path="src/dx_utility.cpp"):
    with open(src_path, "r") as f:
        content = f.read()
    m = re.search(r'uchar MenuPal\[1024\]\s*=\s*\{(.*?)\};', content, re.DOTALL)
    if not m:
        raise ValueError("MenuPal not found in dx_utility.cpp")
    nums = [int(v, 0) for v in re.findall(r'0[xX][0-9A-Fa-f]+|\d+', m.group(1))]
    palette = [(nums[i*4], nums[i*4+1], nums[i*4+2]) for i in range(256)]
    return palette  # palette[i] = (R, G, B) for CEL pixel value i

# ---------------------------------------------------------------------------
# 2. Nearest-colour lookup (squared Euclidean distance)
# ---------------------------------------------------------------------------
def make_lookup(palette):
    """Pre-build a 256^3 is too big; do on-demand with caching."""
    cache = {}
    def nearest(r, g, b):
        key = (r >> 3, g >> 3, b >> 3)   # reduce to ~32-shade buckets
        if key in cache:
            return cache[key]
        best, best_d = 0, 1 << 30
        for i, (pr, pg, pb) in enumerate(palette):
            d = (r-pr)**2 + (g-pg)**2 + (b-pb)**2
            if d < best_d:
                best_d, best = d, i
        cache[key] = best
        return best
    return nearest

# ---------------------------------------------------------------------------
# 3. Encode one scanline to CEL RLE
#    Positive byte N  -> next N bytes are literal palette indices
#    Negative byte -N -> skip N pixels (transparent)
# ---------------------------------------------------------------------------
def encode_row(row, nearest, threshold):
    data = bytearray()
    x, W = 0, len(row)
    while x < W:
        r, g, b = row[x]
        if r + g + b <= threshold:
            # transparent run
            end = x
            while end < W and sum(row[end]) <= threshold:
                end += 1
            count = end - x
            while count > 0:
                run = min(count, 127)
                data.append((256 - run) & 0xFF)   # two's-complement negative
                count -= run
            x = end
        else:
            # literal run
            end = x
            while end < W and sum(row[end]) > threshold:
                end += 1
            indices = [nearest(*row[k]) for k in range(x, end)]
            i = 0
            while i < len(indices):
                run = min(len(indices) - i, 127)
                data.append(run)
                data.extend(indices[i:i+run])
                i += run
            x = end
    return bytes(data)

# ---------------------------------------------------------------------------
# 4. Build full 16-frame CEL
#    Header layout (1-indexed frames, as used by Surface_DrawCEL):
#      header[0]      = num_frames  (int32)
#      header[1..N]   = frame data offsets from file start  (int32 each)
#      header[N+1]    = end-of-last-frame offset  (int32)
#    Frame data = concatenated encoded scanlines, BOTTOM ROW FIRST
#    (Diablo RleDraw advances dst UPWARD after each row)
# ---------------------------------------------------------------------------
def build_cel(frame_bytes, num_frames=NUM_FRAMES):
    header_ints = num_frames + 2          # header[0] + N offsets + end
    header_size = header_ints * 4
    frame_size  = len(frame_bytes)

    hdr = bytearray()
    hdr += struct.pack('<I', num_frames)  # header[0]
    for i in range(num_frames):
        hdr += struct.pack('<I', header_size + i * frame_size)
    hdr += struct.pack('<I', header_size + num_frames * frame_size)   # end

    return bytes(hdr) + frame_bytes * num_frames

# ---------------------------------------------------------------------------
# 5. Main
# ---------------------------------------------------------------------------
def main():
    palette = parse_menu_pal()
    nearest = make_lookup(palette)

    img = Image.open(LOGO_SRC).convert("RGB")
    W, H = img.size
    new_h = round(H * CEL_WIDTH / W)
    img = img.resize((CEL_WIDTH, new_h), Image.LANCZOS)
    print(f"Resized logo: {CEL_WIDTH}x{new_h}")

    pixels = [
        [img.getpixel((x, y)) for x in range(CEL_WIDTH)]
        for y in range(new_h)
    ]

    # Encode rows BOTTOM-TO-TOP (Diablo RleDraw renders upward)
    frame = bytearray()
    for y in range(new_h - 1, -1, -1):
        frame += encode_row(pixels[y], nearest, TRANSPARENT_THRESHOLD)

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
