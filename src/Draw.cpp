#include "stdafx.h"
#include "ItemOverlay.h"
#include "AnimOrbs.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <emmintrin.h>

#if OPTION_C_DIAG_LIGHTSURFACEBASE
// Forward declaration — definition is before DrawIsoLand.
static FILE* Diag_LogFile();
#endif

// === Option C diagnostic: LightSurfaceBase snapshot ===
// Set to 1 to enable.  Takes a snapshot of LightSurface after pre-fill and
// compares at checkpoints to find where contamination occurs.
// Also needs to be set in dx.cpp for allocation/free.
#define OPTION_C_DIAG_LIGHTSURFACEBASE 0

int         FullMask[] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

unsigned    WallTransMask[] =
{
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
	0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555
};

// ---- th2 -------------------------------------------------------------------------------
__forceinline bool SpriteInScreen(int x, int y)
{
	return (uint)y <= (uint)WorkingHeight && (uint)x <= (uint)Screen_LeftBorder + ScreenWidth;
}

// ---- th2 -------------------------------------------------------------------------------
void linecpy(void* dst, void* src, int size, unsigned mask, void* palTable)
{
	// Diagnostic: linecpy should NEVER be called when A8L8 is ON (PutTile's
	// usePPL guard should always route to linecpyPerPixel).  If this logs,
	// the guard is not working.
#if OPTION_C_DIAG_LIGHTSURFACEBASE
	if (g_A8L8Lighting && PixelLight && g_InDungeonRender) {
		FILE* f = Diag_LogFile();
		if (f) { fprintf(f, "*** BUG: linecpy called in A8L8+PPL dungeon mode! size=%d\n", size); fflush(f); }
	}
#endif
	// Approach B (A8L8): clear stale light values so the shader doesn't double-darken
	// pixels that linecpyPerPixel wrote in the same frame and then non-PPL overwrote.
	// lightOfs is a compile-time-constant-per-call delta: adding it to any valid
	// WorkingSurface pointer gives the matching LightSurface byte to zero.
	// When g_A8L8Lighting=false, LightSurface is never sampled by the shader (L8 texture);
	// skip the per-pixel zeroing entirely to restore pre-branch CPU cost.
	const bool hasLight = g_A8L8Lighting && LightSurface && SoftSrcSurfacePtr &&
		(uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
	const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;

	if( mask == 0xFFFFFFFF ){
		if( palTable == (uchar*)1 ){ //  memset / A8L8 full-dark
			if (hasLight) {
				// A8L8: preserve sprite palette index, darken via LightSurface=255.
				// The shader computes ScreenPalette[sv]*(1-255/255)=black regardless
				// of sv — matching linecpyPerPixel's lv==lmax_A8L8 branch behavior.
				// Legacy memset(dst,0) destroyed sprite content and caused pitch-black
				// rectangular tile artifacts (Issue #1).
				memcpy(dst, src, size);
				memset((uchar*)((intptr_t)dst + lightOfs), 255, size);
			} else {
				memset(dst, 0, size);
				if( hasLight ) memset((uchar*)((intptr_t)dst + lightOfs), 0, size);
			}
		}else if( palTable ){				//  memxlat
			for( int i = 0; i < size; i++ ){
				((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
			}
			if( hasLight ) memset((uchar*)((intptr_t)dst + lightOfs), 0, size);
		}else{							// memcpy
			memcpy(dst, src, size);
			if( hasLight ) memset((uchar*)((intptr_t)dst + lightOfs), 0, size);
		}
	}else{
		uchar* const ldst = hasLight ? (uchar*)((intptr_t)dst + lightOfs) : nullptr;
		if( palTable == (uchar*)1 ){ //  memmaskset
			if (hasLight) {
				// A8L8: preserve sprite palette index, darken via LightSurface=255.
				for( int i = 0; i < size; i++, mask <<= 1 ){
					if( mask & 0x80000000 ){
						((uchar*)dst)[i] = ((uchar*)src)[i];
						ldst[i] = 255;
					}
				}
			} else {
				for( int i = 0; i < size; i++, mask <<= 1 ){
					if( mask & 0x80000000 ){
						((uchar*)dst)[i] = 0;
						if(ldst) ldst[i] = 0;
					}
				}
			}
		}else if( palTable ){				//  transparent memxlat
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
					if(ldst) ldst[i] = 0;
				}
			}
		}else{							//  memmask
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)src)[i];
					if(ldst) ldst[i] = 0;
				}
			}
		}
	}
}

// (rt) optimization attempts; the effect is small. An assembler insert might help, but optimizing this on the CPU may not be worthwhile
//__forceinline void linecpy(void* dst, void* src, int size, unsigned mask, void* palTable)
//{
//	//int *d = (int*)dst, *s = (int*) src, sz = size >> 2;
//	//for( int i = 0; i < sz; ++i ){ *d++ = *s++; }
//	//sz = size - (sz << 2);
//	//if( sz >= 2 ){ *(*(short**)&d)++ = *(*(short**)&s)++; sz -= 2; }
//	//if( sz == 1 ){ *(char*)d = *(char*)s; }
//	//for( int i = 0; i < size; ++i ) ((uchar*)dst)[i] = ((uchar*)src)[i];
//	//memcpy(dst, src, size);
//	//return;
//	
//	if( mask == 0xFFFFFFFF ){
//		if( (int)palTable == 1 ){ memset(dst, 0, size); }
//		else if( palTable ){ 
//			//for( int i = 0; i < size; i++ ){ ((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]]; } // xlat
//			int *d = (int*)dst, sz = size >> 2; uchar *p = (uchar*) palTable, *s = (uchar*) src;
//			for( int i = 0; i < sz; ++i ){ *d++ = ((((((uint)p[*(s+3)] << 8) + (uint)p[*(s+2)]) << 8) + (uint)p[*(s+1)]) << 8) + p[*s]; s += 4; }
//			sz = size - (sz << 2);
//			if( sz >= 2 ){ *(*(short**)&d)++ = ((ushort)p[*(s+1)] << 8) + p[*s]; sz -= 2; s += 2; }
//			if( sz == 1 ){ *(char*)d = p[*s]; }
//		}else{ memcpy(dst, src, size); }
//	}else{
//		if( (int)palTable == 1 ){ for( int i = 0; i < size; i++, mask <<= 1 ){ if( mask & 0x80000000 ){ ((uchar*)dst)[i] = 0; } } }// maskset
//	  else if( palTable ){ for( int i = 0; i < size; i++, mask <<= 1 ){ if( mask & 0x80000000 ){ ((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]]; } } }// transparent xlat
//		else{ for( int i = 0; i < size; i++, mask <<= 1 ){ if( mask & 0x80000000 ){ ((uchar*)dst)[i] = ((uchar*)src)[i]; } } }// mask
//	}
//}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall DrawRegularCelViaTRN(int Xpos, int Ypos, const char* image, int frame, int width, const uchar* trn)
{
	if (!WorkingSurface || !image) return;
	uchar* dst = &WorkingSurface[Xpos] + YOffsetHashTable[Ypos];
	const int* img = reinterpret_cast<const int*>(image);
	int size = img[frame + 1] - img[frame];
	const char* src = image + img[frame];
	// Approach B (A8L8): clear LightSurface for every pixel written so UI sprites
	// render at full brightness rather than being darkened by dungeon tile light levels.
	const intptr_t lightOfs = LightSurface
		? (intptr_t)LightSurface - (intptr_t)WorkingSurface : 0;

	// skip header
	if (src[0] == 10 && src[1] == 0) {
		size -= 11;
		src += 11;
	}
	// decode RLE
	for (int i = 0, j = 0, k; i < size; ++i) {
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		if (j >= width) {
			dst -= WorkingWidth;
			j = 0;
		}
		if (src[i] == 127) {
			for (k = 0; k < 127; ++k) dst[j+k] = trn[(uint8_t)src[i+k+1]];
			if (lightOfs) memset((uchar*)((intptr_t)(dst+j) + lightOfs), 0, 127);
			// (th4) Stage 3 (2026-09-05): UI CEL ink drawn through TRN (monster
			// status bar border/icons, character buttons) must not inherit the
			// world's winning light color.
			TintClearRow(dst + j, 127);
			i += 127;
			j += 127;
		}
		else if (src[i] == -128) j += 128;
		else if (src[i] >= 0) {
			for (k = 0; k < src[i]; ++k) dst[j+k] = trn[(uint8_t)src[i+k+1]];
			if (lightOfs && src[i]) memset((uchar*)((intptr_t)(dst+j) + lightOfs), 0, (unsigned char)src[i]);
			// (th4) Stage 3 (2026-09-05): same as the 127-run above.
			if (src[i]) TintClearRow(dst + j, (unsigned char)src[i]);
			j += src[i];
			i += src[i];
		}
		else if (src[i] < 0) j += -src[i];
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall DrawBoxTransparent(int xPosition, int yPosition, int width, int height)
{
	// (th4) Clip to the visible screen first. The loop below indexes WorkingSurface directly with
	// (y + Screen_TopBorder) * WorkingWidth + x + Screen_LeftBorder and has no bounds check, so a
	// box that starts above the screen reads/writes before the surface and crashes. That is what
	// a tooltip taller than the screen produced: Tooltip_Draw*() clamps y with
	// LimitToRange(y, 0, ScreenHeight - height), and when height > ScreenHeight the upper bound is
	// negative and wins (crash dump 2026-09-19: 36-line set tooltip, height 793 on a 594-px-high
	// screen -> y = -199). Callers pass screen space (0..ScreenWidth, 0..ScreenHeight); clipping
	// here keeps every caller safe, and the dim-rect registered below matches what is darkened.
	if (xPosition < 0) { width += xPosition; xPosition = 0; }
	if (yPosition < 0) { height += yPosition; yPosition = 0; }
	if (xPosition + width > ScreenWidth) width = ScreenWidth - xPosition;
	if (yPosition + height > ScreenHeight) height = ScreenHeight - yPosition;
	if (width <= 0 || height <= 0) return;
	// (th4) Tell the item overlay this rectangle is about to be darkened, so true-colour
	// icons under a tooltip dim with the 8-bit ones instead of staying bright. Screen
	// space -- registered BEFORE the borders are added below.
	ItemOverlay_QueueDimRect(xPosition, yPosition, width, height, 0.35f);
	xPosition += Screen_LeftBorder;
	yPosition += Screen_TopBorder;
	size_t position = 0;
	uchar cutColors = 0;
	uchar clrVal = 0;

	struct color {
		inline static void town(const int& position) { WorkingSurface[position] = 252 + (WorkingSurface[position] >> 5); };
		inline static void dungeon(const int& position) { WorkingSurface[position] = 0 /*252 + (WorkingSurface[position] >> 3)*/; };
	};

	void (*redrawColor)(const int&) = nullptr;

	switch( Dungeon->graphType ){
	case DT_0_TOWN : cutColors = 128u; redrawColor = color::town;    break;
	case DT_4_HELL : cutColors =  16u; redrawColor = color::dungeon; break;
	case DT_3_CAVE
	  or DT_5_CRYPT
	  or DT_6_ABYSS: cutColors =  32u; redrawColor = color::dungeon; break;
	}

	for (int i = 0, j; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			position = WorkingWidth * (yPosition + i) + xPosition + j;
			clrVal = WorkingSurface[position];

			if (clrVal) {
				if (clrVal < cutColors)
					redrawColor(position);
				else if ((clrVal >= 128 && clrVal <= 135) || (clrVal >= 144 && clrVal <= 151) || (clrVal >= 136 && clrVal <= 143) || (clrVal > 152 && clrVal < 159))
					WorkingSurface[position] = (clrVal & 0b11111000) + ((clrVal & 0b00000111) >> 2) + 6;
				else
					WorkingSurface[position] = (clrVal & 0b11110000) + ((clrVal & 0b00001111) >> 2) + 12;
				// Approach B (A8L8): WorkingSurface was just rewritten — clear LightSurface
				// so the new color renders at full brightness (no double-darkening).
				if (LightSurface) LightSurface[position] = 0;
				// (th4) Stage 2: same for the alpha overlay under the dimmed box pixel.
				AlphaOverlayClearPixel(&WorkingSurface[position]);
				// (th4) Stage 3 (2026-09-05): same for the colored-light tint — the
				// dimmed box pixel must not be multiplied by the world's light color.
				TintClearPixel(&WorkingSurface[position]);
			}
		}
	}
	// (th4) Box darkened, text not written yet. Copy it so the overlay can lift that
	// text back out from under the icons.
	ItemOverlay_SnapshotDimRect();
}


// ---- th2 -------------------------------------------------------------------------------
void DrawMonsterStatusBar(int monsterID)
{
	const Monster& monster = Monsters[monsterID];
	if( monster.goal == MG_0_DEATH || monster.BaseLife <= 0 ) return;

	int x = (ScreenWidth) / 2 + Screen_LeftBorder;
	int y = Screen_TopBorder + 3;
	auto MB_DrawImg = [&x, &y](int posX, int posY, int width, int height, char* image, int frameIndex = 0) {
		Surface_DrawCEL(x + posX, y + posY + height, image, frameIndex + 1, width);
	};
	auto MB_DrawImgViaTRN = [&x, &y](int posX, int posY, int width, int height, char* image, const uchar (&trn)[256], int frameIndex = 0) {
		DrawRegularCelViaTRN(x + posX, y + posY + height, image, frameIndex + 1, width, trn);
	};
	auto MB_DrawIconViaTRN = [&x, &y](int posX, int posY, int width, int height, char* image, int frameIndex) {
		DrawRegularCelViaTRN(x + posX, y + posY + height, image, frameIndex + 1, width, MonsterBarIconColors[frameIndex]);
	};
	auto MB_DrawHP = [&x, &y](int posX, int posY, int width, int height, uint8_t color = 199) {
		int halfheight = height >> 1;
		int gradpxl = halfheight >> 2;
		for (int i = 0; i < height; ++i) {
			int rowOfs = YOffsetHashTable[posY + y + i] + x + posX;
			memset( &WorkingSurface[rowOfs],
			        color + abs((i - halfheight) * (i - halfheight) / halfheight / gradpxl),
			        width );
			// Approach B (A8L8): clear LightSurface so the HP bar renders at full brightness.
			if (LightSurface) memset( LightSurface + rowOfs, 0, width );
			// (th4) Stage 2: same for the alpha overlay under the HP bar row.
			AlphaOverlayClearRow(&WorkingSurface[rowOfs], width);
			// (th4) Stage 3 (2026-09-05): same for the colored-light tint — the
			// monster status bar is opaque UI ink, no world light color on it.
			TintClearRow(&WorkingSurface[rowOfs], width);
		}
	};
	auto MB_DrawText = [&x, &y](int posX, int posY, const char* text, int color = C_0_White) {
		DrawLevelInfoText(x - Screen_LeftBorder + posX, posY, text, color);
	};

	int specialMonster = 0;
	if (monster.ChampionNumber > 0) specialMonster = 1;
	if (monster.newBossId > 0) specialMonster = 2;

	// LifeScale panel
	uint32_t lifeScale = ((uint64_t)monster.CurrentLife << 8) / monster.BaseLife;
	if (lifeScale > 256u) lifeScale = 256u;
	uint32_t acidScale = ((uint64_t)monster.acidDot << 8) / monster.BaseLife;
	if (acidScale > lifeScale) acidScale = lifeScale;

	// Draw border
	if (AlternativeMonsterBarBorder)
		MB_DrawImgViaTRN(-177, 0, 354, 40, Monster_Bar_Border, MonsterBarBorderColors[2 - specialMonster], 2 - (monster.newBossId > 0));
	else
		MB_DrawImg(-177, 0, 354, 40, Monster_Bar_Border, 3);
	
	// (th4) DARK CAMPAIGN HUB: kin are NPCs, not enemies -- show only the name plate. No HP
	// scale, no HP numbers, no type/immunity icons, no aura line. The border and name are
	// still drawn so hovering a vendor still identifies him.
	const bool hubKinBar = Dungeon == DUN_117_DARKLORD_HIDEOUT;

	// Draw HP scale
	constexpr int hpPosX = -128;
	constexpr int hpPosY = 6;
	if( !hubKinBar ){
		MB_DrawHP(hpPosX, hpPosY, lifeScale, 31, 234); // draw LifeScale
		MB_DrawHP(hpPosX + lifeScale - acidScale, hpPosY, acidScale, 31, 202); // draw AcidScale
		DrawBoxTransparent(x - Screen_LeftBorder + hpPosX + lifeScale, y - Screen_TopBorder + hpPosY, 256 - lifeScale, 31); // draw BackGround

		MB_DrawImg(-25, 30, 50, 50, Monster_Bar_MonIcons, monster.BasePtr->MonsterClass); // draw monster type icon - undead, deamon, beast
	}

	// Immunes states (icons)
	constexpr uint16_t immunes[] = { MR_5_IM_FIRE, MR_6_IM_LIGHT, MR_11_IM_COLD, MR_4_IM_ARCAN, MR_8_IM_ACID, MR_7_IM_CURSE,  };
	constexpr int16_t immuIconSlotPosition[] = { -(20 + 32), -(20 + 32 * 2), -(20 + 32 * 3), 20, 20 + 32, 20 + 32 * 2 };
	constexpr uint8_t regroup[countof(immunes)] = { 3, 1, 0, 5, 4, 2 };
	uint8_t iconSlotsLeft = 0;
	uint8_t iconSlotsRight = 3;
	if( !hubKinBar ){
	for (size_t i = 0; i < 3; ++i) {
		if (!(monster.ResistImmune & immunes[regroup[i]])) {
			MB_DrawIconViaTRN(immuIconSlotPosition[iconSlotsLeft], 38, 32, 32, Monster_Bar_ImIcons, regroup[i]); // draw icons
			++iconSlotsLeft;
		}
		if (!(monster.ResistImmune & immunes[regroup[i+3]])) {
			MB_DrawIconViaTRN(immuIconSlotPosition[iconSlotsRight], 38, 32, 32, Monster_Bar_ImIcons, regroup[i+3]); // draw icons
			++iconSlotsRight;
		}
	}
	}

	char textBuff[128];
	char textInfo[256];

	// Monster name + status
	constexpr uint8_t specialTable[] = { C_0_White, C_1_Blue, C_3_Gold, C_2_Red };
	sprintf_s(textInfo, "%s%s%s", monster.Name, monster.hisBossIndex ? L(" (minion)") : "",
		monster.ChampionNumber > 0 ? L(ChampionsNameTable[monster.ChampionNumber - 1] + 2) : "");
	MB_DrawText(-GetTextWidth(textInfo) / 2, 23, textInfo, specialTable[specialMonster]);

	// HP (text)
	if( !hubKinBar ){
		sprintf_s(textBuff, "%d", monster.CurrentLife >> 6);
		int xShift = GetTextWidth(textBuff) + 3;
		sprintf_s(textInfo, "%s/%d", textBuff, monster.BaseLife >> 6);
		MB_DrawText(-xShift, 36, textInfo, monster.CurrentLife > monster.BaseLife ? C_2_Red : C_0_White);
	}

	// Monster stun threshold
	if (ShowMonsterStunThreshold && !hubKinBar) {
		int thresholdRangeMin, thresholdRangeMax;
		tie(thresholdRangeMin, thresholdRangeMax) = GetMonsterStunThresholdRange(monster);
		ScaleMonsterStunThreshold(thresholdRangeMin);
		ScaleMonsterStunThreshold(thresholdRangeMax);
		int thresholdLife = GetMonsterLifeStunThreshold(monster);
		LimitToMax(thresholdRangeMin, thresholdLife);
		LimitToMax(thresholdRangeMax, thresholdLife);

		constexpr auto thresholdColor = C_10_Enchanted;
		char* format = thresholdRangeMin == thresholdRangeMax ? "%d" : "%d-%d";
		sprintf_s(textBuff, format, thresholdRangeMin, thresholdRangeMax);
		int textWidth = GetTextWidth(textBuff);
		MB_DrawText(-125, 36, textBuff, thresholdColor);

		sprintf_s(textBuff, "ST");
		textWidth = GetTextWidth(textBuff);
		MB_DrawText(-125, 23, textBuff, thresholdColor);
	}


	uint16_t additionalTextRows = 0;
	static const uint16_t textRowHeight[] = { 0 , 14, 28, 42, 56, 70, 84, 98 };
	// TODO: change all stack "constexpr" array to static const
	// there is no such thing as constexpr array in real time function in C++

	// Monster affixes
	static const char* monsterAffixes[] = { "None", "Strong", "Accurate", "Tough", "Dodging", "Armored", "Resistant" };
	int affixesAmount = 0;
	sprintf_s(textInfo, "extra: ");
	for (auto&& affix : monster.affixes)
		if (affix) {
			sprintf_s(textBuff, "%s%s", affixesAmount ? ", " : "", monsterAffixes[affix]);
			strcat(textInfo, textBuff);
			++affixesAmount;
		}
	if (affixesAmount > 0 && !hubKinBar) {
		int textWidth = GetTextWidth(textInfo) / 2;
		DrawBoxTransparent(x - Screen_LeftBorder - textWidth, y - Screen_TopBorder + 76 + textRowHeight[additionalTextRows], textWidth * 2, 12); // draw BackGround
		MB_DrawText(-textWidth, 90 + textRowHeight[additionalTextRows], textInfo, C_2_Red);
		++additionalTextRows;
	}

	// Monster auras
	const int& currentMonsterBaseIndex = monster.SpritePtr->baseMonsterIndex;
	bool breakflag = hubKinBar;	// (th4) hub kin show no aura line
	for (ushort auraIndex = AUI_1_; auraIndex < AUI_COUNT && !breakflag; ++auraIndex)
		for (auto&& auraBaseMonsterIndex : Auras[auraIndex].baseMonsterIndex) {
			if (currentMonsterBaseIndex == auraBaseMonsterIndex) {
				sprintf_s(textInfo, "%s", L(Auras[auraIndex].auraname));	// (th4) localized aura name
				int textWidth = GetTextWidth(textInfo) / 2;
				DrawBoxTransparent(x - Screen_LeftBorder - textWidth, y - Screen_TopBorder + 76 + textRowHeight[additionalTextRows], textWidth*2, 12); // draw BackGround
				MB_DrawText(-textWidth, 90 + textRowHeight[additionalTextRows], textInfo, C_10_Enchanted);
				++additionalTextRows;
				breakflag = true;
				break;
			}
		}
}

// (th4) --- Display-only ranged hit-chance for the ALT readout -------------------------
// The battle path for a monster hitting the player is split: melee -> MvP_Melee (which
// applies the FCA crowd-bonus via AmplifyByNearAttackers), ranged arrow/spell -> MvP_Ranged
// (distance-scaled, NO FCA). The cursor readout must pick the right one per monster, or a
// skeleton archer shows a melee estimate that also wrongly grows when OTHER monsters crowd
// the player. We classify by the monster's AI (the same representative missile each AI fires)
// and replicate ONLY the to-hit portion of MvP_Ranged (no block/reflect/damage/side effects),
// reusing the real Limit_MvP_Ranged_* clamps so the display stays in sync with battle tuning.

// Representative missile the given AI fires, or -1 if the AI is melee-only.
// Mirrors the RangerRetreatsOften / Universal_RangeSecondAttack missile args in Monster.cpp.
// For AIs whose element varies (Ice Age / dungeon), the base (non-variant) missile is used;
// arrow-vs-spell classification is unaffected by those element swaps.
static int MonsterDisplayMissile( const Monster& m )
{
	switch( m.ai ){
	// --- physical arrows (EffectType MT_0_ARROW) ---
	case AI_3_Skel_Archer:
	case AI_7_Goat_Archer:            return MI_0_NONE_ARROW;
	case AI_33_Ranger_Lite_Arrow:     return MI_56_LIGHTING_ARROW;
	case AI_37_Ranger_Fire_Arrow:     return MI_27_FIRE_ARROW;
	case AI_43_Ranger_Acid_Arrow:     return MI_109_ACID_ARROW;
	case AI_44_Ranger_Cold_Arrow:     return MI_126_COLD_ARROW;
	case AI_45_Ranger_Arcane_Arrow:   return MI_108_ARCANE_ARROW;
	// --- spells / bolts / blasts (EffectType != MT_0_ARROW) ---
	case AI_14_Witch:                 return MI_24_ARCANE_STAR;
	case AI_20_Acid_Beast_Ranger:
	case AI_55_Wyrm:                  return MI_57_ACID_MISSILE;
	case AI_32_Ranger_Firebolt:       return MI_1_FIREBOLT;
	case AI_35_Lich:                  return MI_98_LICH_ORA_BLAST;
	case AI_36_Arch_Lich:             return MI_101_YELOW_BLAST;
	case AI_38_Beholder:
	case AI_46_Ranger_Blue_Star:      return MI_131_ARCANE_STAR_BLUE;
	case AI_41_Ranger_snowbolt:       return MI_129_SNOW_BOLT;
	case AI_49_Ranger_Fireblast:      return MI_6_FIREBLAST;
	// --- HYBRIDS: melee primary, ranged SECOND attack -> reported as MELEE (th4) ---
	// These used to return their missile, so the tooltip showed a magic chance-to-get-hit
	// and, once the block line existed, no block chance at all -- while the monster was
	// standing next to you clawing. You spend most of a hybrid fight in melee range, so
	// they now report the melee threat and a melee block chance.
	//
	// THE LIST IS NOT HAND-PICKED: a hybrid is exactly an AI whose handler calls
	// Universal_RangeSecondAttack (Monster.cpp ~11368) -- melee primary with a ranged
	// second attack. Re-derive it from that call if AIs are ever added, do not guess from
	// the enum names: AI_19_Acid_Beast is a hybrid while AI_20_Acid_Beast_Ranger is pure
	// ranged, and AI_9_Magma / AI_27_Diablo read as plain melee but do have ranged second
	// attacks. AI_12_Gargoyle does NOT -- it is pure melee and is deliberately absent.
	// When re-deriving, match each handler body by BRACE COUNTING, not "up to the next
	// Ai_ function": the handlers are NOT in numeric order in Monster.cpp, and slicing
	// that way makes Ai_12_Gargoyle swallow Ai_9_Magma's call and look like a hybrid.
	//
	// Listed explicitly rather than left to `default` so the decision is greppable, and
	// because AI_40 was MISSING from this function entirely -- it fell through to melee by
	// accident, which happened to be right for the threat line but for the wrong reason.
	case AI_9_Magma:
	case AI_16_Thunder_Demon:
	case AI_19_Acid_Beast:
	case AI_27_Diablo:
	case AI_39_Mixed_White_Star_phys:
	case AI_40_Bonewing_cold_blasts:
	case AI_42_Mixed_Firebolt:
	case AI_47_Mixed_Beholder_Star:
	case AI_48_Mixed_Yellow_Star:
	case AI_50_Mixed_Fireblast:
	case AI_51_Mixed_Arcane_Star:
	case AI_52_Mixed_Snowbolt:
	case AI_54_Mixed_Chargedbolt:
	case AI_57_Hybrid_Arc_Star:
	case AI_58_Mixed_Psychic_Bite:   return -1; // melee: threat AND block both use MvP_Melee
	// Reporting hybrids as melee also sidesteps a mismatch this table could never win:
	// AI_42/47/51 swap their missile at runtime under the Ice Age game changer (and AI_40
	// picks one of three at random in DUN_52_MOSSY_GROTTO), so any hardcoded missile here
	// would have been computing the threat against the wrong element's resistance.
	default:                          return -1; // melee-only
	}
}

// Display replica of MvP_Ranged's to-hit (monster branch), averaged over its RNG.
// Returns 0..100, or -1 if not applicable. No FCA, no block, no side effects.
static int CalcMvP_RangedHitChance_Display( int monsterIndex, int playerIndex, int distance, int missileIndex )
{
	if( (uint)monsterIndex >= Monsters_Max_Count || (uint)playerIndex >= PlayersMax_4 ) return -1;
	if( missileIndex < 0 ) return -1;
	Player&  player  = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( !monster.SpritePtr || !monster.BasePtr ) return -1;
	BaseMissile& bm = BaseMissiles[missileIndex];

	const int SEED_SAVE = RandomSeed;
	SetRndSeed( (int)( monsterIndex * 2654435761u + (uint)Dungeon->level + 0x9E37u ) ); // distinct stream from myHit
	const int SAMPLES = 64;
	int sum = 0;
	for( int s = 0; s < SAMPLES; ++s ){
		int realToHit;
		if( bm.EffectType != MT_0_ARROW ){ // spell branch (Missile.cpp MvP_Ranged ~2444)
			int playerAC = 0;
			Player_AC( playerIndex, playerAC );
			realToHit = 95 - (2 * distance) - (playerAC / 50) - PerkValue(PERK_SPELL_DEFLECTION, playerIndex);
			Limit_MvP_Ranged_Magical_ToHit_Chance( playerIndex, realToHit );
		}else{ // arrow branch (Missile.cpp MvP_Ranged ~2460)
			int playerAC = 0;
			Player_AC( playerIndex, playerAC );
			int mc = monster.BasePtr->MonsterClass;
			playerAC += (playerAC * player.spiciesAcPercent[mc]) / 100;
			playerAC += player.spiciesAc[mc];
			int enemyToHit = monster.ToHit + 2 * (monster.MonsterLevel - player.CharLevel);
			if( GameMode == GM_CLASSIC ){
				enemyToHit += 20 + RNG(21);
			}else{
				enemyToHit += by( Difficulty, 30 - 10 + RNG(21),
											-  55 + RNG(116),
											- 155 + RNG(221),
											- 255 + RNG(326),
											- 355 + RNG(431),
											- 455 + RNG(536),
											- 555 + RNG(641),
											- 655 + RNG(746),
											- 755 + RNG(851) );
			}
			enemyToHit -= 2 * distance;
			realToHit = enemyToHit - playerAC;
			int Autohit = 14, Automiss = 95;
			Limit_MvP_Ranged_Arrow_ToHit_Chance( playerIndex, realToHit, Autohit, Automiss );
		}
		LimitToRange( realToHit, 0, 100 );
		sum += realToHit;
	}
	SetRndSeed( SEED_SAVE );
	int percent = ( sum + SAMPLES/2 ) / SAMPLES;
	// (th4) LITHE BUILD: after the hit roll passes, Missile.cpp MvP_Ranged (~2640) rolls
	// RNG(100) > 91 and cancels 8 of 100 hits -- arrows and spells alike. The melee replica
	// (Monster.cpp CalcMvP_MeleeHitChance) applies the same factor for Monster.cpp ~8165.
	if( HasTrait(playerIndex, TraitId::LitheBuild) ) percent = percent * 92 / 100;
	return percent;
}

// (th4) Where the current player's MI_79_REFLECT icon was actually drawn this frame, so
// the charge count can be put on it. SURFACE coordinates -- i.e. borders already
// included, exactly what CL2_Draw takes.
//
// The count must NOT be derived from Xofs/Yofs. Those are the AUTOMAP origin
// (declared in automap.cpp, computed in dx.cpp ~197) and only approximate the player's
// position: they carry a quadratic fudge `- int(w*w + w)`, a `w/h` ratio that is not
// scale-invariant because GUI_MainPanelHeight is subtracted from the height but nothing
// equivalent from the width, and a `HeightShift * 32` term quantised to whole tiles.
// The error therefore changes with RESOLUTION: at 1056x594 it is near zero and the old
// hardcoded offset landed on the icon, at 1280x720 it is not and the count floated off.
// The icon is placed by the tile loop instead, so the only honest anchor is where the
// icon itself ended up -- which is what these hold. Same principle as the overhead HP
// bar in DrawPlayer, which uses rowOffset/colOffset and is correct at every resolution.
// -1 = not drawn this frame.
int g_ReflectIconSurfX = -1;
int g_ReflectIconSurfY = -1;
int g_CurPlayerSurfX   = -1;	// (th4) player sprite centre, the HP bar's own anchor
int g_CurPlayerSurfY   = -1;	// (th4) the hero's body (half his sprite height above the feet): the DYNAMIC ZOOM centre
// (th4) Explicit "were they latched this frame" flags. A coordinate sentinel would not
// do: g_ReflectIconSurfY is the icon's bottom minus 132, which is legitimately NEGATIVE
// when the icon is near the top of the screen, and the count would silently vanish.
bool g_ReflectIconDrawn = false;
bool g_CurPlayerDrawn   = false;

// (th4) Value color for the ALT hit-chance readout (accuracy):
//   0..24 red, 25..49 pink, 50..74 banana, 75+ blue.
static int HitChanceColor( int v )
{
	if( v >= 75 ) return C_1_Blue;
	if( v >= 50 ) return C_6_Banana;
	if( v >= 25 ) return C_8_Pink;
	return C_2_Red;
}

// (th4) Value color for the ALT block-chance readout:
//   0..14 red, 15..29 pink, 30..44 banana, 45+ blue.
// Deliberately NOT HitChanceColor. Hit chance runs the full 0..100, but block chance is
// hard-capped per class by MvP_Melee_Max_Block_Chance -- 10 for Executioner, 20 for
// Guardian/Savage/Berserker, 25 for Assassin -- so the 75/50/25 ramp would leave those
// classes stuck on red even at their ceiling. These thresholds are Ibragim's.
static int BlockChanceColor( int v )
{
	if( v >= 45 ) return C_1_Blue;
	if( v >= 30 ) return C_6_Banana;
	if( v >= 15 ) return C_8_Pink;
	return C_2_Red;
}

// (th4) Inverted color for chance-to-BE-hit (high is bad for the player):
//   0..24 blue, 25..49 banana, 50..74 pink, 75+ red.
static int ThreatChanceColor( int v )
{
	if( v >= 75 ) return C_2_Red;
	if( v >= 50 ) return C_8_Pink;
	if( v >= 25 ) return C_6_Banana;
	return C_1_Blue;
}

// (th4) Chance to BLOCK the hovered monster's attack, for the ALT readout. Display only.
// Returns 0..100, or -1 when the line must not be shown at all.
//
// This is a REPLICA of two different engine paths, not one shared formula -- melee and
// ranged blocking disagree on the value, the cap AND on who is allowed to block:
//   melee : MvP_Melee        (Monster.cpp ~8017) -- gate is just player.CanBlock
//   ranged: MvP_Ranged       (Missile.cpp ~2589) -- CanBlock PLUS class/2H disqualifiers
// Both are RNG-FREE, unlike the two hit-chance lines above, so this needs no sampling
// and no seed save/restore.
//
// ELEMENTAL ARROWS. Since v0.125 the block branch reads
//     blockRnd < blockChance && ((MT_0_ARROW && ET_0_PHYSICAL) || resist% + RNG(41) < 20)
// so a fire/cold/lightning/acid/arcane arrow is blocked only when the SECOND clause also
// passes -- i.e. only while your resist to that element is under 20%, with probability
// (20 - resist%)/41. We therefore report the EFFECTIVE chance: the raw block chance
// scaled by that factor. Physical arrows scale by 1.0 and melee skips the factor, so all
// three cases end up on one number the player can trust.
// NOTE the comment above that branch in Missile.cpp still describes the PRE-v0.125
// behaviour ("always can block arrows, including elemental") and is stale -- do not use
// it as the spec, read the condition.
//
// Two things the number cannot express, both matching what the character sheet already
// shows: it is CONDITIONAL on the attack landing (the engine rolls block only after
// to-hit succeeds), and it assumes you are standing or attacking, since both paths force
// no-block while walking or casting.
static int CalcPlayerBlockChance_Display( int monsterIndex, int playerIndex, int displayMissile )
{
	// (th4) NO game-mode gate here on purpose. This line appears under exactly the same
	// conditions as the two hit-chance lines above it -- the caller's own exclusions
	// (town, DUN_117 hub, ALT not held, mage classes, own summons, friendly monsters,
	// dead monsters) are the whole list. Earlier drafts gated this on classic/coliseum;
	// that was wrong twice over: blocking works in BOTH (classic even has its own caps --
	// archers 80, mages 70, where outside classic they are 0), and coliseum still has
	// monsters, so suppressing only this line there made the tooltip inconsistent with
	// itself. The tooltip never fires on other PLAYERS in any mode: hovering a player
	// sets Cur.playerIndex and leaves Cur.MonsterID at -1, which the caller rejects.
	const Player&  player  = Players[ playerIndex ];
	const Monster& monster = Monsters[ monsterIndex ];
	const int rose = HasTrait( playerIndex, TraitId::ARoseWithThorns ) ? (player.CharLevel / 3) : 0;
	const int aggr = PerkValue( SYNERGY_AGGRESSION, playerIndex, 0 );

	if( displayMissile < 0 ){
		// ---- melee, replica of MvP_Melee ----
		if( !player.CanBlock ) return -1;
		int blockChance = 0;
		MvP_Melee_Block_Chance( monsterIndex, playerIndex, blockChance );   // already >= 0, Scissor zeroes it
		int maxBlockChance = 0;
		MvP_Melee_Max_Block_Chance( playerIndex, maxBlockChance );
		const int twoTowers = HasTrait( playerIndex, TraitId::TwoTowers ) ? (player.CharLevel / 6) : 0;
		const int cap = maxBlockChance - rose + twoTowers - aggr;           // same cap MvP_Melee applies
		if( cap <= 0 ) return -1;                                          // cannot block at all: no line
		LimitToMax( blockChance, cap );
		LimitToMin( blockChance, 0 );
		return blockChance;
	}

	// ---- ranged, replica of MvP_Ranged ----
	const BaseMissile& bm = BaseMissiles[ displayMissile ];
	if( bm.EffectType != MT_0_ARROW ) return -1;   // spells are a resist roll, not a block

	// canBlock, copied condition-for-condition from MvP_Ranged -- these disqualifiers do
	// NOT exist on the melee path, so the two branches genuinely differ.
	bool canBlock = player.CanBlock;
	if( const Item* b = player.OnBodySlots; player.ClassID == PC_5_SAVAGE
	 && fold( || (not( x.ItemCode, IC_M1_NONE, IC_3_BOW ) && x.EquippedLocation == EL_2_TWO_HANDED), b[IS_LeftHand], b[IS_RightHand] ) ){
		canBlock = false;
	}
	const Item& lh = player.OnBodySlots[ IS_LeftHand ];
	const Item& rh = player.OnBodySlots[ IS_RightHand ];
	const bool checkFor2H = ( rh.TypeID == ITEM_1_WEAPON && rh.EquippedLocation == EL_2_TWO_HANDED )
	                     || ( lh.TypeID == ITEM_1_WEAPON && lh.EquippedLocation == EL_2_TWO_HANDED );
	if( player.fullClassId == PFC_GUARDIAN || player.fullClassId == PFC_KENSEI ){
		if( checkFor2H && ( rh.ItemCode == IC_1_SWORD || lh.ItemCode == IC_1_SWORD
		                 || rh.ItemCode == IC_4_MACE  || lh.ItemCode == IC_4_MACE ) ) canBlock = false;
	}
	if( player.fullClassId == PFC_SHUGOKI ){
		if( checkFor2H && ( rh.ItemCode == IC_2_AXE || lh.ItemCode == IC_2_AXE ) ) canBlock = false;
	}
	if( player.fullClassId == PFC_DIMACHAERUS ) canBlock = false;
	if( HasTrait( playerIndex, TraitId::Scissor ) ) canBlock = false;
	if( !canBlock ) return -1;

	int blockChance = player.BaseDexterity + player.blockChance - rose - aggr;	// (th4) class block bonus retired
	if( player.fullClassId == PFC_MURMILLO ) blockChance += player.CharLevel;
	blockChance -= monster.ArmorClass / 2;         // the M1_TRAP branch cannot apply to a hovered monster
	int maxBlockChanceRanged = 0;
	MvP_Ranged_Max_Block_Chance( playerIndex, maxBlockChanceRanged );
	const int cap = maxBlockChanceRanged - rose - aggr;
	if( cap <= 0 ) return -1;
	LimitToRange( blockChance, 0, cap );

	if( bm.DamageType != ET_0_PHYSICAL ){
		// Elemental arrow: scale by P(resist% + RNG(41) < 20). RNG(41) yields 0..40, so the
		// number of winning rolls is (20 - resist%) clamped into [0,41].
		int winning = 20 - (int)( player.resistPercent[ bm.DamageType ] * 100 );
		LimitToRange( winning, 0, 41 );
		blockChance = ( blockChance * winning + 20 ) / 41;   // rounded
	}
	return blockChance;
}

// ---- th4 -------------------------------------------------------------------------------
// Fallout-style hit-chance readout that floats next to the mouse cursor while ALT is held.
// Shows my chance to hit the hovered monster (current weapon) and its chance to hit me.
// Excluded: caster classes (all mages), my own summons, and friendly monsters
// (which includes other players' minions). Display only - no gameplay effect.
void DrawCursorHitChanceInfo()
{
	int monsterID = Cur.MonsterID;
	if( monsterID < 0 || monsterID >= Monsters_Max_Count ) return;
	if( !( Dungeon->genType || Dungeon->isQuest ) ) return; // dungeon only, not town
	if( Dungeon == DUN_117_DARKLORD_HIDEOUT ) return; // (th4) hub kin are NPCs -- nothing to fight, no hit chances
	if( !(IsAltPressed || AltHighlight) ) return;

	const Player& player = Players[ CurrentPlayerIndex ];
	if( player.ClassID == PC_2_MAGE ) return; // no readout for caster classes

	if( IsPlayerSummon(monsterID) ) return;   // my own minion
	const Monster& monster = Monsters[ monsterID ];
	if( !monster.SpritePtr || (monster.flag & MF_6_FRIENDLY) ) return; // friendly / other players' minions
	if( monster.goal == MG_0_DEATH || monster.BaseLife <= 0 ) return;

	// My chance to hit it (current weapon: ranged if bow/gun/knife/mallet in hand, else melee).
	// CalcPvM_*HitChance roll live RNG (AC jitter) on every call, so calling once per frame
	// makes the number flicker. Instead we AVERAGE many samples to get the expected value.
	// The samples advance the shared RandomSeed, so we snapshot it, deterministically re-seed
	// for a frame-stable result, then RESTORE it afterwards -> net-zero RNG consumption and
	// the battle hit-chance functions stay byte-for-byte untouched.
	int myHit;
	{
		const int SEED_SAVE = RandomSeed;
		SetRndSeed( (int)( monsterID * 2654435761u + (uint)Dungeon->level ) ); // stable per hovered monster & level
		const int SAMPLES = 64;
		int sum = 0, negFirst = 0;
		bool firstDone = false;
		for( int s = 0; s < SAMPLES; ++s ){
			int h;
			if( player.IsBowInHand ){
				int distance = CalculateArrowDistance( CurrentPlayerIndex, monsterID );
				h = Calc_PvM_Ranged_Hit_Chance( CurrentPlayerIndex, monsterID, distance, MI_0_NONE_ARROW );
			}else{
				h = CalcPvM_MeleeHitChance( CurrentPlayerIndex, monsterID, false );
			}
			if( !firstDone ){ negFirst = h; firstDone = true; } // immune/sentinel is stable across samples
			if( h < 0 ){ sum = -1; break; }                     // negative sentinel -> "Can't hit"
			sum += h;
		}
		myHit = ( sum < 0 ) ? negFirst : ( sum + SAMPLES/2 ) / SAMPLES; // rounded average
		SetRndSeed( SEED_SAVE ); // restore: gameplay RNG stream is unaffected
	}
	// Its chance to hit me. Ranged/hybrid monsters (skeleton archers, casters, spitters)
	// threaten via MvP_Ranged (distance-scaled, no FCA) -- using the melee calc for them
	// shows a wrong static value that also inflates when other monsters crowd the player.
	// Pick the ranged replica when the monster's AI fires a missile, else melee.
	const int rangedMissile = MonsterDisplayMissile( monster );   // (th4) shared with the block line below
	int itsHit;
	{
		if( rangedMissile >= 0 ){
			int rDist = CalculateArrowDistance( CurrentPlayerIndex, monsterID );
			itsHit = CalcMvP_RangedHitChance_Display( monsterID, CurrentPlayerIndex, rDist, rangedMissile );
		}else{
			itsHit = CalcMvP_MeleeHitChance( monsterID, CurrentPlayerIndex ); // already an expected value, no RNG
		}
	}

	// (th4) Chance to block its attack. -1 = do not show the line (this character cannot
	// block at all, or the monster attacks with a spell rather than melee/arrow).
	const int itsBlock = CalcPlayerBlockChance_Display( monsterID, CurrentPlayerIndex, rangedMissile );
	const bool showBlock = ( itsBlock >= 0 );

	// Split each line into a white LABEL and a color-coded VALUE.
	// Line A = accuracy (HitChanceColor). Line B = chance to be hit, inverted
	// (ThreatChanceColor). A "Can't hit" -> red; B "Can't hit" (monster can't reach
	// you, good) -> blue.
	char labelA[64], valA[16];
	char labelB[64], valB[16];
	char labelC[64], valC[16];
	sprintf_s( labelA, "%s: ", L("Chance to hit") );
	sprintf_s( labelB, "%s: ", L("Chance to get hit") );
	sprintf_s( labelC, "%s: ", L("Chance to block") );

	// Line A (my accuracy) and line B (chance to be hit) are color-coded, B inverted.
	int colA, colB;
	if( myHit < 0 ){ sprintf_s( valA, "%s", L("Can't hit") ); colA = C_2_Red; }
	else{ sprintf_s( valA, "%i%%", myHit ); colA = HitChanceColor( myHit ); }
	if( itsHit < 0 ){ sprintf_s( valB, "%s", L("Can't hit") ); colB = C_1_Blue; }
	else{ sprintf_s( valB, "%i%%", itsHit ); colB = ThreatChanceColor( itsHit ); }
	int colC = C_0_White;
	if( showBlock ){ sprintf_s( valC, "%i%%", itsBlock ); colC = BlockChanceColor( itsBlock ); }   // high block is GOOD
	else valC[0] = 0;

	// Position just below-right of the cursor (CursorX/Y are play-area relative; the draw
	// helpers add Screen_Left/TopBorder internally). Clamp so the box stays on screen.
	const int lineH = 13;
	const int lines = showBlock ? 3 : 2;                 // (th4) block line is optional
	int wA = GetTextWidth(labelA) + GetTextWidth(valA);
	int wB = GetTextWidth(labelB) + GetTextWidth(valB);
	int wC = showBlock ? GetTextWidth(labelC) + GetTextWidth(valC) : 0;
	int boxW = (wA > wB ? wA : wB);
	if( wC > boxW ) boxW = wC;
	boxW += 8;
	int boxH = lineH * lines + 4;
	int px = CursorX + 20;
	int py = CursorY + 4;
	LimitToMax( px, ScreenWidth  - boxW - 1 );
	LimitToMax( py, ScreenHeight - boxH - 1 );
	LimitToMin( px, 0 );
	LimitToMin( py, 0 );

	DrawBoxTransparent( px, py, boxW, boxH );
	int labWA = DrawLevelInfoText( px + 4, py + 2 + lineH,     labelA, C_0_White );
	DrawLevelInfoText( px + 4 + labWA, py + 2 + lineH,     valA, colA );
	int labWB = DrawLevelInfoText( px + 4, py + 2 + lineH * 2, labelB, C_0_White );
	DrawLevelInfoText( px + 4 + labWB, py + 2 + lineH * 2, valB, colB );
	if( showBlock ){
		int labWC = DrawLevelInfoText( px + 4, py + 2 + lineH * 3, labelC, C_0_White );
		DrawLevelInfoText( px + 4 + labWC, py + 2 + lineH * 3, valC, colC );
	}
}

// Precomputed pointers to each LightTable row, indexed by light level [0..LightMax].
// LightMax is at most 15 (standard mode) or 3 (Light4Levels mode). The full LightTable
// has 27 rows (light levels 0-15, R_RED at 16, boss TRNs at 19-26), but PPL code only
// ever indexes with values in [0..LightMax] — clamped by CalculatePerPixelLight(MT).
// Pointers are stable for the process lifetime (LightTable is a fixed-size global;
// its CONTENT changes at level load, but its ADDRESS and row stride never change,
// so these pointers never need re-initializing after content changes).
static const uchar* s_LightTablePtrs[16]; // rows 0..15 — covers full LightMax range

static void InitLightTablePtrs()
{
	for( int i = 0; i < 16; i++ )
		s_LightTablePtrs[i] = &LightTable[256 * i];
}

// ---- th2 -------------------------------------------------------------------------------
unsigned char* __fastcall GetCurrentLightTable(int y, int x)
{
	uchar CurPalTabIndex = PerPixelLight[64 * y + x + PerPixelXOffset];

	if( CurPalTabIndex != 0 && CurPalTabIndex != LightMax )
		return (uchar*)s_LightTablePtrs[CurPalTabIndex];
	else if( CurPalTabIndex == LightMax )
		return (unsigned char*)1;
	else // CurPalTabIndex == 0: full bright; return LightTable row 0 (same as &LightTable[0])
		return (uchar*)s_LightTablePtrs[0];
}

// (th4) WALL BASE LIGHT state (see linecpyPerPixel, standard lighting only): >= 0 while the
// wall layers of a cell are drawn (the top row of its floor diamond), -1 otherwise.
static int g_WallBaseTopRow = -1;

// (th4) PPL CELL FILL -- the 64x32 per-pixel light block of one map cell, shared by
// CalculatePerPixelLight (single-thread) and CalculatePerPixelLightMT (worker threads).
// Bilinear blend of the 3x3 LightMap neighbourhood in grid[][], same integer math as the scalar
// incremental loop it replaced (2026-10-02), byte for byte: checked against that loop on 400k
// random grids, both scalings, LightMax 0..16. The derivation is below so it can be re-checked.
// Along a segment the value is quadratic in the step k: v_k = v + k*d + A*k*(k-1) (v = val4096 at
// the segment start, d = first difference, A = cross term; the old loop's "delta2" was 2*A).
// SSE2 evaluates 8 consecutive k at once: lane j starts at v_j and steps by
// v_{j+8} - v_j = 8*d + A*(16*j + 56), which itself grows by 128*A per step. All int32, no float.
// Lanes past the segment end compute garbage that the next segment overwrites in row[] (the row is
// assembled there and copied out whole), so nothing is written outside the 64x32 block.
// a8l8Scale: (val*17 + 2048) >> 12 clamped to 0..255; otherwise (val + 2048) >> 12 clamped to
// 0..LightMax. Within a segment val4096 is never negative, so the old code's unsigned multiply in
// the A8L8 case and the signed shift here agree.
static void PPLFillCell( const int grid[3][3], uchar* out, bool a8l8Scale )
{
	alignas(16) uchar row[64 + 16];
	const __m128i zero  = _mm_setzero_si128();
	const __m128i hi    = _mm_set1_epi16( (short)( a8l8Scale ? 255 : (int)LightMax ) );
	const __m128i round = _mm_set1_epi32( 2048 );
	for( int y = 0; y < 32; y++, out += 64 ){
		const int y2 = 2 * y;
		int sx = 0;
		while( sx < 64 ){
			const int dr64     = sx + y2;
			const int dc64_cm1 = y2 - sx + 64;
			const int dr    = dr64     >> 6;   // 0 or 1
			const int dcOff = dc64_cm1 >> 6;   // 0 or 1
			// segment end: dr flips 0->1 at sx = 64-y2; dcOff flips 1->0 at sx = y2+1
			int sxEnd = 64;
			if( dr == 0 )   { int b = 64 - y2;  if( b > sx && b < sxEnd ) sxEnd = b; }
			if( dcOff == 1 ){ int b = y2 + 1;   if( b > sx && b < sxEnd ) sxEnd = b; }
			const int v00 = grid[dr  ][dcOff  ], v10 = grid[dr+1][dcOff  ];
			const int v01 = grid[dr  ][dcOff+1], v11 = grid[dr+1][dcOff+1];
			const int fr = dr64 & 63, fc = dc64_cm1 & 63;
			const int v = v00*(64-fr)*(64-fc) + v10*fr*(64-fc) + v01*(64-fr)*fc + v11*fr*fc;
			const int A = -v00 + v10 + v01 - v11;
			const int d = (v10-v00)*(64-fc) + (v11-v01)*fc - (v01-v00)*(64-fr) - (v11-v10)*fr + A;
			__m128i V0 = _mm_setr_epi32( v,         v + d,          v + 2*d + 2*A,  v + 3*d + 6*A );
			__m128i V1 = _mm_setr_epi32( v + 4*d + 12*A, v + 5*d + 20*A, v + 6*d + 30*A, v + 7*d + 42*A );
			__m128i D0 = _mm_setr_epi32( 8*d + 56*A,  8*d + 72*A,  8*d + 88*A,  8*d + 104*A );
			__m128i D1 = _mm_setr_epi32( 8*d + 120*A, 8*d + 136*A, 8*d + 152*A, 8*d + 168*A );
			const __m128i DD = _mm_set1_epi32( 128 * A );
			for( int k = sx; k < sxEnd; k += 8 ){
				__m128i a = V0, b = V1;
				if( a8l8Scale ){
					a = _mm_add_epi32( _mm_slli_epi32( a, 4 ), a );   // *17
					b = _mm_add_epi32( _mm_slli_epi32( b, 4 ), b );
				}
				a = _mm_srai_epi32( _mm_add_epi32( a, round ), 12 );
				b = _mm_srai_epi32( _mm_add_epi32( b, round ), 12 );
				__m128i p = _mm_packs_epi32( a, b );
				p = _mm_min_epi16( _mm_max_epi16( p, zero ), hi );
				_mm_storel_epi64( (__m128i*)&row[k], _mm_packus_epi16( p, p ) );
				V0 = _mm_add_epi32( V0, D0 ); V1 = _mm_add_epi32( V1, D1 );
				D0 = _mm_add_epi32( D0, DD ); D1 = _mm_add_epi32( D1, DD );
			}
			sx = sxEnd;
		}
		memcpy( out, row, 64 );
	}
}

// (th4) PPL CELL CACHE (single-thread standard lighting, CalculatePerPixelLight): PPLFillCell's 64x32 block is
// a pure function of the cell's 3x3 LightMap neighbourhood and LightMax, and those rarely change from one
// frame to the next (only near moving or flickering lights). So blocks are kept in a direct-mapped cache
// keyed by exactly those inputs, and a hit just points PerPixelLight at the stored block (zero copy, like
// the MT path does with s_PPLData) -- same bytes as computing it, nothing visible changes. PPLFillCell was
// the top CPU function in Mordor's profile (2026-10-05). The tag holds every input bit, so a slot can only
// match the very same inputs; a slot is only ever overwritten with a newer block for other inputs.
// Its own arrays, NOT s_PPLData: the MT workers write that one, and a timed-out dispatch may still be at it.
static const int PPL_CACHE_N = 4096;					// 4096 x 2 KB = 8 MB
struct PPLCacheTag { uint a, b, c; };					// grid bytes 0..3, 4..7, 8 | LightMax << 8 | valid << 16
static PPLCacheTag s_PPLCacheTag[ PPL_CACHE_N ];
static uchar       s_PPLCacheData[ PPL_CACHE_N ][ 64 * 32 ];

static uchar* PPLCachedCell( const int grid[3][3] )
{
	const uint a = (uint)grid[0][0] | (uint)grid[0][1] << 8 | (uint)grid[0][2] << 16 | (uint)grid[1][0] << 24;
	const uint b = (uint)grid[1][1] | (uint)grid[1][2] << 8 | (uint)grid[2][0] << 16 | (uint)grid[2][1] << 24;
	const uint c = (uint)grid[2][2] | (uint)(uchar)LightMax << 8 | 1u << 16;
	uint h = a * 0x9E3779B1u ^ b * 0x85EBCA77u ^ c * 0xC2B2AE3Du;
	h ^= h >> 15; h *= 0x2C1B3C6Du; h ^= h >> 12;
	const int slot = (int)( h & ( PPL_CACHE_N - 1 ) );
	PPLCacheTag& t = s_PPLCacheTag[ slot ];
	if( t.a != a || t.b != b || t.c != c ){
		PPLFillCell( grid, s_PPLCacheData[ slot ], false );	// standard lighting only (the caller returns before for A8L8)
		t.a = a; t.b = b; t.c = c;
	}
	return s_PPLCacheData[ slot ];
}

// (th4) PPL REMAP (standard lighting, !g_A8L8Lighting): s_PPLRemap[lv][sv] is the colour linecpyPerPixel
// writes for palette index sv at light level lv -- sv itself for lv == 0 (full bright = the raw index,
// deliberately NOT LightTable row 0, which the Hell blood-fade rewrites), LightTable row lv for
// 1..LightMax-1, 0 (black) for lv >= LightMax. ONE flat table, so a pixel costs one lookup at
// (lv << 8 | sv) -- the per-pixel hot spot in Mordor's Hellforge profile (2026-10-05); it was a row-pointer
// table before, one dependent load more per pixel. Same result for every lv / sv.
// LightTable CONTENT changes between frames (AnimateHellPalette cycles the blood rows; MakeLightTable on a
// new level), so rows 1..LightMax-1 are copied again once per frame (PPLRemap_Refresh, DrawIsoLand start:
// ~4 KB); the identity and black rows only when LightMax changes. Read only on the render thread.
static uchar s_PPLRemap[256][256];
static int   s_PPLRemapLmax = -1;

static void PPLRemap_Refresh()
{
	const int lmax = (uchar)LightMax;
	if( lmax != s_PPLRemapLmax ){
		memset( s_PPLRemap, 0, sizeof( s_PPLRemap ) );				// lv >= LightMax: black
		for( int i = 0; i < 256; i++ ) s_PPLRemap[0][i] = (uchar)i;	// lv 0: the raw index
		s_PPLRemapLmax = lmax;
	}
	for( int lv = 1; lv < lmax; lv++ ) memcpy( s_PPLRemap[lv], &LightTable[256 * lv], 256 );
}

// (th4) A8L8 FAST PATH STATE (linecpyPerPixel, g_A8L8Lighting). What the Enriched Light loop does
// with a pixel depends on two tables MakeLightTable (Lights.cpp) sets once per level:
//   g_PPLAnimRemap      -- the identity table everywhere except Hell (LightTable row 0 there, which
//                          AnimateHellPalette rewrites every frame);
//   g_PPLBrightnessMask -- all 0xFF except crypt/abyss, where one contiguous range of entries
//                          (CRYPT_BRIGHT_PAL_FIRST..END) is 0x00.
// With the remap the identity, d = sv for every lv, i.e. a plain copy; with the mask all 0xFF,
// dstL = lv, a plain copy; with one 0x00 range, dstL = lv except 0 inside the range -- all SSE2.
// MakeLightTable reports both through PPL_NoteRemapAndMask() right after setting them; nothing else
// writes either table. Until the first report the state says "generic" and the per-pixel loop runs.
// IF YOU EVER WRITE g_PPLAnimRemap OR g_PPLBrightnessMask ANYWHERE ELSE, call PPL_NoteRemapAndMask
// there too, or the fast path keeps using the old picture of the tables.
enum { PPLMASK_GENERIC = 0, PPLMASK_ALL_FF = 1, PPLMASK_ONE_RANGE = 2 };
static bool s_PPLRemapIdentity = false;
static int  s_PPLMaskKind = PPLMASK_GENERIC;
static int  s_PPLMaskLo = 0, s_PPLMaskN = 0;	// PPLMASK_ONE_RANGE: entries lo .. lo+N-1 are 0x00

void PPL_NoteRemapAndMask( bool remapIsIdentity )
{
	s_PPLRemapIdentity = remapIsIdentity;
	int lo = -1, hi = -1, kind = PPLMASK_ALL_FF;
	for( int i = 0; i < 256 && kind != PPLMASK_GENERIC; i++ ){
		const uchar m = g_PPLBrightnessMask[ i ];
		if( m == 0xFF ){ if( lo >= 0 && hi < 0 ) hi = i; continue; }
		if( m != 0x00 || hi >= 0 ){ kind = PPLMASK_GENERIC; break; }	// another value, or a second 0x00 run
		if( lo < 0 ) lo = i;
		kind = PPLMASK_ONE_RANGE;
	}
	if( kind == PPLMASK_ONE_RANGE ){ if( hi < 0 ) hi = 256; s_PPLMaskLo = lo; s_PPLMaskN = hi - lo; }
	s_PPLMaskKind = kind;
}

// dstL for 16 pixels under a known mask kind: lv, or lv with the 0x00-range entries zeroed.
static inline __m128i PPLMaskedLight( __m128i lv, __m128i sv )
{
	if( s_PPLMaskKind == PPLMASK_ALL_FF ) return lv;
	const __m128i x = _mm_sub_epi8( sv, _mm_set1_epi8( (char)s_PPLMaskLo ) );
	const __m128i in = _mm_cmpeq_epi8( _mm_min_epu8( x, _mm_set1_epi8( (char)( s_PPLMaskN - 1 ) ) ), x );	// (uchar)(sv - lo) < N
	return _mm_andnot_si128( in, lv );
}

// ---- th2 -------------------------------------------------------------------------------
// xStart: starting column within the 32px sub-tile (0 for squares/right-shapes; skip for left triangles/trapezoids)
void __fastcall linecpyPerPixel(void* dst, void* src, int size, int h, unsigned mask, int xStart)
{
	// (th4) WALL BASE LIGHT -- STANDARD LIGHTING ONLY (!g_A8L8Lighting). The per-cell
	// PerPixelLight 64x32 block is a map of the cell's FLOOR; wall layers used to read it at the
	// wall pixel's own row, so every 32-px wall layer repeated the floor's diamond gradient -- the
	// stepped "sawtooth" of light and dark on wall faces. Here a wall-layer pixel takes the light of
	// the point where its column meets the floor: the back edge of its cell's diamond (walls stand
	// on the two back edges: row (31 - x) / 2 left of the top vertex, (x - 32) / 2 right of it,
	// x = column 0..63 in the cell). So the light is constant up each wall column and changes
	// smoothly along the wall. Set by the wall-layer loops in DrawSceneDungeonTop and DrawMapCell
	// (g_WallBaseTopRow >= 0 while wall layers are drawn), -1 everywhere else.
	// Enriched Light (Enhanced / Ultra, g_A8L8Lighting) is deliberately NOT touched: it keeps its
	// own screen-space light path below, byte for byte.
	if( g_WallBaseTopRow >= 0 && !g_A8L8Lighting ){
		if( s_PPLRemapLmax != (uchar)LightMax ) PPLRemap_Refresh();	// (normally refreshed at DrawIsoLand start)
		uchar* d = (uchar*)dst;
		const uchar* s = (const uchar*)src;
		const bool full = mask == 0xFFFFFFFF;
		const int colBase = PerPixelXOffset + xStart;
		for( int i = 0; i < size; i++, mask <<= 1 ){
			if( !full && !( mask & 0x80000000 ) ) continue;
			int col = colBase + i; if( col < 0 ) col = 0; else if( col > 63 ) col = 63;
			const uchar lv = PerPixelLight[ 64 * ( ( col < 32 ? 31 - col : col - 32 ) >> 1 ) + col ];
			d[ i ] = s_PPLRemap[ lv ][ s[ i ] ];	// lv 0 = raw index, >= LightMax = black (PPL REMAP)
		}
		return;
	}
	h -= 1; // convert [1..32] scanline counter to [0..31] PerPixelLight row index
	// Direct lookup: avoids per-pixel function-call overhead; correctly offsets into the 64px-wide buffer
	const uchar* __restrict ppLight = &PerPixelLight[64 * h + PerPixelXOffset + xStart];
	uchar* __restrict        d      = (uchar*)dst;
	const uchar* __restrict  s      = (const uchar*)src;
	// When A8L8 is disabled, apply LightTable darkening per-pixel to WorkingSurface only.
	// This restores the pre-A8L8-branch PPL behavior: per-pixel smooth light gradients
	// with discrete palette darkening (color banding). LightSurface is never accessed —
	// safe even when LightSurface == nullptr.
	if (!g_A8L8Lighting) {
		// Pre-A8L8 path: restore exact pre-branch behavior.
		// Use raw s[i] — do NOT apply g_PPLAnimRemap here.
		// AnimateHellPalette cycles all LightTable rows, so s_LightTablePtrs[lv][s[i]]
		// naturally picks up the animated blood colors once per frame via the lv row.
		// Applying g_PPLAnimRemap first (row 0) and then s_LightTablePtrs[lv] (row lv)
		// would hit the cycle twice → 2x animation speed.
		// (th4) One lookup per pixel in the flat PPL REMAP (defined above this function): same result as
		// the old (lv==0) ? sv : (lv>=LightMax ? 0 : LightTable row lv) for every lv/sv.
		if( s_PPLRemapLmax != (uchar)LightMax ) PPLRemap_Refresh();	// (normally refreshed at DrawIsoLand start)
		const uchar* __restrict rem = &s_PPLRemap[0][0];
		if (mask == 0xFFFFFFFF) {
			for (int i = 0; i < size; i++) d[i] = rem[(ppLight[i] << 8) | s[i]];
		} else {
			for (int i = 0; i < size; i++, mask <<= 1)
				if (mask & 0x80000000) d[i] = rem[(ppLight[i] << 8) | s[i]];
		}
		return;
	}
	// Approach B (A8L8): write raw palette index to WorkingSurface, light level to LightSurface.
	// The GPU shader will do the brightness multiply in floating-point → no banding.
	// NOTE: do NOT add xStart here. dst already points to the correct WorkingSurface
	// column (some callers pass &DstSurf[skip] with xStart==skip, others pass DstSurf
	// with xStart == sub-tile column offset). LightSurface mirrors WorkingSurface 1:1,
	// so dstL[i] must correspond to d[i] == dst[i] with no extra offset.
	ptrdiff_t bufOffset = (uchar*)dst - SoftSrcSurfacePtr;
	// srcL: ambient light levels from LightSurfaceBase (immutable pre-fill snapshot, written by
	// FillScreenSpaceLighting alongside LightSurface). dstL: LightSurface, the live buffer the
	// shader reads. Both are separate calloc'd buffers (dx.cpp), distinct from d (WorkingSurface)
	// and s (CEL source), so __restrict holds for all four.
	const uchar* __restrict srcL = LightSurfaceBase + bufOffset;
	uchar* __restrict dstL = LightSurface + bufOffset;
	// Per-call-invariant globals hoisted into locals so they stay in registers.
	const bool isDrawingOpaqueWall = g_IsDrawingOpaqueWall;
	uchar* const wobSurface = WallOcclusionSurface;
	const uchar* const remap = g_PPLAnimRemap;
	const uchar* const bmask = g_PPLBrightnessMask;
	// Screen-space PPL: LightSurface is pre-filled by FillScreenSpaceLighting with
	// bilinearly interpolated LightMap values.  Read lv from srcL (the immutable
	// pre-fill snapshot) instead of PerPixelLight.  This gives every pixel its exact
	// iso-coordinate bilinear value — no per-cell swaps, no layer discontinuities,
	// and sprite-cleared LightSurface pixels do not contaminate tile rendering.
	// lv==0 branch is REQUIRED: LightTable row 0 is rewritten by the blood-fade code
	// for Hell dungeon type and is NOT the identity. The original bypass (d[i]=s[i])
	// is the correct "full bright" behavior regardless of any palette effect on row 0.
	// g_PPLBrightnessMask: 0xFF = apply lv normally; 0x00 = force lv=0 (always bright).
	// Used for crypt/abyss entries 1-15 which the LightTable maps to themselves in every
	// row — the GPU path must match that "always bright" behaviour.
	// lv==0 = full bright, lv==255 = full dark, 1-254 = intermediate.
	// Full-dark uses d[i]=sv (real palette index, not 0) because the shader computes
	// ScreenPalette[sv]*(1-255/255)=black regardless of sv.
	// The brightness mask is applied at lv==255 as well: crypt/abyss lava-range entries stay full-bright even
	// in total darkness (level-invariant "always bright", matching the intermediate-lv
	// branch and V1's LightTable identity mapping) so the animated floor/stairs are not
	// cut off at the light-field edge.  In every other dungeon the mask is all-0xFF, so
	// lmax_A8L8 & mask == lmax_A8L8 and these branches are byte-identical to before.
	// (th4) Branch-free form of the three cases above, same bytes for every lv/sv:
	//   d    = sv for lv 0 and lv 255, g_PPLAnimRemap[sv] otherwise. (uchar)(lv - 1) >= 254 is
	//          exactly lv == 0 || lv == 255.
	//   dstL = lv & g_PPLBrightnessMask[sv] -- 0 when lv == 0, the mask itself when lv == 255
	//          (255 & m == m), lv & m otherwise.
	// The full-mask path writes WOB_WALL to every pixel it touches, so it is one memset after
	// the loop instead of a store per pixel.
	// (th4) "Full run" = every pixel 0..size-1 has its mask bit set. Besides mask == 0xFFFFFFFF that is
	// the triangle / trapezoid rows, whose mask is shifted (mask << skip) but still all ones over their
	// size pixels; they used to take the per-pixel masked loop for nothing. Same pixels written either way.
	const bool fullRun = size <= 0 || ( size >= 32 ? mask == 0xFFFFFFFF : ( mask >> ( 32 - size ) ) == ( 1u << size ) - 1u );
	if (fullRun) {
		int i = 0;
		if (s_PPLMaskKind != PPLMASK_GENERIC) {	// A8L8 FAST PATH (state above PPL_NoteRemapAndMask)
			if (s_PPLRemapIdentity) {
				// remap is the identity -> d = sv for every lv: copy; dstL via the mask kind
				for (; i + 16 <= size; i += 16) {
					const __m128i sv = _mm_loadu_si128((const __m128i*)(s + i));
					const __m128i lv = _mm_loadu_si128((const __m128i*)(srcL + i));
					_mm_storeu_si128((__m128i*)(d + i), sv);
					_mm_storeu_si128((__m128i*)(dstL + i), PPLMaskedLight(lv, sv));
				}
				if (i + 8 <= size) {
					const __m128i sv = _mm_loadl_epi64((const __m128i*)(s + i));
					const __m128i lv = _mm_loadl_epi64((const __m128i*)(srcL + i));
					_mm_storel_epi64((__m128i*)(d + i), sv);
					_mm_storel_epi64((__m128i*)(dstL + i), PPLMaskedLight(lv, sv));
					i += 8;
				}
			} else {
				// Hell: d still needs the per-pixel remap lookup; dstL goes 16 at a time
				for (; i + 16 <= size; i += 16) {
					const __m128i sv = _mm_loadu_si128((const __m128i*)(s + i));
					const __m128i lv = _mm_loadu_si128((const __m128i*)(srcL + i));
					_mm_storeu_si128((__m128i*)(dstL + i), PPLMaskedLight(lv, sv));
					for (int k = i; k < i + 16; k++) d[k] = (uchar)(srcL[k] - 1) >= 254 ? s[k] : remap[s[k]];
				}
			}
		}
		for (; i < size; i++) {
			const uchar lv = srcL[i], sv = s[i];
			const uchar rv = remap[sv];
			d[i]    = (uchar)(lv - 1) >= 254 ? sv : rv;
			dstL[i] = lv & bmask[sv];
		}
		// (th4) mark wall occlusion for silhouette pass
		if (isDrawingOpaqueWall && wobSurface && size > 0) memset(wobSurface + bufOffset, WOB_WALL, size);
	} else {
		for (int i = 0; i < size; i++, mask <<= 1) {
			if (mask & 0x80000000) {
				const uchar lv = srcL[i], sv = s[i];
				const uchar rv = remap[sv];
				d[i]    = (uchar)(lv - 1) >= 254 ? sv : rv;
				dstL[i] = lv & bmask[sv];
				if (isDrawingOpaqueWall && wobSurface) wobSurface[bufOffset + i] = WOB_WALL;
			}
		}
	}
}

// ===== Per-tile PPL buffer cache for MultiThreadPixelLight mode =====
// Workers write s_PPLData[r][c] in parallel; the serial render loop points PerPixelLight at it (zero copy).
static uchar s_PPLData[FineMap_112][FineMap_112][64 * 32];
static bool  s_PPLUniform[FineMap_112][FineMap_112];
// (th4) PPL CELL CACHE, MT half (see PPLCachedCell for the single-thread half): s_PPLData[r][c] keeps the
// block of the inputs in s_PPLSig[r][c] -- the cell's 3x3 LightMap neighbourhood, LightMax and the A8L8 flag.
// Same inputs next frame = same block already there, so the worker skips PPLFillCell. Only the worker that
// owns cell (r, c) this frame touches its data and its tag (each cell is dispatched once per frame).
static PPLCacheTag s_PPLSig[FineMap_112][FineMap_112];

static void CalculatePerPixelLightMT(int Row, int Col, uchar* outData, bool& outUniform)
{
	const uint lmax_A8L8 = 255; // max level in A8L8 mode is always 255 (full dark) regardless of LightMax, because the shader remaps it to 0.0 brightness; this allows full 256-level smooth gradients in A8L8 mode even when LightMax is only 3 or 15.
	const uint A8L8_SCALE = lmax_A8L8 / 15; // = 17, maps LightMap [0..15] to A8L8 [0..255]
	auto L = [](int r, int c) -> int {
		r = r < 0 ? 0 : r >= FineMap_112 ? FineMap_112-1 : r;
		c = c < 0 ? 0 : c >= FineMap_112 ? FineMap_112-1 : c;
		return (uchar)LightMap[r][c];
	};
	int grid[3][3];
	for( int dr = 0; dr < 3; dr++ )
		for( int dc = 0; dc < 3; dc++ )
			grid[dr][dc] = L(Row-1+dr, Col-1+dc);
	{
		int v0 = grid[0][0]; bool allEqual = true;
		for( int i = 0; i < 3 && allEqual; i++ )
			for( int j = 0; j < 3 && allEqual; j++ )
				if( grid[i][j] != v0 ) allEqual = false;
		if( allEqual ){
			if (g_A8L8Lighting) {
				// A8L8: fill outData with the uniform scaled value so the
				// PPL path is always used (see CalculatePerPixelLight for details).
				outUniform = false;
				int val = (v0 * A8L8_SCALE > lmax_A8L8) ? lmax_A8L8 : v0 * A8L8_SCALE;
				memset(outData, (uchar)val, 64 * 32);
				if( outData == s_PPLData[ Row ][ Col ] ) s_PPLSig[ Row ][ Col ].c = 0;	// (th4) PPL CELL CACHE: the block changed
			} else {
				outUniform = true;
			}
			return;
		}
	}
	outUniform = false;
	// (th4) PPL CELL CACHE (above s_PPLSig): nothing to do if this cell's block already has these inputs
	const uint ta = (uint)grid[0][0] | (uint)grid[0][1] << 8 | (uint)grid[0][2] << 16 | (uint)grid[1][0] << 24;
	const uint tb = (uint)grid[1][1] | (uint)grid[1][2] << 8 | (uint)grid[2][0] << 16 | (uint)grid[2][1] << 24;
	const uint tc = (uint)grid[2][2] | (uint)(uchar)LightMax << 8 | 1u << 16 | ( g_A8L8Lighting ? 1u << 17 : 0u );
	PPLCacheTag& sig = s_PPLSig[ Row ][ Col ];
	if( outData == s_PPLData[ Row ][ Col ] && sig.a == ta && sig.b == tb && sig.c == tc ) return;
	sig.c = 0;	// invalid while being written
	PPLFillCell( grid, outData, g_A8L8Lighting ); // (th4) shared SSE2 fill -- see PPLFillCell, above linecpyPerPixel
	if( outData == s_PPLData[ Row ][ Col ] ){ sig.a = ta; sig.b = tb; sig.c = tc; }
}

// ===== Persistent thread pool =====
// KEY DESIGN DECISIONS:
//   1. Two mutex/cv pairs: s_PPLWorkMtx/s_PPLCvWork (worker sleep) and
//      s_PPLDoneMtx/s_PPLCvDone (main done-wait). Held sequentially, never together.
//   2. Global barrier: PPLPoolDispatch blocks until all tiles done, then render reads
//      s_PPLData with zero per-tile sync overhead. Overlap (~380µs on 7 cores) is
//      negligible vs render time (~3ms/frame), so a barrier is the right trade-off.
//   3. Main thread does NOT participate in tile work (no counting complexity).
//   4. s_PPLQuit is std::atomic<bool>: shutdown doesn't race with in-progress work.
static const int PPL_MAX_WORKERS = 7;
// Max wait for workers to finish a dispatch; a normal dispatch takes <1ms, so
// 1s is a 1000x safety margin.  On timeout the caller falls back to
// single-threaded completion — a dead or stalled worker can never deadlock
// the game (pattern from PPLMTDebug @ 4698dc7a).
static const int PPL_DISPATCH_TIMEOUT_MS = 1000;
// (th4) 2026-09-10 hardening: bounded settle after a RESOLVE-dispatch timeout
// (see PPLResolveDispatch's timeout path below). This code only runs when the
// 1 s timeout above already fired — an event never observed in normal play
// (s_PPLTimeoutCount == 0 in every session so far). The bound is sized to
// cover realistic worker-resume latencies (debugger-pause release, OS thread
// starvation) before the caller unlocks the s0 color texture: the previous
// 2 ms window left a hole where a late worker resume could write one row
// segment into texture memory the driver may already have re-used
// (documented limitation (b) of the MT resolve).
//static const int PPL_RESOLVE_SETTLE_MS = 250;
struct PPLTileCoord { short r, c; };
static PPLTileCoord              s_PPLTiles[FineMap_112 * FineMap_112];
static std::atomic<int>          s_PPLClaimed{0}; // next tile index for work-stealing
static std::atomic<int>          s_PPLDone{0};    // completed tile count this dispatch
static std::atomic<int>          s_PPLStaleAborts{0}; // stale workers aborted rows mid-dispatch (cross-frame protection)
static std::atomic<int>          s_PPLTotal{0};   // total tiles this dispatch
static std::atomic<int>          s_PPLFrameId{0}; // incremented each dispatch to wake workers
static std::atomic<bool>         s_PPLQuit{false};
static std::mutex                s_PPLWorkMtx;    // guards s_PPLFrameId for worker cv predicate
static std::condition_variable   s_PPLCvWork;     // workers sleep here between frames
static std::mutex                s_PPLDoneMtx;    // guards done-wait for main thread
static std::condition_variable   s_PPLCvDone;     // main sleeps here until all tiles done
static int                       s_PPLWorkerCnt = 0;
static bool                      s_PPLDispatched = false;
static std::thread               s_PPLWorkers[PPL_MAX_WORKERS];

// Session-level worker-pool health.  Accessed only from the render thread, so
// plain variables are enough (no atomics).
static const int s_PPLMaxTimeouts = 3;    // consecutive dispatch timeouts before MT is disabled
static int       s_PPLTimeoutCount = 0;   // consecutive dispatch timeouts so far
static bool      s_PPLPoolBroken = false; // pool presumed broken: skip MT dispatch for the session

// Called by the dispatch sites on a wait_for timeout.  A single timeout can be
// a one-off stall, but several in a row mean a worker is stuck or dead — every
// further dispatch would cost another 1 s stall.  Mark the pool broken so the
// renderer runs single-threaded for the rest of the session (full speed, no
// per-frame stall) instead of trying to fix a thread that may never come back.
static void PPLNoteTimeout()
{
	if( ++s_PPLTimeoutCount >= s_PPLMaxTimeouts && !s_PPLPoolBroken ){
		s_PPLPoolBroken = true;
		// (th4) 2026-09-10 diagnostics: repeated dispatch timeouts are exactly
		// the situation where a stalled worker can resume after the caller's
		// UnlockRect — log it so a crash can be correlated with MT degradation.
		D3DDIAG("PPL pool disabled after %d consecutive dispatch timeouts", s_PPLTimeoutCount);
		OutputDebugString("PPL: worker pool marked broken after repeated dispatch timeouts; multi-threaded lighting disabled for this session\n");
	}
}

// ===== Screen-space MT pre-fill shared state =====
// Set by main thread before dispatching workers; read-only by workers during
// row processing.  Each worker writes to disjoint rows of LightSurface — no
// per-pixel atomics or locks needed.
static int  s_SSRowStart = 0;   // first screen Y row to process
static int  s_SSRowCount = 0;   // total number of rows
static int  s_SSEndX     = 0;   // rightmost pixel column (exclusive)
static int  s_SSX0       = 0;   // iso projection X0 constant
static int  s_SSY0       = 0;   // iso projection Y0 constant
static int  s_SSMode     = 0;   // 0 = per-cell tiles, 1 = old screen-space rows, 2 = V2 field rows

static void FillScreenSpaceRow(int wsY, int epoch = -1); // forward decl — defined after PPLPoolDispatch; epoch<0 = never abort (ST/main)

// ===== V2 per-pixel light field MT shared state (Rev 24) =====
// Set by the main thread before dispatch; read-only by workers.  Rows are
// disjoint — no per-pixel atomics or locks.  The legacy dispatch path above
// is untouched; V2 uses its own mode flag (s_SSMode == 2).
struct V2Stamp {
	int  centerRow64;
	int  centerCol64;
	int  D64;
	int  D64sq;
	int  rad;
	const uchar* lut;
	// (th4) Stage 3: the light's tint (white = no tint). Written to
	// LightTintSurface wherever this light strictly brightens a pixel.
	uchar tintR, tintG, tintB;
};
static void FillScreenSpaceRowV2(int wsY, int X0, int Y0, int startX, int endX,
	bool useStaticAmbient, bool zeroAmbient, const V2Stamp* stamps, int stampCount, int epoch = -1);

static V2Stamp s_V2Stamps[Lights_128_Count];
static int     s_V2StampCount = 0;
static int     s_V2X0 = 0, s_V2Y0 = 0, s_V2StartX = 0, s_V2EndX = 0;
static int     s_V2RowStart = 0, s_V2RowCount = 0;
static bool    s_V2UseStaticAmbient = false, s_V2ZeroAmbient = false;

// (th4) Stage 4 item 6 (Option A): palette->RGB resolve dispatch state (mode 3).
// The context is published under s_PPLWorkMtx before the frameId bump, so a
// worker's claim always reads a consistent context (same M2 rule as V2).
static ResolveMTContext s_ResolveCtx;
static int              s_ResolveRowStart = 0, s_ResolveRowCount = 0;
static std::atomic<int> s_ResolveInFlight{0}; // workers currently inside a row (timeout settle only)

// (th4) Stage 4 item 6: epoch accessor for the resolve row function (keeps
// s_PPLFrameId private to this translation unit).
int PPLCheckEpoch(int epoch)
{
	return s_PPLFrameId.load(std::memory_order_relaxed) == epoch ? 1 : 0;
}

static void PPLWorkerLoop(int)
{
	int lastFrame = 0;
	for(;;){
		// Sleep until a new frame is dispatched or shutdown requested.
		int epoch = lastFrame;
		int mode = 0;
		int total = 0;
		{
			std::unique_lock<std::mutex> lk(s_PPLWorkMtx);
			s_PPLCvWork.wait(lk, [&]{
				return s_PPLQuit.load(std::memory_order_relaxed)
				    || s_PPLFrameId.load(std::memory_order_relaxed) != lastFrame;
			});
			if( s_PPLQuit.load() ) return;
			lastFrame = s_PPLFrameId.load(std::memory_order_relaxed);
			epoch = lastFrame;
			mode = s_SSMode;
			total = s_PPLTotal.load(std::memory_order_relaxed);
		} // s_PPLWorkMtx released here — never held during tile/row work

		// Work-steal tiles (mode 0), old screen-space rows (mode 1) or
		// V2 per-pixel field rows (mode 2).  Each claim is made under
		// s_PPLWorkMtx — the same mutex the main thread holds while it
		// publishes a dispatch (mode + counter resets + frameId bump) — so the
		// frameId check and the counter claim are atomic w.r.t. a publish: a
		// worker can never claim a work item of a newer dispatch while still
		// holding the old epoch (closes the publish-vs-stale-claim race), and
		// s_SSMode/s_PPLTotal are always read under the mutex (no data race).
		for(;;){
			int workIdx;
			{
				std::lock_guard<std::mutex> lk(s_PPLWorkMtx);
				const int curFrame = s_PPLFrameId.load(std::memory_order_relaxed);
				if( curFrame != epoch ){
					// A newer dispatch was published while we were working:
					// refresh the snapshot BEFORE claiming so this claim
					// belongs to the new dispatch with a consistent context.
					lastFrame = epoch = curFrame;
					mode = s_SSMode;
					total = s_PPLTotal.load(std::memory_order_relaxed);
				}
				workIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
			}
			if( workIdx >= total ) break;
			if (mode == 2) {
				// === V2 per-pixel light field: each work item is one screen Y row ===
				FillScreenSpaceRowV2(s_V2RowStart + workIdx, s_V2X0, s_V2Y0, s_V2StartX, s_V2EndX,
					s_V2UseStaticAmbient, s_V2ZeroAmbient, s_V2Stamps, s_V2StampCount, epoch);
			} else if (mode == 1) {
				// === Old screen-space A8L8 mode: each work item is one screen Y row ===
				FillScreenSpaceRow(s_SSRowStart + workIdx, epoch);
			} else if (mode == 3) {
				// === Stage 4 item 6: palette->RGB resolve rows (locked s0 texture) ===
				// in-flight tracks workers inside a row so the timeout path can
				// settle before the caller unlocks the s0 texture.
				s_ResolveInFlight.fetch_add(1, std::memory_order_relaxed);
				ResolveRowMT(s_ResolveRowStart + workIdx, s_ResolveCtx, epoch);
				s_ResolveInFlight.fetch_sub(1, std::memory_order_relaxed);
			} else {
				// === Per-cell mode (legacy): each work item is one (row,col) tile ===
				int r = s_PPLTiles[workIdx].r, c = s_PPLTiles[workIdx].c;
				CalculatePerPixelLightMT(r, c, s_PPLData[r][c], s_PPLUniform[r][c]);
			}
			// Epoch-guarded done: a work item that finished under its own
			// epoch counts normally.  One that finished AFTER a newer dispatch
			// was published was abandoned (timeout-drain); any pixels it wrote
			// late may have landed in the new frame's buffers, so flag the
			// frame — its cache snapshot must not capture them.
			if( s_PPLFrameId.load(std::memory_order_relaxed) == epoch ){
				if( s_PPLDone.fetch_add(1, std::memory_order_acq_rel) + 1 >= total ){
					std::lock_guard<std::mutex> dlk(s_PPLDoneMtx);
					s_PPLCvDone.notify_one();
					break;
				}
			}else{
				s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
			}
		}
	}
}

static void PPLPoolInit()
{
	if( s_PPLWorkerCnt > 0 ) return;
	s_PPLWorkerCnt = std::max(1, std::min(PPL_MAX_WORKERS,
	                           (int)std::thread::hardware_concurrency() - 1));
	for( int i = 0; i < s_PPLWorkerCnt; i++ )
		s_PPLWorkers[i] = std::thread(PPLWorkerLoop, i);
}

static void PPLPoolShutdown()
{
	if( s_PPLWorkerCnt == 0 ) return;
	s_PPLQuit.store(true, std::memory_order_relaxed);
	// Must increment frameId under the work mutex so sleeping workers' predicates fire.
	{ std::lock_guard<std::mutex> lk(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
	s_PPLCvWork.notify_all();
	if( s_PPLPoolBroken ){
		// A worker is stuck or dead: join() could hang process exit forever.
		// Detach instead — the OS reclaims the threads when the process exits.
		OutputDebugString("PPL: worker pool is broken — detaching workers at shutdown\n");
		for( int i = 0; i < s_PPLWorkerCnt; i++ )
			if( s_PPLWorkers[i].joinable() ) s_PPLWorkers[i].detach();
		s_PPLWorkerCnt = 0;
		return;
	}
	for( int i = 0; i < s_PPLWorkerCnt; i++ )
		if( s_PPLWorkers[i].joinable() ) s_PPLWorkers[i].join();
	s_PPLWorkerCnt = 0;
}

struct PPLPoolGuard { ~PPLPoolGuard(){ PPLPoolShutdown(); } };
static PPLPoolGuard s_PPLPoolGuard; // destructed before s_PPLWorkers (reverse init order)

static void PPLPoolDispatch(int row, int col, int screenRowEnd, int horCount)
{
	int count = 0;
	for (int k = 0; k < screenRowEnd; k++) {
		for( int i = 0; i < horCount; i++ ){
			const int r = row + k + i;
			if( r < 0 || r >= FineMap_112 ) continue;
			const int c_even = col + k - i;
			if( c_even >= 0 && c_even < FineMap_112 )
				s_PPLTiles[count++] = {(short)r, (short)c_even};
			const int c_odd = col + k + 1 - i;
			if( c_odd >= 0 && c_odd < FineMap_112 )
				s_PPLTiles[count++] = {(short)r, (short)c_odd};
		}
	}

	if( count == 0 ) return;

	s_PPLDispatched = true;
	// Publish mode + counters + frameId under s_PPLWorkMtx so the whole
	// dispatch state is atomic w.r.t. the workers' claim sections (M2 fix),
	// and the cv predicate fires atomically — workers won't miss the wake even
	// if they check just before notify.
	{
		std::lock_guard<std::mutex> lk(s_PPLWorkMtx);
		s_SSMode = 0;
		s_PPLClaimed.store(0, std::memory_order_relaxed);
		s_PPLDone.store(0,    std::memory_order_relaxed);
		s_PPLTotal.store(count, std::memory_order_relaxed);
		s_PPLFrameId.fetch_add(1, std::memory_order_relaxed);
	}
	s_PPLCvWork.notify_all();

	// Block until all tiles are done. s_PPLDoneMtx is separate from s_PPLWorkMtx:
	// workers lock it briefly only to call notify_one — no cross-mutex deadlock risk.
	// If all workers finish before we reach this wait, the predicate is already true
	// and wait() returns without sleeping (no missed-notification risk).
	// wait_for with timeout: a dead/stalled worker can never deadlock the game —
	// on timeout fall back to the single-threaded PPL path for this frame.
	std::unique_lock<std::mutex> lk(s_PPLDoneMtx);
	const bool completed = s_PPLCvDone.wait_for(lk, std::chrono::milliseconds(PPL_DISPATCH_TIMEOUT_MS), [count]{
		return s_PPLDone.load(std::memory_order_acquire) >= count;
	});
	if (!completed) {
		static bool logged = false;
		if (!logged) {
			logged = true;
			OutputDebugString("PPL TIMEOUT: per-cell dispatch incomplete; falling back to ST PPL\n");
		}
		s_PPLDispatched = false; // render loop uses single-threaded PPL for this frame
		PPLNoteTimeout(); // M1: repeated timeouts disable MT for the session
	} else {
		s_PPLTimeoutCount = 0;
	}
	lk.unlock();

	// Rev 27 hardening: expire worker epochs at the end of the dispatch (see
	// FillScreenSpaceLightingV2MT).  Per-cell tiles have no in-tile epoch
	// checks, so a mid-tile resume can still finish one tile — but its done
	// increment is epoch-guarded and flagged stale, keeping the counters
	// truthful for the next dispatch.
	{ std::lock_guard<std::mutex> lk2(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
}


// ===== Screen-space per-pixel lighting: single-row worker =====
// Processes one screen Y row of the A8L8 bilinear pre-fill.  Identical math
// to FillScreenSpaceLighting but scoped to a single wsY.  Called by worker
// threads during MT dispatch; each worker claims rows via s_PPLClaimed.
// Writes directly to LightSurface[wsY][*] — rows are disjoint, no races.
static void FillScreenSpaceRow(int wsY, int epoch)
{
	const uint lmax_A8L8 = 255;
	const uint A8L8_SCALE = 17;

	auto L = [](int r, int c) -> int {
		if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
		if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
		return (uchar)LightMap[r][c];
	};
	auto floor64 = [](int val64, int& frac) -> int {
		if (val64 >= 0) { frac = val64 & 63; return val64 >> 6; }
		else { int q = (val64 - 63) / 64; frac = val64 - q * 64; return q; }
	};

	const int startX = Screen_LeftBorder;
	const int dy2 = 2 * wsY;
	int isoRow_64 = startX + dy2 - s_SSX0 - 2 * s_SSY0;
	int isoCol_64 = dy2 - startX - 2 * s_SSY0 + s_SSX0;
	uchar* dstRow = LightSurface + wsY * WorkingWidth;
	uchar* dstRowBase = LightSurfaceBase + wsY * WorkingWidth;
	int wsX = startX;

	while (wsX < s_SSEndX) {
		// A worker whose dispatch was abandoned (timeout + drain) must stop
		// writing as soon as a newer dispatch is published; otherwise its
		// stale row bytes land in the NEXT frame's buffers.  Segments are
		// <= 64 px, so a late resume writes at most one short patch.
		if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
			s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
			return;
		}
		int fr, fc;
		int iRow = floor64(isoRow_64, fr);
		int iCol = floor64(isoCol_64, fc);

		int v00 = L(iRow,     iCol);
		int v10 = L(iRow + 1, iCol);
		int v01 = L(iRow,     iCol + 1);
		int v11 = L(iRow + 1, iCol + 1);

		int toRow = 64 - fr;
		int toCol = fc + 1;
		int segLen = toRow < toCol ? toRow : toCol;
		int remaining = s_SSEndX - wsX;
		if (segLen > remaining) segLen = remaining;

		if (v00 == v10 && v00 == v01 && v00 == v11) {
			int val = v00 * A8L8_SCALE;
			if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
			memset(&dstRow[wsX], (uchar)val, segLen);
			memset(&dstRowBase[wsX], (uchar)val, segLen);
			wsX += segLen;
		} else {
			int val4096 = v00*(64-fr)*(64-fc) + v10*fr*(64-fc) + v01*(64-fr)*fc + v11*fr*fc;
			const int A = -v00+v10+v01-v11, delta2 = 2*A;
			int delta = (v10-v00)*(64-fc)+(v11-v01)*fc - (v01-v00)*(64-fr)-(v11-v10)*fr + A;
			for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2) {
				int val = (val4096 * A8L8_SCALE + 2048) >> 12;
				if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
				dstRow[wsX] = (uchar)val;
				dstRowBase[wsX] = (uchar)val;
			}
		}
		isoRow_64 += segLen;
		isoCol_64 -= segLen;
	}
}

// ===== Screen-space per-pixel lighting pre-fill (single-threaded) =====
// Fills LightSurface with bilinearly interpolated LightMap values for every
// visible screen pixel.  Called once per frame before the render loop.
//
// Unlike the per-cell approach (which chops the bilinear field into 64×32
// tiles and requires per-layer swaps to stitch them back), this iterates
// directly over screen pixels.  Each pixel's iso coordinate is computed
// from the global screen-to-iso projection, guaranteeing C0 continuity
// everywhere — no cell boundaries, no layer discontinuities.
//
// The bilinear math uses the same (val*17+2048)>>12 A8L8 scaling and the
// same incremental-delta optimisation as CalculatePerPixelLight.
static void FillScreenSpaceLighting(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor)
{
	if (!g_A8L8Lighting || !LightSurface || !LightSurfaceBase) return;

	const uint lmax_A8L8 = 255; // max level in A8L8 mode is always 255 (full dark) regardless of LightMax, because the shader remaps it to 0.0 brightness; this allows full 256-level smooth gradients in A8L8 mode even when LightMax is only 3 or 15.
	const uint A8L8_SCALE = lmax_A8L8 / 15; // = 17, maps LightMap [0..15] to A8L8 [0..255]

	// Compute X0, Y0: working-surface coordinates of iso origin (0,0).
	// The centre of the base cell (sx=32, ppLight row y=16) maps exactly
	// to iso (baseRow, baseCol).  Its screen position is:
	//   wsX = offsetX + 32
	//   wsY = offsetY - 31 + 16 = offsetY - 15
	const int X0 = offsetX + 32 - 32 * baseRow + 32 * baseCol;
	const int Y0 = offsetY - 15 - 16 * baseRow - 16 * baseCol;

	// Inverse projection:   isoRow·64 = wsX + 2·wsY − X0 − 2·Y0
	//                        isoCol·64 = 2·wsY − wsX − 2·Y0 + X0

	// Clamp helper — only evaluated at segment boundaries (every ~32-64 px)
	auto L = [](int r, int c) -> int {
		if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
		if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
		return (uchar)LightMap[r][c];
	};

	// Helper: floor-divide by 64 with remainder in [0,63]
	auto floor64 = [](int val64, int& frac) -> int {
		if (val64 >= 0) {
			frac = val64 & 63;
			return val64 >> 6;
		} else {
			int q = (val64 - 63) / 64;
			frac = val64 - q * 64;
			return q;
		}
	};

	const int startX = Screen_LeftBorder;
	const int startY = Screen_TopBorder;
	const int endX   = startX + ScreenWidth  / zoomFactor;
	const int endY   = startY + ScreenHeight / zoomFactor;

	for (int wsY = startY; wsY < endY; wsY++) {
		const int dy2 = 2 * wsY;

		// isoRow_64 / isoCol_64 for the first column of this row
		int isoRow_64 = startX + dy2 - X0 - 2 * Y0;
		int isoCol_64 = dy2 - startX - 2 * Y0 + X0;

		uchar* dstRow = LightSurface + wsY * WorkingWidth;
		uchar* dstRowBase = LightSurfaceBase + wsY * WorkingWidth;
		int wsX = startX;

		while (wsX < endX) {
			int fr, fc;
			int iRow = floor64(isoRow_64, fr);
			int iCol = floor64(isoCol_64, fc);

			// 2×2 LightMap neighbourhood for this segment
			int v00 = L(iRow,     iCol);
			int v10 = L(iRow + 1, iCol);
			int v01 = L(iRow,     iCol + 1);
			int v11 = L(iRow + 1, iCol + 1);

			// Segment length: how many pixels until iRow or iCol changes.
			// isoRow_64 increases by 1 per pixel → next multiple of 64 after (64-fr) steps.
			// isoCol_64 decreases by 1 per pixel → next lower multiple of 64 after (fc+1) steps.
			int toRow = 64 - fr;
			int toCol = fc + 1;
			int segLen = toRow < toCol ? toRow : toCol;
			int remaining = endX - wsX;
			if (segLen > remaining) segLen = remaining;

			// Uniform-segment fast path: when all four LightMap corners agree,
			// the bilinear blend is constant (val4096 = v00*4096, delta = 0).
			// Use memset to fill the segment in one call instead of iterating
			// pixel-by-pixel.  Numerically identical to the general loop below
			// because (v00*4096*17+2048)>>12 = v00*17 exactly (4096*17 = 69632
			// is a multiple of 4096, so the rounding term is a no-op).
			if (v00 == v10 && v00 == v01 && v00 == v11) {
				int val = v00 * A8L8_SCALE;
				if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
				memset(&dstRow[wsX], (uchar)val, segLen);
				memset(&dstRowBase[wsX], (uchar)val, segLen);
				wsX += segLen;
			}
			else {
				// Exact bilinear value * 4096 at segment start
				int val4096 = v00 * (64 - fr) * (64 - fc)
							+ v10 * fr       * (64 - fc)
							+ v01 * (64 - fr) * fc
							+ v11 * fr       * fc;

				// Incremental delta: along the scanline (fr+1, fc-1) per step,
				// val4096 is quadratic with cross-coefficient A = -v00+v10+v01-v11.
				const int A      = -v00 + v10 + v01 - v11;
				const int delta2 = 2 * A;
				int delta = (v10 - v00) * (64 - fc) + (v11 - v01) * fc
						  - (v01 - v00) * (64 - fr) - (v11 - v10) * fr + A;

				for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2) {
					int val = (val4096 * A8L8_SCALE + 2048) >> 12;
					if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
					dstRow[wsX] = (uchar)val;
					dstRowBase[wsX] = (uchar)val;
				}
			}

			// Advance iso coordinates for the next segment
			isoRow_64 += segLen;
			isoCol_64 -= segLen;
		}
	}
}

// Exact 32-bit integer square root (floor).  Only ever called with n <= 2^21
// in the V2 pre-fill, but the leading power-of-4 alignment keeps it exact for
// any n < 2^30.  No floats, no division in the per-pixel path.
static inline unsigned Isqrt32(unsigned n)
{
	unsigned res = 0;
	unsigned bit = 1u << 30;
	while (bit > n) bit >>= 2;
	while (bit) {
		if (n >= res + bit) {
			n -= res + bit;
			res = (res >> 1) + bit;
		} else {
			res >>= 1;
		}
		bit >>= 2;
	}
	return res;
}

// ===== Screen-space per-pixel lighting pre-fill V2 (A8L8 per-pixel light field) =====
// Phase 2 of doc\PixelLight_A8L8_PerPixelField_ImplementationPlan.md §3.2-§3.4.
//
// Ambient: bilinear interpolation of StaticLightMap (dungeons) / LightMap
// (town or FullLighting) — exactly the same math as FillScreenSpaceLighting.
// On top of that, EVERY non-off LightList light is stamped per pixel with its
// continuous radial falloff taken from g_A8L8LightBright (1/64-tile linear
// distance LUT), using min() semantics — the per-pixel analogue of
// DoLighting's per-tile min() stamps.  Both LightSurface and LightSurfaceBase
// receive ambient + stamps, matching today's pre-fill invariants.
//
// Light centers (§3.3):
//   - current player's light: centerRow_64 = row*64 + xOfs + 2*yOfs,
//     centerCol_64 = col*64 + 2*yOfs - xOfs — the exact (interpolated)
//     sprite position, no >>3 quantization, no dead-zone (Bug 2 fix).
//   - all other lights: row*64 + deltaRow*8, col*64 + deltaCol*8
//     (placement offsets; static lights have 0/0 via AddLight).
//
// ---- exact frame-coherence cache (Rev 20) ----
// The filled region of LightSurface/LightSurfaceBase is a pure function
// of the inputs keyed below (camera projection, light snapshot, ambient
// maps, falloff LUT, config).  When every input matches the last fill,
// the pristine copy taken right after that fill is restored into the
// buffers — they then hold EXACTLY the bytes a fresh fill would produce
// at this point in the frame.  Overlay code (panel, info window, HP
// bars, chat...) may have cleared regions in between; restoring
// overwrites those just as a full recompute would.  A validity check,
// not an approximation: byte-identical to recomputing, at memcpy cost.
static bool         cacheValid = false;
static int          cacheBaseRow, cacheBaseCol, cacheOffsetX, cacheOffsetY;
static int          cacheZoom, cacheDun, cacheFullLighting;
static int          cacheScreenW, cacheScreenH, cacheWorkingW;
static int          cacheBorderL, cacheBorderT;
static int          cacheStampCount;
static int          cacheCenR[Lights_128_Count], cacheCenC[Lights_128_Count], cacheD64[Lights_128_Count], cacheRad[Lights_128_Count];
// (th4) Stage 3: per-stamp tint in the cache key, plus the tint field
// snapshot — a cache hit skips the fill, so the tint bytes must be
// restored the same way the light bytes are.
static int          cacheTintR[Lights_128_Count], cacheTintG[Lights_128_Count], cacheTintB[Lights_128_Count];
static unsigned char cacheLightMap[FineMap_112 * FineMap_112];
static unsigned char cacheStaticMap[FineMap_112 * FineMap_112];
static uint          cacheStaticTint[FineMap_112 * FineMap_112]; // (th4) Rev 2.46 Option C: the colored static bake map (changes only at level load)
static unsigned char cacheLut[16 * 1025];
static std::vector<uchar> cacheSurf;
static std::vector<uchar> cacheSurfBase;
static std::vector<uint> cacheTintSurf;

// Snapshot of all non-off LightList lights (Rev 24: shared by ST and MT).
static void V2BuildStamps(V2Stamp* stamps, int& stampCount)
{
	stampCount = 0;
	const Player& player = Players[CurrentPlayerIndex];
	const int playerLightIndex = player.LightIndex;
	for (int i = 0; i < LightsCount && stampCount < Lights_128_Count; i++) {
		const int l = LightIndexes[i];
		if (l < 0 || l >= Lights_128_Count) continue;
		const Light& light = LightList[l];
		if (light.off) continue;
		int rad = light.radius;
		if ((uint)rad > 15) rad = 15;
		int centerRow64, centerCol64;
		if (l == playerLightIndex) {
			// Interpolated per-frame center: concentric with the drawn
			// sprite every rendered frame (xOfs/yOfs already include the
			// MotionInterpolationBegin shift at this point).
			centerRow64 = light.row * 64 + player.xOfs + 2 * player.yOfs;
			centerCol64 = light.col * 64 + 2 * player.yOfs - player.xOfs;
		} else {
			centerRow64 = light.row * 64 + light.deltaRow * 8;
			centerCol64 = light.col * 64 + light.deltaCol * 8;
		}
		const uchar* lut = g_A8L8LightBright[rad];
		// Effective support radius: largest d (1/64 units) with B > 0.
		// LUT rows are monotone non-increasing, so binary search.
		int lo = 0, hi = 1025;
		while (lo + 1 < hi) {
			const int mid = (lo + hi) >> 1;
			if (lut[mid] > 0) lo = mid; else hi = mid;
		}
		V2Stamp& st = stamps[stampCount++];
		st.centerRow64 = centerRow64;
		st.centerCol64 = centerCol64;
		st.D64 = lo;
		st.D64sq = lo * lo;
		st.rad = rad;
		st.lut = lut;
		// (th4) Stage 3: winner tint follows the light (white = no tint).
		st.tintR = LightTintList[l].r;
		st.tintG = LightTintList[l].g;
		st.tintB = LightTintList[l].b;
	}
	// (th4) Stage 3 (Rev 2.26): stamp WHITE lights before colored lights
	// (stable partition — whites and coloreds each keep their creation
	// order). A colored light that wins against a white competitor then
	// measures its win margin against the white light's already-stamped
	// value, so light-interaction boundaries fade smoothly to white instead
	// of hard sectors (the earlier firebolts showed full-strength red up to
	// the boundary of a later white competitor). The min() value pass is
	// order-independent — the light bytes are unchanged; only the tint
	// field becomes smooth.
	// (th4) Rev 2.27 gate: non-white tints can only exist in ultra mode with
	// an active colored light (ApplyMissileLightTint is gated on
	// g_ARGBLighting). Everywhere else every stamp is white and the stable
	// partition is a no-op, so the pre-branch stamp order — and the whole
	// pre-Stage-3 behaviour — is untouched.
	if (g_ARGBLighting && g_LightTintActive) {
		V2Stamp sorted[Lights_128_Count];
		int n = 0;
		for (int pass = 0; pass < 2; pass++) {
			for (int i = 0; i < stampCount; i++) {
				const bool isWhite = stamps[i].tintR == 255
					&& stamps[i].tintG == 255 && stamps[i].tintB == 255;
				if ((pass == 0) == isWhite) sorted[n++] = stamps[i];
			}
		}
		for (int i = 0; i < stampCount; i++) stamps[i] = sorted[i];
	}
}

// Per-frame V2 inputs (Rev 24): built once on the render thread and shared
// by the ST and MT paths — no duplicated setup between them.
struct V2FrameCtx {
	int  X0, Y0;
	int  startX, startY, endX, endY;
	bool useStaticAmbient;
	bool zeroAmbient;
	V2Stamp stamps[Lights_128_Count];
	int  stampCount;
};

static void V2BuildFrame(V2FrameCtx& f, int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun)
{
	f.startX = Screen_LeftBorder;
	f.startY = Screen_TopBorder;
	f.endX   = f.startX + ScreenWidth  / zoomFactor;
	f.endY   = f.startY + ScreenHeight / zoomFactor;
	// X0, Y0: working-surface coordinates of iso origin (0,0) — identical to V1.
	f.X0 = offsetX + 32 - 32 * baseRow + 32 * baseCol;
	f.Y0 = offsetY - 15 - 16 * baseRow - 16 * baseCol;
	f.useStaticAmbient = dun && !FullLighting;
	f.zeroAmbient = false;
	V2BuildStamps(f.stamps, f.stampCount);
}

// Cache check.  Returns true when every keyed input matches the stored entry.
// ignoreProjection (translation probe only): skips the four projection terms
// (baseRow/baseCol/offsetX/offsetY) so a pure camera move can match the key;
// the translation path then validates and applies the exact pixel delta
// (dX0, dY0) itself.  The plain cache-hit path keeps the full key.
static bool V2CacheKeyMatches(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun,
	const V2Stamp* stamps, int stampCount, bool ignoreProjection = false)
{
	if (!cacheValid
		|| cacheZoom != zoomFactor
		|| cacheDun != (int)dun
		|| cacheFullLighting != FullLighting
		|| cacheScreenW != ScreenWidth || cacheScreenH != ScreenHeight
		|| cacheWorkingW != WorkingWidth
		|| cacheBorderL != Screen_LeftBorder || cacheBorderT != Screen_TopBorder
		|| cacheStampCount != stampCount) return false;
	if (!ignoreProjection
		&& (cacheBaseRow != baseRow || cacheBaseCol != baseCol
		 || cacheOffsetX != offsetX || cacheOffsetY != offsetY)) return false;
	for (int i = 0; i < stampCount; i++) {
		if (cacheCenR[i] != stamps[i].centerRow64
			|| cacheCenC[i] != stamps[i].centerCol64
			|| cacheD64[i] != stamps[i].D64
			|| cacheRad[i] != stamps[i].rad
			// (th4) Stage 3: a changed tint must invalidate the cache — the
			// tint field is part of the fill output.
			|| cacheTintR[i] != stamps[i].tintR
			|| cacheTintG[i] != stamps[i].tintG
			|| cacheTintB[i] != stamps[i].tintB) return false;
	}
	return memcmp(cacheLightMap, LightMap, sizeof(cacheLightMap)) == 0
		&& memcmp(cacheStaticMap, StaticLightMap, sizeof(cacheStaticMap)) == 0
		// (th4) Rev 2.46: the colored static bake map only changes at level load
		// and is only read under ultra — gate the 50 KB memcmp so the pre-branch
		// path keeps its exact per-frame cost.
		&& ( !g_ARGBLighting || memcmp(cacheStaticTint, StaticTintMap, sizeof(cacheStaticTint)) == 0 )
		&& memcmp(cacheLut, g_A8L8LightBright, sizeof(cacheLut)) == 0;
}

// Cache check + restore.  Returns true (and restores) on a hit.
static bool V2CacheHit(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun,
	const V2Stamp* stamps, int stampCount, int startY, int endY)
{
	const size_t regionSize = (size_t)(endY - startY) * (size_t)WorkingWidth;
	if (regionSize > 0 && cacheSurf.size() >= regionSize
	 && V2CacheKeyMatches(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun, stamps, stampCount)) {
		memcpy(LightSurface + (size_t)startY * WorkingWidth, cacheSurf.data(), regionSize);
		memcpy(LightSurfaceBase + (size_t)startY * WorkingWidth, cacheSurfBase.data(), regionSize);
		// (th4) Stage 3: restore the tint field too — a hit skips the fill,
		// so LightTintSurface would keep stale content otherwise. Skipped
		// under the idle gate (no colored light active — nothing reads the
		// field this frame).
		if (g_LightTintActive && LightTintSurface && cacheTintSurf.size() >= regionSize)
			memcpy(LightTintSurface + (size_t)startY * WorkingWidth, cacheTintSurf.data(), regionSize * sizeof(uint));
		return true;
	}
	return false;
}

// Store the cache key and the pristine snapshot of the just-filled region.
static void V2CacheStore(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun,
	const V2Stamp* stamps, int stampCount, int startY, int endY)
{
	const size_t regionSize = (size_t)(endY - startY) * (size_t)WorkingWidth;
	cacheValid = true;
	cacheBaseRow = baseRow; cacheBaseCol = baseCol;
	cacheOffsetX = offsetX; cacheOffsetY = offsetY;
	cacheZoom = zoomFactor;
	cacheDun = (int)dun;
	cacheFullLighting = FullLighting;
	cacheScreenW = ScreenWidth; cacheScreenH = ScreenHeight;
	cacheWorkingW = WorkingWidth;
	cacheBorderL = Screen_LeftBorder; cacheBorderT = Screen_TopBorder;
	cacheStampCount = stampCount;
	for (int i = 0; i < stampCount; i++) {
		cacheCenR[i] = stamps[i].centerRow64;
		cacheCenC[i] = stamps[i].centerCol64;
		cacheD64[i] = stamps[i].D64;
		cacheRad[i] = stamps[i].rad;
		cacheTintR[i] = stamps[i].tintR;
		cacheTintG[i] = stamps[i].tintG;
		cacheTintB[i] = stamps[i].tintB;
	}
	memcpy(cacheLightMap, LightMap, sizeof(cacheLightMap));
	memcpy(cacheStaticMap, StaticLightMap, sizeof(cacheStaticMap));
	if (g_ARGBLighting) memcpy(cacheStaticTint, StaticTintMap, sizeof(cacheStaticTint)); // (th4) Rev 2.46: gated with the key compare above
	memcpy(cacheLut, g_A8L8LightBright, sizeof(cacheLut));
	if (regionSize > 0) {
		if (cacheSurf.size() < regionSize) cacheSurf.resize(regionSize);
		if (cacheSurfBase.size() < regionSize) cacheSurfBase.resize(regionSize);
		memcpy(cacheSurf.data(), LightSurface + (size_t)startY * WorkingWidth, regionSize);
		memcpy(cacheSurfBase.data(), LightSurfaceBase + (size_t)startY * WorkingWidth, regionSize);
		// (th4) Stage 3: snapshot the tint field for byte-identical restore.
		// Skipped under the idle gate; a later colored frame always
		// invalidates the key (white->colored tint compare) and resnapshots.
		if (g_LightTintActive && LightTintSurface) {
			if (cacheTintSurf.size() < regionSize) cacheTintSurf.resize(regionSize);
			memcpy(cacheTintSurf.data(), LightTintSurface + (size_t)startY * WorkingWidth, regionSize * sizeof(uint));
		}
	}
	// Rev 27 hardening: a stale worker that resumed during the snapshot wrote
	// bytes a later recompute would not reproduce.  The epoch-abort flags
	// s_PPLStaleAborts before it writes, so a non-zero flag means the entry
	// cannot be trusted — invalidate so the next identical-inputs frame
	// recomputes (self-healing).  A stale write AFTER this check touches only
	// live buffers (one-frame patch); the entry stays clean.  Every store path
	// (ST fill, translation hit, MT dispatch) is protected by this one check.
	// After consuming the flag, clear it: the only other reset site is the
	// V2 MT publish block, so in an ST-only session (pool broken, MT off) a
	// latched flag would otherwise invalidate every store and permanently
	// disable the frame cache.  Clearing is safe — an abort flagged after the
	// check belongs to a stale write that happens after the snapshot, which
	// cannot taint the stored entry (the next frame fully rewrites the region
	// on hit, translation or recompute).
	if (s_PPLStaleAborts.load(std::memory_order_acquire) != 0) {
		cacheValid = false;
		s_PPLStaleAborts.store(0, std::memory_order_relaxed);
	}
}

// (th4) item 7 refactor: one translated row of the cached field — the exact
// per-row logic V2TranslationHit used inline. srcY inside the cached range:
// x-shifted overlap copy + sliver fill via the same row kernel; otherwise the
// whole row is filled. Shared by the translation hit (all rows) and the band
// fill (clean rows only — a clean row contains no changed-stamp influence, so
// the same translation algebra applies per row).
static void V2ShiftCopyRow(const V2FrameCtx& f, int y, int dX0, int dY0, int ax)
{
	const int startX = f.startX, startY = f.startY, endX = f.endX, endY = f.endY;
	const int W = WorkingWidth;
	const int w = endX - startX;
	const int srcY = y - dY0;
	if (srcY < startY || srcY >= endY) {
		FillScreenSpaceRowV2(y, f.X0, f.Y0, startX, endX,
			f.useStaticAmbient, f.zeroAmbient, f.stamps, f.stampCount, -1);
		return;
	}
	int xCopyDst, xCopySrc, xCopyLen, xSliverStart, xSliverEnd;
	if (dX0 >= 0) {
		xCopyDst = startX + ax; xCopySrc = startX; xCopyLen = w - ax;
		xSliverStart = startX;  xSliverEnd = startX + ax;
	} else {
		xCopyDst = startX; xCopySrc = startX + ax; xCopyLen = w - ax;
		xSliverStart = endX - ax; xSliverEnd = endX;
	}
	const size_t srcOff = (size_t)(srcY - startY) * (size_t)W;
	memcpy(LightSurface + (size_t)y * W + xCopyDst, cacheSurf.data() + srcOff + xCopySrc, (size_t)xCopyLen);
	memcpy(LightSurfaceBase + (size_t)y * W + xCopyDst, cacheSurfBase.data() + srcOff + xCopySrc, (size_t)xCopyLen);
	// (th4) Stage 3: the tint field translates exactly like the light field
	// (same pure function of the iso coordinates).
	if (g_LightTintActive && LightTintSurface)
		memcpy(LightTintSurface + (size_t)y * W + xCopyDst,
			cacheTintSurf.data() + srcOff + xCopySrc, (size_t)xCopyLen * sizeof(uint));
	if (xSliverStart < xSliverEnd)
		FillScreenSpaceRowV2(y, f.X0, f.Y0, xSliverStart, xSliverEnd,
			f.useStaticAmbient, f.zeroAmbient, f.stamps, f.stampCount, -1);
}

// Rev 27 (§4.1): translation reuse for the exact frame cache.  The filled
// field is a pure function of the iso coordinates, so when only the
// projection constants move by (dX0, dY0) and every other input (light
// snapshot, maps, LUT, dimensions) is unchanged, the whole field translates
// in screen space by exactly (-dX0, -dY0) pixels:
//   isoRow64_new(x,y) = isoRow64_old(x-dX0, y-dY0), likewise isoCol64
// (follows from isoRow64 = x+2y-X0-2Y0, isoCol64 = 2y-x-2Y0+X0).  The shift
// is by integer pixels, so the previously computed samples are reused with
// no resampling — byte-identical by construction.  Restores the shifted
// overlap from the cached snapshot and fills only the exposed slivers/rows
// with the SAME row function (epoch -1, main thread only).  Returns false
// when the key does not match or there is no overlap to reuse.
static bool V2TranslationHit(const V2FrameCtx& f, int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun)
{
	// Projection-free key: only the camera may have moved.  Every other input
	// (light snapshot, ambient maps, LUT, dimensions, zoom) must be identical
	// or the translation is rejected and the caller does a full recompute.
	if (!V2CacheKeyMatches(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun, f.stamps, f.stampCount, true))
		return false;
	const int startX = f.startX, endX = f.endX, startY = f.startY, endY = f.endY;
	const int W = WorkingWidth;
	const int w = endX - startX, h = endY - startY;
	if (w <= 0 || h <= 0) return false;
	const int dX0 = (offsetX + 32 - 32 * baseRow + 32 * baseCol)
	              - (cacheOffsetX + 32 - 32 * cacheBaseRow + 32 * cacheBaseCol);
	const int dY0 = (offsetY - 15 - 16 * baseRow - 16 * baseCol)
	              - (cacheOffsetY - 15 - 16 * cacheBaseRow - 16 * cacheBaseCol);
	const int ax = dX0 < 0 ? -dX0 : dX0;
	const int ay = dY0 < 0 ? -dY0 : dY0;
	if (ax >= w || ay >= h) return false; // no overlap in one dimension
	if (cacheSurf.size() < (size_t)h * (size_t)W) return false; // defensive; sized on store
	// (th4) Stage 3: the tint field must translate too — reject the hit if
	// its snapshot was not stored with the light snapshot (fall back to a
	// full recompute, which refills both). Idle gate: no colored light
	// active -> nothing to translate.
	if (g_LightTintActive && LightTintSurface && cacheTintSurf.size() < (size_t)h * (size_t)W) return false;

	// Vertical: F_new(y) = F_old(y - dY0).  Rows whose source row lies inside
	// the cached range get the shifted copy; exposed rows are filled whole.
	// (th4) item 7 refactor: the per-row shift+sliver logic lives in
	// V2ShiftCopyRow (shared with the band fill's clean rows).
	for (int y = startY; y < endY; y++)
		V2ShiftCopyRow(f, y, dX0, dY0, ax);
	return true;
}

// (th4) Stage 4 item 7 (Rev 2.59): changed-stamp band fill shared state.
// Row mask of the band path: dirty[row - startY] != 0 -> recompute the row.
static std::vector<uchar> s_BandDirty;

// Non-stamp, projection-free key base (item 7): identical inputs except the
// camera terms and possibly some stamps. Mirrors V2CacheKeyMatches without
// the stamp loop and the projection compare.
static bool V2BandBaseKeyMatches(int zoomFactor, bool dun)
{
	if (!cacheValid
		|| cacheZoom != zoomFactor
		|| cacheDun != (int)dun
		|| cacheFullLighting != FullLighting
		|| cacheScreenW != ScreenWidth || cacheScreenH != ScreenHeight
		|| cacheWorkingW != WorkingWidth
		|| cacheBorderL != Screen_LeftBorder || cacheBorderT != Screen_TopBorder) return false;
	return memcmp(cacheLightMap, LightMap, sizeof(cacheLightMap)) == 0
		&& memcmp(cacheStaticMap, StaticLightMap, sizeof(cacheStaticMap)) == 0
		// (th4) Rev 2.46: gate the colored static bake compare exactly like the
		// full key (non-ultra keeps its exact per-frame cost).
		&& (!g_ARGBLighting || memcmp(cacheStaticTint, StaticTintMap, sizeof(cacheStaticTint)) == 0)
		&& memcmp(cacheLut, g_A8L8LightBright, sizeof(cacheLut)) == 0;
}

// (th4) Stage 4 item 7: dispatch one contiguous dirty-row span over the PPL
// pool (mode 2). The caller resets s_PPLStaleAborts ONCE before the first
// span so a stale write during any span is still flagged at cache-store time;
// claimed/done are reset here per span (each span is a complete dispatch).
// Mirrors the pre-item-7 single dispatch exactly.
static void V2FillRowsMT(const V2FrameCtx& f, int rowStart, int rowCount)
{
	s_V2X0 = f.X0; s_V2Y0 = f.Y0;
	s_V2StartX = f.startX; s_V2EndX = f.endX;
	s_V2UseStaticAmbient = f.useStaticAmbient;
	s_V2ZeroAmbient = f.zeroAmbient;
	s_V2StampCount = f.stampCount;
	for (int i = 0; i < f.stampCount; i++) s_V2Stamps[i] = f.stamps[i];
	s_V2RowStart = rowStart;
	s_V2RowCount = rowCount;
	// Publish mode + counters + frameId under s_PPLWorkMtx so the whole
	// dispatch state is atomic w.r.t. the workers' claim sections (M2 fix).
	{
		std::lock_guard<std::mutex> lk(s_PPLWorkMtx);
		s_SSMode = 2;
		s_PPLClaimed.store(0, std::memory_order_relaxed);
		s_PPLDone.store(0,    std::memory_order_relaxed);
		s_PPLTotal.store(rowCount, std::memory_order_relaxed);
		s_PPLFrameId.fetch_add(1, std::memory_order_relaxed);
	}
	s_PPLCvWork.notify_all();

	// Main thread also work-steals rows to reduce tail latency.
	bool allDone = false;
	for (;;) {
		int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
		if (rowIdx >= rowCount) break;
		FillScreenSpaceRowV2(s_V2RowStart + rowIdx, s_V2X0, s_V2Y0, s_V2StartX, s_V2EndX,
			s_V2UseStaticAmbient, s_V2ZeroAmbient, s_V2Stamps, s_V2StampCount, -1);
		if (s_PPLDone.fetch_add(1, std::memory_order_acq_rel) + 1 >= rowCount) {
			allDone = true;
			s_PPLTimeoutCount = 0;
			break;
		}
	}

	// Wait for workers to finish remaining rows — with a timeout so a
	// dead/stalled worker can never deadlock the game; on timeout the main
	// thread drains the remaining rows single-threaded (same row code —
	// output stays byte-identical).
	if (!allDone) {
		std::unique_lock<std::mutex> lk(s_PPLDoneMtx);
		const bool completed = s_PPLCvDone.wait_for(lk, std::chrono::milliseconds(PPL_DISPATCH_TIMEOUT_MS), [&]{
			return s_PPLDone.load(std::memory_order_acquire) >= rowCount;
		});
		if (!completed) {
			static bool logged = false;
			if (!logged) {
				logged = true;
				OutputDebugString("PPL TIMEOUT (V2 band span): draining remaining rows single-threaded\n");
			}
			for (;;) {
				int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
				if (rowIdx >= rowCount) break;
				FillScreenSpaceRowV2(s_V2RowStart + rowIdx, s_V2X0, s_V2Y0, s_V2StartX, s_V2EndX,
					s_V2UseStaticAmbient, s_V2ZeroAmbient, s_V2Stamps, s_V2StampCount, -1);
				s_PPLDone.fetch_add(1, std::memory_order_acq_rel);
			}
			PPLNoteTimeout(); // M1: repeated timeouts disable MT for the session
		} else {
			s_PPLTimeoutCount = 0;
		}
	}
}

// (th4) Stage 4 item 7: changed-stamp band fill. The frame-cache hit and the
// Rev 27 translation hit both reject on ANY stamp change, so moving lights
// (missiles) force a full-screen recompute every frame. A stamp contributes
// nothing outside its disc (B == 0 is a no-op for both the min() light byte
// and TintAccumulate), so rows disjoint from the union of every CHANGED
// stamp's old+new discs are byte-identical to the shifted cached field:
// those rows are memcpy'd from the cache (shifted by the camera delta,
// exactly like V2TranslationHit), and only the dirty rows run the exact
// FillScreenSpaceRowV2 kernel. Changed-stamp detection is conservative
// (index-based compare over-marks, never under-marks — every output-affecting
// field is compared). The disc row test is the row setup's own predicate:
// s(y) = 4y - 4*Y0 - cr - cc, a disc hits row y iff s(y)^2 <= 2*D64sq
// (dr + dc is x-independent); old discs are evaluated with the CURRENT Y0
// because the old field shifts by the camera delta. Returns true when the
// band path completed (all rows hold the exact new-frame bytes); false to
// fall through to the translation / full-recompute paths.
static bool V2BandHit(V2FrameCtx& f, int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun, bool useMT)
{
	if (!V2BandBaseKeyMatches(zoomFactor, dun)) return false;
	if (f.zeroAmbient) return false; // all-zero field: the full path is memsets
	const int startX = f.startX, startY = f.startY, endX = f.endX, endY = f.endY;
	const int W = WorkingWidth;
	const int w = endX - startX, h = endY - startY;
	if (w <= 0 || h <= 0) return false;
	if (cacheSurf.size() < (size_t)h * (size_t)W) return false;
	// (th4) Stage 3: a tint-active frame needs the tint snapshot to shift-copy
	// clean rows (same rejection as the translation path — the full recompute
	// refills both fields).
	if (g_LightTintActive && LightTintSurface && cacheTintSurf.size() < (size_t)h * (size_t)W) return false;

	// Changed-stamp detection by INDEX. Index shifts (a light going off, the
	// white/colored stable partition reordering) mark EXTRA stamps changed —
	// never fewer, because every output-affecting field is compared.
	bool dirtyNew[Lights_128_Count], dirtyOld[Lights_128_Count];
	int nDirty = 0;
	const int cmpCount = f.stampCount < cacheStampCount ? f.stampCount : cacheStampCount;
	for (int i = 0; i < cmpCount; i++) {
		const bool same = cacheCenR[i] == f.stamps[i].centerRow64
			&& cacheCenC[i] == f.stamps[i].centerCol64
			&& cacheD64[i] == f.stamps[i].D64
			&& cacheRad[i] == f.stamps[i].rad
			&& cacheTintR[i] == f.stamps[i].tintR
			&& cacheTintG[i] == f.stamps[i].tintG
			&& cacheTintB[i] == f.stamps[i].tintB;
		dirtyNew[i] = dirtyOld[i] = !same;
		if (!same) nDirty++;
	}
	for (int i = cmpCount; i < f.stampCount; i++) { dirtyNew[i] = true; dirtyOld[i] = false; nDirty++; }
	for (int i = cmpCount; i < cacheStampCount; i++) { dirtyNew[i] = false; dirtyOld[i] = true; nDirty++; }
	if (nDirty == 0) return false; // pure camera move: the translation path handles it

	// Camera delta (same formulas as V2TranslationHit).
	const int dX0 = (offsetX + 32 - 32 * baseRow + 32 * baseCol)
	              - (cacheOffsetX + 32 - 32 * cacheBaseRow + 32 * cacheBaseCol);
	const int dY0 = (offsetY - 15 - 16 * baseRow - 16 * baseCol)
	              - (cacheOffsetY - 15 - 16 * cacheBaseRow - 16 * cacheBaseCol);
	const int ay = dY0 < 0 ? -dY0 : dY0;
	const int ax = dX0 < 0 ? -dX0 : dX0;
	if (ax >= w || ay >= h) return false; // no copy reuse: the full path is one dispatch

	// Dirty row mask: rows whose shifted copy source leaves the cached row
	// range must recompute; changed discs mark the rest.
	if (s_BandDirty.size() < (size_t)h) s_BandDirty.resize(h);
	memset(s_BandDirty.data(), 0, (size_t)h);
	for (int y = startY; y < endY; y++)
		if (y - dY0 < startY || y - dY0 >= endY) s_BandDirty[y - startY] = 1;
	{
		auto markDisc = [&](long long cr, long long cc, long long D64) {
			const long long maxS2 = 2LL * D64 * D64;    // D64 <= 1025 -> ~2.1M, fits
			const long long s0 = 4LL * f.Y0 + cr + cc;  // s(y) = 4y - s0
			const int maxS = (int)Isqrt32((unsigned)maxS2);
			int yLo = (int)((s0 - maxS) / 4) - 1;
			int yHi = (int)((s0 + maxS) / 4) + 1;
			if (yLo < startY) yLo = startY;
			if (yHi > endY - 1) yHi = endY - 1;
			for (int y = yLo; y <= yHi; y++) {
				const long long s = 4LL * y - s0;
				if (s * s <= maxS2) s_BandDirty[y - startY] = 1;
			}
		};
		for (int i = 0; i < f.stampCount; i++)
			if (dirtyNew[i])
				markDisc(f.stamps[i].centerRow64, f.stamps[i].centerCol64, f.stamps[i].D64);
		for (int i = 0; i < cacheStampCount; i++)
			if (dirtyOld[i])
				markDisc(cacheCenR[i], cacheCenC[i], cacheD64[i]);
	}

	// Clean rows: the same per-row translation algebra as V2TranslationHit
	// (x-shifted overlap copy + sliver fill) — a clean row contains no
	// changed-stamp influence, so the shift invariance holds per row.
	for (int y = startY; y < endY; y++)
		if (!s_BandDirty[y - startY])
			V2ShiftCopyRow(f, y, dX0, dY0, ax);

	// Dirty rows: the exact row kernel — ST, or dispatched as contiguous
	// spans over the pool. ~100% dirty degenerates to one span = the
	// pre-item-7 dispatch, so the band path can never regress.
	const bool mtOk = useMT && !s_PPLPoolBroken;
	if (mtOk) PPLPoolInit();
	if (mtOk && s_PPLWorkerCnt > 0) {
		s_PPLStaleAborts.store(0, std::memory_order_relaxed); // once for all spans
		int spanStart = -1;
		for (int y = startY; y <= endY; y++) {
			const bool dirty = y < endY && s_BandDirty[y - startY] != 0;
			if (dirty && spanStart < 0) spanStart = y;
			if (!dirty && spanStart >= 0) {
				V2FillRowsMT(f, spanStart, y - spanStart);
				spanStart = -1;
			}
		}
		// Rev 27 hardening: expire worker epochs after the last span (same as
		// the single-dispatch path).
		{ std::lock_guard<std::mutex> lk(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
	} else {
		for (int y = startY; y < endY; y++)
			if (s_BandDirty[y - startY])
				FillScreenSpaceRowV2(y, f.X0, f.Y0, startX, endX, f.useStaticAmbient, f.zeroAmbient, f.stamps, f.stampCount, -1);
	}
	return true;
}

// Full-bright detection (Rev 22): all-zero ambient map -> memset fast path.
static bool V2ScanZeroAmbient(bool useStaticAmbient)
{
	if (useStaticAmbient) {
		for (int r = 0; r < FineMap_112; r++)
			for (int c = 0; c < FineMap_112; c++)
				if (StaticLightMap[r][c]) return false;
	} else {
		for (int r = 0; r < FineMap_112; r++)
			for (int c = 0; c < FineMap_112; c++)
				if (LightMap[r][c]) return false;
	}
	return true;
}

// Rev 24: ST and MT share the single row implementation below — byte-
// identical by construction.
static void FillScreenSpaceLightingV2(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun)
{
	if (!g_A8L8Lighting || !LightSurface || !LightSurfaceBase) return;

	// Max-brightness combine only (Rev 22): dst = min(dst, 255 - B).
	// The bounded-additive A8L8AdditiveLight mode was REMOVED — playtests
	// showed vast additive light pools around portals/walls, suppression
	// rings around other light sources (ambient-floor interaction) and a
	// heavy FPS cost.  This is the pre-branch max-brightness behaviour.

	// Ambient source: only in dungeons does StaticLightMap hold the base
	// darkness.  Town / FullLighting keeps today's LightMap-based fill
	// (all-zero there anyway).

	// Build the frame inputs (geometry + light snapshot) once — the same
	// builder is used by the MT path (Rev 24).
	V2FrameCtx f;
	V2BuildFrame(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun);

	// Cache validity check (Rev 20): identical inputs -> restore the exact
	// previous fill output and skip the whole recompute.  Rev 27 adds a
	// translation hit (only the projection moved) — restore the shifted
	// overlap and fill just the exposed slivers.
	if (V2CacheHit(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
		f.stamps, f.stampCount, f.startY, f.endY)) return;

	// Rev 22 full-bright detection: with an all-zero ambient map the fill is
	// plain memsets and every light stamp is a no-op (min(0, lv) == 0) — the
	// row loop below then takes the memset fast path, byte-identically.
	f.zeroAmbient = V2ScanZeroAmbient(f.useStaticAmbient);

	// (th4) Stage 4 item 7: changed-stamp band fill (ST variant — this path
	// runs when MT is off or the pool is broken).
	if (V2BandHit(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun, false)) {
		V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
			f.stamps, f.stampCount, f.startY, f.endY);
		return;
	}

	if (!V2TranslationHit(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun)) {
		for (int wsY = f.startY; wsY < f.endY; wsY++)
			FillScreenSpaceRowV2(wsY, f.X0, f.Y0, f.startX, f.endX, f.useStaticAmbient, f.zeroAmbient, f.stamps, f.stampCount, -1);
	}

	// Rev 20: pristine snapshot of the just-filled region for the
	// frame-coherence cache checked at the top of the next call.
	V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
		f.stamps, f.stampCount, f.startY, f.endY);
}

// (th4) Rev 2.44: per-pixel tint blend weight table, indexed [oldMass][B]:
// w = (B * 255 + (mass + B) / 2) / (mass + B) — the integer-rounded weight of
// an incoming brightness-B light against the accumulated brightness mass.
// Replaces the per-pixel 32-bit division in
// writeTintPixel: the multi-light average now covers colored-stamp x
// player-lit overlap regions (up to near-fullscreen for a radius-15 portal),
// so the div moved from "rare colored-vs-colored overlap pixels" into the
// hot path. The table is filled with the exact integer expression, so a
// lookup equals the division for every reachable (mass, B) pair — bit-exact
// by construction (the debug self-check below and
// tools/v2_stamp_identity.cpp verify this). (th4) Rev 2.45: the table serves
// masses 0..255 only; the true-mass path (m > 255, deep-overlap cores)
// takes the same expression via one 32-bit idiv.
static uchar g_TintWeightLut[256][256]; // [oldMass 0..255][B 0..255]
namespace {
	struct TintWeightLutInit {
		TintWeightLutInit() {
			for (int m = 0; m <= 255; m++) {
				for (int B = 0; B <= 255; B++) {
					uint w = 0;
					if (m > 0 && B > 0) {
						const uint sum = (uint)m + (uint)B; // 2..510
						w = ((uint)B * 255 + (sum >> 1)) / sum;
					}
					g_TintWeightLut[m][B] = (uchar)w;
				}
			}
#ifdef _DEBUG
			// One-time self-check: the table must equal the live formula.
			for (int m = 1; m <= 255; m++) {
				for (int B = 1; B <= 255; B++) {
					const uint sum = (uint)m + (uint)B;
					const uint ref = ((uint)B * 255 + (sum >> 1)) / sum;
					if (g_TintWeightLut[m][B] != (uchar)ref) __debugbreak();
				}
			}
#endif
		}
	} _tintWeightLutInit;
}

// (th4) P3: single true-mass tint mix entry point, shared by the dynamic
// stamps and the static colored-bake ambient pass. Byte-identical to the
// former writeTintPixel lambda (Rev 2.44/2.45): B == 0 is a no-op, the first
// contributor over an A == 0 (white) word is a plain replace, and any later
// contributor blends into the running ΣBC/ΣB true-mass average with the
// saturated A byte = min(255, ΣB).
static inline void TintAccumulate(uint* tintRow, ushort* massRow, int px, uint word, uint B)
{
	if (B == 0) return;                          // outside the light's disc
	const uint old = tintRow[px];
	const uint oldA = old >> 24;                 // saturated mass (A byte)
	if (oldA == 0) {                             // first contributor over the white pre-fill
		tintRow[px] = word | (B << 24);
		massRow[px] = (ushort)B;                 // seed the true mass
	} else {
		const uint m = massRow[px];              // TRUE accumulated brightness mass
		const uint sum = m + B;                  // ≤ 8160 + 255
		const uint w = (m <= 255)
			? g_TintWeightLut[m][B]              // (B*255 + (m+B)/2) / (m+B)
			: ((B * 255 + (sum >> 1)) / sum);    // true-mass idiv (deep overlap only)
		const uint wr = (word >> 16) & 0xFF, wg = (word >> 8) & 0xFF, wb = word & 0xFF;
		const uint orr = (old >> 16) & 0xFF, og = (old >> 8) & 0xFF, ob = old & 0xFF;
		const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
		const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
		const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
		const uint na = (sum > 255) ? 255 : sum; // shader strength saturates
		tintRow[px] = nb | (ng << 8) | (nr << 16) | (na << 24);
		massRow[px] = (ushort)sum;
	}
}

// ===== V2 single row (shared by ST and MT — Rev 24) =====
// Identical math to the original V2 row loop.  Micro-optimisations:
//   - __restrict on the row pointers;
//   - envelope pair early-out for disjoint intervals (a competitor whose
//     disc does not touch this light's interval can never constrain it);
//   - Rev 27 (§4.2): envelope pairs skip the idiv when the crossing is
//     provably outside the overlap (conservative — byte-identical);
//   - Rev 27 (§4.3): single-buffer stamping — LightSurfaceBase gets the same
//     bytes via one row-end copy at the end of the function (byte-identical).
static void FillScreenSpaceRowV2(int wsY, int X0, int Y0, int startX, int endX,
	bool useStaticAmbient, bool zeroAmbient, const V2Stamp* stamps, int stampCount, int epoch)
{
	const uint lmax_A8L8 = 255; // max level in A8L8 mode is always 255 (full dark) regardless of LightMax
	const uint A8L8_SCALE = lmax_A8L8 / 15; // = 17, maps LightMap [0..15] to A8L8 [0..255]

	auto L = [useStaticAmbient](int r, int c) -> int {
		if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
		if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
		return useStaticAmbient ? (uchar)StaticLightMap[r][c] : (uchar)LightMap[r][c];
	};

	// Helper: floor-divide by 64 with remainder in [0,63] (same as V1)
	auto floor64 = [](int val64, int& frac) -> int {
		if (val64 >= 0) {
			frac = val64 & 63;
			return val64 >> 6;
		} else {
			int q = (val64 - 63) / 64;
			frac = val64 - q * 64;
			return q;
		}
	};

	// ceil(x/2) and floor(x/2) with correct behaviour for negative x
	auto ceilDiv2  = [](int x) -> int { return x >= 0 ? (x + 1) / 2 : x / 2; };
	auto floorDiv2 = [](int x) -> int { return x >= 0 ? x / 2 : (x - 1) / 2; };

	// floor(n/d) / ceil(n/d) for any non-zero signs (d may be negative).
	// Used by the Rev 20 nearest-light envelope for half-line crossings.
	auto floorDivI = [](int n, int d) -> int {
		const int q = n / d, r = n % d;
		return (r != 0 && ((r < 0) != (d < 0))) ? q - 1 : q;
	};
	auto ceilDivI = [](int n, int d) -> int {
		const int q = n / d, r = n % d;
		return (r != 0 && ((r < 0) == (d < 0))) ? q + 1 : q;
	};

	const int dy2 = 2 * wsY;

	// isoRow_64 / isoCol_64 for the first column of this row (as in V1)
	const int rowIsoRow64 = startX + dy2 - X0 - 2 * Y0;
	const int rowIsoCol64 = dy2 - startX - 2 * Y0 + X0;

	uchar* __restrict dstRow = LightSurface + wsY * WorkingWidth;
	uchar* __restrict dstRowBase = LightSurfaceBase + wsY * WorkingWidth;
	// (th4) Stage 3: tint row (uint* indexed by PIXEL offset, same convention
	// as SpriteLayerSurface). Null outside ultra — the V2 fill also runs in
	// plain A8L8PixelField mode where the tint surface does not exist — and
	// null whenever no colored light is active (idle gate, Stage 4 item 4):
	// with all-white tints the field content is never read, so skipping the
	// pre-fill and stamp stores keeps the fill at its pre-Stage-3 cost.
	uint* __restrict tintRow = (g_LightTintActive && LightTintSurface)
		? LightTintSurface + wsY * WorkingWidth : nullptr;
	// (th4) Rev 2.45: row-scoped scratch of the TRUE brightness mass (ushort,
	// unclamped — ≤ Lights_128_Count × 255 = 32640). The blend denominator needs the
	// unclamped ΣB_i; the tint WORD's A byte keeps min(255, m) for the
	// shader. The mass is transient inside this function call (prefill ->
	// stamps) and dies at row end — nothing uploads, caches or zooms it.
	// Stack-allocated per invocation (alloca), so MT workers are isolated
	// with zero plumbing. Rev 2.44 weighted against the clamped A byte —
	// exact for ≤ 2 lights, but 3+ bright overlaps degraded smoothly toward
	// an order-dependent average; the true mass restores the exact ΣBC/ΣB
	// model. Null outside ultra / while no colored light is active (same
	// gate as tintRow).
	ushort* massRow = nullptr;
	if (tintRow) {
		massRow = (ushort*)alloca((size_t)WorkingWidth * sizeof(ushort));
		memset(massRow, 0, (size_t)WorkingWidth * sizeof(ushort));
	}

	// (th4) Stage 3: the tint row starts WHITE everywhere (A = 0 = no tint).
	// (th4) Rev 2.46 Option C + P3: a colored static bake (g_StaticTintActive)
	// then seeds the ambient as the FIRST tint contributor through the SAME
	// TintAccumulate the dynamic stamps use — rule R-AMB: white pixels keep
	// the white pre-fill (A = 0, mass 0), so dynamic x dynamic mixing stays
	// byte-identical and white-ambient levels are untouched.
	if (tintRow) {
		const __m128i white = _mm_set1_epi32((int)TINT_WHITE);
		__m128i* tp = (__m128i*)(tintRow + startX);
		int i = 0;
		for (; i + 16 <= endX - startX; i += 16) {
			_mm_storeu_si128(tp, white); ++tp;
			_mm_storeu_si128(tp, white); ++tp;
			_mm_storeu_si128(tp, white); ++tp;
			_mm_storeu_si128(tp, white); ++tp;
		}
		for (; i < endX - startX; ++i) tintRow[startX + i] = TINT_WHITE;
	}
	// (th4) P3: static-bake ambient contributor — same bilinear sampler as the
	// old pre-fill branch, but every non-white pixel enters the common
	// TintAccumulate path (first contributor over the white pre-fill). The
	// B == 0 colored case writes the rgb word directly to keep the tint field
	// byte-identical to the old branch.
	// (th4) zeroAmbient guard: a fully bright level takes the memset fast path
	// that skips dynamic stamps too — skip the bake here as well, otherwise the
	// bake's center-only tiles would tint the fully bright floor at A = 255.
	if (tintRow && g_ARGBLighting && g_StaticTintActive && useStaticAmbient && !zeroAmbient) {
		auto TL = [](int r, int c) -> uint {
			if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
			if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
			return StaticTintMap[r][c];
		};
		int isoRow_64 = rowIsoRow64;
		int isoCol_64 = rowIsoCol64;
		int wsX = startX;
		while (wsX < endX) {
			// Same cross-frame guard as the value fill: a worker resumed
			// after the timeout-drain must stop within one <=64 px segment.
			if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
				s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
				return;
			}
			int fr, fc;
			int iRow = floor64(isoRow_64, fr);
			int iCol = floor64(isoCol_64, fc);
			int v00 = L(iRow,     iCol);
			int v10 = L(iRow + 1, iCol);
			int v01 = L(iRow,     iCol + 1);
			int v11 = L(iRow + 1, iCol + 1);
			uint c00 = TL(iRow,     iCol);
			uint c10 = TL(iRow + 1, iCol);
			uint c01 = TL(iRow,     iCol + 1);
			uint c11 = TL(iRow + 1, iCol + 1);
			int toRow = 64 - fr;
			int toCol = fc + 1;
			int segLen = toRow < toCol ? toRow : toCol;
			int remaining = endX - wsX;
			if (segLen > remaining) segLen = remaining;

			if (v00 == v10 && v00 == v01 && v00 == v11
			 && c00 == c10 && c00 == c01 && c00 == c11) {
				int val = v00 * A8L8_SCALE;
				if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
				const uint cw = c00 & 0xFFFFFFu; // ignore the map word's A byte
				if (cw != TINT_WHITE) {
					const uint B = (uint)(lmax_A8L8 - val);
					if (B > 0) {
						for (int i = 0; i < segLen; i++)
							TintAccumulate(tintRow, massRow, wsX + i, cw, B);
					} else {
						for (int i = 0; i < segLen; i++) tintRow[wsX + i] = cw;
					}
				}
				wsX += segLen;
			} else {
				int val4096 = v00 * (64 - fr) * (64 - fc)
							+ v10 * fr       * (64 - fc)
							+ v01 * (64 - fr) * fc
							+ v11 * fr       * fc;
				const int A      = -v00 + v10 + v01 - v11;
				const int delta2 = 2 * A;
				int delta = (v10 - v00) * (64 - fc) + (v11 - v01) * fc
						  - (v01 - v00) * (64 - fr) - (v11 - v10) * fr + A;

				struct Chan { int val; int delta; int delta2; };
				auto MakeChan = [&](int ch00, int ch10, int ch01, int ch11) -> Chan {
					Chan ch;
					ch.val = ch00 * (64 - fr) * (64 - fc) + ch10 * fr * (64 - fc)
					       + ch01 * (64 - fr) * fc       + ch11 * fr * fc;
					int a = -ch00 + ch10 + ch01 - ch11;
					ch.delta2 = 2 * a;
					ch.delta = (ch10 - ch00) * (64 - fc) + (ch11 - ch01) * fc
					         - (ch01 - ch00) * (64 - fr) - (ch11 - ch10) * fr + a;
					return ch;
				};
				Chan cb = MakeChan((int)(c00 >>  0) & 0xFF, (int)(c10 >>  0) & 0xFF, (int)(c01 >>  0) & 0xFF, (int)(c11 >>  0) & 0xFF);
				Chan cg = MakeChan((int)(c00 >>  8) & 0xFF, (int)(c10 >>  8) & 0xFF, (int)(c01 >>  8) & 0xFF, (int)(c11 >>  8) & 0xFF);
				Chan cr = MakeChan((int)(c00 >> 16) & 0xFF, (int)(c10 >> 16) & 0xFF, (int)(c01 >> 16) & 0xFF, (int)(c11 >> 16) & 0xFF);

				for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2,
					cb.val += cb.delta, cb.delta += cb.delta2,
					cg.val += cg.delta, cg.delta += cg.delta2,
					cr.val += cr.delta, cr.delta += cr.delta2) {
					int val = (val4096 * A8L8_SCALE + 2048) >> 12;
					if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
					uint b8 = (uint)((cb.val + 2048) >> 12);
					uint g8 = (uint)((cg.val + 2048) >> 12);
					uint r8 = (uint)((cr.val + 2048) >> 12);
					uint w = b8 | (g8 << 8) | (r8 << 16);
					if (w != TINT_WHITE) {
						const uint B = (uint)(lmax_A8L8 - val);
						if (B > 0) TintAccumulate(tintRow, massRow, wsX, w, B);
						else tintRow[wsX] = w; // byte parity with the old branch
					}
				}
			}
			isoRow_64 += segLen;
			isoCol_64 -= segLen;
		}
	}

	if (zeroAmbient) {
		// Full-bright row: ambient == 0, stamps are no-ops.
		memset(dstRow + startX, 0, (size_t)(endX - startX));
		memset(dstRowBase + startX, 0, (size_t)(endX - startX));
		return;
	}

	// ---- ambient bilinear fill (identical math to FillScreenSpaceLighting) ----
	{
		int isoRow_64 = rowIsoRow64;
		int isoCol_64 = rowIsoCol64;
		int wsX = startX;

		while (wsX < endX) {
			// Same cross-frame guard as FillScreenSpaceRow: a worker resumed
			// after the timeout-drain must stop within one <=64 px segment.
			if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
				s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
				// Rev 27 (§4.3): keep the base==surface invariant the old
				// dual-write code produced on this abort path.
				memcpy(dstRowBase + startX, dstRow + startX, (size_t)(endX - startX));
				return;
			}
			int fr, fc;
			int iRow = floor64(isoRow_64, fr);
			int iCol = floor64(isoCol_64, fc);

			int v00 = L(iRow,     iCol);
			int v10 = L(iRow + 1, iCol);
			int v01 = L(iRow,     iCol + 1);
			int v11 = L(iRow + 1, iCol + 1);

			int toRow = 64 - fr;
			int toCol = fc + 1;
			int segLen = toRow < toCol ? toRow : toCol;
			int remaining = endX - wsX;
			if (segLen > remaining) segLen = remaining;

			if (v00 == v10 && v00 == v01 && v00 == v11) {
				int val = v00 * A8L8_SCALE;
				if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
				memset(&dstRow[wsX], (uchar)val, segLen);
				wsX += segLen;
			} else {
				int val4096 = v00 * (64 - fr) * (64 - fc)
							+ v10 * fr       * (64 - fc)
							+ v01 * (64 - fr) * fc
							+ v11 * fr       * fc;

				const int A      = -v00 + v10 + v01 - v11;
				const int delta2 = 2 * A;
				int delta = (v10 - v00) * (64 - fc) + (v11 - v01) * fc
						  - (v01 - v00) * (64 - fr) - (v11 - v10) * fr + A;

				for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2) {
					int val = (val4096 * A8L8_SCALE + 2048) >> 12;
					if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
					dstRow[wsX] = (uchar)val;
				}
			}

			isoRow_64 += segLen;
			isoCol_64 -= segLen;
		}
	}

	// ---- per-light stamps (Rev 20: max-mode nearest-light envelope) ----
	// Along the scanline t = x - startX: dr = drStart + t, dc = dcStart - t,
	// d2(t) = 2t^2 + 2(drStart - dcStart)t + drStart^2 + dcStart^2.
	// The circle d2 <= D64^2 intersects the row iff the quadratic minimum
	// (drStart + dcStart)^2 / 2 <= D64^2, and the roots are
	// t = ((dcStart - drStart) +/- sqrt(2*D64^2 - (drStart + dcStart)^2)) / 2.
	//
	// Rev 20 — max mode: dst = min(ambient, 255 - max B_s).  Lights sharing
	// one LUT row (same radius) satisfy max B_s = LUT[min d_s], i.e. the
	// winner is the light with the smallest d2.  Every d2 curve has the same
	// leading coefficient 2, so "j strictly beats i" is a HALF-LINE in t:
	// (a_j - a_i)t < b_i - b_j, with a = 2(drStart - dcStart),
	// b = drStart^2 + dcStart^2.  Light i's winner set is its disc interval
	// minus a union of such half-lines clipped to the competitors' disc
	// intervals — stamped piecewise, i.e. ONE light per pixel in same-radius
	// clusters (fire wall / NOVA ~10x).  Mixed radii combine through min()
	// on the shared dst buffer exactly as before.  Byte-identical to the
	// original full-interval loop.
	struct RowLight {
		int drStart;
		int dcStart;
		int tMin;
		int tMax;
		int D64;
		const uchar* lut;
		// (th4) Stage 3: winning-light tint (white = no tint).
		uchar tintR, tintG, tintB;
	};
	RowLight rl[Lights_128_Count];
	int rlCount = 0;
	for (int s = 0; s < stampCount; s++) {
		const V2Stamp& st = stamps[s];
		const int drStart = rowIsoRow64 - st.centerRow64;
		const int dcStart = rowIsoCol64 - st.centerCol64;
		const long long ssum = (long long)(drStart + dcStart) * (drStart + dcStart);
		if (ssum > 2LL * st.D64sq) continue; // row outside this light's disc
		const int b = dcStart - drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > endX - startX - 1) tMax = endX - startX - 1;
		if (tMin > tMax) continue;
		RowLight& r = rl[rlCount++];
		r.drStart = drStart;
		r.dcStart = dcStart;
		r.tMin = tMin;
		r.tMax = tMax;
		r.D64 = st.D64;
		r.lut = st.lut;
		r.tintR = st.tintR;
		r.tintG = st.tintG;
		r.tintB = st.tintB;
	}

	// Exact stamping over t in [tS, tE] inclusive — identical arithmetic
	// to the original loop, initialised at tS.
	//
	// Rev 21 — SSE2-vectorised inner loop, 16 pixels per iteration.
	// The sequential scalar state (d2, delta; second difference 4) is
	// kept exact.  Per group, the four d2 lanes advance by a constant
	// per-lane vector ({4d+24, 4d+40, 4d+56, 4d+72} + 64 per group) and
	// d = floor(sqrt(d2)) is computed with sqrtps + truncation.  For
	// d2 <= 1023^2 (< 2^20) the float sqrt is correctly rounded and
	// sqrt(n) is either an integer or > 2^-11 away from one, so the
	// truncation is EXACT (also enforced by the identity tool).  The
	// per-pixel application is byte-exact SSE2:
	//   lv = 255 - B = ~B, dst = min(dst, lv)  (max-brightness combine)
	// The d <= D64 guard is folded into B = 0 (a no-op).
	// Output is byte-identical to the original scalar loop.
	// (th4) Stage 3 (Rev 2.29/2.30): brightness-weighted tint blend. The A byte
	// of the tint word carries the winner's brightness advantage over the
	// AMBIENT (NOT over the local runner-up) — the shader's lerp(white, rgb,
	// A) is strong where A is large, so a pool keeps its color wherever it
	// clearly beats the ambient (including inside another colored pool, where
	// the crossfade should stay saturated) and dissolves toward white only at
	// its outer edge (A -> 0 where the pool meets darkness). The win-side A
	// telescopes: oldA is the old winner's ambient advantage, so
	// A' = oldA + beat = ambient - lv' (<= 255 by construction).
	// RGB updates run on BOTH sides of an interaction (only for OLD COLORED
	// words — white/ambient keeps the plain replace, byte-identical to
	// Rev 2.26 for single-colored-light and colored-vs-white scenes):
	//   - strict win: rgb' = lerp(old, winner, w), A' = oldA + beat
	//   - lose:       rgb' = lerp(old, winner, w), A unchanged
	//     (the side Rev 2.27/2.28 never updated — the hard edge on the
	//     first-stamped light's side of the boundary).
	// (th4) Rev 2.44: the tint word now accumulates the brightness-weighted
	// average of EVERY in-disc light (white included) — order-independent and
	// continuous for any number of distinct colors (the pre-Rev-2.44 win/lose
	// pair rule was order-dependent with 3+ colors: a colored light losing to
	// a white word was silently dropped, so adjacent pixels on opposite sides
	// of a light-interaction boundary took different event sequences and
	// jumped discontinuously — the rotating hard arcs/lines). The A byte now
	// carries the accumulated brightness mass m = min(255, sum B_i): the
	// shader's lerp(white, rgb, A) keeps its "pool glows at the center,
	// dissolves to white at the outer edge" falloff, and the crossfade weight
	// is the incoming light's brightness relative to the accumulated mass
	// (w = B/(m + B)). For up to two colored lights over dark ambient this
	// is byte-identical to the old Rev 2.30 rule (the old Bp was the single
	// prior contributor's brightness). The tint word's A byte carries
	// min(255, mass) — the shader strength saturates there. (th4) Rev 2.45:
	// the WEIGHT base is the TRUE mass from the row scratch (massRow), so
	// deep overlaps follow the exact ΣBC/ΣB average instead of the clamped
	// value — one 32-bit idiv only while the true mass exceeds 255 (the
	// deep-overlap cores). Pre-2.45 the clamped base degraded smoothly but
	// order-dependently in those cores.
	// (th4) P3: the mix itself lives in TintAccumulate (defined with
	// g_TintWeightLut above) — one code path for dynamic stamps and the
	// static-bake ambient contributor.
	auto stampSegment = [&](const RowLight& r, int tS, int tE) {
		// Stamp pieces can span the whole row, so the guard is also checked
		// per 16 px group below (stale patch <= ~31 px worst case).
		if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
			s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
			return;
		}
		const int t0 = tS;
		int dr = r.drStart + t0;
		int dc = r.dcStart - t0;
		int d2 = dr * dr + dc * dc;
		int delta = 2 * (dr - dc) + 2; // d2 increment for the next pixel

		const uchar* __restrict lut = r.lut;
		const int D64 = r.D64;
		uchar* dst = dstRow;
		// (th4) Stage 3: constant winner tint for this segment — the same
		// light stamps the whole piece, so the tint word is one constant.
		// (th4) 2026-09-04 BUGFIX: the byte order must match the A8R8G8B8
		// texture's little-endian memory layout (byte 0 = BLUE, byte 1 =
		// green, byte 2 = red) — exactly the rgbLUT packing the color texture
		// uses. The original (r | g<<8 | b<<16) put RED in byte 0, so the
		// shader sampled .r=blue/.b=red and the red firebolt pool rendered
		// as a dark blue sheet (white was unaffected — symmetric).
		const uint tintWord = tintRow
			? ((uint)r.tintB | ((uint)r.tintG << 8) | ((uint)r.tintR << 16))
			: 0u;
		const __m128i tintVec = _mm_set1_epi32((int)tintWord);

		int x = startX + tS;
		const int xEnd = startX + tE;

		// SIMD main loop: 16 pixels per iteration (4 x 4-lane sqrt).
		for (; x + 16 <= xEnd + 1; x += 16) {
			if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
				s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
				return;
			}
			int laneD2[4];
			laneD2[0] = d2;
			laneD2[1] = d2 + delta;
			laneD2[2] = d2 + 2 * delta + 4;
			laneD2[3] = d2 + 3 * delta + 12;
			__m128i v = _mm_loadu_si128((const __m128i*)laneD2);
			int laneD[16];
			// per-lane 4-px forward difference of d2: 4(delta+4i)+24
			__m128i step = _mm_add_epi32(_mm_set1_epi32(4 * delta + 24),
										_mm_setr_epi32(0, 16, 32, 48));
			for (int g = 0; g < 4; g++) {
				__m128 f = _mm_cvtepi32_ps(v);
				f = _mm_sqrt_ps(f);
				_mm_storeu_si128((__m128i*)(laneD + 4 * g), _mm_cvttps_epi32(f));
				v = _mm_add_epi32(v, step);
				step = _mm_add_epi32(step, _mm_set1_epi32(64));
			}
			uchar bbuf[16];
			for (int i = 0; i < 16; i++) {
				const int dd = laneD[i];
				bbuf[i] = ((uint)dd <= (uint)D64) ? lut[dd] : (uchar)0;
			}
			const __m128i bv = _mm_loadu_si128((const __m128i*)bbuf);
			{
				const __m128i lv = _mm_xor_si128(bv, _mm_set1_epi8((char)0xFF));
				__m128i d0 = _mm_loadu_si128((const __m128i*)(dst + x));
				__m128i dn = _mm_min_epu8(d0, lv);
				_mm_storeu_si128((__m128i*)(dst + x), dn);
				// (th4) Rev 2.44: EVERY in-disc pixel (B > 0) contributes its
				// brightness-weighted color to the running mass average — win
				// and lose alike (order-independent, continuous for any number
				// of distinct tints; the pre-Rev-2.44 win/lose pair rule was
				// order-dependent with 3+ colors and produced hard arcs/lines
				// at light-interaction boundaries). The A byte of the tint
				// word is the accumulated brightness mass. Lanes with B > 0
				// AND old mass > 0 take the per-pixel blend (scalar); every
				// other lane is out-of-disc (keep old) or a first contributor
				// (plain replace word | B<<24).
				if (tintRow) {
					const __m128i s80 = _mm_set1_epi8((char)0x80);
					// in-disc mask (B > 0): cmpgt_epi8 is SIGNED, so sign-flip
					// the operand first for an unsigned-bytes compare.
					const __m128i mB = _mm_cmpgt_epi8(_mm_xor_si128(bv, s80), s80);
					__m128i* tp = (__m128i*)(tintRow + x);
					__m128i t0 = _mm_loadu_si128(tp + 0);
					__m128i t1 = _mm_loadu_si128(tp + 1);
					__m128i t2 = _mm_loadu_si128(tp + 2);
					__m128i t3 = _mm_loadu_si128(tp + 3);
					// blend needed in any lane with B > 0 AND old mass > 0?
					bool needScalar = false;
					{
						const __m128i zz = _mm_setzero_si128();
						const __m128i a0 = _mm_cmpgt_epi32(_mm_srli_epi32(t0, 24), zz);
						const __m128i a1 = _mm_cmpgt_epi32(_mm_srli_epi32(t1, 24), zz);
						const __m128i a2 = _mm_cmpgt_epi32(_mm_srli_epi32(t2, 24), zz);
						const __m128i a3 = _mm_cmpgt_epi32(_mm_srli_epi32(t3, 24), zz);
						needScalar = _mm_movemask_epi8(_mm_or_si128(
							_mm_or_si128(_mm_and_si128(a0, mB), _mm_and_si128(a1, mB)),
							_mm_or_si128(_mm_and_si128(a2, mB), _mm_and_si128(a3, mB)))) != 0;
					}
					if (!needScalar) {
						// whole group: every lane is out-of-disc (keep old) or
						// a first contributor (replace with word | B<<24).
						const __m128i z = _mm_setzero_si128();
						const __m128i b16lo = _mm_unpacklo_epi8(bv, z);
						const __m128i b16hi = _mm_unpackhi_epi8(bv, z);
						__m128i w0 = _mm_or_si128(_mm_slli_epi32(_mm_unpacklo_epi16(b16lo, z), 24), tintVec);
						__m128i w1 = _mm_or_si128(_mm_slli_epi32(_mm_unpackhi_epi16(b16lo, z), 24), tintVec);
						__m128i w2 = _mm_or_si128(_mm_slli_epi32(_mm_unpacklo_epi16(b16hi, z), 24), tintVec);
						__m128i w3 = _mm_or_si128(_mm_slli_epi32(_mm_unpackhi_epi16(b16hi, z), 24), tintVec);
						const __m128i mlo = _mm_unpacklo_epi8(mB, mB);
						const __m128i mhi = _mm_unpackhi_epi8(mB, mB);
						const __m128i m4a = _mm_unpacklo_epi16(mlo, mlo);
						const __m128i m4b = _mm_unpackhi_epi16(mlo, mlo);
						const __m128i m4c = _mm_unpacklo_epi16(mhi, mhi);
						const __m128i m4d = _mm_unpackhi_epi16(mhi, mhi);
						t0 = _mm_or_si128(_mm_and_si128(m4a, w0), _mm_andnot_si128(m4a, t0));
						t1 = _mm_or_si128(_mm_and_si128(m4b, w1), _mm_andnot_si128(m4b, t1));
						t2 = _mm_or_si128(_mm_and_si128(m4c, w2), _mm_andnot_si128(m4c, t2));
						t3 = _mm_or_si128(_mm_and_si128(m4d, w3), _mm_andnot_si128(m4d, t3));
						_mm_storeu_si128(tp + 0, t0);
						_mm_storeu_si128(tp + 1, t1);
						_mm_storeu_si128(tp + 2, t2);
						_mm_storeu_si128(tp + 3, t3);
						// (th4) Rev 2.45: seed the true-mass scratch for the
						// first-contributor lanes (mass = B, 16-bit lanes).
						// Lanes with B == 0 keep their scratch value — still 0
						// by the same oldA == 0 invariant.
						{
							__m128i* mp = (__m128i*)(massRow + x);
							__m128i ms0 = _mm_loadu_si128(mp + 0);
							__m128i ms1 = _mm_loadu_si128(mp + 1);
							ms0 = _mm_or_si128(_mm_and_si128(mlo, b16lo), _mm_andnot_si128(mlo, ms0));
							ms1 = _mm_or_si128(_mm_and_si128(mhi, b16hi), _mm_andnot_si128(mhi, ms1));
							_mm_storeu_si128(mp + 0, ms0);
							_mm_storeu_si128(mp + 1, ms1);
						}
					} else {
						// some lane blends: per-pixel mass rule (16 lanes;
						// same math as the scalar tail below).
						for (int i = 0; i < 16; i++) {
							TintAccumulate(tintRow, massRow, x + i, tintWord, (uint)bbuf[i]);
						}
					}
				}
			}
			d2 += 16 * delta + 480;
			delta += 64;
		}

		// Scalar tail — identical to the original loop.
		if (epoch >= 0 && s_PPLFrameId.load(std::memory_order_relaxed) != epoch) {
			s_PPLStaleAborts.fetch_add(1, std::memory_order_relaxed);
			return;
		}
		int d = (int)Isqrt32((unsigned)d2);
		int dsq = d * d;
		for (; x <= xEnd; x++) {
			if ((uint)d <= (uint)D64) { // inside the disc -> LUT entry > 0
				const int B = (int)lut[d];
				const int lv = 255 - B;
				// (th4) Rev 2.44: every in-disc pixel contributes to the mass
				// average (win and lose alike), BEFORE the light byte. The
				// subtraction stays inside the tintRow gate, so the non-tint
				// path is byte-for-byte the pre-Stage-3 loop (no dead math).
				if (tintRow) TintAccumulate(tintRow, massRow, x, tintWord, (uint)B);
				if (lv < dst[x]) dst[x] = (uchar)lv;
			}
			// advance d2 / d / dsq to the next pixel
			d2 += delta;
			delta += 4;
			while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
			while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
		}
	};

	{
		// Stamp only each light's winner pieces within its radius group
		// (lower envelope of lines with restricted domains).
		int rmP[Lights_128_Count], rmQ[Lights_128_Count];
		for (int i = 0; i < rlCount; i++) {
			const RowLight& ri = rl[i];
			const int ai = 2 * (ri.drStart - ri.dcStart);
			const int bi = ri.drStart * ri.drStart + ri.dcStart * ri.dcStart;
			int rmCount = 0;
			bool beatenEverywhere = false;
			for (int j = 0; j < rlCount; j++) {
				if (j == i || rl[j].lut != ri.lut) continue; // same radius only
				// (th4) Rev 2.26: the nearest-light envelope applies only
				// between same-radius lights of the SAME tint color. A colored
				// light vs a white light of the same radius must BOTH stamp
				// (the min() value pass is unaffected) so the colored winner's
				// win margin sees the white light's value — a smooth boundary
				// instead of a hard sector.
				// (th4) Rev 2.27 gate: with identical tints the test is a no-op,
				// which is exactly the pre-Stage-3 state — the envelope loop
				// stays untouched unless a colored light is actually active.
				if (g_LightTintActive && (rl[j].tintR != ri.tintR
					|| rl[j].tintG != ri.tintG || rl[j].tintB != ri.tintB)) continue;
				const RowLight& rj = rl[j];
				if (rj.tMax < ri.tMin || rj.tMin > ri.tMax) continue; // Rev 24: disjoint -> no constraint
				const int da = 2 * (rj.drStart - rj.dcStart) - ai;
				const int db = bi - (rj.drStart * rj.drStart + rj.dcStart * rj.dcStart);
				int p = 0, q = 0;
				// (th4) Rev 2.37: EXACT TIES between same-radius same-color
				// lights go to the LOWER stamp index. The light bytes are
				// unaffected (an exact tie means equal d2 -> equal LUT entry
				// -> equal lv, and min() ties are no-ops), but without a
				// tie-break BOTH lights stamp tie pixels and the tint
				// crossfade applies TWICE: bright 1-px lines along the
				// nearest-light bisectors (the "sharp straight line edges"
				// in a spammed charged-bolt field, rotating as the bolts
				// fly) and a whole disc doubling when two lights share a
				// center (the "crisp rotating sector disc").
				// The tie-break runs ONLY while this row is writing tint
				// words (tintRow != null). The else branch below is the
				// exact pre-Stage-3 code — with no tint the envelope is
				// byte-identical to the pre-branch engine and pays nothing.
				if (tintRow && j < i) {
					if (da == 0) {
						if (db >= 0) { beatenEverywhere = true; break; } // j ties-or-beats i everywhere
						continue;
					} else if (da > 0) {
						// tie-inclusive removal: t <= floor(db/da);
						// empty iff floor(db/da) < T  <=>  db < da*T.
						const int T = rj.tMin > ri.tMin ? rj.tMin : ri.tMin;
						if (db < da * T) continue;
						q = floorDivI(db, da);
						p = -0x40000000;
					} else {
						// tie-inclusive removal: t >= ceil(db/da);
						// empty iff ceil(db/da) > U  <=>  db < da*U (da < 0).
						const int U = rj.tMax < ri.tMax ? rj.tMax : ri.tMax;
						if (db < da * U) continue;
						p = ceilDivI(db, da);
						q = 0x40000000;
					}
				} else {
					// pre-Stage-3 strict removal (unchanged):
					if (da == 0) {
						if (db > 0) { beatenEverywhere = true; break; } // j lower everywhere
						continue;
					} else if (da > 0) {
						// Rev 27 (§4.2): skip the idiv when the removal is
						// provably before i's interval. q_raw = ceil(db/da)-1
						// >= T  <=>  db > da*T (da > 0; |da*T| <= ~2^27).
						const int T = rj.tMin > ri.tMin ? rj.tMin : ri.tMin;
						if (db <= da * T) continue;
						// strict beat set: t < db/da -> t <= ceil(db/da) - 1
						q = ceilDivI(db, da) - 1;
						p = -0x40000000;
					} else {
						// Rev 27 (§4.2): symmetric skip — p_raw = floor(db/da)+1
						// constrains i only if p_raw <= U  <=>  db > da*U (da < 0).
						const int U = rj.tMax < ri.tMax ? rj.tMax : ri.tMax;
						if (db <= da * U) continue;
						// strict beat set: t > db/da -> t >= floor(db/da) + 1
						p = floorDivI(db, da) + 1;
						q = 0x40000000;
					}
				}
				// clip to j's disc interval (outside it j contributes 0)
				if (p < rj.tMin) p = rj.tMin;
				if (q > rj.tMax) q = rj.tMax;
				if (p <= q) { rmP[rmCount] = p; rmQ[rmCount] = q; rmCount++; }
			}
			if (beatenEverywhere) continue;
			// sort removal intervals by p (insertion sort, <= 32 entries)
			for (int a = 1; a < rmCount; a++) {
				const int pp = rmP[a], qq = rmQ[a];
				int b = a - 1;
				while (b >= 0 && rmP[b] > pp) { rmP[b + 1] = rmP[b]; rmQ[b + 1] = rmQ[b]; b--; }
				rmP[b + 1] = pp;
				rmQ[b + 1] = qq;
			}
			// stamp [tMin, tMax] minus the removed intervals
			int cur = ri.tMin;
			for (int a = 0; a < rmCount && cur <= ri.tMax; a++) {
				if (rmP[a] > ri.tMax) break; // remaining removals beyond the disc
				if (rmP[a] > cur) stampSegment(ri, cur, rmP[a] - 1);
				if (rmQ[a] >= cur) cur = rmQ[a] + 1;
			}
			if (cur <= ri.tMax) stampSegment(ri, cur, ri.tMax);
		}
	}

	// Rev 27 (§4.3): single-buffer fill — LightSurfaceBase receives the SAME
	// bytes via one row-end copy.  The old code wrote identical values to both
	// buffers per pixel, so this is byte-identical output and keeps the
	// "immutable pre-fill reference" semantics of LightSurfaceBase intact.
	memcpy(dstRowBase + startX, dstRow + startX, (size_t)(endX - startX));
}

// ===== V2 multi-threaded dispatch (Rev 24) =====
// Same byte-identical row code as the ST path, dispatched over the existing
// PPL worker pool.  The legacy FillScreenSpaceLightingDispatch /
// FillScreenSpaceRow path is untouched.  Falls back to single-threaded rows
// when no workers are available.
static void FillScreenSpaceLightingV2MT(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor, bool dun)
{
	if (!g_A8L8Lighting || !LightSurface || !LightSurfaceBase) return;

	// Build the frame inputs once (same builder as the ST path — Rev 24).
	V2FrameCtx f;
	V2BuildFrame(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun);
	if (V2CacheHit(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
		f.stamps, f.stampCount, f.startY, f.endY)) return;
	f.zeroAmbient = V2ScanZeroAmbient(f.useStaticAmbient);

	// Rev 27 (§4.1): a translation hit is resolved entirely on the main
	// thread (restore + sliver fills, no dispatch) — the dispatch protocol
	// carries one global row range and a hit leaves too little work to
	// parallelise.  Stale-abort protection for the store below is
	// centralised in V2CacheStore.
	if (V2TranslationHit(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun)) {
		V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
			f.stamps, f.stampCount, f.startY, f.endY);
		return;
	}

	// (th4) Stage 4 item 7: changed-stamp band fill — moving lights recompute
	// only their dirty rows; clean rows shift-copy from the cache.
	if (V2BandHit(f, baseRow, baseCol, offsetX, offsetY, zoomFactor, dun, true)) {
		V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
			f.stamps, f.stampCount, f.startY, f.endY);
		return;
	}

	PPLPoolInit();
	if (s_PPLWorkerCnt == 0) {
		// no workers: run the same rows single-threaded
		for (int wsY = f.startY; wsY < f.endY; wsY++)
			FillScreenSpaceRowV2(wsY, f.X0, f.Y0, f.startX, f.endX, f.useStaticAmbient, f.zeroAmbient, f.stamps, f.stampCount, -1);
		V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
			f.stamps, f.stampCount, f.startY, f.endY);
		return;
	}

	// One stale-abort reset for the whole frame (V2FillRowsMT spans share the
	// flag so a late write during any span is still seen at store time).
	s_PPLStaleAborts.store(0, std::memory_order_relaxed);
	V2FillRowsMT(f, f.startY, f.endY - f.startY);

	// Rev 27 hardening: expire worker epochs at the end of the dispatch, so a
	// worker stalled mid-row that resumes during a LATER frame (cache hit,
	// translation hit, ST fill — frames that publish no dispatch) sees a
	// changed frameId, aborts within ~64 px and flags s_PPLStaleAborts,
	// instead of finishing its stale row unflagged (which could bake stale
	// bytes into the frame cache). No notify needed: sleeping workers hold
	// no rows; a worker still in its claim loop sees the change on its next
	// claim and re-sleeps after a no-op cycle.
	{ std::lock_guard<std::mutex> lk(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }

	// Only snapshot the cache when every row actually completed AND no worker
	// wrote stale bytes into this dispatch's buffers.  Leaving the previous
	// cache entry untouched is self-healing (V2CacheStore also re-checks
	// s_PPLStaleAborts after the snapshot memcpy).
	if (s_PPLDone.load(std::memory_order_acquire) >= f.endY - f.startY
	 && s_PPLStaleAborts.load(std::memory_order_acquire) == 0) {
		V2CacheStore(baseRow, baseCol, offsetX, offsetY, zoomFactor, dun,
			f.stamps, f.stampCount, f.startY, f.endY);
	}
}

// ===== Stage 4 item 6 (Option A): MT palette->RGB resolve dispatch =====
// Dispatches rows of the locked s0 color texture over the persistent PPL pool
// (s_SSMode == 3). The CALLER holds the D3D lock for the whole dispatch and
// unlocks after it returns; workers do pure memory writes (no D3D calls). Each
// row is a pure function of its own inputs, so any thread assignment — including
// the timeout drain — writes byte-identical data. Returns 1 when the rows were
// handled (MT or drained); 0 when MT is unavailable (pool broken / no workers /
// surface too small) so the caller runs the ST loop.
int PPLResolveDispatch(const ResolveMTContext& ctx)
{
	if (s_PPLPoolBroken || ctx.width <= 0 || ctx.height <= 0) return 0;
	// Too small to amortize the dispatch (~10-30 us publish+steal overhead).
	if ((long long)ctx.width * ctx.height < (long long)640 * 480) return 0;
	PPLPoolInit();
	if (s_PPLWorkerCnt == 0) return 0;

	s_ResolveCtx = ctx;
	s_ResolveRowStart = 0;
	s_ResolveRowCount = ctx.height;
	// Publish mode + counters + frameId under s_PPLWorkMtx (M2 fix, same as V2MT).
	{
		std::lock_guard<std::mutex> lk(s_PPLWorkMtx);
		s_SSMode = 3;
		s_PPLClaimed.store(0, std::memory_order_relaxed);
		s_PPLDone.store(0,    std::memory_order_relaxed);
		s_PPLStaleAborts.store(0, std::memory_order_relaxed);
		s_PPLTotal.store(s_ResolveRowCount, std::memory_order_relaxed);
		s_PPLFrameId.fetch_add(1, std::memory_order_relaxed);
	}
	s_PPLCvWork.notify_all();

	// Main thread also work-steals rows to reduce tail latency.
	bool allDone = false;
	for (;;) {
		int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
		if (rowIdx >= s_ResolveRowCount) break;
		ResolveRowMT(s_ResolveRowStart + rowIdx, s_ResolveCtx, -1);
		if (s_PPLDone.fetch_add(1, std::memory_order_acq_rel) + 1 >= s_ResolveRowCount) {
			allDone = true;
			s_PPLTimeoutCount = 0;
			break;
		}
	}

	if (!allDone) {
		std::unique_lock<std::mutex> lk(s_PPLDoneMtx);
		const bool completed = s_PPLCvDone.wait_for(lk, std::chrono::milliseconds(PPL_DISPATCH_TIMEOUT_MS), []{
			return s_PPLDone.load(std::memory_order_acquire) >= s_ResolveRowCount;
		});
		if (!completed) {
			static bool logged = false;
			if (!logged) {
				logged = true;
				OutputDebugString("PPL TIMEOUT (resolve): draining remaining rows single-threaded\n");
			}
			for (;;) {
				int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
				if (rowIdx >= s_ResolveRowCount) break;
				ResolveRowMT(s_ResolveRowStart + rowIdx, s_ResolveCtx, -1);
				s_PPLDone.fetch_add(1, std::memory_order_acq_rel);
			}
			PPLNoteTimeout(); // M1: repeated timeouts disable MT for the session
			// Expire worker epochs, then settle: a worker paused mid-row may
			// finish its CURRENT row (identical bytes) into the caller's still-
			// locked rect — wait (bounded) so no write can land after UnlockRect.
			// ACCEPTED LIMITATIONS of this path (same philosophy as V2MT):
			//  (a) a stalled worker's claimed row is not re-run by the drain, so
			//      that one row keeps the PREVIOUS frame's texture content for
			//      one frame (the pool disables itself after 3 timeouts);
			//  (b) a worker stalled >1s that resumes AFTER this settle AND after
			//      the caller's UnlockRect could write one row into a stale
			//      pointer — bounded to its own row, practically unreachable.
//			//  (b) a worker stalled longer than timeout + settle that resumes
//			//      AFTER the caller's UnlockRect could write one row into a
//			//      stale pointer — bounded to its own row; the 250 ms settle
//			//      below shrinks this to genuinely stuck-thread territory.
			{ std::lock_guard<std::mutex> lk2(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
			const auto settleDeadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(2);
//			// (th4) 2026-09-10 hardening (cheap): was a 2 ms spin. No cost on
//			// the normal path — this only runs after the 1 s dispatch timeout
//			// above already fired. When it does run, the stalled worker is
//			// usually just finishing its current row segment, so in-flight
//			// drops to 0 within microseconds and the first iteration exits
//			// immediately; the longer bound only matters for the pathological
//			// late-resume case it exists to cover. Sleep(1) per iteration
//			// instead of a bare yield so a stall can never spin a core for the
//			// whole window (≤ PPL_RESOLVE_SETTLE_MS wakeups).
//			const auto settleDeadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(PPL_RESOLVE_SETTLE_MS);
			while (s_ResolveInFlight.load(std::memory_order_acquire) != 0
				&& std::chrono::steady_clock::now() < settleDeadline) {
				std::this_thread::yield();
//				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			// (th4) 2026-09-10 diagnostics: >0 here means a worker is still
			// writing into the locked s0 texture while the caller is about to
			// return and UnlockRect — the one path where the documented
			// limitation (b) can corrupt D3D-owned memory. Logged every time;
			// a crash log containing this line ties a d3d9 fault to an MT
			// resolve timeout.
			D3DDIAG("PPL RESOLVE timeout: drained; workers in-flight at unlock = %d",
				s_ResolveInFlight.load(std::memory_order_acquire));
		} else {
			s_PPLTimeoutCount = 0;
		}
	}

	// Rev 27 hardening: expire worker epochs at the end of the dispatch (see
	// FillScreenSpaceLightingV2MT). On the normal path every claim completed
	// before this point, so no row can land late; on the timeout path the
	// settle above ran first.
	{ std::lock_guard<std::mutex> lk(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
	return 1;
}

// (th4) PERSPECTIVE MODE (config Perspective: off, on = 5, or 1..10). A cheap imitation of
// Diablo II's perspective view: the finished WORLD image (right after DrawIsoLand, before any
// UI is drawn) is re-sampled so the ground seems tilted away from the camera --
//   horizontally: rows are magnified more toward the bottom of the screen (top unchanged),
//                 around the screen's centre column: s(u) = 1 + b*u, u = y / ScreenHeight;
//   vertically:   rows are foreshortened toward the top: srcY = H * (u + a*u*(1 - u)).
// Both only ever read INSIDE the drawn screen area, so no black edges appear. Unlike real
// D2 perspective, sprites are warped together with the floor (this is a 2D warp of the
// finished image, not 3D geometry). The mouse is mapped through the SAME function in
// DispatchCursorAboveIsoland (Main.cpp), so clicks land on what is shown. Things drawn
// after DrawIsoLand (panels, text, cursor) are not warped.
static float PerspA() { int l = Perspective; if( l > 10 ) l = 10; return l * 0.024f; }
static float PerspB() { int l = Perspective; if( l > 10 ) l = 10; return l * 0.030f; }

// the warp's two coefficients (0, 0 when Perspective is off), for shaders that must undo it
void Perspective_GetParams( float& a, float& b )
{
	if( Perspective <= 0 ){ a = b = 0.0f; return; }
	a = PerspA(); b = PerspB();
}

// screen pixel shown at (x, y) -> world pixel it samples (both in screen coordinates)
void Perspective_ScreenToWorld( int& x, int& y )
{
	if( Perspective <= 0 || ScreenHeight <= 0 ) return;
	const float a = PerspA(), b = PerspB();
	const float u = (float)y / ScreenHeight;
	const float s = 1.0f + b * u;
	const float xc = ScreenWidth * 0.5f;
	int ys = (int)( ScreenHeight * ( u + a * u * ( 1.0f - u ) ) + 0.5f );
	int xs = (int)( xc + ( x - xc ) / s + 0.5f );
	if( ys < 0 ) ys = 0; if( ys > ScreenHeight - 1 ) ys = ScreenHeight - 1;
	if( xs < 0 ) xs = 0; if( xs > ScreenWidth - 1 ) xs = ScreenWidth - 1;
	x = xs; y = ys;
}

// Warp one screen-parallel buffer (same layout as WorkingSurface: pitch WorkingWidth, screen
// area starting at Screen_TopBorder / Screen_LeftBorder). T = uchar or uint.
template<typename T>
static void Perspective_WarpBuffer( T* base, const int* rowSrc, const int* rowAcc, const int* rowStep )
{
	if( !base ) return;
	const int W = ScreenWidth, H = ScreenHeight;
	static T* copy = 0; static int copySize = 0;	// one static copy per element type
	if( copySize != W * H ){ free( copy ); copy = (T*)malloc( sizeof(T) * W * H ); copySize = copy ? W * H : 0; }
	if( !copySize ) return;
	T* scr = base + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	for( int y = 0; y < H; ++y ) memcpy( copy + y * W, scr + y * WorkingWidth, sizeof(T) * W );
	for( int y = 0; y < H; ++y ){
		const T* src = copy + rowSrc[y] * W;
		T* dst = scr + y * WorkingWidth;
		int acc = rowAcc[y]; const int step = rowStep[y];
		for( int x = 0; x < W; ++x, acc += step ){
			int xs = acc >> 16;
			if( xs < 0 ) xs = 0; else if( xs > W - 1 ) xs = W - 1;
			dst[x] = src[xs];
		}
	}
}

// world pixel -> screen pixel where it is SHOWN (inverse of Perspective_ScreenToWorld), found by
// a few fixed-point iterations; used to place effects computed in world coordinates.
void Perspective_WorldToScreen( int& x, int& y )
{
	if( Perspective <= 0 ) return;
	const int tx = x, ty = y;
	int gx = x, gy = y;
	for( int it = 0; it < 4; ++it ){
		int sx = gx, sy = gy;
		Perspective_ScreenToWorld( sx, sy );
		gx += tx - sx; gy += ty - sy;
	}
	x = gx; y = gy;
}

static void Perspective_WarpWorld()
{
	if( Perspective <= 0 ) return;
	const int W = ScreenWidth, H = ScreenHeight;
	// per-row mapping, shared by every buffer below
	static int* rowSrc = 0; static int* rowAcc = 0; static int* rowStep = 0; static int rowsH = 0;
	if( rowsH != H ){
		free( rowSrc ); free( rowAcc ); free( rowStep );
		rowSrc = (int*)malloc( sizeof(int) * H ); rowAcc = (int*)malloc( sizeof(int) * H ); rowStep = (int*)malloc( sizeof(int) * H );
		rowsH = ( rowSrc && rowAcc && rowStep ) ? H : 0;
	}
	if( !rowsH ) return;
	const float a = PerspA(), b = PerspB();
	const float xc = W * 0.5f;
	for( int y = 0; y < H; ++y ){
		const float u = (float)y / H;
		int ys = (int)( H * ( u + a * u * ( 1.0f - u ) ) + 0.5f );
		rowSrc[y] = ys > H - 1 ? H - 1 : ys;
		const float inv = 1.0f / ( 1.0f + b * u );			// xs = xc + (x - xc) / s, 16.16 fixed point
		rowStep[y] = (int)( inv * 65536.0f );
		rowAcc[y] = (int)( ( xc - xc * inv ) * 65536.0f ) + 32768;
	}
	// The 8-bit world image, and EVERY layer that is kept pixel-for-pixel beside it and
	// combined with it later on the GPU (dx_utility.cpp upload, ~1970-2000). Warping only the
	// 8-bit image left Enriched Light Ultra's see-through-wall sprite copy (SpriteLayerSurface +
	// its weight AlphaSurface) unwarped, which showed characters and monsters twice behind
	// walls; the light (LightSurface) and light-colour (LightTintSurface) layers must match too.
	// A NEW screen-parallel layer read after DrawIsoLand must be added here as well.
	Perspective_WarpBuffer<uchar>( SoftSrcSurfacePtr, rowSrc, rowAcc, rowStep );
	Perspective_WarpBuffer<uchar>( LightSurface, rowSrc, rowAcc, rowStep );
	Perspective_WarpBuffer<uchar>( AlphaSurface, rowSrc, rowAcc, rowStep );
	Perspective_WarpBuffer<uint>( SpriteLayerSurface, rowSrc, rowAcc, rowStep );
	Perspective_WarpBuffer<uint>( LightTintSurface, rowSrc, rowAcc, rowStep );
	// (th4) town river water mask (TownWater.cpp), read by dx_utility.cpp HeatHaze_BuildMask
	// against the warped world snapshot; 0 when there is none this frame
	Perspective_WarpBuffer<uchar>( TownWater_Surface(), rowSrc, rowAcc, rowStep );
	// (th4) the Ice Age mirror's river classes (TownWater.cpp ICE RIVER, MSurf), read with the mask above; 0 when none
	Perspective_WarpBuffer<uchar>( TownWater_MirrorSurface(), rowSrc, rowAcc, rowStep );
	// (th4) town puddle mask (TownWater.cpp PUDDLES), read by dx_utility.cpp Wet_BuildMask; 0 when none
	Perspective_WarpBuffer<uchar>( TownWater_PuddleSurface(), rowSrc, rowAcc, rowStep );
}

// (th4) TOWN NIGHT through the light buffer (Enriched Light modes). Runs right after the light
// fill, before any tile or sprite is drawn: raises the darkness L of every visible pixel to at
// least the current night level, in LightSurface and in LightSurfaceBase (the fill snapshot the
// tile drawers read). UI drawn later writes L = 0, so panels, text and the cursor stay day-bright
// without any masking. Sprites write their own L from DarkLevel, which DayNight_TownSpriteDark
// already raises at night (palette.cpp), so they are not darkened twice. Frames that do not
// redraw the world keep the previous LightSurface, so transitions keep the night too.
// Known difference to the palette method: no blue shift, and the church's red glow darkens
// with everything else (the palette method skipped its entries).
static void TownNight_ApplyToLight( int zoomFactor )
{
	if( Dungeon != DUN_0_TOWN || !LightSurface ) return;
	const int q = DayNight_LightNightQ8();
	if( q <= 0 ) return;
	const int L = DayNight_LightNightMaxL() * q / 256;
	if( L <= 0 ) return;
	const int sx0 = Screen_LeftBorder, sy0 = Screen_TopBorder;
	const int w = ScreenWidth / zoomFactor, h = ScreenHeight / zoomFactor;
	const uchar Lb = (uchar)L;
	for( int y = sy0; y < sy0 + h; ++y ){
		uchar* a = LightSurface + y * WorkingWidth + sx0;
		uchar* b = LightSurfaceBase ? LightSurfaceBase + y * WorkingWidth + sx0 : 0;
		for( int x = 0; x < w; ++x ){
			if( a[x] < Lb ) a[x] = Lb;
			if( b && b[x] < Lb ) b[x] = Lb;
		}
	}
}

// (th4) WYRMS (BM_259_WATER_WYRM, BM_263_RED_WYRM) live half-submerged in pools: their sprite
// is drawn 60 px LOWER than the cell (MonsterSpritePosAdjust, "y += 60"), and the part below the
// water line used to be hidden only because the floor of the cells drawn after the wyrm painted
// over it. That also hid the whole body whenever it reached into those cells -- the spit attack
// leans the wyrm forward, and it vanished behind the pool's floor tiles.
// Now wyrms are drawn AFTER the whole scene (DrawDeferredWyrms, called from DrawIsoLand right
// after the cell loops), with the submerged part cut off explicitly instead: the sprite's bottom
// WYRM_SUBMERGED_SKIP bytes-of-header (= 32-row blocks x2, see CL2_Draw offsetStart) are skipped,
// i.e. the bottom 64 rows -- the 60 px it is lowered by, rounded to CL2's 32-row blocks.
static const int WYRM_SUBMERGED_SKIP = 4;		// standing / hit / dying: bottom 64 rows under water
// While attacking (melee or spit, ANIM_2_ATTACK) the wyrm leans FORWARD and its head dips toward the
// surface; cutting 64 rows there chopped the head off. Only the bottom 32 rows are hidden then.
// Tune per look: 0 = nothing hidden, 2 = 32 rows, 4 = 64 rows (CL2 cuts in 32-row blocks).
static const int WYRM_SUBMERGED_SKIP_ATTACK = 2;
static bool IsSubmergedWyrm( const Monster& m ){ return m.SpritePtr && is( m.SpritePtr->baseMonsterIndex, BM_259_WATER_WYRM, BM_263_RED_WYRM ); }
struct DeferredWyrm { int monsterIndex, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY; uchar* surfaceDest; uchar placeFlag; };
static DeferredWyrm g_DeferredWyrms[ 32 ];
static int g_DeferredWyrmCount = 0;
static bool g_DrawingDeferredWyrms = false;

void __fastcall DrawMonsterInTile( int monsterIndex, uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, bool firstDraw, uchar placeFlag );	// defined below
// (th4) Draw the wyrms queued by DrawMonsterInTile, on top of the finished scene and with their
// submerged part cut off. Same call, same light level as the cell pass would have used
// (DarkLevel of the wyrm's cell); firstDraw = false, because the neighbour-cell redraw that
// DrawMonsterInTile does in the cell pass would paint the pool floor back over the body.
static void DrawDeferredWyrms()
{
	if( !g_DeferredWyrmCount ) return;
	const int savedDark = DarkLevel;
	g_DrawingDeferredWyrms = true;
	for( int i = 0; i < g_DeferredWyrmCount; ++i ){
		const DeferredWyrm& w = g_DeferredWyrms[ i ];
		DarkLevel = LightMap[ w.row ][ w.col ];
		DrawMonsterInTile( w.monsterIndex, w.surfaceDest, w.row, w.col, w.screenCellRow, w.tileInCellIndex, w.playerOffsetX, w.playerOffsetY, false, w.placeFlag );
	}
	g_DrawingDeferredWyrms = false;
	g_DeferredWyrmCount = 0;
	DarkLevel = savedDark;
}

// ===== Screen-space per-pixel lighting pre-fill (multi-threaded) =====
// Dispatches FillScreenSpaceRow across the persistent worker pool.
// Divides the visible screen Y range into rows; workers steal rows via
// s_PPLClaimed.  Main thread also participates to reduce tail latency.
// Falls back to single-threaded FillScreenSpaceLighting if no workers.
static void FillScreenSpaceLightingDispatch(int baseRow, int baseCol, int offsetX, int offsetY, int zoomFactor)
{
	if (!g_A8L8Lighting || !LightSurface || !LightSurfaceBase) return;

	// Ensure thread pool is initialised (no-op after first call).
	PPLPoolInit();

	if (s_PPLWorkerCnt == 0) {
		FillScreenSpaceLighting(baseRow, baseCol, offsetX, offsetY, zoomFactor);
		return;
	}

	// Compute shared projection constants once.
	s_SSX0 = offsetX + 32 - 32 * baseRow + 32 * baseCol;
	s_SSY0 = offsetY - 15 - 16 * baseRow - 16 * baseCol;
	s_SSRowStart = Screen_TopBorder;
	s_SSRowCount = ScreenHeight / zoomFactor;
	s_SSEndX     = Screen_LeftBorder + ScreenWidth / zoomFactor;

	if (s_SSRowCount <= 0) return;

	// Publish mode + counters + frameId under s_PPLWorkMtx so the whole
	// dispatch state is atomic w.r.t. the workers' claim sections (M2 fix).
	{
		std::lock_guard<std::mutex> lk(s_PPLWorkMtx);
		s_SSMode = 1;
		s_PPLClaimed.store(0, std::memory_order_relaxed);
		s_PPLDone.store(0,    std::memory_order_relaxed);
		s_PPLTotal.store(s_SSRowCount, std::memory_order_relaxed);
		s_PPLFrameId.fetch_add(1, std::memory_order_relaxed);
	}
	s_PPLCvWork.notify_all();

	// Main thread also work-steals rows to reduce tail latency.
	bool allDone = false;
	for (;;) {
		int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
		if (rowIdx >= s_SSRowCount) break;
		FillScreenSpaceRow(s_SSRowStart + rowIdx, -1);
		if (s_PPLDone.fetch_add(1, std::memory_order_acq_rel) + 1 >= s_SSRowCount) {
			allDone = true;
			s_PPLTimeoutCount = 0;
			break;
		}
	}

	// Wait for workers to finish remaining rows — with a timeout so a
	// dead/stalled worker can never deadlock the game.  On timeout the main
	// thread drains whatever rows are left single-threaded (same pattern as
	// PPLMTDebug @ 4698dc7a).
	if (!allDone) {
		std::unique_lock<std::mutex> lk(s_PPLDoneMtx);
		const bool completed = s_PPLCvDone.wait_for(lk, std::chrono::milliseconds(PPL_DISPATCH_TIMEOUT_MS), []{
			return s_PPLDone.load(std::memory_order_acquire) >= s_SSRowCount;
		});
		if (!completed) {
			static bool logged = false;
			if (!logged) {
				logged = true;
				OutputDebugString("PPL TIMEOUT: screen-space dispatch incomplete; draining rows single-threaded\n");
			}
			for (;;) {
				int rowIdx = s_PPLClaimed.fetch_add(1, std::memory_order_relaxed);
				if (rowIdx >= s_SSRowCount) break;
				FillScreenSpaceRow(s_SSRowStart + rowIdx, -1);
				s_PPLDone.fetch_add(1, std::memory_order_acq_rel);
			}
			PPLNoteTimeout(); // M1: repeated timeouts disable MT for the session
		} else {
			s_PPLTimeoutCount = 0;
		}
	}

	// Rev 27 hardening: expire worker epochs at the end of the dispatch (see
	// FillScreenSpaceLightingV2MT).
	{ std::lock_guard<std::mutex> lk(s_PPLWorkMtx); s_PPLFrameId.fetch_add(1, std::memory_order_relaxed); }
}

// (rt) look at the lighting of neighbouring tiles on the map and interpolate it
// (rt) get per-pixel lighting with minimal code changes
// ---- th2 -------------------------------------------------------------------------------
void __fastcall CalculatePerPixelLight(int Row, int Col)
{
	if( MultiThreadPixelLight && s_PPLDispatched ){
		// PPLPoolDispatch() already waited for all tiles to be computed (global barrier).
		// No per-tile synchronization needed here — just redirect the pointer.
		PerPixelLightUniform = s_PPLUniform[Row][Col];
		if( !PerPixelLightUniform )
			PerPixelLight = s_PPLData[Row][Col]; // zero-copy: point directly at pre-computed data
		return;
	}
	// A8L8 screen-space path: LightSurface is pre-filled once per frame by
	// FillScreenSpaceLighting with globally continuous bilinear values.
	// linecpyPerPixel reads from LightSurface (not PerPixelLight) in this mode,
	// so the per-cell PerPixelLight fill below is wasted computation — skip it.
	// PerPixelLightUniform=false ensures PutTile still calls linecpyPerPixel
	// (the PPL path) rather than falling back to gamma-2 LightTable darkening.
	if (g_A8L8Lighting) {
		PerPixelLightUniform = false;
		return;
	}
	const uint lmax_A8L8 = 255; // max level in A8L8 mode is always 255 (full dark) regardless of LightMax, because the shader remaps it to 0.0 brightness; this allows full 256-level smooth gradients in A8L8 mode even when LightMax is only 3 or 15.
	const uint A8L8_SCALE = lmax_A8L8 / 15; // = 17, maps LightMap [0..15] to A8L8 [0..255]
	// Each map cell covers a 64x32 pixel screen region (two 32x32 sub-tiles).
	// Isometric inverse: for screen pixel (sx, y) within the 64-pixel-wide cell,
	//   dr*64 = sx + 2*y    (fractional row offset from Row, scaled *64)
	//   dc*64 = 2*y - sx    (fractional col offset from Col, scaled *64)
	//
	// Precompute a 3x3 grid of LightMap values so the inner loop has zero
	// per-pixel bounds checks or divisions: just 4 table lookups + a shift.
	// grid[dr][dc] = LightMap at (Row+dr, Col-1+dc), dr=0..2, dc=0..2.

	auto L = [&](int r, int c) -> int {
		r = r < 0 ? 0 : r >= FineMap_112 ? FineMap_112-1 : r;
		c = c < 0 ? 0 : c >= FineMap_112 ? FineMap_112-1 : c;
		return (uchar)LightMap[r][c];
	};
	int grid[3][3];
	for( int dr = 0; dr < 3; dr++ )
		for( int dc = 0; dc < 3; dc++ )
			grid[dr][dc] = L(Row-1+dr, Col-1+dc);

	// Fast path: if all 9 neighbors equal, the bilinear blend produces a constant.
	// Skip filling PerPixelLight and use the regular linecpy path instead.
	{
		int v0 = grid[0][0];
		bool allEqual = true;
		for( int i = 0; i < 3 && allEqual; i++ )
			for( int j = 0; j < 3 && allEqual; j++ )
				if( grid[i][j] != v0 ) allEqual = false;
		if( allEqual ){
			if (g_A8L8Lighting) {
				// A8L8: fill PerPixelLight with the uniform scaled value so the
				// PPL path is always used.  This avoids the gamma-2 LightTable
				// fallback which is perceptually much darker than the linear
				// shader (e.g. LM=12 → ~4 % perceived via LightTable vs ~48 %
				// via shader), causing uniformly-lit tiles to appear pitch-black
				// next to non-uniform tiles that use the PPL path.
				PerPixelLightUniform = false;
				PerPixelLight = s_PerPixelLightBuf;
				int val = (v0 * A8L8_SCALE > lmax_A8L8) ? lmax_A8L8 : v0 * A8L8_SCALE;
				memset(PerPixelLight, (uchar)val, 64 * 32);
			} else {
				PerPixelLightUniform = true;
			}
			return;
		}
	}
	PerPixelLightUniform = false;
	PerPixelLight = PPLCachedCell( grid );	// (th4) PPL CELL CACHE: the stored block for these inputs (computed by PPLFillCell on a miss)
}

//----- (00719720) --------------------------------------------------------
// (th4) LOOTABLE objects for the Alt reveal: chests, sarcophagi, the pod/urn/barrel
// family, plus the goat shrine and cauldron.
// Deliberately NOT every selectable object -- doors, levers, ordinary shrines, books and
// crucifixes are selectable too, and revealing those through walls would hand the player
// the level layout. Explosive pods (BO_58) are included: they look identical to the
// dropping kind, so leaving them out would make the outline itself a tell.
// BO_79/BO_80 are the two one-shot shrines (Object.cpp groups them together and both
// clear `selectable` when used), so they behave like a container rather than like the
// permanent BO_59/BO_60 shrines, which stay out.
// (th4) What the Awareness ring warns about: a trapped object, or an EXPLOSIVE pod.
// BO_58 carries no IsTrapped flag -- it is its own object type -- but it is a trap in
// every way that matters to the player, and it is visually identical to the harmless
// BO_57 that drops loot.
// NOTE this goes slightly further than the trapped-chest case: the info panel names
// BOTH pod kinds just "Barrel"/"Pod"/"Urn" (Object.cpp ~6735), so the red ring tells
// the player something hovering never would. That is deliberate -- Awareness reads
// "can see trapped objects" -- but it IS a buff to the perk, not just a consistency fix.
static bool IsAwarenessDanger( const Object& object )
{
	if( PerkValue( PERK_AWARENESS, CurrentPlayerIndex ) <= 0 ) return false;
	return object.IsTrapped || object.BaseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE;
}

static bool IsLootableObject( const Object& object )
{
	if( !object.selectable ) return false;
	const int b = object.BaseObjectIndex;
	return ( b >= BO_5_SMALL_CHEST && b <= BO_7_LARGE_CHEST )
	    || ( b >= BO_68_TRAPED_SMALL_CHEST && b <= BO_70_TRAPED_LARGE_CHEST )
	    ||   b == BO_48_SARCOPHAGUS
	    ||   b == BO_57_POD_URN_BARREL_WITH_DROP
	    ||   b == BO_58_POD_URN_BARREL_EXPLOSIVE
	    ||   b == BO_79_GOAT_SHRINE
	    ||   b == BO_80_CAULDRON
	    ||   b == BO_97_OGDEN_CHEST;
}

int __fastcall IsChest( int objectIndex )
{
	Object& object = Objects[ objectIndex ];
	int baseObjectIndex = object.BaseObjectIndex;
	return AutomapMode
		&& object.selectable
		&& (baseObjectIndex >= BO_5_SMALL_CHEST && baseObjectIndex <= BO_7_LARGE_CHEST
			 || baseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && baseObjectIndex <= BO_70_TRAPED_LARGE_CHEST);
}

//----- (00460C68) --------------------------------------------------------
void __fastcall DrawTransparentWall(int a1, int a2)
{

}

//----- (00462295) --------------------------------------------------------
void __fastcall DrawTransparentCorner(int a1, int a2)
{

}

// (rt) optimization of DrawDunCelTrans, BigFuncWithCELFile and DrawTransparentCorner
// (th4) TownWater.cpp GrassDecodePiece decodes floor pieces the same way (GRASS) -- a change to the formats here goes there too
//----- (th2) -------------------------------------------------------------
void PutTile(uchar* DstSurf, int tileType, char* celPtr, int PalIndex){
	int h, x, shift, skip;
	uchar* palTable = NULL;
	uint* mask = (uint*)&FullMask[31];
	const bool usePPL = PixelLight && (g_A8L8Lighting || !PerPixelLightUniform);
	// When g_A8L8Lighting is ON, always use linecpyPerPixel — it reads the
	// pre-filled LightSurface and writes back via dstL (no-op in normal dungeons,
	// g_PPLBrightnessMask-aware in crypt/abyss).  The non-PPL linecpy path
	// unconditionally clears LightSurface to 0 and is incompatible with A8L8.
#if OPTION_C_DIAG_LIGHTSURFACEBASE
	if (g_A8L8Lighting && g_InDungeonRender && !usePPL) {
		FILE* f = Diag_LogFile();
		if (f) { fprintf(f, "*** BUG: PutTile usePPL=false in A8L8 dungeon! PixelLight=%d PerPixelLightUniform=%d\n", PixelLight, PerPixelLightUniform); fflush(f); }
	}
#endif
	if( DarkLevel != 0 && DarkLevel != LightMax && PalIndex){
		palTable = &((uchar*)LightTable)[256 * DarkLevel];
	}else if( DarkLevel == LightMax){
		palTable = (uchar*)1;
	}
//	if( TransparentMode ){
	if( TransparentMode && !g_A8L8Lighting ){// Always force opaque walls for A8L8 path, as player and monster can be partially seen through opaque walls.  Note that when player is covered by Hydra behind wall, player outline may not be visible.
		if( PartialTrans == TSP_0_NONE ){ // (rt) draw a transparent wall
			DrawTransparentWall(0,0);
			mask = &WallTransMask[31];
		}
		else if( PartialTrans == TSP_1_LEFT && (WallsTransparency[WallIndex] & BIT(TSP_1_LEFT)) ){ // (rt) draw the left transparent trapezoid
			DrawTransparentCorner(0,0);
			mask = (uint*)&LeftTransMask[31];
		}
		else if( PartialTrans == TSP_2_RIGHT && (WallsTransparency[WallIndex] & BIT(TSP_2_RIGHT)) ){ // (rt) draw the right transparent trapezoid
			DrawTransparentCorner(0,0);
			mask = (uint*)&RightTransMask[31];
		}
	}
	switch (tileType ){
		case 0: //   (rt) draw an opaque 32x32 square
			for( h = 32; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, 0);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		case 1: // (rt) draw a semi-transparent square or triangle
			int CELWidth;
			for( h = 32; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				for( x = 0; x < 32; ){
					CELWidth = *celPtr++;
					if( CELWidth >= 0 ){
						x += CELWidth;
						if( TransparentMode && PartialTrans == TSP_0_NONE)                // (rt) if this is a wall
							if( usePPL )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, *mask, x - CELWidth);      //  (rt) xStart = start of the span in the sub-tile
							else
								linecpy(DstSurf, celPtr, CELWidth, *mask, palTable);      //  (rt) the semi-transparent format already includes the mask
						else
							if( usePPL )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, 0xFFFFFFFF, x - CELWidth); //  (rt) xStart = start of the span in the sub-tile
							else
								linecpy(DstSurf, celPtr, CELWidth, 0xFFFFFFFF, palTable); //  (rt) the semi-transparent format already includes the mask
						celPtr += CELWidth;
						DstSurf += CELWidth;
					}else{
						DstSurf -= CELWidth;
						x -= CELWidth;
					}
				}
				DstSurf -= WorkingWidth + 32;
			}
			break;
		case 2: // (rt) draw the left opaque triangle
			for( shift = 2, h = 32, skip = 30; skip != 32; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, skip);
				else
					linecpy(&DstSurf[skip], &celPtr[shift], 32 - skip, *mask << skip, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				if( h > 17 )
					skip -= 2;
				else
					skip += 2;
			}
			break;
		case 3: // (rt) draw the right opaque triangle
			for( shift = 0, h = 32, skip = 30; skip != 32; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, 0);
				else
					linecpy(DstSurf, &celPtr[shift], 32 - skip, *mask, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				if( h > 17 )
					skip -= 2;
				else
					skip += 2;
			}
			break;
		case 4: // (rt) draw the left opaque trapezoid
			for( shift = 2, h = 32, skip = 30; h > 16; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, skip);
				else
					linecpy(&DstSurf[skip], &celPtr[shift], 32 - skip, *mask << skip, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, 0);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		default: // (rt) draw the right opaque trapezoid
			for( shift = 0, h = 32, skip = 30; h > 16; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, 0);
				else
					linecpy(DstSurf, &celPtr[shift], 32 - skip, *mask, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( usePPL )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, 0);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
	}
}

// (rt) draws the tiles at the top of the screen (which must not be drawn above WorkingSurfaceEnd)
//----- (00462ECD) --------------------------------------------------------
void __fastcall DrawDunCelTrans( uchar* surfacePtr ) // DrawMTileClipTop
{
	uint maxIndex = 0;
	char* celPtr = NULL;
	uint selector = 0; // tile type
	if( DarkLevel == 0 || (PixelLight && !PerPixelLightUniform) ){ // (rt) drawing with the default palette, or PixelLight handles the shading itself
		if( CurTileID & 0x8000 ){
			CurTileID = DungeonTilesOffset[16 * (CurTileID & 0xFFF)] + (ushort)(CurTileID & 0xF000);
		}
		celPtr = (char*)CurrentLevelCELFile + CurrentLevelCELFile[(ushort)CurTileID & 0x0FFF];
		maxIndex = CurTileID;
		LOBYTE_IDA(maxIndex) = BYTE1(maxIndex);
		selector = ((maxIndex >> 4) & 7) + 8;
		// (rt) crash when entering the game
		PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
	}else if( DarkLevel != LightMax ){ // (rt) drawing with the shading palette, mostly in dungeons
		if( CurTileID & 0x8000 ){
			celPtr = &DarkenDungeonTiles[DungeonTilesOffset[DarkLevel + 16 * (CurTileID & 0xFFF)]];
			selector = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
		}else{
			char* CELPtr2 = (char*)CurrentLevelCELFile + CurrentLevelCELFile[CurTileID & 0xFFF];
			ushort Selector2 = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, Selector2 & 0xFF, CELPtr2, DarkLevel);
		}
	}else{ // (rt) drawing with the black palette
		if( CurTileID & 0x8000 ){
			CurTileID = DungeonTilesOffset[16 * (CurTileID & 0xFFF)] + (ushort)(CurTileID & 0xF000);
		}
		celPtr = (char*)CurrentLevelCELFile + CurrentLevelCELFile[CurTileID & 0xFFF];
		PutTile(surfacePtr, ((unsigned int)CurTileID >> 12) & 7, celPtr, DarkLevel);
	}
}

//----- (004672C6) --------------------------------------------------------
void __fastcall DrawTransparency(uchar* dst)
{
	int result; // eax@1 — unused (dead decompilation artefact)
	int leftOffset;     // edx@1 — column offset from tile left edge (30,28,...,0)
	int dwordRun;     // ebx@1 — number of dwords to clear per row (1,2,...,16)
	uchar* rowCursor;  // edi@1 — current row pointer walking up the tile
	int runCount;     // ecx@2 — inner loop counter
	uchar* zeroStart;     // edi@2 — pixel address where clearing begins
	int leftOffset2;     // edx@6 — column offset for second (bottom-half) pass (2,4,...,30)
	int dwordRun2;     // ebx@6 — dword count for second pass (15,14,...,1)
	int runCount2;     // ecx@7 — inner loop counter for second pass
	uchar* zeroStart2;    // edi@7 — clear start for second pass
	rowCursor = dst;
	leftOffset = 30;
	dwordRun = 1;
	result = 0;
	while( 1 ){
		if( (size_t)rowCursor < (size_t)WorkingSurfaceDrawBegin ){ return; }
		zeroStart = leftOffset + rowCursor;
		runCount = dwordRun;
		do{
			*(uint*)zeroStart = 0;
			zeroStart += 4;
			--runCount;
		}while( runCount );
		rowCursor = leftOffset + zeroStart - (WorkingWidth + Screen_LeftBorder);
		if( !leftOffset )
			break;
		leftOffset -= 2;
		++dwordRun;
	}
	leftOffset2 = 2;
	dwordRun2 = 15;
	do{
		zeroStart2 = leftOffset2 + rowCursor;
		runCount2 = dwordRun2;
		do{
			*(uint*)zeroStart2 = 0;
			zeroStart2 += 4;
			--runCount2;
		}while( runCount2 );
		rowCursor = leftOffset2 + zeroStart2 - (WorkingWidth + Screen_LeftBorder);
		--dwordRun2;
		leftOffset2 += 2;
	}while( leftOffset2 != 32 );
}

//----- (00467322) --------------------------------------------------------
void ZeroCursorImageWidth()
{
	CurCursorWidth = 0;
	DrawnCursorWIdth = 0;
}

//----- (th2) -------------------------------------------------------------
void __fastcall DrawMissile(Missile& mis, int mayBeIsExploding, int playerOffsetX, int playerOffsetY, int shiftY, int tileInCellIndex)
{
	if( mis.HasActSprite && mis.timeFrame >= mis.drawDelay && mis.MayBeIsExploding == mayBeIsExploding && mis.curAnimationPtr
		&& mis.FrameIndex >= 1 && *(int*) mis.curAnimationPtr <= 100 && mis.FrameIndex <= *(int*) mis.curAnimationPtr ){
		int x = playerOffsetX + mis.ColDisplacement - mis.animOffset; // here missile coord
		int y = playerOffsetY + mis.RowDisplacement;
		switch(mis.BaseMissileIndex){
		case MI_139_THROWING_KNIFE: y -= 35; break;
		case MI_142_AURA_HIT_GFX: {
			Monster& monster = Monsters[mis.value[2]];
			int h = monster.CurAction == A_3_WALK_HORIZ;
			int r = ((monster.Row + monster.NextRow * h) >> h) - mis.value[3];
			int c = ((monster.Col + monster.NextCol * h) >> h) - mis.value[4];
			y += monster.DeltaY + (r + c) * (16 - 32 * h);
			x += monster.DeltaX + (r - c) * 32;
			if( ! SpriteInScreen(x, y) ) return;
			} break;
		}
		// (th4) reflect icon: remember where it landed -- see the note by g_ReflectIconSurfX.
		if( mis.BaseMissileIndex == MI_79_REFLECT && mis.CasterIndex == CurrentPlayerIndex ){
			// x is the sprite's LEFT edge (animOffset was subtracted above) and animWidth is
			// its pitch, so +animWidth/2 is the horizontal centre. y is its BOTTOM, because
			// CEL frames are drawn upward from the anchor, so the visual centre sits a fixed
			// lift above it. 132 is measured, not guessed: the old hardcoded version drew at
			// (player anchor - 116) and sat on the icon at the resolution it was tuned for,
			// and this icon's anchor is +16 below the player's, so 16 + 116 = 132. It is
			// sprite geometry, which does not vary with resolution.
			// VERTICAL comes from the icon. y is its BOTTOM (CEL frames draw upward from
			// the anchor), so the visual centre is a fixed lift above it. 132 is measured:
			// the old version drew at (player anchor - 116) and sat on the icon at the
			// resolution it was tuned for, and this icon's anchor is +16 below the
			// player's. Sprite geometry, so it does not vary with resolution.
			// HORIZONTAL does NOT come from the icon: mis.animWidth is the CEL PITCH, the
			// buffer width, not the width of the visible glyph, so x + animWidth/2 lands
			// well left of the shield. The old code centred on the PLAYER horizontally and
			// was correct there, so g_CurPlayerSurfX is used for x instead -- see below.
			// Lift from the icon's draw anchor (its BOTTOM -- CEL frames draw upward) to
			// where the count should sit. This is SPRITE GEOMETRY: it is the same number
			// at every resolution and window size, because the anchor it is applied to is
			// the icon's own draw position rather than Xofs/Yofs.
			// Bracketed by eye against the shield: 132 left the glyph below centre, 146
			// left it above, so 139 is the middle. It cannot be derived from animWidth --
			// that is the CEL PITCH (160), while the visible shield occupies only part of
			// that buffer, so the visible centre is not the buffer centre.
			const int kReflectSpriteCentreLift = 139;
			g_ReflectIconSurfX = x;				// kept for reference / future tuning only
			g_ReflectIconSurfY = y - kReflectSpriteCentreLift;
			g_ReflectIconDrawn = true;
		}
		if( mis.uniqTrans ){
			CL2_DrawDark(x, y, mis.curAnimationPtr, mis.FrameIndex, mis.animWidth, shiftY, tileInCellIndex, mis.uniqTrans + 3);
		}else{
			// Option C: save for post-render LightSurface re-draw
			if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_LightSurfaceOnlyPass) {
				if (g_PostRenderSpriteCount < MAX_POSTRENDER_SPRITES) {
					auto& cmd = g_PostRenderSprites[g_PostRenderSpriteCount++];
					cmd.x = x; cmd.y = y;
					cmd.cl2Buf = mis.curAnimationPtr; cmd.frameNum = mis.FrameIndex;
					cmd.pitch = mis.animWidth; cmd.shiftY = shiftY;
					cmd.tileInCellIndex = tileInCellIndex;
					cmd.darken = (mis.flag & MSF_LIGHT) != 0;
					cmd.darkLevel = (uchar)DarkLevel;
					cmd.spriteType = SpriteType::Missile;  // (th4)
					// (th4) Stage 3 owner-pixel exclusion: a missile that casts
					// a colored light keeps its own sprite untinted — the replay
					// writes white tint at its pixels. Keyed on the same side
					// table the light fill reads.
					cmd.tintExclude = mis.LightIndex >= 0
						&& (LightTintList[mis.LightIndex].r != 255
						 || LightTintList[mis.LightIndex].g != 255
						 || LightTintList[mis.LightIndex].b != 255);
				}
			}
//			CL2_Draw(x, y, mis.curAnimationPtr, mis.FrameIndex, mis.animWidth, shiftY, tileInCellIndex, mis.flag & MSF_LIGHT);
			{
				// (th4) town night: missile-drawn effects (the Abyss entrance visuals above all)
				// darken with the dusk phase like everything else in town. nightDark is 0 outside
				// town / by day, so dungeons and daytime keep the original MSF_LIGHT behaviour.
				int nightDark = DayNight_TownSpriteDark();
				int nightPrev = DarkLevel;
				bool misDarken = (mis.flag & MSF_LIGHT) != 0;
				if( nightDark > 0 ){ misDarken = true; if( nightDark > DarkLevel ) DarkLevel = nightDark; }
				CL2_Draw(x, y, mis.curAnimationPtr, mis.FrameIndex, mis.animWidth, shiftY, tileInCellIndex, misDarken);
				DarkLevel = nightPrev;
			}
		}
	}
}

//----- (00467331) --------------------------------------------------------
void __fastcall DrawMissileInTile(int row, int col, int playerOffsetX, int playerOffsetY, int shiftY, int tileInCellIndex, int mayBeIsExploding)
{
	MissileIndexT castNum = MissilesMap[ row ][ col ];
	if( castNum == 0 ) return; // (rt) the original has no such check, since FlagMap and MissilesMap must not diverge
							   //(rt) castNum = -1;// Set this way for now: while it iterates everything draws correctly; once it stops iterating it draws only every other time.
	if( castNum == Missile_Many ){
		for( int i = 0; i < MissileAmount; i++){
			uint missileIndex = MissileIndexes[i];
			if( missileIndex >= MissilesMax ) break;
			Missile& cast = Missiles[missileIndex];
			if( cast.Row == row && cast.Col == col ){
				DrawMissile(cast, mayBeIsExploding, playerOffsetX, playerOffsetY, shiftY, tileInCellIndex);
			}
		}
	}else{
		Missile& cast = Missiles[castNum - 1];
		DrawMissile(cast, mayBeIsExploding, playerOffsetX, playerOffsetY, shiftY, tileInCellIndex);
	}
}

//----- (00467679) --------------------------------------------------------
void __fastcall Draw_Dying_Player(int row, int col, int playerOffsetX, int playerOffsetY, int shiftY, int tileInCellIndex, int unusedParam)
{
	int curAnimPtr;          // ecx@9 — pointer to player's current animation CEL data
	int curAnimFrame;          // edx@10 — player's current animation frame number
	unsigned int curAnim; // ecx@11 — frame count read from animation header
	FlagMap[ row ][ col ] &= ~CF_4_DEAD_PLAYER;
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && !player.CurLife && player.dungeon == Dungeon && player.Row == row && player.Col == col ){
			curAnimPtr = (int)player.currentAnimationPointer;
			if( ! curAnimPtr ){
				break;
			}
			curAnimFrame = player.currentAnimationFrame;
			if( curAnimFrame < 1 ){
				break;
			}
			curAnim = *(uint*)curAnimPtr;
			if( curAnim > 70 ){
				break;
			}
			if( curAnimFrame > (int)curAnim ){
				break;
			}
			FlagMap[ row ][ col ] |= CF_4_DEAD_PLAYER;
			// SMutant death animation offsets
			if (player.gameChanger & BIT(GC_28_SUPERMUTANT) && player.CurAction == PCA_8_DEATH) {
				player.xOfs = 0;
				player.yOfs = 24;
			}
			else if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER) && player.CurAction == PCA_8_DEATH) {
				player.xOfs = 0;
				player.yOfs = 2;
			}
			DrawPlayer(playerIndex, row, col, playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, shiftY, tileInCellIndex);
		}
	}
}


//----- overhead HP bars (Display_Player_HP_bars / Display_Monster_HP_bars) -----
bool g_OutlineDbgLog = false;	// (th4) TH4_OUTLINE_DEBUG: file scope (not in the namespace below) so Engine.cpp can read it
namespace {
	const int  OverheadBar_Width = 40; // bar width in px
	const int  OverheadBar_Height = 2;  // bar fill height in px
	const int  OverheadBar_Gap = 15; // px gap above the sprite's top edge
	const int  OverheadBar_DefaultHeight = 80; // fallback when a monster's SpriteHeight is 0 (unset)
	const int  OverheadBar_PlayerHeight = 78; // fixed player sprite height (players are ~same height; ignore per-animation size)

	// Monsters whose overhead HP bar looks wrong and should be suppressed.
	// Add baseMonsterIndex (BM_*) values here to hide their bar.
	bool OverheadBar_IsSuppressed(int baseMonsterIndex)
	{
		switch (baseMonsterIndex) {
		case BM_260_ARMAGEDDON_WALKER:
		case BM_259_WATER_WYRM:
		case BM_263_RED_WYRM:
			return true;
		default:
			return false;
		}
	}
	const int  OverheadBar_Lift = 90; // fixed px lift above the sprite's feet (no per-sprite height)
	const uchar OverheadBar_Outline = 0;  // near-black outline
	const uchar OverheadBar_Red = 234; // hostile fill (same red the monster status bar HP uses)
	// (th4) Alt lootable-reveal ring colours. 194 is the gold DrawObjectSprite already
	// uses for a hovered/Alt object; 234 is the SAME red as the hostile HP bar above, so
	// the palette entry is one the player already reads as "danger" elsewhere.
	const uchar AltLoot_Ring        = 194;
	const uchar AltLoot_RingTrapped = 234;
	const uchar OverheadBar_Blue = 162; // friendly fill (bluish)
	const uchar OverheadBar_Gold = 147; // (th4) boss fill: unique monsters (newBossId). Saturated acid yellow (F4F100) from the fixed 144-151 yellow ramp

	// One horizontal run of pixels on the working surface, clipped to screen.
	void OverheadBar_Row(int sx, int sy, int width, uchar color)
	{
		if (width <= 0) return;
		if (sy < 0 || sy >= MaxScreenHeight) return;
		if (sx < 0) { width += sx; sx = 0; }
		if (sx + width > MaxScreenWidth) width = MaxScreenWidth - sx;
		if (width <= 0) return;
		const int rowOfs = YOffsetHashTable[sy] + sx;
		memset(&WorkingSurface[rowOfs], color, width);
		if (LightSurface) memset(LightSurface + rowOfs, 0, width); // full brightness
		// (th4) Stage 2: same for the alpha overlay under the bar row (bars are
		// flushed after the replay, so the overlay must not composite over them).
		AlphaOverlayClearRow(&WorkingSurface[rowOfs], width);
		// (th4) Stage 3 (2026-09-05): same for the colored-light tint — the
		// overhead HP bar is opaque UI ink, no world light color on it.
		TintClearRow(&WorkingSurface[rowOfs], width);
	}

	// Draws an outlined HP bar centered at centerX, with its bottom at bottomY.
	void DrawOverheadHpBar(int centerX, int bottomY, int curLife, int maxLife, uchar fillColor)
	{
		if (maxLife <= 0) return;
		int fill = OverheadBar_Width * curLife / maxLife;
		if (fill < 0) fill = 0;
		if (fill > OverheadBar_Width) fill = OverheadBar_Width;

		const int left = centerX - OverheadBar_Width / 2;
		const int top = bottomY - OverheadBar_Height;

		// 1px dark outline box around the whole bar.
		OverheadBar_Row(left - 1, top - 1, OverheadBar_Width + 2, OverheadBar_Outline); // top edge
		OverheadBar_Row(left - 1, top + OverheadBar_Height, OverheadBar_Width + 2, OverheadBar_Outline); // bottom edge
		for (int i = 0; i < OverheadBar_Height; ++i) {
			OverheadBar_Row(left - 1, top + i, 1, OverheadBar_Outline); // left edge
			OverheadBar_Row(left + OverheadBar_Width, top + i, 1, OverheadBar_Outline); // right edge
		}

		// Fill (colored) + remaining track (dark).
		for (int i = 0; i < OverheadBar_Height; ++i) {
			if (fill > 0) OverheadBar_Row(left, top + i, fill, fillColor);
			if (fill < OverheadBar_Width) OverheadBar_Row(left + fill, top + i, OverheadBar_Width - fill, OverheadBar_Outline);
		}
	}

	// (th4) Overhead bars are QUEUED here and flushed once the whole scene is done, rather
	// than being drawn where they are computed. They used to be memset straight into
	// WorkingSurface from inside DrawMonster/DrawPlayer -- that is, in the middle of the
	// back-to-front tile loop -- so any tile belonging to a cell drawn afterwards (an arch,
	// a vault) simply painted over them.
	//
	// EnrichedLight is what made it obvious rather than what caused it. The a8l8 post-render
	// pass in DrawIsoLand re-runs SPRITES with g_LightSurfaceOnlyPass, and Engine.cpp (~995)
	// gives them a draw-through-wall silhouette off WallOcclusionSurface. A raw memset is not
	// a sprite, never entered that list, and got none of it -- so the monster showed over the
	// vault and its bar did not. Worse, OverheadBar_Row zeroes LightSurface without marking
	// WallOcclusionSurface, and the wall draw skips its own LightSurface write under PPL
	// (Engine.cpp ~1004), which left a bar-shaped patch of the arch at full brightness.
	//
	// Flushing after the scene puts bars unconditionally on top and fixes both, in every
	// lighting mode, without touching the a8l8 path.
	// (th4) ALT LOOTABLE OUTLINES -- same deferral, same reason as the bars below.
	// Queued from DrawMapCell during the tile loop, flushed after the whole scene so a
	// cell drawn later cannot paint over the ring. Only the ring is drawn (never the
	// body), so a chest behind a wall reads as a silhouette instead of showing through.
	// Not gated on any lighting mode: the occlusion this solves exists in all of them.
	struct AltLootCmd { char* cel; int celIndex, celWidth, x, y, upHalfCell, screenCellRow; uchar ring; bool drawnInline;
	                    int dbgObj, dbgRow, dbgCol; };	// (th4) dbgObj = object index, also used by the one-ring-per-object merge; dbgRow/Col: TH4_OUTLINE_DEBUG
	const int MAX_ALT_LOOT = 256;
	AltLootCmd g_AltLoot[MAX_ALT_LOOT];
	int g_AltLootCount = 0;

	struct OverheadBarCmd { int centerX, bottomY, curLife, maxLife; uchar fillColor; };
	const int MAX_OVERHEAD_BARS = 256;
	OverheadBarCmd g_OverheadBars[MAX_OVERHEAD_BARS];
	int g_OverheadBarCount = 0;

	void QueueOverheadHpBar(int centerX, int bottomY, int curLife, int maxLife, uchar fillColor)
	{
		SnowDepth_Measure();	// (th4) SNOW DEPTH (Snowfall.cpp THE HP BARS): the body's height, before the bar adds to it
		// (th4) DELIBERATELY GATED. The deferred path exists only for the a8l8 renderer,
		// whose post-render pass is what created the mismatch in the first place. With
		// EnrichedLight off there is no LightSurface, no WallOcclusionSurface and no
		// silhouette pass, so bars stay exactly where they always were -- drawn inline,
		// occluded by later tiles. Same condition the post-render pass itself uses.
		if (!(g_A8L8Lighting && PixelLight)) {
			DrawOverheadHpBar(centerX, bottomY, curLife, maxLife, fillColor);
			return;
		}
		// The post-render passes call DrawPlayer a second time; without this guard every
		// player bar would be queued twice.
		if (g_LightSurfaceOnlyPass) return;
		if (maxLife <= 0) return;
		if (g_OverheadBarCount >= MAX_OVERHEAD_BARS) return;
		OverheadBarCmd& cmd = g_OverheadBars[g_OverheadBarCount++];
		cmd.centerX = centerX;
		cmd.bottomY = bottomY;
		cmd.curLife = curLife;
		cmd.maxLife = maxLife;
		cmd.fillColor = fillColor;
	}

	void QueueAltLootOutline(int objectIndex, int row, int col, int playerOffsetX, int playerOffsetY, int screenCellRow, bool drawnInline)
	{
		if (g_LightSurfaceOnlyPass) return;          // post-render passes must not re-queue
		if (g_AltLootCount >= MAX_ALT_LOOT) return;
		if ((uint)objectIndex >= MaxObjects_127) return;
		const Object& o = Objects[objectIndex];
		if (!o.CelFilePtr || o.FrameIndex < 1) return;
		const int maxFrame = *(int*)o.CelFilePtr;   // same sanity DrawObjectSprite applies
		if (maxFrame > 50 || o.FrameIndex > maxFrame) return;
		// (th4) ONE RING PER OBJECT PER FRAME. Wide objects (sarcophagi are two tiles) are met
		// by DrawMapCell once per cell they cover, and each visit used to queue its own ring.
		// Identical rings on top of each other are harmless, but near the top edge of the screen
		// the two cells sit in different screen rows and got DIFFERENT top-edge cuts (below), and
		// two differently cut rings combined into a broken outline. So a repeat visit only merges:
		// the whole ring is kept unless EVERY visiting cell drew the object inline.
		for (int i = 0; i < g_AltLootCount; ++i) {
			if (g_AltLoot[i].dbgObj == objectIndex) {
				g_AltLoot[i].drawnInline = g_AltLoot[i].drawnInline && drawnInline;
				return;
			}
		}
		// (th4) Position exactly as DrawObjectSprite computes it for a static object
		// (InAction == 0): the sprite hangs off the cell that OWNS it, which is not
		// necessarily the cell being drawn, so the row/col delta has to be applied.
		const int rowDelta = o.Row - row;
		const int colDelta = o.Col - col;
		AltLootCmd& cmd = g_AltLoot[g_AltLootCount++];
		cmd.cel           = o.CelFilePtr;
		cmd.celIndex      = o.FrameIndex;
		cmd.celWidth      = o.Width;
		cmd.x             = playerOffsetX + 32 * rowDelta - o.OffsetX - 32 * colDelta;
		cmd.y             = playerOffsetY + 16 * (colDelta + rowDelta);
		cmd.upHalfCell    = 0;
		// (th4) ALWAYS THE WHOLE SPRITE (8 = no cut). The caller's tileInCellIndex is the
		// original Diablo top-of-screen rule, min(2*screenCellRow+2, 8) (~4843): sprites in the
		// top screen rows are drawn from their bottom 32*n rows only, because D1 had nothing above
		// the screen. A ring traced from a cut sprite follows the cut line as if it were an edge.
		// TH4 has Screen_TopBorder (160) rows above the screen, and DrawItemOutlineOnMap
		// (Engine.cpp) refuses any box that would leave the surface, so the whole ring is safe.
		(void)screenCellRow;
		cmd.screenCellRow = 8;
		// (th4) Trapped objects and explosive pods ring RED -- see IsAwarenessDanger.
		cmd.ring = IsAwarenessDanger( o ) ? AltLoot_RingTrapped : AltLoot_Ring;
		// Was the object drawn normally this frame? If so its inline outline already has
		// correct z-order and this pass must only add the part a WALL covered -- otherwise
		// the ring lands on top of the player and monsters. If it was not drawn (dark
		// cell) there is nothing to preserve and the whole ring is kept.
		cmd.drawnInline = drawnInline;
		cmd.dbgObj = objectIndex; cmd.dbgRow = row; cmd.dbgCol = col;
	}

	// ===================== DEBUG INSTRUMENTATION (th4) -- ALT OUTLINES =====================
	// Set TH4_OUTLINE_DEBUG to 0 to disable. Observes only.
	// Investigating a broken Alt outline on an object near the TOP edge of the screen.
	// Logs to TH4_outline.log, only when the set of queued outlines changes (holding Alt
	// still gives one block): per command the object, the cell that queued it, the
	// tileInCellIndex cut (8 = whole sprite), whether it was drawn inline (-> ring kept
	// only over walls), screen position. (The matching per-ring geometry lines lived in
	// Engine.cpp DrawItemOutlineOnMap and were removed after the fix; g_OutlineDbgLog is the
	// hook to re-add them there if this ever needs looking at again.)
	#define TH4_OUTLINE_DEBUG 0
	void FlushAltLootOutlines()
	{
	#if TH4_OUTLINE_DEBUG
		static unsigned lastSig = 0;
		unsigned sig = 2166136261u;
		for (int i = 0; i < g_AltLootCount; ++i) {
			const AltLootCmd& c = g_AltLoot[i];
			sig = (sig ^ (unsigned)(c.dbgObj * 131 + c.dbgRow * 7 + c.dbgCol * 3 + c.screenCellRow * 11 + c.drawnInline)) * 16777619u;
			sig = (sig ^ (unsigned)(c.x * 5 + c.y)) * 16777619u;
		}
		g_OutlineDbgLog = g_AltLootCount > 0 && sig != lastSig;
		if (g_AltLootCount > 0) lastSig = sig;
		if (g_OutlineDbgLog) if (FILE* f = fopen("TH4_outline.log", "a")) {
			fprintf(f, "=== frame %u  outlines=%d  Screen %dx%d  TopBorder %d\n", (unsigned)WorldFrameIndex, g_AltLootCount, ScreenWidth, ScreenHeight, (int)Screen_TopBorder);
			fclose(f);
		}
	#endif
		for (int i = 0; i < g_AltLootCount; ++i) {
			const AltLootCmd& c = g_AltLoot[i];
	#if TH4_OUTLINE_DEBUG
			if (g_OutlineDbgLog) if (FILE* f = fopen("TH4_outline.log", "a")) {
				const Object& o = Objects[c.dbgObj];
				fprintf(f, "  obj=%d base=%d objRC(%d,%d) queuedFromRC(%d,%d) cut=%d inline=%d xy(%d,%d) w=%d frame=%d\n",
					c.dbgObj, (int)o.BaseObjectIndex, o.Row, o.Col, c.dbgRow, c.dbgCol, c.screenCellRow, (int)c.drawnInline,
					c.x, c.y, c.celWidth, c.celIndex);
				fclose(f);
			}
	#endif
			DrawItemOutlineOnMap(c.x, c.y, c.cel, c.celIndex, c.celWidth,
			                     c.upHalfCell, c.screenCellRow, c.ring, c.drawnInline);
		}
		g_AltLootCount = 0;
	#if TH4_OUTLINE_DEBUG
		g_OutlineDbgLog = false;
	#endif
	}

	void ResetOverheadHpBars()
	{
		g_OverheadBarCount = 0;
	}

	// Called from DrawIsoLand after the tile loop and both post-render passes, but BEFORE
	// the zoom block -- the queued coordinates are pre-zoom 1x, same as g_PostRenderSprites.
	void FlushOverheadHpBars()
	{
		for (int i = 0; i < g_OverheadBarCount; ++i) {
			const OverheadBarCmd& cmd = g_OverheadBars[i];
			DrawOverheadHpBar(cmd.centerX, cmd.bottomY, cmd.curLife, cmd.maxLife, cmd.fillColor);
		}
		g_OverheadBarCount = 0;
	}

	// Real decoded pixel height of a CL2 frame = total decoded pixels / width.
	// Counts run lengths exactly as Rle2Draw decodes them (no surface writes).
	int Cl2FrameHeight(char* cl2Buf, int frameNum, int pitch)
	{
		if (!cl2Buf || frameNum < 1 || pitch <= 0) return 0;
		uint* header = (uint*)cl2Buf;
		uint frameCount = header[0];
		if ((uint)frameNum > frameCount) frameNum = frameCount;

		uchar* from = (uchar*)cl2Buf + header[frameNum];
		int    rleEnd = (int)(header[frameNum + 1] - header[frameNum]); // frame length in bytes

		// First ushort of the frame's offset table = byte offset where the RLE begins.
		// (Decoding from the start of the frame would mis-read the offset table as RLE.)
		int rleStart = *(ushort*)from;
		if (rleStart <= 0 || rleStart >= rleEnd) return 0;

		uchar* src = from + rleStart;
		int size = rleEnd - rleStart;

		int pixels = 0;
		for (int i = 0; i < size; ) {
			uchar blockSize = src[i++];
			if (blockSize > 190) {            // colored: N pixels, each with its own color byte
				int n = 65 - (blockSize - 191);
				pixels += n;
				i += n;
			}
			else if (blockSize > 127) {       // solid: N pixels sharing one color byte
				int n = 63 - (blockSize - 128);
				pixels += n;
				i += 1;
			}
			else {                            // transparent: N skipped pixels
				pixels += blockSize;
			}
		}
		return pixels / pitch;
	}
}

//----- (0046775B) --------------------------------------------------------
void __fastcall DrawPlayer(int playerIndex, int row, int col, int rowOffset, int colOffset, char* currentAnimationPointer, int currentAnimationFrame, int pitch, int shiftY, int screenCellRow)
{
	if( (FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR ||	Players[CurrentPlayerIndex].Infravision || !Dungeon->isQuest && Dungeon == DUN_0_TOWN )
	 && currentAnimationPointer && currentAnimationFrame >= 1 && *(int*)currentAnimationPointer <= 50 && currentAnimationFrame <= *(int*)currentAnimationPointer ){
		// Save player draw params for post-render LightSurface-only re-draw pass
		// Save for ALL player indices so multiplayer co-op glow works correctly.
		if (!g_LightSurfaceOnlyPass && (uint)playerIndex < PlayersMax_4) {
			auto& cmd = g_ReDraw[playerIndex];
			cmd.valid = true;
			cmd.playerIndex = playerIndex;
			cmd.row = row; cmd.col = col;
			cmd.rowOffset = rowOffset; cmd.colOffset = colOffset;
			cmd.animPtr = currentAnimationPointer;
			cmd.frame = currentAnimationFrame;
			cmd.pitch = pitch;
			cmd.shiftY = shiftY;
			cmd.screenCellRow = screenCellRow;
			cmd.spriteType = SpriteType::Player;  // (th4)
			cmd.darkLevel = -1;
		}
		// During LightSurface-only re-draw pass, only call CL2_Draw (no outline, no HP bar)
		if (g_LightSurfaceOnlyPass) {
			if ((uint)playerIndex < PlayersMax_4 && g_ReDraw[playerIndex].valid) {
				const auto& cmd = g_ReDraw[playerIndex];
				if (cmd.darkLevel >= 0) {
					// (th4) THIEF under A8L8: the post-render pass is what actually sets the
					// player's LightSurface (the main pass skips it), and the plain Player
					// replay pins it to full bright -- which is why she glowed in the dark
					// with EnrichedLight on. Replay her darkened with the cell shade captured
					// in the main pass, so A8L8 matches the non-A8L8 look: dark in the dark,
					// lit beside a torch, faint silhouette at the cap.
					int prevDark = DarkLevel;
					DarkLevel = cmd.darkLevel;
					CL2_Draw(cmd.rowOffset, cmd.colOffset, cmd.animPtr, cmd.frame, cmd.pitch, cmd.shiftY, cmd.screenCellRow, true);
					DarkLevel = prevDark;
				} else {
					CL2_Draw(cmd.rowOffset, cmd.colOffset, cmd.animPtr, cmd.frame, cmd.pitch, cmd.shiftY, cmd.screenCellRow);
				}
			}
			return;
		}
		if( playerIndex == Cur.playerIndex ){
			CL2_DrawOutline(165, rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch, shiftY, screenCellRow);
		}
		if( playerIndex == CurrentPlayerIndex ){
			// (th4) THIEF: every other hero is deliberately drawn FULL BRIGHT here (the
			// no-darken CL2_Draw ignores DarkLevel entirely) -- which is why she glowed in
			// pitch darkness. She alone is shaded by her tile's ambient light: dark in the
			// dark, lit beside a torch. The cap keeps a faint silhouette so the PLAYER can
			// always find her even when monsters cannot -- her partial invisibility.
			if( MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Thief) ){
				int thiefPrevDark = DarkLevel;
				enum { THIEF_SILHOUETTE_DARK_CAP = 12 };	// 15 would erase her completely; tune freely
				if( DarkLevel > THIEF_SILHOUETTE_DARK_CAP ) DarkLevel = THIEF_SILHOUETTE_DARK_CAP;
				// (th4) A8L8: remember the shade for the post-render replay (see above).
				if (!g_LightSurfaceOnlyPass && (uint)playerIndex < PlayersMax_4) {
					g_ReDraw[playerIndex].darkLevel = DarkLevel;
					g_ReDraw[playerIndex].spriteType = SpriteType::PlayerShaded;
				}
				CL2_Draw(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch, shiftY, screenCellRow, true);
				DarkLevel = thiefPrevDark;
			}else{
				CL2_Draw(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch, shiftY, screenCellRow);
			}
		}else if( !(FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR) || Players[CurrentPlayerIndex].Infravision && DarkLevel > 8 ){
			CL2_DrawDark(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch, shiftY, screenCellRow, 1);
		}
		else {
			int prevPal = DarkLevel;
			if (DarkLevel >= 5) {
				DarkLevel -= 5;
			}
			else {
				DarkLevel = 0;
			}
			CL2_Draw(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch, shiftY, screenCellRow, true);
			DarkLevel = prevPal;
		}

		Breath_NotePlayer( playerIndex, rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch );	// (th4) COLD BREATH (Breath.cpp)
		if( Dungeon == DUN_44_RAVENHOLM ) TownWater_NotePlayer( playerIndex, rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, pitch );	// (th4) RAVENHOLM'S RIVER: hero reflections (TownWater.cpp)

		// (th4) player sprite centre, for the reflect charge count. Same expression the HP
		// bar below uses for centerX, which is correct at every resolution because it comes
		// from the tile the sprite was drawn on rather than from Xofs.
		if( playerIndex == CurrentPlayerIndex && !g_LightSurfaceOnlyPass ){
			g_CurPlayerSurfX = rowOffset + pitch / 2;
			g_CurPlayerSurfY = colOffset - 78 / 2;	// (th4) DYNAMIC ZOOM centre; 78 = OverheadBar_PlayerHeight
			g_CurPlayerDrawn = true;
		}

		// Overhead HP bar (config-gated). Players are friendly/blue.
		if (Display_Player_HP_bars) {
			const Player& barPlayer = Players[playerIndex];
			if (barPlayer.CurAction != PCA_8_DEATH && barPlayer.MaxCurLife > 0) {
				const int centerX = rowOffset + pitch / 2;
				const int barBot = colOffset - OverheadBar_PlayerHeight - OverheadBar_Gap;
				QueueOverheadHpBar(centerX, barBot, barPlayer.CurLife, barPlayer.MaxCurLife, OverheadBar_Blue);
			}
		}
	}
}

void* MotionInterpolationFunctions[] = {
	(void( __fastcall * )(int))ActTrash16SpellEffect,
	ActAbyssEnterExplodeFragment,
	ActAcidSplash,
	ActApocalypse,
	ActApocExplode,
	ActArrow,
	ActBoneSpirit,
	ActChargedBolt,
	ActHydramancerChargedBolt,
	ActElemental,
	ActEthereal,
	ActFireArcanBolt,
	ActFireblast,
	ActFireRing,
	ActFireWall,
	ActFireWallSegment,
	ActFlashBack,
	ActFlashForward,
	ActFury,
	ActGolem,
	ActForcewave,
	ActForcewaveSegment,
	ActHolyBolt,
	ActHydra,
	ActIncinerate,
	ActIncinerateSegment,
	ActInfravision,
	ActLightningNovaSegment,
	ActLightningSegment,
	ActLightningAsArrow,
	ActLightning,
	ActLightningRing,
	ActLightningWall,
	ActNova,
	ActMagicArrow,
	ActMagicExplosion,
	ActManaShield,
	ActMonstersRun,
	ActNullSpell,
	ActRedPortal,
	ActReflect,
	ActRune,
	ActSomeAnotherFlashBk,
	ActSomeAnotherFlashFr,
	ActSomeAnotherMonsterRun,
	ActSpawnMeatBall,
	ActSpecialArrowSpell,
	ActSpecialEffect,
	ActSpell51_KRULL,
	ActSpell59AcidPools,
	ActSpell62_RisingLight,
	ActSpell70LightningWallSegment,
	ActSpell88FireBall,
	ActStoneCurse,
	ActTeleport,
	ActTownPortal,
	ActWeaponElementalDamage,
	ActAuraElementalGFX,
    ActFlaskThrow
};

struct MisPrev
{
	int DeltaX;
	int DeltaY;
	int WholeDeltaX;
	int WholeDeltaY;
	int RowDisplacement;
	int ColDisplacement;
	int Row;
	int Col;
	MissileIndexT MisMap;
	MissileIndexT MisMapPrev;
	uchar ForbMap;
	uchar ForbMapPrev;
	int CasterRow;
	int CasterCol;
};
MisPrev MissilePrev[ MissilesMax ];
struct MonPrev
{
	int DeltaX;
	int DeltaY;
};
MonPrev MonsterPrev[ Monsters_Max_Count ];

int PlayerDeltaX, PlayerDeltaY, PlayerShiftX, PlayerShiftY;
bool PlayerInterpolated;

//int fullCellDelta;
//int nextCellPCmd;
int PlayerNextRow = 0, PlayerNextCol = 0;
int PlayerNextNextRow = 0, PlayerNextNextCol = 0;

// 80 fps motion interpolation: screen scrolling, spell moving, monster moving (not yet)
void MotionInterpolationBegin( int &offsetX, int &offsetY )
{
	Player& player = Players[ CurrentPlayerIndex ];
	double ic = (double)InterpolationFrame / FramesMax; // interpolation coefficient
	PlayerInterpolated = false;
	int scrollDir = Scroll.dir;
	if( scrollDir > 0 ){
		//int numberOfSubSteps = DungeonLevel || IsQuestFloor ? PlayerClasses[player.ClassID].NumberOfSubSteps : 8;
		bool fullCellDelta = /*player.CurAnimFrameCount >= numberOfSubSteps*/ false;
		switch( scrollDir ){
		case SDIR_1_U : fullCellDelta = player.yOfs == -32; break; //  🡱	Up
		case SDIR_2_UR:	fullCellDelta = player.yOfs == -16; break; //  🡵	Up Right
		case SDIR_3_R :	fullCellDelta = player.xOfs ==  32; break; //  🡲	Right
		case SDIR_4_DR:	fullCellDelta = player.yOfs ==   0; break; //  🡶	Down Right
		case SDIR_5_D :	fullCellDelta = player.yOfs ==   0; break; //  🡳	Down
		case SDIR_6_DL:	fullCellDelta = player.yOfs ==   0; break; //  🡷	Down Left
		case SDIR_7_L :	fullCellDelta = player.xOfs == -32; break; //  🡰	Left 
		case SDIR_8_UL:	fullCellDelta = player.yOfs == -16; break; //  🡴	Up Left
		}
		if( fullCellDelta ){
			// Repeat_Mouse_LButtonDown condition
			//if( IsAltPressed && MouseButtonPressed == MBP_1_LEFT && ! RButtonEvent && (EasyClick || IsShiftPressed)
			// && (!PanelClick || EnteredPanelWithAutoClick) && ShiftReleasedEvent
			// && gbBufferMsgs != BUFFER_ON && DungeonLevel == player.DungeonLevel ){
			//	scrollDir = OrientationToTarget(player.Row, player.Col, Cur.Row, Cur.Col);
			//	//scrollDir = 0;
			//}else{ 
				int nextCellPCmd = player.walkPath[0];
				int newScrollDir = by(nextCellPCmd + 1, 0, 0, 2, 8, 4, 6, 1, 3, 5, 7);
//				if (newScrollDir) {// OOB risk
				if( newScrollDir && (unsigned)nextCellPCmd < 9 ){
					switch( scrollDir ){
					case SDIR_1_U : PlayerNextRow = player.Row + player.CursorRow; PlayerNextCol = player.Col + player.CursorCol; break; //  🡱	Up
					case SDIR_2_UR: PlayerNextRow = player.Row + player.CursorRow; PlayerNextCol = player.Col + player.CursorCol; break; //  🡵	Up Right
					case SDIR_3_R :	PlayerNextRow = player.CursorRow;              PlayerNextCol = player.CursorCol;              break; //  🡲	Right
					case SDIR_4_DR:	PlayerNextRow = player.Row;                    PlayerNextCol = player.Col;                    break; //  🡶	Down Right
					case SDIR_5_D :	PlayerNextRow = player.Row;                    PlayerNextCol = player.Col;                    break; //  🡳	Down
					case SDIR_6_DL:	PlayerNextRow = player.Row;                    PlayerNextCol = player.Col;                    break; //  🡷	Down Left
					case SDIR_7_L :	PlayerNextRow = player.CursorRow;              PlayerNextCol = player.CursorCol;              break; //  🡰	Left 
					case SDIR_8_UL: PlayerNextRow = player.Row + player.CursorRow; PlayerNextCol = player.Col + player.CursorCol; break; //  🡴	Up Left
					}
					PlayerNextNextRow = PlayerNextRow - RowStep[nextCellPCmd], PlayerNextNextCol = PlayerNextCol - ColStep[nextCellPCmd];
					if( !CellFreeForPlayer(CurrentPlayerIndex, PlayerNextNextRow, PlayerNextNextCol) ){
						scrollDir = 0;
					}else{
						scrollDir = newScrollDir;
					}
				}else{
					scrollDir = 0;
				}
			//}
		}
		if( scrollDir > 0 ){
			PlayerInterpolated = true;
			int diagonal = 1 + (scrollDir & 1);
			int speedY = (1 + player.running) * 2 * diagonal; // 4 : 2
			int speedX = speedY * 2; // 8 : 4
			int x = (int)round(ic * speedX);
			int y = (int)round(ic * speedY);
			PlayerDeltaX = player.yOfs;
			PlayerDeltaY = player.xOfs;
			switch( scrollDir ){
			case SDIR_1_U :                                 player.yOfs -= y; offsetY += y; break; //  🡱	Up
			case SDIR_2_UR: offsetX -= x; player.xOfs += x; player.yOfs -= y; offsetY += y; break; //  🡵	Up Right
			case SDIR_3_R : offsetX -= x; player.xOfs += x;                                 break; //  🡲	Right
			case SDIR_4_DR: offsetX -= x; player.xOfs += x; player.yOfs += y; offsetY -= y; break; //  🡶	Down Right
			case SDIR_5_D :                                 player.yOfs += y; offsetY -= y; break; //  🡳	Down
			case SDIR_6_DL: offsetX += x; player.xOfs -= x; player.yOfs += y; offsetY -= y; break; //  🡷	Down Left
			case SDIR_7_L : offsetX += x; player.xOfs -= x;                                 break; //  🡰	Left 
			case SDIR_8_UL: offsetX += x; player.xOfs -= x; player.yOfs -= y; offsetY += y;	break; //  🡴	Up Left
			}
			PlayerShiftX = PlayerDeltaX - player.yOfs;
			PlayerShiftY = PlayerDeltaY - player.xOfs;
		}
	}
	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[ i ];
		Monster& monster = Monsters[ monsterIndex ]; MonPrev& monPrev = MonsterPrev[ monsterIndex ];
		if( monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
			monPrev.DeltaX = monster.DeltaX;
			monPrev.DeltaY = monster.DeltaY;
			int x = (int)round(ic * monster.SpeedX / 16);
			int y = (int)round(ic * monster.SpeedY / 16);
			if( !monster.SpeedX || !monster.SpeedY || abs(x) > 1 || abs(y) > 1 ){
				monster.DeltaX += x;
				monster.DeltaY += y;
			}
		} 
	}
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int missileIndex = MissileIndexes[ spellIndexIndex ];
		Missile& missile = Missiles[ missileIndex ]; MisPrev& misPrev = MissilePrev[ missileIndex ];
		misPrev.DeltaX = missile.DeltaX;
		misPrev.DeltaY = missile.DeltaY;
		misPrev.WholeDeltaX = missile.WholeDeltaX;
		misPrev.WholeDeltaY = missile.WholeDeltaY;
		misPrev.RowDisplacement = missile.RowDisplacement;
		misPrev.ColDisplacement = missile.ColDisplacement;
		misPrev.Row = missile.Row;
		misPrev.Col = missile.Col;
		misPrev.CasterRow = missile.CasterRow;
		misPrev.CasterCol = missile.CasterCol;
		missile.DeltaX = (int)round(ic * missile.DeltaX);
		missile.DeltaY = (int)round(ic * missile.DeltaY);
		if( missile.BaseMissileIndex == MI_13_MANA_SHIELD ){ //Dragon Mana shield
			ManaShieldAction( missileIndex );
		}else{
			BaseMissiles[ missile.BaseMissileIndex ].actFunc( missileIndex );
		}
		if( missile.Row != misPrev.Row || missile.Col != misPrev.Col ){
			misPrev.ForbMapPrev = FlagMap[ misPrev.Row ][ misPrev.Col ];
			misPrev.MisMapPrev = MissilesMap[ misPrev.Row ][ misPrev.Col ];
			misPrev.ForbMap = FlagMap[ missile.Row ][ missile.Col ];
			misPrev.MisMap = MissilesMap[ missile.Row ][ missile.Col ];
			PutMissile( missileIndex );
			if( misPrev.MisMapPrev == missileIndex + 1 ){
				MissilesMap[ misPrev.Row ][ misPrev.Col ] = 0;
			}
		}
	}
}

// 80 fps motion interpolation: screen scrolling, spell moving, monster moving (not yet)
void MotionInterpolationEnd()
{
	Player& player = Players[ CurrentPlayerIndex ];
	if( PlayerInterpolated ){
		player.yOfs = PlayerDeltaX;
		player.xOfs = PlayerDeltaY;
	}
	for( int spellIndexIndex = MissileAmount - 1; spellIndexIndex >= 0; --spellIndexIndex ){
		int missileIndex = MissileIndexes[ spellIndexIndex ];
		Missile& missile = Missiles[ missileIndex ]; MisPrev& mPrev = MissilePrev[ missileIndex ];
		missile.DeltaX = mPrev.DeltaX;
		missile.DeltaY = mPrev.DeltaY;
		missile.WholeDeltaX = mPrev.WholeDeltaX;
		missile.WholeDeltaY = mPrev.WholeDeltaY;
		if( missile.Row != mPrev.Row || missile.Col != mPrev.Col ){
			FlagMap[ missile.Row ][ missile.Col ] = mPrev.ForbMap;
			MissilesMap[ missile.Row ][ missile.Col ] = mPrev.MisMap;
			FlagMap[ mPrev.Row ][ mPrev.Col ] = mPrev.ForbMapPrev;
			MissilesMap[ mPrev.Row ][ mPrev.Col ] = mPrev.MisMapPrev;
		}
		missile.RowDisplacement = mPrev.RowDisplacement;
		missile.ColDisplacement = mPrev.ColDisplacement;
		missile.Row = mPrev.Row;
		missile.Col = mPrev.Col;
		missile.CasterRow = mPrev.CasterRow;
		missile.CasterCol = mPrev.CasterCol;
	}
	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[ i ];
		Monster& monster = Monsters[ monsterIndex ]; MonPrev& monPrev = MonsterPrev[ monsterIndex ];
		if( monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
			monster.DeltaX = monPrev.DeltaX;
			monster.DeltaY = monPrev.DeltaY;
		}
	}
}

// ===== Option C diagnostic: LightSurfaceBase =====
#if OPTION_C_DIAG_LIGHTSURFACEBASE
#include <cstdio>

static FILE* Diag_LogFile()
{
	static FILE* f = nullptr;
	if (!f) {
		// Write to a fixed path so it's easy to find.  If this fails (e.g.
		// permissions), fall back to the current directory.
		f = fopen("C:\\TH4_BUILD\\OptionC_diag.log", "w");
		if (!f) f = fopen("OptionC_diag.log", "w");
		if (f) {
			fprintf(f, "=== Option C LightSurface Diagnostic ===\n");
			fprintf(f, "WorkingWidth=%d WorkingHeight=%d A8L8=%d PixelLight=%d\n",
				WorkingWidth, WorkingHeight, g_A8L8Lighting, PixelLight);
			fflush(f);
		}
	}
	return f;
}

static int Diag_FrameNum = 0;

static void Diag_SnapshotLightSurface()
{
	if (LightSurfaceBase && LightSurface) {
		memcpy(LightSurfaceBase, LightSurface, (MaxWorkingHeight + 256) * MaxWorkingWidth);
		Diag_FrameNum++;
		FILE* f = Diag_LogFile();
		if (f) { fprintf(f, "FRAME %d snapshot taken\n", Diag_FrameNum); fflush(f); }
	}
}

static int Diag_CompareLightSurface(const char* checkpoint, int expectDiffs)
{
	if (!LightSurfaceBase || !LightSurface) return 0;
	FILE* f = Diag_LogFile();
	int diffs = 0, firstDiff = -1;
	int total = (MaxWorkingHeight + 256) * MaxWorkingWidth;
	for (int i = 0; i < total; i++) {
		if (LightSurface[i] != LightSurfaceBase[i]) {
			if (diffs == 0) firstDiff = i;
			diffs++;
			if (diffs <= 8 && f) {
				fprintf(f, "  [%d] %s diff#%d ofs=%d r=%d c=%d snap=%d curr=%d\n",
					Diag_FrameNum, checkpoint, diffs, i,
					i / WorkingWidth, i % WorkingWidth,
					(int)LightSurfaceBase[i], (int)LightSurface[i]);
			}
		}
	}
	if (f) {
		if (diffs == 0)
			fprintf(f, "FRAME %d %s: 0 diffs (clean)\n", Diag_FrameNum, checkpoint);
		else {
			fprintf(f, "FRAME %d %s: %d diffs (first ofs=%d r=%d c=%d)",
				Diag_FrameNum, checkpoint, diffs, firstDiff,
				firstDiff >= 0 ? firstDiff / WorkingWidth : -1,
				firstDiff >= 0 ? firstDiff % WorkingWidth : -1);
			if (diffs != expectDiffs && expectDiffs >= 0)
				fprintf(f, " *** UNEXPECTED");
			fprintf(f, "\n");
		}
		fflush(f);
	}
	return diffs;
}

// Per-cell contamination check.  Checks the FULL cell footprint (floor + wall layers)
// covering numLayers 32-pixel-high blocks stacked vertically, each 32 pixels wide,
// starting at 'floorDst' (bottom-left pixel of the floor sub-tile in WorkingSurface).
// Only reports the FIRST cell in the frame that introduces contamination.
static bool Diag_SubTileFound = false; // reset each frame

static void Diag_ResetSubTileCheck()
{
	Diag_SubTileFound = false;
}

static int Diag_CheckCellFull(const char* stage, int row, int col, uchar* floorDst, int numLayers, bool skipIfAlreadyFound)
{
	if (!LightSurfaceBase || !LightSurface) return 0;
	if (skipIfAlreadyFound && Diag_SubTileFound) return 0;
	FILE* f = Diag_LogFile();
	if (!f) return 0;

	int diffs = 0, firstOfs = -1;
	uchar firstSnap = 0, firstCurr = 0;
	intptr_t baseOffset = floorDst - WorkingSurface;

	// Check each layer from floor upward (layer 0 = floor, layer 1+ = walls)
	for (int layer = 0; layer < numLayers; layer++) {
		intptr_t layerBase = baseOffset - (intptr_t)layer * 32 * WorkingWidth;
		for (int h = 0; h < 32; h++) {
			intptr_t rowStart = layerBase - (intptr_t)h * WorkingWidth;
			for (int w = 0; w < 32; w++) {
				intptr_t ofs = rowStart + w;
				if (ofs < 0 || ofs >= (MaxWorkingHeight + 256) * MaxWorkingWidth) continue;
				uchar snap = LightSurfaceBase[ofs];
				uchar curr = LightSurface[ofs];
				if (snap != curr) {
					if (diffs == 0) {
						firstOfs = (int)ofs;
						firstSnap = snap;
						firstCurr = curr;
					}
					diffs++;
				}
			}
		}
	}

	if (diffs > 0) {
		Diag_SubTileFound = true;
		fprintf(f, "FRAME %d *** CONTAMINATION FOUND at cell(%d,%d) stage=%s: %d diffs in %d-layer footprint. First: ofs=%d r=%d c=%d snap=%d curr=%d DarkLevel=%d CurTileID=%d TransparentMode=%d PartialTrans=%d\n",
			Diag_FrameNum, row, col, stage, diffs, numLayers, firstOfs,
			firstOfs >= 0 ? (int)(firstOfs / WorkingWidth) : -1,
			firstOfs >= 0 ? (int)(firstOfs % WorkingWidth) : -1,
			(int)firstSnap, (int)firstCurr,
			DarkLevel, CurTileID, TransparentMode, PartialTrans);
		fflush(f);
	}
	return diffs;
}

// Legacy wrapper for backward compatibility
static int Diag_CheckSubTile(const char* stage, int row, int col, uchar* dst, bool skipIfAlreadyFound)
{
	return Diag_CheckCellFull(stage, row, col, dst, 1, skipIfAlreadyFound);
}
#else
#define Diag_SnapshotLightSurface() ((void)0)
#define Diag_CompareLightSurface(c, e) ((void)0)
#define Diag_ResetSubTileCheck() ((void)0)
#define Diag_CheckSubTile(stage, row, col, dst, skip) ((void)0)
#define Diag_CheckCellFull(stage, row, col, dst, layers, skip) ((void)0)
#endif

// (rt) semi-transparent tiles and monsters are deliberately not drawn in town
//----- (00467AE3) --------------------------------------------------------

// (th4) An OPEN cell (walkable: not TileBlockWalking, the table monster/player movement uses).
// Its layer 0 is plain floor; walls, fences and other solid pieces carry the bottom of the wall
// / fence in their layer 0. Used by DrawMapCell (see REPAINT DECALS there).
static inline bool IsOpenFloorCell( int piece )
{
	return piece > 0 && !TileBlockWalking[ piece ];
}

static void StandMask_BeginFrame();	// (th4) STANDING-SPRITE MASK, defined above DrawCellDecals
void __fastcall DrawIsoLand( int playerRow, int playerCol, bool dun, bool zoom ) // SVGADrawView / T_SVGADrawView
{
	g_ReflectIconDrawn = g_CurPlayerDrawn = false;	// (th4) re-latched by DrawMissile/DrawPlayer below
	TownWater_BeginFrame();	// (th4) town river: river cells and heroes are noted again during the loop (TownWater.cpp)
	DustMotes_BeginFrame();	// (th4) church/catacomb dust: mark the object-light cells for this frame (DustMotes.cpp)
	Flames_BeginFrame();	// (th4) torch / brazier / candle flames: forget last frame's notes (Flames.cpp)
	StandMask_BeginFrame();	// (th4) corpses / blood are kept under heroes and monsters (STANDING-SPRITE MASK)
	BioGlow_BeginFrame();	// (th4) bioluminescent night: the glow map is filled again below (dx_utility.cpp BIOGLOW)
	Breath_BeginFrame();	// (th4) cold breath: heroes, towners and monsters are noted again below (Breath.cpp)
	NpcMark_BeginFrame();	// (th4) NPC marks ("!" over an NPC with something to say): noted again below (NpcMark.cpp)
	// One-time init: populate row pointers for LightTable (address is constant for
	// process lifetime; content may change at level load but addresses never do).
	if (!s_LightTablePtrs[0]) {InitLightTablePtrs();}
	if( PixelLight && !g_A8L8Lighting ) PPLRemap_Refresh();	// (th4) PPL REMAP: this frame's LightTable rows
	g_WallBaseTopRow = -1;	// (th4) WALL BASE LIGHT
	// (rt) row, col - the left cell of the diagonal row being drawn (right-up across the map arrays)
	int zoomFactor = zoom ? 2 : 1;
	int row = playerRow - ScreenWidth / 64 / zoomFactor;  // (rt) shift to the top-left corner (the map on screen is rotated clockwise)
	int col = playerCol - 1;
	row += HeightShift / zoomFactor; col += HeightShift / zoomFactor;
	int offsetX = Scroll.xOfs + Screen_LeftBorder;
	int offsetY = Scroll.yOfs + Screen_TopBorder + 15;
	int HorCellDrawCount = ScreenWidth / 64 / zoomFactor;
	int BottomHeight = dun ? 8 : 5; // 3 : 0 for zoom but no using in th2. Dead variable???
	switch( Scroll.dir ){
	case 1:										offsetY -= 32; --row;	--col;	++BottomHeight;	break; // (rt) up
	case 2:	++HorCellDrawCount;					offsetY -= 32; --row;	--col;	++BottomHeight;	break; // (rt) right up
	case 3: ++HorCellDrawCount;																	break; // (rt) right
	case 4:	++HorCellDrawCount;													++BottomHeight;	break; // (rt) right down
	case 5:																		++BottomHeight;	break; // (rt) down
	case 6:	++HorCellDrawCount; offsetX -= 64;				   --row;	++col;	++BottomHeight;	break; // (rt) left down
	case 7: ++HorCellDrawCount; offsetX -= 64;				   --row;	++col;					break; // (rt) left
	case 8:	++HorCellDrawCount; offsetX -= 64;	offsetY -= 32; row -= 2;		++BottomHeight;	break; // (rt) left up
	}
	++HorCellDrawCount; // (rt) non-original drawing hack
	bool interpolation = false;
	PlayerShiftX = PlayerShiftY = 0;
	if( InterpolationFrame && !GamePaused && !IsPausedInMenu() ){ MotionInterpolationBegin( offsetX, offsetY ); interpolation = true; }
	auto drawScene = dun ? DrawSceneDungeonTop : DrawSceneTownTop;
	int screenRowEnd = ScreenHeight / 32 / zoomFactor + (dun ? 5 : 8); // (rt) drawing hack; originally < (dum ? 4 : 7) before the player sprite
//	int screenRowEnd = ScreenHeight / 32 / zoomFactor + BottomHeight;

/*
	// Interpolation shifts offsetX right by ≤8 px, potentially leaving the
	// leftmost screen edge uncovered.  Start one cell earlier (northwest in
	// screen space = row-1, col+1 on map) and draw extra cells so the
	// shifted tiles still fully cover the screen.
	// Might need to consider player movement speed, edge case only relevant when player use speed hack.
	if (interpolation) {
		if (Scroll.dir == SDIR_1_U) {
			offsetY -= 32;
			--row;
			--col;
			++HorCellDrawCount;
			++screenRowEnd;
		}
//		else if (Scroll.dir == SDIR_7_L) {// If condition is NOT commented out, left screen edge flickers when doing circular motion
			offsetX -= 64;
			--row;
			++col;
			++HorCellDrawCount;
			++screenRowEnd;
//		}
	}
*/

	// Interpolation sub-pixel shifts (max ±16 px X, ±8 px Y from
	// MotionInterpolationBegin) can expose screen edges.  Extend the drawing
	// area by one cell in all directions — 64 px X / 32 px Y — to absorb
	// any shift regardless of movement direction.  Skip when standing still
	// (Scroll.dir == SDIR_0_NONE) since no viewport shift occurs.
	// Note that motion interpolation has offsetX shift for all 6 directions except UP(1) and DOWN(5)
	if (interpolation && Scroll.dir != SDIR_0_NONE) {
		offsetY -= 32;
		offsetX -= 64;
		row -= 2;
		HorCellDrawCount += 2;
		screenRowEnd += 2;
	}

	s_PPLDispatched = false; // reset every frame; PPLPoolDispatch sets it true for dungeon frames only
	// Screen-space PPL pre-fill: compute bilinear LightMap values for every
	// visible pixel and write them to LightSurface once per frame.
	// This replaces per-cell PPL computation with a globally continuous field.
	// In A8L8 mode, linecpyPerPixel reads from LightSurface (not PerPixelLight),
	// so the MT tile dispatch is redundant — pre-fill handles both ST and MT.
	// Non-A8L8 mode still uses the legacy MT dispatch (PerPixelLight-based).
	g_InDungeonRender = true;
	for (int pi = 0; pi < PlayersMax_4; ++pi) g_ReDraw[pi].valid = false;  // reset each frame
	g_PostRenderSpriteCount = 0;  // Option C: reset monster/missile post-render list
	SnowDepth_FrameStart();       // (th4) SNOW DEPTH's Enriched Light records live as long as these lists (Snowfall.cpp)
	ResetOverheadHpBars();        // (th4) overhead bars are queued during the loop, flushed after it
	if (PixelLight && g_A8L8Lighting) {
		if (A8L8PixelField) {
			// A8L8 per-pixel light field: ambient + per-pixel light stamps.
			// Rev 24: ST and MT share one byte-identical row implementation.
			// Rev 26 (M1): once the pool is marked broken, stay on ST for the
			// rest of the session instead of stalling 1 s per frame.
			if (MultiThreadPixelLight && dun && !s_PPLPoolBroken) {
				FillScreenSpaceLightingV2MT(row, col, offsetX, offsetY, zoomFactor, dun);
			} else {
				FillScreenSpaceLightingV2(row, col, offsetX, offsetY, zoomFactor, dun);
			}
		}
		else if (MultiThreadPixelLight && dun) {
			if (!s_PPLPoolBroken)
				FillScreenSpaceLightingDispatch(row, col, offsetX, offsetY, zoomFactor);
			else
				FillScreenSpaceLighting(row, col, offsetX, offsetY, zoomFactor);
		}
		else {
			FillScreenSpaceLighting(row, col, offsetX, offsetY, zoomFactor);
		}
		TownNight_ApplyToLight( zoomFactor );	// (th4) town night as ambient darkness (world only)
		// Diagnostic: snapshot LightSurface before any tile rendering
		Diag_SnapshotLightSurface();
		Diag_ResetSubTileCheck();
	} else if (PixelLight && MultiThreadPixelLight && dun && !s_PPLPoolBroken) {
		PPLPoolInit(); // no-op after first call; workers persist for process lifetime
		PPLPoolDispatch(row, col, screenRowEnd, HorCellDrawCount);
	}
	for( int screenRow = 0; screenRow < screenRowEnd; ++screenRow, offsetY += 32 ){
		drawScene(row, col++, offsetX, offsetY, HorCellDrawCount, screenRow, 0);				// (rt) even cells
//		(rt) drawScene(row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, screenRow, dun);	// odd cells
		drawScene(row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, screenRow, 1);	// (rt) odd cells (halfFlag=1)
	}
	if( interpolation ){ MotionInterpolationEnd(); }

	DrawDeferredWyrms();	// (th4) half-submerged wyrms, on top of the pool floor (see IsSubmergedWyrm)

	// Diagnostic: compare after all tiles rendered (should be zero diffs)
	Diag_CompareLightSurface("after_tiles", 0);

	// Option C: post-render re-draw monsters and missiles to write their
	// brightness levels to LightSurface.  Drawn before player so the
	// player (drawn last) always wins at overlapping pixels.
	// MUST run BEFORE zoom — zoom 2x-scales LightSurface, and post-render
	// coordinates are in pre-zoom (1x) space.
	if (g_A8L8Lighting && PixelLight && g_PostRenderSpriteCount > 0) {
		g_LightSurfaceOnlyPass = true;
		for (int i = 0; i < g_PostRenderSpriteCount; ++i) {
			auto& cmd = g_PostRenderSprites[i];
			int savedDark = DarkLevel;
			DarkLevel = cmd.darkLevel;// restore DarkLevel for correct lightFill. cmd.darkLevel is the DarkLevel value at the map cell where the sprite was first drawn. The post-render needs it because CL2_Draw computes lightFill = DarkLevel * 17.  The call chain CL2_Draw -> Rle2Draw never reads the global DarkLevel for anything.  No bug here.
			g_CurrentPostRenderType = cmd.spriteType;  // (th4) thread sprite type to Rle2Draw
			g_TintExcludeCurrentSprite = cmd.tintExclude;  // (th4) Stage 3: owner-pixel tint override to white
			SnowDepth_ReplayBegin(cmd.x, cmd.y, cmd.cl2Buf, cmd.frameNum);	// (th4) SNOW DEPTH: buried legs keep the snow's light (Snowfall.cpp ENRICHED LIGHT)
			CL2_Draw(cmd.x, cmd.y, cmd.cl2Buf, cmd.frameNum, cmd.pitch, cmd.shiftY, cmd.tileInCellIndex, cmd.darken);
			SnowDepth_ReplayEnd();
			DarkLevel = savedDark;
		}
		g_TintExcludeCurrentSprite = false;
		g_LightSurfaceOnlyPass = false;
	}

	// Post-render LightSurface-only re-draw: re-clear LightSurface to 0
	// at player sprite pixels so all players appear full-bright through walls.
	// Iterates all 4 player slots (multiplayer co-op support).
	if (g_A8L8Lighting && PixelLight) {
		g_LightSurfaceOnlyPass = true;
		for (int pi = 0; pi < PlayersMax_4; ++pi) {
			if (!g_ReDraw[pi].valid) continue;
			const auto& cmd = g_ReDraw[pi];
			g_CurrentPostRenderType = cmd.spriteType;  // (th4) Player, or PlayerShaded for the THIEF
			SnowDepth_ReplayBegin(cmd.rowOffset, cmd.colOffset, cmd.animPtr, cmd.frame);	// (th4) SNOW DEPTH (Snowfall.cpp ENRICHED LIGHT)
			DrawPlayer(
				cmd.playerIndex,
				cmd.row, cmd.col,
				cmd.rowOffset, cmd.colOffset,
				cmd.animPtr,
				cmd.frame,
				cmd.pitch,
				cmd.shiftY,
				cmd.screenCellRow);
			SnowDepth_ReplayEnd();
		}
		g_LightSurfaceOnlyPass = false;
	}

	// Diagnostic: compare after all post-render (expected diffs at sprite pixels)
	Diag_CompareLightSurface("after_postrender", -1);

	// (th4) Overhead HP bars, last of all: they are flat screen-space rectangles with no
	// depth of their own, so they belong on top of every tile and every sprite. Must run
	// BEFORE the zoom block -- the queued coordinates are pre-zoom 1x, exactly like the
	// g_PostRenderSprites coordinates above.
	FlushAltLootOutlines();   // (th4) Alt lootable rings, on top of every tile
	FlushOverheadHpBars();
	NpcMark_Flush();          // (th4) the old 8-bit NPC mark where no shader runs / in Classic, on top of every tile (NpcMark.cpp)

	if( zoom ){ // (rt) stretch the image 2x
		uchar* SurfPtr = &WorkingSurface[ YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder];
		uchar* LightPtr = (g_A8L8Lighting && LightSurface)
			? &LightSurface[YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder]
			: nullptr;
		// (th4) Stage 2: 2x upscale the overlay pair too (1-byte and 4-byte px).
		uchar* AlphaPtr = (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface)
			? &AlphaSurface[YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder]
			: nullptr;
		// SpriteLayerSurface is uint* indexed by PIXEL offset (same space as
		// WorkingSurface) — the element index equals the pixel index directly.
		uint* SpritePtr = (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && SpriteLayerSurface)
			? SpriteLayerSurface + YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder
			: nullptr;
		// (th4) Stage 3: 2x upscale the tint surface too (4-byte pixels, same
		// uint* pixel-offset convention as SpriteLayerSurface). Idle gate:
		// skipped when no colored light is active (nothing reads the field).
		uint* TintPtr = (g_ARGBLighting && g_LightTintActive && LightTintSurface)
			? LightTintSurface + YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder
			: nullptr;
//		for (int h = ScreenHeight / 2 - 1; h >= 0; h--) {// Proposed fix for off by 1 error, probably affecting south side
//			for (int w = ScreenWidth / 2 - 1; w >= 0; w--) {// Proposed fix for off by 1 error, probably affecting east side
		for( int h = ScreenHeight / 2; h >= 0; h-- ){
			for( int w = ScreenWidth / 2; w >= 0; w-- ){
				char val = SurfPtr[w + h * WorkingWidth];
				SurfPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = val;
				SurfPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = val;
				SurfPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = val;
				SurfPtr[(2 * w) + (2 * h) * WorkingWidth] = val;
				if (LightPtr) {
					uchar lv = LightPtr[w + h * WorkingWidth];
					LightPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = lv;
					LightPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = lv;
					LightPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = lv;
					LightPtr[(2 * w) + (2 * h) * WorkingWidth] = lv;
				}
				if (AlphaPtr) {
					uchar av = AlphaPtr[w + h * WorkingWidth];
					AlphaPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = av;
					AlphaPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = av;
					AlphaPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = av;
					AlphaPtr[(2 * w) + (2 * h) * WorkingWidth] = av;
				}
				if (SpritePtr) {
					// NOTE: SpritePtr is uint* anchored at the visible origin.
					// SpritePtr[i] addresses ELEMENT i (byte 4*i), and pixel
					// (x,y) has element index x + y*WorkingWidth — so the pixel
					// index IS x + y*WorkingWidth (4-byte pixels, stride matches).
					uint sv = SpritePtr[w + h * WorkingWidth];
					SpritePtr[(2 * w) + (2 * h + 1) * WorkingWidth] = sv;
					SpritePtr[(2 * w + 1) + (2 * h) * WorkingWidth] = sv;
					SpritePtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = sv;
					SpritePtr[(2 * w) + (2 * h) * WorkingWidth] = sv;
				}
				if (TintPtr) {
					// (th4) Stage 3: same 2x pattern, 4-byte tint words.
					uint tv = TintPtr[w + h * WorkingWidth];
					TintPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = tv;
					TintPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = tv;
					TintPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = tv;
					TintPtr[(2 * w) + (2 * h) * WorkingWidth] = tv;
				}
			}
		}
	}

	// Diagnostic: compare after zoom pass (may have diffs due to 2x scaling)
	Diag_CompareLightSurface("after_zoom", -1);

	g_InDungeonRender = false;
}

//----- (004696D3) --------------------------------------------------------
void __fastcall DrawSceneDungeonTop( int row, int col, int offsetX, int offsetY, int horCellDrawCount, int screenCellRow, int halfFlag  ) // DrawHTLX
{
	if( horCellDrawCount <= 0 ) return;
	// (rt) row, col - the left cell of the diagonal row being drawn (right-up across the map arrays)
	TileColumn* tile = &TileMap[IsometricMapOffset(row, col)];
	int tileInCellIndex = std::min(2 * screenCellRow + 2, 8);
	bool oddFirstCell = (halfFlag != 0);
	do{
		if( In112(row, col) ){
			#if CHEATS
			bool black = row == PlayerNextRow && col == PlayerNextCol;
			bool brown = row == PlayerNextNextRow && col == PlayerNextNextCol;
			#endif
			DarkLevel = LightMap[ row ][ col ];
			DustMotes_NoteCell( row, col, offsetX, offsetY );	// (th4) screen spot of a torch/candle cell (DustMotes.cpp)
			if( PixelLight ) CalculatePerPixelLight(row, col);
			WallIndex = FineMap[ row ][ col ];
			if( WallIndex ){
				TransparentMode = CellTransparent[WallIndex] & TransList[TransMap[ row ][ col ]];
				PartialTrans = TSP_1_LEFT;
				int yOfs = YOffsetHashTable[offsetY];
				uchar* leftTileDst = &WorkingSurface[offsetX + yOfs];
				if( Dungeon == DUN_44_RAVENHOLM ) TownWater_CellBegin( WallIndex, offsetX, offsetY, row, col );	// (th4) ICE TREES: Ravenholm's leaves; ROCK SNOW: its rocks and thatch roofs (TownWater.cpp); no-op unless Ice Age
				// (rt) draw the left triangles and walls (skip for odd-row first cell)
				if( !oddFirstCell ){
					CurTileID = tile->Layer[0].Left;
					PerPixelXOffset = 0;
					if( CurTileID )DrawDunCelTrans(leftTileDst);
				}
				PartialTrans = TSP_2_RIGHT;
				// (rt) draw the right triangles and walls
				CurTileID = tile->Layer[0].Right;
				PerPixelXOffset = 32;
				if( CurTileID ) DrawDunCelTrans(leftTileDst + 32);
				if( Dungeon == DUN_44_RAVENHOLM ){	// (th4) as TownMap.cpp DrawSceneTownTop does for the town (TownWater.cpp)
					TownWater_FloorDrawn( row, col, offsetX, offsetY );	// GRASS: Ravenholm's floor is drawn
					TownWater_NoteCell( WallIndex, offsetX, offsetY, tile->Layer[0].Left, tile->Layer[0].Right );	// RAVENHOLM'S RIVER: its river cells, for the hero reflections
				}
#if OPTION_C_DIAG_LIGHTSURFACEBASE
				// Diagnostic: check FULL cell footprint (all layers) after floor tiles.
				{
					int numLayers = MaxTilesInCell >> 1; // floor + wall layers
					uchar* floorDst = &WorkingSurface[offsetX + yOfs];
					if (!oddFirstCell) Diag_CheckCellFull("L_floor", row, col, floorDst, numLayers, true);
					Diag_CheckCellFull("R_floor", row, col, floorDst + 32, numLayers, true);
				}
#endif
				#if CHEATS
				#if 0
				if( DevelopMode && black ) DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
				if( DevelopMode && brown ) DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY], 194);
				#endif
				#endif
				PartialTrans = TSP_0_NONE;
				int tileIndex = 1;
				g_IsDrawingOpaqueWall = true;  // (th4) mark WallOcclusionSurface during wall rendering
				if( (MaxTilesInCell >> 1) - 1 > 1 ){
					uchar* rightTileDst = leftTileDst + 32;
					ushort* nextTile = &tile->Layer[1].Left;
					do{
						rightTileDst -= WorkingWidth * 32;
						leftTileDst -= WorkingWidth * 32;
						g_WallBaseTopRow = offsetY - 31;	// (th4) WALL BASE LIGHT (linecpyPerPixel)
//						if (screenCellRow >= tileIndex) {
							// Draw ALL wall layers unconditionally; PutTile clips above screen.
							if (!oddFirstCell) {
								int leftTile = *(ushort*)nextTile;
								CurTileID = leftTile;
								if (leftTile) {
									PerPixelXOffset = 0;
									DrawDunCelTrans(leftTileDst); // (rt) left wall
								}
							}
							int rightTile = nextTile[1];
							CurTileID = rightTile;
							if (rightTile) {
								PerPixelXOffset = 32;
								DrawDunCelTrans(rightTileDst); // (rt) right wall
							}
//						}
						++tileIndex;
						nextTile += 2;
					}while( tileIndex < (MaxTilesInCell >> 1) );// (rt) drawing hack; should be one more - 1 );
				g_IsDrawingOpaqueWall = false;  // (th4) wall layer rendering done
				g_WallBaseTopRow = -1;
				}
				if( Dungeon == DUN_44_RAVENHOLM ) TownWater_CellEnd();	// (th4) ICE TREES (pairs with the CellBegin above)
#if OPTION_C_DIAG_LIGHTSURFACEBASE
				// Diagnostic: check FULL cell footprint (all layers) after wall layers.
				{
					int numLayers = MaxTilesInCell >> 1;
					uchar* floorDst = &WorkingSurface[offsetX + yOfs];
					if (!oddFirstCell) Diag_CheckCellFull("L_walls", row, col, floorDst, numLayers, true);
					Diag_CheckCellFull("R_walls", row, col, floorDst + 32, numLayers, true);
				}
#endif
				DrawDynamicObjects(&WorkingSurface[offsetX + yOfs], row, col, screenCellRow, tileInCellIndex, offsetX, offsetY, 1);
#if OPTION_C_DIAG_LIGHTSURFACEBASE
				// Diagnostic: check FULL cell footprint (all layers) after dynamic objects.
				{
					int numLayers = MaxTilesInCell >> 1;
					uchar* floorDst = &WorkingSurface[offsetX + yOfs];
					if (!oddFirstCell) Diag_CheckCellFull("L_objs", row, col, floorDst, numLayers, true);
					Diag_CheckCellFull("R_objs", row, col, floorDst + 32, numLayers, true);
				}
#endif
			}else{
				DrawTransparency(&WorkingSurface[offsetX + YOffsetHashTable[offsetY]]);
			}
		}else{
			// (rt) this function evidently fills in the places where the map is missing
			DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
		}
		#if 0
		if( DevelopMode ) DrawTileGrid(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
		#endif
		oddFirstCell = false;
		++tile;
		++row;
		--col;
		offsetX += 64;
	}while( horCellDrawCount-- != 1 );
}

//----- (th3) -------------------------------------------------------------
void __fastcall MonsterSpritePosAdjust(int baseMonsterIndex, int anim, int dir, short& x, short& y)
{
	switch (baseMonsterIndex) {
	case BM_232_ABYSSAL_SUCCUBUS: y += 26; break;
	case BM_NPC_420_GRISWOLD: y += 24; break;
	case BM_233_BLOOD_LORD: if (anim == ANIM_4_DIE) { y += 10; } break;
	case BM_234_STEEL_LORD:
		switch (anim) {
		case ANIM_4_DIE:    y += 13; break; // attack animation needs adjusting too: minus 8 pixels
		case ANIM_3_HIT:    y += 10; break;
		case ANIM_0_STAY:   y += 10; break;
		case ANIM_5_SECOND: y += 7; break;
		case ANIM_1_WALK:   y += 9; break;
		} break;
	case BM_236_CORPSE_EATER:
		switch (anim) {
		case ANIM_4_DIE:    y += 32; break;
		case ANIM_3_HIT:    y += 20; break;
		case ANIM_0_STAY:   y += 20; break;
		case ANIM_5_SECOND: y += 20; break;
		case ANIM_1_WALK:   y += 20; break;
		case ANIM_2_ATTACK: y += 20; break;
		} break;
	case BM_222_NIGHT_DEMON:
	case BM_237_CONTAMINATOR:
	case BM_238_TAINTED:
	case BM_242_REANIMATED_HORROR:
	case BM_239_AFFLICTED:	y += 30; break;
	case BM_240_DEMON_PRIEST:
		switch (anim) {
		case ANIM_4_DIE:    y += 33; break;
		case ANIM_3_HIT:    y += 30; break;
		case ANIM_0_STAY:   y += 30; break;
		case ANIM_5_SECOND: y += 30; break;
		case ANIM_1_WALK:   y += 30; break;
		case ANIM_2_ATTACK: y += 30; break;
		} break;
	case BM_243_IZUAL:		y += 55; break;
	case BM_245_BLUNDERER:		y += 74; break;
	case BM_246_GREATER_MUMMY:
		switch (anim) {
		case ANIM_4_DIE:    y += 36; break;
		case ANIM_3_HIT:    y += 20; break;
		case ANIM_0_STAY:   y += 20; break;
		case ANIM_5_SECOND: y += 20; break;
		case ANIM_1_WALK:   y += 20; break;
		case ANIM_2_ATTACK: y += 20; break;
		} break;
	case BM_244_HORADRIM_ANCIENT:	y += 35; break;
	case BM_249_SCARAB:
	case BM_250_BONE_SCARAB:
	case BM_247_SIEGEBREAKER_BEAST:
	case BM_231_BEASTMASTER_SUMMON_3: y += 60; break;
	case BM_248_SALAMANDER: y += 61; break;
	case BM_251_WENDIGO:	y += 40; break;
	case BM_252_HEPHASTO:		y += 40; break;
	case BM_253_EXPLODER:	y += 40; break;
	case BM_254_TWILIGHT_CLAN:	y += 30; break;
	case BM_259_WATER_WYRM: 
	case BM_263_RED_WYRM	:y += 60; break;
	case BM_290_FUNGAL_ROOT_LITE:
	case BM_289_FUNGAL_ROOT:	y += 9; break;
	case BM_291_MYCONID:		y += 20; break;
	case BM_293_PIT_FIEND:		y += 36; break;
	case BM_296_ICE_TROLL:		y += 33; break;
	case BM_297_OROG:			y += 50; break;
	case BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS:	y += 18; break;
	case BM_292_BUGBEAR:		y += 62;
		switch (anim) {
		case ANIM_1_WALK: y += 9; break;

		}
		break;
	case BM_265_CROC or BM_266_CROC_BERS:		y += 23+30; break;
	case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2:		y += 80; break;
	case BM_269_BOG_CRAWLER or BM_270_MUD_CRAWLER:	y += 20; break;
	case BM_271_THORNED_HULK:	y += 32; break;
	case BM_272_SPIDER_QUEEN:	y += 25; break;
	case BM_273_FLAME_FIEND:	y += 26; break;
	case BM_274_FLESH_DEVOURER:	y += 30; break;
	case BM_275_BEHEMOTH:	y += 18; break;
	case BM_276_SWORD_SPIDER:	y += 12; break;
	case BM_277_SPIDERLING:	y += 11; break;
	case BM_278_LEAPER:		y += 34; break;
	case BM_279_FLOATER:
	case BM_280_FLOATER_X2:		y += 15; break;
	case BM_282_GREMLIN_BOW:
	case BM_281_FALLEN_BOW:		y += 11; break;
	case BM_286_BRINE_CORPSE:	/*x += 11,*/ y += 18; break;
	case BM_287_ARMORED_SKELETON:	y += 29; break;
	case BM_288_EARTH_GOLEM:	y += 18; break;
	case BM_298_TENTACLIST_BOSS:	y += 50; break;
	case BM_299_TENTACLIST2:
	case BM_300_TENTACLIST_ABYSS:	y += 42; break;
	case BM_283_FROST_WARDEN:	y += 20;
		switch (anim) {
		case ANIM_1_WALK: y += 6; break;
		}
		break;
	case BM_260_ARMAGEDDON_WALKER:
		switch (anim) {
		case ANIM_4_DIE:    y += 36;
			switch (dir) {
			case DIR_0_D: x += 19; break;
			case DIR_1_DL: x += 54; break;
			case DIR_2_L: x += 44; y += 10; break;
			case DIR_3_UL: x += 34; y += 18; break;
			case DIR_4_U: x += 14; y += 30; break;
			case DIR_5_UR: x += -45; y += 21; break;
			case DIR_6_R: x += -45; y += 12; break;
			case DIR_7_DR: x += -41; y += 11; break;
			}
			break;
		case ANIM_3_HIT:    y += 53;
			switch (dir) {
			case DIR_0_D: x += 31; break;
			case DIR_1_DL: x += 40; break;
			case DIR_2_L: x += 49; break;
			case DIR_3_UL: x += 59; y += 8; break;
			case DIR_4_U: x += 45; y += 17; break;
			case DIR_5_UR: x += -47; y += 10; break;
			case DIR_6_R: x += -48; y += 3; break;
			case DIR_7_DR: x += -40; y += 3; break;
			}
			break;
		case ANIM_0_STAY:   y += 64;
			switch (dir) {
			case DIR_0_D: x += 40; break;
			case DIR_1_DL: x += 53; break;
			case DIR_2_L: x += 45; break;
			case DIR_3_UL: x += 54; break;
			case DIR_4_U: x += 43; break;
			case DIR_5_UR: x += -53; break;
			case DIR_6_R: x += -45; break;
			case DIR_7_DR: x += -59; break;
			}
			break;
		case ANIM_1_WALK:   y += 57;
			switch (dir) {
			case DIR_0_D: x += 34; break;
			case DIR_1_DL: x += 54; break;
			case DIR_2_L: x += 44; break;
			case DIR_3_UL: x += 54; break;
			case DIR_4_U: x += 34; break;
			case DIR_5_UR: x += -51; break;
			case DIR_6_R: x += -45; break;
			case DIR_7_DR: x += -55; break;
			}
			break;
		case ANIM_5_SECOND:
		case ANIM_2_ATTACK: y += 38; break;
		}
		break;
	case BM_261_SKELETAL_KNIGHT:
		switch (anim) {
		case ANIM_0_STAY:   y += 31;
			switch (dir) {
			case DIR_0_D:	x += -3;	y += 10;	break;
			case DIR_1_DL:	x += -4;				break;
			case DIR_2_L:	x += -15;	y -= 5;	break;
			case DIR_3_UL:	x += -2;	y += -11;	break;
			case DIR_4_U:	x += 2;	y += -13;	break;
			case DIR_5_UR:	x += 1;	y += -9;	break;
			case DIR_6_R:	x += 9;	y += 1;	break;
			case DIR_7_DR:	x += 1;	y += 3;	break;
			}
			break;
		case ANIM_1_WALK:
			switch (dir) {
			case DIR_0_D: x += -5; y += 30; break;
			case DIR_1_DL: x += -3; y += 35; break;
			case DIR_2_L: x += -12; y += 25; break;
			case DIR_3_UL: x += -0; y += 30; break;
			case DIR_4_U: x += 4; y += 30; break;
			case DIR_5_UR: x += 0; y += 35; break;
			case DIR_6_R: x += 0; y += 32; break;
			case DIR_7_DR: x += -2; y += 35; break;
			}
			break;
		case ANIM_3_HIT:
			switch (dir) {
			case DIR_0_D: x += -2; y += 28; break;
			case DIR_1_DL: x += 0; y += 26; break;
			case DIR_2_L: x += -1; y += 19; break;
			case DIR_3_UL: x += 4; y += 22; break;
			case DIR_4_U: x += 0; y += 16; break;
			case DIR_5_UR: x += -4; y += 24; break;
			case DIR_6_R: x += 1; y += 23; break;
			case DIR_7_DR: x += -1; y += 26; break;
			}
			break;
		case ANIM_4_DIE:
			switch (dir) {
			case DIR_0_D: x += 0; y += 40; break;
			case DIR_1_DL: x += -5; y += 40; break;
			case DIR_2_L: x += -17; y += 22; break;
			case DIR_3_UL: x += -6; y += 14; break;
			case DIR_4_U: x += 1; y += 9; break;
			case DIR_5_UR: x += 4; y += 20; break;
			case DIR_6_R: x += 8; y += 27; break;
			case DIR_7_DR: x += 5; y += 40; break;
			}
			break;
		case ANIM_2_ATTACK: y += 22;
			switch (dir) {
			case DIR_0_D:	x +=  10;				break;
			case DIR_1_DL:	x +=  -3;	y -= 3;		break;
			case DIR_2_L:	x += -15;	y += 4;		break;
			case DIR_3_UL:	x +=   3;	y += 10;	break;
			case DIR_4_U:	x += -13;	y += 4;		break;
			case DIR_5_UR:	x +=  -4;	y += 11;	break;
			case DIR_6_R:	x +=   9;	y += 10;	break;
			case DIR_7_DR:	x +=   0;				break;
			}
			break;
		case ANIM_5_SECOND:
			switch (dir) {
			case DIR_0_D:	x -= 2;		y += 6;		break;
			case DIR_1_DL:	x += 5;		y += 16;	break;
			case DIR_2_L:	x += -17;	y += 6;		break;
			case DIR_3_UL:	x += -12;	y += 17;	break;
			case DIR_4_U:	x += -12;	y += 14;	break;
			case DIR_5_UR:	x += 12;	y += 19;	break;
			case DIR_6_R:	x += 11;	y += 12;	break;
			case DIR_7_DR:	x += -7;	y += 18;	break;
			}
			break;
		}
		break;
	}
}

#if CHEATS
volatile int ShiftX = 0;
volatile int ShiftY = 0;
#endif
//----- (th3) -------------------------------------------------------------
void __fastcall MonsterPosAdjust(AnimDir* animDir, int& monsterX, int& monsterY, int& tilesInCell, int monsterIndex /*= 0*/)
{
	monsterX += animDir->xOfs;
	monsterY += animDir->yOfs;

	#if CHEATS
	Monster& monster = Monsters[monsterIndex];
	#if 0
	if( monsterIndex == 142 ){
		Dir = monster->Orientation; CurDir = monster->curOrientation;
		if( monster->curAnimation->data != monster->SpritePtr->animation[MonsterAnimIndex(monster->CurAction)].oriented[monster->Orientation].data){
			__debugbreak();
		}
	}
	#endif
	int x = 0, y = 0;
	switch( monster.SpritePtr->baseMonsterIndex ){
	case BM_260_ARMAGEDDON_WALKER:
		switch (MonsterAnimIndex(monster.CurAction)) {
		case ANIM_4_DIE:    break;
		case ANIM_3_HIT:    break;
		case ANIM_0_STAY:   break;
		case ANIM_5_SECOND: break;
		case ANIM_1_WALK:   y += 0;
			switch( monster.Orientation ){
			case DIR_0_D : x += 0; break;
			case DIR_1_DL: x += 0; break;
			case DIR_2_L : x += 0; break;
			case DIR_3_UL: x += 0; break;
			case DIR_4_U : x += 0; break;
			case DIR_5_UR: x += 0; break;
			case DIR_6_R : x += 0; break;
			case DIR_7_DR: x += 0; break;
			}
			x += ShiftX;
			y += ShiftY;
			break;
		case ANIM_2_ATTACK: break;
		}
		break;
	}
	monsterX += x;
	monsterY += y;
	tilesInCell += (y / 16) * 2;
	#endif

	tilesInCell += (animDir->yOfs / 16) * 2;
	LimitToMax(tilesInCell, 8);
}

#if CHEATS
volatile int Dir = 0;
volatile int CurDir = 0;
#endif
//----- (th2) -------------------------------------------------------------
void __fastcall DrawMonsterInTile( int monsterIndex, uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, bool firstDraw, uchar placeFlag )
{
	// (th4) wyrms are queued here and drawn after the scene, see DrawDeferredWyrms
	if( !g_DrawingDeferredWyrms && monsterIndex >= 0 && monsterIndex < Monsters_Max_Count && IsSubmergedWyrm( Monsters[ monsterIndex ] ) ){
		for( int i = 0; i < g_DeferredWyrmCount; ++i ) if( g_DeferredWyrms[ i ].monsterIndex == monsterIndex ) return;	// once per frame
		if( g_DeferredWyrmCount < 32 ){
			g_DeferredWyrms[ g_DeferredWyrmCount++ ] = { monsterIndex, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY, surfaceDest, placeFlag };
			return;
		}
	}
	int submergeSkip = 0;
	if( g_DrawingDeferredWyrms ){
		submergeSkip = MonsterAnimIndex( Monsters[ monsterIndex ].CurAction ) == ANIM_2_ATTACK ? WYRM_SUBMERGED_SKIP_ATTACK : WYRM_SUBMERGED_SKIP;
	}
	if( placeFlag & CF_64_VISIBLE_BY_CURSOR || Players[CurrentPlayerIndex].Infravision || ( AlwaysHighlightSummons && monsterIndex < SummonMonsters_Count ) ){
		if( monsterIndex < Monsters_Max_Count ){
			Monster* monster = &Monsters[monsterIndex];
			if( !( monster->flag & MF_1_INVISIBLE ) ){
				MonsterSprite* monsterSprite = monster->SpritePtr;
				if( monsterSprite ){
					int y = playerOffsetY + monster->DeltaY;
					int x = playerOffsetX + monster->DeltaX - monsterSprite->StartOfsX;
					int tilesInCell = tileInCellIndex;
					MonsterPosAdjust(monster->curAnimation, x, y, tilesInCell, monsterIndex);
					if( SpriteInScreen(x, y) ){
						uchar borderColor = 0;
						bool drawOutline = false;
						// (th4) SNOW DEPTH (Snowfall.cpp): Ice Age Ravenholm's monsters stand knee-deep, outline included;
						// fliers hover above the snow; not on the frozen river's water or on bare ground (TownWater_FeetBare)
						SnowDepth_Begin( x, y, monsterSprite->Pitch, !Snowfall_Active() || monster->canFly || TownWater_FeetBare( row, col, monster->DeltaX, monster->DeltaY ), SnowDepthSlope( monster->Orientation ), false );
						if( monsterIndex == CurMon || ( AlwaysHighlightSummons && monsterIndex < SummonMonsters_Count ) ) {
							// (th4) Dark Campaign hub: kin hover BLUE. 184 is mid-way along the blue
							// row of the palette's LOWER half (indices 176-191), which is identical in
							// every dungeon palette -- unlike 165, which lives in the upper half and so
							// takes whatever colour the current level's .pal happens to assign it.
							// The mission 10 escort fights for the player, so it hovers blue too --
							// same as a summon, rather than the hostile red.
							bool dcEscort = GameMode == GM_DARK_CAMPAIGN && (monster->flag & MF_6_FRIENDLY)
										 && monster->newBossId && DarkCampaignIsEscortBoss( monster->newBossId - 1 );
							// (th4) DC mission 13 only: Lazarus fights on the player's side in his own
							// lair, so he hovers the same colour as the girls. Gated on GM_DARK_CAMPAIGN
							// AND DUN_121 so the hostile Lazarus of the normal campaign is untouched.
							bool dcLazarus = GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
										  && (monster->flag & MF_6_FRIENDLY)
										  && monster->newBossId && monster->newBossId - 1 == UM_4_ArchBishop_Lazarus;
							// (th4) DC mission 15: the armorer hovers the same colour, for the same
							// reason -- he is on the player's side here. Gated on DUN_43 so the
							// normal campaign's Hephasto keeps his hostile outline.
							bool dcHephasto = GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
										   && (monster->flag & MF_6_FRIENDLY)
										   && monster->newBossId && monster->newBossId - 1 == UM_587_Hephasto_the_Armorer;
							borderColor = Dungeon == DUN_117_DARKLORD_HIDEOUT ? 184
											  : ( monsterIndex < SummonMonsters_Count || dcEscort || dcLazarus || dcHephasto ) ? 165 : 233;
							drawOutline = true;
							CL2_DrawOutline(borderColor, x, y, monster->curAnimation->data, monster->CurFrame, monsterSprite->Pitch, submergeSkip, tilesInCell);
						}
						DrawMonster(row, col, x, y, monsterIndex, submergeSkip, tilesInCell);
						// (th4) A8L8 ring-light pass after the body: lifts only the visible
						// hover outline to full brightness (the monster body is never touched
						// -- see Rle2DrawLightOutline). Mirrors the ALT-highlight fix for
						// dungeon objects (commit c5a66475).
						if( drawOutline && g_A8L8Lighting ){
							CL2_DrawLightOutline(borderColor, x, y, monster->curAnimation->data, monster->CurFrame, monsterSprite->Pitch, submergeSkip, tilesInCell);
						}
						SnowDepth_End( monster->curAnimation->data, monster->CurFrame );	// (th4) SNOW DEPTH: key = what DrawMonster queued for the Enriched Light replay
						if( firstDraw && ! (monster->flag & MF_14_UNDER_ARCH) ){
							DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tilesInCell, playerOffsetX - 64, playerOffsetY);
						}
					}
				}
			}
		}
	}
}


// (th4) STANDING-SPRITE MASK -- corpses and blood pools are drawn UNDER heroes and monsters.
// The dungeon is drawn cell by cell, back to front, and a corpse is drawn in its own cell's turn
// like everything else. A corpse lying one or two cells in front of a hero or monster is therefore
// drawn AFTER it, and big corpses (they reach well up the screen) came out on top of legs and
// bodies -- an old engine behaviour. Corpses and blood lie flat on the ground, so a standing
// sprite must always win over them:
//   StandMask_BeginSprite / EndSprite  around every hero / monster / dying-hero draw in
//       DrawDynamicObjects: the pixels the sprite changed are stamped in SM_Mask (one ushort per
//       WorkingSurface pixel, stamped with a per-frame number, so it is never cleared), and the
//       sprite's screen box is remembered;
//   Decal_Begin / Decal_End  around every blood / corpse draw in DrawCellDecals: when the decal's
//       box touches a remembered sprite box, the covered area is saved first and every stamped
//       pixel is put back afterwards (colour index, and the LightSurface value under Enriched
//       Light), so the sprite stays on top. Decals that touch no sprite are drawn untouched.
// Frames with no corpse or blood on the whole map skip all of it (StandMask_BeginFrame). The
// sprite boxes are generous screen rectangles around the cell (monsters and the dying hero) or the
// exact animation width (heroes); pixels the sprite drew in the same colour index as what was
// under it are not stamped -- a decal may cover such a pixel, which is invisible anyway.
// Nesting: a monster draw can repaint a neighbour cell (DrawMapCell) that draws more sprites, so
// the sprite snapshots are a small stack.
namespace {
	struct SMBox { int x0, y0, x1, y1; };
	std::vector<ushort> SM_Mask;
	ushort SM_Stamp = 0;
	bool SM_On = false;
	SMBox SM_Boxes[ 256 ];
	int SM_BoxCount = 0;
	const int SM_DEPTH = 6;
	SMBox SM_Stack[ SM_DEPTH ];
	std::vector<uchar> SM_StackPx[ SM_DEPTH ];
	int SM_Depth = 0;
	SMBox SM_Decal;
	bool SM_DecalOn = false;
	std::vector<uchar> SM_DecalPx, SM_DecalL;

	SMBox SM_Clamp( int x0, int y0, int x1, int y1 )
	{
		return { max( 0, x0 ), max( 0, y0 ), min( WorkingWidth, x1 ), min( Screen_TopBorder + ScreenHeight + 64, y1 ) };
	}
	void SM_Save( const SMBox& b, std::vector<uchar>& to, const uchar* surf )
	{
		const int w = b.x1 - b.x0;
		to.resize( (size_t)w * ( b.y1 - b.y0 ) );
		for( int y = b.y0; y < b.y1; ++y ) memcpy( &to[ (size_t)( y - b.y0 ) * w ], surf + YOffsetHashTable[ y ] + b.x0, w );
	}
}

// (th4) Draw.cpp DrawIsoLand start.
static void StandMask_BeginFrame()
{
	SM_On = false; SM_BoxCount = 0; SM_Depth = 0; SM_DecalOn = false;
	if( !WorkingSurface ) return;
	bool any = false;
	for( int i = 0; i < 112 * 112 && !any; ++i ) any = DeathMonstersMap[ 0 ][ i ] || BloodMap[ 0 ][ i ];
	if( !any ) return;
	const size_t size = (size_t)WorkingWidth * ( Screen_TopBorder + ScreenHeight + 128 );
	if( SM_Mask.size() != size ){ SM_Mask.assign( size, 0 ); SM_Stamp = 0; }
	if( ++SM_Stamp == 0 ){ std::fill( SM_Mask.begin(), SM_Mask.end(), (ushort)0 ); SM_Stamp = 1; }
	SM_On = true;
}

static void StandMask_BeginSprite( int x0, int y0, int x1, int y1 )
{
	if( !SM_On ) return;
	if( SM_Depth >= SM_DEPTH ){ ++SM_Depth; return; }		// too deep: this one is not masked
	SMBox& b = SM_Stack[ SM_Depth ];
	b = SM_Clamp( x0, y0, x1, y1 );
	if( b.x1 > b.x0 && b.y1 > b.y0 ) SM_Save( b, SM_StackPx[ SM_Depth ], WorkingSurface );
	++SM_Depth;
}

// Stamp what changed in the box of stack entry i since its snapshot.
static void SM_MarkEntry( int i )
{
	const SMBox& b = SM_Stack[ i ];
	if( b.x1 <= b.x0 || b.y1 <= b.y0 ) return;
	const std::vector<uchar>& before = SM_StackPx[ i ];
	const int w = b.x1 - b.x0;
	for( int y = b.y0; y < b.y1; ++y ){
		const int o = YOffsetHashTable[ y ] + b.x0;
		const uchar* now = WorkingSurface + o;
		const uchar* was = &before[ (size_t)( y - b.y0 ) * w ];
		ushort* m = &SM_Mask[ o ];
		// (th4) PERFORMANCE: 16 pixels at a time, skipping unchanged blocks -- most of the generous box is
		// untouched by the sprite. Same stamps as the plain per-pixel loop (the tail below). This loop ran
		// for every monster draw and every repaint inside one; with a crowd of monsters and a corpse on the
		// map it was the bulk of an 11 ms per frame sprite cost (measured in the Hellforge, 2026-10-05).
		int x = 0;
		for( ; x + 16 <= w; x += 16 ){
			const int eq = _mm_movemask_epi8( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( now + x ) ), _mm_loadu_si128( (const __m128i*)( was + x ) ) ) );
			if( eq == 0xFFFF ) continue;
			for( int k = 0; k < 16; ++k ) if( !( eq & ( 1 << k ) ) ) m[ x + k ] = SM_Stamp;
		}
		for( ; x < w; ++x ) if( now[ x ] != was[ x ] ) m[ x ] = SM_Stamp;
	}
	if( SM_BoxCount < 256 ) SM_Boxes[ SM_BoxCount++ ] = b;
}

static void StandMask_EndSprite()
{
	if( !SM_On || SM_Depth <= 0 ) return;
	--SM_Depth;
	if( SM_Depth >= SM_DEPTH ) return;
	SM_MarkEntry( SM_Depth );
}

// (th4) DrawMapCell repaints a neighbour cell (floor, walls) in the middle of a monster draw
// (DrawMonsterInTile calls it). Those repainted pixels are scenery, not the monster: stamp what the
// monster drew so far, and after the repaint start a fresh snapshot, so only the monster's own
// pixels are stamped. Called from DrawMapCell's entry and exit.
static void StandMask_Suspend()
{
	if( !SM_On || SM_Depth <= 0 || SM_Depth > SM_DEPTH ) return;
	SM_MarkEntry( SM_Depth - 1 );
}
static void StandMask_Resume()
{
	if( !SM_On || SM_Depth <= 0 || SM_Depth > SM_DEPTH ) return;
	const SMBox& b = SM_Stack[ SM_Depth - 1 ];
	if( b.x1 > b.x0 && b.y1 > b.y0 ) SM_Save( b, SM_StackPx[ SM_Depth - 1 ], WorkingSurface );
}

static void Decal_Begin( int x0, int y0, int x1, int y1 )
{
	SM_DecalOn = false;
	if( !SM_On || !SM_BoxCount ) return;
	const SMBox d = SM_Clamp( x0, y0, x1, y1 );
	if( d.x1 <= d.x0 || d.y1 <= d.y0 ) return;
	bool touches = false;
	for( int i = 0; i < SM_BoxCount && !touches; ++i ){
		const SMBox& s = SM_Boxes[ i ];
		touches = s.x0 < d.x1 && d.x0 < s.x1 && s.y0 < d.y1 && d.y0 < s.y1;
	}
	if( !touches ) return;
	SM_Decal = d;
	SM_Save( d, SM_DecalPx, WorkingSurface );
	if( g_A8L8Lighting && LightSurface ) SM_Save( d, SM_DecalL, LightSurface );
	SM_DecalOn = true;
}

static void Decal_End()
{
	if( !SM_DecalOn ) return;
	SM_DecalOn = false;
	const SMBox& d = SM_Decal;
	const int w = d.x1 - d.x0;
	const bool light = g_A8L8Lighting && LightSurface && SM_DecalL.size() == SM_DecalPx.size();
	for( int y = d.y0; y < d.y1; ++y ){
		const int o = YOffsetHashTable[ y ] + d.x0;
		const size_t r = (size_t)( y - d.y0 ) * w;
		const ushort* m = &SM_Mask[ o ];
		// (th4) PERFORMANCE: 8 mask entries at a time, skipping blocks with no stamped pixel (most of them);
		// the same pixels are put back as by the plain per-pixel loop.
		const __m128i stamp = _mm_set1_epi16( (short)SM_Stamp );
		int x = 0;
		for( ; x + 8 <= w; x += 8 ){
			if( !_mm_movemask_epi8( _mm_cmpeq_epi16( _mm_loadu_si128( (const __m128i*)( m + x ) ), stamp ) ) ) continue;
			for( int k = x; k < x + 8; ++k ){
				if( m[ k ] != SM_Stamp ) continue;
				WorkingSurface[ o + k ] = SM_DecalPx[ r + k ];
				if( light ) LightSurface[ o + k ] = SM_DecalL[ r + k ];
			}
		}
		for( ; x < w; ++x ){
			if( m[ x ] != SM_Stamp ) continue;
			WorkingSurface[ o + x ] = SM_DecalPx[ r + x ];
			if( light ) LightSurface[ o + x ] = SM_DecalL[ r + x ];
		}
	}
}

// (th4) Blood pool and corpse of one cell -- the floor decals. Drawn by DrawDynamicObjects in
// the cell's own turn, and again by DrawMapCell for the cells right in front of a repainted cell
// (see REPAINT DECALS there). Uses the global DarkLevel as the cell light (the caller sets it).
// Corpses and blood are drawn in fully dark cells too when per-pixel light is on: the main pass
// then leaves LightSurface alone (Engine.cpp Rle2Draw, "pre-fill handles it"), so they take the
// floor's own per-pixel light and fade out with it instead of popping in and out whole at the
// edge of the hero's light. Classic lighting keeps the old DarkLevel < LightMax gate.
static void DrawCellDecals( int row, int col, int playerOffsetX, int playerOffsetY, int tileInCellIndex )
{
	const int cell = 112 * row + col;
	const uchar bloodNum = BloodMap[ 0 ][ cell ];
	const ushort deadMonsterSpriteNum = DeathMonstersMap[ 0 ][ cell ];
	if( !bloodNum && !deadMonsterSpriteNum ) return;
	const bool decalsVisible = DarkLevel < LightMax || ( g_A8L8Lighting && PixelLight && g_InDungeonRender );
	if( !decalsVisible ) return;
	    if( bloodNum > 0 && bloodNum <= 9){
	        int bloodIndex = bloodNum - 1;
	        DeadStruct& bb = blood[bloodIndex];
	        int offsetX = playerOffsetX - bb.deadWidth2;
			char* orientedPtr = bb.oriented[0].data;
			int data1 = bb.deadFrame;
			if( orientedPtr && data1 >= 1 && *(int*)orientedPtr <= 50 && data1 <= *(int*)orientedPtr ){
				Decal_Begin( offsetX - 8, playerOffsetY - 192, offsetX + bb.deadWidth + 8, playerOffsetY + 16 );	// (th4) STANDING-SPRITE MASK
				if( bb.deadTrans ){
					CL2_DrawDark(offsetX, playerOffsetY, orientedPtr, data1, bb.deadWidth, 0, tileInCellIndex, bb.deadTrans);
				}else{
					CL2_Draw(offsetX, playerOffsetY, orientedPtr, data1, bb.deadWidth, 0, tileInCellIndex, true);
				}
				Decal_End();
			}
        }
		// draw died monsters
		if( deadMonsterSpriteNum > 0 && (deadMonsterSpriteNum & 0xff) <= DeadMonsters_Sprites_Max_Count ){
			int spriteIndex = (deadMonsterSpriteNum & 0xff) - 1;
			int dir = (deadMonsterSpriteNum >> 8) & 7;
			DeadStruct& dd = dead[spriteIndex];
			int offsetX = playerOffsetX - dd.deadWidth2;
			int offsetY = playerOffsetY;
			int tilesInCell = tileInCellIndex;
			MonsterPosAdjust(&dd.oriented[dir], offsetX, offsetY, tilesInCell);
			char* orientedPtr = dd.oriented[dir].data;
			int data1 = dd.deadFrame;
			if( orientedPtr && data1 >= 1 && *(int*)orientedPtr <= 70 && data1 <= *(int*)orientedPtr ){
				Decal_Begin( offsetX - 8, offsetY - 224, offsetX + dd.deadWidth + 8, offsetY + 16 );	// (th4) STANDING-SPRITE MASK
				if( dd.deadTrans ){
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteLight( offsetX, playerOffsetY, (int) MonsterSprites[spriteIndex].animation[ANIM_4_DIE].oriented[orient], MonsterSprites[spriteIndex].animation[ANIM_4_DIE].frameCount - 1, MonsterSprites[spriteIndex].Pitch, 0, tileInCellIndex, 1 );

					CL2_DrawDark(offsetX, offsetY, orientedPtr, data1, dd.deadWidth, 0, tilesInCell, dd.deadTrans);
				}else{
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteDarken( offsetX, playerOffsetY,	(int) MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].oriented[ orient ],	MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].frameCount - 1, MonsterSprites[ spriteIndex ].Pitch, 0, tileInCellIndex );
					
					CL2_Draw(offsetX, offsetY, orientedPtr, data1, dd.deadWidth, 0, tilesInCell, true);
				}
				Decal_End();
			}
		}
}

// (rt) draws all dynamic objects: players, monsters, chests, items on the ground... tileInCellIndex
// (rt) bug: dynamic objects in the same cell as the player are not drawn, e.g. the town portal
//----- (00469B8C) --------------------------------------------------------
void __fastcall DrawDynamicObjects( uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, bool firstDraw ) // DrawHTLXsub
{
	int cell = 112 * row + col;
	uchar placeFlag = FlagMap[ 0 ][ cell ];
	int objectSprite = ObjectsMap[ 0 ][ cell ];
	int itemNum = ItemMap[ 0 ][ cell ];
	ushort deadMonsterSpriteNum = DeathMonstersMap[ 0 ][ cell ];
	uchar bloodNum = BloodMap[ 0 ][ cell ];
	char arch = ArchGraphicsMap[ 0 ][ cell ];
	uchar transparent = TransMap[ 0 ][ cell ];
	char playerNum = PlayerMap[ 0 ][ cell ];
	char prevPlayerNum = PlayerMap[ 0 ][ cell - 1 ];
	if( TestFOPMFlag && placeFlag & CF_64_VISIBLE_BY_CURSOR ){
		WriteTileToSurface(surfaceDest, SquareCELPtr, 1, 64, 0, tileInCellIndex);
	}
	if( MayBeExplodingMagicFlag && placeFlag & CF_1_SPELL_CAST ){
		DrawMissileInTile(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 1);
	}
	// (th4) Corpses and blood are drawn in fully dark cells too when per-pixel light is on: the
	// main pass then leaves LightSurface alone (Engine.cpp Rle2Draw, "pre-fill handles it"), so
	// they take the floor's own per-pixel light and fade out with it. The old gate dropped a
	// corpse the moment its cell's integer light reached LightMax while the floor there could
	// still look lit, so corpses at the edge of the hero's light popped in and out whole. Classic
	// lighting keeps the gate (there the cell light IS what the floor shows); objects keep it.
	DrawCellDecals( row, col, playerOffsetX, playerOffsetY, tileInCellIndex );	// (th4) blood + corpse
	if( DarkLevel < LightMax ){
		if( objectSprite ){
			DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 1, 0, tileInCellIndex);
		}
	}
	if( itemNum > 0 && itemNum <= MaxItems_255 ){ //	(rt) draw the items on the ground
		Item* item = &Items[itemNum - 1];
		if( !item->dropping ){
			if( item->ItemCELFilePtr && item->FrameIndex >= 1 ){
				if( *(int*)item->ItemCELFilePtr >= item->FrameIndex && *(uint*)item->ItemCELFilePtr <= 50 ){
					int outlineColor = IsAltPressed || AltHighlight || itemNum - 1 == Cur.ItemID /*|| AutomapMode == 1*/ ? 181 : -1;
					DrawItemOnMap(playerOffsetX - item->subOffsetX, playerOffsetY, item->ItemCELFilePtr, item->FrameIndex, item->AnimWidth, 0, tileInCellIndex, outlineColor);
				}
			}
		}
	}
	if( placeFlag & CF_32_PLAYER_HORIZONTAL ){
		int playerIndex = -1 - prevPlayerNum; // (rt) there was a duplication bug here when shots hit during horizontal movement
		if( (uint) playerIndex < PlayersMax_4 ){
			Player& player = Players[playerIndex];
			g_SkipSpriteLightClear = true;
			StandMask_BeginSprite( playerOffsetX + player.xOfs - player.animWidth2 - 8, playerOffsetY + player.yOfs - 192, playerOffsetX + player.xOfs - player.animWidth2 + player.animWidth + 8, playerOffsetY + player.yOfs + 16 );	// (th4)
			// (th4) SNOW DEPTH: the hero walking HORIZONTALLY is drawn here, not at the DrawPlayer call below -- same wrap as there
			// (Snowfall.cpp). The feet are measured from THIS cell (the offsets drawn with), like TownMap.cpp's horizontal branch.
			SnowDepth_Begin( playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.animWidth, !Snowfall_Active() || TownWater_FeetBare( row, col, player.xOfs, player.yOfs ), SnowDepthSlope( player.dir ), true );
			DrawPlayer(playerIndex, row, col - 1, playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
			SnowDepth_End( player.currentAnimationPointer, player.currentAnimationFrame );
			StandMask_EndSprite();
			g_SkipSpriteLightClear = false;
			if( firstDraw && player.UnderArcType ){
				if( player.UnderArcType == 2 ){
					DrawMapCell(surfaceDest - WorkingWidth * 16 - 96, row - 2, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 96, playerOffsetY - 16);
				}
				DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
			}
		}
	}
	// draw horizontal moving monsters
	if( placeFlag & CF_16_MONSTER_HORIZONTAL && MonsterMap[0][cell - 1] < 0 ){
//		g_SkipSpriteLightClear = true;// If enabled, A8L8 path with screen space PPL will make monsters behind opaque walls invisible, this also allow monsters have ambient shadow blend into them.
		StandMask_BeginSprite( playerOffsetX - 160, playerOffsetY - 224, playerOffsetX + 160, playerOffsetY + 32 );	// (th4) STANDING-SPRITE MASK
		DrawMonsterInTile(-MonsterMap[0][cell - 1] - 1, surfaceDest, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY, firstDraw, placeFlag);
		StandMask_EndSprite();
//		g_SkipSpriteLightClear = false;
	}
	if( placeFlag & CF_4_DEAD_PLAYER ){
		StandMask_BeginSprite( playerOffsetX - 96, playerOffsetY - 192, playerOffsetX + 160, playerOffsetY + 24 );	// (th4) STANDING-SPRITE MASK
		Draw_Dying_Player(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
		StandMask_EndSprite();
	}
	if( playerNum > 0 ){
		int playerIndex = playerNum - 1;
		if( (uint) playerIndex < PlayersMax_4 ){
			Player& player = Players[playerIndex];

			// Screen-space PPL: skip LightSurface clearing during player draw.
			// The pre-fill already sets LightSurface≈0 at the player's position
			// (center of light radius), so the player stays bright.  This protects
			// the archway cell's LightSurface from being cleared by the player
			// sprite/outline — no save/restore needed.
			g_SkipSpriteLightClear = true;
			StandMask_BeginSprite( playerOffsetX + player.xOfs - player.animWidth2 - 8, playerOffsetY + player.yOfs - 192, playerOffsetX + player.xOfs - player.animWidth2 + player.animWidth + 8, playerOffsetY + player.yOfs + 16 );	// (th4)
			// (th4) SNOW DEPTH (Snowfall.cpp): Ice Age Ravenholm's heroes stand knee-deep, outline included, and lose their painted shadow.
			// TWO hero draws in this function: here and the horizontal-move branch above (CF_32_PLAYER_HORIZONTAL) -- keep both.
			// Here and not inside DrawPlayer: the Enriched Light replay calls DrawPlayer again (Snowfall.cpp ENRICHED LIGHT).
			SnowDepth_Begin( playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.animWidth, !Snowfall_Active() || TownWater_FeetBare( row, col, player.xOfs, player.yOfs ), SnowDepthSlope( player.dir ), true );
			DrawPlayer(playerIndex, row, col, playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
			SnowDepth_End( player.currentAnimationPointer, player.currentAnimationFrame );
			StandMask_EndSprite();
			g_SkipSpriteLightClear = false;
			if( firstDraw && player.UnderArcType ){
				if( player.UnderArcType == 2 ){
					DrawMapCell(surfaceDest - WorkingWidth * 16 - 96, row - 2, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 96, playerOffsetY - 16);
				}
				DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
			}
		}
	}
	if( MonsterMap[0][cell] > 0 ){
//		g_SkipSpriteLightClear = true;// If enabled, A8L8 path with screen space PPL will make monsters behind opaque walls invisible, this also allow monsters have ambient shadow blend into them.
		StandMask_BeginSprite( playerOffsetX - 160, playerOffsetY - 224, playerOffsetX + 160, playerOffsetY + 32 );	// (th4) STANDING-SPRITE MASK
		DrawMonsterInTile(MonsterMap[0][cell] - 1, surfaceDest, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY, firstDraw, placeFlag);
		StandMask_EndSprite();
//		g_SkipSpriteLightClear = false;
		// (th4) DC mission 13: Lazarus wants a word once the lair is clear, so he wears the
		// same exclamation mark a towner with something to say does. Same NPC mark at the same
		// place as TownMap.cpp (NpcMark.cpp: noted here, drawn after the whole world), and it
		// honours the NpcSign option exactly as towners do.
		// Drawn here rather than inside DrawMonsterInTile, because that body only runs while
		// the cursor is on him -- the mark has to be visible from across the room.
		if( NpcSign && GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
		 && !DarkCampaignLazarusThanked ){
			int lazIdx = DarkCampaignLazarusIndex();
			if( lazIdx >= 0 && MonsterMap[0][cell] - 1 == lazIdx && DarkCampaignLazarusLairClear() ){
				NpcMark_Note( playerOffsetX + 32, playerOffsetY - 103 );
			}
		}
		// (th4) DC mission 25: the Butcher wants out of his cell. The mark shows from the
		// moment the player can see him -- unlike Lazarus and the armorer there is nothing
		// to finish first, freeing him IS the mission.
		if( NpcSign && GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_111_CHURCH_SACRISTY
		 && !DarkCampaignButcherIsFreed() ){
			int butIdx = DarkCampaignButcherIndex();
			if( butIdx >= 0 && MonsterMap[0][cell] - 1 == butIdx ){
				NpcMark_Note( playerOffsetX + 32, playerOffsetY - 103 );
			}
		}
		// (th4) DC mission 15: the armorer wants a word once the forge is safe.
		if( NpcSign && GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
		 && !DarkCampaignHephastoThanked ){
			int hepIdx = DarkCampaignHephastoIndex();
			// ...and only once he is actually back at his anvil -- no mark while he is
			// still crossing the room. DarkCampaignHephastoAtForge() is the same test the
			// talk uses, so the mark and the ability to speak appear together.
			if( hepIdx >= 0 && MonsterMap[0][cell] - 1 == hepIdx
			 && DarkCampaignHellforgeClear() && DarkCampaignHephastoAtForge() ){
				NpcMark_Note( playerOffsetX + 32, playerOffsetY - 103 );
			}
		}
	}
	if( placeFlag & CF_1_SPELL_CAST ){ // (rt) ForbMap diverges from MisMap here, resulting in a crash on index -1
		DrawMissileInTile(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	// (th4) THIEF: objects on PITCH-DARK cells are normally never drawn at all (the
	// DarkLevel < LightMax gate below) -- which made every in-sprite brightening fix
	// unreachable. Per Mordor's design: holding Alt reveals them for her; the existing
	// Alt outline (color 194 in DrawObjectSprite) plus the Thief DarkLevel cap and the
	// per-pixel LightSurface clear make them readable once the call is allowed through.
	bool thiefAltRevealsObjects = (IsAltPressed || AltHighlight)
		&& MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Thief);
	// (th4) ALT REVEALS LOOTABLE OBJECTS: queued here, drawn after the scene.
	// Opening this gate is NOT enough on its own -- that only defeats darkness, and
	// the real problem is OCCLUSION: anything drawn from inside this back-to-front
	// tile loop is painted over by any cell drawn afterwards (see the QueueOverheadHpBar
	// note). So the outline is queued and flushed after the whole scene, exactly like
	// the overhead HP bars -- which is what makes it work in EVERY lighting mode.
	// The gate is still opened so the queue sees objects on pitch-dark cells too.
	const int altLootIdx = objectSprite ? abs(objectSprite) - 1 : -1;
	const bool altRevealsLoot = (IsAltPressed || AltHighlight) && !ProhibitAltHighlight
		&& altLootIdx >= 0 && altLootIdx < MaxObjects_127
		&& IsLootableObject( Objects[ altLootIdx ] );
	const bool objectDrawnInline = objectSprite && (DarkLevel < LightMax || thiefAltRevealsObjects);
	if( altRevealsLoot ){
		QueueAltLootOutline( altLootIdx, row, col, playerOffsetX, playerOffsetY, tileInCellIndex, objectDrawnInline );
	}
	if( objectDrawnInline ){ // (rt) there cannot be more than one object in a cell
		DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 0, 0, tileInCellIndex);
	}
	if( itemNum > 0 && itemNum <= MaxItems_255 ){
		Item* item = &Items[itemNum - 1];
		if( item->dropping && item->ItemCELFilePtr != 0 && item->FrameIndex >= 1 ){
			if( *(int*)item->ItemCELFilePtr >= item->FrameIndex && *(uint*)item->ItemCELFilePtr <= 50 ){ // (rt) limit of 50 flip frames?
				int outlineColor = IsAltPressed || AltHighlight || itemNum - 1 == Cur.ItemID /*|| AutomapMode == 1*/ ? 181 : -1;
				DrawItemOnMap(playerOffsetX - item->subOffsetX, playerOffsetY, item->ItemCELFilePtr, item->FrameIndex, item->AnimWidth, 0, tileInCellIndex, outlineColor);
			}
		}
	}
	if( arch && Dungeon->genType ){
		TransparentMode = TransList[transparent];
		g_IsDrawingOpaqueWall = true;  // (th4) arches are opaque walls for silhouette
		if (g_A8L8Lighting) {
			const int TransparentModeSaved = TransparentMode;
			TransparentMode = 0;// Always force opaque archways for A8L8 path, as player and monster can be partially seen through opaque walls.
			CEL_DrawShadow(surfaceDest, CurrentLevelSCELFile, arch, 64, 0, tileInCellIndex);
			TransparentMode = TransparentModeSaved;
		}
		else {
			CEL_DrawShadow(surfaceDest, CurrentLevelSCELFile, arch, 64, 0, tileInCellIndex);
		}
		g_IsDrawingOpaqueWall = false;  // (th4)
	}
}

//----- (th2) -------------------------------------------------------------
int __fastcall CheckMonsterInInfravisionRange(int monsterRow, int monsterCol, int distance)
{
	static int prevDistance;
	static int rms;

	if (distance != prevDistance) {
		prevDistance = distance;
		rms = distance * distance;
		++distance;
		rms += (distance * distance - rms) >> 1;
	}

	const Player& player = Players[CurrentPlayerIndex];
	int deltaRow = monsterRow - player.Row;
	int deltaCol = monsterCol - player.Col;

	return deltaRow * deltaRow + deltaCol * deltaCol < rms ? true : false;
}


// (th4) The Thief trait and the Terminator / Super Mutant / Enclave Soldier game
// changers all use infravision in pitch-dark dungeons, where ambient-brightness
// monsters would be nearly invisible. They all get the same A8L8 post-render
// treatment as InfravisionMonsterThief: open pixels pinned bright
// (kThiefInfravisionBright), wall/object-occluded pixels keep ambient (no silhouette).
static bool InfravisionMonstersPinnedBright()
{
	const Player& player = Players[CurrentPlayerIndex];
//	return HasTrait(CurrentPlayerIndex, TraitId::Thief) || (player.gameChanger & BIT(GC_27_TERMINATOR));
	return true;// Force max brightness infravision for all A8L8 modes, normal monsters under infravision no longer darken with ambient.
}

//----- (0046A1FB) --------------------------------------------------------
void __fastcall DrawMonster(int row, int col, int monsterX, int monsterY, unsigned int monsterIndex, int shiftY, int tileInCellIndex)
{
	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite* monsterSprite = monster.SpritePtr;
	char* curAnimation = monster.curAnimation->data;
	if (!curAnimation) {
		return;
	}
	int curFrame = monster.CurFrame;
	int maybeCountOfFrames = *(uint*)curAnimation;
	if (curFrame < 1) {
		return;
	}
	if (maybeCountOfFrames > 70 || curFrame > maybeCountOfFrames) {
		return;
	}
	int pitch = monsterSprite->Pitch;

	// Overhead HP bar (config-gated). Drawn for every visible monster regardless of
	// which draw path is taken below (normal, boss/lit, or stoned), so unique bosses
	// (newBossId) get a bar too. Minions (summon range) are friendly/blue, others red.
	auto drawOverheadBar = [&]() {
		if (Dungeon == DUN_117_DARKLORD_HIDEOUT) return; // (th4) hub kin are NPCs -- no HP bar
		const bool isMinion = monsterIndex < SummonMonsters_Count;
		if (!isMinion && monster.SpritePtr && OverheadBar_IsSuppressed(monster.SpritePtr->baseMonsterIndex)) return; // no-bar list
		if ((isMinion && Display_Player_HP_bars) || (!isMinion && Display_Monster_HP_bars)) {
			if (monster.goal != MG_0_DEATH && monster.BaseLife > 0) {
				int frameH = (monster.BasePtr ? monster.BasePtr->SpriteHeight : 0);
				if (frameH <= 0) frameH = OverheadBar_DefaultHeight;
				const int centerX = monsterX + pitch / 2;
				const int barBot = monsterY - frameH - OverheadBar_Gap;
				QueueOverheadHpBar(centerX, barBot, monster.CurrentLife, monster.BaseLife,
					isMinion ? OverheadBar_Blue : (monster.newBossId ? OverheadBar_Gold : OverheadBar_Red));
			}
		}
		};

	// (th4) OUT-OF-SIGHT branch: the monster's cell is not in the player's line of sight
	// (CF_64_VISIBLE_BY_CURSOR, set by DoVision, automap.cpp ~1421). Monsters here are only drawn
	// as the solid red INFRAVISION silhouette (CL2_DrawDark flag 1 = the Infra.TRN row of
	// LightTable), and only in infravision range. Player minions (monsterIndex <
	// SummonMonsters_Count) used to take this branch too, with or without infravision, so a Golem
	// or any summon whose cell was merely out of line of sight -- a cell in a wall alcove or
	// behind a wall corner, typically warp / stair / portal spots, even inside the lit area --
	// was painted as a flat red/pink blob. Minions now take this branch only when the player
	// really has infravision; otherwise they fall through and are drawn normally (lit by their
	// cell's DarkLevel, with their HP bar). Known-good check: a Golem standing next to the stairs
	// looks the same as one in the open; with an infravision item it turns red in the dark like
	// any monster.
	const bool minionOwnSight = monsterIndex < SummonMonsters_Count && !Players[CurrentPlayerIndex].Infravision;
	if (!(FlagMap[row][col] & CF_64_VISIBLE_BY_CURSOR) && !minionOwnSight) {
		const Player& player = Players[CurrentPlayerIndex];
		int light_radius = player.ClassID == PC_1_ARCHER ? 11 : 10;
		if (/*abs(monster.Row - Players[CurrentPlayerIndex].Row) < 9 && abs(monster.Col - Players[CurrentPlayerIndex].Col) < 9*/
			(CheckMonsterInInfravisionRange(row, col, light_radius) || monsterIndex < SummonMonsters_Count) && !TileBlockSight[FineMap[row][col]]) { // Infravision limit
			// Option C: save infravision monster for post-render
			if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_LightSurfaceOnlyPass) {
				if (g_PostRenderSpriteCount < MAX_POSTRENDER_SPRITES) {
					auto& cmd = g_PostRenderSprites[g_PostRenderSpriteCount++];
					cmd.x = monsterX; cmd.y = monsterY;
					cmd.cl2Buf = curAnimation; cmd.frameNum = curFrame;
					cmd.pitch = pitch; cmd.shiftY = shiftY;
					cmd.tileInCellIndex = tileInCellIndex;
//					cmd.darken = true;
//					cmd.darkLevel = (uchar)DarkLevel;
					// (th4) Infravision brightness is cell-independent (like Infra.TRN); tying it to per-cell DarkLevel causes flicker
					cmd.darken = false;
					cmd.darkLevel = 0;// not used when darken=false, but zero for clarity
					// (th4) THIEF + infravision: pitch-dark ambient would hide the red tint.
					// InfravisionMonsterThief pins open pixels bright and leaves
					// wall-occluded pixels at ambient (no silhouette). Bosses get the
					// same treatment: giving them Monster type here would write full
					// brightness onto wall-occluded pixels (lightFill=0 → silhouetteBright=0)
					// and paint a glowing boss-shaped silhouette on the wall.
					cmd.spriteType = InfravisionMonstersPinnedBright()
						? SpriteType::InfravisionMonsterThief : SpriteType::InfravisionMonster;  // (th4) infravision: no silhouette
					cmd.tintExclude = false;  // (th4) Stage 3: no owner-pixel tint override
				}
			}
			CL2_DrawDark(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, 1);
		}
		return;
	}
	int someFlag = 0; // monster light index 
	if (monster.newBossId) {
		someFlag = monster.uniqTrans + 4;
	}
	if (monster.CurAction == A_15_STONE) {
		someFlag = 2;
	}
	// (th4) In A8L8, infravision must NOT override boss/stone light because
	// the post-render pass treats InfravisionMonster as "skip all LightSurface
	// writes", leaving stale LightSurface=255 from earlier tile rendering
	// (pitch-black cells north of the boss) -- causing frame-to-frame flashing.
	// Non-A8L8 has no LightSurface, so the original override is harmless.
	if (Players[CurrentPlayerIndex].Infravision && DarkLevel > 8
		&& (!g_A8L8Lighting || !someFlag)) {
		someFlag = 1;
	}
	if (someFlag) {
		// Option C: save boss/stoned/infravision monster for post-render.
		// Use CL2_Draw (not CL2_DrawDark) in post-render since only LightSurface matters.
		if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_LightSurfaceOnlyPass) {
			if (g_PostRenderSpriteCount < MAX_POSTRENDER_SPRITES) {
				auto& cmd = g_PostRenderSprites[g_PostRenderSpriteCount++];
				cmd.x = monsterX; cmd.y = monsterY;
				cmd.cl2Buf = curAnimation; cmd.frameNum = curFrame;
				cmd.pitch = pitch; cmd.shiftY = shiftY;
				cmd.tileInCellIndex = tileInCellIndex;
//				cmd.darken = true;
				cmd.darken = false;// Do NOT darken boss, as they have special light radius and will flicker when moving if darkened.
				cmd.darkLevel = (uchar)DarkLevel;
				// (th4) someFlag=1 -> infravision -> no silhouette; otherwise normal Monster behavior
				if (someFlag == 1) {
					// (th4) THIEF + infravision: ambient is pitch black, so the red-tinted
					// sprite must not inherit it. InfravisionMonsterThief pins open pixels
					// bright and leaves wall-occluded pixels at ambient (no half-red /
					// half-silhouette split).
					cmd.spriteType = InfravisionMonstersPinnedBright()
						? SpriteType::InfravisionMonsterThief : SpriteType::InfravisionMonster;
				} else {
					cmd.spriteType = SpriteType::Monster; // boss / stoned keep existing behavior
				}
				cmd.tintExclude = false;  // (th4) Stage 3: no owner-pixel tint override
			}
		}
		CL2_DrawDark(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, someFlag);
		if( !g_LightSurfaceOnlyPass ) Breath_NoteMonster( monsterIndex, monsterX, monsterY, curAnimation, curFrame, pitch );	// (th4) COLD BREATH (Breath.cpp; skips stoned ones)
		drawOverheadBar(); // bosses (newBossId) and stoned monsters exit here - draw their bar too
		return;
	}
	// Option C: save for post-render LightSurface re-draw
	if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_LightSurfaceOnlyPass) {
		if (g_PostRenderSpriteCount < MAX_POSTRENDER_SPRITES) {
			auto& cmd = g_PostRenderSprites[g_PostRenderSpriteCount++];
			cmd.x = monsterX; cmd.y = monsterY;
			cmd.cl2Buf = curAnimation; cmd.frameNum = curFrame;
			cmd.pitch = pitch; cmd.shiftY = shiftY;
			cmd.tileInCellIndex = tileInCellIndex;
			cmd.darken = true;
			cmd.darkLevel = (uchar)DarkLevel;
			cmd.spriteType = SpriteType::Monster;  // (th4)
			cmd.tintExclude = false;  // (th4) Stage 3: no owner-pixel tint override
		}
	}
//	CL2_Draw(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, true);
	{
		// (th4) town night: towners, cows and any monster in town darken with the dusk
		// phase, matching the terrain palette blend (DayNight_TownSpriteDark: 0 outside
		// town / by day, up to 8 at full night).
		int nightDark = DayNight_TownSpriteDark();
		int nightPrevDark = DarkLevel;
		if( nightDark > DarkLevel ) DarkLevel = nightDark;
		CL2_Draw(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, true);
		DarkLevel = nightPrevDark;
	}
	if( !g_LightSurfaceOnlyPass ) Breath_NoteMonster( monsterIndex, monsterX, monsterY, curAnimation, curFrame, pitch );	// (th4) COLD BREATH (Breath.cpp)

	drawOverheadBar();
}

//----- (0046A2F5) --------------------------------------------------------
void __fastcall DrawObjectSprite(int row, int col, int playerOffsetX, int playerOffsetY, int someFlag, int upHalfCell, int screenCellRow)
{
	int colPixelPos;
	int rowPixelPos;
	char objectNum = ObjectsMap[ row ][ col ];
	char objectIndex = abs(objectNum)-1;
	Object& object = Objects[objectIndex];//(rt) understood
	if( objectNum <= 0 ){
		if( object.InAction != someFlag ){
			return;
		}
		screenCellRow = 8; // (rt) checked why it is 8 
		int rowDelta = object.Row - row;
		int colDelta = object.Col - col;
		rowPixelPos = playerOffsetX + 32 * rowDelta - object.OffsetX - 32 * colDelta;
		colPixelPos = playerOffsetY + 16 * (colDelta + rowDelta); // (rt) all local variables should start with a lowercase letter, globals and function names with an uppercase one
		upHalfCell = 0; // (rt) more convenient this way
	}else{
		if( object.InAction != someFlag ){
			return;
		}
		colPixelPos = playerOffsetY;
		rowPixelPos = playerOffsetX - object.OffsetX;
	}
	// (th4) town night: town objects (the Abyss entrance above all) darken with the dusk
	// phase like the towners do. Applied before the Thief block so her cap still guarantees
	// her a readable silhouette; restored via the same save/restore below.
	int thiefSavedDarkLevel = DarkLevel;	// (also the pre-night value -- restored at function end)
	{
		int nightDark = DayNight_TownSpriteDark();
		if( nightDark > DarkLevel ) DarkLevel = nightDark;
	}
	// (th4) THIEF: she must see dungeon objects (barrels, chests, doors...) even on pitch-dark
	// tiles -- her whole playstyle lives in the dark. Cap the draw darkness for OBJECT sprites
	// only; monsters/items/terrain keep true ambient. Same save/restore idiom as DrawMapCell.
	if( MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Thief) && DarkLevel > 10 ){
		DarkLevel = 10; // dim silhouette, not full bright
	}
	if( objectIndex >= 0 && objectIndex < MaxObjects_127 && object.CelFilePtr ){  // (rt) struct120Array127[63].field_14 = 165, should be a pointer to int
		int maxFrameIndex = *(int*)object.CelFilePtr;// (rt) some object has a broken pointer, resulting in a crash when accessing a protected area
		if( object.FrameIndex >= 1 && maxFrameIndex <= 50 && object.FrameIndex <= maxFrameIndex ){
			// (th4) Trapped objects and explosive pods ring RED -- see IsAwarenessDanger.
			// Kept identical to the queued Alt reveal in FlushAltLootOutlines, or an
			// object would change colour as it came into view.
			const bool altOutline = (IsAltPressed || AltHighlight) && object.selectable;
			const bool ringTrapped = IsAwarenessDanger( object );
			int outlineColor = ( altOutline || objectIndex == Cur.ObjectIndex )
				? ( ringTrapped ? AltLoot_RingTrapped : AltLoot_Ring ) : -1;
			// Allow LightSurface clearing only for light-source objects (torches/braziers/candles/flames)
			// that need to appear full bright.  Other objects (doors/chests) must respect ambient light.
			// Only need the g_ForceSpriteLightClear flag for the g_A8L8Lighting path.
			// List may not be complete.
			if (g_A8L8Lighting) {
				const int oi = object.BaseObjectIndex;
				const bool isLightSource = (oi >= BO_0_L1BRAZ && oi <= BO_3_TORCH)      // 0-3: braziers + torch
					                    || (oi >= BO_8_L1BRAZ && oi <= BO_10_L1BRAZ)    // 8-10: braziers + candle
					                    || (oi >= BO_15_L1BRAZ && oi <= BO_19_L1BRAZ)   // 15-19: braziers
					                    || (oi >= BO_44_WTORCH4 && oi <= BO_47_WTORCH2) // 44-47: wall torches
					                    ||  oi == BO_49_FLAME1;                         // 49: flame
				// (th4) THIEF: the DarkLevel cap above is invisible under per-pixel lighting --
				// there the darkness comes from LightSurface, not the palette tables. So for her,
				// EVERY object gets the same LightSurface clear that torches/braziers use.
				const bool thiefSeesObjects = MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Thief);
				if (isLightSource || thiefSeesObjects) g_ForceSpriteLightClear = true;
			}
			// (th4) mark drawn object pixels as WOB_OBJECT in WallOcclusionSurface so
			// the Thief-infravision post-render can tell "monster pixel covered by an
			// object" apart from "monster pixel in the open" (skip write -> ambient,
			// no silhouette behind doors/chests/etc.).
			g_IsDrawingOpaqueObject = true;
			if( object.field_C ){
				DrawItemOnMap(rowPixelPos, colPixelPos, object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow, outlineColor);
			}else if( object.CelFilePtr ){// (rt) redundant check; already checked above
				CEL_Draw(rowPixelPos, colPixelPos, object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow, 0, outlineColor);
			}
			g_IsDrawingOpaqueObject = false;
			if (g_A8L8Lighting) {
				g_ForceSpriteLightClear = false;
			}
			Flames_NoteObject( objectIndex, rowPixelPos, colPixelPos );	// (th4) sparks + halo if its sprite has a flame (Flames.cpp)
		}
	}
	DarkLevel = thiefSavedDarkLevel; // (th4) THIEF: restore true ambient for everything after
}

//----- (0046A443) --------------------------------------------------------
void __fastcall DrawMapCell( uchar* dst, int row, int col, int curTileInCell, int tileInCellIndex, int playerOffsetX, int playerOffsetY )
{
	if( ! In112(row, col) ) return;
	StandMask_Suspend();	// (th4) the repaint is scenery, not the sprite being drawn (STANDING-SPRITE MASK)
	int savedTransparentMode;         // ebx@1 — saves global TransparentMode to restore on exit
	int savedDarkLevel;         // edi@1 — saves global DarkLevel to restore on exit
	TileColumn* tileColumn; // esi@1 — pointer to this cell's tile column data
	int nextLayerTiles;        // esi@6 — pointer to next layer's tile pair (advances by 4 per layer)
	int result;     // eax@13 — dead decompilation artefact (only assigned, never read)
	int cellIndex;        // eax@1 — linear index = 112*row+col
	int wallType;        // ecx@1 — FineMap[cell] = wall/piece type for this cell
	int light;        // edx@1 — LightMap[cell] = base light level for this cell
	int transVal;        // eax@1 — TransMap[cell] = transparency flags for this cell
	int newTransparentMode;        // eax@1 — combined transparency: CellTransparent[wallType] & TransList[transVal]
	int tileMapOffset;        // eax@1 — isometric offset into TileMap array
	int leftTile;        // eax@1 — Layer[0].Left tile ID (left half of floor tile)
	int rightTile;        // eax@3 — Layer[0].Right tile ID (right half of floor tile)
	int layerLeftTile;        // eax@8 — current upper layer's left tile ID
	int layerRightTile;        // eax@10 — current upper layer's right tile ID
	int cellRow;        // [sp+10h] [bp-10h]@1 — saved row (cell row coordinate)
	int savedWallIndex;        // [sp+Ch] [bp-14h]@1 — saves global WallIndex to restore on exit
	uchar* cellDst;        // [sp+1Ch] [bp-4h]@1 — destination surface pointer for this cell
	int tileInCell; // [sp+18h] [bp-8h]@5 — current layer index (0=floor, 1..N=upper wall layers)
	cellRow = row;
	savedWallIndex = WallIndex;
	cellIndex = 112 * row + col;
	savedTransparentMode = TransparentMode;
	wallType = FineMap[ 0 ][ cellIndex ];
	savedDarkLevel = DarkLevel;
	light = LightMap[ 0 ][ cellIndex ];
	transVal = TransMap[ 0 ][ cellIndex ];
	WallIndex = wallType;
	DarkLevel = light;
	if( PixelLight ) CalculatePerPixelLight(cellRow, col);
	newTransparentMode = CellTransparent[wallType] & TransList[transVal];
	cellDst = dst;
	TransparentMode = newTransparentMode;
	tileMapOffset = IsometricMapOffset(cellRow, col);
	PartialTrans = TSP_1_LEFT;
	tileColumn = &TileMap[tileMapOffset];
	// (th4) An OPEN cell's floor is not repainted: only walls and objects must cover the sprite,
	// and a floor repaint would cut corpses of other cells reaching into it. Solid cells still
	// repaint layer 0 -- it carries the bottom of the wall / fence -- see REPAINT DECALS below.
	const bool repaintFloor = !IsOpenFloorCell( wallType );
	leftTile = tileColumn->Layer[0].Left;
	CurTileID = tileColumn->Layer[0].Left;
	PerPixelXOffset = 0;
	if( leftTile && repaintFloor )
		DrawDunCelTrans((uchar*)dst);
	PartialTrans = TSP_2_RIGHT;
	rightTile = tileColumn->Layer[0].Right;
	CurTileID = tileColumn->Layer[0].Right;
	PerPixelXOffset = 32;
	if( rightTile && repaintFloor )
		DrawDunCelTrans((uchar*)(dst + 32));
	PartialTrans = TSP_0_NONE;
	tileInCell = 1;
	if( (MaxTilesInCell >> 1) - 1 > 1 ){
		nextLayerTiles = (int)&tileColumn->Layer[1].Left;
		// === Screen-space PPL: per-layer swap is no longer needed ===
		// FillScreenSpaceLighting provides globally continuous lighting
		// for every screen pixel.  Removing the per-layer swap also
		// eliminates the discrete 32-pixel layer steps that caused
		// archway banding.
		g_IsDrawingOpaqueWall = true;  // (th4) mark WallOcclusionSurface during wall rendering
		do{
			cellDst -= WorkingWidth * 32;
			g_WallBaseTopRow = playerOffsetY - 31;	// (th4) WALL BASE LIGHT (linecpyPerPixel)
//			if (curTileInCell >= tileInCell) {
				// Draw ALL arch layers unconditionally; PutTile clips above screen.
				layerLeftTile = *(ushort*)nextLayerTiles;
				CurTileID = layerLeftTile;
				PerPixelXOffset = 0;
				if (layerLeftTile)
					DrawDunCelTrans(cellDst);
				layerRightTile = *(ushort*)(nextLayerTiles + 2);
				CurTileID = layerRightTile;
				PerPixelXOffset = 32;
				if (layerRightTile)
					DrawDunCelTrans(cellDst + 32);
//			}
			++tileInCell;
			nextLayerTiles += 4;
		}while( tileInCell < (MaxTilesInCell >> 1) - 1 );
		g_IsDrawingOpaqueWall = false;  // (th4) wall layer rendering done
		g_WallBaseTopRow = -1;
	}
	// (th4) ROCK SNOW (TownWater.cpp): this repaint covered Ravenholm's rock / roof snow -- paint it again, only on the layers
	// repainted above (the floor only if repaintFloor; layers 1 .. (MaxTilesInCell >> 1) - 2), never over the sprite
	if( Dungeon == DUN_44_RAVENHOLM ) TownWater_TileRedrawn( cellRow, col, playerOffsetX, playerOffsetY, repaintFloor ? 0 : 32, 32 * ( ( MaxTilesInCell >> 1 ) - 1 ) );
	DrawDynamicObjects(dst, cellRow, col, curTileInCell, tileInCellIndex, playerOffsetX, playerOffsetY, 0);
	// (th4) REPAINT DECALS. This repaint runs AFTER the cells in front of (cellRow, col) may
	// already be drawn (it is triggered from a moving hero / monster later in the loop), and the
	// layer 0 of a solid cell (a fence, a wall) is floor-coloured around the post. Left alone it
	// paints floor over the parts of corpses and blood pools that lie in front of the fence and
	// reach back into its cell -- tile-shaped holes in a corpse pile, only while someone walks along
	// the fence (known case: the clay golem pen, fence cells (50,43) / (51,43) against corpses
	// (50,44) / (51,44)). The two cells directly in front -- (row, col+1) at (-32, +16) and (row+1, col) at
	// (+32, +16) on screen, the same step DrawMapCell's callers use -- get their decals drawn
	// again on top. A corpse in front of the fence belongs on top of it, so this is the normal
	// draw order restored; the hero / monster that triggered the repaint stays on top of the
	// redrawn decals through the STANDING-SPRITE MASK (Decal_Begin / Decal_End in
	// DrawCellDecals). Done for every repainted cell, not only solid ones: the upper layers
	// (fence rails, wall faces) of any cell can cover a decal in front of it too.
	{
		static const int front[ 2 ][ 4 ] = { { 0, 1, -32, 16 }, { 1, 0, 32, 16 } };	// dRow, dCol, dX, dY
		for( const auto& f : front ){
			const int fr = cellRow + f[ 0 ], fc = col + f[ 1 ];
			if( !In112( fr, fc ) ) continue;
			DarkLevel = LightMap[ fr ][ fc ];
			DrawCellDecals( fr, fc, playerOffsetX + f[ 2 ], playerOffsetY + f[ 3 ], tileInCellIndex );
		}
	}
	result = savedWallIndex;
	DarkLevel = savedDarkLevel;
	TransparentMode = savedTransparentMode;
	WallIndex = savedWallIndex;
	StandMask_Resume();	// (th4)
}

//----- (0046A800) --------------------------------------------------------
void ClearScreen()
{
	// (th4) The screen's contents are going away, so the replacement-art queue retained
	// for fade replay must go with them. Without this, leaving the game to the meta menu
	// left the inventory's icons in the held queue and the menu's own fade-in replayed
	// them for a frame over the menu.
	ItemOverlay_ForgetHeldQueue();
	int rowCount;   // edx@1 — remaining screen rows to clear
	void* rowStart; // edi@1 — pointer to start of current row in WorkingSurface
	LockSrcSurface();
	rowStart = WorkingSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	rowCount = ScreenHeight;
	do{
		memset(rowStart, 0, ScreenWidth);
		rowStart = (char*)rowStart + WorkingWidth;
		--rowCount;
	}while( rowCount );
	// Clear LightSurface to 0 (full bright) so unlit areas default correctly.
	// Skip when A8L8 is disabled (LightSurface is never sampled by the L8 shader).
	if (g_A8L8Lighting && LightSurface) {
		void* lightRowStart = LightSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		rowCount = ScreenHeight;
		do{
			memset(lightRowStart, 0, ScreenWidth);
			lightRowStart = (char*)lightRowStart + WorkingWidth;
			--rowCount;
		}while( rowCount );
	}
	// (th4) Stage 2: clear the alpha-overlay pair too, same pattern as
	// LightSurface above. ClearScreen is used by the loading screen, video
	// and exit paths — none of them replay sprites, so the surfaces would
	// otherwise keep the last gameplay frame's occluded-sprite pixels and
	// the ultra upload would composite a ghost (e.g. the Town Portal the
	// player just entered) over the loading screen (§4.7 post-UI clear gap).
	if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface) {
		uchar* alphaRowStart = AlphaSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		uint* spriteRowStart = SpriteLayerSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		rowCount = ScreenHeight;
		do{
			memset(alphaRowStart, 0, ScreenWidth);
			memset(spriteRowStart, 0, ScreenWidth * sizeof(uint));
			alphaRowStart += WorkingWidth;
			spriteRowStart += WorkingWidth;
			--rowCount;
		}while( rowCount );
	}
	// (th4) Stage 3: white-fill the tint surface too — the loading/video/exit
	// uploads run RenderQuadTextureUltra (gbRunGame && DrawMode == 1) with the
	// LAST gameplay frame's tint field still in LightTintSurface, and the
	// shader would tint the loading CEL red (the Stage-2 ghost's Stage-3
	// analogue). White = no tint. Also clear the idle gate: no colored light
	// is active on non-gameplay frames, so the upload takes the idle path.
	g_LightTintActive = false;
	if (g_ARGBLighting && LightTintSurface) {
		uint* tintRowStart = LightTintSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
		rowCount = ScreenHeight;
		do{
			for (int x = 0; x < ScreenWidth; ++x) tintRowStart[x] = TINT_WHITE;
			tintRowStart += WorkingWidth;
			--rowCount;
		}while( rowCount );
	}
	UnlockSrcSurface();
}

//----- (0046A834) --------------------------------------------------------
void __fastcall DrawNotGameplayScreen(int showCursor)
{
	if( showCursor ){
		LockSrcSurface();
		RenderCursor();
		UnlockSrcSurface();
	}
	DrawGameplayToScreen();
	if( showCursor ){
		LockSrcSurface();
		DrawCursor();
		UnlockSrcSurface();
	}
}

//----- (0046A896) --------------------------------------------------------
void DrawCursor()
{
	uchar* cursorBuf;
	uchar* screenCursorRect;
	int hLeft;
	if( CurCursorWidth ){
		cursorBuf = CursorImage;
		screenCursorRect = &WorkingSurface[WorkingWidth * CurCursorStartY + WorkingWidth * Screen_TopBorder + Screen_LeftBorder] + CurCursorStartX;
		if( CurCursorHeight ){
			hLeft = CurCursorHeight;
			do{
				memcpy(screenCursorRect, cursorBuf, CurCursorWidth);
				cursorBuf += CurCursorWidth;
				screenCursorRect += WorkingWidth;
				--hLeft;
			}while( hLeft );
		}
		DrawnCursorStartX = CurCursorStartX;
		DrawnCursorStartY = CurCursorStartY;
		DrawnCursorWIdth = CurCursorWidth;
		DrawnCursorHeight = CurCursorHeight;
		CurCursorWidth = 0;
	}
}

//----- (th4) -------------------------------------------------------------
// Stage 2: pixel-accurate overlay clear under the cursor sprite. Replays the
// cursor CEL's run structure (same addressing and offset table as the
// CEL_Draw(..., offsetStart=0, offsetEnd=8) call that drew it) and zeroes
// AlphaSurface/SpriteLayerSurface exactly under the pixels the CEL writes.
// Transparent CEL cells are skip-runs, so the ghost stays visible around the
// cursor silhouette — the old bounding-rect clear removed the ghost over the
// whole cursor box and showed a hard square of foreground wall.
// (th4) Stage 3 (2026-09-05): the same replay also clears the colored-light
// tint under the cursor pixels (R_TINT_CLEAR) — the cursor is UI ink and must
// never inherit the world's winning light color. Idle-gated.
static void CursorClearAlphaOverlay(char* celBuf, uint frameNum, int drawWidth, int x, int y)
{
	const bool clearAlpha = g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface;
	const bool clearTint  = g_ARGBLighting && g_LightTintActive && LightTintSurface;
	if (!(clearAlpha || clearTint)) return;
	if (!WorkingSurface || !celBuf || !drawWidth) return;
	uint* header = (uint*)celBuf;
	uint frameCount = header[0];
	if (frameNum > frameCount) frameNum = frameCount;
	int frameOffset = header[frameNum];
	char* from = celBuf + frameOffset;
	int offVal = *(ushort*)(from + 0); // offsetStart=0, matching the cursor CEL_Draw call
	if (!offVal) return;               // same early-out as CEL_DrawTo
	int rleLen = header[frameNum + 1] - frameOffset - offVal; // offsetEnd==8 -> no second offset
	from += offVal;
	uchar* dst = WorkingSurface + YOffsetHashTable[y] + x;
	if (clearAlpha) RleDraw<R_OVERLAY_CLEAR | R_CLIP>(dst, from, rleLen, drawWidth, 0);
	if (clearTint)  RleDraw<R_TINT_CLEAR | R_CLIP>(dst, from, rleLen, drawWidth, 0);
}

//----- (0046A925) --------------------------------------------------------
void RenderCursor()
{
	if (!XinputAutoTargettingOff) {
		return;
	}

	Object& objectUnderCursor = Objects[Cur.ObjectIndex];
	if( Cur.GraphicsID == CM_0_INVISIBLE || !( ItemWidth && ItemHeight )){
		return;
	}
	// (rt) Determined the cursor's X
	int cursorX = CursorX - 1;
	if( CursorX >= 1 ){
		if( cursorX > ScreenWidth - 1 ){
			return;
		}
	}else{
		cursorX = 0;
	}
	// (rt) Determined the cursor's Y
	uint cursorY = CursorY - 1;
	if( CursorY >= 1 ){
		if( (int)cursorY > ScreenHeight - 1 ){
			return;
		}
	}else{
		cursorY = 0;
	}
	// (rt) Determined the cursor's right coordinate
	int cursorRight = ItemWidth + 1 + cursorX;
	if( cursorRight > ScreenWidth - 1 ){
		cursorRight = ScreenWidth - 1;
	}
	CurCursorStartY = cursorY;
	CurCursorStartX = cursorX & ~3;
	CurCursorWidth = (cursorRight | 3) - (cursorX & ~3) + 1;
	// (rt) Determined the cursor's bottom coordinate
	int cursorDown = cursorY + ItemHeight + 1;
	if( cursorDown > ScreenHeight - 1 ){
		cursorDown = ScreenHeight - 1;
	}
	uchar* cursorBuf = CursorImage;
	int curHeight = 1 - cursorY + cursorDown;
	CurCursorHeight = curHeight;
	uchar* screenCursorRect = &WorkingSurface[WorkingWidth * (cursorY + Screen_TopBorder) + Screen_LeftBorder] + (cursorX & ~3);
	int graphicsID;

	if( curHeight ){// (rt) If the height is non-zero, go through each pixel row and copy a line of pixels as wide as the image onto the cursor
		int hLeft = curHeight;
		do{
			memcpy(cursorBuf, screenCursorRect, CurCursorWidth);
			cursorBuf += CurCursorWidth;
			screenCursorRect += WorkingWidth;
			--hLeft;
		}while( hLeft );
	}
	graphicsID = Cur.GraphicsID;

	int xCenter = cursorX + 1;
	int yCenter = cursorY + 1;
	
	const Player& player = Players[ CurrentPlayerIndex ];
	
	if( graphicsID < CM_12_ITEMS_PICS_START ){
		int y = yCenter + ItemHeight + Screen_TopBorder - 1;
		int x = xCenter + Screen_LeftBorder;
		int spell = player.SpellOnCursor;
		bool red = 
			graphicsID == CM_7_TELEKINESIS &&
				((abs( player.Row - Cur.Row ) > 10 || abs( player.Col - Cur.Col ) > 10)
				|| (is(objectUnderCursor.BaseObjectIndex, BO_5_SMALL_CHEST, BO_6_CHEST, BO_7_LARGE_CHEST, BO_68_TRAPED_SMALL_CHEST, BO_69_TRAPED_CHEST, BO_70_TRAPED_LARGE_CHEST, BO_48_SARCOPHAGUS)
				&& ! CheckCastInSight(player.Row, player.Col, objectUnderCursor.Row, objectUnderCursor.Col)))
		 || graphicsID == CM_9_TARGET &&
				(CooldownCheck(CurrentPlayerIndex, spell)
				|| is(spell, PS_6_FIRE_WALL, PS_40_LIGHTING_WALL) && ! LocateWallSegments(player.Row, player.Col, Cur.Row, Cur.Col)
				|| spell >= MC_47_RUNE_OF_FIRE && spell <= MC_51_RUNE_OF_STONE && ! CheckCastInSight(player.Row, player.Col, Cur.Row, Cur.Col) && ! FoundCellForTargetInRadius(-1, &Cur.Row, &Cur.Col, 10));
		CEL_Draw(x, y, Invobj1_item_sprites, graphicsID, ItemWidth, 0, 8, (int)red | R_CLIP);
		// (th4) Stage 2: clear the overlay exactly under the cursor pixels just
		// drawn, so the ghost never composites over the hand and the wall around
		// it stays untouched (no square artifact).
		CursorClearAlphaOverlay(Invobj1_item_sprites, graphicsID, ItemWidth, x, y);

		#ifdef canned
        if( graphicsID == CM_1_NORMAL_HAND && Cur.MonsterID != -1 && Dungeon > DUN_0_TOWN ){
        	if( player.IsBowInHand ){
				int distance = CalculateArrowDistance( CurrentPlayerIndex, Cur.MonsterID );
				int baseMissileIndex = MI_0_NONE_ARROW; // todo choose effect depend on flags
				int hitChance = Calc_PvM_Ranged_Hit_Chance( CurrentPlayerIndex, Cur.MonsterID, distance, baseMissileIndex );
				if( hitChance < 0 ){
					strcpy( InfoPanelBuffer, "Can't hit" );
				}else{
					sprintf( InfoPanelBuffer, "%i%%", hitChance );
				}
				DrawText(xCenter, yCenter, xCenter + 20, InfoPanelBuffer, 0);
			}else{
				int hitChance = CalcPvM_MeleeHitChance( CurrentPlayerIndex, Cur.MonsterID, false );
				//int SweepAttackHitChance = -1;
				if( hitChance < 0 ){
					strcpy( InfoPanelBuffer, "Can't hit" );
				}else{
					// todo add swap attack hit chance
					sprintf( InfoPanelBuffer, "%i%%", hitChance );
				}
				DrawText(xCenter, yCenter, xCenter + 20, InfoPanelBuffer, 0);
			}
        }
		#endif
	}else{
		uint borderColor = 197;
		if( player.ItemOnCursor.MagicLevel ){
			borderColor = 181;
		}if( !player.ItemOnCursor.IsReqMet ){
			borderColor = 229;
		}
		const int overlayGfx = graphicsID;	// (th4) GetItemSprites() mutates it
		char* itemCELArray = GetItemSprites( graphicsID );
		int x = xCenter + Screen_LeftBorder;
		int y = yCenter + ItemHeight + Screen_TopBorder - 1;
		ItemCEL_Draw( overlayGfx, x, y, itemCELArray, graphicsID, ItemWidth, 0, 8, (int)!player.ItemOnCursor.IsReqMet | R_CLIP | player.ItemOnCursor.mirrorImage << 1, borderColor);
		// (th4) Stage 2: same pixel-accurate overlay clear, but only when the
		// 8-bit CEL was actually drawn — with replacement PNG art the icon is
		// drawn post-quad on top of the composite and needs no clearing.
		if (!ItemOverlay_HasReplacementArt(overlayGfx)) {
			CursorClearAlphaOverlay(itemCELArray, graphicsID, ItemWidth, x, y);
		}
		if( ItemOverlay_Queue(overlayGfx, x, y, ItemWidth, ItemHeight, player.ItemOnCursor.mirrorImage != 0, borderColor, (int)!player.ItemOnCursor.IsReqMet | R_CLIP | player.ItemOnCursor.mirrorImage << 1) ){	// (th4)
			ItemOverlay_MarkQueuedOnCursor();	// (th4) dragged: stays above the in-game menu
		}
		ItemOverlay_MarkCursorItem(overlayGfx);	// (th4) the cursor is this item; do not re-lift it
	}
}

//----- (0046AB96) --------------------------------------------------------
void __fastcall DrawGameplayToScreen()
{
	if( !IsMainWindowMinimized && (IsMainWindowActive || (BackgroundDraw && (!IsFullScreen || BigWin))) ){
		ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder );
	}
}

//----- (th2) -------------------------------------------------------------
int __fastcall GetTextWidth( const char* s )
{
	int l = 0;
	// (th4) ARABIC measures SHAPED glyphs with the proportional advance, matching
	// DrawLevelInfoText exactly. The GC window centres with -GetTextWidth(s)/2, so a
	// byte-wise width would offset every header and row by the difference.
	if( GameLanguage == LANG_AR ){
		static AR_Glyph g[ 512 ];
		const int gn = AR_Layout( s, g, 512 );
		for( int k = 0; k < gn; ++k ){
			if( g[k].isForm )   l += AR_Advance( g[k].cell, 0 );
			else if( g[k].raw ) l += FontWidthSmall[ FontIndexSmall[ Codepage[ g[k].raw ] ] ] + 1;
		}
		return l;
	}
	while( *s ){ // (th4) CHI: a GB2312 pair is one character, two bytes
		int lid, w;
		s += ChiNextSmall( (const uchar*)s, &lid, &w );
		l += w + 1;
	}
	return l;
}

//----- (th2) -------------------------------------------------------------
int __fastcall GetTextWidthBig( const char* s )
{
	// (th4) Arabic measures SHAPED glyphs -- lam+alef is two stored pairs and one
	// drawn glyph, so a byte-wise width is short by a whole cell per ligature.
	if( GameLanguage == LANG_AR ){
		static AR_Glyph g[ 512 ];
		const int gn = AR_Layout( s, g, 512 );
		int w = 0;
		for( int k = 0; k < gn; ++k ){
			w += g[k].isForm ? AR_Advance( g[k].cell, 2 )
			                 : ( FontWidthBigPtr[ FontIndexBigPtr[ Codepage[ g[k].raw ] ] ] + 1 );
		}
		return w;
	}
	int l = 0;
	while (*s) {
		if( IsChiLead( (uchar)s[0] ) && IsChiTrail( (uchar)s[1] ) ){ // (th4) GB2312 pair
			l += ( GameLanguage == LANG_HIN   // (th4) see HindiText.cpp
			     ? HIN_Advance( ChiLetterID( (uchar)s[0], (uchar)s[1] ) - HIN_FIRST_ID, 2 )
			     : CHI_CELL_W_BIG ) + 1;
			s += 2;
			continue;
		}
		l += FontWidthBigPtr[FontIndexBigPtr[Codepage[*s++]]] + 1;
	}
	return l;
}

//----- (th2) -------------------------------------------------------------
int __fastcall GetTextWidthMid( const char* s )
{
	// (th4) Arabic measures SHAPED glyphs -- lam+alef is two stored pairs and one
	// drawn glyph, so a byte-wise width is short by a whole cell per ligature.
	if( GameLanguage == LANG_AR ){
		static AR_Glyph g[ 512 ];
		const int gn = AR_Layout( s, g, 512 );
		int w = 0;
		for( int k = 0; k < gn; ++k ){
			w += g[k].isForm ? AR_Advance( g[k].cell, 1 )
			                 : ( FontWidthMidPtr[ FontIndexMidPtr[ Codepage[ g[k].raw ] ] ] + 1 );
		}
		return w;
	}
	int l = 0;
	while (*s) {
		if( IsChiLead( (uchar)s[0] ) && IsChiTrail( (uchar)s[1] ) ){ // (th4) GB2312 pair
			l += ( GameLanguage == LANG_HIN   // (th4) see HindiText.cpp
			     ? HIN_Advance( ChiLetterID( (uchar)s[0], (uchar)s[1] ) - HIN_FIRST_ID, 1 )
			     : CHI_CELL_W_MID ) + 1;
			s += 2;
			continue;
		}
		l += FontWidthMidPtr[FontIndexMidPtr[Codepage[*s++]]] + 1;
	}
	return l;
}

bool AltHighlight = false;

//----- (th2) -------------------------------------------------------------
void DrawSelectionDebug()
{
    int curX = CursorX;
	int curY = CursorY;

    for( int x = 0; x < ScreenWidth; ++x ){
        if(x % 8 >= 4){
            continue;
        }
        for( int y = 0; y < ScreenHeight; ++y ){
            if(y % 8 >= 4){
                continue;
            }
            CursorX = x;
            CursorY = y;
            DispatchCursorAboveIsoland();
            
            int color = -1;
            
            if( Cur.InventoryCellID != -1 ){
                color = 10;
            }else if( Cur.ItemID != -1 ){
                color = 20;
            }else if( Cur.ObjectIndex != -1 ){
                color = 1;
            }else if( Cur.playerIndex != -1 ){
                color = 40;
            }else if( Cur.MonsterID != -1 ){
                color = 50;
            }else if( IswarpUnderCursor ){
                color = 80;
            }
            
            if( color != -1 ){
                WorkingSurface[(Screen_TopBorder + y) * WorkingWidth + (Screen_LeftBorder + x)] = color;
            }
        }
    }
    
    CursorX = curX;
    CursorY = curY;
    DispatchCursorAboveIsoland();
}

void DrawTerminatorReadout()
{
	static const char* tokens[] = {
		"0x", "SEG", "REG", "IRQ", "MOV", "JMP", "XOR", "PTR",
		"MEM", "CRC", "ACK", "NAK", "SYS", "BUS", "DMA", "TTL",
		"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "ESP", "EBP",
		"VEC", "OPC", "FLG", "CLK", "ROM", "NOP", "RET", "CMP",
		"LEA", "INT", "DIV", "MUL", "SHL", "SHR", "AND", "NOT",
		"PSH", "POP", "CLI", "STI", "HLT", "WAIT", "LCK", "REP",
		"PIN", "ADR", "OFS", "CHK", "ECC", "FIFO", "TLB", "MMU",
		"CAL", "TST", "NEG", "INC", "DEC", "ROL", "ROR", "SBB",
		"ADC", "CWD", "CDQ", "BSF", "BSR", "BTC", "BTR", "BTS",
		"CPU", "ALU", "FPU", "GPU", "RAM", "VMM", "PCI", "AGP",
		"USB", "SPI", "I2C", "UART", "GPIO", "ADC", "DAC", "PWM",
		"NMI", "RST", "WDT", "PLL", "OSC", "VCC", "GND", "VDD",
		"ASM", "BIN", "HEX", "OCT", "DEC", "PAR", "ODD", "EVN",
		"SRC", "DST", "TMP", "ACC", "IDX", "CNT", "LEN", "POS",
		"MAP", "TBL", "ARR", "BUF", "QUE", "STK", "HSH", "LNK",
		"NODE", "LEAF", "ROOT", "EDGE", "GRPH", "TREE", "HEAP", "POOL",
		"SCAN", "SEEK", "LOCK", "SYNC", "GATE", "PORT", "BANK", "PAGE",
		"WORD", "DWRD", "QWRD", "NIBL", "BYTE", "MASK", "SIGN", "ZERO",
	};
	const int tokenCount = sizeof(tokens) / sizeof(tokens[0]);

	static const char* phrases[] = {
		"CPU LOAD: 38 PCT",
		"CLOCK SPEED: 37 GHZ",
		"CORE TEMP: 41 C",
		"THERMAL THROTTLE: OFF",
		"VOLTAGE RAIL: STABLE",
		"POWER CELL: 87 PCT",
		"BATTERY DRAIN: 0.3 PCT/M",
		"BACKUP CORE: STANDBY",
		"CACHE HIT RATE: 94 PCT",
		"MEMORY FREE: 64 TB",
		"STACK DEPTH: NOMINAL",
		"PARITY CHECK: PASS",
		"ECC ERRORS: 0",
		"BUS LATENCY: 4 MS",
		"INTERRUPT VEC: VALID",
		"WATCHDOG: RESET",
		"KERNEL FAULTS: 0",
		"FIRMWARE REV: 2.9.1",
		"DIAGNOSTIC: 14 OF 14",
		"BOOT SEQUENCE OK",
		"CHASSIS INTEGRITY: 96 PCT",
		"ARMOR PLATING: 96 PCT",
		"ENDOSKELETON: INTACT",
		"SERVO TORQUE: NOMINAL",
		"HYDRAULIC PSI: OK",
		"LIMB ACTUATORS: SYNCED",
		"GYRO BALANCE: LOCKED",
		"GRIP STRENGTH: 540 KG",
		"REFLEX TIMING: 8 MS",
		"COOLANT FLOW: NOMINAL",
		"COOLANT TEMP: 39 C",
		"FAN RPM: 3200",
		"OPTICAL ARRAY: ALIGNED",
		"LIDAR SWEEP: 360 DEG",
		"RADAR LOCK: STABLE",
		"SENSOR FUSION: OK",
		"NIGHT VISION: ENGAGED",
		"ZOOM FACTOR: 12X",
		"FRAME RATE: 240 FPS",
		"SIGNAL STRENGTH: 78 PCT",
		"UPLINK LATENCY: 4 MS",
		"EM SHIELD: NOMINAL",
		"PLL LOCK: ACQUIRED",
		"OSC DRIFT: 2 PPM",
		"DMA CHANNEL: IDLE",
		"FIFO DEPTH: 32",
		"TLB FLUSH: DONE",
		"MMU MAP: VALID",
		"REGISTER FILE: CLEAN",
		"PIPELINE STALL: 0",
		"BRANCH PREDICT: 97 PCT",
		"FPU STATUS: READY",
		"ALU QUEUE: EMPTY",
		"GPU TEMP: 44 C",
		"VRM LOAD: 22 PCT",
		"PSU OUTPUT: 12.0 V",
		"GROUND FAULT: NONE",
		"CAP CHARGE: FULL",
		"RELAY STATE: CLOSED",
		"ACTUATOR LAG: 1 MS",
		"JOINT WEAR: 3 PCT",
		"TORSO SERVO: NOMINAL",
		"NECK GIMBAL: CENTERED",
		"HAND TORQUE: NOMINAL",
		"FOOT SENSOR: CONTACT",
		"SPINE ALIGN: TRUE",
		"HEAT SINK: 48 C",
		"AIRFLOW: UNOBSTRUCTED",
		"DUST FILTER: 12 PCT",
		"SELF TEST: PASS",
		"ERROR LOG: EMPTY",
		"UPTIME: 04:12:33",
		"PROC REV: T-800",
		"MICROCODE: LOADED",
		"INSTR CACHE: WARM",
		"DATA CACHE: WARM",
		"PREFETCH: ACTIVE",
		"SCHEDULER: BALANCED",
		"THREAD COUNT: 16",
		"IDLE CYCLES: LOW",
		"POWER MODE: COMBAT",
	};
	const int phraseCount = sizeof(phrases) / sizeof(phrases[0]);
	static const char hexd[] = "0123456789ABCDEF";

	const int visible = 12;
	const int lineH = 11;
	const int x = ScreenWidth - 200;
	int y = ScreenHeight - (visible * lineH) + 6;

	int scroll = (GetTickCountTh() / 2400) % 4096;

	for (int i = 0; i < visible; i++) {
		char buf[40];
		int  p = 0;

		unsigned seed = (unsigned)(scroll + i);
		seed ^= seed << 13; seed *= 2654435761u;
		seed ^= seed >> 17; seed *= 2246822519u;
		seed ^= seed << 5;

		unsigned pick = seed >> 27;

		if ((pick % 5u) == 0u) {
			const char* ph = phrases[(seed >> 11) % phraseCount];
			while (*ph && p < 27) buf[p++] = *ph++;   // cap phrase to hex width
			buf[p] = 0;
			DrawText(x, y, -1, buf, C_0_White);
			y += lineH;
			continue;
		}

		if (pick & 1u) {
			const char* t = tokens[(seed >> 7) % tokenCount];
			while (*t && p < 24) buf[p++] = *t++;
			buf[p++] = ' ';
		}

		while (p < 27) {   // was 31 -> narrower again
			for (int k = 0; k < 4; k++) {
				seed = seed * 1103515245u + 12345u;
				buf[p++] = hexd[(seed >> 24) & 0xF];
			}
			buf[p++] = ' ';
		}
		buf[p] = 0;

		DrawText(x, y, -1, buf, C_0_White);
		y += lineH;
	}
}

bool InfoTip = true;
bool IsInterface = true;
float AspectX = 1.0, AspectY = 1.0;
//----- (0046AF66) --------------------------------------------------------
//----- (th4) -------------------------------------------------------------
// Stage 2 post-UI overlay clear (§4.7): after MainDrawFunc has drawn every
// end-of-frame UI element into WorkingSurface, zero AlphaSurface and
// SpriteLayerSurface over UI regions so the in-shader overlay composite can
// never draw sprite RGB on top of UI pixels.  UI drawing writes no WOB bits,
// so rect clearing is the only mechanism.  All rects are in post-zoom screen
// coordinates (the same space the WorkingSurface visible region uses).
static void ClearAlphaOverlayScreenRect(int x0, int y0, int x1, int y1)
{
	LimitToRange(x0, 0, ScreenWidth);
	LimitToRange(x1, 0, ScreenWidth);
	LimitToRange(y0, 0, ScreenHeight);
	LimitToRange(y1, 0, ScreenHeight);
	if (x0 >= x1 || y0 >= y1) return;
	uchar* arow = AlphaSurface + WorkingWidth * (Screen_TopBorder + y0) + Screen_LeftBorder + x0;
	uint* srow = SpriteLayerSurface + WorkingWidth * (Screen_TopBorder + y0) + Screen_LeftBorder + x0;
	for (int y = y0; y < y1; ++y) {
		memset(arow, 0, x1 - x0);
		memset(srow, 0, (x1 - x0) * sizeof(uint));
		arow += WorkingWidth;
		srow += WorkingWidth;
	}
}

void ClearAlphaOverlayUnderUI()
{
	if (!(g_ARGBLighting && EnrichedLightUltraAlpha != 0)) return;
	if (!AlphaSurface || !SpriteLayerSurface) return;

	// Fixed panel band along the bottom (post-zoom). The main panel ART is
	// GUI_MainPanelArtHeight (128) px tall and GUI_Width (640) wide, CENTERED —
	// NOT the full screen width (same rect SetupPanelRect anchors to,
	// Panel.cpp:1201). The strip above the panel (globe overhang + version/
	// difficulty text) is NOT rect-cleared: the text clears the overlay per
	// glyph pixel (DrawLevelInfoText -> DrawLetter) and the globe wells get
	// their own clears below. Everything else in the strip keeps its
	// translucency (2026-09-04, Rev 2.35b).
	const int panelBandX = (ScreenWidth - GUI_Width) / 2;
	ClearAlphaOverlayScreenRect(panelBandX, ScreenHeight - GUI_MainPanelArtHeight,
		panelBandX + GUI_Width, ScreenHeight);

	// (th4) 2026-09-04: the life/mana orb wells extend ABOVE the panel top and
	// their glass is semi-transparent — the composite behind the glass must be
	// the clean playfield, not an occluded sprite (a ghost would show through
	// the glass). Cleared per pixel, not as a rect: the classic 8-bit glass
	// clears inside PutWithAlpha (Panel.cpp) and the animated RGBA orbs clear
	// with the current frame's alpha (AnimOrbs_ClearOverlayWells).
	AnimOrbs_ClearOverlayWells();

	// Cursor: no rect clear here (Rev 2.19). A bounding-rect clear replaced the
	// ghost under the whole cursor box with a hard square of foreground wall.
	// RenderCursor instead clears the overlay pixel-accurately, exactly under
	// the cursor CEL's written pixels (R_OVERLAY_CLEAR pass in RleDraw).

	// Tooltips drawn this frame (union recorded by Tooltip_Draw*).
	if (g_TooltipClearRect.Width > 0 && g_TooltipClearRect.Heigth > 0) {
		ClearAlphaOverlayScreenRect(g_TooltipClearRect.Left, g_TooltipClearRect.Top,
			g_TooltipClearRect.Right, g_TooltipClearRect.Down);
	}

	// ALT item-label text (HighlightItemsNameOnMap) needs NO rect clear: the
	// label ink (DrawBox rows + DrawLetterPixel glyphs) already zeroes the
	// overlay per pixel at draw time, the LightSurface pattern. A rect clear
	// here over-cleared a margin around each label (removed 2026-09-07).

	// Full-screen overlays (red death screen, Xinput guide) genuinely cover the
	// whole view — keep the whole-region clear for them alone.
	if (IsPlayerDead || IsXinputGuideVisible) {
		ClearAlphaOverlayScreenRect(0, 0, ScreenWidth, ScreenHeight);
		return;
	}

	// Side / centered panels: clear only each visible panel's own rect, so the
	// translucency stays alive in the open game view around them — the same
	// region-specific behaviour LightSurface gets from the per-draw clears in
	// DrawBox/DrawRect/DrawLetterPixel. Rects are screen coords (SetupPanelRect).
	if (IsINVPanelVisible)         ClearAlphaOverlayScreenRect(InventoryPanelRect.Left, InventoryPanelRect.Top, InventoryPanelRect.Right, InventoryPanelRect.Down);
	if (IsCHARPanelVisible)        ClearAlphaOverlayScreenRect(CharPanelRect.Left, CharPanelRect.Top, CharPanelRect.Right, CharPanelRect.Down);
	if (IsSpellBookVisible)        ClearAlphaOverlayScreenRect(SpellBookRect.Left, SpellBookRect.Top, SpellBookRect.Right, SpellBookRect.Down);
	if (IsQUESTPanelVisible)       ClearAlphaOverlayScreenRect(QuestPanelRect.Left, QuestPanelRect.Top, QuestPanelRect.Right, QuestPanelRect.Down);
	if (IsStashPanelVisible)       ClearAlphaOverlayScreenRect(VisualStashPanel.Left, VisualStashPanel.Top, VisualStashPanel.Right, VisualStashPanel.Down);
	if (IsVisualTradePanelVisible) ClearAlphaOverlayScreenRect(VisualTradePanel.Left, VisualTradePanel.Top, VisualTradePanel.Right, VisualTradePanel.Down);
	if (IsCraftPanelVisible)       ClearAlphaOverlayScreenRect(CraftPanel.Left, CraftPanel.Top, CraftPanel.Right, CraftPanel.Down);
	if (IsEnchantPanelVisible)     ClearAlphaOverlayScreenRect(EnchantPanel.Left, EnchantPanel.Top, EnchantPanel.Right, EnchantPanel.Down);
	if (IsQuenchPanelVisible)      ClearAlphaOverlayScreenRect(QuenchPanel.Left, QuenchPanel.Top, QuenchPanel.Right, QuenchPanel.Down);
	if (IsTransmutePanelVisible)   ClearAlphaOverlayScreenRect(TransmutePanel.Left, TransmutePanel.Top, TransmutePanel.Right, TransmutePanel.Down);
	if (IsPowerupPanelVisible)     ClearAlphaOverlayScreenRect(PowerupPanel.Left, PowerupPanel.Top, PowerupPanel.Right, PowerupPanel.Down);
	if (IsPerksPanelVisible)       ClearAlphaOverlayScreenRect(PerksPanelPos.Left, PerksPanelPos.Top, PerksPanelPos.Right, PerksPanelPos.Down);
	// (th4) 2026-09-05: the Game-Changer info window is NOT rect-cleared
	// anymore (frame art is not a rectangle — the rect over-cleared 4 thin
	// strips along the edges). Its ink clears the overlay per pixel at draw
	// time (IW_DrawRegularCel + InfoWindow_DrawRect), the LightSurface pattern.
	// (th4) Rev 2.33: the chat history is NOT rect-cleared anymore — its text
	// (DrawLetter) and border (ChatHistory_DrawBorder) clear the overlay per
	// pixel, and the semi-transparent background deliberately keeps the ghost
	// visible through it.
	if (IsGoldSplitPanelVisible)   ClearAlphaOverlayScreenRect(GoldSplitPanelRect.Left, GoldSplitPanelRect.Top, GoldSplitPanelRect.Right, GoldSplitPanelRect.Down);

	// Centered dialogs drawn in the classic GUI space (help): clear that
	// centered 640x480-area rect — the same rect the meta menu is drawn at.
	// Spell select (2026-09-04), the ESC/pause menu (Rev 2.32), towner
	// dialogs (Rev 2.33) and the speech panel (Rev 2.34) are NOT in this
	// list anymore — their pixels clear the overlay where they are drawn
	// (the LightSurface pattern), so the translucency stays alive in the
	// rest of the playfield while they are open. Speech: DrawDialogBox goes
	// through Surface_DrawCEL (Rev 2.32 pass), the glyphs through
	// DrawLetterInSpeech (Rev 2.34) — the box dither holes stay translucent.
	const bool anyCenteredDialog = IsHELPVisible;
	if (anyCenteredDialog) {
		const int gx = (ScreenWidth - GUI_Width) / 2;
		const int gy = (ScreenHeight - GUI_Height) / 2;
		ClearAlphaOverlayScreenRect(gx, gy, gx + GUI_Width, gy + GUI_Height);
	}
}

//----- (th4) -------------------------------------------------------------
// Stage 3 post-UI tint clear: white-fill LightTintSurface over one post-zoom
// screen rect (the Stage-3 analog of ClearAlphaOverlayScreenRect). OPAQUE UI
// (popup panels, tooltips, the fixed bottom panel band) gets a bounding-box
// clear; transparent-with-glyph UI (ESC menu, dialogs, chat, floating text) is
// NOT cleared here — those pixels are cleared per ink-pixel at draw time
// (TintClearPixel/Row and the R_TINT_CLEAR RleDraw pass), so the tint keeps
// showing through their transparent regions. The cursor is cleared
// pixel-accurately at draw time too (CursorClearAlphaOverlay). Deliberately
// NOT cleared: the chat-history background (semi-transparent, tint shows
// through), the spell-select list (per-ink via DrawSpellIcon), and the
// Game-Changer info window (per-ink via IW_DrawRegularCel/InfoWindow_DrawRect
// — its frame art is not a rectangle, a rect clear over-cleared 4 thin strips).
static void ClearTintScreenRect(int x0, int y0, int x1, int y1)
{
	LimitToRange(x0, 0, ScreenWidth);
	LimitToRange(x1, 0, ScreenWidth);
	LimitToRange(y0, 0, ScreenHeight);
	LimitToRange(y1, 0, ScreenHeight);
	if (x0 >= x1 || y0 >= y1) return;
	uint* trow = LightTintSurface + WorkingWidth * (Screen_TopBorder + y0) + Screen_LeftBorder + x0;
	for (int y = y0; y < y1; ++y) {
		for (int x = 0; x < x1 - x0; ++x) trow[x] = TINT_WHITE;
		trow += WorkingWidth;
	}
}

void ClearTintUnderUI()
{
	if (!(g_ARGBLighting && g_LightTintActive && LightTintSurface)) return;

	// Fixed panel band along the bottom (post-zoom) — the same rect
	// ClearAlphaOverlayUnderUI uses: GUI_MainPanelArtHeight (128) rows,
	// GUI_Width (640) wide, CENTERED. The strip above the band (globe overhang
	// + the version/difficulty text) is NOT rect-cleared: the text clears the
	// tint per glyph pixel (DrawLevelInfoText -> DrawLetter), the classic 8-bit
	// orb glass per pixel (PutWithAlpha), and the animated orb wells per pixel
	// below (AnimOrbs_ClearTintWells). Everything else in the strip keeps its
	// tint.
	const int panelBandX = (ScreenWidth - GUI_Width) / 2;
	ClearTintScreenRect(panelBandX, ScreenHeight - GUI_MainPanelArtHeight,
		panelBandX + GUI_Width, ScreenHeight);
	AnimOrbs_ClearTintWells();

	// Tooltips drawn this frame (opaque boxes).
	if (g_TooltipClearRect.Width > 0 && g_TooltipClearRect.Heigth > 0) {
		ClearTintScreenRect(g_TooltipClearRect.Left, g_TooltipClearRect.Top,
			g_TooltipClearRect.Right, g_TooltipClearRect.Down);
	}

	// Full-screen overlays (red death screen, Xinput guide) genuinely cover the
	// whole view — keep the whole-region clear for them alone.
	if (IsPlayerDead || IsXinputGuideVisible) {
		ClearTintScreenRect(0, 0, ScreenWidth, ScreenHeight);
		return;
	}

	// Opaque popup panels: clear each visible panel's own rect (the same list
	// ClearAlphaOverlayUnderUI uses). The playfield around them keeps its tint.
	if (IsINVPanelVisible)         ClearTintScreenRect(InventoryPanelRect.Left, InventoryPanelRect.Top, InventoryPanelRect.Right, InventoryPanelRect.Down);
	if (IsCHARPanelVisible)        ClearTintScreenRect(CharPanelRect.Left, CharPanelRect.Top, CharPanelRect.Right, CharPanelRect.Down);
	if (IsSpellBookVisible)        ClearTintScreenRect(SpellBookRect.Left, SpellBookRect.Top, SpellBookRect.Right, SpellBookRect.Down);
	if (IsQUESTPanelVisible)       ClearTintScreenRect(QuestPanelRect.Left, QuestPanelRect.Top, QuestPanelRect.Right, QuestPanelRect.Down);
	if (IsStashPanelVisible)       ClearTintScreenRect(VisualStashPanel.Left, VisualStashPanel.Top, VisualStashPanel.Right, VisualStashPanel.Down);
	if (IsVisualTradePanelVisible) ClearTintScreenRect(VisualTradePanel.Left, VisualTradePanel.Top, VisualTradePanel.Right, VisualTradePanel.Down);
	if (IsCraftPanelVisible)       ClearTintScreenRect(CraftPanel.Left, CraftPanel.Top, CraftPanel.Right, CraftPanel.Down);
	if (IsEnchantPanelVisible)     ClearTintScreenRect(EnchantPanel.Left, EnchantPanel.Top, EnchantPanel.Right, EnchantPanel.Down);
	if (IsQuenchPanelVisible)      ClearTintScreenRect(QuenchPanel.Left, QuenchPanel.Top, QuenchPanel.Right, QuenchPanel.Down);
	if (IsTransmutePanelVisible)   ClearTintScreenRect(TransmutePanel.Left, TransmutePanel.Top, TransmutePanel.Right, TransmutePanel.Down);
	if (IsPowerupPanelVisible)     ClearTintScreenRect(PowerupPanel.Left, PowerupPanel.Top, PowerupPanel.Right, PowerupPanel.Down);
	if (IsPerksPanelVisible)       ClearTintScreenRect(PerksPanelPos.Left, PerksPanelPos.Top, PerksPanelPos.Right, PerksPanelPos.Down);
	// (th4) The Game-Changer info window is NOT rect-cleared: its frame art is
	// not a rectangle (the 4 corner decorations stick out), so a bounding-box
	// clear over-cleared 4 thin strips along the edges. Its ink clears the tint
	// per pixel at draw time (IW_DrawRegularCel + InfoWindow_DrawRect), the
	// LightSurface pattern.
	if (IsGoldSplitPanelVisible)   ClearTintScreenRect(GoldSplitPanelRect.Left, GoldSplitPanelRect.Top, GoldSplitPanelRect.Right, GoldSplitPanelRect.Down);

	// Centered help dialog drawn in the classic GUI space: clear that centered
	// 640x480-area rect (the same rect the alpha pass uses). The ESC/pause
	// menu, towner dialogs, speech panel, chat history and the spell list are
	// NOT in this list — their ink clears the tint per pixel at draw time.
	const bool anyCenteredDialog = IsHELPVisible;
	if (anyCenteredDialog) {
		const int gx = (ScreenWidth - GUI_Width) / 2;
		const int gy = (ScreenHeight - GUI_Height) / 2;
		ClearTintScreenRect(gx, gy, gx + GUI_Width, gy + GUI_Height);
	}
}

void MainDrawFunc() // DrawAndBlit
{
	AspectX = (float) WinWidth / ScreenWidth;
	AspectY = (float) WinHeight / ScreenHeight;
	if( gbRunGame ){
		DynZoom_Tick();	// (th4) DYNAMIC ZOOM: ease the factor toward the wheel target (DynamicZoom.cpp)
		LockSrcSurface();
		// draw gameplay
		{
			// TH4 "Miami 02": clear the gameplay viewport every frame. The dungeon pass
			// (DrawIsoLand) only paints opaque tiles and assumes it covers every pixel,
			// but transparent walls blend through 50% mask holes, and end-of-frame
			// overlays (cursor, menus, floating combat text) are never restored in the
			// gameplay loop — so those stale pixels survived in WorkingSurface and ghosted
			// through transparent-wall holes (smear), or showed where nothing was repainted.
			// Starting from black makes holes reveal black where nothing is behind, and real
			// geometry where there is (painter's order draws background first) — exactly the
			// buffer state the renderer always assumed. Clears WorkingSurface + LightSurface
			// over the visible area only (panel/globes are redrawn below).
			{
				uchar* row = WorkingSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
				for( int r = 0; r < ScreenHeight; ++r, row += WorkingWidth ) memset(row, 0, ScreenWidth);
				if( g_A8L8Lighting && LightSurface ){
					uchar* lrow = LightSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
					for( int r = 0; r < ScreenHeight; ++r, lrow += WorkingWidth ) memset(lrow, 0, ScreenWidth);
				}
				// (th4) Clear WallOcclusionSurface at frame start so stale values
				// from previous frames don't cause incorrect silhouette decisions.
				if (WallOcclusionSurface) {
					uchar* wrow = WallOcclusionSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
					for (int r = 0; r < ScreenHeight; ++r, wrow += WorkingWidth) memset(wrow, 0, ScreenWidth);
				}
				// (th4) Stage 2: clear the overlay surfaces over the visible region
				// too — stale pixels from previous frames must not composite (§4.7).
				if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface) {
					uchar* arow = AlphaSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
					uint* srow = SpriteLayerSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
					for (int r = 0; r < ScreenHeight; ++r) {
						memset(arow, 0, ScreenWidth);
						memset(srow, 0, ScreenWidth * sizeof(uint));
						arow += WorkingWidth;
						srow += WorkingWidth;
					}
				}
			}
			if( Dungeon->genType == DT_0_TOWN && !Dungeon->isQuest ){
			    DarkLevel = 0; TransparentMode = 0; // (rt) if greater than zero, the tiles are transparent
			}
			// (th4) Stage 2: rebuild the shared palette->RGB table once per frame
			// before DrawIsoLand — the post-render replay writes rgbLUT[] into
			// SpriteLayerSurface, and the ultra upload reuses the same table.
			if (g_ARGBLighting && EnrichedLightUltraAlpha != 0) {
				BuildScreenPaletteRGBLUT(rgbLUT);
			}
			// (th4) Stage 3 idle gate: true when any NON-OFF light carries a
			// non-white tint this frame. False skips every Stage-3 per-frame
			// cost (tint pre-fill + stamp stores, cache tint snapshot/restore,
			// zoom upscale, owner-exclusion writes, s3 texture upload) — the
			// V2 fill and the shader output are then byte-identical to
			// pre-Stage-3. Computed from the same predicate the stamp builder
			// uses (V2BuildStamps skips off lights).
			g_LightTintActive = false;
			// (th4) Rev 2.27: gate the scan itself — non-white tints can only
			// exist in ultra mode (ApplyMissileLightTint is gated), so the
			// pre-branch path skips the loop entirely.
			if (g_ARGBLighting) {
				for (int i = 0; i < LightsCount && !g_LightTintActive; i++) {
					const int l = LightIndexes[i];
					if (l < 0 || l >= Lights_128_Count || LightList[l].off) continue;
					const LightTint& t = LightTintList[l];
					if (t.r != 255 || t.g != 255 || t.b != 255) g_LightTintActive = true;
				}
				// (th4) Rev 2.46 Option C: a colored STATIC bake also feeds the
				// idle gate (the tint ambient + s3 upload must run for it even
				// when no runtime light is tinted).
				if (g_StaticTintActive) g_LightTintActive = true;
			}
			DrawIsoLand( PlayerRowPos, PlayerColPos, Dungeon->genType || Dungeon->isQuest, !IsZoomDisable );
			TownWater_Render( IsZoomDisable ? 1 : 2 );	// (th4) town river mask + hero reflections, post-zoom, before the warp (TownWater.cpp)
			Snowfall_Render( IsZoomDisable ? 1 : 2 );	// (th4) Ice Age snow in town / Ravenholm, over the world and the reflections, under the UI (Snowfall.cpp)
			Weather_Render( IsZoomDisable ? 1 : 2 );	// (th4) town storm nights: lightning, thunder (Weather.cpp); the rain is Rain.hlsl
			DustMotes_Render( IsZoomDisable ? 1 : 2 );	// (th4) dust around torches / candles in church and catacombs (DustMotes.cpp)
			Embers_Render( IsZoomDisable ? 1 : 2 );	// (th4) sparks rising from cave lava (Embers.cpp)
			Flames_Render( IsZoomDisable ? 1 : 2 );	// (th4) sparks from torches / braziers, halo list for FlameGlow.hlsl (Flames.cpp); before the snapshot
			Fireflies_Render( IsZoomDisable ? 1 : 2 );	// (th4) fireflies over Tristram on clear nights (Fireflies.cpp)
			// (th4) DYNAMIC ZOOM: the Reflect count goes INTO the world while zoomed, so it grows with its icon (Player.cpp
			// DrawReflectCount); same dungeon-only rule as DrawCombatTextAbovePlayer below. Before the warp, like the icon.
			if( DynZoom_Active() && ( Dungeon->genType || Dungeon->isQuest ) ) DrawReflectCount();
			Perspective_WarpWorld();	// (th4) optional pseudo-perspective, world only, before any UI
			DynZoom_SnapshotWorld();	// (th4) DYNAMIC ZOOM: world-only copy, zoomed on the GPU at present (DynamicZoom.cpp); FIRST: HeatHaze_SnapshotWorld reuses it
			HeatHaze_SnapshotWorld();	// (th4) world-only frame for the heat haze mask (dx_utility.cpp)
			NpcMark_SnapshotWorld();	// (th4) NPC marks: where they are + their cover boxes, world only, before any UI (NpcMark.cpp)
			if( IsSecondWin ){
				extern bool OnlySecondWin; OnlySecondWin = true;
				ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder );
				OnlySecondWin = false;
			}

			// (th4) No Map game changer: the map must be OFF, not merely un-toggleable. AutomapMode
			// starts at 1 (automap.cpp ResetAutoMap) and is restored from the save (Saving.cpp
			// "AutomapMode = GetNextByte()"), while ToggleAutomap refuses to change it under this GC
			// -- so a map that was on stayed on for good. Forcing 0 here, once per drawn frame,
			// also stops the map from taking over the arrow / zoom keys (Main.cpp "else if(
			// AutomapMode )" branches).
			if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_19_NO_MAP) ) AutomapMode = 0;
			if( AutomapMode ) DrawAutoMap();

			if( (IsAltPressed || AltHighlight) && !ProhibitAltHighlight){
				HighlightItemsNameOnMap();
			}else{
				HighlightedItem.ItemID = -1;
			}

			if( Dungeon->genType || Dungeon->isQuest ){
				DrawFloatingHealthAndMana();
				DrawCombatTextAbovePlayer();
				if( CurMon != -1 && ShowMonsterHealthBar ){
					// DrawMonsterHealthBar(CurMon);
					if (GameMode != GM_CLASSIC) DrawMonsterStatusBar(CurMon);
				}
				DrawCursorHitChanceInfo();
			}
			if( CurrentDialogIndex && !Speech_IsPanelVisible ) DrawTownerDialog();

            DrawAllDamagedItemsPics();
            if( getPlayerPerksPointsLeft(PERK_TYPE_COMMON) > 0 && !IsPerksPanelVisible && !SelectCurSpellMode && !IsInfoWindowVisible ) DrawPerkUpButton();
            if( Players[ CurrentPlayerIndex ].AvailableLvlPoints && !IsCHARPanelVisible && !SelectCurSpellMode && !IsInfoWindowVisible) DrawLevelUpButton();
            DrawPlayerStatusMessages();
            if (IsChatHistoryVisible) ChatHistory_Draw();
            
            if( IsInterface ){
                // !!! Don't touch it until release version, it's critical for bug fixing from screenshots
               // #if comment
                if (ShowVersionOnScreen) {
                    char dest[64];
                    sprintf(dest, PRJ_NAME" %s %s", THE_HELL_VERSION_STRING, DifNameInGame()); // (th4) CEL font, not the menu atlas // disabled showing version on the center of the top part of the screen
                   // DrawLevelInfoText(ScreenWidth / 2 - 112, 14, dest, 3);// old
					DrawLevelInfoText(ScreenWidth / 2 - GetTextWidth(dest) / 2, ScreenHeight - 132, dest, 3, true); // this is drawn below now (because of new monster HP bar)
                }
                //#endif
                DrawInfoLeft();
                DrawInfoRight();
            }
            
			ItemOverlay_FrameComposed();   // (th4) the panels below are what this frame shows
			// (th4) HeatMask_AddUiRect: each panel's rectangle is UI for the world-only effects (dx_utility.cpp UI RECTS)
			if (IsINVPanelVisible) { DrawINVPanel(); HeatMask_AddUiRect( InventoryPanelRect ); }
			else if( IsSpellBookVisible ){ DrawSpellBook(); HeatMask_AddUiRect( SpellBookRect ); }

			if( IsCHARPanelVisible ){ CharPanel_Draw(); HeatMask_AddUiRect( CharPanelRect ); }
			else if( IsQUESTPanelVisible ){ QuestPanel_Draw(); HeatMask_AddUiRect( QuestPanelRect ); }
			else if( IsStashPanelVisible ){ StashPanel_Draw(); HeatMask_AddUiRect( VisualStashPanel ); }
			else if( IsVisualTradePanelVisible ){ VisualTrade_Draw(); HeatMask_AddUiRect( VisualTradePanel ); }
			else if( IsCraftPanelVisible ){ Craft_Draw(); HeatMask_AddUiRect( CraftPanel ); }
			else if( IsEnchantPanelVisible ){ Enchant_Draw(); HeatMask_AddUiRect( EnchantPanel ); }
			else if( IsQuenchPanelVisible ){ Quench_Draw(); HeatMask_AddUiRect( QuenchPanel ); }
			else if( IsTransmutePanelVisible ){ Transmute_Draw(); HeatMask_AddUiRect( TransmutePanel ); }
			else if (IsPowerupPanelVisible){ Powerups_Draw(); HeatMask_AddUiRect( PowerupPanel ); }

			if (IsPerksPanelVisible){ DrawPerksPanel(); HeatMask_AddUiRect( PerksPanelPos ); }
			if (IsInfoWindowVisible){ InfoWindow_Draw(); HeatMask_AddUiRect( InfoWindowRect ); }
			if (IsXinputGuideVisible) DrawXinputGuide();

			if( Speech_IsPanelVisible ) DrawSpeech();
			if( SelectCurSpellMode ) DrawSpellsPanel();
			if( IsGoldSplitPanelVisible ){
				// (th4) The panel is 8-bit art; queued RGBA item icons would otherwise draw
				// over it. Claim its rect BEFORE drawing: everything the panel paints counts
				// as ink and is re-drawn on top of the icons (same mechanism as tooltips and
				// the main menu). Factor 1.0f = pixels the panel leaves untouched keep their
				// normal look. Height 296 spans from the rect top to the CEL bottom row
				// (drawn at Top + 295 in SplitGoldPile); over-cover is free with ink keying.
				ItemOverlay_QueueDimRect(GoldSplitPanelRect.Left, GoldSplitPanelRect.Top,
					GoldSplitPanelRect.Width, 296, 1.0f);
				ItemOverlay_SnapshotDimRect();
				SplitGoldPile( HowMuchGoldYouWantToRemove );
				HeatMask_AddUiRect( GoldSplitPanelRect );	// (th4) UI RECTS
			}
			
			if( IsHELPVisible ) GameHelp();
			if (!IsXinputGuideVisible)
			DrawOnScreenMessage();
			if (IsPlayerDead) DrawRedScreen();
			
			if (GamePaused && !IsPlayerDead) Menu_ShowPause();
			DrawPopupScreen();
			DrawInfoPanel();
			DrawLifeGlobeTop();
			DrawManaGlobeTop();
		}
		if( IsInterface ){
			DrawMainPanel();
			{	// (th4) UI RECTS (dx_utility.cpp): its black parts never count as world. ONLY THE ART: MainPanelRect is
				// GUI_MainPanelHeight (144) tall, but DrawMainPanel copies 128 rows -- the top 16 rows of the rect are
				// world, and as UI they lost the night tint / mist (a bright band above the panel, Mordor 2026-10-05).
				DisplayObject art = MainPanelRect;
				art.Top += GUI_MainPanelHeight - 128; art.Heigth -= GUI_MainPanelHeight - 128;
				HeatMask_AddUiRect( art );
			}
			if (!TalkPanelMode) {
				DrawXpBar();
			}
			DrawLifeGlobeBottom();
			DrawManaGlobeBottom();
			DrawMainButtons();
			DrawBeltInventory();
			if( TalkPanelMode ){
			    DrawTalkPanel();
			}
			if (!IsPlayerDead
				&& (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR))
				&& Players[CurrentPlayerIndex].Infravision) {
				DrawRedScreen();        // Terminator vision: red tint while infravision active
				DrawTerminatorReadout(); // white scrolling gibberish, bottom-right
			}
		}
		if (!CurrentMenuWindowPtr && !GamePaused) {
			Tooltip_ResetLiftRects();   // (th4) clear last frame's lift rects before republishing
			Tooltip_Draw(); Tooltip_Draw_2(); Tooltip_Draw_3();
		}
		Menu_Draw();

		//XinputHandleMouseMove();

		#if TEST_RNG_64
		static int trueR = 0;
		for( int i = 0; i < 1024 * 1024 / 2; ++i ){
			uint r = GetRndSeed64();
			int y = (r >> 10) & 1023;
			int x = r & 1023;
			if( y == 0 && x == 1 ) ++trueR;
			WorkingSurface[Screen_LeftBorder + 30 + YOffsetHashTable[y + 220] + x] = 240;
		}
		#endif

		//if( IsInterface || ObjectUnderCursor.GraphicsID >= CM_12_ITEMS_PICS_START ){
			RenderCursor();
		//}
		
		//DrawSelectionDebug();
		
		UnlockSrcSurface();
		ClearAlphaOverlayUnderUI();
		ClearTintUnderUI();
		DrawGameplayToScreen();
	}
}

