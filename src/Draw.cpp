#include "stdafx.h"

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
	if( mask == 0xFFFFFFFF ){
		if( palTable == (uchar*)1 ){ //  memset
			memset(dst, 0, size);
		}else if( palTable ){				//  memxlat
			for( int i = 0; i < size; i++ ){
				((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
			}
		}else{							// memcpy
			memcpy(dst, src, size);
		}
	}else{
		if( palTable == (uchar*)1 ){ //  memmaskset
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = 0;
				}
			}
		}else if( palTable ){				//  transparent memxlat
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)palTable)[((uchar*)src)[i]];
				}
			}
		}else{							//  memmask
			for( int i = 0; i < size; i++, mask <<= 1 ){
				if( mask & 0x80000000 ){
					((uchar*)dst)[i] = ((uchar*)src)[i];
				}
			}
		}
	}
}

// пробы оптимизации, эффект слабый, может ассемблерная вставка поможет, но возмжно на CPU это оптимизировать смысла нет
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
			i += 127;
			j += 127;
		}
		else if (src[i] == -128) j += 128;
		else if (src[i] >= 0) {
			for (k = 0; k < src[i]; ++k) dst[j+k] = trn[(uint8_t)src[i+k+1]];
			j += src[i];
			i += src[i];
		}
		else if (src[i] < 0) j += -src[i];
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall DrawBoxTransparent(int xPosition, int yPosition, int width, int height)
{
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
			}
		}
	}
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
		for (int i = 0; i < height; ++i)
			memset( &WorkingSurface[YOffsetHashTable[posY + y + i] + x + posX],
                    color + abs((i - halfheight) * (i - halfheight) / halfheight / gradpxl),
                    width );
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
	
	// Draw HP scale
	constexpr int hpPosX = -128;
	constexpr int hpPosY = 6;
	MB_DrawHP(hpPosX, hpPosY, lifeScale, 31, 234); // draw LifeScale
	MB_DrawHP(hpPosX + lifeScale - acidScale, hpPosY, acidScale, 31, 202); // draw AcidScale
	DrawBoxTransparent(x - Screen_LeftBorder + hpPosX + lifeScale, y - Screen_TopBorder + hpPosY, 256 - lifeScale, 31); // draw BackGround

	MB_DrawImg(-25, 30, 50, 50, Monster_Bar_MonIcons, monster.BasePtr->MonsterClass); // draw monster type icon - undead, deamon, beast

	// Immunes states (icons)
	constexpr uint16_t immunes[] = { MR_5_IM_FIRE, MR_6_IM_LIGHT, MR_11_IM_COLD, MR_4_IM_ARCAN, MR_8_IM_ACID, MR_7_IM_CURSE,  };
	constexpr int16_t immuIconSlotPosition[] = { -(20 + 32), -(20 + 32 * 2), -(20 + 32 * 3), 20, 20 + 32, 20 + 32 * 2 };
	constexpr uint8_t regroup[countof(immunes)] = { 3, 1, 0, 5, 4, 2 };
	uint8_t iconSlotsLeft = 0;
	uint8_t iconSlotsRight = 3;
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

	char textBuff[128];
	char textInfo[256];

	// Monster name + status
	constexpr uint8_t specialTable[] = { C_0_White, C_1_Blue, C_3_Gold, C_2_Red };
	sprintf_s(textInfo, "%s%s%s", monster.Name, monster.hisBossIndex ? " (minion)" : "",
		monster.ChampionNumber > 0 ? ChampionsNameTable[monster.ChampionNumber - 1] + 2 : "");
	MB_DrawText(-GetTextWidth(textInfo) / 2, 23, textInfo, specialTable[specialMonster]);

	// HP (text)
	sprintf_s(textBuff, "%d", monster.CurrentLife >> 6);
	int xShift = GetTextWidth(textBuff) + 3;
	sprintf_s(textInfo, "%s/%d", textBuff, monster.BaseLife >> 6);
	MB_DrawText(-xShift, 36, textInfo, monster.CurrentLife > monster.BaseLife ? C_2_Red : C_0_White);

	// Monster stun threshold
	if (ShowMonsterStunThreshold) {
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
	if (affixesAmount > 0) {
		int textWidth = GetTextWidth(textInfo) / 2;
		DrawBoxTransparent(x - Screen_LeftBorder - textWidth, y - Screen_TopBorder + 76 + textRowHeight[additionalTextRows], textWidth * 2, 12); // draw BackGround
		MB_DrawText(-textWidth, 90 + textRowHeight[additionalTextRows], textInfo, C_2_Red);
		++additionalTextRows;
	}

	// Monster auras
	const int& currentMonsterBaseIndex = monster.SpritePtr->baseMonsterIndex;
	bool breakflag = false;
	for (ushort auraIndex = AUI_1_; auraIndex < AUI_COUNT && !breakflag; ++auraIndex)
		for (auto&& auraBaseMonsterIndex : Auras[auraIndex].baseMonsterIndex) {
			if (currentMonsterBaseIndex == auraBaseMonsterIndex) {
				sprintf_s(textInfo, "%s", Auras[auraIndex].auraname);
				int textWidth = GetTextWidth(textInfo) / 2;
				DrawBoxTransparent(x - Screen_LeftBorder - textWidth, y - Screen_TopBorder + 76 + textRowHeight[additionalTextRows], textWidth*2, 12); // draw BackGround
				MB_DrawText(-textWidth, 90 + textRowHeight[additionalTextRows], textInfo, C_10_Enchanted);
				++additionalTextRows;
				breakflag = true;
				break;
			}
		}
}

// ---- th2 -------------------------------------------------------------------------------
unsigned char* __fastcall GetCurrentLightTable(int y, int x)
{
	uchar CurPalTabIndex = PerPixelLight[32 * y + x];

	if( CurPalTabIndex != 0 && CurPalTabIndex != LightMax )
		return &((uchar*)LightTable)[256 * CurPalTabIndex];
	else if( CurPalTabIndex == LightMax )
		return (unsigned char*)1;
	else
		return &((uchar*)LightTable)[256 * DarkLevel];
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall linecpyPerPixel(void* dst, void* src, int size, int h, unsigned mask, void* tbl)
{
	for( int i = 0; i < size; i++, mask <<= 1 ){
		tbl = GetCurrentLightTable(h, i);
		if( mask == 0xFFFFFFFF ){
			if( tbl == (unsigned char*)1 ){ //  memmaskset
				((unsigned char*)dst)[i] = 0;
			}
			else if( tbl ){				//  transparent memxlat
				((unsigned char*)dst)[i] = ((unsigned char*)tbl)[((unsigned char*)src)[i]];
			}else{							//  memmask
				((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
			}

		}else{
			if( tbl == (unsigned char*)1 ){ //  memmaskset
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = 0;
			}
			else if( tbl ){				//  transparent memxlat
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = ((unsigned char*)tbl)[((unsigned char*)src)[i]];
			}else{							//  memmask
				if( mask & 0x80000000 )
					((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
			}
		}
	}
}

// смотрим освещенность соседних тайлов на карте и интерполируем освещенность
// получаем PerPixel освещенность с минимальными изменениями в коде
// ---- th2 -------------------------------------------------------------------------------
void __fastcall CalculatePerPixelLight(int Row, int Col)
{
	int palCEL = LightMap[Row][Col];
	int palTop = LightMap[Row][Col - 1];
	int palBottom = LightMap[Row][Col + 1];
	int palLeft = LightMap[Row - 1][Col];
	int palRight = LightMap[Row + 1][Col];

	int x, y, a;

	// алгоритм уж как-то слишком уж лобовой цикл можно уполовинить
	// (palCEL-palRight)/32 вынести из циклов и т.д.
	for( a = 0; a <= 63; a++ ){
		for( x = 0; x <= a; x++ ){
			y = a - x;
			if( a < 32 ){
				PerPixelLight[y * 32 + x] = palRight + ((palCEL - palRight) * a) / 32;
			}else{
				PerPixelLight[y * 32 + x] = palCEL + ((palLeft - palCEL) * a) / 32;
			}
		}
	}

	for( a = -32; a <= 31; a++ ){
		for( x = 0; x <= 31 + a; x++ ){
			y = x - a;
			if( a < 0 ){
				PerPixelLight[y * 32 + x] += palTop + ((palCEL - palTop) * a) / 32;
			}else{
				PerPixelLight[y * 32 + x] += palCEL + ((palBottom - palCEL) * a) / 32;
			}
			PerPixelLight[y * 32 + x] = PerPixelLight[y * 32 + x] >> 1;
		}
	}
}

//----- (00719720) --------------------------------------------------------
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

// оптимизация DrawDunCelTrans, BigFuncWithCELFile и DrawTransparentCorner
//----- (th2) -------------------------------------------------------------
void PutTile(uchar* DstSurf, int tileType, char* celPtr, int PalIndex)
{
	int h, x, shift, skip;
	uchar* palTable = NULL;
	uint* mask = (uint*)&FullMask[31];
	if( DarkLevel != 0 && DarkLevel != LightMax && PalIndex){
		palTable = &((uchar*)LightTable)[256 * DarkLevel];
	}else if( DarkLevel == LightMax){
		palTable = (uchar*)1;
	}
	if( TransparentMode ){
		if( PartialTrans == TSP_0_NONE ){ // рисуем прозрачную стену
			DrawTransparentWall(0,0);
			mask = &WallTransMask[31];
		}
		else if( PartialTrans == TSP_1_LEFT && (WallsTransparency[WallIndex] & BIT(TSP_1_LEFT)) ){ // рисуем левую прозрачную трапецию
			DrawTransparentCorner(0,0);
			mask = (uint*)&LeftTransMask[31];
		}
		else if( PartialTrans == TSP_2_RIGHT && (WallsTransparency[WallIndex] & BIT(TSP_2_RIGHT)) ){ // рисуем правую прозрачную трапецию
			DrawTransparentCorner(0,0);
			mask = (uint*)&RightTransMask[31];
		}
	}
	switch (tileType ){
		case 0: //   рисуем непрозрачный квадрат 32х32
			for( h = 32; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		case 1: // рисуем полупрозрачный квадрат или треугольник
			int CELWidth;
			for( h = 32; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				for( x = 0; x < 32; ){
					CELWidth = *celPtr++;
					if( CELWidth >= 0 ){
						x += CELWidth;
						if( TransparentMode && PartialTrans == TSP_0_NONE)                // если это стена
							if( PixelLight )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, *mask, palTable);      //  в полупрозрачном формате уже учтена маска
							else
								linecpy(DstSurf, celPtr, CELWidth, *mask, palTable);      //  в полупрозрачном формате уже учтена маска
						else
							if( PixelLight )
								linecpyPerPixel(DstSurf, celPtr, CELWidth, h, 0xFFFFFFFF, palTable); //  в полупрозрачном формате уже учтена маска
							else
								linecpy(DstSurf, celPtr, CELWidth, 0xFFFFFFFF, palTable); //  в полупрозрачном формате уже учтена маска
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
		case 2: // рисуем левый непрозрачный треугольник
			for( shift = 2, h = 32, skip = 30; skip != 32; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, palTable);
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
		case 3: // рисуем правый непрозрачный треугольник
			for( shift = 0, h = 32, skip = 30; skip != 32; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, palTable);
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
		case 4: // рисуем левую непрозрачную трапецию
			for( shift = 2, h = 32, skip = 30; h > 16; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(&DstSurf[skip], &celPtr[shift], 32 - skip, h, *mask << skip, palTable);
				else
					linecpy(&DstSurf[skip], &celPtr[shift], 32 - skip, *mask << skip, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
		default: // рисуем правую непрозрачную трапецию
			for( shift = 0, h = 32, skip = 30; h > 16; h--, shift ^= 2, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(DstSurf, &celPtr[shift], 32 - skip, h, *mask, palTable);
				else
					linecpy(DstSurf, &celPtr[shift], 32 - skip, *mask, palTable);
				celPtr += 32 - (skip - shift);
				DstSurf -= WorkingWidth;
				skip -= 2;
			}
			for( ; h > 0; h--, mask-- ){
				if( DstSurf >= WorkingSurfaceDrawEnd || (size_t)DstSurf < (size_t)WorkingSurfaceDrawBegin ){ break; }
				if( PixelLight )
					linecpyPerPixel(DstSurf, celPtr, 32, h, *mask, palTable);
				else
					linecpy(DstSurf, celPtr, 32, *mask, palTable);
				celPtr += 32;
				DstSurf -= WorkingWidth;
			}
			break;
	}
}

// рисует тайлы в верхней части экрана ( которые не должны рисоваться выше WorkingSurfaceEnd)
//----- (00462ECD) --------------------------------------------------------
void __fastcall DrawDunCelTrans( uchar* surfacePtr ) // DrawMTileClipTop
{
	uint maxIndex = 0;
	char* celPtr = NULL;
	uint selector = 0; // tile type
	if( DarkLevel == 0 ){ // рисование с дефолтной палитрой т.е. при максимальной освещённости
		if( CurTileID & 0x8000 ){
			CurTileID = DungeonTilesOffset[16 * (CurTileID & 0xFFF)] + (ushort)(CurTileID & 0xF000);
		}
		celPtr = (char*)CurrentLevelCELFile + CurrentLevelCELFile[(ushort)CurTileID & 0x0FFF];
		maxIndex = CurTileID;
		LOBYTE_IDA(maxIndex) = BYTE1(maxIndex);
		selector = ((maxIndex >> 4) & 7) + 8;
		// падеж при входе в игру
		PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
	}else if( DarkLevel != LightMax ){ // рисование с палитрой затенения, в основном в подземелье
		if( CurTileID & 0x8000 ){
			celPtr = &DarkenDungeonTiles[DungeonTilesOffset[DarkLevel + 16 * (CurTileID & 0xFFF)]];
			selector = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, (selector & 0xFF) - 8, celPtr, 0);
		}else{
			char* CELPtr2 = (char*)CurrentLevelCELFile + CurrentLevelCELFile[CurTileID & 0xFFF];
			ushort Selector2 = (ushort)CurTileID >> 12;
			PutTile(surfacePtr, Selector2 & 0xFF, CELPtr2, DarkLevel);
		}
	}else{ // рисование с чёрной палитрой
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
	int result; // eax@1
	int v2;     // edx@1
	int v3;     // ebx@1
	uchar* v4;  // edi@1
	int v5;     // ecx@2
	uchar* v6;     // edi@2
	int v7;     // edx@6
	int v8;     // ebx@6
	int v9;     // ecx@7
	uchar* v10;    // edi@7
	v4 = dst;
	v2 = 30;
	v3 = 1;
	result = 0;
	while( 1 ){
		if( (size_t)v4 < (size_t)WorkingSurfaceDrawBegin ){ return; }
		v6 = v2 + v4;
		v5 = v3;
		do{
			*(uint*)v6 = 0;
			v6 += 4;
			--v5;
		}while( v5 );
		v4 = v2 + v6 - (WorkingWidth + Screen_LeftBorder);
		if( !v2 )
			break;
		v2 -= 2;
		++v3;
	}
	v7 = 2;
	v8 = 15;
	do{
		v10 = v7 + v4;
		v9 = v8;
		do{
			*(uint*)v10 = 0;
			v10 += 4;
			--v9;
		}while( v9 );
		v4 = v7 + v10 - (WorkingWidth + Screen_LeftBorder);
		--v8;
		v7 += 2;
	}while( v7 != 32 );
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
		if( mis.uniqTrans ){
			CL2_DrawDark(x, y, mis.curAnimationPtr, mis.FrameIndex, mis.animWidth, shiftY, tileInCellIndex, mis.uniqTrans + 3);
		}else{
			CL2_Draw(x, y, mis.curAnimationPtr, mis.FrameIndex, mis.animWidth, shiftY, tileInCellIndex, mis.flag & MSF_LIGHT);
		}
	}
}

//----- (00467331) --------------------------------------------------------
void __fastcall DrawMissileInTile(int row, int col, int playerOffsetX, int playerOffsetY, int shiftY, int tileInCellIndex, int mayBeIsExploding)
{
	MissileIndexT castNum = MissilesMap[ row ][ col ];
	if( castNum == 0 ) return; // в оригинале нет этой проверки, так как FlagMap и MissilesMap не должны расходиться
							   //castNum = -1;// Пока поставлено так, ибо пока он перебирает, всё рисуется нормально, как только перестаёт перебирать, начинает рисовать через раз.
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
void __fastcall DrawDiedPlayer(int row, int col, int arg0, int a4, int a9, int a10, int zero)
{
	int curAnimPtr;          // ecx@9
	int curAnimFrame;          // edx@10
	unsigned int curAnim; // ecx@11
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
			DrawPlayer(playerIndex, row, col, arg0 + player.xOfs - player.animWidth2, a4 + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, a9, a10);
		}
	}
}

//----- (0046775B) --------------------------------------------------------
void __fastcall DrawPlayer(int playerIndex, int row, int col, int rowOffset, int colOffset, char* currentAnimationPointer, int currentAnimationFrame, int a8, int a9, int screenCellRow)
{
	if( (FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR ||	Players[CurrentPlayerIndex].Infravision || !Dungeon->isQuest && Dungeon == DUN_0_TOWN )
	 && currentAnimationPointer && currentAnimationFrame >= 1 && *(int*)currentAnimationPointer <= 50 && currentAnimationFrame <= *(int*)currentAnimationPointer ){
		if( playerIndex == Cur.playerIndex ){
			CL2_DrawOutline(165, rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow);
		}
		if( playerIndex == CurrentPlayerIndex ){
			CL2_Draw(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow);
		}else if( !(FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR) || Players[CurrentPlayerIndex].Infravision && DarkLevel > 8 ){
			CL2_DrawDark(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow, 1);
		}else{
			int prevPal = DarkLevel;
			if( DarkLevel >= 5 ){
				DarkLevel -= 5;
			}else{
				DarkLevel = 0;
			}
			CL2_Draw(rowOffset, colOffset, currentAnimationPointer, currentAnimationFrame, a8, a9, screenCellRow, true);
			DarkLevel = prevPal;
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
				if( newScrollDir ){
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

// в городе принципиально не рисуются полупрозрачные тайлы и монстры
//----- (00467AE3) --------------------------------------------------------
void __fastcall DrawIsoLand( int playerRow, int playerCol, bool dun, bool zoom ) // SVGADrawView / T_SVGADrawView
{
	// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
	int zoomFactor = zoom ? 2 : 1;
	int row = playerRow - ScreenWidth / 64 / zoomFactor;  // сдвиг в верхний левый угол (карта на экране повернута по часовой стрелке)
	int col = playerCol - 1;
	row += HeightShift / zoomFactor; col += HeightShift / zoomFactor;
	int offsetX = Scroll.xOfs + Screen_LeftBorder;
	int offsetY = Scroll.yOfs + Screen_TopBorder + 15;
	int HorCellDrawCount = ScreenWidth / 64 / zoomFactor;
	int BottomHeight = dun ? 8 : 5; // 3 : 0 for zoom but no using in th2
	switch( Scroll.dir ){
	case 1:										offsetY -= 32; --row;	--col;	++BottomHeight;	break; // вверх
	case 2:	++HorCellDrawCount;					offsetY -= 32; --row;	--col;	++BottomHeight;	break; // вправо вверх
	case 3: ++HorCellDrawCount;																	break; // вправо
	case 4:	++HorCellDrawCount;													++BottomHeight;	break; // вправо вниз
	case 5:																		++BottomHeight;	break; // вниз
	case 6:	++HorCellDrawCount; offsetX -= 64;				   --row;	++col;	++BottomHeight;	break; // влево вниз
	case 7: ++HorCellDrawCount; offsetX -= 64;				   --row;	++col;					break; // влево
	case 8:	++HorCellDrawCount; offsetX -= 64;	offsetY -= 32; row -= 2;		++BottomHeight;	break; // влево вверх
	}
	++HorCellDrawCount; // хак неоригинальной отрисовки
	bool interpolation = false;
	PlayerShiftX = PlayerShiftY = 0;
	if( InterpolationFrame && !GamePaused && !IsPausedInMenu() ){ MotionInterpolationBegin( offsetX, offsetY ); interpolation = true; }
	auto drawScene = dun ? DrawSceneDungeonTop : DrawSceneTownTop;
	int screenRowEnd = ScreenHeight / 32 / zoomFactor + (dun ? 5 : 8); // хак отрисовки, оригинально < (dum ? 4 : 7) до спрайта игрока
	for( int screenRow = 0; screenRow < screenRowEnd; ++screenRow, offsetY += 32 ){
		drawScene(row, col++, offsetX, offsetY, HorCellDrawCount, screenRow, 0);				// чётные целлы
		drawScene(row++, col, offsetX - 32, offsetY + 16, HorCellDrawCount, screenRow, dun);	// нечётные целлы
	}
	if( interpolation ){ MotionInterpolationEnd(); }
	if( zoom ){ // растягиваем изображение в 2 раза
		uchar* SurfPtr = &WorkingSurface[ YOffsetHashTable[Screen_TopBorder] + Screen_LeftBorder];
		for( int h = ScreenHeight / 2; h >= 0; h-- ){
			for( int w = ScreenWidth / 2; w >= 0; w-- ){
				char val = SurfPtr[w + h * WorkingWidth];
				SurfPtr[(2 * w) + (2 * h + 1) * WorkingWidth] = val;
				SurfPtr[(2 * w + 1) + (2 * h) * WorkingWidth] = val;
				SurfPtr[(2 * w + 1) + (2 * h + 1) * WorkingWidth] = val;
				SurfPtr[(2 * w) + (2 * h) * WorkingWidth] = val;
			}
		}
	}
}

//----- (004696D3) --------------------------------------------------------
void __fastcall DrawSceneDungeonTop( int row, int col, int offsetX, int offsetY, int horCellDrawCount, int screenCellRow, int halfFlag  ) // DrawHTLX
{
	if( horCellDrawCount <= 0 ) return;
	// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
	TileColumn* tile = &TileMap[IsometricMapOffset(row, col)];
	int tileInCellIndex = std::min(2 * screenCellRow + 2, 8);
	do{
		if( In112(row, col) ){
			#if CHEATS
			bool black = row == PlayerNextRow && col == PlayerNextCol;
			bool brown = row == PlayerNextNextRow && col == PlayerNextNextCol;
			#endif
			DarkLevel = LightMap[ row ][ col ];
			if( PixelLight ) CalculatePerPixelLight(row, col);
			WallIndex = FineMap[ row ][ col ];
			if( WallIndex ){
				TransparentMode = CellTransparent[WallIndex] & TransList[TransMap[ row ][ col ]];
				PartialTrans = TSP_1_LEFT;
				int yOfs = YOffsetHashTable[offsetY];
				uchar* leftTileDst = &WorkingSurface[offsetX + yOfs];
				// рисуем левые треугольники и стены
				CurTileID = tile->Layer[0].Left;
				if( CurTileID )DrawDunCelTrans(leftTileDst);
				PartialTrans = TSP_2_RIGHT;
				// рисуем правые треугольники и стены
				CurTileID = tile->Layer[0].Right;
				if( CurTileID ) DrawDunCelTrans(leftTileDst + 32);
				#if CHEATS
				#if 0
				if( DevelopMode && black ) DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
				if( DevelopMode && brown ) DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY], 194);
				#endif
				#endif
				PartialTrans = TSP_0_NONE;
				int tileIndex = 1;
				if( (MaxTilesInCell >> 1) - 1 > 1 ){
					uchar* rightTileDst = leftTileDst + 32;
					ushort* nextTile = &tile->Layer[1].Left;
					do{
						rightTileDst -= WorkingWidth * 32;
						leftTileDst -= WorkingWidth * 32;
						if( screenCellRow >= tileIndex ){
							int leftTile = *(ushort*)nextTile;
							CurTileID = *(ushort*)nextTile;
							if( leftTile ){
								DrawDunCelTrans(leftTileDst); // левая стена
							}
							int rightTile = nextTile[1];
							CurTileID = nextTile[1];
							if( rightTile ){
								DrawDunCelTrans(rightTileDst); // правая стена
							}
						}
						++tileIndex;
						nextTile += 2;
					}while( tileIndex < (MaxTilesInCell >> 1) );// хак отрисовки, дожно быть еще - 1 );
				}
				DrawDynamicObjects(&WorkingSurface[offsetX + yOfs], row, col, screenCellRow, tileInCellIndex, offsetX, offsetY, 1);
			}else{
				DrawTransparency(&WorkingSurface[offsetX + YOffsetHashTable[offsetY]]);
			}
		}else{
			// эта функция очевидно зарисовывает места где отсутствует карта
			DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
		}
		#if 0
		if( DevelopMode ) DrawTileGrid(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
		#endif
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
						if( monsterIndex == CurMon || ( AlwaysHighlightSummons && monsterIndex < SummonMonsters_Count ) ) {
							uchar borderColor = monsterIndex < SummonMonsters_Count ? 165 : 233;
							CL2_DrawOutline(borderColor, x, y, monster->curAnimation->data, monster->CurFrame, monsterSprite->Pitch, 0, tilesInCell);
						}
						DrawMonster(row, col, x, y, monsterIndex, 0, tilesInCell);
						if( firstDraw && ! (monster->flag & MF_14_UNDER_ARCH) ){
							DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tilesInCell, playerOffsetX - 64, playerOffsetY);
						}
					}
				}
			}
		}
	}
}

// рисует все динамические обьекты: игроков, монстров, ящики, предметы на земле... tileInCellIndex
// ошибка: динамические объекты в одной клетке с игроком не отрисовываются, например town portal
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
	if( DarkLevel < LightMax ){
	    if( bloodNum > 0 && bloodNum <= 9){
	        int bloodIndex = bloodNum - 1;
	        DeadStruct& bb = blood[bloodIndex];
	        int offsetX = playerOffsetX - bb.deadWidth2;
			char* orientedPtr = bb.oriented[0].data;
			int data1 = bb.deadFrame;
			if( orientedPtr && data1 >= 1 && *(int*)orientedPtr <= 50 && data1 <= *(int*)orientedPtr ){
				if( bb.deadTrans ){
					CL2_DrawDark(offsetX, playerOffsetY, orientedPtr, data1, bb.deadWidth, 0, tileInCellIndex, bb.deadTrans);
				}else{
					CL2_Draw(offsetX, playerOffsetY, orientedPtr, data1, bb.deadWidth, 0, tileInCellIndex, true);
				}
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
				if( dd.deadTrans ){
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteLight( offsetX, playerOffsetY, (int) MonsterSprites[spriteIndex].animation[ANIM_4_DIE].oriented[orient], MonsterSprites[spriteIndex].animation[ANIM_4_DIE].frameCount - 1, MonsterSprites[spriteIndex].Pitch, 0, tileInCellIndex, 1 );

					CL2_DrawDark(offsetX, offsetY, orientedPtr, data1, dd.deadWidth, 0, tilesInCell, dd.deadTrans);
				}else{
					// trying to draw last die anim frame under corpse
					//DrawMovingSpriteDarken( offsetX, playerOffsetY,	(int) MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].oriented[ orient ],	MonsterSprites[ spriteIndex ].animation[ ANIM_4_DIE ].frameCount - 1, MonsterSprites[ spriteIndex ].Pitch, 0, tileInCellIndex );
					
					CL2_Draw(offsetX, offsetY, orientedPtr, data1, dd.deadWidth, 0, tilesInCell, true);
				}
			}
		}
		if( objectSprite ){
			DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 1, 0, tileInCellIndex);
		}
	}
	if( itemNum > 0 && itemNum <= MaxItems_255 ){ //	рисуем шмотки на земле
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
		int playerIndex = -1 - prevPlayerNum; // тут был баг с дубликатом при попадании выстрелами при горизонтальном движении
		if( (uint) playerIndex < PlayersMax_4 ){
			Player& player = Players[playerIndex];
			DrawPlayer(playerIndex, row, col - 1, playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
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
		DrawMonsterInTile(-MonsterMap[0][cell - 1] - 1, surfaceDest, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY, firstDraw, placeFlag);
	}
	if( placeFlag & CF_4_DEAD_PLAYER ){
		DrawDiedPlayer(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( playerNum > 0 ){
		int playerIndex = playerNum - 1;
		if( (uint) playerIndex < PlayersMax_4 ){
			Player& player = Players[playerIndex];
			DrawPlayer(playerIndex, row, col, playerOffsetX + player.xOfs - player.animWidth2, playerOffsetY + player.yOfs, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
			if( firstDraw && player.UnderArcType ){
				if( player.UnderArcType == 2 ){
					DrawMapCell(surfaceDest - WorkingWidth * 16 - 96, row - 2, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 96, playerOffsetY - 16);
				}
				DrawMapCell(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
			}
		}
	}
	if( MonsterMap[0][cell] > 0 ){
		DrawMonsterInTile(MonsterMap[0][cell] - 1, surfaceDest, row, col, screenCellRow, tileInCellIndex, playerOffsetX, playerOffsetY, firstDraw, placeFlag);
	}
	if( placeFlag & CF_1_SPELL_CAST ){ // тут ForbMap расходиться с MisMap, в результате падение на -1 индексе
		DrawMissileInTile(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( objectSprite && DarkLevel < LightMax ){ // объектов не может быть больше одного в клетке
		DrawObjectSprite(row, col, playerOffsetX, playerOffsetY, 0, 0, tileInCellIndex);
	}
	if( itemNum > 0 && itemNum <= MaxItems_255 ){
		Item* item = &Items[itemNum - 1];
		if( item->dropping && item->ItemCELFilePtr != 0 && item->FrameIndex >= 1 ){
			if( *(int*)item->ItemCELFilePtr >= item->FrameIndex && *(uint*)item->ItemCELFilePtr <= 50 ){ // ограничение на 50 кадров флипа ?
				int outlineColor = IsAltPressed || AltHighlight || itemNum - 1 == Cur.ItemID /*|| AutomapMode == 1*/ ? 181 : -1;
				DrawItemOnMap(playerOffsetX - item->subOffsetX, playerOffsetY, item->ItemCELFilePtr, item->FrameIndex, item->AnimWidth, 0, tileInCellIndex, outlineColor);
			}
		}
	}
	if( arch && Dungeon->genType ){
		TransparentMode = TransList[transparent];
		CEL_DrawShadow(surfaceDest, CurrentLevelSCELFile, arch, 64, 0, tileInCellIndex);
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

//----- (0046A1FB) --------------------------------------------------------
void __fastcall DrawMonster( int row, int col, int monsterX, int monsterY, unsigned int monsterIndex, int shiftY, int tileInCellIndex )
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite* monsterSprite = monster.SpritePtr;
	char* curAnimation = monster.curAnimation->data;
	if( !curAnimation ){
		return;
	}		
	int curFrame = monster.CurFrame;
	int maybeCountOfFrames = *(uint*)curAnimation;
	if( curFrame < 1 ){
		return;
	}
	if( maybeCountOfFrames > 70 || curFrame > maybeCountOfFrames ){
		return;
	}
	int pitch = monsterSprite->Pitch;
	if( !(FlagMap[ row ][ col ] & CF_64_VISIBLE_BY_CURSOR) ){
		const Player& player = Players[CurrentPlayerIndex]; 
		int light_radius = player.ClassID == PC_1_ARCHER ? 11 : 10;
		if (/*abs(monster.Row - Players[CurrentPlayerIndex].Row) < 9 && abs(monster.Col - Players[CurrentPlayerIndex].Col) < 9*/
			(CheckMonsterInInfravisionRange(row, col, light_radius) || monsterIndex < SummonMonsters_Count) && !TileBlockSight[FineMap[row][col]]) { // Infravision limit
			CL2_DrawDark(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, 1);
		}
		return;
	}
	int someFlag = 0; // monster light index 
	if( monster.newBossId ){
		someFlag = monster.uniqTrans + 4;
	}
	if( monster.CurAction == A_15_STONE ){
		someFlag = 2;
	}
	if( Players[CurrentPlayerIndex].Infravision && DarkLevel > 8 ){
		someFlag = 1;
	}
	if( someFlag ){
		CL2_DrawDark(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, someFlag);
		return;
	}
	CL2_Draw(monsterX, monsterY, curAnimation, curFrame, pitch, shiftY, tileInCellIndex, true);
}

//----- (0046A2F5) --------------------------------------------------------
void __fastcall DrawObjectSprite(int row, int col, int playerOffsetX, int playerOffsetY, int someFlag, int upHalfCell, int screenCellRow)
{
	int colPixelPos;
	int rowPixelPos;
	char objectNum = ObjectsMap[ row ][ col ];
	char objectIndex = abs(objectNum)-1;
	Object& object = Objects[objectIndex];//понятно
	if( objectNum <= 0 ){
		if( object.InAction != someFlag ){
			return;
		}
		screenCellRow = 8; // я смотрел почему 8 
		int rowDelta = object.Row - row;
		int colDelta = object.Col - col;
		rowPixelPos = playerOffsetX + 32 * rowDelta - object.OffsetX - 32 * colDelta;
		colPixelPos = playerOffsetY + 16 * (colDelta + rowDelta); // все локальные переменные надо писать с маленкой буквы , глобальные и имена фукнций с большой
		upHalfCell = 0; // так удобнее
	}else{
		if( object.InAction != someFlag ){
			return;
		}
		colPixelPos = playerOffsetY;
		rowPixelPos = playerOffsetX - object.OffsetX;
	}
	if( objectIndex >= 0 && objectIndex < MaxObjects_127 && object.CelFilePtr ){  // struct120Array127[63].field_14 = 165, должен быть указатель на int
		int maxFrameIndex = *(int*)object.CelFilePtr;// у какого то объекта указатель указывает битый в результате получаем крит при попытке доступа в защищенную область
		if( object.FrameIndex >= 1 && maxFrameIndex <= 50 && object.FrameIndex <= maxFrameIndex ){
			int outlineColor = ( (IsAltPressed || AltHighlight) && object.selectable ) || objectIndex == Cur.ObjectIndex /*|| IsChest (objectIndex)*/ ? 194 : -1; // объект под курсором, рисуем подсветку
			if( object.field_C ){
				DrawItemOnMap(rowPixelPos, colPixelPos, object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow, outlineColor);
			}else if( object.CelFilePtr ){// лишняя проверка. уже выше проверялось
				CEL_Draw(rowPixelPos, colPixelPos, object.CelFilePtr, object.FrameIndex, object.Width, upHalfCell, screenCellRow, 0, outlineColor);
			}
		}
	}
}

//----- (0046A443) --------------------------------------------------------
void __fastcall DrawMapCell( uchar* dst, int row, int col, int curTileInCell, int a5, int a6, int a7 )
{
	if( ! In112(row, col) ) return;
	int v7;         // ebx@1
	int v8;         // edi@1
	TileColumn* v9; // esi@1
	int v10;        // esi@6
	int result;     // eax@13
	int v12;        // eax@1
	int v14;        // ecx@1
	int light;        // edx@1
	int v16;        // eax@1
	int v17;        // eax@1
	int v18;        // eax@1
	int v19;        // eax@1
	int v20;        // eax@3
	int v21;        // eax@8
	int v22;        // eax@10
	int v23;        // [sp+10h] [bp-10h]@1
	int v24;        // [sp+Ch] [bp-14h]@1
	uchar* v26;        // [sp+1Ch] [bp-4h]@1
	int tileInCell; // [sp+18h] [bp-8h]@5
	v23 = row;
	v24 = WallIndex;
	v12 = 112 * row + col;
	v7 = TransparentMode;
	v14 = FineMap[ 0 ][ v12 ];
	v8 = DarkLevel;
	light = LightMap[ 0 ][ v12 ];
	v16 = TransMap[ 0 ][ v12 ];
	WallIndex = v14;
	DarkLevel = light;
	v17 = CellTransparent[v14] & TransList[v16];
	v26 = dst;
	TransparentMode = v17;
	v18 = IsometricMapOffset(v23, col);
	PartialTrans = TSP_1_LEFT;
	v9 = &TileMap[v18];
	v19 = v9->Layer[0].Left;
	CurTileID = v9->Layer[0].Left;
	if( v19 )
		DrawDunCelTrans((uchar*)dst);
	PartialTrans = TSP_2_RIGHT;
	v20 = v9->Layer[0].Right;
	CurTileID = v9->Layer[0].Right;
	if( v20 )
		DrawDunCelTrans((uchar*)(dst + 32));
	PartialTrans = TSP_0_NONE;
	tileInCell = 1;
	if( (MaxTilesInCell >> 1) - 1 > 1 ){
		v10 = (int)&v9->Layer[1].Left;
		do{
			v26 -= WorkingWidth * 32;
			if( curTileInCell >= tileInCell ){
				v21 = *(ushort*)v10;
				CurTileID = *(ushort*)v10;
				if( v21 )
					DrawDunCelTrans(v26);
				v22 = *(ushort*)(v10 + 2);
				CurTileID = *(ushort*)(v10 + 2);
				if( v22 )
					DrawDunCelTrans(v26 + 32);
			}
			++tileInCell;
			v10 += 4;
		}while( tileInCell < (MaxTilesInCell >> 1) - 1 );
	}
	DrawDynamicObjects(dst, v23, col, curTileInCell, a5, a6, a7, 0);
	result = v24;
	DarkLevel = v8;
	TransparentMode = v7;
	WallIndex = v24;
}

//----- (0046A800) --------------------------------------------------------
void ClearScreen()
{
	int v0;   // edx@1
	void* v1; // edi@1
	LockSrcSurface();
	v1 = WorkingSurface + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	v0 = ScreenHeight;
	do{
		memset(v1, 0, ScreenWidth);
		v1 = (char*)v1 + WorkingWidth;
		--v0;
	}while( v0 );
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
	// Определили Х курсора
	int cursorX = CursorX - 1;
	if( CursorX >= 1 ){
		if( cursorX > ScreenWidth - 1 ){
			return;
		}
	}else{
		cursorX = 0;
	}
	// Определили Y курсора
	uint cursorY = CursorY - 1;
	if( CursorY >= 1 ){
		if( (int)cursorY > ScreenHeight - 1 ){
			return;
		}
	}else{
		cursorY = 0;
	}
	// Определили правую координату курсора
	int cursorRight = ItemWidth + 1 + cursorX;
	if( cursorRight > ScreenWidth - 1 ){
		cursorRight = ScreenWidth - 1;
	}
	CurCursorStartY = cursorY;
	CurCursorStartX = cursorX & ~3;
	CurCursorWidth = (cursorRight | 3) - (cursorX & ~3) + 1;
	// Определили нижнюю координату курсора
	int cursorDown = cursorY + ItemHeight + 1;
	if( cursorDown > ScreenHeight - 1 ){
		cursorDown = ScreenHeight - 1;
	}
	uchar* cursorBuf = CursorImage;
	int curHeight = 1 - cursorY + cursorDown;
	CurCursorHeight = curHeight;
	uchar* screenCursorRect = &WorkingSurface[WorkingWidth * (cursorY + Screen_TopBorder) + Screen_LeftBorder] + (cursorX & ~3);
	int graphicsID;

	if( curHeight ){// Если высота ненулевая проходим по каждому пикселю высоты и копируем на курсор имэдж линию пикселей равную ширине
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
		CEL_Draw(x, y, ClassicModItemSprites, graphicsID, ItemWidth, 0, 8, (int)red | R_CLIP);

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
		char* itemCELArray = GetItemSprites( graphicsID );
		int x = xCenter + Screen_LeftBorder;
		int y = yCenter + ItemHeight + Screen_TopBorder - 1;
		CEL_Draw(x, y, itemCELArray, graphicsID, ItemWidth, 0, 8, (int)!player.ItemOnCursor.IsReqMet | R_CLIP | player.ItemOnCursor.mirrorImage << 1, borderColor);
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
	while( *s ){
		l += FontWidthSmall[ FontIndexSmall[ Codepage[ *s++ ] ] ] + 1;
	}
	return l;
}

//----- (th2) -------------------------------------------------------------
int __fastcall GetTextWidthBig( const char* s )
{
	int l = 0;
	while (*s) {
		l += FontWidthBig[FontIndexBig[Codepage[*s++]]] + 1;
	}
	return l;
}

//----- (th2) -------------------------------------------------------------
int __fastcall GetTextWidthMid( const char* s )
{
	int l = 0;
	while (*s) {
		l += FontWidthMid[FontIndexMid[Codepage[*s++]]] + 1;
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


bool InfoTip = true;
bool IsInterface = true;
float AspectX = 1.0, AspectY = 1.0;
//----- (0046AF66) --------------------------------------------------------
void MainDrawFunc() // DrawAndBlit
{
	AspectX = (float) WinWidth / ScreenWidth;
	AspectY = (float) WinHeight / ScreenHeight;
	if( gbRunGame ){
		LockSrcSurface();
		// draw gameplay
		{
			if( Dungeon->genType == DT_0_TOWN && !Dungeon->isQuest ){
			    DarkLevel = 0; TransparentMode = 0; // если больше нуля то тайлы прозрачные
			}
			DrawIsoLand( PlayerRowPos, PlayerColPos, Dungeon->genType || Dungeon->isQuest, !IsZoomDisable );
			if( IsSecondWin ){
				extern bool OnlySecondWin; OnlySecondWin = true;
				ConvertPaletteSurfaceToTrueColor( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder );
				OnlySecondWin = false;
			}

			if( AutomapMode ) DrawAutoMap();

			if( (IsAltPressed || AltHighlight) && !ProhibitAltHighlight){
				HighlightItemsNameOnMap();
			}else{
				HighlightedItem.ItemID = -1;
			}

			if( Dungeon->genType || Dungeon->isQuest ){
				// DrawFloatingHealthAndMana(); // turning off for now...
				DrawCombatTextAbovePlayer();
				if( CurMon != -1 && ShowMonsterHealthBar ){
					// DrawMonsterHealthBar(CurMon);
					if (GameMode != GM_CLASSIC) DrawMonsterStatusBar(CurMon);
				}
			}
			if( CurrentDialogIndex && !Speech_IsPanelVisible ) DrawTownerDialog();

            DrawAllDamagedItemsPics();
            if( getPlayerPerksPointsLeft(PERK_TYPE_COMMON) > 0 && !IsPerksPanelVisible && !SelectCurSpellMode && !IsInfoWindowVisible ) DrawPerkUpButton();
            if( Players[ CurrentPlayerIndex ].AvailableLvlPoints && !IsCHARPanelVisible && !SelectCurSpellMode && !IsInfoWindowVisible) DrawLevelUpButton();
            DrawPlayerStatusMessages();
            
            if( IsInterface ){
                // !!! Don't touch it until release version, it's critical for bug fixing from screenshots
               // #if comment
                if (ShowVersionOnScreen) {
                    char dest[64];
                    sprintf(dest, PRJ_NAME" %s %s", THE_HELL_VERSION_STRING, DifName()); // disabled showing version on the center of the top part of the screen
                   // DrawLevelInfoText(ScreenWidth / 2 - 112, 14, dest, 3);// old
					DrawLevelInfoText(ScreenWidth / 2 - GetTextWidth(dest) / 2, ScreenHeight - 132, dest, 3, true); // this is drawn below now (because of new monster HP bar)
                }
                //#endif
                DrawInfoLeft();
                DrawInfoRight();
            }
            
			if (IsINVPanelVisible) { DrawINVPanel(); }
			else if( IsSpellBookVisible ) DrawSpellBook();

			if( IsCHARPanelVisible ) CharPanel_Draw();
			else if( IsQUESTPanelVisible ) QuestPanel_Draw();
			else if( IsStashPanelVisible ) StashPanel_Draw();
			else if( IsVisualTradePanelVisible ) VisualTrade_Draw();
			else if( IsCraftPanelVisible ) Craft_Draw();
			else if( IsEnchantPanelVisible ) Enchant_Draw();
			else if( IsQuenchPanelVisible ) Quench_Draw();
			else if( IsTransmutePanelVisible ) Transmute_Draw();

			if (IsPerksPanelVisible) DrawPerksPanel();
			if (IsInfoWindowVisible) InfoWindow_Draw();
			if (IsXinputGuideVisible) DrawXinputGuide();

			if( Speech_IsPanelVisible ) DrawSpeech();
			if( SelectCurSpellMode ) DrawSpellsPanel();
			if( IsGoldSplitPanelVisible ) SplitGoldPile( HowMuchGoldYouWantToRemove );
			
			if( IsHELPVisible ) GameHelp();
			if (!IsXinputGuideVisible)
			DrawOnScreenMessage();
			if( IsPlayerDead ) DrawRedScreen();
			if( GamePaused && !IsPlayerDead ) Menu_ShowPause();
			DrawPopupScreen();
			DrawInfoPanel();
			DrawLifeGlobeTop();
			DrawManaGlobeTop();
		}
		if( IsInterface ){
			DrawMainPanel();
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
		}
		if (!CurrentMenuWindowPtr && !GamePaused) {
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
		DrawGameplayToScreen();
	}
}

