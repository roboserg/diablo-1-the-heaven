#include "stdafx.h"

PALETTEENTRY BasePalette[256];
PALETTEENTRY ScreenPalette[256];
PALETTEENTRY FilePalette[256];
int CryptPalAnimEachSecFrame;
int AbyssPalAnimEachSecFrame;
int AbyssPalCyclingEachSecFrame;
int CryptPalCyclingEachSecFrame = 0;
int HalfReservedSystemPalette;

// Light level palettes: 19 (15 + 4 LIGTH_TYPE (infra,stone,grey,unique) for dungeon cells (?)
// plus LightTable_BossRowCount rows from index 19 up, one per unique monster on the level.
// (th4) Row map and sizes now live in vars.h -- see LightTable_BossRowFirst there.
uchar LightTable[LightTable_RowCount * 256];

//----- (00455396) --------------------------------------------------------
void SavePaletteSettings()
{
	SRegSaveValue("Hellfire", "Gamma Correction",    0, ScreenGamma   );
	SRegSaveValue("Hellfire", "Contrast Correction", 0, ScreenContrast);
}

//----- (004553AE) --------------------------------------------------------
void SetScreenSurfacePalette()
{
	InitGammaCorrectionValue();
	memcpy(&ScreenPalette, &FilePalette, sizeof(FilePalette));
	InitSystemPalette();
	HRESULT hr = DirectDrawPtr->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, ScreenPalette, &DirectDrawPalettePtr, 0);
	if( hr ){ 
		ErrorIn_CPP_File(ID_DIALOG7, hr, __FILE__, __LINE__);
	}
	// (rt) TH 1 sometimes reports DDERR_INVALIDPIXELFORMAT here
	hr = ScreenSurface->SetPalette(DirectDrawPalettePtr);
	if( hr ){
		ErrorIn_CPP_File(ID_DIALOG7, hr, __FILE__, __LINE__);
	}
}

//----- (00455426) --------------------------------------------------------
void InitGammaCorrectionValue()
{
	if( !SRegLoadValue("Hellfire", "Gamma Correction",    0, &ScreenGamma   ) ){ ScreenGamma    = 100; }
	if( !SRegLoadValue("Hellfire", "Contrast Correction", 0, &ScreenContrast) ){ ScreenContrast = 100; }
	LimitToRange(ScreenGamma,    30, 100);
	LimitToRange(ScreenContrast, 30, 100);
	ScreenGamma    -= ScreenGamma    % 5;
	ScreenContrast -= ScreenContrast % 5;
}

//----- (00455478) --------------------------------------------------------
uchar* InitSystemPalette()
{
	uchar* result; // eax@1
	int v1;       // eax@4
	int v2;       // edx@5
	uchar* v3;     // ecx@5
	int v4;       // edi@7
	int v5;       // eax@8
	HDC v6;       // eax@4
	HDC v7;       // esi@4
	int v8;       // eax@4
	int v9;       // esi@7
	HDC hDC;      // [sp+10h] [bp-4h]@4
	result = &ScreenPalette[0].peFlags;
	do{
		*result = 5;
		result += 4;
	}while( (int)result < (int)&ScreenPalette[256].peFlags );
	if( !UseReservedSystemPalette ){
		v6 = GetDC(0);
		v7 = v6;
		hDC = v6;
		v8 = GetDeviceCaps(v6, NUMRESERVED) / 2;
		HalfReservedSystemPalette = v8;
		GetSystemPaletteEntries(v7, 0, v8, ScreenPalette);
		v1 = HalfReservedSystemPalette;
		if( HalfReservedSystemPalette > 0 ){
			v3 = &ScreenPalette[0].peFlags;
			v2 = HalfReservedSystemPalette;
			do{
				*v3 = 0;
				v3 += 4;
				--v2;
			}while( v2 );
		}
		v9 = 256 - v1;
		v4 = 4 * (256 - v1);
		GetSystemPaletteEntries(hDC, 256 - v1, v1, (PALETTEENTRY*)((char*)&ScreenPalette + v4));
		if( v9 < 256 ){
			v5 = v4 + (int)&ScreenPalette[0].peFlags;
			do{
				*(uchar*)v5 = 0;
				v5 += 4;
			}while( v5 < (int)&ScreenPalette[256].peFlags );
		}
		result = (uchar*)ReleaseDC(0, hDC);
	}
	return result;
}

//----- (0045552B) --------------------------------------------------------
void __fastcall LoadPalette(char* paletteFileName)
{
	HANDLE paletteFile; // [sp+300h] [bp-4h]@1
	PalEntry3 palData[256];      // [sp+0h] [bp-304h]@1
	File_Open(paletteFileName, (void**)&paletteFile, FIVE_TRY);
	// @todo File_Read doesn't return amount of readed data
	if( 0 == File_Read(paletteFile, palData, 256*sizeof(PalEntry3)) ){
		OutputDebugString("Attempt to read pallette has failed.\n");
		return;
	}
	File_Close(paletteFile);
	for( int i = 0; i < 256; ++i ){
		FilePalette[i].peRed   = palData[i].Red;
		FilePalette[i].peGreen = palData[i].Green;
		FilePalette[i].peBlue  = palData[i].Blue;
		FilePalette[i].peFlags = 0;
	}
	//if( strcmp(paletteFileName, "levels\\l3data\\glacial03.pal") == 0 ){
	//	for( int i = 1; i < 32; ++i ){
	//		FilePalette[ i ].peRed   -= 128;
	//		FilePalette[ i ].peGreen -= 128;
	//		FilePalette[ i ].peBlue  -= 128;
	//	}
	//}
	// Town day/night: FilePalette just changed, so the day/night snapshots must
	// be rebuilt from it on the next town frame.
	DayNight_Invalidate();
	Grotto_Invalidate();
}

//----- (00455587) --------------------------------------------------------
void __fastcall SelectRandomLevelPalette(int dungeonType)
{
	int palNum;
	char *palTown;
	int palRange;
	char paletteFile[260];

	// Pot flips in Ice Age-------------------------------
	// Placed here because SetFlipTable() is called before GC data is loaded - it didn't work there.
	// (th4) Ice Age shows the healing potions and the relic of healing on the ground in blue. The
	// normal entries are saved the first time through and put back for every hero WITHOUT Ice Age:
	// ItemFlipTable is global for the whole session, so without the restore a hero played after an
	// Ice Age hero kept blue healing potions on the ground until the game was restarted.
	{
		static const int iceFlip[ 3 ] = { 314 + 168, 449 + 168, 180 + 168 };	// heal pot, full heal pot, relict of heal
		static const uchar iceFlipBlue[ 3 ] = { FL_20_FBTTLEBB, FL_20_FBTTLEBB, FL_56_RELICTBB };
		static uchar iceFlipNormal[ 3 ];
		static bool iceFlipSaved = false;
		if( !iceFlipSaved ){
			for( int k = 0; k < 3; ++k ) iceFlipNormal[ k ] = ItemFlipTable[ iceFlip[ k ] ];
			iceFlipSaved = true;
		}
		const bool iceBlue = ( Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) ) && !UserPotionB && !UserPanelB;
		for( int k = 0; k < 3; ++k ) ItemFlipTable[ iceFlip[ k ] ] = iceBlue ? iceFlipBlue[ k ] : iceFlipNormal[ k ];
	}//--------------------------------------------------------

	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {

		if (dungeonType == DT_3_CAVE) {
			palNum = RNG(5) + 1; // 5 palettes for caves
			sprintf(paletteFile, "Levels\\iceage\\3caves_%i.PAL", palNum);
			LoadPalette(paletteFile);
			return;
		}
		else if (dungeonType == DT_1_CHURCH) {
			palNum = RNG(6) + 1;
			sprintf(paletteFile, "Levels\\iceage\\1church_%i.PAL", palNum);
			LoadPalette(paletteFile);
			return;
		}
		else if (dungeonType == DT_2_CATACOMB) {
			palNum = RNG(4) + 1;
			sprintf(paletteFile, "Levels\\iceage\\2catas_%i.PAL", palNum);
			LoadPalette(paletteFile);
			return;
		}
		else if (dungeonType == DT_4_HELL) {
			palNum = RNG(4) + 1;
			sprintf(paletteFile, "Levels\\iceage\\4hell_%i.PAL", palNum);
			LoadPalette(paletteFile);
			return;
		}
		else if (dungeonType == DT_5_CRYPT) {
			palNum = RNG(3) + 1;
			sprintf(paletteFile, "Levels\\iceage\\5crypt_%i.PAL", palNum);
			LoadPalette(paletteFile);
			return;
		}
		else {
			switch (dungeonType) {
			//case DT_1_CHURCH_OR_CRYPT:		palTown = "Levels\\iceage\\1church.pal"; break;
			//case DT_2_CATACOMB:			palTown = "Levels\\iceage\\2catas.pal"; break;
			//case DT_3_CAVE_OR_ABYSS:		palTown = "Levels\\iceage\\3caves.pal"; break;
			//case DT_4_HELL:					palTown = "Levels\\iceage\\4hell_2.pal"; break;
			//case DT_5_CRYPT:				palTown = "Levels\\iceage\\5crypt.pal"; break;
			case DT_6_ABYSS:				palTown = "Levels\\iceage\\6abyss.pal"; break;
			case DT_0_TOWN:	/*default:*/	palTown = "Levels\\iceage\\0town_P.pal"; break;
			}
			LoadPalette(palTown);
			return;
		}
	}
	else if (original_palettes || GameMode == GM_CLASSIC) {
		if (dungeonType) {
			switch (dungeonType) {
			case DT_1_CHURCH:			palRange = 5;	break;
			case DT_2_CATACOMB:			palRange = 5;	break;
			case DT_3_CAVE:				palRange = 4;	break;
			case DT_4_HELL:				palRange = 4;	break;
			case DT_5_CRYPT:			palRange = 1;	break;
			case DT_6_ABYSS: default:	palRange = 6;	break;
			}
		}
		else {
			palTown = "Levels\\orig_pals\\town.pal";
			LoadPalette(palTown);
			return;
		}
		palNum = RNG(palRange) + 1;
		sprintf(paletteFile, "Levels\\orig_pals\\L%i_%i.PAL", dungeonType, palNum);
	}
	else{
		if (dungeonType) {
			switch (dungeonType) {
			case DT_1_CHURCH:			palRange = 295;	break;
			case DT_2_CATACOMB:			palRange = 300;	break;
			case DT_3_CAVE:				palRange =  93;	break;
			case DT_4_HELL:				palRange = 900;	break;
			case DT_5_CRYPT:			palRange = 311;	break;
			case DT_6_ABYSS: default:	palRange = 166;	break;
			}
		}
		else {// DT_0_TOWN
			if (MaxCountOfPlayersInGame == 1 && (GameMode != GM_IRONMAN && GameMode != GM_SPEEDRUN && GameMode != GM_NIGHTMARE)) {
				if (Difficulty == 1) {
					palTown = "Levels\\TownData\\Towx.pal";
				}
				else if (Difficulty >= 2) {
					palTown = "Levels\\TownData\\Towz.pal";
				}
				else {
					palTown = "Levels\\l0data\\l0_49.pal"; // Town.pal"; 
				}
				LoadPalette(palTown);
				return;
			}
			palRange = 13;
		}
		palNum = RNG(palRange) + 1;
		sprintf(paletteFile, "Levels\\L%iData\\L%i_%i.PAL", dungeonType, dungeonType, palNum);
		if (dungeonType == 6) {
			//if( !NotUsedFlagWithPalette ){
				//++palNum; // perhaps this flag is obsolete
			//}
			sprintf(paletteFile, "NLevels\\L%iData\\L%iBase%i.PAL", 6, 6, palNum);
		}
	}
	//else if (dungeonType == 3) {
	//	sprintf(paletteFile, "Levels\\L3Data\\l3_95.PAL");
	//}
	LoadPalette( paletteFile );
}

//----- (00455617) --------------------------------------------------------
void RestoreScreenSurface()
{
	if( DDraw /*render == ddraw*/ ){
		if( !ScreenSurface32     || ScreenSurface32->IsLost()     != DDERR_SURFACELOST || SUCCEEDED(ScreenSurface32->Restore())
		 && !ScreenSurface32Back || ScreenSurface32Back->IsLost() != DDERR_SURFACELOST || SUCCEEDED(ScreenSurface32Back->Restore())){
			//#ifdef USE_32BITCOLOR
			SDrawRealizePalette();
			//#endif
		}
	}
}

//----- (00455642) --------------------------------------------------------
void IncPaletteGamma()
{
	if( ScreenGamma < 100 ){
		ScreenGamma += 5;
		if( ScreenGamma > 100 )
			ScreenGamma = 100;
		ApplyGammaToPalette(ScreenPalette, BasePalette, 256);
		UpdatePalette();
	}
}

//----- (0045567B) --------------------------------------------------------
void UpdatePalette()
{
	//(rt) if( th2 ) return; // this way garbage sometimes appears on screen during fades, e.g. when exiting the game; look into it
	UINT paletteCount; // eax@3
	UINT startPaletteIndex; // ecx@3
	if( DirectDrawPalettePtr ){
		if( UseReservedSystemPalette ){
			startPaletteIndex = 0;
			paletteCount = 256;
		}else{
			startPaletteIndex = HalfReservedSystemPalette;
			paletteCount = 256 - 2 * HalfReservedSystemPalette;
		}
		//#ifndef USE_32BITCOLOR
		SDrawUpdatePalette( startPaletteIndex, paletteCount, &ScreenPalette[ startPaletteIndex ], 0 );
		//#endif
	}
}

//----- (004556B6) --------------------------------------------------------
void __fastcall ApplyGammaToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount)
{
	//ScreenGamma - uint[100 - 30] 1.00 - 0.30
	double gammaPercent = 0.01 * ScreenGamma;
	for (int i = 0; i < paletteCount; ++i) {
		screenPalette[i].peRed   = ftol(pow(GammaKoef * basePalette[i].peRed,   gammaPercent) * 256.0);
		screenPalette[i].peGreen = ftol(pow(GammaKoef * basePalette[i].peGreen, gammaPercent) * 256.0);
		screenPalette[i].peBlue  = ftol(pow(GammaKoef * basePalette[i].peBlue,  gammaPercent) * 256.0);
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall ApplyContrastToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount)
{
	int mul = (100 - ScreenContrast) * 16 / 70 + 16;
	for( int i = 0; i < paletteCount; ++i ){
		screenPalette[i].peRed   = min( basePalette[i].peRed   * mul / 16, 255 );
		screenPalette[i].peGreen = min( basePalette[i].peGreen * mul / 16, 255 );
		screenPalette[i].peBlue  = min( basePalette[i].peBlue  * mul / 16, 255 );
	}
	screenPalette[255].peRed = screenPalette[255].peGreen = screenPalette[255].peBlue = 5;
}

//----- (th2) -------------------------------------------------------------
void __fastcall ApplyGammaAndContrastToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount)
{
	int mul = (100 - ScreenContrast) * 16 / 70 + 16;
	double gammaPercent = 0.01 * ScreenGamma;
	for( int i = 0; i < paletteCount; ++i ){
		screenPalette[i].peRed   = min( ftol(pow(GammaKoef * basePalette[i].peRed,   gammaPercent) * 256.0) * mul / 16, 255 );
		screenPalette[i].peGreen = min( ftol(pow(GammaKoef * basePalette[i].peGreen, gammaPercent) * 256.0) * mul / 16, 255 );
		screenPalette[i].peBlue  = min( ftol(pow(GammaKoef * basePalette[i].peBlue,  gammaPercent) * 256.0) * mul / 16, 255 );
	}
	screenPalette[255].peRed = screenPalette[255].peGreen = screenPalette[255].peBlue = 5;
}

//----- (00455787) --------------------------------------------------------
void DecPaletteGamma()
{
	if( ScreenGamma > 30 ){
		ScreenGamma -= 5;
		if( ScreenGamma < 30 )
			ScreenGamma = 30;
		ApplyGammaToPalette(ScreenPalette, BasePalette, 256);
		UpdatePalette();
	}
}

//----- (004557C0) --------------------------------------------------------
int __fastcall SetGammaByPos(int gammaPos)
{
	if( gammaPos ){
		ScreenGamma = 130 - gammaPos;
		ApplyGammaAndContrastToPalette(ScreenPalette, BasePalette, 256); // ApplyGammaAToPalette
		UpdatePalette();
	}
	return 130 - ScreenGamma;
}

//----- (th2) --------------------------------------------------------
int __fastcall SetContrastByPos(int contrastPos)
{
	if( contrastPos ){
		ScreenContrast = 130 - contrastPos;
		ApplyGammaAndContrastToPalette(ScreenPalette, BasePalette, 256); // ApplyContrastToPalette
		UpdatePalette();
	}
	return 130 - ScreenContrast;
}

//----- (004557F5) --------------------------------------------------------
void BlackPalette()
{
	SetFadeLevel(0);
	//DrawNotGameplayScreen( 0 );
}

// (rt) brightness from 0 to 256
// (th4) The level SetFadeLevel last applied, 0..256. This is the ONLY place the
// in-game palette is scaled for a fade, so it is the one number that says how far
// the 8-bit frame is from black. True-colour overlays that live outside the palette
// (AnimOrbs) read it to fade in step -- without it they stay full-bright while the
// screen goes dark under them.
int PaletteFadeLevel = 256;

// (th4) True only while PaletteFadeIn/Out is running its loop. Those loops do NOT
// redraw anything -- they call SetFadeLevel + DrawNotGameplayScreen, which re-converts
// the SAME 8-bit WorkingSurface at a new brightness. Anything drawn from a per-frame
// queue (ItemOverlay) therefore has nothing to draw and must replay its last queue
// instead of blinking out. A brightness test is not enough: the first fade-out step is
// still at 256, which left a one-frame gap.
bool PaletteFading = false;

//----- (004557FC) --------------------------------------------------------
void __fastcall SetFadeLevel(int brightness)
{
	PaletteFadeLevel = brightness < 0 ? 0 : brightness > 256 ? 256 : brightness;
	if( DirectDrawPtr ){
		for( int i = 0; i < 255; ++i ){
			ScreenPalette[i].peRed	= (brightness * BasePalette[i].peRed) >> 8;
			ScreenPalette[i].peGreen= (brightness * BasePalette[i].peGreen) >> 8;
			ScreenPalette[i].peBlue	= (brightness * BasePalette[i].peBlue) >> 8;
		}
		if( !(IsFullScreen && !BigWin) && Vsync ){
			//(rt) Sleep( 3 ); // if vsync is hardware-disabled on ddraw (?); disabled for now to check whether that happens 
			DirectDrawPtr->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
		}
		UpdatePalette();
	}
}

//----- (00455867) --------------------------------------------------------
void* __fastcall PaletteFadeIn(int delta)
{
	ApplyGammaAndContrastToPalette(BasePalette, FilePalette, 256);
	PaletteFading = true;
	for( int brightness = 0; brightness < 256; brightness += delta ){
		SetFadeLevel(brightness);
		DrawNotGameplayScreen(0);
	}
	SetFadeLevel(256); // (rt) this call was not restored in th1
	DrawNotGameplayScreen(0);
	PaletteFading = false;   // (th4) after the final frame, not before it
	void* result = memcpy(&BasePalette, &FilePalette, sizeof(FilePalette));
	PaletteLightening = 1;
	return result;
}

//----- (004558BD) --------------------------------------------------------
void __fastcall PaletteFadeOut(int delta)
{
	if( PaletteLightening ){
		PaletteFading = true;
		for( int brightness = 256; brightness > 0; brightness -= delta ){
			SetFadeLevel(brightness);
			DrawNotGameplayScreen(0);
		}
		SetFadeLevel(0); // (rt) this call was not restored in th1
		DrawNotGameplayScreen(0);
		PaletteFading = false;
		PaletteLightening = 0;
	}
}

//----- (004558ED) --------------------------------------------------------
void AnimateCavePalette()
{
	PALETTEENTRY _1 = ScreenPalette[1];
	for( int i = 1; i < 31; ++i ){
		ScreenPalette[ i ] = ScreenPalette[ i + 1 ];
	}
	ScreenPalette[ 31 ] = _1;
	UpdatePalette();
}

//----- (00455939) --------------------------------------------------------
void AnimateCryptPalette()
{
	if( CryptPalCyclingEachSecFrame <= 1 ){
		++CryptPalCyclingEachSecFrame;
	}else{
		PALETTEENTRY _15 = ScreenPalette[15];
		for( int i = 15; i > 1; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 1 ] = _15;
		CryptPalCyclingEachSecFrame = 0;
	}
	if( CryptPalAnimEachSecFrame <= 0 ){
		CryptPalAnimEachSecFrame = 1;
	}else{
		PALETTEENTRY _31 = ScreenPalette[31];
		for( int i = 31; i > 16; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 16 ] = _31;
		UpdatePalette();
		++CryptPalAnimEachSecFrame; // must be --CryptPalAnimEachSecFrame, original copy-paste bug (from AbyssPaletteAnim)
	}
}

//----- (00455A07) --------------------------------------------------------
void AnimateAbyssPalette()
{
	if( AbyssPalCyclingEachSecFrame == 2 ){
		PALETTEENTRY _8 = ScreenPalette[ 8 ];
		for( int i = 8; i > 1; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 1 ] = _8;
		AbyssPalCyclingEachSecFrame = 0;
	}else{
		++AbyssPalCyclingEachSecFrame;
	}
	if( AbyssPalAnimEachSecFrame == 2 ){
		PALETTEENTRY _15 = ScreenPalette[ 15 ];
		for( int i = 15; i > 9; --i ){
			ScreenPalette[ i ] = ScreenPalette[ i - 1 ];
		}
		ScreenPalette[ 9 ] = _15;
		UpdatePalette();
		AbyssPalAnimEachSecFrame = 0;
	}else{
		++AbyssPalAnimEachSecFrame;
	}
}

//----- (00455AD2) --------------------------------------------------------
void __fastcall ApplyAnimFilePalette(int animPalReserve)
{
	for( int i = 32 - animPalReserve; i >= 0; --i ){
		BasePalette[i] = FilePalette[i];
	}
	ApplyGammaToPalette(ScreenPalette, BasePalette, 32);
	UpdatePalette();
}
// ============================================================================
//  Town day / night cycle
// ----------------------------------------------------------------------------
//  Interpolates the town terrain colours between a "day" snapshot (the palette
//  as loaded from disk) and a procedurally darkened "night" version, driven by
//  the real wall-clock (GetTickCountTh, ticks of 1/MAX_FPS s) so it never
//  affects gameplay logic or MP sync. Church emissive-red glow indices are
//  auto-detected and excluded so they stay full-bright at all hours.
//
//  All knobs are the constants below - tweak freely.
// ============================================================================

// --- tunables ---------------------------------------------------------------
// GetTickCountTh() returns time in units of 1/MAX_FPS second (MAX_FPS = 10000),
// i.e. 1 second = 10000 ticks. NOT milliseconds.
static const i64  DAYNIGHT_TICKS_PER_SEC = MAX_FPS;                 // 10000
// Full day -> night -> day cycle length, in real seconds. 60 min default.
static const i64  DAYNIGHT_CYCLE_SEC     = 60LL/*60 minute cycle*/ * 60LL;             // 3600 s
static const i64  DAYNIGHT_CYCLE_TICKS   = DAYNIGHT_CYCLE_SEC * DAYNIGHT_TICKS_PER_SEC;
// Day:night length ratio. Day lasts DAYNIGHT_DAY_WEIGHT, night lasts
// DAYNIGHT_NIGHT_WEIGHT. 2:1 => day is twice as long as night.
static const int  DAYNIGHT_DAY_WEIGHT    = 2;
static const int  DAYNIGHT_NIGHT_WEIGHT  = 1;
// Terrain palette index range that the cycle is allowed to touch. Indices
// 128..255 are UI / orbs / light ramps / text and must never shift. Index 0 is
// the black/shadow anchor and is left alone.
static const int  DAYNIGHT_TERRAIN_FIRST = 1;
static const int  DAYNIGHT_TERRAIN_LAST  = 127;   // inclusive

// (th4) The tooltip dim ramp, blended alongside the terrain.
//
// DrawBoxTransparent's town branch (Draw.cpp:202) rewrites every pixel under a tooltip as
// 252 + (index >> 5) -- and its cutColors is 128, so the pixels it rewrites are EXACTLY the
// terrain range below. Every pixel the night cycle darkens was being moved into four entries
// the night cycle ignored, so after dusk the box under a tooltip kept its full daytime colours
// while the town around it went dark. Invisible by day, because 252..255 IS the correct dim
// ramp then; it only shows once the terrain has blended away from it.
//
// 255 is deliberately excluded: ApplyContrastToPalette and ApplyGammaAndContrastToPalette both
// pin it to near-black (5,5,5) after every rebuild, so it is already as dark as it goes and
// writing it here would just fight them.
static const int  DAYNIGHT_DIMRAMP_FIRST = 252;
static const int  DAYNIGHT_DIMRAMP_LAST  = 254;   // inclusive

static bool DayNight_IsManaged( int i )
{
	return ( i >= DAYNIGHT_TERRAIN_FIRST && i <= DAYNIGHT_TERRAIN_LAST )
	    || ( i >= DAYNIGHT_DIMRAMP_FIRST && i <= DAYNIGHT_DIMRAMP_LAST );
}
// Night look: brightness multiplier at deepest midnight (0..256, 256 = day).
static const int  DAYNIGHT_NIGHT_BRIGHT  = 90;    // ~35% of daytime
// Cool blue-shift at midnight (added to blue, subtracted from red), 0..64.
static const int  DAYNIGHT_NIGHT_BLUE    = 14;
// ----------------------------------------------------------------------------

static PALETTEENTRY DayNight_DayPal  [256]; // daytime snapshot (from FilePalette)
static PALETTEENTRY DayNight_NightPal [256]; // procedural night version
static bool         DayNight_GlowMask [256]; // true = emissive glow, do not shift
static bool         DayNight_Ready = false;  // snapshot built for current town pal

// (th4) Night-ness threshold for the town AMBIENCE switch (not the visuals).
// night is 0..256 (0 = day, 256 = deep night); switch around dusk midpoint.
// Hysteresis: cross UP at HI to go night, DOWN at LO to go day, so the ambient
// loop can't flip-flop while sitting exactly on the boundary.
static const int  DAYNIGHT_AMB_NIGHT_HI = 140;  // >= this -> night ambience
static const int  DAYNIGHT_AMB_NIGHT_LO = 116;  // <= this -> day ambience
static bool       DayNight_IsNightAmb   = false; // latched night/day for ambience

// Detect a church emissive-red glow entry: strongly red-dominant & saturated.
static bool DayNight_IsGlow( const PALETTEENTRY& c )
{
	int r = c.peRed, g = c.peGreen, b = c.peBlue;
	int hi = g > b ? g : b;
	return r >= 90 && g <= 25 && b <= 25 && (r - hi) >= 70;
}

// Build day + night snapshots from the currently loaded FilePalette. Call once
// on town entry (or whenever the town palette is (re)loaded).
void DayNight_BuildSnapshots()
{
	memcpy( DayNight_DayPal, FilePalette, sizeof(DayNight_DayPal) );

	for( int i = 0; i < 256; ++i ){
		DayNight_NightPal[i] = FilePalette[i];
		DayNight_GlowMask[i] = false;

		bool inRange = DayNight_IsManaged( i );
		if( !inRange ) continue;

		if( DayNight_IsGlow( FilePalette[i] ) ){
			DayNight_GlowMask[i] = true; // emissive: keep full-bright at night
			continue;
		}

		int r = FilePalette[i].peRed;
		int g = FilePalette[i].peGreen;
		int b = FilePalette[i].peBlue;
		// darken
		r = r * DAYNIGHT_NIGHT_BRIGHT >> 8;
		g = g * DAYNIGHT_NIGHT_BRIGHT >> 8;
		b = b * DAYNIGHT_NIGHT_BRIGHT >> 8;
		// cool blue-shift
		r -= DAYNIGHT_NIGHT_BLUE;
		b += DAYNIGHT_NIGHT_BLUE;
		LimitToRange( r, 0, 255 );
		LimitToRange( g, 0, 255 );
		LimitToRange( b, 0, 255 );
		DayNight_NightPal[i].peRed   = (BYTE)r;
		DayNight_NightPal[i].peGreen = (BYTE)g;
		DayNight_NightPal[i].peBlue  = (BYTE)b;
	}
	DayNight_Ready = true;
}

// Force a rebuild on the next frame (e.g. after loading a new town palette).
void DayNight_Invalidate()
{
	DayNight_Ready = false;
}

// 0..256 night-ness for a given tick timestamp (ticks = 1/MAX_FPS s).
// 0 = full day, 256 = deep night. The cycle is split into four segments so the
// day plateau can be longer than the night plateau (DAY_WEIGHT:NIGHT_WEIGHT),
// with eased dawn/dusk transitions between them:
//     [ day plateau ][ dusk ][ night plateau ][ dawn ]
//        DAY_WEIGHT      1       NIGHT_WEIGHT     1      (in weight units)
static int DayNight_PhaseQ8( i64 ticks )
{
	i64 cycle = DAYNIGHT_CYCLE_TICKS > 0 ? DAYNIGHT_CYCLE_TICKS : 1;
	i64 pos   = ticks % cycle; if( pos < 0 ) pos += cycle;

	// segment widths in weight units, converted to a shared fixed-point scale
	i64 wDay   = DAYNIGHT_DAY_WEIGHT;   // day plateau
	i64 wDusk  = 1;                     // day -> night transition
	i64 wNight = DAYNIGHT_NIGHT_WEIGHT; // night plateau
	i64 wDawn  = 1;                     // night -> day transition
	i64 wTotal = wDay + wDusk + wNight + wDawn;

	// position within the cycle scaled to weight units (fixed point, *1024)
	i64 u = pos * wTotal * 1024 / cycle; // 0 .. wTotal*1024
	i64 bDay   = wDay   * 1024;
	i64 bDusk  = bDay  + wDusk  * 1024;
	i64 bNight = bDusk + wNight * 1024;

	i64 tri; // 0 = day, 256 = night (pre-easing)
	if( u < bDay ){
		tri = 0;                                       // day plateau
	}else if( u < bDusk ){
		tri = ( (u - bDay)  * 256 ) / (wDusk * 1024);  // dusk: 0 -> 256
	}else if( u < bNight ){
		tri = 256;                                     // night plateau
	}else{
		tri = 256 - ( (u - bNight) * 256 ) / (wDawn * 1024); // dawn: 256 -> 0
	}
	if( tri < 0 )   tri = 0;
	if( tri > 256 ) tri = 256;

	// smoothstep easing t*t*(3-2t) on 0..256 fixed point
	i64 t = tri;
	i64 eased = ( t * t * (3 * 256 - 2 * t) ) / (256 * 256);
	if( eased < 0 )   eased = 0;
	if( eased > 256 ) eased = 256;
	return (int)eased;
}

// (th4) NIGHT AS LIGHTING (Enriched Light modes: PixelLight + g_A8L8Lighting, incl. Ultra).
// Darkening palette entries 1..127 also darkened the interface, whose art uses the same
// entries. In the per-pixel lighting modes every pixel's final colour is palette * (1 - L/255)
// with L from LightSurface, the world writes L from the light fill, and every UI write sets
// L = 0 (full bright) -- so the night is applied there instead: the palette stays at DAY and
// Draw.cpp TownNight_ApplyToLight lowers the ambient light of the world after the fill.
// Classic lighting has no light buffer and keeps the palette method below.
static int DayNight_LightNight = 0;		// 0..256 night-ness handed to the light buffer, 0 when the palette does it
// (th4) Classic has no day/night in any form (Mordor's call): answer "day" there, like the other
// DayNight_* getters that drive the picture (DayNight_NightIndex is not one of them: only Weather.cpp
// reads it, behind WeatherHere, which excludes Classic). Needed even though AnimateTownPalette never
// runs in Classic: this static is
// written only there and never reset, so a town night from an earlier game in the same session
// would otherwise stay frozen over a Classic town (Draw.cpp TownNight_ApplyToLight, Enriched Light).
int DayNight_LightNightQ8() { return GameMode == GM_CLASSIC ? 0 : DayNight_LightNight; }
// Light-buffer darkness (0..255) matching the palette night at full strength: the palette method
// scaled RAW colours by NIGHT_BRIGHT/256 before gamma, i.e. by (NIGHT_BRIGHT/256)^gamma on screen.
int DayNight_LightNightMaxL()
{
	const double k = pow( DAYNIGHT_NIGHT_BRIGHT / 256.0, 0.01 * ScreenGamma );
	int L = (int)( 255.0 * ( 1.0 - k ) + 0.5 );
	return L < 0 ? 0 : ( L > 255 ? 255 : L );
}

// Per-frame town update. Interpolates the terrain entries between the day and
// night snapshots (which live in raw file-palette space), then runs the blended
// colour through the same gamma + contrast transform the rest of the engine
// uses, so the player's brightness/contrast settings are honoured at all hours.
// Glow indices and out-of-range indices are left exactly as the engine set them.
// (th4) Ice Age nights are lighter: the night's strength is scaled to this share (60% = 40%
// less dark) in the town palette blend / the Enriched Light night (AnimateTownPalette) and in the
// sprite darkening (DayNight_TownSpriteDark). The phase itself -- dusk and dawn timing, the
// ambience latch, DayNight_NightQ8 / NightPosQ8 for the weather effects -- is unchanged.
static const int DAYNIGHT_ICE_AGE_NIGHT_PCT = 60;
static int DayNight_IceAgeScale( int night )
{
	return ( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) ? night * DAYNIGHT_ICE_AGE_NIGHT_PCT / 100 : night;
}

void AnimateTownPalette()
{
	if( !DayNight_Ready ){
		DayNight_BuildSnapshots();
	}

	int night = DayNight_PhaseQ8( GetTickCountTh() ); // 0..256
	const int phaseNight = night;
	// (th4) lightning (Weather.cpp, storm nights): for the length of a flash the night is scaled
	// back toward day -- in the palette blend below, or in the light buffer under Enriched Light.
	// phaseNight (the ambience latch) stays unflashed.
	night = night * ( 256 - Weather_FlashQ8() ) >> 8;
	night = DayNight_IceAgeScale( night );	// (th4) lighter Ice Age nights
	DayNight_LightNight = 0;
	if( PixelLight && g_A8L8Lighting ){	// light-buffer night (see the note above): palette stays at day
		DayNight_LightNight = night;
		night = 0;
	}
	int day   = 256 - night;

	// (th4) latch day/night for the ambience switch, with hysteresis (see consts)
	if( phaseNight >= DAYNIGHT_AMB_NIGHT_HI )      DayNight_IsNightAmb = true;
	else if( phaseNight <= DAYNIGHT_AMB_NIGHT_LO ) DayNight_IsNightAmb = false;

	int mul = (100 - ScreenContrast) * 16 / 70 + 16;
	double gammaPercent = 0.01 * ScreenGamma;

	// (th4) Two ranges now: the terrain, and the tooltip dim ramp DrawBoxTransparent maps that
	// terrain into. Both have to ride the same phase, or a tooltip at night is a daylit hole.
	for( int pass = 0; pass < 2; ++pass ){
		const int first = pass ? DAYNIGHT_DIMRAMP_FIRST : DAYNIGHT_TERRAIN_FIRST;
		const int last  = pass ? DAYNIGHT_DIMRAMP_LAST  : DAYNIGHT_TERRAIN_LAST;
		for( int i = first; i <= last; ++i ){
			if( DayNight_GlowMask[i] ) continue; // emissive glow stays as-is

			// blend in raw palette space
			int r = ( DayNight_DayPal[i].peRed   * day + DayNight_NightPal[i].peRed   * night ) >> 8;
			int g = ( DayNight_DayPal[i].peGreen * day + DayNight_NightPal[i].peGreen * night ) >> 8;
			int b = ( DayNight_DayPal[i].peBlue  * day + DayNight_NightPal[i].peBlue  * night ) >> 8;

			// same gamma + contrast the engine applies (see ApplyGammaAndContrastToPalette)
			r = min( ftol(pow(GammaKoef * r, gammaPercent) * 256.0) * mul / 16, 255 );
			g = min( ftol(pow(GammaKoef * g, gammaPercent) * 256.0) * mul / 16, 255 );
			b = min( ftol(pow(GammaKoef * b, gammaPercent) * 256.0) * mul / 16, 255 );

			ScreenPalette[i].peRed   = (BYTE)r;
			ScreenPalette[i].peGreen = (BYTE)g;
			ScreenPalette[i].peBlue  = (BYTE)b;
		}
	}
	UpdatePalette();
}

// (th4) Is it currently "night" for the purpose of the town ambience swap?
// Latched by AnimateTownPalette from the same phase value that drives the
// visuals, with hysteresis around the dusk midpoint. Meaningful only in town
// (that's the only place AnimateTownPalette runs); elsewhere it holds its last
// town value, which callers ignore because they gate on DT_0_TOWN first.
// (th4) Town-night sprite darkening. NPCs, cows and town objects are drawn through the
// normal DarkLevel-shaded paths, but town's LightMap is fully lit, so they stayed
// day-bright all night while the terrain palette blended to darkness around them.
// This returns a DarkLevel contribution derived from the SAME eased phase that drives
// the terrain blend, so sprites fade with dusk in lockstep with the ground.
int DayNight_TownSpriteDark()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || !DayNight_Ready ) return 0;
	int night = DayNight_PhaseQ8( GetTickCountTh() ); // 0..256, smoothstep-eased
	night = night * ( 256 - Weather_FlashQ8() ) >> 8;	// (th4) lightning lights the sprites too (Weather.cpp)
	night = DayNight_IceAgeScale( night );	// (th4) lighter Ice Age nights
	enum { DAYNIGHT_SPRITE_DARK_MAX = 4 };	// how dark sprites get at full night; tune freely (8 was too heavy per playtest)
	// (th4) Ice Age: heroes, NPCs, cows (and the town objects drawn the same way) darken only half as
	// much at night -- on top of the lighter Ice Age night above. Every lighting mode reads this value.
	enum { DAYNIGHT_ICE_AGE_SPRITE_PCT = 50 };
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) )
		return night * DAYNIGHT_SPRITE_DARK_MAX * DAYNIGHT_ICE_AGE_SPRITE_PCT / 100 / 256;
	return night * DAYNIGHT_SPRITE_DARK_MAX / 256;
}

// (th4) Town night strength, 0 (day) .. 256 (full night), smoothstep-eased: the SAME phase that
// drives the terrain blend and DayNight_TownSpriteDark, so anything fading with it stays in
// lockstep with dusk and dawn. 0 outside town, in Classic (no cycle) and before the snapshots
// exist. Used by dx_utility.cpp Mist_Active (town ground mist).
// (th4) Number of the current day/night cycle (0, 1, 2, ...). A night lies inside one cycle
// ([day][dusk][night][dawn], see DayNight_PhaseQ8), so this is also "which night it is".
// Weather.cpp rolls one weather per value.
int DayNight_NightIndex()
{
	const i64 cycle = DAYNIGHT_CYCLE_TICKS > 0 ? DAYNIGHT_CYCLE_TICKS : 1;
	const i64 t = GetTickCountTh();
	return (int)( ( t >= 0 ? t : 0 ) / cycle );
}

// (th4) Where we are inside the night, 0..255: 0 = dusk begins, 128 = middle of the dark
// plateau, 255 = dawn almost over; -1 by day. Same segments as DayNight_PhaseQ8 (day, dusk,
// night, dawn by weight), linear. Used by Fireflies.cpp (first half of the night only).
int DayNight_NightPosQ8()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || !DayNight_Ready ) return -1;
	const i64 cycle = DAYNIGHT_CYCLE_TICKS > 0 ? DAYNIGHT_CYCLE_TICKS : 1;
	i64 pos = GetTickCountTh() % cycle; if( pos < 0 ) pos += cycle;
	const i64 wTotal = DAYNIGHT_DAY_WEIGHT + 1 + DAYNIGHT_NIGHT_WEIGHT + 1;	// day, dusk, night, dawn
	const i64 u = pos * wTotal * 1024 / cycle;
	const i64 nightStart = DAYNIGHT_DAY_WEIGHT * 1024;
	if( u < nightStart ) return -1;
	const i64 nightLen = ( 1 + DAYNIGHT_NIGHT_WEIGHT + 1 ) * 1024;
	int q = (int)( ( u - nightStart ) * 256 / nightLen );
	return q > 255 ? 255 : q;
}

// (th4) Where we are inside the DAY, 0..255: 0 = the day plateau begins (dawn just ended), 255 = dusk
// is about to begin; -1 at dusk, night and dawn. Same segments and conditions as DayNight_NightPosQ8
// (one cycle = [day][dusk][night][dawn], so the day of cycle N follows night N - 1). Used by
// Weather.cpp Weather_WetQ8 (the town dries out over the day after a storm).
int DayNight_DayPosQ8()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || !DayNight_Ready ) return -1;
	const i64 cycle = DAYNIGHT_CYCLE_TICKS > 0 ? DAYNIGHT_CYCLE_TICKS : 1;
	i64 pos = GetTickCountTh() % cycle; if( pos < 0 ) pos += cycle;
	const i64 wTotal = DAYNIGHT_DAY_WEIGHT + 1 + DAYNIGHT_NIGHT_WEIGHT + 1;	// day, dusk, night, dawn
	const i64 u = pos * wTotal * 1024 / cycle;
	const i64 dayLen = DAYNIGHT_DAY_WEIGHT * 1024;
	if( u >= dayLen ) return -1;
	int q = (int)( u * 256 / dayLen );
	return q > 255 ? 255 : q;
}

int DayNight_NightQ8()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || !DayNight_Ready ) return 0;
	return DayNight_PhaseQ8( GetTickCountTh() );
}

bool DayNight_IsNight()
{
	// Classic mode has no day/night cycle, so AnimateTownPalette never runs and the
	// latch below would keep a stale value from an earlier non-Classic game. Classic
	// town is always day -- answer authoritatively here so every caller agrees with
	// what's actually on screen.
	if( GameMode == GM_CLASSIC ) return false;
	return DayNight_IsNightAmb;
}

// ============================================================================
//  Mossy Grotto hallucination palette (TH4)
// ----------------------------------------------------------------------------
//  Slow, sickly colour drift for DUN_52_MOSSY_GROTTO. The terrain colours
//  breathe toward a small curated set of "sick" tints (bruise-purple, plague-
//  green, jaundice-yellow, dried-blood) and back, cross-fading slowly between
//  them so it never lands on clean/cheerful hues. The animated puddle band
//  (indices 1..31) keeps its rotating shimmer AND gets a stronger sickly pulse,
//  so it stays the hypnotic focal point. UI/orbs/light (128..255) untouched.
//
//  Driven by GetTickCountTh (ticks of 1/MAX_FPS s); no gameplay/MP effect.
//  All knobs are the constants below.
// ============================================================================

// --- tunables ---------------------------------------------------------------
// Terrain range that drifts (puddles 1..31 are handled specially below).
static const int GROTTO_TERRAIN_FIRST = 32;
static const int GROTTO_TERRAIN_LAST  = 127;
// Puddle / shimmer band (kept rotating by the cave cycle, tinted extra here).
static const int GROTTO_PUDDLE_FIRST  = 1;
static const int GROTTO_PUDDLE_LAST   = 31;
// Seconds for one full pass through all sickly tints (whole "wrongness" cycle).
static const i64 GROTTO_TINT_CYCLE_SEC = 48;
// Max blend toward a tint at the peak of the breath, 0..256. Lower = subtler.
static const int GROTTO_TERRAIN_STRENGTH = 120;  // pull on walls/moss (lum-gated)
static const int GROTTO_PUDDLE_STRENGTH  = 150;  // hue swing on the puddles
static const int GROTTO_PUDDLE_SAT       = 200;  // <256 = slightly desaturate (not neon)
// Breathing (saturation/brightness pulse) period in seconds and depth (0..64).
static const i64 GROTTO_BREATH_SEC   = 11;
static const int GROTTO_BREATH_DEPTH = 22;
// ----------------------------------------------------------------------------

// Curated sickly tint set. Values are reference hues at full brightness; they
// are scaled down per-pixel by source luminance in Grotto_Sicken, so only mid/
// bright terrain takes strong colour while shadows stay dark.
// Deliberately no clean blues/cyans/whites - only bruise/plague/jaundice/blood.
static const int GROTTO_TINTS[][3] = {
	{ 150,  40, 210 },  // bruise purple
	{  70, 180,  55 },  // plague green
	{ 210, 190,  20 },  // jaundice yellow
	{ 180,  30,  30 },  // dried blood
	{ 110,  55, 170 },  // deep violet rot
};
static const int GROTTO_TINT_COUNT = (int)(sizeof(GROTTO_TINTS)/sizeof(GROTTO_TINTS[0]));

static PALETTEENTRY Grotto_BasePal[256]; // terrain snapshot (file-palette space)
static bool         Grotto_Ready = false;

void Grotto_Invalidate(){ Grotto_Ready = false; }

static void Grotto_BuildSnapshot()
{
	memcpy( Grotto_BasePal, FilePalette, sizeof(Grotto_BasePal) );
	Grotto_Ready = true;
}

// triangle wave 0..256..0 for a phase in [0,period)
static int Grotto_Tri256( i64 pos, i64 period )
{
	if( period <= 0 ) return 0;
	i64 half = period / 2;
	i64 tri  = pos < half ? (pos * 256 / half)
	                      : (256 - (pos - half) * 256 / half);
	if( tri < 0 ) tri = 0; if( tri > 256 ) tri = 256;
	return (int)tri;
}

// Recolour a base colour toward a sickly tint WITHOUT raising its brightness.
// The tint is scaled by the source pixel's own luminance, so a near-black rock
// stays near-black (the tint barely applies) while mid/bright pixels take the
// full sick hue. This keeps shadows as shadows instead of washing them grey.
static void Grotto_Sicken( int base_r, int base_g, int base_b,
                           const int tint[3], int amt, int breath,
                           int& out_r, int& out_g, int& out_b )
{
	// breathing nudges the blend amount up/down a little
	int a = amt + ((breath - 128) * GROTTO_BREATH_DEPTH >> 8);
	if( a < 0 ) a = 0; if( a > 256 ) a = 256;
	int inv = 256 - a;

	// source luminance 0..255 gates how bright the tint may be for THIS pixel
	int lum = (base_r * 77 + base_g * 150 + base_b * 29) >> 8; // ~0.30/0.59/0.11
	// tint scaled down to this pixel's luminance -> never brighter than source
	int tr = tint[0] * lum >> 8;
	int tg = tint[1] * lum >> 8;
	int tb = tint[2] * lum >> 8;

	out_r = (base_r * inv + tr * a) >> 8;
	out_g = (base_g * inv + tg * a) >> 8;
	out_b = (base_b * inv + tb * a) >> 8;
	LimitToRange( out_r, 0, 255 );
	LimitToRange( out_g, 0, 255 );
	LimitToRange( out_b, 0, 255 );
}

// Per-frame hallucination update for the Mossy Grotto.
void AnimateGrottoHallucination()
{
	if( !Grotto_Ready ) Grotto_BuildSnapshot();

	i64 ticks = GetTickCountTh();
	i64 tps   = MAX_FPS;

	// Puddle shimmer: rotate the BASE puddle colours (from the clean snapshot) by
	// an offset derived from the frame clock. We do NOT read ScreenPalette here,
	// so there is no frame-to-frame feedback (which previously made the puddles
	// drift and freeze dark). indices 1..31 rotate as a ring, like the cave does.
	int puddleSpan = GROTTO_PUDDLE_LAST - GROTTO_PUDDLE_FIRST + 1; // 31
	int rot = (int)((ticks / (MAX_FPS / ENGINE_FPS)) % puddleSpan);

	// --- which pair of sickly tints, and how far between them -----------------
	i64 cycleTicks = GROTTO_TINT_CYCLE_SEC * tps; if( cycleTicks <= 0 ) cycleTicks = 1;
	i64 pos = ticks % cycleTicks; if( pos < 0 ) pos += cycleTicks;
	// position across the whole tint ring (fixed point *256)
	i64 ring = pos * GROTTO_TINT_COUNT * 256 / cycleTicks; // 0 .. COUNT*256
	int idxA = (int)(ring >> 8) % GROTTO_TINT_COUNT;
	int idxB = (idxA + 1) % GROTTO_TINT_COUNT;
	int frac = (int)(ring & 255); // 0..255 blend A->B
	int curTint[3];
	for( int c = 0; c < 3; ++c )
		curTint[c] = (GROTTO_TINTS[idxA][c] * (256 - frac) + GROTTO_TINTS[idxB][c] * frac) >> 8;

	// --- breathing pulse ------------------------------------------------------
	i64 breathTicks = GROTTO_BREATH_SEC * tps; if( breathTicks <= 0 ) breathTicks = 1;
	int breath = Grotto_Tri256( ticks % breathTicks, breathTicks ); // 0..256

	int mul = (100 - ScreenContrast) * 16 / 70 + 16;
	double gammaPercent = 0.01 * ScreenGamma;

	// --- terrain (walls, moss, dirt) -----------------------------------------
	for( int i = GROTTO_TERRAIN_FIRST; i <= GROTTO_TERRAIN_LAST; ++i ){
		int nr, ng, nb;
		Grotto_Sicken( Grotto_BasePal[i].peRed, Grotto_BasePal[i].peGreen, Grotto_BasePal[i].peBlue,
		               curTint, GROTTO_TERRAIN_STRENGTH, breath, nr, ng, nb );
		nr = min( ftol(pow(GammaKoef * nr, gammaPercent) * 256.0) * mul / 16, 255 );
		ng = min( ftol(pow(GammaKoef * ng, gammaPercent) * 256.0) * mul / 16, 255 );
		nb = min( ftol(pow(GammaKoef * nb, gammaPercent) * 256.0) * mul / 16, 255 );
		ScreenPalette[i].peRed   = (BYTE)nr;
		ScreenPalette[i].peGreen = (BYTE)ng;
		ScreenPalette[i].peBlue  = (BYTE)nb;
	}

	// --- puddles: vivid, feedback-free -----------------------------------------
	// Source each puddle from the rotated BASE snapshot (no ScreenPalette read),
	// preserve its brightness, swing hue toward the current tint, keep it vivid.
	for( int i = GROTTO_PUDDLE_FIRST; i <= GROTTO_PUDDLE_LAST; ++i ){
		// rotated source index within the puddle band
		int off = (i - GROTTO_PUDDLE_FIRST + rot) % puddleSpan;
		int srcIdx = GROTTO_PUDDLE_FIRST + off;
		int pr = Grotto_BasePal[srcIdx].peRed;
		int pg = Grotto_BasePal[srcIdx].peGreen;
		int pb = Grotto_BasePal[srcIdx].peBlue;
		int srcLum = (pr * 77 + pg * 150 + pb * 29) >> 8; if( srcLum < 1 ) srcLum = 1;

		int tintLum = (curTint[0] * 77 + curTint[1] * 150 + curTint[2] * 29) >> 8;
		if( tintLum < 1 ) tintLum = 1;
		int tr = curTint[0] * srcLum / tintLum;
		int tg = curTint[1] * srcLum / tintLum;
		int tb = curTint[2] * srcLum / tintLum;

		int a = GROTTO_PUDDLE_STRENGTH; int inv = 256 - a;
		int nr = (pr * inv + tr * a) >> 8;
		int ng = (pg * inv + tg * a) >> 8;
		int nb = (pb * inv + tb * a) >> 8;

		int newLum = (nr * 77 + ng * 150 + nb * 29) >> 8; if( newLum < 1 ) newLum = 1;
		nr = nr * srcLum / newLum;
		ng = ng * srcLum / newLum;
		nb = nb * srcLum / newLum;

		int mean = (nr + ng + nb) / 3;
		nr = mean + (nr - mean) * GROTTO_PUDDLE_SAT / 256;
		ng = mean + (ng - mean) * GROTTO_PUDDLE_SAT / 256;
		nb = mean + (nb - mean) * GROTTO_PUDDLE_SAT / 256;

		LimitToRange( nr, 0, 255 );
		LimitToRange( ng, 0, 255 );
		LimitToRange( nb, 0, 255 );
		ScreenPalette[i].peRed   = (BYTE)nr;
		ScreenPalette[i].peGreen = (BYTE)ng;
		ScreenPalette[i].peBlue  = (BYTE)nb;
	}

	UpdatePalette();
}
