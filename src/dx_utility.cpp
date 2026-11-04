#include "stdafx.h"
#ifndef OLD_PCX_SCREENSHOT
#include "ScreenShotMaker.h"
#endif
#include "AnimOrbs.h"
#include "MenuPortrait.h"
#include "ItemOverlay.h"
#include <intrin.h> // (th4) diagnostics: _ReturnAddress() for the SwitchFullscreen call-site log

LPDIRECT3DDEVICE9 D3dDevice = 0; // Our rendering device
// (th4) 2026-09-10 reentrancy guard (MP "New Game" d3d9 crash fix): true while
// D3dReset()/SecondReset() is inside IDirect3DDevice9::Reset(); every D3D touch
// point must become a no-op in that window (see ConvertPaletteSurfaceToTrueColor).
bool g_D3dInReset = false;
LPDIRECT3DDEVICE9 SecDevice = 0; // Second win rendering device
LPDIRECT3DSURFACE9 BackBuffer = 0; // d3d hardware backbuffer, for copy render surface
LPDIRECT3DSURFACE9 SecBuffer = 0; // d3d hardware backbuffer, for second win
LPDIRECT3DSURFACE9 D3dSurface = 0; // rendering surface, like frontbuffer ScreenSurface32
LPDIRECT3DSURFACE9 SecD3dSurface = 0; // rendering surface, for second win
LPDIRECT3DSURFACE9 VideoSurface = 0; // stretching video surface
LPDIRECT3DSURFACE9 SecVideoSurface = 0; // stretching video surface for second win

// (th4) SHARP SCALING (config "SharpScaling", default off; only acts when "filter" is on).
// The finished frame (ScreenWidth x ScreenHeight in VideoSurface / the palette path's
// surface) reaches the window through StretchRect with FilteringMode. At a non-integer
// ratio -- e.g. 1056x594 on a 1920x1080 window (x1.82) -- plain bilinear smears every
// pixel edge. Sharp bilinear does it in two steps with the same hardware StretchRect:
//   1. POINT-scale the frame by a whole factor k = ceil(ratio) into an intermediate
//      render target (every game pixel becomes a crisp k x k block);
//   2. LINEAR-scale that to the window size, which now only blends the thin seams
//      where the fractional remainder falls.
// No shader involved, so it serves all three present paths (palette quad, Ultra quad,
// surface mode) -- every former StretchRect(..., FilteringMode) call below goes through
// SharpStretch. Falls back to the plain StretchRect when off, when not enlarging, or if
// the intermediate target cannot be made.
// The intermediate target is per device (slot 0 = D3dDevice, 1 = SecDevice) and must be
// released with the other default-pool surfaces on a device reset: dx.cpp calls
// SharpScale_Release() next to every VideoSurface release.
static LPDIRECT3DSURFACE9 SharpSurf[2] = { 0, 0 };
static int SharpW[2] = { 0, 0 }, SharpH[2] = { 0, 0 };
static D3DFORMAT SharpFmt[2] = { D3DFMT_UNKNOWN, D3DFMT_UNKNOWN };

static void Fsr_Release( int slot );	// FSR 1.0 resources, defined below SharpStretch
static void PostFx_Release( int slot );	// post effects, defined below FsrPresent
static void Antialias_Release( int slot );	// (th4) Antialiasing option, defined below PostFxPresent
static void Mist_Release( int slot );	// (th4) town ground mist, defined above PostFxPresent
static bool Mist_Active();			// (th4) the one place that decides whether the mist shows
static void Rain_Release( int slot );	// (th4) storm-night rain, defined above PostFxPresent
static bool Rain_Active();			// (th4) the one place that decides whether the rain shows
static void BioGlow_Release( int slot );	// (th4) bioluminescent night halo, defined above PostFxPresent
static void Frost_Release( int slot );	// (th4) Ice Age frost on the screen edges, defined above PostFxPresent
static bool Frost_Active();			// (th4) the one place that decides whether the frost needs the mask
static void Retro_Release( int slot );	// (th4) RetroMode, defined above PostFxPresent
static void Atmos_Release( int slot );	// (th4) ATMOSPHERE effects (cloud shadows, spell trails, level-up glow), above PostFxPresent
static bool Atmos_NeedMask();			// (th4) the same block: do any of them need the world/UI mask this frame
static bool Wet_Active();				// (th4) ATMOSPHERE block: WET GROUND shows this frame
static void Zoom_Release( int slot );	// (th4) DYNAMIC ZOOM, defined above RenderQuadTexture
static void SnowFlakes_Release( int slot );	// (th4) SNOWFALL ON THE GPU, defined below Breath_Present
static void SnowDrift_Release( int slot );	// (th4) DRIFTING SNOW, defined just above SNOWFALL ON THE GPU
static bool SnowDrift_Active();				// (th4) the same block: the drift shows this frame

// (th4) Persistent state blocks for the FSR and post-effect passes, one per device slot:
// created once, re-captured each frame (creating a D3DSBT_ALL block every frame is costly).
// Released with the rest of this file's default-pool resources in SharpScale_Release.
static IDirect3DStateBlock9* FsrStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* PostFxStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* AaStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* MistStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* RainStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* FrostStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* RetroStateBlock[2] = { 0, 0 };
static IDirect3DStateBlock9* AtmosStateBlock[2] = { 0, 0 };	// shared by the ATMOSPHERE passes (they run one after another)
static IDirect3DStateBlock9* CaptureStateBlock( IDirect3DStateBlock9*& blk, LPDIRECT3DDEVICE9 dev )
{
	if( !blk ){
		if( D3D_OK != dev->CreateStateBlock( D3DSBT_ALL, &blk ) ) blk = 0;
	}else if( D3D_OK != blk->Capture() ){
		blk->Release(); blk = 0;
	}
	return blk;
}

void SharpScale_Release( int slot )
{
	if( slot < 0 || slot > 1 ) return;
	if( FsrStateBlock[slot] ){ FsrStateBlock[slot]->Release(); FsrStateBlock[slot] = 0; }
	if( PostFxStateBlock[slot] ){ PostFxStateBlock[slot]->Release(); PostFxStateBlock[slot] = 0; }
	if( AaStateBlock[slot] ){ AaStateBlock[slot]->Release(); AaStateBlock[slot] = 0; }
	if( MistStateBlock[slot] ){ MistStateBlock[slot]->Release(); MistStateBlock[slot] = 0; }
	if( RainStateBlock[slot] ){ RainStateBlock[slot]->Release(); RainStateBlock[slot] = 0; }
	if( FrostStateBlock[slot] ){ FrostStateBlock[slot]->Release(); FrostStateBlock[slot] = 0; }
	if( RetroStateBlock[slot] ){ RetroStateBlock[slot]->Release(); RetroStateBlock[slot] = 0; }
	if( AtmosStateBlock[slot] ){ AtmosStateBlock[slot]->Release(); AtmosStateBlock[slot] = 0; }
	if( SharpSurf[slot] ){ SharpSurf[slot]->Release(); SharpSurf[slot] = 0; }
	SharpW[slot] = SharpH[slot] = 0;
	Fsr_Release( slot );	// (th4) same default pool, same reset points
	PostFx_Release( slot );
	Antialias_Release( slot );
	Mist_Release( slot );
	Rain_Release( slot );
	BioGlow_Release( slot );
	Frost_Release( slot );
	Retro_Release( slot );
	Atmos_Release( slot );
	Zoom_Release( slot );	// (th4) DYNAMIC ZOOM targets, mask, shader
	SnowFlakes_Release( slot );	// (th4) SNOWFALL ON THE GPU: shaders, declaration, buffers
	SnowDrift_Release( slot );	// (th4) DRIFTING SNOW: its shader
}

static HRESULT SharpStretch( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 src, const RECT* srcRect,
                             LPDIRECT3DSURFACE9 dst, const RECT* dstRect, D3DTEXTUREFILTERTYPE filter )
{
	if( !dev ) return D3DERR_INVALIDCALL;
	if( !SharpScaling || filter == D3DTEXF_NONE || !src || !dst ){
		return dev->StretchRect( src, srcRect, dst, dstRect, filter );
	}
	D3DSURFACE_DESC sd, dd;
	if( D3D_OK != src->GetDesc( &sd ) || D3D_OK != dst->GetDesc( &dd ) ){
		return dev->StretchRect( src, srcRect, dst, dstRect, filter );
	}
	const int sw = srcRect ? srcRect->right - srcRect->left : (int)sd.Width;
	const int sh = srcRect ? srcRect->bottom - srcRect->top : (int)sd.Height;
	const int dw = dstRect ? dstRect->right - dstRect->left : (int)dd.Width;
	const int dh = dstRect ? dstRect->bottom - dstRect->top : (int)dd.Height;
	if( sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0 ) return dev->StretchRect( src, srcRect, dst, dstRect, filter );
	int kx = ( dw + sw - 1 ) / sw, ky = ( dh + sh - 1 ) / sh;	// ceil(ratio) per axis
	if( kx > 4 ) kx = 4;										// cap the intermediate's size
	if( ky > 4 ) ky = 4;
	if( kx <= 1 && ky <= 1 ) return dev->StretchRect( src, srcRect, dst, dstRect, filter );	// not enlarging
	const int slot = ( dev == D3dDevice ) ? 0 : 1;
	const int iw = sw * kx, ih = sh * ky;
	if( !SharpSurf[slot] || SharpW[slot] != iw || SharpH[slot] != ih || SharpFmt[slot] != sd.Format ){
		SharpScale_Release( slot );
		if( D3D_OK != dev->CreateRenderTarget( iw, ih, sd.Format, D3DMULTISAMPLE_NONE, 0, FALSE, &SharpSurf[slot], 0 ) ){
			SharpSurf[slot] = 0;
			return dev->StretchRect( src, srcRect, dst, dstRect, filter );
		}
		SharpW[slot] = iw; SharpH[slot] = ih; SharpFmt[slot] = sd.Format;
	}
	if( D3D_OK != dev->StretchRect( src, srcRect, SharpSurf[slot], 0, D3DTEXF_POINT ) ){
		return dev->StretchRect( src, srcRect, dst, dstRect, filter );
	}
	return dev->StretchRect( SharpSurf[slot], 0, dst, dstRect, D3DTEXF_LINEAR );
}

// (th4) FSR 1.0 UPSCALING (config "FSR", default off; FSRSharpness 0..20 = 0.0..2.0 stops,
// lower is sharper). AMD FidelityFX Super Resolution 1.0 as two full-screen passes:
//   EASU (FsrEasu.hlsl): game frame -> FsrMidTex at WINDOW size, edge-adaptive upscale;
//   RCAS (FsrRcas.hlsl): FsrMidTex -> back buffer, contrast-adaptive sharpening.
// It replaces the final stretch in the two textured-quad present paths (palette and
// Ultra), and takes precedence over SharpScaling. It needs "filter" on (that is what makes
// those paths render into VideoSurface and stretch at all) and only acts when the window
// is larger than the game resolution. Surface mode (DrawMode 0) has no shaders and keeps
// the plain / sharp stretch. Returns false -> caller falls back to SharpStretch.
// Resources are per device (slot 0 = D3dDevice, 1 = SecDevice), default pool, released
// by SharpScale_Release() on reset / shutdown together with the sharp-scaling surface.
// Shader bytecode: FsrEasu.h / FsrRcas.h, generated by the <FxCompile> items for the two
// .hlsl files in thehell3.vcxproj (ps_3_0, entry points main_easu / main_rcas).
#include "FsrEasu.h"
#include "FsrRcas.h"
static IDirect3DPixelShader9* FsrEasuPS[2] = { 0, 0 };
static IDirect3DPixelShader9* FsrRcasPS[2] = { 0, 0 };
static LPDIRECT3DTEXTURE9 FsrSrcTex[2] = { 0, 0 }, FsrMidTex[2] = { 0, 0 };
static int FsrSrcW[2], FsrSrcH[2], FsrMidW[2], FsrMidH[2];

static void Fsr_Release( int slot )
{
	if( FsrEasuPS[slot] ){ FsrEasuPS[slot]->Release(); FsrEasuPS[slot] = 0; }
	if( FsrRcasPS[slot] ){ FsrRcasPS[slot]->Release(); FsrRcasPS[slot] = 0; }
	if( FsrSrcTex[slot] ){ FsrSrcTex[slot]->Release(); FsrSrcTex[slot] = 0; }
	if( FsrMidTex[slot] ){ FsrMidTex[slot]->Release(); FsrMidTex[slot] = 0; }
	FsrSrcW[slot] = FsrSrcH[slot] = FsrMidW[slot] = FsrMidH[slot] = 0;
}

static bool Fsr_EnsureTex( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9& tex, int& tw, int& th, int w, int h, D3DFORMAT fmt )
{
	if( tex && tw == w && th == h ) return true;
	if( tex ){ tex->Release(); tex = 0; }
	if( D3D_OK != dev->CreateTexture( w, h, 1, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, 0 ) ){ tex = 0; return false; }
	tw = w; th = h;
	return true;
}

static void Fsr_Quad( LPDIRECT3DDEVICE9 dev, int w, int h )
{
	struct V { float x, y, z, rhw, u, v; };
	const float r = (float)w - 0.5f, b = (float)h - 0.5f;
	V q[4] = { { -0.5f, -0.5f, 0, 1, 0, 0 }, { r, -0.5f, 0, 1, 1, 0 }, { -0.5f, b, 0, 1, 0, 1 }, { r, b, 0, 1, 1, 1 } };
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, q, sizeof(V) );
}

static bool FsrPresent( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 src, LPDIRECT3DSURFACE9 dst )
{
	if( !FsrUpscale || !dev || !src || !dst ) return false;
	D3DSURFACE_DESC sd, dd;
	if( D3D_OK != src->GetDesc( &sd ) || D3D_OK != dst->GetDesc( &dd ) ) return false;
	const int sw = (int)sd.Width, sh = (int)sd.Height, dw = (int)dd.Width, dh = (int)dd.Height;
	if( dw <= sw && dh <= sh ) return false;			// not enlarging: nothing for FSR to do
	const int slot = ( dev == D3dDevice ) ? 0 : 1;
	if( !FsrEasuPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_easu, &FsrEasuPS[slot] ) ){ FsrEasuPS[slot] = 0; return false; }
	if( !FsrRcasPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_rcas, &FsrRcasPS[slot] ) ){ FsrRcasPS[slot] = 0; return false; }
	if( !Fsr_EnsureTex( dev, FsrSrcTex[slot], FsrSrcW[slot], FsrSrcH[slot], sw, sh, D3DFMT_X8R8G8B8 ) ) return false;
	if( !Fsr_EnsureTex( dev, FsrMidTex[slot], FsrMidW[slot], FsrMidH[slot], dw, dh, D3DFMT_X8R8G8B8 ) ) return false;

	// the finished frame lives in a plain render-target SURFACE; shaders need a TEXTURE
	LPDIRECT3DSURFACE9 srcTexSurf = 0, midSurf = 0;
	if( D3D_OK != FsrSrcTex[slot]->GetSurfaceLevel( 0, &srcTexSurf ) ) return false;
	if( D3D_OK != FsrMidTex[slot]->GetSurfaceLevel( 0, &midSurf ) ){ srcTexSurf->Release(); return false; }
	bool ok = D3D_OK == dev->StretchRect( src, 0, srcTexSurf, 0, D3DTEXF_NONE );

	IDirect3DStateBlock9* sb = ok ? CaptureStateBlock( FsrStateBlock[slot], dev ) : 0;
	if( ok ){
		dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
		dev->SetRenderState( D3DRS_ZENABLE, FALSE );
		dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
		dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE );
		// pass 1: EASU, game frame -> mid (window size)
		const float c0[4] = { (float)sw / dw, (float)sh / dh, 0.5f * sw / dw - 0.5f, 0.5f * sh / dh - 0.5f };
		const float c1[4] = { 1.0f / sw, 1.0f / sh, 0, 0 };
		ok = D3D_OK == dev->SetRenderTarget( 0, midSurf );
		if( ok ){
			dev->SetPixelShader( FsrEasuPS[slot] );
			dev->SetPixelShaderConstantF( 0, c0, 1 );
			dev->SetPixelShaderConstantF( 1, c1, 1 );
			dev->SetTexture( 0, FsrSrcTex[slot] );
			Fsr_Quad( dev, dw, dh );
		}
		// pass 2: RCAS, mid -> back buffer
		int sharp = FsrSharpness; if( sharp < 0 ) sharp = 0; if( sharp > 20 ) sharp = 20;
		// exp2(-stops) for stops = 0.0 .. 2.0 in tenths (no <math.h> in this project, see stdafx.h)
		static const float kSharp[21] = { 1.00000f, 0.93303f, 0.87055f, 0.81225f, 0.75786f, 0.70711f, 0.65975f, 0.61557f, 0.57435f, 0.53589f, 0.50000f, 0.46652f, 0.43528f, 0.40613f, 0.37893f, 0.35355f, 0.32988f, 0.30779f, 0.28717f, 0.26794f, 0.25000f };
		const float r0[4] = { 1.0f / dw, 1.0f / dh, kSharp[sharp], 0 };
		if( D3D_OK != dev->SetRenderTarget( 0, dst ) ) ok = false;
		if( ok ){
			dev->SetPixelShader( FsrRcasPS[slot] );
			dev->SetPixelShaderConstantF( 0, r0, 1 );
			dev->SetTexture( 0, FsrMidTex[slot] );
			Fsr_Quad( dev, dw, dh );
		}
		dev->SetTexture( 0, 0 );
	}
	if( sb ){ sb->Apply(); }			// kept for the next frame (FsrStateBlock)
	dev->SetRenderTarget( 0, dst );		// the caller presents from here
	srcTexSurf->Release(); midSurf->Release();
	return ok;
}

// (th4) POST EFFECTS (config ChromaticAberration / FilmGrain, both 0 = off by default).
// One ps_3_0 pass (PostFx.hlsl) over the FINISHED back buffer: copy it to PostTex, then draw
// it back through the shader. Called at the very end of both textured-quad present paths
// (palette and Ultra), after the upscale, so it works with or without FSR / SharpScaling
// and at any window size. Surface mode has no shaders and is unaffected. Resources are per
// device like FSR's and released by SharpScale_Release().
#include "PostFx.h"
static IDirect3DPixelShader9* PostFxPS[2] = { 0, 0 };
static LPDIRECT3DTEXTURE9 PostTex[2] = { 0, 0 };
static int PostTexW[2], PostTexH[2];
static LPDIRECT3DTEXTURE9 HeatTex[2] = { 0, 0 };	// L8 heat mask, game resolution, dynamic
static int HeatTexW[2], HeatTexH[2];
#include "River.h"
static IDirect3DPixelShader9* RiverPS[2] = { 0, 0 };	// (th4) RIVER FLOW (River_Present)
static LPDIRECT3DTEXTURE9 RiverFlowTex[2] = { 0, 0 };	// THE FLOW MAP, 112 x 112 A8R8G8B8, DYNAMIC
static uint RiverFlowUp[2] = { 0, 0 };				// TownWater.cpp RiverFlowGen in RiverFlowTex, 0 = none

static void PostFx_Release( int slot )
{
	if( RiverPS[slot] ){ RiverPS[slot]->Release(); RiverPS[slot] = 0; }	// (th4) RIVER FLOW
	if( RiverFlowTex[slot] ){ RiverFlowTex[slot]->Release(); RiverFlowTex[slot] = 0; }
	RiverFlowUp[slot] = 0;
	if( PostFxPS[slot] ){ PostFxPS[slot]->Release(); PostFxPS[slot] = 0; }
	if( PostTex[slot] ){ PostTex[slot]->Release(); PostTex[slot] = 0; }
	if( HeatTex[slot] ){ HeatTex[slot]->Release(); HeatTex[slot] = 0; }
	PostTexW[slot] = PostTexH[slot] = HeatTexW[slot] = HeatTexH[slot] = 0;
}

// (th4) DYNAMIC ZOOM -- THE VIEW: where the world is on screen in the frame PostFxPresent is working on.
// Everything the CPU drew -- the 8-bit world, TownWater's maps, the flame halo list, the breath puffs, and the
// tile -> screen formula (Atmos_AnchorDrawn) -- is in the DRAWN frame (game px; Alt+Z's 2x is drawn by the CPU,
// so it is part of it). With the wheel zoom (DynamicZoom.cpp) the screen shows that world scaled about the
// hero, the UI unscaled on top:   on screen = drawn * ViewZ + ( ViewSX, ViewSY )
// View_Begin (the first thing PostFxPresent does) sets it for the device being drawn: the main device while
// the zoom was composited (DynZoom_EndFrame), identity otherwise -- the second window always shows 1x.
// Every world-anchored pass maps through it: View_Point for positions (and x ViewZ for sizes), View_Z() for the
// "zoom" its shader divides by, Atmos_Anchor for the ground anchor; the maps built in the drawn frame (the heat
// / water mask, the wet map) are carried onto the screen by DynZoom_RemapHalf / DynZoom_RemapFull, and the
// shaders that read drawn-frame layers take the shift (TreeSway / SunRays / BioGlow / PostFx.hlsl).
// A NEW world effect must do the same, or it slides off its spot of ground while the wheel turns.
// KNOWN-GOOD CHECK: with the zoom off every pass gets ViewZ 1 and shift 0 -- exactly the old constants.
static float ViewZ = 1.0f, ViewSX = 0.0f, ViewSY = 0.0f;
static bool  ViewDyn = false;	// the wheel zoom is on screen for this device
static void View_Begin( LPDIRECT3DDEVICE9 dev )
{
	ViewZ = 1.0f; ViewSX = ViewSY = 0.0f; ViewDyn = false;
	if( dev != D3dDevice || !DynZoom_OnScreen() ) return;
	DynZoom_View( ViewZ, ViewSX, ViewSY );
	ViewDyn = ViewZ > 1.0005f;
}
static inline void View_Point( float& x, float& y ) { x = x * ViewZ + ViewSX; y = y * ViewZ + ViewSY; }
// game px on screen per px of the world: Alt+Z's 2, or the wheel's factor (never both: DynZoom_Allowed)
static inline float View_Z() { return ( IsZoomDisable ? 1.0f : 2.0f ) * ViewZ; }
static void Atmos_AnchorDrawn( float& x, float& y );	// ATMOSPHERE block: tile (0,0) in the drawn frame
static void Atmos_Anchor( float& x, float& y );			// the same on screen (THE VIEW applied)

// (th4) HEAT HAZE -- which pixels are burning lava.
// Lava is not a separate layer: cave lava tiles are drawn with the palette entries 1..31,
// the range AnimateCavePalette (palette.cpp ~448) rotates every tick to make it flow. So a
// world pixel with an index in 1..31 on a cave level IS lava. The UI can use those entries
// too, so the world is snapshotted right after DrawIsoLand (Draw.cpp, HeatHaze_SnapshotWorld)
// and a pixel only counts as world if the final frame still holds the same index there --
// anything drawn over it afterwards (panels, text, cursor, tooltips) is left undistorted.
// Levels WITHOUT burning lava, where the same entries are water or ice (HeatHaze_LevelHasLava):
// every non-cave tileset (abyss 1..15 is water, crypt is not lava), the quest maps Infested
// Cellar (27), Poisoned
// Water Supply (28, and its Dark Campaign copy 118), Frozen Loch (39), Island (40), Fetid
// Cavern (51), Mossy Grotto (52) and Still Springs (112, Dark Campaign) -- water there --
// and Ice Age caves outside
// quest maps (the cycle is stopped there and lava reads as ice). Add a map to that list if a
// new cave map uses 1..31 for something that must not shimmer.
static uchar* HeatWorldSnap = 0;	// ScreenWidth x ScreenHeight copy of the world-only frame
static int HeatSnapW = 0, HeatSnapH = 0;
static bool HeatSnapValid = false;
static const uchar* HeatSnapRows = 0;	// (th4) this frame's world-only frame: HeatWorldSnap (pitch ScreenWidth), or with the
static int HeatSnapPitch = 0;			// DYNAMIC ZOOM on, the zoom's own copy of the same pixels (pitch WorkingWidth)

bool HeatHaze_LevelHasLava()
{
	if( GameMode == GM_CLASSIC ) return false;	// (th4) never in Classic, whatever HeatHaze says (Mordor's call)
	if( Dungeon->graphType != DT_3_CAVE ) return false;
	if( is( (DUNGEON)Dungeon, DUN_27_THE_INFESTED_CELLAR, DUN_28_POISONED_WATER_SUPPLY, DUN_118_POISONED_WATER_SUPPLY_DC,
	        DUN_39_FROZEN_LOCH, DUN_40_ISLAND, DUN_51_FETID_CAVERN, DUN_52_MOSSY_GROTTO,
	        DUN_112_STILL_SPRINGS ) ) return false;
	if( (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) && !Dungeon->isQuest ) return false;
	return true;
}

// (th4) WATER SHIMMER uses the same mask. On these cave quest maps the palette entries 1..31
// that are lava elsewhere are WATER (see the exclusion list in HeatHaze_LevelHasLava).
// Only Fetid Cavern (51) has it. Tested and rejected (looked bad): Poisoned Water Supply (28/118),
// Island (40), Still Springs (112), Mossy Grotto (52), Infested Cellar (27). Frozen Loch (39) is
// frozen solid. All of them stay excluded from heat haze (HeatHaze_LevelHasLava).
// (th4) The TOWN RIVER uses the same ripple, but not the 1..31 colour rule: town water shares its
// palette entries with grass and rocks, so TownWater.cpp builds its own per-pixel mask and
// HeatHaze_BuildMask reads that instead (the townWater branch there).
bool PostFx_LevelHasWater()
{
	if( Dungeon == DUN_0_TOWN && !Dungeon->isQuest ) return TownWater_RiverRipples();	// (th4) TownWater.cpp; false = frozen (Ice Age)
	if( Dungeon == DUN_44_RAVENHOLM ) return TownWater_RiverRipples();	// (th4) its stagnant river ripples too (TownWater.cpp RAVENHOLM'S RIVER)
	if( Dungeon->graphType != DT_3_CAVE ) return false;
	return (DUNGEON)Dungeon == DUN_51_FETID_CAVERN;
}

// (th4) SHOCKWAVE rings: short-lived distortion rings from explosions and novas, queued by
// CastMissile (Missile.cpp) for the missile types listed there. Newest replaces oldest.
struct ShockRing { int row, col, ox, oy; DWORD t0; bool used; };
static ShockRing ShockRings[4] = {};
static const DWORD SHOCK_MS = 550;			// lifetime of one ring
// ox / oy: the missile's pixel displacement from its tile (Missile::ColDisplacement /
// RowDisplacement, as DrawMissile uses them, Draw.cpp ~3783), so the ring starts where the
// explosion is DRAWN, not at the tile's ground point.
void Shockwave_Add( int row, int col, int ox, int oy )
{
	if( Shockwave <= 0 || GameMode == GM_CLASSIC ) return;	// (th4) never in Classic, whatever Shockwave says
	int slot = 0; DWORD oldest = 0xFFFFFFFF;
	for( int i = 0; i < 4; ++i ){
		if( !ShockRings[i].used ){ slot = i; break; }
		if( ShockRings[i].t0 < oldest ){ oldest = ShockRings[i].t0; slot = i; }
	}
	ShockRings[slot].row = row; ShockRings[slot].col = col; ShockRings[slot].ox = ox; ShockRings[slot].oy = oy;
	ShockRings[slot].t0 = GetTickCount(); ShockRings[slot].used = true;
}
static bool Shockwave_Active()
{
	if( Shockwave <= 0 || GameMode == GM_CLASSIC ) return false;	// (th4) never in Classic (Shockwave_Add refuses there too)
	const DWORD now = GetTickCount();
	for( int i = 0; i < 4; ++i ) if( ShockRings[i].used && now - ShockRings[i].t0 < SHOCK_MS ) return true;
	return false;
}

// Which effects need the world/UI mask this frame (it costs a snapshot + a CPU pass).
static bool PostFx_NeedMask()
{
	return ( HeatHaze > 0 && HeatHaze_LevelHasLava() ) || ( WaterShimmer > 0 && PostFx_LevelHasWater() )
	    || TiltShift > 0 || Shockwave_Active()
	    || Mist_Active()	// (th4) the ground mist reads the world/UI mask
	    || Rain_Active()	// (th4) so does the storm rain
	    || GlowNight_Active() || Fireflies_Active() || BloodNight_Active() || IceTrees_Active()	// (th4) bioluminescent / firefly / blood moon
	    || Frost_Active()	// (th4) Ice Age frost on the screen edges
	    || Atmos_NeedMask();	// (th4) cloud shadows, spell trails, level-up glow
}

// (th4) UI RECTS: the panels drawn this frame, in screen game px. The mask below calls a pixel UI when the
// finished frame DIFFERS from the world snapshot -- but a UI pixel that happens to equal the world under it
// is missed. That is common for BLACK: the panels' black areas (palette 0) over an unlit floor in standard
// lighting (also palette 0) passed as world, and the flame halos, breath, rain... lit the panel up in the
// shape of the dark floor behind it ("the brazier shows through the panel", Mordor 2026-10-04). So every
// panel tells the mask its rectangle as it is drawn (Draw.cpp, the panel block after the world: one
// HeatMask_AddUiRect per Draw* call, and the main panel), and those rectangles are UI whatever the pixels
// say. A NEW PANEL needs the same call next to its Draw function, or its black parts can glow.
static int UiRects[32][4];	// x0, y0, x1, y1 (exclusive)
static int UiRectN = 0;
void HeatMask_AddUiRect( const DisplayObject& r )
{
	if( UiRectN >= 32 || r.Width <= 0 || r.Heigth <= 0 ) return;
	UiRects[ UiRectN ][ 0 ] = r.Left; UiRects[ UiRectN ][ 1 ] = r.Top;
	UiRects[ UiRectN ][ 2 ] = r.Left + r.Width; UiRects[ UiRectN ][ 3 ] = r.Top + r.Heigth;
	++UiRectN;
}

// Called right after DrawIsoLand: remember the world as drawn, before any UI goes on top.
void HeatHaze_SnapshotWorld()
{
	UiRectN = 0;	// (th4) UI RECTS: collected again by this frame's panels
	HeatSnapValid = false; HeatSnapRows = 0;
	if( !PostFx_NeedMask() ) return;
	// (th4) DYNAMIC ZOOM: DynZoom_SnapshotWorld (called just before, Draw.cpp) already copied these pixels -- use its
	// copy rather than make a second one. DrawRedScreen remaps that copy too (DynZoom_OnRedScreen), so under the red
	// screen HeatHaze_BuildMask / Wet_BuildMask call the frame all UI themselves (DynZoom_RedScreen).
	if( const uchar* zc = DynZoom_FrameCopy() ){ HeatSnapRows = zc; HeatSnapPitch = WorkingWidth; HeatSnapValid = true; return; }
	if( HeatSnapW != ScreenWidth || HeatSnapH != ScreenHeight ){
		free( HeatWorldSnap ); HeatWorldSnap = (uchar*)malloc( ScreenWidth * ScreenHeight );
		HeatSnapW = HeatWorldSnap ? ScreenWidth : 0; HeatSnapH = HeatWorldSnap ? ScreenHeight : 0;
	}
	if( !HeatWorldSnap ) return;
	const uchar* src = SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	for( int y = 0; y < ScreenHeight; ++y ) memcpy( HeatWorldSnap + y * ScreenWidth, src + y * WorkingWidth, ScreenWidth );
	HeatSnapRows = HeatWorldSnap; HeatSnapPitch = ScreenWidth;
	HeatSnapValid = true;
}

#include <emmintrin.h>	// (th4) SSE2 for HeatHaze_BuildMask (helpers below)
// (th4) HEAT MASK SSE2 HELPERS (HeatHaze_BuildMask). Each processes 16 mask texels (32 game pixels)
// per step and produces exactly what the scalar loops below them produce; the scalar loops still run
// for the row tail. Byte flags are 0x00 / 0xFF.
// Pair-OR: texel k of a 16-texel block covers pixels 2k and 2k+1, i.e. the two bytes of 16-bit lane
// k. (lane | lane >> 8) & 0xFF puts their OR in the low byte; packus then packs two 8-texel halves.
static inline __m128i HM_PairOr( __m128i lo, __m128i hi )
{
	const __m128i ff = _mm_set1_epi16( 0x00FF );
	lo = _mm_and_si128( _mm_or_si128( lo, _mm_srli_epi16( lo, 8 ) ), ff );
	hi = _mm_and_si128( _mm_or_si128( hi, _mm_srli_epi16( hi, 8 ) ), ff );
	return _mm_packus_epi16( lo, hi );
}
static inline __m128i HM_NonZero( __m128i v )	// 0xFF where v != 0
{
	return _mm_xor_si128( _mm_cmpeq_epi8( v, _mm_setzero_si128() ), _mm_set1_epi8( (char)0xFF ) );
}
static inline __m128i HM_Is1to31( __m128i w )	// 0xFF where w is 1..31: (uchar)(w - 1) <= 30
{
	const __m128i x = _mm_sub_epi8( w, _mm_set1_epi8( 1 ) );
	return _mm_cmpeq_epi8( _mm_min_epu8( x, _mm_set1_epi8( 30 ) ), x );
}
static inline __m128i HM_Sel( __m128i m, __m128i a, __m128i b )	// m ? a : b, per byte
{
	return _mm_or_si128( _mm_and_si128( m, a ), _mm_andnot_si128( m, b ) );
}
static inline int HM_Count( __m128i m )	// number of 0xFF bytes
{
	const __m128i s = _mm_sad_epu8( _mm_and_si128( m, _mm_set1_epi8( 1 ) ), _mm_setzero_si128() );
	return _mm_cvtsi128_si32( s ) + _mm_cvtsi128_si32( _mm_srli_si128( s, 8 ) );
}
// Classify one 16-texel block of a cave/dungeon row (the non-town loop). Returns the lava count.
static inline int HM_ClassBlock( const uchar* w0, const uchar* w1, const uchar* f0, const uchar* f1,
                                 uchar* c, uchar S )
{
	const __m128i w0a = _mm_loadu_si128( (const __m128i*)w0 ), w0b = _mm_loadu_si128( (const __m128i*)( w0 + 16 ) );
	const __m128i w1a = _mm_loadu_si128( (const __m128i*)w1 ), w1b = _mm_loadu_si128( (const __m128i*)( w1 + 16 ) );
	const __m128i f0a = _mm_loadu_si128( (const __m128i*)f0 ), f0b = _mm_loadu_si128( (const __m128i*)( f0 + 16 ) );
	const __m128i f1a = _mm_loadu_si128( (const __m128i*)f1 ), f1b = _mm_loadu_si128( (const __m128i*)( f1 + 16 ) );
	const __m128i ui = HM_NonZero( HM_PairOr( _mm_or_si128( _mm_xor_si128( f0a, w0a ), _mm_xor_si128( f1a, w1a ) ),
	                                          _mm_or_si128( _mm_xor_si128( f0b, w0b ), _mm_xor_si128( f1b, w1b ) ) ) );
	const __m128i lava = HM_NonZero( HM_PairOr( _mm_or_si128( HM_Is1to31( w0a ), HM_Is1to31( w1a ) ),
	                                            _mm_or_si128( HM_Is1to31( w0b ), HM_Is1to31( w1b ) ) ) );
	__m128i cls = HM_Sel( lava, _mm_set1_epi8( (char)S ), _mm_set1_epi8( 1 ) );
	__m128i sprite = _mm_setzero_si128();
	if( S == 3 ){
		const __m128i hb = _mm_set1_epi8( (char)0x80 );
		sprite = HM_NonZero( HM_PairOr( _mm_and_si128( _mm_or_si128( w0a, w1a ), hb ), _mm_and_si128( _mm_or_si128( w0b, w1b ), hb ) ) );
		cls = HM_Sel( sprite, _mm_set1_epi8( 4 ), cls );
	}
	cls = _mm_andnot_si128( ui, cls );
	_mm_storeu_si128( (__m128i*)c, cls );
	return S != 1 ? HM_Count( _mm_andnot_si128( _mm_or_si128( ui, sprite ), lava ) ) : 0;
}
// Town river block (the townWater branch with a mask row). Returns the water count.
static inline int HM_TownBlock( const uchar* w0, const uchar* w1, const uchar* f0, const uchar* f1,
                                const uchar* m0, const uchar* m1, uchar* c, uchar S )
{
	const __m128i z = _mm_setzero_si128();
	__m128i w[4], m[4];
	const uchar* wp[4] = { w0, w0 + 16, w1, w1 + 16 };
	const uchar* fp[4] = { f0, f0 + 16, f1, f1 + 16 };
	const uchar* mp[4] = { m0, m0 + 16, m1, m1 + 16 };
	__m128i uiHalf[2] = { z, z }, match[4], spr[4];
	for( int i = 0; i < 4; ++i ){
		w[i] = _mm_loadu_si128( (const __m128i*)wp[i] );
		m[i] = _mm_loadu_si128( (const __m128i*)mp[i] );
		const __m128i f = _mm_loadu_si128( (const __m128i*)fp[i] );
		uiHalf[i & 1] = _mm_or_si128( uiHalf[i & 1], _mm_xor_si128( f, w[i] ) );
		match[i] = _mm_andnot_si128( _mm_cmpeq_epi8( m[i], z ), _mm_cmpeq_epi8( m[i], w[i] ) );	// m && m == w
		spr[i] = _mm_andnot_si128( match[i], _mm_cmplt_epi8( w[i], z ) );					// !match && (w & 0x80)
	}
	const __m128i ui     = HM_NonZero( HM_PairOr( uiHalf[0], uiHalf[1] ) );
	const __m128i water  = HM_NonZero( HM_PairOr( _mm_or_si128( match[0], match[2] ), _mm_or_si128( match[1], match[3] ) ) );
	const __m128i sprite = HM_NonZero( HM_PairOr( _mm_or_si128( spr[0], spr[2] ), _mm_or_si128( spr[1], spr[3] ) ) );
	__m128i cls = HM_Sel( water, _mm_set1_epi8( (char)S ), _mm_set1_epi8( 1 ) );
	cls = HM_Sel( sprite, _mm_set1_epi8( 4 ), cls );
	cls = _mm_andnot_si128( ui, cls );
	_mm_storeu_si128( (__m128i*)c, cls );
	return HM_Count( _mm_andnot_si128( _mm_or_si128( ui, sprite ), water ) );
}

// (th4) THE AUTOMAP IS UI TOO. With MapBlend on (the default) the automap's lines and dots are NOT drawn into the
// 8-bit frame: AutomapDrawLine / AutomapDrawPoint1 (automap.cpp) only collect them, and RenderQuadTexture /
// RenderQuadTextureUltra draw them as D3D primitives straight into the true-colour frame. Every mask that finds the UI
// by comparing the 8-bit frame with the world snapshot -- the world/UI mask here, the GRASS world map -- therefore took
// them for world: blades of grass grew out of them in the map's colour and over them (Mordor: "a part of the map
// displayed there, which also colours the grass" -- in town only a DevelopMode build draws the map, DrawAutoMap; in
// Ravenholm every player has it), and the water ripple shook the lines over the river. This marks their pixels UI
// (0) in such a mask, one pixel around each too (a D3D line does not cover exactly Bresenham's pixels), in SCREEN
// game px like the lines -- so after DynZoom_Remap* under the wheel zoom (the map is not zoomed). half: the mask has
// one texel per 2 x 2 game px (this world/UI mask). Without MapBlend the lines are 8-bit pixels and need nothing.
extern unsigned int AutomapPointsCount;
extern unsigned int AutomapLinePointsCount;
extern line_vertex AutomapPoints[20000];
extern line_vertex AutomapLines[10000];
static void Automap_MarkUi( uchar* mask, int pitch, int w, int h, bool half )
{
	if( gbRunGame != 1 || !AutomapMode || !MapBlend ) return;
	const int sh = half ? 1 : 0;
	auto mark = [&]( int x, int y ){
		for( int dy = -1; dy <= 1; ++dy ) for( int dx = -1; dx <= 1; ++dx ){
			const int mx = ( x + dx ) >> sh, my = ( y + dy ) >> sh;
			if( mx >= 0 && my >= 0 && mx < w && my < h ) mask[ (size_t)my * pitch + mx ] = 0;
		}
	};
	for( uint i = 0; i + 1 < AutomapLinePointsCount; i += 2 ){
		int x0 = (int)AutomapLines[ i ].pos.x, y0 = (int)AutomapLines[ i ].pos.y;
		const int x1 = (int)AutomapLines[ i + 1 ].pos.x, y1 = (int)AutomapLines[ i + 1 ].pos.y;
		const int dx = abs( x1 - x0 ), sx = x0 < x1 ? 1 : -1, dy = -abs( y1 - y0 ), sy = y0 < y1 ? 1 : -1;
		int err = dx + dy;
		for( int n = 0; n < 4096; ++n ){	// Bresenham; map lines are short, the cap only guards a broken vertex
			mark( x0, y0 );
			if( x0 == x1 && y0 == y1 ) break;
			const int e2 = 2 * err;
			if( e2 >= dy ){ err += dy; x0 += sx; }
			if( e2 <= dx ){ err += dx; y0 += sy; }
		}
	}
	for( uint i = 0; i < AutomapPointsCount; ++i ) mark( (int)AutomapPoints[ i ].pos.x, (int)AutomapPoints[ i ].pos.y );
}

// Build the L8 mask (see PostFx.hlsl) from the snapshot and the finished 8-bit frame.
static bool HeatHaze_BuildMask( LPDIRECT3DDEVICE9 dev, int slot )
{
	// PERFORMANCE: this runs every RENDERED frame (smooth scrolling can be 300+ fps), so the
	// mask is built at HALF resolution (one texel per 2x2 game pixels), with tight loops and
	// no per-pixel lambdas or bounds checks. Haze needs no per-pixel precision; the UI test
	// stays exact because a texel is UI if ANY of its four pixels changed after the world pass.
	// If no lava is visible this frame the function returns false and the shader skips the haze.
	if( !HeatSnapValid || !HeatSnapRows ) return false;
	HeatSnapValid = false;	// one use per drawn frame
	const int W = ScreenWidth, H = ScreenHeight, W2 = W / 2, H2 = H / 2;
	if( W2 < 3 || H2 < 3 ) return false;
	// 1) classify 2x2 blocks: 0 = UI over the world, 1 = world, 2 = lava
	static uchar* cls = 0; static uchar* tmp = 0; static int clsSize = 0;
	if( clsSize != W2 * H2 ){ free( cls ); free( tmp ); cls = (uchar*)malloc( W2 * H2 ); tmp = (uchar*)malloc( W2 * H2 ); clsSize = ( cls && tmp ) ? W2 * H2 : 0; }
	if( !clsSize ) return false;
	// (th4) DYNAMIC ZOOM (THE VIEW above): the classes are found in the DRAWN frame -- the world snapshot
	// against itself, so no UI there -- and carried onto the screen at the end (DynZoom_RemapHalf, with the
	// zoom's own UI mask, panel rectangles included). Under DrawRedScreen everything is UI (no mask), as
	// without the zoom, where the red frame differs from the snapshot everywhere.
	const bool dz = ViewDyn;
	if( dz && DynZoom_RedScreen() ) return false;
	const uchar* fin = dz ? HeatSnapRows : SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	const int finPitch = dz ? HeatSnapPitch : WorkingWidth;
	// (th4) DYNAMIC ZOOM, PERFORMANCE: only the texels the zoomed view shows are worked out -- cx0..cx1 x cy0..cy1 =
	// the box DynZoom_RemapHalf reads, plus 3 texels: the closing below changes at most 2 texels in from the edge of
	// the area it runs on, so inside it every texel is exactly what a whole-screen pass gives. ~5% of the screen at
	// x4.5 (measured 2026-10-07, 1080p, town: this mask went from 1.55 to 0.42 ms a frame). Without the zoom: the
	// whole screen, as always.
	int cx0 = 0, cx1 = W2, cy0 = 0, cy1 = H2;
	if( dz ){
		int bx0, by0, bx1, by1;
		if( !DynZoom_ShownBox( bx0, by0, bx1, by1 ) ) return false;
		cx0 = max( 0, bx0 / 2 - 3 ); cx1 = min( W2, ( bx1 - 1 ) / 2 + 4 );
		cy0 = max( 0, by0 / 2 - 3 ); cy1 = min( H2, ( by1 - 1 ) / 2 + 4 );
		if( cx1 - cx0 < 3 || cy1 - cy0 < 3 ){ cx0 = 0; cx1 = W2; cy0 = 0; cy1 = H2; }
	}
	// entries 1..31 are lava on lava levels and water on the water maps; S = the class they get
	const bool lavaOn = HeatHaze > 0 && HeatHaze_LevelHasLava();
	const bool waterOn = !lavaOn && WaterShimmer > 0 && PostFx_LevelHasWater();
	// (th4) Town river: water comes from TownWater.cpp's mask, not from entries 1..31 (0 when this
	// frame shows no river -- then nothing is water and the town gets the plain world mask).
	const bool inTown = ( Dungeon == DUN_0_TOWN && !Dungeon->isQuest ) || Dungeon == DUN_44_RAVENHOLM;	// (th4) TownWater.cpp's river mask: the town, and Ravenholm (RAVENHOLM'S RIVER)
	const uchar* townWater = ( waterOn && inTown ) ? TownWater_MaskRows() : 0;
	const uchar S = lavaOn ? 2 : ( waterOn ? 3 : 1 );
	int lavaCount = 0;
	// (th4) The three loop groups below (classify, close, map) run 16 texels at a time with SSE2
	// (HM_* helpers above), byte-identical to the scalar loops, which still finish each row tail.
	// Verified 2026-10-02 against the previous scalar-only version on 6000 random frames (all
	// modes, odd sizes, UI overlap, sprites, town mask present/absent): about 8x faster.
	for( int y2 = cy0; y2 < cy1; ++y2 ){	// 16 texels at a time; the scalar loops finish the row
		const uchar* w0 = HeatSnapRows + ( 2 * y2 ) * HeatSnapPitch;  const uchar* w1 = w0 + HeatSnapPitch;
		const uchar* f0 = fin + ( 2 * y2 ) * finPitch; const uchar* f1 = f0 + finPitch;
		uchar* c = cls + y2 * W2;
		if( waterOn && inTown ){
			const uchar* m0 = townWater ? townWater + ( 2 * y2 ) * WorkingWidth : 0;
			const uchar* m1 = m0 ? m0 + WorkingWidth : 0;
			int x2 = cx0;
			if( m0 ) for( ; x2 + 16 <= cx1; x2 += 16 ) lavaCount += HM_TownBlock( w0 + 2 * x2, w1 + 2 * x2, f0 + 2 * x2, f1 + 2 * x2, m0 + 2 * x2, m1 + 2 * x2, c + x2, S );
			for( ; x2 < cx1; ++x2 ){
				const int x = 2 * x2;
				if( ( f0[x] ^ w0[x] ) | ( f0[x + 1] ^ w0[x + 1] ) | ( f1[x] ^ w1[x] ) | ( f1[x + 1] ^ w1[x + 1] ) ){ c[x2] = 0; continue; }
				if( !m0 ){ c[x2] = 1; continue; }
				const bool a = m0[x] && m0[x] == w0[x], b = m0[x + 1] && m0[x + 1] == w0[x + 1];
				const bool d = m1[x] && m1[x] == w1[x], e = m1[x + 1] && m1[x + 1] == w1[x + 1];
				const bool sprite = ( !a && ( w0[x] & 0x80 ) ) || ( !b && ( w0[x + 1] & 0x80 ) )
				                 || ( !d && ( w1[x] & 0x80 ) ) || ( !e && ( w1[x + 1] & 0x80 ) );
				if( sprite ){ c[x2] = 4; continue; }
				const bool water = a || b || d || e;
				c[x2] = water ? S : 1;
				lavaCount += water;
			}
			continue;
		}
		int x2 = cx0;
		for( ; x2 + 16 <= cx1; x2 += 16 ) lavaCount += HM_ClassBlock( w0 + 2 * x2, w1 + 2 * x2, f0 + 2 * x2, f1 + 2 * x2, c + x2, S );
		for( ; x2 < cx1; ++x2 ){
			const int x = 2 * x2;
			if( ( f0[x] ^ w0[x] ) | ( f0[x + 1] ^ w0[x + 1] ) | ( f1[x] ^ w1[x] ) | ( f1[x + 1] ^ w1[x + 1] ) ){ c[x2] = 0; continue; }
			const bool lava = (uchar)( w0[x] - 1 ) < 31 || (uchar)( w0[x + 1] - 1 ) < 31 || (uchar)( w1[x] - 1 ) < 31 || (uchar)( w1[x + 1] - 1 ) < 31;
			if( S == 3 && ( ( w0[x] | w0[x + 1] | w1[x] | w1[x + 1] ) & 0x80 ) ){ c[x2] = 4; continue; }
			c[x2] = lava ? S : 1;
			lavaCount += lava && S != 1;
		}
	}
	// (th4) UI RECTS (above HeatHaze_SnapshotWorld): the panels are UI, even where they match the world
	// (under the DYNAMIC ZOOM they are in its UI mask, applied by DynZoom_RemapHalf below)
	for( int i = 0; i < ( dz ? 0 : UiRectN ); ++i ){
		const int x0 = max( 0, UiRects[ i ][ 0 ] >> 1 ), y0 = max( 0, UiRects[ i ][ 1 ] >> 1 );
		const int x1 = min( W2, ( UiRects[ i ][ 2 ] + 1 ) >> 1 ), y1 = min( H2, ( UiRects[ i ][ 3 ] + 1 ) >> 1 );
		for( int y2 = y0; y2 < y1; ++y2 ) if( x1 > x0 ) memset( cls + y2 * W2 + x0, 0, x1 - x0 );
	}
	// nothing special on screen and no effect that needs the plain world/UI mask: skip it all
	if( !lavaCount && TiltShift <= 0 && !Shockwave_Active() && !Mist_Active() && !Rain_Active() && !GlowNight_Active() && !Fireflies_Active() && !BloodNight_Active() && !Frost_Active() && !IceTrees_Active() && !Atmos_NeedMask() ) return false;
	// 2) CLOSE the lava area by 1 texel (= 2 game px): dilate then erode, separable. Lava tiles
	// are drawn in blocks whose edges carry a few pixels outside 1..31; unclosed, those seams
	// get a different shimmer phase and the tile grid shows through. UI (0) is never changed.
	// SSE2 closing: same rules as the scalar loops kept for the tails / row ends. x runs cx0+1..cx1-2 in the
	// horizontal passes (the area's ends have their own one-sided rules), cx0..cx1-1 in the vertical ones;
	// cx0..cx1 x cy0..cy1 is the whole screen unless the DYNAMIC ZOOM narrowed it (above).
	const __m128i vS = _mm_set1_epi8( (char)S ), v1 = _mm_set1_epi8( 1 ), v4 = _mm_set1_epi8( 4 ), vz = _mm_setzero_si128();
	// dilate horizontally cls -> tmp
	for( int y = cy0; y < cy1; ++y ){
		const uchar* s = cls + y * W2; uchar* d = tmp + y * W2;
		d[cx0] = ( s[cx0] && s[cx0] != 4 && ( s[cx0] == S || s[cx0 + 1] == S ) ) ? S : s[cx0];
		int x = cx0 + 1;
		for( ; x + 16 <= cx1 - 1; x += 16 ){
			const __m128i v = _mm_loadu_si128( (const __m128i*)( s + x ) );
			const __m128i l = _mm_loadu_si128( (const __m128i*)( s + x - 1 ) ), r = _mm_loadu_si128( (const __m128i*)( s + x + 1 ) );
			const __m128i grow = _mm_andnot_si128( _mm_or_si128( _mm_cmpeq_epi8( v, vz ), _mm_cmpeq_epi8( v, v4 ) ),
			                     _mm_or_si128( _mm_or_si128( _mm_cmpeq_epi8( v, vS ), _mm_cmpeq_epi8( l, vS ) ), _mm_cmpeq_epi8( r, vS ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), HM_Sel( grow, vS, v ) );
		}
		for( ; x < cx1 - 1; ++x ){ const uchar v = s[x]; d[x] = ( v && v != 4 && ( v == S || s[x - 1] == S || s[x + 1] == S ) ) ? S : v; }
		d[cx1 - 1] = ( s[cx1 - 1] && s[cx1 - 1] != 4 && ( s[cx1 - 1] == S || s[cx1 - 2] == S ) ) ? S : s[cx1 - 1];
	}
	// dilate vertically tmp -> cls
	for( int y = cy0; y < cy1; ++y ){
		const uchar* s = tmp + y * W2; const uchar* up = y > cy0 ? s - W2 : s; const uchar* dn = y < cy1 - 1 ? s + W2 : s;
		uchar* d = cls + y * W2;
		int x = cx0;
		for( ; x + 16 <= cx1; x += 16 ){
			const __m128i v = _mm_loadu_si128( (const __m128i*)( s + x ) );
			const __m128i u = _mm_loadu_si128( (const __m128i*)( up + x ) ), n = _mm_loadu_si128( (const __m128i*)( dn + x ) );
			const __m128i grow = _mm_andnot_si128( _mm_or_si128( _mm_cmpeq_epi8( v, vz ), _mm_cmpeq_epi8( v, v4 ) ),
			                     _mm_or_si128( _mm_or_si128( _mm_cmpeq_epi8( v, vS ), _mm_cmpeq_epi8( u, vS ) ), _mm_cmpeq_epi8( n, vS ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), HM_Sel( grow, vS, v ) );
		}
		for( ; x < cx1; ++x ){ const uchar v = s[x]; d[x] = ( v && v != 4 && ( v == S || up[x] == S || dn[x] == S ) ) ? S : v; }
	}
	// erode horizontally cls -> tmp (a lava/water texel next to plain world becomes world; UI neighbours don't erode)
	for( int y = cy0; y < cy1; ++y ){
		const uchar* s = cls + y * W2; uchar* d = tmp + y * W2;
		d[cx0] = s[cx0]; d[cx1 - 1] = s[cx1 - 1];
		int x = cx0 + 1;
		for( ; x + 16 <= cx1 - 1; x += 16 ){
			const __m128i v = _mm_loadu_si128( (const __m128i*)( s + x ) );
			const __m128i l = _mm_loadu_si128( (const __m128i*)( s + x - 1 ) ), r = _mm_loadu_si128( (const __m128i*)( s + x + 1 ) );
			const __m128i plain = _mm_or_si128( _mm_or_si128( _mm_cmpeq_epi8( l, v1 ), _mm_cmpeq_epi8( r, v1 ) ),
			                                    _mm_or_si128( _mm_cmpeq_epi8( l, v4 ), _mm_cmpeq_epi8( r, v4 ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), HM_Sel( _mm_and_si128( _mm_cmpeq_epi8( v, vS ), plain ), v1, v ) );
		}
		for( ; x < cx1 - 1; ++x ){ const uchar v = s[x]; d[x] = ( v == S && ( s[x - 1] == 1 || s[x + 1] == 1 || s[x - 1] == 4 || s[x + 1] == 4 ) ) ? 1 : v; }
	}
	// erode vertically tmp -> cls
	for( int y = cy0; y < cy1; ++y ){
		const uchar* s = tmp + y * W2; const uchar* up = y > cy0 ? s - W2 : s; const uchar* dn = y < cy1 - 1 ? s + W2 : s;
		uchar* d = cls + y * W2;
		int x = cx0;
		for( ; x + 16 <= cx1; x += 16 ){
			const __m128i v = _mm_loadu_si128( (const __m128i*)( s + x ) );
			const __m128i u = _mm_loadu_si128( (const __m128i*)( up + x ) ), n = _mm_loadu_si128( (const __m128i*)( dn + x ) );
			const __m128i plain = _mm_or_si128( _mm_or_si128( _mm_cmpeq_epi8( u, v1 ), _mm_cmpeq_epi8( n, v1 ) ),
			                                    _mm_or_si128( _mm_cmpeq_epi8( u, v4 ), _mm_cmpeq_epi8( n, v4 ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), HM_Sel( _mm_and_si128( _mm_cmpeq_epi8( v, vS ), plain ), v1, v ) );
		}
		for( ; x < cx1; ++x ){ const uchar v = s[x]; d[x] = ( v == S && ( up[x] == 1 || dn[x] == 1 || up[x] == 4 || dn[x] == 4 ) ) ? 1 : v; }
	}
	// (th4) RUNNING RIVER: a water texel holding a hero REFLECTION pixel becomes class 5 (code 150): River.hlsl ripples
	// it but does not slide it (that would drag the reflection downstream); PostFx.hlsl's ripple takes 150 for
	// water like 170. Marked after the closing, which knows only one water class. The pixels: TownWater.cpp
	// RiverReflPix, in the drawn frame like these classes.
	if( townWater ){
		int n = 0;
		const int* rp = TownWater_ReflectionPixels( n );
		for( int i = 0; i < n; ++i ){
			const int x2 = ( rp[ i ] & 0xFFFF ) >> 1, y2 = rp[ i ] >> 17;
			if( x2 >= cx0 && x2 < cx1 && y2 >= cy0 && y2 < cy1 && cls[ y2 * W2 + x2 ] == S ) cls[ y2 * W2 + x2 ] = 5;
		}
	}
	// (th4) DYNAMIC ZOOM: the classes of the drawn frame onto the screen (tmp is free after the closing)
	const uchar* out = cls;
	if( dz ){
		if( !DynZoom_RemapHalf( cls, W2, H2, tmp, 0 ) ) return false;
		out = tmp;
	}
	// 3) upload (half resolution L8)
	if( !HeatTex[slot] || HeatTexW[slot] != W2 || HeatTexH[slot] != H2 ){
		if( HeatTex[slot] ){ HeatTex[slot]->Release(); HeatTex[slot] = 0; }
		if( D3D_OK != dev->CreateTexture( W2, H2, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &HeatTex[slot], 0 ) ){ HeatTex[slot] = 0; return false; }
		HeatTexW[slot] = W2; HeatTexH[slot] = H2;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != HeatTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
	// L8 codes read by PostFx.hlsl: 0 = UI over the world, 80 = world, 255 = lava, 170 = water;
	// class 4 (a sprite on a water map) is plain world to the shader; class 5 (water with a hero reflection,
	// RUNNING RIVER above) = 150, water to PostFx.hlsl, "do not slide" to River.hlsl
	static const uchar kOut[6] = { 0, 80, 255, 170, 80, 150 };
	for( int y = 0; y < H2; ++y ){
		const uchar* c = out + y * W2;
		uchar* d = (uchar*)lr.pBits + y * lr.Pitch;
		int x = 0;
		for( ; x + 16 <= W2; x += 16 ){	// SSE2 form of kOut[]: classes are only ever 0..5
			const __m128i v = _mm_loadu_si128( (const __m128i*)( c + x ) );
			__m128i o = _mm_and_si128( _mm_or_si128( _mm_cmpeq_epi8( v, v1 ), _mm_cmpeq_epi8( v, v4 ) ), _mm_set1_epi8( 80 ) );
			o = _mm_or_si128( o, _mm_cmpeq_epi8( v, _mm_set1_epi8( 2 ) ) );	// 255
			o = _mm_or_si128( o, _mm_and_si128( _mm_cmpeq_epi8( v, _mm_set1_epi8( 3 ) ), _mm_set1_epi8( (char)170 ) ) );
			o = _mm_or_si128( o, _mm_and_si128( _mm_cmpeq_epi8( v, _mm_set1_epi8( 5 ) ), _mm_set1_epi8( (char)150 ) ) );
			_mm_storeu_si128( (__m128i*)( d + x ), o );
		}
		for( ; x < W2; ++x ) d[x] = kOut[ c[x] ];
	}
	Automap_MarkUi( (uchar*)lr.pBits, lr.Pitch, W2, H2, true );	// (th4) THE AUTOMAP IS UI TOO (above)
	ItemOverlay_MarkUi( (uchar*)lr.pBits, lr.Pitch, W2, H2, true );	// (th4) THE ITEM ART IS UI TOO (ItemOverlay.cpp): the dragged item over the world
	HeatTex[slot]->UnlockRect( 0 );
	return true;
}

// (th4) GROUND MIST -- slow banks of mist over the NORMAL town on MIST nights (Weather.cpp
// Weather_Tonight: every night is rolled clear / mist / storm, synced in multiplayer). It fades in
// and out with the day/night cycle. GroundMist (vars.h) is fixed at 5, the tuned look; its config
// row is disabled FOR NOW (a per-player switch would break the synced weather), the code still
// scales the opacity by GroundMist / 5 so the option can come back as it was.
// WHICH PASS THIS IS: one window-size shader pass (Mist.hlsl), the first thing PostFxPresent does
// after HeatHaze_BuildMask, so it runs after FSR / the stretch and before the PostFx.hlsl effects.
// It needs the world/UI mask (mist never covers panels, text or the cursor), so PostFx_NeedMask
// and HeatHaze_BuildMask both ask Mist_Active(). The mist pattern is fixed to the ground (same
// tile (0,0) anchor as the water ripple, c9 in PostFxPresent) and drifts on its own clock that
// stops while the game is paused (GamePaused / IsPausedInMenu, as Snowfall.cpp and AnimOrbs.cpp).
// NOT shown: Classic (no day/night cycle), Ice Age (it snows there, Snowfall.cpp), quest maps
// on the town tileset such as Ravenholm (not Dungeon == DUN_0_TOWN non-quest), dungeons.
// Strength = DayNight_NightQ8() (palette.cpp: the same eased phase as the terrain night blend) x
// MIST_ALPHA x GroundMist / 5. The mask is read with LINEAR filtering and turned into a 0..1
// weight (Mist.hlsl): it is half resolution and marks a whole 2x2 block as UI when any pixel of
// it differs, so a hard test left a stepped, mist-free ring around the round orb tops. KNOWN-GOOD CHECK: normal town, wait for dusk: pale blue-grey banks with clear gaps
// fade in with the darkness, lie flat on the ground, move with the ground when walking, drift very
// slowly, freeze on pause, and are gone at dawn; the bottom panel stays clean.
#include "Mist.h"
static IDirect3DPixelShader9* MistPS[2] = { 0, 0 };
static const float MIST_ALPHA = 0.315f;					// opacity of the thickest mist at full night, GroundMist 5
static const float MIST_RGB[3] = { 0.42f, 0.46f, 0.55f };	// moonlit blue-grey

static bool Mist_Active()
{
	if( GroundMist <= 0 ) return false;
	if( Dungeon != DUN_0_TOWN || Dungeon->isQuest ) return false;
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) return false;
	if( Weather_Tonight() != WEATHER_MIST ) return false;	// (th4) mist only on mist nights, never with rain
	return DayNight_NightQ8() > 0;	// 0 in Classic and by day
}

static void Mist_Release( int slot )
{
	if( MistPS[slot] ){ MistPS[slot]->Release(); MistPS[slot] = 0; }
}

// Mist time in seconds, frozen while paused. GetTickCountTh() counts 1/MAX_FPS s (enums.h).
static float Mist_Clock()
{
	static long long last = 0;
	static double sec = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;	// first call or a stall of over a second
	if( !GamePaused && !IsPausedInMenu() ) sec += (double)d / MAX_FPS;
	if( sec > 100000.0 ) sec -= 100000.0;	// keep float precision in the shader
	return (float)sec;
}

// dst = back buffer (window size); maskOk = HeatHaze_BuildMask succeeded this frame.
static void Mist_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	const float t = Mist_Clock();	// runs every frame so the clock never jumps
	if( !Mist_Active() || !maskOk || !HeatTex[slot] ) return;
	const int night = DayNight_NightQ8();
	if( night <= 0 ) return;
	if( !MistPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_mist, &MistPS[slot] ) ){ MistPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return;

	// ground anchor: screen position (game px) of tile (0,0) -- Atmos_Anchor, THE VIEW applied (DYNAMIC ZOOM)
	float ax, ay; Atmos_Anchor( ax, ay );
	const float c[3][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), t },
		{ ax, ay, MIST_ALPHA * night / 256.0f * GroundMist / 5.0f, 0 },
		{ MIST_RGB[0], MIST_RGB[1], MIST_RGB[2], 0 },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( MistStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 2; ++st ){
		const DWORD f = st == 1 ? D3DTEXF_LINEAR : D3DTEXF_POINT;	// s1 = mask, linear: soft edge (Mist.hlsl)
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( MistPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, HeatTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next frame (MistStateBlock)
}

// (th4) STORM RAIN -- the rain of storm nights in the normal town (Weather.cpp rolls the nights;
// lightning and thunder live there too). One window-size shader pass, Rain.hlsl, right after the
// mist pass (mist and rain never share a night). WINDOW resolution on purpose: streaks about one
// screen pixel wide, semi-transparent and anti-aliased, where the 8-bit frame could only give
// opaque game pixels (2x2 blocks at the default zoom). It needs the world/UI mask (never over the
// UI), so PostFx_NeedMask and HeatHaze_BuildMask both ask Rain_Active().
// Strength = DayNight_NightQ8() / 256 (fades in at dusk, out at dawn); time = Weather_ClockSec()
// (stops on pause); flash = Weather_FlashQ8() (the rain lights up with a strike).
// KNOWN-GOOD CHECK: storm night: thin light streaks in three depths, leaning with the wind, small
// flat rings splashing on the ground; brighter for an instant with each lightning strike; the
// panels, orbs and cursor stay dry; pause freezes it.
#include "Rain.h"
static IDirect3DPixelShader9* RainPS[2] = { 0, 0 };
static const float RAIN_SLANT = 0.18f;	// screen x shift per px of fall (the wind)

// (th4) How heavy the visible rain is, 0..1, from the night strength (DayNight_NightQ8, 0..256).
// It is tied to the RAIN SOUND (SoundBuffer.cpp PlayAmbient: on above night 24, off below night 8,
// then a 6 s fade-out): full rain from RAIN_FULL_NIGHT up, thinning out below it and gone at
// RAIN_SOUND_OFF_NIGHT -- the moment the sound starts fading. A plain night/256 left the drops
// nearly invisible for the whole late dawn while the rain could still be heard. Keep
// RAIN_SOUND_OFF_NIGHT equal to the "off" threshold in PlayAmbient.
static const int RAIN_FULL_NIGHT      = 64;
static const int RAIN_SOUND_OFF_NIGHT = 8;
static float Rain_Strength()
{
	const int night = DayNight_NightQ8();
	if( night <= RAIN_SOUND_OFF_NIGHT ) return 0.0f;
	if( night >= RAIN_FULL_NIGHT ) return 1.0f;
	return (float)( night - RAIN_SOUND_OFF_NIGHT ) / ( RAIN_FULL_NIGHT - RAIN_SOUND_OFF_NIGHT );
}

static bool Rain_Active()
{
	return Weather_Tonight() == WEATHER_STORM && Rain_Strength() > 0.0f;
}

static void Rain_Release( int slot )
{
	if( RainPS[slot] ){ RainPS[slot]->Release(); RainPS[slot] = 0; }
}

// dst = back buffer (window size); maskOk = HeatHaze_BuildMask succeeded this frame.
static void Rain_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !Rain_Active() || !maskOk || !HeatTex[slot] ) return;
	if( !RainPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_rain, &RainPS[slot] ) ){ RainPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return;

	float ax, ay; Atmos_Anchor( ax, ay );	// (th4) the rain is fixed to the ground, not the screen (Rain.hlsl c2)
	const float c[3][4] = {
		{ (float)w, (float)h, Weather_ClockSec(), Rain_Strength() },
		{ Weather_FlashQ8() / 256.0f, h / 1080.0f, RAIN_SLANT, 0 },
		{ ax * w / ScreenWidth, ay * h / ScreenHeight, 0, 0 },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( RainStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 2; ++st ){
		const DWORD f = st == 1 ? D3DTEXF_LINEAR : D3DTEXF_POINT;	// s1 = mask, linear: soft edge (Rain.hlsl)
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( RainPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, HeatTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next frame (RainStateBlock)
}

// (th4) BIOGLOW -- the soft light of a bioluminescent night (Weather.cpp WEATHER_GLOW, decided by
// Fireflies.cpp GlowNight_Active). The glowing river (TownWater.cpp GlowRiver) and the ground
// lights (Fireflies.cpp, glow kind) are drawn in the 8-bit frame; each of their pixels also calls
// BioGlow_Add, which counts it into a quarter-resolution map (BioGlow_BeginFrame clears it at the
// start of DrawIsoLand). The river additionally marks each of its pixels, full resolution, with its
// texture brightness (BioGlow_SetWater, TownWater.cpp GlowRiver). Here both maps are uploaded (L8)
// and BioGlow.hlsl colours the marked water in TRUE COLOUR (a smooth blue ramp from its own
// texture, slow swells, sparkles -- the palette has too few blues for that), blurs the glow map
// into a soft blue-cyan halo and tints the night a deeper blue -- world pixels only (world/UI
// mask, so PostFx_NeedMask and HeatHaze_BuildMask include GlowNight_Active). Strength follows the
// night. The water map is in game pixels after the zoom and before Perspective_WarpWorld, so with
// Perspective on the colouring does not follow the warp.
// Tuning: BIOGLOW_GAIN (map count -> brightness), BIOGLOW_TINT, BIOGLOW_RGB; river colours in
// BioGlow.hlsl (deep / bright, swell and sparkle strength).
#include "BioGlow.h"
static IDirect3DPixelShader9* BioGlowPS[2] = { 0, 0 };
static IDirect3DStateBlock9* BioGlowStateBlock[2] = { 0, 0 };
static LPDIRECT3DTEXTURE9 BioGlowTex[2] = { 0, 0 }, BioGlowWaterTex[2] = { 0, 0 }, BioGlowGroundTex[2] = { 0, 0 };
static int BioGlowTexW[2], BioGlowTexH[2], BioGlowWaterW[2], BioGlowWaterH[2], BioGlowGroundW[2], BioGlowGroundH[2];
static std::vector<uchar> BioGlowGround;	// blood moon: 255 = town tile pixel (entry 1..127), game res
static std::vector<ushort> BioGlowMap;
static std::vector<uchar> BioGlowWater;		// full game resolution: 0 = not water, 1..255 = water brightness
static int* BioGlowWaterSet = 0;			// pixels set this frame: cleared one by one next frame, not a full memset
static size_t BioGlowSetN = 0, BioGlowSetCap = 0;	// (a plain buffer: a std::vector push_back per glowing pixel cost too much)
static void BioGlow_NoteSet( int i )
{
	if( BioGlowSetN == BioGlowSetCap ){
		const size_t cap = BioGlowSetCap ? BioGlowSetCap * 2 : 65536;
		int* p = (int*)realloc( BioGlowWaterSet, cap * sizeof( int ) );
		if( !p ) return;
		BioGlowWaterSet = p; BioGlowSetCap = cap;
	}
	BioGlowWaterSet[ BioGlowSetN++ ] = i;
}
static int BioGlowW = 0, BioGlowH = 0, BioGlowCount = 0;
static const int   BIOGLOW_GAIN = 6;		// map value = count x gain (a 4x4 block full of river = 16)
static const float BIOGLOW_TINT = 0.55f;	// how much the night turns deeper blue (0 = off)
static const float BIOGLOW_RGB[3] = { 0.15f, 0.45f, 1.0f };
// (th4) FIREFLY HALO: on CLEAR nights while fireflies fly (Fireflies.cpp Fireflies_Active), the same
// pass gives every lit firefly a soft yellow-green halo (Fireflies.cpp adds to the glow map). No
// night tint and no water colouring then: the water map stays empty and the tint is passed as 0.
static const float FIREFLY_HALO_RGB[3] = { 0.55f, 0.75f, 0.20f };
// (th4) BLOOD MOON (Weather.cpp WEATHER_BLOOD, Fireflies.cpp BloodNight_Active): the same pass with
// blood-moon colours -- the river (marked by TownWater.cpp GlowRiver) coloured crimson and much
// stronger than the glow nights' 8%, the tree leaves (TownWater.cpp GlowLeaves) glowing gold-orange-
// red, and a red halo around both. NO night tint: tinting the whole frame also turned heroes, NPCs
// and cows red.
static const float BLOOD_HALO_RGB[3]   = { 0.90f, 0.10f, 0.06f };
static const float BLOOD_LEAF_DEEP[3]  = { 0.55f, 0.10f, 0.02f };	// deep red
static const float BLOOD_LEAF_BRIGHT[3] = { 1.00f, 0.72f, 0.22f };	// gold
static const float BLOOD_LEAF_MIX      = 0.50f;
// GROUND WARMTH: town tile pixels only (palette entries 1..127 of the frame -- never sprites or UI,
// see BioGlow.hlsl c10 / s4) are multiplied toward warm: red up a little, blue down.
static const float BLOOD_WARM_RGB[3]   = { 1.32f, 1.04f, 0.68f };
static const float BLOOD_WARM          = 1.00f;
static const float BLOOD_WATER_DEEP[3] = { 0.22f, 0.00f, 0.02f };
static const float BLOOD_WATER_BRIGHT[3] = { 0.85f, 0.10f, 0.08f };
static const float BLOOD_WATER_MIX     = 0.45f;
static const float BLOOD_SPARKLE[3]    = { 1.00f, 0.55f, 0.45f };
static const float BLOOD_WATER_CAP     = 1.00f;	// how far the crimson grows with the night (1 = all the way) (its halo: TownWater.cpp GlowRiver BLOOD_RIVER_HALO_PCT)
// glow nights' tint / water colours (were constants in BioGlow.hlsl)
static const float GLOW_TINT_RGB[3]    = { 0.72f, 0.86f, 1.25f };
static const float GLOW_WATER_DEEP[3]  = { 0.02f, 0.10f, 0.38f };
static const float GLOW_WATER_BRIGHT[3] = { 0.35f, 0.80f, 1.00f };
static const float GLOW_WATER_MIX      = 0.08f;
static const float GLOW_SPARKLE[3]     = { 0.85f, 0.97f, 1.00f };
static const float GLOW_LEAF_DEEP[3]   = { 0.50f, 0.05f, 0.25f };	// pink
static const float GLOW_LEAF_BRIGHT[3] = { 1.00f, 0.50f, 0.72f };
static const float GLOW_LEAF_MIX       = 0.50f;
// (th4) ICE TREES (Ice Age, town and Ravenholm -- TownWater.cpp IceTrees_Active): the same pass, by day
// and by night: the leaves (TownWater.cpp GlowLeaves) FROSTED on a ramp from shaded snow to white, and a
// faint lavender-white halo around them. The trees are SNOW-COVERED now (TownWater.cpp SNOW ON THE TREES
// paints the snow itself, in the 8-bit frame), so this pass only frosts what is under the snow: Mordor
// (2026-10-09) wanted them "not glowing (that much at least)" -- the halo was at the glow nights' full
// strength (ICE_HALO_STRENGTH 1) and the leaves 85% white (ICE_LEAF_MIX 0.85) before. The halo on the frozen RIVER
// stays at that full strength (c1.w below; cut with the trees, the river went dark). No night tint, no ground warmth.
static const float ICE_HALO_RGB[3]    = { 0.80f, 0.83f, 1.00f };	// lavender-white
static const float ICE_HALO_STRENGTH  = 0.2f;	// x the halo colour: a hint of glow, no more
static const float ICE_LEAF_DEEP[3]   = { 0.72f, 0.78f, 0.92f };	// shaded snow
static const float ICE_LEAF_BRIGHT[3] = { 1.00f, 1.00f, 1.00f };
static const float ICE_LEAF_MIX       = 0.35f;	// how much of a leaf becomes frost
// At NIGHT the white leaves and their halo dim: in town from full by day to ICE_NIGHT_DIM at full
// night (the same eased phase as the terrain, DayNight_NightQ8); Ravenholm is always night, so it
// always has ICE_NIGHT_DIM. Lower = dimmer nights.
static const float ICE_NIGHT_DIM      = 0.45f;
static bool BioGlow_Active(){ return GlowNight_Active() || Fireflies_Active() || BloodNight_Active() || IceTrees_Active(); }

void BioGlow_BeginFrame()
{
	BioGlowCount = 0;
	BioGlowW = ScreenWidth / 4; BioGlowH = ScreenHeight / 4;
	if( !BioGlow_Active() ){ BioGlowMap.clear(); BioGlowWater.clear(); BioGlowSetN = 0; return; }
	BioGlowMap.assign( (size_t)BioGlowW * BioGlowH, 0 );
	const size_t n = (size_t)ScreenWidth * ScreenHeight;
	if( BioGlowWater.size() != n ){ BioGlowWater.assign( n, 0 ); BioGlowSetN = 0; }
	uchar* const water = BioGlowWater.data();
	for( size_t k = 0; k < BioGlowSetN; ++k ) water[ BioGlowWaterSet[ k ] ] = 0;
	BioGlowSetN = 0;
}

void BioGlow_SetWater( int x, int y, int v )
{
	if( BioGlowWater.empty() || x < 0 || y < 0 || x >= ScreenWidth || y >= ScreenHeight ) return;
	const int i = y * ScreenWidth + x;
	uchar* const water = BioGlowWater.data();
	if( v && !water[ i ] ) BioGlow_NoteSet( i );
	water[ i ] = (uchar)v;
}

// (th4) TownWater.cpp GlowLeaves, once per leaf: EXACTLY its old z x z BioGlow_SetWater( rx + ox, ry + oy, code ) loop
// (clipped to the screen as it was) and then BioGlow_Add( rx, ry, amount ) -- the same writes in the same order, in one
// call instead of z * z + 1, each with its checked std::vector access (GlowLeaves cost ~0.5 .. 1 ms a frame among trees)
void BioGlow_AddLeaf( int rx, int ry, int z, int code, int amount )
{
	if( !BioGlowWater.empty() ){
		uchar* const water = BioGlowWater.data();
		for( int oy = 0; oy < z && ry + oy < ScreenHeight; ++oy )
			for( int ox = 0; ox < z && rx + ox < ScreenWidth; ++ox ){
				const int x = rx + ox, y = ry + oy;
				if( x < 0 || y < 0 ) continue;
				const int i = y * ScreenWidth + x;
				if( code && !water[ i ] ) BioGlow_NoteSet( i );
				water[ i ] = (uchar)code;
			}
	}
	if( BioGlowMap.empty() || rx < 0 || ry < 0 ) return;	// = BioGlow_Add( rx, ry, amount )
	const int bx = rx >> 2, by = ry >> 2;
	if( bx >= BioGlowW || by >= BioGlowH ) return;
	ushort* const v = BioGlowMap.data() + (size_t)by * BioGlowW + bx;
	*v = (ushort)min( 60000, *v + amount );
	++BioGlowCount;
}

void BioGlow_Add( int x, int y, int amount )
{
	if( BioGlowMap.empty() || x < 0 || y < 0 ) return;
	const int bx = x >> 2, by = y >> 2;
	if( bx >= BioGlowW || by >= BioGlowH ) return;
	ushort& v = BioGlowMap[ (size_t)by * BioGlowW + bx ];
	v = (ushort)min( 60000, v + amount );
	++BioGlowCount;
}

static void BioGlow_Release( int slot )
{
	if( BioGlowPS[slot] ){ BioGlowPS[slot]->Release(); BioGlowPS[slot] = 0; }
	if( BioGlowStateBlock[slot] ){ BioGlowStateBlock[slot]->Release(); BioGlowStateBlock[slot] = 0; }
	if( BioGlowTex[slot] ){ BioGlowTex[slot]->Release(); BioGlowTex[slot] = 0; }
	if( BioGlowWaterTex[slot] ){ BioGlowWaterTex[slot]->Release(); BioGlowWaterTex[slot] = 0; }
	if( BioGlowGroundTex[slot] ){ BioGlowGroundTex[slot]->Release(); BioGlowGroundTex[slot] = 0; }
	BioGlowTexW[slot] = BioGlowTexH[slot] = BioGlowWaterW[slot] = BioGlowWaterH[slot] = 0;
	BioGlowGroundW[slot] = BioGlowGroundH[slot] = 0;
}

// (th4) Upload one L8 map, (re)creating the texture when the size changes.
static bool BioGlow_Upload( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9& tex, int& tw, int& th, int w, int h, const uchar* src )
{
	if( !tex || tw != w || th != h ){
		if( tex ){ tex->Release(); tex = 0; }
		if( D3D_OK != dev->CreateTexture( w, h, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &tex, 0 ) ){ tex = 0; return false; }
		tw = w; th = h;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != tex->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
	for( int y = 0; y < h; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, src + (size_t)y * w, w );
	tex->UnlockRect( 0 );
	return true;
}

static void BioGlow_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !BioGlow_Active() || !maskOk || !HeatTex[slot] || BioGlowMap.empty() || BioGlowW < 4 || BioGlowH < 4 ) return;
	const bool ice = IceTrees_Active();	// (th4) ICE TREES, day and night (see ICE_*)
	const bool glowNight = !ice && GlowNight_Active();
	const bool bloodNight = !ice && !glowNight && BloodNight_Active();	// else: firefly night (FIREFLY HALO)
	if( BioGlowWater.size() != (size_t)ScreenWidth * ScreenHeight ) return;
	const int night = DayNight_NightQ8();
	if( night <= 0 && !ice ) return;
	static std::vector<uchar> glow8;
	glow8.resize( BioGlowMap.size() );
	{	// plain pointers: the same values, without a checked access per element
		const ushort* const m = BioGlowMap.data(); uchar* const g = glow8.data(); const size_t n = BioGlowMap.size();
		for( size_t i = 0; i < n; ++i ) g[ i ] = (uchar)min( 255, m[ i ] * BIOGLOW_GAIN );
	}
	if( !BioGlow_Upload( dev, BioGlowTex[slot], BioGlowTexW[slot], BioGlowTexH[slot], BioGlowW, BioGlowH, glow8.data() ) ) return;
	if( !BioGlow_Upload( dev, BioGlowWaterTex[slot], BioGlowWaterW[slot], BioGlowWaterH[slot], ScreenWidth, ScreenHeight, BioGlowWater.data() ) ) return;
	// GROUND WARMTH map (blood moon only): the town's tile pixels in the finished 8-bit frame
	const bool warmGround = !ice && !GlowNight_Active() && BloodNight_Active() && SoftSrcSurfacePtr;
	if( warmGround ){
		BioGlowGround.resize( (size_t)ScreenWidth * ScreenHeight );
		// (th4) DYNAMIC ZOOM: from the WORLD copy -- the shader reads this map in the drawn frame (c11), where
		// the finished frame has the UI in other places than the screen shows it
		const uchar* src = ViewDyn && DynZoom_WorldRows() ? DynZoom_WorldRows() : SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		// (th4) 16 pixels at a time (2026-10-12, ~0.8 ms a frame one by one): an entry 1..127 is exactly a positive signed
		// byte, and the SSE2 compare gives 0xFF for it -- the same 255 / 0 bytes as the per-pixel test (all 256 values agree)
		const __m128i zero = _mm_setzero_si128();
		uchar* const ground = BioGlowGround.data();
		for( int y = 0; y < ScreenHeight; ++y ){
			const uchar* r = src + (size_t)y * WorkingWidth;
			uchar* d = ground + (size_t)y * ScreenWidth;
			int x = 0;
			for( ; x + 16 <= ScreenWidth; x += 16 ) _mm_storeu_si128( (__m128i*)( d + x ), _mm_cmpgt_epi8( _mm_loadu_si128( (const __m128i*)( r + x ) ), zero ) );
			for( ; x < ScreenWidth; ++x ) d[ x ] = ( r[ x ] >= 1 && r[ x ] <= 127 ) ? 255 : 0;
		}
		if( !BioGlow_Upload( dev, BioGlowGroundTex[slot], BioGlowGroundW[slot], BioGlowGroundH[slot], ScreenWidth, ScreenHeight, BioGlowGround.data() ) ) return;
	}
	if( !BioGlowPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_bioglow, &BioGlowPS[slot] ) ){ BioGlowPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return;
	// world anchor for the river's swells and sparkles: screen position (game px) of tile (0,0) --
	// Atmos_Anchor, THE VIEW applied (DYNAMIC ZOOM)
	float ax, ay; Atmos_Anchor( ax, ay );
	const float z = View_Z();
	const float* halo    = ice ? ICE_HALO_RGB : glowNight ? BIOGLOW_RGB : bloodNight ? BLOOD_HALO_RGB : FIREFLY_HALO_RGB;
	const float* tintRgb = GLOW_TINT_RGB;	// only glow nights tint (amount 0 on the others)
	const float* wDeep   = bloodNight ? BLOOD_WATER_DEEP : GLOW_WATER_DEEP;
	const float* wBright = bloodNight ? BLOOD_WATER_BRIGHT : GLOW_WATER_BRIGHT;
	const float  wMix    = bloodNight ? BLOOD_WATER_MIX : GLOW_WATER_MIX;
	const float* spark   = bloodNight ? BLOOD_SPARKLE : GLOW_SPARKLE;
	const float* lDeep   = ice ? ICE_LEAF_DEEP : bloodNight ? BLOOD_LEAF_DEEP : GLOW_LEAF_DEEP;
	const float* lBright = ice ? ICE_LEAF_BRIGHT : bloodNight ? BLOOD_LEAF_BRIGHT : GLOW_LEAF_BRIGHT;
	// ICE TREES dim at night (ICE_NIGHT_DIM): one factor for the halo and both leaf colours
	const float iceNight = Dungeon == DUN_0_TOWN ? night / 256.0f : 1.0f;
	const float iceK = ice ? 1.0f - ( 1.0f - ICE_NIGHT_DIM ) * iceNight : 1.0f;
	const float haloK = ice ? iceK * ICE_HALO_STRENGTH : 1.0f;	// (th4) ICE TREES: the faint halo (ICE_HALO_STRENGTH)
	const float  lMix    = ice ? ICE_LEAF_MIX : bloodNight ? BLOOD_LEAF_MIX : GLOW_LEAF_MIX;
	const float c[12][4] = {
		{ ice ? 1.0f : night / 256.0f, glowNight ? BIOGLOW_TINT : 0.0f, 1.0f / BioGlowW, 1.0f / BioGlowH },
		// w: halo on the water itself (BioGlow.hlsl c1). (th4) ICE TREES: the river keeps the FULL halo it had before the
		// trees' halo was cut to ICE_HALO_STRENGTH (v0.766): without it the frozen river went dark (Mordor) -- so w undoes
		// that factor on the water only
		{ halo[0] * haloK, halo[1] * haloK, halo[2] * haloK, bloodNight ? 0.0f : ice ? 1.0f / ICE_HALO_STRENGTH : 1.0f },
		{ ax, ay, z, Mist_Clock() },
		{ (float)ScreenWidth, (float)ScreenHeight, 0, 0 },
		{ tintRgb[0], tintRgb[1], tintRgb[2], 0 },
		{ wDeep[0], wDeep[1], wDeep[2], wMix },
		{ wBright[0], wBright[1], wBright[2], bloodNight ? BLOOD_WATER_CAP : 1.0f },
		{ spark[0], spark[1], spark[2], 0 },
		{ lDeep[0] * iceK, lDeep[1] * iceK, lDeep[2] * iceK, lMix },
		{ lBright[0] * iceK, lBright[1] * iceK, lBright[2] * iceK, 0 },
		{ BLOOD_WARM_RGB[0], BLOOD_WARM_RGB[1], BLOOD_WARM_RGB[2], warmGround ? BLOOD_WARM : 0.0f },
		// c11 (th4) DYNAMIC ZOOM: screen uv -> drawn-frame uv for the maps s1 / s3 / s4 (identity without it)
		{ 1.0f / ViewZ, -ViewSX / ( ViewZ * ScreenWidth ), -ViewSY / ( ViewZ * ScreenHeight ), 0 },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( BioGlowStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 5; ++st ){
		const DWORD f = ( st == 0 || st >= 3 ) ? D3DTEXF_POINT : D3DTEXF_LINEAR;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( BioGlowPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 12 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, BioGlowTex[slot] );
	dev->SetTexture( 2, HeatTex[slot] );
	dev->SetTexture( 3, BioGlowWaterTex[slot] );
	dev->SetTexture( 4, warmGround ? BioGlowGroundTex[slot] : 0 );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 ); dev->SetTexture( 3, 0 ); dev->SetTexture( 4, 0 );
	if( sb ){ sb->Apply(); }
}

// (th4) ICE FROST -- frost creeping in from the screen edges under the Ice Age game changer
// (config IceFrost 0..10, default 7 = on; 0 / off = never). While Ice Age is on: the town and every
// generated dungeon level, but NOT quest maps (Dungeon->isQuest) -- those keep their own look under
// Ice Age, they are not ice-themed. And the two frozen quest maps, Glacial Caverns (38) and Frozen
// Loch (39), get it in every mode, Ice Age or not (Frost_Wanted -- add a map there). It CREEPS IN while the hero stands
// still (CurAction == PCA_0_STAND) and MELTS while he does anything else -- walking, attacking,
// casting, blocking, getting hit: FROST_CREEP_SEC from nothing to its full reach, FROST_MELT_SEC back.
// The frost closes in as an OVAL (Frost.hlsl): the clear middle is a screen-shaped ellipse whose
// radius (normalised: 1 = touching the edge midpoints, sqrt(2) = the corners) shrinks from
// FROST_OVAL_START to the final radius FROST_OVAL_AT0 - FROST_OVAL_PER_STEP * IceFrost as the frost
// creeps in -- so it starts in the corners, spreads along the edges and closes in; a thin crust
// (FROST_CRUST) forms along every edge as well. IceFrost 10 leaves a clear oval 3/4 of the screen
// across, 5 frosts the corners and the edges, 1 only the corners. The creep is eased. Its clock stops while the game is paused
// (GamePaused / IsPausedInMenu, as Mist_Clock), so a paused game keeps its frost as it was. Purely
// local, nothing synced.
// THE PATTERN is real window frost: feathers grown from the edges, made ONCE on the CPU by
// FrostTex_Generate (below, no data file) into the FROST FEATHERS texture. That takes about a
// quarter of a second, so it runs on a background thread the first time frost is wanted
// (Frost_StartGenerate); the frost simply starts drawing when it is ready -- it creeps in from
// nothing anyway. Each texel also stores WHEN its feather reaches it, so as the frost creeps in it
// GROWS along the feathers (Frost.hlsl). FROST_TEX_DEPTH = how much of the window height the
// texture's depth covers -- enough for every point the smallest oval leaves frosted.
// WHICH PASS THIS IS: one window-size shader pass, Frost.hlsl, right after the mist / rain / glow
// passes and before the PostFx.hlsl effects. World pixels only, so it needs the world/UI mask:
// PostFx_NeedMask and HeatHaze_BuildMask both ask Frost_Active().
// KNOWN-GOOD CHECK: Ice Age hero, stand still: after a few seconds a thin crust of tiny crystals
// appears along all four edges, plumes first in the corners; over about a minute and a half
// feathery plumes grow in, curving, with fine combed barbs and a milky film between them, closing
// in as an oval (beside and above the bottom panel too);
// one step and it starts melting (gone in about ten seconds); the bottom panel, the orbs and the
// cursor are never frosted. IceFrost 10 leaves a clear oval about 3/4 of the screen across, IceFrost 1
// only frosts the corners. Ice Age on a quest map such as Ravenholm or the Infested Cellar: no
// frost. Without Ice Age nothing changes, except on Glacial Caverns and Frozen Loch.
#include "Frost.h"
#include <thread>
#include <atomic>
static IDirect3DPixelShader9* FrostPS[2] = { 0, 0 };
static const float FROST_CREEP_SEC      = 100.0f;	// standing still: nothing -> full reach
static const float FROST_MELT_SEC       = 10.0f;	// moving / fighting: full reach -> nothing
static const float FROST_OVAL_START     = 1.45f;	// oval radius with no frost (> sqrt(2): nothing on screen)
static const float FROST_OVAL_AT0       = 1.30f;	// final radius = AT0 - PER_STEP * IceFrost ...
static const float FROST_OVAL_PER_STEP  = 0.055f;	// ... 10 -> 0.75, 5 -> 1.025, 1 -> 1.245
static const float FROST_CRUST          = 0.06f;	// thin crust along every edge, share of the texture depth
static const float FROST_TEX_DEPTH      = 0.40f;	// window height covered by the texture's depth (> max reach)
static const float FROST_OPACITY        = 0.95f;	// how white the ice gets where it is thickest
static float FrostAmount = 0.0f;	// 0..1: how far the frost has crept, as a share of its full reach
static float FrostSec    = 0.0f;	// frost clock in seconds, stops on pause (Frost.hlsl glints)

// (th4) FROST FEATHERS -- the frost pattern, generated ONCE on the CPU (no data file), then kept as
// a texture. Real window frost is dendritic: plumes ("feathers"), each a gently curving spine with
// dense, fine, parallel barbs angled toward its tip, growing in from the edge of the pane and
// overlapping. The texture is in EDGE SPACE: x runs along a screen edge (wraps, so it tiles), y is
// the depth in from the edge (row 0 = the edge). Frost.hlsl lays it along the left, right and top
// edges. Channels: R = how bright the ice line is there (0 = clear), G = GROWTH TIME, the distance
// along its feather from the edge as a share of FROST_TEX_H (255 = never frosts), B = the fine
// frosty film between the feathers, A = the film's growth time. Because every pixel knows when its feather reaches it, the
// shader can let the frost really GROW along the feathers as it creeps in. Deterministic (own RNG):
// the same pattern every run, every machine.
static const int FROST_TEX_W = 2048;	// along the edge (wraps)
static const int FROST_TEX_H = 512;		// in from the edge

struct FrostRng {
	unsigned s;
	float f(){ s = s * 1664525u + 1013904223u; return ( s >> 8 ) * ( 1.0f / 16777216.0f ); }
	float r( float a, float b ){ return a + ( b - a ) * f(); }
};

struct FrostCanvas {
	float* ink;		// brightness, max-blended
	float* when;	// growth time 0..1, min-blended (1 = never)
	// anti-aliased point: spread over the 4 nearest pixels; x wraps, y clips
	void plot( float x, float y, float v, float t ){
		const int x0 = (int)floorf( x ), y0 = (int)floorf( y );
		const float fx = x - x0, fy = y - y0;
		const float w[4] = { ( 1 - fx ) * ( 1 - fy ), fx * ( 1 - fy ), ( 1 - fx ) * fy, fx * fy };
		for( int k = 0; k < 4; ++k ){
			const int yy = y0 + ( k >> 1 );
			if( yy < 0 || yy >= FROST_TEX_H ) continue;
			int xx = ( x0 + ( k & 1 ) ) % FROST_TEX_W; if( xx < 0 ) xx += FROST_TEX_W;
			const int i = yy * FROST_TEX_W + xx;
			const float a = v * w[k] * 1.6f;
			if( a > ink[i] ) ink[i] = a > 1.0f ? 1.0f : a;
			if( w[k] > 0.15f && t < when[i] ) when[i] = t;
		}
	}
};

// one barb: a short line leaving the spine at 'ang', curling back toward the spine direction 'spineAng'
static void FrostBarb( FrostCanvas& cv, float x, float y, float ang, float spineAng, float len, float v, float t0 )
{
	const float curl = ( spineAng - ang ) * 0.35f;	// barbs bend slightly toward the tip
	const int n = (int)( len * 2.0f ) + 1;
	for( int i = 1; i <= n; ++i ){
		const float s = (float)i / n;
		const float a = ang + curl * s;
		x += cosf( a ) * len / n; y += sinf( a ) * len / n;
		cv.plot( x, y, v * ( 1.0f - 0.55f * s ), t0 + s * len / FROST_TEX_H );
	}
}

// one feather: spine from (x,y) at angle 'ang' (radians, +y = into the screen), with barbs and,
// at depth 0, the odd side plume
static void FrostFeather( FrostCanvas& cv, FrostRng& rng, float x, float y, float ang, float len, int depth, float t0 )
{
	const float bend    = rng.r( -1.5f, 1.5f );			// total change of direction over the spine (more toward the tip)
	const float barbAng = rng.r( 0.55f, 0.95f );		// barbs leave the spine 31..54 degrees off
	const float barbMax = len * ( depth ? rng.r( 0.18f, 0.30f ) : rng.r( 0.12f, 0.24f ) );
	const float spacing = rng.r( 1.7f, 2.6f );			// px between barbs: the fine parallel combing
	const int   sides   = rng.f() < 0.6f ? 2 : ( rng.f() < 0.5f ? 1 : -1 );	// both, left only, right only
	const float vSpine  = depth ? 0.70f : 0.85f, vBarb = depth ? 0.55f : 0.65f;
	float arc = 0.0f, nextBarb = rng.r( 2.0f, 6.0f );
	const float step = 0.5f;
	for( float d = 0.0f; d < len; d += step ){
		const float s = d / len;
		const float a = ang + bend * s * sqrtf( s );
		x += cosf( a ) * step; y += sinf( a ) * step; arc += step;
		if( y < -4.0f || y > FROST_TEX_H + 4.0f ) break;
		const float t = t0 + arc / FROST_TEX_H;
		cv.plot( x, y, vSpine * ( 1.0f - 0.4f * s ), t );
		if( arc >= nextBarb ){
			nextBarb = arc + spacing * rng.r( 0.85f, 1.15f );
			// barbs are longest a third of the way up and taper to nothing at the tip
			const float rise = s < 0.12f ? s / 0.12f : 1.0f, fall = s > 0.55f ? 1.0f - 0.85f * ( s - 0.55f ) / 0.45f : 1.0f;
			const float taper = rise * fall;
			const float bl = barbMax * taper * rng.r( 0.7f, 1.0f );
			if( bl > 1.0f ){
				if( sides != -1 ) FrostBarb( cv, x, y, a - barbAng, a, bl, vBarb * rng.r( 0.75f, 1.0f ), t );
				if( sides !=  1 ) FrostBarb( cv, x, y, a + barbAng, a, bl, vBarb * rng.r( 0.75f, 1.0f ), t );
			}
		}
		if( depth == 0 && s > 0.15f && s < 0.7f && rng.f() < 0.004f )
			FrostFeather( cv, rng, x, y, a + ( rng.f() < 0.5f ? -1.0f : 1.0f ) * rng.r( 0.5f, 1.0f ), ( len - d ) * rng.r( 0.35f, 0.6f ), 1, t );
	}
}

// Fills rgba (FROST_TEX_W x FROST_TEX_H, 4 bytes per texel: R ink, G growth time, B film, A film growth time).
static void FrostTex_Generate( uchar* rgba )
{
	const int N = FROST_TEX_W * FROST_TEX_H;
	std::vector<float> ink( N, 0.0f ), when( N, 1.0f );
	FrostCanvas cv = { ink.data(), when.data() };
	FrostRng rng = { 0x1CEF0057u };
	// big plumes reaching deep in
	for( int i = 0; i < 70; ++i )
		FrostFeather( cv, rng, rng.r( 0.0f, (float)FROST_TEX_W ), rng.r( -2.0f, 10.0f ), 1.5708f + rng.r( -0.75f, 0.75f ),
		              FROST_TEX_H * rng.r( 0.40f, 0.95f ), 0, rng.r( 0.0f, 0.03f ) );
	// medium ones
	for( int i = 0; i < 160; ++i )
		FrostFeather( cv, rng, rng.r( 0.0f, (float)FROST_TEX_W ), rng.r( -2.0f, 40.0f ), 1.5708f + rng.r( -1.0f, 1.0f ),
		              FROST_TEX_H * rng.r( 0.15f, 0.45f ), 0, rng.r( 0.0f, 0.06f ) );
	// a dense crust of small crystals along the edge
	for( int i = 0; i < 700; ++i )
		FrostFeather( cv, rng, rng.r( 0.0f, (float)FROST_TEX_W ), rng.r( -2.0f, 30.0f ), 1.5708f + rng.r( -1.3f, 1.3f ),
		              rng.r( 8.0f, 45.0f ), 1, rng.r( 0.0f, 0.04f ) );
	// FILM between the feathers: a soft density (box blur of the ink, radius FILM_R, x wraps) and the
	// earliest growth time around each texel (min filter, same radius), so the film fills in only
	// among feathers that have already grown there, and never ends in a straight line.
	const int FILM_R = 6;
	std::vector<float> tmp( N ), dens( N ), tmin( N ), tmin2( N );
	for( int y = 0; y < FROST_TEX_H; ++y ){
		for( int x = 0; x < FROST_TEX_W; ++x ){
			float sum = 0.0f, mn = 1.0f;
			for( int k = -FILM_R; k <= FILM_R; ++k ){
				const int xx = ( x + k + FROST_TEX_W ) % FROST_TEX_W, i = y * FROST_TEX_W + xx;
				sum += ink[i]; if( when[i] < mn ) mn = when[i];
			}
			tmp[ y * FROST_TEX_W + x ] = sum / ( 2 * FILM_R + 1 ); tmin2[ y * FROST_TEX_W + x ] = mn;
		}
	}
	for( int y = 0; y < FROST_TEX_H; ++y ){
		for( int x = 0; x < FROST_TEX_W; ++x ){
			float sum = 0.0f, mn = 1.0f; int cnt = 0;
			for( int k = -FILM_R; k <= FILM_R; ++k ){
				const int yy = y + k; if( yy < 0 || yy >= FROST_TEX_H ) continue;
				const int i = yy * FROST_TEX_W + x;
				sum += tmp[i]; ++cnt; if( tmin2[i] < mn ) mn = tmin2[i];
			}
			dens[ y * FROST_TEX_W + x ] = sum / cnt; tmin[ y * FROST_TEX_W + x ] = mn;
		}
	}
	for( int i = 0; i < N; ++i ){
		FrostRng g = { (unsigned)i * 2654435761u };
		const float grain = g.f();
		float film = dens[i] * 2.2f; if( film > 1.0f ) film = 1.0f;
		film *= 0.35f + 0.65f * grain;
		rgba[ i * 4 + 0 ] = (uchar)( ink[i] * 255.0f + 0.5f );
		rgba[ i * 4 + 1 ] = (uchar)( when[i] * 255.0f + 0.5f );
		rgba[ i * 4 + 2 ] = (uchar)( film * 255.0f + 0.5f );
		rgba[ i * 4 + 3 ] = (uchar)( tmin[i] * 255.0f + 0.5f );
	}
}

// FROST FEATHERS texture state. FrostGenState: 0 = not started, 1 = generating (background thread),
// 2 = FrostTexBGRA is ready. The texture itself is per device slot, MANAGED (it survives a device
// reset), uploaded from FrostTexBGRA on first use and released with the shader in Frost_Release.
static std::atomic<int>     FrostGenState{ 0 };
static std::vector<uchar>   FrostTexBGRA;		// D3DFMT_A8R8G8B8 bytes: B film, G growth time, R ink, A film time
static LPDIRECT3DTEXTURE9   FrostTex[2] = { 0, 0 };

static void Frost_StartGenerate()
{
	int expected = 0;
	if( !FrostGenState.compare_exchange_strong( expected, 1 ) ) return;	// already running or done
	std::thread( []{
		std::vector<uchar> rgba( FROST_TEX_W * FROST_TEX_H * 4 );
		FrostTex_Generate( rgba.data() );
		for( size_t i = 0; i < rgba.size(); i += 4 ) std::swap( rgba[ i ], rgba[ i + 2 ] );	// RGBA -> BGRA
		FrostTexBGRA.swap( rgba );
		FrostGenState.store( 2, std::memory_order_release );
	} ).detach();
}

static bool Frost_EnsureTex( LPDIRECT3DDEVICE9 dev, int slot )
{
	if( FrostTex[slot] ) return true;
	if( FrostGenState.load( std::memory_order_acquire ) != 2 ) return false;
	if( D3D_OK != dev->CreateTexture( FROST_TEX_W, FROST_TEX_H, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &FrostTex[slot], 0 ) ){ FrostTex[slot] = 0; return false; }
	D3DLOCKED_RECT lr;
	if( D3D_OK != FrostTex[slot]->LockRect( 0, &lr, 0, 0 ) ){ FrostTex[slot]->Release(); FrostTex[slot] = 0; return false; }
	for( int y = 0; y < FROST_TEX_H; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, &FrostTexBGRA[ (size_t)y * FROST_TEX_W * 4 ], FROST_TEX_W * 4 );
	FrostTex[slot]->UnlockRect( 0 );
	return true;
}

static bool Frost_Wanted()
{
	if( IceFrost <= 0 || !gbRunGame ) return false;
	if( is( (DUNGEON)Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH ) ) return true;	// frozen maps, any mode
	return ( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) && !Dungeon->isQuest;	// Ice Age, not on quest maps
}

// true when the frost needs the world/UI mask this frame: it is on screen, or about to creep in
static bool Frost_Active()
{
	return Frost_Wanted() && ( FrostAmount > 0.0f || Players[ CurrentPlayerIndex ].CurAction == PCA_0_STAND );
}

static void Frost_Release( int slot )
{
	if( FrostPS[slot] ){ FrostPS[slot]->Release(); FrostPS[slot] = 0; }
	if( FrostTex[slot] ){ FrostTex[slot]->Release(); FrostTex[slot] = 0; }	// re-uploaded from FrostTexBGRA when needed
}

// Once per frame (Frost_Present): advance the creep / melt and the clock. GetTickCountTh() counts
// 1/MAX_FPS s (enums.h), as in Mist_Clock.
static void Frost_Update()
{
	static long long last = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;	// first call or a stall of over a second
	if( !Frost_Wanted() ){ FrostAmount = 0.0f; return; }
	if( GamePaused || IsPausedInMenu() ) return;
	const float dt = (float)d / MAX_FPS;
	FrostSec += dt;
	if( FrostSec > 100000.0f ) FrostSec -= 100000.0f;	// keep float precision in the shader
	if( Players[ CurrentPlayerIndex ].CurAction == PCA_0_STAND ) FrostAmount += dt / FROST_CREEP_SEC;
	else FrostAmount -= dt / FROST_MELT_SEC;
	if( FrostAmount < 0.0f ) FrostAmount = 0.0f; else if( FrostAmount > 1.0f ) FrostAmount = 1.0f;
}

// dst = back buffer (window size); maskOk = HeatHaze_BuildMask succeeded this frame.
static void Frost_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	Frost_Update();	// every frame, so the creep never jumps
	if( !Frost_Wanted() ) return;
	Frost_StartGenerate();	// no-op after the first call
	if( FrostAmount <= 0.0f || !maskOk || !HeatTex[slot] ) return;
	if( !Frost_EnsureTex( dev, slot ) ) return;	// still generating: nothing to draw yet
	if( !FrostPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_frost, &FrostPS[slot] ) ){ FrostPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return;
	const int strength = IceFrost > 10 ? 10 : IceFrost;
	const float e = FrostAmount * FrostAmount * ( 3.0f - 2.0f * FrostAmount );	// eased creep
	const float ovalEnd = FROST_OVAL_AT0 - FROST_OVAL_PER_STEP * strength;
	const float oval = FROST_OVAL_START + ( ovalEnd - FROST_OVAL_START ) * e;	// radius of the clear oval
	const float c[4][4] = {
		{ oval, FrostSec, FROST_TEX_DEPTH * h / FROST_TEX_H, FROST_OPACITY },
		{ 1.0f / w, 1.0f / h, (float)w, (float)h },
		{ 1.0f / FROST_TEX_W, 1.0f / FROST_TEX_H, (float)FROST_TEX_W, (float)FROST_TEX_H },
		{ FROST_CRUST * e, 0, 0, 0 },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( FrostStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 3; ++st ){
		// s0: the refraction offset is sub-pixel; s1 = mask, soft edge; s2 = feathers, smooth
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, st == 2 ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP );	// feathers tile along the edge
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( FrostPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 4 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, HeatTex[slot] );
	dev->SetTexture( 2, FrostTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next frame (FrostStateBlock)
}

// (th4) RETRO MODES (config RetroMode 0 = off, 1..19; RetroPixel 0 = the mode's own pixel size,
// 1..8 = forced). The WHOLE screen -- world, panels, text, cursor -- redrawn in the look of an old
// machine. Purely local, nothing synced. Called from PostFxPresent after the mist / rain / glow /
// frost passes and before the PostFx.hlsl pass, so CRT, film grain and the rest lay ON TOP of the
// retro image (RetroMode 9 + CRT = a green-screen terminal). In game only (gbRunGame); the meta
// menus are not touched. Surface mode (DrawMode 0) has no shaders and no retro modes.
// Two passes per frame:
//   1. Retro.hlsl (colour families) or RetroCell.hlsl (ZX / ASCII / Teletext; the same body, split
//      only for the ps_3_0 slot limit): the window-size frame copy (PostTex) -> RetroTex at LOW
//      resolution, one texel per retro pixel = B x B game pixels.
//   2. RetroUp.hlsl: RetroTex -> back buffer, "sharp bilinear" (crisp blocks, one blended seam).
// EVERY mode's numbers are in RetroModes[] below -- palette, family (kind), default block size,
// dither, gamma lift / gain (Diablo is dark: without the lift a 4-colour mode is mostly its
// darkest entry), PAL dither spread, halftone cell. The shader header (Retro.hlsl) explains the
// families. To add a mode: append a row here (RetroMode N = row N), then extend the RetroMode
// description in Config.cpp. RETRO_MODES follows the table. Row 19, VHS (kind 8), is not a palette look: it
// has its own one-pass shader (Vhs.hlsl, Vhs_Present below) and Retro_Present hands it over.
// KNOWN-GOOD CHECK: RetroMode 13 (pixelate) must show the normal picture in 4x4 blocks with the
// UI included; RetroMode 1 must show only four greens. If nothing changes at all, a shader failed
// to create (check the fxc output for Retro.hlsl / RetroCell.hlsl / RetroUp.hlsl).
// Resources are per device (slot 0 = D3dDevice, 1 = SecDevice), released in SharpScale_Release.
#include "Retro.h"
#include "RetroCell.h"
#include "RetroUp.h"
static IDirect3DPixelShader9* RetroPS[2] = { 0, 0 };
static IDirect3DPixelShader9* RetroCellPS[2] = { 0, 0 };
static IDirect3DPixelShader9* RetroUpPS[2] = { 0, 0 };
static IDirect3DPixelShader9* VhsPS[2] = { 0, 0 };		// (th4) RetroMode 19, VHS TAPE (Vhs_Present)
static LPDIRECT3DTEXTURE9 RetroTex[2] = { 0, 0 };		// low-res image, default pool render target
static int RetroTexW[2], RetroTexH[2];
static LPDIRECT3DTEXTURE9 RetroPalTex[2] = { 0, 0 };	// 64 x 1 palette of the current mode, managed
static LPDIRECT3DTEXTURE9 RetroGlyphTex[2] = { 0, 0 };	// 80 x 8 ASCII glyphs, managed
static const void* RetroPalFor[2] = { 0, 0 };			// which mode's palette RetroPalTex holds

// palettes, 0xRRGGBB, dark -> bright for the RAMP family
static const uint RetroPalGB[] = { 0x0F380F, 0x306230, 0x8BAC0F, 0x9BBC0F };
static const uint RetroPalGBP[] = { 0x1E1E1E, 0x5A5A5A, 0x9C9C9C, 0xD6D6CE };
static const uint RetroPalVB[] = { 0x000000, 0xB00000, 0xFF2A2A };
static const uint RetroPalCGA[] = { 0x000000, 0x55FFFF, 0xFF55FF, 0xFFFFFF };	// mode 4, palette 1, high intensity
static const uint RetroPalEGA[] = { 0x000000, 0x0000AA, 0x00AA00, 0x00AAAA, 0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
	0x555555, 0x5555FF, 0x55FF55, 0x55FFFF, 0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF };
static const uint RetroPalC64[] = { 0x000000, 0xFFFFFF, 0x68372B, 0x70A4B2, 0x6F3D86, 0x588D43, 0x352879, 0xB8C76F,
	0x6F4F25, 0x433900, 0x9A6759, 0x444444, 0x6C6C6C, 0x9AD284, 0x6C5EB5, 0x959595 };	// "Pepto"
static const uint RetroPalNES[] = { 0x000000,
	0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400, 0x503000, 0x007800, 0x006800, 0x005800, 0x004058,
	0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10, 0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888,
	0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044, 0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878,
	0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8, 0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8 };
static const uint RetroPalAmber[] = { 0x000000, 0x5A2A00, 0xC07000, 0xFFB000, 0xFFE0A0 };
static const uint RetroPalGreen[] = { 0x000000, 0x0A3A12, 0x1E9A3A, 0x33FF66, 0xC0FFC8 };
static const uint RetroPalSepia[] = { 0x1A120A, 0x5C4127, 0xA88A63, 0xE8D6B0, 0xFFF6E0 };
static const uint RetroPalMono[] = { 0x000000, 0xFFFFFF };
static const uint RetroPalPC98[] = { 0x000000, 0x221111, 0x442222, 0x663322, 0x884433, 0xAA6644, 0xCC9966, 0xFFDDAA,
	0x112244, 0x224466, 0x336688, 0x6699BB, 0x660000, 0xBB2200, 0x557733, 0xEEEEEE };	// a dark-fantasy 16 of 4096
// ZX: entries 0..7 normal, 8..15 BRIGHT -- the shader relies on this split (ink and paper share one half)
static const uint RetroPalZX[] = { 0x000000, 0x0000D7, 0xD70000, 0xD700D7, 0x00D700, 0x00D7D7, 0xD7D700, 0xD7D7D7,
	0x000000, 0x0000FF, 0xFF0000, 0xFF00FF, 0x00FF00, 0x00FFFF, 0xFFFF00, 0xFFFFFF };
// Teletext: entry 0 black, 1..7 the foreground colours (the shader searches 1..7)
static const uint RetroPalTTX[] = { 0x000000, 0xFF0000, 0x00FF00, 0xFFFF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF };

// ASCII glyphs, 8x8, bit 7 = left; ordered by ink coverage (Retro.hlsl picks glyph = brightness * 10)
static const uchar RetroGlyphs[10][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },						// ' '
	{ 0, 0, 0, 0, 0, 0x18, 0x18, 0 },				// '.'
	{ 0, 0, 0, 0x7E, 0, 0, 0, 0 },					// '-'
	{ 0, 0x18, 0x18, 0, 0, 0x18, 0x18, 0 },			// ':'
	{ 0, 0, 0x7E, 0, 0x7E, 0, 0, 0 },				// '='
	{ 0, 0x18, 0x18, 0x7E, 0x18, 0x18, 0, 0 },		// '+'
	{ 0, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0 },	// '%'
	{ 0, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0, 0 },		// '*'
	{ 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0 },	// '#'
	{ 0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0 },	// '@'
};

// kind: 0 RAMP, 1 PAL, 2 PIX, 3 QUANT, 4 HALF, 5 ZX, 6 ASCII, 7 TTX (Retro.hlsl header), 8 VHS (Vhs.hlsl --
// its own pass, Vhs_Present; the other numbers of its row are unused);
// block: default retro pixel size in game px; dither: 0 none / smooth, 1 Bayer 8x8, 2 Bayer 4x4, 3 noise;
// palN: palette entries (QUANT: levels per channel)
struct RetroModeDef { const char* name; int kind, block, dither; float gamma, gain, spread, cell; int palN; const uint* pal; };
#define RETRO_PAL( a ) (int)( sizeof( a ) / sizeof( a[0] ) ), a
static const RetroModeDef RetroModes[] = {
	{ "Game Boy",            0, 3, 1, 0.60f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalGB ) },	// 1
	{ "Game Boy Pocket",     0, 3, 1, 0.60f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalGBP ) },	// 2
	{ "Virtual Boy",         0, 2, 0, 0.70f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalVB ) },	// 3
	{ "CGA",                 1, 3, 0, 0.50f, 1.3f, 0.00f, 0, RETRO_PAL( RetroPalCGA ) },	// 4
	{ "EGA",                 1, 2, 0, 0.55f, 1.25f, 0.00f, 0, RETRO_PAL( RetroPalEGA ) },	// 5
	{ "C64",                 1, 3, 0, 0.55f, 1.25f, 0.00f, 0, RETRO_PAL( RetroPalC64 ) },	// 6
	{ "NES",                 1, 3, 0, 0.55f, 1.25f, 0.00f, 0, RETRO_PAL( RetroPalNES ) },	// 7
	{ "amber monitor",       0, 1, 0, 0.70f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalAmber ) },	// 8
	{ "green monitor",       0, 1, 0, 0.70f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalGreen ) },	// 9
	{ "sepia",               0, 1, 0, 0.85f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalSepia ) },	// 10
	{ "1-bit Mac",           0, 2, 3, 0.60f, 1.0f, 0.00f, 0, RETRO_PAL( RetroPalMono ) },	// 11
	{ "newspaper halftone",  4, 1, 0, 0.50f, 1.3f, 0.00f, 6, 0, 0 },						// 12
	{ "pixelate",            2, 4, 0, 1.00f, 1.0f, 0.00f, 0, 0, 0 },						// 13
	{ "PS1",                 3, 2, 2, 1.00f, 1.0f, 0.00f, 0, 32, 0 },						// 14 (32 levels = 15-bit colour)
	{ "PC-98",               1, 1, 1, 0.80f, 1.0f, 0.22f, 0, RETRO_PAL( RetroPalPC98 ) },	// 15
	{ "ZX Spectrum",         5, 2, 1, 0.55f, 1.3f, 0.00f, 0, RETRO_PAL( RetroPalZX ) },	// 16
	{ "ASCII",               6, 1, 0, 0.60f, 1.2f, 0.00f, 0, 0, 0 },						// 17
	{ "Teletext",            7, 4, 0, 0.60f, 1.2f, 0.00f, 0, RETRO_PAL( RetroPalTTX ) },	// 18
	{ "VHS tape",            8, 1, 0, 1.00f, 1.0f, 0.00f, 0, 0, 0 },						// 19 (Vhs_Present; not in Classic)
};
#undef RETRO_PAL
static const int RETRO_MODES = (int)( sizeof( RetroModes ) / sizeof( RetroModes[0] ) );

static void Retro_Release( int slot )
{
	IDirect3DPixelShader9** ps[] = { &RetroPS[slot], &RetroCellPS[slot], &RetroUpPS[slot], &VhsPS[slot] };
	for( auto p : ps ) if( *p ){ (*p)->Release(); *p = 0; }
	LPDIRECT3DTEXTURE9* tx[] = { &RetroTex[slot], &RetroPalTex[slot], &RetroGlyphTex[slot] };
	for( auto t : tx ) if( *t ){ (*t)->Release(); *t = 0; }
	RetroTexW[slot] = RetroTexH[slot] = 0;
	RetroPalFor[slot] = 0;
}

// palette of mode m in RetroPalTex (refilled when the mode changes) + the glyph texture
static bool Retro_EnsureLookups( LPDIRECT3DDEVICE9 dev, int slot, const RetroModeDef& m )
{
	D3DLOCKED_RECT lr;
	if( !RetroGlyphTex[slot] ){
		if( D3D_OK != dev->CreateTexture( 80, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &RetroGlyphTex[slot], 0 ) ){ RetroGlyphTex[slot] = 0; return false; }
		if( D3D_OK != RetroGlyphTex[slot]->LockRect( 0, &lr, 0, 0 ) ){ RetroGlyphTex[slot]->Release(); RetroGlyphTex[slot] = 0; return false; }
		for( int y = 0; y < 8; ++y ){
			uint* row = (uint*)( (uchar*)lr.pBits + y * lr.Pitch );
			for( int x = 0; x < 80; ++x ) row[x] = ( RetroGlyphs[ x / 8 ][ y ] & ( 0x80 >> ( x % 8 ) ) ) ? 0xFFFFFFFF : 0xFF000000;
		}
		RetroGlyphTex[slot]->UnlockRect( 0 );
	}
	if( !RetroPalTex[slot] ){
		if( D3D_OK != dev->CreateTexture( 64, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &RetroPalTex[slot], 0 ) ){ RetroPalTex[slot] = 0; return false; }
		RetroPalFor[slot] = 0;
	}
	if( RetroPalFor[slot] != &m ){
		if( D3D_OK != RetroPalTex[slot]->LockRect( 0, &lr, 0, 0 ) ) return false;
		uint* row = (uint*)lr.pBits;
		for( int i = 0; i < 64; ++i ) row[i] = 0xFF000000 | ( m.pal && i < m.palN ? m.pal[i] : 0 );
		RetroPalTex[slot]->UnlockRect( 0 );
		RetroPalFor[slot] = &m;
	}
	return true;
}

// (th4) VHS TAPE -- RetroMode 19 (the "VHS tape" row of RetroModes[], kind 8). Not a palette look like the
// other rows, so Retro_Present hands it to this pass instead of its two: Vhs.hlsl redraws the window-size frame
// copy as a worn tape on an old VCR (its header lists what it imitates and how). RetroPixel does not apply.
// Never in Classic (Mordor: Classic keeps the original look). Purely local, nothing synced.
// The clock is REAL time, not the pause-aware one: the tape runs on while the game is paused, and a paused game
// shows the PAUSE look of a VCR (GamePaused: the frame hops, two noise bars). TRACKING GLITCHES: real time is cut
// into VHS_WINDOW_SEC windows; in VHS_GLITCH_SHARE of them (a hash of the window number) a band of torn lines
// rolls through the picture once, up or down, for 1.5 .. 3 s at a hashed moment -- now and then, not all the time.
// KNOWN-GOOD CHECK: RetroMode 19, in a dungeon: soft picture with colour smearing to the right, a faint line
// wobble, torn lines at the very bottom, light snow; within a minute a tracking band rolls through; press pause:
// the picture hops and two noise bars appear. RetroMode 19 in Classic: no change at all.
#include "Vhs.h"
static const double VHS_WINDOW_SEC = 20.0;
static const int    VHS_GLITCH_SHARE_OF_4 = 3;	// windows (of every 4) that have a tracking glitch
static const float  VHS_BAND_HALF = 0.05f;		// half height of a tracking band, in picture heights

static void Vhs_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	if( GameMode == GM_CLASSIC ) return;
	if( !VhsPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_vhs, &VhsPS[slot] ) ){ VhsPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return;
	static DWORD start = 0;
	const DWORD now = GetTickCount();
	if( !start ) start = now;
	const double t = ( now - start ) / 1000.0;
	static unsigned frame = 0;
	frame = ( frame + 1 ) % 997;
	const bool paused = GamePaused != 0;
	float bandY = -9.0f, bandK = 0.0f;	// -9 = no band
	if( !paused ){
		const int win = (int)( t / VHS_WINDOW_SEC );
		uint hh = (uint)win * 2654435761u; hh ^= hh >> 15; hh *= 0x2c1b3c6du; hh ^= hh >> 12;
		const double inWin = t - win * VHS_WINDOW_SEC;
		const double begin = 2.0 + ( hh & 1023 ) / 1023.0 * ( VHS_WINDOW_SEC - 6.0 );
		const double len = 1.5 + ( ( hh >> 10 ) & 1023 ) / 1023.0 * 1.5;
		if( (int)( ( hh >> 20 ) & 3 ) < VHS_GLITCH_SHARE_OF_4 && inWin >= begin && inWin < begin + len ){
			const float ph = (float)( ( inWin - begin ) / len );
			bandY = ( hh >> 22 ) & 1 ? -0.1f + 1.2f * ph : 1.1f - 1.2f * ph;	// rolls down or up
			bandK = sinf( 3.14159f * ph ) * ( 0.55f + 0.45f * ( ( hh >> 23 ) & 255 ) / 255.0f );
		}
	}
	const float c[3][4] = {
		{ (float)w, (float)h, 1.0f / w, 1.0f / h },
		{ (float)fmod( t, 1000.0 ), (float)frame, paused ? 1.0f : 0.0f, (float)w / h },
		{ bandY, bandK, VHS_BAND_HALF, 0 },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( RetroStateBlock[slot], dev );	// shared: VHS replaces the retro passes
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// the taps fall between pixels
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE );
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( VhsPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 0, PostTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 );
	if( sb ){ sb->Apply(); }
	dev->SetRenderTarget( 0, dst );
}

// dst = back buffer (window size)
static void Retro_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	if( RetroMode <= 0 || RetroMode > RETRO_MODES || !gbRunGame ) return;
	const RetroModeDef& m = RetroModes[ RetroMode - 1 ];
	if( m.kind == 8 ){ Vhs_Present( dev, dst, slot ); return; }	// (th4) VHS tape: its own pass (above)
	const bool cell = m.kind >= 5;
	IDirect3DPixelShader9*& ps = cell ? RetroCellPS[slot] : RetroPS[slot];
	if( !ps && D3D_OK != dev->CreatePixelShader( (DWORD*)( cell ? g_ps30_main_retrocell : g_ps30_main_retro ), &ps ) ){ ps = 0; return; }
	if( !RetroUpPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_retroup, &RetroUpPS[slot] ) ){ RetroUpPS[slot] = 0; return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	int b = RetroPixel > 0 ? RetroPixel : m.block;
	if( b > 8 ) b = 8;
	const int lw = ( ScreenWidth + b - 1 ) / b, lh = ( ScreenHeight + b - 1 ) / b;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	if( !Fsr_EnsureTex( dev, RetroTex[slot], RetroTexW[slot], RetroTexH[slot], lw, lh, D3DFMT_X8R8G8B8 ) ) return;
	if( !Retro_EnsureLookups( dev, slot, m ) ) return;
	LPDIRECT3DSURFACE9 copySurf = 0, lowSurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return;
	const bool ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok || D3D_OK != RetroTex[slot]->GetSurfaceLevel( 0, &lowSurf ) ) return;
	IDirect3DStateBlock9* sb = CaptureStateBlock( RetroStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 3; ++st ){
		// s0 frame copy: linear (game px -> window px is fractional); s1 palette, s2 glyphs: exact texels
		const DWORD f = st == 0 ? D3DTEXF_LINEAR : D3DTEXF_POINT;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	// pass 1: frame copy -> low-res retro image (Retro.hlsl / RetroCell.hlsl constants c0..c2)
	const float c[3][4] = {
		{ (float)lw, (float)lh, (float)b, (float)m.kind },
		{ 1.0f / ScreenWidth, 1.0f / ScreenHeight, m.gamma, (float)m.dither },
		{ (float)m.palN, m.spread, m.cell, m.gain },
	};
	dev->SetRenderTarget( 0, lowSurf );
	dev->SetPixelShader( ps );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, RetroPalTex[slot] );
	dev->SetTexture( 2, RetroGlyphTex[slot] );
	Fsr_Quad( dev, lw, lh );
	dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 );
	// pass 2: low-res image -> window, sharp bilinear (RetroUp.hlsl constants c0..c1)
	const float u[2][4] = {
		{ (float)lw, (float)lh, (float)ScreenWidth / b, (float)ScreenHeight / b },
		{ (float)w * b / ScreenWidth, (float)h * b / ScreenHeight, 0, 0 },
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( RetroUpPS[slot] );
	dev->SetPixelShaderConstantF( 0, &u[0][0], 2 );
	dev->SetTexture( 0, RetroTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 );
	lowSurf->Release();
	if( sb ){ sb->Apply(); }	// kept for the next frame (RetroStateBlock)
	dev->SetRenderTarget( 0, dst );
}

// (th4) ATMOSPHERE EFFECTS -- optional window-size passes, purely local (nothing synced), all over
// WORLD pixels only (they read the world/UI mask, so Atmos_NeedMask() is asked by PostFx_NeedMask
// and by the skip test in HeatHaze_BuildMask). Each has its own shader whose header explains the
// look; the C++ here only decides WHEN and passes the numbers.
//   CloudShadows 0..10  CloudShadow.hlsl  DEFAULT ON (9). Normal town only (not Ravenholm or any other
//                                         quest map on the town tileset), never under Ice Age or in Classic,
//                                         only on days that are NOT sunny (Weather_SunQ8: those get SunRays), by DAY
//                                         only: full in daylight, gone by CLOUDS_NIGHT_OFF at dusk
//                                         (Clouds_Active / Clouds_Day). Ground-anchored. Its shader's other
//                                         mode is the SUNLIGHT of a sunny spell, drawn by SunRays_Present.
//   SpellTrails 0..10   Trails.hlsl +     DISABLED FOR NOW: its config row is commented out and
//                       TrailsShow.hlsl   SpellTrails stays 0 (Config.cpp), the code is kept for later.
//                                         Afterimages of bright pixels near live missiles, a ping-pong
//                                         buffer that moves with the ground (TrailTex).
//   TreeSway 0..10      TreeSway.hlsl     DEFAULT ON (4). Trees sway in the WIND (Wind_Strength: storm
//                                         nights, windy spells by day, always under Ice Age and in
//                                         Ravenholm; never in Classic). Only tree pixels move: TownWater.cpp BuildSwayLayers
//                                         says which pixel belongs to which tree and what is in front.
//   LevelUpGlow on/off  LevelUp.hlsl      golden ring + pillar for LEVELUP_SEC, started by
//                                         LevelUpFx_Start() (Player.cpp NextPlayerLevel, local hero)
//   WetGround 0..10     Wet.hlsl          DEFAULT ON (5). Normal town while it is wet (Weather.cpp
//                                         Weather_WetQ8: a storm night and the day after): wet ground,
//                                         puddles mirroring the sky, raindrop rings. Reads its own WET MAP
//                                         (Wet_BuildMask), not the world/UI mask. Puddles: TownWater.cpp.
//   ColdBreath 0..10    Breath.hlsl       DEFAULT ON (5). Breath vapour in the cold (Breath.cpp
//                                         Breath_Active: Ice Age, Glacial Caverns, Frozen Loch), and the
//                                         green toxic fumes of the fungi anywhere; the puffs come from
//                                         Breath.cpp Breath_Update.
//   (no config)         FlameGlow.hlsl    always on: a soft warm halo around the flames of torches, braziers,
//                                         candles and burning crosses (Flames.cpp finds them and says how
//                                         visible and bright; it also draws their sparks, in the 8-bit frame).
//   NpcSign on/off      NpcMark.hlsl      DEFAULT ON. The Diablo II-style "!" bubble over an NPC with something to
//                                         say (NpcMark.cpp). NOT world-masked: each mark has its own small cover
//                                         box, so the UI stays on top (NpcMark_Present, after the flame halos).
//   SunRays 0..10       SunRays.hlsl      DEFAULT ON (2). Shafts of sunlight falling through the tree crowns of
//                                         the normal town on SUNNY days (Weather.cpp Weather_SunQ8: half the
//                                         days, never the day after a storm, not Classic / Ice Age), during
//                                         one sunny spell of about half the day.
//                                         The crowns come from TownWater.cpp's TREE SWAY layer 1.
//   GrassBlades 0..10   Grass.hlsl        DEFAULT ON (10 = how tall). Blades of grass on the grass of the town and
//                                         Ravenholm (TownWater.cpp GRASS says where), swaying in the wind
//                                         (Wind_Weather); never in Classic or under the Ice Age (Grass_Active).
// (Falling leaves, halation and god rays were tried here and removed. The god rays were applied to no
// particular source, and nobody could tell where they were -- the SUN RAYS start in the tree crowns, so
// where they are is never in doubt.)
// Order in PostFxPresent: grass blades -> wet ground -> clouds (on the ground, under everything else) -> mist / rain /
// glow nights -> tree sway (after the glow pass, so recoloured crowns move whole) -> sun rays (over the
// swayed crowns) -> trails -> level-up -> flame halos -> breath -> NPC marks -> frost -> retro -> PostFx.hlsl.
// The clock (Atmos_Tick, once per PostFxPresent) stops on pause like Mist_Clock.
// The ground anchor (Atmos_Anchor) is the same tile (0,0) formula as PostFxPresent c9 / Mist_Present.
// KNOWN-GOOD CHECKS: town at noon: big soft shadows crawl over the ground and the houses, never over
// the panel; none at night, none under Ice Age, none in Ravenholm. SpellTrails (if re-enabled) 10:
// a fireball draws an orange streak behind it; a wall torch does NOT smear while you walk past it.
// LevelUpGlow on: a golden ring on the floor and a pillar of light at the moment of a level-up.
#include "CloudShadow.h"
#include "TreeSway.h"
#include "Trails.h"
#include "TrailsShow.h"
#include "LevelUp.h"
#include "Wet.h"
#include "Breath.h"
#include "FlameGlow.h"
#include "SunRays.h"
#include "NpcMark.h"
#include "Grass.h"
#include "GrassCompose.h"
#include "SnowGround.h"
#include "IceRiver.h"
static IDirect3DPixelShader9* CloudsPS[2] = { 0, 0 };
static IDirect3DPixelShader9* WetPS[2] = { 0, 0 };		// (th4) WET GROUND (below)
static LPDIRECT3DTEXTURE9 WetTex[2] = { 0, 0 };			// the WET MAP, game resolution, L8, DYNAMIC (default pool)
static int WetTexW[2], WetTexH[2];
static bool WetMapOk[2] = { false, false };				// Wet_BuildMask filled it this frame
static IDirect3DPixelShader9* BreathPS[2] = { 0, 0 };	// (th4) COLD BREATH (below)
static IDirect3DPixelShader9* FlameGlowPS[2] = { 0, 0 };	// (th4) FLAME HALOS (below)
static IDirect3DPixelShader9* SunRaysPS[2] = { 0, 0 };	// (th4) SUN RAYS (below)
static IDirect3DPixelShader9* NpcMarkPS[2] = { 0, 0 };	// (th4) NPC MARKS (below, NpcMark_Present)
static LPDIRECT3DTEXTURE9 NpcMarkTex[2] = { 0, 0 };		// their cover cells, L8, DYNAMIC (default pool), only grows
static IDirect3DPixelShader9* GrassPS[2] = { 0, 0 };	// (th4) GRASS (below, Grass_Present)
static IDirect3DPixelShader9* GrassComposePS[2] = { 0, 0 };	// its GrassResolution 1 / 2 compose pass (GrassCompose.hlsl)
static LPDIRECT3DTEXTURE9 GrassLowTex[2] = { 0, 0 };		// the grass layer at game / half game resolution (render target)
static int GrassLowW[2], GrassLowH[2];
static LPDIRECT3DTEXTURE9 GrassLayTex[2] = { 0, 0 }, GrassBlkTex[2] = { 0, 0 }, GrassCellTex[2] = { 0, 0 };	// its layer, block map, cell shares: L8, DYNAMIC
static LPDIRECT3DTEXTURE9 GrassIdxTex[2] = { 0, 0 }, GrassWorldTex[2] = { 0, 0 }, GrassPalTex[2] = { 0, 0 };	// floor indices, WORLD MAP (L8), palette (A8R8G8B8)
static int GrassLayW[2], GrassLayH[2], GrassBlkW[2], GrassBlkH[2], GrassCellW[2], GrassCellH[2], GrassIdxW[2], GrassIdxH[2], GrassWorldW[2], GrassWorldH[2];
static bool GrassWorldOk[2] = { false, false };			// Grass_BuildMask filled GrassWorldTex this frame
static LPDIRECT3DTEXTURE9 GrassGameTex[2] = { 0, 0 };	// the frame at GAME resolution, before the stretch to the window (Grass_Present)
static int GrassGameW[2], GrassGameH[2];
static uint GrassCellGen[2] = { 0, 0 };					// TownWater_GrassCells generation in GrassCellTex, 0 = none
static int NpcMarkTexW[2], NpcMarkTexH[2];
static IDirect3DPixelShader9* SnowGroundPS[2] = { 0, 0 };	// (th4) SNOW GROUND (below, SnowGround_Present)
static LPDIRECT3DTEXTURE9 SnowLayTex[2] = { 0, 0 }, SnowLutTex[2] = { 0, 0 };	// its ground layer (pre-zoom) and art-luminance table (256 x 1), L8
static int SnowLayW[2], SnowLayH[2], SnowLutW[2], SnowLutH[2];
static IDirect3DPixelShader9* IceRiverPS[2] = { 0, 0 };	// (th4) ICE RIVER (below, IceRiver_Present)
static LPDIRECT3DTEXTURE9 IceRiverTex[2] = { 0, 0 };	// its MAP, game resolution, L8, DYNAMIC (default pool)
static LPDIRECT3DTEXTURE9 IceNoiseTex[2] = { 0, 0 };	// its cloud noise, 256 x 256 L8, MANAGED (made once)
static int IceRiverTexW[2], IceRiverTexH[2];
static bool IceRiverMapOk[2] = { false, false };		// IceRiver_BuildMask filled IceRiverTex this frame
static LPDIRECT3DTEXTURE9 SunGroundTex[2] = { 0, 0 };	// its ground layer (TownWater.cpp SUN GROUND), pre-zoom, L8, DYNAMIC
static int SunGroundTexW[2], SunGroundTexH[2];
static uint SunGroundUploadedAt[2] = { 0, 0 };			// PostFxFrame of the last upload (the water part reuses it)
static uint PostFxFrame = 0;							// (th4) counts PostFxPresent calls: the sway layers upload once per frame
static uint SwayUploadedAt[2] = { 0, 0 };				// PostFxFrame of the last layer upload, 0 = none
static IDirect3DPixelShader9* TrailsPS[2] = { 0, 0 };
static IDirect3DPixelShader9* TrailsShowPS[2] = { 0, 0 };
static IDirect3DPixelShader9* LevelUpPS[2] = { 0, 0 };
static IDirect3DPixelShader9* TreeSwayPS[2] = { 0, 0 };
// TREE SWAY layers + palette: DYNAMIC textures, rewritten whole every frame with D3DLOCK_DISCARD, so the
// driver hands out a fresh buffer and the CPU never waits for the GPU to finish reading the old one.
// (They were MANAGED with partial locks first; windy town then ran at ~65 fps against ~400 calm, with
// the measured CPU work only ~2.5 ms -- a CPU/GPU wait on those locks was the suspected rest.)
// Default pool: released with the rest in Atmos_Release (SharpScale_Release, on reset).
static LPDIRECT3DTEXTURE9 SwayL1Tex[2] = { 0, 0 }, SwayL2Tex[2] = { 0, 0 }, SwayPalTex[2] = { 0, 0 };
static int SwayTexW[2], SwayTexH[2];
static LPDIRECT3DTEXTURE9 TrailTex[2][2] = { { 0, 0 }, { 0, 0 } };	// [slot][ping-pong], window size
static int TrailTexW[2][2], TrailTexH[2][2];
static int TrailCur[2] = { 0, 0 };				// which of the pair holds the current trails
static bool TrailValid[2] = { false, false };	// false = clear before use (new level, jump, resize)
static float TrailAnchorX[2], TrailAnchorY[2];	// ground anchor of the last update
static int TrailDungeon[2] = { -1, -1 };
static float TrailZoom[2] = { 0, 0 };	// View_Z() of the last update

static float AtmosSec = 0.0f, AtmosDt = 0.0f;	// pause-aware clock and this frame's step, seconds
static float LevelUpT0 = -1.0f;					// AtmosSec when the last level-up glow started, < 0 = none
static DUNGEON LevelUpDun;						// (th4) the level it started on: the glow FOLLOWS the hero there and
												// ends at once when he leaves it (portal, stairs, load -- Mordor)
static const float LEVELUP_SEC = 1.6f;

static void Atmos_Release( int slot )
{
	IDirect3DPixelShader9** ps[] = { &CloudsPS[slot], &TrailsPS[slot], &TrailsShowPS[slot], &LevelUpPS[slot], &TreeSwayPS[slot], &WetPS[slot], &BreathPS[slot], &FlameGlowPS[slot], &SunRaysPS[slot], &NpcMarkPS[slot], &GrassPS[slot], &GrassComposePS[slot], &SnowGroundPS[slot], &IceRiverPS[slot] };
	LPDIRECT3DTEXTURE9* sw[] = { &SwayL1Tex[slot], &SwayL2Tex[slot], &SwayPalTex[slot], &WetTex[slot], &SunGroundTex[slot], &NpcMarkTex[slot], &GrassLayTex[slot], &GrassBlkTex[slot], &GrassCellTex[slot], &GrassIdxTex[slot], &GrassWorldTex[slot], &GrassPalTex[slot], &GrassGameTex[slot], &GrassLowTex[slot], &SnowLayTex[slot], &SnowLutTex[slot], &IceRiverTex[slot], &IceNoiseTex[slot] };
	IceRiverTexW[slot] = IceRiverTexH[slot] = 0; IceRiverMapOk[slot] = false;
	GrassLayW[slot] = GrassLayH[slot] = GrassBlkW[slot] = GrassBlkH[slot] = GrassCellW[slot] = GrassCellH[slot] = 0; GrassCellGen[slot] = 0;
	GrassIdxW[slot] = GrassIdxH[slot] = GrassWorldW[slot] = GrassWorldH[slot] = 0; GrassWorldOk[slot] = false;
	GrassGameW[slot] = GrassGameH[slot] = 0;
	GrassLowW[slot] = GrassLowH[slot] = 0;
	SnowLayW[slot] = SnowLayH[slot] = SnowLutW[slot] = SnowLutH[slot] = 0;
	NpcMarkTexW[slot] = NpcMarkTexH[slot] = 0;
	SunGroundTexW[slot] = SunGroundTexH[slot] = 0; SunGroundUploadedAt[slot] = 0;
	for( auto t : sw ) if( *t ){ (*t)->Release(); *t = 0; }
	SwayTexW[slot] = SwayTexH[slot] = 0; SwayUploadedAt[slot] = 0;
	WetTexW[slot] = WetTexH[slot] = 0; WetMapOk[slot] = false;
	for( auto p : ps ) if( *p ){ (*p)->Release(); *p = 0; }
	for( int i = 0; i < 2; ++i ){
		if( TrailTex[slot][i] ){ TrailTex[slot][i]->Release(); TrailTex[slot][i] = 0; }
		TrailTexW[slot][i] = TrailTexH[slot][i] = 0;
	}
	TrailValid[slot] = false;
}

// Once per PostFxPresent: advance the clock unless paused. GetTickCountTh() counts 1/MAX_FPS s.
static void Atmos_Tick()
{
	static long long last = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;	// first call or a stall of over a second
	AtmosDt = ( GamePaused || IsPausedInMenu() ) ? 0.0f : (float)d / MAX_FPS;
	AtmosSec += AtmosDt;
	if( AtmosSec > 100000.0f ){ AtmosSec -= 100000.0f; LevelUpT0 = -1.0f; }	// keep float precision in the shaders
}

static bool Atmos_InTown() { return Dungeon == DUN_0_TOWN && !Dungeon->isQuest; }

// position (game px) of tile (0,0) in the DRAWN frame: the ground anchor, the same tile -> screen formula as
// the Alt item labels (automap.cpp). Alt+Z's 2x is in it (the CPU draws that); the wheel zoom is not.
static void Atmos_AnchorDrawn( float& x, float& y )
{
	const int zoom = IsZoomDisable ? 1 : 2;
	const int row = ( 0 - PlayerRowPos ) * zoom, col = ( 0 - PlayerColPos ) * zoom;
	x = (float)( 32 * ( row - col ) + ( 200 * zoom * ( Scroll.xOfs + PlayerShiftY ) / 100 >> 1 ) + Xofs - Screen_LeftBorder );
	y = (float)( 16 * ( row + col ) + ( 200 * zoom * ( Scroll.yOfs + PlayerShiftX ) / 100 >> 1 ) + Yofs - Screen_TopBorder );
}
// the ground anchor ON SCREEN (THE VIEW applied, DYNAMIC ZOOM): what the shaders measure world positions
// from -- world px = ( screen px - anchor ) / View_Z()
static void Atmos_Anchor( float& x, float& y )
{
	Atmos_AnchorDrawn( x, y );
	View_Point( x, y );
}

// screen position (game px) of a point on the map: tile + pixel offset (the Shockwave ring formula), then
// the Perspective warp and THE VIEW (DYNAMIC ZOOM)
static void Atmos_TileToScreen( int tileRow, int tileCol, int ox, int oy, float& fx, float& fy )
{
	const int zoom = IsZoomDisable ? 1 : 2;
	const int row = ( tileRow - PlayerRowPos ) * zoom, col = ( tileCol - PlayerColPos ) * zoom;
	int sx = 32 * ( row - col ) + ( 200 * zoom * ( Scroll.xOfs + PlayerShiftY ) / 100 >> 1 ) + Xofs - Screen_LeftBorder + ox * zoom;
	int sy = 16 * ( row + col ) + ( 200 * zoom * ( Scroll.yOfs + PlayerShiftX ) / 100 >> 1 ) + Yofs - Screen_TopBorder + oy * zoom;
	Perspective_WorldToScreen( sx, sy );	// follow the Perspective warp, if on
	fx = (float)sx; fy = (float)sy;
	View_Point( fx, fy );
}

// CLOUD SHADOWS are a DAYTIME thing: full strength in daylight, fading out over the start of dusk
// and gone from CLOUDS_NIGHT_OFF on (DayNight_NightQ8, palette.cpp: 0 = day .. 256 = full night;
// always 0 in Classic, which has no night) -- never at night, never in the dawn half either.
static const int CLOUDS_NIGHT_OFF = 64;
static float Clouds_Day()
{
	const int night = DayNight_NightQ8();
	return night >= CLOUDS_NIGHT_OFF ? 0.0f : 1.0f - (float)night / CLOUDS_NIGHT_OFF;
}
// Atmos_InTown() = DUN_0_TOWN and not a quest map, so Ravenholm (DUN_44, town tileset) never gets them.
// (th4) CLOUDY DAYS ONLY: a SUNNY day (Weather.cpp Weather_SunnyDay -- the day roll, half the days) has
// the SUN RAYS (during its sunny spell, Weather_SunQ8) and no cloud shadows ALL day, also before and after
// the spell; every other day has the clouds and no sun (Mordor's split). The test is the weather roll, NOT
// the SunRays option: the sky is the same for every player of a game, the options only say what is drawn.
static bool Clouds_Active()
{
	return CloudShadows > 0 && gbRunGame && Atmos_InTown() && Clouds_Day() > 0.0f
	    && GameMode != GM_CLASSIC	// never in Classic, whatever CloudShadows says (Mordor's call)
	    && !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) )	// never under Ice Age
	    && !Weather_SunnyDay();	// never on a sunny day (above)
}
static bool LevelUp_Active() { return LevelUpGlow && gbRunGame && LevelUpT0 >= 0.0f && AtmosSec - LevelUpT0 < LEVELUP_SEC && (DUNGEON)Dungeon == LevelUpDun; }
static bool Atmos_NeedMask()
{
	return Clouds_Active() || LevelUp_Active() || ( SpellTrails > 0 && gbRunGame ) || TreeSway_Active()
	    || Wet_Active()		// the WET MAP is built from the world snapshot
	    || Breath_Active()	// the puffs read the world/UI mask
	    || Flames_Halos( 0, 0, 1 ) > 0	// so do the flame halos (Flames.cpp, filled by Flames_Render before the snapshot)
	    || SunRays_Active()	// and the sun rays
	    || SnowGround_Active()	// and the snow on the ground (SNOW GROUND)
	    || IceRiver_Active()	// the ICE RIVER's map is built from the world snapshot
	    || Grass_Active()	// and the blades of grass (only over world pixels)
	    || Snowfall_GpuWanted()	// and the falling snow (SNOWFALL ON THE GPU)
	    || SnowDrift_Active();	// and the drifting snow of a storm (DRIFTING SNOW)
}

// (th4) Player.cpp NextPlayerLevel, local hero only: start the golden level-up glow. It is drawn where the
// hero is EVERY frame (LevelUp_Present), so it follows him while it fades; leaving the level ends it.
void LevelUpFx_Start()
{
	if( !LevelUpGlow ) return;
	LevelUpDun = Dungeon;
	LevelUpT0 = AtmosSec;
}

// Shared pass plumbing: copy the frame to PostTex (s0), capture the state block, set the states.
// linear0: s0 filtering (LINEAR where a pass samples between pixels). s1 = mask, always LINEAR.
static IDirect3DStateBlock9* Atmos_Begin( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, int& w, int& h, bool linear0, bool& ok )
{
	ok = false;
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return 0;
	w = (int)dd.Width; h = (int)dd.Height;
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return 0;
	LPDIRECT3DSURFACE9 copySurf = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &copySurf ) ) return 0;
	ok = D3D_OK == dev->StretchRect( dst, 0, copySurf, 0, D3DTEXF_NONE );
	copySurf->Release();
	if( !ok ) return 0;
	IDirect3DStateBlock9* sb = CaptureStateBlock( AtmosStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 3; ++st ){
		const DWORD f = ( st == 0 && !linear0 ) ? D3DTEXF_POINT : D3DTEXF_LINEAR;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, HeatTex[slot] );
	return sb;
}
static void Atmos_End( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, IDirect3DStateBlock9* sb )
{
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next pass (AtmosStateBlock)
	dev->SetRenderTarget( 0, dst );
}
static bool Atmos_Shader( LPDIRECT3DDEVICE9 dev, IDirect3DPixelShader9*& ps, const BYTE* code )
{
	if( !ps && D3D_OK != dev->CreatePixelShader( (DWORD*)code, &ps ) ) ps = 0;
	return ps != 0;
}

// (th4) WET GROUND (config WetGround 0..10, default 5): the normal town while it is wet -- Weather.cpp
// Weather_WetQ8 says how wet (a storm night, and the day after it while it dries). The PUDDLES are found
// and drawn in the 8-bit frame by TownWater.cpp (darker water, the heroes' reflections); this pass,
// Wet.hlsl, adds the true-colour look on top: wet ground darker and a little more saturated, puddles
// mirroring the sky (WET_SKY_DAY .. WET_SKY_NIGHT with the night, lit by lightning), raindrop rings in
// them while it rains. First ATMOSPHERE pass, so cloud shadows, mist and rain lie on top.
// THE WET MAP (Wet_BuildMask, game resolution, L8, sampled POINT -- exact codes, game-pixel edges like
// the art): from the world snapshot (HeatHaze_SnapshotWorld, taken after the world was drawn), the
// finished 8-bit frame and TownWater_PuddleRows():
//   0   dry: a UI pixel (it changed after the world was drawn), a sprite pixel (128..255), void (0)
//   64  wet ground: a terrain pixel (1..127) -- ground, walls, roofs and trees all get wet
//   128..255 a puddle (TownWater.cpp's code: rim .. deep); includes the reflections drawn into it
// It must be built BEFORE HeatHaze_BuildMask, which uses the snapshot up (PostFxPresent calls it first).
// The snapshot is only taken when PostFx_NeedMask -> Atmos_NeedMask asks for it, so that asks Wet_Active.
// KNOWN-GOOD CHECK: TownWater.cpp PUDDLES.
// The TINT of the sky the puddles mirror (Wet.hlsl takes its brightness from the place around the
// puddle -- an absolute sky colour made daytime puddles look like patches of snow).
static const float WET_SKY_DAY[3]   = { 0.88f, 0.94f, 1.00f };	// overcast grey-blue by day
static const float WET_SKY_NIGHT[3] = { 0.62f, 0.72f, 1.00f };	// deeper blue at night
static const float WET_REFLECT_DAY   = 1.6f;	// how bright the mirrored sky is, x the ground's brightness: by day ...
static const float WET_REFLECT_NIGHT = 0.7f;	// ... and at night (puddles darker than the ground)
static bool Wet_Active()
{
	return WetGround > 0 && gbRunGame && Weather_WetQ8() > 0;
}

static void Wet_BuildMask( LPDIRECT3DDEVICE9 dev, int slot )
{
	WetMapOk[slot] = false;
	if( !Wet_Active() || !HeatSnapValid || !HeatSnapRows || !SoftSrcSurfacePtr ) return;
	// (th4) DYNAMIC ZOOM (THE VIEW): the map is built in the DRAWN frame from the snapshot alone (no UI there),
	// into Wet1x, and carried onto the screen with the zoom's UI mask (DynZoom_RemapFull). Red screen: all
	// dry, as without the zoom (there the red frame differs from the snapshot everywhere).
	const bool dz = ViewDyn;
	if( dz && DynZoom_RedScreen() ) return;
	const int W = ScreenWidth, H = ScreenHeight;
	static std::vector<uchar> Wet1x;
	if( dz ) Wet1x.resize( (size_t)W * H );
	// (th4) DYNAMIC ZOOM, PERFORMANCE: only the box the zoomed view shows (DynZoom_RemapFull reads nothing else; the
	// wet map has no neighbourhood rule, so the box is exact as it is). Without the zoom: the whole screen.
	int bx0 = 0, by0 = 0, bx1 = W, by1 = H;
	if( dz && !DynZoom_ShownBox( bx0, by0, bx1, by1 ) ) return;
	if( !WetTex[slot] || WetTexW[slot] != W || WetTexH[slot] != H ){
		if( WetTex[slot] ){ WetTex[slot]->Release(); WetTex[slot] = 0; }
		if( D3D_OK != dev->CreateTexture( W, H, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &WetTex[slot], 0 ) ){ WetTex[slot] = 0; return; }
		WetTexW[slot] = W; WetTexH[slot] = H;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != WetTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
	const uchar* fin = SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	const uchar* pud = TownWater_PuddleRows();
	// 16 pixels at a time (SSE2, the HM_* helpers above); the scalar loop finishes each row and is the
	// reference: out = frame == snapshot ? ( puddle ? its code : 1..127 ? 64 : 0 ) : 0
	const __m128i zero = _mm_setzero_si128(), one = _mm_set1_epi8( 1 ), top = _mm_set1_epi8( 126 ), wet = _mm_set1_epi8( 64 );
	for( int y = by0; y < by1; ++y ){
		const uchar* s = HeatSnapRows + (size_t)y * HeatSnapPitch;
		const uchar* f = dz ? s : fin + (size_t)y * WorkingWidth;
		const uchar* p = pud ? pud + (size_t)y * WorkingWidth : 0;
		uchar* d = dz ? &Wet1x[ (size_t)y * W ] : (uchar*)lr.pBits + (size_t)y * lr.Pitch;
		int x = bx0;
		for( ; x + 16 <= bx1; x += 16 ){
			const __m128i fv = _mm_loadu_si128( (const __m128i*)( f + x ) );
			const __m128i same = _mm_cmpeq_epi8( fv, _mm_loadu_si128( (const __m128i*)( s + x ) ) );
			const __m128i t = _mm_sub_epi8( fv, one );
			__m128i o = _mm_and_si128( _mm_cmpeq_epi8( _mm_min_epu8( t, top ), t ), wet );	// fv in 1..127
			if( p ){
				const __m128i pv = _mm_loadu_si128( (const __m128i*)( p + x ) );
				o = HM_Sel( _mm_cmplt_epi8( pv, zero ), pv, o );	// 128..255: a puddle
			}
			_mm_storeu_si128( (__m128i*)( d + x ), _mm_and_si128( o, same ) );
		}
		for( ; x < bx1; ++x ){
			const uchar fv = f[ x ];
			uchar o = ( fv >= 1 && fv <= 127 ) ? 64 : 0;
			if( p && p[ x ] >= 128 ) o = p[ x ];
			d[ x ] = fv == s[ x ] ? o : 0;
		}
	}
	const bool ok = !dz || DynZoom_RemapFull( Wet1x.data(), W, (uchar*)lr.pBits, lr.Pitch, 0 );
	if( ok ) ItemOverlay_MarkUi( (uchar*)lr.pBits, lr.Pitch, W, H, false );	// (th4) THE ITEM ART IS UI TOO (ItemOverlay.cpp): 0 = dry, never wet a dragged item
	WetTex[slot]->UnlockRect( 0 );
	WetMapOk[slot] = ok;
}

static void Wet_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	if( !WetMapOk[slot] || !WetTex[slot] || !Wet_Active() ) return;
	if( !Atmos_Shader( dev, WetPS[slot], g_ps30_main_wet ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	dev->SetTexture( 1, WetTex[slot] );	// instead of the world/UI mask Atmos_Begin set
	dev->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );	// codes: exact texels
	dev->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	const float sN = ( WetGround > 10 ? 10 : WetGround ) * 0.1f;	// 5 = 0.5
	const float wet = Weather_WetQ8() / 256.0f;
	const float night = DayNight_NightQ8() / 256.0f;
	float sky[3];
	for( int i = 0; i < 3; ++i ) sky[i] = WET_SKY_DAY[i] + ( WET_SKY_NIGHT[i] - WET_SKY_DAY[i] ) * night;
	float ax, ay; Atmos_Anchor( ax, ay );
	const float c[4][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), Weather_ClockSec() },
		{ ax, ay, 0.30f * sN * wet, 0.30f * sN * wet },					// darkening, extra saturation (5 = 15% each when soaked)
		{ sky[0], sky[1], sky[2], 0.25f + 0.35f * sN },					// puddle opacity (5 = 0.425)
		{ Rain_Active() ? Rain_Strength() : 0.0f, Weather_FlashQ8() / 256.0f, WET_REFLECT_DAY + ( WET_REFLECT_NIGHT - WET_REFLECT_DAY ) * night, 0 },
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( WetPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 4 );
	Fsr_Quad( dev, w, h );
	Atmos_End( dev, dst, sb );
}

static void Clouds_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !Clouds_Active() || !maskOk || !HeatTex[slot] ) return;	// (the SUNLIGHT of a sunny day is this shader's mode 1, drawn by SunRays_Present)
	if( !Atmos_Shader( dev, CloudsPS[slot], g_ps30_main_clouds ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	const float day = Clouds_Day();
	float ax, ay; Atmos_Anchor( ax, ay );
	const float c[2][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), AtmosSec },
		{ ax, ay, ( CloudShadows > 10 ? 10 : CloudShadows ) * 0.05f * day, 0 },	// 10 = shade takes half the light; w 0 = THE MODE: clouds
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( CloudsPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 2 );
	Fsr_Quad( dev, w, h );
	Atmos_End( dev, dst, sb );
}

static void Trails_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( SpellTrails <= 0 || !gbRunGame || !maskOk || !HeatTex[slot] ) return;
	if( !Atmos_Shader( dev, TrailsPS[slot], g_ps30_main_trails ) || !Atmos_Shader( dev, TrailsShowPS[slot], g_ps30_main_trailsshow ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	for( int i = 0; i < 2; ++i ){
		if( TrailTexW[slot][i] != w || TrailTexH[slot][i] != h ) TrailValid[slot] = false;
		if( !Fsr_EnsureTex( dev, TrailTex[slot][i], TrailTexW[slot][i], TrailTexH[slot][i], w, h, D3DFMT_X8R8G8B8 ) ){ Atmos_End( dev, dst, sb ); return; }
	}
	const float zoom = View_Z();
	float ax, ay; Atmos_Anchor( ax, ay );
	const float dx = ax - TrailAnchorX[slot], dy = ay - TrailAnchorY[slot];
	// start clean on a new level, a zoom change (Alt+Z or the wheel), a teleport-sized jump or a resize
	if( !TrailValid[slot] || TrailDungeon[slot] != (int)(DUNGEON)Dungeon || fabsf( TrailZoom[slot] - zoom ) > 0.001f || dx * dx + dy * dy > 200.0f * 200.0f ){
		for( int i = 0; i < 2; ++i ){
			LPDIRECT3DSURFACE9 s = 0;
			if( D3D_OK == TrailTex[slot][i]->GetSurfaceLevel( 0, &s ) ){ dev->SetRenderTarget( 0, s ); dev->Clear( 0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0 ); s->Release(); }
		}
		TrailValid[slot] = true; TrailDungeon[slot] = (int)(DUNGEON)Dungeon; TrailZoom[slot] = zoom;
		TrailAnchorX[slot] = ax; TrailAnchorY[slot] = ay;
	}
	if( AtmosDt > 0.0f ){	// paused: the trails hold still
		// spots of the live missiles, game px -> uv; the spot sits a little above the ground point,
		// where a missile sprite is drawn
		float spots[24][4] = {};
		int n = 0;
		for( int i = 0; i < MissileAmount && n < 24; ++i ){
			const Missile& m = Missiles[ MissileIndexes[i] ];
			float sx, sy;
			Atmos_TileToScreen( m.Row, m.Col, m.ColDisplacement, m.RowDisplacement - 24, sx, sy );
			if( sx < -64 || sy < -64 || sx > ScreenWidth + 64 || sy > ScreenHeight + 64 ) continue;
			spots[n][0] = ( sx + 0.5f ) / ScreenWidth; spots[n][1] = ( sy + 0.5f ) / ScreenHeight; ++n;
		}
		const int lv = SpellTrails > 10 ? 10 : SpellTrails;
		const float halfLife = 0.06f + 0.03f * lv;	// seconds; 10 = 0.36 s
		const float c[2][4] = {
			{ dx / ScreenWidth, dy / ScreenHeight, powf( 0.5f, AtmosDt / halfLife ), 0.35f },
			{ 1.0f / ScreenWidth, 1.0f / ScreenHeight, (float)n, 40.0f * zoom },
		};
		LPDIRECT3DSURFACE9 next = 0;
		if( D3D_OK == TrailTex[slot][ TrailCur[slot] ^ 1 ]->GetSurfaceLevel( 0, &next ) ){
			dev->SetRenderTarget( 0, next );
			dev->SetPixelShader( TrailsPS[slot] );
			dev->SetPixelShaderConstantF( 0, &c[0][0], 2 );
			dev->SetPixelShaderConstantF( 2, &spots[0][0], 24 );
			dev->SetTexture( 2, TrailTex[slot][ TrailCur[slot] ] );
			Fsr_Quad( dev, w, h );
			next->Release();
			TrailCur[slot] ^= 1;
			TrailAnchorX[slot] = ax; TrailAnchorY[slot] = ay;
		}
	}
	dev->SetRenderTarget( 0, dst );
	dev->SetSamplerState( 2, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 2, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	dev->SetPixelShader( TrailsShowPS[slot] );
	dev->SetTexture( 2, TrailTex[slot][ TrailCur[slot] ] );
	Fsr_Quad( dev, w, h );
	Atmos_End( dev, dst, sb );
}

// (th4) TREE SWAY -- the wind. The trees themselves are moved in the 8-bit frame by TownWater.cpp
// (TREE SWAY there); this part only says how hard the wind blows and how far each crown leans.
// Wind_Weather() is 0..1: how hard it blows right now (Wind_Strength(): the same, but 0 while TreeSway is off;
// the GRASS reads Wind_Weather, so it sways with the trees even when the trees are switched off).
//   Ravenholm: always (1). Normal town under Ice Age: always, day and night (1).
//   Normal town otherwise: on STORM nights it follows the rain (Rain_Strength: in with the rain at
//   dusk, out with it at dawn); by DAY there are windy spells -- the clock is cut into
//   WIND_SPELL_SEC windows, WIND_SPELL_SHARE of them windy (a hash of the window number), with a
//   WIND_RAMP_SEC ease between them -- fading out over the first half of dusk. Calm otherwise.
//   Everywhere else (dungeons, other quest maps): 0. In Classic: always 0, so no tree sway there.
// The clock (TreeSway_Tick, once per drawn frame from TownWater_Render) stops on pause and works in
// every draw mode; it is local, not synced in multiplayer.
static const float WIND_SPELL_SEC   = 120.0f;
static const float WIND_SPELL_SHARE = 0.40f;
static const float WIND_RAMP_SEC    = 15.0f;
static float WindSec = 0.0f;
void TreeSway_Tick()
{
	static long long last = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;	// first call or a stall of over a second
	if( !GamePaused && !IsPausedInMenu() ) WindSec += (float)d / MAX_FPS;
	if( WindSec > 100000.0f ) WindSec -= 100000.0f;
}
static float Wind_DaySpells()
{
	const float t = WindSec / WIND_SPELL_SEC;
	const int i = (int)floorf( t );
	const float f = t - (float)i;
	auto windy = []( int k ) -> float {
		uint h = (uint)k * 2654435761u; h ^= h >> 15; h *= 0x2c1b3c6du; h ^= h >> 12;
		return ( h & 1023 ) < (uint)( 1024 * WIND_SPELL_SHARE ) ? 1.0f : 0.0f;
	};
	const float a = windy( i ), b = windy( i + 1 );
	const float r = WIND_RAMP_SEC / WIND_SPELL_SEC;	// the ease sits at the end of each window
	float k = f < 1.0f - r ? 0.0f : ( f - ( 1.0f - r ) ) / r;
	k = k * k * ( 3.0f - 2.0f * k );
	return a + ( b - a ) * k;
}
// (th4) Wind_Weather: how hard the wind blows, whatever TreeSway says -- the GRASS sways with it too (Grass_Present,
// its own option GrassBlades); Wind_Strength is the trees' (and the breath's and the sun rays') view of it: 0 while
// TreeSway is off.
static float Wind_Weather()
{
	if( !gbRunGame ) return 0.0f;
	if( GameMode == GM_CLASSIC ) return 0.0f;	// (th4) no wind, so no tree sway, in Classic whatever TreeSway says
	if( Dungeon == DUN_44_RAVENHOLM ) return 1.0f;
	if( !Atmos_InTown() ) return 0.0f;
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) return 1.0f;
	float w = Weather_Tonight() == WEATHER_STORM ? Rain_Strength() : 0.0f;
	const int night = DayNight_NightQ8();
	const float day = night >= 128 ? 0.0f : 1.0f - night / 128.0f;
	const float spell = Wind_DaySpells() * day;
	return spell > w ? spell : w;
}
static float Wind_Strength()
{
	return TreeSway > 0 ? Wind_Weather() : 0.0f;
}
// (th4) global (funcs.h): TownWater.cpp records the tree pixels only while this is true
bool TreeSway_Active() { return Wind_Strength() > 0.0f; }

// (th4) The TREE SWAY layers (TownWater.cpp BuildSwayLayers) onto the GPU: SwayL1Tex / SwayL2Tex. Shared by
// TreeSway_Present and SunRays_Present, uploaded at most once per PostFxPresent (both may want them the
// same frame: a windy, sunny day). The whole layers, every time (DISCARD leaves the texture undefined, so
// all of it is written). False = no usable layers this frame. l2 = 0: layer 1 only (a sunny day without
// wind -- TownWater.cpp SwayNeedK; TreeSway_Present does not run then).
static bool SwayLayers_Upload( LPDIRECT3DDEVICE9 dev, int slot, const uint* l1, const uchar* l2, int lw, int lh )
{
	if( SwayUploadedAt[slot] == PostFxFrame && SwayL1Tex[slot] && SwayTexW[slot] == lw && SwayTexH[slot] == lh ) return true;
	if( !SwayL1Tex[slot] || SwayTexW[slot] != lw || SwayTexH[slot] != lh ){
		LPDIRECT3DTEXTURE9* sw[] = { &SwayL1Tex[slot], &SwayL2Tex[slot] };
		for( auto t : sw ) if( *t ){ (*t)->Release(); *t = 0; }
		SwayTexW[slot] = SwayTexH[slot] = 0;
		if( D3D_OK != dev->CreateTexture( lw, lh, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SwayL1Tex[slot], 0 ) ){ SwayL1Tex[slot] = 0; return false; }
		if( D3D_OK != dev->CreateTexture( lw, lh, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &SwayL2Tex[slot], 0 ) ){ SwayL2Tex[slot] = 0; return false; }
		SwayTexW[slot] = lw; SwayTexH[slot] = lh;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != SwayL1Tex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
	for( int y = 0; y < lh; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, l1 + (size_t)y * lw, (size_t)lw * 4 );
	SwayL1Tex[slot]->UnlockRect( 0 );
	if( l2 ){
		if( D3D_OK != SwayL2Tex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
		for( int y = 0; y < lh; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, l2 + (size_t)y * lw, lw );
		SwayL2Tex[slot]->UnlockRect( 0 );
	}
	SwayUploadedAt[slot] = PostFxFrame;
	return true;
}

static void TreeSway_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	const float wind = Wind_Strength();
	const uint* l1 = 0; const uchar* l2 = 0; int lw = 0, lh = 0, box[4];
	if( wind <= 0.0f || !TownWater_SwayLayers( l1, l2, lw, lh, box ) || !maskOk || !HeatTex[slot] ) return;
	const int zoom = IsZoomDisable ? 1 : 2;
	if( lw != ( ScreenWidth + zoom - 1 ) / zoom || lh != ( ScreenHeight + zoom - 1 ) / zoom || !l1 || !l2 ) return;	// layers are pre-zoom size
	if( !Atmos_Shader( dev, TreeSwayPS[slot], g_ps30_main_treesway ) ) return;
	if( !SwayLayers_Upload( dev, slot, l1, l2, lw, lh ) ) return;
	(void)box;
	// the palette the 8-bit frame is drawn with right now (day / night changes it)
	if( !SwayPalTex[slot] && D3D_OK != dev->CreateTexture( 256, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SwayPalTex[slot], 0 ) ){ SwayPalTex[slot] = 0; return; }
	{
		D3DLOCKED_RECT lr;
		if( D3D_OK != SwayPalTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
		uint* p = (uint*)lr.pBits;
		for( int i = 0; i < 256; ++i ) p[i] = 0xFF000000u | (uint)ScreenPalette[i].peRed << 16 | (uint)ScreenPalette[i].peGreen << 8 | ScreenPalette[i].peBlue;
		SwayPalTex[slot]->UnlockRect( 0 );
	}
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, true, ok );
	if( !ok ) return;
	for( int st = 2; st <= 4; ++st ){	// layers and palette hold indices: exact texels
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	float ax, ay; Atmos_Anchor( ax, ay );
	const float c[3][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), WindSec },
		{ ax, ay, ( TreeSway > 10 ? 10 : TreeSway ) * 0.3f * wind, 0 },	// world px at full weight: 5 = 1.5, 10 = 3 (Mordor's chosen strength; TownWater.cpp SWAY_MARGIN covers it)
		{ (float)lw, (float)lh, ViewSX, ViewSY },	// layer size (pre-zoom); zw DYNAMIC ZOOM shift: game px = layer px * c0.z + c2.zw
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( TreeSwayPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 2, SwayL1Tex[slot] );
	dev->SetTexture( 3, SwayL2Tex[slot] );
	dev->SetTexture( 4, SwayPalTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 3, 0 ); dev->SetTexture( 4, 0 );
	Atmos_End( dev, dst, sb );
}

// (th4) SUN RAYS (config SunRays 0..10, default 2): shafts of sunlight through the tree crowns of the normal
// town on SUNNY days (Weather.cpp Weather_SunQ8: the day roll, faded with the dusk / dawn; it already
// excludes Classic, Ice Age and everything but the normal town). The crowns are TownWater.cpp's TREE SWAY
// layer 1, recorded on sunny days too (TownWater_CellBegin asks SunRays_Active). SunRays.hlsl's header has
// the look; the numbers that set it are here:
//   SUN_DIR     the way the light travels on screen, world px. The town art's baked shadows fall about
//               0.70 px LEFT and 0.15 .. 0.25 px UP per pixel of height (signpost and tree by the inn,
//               Mordor's screenshot; NOT the monster sprites, whose sun is steeper) -- a ray from a crown
//               to its shadow on the ground goes (-0.70, 1 - 0.2) per pixel of height: down-left, ~40
//               degrees from vertical. FIXED: the tiles' shadows are fixed too (a moving sun would need
//               dynamic shadows first -- Mordor, 2026-10-04).
//   SUN_LOOKS   the look, one named row per version Mordor compared (SUN_LOOK picks the live one; keep the
//               old rows, so any of them is one number away): ray length, the pattern across the light,
//               brightness per SunRays step, the canopy-gap pattern. Two patterns: BANDS (soft noise
//               stripes, Sunny_01) and LINES (many hair-thin rays, each with its own presence, brightness
//               and slight jitter, Sunny_02 -- "in reality there are more of them but really thin").
// The light lands on the GROUND only (TownWater.cpp SUN GROUND): never a roof, a wall, a fence or a sprite
// (river banks and the water itself: yes). First look (2026-10-04, Mordor): rays over the rooftops, too thick, too many -- hence the
// ground layer, thinner and sparser shafts, and not every crown lets light through (SunRays.hlsl).
// Balance (measured on a mock-up, flames/sunstats.py in that session): the first look lit ~8300 px of a
// test lawn; the next try lit ~1600 and showed NOTHING in game (TH4_sunrays.log proved the pass ran: the
// two sparsity filters multiplied each other down) -- these numbers light ~4300, about half the first.
// One quad over the crowns' box (TownWater_SwayLayers) widened by the rays' reach, not the whole window.
// TWO PARTS when the river ripples (WaterShimmer, PostFx.hlsl -- the last full-screen pass, which moves
// everything drawn on the water before it): `part` 1 = all ground but the water, here with the other
// atmosphere passes; `part` 2 = only the water, AFTER the PostFx.hlsl pass, so the light on the water stays
// still while the water moves (Mordor). No ripple: `part` 0, everything in the first place.
static const float SUN_DIR_X = -0.70f, SUN_DIR_Y = 0.80f;
struct SunLook {
	const char* name;
	float len;				// how far below / left of a crown its light still reaches, world px
	float strength;			// brightness per SunRays step at full sun
	int lines;				// 0 = BANDS, 1 = LINES
	float across;			// BANDS: stripe width / LINES: spacing between line slots, world px
	float shaftLo, shaftHi;	// BANDS: noise threshold of a stripe (smoothstep)
	float lineHalfW;		// LINES: half width of a line's solid core, world px (plus ~0.7 px soft edge)
	float linePresence;		// LINES: share of the line slots that hold a ray
	float gapLo, gapHi;		// CANOPY GAPS: where the leaves let light through (smoothstep of a world noise)
};
static const SunLook SUN_LOOKS[] = {
	//  name          len   strength lines across shaftLo shaftHi halfW presence gapLo gapHi
	{ "Sunny_01",    90.0f, 0.07f,   0,    5.0f,  0.52f,  0.76f,  0.0f,  0.0f,    0.32f, 0.52f },	// Mordor: "almost acceptable" at SunRays 2, but thick / solid
	{ "Sunny_02",    90.0f, 0.12f,   1,    2.6f,  0.0f,   0.0f,   0.30f, 0.70f,   0.20f, 0.40f },	// many hair-thin rays -- Mordor: "worse", back to Sunny_01
};
static const int SUN_LOOK = 0;	// which row of SUN_LOOKS is live (0 = Sunny_01)
// (th4) SUNLIGHT (Mordor: "the rays look nice but the rest of the terrain doesn't look sunlit"): while the rays shine
// the town looks sunlit -- the lit colours brighter, warmer and richer, the shadows painted into the art kept dark
// and a little cool (the formula: CloudShadow.hlsl main_clouds mode 1). World pixels only (heroes and NPCs too),
// never the panels. Strength = Weather_SunQ8, so it eases in and out with the spell (~15 s); not scaled by the
// SunRays value (default 2 keeps the rays faint; the light on the ground is these numbers).
// COST: it is drawn in the rays' own pass (SunRays_Present) -- the rays' frame copy serves both, so all it adds to a
// sunny frame is the pixels around the rays' box. (A pass of its own cost a second full-window copy and pass.)
static const float SUNLIGHT_GAIN       = 1.45f;							// lit colours x this ...
static const float SUNLIGHT_WARM[3]    = { 1.10f, 1.00f, 0.80f };		// ... in this colour (golden; its luminance is ~1)
static const float SUNLIGHT_SHADE      = 0.96f;							// baked shadows x this ...
static const float SUNLIGHT_COOL[3]    = { 0.94f, 0.98f, 1.06f };		// ... and a touch of blue
static const float SUNLIGHT_SHADOW_LUM = 0.065f;						// luminance 0..1: below = shadow,
static const float SUNLIGHT_LIT_LUM    = 0.12f;							// above = fully in the sun (town.pal: see CloudShadow.hlsl)
static const float SUNLIGHT_SATURATION = 1.15f;							// colour in the light, 1 = unchanged
// c8..c10 of CloudShadow.hlsl (mode 1) and SunRays.hlsl: the look above, worked out so the shaders need only a few
// instructions -- sunlit = c x c9 + lit x ( c x c8 + luminance x c10 ), lit = smoothstep of the luminance (c8.w, c9.w),
// then out = lerp( c, saturate( sunlit ), strength c10.w ). Strength s: 0 = no change (part 2, the water).
static void Sunlight_Constants( LPDIRECT3DDEVICE9 dev, float s )
{
	float c[3][4];
	for( int k = 0; k < 3; ++k ){
		const float A = SUNLIGHT_COOL[k] * SUNLIGHT_SHADE, B = SUNLIGHT_WARM[k] * SUNLIGHT_GAIN, S = SUNLIGHT_SATURATION;
		c[0][k] = S * B - A;			// the colour in the light, minus the shadow's
		c[1][k] = A;					// the shadow
		c[2][k] = ( 1.0f - S ) * B;		// the saturation: the luminance's share of the light
	}
	c[0][3] = SUNLIGHT_SHADOW_LUM;
	c[1][3] = 1.0f / ( SUNLIGHT_LIT_LUM - SUNLIGHT_SHADOW_LUM );
	c[2][3] = s;
	dev->SetPixelShaderConstantF( 8, &c[0][0], 3 );
}
bool SunRays_Active()
{
	return SunRays > 0 && gbRunGame && Atmos_InTown() && GameMode != GM_CLASSIC && Weather_SunQ8() > 0;
}

static void SunRays_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk, int part )
{
	if( !SunRays_Active() || !maskOk || !HeatTex[slot] ) return;
	// the RAYS need the tree layers; the SUNLIGHT (above) does not -- without them it lights the whole window alone
	const uint* l1 = 0; const uchar* l2 = 0; int lw = 0, lh = 0, box[4];
	const uchar* gl = 0; int gw = 0, gh = 0;
	const int zoom = IsZoomDisable ? 1 : 2;
	bool rays = TownWater_SwayLayers( l1, l2, lw, lh, box ) && TownWater_SunGround( gl, gw, gh ) && gl && l1
	         && lw == ( ScreenWidth + zoom - 1 ) / zoom && lh == ( ScreenHeight + zoom - 1 ) / zoom	// layers are pre-zoom size (layer 1 is all the rays read)
	         && gw == lw && gh == lh
	         && Atmos_Shader( dev, SunRaysPS[slot], g_ps30_main_sunrays ) && SwayLayers_Upload( dev, slot, l1, l2, lw, lh );
	if( rays && ( !SunGroundTex[slot] || SunGroundTexW[slot] != gw || SunGroundTexH[slot] != gh ) ){
		if( SunGroundTex[slot] ){ SunGroundTex[slot]->Release(); SunGroundTex[slot] = 0; }
		SunGroundTexW[slot] = SunGroundTexH[slot] = 0; SunGroundUploadedAt[slot] = 0;
		if( D3D_OK != dev->CreateTexture( gw, gh, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &SunGroundTex[slot], 0 ) ){ SunGroundTex[slot] = 0; rays = false; }
		else{ SunGroundTexW[slot] = gw; SunGroundTexH[slot] = gh; }
	}
	if( rays && SunGroundUploadedAt[slot] != PostFxFrame ){	// once per frame: the water part reuses it
		D3DLOCKED_RECT lr;
		if( D3D_OK != SunGroundTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) rays = false;
		else{
			for( int y = 0; y < gh; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, gl + (size_t)y * gw, gw );
			SunGroundTex[slot]->UnlockRect( 0 );
			SunGroundUploadedAt[slot] = PostFxFrame;
		}
	}
	const bool light = part != 2 && Atmos_Shader( dev, CloudsPS[slot], g_ps30_main_clouds );	// SUNLIGHT: with the first part (part 2 = the water only, lit already)
	if( !rays && !light ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	for( int st = 2; st <= 3; ++st ){	// layer 1 and the ground layer: exact texels
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	const float len = sqrtf( SUN_DIR_X * SUN_DIR_X + SUN_DIR_Y * SUN_DIR_Y );
	const float dx = SUN_DIR_X / len, dy = SUN_DIR_Y / len;
	float ax, ay; Atmos_Anchor( ax, ay );
	const float sun = Weather_SunQ8() / 256.0f;
	const SunLook& L = SUN_LOOKS[ SUN_LOOK ];
	const float vz = View_Z();	// game px on screen per layer px (Alt+Z or the wheel; THE VIEW)
	auto inv = []( float lo, float hi ){ return hi > lo ? 1.0f / ( hi - lo ) : 0.0f; };	// (the LINES row has no BANDS threshold)
	// THE PART as the ground codes it lights (SunRays.hlsl c3.z, c6.w): land 1, water 0.5 (TownWater.cpp SUN GROUND)
	const float partMid  = part == 2 ? 0.575f : part == 1 ? 1.375f : 1.2f;		// 2: water only (0.4 .. 0.75), 1: land only (0.75 .. 2),
	const float partHalf = part == 2 ? 0.175f : part == 1 ? 0.625f : 0.8f;		// 0: both (0.4 .. 2)
	const float c[13][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, vz, AtmosSec },
		{ ax, ay, ( SunRays > 10 ? 10 : SunRays ) * L.strength * sun, Wind_Strength() },	// xy unused by the shader (c11)
		{ (float)lw, (float)lh, dx, dy },
		{ L.len, L.across, partMid, (float)L.lines },
		{ L.shaftLo, L.shaftHi, L.gapLo, L.gapHi },
		{ L.lineHalfW, L.linePresence, 0, 0 },
		{ inv( L.shaftLo, L.shaftHi ), inv( L.gapLo, L.gapHi ), 1.0f / L.len, partHalf },	// reciprocals (SunRays.hlsl c6, c7: keep it inside 512 slots)
		{ 1.0f / vz, lw ? 1.0f / lw : 0.0f, lh ? 1.0f / lh : 0.0f, 1.0f / L.across },	// (no layers = no rays: lw 0)
		{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },	// c8..c10 SUNLIGHT: Sunlight_Constants below
		{ ScreenWidth / vz, ScreenHeight / vz, -ax / vz, -ay / vz },		// screen uv -> world px (was (game px - anchor) / zoom)
		{ ScreenWidth / vz, ScreenHeight / vz, -ViewSX / vz, -ViewSY / vz },	// screen uv -> layer px, DYNAMIC ZOOM shift (was game px / zoom + c5.zw)
	};
	// the rays' quad: the crowns' box (pre-zoom px) widened by the rays' reach down-left, on screen (layer px * vz +
	// the DYNAMIC ZOOM shift), in window px; none without the rays
	int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	if( rays ){
		const float reachX = dx * L.len, reachY = dy * L.len;
		const float bx0 = ( box[0] + ( reachX < 0 ? reachX : 0 ) - 2 ) * vz + ViewSX, bx1 = ( box[2] + ( reachX > 0 ? reachX : 0 ) + 2 ) * vz + ViewSX;
		const float by0 = ( box[1] + ( reachY < 0 ? reachY : 0 ) - 2 ) * vz + ViewSY, by1 = ( box[3] + ( reachY > 0 ? reachY : 0 ) + 2 ) * vz + ViewSY;
		const float kx = (float)w / ScreenWidth, ky = (float)h / ScreenHeight;
		x0 = (int)floorf( bx0 * kx ); x1 = (int)ceilf( bx1 * kx ); y0 = (int)floorf( by0 * ky ); y1 = (int)ceilf( by1 * ky );
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0; if( x1 > w ) x1 = w; if( y1 > h ) y1 = h;
		if( x0 >= x1 || y0 >= y1 ) x0 = x1 = y0 = y1 = 0;
	}
	struct V { float x, y, z, rhw, u, v; };
	auto rect = [&]( V* q, int& n, int ra, int rb, int rc, int rd ){	// [ra, rc) x [rb, rd) in window px, as 2 triangles
		if( ra >= rc || rb >= rd ) return;
		const float l = ra - 0.5f, rt = rc - 0.5f, t = rb - 0.5f, b = rd - 0.5f;
		const float u0 = (float)ra / w, u1 = (float)rc / w, v0 = (float)rb / h, v1 = (float)rd / h;
		const V k[6] = { { l, t, 0, 1, u0, v0 }, { rt, t, 0, 1, u1, v0 }, { l, b, 0, 1, u0, v1 },
		                 { rt, t, 0, 1, u1, v0 }, { rt, b, 0, 1, u1, v1 }, { l, b, 0, 1, u0, v1 } };
		for( const V& v : k ) q[ n++ ] = v;
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	Sunlight_Constants( dev, light ? sun : 0.0f );	// c8..c10 for both shaders; 0 = the rays change nothing else (part 2)
	if( light ){	// SUNLIGHT around the rays' box (the whole window without the rays): CloudShadow.hlsl mode 1
		const float mode[4] = { 0, 0, 0, 1 };		// its c1.w THE MODE
		dev->SetPixelShader( CloudsPS[slot] );
		dev->SetPixelShaderConstantF( 1, mode, 1 );
		V q[24]; int n = 0;
		rect( q, n, 0, 0, w, y0 ); rect( q, n, 0, y1, w, h ); rect( q, n, 0, y0, x0, y1 ); rect( q, n, x1, y0, w, y1 );
		if( n ) dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, n / 3, q, sizeof( V ) );
	}
	if( x0 < x1 ){	// the RAYS, and the sunlight inside their box (SunRays.hlsl)
		dev->SetPixelShader( SunRaysPS[slot] );
		dev->SetPixelShaderConstantF( 0, &c[0][0], 8 );
		dev->SetPixelShaderConstantF( 11, &c[11][0], 2 );
		dev->SetTexture( 2, SwayL1Tex[slot] );
		dev->SetTexture( 3, SunGroundTex[slot] );
		V q[6]; int n = 0;
		rect( q, n, x0, y0, x1, y1 );
		dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, q, sizeof( V ) );
		dev->SetTexture( 2, 0 ); dev->SetTexture( 3, 0 );
	}
	Atmos_End( dev, dst, sb );
}

// (th4) GRASS (config GrassBlades 0..10, default 10 = how tall): blades of grass growing out of the grass of the
// normal town and of Ravenholm, swaying in the wind. TownWater.cpp GRASS finds where they stand and where they
// may be seen (the layer, the block map, the grass share of each map cell); Grass.hlsl draws them -- its header
// has the look. The numbers that set it are here:
//   height     the tallest blade is GRASS_HEIGHT_BASE + GrassBlades world px (default 14 -- about ankle-high to a hero);
//              each blade is 40..100% of it, a little shorter where the grass is thin. Never above
//              GRASS_HEIGHT_MAX: TownWater.cpp's BLOCK map assumes a blade fits in one GRASS_BLOCK.
//   GRASS_SWAY how far the tip of the tallest blade bends in a full gust, as a share of its height; the wind is
//              Wind_Weather (the trees' wind, but not switched off with TreeSway). Calm: the blades stand still.
//   GRASS_SHADOW  the art's baked shadows go on up the blades (Grass.hlsl SHADOWS ON THE BLADES), along the town
//              art's light direction -- SUN_DIR of the SUN RAYS above, the one place that number lives.
// WHEN: Grass_Active -- the option on, a game running in the textured-quad paths (DrawMode 1, not DDraw: the
// shader passes only run there), the normal town or Ravenholm, NEVER in Classic (Mordor: Classic keeps the
// original look) and NEVER under the Ice Age (no grass under the snow).
// COST: one window-size pass over the grass's box only; the block map turns most non-grass pixels away with
// one look-up, and a pixel walks at most HMAX + 2 rows x 5 roots. The CPU half is a few memcpy per drawn cell
// and one SSE2 pass over the screen (TownWater.cpp BuildGrassLayer).
static const int   GRASS_HEIGHT_BASE = 4;
static const int   GRASS_HEIGHT_MAX  = 14;
static const float GRASS_SWAY        = 0.5f;
static const float GRASS_SHADOW      = 0.55f;	// a blade point in the art's baked shadow is drawn this bright (Grass.hlsl SHADOWS ON THE BLADES)
bool Grass_Active()
{
	return GrassBlades > 0 && gbRunGame && GameMode != GM_CLASSIC && !DDraw && DrawMode == 1
	    && !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) )
	    && ( Atmos_InTown() || Dungeon == DUN_44_RAVENHOLM );
}

// (th4) GRASS: one L8 DYNAMIC texture rewritten whole (DISCARD), sized w x h; src rows of srcPitch bytes. False =
// no texture. transpose: write src[ x ][ y ] (the 112 x 112 cell shares are [row][col], the texture's u is the row).
static bool Grass_Upload( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9& tex, int& tw, int& th, int w, int h, const uchar* src, int srcPitch, bool transpose = false )
{
	if( !tex || tw != w || th != h ){
		if( tex ){ tex->Release(); tex = 0; }
		tw = th = 0;
		if( D3D_OK != dev->CreateTexture( w, h, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &tex, 0 ) ){ tex = 0; return false; }
		tw = w; th = h;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != tex->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
	for( int y = 0; y < h; ++y ){
		uchar* d = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
		if( !transpose ) memcpy( d, src + (size_t)y * srcPitch, w );
		else for( int x = 0; x < w; ++x ) d[ x ] = src[ (size_t)x * srcPitch + y ];
	}
	tex->UnlockRect( 0 );
	return true;
}

// (th4) SNOW GROUND (Mordor, 2026-10-09: "bury the floor in snow" -- the first baby step: the GROUND only): under the Ice
// Age the open ground of Tristram lies under a blanket of bright snow. The town's Ice Age palette alone is dark --
// its ground entries are around 0.15 of white by DAY (iceage\0town_P.pal, measured) -- so the snow is a true-colour
// pass over exactly the open ground: TownWater.cpp SNOW GROUND (the floors of walkable and tree cells that nothing
// covered later; never houses, walls, fences, rocks, the river, sprites or items). SnowGround.hlsl has the look:
// paths and grass vanish under one even white, the art's baked shadows stay as blue shadows on it, and the snow
// takes the LIGHT the game gave each pixel (finished colour / the art's own colour: day, night, torches, Enriched
// Light), so it is dim at night and bright by day. The heroes' and NPCs' legs sink into it (Snowfall.cpp SNOW DEPTH
// copies the ground back over them, and this pass then snows on it).
// Its numbers: SNOW_LIT / SNOW_SHADOW colours, SNOW_SHADOW_LUM0..1 = the art luminance where shadow turns to lit snow,
// SNOW_TEXTURE = how much of the art's own light / dark is kept (0 = flat white), SNOW_LIGHT_CAP.
// WHEN: SnowGround_Active -- where it snows (Snowfall_Active), in the normal town for now (not Ravenholm), never in
// Classic, only DrawMode 1 / not DDraw (the pass runs only there).
static const float SNOW_LIT[3]       = { 0.92f, 0.94f, 0.98f };
// (th4) SOFTER BLUE (Mordor, 2026-10-09: "a little brighter blue, not saturated, just less noticeable"): the snow in the
// art's baked shadows was (0.50, 0.58, 0.74) -- too strong; white, grey, dark grey and black were tried the same evening
// and rejected. Now brighter (luminance 0.74, was 0.57) and half as saturated (blue - red 0.12, was 0.24).
static const float SNOW_SHADOW[3]    = { 0.70f, 0.74f, 0.82f };
static const float SNOW_SHADOW_LUM0  = 0.06f, SNOW_SHADOW_LUM1 = 0.11f;
static const float SNOW_TEXTURE      = 0.12f, SNOW_TEXTURE_LUM0 = 0.10f, SNOW_TEXTURE_LUM1 = 0.25f;
static const float SNOW_LIGHT_CAP    = 1.3f;
// (th4) ICE AGE NIGHT (Mordor, 2026-10-10, from his photos of moonlit snow: "the magical feeling of tints of blue and
// purple, but not childish goofiness"): by night (Snowfall_NightQ8) the snow's colours move to these -- moonlit icy blue,
// and in the art's baked shadows a soft violet-blue only a little darker (by day the lit snow clips to white, which hides
// most of the lit / shadow difference; at night nothing clips, and the day colours made a coarse mottle) -- and
// SNOW_TEXTURE to SNOW_TEXTURE_NIGHT of itself. The light still comes from the game (dim at night). The rest of the town
// is the 8-bit night palette (palette.cpp), which is already a dark blue under the Ice Age palette: only the snow went
// grey. Two things were tried first and REMOVED: a full-screen true-colour moonlight grade of the finished frame (a pass
// with 18 texture reads per pixel; it amplified every unevenness into grain -- "performance is killed, looks ugly",
// Mordor) and a moonlit night palette (it came out GREYER than the old night's blue shift on these dark colours).
static const float SNOW_LIT_NIGHT[3]    = { 0.66f, 0.79f, 1.00f };
static const float SNOW_SHADOW_NIGHT[3] = { 0.60f, 0.66f, 1.00f };
static const float SNOW_TEXTURE_NIGHT   = 0.5f;
// (th4) FLAKES ON THE SNOW: this pass paints snow over every pixel of its layer, and the layer was recorded before the
// snowflakes were drawn -- so a flake on the snow was repainted as snow lit by the FLAKE's brightness over the ground
// art's: always SNOW_LIGHT_CAP, pure white. By day that is white on white; at night (snow ~0.6) it was a white pixel on
// ~17% of the snow (Mordor's night screenshots, measured 2026-10-10). So a pixel's light is also capped at its art entry's
// OWN light -- the current palette's luminance over the art's (s3 row 1) -- x SNOW_FLAKE_CAP: a flake takes the ground's
// light. The frame is brighter than ScreenPalette, so this cap acts on nearly all the snow: it IS the snow's light at
// night (one value per palette entry: even snow); by day it lies above SNOW_LIGHT_CAP, so the day look is unchanged.
// KEEP IT (SnowGround.hlsl THE LIGHT): taking it out on a wrong theory broke the night snow twice in one day -- bright
// rings on every game pixel's edge (the light measured per window pixel), then a grid (neighbour caps). NO BLACK DOTS in
// the banks' shadows were holes in the snow LAYER (TownWater.cpp WATER SPECKS ON THE BANK), not this cap.
static const float SNOW_FLAKE_CAP = 1.08f;
// (th4) MOONLIGHT GLINTS (Mordor, 2026-10-10): by night single pixels of the snow sparkle -- all in SnowGround.hlsl (its
// header has how, its GLINT_* constants the look); this pass only hands it the ground anchor, the clock and the night.
bool SnowGround_Active()
{
	return gbRunGame && Snowfall_Active() && ( Atmos_InTown() || Dungeon == DUN_44_RAVENHOLM ) && GameMode != GM_CLASSIC && !DDraw && DrawMode == 1;	// (th4) Ravenholm too
}
static void SnowGround_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !SnowGround_Active() || !maskOk || !HeatTex[slot] ) return;
	const uchar* gl = 0; int gw = 0, gh = 0;
	if( !TownWater_SnowGround( gl, gw, gh ) || !gl || gw <= 0 || gh <= 0 ) return;
	if( !Atmos_Shader( dev, SnowGroundPS[slot], g_ps30_main_snowground ) ) return;
	if( !Grass_Upload( dev, SnowLayTex[slot], SnowLayW[slot], SnowLayH[slot], gw, gh, gl, gw ) ) return;
	uchar lut[ 512 ];	// row 0: the art's luminance per palette entry, the same weights SnowGround.hlsl uses for the frame;
						// row 1: the CURRENT palette's (day / night, gamma, contrast) -- FLAKES ON THE SNOW above
	for( int i = 0; i < 256; ++i ){
		lut[ i ] = (uchar)( ( FilePalette[ i ].peRed * 299 + FilePalette[ i ].peGreen * 587 + FilePalette[ i ].peBlue * 114 ) / 1000 );
		lut[ 256 + i ] = (uchar)( ( ScreenPalette[ i ].peRed * 299 + ScreenPalette[ i ].peGreen * 587 + ScreenPalette[ i ].peBlue * 114 ) / 1000 );
	}
	if( !Grass_Upload( dev, SnowLutTex[slot], SnowLutW[slot], SnowLutH[slot], 256, 2, lut, 256 ) ) return;
	// ICE AGE NIGHT (above): the colours of this moment
	const float n = Snowfall_NightQ8() / 256.0f;
	float lit[3], shd[3];
	for( int c = 0; c < 3; ++c ){ lit[c] = SNOW_LIT[c] + ( SNOW_LIT_NIGHT[c] - SNOW_LIT[c] ) * n; shd[c] = SNOW_SHADOW[c] + ( SNOW_SHADOW_NIGHT[c] - SNOW_SHADOW[c] ) * n; }
	const float tex = SNOW_TEXTURE * ( 1.0f + ( SNOW_TEXTURE_NIGHT - 1.0f ) * n );
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	for( int st = 2; st <= 3; ++st ){	// the layer and the table: exact texels
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	const float vz = View_Z();	// game px on screen per layer px (Alt+Z or the wheel; THE VIEW)
	// the town dialog / NPC speech box (PlayerTrade.cpp DrawDialogBox): the world shows through its 50 % DITHER, and
	// the snowy ground must show through it like everything else (Mordor). The half-res world/UI mask cannot: a texel
	// is UI if any of its 2 x 2 pixels is, and every 2 x 2 in the box holds a dither dot -- the whole box read as UI, no
	// snow behind it but a few stray spots. Inside the box the shader reads the FULL-RES WORLD MAP instead (s4 =
	// Grass_BuildMask's, built for the snow while a box is open): dots and text UI, the pixels between them world.
	// Screen uv; none (or no map) = 2 = the half-res mask everywhere. (Not a UI RECT: the DYNAMIC ZOOM reads those.)
	int bx0, by0, bx1, by1;
	const bool box = DynZoom_SeeThroughBoxes( &bx0, &by0, &bx1, &by1 ) > 0 && GrassWorldOk[slot] && GrassWorldTex[slot];
	// MOONLIGHT GLINTS (SnowGround.hlsl): the ground anchor in LAYER px (drawn px / the Alt+Z zoom; the wheel zoom draws at 1x)
	float gax, gay; Atmos_AnchorDrawn( gax, gay );
	const float zl = IsZoomDisable ? 1.0f : 2.0f;
	const float c[7][4] = {
		{ ScreenWidth / vz, ScreenHeight / vz, -ViewSX / vz, -ViewSY / vz },	// screen uv -> layer px, + the DYNAMIC ZOOM shift (as SunRays.hlsl c12)
		{ 1.0f / gw, 1.0f / gh, 1.0f, SNOW_LIGHT_CAP },
		{ lit[0], lit[1], lit[2], SNOW_SHADOW_LUM1 },
		{ shd[0], shd[1], shd[2], SNOW_SHADOW_LUM0 },
		{ tex, SNOW_TEXTURE_LUM0, 1.0f / ( SNOW_TEXTURE_LUM1 - SNOW_TEXTURE_LUM0 ), SNOW_FLAKE_CAP },
		{ box ? (float)bx0 / ScreenWidth : 2.0f, box ? (float)by0 / ScreenHeight : 2.0f,
		  box ? (float)bx1 / ScreenWidth : 2.0f, box ? (float)by1 / ScreenHeight : 2.0f },
		{ gax / zl, gay / zl, AtmosSec, n },	// MOONLIGHT GLINTS: anchor, pause-aware clock, the night
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( SnowGroundPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 7 );
	dev->SetTexture( 2, SnowLayTex[slot] );
	dev->SetTexture( 3, SnowLutTex[slot] );
	dev->SetTexture( 4, box ? GrassWorldTex[slot] : 0 );	// the FULL-RES WORLD MAP (above): exact pixels
	dev->SetSamplerState( 4, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 4, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 4, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 4, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 4, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 4, D3DSAMP_SRGBTEXTURE, FALSE );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 2, 0 ); dev->SetTexture( 3, 0 ); dev->SetTexture( 4, 0 );
	Atmos_End( dev, dst, sb );
}

// (th4) ICE RIVER -- the river of Ice Age Tristram as a still, shiny MIRROR of a winter sky, by day (Mordor, 2026-10-10,
// from his photos of winter rivers). STEP 1 of 3, built in baby steps: the sky, its clouds and the water's own depth;
// step 2 = the snowy trees of the banks mirrored in it, step 3 = the far bank's snow mirrored. The look and its numbers
// live in IceRiver.hlsl (header there); the ones here are about time and motion.
// WHEN: IceRiver_Active / IceRiver_Shows -- config IceAgeRiverMirror (default OFF, Mordor; off = the old frozen river), where it snows
// (Snowfall_Active), in the normal town only (Ravenholm is always night, and at night the river stays exactly as it was:
// Mordor's choice, "dark water"), never in Classic, DrawMode 1 / not DDraw. config.ini is read at start-up only. By day only: IceRiver_Day fades it out over the dusk (gone at ICE_RIVER_NIGHT_END of DayNight_NightQ8) and in
// again at dawn. The sky follows the snow: a paler, softer blue under more cloud in a storm, blue with white clouds in a
// calm (Snowfall_Storm, eased like the snow itself). No grey anywhere (Mordor: IceRiver.hlsl STORM).
// THE DEPTH (ICE_SKY_PARALLAX): the mirrored sky moves only that share of the ground's motion when the view scrolls, so the
// water slides past a far-away sky, as in a real reflection (Mordor's pick over a sky painted on the water). It is measured
// about the screen centre, so the wheel zoom (which keeps the hero in place) magnifies it without shifting it.
// THE MAP (IceRiver_BuildMask, game resolution, L8, sampled POINT), from TownWater.cpp's river mask (TownWater_MirrorRows:
// the palette index of every water pixel, 0 = none), the world snapshot and the finished frame:
//   0   covered: the UI (finished frame != snapshot, the UI RECTS, the automap's MapBlend lines, item art), a sprite or a
//       snowflake (snapshot entry 128..255 -- the sprite half of the palette, where the flakes' shades are too), or the
//       river under something the town drew over it (TownWater.cpp MSurf 2: a tree in front of it, a rock) -- never
//       painted, and counted as water when the shader looks for the bank (no dark line round a hero in the river or
//       round the branches of a tree in front of it)
//   24  bank: every other pixel -- terrain that is not river water
//   128 water: the river mask's index is still what the snapshot shows (anything drawn over it later drops out). The
//       mask is SOLID while the mirror shows: TownWater.cpp fills the art's grains and checkerboards into it (MirrorFill)
//   192 water holding a hero REFLECTION pixel (TownWater_ReflectionPixels): the shader averages their dither
// Built BEFORE HeatHaze_BuildMask, which uses the snapshot up (PostFxPresent). Under the DYNAMIC ZOOM like the WET MAP: in
// the drawn frame from the snapshot alone, then onto the screen with the zoom's UI mask (DynZoom_RemapFull, UI = 0).
// COUPLINGS: TownWater.cpp builds its river mask under the Ice Age for this even with WaterShimmer and WaterReflection off
// (FeatureOn), hands it to Perspective_WarpWorld while it is in use (TownWater_Surface) and darkens the water then only
// if one of those options is on (its old look at night). The pass runs right after SnowGround_Present, so the ice halo
// (BioGlow), the frost and the rest lie on top.
// KNOWN-GOOD CHECK: Ice Age Tristram by day, in a calm spell: the river shows a blue sky with white clouds; walk along it
// and the clouds slide past slower than the banks; a hero beside the water is mirrored with no checker pattern; the
// snowflakes fall over the water; a thin dark line follows the banks, never a hero standing in the river; in a storm the
// water turns a paler blue, never grey; at dusk it fades to the old dark river; on pause the clouds stop.
static const float ICE_SKY_PARALLAX     = 0.15f;	// 0 = the sky fixed to the screen, 1 = painted on the water
static const float ICE_CLOUD_WIND       = 6.0f;		// cloud drift, world px per second to the right (x 2 in a storm)
static const int   ICE_RIVER_NIGHT_END  = 160;		// DayNight_NightQ8 at which the mirror is gone (0 day .. 256 night)
static const float ICE_RIVER_DUSK_LIGHT = 0.55f;	// its brightness when it is about to go (x 1 by full day)
static double IceCloudDrift = 0.0;					// world px the clouds have drifted (summed: a storm speeds them up smoothly)

bool IceRiver_Active()
{
	return IceAgeRiverMirror && gbRunGame && Snowfall_Active() && Atmos_InTown() && GameMode != GM_CLASSIC && !DDraw && DrawMode == 1;
}
static float IceRiver_Day()
{
	const int night = DayNight_NightQ8();
	if( night >= ICE_RIVER_NIGHT_END ) return 0.0f;
	const float t = (float)night / ICE_RIVER_NIGHT_END;
	return 1.0f - t * t * ( 3.0f - 2.0f * t );
}
// the mirror is on screen: TownWater.cpp makes its river mask solid only then (at night all stays as before)
bool IceRiver_Shows() { return IceRiver_Active() && IceRiver_Day() > 0.0f; }

static void IceRiver_BuildMask( LPDIRECT3DDEVICE9 dev, int slot )
{
	IceRiverMapOk[slot] = false;
	if( !IceRiver_Shows() || !HeatSnapValid || !HeatSnapRows || !SoftSrcSurfacePtr ) return;
	const uchar* cls = 0;
	const uchar* riv = TownWater_MirrorRows( cls );
	if( !riv || !cls ) return;	// no river on screen this frame
	const bool dz = ViewDyn;
	if( dz && DynZoom_RedScreen() ) return;
	const int W = ScreenWidth, H = ScreenHeight;
	static std::vector<uchar> Ice1x;	// the map, built here in system memory (the reflections are marked by reading it
	Ice1x.resize( (size_t)W * H );		// back; a locked DYNAMIC texture is slow to read), then copied / remapped into the texture
	int bx0 = 0, by0 = 0, bx1 = W, by1 = H;
	if( dz && !DynZoom_ShownBox( bx0, by0, bx1, by1 ) ) return;	// DynZoom_RemapFull reads nothing else
	if( !IceRiverTex[slot] || IceRiverTexW[slot] != W || IceRiverTexH[slot] != H ){
		if( IceRiverTex[slot] ){ IceRiverTex[slot]->Release(); IceRiverTex[slot] = 0; }
		IceRiverTexW[slot] = IceRiverTexH[slot] = 0;
		if( D3D_OK != dev->CreateTexture( W, H, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &IceRiverTex[slot], 0 ) ){ IceRiverTex[slot] = 0; return; }
		IceRiverTexW[slot] = W; IceRiverTexH[slot] = H;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != IceRiverTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
	const uchar* fin = SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	// 16 pixels at a time (SSE2); the scalar loop finishes each row and is the reference:
	// out = finished != snapshot ? 0 : river index still in the snapshot ? 128 : sprite / flake / covered river ? 0 : 24
	// (covered river: the river mask hidden by a later draw, or TownWater.cpp MSurf 2 -- a tree in front of the river)
	const __m128i zero = _mm_setzero_si128(), ones = _mm_set1_epi8( (char)0xFF ), vBank = _mm_set1_epi8( 24 ), vWater = _mm_set1_epi8( (char)128 );
	const __m128i two = _mm_set1_epi8( 2 );

	for( int y = by0; y < by1; ++y ){
		const uchar* s = HeatSnapRows + (size_t)y * HeatSnapPitch;
		const uchar* f = dz ? s : fin + (size_t)y * WorkingWidth;
		const uchar* r = riv + (size_t)y * WorkingWidth;
		const uchar* m = cls + (size_t)y * WorkingWidth;
		uchar* d = &Ice1x[ (size_t)y * W ];
		int x = bx0;
		for( ; x + 16 <= bx1; x += 16 ){
			const __m128i sv = _mm_loadu_si128( (const __m128i*)( s + x ) ), fv = _mm_loadu_si128( (const __m128i*)( f + x ) );
			const __m128i rv = _mm_loadu_si128( (const __m128i*)( r + x ) );
			const __m128i inRiver = _mm_xor_si128( _mm_cmpeq_epi8( rv, zero ), ones );
			const __m128i water = _mm_and_si128( inRiver, _mm_cmpeq_epi8( rv, sv ) );
			__m128i covered = _mm_or_si128( _mm_cmplt_epi8( sv, zero ), _mm_andnot_si128( water, inRiver ) );	// sv >= 128, or the river hidden
			covered = _mm_or_si128( covered, _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( m + x ) ), two ) );
			const __m128i o = _mm_or_si128( _mm_and_si128( water, vWater ), _mm_andnot_si128( _mm_or_si128( water, covered ), vBank ) );
			_mm_storeu_si128( (__m128i*)( d + x ), _mm_and_si128( o, _mm_cmpeq_epi8( fv, sv ) ) );
		}
		for( ; x < bx1; ++x ){
			const uchar sv = s[ x ], rv = r[ x ];
			const uchar o = ( rv && rv == sv ) ? 128 : ( sv >= 128 || rv || m[ x ] == 2 ) ? 0 : 24;
			d[ x ] = f[ x ] == sv ? o : 0;
		}
	}
	// hero reflection pixels (TownWater.cpp DrawReflection lists them, in the drawn frame)
	int n = 0;
	const int* rp = TownWater_ReflectionPixels( n );
	for( int i = 0; i < n; ++i ){
		const int x = rp[ i ] & 0xFFFF, y = rp[ i ] >> 16;
		if( x >= bx0 && x < bx1 && y >= by0 && y < by1 && Ice1x[ (size_t)y * W + x ] == 128 ) Ice1x[ (size_t)y * W + x ] = 192;
	}
	bool ok = true;
	if( dz ) ok = DynZoom_RemapFull( Ice1x.data(), W, (uchar*)lr.pBits, lr.Pitch, 0 );
	else for( int y = 0; y < H; ++y ) memcpy( (uchar*)lr.pBits + (size_t)y * lr.Pitch, &Ice1x[ (size_t)y * W ], W );
	if( !dz ) for( int i = 0; i < UiRectN; ++i ){	// UI RECTS: the panels are UI even where they match the world
		const int x0 = max( 0, UiRects[ i ][ 0 ] ), y0 = max( 0, UiRects[ i ][ 1 ] );
		const int x1 = min( W, UiRects[ i ][ 2 ] ), y1 = min( H, UiRects[ i ][ 3 ] );
		for( int y = y0; y < y1; ++y ) if( x1 > x0 ) memset( (uchar*)lr.pBits + (size_t)y * lr.Pitch + x0, 0, x1 - x0 );
	}
	if( ok ) Automap_MarkUi( (uchar*)lr.pBits, lr.Pitch, W, H, false );	// (th4) THE AUTOMAP IS UI TOO (HeatHaze_BuildMask): its MapBlend lines are not in the 8-bit frame
	if( ok ) ItemOverlay_MarkUi( (uchar*)lr.pBits, lr.Pitch, W, H, false );	// (th4) THE ITEM ART IS UI TOO (ItemOverlay.cpp): a dragged item over the river
	IceRiverTex[slot]->UnlockRect( 0 );
	IceRiverMapOk[slot] = ok;
}

// The clouds' noise: a 256 x 256 tileable fBm of value noise (6 octaves, 4 .. 128 cells across), its levels
// equalised so that IceRiver.hlsl's CLOUD_COVER reads as "about this share of the sky is clear". Made once (MANAGED).
static bool IceRiver_Noise( LPDIRECT3DDEVICE9 dev, int slot )
{
	if( IceNoiseTex[slot] ) return true;
	const int N = 256;
	static std::vector<uchar> img;
	if( img.empty() ){
		std::vector<float> acc( (size_t)N * N, 0.0f );
		uint seed = 0x2545F491u;	// fixed: every player sees the same clouds
		float amp = 1.0f;
		for( int o = 0, cells = 4; o < 6; ++o, cells *= 2, amp *= 0.5f ){
			std::vector<float> lat( (size_t)cells * cells );
			for( auto& v : lat ){ seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; v = ( seed >> 8 ) / 16777216.0f; }
			for( int y = 0; y < N; ++y ){
				const float fy = (float)y * cells / N; const int iy = (int)fy, y1 = ( iy + 1 ) % cells;
				float ty = fy - iy; ty = ty * ty * ( 3.0f - 2.0f * ty );
				for( int x = 0; x < N; ++x ){
					const float fx = (float)x * cells / N; const int ix = (int)fx, x1 = ( ix + 1 ) % cells;
					float tx = fx - ix; tx = tx * tx * ( 3.0f - 2.0f * tx );
					const float a = lat[ iy * cells + ix ], b = lat[ iy * cells + x1 ], c = lat[ y1 * cells + ix ], e = lat[ y1 * cells + x1 ];
					acc[ (size_t)y * N + x ] += amp * ( a + ( b - a ) * tx + ( c - a ) * ty + ( a - b - c + e ) * tx * ty );
				}
			}
		}
		// equalise: each value -> its rank among all (4096 bins)
		float lo = acc[ 0 ], hi = acc[ 0 ];
		for( float v : acc ){ lo = min( lo, v ); hi = max( hi, v ); }
		const int B = 4096;
		std::vector<int> hist( B + 1, 0 );
		auto bin = [&]( float v ){ return (int)( ( v - lo ) / ( hi - lo + 1e-6f ) * B ); };
		for( float v : acc ) ++hist[ bin( v ) ];
		for( int i = 1; i <= B; ++i ) hist[ i ] += hist[ i - 1 ];
		img.resize( (size_t)N * N );
		for( size_t i = 0; i < acc.size(); ++i ) img[ i ] = (uchar)( 255 * hist[ bin( acc[ i ] ) ] / ( N * N ) );
	}
	if( D3D_OK != dev->CreateTexture( N, N, 1, 0, D3DFMT_L8, D3DPOOL_MANAGED, &IceNoiseTex[slot], 0 ) ){ IceNoiseTex[slot] = 0; return false; }
	D3DLOCKED_RECT lr;
	if( D3D_OK != IceNoiseTex[slot]->LockRect( 0, &lr, 0, 0 ) ){ IceNoiseTex[slot]->Release(); IceNoiseTex[slot] = 0; return false; }
	for( int y = 0; y < N; ++y ) memcpy( (uchar*)lr.pBits + (size_t)y * lr.Pitch, &img[ (size_t)y * N ], N );
	IceNoiseTex[slot]->UnlockRect( 0 );
	return true;
}

static void IceRiver_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	// the drift, on the pause-aware clock (AtmosDt: this PostFxPresent's step, like Atmos_Tick)
	IceCloudDrift += AtmosDt * ICE_CLOUD_WIND * ( 1.0f + Snowfall_Storm() );
	if( IceCloudDrift > 1.0e6 ) IceCloudDrift -= 1.0e6;	// float precision in the shader; one jump in ~a day of play
	if( !IceRiverMapOk[slot] || !IceRiverTex[slot] || !IceRiver_Active() ) return;
	const float day = IceRiver_Day();
	if( day <= 0.0f ) return;
	if( !Atmos_Shader( dev, IceRiverPS[slot], g_ps30_main_iceriver ) || !IceRiver_Noise( dev, slot ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	dev->SetSamplerState( 2, D3DSAMP_MINFILTER, D3DTEXF_POINT );	// the map: exact codes
	dev->SetSamplerState( 2, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// the noise: smooth, tiled
	dev->SetSamplerState( 3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 3, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 3, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	dev->SetSamplerState( 3, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	dev->SetSamplerState( 3, D3DSAMP_SRGBTEXTURE, FALSE );
	float ax, ay; Atmos_Anchor( ax, ay );
	const float vz = View_Z();
	const float cx = ( ScreenWidth * 0.5f - ax ) / vz, cy = ( ScreenHeight * 0.5f - ay ) / vz;	// the world point at the screen centre
	const float c[3][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, vz, AtmosSec },
		{ ICE_SKY_PARALLAX * cx + (float)IceCloudDrift, ICE_SKY_PARALLAX * cy, day, Snowfall_Storm() },
		{ ICE_RIVER_DUSK_LIGHT + ( 1.0f - ICE_RIVER_DUSK_LIGHT ) * day, 0, 0, 0 },
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( IceRiverPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 3 );
	dev->SetTexture( 2, IceRiverTex[slot] );
	dev->SetTexture( 3, IceNoiseTex[slot] );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 3, 0 );
	Atmos_End( dev, dst, sb );
}

// (th4) GRASS: the WORLD MAP, GrassWorldTex -- one byte per GAME pixel, 255 = world, 0 = UI. Grass.hlsl tests it,
// not the shared world/UI mask (HeatTex): that one is half resolution (a texel is UI if ANY of its 2 x 2 pixels
// is) and read smoothed, so around the mouse cursor -- drawn into the 8-bit frame like all the UI (Draw.cpp
// DrawCursor) -- a fringe of about two pixels lost its blades, and the fringe went wherever the cursor went
// (Mordor: "some movement under the cursor"). Built like Wet_BuildMask's WET MAP, the full-resolution map next
// to it: UI = the finished frame differs from the world snapshot, plus the panels' rectangles (UI RECTS); under
// the DYNAMIC ZOOM the drawn frame is all world and DynZoom_RemapFull puts the zoom's UI mask on it. Must run
// BEFORE HeatHaze_BuildMask, which uses the snapshot up (PostFxPresent calls it right after Wet_BuildMask).
// (th4) SNOW GROUND uses this WORLD MAP too, inside the town dialog / speech box only (SnowGround_Present), and so does
// the falling snow (Snowfall_Present): never together with the grass (Grass_Active is off under the Ice Age), so the one
// texture serves all three.
static void Grass_BuildMask( LPDIRECT3DDEVICE9 dev, int slot )
{
	GrassWorldOk[slot] = false;
	int bx0, by0, bx1, by1;
	const bool snowBox = ( SnowGround_Active() || Snowfall_GpuWanted() ) && DynZoom_SeeThroughBoxes( &bx0, &by0, &bx1, &by1 ) > 0;	// (th4) the falling snow too (Snowfall_Present)
	if( !( Grass_Active() || snowBox ) || !HeatSnapValid || !HeatSnapRows || !SoftSrcSurfacePtr ) return;
	const bool dz = ViewDyn;
	if( dz && DynZoom_RedScreen() ) return;	// all UI: no blades, as without the zoom under DrawRedScreen
	const int W = ScreenWidth, H = ScreenHeight;
	if( !GrassWorldTex[slot] || GrassWorldW[slot] != W || GrassWorldH[slot] != H ){
		if( GrassWorldTex[slot] ){ GrassWorldTex[slot]->Release(); GrassWorldTex[slot] = 0; }
		GrassWorldW[slot] = GrassWorldH[slot] = 0;
		if( D3D_OK != dev->CreateTexture( W, H, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &GrassWorldTex[slot], 0 ) ){ GrassWorldTex[slot] = 0; return; }
		GrassWorldW[slot] = W; GrassWorldH[slot] = H;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != GrassWorldTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
	bool ok = true;
	if( dz ){
		static std::vector<uchar> all;	// the drawn frame: world everywhere (the UI is not in it)
		if( all.size() != (size_t)W * H ) all.assign( (size_t)W * H, 255 );
		ok = DynZoom_RemapFull( all.data(), W, (uchar*)lr.pBits, lr.Pitch, 0 );
	}else{
		const uchar* fin = SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		for( int y = 0; y < H; ++y ){	// 255 where the finished frame still shows the world, 16 at a time
			const uchar* f = fin + (size_t)y * WorkingWidth;
			const uchar* sn = HeatSnapRows + (size_t)y * HeatSnapPitch;
			uchar* d = (uchar*)lr.pBits + (size_t)y * lr.Pitch;
			int x = 0;
			for( ; x + 16 <= W; x += 16 ) _mm_storeu_si128( (__m128i*)( d + x ), _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( f + x ) ), _mm_loadu_si128( (const __m128i*)( sn + x ) ) ) );
			for( ; x < W; ++x ) d[ x ] = f[ x ] == sn[ x ] ? 255 : 0;
		}
		for( int i = 0; i < UiRectN; ++i ){	// UI RECTS: the panels are UI even where they match the world
			const int x0 = max( 0, UiRects[ i ][ 0 ] ), y0 = max( 0, UiRects[ i ][ 1 ] );
			const int x1 = min( W, UiRects[ i ][ 2 ] ), y1 = min( H, UiRects[ i ][ 3 ] );
			for( int y = y0; y < y1; ++y ) if( x1 > x0 ) memset( (uchar*)lr.pBits + (size_t)y * lr.Pitch + x0, 0, x1 - x0 );
		}
	}
	if( ok ) Automap_MarkUi( (uchar*)lr.pBits, lr.Pitch, W, H, false );	// (th4) THE AUTOMAP IS UI TOO (HeatHaze_BuildMask): its MapBlend lines are not in the 8-bit frame
	if( ok ) ItemOverlay_MarkUi( (uchar*)lr.pBits, lr.Pitch, W, H, false );	// (th4) THE ITEM ART IS UI TOO (ItemOverlay.cpp): no blades over a dragged item
	GrassWorldTex[slot]->UnlockRect( 0 );
	GrassWorldOk[slot] = ok;
}

static void Grass_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	if( !Grass_Active() || !GrassWorldOk[slot] || !GrassWorldTex[slot] ) return;
	const uchar* lay = 0; const uchar* idx = 0; const uchar* blk = 0; int lw = 0, lh = 0, bw = 0, bh = 0, box[4], c00[2], block = 16;
	if( !TownWater_GrassLayer( lay, idx, lw, lh, blk, bw, bh, box, c00, block ) || !lay || !idx || !blk ) return;
	const int zoom = IsZoomDisable ? 1 : 2;
	if( lw != ( ScreenWidth + zoom - 1 ) / zoom || lh != ( ScreenHeight + zoom - 1 ) / zoom ) return;	// layer is pre-zoom size
	uint gen = 0;
	const uchar* cells = TownWater_GrassCells( gen );
	if( !cells ) return;
	if( !Atmos_Shader( dev, GrassPS[slot], g_ps30_main_grass ) ) return;
	if( !Grass_Upload( dev, GrassLayTex[slot], GrassLayW[slot], GrassLayH[slot], lw, lh, lay, lw ) ) return;
	if( !Grass_Upload( dev, GrassBlkTex[slot], GrassBlkW[slot], GrassBlkH[slot], bw, bh, blk, bw ) ) return;
	if( !Grass_Upload( dev, GrassIdxTex[slot], GrassIdxW[slot], GrassIdxH[slot], lw, lh, idx, lw ) ) return;
	// the palette the 8-bit frame is shown with now (day / night changes it), as TreeSway_Present's
	if( !GrassPalTex[slot] && D3D_OK != dev->CreateTexture( 256, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &GrassPalTex[slot], 0 ) ){ GrassPalTex[slot] = 0; return; }
	{
		D3DLOCKED_RECT lr;
		if( D3D_OK != GrassPalTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
		uint* p = (uint*)lr.pBits;
		for( int i = 0; i < 256; ++i ) p[i] = 0xFF000000u | (uint)ScreenPalette[i].peRed << 16 | (uint)ScreenPalette[i].peGreen << 8 | ScreenPalette[i].peBlue;
		GrassPalTex[slot]->UnlockRect( 0 );
	}
	if( GrassCellGen[slot] != gen || !GrassCellTex[slot] ){	// the cell shares change only with the map
		if( !Grass_Upload( dev, GrassCellTex[slot], GrassCellW[slot], GrassCellH[slot], 112, 112, cells, 112, true ) ) return;
		GrassCellGen[slot] = gen;
	}
	// THE GAME-RESOLUTION FRAME (s7): a blade takes its colour from ONE game pixel, its root. The window frame this
	// pass works on is, with filtering on and a window bigger than the game, the game frame STRETCHED smoothly (or
	// by FSR, with its sharpening) -- each window pixel a blend that includes up to a quarter of the game pixel next
	// door. A root beside a white letter, a lit window or a bright sprite then grew a whitish blade (Mordor: "some
	// blades have a brighter look", the "PERKS" button at night). That stretch reads the frame from VideoSurface
	// (RenderQuadTexture / RenderQuadTextureUltra: useRenderTexture), which still holds it now: one copy of it, and
	// the colours are exact. Without a stretch the window pixels ARE the game pixels: s7 is the window frame.
	LPDIRECT3DSURFACE9 vid = slot == 0 ? VideoSurface : SecVideoSurface;
	bool gameCopy = false;
	{
		D3DSURFACE_DESC dd;
		if( vid && FilteringMode != D3DTEXF_NONE && D3D_OK == dst->GetDesc( &dd ) && ( (int)dd.Width != ScreenWidth || (int)dd.Height != ScreenHeight )
		 && Fsr_EnsureTex( dev, GrassGameTex[slot], GrassGameW[slot], GrassGameH[slot], ScreenWidth, ScreenHeight, D3DFMT_X8R8G8B8 ) ){
			LPDIRECT3DSURFACE9 gs = 0;
			if( D3D_OK == GrassGameTex[slot]->GetSurfaceLevel( 0, &gs ) ){
				gameCopy = D3D_OK == dev->StretchRect( vid, 0, gs, 0, D3DTEXF_NONE );
				gs->Release();
			}
		}
	}
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );	// s0 POINT
	if( !ok ) return;
	dev->SetTexture( 1, GrassWorldTex[slot] );	// the WORLD MAP instead of the half-res world/UI mask Atmos_Begin set
	dev->SetTexture( 7, gameCopy ? GrassGameTex[slot] : PostTex[slot] );	// THE GAME-RESOLUTION FRAME (above)
	for( int st = 1; st <= 7; ++st ){	// codes, indices, the world map, the palette, the game frame: exact texels; the cell shares blend between cells
		const DWORD f = st == 3 ? D3DTEXF_LINEAR : D3DTEXF_POINT;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	const int gb = GrassBlades > 10 ? 10 : GrassBlades;
	float hmax = (float)( GRASS_HEIGHT_BASE + gb );
	if( hmax > GRASS_HEIGHT_MAX ) hmax = (float)GRASS_HEIGHT_MAX;
	const float wind = Wind_Weather();
	const float vz = View_Z();	// game px on screen per layer px (Alt+Z or the wheel; THE VIEW)
	float c[6][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, vz, WindSec },
		{ (float)c00[0], (float)c00[1], GRASS_SWAY * hmax * wind, vz * (float)w / ScreenWidth },
		{ (float)lw, (float)lh, ViewSX, ViewSY },
		{ hmax, 1.0f / hmax, hmax + 2.0f, 1.0f / block },
		{ (float)bw, (float)bh, wind, 1.0f },
		{ SUN_DIR_X, SUN_DIR_Y - 1.0f, GRASS_SHADOW, 0 },	// SHADOWS ON THE BLADES: the ground spot a point's sunlight comes from, per px of height (the art's light, SUN RAYS above); w THE MODE
	};
	dev->SetTexture( 2, GrassLayTex[slot] );
	dev->SetTexture( 3, GrassCellTex[slot] );
	dev->SetTexture( 4, GrassBlkTex[slot] );
	dev->SetTexture( 5, GrassIdxTex[slot] );
	dev->SetTexture( 6, GrassPalTex[slot] );
	// the quad: the grass's box (pre-zoom px; TownWater.cpp already widened it to where the blades reach), on
	// screen (layer px * vz + the DYNAMIC ZOOM shift), in the px of a tw x th target -- as SunRays_Present
	auto drawBox = [&]( int tw, int th ){
		const float kx = (float)tw / ScreenWidth, ky = (float)th / ScreenHeight;
		int x0 = (int)floorf( ( box[0] * vz + ViewSX ) * kx ), x1 = (int)ceilf( ( box[2] * vz + ViewSX ) * kx );
		int y0 = (int)floorf( ( box[1] * vz + ViewSY ) * ky ), y1 = (int)ceilf( ( box[3] * vz + ViewSY ) * ky );
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0; if( x1 > tw ) x1 = tw; if( y1 > th ) y1 = th;
		if( x0 >= x1 || y0 >= y1 ) return;
		struct V { float x, y, z, rhw, u, v; };
		const float l = x0 - 0.5f, rt = x1 - 0.5f, t = y0 - 0.5f, b = y1 - 0.5f;
		const float u0 = (float)x0 / tw, u1 = (float)x1 / tw, v0 = (float)y0 / th, v1 = (float)y1 / th;
		const V q[6] = { { l, t, 0, 1, u0, v0 }, { rt, t, 0, 1, u1, v0 }, { l, b, 0, 1, u0, v1 },
		                 { rt, t, 0, 1, u1, v0 }, { rt, b, 0, 1, u1, v1 }, { l, b, 0, 1, u0, v1 } };
		dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
		dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, q, sizeof( V ) );
	};
	// THE RESOLUTION (config GrassResolution, Grass.hlsl c5.w THE MODE): 0 = the blades over the frame at window
	// resolution, one pass. 1 / 2 = the blades into GrassLowTex at the game's resolution / half of it (the heavy pass
	// on 1/3 .. 1/12 of the pixels at 1080p), then GrassCompose.hlsl lays that layer over the window frame. The
	// world itself is drawn at the game's resolution and stretched, so at 1 the blades match it. DEFAULT 2 (Mordor, 2026-10-09:
	// players on built-in graphics lost the town's frame rate to the grass; GPU timestamp queries measured ~4 ms
	// a frame at window resolution on an RX 6900 XT).
	bool low = GrassResolution == 1 || GrassResolution == 2;
	const int tw = GrassResolution == 2 ? ( ScreenWidth + 1 ) / 2 : ScreenWidth, th = GrassResolution == 2 ? ( ScreenHeight + 1 ) / 2 : ScreenHeight;
	LPDIRECT3DSURFACE9 lowSurf = 0;
	if( low && ( !Atmos_Shader( dev, GrassComposePS[slot], g_ps30_main_grasscompose )
	          || !Fsr_EnsureTex( dev, GrassLowTex[slot], GrassLowW[slot], GrassLowH[slot], tw, th, D3DFMT_A8R8G8B8 )
	          || D3D_OK != GrassLowTex[slot]->GetSurfaceLevel( 0, &lowSurf ) ) ) low = false;	// then the window-resolution pass
	dev->SetPixelShader( GrassPS[slot] );
	if( !low ){
		dev->SetRenderTarget( 0, dst );
		dev->SetPixelShaderConstantF( 0, &c[0][0], 6 );
		drawBox( w, h );
	}else{
		dev->SetRenderTarget( 0, lowSurf );
		dev->Clear( 0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0 );	// no blade = transparent
		c[1][3] = vz * (float)tw / ScreenWidth;	// target px per layer px: the anti-aliasing at the target's size
		c[5][3] = 1.0f;
		dev->SetPixelShaderConstantF( 0, &c[0][0], 6 );
		drawBox( tw, th );
		lowSurf->Release();
		dev->SetRenderTarget( 0, dst );
		dev->SetPixelShader( GrassComposePS[slot] );
		dev->SetTexture( 8, GrassLowTex[slot] );
		dev->SetSamplerState( 8, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		dev->SetSamplerState( 8, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		dev->SetSamplerState( 8, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( 8, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( 8, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( 8, D3DSAMP_SRGBTEXTURE, FALSE );
		c[5][3] = 2.0f;
		dev->SetPixelShaderConstantF( 0, &c[0][0], 6 );
		drawBox( w, h );
		dev->SetTexture( 8, 0 );
	}
	dev->SetTexture( 2, 0 ); dev->SetTexture( 3, 0 ); dev->SetTexture( 4, 0 ); dev->SetTexture( 5, 0 ); dev->SetTexture( 6, 0 ); dev->SetTexture( 7, 0 );
	Atmos_End( dev, dst, sb );
}

static void LevelUp_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !LevelUp_Active() || !maskOk || !HeatTex[slot] ) return;
	if( !Atmos_Shader( dev, LevelUpPS[slot], g_ps30_main_levelup ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	float sx, sy;
	const Player& p = Players[ CurrentPlayerIndex ];	// where the hero is NOW: the glow follows him (LevelUpFx_Start)
	Atmos_TileToScreen( p.Row, p.Col, p.xOfs, p.yOfs, sx, sy );
	const float c[2][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), ( AtmosSec - LevelUpT0 ) / LEVELUP_SEC },
		{ sx, sy, 0, 0 },
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( LevelUpPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 2 );
	Fsr_Quad( dev, w, h );
	Atmos_End( dev, dst, sb );
}

// (th4) COLD BREATH (config ColdBreath 0..10, default 5): Breath.cpp notes the sprites, finds their
// heads and keeps the puffs fixed to the world (its header has the whole story); here they are drawn by
// Breath.hlsl as soft vapour over world pixels (world/UI mask). Breath_Update runs every presented frame
// so the breaths keep their rhythm; MAX 48 puffs a frame (Breath.hlsl's constant array, c2..c49).
static void Breath_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	const int zoom = IsZoomDisable ? 1 : 2;
	float dax, day; Atmos_AnchorDrawn( dax, day );	// Breath.cpp keeps its puffs in the DRAWN frame
	float puffs[48][4];
	const int n = Breath_Update( AtmosSec, zoom, dax, day, Wind_Strength(), &puffs[0][0], 48 );
	if( n <= 0 || !maskOk || !HeatTex[slot] ) return;
	// (th4) DYNAMIC ZOOM (THE VIEW): the puffs onto the screen -- centre and radius
	for( int i = 0; i < n; ++i ){ View_Point( puffs[ i ][ 0 ], puffs[ i ][ 1 ] ); puffs[ i ][ 2 ] *= ViewZ; }
	float ax = dax, ay = day; View_Point( ax, ay );
	if( !Atmos_Shader( dev, BreathPS[slot], g_ps30_main_breath ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	const float c[2][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, View_Z(), AtmosSec },
		{ ax, ay, ( ColdBreath > 10 ? 10 : ColdBreath ) * 0.16f, (float)n },	// opacity: 5 = 0.8
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( BreathPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 2 );
	dev->SetPixelShaderConstantF( 2, &puffs[0][0], n );
	// ONE SQUARE PER PUFF, not a full-window quad: only pixels a puff can touch are shaded (a full-window
	// pass cost ~3 ms at 1080p with 48 puffs on a fast GPU). Same shader, same constants: each pixel in a
	// square still sums EVERY puff and writes the finished colour opaquely (no blending), so where squares
	// overlap the pixel is written twice with the same value -- the picture is identical to the full pass.
	// UVs match Fsr_Quad's full-window mapping (pixel i of dst gets u = (i + 0.5) / w).
	struct V { float x, y, z, rhw, u, v; };
	V q[ 48 * 6 ];
	int nv = 0;
	const float kx = (float)w / ScreenWidth, ky = (float)h / ScreenHeight;
	for( int i = 0; i < n; ++i ){
		const float r = fabsf( puffs[ i ][ 2 ] ) + 1.0f;	// radius in game px (negative = toxic, Breath.cpp)
		int x0 = (int)floorf( ( puffs[ i ][ 0 ] - r ) * kx ), x1 = (int)ceilf( ( puffs[ i ][ 0 ] + r ) * kx );
		int y0 = (int)floorf( ( puffs[ i ][ 1 ] - r ) * ky ), y1 = (int)ceilf( ( puffs[ i ][ 1 ] + r ) * ky );
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0; if( x1 > w ) x1 = w; if( y1 > h ) y1 = h;
		if( x0 >= x1 || y0 >= y1 ) continue;
		const float l = x0 - 0.5f, rt = x1 - 0.5f, t = y0 - 0.5f, b = y1 - 0.5f;
		const float u0 = (float)x0 / w, u1 = (float)x1 / w, v0 = (float)y0 / h, v1 = (float)y1 / h;
		const V a = { l, t, 0, 1, u0, v0 }, bb = { rt, t, 0, 1, u1, v0 }, cc = { l, b, 0, 1, u0, v1 }, d = { rt, b, 0, 1, u1, v1 };
		q[ nv++ ] = a; q[ nv++ ] = bb; q[ nv++ ] = cc;
		q[ nv++ ] = bb; q[ nv++ ] = d; q[ nv++ ] = cc;
	}
	if( nv ){
		dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
		dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, nv / 3, q, sizeof( V ) );
	}
	Atmos_End( dev, dst, sb );
}

// (th4) DRIFTING SNOW (Ice Age, no config; Mordor 2026-10-11, after a blizzard video: "the snow advection effect above
// ground ... can we have it during storms?" -- fixed to the ground, "start medium"): in a STORM, clouds of snow blown along
// the ground, racing and churning with the wind. SnowDrift.hlsl has the look; here: WHEN, the numbers and the pass.
// WHEN: where the snow ground is (SnowGround_Active: Ice Age Tristram + Ravenholm, not Classic, DrawMode 1, not DDraw)
// while a storm blows (Snowfall_Storm > 0): it builds up and dies away with THE SPELLS' ramp, a calm snowfall has none --
// the pass is skipped then, so it costs nothing outside a storm.
// COST: ALL ON THE GPU (Mordor: the CPU is already busy in the Ice Age town): one window-size pass, no frame copy (SCREEN
// blend straight onto the frame, as the falling snow), five value noises a pixel (~330 ps_3_0 slots); the CPU only sets 8
// constants a frame.
// THE ORDER (PostFxPresent): after the flame halos, before the breath and the falling snow, which stay crisp on top.
// Over world pixels only (the world/UI mask; Atmos_NeedMask asks SnowDrift_Active); inside the town dialog / speech box
// through the full-res WORLD MAP (Grass_BuildMask builds it whenever SnowGround_Active, as for the snow ground).
// Fixed to the ground by Atmos_Anchor / View_Z (THE VIEW: it stays on its ground while the wheel zoom turns).
// KNOWN-GOOD CHECK: Ice Age town, wait for a storm (flat, fast streaks): soft white clouds fade in over the ramp and race to
// the right along the ground, changing shape as they go, plainest over the houses, trees and the snow's blue shadows;
// walking moves them with the ground; pause freezes them; the panels and the dialog's text stay clean; when the calm
// comes back they fade out.
#include "SnowDrift.h"
static IDirect3DPixelShader9* SnowDriftPS[2] = { 0, 0 };
static const float DRIFT_ALPHA = 0.80f;	// the thickest cloud at full storm, 0..1 (Mordor: "medium -> heavy"; medium was 0.55)
static const float DRIFT_COVER = 0.33f;	// where the clouds begin on the noise, SnowDrift.hlsl c1.w: lower = more cloud, fewer gaps (medium 0.40)
static const float DRIFT_RGB[3]       = { 0.80f, 0.82f, 0.86f };	// by day: the mid flakes' shade (Snowfall.cpp SHADE_RGB[ 1 ])
static const float DRIFT_RGB_NIGHT[3] = { 0.59f, 0.64f, 0.78f };	// by night: theirs at night (SHADE_NIGHT_RGB[ 1 ]), by Snowfall_NightQ8
// the noise octaves (SnowDrift.hlsl): world px across one cell, how many times longer it is along the wind, and its share in
// % of the wind's travel (Snowfall_WindTravel, px at the mean wind) -- the fine detail outruns the big puffs: they churn;
// wrap = cells after which the octave repeats exactly: 289 (hash()), 5 x 289 for the TURNED GRIDS (SnowDrift.hlsl ROT_*)
struct DriftOctave { float scale, stretch, windPct; double wrap; };
static const DriftOctave kDriftOctaves[ 4 ] = {
	{ 120.0f, 1.8f, 25.0f, 1445.0 },	// c3.xy / c6.xy: the big puffs (ROT_BIG)
	{ 260.0f, 1.5f, 15.0f,  289.0 },	// c3.zw / c6.zw: the warp that bends them (not turned; it also drifts a little across the wind)
	{  48.0f, 1.8f, 35.0f, 1445.0 },	// c4.xy / c7.xy: the middle octave (ROT_MID)
	{  20.0f, 1.6f, 50.0f, 1445.0 },	// c4.zw / c7.zw: the fine detail (ROT_FINE)
};
static const double DRIFT_WARP_SLANT = 0.3;	// the warp's travel across the wind, x its travel along it
static bool SnowDrift_Active()
{
	return SnowGround_Active() && Snowfall_Storm() > 0.0f;
}
static void SnowDrift_Release( int slot )
{
	if( SnowDriftPS[slot] ){ SnowDriftPS[slot]->Release(); SnowDriftPS[slot] = 0; }
}
static void SnowDrift_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !SnowDrift_Active() || !maskOk || !HeatTex[slot] ) return;
	if( !Atmos_Shader( dev, SnowDriftPS[slot], g_ps30_main_snowdrift ) ) return;
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	float c[ 8 ][ 4 ] = {};
	float ax, ay; Atmos_Anchor( ax, ay );	// fixed to the ground (THE VIEW applied)
	c[ 0 ][ 0 ] = (float)ScreenWidth; c[ 0 ][ 1 ] = (float)ScreenHeight; c[ 0 ][ 2 ] = View_Z();
	c[ 1 ][ 0 ] = ax; c[ 1 ][ 1 ] = ay; c[ 1 ][ 2 ] = DRIFT_ALPHA * Snowfall_Storm(); c[ 1 ][ 3 ] = DRIFT_COVER;
	const float night = Snowfall_NightQ8() / 256.0f;
	for( int k = 0; k < 3; ++k ) c[ 2 ][ k ] = DRIFT_RGB[ k ] + ( DRIFT_RGB_NIGHT[ k ] - DRIFT_RGB[ k ] ) * night;
	const double travel = Snowfall_WindTravel();
	for( int o = 0; o < 4; ++o ){
		const DriftOctave& oc = kDriftOctaves[ o ];
		const double along = 1.0 / ( oc.scale * oc.stretch ), across = 1.0 / oc.scale, t = travel * oc.windPct / 100.0;
		float* off = &c[ 3 + o / 2 ][ ( o & 1 ) * 2 ];
		float* sc = &c[ 6 + o / 2 ][ ( o & 1 ) * 2 ];
		off[ 0 ] = (float)fmod( t * along, oc.wrap );	// where the octave repeats (above): the same picture, no lost precision
		off[ 1 ] = o == 1 ? (float)fmod( t * DRIFT_WARP_SLANT * across, oc.wrap ) : 0.0f;
		sc[ 0 ] = (float)along; sc[ 1 ] = (float)across;
	}
	// the town dialog / NPC speech box (SnowGround_Present has the why): inside it the FULL-RES WORLD MAP, s1, POINT
	int bx0, by0, bx1, by1;
	const bool box = DynZoom_SeeThroughBoxes( &bx0, &by0, &bx1, &by1 ) > 0 && GrassWorldOk[slot] && GrassWorldTex[slot];
	c[ 5 ][ 0 ] = box ? (float)bx0 / ScreenWidth : 2.0f; c[ 5 ][ 1 ] = box ? (float)by0 / ScreenHeight : 2.0f;
	c[ 5 ][ 2 ] = box ? (float)bx1 / ScreenWidth : 2.0f; c[ 5 ][ 3 ] = box ? (float)by1 / ScreenHeight : 2.0f;
	IDirect3DStateBlock9* sb = CaptureStateBlock( AtmosStateBlock[slot], dev );
	dev->SetRenderTarget( 0, dst );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );			// SnowDrift.hlsl writes its colour x its thickness
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );	// SCREEN: frame x ( 1 - drift ) + drift
	dev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 2; ++st ){
		const DWORD f = st == 0 ? D3DTEXF_LINEAR : D3DTEXF_POINT;	// s0 = the mask, soft at the UI's edges; s1 = the world map, exact
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	dev->SetPixelShader( SnowDriftPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[ 0 ][ 0 ], 8 );
	dev->SetTexture( 0, HeatTex[slot] );
	dev->SetTexture( 1, box ? GrassWorldTex[slot] : 0 );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next pass (AtmosStateBlock)
	dev->SetRenderTarget( 0, dst );
}

// (th4) SNOWFALL ON THE GPU (Snowfall.cpp has the design and the numbers): the falling snow of the Ice Age, one INSTANCED
// quad per flake -- SnowFlakesVS.hlsl moves it exactly as the 8-bit flakes moved, SnowFlakes.hlsl draws it as a soft rice
// grain -- blended SCREEN (frame + grain x ( 1 - frame ): only ever brighter) straight onto the finished frame (no copy),
// over world pixels only (the world/UI mask, s0;
// Atmos_NeedMask asks for it). Stream 0 = the quad's 4 corners, stream 1 = the flakes (Snowfall_GpuFlakes: 8 floats each,
// a MANAGED buffer written again only when they change, gen); the vertex shader's constants come from
// Snowfall_GpuConsts every frame. It needs vertex shader 3.0, instancing and hardware vertex processing: without them
// (or if a resource cannot be made) Snowfall_GpuFailed sends the snow back to the 8-bit flakes for the session.
// The first PostFx pass with a VERTEX SHADER: it puts back no shader, no declaration, the default stream frequencies and
// no stream 1 before the state block is applied, so the FVF / XYZRHW passes after it are untouched.
// COST: ~190 000 flakes at 1920 x 1080 and as many storm-only ones (Snowfall.cpp: collapsed while gone), 4 vertices
// each and the grains' few pixels; no CPU work per flake.
#include "SnowFlakes.h"
#include "SnowFlakesVS.h"
static IDirect3DVertexShader9* SnowFlakesVS[2] = { 0, 0 };
static IDirect3DPixelShader9* SnowFlakesPS[2] = { 0, 0 };
static IDirect3DVertexDeclaration9* SnowFlakesDecl[2] = { 0, 0 };
static IDirect3DVertexBuffer9* SnowQuadVB[2] = { 0, 0 };
static IDirect3DVertexBuffer9* SnowInstVB[2] = { 0, 0 };
static IDirect3DIndexBuffer9* SnowQuadIB[2] = { 0, 0 };
static uint SnowInstGen[2] = { 0, 0 };
static int SnowInstCap[2] = { 0, 0 };
static void SnowFlakes_Release( int slot )
{
	if( SnowFlakesVS[slot] ){ SnowFlakesVS[slot]->Release(); SnowFlakesVS[slot] = 0; }
	if( SnowFlakesPS[slot] ){ SnowFlakesPS[slot]->Release(); SnowFlakesPS[slot] = 0; }
	if( SnowFlakesDecl[slot] ){ SnowFlakesDecl[slot]->Release(); SnowFlakesDecl[slot] = 0; }
	if( SnowQuadVB[slot] ){ SnowQuadVB[slot]->Release(); SnowQuadVB[slot] = 0; }
	if( SnowInstVB[slot] ){ SnowInstVB[slot]->Release(); SnowInstVB[slot] = 0; }
	if( SnowQuadIB[slot] ){ SnowQuadIB[slot]->Release(); SnowQuadIB[slot] = 0; }
	SnowInstGen[slot] = 0; SnowInstCap[slot] = 0;
}
// the shaders, the declaration and the quad, once per device; false = this device cannot draw the snow
static bool SnowFlakes_Ensure( LPDIRECT3DDEVICE9 dev, int slot )
{
	if( SnowFlakesVS[slot] && SnowFlakesPS[slot] && SnowFlakesDecl[slot] && SnowQuadVB[slot] && SnowQuadIB[slot] ) return true;
	D3DCAPS9 caps; D3DDEVICE_CREATION_PARAMETERS cp;
	if( D3D_OK != dev->GetDeviceCaps( &caps ) || D3D_OK != dev->GetCreationParameters( &cp ) ) return false;
	if( caps.VertexShaderVersion < D3DVS_VERSION( 3, 0 ) || caps.PixelShaderVersion < D3DPS_VERSION( 3, 0 )
	 || !( cp.BehaviorFlags & ( D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING ) ) ) return false;
	if( !SnowFlakesVS[slot] && D3D_OK != dev->CreateVertexShader( (DWORD*)g_vs30_main_snowflakesvs, &SnowFlakesVS[slot] ) ){ SnowFlakesVS[slot] = 0; return false; }
	if( !SnowFlakesPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_snowflakes, &SnowFlakesPS[slot] ) ){ SnowFlakesPS[slot] = 0; return false; }
	if( !SnowFlakesDecl[slot] ){
		const D3DVERTEXELEMENT9 el[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// the corner
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// x0, y0, speed, layer
			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// churn phases, rates
			D3DDECL_END()
		};
		if( D3D_OK != dev->CreateVertexDeclaration( el, &SnowFlakesDecl[slot] ) ){ SnowFlakesDecl[slot] = 0; return false; }
	}
	if( !SnowQuadVB[slot] ){
		const float corners[ 8 ] = { -1, -1, 1, -1, -1, 1, 1, 1 };
		void* p = 0;
		if( D3D_OK != dev->CreateVertexBuffer( sizeof( corners ), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &SnowQuadVB[slot], 0 ) ){ SnowQuadVB[slot] = 0; return false; }
		if( D3D_OK != SnowQuadVB[slot]->Lock( 0, 0, &p, 0 ) ) return false;
		memcpy( p, corners, sizeof( corners ) ); SnowQuadVB[slot]->Unlock();
	}
	if( !SnowQuadIB[slot] ){
		const ushort idx[ 6 ] = { 0, 1, 2, 2, 1, 3 };
		void* p = 0;
		if( D3D_OK != dev->CreateIndexBuffer( sizeof( idx ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &SnowQuadIB[slot], 0 ) ){ SnowQuadIB[slot] = 0; return false; }
		if( D3D_OK != SnowQuadIB[slot]->Lock( 0, 0, &p, 0 ) ) return false;
		memcpy( p, idx, sizeof( idx ) ); SnowQuadIB[slot]->Unlock();
	}
	return true;
}
static void Snowfall_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	if( !gbRunGame || !Snowfall_GpuWanted() || !maskOk || !HeatTex[slot] ) return;
	if( !SnowFlakes_Ensure( dev, slot ) ){ Snowfall_GpuFailed(); return; }
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	const float* data = 0; uint gen = 0;
	const int n = Snowfall_GpuFlakes( w, h, data, gen );
	if( n <= 0 ) return;
	if( gen != SnowInstGen[slot] || !SnowInstVB[slot] ){	// the flakes changed: into the buffer (bigger if needed)
		if( SnowInstVB[slot] && n > SnowInstCap[slot] ){ SnowInstVB[slot]->Release(); SnowInstVB[slot] = 0; }
		if( !SnowInstVB[slot] ){
			if( D3D_OK != dev->CreateVertexBuffer( (UINT)n * 32, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &SnowInstVB[slot], 0 ) ){ SnowInstVB[slot] = 0; Snowfall_GpuFailed(); return; }
			SnowInstCap[slot] = n;
		}
		void* p = 0;
		if( D3D_OK != SnowInstVB[slot]->Lock( 0, (UINT)n * 32, &p, 0 ) ) return;
		memcpy( p, data, (size_t)n * 32 ); SnowInstVB[slot]->Unlock();
		SnowInstGen[slot] = gen;
	}
	float c[ 16 ][ 4 ]; int nc = 0;
	if( !Snowfall_GpuConsts( w, h, &c[ 0 ][ 0 ], nc ) || nc > 16 ) return;
	IDirect3DStateBlock9* sb = CaptureStateBlock( AtmosStateBlock[slot], dev );
	dev->SetRenderTarget( 0, dst );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );			// SnowFlakes.hlsl writes its colour x its alpha
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );	// SCREEN: frame x ( 1 - grain ) + grain
	dev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// the mask, soft at the UI's edges (as Rain.hlsl)
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE );
	dev->SetTexture( 0, HeatTex[slot] );
	// the town dialog / NPC speech box (SnowGround_Present has the why): inside it the FULL-RES WORLD MAP, s1, POINT
	int bx0, by0, bx1, by1;
	const bool box = DynZoom_SeeThroughBoxes( &bx0, &by0, &bx1, &by1 ) > 0 && GrassWorldOk[slot] && GrassWorldTex[slot];
	const float seeBox[ 4 ] = { box ? (float)bx0 / ScreenWidth : 2.0f, box ? (float)by0 / ScreenHeight : 2.0f,
	                            box ? (float)bx1 / ScreenWidth : 2.0f, box ? (float)by1 / ScreenHeight : 2.0f };
	dev->SetPixelShaderConstantF( 0, seeBox, 1 );
	dev->SetTexture( 1, box ? GrassWorldTex[slot] : 0 );
	dev->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetVertexDeclaration( SnowFlakesDecl[slot] );
	dev->SetVertexShader( SnowFlakesVS[slot] );
	dev->SetPixelShader( SnowFlakesPS[slot] );
	dev->SetVertexShaderConstantF( 0, &c[ 0 ][ 0 ], nc );
	dev->SetStreamSource( 0, SnowQuadVB[slot], 0, 8 );
	dev->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA | (UINT)n );
	dev->SetStreamSource( 1, SnowInstVB[slot], 0, 32 );
	dev->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1u );
	dev->SetIndices( SnowQuadIB[slot] );
	const bool drawn = D3D_OK == dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
	dev->SetStreamSourceFreq( 0, 1 ); dev->SetStreamSourceFreq( 1, 1 );	// back to plain drawing for every pass after this one
	dev->SetStreamSource( 1, 0, 0, 0 ); dev->SetStreamSource( 0, 0, 0, 0 ); dev->SetIndices( 0 );
	dev->SetVertexShader( 0 ); dev->SetVertexDeclaration( 0 );
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next pass (AtmosStateBlock)
	dev->SetRenderTarget( 0, dst );
	if( !drawn ) Snowfall_GpuFailed();
}

// (th4) FLAME HALOS (no config): Flames.cpp notes the light-emitting objects as they are drawn, counts
// how much of each flame is visible in the finished 8-bit frame and lists the halos (its header has the
// whole story); here FlameGlow.hlsl draws them as a soft warm glow over world pixels (world/UI mask).
// Same plumbing as Breath_Present just above: frame copy, MAX 32 halos a frame (FlameGlow.hlsl's
// constant arrays, c1..c32 and c33..c64), ONE SQUARE PER HALO (each pixel sums every halo, opaque write).
static void FlameGlow_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	float pos[32][4], col[32][4];
	const int n = Flames_Halos( &pos[0][0], &col[0][0], 32 );
	if( n <= 0 || !maskOk || !HeatTex[slot] ) return;
	// (th4) DYNAMIC ZOOM (THE VIEW): Flames.cpp lists the halos in the drawn frame -- onto the screen
	for( int i = 0; i < n; ++i ){ View_Point( pos[ i ][ 0 ], pos[ i ][ 1 ] ); pos[ i ][ 2 ] *= ViewZ; }
	if( !Atmos_Shader( dev, FlameGlowPS[slot], g_ps30_main_flameglow ) ) return;
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, false, ok );
	if( !ok ) return;
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( FlameGlowPS[slot] );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	// each halo's square (window px) -- the area its glow can reach
	int hx0[32], hy0[32], hx1[32], hy1[32];
	int bx0 = w, by0 = h, bx1 = 0, by1 = 0;
	const float kx = (float)w / ScreenWidth, ky = (float)h / ScreenHeight;
	for( int i = 0; i < n; ++i ){
		const float rx = pos[ i ][ 2 ] + 1.0f, ry = pos[ i ][ 2 ] / 0.87f + 1.0f;	// FlameGlow.hlsl stretches it 1/0.87 up and down
		int x0 = (int)floorf( ( pos[ i ][ 0 ] - rx ) * kx ), x1 = (int)ceilf( ( pos[ i ][ 0 ] + rx ) * kx );
		int y0 = (int)floorf( ( pos[ i ][ 1 ] - ry ) * ky ), y1 = (int)ceilf( ( pos[ i ][ 1 ] + ry ) * ky );
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0; if( x1 > w ) x1 = w; if( y1 > h ) y1 = h;
		if( x0 >= x1 || y0 >= y1 ){ x0 = x1 = y0 = y1 = 0; }
		hx0[ i ] = x0; hy0[ i ] = y0; hx1[ i ] = x1; hy1[ i ] = y1;
		if( x0 < x1 ){ bx0 = min( bx0, x0 ); by0 = min( by0, y0 ); bx1 = max( bx1, x1 ); by1 = max( by1, y1 ); }
	}
	// (th4) PERFORMANCE: TILES, each pixel shaded ONCE. The old way drew one square per halo and every pixel
	// of every square summed all the halos, so where flames crowd (Hellforge's burning crosses: 72 px halos,
	// up to 32 of them) a pixel was shaded 10..20 times, 32 halos each time. Now the halos' area is cut into
	// FLAME_TILE x FLAME_TILE window px tiles, and each tile is drawn once with only the halos whose square
	// touches it, in their original order -- a halo that does not reach a pixel adds exactly nothing there
	// (FlameGlow.hlsl: r2 >= 1), so the sum, and the picture, are the same as before.
	const int FLAME_TILE = 128;
	float tpos[32][4], tcol[32][4];
	struct V { float x, y, z, rhw, u, v; };
	for( int ty = by0; ty < by1; ty += FLAME_TILE ){
		for( int tx = bx0; tx < bx1; tx += FLAME_TILE ){
			const int X1 = min( tx + FLAME_TILE, bx1 ), Y1 = min( ty + FLAME_TILE, by1 );
			int m = 0, ux0 = X1, uy0 = Y1, ux1 = tx, uy1 = ty;
			for( int i = 0; i < n; ++i ){
				if( hx0[ i ] >= X1 || hx1[ i ] <= tx || hy0[ i ] >= Y1 || hy1[ i ] <= ty ) continue;
				memcpy( tpos[ m ], pos[ i ], sizeof( tpos[ m ] ) ); memcpy( tcol[ m ], col[ i ], sizeof( tcol[ m ] ) ); ++m;
				ux0 = min( ux0, max( tx, hx0[ i ] ) ); uy0 = min( uy0, max( ty, hy0[ i ] ) );
				ux1 = max( ux1, min( X1, hx1[ i ] ) ); uy1 = max( uy1, min( Y1, hy1[ i ] ) );
			}
			if( !m || ux0 >= ux1 || uy0 >= uy1 ) continue;	// only what the halos' squares cover, as before
			const float c0[4] = { (float)ScreenWidth, (float)ScreenHeight, (float)m, 0 };
			dev->SetPixelShaderConstantF( 0, c0, 1 );
			dev->SetPixelShaderConstantF( 1, &tpos[0][0], m );
			dev->SetPixelShaderConstantF( 33, &tcol[0][0], m );
			const float l = ux0 - 0.5f, rt = ux1 - 0.5f, t = uy0 - 0.5f, b = uy1 - 0.5f;
			const float u0 = (float)ux0 / w, u1 = (float)ux1 / w, v0 = (float)uy0 / h, v1 = (float)uy1 / h;
			const V q[6] = { { l, t, 0, 1, u0, v0 }, { rt, t, 0, 1, u1, v0 }, { l, b, 0, 1, u0, v1 },
			                 { rt, t, 0, 1, u1, v0 }, { rt, b, 0, 1, u1, v1 }, { l, b, 0, 1, u0, v1 } };
			dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, q, sizeof( V ) );
		}
	}
	Atmos_End( dev, dst, sb );
}

// (th4) NPC MARKS (config NpcSign): the Diablo II-style "!" bubble over an NPC with something to say. NpcMark.cpp
// has the whole story (who, where, the cover), NpcMark.hlsl the look. NOT a world/UI-mask pass: each mark brings
// its own small COVER (what the UI drew over it, NpcMark_Cover) as a cell of NpcMarkTex, so there is no frame
// copy and no mask pass, and a frame without a mark costs nothing. Premultiplied alpha-blended quads, one per
// mark; called after the breath and before frost / retro / PostFx.hlsl. Classic and the no-shader modes get the
// old 8-bit mark instead (NpcMark.cpp NpcMark_Flush). KNOWN-GOOD CHECK: NpcMark.cpp's header.
static void NpcMark_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot )
{
	const int MAXN = 16;
	float pos[ MAXN ][ 2 ], reach[ 4 ] = { 0, 0, 0, 0 }, scale = 1.0f, redLens[ 3 ] = { 0, 0, 0 }, redInk[ 3 ] = { 0, 0, 0 };
	int box[ MAXN ][ 4 ];
	bool red = false;
	const int n = NpcMark_List( pos, box, MAXN, reach, scale, red, redLens, redInk );
	if( n <= 0 ) return;
	if( !NpcMarkPS[ slot ] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_npcmark, &NpcMarkPS[ slot ] ) ){
		NpcMarkPS[ slot ] = 0; NpcMark_GpuFailed(); return;	// the 8-bit mark from the next frame on
	}
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	const float kx = (float)w / ScreenWidth, ky = (float)h / ScreenHeight;
	// the cover cells: 4 a row, each as big as the largest box plus 1 px of "world" all round, so the linear
	// filter never reads a neighbour's cell
	int cw = 0, ch = 0;
	for( int i = 0; i < n; ++i ){ cw = max( cw, box[ i ][ 2 ] ); ch = max( ch, box[ i ][ 3 ] ); }
	cw += 2; ch += 2;
	const int tw = ( n < 4 ? n : 4 ) * cw, th = ( ( n + 3 ) / 4 ) * ch;
	LPDIRECT3DTEXTURE9& tex = NpcMarkTex[ slot ];
	if( !tex || NpcMarkTexW[ slot ] < tw || NpcMarkTexH[ slot ] < th ){	// only ever grows
		const int nw = ( max( tw, NpcMarkTexW[ slot ] ) + 63 ) & ~63, nh = ( max( th, NpcMarkTexH[ slot ] ) + 63 ) & ~63;
		if( tex ){ tex->Release(); tex = 0; }
		NpcMarkTexW[ slot ] = NpcMarkTexH[ slot ] = 0;
		if( D3D_OK != dev->CreateTexture( nw, nh, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &tex, 0 ) ){ tex = 0; return; }
		NpcMarkTexW[ slot ] = nw; NpcMarkTexH[ slot ] = nh;
	}
	const float TW = (float)NpcMarkTexW[ slot ], TH = (float)NpcMarkTexH[ slot ];
	D3DLOCKED_RECT lr;
	if( D3D_OK != tex->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return;
	uchar* base = (uchar*)lr.pBits;
	for( int y = 0; y < th; ++y ) memset( base + (size_t)y * lr.Pitch, 0, tw );
	for( int i = 0; i < n; ++i ) NpcMark_Cover( i, base + (size_t)( ( i / 4 ) * ch + 1 ) * lr.Pitch + ( i % 4 ) * cw + 1, lr.Pitch, UiRects, UiRectN );
	tex->UnlockRect( 0 );
	IDirect3DStateBlock9* sb = CaptureStateBlock( AtmosStateBlock[ slot ], dev );
	dev->SetRenderTarget( 0, dst );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );			// premultiplied (NpcMark.hlsl)
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE );
	dev->SetPixelShader( NpcMarkPS[ slot ] );
	dev->SetTexture( 0, tex );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	const float fade = PaletteFadeLevel >= 256 ? 1.0f : PaletteFadeLevel / 256.0f;	// palette fades (warps, the ESC menu)
	struct V { float x, y, z, rhw, u, v; };
	for( int i = 0; i < n; ++i ){
		float cx = pos[ i ][ 0 ], cy = pos[ i ][ 1 ];
		View_Point( cx, cy );						// THE VIEW (DYNAMIC ZOOM)
		const float s = scale * ViewZ;				// game px per 1x px of the mark: it grows with Alt+Z / the wheel
		// the quad: the mark's reach, window px, clipped to the window
		int X0 = (int)floorf( ( cx - reach[ 0 ] * s ) * kx ), Y0 = (int)floorf( ( cy - reach[ 1 ] * s ) * ky );
		int X1 = (int)ceilf( ( cx + reach[ 2 ] * s ) * kx ), Y1 = (int)ceilf( ( cy + reach[ 3 ] * s ) * ky );
		if( X0 < 0 ) X0 = 0; if( Y0 < 0 ) Y0 = 0; if( X1 > w ) X1 = w; if( Y1 > h ) Y1 = h;
		if( X0 >= X1 || Y0 >= Y1 ) continue;
		// the cover box was placed for the frame as drawn; a present that shows the world elsewhere (the second
		// window under the wheel zoom, a fade frame after it) skips the mark rather than cover it wrongly
		const float bx = (float)box[ i ][ 0 ], by = (float)box[ i ][ 1 ];
		if( X0 / kx < bx - 0.5f || X1 / kx > bx + box[ i ][ 2 ] + 0.5f || Y0 / ky < by - 0.5f || Y1 / ky > by + box[ i ][ 3 ] + 0.5f ) continue;
		// c0: one window px in the mark's px (edge softness), the fade, the red screen; c3: this mark's cover
		// cell -- its px = cell origin + game px - box origin, game px = centre + mark px * s
		const float cellX = (float)( ( i % 4 ) * cw + 1 ), cellY = (float)( ( i / 4 ) * ch + 1 );
		const float c[ 4 ][ 4 ] = {
			{ 1.0f / ( s * min( kx, ky ) ), fade, red ? 1.0f : 0.0f, 0 },
			{ redLens[ 0 ], redLens[ 1 ], redLens[ 2 ], 0 },
			{ redInk[ 0 ], redInk[ 1 ], redInk[ 2 ], 0 },
			{ s / TW, s / TH, ( cellX + cx - bx ) / TW, ( cellY + cy - by ) / TH },
		};
		dev->SetPixelShaderConstantF( 0, &c[ 0 ][ 0 ], 4 );
		// t0 = the mark's own px at each corner (a window edge X is game px X / kx)
		const float u0 = ( X0 / kx - cx ) / s, u1 = ( X1 / kx - cx ) / s, v0 = ( Y0 / ky - cy ) / s, v1 = ( Y1 / ky - cy ) / s;
		const V q[ 4 ] = { { X0 - 0.5f, Y0 - 0.5f, 0, 1, u0, v0 }, { X1 - 0.5f, Y0 - 0.5f, 0, 1, u1, v0 },
		                   { X0 - 0.5f, Y1 - 0.5f, 0, 1, u0, v1 }, { X1 - 0.5f, Y1 - 0.5f, 0, 1, u1, v1 } };
		dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, q, sizeof( V ) );
	}
	dev->SetTexture( 0, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next pass (AtmosStateBlock)
	dev->SetRenderTarget( 0, dst );
}

// (th4) MOSSY GROTTO "TRIP" (config GrottoTrip, default on; off = the map uses the player's normal
// ChromaticAberration setting like any other level -- purely local, nothing synced):
// in DUN_52_MOSSY_GROTTO chromatic aberration is FORCED on,
// overriding the player's ChromaticAberration setting (even "off"), and grows the closer the
// player stands to a Fungal Root (BM_289_FUNGAL_ROOT / BM_290_FUNGAL_ROOT_LITE, the static
// "shrieker" mushrooms placed on that map):
//   distance d = tiles to the nearest Fungal Root, diagonals count as 1 (melee range = 1)
//   CA = 11 - d, floored at 2   -> 10 in melee range, 9 at 2 tiles, ... 2 at 9+ tiles / none
//   STACKING: every OTHER Fungal Root within 9 tiles adds +1 on top, so standing among a
//   cluster goes past 10. Capped at GROTTO_CA_MAX (pixels of split at the screen corners).
// Dead Fungal Roots count exactly like living ones (design choice): living ones are found in
// MonsterMap, dead ones by their corpse in DeathMonstersMap, whose low byte - 1 indexes dead[]
// (Main.cpp InitDead) and dead[].baseMonsterIndex names the monster type.
// Only the 19x19 cells around the player are scanned, once per frame.
static int GrottoTripCA()
{
	const Player& p = Players[CurrentPlayerIndex];
	int best = 99, count = 0;
	for( int dr = -9; dr <= 9; ++dr ){
		for( int dc = -9; dc <= 9; ++dc ){
			const int r = p.Row + dr, c = p.Col + dc;
			if( r < 0 || c < 0 || r >= FineMap_112 || c >= FineMap_112 ) continue;
			const int d = max( abs( dr ), abs( dc ) );
			int b = -1;
			const int mv = MonsterMap[ r ][ c ];
			if( mv ){
				const int mi = abs( mv ) - 1;
				if( mi >= 0 && mi < Monsters_Max_Count && Monsters[ mi ].SpritePtr ) b = Monsters[ mi ].SpritePtr->baseMonsterIndex;
			}
			if( b != BM_289_FUNGAL_ROOT && b != BM_290_FUNGAL_ROOT_LITE ){
				const ushort dv = DeathMonstersMap[ r ][ c ];
				const int di = ( dv & 0xff ) - 1;
				if( dv && di >= 0 && di < DeadMonsters_Sprites_Max_Count ) b = dead[ di ].baseMonsterIndex;
			}
			if( b == BM_289_FUNGAL_ROOT || b == BM_290_FUNGAL_ROOT_LITE ){
				++count;
				if( d < best ) best = d;
			}
		}
	}
	const int GROTTO_CA_MAX = 30;
	int ca = 11 - best;
	if( ca < 2 ) ca = 2;
	if( count > 1 ) ca += count - 1;	// +1 for each Fungal Root beyond the nearest
	return ca > GROTTO_CA_MAX ? GROTTO_CA_MAX : ca;
}

// (th4) RIVER FLOW -- THE RUNNING RIVER of Tristram (config WaterShimmer: the same switch, the same sway as the ripple).
// Mordor: the river "ripples like a swamp" -- PostFx.hlsl's ripple sways every water pixel around its own spot, which is
// right for still water (the cave water maps, Ravenholm's stagnant river) but not for a river. Here the town river's
// texture SLIDES downstream and its ripples travel with it: River.hlsl (its header has how), one window-size pass run
// where PostFx.hlsl's ripple acted (after the retro modes, before the PostFx.hlsl pass), which then skips the water
// (PostFxPresent: waterRipple). The direction at every spot: TownWater.cpp THE FLOW MAP (each stream runs to its left
// end, along its bends). The numbers:
//   RIVER_SPEED      world px per second (a hero is ~80 px tall: an easy walking pace for the water)
//   RIVER_CYCLE_SEC  one cycle of the slide: each copy of the water travels RIVER_SPEED x this, then jumps back
//                    unseen. The whole river is in one phase, so it pulses faintly in step (half way through a cycle
//                    the two copies blend half and half, RIVER_SPEED x this / 2 px apart): shorter = fainter pulse.
//   RIVER_WAVES      the ripple's three waves, as whole numbers of waves per turn of the STREAM PHASE (TownWater.cpp
//                    RIVER_WAVE_PERIOD world px): 13 / 6 / 9 = ~20 / 43 / 28 px long. Whole numbers because the phase
//                    is stored wrapped -- anything else shows a seam where it wraps. Their time phases (c5) are
//                    wrapped to 0..2 pi HERE, in double: the shader's sin() of an ever-growing clock loses precision.
// Ice Age: frozen, no mask water at all (TownWater_RiverRipples). Never in Classic (no mask water either).
static const float RIVER_SPEED     = 10.0f;
static const float RIVER_CYCLE_SEC = 0.8f;
static const float RIVER_WAVES[ 3 ] = { 13.0f, 6.0f, 9.0f };
static bool River_Present( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int slot, bool maskOk )
{
	int wa = WaterShimmer; if( wa > 10 ) wa = 10;
	if( !maskOk || wa <= 0 || !HeatTex[slot] ) return false;
	const uint* flowTexels = 0; uint gen = 0; int c00[2]; float wavePeriod = 256.0f;
	if( !TownWater_RiverFlow( flowTexels, gen, c00, wavePeriod ) || !flowTexels ) return false;
	if( !Atmos_Shader( dev, RiverPS[slot], g_ps30_main_river ) ) return false;
	if( RiverFlowUp[slot] != gen || !RiverFlowTex[slot] ){	// the flow map changes only with the town
		if( !RiverFlowTex[slot] && D3D_OK != dev->CreateTexture( 112, 112, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RiverFlowTex[slot], 0 ) ){ RiverFlowTex[slot] = 0; return false; }
		D3DLOCKED_RECT lr;
		if( D3D_OK != RiverFlowTex[slot]->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
		for( int y = 0; y < 112; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, flowTexels + y * 112, 112 * 4 );
		RiverFlowTex[slot]->UnlockRect( 0 );
		RiverFlowUp[slot] = gen;
	}
	int w, h; bool ok;
	IDirect3DStateBlock9* sb = Atmos_Begin( dev, dst, slot, w, h, true, ok );	// s0 LINEAR: the offsets are sub-pixel
	if( !ok ) return false;
	for( int st = 1; st <= 2; ++st ){	// s1 the mask (codes: exact texels), s2 the flow map (blends between cells)
		const DWORD f = st == 1 ? D3DTEXF_POINT : D3DTEXF_LINEAR;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	}
	const float altZ = IsZoomDisable ? 1.0f : 2.0f;
	float pa = 0, pb = 0; Perspective_GetParams( pa, pb );
	float wavePh[ 3 ];	// each wave's fronts move downstream at RIVER_SPEED: phase = waves x 2 pi / period x speed x t, wrapped
	for( int i = 0; i < 3; ++i ) wavePh[ i ] = (float)fmod( RIVER_WAVES[ i ] * 6.283185307179586 / wavePeriod * RIVER_SPEED * (double)AtmosSec, 6.283185307179586 );
	const float c[7][4] = {
		{ (float)ScreenWidth, (float)ScreenHeight, 1.0f / ViewZ, AtmosSec },
		{ ViewSX, ViewSY, altZ, wa * 0.35f * (float)w / ScreenWidth * ViewZ },	// the sway: PostFx.hlsl's water sway (PostFxPresent c7.z)
		{ c00[0] * altZ, c00[1] * altZ, RIVER_SPEED, 1.0f / RIVER_CYCLE_SEC },
		{ ViewZ * altZ * (float)w / ScreenWidth, ViewZ * altZ * (float)h / ScreenHeight, RIVER_SPEED * RIVER_CYCLE_SEC, 0 },
		{ pa, pb, 1.0f / w, 1.0f / h },
		{ wavePh[ 0 ], wavePh[ 1 ], wavePh[ 2 ], 0 },
		{ RIVER_WAVES[ 0 ], RIVER_WAVES[ 1 ], RIVER_WAVES[ 2 ], 0 },
	};
	dev->SetRenderTarget( 0, dst );
	dev->SetPixelShader( RiverPS[slot] );
	dev->SetPixelShaderConstantF( 0, &c[0][0], 7 );
	dev->SetTexture( 2, RiverFlowTex[slot] );
	Fsr_Quad( dev, w, h );
	Atmos_End( dev, dst, sb );
	return true;
}

static void PostFxPresent( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst )
{
	int ab = ChromaticAberration, gr = FilmGrain, hz = HeatHaze;
	int wa = WaterShimmer, tl = TiltShift, cr = CrtEffect, sw = Shockwave;
	if( wa > 10 ) wa = 10; if( tl > 10 ) tl = 10; if( cr > 10 ) cr = 10; if( sw > 10 ) sw = 10;
	const bool grottoTrip = GrottoTrip && Dungeon == DUN_52_MOSSY_GROTTO;
	if( grottoTrip ) ab = GrottoTripCA();	// forced, see GrottoTripCA
	if( ab < 0 ) ab = 0;
	if( ab > 10 && !grottoTrip ) ab = 10;	// the grotto trip may go past 10 (GrottoTripCA caps it)
	if( gr < 0 ) gr = 0; if( gr > 10 ) gr = 10;
	if( hz < 0 ) hz = 0; if( hz > 10 ) hz = 10;
	if( !dev || !dst ) return;
	View_Begin( dev );	// (th4) DYNAMIC ZOOM: THE VIEW of this frame, for every pass below
	const int slotEarly = ( dev == D3dDevice ) ? 0 : 1;
	Wet_BuildMask( dev, slotEarly );	// (th4) WetGround: before HeatHaze_BuildMask uses up the world snapshot
	Grass_BuildMask( dev, slotEarly );	// (th4) GrassBlades: its full-resolution WORLD MAP, from the same snapshot
	IceRiver_BuildMask( dev, slotEarly );	// (th4) ICE RIVER: its map, from the same snapshot
	const bool mask = HeatHaze_BuildMask( dev, slotEarly );	// world/UI (+lava or water) mask, if any effect needs it
	Atmos_Tick();	// (th4) ATMOSPHERE clock, every frame (stops on pause)
	++PostFxFrame; if( !PostFxFrame ) PostFxFrame = 1;	// (th4) frame number for SwayLayers_Upload (0 = never uploaded)
	Grass_Present( dev, dst, slotEarly );	// (th4) GrassBlades: blades of grass, first: their colours come from the frame BEFORE any pass (THE GAME-RESOLUTION FRAME)
	Wet_Present( dev, dst, slotEarly );	// (th4) WetGround: the wet town and its puddles -- over the blades too, so they are as wet as the ground
	Clouds_Present( dev, dst, slotEarly, mask );	// (th4) CloudShadows, on the ground under everything else
	SnowGround_Present( dev, dst, slotEarly, mask );	// (th4) SNOW GROUND: Ice Age; never with the clouds (Clouds_Active excludes the Ice Age)
	IceRiver_Present( dev, dst, slotEarly );	// (th4) ICE RIVER: the Ice Age river mirrors a winter sky by day (own map, not the world/UI mask)
	Mist_Present( dev, dst, slotEarly, mask );	// (th4) town ground mist at night, before the PostFx.hlsl effects
	Rain_Present( dev, dst, slotEarly, mask );	// (th4) storm-night rain (never on a mist night), same slot
	BioGlow_Present( dev, dst, slotEarly, mask );	// (th4) soft halo of a bioluminescent night
	TreeSway_Present( dev, dst, slotEarly, mask );	// (th4) TreeSway: after the glow pass, so recoloured crowns move whole
	const bool rippleWater = mask && wa > 0 && PostFx_LevelHasWater();	// (th4) the PostFx.hlsl ripple will move the water (the 'water' below)
	SunRays_Present( dev, dst, slotEarly, mask, rippleWater ? 1 : 0 );	// (th4) SunRays: sunny days, light through the (swayed) crowns; water after the ripple
	Trails_Present( dev, dst, slotEarly, mask );	// (th4) SpellTrails
	LevelUp_Present( dev, dst, slotEarly, mask );	// (th4) LevelUpGlow
	FlameGlow_Present( dev, dst, slotEarly, mask );	// (th4) halos around torches / braziers / candles (Flames.cpp)
	SnowDrift_Present( dev, dst, slotEarly, mask );	// (th4) DRIFTING SNOW: an Ice Age storm's clouds of snow along the ground, under the breath and the flakes
	Breath_Present( dev, dst, slotEarly, mask );	// (th4) ColdBreath, puffs of vapour (Breath.cpp)
	Snowfall_Present( dev, dst, slotEarly, mask );	// (th4) SNOWFALL ON THE GPU: the falling snow, in front of the world, under the UI
	NpcMark_Present( dev, dst, slotEarly );	// (th4) the "!" bubble over NPCs with something to say (NpcMark.cpp), under frost / retro / PostFx.hlsl
	Frost_Present( dev, dst, slotEarly, mask );	// (th4) Ice Age frost creeping in from the screen edges
	Retro_Present( dev, dst, slotEarly );	// (th4) RetroMode, whole screen; before PostFx.hlsl so CRT etc. lay on top
	const bool riverRan = River_Present( dev, dst, slotEarly, mask );	// (th4) RIVER FLOW: the town river runs (its own pass); PostFx.hlsl then leaves its water alone
	const bool haze = mask && hz > 0 && HeatHaze_LevelHasLava();
	const bool water = mask && wa > 0 && PostFx_LevelHasWater();
	const bool waterRipple = water && !riverRan;	// (th4) RIVER FLOW did the town river already
	const bool tilt = mask && tl > 0;
	const bool shock = mask && sw > 0 && Shockwave_Active();
	if( !ab && !gr && !haze && !waterRipple && !tilt && !shock && cr <= 0 ){
		if( water ) SunRays_Present( dev, dst, slotEarly, mask, 2 );	// (th4) the sun on the water still comes after it moved (see the end)
		return;
	}
	D3DSURFACE_DESC dd;
	if( D3D_OK != dst->GetDesc( &dd ) ) return;
	const int w = (int)dd.Width, h = (int)dd.Height;
	const int slot = ( dev == D3dDevice ) ? 0 : 1;
	if( !PostFxPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_postfx, &PostFxPS[slot] ) ){ PostFxPS[slot] = 0; return; }
	if( !Fsr_EnsureTex( dev, PostTex[slot], PostTexW[slot], PostTexH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return;
	LPDIRECT3DSURFACE9 ts = 0;
	if( D3D_OK != PostTex[slot]->GetSurfaceLevel( 0, &ts ) ) return;
	if( D3D_OK != dev->StretchRect( dst, 0, ts, 0, D3DTEXF_NONE ) ){ ts->Release(); return; }
	ts->Release();
	IDirect3DStateBlock9* sb = CaptureStateBlock( PostFxStateBlock[slot], dev );
	dev->SetRenderTarget( 0, dst );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// sub-pixel channel offsets
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE );
	static unsigned frame = 0; ++frame;
	// aberration: config step = 1 px at the corners; grain: config step = 0.02 of full range
	const float c0[4] = { 1.0f / w, 1.0f / h, (float)ab, gr * 0.02f };
	// haze: config step = 0.6 game pixels of sway at full heat, scaled to the window
	const float hazePx = haze ? hz * 0.6f * (float)w / ScreenWidth * ViewZ : 0.0f;	// (th4) x the wheel zoom: the same sway on the bigger lava
	const float c1[4] = { (float)( frame % 997 ), hazePx, (float)( GetTickCount() % 600000 ) / 1000.0f, 0 };
	const float c2[4] = { 2.0f / ScreenWidth, 2.0f / ScreenHeight, 1.0f / ViewZ, 0 };	// heat mask texel = 2x2 game pixels (HeatHaze_BuildMask); z: 1 / the DYNAMIC ZOOM (c8.zw its shift)
	// shockwave rings c3..c6: centre in uv, radius in uv (height units), strength in uv
	float rings[4][4] = {};
	if( shock ){
		const DWORD now = GetTickCount();
		for( int i = 0; i < 4; ++i ){
			const ShockRing& r = ShockRings[i];
			if( !r.used || now - r.t0 >= SHOCK_MS ) continue;
			const float t = (float)( now - r.t0 ) / SHOCK_MS;		// 0..1
			// tile -> screen: Atmos_TileToScreen, the formula the Alt ground labels use (automap.cpp), then the
			// Perspective warp and THE VIEW. The formula gives the tile's GROUND point; an explosion sprite is
			// drawn upwards from its ground point, so its visual centre sits higher. SHOCK_LIFT_PX lifts the
			// ring to that centre (one tile's height); raise/lower it if rings look off vertically.
			const int SHOCK_LIFT_PX = 32;
			float sx, sy;
			Atmos_TileToScreen( r.row, r.col, r.ox, r.oy - SHOCK_LIFT_PX, sx, sy );
			rings[i][0] = ( sx + 0.5f ) / ScreenWidth;
			rings[i][1] = ( sy + 0.5f ) / ScreenHeight;
			rings[i][2] = t * 0.28f * ViewZ;					// grows to ~28% of the screen height (x the wheel zoom)
			rings[i][3] = sw * 0.0022f * ( 1.0f - t ) * ( 1.0f - t ) * ViewZ;	// fades out quickly
		}
	}
	// c7: tilt-shift amount (0..1), CRT amount (0..1), water sway in window px, aspect w/h
	const float c7[4] = { tilt ? tl * 0.1f : 0.0f, cr > 0 ? cr * 0.1f : 0.0f, waterRipple ? wa * 0.35f * (float)w / ScreenWidth * ViewZ : 0.0f, (float)w / h };
	// c8: game-frame height (scanlines follow game pixel rows), mask valid flag
	const float c8[4] = { (float)ScreenHeight, mask ? 1.0f : 0.0f, ViewSX, ViewSY };	// zw: the DYNAMIC ZOOM shift (c2.z)
	// c9: WORLD ANCHOR for the water ripple -- position (game px) of tile (0,0) in the DRAWN frame, via
	// the same tile->screen formula as the Alt ground labels (automap.cpp), plus the Perspective
	// coefficients so the shader can undo the warp (after taking the DYNAMIC ZOOM off: c2.z, c8.zw).
	// drawn position - anchor = a fixed world position,
	// so the ripple belongs to the water itself: scrolling, the cursor and monsters walking
	// over it cannot change its pattern.
	float c9[4] = { 0, 0, 0, 0 };
	if( water ){
		Atmos_AnchorDrawn( c9[0], c9[1] );	// in the DRAWN frame: the shader takes the wheel zoom off first (c2.z, c8.zw)
		Perspective_GetParams( c9[2], c9[3] );
	}
	dev->SetPixelShader( PostFxPS[slot] );
	dev->SetPixelShaderConstantF( 0, c0, 1 );
	dev->SetPixelShaderConstantF( 1, c1, 1 );
	dev->SetPixelShaderConstantF( 2, c2, 1 );
	dev->SetPixelShaderConstantF( 3, &rings[0][0], 4 );
	dev->SetPixelShaderConstantF( 7, c7, 1 );
	dev->SetPixelShaderConstantF( 8, c8, 1 );
	dev->SetPixelShaderConstantF( 9, c9, 1 );
	dev->SetTexture( 0, PostTex[slot] );
	dev->SetTexture( 1, mask ? HeatTex[slot] : 0 );
	dev->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	dev->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	Fsr_Quad( dev, w, h );
	dev->SetTexture( 0, 0 );
	dev->SetTexture( 1, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next frame (PostFxStateBlock)
	dev->SetRenderTarget( 0, dst );
	if( water ) SunRays_Present( dev, dst, slot, mask, 2 );	// (th4) SunRays on the rippling water, after the ripple: the light stays still (SUN RAYS)
}
// (th4) ANTIALIASING (config "Antialiasing": 0 off, 1 FXAA, 2 SMAA).
// WHICH PASS THIS IS: one of three optional full-frame passes in the two textured-quad present
// paths (RenderQuadTexture = palette, RenderQuadTextureUltra = Ultra), in this order:
//   scene quad + automap + orbs + item icons -> AntialiasPresent (GAME resolution, in place)
//   -> FsrPresent or SharpStretch (upscale to the window) -> PostFxPresent (window size, last).
// AA runs before the upscale on purpose: edges are found on the real pixels, it costs a
// fraction of a window-size pass, and FSR then scales an already smooth frame. Surface mode
// (DrawMode 0) has no shaders and no AA.
// It replaced FullscreenFX 6: that FXAA was a SCENE shader (FXAAShader*.hlsl, palette lookup +
// blur in one), so it excluded vignette / fog / bloom. Config.cpp maps an old FullscreenFX 6 to
// Antialiasing 1 and FullscreenFX 0; dx.cpp SwitchVirtualShader no longer selects it.
//   FXAA (AaFxaa.hlsl): one pass, the old 5-tap filter, on the finished RGB frame.
//   SMAA (SmaaEdge / SmaaWeights / SmaaBlend.hlsl over the unmodified reference SMAA.hlsli,
//   ULTRA preset, DX9 flavour): colour edges -> AaEdgeTex, blending weights -> AaBlendTex (with
//   the precomputed SmaaAreaTex.h / SmaaSearchTex.h lookup textures), neighbourhood blend back
//   into the frame. The sampler filters below are part of the algorithm (see each wrapper).
// The frame is copied to AaSrcTex first (the target cannot be read while written). Resources
// are per device (slot 0 = D3dDevice, 1 = SecDevice) and released with the other default-pool
// objects in SharpScale_Release (dx.cpp calls it on reset / shutdown). The area/search textures
// are MANAGED (filled once from the headers) and released there too, simply to keep one
// lifetime for everything in this block.
// KNOWN-GOOD CHECK: a diagonal wall edge at SMAA shows a smooth ramp with the wall and floor
// texture beside it still sharp; FXAA softens a little more. If SMAA shows no change at all, a
// shader failed to create (check the fxc output for the three Smaa*.hlsl).
#include "AaFxaa.h"
#include "SmaaEdge.h"
#include "SmaaWeights.h"
#include "SmaaBlend.h"
#include "SmaaAreaTex.h"
#include "SmaaSearchTex.h"
static IDirect3DPixelShader9* AaFxaaPS[2] = { 0, 0 };
static IDirect3DPixelShader9* SmaaEdgePS[2] = { 0, 0 };
static IDirect3DPixelShader9* SmaaWeightsPS[2] = { 0, 0 };
static IDirect3DPixelShader9* SmaaBlendPS[2] = { 0, 0 };
static LPDIRECT3DTEXTURE9 AaSrcTex[2] = { 0, 0 }, AaEdgeTex[2] = { 0, 0 }, AaBlendTex[2] = { 0, 0 };
static LPDIRECT3DTEXTURE9 SmaaAreaTex[2] = { 0, 0 }, SmaaSearchTex[2] = { 0, 0 };
static int AaSrcW[2], AaSrcH[2], AaEdgeW[2], AaEdgeH[2], AaBlendW[2], AaBlendH[2];

static void Antialias_Release( int slot )
{
	IDirect3DPixelShader9** ps[] = { &AaFxaaPS[slot], &SmaaEdgePS[slot], &SmaaWeightsPS[slot], &SmaaBlendPS[slot] };
	for( auto p : ps ) if( *p ){ (*p)->Release(); *p = 0; }
	LPDIRECT3DTEXTURE9* tx[] = { &AaSrcTex[slot], &AaEdgeTex[slot], &AaBlendTex[slot], &SmaaAreaTex[slot], &SmaaSearchTex[slot] };
	for( auto t : tx ) if( *t ){ (*t)->Release(); *t = 0; }
	AaSrcW[slot] = AaSrcH[slot] = AaEdgeW[slot] = AaEdgeH[slot] = AaBlendW[slot] = AaBlendH[slot] = 0;
}

// Upload one of the SMAA lookup tables (rows of 'pitch' bytes) into a managed texture.
static bool Smaa_MakeLookup( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9& tex, int w, int h, D3DFORMAT fmt, const unsigned char* bytes, int pitch )
{
	if( tex ) return true;
	if( D3D_OK != dev->CreateTexture( w, h, 1, 0, fmt, D3DPOOL_MANAGED, &tex, 0 ) ){ tex = 0; return false; }
	D3DLOCKED_RECT lr;
	if( D3D_OK != tex->LockRect( 0, &lr, 0, 0 ) ){ tex->Release(); tex = 0; return false; }
	for( int y = 0; y < h; ++y ) memcpy( (uchar*)lr.pBits + y * lr.Pitch, bytes + y * pitch, pitch );
	tex->UnlockRect( 0 );
	return true;
}

static void Aa_Sampler( LPDIRECT3DDEVICE9 dev, int stage, bool linear )
{
	const DWORD f = linear ? D3DTEXF_LINEAR : D3DTEXF_POINT;
	dev->SetSamplerState( stage, D3DSAMP_MINFILTER, f );
	dev->SetSamplerState( stage, D3DSAMP_MAGFILTER, f );
	dev->SetSamplerState( stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	dev->SetSamplerState( stage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( stage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	dev->SetSamplerState( stage, D3DSAMP_SRGBTEXTURE, FALSE );
}

// Antialiases 'surf' (the render target the scene quad was drawn into) in place. Leaves the
// device's render target and states as it found them. Returns false when off or on any failure
// (the frame is then simply not antialiased).
static bool AntialiasPresent( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 surf )
{
	const int mode = Antialiasing;
	if( mode <= 0 || !dev || !surf ) return false;
	D3DSURFACE_DESC sd;
	if( D3D_OK != surf->GetDesc( &sd ) ) return false;
	const int w = (int)sd.Width, h = (int)sd.Height;
	const int slot = ( dev == D3dDevice ) ? 0 : 1;
	const bool smaa = mode >= 2;
	if( smaa ){
		if( !SmaaEdgePS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_smaa_edge, &SmaaEdgePS[slot] ) ){ SmaaEdgePS[slot] = 0; return false; }
		if( !SmaaWeightsPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_smaa_weights, &SmaaWeightsPS[slot] ) ){ SmaaWeightsPS[slot] = 0; return false; }
		if( !SmaaBlendPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_smaa_blend, &SmaaBlendPS[slot] ) ){ SmaaBlendPS[slot] = 0; return false; }
		if( !Smaa_MakeLookup( dev, SmaaAreaTex[slot], AREATEX_WIDTH, AREATEX_HEIGHT, D3DFMT_A8L8, areaTexBytes, AREATEX_PITCH ) ) return false;
		if( !Smaa_MakeLookup( dev, SmaaSearchTex[slot], SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, D3DFMT_L8, searchTexBytes, SEARCHTEX_PITCH ) ) return false;
		if( !Fsr_EnsureTex( dev, AaEdgeTex[slot], AaEdgeW[slot], AaEdgeH[slot], w, h, D3DFMT_A8R8G8B8 ) ) return false;
		if( !Fsr_EnsureTex( dev, AaBlendTex[slot], AaBlendW[slot], AaBlendH[slot], w, h, D3DFMT_A8R8G8B8 ) ) return false;
	}else{
		if( !AaFxaaPS[slot] && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_aafxaa, &AaFxaaPS[slot] ) ){ AaFxaaPS[slot] = 0; return false; }
	}
	if( !Fsr_EnsureTex( dev, AaSrcTex[slot], AaSrcW[slot], AaSrcH[slot], w, h, D3DFMT_X8R8G8B8 ) ) return false;

	LPDIRECT3DSURFACE9 srcSurf = 0, prevRT = 0;
	if( D3D_OK != AaSrcTex[slot]->GetSurfaceLevel( 0, &srcSurf ) ) return false;
	bool ok = D3D_OK == dev->StretchRect( surf, 0, srcSurf, 0, D3DTEXF_NONE );
	srcSurf->Release();
	if( !ok ) return false;
	if( D3D_OK != dev->GetRenderTarget( 0, &prevRT ) ) prevRT = 0;

	IDirect3DStateBlock9* sb = CaptureStateBlock( AaStateBlock[slot], dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	const float metrics[4] = { 1.0f / w, 1.0f / h, (float)w, (float)h };	// c0 in every AA shader
	dev->SetPixelShaderConstantF( 0, metrics, 1 );
	if( !smaa ){
		ok = D3D_OK == dev->SetRenderTarget( 0, surf );
		if( ok ){
			dev->SetPixelShader( AaFxaaPS[slot] );
			Aa_Sampler( dev, 0, false );
			dev->SetTexture( 0, AaSrcTex[slot] );
			Fsr_Quad( dev, w, h );
		}
	}else{
		LPDIRECT3DSURFACE9 edgeSurf = 0, blendSurf = 0;
		ok = D3D_OK == AaEdgeTex[slot]->GetSurfaceLevel( 0, &edgeSurf );
		if( ok && D3D_OK != AaBlendTex[slot]->GetSurfaceLevel( 0, &blendSurf ) ) ok = false;
		// pass 1: colour edges (discards non-edges, so the target is cleared first)
		if( ok && D3D_OK == dev->SetRenderTarget( 0, edgeSurf ) ){
			dev->Clear( 0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0 );
			dev->SetPixelShader( SmaaEdgePS[slot] );
			Aa_Sampler( dev, 0, false );
			dev->SetTexture( 0, AaSrcTex[slot] );
			Fsr_Quad( dev, w, h );
		}else ok = false;
		// pass 2: blending weights
		if( ok && D3D_OK == dev->SetRenderTarget( 0, blendSurf ) ){
			dev->Clear( 0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0 );
			dev->SetPixelShader( SmaaWeightsPS[slot] );
			Aa_Sampler( dev, 0, true );  dev->SetTexture( 0, AaEdgeTex[slot] );
			Aa_Sampler( dev, 1, true );  dev->SetTexture( 1, SmaaAreaTex[slot] );
			Aa_Sampler( dev, 2, false ); dev->SetTexture( 2, SmaaSearchTex[slot] );
			Fsr_Quad( dev, w, h );
		}else ok = false;
		// pass 3: neighbourhood blending back into the frame
		if( ok && D3D_OK == dev->SetRenderTarget( 0, surf ) ){
			dev->SetPixelShader( SmaaBlendPS[slot] );
			Aa_Sampler( dev, 0, true ); dev->SetTexture( 0, AaSrcTex[slot] );
			Aa_Sampler( dev, 1, true ); dev->SetTexture( 1, AaBlendTex[slot] );
			dev->SetTexture( 2, 0 );
			Fsr_Quad( dev, w, h );
		}else ok = false;
		if( edgeSurf ) edgeSurf->Release();
		if( blendSurf ) blendSurf->Release();
	}
	dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 );
	if( sb ){ sb->Apply(); }	// kept for the next frame (AaStateBlock); restores shader, textures, samplers
	if( prevRT ){ dev->SetRenderTarget( 0, prevRT ); prevRT->Release(); }
	return ok;
}

// http://stackoverflow.com/questions/14955954/update-directx-texture
// http://stackoverflow.com/questions/10596453/how-do-i-correctly-implement-texture-pixel-to-texel-correction-in-direct3d-9
LPDIRECT3DTEXTURE9 D3dTexture = 0; // rendering texture
LPDIRECT3DTEXTURE9 SecTexture = 0; // rendering texture for second win
// http://stackoverflow.com/questions/13145084/fast-paletted-screen-blit-with-direct3d-9
LPDIRECT3DTEXTURE9 D3dPalette = 0; // palette texture
LPDIRECT3DTEXTURE9 SecPalette = 0; // palette texture for second win
LPDIRECT3DTEXTURE9 D3dTextureColor = 0; // (th4) ultra color texture (A8R8G8B8, s0)
LPDIRECT3DTEXTURE9 SecTextureColor = 0; // (th4) ultra color texture for second win
LPDIRECT3DTEXTURE9 D3dTextureL8 = 0; // (th4) ultra light texture (L8, s1)
LPDIRECT3DTEXTURE9 SecTextureL8 = 0; // (th4) ultra light texture for second win
LPDIRECT3DTEXTURE9 D3dTextureFlag = 0; // (th4) ultra vignetting flag texture (L8, s4)
LPDIRECT3DTEXTURE9 SecTextureFlag = 0; // (th4) ultra vignetting flag texture for second win
LPDIRECT3DTEXTURE9 D3dTextureOverlay = 0; // (th4) ultra overlay texture (A8R8G8B8, s2)
LPDIRECT3DTEXTURE9 SecTextureOverlay = 0; // (th4) ultra overlay texture for second win
LPDIRECT3DTEXTURE9 D3dTextureTint = 0; // (th4) Stage 3 tint texture (A8R8G8B8, s3)
LPDIRECT3DTEXTURE9 SecTextureTint = 0; // (th4) Stage 3 tint texture for second win
IDirect3DPixelShader9* PaletteShader = 0; // Main shader
IDirect3DPixelShader9* InterfaceShader = 0; // reserved for second layer
IDirect3DPixelShader9* DirectShader = 0; // shader for automap
struct TransformedVertex { vec4 pos;	vec2 tex; };

extern unsigned int AutomapPointsCount;
extern unsigned int AutomapLinePointsCount;
extern line_vertex AutomapPoints[20000];
extern line_vertex AutomapLines[10000];

bool D3dRef = false;
void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp);
bool D3dReset();

// ===== (th4) 2026-09-10 diagnostics: d3d9 callback-crash investigation =====
// Writes "th4_d3d_diag.log" next to the exe (working directory). The gated
// entries follow the overlay_dump_on.flag pattern: create "d3d_diag_on.flag"
// to enable them, delete it to stop. The raw variant always writes and is used
// for crash-time lines (VEH handler) so a fatal run always leaves a trace.
static void D3dDiagWriteLine(const char* line)
{
	FILE* f = fopen("th4_d3d_diag.log", "a");
	if (!f) return;
	SYSTEMTIME st;
	GetLocalTime(&st);
	fprintf(f, "[%02d:%02d:%02d.%03u][tid %lu] %s\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, GetCurrentThreadId(), line);
	fclose(f);
}

void D3dDiagLogRaw(const char* line)
{
	D3dDiagWriteLine(line);
}

bool D3dDiagEnabled()
{
	static int check = 0;
	static bool enabled = false;
	if (--check <= 0) {
		check = 240;
		enabled = GetFileAttributesA("d3d_diag_on.flag") != INVALID_FILE_ATTRIBUTES;
	}
	return enabled;
}

void D3dDiagLog(const char* fmt, ...)
{
	if (!D3dDiagEnabled()) return;
	char msg[1024];
	va_list va;
	va_start(va, fmt);
	// (th4) 2026-09-10: _vsnprintf_s + _TRUNCATE so an over-long message can
	// never raise the invalid-parameter handler (vsprintf_s would) — a logger
	// that can kill the process is worse than a truncated line.
	_vsnprintf_s(msg, sizeof(msg), _TRUNCATE, fmt, va);
	va_end(va);
	D3dDiagWriteLine(msg);
}

void D3dDiagAddr(char* out, size_t outSize, void* addr)
{
	char name[MAX_PATH] = "?";
	DWORD off = 0;
	MEMORY_BASIC_INFORMATION mbi;
	if (addr && VirtualQuery(addr, &mbi, sizeof(mbi))) {
		char fullPath[MAX_PATH] = "";
		if (GetModuleFileNameA((HMODULE)mbi.AllocationBase, fullPath, sizeof(fullPath))) {
			char* s = strrchr(fullPath, '\\');
			strncpy_s(name, s ? s + 1 : fullPath, _TRUNCATE);
		}
		off = (DWORD)((uintptr_t)addr - (uintptr_t)mbi.AllocationBase);
	}
	// (th4) 2026-09-10: same _TRUNCATE reasoning as D3dDiagLog — truncate,
	// never terminate.
	_snprintf_s(out, outSize, _TRUNCATE, "%s+0x%X", name, off);
}

#ifdef TH4_D3D_DIAG_ENABLED
// (th4) 2026-09-10 Phase 2(d) diagnosis-only probes for the post-transition
// wedge (Present 0x88760873 between the two forced SwitchFullscreen restyles
// of one click; see Rev 2.52e). Everything here READS state: window style,
// rects, foreground, TestCooperativeLevel, the swapchain's current present
// parameters, and the parameters SetD3dParam() would request next (pure fill).
// No behaviour is changed anywhere. Runtime-gated by "d3d_diag_on.flag".
static DWORD s_diagWedgeFirstFail = 0; // 0 = no failed-Present episode active
static unsigned s_diagWedgeFails = 0;  // failed draws in the current episode
static void D3dDiagPhaseD(const char* tag)
{
	if (!D3dDiagEnabled()) return; // keep the probes free when logging is off
	HWND fg = GetForegroundWindow();
	char fgCls[64] = "?";
	if (fg) GetClassNameA(fg, fgCls, sizeof(fgCls));
	LONG style = 0;
	RECT wr = { 0, 0, 0, 0 }, cr = { 0, 0, 0, 0 };
	if (ghMainWnd) {
		style = (LONG)GetWindowLongPtr(ghMainWnd, GWL_STYLE);
		GetWindowRect(ghMainWnd, &wr);
		GetClientRect(ghMainWnd, &cr);
	}
	HRESULT coop = D3dDevice ? D3dDevice->TestCooperativeLevel() : (HRESULT)0;
	D3DPRESENT_PARAMETERS want;
	SetD3dParam(want); // pure: ZeroMemory + fill, no side effects
	D3DPRESENT_PARAMETERS have;
	ZeroMemory(&have, sizeof(have));
	HRESULT gpp = E_FAIL;
	if (D3dDevice) {
		IDirect3DSwapChain9* sc = 0;
		if (SUCCEEDED(D3dDevice->GetSwapChain(0, &sc)) && sc) {
			gpp = sc->GetPresentParameters(&have);
			sc->Release();
		}
	}
	D3DDIAG("%s: style=0x%08X win=%d,%d %dx%d client=%dx%d iconic=%d vis=%d fgMain=%d fgClass='%s' coop=0x%08X",
		tag, (unsigned)style, (int)wr.left, (int)wr.top, (int)(wr.right - wr.left), (int)(wr.bottom - wr.top),
		(int)cr.right, (int)cr.bottom, (int)IsIconic(ghMainWnd), (int)IsWindowVisible(ghMainWnd),
		(int)(fg == ghMainWnd), fgCls, (unsigned)coop);
	if (SUCCEEDED(gpp)) {
		D3DDIAG("%s: have bb=%ux%u fmt=%u cnt=%u int=%u win=%d hwnd=%p flags=0x%X | want bb=%ux%u fmt=%u int=%u win=%d game=%d vsync=%d",
			tag, have.BackBufferWidth, have.BackBufferHeight, (unsigned)have.BackBufferFormat, have.BackBufferCount,
			have.PresentationInterval, (int)have.Windowed, (void*)have.hDeviceWindow, (unsigned)have.Flags,
			want.BackBufferWidth, want.BackBufferHeight, (unsigned)want.BackBufferFormat, want.PresentationInterval,
			(int)want.Windowed, (int)gbRunGame, (int)Vsync);
	} else {
		D3DDIAG("%s: swapchain params unavailable (hr=0x%08X) | want bb=%ux%u fmt=%u int=%u win=%d game=%d vsync=%d",
			tag, (unsigned)gpp, want.BackBufferWidth, want.BackBufferHeight, (unsigned)want.BackBufferFormat,
			want.PresentationInterval, (int)want.Windowed, (int)gbRunGame, (int)Vsync);
	}
}
#endif


//LPDIRECTDRAW7 DirectDrawPtr7 = 0;
LPDIRECTDRAWSURFACE ScreenSurface32 = 0;
LPDIRECTDRAWSURFACE ScreenSurface32Back = 0;

LPDIRECTDRAW DirectDrawPtr;
LPDIRECTDRAWPALETTE DirectDrawPalettePtr;
LPDIRECTDRAWSURFACE DDSrcSurface;
LPDIRECTDRAWSURFACE ScreenSurface;       // match ScreenSurface *(LPDIRECTDRAWSURFACE2*)0x1501EE40 from storm.dll
int ScreenSurfacePitch;
// 15009BD0 DrawPrimarySurface storm.dll
CRITICAL_SECTION DDSurfaceCritSec;
DDSURFACEDESC ScreenSurfaceDesc;
int SurfaceLockCount;
uchar* SoftSrcSurfacePtr;           // (rt) pointer to a 656*WorkingWidth memory block

int DrawnCursorWIdth;
int CurCursorWidth;
int DrawnCursorStartX;
int DrawnCursorStartY;
int DrawnCursorHeight;
int CurCursorStartX;
int CurCursorStartY;
int CurCursorHeight;
unsigned char CursorImage[1920 + MaxItemCellWidth_4 * CELL_SIZE * MaxItemCellHeight_7 * CELL_SIZE];

//----- PCX & GIF ------------------------------------------------------------------------
enum PIC_FORMAT { PF_PCX = 0, PF_GIF = 1 };
int ScreenshotFormat = PF_GIF;

struct PcxHeader
{
	char Manufacturer;          // (rt) Constant flag 10 = ZSoft .PCX
	char Version;               // (rt) 5 = Version 3.0
	char Encoding;              // (rt) 1 = .PCX run-length encoding
	char BitsPerLayer;          // (rt) Bits per pixel per plane
	unsigned short Xmin;        // (rt) Image dimensions
	unsigned short Ymin;        // (rt) Image dimensions
	unsigned short Xmax;        // (rt) Image dimensions
	unsigned short Ymax;        // (rt) Image dimensions
	unsigned short HDpi;        // (rt) Horizontal resolution of the creating device
	unsigned short VDpi;        // (rt) Vertical resolution of the creating device
	char Colormap[48];          // (rt) CGA/EGA Palette
	char Reserved;
	char NPlanes;               // (rt) Number of colour planes
	unsigned short BPL;         // (rt) Bytes per line per colour plane (even)
	unsigned short Palette;     // (rt) How to interpret the EGA palette: 1 = colour/monochrome, 2 = greyscale
	unsigned short HscreenSize; // Horizontal screen size in pixels. Not used.
	unsigned short VscreenSize; // Vertical screen size in pixels. Not used.
	char Filler[54];            // (rt) Zero-filled to the end of the header
};

struct GIF
{
	ushort w, h;
	int depth;
	int fd;
	int offset;
	int nframes;
	uchar* frame, * back;
	uint partial;
	uchar buffer[255];
};

GIF* new_gif(const char* fname, ushort width, ushort height, uchar* palette, int depth, int loop);
void add_frame(GIF* gif, ushort delay);
void close_gif(GIF* gif);

typedef uchar uchara[];

/* helper to write a little-endian 16-bit number portably */
#define write_num(fd, n) write((fd), uchara {uchar((n) & 0xFF), uchar((n) >> 8)} , 2)

static uchar vga[0x30] = {
	0x00, 0x00, 0x00,
	0xAA, 0x00, 0x00,
	0x00, 0xAA, 0x00,
	0xAA, 0x55, 0x00,
	0x00, 0x00, 0xAA,
	0xAA, 0x00, 0xAA,
	0x00, 0xAA, 0xAA,
	0xAA, 0xAA, 0xAA,
	0x55, 0x55, 0x55,
	0xFF, 0x55, 0x55,
	0x55, 0xFF, 0x55,
	0xFF, 0xFF, 0x55,
	0x55, 0x55, 0xFF,
	0xFF, 0x55, 0xFF,
	0x55, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,
};

#pragma warning(disable: 4200)
struct Node {
	ushort key;
	Node* children[];
};

Node* new_node(ushort key, int degree)
{
	Node* node = (Node*)calloc(1, sizeof(*node) + degree * sizeof(Node*));
	if (node)
		node->key = key;
	return node;
}

void del_trie(Node* root, int degree)
{
	if (!root)
		return;
	for (int i = 0; i < degree; i++)
		del_trie(root->children[i], degree);
	free(root);
}

void put_loop(GIF* gif, ushort loop);

GIF* new_gif(const char* fname, ushort width, ushort height, uchar* palette, int depth, int loop)
{
	int i, r, g, b, v;
	GIF* gif = (GIF*)calloc(1, sizeof(*gif) + 2 * width * height);
	if (!gif)
		goto no_gif;
	gif->w = width; gif->h = height;
	gif->depth = depth > 1 ? depth : 2;
	gif->frame = (uint8_t*)&gif[1];
	gif->back = &gif->frame[width * height];
	gif->fd = _creat(fname, _S_IREAD | _S_IWRITE);
	if (gif->fd == -1)
		goto no_fd;
#ifdef _WIN32
	setmode(gif->fd, O_BINARY);
#endif
	write(gif->fd, "GIF89a", 6);
	write_num(gif->fd, width);
	write_num(gif->fd, height);
	write(gif->fd, uchara{ uchar(0xF0 | (depth - 1)), 0x00, 0x00 }, 3);
	if (palette) {
		write(gif->fd, palette, 3 << depth);
	}
	else if (depth <= 4) {
		write(gif->fd, vga, 3 << depth);
	}
	else {
		write(gif->fd, vga, sizeof(vga));
		i = 0x10;
		for (r = 0; r < 6; r++) {
			for (g = 0; g < 6; g++) {
				for (b = 0; b < 6; b++) {
					write(gif->fd, uchara{ uchar(r * 51), uchar(g * 51), uchar(b * 51) }, 3);
					if (++i == 1 << depth)
						goto done_gct;
				}
			}
		}
		for (i = 1; i <= 24; i++) {
			v = i * 0xFF / 25;
			write(gif->fd, uchara{ (uchar)v, (uchar)v, (uchar)v }, 3);
		}
	}
done_gct:
	if (loop >= 0 && loop <= 0xFFFF)
		put_loop(gif, (ushort)loop);
	return gif;
no_fd:
	free(gif);
no_gif:
	return NULL;
}

void put_loop(GIF* gif, ushort loop)
{
	write(gif->fd, uchara{ '!', 0xFF, 0x0B }, 3);
	write(gif->fd, "NETSCAPE2.0", 11);
	write(gif->fd, uchara{ 0x03, 0x01 }, 2);
	write_num(gif->fd, loop);
	write(gif->fd, "\0", 1);
}

/* Add packed key to buffer, updating offset and partial.
 *   gif->offset holds position to put next *bit*
 *   gif->partial holds bits to include in next byte */
void put_key(GIF* gif, ushort key, int key_size)
{
	int byte_offset, bit_offset, bits_to_write;
	byte_offset = gif->offset / 8;
	bit_offset = gif->offset % 8;
	gif->partial |= ((uint)key) << bit_offset;
	bits_to_write = bit_offset + key_size;
	while (bits_to_write >= 8) {
		gif->buffer[byte_offset++] = gif->partial & 0xFF;
		if (byte_offset == 0xFF) {
			write(gif->fd, "\xFF", 1);
			write(gif->fd, gif->buffer, 0xFF);
			byte_offset = 0;
		}
		gif->partial >>= 8;
		bits_to_write -= 8;
	}
	gif->offset = (gif->offset + key_size) % (0xFF * 8);
}

void end_key(GIF* gif)
{
	int byte_offset;
	byte_offset = gif->offset / 8;
	gif->buffer[byte_offset++] = gif->partial & 0xFF;
	write(gif->fd, uchara{ (uchar)byte_offset }, 1);
	write(gif->fd, gif->buffer, byte_offset);
	write(gif->fd, "\0", 1);
	gif->offset = gif->partial = 0;
}

void put_image(GIF* gif, ushort w, ushort h, ushort x, ushort y)
{
	int nkeys, key_size, i, j;
	Node* node, * child, * root;
	int degree = 1 << gif->depth;

	write(gif->fd, ",", 1);
	write_num(gif->fd, x);
	write_num(gif->fd, y);
	write_num(gif->fd, w);
	write_num(gif->fd, h);
	write(gif->fd, uchara{ 0x00, (uchar)gif->depth }, 2);
	root = new_node(0, degree);
	/* Create nodes for single pixels. */
	for (nkeys = 0; nkeys < degree; nkeys++)
		root->children[nkeys] = new_node(nkeys, degree);
	node = root;
	nkeys += 2; /* skip clear code and stop code */
	key_size = gif->depth + 1;
	put_key(gif, degree, key_size); /* clear code */
	for (i = y; i < y + h; i++) {
		for (j = x; j < x + w; j++) {
			uchar pixel = gif->frame[i * gif->w + j] & (degree - 1);
			child = node->children[pixel];
			if (child) {
				node = child;
			}
			else {
				put_key(gif, node->key, key_size);
				if (nkeys < 0x1000) {
					if (nkeys == (1 << key_size))
						key_size++;
					node->children[pixel] = new_node(nkeys++, degree);
				}
				node = root->children[pixel];
			}
		}
	}
	put_key(gif, node->key, key_size);
	put_key(gif, degree + 1, key_size); /* stop code */
	end_key(gif);
	del_trie(root, degree);
}

int get_bbox(GIF* gif, ushort* w, ushort* h, ushort* x, ushort* y)
{
	int i, j, k;
	int left, right, top, bottom;
	left = gif->w; right = 0;
	top = gif->h; bottom = 0;
	k = 0;
	for (i = 0; i < gif->h; i++) {
		for (j = 0; j < gif->w; j++, k++) {
			if (gif->frame[k] != gif->back[k]) {
				if (j < left)   left = j;
				if (j > right)  right = j;
				if (i < top)    top = i;
				if (i > bottom) bottom = i;
			}
		}
	}
	if (left != gif->w && top != gif->h) {
		*x = left; *y = top;
		*w = right - left + 1;
		*h = bottom - top + 1;
		return 1;
	}
	else {
		return 0;
	}
}

void set_delay(GIF* gif, ushort d)
{
	write(gif->fd, uchara{ '!', 0xF9, 0x04, 0x04 }, 4);
	write_num(gif->fd, d);
	write(gif->fd, "\0\0", 2);
}

void add_frame(GIF* gif, ushort delay)
{
	ushort w, h, x, y;
	uchar* tmp;

	if (delay)
		set_delay(gif, delay);
	if (gif->nframes == 0) {
		w = gif->w;
		h = gif->h;
		x = y = 0;
	}
	else if (!get_bbox(gif, &w, &h, &x, &y)) {
		/* image's not changed; save one pixel just to add delay */
		w = h = 1;
		x = y = 0;
	}
	put_image(gif, w, h, x, y);
	gif->nframes++;
	tmp = gif->back;
	gif->back = gif->frame;
	gif->frame = tmp;
}

void close_gif(GIF* gif)
{
	write(gif->fd, ";", 1);
	close(gif->fd);
	free(gif);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

__forceinline void EncodePixel(unsigned char countSamePixel, unsigned char pixel, unsigned char*& destBuffer, int& bytesToWrite)
{
	if (countSamePixel > 1 || pixel >= 0b11000000) {
		*destBuffer++ = countSamePixel | 0b11000000;
		++bytesToWrite;
	}
	*destBuffer++ = pixel;
	++bytesToWrite;
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// th2, patch for TH1 on windows 7 and up for debug purpose
uint GetCooperativeLevel()
{
	//ex( WinMajorMinorVer )
	//return ( (short) GetVersion() ) == 0x0106 /*windows 7*/ ? // alternate ver 
	return WinMajorMinorVer >= 0x0601 /*windows 7*/ ?
		//#ifdef _DEBUG
		DDSCL_NORMAL
		//#elif defined _PROTECTION
		//	DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT
		//#else
		//	DDSCL_NORMAL | DDSCL_FULLSCREEN
		//#endif
		:
	DDSCL_NORMAL /*DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT*/
		;
}

int DoMenuScreenShot, ShotPitch, ShotStartingX, ShotStartingY, ShotWidth, ShotHeight;
uchar* ShotSrc;
PALETTEENTRY* ShotPalette;

//----- (00403368) --------------------------------------------------------
void SaveScreenShot()
{
#ifdef OLD_PCX_SCREENSHOT
	bool retVal;
	char FileName[260];
	//SaveScreenShot_gif( );
	HANDLE hFileHandle = CreateScreenShot(FileName);
	if (ScreenshotFormat == PF_GIF || DoMenuScreenShot) {
		uchar palette[3 * 256];
		int width = DoMenuScreenShot ? ShotWidth : ScreenWidth;
		int height = DoMenuScreenShot ? ShotHeight : ScreenHeight;
		int pitch = DoMenuScreenShot ? ShotPitch : WorkingWidth;
		PALETTEENTRY* srcPal = DoMenuScreenShot ? ShotPalette : ScreenPalette;
		uchar* srcBuf = DoMenuScreenShot ? ShotSrc : SoftSrcSurfacePtr;
		int startX = DoMenuScreenShot ? ShotStartingX : Screen_LeftBorder;
		int startY = DoMenuScreenShot ? ShotStartingY : Screen_TopBorder;
		if (DoMenuScreenShot && !VideoWindowExists && Handler1 != DisableInputWindowProc) {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(ghMainWnd, &p);
			if (p.x >= 0 && p.x < GUI_Width - 1 && p.y >= 0 && p.y < GUI_Width - 1) {
				HDC hdc1;
				HDC hdc;
				ScreenSurface->GetDC(&hdc1);
				ScreenSurface->GetDC(&hdc);
				DrawIcon(hdc, p.x, p.y, LoadCursor(HInstance, "DIABLOCURSOR"));
				ScreenSurface->ReleaseDC(hdc);
				ScreenSurface->ReleaseDC(hdc1);
			}
		}
		for (int i = 0; i < 256; ++i) memcpy(&palette[3 * i], &srcPal[i], 3);
		GIF* gif = new_gif(FileName, width, height, palette, 8, -1);
		uchar* dst = gif->frame;
		uchar* src = srcBuf + startX + startY * pitch;
		// Approach B (A8L8): apply LightTable to reconstruct the true visual palette
		// indices. Raw WorkingSurface holds unlit indices (PPL) or 0 (fully dark),
		// so a plain memcpy yields a blown-out or pitch-black screenshot.
/*
		uchar* lsrc = (!DoMenuScreenShot && LightSurface)
			? LightSurface + startX + startY * pitch
			: nullptr;
		for( int i = 0; i < height; ++i ){
			if( lsrc ){
				for( int x = 0; x < width; ++x ){
					uchar lv  = lsrc[x];
					uchar idx = src[x];
					dst[x] = (lv > 0) ? LightTable[lv * 256 + idx] : idx;
				}
				lsrc += pitch;
			}else{
				memcpy( dst, src, width );
			}
			dst += width;
			src += pitch;
		}
*/
// 256 brightness levels A8L8 texture
		uchar* lsrc = (!DoMenuScreenShot && LightSurface)
			? LightSurface + startX + startY * pitch
			: nullptr;

		// Precompute gamma‑corrected palette as RGB triples (0‑255)
		struct RGB { uchar r, g, b; };
		RGB palRGB[256];
		for (int i = 0; i < 256; ++i) {
			palRGB[i].r = srcPal[i].peRed;
			palRGB[i].g = srcPal[i].peGreen;
			palRGB[i].b = srcPal[i].peBlue;
		}

		for (int row = 0; row < height; ++row) {
			if (lsrc) {
				for (int x = 0; x < width; ++x) {
					uchar lv = lsrc[x];          // 0 = full bright, 255 = full dark
					uchar idx = src[x];           // original palette index

					// Shader formula: brightness = saturate(1.0 - lv/255.0)
					float factor = 1.0f - lv / 255.0f;
					if (factor < 0.0f) factor = 0.0f;
					if (factor > 1.0f) factor = 1.0f;

					// Compute lit colour (float, then clamp to 0‑255)
					int r = (int)(palRGB[idx].r * factor);
					int g = (int)(palRGB[idx].g * factor);
					int b = (int)(palRGB[idx].b * factor);
					if (r > 255) r = 255; if (g > 255) g = 255; if (b > 255) b = 255;

					// Find closest palette index (Euclidean distance in RGB space)
					int bestIdx = 0;
					int bestDist = INT_MAX;
					for (int palIdx = 0; palIdx < 256; ++palIdx) {
						int dr = r - palRGB[palIdx].r;
						int dg = g - palRGB[palIdx].g;
						int db = b - palRGB[palIdx].b;
						int dist = dr * dr + dg * dg + db * db;
						if (dist < bestDist) {
							bestDist = dist;
							bestIdx = palIdx;
						}
					}
					dst[x] = (uchar)bestIdx;
				}
				lsrc += pitch;
			}
			else {
				// No LightSurface (menu, video, etc.) – use original indices unchanged
				memcpy(dst, src, width);
			}
			dst += width;
			src += pitch;
		}
		add_frame(gif, 0);
		close_gif(gif);
	}
	else {
		if (hFileHandle != INVALID_HANDLE_VALUE) {
			retVal = WriteScreenShotHeader(hFileHandle, ScreenWidth, ScreenHeight);
			if (retVal) {
#ifndef USING_DDSURFACE_INGAME
				retVal = PCXWriteRow(hFileHandle, ScreenWidth, ScreenHeight, WorkingWidth, SoftSrcSurfacePtr + Screen_LeftBorder + Screen_TopBorder * WorkingWidth);
				if (false) {
					char buf[300]; // for status string (~)
					sprintf_s(buf, 300, "Screenshot saved %s", FileName); // TheHellVersion
					NetSendString(1 << CurrentPlayerIndex, buf);
				}
#else
				PALETTEENTRY palette[256];
				DirectDrawPalettePtr->GetEntries(0, 0, 256, palette);
				MakeScreenRedWhileSavingScreenshot(palette);
				DDSURFACEDESC ScreenSurfaceDesc;
				ScreenSurfaceDesc.dwSize = sizeof(ScreenSurfaceDesc);
				ScreenSurface->Lock(0, &ScreenSurfaceDesc, DDLOCK_WAIT, 0);
				retVal = PCXWriteRow(hFileHandle, ScreenWidth, ScreenHeight, ScreenSurfaceDesc.lPitch, (unsigned char*)ScreenSurfaceDesc.lpSurface);
				ScreenSurface->Unlock(0);
				DirectDrawPalettePtr->SetEntries(0, 0, 256, palette);
#endif
#ifdef USE_32BITCOLOR
				retVal = WritePaletteToPCX(hFileHandle, ScreenPalette);
#else
				PALETTEENTRY pal[256];
				GetSystemPaletteEntries(GetDC(0), 0, 256, pal);
				retVal = WritePaletteToPCX(hFileHandle, pal);
#endif
			}
			CloseHandle(hFileHandle);
			if (!retVal)
				DeleteFileA(FileName);
			//Sleep(300);
		}
	}
#else
	// @todo Move this guard into a common header file
	class DDSurfaceGuard {
	public:
		explicit DDSurfaceGuard(LPDIRECTDRAWSURFACE surface)
			: m_surface(surface) {};
		~DDSurfaceGuard()
		{
			if (m_surface) {
				m_surface->Unlock(0);
			}
		}
	private:
		LPDIRECTDRAWSURFACE m_surface;
	};

	DDSURFACEDESC dsd = { 0 };
	dsd.dwSize = sizeof(DDSURFACEDESC);
	ScreenSurface->Lock(0, &dsd, DDLOCK_WAIT, 0);
	DDSurfaceGuard dds_guard(ScreenSurface);
	TheHell::Graphics::MakeScreenShot(FilePalette, dsd.dwWidth, dsd.dwHeight, dsd.lPitch,
		static_cast<unsigned char*>(dsd.lpSurface), TheHell::Graphics::ScreenshotFormat::PNG);
#endif
}

//----- (00403451) --------------------------------------------------------
bool __fastcall WriteScreenShotHeader(HANDLE hFile, int width, int height)
{
#ifdef OLD_PCX_SCREENSHOT
	DWORD NumberOfBytesWritten;
	PcxHeader pcxHeader;
	memset(&pcxHeader, 0, sizeof(pcxHeader));
	pcxHeader.Manufacturer = 10;
	pcxHeader.Version = 5;
	pcxHeader.Encoding = 1;
	pcxHeader.BitsPerLayer = 8;
	pcxHeader.Xmin = 0;
	pcxHeader.Ymin = 0;
	pcxHeader.Xmax = (ushort)width - 1;
	pcxHeader.Ymax = (ushort)height - 1;
	pcxHeader.HDpi = 300;
	pcxHeader.VDpi = 300;
	pcxHeader.NPlanes = 1;
	pcxHeader.BPL = width;
	WriteFile(hFile, &pcxHeader, sizeof(pcxHeader), &NumberOfBytesWritten, 0);
	return NumberOfBytesWritten == sizeof(pcxHeader);
#else
	throw std::exception(__FUNCTION__ " Deprecated.");
#endif
}

// some PCX image writing-related stuff
//----- (004034E7) --------------------------------------------------------
bool __fastcall WritePaletteToPCX(HANDLE hFile, PALETTEENTRY* a2)
{
#ifdef OLD_PCX_SCREENSHOT
	int v2;                     // eax@1
	int v3;                     // edx@1
	uchar* v4;                  // esi@1
	uchar Buffer[772];          // [sp+8h] [bp-308h]@1
	DWORD NumberOfBytesWritten; // [sp+30Ch] [bp-4h]@3
	v2 = (int)a2 + 2;
	Buffer[0] = 12;
	v4 = &Buffer[2];
	v3 = 256;
	do {
		*(v4 - 1) = *(uchar*)(v2 - 2);
		*v4 = *(uchar*)(v2 - 1);
		*(v4 + 1) = *(uchar*)v2;
		v2 += 4;
		v4 += 3;
		--v3;
	} while (v3);
	WriteFile(hFile, Buffer, 0x301u, &NumberOfBytesWritten, 0);
	return NumberOfBytesWritten == 769;
#else
	//
	// Nothing to do
	// This function is deprecated and will be removed eventually
	//
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

// PCX-body writing
//----- (00403550) --------------------------------------------------------
bool __fastcall PCXWriteRow(HANDLE hFile, int width, int height, long pitch, unsigned char* pixels)
{
#ifdef OLD_PCX_SCREENSHOT
	DWORD NumberOfBytesWritten;
	unsigned char* encoded = (uchar*)malloc(2 * width); //allocating a row of pixels, 2 bytes per pixel for PCX format
	for (int row = 0; row < height; row++) {
		int bytesToWrite = PCXConvertRow(pixels, encoded, width);
		BOOL writen = WriteFile(hFile, encoded, bytesToWrite, &NumberOfBytesWritten, 0);
		if (!writen || NumberOfBytesWritten != bytesToWrite) {
			return false;
		}
		pixels += pitch;
	}
	free(encoded);
	return true;
#else
	// Nothing to do
	// This function is deprecated and will be removed eventually
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

//Run-length encoding raw pixels row to PCX format
//----- (004035C9) --------------------------------------------------------
int __fastcall PCXConvertRow(unsigned char* sourcePixels, unsigned char* destBuffer, long rowWidth)
{
#ifdef OLD_PCX_SCREENSHOT
	int bytesToWrite = 0;
	unsigned char pixel = *sourcePixels++, prevPixel = pixel, countSamePixel = 1;
	for (int x = 2; x <= rowWidth; x++, prevPixel = pixel) {
		pixel = *sourcePixels++;
		if (pixel == prevPixel && countSamePixel < 63) {
			countSamePixel++;
		}
		else {
			EncodePixel(countSamePixel, prevPixel, destBuffer, bytesToWrite);
			countSamePixel = 1;
		}
	}
	EncodePixel(countSamePixel, pixel, destBuffer, bytesToWrite);
	return bytesToWrite;
#else
	//
	// Nothing to do
	// This function is deprecated and will be removed eventually
	//
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

//function creates a screen shot file (generates next name) an returns Win32 handle to created file (INVALID_HANDLE_VALUE for error)
//----- (00403605) --------------------------------------------------------
HANDLE __fastcall CreateScreenShot(char(&pOutputFilename)[MAX_PATH])
{
#ifdef OLD_PCX_SCREENSHOT
	int v2;                // esi@1
	int snapshotIdx;       // eax@6
	__int32 v5;            // eax@1
	uchar Dst[10000];      // [sp+124h] [bp-64h]@1
	struct _finddata_t a2; // [sp+Ch] [bp-17Ch]@1
	memset(Dst, 0, sizeof(Dst));
	v5 = _findfirst(ScreenshotFormat ? "THscrn????.gif" : "THscrn????.PCX", &a2);
	v2 = v5;
	if (v5 != -1) {
		do {
			if (isdigit(a2.name[6])) {
				if (isdigit(a2.name[7])) {
					if (isdigit(a2.name[8])) {
						if (isdigit(a2.name[9])) {
							Dst[1000 * a2.name[6] - 48000 + 100 * a2.name[7] - 4800 + 10 * a2.name[8] - 480 + a2.name[9] - 48] = 1; //53328
						}
					}
				}
			}
		} while (!_findnext(v2, &a2));
	}
	snapshotIdx = 0;
	while (Dst[snapshotIdx]) {
		++snapshotIdx;
		if (snapshotIdx >= sizeof(Dst))
			return INVALID_HANDLE_VALUE;
	}
	sprintf(pOutputFilename, ScreenshotFormat ? "THscrn%04d.gif" : "THscrn%04d.PCX", snapshotIdx);
	if (ScreenshotFormat) {
		return 0;
	}
	else {
		return CreateFileA(pOutputFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}
#else
	throw std::exception(__FUNCTION__ " Deprecated");
#endif
}

//----- (004036CF) --------------------------------------------------------
int __fastcall MakeScreenRedWhileSavingScreenshot(PALETTEENTRY* screenPalette)
{
#ifdef OLD_PCX_SCREENSHOT 
	PALETTEENTRY redPal[256];
	for (int i = 0; i < 256; i++) {
		redPal[i].peRed = screenPalette[i].peRed;
		redPal[i].peGreen = 0;
		redPal[i].peBlue = 0;
		redPal[i].peFlags = 0;
	}
	return DirectDrawPalettePtr->SetEntries(0, 0, 256, redPal);
#else
	//
	// Nothing to do
	// This function is deprecated and will be removed eventually
	//
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

// loaded from ui_art\mainmenz.pcx (dragon)
uchar MorMenuPal[1024] = {
	0xff, 0xff, 0xff, 0, 0x00, 0xff, 0xff, 0, 0xff, 0x00, 0xff, 0, 0x00, 0x00, 0xff, 0,
	0xff, 0xff, 0x00, 0, 0x00, 0xff, 0x00, 0, 0xff, 0x00, 0x00, 0, 0x80, 0x80, 0x80, 0,
	0xa0, 0xa0, 0xa4, 0, 0xc1, 0xcc, 0xd9, 0, 0x00, 0x00, 0x00, 0, 0x08, 0x00, 0x00, 0,
	0x08, 0x00, 0x08, 0, 0x00, 0x08, 0x00, 0, 0x12, 0x00, 0x00, 0, 0x00, 0x08, 0x08, 0,
	0x00, 0x00, 0x00, 0, 0x11, 0x11, 0x11, 0, 0x1e, 0x1e, 0x1e, 0, 0x2e, 0x2e, 0x2e, 0,
	0x3d, 0x3d, 0x3d, 0, 0x4c, 0x4c, 0x4c, 0, 0x59, 0x59, 0x59, 0, 0x66, 0x66, 0x66, 0,
	0x73, 0x73, 0x73, 0, 0x85, 0x85, 0x85, 0, 0x94, 0x94, 0x94, 0, 0xa3, 0xa3, 0xa3, 0,
	0xb8, 0xb8, 0xb8, 0, 0xcc, 0xcc, 0xcc, 0, 0xde, 0xde, 0xde, 0, 0xf3, 0xf3, 0xf3, 0,
	0x0c, 0x05, 0x05, 0, 0x23, 0x09, 0x09, 0, 0x39, 0x0d, 0x0d, 0, 0x4f, 0x11, 0x11, 0,
	0x66, 0x15, 0x15, 0, 0x7c, 0x19, 0x19, 0, 0x93, 0x1e, 0x1e, 0, 0xa9, 0x22, 0x22, 0,
	0xbf, 0x27, 0x27, 0, 0xc7, 0x38, 0x38, 0, 0xcf, 0x49, 0x49, 0, 0xd8, 0x5b, 0x5b, 0,
	0xe0, 0x6c, 0x6c, 0, 0xe8, 0x7d, 0x7d, 0, 0xf4, 0x96, 0x96, 0, 0xff, 0xbd, 0xbd, 0,
	0x0f, 0x05, 0x00, 0, 0x25, 0x0e, 0x03, 0, 0x3f, 0x17, 0x08, 0, 0x5a, 0x22, 0x0c, 0,
	0x6f, 0x29, 0x10, 0, 0x85, 0x32, 0x13, 0, 0x9b, 0x3b, 0x18, 0, 0xb1, 0x43, 0x1b, 0,
	0xc7, 0x4b, 0x1f, 0, 0xcc, 0x61, 0x33, 0, 0xc7, 0x7b, 0x52, 0, 0xd0, 0x8c, 0x62, 0,
	0xdc, 0x9f, 0x70, 0, 0xe7, 0xb3, 0x7e, 0, 0xf4, 0xc9, 0x96, 0, 0xff, 0xe2, 0xb3, 0,
	0x14, 0x0b, 0x00, 0, 0x1a, 0x14, 0x08, 0, 0x31, 0x28, 0x16, 0, 0x39, 0x31, 0x1d, 0,
	0x48, 0x40, 0x27, 0, 0x5b, 0x51, 0x34, 0, 0x69, 0x60, 0x3f, 0, 0x78, 0x6f, 0x49, 0,
	0x87, 0x7e, 0x54, 0, 0x98, 0x8b, 0x5d, 0, 0xab, 0x9a, 0x63, 0, 0xbc, 0xa8, 0x6c, 0,
	0xcc, 0xb7, 0x75, 0, 0xdd, 0xc4, 0x7e, 0, 0xee, 0xd1, 0x8c, 0, 0xff, 0xe3, 0xa4, 0,
	0x05, 0x07, 0x0c, 0, 0x0d, 0x11, 0x1b, 0, 0x19, 0x1e, 0x2d, 0, 0x25, 0x2b, 0x41, 0,
	0x2f, 0x36, 0x50, 0, 0x39, 0x41, 0x5f, 0, 0x43, 0x4c, 0x6f, 0, 0x4e, 0x58, 0x7d, 0,
	0x58, 0x63, 0x8d, 0, 0x66, 0x70, 0x99, 0, 0x79, 0x7f, 0xa0, 0, 0x87, 0x8d, 0xac, 0,
	0x93, 0x99, 0xb9, 0, 0x9f, 0xa5, 0xc6, 0, 0xb2, 0xb7, 0xd7, 0, 0xc8, 0xcd, 0xea, 0,
	0x0c, 0x07, 0x07, 0, 0x1b, 0x0e, 0x0e, 0, 0x33, 0x19, 0x19, 0, 0x44, 0x21, 0x21, 0,
	0x56, 0x29, 0x29, 0, 0x68, 0x31, 0x31, 0, 0x79, 0x39, 0x39, 0, 0x8b, 0x41, 0x41, 0,
	0x9c, 0x49, 0x49, 0, 0xa5, 0x5a, 0x5a, 0, 0xa8, 0x71, 0x71, 0, 0xb3, 0x80, 0x80, 0,
	0xbd, 0x8f, 0x8f, 0, 0xca, 0x9e, 0x9e, 0, 0xd7, 0xb2, 0xb2, 0, 0xe8, 0xca, 0xca, 0,
	0x1e, 0x07, 0x00, 0, 0x57, 0x1a, 0x00, 0, 0x8f, 0x2e, 0x00, 0, 0xc7, 0x41, 0x00, 0,
	0xf0, 0x4c, 0x00, 0, 0xfe, 0x69, 0x24, 0, 0xff, 0x8c, 0x57, 0, 0xfe, 0xbe, 0xa0, 0,
	0x19, 0x19, 0x00, 0, 0x57, 0x55, 0x00, 0, 0x86, 0x86, 0x00, 0, 0xc3, 0xc3, 0x00, 0,
	0xf0, 0xec, 0x00, 0, 0xfe, 0xfb, 0x24, 0, 0xff, 0xfc, 0x57, 0, 0xff, 0xfd, 0x9f, 0,
	0x23, 0x00, 0x00, 0, 0x5a, 0x00, 0x00, 0, 0x91, 0x00, 0x00, 0, 0xbd, 0x00, 0x00, 0,
	0xf0, 0x00, 0x00, 0, 0xfe, 0x24, 0x24, 0, 0xff, 0x57, 0x57, 0, 0xff, 0x9f, 0x9f, 0,
	0x00, 0x00, 0x19, 0, 0x00, 0x00, 0x57, 0, 0x00, 0x00, 0x8a, 0, 0x00, 0x00, 0xbd, 0,
	0x01, 0x01, 0xef, 0, 0x24, 0x24, 0xfe, 0, 0x57, 0x57, 0xff, 0, 0x9f, 0x9f, 0xff, 0,
	0x6c, 0x6c, 0x68, 0, 0x64, 0x64, 0x60, 0, 0x60, 0x60, 0x5c, 0, 0x5c, 0x5c, 0x58, 0,
	0x58, 0x58, 0x54, 0, 0x54, 0x54, 0x50, 0, 0x50, 0x50, 0x48, 0, 0x4c, 0x4c, 0x48, 0,
	0x48, 0x48, 0x44, 0, 0x44, 0x44, 0x40, 0, 0x40, 0x40, 0x3c, 0, 0x40, 0x40, 0x38, 0,
	0x40, 0x3c, 0x38, 0, 0x3c, 0x3c, 0x38, 0, 0x3c, 0x3c, 0x34, 0, 0x3c, 0x38, 0x34, 0,
	0x38, 0x38, 0x34, 0, 0x38, 0x38, 0x30, 0, 0x38, 0x34, 0x30, 0, 0x34, 0x34, 0x30, 0,
	0x34, 0x34, 0x2c, 0, 0x34, 0x30, 0x2c, 0, 0x34, 0x30, 0x28, 0, 0x30, 0x30, 0x2c, 0,
	0x30, 0x30, 0x28, 0, 0x30, 0x2c, 0x28, 0, 0x2c, 0x2c, 0x28, 0, 0x2c, 0x2c, 0x24, 0,
	0x2c, 0x28, 0x24, 0, 0x2c, 0x28, 0x20, 0, 0x28, 0x28, 0x24, 0, 0x28, 0x28, 0x20, 0,
	0x28, 0x24, 0x20, 0, 0x28, 0x24, 0x1c, 0, 0x24, 0x24, 0x20, 0, 0x24, 0x24, 0x1c, 0,
	0x24, 0x24, 0x18, 0, 0x24, 0x20, 0x1c, 0, 0x24, 0x20, 0x18, 0, 0x24, 0x20, 0x14, 0,
	0x20, 0x20, 0x18, 0, 0x20, 0x1c, 0x18, 0, 0x20, 0x1c, 0x14, 0, 0x20, 0x20, 0x0c, 0,
	0x20, 0x1c, 0x10, 0, 0x1c, 0x1c, 0x14, 0, 0x1c, 0x1c, 0x10, 0, 0x18, 0x1c, 0x14, 0,
	0x1c, 0x18, 0x14, 0, 0x1c, 0x1c, 0x08, 0, 0x1c, 0x18, 0x10, 0, 0x18, 0x18, 0x14, 0,
	0x18, 0x18, 0x10, 0, 0x14, 0x18, 0x10, 0, 0x18, 0x14, 0x10, 0, 0x18, 0x18, 0x08, 0,
	0x14, 0x14, 0x10, 0, 0x14, 0x14, 0x0c, 0, 0x10, 0x14, 0x10, 0, 0x14, 0x14, 0x08, 0,
	0x14, 0x10, 0x0c, 0, 0x10, 0x10, 0x0c, 0, 0x10, 0x10, 0x08, 0, 0x0c, 0x10, 0x0c, 0,
	0x0c, 0x0c, 0x0c, 0, 0x0c, 0x0c, 0x08, 0, 0x0c, 0x0c, 0x04, 0, 0x08, 0x08, 0x08, 0,
	0x04, 0x08, 0x08, 0, 0x08, 0x08, 0x04, 0, 0x04, 0x04, 0x04, 0, 0x08, 0x0c, 0x08, 0,
	0x04, 0x08, 0x04, 0, 0x28, 0x20, 0x14, 0, 0x20, 0x18, 0x10, 0, 0x18, 0x14, 0x0c, 0,
	0x14, 0x10, 0x08, 0, 0x10, 0x0c, 0x08, 0, 0x0c, 0x08, 0x08, 0, 0x08, 0x04, 0x04, 0,
	0xbf, 0x26, 0x0c, 0, 0x89, 0x3e, 0x07, 0, 0xa9, 0x1b, 0x08, 0, 0x82, 0x2e, 0x05, 0,
	0x98, 0x17, 0x05, 0, 0x6a, 0x30, 0x08, 0, 0x84, 0x16, 0x04, 0, 0x82, 0x06, 0x02, 0,
	0x64, 0x1d, 0x01, 0, 0x6c, 0x0a, 0x01, 0, 0x41, 0x1e, 0x07, 0, 0x61, 0x00, 0x00, 0,
	0x49, 0x0c, 0x00, 0, 0x37, 0x00, 0x00, 0, 0x28, 0x0c, 0x01, 0, 0x11, 0x0a, 0x03, 0,
	0xe1, 0x95, 0x2d, 0, 0xc6, 0x6a, 0x24, 0, 0xb3, 0x55, 0x17, 0, 0xbe, 0x43, 0x0d, 0,
	0xa0, 0x44, 0x0f, 0, 0x8d, 0x44, 0x13, 0, 0x83, 0x99, 0xb1, 0, 0x4f, 0x65, 0x7d, 0,
	0xc0, 0xc0, 0xc0, 0, 0x00, 0x80, 0x80, 0, 0x80, 0x00, 0x80, 0, 0x00, 0x00, 0x80, 0,
	0x80, 0x80, 0x00, 0, 0x00, 0x80, 0x00, 0, 0x80, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0,
};

// load from original ex main menu screenshot
uchar MenuPal[1024] = {
	0xFF, 0xFF, 0xFF, 0, 0x00, 0xFF, 0xFF, 0, 0xFF, 0x00, 0xFF, 0, 0x00, 0x00, 0xFF, 0, 0xFF, 0xFF, 0x00, 0, 0x00, 0xFF, 0x00, 0, 0xFF, 0x00, 0x00, 0, 0x80, 0x80, 0x80, 0,
	0xA0, 0xA0, 0xA4, 0, 0xFF, 0xFB, 0xF0, 0, 0x00, 0x00, 0x00, 0, 0x08, 0x00, 0x00, 0, 0x08, 0x00, 0x08, 0, 0x00, 0x08, 0x00, 0, 0x12, 0x00, 0x00, 0, 0x00, 0x08, 0x08, 0,
	0x00, 0x00, 0x00, 0, 0x11, 0x11, 0x11, 0, 0x1E, 0x1E, 0x1E, 0, 0x2E, 0x2E, 0x2E, 0, 0x3D, 0x3D, 0x3D, 0, 0x59, 0x59, 0x59, 0, 0x66, 0x66, 0x66, 0, 0x73, 0x73, 0x73, 0,
	0x85, 0x85, 0x85, 0, 0x94, 0x94, 0x94, 0, 0xA3, 0xA3, 0xA3, 0, 0xB8, 0xB8, 0xB8, 0, 0xCC, 0xCC, 0xCC, 0, 0xDE, 0xDE, 0xDE, 0, 0xF3, 0xF3, 0xF3, 0, 0xF3, 0xF3, 0xF3, 0,
	0x0C, 0x05, 0x05, 0, 0x23, 0x09, 0x09, 0, 0x39, 0x0D, 0x0D, 0, 0x4F, 0x11, 0x11, 0, 0x66, 0x15, 0x15, 0, 0x7C, 0x19, 0x19, 0, 0x93, 0x1E, 0x1E, 0, 0xA9, 0x22, 0x22, 0,
	0xBF, 0x27, 0x27, 0, 0xC7, 0x38, 0x38, 0, 0xCF, 0x49, 0x49, 0, 0xD8, 0x5B, 0x5B, 0, 0xE0, 0x6C, 0x6C, 0, 0xE8, 0x7D, 0x7D, 0, 0xF4, 0x96, 0x96, 0, 0xFF, 0xBD, 0xBD, 0,
	0x0F, 0x05, 0x00, 0, 0x25, 0x0E, 0x03, 0, 0x3F, 0x17, 0x08, 0, 0x5A, 0x22, 0x0C, 0, 0x6F, 0x29, 0x10, 0, 0x85, 0x32, 0x13, 0, 0x9B, 0x3B, 0x18, 0, 0xB1, 0x43, 0x1B, 0,
	0xC7, 0x4B, 0x1F, 0, 0xCC, 0x61, 0x33, 0, 0xC7, 0x7B, 0x52, 0, 0xD0, 0x8C, 0x62, 0, 0xDC, 0x9F, 0x70, 0, 0xE7, 0xB3, 0x7E, 0, 0xF4, 0xC9, 0x96, 0, 0xFF, 0xE2, 0xB3, 0,
	0x14, 0x0B, 0x00, 0, 0x1A, 0x14, 0x08, 0, 0x31, 0x28, 0x16, 0, 0x39, 0x31, 0x1D, 0, 0x48, 0x40, 0x27, 0, 0x5B, 0x51, 0x34, 0, 0x69, 0x60, 0x3F, 0, 0x78, 0x6F, 0x49, 0,
	0x87, 0x7E, 0x54, 0, 0x98, 0x8B, 0x5D, 0, 0xAB, 0x9A, 0x63, 0, 0xBC, 0xA8, 0x6C, 0, 0xCC, 0xB7, 0x75, 0, 0xDD, 0xC4, 0x7E, 0, 0xEE, 0xD1, 0x8C, 0, 0xFF, 0xE3, 0xA4, 0,
	0x05, 0x07, 0x0C, 0, 0x0D, 0x11, 0x1B, 0, 0x19, 0x1E, 0x2D, 0, 0x25, 0x2B, 0x41, 0, 0x2F, 0x36, 0x50, 0, 0x39, 0x41, 0x5F, 0, 0x43, 0x4C, 0x6F, 0, 0x4E, 0x58, 0x7D, 0,
	0x58, 0x63, 0x8D, 0, 0x66, 0x70, 0x99, 0, 0x79, 0x7F, 0xA0, 0, 0x87, 0x8D, 0xAC, 0, 0x93, 0x99, 0xB9, 0, 0x9F, 0xA5, 0xC6, 0, 0xB2, 0xB7, 0xD7, 0, 0xC8, 0xCD, 0xEA, 0,
	0x0C, 0x07, 0x07, 0, 0x1B, 0x0E, 0x0E, 0, 0x33, 0x19, 0x19, 0, 0x44, 0x21, 0x21, 0, 0x56, 0x29, 0x29, 0, 0x68, 0x31, 0x31, 0, 0x79, 0x39, 0x39, 0, 0x8B, 0x41, 0x41, 0,
	0x9C, 0x49, 0x49, 0, 0xA5, 0x5A, 0x5A, 0, 0xAB, 0x77, 0x73, 0, 0xB3, 0x80, 0x80, 0, 0xBD, 0x8F, 0x8F, 0, 0xCA, 0x9E, 0x9E, 0, 0xD7, 0xB2, 0xB2, 0, 0xE8, 0xCA, 0xCA, 0,
	0x1E, 0x07, 0x00, 0, 0x57, 0x1A, 0x00, 0, 0x8F, 0x2E, 0x00, 0, 0xC7, 0x41, 0x00, 0, 0xF0, 0x4C, 0x00, 0, 0xFE, 0x69, 0x24, 0, 0xFF, 0x8C, 0x57, 0, 0xFE, 0xBE, 0xA0, 0,
	0x19, 0x19, 0x00, 0, 0x57, 0x55, 0x00, 0, 0x86, 0x86, 0x00, 0, 0xC3, 0xC3, 0x00, 0,	0xF0, 0xEC, 0x00, 0, 0xFE, 0xFB, 0x24, 0, 0xFF, 0xFC, 0x57, 0, 0xFF, 0xFD, 0x9F, 0,
	0x23, 0x00, 0x00, 0, 0x5A, 0x00, 0x00, 0, 0x91, 0x00, 0x00, 0, 0xBD, 0x00, 0x00, 0, 0xF0, 0x00, 0x00, 0, 0xFE, 0x24, 0x24, 0, 0xFF, 0x57, 0x57, 0, 0xFF, 0x9F, 0x9F, 0,
	0x00, 0x00, 0x19, 0, 0x00, 0x00, 0x57, 0, 0x00, 0x00, 0x8A, 0, 0x00, 0x00, 0xBD, 0, 0x01, 0x01, 0xEF, 0, 0x24, 0x24, 0xFE, 0, 0x57, 0x57, 0xFF, 0, 0x9F, 0x9F, 0xFF, 0,
	0x30, 0x28, 0x24, 0, 0x2C, 0x24, 0x1C, 0, 0x28, 0x20, 0x1C, 0, 0x20, 0x1C, 0x18, 0, 0x84, 0x60, 0x4C, 0, 0x7C, 0x5C, 0x48, 0, 0x74, 0x54, 0x44, 0, 0x6C, 0x50, 0x3C, 0,
	0x6C, 0x4C, 0x38, 0, 0x64, 0x48, 0x34, 0, 0x60, 0x44, 0x30, 0, 0x5C, 0x40, 0x30, 0, 0x50, 0x38, 0x2C, 0, 0x48, 0x34, 0x24, 0, 0x3C, 0x2C, 0x24, 0, 0x00, 0x00, 0xD7, 0,
	0x2B, 0x17, 0x17, 0, 0x17, 0x0B, 0x0B, 0, 0x00, 0x00, 0xA3, 0, 0x00, 0x00, 0x6F, 0, 0x00, 0x00, 0x43, 0, 0x0F, 0x0B, 0x00, 0, 0x2B, 0x0B, 0x00, 0, 0x00, 0x00, 0x2B, 0,
	0x00, 0x00, 0x4F, 0, 0x00, 0x00, 0x37, 0, 0x00, 0x00, 0x2F, 0, 0x0F, 0x0F, 0x0F, 0, 0x00, 0x00, 0x27, 0, 0x6B, 0x43, 0x3B, 0, 0x53, 0x37, 0x2F, 0, 0x6F, 0x43, 0x3B, 0,
	0x50, 0x24, 0x20, 0, 0x50, 0x1C, 0x18, 0, 0x48, 0x18, 0x14, 0, 0x38, 0x1C, 0x18, 0, 0x3C, 0x14, 0x10, 0, 0x34, 0x10, 0x10, 0, 0x2C, 0x10, 0x0C, 0, 0x2C, 0x0C, 0x0C, 0,
	0x28, 0x0C, 0x0C, 0, 0x24, 0x14, 0x0C, 0, 0x20, 0x10, 0x08, 0, 0x1C, 0x10, 0x08, 0, 0x18, 0x0C, 0x08, 0, 0x14, 0x0C, 0x04, 0, 0x10, 0x08, 0x04, 0, 0x0C, 0x08, 0x04, 0,
	0x00, 0xCC, 0x00, 0, 0x00, 0xB8, 0x00, 0, 0x00, 0xAC, 0x00, 0, 0x00, 0xA4, 0x00, 0, 0x00, 0x90, 0x00, 0, 0x00, 0x78, 0x00, 0, 0x00, 0x6C, 0x00, 0, 0x00, 0x64, 0x00, 0,
	0x40, 0x40, 0x3C, 0, 0x3C, 0x3C, 0x38, 0, 0x34, 0x34, 0x30, 0, 0x2C, 0x2C, 0x28, 0, 0x24, 0x28, 0x24, 0, 0x20, 0x24, 0x20, 0, 0x1C, 0x24, 0x1C, 0, 0x18, 0x1C, 0x18, 0,
	0xF0, 0x9C, 0x88, 0, 0xE4, 0x94, 0x80, 0, 0xD8, 0x8C, 0x7C, 0, 0xD4, 0x88, 0x78, 0, 0xCC, 0x84, 0x74, 0, 0xC0, 0x7C, 0x6C, 0, 0xAC, 0x70, 0x60, 0, 0x9C, 0x64, 0x58, 0,
	0x90, 0x5C, 0x50, 0, 0x88, 0x58, 0x4C, 0, 0x7C, 0x50, 0x44, 0, 0x74, 0x48, 0x40, 0, 0x5C, 0x3C, 0x34, 0, 0x48, 0x2C, 0x28, 0, 0x34, 0x20, 0x1C, 0, 0x2C, 0x1C, 0x18, 0,
	0xBF, 0x26, 0x0C, 0, 0x89, 0x3E, 0x07, 0, 0xA9, 0x1B, 0x08, 0, 0x82, 0x2E, 0x05, 0, 0x98, 0x17, 0x05, 0, 0x6A, 0x30, 0x08, 0, 0x84, 0x16, 0x04, 0, 0x82, 0x06, 0x02, 0,
	0x64, 0x1D, 0x01, 0, 0x6C, 0x0A, 0x01, 0, 0x41, 0x1E, 0x07, 0, 0x61, 0x00, 0x00, 0, 0x49, 0x0C, 0x00, 0, 0x37, 0x00, 0x00, 0, 0x28, 0x0C, 0x01, 0, 0x11, 0x0A, 0x03, 0,
	0xE1, 0x95, 0x2D, 0, 0xC6, 0x6A, 0x24, 0, 0xB3, 0x55, 0x17, 0, 0xBE, 0x43, 0x0D, 0, 0xA0, 0x44, 0x0F, 0, 0x8D, 0x44, 0x13, 0, 0xEC, 0xE9, 0xD8, 0, 0xAC, 0xA8, 0x99, 0,
	0xC0, 0xC0, 0xC0, 0, 0x00, 0x80, 0x80, 0, 0x80, 0x00, 0x80, 0, 0x00, 0x00, 0x80, 0, 0x80, 0x80, 0x00, 0, 0x00, 0x80, 0x00, 0, 0x80, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0
};

//----- (th2) -------------------------------------------------------------
void LoadDragonMenuPalette()
{
	PALETTEENTRY* menuPal = (PALETTEENTRY*)MorMenuPal;
	for (int i = 0; i < 256; i++) {
		ScreenPalette[i].peBlue = menuPal[255 - i].peBlue;
		ScreenPalette[i].peGreen = menuPal[255 - i].peGreen;
		ScreenPalette[i].peRed = menuPal[255 - i].peRed;
	}
}

//----- (th2) -------------------------------------------------------------
void LoadMainMenuPalette()
{
	PALETTEENTRY* menuPal = (PALETTEENTRY*)MenuPal;
	for (int i = 0; i < 256; i++) {
		ScreenPalette[i].peBlue = menuPal[255 - i].peBlue;
		ScreenPalette[i].peGreen = menuPal[255 - i].peGreen;
		ScreenPalette[i].peRed = menuPal[255 - i].peRed;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall AdjustDDrawWindow(HWND hwnd)
{
	RECT r;
	GetWindowRect(hwnd, &r);
	int x = r.left;
	int y = r.top;
	AdjustWindowRectEx(&r, GetWindowLong(hwnd, GWL_STYLE), GetMenu(hwnd) != NULL, GetWindowLong(hwnd, GWL_EXSTYLE));
	MoveWindow(hwnd, x, y, r.right - r.left, r.bottom - r.top, true);
	// SetFocus(hwnd);// keep the mouse from leaving the window.
	// ClipCursor(&client);
}

//----- (th2) -------------------------------------------------------------
template <typename T, int redMax, int greenMax, int blueMax, int greenPos, int redPos> //__forceinline
void drawPixels(int left, int top, uchar* src, void* dst, int dstPitch, int srcPitch, int width, int height, PALETTEENTRY* screenPalette)
{
	T pal[256];
	for (int i = 0; i < 256; i++) {
		pal[i] = (T)((float)screenPalette[i].peBlue / 256.0 * blueMax) + ((T)((float)screenPalette[i].peGreen / 256.0 * greenMax) << greenPos) + ((T)((float)screenPalette[i].peRed / 256.0 * redMax) << redPos);
	}
	uchar* pixel = src + left + top * width;
	// DDraw fallback: re-apply LightTable since WorkingSurface now holds raw un-lit indices.
	// Only valid when src is within the working buffer (in-game DrawMode==0 path).
	// During menu/intro/video, src is the Storm DirectDraw surface (different allocation)
	// and must not be used to index into LightSurface.
	bool srcInWorkingBuf = LightSurface && SoftSrcSurfacePtr &&
		(uintptr_t)src >= (uintptr_t)SoftSrcSurfacePtr &&
		(uintptr_t)src < (uintptr_t)SoftSrcSurfacePtr + (uintptr_t)(MaxWorkingHeight + 256) * MaxWorkingWidth;
	ptrdiff_t lightOffset = srcInWorkingBuf ? (src - SoftSrcSurfacePtr) : -1;
	uchar* lightPixel = lightOffset >= 0 ? (LightSurface + lightOffset + left + top * width) : nullptr;
	T* pixel32 = (T*)dst + left + top * dstPitch / sizeof(T);
	int srcDelta = srcPitch - width;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			uchar idx = *pixel++;
			if (lightPixel) {
				uchar lv = *lightPixel++;
				if (lv > 0 && lv < (uchar)LightMax) idx = LightTable[lv * 256 + idx];
				else if (lv >= (uchar)LightMax) idx = 0;
			}
			*pixel32++ = pal[idx];
		}
		pixel += srcDelta;
		if (lightPixel) lightPixel += srcDelta;
		*(char**)&pixel32 += dstPitch - width * sizeof(T);
	}
}

bool AltEnterPressed = false;
extern bool MainMainMenu;
int OldWorldProcess = 0;

//----- (th2) -------------------------------------------------------------
bool AltEnter()
{
	if (IsSecondWin /*|| VideoWindowExists*/) return false;
	int isAltPressed = GetAsyncKeyState(VK_MENU) < 0;
	if (ExtraMouseButtons) {
		isAltPressed |= GetAsyncKeyState(VK_XBUTTON1) < 0;
	}
	IsAltPressed = isAltPressed;
	bool enterPressed = GetAsyncKeyState(VK_RETURN) < 0;
	if (AltEnterPressed && !IsAltPressed && !enterPressed) {
		AltEnterPressed = false;
		// (th4) 2026-09-10 diagnostics: an accidental Alt+Enter overlap (Enter
		// pressed to confirm a menu item while Alt is held for the layout
		// switch, Alt+Tab followed by Enter, or a side mouse button mapped to
		// "alt" via ExtraMouseButtons) fires a fullscreen toggle + device
		// reset. Log the edges so it is identifiable in the crash log.
		D3DDIAG("AltEnter release detected (gbRunGame=%d, IsFullScreen=%d)", gbRunGame, (int)IsFullScreen);
		if (gbRunGame /*|| MainMainMenu*/) {
			if (!IsFullScreen) {
				CursorOff();
			}
			IsFullScreen = !IsFullScreen;
			return true;
		}
	}
	else if (!AltEnterPressed && IsAltPressed && enterPressed) {
		AltEnterPressed = true;
		D3DDIAG("AltEnter press armed (vk_menu=%d, xbutton1=%d)",
			(int)(GetAsyncKeyState(VK_MENU) < 0),
			ExtraMouseButtons ? (int)(GetAsyncKeyState(VK_XBUTTON1) < 0) : -1);
	}
	if (OldWorldProcess != gbRunGame) {
		OldWorldProcess = gbRunGame;
		if (gbRunGame) {
			POINT p;
			GetCursorPos(&p);
			SetCursorPos(p.x + (ScreenWidth - GUI_Width) / 2 * WinWidth / ScreenWidth, p.y + (ScreenHeight - GUI_Height) / 2 * WinHeight / ScreenHeight);
			ScreenToClient(ghMainWnd, &p);
			CursorX = p.x + (ScreenWidth - GUI_Width) / 2;
			CursorY = p.y + (ScreenHeight - GUI_Height) / 2;
		}
		else {
			POINT c{ 0, 0 };
			ClientToScreen(ghMainWnd, &c);
			SetCursorPos(c.x + GUI_Width / 2 * WinWidth / ScreenWidth, c.y + GUI_Height / 2 * WinHeight / ScreenHeight);
		}
		if (IsFullScreen && !BigWin) {
			// (th4) 2026-09-10 diagnostics: THIS is the path that forces a
			// fullscreen window re-style + D3dReset on every game-state
			// transition (OldWorldProcess change) while fullscreen — not an
			// Alt+Enter accident. Logged so the SwitchFullscreen bursts in the
			// crash log are self-explanatory.
			D3DDIAG("AltEnter transition: forcing SwitchFullscreen (fullscreen window fix)");
			return true;
		}
	}
	return false;
}

#define PS_PALETTE
int PrevWinWidth, PrevWinHeight;

bool OnlySecondWin = false;
//----- (th2) -------------------------------------------------------------
void SwitchFullscreen(bool forceWindowed /*= false*/)
{
#ifdef TH4_D3D_DIAG_ENABLED
	// (th4) 2026-09-10 diagnostics: log every entry with its caller so an
	// accidental fullscreen toggle (Alt+Enter overlap, XButton-as-Alt, menu
	// action) is identifiable in the crash log. Gated by "d3d_diag_on.flag";
	// compiled out entirely unless TH4_D3D_DIAG_ENABLED is defined (funcs.h).
	{
		char caller[128];
		D3DDIAGADDR(caller, sizeof(caller), _ReturnAddress());
		// (th4) 2026-09-10 Phase 0: active + fgIsMain added — the MP "New Game"
		// device-lost storm starts when the fullscreen window is not active.
		D3DDIAG("SwitchFullscreen enter (caller %s, forceWindowed=%d, IsFullScreen=%d, active=%d, fgIsMain=%d)",
			caller, (int)forceWindowed, (int)IsFullScreen,
			IsMainWindowActive, (int)(GetForegroundWindow() == ghMainWnd));
	}
#endif
	if (forceWindowed) {
		if (IsFullScreen) {
			IsFullScreen = false;
		}
		else {
			return;
		}
	}
	int width = WinWidth, height = WinHeight;
	if (IsFullScreen) {
		PrevWinWidth = WinWidth;
		PrevWinHeight = WinHeight;
	buggy_res:
		width = BigWin ? GetSystemMetrics(SM_CXSCREEN) : ScreenWidth;
		height = BigWin ? GetSystemMetrics(SM_CYSCREEN) : ScreenHeight;
	}
	else {
		width = PrevWinWidth ? PrevWinWidth : ScreenWidth;
		height = PrevWinHeight ? PrevWinHeight : ScreenHeight;
	}
	RECT r = { 0, 0, width - 1, height - 1 };
	if (!IsFullScreen) {
		AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW, 0, 0);
	}
#ifdef TH4_D3D_DIAG_ENABLED
	D3dDiagPhaseD("SF pre-restyle"); // Phase 2(d): state before the restyle
#endif
	SetWindowLongPtr(ghMainWnd, GWL_STYLE, IsFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW);
	SetWindowPos(ghMainWnd, HWND_NOTOPMOST, IsFullScreen ? 0 : StartWindowX, IsFullScreen ? 0 : StartWindowY,
		r.right - r.left + 1, r.bottom - r.top + 1, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	if (!D3dReset()) {
		if (IsFullScreen && !BigWin) { // (rt) handling odd resolutions that attempt fullscreen
			BigWin = true;
			goto buggy_res;
		}
	}
#ifdef TH4_D3D_DIAG_ENABLED
	// (th4) 2026-09-10 Phase 0: did the restyle/reset cost us the activation?
	// (exclusive fullscreen + not foreground == device stays lost until the
	// window is activated again — the observed MP NEW GAME storm trigger).
	D3DDIAG("SwitchFullscreen done (IsFullScreen=%d, active=%d, minimized=%d, fgIsMain=%d)",
		(int)IsFullScreen, IsMainWindowActive, (int)IsMainWindowMinimized,
		(int)(GetForegroundWindow() == ghMainWnd));
	D3dDiagPhaseD("SF post-reset"); // Phase 2(d): state after restyle + reset
#endif
	if (IsFullScreen) {
		CursorOff();
	}
	PrevRestrictState = false;
}

// =============================================================================
// (th4) DYNAMIC ZOOM -- GPU half. The design and the CPU half (snapshot, UI mask, wheel, mouse) are in
// DynamicZoom.cpp. When DynZoom_PresentReady(), both scene paths below render TWICE through their normal
// scene shader -- the world snapshot into ZoomWorldTex, the finished frame into ZoomFinalTex (both game
// resolution) -- and Zoom_Composite (ZoomComposite.hlsl) writes the scene's real target: UI pixels from the
// finished frame, world pixels from the world render scaled about the hero, sharp-bilinear. Everything after
// the scene quad (automap lines, orbs, item icons, antialiasing, FSR / sharp scaling, PostFxPresent) then
// runs on that target as before; the world effects in PostFxPresent follow the zoom through THE VIEW (top of
// the PostFx block), told by DynZoom_EndFrame( zoom ) whether this frame was zoomed. Main device only (slot
// 0); the second window always shows 1x.
// Resources are default pool: released by Zoom_Release from SharpScale_Release like the rest of this file.
#include "ZoomComposite.h"
static IDirect3DPixelShader9* ZoomPS = 0;
static IDirect3DStateBlock9* ZoomStateBlock = 0;
static LPDIRECT3DTEXTURE9 ZoomWorldTex = 0, ZoomFinalTex = 0, ZoomMaskTex = 0;
static int ZoomWorldW = 0, ZoomWorldH = 0, ZoomFinalW = 0, ZoomFinalH = 0, ZoomMaskW = 0, ZoomMaskH = 0;

static void Zoom_Release( int slot )
{
	if( slot != 0 ) return;
	if( ZoomPS ){ ZoomPS->Release(); ZoomPS = 0; }
	if( ZoomStateBlock ){ ZoomStateBlock->Release(); ZoomStateBlock = 0; }
	if( ZoomWorldTex ){ ZoomWorldTex->Release(); ZoomWorldTex = 0; }
	if( ZoomFinalTex ){ ZoomFinalTex->Release(); ZoomFinalTex = 0; }
	if( ZoomMaskTex ){ ZoomMaskTex->Release(); ZoomMaskTex = 0; }
	ZoomWorldW = ZoomWorldH = ZoomFinalW = ZoomFinalH = ZoomMaskW = ZoomMaskH = 0;
}

// a game-resolution render-target texture; a NEW one is filled black once (the FX scene shaders blend with
// the target's previous contents, as they do with VideoSurface)
static bool Zoom_EnsureTarget( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9& tex, int& tw, int& th )
{
	const bool fresh = !tex || tw != ScreenWidth || th != ScreenHeight;
	if( !Fsr_EnsureTex( dev, tex, tw, th, ScreenWidth, ScreenHeight, D3DFMT_X8R8G8B8 ) ) return false;
	if( fresh ){
		LPDIRECT3DSURFACE9 su = 0;
		if( D3D_OK == tex->GetSurfaceLevel( 0, &su ) ){ dev->ColorFill( su, 0, D3DCOLOR_XRGB( 0, 0, 0 ) ); su->Release(); }
	}
	return true;
}

// Start of a scene path: is this frame zoomed? Builds and uploads the UI mask (the frame is finished here).
static bool Zoom_Prepare( LPDIRECT3DDEVICE9 dev )
{
	if( dev != D3dDevice || OnlySecondWin || gbRunGame != 1 || !DynZoom_PresentReady() ) return false;
	if( !ZoomPS && D3D_OK != dev->CreatePixelShader( (DWORD*)g_ps30_main_zoom, &ZoomPS ) ){ ZoomPS = 0; return false; }
	if( !Zoom_EnsureTarget( dev, ZoomWorldTex, ZoomWorldW, ZoomWorldH )
	 || !Zoom_EnsureTarget( dev, ZoomFinalTex, ZoomFinalW, ZoomFinalH ) ) return false;
	if( !ZoomMaskTex || ZoomMaskW != ScreenWidth || ZoomMaskH != ScreenHeight ){
		if( ZoomMaskTex ){ ZoomMaskTex->Release(); ZoomMaskTex = 0; }
		if( D3D_OK != dev->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &ZoomMaskTex, 0 ) ){ ZoomMaskTex = 0; return false; }
		ZoomMaskW = ScreenWidth; ZoomMaskH = ScreenHeight;
	}
	D3DLOCKED_RECT lr;
	if( D3D_OK != ZoomMaskTex->LockRect( 0, &lr, 0, D3DLOCK_DISCARD ) ) return false;
	DynZoom_BuildUiMask( (uchar*)lr.pBits, lr.Pitch, UiRects, UiRectN );
	ZoomMaskTex->UnlockRect( 0 );
	return true;
}

// The scene quad (current shader + textures) into a zoom target at game resolution.
static bool Zoom_DrawScene( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 tex )
{
	LPDIRECT3DSURFACE9 su = 0;
	if( D3D_OK != tex->GetSurfaceLevel( 0, &su ) ) return false;
	const bool ok = D3D_OK == dev->SetRenderTarget( 0, su );
	su->Release();
	if( !ok ) return false;
	const float r = (float)ScreenWidth - 0.5f, b = (float)ScreenHeight - 0.5f;
	TransformedVertex q[4] = { { { -0.5f, -0.5f, 0, 1 }, { 0, 0 } }, { { r, -0.5f, 0, 1 }, { 1, 0 } },
	                           { { -0.5f, b, 0, 1 }, { 0, 1 } },     { { r, b, 0, 1 }, { 1, 1 } } };
	if( D3D_OK != dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 ) ) return false;
	return D3D_OK == dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, q, sizeof(TransformedVertex) );
}

// ZoomFinalTex + ZoomWorldTex + mask -> dst (tw x th). Leaves dst as the render target; all other state restored.
static bool Zoom_Composite( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 dst, int tw, int th )
{
	float z, cx, cy; DynZoom_Params( z, cx, cy );
	const float c[2][4] = {
		{ 1.0f / z, cx, cy, z },
		{ (float)ScreenWidth, (float)ScreenHeight, 1.0f / ScreenWidth, 1.0f / ScreenHeight },
	};
	IDirect3DStateBlock9* sb = CaptureStateBlock( ZoomStateBlock, dev );
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	dev->SetRenderState( D3DRS_ZENABLE, FALSE );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	for( int st = 0; st < 3; ++st ){
		const DWORD f = st == 1 ? D3DTEXF_LINEAR : D3DTEXF_POINT;
		dev->SetSamplerState( st, D3DSAMP_MINFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MAGFILTER, f );
		dev->SetSamplerState( st, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
		dev->SetSamplerState( st, D3DSAMP_SRGBTEXTURE, FALSE );
	}
	bool ok = D3D_OK == dev->SetRenderTarget( 0, dst );
	if( ok ){
		dev->SetPixelShader( ZoomPS );
		dev->SetPixelShaderConstantF( 0, &c[0][0], 2 );
		dev->SetTexture( 0, ZoomFinalTex );
		dev->SetTexture( 1, ZoomWorldTex );
		dev->SetTexture( 2, ZoomMaskTex );
		Fsr_Quad( dev, tw, th );
		dev->SetTexture( 0, 0 ); dev->SetTexture( 1, 0 ); dev->SetTexture( 2, 0 );
	}
	if( sb ) sb->Apply();	// shader, its constants, the scene textures and samplers back
	return ok;
}

//----- (th2) -------------------------------------------------------------
// (th4) DYNAMIC ZOOM: the upload half of RenderQuadTexture, so it can run for the world snapshot too.
static int RenderQuadTexture_Upload(LPDIRECT3DTEXTURE9 tex, LPDIRECT3DTEXTURE9 pal, uchar* src)
{
	D3DLOCKED_RECT rect;
	if (D3D_OK != tex->LockRect(0, &rect, 0, 0)) return 0;
	// (rt) update the palette texture
	int dstPitch = rect.Pitch;
	int srcPitch = (!gbRunGame && !LoadingScreenCEL /*Handler1 != LoadingWindowHandler*/) ? ScreenWidth : WorkingWidth;
#if defined(MORE_COLOR)
	ushort* dst = (ushort*)rect.pBits;
	for (int row = 0; row < ScreenHeight; ++row) {
		for (int col = 0; col < ScreenWidth; ++col) {
			*dst++ = *src++;
		}
		dst += dstPitch / sizeof(*dst) - ScreenWidth;
		src += srcPitch - ScreenWidth;
	}
#else
	if (!g_A8L8Lighting) {
		// L8 path: simple memcpy, one byte per pixel (palette index only).
		uchar* dst = (uchar*)rect.pBits;
		for (int row = 0; row < ScreenHeight; ++row) {
			memcpy(dst, src, ScreenWidth);
			dst += dstPitch;
			src += srcPitch;
		}
	}
	else {
		// A8L8 interleave: low byte = palette index, high byte = light level.
		// SSE2 fast path: processes 16 pixels per iteration using _mm_unpacklo/hi_epi8
		// to interleave palette and light bytes without per-pixel shift+OR overhead.
		// Scalar fallback handles residual columns and non-SSE builds.
		{
			ptrdiff_t srcByteOffset = src - SoftSrcSurfacePtr;
			const uchar* srcLight = LightSurface + srcByteOffset;
			ushort* dst16 = (ushort*)rect.pBits;
			int     dstPitch16 = dstPitch / (int)sizeof(ushort);
			for (int row = 0; row < ScreenHeight; ++row) {
				int col = 0;
#if defined(_M_IX86) || defined(_M_X64)
				for (; col + 16 <= ScreenWidth; col += 16) {
					__m128i palV = _mm_loadu_si128((const __m128i*)(src + col));
					__m128i lightV = _mm_loadu_si128((const __m128i*)(srcLight + col));
					// A8L8: low byte = L (palette index), high byte = A (light level)
					// unpacklo_epi8(a,b) = a[0],b[0], a[1],b[1], ..., a[7],b[7]
					_mm_storeu_si128((__m128i*)(dst16 + col), _mm_unpacklo_epi8(palV, lightV));
					_mm_storeu_si128((__m128i*)(dst16 + col + 8), _mm_unpackhi_epi8(palV, lightV));
				}
#endif
				for (; col < ScreenWidth; ++col) {
					dst16[col] = (ushort)((srcLight[col] << 8) | src[col]);
				}
				dst16 += dstPitch16;
				src += srcPitch;
				srcLight += srcPitch;
			}
		}
	} // g_A8L8Lighting
#endif
	tex->UnlockRect(0);
	if (D3D_OK != pal->LockRect(0, &rect, 0, 0)) return 0; // (rt) update the palette 
	uint* dstPal = (uint*)rect.pBits;
	PALETTEENTRY* srcPal = ScreenPalette;
	for (int i = 0; i < 256; ++i) {
		*dstPal++ = srcPal->peBlue + (srcPal->peGreen << 8) + (srcPal->peRed << 16);
		++srcPal;
	}
	pal->UnlockRect(0);
	return 1;
}

//----- (th2) -------------------------------------------------------------
int RenderQuadTexture(LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 tex, LPDIRECT3DTEXTURE9 pal, LPDIRECT3DSURFACE9 vid, uchar* src)
{
	// (th4) DYNAMIC ZOOM: the world snapshot first, through the same upload + scene shader, into ZoomWorldTex
	LPDIRECT3DSURFACE9 rt0 = 0;
	bool zoom = Zoom_Prepare( dev );
	if( zoom ){
		if( D3D_OK != dev->GetRenderTarget( 0, &rt0 ) ) zoom = false;
	}
	if( zoom ){
		uchar* wsrc = DynZoom_BeginSnapshotUpload( src );
		const int up = RenderQuadTexture_Upload( tex, pal, wsrc );
		DynZoom_EndSnapshotUpload();
		zoom = up && Zoom_DrawScene( dev, ZoomWorldTex );
		dev->SetRenderTarget( 0, rt0 );
		rt0->Release(); rt0 = 0;
	}
	if( !RenderQuadTexture_Upload( tex, pal, src ) ) return 0;
	//D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurf = 0;
	if (D3D_OK == dev->GetRenderTarget(0, &pSurf)) {
		if (D3D_OK != pSurf->GetDesc(&desc)) { pSurf->Release(); return 0; }
		bool useRenderTexture = FilteringMode != D3DTEXF_NONE && (desc.Width != ScreenWidth || desc.Height != ScreenHeight);
		float width = (float)(useRenderTexture ? ScreenWidth : desc.Width) - 0.5f;
		float height = (float)(useRenderTexture ? ScreenHeight : desc.Height) - 0.5f;
		TransformedVertex QuadVertex[4] = {
			{ {  -0.5,   -0.5, 0, 1 }, { 0, 0 } },
			{ { width,   -0.5, 0, 1 }, { 1, 0 } },
			{ {  -0.5, height, 0, 1 }, { 0, 1 } },
			{ { width, height, 0, 1 }, { 1, 1 } }
		};
		// (th4) DYNAMIC ZOOM: the finished frame into ZoomFinalTex, then the composite into the real target
		if( zoom && !( Zoom_DrawScene( dev, ZoomFinalTex )
		            && Zoom_Composite( dev, useRenderTexture ? vid : pSurf, useRenderTexture ? ScreenWidth : (int)desc.Width,
		                                    useRenderTexture ? ScreenHeight : (int)desc.Height ) ) ){
			zoom = false;
			dev->SetRenderTarget( 0, pSurf );	// fall back to the plain quad below
		}
		if( !zoom ){
		if (useRenderTexture) {
			if (D3D_OK != dev->SetRenderTarget(0, vid)) { pSurf->Release(); return 0; }
		}
		if (D3D_OK != dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1)) { pSurf->Release(); return 0; }
		if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, QuadVertex, sizeof(TransformedVertex))) { pSurf->Release(); return 0; }
		}
		if( dev == D3dDevice ) DynZoom_EndFrame( zoom );	// (th4) the screen shows the zoomed frame, or the plain one

		if (gbRunGame == 1 && AutomapMode && MapBlend) {
			if (D3D_OK != dev->SetPixelShader(DirectShader)) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_LINELIST, AutomapLinePointsCount / 2, &AutomapLines, sizeof(line_vertex))) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_POINTLIST, AutomapPointsCount, &AutomapPoints, sizeof(line_vertex))) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->SetPixelShader(PaletteShader)) { pSurf->Release(); return 0; }
		}
		// Animated life/mana orb overlays (full-color RGBA). Drawn into the same
		// target the game quad used, so they inherit identical scaling. No-op
		// unless config PANEL + POTION are both on and we're in gameplay.
		{
			int orbTargetW = useRenderTexture ? ScreenWidth : (int)desc.Width;
			int orbTargetH = useRenderTexture ? ScreenHeight : (int)desc.Height;
			AnimOrbs_DrawD3D(dev, orbTargetW, orbTargetH);
		}
		// (th4) True-colour inventory item icons. Same target as the game quad, so it
		// inherits identical scaling. No-op unless an icon queued this frame has a PNG.
		ItemOverlay_DrawD3D(dev);
		AntialiasPresent(dev, useRenderTexture ? vid : pSurf);	// (th4) config Antialiasing, at game resolution before the upscale
		//-----------------------------------------------------------------------
		if (useRenderTexture) {
			if (D3D_OK != dev->SetRenderTarget(0, pSurf)) { pSurf->Release(); return 0; }
			if (!FsrPresent(dev, vid, pSurf)	// (th4) FSR 1.0 when enabled, else plain / sharp stretch
			 && D3D_OK != SharpStretch(dev, vid, 0, pSurf, 0, FilteringMode)) { pSurf->Release(); return 0; }
		}
		PostFxPresent(dev, pSurf);	// (th4) optional chromatic aberration / film grain, last
	}
	else {
		if (pSurf) pSurf->Release(); return 0;
	}
	if (pSurf) pSurf->Release();
	return 1;
}

//----- (th4) -------------------------------------------------------------
// Shared per-frame palette->RGB table builder. Packing is identical to the
// D3dPalette upload (peBlue | peGreen<<8 | peRed<<16), so a resolved RGB
// word is byte-compatible with the shader's tex1D result. Used by both the
// ultra upload (RenderQuadTextureUltra) and the Stage-2 post-render replay
// (Rle2Draw writes rgbLUT[sv] into SpriteLayerSurface).
void BuildScreenPaletteRGBLUT(uint* out)
{
	PALETTEENTRY* srcPal = ScreenPalette;
	for (int i = 0; i < 256; ++i) {
		out[i] = (uint)srcPal->peBlue | ((uint)srcPal->peGreen << 8) | ((uint)srcPal->peRed << 16);
		++srcPal;
	}
}

// (th4) Stage 4 item 6 (Option A): single palette->RGB resolve row — the exact
// kernel the ST path and the MT workers share. Byte-identical to the pre-MT
// inline loop by construction: same SSE2 gather, same scalar tail, same flag
// row write, per-row pointers derived from the same bases.
static void ResolveRow(int row, const ResolveMTContext& ctx)
{
	uint* dstRow = ctx.dst32 + (size_t)row * ctx.dstPitch32;
	const uchar* srcRow = ctx.src + (size_t)row * ctx.srcPitch;
	const uchar* alphaRow = ctx.srcAlpha ? ctx.srcAlpha + (size_t)row * ctx.srcPitch : nullptr;
	uchar* flagRow = ctx.flag ? ctx.flag + (size_t)row * ctx.srcPitch : nullptr;
	int col = 0;
#if defined(_M_IX86) || defined(_M_X64)
	// (th4) 8 px per step: two 4-px LUT gathers; the 8 alpha bytes go to bit 24 of their lanes by
	// unpacking with zero (byte -> high byte of a word -> high byte of a dword = a << 24), the
	// same value the per-pixel shifts in the 4-px loop below build. That loop and the scalar tail
	// still finish the row. Verified against the previous version (2026-10-02, random rows).
	for (; col + 8 <= ctx.width; col += 8) {
		__m128i rgb0 = _mm_setr_epi32(
			(int)rgbLUT[srcRow[col]],     (int)rgbLUT[srcRow[col + 1]],
			(int)rgbLUT[srcRow[col + 2]], (int)rgbLUT[srcRow[col + 3]]);
		__m128i rgb1 = _mm_setr_epi32(
			(int)rgbLUT[srcRow[col + 4]], (int)rgbLUT[srcRow[col + 5]],
			(int)rgbLUT[srcRow[col + 6]], (int)rgbLUT[srcRow[col + 7]]);
		if (alphaRow) {
			const __m128i z  = _mm_setzero_si128();
			const __m128i a8 = _mm_loadl_epi64((const __m128i*)(alphaRow + col));
			const __m128i a16 = _mm_unpacklo_epi8(z, a8);			// a << 8 per word
			rgb0 = _mm_or_si128(rgb0, _mm_unpacklo_epi16(z, a16));	// a << 24 per dword
			rgb1 = _mm_or_si128(rgb1, _mm_unpackhi_epi16(z, a16));
		}
		_mm_storeu_si128((__m128i*)(dstRow + col), rgb0);
		_mm_storeu_si128((__m128i*)(dstRow + col + 4), rgb1);
	}
	for (; col + 4 <= ctx.width; col += 4) {
		// 4 LUT gathers, one 128-bit store per 4 px
		__m128i rgb = _mm_setr_epi32(
			(int)rgbLUT[srcRow[col]],
			(int)rgbLUT[srcRow[col + 1]],
			(int)rgbLUT[srcRow[col + 2]],
			(int)rgbLUT[srcRow[col + 3]]);
		if (alphaRow) {
			// Load 4 alpha bytes, promote each to bit 24 of its own 32-bit
			// lane, OR with RGB. (2026-09-02 BUGFIX: do NOT "simplify" this
			// back to madd_epi16 — madd ADDS pairs of 16-bit lanes.)
			__m128i aw = _mm_setr_epi32(
				(int)((uint)alphaRow[col]     << 24),
				(int)((uint)alphaRow[col + 1] << 24),
				(int)((uint)alphaRow[col + 2] << 24),
				(int)((uint)alphaRow[col + 3] << 24));
			rgb = _mm_or_si128(rgb, aw);
		}
		_mm_storeu_si128((__m128i*)(dstRow + col), rgb);
	}
#endif
	for (; col < ctx.width; ++col) {
		dstRow[col] = rgbLUT[srcRow[col]] | (alphaRow ? ((uint)alphaRow[col] << 24) : 0u);
	}
	if (flagRow) {
		// vignetting branch carrier: index >= 128 -> 255 (s4, §4.4)
		col = 0;
#if defined(_M_IX86) || defined(_M_X64)
		// (th4) 16 at a time: a signed byte < 0 is exactly index >= 128, and the compare gives 0xFF / 0x00
		for (; col + 16 <= ctx.width; col += 16)
			_mm_storeu_si128((__m128i*)(flagRow + col), _mm_cmplt_epi8(_mm_loadu_si128((const __m128i*)(srcRow + col)), _mm_setzero_si128()));
#endif
		for (; col < ctx.width; ++col) {
			flagRow[col] = srcRow[col] >= 128 ? 255 : 0;
		}
	}
}

// (th4) Stage 4 item 6: worker-facing entry. The epoch guard runs FIRST — a
// stale worker (abandoned on a timeout) must not dereference a context that a
// newer dispatch may have overwritten; it aborts before touching any pointer.
// epoch < 0 = ST / main-thread path, no checks. Returns 1 when the row ran.
int ResolveRowMT(int row, const ResolveMTContext& ctx, int epoch)
{
	if (epoch >= 0 && !PPLCheckEpoch(epoch)) return 0;
	ResolveRow(row, ctx);
	return 1;
}

// (th4) Stage 4 item 5: per-window tint-texture content tracking (white == true).
// File-scope so D3dReset/SecondReset can force the statics back to white after
// the dynamic tint textures are recreated (their content is lost on reset and
// re-white-filled there). Idle frames skip the s3 upload only while the static
// says the texture already holds white.
static bool s_secTintWhite = true, s_mainTintWhite = true;

// (th4) Stage 4 item 5: called from D3dReset (secondWindow == false) and
// SecondReset (true) after the dynamic tint texture was recreated and
// white-filled — the texture content IS white again.
void SetUltraTintWhiteState(bool secondWindow)
{
	if (secondWindow) s_secTintWhite = true; else s_mainTintWhite = true;
}

//----- (th4) -------------------------------------------------------------
// Stage 2 debug aid: while a file named "overlay_dump_on.flag" exists in the
// process working directory, dump the overlay source surfaces every 30 frames
// (overlay_dump_{alpha,sprite,wob,ws}.raw = visible region, 8/32/8/8 bpp).
// Delete the flag file to stop. Inspect with any raw-image viewer or Python.
static void DumpOverlaySurfacesDebug(const uchar* ws, const uchar* alpha, const uint* sprite, const uchar* wob, const uint* tint, int srcByteOffset, int srcPitch)
{
	const uchar* pA = alpha ? alpha + srcByteOffset : nullptr;
	const uint* pS = sprite ? sprite + srcByteOffset : nullptr;
	const uchar* pW = wob ? wob + srcByteOffset : nullptr;
	const uint* pT = tint ? tint + srcByteOffset : nullptr;
	uchar* zeroBuf = (uchar*)calloc(ScreenWidth, 4);
	FILE* f;
	if ((f = fopen("overlay_dump_alpha.raw", "wb"))) {
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(pA ? pA + row * srcPitch : zeroBuf, 1, ScreenWidth, f);
		fclose(f);
	}
	if ((f = fopen("overlay_dump_sprite.raw", "wb"))) {
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(pS ? (const uchar*)(pS + row * srcPitch) : zeroBuf, 1, ScreenWidth * sizeof(uint), f);
		fclose(f);
	}
	if ((f = fopen("overlay_dump_wob.raw", "wb"))) {
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(pW ? pW + row * srcPitch : zeroBuf, 1, ScreenWidth, f);
		fclose(f);
	}
	if ((f = fopen("overlay_dump_ws.raw", "wb"))) {
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(ws + row * srcPitch, 1, ScreenWidth, f);
		fclose(f);
	}
	// (th4) 2026-09-02: also dump the post-zoom LightSurface -- the composite
	// mixes wall*lit(light) with the overlay, so the light field is needed to
	// judge whether per-pixel dark columns come from the wall's light.
	if ((f = fopen("overlay_dump_light.raw", "wb"))) {
		const uchar* pL = LightSurface ? LightSurface + srcByteOffset : nullptr;
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(pL ? pL + row * srcPitch : zeroBuf, 1, ScreenWidth, f);
		fclose(f);
	}
	// (th4) Stage 3: dump the tint field (BGRA words, visible region) — the
	// ST-vs-MT A/B and the red-pool visual check both read this dump.
	if ((f = fopen("overlay_dump_tint.raw", "wb"))) {
		for (int row = 0; row < ScreenHeight; ++row)
			fwrite(pT ? (const uchar*)(pT + row * srcPitch) : zeroBuf, 1, ScreenWidth * sizeof(uint), f);
		fclose(f);
	}
	free(zeroBuf);
}

//----- (th4) -------------------------------------------------------------
// Ultra two-texture upload: A8R8G8B8 color (s0) + L8 light (s1) + L8
// vignetting flag (s4).  The palette->RGB resolve moves from the GPU
// (tex1D in every shader) to the CPU here: a 256-entry LUT built once per
// frame from ScreenPalette with the same packing as the D3dPalette upload.
// Stage 1 shipped the A byte as constant 255 (output-alpha parity with the
// X8R8G8B8 palette texel); Stage 2+ merges AlphaSurface instead.
// (th4) DYNAMIC ZOOM: the upload half of RenderQuadTextureUltra, so it can run for the world snapshot too.
static int RenderQuadTextureUltra_Upload(LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 texColor, LPDIRECT3DTEXTURE9 texLight, LPDIRECT3DTEXTURE9 texFlag, LPDIRECT3DTEXTURE9 texOverlay, LPDIRECT3DTEXTURE9 texTint, uchar* src)
{
	// (th4) Stage 4 item 5 step 3: s0 (color) is dynamic+default too, so after
	// a failed device reset its handle can be null while the device is lost —
	// the old "managed color lock fails first" safety net no longer exists.
	// Guard the two unconditionally-locked textures here (s2/s3/s4 are already
	// guarded at their own sites).
	if (!texColor || !texLight) return 0;
	D3DLOCKED_RECT rect;
	// per-frame palette->RGB LUT (peBlue | peGreen<<8 | peRed<<16)
	BuildScreenPaletteRGBLUT(rgbLUT);
	int srcPitch = (!gbRunGame && !LoadingScreenCEL /*Handler1 != LoadingWindowHandler*/) ? ScreenWidth : WorkingWidth;
	ptrdiff_t srcByteOffset = src - SoftSrcSurfacePtr; // same offset logic as the A8L8 branch
	const bool writeFlag = FullscreenFX == 1 && IndexFlagSurface != nullptr;
	// (th4) Stage 2: overlay weight source. Feature on -> merge AlphaSurface
	// into the A byte; feature off -> constant 0 (no overlay; the Stage-2
	// shaders never forward rgba.a as output alpha).
	const bool alphaOn = g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface != nullptr;
	const uchar* srcAlpha = alphaOn ? AlphaSurface + srcByteOffset : nullptr;
	// (th4) Stage 2 debug dump gate (see DumpOverlaySurfacesDebug above).
#ifdef AlphaBlendingDebug
	{
		static int overlayDumpFrame = 0;
		if (++overlayDumpFrame >= 30) {
			overlayDumpFrame = 0;
			FILE* probe = fopen("overlay_dump_on.flag", "rb");
			if (probe) {
				fclose(probe);
				DumpOverlaySurfacesDebug(src, AlphaSurface, SpriteLayerSurface, WallOcclusionSurface, LightTintSurface, (int)srcByteOffset, srcPitch);
			}
		}
	}
#endif
	// color pass: dst32 = rgbLUT[src[i]] | (alpha << 24) (SSE2 4 px/iteration)
	// (th4) Stage 4 item 5 step 3: s0 is dynamic — full-surface write, DISCARD
	// lock (never waits on the GPU, never copies on unlock).
	if (D3D_OK != texColor->LockRect(0, &rect, 0, D3DLOCK_DISCARD)) return 0;
	{
		ResolveMTContext ctx;
		ctx.dst32 = (uint*)rect.pBits;
		ctx.dstPitch32 = (int)(rect.Pitch / sizeof(uint));
		ctx.src = src;
		ctx.srcPitch = srcPitch;
		ctx.srcAlpha = srcAlpha;
		ctx.flag = writeFlag ? IndexFlagSurface + srcByteOffset : nullptr;
		ctx.width = ScreenWidth;
		ctx.height = ScreenHeight;
		// (th4) Stage 4 item 5 step 3 (2026-09-07, final = Option A after in-game A/B):
		// s0 is a D3DUSAGE_DYNAMIC texture locked with D3DLOCK_DISCARD, and the
		// palette->RGB resolve is dispatched over the shared PPL pool (item 6).
		// Measured A/B (same build, same scenes): MT resolve is ~+6% in the
		// quest dungeon (the scene that matters — 350 vs 330 with ST) because
		// the gather dominates the frame there, but ~-9% in idle town (370 vs
		// 405 with ST) because the pool wake/claim overhead exceeds the gain
		// on a tiny hot task. No WC-write pathology was observed on the
		// dynamic buffer, so Option A is a MEASURED choice. To fall back to ST
		// (Option B), comment this gate out and rebuild.
		if (!MultiThreadPixelLight || !PPLResolveDispatch(ctx)) {
			for (int row = 0; row < ctx.height; ++row) {
				ResolveRow(row, ctx);
			}
		}
	}
	// (th4) debug aid 2026-09-02: dump the PACKED color texture (the bytes the
	// shader actually samples) — the earlier overlay dumps captured the system
	// buffers, which hid an upload-side alpha packing bug.
#ifdef AlphaBlendingDebug
	{
		static int texDumpFrame = 0;
		if (++texDumpFrame >= 30) {
			texDumpFrame = 0;
			FILE* probe = fopen("overlay_dump_on.flag", "rb");
			if (probe) {
				fclose(probe);
				FILE* f = fopen("overlay_dump_texcolor.raw", "wb");
				if (f) {
					for (int row = 0; row < ScreenHeight; ++row)
						fwrite((uchar*)rect.pBits + row * rect.Pitch, 1, ScreenWidth * 4, f);
					fclose(f);
				}
			}
		}
	}
#endif
	texColor->UnlockRect(0);
	// light pass: pitch-adjusted row memcpy of LightSurface (post-zoom bytes)
	// (th4) Stage 4 item 5: s1 is dynamic — full-surface write, DISCARD lock.
	if (D3D_OK != texLight->LockRect(0, &rect, 0, D3DLOCK_DISCARD)) return 0;
	{
		uchar* dstL8 = (uchar*)rect.pBits;
		const uchar* srcLight = LightSurface + srcByteOffset;
		for (int row = 0; row < ScreenHeight; ++row) {
			memcpy(dstL8, srcLight, ScreenWidth);
			dstL8 += rect.Pitch;
			srcLight += srcPitch;
		}
	}
	texLight->UnlockRect(0);
	// (th4) Stage 2: overlay upload (s2) — row memcpy of SpriteLayerSurface.
	// SpriteLayerSurface is uint* indexed by PIXEL offset (4 bytes per pixel),
	// so uint* row advance of srcPitch elements = 4*srcPitch bytes.
	// Runs only when the alpha feature is on; at weight 0 the shader never
	// uses the overlay and the texture stays zero-filled from creation.
	// (th4) Stage 4 item 5: s2 is dynamic — full-surface write, DISCARD lock.
	if (alphaOn && texOverlay && SpriteLayerSurface) {
		if (D3D_OK != texOverlay->LockRect(0, &rect, 0, D3DLOCK_DISCARD)) return 0;
		{
			uchar* dstO = (uchar*)rect.pBits;
			const uint* srcO = SpriteLayerSurface + srcByteOffset;
			for (int row = 0; row < ScreenHeight; ++row) {
				memcpy(dstO, srcO, ScreenWidth * sizeof(uint));
				dstO += rect.Pitch;
				srcO += srcPitch;
			}
		}
		texOverlay->UnlockRect(0);
	}
	// (th4) Stage 3: tint upload (s3) — plain row memcpy of LightTintSurface,
	// no per-pixel packing (the buffer byte layout matches the A8R8G8B8
	// texture). Idle gate (Stage 4 item 4): with no colored light active the
	// shaders must sample a WHITE tint texture — multiply by 1.0, bit-exact
	// with the pre-Stage-3 pipeline — so the per-frame 8 MB upload is skipped
	// and the texture is white-filled ONCE per colored->idle transition (per
	// window). The textures are created white-filled (CreateScreenSurface /
	// ChangeScreenRes) and re-white-filled on device reset (D3dReset /
	// SecondReset call SetUltraTintWhiteState); content persists across
	// frames as long as the texture is not re-locked, so the statics track
	// per-texture content. A stale static can only cause one redundant white
	// fill, never a wrong color. (th4) Stage 4 item 5: s3 is dynamic — the
	// lock must be D3DLOCK_DISCARD (full-surface write).
	if (texTint) {
		const bool tintActive = g_LightTintActive && LightTintSurface != nullptr;
		bool& tintWhite = (texTint == SecTextureTint) ? s_secTintWhite : s_mainTintWhite;
		if (tintActive || !tintWhite) {
			if (D3D_OK != texTint->LockRect(0, &rect, 0, D3DLOCK_DISCARD)) return 0;
			{
				uchar* dstT = (uchar*)rect.pBits;
				if (tintActive) {
					const uint* srcT = LightTintSurface + srcByteOffset;
					for (int row = 0; row < ScreenHeight; ++row) {
						memcpy(dstT, srcT, ScreenWidth * sizeof(uint));
						dstT += rect.Pitch;
						srcT += srcPitch;
					}
					tintWhite = false;
				} else {
					// colored -> idle transition: one white fill (all bytes
					// 0xFF = white in A8R8G8B8), then idle frames skip.
					for (int row = 0; row < ScreenHeight; ++row) {
						memset(dstT, 0xFF, ScreenWidth * sizeof(uint));
						dstT += rect.Pitch;
					}
					tintWhite = true;
				}
			}
			texTint->UnlockRect(0);
		}
	}
	// flag upload — only when FullscreenFX == 1 (the only shader that samples s4)
	if (writeFlag && texFlag) {
		if (D3D_OK != texFlag->LockRect(0, &rect, 0, 0)) return 0;
		{
			uchar* dstFlag = (uchar*)rect.pBits;
			const uchar* srcFlag = IndexFlagSurface + srcByteOffset;
			for (int row = 0; row < ScreenHeight; ++row) {
				memcpy(dstFlag, srcFlag, ScreenWidth);
				dstFlag += rect.Pitch;
				srcFlag += srcPitch;
			}
		}
		texFlag->UnlockRect(0);
	}
	// (th4) Stage 2 rungs 2-3: pixel-shader constant c0 selects the overlay
	// weight shaping used by sampleLit in all 7 RGB shaders:
	//   x: 1 = rung 2 contrast-weighted alpha (0 = rung 1 constant)
	//   y: 1 = rung 3 rim emphasis — mask edges keep full base alpha.
	//       NOTE ps_2_0 has no flow control and fxc hoists texture reads
	//       above uniform branches, so a runtime gate could never skip the
	//       rim taps: the rim family is compiled from the *ShaderRimRGB.hlsl
	//       wrapper sources and SwitchVirtualShader picks the family
	//       (duplicate FxCompile Includes on one source are not allowed —
	//       VS rejects them at project load).
	//   z,w: texel size (1/ScreenWidth, 1/ScreenHeight) for the rim taps
	{
		float alphaContrastConst[4] = {
			g_ARGBAlphaContrast ? 1.0f : 0.0f,
			g_ARGBAlphaRim ? 1.0f : 0.0f,
			ScreenWidth > 0 ? 1.0f / ScreenWidth : 0.0f,
			ScreenHeight > 0 ? 1.0f / ScreenHeight : 0.0f
		};
		dev->SetPixelShaderConstantF(0, alphaContrastConst, 1);
	}
	return 1;
}

int RenderQuadTextureUltra(LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 texColor, LPDIRECT3DTEXTURE9 texLight, LPDIRECT3DTEXTURE9 texFlag, LPDIRECT3DTEXTURE9 texOverlay, LPDIRECT3DTEXTURE9 texTint, LPDIRECT3DSURFACE9 vid, uchar* src)
{
	if (!texColor || !texLight) return 0;
	// (th4) DYNAMIC ZOOM: the world snapshot first, through the same upload + scene shader, into ZoomWorldTex
	LPDIRECT3DSURFACE9 rt0 = 0;
	bool zoom = Zoom_Prepare( dev );
	if( zoom ){
		if( D3D_OK != dev->GetRenderTarget( 0, &rt0 ) ) zoom = false;
	}
	if( zoom ){
		uchar* wsrc = DynZoom_BeginSnapshotUpload( src );
		const int up = RenderQuadTextureUltra_Upload( dev, texColor, texLight, texFlag, texOverlay, texTint, wsrc );
		DynZoom_EndSnapshotUpload();
		zoom = up && Zoom_DrawScene( dev, ZoomWorldTex );
		dev->SetRenderTarget( 0, rt0 );
		rt0->Release(); rt0 = 0;
	}
	if( !RenderQuadTextureUltra_Upload( dev, texColor, texLight, texFlag, texOverlay, texTint, src ) ) return 0;
	// no palette upload in ultra mode — the RGB shaders have no tex1D
	//D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurf = 0;
	if (D3D_OK == dev->GetRenderTarget(0, &pSurf)) {
		if (D3D_OK != pSurf->GetDesc(&desc)) { pSurf->Release(); return 0; }
		bool useRenderTexture = FilteringMode != D3DTEXF_NONE && (desc.Width != ScreenWidth || desc.Height != ScreenHeight);
		// (th4) 2026-09-10: vid is only used when stretching through the
		// intermediate target; after a failed reset it can be null (the
		// texture uploads above are already null-guarded). Skip instead of
		// entering d3d9 with it.
		if (useRenderTexture && !vid) {
			D3DDIAG("ultra draw skipped: VideoSurface is null (failed reset?)");
			pSurf->Release();
			return 0;
		}
		float width = (float)(useRenderTexture ? ScreenWidth : desc.Width) - 0.5f;
		float height = (float)(useRenderTexture ? ScreenHeight : desc.Height) - 0.5f;
		TransformedVertex QuadVertex[4] = {
			{ {  -0.5,   -0.5, 0, 1 }, { 0, 0 } },
			{ { width,   -0.5, 0, 1 }, { 1, 0 } },
			{ {  -0.5, height, 0, 1 }, { 0, 1 } },
			{ { width, height, 0, 1 }, { 1, 1 } }
		};
		// (th4) DYNAMIC ZOOM: the finished frame into ZoomFinalTex, then the composite into the real target
		if( zoom && !( Zoom_DrawScene( dev, ZoomFinalTex )
		            && Zoom_Composite( dev, useRenderTexture ? vid : pSurf, useRenderTexture ? ScreenWidth : (int)desc.Width,
		                                    useRenderTexture ? ScreenHeight : (int)desc.Height ) ) ){
			zoom = false;
			dev->SetRenderTarget( 0, pSurf );	// fall back to the plain quad below
		}
		if( !zoom ){
		if (useRenderTexture) {
			if (D3D_OK != dev->SetRenderTarget(0, vid)) { pSurf->Release(); return 0; }
		}
		if (D3D_OK != dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1)) { pSurf->Release(); return 0; }
		if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, QuadVertex, sizeof(TransformedVertex))) { pSurf->Release(); return 0; }
		}
		if( dev == D3dDevice ) DynZoom_EndFrame( zoom );	// (th4) the screen shows the zoomed frame, or the plain one

		if (gbRunGame == 1 && AutomapMode && MapBlend) {
			if (D3D_OK != dev->SetPixelShader(DirectShader)) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_LINELIST, AutomapLinePointsCount / 2, &AutomapLines, sizeof(line_vertex))) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->DrawPrimitiveUP(D3DPT_POINTLIST, AutomapPointsCount, &AutomapPoints, sizeof(line_vertex))) { pSurf->Release(); return 0; }
			if (D3D_OK != dev->SetPixelShader(PaletteShader)) { pSurf->Release(); return 0; }
		}
		// Animated life/mana orb overlays (full-color RGBA). Drawn into the same
		// target the game quad used, so they inherit identical scaling. No-op
		// unless config PANEL + POTION are both on and we're in gameplay.
		{
			int orbTargetW = useRenderTexture ? ScreenWidth : (int)desc.Width;
			int orbTargetH = useRenderTexture ? ScreenHeight : (int)desc.Height;
			AnimOrbs_DrawD3D(dev, orbTargetW, orbTargetH);
		}
		// (th4) True-colour inventory item icons. Same target as the game quad, so it
		// inherits identical scaling. No-op unless an icon queued this frame has a PNG.
		ItemOverlay_DrawD3D(dev);
		AntialiasPresent(dev, useRenderTexture ? vid : pSurf);	// (th4) config Antialiasing, at game resolution before the upscale
		//-----------------------------------------------------------------------
		if (useRenderTexture) {
			if (D3D_OK != dev->SetRenderTarget(0, pSurf)) { pSurf->Release(); return 0; }
			if (!FsrPresent(dev, vid, pSurf)	// (th4) FSR 1.0 when enabled, else plain / sharp stretch
			 && D3D_OK != SharpStretch(dev, vid, 0, pSurf, 0, FilteringMode)) { pSurf->Release(); return 0; }
		}
		PostFxPresent(dev, pSurf);	// (th4) optional chromatic aberration / film grain, last
	}
	else {
		if (pSurf) pSurf->Release(); return 0;
	}
	if (pSurf) pSurf->Release();
	return 1;
}

//----- (th2) -------------------------------------------------------------
int RenderSurface(LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 vid, LPDIRECT3DSURFACE9 sur, LPDIRECT3DSURFACE9 buf, int startingX, int startingY, uchar* src, int pixelFormat, int srcPitch, int srcWidth, int srcHeight, PALETTEENTRY* screenPalette, RECT& srcRect, RECT& dstRect)
{
	int dstPitch = 0;
	void* dst = 0;
	D3DLOCKED_RECT rect;
	// (th4) 2026-09-10: guard for the d3d9 crash investigation — null handles
	// here (failed reset / full device release) used to reach d3d9 directly.
	if (!dev || !vid || !sur || !buf) {
		D3DDIAG("RenderSurface skipped: dev=%p vid=%p sur=%p buf=%p", (void*)dev, (void*)vid, (void*)sur, (void*)buf);
		return 0;
	}
	if (D3D_OK != sur->LockRect(&rect, 0, 0)) return 0;
	dstPitch = rect.Pitch;
	dst = rect.pBits;
	switch (pixelFormat) {
	case 15: drawPixels<ushort, 32, 32, 32, 5, 10>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
	case 16: drawPixels<ushort, 32, 64, 32, 5, 11>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
	case 32: drawPixels<uint, 256, 256, 256, 8, 16>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
	}
	sur->UnlockRect();
	if (!gbRunGame) {
		if (D3D_OK != dev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0 /*a++*/), 1.0f, 0)) return 0;
	}
	if (!gbRunGame && !VideoWindowExists) { // (rt) main menu
		if (D3D_OK != dev->UpdateSurface(sur, &srcRect, vid, 0)) return 0;
		if (D3D_OK != SharpStretch(dev, vid, gbRunGame ? 0 : &srcRect, buf, gbRunGame ? 0 : &dstRect, FilteringMode)) return 0;
	}
	else if (VideoWindowExists || LoadingScreenCEL) { // (rt) video and loading screen
		if (D3D_OK != dev->UpdateSurface(sur, 0, vid, 0)) return 0;
		if (D3D_OK != SharpStretch(dev, vid, &srcRect, buf, 0, FilteringMode)) return 0;
	}
	else {  // (rt) gameplay in surface mode
		if (D3D_OK != dev->UpdateSurface(sur, 0, vid, 0)) return 0;
		if (D3D_OK != SharpStretch(dev, vid, 0, buf, 0, FilteringMode)) return 0;
	}
	return 1;
}

// (th4) 2026-09-10: device-lost reset policy (MP "New Game" crash follow-up).
// In exclusive fullscreen — the only case where SetD3dParam builds real
// fullscreen parameters (IsFullScreen && !BigWin) — a deactivated window
// cannot re-acquire the device: D3dReset() "succeeds", but the next Present
// is lost again, so the draw path below used to call D3dReset() once per
// frame while the game was in the background (observed: 35 resets in 0.7 s).
// While the window is not active, allow at most one attempt per interval as a
// watchdog, so a stale activation flag can never lock the game out of
// recovery; the moment the window is active again the gate returns true and
// the next failed draw resets normally (<= 1 frame). Windowed / BigWin paths
// are unchanged (return true).
//
// HISTORY — why an ACTIVE window is NOT paced (reverted 2026-09-10 evening):
// a Phase 2 back-off (commit 1115b6e14) refused repeat resets while the window
// was active (200 ms / 500 ms between attempts). In its first run
// (th4_d3d_diag_1115b6e1.log) the next Present() after a refused reset did NOT
// return for 33.7 s: the game froze with a dead message pump (Alt+Tab dead)
// until a Task Manager desktop switch made Present return 0x88760868
// (DEVICELOST). With the immediate-reset policy below, the same
// post-transition wedge produced only fast-failing Presents (0x88760873,
// 35 resets in 0.6 s — Phase 0 log) and healed at the next state transition.
// Do NOT re-introduce pacing between failed draws without explaining that
// 33.7 s stall first.
static const DWORD D3D_RESET_INACTIVE_RETRY_MS = 1000;  // window not active: watchdog
static DWORD s_lastInactiveResetAttempt = 0;            // 0 = none since last active
// Phase 2 back-off state, DISABLED 2026-09-10 (freeze, see HISTORY above).
// Kept for reference — do not re-enable:
//   static const DWORD D3D_RESET_RETRY1_MS = 200;
//   static const DWORD D3D_RESET_RETRY2_MS = 500;
//   static const DWORD D3D_RESET_EPISODE_IDLE_MS = 2000;
//   static DWORD s_lastLostDeviceReset = 0;
//   static DWORD s_lastLostDeviceFail = 0;
//   static int   s_lostDeviceAttempts = 0;
static bool D3dLostDeviceResetAllowed()
{
	if (!IsFullScreen || BigWin) return true; // not exclusive fullscreen: unchanged
#ifdef TH4_D3D_DIAG_ENABLED
	// Phase 2(c) diagnostic (logging only — it does not affect the decision):
	// name the window that took the foreground. The Phase 0 log shows the
	// hand-over is IN-PROCESS (WM_ACTIVATEAPP never fires), so class+pid tell
	// us which of our own windows it is. One line per hand-over; re-armed when
	// the main window owns the foreground again.
	{
		static bool s_fgHandoverLogged = false;
		const HWND fg = GetForegroundWindow();
		if (fg == ghMainWnd) {
			s_fgHandoverLogged = false;
		} else if (!s_fgHandoverLogged) {
			s_fgHandoverLogged = true;
			char cls[64] = "?";
			GetClassNameA(fg, cls, sizeof(cls));
			DWORD fgPid = 0;
			GetWindowThreadProcessId(fg, &fgPid);
			D3DDIAG("foreground is not the main window: hwnd=%p class='%s' pid=%lu (our pid=%lu) active=%d minimized=%d",
				(void*)fg, cls, fgPid, GetCurrentProcessId(), IsMainWindowActive, (int)IsMainWindowMinimized);
		}
	}
#endif
	if (IsMainWindowActive && !IsMainWindowMinimized) {
		s_lastInactiveResetAttempt = 0; // re-arm the watchdog
		return true;
	}
	const DWORD now = GetTickCount(); // unsigned subtraction wraps safely
	bool allowed = true;
	if (s_lastInactiveResetAttempt != 0 &&
		(now - s_lastInactiveResetAttempt) < D3D_RESET_INACTIVE_RETRY_MS) {
		allowed = false;
	} else {
		s_lastInactiveResetAttempt = now;
	}
#ifdef TH4_D3D_DIAG_ENABLED
	// Throttled here: the refused path runs once per failed draw (frame rate)
	// while the storm lasts, so the call sites stay silent and this logs about
	// one line per second.
	if (!allowed) {
		static DWORD s_lastSkipLog = 0;
		if (now - s_lastSkipLog > 1000) {
			s_lastSkipLog = now;
			D3DDIAG("device-reset skipped: exclusive fullscreen, window not active (watchdog retries in %u ms)",
				(unsigned)D3D_RESET_INACTIVE_RETRY_MS);
		}
	}
#endif
	return allowed;
}

//----- (th2) -------------------------------------------------------------
LPDIRECTDRAWSURFACE __fastcall ConvertPaletteSurfaceToTrueColor(uchar* src)
{
	// (th4) 2026-09-10 reentrancy guard (root fix for the MP "New Game" d3d9
	// crash): this SDraw callback is invoked by Storm's dialogs from nested
	// message pumps. If such a message is dispatched while D3dReset() is inside
	// IDirect3DDevice9::Reset(), touching the device here is fatal (crash dump:
	// reentrant BeginScene() -> AV in d3d9+0x663D1). Skip the frame instead.
	if (g_D3dInReset) {
#ifdef TH4_D3D_DIAG_ENABLED
		static DWORD s_lastResetSkipLog = 0;
		const DWORD now = GetTickCount();
		if (now - s_lastResetSkipLog > 1000) {
			s_lastResetSkipLog = now;
			D3DDIAG("menu draw skipped: device reset in progress");
		}
#endif
		return ScreenSurface;
	}
	if ((uint)src < 0x000'0100u) {
		//__debugbreak();
		return ScreenSurface;
	}
	PALETTEENTRY* screenPalette = gbRunGame && !VideoWindowExists ? ScreenPalette : (PALETTEENTRY*)(StormReloc + 0x1505E670 /*0x15039B14*/); // (rt) storm working palette 1611
	int pixelFormat = D3dRef ? 16 : 32;
	int width = IsFullScreen && !BigWin ? ScreenWidth : WinWidth;
	int height = IsFullScreen && !BigWin ? ScreenHeight : WinHeight;
	int gui_width = IsFullScreen && !BigWin || ScreenWidth == WinWidth ? GUI_Width : (int)((double)WinWidth / ScreenWidth * GUI_Width);
	int gui_height = IsFullScreen && !BigWin || ScreenHeight == WinHeight ? GUI_Height : (int)((double)WinHeight / ScreenHeight * GUI_Height);
	POINT metaMenuPoint{ (width - gui_width) / 2, (height - gui_height) / 2 };
	RECT classicRect{ 0, 0, GUI_Width - 1, GUI_Height - 1 };
	RECT metaMenuRect{ metaMenuPoint.x, metaMenuPoint.y, metaMenuPoint.x + gui_width - 1, metaMenuPoint.y + gui_height - 1 };
	// (th4) Keep the mouse inside the menu. NOTE it is classicRect that goes in, not
	// metaMenuRect: the physical pointer lives in the 640x480 dialog space anchored at the
	// client origin -- that is the space Storm lays its dialogs out in and the space
	// GetWindowRect reports them in -- while metaMenuRect is only where that space is DRAWN
	// after stretching. Clipping to the drawn rect walls off everything to the left of it.
	// Recomputed every frame, so a window move or resize is picked up for free. No-op during
	// gameplay and video.
	RestrictMouseToMenu(ghMainWnd, classicRect);
	int startingX = 0;
	int startingY = 0;//gbRunGame ? 0 : updatingPoint.y;
	if (DDraw /*render == ddraw*/) {
#if ADJUST_WINDOW_RECT_WITH_MENU
		RECT window, client = { 0, 0, ScreenWidth, ScreenHeight };
		GetWindowRect(ghMainWnd, &window);
		AdjustWindowRectEx(&client, GetWindowLong(ghMainWnd, GWL_STYLE), GetMenu(ghMainWnd) != NULL, GetWindowLong(ghMainWnd, GWL_EXSTYLE));
		startingX = window.left - client.left;
		startingY = window.top - client.top;
#endif
		DDSURFACEDESC surfDesc32;
		surfDesc32.dwSize = sizeof(surfDesc32);
		// draw backbuffer
		ScreenSurface32Back->Lock(nullptr, &surfDesc32, /*DDLOCK_WRITEONLY |*/ DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0);
		pixelFormat = surfDesc32.ddpfPixelFormat.dwRGBBitCount;
		int dstPitch = surfDesc32.lPitch;
		void* dst = surfDesc32.lpSurface;
		int srcPitch = (VideoWindowExists || (!gbRunGame && !LoadingScreenCEL/* Handler1 != LoadingWindowHandler*/)) ? ScreenSurfacePitch : WorkingWidth;
		int srcWidth = gbRunGame && !VideoWindowExists ? ScreenWidth : GUI_Width;
		int srcHeight = gbRunGame && !VideoWindowExists ? ScreenHeight : GUI_Height;
		switch (pixelFormat) {
		case 15: drawPixels<ushort, 32, 32, 32, 5, 10>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
		case 16: drawPixels<ushort, 32, 64, 32, 5, 11>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
		case 32: drawPixels<uint, 256, 256, 256, 8, 16>(startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette); break;
		}
		ScreenSurface32Back->Unlock(0);
		// blit/flip on screen
		POINT p = { 0, 0 };
		ClientToScreen(ghMainWnd, &p);
		RECT windowRect;
		GetClientRect(ghMainWnd, &windowRect);
		OffsetRect(&windowRect, p.x, p.y);
		RECT* srcRect = 0; // gameplay and loading screen - full rect
		if (!gbRunGame && !VideoWindowExists) { // meta menu
			windowRect.right = windowRect.left + metaMenuRect.right;
			windowRect.bottom = windowRect.top + metaMenuRect.bottom;
			windowRect.left += metaMenuRect.left;
			windowRect.top += metaMenuRect.top;
			srcRect = &classicRect;
		}
		else if (VideoWindowExists) { // video
			srcRect = &classicRect;
		}
		HRESULT hr = ScreenSurface32->Blt(&windowRect, ScreenSurface32Back, srcRect, DDBLT_WAIT, NULL);
	}
	else { /*render == d3d*/
		int ok = -1;
		bool second = IsSecondWin && (OnlySecondWin || VideoWindowExists || !gbRunGame || LoadingScreenCEL /*Handler1 == LoadingWindowHandler*/) && SecDevice != nullptr;
		// (th4) 2026-09-10: defensive guards for the d3d9 callback-crash
		// investigation. After any ReleaseDDSurface path (error/exit) or a
		// failed reset these handles can be null — skip the frame instead of
		// feeding them to d3d9.
		if (!OnlySecondWin && !D3dDevice) {
			D3DDIAG("menu draw skipped: D3dDevice is null");
			return ScreenSurface;
		}
		HRESULT d3dOk = D3D_OK;
		if (D3D_OK == (d3dOk = (OnlySecondWin ? D3D_OK : D3dDevice->BeginScene()))) {
			if (second) SecDevice->BeginScene();
			int srcPitch = (VideoWindowExists || (!gbRunGame && !LoadingScreenCEL/* Handler1 != LoadingWindowHandler*/)) ? ScreenSurfacePitch : WorkingWidth;
			int srcWidth = gbRunGame && !VideoWindowExists ? ScreenWidth : GUI_Width;
			int srcHeight = gbRunGame && !VideoWindowExists ? ScreenHeight : GUI_Height;
			if (DoMenuScreenShot && !OnlySecondWin) {
				ShotPalette = screenPalette;
				ShotSrc = src;
				ShotPitch = srcPitch;
				ShotStartingX = startingX;
				ShotStartingY = startingY;
				ShotWidth = srcWidth;
				ShotHeight = srcHeight;
				SaveScreenShot();
				DoMenuScreenShot = 0;
			}
			if (DrawMode == 0 || !gbRunGame || VideoWindowExists /*|| LoadingScreenCEL*/) { // update backbuffer
				if (!OnlySecondWin) ok = RenderSurface(D3dDevice, VideoSurface, D3dSurface, BackBuffer, startingX, startingY, src, pixelFormat, srcPitch, srcWidth, srcHeight, screenPalette, classicRect, metaMenuRect);
				if (second) ok = RenderSurface(SecDevice, SecVideoSurface, SecD3dSurface, SecBuffer, startingX, startingY, src, pixelFormat, srcPitch, srcWidth, srcHeight, screenPalette, classicRect, metaMenuRect);
				// (th4) True-colour portrait overlay. It belongs HERE, not beside the orbs:
				// the test above sends everything with !gbRunGame down this branch, so the meta
				// menu never reaches RenderQuadTexture at all. RenderSurface has just stretched
				// the menu from classicRect into metaMenuRect on the back buffer, so the overlay
				// gets both rects and maps GUI coordinates through them. No-op unless a dialog
				// is showing a portrait that has a strip.
				if (!OnlySecondWin) MenuPortrait_DrawOverlay(D3dDevice, classicRect, metaMenuRect, src, srcPitch);
			}
			else { // DrawMode == 1 // draw textured quad
				if (g_ARGBLighting) {
					// (th4) ultra two-texture path: A8R8G8B8 + L8, no palette texture
					if (!OnlySecondWin) ok = RenderQuadTextureUltra(D3dDevice, D3dTextureColor, D3dTextureL8, D3dTextureFlag, D3dTextureOverlay, D3dTextureTint, VideoSurface, src);
					if (second) ok = RenderQuadTextureUltra(SecDevice, SecTextureColor, SecTextureL8, SecTextureFlag, SecTextureOverlay, SecTextureTint, SecVideoSurface, src);
				} else {
					if (!OnlySecondWin) ok = RenderQuadTexture(D3dDevice, D3dTexture, D3dPalette, VideoSurface, src);
					if (second) ok = RenderQuadTexture(SecDevice, SecTexture, SecPalette, SecVideoSurface, src);
				}
			}
			if (!OnlySecondWin) D3dDevice->EndScene();
			if (second) SecDevice->EndScene();
			//(rt) if( Vsync && (BigWin || !IsFullScreen) ) DirectDrawPtr->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, 0 ); // vsync attempt for windowed mode, not very good
			HRESULT pres = OnlySecondWin ? D3D_OK : D3dDevice->Present(NULL, NULL, (D3dRef && !gbRunGame && !VideoWindowExists) ? DlgHwnd : NULL, NULL);
			if (second) SecDevice->Present(NULL, NULL, SecWnd, NULL);
			if (!ok || D3D_OK != pres /*&& !(IsStartTh1 && !Exe::IsMainWindowActive)*/) {
#ifdef TH4_D3D_DIAG_ENABLED
				// throttled: a lost-device episode fails every frame
				static DWORD s_lastDrawFail = 0;
				const DWORD now = GetTickCount();
				// Phase 2(d): track the episode so "draw recovered" can report its
				// exact length — the visible MP black window.
				if (s_diagWedgeFirstFail == 0) s_diagWedgeFirstFail = now;
				++s_diagWedgeFails;
				if (now - s_lastDrawFail > 1000) {
					s_lastDrawFail = now;
					// (th4) 2026-09-10 Phase 0: the activation state decides whether
					// this reset can possibly succeed (see D3dLostDeviceResetAllowed).
					D3DDIAG("draw failed: ok=%d present=0x%08X active=%d minimized=%d fgIsMain=%d",
						ok, (unsigned)pres, IsMainWindowActive, (int)IsMainWindowMinimized,
						(int)(GetForegroundWindow() == ghMainWnd));
					D3dDiagPhaseD("draw-fail"); // Phase 2(d): full state probe
				}
#endif
				// (th4) 2026-09-10: inactive-window gate (see
				// D3dLostDeviceResetAllowed()). The original call is kept verbatim
				// inside the gate; when the gate refuses, the skipped reset could not
				// have restored the device anyway (exclusive fullscreen, window not
				// active); the helper logs the refusal (throttled ~1 line/s).
				if (D3dLostDeviceResetAllowed()) {
					if (!D3dReset()) {} // TerminateWithError( "Direct 3D device restoring error" );
				}
			}
#ifdef TH4_D3D_DIAG_ENABLED
			// Phase 2(d): first successful frame after failed ones — closes the
			// episode and reports its exact length (the visible MP black window).
			else if (s_diagWedgeFirstFail != 0) {
				const DWORD now = GetTickCount();
				D3DDIAG("draw recovered: fails=%u over %u ms", s_diagWedgeFails, (unsigned)(now - s_diagWedgeFirstFail));
				D3dDiagPhaseD("recovered");
				s_diagWedgeFirstFail = 0;
				s_diagWedgeFails = 0;
			}
#endif
		}
		else {
#ifdef TH4_D3D_DIAG_ENABLED
			// throttled: a lost-device episode fails every frame
			static DWORD s_lastBsFail = 0;
			const DWORD now = GetTickCount();
			if (now - s_lastBsFail > 1000) {
				s_lastBsFail = now;
				D3DDIAG("BeginScene failed hr=0x%08X (device lost?)", (unsigned)d3dOk);
			}
#endif
			--AllDrawCount;
		}
		if (!IsMainWindowMinimized && not(d3dOk, D3D_OK, D3DERR_WASSTILLDRAWING)) {
			// (th4) 2026-09-10: inactive-window gate (see D3dLostDeviceResetAllowed()).
			if (D3dLostDeviceResetAllowed()) D3dReset();
		}
	}
	if (AltEnter()) {
		SwitchFullscreen();
	}
	//}
	return ScreenSurface;
}