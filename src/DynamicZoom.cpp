#include "stdafx.h"
#include <emmintrin.h>
#include <math.h>

// =============================================================================
// (th4) DYNAMIC ZOOM -- the mouse wheel zooms the WORLD in and out, smoothly, centred on the hero.
// Config DynamicZoom (default off). Wheel up = in, wheel down = out. Cosmetic and local: nothing is
// sent over the network or saved; every game starts at no zoom (ClearPrevSessionData -> DynZoom_Reset).
//
// RANGE: from no zoom (1.0, the normal view) to ScreenHeight / DZ_MIN_VIEW_H -- the most zoomed-in view
// still shows 240 game px of height of the world (Mordor). The UI is never scaled, so the panels keep
// their 594-high layout whatever the zoom: only the world gets bigger. x4.5 at 1920x1080, x2.475 at 594.
// Each wheel notch zooms by about DZ_NOTCH (DynZoom_Steps picks the whole number of equal geometric steps
// closest to that for the range, so the feel is the same at every resolution: 7 notches at 1080p); the
// factor eases toward the target in LOG space, so a long flick zooms at an even pace, not fast-then-slow.
// Never in Classic (Mordor: Classic keeps the original look), never while the old Alt+Z 2x zoom
// (IsZoomDisable == 0 / config ForceZoom) is on -- that one is kept as it was -- and only in the
// textured-quad present (DrawMode 1, not DDraw): surface mode has no shaders to zoom with.
//
// HOW (world only, the UI is never scaled) -- three pieces:
//  1. CPU, this file: the world is drawn at 1x exactly as without zoom (DrawIsoLand, the CPU effects,
//     Perspective_WarpWorld) -- the DRAWN FRAME. Right after that, DynZoom_SnapshotWorld (Draw.cpp, next
//     to HeatHaze_SnapshotWorld) copies the world-only buffers: palette indices and LightSurface for the
//     whole screen (the UI mask needs them), AlphaSurface / SpriteLayerSurface / LightTintSurface for the
//     box the zoomed view can reach. Then the UI is drawn over the frame as usual.
//  2. GPU, dx_utility.cpp "DYNAMIC ZOOM": RenderQuadTexture / RenderQuadTextureUltra upload the SNAPSHOT
//     through the normal scene shaders into ZoomWorldTex (DynZoom_BeginSnapshotUpload swaps the surface
//     globals for the duration of that upload), the finished frame into ZoomFinalTex, and ZoomComposite.hlsl
//     writes the target: UI pixels from the finished frame, world pixels from ZoomWorldTex scaled about the
//     hero with sharp-bilinear filtering (crisp pixel art, no uneven pixels, no crawl).
//  3. The UI MASK (DynZoom_BuildUiMask, full game resolution): a pixel is UI where the finished frame
//     differs from the snapshot (index, or LightSurface -- every UI write sets L = 0), inside a panel
//     rectangle (HeatMask_AddUiRect), or a world pixel boxed in by UI on 3 sides (a UI pixel that happens to
//     equal the world under it) -- except inside a SEE-THROUGH BOX (DynZoom_NoteSeeThrough, below). DrawRedScreen (pause / death / Terminator vision) remaps the whole frame,
//     so it remaps the snapshot too (DynZoom_OnRedScreen) -- otherwise the whole screen would read as UI
//     and the view would jump back to 1x while paused.
// THE MAPPING, one formula everywhere: a point p of the drawn frame (game px) is shown at
//     c + z * (p - c)  =  p * z + shift,  shift = c * (1 - z)
// where z is the factor and c the hero's body on screen; the composite shows, at screen pixel x, the drawn
// pixel floor( c + (x + 0.5 - c) / z ) (sharp-bilinear: that is the texel that covers most of the pixel).
// TWO STATES, because the mouse and the post effects need the frame ON SCREEN while the next one is drawn:
//   s_Z / s_CX / s_CY + s_SnapValid  the frame being drawn (latched by DynZoom_SnapshotWorld); CPU text
//                                    drawn over the world this frame maps with DynZoom_WorldToScreen
//   s_Shown* + s_OnScreen            the frame last presented (DynZoom_EndFrame): the mouse
//                                    (DynZoom_ScreenToWorld) and the post effects (DynZoom_View)
// WHO MAPS WHAT:
//   - the mouse: DispatchCursorAboveIsoland (Main.cpp), DynZoom_ScreenToWorld before the Perspective inverse;
//   - text placed over the world after the snapshot: the Alt item labels (automap.cpp
//     HighlightItemsNameOnMap) and floating combat text (Player.cpp DrawCombatTextAbovePlayer) --
//     DynZoom_WorldToScreen. A NEW label anchored to a world position needs it. (The Reflect count is the
//     exception: while zoomed it is drawn INTO the world so it grows with its icon -- Player.cpp DrawReflectCount);
//   - the true-colour post effects (dx_utility.cpp, every pass of PostFxPresent): THE VIEW there (View_Begin,
//     View_Point, Atmos_Anchor) -- see its comment. Their maps built on the CPU in the drawn frame (heat /
//     water mask, wet map) are carried onto the screen by DynZoom_RemapHalf / DynZoom_RemapFull: world
//     pixels take the drawn pixel the composite shows there, UI pixels the UI code.
// PERFORMANCE (measured 2026-10-07 with a temporary timing log: the frame waits on the CPU, the GPU work of the zoom
// is ~0.2 ms): those maps are worked out only for the part of the drawn frame the view shows (DynZoom_ShownBox;
// the heat mask adds 3 texels so its closing stays exact); HeatHaze_SnapshotWorld reuses this file's world copy
// (DynZoom_FrameCopy -- Draw.cpp calls DynZoom_SnapshotWorld first) instead of copying the frame again; the UI
// mask is one streaming pass. Every one of these gives the same bytes as the plain version.
// KNOWN ISSUE (found 2026-10-07, not fixed -- Mordor runs FullscreenFX 0): the world copy goes through the SCENE
// shader, so FullscreenFX 1 / 2 / 3 (vignette, fog: dx.cpp SwitchVirtualShader -- they darken / desaturate by SCREEN
// position) are baked into it at 1x and then magnified with it: zoomed in, the world mostly loses them while the UI
// keeps them. Bloom (5) works on neighbouring pixels, so it simply scales with the world. A fix needs the world copy
// drawn without the screen-space term and that term applied to world pixels in ZoomComposite.hlsl at the screen
// position (the vignette also skips palette indices >= 128, so the composite would need the copy's indices too).
// Drawn INTO the world, so scaled with it: everything DrawIsoLand draws, overhead HP bars and Alt loot
// rings included, and every 8-bit effect drawn before the snapshot (embers, sparks, snow, fireflies...).
// The bars and rings growing with the zoom is KEPT ON PURPOSE for now (Mordor, 2026-10-07: he decides on
// player feedback) -- not a bug to fix unasked.
// KNOWN-GOOD CHECK: 1920x1080, DynamicZoom on, in a dungeon: wheel up 7 notches -> the world grows to
// x4.5 about the hero in smooth steps, panels / belt / cursor / tooltips keep their size; clicking a
// monster or an item still hits it; Alt labels sit over their items; open the inventory and pause (red
// screen) -- the zoom stays. In town at night: the mist / glow / rain stay on their spots of ground while
// the wheel turns, and never cover the panel.
// =============================================================================

extern int  g_CurPlayerSurfX, g_CurPlayerSurfY;	// Draw.cpp DrawPlayer: the hero's sprite centre / body, surface px
extern bool g_CurPlayerDrawn;

static const float DZ_NOTCH      = 1.25f;   // about how much one wheel notch zooms (DynZoom_Steps)
static const float DZ_MIN_VIEW_H = 240.0f;  // the most zoomed-in view still shows this many game px of height
static const float DZ_SPEED      = 12.0f;   // 1/s: how fast the factor eases toward the target (~0.25 s)

static int   s_Step = 0;                    // 0 .. DynZoom_Steps()
static float s_Target = 1.0f, s_Cur = 1.0f;
static DWORD s_LastMs = 0;

// the frame being drawn (latched by DynZoom_SnapshotWorld): factor and centre in game px
static float s_Z = 1.0f, s_CX = 0.0f, s_CY = 0.0f;
static bool  s_SnapValid = false;           // it has a world copy: the present will zoom it
static bool  s_Red = false;                 // DrawRedScreen remapped it (pause / death / Terminator vision)
// the frame last presented (DynZoom_EndFrame)
static float s_ShownZ = 1.0f, s_ShownCX = 0.0f, s_ShownCY = 0.0f;
static bool  s_OnScreen = false;

// world-only copies, same layout as the surfaces they copy (pitch WorkingWidth, borders included)
static uchar* s_SnapIdx = 0;
static uchar* s_SnapLight = 0;
static uchar* s_SnapAlpha = 0;
static uint*  s_SnapSprite = 0;
static uint*  s_SnapTint = 0;
static int    s_SnapWW = 0, s_SnapWH = 0;

// (th4) SEE-THROUGH BOXES: the town dialog (PlayerTrade.cpp DrawDialogBox) lets the world show through a
// CHECKERBOARD -- every other pixel of its inside set to black. Every world pixel there is boxed in by four
// black ones, so the hole fill of the UI mask took the whole box for UI, and the composite showed it from the
// finished frame: the world inside the dialog stayed at 1x while the world around it was zoomed (Mordor). Inside
// a box noted here the hole fill is skipped: the black dots are UI, the pixels between them world -- the zoomed
// world, seen through the same 50% dither as without zoom. Noted while the UI is drawn, forgotten at the next
// DynZoom_SnapshotWorld. A NEW dithered see-through box needs the same call next to its dither.
static int s_SeeRects[8][4];	// x0, y0, x1, y1 (exclusive), screen game px
static int s_SeeN = 0;
void DynZoom_NoteSeeThrough( int x0, int y0, int x1, int y1 )
{
	if( s_SeeN >= 8 ) return;
	if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0;
	if( x1 > ScreenWidth ) x1 = ScreenWidth; if( y1 > ScreenHeight ) y1 = ScreenHeight;
	if( x1 <= x0 || y1 <= y0 ) return;
	s_SeeRects[ s_SeeN ][ 0 ] = x0; s_SeeRects[ s_SeeN ][ 1 ] = y0; s_SeeRects[ s_SeeN ][ 2 ] = x1; s_SeeRects[ s_SeeN ][ 3 ] = y1;
	++s_SeeN;
}
// (th4) SEE-THROUGH BOXES of this frame, noted whether the zoom is on or not -- also for dx_utility.cpp
// SnowGround_Present: the snow must not light up the world seen through the box's dither
int DynZoom_SeeThroughBoxes( int* x0, int* y0, int* x1, int* y1 )
{
	if( !s_SeeN ) return 0;
	*x0 = s_SeeRects[ 0 ][ 0 ]; *y0 = s_SeeRects[ 0 ][ 1 ]; *x1 = s_SeeRects[ 0 ][ 2 ]; *y1 = s_SeeRects[ 0 ][ 3 ];
	for( int k = 1; k < s_SeeN; ++k ){	// their bounding box (there is one dialog at a time)
		*x0 = min( *x0, s_SeeRects[ k ][ 0 ] ); *y0 = min( *y0, s_SeeRects[ k ][ 1 ] );
		*x1 = max( *x1, s_SeeRects[ k ][ 2 ] ); *y1 = max( *y1, s_SeeRects[ k ][ 3 ] );
	}
	return s_SeeN;
}

// the UI mask of the frame (DynZoom_BuildUiMask; ScreenWidth x ScreenHeight, pitch ScreenWidth, 0 = world;
// s_UiRaw = before the hole fill), and for each screen column / row the drawn pixel the composite shows there
static uchar* s_UiMask = 0;
static uchar* s_UiRaw = 0;
static int    s_UiW = 0, s_UiH = 0;
static bool   s_UiValid = false;
static int*   s_MapX = 0;
static int*   s_MapY = 0;
static int    s_MapW = 0, s_MapH = 0;

float DynZoom_Max()
{
	const float m = ScreenHeight / DZ_MIN_VIEW_H;
	return m > 1.0f ? m : 1.0f;
}

// wheel notches from no zoom to the maximum: equal geometric steps, as close to DZ_NOTCH each as the range allows
static int DynZoom_Steps()
{
	const int n = (int)floorf( logf( DynZoom_Max() ) / logf( DZ_NOTCH ) + 0.5f );
	return n < 1 ? 1 : n;
}

bool DynZoom_Allowed()
{
	return DynamicZoom && gbRunGame && GameMode != GM_CLASSIC && IsZoomDisable && !DDraw && DrawMode == 1
	    && DynZoom_Max() > 1.001f;
}

bool DynZoom_Active()
{
	return DynZoom_Allowed() && s_Cur > 1.0005f;
}

void DynZoom_Reset()
{
	s_Step = 0; s_Target = s_Cur = 1.0f; s_Z = 1.0f;
	s_SnapValid = s_Red = s_OnScreen = s_UiValid = false;
}

// OnMouseWheel (thehell2.cpp) asks this once the open panels had their chance. true = the wheel was used.
bool DynZoom_OnWheel( int wheelDelta )
{
	if( !DynZoom_Allowed() || !wheelDelta ) return false;
	s_Step += wheelDelta > 0 ? 1 : -1;
	const int steps = DynZoom_Steps();
	if( s_Step < 0 ) s_Step = 0;
	if( s_Step > steps ) s_Step = steps;
	return true;
}

// Once per drawn frame, before the world (MainDrawFunc).
void DynZoom_Tick()
{
	const DWORD now = GetTickCount();
	float dt = s_LastMs ? ( now - s_LastMs ) / 1000.0f : 0.0f;
	s_LastMs = now;
	if( dt > 0.25f ) dt = 0.25f;
	if( !DynZoom_Allowed() ){ s_Step = 0; s_Target = s_Cur = 1.0f; return; }
	const int steps = DynZoom_Steps();	// recomputed: the resolution may have changed
	if( s_Step > steps ) s_Step = steps;
	s_Target = powf( DynZoom_Max(), (float)s_Step / steps );
	// ease in LOG space: every frame closes the same share of the remaining zoom RATIO
	const float lc = logf( s_Cur ), lt = logf( s_Target );
	const float ln = lc + ( lt - lc ) * ( 1.0f - expf( -DZ_SPEED * dt ) );
	s_Cur = fabsf( lt - ln ) < 0.001f ? s_Target : expf( ln );
}

static bool DynZoom_EnsureBuffers()
{
	const int ww = WorkingWidth, wh = WorkingHeight;
	const size_t n = (size_t)ww * wh;
	if( s_SnapWW != ww || s_SnapWH != wh ){
		free( s_SnapIdx ); free( s_SnapLight ); free( s_SnapAlpha ); free( s_SnapSprite ); free( s_SnapTint );
		s_SnapIdx = s_SnapLight = s_SnapAlpha = 0; s_SnapSprite = s_SnapTint = 0;
		s_SnapWW = ww; s_SnapWH = wh;
	}
	if( !s_SnapIdx ) s_SnapIdx = (uchar*)calloc( n, 1 );
	if( LightSurface && !s_SnapLight ) s_SnapLight = (uchar*)calloc( n, 1 );
	if( AlphaSurface && !s_SnapAlpha ) s_SnapAlpha = (uchar*)calloc( n, 1 );
	if( SpriteLayerSurface && !s_SnapSprite ) s_SnapSprite = (uint*)calloc( n, sizeof(uint) );
	if( LightTintSurface && !s_SnapTint ) s_SnapTint = (uint*)calloc( n, sizeof(uint) );
	if( s_MapW != ScreenWidth || s_MapH != ScreenHeight ){
		free( s_MapX ); free( s_MapY );
		s_MapX = (int*)malloc( ScreenWidth * sizeof(int) ); s_MapY = (int*)malloc( ScreenHeight * sizeof(int) );
		s_MapW = ( s_MapX && s_MapY ) ? ScreenWidth : 0; s_MapH = ( s_MapX && s_MapY ) ? ScreenHeight : 0;
	}
	return s_SnapIdx && ( !LightSurface || s_SnapLight ) && ( !AlphaSurface || s_SnapAlpha )
	    && ( !SpriteLayerSurface || s_SnapSprite ) && ( !LightTintSurface || s_SnapTint ) && s_MapW;
}

// Called right after HeatHaze_SnapshotWorld (Draw.cpp): the world is complete, no UI yet.
void DynZoom_SnapshotWorld()
{
	s_SeeN = 0;	// (th4) SEE-THROUGH BOXES: noted again by this frame's UI
	s_SnapValid = false; s_Red = false; s_UiValid = false;
	if( !DynZoom_Active() || !DynZoom_EnsureBuffers() ) return;
	// centre: the hero's body, where DrawPlayer drew him this frame (pre-warp surface px), then the warp
	float cx = ScreenWidth * 0.5f, cy = ScreenHeight * 0.5f;
	if( g_CurPlayerDrawn ){
		int x = g_CurPlayerSurfX - Screen_LeftBorder, y = g_CurPlayerSurfY - Screen_TopBorder;
		Perspective_WorldToScreen( x, y );
		cx = (float)x; cy = (float)y;
	}
	if( cx < 0 ) cx = 0; if( cx > ScreenWidth ) cx = (float)ScreenWidth;
	if( cy < 0 ) cy = 0; if( cy > ScreenHeight ) cy = (float)ScreenHeight;
	s_Z = s_Cur; s_CX = cx; s_CY = cy;
	// screen column / row -> the drawn pixel shown there (DynZoom_RemapFull)
	for( int x = 0; x < ScreenWidth; ++x ){
		const int q = (int)floorf( s_CX + ( x + 0.5f - s_CX ) / s_Z );
		s_MapX[x] = q < 0 ? 0 : ( q >= ScreenWidth ? ScreenWidth - 1 : q );
	}
	for( int y = 0; y < ScreenHeight; ++y ){
		const int q = (int)floorf( s_CY + ( y + 0.5f - s_CY ) / s_Z );
		s_MapY[y] = q < 0 ? 0 : ( q >= ScreenHeight ? ScreenHeight - 1 : q );
	}
	// the box of the 1x world the zoomed view can reach (+3 px for the filter taps and the rim shader)
	int x0 = (int)floorf( cx - cx / s_Z ) - 3, x1 = (int)ceilf( cx + ( ScreenWidth - cx ) / s_Z ) + 3;
	int y0 = (int)floorf( cy - cy / s_Z ) - 3, y1 = (int)ceilf( cy + ( ScreenHeight - cy ) / s_Z ) + 3;
	if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0;
	if( x1 > ScreenWidth ) x1 = ScreenWidth; if( y1 > ScreenHeight ) y1 = ScreenHeight;
	const size_t top = (size_t)WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	for( int y = 0; y < ScreenHeight; ++y ){
		const size_t o = top + (size_t)y * WorkingWidth;
		memcpy( s_SnapIdx + o, SoftSrcSurfacePtr + o, ScreenWidth );
		if( s_SnapLight ) memcpy( s_SnapLight + o, LightSurface + o, ScreenWidth );
		if( y < y0 || y >= y1 ) continue;
		if( s_SnapAlpha ) memcpy( s_SnapAlpha + o + x0, AlphaSurface + o + x0, x1 - x0 );
		if( s_SnapSprite ) memcpy( s_SnapSprite + o + x0, SpriteLayerSurface + o + x0, ( x1 - x0 ) * sizeof(uint) );
		if( s_SnapTint ) memcpy( s_SnapTint + o + x0, LightTintSurface + o + x0, ( x1 - x0 ) * sizeof(uint) );
	}
	s_SnapValid = true;
}

// DrawRedScreen (Panel.cpp) just remapped the whole frame through `xlat` (indices below minIndex kept):
// do the same to the world copy, so the world stays world and the zoomed world is red as well.
void DynZoom_OnRedScreen( const uchar* xlat, int minIndex )
{
	if( !s_SnapValid || !xlat ) return;
	s_Red = true;
	const size_t top = (size_t)WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	for( int y = 0; y < ScreenHeight; ++y ){
		uchar* p = s_SnapIdx + top + (size_t)y * WorkingWidth;
		for( int x = 0; x < ScreenWidth; ++x ) if( p[x] >= minIndex ) p[x] = xlat[ p[x] ];
	}
}

// ---- present side (dx_utility.cpp) ------------------------------------------------------------------
bool DynZoom_PresentReady() { return s_SnapValid && DynZoom_Active(); }
void DynZoom_Params( float& z, float& cx, float& cy ) { z = s_Z; cx = s_CX; cy = s_CY; }

// After the scene quad of the main device (RenderQuadTexture / RenderQuadTextureUltra): `shown` = the
// composite ran, so the screen now shows the zoomed frame.
void DynZoom_EndFrame( bool shown )
{
	s_SnapValid = false;
	s_OnScreen = shown;
	if( shown ){ s_ShownZ = s_Z; s_ShownCX = s_CX; s_ShownCY = s_CY; }
}

// Upload the SNAPSHOT through the normal upload code: the upload reads SoftSrcSurfacePtr / LightSurface /
// AlphaSurface / SpriteLayerSurface / LightTintSurface by offset from `src`, so those globals point at the
// copies (same layout) until DynZoom_EndSnapshotUpload. Returns `src` moved into the copy.
static uchar* sv_Soft = 0; static uchar* sv_Light = 0; static uchar* sv_Alpha = 0;
static uint* sv_Sprite = 0; static uint* sv_Tint = 0; static bool sv_On = false;
uchar* DynZoom_BeginSnapshotUpload( uchar* src )
{
	const ptrdiff_t off = src - SoftSrcSurfacePtr;
	sv_Soft = SoftSrcSurfacePtr; sv_Light = LightSurface; sv_Alpha = AlphaSurface; sv_Sprite = SpriteLayerSurface; sv_Tint = LightTintSurface;
	SoftSrcSurfacePtr = s_SnapIdx;
	if( LightSurface ) LightSurface = s_SnapLight;
	if( AlphaSurface ) AlphaSurface = s_SnapAlpha;
	if( SpriteLayerSurface ) SpriteLayerSurface = s_SnapSprite;
	if( LightTintSurface ) LightTintSurface = s_SnapTint;
	sv_On = true;
	return s_SnapIdx + off;
}
void DynZoom_EndSnapshotUpload()
{
	if( !sv_On ) return;
	SoftSrcSurfacePtr = sv_Soft; LightSurface = sv_Light; AlphaSurface = sv_Alpha; SpriteLayerSurface = sv_Sprite; LightTintSurface = sv_Tint;
	sv_On = false;
}

// The UI mask: 255 = UI (take the finished frame), 0 = world (take the zoomed world). Built here (the post
// effects remap their maps with it, DynZoom_Remap*), then copied to dst: ScreenWidth x ScreenHeight bytes
// with the given pitch. rects = HeatMask's panel rectangles (screen game px, x1/y1 exclusive).
// PERFORMANCE (it runs every zoomed frame): ONE pass, row by row. The raw mask (finished frame != copy, or a
// panel rect) is made one row ahead into a ring of three rows (s_UiRaw), and the hole fill of the row between
// goes to s_UiMask and to dst while those rows are still in the cache, with 3 rows of scratch instead of a
// whole frame. Same bytes as four separate passes, and measured the same ~0.6 ms a frame at 1080p (2026-10-07):
// the time is in reading the two frames and writing the texture, not in the pass structure.
void DynZoom_BuildUiMask( uchar* dst, int pitch, const int (*rects)[4], int rectN )
{
	const int W = ScreenWidth, H = ScreenHeight;
	if( s_UiW != W || s_UiH != H ){
		free( s_UiMask ); free( s_UiRaw );
		s_UiMask = (uchar*)malloc( (size_t)W * H ); s_UiRaw = (uchar*)malloc( (size_t)W * 3 );	// raw rows y-1, y, y+1
		s_UiW = s_UiMask && s_UiRaw ? W : 0; s_UiH = s_UiMask && s_UiRaw ? H : 0;
	}
	if( !s_UiW || W < 3 || H < 3 ){ for( int y = 0; y < H; ++y ) memset( dst + (size_t)y * pitch, 255, W ); return; }	// all UI = the plain frame
	int rc[32][4], rn = 0;	// the panel rects, clipped to the screen
	for( int r = 0; r < rectN && rn < 32; ++r ){
		int x0 = rects[r][0], y0 = rects[r][1], x1 = rects[r][2], y1 = rects[r][3];
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0;
		if( x1 > W ) x1 = W; if( y1 > H ) y1 = H;
		if( x1 > x0 && y1 > y0 ){ rc[rn][0] = x0; rc[rn][1] = y0; rc[rn][2] = x1; rc[rn][3] = y1; ++rn; }
	}
	const size_t top = (size_t)WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	const bool light = s_SnapLight && LightSurface;
	const __m128i ff = _mm_set1_epi8( (char)0xFF );
	auto rawRow = [&]( int y, uchar* d ){	// row y of the raw mask: 255 where the frame differs from the copy, or a panel covers it
		const size_t o = top + (size_t)y * WorkingWidth;
		const uchar* fin = SoftSrcSurfacePtr + o; const uchar* snap = s_SnapIdx + o;
		const uchar* finL = light ? LightSurface + o : 0; const uchar* snapL = light ? s_SnapLight + o : 0;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ){
			__m128i same = _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( fin + x ) ), _mm_loadu_si128( (const __m128i*)( snap + x ) ) );
			if( light ) same = _mm_and_si128( same, _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( finL + x ) ), _mm_loadu_si128( (const __m128i*)( snapL + x ) ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), _mm_xor_si128( same, ff ) );
		}
		for( ; x < W; ++x ) d[x] = ( fin[x] != snap[x] || ( light && finL[x] != snapL[x] ) ) ? 255 : 0;
		for( int r = 0; r < rn; ++r ) if( y >= rc[r][1] && y < rc[r][3] ) memset( d + rc[r][0], 255, rc[r][2] - rc[r][0] );
	};
	// a world pixel with UI on 3 of its 4 sides is a UI pixel that equals the world under it (a glyph hole):
	// UI too (254). Judged on the RAW mask, so a filled pixel never fills its neighbours. 16 pixels at a time
	// (SSE2); the scalar loop finishes each row and is the reference -- the two give the same bytes.
	const __m128i z = _mm_setzero_si128(), one = _mm_set1_epi8( 1 ), two = _mm_set1_epi8( 2 ), hole = _mm_set1_epi8( (char)254 );
	uchar* m = s_UiMask;
	uchar* r0 = s_UiRaw; uchar* r1 = r0 + W; uchar* r2 = r1 + W;	// raw rows y-1, y, y+1
	rawRow( 0, r0 ); rawRow( 1, r1 );
	memcpy( m, r0, W ); memcpy( dst, r0, W );	// the edge rows and columns are never filled
	for( int y = 1; y < H - 1; ++y ){
		rawRow( y + 1, r2 );
		const uchar* s = r1; const uchar* up = r0; const uchar* dn = r2;
		uchar* d = m + (size_t)y * W;
		d[0] = s[0]; d[W - 1] = s[W - 1];
		int x = 1;
		for( ; x + 16 <= W - 1; x += 16 ){
			const __m128i c = _mm_loadu_si128( (const __m128i*)( s + x ) );
			__m128i n = _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( s + x - 1 ) ), z ), one );	// 1 per UI neighbour
			n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( s + x + 1 ) ), z ), one ) );
			n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( up + x ) ), z ), one ) );
			n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( dn + x ) ), z ), one ) );
			const __m128i fill = _mm_and_si128( _mm_cmpeq_epi8( c, z ), _mm_cmpgt_epi8( n, two ) );
			_mm_storeu_si128( (__m128i*)( d + x ), _mm_or_si128( c, _mm_and_si128( fill, hole ) ) );
		}
		for( ; x < W - 1; ++x ){
			const int n = ( s[x - 1] != 0 ) + ( s[x + 1] != 0 ) + ( up[x] != 0 ) + ( dn[x] != 0 );
			d[x] = ( !s[x] && n >= 3 ) ? 254 : s[x];
		}
		for( int k = 0; k < s_SeeN; ++k ){	// (th4) SEE-THROUGH BOXES (above): no hole fill there, the raw row as it is
			const int* sr = s_SeeRects[ k ];
			if( y >= sr[1] && y < sr[3] ) memcpy( d + sr[0], s + sr[0], sr[2] - sr[0] );
		}
		memcpy( dst + (size_t)y * pitch, d, W );
		uchar* t = r0; r0 = r1; r1 = r2; r2 = t;
	}
	memcpy( m + (size_t)( H - 1 ) * W, r1, W ); memcpy( dst + (size_t)( H - 1 ) * pitch, r1, W );	// the last row, raw
	s_UiValid = true;
}

// screen game px -> the drawn frame (the mouse; Main.cpp DispatchCursorAboveIsoland, before Perspective).
// Uses the frame ON SCREEN -- the one the player is pointing at.
void DynZoom_ScreenToWorld( int& x, int& y )
{
	if( !s_OnScreen ) return;
	x = (int)floorf( s_ShownCX + ( x + 0.5f - s_ShownCX ) / s_ShownZ );
	y = (int)floorf( s_ShownCY + ( y + 0.5f - s_ShownCY ) / s_ShownZ );
}

// the drawn frame -> screen game px, for text anchored to the world and drawn over it THIS frame (after the
// snapshot): the Alt item labels, floating combat text. A no-op when this frame is not zoomed.
void DynZoom_WorldToScreen( int& x, int& y )
{
	if( !s_SnapValid ) return;
	x = (int)floorf( s_CX + ( x - s_CX ) * s_Z + 0.5f );
	y = (int)floorf( s_CY + ( y - s_CY ) * s_Z + 0.5f );
}

// ---- the frame on screen, for the post effects (dx_utility.cpp THE VIEW) ------------------------------
bool DynZoom_OnScreen() { return s_OnScreen; }

// on screen = drawn * z + (sx, sy); z = 1, shift 0 when the frame on screen is not zoomed
void DynZoom_View( float& z, float& sx, float& sy )
{
	if( !s_OnScreen ){ z = 1.0f; sx = sy = 0.0f; return; }
	z = s_ShownZ; sx = s_ShownCX * ( 1.0f - s_ShownZ ); sy = s_ShownCY * ( 1.0f - s_ShownZ );
}

// DrawRedScreen ran on the frame on screen: the post effects treat it all as UI, as they do without the zoom
// (there the finished frame differs from their world snapshot everywhere)
bool DynZoom_RedScreen() { return s_OnScreen && s_Red; }

// the world copy of the frame on screen (palette indices, pitch WorkingWidth, at the screen's top-left), 0 if none
const uchar* DynZoom_WorldRows()
{
	if( !s_OnScreen || !s_SnapIdx ) return 0;
	return s_SnapIdx + (size_t)WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
}

// the part of the drawn frame the frame on screen shows: pixels [x0, x1) x [y0, y1) -- everything a remap of
// it reads, so a map only needs working out there (HeatHaze_BuildMask, Wet_BuildMask)
bool DynZoom_ShownBox( int& x0, int& y0, int& x1, int& y1 )
{
	if( !s_OnScreen || s_MapW != ScreenWidth || s_MapH != ScreenHeight ) return false;
	x0 = s_MapX[0]; x1 = s_MapX[ ScreenWidth - 1 ] + 1;
	y0 = s_MapY[0]; y1 = s_MapY[ ScreenHeight - 1 ] + 1;
	return true;
}

// this frame's world copy (palette indices, pitch WorkingWidth, at the screen's top-left), 0 = none taken:
// HeatHaze_SnapshotWorld (called right after DynZoom_SnapshotWorld) uses it instead of copying the same
// pixels a second time. Valid until the next DynZoom_SnapshotWorld.
const uchar* DynZoom_FrameCopy()
{
	return s_SnapValid ? s_SnapIdx + (size_t)WorkingWidth * Screen_TopBorder + Screen_LeftBorder : 0;
}

// 16 bytes: uiValue where the UI byte is not 0, else v
static inline __m128i DZ_SelUi( __m128i ui, __m128i v, __m128i uiv )
{
	const __m128i isUi = _mm_xor_si128( _mm_cmpeq_epi8( ui, _mm_setzero_si128() ), _mm_set1_epi8( (char)0xFF ) );
	return _mm_or_si128( _mm_and_si128( isUi, uiv ), _mm_andnot_si128( isUi, v ) );
}

// A full-resolution map built in the DRAWN frame (one byte per game px, pitch srcPitch) -> the screen:
// UI pixels get uiValue, world pixels the value of the drawn pixel the composite shows there.
// dst = ScreenWidth x ScreenHeight with pitch dstPitch, must not overlap src. false = no UI mask this frame.
// PERFORMANCE: a drawn row is shown on about z screen rows in a row, so it is fetched (scattered reads) once
// into `row`; each screen row is then a 16-at-a-time select against the UI mask. Same bytes as per pixel.
bool DynZoom_RemapFull( const uchar* src, int srcPitch, uchar* dst, int dstPitch, uchar uiValue )
{
	if( !s_OnScreen || !s_UiValid || s_UiW != ScreenWidth || s_UiH != ScreenHeight || s_MapW != ScreenWidth || s_MapH != ScreenHeight ) return false;
	const int W = ScreenWidth;
	static uchar* row = 0; static int rowN = 0;
	if( rowN < W ){ free( row ); row = (uchar*)malloc( W ); rowN = row ? W : 0; }
	if( !row ) return false;
	const __m128i uiv = _mm_set1_epi8( (char)uiValue );
	int fetched = -1;
	for( int y = 0; y < ScreenHeight; ++y ){
		if( s_MapY[y] != fetched ){
			fetched = s_MapY[y];
			const uchar* s = src + (size_t)fetched * srcPitch;
			for( int x = 0; x < W; ++x ) row[x] = s[ s_MapX[x] ];
		}
		const uchar* ui = s_UiMask + (size_t)y * W;
		uchar* d = dst + (size_t)y * dstPitch;
		int x = 0;
		for( ; x + 16 <= W; x += 16 )
			_mm_storeu_si128( (__m128i*)( d + x ), DZ_SelUi( _mm_loadu_si128( (const __m128i*)( ui + x ) ), _mm_loadu_si128( (const __m128i*)( row + x ) ), uiv ) );
		for( ; x < W; ++x ) d[x] = ui[x] ? uiValue : row[x];
	}
	return true;
}

// The same for a HALF-resolution map (w2 x h2 texels of 2x2 game px, pitch w2 -- HeatHaze_BuildMask's):
// a texel is UI if any of its four screen pixels is (that mask's own rule), else it takes the drawn
// texel under its centre. Same row fetch and 16-at-a-time select as above.
bool DynZoom_RemapHalf( const uchar* src, int w2, int h2, uchar* dst, uchar uiValue )
{
	if( !s_OnScreen || !s_UiValid || s_UiW != ScreenWidth || s_UiH != ScreenHeight || 2 * w2 > ScreenWidth || 2 * h2 > ScreenHeight ) return false;
	static int* mx = 0; static int* my = 0; static uchar* row = 0; static int mxN = 0, myN = 0;
	if( mxN < w2 ){ free( mx ); free( row ); mx = (int*)malloc( w2 * sizeof(int) ); row = (uchar*)malloc( w2 ); mxN = mx && row ? w2 : 0; }
	if( myN < h2 ){ free( my ); my = (int*)malloc( h2 * sizeof(int) ); myN = my ? h2 : 0; }
	if( !mxN || !myN ) return false;
	for( int x2 = 0; x2 < w2; ++x2 ){
		const int q = (int)floorf( ( s_ShownCX + ( 2 * x2 + 1 - s_ShownCX ) / s_ShownZ ) * 0.5f );
		mx[x2] = q < 0 ? 0 : ( q >= w2 ? w2 - 1 : q );
	}
	for( int y2 = 0; y2 < h2; ++y2 ){
		const int q = (int)floorf( ( s_ShownCY + ( 2 * y2 + 1 - s_ShownCY ) / s_ShownZ ) * 0.5f );
		my[y2] = q < 0 ? 0 : ( q >= h2 ? h2 - 1 : q );
	}
	const __m128i uiv = _mm_set1_epi8( (char)uiValue ), ff16 = _mm_set1_epi16( 0x00FF );
	int fetched = -1;
	for( int y2 = 0; y2 < h2; ++y2 ){
		if( my[y2] != fetched ){
			fetched = my[y2];
			const uchar* s = src + (size_t)fetched * w2;
			for( int x2 = 0; x2 < w2; ++x2 ) row[x2] = s[ mx[x2] ];
		}
		const uchar* u0 = s_UiMask + (size_t)( 2 * y2 ) * ScreenWidth; const uchar* u1 = u0 + ScreenWidth;
		uchar* d = dst + (size_t)y2 * w2;
		int x2 = 0;
		for( ; x2 + 16 <= w2; x2 += 16 ){
			// UI where any of the texel's 4 pixels is: OR the two rows, then the two bytes of each 16-bit lane
			// (dx_utility.cpp HM_PairOr does the same)
			__m128i lo = _mm_or_si128( _mm_loadu_si128( (const __m128i*)( u0 + 2 * x2 ) ), _mm_loadu_si128( (const __m128i*)( u1 + 2 * x2 ) ) );
			__m128i hi = _mm_or_si128( _mm_loadu_si128( (const __m128i*)( u0 + 2 * x2 + 16 ) ), _mm_loadu_si128( (const __m128i*)( u1 + 2 * x2 + 16 ) ) );
			lo = _mm_and_si128( _mm_or_si128( lo, _mm_srli_epi16( lo, 8 ) ), ff16 );
			hi = _mm_and_si128( _mm_or_si128( hi, _mm_srli_epi16( hi, 8 ) ), ff16 );
			_mm_storeu_si128( (__m128i*)( d + x2 ), DZ_SelUi( _mm_packus_epi16( lo, hi ), _mm_loadu_si128( (const __m128i*)( row + x2 ) ), uiv ) );
		}
		for( ; x2 < w2; ++x2 ){
			const int x = 2 * x2;
			d[x2] = ( u0[x] | u0[x + 1] | u1[x] | u1[x + 1] ) ? uiValue : row[x2];
		}
	}
	return true;
}
