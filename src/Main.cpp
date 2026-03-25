#include "stdafx.h"

bool MouseMoved = false;
bool IsMainDraw = false;
bool Berserk = false;
bool ForceMove = false;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (th2) -------------------------------------------------------------
void ChangeCursorPos(int dx, int dy)
{
	POINT p;
	GetCursorPos(&p);
	CursorX += dx;
	CursorY += dy;
	SetCursorPos(p.x + (int)round((double)dx * WinWidth / ScreenWidth), p.y + (int)round((double)dy * WinHeight / ScreenHeight) );

}

//----- (th2) -------------------------------------------------------------
void FixItemCursor(int sign)
{
	Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
	if( !IsController() || XinputNotInUse(true) ){
		ChangeCursorPos(sign * (int)round((double)ItemWidth / 2), sign * (int)round((double)ItemHeight / 2));
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall SetCursorPosition(HWND hwnd, uint coordXY)
{

	POINT pt;
	pt.x = (int)(ushort)coordXY;
	pt.y = (int)((uint)coordXY >> 16);
	#ifndef USE_32BITCOLOR
	ClientToScreen(hwnd, &pt);
	#endif
	int tempx = CursorX, tempy = CursorY;
	CursorX = ( IsFullScreen && !BigWin || WinWidth  == ScreenWidth  ) ? pt.x : (int) ( double(ScreenWidth) / WinWidth * pt.x );
	CursorY = ( IsFullScreen && !BigWin || WinHeight == ScreenHeight ) ? pt.y : (int) ( double(ScreenHeight) / WinHeight * pt.y );
	if (IsController()) { //Protect the cursor from apperance if Windows functions would call SetCursorPosition with the same coords
		if (CursorX != tempx || CursorY != tempy) {
			XinputAutoTargettingOff = true;
			XinputAutoTargettingOffTimer = 0;
		}
	}
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (th3) -------------------------------------------------------------
void InitItemsCel(char* itemSprites, int firstIndex)
{
	int i, width, height;
	int lastItem = *(int*) itemSprites;
	for( i = 0; i < lastItem; ++i ){
		char* item = itemSprites + ((int*) itemSprites)[i + 1];
		item += *(short*)item; // начало пикселей спрайта

		width = 0;

		#if 0
		while( width != 28 && width < 56 ){
			if( *item < 0 ){ width += -*item++; }else{ width += *item; item += *item + 1; }
		}
		height = 1;
		char* next = itemSprites + ((int*) itemSprites)[i + 2];
		while( item < next ){
			int widthLeft = width;
			while( widthLeft > 0 ){
				if( *item < 0 ){ widthLeft -= -*item++; }else{ widthLeft -= *item; item += *item + 1; }
			}
			if( widthLeft != 0 ){
				if( item >= next ) break;
				if( width == 28 ){
					width = 56;
					height /= 2;
					widthLeft += 28;
					while( widthLeft > 0 ){
						if( *item < 0 ){ widthLeft -= -*item++; } else{ widthLeft -= *item; item += *item + 1; }
					}
				}else __debugbreak();
			}
			++height;
		}
		#else
		width = CELL_SIZE;
		char* curItem = item;
		recalc:
		height = 0;
		char* next = itemSprites + ((int*) itemSprites)[i + 2];
		while( item < next ){
			int widthLeft = width;
			while( widthLeft > 0 ){
				int count = *item++; // count of pixels
				if( count < 0 ){
					widthLeft -= -count; // negative count - transparent pixels skipped
				}else{
					widthLeft -=  count; // positive byte - count of solid pixels
					item += count;  // skip solid pixels block
				}
			}
			if( widthLeft != 0 ){
				if( item >= next ) break;
				if( width < CELL_SIZE * MaxItemCellWidth_4 ){
					width += CELL_SIZE;
					item = curItem;
					goto recalc;
				}//else __debugbreak();
			}
			++height;
		}
		//if( height % CELL_SIZE == 1 ) --height;
		//if( width <= 0 || width > CELL_SIZE * MaxItemCellWidth_4 || width % CELL_SIZE != 0 ) __debugbreak();
		//if( height <= 0 || height > CELL_SIZE * MaxItemCellWidth_4 + 1 || (height % CELL_SIZE) > 1 ) __debugbreak();
		//if( ItemWidthTable[168 + 12 + i] != width || ItemHeightTable[168 + 12 + i] != height ) __debugbreak();
		#endif

		int picIndex = firstIndex + i;
		ItemWidthTable[ picIndex ] = width;
		ItemHeightTable[ picIndex ] = height;
		if( picIndex >= MAX_9000_ITEM_PICS ){
			if( IsFullScreen ) SwitchFullscreen( 1 );
			char text[ 128 ];
			sprintf( text, "The number of item pictures (%i) has been exceeded", picIndex );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit( 1 );
		}
	}
}

//----- (00407A46) -------------------------------------------------------- interface infopanel
void InitItemsCELArrays()
{
	if( !ClassicItemSprites ) ClassicItemSprites = (char*) LoadFile( "Data\\Inv\\Objcurs.CEL" ); // diablo.mpq
	//if( !AddonItemSprites ) AddonItemSprites = (char*) LoadFile( "Data\\Inv\\Objcurs2.CEL" ); // hellfire.mpq не используется, в раздаче его нет, thdata.mor это он же измененный
	if( !ClassicModItemSprites ) ClassicModItemSprites = (char*) LoadFile( "Data\\Inv\\invobj1.CEL" ); // thdata.mor
	if( !AddonModItemSprites ) AddonModItemSprites = (char*) LoadFile( "data\\inv\\TH2objcurs.cel" ); // thdata.mor, th3data.mor
	if( !Th3ItemSprites ) Th3ItemSprites = (char*) LoadFile( "data\\inv\\TH3objcurs.cel" ); // th3data.mor
	ZeroCursorImageWidth();
	InitItemsCel(AddonModItemSprites, 168 + 12);
	InitItemsCel(Th3ItemSprites, 3716 + 168 + 12 + 1);
}

//----- (00407A6D) -------------------------------------------------------- interface infopanel
void FreeItemsCELArrays()
{
	FreeMemZero( ClassicItemSprites );
	//FreeMemZero( AddonItemSprites );
	FreeMemZero( ClassicModItemSprites);
	FreeMemZero( AddonModItemSprites );
	FreeMemZero( Th3ItemSprites );
	ZeroCursorImageWidth();
}

//----- (00407A96) --------------------------------------------------------
void __fastcall SetupItemSizes(int graphicsIndex)
{
	uint w = ItemWidthTable[graphicsIndex], h = ItemHeightTable[graphicsIndex];
	if( graphicsIndex && ( w == 0 || h == 0 ) ){
		TerminateWithError( "Please update TH4data.mor file, item %i not found", graphicsIndex );
	}
	Cur.ItemWidth = w; Cur.ItemHeight = h;
	ItemWidthCells = w / CELL_SIZE; ItemHeightCells = h / CELL_SIZE;
}

// сюда какое то адовое количестово и jmp и call на обе функции, как бы не упало при сверке
//----- (00407ACA) --------------------------------------------------------
//----- (00407AED) --------------------------------------------------------// джамп на 00407ACA
void __fastcall SetCursorGraphics( int graphicsIndex )
{
	Cur.GraphicsID = graphicsIndex;
	ItemWidth = ItemWidthTable[graphicsIndex];
	ItemHeight = ItemHeightTable[graphicsIndex];
	SetupItemSizes(graphicsIndex);
}

//----- (00407AF2) -------------------------------------------------------- portals
void ResetCursorOnPlayer()
{
	SetCursorGraphics(CM_1_NORMAL_HAND);
	Cur.GroundRow = Cur.Row = PlayerRowPos;
	Cur.GroundCol = Cur.Col = PlayerColPos;
	Cur.MonsterID = -1;
	CurMon = -1;
	Cur.ObjectIndex = -1;
	Cur.ItemID = -1;
	Cur.playerIndex = -1;
	ZeroCursorImageWidth();
}

//----- (00407B2F) -------------------------------------------------------- portals
void ShowTownPortalDescription()
{
	int cursorRow = Cur.Row;
	int cursorCol = Cur.Col;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		int row = spell.Row;
		int col = spell.Col;
		if( spell.BaseMissileIndex != MI_10_TOWN_PORTAL ){
			continue;
		}
		if( cursorRow == row - 1 && cursorCol == col 
			|| cursorRow == row && cursorCol == col - 1 
			|| (cursorRow == row - 1 || cursorRow == row - 2) && (cursorCol == col - 1 || cursorCol == col - 2)
			|| cursorRow == row && cursorCol == col ){// треугольник над курсором проверяется
				IswarpUnderCursor = true;
				InfoPanel_ClearBody();
				strcpy(InfoPanelHeader, "Town Portal");
				sprintf(InfoPanelBuffer, "from %s", Players[spell.CasterIndex].playerName);
				InfoPanel_AddLine(InfoPanelBuffer, true);
				cursorRow = row;
				cursorCol = col;
				Cur.Row = row;
				Cur.Col = cursorCol;
		}
	}
}

//----- (00407C73) -------------------------------------------------------- portals
void ShowUnholyPortalDescription()
{
	if( Dungeon->isQuest ){
		if( is(Dungeon, DUN_35_PASSAGE_OF_FIRE, DUN_36_VALLEY_OF_DESPAIR, DUN_40_ISLAND, DUN_45_THE_DEN) ){
			return;
		}
	}else if( IsQuestOnLevel( Q_20_SUNLESS_SEA ) ){
		return;
	}
	int cursorRow = Cur.Row;
	int cursorCol = Cur.Col;

	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		int row = spell.Row;
		int col = spell.Col;
		if( spell.BaseMissileIndex != MI_65_RED_PORTAL ){
			continue;
		}
		if( cursorRow == row - 1 && cursorCol == col 
		 || cursorRow == row && cursorCol == col - 1 
		 || (cursorRow == row - 1 || cursorRow == row - 2) && (cursorCol == col - 1 || cursorCol == col - 2)
		 || cursorRow == row && cursorCol == col ){// треугольник над курсором проверяется
			IswarpUnderCursor = true;
			InfoPanel_ClearBody();
			strcpy(InfoPanelHeader, "Portal to");
			char* levelName;
			if( Dungeon->isQuest ){
				if( is (Dungeon, DUN_44_RAVENHOLM, DUN_46_COLISEUM) ){
					levelName = "Tristram";
				}else{
					levelName = "level 15";
				}
			}else if( Dungeon == DUN_0_TOWN ){
				levelName = "RavenHolm";
			}else{
				levelName = "The Unholy Altar";
			}
			strcpy(InfoPanelBuffer, levelName);
			InfoPanel_AddLine(InfoPanelBuffer, true);
			Cur.Row = row;
			Cur.Col = col;
		}
	}
}

int CurFineMap = 0;
//----- (00778830) ------------------------------------------------------------
void __fastcall CheckSignes(int row, int col)
{
	CurFineMap = FineMap[ row ][ col ];
	for( int i = 0; Signes[i].messageType != -1; ++i ){  // signes patch (3) did
		Sign& s = Signes[i];
		if( s.objectType == 1 ){
			if( CurFineMap && (s.objectNum == CurFineMap || s.objectNum2 == CurFineMap) || !s.objectNum ){
				if( s.left || s.top ){
					if( s.right || s.bottom ){
						if( row >= s.left && col >= s.top && row <= s.right && col <= s.bottom ){
							CurSign = i; return;
						}
						continue;
					}
					if( row == s.left && col == s.top ){
						CurSign = i; return;
					}
					continue;
				}
				if( s.objectNum ){
					CurSign = i; return;
				}
			}
		}
	}
	CurSign = -1;
}
int __fastcall ReturnMemorialSignType() {
	switch (GameMode){
		case GM_IRONMAN: 
			return MEMORIAL_IRONMAN;
		case GM_SPEEDRUN:
			return MEMORIAL_SPEEDRUN;
		default: 
			return -1;
	}
}

void __fastcall CheckMemorialSignes(int row, int col)
{
	CurFineMap = FineMap[ row ][ col ];
		Sign& s = MemorialSign;
		if( s.objectType == 1 ){
			if( CurFineMap && (s.objectNum == CurFineMap || s.objectNum2 == CurFineMap) || !s.objectNum ){
				if( s.left || s.top ){
					if( s.right || s.bottom ){
						if( row >= s.left && col >= s.top && row <= s.right && col <= s.bottom ){
							CurSign = ReturnMemorialSignType(); return;
						}
					}
					if (row == s.left && col == s.top) {
						CurSign = ReturnMemorialSignType(); return;
					}
				}
				if( s.objectNum ){
					CurSign = ReturnMemorialSignType(); return;
				}
			}
		}
	CurSign = -1;	
}

//----- (th2) ------------------------------------------------------------- interface
bool CursorInMainPanel()
{
	int x = CursorX;
	int y = CursorY;
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		x += (CurCursorWidth / 2); // For items we consider cursor x as item's center
		y += (CurCursorHeight / 2);
	}
	return IsInterface && y > ScreenHeight - 128 && y < ScreenHeight
		&& x >= ( ScreenWidth - GUI_Width ) / 2 && x < GUI_Width + (ScreenWidth - GUI_Width) / 2;
}

#define check_pi(v) (uint)v <= 3 ? v : -1 // check player index
//----- (00407DA9) -------------------------------------------------------- land interface
void DispatchCursorAboveIsoland() // CheckCursMove
{
	if (IsController() && !XinputAutoTargettingOff) {
		return;
	}

	int  monsterNum   = 0;
	int  monsterIndex = 0;
	int  townerNum    = 0;
	char playerNum    = 0;
	char playerIndex  = 0;
	char objectNum    = 0;
	char objectIndex  = 0;
	char selectable   = 0;
	int itemNum      = 0;
	int itemIndex    = 0;
	int  cursorOnNear = 0;

    VisualTrade_ClearInfostring();
	int curX = CursorX;
	int curY = CursorY;
	if( !IsZoomDisable ){
		curX >>= 1;
		curY >>= 1;
	}
	int x1 = curX - Scroll.xOfs;
	int y1 = curY - Scroll.yOfs;
	Player* player = &Players[ CurrentPlayerIndex ];
	if( Scroll.dir ){
		x1 -= (Players[ CurrentPlayerIndex ].dx64 >> 8)
			- ((Players[ CurrentPlayerIndex ].dx64 + Players[ CurrentPlayerIndex ].xVel) >> 8);
		y1 -= (player->dy64 >> 8) - ((player->dy64 + Players[ CurrentPlayerIndex ].yVel) >> 8);
	}
	if( x1 < 0 ){
		x1 = 0;
	}
	if( x1 >= ScreenWidth /*640*/ ){
		x1 = ScreenWidth;
	}
	if( y1 < 0 ){
		y1 = 0;
	}
	if( y1 >= ScreenHeight /*480*/ ){
		y1 = ScreenHeight;
	}
	int x2 = x1 >> 6;
	int y2 = y1 >> 5;
	int v94 = x1 & 0b0011'1111; // 0x3F
	int v8 = y1 & 0b0001'1111; // 0x1F
	int curRow = PlayerRowPos + x2 + y2 - (IsZoomDisable != 0 ? ScreenWidth / 64 : ScreenWidth / 128 ); // 10 : 5
	int curCol = PlayerColPos + y2 - x2;
	int zoomFactor = IsZoomDisable ? 1 : 2;
	curRow += HeightShift / zoomFactor; curCol += HeightShift / zoomFactor;
	int curMapCol = curCol;
	if( v8 < v94 >> 1 ){
		curMapCol = --curCol;
	}
	if( v8 >= 32 - (v94 >> 1) ){
		++curRow;
	}
	Limit112(curRow, curCol, curMapCol);
	if( v8 < v94 >> 1 ){
		if( v8 >= 32 - (v94 >> 1) ){
			goto LABEL_43;
		}
		if( v8 < v94 >> 1 ){
			goto LABEL_319;
		}
	}
	if( v8 >= 32 - (v94 >> 1) ){
	LABEL_319:
		if( v94 >= 32 ){
			goto LABEL_42;
		}
	LABEL_43:
			cursorOnNear = 1;
		goto LABEL_44;
	}
LABEL_42:
	cursorOnNear = 0;
LABEL_44:
	Cur.ObjectIndex = -1;
	Cur.ItemID = -1;
	int townerIndex = -1;
	Cur.MonsterID = CurMon;
	CurMon = -1;
	Cur.InventoryCellID = -1;
	Cur.playerIndex = -1;
	Tooltip_Clear();
	MaybeNeedDrawInfoPanel = 0;
	IswarpUnderCursor = 0;
	Cur.GroundRow = curRow;
	Cur.GroundCol = curCol;
	if( EasyClick && EnteredPanelWithAutoClick ){
		Cur.Row = curRow;
		Cur.Col = curCol;
	}
	if( ForceMove && IsNoNeedToDropItemUsingCtrl()) {
		Cur.Row = curRow;
		Cur.Col = curCol;
		return;
	}
	if( !player->notHittableInTransition ){
		if( Cur.GraphicsID >= 12 || SelectCurSpellMode ){
			Cur.Row = curRow;
			Cur.Col = curCol;
			return;
		}
		// показываем подсказку если над панелью
		if( CursorInMainPanel() ){
			PopUpHelpOnButton();
			return;
		}
		if( IsPopupScreenOpen ){
		    return;
        }
		
        if( IsPerksPanelVisible && CursorIntoDisplayObject(PerksPanelPos) ){
            PerksPanel_MouseMove();
            return;
        }
		if (IsInfoWindowVisible && CursorIntoDisplayObject(InfoWindowRect)) {
			InfoWindow_MouseMove();
			return;
		}
        if( IsStashPanelVisible && CursorIntoDisplayObject(VisualStashPanel) ){
            StashPanel_MouseMove();
            return;
        }
        if( IsVisualTradePanelVisible && CursorIntoDisplayObject(VisualTradePanel) ){
            VisualTrade_MouseMove();
            return;
        }
        if( IsCraftPanelVisible && CursorIntoDisplayObject(CraftPanel) ){
            Craft_MouseMove();
            return;
        }
		if (IsEnchantPanelVisible && CursorIntoDisplayObject(EnchantPanel)) {
			Enchant_MouseMove();
			return;
		}
		if (IsQuenchPanelVisible && CursorIntoDisplayObject(QuenchPanel)) {
			Quench_MouseMove();
			return;
		}
		if (IsTransmutePanelVisible && CursorIntoDisplayObject(TransmutePanel)) {
			Transmute_MouseMove();
			return;
		}
		if( IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect) ){
            Cur.InventoryCellID = GetSlotMouseAbove_Inv();
            Tooltip_ShowItemInfo(Cur.InventoryCellID);
            return;
        }
        if( IsQUESTPanelVisible && CursorIntoDisplayObject(QuestPanelRect) ){
            return;
        }
        if( IsSpellBookVisible && CursorIntoDisplayObject(SpellBookRect) ){
            return;
        }
        if( IsCHARPanelVisible && CursorIntoDisplayObject(CharPanelRect) ){ // TODO: extract to func, and combine with CalcCharParams / PvM_Melee / PvM_Ranged
            auto& player = Players[CurrentPlayerIndex];
            if( player.AvailableLvlPoints > 0 ){
                const PlayerStat maxStat = GetPlayerMaxStat( CurrentPlayerIndex );
                auto showCurStatTooltip = [&]( const char* name, const int value )
                    {
                        Tooltip_Clear();
                        Tooltip_SetOffsetRight();
                        sprintf(InfoPanelBuffer, "Current %s: %i", name, value );
                        Tooltip_AddLine(InfoPanelBuffer, C_0_White);
                    };
                
                if(      player.BaseStrength  < maxStat.Strength  && CursorIntoDisplayObject(CharPanelCurStrRect) )
                    showCurStatTooltip( "Strength",  player.CurStrength );
                else if( player.BaseMagic     < maxStat.Magic     && CursorIntoDisplayObject(CharPanelCurMagRect) )
                    showCurStatTooltip( "Magic",     player.CurMagic );
                else if( player.BaseDexterity < maxStat.Dexterity && CursorIntoDisplayObject(CharPanelCurDexRect) )
                    showCurStatTooltip( "Dexterity", player.CurDexterity );
                else if( player.BaseVitality  < maxStat.Vitality  && CursorIntoDisplayObject(CharPanelCurVitRect) )
                    showCurStatTooltip( "Vitality",  player.CurVitality );
            }
			if (CursorIntoDisplayObject(CharPanelBaseStrRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				Tooltip_AddLine("strength affects:", C_1_Blue);
				if (not(player.fullClassId, PFC_TRAPPER, PFC_BOMBARDIER, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER,
					PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					if (is(player.fullClassId, PFC_ROGUE, PFC_DRUID) || HasTrait(CurrentPlayerIndex, TraitId::Paladin) || HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
						Tooltip_AddLine("damage in melee combat", C_0_White);
					}
					else if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
						Tooltip_AddLine("damage in melee and ranged combat", C_0_White);
					}
				}
				if ((is(player.ClassID, PC_0_WARRIOR, PC_3_MONK, PC_4_ROGUE, PC_5_SAVAGE) || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) && GameMode != GM_CLASSIC) {
					Tooltip_AddLine("knockback resistance", C_0_White);
				}
				if(is(player.fullClassId, PFC_WARRIOR, PFC_TEMPLAR, PFC_INQUISITOR, PFC_MONK, PFC_SHINOBI, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR)
					&& (!HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger))) {
					Tooltip_AddLine("melee hit block chance", C_0_White);
				}
				Tooltip_AddLine("ability to equip items", C_0_White);
			}
			else if (CursorIntoDisplayObject(CharPanelBaseMagRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();				
				Tooltip_AddLine("magic affects:", C_1_Blue);
				Tooltip_AddLine("maximum mana points", C_0_White);
				Tooltip_AddLine("spell accuracy", C_0_White);
				Tooltip_AddLine("spell damage", C_0_White);	
				if (GameMode != GM_COLISEUM && GameMode != GM_CLASSIC) {
					Tooltip_AddLine("mana regeneration speed", C_0_White);
				}
				if (is(player.fullClassId, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER)) {
					Tooltip_AddLine("summoned minion stats", C_0_White);
				}
				Tooltip_AddLine("ability to equip items", C_0_White);
			}
			else if (CursorIntoDisplayObject(CharPanelBaseDexRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				Tooltip_AddLine("dexterity affects:", C_1_Blue);
				if (not(player.fullClassId, PFC_TRAPPER, PFC_BOMBARDIER, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, 
					PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
						Tooltip_AddLine("accuracy with pistols", C_0_White);
						Tooltip_AddLine("damage with pistols", C_0_White);
					}
					else if (is(player.fullClassId, PFC_ROGUE, PFC_DRUID) || HasTrait(CurrentPlayerIndex, TraitId::Paladin) || HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
						Tooltip_AddLine("accuracy of melee attacks", C_0_White);
					}
					else {
						Tooltip_AddLine("accuracy of melee and ranged attacks", C_0_White);
					}
				}
				Tooltip_AddLine("armor class", C_0_White);
				if (is(player.fullClassId, PFC_WARRIOR, PFC_TEMPLAR, PFC_INQUISITOR, PFC_MONK, PFC_SHINOBI, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR)
					&& (!HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger))) {
					Tooltip_AddLine("melee hit block chance", C_0_White);
					Tooltip_AddLine("arrow hit block chance", C_0_White);
				}
				Tooltip_AddLine("ability to equip items", C_0_White);
			}
			else if (CursorIntoDisplayObject(CharPanelBaseVitRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				Tooltip_AddLine("vitality affects:", C_1_Blue);
				Tooltip_AddLine("maximum hit points", C_0_White);
				if (GameMode != GM_COLISEUM && GameMode != GM_CLASSIC) {
					Tooltip_AddLine("life regeneration speed", C_0_White);
				}
				if (GameMode != GM_CLASSIC) {
					Tooltip_AddLine("stun threshold", C_0_White);
				}
				//Tooltip_AddLine("DFE limit", C_0_White);
				if (!HasTrait(CurrentPlayerIndex, TraitId::Stoneform)) {
					if ((!HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) && GameMode != GM_CLASSIC) {
						Tooltip_AddLine("melee damage resistance", C_0_White);
						Tooltip_AddLine("arrow damage resistance", C_0_White);
					}
					if (GameMode != GM_CLASSIC){
						Tooltip_AddLine("fire damage resistance", C_0_White);
					Tooltip_AddLine("lightning damage resistance", C_0_White);
					Tooltip_AddLine("magic damage resistance", C_0_White);
					Tooltip_AddLine("cold damage resistance", C_0_White);
					Tooltip_AddLine("acid damage resistance", C_0_White);
					}
				}
				Tooltip_AddLine("ability to equip items", C_0_White);
			}

			// --------------------------------------------------------- DAMAGE ---------------------------------------------------
			else if (CursorIntoDisplayObject(CharPanelDamageRect)) { 
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				Tooltip_AddLine("damage affects:", C_1_Blue);
				Tooltip_AddLine("how much damage your attacks do to enemies,", C_0_White); 
				int c = 128 + C_1_Blue;
				if (HasTrait(CurrentPlayerIndex, TraitId::Psion)) {					
					int psionic_dmg = (3 + player.CharLevel * player.CurMagic / 150) + PerkValue(PERK_PSYCHOKINESIS, CurrentPlayerIndex);
					sprintf(InfoPanelBuffer, "Psionic damage: %c%i", c, psionic_dmg);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					if (PerkValue(PERK_AMPLIFY_DAMAGE, CurrentPlayerIndex) > 0) {
						psionic_dmg += psionic_dmg * (PerkValue(PERK_AMPLIFY_DAMAGE, CurrentPlayerIndex)) / 100;
						sprintf(InfoPanelBuffer, "Amplified: %c%i", c, psionic_dmg);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}					
				}
				else if (HasTrait(CurrentPlayerIndex, TraitId::Devastator)) {
					if (PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex)
						+ PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex)
						+ PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex)
						+ PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) > 0) {
						int rad = 3 + (PerkValue(PERK_AURA_RADIUS, CurrentPlayerIndex) / 10) + (PerkValue(SYNERGY_BASTION_OF_ELEMENTS, CurrentPlayerIndex) / 10);
						sprintf(InfoPanelBuffer, "Elemental aura radius: %c%i", c, rad);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					int aura_mastery_boost = PerkValue(SYNERGY_AURA_MASTERY, CurrentPlayerIndex);
					int damageAcid = PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex);
					damageAcid += aura_mastery_boost * damageAcid / 100;
					int damageFire = PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex);
					damageFire += aura_mastery_boost * damageFire / 100;
					int damageArcane = PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex);
					damageArcane += aura_mastery_boost * damageArcane / 100;
					int damageLightning = PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex);
					damageLightning += aura_mastery_boost * damageLightning / 100;
					if (PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex) > 0) {
						sprintf(InfoPanelBuffer, "Fire aura damage: %c%i", c, damageFire);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					if (PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex) > 0) {
						sprintf(InfoPanelBuffer, "Lightning aura damage: %c%i", c, damageLightning);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					if (PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) > 0) {
						sprintf(InfoPanelBuffer, "Arcane aura damage: %c%i", c, damageArcane);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					if (PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex) > 0) {
						sprintf(InfoPanelBuffer, "Toxic aura damage: %c%i", c, damageAcid);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}					
				}
				else {
					if (HasTrait(CurrentPlayerIndex, TraitId::HolyAura)) {
						int holyAuraDamage = 1 + player.CharLevel * player.CurMagic / 40;
						holyAuraDamage += PerkValue(PERK_RIGHTEOUS_ANGER, CurrentPlayerIndex);
						sprintf(InfoPanelBuffer, "Holy aura damage: %c%i", c, holyAuraDamage);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					if (MaxDmgFromChar != MinDmgFromChar) {
						int av_dmg = (MinDmgFromChar + MaxDmgFromChar) / 2;
						sprintf(InfoPanelBuffer, "Average damage: %c%i", c, av_dmg);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}					
					if( Players[CurrentPlayerIndex].effectFlag[EA_MORE_DAMAGE_CHANCE] ){
						sprintf(InfoPanelBuffer, "(RUIN) 30%% chance for +50%% damage: %c%i - %i%c", c, MinDmgFromChar * 15 / 10, MaxDmgFromChar * 15 / 10, c);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					// ---------------------------------------------------------------- DISPLAY OF SPECIES MELEE DAMAGE -----------------------------------------------
					if ((is(player.fullClassId, PFC_WARRIOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_MONK, PFC_SHINOBI, PFC_SHUGOKI, PFC_KENSEI, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN,
						PFC_SAVAGE, PFC_BERSERKER, PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID) || HasTrait(CurrentPlayerIndex, TraitId::Mamluk))
						&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))) && GameMode != GM_COLISEUM
						) {
						int common_dmg = player.ItemsAddDamage + player.BaseDamage;
						int mdvu_min = common_dmg + player.spiciesDamageMin[MON_0_UNDEAD] + player.MinDamageFromItem
							+ (player.MinDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_0_UNDEAD]) / 100);
						int mdvu_max = common_dmg + player.spiciesDamageMax[MON_0_UNDEAD] + player.MaxDamageFromItem
							+ (player.MaxDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_0_UNDEAD]) / 100);
						int mdvd_min = common_dmg + player.spiciesDamageMin[MON_1_DEMON] + player.MinDamageFromItem
							+ player.MinDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_1_DEMON]) / 100;
						int mdvd_max = common_dmg + player.spiciesDamageMax[MON_1_DEMON] + player.MaxDamageFromItem
							+ player.MaxDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_1_DEMON]) / 100;
						int mdvb_min = common_dmg + player.spiciesDamageMin[MON_2_BEAST] + player.MinDamageFromItem
							+ player.MinDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_2_BEAST]) / 100;
						int mdvb_max = common_dmg + player.spiciesDamageMax[MON_2_BEAST] + player.MaxDamageFromItem
							+ player.MaxDamageFromItem * (player.ItemsAddDamagePercents + player.spiciesDamagePercent[MON_2_BEAST]) / 100;
						if (HasTrait(CurrentPlayerIndex, TraitId::Bouncer) && player.OnBodySlots[IS_LeftHand].ItemCode != IC_3_BOW) {
							mdvu_min += mdvu_min * 50 / 100; mdvu_max += mdvu_max * 50 / 100;
							mdvd_min += mdvd_min * 50 / 100; mdvd_max += mdvd_max * 50 / 100;
							mdvb_min += mdvb_min * 50 / 100; mdvb_max += mdvb_max * 50 / 100;
						}
						if (HasTrait(CurrentPlayerIndex, TraitId::Destroyer)) {
							mdvu_min += mdvu_min * 15 / 100; mdvu_max += mdvu_max * 15 / 100;
							mdvd_min += mdvd_min * 15 / 100; mdvd_max += mdvd_max * 15 / 100;
							mdvb_min += mdvb_min * 15 / 100; mdvb_max += mdvb_max * 15 / 100;
						}
						int weaponClass = -1;
						int leftHandItemCode = player.OnBodySlots[IS_LeftHand].ItemCode;
						int rightHandItemCode = player.OnBodySlots[IS_RightHand].ItemCode;
						if (leftHandItemCode == IC_1_SWORD && rightHandItemCode != IC_4_MACE || leftHandItemCode != IC_4_MACE && rightHandItemCode == IC_1_SWORD) { weaponClass = IC_1_SWORD; }
						else if (leftHandItemCode == IC_4_MACE && rightHandItemCode != IC_1_SWORD || leftHandItemCode != IC_1_SWORD && rightHandItemCode == IC_4_MACE) { weaponClass = IC_4_MACE; }
						else if (leftHandItemCode == IC_20_CLAW || rightHandItemCode == IC_20_CLAW) { weaponClass = IC_20_CLAW; }
						else if (leftHandItemCode == IC_21_KNIFE || rightHandItemCode == IC_21_KNIFE) { weaponClass = IC_21_KNIFE; }
						else if (leftHandItemCode == IC_22_MALLET || rightHandItemCode == IC_22_MALLET) { weaponClass = IC_22_MALLET; }
						else if (leftHandItemCode == IC_23_PISTOL || rightHandItemCode == IC_23_PISTOL) { weaponClass = IC_23_PISTOL; }
						else if (leftHandItemCode == IC_10_STAFF || rightHandItemCode == IC_10_STAFF) { weaponClass = IC_10_STAFF; }
						//-----------------------------------------------/// mdvu_min -------------------------------------------
						int damage = mdvu_min; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
								int damageModPercent = 0;
								if (weaponClass == IC_1_SWORD) {
									if (player.fullClassId != PFC_THRAEX) { damageModPercent = -50; }
								}
								else if (weaponClass == IC_4_MACE) {
									damageModPercent = 50;
									if (player.fullClassId == PFC_TEMPLAR) { damageModPercent += 20; }
									else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) { damageModPercent -= 35; }
									else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) { damageModPercent -= 25; }
									else if (player.fullClassId == PFC_KENSEI) { damageModPercent -= 20; }
									else if (player.fullClassId == PFC_GUARDIAN) { damageModPercent -= 10; }
								}
								else if (weaponClass == IC_20_CLAW) { damageModPercent = -20; }
								damage += damage * damageModPercent / 100;
							}
						}
						damage += damage * player.spiciesDamageTotal[MON_0_UNDEAD] / 100;
						damage += damage * PerkValue(PERK_UNDEAD_HUNTER, CurrentPlayerIndex) / 100;
						mdvu_min = damage;
						//-----------------------------------------------/// mdvu_max -------------------------------------------
						damage = mdvu_max; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
								int damageModPercent = 0;
								if (weaponClass == IC_1_SWORD) {
									if (player.fullClassId != PFC_THRAEX) { damageModPercent = -50; }
								}
								else if (weaponClass == IC_4_MACE) {
									damageModPercent = 50;
									if (player.fullClassId == PFC_TEMPLAR) { damageModPercent += 20; }
									else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) { damageModPercent -= 35; }
									else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) { damageModPercent -= 25; }
									else if (player.fullClassId == PFC_KENSEI) { damageModPercent -= 20; }
									else if (player.fullClassId == PFC_GUARDIAN) { damageModPercent -= 10; }
								}
								else if (weaponClass == IC_20_CLAW) { damageModPercent = -20; }
								damage += damage * damageModPercent / 100;
							}
						}
						damage += damage * player.spiciesDamageTotal[MON_0_UNDEAD] / 100;
						damage += damage * PerkValue(PERK_UNDEAD_HUNTER, CurrentPlayerIndex) / 100;
						mdvu_max = damage;
						//-----------------------------------------------/// mdvd_min -------------------------------------------
						damage = mdvd_min; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if (player.fullClassId == PFC_MONK && weaponClass == IC_10_STAFF) { damage += damage / 5; }
						}
						damage += damage * player.spiciesDamageTotal[MON_1_DEMON] / 100;
						damage += damage * PerkValue(PERK_DEMON_HUNTER, CurrentPlayerIndex) / 100;
						mdvd_min = damage;
						//-----------------------------------------------/// mdvd_max -------------------------------------------
						damage = mdvd_max; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if (player.fullClassId == PFC_MONK && weaponClass == IC_10_STAFF) { damage += damage / 5; }
						}
						damage += damage * player.spiciesDamageTotal[MON_1_DEMON] / 100;
						damage += damage * PerkValue(PERK_DEMON_HUNTER, CurrentPlayerIndex) / 100;
						mdvd_max = damage;
						//-----------------------------------------------/// mdvb_min -------------------------------------------
						damage = mdvb_min; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if ((player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID)) {
								if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
									if (weaponClass == IC_4_MACE) { damage -= damage / 2; }
									else if (weaponClass == IC_1_SWORD) {
										if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) { damage += 15 * damage / 100; }
										else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) { damage += 80 * damage / 100; }
										else if (player.fullClassId == PFC_KENSEI) { damage += 30 * damage / 100; }
										else if (player.fullClassId == PFC_GUARDIAN) { damage += 40 * damage / 100; }
										else { damage += damage / 2; }
									}
									else if (weaponClass == IC_20_CLAW) { damage += damage / 5; }
									else if (weaponClass == IC_10_STAFF && player.fullClassId == PFC_MONK) { damage -= 20 * damage / 100; }
								}
							}
						}
						damage += damage * player.spiciesDamageTotal[MON_2_BEAST] / 100;
						damage += damage * PerkValue(PERK_BEAST_HUNTER, CurrentPlayerIndex) / 100;
						mdvb_min = damage;
						//-----------------------------------------------/// mdvb_max -------------------------------------------
						damage = mdvb_max; // ------------------------------------ dmg type
						if (!(player.gameChanger & BIT(GC_10_HACKNSLASH))) {
							if ((player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID)) {
								if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
									if (weaponClass == IC_4_MACE) { damage -= damage / 2; }
									else if (weaponClass == IC_1_SWORD) {
										if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) { damage += 15 * damage / 100; }
										else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) { damage += 80 * damage / 100; }
										else if (player.fullClassId == PFC_KENSEI) { damage += 30 * damage / 100; }
										else if (player.fullClassId == PFC_GUARDIAN) { damage += 40 * damage / 100; }
										else { damage += damage / 2; }
									}
									else if (weaponClass == IC_20_CLAW) { damage += damage / 5; }
									else if (weaponClass == IC_10_STAFF && player.fullClassId == PFC_MONK) { damage -= 20 * damage / 100; }
								}
							}
						}
						damage += damage * player.spiciesDamageTotal[MON_2_BEAST] / 100;
						damage += damage * PerkValue(PERK_BEAST_HUNTER, CurrentPlayerIndex) / 100;
						mdvb_max = damage;
						//-----------------------------------------------///-------------------------------------------
						if( player.effectFlag[EA_RANDOM_DAMAGE] ){//dread
							mdvu_min = 8 * mdvu_min / 10; mdvu_max = 15 * mdvu_max / 10;
							mdvd_min = 8 * mdvd_min / 10; mdvd_max = 15 * mdvd_max / 10;
							mdvb_min = 8 * mdvb_min / 10; mdvb_max = 15 * mdvb_max / 10;
						}
						if (player.gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
							mdvu_min *= 2;	mdvu_max *= 2;
							mdvd_min *= 2;	mdvd_max *= 2;
							mdvb_min *= 2;	mdvb_max *= 2;
						}
						LimitToMin(mdvu_min, 1);
						LimitToMin(mdvd_min, 1);
						LimitToMin(mdvb_min, 1);
						mdvu_max = mdvu_max < mdvu_min ? mdvu_min : mdvu_max;
						if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (mdvu_min == mdvu_max) {
								sprintf(InfoPanelBuffer, "to Undead: %c%i", c, mdvu_min);
							}
							else {
								sprintf(InfoPanelBuffer, "to Undead: %c%i%c - %c%i%c, average %c%i%c", c, mdvu_min, c, c, mdvu_max, c, c, (mdvu_min + mdvu_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvu_min = 1;
							mdvu_max = mdvu_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, "sweep damage to Undead: %c%i%c - %c%i%c, average %c%i%c", c, mdvu_min, c, c, mdvu_max, c, c, (mdvu_min + mdvu_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						mdvd_max = mdvd_max < mdvd_min ? mdvd_min : mdvd_max;
						if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (mdvd_min == mdvd_max) {
								sprintf(InfoPanelBuffer, "to Demons: %c%i", c, mdvd_min);
							}
							else {
								sprintf(InfoPanelBuffer, "to Demons: %c%i%c - %c%i%c, average %c%i%c", c, mdvd_min, c, c, mdvd_max, c, c, (mdvd_min + mdvd_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvd_min = 1;
							mdvd_max = mdvd_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, "sweep damage to Demons: %c%i%c - %c%i%c, average %c%i%c", c, mdvd_min, c, c, mdvd_max, c, c, (mdvd_min + mdvd_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						mdvb_max = mdvb_max < mdvb_min ? mdvb_min : mdvb_max;
						if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (mdvd_min == mdvd_max) {
								sprintf(InfoPanelBuffer, "to Beasts: %c%i", c, mdvb_min);
							}
							else {
								sprintf(InfoPanelBuffer, "to Beasts: %c%i%c - %c%i%c, average %c%i%c", c, mdvb_min, c, c, mdvb_max, c, c, (mdvb_min + mdvb_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvb_min = 1;
							mdvb_max = mdvb_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, "sweep damage to Beasts: %c%i%c - %c%i%c, average %c%i%c", c, mdvb_min, c, c, mdvb_max, c, c, (mdvb_min + mdvb_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (PerkValue(PERK_PIERCING_BULLETS, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, "bullet pierce chance: %c%i%%%c", c, PerkValue(PERK_PIERCING_BULLETS, CurrentPlayerIndex), c);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
							if (PerkValue(PERK_STOPPING_POWER, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, "monster stun threshold overpower: %c%i%%%c", c, PerkValue(PERK_STOPPING_POWER, CurrentPlayerIndex), c);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}
					}
				}
			}
			// -------- END OF DAMAGE -------------
			
			// -------- LIFE REGEN -------------
			else if (CursorIntoDisplayObject(CharPanelLifeRegen) /*&& GameMode != GM_COLISEUM*/) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				//Tooltip_AddLine("life regeneration determines:", C_1_Blue); 
				int c = 128 + C_1_Blue; 
				double lps = 20.00f * (double)HealthRegen / 64.00f;
				sprintf(InfoPanelBuffer, "hit points restored per second: %c%.1f", c, lps ); 
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);			
				EffectAction& ea = player.effectActionValue[EA_LIFE_STEAL];
				if (ea.maxVal > 0) {
					sprintf(InfoPanelBuffer, ea.minVal == ea.maxVal ? "life stolen per melee hit: +%i" : "life stolen per melee hit: +(%i-%i)", ea.minVal, ea.maxVal);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
			}
			// -------- END OF LIFE REGEN -------------

			// -------- MANA REGEN -------------
			else if (CursorIntoDisplayObject(CharPanelManaRegen) /*&& GameMode != GM_COLISEUM*/) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				int c = 128 + C_1_Blue; 
				double mps = 20.00f * (double)ManaRegen / 64.00f;
				sprintf(InfoPanelBuffer, "mana points restored per second: %c%.1f", c, mps);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				EffectAction& ea = player.effectActionValue[EA_MANA_STEAL];
				if (ea.maxVal > 0) {
					sprintf(InfoPanelBuffer, ea.minVal == ea.maxVal ? "mana stolen per melee hit: +%i" : "mana stolen per melee hit: +(%i-%i)", ea.minVal, ea.maxVal);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
			}	
			// -------- END OF MANA REGEN -------------

			// -------- CRIT DMG -------------
			else if (CursorIntoDisplayObject(CharPanelCritDmg)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				int c = 128 + C_1_Blue;

				int minDamage = 0;
				int maxDamage = 0;
				Item* throwable = findThrowableItem(CurrentPlayerIndex);
				if (throwable)// flask / trap damage calculation
				{
					// +damage removed. +damage% applied from all items
					minDamage = throwable->MinDamage + throwable->MinDamage * player.ItemsAddDamagePercents / 100;
					maxDamage = throwable->MaxDamage + throwable->MaxDamage * player.ItemsAddDamagePercents / 100;
					int elem = BaseMissiles[ThrowableMissile(throwable)].DamageType;
					minDamage += minDamage * player.elementPercent[elem] / 100 + player.elementMinDamage[elem];
					maxDamage += maxDamage * player.elementPercent[elem] / 100 + player.elementMaxDamage[elem];
				}
				else
				{
					minDamage = player.ItemsAddDamage + player.BaseDamage + player.ItemsAddDamagePercents * player.MinDamageFromItem / 100 + player.MinDamageFromItem;
					maxDamage = player.ItemsAddDamage + player.BaseDamage + player.ItemsAddDamagePercents * player.MaxDamageFromItem / 100 + player.MaxDamageFromItem;
					if (player.gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
						minDamage *= 2;
						maxDamage *= 2;
					}
					if( player.effectFlag[EA_RANDOM_DAMAGE] ){ // dread
						minDamage = 8 * minDamage / 10;
						maxDamage = 15 * maxDamage / 10;
					}
				}
				LimitToMin(minDamage, 1);
				LimitToMin(maxDamage, 1);
				//maxDamage = maxDamage < minDamage ? minDamage : maxDamage;
				if (maxDamage < minDamage) {
					maxDamage = minDamage;
				}
				int critType = CR_MELEE;
				if (player.OnBodySlots[IS_RightHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].IsReqMet
					&& is(player.OnBodySlots[IS_RightHand].ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL, IC_19_TRAP)) {
					critType = CR_ARROW;
				}
				if (player.OnBodySlots[IS_LeftHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].IsReqMet
					&& is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL, IC_19_TRAP)) {
					critType = CR_ARROW;
				}
				if (player.fullClassId == PFC_SCOUT || player.fullClassId == PFC_INQUISITOR) {
					critType = CR_ELEMENTAL;
				}
				else if ( /*(player.fullClassId == PFC_ASSASSIN && HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery))
					||*/ player.fullClassId == PFC_BOMBARDIER || player.fullClassId == PFC_MAGE
					|| (player.fullClassId == PFC_ELEMENTALIST && (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))
					|| player.fullClassId == PFC_WARLOCK/* || HasTrait(CurrentPlayerIndex, TraitId::Paladin)*/) {
					critType = CR_MAGIC;
				}

				BaseCrit bc = GetBaseCrits(critType, CurrentPlayerIndex);
				int critChance = bc.chance + (bc.levelDiv ? player.CharLevel / bc.levelDiv : 0) + player.critChance;
				int critBonusDamagePercent = bc.percent + player.critPercent;

				int damageMin2 = bc.damageMin + player.critDamageMin;
				int damageMax2 = bc.damageMax + player.critDamageMax;
				int minCritDmg = minDamage + (minDamage * critBonusDamagePercent / 100) + (player.fullClassId == PFC_TRAPPER ? 0 : damageMin2);
				int maxCritDmg = maxDamage + (maxDamage * critBonusDamagePercent / 100) + (player.fullClassId == PFC_TRAPPER ? 0 : damageMax2);
				double acd = ((double)minCritDmg + (double)maxCritDmg) / 2;
				sprintf(InfoPanelBuffer, "average crit damage: %c%.1f", c, acd);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}
			// -------- END OF CRIT DMG -------------
			
			// -------- CRIT CHANCE -------------
			
			else if (CursorIntoDisplayObject(CharPanelCritChance)) {
				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
					BaseCrit bc = GetBaseCrits(CR_MAGIC, CurrentPlayerIndex);
					int cc = bc.chance;
					int c = 128 + C_1_Blue;
					sprintf(InfoPanelBuffer, "spell crit chance: %c%i%%", c, cc);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
				else if (player.fullClassId == PFC_INQUISITOR) {
					BaseCrit bc = GetBaseCrits(CR_ELEMENTAL, CurrentPlayerIndex);
					int cc = bc.chance;
					int c = 128 + C_1_Blue;
					sprintf(InfoPanelBuffer, "weapon elemental damage crit chance: %c%i%%", c, cc);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
			}
			
			// -------- END OF CRIT CHANCE ------

			else if (CursorIntoDisplayObject(CharPanelArmorClass)) { // -------- ARMOR CLASS -----------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
					// ---- main description ----
				Tooltip_AddLine("armor class affects:", C_1_Blue);
				Tooltip_AddLine("percentage of enemy attacks deflected,", C_0_White);
					// -- stun --
				int minST, maxST; 
				tie(minST, maxST) = GetPlayerStunThreshold(player);
				LimitToMin(minST, 0); 
				LimitToMin(maxST, 0);
				int stun_avoid_chance = PerkValue(PERK_RESILIENCE, CurrentPlayerIndex) + PerkValue(PERK_FOOTWORK, CurrentPlayerIndex) + PerkValue(SYNERGY_PERSEVERANCE, CurrentPlayerIndex);
				int c = 128 + C_1_Blue ;
				if (minST == maxST ){ sprintf(InfoPanelBuffer, "Stun Threshold: %c%i", c, minST); }
				else { sprintf(InfoPanelBuffer, "Stun Threshold: %c%i - %i%c", c, minST, maxST, c); }
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				if (stun_avoid_chance > 0) {
					sprintf(InfoPanelBuffer, "Stun avoid chance: %c%i%%", c, stun_avoid_chance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- spell deflect -------
				int playerAC = 0;
				Player_AC(CurrentPlayerIndex, playerAC);
				int spell_deflect_chance = 95 - playerAC / 50 - PerkValue(PERK_SPELL_DEFLECTION, CurrentPlayerIndex);
				Limit_MvP_Ranged_Magical_ToHit_Chance(CurrentPlayerIndex, spell_deflect_chance); // is this line even needed ?
				spell_deflect_chance = 100 - spell_deflect_chance;
				sprintf(InfoPanelBuffer, "Spell deflect chance: %c%i%%", c, spell_deflect_chance);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);

					// ------- palading's mana shield eefficiency -------

				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
					int drMS = 100 - PerkValue(PERK_FORTIFIED_SHIELD, CurrentPlayerIndex);
					sprintf(InfoPanelBuffer, "Mana Shield damage modifier: %c%i%%", c, drMS);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- max block chance (melee) -------
				int maxBlockChance = 0;
				MvP_Melee_Max_Block_Chance(CurrentPlayerIndex, maxBlockChance);
				if (maxBlockChance > 0) {//skip displaying these stats if no blocking is possible
					int a_rose_with_thorns_trait_penalty = HasTrait(CurrentPlayerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
					int two_towers_trait_benefit_1 = HasTrait(CurrentPlayerIndex, TraitId::TwoTowers) ? (player.CharLevel / 6) : 0;
					maxBlockChance +=
						-a_rose_with_thorns_trait_penalty
						+ two_towers_trait_benefit_1
						- PerkValue(SYNERGY_AGGRESSION, CurrentPlayerIndex, 0);
					sprintf(InfoPanelBuffer, "Max block chance vs melee: %c%i%%", c, maxBlockChance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);

					// ------- block rate (melee) -------
					int playerBlockChance = 0;
					//MvP_Melee_Block_Chance(0, playerIndex, playerBlockChance);// mor: not sure monsterIndex is needed here (could even cause issues)				
					playerBlockChance = player.BlockBonus + player.blockChance
						+ ((player.BaseDexterity + player.BaseStrength/* - monster.ArmorClass*/) / 2)
						- a_rose_with_thorns_trait_penalty
						+ two_towers_trait_benefit_1
						- PerkValue(SYNERGY_AGGRESSION, CurrentPlayerIndex, 0);
					LimitToMin(playerBlockChance, 0); 
					//playerBlockChance += PerkValue(PERK_TUCK_TAILS, playerIndex)/* + player.blockChance*/;
					sprintf(InfoPanelBuffer, "Block rating vs melee hits: %c%i", c, playerBlockChance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- block rate (ranged) -------
				int a_rose_with_thorns_trait_penalty_ranged = HasTrait(CurrentPlayerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
				int blockChanceRanged = player.BlockBonus
					+ player.BaseDexterity
					+ player.blockChance
					- a_rose_with_thorns_trait_penalty_ranged
					- PerkValue(SYNERGY_AGGRESSION, CurrentPlayerIndex, 0)
					//+ player.CharLevel
					/* - (monster.ArmorClass / 2)*/;
				if (player.fullClassId == PFC_MURMILLO) {
					blockChanceRanged += player.CharLevel;
				}
				int maxBlockChanceRanged = 0; 
				MvP_Ranged_Max_Block_Chance(CurrentPlayerIndex, maxBlockChanceRanged);
				switch (player.fullClassId) {
				case PFC_IRON_MAIDEN:
				case PFC_WARRIOR:	maxBlockChanceRanged = 70; break;
				case PFC_ASSASSIN:	maxBlockChanceRanged = 20; break;
				default:			maxBlockChanceRanged = 65; break;
				} 
				int finalMaxBlockRate = maxBlockChanceRanged - a_rose_with_thorns_trait_penalty_ranged - PerkValue(SYNERGY_AGGRESSION, CurrentPlayerIndex, 0);
				LimitToMin(blockChanceRanged, 0);
				LimitToMin(finalMaxBlockRate, 0);
				if (is(player.fullClassId, PFC_GUARDIAN, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_DEMONOLOGIST,
					PFC_BEASTMASTER, PFC_KENSEI, PFC_SHUGOKI, PFC_ROGUE, PFC_EXECUTIONER, PFC_BERSERKER, PFC_SAVAGE, PFC_DIMACHAERUS)
					|| HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger)) {
					finalMaxBlockRate = 0;
				}
				if (finalMaxBlockRate > 0) {
					sprintf(InfoPanelBuffer, "Max block chance vs arrows: %c%i%%", c, finalMaxBlockRate);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					sprintf(InfoPanelBuffer, "Block rating vs arrows: %c%i", c, blockChanceRanged);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- autohit (melee) -------
				int autoHit = 20;
				int automiss = 100;
				MvP_Melee_Autohit_chance(CurrentPlayerIndex, autoHit, automiss);
				sprintf(InfoPanelBuffer, "Min chance to get hit (melee): %c%i%%", c, autoHit);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				sprintf(InfoPanelBuffer, "Max chance to get hit (melee): %c%i%%", c, automiss);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					// ------- autohit min-max (arrows) -------
				int Autohit = 14;
				int Automiss = 95;
				int realToHitChance = 0;
				Limit_MvP_Ranged_Arrow_ToHit_Chance(CurrentPlayerIndex, realToHitChance, Autohit, Automiss);
				sprintf(InfoPanelBuffer, "Min chance to get hit (arrows): %c%i%%", c, Autohit);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				sprintf(InfoPanelBuffer, "Max chance to get hit (arrows): %c%i%%", c, Automiss);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}
			// --- END OF ARMOR CLASS ---

			 // -------- TO HIT -----------------
			else if (CursorIntoDisplayObject(charPanelToHit)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				Tooltip_AddLine("to hit chance affects:", C_1_Blue);
				Tooltip_AddLine("accuracy of your attacks against enemies,", C_0_White);
				int c = 128 + C_1_Blue;				
				int hitChance = player.CurMagic + 25;
				switch (player.fullClassId) {
				case PFC_INQUISITOR:	hitChance += 80; break;
				case PFC_DRUID:
				case PFC_SCOUT:
				case PFC_TRAPPER:		hitChance += 50; break;
				case PFC_MAGE:
				case PFC_ELEMENTALIST:
				case PFC_WARLOCK:		hitChance += 25; break;
				case PFC_DEMONOLOGIST:
				case PFC_NECROMANCER:
				case PFC_BEASTMASTER:	hitChance += 10; break;
				case PFC_ROGUE:
				case PFC_ASSASSIN:
				case PFC_BOMBARDIER:	hitChance += 60; break;
				}
				hitChance += (is(GameMode, GM_EASY/*, GM_CLASSIC*/) ? 30 : 0) + PerkValue(PERK_MASTER_CASTER, CurrentPlayerIndex);
				if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {	hitChance += 30;}
				hitChance += PerkValue(PERK_MASTER_CASTER, playerIndex) + PerkValue(PERK_SANCTITY, playerIndex);
				if (HasTrait(playerIndex, TraitId::Paladin)) {	hitChance += 30;}
				sprintf(InfoPanelBuffer, "Spell accuracy rating: %c%i", c, hitChance);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}
			// -------- END OF TO HIT -----------

			else if (CursorIntoDisplayObject(CharPanelResistAcid)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_4_ACID]);
				sprintf(InfoPanelBuffer, "%c%.2f%%%c acid damage resisted", c, player.resistPercent[ET_4_ACID] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistMagic)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_3_ARCAN]);
				sprintf(InfoPanelBuffer, "%c%.2f%%%c arcane damage resisted", c, player.resistPercent[ET_3_ARCAN] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistFire)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_1_FIRE]);
				sprintf(InfoPanelBuffer, "%c%.2f%%%c fire damage resisted", c, player.resistPercent[ET_1_FIRE] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistCold)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_6_COLD]);
				sprintf(InfoPanelBuffer, "%c%.2f%%%c cold damage resisted", c, player.resistPercent[ET_6_COLD] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistLightning)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_2_LIGHTNING]);
				sprintf(InfoPanelBuffer, "%c%.2f%%%c lightning damage resisted", c, player.resistPercent[ET_2_LIGHTNING] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistArrow)) { //  ------------------ ARROW DAMAGE RESIST CELL --------------------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c1 = 128 + (player.resistPercentArrowMin > 0 ? (player.resistPercentArrowMin < player.resistPercentArrowMinLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				int c2 = 128 + (player.resistPercentArrowMax > 0 ? (player.resistPercentArrowMax < player.resistPercentArrowMaxLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				if (player.resistPercentArrowMin == player.resistPercentArrowMax) {
					sprintf(InfoPanelBuffer, "%c%.2f%%%c arrow damage resisted", c1, player.resistPercentArrowMin * 100, c1);
				}
				else {
					sprintf(InfoPanelBuffer, "%c%.2f%%%c to %c%.2f%%%c arrow damage resisted, (avg %c%.2f%%%c)", c1, player.resistPercentArrowMin * 100, c1, c2, player.resistPercentArrowMax * 100, c2, 129, (player.resistPercentArrowMin * 100 + player.resistPercentArrowMax * 100) / 2, 129);
				}
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistMelee)) { //  ------------------ MELEE DAMAGE RESIST CELL --------------------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c1 = 128 + (player.resistPercentMeleeMin > 0 ? (player.resistPercentMeleeMin < player.resistPercentMeleeMinLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				int c2 = 128 + (player.resistPercentMeleeMax > 0 ? (player.resistPercentMeleeMax < player.resistPercentMeleeMaxLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				if (player.resistPercentMeleeMin == player.resistPercentMeleeMax) {
					sprintf(InfoPanelBuffer, "%c%.2f%%%c melee damage resisted", c1, player.resistPercentMeleeMin * 100, c1);
				}
				else {
					sprintf(InfoPanelBuffer, "%c%.2f%%%c to %c%.2f%%%c melee damage resisted, (avg %c%.2f%%%c)", c1, player.resistPercentMeleeMin * 100, c1, c2, player.resistPercentMeleeMax * 100, c2, 129, (player.resistPercentMeleeMin * 100 + player.resistPercentMeleeMax * 100)/2, 129);
				}
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (player.Xp >= 1'000'000'000 && CursorIntoDisplayObject(CharPanelXp)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				PrintBigNum(InfoPanelBuffer, player.Xp, '.');
				Tooltip_AddLine(InfoPanelBuffer, player.CharLevel < 150 ? C_0_White : C_3_Gold);
			}
			else if (player.CharLevel < 150 && player.NextLevelUp >= 1'000'000'000 && CursorIntoDisplayObject(CharPanelXpNext)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				PrintBigNum(InfoPanelBuffer, player.NextLevelUp, '.');
				Tooltip_AddLine(InfoPanelBuffer);
			}
			return;
        }
        
		{
			int curCell = 112 * curRow + curCol;
			{
				if( !(Dungeon->genType || Dungeon->isQuest) ){
					// добавить отображение надписи на могильной плите
					CheckSignes( curRow, curCol ); // signes patch (5)
					if( cursorOnNear ){
						if( curCol < FineMap_112 - 1 ){
							townerNum = MonsterMap[ 0 ][ curCell + 1 ];
							if( townerNum > 0 ){
								townerIndex = townerNum - 1;
								CurMon = townerNum - 1;
								Cur.Row = curRow;
								Cur.Col = curCol + 1;
							}
						}
					}else{
						if( curRow < FineMap_112 - 1 ){
							townerNum = MonsterMap[ 1 ][ curCell ];
							if( townerNum > 0 ){
								townerIndex = townerNum - 1;
								CurMon = townerNum - 1;
								Cur.Row = curRow + 1;
								Cur.Col = curCol;
							}
						}
					}
					townerNum = MonsterMap[ 0 ][ curCell ];
					if( townerNum > 0 ){
						townerIndex = townerNum - 1;
						Cur.Row = curRow;
						CurMon = townerNum - 1;
						Cur.Col = curCol;
					}
					if( curRow < FineMap_112 - 1 && curCol < FineMap_112 - 1 ){
						townerNum = MonsterMap[ 1 ][ curCell + 1 ];
						if( townerNum > 0 ){
							townerIndex = townerNum - 1;
							Cur.Row = curRow + 1;
							CurMon = townerNum - 1;
							Cur.Col = curCol + 1;
						}
					}
					if( townerIndex < 0 || !Towners[ townerIndex ].field_54 ){
						CurMon = -1;
					}
				LABEL_210:
					if( CurMon == -1 ){
						if( !cursorOnNear && curRow < FineMap_112 - 1 ){
							playerNum = PlayerMap[ 1 ][ curCell ];
							if( playerNum ){
								playerIndex = playerNum <= 0 ? -1 - playerNum : playerNum - 1;
								if( playerIndex != CurrentPlayerIndex ){
									if( Players[ playerIndex ].CurLife ){
										Cur.Col = curCol;
										Cur.Row = curRow + 1;
										Cur.playerIndex = check_pi(playerIndex);
									}
								}
							}
						}
						if( cursorOnNear && curCol < FineMap_112 - 1 ){
							playerNum = PlayerMap[ 0 ][ curCell + 1 ];
							if( playerNum ){
								playerIndex = playerNum <= 0 ? -1 - playerNum : playerNum - 1;
								if( playerIndex != CurrentPlayerIndex ){
									if( Players[ playerIndex ].CurLife ){
										Cur.Row = curRow;
										Cur.Col = curCol + 1;
										Cur.playerIndex = check_pi(playerIndex);
									}
								}
							}
						}
						playerNum = PlayerMap[ 0 ][ curCell ];
						if( playerNum ){
							playerIndex = playerNum <= 0 ? -1 - playerNum : playerNum - 1;
							if( playerIndex != CurrentPlayerIndex ){
								Cur.Row = curRow;
								Cur.Col = curCol;
								Cur.playerIndex = check_pi(playerIndex);
							}
						}
						if( FlagMap[ 0 ][ curCell ] & CF_4_DEAD_PLAYER ){
							for( int i = 0; i < PlayersMax_4; ++i ){
								if( Players[i].Col == curRow && Players[i].Row == curCol && i != CurrentPlayerIndex ){
									Cur.Row = curRow;
									Cur.Col = curCol;
									Cur.playerIndex = check_pi(i);
								}
							}
						}
						if( Cur.GraphicsID == CM_8_RISING ){
							for(int row = -1 + (curRow <= 0); row <= 1 - (curRow >= FineMap_112 - 1); ++row ){
								for( int col = -1 + (curCol <= 0); row <= 1 - (curCol >= FineMap_112 - 1); ++col ){
									if( FlagMap[ row ][ col + curCell ] & CF_4_DEAD_PLAYER ){
										for( int i = 0; i < PlayersMax_4; ++i ){
											if( Players[ i ].Col == col && Players[ i ].Row == row && i != CurrentPlayerIndex ){
												Cur.Row = row;
												Cur.Col = col;
												Cur.playerIndex = check_pi( i );
											}
										}
									}
								}
							}
						}
						if( curRow < FineMap_112 - 1 && curCol < FineMap_112 - 1 ){
							playerNum = PlayerMap[ 1 ][ curCell + 1 ];
							if( playerNum ){
								playerIndex = playerNum <= 0 ? -1 - playerNum : playerNum - 1;
								if( playerIndex != CurrentPlayerIndex && Players[ playerIndex ].CurLife ){
									Cur.playerIndex = check_pi(playerIndex);
									Cur.Row = curRow + 1;
									Cur.Col = curCol + 1;
								}
							}
						}
						if( CurMon == -1 ){
							if( Cur.playerIndex != -1 ){
							LABEL_311:
								if( Cur.GraphicsID == 2 ){
									Cur.ObjectIndex = -1;
									Cur.ItemID = -1;
									CurMon = -1;
									Cur.Row = curRow;
									Cur.Col = curCol;
								}
								return;
							}
							if( cursorOnNear ){
								if( curCol < FineMap_112 - 1 ){
									objectNum = ObjectsMap[ 0 ][ curCell + 1 ];
									if( objectNum ){
										objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
										if( Objects[ objectIndex ].selectable >= 2 ){
											Cur.Row = curRow;
											Cur.Col = curCol + 1;
											Cur.ObjectIndex = objectIndex;
										}
									}
								}
							}else{
								if( curRow < FineMap_112 - 1 ){
									objectNum = ObjectsMap[ 1 ][ curCell ];
									if( objectNum ){
										objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
										if( Objects[ objectIndex ].selectable >= 2 ){
											Cur.Col = curCol;
											Cur.Row = curRow + 1;
											Cur.ObjectIndex = objectIndex;
										}
									}
								}
							}
							objectNum = ObjectsMap[ 0 ][ curCell ];
							if( objectNum ){
								objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
								selectable = Objects[ objectIndex ].selectable;
								if( selectable == 1 || selectable == 3 ){
									Cur.Row = curRow;
									Cur.Col = curCol;
									Cur.ObjectIndex = objectIndex;
								}
							}
							if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 1 ){
								objectNum = ObjectsMap[ 1 ][ curCell + 1 ];
								if( objectNum ){
									objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
									if( Objects[ objectIndex ].selectable >= 2 ){
										Cur.ObjectIndex = objectIndex;
										Cur.Row = curRow + 1;
										Cur.Col = curCol + 1;
									}
								}
							}
						}
					}
					if( Cur.playerIndex == -1 && Cur.ObjectIndex == -1 && CurMon == -1 ){
						if( cursorOnNear ){
							if( curCol < FineMap_112 - 1 ){
								itemNum = ItemMap[ 0 ][ curCell + 1 ];
								if( itemNum > 0 ){
									itemIndex = itemNum - 1;
									if( Items[ itemIndex ].animPhase >= 2 ){
										Cur.Row = curRow;
										Cur.Col = curCol + 1;
										Cur.ItemID = itemIndex;
									}
								}
							}
						}else{
							if( curRow < FineMap_112 - 1 ){
								itemNum = ItemMap[ 1 ][ curCell ];
								if( itemNum > 0 ){
									itemIndex = itemNum - 1;
									if( Items[ itemIndex ].animPhase >= 2 ){
										Cur.Col = curCol;
										Cur.Row = curRow + 1;
										Cur.ItemID = itemIndex;
									}
								}
							}
						}
						itemNum = ItemMap[ 0 ][ curCell ];
						if( itemNum > 0 ){
							itemIndex = itemNum - 1;
							char animPhase = Items[ itemIndex ].animPhase;
							if( animPhase == 1 || animPhase == 3 ){
								Cur.Row = curRow;
								Cur.Col = curCol;
								Cur.ItemID = itemIndex;
							}
						}
						if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 1 ){
							itemNum = ItemMap[ 1 ][ curCell + 1 ];
							if( itemNum > 0 ){
								itemIndex = itemNum - 1;
								if( Items[ itemIndex ].animPhase >= 2 ){
									Cur.ItemID = itemIndex;
									Cur.Row = curRow + 1;
									Cur.Col = curCol + 1;
								}
							}
						}
						if( Cur.ItemID == -1 ){
							Cur.Row = curRow;
							Cur.Col = curCol;
							ShowDungeonEntryDescriptions();
							ShowTownPortalDescription();
							ShowUnholyPortalDescription();
							curCol = curMapCol;
						}
					}
					goto LABEL_311;
				}
				if (Dungeon == DUN_2_CHURCH_2 && is(GameMode, GM_IRONMAN, GM_SPEEDRUN))
				{
					CheckMemorialSignes(curRow, curCol);
				}
				if( CurMon == -1 ){
					goto LABEL_320;
				}
				if( cursorOnNear ){
					if( curCol < FineMap_112 - 2 && curRow < FineMap_112 - 1 ){
						monsterNum = MonsterMap[ 1 ][ curCell + 2 ];
						if( monsterNum && FlagMap[ 1 ][ curCell + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( monsterIndex == Cur.MonsterID
								&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
								CurMon = monsterIndex;
								Cur.Row = curRow + 1;
								Cur.Col = curCol + 2;
							}
						}
					}
				}else{
					if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 2 ){
						monsterNum = MonsterMap[ 2 ][ curCell + 1 ];
						if( monsterNum && FlagMap[ 2 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( monsterIndex == Cur.MonsterID
								&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
								CurMon = monsterIndex;
								Cur.Row = curRow + 1;
								Cur.Col = curCol + 2;
							}
						}
					}
				}
				if( curCol < FineMap_112 - 2 && curRow < FineMap_112 - 2 ){
					monsterNum = MonsterMap[ 2 ][ curCell + 2 ];
					if( monsterNum && FlagMap[ 2 ][ curCell + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
						monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
						if( monsterIndex == Cur.MonsterID
							&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
							&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
							&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
							CurMon = monsterIndex;
							Cur.Row = curRow + 2;
							Cur.Col = curCol + 2;
						}
					}
				}
				if( cursorOnNear ){
					if( curCol < FineMap_112 - 1 ){
						monsterNum = MonsterMap[ 0 ][ curCell + 1 ];
						if( monsterNum && FlagMap[ 0 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( monsterIndex == Cur.MonsterID
								&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
								Cur.Row = curRow;
								Cur.Col = curCol + 1;
								CurMon = monsterIndex;
							}
						}
					}
				}else{
					if( curRow < FineMap_112 - 1 ){
						monsterNum = MonsterMap[ 1 ][ curCell ];
						if( monsterNum && FlagMap[ 1 ][ curCell ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( monsterIndex == Cur.MonsterID
								&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
								Cur.Col = curCol;
								Cur.Row = curRow + 1;
								CurMon = monsterIndex;
							}
						}
					}
				}
				monsterNum = MonsterMap[ 0 ][ curCell ];
				if( monsterNum && FlagMap[ 0 ][ curCell ] & CF_64_VISIBLE_BY_CURSOR ){
					monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
					if( monsterIndex == Cur.MonsterID
						&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
						&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
						&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 1 ){
						Cur.Row = curRow;
						Cur.Col = curCol;
						CurMon = monsterIndex;
					}
				}
				if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 1 ){
					monsterNum = MonsterMap[ 1 ][ curCell + 1 ];
					if( monsterNum && FlagMap[ 1 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
						monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
						if( monsterIndex == Cur.MonsterID
							&& (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
							&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
							&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
							CurMon = monsterIndex;
							Cur.Row = curRow + 1;
							Cur.Col = curCol + 1;
						}
					}
				}
				if( CurMon == -1 ){
					goto LABEL_320;
				}
				if( Monsters[ CurMon ].flag & MF_1_INVISIBLE ){
					Cur.Row = curRow;
					CurMon = -1;
					Cur.Col = curCol;
				}
				if( CurMon == -1 ){
				LABEL_320:
					if( cursorOnNear ){
						if( curCol < FineMap_112 - 2 && curRow < FineMap_112 - 1 ){
							monsterNum = MonsterMap[ 1 ][ curCell + 2 ];
							if( monsterNum && FlagMap[ 1 ][ curCell + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
								monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
								if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
									&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
									&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
									CurMon = monsterIndex;
									Cur.Row = curRow + 1;
									Cur.Col = curCol + 2;
								}
							}
						}
					}else{
						if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 2 ){
							monsterNum = MonsterMap[ 2 ][ curCell + 1 ];
							if( monsterNum && FlagMap[ 2 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
								monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
								if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
									&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
									&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
									CurMon = monsterIndex;
									Cur.Row = curRow + 2;
									Cur.Col = curCol + 1;
								}
							}
						}
					}
					if( curCol < FineMap_112 - 2 && curRow < FineMap_112 - 2 ){
						monsterNum = MonsterMap[ 2 ][ curCell + 2 ];
						if( monsterNum && FlagMap[ 2 ][ curCell + 2 ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0 
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 4 ){
								CurMon = monsterIndex;
								Cur.Row = curRow + 2;
								Cur.Col = curCol + 2;
							}
						}
					}
					if( cursorOnNear ){
						if( curCol < FineMap_112 - 1 ){
							monsterNum = MonsterMap[ 0 ][ curCell + 1 ];
							if( monsterNum && FlagMap[ 0 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
								monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
								if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
									&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
									&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
									Cur.Row = curRow;
									Cur.Col = curCol + 1;
									CurMon = monsterIndex;
								}
							}
						}
					}else{
						if( curRow < FineMap_112 - 1 ){
							monsterNum = MonsterMap[ 1 ][ curCell ];
							if( monsterNum && FlagMap[ 1 ][ curCell ] & CF_64_VISIBLE_BY_CURSOR ){
								monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
								if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0
									&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
									&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
									Cur.Col = curCol;
									Cur.Row = curRow + 1;
									CurMon = monsterIndex;
								}
							}
						}
					}
					monsterNum = MonsterMap[ 0 ][ curCell ];
					if( monsterNum && FlagMap[ 0 ][ curCell ] & CF_64_VISIBLE_BY_CURSOR ){
						monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
						if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0 
							&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
							&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 1 ){
							Cur.Row = curRow;
							Cur.Col = curCol;
							CurMon = monsterIndex;
						}
					}
					if( curCol < FineMap_112 - 1 && curRow < FineMap_112 - 1 ){
						monsterNum = MonsterMap[ 1 ][ curCell + 1 ];
						if( monsterNum && FlagMap[ 1 ][ curCell + 1 ] & CF_64_VISIBLE_BY_CURSOR ){
							monsterIndex = monsterNum <= 0 ? -1 - monsterNum : monsterNum - 1;
							if( (int) (Monsters[ monsterIndex ].CurrentLife & 0xFFFFFFC0) > 0 
								&& (!IsPlayerSummon(monsterIndex) || (IsAltPressed || AltHighlight))
								&& Monsters[ monsterIndex ].BasePtr->SelectionOutline & 2 ){
								CurMon = monsterIndex;
								Cur.Row = curRow + 1;
								Cur.Col = curCol + 1;
							}
						}
					}
					if( CurMon != -1 && Monsters[ CurMon ].flag & MF_1_INVISIBLE ){
						Cur.Row = curRow;
						CurMon = -1;
						Cur.Col = curCol;
					}
					goto LABEL_210;
				}
			}
		}
	}
}

//----- (00408BD8) --------------------------------------------------------
void InitDead()
{
	int mtypes[ BM_COUNT ];

    for( int& mtype : mtypes ){
		mtype = 0;
    }
    
	int deadIndex = 0;
	
	for( int i = 0; i < MonsterSpritesCount; ++i){
	    MonsterSprite& sprite = MonsterSprites[i];
	    if( !mtypes[sprite.baseMonsterIndex] && deadIndex < DeadMonsters_Sprites_Max_Count - 1 ){
	        DeadStruct& dd = dead[deadIndex];
	        for( int d = 0; d < 8; ++d ){
				dd.oriented[d] = sprite.animation[ANIM_4_DIE].oriented[d];
            }
            dd.deadFrame = sprite.animation[ANIM_4_DIE].frameCount;
			dd.deadWidth = sprite.Pitch;
			dd.deadWidth2 = sprite.StartOfsX;
			dd.deadTrans = 0;
			dd.baseMonsterIndex = sprite.baseMonsterIndex;
			sprite.deadSpriteNum = ++deadIndex;
			mtypes[sprite.baseMonsterIndex] = sprite.deadSpriteNum;
			
        }
    }
	
	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[i];
		Monster& monster = Monsters[monsterIndex];
	    if( monster.newBossId  && deadIndex < DeadMonsters_Sprites_Max_Count - 1 ){
	        MonsterSprite& sprite = *monster.SpritePtr;
	        DeadStruct& dd = dead[deadIndex];
	        for( int d = 0; d < 8; ++d ){
				dd.oriented[d] = sprite.animation[ANIM_4_DIE].oriented[d];
            }
            dd.deadFrame = sprite.animation[ANIM_4_DIE].frameCount;
			dd.deadWidth = sprite.Pitch;
			dd.deadWidth2 = sprite.StartOfsX;
			dd.deadTrans = monster.uniqTrans + 4;
			dd.baseMonsterIndex = sprite.baseMonsterIndex;
			monster.udeadNum = ++deadIndex;
		}
    }
}

//----- (00408D88) --------------------------------------------------------
void __fastcall AddMonsterCorpse( int row, int col, int monsterSpriteNum, int orient )
{
	DeathMonstersMap[ row ][ col ] = (monsterSpriteNum & 0xff) + (orient << 8); // was uchar num and orient, causing dead array overflow
}

//----- (00408DA9) --------------------------------------------------------
void SyncUniqDead()
{
	// See function InitDead() to understand what this is all about
	for( int i = 0; i < MonstersCount; i++ ){
		Monster& monster = Monsters[MonsterIndexes[i]];
		if( monster.newBossId ){ // (3) расширение номера босса
			for (int col = 0; col < 112; col++) {
				for (int row = 0; row < 112; row++) {
					if( (DeathMonstersMap[row][col] & 0xff) == monster.udeadNum ){ // original was 0x1f, expaned to byte
						ChangeLightPos(monster.LightIndex, row, col);
					}
				}
			}
		}
	}
}

//----- (th2) -------------------------------------------------------------
void SetBossLightActive()
{
	for (int i = 0; i < MonstersCount; i++) {
		Monster& monster = Monsters[MonsterIndexes[i]];
		if (monster.newBossId) {
			//if (/*monster.ActivationCounter == 255*/ true) { //if boss was left during the fight //we check all monsters, not only activeted ones, as it seems that all bosses have issues to find proper placement after reload
			//	ObjectLights[monster.LightIndex].Off = 1; //turn off the old light
			//	NeedLightRecalc = 1;
			//	RecalculateLights();
			//	monster.LightIndex = AddObjectLight(monster.Row, monster.Col, 3); // and give him a new one
			//}

			//above chunk used to be potential fix for bosses light getting misplaced, but in some cases replaced player's light with the boss light
			bool pedestalActivated = false;
			if (IsQuestOnLevel(Q_9_VALOR)) {
				for (int i = 0; i < ObjectsCount; i++)
				{
					if (Objects[i].BaseObjectIndex == BO_73_PEDESTAL_OF_BLOOD) {
						pedestalActivated = Objects[i].maybeFrameIndex == 3;
					}
				}
			}
			int bossie = GameMode == GM_CLASSIC ? UM_C_902_RAGEHORN : UM_677_Guardian_of_Valor;
			if (Monsters[i].newBossId - 1 == bossie) {
				if (pedestalActivated) {
					ChangeLightPos(Monsters[i].LightIndex, Monsters[i].Row, Monsters[i].Col);
				}
			}
			else {
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col); // just move boss light to his current position, without recreating one
			}
			NeedLightRecalc = 1;
			ProcessLightList();
		}
	}
}

//----- (th2) -------------------------------------------------------------
void InitBlood()
{
    for( int i = 0; i < 3; ++i ){
        MissileSprite& sprite = MissileSprites[MIS_10_BLOOD + i];
        for( int d = 0; d < 3; ++d ){
            DeadStruct& bb = blood[i * 3 + d];
            for( auto& o : bb.oriented ){
                o.data = sprite.anim[d];
				o.xOfs = 0;
				o.yOfs = 0;
            }
            bb.deadFrame = sprite.animFrames[d];
            bb.deadWidth = sprite.animWidth[d];
            bb.deadWidth2 = sprite.animOffset[d];
            bb.deadTrans = 0;
        }
    }
    
	// TODO: some strange try to randomize level bloods, have to remove or change to rand
	//for( int row = 0; row < 112; ++row ){
	//    for( int col = 0; col < 112; ++col ){
	//        if( !TileBlockWalking[FineMap[ row ][ col ]] ){
	//            BloodMap[ row ][ col ] = RNG(3) + 6 + 1;
	//        }
	//    }
	//}
}

//----- (th2) -------------------------------------------------------------
void AddBlood( int row, int col, int missileSpriteIndex, int variation )
{
    int i = missileSpriteIndex - MIS_10_BLOOD;
    BloodMap[ row ][ col ] = (i * 3 + variation) + 1;
}

//----- (00408E2D) -------------------------------------------------------- land
void LoadSquareCEL()
{
	if( TestFOPMFlag ){
		SquareCELPtr = (char*)LoadFile("Data\\Square.CEL");
	}
}

//----- (00408E48) -------------------------------------------------------- land
void FreeSquareSprite()
{
	FreeMemZero(SquareCELPtr);
}

#if MULTI_PLAYER_DEBUG_CHECK
//----- (00408E5A) -------------------------------------------------------- land
void CheckMonsterAndPlayerMapsClearForMultiplayer()
{
	int col = 0;
	auto MonsterMapColPtr = MonsterMap[0];
	do{
		int row = 0;
		auto MonsterMapCellPtr = MonsterMapColPtr;
		int CellIndex = col;
		do{
			if( *MonsterMapCellPtr ){
				TerminateWithError("Monsters not cleared"); // TODO: continue here, uncomment before release
			}
			if( PlayerMap[ 0 ][ CellIndex ] ){
				TerminateWithError("Players not cleared"); // TODO: continue here, uncomment before release
			}
			MonsterMapCellPtr += 112;
			int cell = col + 112 * (row + 112 * Dungeon);
			++row;
			CellIndex += 112;
			MultiplayerDebugMap1[cell] = FlagMap[ 0 ][ CellIndex ] & CF_2_VISIBLE_BY_PLAYER;
			MultiplayerDebugMap2[cell] = FlagMap[ 0 ][ CellIndex ] & CF_8_FORBIDDEN;
		}while( row < 112 );
		++MonsterMapColPtr;
		++col;
	}while( (int)MonsterMapColPtr < (int)&MonsterMap[ 0 ][ 112 ] );
}
#endif

//----- (00408EE9) -------------------------------------------------------- land
void FreeDungeonResources()
{
	FreeMemZero(CurrentLevelCELFile);
	FreeMemZero(CurrentLevelTilesFile);
	FreeMemZero(CurrentLevelMINFile);
	FreeMemZero(CurrentLevelSCELFile);

	FreePlayerMissileSprites();
	FreeMonsterResources();
	FreeObjectsSprites();
	FreeMonsterSounds();
	FreeTowners();
}

// return FALSE to quit game
// return TRUE to continue game
//----- (00408F61) -------------------------------------------------------- game
int __fastcall StartGame( int newGame, int singleGame )
{
	gbSelectProvider = 1;

	while( 1 ){

		// initialize network
		bool exitProgram = 0;
		if( !NetInit( singleGame, &exitProgram ) ){
			gbRunGameResult = !exitProgram;
			break;
		}

		gbSelectProvider = 0;

		int msg = 0;
		if( newGame || ! gbValidSaveFile ){

			InitLevels();
			InitQuests();
			InitPortals();
			InitDungMsgs(CurrentPlayerIndex);
			
			if( ! gbValidSaveFile && gbSaveFileExists ){
				// clear the items from the character's inventory
				StripPlayer(CurrentPlayerIndex);
			}
			msg = WM_1034_NEW_GAME;
		}else{
			msg = WM_1035_LOAD_GAME;
		}

		RunGameLoop(msg);
        NetClose();
		
		// in single player mode, exit this loop
		if( MaxCountOfPlayersInGame == 1 ) break;

		// in multiplayer mode, only break out of the
		// loop if the player wants to exit the game, otherwise
		// the loop will be exited based on results from NetInit()
		if( ! gbRunGameResult ) break;
	}

	SNetDestroy();

	return gbRunGameResult;
}

using namespace std::chrono;
auto StartTime = steady_clock::now();
nanoseconds PrevTime{ 0 }, Tick{ 0 };

double TimeSpeed = 1.0;

const double TimeSpeeds[] = { 1.0, 1.25, 1.5 };
int CurTimeSpeedIndex = 0;

//----- (th2) -------------------------------------------------------------
void TimeReset()
{
	TimeSpeed = 1.0;
	CurTimeSpeedIndex = 0;
}

//----- (th2) -------------------------------------------------------------
void TimeChange( double shift )
{
	double prevTimeSpeed = TimeSpeed;
	TimeSpeed += shift;
	LimitToMin( TimeSpeed, 0.01 );
	if( abs(TimeSpeed - 1.0) < 0.001 ){
		TimeSpeed = 1.0;
	}
}


//----- (th2) -------------------------------------------------------------
void TimeChangeIndex( int shiftIndex )
{
	double prevTimeSpeed = TimeSpeed;
	CurTimeSpeedIndex += shiftIndex;
	LimitToRange( CurTimeSpeedIndex, 0, (int)countof(TimeSpeeds) - 1 );
	TimeSpeed = TimeSpeeds[CurTimeSpeedIndex];
}

//----- (th2) -------------------------------------------------------------
void TimeToggle()
{
	if( CurTimeSpeedIndex < countof(TimeSpeeds) - 1 ){
		TimeChangeIndex(1);
	}else{
		TimeReset();
	}
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- new timer with time control ---------------------------------------
i64 GetTickCountTh()
{
	nanoseconds curTime = steady_clock::now() - StartTime;
	i64 delta = (curTime - PrevTime).count();
	PrevTime = curTime;
	if( TimeSpeed != 1.0 && (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL) && MaxCountOfPlayersInGame == 1
	 || DevelopMode) ){
		delta = i64(TimeSpeed * delta);
	}
	Tick += nanoseconds(delta);
	return duration_cast<duration<i64, ratio<1, MAX_FPS>>>(Tick).count();
}

//----- (th2) -------------------------------------------------------------
GAME_MODE_TYPE GameModeType()
{
    switch( GameMode )
    {
    case GM_EASY
      or GM_NORMAL
      or GM_HARD
	  or GM_COLISEUM
	  or GM_CLASSIC:   return GAME_MODE_TYPE::NORMAL;
    case GM_IRONMAN
      or GM_SURVIVAL:  return GAME_MODE_TYPE::IRON;
    case GM_HARDCORE:  return GAME_MODE_TYPE::HARDCORE;
    case GM_SPEEDRUN
      or GM_NIGHTMARE: return GAME_MODE_TYPE::NIGHTMARE;
	default:           return GAME_MODE_TYPE::NORMAL;
	}
}

bool isTownPortalForbidden()
{
	return GameModeType() == GAME_MODE_TYPE::NIGHTMARE || GameModeType() == GAME_MODE_TYPE::IRON;
}

double FramePart;
int DelayIdleRoutine = -1;
bool InitBerserk = true;
BerserkTarget BerserkMonsters[Monsters_Max_Count];
BerserkTarget BerserkObjects[MaxObjects_127];
BerserkTarget BerserkPlayer;

//----- (00409015) -------------------------------------------------------- game
void __fastcall RunGameLoop(int gameMessage) // run_game_loop
{
	static int shiftPressed = IsShiftPressed;
	ResetFrameSyncCritSection(1);
	gbRunGame = 1;
	ClearPrevSessionData(gameMessage);
	ProcHandler prevHandler = SetWindowProc(GameWindowProc);
	RecalcLifeManaGlobes();
	#if CHEATS
	if( FreeSpell ){ FreeSpell = false; void SwitchFreeSpell(); SwitchFreeSpell(); }
	#endif
    RunDeltaInfo();

	gbRunGame = 1;
	BoolWithPlayer = 1;
	gbRunGameResult = 1;
	MainDrawFunc();
	PaletteFadeIn( 8 );
	IsWaitingNetTimeOut_MayBe = 1;
	ResetFrameSyncCritSection(0);
	int v4 = 0;
	bool worldSleep = false;
	bool hasInput = false;

	MSG msg;
	while( gbRunGame ){
		XinputMain();
		AnimateLevelPalette(); // palette cycling

		// if there are any messages in the queue, process them all
		if( PeekMessageA( &msg, 0, 0, 0, PM_NOREMOVE ) ){
			// bump thread priority to make sure that
			// during peekmessage loop we are less likely
			// to yield to another process
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
			while( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) ){
				if( msg.message == WM_QUIT ){
					gbRunGameResult = 0;
					gbRunGame = 0;
					break;
				}
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			bool run = gbRunGame && NThreadRunGameLoop(false);
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
			worldSleep = ! run;
		}else{
			// not enough time has elapsed since last gameloop
			worldSleep = ! NThreadRunGameLoop(false);
		}

		if( !worldSleep ){ // не всегда в SinglePlayer, возможно ждет когда сообщения закончаться
			Player& player = Players[CurrentPlayerIndex];
			if( IsAutoPickup ){
				GoldAutoPickup(CurrentPlayerIndex);
			}
			bool autoClick = EasyClick && ActionClick();
			
			ProcessBerserk(player, autoClick);

			if (autoClick) {
				int isShiftPressed = GetAsyncKeyState(VK_SHIFT) < 0;
				if (ExtraMouseButtons) {
					isShiftPressed |= GetAsyncKeyState(VK_XBUTTON2) < 0;
				}
				if (shiftPressed && !isShiftPressed /*&& MouseButtonPressed != MBP_0_NONE*/) {
					ShiftReleasedEvent = true; // ShiftPressedEvent = false; // убрано потому что на обработать // почему то было убрано, пока верну
				}
				else if (!shiftPressed && isShiftPressed /*&& MouseButtonPressed == MBP_1_LEFT*/) {
					ShiftPressedEvent = true; // ShiftReleasedEvent = false; // убрано чтобы обработалось одноразовое событие, почему то было убрано и было движение вместо удара с шифтом иногда
				}
				shiftPressed = isShiftPressed;
				IsShiftPressed = isShiftPressed;
			}
			CheckPanelClick();
			if (autoClick) {
				if ( /*! LButtonEvent &&*/ player.CurAction != PCA_4_ATTACK && player.CurAction != PCA_5_RANGE_ATTACK /*&& (ShiftPressedEvent || ShiftReleasedEvent)*/) {
					if (TelekinesRecentlyAppliedToMonster == false) {
						//track_repeat_walk( false );
						Repeat_Mouse_LButtonDown();
					}
				}
				// разбираюсь
				if (!RButtonEvent && player.CurAction != PCA_9_SPELL) {
					Repeat_Mouse_RButtonDown();
				}
			}
            
			// actual game logic
			NetReceivePackets();
			IsMainDraw = true;
			InterpolationFrame = 0;
			FramesMax = MaxFPS / ENGINE_FPS; // TODO: set fps auto/max, auto for fullscreen vsync, max for benchmarking no vsync
			FramePart = FramesMax / (double) (MAX_FPS / ENGINE_FPS);
			GameLoop(IsWaitingNetTimeOut_MayBe);
			IsWaitingNetTimeOut_MayBe = 0;
			MainDrawFunc();
			++MainDrawCount; ++AllDrawCount;
			if( MainDrawCount == 21 ){
				MainDrawCount = 1; FPS = AllDrawCount - 1; AllDrawCount = 1;
			}
			IsMainDraw = false;
			MouseMoved = false;
		}else{
			// отрисовка скроллинга экрана до 240 fps ( дополнительная плавность без изменения скорости 20 fps геймплея )
			// сдвиг для активных иконок mana shield, reflect или вообще для всех спеллов
			// Missile.WholeDeltaX, Missile.DeltaX, MissileAction, ManaShieldAction
			i64 tick = GetTickCountTh();
			i64 dt = MainFrameTimer - tick; // MainFrameTime - время следующего кадра (50ms)
			int frameToDraw = int( ((double)(MAX_FPS / ENGINE_FPS) - dt) * FramePart );
			if( InterpolationFrame < frameToDraw && frameToDraw < FramesMax ){
				InterpolationFrame = frameToDraw;
				if( !IsGameMenuActive() && !NetCursorItemId ){ DispatchCursorAboveIsoland(); }
				MainDrawFunc(); ++AllDrawCount;
			}else{
				//if( ! TalkPanelMode ) // медленно набирается текст в чате, но это практически не помогло
				if( LowCpuUsage || MaxFPS < 40 ){
					Sleep(1); // в TH1 15 ms // разгрузка процессора
				}
			}
		}
	}

	// save character in multiplayer mode
	if( MaxCountOfPlayersInGame > 1 
		|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_1_AUTO_SAVE)) 
		|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)) ){
		UpdatePlayerFile();
	}
	ReleasePlayerFile();

	gbRunGame = 2;
	PaletteFadeOut(8);
	gbRunGame = 0;

	SetCursorGraphics(CM_0_INVISIBLE);
	ClearScreen();

	DrawNotGameplayScreen(1);
	
	// restore old window procedure
	SetWindowProc(prevHandler);
	
	FreeGame();
	if( gbDoEnding ){
		gbDoEnding = 0;
		DoEnding();
	}
	
	UberDiabloKilled = false;
	ResetSpeedrunTimer();
	//GameMode = GM_NORMAL; // don't need after adding coliseum (break the menu game mode workflow)
}
//----- (004091B5) -------------------------------------------------------- game
void __fastcall ClearPrevSessionData(int msg)
{
	gbDoEnding = 0;
	UberDiabloKilled = false;
	ResetSpeedrunTimer();
	IsZoomDisable = !ForceZoom;
	InitItemsCELArrays();
	LoadSquareCEL();
	StopMusic();
	StopAmbient();
	ShowProgress(msg);
	Menu_Init();
	ResetCursorOnPlayer();
	NetCursorItemId = 0;
	MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
    TrackInit( false );
}

//----- (00409205) -------------------------------------------------------- game
void FreeGame()
{
	FreePanels();
	FreeInvPanelCEL();
	FreeMenuGraphics();
	FreeSpeechPanelPtrs();
	FreeCELPtr();
	for( uint i = 0; i < PlayersMax_4; ++i ) FreePlayerGFX(i);
	FreeUniqueItemCELFilesPtrs();
	FreeItemsCELArrays();
	FreeSquareSprite();
	FreeDungeonResources();
}

//----- (00409249) -------------------------------------------------------- main
bool NoMyExec()
{
	SetLastError(0);
	CreateEventA(0, 0, 0, "TheHell4Event");
	return GetLastError() != 183; // 0x000000B7
}

//-----  crash minidump functional ----------------------------------------
void CreateMiniDump( EXCEPTION_POINTERS* pep )
{
	// Open the file 

	HANDLE hFile = CreateFile( _T( "Th4CrashDump.dmp" ), GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( (hFile != NULL) && (hFile != INVALID_HANDLE_VALUE) ){
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = pep;
		mdei.ClientPointers = FALSE;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE) (MiniDumpWithFullMemory |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithHandleData |
			MiniDumpWithThreadInfo |
			MiniDumpWithUnloadedModules);

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 );

		if( !rv )
			_tprintf( _T( "MiniDumpWriteDump failed. Error: %u \n" ), GetLastError() );
		else
			_tprintf( _T( "Minidump created.\n" ) );

		// Close the file 


		CloseHandle( hFile );

	} else{
		_tprintf( _T( "CreateFile failed. Error: %u \n" ), GetLastError() );
	}
}

//----- (00409283) -------------------------------------------------------- main
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	__try{
	#ifdef SHOW_CALL_STACK
	void InintShowCallStack(); InintShowCallStack();
	#endif
	InitTH2( &hInstance );

	// ------ Original code start here ---------------------------------------
	#ifdef _PROTECTION
	HINSTANCE prevInst = hInstance;
	MyLoadModule(hInstance);
	hInstance = prevInst;
	#endif
	HInstance = hInstance;
	ShowCursor(0);
	InitializeHashSource();
	AllocPlayers();

	// Исключения в th2 пока не перехватываем
	#ifdef EXCEPTION
	OldTopLevelExceptionFilter = (int(__stdcall*)(struct _EXCEPTION_POINTERS*))SetUnhandledExceptionFilter((LONG(__stdcall*)(struct _EXCEPTION_POINTERS*))TopLevelExceptionFilter);
	#endif
	// Проверки на уже запущенность
	if( !DevelopMode && (AlreadyHasWindow("THEHELL4") || !NoMyExec()) ){ 
		return 0;
	}
	InitOnScreenData();
	if( !*lpCmdLine ){
		char buf[ 256 ];
		buf[ 0 ] = 0;
		FILE* fileGenData = fopen(aGendataDiabvic, "r");
		if( fileGenData ){
			fgets(buf, 255, fileGenData);
			fclose(fileGenData);
		}
	}
	InitSomeBoolFlags();
	CreateMainWindow();
	InitItemsCELArrays(); // first init of item's tables

	MenuSoundInit();
	UiInitialize();
	
	if( IsStartVideo ){
		PlayVideo("gendata\\logo.smk", 1);
		PlayVideo( "gendata\\diablo1.smk", 1);
	}
	BlackPalette();
	InitRandomSeed64(TrueRandom64()); // TODO: add separate seed sequence (items/transmute), may be add it to save
	DisplayMainMenu();

	UiDestroy();
	SavePaletteSettings();

	if( ghMainWnd ){
		// sleep before we destroy window so
		// that SFX have time to finish before exit
		SleepTh(300);
		DestroyWindow(ghMainWnd);
	}
	// ------ Original code end here ---------------------------------------

	ExitTH2();
	} __except( CreateMiniDump( GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER ) {}
	
	return 0;
}

//----- (004093F9) -------------------------------------------------------- game
void InitSomeBoolFlags()
{
	Enable5Class = 1;
	Enable6Class = 1;
	SomeBoolFlag_17 = 1;
	SomeBoolFlag_18 = 1;
	CowQuestOn = 0;
}

//----- (0040945C) --------------------------------------------------------
void __fastcall Character_Passive_Life_and_Mana_Regeneration( int playerIndex ) // life & mana regeneration (passive)
{
	bool UpdateManaRegen = true;
	bool UpdateHealthRegen = true;
	Player& player = Players[playerIndex];
	int lifeAdd = 0;
	int manaAdd = 0;
	int curVitality = player.CurVitality;
	/*if(       curVitality < 16 ){ lifeAdd = (RNG(100) + 1 >= 4 * curVitality + 15 ) ? 0 : 1;
	}else if( curVitality < 32 ){ lifeAdd = (RNG(80) + 1 >= 4 * (curVitality - 16)) ? 1 : 2;
	}else if( curVitality < 48 ){ lifeAdd = (RNG(80) + 1 >= 4 * (curVitality - 32)) ? 2 : 3;
	}else if( curVitality < 64 ){ lifeAdd = (RNG(80) + 1 >= 4 * (curVitality - 48)) ? 3 : 4;
	}else if( curVitality < 80 ){ lifeAdd = (RNG(80) + 1 >= 4 * (curVitality - 64)) ? 4 : 5;
	}else{
		lifeAdd = curVitality >> 4;
	}*/
	
	lifeAdd = HasTrait(playerIndex, TraitId::BlisteredSkin) ? 0 : (curVitality / 20);
	//if (player.fullClassId == PFC_EXECUTIONER) {
		//lifeAdd += lifeAdd*2 + 4; // Executioner gets triple life regen speed
	//}



	switch (player.fullClassId) {
		//case PC_2_MAGE:	if (RNG(3)) { lifeAdd = 0;	UpdateHealthRegen = false; } break;
	case PFC_GUARDIAN: lifeAdd += 1; break;
	case PFC_MONK: lifeAdd += 1;	break;
	case PFC_KENSEI: 
		{
			if (HasTrait(playerIndex, TraitId::BlisteredSkin)) { lifeAdd += (player.MaxCurLife / 25) >> 6; }
			else { lifeAdd += 1; }
		} break;
	case PFC_DRUID:
	case PFC_SHUGOKI: lifeAdd += 2; break;
	case PFC_SHINOBI:
	case PFC_SAVAGE: lifeAdd += 1; break;
	case PFC_BERSERKER: lifeAdd += 1; break;
	case PFC_EXECUTIONER:
		{
			if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {	++lifeAdd; }
			else { lifeAdd += lifeAdd + lifeAdd / 3 + 5; }
		} break;
	}
	
	if (player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)) {
		lifeAdd+=1;
	}

	if( player.effectFlag[EA_LIFE_REGEN_DOUBLE] ){ //double life regen affix flag now calced after class bonuses
		lifeAdd *= 2;
	}
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_13_NO_REGENS)) {
		lifeAdd = 0;
	}
	// DLVL related life regen malus removed
	/*if (DungeonLevel == 22) { // dungeon level modifier to base regeneration (of life)
		lifeAdd -= lifeAdd / 8;
	}
	else if (DungeonLevel == 23) {
		lifeAdd -= lifeAdd / 4;
	}
	else if (DungeonLevel == 24) {
		lifeAdd /= 2;
	}*/

	if (HasTrait(playerIndex, TraitId::BloodOath)) { 
		goto LifeRegenSkippingPlrStances;
	}

	if (HasTrait(playerIndex, TraitId::Adrenaline)) {
		if (player.CurAction) {
			if (not(player.CurAction, PCA_1_WALK_UP, PCA_2_WALK_DOWN, PCA_3_WALK_HORISONTAL)) {
				switch (player.CurAction) {
				case PCA_4_ATTACK:			lifeAdd += 3 + player.CharLevel * 2; break;
				case PCA_5_RANGE_ATTACK:	lifeAdd += 3 + player.CharLevel; break;
				case PCA_6_BLOCK:
				case PCA_7_GOT_HIT: lifeAdd += 3 + player.CharLevel * 5; break;
				case PCA_9_SPELL: lifeAdd += 1 + (lifeAdd / 2); break;
				}
			}
		}
		else {
			lifeAdd -= 3 + player.CharLevel * 2;
		}
	}
	else {
		if (player.CurAction) {
			if (not(player.CurAction, PCA_1_WALK_UP, PCA_2_WALK_DOWN, PCA_3_WALK_HORISONTAL)) {
				switch (player.CurAction) {
				case PCA_4_ATTACK:
				case PCA_5_RANGE_ATTACK:
					if (player.fullClassId == PFC_EXECUTIONER) {
						if (PerkValue(PERK_UNWAVERING_ATTACK, playerIndex) > 0) {
							lifeAdd *= 2;
						}
						else {
							lifeAdd -= lifeAdd / 3;// -33% by default only?... gotta check
						}
					}
					else {
						if (PerkValue(PERK_FIGHTING_STANCE, playerIndex) > 0) {
							lifeAdd *= 2;
						}
						else {
							lifeAdd /= 4;
						}
					}
					break;
				case PCA_6_BLOCK:	
				case PCA_7_GOT_HIT: lifeAdd = 0;
					break;
				case PCA_9_SPELL:
					if (PerkValue(PERK_FIGHTING_STANCE, playerIndex) > 0){
						lifeAdd *= 2;
					}
					else {
						lifeAdd /= 2;
					}
					break;
				}
			}
		}
		else {
			lifeAdd *= 2;
		}
	}
	
	LifeRegenSkippingPlrStances:
	if (!IsSomeMonstersActivated()) {
		lifeAdd += lifeAdd * PerkValue(PERK_REHABILITATION, playerIndex) / 100;
	}
	if (IsSomeMonstersActivated()) {
		lifeAdd += lifeAdd * PerkValue(PERK_BLOOD_DRUNK, playerIndex) / 100 + PerkValue(SYNERGY_PAINKILLER, playerIndex);
	}
	lifeAdd += lifeAdd * player.lifeRegenPercent / 100;
	lifeAdd += player.lifeRegen;

	if( player.effectFlag[EA_LOSE_REGENERATION] && lifeAdd > 0 ) lifeAdd = 0;

	if( Dungeon->isQuest ){
		switch( (DUNGEON)Dungeon ){
		case DUN_38_GLACIAL_CAVERNS: player.fullClassId == PFC_EXECUTIONER ? lifeAdd -= (20 + Difficulty * 8 ) : lifeAdd -= ( 4 + Difficulty * 3 ); break;
		case DUN_39_FROZEN_LOCH:		player.fullClassId == PFC_EXECUTIONER ? lifeAdd -= ( 40 + 15 * Difficulty / 2 ) : lifeAdd -= ( 8 + Difficulty * 4 ); break;
		case DUN_40_ISLAND:			lifeAdd = 2 * lifeAdd + 1; break;
		case DUN_30_HALLS_OF_ANGUISH:
		case DUN_31_ANDARIELS_THRONE: player.fullClassId == PFC_EXECUTIONER ? lifeAdd -= player.CurVitality / 2 : lifeAdd -= player.CurVitality / 4; break; // тренируюсь с тернарным оператором, не факт что правильно
		}
	}	

	if (player.CurLife < player.MaxCurLife * 40 / 100) {
		lifeAdd += PerkValue(PERK_WILL_TO_LIVE, playerIndex);
	}
	
	if (HasTrait(playerIndex, TraitId::Thrill_Seeker)) {
		if (player.CurLife < player.MaxCurLife * 35 / 100) {
			player.xpGainPercent += 50;
		}
		else if (player.CurLife > player.MaxCurLife * 65 / 100) {
			player.xpGainPercent -= 25;
		}
	}

	if (PerkValue(PERK_PATH_OF_BLOOD, playerIndex) >= 1) {
		lifeAdd += player.MaxCurLife / (2000 / PerkValue(PERK_PATH_OF_BLOOD, playerIndex));
	}

	if (GameMode == GM_CLASSIC) {
		lifeAdd = 0 + player.lifeRegen;
	}
	else if (GameMode == GM_COLISEUM) {
		if (Dungeon->genType == DT_0_TOWN) {
			lifeAdd = 1;
		}
		else {
			lifeAdd = 0;
		}
	}

	player.CurLife += lifeAdd;
	player.BaseLife += lifeAdd;
	if( Dungeon->genType == DT_0_TOWN && !Dungeon->isQuest ){
		LimitToMin(player.CurLife, 64);
		LimitToMin(player.BaseLife, 64);
	}

	if (playerIndex == CurrentPlayerIndex) {
		if (UpdateHealthRegen) {
			HealthRegen = lifeAdd;
		}
	}

	if( player.CurLife > player.MaxCurLife ){
		player.CurLife = player.MaxCurLife;
		player.BaseLife = player.MaxBaseLife;
	}
	/*----------------------------- MANA REGEN ------------------------------*/
	int curMagic = player.CurMagic;
	if (player.fullClassId == PFC_DRUID) {
		manaAdd = curMagic / 12 + player.CharLevel / 10;
	}
	else if (player.fullClassId == PFC_ROGUE || ( player.fullClassId == PFC_ASSASSIN && HasTrait ( playerIndex, TraitId::Black_Witchery ) ) ) {
		manaAdd = curMagic / 12;
	}
	else if (HasTrait(playerIndex, TraitId::Feral)) {
		manaAdd = curMagic / 40 + 1;
	}
	else {
		manaAdd = curMagic / 20; 
	}

	if (HasTrait(playerIndex, TraitId::Paladin)) {				
		//manaAdd = curMagic / 15;
		manaAdd += 2;
		if (PerkValue(PERK_PATH_OF_MANA, playerIndex) >= 1) {
			manaAdd += player.MaxCurMana / (2000 / PerkValue(PERK_PATH_OF_MANA, playerIndex));
		}
	}
	
	if( is( player.fullClassId, PFC_MAGE,/* PFC_MONK,*/ PFC_ROGUE)) {
		manaAdd += 2;
	}
	else if (player.fullClassId == PFC_ASSASSIN && HasTrait(playerIndex, TraitId::Black_Witchery)) {
		manaAdd += 3;
	}
	else if (player.fullClassId == PFC_DRUID) { 
		manaAdd += 1; 
	}
	else if( player.fullClassId == PFC_WARLOCK ){
		manaAdd += 4;
	}
	else if( is( player.fullClassId, PFC_ELEMENTALIST/*, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER*/) ){
		manaAdd += HasTrait(playerIndex, TraitId::Mamluk) ? 0 : 1;
	}
	else if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		manaAdd += player.CharLevel / 5;
	}

	if (HasTrait(playerIndex, TraitId::Necropathy)) {
		manaAdd /= 2;	
	}
	else if (HasTrait(playerIndex, TraitId::Psion) || Players[CurrentPlayerIndex].gameChanger & BIT(GC_13_NO_REGENS)) {
		manaAdd = 0;
	}

	if( player.effectFlag[EA_MANA_REGEN_DOUBLE] ){
		manaAdd *= 2;
	}

	if (HasTrait(playerIndex, TraitId::BloodOath)) {
		goto ManaRegenSkippingPlrStances;
	}

	if (player.CurAction) {
		if (not(player.CurAction, PCA_1_WALK_UP, PCA_2_WALK_DOWN, PCA_3_WALK_HORISONTAL)) {
			switch (player.CurAction) {
			case PCA_4_ATTACK:
			case PCA_5_RANGE_ATTACK: 
				if (PerkValue(PERK_FIGHTING_STANCE, playerIndex) > 0) {
					manaAdd *= 2;
				}
				else {
					manaAdd /= 2;
				}
				break;
			case PCA_6_BLOCK:
			case PCA_7_GOT_HIT: manaAdd = 0; break;
			case PCA_9_SPELL:
				if (PerkValue(PERK_TRANQUILITY, playerIndex) > 0 || PerkValue(PERK_FIGHTING_STANCE, playerIndex) > 0) {
					manaAdd *= 2;
				}
				else {
					manaAdd /= 4;
				}
				break;
			}
		}
	}
	else {
		manaAdd *= 2;
	}

	ManaRegenSkippingPlrStances:
	if (!IsSomeMonstersActivated()) {
		manaAdd += manaAdd * PerkValue(PERK_REHABILITATION, playerIndex) / 100;
	}
	manaAdd += manaAdd * player.manaRegenPercent / 100;
	manaAdd += player.manaRegen;

	if( player.effectFlag[EA_LOSE_REGENERATION] && manaAdd > 0 ) manaAdd = 0;

	if( Dungeon->isQuest ){
		switch( (DUNGEON)Dungeon ){
		case DUN_38_GLACIAL_CAVERNS: manaAdd -= 2 + 9 * Difficulty / 4 ; break; // 2 - 20
		case DUN_39_FROZEN_LOCH:		manaAdd -= 4 + 21 * Difficulty / 8; break; // 4 - 25
		case DUN_40_ISLAND:			manaAdd = 2 * manaAdd + 1; break;
		}
	}

	if (GameMode == GM_CLASSIC) {
		manaAdd = 0 + player.manaRegen;
	}
	else if (GameMode == GM_COLISEUM) {
		if (Dungeon->genType == DT_0_TOWN) {
			manaAdd = 1;
		}
		else {
			manaAdd = 0;
		}
	}

	if( player.CurMana <= 0 && manaAdd <= 0 ){
		manaAdd = 0;
	}
	player.CurMana += manaAdd;
	player.BaseMana += manaAdd;
	if (playerIndex == CurrentPlayerIndex) {
		if (UpdateManaRegen) {
			ManaRegen = manaAdd;
		}
	}
	if( player.CurMana > player.MaxCurMana ){
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
	}
	// Fix ManaShied above zero life invulnerability hack
	if( player.CurLife < 64 ) for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_13_MANA_SHIELD && spell.CasterIndex == playerIndex && !spell.IsDeleted ){
			player.CurMana -= 64 - player.CurLife;
			player.CurLife = 64;
		}
	}
}

//----- (0070B550) -------------------------------------------------------- monster
void __fastcall MonsterLifeSteal( int delLife, int monsterIndex, int playerIndex )
{
	Monster& monster = Monsters[monsterIndex];	
	int baseMonster = monster.SpritePtr->baseMonsterIndex;
	/*if (GameMode == GM_CLASSIC) {
		return;
	}*/
	if (monster.CurrentLife > monster.BaseLife) {
		return;
	}
	if (!HasTrait(playerIndex, TraitId::Bloodless)) {
		if (monster.flag & MF_5_TARGET_MONSTER || is(baseMonster,
			BM_0_BURNED_CORPSE, BM_1_OBLIVION_KNIGHT, BM_2_ROTTING_CARCASS,
			BM_6_GRAVE_DIGGER, BM_297_OROG,
			BM_15_TOMB_SLAVE,
			BM_28_BLOOD_WARPER,
			BM_38_UNDEAD_WEREWOLF,
			BM_50_SKELETON_LORD,
			BM_58_DEFILER,
			BM_110_DIABLO,
			BM_126_PLAGUE_EATER,
			BM_137_UBER_DIABLO,
			BM_C_304_BLACK_DEATH,
			BM_C_391_LEORIC)) {
			if (MaxCountOfPlayersInGame != 1) {
				monster.CurrentLife += 3 * delLife / 4;// 75% lifesteal in MP
			}
			else if (is (GameMode, GM_IRONMAN, GM_SPEEDRUN) ){
				monster.CurrentLife += delLife / 5;// 20% lifesteal in SP im+sr
			}
			else {
				monster.CurrentLife += 3 * delLife / 10;// 30% lifesteal in SP
			}
		}
		else if (is(baseMonster, BM_140_GHOUL, BM_169_ZOMBIE)) {
			if (MaxCountOfPlayersInGame != 1) {
				monster.CurrentLife += delLife;// 100% lifesteal in MP
			}
			else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN)) {
				monster.CurrentLife += delLife / 4;// 25% lifesteal in SP im+sr
			}
			else {
				monster.CurrentLife += 7 * delLife / 20;// 35% lifesteal in SP
			}
		}
		else if (is(baseMonster, BM_166_GOATLORD_BOSS, BM_193_ABATAN, BM_194_BLOODIED_HIDDEN)) {
			if (GameMode == GM_NORMAL || GameMode == GM_HARDCORE) {
				monster.CurrentLife += 5 * delLife / 2;// 250% lifesteal
			}
			else if (GameMode == GM_HARD) {
				monster.CurrentLife += 3 * delLife;// 300% lifesteal
			}
			else {
				monster.CurrentLife += 2 * delLife;// 200% lifesteal
			}
		}
		else if (is(baseMonster, BM_215_CADAVER)) {
			if (MaxCountOfPlayersInGame != 1) {
				monster.CurrentLife += 5 * delLife / 4;// 125% lifesteal in MP
			}
			else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN)) {
				monster.CurrentLife += delLife / 3;// 33% lifesteal in SP im+sr
			}
			else if (GameMode == GM_HARD) {				
				monster.CurrentLife += delLife;// 100% lifesteal in SP Hard
			}
			else {
				monster.CurrentLife += delLife / 2;// 50% lifesteal
			}
		}
		LimitToMax(monster.CurrentLife, monster.BaseLife);
	}
}

//----- (004095FC) -------------------------------------------------------- interface
void InitOnScreenData()
{
	CursorX = GUI_PanelWidth;
	CursorY = 240;
	Scroll.dx = 0;
	Scroll.dy = 0;
	Scroll.xOfs = 0;
	Scroll.yOfs = 0;
	Scroll.dir = SDIR_0_NONE;
	ClearOnScreenMessages();
}

//----- (0040964C) -------------------------------------------------------- main
int __stdcall TopLevelExceptionFilter(struct _EXCEPTION_POINTERS* a1)
{
	int result; // eax@2
	ReleaseDDSurface();
	CloseGameArchives(0);
	if( OldTopLevelExceptionFilter )
		result = OldTopLevelExceptionFilter(a1);
	else
		result = 0;
	return result;
}

//----- (0040966E) --------------------------------------------------------
HWND __fastcall AlreadyHasWindow(char* lpClassName)
{
	HWND result; // eax@1
	HWND v2;     // esi@1
	HWND v3;     // eax@3
	HWND v4;     // edi@5
	HWND v5;     // eax@5
	result = FindWindowA(lpClassName, 0);
	v2 = result;
	if( result ){
		v3 = GetLastActivePopup(result);
		if( v3 )
			v2 = v3;
		v5 = GetTopWindow(v2);
		v4 = v5;
		if( !v5 )
			v4 = v2;
		SetForegroundWindow(v2);
		SetFocus(v4);
		result = (HWND)1;
	}
	return result;
}

//----- (004096B1) --------------------------------------------------------
int __fastcall MyLoadModule(HMODULE hModule)
{
	char* v1;                                       // eax@1
	volatile LONG* result;                          // eax@7
	volatile LONG* v3;                              // esi@8
	HWND v4;                                        // eax@17
	DWORD v5;                                       // ST20_4@19
	DWORD* v6;                                      // ST24_4@19
	HWND v7;                                        // eax@21
	char ApplicationName[MAX_PATH];                      // [sp+234h] [bp-188h]@1
	char Name[276];                                 // [sp+120h] [bp-29Ch]@1
	struct _SYSTEM_INFO SystemInfo;                 // [sp+37Ch] [bp-40h]@5
	DWORD hWnd;                                     // [sp+3B8h] [bp-4h]@5
	HANDLE hObject;                                 // [sp+3B0h] [bp-Ch]@7
	DWORD dwProcessId;                              // [sp+3B4h] [bp-8h]@7
	char Dest[276];                                 // [sp+Ch] [bp-3B0h]@8
	struct _STARTUPINFOA StartupInfo;               // [sp+338h] [bp-84h]@10
	struct _PROCESS_INFORMATION ProcessInformation; // [sp+3A0h] [bp-1Ch]@10
	ApplicationName[0] = ZeroString[0];
	memset(&ApplicationName[1], 0, 0x100u);
	*(ushort*)&ApplicationName[257] = 0;
	ApplicationName[259] = 0;
	//strcpy(ApplicationName,"d:\\проекты\\TheHell\\TheHell2\\th.exe");
	GetModuleFileNameA(HInstance, ApplicationName, 0x104u);
	sprintf(Name, "Reload-%s", ApplicationName);
	v1 = Name;
	if( Name[0] ){
		do{
			if( *v1 == 92 )
				*v1 = 47;
			++v1;
		}while( *v1 );
	}
	GetSystemInfo(&SystemInfo);
	hWnd = 4096;
	if( SystemInfo.dwPageSize >= 0x1000 )
		hWnd = SystemInfo.dwPageSize;
	hObject = CreateFileMappingA(INVALID_HANDLE_VALUE, 0, 0x8000004u, 0, hWnd, Name);
	result = (volatile LONG*)GetLastError();
	dwProcessId = result != (volatile LONG*)183; // 0x000000B7
	if( hObject ){
		strcpy(Dest, "Reload-Diablo");
		CreateFileMappingA(INVALID_HANDLE_VALUE, 0, 0x8000004u, 0, hWnd, Dest);
		result = (volatile LONG*)MapViewOfFile(hObject, 0xF001Fu, 0, 0, hWnd);
		v3 = result;
		if( result ){
			if( dwProcessId ){
				*result = -1;
				*(result + 1) = 0;
				memset(&StartupInfo, 0, sizeof(StartupInfo));
				StartupInfo.cb = sizeof(StartupInfo);
				memset(&ProcessInformation, 0, sizeof(ProcessInformation));
				CreateProcessA(ApplicationName, 0, 0, 0, 0, CREATE_NEW_PROCESS_GROUP, 0, 0, &StartupInfo, &ProcessInformation);
				WaitForInputIdle(ProcessInformation.hProcess, 0xFFFFFFFFu);
				CloseHandle(ProcessInformation.hThread);
				CloseHandle(ProcessInformation.hProcess);
				while( *v3 < 0 )
					SleepTh(1000);
				UnmapViewOfFile((const void*)v3);
				CloseHandle(hObject);
				ExitProcess(0);
			}
			if( InterlockedIncrement((long*)result) ){
				v4 = GetForegroundWindow();
				do{
					hWnd = (DWORD)v4;
					v4 = GetWindow(v4, 3u);
				}while( v4 );
				v6 = &dwProcessId;
				v5 = hWnd;
				while( 1 ){
					GetWindowThreadProcessId((HWND)v5, v6);
					if( dwProcessId == *(v3 + 1) )
						break;
					v7 = GetWindow((HWND)hWnd, 2u);
					hWnd = (DWORD)v7;
					if( !v7 )
						goto LABEL_24;
					v6 = &dwProcessId;
					v5 = (DWORD)v7;
				}
				SetForegroundWindow((HWND)hWnd);
LABEL_24:
				UnmapViewOfFile((const void*)v3);
				CloseHandle(hObject);
				ExitProcess(0);
			}
			result = (volatile LONG*)GetCurrentProcessId();
			*(v3 + 1) = (LONG)result;
		}
	}
	return (DWORD)result;
}

//----- (004098C2) -------------------------------------------------------- interface
int EscHandle()
{
	int result = 0;

	if( IsPopupScreenOpen                              ){ ClosePopupScreen();                                     result = 1; }
	if( IsHELPVisible                                  ){ IsHELPVisible = 0;                                      result = 1; }
	if( Speech_IsPanelVisible                          ){ Speech_IsPanelVisible = 0; StopSpeech();      result = 1; }
	else if( CurrentDialogIndex                        ){ PrevTownerDialog();                                     result = 1; }
	if( OnScreenMessage::Head != OnScreenMessage::Tail ){ OnScreenMessage::Queue[OnScreenMessage::Head].time = 0; result = 1; }
	if( TalkPanelMode                                  ){ CloseTalkPanel();                                       result = 1; }
	if( IsGoldSplitPanelVisible                        ){ GoldSplitHandle(VK_27_ESC_KEY);                         result = 1; }
	if( SelectCurSpellMode                             ){ SelectCurSpellMode = 0;                                 result = 1; }
	if( IsStashPanelVisible                            ){ IsStashPanelVisible = false; CloseInventoryPanel();     result = 1; }
    if( IsVisualTradePanelVisible                      ){ VisualTrade_Close();         CloseInventoryPanel();     result = 1; }
    if( IsCraftPanelVisible                            ){ Craft_Close();               CloseInventoryPanel();     result = 1; }
	if( IsEnchantPanelVisible                          ){ Enchant_Close();             CloseInventoryPanel();     result = 1; }
	if( IsQuenchPanelVisible                           ){ Quench_Close();              CloseInventoryPanel();     result = 1; }
	if( IsTransmutePanelVisible                        ){ Transmute_Close();           CloseInventoryPanel();     result = 1; }
    if( IsPerksPanelVisible                            ){ ClosePerksPanel();                                      result = 1; }
	if( IsInfoWindowVisible                            ){ InfoWindow_Close();                                     result = 1; }
	return result;
}

//----- (00409951) --------------------------------------------------------
int __stdcall DisableInputWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		extern int DoMenuScreenShot; DoMenuScreenShot = 1;
	}
	char v5; // zf@16
	if( msg <= WM_SYSKEYDOWN ){
		if( msg != WM_SYSKEYDOWN && (msg < WM_KEYFIRST || msg > WM_CHAR) ){
			return DefaultHandler(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
	if( msg == WM_SYSCOMMAND || msg == WM_MOUSEMOVE ){
		return 0;
	}
	if( msg == WM_LBUTTONDOWN ){
		if( MouseButtonPressed ){
			return 0;
		}
		MouseButtonPressed = MBP_1_LEFT;
		goto LABEL_24;
	}
	if( msg == WM_LBUTTONUP ){
		v5 = MouseButtonPressed == MBP_1_LEFT;
LABEL_20:
		if( v5 ){
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
		}
		return 0;
	}
	if( msg == WM_RBUTTONDOWN ){
		if( MouseButtonPressed ){
			return 0;
		}
		MouseButtonPressed = MBP_2_RIGHT;
LABEL_24:
		SetCapture(hWnd);
		return 0;
	}
	if( msg == WM_RBUTTONUP ){
		v5 = MouseButtonPressed == MBP_2_RIGHT;
		goto LABEL_20;
	}
	if( msg != WM_CAPTURECHANGED ){
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( hWnd != (HWND)lParam ){
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
	}
	return 0;
}

int OldMaxFps = ENGINE_FPS;
__int64 oldPlayerAvailableSpellMask = -1;
short OldSpellLevels[128] = { 0 };

//----- (th21) --------------------------------------------------------
void SwitchFreeSpell()
{
	#if CHEATS
	if( !DevelopMode ) return;
	if( !(oldPlayerAvailableSpellMask == -1 && FreeSpell) ){
		swap( oldPlayerAvailableSpellMask, Players[ CurrentPlayerIndex ].AvailableSpellMask );
	}
	FreeSpell = !FreeSpell;
	if( FreeSpell ){
		for( int i = 0; i < 128; ++i ){
			if( Players[ CurrentPlayerIndex ].SpellLevels[ i ] < (Players[ CurrentPlayerIndex ].CharLevel >> 2) + 3 ){
				OldSpellLevels[ i ] = Players[ CurrentPlayerIndex ].SpellLevels[ i ];
				Players[ CurrentPlayerIndex ].SpellLevels[ i ] = (Players[ CurrentPlayerIndex ].CharLevel >> 2) + 3;
			}
		}
	}else{
		for( int i = 0; i < 128; ++i ){
			if( OldSpellLevels[ i ] ){
				Players[ CurrentPlayerIndex ].SpellLevels[ i ] = OldSpellLevels[ i ];
				OldSpellLevels[ i ] = 0;
			}
		}
	}
	#endif
}

//----- (th2) --------------------------------------------------------
void SwitchGodMode()
{
	#if CHEATS
	if( !DevelopMode ) return;
	IsGodMode = !IsGodMode;
	//if( IsGodMode ){ OneShoot = true; Hedgehog = true; }else{ OneShoot = false; Hedgehog = false; }
	#endif
}

//int CurShrine;
// main window proc, используется в игровом процессе, вызывается как Handler1 в CallHandler1IfExists
//----- (00409A1A) --------------------------------------------------------
int __stdcall GameWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if( SyncTH && ! Th2InMessageQueue ){
	//	PostMessageA( hWnd, msg, wParam, lParam );
	//	return 0;
	//}
	int isGameObject; // eax@29
	//bool res;
	if( msg <= WM_SYSCOMMAND ){
		switch( msg ){
		case WM_SYSCOMMAND:
			if( wParam != SC_CLOSE ) // Closes the window
				return DefaultHandler(hWnd, msg, wParam, lParam);
			gbRunGame = 0;
			gbRunGameResult = 0;
			break;
		case WM_KEYDOWN: // is handled gameplay keys ( 1 ) "mod keys"
		// case WM_SYSKEYDOWN: // TODO: check if need
			if (ExtraMouseButtons) {
				if (wParam == VK_16_SHIFT || wParam & MK_XBUTTON2) {
					IsShiftPressed = true;
				}
				else if (wParam == VK_CAPITAL && DevelopMode) { // CapsLock отключения интерфейса
					IsInterface = !IsInterface;
					//IsInterface ? CursorOff() : CursorOn();
				}
				KeyPressHandler(wParam);
				break;
			}
			else {
				if (wParam == VK_16_SHIFT) {
					IsShiftPressed = true;
				}
				else if (wParam == VK_CAPITAL && DevelopMode) { // CapsLock отключения интерфейса
					IsInterface = !IsInterface;
					//IsInterface ? CursorOff() : CursorOn();
				}
				KeyPressHandler(wParam);
				break;
			}
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if( wParam == VK_16_SHIFT ){
				IsShiftPressed = false;
				if (ExtraMouseButtons) {
					IsShiftPressed |= wParam & MK_XBUTTON2;
				}
				//ShiftReleased = true;
			}
			if( wParam == VK_MENU ){ if( !NeedAltHold ){ AltHighlight = !AltHighlight; } }
			KeyUpHandler( wParam );
			break;
		default:
			if( msg != WM_CHAR ){
				if( msg == WM_SYSKEYDOWN ){
					#if CHEATS
					extern int WeaponIndex, ArmorIndex, UseLookIndex;
					#endif
					switch( wParam ){
					case VK_MENU:
						//if( CurShrine ){
						//	Objects[CurShrine].BaseObjectIndex = BO_60_SHRINE_2;
						//	Objects[CurShrine].ShrineIndex++;
						//	if( Objects[CurShrine].ShrineIndex > 33 ) Objects[CurShrine].ShrineIndex = 0;
						//	Objects[CurShrine].selectable = 1;
						//}
						break; // убирает Idle Sys Menu Loop по Alt (по Alt+Space входит в меню как обычно)
#if CHEATS
					case VK_40_DOWN_ARROW_KEY : { extern volatile int ShiftY; ShiftY++; } break;
					case VK_38_UP_ARROW_KEY   : { extern volatile int ShiftY; ShiftY--; } break;
					case VK_39_RIGHT_ARROW_KEY: { extern volatile int ShiftX; ShiftX++; } break;
					case VK_37_LEFT_ARROW_KEY : { extern volatile int ShiftX; ShiftX--; } break;
					case VK_81_Q_KEY: 
						if (IsDebuggerPresent()) {
							//extern void Out(); Out();
							__debugbreak();
						}
						break; // Alt + Q, breakpoint, works only under visual studio debugger
					case VK_67_C_KEY: // Alt + C cycle armor look for animation testing
						if( DevelopMode && Players[ CurrentPlayerIndex ].CurAction == PCA_0_STAND ){
							if( ++ArmorIndex >= countof(ArmorAnim) ) ArmorIndex = 0; UseLookIndex = 1;
							Players[ CurrentPlayerIndex ].LoadPlayerAnimationMask = PAF_0_NO;
							LoadPlayerGFX( CurrentPlayerIndex, PAF_1_STAY );
							SetPlrAnims( CurrentPlayerIndex );
							ContinueStay( CurrentPlayerIndex );
						} break;
					case VK_88_X_KEY: // Alt + X cycle weapon look for animation testing
						if( DevelopMode && Players[ CurrentPlayerIndex ].CurAction == PCA_0_STAND ){
							if( ++WeaponIndex >= PAW_COUNT ) WeaponIndex = 0; UseLookIndex = 1;
							Players[ CurrentPlayerIndex ].LoadPlayerAnimationMask = PAF_0_NO;
							LoadPlayerGFX( CurrentPlayerIndex, PAF_1_STAY );
							SetPlrAnims( CurrentPlayerIndex );
							ContinueStay( CurrentPlayerIndex );
						} break;
					case VK_85_U_KEY: { // Alt + U, drop of a given unique
						//break;
						int itemIndex = -1;
						//SpawnSomething(2482, Players[CurrentPlayerIndex].Row + 1, Players[CurrentPlayerIndex].Col, 0, 0, 0, 0);
						int uniqId = 4575; // can change
						if( IsGoldSplitPanelVisible ) uniqId = HowMuchGoldYouWantToRemove; // if gold split panel opened - use input as uniq id
						UniqueItem& uniq = UniqueItems[uniqId];
						for( size_t i = 0; i < count_BaseItems; ++i ){ // drop all variants of base with this uniq.uniqLine
							BaseItem& base = BaseItems[i];
							if( uniq.uniqLine == base.uniqLine ){
								itemIndex = SpawnUnique( uniqId, Players[ CurrentPlayerIndex ].Row + 1, Players[ CurrentPlayerIndex ].Col, false, i );
								if( itemIndex != -1 ) Items[itemIndex].Identified = 1;
								//CreateItemIgnoringDungeonLevel(Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, 0, IC_0_OTHER, 2483, 0);
							}
						}
						if( itemIndex == -1 ){ // if not found, drop by uniq.baseId
							itemIndex = SpawnUnique( uniqId, Players[ CurrentPlayerIndex ].Row + 1, Players[ CurrentPlayerIndex ].Col );
							if( itemIndex != -1 ) Items[itemIndex].Identified = 1;
						}
						break;
					}
                    case VK_75_K_KEY: { // Alt + K, drop of a given line of unique items
                        for( size_t i = 0; i < count_UniqueItems; ++i ){
                            UniqueItem& uniq = UniqueItems[i];
                            if( uniq.uniqLine == HowMuchGoldYouWantToRemove ){
                                int itemIndex = -1;
                                itemIndex = SpawnUnique( i, Players[ CurrentPlayerIndex ].Row + 1, Players[ CurrentPlayerIndex ].Col );
                                if( itemIndex != -1 ) Items[itemIndex].Identified = 1;
                            }
                        }
                        break; }
                    case VK_76_L_KEY: { // Alt + L, drop of a given line of set uniques
                        for( size_t i = 0; i < count_UniqueItems; ++i ){
                            UniqueItem& uniq = UniqueItems[i];
                            if( uniq.uniqueSetIndex == HowMuchGoldYouWantToRemove ){
                                int itemIndex = -1;
                                itemIndex = SpawnUnique( i, Players[ CurrentPlayerIndex ].Row + 1, Players[ CurrentPlayerIndex ].Col );
//                                if( itemIndex != -1 ){
//                                    ItemsOnGround[itemIndex].Identified = 1;
//                                }
                            }
                        }
                        break; }
					case VK_66_B_KEY: { // Alt + B, summon the specified boss
						break;
						extern int BossRow, BossCol;
						BossRow = Players[ CurrentPlayerIndex ].Row + 2;
						BossCol = Players[ CurrentPlayerIndex ].Col;
						int um = UM_11_The_Defiler;
						if( AddMonsterTypeToDungeon( UniqueMonsters[um].BaseMonsterIndex, MPF_4_UNIQ ) > -1 ){
							CreateBoss( um, 0, 0 );
						}
						break; }
					case VK_78_N_KEY:
						NextPlayerLevel(CurrentPlayerIndex);
						if( Players[ CurrentPlayerIndex ].Xp < LevelUp( Players[ CurrentPlayerIndex ].CharLevel - 1 ) ){
							Players[ CurrentPlayerIndex ].Xp = LevelUp( Players[ CurrentPlayerIndex ].CharLevel - 1 );
						}
						if( Players[ CurrentPlayerIndex ].CharLevel == PlayerMaxLevel() ){
							Players[ CurrentPlayerIndex ].Xp = LevelUp( PlayerMaxLevel() - 1 );
						}
						break; // Alt + N, level up
					case VK_65_A_KEY: // Alt + A
						//DotAcidMechanic = !DotAcidMechanic;
						//IsAutoPickup = !IsAutoPickup; //disabling/enabling auto-selection (for now, only gold)
						break;
					case VK_71_G_KEY: // Alt + G, disable/enable immortality
						SwitchGodMode();
						break;
					case VK_83_S_KEY: // Alt + S, disable/enable free spells
						SwitchFreeSpell();
						break;
					case VK_70_F_KEY: // Alt + F, Disabling/enabling FPS increase
						//Players[0].OnBodySlots[4].affixFlag = Players[0].itemAffixFlag ? 0 : AF_21_FAST_ATTACK_SPEED_4;
						//Players[0].itemAffixFlag = Players[0].itemAffixFlag ? 0 : AF_21_FAST_ATTACK_SPEED_4;
						swap( OldMaxFps, MaxFPS );
						break;
					case VK_69_E_KEY: // Alt + E, easy click switch
						// break; // Sometimes there's a problem with shift events, I'm figuring it out
						EasyClick = !EasyClick;
						break;
					case VK_84_T_KEY: // Alt + T tile check mode
						IsShowTileNum = !IsShowTileNum;
						break;
					case VK_86_V_KEY: // Alt + V drop of a given item
						void PutStartItemToInventory(int playerIndex, int baseItemIndex);
						PutStartItemToInventory( CurrentPlayerIndex, HowMuchGoldYouWantToRemove );
						//PutStartItemToInventory( CurrentPlayerIndex, 401 );
						//PutStartItemToInventory( CurrentPlayerIndex, 402 );
						break;
					case VK_73_I_KEY: // Alt + I goto next dungeon
						{DUNGEON next = DUN_28_POISONED_WATER_SUPPLY;//Dungeon + 1;
						StartNewLvl( CurrentPlayerIndex, next > DUN_24_CRYPT_4 ? WM_1029_GOTO_QUEST_DUNGEON : WM_1031_GOTO_DUNGEON, next);
						break;}
					case VK_68_D_KEY: // Alt + D goto dungeon
						{DUNGEON next = (DUNGEON)HowMuchGoldYouWantToRemove;
						StartNewLvl( CurrentPlayerIndex, next > DUN_24_CRYPT_4 ? WM_1029_GOTO_QUEST_DUNGEON : WM_1031_GOTO_DUNGEON, next );
						break;}
					case VK_87_W_KEY: // Alt + W, switching light table generation methods
						OriginalLightLevels = !OriginalLightLevels;
						MakeLightTable();
						break;
					case VK_48_0_KEY: // Alt + 0 Add 1 000 000 gold
						SpawnSomething(BI_0_GOLD, PlayerRowPos, PlayerColPos, 0, 0, 0, 0, 1'000'000);
						break;
					case VK_82_R_KEY: //Alt + R test spell book drop randomness
						if( CreateSpellBook( PlayerRowPos, PlayerColPos, -123, 0, 1 ) == -1 ){
							ofstream gen("gen_"s + to_string(CurItemGenVersion), ios_base::app);
							struct Sp { int spell; int count;} sp[PS_COUNT]; int sps = 0;
							for( int i = 0; i < MaxItems_255; ++i ){
								Item& item = Items[i];
								if( item.MagicCode == MC_24_BOOKS ){
									bool found = false;
									for( int s = 0; s < sps; ++s ){
										if( sp[s].spell == item.SpellIndex ){
											++sp[s].count;
											found = true;
											break;
										}
									}
									if( ! found ){
										sp[sps].spell = item.SpellIndex;
										sp[sps].count = 1;
										++sps;
									}
								}
							}
							sort(sp, sp+sps, [](Sp& a, Sp& b) { return a.spell < b.spell; });
							for( int s = 0; s < sps; ++s ){
								gen << sp[s].count << ",0" << sp[s].spell << " ";
							}
							gen << endl;
							InitItems();
							memset(ItemMap, 0, sizeof( ItemMap ));
						}
						break;
						//free key to bind
#endif
					case VK_90_Z_KEY: // Alt + Z, переключение zoom
						if( !ForceZoom ) IsZoomDisable = !IsZoomDisable;
						break;
					//case VK_82_R_KEY: // Alt + Shift + R, Resetting the image output window //mor: under cheats, this gives an error and doesn't compile!
					//	if( IsShiftPressed ){ SwitchFullscreen(); SwitchFullscreen(); }
					//	break;
					default:
						if( CheckF10Message(wParam) ) return 0; // is handled gameplay keys ( 2 ) F10 ?
						return DefaultHandler(hWnd, msg, wParam, lParam);
					}
					return 0;
				}
				return DefaultHandler(hWnd, msg, wParam, lParam);
			}
			HandleCharHotkey(wParam); // is handled gameplay keys ( 3 ) "classic keys"
			break;
		}
		return 0;
	}
	if( msg > WM_CAPTURECHANGED ){
		if( msg >= WM_1026_NEXT_DUNGEON && msg <= WM_1033_RESTART_IN_TOWN ){
			if( MaxCountOfPlayersInGame > 1 ){
				UpdatePlayerFile();
			}
			ResetFrameSyncCritSection(1);
			PaletteFadeOut(8);
			StopAllSounds();
            TrackInit( false );
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
			ShowProgress(msg);
			MainDrawFunc();
			if( gbRunGame )
				PaletteFadeIn(8);
			ResetFrameSyncCritSection(0);
			IsWaitingNetTimeOut_MayBe = 1;
			return 0;
		}
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg == WM_CAPTURECHANGED ){
		if( SyncTH ){
			if( lParam & 0x1000'0000 ){
				lParam &= ~0x1000'0000;
			}else{
				return DefaultHandler(hWnd, msg, wParam, lParam);
			}
		}
		if( hWnd != (HWND)lParam ){
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
            TrackInit( false );
		}
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg == WM_MOUSEMOVE ){
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		SetCursorPosition(hWnd, lParam);
		Menu_ItemMouseClick((unsigned __int16)lParam); ///////////////////////////////
		MouseMoved = true;
		//return 0;
	}
	if( msg == WM_MBUTTONDOWN ){
	    ChangeAltWeaponPairToNext( !IsINVPanelVisible );
    }
	if( msg == WM_LBUTTONDOWN ){
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		SetCursorPosition(hWnd, lParam);
		if( MouseButtonPressed == MBP_0_NONE ){
			MouseButtonPressed = MBP_1_LEFT;
			if( IsShiftPressed ){
				ShiftReleasedEvent = false;
			}else{
				ShiftPressedEvent = false;
			}
			SetCapture(hWnd);
			LButtonEvent = true;
			isGameObject = Mouse_LButtonDown(wParam); // в wParam передается состояние шифта 
			if (Berserk) {				
				TrackInit(false);
			}
			else {
				TrackInit(isGameObject);
			}
		}else if( EasyClick && MouseButtonPressed == MBP_2_RIGHT ){
			OldMouseButtonPressed = MBP_2_RIGHT;
			MouseButtonPressed = MBP_1_LEFT;
			if( IsShiftPressed ){
				ShiftReleasedEvent = false;
			}else{
				ShiftPressedEvent = false;
			}
			SetCapture( hWnd );
			LButtonEvent = true;
			isGameObject = Mouse_LButtonDown(wParam); // в wParam передается состояние шифта 
			if (Berserk) {
				TrackInit(false);
			}
			else {
				TrackInit(isGameObject);
			}
		}
		return 0;
	}
	if( msg == WM_LBUTTONUP ){
		TelekinesRecentlyAppliedToMonster = false;
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		SetCursorPosition(hWnd, lParam);
		if( EasyClick && OldMouseButtonPressed == MBP_1_LEFT && MouseButtonPressed == MBP_2_RIGHT ){
			OldMouseButtonPressed = MBP_0_NONE;
			OnMouseLButtonUp();
            TrackInit( false );
			return 0;
		}else if( EasyClick && OldMouseButtonPressed == MBP_2_RIGHT && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_0_NONE;
			MouseButtonPressed = MBP_2_RIGHT;
			//SetCapture( hWnd );
            TrackInit( false );
			Mouse_RButtonDown(0);
			return 0;
		}else if( MouseButtonPressed != MBP_1_LEFT ){
			return 0;
		}
		ShiftReleasedEvent = false;
		ShiftPressedEvent = false;
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
		OnMouseLButtonUp();
        TrackInit( false );
		ReleaseCapture();
		return 0;
	}
	if( msg == WM_RBUTTONDOWN ){
		if( CurrentMenuWindowPtr ){ // правая кнопка в меню в игре
			PlayGlobalSound( S_75_I_TITLEMOV );
			if( CurrentMenuWindowPtr == OptionsMenu ){
				SelectMainMenu(); // выход в главное меню
			}else{
				Menu_SetupMenuWindow( 0, 0 ); // закрытие меню
			}
			return 0;
		}
		if( CurrentDialogIndex ){ // по правой кнопке возрат в диалоге назад (как Esc)
			EscHandle();
			return 0;
		}
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		SetCursorPosition(hWnd, lParam);
		if( !MouseButtonPressed ){
			MouseButtonPressed = MBP_2_RIGHT;
			SetCapture(hWnd);
			Mouse_RButtonDown(0);
		}else if( EasyClick && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_1_LEFT;
			MouseButtonPressed = MBP_2_RIGHT;
			RButtonEvent = true;
			if( IsShiftPressed ){
				ShiftPressedEvent = true; //ShiftReleasedEvent = false;
			}else{
				ShiftReleasedEvent = true; //ShiftPressedEvent = false;
			}
			SetCapture( hWnd );
            TrackInit( false );
			if( ! Mouse_RButtonDown(1) ){
				RButtonEvent = false;
			}
		}
		return 0;
	}
	if( msg == WM_RBUTTONUP ){
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		SetCursorPosition(hWnd, lParam);
		if( EasyClick && OldMouseButtonPressed == MBP_1_LEFT && MouseButtonPressed == MBP_2_RIGHT ){
			MouseButtonPressed = MBP_1_LEFT;
			OldMouseButtonPressed = MBP_0_NONE;
			//RButtonEvent = false;
			if( ! RButtonEvent ){
				//ShiftReleasedEvent = false;
				//ShiftPressedEvent = false;
				isGameObject = Mouse_LButtonDown( wParam ); // в wParam передается состояние шифта 
                TrackInit( isGameObject );
			}
			return 0;
		}else if( EasyClick && OldMouseButtonPressed == MBP_2_RIGHT && MouseButtonPressed == MBP_1_LEFT ){
			OldMouseButtonPressed = MBP_0_NONE;
			return 0;
		}else if( MouseButtonPressed == MBP_2_RIGHT ){
			MouseButtonPressed = MBP_0_NONE;
		}
		ReleaseCapture();
		return 0;
	}
	if( msg == WM_MBUTTONDOWN ){
		if( CurrentDialogIndex ){
			if( FirstVisibleItem < ItemCount - 1 && SelectedDialogRow != BottomSelectedRow ){
				while( DialogPageDown() ); // вниз списка
			}else{
				while( DialogPageUp() ); // вверх списка
			}
		}
	}
	if( msg == WM_MOUSEWHEEL ){
		IsShiftPressed = wParam & MK_SHIFT;
		if (ExtraMouseButtons) {
			IsShiftPressed |= wParam & MK_XBUTTON2;
		}
		OnMouseWheel(wParam);
		return 0;
	}
	RestrictMouseToWindow(hWnd);
	return DefaultHandler(hWnd, msg, wParam, lParam);
}

//----- (00409C92) -------------------------------------------------------- interface
 int __fastcall Mouse_LButtonDown(int wParam)
{
	if( ForceMove && IsNoNeedToDropItemUsingCtrl()) {
		return true;
	}
	if( Menu_CheckMouseClick(MOUSE_BUTTON_DOWN) || TalkPanel_CheckMouseClick() || NetCursorItemId ){
		return 0;
	}
	if( IsPlayerDead ){ // Какое то действие с мэссэдж панелью
		Check_MenuORSendMessage();
		return 0;
	}
	if( GamePaused == 2 ){ // Если поставили на паузу, ЛКМ для дальнейших действий не обрабатываем
		return 0;
	}
	if( IsPopupScreenOpen ){
		ClosePopupScreen();
		return 0;
	}
	if( SelectCurSpellMode ){ // Если сейчас выбор заклинания, пробуем выбрать. (даже если нажатие будет не по иконке, любое нажатие закроет выбор заклинания)
		SelectCurrentSpell();
		return 0;
	}
	if( CurrentDialogIndex ){ // Если болтаем с неписями, проверяем куда попали в их диалоге
		if( CheckDialogMouseClick() ) return 0;
	}
	if( CursorInMainPanel() ){
		if( /*Berserk*/HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
			return 0;
		}
		if( !TalkPanelMode && !IsGoldSplitPanelVisible && !IsGameMenuActive() ){
			if( CursorIntoBelt() ) Inventory_LButtonClick();
		}
		CheckMainPanelRegion();
		if( Cur.GraphicsID > CM_1_NORMAL_HAND && Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
			SetCursorGraphics(CM_1_NORMAL_HAND);
		}
		return 0;
	}
	if( IsGameMenuActive() || CursorSkillApply() ){
		return 0;
	}
	if( IsQUESTPanelVisible && CursorIntoDisplayObject(QuestPanelRect) ){
	    if( CursorIntoDisplayObject(QuestPanelTextAreaRect) ){
		    QuestPanel_CheckLButtonClick();
        }
		return 0;
	}
	if( Speech_IsPanelVisible ){
		Speech_IsPanelVisible = 0;
		StopSpeech();
		return 0;
	}
	if( IsCHARPanelVisible && CursorIntoDisplayObject(CharPanelRect) ){
        CharPanel_MouseDown();
		return 0;
	}
	if( IsStashPanelVisible && CursorIntoDisplayObject(VisualStashPanel) ){
        Stash_LButtonDown();
		return 0;
	}
	if (IsEnchantPanelVisible && CursorIntoDisplayObject(EnchantPanel)) {
		Enchant_LButtonDown();
		return 0;
	}
	if (IsQuenchPanelVisible && CursorIntoDisplayObject(QuenchPanel)) {
		Quench_LButtonDown();
		return 0;
	}
	if (IsTransmutePanelVisible && CursorIntoDisplayObject(TransmutePanel)) {
		Transmute_LButtonDown();
		return 0;
	}
	if( IsVisualTradePanelVisible && CursorIntoDisplayObject(VisualTradePanel) ){
        VisualTrade_LButtonDown();
		return 0;
	}
	if( IsCraftPanelVisible && CursorIntoDisplayObject(CraftPanel) ){
        Craft_LButtonDown();
		return 0;
	}
	if( IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect) ){
		if( /*Berserk*/HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
			return 0;
		}
		if( !IsGoldSplitPanelVisible ){
			Inventory_LButtonClick();
		}
		return 0;
	} 
	if( IsSpellBookVisible && CursorIntoDisplayObject(SpellBookRect) ){
		BookPanel_CkeckLButtonClick();
		return 0;
	}
	if( IsPerksPanelVisible && CursorIntoDisplayObject(PerksPanelPos) ){
		HandlePerksWindowClick();
		return 0;
	}
	if (IsInfoWindowVisible && CursorIntoDisplayObject(InfoWindowRect)) {
		InfoWindow_MouseDown();
		return 0;
	}
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){ //  если к курсору подвешена шмотка
		if( DropCursorItemBeforeTrig() ){
			FixItemCursor(-1);
		}
		return 0;
	}
    if( getPlayerPerksPointsLeft(PERK_TYPE_COMMON) > 0 && !SelectCurSpellMode ){
        CheckPerkUpButton();
    }
	if( Players[CurrentPlayerIndex].AvailableLvlPoints && !SelectCurSpellMode ){
		CheckLevelUpButton();
	}
	if( !IsCursorOnLevelUpButton && !IsCursorOnPerkUpButton ){
		int isoLand = IsoLand_CheckLButtonClick((wParam & MK_SHIFT) /*wParam == 5*/ /* нажат Shift */, 0); // клик на игровом поле ( передвижение, атака, выстрел )
		if (ExtraMouseButtons) {
			isoLand = IsoLand_CheckLButtonClick((wParam & MK_SHIFT) || (wParam & MK_XBUTTON2), 0);
		}
		if( IsStashPanelVisible || IsVisualTradePanelVisible || IsCraftPanelVisible || IsEnchantPanelVisible || IsQuenchPanelVisible || (IsTransmutePanelVisible && !IsShiftPressed) ){
		    CloseInventoryPanel();
        }
		return isoLand;
	}
	return 0;
}

// return 1 это клик на пустой пол (для простого движения)
//----- (00409E8C) -------------------------------------------------------- interface
int __fastcall IsoLand_CheckLButtonClick(int shiftPressed, int repeat)
{
	Player& player = Players[CurrentPlayerIndex];
	if (Berserk) {
		return 0;
	}
	if( ForceMove ) {
		return true;
	}
	int itemId = AltOnlyItemPick && ! IsAltPressed ? -1 : Cur.ItemID;
	if( (IsAltPressed || AltHighlight) && HighlightedItem.ItemID != -1 && !repeat ){ // Mouse over highlighted item
        NetSendCmdLocParam1( 1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, HighlightedItem.Row, HighlightedItem.Col, HighlightedItem.ItemID );
		ShiftReleasedEvent = false; // try to fix not getting item after shift
	}else if( !(Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_20_RAVENHOLM*/) ){ // Если город
		if( itemId != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !repeat ){
            NetSendCmdLocParam1( 1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, Cur.Row, Cur.Col, itemId ); // Поднятие вещи с пола города. Работает с шифтом тоже
			ShiftReleasedEvent = false; // try to fix not getting item after shift
		}else if( CurMon != -1 && !repeat ){
            NetSendCmdLocParam1( 1, CMD_29_TALKXY, Cur.Row, Cur.Col, CurMon ); // Клик на горожанина
		}else{
			return all( -1, itemId, CurMon, Cur.playerIndex ); // движение если пол
		}
	}else if( itemId != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !shiftPressed && !repeat){ // Клик на вещь лежащую на полу подземелья
		NetSendCmdLocParam1( 1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, Cur.Row, Cur.Col, itemId );
		ShiftReleasedEvent = false; // try to fix not getting item after shift
	}else if( Cur.ObjectIndex != -1 && ( ! shiftPressed || abs(player.Row - Cur.Row) < 2 && abs(player.Col - Cur.Col) < 2
	 && Objects[Cur.ObjectIndex].destructable == 1) && !repeat ){// Если под курсором объект и он находится на рассоянии 1 клетки
        NetSendCmdLocParam1( 1, Cur.GraphicsID == CM_5_TRAP_DISARM ? CMD_17_DISARMXY : CMD_16_OPOBJXY, Cur.Row, Cur.Col, Cur.ObjectIndex );// активация объекта
		//ShiftPressedEvent = false;
		//ShiftReleasedEvent = false;
	}else if( !(player.IsBowInHand || player.IsFlaskInHand) ){ // не лук (мили оружие / без оружия)
		if( shiftPressed ){ // мили с шифтом
			if( CurMon == -1 || !MonsterTalking(CurMon) ){
                NetSendCmdLoc( PRIORITY_LOW, CMD_55_SATTACKXY, Cur.Row, Cur.Col );// C шифтом атака нелучником в пустоту
				return 0; // и стоим на месте
			}
		}else{ // мили клик без шифта
			if( CurMon == -1 ){ // нет монстра под курсором
				if( Cur.playerIndex != -1 && !PlayerFriendly() ){
					NetSendCmdParam1( 0, CMD_19_ATTACKPID, Cur.playerIndex );// мили атака другого игрока
				}else{
					return all( -1, itemId, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // движение если пол
				}
			}
		}
		if( Cur.GraphicsID != CM_7_TELEKINESIS ){
			if( CurMon >= SummonMonsters_Count ){
				NetSendCmdParam1( 0, CMD_18_ATTACKID, CurMon );// Без шифта атака монстра (не голем/суммон)
			}else{
				return true; // подходим к своим мобам/голему
			}
		}
	}else{ // лук или фласки
		if( shiftPressed ){
            NetSendCmdLoc( PRIORITY_LOW, CMD_13_RATTACKXY, Cur.Row, Cur.Col ); // Стрельба луком с шифтом
		}else{ // с луком без шифта
			if( CurMon == -1 ){ // Что то связанное с монстрами и скорее всего луком, ибо атаки не луком уже были
				if( Cur.playerIndex != -1 && !PlayerFriendly() ){
					NetSendCmdParam1( 0, CMD_21_RATTACKPID, Cur.playerIndex );// Атака луком игрока
				}else{
					return all( -1, itemId, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // движение если пол
				}
			}else if( CurMon >= SummonMonsters_Count ){
				NetSendCmdParam1( 0, MonsterTalking( CurMon ) ? CMD_18_ATTACKID : CMD_20_RATTACKID, CurMon ); // Атака луком монстра (не голем), говорящих монстров бьем ногой с луком
			}else{
				return true; // подходим к своим мобам/голему
			}
		}
	}
	return 0; // сюда по идее можно поставить return all, а все остальные return вообще убрать из функции
}

//----- (0040A0D9) -------------------------------------------------------- game
bool CursorSkillApply() // TryIconCurs
{
	bool applied = true;
	switch (Cur.GraphicsID ){
		case CM_8_RISING:
			//if(ObjectUnderCursor.playerIndex != -1){ // этой проверки нет в th1
            NetSendCmdParam1(1, CMD_26_RESURRECT, Cur.playerIndex);
			//MinusManaOrChargeOrRelicByPriceOfSSpell(CurrentPlayerIndex, 0); // этого вызова нет в th1
			//}
			break;
		case CM_10_HEAL_OTHER:
			//if(ObjectUnderCursor.playerIndex != -1){ // это проверки нет в th1
            NetSendCmdParam1(1, CMD_85_HEALOTHER, Cur.playerIndex);
			//MinusManaOrChargeOrRelicByPriceOfSSpell(CurrentPlayerIndex, PS_34_HEAL_OTHER); // этого вызова нет в th1
			//}
			break;
		case CM_7_TELEKINESIS:
			TelekinesApply();
			break;
		case CM_2_IDENTIFY:
			if( Cur.InventoryCellID != -1 ){
			    if( Cur.IsTradeService ){
			        VisualTrade_IdentifyItem(Cur.InventoryCellID);
                }else{
				    IdentifyItemWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
                }
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_3_REPAIR:
			if( Cur.InventoryCellID != -1 ){
			    if( Cur.IsTradeService ){
			        VisualTrade_RepairItem(Cur.InventoryCellID);
                }else{
				    RepairItemWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
                }
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_4_STAFF_RECHARGE:
			if( Cur.InventoryCellID != -1 ){
			    if( Cur.IsTradeService ){
			        VisualTrade_RechargeItem(Cur.InventoryCellID);
                }else{
				    RechargeStaffWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
                }
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_6_OIL:
			if( Cur.InventoryCellID != -1 ){
				UseOilWithCursor(CurrentPlayerIndex, Cur.InventoryCellID);
			}else{
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
	    case CM_9_TARGET:
			{
			Player& player = Players[CurrentPlayerIndex];
			int spellId = player.SpellOnCursor;
			// TODO: check postponing deleting item to REAL cast func StartSpell or PM_DoSpell (it may lead to desync or deleting wrong item)
		    if( FindItemInInventoryOrBeltBySeed(CurrentPlayerIndex, Cur.UsedItemSeed, Cur.UsedItemBase) && ! CooldownCheck(CurrentPlayerIndex, spellId) ){
				bool canUse = true;
				switch(spellId){
				// TODO: check why MC_47..51 is used instead of PS_47..51 as spellId
				case MC_47_RUNE_OF_FIRE or MC_48_RUNE_OF_LIGHTNING or MC_49_RUNE_OF_SHOCK or MC_50_RUNE_OF_BLAZE or MC_51_RUNE_OF_STONE:
					canUse = CheckCastInSight(player.Row, player.Col, Cur.Row, Cur.Col) && FoundCellForTargetInRadius(-1, &Cur.Row, &Cur.Col, 10);
					break;
				case PS_6_FIRE_WALL or PS_40_LIGHTING_WALL:
					canUse = LocateWallSegments(player.Row, player.Col, Cur.Row, Cur.Col);
					break;
				}
				if( canUse ){
					ClearPlayerItemBySeed(CurrentPlayerIndex, Cur.UsedItemSeed, Cur.UsedItemBase);
					if( is(spellId, PS_6_FIRE_WALL, PS_40_LIGHTING_WALL) ){ //Walls from right click Relict use should have direction info
						NetSendCmdLocParam3(1, CMD_92_SPELLXYD, Cur.GroundRow, Cur.GroundCol, spellId, OrientationToTarget(Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, Cur.GroundRow, Cur.GroundCol), PlayerSpellLevel(CurrentPlayerIndex, spellId));
					}else if( CurMon != -1 ){
						NetSendCmdParam3( 1, CMD_24_TSPELLID, CurMon, spellId, PlayerSpellLevel( CurrentPlayerIndex, spellId ) );
					}else if( Cur.playerIndex == -1 ){
						NetSendCmdLocParam2( 1, CMD_15_TSPELLXY, Cur.Row, Cur.Col, spellId, PlayerSpellLevel( CurrentPlayerIndex, spellId ) );
					}else{
						NetSendCmdParam3( 1, CMD_25_TSPELLPID, Cur.playerIndex, spellId, PlayerSpellLevel( CurrentPlayerIndex, spellId ) );
					}
				}
            }
			SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			break;
		case CM_5_TRAP_DISARM:
			if( Cur.ObjectIndex == -1 ){
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}else{
				applied = false;
			}
			break;
		default:
			applied = false;
			break;
	}
	if( applied ){
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE; // prevent autoclick after use skill
		Cur.UsedItemSeed = 0; Cur.UsedItemBase = -1;
	}
	return applied;
}


bool CursorOnManaGlobe() {
	int centerX = ScreenWidth / 2 + 182;
	int centerY = ScreenHeight - 100;
	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
}

bool CursorOnHealthGlobe() {
	int centerX = ScreenWidth / 2 - 182;
	int centerY = ScreenHeight - 100;
	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
}

//----- (0040A2C9) -------------------------------------------------------- interface
void OnMouseLButtonUp()
{
	perkUpArrowPressed = false;
	perkDownArrowPressed = false;
	Menu_CheckMouseClick(MOUSE_BUTTON_UP);
	TalkPanel_LButtonUp();
	if( MainPanelButtonFlag ){
		MainPanelButtonControl();
	}
	if( ButtonStatDown ){
        CharPanel_MouseClick();
	}
	if( IsCursorOnLevelUpButton ){
		LevelUPButton_MouseClick();
        return;
	}
    if( IsCursorOnPerkUpButton ){
        PerkUPButton_MouseClick();
        return;
    }
	if( CurrentDialogIndex ){
		Dialog_MouseClick();
	}
	if( IsStashPanelVisible ){
        StashPanel_MouseClick();
    }
    if( IsVisualTradePanelVisible ){
        VisualTrade_MouseClick();
    }
    if( IsCraftPanelVisible ){
        Craft_MouseClick();
    }
	if (IsEnchantPanelVisible) {
		Enchant_MouseClick();
	}
	if (IsQuenchPanelVisible) {
		Quench_MouseClick();
	}
	if (IsTransmutePanelVisible) {
		Transmute_MouseClick();
	}
	if (IsInfoWindowVisible) {
		InfoWindow_MouseUp();
	}
	if (CursorOnHealthGlobe()) {
		ShowNumbersOnHealth = !ShowNumbersOnHealth;
		PlayGlobalSound(75);
	}
	else if (CursorOnManaGlobe()) {
		ShowNumbersOnMana = !ShowNumbersOnMana;
		PlayGlobalSound(75);
	}

}

//----- (th2) -------------------------------------------------------------
bool __fastcall SpellCasting()
{
	// Если курсор изменил изображение -- не кастуем
	if (Cur.GraphicsID != CM_1_NORMAL_HAND) // Любой нестандарный вид курсора (вещь, телекинез, речардж и т.п)
		return false;

	Player& player = Players[CurrentPlayerIndex];
	PlayerClass& playerClass = PlayerClasses[player.ClassID];
	int currentSpellNumber = player.CurrentSpellIndex;

	// Заклинания кастуемые без указания положения курсора
	switch (currentSpellNumber) {
		// personal
	case PS_2_HEALING:
	case PS_11_MANA_SHIELD:
	case PS_16_REFLECT:
	case PS_22_FURY:
	case PS_9_INFRAVISION:
	case PS_37_MANA_RECHARGE:
	case PS_38_MAGI:
		// around
	case PS_17_LIGHTING_RING:
	case PS_19_FLAME_RING:
		// inventory
	case PS_5_IDENTIFY:
	case PS_26_ITEM_REPAIR:
	case PS_27_STAFF_RECHARGE:
	case PS_28_TRAP_DISARM:
		break;
	default:
		// При прочих заклинаниях не кастуем если:
		if (CursorInMainPanel() // Курсор над основной панелью
			|| (IsCHARPanelVisible && CursorIntoDisplayObject(CharPanelRect)) // Курсор над панелью с характеристиками
			|| (IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect)) // Курсор над инвентарём
			|| (IsPerksPanelVisible && CursorIntoDisplayObject(PerksPanelPos)) // Курсор над панелью перков
			|| (IsQUESTPanelVisible && CursorIntoDisplayObject(QuestPanelRect)) // Курсор над панелью квестов
			|| (IsSpellBookVisible && CursorIntoDisplayObject(SpellBookRect)) // Курсор над панелью заклинаний
			|| (IsInfoWindowVisible && CursorIntoDisplayObject(InfoWindowRect)) // Курсор над окном game changers
			)
		{
			return false;
		}
	}

	// Звуки издаваемые героями при попытке скастовать заклинание
	int voiceIndex;

	if (currentSpellNumber == -1) { // Если не установлено заклинание
		voiceIndex = playerClass.Voice[PLAYER_VOICE_NO_SPELL_CHANGED];
		PlayGlobalSound(voiceIndex);
		return false;
	}
	if (!IsSpellEnabledHere(currentSpellNumber, CurrentPlayerIndex)) { // Если заклинание недоступно
		voiceIndex = playerClass.Voice[PLAYER_VOICE_SPELL_DISABLE_IN_TOWN];
		PlayGlobalSound(voiceIndex);
		return false;
	}

	if (CooldownCheck(CurrentPlayerIndex, currentSpellNumber)) { // Если отсчитывает счётчик кулдауна
		switch (player.ClassID) {
			case PC_5_SAVAGE:
			case PC_0_WARRIOR: voiceIndex = S_766_WAR_37; break;
			case PC_4_ROGUE:
			case PC_1_ARCHER:  voiceIndex = S_657_ROG_37; break;
			case PC_2_MAGE:    voiceIndex = S_554_SOR_37; break;
			case PC_3_MONK:    voiceIndex = S_1339;       break;
			default:           voiceIndex = 0;
		}
		PlayGlobalSound(voiceIndex);
		return false;
	}

	// Проверяем условия наличия ресурсов для каста
	int spellType = player.SpellType;
	bool checkCastRequirementPassed = false;
	switch (spellType) {
	case SO_0_SKILL:
		checkCastRequirementPassed = true;
		break;
	case SO_1_SPELL:
		checkCastRequirementPassed = IsManaEnoughtForSpell(CurrentPlayerIndex, currentSpellNumber, spellType, 0);
		break;
	case SO_2_RELIC:
		checkCastRequirementPassed = IsHasRelicForSpell();
		break;
	case SO_3_EQUIPED_ITEM:
		checkCastRequirementPassed = IsHasChargesForSpell(currentSpellNumber);
		break;
	}

	// Если ресурсов недостаточно, проигрываем звук
	if (!(DevelopMode && FreeSpell) && !checkCastRequirementPassed) {
		voiceIndex = playerClass.Voice[PLAYER_VOICE_WITH_SPELL_TYPE];
		PlayGlobalSound(voiceIndex);
		return false;
	}

	// Условия ниже полностью перенесены из "int __fastcall Mouse_RButtonDown( int repeat );"

	//if the distance is more than 10 cells, move 10 cells in the direction of the click
	if (!(DevelopMode && FreeSpell) //bypass 10 range limit if cheats are on
		&& is(currentSpellNumber,  //list of spells limited to range of 10 tiles
			PS_34_HEAL_OTHER,
			PS_23_TELEPORT,
			PS_7_TOWN_PORTAL,
			PS_6_FIRE_WALL,
			PS_40_LIGHTING_WALL,
			PS_13_HYDRA,
			PS_21_GOLEM,
			PS_52_LESSER_SUMMON,
			PS_53_COMMON_SUMMON,
			PS_54_GREATER_SUMMON/*,
			PS_55_UNSUMMON*/)
		&& (abs(player.Row - Cur.Row) > 10 || abs(player.Col - Cur.Col) > 10)) { //real X/Y coords computations
		int row = Cur.Row;
		int col = Cur.Col;
		int signRow = player.Row - row >= 0 ? 1 : -1;
		int signCol = player.Col - col >= 0 ? 1 : -1;
		int deltaRow = abs(player.Row - row);
		int deltaCol = abs(player.Col - col);
		if (deltaRow > deltaCol) {
			deltaCol = deltaCol * 10 / deltaRow; deltaRow = 10;
		}
		else {
			deltaRow = deltaRow * 10 / deltaCol; deltaCol = 10;
		}
		row = player.Row - signRow * deltaRow;
		col = player.Col - signCol * deltaCol;

		
		XinputSpellCastingPositioning(player, currentSpellNumber, row, col);
		//Net CMD should be sent with low priority to prevent double cast desync. High priority packets are buggy!
		if (currentSpellNumber == PS_6_FIRE_WALL || currentSpellNumber == PS_40_LIGHTING_WALL) { // limited walls
			int orientation = OrientationToTarget(player.Row, player.Col, Cur.GroundRow, Cur.GroundCol);
			int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
			NetSendCmdLocParam3(0, CMD_92_SPELLXYD, row, col, currentSpellNumber, orientation, spellLevel);
		}
		else {
			int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
			NetSendCmdLocParam2(0, CMD_14_SPELLXY, row, col, currentSpellNumber, spellLevel); //limited XY spells
		}

	}
	else if (currentSpellNumber == PS_6_FIRE_WALL || currentSpellNumber == PS_40_LIGHTING_WALL) { // walls
		int orientation = OrientationToTarget(player.Row, player.Col, Cur.GroundRow, Cur.GroundCol);
		int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
		NetSendCmdLocParam3(0, CMD_92_SPELLXYD, Cur.GroundRow, Cur.GroundCol, currentSpellNumber, orientation, spellLevel);
	}
	else if (CurMon != -1) {// Monster under cursor
		int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
		NetSendCmdParam3(0, CMD_22_SPELLID, CurMon, currentSpellNumber, spellLevel);
	}
	else if (Cur.playerIndex == -1) {// Nothing under the cursor
		int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
		XinputSpellCastingPositioning(player, currentSpellNumber, Cur.Row, Cur.Col);
		NetSendCmdLocParam2(0, CMD_14_SPELLXY, Cur.Row, Cur.Col, currentSpellNumber, spellLevel);
	}
	else {// Player under cursor
		int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber); // spell level calc at rmb click
		NetSendCmdParam3(0, CMD_23_SPELLPID, Cur.playerIndex, currentSpellNumber, spellLevel);
	}

	#if SPELL_QUEUE_AND_COOLDOWN_FIX
	// Player right clicks to Teleport, immediately followed by casting a different spell before Teleport finishes,
	// the second spell will now have correct orientation and coordinates.
	if( currentSpellNumber != PS_23_TELEPORT ){
		player.PrevRow = player.Row;
		player.PrevCol = player.Col;
	}
	#endif

	return true;
}

//----- (0040A30E) -------------------------------------------------------- interface
int __fastcall Mouse_RButtonDown( int repeat )
{
	Player& player = Players[ CurrentPlayerIndex ];
	if( /*Berserk*/HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (player.activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
		return 0;
	}
	int currentSpellNumber = player.CurrentSpellIndex;
	
	if( IsGameMenuActive() || NetCursorItemId || GamePaused == 2 || Players[CurrentPlayerIndex].notHittableInTransition ){// Какое-то странное условие
		return 0;
	}
	if( IsPopupScreenOpen ){
		if( !repeat ) ClosePopupScreen();
		return 0;
	}
	if( CurrentDialogIndex || IsPerksPanelVisible || IsInfoWindowVisible){
		return 0;
	}
	if( SelectCurSpellMode != 0 && !repeat ){
		SelectCurrentSpell();
		return 0;
	}
	if( repeat && (Cur.InventoryCellID != -1 && !EnteredPanelWithAutoClick || currentSpellNumber == -1) ) return 0;
	
	if( !repeat && IsInterface && !( (!IsSpellBookVisible || !CursorIntoDisplayObject(SpellBookRect)) // Исправление глюка с поясом 5-8
	 && ( CursorInMainPanel() || ! CursorSkillApply() /*Что то связанное с нет командами*/
	 && ( Cur.InventoryCellID == -1 /*Если нет вещи под курсором вроде бы это в следующем сабе проверяется*/ 
	 || !RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID, true) ) ) ) ){ // may be duplicate call (of below) and needs to remove, or a decompile error
		return 0;
	}
	if( Cur.GraphicsID != CM_1_NORMAL_HAND && !repeat ){
		return 0;
	}
	// тут не срабатывали реликты, так как срабатывал repeat
	if( !repeat && Cur.InventoryCellID != -1 ){
		// Использование реликтов похоже
		if( RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID, true) ) return 0;
	}
	if( (unsigned int)CurrentPlayerIndex >= 4 ){
		TerminateWithError("CheckPlrSpell: illegal player %d", CurrentPlayerIndex);
	}
		//Unexpect3D:
		//По видимому там срабатывает "репит", при зажимании правой кнопки мыши.
		/* the fix */
	if (repeat && IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect)) {
		return 0;
	}
		/* end of the fix */
	return SpellCasting();
}

//----- (0040A3FB) -------------------------------------------------------- main
int __fastcall CheckF10Message(int key)
{
	int result; // eax@3
	if( IsGameMenuActive() || key != VK_F10 ){
		result = 0;
	}else{
		SendNetGroupChatMesage(1);
		result = 1;
	}
	return result;
}

//----- (0040A41D) -------------------------------------------------------- net
void __fastcall SendNetGroupChatMesage(int index)
{
	int v1;                  // esi@1
	char* v2;                // eax@4
	char FileName[MAX_PATH];      // [sp+4h] [bp-154h]@2
	char ReturnedString[80]; // [sp+108h] [bp-50h]@6
	v1 = index;
	if( MaxCountOfPlayersInGame != 1 ){
		if( !GetModuleFileNameA(HInstance, FileName, MAX_PATH) )
			TerminateWithError("Can't get program name");
		v2 = strrchr(FileName, '\\');
		if( v2 )
			*v2 = 0;
		strcat(FileName, "\\hellmsg.ini");
		GetPrivateProfileStringA("NetMsg", KeyNameTable[v1], NetMsgTable[v1], ReturnedString, 80, FileName);
        NetSendString( 0xFFFFFFFF, ReturnedString );
	}
}

//----- (0040A4BE) -------------------------------------------------------- game
void __fastcall KeyUpHandler( int key)
{
	if( key == VK_SNAPSHOT ){
		SaveScreenShot();
	}else if(key == VK_CONTROL){
		//if( !(GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_RMENU) < 0) ){ // not AltGr // TODO: check if need (testing say no)
			IsCtrlPressed = false;
			if( ForceMoveOnCtrl ){
				ForceMove = false;
			}
		//}
	}
}

//----- (0040A634) -------------------------------------------------------- interface
int __fastcall F1F4PressHandler( WPARAM fIndex, bool isF1 )
{
	int newSpellNumber;
	switch( fIndex ){
	case 0:		newSpellNumber = PS_22_FURY;		break;
	case 1:		newSpellNumber = PS_2_HEALING;		break;
	case 2:		newSpellNumber = PS_16_REFLECT;		break;
	default:	newSpellNumber = PS_11_MANA_SHIELD;	break;
	}
	return newSpellNumber;
}

//----- (0040A4C9) -------------------------------------------------------- interface
void __fastcall KeyPressHandler(WPARAM key)
{
	Player& player = Players[CurrentPlayerIndex];
	if( Menu_DispatchKey(key) ){
		return;
	}
	if( HandleTalkPanelControlKeys(key) ){
		return;
	}
	if( IsPlayerDead ){
		if( !NetCursorItemId ){
			if( key >= VK_120_F9_KEY && key <= VK_123_F12_KEY){
				int groupMessageIndex = key - VK_120_F9_KEY;
				SendNetGroupChatMesage(groupMessageIndex);
			}

			if( key == VK_13_RETURN_KEY){
				OpenTalkPanel();
			}

			if( key == VK_27_ESC_KEY ){
				if( !EscHandle() ){
                    TrackInit( false );
					SelectMainMenu();
				}
			}
		}
		return;
	}
	if( key == VK_27_ESC_KEY ){
		if( !EscHandle() ){
            TrackInit( false );
			if (DisableESCinBattle && GameMode == GM_IRONMAN && MaxCountOfPlayersInGame == 1) {
				if (!IsSomeMonstersActivated()) {
					SelectMainMenu();
				}
			}
			else {
				SelectMainMenu();
			}
		}
		return;
	}
	if( NetCursorItemId || IsGoldSplitPanelVisible ){
		return;
	}
	if( key == VK_19_PAUSE_KEY ){
		HandlePause();
		return;
	}
	if( GamePaused != 2 && ! TalkPanelMode ){ // next hotkeys available only if no talk panel activated
		if( key == VK_84_T_KEY && IsTransmutePanelVisible ){
			Transmute_Start();
			return;
		}
		if (key == VK_82_R_KEY && IsVisualTradePanelVisible) {
			VisualTrade_Restock();
			return;
		}
		if( key == VK_13_RETURN_KEY ){
			if( CurrentDialogIndex ){
				HandleTownersMenu();
			}else if( IsQUESTPanelVisible ){
				QuestPanel_StartSpeech();
			}/*else if( IsPerksPanelVisible ){
				PerkPanel_ImprovePerk();
			}*/else{
				OpenTalkPanel();
			}
			return;
		}
		// Manual Hotkeys (F5-F8 + QWERT,ASDFG,ZX)
		if (MoreManualHotkeys) {
			int customSpellIndex = -1;
			if (key >= VK_112_F1_KEY && key <= VK_119_F8_KEY)
				customSpellIndex = key - VK_112_F1_KEY;
			else if (key >= VK_65_A_KEY && key <= VK_90_Z_KEY) {
				// if (!(key == VK_67_C_KEY || key == VK_73_I_KEY || key == VK_80_P_KEY || key == VK_86_V_KEY))
					customSpellIndex = key - VK_65_A_KEY + 12;
			}
			else if (key == VK_17_CTRL_KEY) {
				if (!(GetAsyncKeyState(VK_RMENU) < 0)) { // TODO: check wtf is this
					if (GetAsyncKeyState(VK_RCONTROL) < 0)
						customSpellIndex = 39;
					else
						customSpellIndex = 38;
				}
			}
			if (customSpellIndex != -1) {
				if (SelectCurSpellMode) {
					AssignNewSpellToKeyExpanded(customSpellIndex);
					return;
				}
				else if (SpellHotkeys.hotkey[customSpellIndex] > 0) {
					if (SelectSpellByHotKeyExpanded(customSpellIndex)) {
						if(InstantHotkeys) SpellCasting();
						return;
					}
				}
			}
		}
		else
		{
			if (key >= VK_116_F5_KEY && key <= VK_119_F8_KEY) {
				int customSpellIndex = key - VK_116_F5_KEY;
				if (SelectCurSpellMode) {
					AssignNewSpellToKey(customSpellIndex);
				}
				else {
					SelectSpellByHotKey(customSpellIndex);
				}
				return;
			}
		}
		// New Spell Shortcuts System
		if (NewShortcuts) {
			// Список заклинаний с информацией по-умолчанию для новой системы шоткатов
			// 26 букв английского алфавита, 12 функциональных клавиш и CTRL (левый и правый).
			// До восьми заклинаний на каждой клавише
			// ToDo: можно переместить в "dataseg.cpp"
			static constexpr int shortcuts_amount = 40; // static inline const [C++03]
			static constexpr int shortcut_capacity = 8; // static inline const [C++03]
			static volatile int GlobalCustomKeys[shortcuts_amount][shortcut_capacity] = {
				/*A*/{PS_19_FLAME_RING,PS_6_FIRE_WALL,PS_20_INCINERATE,0,0,0,0,0},
				/*B*/{PS_52_LESSER_SUMMON,PS_53_COMMON_SUMMON,PS_54_GREATER_SUMMON,PS_55_UNSUMMON,0,0,0,0},
				/*C*/{0,0,0,0,0,0,0,0},
				/*D*/{PS_15_FORCE_WAVE,PS_29_ELEMENTAL,PS_36_BONE_SPIRIT,0,0,0,0,0},
				/*E*/{PS_31_HOLY_BOLT,PS_39_HOLY_NOVA,0,0,0,0,0,0},
				/*F*/{PS_23_TELEPORT,PS_33_TELEKINES,PS_7_TOWN_PORTAL,0,0,0,0,0},
				/*G*/{PS_21_GOLEM,0,0,0,0,0,0,0},
				/*H*/{PS_13_HYDRA,0,0,0,0,0,0,0},
				/*I*/{0,0,0,0,0,0,0,0},
				/*J*/{0,0,0,0,0,0,0,0},
				/*K*/{0,0,0,0,0,0,0,0},
				/*L*/{0,0,0,0,0,0,0,0},
				/*M*/{0,0,0,0,0,0,0,0},
				/*N*/{0,0,0,0,0,0,0,0},
				/*O*/{0,0,0,0,0,0,0,0},
				/*P*/{0,0,0,0,0,0,0,0},
				/*Q*/{PS_1_FIREBOLT,PS_12_FIREBLAST,PS_41_FIERY_NOVA,0,0,0,0,0},
				/*R*/{PS_35_ARCANE_STAR,PS_43_ARCANE_NOVA,0,0,0,0,0,0},
				/*S*/{PS_17_LIGHTING_RING,PS_40_LIGHTING_WALL,PS_3_LIGHTNING,0,0,0,0,0},
				/*T*/{PS_56_ICE_BOLT,PS_59_RANCID_BOLT,PS_57_FREEZING_BALL,PS_60_TOXIC_BALL,PS_58_FROST_NOVA,PS_61_ACID_NOVA,0,0},
				/*U*/{0,0,0,0,0,0,0,0},
				/*V*/{0,0,0,0,0,0,0,0},
				/*W*/{PS_30_CHARGED_BOLT,PS_14_BALL_LIGHTNING,PS_18_LIGHTNING_NOVA,0,0,0,0,0},
				/*X*/{PS_16_REFLECT,PS_22_FURY,PS_8_STONE_CURSE,0,0,0,0,0},
				/*Y*/{PS_4_FLASH,0,0,0,0,0,0,0},
				/*Z*/{PS_11_MANA_SHIELD,PS_2_HEALING,PS_34_HEAL_OTHER,0,0,0,0,0},
				/*F1*/{PS_22_FURY,0,0,0,0,0,0,0},
				/*F2*/{PS_2_HEALING,0,0,0,0,0,0,0},
				/*F3*/{PS_16_REFLECT,0,0,0,0,0,0,0},
				/*F4*/{PS_11_MANA_SHIELD,0,0,0,0,0,0,0},
				/*F5*/{PS_56_ICE_BOLT,PS_59_RANCID_BOLT,PS_52_LESSER_SUMMON,0,0,0,0,0},
				/*F6*/{PS_57_FREEZING_BALL,PS_60_TOXIC_BALL,PS_53_COMMON_SUMMON,0,0,0,0,0},
				/*F7*/{PS_58_FROST_NOVA,PS_61_ACID_NOVA,PS_54_GREATER_SUMMON,0,0,0,0,0},
				/*F8*/{PS_55_UNSUMMON,0,0,0,0,0,0,0},
				/*F9*/{0,0,0,0,0,0,0,0},
				/*F10*/{0,0,0,0,0,0,0,0},
				/*F11*/{0,0,0,0,0,0,0,0},
				/*F12*/{0,0,0,0,0,0,0,0},
				/*LCTRL*/{PS_13_HYDRA,0,0,0,0,0,0,0},
				/*RCTRL*/{PS_26_ITEM_REPAIR,PS_27_STAFF_RECHARGE,PS_5_IDENTIFY,PS_28_TRAP_DISARM,PS_9_INFRAVISION,0,0,0}
			};

			static bool CustomKeysFileLoaded = false; // "CustomKeysLoad" будем вызывать только единожды

			// Загрузка файла "config.ini" (ранее "CustomKeys.txt") с глобальными клавишами заклинаний
			// ToDo: можно переместить в "IniFile.cpp" / "Config.cpp"
			auto CustomKeysLoad = [](char* txtFileName) {
				int ch = -1;
				int spell = 0;
				int keytype = 0;
				int cnt[shortcuts_amount] = { 0 };
				int user_keys[shortcuts_amount][shortcut_capacity] = { 0 }; // PS_M1_NONE or PS_0_NONE
				char data[1024];

				if (FILE* file = fopen(txtFileName, "r")) {
					while (fscanf_s(file, "%s", data, 1024) != EOF) {
						if (data[0] == '[') {
							if (data[2] == ']') {
								ch = data[1] - 'A';
								keytype = 0;
							}
							else if (data[1] == 'F') {
								if (data[3] == ']') {
									ch = (data[2] - '1') + 26;
									keytype = 1;
								}
								else if (data[2] == '1' && data[4] == ']') {
									ch = (data[3] - '0') + 35;
									keytype = 1;
								}
							}
							else if (!_stricmp(data, "[LeftCTRL]")) {
								ch = 38;
								keytype = 2;
							}
							else if (!_stricmp(data, "[RightCTRL]")) {
								ch = 39;
								keytype = 2;
							}
						}

						if (keytype == 0 && (ch < 0 || ch > 25)) continue;		// (0-25) A-Z [26]
						if (keytype == 1 && (ch < 26 || ch > 37)) continue;		// (26-37) F1-F12 [12]
						if (keytype == 2 && (ch < 38 || ch > 39 )) continue;	// (38-39) LCTRL-RCTRL [2]

						if		(!_stricmp(data, "FIREBOLT"))		spell = PS_1_FIREBOLT;
						else if (!_stricmp(data, "FIREBLAST"))		spell = PS_12_FIREBLAST;
						else if (!_stricmp(data, "FIERY_NOVA"))		spell = PS_41_FIERY_NOVA;
						else if (!_stricmp(data, "CHARGED_BOLT"))	spell = PS_30_CHARGED_BOLT;
						else if (!_stricmp(data, "BALL_LIGHTNING"))	spell = PS_14_BALL_LIGHTNING;
						else if (!_stricmp(data, "LIGHTNING_NOVA"))	spell = PS_18_LIGHTNING_NOVA;
						else if (!_stricmp(data, "HOLY_BOLT"))		spell = PS_31_HOLY_BOLT;
						else if (!_stricmp(data, "HOLY_NOVA"))		spell = PS_39_HOLY_NOVA;
						else if (!_stricmp(data, "ARCANE_STAR"))	spell = PS_35_ARCANE_STAR;
						else if (!_stricmp(data, "ARCANE_NOVA"))	spell = PS_43_ARCANE_NOVA;
						else if (!_stricmp(data, "ICE_BOLT"))		spell = PS_56_ICE_BOLT;
						else if (!_stricmp(data, "RANCID_BOLT"))	spell = PS_59_RANCID_BOLT;
						else if (!_stricmp(data, "FREEZING_BALL"))	spell = PS_57_FREEZING_BALL;
						else if (!_stricmp(data, "TOXIC_BALL"))		spell = PS_60_TOXIC_BALL;
						else if (!_stricmp(data, "FROST_NOVA"))		spell = PS_58_FROST_NOVA;
						else if (!_stricmp(data, "ACID_NOVA"))		spell = PS_61_ACID_NOVA;
						else if (!_stricmp(data, "FLAME_RING"))		spell = PS_19_FLAME_RING;
						else if (!_stricmp(data, "FIRE_WALL"))		spell = PS_6_FIRE_WALL;
						else if (!_stricmp(data, "INCINERATE"))		spell = PS_20_INCINERATE;
						else if (!_stricmp(data, "LIGHTING_RING"))	spell = PS_17_LIGHTING_RING;
						else if (!_stricmp(data, "LIGHTING_WALL"))	spell = PS_40_LIGHTING_WALL;
						else if (!_stricmp(data, "LIGHTNING"))		spell = PS_3_LIGHTNING;
						else if (!_stricmp(data, "FORCE_WAVE"))		spell = PS_15_FORCE_WAVE;
						else if (!_stricmp(data, "ELEMENTAL"))		spell = PS_29_ELEMENTAL;
						else if (!_stricmp(data, "BONE_SPIRIT"))	spell = PS_36_BONE_SPIRIT;
						else if (!_stricmp(data, "TELEPORT"))		spell = PS_23_TELEPORT;
						else if (!_stricmp(data, "TELEKINES"))		spell = PS_33_TELEKINES;
						else if (!_stricmp(data, "TOWN_PORTAL"))	spell = PS_7_TOWN_PORTAL;
						else if (!_stricmp(data, "FLASH"))			spell = PS_4_FLASH;
						else if (!_stricmp(data, "GOLEM"))			spell = PS_21_GOLEM;
						else if (!_stricmp(data, "HYDRA"))			spell = PS_13_HYDRA;
						else if (!_stricmp(data, "MANA_SHIELD"))	spell = PS_11_MANA_SHIELD;
						else if (!_stricmp(data, "HEALING"))		spell = PS_2_HEALING;
						else if (!_stricmp(data, "HEAL_OTHER"))		spell = PS_34_HEAL_OTHER;
						else if (!_stricmp(data, "REFLECT"))		spell = PS_16_REFLECT;
						else if (!_stricmp(data, "FURY"))			spell = PS_22_FURY;
						else if (!_stricmp(data, "STONE_CURSE"))	spell = PS_8_STONE_CURSE;
						else if (!_stricmp(data, "LESSER_SUMMON"))	spell = PS_52_LESSER_SUMMON;
						else if (!_stricmp(data, "COMMON_SUMMON"))	spell = PS_53_COMMON_SUMMON;
						else if (!_stricmp(data, "GREATER_SUMMON"))	spell = PS_54_GREATER_SUMMON;
						else if (!_stricmp(data, "UNSUMMON"))		spell = PS_55_UNSUMMON;
						// additionally
						else if (!_stricmp(data, "ITEM_REPAIR"))	spell = PS_26_ITEM_REPAIR;
						else if (!_stricmp(data, "ITEM_RECHARGE"))	spell = PS_27_STAFF_RECHARGE;
						else if (!_stricmp(data, "IDENTIFY"))		spell = PS_5_IDENTIFY;
						else if (!_stricmp(data, "TRAP_DISARM"))	spell = PS_28_TRAP_DISARM;
						else if (!_stricmp(data, "PHASING"))		spell = PS_10_PHASING;
						else if (!_stricmp(data, "APOCALYPSE"))		spell = PS_24_APOCALYPSE;
						else if (!_stricmp(data, "MAGI"))			spell = PS_38_MAGI;
						else if (!_stricmp(data, "MANA_RECHARGE"))	spell = PS_37_MANA_RECHARGE;
						else if (!_stricmp(data, "SEEING"))			spell = PS_9_INFRAVISION;
						else if (!_stricmp(data, "ETHEREAL"))		spell = PS_25_ETHEREAL;
						// else if (!_stricmp(data, "WARP"))		spell = PS_42_WARP;
						// else if (!_stricmp(data, "BERSERK"))		spell = PS_44_BERSERK;
						else continue;

						// назначаем в слот к шоткату переопределённое заклинание
						user_keys[ch][cnt[ch]] = spell; 

						if (cnt[ch] < (shortcut_capacity - 1))
							cnt[ch] += 1; // переходим к следующему свободному слоту (до крайнего)
					}
					fclose(file);

					//extern volatile int GlobalCustomKeys[shortcuts_amount][shortcut_capacity];

					// проверяем были ли переопределены клавиши в "config.ini"
					ch = 0;
					for (int i = 0; i < shortcuts_amount; ++i)
						if (cnt[i] > 0) {
							ch = 1;
							break;
					}
					// если клавиши переопределены, то заменяем ими определения по-умолчанию
					if(ch)
						for (int i = 0, j; i < shortcuts_amount; ++i)
							for (j = 0; j < shortcut_capacity; ++j)
								GlobalCustomKeys[i][j] = user_keys[i][j];
				}
			};

			auto ApplyHotkey = [&CustomKeysLoad, &player](int key) {
				
				// Вызов загрузки файла с горячими клавишами (грузим один раз)
				// ToDo: хорошо бы переместить в "Config.cpp"
				if (!CustomKeysFileLoaded) {
					CustomKeysLoad("config.ini");
					CustomKeysFileLoaded = true;
				}

				//extern volatile int GlobalCustomKeys[shortcuts_amount][shortcut_capacity];
				int nextSpell = PS_M1_NONE;

				if (GlobalCustomKeys[key][0] < 1)
					return 0;
				
				for (int i = 0, j; i < (shortcut_capacity -1); ++i)
					if ( GlobalCustomKeys[key][i] == player.CurrentSpellIndex)
						for( j = i+1; j < shortcut_capacity; ++j )
							if( GlobalCustomKeys[key][j] > 0 ) {
								nextSpell = GlobalCustomKeys[key][j];
								if (nextSpell == PS_M1_NONE || nextSpell == PS_0_NONE)
									break;
								if ((player.AvailableSpellMask & BIT(nextSpell))
									&& (!SkipUnusableSpells || PlayerSpellLevel(CurrentPlayerIndex, nextSpell) > 0)
									) {
									player.CurrentSpellIndex = nextSpell;
									player.SpellType = SO_1_SPELL;
									return 1;
								}
								if (player.AvailableSkillMask & BIT(nextSpell)) {
									player.CurrentSpellIndex = nextSpell;
									player.SpellType = SO_0_SKILL;
									return 1;
								}
							}
				for (int i = 0; i < shortcut_capacity; ++i)
					if ( GlobalCustomKeys[key][i] > 0 ) {
						nextSpell = GlobalCustomKeys[key][i];
						if (nextSpell == PS_M1_NONE || nextSpell == PS_0_NONE)
							continue;
						if ((player.AvailableSpellMask & BIT(nextSpell))
							&& (!SkipUnusableSpells || PlayerSpellLevel(CurrentPlayerIndex, nextSpell) > 0)
							) {
							player.CurrentSpellIndex = nextSpell;
							player.SpellType = SO_1_SPELL;
							return 1;
						}
						if (player.AvailableSkillMask & BIT(nextSpell)) {
							player.CurrentSpellIndex = nextSpell;
							player.SpellType = SO_0_SKILL;
							return 1;
						}
					}
				//if (!SkipUnusableSpells) { // выставляем иконку "отсутствия заклинания"
				//	player.CurrentSpellNumber = PS_M1_NONE;
				//	player.SpellType = SO_4_DISABLE;
				//}
				return 0;
			};

			// Отлавливаем клавиши [A]-[Z], [F1]-[F8], [CTRL]
			if (key >= VK_65_A_KEY && key <= VK_90_Z_KEY) {
				switch (key) { // клавиши-исключения
				case VK_67_C_KEY: // Character panel
				case VK_86_V_KEY: // Swap weapon
				case VK_73_I_KEY: // Inventory
				case VK_80_P_KEY: // Perk panel
					break;
				default:
					if (ApplyHotkey(key - VK_65_A_KEY)) return; // #65->#0	
				}
			}
			else if (key >= VK_112_F1_KEY && key <= VK_119_F8_KEY /*VK_123_F12_KEY*/) {
				if(ApplyHotkey(key - VK_112_F1_KEY + 26)) return; // #112->#26	
			}
			else if (key == VK_CONTROL) {
				//if( !(GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_RMENU) < 0) ){ // not AltGr // TODO: check if need (testing say no)
					IsCtrlPressed = true;
					if( ForceMoveOnCtrl ){
						ForceMove = true;
					}
					if (((ForceMoveOnCtrl || DropItemOnCtrl) && KeepSpellsOnCtrls) || (!ForceMoveOnCtrl && !DropItemOnCtrl)) {
						if (ApplyHotkey(GetAsyncKeyState(VK_RCONTROL) < 0 ? 39 : 38)) return; // #38 #39
					}
				//}
			}
		}
		else
		{
			int newSpellNumber = PS_M1_NONE;
			switch (key) {
			case VK_87_W_KEY:	newSpellNumber = PS_6_FIRE_WALL;		break;
			case VK_81_Q_KEY:	newSpellNumber = PS_23_TELEPORT;		break;
			case VK_69_E_KEY:	newSpellNumber = PS_40_LIGHTING_WALL;	break;
			case VK_79_O_KEY:	newSpellNumber = PS_7_TOWN_PORTAL;		break;
			case VK_82_R_KEY:	newSpellNumber = PS_41_FIERY_NOVA;		break;
			case VK_84_T_KEY:	newSpellNumber = PS_18_LIGHTNING_NOVA;	break;
			case VK_89_Y_KEY:	newSpellNumber = PS_43_ARCANE_NOVA;		break;
			case VK_85_U_KEY:	newSpellNumber = PS_39_HOLY_NOVA;		break;
			case VK_65_A_KEY:	newSpellNumber = PS_12_FIREBLAST;		break;
			case VK_68_D_KEY:	newSpellNumber = PS_35_ARCANE_STAR;		break;
			case VK_70_F_KEY:	newSpellNumber = PS_8_STONE_CURSE;		break;
			case VK_71_G_KEY:	newSpellNumber = PS_21_GOLEM;			break;
			case VK_72_H_KEY:	newSpellNumber = PS_31_HOLY_BOLT;		break;
			case VK_74_J_KEY:	newSpellNumber = PS_4_FLASH;			break;
			case VK_75_K_KEY:	newSpellNumber = PS_20_INCINERATE;		break;
			case VK_76_L_KEY:	newSpellNumber = PS_3_LIGHTNING;		break;
			case VK_88_X_KEY:	newSpellNumber = PS_15_FORCE_WAVE;		break;
			case VK_78_N_KEY:	newSpellNumber = PS_19_FLAME_RING;		break;
			case VK_77_M_KEY:	newSpellNumber = PS_17_LIGHTING_RING;	break;
			case VK_66_B_KEY:	newSpellNumber = PS_36_BONE_SPIRIT;		break;
			case VK_83_S_KEY:	newSpellNumber = PS_14_BALL_LIGHTNING;	break;
			case VK_90_Z_KEY:	newSpellNumber = PS_29_ELEMENTAL;		break;
			case VK_17_CTRL_KEY:
				//if( !(GetAsyncKeyState(VK_LCONTROL) < 0 && GetAsyncKeyState(VK_RMENU) < 0) ){ // not AltGr // TODO: check if need (testing say no) 
					IsCtrlPressed = true;
					if( ForceMoveOnCtrl ){
						ForceMove = true;
					}
					if (((ForceMoveOnCtrl || DropItemOnCtrl) && KeepSpellsOnCtrls) || (!ForceMoveOnCtrl && !DropItemOnCtrl)) {
						newSpellNumber = PS_13_HYDRA;
					}
				//}
				break;
			}
			if (key >= VK_112_F1_KEY && key <= VK_115_F4_KEY && !SelectCurSpellMode) {
				newSpellNumber = F1F4PressHandler(key - VK_112_F1_KEY, key == VK_112_F1_KEY);
			}
			if (newSpellNumber != PS_M1_NONE) {
				if (player.SpellLevels[newSpellNumber] >= 1) {
					player.CurrentSpellIndex = newSpellNumber;
					player.SpellType = SO_1_SPELL;
				}
				return;
			}
		}
		if( key >= VK_120_F9_KEY && key <= VK_123_F12_KEY){
			int groupMessageIndex = key - VK_120_F9_KEY;
			SendNetGroupChatMesage(groupMessageIndex);
			return;
		}
		if( key == VK_38_UP_ARROW_KEY ){
			if( CurrentDialogIndex ){
				DialogUp();
			}else if( IsQUESTPanelVisible ){
				QuestPanel_ShiftUp();
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(-1);
			}else if (IsInfoWindowVisible) {
				InfoWindow_Prev();
			}else if( IsHELPVisible ){
				ShiftHelpUp();
			}else if( AutomapMode ){
				ShiftAutomapUp();
			}else if( IsTransmutePanelVisible ){
				if( TransmuteShowRecipeList && ! TransmuteSlots.count ){
					TransmuteRecipeListUp();
				}
			}
			return;
		}
		if( key == VK_40_DOWN_ARROW_KEY ){
			if( CurrentDialogIndex ){
				DialogDown();
			}else if( IsQUESTPanelVisible ){
				QuestPanel_ShiftDown();
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(1);
			}else if (IsInfoWindowVisible) {
				InfoWindow_Next();
			}else if( IsHELPVisible ){
				ShiftHelpDown();
			}else if( AutomapMode ){
				ShiftAutomapDown();
			}else if( IsTransmutePanelVisible ){
				if( TransmuteShowRecipeList && ! TransmuteSlots.count ){
					TransmuteRecipeListDown();
				}
			}
			return;
		}
		if( key == VK_33_PAGE_UP ){
			if( CurrentDialogIndex ){
				DialogPageUp();
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(-7);
			}
			return;
		}
		if( key == VK_34_PAGE_DOWN ){
			if( CurrentDialogIndex ){
				DialogPageDown();
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(7);
			}
			return;
		}
		if( key == VK_36_HOME_KEY ){
			if( CurrentDialogIndex ){
				while( DialogPageUp() );
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(-PERKS_MAX_800);
			}
			return;
		}
		if( key == VK_35_END_KEY ){
			if( CurrentDialogIndex ){
				while( DialogPageDown() );
			}else if( IsPerksPanelVisible ){
				PerkPanel_Shift(PERKS_MAX_800);
			}
			return;
		}
		if( key == VK_37_LEFT_ARROW_KEY ){
			if (IsStashPanelVisible) {
				StashPanel_PrevTab();
			}else if( IsPerksPanelVisible ){
				PerkPanel_ShiftLeft();
			}else if( AutomapMode ){
				ShiftAutomapLeft();
			}
			return;
		}
		if( key == VK_39_RIGHT_ARROW_KEY ){
			if (IsStashPanelVisible) {
				StashPanel_NextTab();
			}else if( IsPerksPanelVisible ){
				PerkPanel_ShiftRight();
			}else if( AutomapMode ){
				ShiftAutomapRight();
			}
			return;
		}
		if( key == VK_9_TAB_KEY ){
			ToggleAutomap();
			return;
		}
		if( key == VK_32_SPACE_KEY ){
			IsHELPVisible = false;
			IsINVPanelVisible = false;
			IsStashPanelVisible = false;
			VisualTrade_Close();
			Craft_Close();
			Enchant_Close();
			Quench_Close();
			Transmute_Close();
			IsPerksPanelVisible = false;
			IsInfoWindowVisible = false;
			IsCHARPanelVisible = false;
			IsSpellBookVisible = false;
			SelectCurSpellMode = false;
			if( Speech_IsPanelVisible && Dungeon->genType == DT_0_TOWN ){
				Speech_IsPanelVisible = false;
				StopSpeech();
			}
			IsQUESTPanelVisible = false;
			ResetMainMenu();
			ClosePopupScreen();
			return;
		}
		// non input line hotkeys (prev was in HandleCharHotKey dependent of keyboard language layout)
		/*if( key == VK_48_0_KEY ){
			if( !IsINVPanelVisible ){
				DialogRowsCount = 1;
				FirstVisibleItem = 0;
				SelectedDialogRow = PT_SomeDialogRowForMessageLog;
				ChangeTownerDialog(PD_25_MessageLog);
				return;
			}
		}*/
		if( key == VK_80_P_KEY ){
			TogglePerksPanel();
			return;
		}
		#if CHEATS
		if( key == VK_OEM_COMMA  ){ TimeChange( -0.01 ); return; } // < ,
		if( key == VK_OEM_PERIOD ){ TimeChange(  0.01 ); return; } // > .
		if( key == VK_OEM_2      ){ TimeReset();         return; } // ? /
		#else
		if( Buttons[B_TIME].active ){
			//if( key == VK_OEM_COMMA  ){ TimeChangeIndex(-1); return; } // < ,
			//if( key == VK_OEM_PERIOD ){ TimeChangeIndex( 1); return; } // > .
			//if( key == VK_OEM_2      ){ TimeReset();         return; } // ? /
			if( key == VK_OEM_PERIOD ){ TimeToggle(); return; } // > .
		}
		#endif

		int beltItemNumberToUse = 0;
			switch (key) {
			case VK_67_C_KEY: if (!CurrentDialogIndex) { ToggleCharPanel(); } break;
			case VK_73_I_KEY: if (!CurrentDialogIndex) { ToggleInventoryPanel(); } break;
			case VK_86_V_KEY: ChangeAltWeaponPairToNext(!IsINVPanelVisible); break;
			case VK_189_OEM_MINUS_KEY or VK_SUBTRACT: if (AutomapMode) { ZoomAutomapMinus(); } break; // shift + "-"
			case VK_187_OEM_PLUS_KEY or VK_ADD: if (AutomapMode) { ZoomAutomapPlus(); } break; // "+" without shift
			case VK_49_1_KEY or VK_NUMPAD1: beltItemNumberToUse = 1; break; // shift + 1
			case VK_50_2_KEY or VK_NUMPAD2: beltItemNumberToUse = 2; break; // shift + 2
			case VK_51_3_KEY or VK_NUMPAD3: beltItemNumberToUse = 3; break; // shift + 3
			case VK_52_4_KEY or VK_NUMPAD4: beltItemNumberToUse = 4; break; // shift + 4
			case VK_53_5_KEY or VK_NUMPAD5: beltItemNumberToUse = 5; break; // shift + 5
			case VK_54_6_KEY or VK_NUMPAD6: beltItemNumberToUse = 6; break; // shift + 6
			case VK_55_7_KEY or VK_NUMPAD7: beltItemNumberToUse = 7; break; // shift + 7
			case VK_56_8_KEY or VK_NUMPAD8: beltItemNumberToUse = 8; break; // shift + 8
			case VK_192_TILDA: // th1 doesn't have status string functional (?)
				char buf[100]; // for status string (~)
				GameMode == GM_COLISEUM ? sprintf(buf, "%s, mode = %s", THE_HELL_VERSION_HUMAN_STRING, getGameModeName(player.gameMode)) :
				sprintf(buf, "%s, difficulty = %s, mode = %s", THE_HELL_VERSION_HUMAN_STRING, DifName(), getGameModeName(player.gameMode)); // TheHellVersion
				NetSendString(1 << CurrentPlayerIndex, buf);
				break;
			}
		if( beltItemNumberToUse != 0 ){
			int beltItemIndexToUse = beltItemNumberToUse - 1;
			if( not( player.BeltInventory[beltItemIndexToUse].ItemCode, IC_M1_NONE, IC_11_GOLD) ){
				// если курсор с предметом или спец.указателем,
				// то с пояса по кнопкам используем только предметы без таргета
				if (Cur.GraphicsID != CM_1_NORMAL_HAND
					&& not(player.BeltInventory[beltItemIndexToUse].MagicCode,
						MC_2_POTION_OF_FULL_HEALING,
						MC_3_POTION_OF_HEALING,
						MC_6_POTION_OF_MANA,
						MC_7_POTION_OF_FULL_MANA,
						MC_18_POTION_OF_REJUVENATION,
						MC_19_POTION_OF_FULL_REJUVENATION,
						MC_21_RELIC_NEED_NO_TARGET) )
					return;
				RightClickOnItem( CurrentPlayerIndex, IS_50_47_Belt_Start + beltItemIndexToUse );
			}
			return;
		}
	}
}

//----- (0040A8FC) -------------------------------------------------------- game
void HandlePause()
{
	if( MaxCountOfPlayersInGame <= 1 ){
		if( GamePaused ){
			GamePaused = 0;
		}else{
			bool isPauseDisabled = is( GameMode, GM_NIGHTMARE, GM_IRONMAN ) && IsSomeMonstersActivated() // Pause disabled in battle Mordor: Survival mode does NOT need this limitation
			                    || is( GameMode, GM_SPEEDRUN ); // pause is always disabled in speedrun
			if (!isPauseDisabled) {
					GamePaused = 2;
					StopAllSounds();
					TrackInit(false);
			}
		}
	}
}

// handle ascii code from WM_CHAR, may add handy localization, but better work with scancodes
//----- (0040A938) -------------------------------------------------------- interface
void __fastcall HandleCharHotkey( int keyChar )
{
	if( IsGameMenuActive() || PutNewCharInTalkPanelPromt( keyChar ) || NetCursorItemId || IsPlayerDead ){
		return;
	}
	if( GamePaused != 2 ){
		if( IsPopupScreenOpen ){
			ClosePopupScreen();
			return;
		}
		if( IsGoldSplitPanelVisible ){
			GoldSplitHandle( keyChar );
			return;
		}
	}
}

//----- (0040AE68) -------------------------------------------------------- game
void LoadLvlGFX()
{
	switch( Dungeon->graphType ){
	case DT_0_TOWN:
		CurrentLevelCELFile = (int*)LoadFile("NLevels\\TownData\\Town.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("NLevels\\TownData\\Town.TIL");
		CurrentLevelMINFile = (char*)LoadFile("NLevels\\TownData\\Town.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\TownData\\TownS.CEL");
		break;
	case DT_1_CHURCH:
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L1Data\\L1.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L1Data\\L1.TIL");
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L1Data\\L1.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL");
		break;
	case DT_5_CRYPT:
		CurrentLevelCELFile = (int*)LoadFile("NLevels\\L5Data\\L5.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("NLevels\\L5Data\\L5.TIL");
		CurrentLevelMINFile = (char*)LoadFile("NLevels\\L5Data\\L5.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("NLevels\\L5Data\\L5S.CEL");
		break;
	case DT_2_CATACOMB:
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L2Data\\L2.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L2Data\\L2.TIL");
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L2Data\\L2.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L2Data\\L2S.CEL");
		break;
	case DT_3_CAVE:
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L3Data\\L3.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L3Data\\L3.TIL");
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L3Data\\L3.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL");
		break;
	case DT_6_ABYSS:
		CurrentLevelCELFile = (int*)LoadFile("NLevels\\L6Data\\L6.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("NLevels\\L6Data\\L6.TIL");
		CurrentLevelMINFile = (char*)LoadFile("NLevels\\L6Data\\L6.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L1Data\\L1S.CEL");
		break;
	case DT_4_HELL:
		CurrentLevelCELFile = (int*)LoadFile("Levels\\L4Data\\L4.CEL");
		CurrentLevelTilesFile = (char*)LoadFile("Levels\\L4Data\\L4.TIL");
		CurrentLevelMINFile = (char*)LoadFile("Levels\\L4Data\\L4.MIN");
		CurrentLevelSCELFile = (char*)LoadFile("Levels\\L2Data\\L2S.CEL");
		break;
	default:
		TerminateWithError("LoadLvlGFX");
		break;
	}
}

//----- (0040B02D) -------------------------------------------------------- game
void LoadMissilesAndObjects()
{
	ProgressUpdate();
	ProgressUpdate();
	LoadObjectSprites();
	ProgressUpdate();
	LoadMissileGFX();
	ProgressUpdate();
}

//----- (0040B05A) -------------------------------------------------------- land
void __fastcall CreateLevel(int lvlDir)
{
	switch( Dungeon->genType ){
	case DT_0_TOWN: // town
		CreateTown(lvlDir);
		AddWarpsToTown();
		break;
	case DT_1_CHURCH: // church or crypt
		CreateChurchTypeLabyrinth(DungeonRandSeed[(DUNGEON)Dungeon], lvlDir);
		AddWarpsToChurchTypeLabyrinth();
		ClearAreasAroundWarps();
		break;
	case DT_2_CATACOMB: // catacomb
		CreateCatacombTypeLabyrinth(DungeonRandSeed[(DUNGEON)Dungeon], lvlDir);
		AddWarpsToCatacombTypeLabyrinth();
		ClearAreasAroundWarps();
		break;
	case DT_3_CAVE: // cave or abyss
		CreateCaveTypeLabyrinth(DungeonRandSeed[(DUNGEON)Dungeon], lvlDir);
		AddWarpsToCaveTypeLabyrinth();
		ClearAreasAroundWarps();
		break;
	case DT_4_HELL: // hell
		CreateHellTypeLabyrinth(DungeonRandSeed[(DUNGEON)Dungeon], lvlDir);
		AddWarpsToHellTypeLabyrinth();
		ClearAreasAroundWarps();
		break;
	default:
		TerminateWithError("CreateLevel"); // TODO: warn about possible generation type expansion crash
	}
	SelectRandomLevelPalette(Dungeon->graphType);
}

//----- (0040B151) -------------------------------------------------------- game
void __fastcall LoadGameLevel(int startNewGame, int lvlDir)
{
    GameMode = Players[CurrentPlayerIndex].gameMode;
    EnforceNoSave = (GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN);
	MemorialSign = { -1 }; //Iron Man Memorial on map message clear 

	#if SPELL_QUEUE_AND_COOLDOWN_FIX
	// Additional safety measure to clear spell queue upon level change. Might fix the rare, misplaced TP location bug in Town in MP games.
	// for (int i = 0; i < PlayersMax_4; ++i) PlayersSpellcastQueue[i] = {}; // ? can lead to net desync
	PlayersSpellcastQueue[CurrentPlayerIndex] = {};
	#endif

	Cur.InventoryCellID = -1; // fixing annoying bug that made helmet tooltip show under cursor after starting the game
	PlayerLastDrawnHealth = -1; // fixing bug that shows hp and manaloss floats after playing with different character
	PlayerLastDrawnMana = -1; // same
	if( SomeLevelRandSeed ){
		DungeonRandSeed[(DUNGEON)Dungeon] = SomeLevelRandSeed;
	}
	if( startNewGame ){ StopMusic(); StopAmbient(); }
	SetCursorGraphics(CM_1_NORMAL_HAND);
	SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);
	ProgressUpdate();
	MakeLightTable(); // здесь задаются палитры разной яркости, из которых потом создаются тайлы разной яркости
	LoadLvlGFX();
	ProgressUpdate();
	if( startNewGame ){
		LoadPanelsGraphic();
		LoadFlipItemCELFiles();
		LoadSpeechPanelData();
		for( int i = 0; i < MaxCountOfPlayersInGame; i++ ){ // TODO: Выполнение IsSinglePlayerMode раз - ошибка?
			InitPlrGFXMem(i);
		}
		InitInterface();
		SetItem_Init();
		ResetHelp();
	}
	SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);
	if( Dungeon == DUN_0_TOWN /*|| Dungeon == DUN_35_PASSAGE_OF_FIRE*/ ){
		ShopItemsGenerationByPlayerLevel();
	}
	ProgressUpdate();
	InitDungeonAutomapData();
	if( Dungeon != DUN_0_TOWN && lvlDir != LVL_4_NODIR ){
		InitLighting();
		InitVision();
	}
	InitMonsterSlots();
	ProgressUpdate();
	ClearWarps();
	if( ! Dungeon->isQuest ){
		LoadQuestPatterns();
		CreateLevel(lvlDir);
		ProgressUpdate();
		FillSolidBlockTables();		
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
			if (is(Dungeon, DUN_9_CAVE_1, DUN_10_CAVE_2, DUN_11_CAVE_3, DUN_12_CAVE_4)) {
				Frozen_Loch_BlockSet();
			}
			else if (is(Dungeon, DUN_17_ABYSS_1, DUN_18_ABYSS_2, DUN_19_ABYSS_3, DUN_20_ABYSS_4)) {
				Ice_Age_Abyss_Jesus_mode();
			}
		}
		SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);
		if( Dungeon->genType != DT_0_TOWN ){ // TODO: check this out
			GetDungeonMonsterTypes();
			InitRooms();
			LoadMissilesAndObjects();
		}else{
			LoadMissileGFX();
		}
		ProgressUpdate();
		if( lvlDir == LVL_3_RTN || lvlDir == LVL_7_TWARPUP && Dungeon.prev == DUN_44_RAVENHOLM ){//This should also include other quest maps with entrances in town.
			GetReturnLvlPos();
		}
		if (lvlDir == LVL_5_WARP) {// Taking TP
			GetLevelEntryPoint();
			FixTPandStairEntryPoint(lvlDir);
		} else if (is(lvlDir, LVL_0_DOWN, LVL_1_UP, LVL_6_TWARPDN, LVL_7_TWARPUP)) {// Taking stairs.  MP quest update should include LVL_3_RTN = going from Quest map back to normal dungeon or town.  However, this is not ready yet as those MP maps with entrances in town (e.g. Disused Reliquary) do not have correct return coordinates set.
			// TODO: continue here, fix mp points from ravenoholm in mp
			FixTPandStairEntryPoint(lvlDir);// loadingMode parameter is used for fixing Crypt entrance shift so player won't end up inside the stair, etc.
		}
		ProgressUpdate();
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && Dungeon == player.dungeon ){
				InitPlayerGFX( playerIndex );
				if( lvlDir != LVL_4_NODIR ){
					InitPlayer(playerIndex, startNewGame);
				}
			}
		}
		PlayDungMsgs();
		InitMultiView();
		ProgressUpdate();

		// Resync after level changing bug
		// This bug occurs when 2 players change levels one after the other.  The first player
		// down thinks the other player is one level above, because the change level msg has
		// not been processed during loading. The second player knows that they are both
		// on the same level, so more players get inited, and randoms desync.
		SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);

        if( GameMode == GM_SPEEDRUN ){
            if( (DUNGEON)Dungeon % 4 == 1 ){ // TODO: continue here, change to save save/gamemode flag
                if( !Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] ){
                    AdvanceSpeedrunTimer();
                }
            }
        }

		if( Dungeon != DUN_0_TOWN ){
			if( startNewGame || lvlDir == LVL_4_NODIR || !Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] || MaxCountOfPlayersInGame != 1 ){
				// Save theme room areas
				HoldThemeRooms();
				DungeonSeedCheck1[(DUNGEON)Dungeon] = GetRndSeed(); // first dungeon generation check point for debug multi
				if ( MaxCountOfPlayersInGame == 1 
				 && (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_HARDCORE, GM_NIGHTMARE) )
				 && (Players[CurrentPlayerIndex].ClassID == PC_2_MAGE)
				 && (Dungeon >= DUN_9_CAVE_1 && Dungeon <= DUN_12_CAVE_4) ){
					// invis on enter works wrong, need another logic
					// IsPlayerLightRadiusChanged = 0; // Set player invisible to monsters on first entering to new level while do not start moving
				}
				// Init monsters into dungeon
				InitMonsters();
				DungeonSeedCheck2[(DUNGEON)Dungeon] = GetRndSeed(); // second dungeon generation check point for debug multi
				// Init objects
				InitObjects();
				// Init items
				InitItems();
				if( Dungeon < DUN_17_ABYSS_1 || Dungeon > DUN_20_ABYSS_4 ){
					FillAloneRooms();
				}
				DungeonSeedCheck3[(DUNGEON)Dungeon] = GetRndSeed(); // third dungeon generation check point for debug multi
				// Init missiles
				InitMissiles();
				// Init dead info, not dungeon layout though
				InitDead();
                InitBlood();
				DungeonSeedCheck4[(DUNGEON)Dungeon] = GetRndSeed(); // last dungeon generation check point for debug multi
				// if multiplayer resync level
				if( MaxCountOfPlayersInGame != 1 ){
                    DeltaLoadLevel();
				}
				ProgressUpdate();
				SavePreLighting();
			}else{
				// Init monsters so uniques are loaded
				InitMonsters();
				// Init missiles
				InitMissiles();
				// Init dead info, not dungeon layout though
				InitDead();
                InitBlood();
				ProgressUpdate();
				// Load where things were
				LoadLevel();
				ProgressUpdate();
			}
			if( MaxCountOfPlayersInGame == 1 ){
				ResyncQuests();
			}else{
				ChangeNearestQuestStatus();
				MPResyncQuests();
			}
		}else{ // DUN_0_TOWN
			// Make everything visible
			for( int col = 0; col < 112; col++ ){
				for( int row = 0; row < 112; row++ ){
					FlagMap[ row ][ col ] |= CF_64_VISIBLE_BY_CURSOR;
				}
			}
			ClearAllObjects();
			// Init town people
			InitTowners();
			// Init items
			InitItems();
			// Init missiles
			InitMissiles();
			ProgressUpdate();

			if( !startNewGame && lvlDir != LVL_4_NODIR && Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] ){
				if( MaxCountOfPlayersInGame != 1 ){
                    DeltaLoadLevel();
					ProgressUpdate();
				}else{
					LoadLevel();
				}
			}else{
				if( MaxCountOfPlayersInGame != 1 ){
                    DeltaLoadLevel();
				}
				ProgressUpdate();
			}
			if( MaxCountOfPlayersInGame == 1 ){
				ResyncQuests();
			}else{
				ChangeNearestQuestStatus();
				MPResyncQuests();
			}

		}
		ClearQuestPatterns();
	}else{ // Preset levels // Dungeon->isQuest
		LoadSetMap();
		ProgressUpdate();

		GetDungeonMonsterTypes();
		
		InitMonsters();

		LoadMissileGFX();

		InitDead();
        InitBlood();
		
		// Open Tiles file
		FillSolidBlockTables();
		if( Dungeon == DUN_39_FROZEN_LOCH || Dungeon == DUN_51_FETID_CAVERN
		/* || (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !IsQuestFloor) */ ) {
			Frozen_Loch_BlockSet();
		}
		ProgressUpdate();

		if (lvlDir == LVL_5_WARP) {
			GetLevelEntryPoint();
			FixTPandStairEntryPoint(lvlDir);//Apply player position non-collision fix for MP quest maps.
		} else if (lvlDir == LVL_2_SET) {//Going to MP quest map via stair, this also applies to, for example, going back and forth between Hall of Anguish and Andy Throne.
			FixTPandStairEntryPoint(lvlDir);
		}

		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && Dungeon == player.dungeon ){
				InitPlayerGFX(playerIndex);
				if( lvlDir != LVL_4_NODIR ){
					InitPlayer(playerIndex, startNewGame);
				}
			}
		}
		InitMultiView();
		ProgressUpdate();

		// New code for mp quest sync?  Yes, we now use MPResyncQuests().
		if( MaxCountOfPlayersInGame == 1 ){ // SP quest area
			if( startNewGame || lvlDir == LVL_4_NODIR || !Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] ){
				// Init items
				InitItems();
				SavePreLighting();
			}else{
				// Load where things were
				LoadLevel();
			}
		}else{ // MP quest area. DeltaLoadLevel() makes sure monsters and items are sync'd for quest area.  However, because quest area and its base Dlvl share the same sgLevels[], now monsters killed in base Dlvl will drop items and decrease monster count in quest area, and vice versa.  Player may not be able to finish the quest cuz monster count cannot drop to 0 due to shared sgLevels[].  Must separate sgLevels[] for quest area and its base Dlvl!
			if( startNewGame || lvlDir == LVL_4_NODIR || !Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] ){
				InitItems();
			}
			SavePreLighting(); // No dark square around player in PWS
			DeltaLoadLevel(); // Ensures monster count and item drop sync
			MPResyncQuests();
		}

		// Init missiles
		InitMissiles();
		ProgressUpdate();
		SetReturnDungeonPos(Dungeon); // reset return point from quest on load (and return?)
	}
	SyncPortals();

	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && player.dungeon == Dungeon && !player.lvlChanging || playerIndex == CurrentPlayerIndex ){
			if( player.CurLife <= 0 ){
				FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
			}else{
				if( MaxCountOfPlayersInGame == 1 ){
					PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
				}else{
					SyncInitPlrPos(playerIndex);
				}
			}
		}
	}

	if( Dungeon != DUN_0_TOWN ){
		InitDungeonTiles();
	}
	InitLightMax();
	ProgressUpdate();
	ProgressUpdate();//This is called twice in a row, why?
	if( startNewGame ){
		MayBeViewInit();
		ProgressUpdate();
	}
	// TODO: continue here, check all place where genType check instead of Dungeon for RavenHolm
	if( Dungeon != DUN_0_TOWN ){
		ProcessLightList();
		ProcessVisionList();
	}

	if(0){ // TODO: unused ?
		Quest& fleshdoomQuest = Quests[Q_21_THE_CURSED_BLADE];
		if( fleshdoomQuest.status == QS_3_COMPLETE && Dungeon == DUN_24_CRYPT_4 ){
			LoadOpennedUberRoom();
		}
	}

	if( lvlDir == LVL_4_NODIR ) srand(NonGameplaySeed); // restore music on load game
	PlayMusic();
	PlayAmbient();

	while( !ProgressUpdate() ); // Что будет выполняться в цикле? Нужно это проверить пошагово
	// Скорее всего полоску которая ещё не дошла до конца, доведут до него.

	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( Dungeon->isQuest && Dungeon == DUN_25_KING_LEORICS_TOMB && kingLeoricQuest.status == QS_2_IN_PROGRESS ){
		PlayGlobalSound(S_962_M_SKING01);
	}
	if( Dungeon == DUN_15_HELL_3 && MaxCountOfPlayersInGame != 1 ){
		for (int monsterIndex = SummonMonsters_Count; monsterIndex < Monsters_Max_Count; monsterIndex++) {
			Monster& monster = Monsters[monsterIndex];
			if (monster.Name == UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus].NamePtr && monster.CurrentLife <= 0) {//Need to check for bugged invincible state (HP < 0), so players can proceed to Dlvl 16.
				for( int row = 0; row < 112; ++row ) for( int col = 0; col < 112; ++col ){
					if (FineMap[row][col] == 370) AddWarp(row, col, WM_1026_NEXT_DUNGEON);
				}
			}
		}
	}
	ValidateMonsters();
	//if (OverwriteGolemHP && GolemCurrentHealthDuringLevelTransition > 0 && DungeonLevel) {
	//	int someInt = PlayerCastSummaryLevel(CurrentPlayerIndex, PS_21_GOLEM);
	//	NetSendCmdLocParam2(1, CMD_SPELLXY, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, PS_21_GOLEM, someInt);
	//	//CastPlayerSpell(CurrentPlayerIndex, PS_21_GOLEM, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, CursorX,CursorY, 0, 38); // didn't work alone for some reason and after previous line created second golem
	//}
}

//----- (0040B6F9) -------------------------------------------------------- game
void __fastcall GameLoop(int startup) // game_loop
{
	int maxLoops = startup ? ENGINE_FPS * 3 : 3; // MAX_CONSECUTIVE_LOOPS = 3
	while( maxLoops-- ){
		
		// wait for synchronous network message
		if( ! NetEndSendCycle() ){
			TimeOutCursor(true);
			break;
		}
		TimeOutCursor(false);
		
		// run logic at ENGINE_FPS
		GameLogic();
		
		if( !gbRunGame // if the game mode changed, then don't continue loop
		 || MaxCountOfPlayersInGame == 1 // don't run multiple loops in single player mode
		 || ! NThreadRunGameLoop(true) ){ // have we been in the loop too long
			break;
		}
	}
}

//----- (th2) -------------------------------------------------------------
bool IsPausedInMenu()
{
	return MaxCountOfPlayersInGame == 1 && !is( GameMode, GM_NIGHTMARE, GM_IRONMAN, GM_SPEEDRUN ) && IsGameMenuActive(); // Mordor: don't change
}

//----- (0040B74C) -------------------------------------------------------- game
void GameLogic()
{
	ColorShifter++;
	if( is( GameMode, GM_SPEEDRUN ) ){
        if( !UberDiabloKilled && !( Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead ) ){
            ++SpeedrunTimer;
        }
    }
    
	if( GamePaused != 2 ){
		if( GamePaused == 1 ) GamePaused = 2;
		if( ! IsPausedInMenu() ){
			if( !IsGameMenuActive() && !NetCursorItemId ){
				DispatchCursorAboveIsoland();
                TrackMouse();
			}
			if( BoolWithPlayer ){
				#ifdef OLD_32_BIT_RNG
				if( MaxCountOfPlayersInGame == 1 ){ // this "feature" can break network syncronization
					int count = RNG(10); // Make random generator even less predictable
					while(count--){
						GetRndSeed();
					}
				}
				#endif
				WorldFrameIndex++;
				ProcessPlayers();
			}
			DamageMonstersByAuras(); // There is a little bit righter place for this, so you need to modify FPS dependency in this, it calls 20 times per second
			if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_20_RAVENHOLM*/ ){ // not town
				ProcessMonsters();
				ProcessObjects();
				ProcessMissiles();
				ProcessItems();
				ProcessLightList();
				ProcessVisionList();
			}else{ // town
				ProcessTowners();
				ProcessItems();
				ProcessMissiles();
			}
			UpdateSound();
			CheckPlayerStatusMessagesTimeout();
			CheckTriggers();
			QuestsActions();
			if( MaxCountOfPlayersInGame != 1 
				|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_1_AUTO_SAVE))
				|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)) ){        // network game
				i64 tickCount = GetTickCountTh(); // сетевая игра, перехватываем вызов
				if( tickCount - AutoSaveTickCount > 60000_ms ){ // once per minute
					AutoSaveTickCount = tickCount;
					UpdatePlayerFile();
				}
			}
		}
	}
}

//----- (0040B800) -------------------------------------------------------- game
void __fastcall TimeOutCursor(bool timeout)
{
	if( timeout ){
		if( !NetCursorItemId && !MouseButtonPressed ){
			NetCursorItemId = Cur.GraphicsID;
            multi_net_ping();
			InfoPanel_ClearBody();
			InfoPanel_AddLine("-- Network timeout --", true);
			InfoPanel_AddLine("-- Waiting for players --", true);
			SetCursorGraphics(CM_11_SAND_GLASS);
		}
		DrawNotGameplayScreen(1);
	}else if( NetCursorItemId ){
		SetCursorGraphics( NetCursorItemId );
		NetCursorItemId = 0;
		InfoPanel_ClearBody();
	}
}

//----- (0040B884) -------------------------------------------------------- land
void AnimateLevelPalette()
{
	i64 battletrance = BattleTrance ? (MAX_FPS / ENGINE_FPS) : 0_ms;
	i64 tick = GetTickCountTh(); // это таймер не влияет на гемплей, не перехватываем
	if( tick - PaletteFrameSync >= battletrance + (MAX_FPS / ENGINE_FPS)){
		PaletteFrameSync = tick;
		if( Dungeon->genType == DT_4_HELL ){
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
				// do not animate
			}
			else{
				AnimateHellPalette();
			}
		}else if( Dungeon->graphType == DT_5_CRYPT ){
			if (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) || Dungeon->isQuest) {
				AnimateCryptPalette();
			}
		}else if( Dungeon->graphType == DT_6_ABYSS ){
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
				// skip animating water in abyss if Ice Age and not quest floor
			}
			else {
				AnimateAbyssPalette();
			}
		}
		else if (Dungeon->genType == DT_3_CAVE) {
			if (!UseReservedSystemPalette) {
				AnimateCavePalette();
				// превращение лавы в лед, Lost Treasure Quest
			}
			else if (Dungeon->isQuest && Dungeon == DUN_39_FROZEN_LOCH) {
				// don't animate
			}
			else if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
				// no anime
			}
			else if( !(Dungeon->isQuest && Dungeon == DUN_39_FROZEN_LOCH) ){
				AnimateCavePalette();
			}
		}
	}
}

//----- (0040B8D7) -------------------------------------------------------- player
void AllocPlayers()
{
	Players = AllocPlayersChunk(nullptr);
	if( !Players ){
		TerminateWithError("Unable to initialize memory");
	}
	memset(Players, 0, sizeof(Player) * (PlayersMax_4 + 1)); // 5-th player struct reserved for special save operations
}

//----- (0040B908) -------------------------------------------------------- player
Player* __fastcall AllocPlayersChunk(Player* memory)
{
	Player* v1; // edi@1
	void* v3; // ebx@1
	Player* v4; // ebp@1
	Player* result; // eax@4

	v1 = memory;
	v3 = malloc( rand() & 0x7FFF );
	v4 = (Player*) malloc( sizeof(Player) * (PlayersMax_4 + 1) ); // 5-th player struct reserved for special save operations
	if( v3 ){
		free( v3 );
	}
	if( v4 ){
		if( v1 ){
			memcpy( v4, v1, sizeof(Player) * (PlayersMax_4 + 1) ); // 5-th player struct reserved for special save operations
			free( v1 );
		}
		result = v4;
	}else{
		result = v1;
	}
	return result;
}

//----- (th2) --------------------------------------------------------
void FreeCharacterTable()
{
	if( Players ){
        free(Players);
	}
}

//----- (th2) --------------------------------------------------------
void ProcessBerserk(Player& player, bool& autoClick)
{
	if( HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (player.activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){

		bool isRecalcAllTargetsDoneForThisLoopAlready = false;

		if (InitBerserk) {
			InitBerserk = false;
			BerserkInit();
			BerserkRecalcAllTargets(player);
			isRecalcAllTargetsDoneForThisLoopAlready = true;
		}

		Berserk = true;
		autoClick = false;

		int closestDistanceMonsterIndex = -1;
		int closestDistanceObjectIndex = -1;
		uint closestMonsterDistance = -1;
		uint monsterDistance = -1;
		uint closestObjectrDistance = -1;
		uint objectDistance = -1;
		int deltaRow;
		int deltaCol;

		if ((BerserkPlayer.LastRow != player.Row) || (BerserkPlayer.LastCol != player.Col)) {
			BerserkRecalcAllTargets(player);
			isRecalcAllTargetsDoneForThisLoopAlready = true;
		}

		for (int i = 0; i < MonstersCount; i++)
		{
			int monsterIndex = MonsterIndexes[i];
			Monster& monster = Monsters[monsterIndex];

			if (((BerserkMonsters[i].LastRow != monster.NextRow) || (BerserkMonsters[i].LastCol != monster.NextCol)) && !isRecalcAllTargetsDoneForThisLoopAlready) {
				BerserkRecalcSingleMonster(player, i);
			}

			if (monster.goal != MG_0_DEATH 
				&& monster.goal != MG_7_WAIT_TO_TALK  // mor: might wanna add state 6 here too...
				&& BerserkMonsters[i].isReachable && !IsPlayerSummon(monsterIndex)) {
				deltaRow = abs(monster.NextRow - player.NextRow);
				deltaCol = abs(monster.NextCol - player.NextCol);
				monsterDistance = deltaRow < deltaCol ? deltaCol : deltaRow;
				if (monsterDistance < closestMonsterDistance) {
					closestMonsterDistance = monsterDistance;
					closestDistanceMonsterIndex = monsterIndex;
				}
			}
		}

		if (closestDistanceMonsterIndex != -1) {
			NetSendCmdParam1(1, CMD_18_ATTACKID, closestDistanceMonsterIndex);
		}

		if (closestDistanceMonsterIndex == -1) {
			for (int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++) {

				int objectIndex = ObjectActive[objectIndexIndex];
				Object& object = Objects[objectIndex];

				int baseObjectIndex = object.BaseObjectIndex;
				deltaRow = abs(object.Row - player.NextRow);
				deltaCol = abs(object.Col - player.NextCol);

				if (!is(baseObjectIndex, BO_1_CHURCH_DOOR_MAIN_DIAGONAL, BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL, BO_42_CATACOMB_DOOR_MAIN_DIAGONAL, BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL, BO_74_CAVE_DOOR_MAIN_DIAGONAL, BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL, BO_57_POD_URN_BARREL_WITH_DROP, BO_58_POD_URN_BARREL_EXPLOSIVE)) {
					continue;
				}
				if (is(baseObjectIndex, BO_1_CHURCH_DOOR_MAIN_DIAGONAL, BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL, BO_42_CATACOMB_DOOR_MAIN_DIAGONAL, BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL, BO_74_CAVE_DOOR_MAIN_DIAGONAL, BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL)
					&& object.OpenState != OS_0_CLOSED) {
					continue;
				}
				if (is(baseObjectIndex, BO_57_POD_URN_BARREL_WITH_DROP, BO_58_POD_URN_BARREL_EXPLOSIVE)
					&& object.destructable == -1) {
					continue;
				}
				if (!BerserkObjects[objectIndexIndex].isReachable) {
					continue;
				}

				objectDistance = deltaRow < deltaCol ? deltaCol : deltaRow;
				if (objectDistance < closestObjectrDistance) {
					closestObjectrDistance = objectDistance;
					closestDistanceObjectIndex = objectIndex;
				}
			}
			if (closestDistanceObjectIndex != -1) {
				Object& object = Objects[closestDistanceObjectIndex];
				NetSendCmdLocParam1(1, CMD_16_OPOBJXY, object.Row, object.Col, closestDistanceObjectIndex);
			}
		}

		isRecalcAllTargetsDoneForThisLoopAlready = false;
		
		
		if (player.CurAction == PCA_0_STAND) {
			DelayIdleRoutine++;
			if (DelayIdleRoutine == 5) {
				DelayIdleRoutine = 0;
			}
			if (!DelayIdleRoutine) {
				BerserkRecalcAllTargets(player);
			}
			Berserk = false;
		}
	}
	else {
		Berserk = false;
	}
}

//----- (th2) --------------------------------------------------------
void BerserkRecalcAllTargets(Player& player)
{
	for( int i = 0; i < MonstersCount; i++ ){
		BerserkRecalcSingleMonster(player, i);
	}
	for( int i = 0; i < ObjectsCount; i++ ){
		BerserkRecalcSingleObject(player, i);
	}
	BerserkPlayer.LastRow = player.Row;
	BerserkPlayer.LastCol = player.Col;
}

//----- (th2) --------------------------------------------------------
void BerserkInit()
{
	for (int i = 0; i < Monsters_Max_Count; i++)
	{
		BerserkMonsters[i].index = i;
		BerserkMonsters[i].LastRow = -1;
		BerserkMonsters[i].LastCol = -1;
		BerserkMonsters[i].isReachable = false;
	}
	for (int i = 0; i < MaxObjects_127; i++)
	{
		BerserkObjects[i].index = i;
		BerserkObjects[i].LastRow = -1;
		BerserkObjects[i].LastCol = -1;
		BerserkObjects[i].isReachable = false;
	}
}

//----- (th2) --------------------------------------------------------
void BerserkRecalcSingleMonster(Player& player, int i)
{
	char steps[28];
	int monsterIndex = MonsterIndexes[i];
	Monster& monster = Monsters[monsterIndex];
	BerserkMonsters[i].LastRow = monster.NextRow;
	BerserkMonsters[i].LastCol = monster.NextCol;
	BerserkMonsters[i].isReachable = FindPath(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.NextRow, monster.NextCol, player.NextRow, player.NextCol, steps);
}

//----- (th2) --------------------------------------------------------
void BerserkRecalcSingleObject(Player& player, int i)
{
	char steps[28];
	int objectIndex = ObjectActive[i];
	Object& object = Objects[objectIndex];
	BerserkObjects[i].LastRow = object.Row;
	BerserkObjects[i].LastCol = object.Col;
	BerserkObjects[i].isReachable = FindPath(CheckCellForMeleeMonsterSafeMove, 0, object.Row, object.Col, player.NextRow, player.NextCol, steps);
}

//----- (th2) --------------------------------------------------------
bool IsNoNeedToDropItemUsingCtrl()
{
	return DropItemOnCtrl && (!CursorInMainPanel() && ((IsINVPanelVisible && !CursorIntoDisplayObject(InventoryPanelRect)) || !IsINVPanelVisible));
}

bool PrevRestrictState = false;
//----- (th2) --------------------------------------------------------
void RestrictMouseToWindow(HWND mWindow)
{
	bool restrictState = IsRestrictMouseToWindow && IsMainWindowActive && !IsGameMenuActive() && !GamePaused && gbRunGame;
	if( restrictState != PrevRestrictState ){
		PrevRestrictState = restrictState;
		if( restrictState && !IsSizingWindow ){
			RECT rect;
			GetClientRect(mWindow, &rect);
			POINT ul { rect.left, rect.top };
			POINT lr { rect.right, rect.bottom };
			MapWindowPoints(mWindow, nullptr, &ul, 1);
			MapWindowPoints(mWindow, nullptr, &lr, 1);
			rect = { ul.x, ul.y, lr.x, lr.y };
			ClipCursor(&rect);
		}else{
			ClipCursor(0);
		}
	}
}