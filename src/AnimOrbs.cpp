// AnimOrbs.cpp
// See AnimOrbs.h for overview.
//
// Rendering approach (Option A): in the D3D textured-quad path the game uploads
// the 8-bit WorkingSurface to a texture and a palette shader converts it on the
// GPU while drawing one fullscreen quad. We draw AFTER that quad, as two extra
// alpha-blended textured quads (life + mana), using a fixed-function-style
// pre-transformed vertex (D3DFVF_XYZRHW) and NO pixel shader, so the frames are
// composited in full RGBA with zero palette loss.

#include "stdafx.h"      // common engine precompiled header (windows + d3d9 types)
#include "AnimOrbs.h"

#include <d3d9.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uchar;
typedef long long      i64;

// MPQ file loader: returns a SMalloc'd buffer of the file from th4data.mor.
void* __fastcall LoadFile(const char* fileName, int* byteReaded);
// Storm free (matches LoadFile's allocator).
int    __stdcall  eSMemFree(void* location, char* logfilename, int logline, int defaultValue);
#define ANIMORBS_FREE(p) eSMemFree( (p), (char*)__FILE__, __LINE__, 0 )

// engine millisecond clock (fps-independent timing)
i64 GetTickCountTh();

// config gates + screen geometry (from vars.h / Config.cpp)
extern bool UserPanelB;
extern bool UserPotionB;
extern int  ScreenWidth;
extern int  ScreenHeight;
extern int  gbRunGame;       // nonzero only during actual gameplay
extern bool IsInterface;     // bottom panel visible
extern char* LoadingScreenCEL; // non-null while a loading screen is showing
// (th4) Panel.cpp picks the globe art from GameMode + UserPanelB; the orb overlay
// must follow the SAME choice or the animation will not match the globe under it.
// GameMode is declared in vars.h (type GAME_MODE) and reaches us via stdafx.h, so
// it is deliberately NOT re-declared here -- an "extern int GameMode" would be a
// type mismatch against the real definition.

// Fill ratios (0..80) for clipping the orbs to current life/mana. Player struct
// comes from structs.h (included via stdafx).
extern Player* Players;
extern unsigned int CurrentPlayerIndex;

// --- cursor redraw support ---------------------------------------------------
// The cursor is composited into the 8-bit WorkingSurface BEFORE our orb quad
// draws over it, so it vanishes wherever an orb covers it. We copy the cursor's
// drawn rect back out of WorkingSurface (still intact), convert via ScreenPalette
// to RGBA, and draw it on top of the orbs.
extern unsigned char* WorkingSurface;
extern int   WorkingWidth;
extern PALETTEENTRY ScreenPalette[256];
extern int   CurCursorStartX;   // live cursor rect, set by RenderCursor each frame
extern int   CurCursorStartY;
extern int   CurCursorWidth;
extern int   CurCursorHeight;
extern unsigned char CursorImage[]; // saved pre-cursor background (packed, CurCursorWidth wide)
extern unsigned char* MainPanelImage; // static panel art, [height][640] panel-space 8-bit
// Screen_LeftBorder / Screen_TopBorder are enum constants in enums.h.

// GUI design space (enums.h): GUI_Width=640, GUI_Height=480
static const int kGuiW = 640;
static const int kGuiH = 480;

// Globe placement in GUI space. The 88x88 orb wells sit with their left edges at
// these x coordinates; y top is derived from the classic panel layout (the orb
// art occupies the lower-panel globe wells). Tunable if alignment needs nudging.
// (th4) external linkage: Draw.cpp clears the alpha overlay under the wells
// (the semi-transparent glass must not reveal an occluded sprite behind it).
extern const int kLifeGuiX = 96;    // life globe well, panel X
extern const int kManaGuiX = 461;   // mana globe well, panel X
extern const int kOrbGuiY = 338;   // panel Y of orb well top  <-- TUNE THIS

// Fill baseline nudge (px): anchors the drained-orb surface bottom relative to
// the orb art's bottom edge. 0 = fill bottom at art bottom. Negative raises it.
static const int kFillBottomAdj = 0;
// Fillable span (px): pixel height a FULL orb (ratio 80) occupies in the art.
// 88 = the whole frame. Lower it if the overlay reads fuller than the real globe
// (overlay edge too high); raise it if the overlay reads emptier.
static const int kFillSpanPx = 88;
// Alpha-feather (px) at the fill's top cut edge so it blends into the empty well
// instead of forming a hard line. 0 = sharp. ~4-8 looks natural.
static const int kFillFeatherPx = 5;

// --- overlays that must stay ON TOP of the orbs ------------------------------
// The demon/priest head art and the life/mana number text are drawn into
// WorkingSurface and would be hidden by the orb quad. We redraw those small
// regions from WorkingSurface (palette->RGBA) on top of the orbs.
//
// HEAD rectangles are in SCREEN pixels relative to each orb's screen position
// (orb screen origin = panelX + (ScreenWidth-640)/2 , kOrbGuiY + (ScreenHeight-480)).
// Tune these so they cover exactly the head art at the bottom of each well.
static const bool kRedrawHeads = true;
static const int  kHeadOffX = 24;   // X inset from orb left edge
static const int  kHeadOffY = 56;   // Y inset from orb top edge
static const int  kHeadW = 40;   // head width
static const int  kHeadH = 32;   // head height
static const int  kHeadSat = 50;   // min saturation (max-min channel) to be "orb fill"

// (th4) RED-VISION KEY SNAPSHOT.
//
// Terminator "Seeing" (GC_27 + Infravision) ends the software frame with
// DrawRedScreen (Panel.cpp ~2378), which walks the WHOLE working surface and
// XLATs every index >= 0x20 through a LightTable red ramp. BOTH keys below are
// hue tests, and the ramp destroys hue:
//   mode 2 (heads) drops "strongly red- or blue-dominant" as globe fill. After
//     the ramp every pixel is red-dominant, so the head art tested as fill and
//     vanished wherever it overlaps the orbs.
//   mode 1 (digits) keeps a dim pixel only when NEUTRAL (mx - mn <= kTextNeutral),
//     because the font ramp is pure grey at every step. After the ramp the grey
//     ramp is red, neutrality fails, and the anti-aliased glyph shoulders drop --
//     the same thin, chewed-up digits as the gamma bug, from a different cause.
//
// Same remedy as the gamma fix: classify from data the effect has not touched,
// while still EMITTING the current screen colour so the patch stays red like
// everything else. DrawRedScreen calls AnimOrbs_CaptureKeyBand() before it remaps.
//
// ONE BAND rather than per-rect buffers: the head rects sit inside the globes and
// the digit strips span BETWEEN them, overlapping both, so a single bounding box
// covers every rect either mode can ask for and any sub-rect just indexes into it.
// Worst case is about 88 rows of ~460 px = 40 KB, allocated once.
static const int kKeyBandW = kManaGuiX - kLifeGuiX + ANIMORBS_SIZE; // 453
static const int kKeyBandH = ANIMORBS_SIZE;                        // 88
static uchar gKeyBand[kKeyBandW * kKeyBandH];
static int   gKeyBandX = 0, gKeyBandY = 0;   // screen origin of the band
static bool  gKeyBandValid = false;
static const int  kHeadDom = 30;   // how much the dominant channel must lead the others

// TEXT strip: centered at ScreenWidth/2 +/- 182, baseline y = ScreenHeight-89.
// We redraw a horizontal strip around each number.
static const bool kRedrawText = true;
static const int  kTextHalfW = 40;   // half-width of the strip around center
static const int  kTextTopOff = 12;   // pixels above the baseline
static const int  kTextH = 24;   // strip height
static const int  kTextCenterDX = 182;   // matches Panel.cpp (ScreenWidth/2 +/- 182)
static const int  kTextBaseY = 89;    // matches Panel.cpp (ScreenHeight - 89)
static const int  kTextLuma = 140;   // brightness cutoff: pixels >= this are kept as text
static const int  kTextDarkLuma = 40;    // darkness cutoff: pixels with max channel <= this are candidate outline
static const int  kTextNeutral = 18;    // max channel spread to count as neutral black (reject dark-colored orb)
// (th4) Content-proximity keying: without it the two patch rects read as a faint
// T-shaped overlay on the animated orb, because background pixels of the strip /
// head rect were kept opaque. Now candidate pixels survive only near definite
// content pixels, so the well background stays transparent.
static const int  kTextOutlineReach = 2;  // px: neutral-black kept only this close to a bright digit pixel
static const int  kHeadBrightMin = 56; // min max-channel for a pixel to count as definite head art
static const int  kHeadReach = 2;  // px: other non-fill pixels kept only this close to head art

// ---------------------------------------------------------------------------
// stb_image (single-header image decoder), vendored. This is the ONLY translation
// unit that instantiates it, so the format list here decides what every user of
// stb_image in the project can decode -- MenuPortrait.cpp reads BMP and JPG strips
// through the same instance. PNG alone was enough while the orbs were the only
// caller; STBI_ONLY_PNG made everything else report "unknown image type" even for a
// file with valid magic bytes.
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#include "stb_image.h"
// ---------------------------------------------------------------------------

namespace {

    struct OrbSet {
        // Tightly packed frames: [frame][y][x][b,g,r,a] -- stored in the texture's own byte
        // order (D3DFMT_A8R8G8B8 is BGRA in memory), swapped once at load (LoadOrbSet), so an
        // upload is a plain row copy. Byte 3 is alpha in either order, which is all the well
        // clears (AnimOrbs_ClearOverlayWells / ClearTintWells) read. NULL until loaded.
        uchar* frames = nullptr;         // ANIMORBS_FRAME_COUNT * 88*88*4 bytes
        int    loadedCount = 0;
        IDirect3DTexture9* tex = nullptr; // single reusable dynamic texture (88x88)
        // (th4) PERFORMANCE: the frame currently in `tex`. The animation only advances ~10
        // times a second, while this runs every RENDERED frame (300+ fps with smooth
        // scrolling), so UploadFrame skips the upload when the frame has not changed.
        // -1 = texture content unknown (new, or lost with the device).
        int    uploadedFrame = -1;
    };

    OrbSet gLife;
    OrbSet gMana;
    // (th4) Which mana art is currently in gMana. The panel can switch between blue
    // and dark mana at runtime (menu "Mana: blue" / "Mana: dark" -> UserPanelB), so
    // the loaded set has to be swapped when that changes, not chosen once at startup.
    const char* gManaFolder = nullptr;
    // (th4) Which life art is in gLife: the red Health_orb1, or the blue Health_orb_IA of the
    // Ice Age panel (see WantedLifeFolder). Swapped at runtime like the mana set.
    const char* gLifeFolder = nullptr;
    bool   gInitDone = false;
    int    gFps = ANIMORBS_DEFAULT_FPS;

    // Reusable texture for the cursor patch redrawn on top of the orbs.
    IDirect3DTexture9* gCursorTex = nullptr;
    int gCursorTexW = 0, gCursorTexH = 0;

    // (th4) PATCH CACHE. The heads, the life/mana numbers and the tooltip lifts are rebuilt
    // from WorkingSurface (RedrawRectOverOrbs) -- a two-pass colour key plus an upload. They
    // used to share ONE texture that was locked, filled and drawn four or more times per
    // rendered frame. Each call site now owns a slot with its own texture and remembers what
    // it was built from (rect, mode, the source pixels, the key band, the palettes); when all
    // of that is unchanged the cached texture is simply drawn again. Heads are built once,
    // numbers when they change. Slots: 0/1 heads, 2/3 numbers, 4/5 tooltip lifts (life/mana).
    enum { kPatchSlots = 6, kPatchMaxPixels = ANIMORBS_SIZE * ANIMORBS_SIZE };
    struct PatchSlot {
        IDirect3DTexture9* tex = nullptr;
        int texW = 0, texH = 0;
        bool valid = false;
        int rx = 0, ry = 0, rw = 0, rh = 0, mode = -1, luma = 0;
        unsigned palHash = 0;
        bool hadBand = false;
        uchar src[kPatchMaxPixels];
        uchar band[kPatchMaxPixels];
    };
    PatchSlot gPatch[kPatchSlots];
    unsigned gPalHash = 0;          // ScreenPalette + BasePalette, computed once per drawn frame
    // (th4) One state block for the whole session, re-captured each frame (CreateStateBlock of
    // D3DSBT_ALL every frame is one of the most expensive D3D9 calls). Released on device loss.
    IDirect3DStateBlock9* gStateBlock[2] = { nullptr, nullptr };	// one per device (second window)
    IDirect3DDevice9*     gStateBlockDev[2] = { nullptr, nullptr };

    const int kFrameBytes = ANIMORBS_SIZE * ANIMORBS_SIZE * 4;

    // (th4) Shared glass-coverage threshold for BOTH per-pixel UI clears (the
    // Stage-2 alpha overlay clear and the Stage-3 tint clear): a glass pixel
    // counts as "the orb is really there" when its alpha reaches this. Below
    // it the glass is visually absent, so the pixel keeps its overlay ghost AND
    // its tint — clearing either one there would punch a visible hard-edged
    // hole around the orb's semi-transparent rim. Using one threshold for both
    // surfaces guarantees the two clears never disagree on a pixel.
    const int kClearGlassAlpha = 128;

    // (th4) Mirror of the globe-art choice in Panel.cpp (LoadPanelArt):
    //     GameMode == GM_CLASSIC -> "panel8"                   crimson life & BLUE mana
    //     UserPanelB             -> "front_panel_mana_blue"    red life  & BLUE mana
    //     iceage                 -> "..._black_iceage"         blue life & BLACK mana
    //     else                   -> "front_panel_mana_black"   red life  & BLACK mana
    // Only the mana globe changes colour between these, so only the mana orb set is
    // selected here; the life orb art (Health_orb1) is red in every branch that we
    // draw over. See ManaIsBlue() callers.
    // Classic is not handled here: it never draws these orbs at all (see
    // AnimOrbs_Active), so its globe colours are irrelevant to the choice.
    bool ManaIsBlue() { return UserPanelB; }   // "Mana: blue" vs "Mana: dark"
    const char* WantedManaFolder() { return ManaIsBlue() ? "Mana_orb1" : "Mana_orb2_black"; }

    // (th4) The life globe is red in every panel variant EXCEPT the Ice Age one,
    // which draws a BLUE life globe ("front_panel_mana_black_iceage"). That one gets its
    // own animated set, data\orbs_v1\Health_orb_IA: the Mana_orb1 frames with the blue
    // channel lifted (a gamma curve on B only, R/G and alpha untouched, rim pixels
    // weighted by alpha) to a median blue of ~122 of 255 -- darker than the static
    // Ice Age globe (~172), which read too bright once animated. Same 1500 frames, 88x88
    // RGBA, 0001..1500.png as every other set.
    // Mana stays black there, as on the default panel.
    // The condition mirrors the `iceage` local in Panel.cpp exactly.
    bool LifeIsRed()
    {
        if (UserPanelB) return true;               // "front_panel_mana_blue": red life
        const bool iceage = (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE))
                            && !UserPotionB && !UserPanelB;
        return !iceage;                            // else "front_panel_mana_black": red
    }
    const char* WantedLifeFolder() { return LifeIsRed() ? "Health_orb1" : "Health_orb_IA"; }

    // Load one orb folder's 1500 PNGs into a single RAM block.
    bool LoadOrbSet(OrbSet& set, const char* folder)
    {
        if (set.frames) return true;
        set.frames = (uchar*)malloc((size_t)kFrameBytes * ANIMORBS_FRAME_COUNT);
        if (!set.frames) return false;
        memset(set.frames, 0, (size_t)kFrameBytes * ANIMORBS_FRAME_COUNT);

        char path[256];
        int  ok = 0;
        for (int i = 0; i < ANIMORBS_FRAME_COUNT; ++i) {
            // files are 0001.png .. 1500.png
            sprintf(path, "data\\orbs_v1\\%s\\%04d.png", folder, i + 1);

            int   bytes = 0;
            void* raw = LoadFile(path, &bytes);
            if (!raw || bytes <= 0) continue;

            int w = 0, h = 0, comp = 0;
            // Force 4 channels (RGBA) regardless of source.
            uchar* px = stbi_load_from_memory((const stbi_uc*)raw, bytes, &w, &h, &comp, 4);
            ANIMORBS_FREE(raw);
            if (!px) continue;

            if (w == ANIMORBS_SIZE && h == ANIMORBS_SIZE) {
                uchar* dstF = set.frames + (size_t)i * kFrameBytes;
                for (int k = 0; k < kFrameBytes; k += 4) {	// RGBA -> BGRA, once, at load
                    dstF[k + 0] = px[k + 2]; dstF[k + 1] = px[k + 1];
                    dstF[k + 2] = px[k + 0]; dstF[k + 3] = px[k + 3];
                }
                ++ok;
            }
            stbi_image_free(px);
        }
        set.loadedCount = ok;
        return ok > 0;
    }

    // Upload one frame's RGBA into the set's dynamic texture.
    void UploadFrame(OrbSet& set, int frame)
    {
        if (!set.tex || !set.frames) return;
        if (frame < 0 || frame >= ANIMORBS_FRAME_COUNT) return;
        if (frame == set.uploadedFrame) return;     // (th4) already in the texture

        D3DLOCKED_RECT lr;
        if (D3D_OK != set.tex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

        const uchar* src = set.frames + (size_t)frame * kFrameBytes; // already BGRA
        uchar* dstBase = (uchar*)lr.pBits;
        for (int y = 0; y < ANIMORBS_SIZE; ++y) {
            memcpy(dstBase + (size_t)y * lr.Pitch, src + (size_t)y * ANIMORBS_SIZE * 4, ANIMORBS_SIZE * 4);
        }
        set.tex->UnlockRect(0);
        set.uploadedFrame = frame;
    }

    bool CreateOrbTexture(IDirect3DDevice9* dev, OrbSet& set)
    {
        if (set.tex) return true;
        set.uploadedFrame = -1;
        return D3D_OK == dev->CreateTexture(
            ANIMORBS_SIZE, ANIMORBS_SIZE, 1, D3DUSAGE_DYNAMIC,
            D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &set.tex, nullptr);
    }

    void ReleaseOrbTexture(OrbSet& set)
    {
        if (set.tex) { set.tex->Release(); set.tex = nullptr; }
        set.uploadedFrame = -1;
    }

    // Same pre-transformed vertex layout the engine's quad uses.
    struct OrbVertex { float x, y, z, rhw; float u, v; };
    // Variant with a diffuse color, used to alpha-fade the fill's top edge.
    struct OrbVertex2 { float x, y, z, rhw; unsigned int color; float u, v; };

    // Draw one orb texture as an alpha-blended quad covering [gx,gy]..+88 in GUI
    // space, scaled to the current render target.
    void DrawOrbQuad(IDirect3DDevice9* dev, IDirect3DTexture9* tex,
        int panelX, int panelY, int ratio)
    {
        if (!tex) return;
        if (ratio <= 0) return;          // empty: draw nothing, drained well shows
        if (ratio > 80) ratio = 80;

        // Native-pixel placement using the panel's own anchor:
        //   screenX = panelX + (ScreenWidth  - 640)/2
        //   screenY = panelY + (ScreenHeight - 480)
        const float ox = (float)((ScreenWidth - 640) / 2);
        const float oy = (float)((ScreenHeight - 480));

        const float fullTop = panelY + oy - 0.5f;
        const float fullBottom = fullTop + ANIMORBS_SIZE;
        const float left = panelX + ox - 0.5f;
        const float right = left + ANIMORBS_SIZE;

        // Engine drains bottom-up. ratio is 0..80 where 80 == a FULL orb. Map that
        // onto the art's fillable height (kFillSpanPx, default = full 88px art):
        //   fillPx = ratio/80 * kFillSpanPx
        // Anchor the fill BOTTOM to the orb art bottom (+kFillBottomAdj nudge) and
        // reveal fillPx upward.
        const float fillBottom = fullBottom + (float)kFillBottomAdj;
        float fillPx = (float)ratio * (float)kFillSpanPx / 80.0f;
        float top = fillBottom - fillPx;
        float bottom = fillBottom;
        if (bottom > fullBottom) bottom = fullBottom;
        if (top < fullTop)    top = fullTop;
        if (bottom <= top) return;
        const float vTop = (top - fullTop) / (float)ANIMORBS_SIZE;
        const float vBottom = (bottom - fullTop) / (float)ANIMORBS_SIZE;

        // Feather the cut edge so it isn't a hard line. The top `feather` px fade
        // alpha 0->1; the rest is solid. Uses a diffuse-modulated vertex format.
        const float feather = (float)kFillFeatherPx;
        float bodyTop = top + feather;
        if (bodyTop > bottom) bodyTop = bottom;
        const float vBodyTop = (bodyTop - fullTop) / (float)ANIMORBS_SIZE;

        OrbVertex2 body[4] = {
            { left,  bodyTop, 0.f, 1.f, 0xFFFFFFFF, 0.f, vBodyTop },
            { right, bodyTop, 0.f, 1.f, 0xFFFFFFFF, 1.f, vBodyTop },
            { left,  bottom,  0.f, 1.f, 0xFFFFFFFF, 0.f, vBottom  },
            { right, bottom,  0.f, 1.f, 0xFFFFFFFF, 1.f, vBottom  },
        };

        dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        // (th4) Colour follows the palette fade: TEXTUREFACTOR was set by the caller
        // from PaletteFadeLevel. Only the orb art takes it -- the head/text/cursor
        // patches drawn afterwards are lifted from the already-faded 8-bit frame and
        // would go dark twice, so the restore below puts SELECTARG1 back for them.
        dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        dev->SetTexture(0, tex);

        if (feather > 0.5f && bodyTop > top) {
            OrbVertex2 fade[4] = {
                { left,  top,     0.f, 1.f, 0x00FFFFFF, 0.f, vTop     },
                { right, top,     0.f, 1.f, 0x00FFFFFF, 1.f, vTop     },
                { left,  bodyTop, 0.f, 1.f, 0xFFFFFFFF, 0.f, vBodyTop },
                { right, bodyTop, 0.f, 1.f, 0xFFFFFFFF, 1.f, vBodyTop },
            };
            dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fade, sizeof(OrbVertex2));
        }
        dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, body, sizeof(OrbVertex2));

        // Restore the plain texture-only state the head/text/cursor patches expect.
        dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
        dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
        dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }

    // Returns true if rect A overlaps rect B.
    static bool RectsOverlap(int ax, int ay, int aw, int ah,
        int bx, int by, int bw, int bh)
    {
        return ax < bx + bw && ax + aw > bx &&
            ay < by + bh && ay + ah > by;
    }

    // Copy the cursor's drawn rectangle out of the 8-bit WorkingSurface, convert
    // through ScreenPalette to RGBA, and draw it over the orbs so the cursor stays
    // visible. WorkingSurface still holds the composited cursor at present time;
    // our orb quad only overwrote the backbuffer, not WorkingSurface.
    void RedrawCursorOverOrbs(IDirect3DDevice9* dev)
    {
        const int cw = CurCursorWidth;
        const int ch = CurCursorHeight;
        if (cw <= 0 || ch <= 0) return;

        const int cx = CurCursorStartX;   // screen-space X
        const int cy = CurCursorStartY;   // screen-space Y

        // Only bother if the cursor overlaps either orb rectangle.
        const int ox = (ScreenWidth - 640) / 2;
        const int oy = (ScreenHeight - 480);
        const int lifeX = kLifeGuiX + ox, manaX = kManaGuiX + ox, orbY = kOrbGuiY + oy;
        if (!RectsOverlap(cx, cy, cw, ch, lifeX, orbY, ANIMORBS_SIZE, ANIMORBS_SIZE) &&
            !RectsOverlap(cx, cy, cw, ch, manaX, orbY, ANIMORBS_SIZE, ANIMORBS_SIZE))
            return;

        // (Re)create the cursor patch texture if it needs to grow.
        if (!gCursorTex || gCursorTexW < cw || gCursorTexH < ch) {
            if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; }
            if (D3D_OK != dev->CreateTexture(cw, ch, 1, D3DUSAGE_DYNAMIC,
                D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &gCursorTex, nullptr))
                return;
            gCursorTexW = cw; gCursorTexH = ch;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != gCursorTex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;

        const int LB = (int)Screen_LeftBorder;
        const int TB = (int)Screen_TopBorder;
        uchar* dstBase = (uchar*)lr.pBits;
        const uchar* palBytes = (const uchar*)ScreenPalette;

        for (int y = 0; y < ch; ++y) {
            const uchar* srcRow = WorkingSurface
                + (size_t)WorkingWidth * (cy + y + TB) + (cx + LB);
            const uchar* bgRow = CursorImage + (size_t)y * cw; // saved background, packed
            uchar* dstRow = dstBase + (size_t)y * lr.Pitch;
            for (int x = 0; x < cw; ++x) {
                uchar idx = srcRow[x];
                const uchar* p = palBytes + (size_t)idx * sizeof(PALETTEENTRY);
                uchar r = p[0], g = p[1], b = p[2];
                dstRow[0] = b; dstRow[1] = g; dstRow[2] = r;
                // Only pixels that differ from the saved pre-cursor background are
                // actual cursor pixels -> opaque. Background pixels -> transparent,
                // so the orb shows through everywhere except the cursor itself.
                dstRow[3] = (idx != bgRow[x]) ? 255 : 0;
                dstRow += 4;
            }
        }
        gCursorTex->UnlockRect(0);

        const float left = (float)cx - 0.5f;
        const float top = (float)cy - 0.5f;
        const float right = left + cw;
        const float bottom = top + ch;
        const float u1 = (float)cw / (float)gCursorTexW;
        const float v1 = (float)ch / (float)gCursorTexH;
        OrbVertex v[4] = {
            { left,  top,    0.f, 1.f, 0.f, 0.f },
            { right, top,    0.f, 1.f, u1,  0.f },
            { left,  bottom, 0.f, 1.f, 0.f, v1 },
            { right, bottom, 0.f, 1.f, u1,  v1 },
        };
        dev->SetTexture(0, gCursorTex);
        dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(OrbVertex));
    }

    // Redraw an arbitrary screen-pixel rectangle from WorkingSurface (8-bit ->
    // RGBA via ScreenPalette) on top of the orbs. mode 0 = fully opaque (heads),
    // mode 1 = brightness-keyed: only bright pixels kept (text), dark/background
    // pixels transparent so the animated orb shows through around the glyphs.
    void RedrawRectOverOrbs(IDirect3DDevice9* dev, int slot, int rx, int ry, int rw, int rh,
        int mode = 0, int lumaThreshold = 140)
    {
        if (slot < 0 || slot >= kPatchSlots) return;
        PatchSlot& P = gPatch[slot];
        if (rw <= 0 || rh <= 0) return;
        // (th4) Red-vision key band -- see the note beside gKeyBand. Used only by the
        // two HUE-based keys, and only when this rect lies wholly inside the captured
        // band, so band and rect line up 1:1 with no clamping to get wrong.
        // snapStride/snapOrg are captured BEFORE the clamp below can move rx/ry.
        const bool inBand = gKeyBandValid && (mode == 1 || mode == 2)
            && rx >= gKeyBandX && ry >= gKeyBandY
            && rx + rw <= gKeyBandX + kKeyBandW
            && ry + rh <= gKeyBandY + kKeyBandH;
        const uchar* snapOrg = inBand
            ? gKeyBand + (size_t)(ry - gKeyBandY) * kKeyBandW + (rx - gKeyBandX)
            : nullptr;
        // Clamp to screen bounds.
        if (rx < 0) { rw += rx; rx = 0; }
        if (ry < 0) { rh += ry; ry = 0; }
        if (rx + rw > ScreenWidth)  rw = ScreenWidth - rx;
        if (ry + rh > ScreenHeight) rh = ScreenHeight - ry;
        if (rw <= 0 || rh <= 0) return;

        const int LB = (int)Screen_LeftBorder;
        const int TB = (int)Screen_TopBorder;

        // (th4) cache check: same rect, key, palettes, source pixels and key band -> reuse
        const bool cacheable = rw * rh <= kPatchMaxPixels;
        bool same = cacheable && P.valid && P.tex && P.rx == rx && P.ry == ry && P.rw == rw && P.rh == rh
            && P.mode == mode && P.luma == lumaThreshold && P.palHash == gPalHash && P.hadBand == (snapOrg != nullptr);
        for (int y = 0; same && y < rh; ++y) {
            const uchar* srcRow = WorkingSurface + (size_t)WorkingWidth * (ry + y + TB) + (rx + LB);
            if (memcmp(P.src + (size_t)y * rw, srcRow, rw) != 0) same = false;
            else if (snapOrg && memcmp(P.band + (size_t)y * rw, snapOrg + (size_t)y * kKeyBandW, rw) != 0) same = false;
        }
        if (!same) {

        if (!P.tex || P.texW < rw || P.texH < rh) {
            if (P.tex) { P.tex->Release(); P.tex = nullptr; }
            P.valid = false;
            if (D3D_OK != dev->CreateTexture(rw, rh, 1, D3DUSAGE_DYNAMIC,
                D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &P.tex, nullptr))
                return;
            P.texW = rw; P.texH = rh;
        }

        D3DLOCKED_RECT lr;
        if (D3D_OK != P.tex->LockRect(0, &lr, nullptr, D3DLOCK_DISCARD)) return;
        uchar* dstBase = (uchar*)lr.pBits;
        const uchar* palBytes = (const uchar*)ScreenPalette;
        // (th4) KEY OFF THE UNGAMMA'D PALETTE. The colour we emit must come from
        // ScreenPalette so the patch matches what is on screen, but every classification
        // below has to be a property of the ART, not of the user's gamma slider.
        // ApplyGammaToPalette raises dark values hardest -- pow(v/256, gamma) with gamma
        // as low as 0.30 -- so an empty (dark blue) mana-orb pixel that reads 28 blue at
        // gamma 100 climbs past kHeadBrightMin at gamma 50 and gets promoted from
        // "candidate" to "definite head art". A whole block of well background then goes
        // opaque, which is the maroon rectangle under the numbers. Saturation collapses
        // the same way, so the orbFill test stops rejecting the fill as well. Reading the
        // key from BasePalette makes both tests independent of gamma and contrast.
        const uchar* keyBytes = (const uchar*)BasePalette; // peR,peG,peB,peFlags

        // (th4) Two-pass keying for modes 1 & 2. Pass 1 classifies every pixel:
        //   1 = definite content (bright digit / bright head art)
        //   2 = candidate (digit outline black / darker head shading)
        // Pass 2 keeps candidates only within `reach` px of definite content, so
        // background pixels of the rect go transparent and the animated orb shows
        // through -- this removes the T-shaped overlay the old keying produced.
        static uchar contentMask[8192];
        const bool twoPass = (mode == 1 || mode == 2) && rw * rh <= (int)sizeof(contentMask);
        const int  reach = (mode == 1) ? kTextOutlineReach : kHeadReach;
        if (twoPass) {
            for (int y = 0; y < rh; ++y) {
                const uchar* srcRow = WorkingSurface
                    + (size_t)WorkingWidth * (ry + y + TB) + (rx + LB);
                uchar* m = contentMask + (size_t)y * rw;
                for (int x = 0; x < rw; ++x) {
                    const uchar kidx = snapOrg ? snapOrg[(size_t)y * kKeyBandW + x] : srcRow[x];
                    const uchar* p = keyBytes + (size_t)kidx * sizeof(PALETTEENTRY);
                    uchar r = p[0], g = p[1], b = p[2];
                    int mx = r > g ? (r > b ? r : b) : (g > b ? g : b);
                    int mn = r < g ? (r < b ? r : b) : (g < b ? g : b);
                    if (mode == 1) {
                        int luma = (r * 30 + g * 59 + b * 11) / 100;
                        bool bright = luma >= lumaThreshold;
                        // (th4) CLOSE THE GAP BETWEEN THE TWO CUTOFFS.
                        //
                        // A pixel used to be dropped outright unless it was either bright
                        // (luma >= lumaThreshold, 140) or near-black-and-neutral (max <= 40).
                        // The glyph ramp of a level palette does not oblige: Ravenholm's
                        // greys run 246,231,218,203,186,174,161,146,134,122,109,93,76,57,38,
                        // so SIX steps -- luma 134 down to 57 -- fell between the two and were
                        // made transparent. Those are the anti-aliased shoulders of every
                        // digit, so the numbers lost their soft edges and read thin against
                        // the bright animated orb underneath. It moves with the gamma slider
                        // too: at gamma 70 only four steps drop, at 50 only three.
                        //
                        // Anything below the bright cutoff is now a CANDIDATE rather than a
                        // drop. Candidates are not kept on their own -- pass 2 keeps one only
                        // within kTextOutlineReach px of a definite bright pixel -- so glyph
                        // shoulders and outline survive while orb-well background, which has
                        // no bright pixel near it, still goes transparent. The neutrality test
                        // is kept for the dark end: a dark COLOURED pixel is orb fill, not
                        // outline, and must not be promoted even when it sits beside a digit.
                        // NEUTRALITY, not darkness, is what separates glyph from orb: the
                        // font ramp is pure grey (r == g == b) at every step, while the orb
                        // well and fill are strongly coloured. So a pixel below the bright
                        // cutoff is a candidate whenever it is NEUTRAL, at any brightness --
                        // that admits the whole grey ramp and still rejects orb art. Keying
                        // on brightness alone would have let bright coloured fill (e.g.
                        // 200/30/30, luma 81) become a candidate and paint a red halo
                        // around the digits, which is the maroon-rectangle bug in miniature.
                        bool neutral = (mx - mn <= kTextNeutral);
                        m[x] = bright ? 1 : (neutral ? 2 : 0);
                    }
                    else {
                        int sat = mx - mn;
                        bool orbFill = (sat >= kHeadSat) &&
                            ((r == mx && r - g >= kHeadDom && r - b >= kHeadDom) ||
                                (b == mx && b - g >= kHeadDom && b - r >= kHeadDom));
                        m[x] = orbFill ? 0 : (mx >= kHeadBrightMin ? 1 : 2);
                    }
                }
            }
        }

        for (int y = 0; y < rh; ++y) {
            const uchar* srcRow = WorkingSurface
                + (size_t)WorkingWidth * (ry + y + TB) + (rx + LB);
            uchar* dstRow = dstBase + (size_t)y * lr.Pitch;
            for (int x = 0; x < rw; ++x) {
                uchar idx = srcRow[x];
                const uchar* p = palBytes + (size_t)idx * sizeof(PALETTEENTRY);
                uchar r = p[0], g = p[1], b = p[2];
                dstRow[0] = b; dstRow[1] = g; dstRow[2] = r;
                // Classify from the ungamma'd entry; r/g/b stay the on-screen colour
                // so the emitted pixel still matches the surface underneath.
                const uchar kidx = snapOrg ? snapOrg[(size_t)y * kKeyBandW + x] : idx;
                const uchar* k = keyBytes + (size_t)kidx * sizeof(PALETTEENTRY);
                const uchar kr = k[0], kg = k[1], kb = k[2];
                if (twoPass) {
                    uchar cls = contentMask[(size_t)y * rw + x];
                    uchar a = 0;
                    if (cls == 1) {
                        a = 255;
                    }
                    else if (cls == 2) {
                        // candidate: keep only near definite content
                        for (int dy = -reach; dy <= reach && !a; ++dy) {
                            int ny = y + dy;
                            if (ny < 0 || ny >= rh) continue;
                            const uchar* mrow = contentMask + (size_t)ny * rw;
                            for (int dx = -reach; dx <= reach; ++dx) {
                                int nx = x + dx;
                                if (nx < 0 || nx >= rw) continue;
                                if (mrow[nx] == 1) { a = 255; break; }
                            }
                        }
                    }
                    dstRow[3] = a;
                }
                else if (mode == 1) {
                    // Text key, keyed off the UNGAMMA'D entry so the thresholds mean the
                    // same thing at every gamma setting:
                    //  - keep BRIGHT pixels (white glyph): luma >= lumaThreshold
                    //  - keep NEAR-BLACK NEUTRAL pixels (black outline)
                    //
                    // (th4) NOTE this single-pass fallback keeps the ORIGINAL narrow rule
                    // and is NOT widened like the two-pass path above. Without pass 2 there
                    // is no "near a definite pixel" test, so admitting the whole grey ramp
                    // here would keep neutral background outright. It is unreachable for
                    // mode 1 in practice -- the text strip is 80x24 = 1920 bytes against a
                    // 8192-byte contentMask -- so it stays as the conservative path.
                    int luma = (kr * 30 + kg * 59 + kb * 11) / 100;
                    int mx = kr > kg ? (kr > kb ? kr : kb) : (kg > kb ? kg : kb);
                    int mn = kr < kg ? (kr < kb ? kr : kb) : (kg < kb ? kg : kb);
                    bool bright = luma >= lumaThreshold;
                    bool neutralBlack = (mx <= kTextDarkLuma) && (mx - mn <= kTextNeutral);
                    dstRow[3] = (bright || neutralBlack) ? 255 : 0;
                }
                else if (mode == 2) {
                    // Head key: drop the old globe FILL (a saturated red on life /
                    // saturated blue on mana), keep everything else (the head art,
                    // which is desaturated bone/grey). Saturation test is gamma-
                    // robust. mx-mn large AND a dominant channel => orb fill => drop.
                    int mx = kr > kg ? (kr > kb ? kr : kb) : (kg > kb ? kg : kb);
                    int mn = kr < kg ? (kr < kb ? kr : kb) : (kg < kb ? kg : kb);
                    int sat = mx - mn;
                    // dominant-red (life globe) or dominant-blue (mana globe)
                    bool orbFill = (sat >= kHeadSat) &&
                        ((kr == mx && kr - kg >= kHeadDom && kr - kb >= kHeadDom) ||
                            (kb == mx && kb - kg >= kHeadDom && kb - kr >= kHeadDom));
                    dstRow[3] = orbFill ? 0 : 255;
                }
                else {
                    dstRow[3] = 255; // opaque
                }
                dstRow += 4;
            }
        }
        P.tex->UnlockRect(0);
        // remember what this texture was built from
        P.valid = cacheable;
        if (cacheable) {
            P.rx = rx; P.ry = ry; P.rw = rw; P.rh = rh; P.mode = mode; P.luma = lumaThreshold;
            P.palHash = gPalHash; P.hadBand = (snapOrg != nullptr);
            for (int y = 0; y < rh; ++y) {
                memcpy(P.src + (size_t)y * rw, WorkingSurface + (size_t)WorkingWidth * (ry + y + TB) + (rx + LB), rw);
                if (snapOrg) memcpy(P.band + (size_t)y * rw, snapOrg + (size_t)y * kKeyBandW, rw);
            }
        }
        } // !same

        const float left = (float)rx - 0.5f;
        const float top = (float)ry - 0.5f;
        const float right = left + rw;
        const float bottom = top + rh;
        const float u1 = (float)rw / (float)P.texW;
        const float v1 = (float)rh / (float)P.texH;
        OrbVertex v[4] = {
            { left,  top,    0.f, 1.f, 0.f, 0.f },
            { right, top,    0.f, 1.f, u1,  0.f },
            { left,  bottom, 0.f, 1.f, 0.f, v1 },
            { right, bottom, 0.f, 1.f, u1,  v1 },
        };
        dev->SetTexture(0, P.tex);
        dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(OrbVertex));
    }

    // Redraw the heads and the number text on top of the orbs.
    void RedrawHeadsAndText(IDirect3DDevice9* dev)
    {
        const int ox = (ScreenWidth - 640) / 2;
        const int oy = (ScreenHeight - 480);
        const int lifeX = kLifeGuiX + ox;
        const int manaX = kManaGuiX + ox;
        const int orbY = kOrbGuiY + oy;

        if (kRedrawHeads) {
            // mode 2 = head key: keep static panel art (head), drop globe-fill pixels.
            RedrawRectOverOrbs(dev, 0, lifeX + kHeadOffX, orbY + kHeadOffY, kHeadW, kHeadH, 2);
            RedrawRectOverOrbs(dev, 1, manaX + kHeadOffX, orbY + kHeadOffY, kHeadW, kHeadH, 2);
        }
        if (kRedrawText) {
            const int ty = ScreenHeight - kTextBaseY - kTextTopOff;
            const int lifeCx = ScreenWidth / 2 - kTextCenterDX;
            const int manaCx = ScreenWidth / 2 + kTextCenterDX;
            // mode 1 = brightness key: only the bright digits show, orb shows through.
            RedrawRectOverOrbs(dev, 2, lifeCx - kTextHalfW, ty, kTextHalfW * 2, kTextH, 1, kTextLuma);
            RedrawRectOverOrbs(dev, 3, manaCx - kTextHalfW, ty, kTextHalfW * 2, kTextH, 1, kTextLuma);
        }
        // (th4) CONSUME-ONCE, after BOTH hue-keyed lifts. gKeyBandValid is otherwise
        // only cleared inside the capture, and the capture only runs from
        // DrawRedScreen -- so on the frame Seeing ENDS no capture happens and a stale
        // band would stay "valid" forever, keying every later frame off indices that
        // no longer match the surface. Clearing here means a band is used for exactly
        // the frame it was captured on.
        gKeyBandValid = false;

        // (th4) LIFT THE ITEM TOOLTIP BACK ON TOP.
        //
        // The tooltip is drawn into the 8-bit frame, and we composite over that frame,
        // so a tooltip overlapping a globe loses a band of text to the orb. It was not
        // on the lift list with the heads, numbers and cursor. Tooltip_Draw* publishes
        // each box rect; here we re-blit only the part that actually intersects an orb.
        //
        // mode 0 (fully opaque) is right: the tooltip has its own blended background,
        // and WorkingSurface already holds the exact pixels the software path produced,
        // so re-blitting restores what would have been visible had the orb not covered
        // it. Keying it would punch holes in the box.
        for (int i = 0; i < TooltipLiftCount; ++i) {
            const int tx = TooltipLiftRect[i][0], tw = TooltipLiftRect[i][2];
            const int tyy = TooltipLiftRect[i][1], th = TooltipLiftRect[i][3];
            const int orbX[2] = { lifeX, manaX };
            for (int o = 0; o < 2; ++o) {
                // intersect tooltip rect with this orb's 88x88 square
                const int ix = tx > orbX[o] ? tx : orbX[o];
                const int iy = tyy > orbY ? tyy : orbY;
                const int ix2 = (tx + tw < orbX[o] + ANIMORBS_SIZE) ? (tx + tw) : (orbX[o] + ANIMORBS_SIZE);
                const int iy2 = (tyy + th < orbY + ANIMORBS_SIZE) ? (tyy + th) : (orbY + ANIMORBS_SIZE);
                if (ix2 > ix && iy2 > iy)
                    RedrawRectOverOrbs(dev, 4 + o, ix, iy, ix2 - ix, iy2 - iy, 0);
            }
        }
    }

} // namespace

// ===========================================================================
// Public API
// ===========================================================================

bool AnimOrbs_Init()
{
    if (gInitDone) return true;
    // (th4) The life globe is red in every panel variant we draw over, so the life
    // set is fixed. The mana globe is blue or black depending on the panel art, so
    // its set follows WantedManaFolder(). Only ONE mana set is resident at a time:
    // each set is 1500 * 88*88*4 = ~45 MB, so keeping both would cost ~45 MB for
    // art that is never drawn.
    gLifeFolder = WantedLifeFolder();
    bool a = LoadOrbSet(gLife, gLifeFolder);
    gManaFolder = WantedManaFolder();
    bool b = LoadOrbSet(gMana, gManaFolder);
    gInitDone = true;
    return a || b;
}

// (th4) Swap the mana art if the panel colour changed since it was loaded (the
// menu can flip "Mana: blue" / "Mana: dark" without a restart). Frees the old set
// and its device texture first so the ~45 MB is not held twice.
static void EnsureManaVariant()
{
    const char* want = WantedManaFolder();
    if (!gManaFolder || strcmp(gManaFolder, want) == 0) { gManaFolder = want; return; }
    ReleaseOrbTexture(gMana);
    if (gMana.frames) { free(gMana.frames); gMana.frames = nullptr; }
    gMana.loadedCount = 0;
    gManaFolder = want;
    LoadOrbSet(gMana, gManaFolder);
}

// (th4) Same for the life art: Ice Age is a per-character game changer, so the blue
// life set is needed only once an Ice Age hero is in the game (AnimOrbs_Init runs at
// startup, before any hero is loaded, and picks the red set). One life set resident.
static void EnsureLifeVariant()
{
    const char* want = WantedLifeFolder();
    if (!gLifeFolder || strcmp(gLifeFolder, want) == 0) { gLifeFolder = want; return; }
    ReleaseOrbTexture(gLife);
    if (gLife.frames) { free(gLife.frames); gLife.frames = nullptr; }
    gLife.loadedCount = 0;
    gLifeFolder = want;
    LoadOrbSet(gLife, gLifeFolder);
}

void AnimOrbs_Shutdown()
{
    AnimOrbs_OnDeviceLost();	// (th4) textures, patch cache and the state block
    if (gLife.frames) { free(gLife.frames); gLife.frames = nullptr; }
    if (gMana.frames) { free(gMana.frames); gMana.frames = nullptr; }
    gLife.loadedCount = gMana.loadedCount = 0;
    gManaFolder = nullptr;
    gLifeFolder = nullptr;
    gInitDone = false;
}

void AnimOrbs_OnDeviceCreated(IDirect3DDevice9* dev)
{
    if (!dev) return;
    CreateOrbTexture(dev, gLife);
    CreateOrbTexture(dev, gMana);
}

void AnimOrbs_OnDeviceLost()
{
    ReleaseOrbTexture(gLife);
    ReleaseOrbTexture(gMana);
    if (gCursorTex) { gCursorTex->Release(); gCursorTex = nullptr; gCursorTexW = gCursorTexH = 0; }
    for (auto& P : gPatch) { if (P.tex) { P.tex->Release(); P.tex = nullptr; } P.texW = P.texH = 0; P.valid = false; }
    for (int i = 0; i < 2; ++i) { if (gStateBlock[i]) { gStateBlock[i]->Release(); gStateBlock[i] = nullptr; } gStateBlockDev[i] = nullptr; }
}

// (th4) ONE frame index for the whole frame, and it stops while the game is paused.
//
// Two things were wrong with computing it at each use site:
//  - the three sites (ClearOverlayWells, ClearTintWells, DrawD3D) each called
//    GetTickCountTh() separately, so a tick landing between them showed different orb
//    frames in the same presented image;
//  - GetTickCountTh() keeps running while GamePaused, so the orbs kept animating on a
//    frozen game.
//
// So accumulate animation time only while unpaused, and hand every caller the same
// value. GamePaused == 2 is the settled paused state (1 is the frame it is entered on,
// see Main.cpp), and both count as paused here.
static int AnimOrbs_Frame()
{
    static long long lastMs   = 0;   // clock reading at the previous call
    static long long animMs   = 0;   // time that has actually animated
    static long long cachedAt = -1;  // animMs is already advanced for this clock reading
    const long long ms = GetTickCountTh();
    if (ms != cachedAt) {
        long long delta = ms - lastMs;
        if (delta < 0 || delta > 1000) delta = 0;   // first call, or a long stall
        if (!GamePaused) animMs += delta;           // frozen while paused
        lastMs   = ms;
        cachedAt = ms;
    }
    const int msPer = 1000 / (gFps > 0 ? gFps : 1);
    return msPer > 0 ? (int)((animMs / msPer) % ANIMORBS_FRAME_COUNT) : 0;
}

void AnimOrbs_SetFps(int fps)
{
    if (fps < 1)    fps = 1;
    if (fps > 200)  fps = 200;
    gFps = fps;
}

int AnimOrbs_GetFps() { return gFps; }

// True when the animated orbs are active and will draw over the globes. The
// engine uses this to skip drawing its own (duplicate) globe fill, avoiding a
// double fill level. Mirrors the gates in AnimOrbs_DrawD3D.
bool AnimOrbs_Active()
{
    // (th4) Outside Classic, UserPanelB is NOT a gate -- it selects the mana COLOUR
    // (menu "Mana: blue" / "Mana: dark"), and both colours now have an animated set:
    // Mana_orb1 for blue, Mana_orb2_black for dark. Gating on it used to mean the
    // dark-mana panel, which is the DEFAULT (see the art choice in Panel.cpp), never
    // got animated globes at all.
    // CLASSIC IS DELIBERATELY LEFT AS IT WAS: it has its own panel art ("panel8") and
    // its own look, so it keeps the original UserPanelB gate and is unaffected by the
    // change above.
    // CLASSIC NEVER USES THE ANIMATED ORBS, whatever "panel" is set to in config.ini.
    // Classic mode is meant to look like vanilla Diablo -- it has its own panel art
    // ("panel8") and its own globes, and an RGBA overlay would break that on purpose.
    if (GameMode == GM_CLASSIC) return false;
    if (!gbRunGame || !IsInterface) return false;
    if (LoadingScreenCEL) return false;
    if (!gLife.frames && !gMana.frames) return false;
    return true;
}

// (th4) 2026-09-04 (Rev 2.36b): clear the alpha overlay under the animated orb
// glass, per pixel, before the composite. The glass is semi-transparent, so the
// composite behind it must be the clean playfield — otherwise an occluded
// sprite shows through the glass. Uses the CURRENT frame's alpha (same clock
// formula as AnimOrbs_DrawD3D) and clears only pixels whose glass alpha is at
// least kClearGlassAlpha — near-transparent pixels keep their translucency
// (the orb is effectively not there for them). The classic 8-bit glass top is
// cleared per pixel by PutWithAlpha (Panel.cpp) instead.
// (th4) Snapshot the ORIGINAL 8-bit indices of the whole orb band, before
// DrawRedScreen XLATs the surface through its red ramp. See the note beside
// gKeyBand. Only while the orbs are up.
// (th4) The band snapshot is only a valid key while the surface it was taken from is
// still the surface being drawn. DrawRedScreen is called from THREE places and the
// panel is drawn between two of them:
//   pause  (Menu_ShowPause -> DrawRedScreen)  Draw.cpp ~6751  -- BEFORE the panel
//   death  (IsPlayerDead   -> DrawRedScreen)  Draw.cpp ~6749  -- BEFORE the panel
//   Seeing (Terminator infravision)           Draw.cpp ~6772  -- AFTER the panel
// On the first two the panel repaints the whole orb band AFTER the capture, with
// fresh un-ramped pixels, so the snapshot describes a surface that no longer exists:
// keying against it classified garbage and the life/mana numbers and head art
// disappeared on pause. Anything that repaints the band therefore invalidates it.
void AnimOrbs_InvalidateKeyBand()
{
    gKeyBandValid = false;
}

void AnimOrbs_CaptureKeyBand()
{
    gKeyBandValid = false;
    if (!(kRedrawHeads || kRedrawText) || !AnimOrbs_Active()) return;
    if (!WorkingSurface) return;
    gKeyBandX = kLifeGuiX + (ScreenWidth - GUI_Width) / 2;
    gKeyBandY = kOrbGuiY  + (ScreenHeight - GUI_Height);
    for (int y = 0; y < kKeyBandH; y++) {
        const int sy = gKeyBandY + y;
        uchar* dst = gKeyBand + (size_t)y * kKeyBandW;
        if (sy < 0 || sy >= ScreenHeight) { memset(dst, 0, kKeyBandW); continue; }
        const uchar* src = WorkingSurface + WorkingWidth * (Screen_TopBorder + sy)
                         + Screen_LeftBorder + gKeyBandX;
        for (int x = 0; x < kKeyBandW; x++) {
            const int sx = gKeyBandX + x;
            dst[x] = (sx < 0 || sx >= ScreenWidth) ? 0 : src[x];
        }
    }
    gKeyBandValid = true;
}

void AnimOrbs_ClearOverlayWells()
{
    if (!(g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface)) return;
    if (!AnimOrbs_Active()) return;
    const int       frame = AnimOrbs_Frame();
    const int       ox = (ScreenWidth - GUI_Width) / 2;
    const int       y0 = kOrbGuiY + (ScreenHeight - GUI_Height);
    const int       wellsX[2] = { kLifeGuiX, kManaGuiX };
    OrbSet*         sets[2]  = { &gLife, &gMana };
    for (int w = 0; w < 2; w++) {
        OrbSet& set = *sets[w];
        if (!set.frames) continue;
        const uchar* fr = set.frames + (size_t)frame * kFrameBytes;
        for (int y = 0; y < ANIMORBS_SIZE; y++) {
            const int sy = y0 + y;
            if (sy < 0 || sy >= ScreenHeight) continue;
            const uchar* srcRow = fr + (size_t)y * ANIMORBS_SIZE * 4;
            uchar* arow = AlphaSurface + WorkingWidth * (Screen_TopBorder + sy) + Screen_LeftBorder + ox + wellsX[w];
            uint*  srow = SpriteLayerSurface + WorkingWidth * (Screen_TopBorder + sy) + Screen_LeftBorder + ox + wellsX[w];
            for (int x = 0; x < ANIMORBS_SIZE; x++) {
                if (srcRow[x * 4 + 3] >= kClearGlassAlpha) { arow[x] = 0; srow[x] = 0; }
            }
        }
    }
}

// (th4) Stage 3 (2026-09-05): clear the colored-light tint under the animated
// orb glass, per pixel — the Stage-3 analog of AnimOrbs_ClearOverlayWells.
// The orbs sit in the bottom panel band (bounding-box cleared) but their
// wells protrude above the band top, so the protruding glass pixels need the
// per-pixel clear; the same current-frame-alpha threshold keeps near-
// transparent pixels tinted. Idle-gated.
void AnimOrbs_ClearTintWells()
{
    if (!(g_ARGBLighting && g_LightTintActive && LightTintSurface)) return;
    if (!AnimOrbs_Active()) return;
    const int       frame = AnimOrbs_Frame();
    const int       ox = (ScreenWidth - GUI_Width) / 2;
    const int       y0 = kOrbGuiY + (ScreenHeight - GUI_Height);
    const int       wellsX[2] = { kLifeGuiX, kManaGuiX };
    OrbSet*         sets[2]  = { &gLife, &gMana };
    for (int w = 0; w < 2; w++) {
        OrbSet& set = *sets[w];
        if (!set.frames) continue;
        const uchar* fr = set.frames + (size_t)frame * kFrameBytes;
        for (int y = 0; y < ANIMORBS_SIZE; y++) {
            const int sy = y0 + y;
            if (sy < 0 || sy >= ScreenHeight) continue;
            const uchar* srcRow = fr + (size_t)y * ANIMORBS_SIZE * 4;
            uint* trow = LightTintSurface + WorkingWidth * (Screen_TopBorder + sy) + Screen_LeftBorder + ox + wellsX[w];
            for (int x = 0; x < ANIMORBS_SIZE; x++) {
                if (srcRow[x * 4 + 3] >= kClearGlassAlpha) trow[x] = TINT_WHITE;
            }
        }
    }
}

void AnimOrbs_DrawD3D(IDirect3DDevice9* dev, int targetW, int targetH)
{
    // Gates: feature flags, in actual gameplay, bottom panel visible.
    if (!dev) return;
    // (th4) see AnimOrbs_Active(): Classic keeps its vanilla globes unconditionally;
    // elsewhere UserPanelB selects the mana colour rather than gating.
    if (GameMode == GM_CLASSIC) return;
    if (!gbRunGame || !IsInterface) return;
    if (LoadingScreenCEL) return;   // skip while a loading screen is up
    if (!gLife.frames && !gMana.frames) return;

    // (th4) FOLLOW THE PALETTE FADE. Level transitions, the loading screen and the
    // ESC menu all fade the 8-bit frame through SetFadeLevel(), 32 steps of 8. These
    // orbs are true-colour quads outside the palette, so they used to stay at full
    // brightness while everything under them went to black, and pop in at full on
    // the way back. Whether anyone SAW that depended on how long the fade took:
    // SetFadeLevel waits a vblank per step only when windowed with vsync, so there
    // the fade is a visible half second and the orbs clearly lag it; full-screen or
    // vsync-off the 32 steps run in a few ms and the mismatch is invisible. Hence
    // "only on some players' machines". Modulate by the same level the palette gets.
    const int fadeLevel = PaletteFadeLevel;
    if (fadeLevel <= 0) return;    // frame is black; nothing to draw over it

    EnsureManaVariant();           // swap blue/dark mana art if the menu changed it
    EnsureLifeVariant();           // (th4) swap red / Ice Age blue life art with the hero
    AnimOrbs_OnDeviceCreated(dev); // lazily (re)create textures if needed

    // fps-independent frame index; shared, and frozen while paused.
    const int frame = AnimOrbs_Frame();

    UploadFrame(gLife, frame);     // no-op unless the animation frame changed
    UploadFrame(gMana, frame);

    // (th4) palette fingerprint for the patch cache (fades and the town night change it)
    {
        unsigned h = 2166136261u;
        const uchar* a = (const uchar*)ScreenPalette;
        const uchar* b = (const uchar*)BasePalette;
        for (int i = 0; i < 256 * (int)sizeof(PALETTEENTRY); ++i) { h = (h ^ a[i]) * 16777619u; h = (h ^ b[i]) * 16777619u; }
        gPalHash = h;
    }

    // Snapshot ALL device state. The game's quad relies on a palette pixel
    // shader, a palette texture in stage 1, and specific texture-stage ops;
    // capturing/restoring the full state block is the only safe way to draw
    // here without corrupting the rest of the frame. (th4) The block is created
    // once and re-captured each frame (see gStateBlock).
    IDirect3DStateBlock9* sb = nullptr;
    {
        // a state block belongs to ONE device; the optional second window has its own
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
    }

    // --- set up straight-alpha compositing, no shader ------------------------
    dev->SetVertexShader(nullptr);
    dev->SetPixelShader(nullptr);
    dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
    dev->SetTexture(1, nullptr); // clear the game's palette texture
    dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
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
    dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    dev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    {
        const DWORD lvl = (DWORD)(fadeLevel >= 256 ? 255 : fadeLevel);
        dev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, lvl, lvl, lvl));
    }

    const int lifeRatio = (int)Players[CurrentPlayerIndex].RatioLifeGlobe;
    const int manaRatio = (int)Players[CurrentPlayerIndex].RatioManaGlobe;
    // (th4) the life set follows the panel (WantedLifeFolder). If Health_orb_IA is missing
    // from the data (no frame loaded), the Ice Age globe keeps its static CEL as before.
    if (gLife.loadedCount > 0) DrawOrbQuad(dev, gLife.tex, kLifeGuiX, kOrbGuiY, lifeRatio);
    DrawOrbQuad(dev, gMana.tex, kManaGuiX, kOrbGuiY, manaRatio);

    // Heads + number text must sit on top of the orbs.
    RedrawHeadsAndText(dev);

    // Keep the mouse cursor visible on top of everything.
    RedrawCursorOverOrbs(dev);

    // --- restore everything --------------------------------------------------
    if (sb) { sb->Apply(); }	// (th4) kept for the next frame; released in AnimOrbs_OnDeviceLost
}