#include "stdafx.h"
#include "ItemOverlay.h"

DisplayObject MainPanelRect;
DisplayObject InventoryPanelRect;
DisplayObject GoldSplitPanelRect;
int HowMuchGoldYouWantToRemove;// 004D1108
bool IsGoldSplitPanelVisible;// 004D0E44

// (rt) add the slot coordinate table
// (rt) table of base slots (types and coordinates)
BaseSlot BaseSlots[IS_count] = {
	{ IS_Head,       133,   2,  2, 2, SL_ARMOR,  EL_4_HEAD       },
	{ IS_LeftRing,    90, 150,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_RightRing,  203, 150,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_Neck,       205,  32,  1, 1, SL_ARMOR,  EL_6_NECK       },
	{ IS_LeftHand,    18,  33,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
	{ IS_RightHand,  249,  33,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
	{ IS_Torso,      133,  76,  2, 3, SL_ARMOR,  EL_3_TORSO      },
	{ IS_Waist,      133, 166,  2, 1, SL_ARMOR,  EL_9_WAIST      },
	{ IS_Hands,       18, 148,  2, 2, SL_ARMOR,  EL_10_HANDS     },
	{ IS_Feet,       249, 148,  2, 2, SL_ARMOR,  EL_11_FEET      },
	{ IS_LeftRing2,   90, 182,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_LeftRing3,   90, 118,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_LeftRing4,   90,  86,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_RightRing2, 203, 182,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_RightRing3, 203, 118,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_RightRing4, 203,  86,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_Neck2,       92,  32,  1, 1, SL_ARMOR,  EL_6_NECK       },
  //{ IS_LeftHand2,   18,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
  //{ IS_LeftHand3,   18,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
  //{ IS_RightHand2, 249,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
  //{ IS_RightHand3, 249,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
	{ IS_Inventory,   17, 222, 10, 7, SL_INV,    EL_7_INVENTORY  },
	{ IS_Belt,       205,  20,  8, 1, SL_INV,    EL_8_BELT,      MainPanelRect },
	{ IS_Talisman,    17, 222, 10, 7, SL_INV,    EL_7_INVENTORY  }, // (rt) sigils can go into the normal one, but will only work in the additional one?
};
// (rt) data versioning must be added before extending the structs in th2
// Player -> Player2, PlayerInfo -> PlayerInfo2
// (rt) if inheritance is used, the new slots won't fit into the OnBodySlots[] arrays

ITEM_SLOT LastInvSlotPicked = IS_Head;
vector<Slot> Slots; // (rt) slots
vector<ItemCell> ItemCells; // (rt) slot cells (coordinates of the top-left corner)
Slot* LeftHand, *RightHand, *Inv, *TalismanInv, *Belt;

// ---- th3 -------------------------------------------------------------------------------
void SetupInvPanel(int playerIndex)
{
	// a guard, idk if needed. didn't seem to fix anything
	//if (playerIndex < 0 || playerIndex >= 4) return;

	Slots.reserve(IS_count);
	Slots.clear();
	ItemCells.reserve(194);
	ItemCells.clear();

	for( BaseSlot base: BaseSlots ){
		if( GameMode == GM_CLASSIC ){
			switch(base.id){
			case IS_Head       : base.x = 133; base.y =   2; base.c = 2; base.r = 2; break;
			case IS_LeftRing   : base.x =  48; base.y = 178; base.c = 1; base.r = 1; break;
			case IS_RightRing  : base.x = 249; base.y = 178; base.c = 1; base.r = 1; break;
			case IS_Neck       : base.x = 205; base.y =  32; base.c = 1; base.r = 1; break;
			case IS_LeftHand   : base.x =  18; base.y =  78; base.c = 2; base.r = 3; break;
			case IS_RightHand  : base.x = 249; base.y =  78; base.c = 2; base.r = 3; break;
			case IS_Torso      : base.x = 132; base.y =  76; base.c = 2; base.r = 3; break;
			case IS_Waist:
			case IS_Hands:
			case IS_Feet:		continue; break;
			}
		}
		
		//---------------------------------
		if (Players && playerIndex >= 0 && playerIndex < PlayersMax_4) {// was just "Players" check up to v0.387
			Player& player = Players[playerIndex];

			if (player.gameChanger & BIT(GC_28_SUPERMUTANT)) {
				switch (base.id) {
				case IS_LeftHand:  base.x =  12; base.y = 76; base.c = 4; base.r = 2; break;
				case IS_RightHand: base.x = 198; base.y = 76; base.c = 4; base.r = 2; break;
				}
			}
			else if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
				switch (base.id) {
				case IS_LeftHand:	base.x =  11; base.y = 75; base.c = 3; base.r = 2; break;
				case IS_RightHand:	base.x = 223; base.y = 75; base.c = 3; base.r = 2; break;
				case IS_Torso:		base.x = 104; base.y =  8; base.c = 4; base.r = 7; break;
				case IS_Neck:		base.x = 228; base.y = 32; base.c = 1; base.r = 1; break;
					// left side rings
				case IS_LeftRing:	base.x = 64; base.y = 178; base.c = 1; base.r = 1; break;
				case IS_LeftRing2:	base.x = 28; base.y = 178; base.c = 1; base.r = 1; break;
				case IS_LeftRing3:	base.x = 64; base.y = 141; base.c = 1; base.r = 1; break;
				case IS_LeftRing4:	base.x = 28; base.y = 141; base.c = 1; base.r = 1; break;
					// right side rings
				case IS_RightRing:	base.x = 229; base.y = 178; base.c = 1; base.r = 1; break;
				case IS_RightRing2:	base.x = 265; base.y = 178; base.c = 1; base.r = 1; break;
				case IS_RightRing3:	base.x = 229; base.y = 141; base.c = 1; base.r = 1; break;
				case IS_RightRing4:	base.x = 265; base.y = 141; base.c = 1; base.r = 1; break;
				case IS_Head:
				case IS_Waist:
				case IS_Hands:
				case IS_Feet:
					//case IS_Belt:
					continue; break;// skip these slots entirely for ES
				}
			}
			switch (base.id) {
			case IS_LeftRing2 or IS_LeftRing3 or IS_LeftRing4 or IS_RightRing2 or IS_RightRing3 or IS_RightRing4:
				if (!HasTrait(playerIndex, TraitId::LordOfTheRings) && !(player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER))) continue; break;
			case IS_Neck2: continue; break;
			}
		}
		// TODO: check this out
		short w = CELL_SIZE + (base.equipLoc >= EL_7_INVENTORY); // (rt) Slots are 28px wide, inventory and belt 29px
		short h = CELL_SIZE + (base.equipLoc == EL_7_INVENTORY); // (rt) 28 px Inventory cells are 29px high

		DisplayObject rect;
		rect.Left   = base.x + base.panel.Left + Screen_LeftBorder;
		rect.Top    = base.y + base.panel.Top  + Screen_TopBorder;
		rect.Width  = w * base.c; // (rt) Inventory cells are 29 x 29px
		rect.Heigth = h * base.r;
		rect.Right  = rect.Left + rect.Width;
		rect.Down   = rect.Top  + rect.Heigth;
		Slots.emplace_back(Slot{base.id, rect, base});

		for( short r = 0; r < base.r; ++r ) for( short c = 0; c < base.c; ++c ){
			short x = base.x + c * w;
			short y = base.y + r * h;
			ItemCells.emplace_back(ItemCell{x, y, w, h, r, c, Slots.back()});
		}
	}

	LeftHand = RightHand = Inv = TalismanInv = Belt = 0;
	for( Slot& slot: Slots ){
		switch( slot.id ){
		case IS_LeftHand : LeftHand  = &slot; break;
		case IS_RightHand: RightHand = &slot; break;
		case IS_Inventory: Inv = &slot; break;
		case IS_Talisman : TalismanInv = &slot; break;
		case IS_Belt     : Belt = &slot; break;
		}
	}
}

//----- (th3) -------------------------------------------------------------
bool RightSwapAllowed(int playerIndex, Item& item)
{
	Player& player = Players[playerIndex];
	return item.ItemCode == IC_12_RING || OneHandSpecial(playerIndex, item);
}

int SigilButtonFrame = 1;  // Current frame for sigil button animation
bool SigilButtonPressed = false;  // Optional: if you need pressed state
bool IsSigilInventoryOpen = false;  // Tracks if sigil inventory is open
//----- (0042118B) --------------------------------------------------------
void Inventory_LButtonClick()
{
	// --- Sigil button click check ---
	if (SigilButtonCEL)
	{
		// Disable for Classic and Coliseum modes
		if (GameMode == GM_CLASSIC 
			|| GameMode == GM_COLISEUM 
			|| Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)
			|| Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)
			) {
			//return; // Just ignore clicks in these modes
		}
		else {
			int panelRight = InventoryPanelRect.Right;
			int panelBottom = InventoryPanelRect.Down;

			int btnX = panelRight - 40 - 9;
			int btnY = panelBottom - 32 - 4;  // Removed +32

			if (CursorX >= btnX && CursorX <= btnX + 40 &&
				CursorY >= btnY && CursorY <= btnY + 32)
			{
				PlayGlobalSound(S_75_I_TITLEMOV);

				// Toggle state
				IsSigilInventoryOpen = !IsSigilInventoryOpen;

				// Set frame based on state
				SigilButtonFrame = IsSigilInventoryOpen ? 6 : 1;

				return;
			}
		}
	}


	// rest of the function
	int playerIndex = CurrentPlayerIndex;
	Player& player = Players[playerIndex];
	if (Cur.GraphicsID < CM_12_ITEMS_PICS_START) {
		for (int i = 0; i < 3; ++i) {
			if (IsCursorInInvPanelRect(&WpnSwapBtnsRects[i]) || IsCursorInInvPanelRect(&WpnSwapBtnsRects[i + 3])) {
				ChangeAltWeaponPairTo(i);
				return;
			}
		}
		bool gearSwapping = FastInventoryItemSwap && (IsShiftPressed || (IsAltPressed && FastSwapRightOnAlt));
		int takenCell = TakeInvItemToCursor(playerIndex, CursorX, CursorY, gearSwapping);
		if (gearSwapping && !IsShiftPressed && !RightSwapAllowed(playerIndex, player.ItemOnCursor)) {
			gearSwapping = false;
		}
		if (gearSwapping && Cur.GraphicsID >= CM_12_ITEMS_PICS_START
			&& player.ItemOnCursor.IsReqMet && not(player.ItemOnCursor.EquippedLocation, EL_7_INVENTORY, EL_8_BELT)) {
			HandleGearSwap(takenCell);
		}
	}
	else {
		PutCursorItemToInv(playerIndex, CursorX, CursorY);
	}
}

//----- (th2) -------------------------------------------------------------
void HandleGearSwap(int takenCell)
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.ItemOnCursor.EquippedLocation <= EL_2_TWO_HANDED ){ // weapon
		if( LastInvSlotPicked >= IS_Inventory ){
			if( ! PutWeaponFromCursorToArm(CurrentPlayerIndex, takenCell) ) AutoPutCursorToInventoryOrDrop(true, takenCell);
		}else{
			AutoPutCursorToInventoryOrDrop();
		}
	}else{ // armor
		if( LastInvSlotPicked >= IS_Inventory ){
			if( ! PutArmorToBodySlots() ) AutoPutCursorToInventoryOrDrop(true, takenCell);
		}else{
			AutoPutCursorToInventoryOrDrop();
		}
	}
	SetCursorGraphics(CM_1_NORMAL_HAND);
}

static bool CursorPointsAtCell(int mX, int mY, const ItemCell& cell) {
	BaseSlot& slot = BaseSlots[cell.slot.id];
	int x = cell.x + slot.panel.Left;
	int y = cell.y + slot.panel.Top;

	// Check if the cursor is within the bounds of the cell
	if (!(mX >= x && mX < x + cell.w && mY >= y && mY < y + cell.h))
		return false;

	// Two cells share the same area, but only one of them is active depending on the sigil inventory state
	// This check ensures that we interact with the correct cell based on whether the sigil inventory is open or not
	if (cell.slot.id == IS_Inventory && IsSigilInventoryOpen || cell.slot.id == IS_Talisman && !IsSigilInventoryOpen)
		return false;
	
	return true;
}

static ItemCell * GetCellUnderCursor(int mX, int mY) {
	ItemCell* foundCell = nullptr;
	for (ItemCell& cell : ItemCells) {
		if (CursorPointsAtCell(mX, mY, cell)) {
			foundCell = &cell;
		}
	}
	return foundCell;
}

//----- (004209A6) --------------------------------------------------------
int __fastcall TakeInvItemToCursor( int playerIndex, int mX, int mY, bool gearSwapping )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( player.CurAction > PCA_3_WALK_HORISONTAL ) return -1;

	ItemCell* cellUnderCursor = GetCellUnderCursor(mX, mY);
	if (!cellUnderCursor) return -1;

	ItemCell& cell = *cellUnderCursor;
	BaseSlot& slot = BaseSlots[cell.slot.id];

	//---------------------------------------------------

	if( slot.slotType == SL_ARMOR && player.gameChanger & BIT(GC_2_BT_STRIP) && IsSomeMonstersActivated() ){
		return -1; //No items change during battle
	}
	
	IsGoldSplitPanelVisible = false;
	ShiftReleasedEvent = true;
	cursorItem.ItemCode = IC_M1_NONE;
	LastInvSlotPicked = slot.id;

	int invIndex = -1;
	if( slot.slotType != SL_INV ){
		Item& item = player.OnBodySlots[slot.id];
		if( item.ItemCode != IC_M1_NONE ){
			NetSendCmdDelItem( false, slot.id );
			cursorItem = item;
			item.ItemCode = IC_M1_NONE;
		}
	}else if( slot.id == IS_Inventory || slot.id == IS_Talisman ){
		auto* currentUsedAlt = IsSigilInventoryOpen ? player.TalUsedAlt : player.InvUsedAlt;
		if( int itemNumber = abs(currentUsedAlt[cell.r][cell.c]) ){
			for( int r = 0; r < InvRows; ++r ) for( int c = 0; c < InvCols; ++c ){
				if( abs(currentUsedAlt[r][c]) == itemNumber ){
					if( invIndex == -1 ) invIndex = r * InvCols + c;
					currentUsedAlt[r][c] = 0;
				}
			}
			int itemIndex = itemNumber - 1;
			// Use correct slots array and count for whichever inventory is currently open
			Item* currentSlots = IsSigilInventoryOpen ? player.TalismanSlots : player.InventorySlots;
			int& currentItemCount = IsSigilInventoryOpen ? player.TalItemCount : player.InvItemCount;
			// (th4) A grid cell may name a slot that does not exist: both grids are char[70]
			// holding "slot index + 1", and a damaged save (or any future writer that gets it
			// wrong) can leave a number above the count or above the array. Taking it would
			// index past InventorySlots/TalismanSlots -- the next field in Player is the count
			// itself, then the grid, then the belt (structs.h ~688) -- and would decrement the
			// count below zero. CalcCharParams then calls reserve() with that negative count as
			// size_t and std::length_error kills the process (crash dump 2026-09-20, a sigil
			// grid holding 108 with TalItemCount 0). The cells were already cleared above, so
			// just drop the click: the phantom item disappears and the panel is consistent.
			// SaveInfo.cpp CheckInvalidInvItems repairs this at load time; this is the guard
			// for anything that slips past it in a running game.
			if( itemIndex < 0 || itemIndex >= IS_70_40_Inv_Count || itemIndex >= currentItemCount ){
				cursorItem.ItemCode = IC_M1_NONE;
				if( currentItemCount < 0 ) currentItemCount = 0;
				return invIndex;
			}
			int lastItemIndex = currentItemCount - 1;
			int lastItemNumber = lastItemIndex + 1;

			Item& underItem = currentSlots[itemIndex];
			Item& lastItem = currentSlots[lastItemIndex];

			cursorItem = underItem;
			--currentItemCount;

			// After decrement, currentItemCount now holds the new count.
			// If the removed item was not the last one, swap the last item into its slot and update the grid.
			if( currentItemCount > 0 && currentItemCount != itemIndex ){
				underItem = lastItem;
				for( int r = 0; r < InvRows; ++r ) for( int c = 0; c < InvCols; ++c ){
					auto& curItemNumber = currentUsedAlt[r][c];
					if( curItemNumber == lastItemNumber ){
						curItemNumber = itemNumber;
					}else if( curItemNumber == -lastItemNumber ){
						curItemNumber = -itemNumber;
					}
				}
			}
		}
	}else{ // belt
		Item& item = player.BeltInventory[cell.c];
		if( item.ItemCode != IC_M1_NONE ){
			cursorItem = item;
			item.ItemCode = IC_M1_NONE;
		}
	}

	if( cursorItem.ItemCode == IC_M1_NONE ){
		return invIndex;
	}

	if( IsShiftPressed && IsVisualTradePanelVisible ){
		if( CurrentVisualTradeMode != VTM_Elder ){
			if( VisualTrade_TryToSellItem() ){
				return invIndex;
			}
		}
	}

	if( IsShiftPressed && IsStashPanelVisible && slot.id >= IS_Inventory ){
		if( AddItemToStashFromCursor() ){ // TODO: add playerIndex here
			PlayGlobalSound( S_56_I_INVGRAB );
			return invIndex;
		}
	}

	if( IsShiftPressed && IsTransmutePanelVisible && slot.id >= IS_Inventory ){
		if( Transmute_AutoPutCursorItem(playerIndex) ){
			PlayGlobalSound( S_56_I_INVGRAB );
			return invIndex;
		}
	}

	// (th4) shift-click a gem into the gem combiner: Crafting.cpp Craft_Gems_AutoPutCursorItem
	// (it also refreshes the combine preview, which is what enables the Combine button)
	if( IsShiftPressed && IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && slot.id >= IS_Inventory ){
		if( Craft_Gems_AutoPutCursorItem() ){
			return invIndex;
		}
	}

	if( gearSwapping && ! IsShiftPressed && ! RightSwapAllowed(playerIndex, cursorItem) ){
		gearSwapping = false;
	}
	if ((DropItemOnCtrl || (IsController() && !XinputNotInUse(true))) && IsCtrlPressed && Cur.InventoryCellID <= IS_50_47_Belt_End) {
		if( ! gearSwapping ){
			DropCursorItemBeforeTrig();
			if( NetItem ) SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			FixItemCursor(-1);
			return invIndex;
		}
	}

	if( IsShiftPressed
		&& IsItemAllowedForBelt(player.ItemOnCursor)
		&& CountOfItemsOnBelt() < IS_8_Belt_Count ){
		AutoPutCursorToInventoryOrDrop();
		RecalcPlayer( playerIndex, 1 );
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound( S_56_I_INVGRAB );
		}
	}else{
		if( cursorItem.ItemCode == IC_11_GOLD ){
			player.TotalGold = CalcTotalGold(playerIndex);
		}
		RecalcPlayer( playerIndex, 1 );
		CheckItemStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound( S_56_I_INVGRAB );
			SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			if( !(gearSwapping && cursorItem.IsReqMet && not(cursorItem.EquippedLocation, EL_7_INVENTORY, EL_8_BELT))) FixItemCursor(-1);
		}
	}
	return invIndex;
}

//----- (th2) -------------------------------------------------------------
bool IsArmItemsCompatible( Item& hand, Item& item, int wearDual )
{
	if( is(IC_M1_NONE, hand.ItemCode, item.ItemCode) ) return true;
	enum { UNKNOWN, WEAPON, SHIELD, FLASK };
	auto type = [](Item& i){
		if( i.TypeID == ITEM_1_WEAPON ){
			return is(i.ItemCode, IC_18_FLASK, IC_19_TRAP) ? FLASK : WEAPON ;
		}else if( i.TypeID == ITEM_2_ARMOR ){
			return SHIELD;
		}
		return UNKNOWN;
		};
	if( int handType = type(hand), itemType = type(item); handType == itemType ){
		return handType == WEAPON && wearDual & 1 || handType == SHIELD && wearDual & 2;
	}else{
		return is(B(handType)|B(itemType), B(WEAPON)|B(SHIELD), B(FLASK)|B(SHIELD) );
	}
}

//----- (0041FAD5) --------------------------------------------------------
void __fastcall PutCursorItemToInv( int playerIndex, int mX, int mY )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);

	ItemCell* foundCell = nullptr;
	for( ItemCell& cell: ItemCells ){
		BaseSlot& slot = BaseSlots[cell.slot.id];
		int x = cell.x + slot.panel.Left;
		int y = cell.y + slot.panel.Top;

		int xPos = mX + (Cur.ItemWidth / 2);
		int yPos = mY + (Cur.ItemHeight / 2);
		if( slot.equipLoc == EL_7_INVENTORY ){
			if( ! (ItemWidthCells  & 1) ) xPos -= CELL_SIZE / 2; // even
			if( ! (ItemHeightCells & 1) ) yPos -= CELL_SIZE / 2; // even
		}
		if( slot.equipLoc == EL_8_BELT ){
			if( ! (ItemWidthCells  & 1) ) xPos -= CELL_SIZE / 2; // even
		}
		if( xPos >= x && xPos < x + cell.w && yPos >= y && yPos < y + cell.h ){
			if (cell.slot.id == IS_Inventory && IsSigilInventoryOpen || cell.slot.id == IS_Talisman && !IsSigilInventoryOpen)
				continue;
			foundCell = &cell;
			break; 
		}
	}
	if( ! foundCell ) return;

	ItemCell& cell = *foundCell;
	BaseSlot& slot = BaseSlots[cell.slot.id];

	//---------------------
	if( slot.slotType == SL_ARMOR && player.gameChanger & BIT(GC_2_BT_STRIP) && IsSomeMonstersActivated() ){
		return; //No items change during battle
	}

	bool allowPut = false;
	if( cursorItem.EquippedLocation == slot.equipLoc ){
		allowPut = true;
	}
	auto equipLoc = slot.equipLoc;
	if( equipLoc == EL_1_ONE_HANDED && cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
		equipLoc = EL_2_TWO_HANDED; // TODO: check out wtf is this, base tables should never change (change to locale variable ?)
		if( player.fullClassId == PFC_SAVAGE && is(cursorItem.ItemCode, IC_1_SWORD, IC_4_MACE) && HasTrait(playerIndex, TraitId::MonkeyGrip)){
			equipLoc = EL_1_ONE_HANDED;
		}
		allowPut = true;
	}
	if( cursorItem.EquippedLocation == EL_7_INVENTORY && equipLoc == EL_8_BELT ){
		#ifdef PROPOSAL
		allowPut = CanPutCursorToBelt(player);
		#endif
		allowPut = ItemWidthCells == 1 && ItemHeightCells == 1 && BaseItems[cursorItem.baseItemIndex].UseOnceFlag && cursorItem.IsReqMet && cursorItem.ItemCode != IC_11_GOLD;
	}

	int startC = std::max(cell.c - (ItemWidthCells  - 1) / 2, 0), endC = startC + ItemWidthCells;
	int startR = std::max(cell.r - (ItemHeightCells - 1) / 2, 0), endR = startR + ItemHeightCells;

	// Hoist invUsedAlt here so it is available throughout the whole function
	// (collision check, gold path, placement, etc.)
	auto* invUsedAlt = IsSigilInventoryOpen ? player.TalUsedAlt : player.InvUsedAlt;

	// Only sigils may be placed in the sigil inventory grid; body/belt slots are always allowed
	auto isSigil = [](const Item& item){ return item.baseItemIndex >= BI_2504_SIGIL_FIRST && item.baseItemIndex <= BI_2508_SIGIL_LAST; };
	if( IsSigilInventoryOpen && equipLoc == EL_7_INVENTORY && !isSigil(cursorItem) ) return;

	int itemForReplaceNumber;
	if( equipLoc == EL_7_INVENTORY ){
		itemForReplaceNumber = 0;
		allowPut = true;
		if( cursorItem.ItemCode == IC_11_GOLD ){
			if( cell.c >= player.invCols ) return;
			// Gold can only go in the normal inventory, always use InvUsedAlt regardless of which view is open
			if( IsSigilInventoryOpen ) return; // can't place gold in sigil inventory
			if( int underItemNumber = player.InvUsedAlt[cell.r][cell.c] ){
				if( underItemNumber < 0 ){
					itemForReplaceNumber = -underItemNumber;
				}else if( player.InventorySlots[underItemNumber - 1].ItemCode != IC_11_GOLD ){
					itemForReplaceNumber = underItemNumber;
				}
			}
		}else{
			if( endC > player.invCols || endR > InvRows ) return;

			// (rt) check whether the item can be put into the inventory
			// (rt) it can be placed only if there is nothing under the target place
			// (rt) or if there is exactly one item, which we will replace with the one being placed
			for( int y = startR; y < endR; ++y ) for( int x = startC; x < endC; ++x ){
				int invItemCode = std::abs(invUsedAlt[y][x]);
				if( invItemCode ){
					if( !itemForReplaceNumber ){
						itemForReplaceNumber = invItemCode;
					}else if( itemForReplaceNumber != invItemCode ){
						return;
					}
				}
			}
		}
	}
	if( !allowPut ) return;

	// check 
	if( slot.slotType != SL_INV && !cursorItem.IsReqMet ){
		int soundIndex = S_M1_NO_SOUND;
		switch( player.ClassID ){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE :soundIndex = S_736_WAR_13;  break;
		case PC_1_ARCHER :
		case PC_4_ROGUE  :soundIndex = S_633_ROG_13;  break;
		case PC_2_MAGE   :soundIndex = S_530_SOR_13;  break;
		case PC_3_MONK   :soundIndex = S_850_MONK_13; break;
		}
		if( soundIndex != S_M1_NO_SOUND ) PlayGlobalSound(soundIndex);
		return;
	}
	if( playerIndex == CurrentPlayerIndex ){
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[cursorItem.GraphicValue]]);
	}

	int cursorGraphics = CM_1_NORMAL_HAND;
	Item& bodyItem = player.OnBodySlots[ slot.id ];
	Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
	Item& rightHand = player.OnBodySlots[ IS_RightHand ];
	Item& beltItem = player.BeltInventory[ cell.c ];

	if( slot.slotType != SL_INV && cursorItem.SpellIndex && cursorItem.CurCharges > 0 ){
		player.CurrentSpellIndex = cursorItem.SpellIndex;
		player.SpellType = SO_3_EQUIPED_ITEM;
	}

	// invUsedAlt already declared above - no re-declaration needed

	switch( equipLoc ){
	case EL_4_HEAD or EL_5_FINGER or EL_6_NECK or EL_3_TORSO or EL_9_WAIST or EL_10_HANDS or EL_11_FEET: // th2+ only
        NetSendCmdChItem( false, slot.id );
		if( bodyItem.ItemCode == IC_M1_NONE ){
			memcpy(&bodyItem, &cursorItem, sizeof(Item));
		}else{
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&bodyItem, &cursorItem);
		}
		break;
	case EL_1_ONE_HANDED: {
		int wearDual = (int) is(player.fullClassId, PFC_ROGUE, PFC_DIMACHAERUS, PFC_SAVAGE, PFC_BERSERKER) | HasTrait(playerIndex, TraitId::TwoTowers) << 1;
		bool itemIsCompatibleWithLeft  = IsArmItemsCompatible( leftHand,  cursorItem, wearDual );
		bool itemIsCompatibleWithRight = IsArmItemsCompatible( rightHand, cursorItem, wearDual );

		if( slot.id == IS_RightHand ){
			if( rightHand.ItemCode == IC_M1_NONE ){ // replace two-handed weapon. two-handed weapon moves to cursor
				if( leftHand.ItemCode != IC_M1_NONE && leftHand.EquippedLocation == EL_2_TWO_HANDED
				 && (player.fullClassId != PFC_SAVAGE || not(leftHand.ItemCode, IC_1_SWORD, IC_4_MACE) || !HasTrait(playerIndex, TraitId::MonkeyGrip)) ){
					NetSendCmdChItem( false, IS_LeftHand );
					SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &leftHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &cursorItem );
					break;
				}
				if( !itemIsCompatibleWithLeft ){
					NetSendCmdChItem( false, IS_LeftHand );
					SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &leftHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &cursorItem );
				}else{
					NetSendCmdChItem( false, IS_RightHand );
					memcpy( &rightHand, &cursorItem, sizeof( Item ) );
				}
				break;
			}else{
				if( itemIsCompatibleWithLeft ){
					NetSendCmdChItem( false, IS_RightHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &cursorItem );
					break;
				}else if( itemIsCompatibleWithRight ){
					NetSendCmdChItem( false, IS_LeftHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &leftHand, &cursorItem );
					break;
				}else{
					// Incompatible items in both hands (maybe dual weapons). Need more complicated algorithm
					return;
				}
			}
		}else{ // baseSlot == IS_4_LeftHand
			if( leftHand.ItemCode == IC_M1_NONE ){
				if( !itemIsCompatibleWithRight ){
					NetSendCmdChItem( false, IS_RightHand );
					SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &leftHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &leftHand, &cursorItem );
				}else{
					NetSendCmdChItem( false, IS_LeftHand );
					memcpy( &leftHand, &cursorItem, sizeof( Item ) );
				}
			}else{
				if( itemIsCompatibleWithRight ){
					NetSendCmdChItem( false, IS_LeftHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &leftHand, &cursorItem );
					break;
				}else if( itemIsCompatibleWithLeft ){
					NetSendCmdChItem( false, IS_RightHand );
					cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( &rightHand, &cursorItem );
					break;
				}else{
					return; // Incompatible items in both hands (maybe dual weapons). Need more complicated algorithm
				}
			}
		}
		}break;
	case EL_2_TWO_HANDED:
		NetSendCmdDelItem( false, IS_RightHand );
		if( leftHand.ItemCode == IC_M1_NONE || rightHand.ItemCode == IC_M1_NONE ){
			NetSendCmdChItem( false, IS_LeftHand );
			if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_M1_NONE ){
				memcpy(&leftHand, &cursorItem, sizeof(Item));
			}else{
				if( leftHand.ItemCode == IC_M1_NONE ){
					SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &rightHand);
				}
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &cursorItem);
			}
		}else{
			Item tempItem;
			memcpy(&tempItem, &cursorItem, sizeof(Item));
			Item* item = &rightHand;
			if( rightHand.ItemCode != IC_5_SHIELD ){
				item = &leftHand;
			}
			memcpy(&cursorItem, item, sizeof(Item));
			if( playerIndex == CurrentPlayerIndex ){
				SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}else{
				SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}
			bool isSecondItemPlaced = false;
			for( int i = 0; i < IS_70_40_Inv_Count && !isSecondItemPlaced; ++i ){
				isSecondItemPlaced = AutoPutCursorToCell(player, i, ItemWidthCells, ItemHeightCells, true);
			}
			memcpy(&cursorItem, &tempItem, sizeof(cursorItem));
			if( playerIndex == CurrentPlayerIndex ){
				SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}else{
				SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
			}
			if( ! isSecondItemPlaced ) return;

			if( rightHand.ItemCode == IC_5_SHIELD ){
				rightHand.ItemCode = IC_M1_NONE;
			}else{
				leftHand.ItemCode = IC_M1_NONE;
			}
			NetSendCmdChItem( false, IS_LeftHand );
			if( leftHand.ItemCode == IC_M1_NONE && rightHand.ItemCode == IC_M1_NONE ){
				memcpy(&leftHand, &cursorItem, sizeof(Item));
			}else{
				if( leftHand.ItemCode == IC_M1_NONE ){
					SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &rightHand);
				}
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&leftHand, &cursorItem);
			}
		}
		break;
	case EL_7_INVENTORY:
		if( !itemForReplaceNumber ){
			if( cursorItem.ItemCode == IC_11_GOLD ){
				// Gold always lives in the normal inventory - use InvUsedAlt/InventorySlots directly
				int underItemNumber = player.InvUsedAlt[cell.r][cell.c];
				if( underItemNumber <= 0 ){// (rt) only == 0 is possible. If it were negative, itemForReplace would be != 0
					// (rt) empty cell
					int placedItemIndex = player.InvItemCount++;
					int placedItemNumber = placedItemIndex + 1;
					Item& invItem = player.InventorySlots[placedItemIndex];
					memcpy(&invItem, &cursorItem, sizeof(Item));
					player.InvUsedAlt[cell.r][cell.c] = placedItemNumber;
					player.TotalGold += cursorItem.amount;

					SetGraphicToGold(invItem);// (rt) optimization
					if( invItem.amount > MaxGoldInCell ){
						invItem.GraphicValue = Gold_6_BigPic;
					}
					break;
				}else{
					// (rt) cell with gold
					int underItemIndex = underItemNumber - 1;
					Item& invItem = player.InventorySlots[underItemIndex];
					int goldSumm = invItem.amount + cursorItem.amount;
					if( goldSumm <= MaxGoldInCell ){
						invItem.amount = goldSumm;
						player.TotalGold += cursorItem.amount;
						SetGraphicToGold(invItem);// (rt) optimization
					}else{
						int goldToPut = MaxGoldInCell - invItem.amount;
						player.TotalGold += MaxGoldInCell - goldToPut;
						cursorItem.amount -= goldToPut;
						invItem.amount = MaxGoldInCell;
						SetGraphicToGold(invItem);// (rt) optimization
						SetGraphicToGold(cursorItem);// (rt) optimization
						cursorGraphics = cursorItem.GraphicValue + CM_12_ITEMS_PICS_START;
					}
				}
				break;
			}else{
				// (th4) CAPACITY GUARD. Both inventories are a 70-slot array plus a 70-cell grid
				// (structs.h ~688: InventorySlots/InvItemCount/InvUsed, then TalismanSlots/
				// TalItemCount/TalUsed). Every item occupies at least one cell, so a healthy count
				// never reaches IS_70_40_Inv_Count and this never fires in normal play.
				// It fires only when the count is ALREADY wrong, and that case used to write a
				// whole Item (512 bytes) past the end of the array -- straight over the count and
				// the grid that follow it, which is how a sigil grid ends up holding slot numbers
				// like 71 and 108 (crash dump 2026-09-20). A negative count is just as bad: it
				// writes BEFORE the array. Refusing leaves the item on the cursor, exactly as a
				// full inventory does; nothing is deleted.
				// The repair in SaveInfo.cpp CheckInvalidInvItems fixes such a count at load;
				// this stops one from doing damage inside a running game.
				int& currentCount = IsSigilInventoryOpen ? player.TalItemCount : player.InvItemCount;
				if( currentCount < 0 ) currentCount = 0;
				if( currentCount >= IS_70_40_Inv_Count ) return;
				if (IsSigilInventoryOpen) {
					int newIndex = player.TalItemCount++;
					memcpy(&player.TalismanSlots[newIndex], &cursorItem, sizeof(Item));
					itemForReplaceNumber = newIndex + 1;
				} else {
					int newIndex = player.InvItemCount++;
					memcpy(&player.InventorySlots[newIndex], &cursorItem, sizeof(Item));
					itemForReplaceNumber = newIndex + 1;
				}
			}
		}else{
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold += cursorItem.amount;
			}
			int itemForReplaceIndex = itemForReplaceNumber - 1;
			// Use correct slots depending on which inventory is currently open
			Item* currentInvSlots = IsSigilInventoryOpen ? player.TalismanSlots : player.InventorySlots;
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&currentInvSlots[itemForReplaceIndex], &cursorItem);
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold = CalcTotalGold(playerIndex);
			}
			for( int y = 0, ye = InvRows; y < ye; ++y ){
				for( int x = 0, xe = player.invCols; x < xe; ++x ){
					if( abs(invUsedAlt[y][x]) == itemForReplaceNumber ){
						invUsedAlt[y][x] = 0;
					}
				}
			}
		}
		if( endC > player.invCols || endR > InvRows ){
			return;
		}
		for( int y = startR, ye = endR; y < ye; ++y ){
			for( int x = startC, xe = endC; x < xe; ++x ){
				if( y == endR - 1 && x == startC ){
					invUsedAlt[y][x] = itemForReplaceNumber;
				}else{
					invUsedAlt[y][x] = -itemForReplaceNumber;
				}
			}
		}
		break;
	case EL_8_BELT:
		if( cursorItem.ItemCode != IC_11_GOLD ){
			if( beltItem.ItemCode == IC_M1_NONE ){
				memcpy(&beltItem, &cursorItem, sizeof(Item));
			}else{
				cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&beltItem, &cursorItem);
				if( cursorItem.ItemCode == IC_11_GOLD ){
					player.TotalGold = CalcTotalGold(playerIndex);
				}
			}
		// (rt) the remaining branches only show the current state of the code. 
		// (rt) They never run, because gold can't be put on the belt
		}else if( beltItem.ItemCode == IC_M1_NONE ){
			memcpy(&beltItem, &cursorItem, sizeof(Item));
			player.TotalGold += cursorItem.amount;
		}else if( beltItem.ItemCode != IC_11_GOLD ){
			player.TotalGold += cursorItem.amount;
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&beltItem, &cursorItem);
		}else{
			int goldSumm = beltItem.amount + cursorItem.amount;
			if( goldSumm <= MaxGoldInCell ){
				beltItem.amount = goldSumm;
				player.TotalGold += cursorItem.amount;
				SetGraphicToGold(beltItem);// (rt) optimization
			}else{
				int goldToPut = MaxGoldInCell - beltItem.amount;
				player.TotalGold += goldToPut;
				cursorItem.amount -= goldToPut;
				beltItem.amount = MaxGoldInCell;
				SetGraphicToGold(beltItem);// (rt) optimization
				SetGraphicToGold(cursorItem);// (rt) optimization
				cursorGraphics = cursorItem.GraphicValue + CM_12_ITEMS_PICS_START;
			}
		}
		break;
	}

	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
		if( cursorGraphics == CM_1_NORMAL_HAND ){
			FixItemCursor(1);
		}
		SetCursorGraphics(cursorGraphics);
	}
}

//----- (004228F5) --------------------------------------------------------
int GetSlotMouseAbove_Inv()
{
	

	// rest of the function
	int playerIndex = CurrentPlayerIndex, mX = CursorX, mY = CursorY;
	Player& player = Players[playerIndex];

	ItemCell* cellUnderCursor = GetCellUnderCursor(mX, mY);
	if (!cellUnderCursor) return -1;

	ItemCell& cell = *cellUnderCursor;
	BaseSlot& slot = BaseSlots[cell.slot.id];

	//-------------------------------
	Item* item = 0;
	CurFontColor = 0;
	InfoPanel_ClearBody();

	int invIndex = -1;
	if( slot.slotType != SL_INV ){ // on body
		invIndex = cell.slot.id;
		if( cell.slot.id == IS_RightHand && !(player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE
		 || player.OnBodySlots[IS_LeftHand].EquippedLocation != EL_2_TWO_HANDED
		 || player.fullClassId == PFC_SAVAGE && HasTrait(playerIndex, TraitId::MonkeyGrip) && is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_1_SWORD, IC_4_MACE))){
			invIndex = IS_LeftHand;
		}
	}else if( slot.id == IS_Inventory || slot.id == IS_Talisman ){
		auto* currentUsedAlt = IsSigilInventoryOpen ? player.TalUsedAlt : player.InvUsedAlt;
		Item* currentSlots = IsSigilInventoryOpen ? player.TalismanSlots : player.InventorySlots;
		int itemNumber = abs( currentUsedAlt[cell.r][cell.c] );
		if( ! itemNumber ){
			return -1;
		}
		int itemIndex = itemNumber - 1;
		invIndex = (uchar)itemIndex + IS_Inventory;
		item = &currentSlots[itemIndex];
		// (rt) Add the item's bounding square.
	}else{ // belt
		item = &player.BeltInventory[cell.c];
		if( player.BeltInventory[cell.c].ItemCode == IC_M1_NONE ){
			return -1;
		}
		invIndex = cell.c + IS_50_47_Belt_Start;
	}

	if( invIndex < IS_Inventory ){
		item = &player.OnBodySlots[invIndex];
	}

	if( item->ItemCode == IC_M1_NONE ){
		return -1;
	}
	if( item->ItemCode == IC_11_GOLD ){
		char* currency = formatGoldAmount(item->amount);
		sprintf(InfoPanelHeader, L("%i gold %s"), item->amount, currency);
		return invIndex;
	}
	CurFontColor = ItemColor(*item);
	return invIndex;
}

//----- (th2) -------------------------------------------------------------
uint8_t __fastcall GetItemBackgroundColorType(const Item & item)
{
	uint8_t itemColorType = 0;
	if (not(item.TypeID, ITEM_0_NONE, ITEM_4_GOLD) && not(item.ItemCode, IC_M1_NONE, IC_11_GOLD, IC_14_EAR)) {
		if (!item.IsReqMet) {
			itemColorType = 2;
		}

		if (is(item.MagicLevel, ML_1_MAGIC, ML_2_UNIQUE, ML_3_SET) && item.EquippedLocation > 0 && !item.Identified) {
			itemColorType = 1;
		}
	}
	return itemColorType;
}

pair<EQUIPPED_LOCATION, EQUIPPED_LOCATION> GetEquippedLocation[IS_Inventory] = {
	{EL_4_HEAD      , EL_4_HEAD      }, // IS_Head
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_LeftRing
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_RightRing
	{EL_6_NECK      , EL_6_NECK      }, // IS_Neck
	{EL_1_ONE_HANDED, EL_2_TWO_HANDED}, // IS_LeftHand
	{EL_1_ONE_HANDED, EL_2_TWO_HANDED}, // IS_RightHand
	{EL_3_TORSO     , EL_3_TORSO     }, // IS_Torso
	{EL_9_WAIST     , EL_9_WAIST     }, // IS_Waist
	{EL_10_HANDS    , EL_10_HANDS    }, // IS_Hands
	{EL_11_FEET     , EL_11_FEET     }, // IS_Feet
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_LeftRing2
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_LeftRing3
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_LeftRing4
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_RightRing2
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_RightRing3
	{EL_5_FINGER    , EL_5_FINGER    }, // IS_RightRing4
	{EL_6_NECK      , EL_6_NECK      }, // IS_Neck2
};
//----- (0041E64B) --------------------------------------------------------
void DrawINVPanel()
{
	// (rt) precomputed, resolution-dependent coordinates are used instead of hard-coded constants

	Surface_DrawCEL(InventoryPanelRect.Left + Screen_LeftBorder, InventoryPanelRect.Down + Screen_TopBorder, InvPanelCEL, 1, InventoryPanelRect.Width);// (rt) The inventory picture itself is drawn here

	Player& player = Players[CurrentPlayerIndex];

	int leftWpnSwapImageLeft = InventoryPanelRect.Left + WpnSwapBtnsRects[0].left;
	int rightWpnSwapImageLeft = InventoryPanelRect.Left + WpnSwapBtnsRects[3].left;
	int swapImagesTop = WpnSwapBtnsRects[0].top + InventoryPanelRect.Top;
	int swapImagesWidth = 60;
	Surface_DrawCEL(leftWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopBorder, WpnSwapBtnImg, player.altWeaponIndex + 1, swapImagesWidth);
	Surface_DrawCEL(rightWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopBorder, WpnSwapBtnImg, player.altWeaponIndex + 1, swapImagesWidth);

	// (rt) Conditional highlighting of inventory items
	int curScrX = CursorX + Screen_LeftBorder;
	int curScrY = CursorY + Screen_TopBorder;
	int itemWidth  = Cur.ItemWidth  / 2 + (Cur.ItemWidth  & 1);
	int itemHeight = Cur.ItemHeight / 2 + (Cur.ItemHeight & 1);
	int shadowColor = -1;
	uint8_t colorMark = 0;
	bool useRecolor = true;

	extern bool InventoryBackgroundOldStyle;
	extern bool InventoryTurnOffRedColorItems;

	for( Slot& slot: Slots ){
		if( slot.base.slotType == SL_INV ) break;
		Item& bodyItem = player.OnBodySlots[slot.id];
		
		// (rt) below is the block that colours the inventory background under items
		Item& curItem = player.ItemOnCursor;

		shadowColor = blColor.overcells; // (rt) background colour of inventory slots available for the item held on the cursor
		useRecolor = true;

		if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START && curItem.ItemCode != IC_M1_NONE) {

			if (GetEquippedLocation[slot.id].first == curItem.EquippedLocation || GetEquippedLocation[slot.id].second == curItem.EquippedLocation) {
				if( curScrX >= slot.rect.Left - itemWidth
				 && curScrX <= slot.rect.Right - itemWidth
				 && curScrY >= slot.rect.Top - itemHeight
				 && curScrY <= slot.rect.Down - itemHeight )
				{
					// (rt) set the slot's background colour if the cursor holds an item OVER a slot it can go into
					if (curItem.IsReqMet)
						shadowColor = blColor.overslot;
					else
						shadowColor = blColor.notavailable;
				}
				else if (slot.id == IS_RightHand) { // (rt) "Right hand" correction
					// (rt) No highlight on the right slot for ranged weapons held on the cursor
					if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS
						|| (player.fullClassId == PFC_SAVAGE && HasTrait(CurrentPlayerIndex, TraitId::MonkeyGrip))) {
						if(is(curItem.ItemCode, IC_3_BOW, IC_18_FLASK, IC_19_TRAP, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL))
							shadowColor = -1;
					}
					else if (is(curItem.ItemCode, IC_1_SWORD, IC_2_AXE, IC_3_BOW, IC_4_MACE, IC_20_CLAW, IC_10_STAFF, IC_18_FLASK, IC_19_TRAP, IC_20_CLAW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL))
						shadowColor = -1;
				}
				else if (slot.id == IS_LeftHand) { // (rt) "Left hand" correction
					// (rt) No highlight on the left slot for shields (and similar items) held on the cursor
					if(curItem.ItemCode == IC_5_SHIELD
						// && player.OnBodySlots[IS_4_LeftHand].ItemCode != IC_5_SHIELD
						&& !has(player.traits, TraitId::TwoTowers))
						shadowColor = -1; // 208
				}
				if (shadowColor >= 160) {
					CustomShadowItemSlot(slot.rect.Left, slot.rect.Down, slot.rect.Width, slot.rect.Heigth, shadowColor);
					useRecolor = false;
				}
			}
			// (rt) useRecolor = false;  // uncomment to disable the normal background of items in inventory slots
		}

		// (rt) If there is no item in the slot, go to the next slot
		if (bodyItem.ItemCode == IC_M1_NONE) {
			continue; 
		}

		colorMark = InventoryBackgroundOldStyle ? 0 : GetItemBackgroundColorType(bodyItem);

		// (rt) standard colouring of the inventory background behind items
		if (useRecolor) { // (rt) skip recolouring if it was already coloured further up
			ShadowItemSlot(slot.rect.Left, slot.rect.Down, slot.rect.Width, slot.rect.Heigth, colorMark);
		}
		int graphicValue = bodyItem.GraphicValue + CM_12_ITEMS_PICS_START;
		int itemWidth = ItemWidthTable[graphicValue];
		int itemHeight = ItemHeightTable[graphicValue];
		if( !itemWidth || !itemHeight ) continue;
		char* itemsCELArray;

		//if( slotIndex == IS_4_LeftHand ) __debugbreak();
		// (rt) correction for centring items in the slot
		int x = slot.rect.Left + (slot.rect.Width - itemWidth) / 2;
		int y = slot.rect.Down - (slot.rect.Heigth - itemHeight) / 2;
		if( itemHeight >= CELL_SIZE * 4 ){
			y -= 2;
		}else if( itemHeight >= CELL_SIZE * 3 ){
			y -= 1;
		}
		const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
		itemsCELArray = GetItemSprites( graphicValue );

		int colorOfBorder = -1;
		if( Cur.InventoryCellID == slot.id ){// (rt) If the cursor is over this slot
			colorOfBorder = 197;// (rt) Pick the outline colour
			if( bodyItem.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !bodyItem.IsReqMet ){
				colorOfBorder = 229;
			}
		}
		ItemCEL_Draw( overlayGfx, x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!(bodyItem.IsReqMet || InventoryTurnOffRedColorItems /*|| Cur.InventoryCellID == slotIndex*/) | bodyItem.mirrorImage << 1, colorOfBorder);
		ItemOverlay_Queue(overlayGfx, x, y, itemWidth, itemHeight, bodyItem.mirrorImage != 0, colorOfBorder, (int)!(bodyItem.IsReqMet || InventoryTurnOffRedColorItems /*|| Cur.InventoryCellID == slotIndex*/) | bodyItem.mirrorImage << 1);	// (th4)
		
		if( slot.id == IS_LeftHand && bodyItem.EquippedLocation == EL_2_TWO_HANDED
		 && ( player.fullClassId != PFC_SAVAGE || ! HasTrait(CurrentPlayerIndex, TraitId::MonkeyGrip) || not(bodyItem.ItemCode, IC_1_SWORD, IC_4_MACE) ) ){
			// (rt) If this is a two-handed weapon in slot 4, i.e. the left hand, draw its semi-transparent copy in slot 5, i.e. the right hand
			DisplayObject& rightHandSlotRect = RightHand->rect;
			ShadowItemSlot(rightHandSlotRect.Left, rightHandSlotRect.Down, rightHandSlotRect.Width, rightHandSlotRect.Heigth, colorMark);
			DarkLevel = 0;
			TransparentMode = 1;
			// (rt) correction for centring weapons and shields
			int x = rightHandSlotRect.Left + (rightHandSlotRect.Width - itemWidth) / 2;
			int y = rightHandSlotRect.Down - (rightHandSlotRect.Heigth - itemHeight) / 2;
			if( itemHeight >= CELL_SIZE * 4 ){
				y -= 2;
			}else if( itemHeight >= CELL_SIZE * 3 ){
				y -= 1;
			}
			if( ItemOverlay_HasArt( overlayGfx ) ){	// (th4) replacement art: ghost the PNG, not the old CEL
				ItemOverlay_QueueGhost( overlayGfx, x, y, itemWidth, itemHeight, bodyItem.mirrorImage != 0 );
			}else{
				uchar* surfaceDest = WorkingSurface + YOffsetHashTable[y] + x;// (rt) + (itemWidth == CELL_SIZE ? CELL_SIZE/2 : 0); // this part seems no longer needed
				CEL_DrawShadow(surfaceDest, itemsCELArray, graphicValue, itemWidth, 0, 8, bodyItem.mirrorImage << 1);
			}
			TransparentMode = 0;
		}
	}

	// 0041EF6A
	// (rt) as an optimization, instead of coordinates from the ItemCellPos array
	// (rt) a precomputed, resolution-based inventory rectangle is used
	// (rt) and the coordinates are computed from the cell index

	// Use separate storage arrays depending on which inventory view is open
	auto isSigil = [](const Item& item){ return item.baseItemIndex >= BI_2504_SIGIL_FIRST && item.baseItemIndex <= BI_2508_SIGIL_LAST; };
	Item* currentSlots = IsSigilInventoryOpen ? player.TalismanSlots : player.InventorySlots;
	auto* currentUsedAlt = IsSigilInventoryOpen ? player.TalUsedAlt : player.InvUsedAlt;

	// --- Draw sigil grid overlay BEFORE items so items render on top ---
	if (IsSigilInventoryOpen && SigilGridCEL)
	{
		int gridX = InventoryPanelRect.Left + Screen_LeftBorder + 1;
		int gridY = InventoryPanelRect.Top + Screen_TopBorder + 426;
		Surface_DrawCEL(gridX, gridY, SigilGridCEL, 1, 319);
	}

	// (rt) moving the item's "shadow" across the backpack
	if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START) {
		ChangeItemBacklight(Inv->rect.Left, Inv->rect.Top, player.invCols, InvRows, 0, 0, 2);
	}

	// (rt) Go through all cells and mark the used ones with a certain colour
	for( int row = 0; row < InvRows; ++row ) for( int col = 0; col < InvCols; ++col ){
		if( col >= player.invCols ){
			ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
		}else if(currentUsedAlt[row][col] ){
			if (InventoryBackgroundOldStyle) {
				//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, cellSize, cellSize);
				ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
			}
			else {
				const char curItemIndex = abs(currentUsedAlt[row][col]) - 1;
				if (curItemIndex >= 0 && curItemIndex < IS_70_40_Inv_Count) {
					const Item& curItem = currentSlots[curItemIndex];
					colorMark = GetItemBackgroundColorType(curItem);
					ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE, colorMark);
				}
			}
		}
	}

	// --- Lithe Build: cover the sealed-off right columns of the inventory ---
	// The trait restricts usable columns to player.invCols (the right columns are
	// unusable); draw the padlock cover CEL over that region. Same grid is used for
	// the normal inventory and the sigil "veil", so this one draw covers both.
	if (HasTrait(CurrentPlayerIndex, TraitId::LitheBuild) && LitheBuildCoverCEL) {
		const int litheCoverWidth = 151; // CEL pixel width
		// Surface_DrawCEL is BOTTOM-anchored: y is the bottom scanline and the CEL is
		// drawn upward (same as the inventory panel background draw). So anchor to the
		// bottom-left of the sealed region: grid bottom (Inv->rect.Down), and the left
		// edge of the first sealed column. Inv->rect already includes the screen borders.
		// Tweak litheCoverNudgeX/Y if it needs a few px of adjustment.
		const int litheCoverNudgeX = -2;
		const int litheCoverNudgeY = 3;
		int litheCoverX = Inv->rect.Left + player.invCols * (CELL_SIZE + 1) + litheCoverNudgeX;
		int litheCoverY = Inv->rect.Down + litheCoverNudgeY;
		Surface_DrawCEL(litheCoverX, litheCoverY, LitheBuildCoverCEL, 1, litheCoverWidth);
	}

	// (rt) draw our picture on the cells marked as having a picture
	for( int row = 0; row < InvRows; ++row ) for( int col = 0; col < InvCols; ++col ){
		int invItemNumber = currentUsedAlt[row][col];
		if( invItemNumber <= 0 ){// (rt) If this slot is not marked as having a picture
			continue;
		}
		Item& inventorySlot = currentSlots[invItemNumber - 1];
		int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
		char itemWidthTable = ItemWidthTable[graphicValue];
		char* itemsCELArray;
		int x = Inv->rect.Left + col * (CELL_SIZE + 1);
		int y = Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1);
		if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
		else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;

		// (rt) then the same as drawing items on the body
		const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
		itemsCELArray = GetItemSprites( graphicValue );

		bool itemUnderRegularCursor = Cur.InventoryCellID == invItemNumber - 1 + IS_Inventory && Cur.GraphicsID < CM_12_ITEMS_PICS_START;

		// (rt) Pick the outline colour of the item under the cursor
		int colorOfBorder = -1;
		if( itemUnderRegularCursor ){
			colorOfBorder = 197;
			if( inventorySlot.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !inventorySlot.IsReqMet ){
				colorOfBorder = 229;
			}
		}
		ItemCEL_Draw( overlayGfx, x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /* || itemUnderRegularCursor*/) | inventorySlot.mirrorImage << 1, colorOfBorder);
		ItemOverlay_Queue(overlayGfx, x, y, itemWidthTable, ItemHeightTable[overlayGfx], inventorySlot.mirrorImage != 0, colorOfBorder, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /* || itemUnderRegularCursor*/) | inventorySlot.mirrorImage << 1);	// (th4)
	}
	
	// Total gold text
	char dest[32];
    int gold = Players[CurrentPlayerIndex].TotalGold;
    sprintf(dest, L("gold: %i"), gold); // (th4) localizable
    DrawLevelInfoText(InventoryPanelRect.Right - 300, InventoryPanelRect.Top + 448, dest, 3);
	// (rt) --- Sigil button ---
	if (SigilButtonCEL && GameMode != GM_CLASSIC 
		&& GameMode != GM_COLISEUM 
		&& !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR))
		&& !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER))
		)
	{
		int panelRight = InventoryPanelRect.Right;
		int panelBottom = InventoryPanelRect.Down;

		int absX = panelRight + Screen_LeftBorder;
		int absY = panelBottom + Screen_TopBorder;

		int btnX = absX - 40 - 9;
		int btnY = absY - 32 - 4 + 32;

		Surface_DrawCEL(btnX, btnY, SigilButtonCEL, SigilButtonFrame, 40);
		if (CursorX >= InventoryPanelRect.Right - 40 - 9 &&
			CursorX <= InventoryPanelRect.Right - 40 - 9 + 40 &&
			CursorY >= InventoryPanelRect.Down - 32 - 4 &&
			CursorY <= InventoryPanelRect.Down - 32 - 4 + 32)
		{
			Tooltip_SetOffsetLeft();

			if (IsSigilInventoryOpen){
				Tooltip_AddLine(L("switch to main inventory"), C_0_White);
			}
			else{
				Tooltip_AddLine(L("switch to sigil inventory"), C_0_White);
			}
		}
	}
}

// This might be the function handling splitting gold piles. (drawing the window)
//----- (004070E8) --------------------------------------------------------
void __fastcall SplitGoldPile(int currentAmount)
{
	unsigned int i = 0;
	
	int xOffset = GoldSplitPanelRect.Left;
	int yOffset = GoldSplitPanelRect.Top;
	
	Surface_DrawCEL(xOffset + Screen_LeftBorder, yOffset + 295, CtrlPan_GolddropCEL, 1, GoldSplitPanelRect.Width);
	
	int textLeft = xOffset + 17;
	int textRight = xOffset + 251;
	if( GameLanguage != LANG_EN ){
		sprintf(InfoPanelHeader, L("You have %u gold"), GoldYouHave);
		DrawText(textLeft, yOffset + 44, textRight, InfoPanelHeader, 3);
		DrawText(textLeft, yOffset + 60, textRight, L("How many coins"), 3);
		DrawText(textLeft, yOffset + 78, textRight, L("do you want to take?"), 3);
	}else{
		sprintf(InfoPanelHeader, "You have %u gold", GoldYouHave);
		DrawText(textLeft, yOffset + 44, textRight, InfoPanelHeader, 3);
		sprintf(InfoPanelHeader, "%s.  How many do", formatGoldAmount(GoldYouHave));
		DrawText(textLeft, yOffset + 60, textRight, InfoPanelHeader, 3);
		DrawText(textLeft, yOffset + 78, textRight, "you want to remove?", 3);
	}
	sprintf(InfoPanelHeader, "%u", currentAmount);
	DrawLevelInfoText(xOffset + 39, yOffset + 97, InfoPanelHeader, 0);
	
	int pentOffset = xOffset;
	if( currentAmount <= 0 ){
		pentOffset += 50;
	}else{
		pentOffset += 39;
		while( InfoPanelHeader[i] ){ // (th4) CHI: step by character, not byte
			int lid, w;
			i += ChiNextSmall( (const uchar*)InfoPanelHeader + i, &lid, &w );
			pentOffset += 1 + w;
		}
	}
	Surface_DrawCEL(pentOffset + Screen_LeftBorder, yOffset + 257, PentSpn2CELPtr, PentSpnCurrentPicNumber, 12);
	if( IsMainDraw ) PentSpnCurrentPicNumber = (PentSpnCurrentPicNumber & 7) + 1;
}

//----- (00407217) -------------------------------------------------------- player
int __fastcall GoldSplitHandle(char Key)
{
	Player& player = Players[CurrentPlayerIndex];
	if( !IsGoldSplitPanelVisible ) return 0;
	if( player.CurLife < 64 ){
		IsGoldSplitPanelVisible = false;
		HowMuchGoldYouWantToRemove = 0;
		return 1;
	}
	switch( Key ){
	case VK_13_RETURN_KEY:
		if( HowMuchGoldYouWantToRemove > 0 ){
			GoldSplit(CurrentPlayerIndex, CurCellIndex);
		}
		IsGoldSplitPanelVisible = false;
		return 1;
	case VK_27_ESC_KEY:
		IsGoldSplitPanelVisible = false;
		HowMuchGoldYouWantToRemove = 0;
		return 1;
	case VK_8_BACKSPACE_KEY:
		HowMuchGoldYouWantToRemove /= 10;
		return 1;
	case VK_38_UP_ARROW_KEY:
		if( HowMuchGoldYouWantToRemove < GoldYouHave ){
			HowMuchGoldYouWantToRemove++;
		}
		return 1;
	case VK_40_DOWN_ARROW_KEY:
		if( HowMuchGoldYouWantToRemove > 0){
			HowMuchGoldYouWantToRemove--;
		}
		return 1;
	}
	if( (VK_48_0_KEY <= Key) && (Key <= VK_57_9_KEY) ){
		int NumOfGoldAfterAdd = HowMuchGoldYouWantToRemove * 10 + Key - VK_49_1_KEY + 1;
		if( NumOfGoldAfterAdd <= GoldYouHave ){
			HowMuchGoldYouWantToRemove = NumOfGoldAfterAdd;
		}
		return 1;
	}
	return 0;
}

//----- (00407346) -------------------------------------------------------- player
void __fastcall GoldSplit(int playerIndex, int cellIndex)
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( cellIndex >= IS_50_47_Belt_Start ){
		int beltIndex = cellIndex - IS_50_47_Belt_Start;
		Item& item = player.BeltInventory[beltIndex];
		item.amount -= HowMuchGoldYouWantToRemove;
		if( item.amount <= 0 ){
			ClearBeltSlot(playerIndex, beltIndex);
		}else{
			ChangeGraphValueInBelt(playerIndex, beltIndex);
		}
	}else{
		int inventoryIndex = cellIndex - IS_Inventory;
		Item& item = player.InventorySlots[inventoryIndex];
		item.amount -= HowMuchGoldYouWantToRemove;
		if( item.amount <= 0 ){
			ClearInventorySlot(playerIndex, inventoryIndex);
		}else{
			ChangeGoldGraphicOnQuantity(playerIndex, inventoryIndex);
		}
	}
	SetPlayerHandItem(cursorItem, BI_0_GOLD);
	Item_MakeNonCollisionSeed(playerIndex, cursorItem);
	cursorItem.amount = HowMuchGoldYouWantToRemove;
	cursorItem.IsReqMet = true;
	SetGoldViewOnCursor(playerIndex);
	player.TotalGold = CalcTotalGold(playerIndex);
	HowMuchGoldYouWantToRemove = 0;
}

//----- (00422D3C) --------------------------------------------------------
void OpenGoldSplitPanel()
{
	Player& curPlayer  = Players[CurrentPlayerIndex];
	CurCellIndex       = Cur.InventoryCellID;

	GoldYouHave = curPlayer.InventorySlots[CurCellIndex - IS_Inventory].amount;

	HowMuchGoldYouWantToRemove = 0;
	IsGoldSplitPanelVisible = true;

	if( TalkPanelMode )
		CloseTalkPanel();
}

//----- (th2) -------------------------------------------------------------
void OpenInventoryPanel()
{
	IsSpellBookVisible = false;
	IsPerksPanelVisible = false;
	IsInfoWindowVisible = false;
	IsChatHistoryVisible = false;
	IsGoldSplitPanelVisible = false;
	IsINVPanelVisible = true;
}

//----- (th2) -------------------------------------------------------------
void CloseInventoryPanel()
{
	IsINVPanelVisible = false;
	IsGoldSplitPanelVisible = false;
	IsStashPanelVisible = false;
	VisualTrade_Close();
	Craft_Close();
	Enchant_Close();
	Quench_Close();
	Transmute_Close();
	Powerups_Close();
}
