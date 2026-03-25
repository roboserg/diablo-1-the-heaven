#include "stdafx.h"

Light LightList[32];

//----- (0042BC67) --------------------------------------------------------
void __fastcall RotateOffset(int& x, int& y, int& rx, int& ry, int& ox, int& oy, int& ax, int& ay)
{
	int tmp;
	ax = ay = 0;
	tmp = rx; rx = 7 - ry; ry = tmp; // rotate rx,ry by 90 deg. clockwise
	tmp = ox; ox = 7 - oy; oy = tmp; // rotate origin by 90 deg. clockwise
	x = rx - ox; y = ry - oy;        // set x,y to rx,ry shifted relative to origin
	if( x < 0 ){ x += 8; ax = 1; }   // bring them back into 0..7 range
	if( y < 0 ){ y += 8; ay = 1; }
}

//----- (0042BCDE) --------------------------------------------------------
void __fastcall DoLighting(int row, int col, int radius, int lightIndex)
{
	if( ! To112(row, col) ) return; // фикс порчи памяти (в th1 применено в 1.216)
	if( (uint) radius > 15 ) radius = 15;

	int xoff = 0, yoff = 0; // offset into tile, where tile is broken
	if( lightIndex >= 0 ){
		xoff = LightList[lightIndex].deltaRow;
		yoff = LightList[lightIndex].deltaCol;
		if( xoff < 0 ){ xoff += 8; row--; }
		if( yoff < 0 ){ yoff += 8; col--; }
	}
	// into 8 x 8 grid for increased distance resolution.
	int rxoff = xoff; // rotated offsets
	int ryoff = yoff;
	int orgx = 0, orgy = 0;
	int addx = 0, addy = 0;

	// clipping vars: offsets from row, col; absolute vals
	int maxleft  = row - 15 < 0           ?           1 + row : 15;
	int maxright = row + 15 > FineMap_112 ? FineMap_112 - row : 15;
	int maxup    = col - 15 < 0           ?           1 + col : 15;
	int maxdown  = col + 15 > FineMap_112 ? FineMap_112 - col : 15;
	
	if( Dungeon->graphType < DT_5_CRYPT ){
		LightMap[row][col] = 0;
	}else{
		if( LightMap[row][col] > LightLevels[radius][0] ){
			LightMap[row][col] = LightLevels[radius][0];
		}
	}
	int y, x;
	auto LightQuadrant = [&]( int max1, int max2, auto d1, auto d2 ){
		for( y = 0; y < max1; y++ ){
			for( x = 1; x < max2; x++ ){
				int dist = LightDist[xoff][yoff][x + addx][y + addy];
				if( dist < 128 ){
					int r = row + d1(); if( (uint)r >= FineMap_112 ) continue;
					int c = col + d2(); if( (uint)c >= FineMap_112 ) continue;
					int lightVal = LightLevels[radius][dist];
					if( lightVal < LightMap[r][c] ){
						LightMap[r][c] = lightVal;
					} 
				}
			}
		}
	};

	#define E(expr) [&]{return expr;}
	LightQuadrant( maxup,   maxright, E( x), E( y) );                 //   |x
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //   | 

	LightQuadrant( maxdown, maxright, E( y), E(-x) );                 //   | 
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //   |x

	LightQuadrant( maxdown,  maxleft, E(-x), E(-y) );                 //   | 
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //  x| 

	LightQuadrant( maxup,    maxleft, E(-y), E( x) );                 //  x| 
																	  //   | 
}

//----- (0042C05C) --------------------------------------------------------
void __fastcall DoUnLight( int row, int col, int radius )
{
	int colMin = col - (radius + 1);
	int colMax = radius + 1 + col;
	int rowMin = row - (radius + 1);
	int rowMax = radius + 1 + row;
	if( colMin < 0 ) colMin = 0;
	if( colMax > FineMap_112 ) colMax = FineMap_112;
	if( rowMin < 0 ) rowMin = 0;
	if( rowMax > FineMap_112 ) rowMax = FineMap_112;
	for( int c = colMin; c < colMax; ++c ){
		for( int r = rowMin; r < rowMax; ++r ){
			LightMap[ r ][ c ] = StaticLightMap[ r ][ c ];
		}
	}
}

//----- (0042C0C7) --------------------------------------------------------
void __fastcall DoUnVision(int row, int col, int lightRadius)
{
	int lightRadiusPlusOne = lightRadius + 1;
	int maxCol = col + lightRadiusPlusOne;
	int minCol = col - lightRadiusPlusOne;
	int minRow = row - lightRadiusPlusOne;
	int maxRow = row + lightRadiusPlusOne;
	
	LimitToMin(minRow, 0);
	LimitToMin(minCol, 0);
	LimitToMax(maxRow, FineMap_112);
	LimitToMax(maxCol, FineMap_112);

	for( int curRow = minRow; curRow < maxRow; curRow++ ){
		for( int curCol = minCol; curCol < maxCol; curCol++ ){
			FlagMap[ curRow ][ curCol ] &= ~(CF_2_VISIBLE_BY_PLAYER | CF_64_VISIBLE_BY_CURSOR);
		}
	}
}

//---- new ----------------------------------------------------------------
void FillIceDarkenPalette()
{
	if( Dungeon->isQuest && Dungeon == DUN_39_FROZEN_LOCH 
		// mor: this didnt work. i tried to make nonquest cave maps in ice age lose lava light
		/*|| (!Dungeon->isQuest && Dungeon->genType == DT_3_CAVE && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE))*/){
		//for( int l = 1; l < 15; ++l ){
		//	for( int i = 1; i < 32; ++i ){
		//		PaletteShadowTable[ l * 256 + i ] = i;
		//	}
		//}
		//return;
		LoadPalette( "levels\\l3data\\glacial03.pal" );
		PALETTEENTRY* filePalette = FilePalette;
		for( int l = 1; l < 15; ++l ){
			PALETTEENTRY p16 = filePalette[ 16 ];
			if( false /*EasyClick*/ ){
				//double k = log( (2.71828182845904523536 - 1.) * ((15.-l)/15.) + 1. );
				//double k = log(1.0 + 3.5*((15.-l)/15.)) / log(4.5);
				//double k = log(1.0 + 5.5*((15.-l)/15.)) / log(6.5);
				double k = log(1.0 + 8.0*((15.-l)/15.)) / log(9.0);
				p16.peRed   = (uchar)((double) p16.peRed   * k);
				p16.peGreen = (uchar)((double) p16.peGreen * k);
				p16.peBlue  = (uchar)((double) p16.peBlue  * k);
			}else{
				p16.peRed   = (int) p16.peRed * (15 - l) / 15;
				p16.peGreen = (int) p16.peGreen * (15 - l) / 15;
				p16.peBlue  = (int) p16.peBlue * (15 - l) / 15;
			}
			for( int i = 1; i < 32; ++i ){
				PALETTEENTRY p;
				if( i == 16 ){
					p = p16;
				} else{
					int peRed = (int) p16.peRed - ((int) filePalette[ 16 ].peRed - filePalette[ i ].peRed);
					int peGreen = (int) p16.peGreen - ((int) filePalette[ 16 ].peGreen - filePalette[ i ].peGreen);
					int peBlue = (int) p16.peBlue - ((int) filePalette[ 16 ].peBlue - filePalette[ i ].peBlue);
					p.peRed = peRed < 0 ? 0 : peRed;
					p.peGreen = peGreen < 0 ? 0 : peGreen;
					p.peBlue = peBlue < 0 ? 0 : peBlue;
				}
				int minIndex = 0;
				int dr = abs( (int) p.peRed - filePalette[ 0 ].peRed );
				int dg = abs( (int) p.peGreen - filePalette[ 0 ].peGreen );
				int db = abs( (int) p.peBlue - filePalette[ 0 ].peBlue );
				int minDif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
				for( int c = 1; c < 256; ++c ){
					int dr = abs( (int) p.peRed - filePalette[ c ].peRed );
					int dg = abs( (int) p.peGreen - filePalette[ c ].peGreen );
					int db = abs( (int) p.peBlue - filePalette[ c ].peBlue );
					int dif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
					if( dif < minDif ){
						minDif = dif;
						minIndex = c;
					}
				}
				LightTable[ l * 256 + i ] = minIndex;
			}
		}
	}
}

//----- (0042C3ED) --------------------------------------------------------
void MakeLightTable()
{
	uchar* tbl = LightTable; // Palettes for 16 light values
	int maxLightLevel = Light4Levels ? 3 : 15;

	for( int palIndex = 0, i = 0; i < maxLightLevel; i++, palIndex += Light4Levels ? 5 : 1 ){
		*tbl++ = 0;
		for( int j = 0; j < 8; j++ ){
			uchar col = 16 * j + palIndex;
			uchar max = 16 * j + 15;
			for( int k = 0; k < 16; k++ ){
				if( k != 0 || j != 0 ) *tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
			}
		}
		for( int j = 16; j < 20; j++ ){
			uchar col = 8 * j + (palIndex >> 1);
			uchar max = 8 * j + 7;
			for( int k = 0; k < 8; k++ ){
				*tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
			}
		}
		for( int j = 10; j < 16; j++ ){
			uchar col = 16 * j + palIndex;
			uchar max = 16 * j + 15;
			for( int k = 0; k < 16; k++ ){
				*tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
				if( col == 255 ){
					max = 0;
					col = 0;
				}
			}
		}
		
	}
	for( int i = 0; i < 256; i++ ) *tbl++ = 0;

	// Init fading for blood
	if( Dungeon->graphType == DT_4_HELL ){
		uchar bloodFadeHold[16];
		tbl = LightTable;
		for( int i = 0; i < maxLightLevel; i++ ){
			int l1 = maxLightLevel - i;
			int l2 = l1;
			int div = maxLightLevel / l1;
			int rem = maxLightLevel % l1;
			int cnt = 0;
			bloodFadeHold[0] = 0;
			uchar col = 1;
			for( int j = 1; j < 16; j++ ){
				bloodFadeHold[j] = col;
				l2 += rem;
				if( l2 > l1 && j < 15 ){
					j++;
					l2 -= l1;
					bloodFadeHold[j] = col;
				}
				cnt++;
				if( cnt == div ){
					col++;
					cnt = 0;
				}
			}
			*tbl++ = 0;
			for( int j =  1; j <= 15; j++ ) *tbl++ = bloodFadeHold[j];
			for( int j = 15; j >   0; j-- ) *tbl++ = bloodFadeHold[j];
			*tbl++ = 1;
			tbl += 224;
		}
		*tbl++ = 0;
		for( int j = 0; j < 31; j++ ){
			*tbl++ = 1;
		}
		tbl += 224;
	}

	if( Dungeon->graphType >= DT_5_CRYPT ){ // Make the lava fully bright ?
		tbl = LightTable;
		for( int i = 0; i < maxLightLevel; i++ ){
			*tbl++ = 0;
			for( int j = 1; j < 16; j++ ){ // 16 is the number of rotated colors. JKE
				*tbl++ = j;
			}
			tbl += 240;
		}
		*tbl++ = 0;
		for( int j = 1; j < 16; j++ ){
			*tbl++ = 1;
		}
		tbl += 240;
	}

	// Load infra vision table
	uchar* trn = (uchar*)LoadFile("PlrGFX\\Infra.TRN");
	for( int i = 0; i < 256; i++ ) *tbl++ = trn[i];
	FreeMem(trn);

	// Load stone curse table
	trn = (uchar*)LoadFile("PlrGFX\\Stone.TRN");
	for( int i = 0; i < 256; i++ ) *tbl++ = trn[i];
	FreeMem(trn);

	// Create pause table
	constexpr uchar pauseBandStart = 224; // Pause band starting color
	constexpr uchar pauseBandEnd = pauseBandStart + 15; // Pause band ending color
	for( int i = 0; i < 8; i++ ){
		for( uchar col = pauseBandStart + 2; col < pauseBandEnd; col++ ){
			if( i != 0 || col != pauseBandStart + 2 ){
				*tbl++ = col;
			}else{
				*tbl++ = 0;
			}
		}
		*tbl++ = 0;
		*tbl++ = 0;
		*tbl++ = 0;
	}
	for( int i = 0; i < 4; i++ ){
		uchar col = pauseBandStart;
		for( int j = pauseBandStart; j < pauseBandEnd; j += 2 ){
			*tbl++ = col;
			col += 2;
		}
	}
	for( int i = 0; i < 6; i++ ){
		for( uchar col = pauseBandStart; col < pauseBandEnd; col++ ){
			*tbl++ = col;
		}
		*tbl++ = 0;
	}
		
	if( OriginalLightLevels ){ // original light levels
		for( int rad = 0; rad < 16; ++rad ){ // Doron's new smooth light tables
			for( int d = 0; d < 128; ++d ){
				if( d <= 8 * (rad + 1) ){
					LightLevels[rad][d] = ftol(double(d) * 15.0 / (double(rad + 1) * 8.0) + 0.5);
				}else{
					LightLevels[rad][d] = 15;
				}
			}
		}
		if( Dungeon->graphType >= DT_5_CRYPT ){
			for( int rad = 0; rad < 16; rad++ ){
				double radiusMod = (sqrt((double)(16 - rad))) / 128.0;
				radiusMod *= radiusMod;
				for( int d = 0; d < 128; d++ ){ //here
					LightLevels[15 - rad][d] = 15 - (uchar)(radiusMod * (double)((128 - d) * (128 - d)));
					if( LightLevels[15 - rad][d] > 15 ){
						LightLevels[15 - rad][d] = 0;
					}
					LightLevels[15 - rad][d] = LightLevels[15 - rad][d] - (uchar)((15 - rad) / 2);
					if( LightLevels[15 - rad][d] > 15 ){
						LightLevels[15 - rad][d] = 0;
					}
				}
			}
		}
	}else{ // improved light levels
		const float maxDarkness = 15;
		const float maxBrightness = 0;
		for( unsigned radius = 0; radius < 16; radius++) {
			const unsigned maxDistance = (radius + 1) * 8;
			for (unsigned distance = 0; distance < 128; distance++) {
				if (distance > maxDistance) {
					LightLevels[radius][distance] = 15;
				} else {
					const float factor = static_cast<float>(distance) / static_cast<float>(maxDistance);
					float scaled;
					if( is(Dungeon->graphType, DT_5_CRYPT, DT_6_ABYSS) ){
						// quardratic falloff with over exposure
						const float brightness = static_cast<float>(radius) * 1.25F;
						scaled = factor * factor * brightness + (maxDarkness - brightness);
						scaled = std::max(maxBrightness, scaled);
					} else {
						// Leaner falloff
						scaled = factor * maxDarkness;
					}
					LightLevels[radius][distance] = static_cast<uint8_t>(scaled + 0.5F); // round up
				}
			}
		}

	}

	// Distance Table
	for( int xOfs = 0; xOfs < 8; xOfs++ ){
		for( int yOfs = 0; yOfs < 8; yOfs++ ){
			for( int y = 0; y < 16; y++ ){
				for( int x = 0; x < 16; x++ ){
					int a = x * 8 - yOfs;
					int b = y * 8 - xOfs;
					double ftmp = (uchar)sqrt((double)( a * a + b * b ));
					ftmp += ftmp < 0 ? -0.5 : 0.5; // round
					LightDist[xOfs][yOfs][x][y] = (uchar)ftmp;
				}
			}
		}
	}
	FillIceDarkenPalette();
}

//----- (0042C840) --------------------------------------------------------
void InitLightMax()
{
	LightMax = Light4Levels ? 3 : 15;
}

//----- (0042C856) --------------------------------------------------------
void InitLighting()
{
	LightsCount = 0;
	NeedLightRecalc = 0;
	FullLighting = 0;
	for( int i = 0; i < 32; i++ ){
		LightIndexes[i] = i;
	}
}

//----- (0042C874) --------------------------------------------------------
int __fastcall AddLight(int casterRow, int casterCol, int radius)
{
	int lightIndex = -1;
	if( !FullLighting && LightsCount < 32 ){
		lightIndex = LightIndexes[LightsCount];
		NeedLightRecalc = 1;
		++LightsCount;
		LightList[lightIndex].row = casterRow;
		LightList[lightIndex].col = casterCol;
		LightList[lightIndex].radius = radius;
		LightList[lightIndex].deltaRow = 0;
		LightList[lightIndex].deltaCol = 0;
		LightList[lightIndex].off = 0;
		LightList[lightIndex].unflag = 0;
	}
	return lightIndex;
}

//----- (0042C8E3) --------------------------------------------------------
int __fastcall LightOff(int lightIndex)
{
	int result = 0;
	if( !FullLighting && lightIndex != -1 ){
		result = 1;
		LightList[lightIndex].off = 1;
		NeedLightRecalc = 1;
	}
	return result;
}

//----- (0042C903) --------------------------------------------------------
void __fastcall ChangeLightRadius(int lightIndex, int radius)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		NeedLightRecalc = 1;
		light.prevRadius = light.radius;
		light.radius = radius;
	}
}

//----- (0042C956) --------------------------------------------------------
void __fastcall ChangeLightPos(int lightIndex, int row, int col)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		NeedLightRecalc = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		light.prevRadius = light.radius;
		light.row = row;
		light.col = col;
	}
}

// For increased lighting effect, we specify a position (-8..8,-8..8)
// within each dungeon grid coordinate.  This is deltaRow and deltaCol.
//----- (0042C9B5) --------------------------------------------------------
void __fastcall ChangeLightOffset(int lightIndex, int deltaRow, int deltaCol)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		light.prevRadius = light.radius;
		light.deltaRow = deltaRow;
		light.deltaCol = deltaCol;
		NeedLightRecalc = 1;
	}
}

//----- (0042CA10) --------------------------------------------------------
void __fastcall ChangeLight( int lightIndex, int row, int col, int radius )
{
	Light& light = LightList[lightIndex];
	if( FullLighting || lightIndex == -1 ){
		return;
	}
	light.unflag = 1;
	light.prevRow = light.row;
	light.prevCol = light.col;
	light.prevRadius = light.radius;
	light.row = row;
	light.col = col;
	light.radius = radius;
	NeedLightRecalc = 1;
}

//----- (0042CA7C) --------------------------------------------------------
void ProcessLightList()
{
	if( !FullLighting && NeedLightRecalc ){
		for( int i = 0; i < LightsCount; ++i ){
			int l = LightIndexes[ i ];
			if( LightList[ l ].off ){
				DoUnLight( LightList[ l ].row, LightList[ l ].col, LightList[ l ].radius );
			}
			if( LightList[ l ].unflag ){
				DoUnLight( LightList[ l ].prevRow, LightList[ l ].prevCol, LightList[ l ].prevRadius );
				LightList[ l ].unflag = 0;
			}
		}
		for( int l = 0; l < LightsCount; l++ ){ //  просчитываем освещение от двигающихся объектов в том числе героя
			Light& light = LightList[LightIndexes[l]];
			if( !light.off ){
				DoLighting(light.row, light.col, light.radius, LightIndexes[l]);
			}
		}
		int l = 0;
		while( l < LightsCount ){ // remove off light
			if( LightList[LightIndexes[l]].off ){
				--LightsCount;
				int removedLight = LightIndexes[LightsCount];
				LightIndexes[LightsCount] = LightIndexes[l];
				LightIndexes[l] = removedLight;
			}else{
				l++;
			}
		}
		NeedLightRecalc = 0;
	}
}

//----- (0042CB86) --------------------------------------------------------
void SavePreLighting()
{
	memcpy(StaticLightMap, LightMap, 112*112);
}

//----- (0042CB9E) --------------------------------------------------------
void InitVision()
{
	VisionCount = 0;
	IsDoVision = 0;
	VisionId = 1;
	for( int i = 0; i < TransValue; ++i ) TransList[i] = 0;
}

//----- (0042CBD9) --------------------------------------------------------
int __fastcall AddVision(unsigned int row, int col, int lightRadius, int isCurrentPlayer)
{
	int result;
	if( VisionCount >= 32 ){
		result = lightRadius;
	}else{
		IsDoVision = 1;
		Light& playerVisibility = VisionList[VisionCount];
		result = VisionId++;
		playerVisibility.row = row;
		playerVisibility.col = col;
		playerVisibility.radius = lightRadius;
		playerVisibility.id = result;
		playerVisibility.off = 0;
		playerVisibility.unflag = 0;
		playerVisibility.flag = !(!isCurrentPlayer);
		VisionCount++;
	}
	return result;
}

//----- (0042CC4B) --------------------------------------------------------
void __fastcall ChangeVisionRadius(unsigned int playerIdentificator, unsigned int lightRadius)
{
	for( int i = 0; i < VisionCount; i++ ){
		Light& playerVisibility = VisionList[i];
		if( playerVisibility.id == playerIdentificator ){ // 12
			playerVisibility.unflag = 1;// 20
			playerVisibility.prevRow = playerVisibility.row;// 28 - 0
			playerVisibility.prevCol = playerVisibility.col;// 32 - 4
			playerVisibility.prevRadius = playerVisibility.radius;// 36 - 8
			playerVisibility.radius = lightRadius;// 8
			IsDoVision = 1;
		}
	}
}

//----- (0042CC8C) --------------------------------------------------------
void __fastcall ChangeVision(uint id, uint row, uint col)
{
	for( int i = 0; i < VisionCount; i++ ){
		Light& playerVisibility = VisionList[i];
		if( playerVisibility.id == id ){ // 12
			playerVisibility.unflag = 1;// 20
			playerVisibility.prevRow = playerVisibility.row;// 28 - 0
			playerVisibility.prevCol = playerVisibility.col;// 32 - 4
			playerVisibility.prevRadius = playerVisibility.radius;// 36 - 8
			playerVisibility.row = row;
			playerVisibility.col = col;
			IsDoVision = 1;
		}
	}
}

//----- (0042CCD6) --------------------------------------------------------
void ProcessVisionList()
{
	int loIndex;           // ebp@1
	int hiIndex;           // edx@2
	//int v2;           // edi@2
	//int* v3;          // esi@3
	//int v4;           // ecx@10
	//char* v5;         // edi@10
	//int v6;           // edi@13
	//int* v7;          // esi@14
	int someFlag;           // ecx@18
	const void* hiStruct;    // eax@19
	Light* loStruct;    // ebx@19
	//char v11;         // si@10
	//unsigned int v12; // ecx@10
	loIndex = 0;
	if( IsDoVision ){
		
		for( int i = 0; i < VisionCount; i++ ){
			Light& playerVisibility = VisionList[i];
			if( playerVisibility.off ){ // 16
				DoUnVision(playerVisibility.row, playerVisibility.col, playerVisibility.radius);
			}

			if( playerVisibility.unflag ){ // 20
				DoUnVision(playerVisibility.prevRow, playerVisibility.prevCol, playerVisibility.prevRadius);
				playerVisibility.unflag = 0;
			}
		}
		if( (char)TransValue > 0 ){// Совершенно непонятная конструкция. ППотом посмотрю на неё в действии
			memset(TransList, 0, TransValue);
		}
		for( int i = 0; i < VisionCount; i++ ){
			Light& playerVisibility = VisionList[i];
			if( !playerVisibility.off ){ // 16
				DoVision(playerVisibility.row, playerVisibility.col, playerVisibility.radius, playerVisibility.flag & 1, playerVisibility.flag & 1);// 0 4 8 48 48
			}
		}
		hiIndex = VisionCount;
		do{
			someFlag = 0;
			if( hiIndex > 0 ){
				loStruct = VisionList;
				hiStruct = &VisionList[hiIndex];
				do{
					if( loStruct->off ){
						hiIndex--;
						hiStruct = (char*)hiStruct - 52;
						if( hiIndex > 0 && loIndex != hiIndex ){
							memcpy(loStruct, hiStruct, 52);
						}
						someFlag = 1;
					}
					++loIndex;
					++loStruct;
				}while( loIndex < hiIndex );
				loIndex = 0;
			}
		}while( someFlag != 0 );
		VisionCount = hiIndex;
	}
	IsDoVision = 0;
}

//----- (0042CDD7) --------------------------------------------------------
void AnimateHellPalette() // BloodCycle()
{
	// Cycle the blood
	uchar* tbl = LightTable;
	int lightLevels = Light4Levels ? 4 : 16;
	for( int i = 0; i < lightLevels; i++ ){
		tbl++;
		uchar h = *tbl;
		for( int k = 0; k < 30; k++ ){
			*tbl = *(tbl+1);
			*tbl++;
		}
		*tbl = h;
		tbl += 225;
	}
}
