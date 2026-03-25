#include "stdafx.h"

short LevelAutomapData[512];// 004D03A0
int MapRowPos;// 004D07A0
int MapColPos;// 004D07A4
int AutomapMode;// 004D07A8
char AutomapZoomOffset[42];// 004D07B0
char AutoMap[ 40 ][ 40 ];// 004D07D0
int AutomapZoomFactor;// 004D0E10
int AutomapRow;// 004D0E14
int AutomapCol;// 004D0E18
int AutomapZoom32;// 004D0E1C
int AutomapZoom16;// 004D0E20
int AutomapZoom8;// 004D0E24
int AutomapZoom4;// 004D0E28
int AutomapZoom2;// 004D0E2C

unsigned int AutomapPointsCount = 0;
unsigned int AutomapLinePointsCount = 0;
line_vertex AutomapPoints[20000];
line_vertex AutomapLines[10000];

int AutomapRotated;
uchar AutomapCurrentDrawColor;
int Xofs, Yofs; // Player center on screen buf ( x + Screen_LeftBorder, y + ScreenTopEnd ) 

enum Automap_Param
{
	Default_MiniMapBigX = 382, // 318 + Screen_LeftBorder, but hack for registry option compatibility, fix after moving option to config ?
	Default_MiniMapBigY = 98, // TODO: check why it less then Screen_TopBorder
	Default_MiniMapSmallX = 222, // TODO: should be 158 + Screen_LeftBorder, but hack for registry option compatibility, fix after moving option to config ?
	Default_MiniMapSmallY = 130, // TODO: check why it less then Screen_TopBorder
	Default_AutomapTransparency = 191
};

int MiniMapBigX = Default_MiniMapBigX + 64;
int MiniMapBigY = Default_MiniMapBigY;
int MiniMapSmallX = Default_MiniMapSmallX + 64;
int MiniMapSmallY = Default_MiniMapSmallY;
uint AutomapTransparency = Default_AutomapTransparency;

void LoadMapConfig()
{
	bool loaded = SRegLoadValue( "Hellfire", "Minimap Big X",   0, &MiniMapBigX);
	MiniMapBigX += 64;
	if( ! loaded                                                            || MiniMapBigX   < 0 || MiniMapBigX   > ScreenWidth  ) MiniMapBigX   = Default_MiniMapBigX + 64;
	if( ! SRegLoadValue( "Hellfire", "Minimap Big Y",   0, &MiniMapBigY   ) || MiniMapBigY   < 0 || MiniMapBigY   > ScreenHeight ) MiniMapBigY   = Default_MiniMapBigY;

	loaded = SRegLoadValue( "Hellfire", "Minimap Small X", 0, &MiniMapSmallX);
	MiniMapSmallX += 64;
	if( ! loaded                                                            || MiniMapSmallX < 0 || MiniMapSmallX > ScreenWidth  ) MiniMapSmallX = Default_MiniMapSmallX + 64;
	if( ! SRegLoadValue( "Hellfire", "Minimap Small Y", 0, &MiniMapSmallY ) || MiniMapSmallY < 0 || MiniMapSmallY > ScreenHeight ) MiniMapSmallY = Default_MiniMapSmallY;

	if( !SRegLoadValue( "Hellfire", "Automap Transparency", 0, (int*)&AutomapTransparency ) ) AutomapTransparency = Default_AutomapTransparency;
}

void SaveMapConfig()
{
	SRegSaveValue( "Hellfire", "Minimap Big X",   0, MiniMapBigX - 64   );
	SRegSaveValue( "Hellfire", "Minimap Big Y",   0, MiniMapBigY );
	SRegSaveValue( "Hellfire", "Minimap Small X", 0, MiniMapSmallX - 64 );
	SRegSaveValue( "Hellfire", "Minimap Small Y", 0, MiniMapSmallY );
	SRegSaveValue( "Hellfire", "Automap Transparency", 0, AutomapTransparency );
}

//----- (00401D1D) --------------------------------------------------------
void ResetAutoMap()
{
	AutomapMode = 1;
	AutomapZoomFactor = 50;
	AutomapZoom32 = 32;
	AutomapZoom16 = 16;
	AutomapZoom8 = 8;
	AutomapZoom4 = 4;
	AutomapZoom2 = 2;
}

//----- (00401D61) --------------------------------------------------------
void InitDungeonAutomapData()
{
	int a2 = 10;
	int i = 1;
	do{
		int x = (a2 << 6) / 90;
		AutomapZoomOffset[ i ] = 2 * ( GUI_PanelWidth / x ) + 1;
		int modX = GUI_PanelWidth % x;
		if( modX ){
			AutomapZoomOffset[ i ] = 2 * ( GUI_PanelWidth / x ) + 2;
		}
		if( modX >= 32 * a2 / 100 ){
			++AutomapZoomOffset[ i ];
		}
		a2 += 5;
		++i;
	}while( i < 41 );
	memset(LevelAutomapData, 0, sizeof( LevelAutomapData ));
	char* levelAmpFile;
	switch( Dungeon->graphType ){
	case DT_1_CHURCH  : levelAmpFile = "Levels\\L1Data\\L1.AMP"; break;
	case DT_2_CATACOMB: levelAmpFile = "Levels\\L2Data\\L2.AMP"; break;
	case DT_3_CAVE    : levelAmpFile = "Levels\\L3Data\\L3.AMP"; break;
	case DT_4_HELL    : levelAmpFile = "Levels\\L4Data\\L4.AMP"; break;
	case DT_5_CRYPT   : levelAmpFile = "NLevels\\L5Data\\L5.AMP"; break;
	case DT_6_ABYSS   : levelAmpFile = "NLevels\\L6Data\\L6.AMP"; break;
	default           : return;
	}

	int size;
	char* amp = (char*)LoadFile(levelAmpFile, &size);
	for( int i = 0; i < size; i += 2 ){
		LevelAutomapData[i / 2 + 1] = amp[i] + (amp[i + 1] << 8); // аппаратно независимая загрузка word
	}
	FreeMem(amp);

	memset( AutoMap, 0, sizeof( AutoMap ) );
	for( int i = 0; i < 112*112; i++ ){
		FlagMap[ 0 ][ i ] &= ~CF_128_DISCOVERED;
	}
}

//----- (00401EC2) --------------------------------------------------------
void ShiftAutomapUp()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallY > 0 ) --MiniMapSmallY;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigY > 0 ) --MiniMapBigY;
	}else{
		--AutomapRow;
		--AutomapCol;
	}
}

//----- (00401ECF) --------------------------------------------------------
void ShiftAutomapDown()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallY < ScreenHeight ) ++MiniMapSmallY;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigY < ScreenHeight ) ++MiniMapBigY;
	}else{
		++AutomapRow;
		++AutomapCol;
	}
}

//----- (00401EDC) --------------------------------------------------------
void ShiftAutomapLeft()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallX > 0 ) --MiniMapSmallX;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapBigX > 0 ) --MiniMapBigX;
	}else{
		--AutomapRow;
		++AutomapCol;
	}
}

//----- (00401EE9) --------------------------------------------------------
void ShiftAutomapRight()
{
	if( AutomapZoomFactor == 15 ){
		if( MiniMapSmallX < ScreenWidth ) ++MiniMapSmallX;
	}else if( AutomapZoomFactor == 25 ){
		if( MiniMapSmallX < ScreenWidth ) ++MiniMapBigX;
	}else{
		++AutomapRow;
		--AutomapCol;
	}
}

//----- (th2) -------------------------------------------------------------
void SetAutoMapZoom()
{
	int automapZoom = AutomapZoomFactor == 15 ? (25 * 64) / 100 / 2: (AutomapZoomFactor * 64) / 100;
	AutomapZoom32 = automapZoom;
	AutomapZoom16 = automapZoom / 2;
	AutomapZoom8 = automapZoom / 4;
	AutomapZoom4 = automapZoom / 8;
	AutomapZoom2 = automapZoom / 16;
}

int MinimapZoom = 15;
//----- (00401EF6) --------------------------------------------------------
void ZoomAutomapPlus()
{
	if( IsShiftPressed && MapBlend && DrawMode ){
		AutomapTransparency += 2;
		if( AutomapTransparency > 255u ) AutomapTransparency = 255u;
		return;
	}
	if( AutomapMode == 1 ){
		if( AutomapZoomFactor < 200 ){
			AutomapZoomFactor += 5;
			SetAutoMapZoom();
		}
	}else{
		if( AutomapZoomFactor < 25 ){
			AutomapZoomFactor = 25;
			MinimapZoom = AutomapZoomFactor;
			SetAutoMapZoom();
		}
	}
}

//----- (00401F35) --------------------------------------------------------
void ZoomAutomapMinus()
{
	if( IsShiftPressed && MapBlend && DrawMode ){
		AutomapTransparency -= 2;
		if( AutomapTransparency < 64u ) AutomapTransparency = 64u;
		return;
	}
	if( AutomapMode == 1 ){
		if( AutomapZoomFactor > 50 ){
			AutomapZoomFactor -= 5;
			SetAutoMapZoom();
		}
	}else{
		if( AutomapZoomFactor > 15 ){
			AutomapZoomFactor = 15;
			MinimapZoom = AutomapZoomFactor;
			SetAutoMapZoom();
		}
	}
}

//----- (th2) -------------------------------------------------------------
void CleanAutoMapGraphics()
{
    AutomapPointsCount = 0;
	AutomapLinePointsCount = 0;
}

//----- (00401F72) --------------------------------------------------------
void DrawAutoMap()
{
	CleanAutoMapGraphics();
	
	bool needDisableAutoMap =
                CurrentDialogIndex
                || Speech_IsPanelVisible
                || IsINVPanelVisible
                || IsSpellBookVisible
                || IsCHARPanelVisible
                || IsQUESTPanelVisible
                || IsStashPanelVisible
                || IsVisualTradePanelVisible
                || IsCraftPanelVisible
				|| IsEnchantPanelVisible
				|| IsQuenchPanelVisible
                || IsPerksPanelVisible
				|| IsInfoWindowVisible
                || SelectCurSpellMode
                || IsGoldSplitPanelVisible
                || IsHELPVisible
                || OnScreenMessage::Head != OnScreenMessage::Tail || OnScreenMessage::NotMainDrawing
                || IsPlayerDead
                || GamePaused
                || CurrentMenuWindowPtr
                || IsPopupScreenOpen
				|| IsXinputGuideVisible;
	
	if( needDisableAutoMap ){
	    DrawLevelInfo();
	    return;
    }
	
	if( DevelopMode || Dungeon->genType || Dungeon->isQuest ){ // можно рисовать автокарту в городе, пока рисуется только стрелка
		//WorkingSurfaceDrawEnd = (WorkingSurface + WorkingHeight * WorkingWidth);/*512*/

		int playerMapRowOffset = (PlayerRowPos - 16) >> 1;// положение стрелки и карты относительно нас по умолчанию
		int playerMapColOffset = (PlayerColPos - 16) >> 1;

		int fullMapRowPos = AutomapRow + playerMapRowOffset;// предположительно AutomapXPos отвечает за смещение карты относительно игрока по х а v3 позиция игрока на карте
		int fullMapColPos = AutomapCol + playerMapColOffset;

		//MapRowPos = fullMapRowPos;// Присваивается значение, но нигде не считывается
		//MapColPos = fullMapColPos;// Опять значение присваивается, но не используется

		if( fullMapRowPos < 0 ){ //юго-восточная блокировка. Т.е. по x не должны выходить за максимальное значение
			AutomapRow = -playerMapRowOffset;// Если положение игрока плюс смещение отрицательное, делаем его положительным
		}else if( fullMapRowPos >= 40 ){ // северо-западная блокировка. Т.е. по x не должны выходить за 0 значение
			AutomapRow = 39 - playerMapRowOffset;
		}
		if( fullMapColPos < 0 ){ // северо-восточная блокировка. Т.е. по y не должны выходить за 0 значение
			AutomapCol = -playerMapColOffset;
		}else if( fullMapColPos >= 40 ){ //юго-западная блокировка. Т.е. по y не должны выходить за максимальное значение
			AutomapCol = 39 - playerMapColOffset;
		}

		fullMapRowPos = AutomapRow + playerMapRowOffset;
		fullMapColPos = AutomapCol + playerMapColOffset;
		//MapRowPos = fullMapRowPos;// Присваивается значение, но нигде не считывается
		//MapColPos = fullMapColPos;// Опять значение присваивается, но не используется
	
		int automapZoomOffset = AutomapZoomOffset[(AutomapZoomFactor /*-50*/) / 5];
		automapZoomOffset *= 2; // Сделано чтобы отображалась карта на весь экран, а не на часть. Увы это плохо скажется на быстродействии
		if( (Scroll.yOfs + PlayerShiftX) + (Scroll.xOfs + PlayerShiftY) ){// Отсуп связанный с перемещением игрока в текущий момент
			automapZoomOffset++;
		}

		int mapRowOnPlayer = fullMapRowPos - automapZoomOffset;// Переменная обозначающая какая координата по x карты в данный момент рисуется под игроком. 0 означает центр карты
		int mapColOnPlayer = fullMapColPos - 1; // Переменная обозначающая какая координата по y карты в данный момент рисуется под игроком. 0 означает край карты
		int landX = 0; // положение карты, а не стрелки героя
		int landY = 0;

		if( automapZoomOffset & 1 ){
			landX = Xofs - AutomapZoom32 * ((automapZoomOffset - 1) >> 1);
			landY = Yofs - AutomapZoom16 * ((automapZoomOffset + 1) >> 1);
		}else{
			landX = Xofs + AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom32;
			landY = Yofs - (automapZoomOffset >> 1) * AutomapZoom16 - AutomapZoom8;
		}

		if( PlayerRowPos & 1 ){
			landX -= AutomapZoom8;
			landY -= AutomapZoom4;
		}

		if( PlayerColPos & 1 ){
			landX += AutomapZoom8;
			landY -= AutomapZoom4;
		}
		int xCenter = (AutomapZoomFactor * (Scroll.xOfs + PlayerShiftY) / 100 >> 1) + landX;
		int yCenter = (AutomapZoomFactor * (Scroll.yOfs + PlayerShiftX) / 100 >> 1) + landY;

		if( ScreenWidth == GUI_Width ){
			if( IsINVPanelVisible || IsSpellBookVisible ){
				xCenter -= 160;
			}
			if( IsCHARPanelVisible || IsQUESTPanelVisible ){
				xCenter += 160;
			}
		}

		if( AutomapMode == 2 ){
			automapZoomOffset = AutomapZoomOffset[AutomapZoomFactor/5] * 2;
			xCenter = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom32;
			yCenter = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY + 2) + 28 * AutomapZoom16 - (automapZoomOffset >> 1) * AutomapZoom16 - AutomapZoom8;
			mapRowOnPlayer = 20 - automapZoomOffset;
			mapColOnPlayer = 20 - 1;
		}
		for( int j = 0; j < automapZoomOffset * 2; j++){
			int screenx = xCenter;
			for( int i = 0; i < automapZoomOffset; screenx += AutomapZoom32, i++ ){// Рисуем половину карты. В шахматном порядке
				int mapX = mapRowOnPlayer + i;
				int mapY = mapColOnPlayer - i;
				short staticObjectFlags = GetStaticObjectFlags(mapX, mapY, IsNotMapOpen); // 1 - только открытая карта, 0 - вся карта
				if( staticObjectFlags ){
					AutomapDrawStaticObject(screenx, yCenter, staticObjectFlags);
				}
			}
			mapColOnPlayer++; // За счёт этого смещения рисуются остальные клетки карты.
			screenx = xCenter - AutomapZoom16;
			for( int i = 0; i <= automapZoomOffset; screenx += AutomapZoom32, i++ ){
				int mapX = mapRowOnPlayer + i;
				int mapY = mapColOnPlayer - i;
				short staticObjectFlags = GetStaticObjectFlags(mapX, mapY, IsNotMapOpen); // 1 - только открытая карта, 0 - вся карта
				if( staticObjectFlags ){
					AutomapDrawStaticObject(screenx, yCenter + AutomapZoom8, staticObjectFlags);
				}
			}
			mapRowOnPlayer++;
			yCenter += AutomapZoom16;
		}
		if( Dungeon->genType || Dungeon->isQuest ){
			AutomapDrawPlayers();
		}
		if( Dungeon->genType || Dungeon->isQuest ){
		    for( int summonIndex = 0; summonIndex < SummonMonsters_Count; ++summonIndex ){
		        if( Monsters[summonIndex].Row != 1 && Monsters[summonIndex].Col != 0 && Monsters[summonIndex].CurrentLife > 64 ){
		            AutomapDrawOneMonster( summonIndex, 183 );
                }
            }
        }
		
		if( AutomapMode && MapItem ){
			AutomapDrawItems();
		}
		DrawLevelInfo();
		//AutomapZoomFactor = oldAutomapZoomFactor;
	}else{
		DrawLevelInfo();
	}
}

//----- (004021F6) --------------------------------------------------------
void __fastcall AutomapDrawStaticObject(int x, int y, int staticObjectFlags)
{
	const uchar color1 = 200;
	const uchar color2 = 144;
	if( staticObjectFlags & BIT(15) ){ // Пустая клетка, недоступная для нахождения. Обозначается точками
		AutomapDrawPoint1(x, y, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y - AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y - AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom8, y, color1);
		AutomapDrawPoint1(x + AutomapZoom8, y, color1);
		AutomapDrawPoint1(x, y - AutomapZoom4, color1);
		AutomapDrawPoint1(x, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x + AutomapZoom4 - AutomapZoom16, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom16 - AutomapZoom4, y + AutomapZoom2, color1);
		AutomapDrawPoint1(x - AutomapZoom8, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x + AutomapZoom8, y + AutomapZoom4, color1);
		AutomapDrawPoint1(x - AutomapZoom4, y + AutomapZoom8 - AutomapZoom2, color1);
		AutomapDrawPoint1(x + AutomapZoom4, y + AutomapZoom8 - AutomapZoom2, color1);
		AutomapDrawPoint1(x, y + AutomapZoom8, color1);
	}
	if( staticObjectFlags & BIT(16) ){// Рисуется лестница
		AutomapDrawLine(x - AutomapZoom4, y - AutomapZoom4 - AutomapZoom2, AutomapZoom8 + x + AutomapZoom4, y + AutomapZoom2, color2);
		AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x + AutomapZoom8, y + AutomapZoom4, color2);
		AutomapDrawLine(x - AutomapZoom8 - AutomapZoom4, y - AutomapZoom2, x + AutomapZoom4, AutomapZoom4 + y + AutomapZoom2, color2);
		AutomapDrawLine(x - AutomapZoom16, y, x, y + AutomapZoom8, color2);
	}
	bool isPobDiagWall = false;
	bool isMainDiagWall = false;
	bool isMainDiagCaveWall = false;
	bool isPobDiagCaveWall = false;
	switch( (staticObjectFlags & 0xF) - 1 ){// 1111 нижние полбайта
		case 0:
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			break;
		case 1:
		case 4:		isPobDiagWall = true;		break;
		case 2:
		case 5:		isMainDiagWall = true;		break;
		case 3:
			isPobDiagWall = true;
			isMainDiagWall = true;
			break;
		case 7:
			isPobDiagWall = true;
			isMainDiagCaveWall = true;
			break;
		case 8:
			isMainDiagWall = true;
			isPobDiagCaveWall = true;
			break;
		case 9:			isMainDiagCaveWall = true;		break;
		case 10:		isPobDiagCaveWall = true;		break;
		case 11:
			isMainDiagCaveWall = true;
			isPobDiagCaveWall = true;
			break;
		default:
			break;
	}
	if( isPobDiagWall ){// стена по побочной диагонали а также часть колонн
		if( staticObjectFlags & BIT(9) ){// Дверь для вхождения по главной диагонали
			AutomapDrawLine(x,					y - AutomapZoom8,	x - AutomapZoom4,					y - AutomapZoom8 + AutomapZoom2,	color1);
			AutomapDrawLine(x - AutomapZoom16,	y,					x - AutomapZoom16 + AutomapZoom4,	y - AutomapZoom2,					color1);
			AutomapDrawLine(x - AutomapZoom8,	y - AutomapZoom8,	x - AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y - AutomapZoom8,	x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x - AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x,									y - AutomapZoom4,					color2);
		}
		if( staticObjectFlags & BIT(13) ){ // рещетчатые стены по побочной диагонали
			AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom16, y, color1);
			staticObjectFlags |= BIT(11);
		}
		if( staticObjectFlags & BIT(11) ){ // Дверные проёмы без двери. Обозначаются квадратами. Решётчатые стены тоже так обозначаются.
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
		}
		if( !(staticObjectFlags & (BIT(9)|BIT(11)|BIT(13))) ){ // Собственно сама стена
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom16, y, color1);
		}
	}
	if( isMainDiagWall ){ // стена по главной диагонали а также часть колонн
		if( staticObjectFlags & BIT(10) ){ // Дверь для вхождения по побочной диагонали
			AutomapDrawLine(x,					y - AutomapZoom8,	x + AutomapZoom4,					y - AutomapZoom8 + AutomapZoom2,	color1);
			AutomapDrawLine(x + AutomapZoom16,	y,					x + AutomapZoom16 - AutomapZoom4,	y - AutomapZoom2,					color1);
			AutomapDrawLine(x + AutomapZoom8,	y - AutomapZoom8,	x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y - AutomapZoom8,	x + AutomapZoom16,					y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x,									y - AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x + AutomapZoom16,					y - AutomapZoom4,					color2);
		}
		if( staticObjectFlags & BIT(14) ){ // рещетчатые стены по главной диагонали
			AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom16, y, color1);
			staticObjectFlags |= BIT(12);
		}
		if( staticObjectFlags & BIT(12) ){ // Дверные проёмы без двери. Обозначаются квадратами. Решётчатые стены тоже так обозначаются.
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color1);
			AutomapDrawLine(x, y, x - AutomapZoom8 + AutomapZoom16, y - AutomapZoom4, color1);
		}
		if( !(staticObjectFlags & (BIT(10)|BIT(12)|BIT(14))) ){ // Собственно сама стена
			AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom16, y, color1);
		}
	}
	if( isMainDiagCaveWall ){
		if( staticObjectFlags & BIT(9) ){ // Дверной проём в пещерах для прохода в направлении побочной диагонали
			AutomapDrawLine(x,					y + AutomapZoom8,	x - AutomapZoom4,					y + AutomapZoom8 - AutomapZoom2,	color1);
			AutomapDrawLine(x - AutomapZoom16,	y,					x - AutomapZoom16 + AutomapZoom4,	y + AutomapZoom2,					color1);
			AutomapDrawLine(x - AutomapZoom8,	y + AutomapZoom8,	x - AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y + AutomapZoom8,	x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x - AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x - AutomapZoom8,	y,					x,									y + AutomapZoom4,					color2);
		}else{
			AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom16, y, color1); // стена главной диагонали в пещерах. При этом не каждая
		}
	}
	if( isPobDiagCaveWall ){
		if( staticObjectFlags & BIT(10) ){// Дверной проём в пещерах для прохода в направлении главной диагонали
			AutomapDrawLine(x,					y + AutomapZoom8,	x + AutomapZoom4,					y + AutomapZoom8 - AutomapZoom2,	color1);
			AutomapDrawLine(x + AutomapZoom16,	y,					x + AutomapZoom16 - AutomapZoom4,	y + AutomapZoom2,					color1);
			AutomapDrawLine(x + AutomapZoom8,	y + AutomapZoom8,	x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y + AutomapZoom8,	x + AutomapZoom16,					y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x,									y + AutomapZoom4,					color2);
			AutomapDrawLine(x + AutomapZoom8,	y,					x + AutomapZoom16,					y + AutomapZoom4,					color2);
		}else{
			AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom16, y, color1); // стена побочной диагонали в пещерах. При этом не каждая
		}
	}
}

//----- (TH4) -------------------------------------------------------------
void AutoMapIsometricCalc(int row, int col, int& x, int& y)
{
	if (AutomapMode == 2) {
		x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + AutomapZoom8 * (row - col);
		y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + AutomapZoom4 * (row + col) - AutomapZoom4;
	}else{
		row = row - 2 * AutomapRow - PlayerRowPos;
		col = col - 2 * AutomapCol - PlayerColPos;
		x = AutomapZoom8 * (row - col) + (AutomapZoomFactor * (Scroll.xOfs + PlayerShiftY) / 100 >> 1) + Xofs;
		y = AutomapZoom4 * (row + col) + (AutomapZoomFactor * (Scroll.yOfs + PlayerShiftX) / 100 >> 1) + Yofs - AutomapZoom4;
		if (ScreenWidth == GUI_Width) {
			if (IsINVPanelVisible || IsSpellBookVisible) {
				x -= 160;
			}
			if (IsCHARPanelVisible || IsQUESTPanelVisible) {
				x += 160;
			}
		}
	}
};

int ColorShifter = 0;
//----- (004029A7) --------------------------------------------------------
void AutomapDrawItems()
{
	//ColorShifter++;
	if (ColorShifter > 22)
		ColorShifter = 0;

	Player& player = Players[CurrentPlayerIndex];
	int yPos = 0;
	int xPos = 0;

	if( player.CurAction == PCA_3_WALK_HORISONTAL ){
		xPos = player.NextRow;
		yPos = player.NextCol;
		if( player.dir == 2 ){
			++xPos;
		}else{
			++yPos;
		}
	}else{
		xPos = player.Row;
		yPos = player.Col;
	}

	int xMin = xPos - 8;
	int yMin = yPos - 8;
	int xMax = xPos + 8;
	int yMax = yPos + 8;
	LimitToRange(xMin, 0, 112);
	LimitToRange(xMax, 0, 112);
	LimitToRange(yMin, 0, 112);
	LimitToRange(yMax, 0, 112);

	if( AutomapMode == 2 ){ if( Dungeon->genType || Dungeon->isQuest ){ xMin = 16; xMax = 96; yMin = 16; yMax = 96; }else{ xMin = 0; xMax = 112; yMin = 0; yMax = 112; } }
	for( int r = xMin; r < xMax; r++ ){
		for( int c = yMin; c < yMax; c++ ){

			int x = 0, y = 0;

			if( AutomapMode == 2 && IsNotMapOpen && (Dungeon->genType || Dungeon->isQuest) && !AutoMap[(r-16)/2][(c-16)/2] ) continue; // TODO: check and comment WTF is this
			if( int itemNum = ItemMap[ r ][ c ]; itemNum > 0 ){ //gets in to chunk only if the item is on the floor
				AutoMapIsometricCalc(r, c, x, y);
				uchar color = 129; // синим магические вещи
				Item& item = Items[itemNum - 1];
				if( item.MagicLevel >= ML_2_UNIQUE ) color = 155; // оранжевым уники и сеты
					
				if( item.dropType & D_RARE ){
					color = 137; //pink
				}else if( item.MagicLevel == ML_0_USUAL ){
					if( item.socketsAmount > 0 ){
					    color = 248; // socketed items... grey
					}else{
					    color = 240; // white items... white
                    }
				}
					
				if( item.ItemCode == IC_11_GOLD ) color = 147; // gold yellow
				else if (is(item.MagicCode, MC_2_POTION_OF_FULL_HEALING, MC_3_POTION_OF_HEALING)) color = 139; // red healing pots
				else if (is(item.MagicCode, MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENATION)) color = 176; // white holy waters
				else if( is( item.MagicCode, MC_6_POTION_OF_MANA, MC_7_POTION_OF_FULL_MANA ) ) UserPotionB || GameMode == GM_CLASSIC ? color = 132 : color = 246; // mana pots blue or grey
				else if( item.MagicCode >= MC_10_ELIXIR_OF_STRENGTH && item.MagicCode <= MC_13_ELIXIR_OF_VITALITY || item.MagicCode == MC_44_SPECTRAL_ELIXIR ) color = 155; // эликсиры оранжевым как уники
				else if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HARDENING ) color = 152; // oils beige
				AutomapDrawOneItem(x, y, color);
			}

			if ((/*is(DungeonLevelType[DungeonLevel+1], 
				DT_1_CHURCH_OR_CRYPT, 
				DT_2_CATACOMB,
				DT_3_CAVE_OR_ABYSS,
				DT_4_HELL, 
				DT_5_CRYPT, 
				DT_6_ABYSS)
				&& */(GameMode == GM_IRONMAN || GameMode == GM_SURVIVAL))
				|| ShowObjectsOnMiniMap) {		//copy paste position calculation from items to barrels and chests using the same Row/Col loop
				if (int objectNum = ObjectsMap[r][c]) {		//gets in to chunk only if the item is on the floor
					Object& object2 = Objects[objectNum - 1];
					if (!is(object2.BaseObjectIndex, BO_57_POD_URN_BARREL_WITH_DROP,
						BO_58_POD_URN_BARREL_EXPLOSIVE,
						BO_5_SMALL_CHEST,
						BO_6_CHEST,
						BO_7_LARGE_CHEST,
						BO_68_TRAPED_SMALL_CHEST,
						BO_69_TRAPED_CHEST,
						BO_67_DECAPITATED_BODY,
						BO_70_TRAPED_LARGE_CHEST,
						BO_80_CAULDRON,
						BO_48_SARCOPHAGUS)) {
						continue;			//if it's not a barrel or chest, go for next coordinate
					}

					if (!object2.selectable) {
						continue;			//if it's not selectable, go for next coordinate
					}

					if (Objects[ObjectsMap[r][c + 1] - 1].BaseObjectIndex == BO_48_SARCOPHAGUS) { //Sacrophagus occupies 2 tiles, comment out to show them on minimap for now, watch out for bugs.
						continue;			//if it's sarcophagus overlaping, go for next coordinate
					}

					AutoMapIsometricCalc(r, c, x, y);
					int objLeft = GetNumberOfObjectsInDungeon();
					if (objLeft <= 10) {
						AutomapDrawOneItem(x, y, 240 + (ColorShifter / 2)); //Leftover objects flashing in white when remaining object count is low.
					} else {
						AutomapDrawOneItem(x, y, 243); //lite_grey
					}

					//uchar color;
					//	if (is(object2.BaseObjectIndex, BO_57_POD_URN_BARREL_WITH_DROP, BO_58_POD_URN_BARREL_EXPLOSIVE)) color = 243; //lite_grey
					//	else if (is(object2.BaseObjectIndex, BO_5_SMALL_CHEST, BO_6_CHEST, BO_7_LARGE_CHEST, BO_68_TRAPED_SMALL_CHEST, BO_69_TRAPED_CHEST, BO_70_TRAPED_LARGE_CHEST)) color = 243; //lite_grey
					//	if (object2.selectable) 
					//		AutomapDrawOneItem(x, y, color); //draw barrels and chests
				}
				int monsterNum = MonsterMap[r][c];
				int monstersleft = GetNumberOfMonstersInDungeon();
				if ( monstersleft <= 10 && monsterNum >= SummonMonsters_Count) {
					AutoMapIsometricCalc(r, c, x, y);
					AutomapDrawOneItem(x, y, 228 + (ColorShifter/2)); //flashing_red
				}
			}
		}
	}
}

struct HighlightName
{
	short itemId, row, col, x, y, x_, y_, width, height;
	char magicLevel;
	char text[64];
	HighlightName( short x, short y, short width, short height, short row, short col, short itemId, char magicLevel, const char* text ):
		itemId(itemId), row(row), col(col), x(x), y(y), x_(-1), y_(-1), width(width), height(height), magicLevel( magicLevel ){ strcpy( this->text, text ); }
};
CObjectUnderCursor HighlightedItem;
//----- (004029A7) --------------------------------------------------------
void HighlightItemsNameOnMap()
{
	if (GamePaused) return;

	char textOnGround[ 256 ];
	vector<HighlightName> HighlightNames; HighlightNames.reserve(MaxItems_255 < 1024 ? MaxItems_255 : 1024);
	short zoomFactor = IsZoomDisable ? 1 : 2;
	int textHeight = 13;
	for( int i = 0; i < ItemsCount; i++ ){
		Item& item = Items[ ItemActive[ i ] ];
		int row = (item.MapRow - PlayerRowPos) * zoomFactor;
		int col = (item.MapCol - PlayerColPos) * zoomFactor;
		// items on ground name highlighting
		if( item.ItemCode == IC_11_GOLD ){
			sprintf( textOnGround, "%i gold", item.amount );
		} else{
			const bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
			sprintf(textOnGround, "%s", identified ? item.FullMagicalItemName : item.Name);
		}
		int x = 32 * (row - col) + (200 * zoomFactor * (Scroll.xOfs + PlayerShiftY) / 100 >> 1) + Xofs - Screen_LeftBorder;
		int y = 16 * (row + col) + (200 * zoomFactor * (Scroll.yOfs + PlayerShiftX) / 100 >> 1) + Yofs - Screen_TopBorder - 30;
		int textWidth = GetTextWidth( textOnGround );
		if( x >= 0 && x < ScreenWidth && y - textHeight > 0 && y < ScreenHeight ){
			HighlightNames.emplace_back( x - textWidth / 2, y, textWidth, textHeight, item.MapRow, item.MapCol, ItemActive[ i ], item.MagicLevel, textOnGround );
		}
	}
	enum { borderX = 5 };
	bool highlightItem = false;
	for( uint i = 0, ie = HighlightNames.size(); i < ie; ++i ){
		if( HighlightNames[ i ].x_ == -1 && HighlightNames[ i ].y_ == -1 ){
			HighlightNames[ i ].x_ = HighlightNames[ i ].x; HighlightNames[ i ].y_ = HighlightNames[ i ].y;
		}
		map<short, bool> backtrace;
		bool overlap;
		do{
			overlap = false;
			for( uint j = 0; j < i; ++j ){
				if( abs( HighlightNames[ j ].y_ - HighlightNames[ i ].y_ ) < HighlightNames[ i ].height + 2 ){
					if( HighlightNames[ j ].x_ >= HighlightNames[ i ].x_ && HighlightNames[ j ].x_ - HighlightNames[ i ].x_ < HighlightNames[ i ].width + borderX ){
						overlap = true;
						int newpos = HighlightNames[ j ].x_ - HighlightNames[ i ].width - borderX;
						if( backtrace.find( newpos ) == backtrace.end() ){
							HighlightNames[ i ].x_ = newpos;
							backtrace[newpos] = true;
						} else{
							newpos = HighlightNames[ j ].x_ + HighlightNames[ j ].width + borderX;
							HighlightNames[ i ].x_ = newpos;
							backtrace[newpos] = true;
						}
					}else if( HighlightNames[ j ].x_ < HighlightNames[ i ].x_ && HighlightNames[ i ].x_ - HighlightNames[ j ].x_ < HighlightNames[ j ].width + borderX ){
						overlap = true;
						int newpos = HighlightNames[ j ].x_ + HighlightNames[ j ].width + borderX;;
						if( backtrace.find( newpos ) == backtrace.end() ){
							HighlightNames[ i ].x_ = newpos;
							backtrace[newpos] = true;
						}else{
							newpos = HighlightNames[ j ].x_ - HighlightNames[ i ].width - borderX;
							HighlightNames[ i ].x_ = newpos;
							backtrace[newpos] = true;
						}
					}
				}
			}
		}while( overlap );
	}

	for( HighlightName& name: HighlightNames){
		if( name.x_ == -1 && name.y_ == -1 ){
			name.x_ = name.x; name.y_ = name.y;
		}
		int x = name.x_;
		int y = name.y_;
		int bgcolor = 0;
		if( x + name.width / 2 >= 0 && x - name.width / 2 < ScreenWidth && y - textHeight > 0 && y < ScreenHeight ){
			int bgcolor = 0;
			if( CursorX >= x && CursorX <= x + name.width + 1 && CursorY >= y && CursorY <= y + name.height ){
				bgcolor = 134;
				HighlightedItem.ItemID = name.itemId;
				HighlightedItem.Row = name.row;
				HighlightedItem.Col = name.col;
				highlightItem = true;
			}
			DrawBox( {x, y, name.width + 1, name.height}, bgcolor );
			DrawText( x + 1, y - 2 + textHeight, 0, name.text, ItemColor(Items[ name.itemId ]) );
		}
	}

	if( highlightItem == false ){
		HighlightedItem.ItemID = -1;
	}
}


//----- (00402B4A) --------------------------------------------------------
void __fastcall AutomapDrawOneItem(int x, int y, uchar color)
{
	AutomapDrawLine(x,	y - AutomapZoom4,						x - AutomapZoom8,						y, color);
	AutomapDrawLine(x,	y - AutomapZoom4,						x - AutomapZoom8 + AutomapZoom16,		y, color);
	AutomapDrawLine(x,	y - AutomapZoom4 + AutomapZoom8,		x - AutomapZoom8,						y, color);
	AutomapDrawLine(x,	y - AutomapZoom4 + AutomapZoom8,		x - AutomapZoom8 + AutomapZoom16,		y, color);
}

//----- (00402BD8) --------------------------------------------------------
void __fastcall AutomapDrawOnePlayer(int playerIndex)
{
    int xPos = 0;
    int yPos = 0;
    const uchar playerArrowColor = 153;
    Player& player = Players[playerIndex];
    if (Players[playerIndex].CurAction == PCA_3_WALK_HORISONTAL) {
        xPos = player.NextRow;
        yPos = player.NextCol;
        if (player.dir == 2) {
            ++xPos;
        }
        else {
            ++yPos;
        }
    }
    else {
        xPos = player.Row;
        yPos = player.Col;
    }

    int row, col, x, y;
    if (AutomapMode == 2) {
        row = xPos;
        col = yPos;
        x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + (AutomapZoomFactor * player.xOfs / 100 >> 1) + AutomapZoom8 * (row - col);
        y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + (AutomapZoomFactor * player.yOfs / 100 >> 1) + AutomapZoom4 * (col + row) - AutomapZoom4;
    }
    else {
        row = xPos - 2 * AutomapRow - PlayerRowPos;
        col = yPos - 2 * AutomapCol - PlayerColPos;
        x = (AutomapZoomFactor * player.xOfs / 100 >> 1) + AutomapZoom8 * (row - col) + (AutomapZoomFactor * Scroll.xOfs / 100 >> 1) + Xofs;
        y = (AutomapZoomFactor * player.yOfs / 100 >> 1) + AutomapZoom4 * (col + row) + (AutomapZoomFactor * Scroll.yOfs / 100 >> 1) + Yofs - AutomapZoom4;
        if (ScreenWidth == GUI_Width) {
            if (IsINVPanelVisible || IsSpellBookVisible) {
                x -= 160;
            }
            if (IsCHARPanelVisible || IsQUESTPanelVisible) {
                x += 160;
            }
        }
    }
    switch (player.dir) {
    case 0:
        AutomapDrawLine(x, y, x, y + AutomapZoom8, playerArrowColor);
        AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom2, y + AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom2, y + AutomapZoom4, playerArrowColor);
        break;
    case 1:
        AutomapDrawLine(x, y, x - AutomapZoom8, y + AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4, y + AutomapZoom4, playerArrowColor);
        break;
    case 2:
        AutomapDrawLine(x, y, x - AutomapZoom8, y, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y - AutomapZoom2, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y + AutomapZoom2, playerArrowColor);
        break;
    case 3:
        AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4, y - AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, playerArrowColor);
        break;
    case 4:
        AutomapDrawLine(x, y, x, y - AutomapZoom8, playerArrowColor);
        AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom2, y - AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom2, y - AutomapZoom4, playerArrowColor);
        break;
    case 5:
        AutomapDrawLine(x, y, x + AutomapZoom8, y - AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom4, y - AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
        break;
    case 6:
        AutomapDrawLine(x, y, x + AutomapZoom8, y, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y - AutomapZoom2, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y + AutomapZoom2, playerArrowColor);
        break;
    case 7:
        AutomapDrawLine(x, y, x + AutomapZoom8, y + AutomapZoom4, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, playerArrowColor);
        AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, x + AutomapZoom4, y + AutomapZoom4, playerArrowColor);
        break;
    }
}

void AutomapDrawOneMonster( int monsterIndex, int color )
{
    Monster& monster = Monsters[monsterIndex];
    int xPos = 0;
    int yPos = 0;
    if (monster.CurAction == A_3_WALK_HORIZ) {
        xPos = monster.NextRow;
        yPos = monster.NextCol;
        if (monster.Orientation == 2) {
            ++xPos;
        }
        else {
            ++yPos;
        }
    }
    else {
        xPos = monster.Row;
        yPos = monster.Col;
    }

    int row, col, x, y;
    if (AutomapMode == 2) {
        row = xPos;
        col = yPos;
        x = (AutomapZoomFactor == 15 ? MiniMapSmallX : MiniMapBigX) + (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom8 * (row - col);
        y = (AutomapZoomFactor == 15 ? MiniMapSmallY : MiniMapBigY) + (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom4 * (col + row) - AutomapZoom4;
    }
    else {
        row = xPos - 2 * AutomapRow - PlayerRowPos;
        col = yPos - 2 * AutomapCol - PlayerColPos;
        x = (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom8 * (row - col) + (AutomapZoomFactor * Scroll.xOfs / 100 >> 1) + Xofs;
        y = (AutomapZoomFactor * 0 / 100 >> 1) + AutomapZoom4 * (col + row) + (AutomapZoomFactor * Scroll.yOfs / 100 >> 1) + Yofs - AutomapZoom4;
        if (ScreenWidth == GUI_Width) {
            if (IsINVPanelVisible || IsSpellBookVisible) {
                x -= 160;
            }
            if (IsCHARPanelVisible || IsQUESTPanelVisible) {
                x += 160;
            }
        }
    }
    switch (monster.Orientation) {
    case 0:
        AutomapDrawLine(x, y, x, y + AutomapZoom8, color);
        AutomapDrawLine(x, y + AutomapZoom8, x + AutomapZoom2, y + AutomapZoom4, color);
        AutomapDrawLine(x, y + AutomapZoom8, x - AutomapZoom2, y + AutomapZoom4, color);
        break;
    case 1:
        AutomapDrawLine(x, y, x - AutomapZoom8, y + AutomapZoom4, color);
        AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, color);
        AutomapDrawLine(x - AutomapZoom8, y + AutomapZoom4, x - AutomapZoom4, y + AutomapZoom4, color);
        break;
    case 2:
        AutomapDrawLine(x, y, x - AutomapZoom8, y, color);
        AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y - AutomapZoom2, color);
        AutomapDrawLine(x - AutomapZoom8, y, x - AutomapZoom4, y + AutomapZoom2, color);
        break;
    case 3:
        AutomapDrawLine(x, y, x - AutomapZoom8, y - AutomapZoom4, color);
        AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4, y - AutomapZoom4, color);
        AutomapDrawLine(x - AutomapZoom8, y - AutomapZoom4, x - AutomapZoom4 - AutomapZoom2, y, color);
        break;
    case 4:
        AutomapDrawLine(x, y, x, y - AutomapZoom8, color);
        AutomapDrawLine(x, y - AutomapZoom8, x - AutomapZoom2, y - AutomapZoom4, color);
        AutomapDrawLine(x, y - AutomapZoom8, x + AutomapZoom2, y - AutomapZoom4, color);
        break;
    case 5:
        AutomapDrawLine(x, y, x + AutomapZoom8, y - AutomapZoom4, color);
        AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, x + AutomapZoom4, y - AutomapZoom4, color);
        AutomapDrawLine(x + AutomapZoom8, y - AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, color);
        break;
    case 6:
        AutomapDrawLine(x, y, x + AutomapZoom8, y, color);
        AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y - AutomapZoom2, color);
        AutomapDrawLine(x + AutomapZoom8, y, x + AutomapZoom4, y + AutomapZoom2, color);
        break;
    case 7:
        AutomapDrawLine(x, y, x + AutomapZoom8, y + AutomapZoom4, color);
        AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, AutomapZoom4 + x + AutomapZoom2, y, color);
        AutomapDrawLine(x + AutomapZoom8, y + AutomapZoom4, x + AutomapZoom4, y + AutomapZoom4, color);
        break;
    }
}

//----- (00402FAC) --------------------------------------------------------
ushort __fastcall GetStaticObjectFlags(int row, int col, bool getForAutomap)
{
	if( getForAutomap ){
		if( row == -1 && In40(col) && AutoMap[ 0 ][ col ] ){
			return ~GetStaticObjectFlags(0, col, 0) & BIT(15);
		}
		if( getForAutomap && col == -1 ){
			if( In40(row) && AutoMap[ row ][ 0 ] ){
				return ~GetStaticObjectFlags(row, 0, 0) & BIT(15);
			}
		}
	}
	if( ! In40(row, col) || !AutoMap[ row ][ col ] && getForAutomap ){
		return 0;
	}
	ushort result = LevelAutomapData[Map[ row ][ col ]];
	if( result == 7 ){
		if( (GetStaticObjectFlags(row - 1, col, 0) >> 8) & 8 ){
			if( (GetStaticObjectFlags(row, col - 1, 0) >> 8) & 4 ){
				result = 1;
			}
		}
	}
	return result;
}

//----- (00403080) --------------------------------------------------------
void DrawLevelInfo()
{
	char buffer[ 256 ];
	int yPos = 18;
	int xPos = 8;
	const char levelInfoColor = 3;
	if( MaxCountOfPlayersInGame > 1u ){
		sprintf( buffer, "game: %s", szPlayerName);
		xPos += DrawLevelInfoText( xPos, yPos, buffer, levelInfoColor, OnScreenTextOutline );
		if( szPlayerDescript[0] ){
			sprintf( buffer, ", password: %s", szPlayerDescript);
			xPos += DrawLevelInfoText( xPos, yPos, buffer, levelInfoColor, OnScreenTextOutline );
		}
	}
	if( DevelopMode ){
		sprintf( buffer, "%s, Level: %i", Dungeon->name, Dungeon->level );
	}else if( Dungeon > DUN_0_TOWN ){
		if( Dungeon->isQuest ){
			sprintf( buffer, "%s", Dungeon->name );
		}else{
			sprintf( buffer, "Level: %i", Dungeon->level );
		}
	}else return;
	if(  MaxCountOfPlayersInGame > 1u ){
		xPos += DrawLevelInfoText( xPos, yPos, ", ", levelInfoColor, OnScreenTextOutline );
	}
	DrawLevelInfoText( xPos, yPos, buffer, levelInfoColor, OnScreenTextOutline );
}

//----- (00403189) --------------------------------------------------------
void __fastcall SetAutomapView(int row112, int col112)
{
	int row40 = (row112 - 16) >> 1;
	int col40 = (col112 - 16) >> 1;
	if( row40 < 0 || row40 >= 40 || col40 < 0 || col40 >= 40 ){
		return;
	}
	AutoMap[row40][col40] = true;
	int cellParams = GetStaticObjectFlags(row40, col40, false);
	bool isRoof = (cellParams & AM_B_15_Roof) != 0;
	bool chPlusCol = false;
	bool chPlusRow = false;
	bool chMinusCol = false;
	bool chMinusRow = false;
	bool chMinusRowCol = false;
	switch (cellParams & AM_F_Mask ){
	case AM_2_WestWall:
		{
			if( isRoof ){
				chPlusCol = true;
			}else{
				chMinusRow = true;
			}
			break;
		}
	case AM_3_NorthWall:
		{
			if( isRoof ){
				chPlusRow = true;
			}else{
				chMinusCol = true;
			}
			break;
		}
	case AM_4_NorthWestCorner:
		{
			if( isRoof ){
				chPlusCol = true;
				chPlusRow = true;
			}else{
				chMinusCol = true;
				chMinusRow = true;
				chMinusRowCol = true;
			}
			break;
		}
	case AM_5_WestEdge:
		{
			if( isRoof ){
				chPlusCol = true;
				chMinusCol = true;
			}else{
				chMinusRow = true;
			}
			break;
		}
	case AM_6_NorthEdge:
		{
			if( isRoof ){
				chPlusRow = true;
				chMinusRow = true;
			}else{
				chMinusCol = true;
			}
			break;
		}
	}
	if( chPlusCol && (GetStaticObjectFlags(row40, col40 + 1, false) == (AM_B_15_Roof | AM_7_SouthEastEdge))){
		AutoMap[row40][col40 + 1] = true;
	}
	if( chPlusRow && (GetStaticObjectFlags(row40 + 1, col40, false) == (AM_B_15_Roof | AM_7_SouthEastEdge))){
		AutoMap[row40 + 1][col40] = true;
	}
	if( chMinusCol && (GetStaticObjectFlags(row40, col40 - 1, false) & AM_B_15_Roof)){
		AutoMap[row40][col40 - 1] = true;
	}
	if( chMinusRow && (GetStaticObjectFlags(row40 - 1, col40, false) & AM_B_15_Roof)){
		AutoMap[row40 - 1][col40] = true;
	}
	if( chMinusRowCol && (GetStaticObjectFlags(row40 - 1, col40 - 1, false) & AM_B_15_Roof)){
		AutoMap[row40 - 1][col40 - 1] = true;
	}
}

//----- (0040332A) --------------------------------------------------------
void SyncAutomap()
{
	AutomapRow = 0;
	AutomapCol = 0;
	SetAutoMapZoom();
}

//----- (0040506E) --------------------------------------------------------
void ToggleAutomap()
{
	if ((Players[CurrentPlayerIndex].gameChanger & BIT(GC_19_NO_MAP))) {
		AddOnScreenMessage(OM_77_NO_MAP_GC);
	}
	else if( Dungeon != DUN_0_TOWN || MaxCountOfPlayersInGame != 1) {
		if( AutomapMode ){
			if( IsCtrlPressed && AutomapMode == 2 ){
				if( AutomapZoomFactor == 25 ){
					MiniMapBigX = Default_MiniMapBigX + 64;
					MiniMapBigY = Default_MiniMapBigY;
				}else{
					MiniMapSmallX = Default_MiniMapSmallX + 64;
					MiniMapSmallY = Default_MiniMapSmallY;
				}
			}else{
				if( ++AutomapMode > 2 ) AutomapMode = 0;
				if( AutomapMode == 2 ){
					AutomapZoomFactor = 25; //set to MinimapZoom for default, small minimap size
					SetAutoMapZoom();
				}
			}
		}else{
			AutomapRow = 0;
			AutomapCol = 0;
			AutomapMode = 1;
			AutomapZoomFactor = 50;
			SetAutoMapZoom();
		}
	}else{
		AddOnScreenMessage( OM_1_No_automap ); // no automap available in town
	}
	PlayGlobalSound(75);
}

bool isPointInPanel( int x, int y )
{
    int xS = x - Screen_LeftBorder;
    int yS = y - Screen_TopBorder;


    if( IsINVPanelVisible && yS <= InventoryPanelRect.Down && x >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth )
    {
        return true;
    }
    if( IsSpellBookVisible && yS <= GUI_PanelHeight && x >= WorkingWidth - Screen_LeftBorder - GUI_PanelWidth )
    {
        return true;
    }
    if( (IsCHARPanelVisible || IsQUESTPanelVisible) && yS <= GUI_PanelHeight && x <= Screen_LeftBorder + GUI_PanelWidth )
    {
        return  true;
    }
    if( yS >= ScreenHeight - 136 && x >= (WorkingWidth - GUI_Width) / 2 && x <= (WorkingWidth - GUI_Width) / 2 + GUI_Width )
    {
        return true;
    }
    if( IsPerksPanelVisible )
        if( yS >= PerksPanelPos.Top && yS <= PerksPanelPos.Down )
            if( xS >= PerksPanelPos.Left && xS <= PerksPanelPos.Right )
                return true;

    return false;
}

//----- (004199E7) --------------------------------------------------------
void __fastcall AutomapDrawPoint1( int xPos, int yPos, uchar color )
{
	if( yPos >= Screen_TopBorder && yPos < Screen_TopBorder + ScreenHeight && xPos >= Screen_LeftBorder && xPos < WorkingWidth - Screen_LeftBorder ){
		//if( curPixel < WorkingSurfaceEnd ){
		if( MapBlend && DrawMode ){
			if( AutomapPointsCount >= (countof( AutomapPoints ) - 1)
                || isPointInPanel( xPos, yPos ) )
            {
                return;
            }
			line_vertex& a = AutomapPoints[ AutomapPointsCount ];
			PALETTEENTRY p = ScreenPalette[ color ];
			a.pos.x = (float)(xPos - Screen_LeftBorder);
			a.pos.y = (float)(yPos - Screen_TopBorder);
			// в режиме фильтрации масштабируется корректно автоматически, а без фильтрации миникарта лучше выглядит немасштабированная
			//if( !FilteringMode ){ a.pos.x *= AspectX; a.pos.y *= AspectY; }
			a.pos.z = 0.0;
			a.pos.w = 1.0;
			a.color = (AutomapTransparency << 24) + p.peBlue + ((uint) p.peGreen << 8) + ((uint) p.peRed << 16); // 127 - 50% transparency 
			++AutomapPointsCount;
			return;
		}
		uchar* curPixel = WorkingSurface + xPos + YOffsetHashTable[ yPos ];
		*curPixel = color;
		//}

	}
}

//----- (00419A2C) --------------------------------------------------------
void __fastcall AutomapDrawPoint( int xPos, int yPos )
{
	//if( AutomapRotated ){
	//	if( xPos >= 0 && xPos < ScreenWidth && yPos >= 64 && yPos < WorkingWidth - 64 ){
	//		uchar* curPixel = WorkingSurface + yPos + YOffsetHashTable[ xPos ];
	//		/*if( curPixel < WorkingSurfaceEnd ){*/
	//		*curPixel = AutomapCurrentDrawColor;
	//		/*}*/
	//	}
	//}else{
		if( yPos >= 0 && yPos < /*ScreenWidth*/WorkingHeight && xPos >= 64 && xPos < WorkingWidth - 64 ){
			uchar* curPixel = WorkingSurface + xPos + YOffsetHashTable[ yPos ];
			/*if( curPixel < WorkingSurfaceEnd ){*/
			*curPixel = AutomapCurrentDrawColor;
			/*}*/
		}
	//}
}

// функция полностью переписана, согласно алгоритма Брезенхэма.
//----- (00419AAF) --------------------------------------------------------
void __fastcall AutomapDrawLine( int x1, int y1, int x2, int y2, uchar color )
{
	AutomapCurrentDrawColor = color;
	if( x1 < Screen_LeftBorder || x1 >= WorkingWidth - Screen_RightBorder ){
		return;
	}
	if( x2 < Screen_LeftBorder || x2 >= WorkingWidth - Screen_RightBorder ){
		return;
	}
	if( y1 < Screen_TopBorder || y1 >= WorkingHeight - Screen_BottomBorder ){
		return;
	}
	if( y2 < Screen_TopBorder || y2 >= WorkingHeight - Screen_BottomBorder ){
		return;
	}
	if( MapBlend && DrawMode ){
		if( isPointInPanel( x1, y1 ) || isPointInPanel( x2, y2 ) ){
			return;
		}
		if( AutomapLinePointsCount >= countof(AutomapLines) - 2 ) return;
		line_vertex& a1 = AutomapLines[ AutomapLinePointsCount ];
		line_vertex& a2 = AutomapLines[ AutomapLinePointsCount + 1 ];
		PALETTEENTRY p = ScreenPalette[ color ];
		a1.pos.x = (float) (x1 - Screen_LeftBorder);
		a1.pos.y = (float) (y1 - Screen_TopBorder);
		a1.pos.z = 0;
		a1.pos.w = 1.0;
		a1.color = (AutomapTransparency<<24) + p.peBlue + ((uint)p.peGreen << 8) + ((uint)p.peRed << 16); // 127 - 50% transparency 
		a2.pos.x = (float) (x2 - Screen_LeftBorder);
		a2.pos.y = (float) (y2 - Screen_TopBorder);
		//float dx = abs(a2.pos.x - a1.pos.x);
		//float dy = abs(a2.pos.y - a1.pos.y);
		//float aspect = dx > dy ? dy / dx : dx / dy;
		//a2.pos.x += a2.pos.x > a1.pos.x ? -1.0f * aspect : 1.0f * aspect;
		//a2.pos.y += a2.pos.y > a1.pos.y ? -1.0f * aspect : 1.0f * aspect;
		a2.pos.z = 0;
		a2.pos.w = 1.0;
		a2.color = a1.color;
		// в режиме фильтрации масштабируется корректно автоматически, а без фильтрации миникарта лучше выглядит немасштабированная
		//if( !FilteringMode ){ a1.pos.x *= AspectX; a1.pos.y *= AspectY; a2.pos.x *= AspectX; a2.pos.y *= AspectY; }
		AutomapLinePointsCount += 2;
		return;
	}

	bool alt = false;
	int dx;
	int dy;
	if( x1 > x2 && y1 > y2 ){
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
		tmp = y2;
		y2 = y1;
		y1 = tmp;
		dx = x2 - x1;
		dy = y2 - y1;
	}else{
		dx = x2 - x1;
		dy = y2 - y1;
		if( dy < 0 ){
			dy = -dy;
			int tmp = y2;
			y2 = y1;
			y1 = tmp;
			alt = true;
		}else if( dx < 0 ){
			dx = -dx;
			int tmp = x2;
			x2 = x1;
			x1 = tmp;
			alt = true;
		}
	}
	if( alt ){
		if( dx >= dy ){
			int y = y2;
			int err = 0;
			for( int x = x1; x <= x2; x++ ){
				AutomapDrawPoint( x, y );
				err += dy;
				if( err >= dx ){
					y--;
					err -= dx;
				}
			}
		}else{
			int x = x2;
			int err = 0;
			for( int y = y1; y <= y2; y++ ){
				AutomapDrawPoint( x, y );
				err += dx;
				if( err >= dy ){
					x--;
					err -= dy;
				}
			}
		}
	}else{
		if( dx >= dy ){
			int y = y1;
			int err = 0;
			for( int x = x1; x <= x2; x++ ){
				AutomapDrawPoint( x, y );
				err += dy;
				if( err >= dx ){
					y++;
					err -= dx;
				}
			}
		}else{
			int x = x1;
			int err = 0;
			for( int y = y1; y <= y2; y++ ){
				AutomapDrawPoint( x, y );
				err += dx;
				if( err >= dy ){
					x++;
					err -= dy;
				}
			}
		}
	}
}

//----- (0042C121) --------------------------------------------------------
void __fastcall DoVision( int row, int col, int lightRadius, int someFlag1, int someFlag2 )
{
	if( In112(row, col) ){
		if( someFlag1 ){
			if( (FlagMap[ row ][ col ] & CF_128_DISCOVERED) == 0 ){
				SetAutomapView( row, col );
			}
			FlagMap[ row ][ col ] |= CF_128_DISCOVERED;
		}
		if( someFlag2 ){
			FlagMap[ row ][ col ] |= CF_64_VISIBLE_BY_CURSOR;
		}
		FlagMap[ row ][ col ] |= CF_2_VISIBLE_BY_PLAYER;
	}

	if( Lovecraft ){
		LovecraftDoVision(row, col, lightRadius,someFlag1,someFlag2);
	}else
	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 23; j++ ){
			uchar* novaCircleDeltas = &NovaCircleDeltas[ j ][ 1 ];
			uchar cellBlockSight = 0;
			int v28 = 2 * (lightRadius - RadiusAdj[ j ]);
			for( int k = 0; k < v28 && cellBlockSight == 0; k += 2 ){
				int rowOffset1 = 0;
				int rowOffset2 = 0;
				int colOffset1 = 0;
				int colOffset2 = 0;
				int novaCircleRow = novaCircleDeltas[ k - 1 ];
				int novaCircleCol = novaCircleDeltas[ k ];
				int row2 = 0;
				int col2 = 0;
				switch( i ){
				case 0:
					row2 = row + novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = -1;
					}
					break;
				case 1:
					row2 = row - novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = 1;
						rowOffset2 = 1;
					}
					break;
				case 2:
					row2 = row + novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = 1;
					}
					break;
				case 3:
					row2 = row - novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = -1;
						rowOffset2 = 1;
					}
					break;
				}
				if( To112(row2, col2) ){
					int cell = col2 + 112 * row2;
					cellBlockSight = TileBlockSight[ FineMap[ row2 ][ col2 ] ];
					if( !TileBlockSight[ FineMap[ row2 + rowOffset1 ][ col2 + colOffset1 ] ] || !TileBlockSight[ FineMap[ row2 + rowOffset2 ][ col2 + colOffset2 ] ] ){
						if( someFlag1 ){
							if( (FlagMap[ 0 ][ cell ] & CF_128_DISCOVERED) == 0 ){
								SetAutomapView( row2, col2 );
							}
							FlagMap[ 0 ][ cell ] |= CF_128_DISCOVERED;
						}
						if( someFlag2 ){
							FlagMap[ 0 ][ cell ] |= CF_64_VISIBLE_BY_CURSOR;
						}
						FlagMap[ 0 ][ cell ] |= CF_2_VISIBLE_BY_PLAYER;
						if( !cellBlockSight ){
							int transparentWallsMap = TransMap[ 0 ][ cell ];
							if( transparentWallsMap ){
								TransList[ transparentWallsMap ] = 1;
							}
						}
					}
				}
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall LovecraftDoVision( int row, int col, int lightRadius, int someFlag1, int someFlag2 )
{
	int LovecraftRadius = 3;     // Gribnik

	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 23; j++ ){
			uchar* novaCircleDeltas = &NovaCircleDeltas[ j ][ 1 ];
			uchar cellBlockSight = 0;
			int v28 = 2 * (lightRadius - RadiusAdj[ j ]);
			for( int k = 0; k < v28 && cellBlockSight == 0; k += 2 ){
				int rowOffset1 = 0;
				int rowOffset2 = 0;
				int colOffset1 = 0;
				int colOffset2 = 0;
				int novaCircleRow = novaCircleDeltas[ k - 1 ];
				int novaCircleCol = novaCircleDeltas[ k ];
				int row2 = 0;
				int col2 = 0;
				switch( i ){
				case 0:
					row2 = row + novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = -1;
					}
					break;
				case 1:
					row2 = row - novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = 1;
						rowOffset2 = 1;
					}
					break;
				case 2:
					row2 = row + novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = 1;
					}
					break;
				case 3:
					row2 = row - novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = -1;
						rowOffset2 = 1;
					}
					break;
				}
				if( To112(row2, col2) ){
					int cell = col2 + 112 * row2;
					cellBlockSight = TileBlockSight[ FineMap[ row2 ][ col2 ] ];
					if( !TileBlockSight[ FineMap[ row2 + rowOffset1 ][ col2 + colOffset1 ] ] || !TileBlockSight[ FineMap[ row2 + rowOffset2 ][ col2 + colOffset2 ] ] ){
						if( someFlag1 ){
							if( (FlagMap[ 0 ][ cell ] & CF_128_DISCOVERED) == 0 ){
								SetAutomapView( row2, col2 );
							}
							FlagMap[ 0 ][ cell ] |= CF_128_DISCOVERED;
						}
						//if( someFlag2 ){
						//	FlagMap[ 0 ][ cell ] |= CF_64_VISIBLE_BY_CURSOR;   // не отображаются монстры
						//}
						FlagMap[ 0 ][ cell ] |= CF_2_VISIBLE_BY_PLAYER;     // не видят монстры
						if( !cellBlockSight ){
							int transparentWallsMap = TransMap[ 0 ][ cell ];
							if( transparentWallsMap ){
								TransList[ transparentWallsMap ] = 1;
							}
						}
					}
				}
			}
		}
	}

	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 23; j++ ){
			uchar* novaCircleDeltas = &NovaCircleDeltas[ j ][ 1 ];
			uchar cellBlockSight = 0;
			int v28 = 2 * (LovecraftRadius - RadiusAdj[ j ]);
			for( int k = 0; k < v28 && cellBlockSight == 0; k += 2 ){
				int rowOffset1 = 0;
				int rowOffset2 = 0;
				int colOffset1 = 0;
				int colOffset2 = 0;
				int novaCircleRow = novaCircleDeltas[ k - 1 ];
				int novaCircleCol = novaCircleDeltas[ k ];
				int row2 = 0;
				int col2 = 0;
				switch( i ){
				case 0:
					row2 = row + novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = -1;
					}
					break;
				case 1:
					row2 = row - novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = 1;
						rowOffset2 = 1;
					}
					break;
				case 2:
					row2 = row + novaCircleRow;
					col2 = col - novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						rowOffset1 = -1;
						colOffset2 = 1;
					}
					break;
				case 3:
					row2 = row - novaCircleRow;
					col2 = col + novaCircleCol;
					if( novaCircleRow && novaCircleCol ){
						colOffset1 = -1;
						rowOffset2 = 1;
					}
					break;
				}
				if( To112(row2, col2) ){
					int cell = col2 + 112 * row2;
					cellBlockSight = TileBlockSight[ FineMap[ row2 ][ col2 ] ];
					if( !TileBlockSight[ FineMap[ row2 + rowOffset1 ][ col2 + colOffset1 ] ] || !TileBlockSight[ FineMap[ row2 + rowOffset2 ][ col2 + colOffset2 ] ] ){
						//if( someFlag1 ){
						//	if( (FlagMap[ 0 ][ cell ] & CF_128_DISCOVERED) == 0 ){
						//		SetAutomapView( row2, col2 );
						//	}
						//	FlagMap[ 0 ][ cell ] |= CF_128_DISCOVERED;
						//}
						if( someFlag2 ){
							FlagMap[ 0 ][ cell ] |= CF_64_VISIBLE_BY_CURSOR;   // не отображаются монстры
						}
						//FlagMap[ 0 ][ cell ] |= CF_2_VISIBLE_BY_PLAYER;     // не видят монстры
						if( !cellBlockSight ){
							int transparentWallsMap = TransMap[ 0 ][ cell ];
							if( transparentWallsMap ){
								TransList[ transparentWallsMap ] = 1;
							}
						}
					}
				}
			}
		}
	}
}

//----- (00706FF0) --------------------------------------------------------
void AutomapDrawPlayers()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		if( Players[playerIndex].IsExists != 0 && Players[playerIndex].dungeon == Dungeon ){
			AutomapDrawOnePlayer( playerIndex );
		}
	}
}
