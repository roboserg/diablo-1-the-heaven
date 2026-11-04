# -*- coding: utf-8 -*-
"""
overlay_dump_verify.py — verifier for TH4 EnrichedLightUltra Stage-2 debug dumps.

Verifies the 6 raw dumps written by RenderQuadTextureUltra while a file named
`overlay_dump_on.flag` exists in the game's working directory (C:\\TH4_BUILD):

    overlay_dump_alpha.raw   8 bpp   AlphaSurface (overlay weight, 0/128/255)
    overlay_dump_sprite.raw  32 bpp  SpriteLayerSurface (BGRA words)
    overlay_dump_wob.raw     8 bpp   WallOcclusionSurface (WOB_WALL = 1)
    overlay_dump_ws.raw      8 bpp   WorkingSurface (palette indices)
    overlay_dump_light.raw   8 bpp   LightSurface (post-zoom)
    overlay_dump_texcolor.raw 32 bpp the PACKED A8R8G8B8 texture the shader
                                      samples (B,G,R,A bytes) — the only dump
                                      that can catch UPLOAD-stage bugs.

Checks performed (all at the dump resolution, 1280x720 post-zoom):
  1. Packed texture A byte == AlphaSurface for every pixel (0 expected).
  2. Every overlay pixel (alpha>0) has A > 0 in the packed texture
     (catches the 2026-09-02 _mm_madd_epi16 bug: [a0,a2,0,0] per 4 px).
  3. No A != 0 where alpha == 0.
  4. Texture RGB at overlay pixels equals the rgbLUT[ws] color of the wall
     (s0 holds rgbLUT[WorkingSurface]; the sprite lives in s2, not s0).
     NOTE: needs palette-independent form, so it checks CONSISTENCY instead:
     equal ws indices must yield equal packed RGB anywhere in the frame.
  5. Overlay coverage sits on WOB_WALL (wob&1) at every overlay pixel.
  6. Composite smoothness: simulate the shader (luminance proxies) and check
     the per-column mean has no period-4 (or period-2) structure — the
     signature of the old stripes bug (colmean by mod4 was ~83/95/95/83).

Usage:  python overlay_dump_verify.py [dump_folder]   (default C:\\TH4_BUILD)
Prints PASS/FAIL per check and exits nonzero on any failure.
Requires numpy. Pillow not needed.
"""
import sys
import os
import numpy as np

W, H = 1280, 720


def load8(path):
    return np.fromfile(path, dtype=np.uint8).reshape(H, W)


def load32(path):
    return np.fromfile(path, dtype=np.uint8).reshape(H, W, 4)


def main():
    folder = sys.argv[1] if len(sys.argv) > 1 else r"C:\TH4_BUILD"
    names = ["overlay_dump_alpha.raw", "overlay_dump_sprite.raw",
             "overlay_dump_wob.raw", "overlay_dump_ws.raw",
             "overlay_dump_light.raw", "overlay_dump_texcolor.raw"]
    paths = [os.path.join(folder, n) for n in names]
    missing = [n for n, p in zip(names, paths) if not os.path.isfile(p)]
    if missing:
        print("FAIL: missing dump files in", folder, ":", missing)
        return 1

    alpha = load8(paths[0])
    spr = load32(paths[1])
    wob = load8(paths[2])
    ws = load8(paths[3])
    light = load8(paths[4])
    tex = load32(paths[5])

    amask = alpha > 0
    fails = 0

    def check(name, cond, detail=""):
        nonlocal fails
        ok = bool(cond)
        print(("PASS " if ok else "FAIL ") + name + ("  " + detail if detail else ""))
        if not ok:
            fails += 1

    # 1) packed A byte == AlphaSurface everywhere
    adiff = (tex[:, :, 3].astype(np.int32) != alpha.astype(np.int32))
    check("packed A byte == AlphaSurface (all pixels)",
          adiff.sum() == 0, f"{int(adiff.sum())} mismatches")

    # 2) every overlay pixel has nonzero A in the packed texture
    azero = (tex[:, :, 3] == 0) & amask
    check("packed A > 0 at every overlay pixel", azero.sum() == 0,
          f"{int(azero.sum())} zeroed (stripe bug if period-4)")

    # 3) no A where alpha == 0
    aextra = (tex[:, :, 3] != 0) & ~amask
    check("packed A == 0 where alpha == 0", aextra.sum() == 0,
          f"{int(aextra.sum())} stray A bytes")

    # 4) packed RGB consistency: same ws index -> same RGB (s0 = rgbLUT[ws])
    t32 = tex.astype(np.uint32).view(np.uint32)[:, :, 0] & 0x00FFFFFF
    wsflat = ws.ravel()
    tflat = t32.ravel()
    order = np.argsort(wsflat, kind="stable")
    ws_sorted = wsflat[order]
    t_sorted = tflat[order]
    boundaries = np.where(np.diff(ws_sorted) != 0)[0] + 1
    starts = np.concatenate(([0], boundaries))
    ends = np.concatenate((boundaries, [len(wsflat)]))
    inconsistent = 0
    for s, e in zip(starts, ends):
        if e - s > 1 and np.unique(t_sorted[s:e]).size > 1:
            inconsistent += 1
    check("packed RGB consistent per ws index (s0 == rgbLUT[ws])",
          inconsistent == 0, f"{inconsistent} ws indices with mixed RGB")

    # 5) overlay coverage sits on WOB_WALL
    offwall = amask & ((wob & 1) == 0)
    check("overlay pixels sit on WOB_WALL", offwall.sum() == 0,
          f"{int(offwall.sum())} overlay pixels not on wall")

    # 6) composite smoothness (no period-4/period-2 column structure)
    ys, xs = np.where(amask)
    if len(ys):
        y0, y1, x0, x1 = ys.min(), ys.max(), xs.min(), xs.max()
        spr_lum = spr[:, :, :3][:, :, ::-1] @ np.array([0.299, 0.587, 0.114])
        wall_lum = ws.astype(np.float32)
        lit = wall_lum * np.clip(1.0 - light.astype(np.float32) / 255.0, 0, 1)
        w = alpha.astype(np.float32) / 255.0
        final = spr_lum * w + lit * (1 - w)
        cm = final[y0:y1 + 1, x0:x1 + 1].mean(axis=0)
        by4 = [float(cm[m::4].mean()) for m in range(4)]
        spread = max(by4) - min(by4)
        check("composite column means uniform mod4 (no stripes)",
              spread < 2.0, f"mod4 means {[round(b, 2) for b in by4]}")
    else:
        print("NOTE: no overlay pixels in dumps — composite check skipped")

    print(f"\n{'ALL CHECKS PASSED' if fails == 0 else str(fails) + ' CHECK(S) FAILED'}")
    return 0 if fails == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
