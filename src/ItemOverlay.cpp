// ItemOverlay.cpp
// See ItemOverlay.h for the naming scheme and why this exists.
//
// Structure deliberately mirrors MenuPortrait.cpp and AnimOrbs.cpp: same stb_image
// decode, same D3DFMT_A8R8G8B8 texture, same full state-block save/restore around an
// XYZRHW quad drawn with no pixel shader.

#include "stdafx.h"
#include "ItemOverlay.h"

#include <d3d9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;

// NOT LoadFile(). LoadFile -> File_Open(..., FIVE_TRY) and a miss there ends in the
// "The Hell was unable to open a required file" box, which is fatal for probing an
// optional file. ONE_TRY returns 0 quietly when the file simply is not there.
int   __fastcall File_Open(const char* fileName, HANDLE* fileHandle, int oneTry);
DWORD __fastcall File_GetSize(HANDLE aHandle, LPDWORD lpFileSizeHigh);
int   __fastcall File_Read(HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead);
int   __fastcall File_Close(HANDLE aHandle);

// The 8-bit frame and the software cursor. WorkingSurface still holds the composited
// cursor at this point -- our quads only wrote the back buffer -- so the cursor can be
// lifted back out of it and re-drawn on top. See DrawCursorPatch below.
extern uchar*        WorkingSurface;
extern int           WorkingWidth;
extern int           CurCursorStartX, CurCursorStartY, CurCursorWidth, CurCursorHeight;
extern uchar         CursorImage[];
extern PALETTEENTRY  ScreenPalette[];

// stb_image, instantiated in AnimOrbs.cpp. Its STBI_ONLY_* list there decides which
// of the extensions below actually decode.
extern "C" {
    unsigned char* stbi_load_from_memory(unsigned char const* buffer, int len,
                                         int* x, int* y, int* comp, int req_comp);
    void           stbi_image_free(void* retval_from_stbi_load);
}

namespace {

    struct TransformedVertexT { float x, y, z, rhw; float u, v; };

    // CEL_Draw's y is the BOTTOM row of the sprite, so the quad's top edge is
    // y - h + 1. If converted icons ever sit one pixel off the CEL underneath,
    // this is the knob -- not the call sites.
    const int kYAdjust = 1;

    // ---- decoded art --------------------------------------------------------
    // Item icons are small (the largest inventory sprite is 2x3 cells), so unlike
    // the portrait strips these are kept as one texture each and the decoded RGBA
    // is retained too -- that way a device reset costs a re-upload, not a re-read
    // from the archive.
    enum { kMaxSprites = 512 };     // distinct converted icons resident at once
    enum { kMaxQueue   = 512 };     // icons drawn in a single frame (128 was below stash 10x13 + inventory + belt + body + cursor;
                                    // the cursor is queued last, so a full stash made the dragged item vanish)

    struct Sprite {
        int                gfx;     // pre-GetItemSprites id, the file name
        uchar*             rgba;    // w*h*4, or null once we know it is absent
        int                w, h;
        IDirect3DTexture9* tex;
        bool               tried;   // absent files are looked up exactly once
        // (th4) Animated icons: N_1.png .. N_k.png instead of N.png. rgba/tex above are
        // frame 1 (also used for the outline and the cursor alpha); the rest live here.
        int                 frameCount;   // 1 for a static icon
        uchar**             frameRgba;    // [frameCount], frameRgba[0] == rgba
        IDirect3DTexture9** frameTex;     // [frameCount], uploaded lazily, frameTex[0] == tex
    };
    enum { kAnimFps = 25, kMaxFrames = 256 };
    Sprite gSprites[kMaxSprites];
    int    gSpriteCount = 0;

    // (th4) cover*: the cell box this entry occupies, in CLIENT space, plus a snapshot of
    // the 8-bit surface taken the moment the entry was queued. Anything that differs at
    // flush time was painted by a panel drawn AFTER the item -- see CoverPatch().
    // (th4) screenRed: how many DrawRedScreen passes ran over the surface AFTER this entry
    // was queued (0 normally, 1 on pause/death/Terminator Seeing, 2 if two of those land in
    // one frame). See ItemOverlay_OnRedScreen.
    struct Entry { int gfx, x, y, w, h; bool mirrored; int outline; bool onCursor; bool ghost; bool red;
                   uchar screenRed;
                   int coverX, coverY, coverW, coverH; const uchar* coverSnap; };   // (th4) ghost: 50% checkerboard, like CEL_DrawShadow/R_TRANS
    Entry gQueue[kMaxQueue];
    int   gQueueCount = 0;

    // (th4) THE QUEUE HAS TO SURVIVE A PALETTE FADE.
    //
    // These icons are drawn from a queue the PANEL CODE fills every frame. A palette
    // fade (warp, level change, ESC menu) does not redraw anything: PaletteFadeIn/Out
    // just call SetFadeLevel + DrawNotGameplayScreen, which re-converts the SAME 8-bit
    // WorkingSurface through the new palette. So the 8-bit items fade smoothly while
    // nothing re-queues the replacement art -- gQueueCount is 0, this function returns
    // early, and the icons vanish in a single frame instead of fading. On arrival they
    // reappear only once normal drawing resumes, i.e. after the 8-bit fade-in is done.
    // That is exactly what the report described, and why a brightness factor alone
    // changed nothing.
    //
    // So the last real queue is retained and replayed on any frame that queued nothing
    // while the surface underneath is unchanged. Cleared as soon as a frame queues
    // normally again, and by ItemOverlay_Clear, so it can never show stale art.
    Entry gHeldQueue[kMaxQueue];
    int   gHeldCount = 0;
    bool  gHeldCursorIsItem = false;
    bool  gFrameComposed = false;    // set by MainDrawFunc when the panels were redrawn
    bool  gCursorIsItem = false;   // set for the frame when the cursor is a dragged item

    // Rectangles DrawBoxTransparent darkened this frame, in screen space.
    enum { kMaxDim = 24 };  // (th4) raised from 8: belt digit claims add up to 8 rects on top of tooltips/panels
    struct DimRect { int x, y, w, h; float factor; const uchar* snap; };
    DimRect gDim[kMaxDim];
    int     gDimCount = 0;

    // DimRect::factor is how dark the icons go under that box. DrawBoxTransparent does not
    // scale a colour, it remaps each palette index into the bottom quarter of its own
    // 16-step ramp -- (v & 0xF0) + ((v & 0x0F) >> 2) + 12 -- and no exact multiplier
    // reproduces that on true colour, so the callers pass a match by eye: 0.35f for a
    // tooltip, 0.50f for the trade panel, 1.0f where nothing should darken at all.

    // Pixels of each darkened box as it looked BEFORE its text was written. Anything that
    // differs later is text and gets re-drawn over the icons. One shared arena rather than
    // per-rect allocation: this runs every frame and must not touch the heap.
    // (th4) Raised for the per-item cover snapshots. A stash or trade screen can queue
    // well over a hundred icons and each claims its cell box (a 2x3 item is 56x84 =
    // 4.7 KB), on top of the tooltip and belt-digit claims that already lived here.
    // Running out is not fatal -- SnapshotRect returns nullptr and that item simply
    // keeps the old behaviour -- but at 320 KB it would have run out constantly.
    enum { kSnapArena = 2048 * 1024 };
    uchar gSnap[kSnapArena];
    int   gSnapUsed = 0;

    // (th4) Grab w*h pixels of WorkingSurface into the shared arena. x/y are CLIENT
    // space, matching DimRect, so the borders go back on here. Returns nullptr when the
    // arena is full, and every caller treats that as "no patch" -- degrades to the old
    // behaviour rather than misdrawing.
    const uchar* SnapshotRect(int x, int y, int w, int h)
    {
        if (w <= 0 || h <= 0) return nullptr;
        const size_t need = (size_t)w * h;
        if (gSnapUsed + need > (size_t)kSnapArena) return nullptr;
        uchar* dst = gSnap + gSnapUsed;
        const int LB = (int)Screen_LeftBorder;
        const int TB = (int)Screen_TopBorder;
        for (int row = 0; row < h; ++row) {
            uchar* out = dst + (size_t)row * w;
            const int sy = y + row + TB;
            if (sy < 0 || sy >= (int)WorkingHeight) { memset(out, 0, w); continue; }
            const uchar* srcRow = WorkingSurface + (size_t)WorkingWidth * sy;
            for (int col = 0; col < w; ++col) {
                const int sx = x + col + LB;
                out[col] = (sx < 0 || sx >= (int)WorkingWidth) ? 0 : srcRow[sx];
            }
        }
        gSnapUsed += (int)need;
        return dst;
    }

    IDirect3DTexture9* gTextTex = nullptr;
    int gTextTexW = 0, gTextTexH = 0;

    IDirect3DTexture9* gOutlineTex = nullptr;
    int gOutlineTexW = 0, gOutlineTexH = 0;


    // (th4) One-entry memo in front of the scan. Load() is called several times for the same
    // icon within a frame -- once to draw it, once for its hover outline, and again for the
    // dragged item's rect -- so the immediate repeat is the common case by a distance. The
    // index is only ever a hint: it is re-checked against gfx, and gSprites entries are never
    // moved or removed while the game runs, so a stale hint can only miss, never mislead.
    int gFindHint = -1;

    Sprite* Find(int gfx)
    {
        if (gFindHint >= 0 && gFindHint < gSpriteCount && gSprites[gFindHint].gfx == gfx) {
            return &gSprites[gFindHint];
        }
        for (int i = 0; i < gSpriteCount; ++i) {
            if (gSprites[i].gfx == gfx) { gFindHint = i; return &gSprites[i]; }
        }
        return nullptr;
    }

    // Read one image file from the archive/disk into RGBA. Null if absent or undecodable.
    uchar* LoadImageFile(const char* path, int& w, int& h)
    {
        HANDLE fh = 0;
        if (!File_Open(path, &fh, 1) || !fh) return nullptr;
        const DWORD bytes = File_GetSize(fh, nullptr);
        if (bytes == 0 || bytes == 0xFFFFFFFFu) { File_Close(fh); return nullptr; }

        uchar* raw = (uchar*)malloc(bytes);
        if (!raw) { File_Close(fh); return nullptr; }
        File_Read(fh, raw, bytes);
        File_Close(fh);

        int comp = 0;
        uchar* px = stbi_load_from_memory(raw, (int)bytes, &w, &h, &comp, 4);
        free(raw);
        if (px && (w <= 0 || h <= 0)) { stbi_image_free(px); px = nullptr; }
        return px;
    }

    // Decode data\items\<gfx>.png, falling back to jpg/bmp/tga. One attempt ever,
    // hit or miss -- a missing icon must not cost a file probe every frame.
    Sprite* Load(int gfx)
    {
        // (th4) DDRAW MODE HAS NO OVERLAY. With config "DDraw = on" the frame is presented
        // through DirectDraw (dx.cpp ~383, dx_utility.cpp ~1879) and ItemOverlay_DrawD3D is
        // never called, so any icon routed here would simply be missing: ItemCEL_Draw skips
        // the CEL whenever replacement art exists. Reporting "no art" in that mode puts every
        // caller back on the stock path -- ItemCEL_Draw draws the CEL, the cursor draws its
        // 8-bit sprite (Draw.cpp ~6091 via ItemOverlay_HasReplacementArt), R_RED tints it --
        // with no change to any call site. Every other public entry point goes through this.
        // DDraw is read once from the config at startup (Config.cpp ~382) and never changes
        // at run time, so the Find() cache below cannot hold a stale answer.
        // AnimOrbs.cpp and MenuPortrait.cpp are separate true-colour systems and are not
        // affected by this gate.
        if (DDraw) return nullptr;
        if (Sprite* s = Find(gfx)) return s->rgba ? s : nullptr;
        if (gSpriteCount >= kMaxSprites) return nullptr;

        Sprite& s = gSprites[gSpriteCount++];
        memset(&s, 0, sizeof(s));
        s.gfx = gfx;
        s.tried = true;

        // Split the composite id back into (sheet, number) the same way GetItemSprites
        // does, so the file name is the number written in BaseItems.cpp -- the part
        // BEFORE the "+168" or "+3885" -- rather than the summed runtime value.
        //
        //   BaseItems "N+168"   -> runtime N+180  -> th2\\N
        //   BaseItems "N+3885"  -> runtime N+3897 -> th3\\N
        //   BaseItems "N"       -> runtime N+12   -> inv1\\N
        //
        // (runtime = the table value + CM_12_ITEMS_PICS_START.)
        const char* folder;
        int number;
        if (gfx <= 179)       { folder = "inv1"; number = gfx - 12; }
        else if (gfx <= 3896) { folder = "th2";  number = gfx - 180; }
        else                  { folder = "th3";  number = gfx - 3897; }
        if (number < 0) return nullptr;

        static const char* kExt[] = { "png", "jpg", "bmp", "tga" };
        const int kExtCount = (int)(sizeof(kExt) / sizeof(kExt[0]));
        for (int e = 0; e < kExtCount && !s.rgba; ++e) {
            char path[160];
            sprintf(path, "data\\items\\%s\\%d.%s", folder, number, kExt[e]);
            s.rgba = LoadImageFile(path, s.w, s.h);   // released by ItemOverlay_Shutdown()
        }

        // (th4) No static file: try an animation in its own folder, N\\N_1 .. N\\N_k,
        // counting until one is missing. All frames must match frame 1's size; a
        // mismatch ends the sequence.
        if (!s.rgba) {
            uchar* frames[kMaxFrames];
            int n = 0;
            for (; n < kMaxFrames; ++n) {
                uchar* px = nullptr; int w = 0, h = 0;
                for (int e = 0; e < kExtCount && !px; ++e) {
                    char path[160];
                    sprintf(path, "data\\items\\%s\\%d\\%d_%d.%s", folder, number, number, n + 1, kExt[e]);
                    px = LoadImageFile(path, w, h);
                }
                if (!px) break;
                if (n > 0 && (w != s.w || h != s.h)) { stbi_image_free(px); break; }
                if (n == 0) { s.w = w; s.h = h; }
                frames[n] = px;
            }
            if (n > 0) {
                s.rgba = frames[0];
                s.frameCount = n;
                s.frameRgba = (uchar**)malloc(sizeof(uchar*) * n);
                s.frameTex  = (IDirect3DTexture9**)calloc(n, sizeof(IDirect3DTexture9*));
                if (s.frameRgba && s.frameTex) {
                    memcpy(s.frameRgba, frames, sizeof(uchar*) * n);
                } else {                                  // out of memory: keep it static
                    for (int i = 1; i < n; ++i) stbi_image_free(frames[i]);
                    free(s.frameRgba); free(s.frameTex);
                    s.frameRgba = nullptr; s.frameTex = nullptr; s.frameCount = 1;
                }
            }
        }
        if (s.rgba && s.frameCount == 0) s.frameCount = 1;
        return s.rgba ? &s : nullptr;
    }


    // Where the replacement art lands, in screen space: exactly the CEL frame's rect.
    //
    // Deliberately dumb. Earlier revisions tried to be clever about the frame's padding --
    // scaling to the artwork's ink box, then centring on it -- and every heuristic that
    // helped one item hurt another, because some frames pad above their art and some below.
    // A 1:1 map onto the frame is predictable, and it puts placement where it belongs: in
    // the PNG. To shift art down, add transparent rows at the top of the canvas.
    void FitRect(const Entry& e, const Sprite& sp, int& fx, int& fy, int& fw, int& fh)
    {
        (void)sp;
        // Snap to the CELL GRID, not to the measured frame. ItemWidthTable/ItemHeightTable
        // are the CEL frame's real extent, which is routinely cells*28 + 1 (see the
        // commented-out assertion in Main.cpp) and can carry padding besides. Art authored
        // at the natural 28-per-cell size would then be stretched over that odd extent.
        // Rounding to whole cells and anchoring at the frame's bottom-left maps a 56x84
        // PNG onto a 2x3 item pixel for pixel, with no scaling at all.
        const int cellsW = (e.w + CELL_SIZE / 2) / CELL_SIZE;
        const int cellsH = (e.h + CELL_SIZE / 2) / CELL_SIZE;
        const int boxW = (cellsW > 0 ? cellsW : 1) * CELL_SIZE;
        const int boxH = (cellsH > 0 ? cellsH : 1) * CELL_SIZE;
        const int boxX = e.x - (int)Screen_LeftBorder;
        const int boxY = e.y - boxH + kYAdjust - (int)Screen_TopBorder;

        // Never stretch. Art is drawn at its own pixel size and centred in the cell box,
        // so a 38x38 PNG sits centred in a 2x2 slot and a 56x84 fills a 2x3 exactly.
        // Only art LARGER than the box is scaled, and then only down, aspect preserved,
        // because there is nowhere else for it to go.
        fw = sp.w > 0 ? sp.w : boxW;
        fh = sp.h > 0 ? sp.h : boxH;
        if (fw > boxW || fh > boxH) {
            if ((long long)fw * boxH >= (long long)fh * boxW) {   // width is the tighter fit
                fh = (int)(((long long)fh * boxW + fw / 2) / fw);
                fw = boxW;
            } else {
                fw = (int)(((long long)fw * boxH + fh / 2) / fh);
                fh = boxH;
            }
            if (fw < 1) fw = 1;
            if (fh < 1) fh = 1;
        }
        fx = boxX + (boxW - fw) / 2;
        fy = boxY + (boxH - fh) / 2;
    }

    IDirect3DTexture9* UploadRgba(IDirect3DDevice9* dev, const uchar* rgba, int w, int h)
    {
        IDirect3DTexture9* tex = nullptr;
        if (D3D_OK != dev->CreateTexture(w, h, 1, D3DUSAGE_DYNAMIC,
                                         D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, nullptr)) {
            return nullptr;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != tex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) {
            tex->Release();
            return nullptr;
        }
        // stb gives RGBA; D3DFMT_A8R8G8B8 is BGRA in memory. Same swap AnimOrbs does.
        for (int y = 0; y < h; ++y) {
            const uchar* src = rgba + ((size_t)y * w) * 4;
            uchar*       dst = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
            for (int x = 0; x < w; ++x) {
                dst[0] = src[2]; dst[1] = src[1]; dst[2] = src[0]; dst[3] = src[3];
                src += 4; dst += 4;
            }
        }
        tex->UnlockRect(0);
        return tex;
    }

    // Upload (or re-upload after a device reset) one icon -- frame 1 only.
    bool Upload(IDirect3DDevice9* dev, Sprite& s)
    {
        if (s.tex) return true;
        if (!dev || !s.rgba) return false;
        s.tex = UploadRgba(dev, s.rgba, s.w, s.h);
        if (s.tex && s.frameTex) s.frameTex[0] = s.tex;
        return s.tex != nullptr;
    }

    // (th4) Texture to draw this frame. Static icons: the one texture. Animated: the frame
    // picked by wall-clock time at kAnimFps, uploaded on first use. Falls back to frame 1.
    IDirect3DTexture9* FrameTex(IDirect3DDevice9* dev, Sprite& s)
    {
        if (s.frameCount <= 1 || !s.frameRgba || !s.frameTex) return s.tex;
        const int f = (int)((GetTickCount() * (DWORD)kAnimFps / 1000u) % (DWORD)s.frameCount);
        if (!s.frameTex[f]) s.frameTex[f] = UploadRgba(dev, s.frameRgba[f], s.w, s.h);
        return s.frameTex[f] ? s.frameTex[f] : s.tex;
    }

    // ---- cursor patch -------------------------------------------------------
    IDirect3DTexture9* gCursorTex = nullptr;
    int gCursorTexW = 0, gCursorTexH = 0;

    bool RectsOverlap(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
    {
        return ax < bx + bw && ax + aw > bx &&
               ay < by + bh && ay + ah > by;
    }

    // Our item quads are drawn onto the back buffer AFTER the 8-bit frame was uploaded,
    // so they land on top of the software cursor that was composited into that frame --
    // hovering an item buried the hand under the icon. Lift the cursor back out of
    // WorkingSurface and re-draw it above the icons.
    //
    // CursorImage holds the background saved BEFORE the cursor was stamped, so a pixel
    // that differs from it is cursor and a pixel that matches is background. That gives
    // an exact mask without needing the cursor CEL. Same technique AnimOrbs uses for the
    // orbs; kept separate here because this pass has to run after the item quads.
    void DrawCursorPatch(IDirect3DDevice9* dev, const Entry* queue, int count)
    {
        const int cw = CurCursorWidth;
        const int ch = CurCursorHeight;
        if (cw <= 0 || ch <= 0) return;

        const int cx = CurCursorStartX;      // screen space, borders already excluded
        const int cy = CurCursorStartY;

        // Only if it actually sits over something we drew.
        bool over = false;
        for (int i = 0; i < count && !over; ++i) {
            const Entry& e = queue[i];
            over = RectsOverlap(cx, cy, cw, ch,
                                e.x - (int)Screen_LeftBorder,
                                e.y - e.h + kYAdjust - (int)Screen_TopBorder,
                                e.w, e.h);
        }
        if (!over) return;

        if (!gCursorTex || gCursorTexW < cw || gCursorTexH < ch) {
            if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; }
            if (D3D_OK != dev->CreateTexture(cw, ch, 1, D3DUSAGE_DYNAMIC,
                                             D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                             &gCursorTex, nullptr)) return;
            gCursorTexW = cw; gCursorTexH = ch;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != gCursorTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

        const int LB = (int)Screen_LeftBorder;
        const int TB = (int)Screen_TopBorder;
        uchar* dstBase = (uchar*)lr.pBits;
        const uchar* palBytes = (const uchar*)ScreenPalette;

        for (int y = 0; y < ch; ++y) {
            // (th4) Clamped, as in SnapshotDimRect and DrawTextPatch: the cursor can sit
            // partly off-screen, and an unclamped row read walks off WorkingSurface.
            const int sy = cy + y + TB;
            const bool rowOk = (sy >= 0 && sy < (int)WorkingHeight);
            const uchar* srcRow = rowOk ? WorkingSurface + (size_t)WorkingWidth * sy : nullptr;
            const uchar* bgRow  = CursorImage + (size_t)y * cw;   // packed, cw wide
            uchar* dstRow = dstBase + (size_t)y * lr.Pitch;
            for (int x = 0; x < cw; ++x) {
                const int sx = cx + x + LB;
                if (!rowOk || sx < 0 || sx >= (int)WorkingWidth) {
                    dstRow[3] = 0; dstRow += 4; continue;         // off-screen: nothing to lift
                }
                const uchar idx = srcRow[sx];
                const uchar* p = palBytes + (size_t)idx * sizeof(PALETTEENTRY);
                dstRow[0] = p[2]; dstRow[1] = p[1]; dstRow[2] = p[0];
                dstRow[3] = (idx != bgRow[x]) ? 255 : 0;          // cursor vs background
                dstRow += 4;
            }
        }
        gCursorTex->UnlockRect(0);

        const float left   = (float)cx - 0.5f;
        const float top    = (float)cy - 0.5f;
        const float right  = left + cw;
        const float bottom = top + ch;
        const float u1 = (float)cw / (float)gCursorTexW;
        const float v1 = (float)ch / (float)gCursorTexH;

        TransformedVertexT v[4] = {
            { left,  top,    0.0f, 1.0f, 0.0f, 0.0f },
            { right, top,    0.0f, 1.0f, u1,   0.0f },
            { left,  bottom, 0.0f, 1.0f, 0.0f, v1  },
            { right, bottom, 0.0f, 1.0f, u1,   v1  },
        };
        dev->SetTexture(0, gCursorTex);
        // Full brightness on purpose, NOT SetIconFactor: this patch is lifted from
        // WorkingSurface through ScreenPalette, so the fade is already baked into its
        // pixels. Scaling it again would darken the cursor twice during a transition.
        dev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));  // last icon may have left it dimmed
        dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));
    }

    // (th4) Screen rect of the dragged item's 8-bit CEL, if an item is being dragged. Used to
    // keep that CEL out of the patch pass below -- RenderCursor draws it after Menu_Draw, so
    // inside a claimed rect it would otherwise count as ink and be painted over the
    // replacement art.
    //
    // Deliberately NOT FitRect: that returns where the PNG lands, which is snapped to the
    // cell grid and then centred, so a 38x38 PNG in a 2x2 slot sits inside a 56x56 box. The
    // CEL occupies e.w x e.h anchored at the entry, and it is the CEL that has to be masked.
    // Same expression DrawCursorPatch uses to bound an entry.
    bool CursorItemRect(int& cx, int& cy, int& cw, int& ch)
    {
        for (int i = 0; i < gQueueCount; ++i) {
            const Entry& e = gQueue[i];
            if (!e.onCursor) continue;
            if (!Load(e.gfx)) return false;          // no replacement art: leave the CEL alone
            cx = e.x - (int)Screen_LeftBorder;
            cy = e.y - e.h + kYAdjust - (int)Screen_TopBorder;
            cw = e.w;
            ch = e.h;
            return true;
        }
        return false;
    }

    // Re-draw whatever was written into a darkened box after we snapshotted it -- the
    // tooltip text and its border. Same comparison trick as the cursor: a pixel that
    // differs from the snapshot is new ink and becomes opaque, one that matches is
    // background and becomes transparent, so the icon still shows through around the
    // letters. Runs after the icons, so the text ends up on top where it belongs.
    void DrawTextPatch(IDirect3DDevice9* dev, const DimRect& d)
    {
        if (!d.snap || d.w <= 0 || d.h <= 0) return;

        // (th4) PERFORMANCE: find out whether anything differs from the snapshot BEFORE
        // touching the texture. CoverPatch calls this for every queued item icon on every
        // rendered frame, and for almost all of them nothing was painted over the cell --
        // the lock, per-pixel conversion and upload below used to run anyway and were then
        // thrown away by the "if (!any) return" after them (7.5% of CPU in a profile with
        // the inventory open). The scan uses the same bounds as the fill loop; the dragged
        // item's footprint is not excluded here, so a cell under the cursor item still goes
        // through the full path, where that exclusion is applied.
        {
            const int LB0 = (int)Screen_LeftBorder;
            const int TB0 = (int)Screen_TopBorder;
            bool differs = false;
            for (int y = 0; y < d.h && !differs; ++y) {
                const int sy = d.y + y + TB0;
                if (sy < 0) continue;
                int x0 = 0, x1 = d.w;						// clip to the surface like the fill loop
                if (d.x + LB0 < 0) x0 = -(d.x + LB0);
                if (d.x + LB0 + x1 > (int)WorkingWidth) x1 = (int)WorkingWidth - (d.x + LB0);
                if (x1 <= x0) continue;
                const uchar* srcRow  = WorkingSurface + (size_t)WorkingWidth * sy + d.x + LB0;
                const uchar* snapRow = d.snap + (size_t)y * d.w;
                if (memcmp(srcRow + x0, snapRow + x0, (size_t)(x1 - x0)) != 0) differs = true;
            }
            if (!differs) return;
        }

        if (!gTextTex || gTextTexW < d.w || gTextTexH < d.h) {
            if (gTextTex) { gTextTex->Release(); gTextTex = nullptr; }
            if (D3D_OK != dev->CreateTexture(d.w, d.h, 1, D3DUSAGE_DYNAMIC,
                                             D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                             &gTextTex, nullptr)) return;
            gTextTexW = d.w; gTextTexH = d.h;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != gTextTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

        const int LB = (int)Screen_LeftBorder;
        const int TB = (int)Screen_TopBorder;
        const uchar* palBytes = (const uchar*)ScreenPalette;
        bool any = false;

        // (th4) The dragged item's own 8-bit CEL is drawn by RenderCursor, which runs AFTER
        // Menu_Draw -- so inside a claimed rect it counts as new ink and would be painted
        // straight back over the replacement art. Skip its footprint: the PNG version is
        // drawn after this pass anyway.
        int cx = 0, cy = 0, cw = 0, ch = 0;
        const bool haveCursorItem = CursorItemRect(cx, cy, cw, ch);

        for (int y = 0; y < d.h; ++y) {
            const int sy = d.y + y + TB;
            if (sy < 0) continue;
            const uchar* srcRow  = WorkingSurface + (size_t)WorkingWidth * sy;
            const uchar* snapRow = d.snap + (size_t)y * d.w;
            uchar* dstRow = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
            for (int x = 0; x < d.w; ++x) {
                const int sx = d.x + x + LB;
                if (sx < 0 || sx >= (int)WorkingWidth) { dstRow[3] = 0; dstRow += 4; continue; }
                const uchar idx = srcRow[sx];
                const uchar* p = palBytes + (size_t)idx * sizeof(PALETTEENTRY);
                dstRow[0] = p[2]; dstRow[1] = p[1]; dstRow[2] = p[0];
                bool ink = (idx != snapRow[x]);
                if (ink && haveCursorItem) {
                    const int px = d.x + x, py = d.y + y;      // screen space, as FitRect gives
                    if (px >= cx && px < cx + cw && py >= cy && py < cy + ch) ink = false;
                }
                dstRow[3] = ink ? 255 : 0;
                if (ink) any = true;
                dstRow += 4;
            }
        }
        gTextTex->UnlockRect(0);
        if (!any) return;

        const float left   = (float)d.x - 0.5f;
        const float top    = (float)d.y - 0.5f;
        const float right  = left + d.w;
        const float bottom = top + d.h;
        const float u1 = (float)d.w / (float)gTextTexW;
        const float v1 = (float)d.h / (float)gTextTexH;

        TransformedVertexT v[4] = {
            { left,  top,    0.0f, 1.0f, 0.0f, 0.0f },
            { right, top,    0.0f, 1.0f, u1,   0.0f },
            { left,  bottom, 0.0f, 1.0f, 0.0f, v1  },
            { right, bottom, 0.0f, 1.0f, u1,   v1  },
        };
        dev->SetTexture(0, gTextTex);
        // Full brightness on purpose, as in DrawCursorPatch: already faded via ScreenPalette.
        dev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
        dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));
    }

    // (th4) Draw one quad, dimmed wherever a tooltip box overlaps it.
    //
    // The 8-bit path gets this for free: DrawBoxTransparent darkens WorkingSurface, so a CEL
    // sprite AND its outline go dark together. Our quads are drawn after that pass and know
    // nothing about it, so the overlap is handled here instead -- the same quad is drawn
    // several times, each scissored to one band, dimmed only under the box. Cheaper than
    // splitting the geometry and it keeps the UVs correct for free.
    // (th4) Re-draw whatever a later panel painted over this item's cell box, on top of
    // the icon. Same machinery as the tooltip/menu patch -- DrawTextPatch keys on
    // "differs from the snapshot" -- but scoped to one item instead of a claimed rect,
    // which is what makes it per pixel and therefore honest about tile gaps.
    void CoverPatch(IDirect3DDevice9* dev, const Entry& e)
    {
        if (!e.coverSnap) return;
        DimRect d;
        d.x = e.coverX; d.y = e.coverY; d.w = e.coverW; d.h = e.coverH;
        d.factor = 1.0f;            // nothing here should darken; this is a straight put-back
        d.snap = e.coverSnap;
        DrawTextPatch(dev, d);
    }

    // (th4) FOLLOW THE PALETTE FADE.
    //
    // Replacement icons are true-colour quads drawn outside the palette, so a warp or
    // level change used to make them vanish in one frame while the 8-bit items around
    // them faded out over 32 steps -- and on arrival they popped back at full brightness
    // before the 8-bit ones had finished coming up. Only visible with vsync on, because
    // SetFadeLevel waits a vblank per step only when windowed with vsync; otherwise the
    // whole fade is over inside a frame. Same cause as the life/mana orbs.
    //
    // gFadeMul is PaletteFadeLevel for this frame, 0..255. It multiplies every
    // TEXTUREFACTOR the icon quads use, so it composes with tooltip dimming instead of
    // fighting it. The cursor and text PATCHES do NOT take it: those are lifted from
    // WorkingSurface through ScreenPalette and are already faded -- scaling them again
    // would darken them twice.
    DWORD gFadeMul = 255;

    inline void SetIconFactor(IDirect3DDevice9* dev, DWORD lvl)
    {
        const DWORD v = (lvl * gFadeMul + 127) / 255;
        dev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, v, v, v));
    }

    void DrawQuadDimmed(IDirect3DDevice9* dev, const TransformedVertexT* v,
                        int ix, int iy, int iw, int ih)
    {
        // Only the first overlapping box is considered: two tooltips stacked on one icon
        // does not happen in practice.
        const DimRect* hit = nullptr;
        int cl = 0, ct = 0, cr = 0, cb = 0;      // the overlapping part, screen space
        for (int d = 0; d < gDimCount && !hit; ++d) {
            const DimRect& r = gDim[d];
            // (th4) factor ~1.0 rects are pure ink claims (belt digits, gold split panel):
            // they don't dim, so they must not shadow a real dim box overlapping the same icon.
            if (r.factor >= 0.999f) continue;
            const int l = ix > r.x ? ix : r.x;
            const int t = iy > r.y ? iy : r.y;
            const int rr = (ix + iw < r.x + r.w) ? ix + iw : r.x + r.w;
            const int bb = (iy + ih < r.y + r.h) ? iy + ih : r.y + r.h;
            if (l < rr && t < bb) { hit = &r; cl = l; ct = t; cr = rr; cb = bb; }
        }

        if (!hit) {
            SetIconFactor(dev, 255);
            dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));
            return;
        }

        const DWORD dim = (DWORD)(hit->factor * 255.0f + 0.5f);
        struct Band { int l, t, r, b; DWORD lvl; } bands[5];
        int bandCount = 0;
        if (ct > iy)       bands[bandCount++] = { ix, iy, ix + iw, ct, 255 };             // above
        if (cb < iy + ih)  bands[bandCount++] = { ix, cb, ix + iw, iy + ih, 255 };        // below
        if (cl > ix)       bands[bandCount++] = { ix, ct, cl, cb, 255 };                  // left
        if (cr < ix + iw)  bands[bandCount++] = { cr, ct, ix + iw, cb, 255 };             // right
        bands[bandCount++] = { cl, ct, cr, cb, dim };                                      // under the box

        dev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
        for (int b = 0; b < bandCount; ++b) {
            RECT sc = { bands[b].l, bands[b].t, bands[b].r, bands[b].b };
            if (sc.left < 0) sc.left = 0;            // D3D rejects a scissor outside the target
            if (sc.top  < 0) sc.top  = 0;
            if (sc.right <= sc.left || sc.bottom <= sc.top) continue;
            dev->SetScissorRect(&sc);
            SetIconFactor(dev, bands[b].lvl);
            dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));
        }
        dev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    }

    // (th4) RGBA of the frame being shown right now (frame 1 for static icons).
    const uchar* CurrentFrameRgba(const Sprite& s)
    {
        if (s.frameCount > 1 && s.frameRgba && s.frameTex) {
            const int f = (int)((GetTickCount() * (DWORD)kAnimFps / 1000u) % (DWORD)s.frameCount);
            if (s.frameRgba[f]) return s.frameRgba[f];
        }
        return s.rgba;
    }

    // (th4) DRAWN THIS FRAME, for ItemOverlay_MarkUi (bottom of this file): every icon quad ItemOverlay_DrawD3D put on
    // the screen -- its screen rect (game px, as FitRect gives it) and the art it showed. Emptied at the start of each
    // ItemOverlay_DrawD3D, so it holds exactly the current frame's quads when PostFxPresent builds its masks.
    struct Drawn { const uchar* rgba; int sw, sh, x, y, w, h; bool mirrored, outlined; };
    Drawn gDrawn[kMaxQueue];
    int   gDrawnCount = 0;
    void NoteDrawn(const Sprite& s, const Entry& e, int ix, int iy, int iw, int ih)
    {
        if (gDrawnCount >= kMaxQueue || iw <= 0 || ih <= 0 || s.w <= 0 || s.h <= 0) return;
        gDrawn[gDrawnCount++] = { CurrentFrameRgba(s), s.w, s.h, ix, iy, iw, ih, e.mirrored, e.outline >= 0 };
    }

    // (th4) PALETTE TRANSFORMS FOR REPLACEMENT ART -- the requirement-not-met red (R_RED)
    // and the full-screen red of pause / death / Terminator Seeing (DrawRedScreen).
    //
    // Both are palette-index remaps in the 8-bit engine:
    //   R_RED        RleDraw does `IF( draw & R_RED ) pal = &LightTable[256*16]` (engine.h
    //                ~73): every index of the sprite goes through LightTable row 16.
    //   DrawRedScreen (Panel.cpp ~2384) XLATs the WHOLE WorkingSurface through one
    //                LightTable row (row 18, or row 6 with Light4Levels); in Hell levels
    //                indices below 0x20 are left alone. Everything already on the surface
    //                -- 8-bit item sprites included -- goes red with it.
    // Replacement art is true colour and has no index to push, so reproduce the SAME
    // transforms rather than inventing a red of our own: for a given RGB find the palette
    // entry it came from, run that index through the same rows in the same order the 8-bit
    // path does (R_RED at draw time, then each red-screen pass), and take the RGB back out.
    // A converted item and an unconverted one in the next slot then look identical, and
    // both follow the tables if they ever change.
    //
    // The nearest-entry search (the 128 item entries, see BuildIdxCube) is far too much per pixel per frame, so
    // it is folded once into a 32x32x32 RGB -> index cube. The remap rows are read live
    // per pixel (two byte lookups), so only the PALETTE can make the cube stale: it is
    // rebuilt when an FNV stamp over ScreenPalette changes (level load, Hell row cycling).
    uchar gIdxCube[32 * 32 * 32];
    bool  gIdxCubeValid = false;
    uint  gIdxCubeStamp = 0;

    uint PaletteStamp()
    {
        uint h = 2166136261u;
        for (int i = 0; i < 256; ++i) {
            h = (h ^ ScreenPalette[i].peRed)   * 16777619u;
            h = (h ^ ScreenPalette[i].peGreen) * 16777619u;
            h = (h ^ ScreenPalette[i].peBlue)  * 16777619u;
        }
        return h;
    }

    void BuildIdxCube()
    {
        const uint stamp = PaletteStamp();
        if (gIdxCubeValid && stamp == gIdxCubeStamp) return;
        for (int r = 0; r < 32; ++r)
        for (int g = 0; g < 32; ++g)
        for (int b = 0; b < 32; ++b) {
            const int R = r * 8 + 4, G = g * 8 + 4, B = b * 8 + 4;
            int best = 128, bestD = 1 << 30;
            // (th4) ITEM RANGE ONLY: 128..255. 8-bit item art (objcurs) draws only from the fixed
            // upper half of the palette -- 0..127 is the per-level half and changes with every
            // dungeon type -- and the remap rows are built for that half: row 16 is Infra.TRN
            // (loaded in MakeLightTable, Lights.cpp ~738), written for the item ramps; no item art
            // ever depends on what it does to 0..127. Matching against all 256 let dark or purple
            // art snap to level colours, and a converted item that failed its requirement showed
            // only the red outline instead of going red (seen on a dark purple belt).
            for (int i = 128; i < 256; ++i) {
                const int dr = R - (int)ScreenPalette[i].peRed;
                const int dg = G - (int)ScreenPalette[i].peGreen;
                const int db = B - (int)ScreenPalette[i].peBlue;
                const int d = dr * dr + dg * dg + db * db;
                if (d < bestD) { bestD = d; best = i; }
            }
            gIdxCube[((r * 32) + g) * 32 + b] = (uchar)best;
        }
        gIdxCubeValid = true;
        gIdxCubeStamp = stamp;
    }

    // (th4) The red-screen row of the current frame, recorded by ItemOverlay_OnRedScreen.
    // All DrawRedScreen callers in one frame use the same row (it depends only on
    // Light4Levels and the dungeon type), so one pointer serves every entry's passes.
    const uchar* gScreenXlat = nullptr;
    int          gScreenMin  = 0;       // indices below this are not remapped (0x20 in Hell)

    inline uchar ScreenRedIdx(uchar idx)
    {
        return (gScreenXlat && idx >= gScreenMin) ? gScreenXlat[idx] : idx;
    }

    // (th4) Checkerboard copy of the current frame for a ghosted icon. The parity is taken in
    // SCREEN space (ix+x, iy+y), exactly like R_TRANS does in engine.h, so two ghosts or a
    // ghost over a stipple elsewhere line up. Scratch texture, rebuilt every draw -- a slot
    // icon is a few thousand pixels, and the 8-bit path re-stipples every frame too.
    IDirect3DTexture9* gGhostTex = nullptr;
    int gGhostTexW = 0, gGhostTexH = 0;

    // (th4) UV EXTENT OF A VARIANT QUAD. gGhostTex is ONE scratch texture shared by every
    // variant draw and it only ever GROWS (see VariantTex): after a 2x3 armour it stays
    // 56x84, and a 1x1 ring written into it next occupies just the top-left 28x28. A quad
    // mapped 0..1 then samples the whole 56x84 -- the ring shrinks into a corner, and the
    // icons visibly change size from one variant draw to the next. Every quad drawn from
    // gGhostTex must stop its UVs at the written part: iw/gGhostTexW, ih/gGhostTexH.
    // (The per-sprite textures from FrameTex are exactly the art's size, so 1.0 is right
    // for them.) DrawOutlinePatch does the same with gOutlineTex (u1 = pw / gOutlineTexW).
    void VariantUV(const IDirect3DTexture9* tex, int iw, int ih, float& uMax, float& vMax)
    {
        uMax = vMax = 1.0f;
        if (tex && tex == gGhostTex && gGhostTexW > 0 && gGhostTexH > 0) {
            uMax = (float)iw / (float)gGhostTexW;
            vMax = (float)ih / (float)gGhostTexH;
        }
    }

    // (th4) Was GhostTex. Now also carries the red tints, because an item can be several at
    // once -- an off-hand slot ghosted under a two-hander whose requirement is not met, on a
    // paused screen -- and chained scratch textures would need one slot each. One pass does
    // all of them. screenRed = number of DrawRedScreen passes to apply (Entry::screenRed).
    IDirect3DTexture9* VariantTex(IDirect3DDevice9* dev, const Sprite& s, IDirect3DTexture9* src,
                                  int ix, int iy, int iw, int ih, bool red, bool ghost,
                                  int screenRed = 0)
    {
        (void)src;
        const bool remap = red || (screenRed > 0 && gScreenXlat);
        if (remap) BuildIdxCube();
        // Source pixels: the frame being shown, at the art's own size. Drawn 1:1 in the
        // common case; if scaled down the stipple is still done per screen pixel.
        const uchar* rgba = CurrentFrameRgba(s);
        if (!rgba || iw <= 0 || ih <= 0) return nullptr;

        if (!gGhostTex || gGhostTexW < iw || gGhostTexH < ih) {
            if (gGhostTex) { gGhostTex->Release(); gGhostTex = nullptr; }
            if (D3D_OK != dev->CreateTexture(iw, ih, 1, D3DUSAGE_DYNAMIC,
                                             D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                             &gGhostTex, nullptr)) return nullptr;
            gGhostTexW = iw; gGhostTexH = ih;
        }
        D3DLOCKED_RECT lr;
        if (D3D_OK != gGhostTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return nullptr;
        for (int y = 0; y < ih; ++y) {
            uchar* dst = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
            const int sy = (int)((long long)y * s.h / ih);
            for (int x = 0; x < iw; ++x, dst += 4) {
                const int sx = (int)((long long)x * s.w / iw);
                const uchar* p = rgba + ((size_t)sy * s.w + sx) * 4;
                uchar cr = p[0], cg = p[1], cb = p[2];
                if (remap) {
                    // Same order as the 8-bit path: R_RED when the sprite is stamped, then
                    // each red-screen XLAT over the finished surface.
                    uchar idx = gIdxCube[(((cr >> 3) * 32) + (cg >> 3)) * 32 + (cb >> 3)];
                    if (red) idx = LightTable[256 * 16 + idx];
                    for (int n = 0; n < screenRed; ++n) idx = ScreenRedIdx(idx);
                    cr = ScreenPalette[idx].peRed;
                    cg = ScreenPalette[idx].peGreen;
                    cb = ScreenPalette[idx].peBlue;
                }
                dst[0] = cb; dst[1] = cg; dst[2] = cr;
                // Ghost is R_TRANS: parity taken in SCREEN space so two ghosts line up.
                dst[3] = (ghost && (((ix + x) + (iy + y)) & 1)) ? 0 : p[3];
            }
        }
        gGhostTex->UnlockRect(0);
        return gGhostTex;
    }

    // The hover outline has to follow the REPLACEMENT art, not the CEL. Lifting the
    // engine's own outline out of WorkingSurface traced the old sprite's silhouette,
    // which is visibly wrong the moment the two shapes differ.
    //
    // So generate it from the PNG's alpha instead: in the icon's own screen-space grid,
    // any transparent pixel touching an opaque one becomes outline. There is no competing
    // outline to worry about -- ItemCEL_Draw skips the CEL entirely for sprites that have
    // replacement art, so the engine never drew one.
    void DrawOutlinePatch(IDirect3DDevice9* dev, const Entry& e)
    {
        if (e.outline < 0) return;
        Sprite* sp = Load(e.gfx);
        if (!sp || !sp->rgba) return;          // no art: the CEL's own outline still stands

        // Must trace the art where it is actually drawn, not the whole slot.
        int fx, fy, fw, fh;
        FitRect(e, *sp, fx, fy, fw, fh);

        const int M = 1;                        // outline is one pixel proud of the art
        int px = fx - M;
        int py = fy - M;
        int pw = fw + M * 2;
        int ph = fh + M * 2;
        if (px < 0) { pw += px; px = 0; }
        if (py < 0) { ph += py; py = 0; }
        if (pw <= 0 || ph <= 0) return;

        if (!gOutlineTex || gOutlineTexW < pw || gOutlineTexH < ph) {
            if (gOutlineTex) { gOutlineTex->Release(); gOutlineTex = nullptr; }
            if (D3D_OK != dev->CreateTexture(pw, ph, 1, D3DUSAGE_DYNAMIC,
                                             D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                             &gOutlineTex, nullptr)) return;
            gOutlineTexW = pw; gOutlineTexH = ph;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != gOutlineTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

        // Alpha of the art at icon-space pixel (ix, iy), nearest-neighbour, mirrored to
        // match how the quad is drawn. Outside the icon counts as transparent.
        struct A {
            const Sprite* sp; const uchar* px; int w, h; bool mir;
            uchar operator()(int ix, int iy) const {
                if (w <= 0 || h <= 0) return 0;     // divisors below; never trust them blind
                if (ix < 0 || iy < 0 || ix >= w || iy >= h) return 0;
                if (mir) ix = w - 1 - ix;
                int sx = (int)((long long)ix * sp->w / w);
                int sy = (int)((long long)iy * sp->h / h);
                if (sx < 0) sx = 0; if (sx >= sp->w) sx = sp->w - 1;
                if (sy < 0) sy = 0; if (sy >= sp->h) sy = sp->h - 1;
                return px[((size_t)sy * sp->w + sx) * 4 + 3];
            }
        } alpha{ sp, CurrentFrameRgba(*sp), fw, fh, e.mirrored };   // (th4) outline follows the animation

        const uchar* p = (const uchar*)ScreenPalette + (size_t)e.outline * sizeof(PALETTEENTRY);
        const uchar oR = p[0], oG = p[1], oB = p[2];
        const int offX = px - (fx - M);                              // clamped-away columns
        const int offY = py - (fy - M);
        bool any = false;

        for (int y = 0; y < ph; ++y) {
            uchar* dstRow = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
            const int iy = y + offY - M;
            for (int x = 0; x < pw; ++x) {
                const int ix = x + offX - M;
                bool on = false;
                if (alpha(ix, iy) < 128) {              // only outside the art itself
                    for (int dy = -1; dy <= 1 && !on; ++dy) {
                        for (int dx = -1; dx <= 1 && !on; ++dx) {
                            if (dx || dy) on = alpha(ix + dx, iy + dy) >= 128;
                        }
                    }
                }
                dstRow[0] = oB; dstRow[1] = oG; dstRow[2] = oR;
                dstRow[3] = on ? 255 : 0;
                if (on) any = true;
                dstRow += 4;
            }
        }
        gOutlineTex->UnlockRect(0);
        if (!any) return;

        const float left   = (float)px - 0.5f;
        const float top    = (float)py - 0.5f;
        const float right  = left + pw;
        const float bottom = top + ph;
        const float u1 = (float)pw / (float)gOutlineTexW;
        const float v1 = (float)ph / (float)gOutlineTexH;

        TransformedVertexT v[4] = {
            { left,  top,    0.0f, 1.0f, 0.0f, 0.0f },
            { right, top,    0.0f, 1.0f, u1,   0.0f },
            { left,  bottom, 0.0f, 1.0f, 0.0f, v1  },
            { right, bottom, 0.0f, 1.0f, u1,   v1  },
        };
        dev->SetTexture(0, gOutlineTex);
        // (th4) Dimmed under a tooltip exactly like the icon it traces. Without this the
        // outline stayed at full brightness over a darkened box while its own art went
        // dark -- the 8-bit path never shows that, because there the outline is part of
        // WorkingSurface and DrawBoxTransparent darkens it along with everything else.
        DrawQuadDimmed(dev, v, px, py, pw, ph);
    }

} // namespace

bool ItemOverlay_Queue(int graphicValue, int x, int yBottom, int w, int h, bool mirrored,
                       int outlineColor, int drawFlag)
{
    if (DDraw) return false;   // (th4) nothing flushes the queue in DDraw mode; see Load()
    if (gQueueCount >= kMaxQueue) return false;
    if (w <= 0 || h <= 0) return false;

    Entry& e = gQueue[gQueueCount++];
    e.gfx = graphicValue;
    e.x = x; e.y = yBottom;
    e.w = w; e.h = h;
    e.mirrored = mirrored;
    e.onCursor = false;
    e.outline = outlineColor;
    e.ghost = false;
    e.screenRed = 0;
    // (th4) Only R_RED is taken from the flag word. The rest (R_MIRROR, R_CLIP) are
    // already carried by the dedicated arguments or do not apply to a textured quad.
    e.red = (drawFlag & R_RED) != 0;

    // (th4) An 8-bit item sprite is visible only where nothing painted over it afterwards --
    // DrawINVPanel() stamps the CELs, then DrawSpellsPanel() and friends stamp straight on
    // top. Replacement art never enters that contest, because ItemCEL_Draw() skips the CEL
    // and the PNG is flushed in D3D after the whole surface is uploaded. Snapshot the cell
    // box NOW, while the surface still shows what lies under the item; at flush time any
    // pixel that differs was painted by a later panel, and CoverPatch() puts it back on top.
    // Per pixel, so a grid of tiles with gaps hides the icon exactly where its tiles have
    // ink and lets it show through the gaps -- which a rectangle claim cannot do.
    const int cellsW = (w + CELL_SIZE / 2) / CELL_SIZE;
    const int cellsH = (h + CELL_SIZE / 2) / CELL_SIZE;
    e.coverW = (cellsW > 0 ? cellsW : 1) * CELL_SIZE;
    e.coverH = (cellsH > 0 ? cellsH : 1) * CELL_SIZE;
    e.coverX = x - (int)Screen_LeftBorder;
    e.coverY = yBottom - e.coverH + kYAdjust - (int)Screen_TopBorder;
    e.coverSnap = SnapshotRect(e.coverX, e.coverY, e.coverW, e.coverH);
    return true;
}

bool ItemOverlay_HasArt(int graphicValue)   // (th4)
{
    return Load(graphicValue) != nullptr;
}

bool ItemOverlay_QueueGhost(int graphicValue, int x, int yBottom, int w, int h, bool mirrored)   // (th4)
{
    if (!ItemOverlay_Queue(graphicValue, x, yBottom, w, h, mirrored, -1, 0)) return false;
    gQueue[gQueueCount - 1].ghost = true;
    return true;
}

void ItemCEL_Draw(int overlayGfx, int x, int y, char* celBuf, unsigned frameNum, int drawWidth,
                  int offsetStart, int offsetEnd, int flag, int outlineColor)
{
    // With replacement art the CEL is not drawn AT ALL -- not the body, not the outline.
    //
    // Earlier revisions drew it and then painted the slot's saved background back over it.
    // That could not work once two converted items overlapped: the second item's snapshot
    // was taken after the first item's CEL had already been stamped into the surface, so
    // restoring it dragged the first item's old sprite back on top of its own overlay.
    // Nothing needs the CEL any more -- the hover outline is generated from the PNG's
    // alpha -- so the honest fix is simply not to draw it.
    //
    // Load() is cached, hit or miss, so this costs a small linear scan and nothing else.
    if (Load(overlayGfx)) return;
    CEL_Draw(x, y, celBuf, frameNum, drawWidth, offsetStart, offsetEnd, flag, outlineColor);
}

// (th4) Used by RenderCursor: the overlay-clear pass must only run when the
// 8-bit cursor CEL was actually drawn. With replacement PNG art the icon is
// drawn post-quad on top of the composite and needs no clearing.
bool ItemOverlay_HasReplacementArt(int graphicValue)
{
    return Load(graphicValue) != nullptr;
}

void ItemOverlay_QueueDimRect(int x, int y, int w, int h, float dimFactor)
{
    if (DDraw) return;         // (th4) no D3D flush in DDraw mode; see Load()
    if (gDimCount >= kMaxDim) return;
    if (w <= 0 || h <= 0) return;
    DimRect& d = gDim[gDimCount++];
    d.x = x; d.y = y; d.w = w; d.h = h; d.factor = dimFactor; d.snap = nullptr;
}

void ItemOverlay_SnapshotDimRect()
{
    if (gDimCount <= 0) return;
    DimRect& d = gDim[gDimCount - 1];
    if (d.snap) return;

    const size_t need = (size_t)d.w * d.h;
    if (gSnapUsed + need > (size_t)kSnapArena) return;   // too big: text patch skipped, no harm

    uchar* dst = gSnap + gSnapUsed;
    const int LB = (int)Screen_LeftBorder;
    const int TB = (int)Screen_TopBorder;
    // (th4) Clamped: a wide localized menu line can push the claimed rect past either edge,
    // and an unclamped row read walks off the start or end of WorkingSurface.
    for (int row = 0; row < d.h; ++row) {
        uchar* out = dst + (size_t)row * d.w;
        const int sy = d.y + row + TB;
        if (sy < 0 || sy >= (int)WorkingHeight) { memset(out, 0, d.w); continue; }
        const uchar* srcRow = WorkingSurface + (size_t)WorkingWidth * sy;
        for (int col = 0; col < d.w; ++col) {
            const int sx = d.x + col + LB;
            out[col] = (sx < 0 || sx >= (int)WorkingWidth) ? 0 : srcRow[sx];
        }
    }
    d.snap = dst;
    gSnapUsed += (int)need;
}

void ItemOverlay_ForgetHeldQueue()
{
    gHeldCount = 0;
}

void ItemOverlay_FrameComposed()
{
    gFrameComposed = true;
}

void ItemOverlay_MarkQueuedOnCursor()
{
    // Only ever called straight after a Queue() that returned true, so the last entry is
    // the one just added. Guarded anyway.
    if (gQueueCount <= 0) return;
    gQueue[gQueueCount - 1].onCursor = true;
}

// (th4) DrawRedScreen just XLATed the whole WorkingSurface through `xlat` (indices below
// minIndex untouched). Called from the end of DrawRedScreen (Panel.cpp), after the loop.
//
// WITHOUT THIS, EVERY QUEUED ICON VANISHES ON PAUSE AND DEATH. Each entry snapshotted its
// cell box at queue time (ItemOverlay_Queue -> coverSnap) so CoverPatch() can put back
// whatever a LATER panel painted over the item -- "later" meaning "differs from the
// snapshot". The red screen changes every pixel on the surface, so every pixel of every
// cell box differs, and CoverPatch() pasted the whole (red, empty) cell back over the icon.
// Pause (Menu_ShowPause) and death call DrawRedScreen AFTER the panels (Draw.cpp ~6749/
// ~6751), so this hit the inventory, body slots, stash, trade and craft panels alike.
//
// The 8-bit path just gets its sprites reddened along with everything else. Match it:
//   1. XLAT every snapshot already in the arena the same way, so the snapshots again
//      describe "the surface under the item" and only real later ink (the "Pause" text,
//      popups) keys as cover. Snapshots taken after this call are post-red already.
//   2. Count one red-screen pass on every entry queued so far; VariantTex reddens the
//      art through the same row. Entries queued after this call (belt, main panel, the
//      dragged item) are drawn after the red screen in 8-bit too, and stay normal.
//   3. The hover outline colour is a palette index painted into the surface by CEL_Draw
//      in 8-bit, so it goes through the same XLAT.
// Anchor to verify against: pause a dungeon game with the inventory open -- converted and
// unconverted items must both show, and both in the same red.
void ItemOverlay_OnRedScreen(const unsigned char* xlat, int minIndex)
{
    if (!xlat) return;
    gScreenXlat = xlat;
    gScreenMin  = minIndex;
    for (int i = 0; i < gSnapUsed; ++i) gSnap[i] = ScreenRedIdx(gSnap[i]);
    for (int i = 0; i < gQueueCount; ++i) {
        Entry& e = gQueue[i];
        if (e.screenRed < 255) ++e.screenRed;
        if (e.outline >= 0 && e.outline < 256) e.outline = ScreenRedIdx((uchar)e.outline);
    }
}

void ItemOverlay_MarkCursorItem(int graphicValue)
{
    // Only suppress when we are actually going to overlay the dragged item.
    gCursorIsItem = (Load(graphicValue) != nullptr);
}

// (th4) One state block per device session, re-captured each frame instead of created and
// destroyed every frame (CreateStateBlock of D3DSBT_ALL is one of the most expensive D3D9 calls,
// and this draw runs every frame whenever a belt / inventory icon has replacement art).
// A state block belongs to ONE device; the optional second window draws through its own
// device, so keep one block per device (slot 0 = the device seen first, 1 = the other).
static IDirect3DStateBlock9* gStateBlock[2] = { nullptr, nullptr };
static IDirect3DDevice9*     gStateBlockDev[2] = { nullptr, nullptr };

void ItemOverlay_OnDeviceLost()
{
    for (int i = 0; i < 2; ++i) { if (gStateBlock[i]) { gStateBlock[i]->Release(); gStateBlock[i] = nullptr; } gStateBlockDev[i] = nullptr; }
    gHeldCount = 0;   // (th4) never replay a queue across a device loss / shutdown
    for (int i = 0; i < gSpriteCount; ++i) {
        Sprite& s = gSprites[i];
        if (s.frameTex) {                         // (th4) frame 1 is also frameTex[0]
            for (int f = 0; f < s.frameCount; ++f) {
                if (s.frameTex[f]) { s.frameTex[f]->Release(); s.frameTex[f] = nullptr; }
            }
            s.tex = nullptr;
        }
        if (s.tex) { s.tex->Release(); s.tex = nullptr; }
    }
    if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; gCursorTexW = gCursorTexH = 0; }
    if (gTextTex)   { gTextTex->Release();   gTextTex = nullptr;   gTextTexW = gTextTexH = 0; }
    if (gOutlineTex){ gOutlineTex->Release();gOutlineTex = nullptr;gOutlineTexW = gOutlineTexH = 0; }
    if (gGhostTex)  { gGhostTex->Release();  gGhostTex = nullptr;  gGhostTexW = gGhostTexH = 0; }
    gQueueCount = 0;
}

void ItemOverlay_Shutdown()
{
    gHeldCount = 0;   // (th4) never replay a queue across a device loss / shutdown
    for (int i = 0; i < gSpriteCount; ++i) {
        Sprite& s = gSprites[i];
        if (s.frameTex) {                         // (th4) animated: all frames
            for (int f = 0; f < s.frameCount; ++f) {
                if (s.frameTex[f]) s.frameTex[f]->Release();
                if (s.frameRgba && s.frameRgba[f]) stbi_image_free(s.frameRgba[f]);
            }
            free(s.frameTex); free(s.frameRgba);
            s.frameTex = nullptr; s.frameRgba = nullptr; s.tex = nullptr; s.rgba = nullptr;
        }
        if (s.tex)  { s.tex->Release(); s.tex = nullptr; }
        if (s.rgba) { stbi_image_free(s.rgba); s.rgba = nullptr; }
    }
    if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; gCursorTexW = gCursorTexH = 0; }
    if (gTextTex)   { gTextTex->Release();   gTextTex = nullptr;   gTextTexW = gTextTexH = 0; }
    if (gOutlineTex){ gOutlineTex->Release();gOutlineTex = nullptr;gOutlineTexW = gOutlineTexH = 0; }
    if (gGhostTex)  { gGhostTex->Release();  gGhostTex = nullptr;  gGhostTexW = gGhostTexH = 0; }
    gSpriteCount = 0;
    gFindHint = -1;
    gQueueCount = 0;
}

void ItemOverlay_DrawD3D(IDirect3DDevice9* dev)
{
    // (th4) Match the 8-bit fade; see gFadeMul and gHeldQueue above.
    //
    // WHICH FRAMES ARE REAL. The panel code queues icons only when MainDrawFunc
    // composes a frame; that call also raises gFrameComposed. Every other present --
    // the steps inside PaletteFadeIn/Out, and the bare DrawNotGameplayScreen(1) the
    // in-game menu issues right before it ends the game -- is a RE-CONVERSION of the
    // last composed frame at a different brightness. On those the held queue is
    // replayed; on a composed frame the queue (empty included) becomes the held copy.
    //
    // Three earlier rules were wrong in turn: "replay when brightness < 256" left a
    // blank first fade step; "capture only when something was queued" let a closed
    // inventory's queue linger and flash on exit; "any non-fading present is real"
    // took NewGameMenuHandler's DrawNotGameplayScreen(1) as a real empty frame and
    // wiped the queue right before the exit fade, so the icons vanished at once
    // instead of fading. The signal has to come from the composer, not be inferred.
    //
    // Capture happens BEFORE the black-frame exit: after a warp, MainDrawFunc draws
    // the destination's first frame while the palette is still black, and its queue
    // is what the fade-in replays.
    gDrawnCount = 0;	// (th4) DRAWN THIS FRAME: this present's quads only (ItemOverlay_MarkUi)
    const int lvl = PaletteFadeLevel;
    if (gFrameComposed) {
        if (gQueueCount > 0) memcpy(gHeldQueue, gQueue, sizeof(Entry) * (size_t)gQueueCount);
        gHeldCount = gQueueCount;
        gHeldCursorIsItem = gCursorIsItem;
        gFrameComposed = false;
    } else if (gQueueCount == 0 && gHeldCount > 0) {
        memcpy(gQueue, gHeldQueue, sizeof(Entry) * (size_t)gHeldCount);
        gQueueCount = gHeldCount;
        gCursorIsItem = gHeldCursorIsItem;
    }
    if (lvl <= 0) { gQueueCount = 0; gCursorIsItem = false; gDimCount = 0; gSnapUsed = 0; return; }
    gFadeMul = (DWORD)(lvl >= 256 ? 255 : lvl);

    if (!dev || gQueueCount <= 0) { gQueueCount = 0; gCursorIsItem = false; gDimCount = 0; gSnapUsed = 0; return; }

    // CEL_Draw writes at WorkingSurface + y*WorkingWidth + x, i.e. working-surface
    // absolute coordinates with the borders included. RenderQuadTexture uploads
    // starting at (Screen_LeftBorder, Screen_TopBorder), so those come off to get
    // the on-screen pixel.
    const float ox = (float)Screen_LeftBorder;
    const float oy = (float)Screen_TopBorder;

    IDirect3DStateBlock9* sb = nullptr;
    bool stateSet = false;

    // (th4) Hoisted out of the icon loop: the deferred cursor pass below needs the very same
    // setup, and when the ONLY queued item is the dragged one the loop never runs at all --
    // which left the state unset and the item invisible.
    auto ensureState = [&]() {
        if (stateSet) return;
        const int sbSlot = (gStateBlockDev[0] == dev || !gStateBlockDev[0]) ? 0 : 1;
        IDirect3DStateBlock9*& blk = gStateBlock[sbSlot];
        if (blk && gStateBlockDev[sbSlot] != dev) { blk->Release(); blk = nullptr; }
        if (!blk) {
            if (D3D_OK != dev->CreateStateBlock(D3DSBT_ALL, &blk)) blk = nullptr;
            gStateBlockDev[sbSlot] = blk ? dev : nullptr;
        } else if (D3D_OK != blk->Capture()) {
            blk->Release(); blk = nullptr; gStateBlockDev[sbSlot] = nullptr;
        }
        sb = blk;
        dev->SetVertexShader(nullptr);
        dev->SetPixelShader(nullptr);       // drop the palette shader
        dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
        dev->SetTexture(1, nullptr);        // drop the palette texture
        dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        dev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        dev->SetRenderState(D3DRS_LIGHTING, FALSE);
        dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        // MODULATE against TEXTUREFACTOR so a quad can be dimmed under a tooltip.
        // Alpha stays straight from the texture.
        SetIconFactor(dev, 255);
        dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
        dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        dev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        dev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        stateSet = true;
    };

    for (int i = 0; i < gQueueCount; ++i) {
        Entry& e = gQueue[i];
        if (e.onCursor) continue;               // drawn after the patches, see below

        Sprite* s = Load(e.gfx);
        if (!s) continue;                       // no art -> the CEL underneath stays
        if (!Upload(dev, *s)) continue;

        ensureState();
        IDirect3DTexture9* tex = FrameTex(dev, *s);

        int ix, iy, iw, ih;
        FitRect(e, *s, ix, iy, iw, ih);                            // screen space, centred

        // (th4) Ghost (off-hand slot under a 2H weapon): same policy as the 8-bit path,
        // R_TRANS -- every other screen pixel in a checkerboard, full colour on the rest.
        if (e.ghost || e.red || e.screenRed) {
            tex = VariantTex(dev, *s, tex, ix, iy, iw, ih, e.red, e.ghost, e.screenRed);
            if (!tex) continue;
        }
        dev->SetTexture(0, tex);

        const float left   = (float)ix - 0.5f;
        const float top    = (float)iy - 0.5f;
        const float right  = left + (float)iw;
        const float bottom = top  + (float)ih;

        // The engine can blit an item flipped (bodyItem.mirrorImage). Swap U rather
        // than the vertices, so winding stays the same.
        float uMax, vMax;
        VariantUV(tex, iw, ih, uMax, vMax);     // (th4) < 1 only for the shared scratch texture
        const float u0 = e.mirrored ? uMax : 0.0f;
        const float u1 = e.mirrored ? 0.0f : uMax;

        TransformedVertexT v[4] = {
            { left,  top,    0.0f, 1.0f, u0, 0.0f },
            { right, top,    0.0f, 1.0f, u1, 0.0f },
            { left,  bottom, 0.0f, 1.0f, u0, vMax },
            { right, bottom, 0.0f, 1.0f, u1, vMax },
        };

        DrawQuadDimmed(dev, v, ix, iy, iw, ih);
        NoteDrawn(*s, e, ix, iy, iw, ih);	// (th4) ItemOverlay_MarkUi
    }

    // Hover outlines next, above the icons they belong to.
    if (stateSet) {
        for (int i = 0; i < gQueueCount; ++i) {
            if (gQueue[i].onCursor) continue;
            DrawOutlinePatch(dev, gQueue[i]);
        }
    }

    // The dragged item is drawn BEFORE the patches only in the sense that it is not part of
    // the loop above; it goes on last, below. Make sure the state exists for the patch pass
    // even if nothing but the cursor item was queued.
    for (int i = 0; i < gQueueCount && !stateSet; ++i) {
        if (!gQueue[i].onCursor) continue;
        Sprite* s = Load(gQueue[i].gfx);
        if (s && Upload(dev, *s)) ensureState();
    }

    // Then everything the 8-bit surface drew INTO a claimed rectangle after we snapshotted
    // it -- tooltip text, and the whole in-game menu. Our icon quads have just covered it,
    // so it goes back on top. An item lying in the inventory or worn on the body belongs
    // UNDER the menu; the one being dragged does not, and is drawn after this.
    // (th4) First the per-item cover patches: anything a later panel painted over an
    // item's cell box goes back on top of that item, per pixel. This is the same rule
    // the 8-bit path gets for free from draw order. The dragged item is exempt -- it is
    // drawn last, above everything, and its footprint is skipped inside the patch too.
    if (stateSet) {
        for (int i = 0; i < gQueueCount; ++i) {
            if (gQueue[i].onCursor) continue;
            CoverPatch(dev, gQueue[i]);
        }
    }

    if (stateSet) {
        for (int d = 0; d < gDimCount; ++d) DrawTextPatch(dev, gDim[d]);
    }

    // The dragged item last of all: it follows the cursor and has to stay above everything,
    // menu included. Same draw as the loop above, just deferred past the patches.
    if (stateSet) {
        for (int i = 0; i < gQueueCount; ++i) {
            Entry& e = gQueue[i];
            if (!e.onCursor) continue;
            Sprite* s = Load(e.gfx);
            if (!s || !Upload(dev, *s)) continue;
            ensureState();

            int ix, iy, iw, ih;
            FitRect(e, *s, ix, iy, iw, ih);
            // (th4) The dragged item gets the red tint too -- Draw.cpp passes R_RED for it
            // exactly as the slot sites do, and the 8-bit cursor sprite has always been
            // tinted. FitRect has to run first: VariantTex needs the screen rect.
            IDirect3DTexture9* ctex = FrameTex(dev, *s);
            if (e.red || e.screenRed) {
                IDirect3DTexture9* v = VariantTex(dev, *s, ctex, ix, iy, iw, ih, e.red, false, e.screenRed);
                if (v) ctex = v;
            }
            dev->SetTexture(0, ctex);
            const float left   = (float)ix - 0.5f;
            const float top    = (float)iy - 0.5f;
            const float right  = left + (float)iw;
            const float bottom = top  + (float)ih;
            float uMax, vMax;
            VariantUV(ctex, iw, ih, uMax, vMax);    // (th4) see VariantUV
            const float u0 = e.mirrored ? uMax : 0.0f;
            const float u1 = e.mirrored ? 0.0f : uMax;
            TransformedVertexT v[4] = {
                { left,  top,    0.0f, 1.0f, u0, 0.0f },
                { right, top,    0.0f, 1.0f, u1, 0.0f },
                { left,  bottom, 0.0f, 1.0f, u0, vMax },
                { right, bottom, 0.0f, 1.0f, u1, vMax },
            };
            DrawQuadDimmed(dev, v, ix, iy, iw, ih);
            NoteDrawn(*s, e, ix, iy, iw, ih);	// (th4) ItemOverlay_MarkUi: the dragged item is the one over the world
            DrawOutlinePatch(dev, e);
        }
    }

    // Cursor last, so it sits above every icon we just drew. Only meaningful if at
    // least one icon was actually overlaid -- stateSet says so.
    if (stateSet && !gCursorIsItem) DrawCursorPatch(dev, gQueue, gQueueCount);

    if (sb) { sb->Apply(); }	// (th4) kept for the next frame; released in ItemOverlay_OnDeviceLost
    gQueueCount = 0;
    gCursorIsItem = false;
    gDimCount = 0;
    gSnapUsed = 0;
}

// (th4) THE ITEM ART IS UI TOO -- the same problem and the same cure as dx_utility.cpp Automap_MarkUi. The icons are
// drawn in true colour straight into the frame (ItemOverlay_DrawD3D, RenderQuadTexture / RenderQuadTextureUltra) BEFORE
// PostFxPresent, and an item with replacement art has NO pixels in the 8-bit frame (Draw.cpp skips its CEL). Every mask
// that finds the UI by comparing the 8-bit frame with the world snapshot therefore took an item held on the cursor over
// the world for the ground under it: grass blades grew over it, the river's ripple shook it, the snow, wet ground and
// glow painted it (Mordor, 2026-10-10). Icons inside panels were safe only because the panels' UI RECTS cover them.
// This marks every OPAQUE pixel of every quad drawn this frame UI (0) in such a mask -- per pixel from the art's alpha,
// not the whole rect, or the ground round the item would lose its grass and ripple in a visible box -- and, for an
// outlined item (hover / held), its 1-px OUTLINE ring too, by DrawOutlinePatch's own rule (outside the art, next to a
// pixel of alpha >= 128): the ring is true colour as well, and blades rooted on it grew in its colour (a yellow halo
// above a held item, Mordor 2026-10-10). Screen game px,
// like the quads; half: the mask has one texel per 2 x 2 game px (dx_utility.cpp HeatHaze_BuildMask's world/UI mask).
// CALLERS (dx_utility.cpp, each next to Automap_MarkUi or where it would go): HeatHaze_BuildMask, Grass_BuildMask,
// Wet_BuildMask. A NEW mask built that way needs the call too.
void ItemOverlay_MarkUi(unsigned char* mask, int pitch, int w, int h, bool half)
{
    if (!mask || w <= 0 || h <= 0) return;
    const int sh = half ? 1 : 0;
    for (int i = 0; i < gDrawnCount; ++i) {
        const Drawn& d = gDrawn[i];
        if (!d.rgba) continue;
        // the art's alpha at quad pixel (ix, iy), mirrored as drawn; outside the quad: 0 (DrawOutlinePatch's alpha())
        auto alpha = [&]( int ix, int iy ) -> int {
            if (ix < 0 || iy < 0 || ix >= d.w || iy >= d.h) return 0;
            if (d.mirrored) ix = d.w - 1 - ix;
            return d.rgba[( (size_t)( iy * d.sh / d.h ) * d.sw + ix * d.sw / d.w ) * 4 + 3];
        };
        const int M = d.outlined ? 1 : 0;	// the outline ring is one pixel proud of the art
        const int x0 = d.x - M > 0 ? d.x - M : 0, y0 = d.y - M > 0 ? d.y - M : 0;
        const int x1 = d.x + d.w + M < ( w << sh ) ? d.x + d.w + M : ( w << sh );
        const int y1 = d.y + d.h + M < ( h << sh ) ? d.y + d.h + M : ( h << sh );
        for (int y = y0; y < y1; ++y) {
            uchar* m = mask + (size_t)( y >> sh ) * pitch;
            for (int x = x0; x < x1; ++x) {
                const int ix = x - d.x, iy = y - d.y;
                bool ui = alpha(ix, iy) != 0;
                if (!ui && M && alpha(ix, iy) < 128)	// OUTLINE RING (above)
                    for (int dy = -1; dy <= 1 && !ui; ++dy)
                        for (int dx = -1; dx <= 1 && !ui; ++dx) ui = ( dx || dy ) && alpha(ix + dx, iy + dy) >= 128;
                if (ui) m[x >> sh] = 0;
            }
        }
    }
}
