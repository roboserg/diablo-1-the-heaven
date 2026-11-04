#include "stdafx.h"
#include <intrin.h> // (th4) diagnostics: _ReturnAddress() for the D3dReset call-site log
// (th4) A8L8 only shaders, used by g_A8L8Lighting=true and g_ARGBLighting=false path
#include "PaletteShader.h"
#include "LineShader.h"
#include "FogVenShader.h"
#include "FogShader.h"
#include "VignettingShader.h"
#include "BloomShader.h"
#include "FXAAShader.h"
// (th4) legacy pre-A8L8 L8 shaders — recovered from git commit 98f21596d (tag v0.415) as *ShaderLegacy.hlsl and compiled at build time for the g_A8L8Lighting=false path
#include "PaletteShaderLegacy.h"
#include "VignettingShaderLegacy.h"
#include "FogShaderLegacy.h"
#include "FogVenShaderLegacy.h"
#include "BloomShaderLegacy.h"
#include "FXAAShaderLegacy.h"
// (th4) g_ARGBLighting=true shaders, and Rim family is compile time optimization for g_ARGBAlphaRim=true path
#include "PaletteShaderRGB.h"
#include "VignettingShaderRGB.h"
#include "FogShaderRGB.h"
#include "FogVenShaderRGB.h"
#include "BloomShaderRGB.h"
#include "FXAAShaderRGB.h"
#include "PaletteShaderRimRGB.h"
#include "VignettingShaderRimRGB.h"
#include "FogShaderRimRGB.h"
#include "FogVenShaderRimRGB.h"
#include "BloomShaderRimRGB.h"
#include "FXAAShaderRimRGB.h"
#include "AnimOrbs.h"
#include "MenuPortrait.h"
#include "ItemOverlay.h"

LPDIRECT3D9/*EX*/ g_pD3D = 0; // Used to create the D3DDevice
bool DDrawEmulation = 0;
HMODULE HModule = 0;
extern LPDIRECT3DDEVICE9 SecDevice;
extern LPDIRECT3DSURFACE9 SecBuffer;
extern LPDIRECT3DSURFACE9 SecVideoSurface;
extern LPDIRECT3DSURFACE9 SecD3dSurface;
//HANDLE SharedHandle = 0;
extern LPDIRECT3DTEXTURE9 SecTexture;
extern LPDIRECT3DTEXTURE9 SecPalette;
uchar const* virtualshader = 0;

void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp);

//----- (00417D36) --------------------------------------------------------
int __fastcall SetupScreenResolution(HWND ahWnd)
{
	SetFocus(ahWnd);
	//ShowWindow(ahWnd, SW_SHOWNORMAL);
	int guid = 0;
	if( DDrawEmulation ){
		guid = DDCREATE_EMULATIONONLY;
	}
	HRESULT hr = CallDirectDrawCreate((GUID*)guid, &DirectDrawPtr, 0);
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	}

	#ifdef GUARD
	void ProtectAllGuards(); ProtectAllGuards();
	#endif

	#if tryhackddraw
	// (rt) Applying the ddraw palette mode compatibility key 
	// HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\DirectDraw\Compatibility
	((DDRAWI_DIRECTDRAW_INT*)DirectDrawPtr)->lpLcl->dwAppHackFlags |= 0x0800;
	#endif
	UseReservedSystemPalette = 1;

	hr = DirectDrawPtr->SetCooperativeLevel( ahWnd, DDSCL_NORMAL /*GetCooperativeLevel()*/ );
	
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	}
	#ifndef USE_32BITCOLOR
	#ifdef _DEBUG
	hr = DirectDrawPtr->SetDisplayMode(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 8);
	#else
	hr = DirectDrawPtr->SetDisplayMode(ScreenWidth, ScreenHeight, 8);
	#endif
	if( FAILED(hr) ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
		// (rt) ! the original restored the desktop mode here
	}
	#endif
	// (rt) if these functions are compared, then when the threads are synchronized 
	// (rt) the TH 1 primary surface becomes 32-bit every other time and a DDERR_INVALIDPIXELFORMAT error occurs
	CreateScreenSurface();
	SetScreenSurfacePalette();
	
	GdiSetBatchLimit( 1 );
	CreateWorkingSurface();
	WorkingSurface = SoftSrcSurfacePtr;
	WorkingSurfaceDrawBegin = WorkingSurface + WorkingWidth * Screen_TopBorder;
	WorkingSurfaceDrawEnd = WorkingSurface + WorkingHeight * WorkingWidth;
	return SDrawManualInitialize(ahWnd, DirectDrawPtr, ScreenSurface, ConvertPaletteSurfaceToTrueColor, 0, 0/*DDSrcSurface*/, DirectDrawPalettePtr, 0);
// label1: 1AB50  JMP 00 01 19 7D
// FF FE E6 74
// 		 mov	 ConvertPaletteSurfaceToTrueColor,eax	//[ 15 01 EE 2C h], eax	    // *(int*)0x1501EE2C = eax;
// 		 mov     StartX,0								//[ 15 01 EE 44 h], 0
}

void SwitchVirtualShader() {
	// (th4) 6 (FXAA) is retired -- Config.cpp turns it into Antialiasing 1 -- so anything above 5
	// falls back to the plain shader like the default cases below.
	if (FullscreenFX > 5 || FullscreenFX < 0) FullscreenFX = 0;
	if( g_ARGBLighting ){
		// (th4) ultra two-texture mode (A8R8G8B8 + L8): tested FIRST because
		// GetUserParameters() forces g_A8L8Lighting on when ultra is on.
		// (th4) Stage 2 rung 3 (Rev 2.16): ps_2_0 has no flow control and
		// fxc hoists texture reads above uniform branches, so the rim taps
		// cannot be skipped at runtime — the rim family is a separate set of
		// compiled bytecode produced from the *ShaderRimRGB.hlsl wrapper
		// sources (which #define ENRICHED_RIM and #include the base body),
		// selected here from the config. Takes effect at device creation,
		// same as FullscreenFX. NOTE: the wrappers cannot be replaced by
		// double-compiling the base sources — VS's project system rejects
		// duplicate FxCompile Includes on one file.
		const bool rimShaders = g_ARGBAlphaRim && g_ARGBAlphaContrast && EnrichedLightUltraAlpha != 0;
		switch (FullscreenFX) {
			case 0: virtualshader = rimShaders ? g_ps20_main_palette_rgb_rim : g_ps20_main_palette_rgb; break;
			case 1: virtualshader = rimShaders ? g_ps20_main_vignetting_rgb_rim : g_ps20_main_vignetting_rgb; break;
			case 2: virtualshader = rimShaders ? g_ps20_main_fog_rgb_rim : g_ps20_main_fog_rgb; break;
			case 3: virtualshader = rimShaders ? g_ps20_main_fogven_rgb_rim : g_ps20_main_fogven_rgb; break;
			// (th4) 4 was the old whole-screen perspective shader (PerspectiveShader*.hlsl), RETIRED:
			// it warped the UI too and ignored the mouse. The Perspective config option replaces it
			// (Draw.cpp Perspective_WarpWorld). FullscreenFX 4 now falls back to the plain shader.
			case 4: virtualshader = rimShaders ? g_ps20_main_palette_rgb_rim : g_ps20_main_palette_rgb; break;
			case 5: virtualshader = rimShaders ? g_ps30_main_bloom_rgb_rim : g_ps30_main_bloom_rgb; break;
			// (th4) 6 was FXAA (FXAAShader*.hlsl, still compiled, unused): now the separate Antialiasing
			// option, dx_utility.cpp AntialiasPresent, which works with every FullscreenFX.
			default: virtualshader = rimShaders ? g_ps20_main_palette_rgb_rim : g_ps20_main_palette_rgb;
		}
	}else if( g_A8L8Lighting ){
		switch (FullscreenFX) {
			case 0: virtualshader = g_ps20_main_palette; break;
			case 1: virtualshader = g_ps20_main_vignetting; break;
			case 2: virtualshader = g_ps20_main_fog; break;
			case 3: virtualshader = g_ps20_main_fogven; break;
			case 4: virtualshader = g_ps20_main_palette; break;	// retired perspective, see above
			case 5: virtualshader = g_ps30_main_bloom; break;
			default: virtualshader = g_ps20_main_palette;
		}
	}else{
		// (th4) legacy pre-A8L8 L8 shaders — compiled from the *ShaderLegacy.hlsl sources recovered from git commit 98f21596d (tag v0.415)
		switch (FullscreenFX) {
			case 0: virtualshader = g_ps20_main_palette_legacy; break;
			case 1: virtualshader = g_ps20_main_vignetting_legacy; break;
			case 2: virtualshader = g_ps20_main_fog_legacy; break;
			case 3: virtualshader = g_ps20_main_fogven_legacy; break;
			case 4: virtualshader = g_ps20_main_palette_legacy; break;	// retired perspective, see above
			case 5: virtualshader = g_ps30_main_bloom_legacy; break;
			default: virtualshader = g_ps20_main_palette_legacy;
		}
	}
}

//----- (00417E4C) --------------------------------------------------------
void CreateWorkingSurface()
{
	//DWORD v0; // eax@1  HRESULT hr
	//DWORD fn; // eax@4  HRESULT hr
	//DWORD v3; // eax@8  HRESULT hr
	DDSCAPS ddCaps;              // [sp+78h] [bp-4h]@1
	DDSURFACEDESC DDSurfaceDesc; // [sp+Ch] [bp-70h]@4
	HRESULT hr = ScreenSurface->GetCaps(&ddCaps);
	if( FAILED(hr) ) ShowDirectDrawError(hr, __LINE__, __FILE__);
	DDSurfaceDesc.dwSize = sizeof( DDSurfaceDesc );
	hr = ScreenSurface->Lock(0, &DDSurfaceDesc, DDLOCK_WRITEONLY | DDLOCK_WAIT, 0);
	if( hr == DD_OK ){
		ScreenSurface->Unlock(0);
		SoftSrcSurfacePtr = (uchar*) malloc( (MaxWorkingHeight + 256) * MaxWorkingWidth); // (rt) town drawing hack
		if (LightSurface) { free(LightSurface); LightSurface = nullptr; }
		if (LightSurfaceBase) { free(LightSurfaceBase); LightSurfaceBase = nullptr; }
		if (WallOcclusionSurface) { free(WallOcclusionSurface); WallOcclusionSurface = nullptr; }
		if (IndexFlagSurface) { free(IndexFlagSurface); IndexFlagSurface = nullptr; }
		if (AlphaSurface) { free(AlphaSurface); AlphaSurface = nullptr; }
		if (SpriteLayerSurface) { free(SpriteLayerSurface); SpriteLayerSurface = nullptr; }
		if (LightTintSurface) { free(LightTintSurface); LightTintSurface = nullptr; }
		if (g_A8L8Lighting) {
			LightSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			LightSurfaceBase = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			WallOcclusionSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
		}
		if (g_ARGBLighting) {
			IndexFlagSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
		}
		// (th4) Stage 2 overlay surfaces — only when the alpha feature is on.
		if (g_ARGBLighting && EnrichedLightUltraAlpha != 0) {
			AlphaSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			SpriteLayerSurface = (uint*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, sizeof(uint));
		}
		// (th4) Stage 3 tint surface — whenever ultra is on (the tint texture
		// is always uploaded and the V2 fill rewrites every pixel per frame).
		if (g_ARGBLighting) {
			LightTintSurface = (uint*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, sizeof(uint));
		}
		return;
	}
	if( hr != DDERR_CANTLOCKSURFACE ) //0x887601B3 ){
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	#if USE_DDRAW_WORKING_SURFACE
	memset( &DDSurfaceDesc, 0, sizeof( DDSurfaceDesc ));
	DDSurfaceDesc.dwSize = sizeof( DDSurfaceDesc ); // 108
	DDSurfaceDesc.lPitch = WorkingWidth; // 768
	DDSurfaceDesc.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PITCH|DDSD_PIXELFORMAT;
	DDSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	DDSurfaceDesc.dwHeight = WorkingHeight; // 656
	DDSurfaceDesc.dwWidth = WorkingWidth; // 768
	DDSurfaceDesc.ddpfPixelFormat.dwSize = sizeof( DDSurfaceDesc.ddpfPixelFormat );
	hr = ScreenSurface->GetPixelFormat( &DDSurfaceDesc.ddpfPixelFormat);
	if( FAILED(hr) )
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	
	hr = DirectDrawPtr->CreateSurface( &DDSurfaceDesc, &DDSrcSurface, 0);
	if( hr != DD_OK )
		ErrorIn_CPP_File(ID_DIALOG1, hr, __FILE__, __LINE__);
	#endif
}

// ---- th2 ----------------------------------------
void SetScreenDesc(DDSURFACEDESC& screenDesc)
{
    memset(&screenDesc, 0, sizeof(screenDesc));
    screenDesc.dwSize = sizeof(screenDesc);
	screenDesc.dwFlags = DDSD_CAPS;
	//POINT p{ 0, 0 };
	//ClientToScreen( ghMainWnd, &p );
	//int desktopHeight = GetSystemMetrics( SM_CYSCREEN );
	//int desktopWidth = GetSystemMetrics( SM_CXSCREEN );
	screenDesc.lPitch = MaxScreenWidth;
	screenDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | DDSD_PIXELFORMAT;
	screenDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	// (rt) the main menu has various areas: text, frames, and the TH version number near the very bottom
	// (rt) one of the elements (together with its background rectangle) has its bottom edge two pixels below the screen edge (possibly the version number)
	// (rt) while TH draws it into fullscreen video memory, everything is fine: there is spare unused memory there
	// (rt) once TH was moved to a window and the draw buffer to system memory,
	// (rt) these two extra vertical pixels caused months of difficult debugging
	// (rt) ideally storm's drawing function should check for going outside the screen, but it doesn't; that is a bug
	// (rt) two pixels for the bottom menu element going past the screen edge + a spare buffer for other possible overruns
	screenDesc.dwHeight = MaxScreenHeight + 2 + 128;
	screenDesc.dwWidth = MaxScreenWidth;
	// (rt) added pixel format in TH 1 via a call to this ScreenSurface
	screenDesc.ddpfPixelFormat.dwSize = sizeof( screenDesc.ddpfPixelFormat );
	screenDesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	screenDesc.ddpfPixelFormat.dwRGBBitCount = 8;
}

// ---- th2 ----------------------------------------
void ChangeScreenRes()
{
	// calc screen ratio offset for automap and item higlighting
	HeightShift = (ScreenWidth / 64 - (ScreenHeight - 128) / 32) / 2 - (ScreenWidth > 800 ? 1 : 0) + ScreenCenterUserShiftVertical;
	double h = double( ScreenHeight - GUI_MainPanelHeight ) / double( GUI_Height - GUI_MainPanelHeight );
	double w = double( ScreenWidth ) / double( GUI_Width );
	Xofs = (ScreenWidth - ScreenWidth % 64) / 2 + Screen_LeftBorder;
	Yofs = Screen_TopBorder - int( w * w + w ) + int( double( ScreenHeight - GUI_MainPanelHeight ) / 2.0 * (w / h) ) + 8; // 8 is an automap vertical adjust ?
	Yofs -= HeightShift * 32;
	WorkingWidth = ScreenWidth + Screen_LeftBorder + Screen_RightBorder; //768
	WorkingHeight = ScreenHeight + Screen_TopBorder + Screen_BottomBorder; // 656

	int prevMaxWidth = MaxScreenWidth, prevMaxHeight = MaxScreenHeight;
	MaxScreenWidth = 4096; if( ScreenWidth > MaxScreenWidth ) MaxScreenWidth = ScreenWidth;
	MaxScreenHeight = 2400; if( ScreenHeight > MaxScreenHeight ) MaxScreenHeight = ScreenHeight;
	MaxWorkingWidth = MaxScreenWidth + Screen_LeftBorder + Screen_RightBorder;
	MaxWorkingHeight = MaxScreenHeight + Screen_TopBorder + Screen_BottomBorder;

	int ww = 0; for( auto& y : YOffsetHashTable ){ y = ww; ww += WorkingWidth; }
	SetupDisplayObjects();
	
	#ifdef HARD_RESET
	if( ScreenSurface ){
		ReturnToDDSurface();
		ShowWindow( global_hWnd, SW_SHOWNORMAL );
	}
	#else // Soft reset
	if( g_ARGBLighting ){
		// (th4) ultra mode recreates the five-texture set instead of D3dTexture
		if( D3dTextureColor || D3dTextureL8 || D3dTextureFlag || D3dTextureOverlay || D3dTextureTint ){
			if (D3dTextureColor) { D3dTextureColor->Release(); D3dTextureColor = 0; }
			if (D3dTextureL8) { D3dTextureL8->Release(); D3dTextureL8 = 0; }
			if (D3dTextureFlag) { D3dTextureFlag->Release(); D3dTextureFlag = 0; }
			if (D3dTextureOverlay) { D3dTextureOverlay->Release(); D3dTextureOverlay = 0; }
			if (D3dTextureTint) { D3dTextureTint->Release(); D3dTextureTint = 0; }
			// (th4) Stage 4 item 5: s0 (color), s1 (L8 light), s2 (overlay) and
			// s3 (tint) are D3DUSAGE_DYNAMIC | D3DPOOL_DEFAULT — every
			// per-frame upload is a full-surface write locked with
			// D3DLOCK_DISCARD, which removes the managed-pool GPU wait and the
			// copy-on-unlock. s4 (flag) stays managed. All locks on the dynamic
			// textures MUST use D3DLOCK_DISCARD (D3D9 rejects plain locks on
			// dynamic textures). s0 needs no creation fill — its upload precedes
			// every draw.
			if( D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureColor, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &D3dTextureL8, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, D3DFMT_L8, D3DPOOL_MANAGED, &D3dTextureFlag, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureOverlay, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureTint, 0 ) == D3D_OK ){
				// (th4) zero-fill the recreated overlay texture: the RGB shaders
				// sample s2 even when the alpha feature is off (weight 0), so
				// keep it deterministic instead of leaving driver garbage.
				D3DLOCKED_RECT lr;
				if (D3D_OK == D3dTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
					D3dTextureOverlay->UnlockRect(0);
				}
				// (th4) Stage 3: white-fill the recreated tint texture (white =
				// no tint) so it is never sampled as black before an upload.
				if (D3D_OK == D3dTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
					D3dTextureTint->UnlockRect(0);
				}
				SwitchFullscreen();
			}
		}
	}else if( D3dTexture ){
		D3dTexture->Release();
		if( D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, g_A8L8Lighting ? D3DFMT_A8L8 : D3DFMT_L8, D3DPOOL_MANAGED, &D3dTexture, 0 ) == D3D_OK ){
			SwitchFullscreen();
		}
	}
	if( ScreenSurface && (prevMaxWidth < MaxScreenWidth || prevMaxHeight < MaxScreenHeight) ){
		ScreenSurface->Release();
		DDSURFACEDESC screenDesc;
		SetScreenDesc( screenDesc );
		HRESULT hr = DirectDrawPtr->CreateSurface( &screenDesc, &ScreenSurface, nullptr );
		if( FAILED( hr ) ){
			//__debugbreak();
		}
		ScreenSurface->Lock( 0, &screenDesc, DDLOCK_READONLY | DDLOCK_NOSYSLOCK, 0 );
		extern int ScreenSurfacePitch; ScreenSurfacePitch = screenDesc.lPitch;
		ScreenSurface->Unlock( 0 );
		if( SoftSrcSurfacePtr ){ free( SoftSrcSurfacePtr ); }
		SoftSrcSurfacePtr = (uchar*) malloc( (MaxWorkingHeight + 256) * MaxWorkingWidth ); // (rt) town drawing hack
		if (LightSurface) { free(LightSurface); LightSurface = nullptr; }
		if (LightSurfaceBase) { free(LightSurfaceBase); LightSurfaceBase = nullptr; }
		if (WallOcclusionSurface) { free(WallOcclusionSurface); WallOcclusionSurface = nullptr; }
		if (IndexFlagSurface) { free(IndexFlagSurface); IndexFlagSurface = nullptr; }
		if (AlphaSurface) { free(AlphaSurface); AlphaSurface = nullptr; }
		if (SpriteLayerSurface) { free(SpriteLayerSurface); SpriteLayerSurface = nullptr; }
		if (LightTintSurface) { free(LightTintSurface); LightTintSurface = nullptr; }
		if (g_A8L8Lighting) {
			LightSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			LightSurfaceBase = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			WallOcclusionSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
		}
		if (g_ARGBLighting) {
			IndexFlagSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
		}
		// (th4) Stage 2 overlay surfaces — only when the alpha feature is on.
		if (g_ARGBLighting && EnrichedLightUltraAlpha != 0) {
			AlphaSurface = (uchar*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, 1);
			SpriteLayerSurface = (uint*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, sizeof(uint));
		}
		// (th4) Stage 3 tint surface — whenever ultra is on.
		if (g_ARGBLighting) {
			LightTintSurface = (uint*) calloc((MaxWorkingHeight + 256) * MaxWorkingWidth, sizeof(uint));
		}
		SDrawManualInitialize( ghMainWnd, DirectDrawPtr, ScreenSurface, ConvertPaletteSurfaceToTrueColor, 0, 0/*DDSrcSurface*/, DirectDrawPalettePtr, 0 );
		WorkingSurface = SoftSrcSurfacePtr;
	}
	#endif
	WorkingSurfaceDrawBegin = WorkingSurface + WorkingWidth * Screen_TopBorder;
	WorkingSurfaceDrawEnd = WorkingSurface + WorkingHeight * WorkingWidth;
}

#define PS_PALETTE
//----- (00417F59) --------------------------------------------------------
HRESULT CreateScreenSurface()
{
	SwitchVirtualShader();
	DDSURFACEDESC screenDesc;
	DDSURFACEDESC screenDesc32;
	HRESULT hr;
	#ifdef USE_32BITCOLOR
	SetScreenDesc(screenDesc);
    if( DDraw ){ // render == ddraw
		memset( &screenDesc32, 0, sizeof( screenDesc32 ) );
		screenDesc32.dwSize = sizeof(screenDesc32);
		screenDesc32.dwFlags = DDSD_CAPS;
		screenDesc32.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE /*| DDSCAPS_VISIBLE*/;
		hr = DirectDrawPtr->CreateSurface(&screenDesc32, &ScreenSurface32, nullptr);
		if( FAILED(hr) ) ErrorIn_CPP_File(ID_DIALOG1, hr, "dx_utility.cpp::DDCreateSurface", __LINE__);
		screenDesc32.dwFlags =  DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		screenDesc32.dwWidth = ScreenWidth;
		screenDesc32.dwHeight = ScreenHeight;
		screenDesc32.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		hr = DirectDrawPtr->CreateSurface(&screenDesc32, &ScreenSurface32Back, nullptr);
		if( FAILED(hr) ) ErrorIn_CPP_File(ID_DIALOG1, hr, "dx_utility.cpp::DDCreateSurface", __LINE__);

	}else{ /*render == d3d*/ 
		if( D3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D back buffer" );
		if( IsSecondWin	&& SecDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &SecBuffer ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second window back buffer" );
		if( D3dDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &VideoSurface, 0/*&SharedHandle*/ ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D render target" );
		if( IsSecondWin && SecDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &SecVideoSurface, 0/*&SharedHandle*/ ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second render target" );
		if( D3dDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &D3dSurface, 0 ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D offscreen surface" );
		if( IsSecondWin && SecDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &SecD3dSurface, 0 ) != D3D_OK )
			TerminateWithError( "Unable to create Direct 3D second offscreen surface" );
		// (rt) texture sampling with the palette shader doesn't work, so we texture into an intermediate buffer (VideoSurface)
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		#if defined(MORE_COLOR)
		DrawMode = D3dRef ? 0 : 1 ; // (rt) force texture mode to increase the number of colours
		constexpr D3DFORMAT fmt = D3DFMT_L16;
		constexpr int colors = 512;
		#else
		const D3DFORMAT fmt = g_A8L8Lighting ? D3DFMT_A8L8 : D3DFMT_L8;
		constexpr int colors = 256;
		#endif
		/*
		switch (FullscreenFX) {
			case 0: virtualshader = g_ps20_main_palette; break;
			case 1: virtualshader = g_ps20_main_vignetting; break;
			case 2: virtualshader = g_ps20_main_fog; break;
			case 3: virtualshader = g_ps20_main_fogven; break;
			case 4: virtualshader = g_ps20_main_perspective; break;
			case 5: virtualshader = g_ps20_main_bloom; break;
			default: virtualshader = g_ps20_main_palette;
		}*/
		DrawMode = 1; // return texture mode forcing
		if( DrawMode == 1 ){ // (rt) texture mode
			if( g_ARGBLighting ){
				// (th4) ultra two-texture mode: A8R8G8B8 color + L8 light + L8 vignetting flag + A8R8G8B8 overlay + A8R8G8B8 tint.
				// No 256x1 palette texture — the RGB shader family has no tex1D.
				// (th4) Stage 4 item 5: color/L8/overlay/tint are dynamic+default
				// (see the ChangeScreenRes note); flag stays managed.
				if( D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureColor, 0 ) == D3D_OK
					&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &D3dTextureL8, 0 ) == D3D_OK
					&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, D3DFMT_L8, D3DPOOL_MANAGED, &D3dTextureFlag, 0 ) == D3D_OK
					&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureOverlay, 0 ) == D3D_OK
					&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureTint, 0 ) == D3D_OK
					&& D3dDevice->CreatePixelShader((DWORD*)virtualshader, &PaletteShader ) == D3D_OK
					&& D3dDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK){
					D3dDevice->SetTexture( 0, D3dTextureColor );
					D3dDevice->SetTexture( 1, D3dTextureL8 );
					D3dDevice->SetTexture( 2, D3dTextureOverlay );
					D3dDevice->SetTexture( 3, D3dTextureTint );
					D3dDevice->SetTexture( 4, D3dTextureFlag );
					D3dDevice->SetRenderState(D3DRS_LIGHTING, false);
					D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
					D3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					D3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					D3dDevice->SetPixelShader(PaletteShader);
					// zero-fill the flag texture once; it is only rewritten per frame when
					// FullscreenFX == 1, and only sampled by VignettingShaderRGB (FX 1)
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == D3dTextureFlag->LockRect(0, &lr, 0, 0)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth);
							D3dTextureFlag->UnlockRect(0);
						}
					}
					// (th4) Stage 2: the s2 overlay texture must exist whenever ultra is
					// on (the RGB shaders always sample it); zero-fill once at creation.
					// Its per-frame upload runs only when EnrichedLightUltraAlpha != 0.
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == D3dTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
							D3dTextureOverlay->UnlockRect(0);
						}
					}
					// (th4) Stage 3: the s3 tint texture is sampled every frame —
					// white-fill once at creation (white = no tint). Its per-frame
					// upload is a row memcpy of LightTintSurface.
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == D3dTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
							D3dTextureTint->UnlockRect(0);
						}
					}
				}else{
					TerminateWithError( "Unable to create Direct 3D texture" );
				}
			}else if( D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, fmt, D3DPOOL_MANAGED, &D3dTexture, 0 ) == D3D_OK
				&& D3dDevice->CreatePixelShader((DWORD*)virtualshader, &PaletteShader ) == D3D_OK
				&& D3dDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK){
				D3dDevice->SetTexture( 0, D3dTexture );
				D3dDevice->SetRenderState(D3DRS_LIGHTING, false);
				D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				D3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				D3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				// (rt) pixel shader for palette texture mode
				D3dDevice->CreateTexture( colors, 1, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &D3dPalette, 0 );
				D3dDevice->SetTexture( 1, D3dPalette );
				D3dDevice->SetPixelShader(PaletteShader);
				// (rt) to check the created texture
				//D3DSURFACE_DESC d{};
				//D3dPalette->GetLevelDesc(0, &d);
				//__debugbreak();
			}else{
				//#if defined(MORE_COLOR)
				TerminateWithError( "Unable to create Direct 3D texture" );
				//#else
				//DrawMode = 0;
				//#endif
			}
		}
		if( IsSecondWin && DrawMode == 1 ){ // (rt) texture mode
			if( g_ARGBLighting ){
				// (th4) ultra two-texture mode, second window. color/L8/overlay/
				// tint are dynamic+default (Stage 4 item 5); flag stays managed.
				if( SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureColor, 0 ) == D3D_OK
					&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &SecTextureL8, 0 ) == D3D_OK
					&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, D3DFMT_L8, D3DPOOL_MANAGED, &SecTextureFlag, 0 ) == D3D_OK
					&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureOverlay, 0 ) == D3D_OK
					&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureTint, 0 ) == D3D_OK
					&& SecDevice->CreatePixelShader((DWORD*)virtualshader, &PaletteShader ) == D3D_OK
					&& SecDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK ){
					SecDevice->SetTexture( 0, SecTextureColor );
					SecDevice->SetTexture( 1, SecTextureL8 );
					SecDevice->SetTexture( 2, SecTextureOverlay );
					SecDevice->SetTexture( 3, SecTextureTint );
					SecDevice->SetTexture( 4, SecTextureFlag );
					SecDevice->SetRenderState( D3DRS_LIGHTING, false );
					SecDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
					SecDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					SecDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					SecDevice->SetPixelShader(PaletteShader);
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == SecTextureFlag->LockRect(0, &lr, 0, 0)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth);
							SecTextureFlag->UnlockRect(0);
						}
					}
					// (th4) Stage 2: zero-fill the second window's overlay texture once
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == SecTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
							SecTextureOverlay->UnlockRect(0);
						}
					}
					// (th4) Stage 3: white-fill the second window's tint texture once
					{
						D3DLOCKED_RECT lr;
						if (D3D_OK == SecTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
							for (int row = 0; row < ScreenHeight; ++row)
								memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
							SecTextureTint->UnlockRect(0);
						}
					}
				}else{
					TerminateWithError( "Unable to create Direct 3D second texture" );
				}
			}else if( SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, 0, fmt, D3DPOOL_MANAGED, &SecTexture, 0 ) == D3D_OK
				&& SecDevice->CreatePixelShader((DWORD*)virtualshader, &PaletteShader ) == D3D_OK
				&& SecDevice->CreatePixelShader((DWORD*)g_ps20_main_line, &DirectShader) == D3D_OK ){
				SecDevice->SetTexture( 0, SecTexture );
				SecDevice->SetRenderState( D3DRS_LIGHTING, false );
				SecDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
				SecDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				SecDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				// (rt) pixel shader for palette texture mode
				SecDevice->CreateTexture( colors, 1, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &SecPalette, 0 );
				SecDevice->SetTexture( 1, SecPalette );
				SecDevice->SetPixelShader(PaletteShader);
				// (rt) to check the created texture
				//D3DSURFACE_DESC d{};
				//D3dPalette->GetLevelDesc(0, &d);
				//__debugbreak();
			}else{
				TerminateWithError( "Unable to create Direct 3D second texture" );
			}
		}
	}
	#else
	memset( &screenDesc, 0, sizeof( screenDesc ) );
	screenDesc.dwSize = sizeof( screenDesc );
	screenDesc.dwFlags = DDSD_CAPS;
	screenDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	#endif
    hr = DirectDrawPtr->CreateSurface(&screenDesc, &ScreenSurface, nullptr);
	if( FAILED(hr) ){
		//__debugbreak();
		ErrorIn_CPP_File(ID_DIALOG1, hr, "dx_utility.cpp::DDCreateSurface", __LINE__);
	}
	ScreenSurface->Lock(0, &screenDesc, DDLOCK_READONLY | DDLOCK_NOSYSLOCK, 0);
	extern int ScreenSurfacePitch; ScreenSurfacePitch = screenDesc.lPitch;
	ScreenSurface->Unlock(0);
	return hr;
}

// th2 ---------------------------------------------------
bool D3dReset()
{
	// (th4) 2026-09-10 reentrancy guard (root fix for the MP "New Game" d3d9
	// crash): while D3dDevice->Reset() executes, user32/Storm dispatch window
	// messages synchronously; their handlers can re-enter the drawing (SDraw
	// callback -> ConvertPaletteSurfaceToTrueColor) and call back into d3d9 on
	// a half-reset device (crash dump: reentrant BeginScene() -> AV in
	// d3d9+0x663D1). While a reset is in progress every D3D touch point is a
	// no-op (g_D3dInReset) and a nested reset call is ignored.
	if (g_D3dInReset) {
		D3DDIAG("D3dReset reentrancy ignored (reset already in progress)");
		return true;
	}
	g_D3dInReset = true;
	if( D3dDevice ){
#ifdef TH4_D3D_DIAG_ENABLED
		// (th4) 2026-09-10 diagnostics (d3d9 crash investigation): every reset
		// is logged with its caller (module+offset) so a crash can be tied to
		// a device-lost/recreate episode. Gated by "d3d_diag_on.flag"; throttled
		// because a lost-device loop retries Reset every frame. Compiled out
		// entirely unless TH4_D3D_DIAG_ENABLED is defined (funcs.h).
		static DWORD s_lastResetEnter = 0;
		const DWORD resetTick = GetTickCount();
		if (resetTick - s_lastResetEnter > 1000) {
			s_lastResetEnter = resetTick;
			char caller[128];
			D3DDIAGADDR(caller, sizeof(caller), _ReturnAddress());
			D3DDIAG("D3dReset enter (caller %s)", caller);
		}
#endif
		AnimOrbs_OnDeviceLost(); // free DEFAULT-pool orb textures before Reset
		MenuPortrait_OnDeviceLost(); // (th4) same for the portrait overlay texture
		ItemOverlay_OnDeviceLost();  // (th4) and the item icon textures
		// (th4) Stage 4 item 5: the dynamic s0/s1/s2/s3 textures live in the
		// DEFAULT pool and do NOT survive Reset — release them before it
		// (s4 flag stays managed and restores automatically).
		if (g_ARGBLighting) {
			if (D3dTextureColor) { D3dTextureColor->Release(); D3dTextureColor = 0; }
			if (D3dTextureL8) { D3dTextureL8->Release(); D3dTextureL8 = 0; }
			if (D3dTextureOverlay) { D3dTextureOverlay->Release(); D3dTextureOverlay = 0; }
			if (D3dTextureTint) { D3dTextureTint->Release(); D3dTextureTint = 0; }
		}
		//if( D3dSurface ){ D3dSurface->Release(); D3dSurface = 0; }
		if( VideoSurface ){ VideoSurface->Release(); VideoSurface = 0; }
		SharpScale_Release( 0 );	// (th4) sharp-scaling intermediate, same default pool
		if( BackBuffer ){ BackBuffer->Release(); BackBuffer = 0; }
		D3DPRESENT_PARAMETERS d3dpp;
		SetD3dParam( d3dpp );
#ifdef TH4_D3D_DIAG_ENABLED
		// (th4) 2026-09-10 Phase 2(d): log the requested present parameters
		// whenever they change (SetD3dParam zero-fills first, so memcmp over the
		// whole struct is stable). Diagnosis only — e.g. captures the
		// PresentationInterval flip (menu vs gameplay) between the click's two
		// forced restyles.
		{
			static D3DPRESENT_PARAMETERS s_diagLastResetPp;
			static bool s_diagHasLastResetPp = false;
			if (!s_diagHasLastResetPp || memcmp(&s_diagLastResetPp, &d3dpp, sizeof(d3dpp)) != 0) {
				s_diagHasLastResetPp = true;
				s_diagLastResetPp = d3dpp;
				D3DDIAG("reset params changed: bb=%ux%u fmt=%u cnt=%u int=%u win=%d hwnd=%p flags=0x%X game=%d vsync=%d bigwin=%d",
					d3dpp.BackBufferWidth, d3dpp.BackBufferHeight, (unsigned)d3dpp.BackBufferFormat,
					d3dpp.BackBufferCount, d3dpp.PresentationInterval, (int)d3dpp.Windowed,
					(void*)d3dpp.hDeviceWindow, (unsigned)d3dpp.Flags, (int)gbRunGame, (int)Vsync, (int)BigWin);
			}
		}
#endif
		if( D3D_OK != D3dDevice->Reset( &d3dpp ) ){
			// (th4) Stage 4 item 5: the dynamic textures were released above.
			// Best-effort recreate so the globals are never left null for a
			// WORKING device; when the device is lost the recreate fails too,
			// and RenderQuadTextureUltra's null-guard / lock check returns
			// before any dynamic handle is touched.
			if (g_ARGBLighting) {
				D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureColor, 0 );
				D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &D3dTextureL8, 0 );
				D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureOverlay, 0 );
				D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureTint, 0 );
				// (th4) Stage 4 item 5 hardening: if the recreates
				// unexpectedly SUCCEEDED (device still usable despite the
				// Reset failure), refill the deterministic content (s2 zero,
				// s3 white) and reset the tint statics exactly like the
				// success path, so no frame can ever sample garbage. On a
				// truly lost device the handles stay null and every lock is
				// skipped.
				if (D3dTextureOverlay) {
					D3DLOCKED_RECT lr;
					if (D3D_OK == D3dTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
						for (int row = 0; row < ScreenHeight; ++row)
							memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
						D3dTextureOverlay->UnlockRect(0);
					}
				}
				if (D3dTextureTint) {
					D3DLOCKED_RECT lr;
					if (D3D_OK == D3dTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
						for (int row = 0; row < ScreenHeight; ++row)
							memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
						D3dTextureTint->UnlockRect(0);
					}
				}
				SetUltraTintWhiteState( false );
			}
#ifdef TH4_D3D_DIAG_ENABLED
			{
				// throttled: a lost-device loop retries Reset every frame
				static DWORD s_lastResetFail = 0;
				const DWORD now = GetTickCount();
				if (now - s_lastResetFail > 1000) {
					s_lastResetFail = now;
					D3DDIAG("D3dReset FAILED (device still lost); dyn handles color=%p l8=%p overlay=%p tint=%p",
						(void*)D3dTextureColor, (void*)D3dTextureL8, (void*)D3dTextureOverlay, (void*)D3dTextureTint);
				}
			}
#endif
			g_D3dInReset = false;
			return false;
		}
		D3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer );
		D3dDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &VideoSurface, 0 );
		// (th4) Stage 4 item 5: recreate the dynamic textures after Reset and
		// re-fill their deterministic content (s2 zero, s3 white) so no frame
		// can ever sample discarded memory; then force the per-window tint
		// white-tracking statics back to white (the content IS white again).
		// s0 (color) and s1 (L8) need no fill — every draw is preceded by
		// their full-frame upload in RenderQuadTextureUltra.
		if (g_ARGBLighting) {
			if (!(D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureColor, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &D3dTextureL8, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureOverlay, 0 ) == D3D_OK
				&& D3dDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &D3dTextureTint, 0 ) == D3D_OK )) {
				TerminateWithError( "Unable to recreate Direct 3D dynamic textures" );
			}
			{
				D3DLOCKED_RECT lr;
				if (D3D_OK == D3dTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
					D3dTextureOverlay->UnlockRect(0);
				}
			}
			{
				D3DLOCKED_RECT lr;
				if (D3D_OK == D3dTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
					D3dTextureTint->UnlockRect(0);
				}
			}
			SetUltraTintWhiteState( false );
		}
		//D3dDevice->CreateOffscreenPlainSurface( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &D3dSurface, 0 );
		//(rt) texture sampling with the palette shader doesn't work, so we texture into an intermediate buffer (VideoSurface)
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		//D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		if( DrawMode == 1 ){ // (rt) texture mode
			D3dDevice->SetRenderState( D3DRS_LIGHTING, false );
			D3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			D3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			D3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			if (g_ARGBLighting) {
				D3dDevice->SetTexture( 0, D3dTextureColor );
				D3dDevice->SetTexture( 1, D3dTextureL8 );
				D3dDevice->SetTexture( 2, D3dTextureOverlay );
				D3dDevice->SetTexture( 3, D3dTextureTint );
				D3dDevice->SetTexture( 4, D3dTextureFlag );
			} else {
				D3dDevice->SetTexture( 0, D3dTexture );
				// (rt) pixel shader for palette texture mode
				D3dDevice->SetTexture( 1, D3dPalette );
			}
			D3dDevice->SetPixelShader(PaletteShader);
		}
		D3DDIAG("D3dReset OK");
	}
	g_D3dInReset = false;
	return true;
}

// th2 ---------------------------------------------------
// (th4) 2026-09-10 note: NO CALLERS in-tree (repo-wide search, 2026-09-10) —
// the second window's device is never reset through this function today; it is
// kept for parity with D3dReset(). The reentrancy guard below is therefore
// inert until something calls it.
bool SecondReset()
{
	// (th4) 2026-09-10 reentrancy guard: see D3dReset() — same rule for the
	// second window's device (shared flag: both resets block all drawing).
	if (g_D3dInReset) {
		D3DDIAG("SecondReset reentrancy ignored (reset already in progress)");
		return true;
	}
	g_D3dInReset = true;
	if( SecDevice ){
		// (th4) Stage 4 item 5: same dynamic-texture release-before-Reset as
		// D3dReset (s4 flag stays managed).
		if (g_ARGBLighting) {
			if (SecTextureColor) { SecTextureColor->Release(); SecTextureColor = 0; }
			if (SecTextureL8) { SecTextureL8->Release(); SecTextureL8 = 0; }
			if (SecTextureOverlay) { SecTextureOverlay->Release(); SecTextureOverlay = 0; }
			if (SecTextureTint) { SecTextureTint->Release(); SecTextureTint = 0; }
		}
		// (th4) 2026-09-07: vanilla copy-paste bug fixed — the original line
		// released the MAIN BackBuffer (nulling the main window's present
		// target after any second-window reset). Kept commented out below
		// for reference; the corrected release follows.
		//if( SecBuffer ){ BackBuffer->Release(); BackBuffer = 0; }
		if( SecBuffer ){ SecBuffer->Release(); SecBuffer = 0; }
		if( SecVideoSurface ){ SecVideoSurface->Release(); SecVideoSurface = 0; }
		SharpScale_Release( 1 );	// (th4) sharp-scaling intermediate, same default pool
		D3DPRESENT_PARAMETERS d3dpp;
		SetD3dParam( d3dpp );
		if( D3D_OK != SecDevice->Reset( &d3dpp ) ){
			// (th4) Stage 4 item 5: same best-effort recreate as D3dReset — the
			// upload's null-guard / lock check returns before any handle is
			// touched while the device stays lost.
			if (g_ARGBLighting) {
				SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureColor, 0 );
				SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &SecTextureL8, 0 );
				SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureOverlay, 0 );
				SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureTint, 0 );
				// (th4) Stage 4 item 5 hardening: same refill + static reset as
				// D3dReset's failure path (see there) — on a truly lost device
				// the handles stay null and every lock is skipped.
				if (SecTextureOverlay) {
					D3DLOCKED_RECT lr;
					if (D3D_OK == SecTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
						for (int row = 0; row < ScreenHeight; ++row)
							memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
						SecTextureOverlay->UnlockRect(0);
					}
				}
				if (SecTextureTint) {
					D3DLOCKED_RECT lr;
					if (D3D_OK == SecTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
						for (int row = 0; row < ScreenHeight; ++row)
							memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
						SecTextureTint->UnlockRect(0);
					}
				}
				SetUltraTintWhiteState( true );
			}
			g_D3dInReset = false;
			return false;
		}
		SecDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &SecBuffer );
		SecDevice->CreateRenderTarget( ScreenWidth, ScreenHeight, D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &SecVideoSurface, 0 );
		// (th4) Stage 4 item 5: recreate + refill the second window's dynamic
		// textures (s2 zero, s3 white) and reset its tint white-tracking.
		// s0 (color) and s1 (L8) need no fill — their full-frame upload
		// precedes every draw.
		if (g_ARGBLighting) {
			if (!(SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureColor, 0 ) == D3D_OK
				&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &SecTextureL8, 0 ) == D3D_OK
				&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureOverlay, 0 ) == D3D_OK
				&& SecDevice->CreateTexture( ScreenWidth, ScreenHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SecTextureTint, 0 ) == D3D_OK )) {
				TerminateWithError( "Unable to recreate Direct 3D second dynamic textures" );
			}
			{
				D3DLOCKED_RECT lr;
				if (D3D_OK == SecTextureOverlay->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0, ScreenWidth * 4);
					SecTextureOverlay->UnlockRect(0);
				}
			}
			{
				D3DLOCKED_RECT lr;
				if (D3D_OK == SecTextureTint->LockRect(0, &lr, 0, D3DLOCK_DISCARD)) {
					for (int row = 0; row < ScreenHeight; ++row)
						memset((uchar*)lr.pBits + row * lr.Pitch, 0xFF, ScreenWidth * 4);
					SecTextureTint->UnlockRect(0);
				}
			}
			SetUltraTintWhiteState( true );
		}
		if( DrawMode == 1 ){ // (rt) texture mode
			SecDevice->SetRenderState( D3DRS_LIGHTING, false );
			SecDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
			SecDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SecDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			if (g_ARGBLighting) {
				SecDevice->SetTexture( 0, SecTextureColor );
				SecDevice->SetTexture( 1, SecTextureL8 );
				SecDevice->SetTexture( 2, SecTextureOverlay );
				SecDevice->SetTexture( 3, SecTextureTint );
				SecDevice->SetTexture( 4, SecTextureFlag );
			} else {
				SecDevice->SetTexture( 0, SecTexture );
				// (rt) pixel shader for palette texture mode
				SecDevice->SetTexture( 1, SecPalette );
			}
			SecDevice->SetPixelShader(PaletteShader);
		}
	}
	g_D3dInReset = false;
	return true;
}

// th2 ---------------------------------------------------
void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp)
{
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = BigWin || ! IsFullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = ( ! BigWin && IsFullScreen ) ? ( D3dRef ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8 ) : D3DFMT_UNKNOWN; // (rt) UNKNOWN - the desktop colour depth
	//d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE; // do not work
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = (IsFullScreen && !BigWin) ? ScreenWidth : 0; // WinWidth;
	d3dpp.BackBufferHeight = (IsFullScreen && !BigWin) ? ScreenHeight : 0; // WinHeight;
	d3dpp.PresentationInterval = ( Vsync && (LoadOnStart || gbRunGame) && !BigWin && IsFullScreen) ? D3DPRESENT_INTERVAL_ONE :  // v-sync, except main menu and windowed
	                                                                            D3DPRESENT_INTERVAL_IMMEDIATE;
}

//----- (00417FB1) --------------------------------------------------------
HRESULT __fastcall CallDirectDrawCreate ( GUID *guid, LPDIRECTDRAW* ddraw, IUnknown* iid )
{
	if( ! HModule ){
		HModule = LoadLibraryA("ddraw.dll");
	}
	// (rt) according to MSDN, the DirectDrawCreate address must be obtained via GetProcAddress
	// (rt) without this it may not work on some configurations; to be revisited and reworked if needed
	if( !DDraw /*render == d3d*/ ){
		D3DPRESENT_PARAMETERS d3dpp;
		bool TryedBigWin = false;
		buggy_res:
		if( g_pD3D == 0 ){
			g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
			// (rt) shared resources require at least Windows Vista
			//Direct3DCreate9Ex( D3D_SDK_VERSION, &g_pD3D );
		}
		SetD3dParam(d3dpp);
		if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ghMainWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice ) != D3D_OK ){
			if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ghMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice ) != D3D_OK){
				if( !TryedBigWin && IsFullScreen && !BigWin ){
					goto tryBigWin;
				}else{
					D3dRef = true;
					SetD3dParam(d3dpp);
					HRESULT hr = g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, ghMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice );
					if( hr != D3D_OK ){
						//__debugbreak();
						TerminateWithError("Unable to create Direct3D Device");
					}
				}
			}
		}
		if( IsSecondWin ){
			SetD3dParam( d3dpp );
			if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, SecWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &SecDevice ) != D3D_OK ){
				if( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, SecWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &SecDevice ) != D3D_OK ){
					TerminateWithError( "Unable to create second window Direct3D Device" );
				}
			}
		}
		if( !D3dDevice && IsFullScreen && !BigWin && !TryedBigWin ){
			tryBigWin:
			// (rt) handling odd resolutions and fullscreen in remote desktop
			BigWin = true;
			TryedBigWin = true;
			WinWidth = SystemWidth;
			WinHeight = SystemHeight;
			SetWindowLongPtr( ghMainWnd, GWL_STYLE, WS_POPUP );
			SetWindowPos( ghMainWnd, HWND_NOTOPMOST, 0, 0, SystemWidth, SystemHeight, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
			goto buggy_res;
		}
	}
	// (rt) direct draw can't be fully disabled yet, since storm draws the menu and videos into it
	// also used in windows xp, vm and linux
	HRESULT hresult = DirectDrawCreate( guid, ddraw, iid );
	if( DDraw ){
		DirectDrawPtr->SetCooperativeLevel( ghMainWnd, DDSCL_NORMAL );
		#ifdef DDRAW_EXCLUSIVE_FULLSCREEN_NOT_READY
		DirectDrawPtr->SetCooperativeLevel( ghMainWnd, IsFullScreen && !BigWin ? DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN : DDSCL_NORMAL );
		if( IsFullScreen && !BigWin ){
			DirectDrawPtr->SetDisplayMode( ScreenWidth, ScreenHeight, 32 );
		}
		#endif
	}
	return hresult;
}

//----- (0041802C) --------------------------------------------------------
void LockSrcSurface()
{
	EnterCriticalSection(&DDSurfaceCritSec);
	#if USE_DDRAW_WORKING_SURFACE
	DDSURFACEDESC ddSurfaceDesc; // [sp+0h] [bp-6Ch]@7
	if( SoftSrcSurfacePtr ){
		WorkingSurface = SoftSrcSurfacePtr;
	}else{
		if( !DDSrcSurface ){
			SleepTh(20000);
			TerminateWithError("lock_buf_priv");
		}
		if( !SurfaceLockCount ){
			ddSurfaceDesc.dwSize = sizeof( ddSurfaceDesc );
			HRESULT hr = DDSrcSurface->Lock(0, &ddSurfaceDesc, 1u, 0);
			if( FAILED(hr) )
				ShowDirectDrawError(hr, __LINE__, __FILE__);
			WorkingSurfaceDrawEnd += (int)(ddSurfaceDesc.lpSurface);
			WorkingSurface = (uchar*)(ddSurfaceDesc.lpSurface);
		}
	}
	++SurfaceLockCount;
	#endif
}

//----- (004180BE) --------------------------------------------------------
void UnlockSrcSurface()
{
	#if USE_DDRAW_WORKING_SURFACE
	//DWORD v0; // eax@7  HRESULT hr
	char v1;           // zf@5
	uchar* v2;          // eax@6
	if( !SurfaceLockCount ) //trying to unlock a surface that is not locked
		TerminateWithError("draw main unlock error");
	if( !WorkingSurface )
		TerminateWithError("draw consistency error");
	v1 = SurfaceLockCount-- == 1;
	if( v1 ){
		v2 = WorkingSurface;
		WorkingSurface = 0;
		WorkingSurfaceDrawEnd -= (uint)v2;
		if( !SoftSrcSurfacePtr ){
			HRESULT hr = DDSrcSurface->Unlock(0);
			if( FAILED(hr) )
				ShowDirectDrawError(hr, __LINE__, __FILE__);
		}
	}
	#endif
	LeaveCriticalSection(&DDSurfaceCritSec);
}

// (rt) this function is called on exit, on an error crash,
// (rt) and when the surface is lost and restored after alt-tab
// (rt) it could be marked as not compared, but since there were crashes here in TH 2, it's compared for now
//----- (0041813E) --------------------------------------------------------
void ReleaseDDSurface()
{
	if( ghMainWnd ){
		ShowWindow(ghMainWnd, SW_HIDE);
	}
	SDrawDestroy();
	EnterCriticalSection( &DDSurfaceCritSec );
	if( SoftSrcSurfacePtr ){ free( SoftSrcSurfacePtr ); SoftSrcSurfacePtr = 0; }
	if( LightSurface ){ free( LightSurface ); LightSurface = 0; }
	if( LightSurfaceBase ){ free( LightSurfaceBase ); LightSurfaceBase = 0; }
	if( WallOcclusionSurface ){ free( WallOcclusionSurface ); WallOcclusionSurface = 0; }
	if( IndexFlagSurface ){ free( IndexFlagSurface ); IndexFlagSurface = 0; }
	if( AlphaSurface ){ free( AlphaSurface ); AlphaSurface = 0; }
	if( SpriteLayerSurface ){ free( SpriteLayerSurface ); SpriteLayerSurface = 0; }
	if( LightTintSurface ){ free( LightTintSurface ); LightTintSurface = 0; }
	#if USE_DDRAW_WORKING_SURFACE
	else if( DDSrcSurface ){ ReleaseZero( DDSrcSurface ); }
	#endif
	SurfaceLockCount = 0;
	WorkingSurface = 0;
	ReleaseZero( ScreenSurface32 );
	ReleaseZero( ScreenSurface32Back );
	ReleaseZero( ScreenSurface );
	ReleaseZero( DirectDrawPalettePtr );
	ReleaseZero( DirectDrawPtr );
	//ReleaseZero( DirectDrawPtr7 );
	ReleaseZero( D3dSurface );
	ReleaseZero( VideoSurface );
	SharpScale_Release( 0 );	// (th4)
	ReleaseZero( BackBuffer );
	ReleaseZero( D3dDevice );
	if( IsSecondWin ){
		ReleaseZero( SecD3dSurface );
		ReleaseZero( SecVideoSurface );
		SharpScale_Release( 1 );	// (th4)
		ReleaseZero( SecBuffer );
		ReleaseZero( SecDevice );
	}
	ReleaseZero( g_pD3D );
	LeaveCriticalSection(&DDSurfaceCritSec);
}

//in-game stuff to mess with the resolution
//----- (004181E6) --------------------------------------------------------
void ReturnToDDSurface()
{
	int v0; // esi@1
	int v1; // esi@3
	int v2; // esi@4
	int v3; // eax@3
	EnterCriticalSection(&DDSurfaceCritSec);
	ZeroCursorImageWidth();
	v0 = SurfaceLockCount;
	while( SurfaceLockCount ){
		UnlockSrcSurface();
	}
	ReleaseDDSurface();
	SetupScreenResolution(ghMainWnd);
	v3 = v0;
	v1 = v0 - 1;
	if( v3 ){
		v2 = v1 + 1;
		do{
			LockSrcSurface();
			--v2;
		}while( v2 );
	}
	LeaveCriticalSection(&DDSurfaceCritSec);
}

//----- (00417D08) -------------------------------------------------------- main
void SetupDDSurfaceCriticalSection()
{
	InitializeCriticalSection(&DDSurfaceCritSec);
	atexit( [](){ DeleteCriticalSection(&DDSurfaceCritSec); } );
}
