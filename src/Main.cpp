#include "stdafx.h"
#include "AnimOrbs.h"

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
		item += *(short*)item; // (rt) start of the sprite's pixels

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
	if( !Classic_Item_Sprites ) Classic_Item_Sprites = (char*) LoadFile( "Data\\Inv\\Objcurs.CEL" ); // diablo.mpq
	//(rt) if( !AddonItemSprites ) AddonItemSprites = (char*) LoadFile( "Data\\Inv\\Objcurs2.CEL" ); // hellfire.mpq isn't used, it's not in the distribution, thdata.mor is the same thing modified
	if( !Invobj1_item_sprites ) Invobj1_item_sprites = (char*) LoadFile( "Data\\Inv\\invobj1.CEL" ); // thdata.mor
	if( !TH2objcurs_Item_Sprites ) TH2objcurs_Item_Sprites = (char*) LoadFile( "data\\inv\\TH2objcurs.cel" ); // thdata.mor, th3data.mor
	if( !TH3objcurs_Item_Sprites ) TH3objcurs_Item_Sprites = (char*) LoadFile( "data\\inv\\TH3objcurs.cel" ); // th3data.mor
	ZeroCursorImageWidth();
	InitItemsCel(TH2objcurs_Item_Sprites, 168 + 12);
	InitItemsCel(TH3objcurs_Item_Sprites, 3716 + 168 + 12 + 1);
	AnimOrbs_Init(); // decode animated life/mana orb frames from the MPQ
}

//----- (00407A6D) -------------------------------------------------------- interface infopanel
void FreeItemsCELArrays()
{
	FreeMemZero( Classic_Item_Sprites );
	//FreeMemZero( AddonItemSprites );
	FreeMemZero( Invobj1_item_sprites);
	FreeMemZero( TH2objcurs_Item_Sprites );
	FreeMemZero( TH3objcurs_Item_Sprites );
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

// (rt) many jmps and calls to both functions here; may be fragile in the comparison
//----- (00407ACA) --------------------------------------------------------
//(rt) ----- (00407AED) --------------------------------------------------------// jump to 00407ACA
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
			|| cursorRow == row && cursorCol == col ){// (rt) the triangle above the cursor is checked
			IswarpUnderCursor = true;
			InfoPanel_ClearBody();
			if (Dungeon == DUN_0_TOWN && MaxCountOfPlayersInGame == 1 && GameMode != GM_CLASSIC) {
				// Single-player (non-classic) town portal: show where it leads instead of who cast it.
				strcpy(InfoPanelHeader, (char*)L("Town Portal"));
				sprintf(InfoPanelBuffer, L("To %s"), LocalizedWarpNameAccusative(Dungeons[Portals[spell.CasterIndex].dungeon].warpName));
				InfoPanel_AddLine(InfoPanelBuffer, true);
			}
			else {
				strcpy(InfoPanelHeader, (char*)L("Town Portal"));
				sprintf(InfoPanelBuffer, L("from %s"), Players[spell.CasterIndex].playerName);
				InfoPanel_AddLine(InfoPanelBuffer, true);
			}
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
		 || cursorRow == row && cursorCol == col ){// (rt) the triangle above the cursor is checked
			// (th4) DARK CAMPAIGN: mission portals -- both the outbound one in the Hideout and
			// the return one in a cleared mission level -- are labelled by their own handler
			// (ShowDungeonEntryDescriptions_DarkLordHideout / _remain) so they name the real
			// destination. This generic handler hardcodes "level 15" for any quest floor and
			// spans a several-tile block; both wrong for a single-tile mission portal.
			if( GameMode == GM_DARK_CAMPAIGN ) continue;
			IswarpUnderCursor = true;
			InfoPanel_ClearBody();
			strcpy(InfoPanelHeader, (char*)L("Portal to"));
			char* levelName;
			if( Dungeon->isQuest ){
				if( is (Dungeon, DUN_44_RAVENHOLM, DUN_46_COLISEUM) ){
					levelName = (char*)L("Tristram");
				}else{
					levelName = (char*)L("level 15");
				}
			}else if( Dungeon == DUN_0_TOWN ){
				levelName = (char*)L("RavenHolm");
			}else{
				levelName = (char*)L("The Unholy Altar");
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
// (th4) Total THORNS damage for the character panel, summed into ONE X-Y range.
// Returns false when no line should be drawn.
//
// Iron Maiden only: thorns is not class-gated in the engine, but she is the class built
// around it and Ibragim asked for the line to be hers alone.
//
// THORNS IS PER ACTION, NOT PER ITEM. Item.cpp writes a SEPARATE EffectAction for each
// thorns source: the base item roll (~3656, chance hardcoded 100) and every thorns AFFIX
// (~3585, chance copied off the affix). One shield can therefore contribute TWO actions --
// e.g. a Buckler with base "Thorns damage 29-34" plus a "Barbs" affix "+(17-20), chance
// 96%". OnTakeMeleeTrigger walks them all and rolls RNG(100) < chance for EACH, so one
// incoming melee hit can fire PvM_Melee(isThorns=true) several times. Each of those calls
// adds the flat PERK_ANTISOCIAL and the A-Rose-With-Thorns CharLevel/2 bonus of its own
// (Player.cpp ~5158) -- so the flat bonus is counted once PER ACTION, which is why a
// single two-action shield counts it twice. That is the engine's behaviour; note the
// Antisocial perk text says "per armor piece", so the two disagree.
//
// EXPECTED VALUE (Ibragim's choice). Each action is scaled by its own proc chance, with
// the flat bonus INSIDE the scaling -- the engine only adds it inside PvM_Melee, which
// only runs when that action's proc landed:
//     contribution = (roll + flatBonus) * chance / 100
// The midpoint of the printed range is then the true average damage per hit. Trade-off
// accepted when choosing this: the printed MAX is no longer a number that appears on
// screen -- a landed "Lances" proc (515-750 at 11-15% chance) really deals ~700 while it
// contributes ~90 here. The line reads as sustained output, not as one hit.
// Ignoring chance instead would overstate a Lances build roughly sixfold.
//
// NOT included, checked deliberately: Bouncer/Destroyer/Frenzied sit in the non-thorns
// branch; the DREAD and RUIN item effects (EA_RANDOM_DAMAGE, EA_MORE_DAMAGE_CHANCE) do
// reach thorns but Ibragim asked to skip them; Wrecking Block is a different mechanic
// entirely (it reflects the MONSTER's attack roll, not your thorns).
//
// PRE-MITIGATION, like every other damage line in this panel: the to-hit roll, the
// monster block check, per-monster resistances, the boss -50% and Old Habit x3 all
// happen later in PvM_Melee and none of them are knowable without a target.
static bool CalcThornsDamage_Display( int playerIndex, int& minOut, int& maxOut )
{
	const Player& player = Players[ playerIndex ];
	if( player.fullClassId != PFC_IRON_MAIDEN ) return false;
	const int flatBonus = PerkValue( PERK_ANTISOCIAL, playerIndex )
	                    + ( HasTrait( playerIndex, TraitId::ARoseWithThorns ) ? player.CharLevel / 2 : 0 );
	bool any = false;
	int mn = 0, mx = 0;
	for( int i = 0; player.effectAction[i].id != EA_NO; ++i ){   // same walk OnTakeMeleeTrigger does
		const EffectAction& a = player.effectAction[i];
		if( a.id != EA_THORNS ) continue;
		any = true;
		int chance = a.chance;
		LimitToRange( chance, 0, 100 );
		mn += ( a.minVal + flatBonus ) * chance / 100;
		mx += ( a.maxVal + flatBonus ) * chance / 100;
	}
	if( !any ) return false;                                    // no thorns equipped: no line
	minOut = mn; maxOut = mx;
	return true;
}

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
	DynZoom_ScreenToWorld( curX, curY );	// (th4) DYNAMIC ZOOM: back to the 1x world frame first (the zoom is applied after the warp)
	Perspective_ScreenToWorld( curX, curY );	// (th4) clicks follow the warped world (Draw.cpp)
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
		// (rt) show the hint if over the panel
		if( CursorInMainPanel() ){
			PopUpHelpOnButton();
			return;
		}
		if( IsPopupScreenOpen ){
		    return;
        }
		if( IsChatHistoryVisible && CursorIntoDisplayObject(ChatHistoryRect) ) return;
		
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
		if (IsPowerupPanelVisible && CursorIntoDisplayObject(PowerupPanel)) {
			Powerup_MouseMove();
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
                        sprintf(InfoPanelBuffer, L("Current %s: %i"), name, value );
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
				Tooltip_AddLine(L("strength affects:"), C_1_Blue);
				if (not(player.fullClassId, PFC_TRAPPER, PFC_BOMBARDIER, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER,
					PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					if (is(player.fullClassId, PFC_ROGUE, PFC_DRUID) || HasTrait(CurrentPlayerIndex, TraitId::Paladin) || HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
						Tooltip_AddLine(L("damage in melee combat"), C_0_White);
					}
					else if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
						Tooltip_AddLine(L("damage in melee and ranged combat"), C_0_White);
					}
				}
				if ((is(player.ClassID, PC_0_WARRIOR, PC_3_MONK, PC_4_ROGUE, PC_5_SAVAGE) || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) && GameMode != GM_CLASSIC) {
					Tooltip_AddLine(L("knockback resistance"), C_0_White);
				}
				if(is(player.fullClassId, PFC_WARRIOR, PFC_TEMPLAR, PFC_INQUISITOR, PFC_MONK, PFC_SHINOBI, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR)
					&& (!HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger))) {
					Tooltip_AddLine(L("melee hit block chance"), C_0_White);
				}
				Tooltip_AddLine(L("ability to equip items"), C_0_White);
			}
			else if (CursorIntoDisplayObject(CharPanelBaseMagRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();				
				Tooltip_AddLine(L("magic affects:"), C_1_Blue);
				if (!(player.gameChanger & BIT(GC_27_TERMINATOR))) {
					Tooltip_AddLine(L("maximum mana points"), C_0_White);
					Tooltip_AddLine(L("spell accuracy"), C_0_White);
					Tooltip_AddLine(L("spell damage"), C_0_White);
					if (GameMode != GM_COLISEUM && GameMode != GM_CLASSIC) {
						Tooltip_AddLine(L("mana regeneration speed"), C_0_White);
					}
					if (is(player.fullClassId, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER)) {
						Tooltip_AddLine(L("summoned minion stats"), C_0_White);
					}
				}
				if (GameMode != GM_CLASSIC) { Tooltip_AddLine(L("ability to equip items"), C_0_White); }
			}
			else if (CursorIntoDisplayObject(CharPanelBaseDexRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				Tooltip_AddLine(L("dexterity affects:"), C_1_Blue);
				if (not(player.fullClassId, PFC_TRAPPER, PFC_BOMBARDIER, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, 
					PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
						if (player.gameChanger & BIT(GC_28_SUPERMUTANT)) {
							Tooltip_AddLine(L("accuracy with minigun"), C_0_White);
							Tooltip_AddLine(L("damage with minigun"), C_0_White);
						}
						else if (player.gameChanger & BIT(GC_27_TERMINATOR)) {
							Tooltip_AddLine(L("accuracy with weapons"), C_0_White);
							Tooltip_AddLine(L("damage with weapons"), C_0_White);
						}
						else {
							Tooltip_AddLine(L("accuracy with pistols"), C_0_White);
							Tooltip_AddLine(L("damage with pistols"), C_0_White);
						}
					}
					else if (is(player.fullClassId, PFC_ROGUE, PFC_DRUID) || HasTrait(CurrentPlayerIndex, TraitId::Paladin) || HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
						Tooltip_AddLine(L("accuracy of melee attacks"), C_0_White);
					}
					else {
						Tooltip_AddLine(L("accuracy of melee and ranged attacks"), C_0_White);
					}
				}
				Tooltip_AddLine(L("armor class"), C_0_White);
				if (is(player.fullClassId, PFC_WARRIOR, PFC_TEMPLAR, PFC_INQUISITOR, PFC_MONK, PFC_SHINOBI, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR)
					&& (!HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger))) {
					Tooltip_AddLine(L("melee hit block chance"), C_0_White);
					Tooltip_AddLine(L("arrow hit block chance"), C_0_White);
				}
				Tooltip_AddLine(L("ability to equip items"), C_0_White);
			}
			else if (CursorIntoDisplayObject(CharPanelBaseVitRect)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				Tooltip_AddLine(L("vitality affects:"), C_1_Blue);
				if (!(player.gameChanger & BIT(GC_27_TERMINATOR))) {
					Tooltip_AddLine(L("maximum hit points"), C_0_White);
					if (GameMode != GM_COLISEUM && GameMode != GM_CLASSIC) {
						Tooltip_AddLine(L("life regeneration speed"), C_0_White);
					}
				}
				if (GameMode != GM_CLASSIC) {
					Tooltip_AddLine(L("stun threshold"), C_0_White);
				}
				//Tooltip_AddLine(L("DFE limit"), C_0_White);
				if (!HasTrait(CurrentPlayerIndex, TraitId::Stoneform)) {
					if ((!HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) && GameMode != GM_CLASSIC) {
						Tooltip_AddLine(L("melee damage resistance"), C_0_White);
						Tooltip_AddLine(L("arrow damage resistance"), C_0_White);
					}
					if (GameMode != GM_CLASSIC){
						Tooltip_AddLine(L("fire damage resistance"), C_0_White);
					Tooltip_AddLine(L("lightning damage resistance"), C_0_White);
					Tooltip_AddLine(L("magic damage resistance"), C_0_White);
					Tooltip_AddLine(L("cold damage resistance"), C_0_White);
					Tooltip_AddLine(L("acid damage resistance"), C_0_White);
					}
				}
				if (GameMode != GM_CLASSIC) { Tooltip_AddLine(L("ability to equip items"), C_0_White); }
			}

			// --------------------------------------------------------- TOTAL KILLS (th4) -----------------------------------------
			// Opt-in via the "total_kills" config option (off by default). Single player only,
			// and not in Coliseum -- that's a PvP-only mode where the stat is meaningless.
			else if (total_kills && MaxCountOfPlayersInGame == 1 && GameMode != GM_COLISEUM
			      && CursorIntoDisplayObject(CharPanelNameRect)) {
				Tooltip_Clear();
				sprintf(InfoPanelBuffer, L("total monsters killed : %u"), player.totalKills);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}

			// --------------------------------------------------------- DAMAGE ---------------------------------------------------
			else if (CursorIntoDisplayObject(CharPanelDamageRect)) { 
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				Tooltip_AddLine(L("damage affects:"), C_1_Blue);
				Tooltip_AddLine(L("how much damage your attacks do to enemies,"), C_0_White); 
				int c = 128 + C_1_Blue;
				if (HasTrait(CurrentPlayerIndex, TraitId::Psion)) {					
					int psionic_dmg = (3 + player.CharLevel * player.CurMagic / 150) + PerkValue(PERK_PSYCHOKINESIS, CurrentPlayerIndex);
					sprintf(InfoPanelBuffer, L("Psionic damage: %c%i"), c, psionic_dmg);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					if (PerkValue(PERK_AMPLIFY_DAMAGE, CurrentPlayerIndex) > 0) {
						psionic_dmg += psionic_dmg * (PerkValue(PERK_AMPLIFY_DAMAGE, CurrentPlayerIndex)) / 100;
						sprintf(InfoPanelBuffer, L("Amplified: %c%i"), c, psionic_dmg);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}					
				}
				else {
					if (HasTrait(CurrentPlayerIndex, TraitId::HolyAura)) {
						int holyAuraDamage = 1 + player.CharLevel * player.CurMagic / 40;
						holyAuraDamage += PerkValue(PERK_RIGHTEOUS_ANGER, CurrentPlayerIndex);
						sprintf(InfoPanelBuffer, L("Holy aura damage: %c%i"), c, holyAuraDamage);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					if (MaxDmgFromChar != MinDmgFromChar) {
						int av_dmg = (MinDmgFromChar + MaxDmgFromChar) / 2;
						sprintf(InfoPanelBuffer, L("Average damage: %c%i"), c, av_dmg);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					// (th4) species damage lines: Purge / Exorcism / Lethality (mapping per the
					// perk tooltips: PURGE->undead, EXORCISM->demons, LETHALITY->beasts). Shown for
					// Warriors, Archers except Scout, and Ranger-trait holders; never in Coliseum;
					// only with a bow / crossbow (both IC_3_BOW) or trap equipped -- the same
					// "arrows / bolts / traps" scope the perks themselves state.
					// Per Mordor: if ANY of the three perks is taken, list ALL THREE species --
					// zero-perk species simply show the unmodified range. The same rule reshapes
					// the Thief's Shadow Strike section below.
					static const struct { int perk; const char* label; } speciesTip[3] = {
						{ PERK_PURGE,     "vs undead" },
						{ PERK_EXORCISM,  "vs demons" },
						{ PERK_LETHALITY, "vs beasts" },
					};
					bool anySpeciesPerk = PerkValue(PERK_PURGE,     CurrentPlayerIndex) > 0
					                   || PerkValue(PERK_EXORCISM,  CurrentPlayerIndex) > 0
					                   || PerkValue(PERK_LETHALITY, CurrentPlayerIndex) > 0;
					{
						Item& tipLeft  = player.OnBodySlots[IS_LeftHand];
						Item& tipRight = player.OnBodySlots[IS_RightHand];
						bool speciesWeapon = ( is(tipLeft.ItemCode,  IC_3_BOW, IC_19_TRAP) && tipLeft.IsReqMet )
						                  || ( is(tipRight.ItemCode, IC_3_BOW, IC_19_TRAP) && tipRight.IsReqMet );
						// Warrior qualifies ONLY as a Ranger (class + trait); Archers by class, minus Scout
						bool speciesClasses = (player.ClassID == PC_0_WARRIOR && HasTrait(CurrentPlayerIndex, TraitId::Ranger))
						                   || (player.ClassID == PC_1_ARCHER && player.fullClassId != PFC_SCOUT);
						if (speciesClasses && speciesWeapon && GameMode != GM_COLISEUM && anySpeciesPerk) {
							for (int t = 0; t < 3; t++) {
								int v = PerkValue((PERKS)speciesTip[t].perk, CurrentPlayerIndex);
								sprintf(InfoPanelBuffer, L("%s: %c%i - %i"), L(speciesTip[t].label), c,
									MinDmgFromChar * (100 + v) / 100, MaxDmgFromChar * (100 + v) / 100);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}
					}
					// (th4) THIEF: show her strike-from-the-dark numbers -- 500% base plus
					// Shadow Strike perk (value column carries the extra %).
					if (HasTrait(CurrentPlayerIndex, TraitId::Thief) && player.IsBowInHand) {
						int ssMult = 500 + PerkValue(PERK_SHADOW_STRIKE, CurrentPlayerIndex);
						Tooltip_AddLine(L("Shadow Strike:"), C_1_Blue);
						sprintf(InfoPanelBuffer, L("multiplier: %c%i%%"), c, ssMult);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						// (th4) with any species perk taken, the flat damage line is REPLACED by
						// the three per-species ranges (species bonus and multiplier together)
						if (anySpeciesPerk) {
							for (int t = 0; t < 3; t++) {
								int v = PerkValue((PERKS)speciesTip[t].perk, CurrentPlayerIndex);
								sprintf(InfoPanelBuffer, L("%s: %c%i - %i"), L(speciesTip[t].label), c,
									MinDmgFromChar * (100 + v) / 100 * ssMult / 100,
									MaxDmgFromChar * (100 + v) / 100 * ssMult / 100);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}else{
							sprintf(InfoPanelBuffer, L("damage: %c%i - %i"), c, MinDmgFromChar * ssMult / 100, MaxDmgFromChar * ssMult / 100);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
					}					
					if( Players[CurrentPlayerIndex].effectFlag[EA_MORE_DAMAGE_CHANCE] ){
						sprintf(InfoPanelBuffer, L("(RUIN) 30%% chance for +50%% damage: %c%i - %i%c"), c, MinDmgFromChar * 15 / 10, MaxDmgFromChar * 15 / 10, c);
						Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					}
					// ---------------------------------------------------------------- DISPLAY OF SPECIES MELEE DAMAGE -----------------------------------------------
					if ((is(player.fullClassId, PFC_WARRIOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_MONK, PFC_SHINOBI, PFC_SHUGOKI, PFC_KENSEI, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_GLADIATRIX,
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
						if (HasTrait(CurrentPlayerIndex, TraitId::Frenzied)) {
							mdvu_min += mdvu_min * 75 / 100; mdvu_max += mdvu_max * 75 / 100;
							mdvd_min += mdvd_min * 75 / 100; mdvd_max += mdvd_max * 75 / 100;
							mdvb_min += mdvb_min * 75 / 100; mdvb_max += mdvb_max * 75 / 100;
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
								sprintf(InfoPanelBuffer, L("to Undead: %c%i"), c, mdvu_min);
							}
							else {
								sprintf(InfoPanelBuffer, L("to Undead: %c%i%c - %c%i%c, average %c%i%c"), c, mdvu_min, c, c, mdvu_max, c, c, (mdvu_min + mdvu_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvu_min = 1;
							mdvu_max = mdvu_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, L("sweep damage to Undead: %c%i%c - %c%i%c, average %c%i%c"), c, mdvu_min, c, c, mdvu_max, c, c, (mdvu_min + mdvu_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						mdvd_max = mdvd_max < mdvd_min ? mdvd_min : mdvd_max;
						if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (mdvd_min == mdvd_max) {
								sprintf(InfoPanelBuffer, L("to Demons: %c%i"), c, mdvd_min);
							}
							else {
								sprintf(InfoPanelBuffer, L("to Demons: %c%i%c - %c%i%c, average %c%i%c"), c, mdvd_min, c, c, mdvd_max, c, c, (mdvd_min + mdvd_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvd_min = 1;
							mdvd_max = mdvd_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, L("sweep damage to Demons: %c%i%c - %c%i%c, average %c%i%c"), c, mdvd_min, c, c, mdvd_max, c, c, (mdvd_min + mdvd_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						mdvb_max = mdvb_max < mdvb_min ? mdvb_min : mdvb_max;
						if (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (mdvb_min == mdvb_max) { // (th4) was mdvd_* -- tested the DEMONS pair, so a real beast range could show as one number
								sprintf(InfoPanelBuffer, L("to Beasts: %c%i"), c, mdvb_min);
							}
							else {
								sprintf(InfoPanelBuffer, L("to Beasts: %c%i%c - %c%i%c, average %c%i%c"), c, mdvb_min, c, c, mdvb_max, c, c, (mdvb_min + mdvb_max) / 2, c);
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						if (player.fullClassId == PFC_MONK && player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
							mdvb_min = 1;
							mdvb_max = mdvb_max * (100 + PerkValue(PERK_CLEAVE, CurrentPlayerIndex)) / 200;
							sprintf(InfoPanelBuffer, L("sweep damage to Beasts: %c%i%c - %c%i%c, average %c%i%c"), c, mdvb_min, c, c, mdvb_max, c, c, (mdvb_min + mdvb_max) / 2, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						// (th4) SECUTOR and DRUID: Jackal Technique proc numbers. The perk (PerksData.cpp, PERK_JACKAL_TECHNIQUE)
						// has two value columns: 0 = proc chance %, 1 = extra melee damage %. The hit itself is
						// applied in Player.cpp (search "jackal"): damage += damage * col1 / 100 on RNG(100) < col0.
						// That runs AFTER the species/weapon modifiers this tooltip has already folded into the
						// mdv*_ values above, so the proc lines are simply those values scaled the same way.
						// It runs BEFORE boss reduction, Fatality and the Armored affix -- like the plain lines
						// above, these show damage against an ordinary monster.
						// Must stay AFTER the Monk sweep blocks: those overwrite mdv*_min/max with sweep values
						// (harmless here only because a Monk never has this perk).
						// The class list must match the proc in Player.cpp (Secutor || Druid) and the perk's class
						// mask (IPCM_SECUTOR | IPCM_DRUID in PerksData.cpp): a class that can take the perk must see it here.
						// The three species lines reuse the translated "to Undead/Demons/Beasts" keys, so only the
						// header key below is new to the localization tables.
						if ((player.fullClassId == PFC_SECUTOR || player.fullClassId == PFC_DRUID) && PerkPoints(PERK_JACKAL_TECHNIQUE, CurrentPlayerIndex) > 0) {
							int jackalChance = PerkValue(PERK_JACKAL_TECHNIQUE, CurrentPlayerIndex, 0);
							int jackalBonus  = PerkValue(PERK_JACKAL_TECHNIQUE, CurrentPlayerIndex, 1);
							sprintf(InfoPanelBuffer, L("Jackal Technique damage (%c%i%%%c chance):"), c, jackalChance, c);
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							static const char* jackalSpecies[3][2] = {
								{ "to Undead: %c%i", "to Undead: %c%i%c - %c%i%c, average %c%i%c" },
								{ "to Demons: %c%i", "to Demons: %c%i%c - %c%i%c, average %c%i%c" },
								{ "to Beasts: %c%i", "to Beasts: %c%i%c - %c%i%c, average %c%i%c" },
							};
							const int jMin[3] = { mdvu_min, mdvd_min, mdvb_min };
							const int jMax[3] = { mdvu_max, mdvd_max, mdvb_max };
							for (int t = 0; t < 3; t++) {
								int jlo = jMin[t] + jMin[t] * jackalBonus / 100;
								int jhi = jMax[t] + jMax[t] * jackalBonus / 100;
								if (jlo == jhi) {
									sprintf(InfoPanelBuffer, L(jackalSpecies[t][0]), c, jlo);
								}
								else {
									sprintf(InfoPanelBuffer, L(jackalSpecies[t][1]), c, jlo, c, c, jhi, c, c, (jlo + jhi) / 2, c);
								}
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}
						if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
							if (PerkValue(PERK_PIERCING_BULLETS, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, L("bullet pierce chance: %c%i%%%c"), c, PerkValue(PERK_PIERCING_BULLETS, CurrentPlayerIndex), c);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
							if (PerkValue(PERK_STOPPING_POWER, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, L("monster stun threshold overpower: %c%i%%%c"), c, PerkValue(PERK_STOPPING_POWER, CurrentPlayerIndex), c);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}
						// by (th4): Blessed Hammer per-hit damage, shown LAST (Templar only, perk level >= 1)
						if (player.fullClassId == PFC_TEMPLAR && PerkPoints(PERK_BLESSED_HAMMER, CurrentPlayerIndex) >= 1) {
							sprintf(InfoPanelBuffer, L("Blessed Hammer damage: %c%i"), c, PerkValue(PERK_BLESSED_HAMMER, CurrentPlayerIndex));
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
					}
					// -----------------weapon elemental damage shown------------------------
					else if (is(player.fullClassId, PFC_INQUISITOR, PFC_SCOUT)) {

						// Check if Inquisitor has ranged weapon (bow or crossbow)
						bool inquisitorHasRanged = false;
						Item& leftHand = player.OnBodySlots[IS_LeftHand];
						Item& rightHand = player.OnBodySlots[IS_RightHand]; 
						if (player.fullClassId == PFC_INQUISITOR) {							
							// Check for bow OR crossbow (IC_3_BOW, or add IC_CROSSBOW if exists)
							if (leftHand.ItemCode == IC_3_BOW || rightHand.ItemCode == IC_3_BOW) {
								inquisitorHasRanged = true;
							}
						}

						for (int elem = ET_1_FIRE; elem <= ET_6_COLD; elem++) {
							int idx = elem - 1;
							int tempMin, tempMax;
							int flatMin, flatMax;

							// Choose correct source based on class and weapon type
							if (player.fullClassId == PFC_INQUISITOR) {
								if (inquisitorHasRanged) {
									// Inquisitor with ranged weapon 
									tempMin = player.effectActionValue[EA_ARROW_FIRE + idx].minVal;
									tempMax = player.effectActionValue[EA_ARROW_FIRE + idx].maxVal;
								}
								else {
									// Inquisitor with melee
									tempMin = player.effectActionValue[EA_MELEE_FIRE + idx].minVal;
									tempMax = player.effectActionValue[EA_MELEE_FIRE + idx].maxVal;
								}
							}
							else { // Scout always uses ranged
								tempMin = player.effectActionValue[EA_ARROW_FIRE + idx].minVal;
								tempMax = player.effectActionValue[EA_ARROW_FIRE + idx].maxVal;
							}

							// Apply percent to temp FIRST (exactly like PvM_Ranged)
							tempMin += tempMin * player.elementPercent[elem] / 100;
							tempMax += tempMax * player.elementPercent[elem] / 100;

							// THEN add flat elemental damage (exactly like PvM_Ranged adds addDamage)
							flatMin = tempMin + player.elementMinDamage[elem];
							flatMax = tempMax + player.elementMaxDamage[elem];

							if (flatMin == 0 && flatMax == 0) continue;
							if (player.fullClassId == PFC_SCOUT && leftHand.ItemCode != IC_3_BOW) continue;

							// Apply ModifyElementalWeaponDamage formula
							double classCoef = 1.0;

							if (player.fullClassId == PFC_INQUISITOR) {
								switch (elem) {
								case ET_5_HOLY:      classCoef = 0.5; break;
								case ET_1_FIRE:      classCoef = 1.0 + PerkValue(PERK_EXTRA_CRISPY, playerIndex) / 100.0; break;
								case ET_2_LIGHTNING: classCoef = 1.0 + PerkValue(PERK_FULLY_CHARGED, playerIndex) / 100.0; break;
								case ET_3_ARCAN:     classCoef = 1.0 + PerkValue(PERK_CHAOS_BORN, playerIndex) / 100.0; break;
								case ET_6_COLD:      classCoef = 1.0 + PerkValue(PERK_DEEP_FREEZE, playerIndex) / 100.0; break;
								case ET_4_ACID:      classCoef = 1.0 + PerkValue(PERK_TOXICITY, playerIndex) / 100.0; break;
								}
								if (HasTrait(playerIndex, TraitId::Tormentor)) classCoef += 0.2;
							}
							else if (player.fullClassId == PFC_SCOUT) {
								switch (elem) {
								case ET_1_FIRE:      classCoef = 1.0 + PerkValue(PERK_EXTRA_CRISPY, playerIndex) / 100.0; break;
								case ET_2_LIGHTNING: classCoef = 1.0 + PerkValue(PERK_FULLY_CHARGED, playerIndex) / 100.0; break;
								case ET_5_HOLY:      classCoef = 1.0 + PerkValue(PERK_BLESSED_ARROWS, playerIndex) / 100.0; break;
								case ET_4_ACID:      classCoef = 1.0 + PerkValue(PERK_TOXICITY, playerIndex) / 100.0; break;
								case ET_3_ARCAN:     classCoef = 1.0 + PerkValue(PERK_CHAOS_BORN, playerIndex) / 100.0; break;
								case ET_6_COLD:      classCoef = 1.0 + PerkValue(PERK_DEEP_FREEZE, playerIndex) / 100.0; break;
								}
							}

							double baseDamageCoef = 0.06 * player.BaseDamage;

							// Apply Fury bonus
							if (player.activeBuffFlag & BF_2_FURY_ACTIVE) {
								classCoef += 0.2;
							}

							int finalMin = (int)((baseDamageCoef * flatMin + 5 + player.CharLevel / 2.0) * classCoef);
							int finalMax = (int)((baseDamageCoef * flatMax + 5 + player.CharLevel / 2.0) * classCoef);

							// Double damage for Scout with crossbow (AnimationDelay)
							if (player.fullClassId == PFC_SCOUT) {
								Item& leftHand = player.OnBodySlots[IS_LeftHand];
								Item& rightHand = player.OnBodySlots[IS_RightHand];
								if ((leftHand.ItemCode != IC_M1_NONE && BaseItems[leftHand.baseItemIndex].AnimationDelay) ||
									(rightHand.ItemCode != IC_M1_NONE && BaseItems[rightHand.baseItemIndex].AnimationDelay)) {
									finalMin *= 2;
									finalMax *= 2;
								}
							}

							if (finalMin < 1) finalMin = 1;
							if (finalMax < 1) finalMax = 1;

							int avgDamage = (finalMin + finalMax) / 2;

							// Build the string with average damage in blue brackets
							if (player.fullClassId == PFC_INQUISITOR) {
								if (inquisitorHasRanged) {
									if (finalMin == finalMax) {
										sprintf(InfoPanelBuffer, L("%s (ranged): %d"), L(Elements[elem]), finalMin);
									}
									else {
										sprintf(InfoPanelBuffer, L("%s (ranged): %d-%d [avg %c%d%c]"), L(Elements[elem]), finalMin, finalMax, 128 + C_1_Blue, avgDamage, 128 + C_1_Blue);
									}
								}
								else {
									if (finalMin == finalMax) {
										sprintf(InfoPanelBuffer, L("%s (melee): %d"), L(Elements[elem]), finalMin);
									}
									else {
										sprintf(InfoPanelBuffer, L("%s (melee): %d-%d [avg %c%d%c]"), L(Elements[elem]), finalMin, finalMax, 128 + C_1_Blue, avgDamage, 128 + C_1_Blue);
									}
								}
							}
							else { // Scout
								if (finalMin == finalMax) {
									sprintf(InfoPanelBuffer, L("%s arrow: %d"), L(Elements[elem]), finalMin);
								}
								else {
									sprintf(InfoPanelBuffer, L("%s arrow: %d-%d [avg %c%d%c]"), L(Elements[elem]), finalMin, finalMax, 128 + C_1_Blue, avgDamage, 128 + C_1_Blue);
								}
							}
							Tooltip_AddLine(InfoPanelBuffer, C_0_White);
						}
						// devastator auras -----------------
						if (HasTrait(CurrentPlayerIndex, TraitId::Devastator)) {
							if (PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex)
								+ PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex)
								+ PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex)
								+ PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) > 0) {
								int rad = 3 + (PerkValue(PERK_AURA_RADIUS, CurrentPlayerIndex) / 10) + (PerkValue(SYNERGY_BASTION_OF_ELEMENTS, CurrentPlayerIndex) / 10);
								Tooltip_AddLine(L("Auras:"), C_0_White);
								sprintf(InfoPanelBuffer, L("Elemental aura radius: %c%i"), c, rad);
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
								sprintf(InfoPanelBuffer, L("Fire aura damage: %c%i"), c, damageFire);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
							if (PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, L("Lightning aura damage: %c%i"), c, damageLightning);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
							if (PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, L("Magic aura damage: %c%i"), c, damageArcane);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
							if (PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex) > 0) {
								sprintf(InfoPanelBuffer, L("Toxic aura damage: %c%i"), c, damageAcid);
								Tooltip_AddLine(InfoPanelBuffer, C_0_White);
							}
						}
						// end of devastator auras- ------------------------------------
					}
					// ----------- end of elem damage -------------------------

				}
				// (th4) THORNS -- one summed line, outside the Psion/else split above so it
				// shows whatever else the character has.
				{
					int thMin = 0, thMax = 0;
					if( CalcThornsDamage_Display( CurrentPlayerIndex, thMin, thMax ) ){
						sprintf( InfoPanelBuffer, L("Thorns damage: %c%i - %i"), c, thMin, thMax );
						Tooltip_AddLine( InfoPanelBuffer, C_0_White );
					}
				}
			}
			// -------- END OF DAMAGE -------------
			
			// -------- LIFE REGEN -------------
			else if (CursorIntoDisplayObject(CharPanelLifeRegen) /*&& GameMode != GM_COLISEUM*/) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				//Tooltip_AddLine(L("life regeneration determines:"), C_1_Blue); 
				int c = 128 + C_1_Blue; 
				double lps = 20.00f * (double)HealthRegen / 64.00f;
				int srt = 100 + PerkValue(PERK_IRIDIUM_POWER_CELL, playerIndex, 0);
				sprintf(InfoPanelBuffer, L("hit points restored per second: %c%.1f"), c, lps ); 
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);	
				if (player.gameChanger & BIT(GC_27_TERMINATOR)) {
					sprintf(InfoPanelBuffer, L("critical self-repair threshold: %c%d"), c, srt);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
				EffectAction& ea = player.effectActionValue[EA_LIFE_STEAL];
				if (ea.maxVal > 0) {
					sprintf(InfoPanelBuffer, ea.minVal == ea.maxVal ? L("life stolen per melee hit: +%i") : L("life stolen per melee hit: +(%i-%i)"), ea.minVal, ea.maxVal);
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
				sprintf(InfoPanelBuffer, L("mana points restored per second: %c%.1f"), c, mps);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				EffectAction& ea = player.effectActionValue[EA_MANA_STEAL];
				if (ea.maxVal > 0) {
					sprintf(InfoPanelBuffer, ea.minVal == ea.maxVal ? L("mana stolen per melee hit: +%i") : L("mana stolen per melee hit: +(%i-%i)"), ea.minVal, ea.maxVal);
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
				sprintf(InfoPanelBuffer, L("average crit damage: %c%.1f"), c, acd);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				// (th4) THORNS crit. Thorns DOES crit: ActCriticalHit is called on the thorns
				// damage too (Player.cpp ~5310) and is gated only on class, never on isThorns.
				// It always takes the MELEE crit row -- PvM_Melee passes baseMissileIndex -1 for
				// thorns regardless of what is in hand -- so this uses CR_MELEE and NOT the
				// weapon-derived critType computed above for the main line.
				// The PFC_TRAPPER guard the main line carries is omitted on purpose: this line
				// is Iron Maiden only, so the branch could never be taken.
				{
					int thMin = 0, thMax = 0;
					if( CalcThornsDamage_Display( CurrentPlayerIndex, thMin, thMax ) ){
						BaseCrit bcT   = GetBaseCrits( CR_MELEE, CurrentPlayerIndex );
						int tPercent   = bcT.percent   + player.critPercent;
						int tFlatMin   = bcT.damageMin + player.critDamageMin;
						int tFlatMax   = bcT.damageMax + player.critDamageMax;
						int tMinCrit   = thMin + thMin * tPercent / 100 + tFlatMin;
						int tMaxCrit   = thMax + thMax * tPercent / 100 + tFlatMax;
						sprintf( InfoPanelBuffer, L("Thorns crit damage: %c%i - %i"), c, tMinCrit, tMaxCrit );
						Tooltip_AddLine( InfoPanelBuffer, C_0_White );
					}
				}
			}
			// -------- END OF CRIT DMG -------------
			
			// -------- CRIT CHANCE -------------
			
			else if (CursorIntoDisplayObject(CharPanelCritChance)) {
				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
					BaseCrit bc = GetBaseCrits(CR_MAGIC, CurrentPlayerIndex);
					int cc = bc.chance;
					int c = 128 + C_1_Blue;
					sprintf(InfoPanelBuffer, L("spell crit chance: %c%i%%"), c, cc);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
				else if (player.fullClassId == PFC_INQUISITOR) {
					BaseCrit bc = GetBaseCrits(CR_ELEMENTAL, CurrentPlayerIndex);
					int cc = bc.chance;
					int c = 128 + C_1_Blue;
					sprintf(InfoPanelBuffer, L("weapon elemental damage crit chance: %c%i%%"), c, cc);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}
			}
			
			// -------- END OF CRIT CHANCE ------

			else if (CursorIntoDisplayObject(CharPanelArmorClass)) { // -------- ARMOR CLASS -----------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
					// ---- main description ----
				Tooltip_AddLine(L("armor class affects:"), C_1_Blue);
				Tooltip_AddLine(L("percentage of enemy attacks deflected,"), C_0_White);
					// -- stun --
				int minST, maxST; 
				tie(minST, maxST) = GetPlayerStunThreshold(player);
				LimitToMin(minST, 0); 
				LimitToMin(maxST, 0);
				int stun_avoid_chance = PerkValue(PERK_RESILIENCE, CurrentPlayerIndex) + PerkValue(PERK_FOOTWORK, CurrentPlayerIndex) + PerkValue(SYNERGY_PERSEVERANCE, CurrentPlayerIndex);
				int c = 128 + C_1_Blue ;
				if (minST == maxST ){ sprintf(InfoPanelBuffer, L("Stun Threshold: %c%i"), c, minST); }
				else { sprintf(InfoPanelBuffer, L("Stun Threshold: %c%i - %i%c"), c, minST, maxST, c); }
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				if (stun_avoid_chance > 0) {
					sprintf(InfoPanelBuffer, L("Stun avoid chance: %c%i%%"), c, stun_avoid_chance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- spell deflect -------
				int playerAC = 0;
				Player_AC(CurrentPlayerIndex, playerAC);
				int spell_deflect_chance = 95 - playerAC / 50 - PerkValue(PERK_SPELL_DEFLECTION, CurrentPlayerIndex);
				Limit_MvP_Ranged_Magical_ToHit_Chance(CurrentPlayerIndex, spell_deflect_chance); // is this line even needed ?
				spell_deflect_chance = 100 - spell_deflect_chance;
				sprintf(InfoPanelBuffer, L("Spell deflect chance: %c%i%%"), c, spell_deflect_chance);
				
				if (GameMode != GM_CLASSIC) { Tooltip_AddLine(InfoPanelBuffer, C_0_White); }

					// ------- IDR -----------
				if (GameMode != GM_CLASSIC && GameMode != GM_COLISEUM) {
					int damage = 100;
					damage = AddDamageReduction(CurrentPlayerIndex, damage);
					int damage_spell = 100 - damage;
					int damage_arr = 100 - 3 * damage / 2;
					sprintf(InfoPanelBuffer, L("IDR: Spell %c%i%%%c, Arrow: %c%i%%%c"), c, damage_spell, c, c, damage_arr, c);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- paladin's mana shield efficiency -------

				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
					int drMS = 100 - PerkValue(PERK_FORTIFIED_SHIELD, CurrentPlayerIndex);
					sprintf(InfoPanelBuffer, L("Mana Shield damage modifier: %c%i%%"), c, drMS);
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
					sprintf(InfoPanelBuffer, L("Max block chance vs melee: %c%i%%"), c, maxBlockChance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);

					// ------- block rate (melee) -------
					int playerBlockChance = 0;
					//MvP_Melee_Block_Chance(0, playerIndex, playerBlockChance);// mor: not sure monsterIndex is needed here (could even cause issues)				
					playerBlockChance = player.blockChance	// (th4) class block bonus retired
						+ ((player.BaseDexterity + player.BaseStrength/* - monster.ArmorClass*/) / 2)
						- a_rose_with_thorns_trait_penalty
						+ two_towers_trait_benefit_1
						- PerkValue(SYNERGY_AGGRESSION, CurrentPlayerIndex, 0);
					LimitToMin(playerBlockChance, 0); 
					//playerBlockChance += PerkValue(PERK_TUCK_TAILS, playerIndex)/* + player.blockChance*/;
					sprintf(InfoPanelBuffer, L("Block rating vs melee hits: %c%i"), c, playerBlockChance);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- block rate (ranged) -------
				int a_rose_with_thorns_trait_penalty_ranged = HasTrait(CurrentPlayerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
				int blockChanceRanged = player.BaseDexterity	// (th4) class block bonus retired
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
					|| HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Scissor)) {
					finalMaxBlockRate = 0;
				}
				if (finalMaxBlockRate > 0) {
					sprintf(InfoPanelBuffer, L("Max block chance vs arrows: %c%i%%"), c, finalMaxBlockRate);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					sprintf(InfoPanelBuffer, L("Block rating vs arrows: %c%i"), c, blockChanceRanged);
					Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				}

					// ------- autohit (melee) -------
				int autoHit = 20;
				int automiss = 100;
				MvP_Melee_Autohit_chance(CurrentPlayerIndex, autoHit, automiss);
				sprintf(InfoPanelBuffer, L("Min chance to get hit (melee): %c%i%%"), c, autoHit);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				sprintf(InfoPanelBuffer, L("Max chance to get hit (melee): %c%i%%"), c, automiss);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
					// ------- autohit min-max (arrows) -------
				int Autohit = 14;
				int Automiss = 95;
				int realToHitChance = 0;
				Limit_MvP_Ranged_Arrow_ToHit_Chance(CurrentPlayerIndex, realToHitChance, Autohit, Automiss);
				sprintf(InfoPanelBuffer, L("Min chance to get hit (arrows): %c%i%%"), c, Autohit);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
				sprintf(InfoPanelBuffer, L("Max chance to get hit (arrows): %c%i%%"), c, Automiss);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}
			// --- END OF ARMOR CLASS ---

			 // -------- TO HIT -----------------
			else if (CursorIntoDisplayObject(charPanelToHit)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				// ---- main description ----
				Tooltip_AddLine(L("to hit chance affects:"), C_1_Blue);
				Tooltip_AddLine(L("accuracy of your attacks against enemies,"), C_0_White);
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
				sprintf(InfoPanelBuffer, L("Spell accuracy rating: %c%i"), c, hitChance);
				Tooltip_AddLine(InfoPanelBuffer, C_0_White);
			}
			// -------- END OF TO HIT -----------

			else if (CursorIntoDisplayObject(CharPanelResistAcid)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_4_ACID]);
				sprintf(InfoPanelBuffer, L("%c%.2f%%%c acid damage resisted"), c, player.resistPercent[ET_4_ACID] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistMagic)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_3_ARCAN]);
				sprintf(InfoPanelBuffer, L("%c%.2f%%%c magic damage resisted"), c, player.resistPercent[ET_3_ARCAN] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistFire)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_1_FIRE]);
				sprintf(InfoPanelBuffer, L("%c%.2f%%%c fire damage resisted"), c, player.resistPercent[ET_1_FIRE] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistCold)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_6_COLD]);
				sprintf(InfoPanelBuffer, L("%c%.2f%%%c cold damage resisted"), c, player.resistPercent[ET_6_COLD] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistLightning)) {
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c = 128 + ResistColor(player.resistPercent[ET_2_LIGHTNING]);
				sprintf(InfoPanelBuffer, L("%c%.2f%%%c lightning damage resisted"), c, player.resistPercent[ET_2_LIGHTNING] * 100, c );
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistArrow)) { //  ------------------ ARROW DAMAGE RESIST CELL --------------------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c1 = 128 + (player.resistPercentArrowMin > 0 ? (player.resistPercentArrowMin < player.resistPercentArrowMinLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				int c2 = 128 + (player.resistPercentArrowMax > 0 ? (player.resistPercentArrowMax < player.resistPercentArrowMaxLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				if (player.resistPercentArrowMin == player.resistPercentArrowMax) {
					sprintf(InfoPanelBuffer, L("%c%.2f%%%c arrow damage resisted"), c1, player.resistPercentArrowMin * 100, c1);
				}
				else {
					sprintf(InfoPanelBuffer, L("%c%.2f%%%c to %c%.2f%%%c arrow damage resisted, (avg %c%.2f%%%c)"), c1, player.resistPercentArrowMin * 100, c1, c2, player.resistPercentArrowMax * 100, c2, 129, (player.resistPercentArrowMin * 100 + player.resistPercentArrowMax * 100) / 2, 129);
				}
				Tooltip_AddLine(InfoPanelBuffer);
			}
			else if (CursorIntoDisplayObject(CharPanelResistMelee)) { //  ------------------ MELEE DAMAGE RESIST CELL --------------------------
				Tooltip_Clear();
				Tooltip_SetOffsetRight();
				int c1 = 128 + (player.resistPercentMeleeMin > 0 ? (player.resistPercentMeleeMin < player.resistPercentMeleeMinLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				int c2 = 128 + (player.resistPercentMeleeMax > 0 ? (player.resistPercentMeleeMax < player.resistPercentMeleeMaxLimit ? C_1_Blue : C_3_Gold) : C_0_White);
				if (player.resistPercentMeleeMin == player.resistPercentMeleeMax) {
					sprintf(InfoPanelBuffer, L("%c%.2f%%%c melee damage resisted"), c1, player.resistPercentMeleeMin * 100, c1);
				}
				else {
					sprintf(InfoPanelBuffer, L("%c%.2f%%%c to %c%.2f%%%c melee damage resisted, (avg %c%.2f%%%c)"), c1, player.resistPercentMeleeMin * 100, c1, c2, player.resistPercentMeleeMax * 100, c2, 129, (player.resistPercentMeleeMin * 100 + player.resistPercentMeleeMax * 100)/2, 129);
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
					// (rt) add display of the inscription on the gravestone
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
		if( monster.newBossId ){ // (rt) (3) boss number extension
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
			// (th4): THREE bugs fixed here, together they were the "replaced player's light"
			// mystery the comment above describes, and the lights-off-after-load bug:
			// 1) LightIndex 0 means "boss has no light" (Ragehorn/Guardian/Hazeweaver, see
			//    CreateBoss), but 0 is a real LightList slot and ChangeLightPos only rejects
			//    -1 -- so the unguarded calls below hijacked LightList[0], typically the
			//    player's torch. Same guard as the book/pedestal handlers in Object.cpp.
			// 2) The boss check and the guarded call used Monsters[i] (RAW loop counter)
			//    while `monster` resolves through MonsterIndexes[i]. The two diverge as soon
			//    as any monster has died (DelMonster swap-removes), so the wrong monster's
			//    identity was tested and the wrong monster's light was moved.
			// 3) The pedestal scan walked raw Objects[i] slots; live objects are reached
			//    through ObjectActive[] (as the save/load loops do).
			//if (!monster.LightIndex) continue; // (th4) ORIGINAL: 0 meant "no boss light" — kept as reference
			// (th4) Rev 2.46: the full-deactivation bosses now carry the -1
			// no-light sentinel (CreateBoss), so the old truthiness guard would
			// let them through and re-run the position logic for nothing (the
			// ChangeLightPos(-1, ...) calls are no-ops, but the skip semantics
			// should be preserved).
			if (monster.LightIndex <= 0) continue;
			bool pedestalActivated = false;
			if (IsQuestOnLevel(Q_9_VALOR)) {
				for (int objIdx = 0; objIdx < ObjectsCount; objIdx++)
				{
					if (Objects[ObjectActive[objIdx]].BaseObjectIndex == BO_73_PEDESTAL_OF_BLOOD) {
						pedestalActivated = Objects[ObjectActive[objIdx]].maybeFrameIndex == 3;
					}
				}
			}
			int bossie = GameMode == GM_CLASSIC ? UM_C_902_RAGEHORN : UM_677_Guardian_of_Valor;
			if (monster.newBossId - 1 == bossie) {
				if (pedestalActivated) {
					ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
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
	return GameModeType() == GAME_MODE_TYPE::NIGHTMARE || GameModeType() == GAME_MODE_TYPE::IRON || GameMode == GM_OUTBREAK;
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

		if( !worldSleep ){ // (rt) not always in SinglePlayer, maybe waits for the messages to finish
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
					ShiftReleasedEvent = true; // (rt) ShiftPressedEvent = false; // removed because on process // for some reason it had been removed, restoring for now
				}
				else if (!shiftPressed && isShiftPressed /*&& MouseButtonPressed == MBP_1_LEFT*/) {
					ShiftPressedEvent = true; // (rt) ShiftReleasedEvent = false; // removed so the one-shot event gets processed; for some reason it had been removed, and sometimes there was movement instead of a shift-attack
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
				// (rt) looking into it
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
			// (rt) screen scrolling drawn at up to 240 fps (extra smoothness without changing the 20 fps gameplay speed)
			// (rt) offset for the active mana shield / reflect icons, or for all spells in general
			// Missile.WholeDeltaX, Missile.DeltaX, MissileAction, ManaShieldAction
			i64 tick = GetTickCountTh();
			i64 dt = MainFrameTimer - tick; // (rt) MainFrameTime - time of the next frame (50ms)
			int frameToDraw = int( ((double)(MAX_FPS / ENGINE_FPS) - dt) * FramePart );
			if( InterpolationFrame < frameToDraw && frameToDraw < FramesMax ){
				InterpolationFrame = frameToDraw;
				if( !IsGameMenuActive() && !NetCursorItemId ){ DispatchCursorAboveIsoland(); }
				MainDrawFunc(); ++AllDrawCount;
			}else{
				//(rt) if( ! TalkPanelMode ) // chat text is typed slowly, but this barely helped
				if( LowCpuUsage || MaxFPS < 40 ){
					Sleep(1); // (rt) 15 ms in TH1 // CPU offload
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
	DynZoom_Reset();	// (th4) DYNAMIC ZOOM: every game starts unzoomed (not saved)
	InitItemsCELArrays();
	LoadSquareCEL();
	StopMusic( true );	// (th4) fades out under the loading screen; the next level's music fades in (SoundBuffer.cpp MUSIC_FADE_MS; not in Classic)
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
// (th4) 2026-09-10: one full-memory dump per process. The VEH first-chance
// handler below and WinMain's __except can both see the same crash; without
// this guard the (multi-second) dump would be written twice per crash.
static volatile LONG g_Th4DumpWritten = 0;

void CreateMiniDump( EXCEPTION_POINTERS* pep )
{
	if( InterlockedCompareExchange( &g_Th4DumpWritten, 1, 0 ) != 0 ) return;
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

// (th4) 2026-09-10 diagnostics (d3d9 callback-crash investigation): capture
// crashes that kill the process at a user-callback boundary. An AV raised
// inside a window procedure or a storm callback is converted by the OS to
// STATUS_FATAL_USER_CALLBACK_EXCEPTION and terminates the process BEFORE
// WinMain's __except can run — that class left no Th4CrashDump.dmp and no
// error dialog ("closes to desktop"). A vectored handler sees the ORIGINAL
// first-chance exception, so the dump + log now cover it.
// Writes: "th4_d3d_diag.log" (always) + "Th4CrashDump.dmp" via CreateMiniDump
// (once per process; the __except handler shares that one-shot guard).
// (th4) Compiled out of release builds -- see TH4_CRASH_CAPTURE_ENABLED in funcs.h.
// Left as source rather than deleted: this is the tool that found the MP "New Game"
// reentrancy crash, and the next one will want it.
#ifdef TH4_CRASH_CAPTURE_ENABLED
static volatile LONG g_Th4VehSeen = 0;
static LONG CALLBACK Th4VehHandler( EXCEPTION_POINTERS* eps )
{
	if( eps && eps->ExceptionRecord ){
		const DWORD code = eps->ExceptionRecord->ExceptionCode;
		// AV / fatal user-callback / fail-fast — the codes that end the process.
		if( code == 0xC0000005 || code == 0xC000041D || code == 0xC0000409 ){
			const bool first = InterlockedCompareExchange( &g_Th4VehSeen, 1, 0 ) == 0;
			char where[128];
			D3dDiagAddr( where, sizeof(where), eps->ExceptionRecord->ExceptionAddress );
			char line[256];
			// (th4) 2026-09-10: _snprintf_s + _TRUNCATE (same rule as D3dDiagLog) —
			// sprintf_s would raise the CRT invalid-parameter handler on overflow,
			// i.e. kill the process from inside its own crash handler.
			_snprintf_s( line, sizeof(line), _TRUNCATE,
				"VEH first-chance: code=0x%08X at %s (tid %lu) - %s",
				(unsigned)code, where, GetCurrentThreadId(),
				first ? "writing Th4CrashDump.dmp" : "dump already written" );
			D3dDiagLogRaw( line );
			if( first ) CreateMiniDump( eps );
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

static void InstallTh4VehCrashHandler()
{
	// 1 = call before any other vectored handler.
	AddVectoredExceptionHandler( 1, Th4VehHandler );
}
#endif // TH4_CRASH_CAPTURE_ENABLED

//----- (00409283) -------------------------------------------------------- main
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	#ifdef TH4_CRASH_CAPTURE_ENABLED
	InstallTh4VehCrashHandler(); // (th4) diagnostics: must be armed before __try
	#endif
	__try{
	#ifdef _DEBUG
	// Phase 1 debug harness: "TH4.exe -lightlutcheck" runs the A8L8 light-falloff
	// LUT self-check over all graphType x OriginalLightLevels combos and exits
	// before any game initialization (report: A8L8LightBright_selfcheck.txt).
	if( strstr( lpCmdLine, "-lightlutcheck" ) ){
		RunA8L8LightBrightLutSelfCheck();
		return 0;
	}
	#endif
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

	// (rt) Exceptions are not caught in th2 for now
	#ifdef EXCEPTION
	OldTopLevelExceptionFilter = (int(__stdcall*)(struct _EXCEPTION_POINTERS*))SetUnhandledExceptionFilter((LONG(__stdcall*)(struct _EXCEPTION_POINTERS*))TopLevelExceptionFilter);
	#endif
	// (rt) Checks for an already running instance
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
		case DUN_31_ANDARIELS_THRONE: player.fullClassId == PFC_EXECUTIONER ? lifeAdd -= player.CurVitality / 2 : lifeAdd -= player.CurVitality / 4; break; // (rt) ternary form; correctness not verified
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
	// terminator: no regen
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
		lifeAdd = 0;
		if (Dungeon == DUN_0_TOWN && player.Xp == 0) {
			lifeAdd = 250;
		}
		else if (player.CurLife < ((100 + PerkValue(PERK_IRIDIUM_POWER_CELL, playerIndex, 0)) << 6)) {
			lifeAdd = 2 + PerkValue(PERK_RECONSTRUCTION, playerIndex);
		}
		else { lifeAdd = 0 + PerkValue(SYNERGY_ALLOY_MEMORY, playerIndex); }
		if (!IsSomeMonstersActivated()) {
			lifeAdd += lifeAdd * PerkValue(SYNERGY_COMBAT_LOGS, playerIndex) / 100;
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
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
		manaAdd = 1;
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
	//(rt) strcpy(ApplicationName,"d:\\projects\\TheHell\\TheHell2\\th.exe");
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
	if (IsPowerupPanelVisible						   ){ Powerups_Close();            CloseInventoryPanel();     result = 1; }
	if( IsQuenchPanelVisible                           ){ Quench_Close();              CloseInventoryPanel();     result = 1; }
	if( IsTransmutePanelVisible                        ){ Transmute_Close();           CloseInventoryPanel();     result = 1; }
    if( IsPerksPanelVisible                            ){ ClosePerksPanel();                                      result = 1; }
	if( IsInfoWindowVisible                            ){ InfoWindow_Close();                                     result = 1; }
	if( IsChatHistoryVisible                           ){ IsChatHistoryVisible = false;                           result = 1; }
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
// (rt) main window proc, used during gameplay, called as Handler1 in CallHandler1IfExists
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
				else if (wParam == VK_CAPITAL && DevelopMode) { // (rt) CapsLock turns the interface off
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
				else if (wParam == VK_CAPITAL && DevelopMode) { // (rt) CapsLock turns the interface off
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
						break; // (rt) removes the Idle Sys Menu Loop on Alt (Alt+Space enters the menu as usual)
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
					case VK_90_Z_KEY: // (rt) Alt + Z, toggle zoom
						if( !ForceZoom ) IsZoomDisable = !IsZoomDisable;
						break;
					//case VK_82_R_KEY: // Alt + Shift + R, Resetting the image output window //mor: under cheats, this gives an error and doesn't compile!
					//	if( IsShiftPressed ){ SwitchFullscreen(); SwitchFullscreen(); }
					//	break;
					case VK_77_M_KEY: // Alt + M, toggle multiplayer chat history overlay
						ChatHistory_Toggle();
						break;
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
			isGameObject = Mouse_LButtonDown(wParam); // (rt) the shift state is passed in wParam 
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
			isGameObject = Mouse_LButtonDown(wParam); // (rt) the shift state is passed in wParam 
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
		if( CurrentMenuWindowPtr ){ // (rt) right button in the in-game menu
			PlayGlobalSound( S_75_I_TITLEMOV );
			if( CurrentMenuWindowPtr == OptionsMenu ){
				SelectMainMenu(); // (rt) exit to the main menu
			}else{
				Menu_SetupMenuWindow( 0, 0 ); // (rt) close the menu
			}
			return 0;
		}
		if( CurrentDialogIndex ){ // (rt) right button goes back in a dialog (like Esc)
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
				isGameObject = Mouse_LButtonDown( wParam ); // (rt) the shift state is passed in wParam 
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
				while( DialogPageDown() ); // (rt) down the list
			}else{
				while( DialogPageUp() ); // (rt) up the list
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
	if( IsPlayerDead ){ // (rt) Some action with the message panel
		Check_MenuORSendMessage();
		return 0;
	}
	if( GamePaused == 2 ){ // (rt) If paused, the left mouse button isn't processed further
		return 0;
	}
	if( IsPopupScreenOpen ){
		ClosePopupScreen();
		return 0;
	}
	if( SelectCurSpellMode ){ // (rt) If a spell is being selected, try to select it. (even if the click isn't on an icon, any click closes the spell selection)
		SelectCurrentSpell();
		return 0;
	}
	if( CurrentDialogIndex ){ // (rt) If talking to NPCs, check where in their dialog the click landed
		if( CheckDialogMouseClick() ) return 0;
	}
	if( IsChatHistoryVisible && CursorIntoDisplayObject(ChatHistoryRect) ){
		return 0;
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
		if( QuestPanel_CheckArrowClick() ) return 0;	// (th4) scroll arrows, outside the text area
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
	if (IsPowerupPanelVisible && CursorIntoDisplayObject(PowerupPanel)) {
		Powerups_LButtonDown();
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
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){ //  (rt) if an item is attached to the cursor
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
		int isoLand = IsoLand_CheckLButtonClick((wParam & MK_SHIFT) /*wParam == 5*/ /* (rt) Shift pressed */, 0); // (rt) click on the game field ( movement, attack, shot )
		if (ExtraMouseButtons) {
			isoLand = IsoLand_CheckLButtonClick((wParam & MK_SHIFT) || (wParam & MK_XBUTTON2), 0);
		}
		if(IsPowerupPanelVisible || IsStashPanelVisible || IsVisualTradePanelVisible || IsCraftPanelVisible || IsEnchantPanelVisible || IsQuenchPanelVisible || (IsTransmutePanelVisible && !IsShiftPressed) ){
		    CloseInventoryPanel();
        }
		return isoLand;
	}
	return 0;
}

// (rt) return 1 means a click on empty floor (for plain movement)
//----- (00409E8C) -------------------------------------------------------- interface
int __fastcall IsoLand_CheckLButtonClick(int shiftPressed, int repeat)
{
	Player& player = Players[CurrentPlayerIndex];

	// (th4) DARK CAMPAIGN HUB: no shift-attacks in the Hideout. Killed here, at the input
	// dispatch, so the click never becomes a command at all -- no CMD_55_SATTACKXY (melee
	// in place) and no CMD_13_RATTACKXY (bow in place) are ever sent. Clearing the flag
	// rather than returning early means a shift-click still behaves like a plain click
	// (walk / pick up / talk) instead of being swallowed.
	if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){
		shiftPressed = 0;
	}

	// SUPER MUTANT MINIGUN BURST PROTECTION
	if (MaxCountOfPlayersInGame == 1
		&& player.gameChanger & BIT(GC_28_SUPERMUTANT)
		&& is(IC_23_PISTOL, player.OnBodySlots[IS_RightHand].ItemCode, player.OnBodySlots[IS_LeftHand].ItemCode)
		//&& is(BI_2733_MINIGUN_01, player.OnBodySlots[IS_RightHand].baseItemIndex, player.OnBodySlots[IS_LeftHand].baseItemIndex)
		) {

		// Block new attacks during burst (frames 1-23 while in ranged attack)
		if (player.CurAction == PCA_5_RANGE_ATTACK && player.currentAnimationFrame <= 23) {
			return 0;
		}
	}
	// end of minigun burst protection
	
	// ENCLAVE SOLDIER ATTACK SPEED PROTECTION
	else if (MaxCountOfPlayersInGame == 1
		&& player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)
		&& is(IC_23_PISTOL, player.OnBodySlots[IS_RightHand].ItemCode, player.OnBodySlots[IS_LeftHand].ItemCode)
		) {

		// Block new attacks during animation (frames 1-15), allow auto-repeat on frame 16
		if (player.CurAction == PCA_5_RANGE_ATTACK && player.currentAnimationFrame > 0 && player.currentAnimationFrame < 13) {
			return 0;
		}
	}
	// end of ES attack speed protection



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
	}else if( !(Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_20_RAVENHOLM*/) ){ // (rt) If town
		if( itemId != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !repeat ){
            NetSendCmdLocParam1( 1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, Cur.Row, Cur.Col, itemId ); // (rt) Picking up an item from the town floor. Works with shift too
			ShiftReleasedEvent = false; // try to fix not getting item after shift
		}else if( CurMon != -1 && !repeat ){
            NetSendCmdLocParam1( 1, CMD_29_TALKXY, Cur.Row, Cur.Col, CurMon ); // (rt) Click on a townsperson
		}else{
			return all( -1, itemId, CurMon, Cur.playerIndex ); // (rt) movement if floor
		}
	}else if( itemId != -1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !shiftPressed && !repeat){ // (rt) Click on an item lying on the dungeon floor
		NetSendCmdLocParam1( 1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, Cur.Row, Cur.Col, itemId );
		ShiftReleasedEvent = false; // try to fix not getting item after shift
	}else if( Cur.ObjectIndex != -1 && ( ! shiftPressed || abs(player.Row - Cur.Row) < 2 && abs(player.Col - Cur.Col) < 2
	 && Objects[Cur.ObjectIndex].destructable == 1) && !repeat ){// (rt) If there is an object under the cursor and it is 1 cell away
        NetSendCmdLocParam1( 1, Cur.GraphicsID == CM_5_TRAP_DISARM ? CMD_17_DISARMXY : CMD_16_OPOBJXY, Cur.Row, Cur.Col, Cur.ObjectIndex );// (rt) activate the object
		//ShiftPressedEvent = false;
		//ShiftReleasedEvent = false;
	}else if( !(player.IsBowInHand || player.IsFlaskInHand) ){ // (rt) not a bow (melee weapon / unarmed)
		if( shiftPressed ){ // (rt) melee with shift
			if( CurMon == -1 || !MonsterTalking(CurMon) ){
                NetSendCmdLoc( PRIORITY_LOW, CMD_55_SATTACKXY, Cur.Row, Cur.Col );// (rt) With shift, a non-archer attacks into empty space
				return 0; // (rt) and stands still
			}
		}else{ // (rt) melee click without shift
			if( CurMon == -1 ){ // (rt) no monster under the cursor
				if( Cur.playerIndex != -1 && !PlayerFriendly() ){
					NetSendCmdParam1( 0, CMD_19_ATTACKPID, Cur.playerIndex );// (rt) melee attack on another player
				}else{
					return all( -1, itemId, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // (rt) movement if floor
				}
			}
		}
		if( Cur.GraphicsID != CM_7_TELEKINESIS ){
			if( !IsPlayerSideMonster( CurMon ) ){	// (th4) summons and the DC escort are not targets
				NetSendCmdParam1( 0, CMD_18_ATTACKID, CurMon );// (rt) Without shift, attack a monster (not a golem/summon)
			}else{
				// (th4) DC mission 13: clicking Lazarus once the lair is clear works like clicking a
				// towner -- the player walks over (that is what the return below already does)
				// and the talk fires from QuestsActions once he is adjacent. Only arm it while
				// the mission is still open, so he cannot be milked after the way home appears.
				// (th4) DC mission 13: clicking Lazarus works like clicking a towner -- the player
				// walks over (the return below already does that) and QuestsActions plays the
				// thanks once he is adjacent. Arming does NOT test that the lair is clear: the
				// firing side does, so a click made while heroes are still up is simply
				// remembered rather than thrown away.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
				 && !DarkCampaignLazarusThanked && CurMon == DarkCampaignLazarusIndex() ){
					DarkCampaignLazarusTalkWanted = true;
				}
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
				 && !DarkCampaignHephastoThanked && CurMon == DarkCampaignHephastoIndex()
				 && DarkCampaignHephastoAtForge() ){
					DarkCampaignHephastoTalkWanted = true;
				}
				// (th4) DC mission 26: clicking the Dark Lord in the ruins arms his last
				// words; they fire from Quests.cpp once the player has reached him.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_44_RAVENHOLM
				 && CurMon == DarkCampaignEndLordIndex() ){
					DarkCampaignEndTalkWanted = true;
				}
				// (th4) DC mission 25: clicking the caged Butcher arms his line and walks the
				// player to DC_BUTCHER_TALK_ROW/COL, the tile in front of the cell. Walking
				// at the monster itself would put him inside the cell; the line fires from
				// Quests.cpp once he is standing on that tile.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_111_CHURCH_SACRISTY
				 && !DarkCampaignButcherIsFreed() && CurMon == DarkCampaignButcherIndex() ){
					DarkCampaignButcherTalkWanted = true;
					NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, DC_BUTCHER_TALK_ROW, DC_BUTCHER_TALK_COL );
					return true;
				}
				return true; // (rt) walk up to one's own minions/golem
			}
		}
	}else{ // (rt) bow or flasks
		if( shiftPressed ){
            NetSendCmdLoc( PRIORITY_LOW, CMD_13_RATTACKXY, Cur.Row, Cur.Col ); // (rt) Shooting a bow with shift
		}else{ // (rt) with a bow, without shift
			if( CurMon == -1 ){ // (rt) Something to do with monsters and most likely the bow, since non-bow attacks were handled already
				if( Cur.playerIndex != -1 && !PlayerFriendly() ){
					NetSendCmdParam1( 0, CMD_21_RATTACKPID, Cur.playerIndex );// (rt) Bow attack on a player
				}else{
					return all( -1, itemId, Cur.ObjectIndex, CurMon, Cur.playerIndex ); // (rt) movement if floor
				}
			}else if( !IsPlayerSideMonster( CurMon ) ){	// (th4) summons and the DC escort are not targets
				NetSendCmdParam1( 0, MonsterTalking( CurMon ) ? CMD_18_ATTACKID : CMD_20_RATTACKID, CurMon ); // (rt) Bow attack on a monster (not a golem); talking monsters are kicked when holding a bow
			}else{
				// (th4) DC mission 13: clicking Lazarus works like clicking a towner -- the player
				// walks over (the return below already does that) and QuestsActions plays the
				// thanks once he is adjacent. Arming does NOT test that the lair is clear: the
				// firing side does, so a click made while heroes are still up is simply
				// remembered rather than thrown away.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
				 && !DarkCampaignLazarusThanked && CurMon == DarkCampaignLazarusIndex() ){
					DarkCampaignLazarusTalkWanted = true;
				}
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
				 && !DarkCampaignHephastoThanked && CurMon == DarkCampaignHephastoIndex()
				 && DarkCampaignHephastoAtForge() ){
					DarkCampaignHephastoTalkWanted = true;
				}
				// (th4) DC mission 26: clicking the Dark Lord in the ruins arms his last
				// words; they fire from Quests.cpp once the player has reached him.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_44_RAVENHOLM
				 && CurMon == DarkCampaignEndLordIndex() ){
					DarkCampaignEndTalkWanted = true;
				}
				// (th4) DC mission 25: clicking the caged Butcher arms his line and walks the
				// player to DC_BUTCHER_TALK_ROW/COL, the tile in front of the cell. Walking
				// at the monster itself would put him inside the cell; the line fires from
				// Quests.cpp once he is standing on that tile.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_111_CHURCH_SACRISTY
				 && !DarkCampaignButcherIsFreed() && CurMon == DarkCampaignButcherIndex() ){
					DarkCampaignButcherTalkWanted = true;
					NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, DC_BUTCHER_TALK_ROW, DC_BUTCHER_TALK_COL );
					return true;
				}
				return true; // (rt) walk up to one's own minions/golem
			}
		}
	}
	return 0; // (rt) in theory you could put return all here and remove all the other returns from the function
}

//----- (0040A0D9) -------------------------------------------------------- game
bool CursorSkillApply() // TryIconCurs
{
	bool applied = true;
	switch (Cur.GraphicsID ){
		case CM_8_RISING:
			//(rt) if(ObjectUnderCursor.playerIndex != -1){ // th1 has no such check
            NetSendCmdParam1(1, CMD_26_RESURRECT, Cur.playerIndex);
			//(rt) MinusManaOrChargeOrRelicByPriceOfSSpell(CurrentPlayerIndex, 0); // th1 has no such call
			//}
			break;
		case CM_10_HEAL_OTHER:
			//(rt) if(ObjectUnderCursor.playerIndex != -1){ // th1 has no such check
            NetSendCmdParam1(1, CMD_85_HEALOTHER, Cur.playerIndex);
			//(rt) MinusManaOrChargeOrRelicByPriceOfSSpell(CurrentPlayerIndex, PS_34_HEAL_OTHER); // th1 has no such call
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

// (th4) Orb-click toggle of the life / mana numbers -- switched off (see OnMouseLButtonUp), kept for reference.
//bool CursorOnManaGlobe() {
//	int centerX = ScreenWidth / 2 + 182;
//	int centerY = ScreenHeight - 100;
//	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
//}
//
//bool CursorOnHealthGlobe() {
//	int centerX = ScreenWidth / 2 - 182;
//	int centerY = ScreenHeight - 100;
//	return (abs(centerX - CursorX)*abs(centerX - CursorX) + abs(centerY - CursorY)*abs(centerY - CursorY)) < 1600;
//}

//----- (0040A2C9) -------------------------------------------------------- interface
void OnMouseLButtonUp()
{
	perkUpArrowPressed = false;
	perkDownArrowPressed = false;
	questUpArrowPressed = questDownArrowPressed = false;	// (th4) quest log scroll arrows
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
	if (IsPowerupPanelVisible) {
		Powerup_MouseClick();
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
	// (th4) Clicking the life / mana orb no longer toggles their numbers: that was easy to hit by accident
	// and the numbers seemed to vanish for no reason. They follow only the config options
	// showNumbersOnHealth / showNumbersOnMana (also in the in-game options menu, menu.cpp).
	//if (CursorOnHealthGlobe()) {
	//	ShowNumbersOnHealth = !ShowNumbersOnHealth;
	//	PlayGlobalSound(75);
	//}
	//else if (CursorOnManaGlobe()) {
	//	ShowNumbersOnMana = !ShowNumbersOnMana;
	//	PlayGlobalSound(75);
	//}

}

//----- (th2) -------------------------------------------------------------
bool __fastcall SpellCasting()
{
	// (rt) If the cursor changed its picture -- don't cast
	if (Cur.GraphicsID != CM_1_NORMAL_HAND) // (rt) Any non-standard cursor (item, telekinesis, recharge, etc.)
		return false;

	Player& player = Players[CurrentPlayerIndex];
	PlayerClass& playerClass = PlayerClasses[player.ClassID];
	int currentSpellNumber = player.CurrentSpellIndex;

	// (rt) Spells cast without specifying the cursor position
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
		// (rt) For other spells, don't cast if:
		if (CursorInMainPanel() // (rt) The cursor is over the main panel
			|| (IsCHARPanelVisible && CursorIntoDisplayObject(CharPanelRect)) // (rt) The cursor is over the character panel
			|| (IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect)) // (rt) The cursor is over the inventory
			|| (IsPerksPanelVisible && CursorIntoDisplayObject(PerksPanelPos)) // (rt) The cursor is over the perks panel
			|| (IsQUESTPanelVisible && CursorIntoDisplayObject(QuestPanelRect)) // (rt) The cursor is over the quests panel
			|| (IsSpellBookVisible && CursorIntoDisplayObject(SpellBookRect)) // (rt) The cursor is over the spells panel
			|| (IsInfoWindowVisible  && CursorIntoDisplayObject(InfoWindowRect))  // Cursor over game changers window
			|| (IsChatHistoryVisible && CursorIntoDisplayObject(ChatHistoryRect)) // Cursor over chat history overlay
			)
		{
			return false;
		}
	}

	// (rt) Sounds made by heroes when trying to cast a spell
	int voiceIndex;

	if (currentSpellNumber == -1) { // (rt) If no spell is set
		voiceIndex = playerClass.Voice[PLAYER_VOICE_NO_SPELL_CHANGED];
		PlayGlobalSound(voiceIndex);
		return false;
	}
	if (!IsSpellEnabledHere(currentSpellNumber, CurrentPlayerIndex)) { // (rt) If the spell is unavailable
		voiceIndex = playerClass.Voice[PLAYER_VOICE_SPELL_DISABLE_IN_TOWN];
		PlayGlobalSound(voiceIndex);
		return false;
	}

	if (CooldownCheck(CurrentPlayerIndex, currentSpellNumber)) { // (rt) If the cooldown counter is running
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

	// (rt) Check that there are resources for casting
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

	// (rt) If resources are insufficient, play a sound
	if (!(DevelopMode && FreeSpell) && !checkCastRequirementPassed) {
		voiceIndex = playerClass.Voice[PLAYER_VOICE_WITH_SPELL_TYPE];
		PlayGlobalSound(voiceIndex);
		return false;
	}

	// (rt) The conditions below are moved over entirely from "int __fastcall Mouse_RButtonDown( int repeat );"

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
	
	if( IsGameMenuActive() || NetCursorItemId || GamePaused == 2 || Players[CurrentPlayerIndex].notHittableInTransition ){// (rt) Unclear condition
		return 0;
	}
	if( IsPopupScreenOpen ){
		if( !repeat ) ClosePopupScreen();
		return 0;
	}
	if( CurrentDialogIndex || IsPerksPanelVisible || IsInfoWindowVisible
		|| (IsChatHistoryVisible && CursorIntoDisplayObject(ChatHistoryRect))
	){
		// by (th4): the belt (bottom hotbar) sits outside these panels, so still allow using a
		// belt item (e.g. quaffing a healing potion) by right-clicking it while a panel is open.
		if( !repeat && !CurrentDialogIndex
		 && Cur.InventoryCellID >= IS_50_47_Belt_Start
		 && Cur.InventoryCellID <  IS_50_47_Belt_Start + IS_8_Belt_Count ){
			RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID, true);
		}
		return 0;
	}
	if( SelectCurSpellMode != 0 && !repeat ){
		SelectCurrentSpell();
		return 0;
	}
	if( repeat && (Cur.InventoryCellID != -1 && !EnteredPanelWithAutoClick || currentSpellNumber == -1) ) return 0;
	
	if( !repeat && IsInterface && !( (!IsSpellBookVisible || !CursorIntoDisplayObject(SpellBookRect)) // (rt) Fix for the belt 5-8 glitch
	 && ( CursorInMainPanel() || ! CursorSkillApply() /*(rt) Something to do with net commands*/
	 && ( Cur.InventoryCellID == -1 /*(rt) If there is no item under the cursor; this seems to be checked in the next sub*/ 
	 || !RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID, true) ) ) ) ){ // may be duplicate call (of below) and needs to remove, or a decompile error
		return 0;
	}
	if( Cur.GraphicsID != CM_1_NORMAL_HAND && !repeat ){
		return 0;
	}
	// RIGHT-CLICK ON A BELT/INVENTORY ITEM -- READ THIS BEFORE CHANGING THE GUARDS HERE.
	//
	// Original note (translated from Russian): "relics did not fire here, because repeat
	// fired [instead]". That is why the line below tests !repeat: without it the auto-
	// repeat re-entered this function and consumed the click before the item path could
	// run, so a relic never went off. Keep that guard.
	//
	// (th4) What that guard did NOT cover, and what sItemUsedThisPress above now does:
	// using a belt item empties its slot, so Cur.InventoryCellID becomes -1. The repeat
	// then arrives with no item under the cursor, !repeat skips this block, and control
	// runs on to SpellCasting() at the end of the function -- casting whatever spell is
	// SELECTED. It was reported as "right-clicking a relic casts Identify instead of the
	// relic", but it is not Identify-specific and not relic-specific: a frame trace showed
	// the relic firing correctly on the press and the selected spell firing on the repeat,
	// whichever spell that was. Potions do the same when the last one in a slot is used.
	//
	// So there are TWO separate things to keep working here, and they pull in opposite
	// directions:
	//   press  (repeat == 0) -> must reach RightClickOnItem so the item is used
	//   repeat (repeat == 1) -> must NOT re-use the item, and must NOT cast a spell
	//                           either, if the press already used something
	// Holding the button on open ground still repeat-casts as before: the flag is only
	// set when an item was actually consumed.
	static bool sItemUsedThisPress = false;   // see the note above
	if( !repeat ) sItemUsedThisPress = false;   // a fresh press starts clean
	if( repeat && sItemUsedThisPress ) return 0;
	if( !repeat && Cur.InventoryCellID != -1 ){
		// Relic use goes through here too -- RightClickOnItem dispatches to
		// UsePotionOrScroll, which handles MC_21/MC_22 relics alongside potions,
		// scrolls, runes and books (Item.cpp, UsePotionOrScroll).
		if( RightClickOnItem(CurrentPlayerIndex, Cur.InventoryCellID, true) ){
			sItemUsedThisPress = true;
			return 0;
		}
	}
	if( (unsigned int)CurrentPlayerIndex >= 4 ){
		TerminateWithError("CheckPlrSpell: illegal player %d", CurrentPlayerIndex);
	}
		//Unexpect3D:
		//(rt) Apparently the "repeat" fires there when the right mouse button is held.
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
			// (rt) List of spells with default info for the new hotkey system
			// (rt) 26 letters of the English alphabet, 12 function keys and CTRL (left and right).
			// (rt) Up to eight spells per key
			// (rt) ToDo: could be moved to "dataseg.cpp"
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

			static bool CustomKeysFileLoaded = false; // (rt) "CustomKeysLoad" will be called only once

			// (rt) Loading "config.ini" (formerly "CustomKeys.txt") with the global spell keys
			// (rt) ToDo: could be moved to "IniFile.cpp" / "Config.cpp"
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

						// (rt) assign the overridden spell to the hotkey's slot
						user_keys[ch][cnt[ch]] = spell; 

						if (cnt[ch] < (shortcut_capacity - 1))
							cnt[ch] += 1; // (rt) move to the next free slot (up to the last one)
					}
					fclose(file);

					//extern volatile int GlobalCustomKeys[shortcuts_amount][shortcut_capacity];

					// (rt) check whether keys were overridden in "config.ini"
					ch = 0;
					for (int i = 0; i < shortcuts_amount; ++i)
						if (cnt[i] > 0) {
							ch = 1;
							break;
					}
					// (rt) if keys were overridden, replace the defaults with them
					if(ch)
						for (int i = 0, j; i < shortcuts_amount; ++i)
							for (j = 0; j < shortcut_capacity; ++j)
								GlobalCustomKeys[i][j] = user_keys[i][j];
				}
			};

			auto ApplyHotkey = [&CustomKeysLoad, &player](int key) {
				
				// (rt) Call the hotkey file loading (loaded once)
				// (rt) ToDo: would be good to move to "Config.cpp"
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
				//(rt) if (!SkipUnusableSpells) { // set the "no spell" icon
				//	player.CurrentSpellNumber = PS_M1_NONE;
				//	player.SpellType = SO_4_DISABLE;
				//}
				return 0;
			};

			// (rt) Catch the keys [A]-[Z], [F1]-[F8], [CTRL]
			if (key >= VK_65_A_KEY && key <= VK_90_Z_KEY) {
				switch (key) { // (rt) exception keys
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
			}else if( IsChatHistoryVisible ){
				ChatHistory_ScrollUp();
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
			}else if( IsChatHistoryVisible ){
				ChatHistory_ScrollDown();
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
			Powerups_Close();
			Quench_Close();
			Transmute_Close();
			IsPerksPanelVisible = false;
			IsInfoWindowVisible = false;
			IsChatHistoryVisible = false;
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
				// §10 fix: buf was char[100]; use MaxChatString_128 and snprintf to
				// prevent latent overflow if version or mode strings grow.
				char buf[MaxChatString_128];
				// (th4) This line is INTENTIONALLY NEVER TRANSLATED -- it is a diagnostic
				// string. It also must not be: it is drawn with the in-game CEL font, and the
				// RU menu-overlay values that getGameModeName()/DifName() return are encoded
				// for the title-screen DiabloFont (CP1251 - 0x40), which that font maps to a
				// blank glyph -- the mode and difficulty names showed as nothing. The *English
				// accessors return the raw table entries. Do not swap these for L()/LMenu().
				if( GameMode == GM_COLISEUM )
					snprintf(buf, sizeof(buf), "%s, mode = %s", THE_HELL_VERSION_HUMAN_STRING, getGameModeNameEnglish(player.gameMode));
				else
					snprintf(buf, sizeof(buf), "%s, difficulty = %s, mode = %s", THE_HELL_VERSION_HUMAN_STRING, DifNameEnglish(), getGameModeNameEnglish(player.gameMode));
				NetSendString(1 << CurrentPlayerIndex, buf);
				break;
			}
		if( beltItemNumberToUse != 0 ){
			int beltItemIndexToUse = beltItemNumberToUse - 1;
			if( not( player.BeltInventory[beltItemIndexToUse].ItemCode, IC_M1_NONE, IC_11_GOLD) ){
				// (rt) if the cursor holds an item or a special pointer,
				// (rt) then only items without a target are used from the belt via keys
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
	void ResetFloatingHealthManaAccumulators(); ResetFloatingHealthManaAccumulators();
	IsChatHistoryVisible = false; // Auto-close overlay on map change (history is preserved)
	if( SomeLevelRandSeed ){
		DungeonRandSeed[(DUNGEON)Dungeon] = SomeLevelRandSeed;
	}
	if( startNewGame ){ StopMusic(); StopAmbient(); }
	SetCursorGraphics(CM_1_NORMAL_HAND);
	SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);
	ProgressUpdate();
	MakeLightTable(); // (rt) palettes of different brightness are set up here, from which tiles of different brightness are then created
	LoadLvlGFX();
	ProgressUpdate();
	if( startNewGame ){
		LoadPanelsGraphic();
		LoadFlipItemCELFiles();
		LoadSpeechPanelData();
		AnimSizeCounted = 0; // reset so SelectPlrAnimation re-scans with correct GC flags for this session
		for( int i = 0; i < MaxCountOfPlayersInGame; i++ ){ // (rt) TODO: Running IsSinglePlayerMode once - a mistake?
			InitPlrGFXMem(i);
		}
		InitInterface();
		SetItem_Init();
		ResetHelp();
	}
	SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]);
	// (th4) DARK CAMPAIGN HUB: the hub vendors sell TOWN stock, but that stock is only ever
	// generated on entering town -- and a Dark Campaign never visits town, so every shop
	// array stayed empty and the vendors had nothing to offer. Stock the hub the same way.
	if( Dungeon == DUN_0_TOWN || Dungeon == DUN_117_DARKLORD_HIDEOUT /*|| Dungeon == DUN_35_PASSAGE_OF_FIRE*/ ){
		ShopItemsGenerationByPlayerLevel();
	}
	ProgressUpdate();
	InitDungeonAutomapData();
	if( Dungeon != DUN_0_TOWN && lvlDir != LVL_4_NODIR ){
		InitLighting();
		InitVision();
	}else if( Dungeon == DUN_0_TOWN && g_ARGBLighting ){
		// (th4) Rev 2.49 Stage-4 item-4 fix: town has no dynamic lights of its own,
		// but the per-level light list survives a dungeon->town transition (vanilla
		// Hellfire skips InitLighting for town too). A stale colored light — the
		// dungeon-side town portal light, which is never LightOff'd when the player
		// travels through the portal (AddWarpMissile adds it only in dungeons and the
		// town-side warp missile adds none) — keeps the Stage-3 idle gate
		// g_LightTintActive ON in town AND across game sessions (a new game also
		// starts in town, which would skip this reset again). The gate then runs the
		// whole Stage-3 per-frame machinery (tint cache snapshot/restore, zoom
		// upscale, ClearTintUnderUI, s3 upload) for nothing: town's LightMap is
		// all-zero, so the V2 fill takes the zeroAmbient fast path and the tint
		// field stays white either way — output is byte-identical, only the skipped
		// work differs (~0.6 ms/frame in town with a portal left behind).
		// Gated on g_ARGBLighting: non-ultra never observes a colored light and keeps
		// its code path byte-for-byte unchanged. FullLighting is re-set to 1 by town
		// generation inside CreateLevel, which runs AFTER this block.
		InitLighting();
	}
	// (th4) A new game or a savegame load must not inherit summons from a previous session.
	// The snapshot itself is taken in InitLevelChange(), where the summons actually die.
	if( startNewGame || lvlDir == LVL_4_NODIR ){
		ClearCarriedSummons();
		ClearCarriedManaShield();
		ClearCarriedReflect();
	}
	InitMonsterSlots();
	ProgressUpdate();
	ClearWarps();
	// (th4) Rev 2.46 Option C: the colored static bake map is per-level state.
	// Reset it on EVERY level entry (town/gen/load/quest). (th4) Rev 2.50: the
	// colors are then rebuilt with the single first-`==`-match replay rule
	// (RebuildStaticTintMap) right after the static baseline is final — fresh
	// generation, save-game loads, per-level-file re-entry and quest maps all
	// use the same rule, so a level looks identical freshly generated and
	// after any save/load round trip. Without the reset a quest floor (or
	// town) entered after a colored dungeon would sample the previous level's
	// colors at the same coordinates.
	if( g_ARGBLighting ){
		// (th4) Step-1 P1 fix: fill with the CPU white sentinel TINT_WHITE
		// (0x00FFFFFF, A byte 0), NOT 0xFF — a 0xFFFFFFFF word is misread by
		// the V2 static pre-fill as "colored at full strength", poisoning the
		// ambient mass and hard-switching dynamic tint colors per cell.
		for( int r = 0; r < FineMap_112; r++ )
			for( int c = 0; c < FineMap_112; c++ )
				StaticTintMap[r][c] = TINT_WHITE;
		g_StaticTintActive = false;
	}
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
				MoveMonstersOffBlockingObjects();	// (th4) nobody stands inside a door (Monster.cpp)
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
				// (th4) Rev 2.50: colored static bake via the single replay
				// rule once the static baseline is final (fresh == all load
				// paths byte-identically).
				RebuildStaticTintMap();
				ProcessLightList(); // Apply player/boss dynamic lights on top of static baseline
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
				// (th4) Rev 2.50: static colors for a level restored from its
				// per-level file (SP re-entry) — same rule as generation.
				RebuildStaticTintMap();
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
				RebuildStaticTintMap();
				ProcessLightList(); // Apply player/boss dynamic lights on top of static baseline
			}else{
				// Load where things were
				LoadLevel();
				RebuildStaticTintMap();
			}
		}else{ // MP quest area. DeltaLoadLevel() makes sure monsters and items are sync'd for quest area.  However, because quest area and its base Dlvl share the same sgLevels[], now monsters killed in base Dlvl will drop items and decrease monster count in quest area, and vice versa.  Player may not be able to finish the quest cuz monster count cannot drop to 0 due to shared sgLevels[].  Must separate sgLevels[] for quest area and its base Dlvl!
			if( startNewGame || lvlDir == LVL_4_NODIR || !Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] ){
				InitItems();
			}
			SavePreLighting(); // Save static baseline (wall lights only, no player/boss)
			RebuildStaticTintMap();
			ProcessLightList(); // Apply player/boss dynamic lights on top
			DeltaLoadLevel(); // Ensures monster count and item drop sync
			MPResyncQuests();
		}

		// Init missiles
		InitMissiles();
		ProgressUpdate();
		SetReturnDungeonPos(Dungeon); // reset return point from quest on load (and return?)
	}
	SyncPortals();
	// (th4) Stage 3 load fix: SyncPortals just (re-)cast the warp missiles and
	// their lights — the visited level's town portal on a stair re-entry, the
	// red portal in Ravenholm — with the default WHITE tint. The per-tick
	// ProcessMissiles assert that normally colors them runs only at the NEXT
	// logic tick, so every frame drawn before it showed the pool untinted and
	// g_LightTintActive still off ("portal flashes white for a split second,
	// then colors on"). Apply the same rule once now, after the last tinted
	// light of this load has been created, so the first drawn frame is already
	// colored. Zero per-frame cost. (Save-game loads get their own call in
	// LoadGame, where the restored missile list is final.)
	ReassertAllMissileLightTints();

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
	// (th4) a level change crossfades, except in Classic (the old track is fading out, ClearPrevSessionData); coming from the
	// meta menu (startNewGame: a new game, a save loaded from the main menu) starts at once (SoundBuffer.cpp MUSIC_FADE_MS).
	// A save loaded IN GAME (lvlDir LVL_4_NODIR: only Saving.cpp loads that way) into a different level type -- church to
	// caves, town to hell... -- also switches at once: the fading old track is cut (Mordor). Same level type: crossfade.
	const bool loadedOtherBiome = lvlDir == LVL_4_NODIR && Music_GraphType() != Dungeon->graphType;
	PlayMusic( 0, !startNewGame && !loadedOtherBiome );
	PlayAmbient();

	while( !ProgressUpdate() ); // (rt) What will run in the loop? Need to check this step by step
	// (rt) Most likely a bar that hasn't reached the end yet will be brought to it.

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
	// (th4) DC m25: LoadLevel() drops him from a revisited hub. NOT on the savegame path --
	// LoadGame re-reads every monster after this returns, so creating him here would burn a
	// boss light row and then be overwritten. LoadGame calls this itself, after the restore.
	if( lvlDir != LVL_4_NODIR ){
		DarkCampaignSpawnButcherIfMissing();
	}
	DarkCampaignCollapseDuplicateButchers();	// (th4) DC m25: never leave two of him
	RestoreCarriedSummons();	// (th4) rebuild the carried summons next to the player
	RestoreCarriedManaShield();	// (th4) and put his mana shield back up
	RestoreCarriedReflect();	// (th4) and his reflect, if he still has charges
	ValidateMonsters();
	//AuditMonsterMap("end of LoadGameLevel"); // (th4) MonsterMap ghost detector -- see TH4_MONSTERMAP_DEBUG in Monster.cpp
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

//----- (th4) ------------------------------------------------------------- ambient event sfx
// One random "event" caw per series on its designated levels. Cosmetic only:
// uses local rand() (no synced-RNG / no MP desync) and a plain tick countdown
// at ENGINE_FPS (no GetTickCountTh(), no global time mutation).
void ProcessAmbientSfx()
{
	enum { AMB_RAVEN, AMB_WIND, AMB_MOAN, AMB_THUMP, AMB_SCREAM };

	bool iceAge = Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE);
	bool isQuest = Dungeon->isQuest;
	DUNGEON_TYPE graph = Dungeon->graphType;

	// series, first enum, count, period-in-seconds, condition
	struct { int idx, first, count, period; bool play; } amb[] = {
		{ AMB_RAVEN,  S_1919_AMB_RAVEN_1, 5,  70, Dungeon == DUN_0_TOWN },
		{ AMB_WIND,   S_1924_COLD_WIND_1, 4, 100, (iceAge && Dungeon == DUN_0_TOWN)
												|| (iceAge && Dungeon == DUN_44_RAVENHOLM)
												|| is(Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH) },
		{ AMB_MOAN,   S_1928_MOAN_1,      6, 100, graph == DT_6_ABYSS && !isQuest },
		{ AMB_THUMP,  S_1934_THUMP,       6, 100, graph == DT_4_HELL && !isQuest },
		{ AMB_SCREAM, S_1940_SCREAM,      6, 100, graph == DT_5_CRYPT && !isQuest },
	};

	for (auto& a : amb) {
		if (!a.play) {
			AmbientSfxCountdown[a.idx] = 0; // 0 = disarmed; re-arm (not play) on next entry
			continue;
		}
		int spread = (a.idx == AMB_RAVEN) ? 40 : 30; // raven 70+rnd(40); others 100+rnd(30)
		if (AmbientSfxCountdown[a.idx] <= 0) {        // freshly entered this level: arm, don't play
			AmbientSfxCountdown[a.idx] = (a.period + rand() % spread) * ENGINE_FPS;
		}
		else if (--AmbientSfxCountdown[a.idx] <= 0) { // timer elapsed: play, then re-arm
			PlayGlobalSound(a.first + rand() % a.count);
			AmbientSfxCountdown[a.idx] = (a.period + rand() % spread) * ENGINE_FPS;
		}
	}
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
			}
			else { // town
				ProcessTowners();
				ProcessItems();
				ProcessMissiles();
			}
			ProcessAmbientSfx(); // th4: town ravens + per-level event ambience
			UpdateSound();

			CheckPlayerStatusMessagesTimeout();
			CheckTriggers();
			QuestsActions();
			if( MaxCountOfPlayersInGame != 1 
				|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_1_AUTO_SAVE))
				|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)) ){        // network game
				i64 tickCount = GetTickCountTh(); // (rt) network game, intercept the call
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
	i64 tick = GetTickCountTh(); // (rt) this timer doesn't affect gameplay, not intercepted
	if( tick - PaletteFrameSync >= battletrance + (MAX_FPS / ENGINE_FPS)){
		PaletteFrameSync = tick;
		if( Dungeon->genType == DT_0_TOWN ){
			// (th4) day/night cycle -- real town only. Ravenholm has its own perpetual
			// palette, and Classic mode has no cycle at all (stays full day). Skipping
			// the call leaves the town palette as loaded, which is the daytime look.
			if( Dungeon != DUN_44_RAVENHOLM && GameMode != GM_CLASSIC ){
				AnimateTownPalette();
			}
			PlayAmbient(); // (th4) re-eval town-tileset ambience each palette frame (town day/night swap; Ravenholm always-night). No-op unless it changed.
		}else if( Dungeon->genType == DT_4_HELL ){
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
			if( Dungeon == DUN_52_MOSSY_GROTTO ){
				AnimateGrottoHallucination();
			}
			else if (!UseReservedSystemPalette) {
				AnimateCavePalette();
				// (rt) turning lava into ice, Lost Treasure Quest
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
			// Berserk weapon auto-switch (Approach A: mimic the manual 'V' swap).
			// Default weapon is the axe (melee). If the target is a pure shooter that
			// kites us, switch to the crossbow set (if one is equipped) and shoot it
			// from range; otherwise make sure we are on the axe set and melee it.
			// We only ever swap when the desired weapon differs from what's in hand,
			// so the weapon doesn't thrash every frame.
			Player& berserker = Players[CurrentPlayerIndex];
			Monster& berserkTarget = Monsters[closestDistanceMonsterIndex];
			// Only treat the target as a "shoot it" shooter if it is actually in line of
			// sight; otherwise the crossbow would fire through walls. No LoS -> melee
			// (which paths toward the target around the obstacle).
			bool targetIsShooter = IsShooterAi(berserkTarget.ai)
				&& CheckCastInSight(berserker.Row, berserker.Col, berserkTarget.Row, berserkTarget.Col);
			bool useCrossbow = false;

			if (targetIsShooter && berserker.OnBodySlots[IS_LeftHand].ItemCode != IC_3_BOW) {
				// want crossbow; switch only if one exists in a set
				useCrossbow = SwapToWeaponSetWith(IC_3_BOW);
			}
			else if (targetIsShooter && berserker.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				useCrossbow = true; // already holding the crossbow
			}
			else if (!targetIsShooter && berserker.OnBodySlots[IS_LeftHand].ItemCode != IC_2_AXE) {
				// want melee; only switch to the axe (never melee with the crossbow).
				// If no axe set exists, leave the current set as-is.
				SwapToWeaponSetWith(IC_2_AXE);
			}

			// If we ended up holding the crossbow against a shooter, shoot from range;
			// otherwise melee.
			if (useCrossbow && berserker.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				NetSendCmdParam1(1, CMD_20_RATTACKID, closestDistanceMonsterIndex);
			}
			else {
				NetSendCmdParam1(1, CMD_18_ATTACKID, closestDistanceMonsterIndex);
			}
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

//----- (th4) --------------------------------------------------------
// (th4) Hold the mouse inside the meta menu.
//
// Why ClipCursor rather than clamping the position the game reports: the menu runs in THREE
// coordinate spaces at once -- the window client (e.g. 1920x1080), the gameplay resolution
// ScreenWidth x ScreenHeight (e.g. 1056x594) and the 640x480 GUI the dialogs are actually laid
// out in. Clamping what GetCursorPos_ hands back changes nothing visible, because that value is
// not what positions the drawn hand. ClipCursor sidesteps the whole question: Windows stops the
// physical pointer, so none of those spaces has to agree with any other.
//
// menuRect arrives in ghMainWnd CLIENT pixels and is mapped to screen here, the way
// RestrictMouseToWindow does above. The caller passes the 640x480 DIALOG space, not the
// stretched rect the menu is drawn into -- see the note at the call site.
//
// No config gate, unlike the gameplay version: leaving the menu releases it, losing focus
// releases it, and Windows drops any clip when another app comes to the foreground, so it
// cannot strand the pointer.
static bool PrevMenuRestrictState = false;

void RestrictMouseToMenu(HWND mWindow, const RECT& menuRect)
{
	if( !mWindow ) return;
	const bool restrictState = !gbRunGame && !VideoWindowExists && IsMainWindowActive && !IsSizingWindow;

	if( !restrictState ){
		if( !PrevMenuRestrictState ) return;				// nothing of ours to drop
		PrevMenuRestrictState = false;
		ClipCursor(nullptr);
		// (th4) That also drops a clip RestrictMouseToWindow may have just set: it fires from the
		// window proc and can beat this to it on the way into a game. It only acts on a CHANGE, so
		// it would never notice and the gameplay restriction would stay dead for the session.
		// Reset its cached state so it re-applies.
		PrevRestrictState = false;
		return;
	}

	POINT ul{ menuRect.left,  menuRect.top    };
	POINT lr{ menuRect.right, menuRect.bottom };
	MapWindowPoints(mWindow, nullptr, &ul, 1);
	MapWindowPoints(mWindow, nullptr, &lr, 1);
	// ClipCursor's right and bottom are EXCLUSIVE; the rect passed in ends on the last pixel.
	const RECT rect = { ul.x, ul.y, lr.x + 1, lr.y + 1 };

	// (th4) Ask the OS what the clip actually IS rather than trusting a cached flag. Windows
	// silently drops any clip when another application takes the foreground, and if the window
	// stops rendering while minimised this function never sees the deactivation at all -- so a
	// "did my own state change" test would come back, find nothing changed, and leave the menu
	// unclipped for the rest of the session. One cheap syscall per menu frame buys that back.
	RECT current;
	if( PrevMenuRestrictState && GetClipCursor(&current) && EqualRect(&current, &rect) ) return;

	PrevMenuRestrictState = true;
	ClipCursor(&rect);
}