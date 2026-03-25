#include "stdafx.h"

DisplayObject MainPanelRect;
DisplayObject InventoryPanelRect;
DisplayObject GoldSplitPanelRect;
int HowMuchGoldYouWantToRemove;// 004D1108
bool IsGoldSplitPanelVisible;// 004D0E44

// добавляем таблицу координат слотов
// таблица базовых слотов (типы и координаты )
BaseSlot BaseSlots[IS_count] = {
	{ IS_Head,       133,   2,  2, 2, SL_ARMOR,  EL_4_HEAD       },
	{ IS_LeftRing,    90, 150,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_RightRing,  203, 150,  1, 1, SL_ARMOR,  EL_5_FINGER     },
	{ IS_Neck,       205,  32,  1, 1, SL_ARMOR,  EL_6_NECK       },
	{ IS_LeftHand,    18,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
	{ IS_RightHand,  249,  32,  2, 4, SL_WEAPON, EL_1_ONE_HANDED },
	{ IS_Torso,      132,  76,  2, 3, SL_ARMOR,  EL_3_TORSO      },
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
	{ IS_Talisman,    17, 222, 10, 7, SL_INV,    EL_7_INVENTORY  }, // сигилы можно класть в обычный но работать будут только в дополнительном ?
};
// перед расширением структур в th2 необоходимо добавить версионирование данных
// Player -> Player2, PlayerInfo -> PlayerInfo2
// если применить наследование, но тогда новые слоты не встанут в массивы OnBodySlots[]

ITEM_SLOT LastInvSlotPicked = IS_Head;
vector<Slot> Slots; // слоты
vector<ItemCell> ItemCells; // клетки слотов (Координаты левого верхнего угла)
Slot* LeftHand, *RightHand, *Inv, *Belt;

// ---- th3 -------------------------------------------------------------------------------
void SetupInvPanel(int playerIndex)
{
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
			}
		}
		if( Players ){
			Player& player = Players[playerIndex];
			switch( base.id ){
			case IS_Inventory: if(   player.activeInv ) continue; break;
			case IS_Talisman : if( ! player.activeInv ) continue; break;
			case IS_LeftRing2 or IS_LeftRing3 or IS_LeftRing4 or IS_RightRing2 or IS_RightRing3 or IS_RightRing4:
				if( ! HasTrait(playerIndex, TraitId::LordOfTheRings) ) continue; break;
			case IS_Neck2: continue; break;
			}
		}
		// TODO: check this out
		short w = CELL_SIZE + (base.equipLoc >= EL_7_INVENTORY); // Слоты 28px в ширину, инвентарь и пояс 29px
		short h = CELL_SIZE + (base.equipLoc == EL_7_INVENTORY); // 28 px Ячейки инвентаря имеют высоту в 29px

		DisplayObject rect;
		rect.Left   = base.x + base.panel.Left + Screen_LeftBorder;
		rect.Top    = base.y + base.panel.Top  + Screen_TopBorder;
		rect.Width  = w * base.c; // Ячейки инвентаря размером 29 x 29px
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

	LeftHand = RightHand = Inv = Belt = 0;
	for( Slot& slot: Slots ){
		switch( slot.id ){
		case IS_LeftHand : LeftHand  = &slot; break;
		case IS_RightHand: RightHand = &slot; break;
		case IS_Inventory:
		case IS_Talisman : Inv  = &slot; break;
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

//----- (0042118B) --------------------------------------------------------
void Inventory_LButtonClick()
{
	int playerIndex = CurrentPlayerIndex;
	Player& player = Players[playerIndex];
    if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
        for( int i = 0; i < 3; ++i ){
            if( IsCursorInInvPanelRect( &WpnSwapBtnsRects[ i ] ) || IsCursorInInvPanelRect( &WpnSwapBtnsRects[ i + 3 ] ) ){
                ChangeAltWeaponPairTo( i );
                return;
            }
        }
		bool gearSwapping = FastInventoryItemSwap && (IsShiftPressed || (IsAltPressed && FastSwapRightOnAlt));
		int takenCell = TakeInvItemToCursor(playerIndex, CursorX, CursorY, gearSwapping);
		if( gearSwapping && ! IsShiftPressed && ! RightSwapAllowed(playerIndex, player.ItemOnCursor) ){
			gearSwapping = false;
		}
		if( gearSwapping && Cur.GraphicsID >= CM_12_ITEMS_PICS_START // fix shift-click gear swap item disapear
		 && player.ItemOnCursor.IsReqMet && not(player.ItemOnCursor.EquippedLocation, EL_7_INVENTORY, EL_8_BELT)){
			HandleGearSwap(takenCell);
		}
    }else{
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

//----- (004209A6) --------------------------------------------------------
int __fastcall TakeInvItemToCursor( int playerIndex, int mX, int mY, bool gearSwapping )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( player.CurAction > PCA_3_WALK_HORISONTAL ) return -1;

	bool isHasGotToTheSlot = false;
	int cellIndex = 0;
	for( ItemCell& cell: ItemCells ){
		BaseSlot& slot = BaseSlots[cell.slot.id];
		int x = cell.x + slot.panel.Left;
		int y = cell.y + slot.panel.Top;
		if( mX >= x && mX < x + cell.w && mY >= y && mY < y + cell.h ){ isHasGotToTheSlot = true; break; }
		cellIndex++;
	}
	if( ! isHasGotToTheSlot ) return -1;

	ItemCell& cell = ItemCells[cellIndex];
	BaseSlot& slot = BaseSlots[cell.slot.id];

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
	}else if( slot.id == IS_Inventory ){ // TODO: add IS_Talisman if ?
		if( int itemNumber = abs(player.InvUsedAlt[cell.r][cell.c]) ){
			for( int r = 0; r < InvRows; ++r ) for( int c = 0; c < InvCols; ++c ){
				if( abs(player.InvUsedAlt[r][c]) == itemNumber ){
					if( invIndex == -1 ) invIndex = r * InvCols + c;
					player.InvUsedAlt[r][c] = 0;
				}
			}
			int itemIndex = itemNumber - 1;
			int lastItemIndex = player.InvItemCount - 1; // было --player.InvItemCount, не оригинально
			int lastItemNumber = lastItemIndex + 1;

			Item& underItem = player.InventorySlots[itemIndex];
			Item& lastItem = player.InventorySlots[lastItemIndex];

			cursorItem = underItem;
			--player.InvItemCount;

			if( player.InvItemCount > 0 && player.InvItemCount != itemIndex ){
				underItem = lastItem;
				for( int r = 0; r < InvRows; ++r ) for( int c = 0; c < InvCols; ++c ){
					auto& curItemNumber = player.InvUsedAlt[r][c];
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

	if (IsShiftPressed 
		&& IsCraftPanelVisible 
		&& CurrentCraftPage == CraftPage::CP_Gems
		&& FindGemInfoByBaseItemIndex(cursorItem.baseItemIndex)
		){
		int slot = -1;
		for (int i = 0; i < 3; i++)
		{
			if (GemCombineItems[i].ItemCode == IC_M1_NONE) {
				slot = i;
				break;
			}
		}
		if (slot != -1) {
			memcpy(&GemCombineItems[slot], &cursorItem, sizeof(Item));
			PlayGlobalSound(S_56_I_INVGRAB);
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

	bool isHasGotToTheSlot = false;
	int cellIndex = 0;
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
		if( xPos >= x && xPos < x + cell.w && yPos >= y && yPos < y + cell.h ){ isHasGotToTheSlot = true; break; }
		cellIndex++;
	}
	if( ! isHasGotToTheSlot ) return;

	ItemCell& cell = ItemCells[cellIndex];
	BaseSlot& slot = BaseSlots[cell.slot.id];
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

	int itemForReplaceNumber;
	if( equipLoc == EL_7_INVENTORY ){
		itemForReplaceNumber = 0;
		allowPut = true;
		if( cursorItem.ItemCode == IC_11_GOLD ){
			if( cell.c >= player.invCols ) return;
			if( int underItemNumber = player.InvUsedAlt[cell.r][cell.c] ){
				if( underItemNumber < 0 ){
					itemForReplaceNumber = -underItemNumber;
				}else if( player.InventorySlots[underItemNumber - 1].ItemCode != IC_11_GOLD ){
					itemForReplaceNumber = underItemNumber;
				}
			}
		}else{
			if( endC > player.invCols || endR > InvRows ) return;

			// проверка на возможность положить предмет в инвентарь
			// можно положить только если под местом куда собираемся положить ничего нет
			// или если есть только один предмет, который мы и заменим на устанавливаемый
			for( int y = startR; y < endR; ++y ) for( int x = startC; x < endC; ++x ){
				int invItemCode = std::abs(player.InvUsedAlt[y][x]);
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
				int underItemNumber = player.InvUsedAlt[cell.r][cell.c];
				if( underItemNumber <= 0 ){// возможна только ситуация == 0. если бы было отрицательное, значит было бы itemForReplace != 0
					// пустая клетка
					int placedItemIndex = player.InvItemCount++;
					int placedItemNumber = placedItemIndex + 1;
					Item& invItem = player.InventorySlots[placedItemIndex];
					memcpy(&invItem, &cursorItem, sizeof(Item));
					player.InvUsedAlt[cell.r][cell.c] = placedItemNumber;
					player.TotalGold += cursorItem.amount;

					SetGraphicToGold(invItem);// оптимизация
					if( invItem.amount > MaxGoldInCell ){
						invItem.GraphicValue = Gold_6_BigPic;
					}
					break;
				}else{
					// клетка с золотом
					int underItemIndex = underItemNumber - 1;
					Item& invItem = player.InventorySlots[underItemIndex];
					int goldSumm = invItem.amount + cursorItem.amount;
					if( goldSumm <= MaxGoldInCell ){
						invItem.amount = goldSumm;
						player.TotalGold += cursorItem.amount;
						SetGraphicToGold(invItem);// оптимизация
					}else{
						int goldToPut = MaxGoldInCell - invItem.amount;
						player.TotalGold += MaxGoldInCell - goldToPut;
						cursorItem.amount -= goldToPut;
						invItem.amount = MaxGoldInCell;
						SetGraphicToGold(invItem);// оптимизация
						SetGraphicToGold(cursorItem);// оптимизация
						cursorGraphics = cursorItem.GraphicValue + CM_12_ITEMS_PICS_START;
					}
				}
				break;
			}else{
				memcpy(&player.InventorySlots[player.InvItemCount++], &cursorItem, sizeof(Item));
				itemForReplaceNumber = player.InvItemCount;
			}
		}else{
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold += cursorItem.amount;
			}
			int itemForReplaceIndex = itemForReplaceNumber - 1;
			cursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&player.InventorySlots[itemForReplaceIndex], &cursorItem);
			if( cursorItem.ItemCode == IC_11_GOLD ){
				player.TotalGold = CalcTotalGold(playerIndex);
			}
			for( int y = 0, ye = InvRows; y < ye; ++y ){
				for( int x = 0, xe = player.invCols; x < xe; ++x ){
					if( abs(player.InvUsedAlt[y][x]) == itemForReplaceNumber ){
						player.InvUsedAlt[y][x] = 0;
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
					player.InvUsedAlt[y][x] = itemForReplaceNumber;
				}else{
					player.InvUsedAlt[y][x] = -itemForReplaceNumber;
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
		// остальные ветки нужны только для отображения текущего состояния кода. 
		// Никогда не выполняются потому что на пояс нельзя класть золото
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
				SetGraphicToGold(beltItem);// оптимизация
			}else{
				int goldToPut = MaxGoldInCell - beltItem.amount;
				player.TotalGold += goldToPut;
				cursorItem.amount -= goldToPut;
				beltItem.amount = MaxGoldInCell;
				SetGraphicToGold(beltItem);// оптимизация
				SetGraphicToGold(cursorItem);// оптимизация
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
	int playerIndex = CurrentPlayerIndex, mX = CursorX, mY = CursorY;
	Player& player = Players[playerIndex];

	bool isHasGotToTheSlot = 0;
	int cellIndex = 0;
	for( ItemCell& cell: ItemCells ){
		BaseSlot& slot = BaseSlots[cell.slot.id];
		int x = cell.x + slot.panel.Left;
		int y = cell.y + slot.panel.Top;
		if( mX >= x && mX < x + cell.w && mY >= y && mY < y + cell.h ){ isHasGotToTheSlot = true; break; }
		cellIndex++;
	}
	if( ! isHasGotToTheSlot ) return -1;

	ItemCell& cell = ItemCells[cellIndex];
	BaseSlot& slot = BaseSlots[cell.slot.id];

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
	}else if( slot.id == IS_Inventory ){ // TODO: add IS_Talisman if ?
		int itemNumber = abs( player.InvUsed[cell.r * slot.c + cell.c] );
		if( ! itemNumber ){
			return -1;
		}
		int itemIndex = itemNumber - 1;
		invIndex = (uchar)itemIndex + IS_Inventory;
		item = &player.InventorySlots[itemIndex];
		// Добавляем квадрат нахождения вещи.
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
		sprintf(InfoPanelHeader, "%i gold %s", item->amount, currency);
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
	// вместо захардкоденых констант поставлены предвычесленные с учетом разрешения координаты

	Surface_DrawCEL(InventoryPanelRect.Left + Screen_LeftBorder, InventoryPanelRect.Down + Screen_TopBorder, InvPanelCEL, 1, InventoryPanelRect.Width);// Тут рисуется сама картинка инвентаря

	Player& player = Players[CurrentPlayerIndex];

	int leftWpnSwapImageLeft = InventoryPanelRect.Left + WpnSwapBtnsRects[0].left;
	int rightWpnSwapImageLeft = InventoryPanelRect.Left + WpnSwapBtnsRects[3].left;
	int swapImagesTop = WpnSwapBtnsRects[0].top + InventoryPanelRect.Top;
	int swapImagesWidth = 60;
	Surface_DrawCEL(leftWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopBorder, WpnSwapBtnImg, player.altWeaponIndex + 1, swapImagesWidth);
	Surface_DrawCEL(rightWpnSwapImageLeft + Screen_LeftBorder, swapImagesTop + Screen_TopBorder, WpnSwapBtnImg, player.altWeaponIndex + 1, swapImagesWidth);

	// Условная подсветка предметов в инвентаре
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
		
		// ниже блок кода для покраски фона инвентаря под предметами
		Item& curItem = player.ItemOnCursor;

		shadowColor = blColor.overcells; // цвет фона слотов инвентаря, доступных для взятого курсором предмета
		useRecolor = true;

		if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START && curItem.ItemCode != IC_M1_NONE) {

			if (GetEquippedLocation[slot.id].first == curItem.EquippedLocation || GetEquippedLocation[slot.id].second == curItem.EquippedLocation) {
				if( curScrX >= slot.rect.Left - itemWidth
				 && curScrX <= slot.rect.Right - itemWidth
				 && curScrY >= slot.rect.Top - itemHeight
				 && curScrY <= slot.rect.Down - itemHeight )
				{
					// устанавливаем цвет фона слота, если курсор с предметом НАД доступным ему слотом
					if (curItem.IsReqMet)
						shadowColor = blColor.overslot;
					else
						shadowColor = blColor.notavailable;
				}
				else if (slot.id == IS_RightHand) { // Коррекция "правой руки"
					// Не отображаем подсветку на правом слоте для орудий дальнего боя взятых курсором
					if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS
						|| (player.fullClassId == PFC_SAVAGE && HasTrait(CurrentPlayerIndex, TraitId::MonkeyGrip))) {
						if(is(curItem.ItemCode, IC_3_BOW, IC_18_FLASK, IC_19_TRAP, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL))
							shadowColor = -1;
					}
					else if (is(curItem.ItemCode, IC_1_SWORD, IC_2_AXE, IC_3_BOW, IC_4_MACE, IC_20_CLAW, IC_10_STAFF, IC_18_FLASK, IC_19_TRAP, IC_20_CLAW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL))
						shadowColor = -1;
				}
				else if (slot.id == IS_LeftHand) { // Коррекция "левой руки"
					// Не отображаем подсветку на левом слоте для щитов (и подобным им предметам) взятых курсором
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
			// useRecolor = false;  // разкоментить, если надо отключать обычный фон у предметов в слотах инвентаря
		}

		// Если предмета в слоте нет, переходим к следующему слоту
		if (bodyItem.ItemCode == IC_M1_NONE) {
			continue; 
		}

		colorMark = InventoryBackgroundOldStyle ? 0 : GetItemBackgroundColorType(bodyItem);

		// стандартная окраска фона инвентаря за предметами
		if (useRecolor) { // пропускаем перекраску, если уже покрашена выше по коду
			ShadowItemSlot(slot.rect.Left, slot.rect.Down, slot.rect.Width, slot.rect.Heigth, colorMark);
		}
		int graphicValue = bodyItem.GraphicValue + CM_12_ITEMS_PICS_START;
		int itemWidth = ItemWidthTable[graphicValue];
		int itemHeight = ItemHeightTable[graphicValue];
		if( !itemWidth || !itemHeight ) continue;
		char* itemsCELArray;

		//if( slotIndex == IS_4_LeftHand ) __debugbreak();
		// поправка для центрирования вещей в слоте
		int x = slot.rect.Left + (slot.rect.Width - itemWidth) / 2;
		int y = slot.rect.Down - (slot.rect.Heigth - itemHeight) / 2;
		if( itemHeight >= CELL_SIZE * 4 ){
			y -= 2;
		}else if( itemHeight >= CELL_SIZE * 3 ){
			y -= 1;
		}
		itemsCELArray = GetItemSprites( graphicValue );

		int colorOfBorder = -1;
		if( Cur.InventoryCellID == slot.id ){// Если курсор над этим слотом
			colorOfBorder = 197;// Выбираем цвет обводки
			if( bodyItem.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !bodyItem.IsReqMet ){
				colorOfBorder = 229;
			}
		}
		CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!(bodyItem.IsReqMet || InventoryTurnOffRedColorItems /*|| Cur.InventoryCellID == slotIndex*/) | bodyItem.mirrorImage << 1, colorOfBorder);
		
		if( slot.id == IS_LeftHand && bodyItem.EquippedLocation == EL_2_TWO_HANDED
		 && ( player.fullClassId != PFC_SAVAGE || ! HasTrait(CurrentPlayerIndex, TraitId::MonkeyGrip) || not(bodyItem.ItemCode, IC_1_SWORD, IC_4_MACE) ) ){
			// Если это двуручное оружие в 4м слоте то бишь в левой руке, то рисуем его полупрозрачную копию в 5й слот то бишь в правой руке
			DisplayObject& rightHandSlotRect = RightHand->rect;
			ShadowItemSlot(rightHandSlotRect.Left, rightHandSlotRect.Down, rightHandSlotRect.Width, rightHandSlotRect.Heigth, colorMark);
			DarkLevel = 0;
			TransparentMode = 1;
			// поправка для центрирования оружия и щита
			int x = rightHandSlotRect.Left + (rightHandSlotRect.Width - itemWidth) / 2;
			int y = rightHandSlotRect.Down - (rightHandSlotRect.Heigth - itemHeight) / 2;
			if( itemHeight >= CELL_SIZE * 4 ){
				y -= 2;
			}else if( itemHeight >= CELL_SIZE * 3 ){
				y -= 1;
			}
			uchar* surfaceDest = WorkingSurface + YOffsetHashTable[y] + x;// + (itemWidth == CELL_SIZE ? CELL_SIZE/2 : 0); // вроде этот кусок больше не нужен
			CEL_DrawShadow(surfaceDest, itemsCELArray, graphicValue, itemWidth, 0, 8, bodyItem.mirrorImage << 1);
			TransparentMode = 0;
		}
	}

	// 0041EF6A
	// в качестве оптимизации вместо координат из массива ItemCellPos
	// используются предвычисленный на основе разрешения прямоугольник инвентаря
	// а координаты вычисляются на основе индекса клетки

	// перемещение "тени" предмета по рюкзаку
	if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START) {
		ChangeItemBacklight(Inv->rect.Left, Inv->rect.Top, player.invCols, InvRows, 0, 0, 2);
	}

	// Проходим по всем клеткам и если используемые, отмечаем определённым цветом
	for( int row = 0; row < InvRows; ++row ) for( int col = 0; col < InvCols; ++col ){
		if( col >= player.invCols ){
			ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
		}else if( player.InvUsedAlt[row][col] ){
			if (InventoryBackgroundOldStyle) {
				//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, cellSize, cellSize);
				ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
			}
			else {
				const char curItemIndex = abs(player.InvUsedAlt[row][col]) - 1;
				if (curItemIndex >= 0 && curItemIndex < IS_70_40_Inv_Count) {
					const Item& curItem = player.InventorySlots[curItemIndex];
					colorMark = GetItemBackgroundColorType(curItem);
					ShadowItemSlot(Inv->rect.Left + col * (CELL_SIZE + 1), Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE, colorMark);

				}
			}
		}
	}

	// рисуем на клетках, обозначенных как имеющие картинку нашу картинку
	for( int row = 0; row < InvRows; ++row ) for( int col = 0; col < InvCols; ++col ){
		int invItemNumber = player.InvUsedAlt[row][col];
		if( invItemNumber <= 0 ){// Если этот слот не обозначен как имеющий картинку
			continue;
		}
		Item& inventorySlot = player.InventorySlots[invItemNumber - 1];
		int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
		char itemWidthTable = ItemWidthTable[graphicValue];
		char* itemsCELArray;
		int x = Inv->rect.Left + col * (CELL_SIZE + 1);
		int y = Inv->rect.Down - (InvRows - 1 - row) * (CELL_SIZE + 1);
		if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
		else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;

		// далее аналогично рисованию вещей на теле
		itemsCELArray = GetItemSprites( graphicValue );

		bool itemUnderRegularCursor = Cur.InventoryCellID == invItemNumber - 1 + IS_Inventory && Cur.GraphicsID < CM_12_ITEMS_PICS_START;

		// Выбираем цвет обводки предмета под курсором
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
		CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /* || itemUnderRegularCursor*/) | inventorySlot.mirrorImage << 1, colorOfBorder);
	}
	
	// Total gold text
	char dest[32];
    int gold = Players[CurrentPlayerIndex].TotalGold;
    sprintf(dest, "gold: %i", gold);
    DrawLevelInfoText(InventoryPanelRect.Right - 300, InventoryPanelRect.Top + 448, dest, 3);
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
	sprintf(InfoPanelHeader, "You have %u gold", GoldYouHave);
	DrawText(textLeft, yOffset + 44, textRight, InfoPanelHeader, 3);
	sprintf(InfoPanelHeader, "%s.  How many do", formatGoldAmount(GoldYouHave));
	DrawText(textLeft, yOffset + 60, textRight, InfoPanelHeader, 3);
	DrawText(textLeft, yOffset + 78, textRight, "you want to remove?", 3);
	sprintf(InfoPanelHeader, "%u", currentAmount);
	DrawLevelInfoText(xOffset + 39, yOffset + 97, InfoPanelHeader, 0);
	
	int pentOffset = xOffset;
	if( currentAmount <= 0 ){
		pentOffset += 50;
	}else{
		pentOffset += 39;
		while( InfoPanelHeader[i] ){
			pentOffset += 1 + FontWidthSmall[FontIndexSmall[Codepage[InfoPanelHeader[i++]]]];
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
}
