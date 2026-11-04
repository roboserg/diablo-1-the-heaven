// MenuPortrait.cpp
// See MenuPortrait.h for why this exists and how the strips are named.
//
// Structure deliberately mirrors AnimOrbs.cpp: same stb_image decode, same
// D3DFMT_A8R8G8B8 dynamic texture, same state-block save/restore around an
// XYZRHW quad drawn with no pixel shader.

#include "stdafx.h"
#include <math.h>
#include "MenuPortrait.h"

#include <d3d9.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;

// NOT LoadFile(). LoadFile -> File_Open(..., FIVE_TRY), and a miss there ends in
// FileErrDlg -- the "The Hell was unable to open a required file" box -- so probing
// for an optional file with it is fatal on the first extension that is absent.
// File_Open with ONE_TRY returns 0 quietly when the file is simply not in the
// archive (see File.cpp: it breaks out on GetLastError()==2 before any dialog).
// The unfaded reference palette. MenuPalette[] itself is file-local to Hell.cpp;
// this accessor is the only way at it, and it is what FadePalettePhase() uses as
// its own definition of "full brightness". See MenuFadeLevel() below.
PALETTEENTRY* __fastcall GetMenuPalette(int palColorIndex);

int __fastcall File_Open(const char* fileName, HANDLE* fileHandle, int oneTry);
DWORD __fastcall File_GetSize(HANDLE aHandle, LPDWORD lpFileSizeHigh);
int __fastcall File_Read(HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead);
int __fastcall File_Close(HANDLE aHandle);

// (th4) The menu cursor, taken from the GAME's own buffers -- not from Storm's.
//
// LoadGfxCursor (Hell.cpp:5619) reads ui_art\cursor.pcx into CursorBuf and derives CursorAnim
// from it: 0 where the pcx pixel is non-zero, 0xFF where it is zero. SetGfxCursor then hands
// the pair to SDlgSetSystemCursor, and SDlg composites them into the 8-bit dialog surface as
// (background AND CursorAnim) OR CursorBuf -- so a pixel is cursor ink exactly where CursorBuf
// is non-zero, which is why only CursorBuf is needed here.
//
// Do NOT reach for Storm's own copies through StormReloc using the addresses in StormData.asm.
// That asm was disassembled from Strm2.dll of The Hell 1.57, but the game loads Storm 1611,
// whose data layout differs -- see the palette address in dx_utility.cpp:1181, which carries an
// older one commented out beside the current for exactly this reason. Those reads return
// garbage. These two are real link symbols and need no addresses at all.
extern SIZE CursorSize;
extern unsigned __int8* CursorBuf;   // colour image, 8-bit palette indices, rows of cx bytes

// stb_image, instantiated in AnimOrbs.cpp. Its STBI_ONLY_* list there decides which
// of the formats below actually decode.
extern "C" {
    unsigned char* stbi_load_from_memory(unsigned char const* buffer, int len,
                                         int* x, int* y, int* comp, int req_comp);
    void           stbi_image_free(void* retval_from_stbi_load);
}

namespace {

    struct TransformedVertexT { float x, y, z, rhw; float u, v; };

    // ---- decoded strips -----------------------------------------------------
    // A strip is the whole sheet in RGBA, held in RAM. It cannot be one texture:
    // hero_pix has hundreds of rows, so the strip is far taller than the 8192 or
    // 16384 px limit most cards impose. Only the current portrait's rows are
    // uploaded, and only when the selection changes.
    enum { kStripMax = 4 };              // hero_pix, gcs, traits, + headroom

    struct Strip {
        char   key[64];                  // sheet basename, e.g. "hero_pix"
        uchar* rgba;                     // w*h*4, or null once we know it is absent
        int    w, h;
        bool   tried;                    // absent strips are looked up exactly once
    };
    Strip gStrips[kStripMax];
    int   gStripCount = 0;

    // ---- what the menu last told us to show --------------------------------
    char gSheet[64] = { 0 };
    int  gId   = -1;
    int  gSuppress = 0;              // >0 while a modal box is covering the portrait
    HWND gWin  = 0;                  // re-queried each frame; see the header
    const uchar* gRef8 = 0;          // (th4) the 8-bit sheet the control shows; 0 = unknown
    int   gRef8W = 0;
    // (th4) The "has the dialog's art landed?" gate below is armed once per CONTROL+SHEET,
    // not once per portrait id. These remember what it last passed for.
    HWND gArmedWin = 0;
    char gArmedSheet[64] = { 0 };
    int  gGuiX = 0, gGuiY = 0, gGuiW = 0, gGuiH = 0;

    // ---- the slice currently resident in the texture ------------------------
    IDirect3DTexture9* gTex = nullptr;
    char gTexSheet[64] = { 0 };
    int  gTexId = -1;
    int  gTexW = 0, gTexH = 0;

    // ---- the rebuilt menu cursor --------------------------------------------
    // Kept separate from gTex: the portrait slice only changes when the selection does,
    // the cursor changes whenever the mouse moves.
    IDirect3DTexture9* gCursorTex = nullptr;
    int  gCursorTexW = 0, gCursorTexH = 0;

    // "ui_art\hero_pix.pcx" -> "hero_pix"
    void SheetKey(const char* pcxName, char* out, int outSize)
    {
        out[0] = 0;
        if (!pcxName) return;
        const char* base = pcxName;
        for (const char* p = pcxName; *p; ++p) {
            if (*p == '\\' || *p == '/') base = p + 1;
        }
        int i = 0;
        for (; base[i] && base[i] != '.' && i < outSize - 1; ++i) out[i] = base[i];
        out[i] = 0;
        // (th4) The overlay sheet is a separate asset from the 8-bit .pcx it replaces, so
        // give it its own name rather than shadowing the pcx basename:
        //   ui_art\hero_pix.pcx  ->  data\portraits\hero_pix_overlay.bmp
        // Applies to every sheet this maps (traits, gcs, ...), keeping one convention.
        const char* kSuffix = "_overlay";
        for (int k = 0; kSuffix[k] && i < outSize - 1; ++k) out[i++] = kSuffix[k];
        out[i] = 0;
    }

    Strip* FindStrip(const char* key)
    {
        for (int i = 0; i < gStripCount; ++i) {
            if (0 == strcmp(gStrips[i].key, key)) return &gStrips[i];
        }
        return nullptr;
    }

    // Decode data\portraits\<key>.bmp, falling back to png/jpg/tga. One attempt ever.
    Strip* LoadStrip(const char* key)
    {
        if (!key || !key[0]) return nullptr;
        if (Strip* s = FindStrip(key)) return s->rgba ? s : nullptr;
        if (gStripCount >= kStripMax) return nullptr;

        Strip& s = gStrips[gStripCount++];
        memset(&s, 0, sizeof(s));
        strncpy(s.key, key, sizeof(s.key) - 1);
        s.tried = true;

        // Which of these actually work is decided by the STBI_ONLY_* list in
        // AnimOrbs.cpp -- that is the only translation unit that instantiates
        // stb_image, so its format list governs every caller.
        static const char* kExt[] = { "bmp", "png", "jpg", "tga" };   // (th4) bmp first
        const int kExtCount = (int)(sizeof(kExt) / sizeof(kExt[0]));
        for (int e = 0; e < kExtCount && !s.rgba; ++e) {
            char path[160];
            sprintf(path, "data\\portraits\\%s.%s", key, kExt[e]);

            HANDLE fh = 0;
            if (!File_Open(path, &fh, 1) || !fh) continue;   // absent: try the next one
            const DWORD bytes = File_GetSize(fh, nullptr);
            if (bytes == 0 || bytes == 0xFFFFFFFFu) { File_Close(fh); continue; }

            uchar* raw = (uchar*)malloc(bytes);
            if (!raw) { File_Close(fh); continue; }
            File_Read(fh, raw, bytes);
            File_Close(fh);

            int w = 0, h = 0, comp = 0;
            uchar* px = stbi_load_from_memory(raw, (int)bytes, &w, &h, &comp, 4);
            free(raw);
            if (!px) continue;
            if (w <= 0 || h <= 0) { stbi_image_free(px); continue; }

            s.rgba = px; s.w = w; s.h = h;   // released by MenuPortrait_Shutdown()
        }
        return s.rgba ? &s : nullptr;
    }

    void ReleaseTexture()
    {
        if (gTex) { gTex->Release(); gTex = nullptr; }
        gTexSheet[0] = 0; gTexId = -1; gTexW = gTexH = 0;
        // D3DPOOL_DEFAULT as well, so it has to go on a device reset too.
        if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; }
        gCursorTexW = gCursorTexH = 0;
    }

    // Upload one portrait's rows out of the strip into a fresh texture.
    bool UploadSlice(IDirect3DDevice9* dev, Strip* strip, int id, int ctrlW, int ctrlH)
    {
        if (!dev || !strip || !strip->rgba) return false;
        if (ctrlW <= 0 || ctrlH <= 0) return false;

        if (gTex && gTexId == id && 0 == strcmp(gTexSheet, strip->key)) return true;

        // The strip may be a whole-number upscale of the .pcx. Take the scale from
        // its own width against the control's, and the row height follows -- so a
        // 2x or 4x strip needs no code change and simply looks sharper.
        const int scaleNum = strip->w;
        const int scaleDen = ctrlW;
        if (scaleDen <= 0) return false;
        const int rowH = (int)(((long long)ctrlH * scaleNum) / scaleDen);
        if (rowH <= 0) return false;

        const int y0 = id * rowH;
        if (y0 < 0 || y0 + rowH > strip->h) return false;   // id past the end of the strip

        ReleaseTexture();

        IDirect3DTexture9* tex = nullptr;
        if (D3D_OK != dev->CreateTexture(strip->w, rowH, 1, D3DUSAGE_DYNAMIC,
                                         D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, nullptr)) {
            return false;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != tex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) {
            tex->Release();
            return false;
        }
        // stb gives RGBA; D3DFMT_A8R8G8B8 is BGRA in memory. Same swap AnimOrbs does.
        for (int y = 0; y < rowH; ++y) {
            const uchar* src = strip->rgba + ((size_t)(y0 + y) * strip->w) * 4;
            uchar*       dst = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
            for (int x = 0; x < strip->w; ++x) {
                dst[0] = src[2]; dst[1] = src[1]; dst[2] = src[0]; dst[3] = src[3];
                src += 4; dst += 4;
            }
        }
        tex->UnlockRect(0);

        gTex = tex; gTexId = id; gTexW = strip->w; gTexH = rowH;
        strncpy(gTexSheet, strip->key, sizeof(gTexSheet) - 1);
        return true;
    }

    // ---- fade tracking ------------------------------------------------------
    // FadePalette()/FadePalettePhase() dim the menu by walking the 8-bit palette down
    // towards black. This overlay is an RGBA quad and ignores the palette completely,
    // so without this it stayed at full brightness over a menu fading out -- the
    // portrait appeared to pop out of an otherwise black screen.
    //
    // Rather than hook the ~20 FadePalette() call sites, read the result: compare the
    // palette the menu art is ACTUALLY rendered through against the unfaded reference.
    // That is self-correcting -- fade out, fade in via FadePalettePhase, and the
    // ClearPalette blackout all fall out of it for free, and an unfaded palette gives
    // exactly 1.0 so the common case is untouched.
    //
    // Same array dx_utility.cpp samples for the menu (see ConvertPaletteSurfaceToTrueColor):
    // storm's working palette, inside the relocated Storm2Base image.
    float MenuFadeLevel()
    {
        // (th4) THE BUG THAT MADE THE PORTRAIT "APPEAR FIRST". This used to bail with
        // 1.0 when StormReloc was 0, reading that as "Storm not relocated yet". But
        // StormReloc is (Storm2Base - 0x15000000): it is ZERO exactly when strm2.dll
        // loads at its preferred base, which is the normal case. So the fade was
        // never tracked at all -- the overlay popped in at full brightness while the
        // 8-bit menu came up out of black over a quarter of a second. dx_utility
        // reads the same address with the same expression and never had this test,
        // which is why the 8-bit art faded correctly and the portrait did not.
        // "Not loaded" is Storm2Base == 0, not StormReloc == 0.
        if (!Storm2Base) return 1.0f;
        const PALETTEENTRY* cur  = (const PALETTEENTRY*)(StormReloc + 0x1505E670);
        const PALETTEENTRY* base = GetMenuPalette(0);
        if (!cur || !base) return 1.0f;

        // 0..9 and 246..255 are the reserved system colours -- FadePalette() puts them
        // back from the system palette when it is done, so they never reach black and
        // would stop the ratio ever getting there. Only the art range is meaningful.
        unsigned curSum = 0, baseSum = 0;
        for (int i = 10; i < 246; ++i) {
            curSum  += (unsigned)cur[i].peRed  + cur[i].peGreen  + cur[i].peBlue;
            baseSum += (unsigned)base[i].peRed + base[i].peGreen + base[i].peBlue;
        }
        if (baseSum == 0) return 1.0f;                   // nothing to compare against

        float f = (float)curSum / (float)baseSum;
        if (f > 1.0f) f = 1.0f;
        if (f < 0.0f) f = 0.0f;
        // Deadband. A screen that patches storm's palette without also refreshing
        // MenuPalette (a logo, say) would otherwise sit a few percent under 1.0 forever
        // and dim the portrait permanently. A real fade blows straight through this.
        if (f > 0.95f) f = 1.0f;
        return f;
    }

} // namespace

void MenuPortrait_Set(const char* pcxName, int portraitId, HWND portraitWin,
                      const uchar* pcxData, int pcxWidth)
{
    if (!portraitWin) { gId = -1; gWin = 0; gRef8 = 0; gArmedWin = 0; return; }
    SheetKey(pcxName, gSheet, sizeof(gSheet));
    if (!gSheet[0]) { gId = -1; gWin = 0; gRef8 = 0; gArmedWin = 0; return; }
    // (th4) A change of portrait id alone must NOT re-arm the gate -- see the long
    // comment at the gate. Only a different control or a different sheet does.
    if (portraitWin != gArmedWin || strcmp(gSheet, gArmedSheet) != 0) gArmedWin = 0;
    gId = portraitId;
    gWin = portraitWin;
    gRef8 = pcxData;
    gRef8W = pcxWidth;
}

void MenuPortrait_Suppress(int on)
{
    // Counted, not a bool: SelectYesNo can be reached from a dialog that is itself
    // stacked, and a plain flag would be cleared by the inner box on the way out.
    gSuppress += on ? 1 : -1;
    if (gSuppress < 0) gSuppress = 0;
}

void MenuPortrait_Clear()
{
    gId = -1;
    gWin = 0;
    gRef8 = 0;
    gArmedWin = 0;      // (th4) next portrait must wait for its art again
}

void MenuPortrait_OnDeviceLost()
{
    ReleaseTexture();
}

void MenuPortrait_Shutdown()
{
    ReleaseTexture();
    // The strips are the expensive part -- decoded once and deliberately kept for the
    // process lifetime, which is why OnDeviceLost does NOT drop them. This is the only
    // place they go.
    for (int i = 0; i < gStripCount; ++i) {
        if (gStrips[i].rgba) { stbi_image_free(gStrips[i].rgba); gStrips[i].rgba = nullptr; }
    }
    gStripCount = 0;
    gId = -1; gWin = 0; gSheet[0] = 0;
    gArmedWin = 0; gArmedSheet[0] = 0;
}

// (th4) Rebuild the menu cursor and draw it over the portrait.
//
// The hand is already in the 8-bit dialog surface -- SDlg composited it there -- and the
// portrait quad is drawn on top of that surface, so it buries the cursor. Rather than try to
// lift the hand back out of the 8-bit pixels (there is no saved background for it, unlike the
// in-game cursor that ItemOverlay's DrawCursorPatch recovers from CursorImage), redraw it from
// the same buffer SDlg was given.
//
// The caller has already set up the blend and the TEXTUREFACTOR fade, and its state block is
// restored after this returns, so nothing needs saving here -- including the sampler filter
// overridden below. Reusing the fade is deliberate: the hand dims with the menu exactly as it
// did while it was part of the 8-bit surface.
static void DrawMenuCursorAbove(IDirect3DDevice9* dev,
                                const RECT& srcRect, const RECT& dstRect,
                                double sx, double sy,
                                float portraitLeft, float portraitTop,
                                float portraitRight, float portraitBottom)
{
    const uchar* image = CursorBuf;
    const int cw = (int)CursorSize.cx;
    const int ch = (int)CursorSize.cy;
    if (!image || cw <= 0 || ch <= 0) return;    // no gfx cursor loaded

    // (th4) GetCursorPos is SCREEN space, and the transform below wants CLIENT space --
    // the same mismatch the portrait rect had. It only cancelled out while the client
    // origin sat at screen (0,0); once Alt+Enter leaves a real window at StartWindowX/Y
    // the hand drifts by the window position plus the border. Convert, like the portrait.
    // SDlg draws the cursor with its TOP-LEFT at this point (sub_15008CC0 builds its
    // invalidate rect as x .. x + width), so there is no hotspot to subtract.
    POINT cp;
    if (!GetCursorPos(&cp)) return;
    if (ghMainWnd) ScreenToClient(ghMainWnd, &cp);
    const int cx = cp.x;
    const int cy = cp.y;

    // Same transform the portrait uses, so the hand lands exactly where the 8-bit one was.
    const float left   = (float)(dstRect.left + (cx - srcRect.left) * sx) - 0.5f;
    const float top    = (float)(dstRect.top  + (cy - srcRect.top ) * sy) - 0.5f;
    const float right  = left + (float)(cw * sx);
    const float bottom = top  + (float)(ch * sy);

    // Only bother when it is actually over the portrait -- everywhere else the 8-bit hand in
    // the surface underneath is already correct, and drawing a second one would only risk
    // a filtering seam.
    if (right < portraitLeft || left > portraitRight ||
        bottom < portraitTop || top  > portraitBottom) return;

    if (!gCursorTex || gCursorTexW < cw || gCursorTexH < ch) {
        if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; }
        if (D3D_OK != dev->CreateTexture(cw, ch, 1, D3DUSAGE_DYNAMIC,
                                         D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                         &gCursorTex, nullptr)) return;
        gCursorTexW = cw; gCursorTexH = ch;
    }

    D3DLOCKED_RECT lr;
    if (D3D_OK != gCursorTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

    // Storm's own working palette -- the menu is NOT drawn with ScreenPalette. Same pointer
    // ConvertPaletteSurfaceToTrueColor picks for the !gbRunGame case.
    const PALETTEENTRY* pal = (const PALETTEENTRY*)(StormReloc + 0x1505E670);
    uchar* dstBase = (uchar*)lr.pBits;
    for (int y = 0; y < ch; ++y) {
        const uchar* iRow = image + (size_t)y * cw;
        uchar* dstRow = dstBase + (size_t)y * lr.Pitch;
        for (int x = 0; x < cw; ++x) {
            // LoadGfxCursor sets CursorAnim to 0 wherever CursorBuf is non-zero and 0xFF
            // elsewhere, so the AND/OR blit reduces to: ink exactly where CursorBuf != 0.
            // Reading the mask as well would only restate that.
            const uchar idx = iRow[x];
            const PALETTEENTRY& p = pal[idx];
            dstRow[0] = p.peBlue; dstRow[1] = p.peGreen; dstRow[2] = p.peRed;
            dstRow[3] = idx ? 255 : 0;
            dstRow += 4;
        }
    }
    gCursorTex->UnlockRect(0);

    const float u1 = (float)cw / (float)gCursorTexW;
    const float v1 = (float)ch / (float)gCursorTexH;
    TransformedVertexT v[4] = {
        { left,  top,    0.0f, 1.0f, 0.0f, 0.0f },
        { right, top,    0.0f, 1.0f, u1,   0.0f },
        { left,  bottom, 0.0f, 1.0f, 0.0f, v1  },
        { right, bottom, 0.0f, 1.0f, u1,   v1  },
    };
    // POINT sampling: the cursor is a hard-edged 1:1 sprite and LINEAR would fringe it.
    dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    dev->SetTexture(0, gCursorTex);
    dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));
}

void MenuPortrait_DrawOverlay(IDirect3DDevice9* dev, const RECT& srcRect, const RECT& dstRect,
                              const uchar* menu8, int menuPitch)
{
    if (!dev) return;
    if (gId < 0 || !gWin) return;                    // no portrait showing
    if (gSuppress > 0) return;                       // a modal box is on top of it

    // The dialog may have CLOSED since the last paint -- paints only happen when the
    // selection changes, so nothing would otherwise tell us. Ask the control itself
    // every frame, which also picks up a move.
    //
    // This does NOT detect being covered: a dialog behind a modal one stays visible, and
    // neither IsWindowEnabled on the parent (the menu keeps it disabled anyway) nor
    // WindowFromPoint gave a usable answer. Covering is reported explicitly instead --
    // see MenuPortrait_Suppress and its caller in SelectYesNo.
    if (!IsWindow(gWin) || !IsWindowVisible(gWin)) { gId = -1; gWin = 0; gArmedWin = 0; return; }
    RECT wr;
    if (!GetWindowRect(gWin, &wr)) { gId = -1; gWin = 0; gArmedWin = 0; return; }

    // (th4) GetWindowRect gives SCREEN coordinates, but the transform below treats
    // gGuiX/gGuiY as offsets inside srcRect -- the 640x480 dialog space, which is
    // anchored at the CLIENT origin. Those two agree only while the client origin
    // happens to sit at screen (0,0), which is true for a borderless full-screen
    // window and false the moment there is a title bar or the window is not at the
    // top-left corner. With IsFullScreen=win (BigWin) the game starts borderless at
    // 0,0 and looks right; Alt+Enter drops it to a real window at StartWindowX/Y, and
    // from then on the portrait is drawn offset by exactly the window position plus
    // the non-client border -- it slides down and right off the frame it belongs in.
    // Map to client space so the two spaces agree in every window state.
    POINT wp = { wr.left, wr.top };
    if (ghMainWnd) ScreenToClient(ghMainWnd, &wp);

    gGuiX = wp.x; gGuiY = wp.y;
    gGuiW = wr.right - wr.left; gGuiH = wr.bottom - wr.top;
    if (gGuiW <= 0 || gGuiH <= 0) return;

    // (th4) IS THE 8-BIT PORTRAIT ON SCREEN YET?
    //
    // The control exists, and is "visible", from the dialog's init -- but SDlg paints
    // the new dialog's art into its surface a frame or two later, and until then the
    // previous screen is still what is being presented. Drawing on "control exists"
    // put the true-colour portrait over the main menu before the hero list replaced
    // it, which is exactly what it looked like: the portrait arrived first.
    //
    // This overlay REPLACES the 8-bit portrait SDlgSetBitmap put in the same control,
    // so the honest gate is "that art is there now". Compare the middle row of the
    // presented surface against the same row of the sheet slice the control was given.
    // Palette fades do not disturb it -- they change the palette, not the indices.
    // A near-match is enough: SDlg leaves the cursor and the selection pentagram in
    // the surface, and they can cross the row.
    // ARMED ONCE PER CONTROL+SHEET, NOT PER ID. (th4)
    //
    // The test below asks "is the 8-bit art on screen yet", and it has to compare against
    // a specific row -- so it necessarily fails for one frame after EVERY selection
    // change, not just on the way into a dialog: HeroSelectionChanged calls
    // SDlgSetBitmap and MenuPortrait_Set together, but the surface being presented that
    // frame still holds the PREVIOUS row. The overlay was skipped, leaving the bare 8-bit
    // portrait visible for exactly one frame -- which is invisible while hero_pix.pcx
    // holds real art, and a red flash once it is a placeholder for the true-colour pack.
    //
    // The question it is really there to answer is "has THIS DIALOG's art arrived",
    // asked once on the way in (without it the portrait appeared over the main menu
    // before the hero list replaced it). Once the control has shown one portrait from
    // this sheet, scrolling within it needs no further proof, so the gate latches.
    //
    // The overlay then draws the NEW portrait a frame before the 8-bit row beneath it
    // catches up. That is not visible: the quad covers the control rect completely, so
    // the stale row underneath is never sampled.
    //
    // Re-armed by MenuPortrait_Set on a different control or sheet, by
    // MenuPortrait_Clear, and whenever the control goes away -- so every fresh entry
    // into a dialog still waits, exactly as before.
    if (gWin != gArmedWin) {
        if (gRef8 && menu8 && menuPitch > 0 && gRef8W > 0 && gId >= 0) {
            const int mid = gGuiH / 2;
            const int n = gGuiW < gRef8W ? gGuiW : gRef8W;
            if (gGuiY + mid >= 0 && n > 0) {
                const uchar* have = menu8 + (size_t)(gGuiY + mid) * menuPitch + gGuiX;
                const uchar* want = gRef8 + ((size_t)gId * gGuiH + mid) * gRef8W;
                int same = 0;
                for (int x = 0; x < n; ++x) same += (have[x] == want[x]);
                if (same * 10 < n * 9) return;       // not painted yet: hold the overlay
            }
        }
        gArmedWin = gWin;                            // proven once; scrolling is free
        strncpy(gArmedSheet, gSheet, sizeof(gArmedSheet) - 1);
        gArmedSheet[sizeof(gArmedSheet) - 1] = 0;
    }

    Strip* strip = LoadStrip(gSheet);
    if (!strip) return;                              // no strip -> 8-bit art stays visible
    if (!UploadSlice(dev, strip, gId, gGuiW, gGuiH)) return;

    // RenderSurface stretched srcRect (the 640x480 menu) onto dstRect (back-buffer
    // pixels). Put the portrait through exactly the same transform so it lands on the
    // 8-bit art underneath it at any window size or filtering mode.
    const int sw = srcRect.right - srcRect.left;
    const int sh = srcRect.bottom - srcRect.top;
    const int dw = dstRect.right - dstRect.left;
    const int dh = dstRect.bottom - dstRect.top;
    if (sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;

    const double sx = (double)dw / (double)sw;
    const double sy = (double)dh / (double)sh;

    const float left   = (float)(dstRect.left + (gGuiX - srcRect.left) * sx) - 0.5f;
    const float top    = (float)(dstRect.top  + (gGuiY - srcRect.top ) * sy) - 0.5f;
    const float right  = left + (float)(gGuiW * sx);
    const float bottom = top  + (float)(gGuiH * sy);

    TransformedVertexT v[4] = {
        { left,  top,    0.0f, 1.0f, 0.0f, 0.0f },
        { right, top,    0.0f, 1.0f, 1.0f, 0.0f },
        { left,  bottom, 0.0f, 1.0f, 0.0f, 1.0f },
        { right, bottom, 0.0f, 1.0f, 1.0f, 1.0f },
    };

    // Snapshot everything. Same reasoning as AnimOrbs: the surrounding code leaves its
    // own state set up and we must hand it back untouched.
    IDirect3DStateBlock9* sb = nullptr;
    if (D3D_OK != dev->CreateStateBlock(D3DSBT_ALL, &sb)) sb = nullptr;

    dev->SetVertexShader(nullptr);
    dev->SetPixelShader(nullptr);
    dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    dev->SetTexture(0, gTex);
    dev->SetTexture(1, nullptr);
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
    // Track the palette fade. RGB only: the alpha stays as authored so the portrait keeps
    // covering exactly the same pixels, and both it and the 8-bit art underneath walk down
    // to black together instead of the portrait popping out of a blacked-out menu.
    // (th4) PERCEPTUAL SYNC WITH THE 8-BIT FADE.
    //
    // MenuFadeLevel() is the exact factor the palette is scaled by, and applying it
    // linearly puts the overlay numerically in step with the art around it. It still
    // READS as arriving first: the portrait is a bright, high-contrast image and the
    // menu around it is dark grey and dull gold, and under a linear ramp a bright thing
    // becomes noticeable at a much lower level than a dark one. So the overlay's ramp
    // is steepened -- it stays dark until the surrounding art is already coming up,
    // and the two look simultaneous. Exponent tuned by eye; 1.0 is the old behaviour,
    // larger holds the portrait back longer. Only the ramp changes: at 1.0 it is still
    // exactly full brightness, and at 0 it is still exactly black.
    // Linear: exactly the factor the palette is scaled by. (A steeper ramp was tried
    // while the bug above hid the real cause; with tracking working, linear IS in
    // step with the 8-bit portrait it replaces. kFadeGamma is left as the knob.)
    static const float kFadeGamma = 1.0f;
    const float fade = kFadeGamma == 1.0f ? MenuFadeLevel() : powf(MenuFadeLevel(), kFadeGamma);
    const DWORD lvl  = (DWORD)(fade * 255.0f + 0.5f);
    dev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, lvl, lvl, lvl));
    dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    dev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TransformedVertexT));

    // (th4) ...and put the mouse cursor back on top of it. Must be INSIDE the state block, so
    // it inherits the blend and the fade and leaves nothing of its own behind.
    DrawMenuCursorAbove(dev, srcRect, dstRect, sx, sy, left, top, right, bottom);

    if (sb) { sb->Apply(); sb->Release(); }
}
