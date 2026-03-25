#include "stdafx.h"

// Trade action. Used via icon on bottom
enum VisualTradeAction
{
    VTA_M1_None = -1,
    VTA_Repair = 0,
    VTA_RepairAll,
    VTA_Recharge,
    VTA_RechargeAll,
    VTA_Identify,
    VTA_IdentifyAll,
    VTA_Restock,
    VTA_Upgrade,
    
    VTA_Count
};

struct VisualTradeModeInfo
{
    std::vector<VisualTradePage> pages;
    std::vector<VisualTradeAction> actions;
};

const VisualTradeModeInfo VisualTradeModeInfos[VTM_Count] = {
    /* VTM_Blacksmith */{ {VTP_BlacksmithBasic, VTP_BlacksmithPremium}, {VTA_Repair, VTA_RepairAll, VTA_Restock, VTA_Upgrade} },
    /* VTM_Witch      */{ {VTP_WitchBasic},                             {VTA_Recharge, VTA_RechargeAll, VTA_Restock} },
    /* VTM_Gamble     */{ {VTP_GambleBasic},                            {} },
    /* VTM_Healer     */{ {VTP_HealerBasic},                            {} },
    /* VTM_Elder      */{ {},                                           {VTA_Identify, VTA_IdentifyAll} }
};

const char* VisualTradePagesNames[VTP_Count] = {
    /* VTP_BlacksmithBasic      */ "BASIC",
    /* VTP_BlacksmithPremium    */ "PREMIUM",
    /* VTP_WitchBasic           */ "TRADE",
    /* VTP_GambleBasic          */ "TRADE",
    /* VTP_HealerBasic          */ "TRADE",
};

struct VisualTradeActionInfo
{
    const char* name;
    int icon;
};

const VisualTradeActionInfo VisualTradeActionInfos[VTA_Count] = {
    /* VTA_Repair       */ { "Repair",          0 },
    /* VTA_RepairAll    */ { "Repair All",     20 },
    /* VTA_Recharge     */ { "Recharge",        6 },
    /* VTA_RechargeAll  */ { "Recharge All",   22 },
    /* VTA_Identify     */ { "Identify",        8 },
    /* VTA_IdentifyAll  */ { "Identify All",   24 },
    /* VTA_Restock      */ { "Restock",         4 },
    /* VTA_Upgrade      */ { "Upgrade",        10 }
};
int VisualTrade_ExitButtonIcon = 9;


VisualTradeMode CurrentVisualTradeMode = VTM_M1_None;
VisualTradePage CurrentVisualTradePage = VTP_M1_None;

char* VisualTradePanelCEL = nullptr;
char* VisualTradeTabsCEL = nullptr;
char* VisualTradeBtnsCEL = nullptr;
bool IsVisualTradePanelVisible = false;
DisplayObject VisualTradePanel;
DisplayObject VisualTradeInfostring;
DisplayObject VisualTradeInventory;

DisplayObject VisualTradeButtons[VisualTrade_ButtonsAmount];
bool VisualTradeButtonsStates[VisualTrade_ButtonsAmount];

char VisualTradeInfostringBuffer[256] = "";


static_assert(VisualTrade_MaxSlot < std::numeric_limits<char>::max(), "Can't use char as grid type for VisualTrade");

VisualTradeStorage VisualTradeStorages[VTP_Count];

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_ClearInfostring()
{
    VisualTradeInfostringBuffer[0] = '\0';
}

//----- (th2) -------------------------------------------------------------
void SetVisualTradeInfostring( const char* str )
{
    strcpy(VisualTradeInfostringBuffer, str);
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_ClearPage( VisualTradePage pageId )
{
    VisualTradeStorage& storage = VisualTradeStorages[pageId];
    storage.itemCount = 0;
    for( auto& flag : storage.usedFlags ){
        flag = 0;
    }
}

//----- (th2) -------------------------------------------------------------
bool ItemPanel_PutItemInStorage( VisualTradePage pageId, const Item* item, int aCellID, int aWidth, int aHeight, int isNotRoomTest )
{
	VisualTradeStorage& storage = VisualTradeStorages[pageId];
 
	bool haveRoomForItem = true;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = aCellID / VisualTrade_SlotsX;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; haveRoomForItem; i++ ){// Вычисление есть ли место для вещи
		if( currentInvYPosition >= VisualTrade_SlotsY ){
			haveRoomForItem = false;
			return haveRoomForItem;// На нет и суда нет
		}
		currentInvXPosition = aCellID % VisualTrade_SlotsX;
		if( currentInvXPosition < 0 ){// Если отрицательный - обнуляем
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( !haveRoomForItem ){// То есть по сути чтобы цикл вернул 1 нужно чтобы клетки инвентаря от текущей по х размер ширины были пусты.
				return haveRoomForItem;// На нет и суда нет
			}
			haveRoomForItem = ! abs((char)storage.usedFlags[currentInvXPosition + currentInvYPosition * VisualTrade_SlotsX]);
			currentInvXPosition++;
		}
		currentInvYPosition++;
		if( i == aHeight - 1 ){//если есть место для вещи
			break; // При выходе haveRoomForItem будет 1, что с ним делать следующая конструкция сама решить
		}
	}
	if( !haveRoomForItem || !isNotRoomTest ){
		return haveRoomForItem;
	}
	// Собственно впихивание вещи. haveRoomForItem скорее всего будет равно 1, ибо в случаях когда не равно в предыдущем цикле сразу выход
	memcpy(&storage.items[storage.itemCount], item, sizeof(Item));
	storage.itemCount++;
	currentInvYPosition = aCellID / VisualTrade_SlotsX;

	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; i < aHeight; i++ ){
		currentInvXPosition = aCellID % VisualTrade_SlotsX;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( j || i != aHeight - 1 ){// Обозначаем клетки как занятые вещью
				storage.usedFlags[currentInvXPosition + currentInvYPosition * VisualTrade_SlotsX] = -storage.itemCount; // Если одна из остальных клеток
			}else{
				storage.usedFlags[currentInvXPosition + currentInvYPosition * VisualTrade_SlotsX] = storage.itemCount; // Если клетка с картинкой
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}
	return haveRoomForItem;
}

//----- (th2) -------------------------------------------------------------
void GetForcedSuccessionArray(uchar inventoryWidth, uchar inventoryHeight, uchar itemWidth, uchar itemHeight, vector<uchar>& successionArray)
{
    int successinWidth = inventoryWidth - itemWidth + 1;
	int successinHeight = inventoryHeight - itemHeight + 1;

	for( int j = (successinWidth - 1); j >= 0; j-- ){
        for( int i = 0; i < successinHeight; i++ ){
            successionArray.push_back(i * inventoryWidth + j);
        }
    }
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_AutoPutItemInStorage( VisualTradePage pageId, const Item* item, bool forced = false )
{
    SetupItemSizes(item->GraphicValue + CM_12_ITEMS_PICS_START);
	vector<uchar> successionArray; // Сюда копируется приоритет места инвентаря для каждой позиции.
	successionArray.reserve(VisualTrade_MaxSlot * 2);

    if( forced ){
        GetForcedSuccessionArray(VisualTrade_SlotsX, VisualTrade_SlotsY, ItemWidthCells, ItemHeightCells, successionArray);
    }else{
        GetSuccessionArray(VisualTrade_SlotsX, VisualTrade_SlotsY, ItemWidthCells, ItemHeightCells, successionArray);
    }
	for( auto cellId: successionArray ){
		if( ItemPanel_PutItemInStorage(pageId, item, cellId, ItemWidthCells, ItemHeightCells, 1) ) return true;
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_RemoveItemFromStorage( VisualTradePage pageId, int invItemIndex )
{
	VisualTradeStorage& storage = VisualTradeStorages[pageId];

    if( storage.itemCount <= 0 ) return; // assert

	for(auto& usedFlag : storage.usedFlags){
		if( abs(usedFlag) == invItemIndex + 1 ){
			usedFlag = 0;
		}
	}

	--storage.itemCount;
	int invItemCount = storage.itemCount;
	if( invItemCount > 0 && invItemCount != invItemIndex ){// Если вещи есть и удаляемая вещь не последняя
		memcpy(&storage.items[invItemIndex], &storage.items[invItemCount], sizeof(Item));
		for(auto& invUsed : storage.usedFlags){
			int invItemNumber = invItemIndex + 1;
			int invItemNumber2 = invItemCount + 1;
			if( invUsed == invItemNumber2 ){
                invUsed = invItemNumber;
			}
			if( invUsed == -invItemNumber2 ){
                invUsed = -invItemNumber;
			}
		}
	}
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_GetPageItemsInfo( VisualTradePage pageId, Item** items, int* itemsCount, int* forcedItemsCount )
{
    *items = nullptr;
    *itemsCount = 0;
    *forcedItemsCount = 0;

    switch(pageId){
        case VTP_BlacksmithBasic:
            *items = BasicalItemsArray;
            *itemsCount = countof(BasicalItemsArray);
            break;
        case VTP_BlacksmithPremium:
            *items = MagicItemsStoreArray;
            *itemsCount = countof(MagicItemsStoreArray);
            break;
        case VTP_WitchBasic:
            *items = WitchAdriaItems;
            *itemsCount = countof(WitchAdriaItems);
            *forcedItemsCount = PT_Witch_ForcedItemsCount;
            break;
        case VTP_GambleBasic:
            *items = &WirtItem;
            *itemsCount = 1;
            break;
        case VTP_HealerBasic:
            *items = PepinItems;
            *itemsCount = countof(PepinItems);
            *forcedItemsCount = (MaxCountOfPlayersInGame == 1) ? PT_2_Healer_Singleplayer_ForcedItemsCount
		                                                      : PT_3_Healer_Multiplayer_ForcedItemsCount;
            break;
    }
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_RefreshPage( VisualTradePage pageId )
{
    VisualTradeStorage& storage = VisualTradeStorages[pageId];
    
    Item* items = nullptr;
    int itemsCount = 0;
    int forcedItemsCount = 0;
    
    VisualTrade_GetPageItemsInfo( pageId, &items, &itemsCount, &forcedItemsCount );
    
    // Remove storage items not presented in items array
AGAIN:
    for( int i = 0, ie = storage.itemCount; i < ie; ++i ){
        const Item& item = storage.items[i];
        int seed = item.seed;
        bool found = false;
        for( int j = 0; j < itemsCount; ++j ){
            const Item& origItem = items[j];
            if( origItem.ItemCode != IC_M1_NONE && origItem.seed == seed ){
                found = true;
                break;
            }
        }
        if( !found ){
            VisualTrade_RemoveItemFromStorage( pageId, i );
            goto AGAIN;
        }
    }
    
    // Add items presented in items array but not in storage
    for( int j = 0; j < itemsCount; ++j ){
        const Item& origItem = items[j];
        int seed = origItem.seed;
        bool found = false;
        bool forcedItem = j < forcedItemsCount;
        if( origItem.ItemCode != IC_M1_NONE ){
            for( int i = 0, ie = storage.itemCount; i < ie; ++i ){
                const Item& item = storage.items[i];
                if( item.seed == seed ){
                    found = true;
                    break;
                }
            }
            if( !found ){
                VisualTrade_AutoPutItemInStorage( pageId, &origItem, forcedItem );
            }
        }
    }
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_ResetPage( VisualTradePage pageId )
{
    VisualTrade_ClearPage(pageId);
    VisualTrade_RefreshPage(pageId);
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_Open( VisualTradeMode mode )
{
	IsVisualTradePanelVisible = true;
	IsINVPanelVisible = true;
	
	CurrentVisualTradeMode = mode;
	const auto& pages = VisualTradeModeInfos[mode].pages;
	if( pages.empty() ){
	    CurrentVisualTradePage = VTP_M1_None;
    }else{
	    CurrentVisualTradePage = pages.front();
    }
    VisualTrade_ClearInfostring();
	
	for( bool& state : VisualTradeButtonsStates ){
	    state = false;
    }
	
	if( is( Cur.GraphicsID, CM_2_IDENTIFY, CM_3_REPAIR, CM_4_STAFF_RECHARGE) ){
        SetCursorGraphics(CM_1_NORMAL_HAND);
    }
	
	CurrentDialogIndex = PD_0_None;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_Close()
{
    IsVisualTradePanelVisible = false;
    if( is( Cur.GraphicsID, CM_2_IDENTIFY, CM_3_REPAIR, CM_4_STAFF_RECHARGE) ){
        SetCursorGraphics(CM_1_NORMAL_HAND);
    }
}

//----- (th2) -------------------------------------------------------------
void SetupTradePanel()
{
	SetupPanelRect( VisualTradePanel, LeftPanel, VisualTrade_Width, VisualTrade_Height );
	
	const int tabsY = VisualTradePanel.Top + 6;
    const int tabsWidth = 73;
    const int tabsHeight = 25;

    for( int i = 0; i < VisualTrade_TabsAmount; ++i ){
        SetupDisplayObject(VisualTradeButtons[VisualTrade_TabsStartIndex + i], VisualTradePanel.Left + 25 + (tabsWidth+10)*i, tabsY, tabsWidth, tabsHeight);
    }
    
    const int buttonsWidth = 33;
    const int buttonsHeight = 32;
    const int buttonsY = VisualTradePanel.Down - buttonsHeight - 12;
	
	for( int i = 0; i < VisualTrade_ActionButtonsAmount; ++i ){
	    SetupDisplayObject(VisualTradeButtons[VisualTrade_ActionButtonsStartIndex + i], VisualTradePanel.Left + 12 + (buttonsWidth+6)*i, buttonsY, buttonsWidth, buttonsHeight);
    }
    
    SetupDisplayObject(VisualTradeButtons[VisualTrade_ExitButtonStartIndex], VisualTradePanel.Right - 12 - buttonsWidth, buttonsY, buttonsWidth, buttonsHeight);
    SetupDisplayObject(VisualTradeInfostring, VisualTradePanel.Left + 14, VisualTradePanel.Down - 50 - 19 + 3, 294, 15 - 6);
    SetupDisplayObject(VisualTradeInventory, VisualTradePanel.Left + 15 + 2, VisualTradePanel.Down - 14 + 8 - 74 - VisualTrade_SlotsY * 29, 29 * VisualTrade_SlotsX, 29 * VisualTrade_SlotsY);
}

//----- (th2) -------------------------------------------------------------
void DrawVisualTradeItems()
{
    if( CurrentVisualTradePage == VTP_M1_None ){
        return;
    }
    const auto& storage = VisualTradeStorages[CurrentVisualTradePage];

    extern bool InventoryBackgroundOldStyle;
    extern bool InventoryTurnOffRedColorItems;
    
	for( int row = 0; row < VisualTrade_SlotsY; ++row ){
		for( int col = 0; col < VisualTrade_SlotsX; ++col ){
			if( storage.usedFlagsAlt[row][col] ){
                if (InventoryBackgroundOldStyle) {
                    ShadowItemSlot(VisualTradeInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), VisualTradeInventory.Down + Screen_TopBorder - (VisualTrade_SlotsY - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
                }
                else {
                    const char curItemIndex = abs(storage.usedFlagsAlt[row][col]) - 1;
                    if (curItemIndex >= 0 && curItemIndex < VisualTrade_MaxSlot) {
                        const Item& curItem = storage.items[curItemIndex];
                        ShadowItemSlot(VisualTradeInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), VisualTradeInventory.Down + Screen_TopBorder - (VisualTrade_SlotsY - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE, GetItemBackgroundColorType(curItem));
                    }
                }
            }
		}
	}

	for( int row = 0; row < VisualTrade_SlotsY; ++row ){
		for( int col = 0; col < VisualTrade_SlotsX; ++col ){
            int invItemIndex = storage.usedFlagsAlt[row][col];
            if( invItemIndex <= 0 ){
                continue;
            }
            const Item& inventorySlot = storage.items[invItemIndex - 1];
            int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
            char itemWidthTable = ItemWidthTable[graphicValue];
            
            int x = VisualTradeInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1);
			int y = VisualTradeInventory.Down + Screen_TopBorder - (VisualTrade_SlotsY - 1 - row) * (CELL_SIZE + 1);
			if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
			else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;
			
            char* itemsCELArray = GetItemSprites(graphicValue);
            bool itemUnderCursor = Cur.InventoryCellID == invItemIndex - 1 + Stash_SlotIDStart;
            // Выбираем цвет обводки
			int colorOfBorder = -1;
			if(itemUnderCursor){
                colorOfBorder = 197;
                if( inventorySlot.MagicLevel ){
                    colorOfBorder = 181;
                }
                if( !inventorySlot.IsReqMet ){
                    colorOfBorder = 229;
                }
            }
            CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /*|| itemUnderCursor*/) | inventorySlot.mirrorImage << 1, colorOfBorder);
        }
    }
}

//----- (th2) -------------------------------------------------------------
void DrawVisualTradeTab( const DisplayObject& obj, bool state, const char* label )
{
	Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, VisualTradeTabsCEL, state ? 1 : 2, obj.Width);
	
	DrawText(obj.Left, obj.Down - 5, obj.Right, label, state ? C_0_White : C_7_Grey);
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_Draw()
{
    Surface_DrawCEL(VisualTradePanel.Left + Screen_LeftBorder, VisualTradePanel.Down + Screen_TopBorder, VisualTradePanelCEL, 1, VisualTradePanel.Width);
    DrawVisualTradeItems();
    
    const VisualTradeModeInfo& modeInfo = VisualTradeModeInfos[CurrentVisualTradeMode];
    if( modeInfo.pages.size() > VisualTrade_TabsAmount ) return; // assert
    if( modeInfo.actions.size() > VisualTrade_ActionButtonsAmount ) return; // assert
    
    for( int i = 0, ie = modeInfo.pages.size(); i != ie; ++i ){
        VisualTradePage pageType = modeInfo.pages[i];
        int index = VisualTrade_TabsStartIndex + i;
        DrawVisualTradeTab( VisualTradeButtons[index], pageType == CurrentVisualTradePage, VisualTradePagesNames[pageType] );
    }
    
    for( int i = 0, ie = modeInfo.actions.size(); i != ie; ++i ){
        VisualTradeAction actionType = modeInfo.actions[i];
        int icon = VisualTradeActionInfos[actionType].icon;
        int index = VisualTrade_ActionButtonsStartIndex + i;
        const DisplayObject& obj = VisualTradeButtons[index];
        if (icon == 2) {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_5_LOTL)) {
                VisualTradeButtons[index] = { 0,0,0,0,0,0 };
            }
            else {
                SetupDisplayObject(VisualTradeButtons[index], VisualTradePanel.Left + 12 + (33 + 6) * 2, VisualTradePanel.Down - 32 - 12, 33, 32);
            }
        }
        int pressed = VisualTradeButtonsStates[index] ? 2 : 1;
		Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, VisualTradeBtnsCEL, pressed + icon, obj.Width); // here icon is sprite num
	}

	{
		int icon = VisualTrade_ExitButtonIcon;
		int index = VisualTrade_ExitButtonStartIndex;
		const DisplayObject& obj = VisualTradeButtons[index];
		int pressed = VisualTradeButtonsStates[index] ? 2 : 1;
		Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, VisualTradeBtnsCEL, pressed + icon*2, obj.Width); // here icon is sprites pair num
	}
    
	DrawText(VisualTradeInfostring.Left, VisualTradeInfostring.Down, VisualTradeInfostring.Right, VisualTradeInfostringBuffer, C_0_White);
}

//----- (th2) -------------------------------------------------------------
int GetSlotMouseAbove_Trade()
{
    if( CurrentVisualTradePage == VTP_M1_None ){
        return -1;
    }
    auto& storage = VisualTradeStorages[CurrentVisualTradePage];
    
    int cellIndex = GetCellIndex(VisualTradeInventory, CursorX, CursorY);
	if( cellIndex == -1 ){
		return -1;
	}
	Item* item = nullptr;

	int itemNumber = abs(storage.usedFlags[cellIndex]);
	if( !itemNumber ){
		return -1;
	}
	int itemIndex = itemNumber - 1;
	int inventoryCellIndex = itemIndex + Stash_SlotIDStart;
	item = &storage.items[itemIndex];

	if( item->ItemCode == IC_M1_NONE ){
		return -1;
	}
	if( item->ItemCode == IC_11_GOLD ){
		char* currency = formatGoldAmount(item->amount);
		char infoPanelString[260];
		sprintf(infoPanelString, "%i gold %s", item->amount, currency);
		//InfoPanel_SetHeader(infoPanelString);
		return inventoryCellIndex;
	}
	int ItemInfoPanelColor = 0;
	if( item->MagicLevel == ML_1_MAGIC ){
		//InfoPanel_SetColor(C_1_Blue);
		ItemInfoPanelColor = C_1_Blue;
	}
	else if( item->MagicLevel == ML_2_UNIQUE ){
		char color = item->dropType & D_RARE ? C_8_Pink : C_3_Gold;
		//InfoPanel_SetColor(color);
		ItemInfoPanelColor = color;
	}
	return inventoryCellIndex;
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_MouseMove()
{
    const VisualTradeModeInfo& modeInfo = VisualTradeModeInfos[CurrentVisualTradeMode];
    if( modeInfo.actions.size() > VisualTrade_ActionButtonsAmount ) return; // assert

    int totalGold = Players[CurrentPlayerIndex].TotalGold;
    
    for( int i = 0, ie = modeInfo.actions.size(); i != ie; ++i ){
        int index = VisualTrade_ActionButtonsStartIndex + i;
        const DisplayObject& obj = VisualTradeButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            VisualTradeAction actionType = modeInfo.actions[i];
            Tooltip_Clear();
            Tooltip_SetOffsetTop();
            switch(actionType){
                case VTA_Repair:
                    Tooltip_AddLine("REPAIR ITEM");
                    Tooltip_AddLine("Hover over item to see price");
                    Tooltip_AddLine("Click on item to repair");
                    break;
                case VTA_RepairAll:
                {
                    Tooltip_AddLine("REPAIR ALL ITEMS");
                    int price = VisualTrade_GetRepairAllPrice();
                    if( price ){
                        sprintf(InfoPanelBuffer, "price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                    }else{
                        Tooltip_AddLine("Nothing to repair");
                    }
                }
                break;
                case VTA_Recharge:
                    Tooltip_AddLine("RECHARGE ITEM");
                    Tooltip_AddLine("Hover over item to see price");
                    Tooltip_AddLine("Click on item to recharge");
                    break;
                case VTA_RechargeAll:
                {
                    Tooltip_AddLine("RECHARGE ALL ITEMS");
                    int price = VisualTrade_GetRechargeAllPrice();
                    if( price ){
                        sprintf(InfoPanelBuffer, "price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                    }else{
                        Tooltip_AddLine("Nothing to recharge");
                    }
                }
                    break;
                case VTA_Identify:
                    Tooltip_AddLine("IDENTIFY ITEM");
                    Tooltip_AddLine("Hover over item to see price");
                    Tooltip_AddLine("Click on item to identify");
                    break;
                case VTA_IdentifyAll:
                {
                    Tooltip_AddLine("IDENTIFY ALL ITEMS");
                    int price = VisualTrade_GetIdentifyAllPrice();
                    if( price ){
                        sprintf(InfoPanelBuffer, "price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                    }else{
                        Tooltip_AddLine("Nothing to identify");
                    }
                }
                    break;
                case VTA_Restock:
                {
//                    Tooltip_AddLine("RESTOCK ITEMS");
//
//                    switch(CurrentVisualTradePage){
//                        case VTP_BlacksmithBasic:   Tooltip_AddLine("Restock blacksmith's basic items");    break;
//                        case VTP_BlacksmithPremium: Tooltip_AddLine("Restock blacksmith's premium items");  break;
//                        case VTP_WitchBasic:        Tooltip_AddLine("Restock witch's items");               break;
//                    }
//
                    int price = VisualTrade_GetRestockPrice();
                    sprintf(InfoPanelBuffer, "RESTOCK ITEMS for %i gold", price);
                    Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                    Tooltip_AddLine("Hotkey: R");
                }
                    break;
                case VTA_Upgrade:
                {
                    if (VisualTrade_GriswoldUpgradeAvailable()) {
                        uchar griswoldLevel = VisualTrade_GetGriswoldLevel();
                        if (griswoldLevel >= 25)
                        {
							sprintf(InfoPanelBuffer, "Item level upgrades: +%i (max)", griswoldLevel);
							Tooltip_AddLine(InfoPanelBuffer, C_3_Gold);
                        }
                        else
                        {
                            sprintf(InfoPanelBuffer, "Item level upgrades: +%i ", griswoldLevel);
                            Tooltip_AddLine(InfoPanelBuffer);
                            int price = VisualTrade_GetGriswoldUpgradePrice();
                            sprintf(InfoPanelBuffer, "UPGRADE for %i gold", price);
                            Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                        }
                    }
                    else 
                    {
						sprintf(InfoPanelBuffer, "Not available");
						Tooltip_AddLine(InfoPanelBuffer, C_2_Red);
                    }
                }
                break;
            }
            return;
        }
    }
    
    {
        int index = VisualTrade_ExitButtonStartIndex;
        const DisplayObject& obj = VisualTradeButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            Tooltip_Clear();
            Tooltip_SetOffsetTop();
            Tooltip_AddLine("Exit");
        }
    }
    
    MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();
	Cur.InventoryCellID = GetSlotMouseAbove_Trade();
	Tooltip_ShowItemInfo(Cur.InventoryCellID);
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_ButtonsMouseClick()
{
    const VisualTradeModeInfo& modeInfo = VisualTradeModeInfos[CurrentVisualTradeMode];
    if( modeInfo.pages.size() > VisualTrade_TabsAmount ) return; // assert
    if( modeInfo.actions.size() > VisualTrade_ActionButtonsAmount ) return; // assert

    for( int i = 0, ie = modeInfo.pages.size(); i != ie; ++i ){
        int index = VisualTrade_TabsStartIndex + i;
        const DisplayObject& obj = VisualTradeButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            VisualTradePage pageType = modeInfo.pages[i];
            if( pageType != CurrentVisualTradePage ){
                CurrentVisualTradePage = pageType;
                PlayGlobalSound(S_75_I_TITLEMOV);
            }
        }
    }
    
    for( int i = 0, ie = modeInfo.actions.size(); i != ie; ++i ){
        int index = VisualTrade_ActionButtonsStartIndex + i;
        const DisplayObject& obj = VisualTradeButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            VisualTradeButtonsStates[index] = true;
        }
    }
    
    {
        int index = VisualTrade_ExitButtonStartIndex;
        const DisplayObject& obj = VisualTradeButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            VisualTradeButtonsStates[index] = true;
        }
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_LButtonDown()
{
    if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		VisualTrade_ButtonsMouseClick();
        VisualTrade_TryToBuyItem(Cur.InventoryCellID, !IsShiftPressed);
	}else{
	    if( CurrentVisualTradeMode != VTM_Elder ){
	        VisualTrade_TryToSellItem();
        }
	}
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_ActionButtonsMouseClick()
{
    const VisualTradeModeInfo& modeInfo = VisualTradeModeInfos[CurrentVisualTradeMode];
    if( modeInfo.pages.size() > VisualTrade_TabsAmount ) return; // assert
    if( modeInfo.actions.size() > VisualTrade_ActionButtonsAmount ) return; // assert

    for( int i = 0, ie = modeInfo.actions.size(); i != ie; ++i ){
        int index = VisualTrade_ActionButtonsStartIndex + i;
        if( VisualTradeButtonsStates[index] ){
            VisualTradeButtonsStates[index] = false;
            const DisplayObject& obj = VisualTradeButtons[index];
            if( CursorIntoDisplayObject(obj) ){
                VisualTradeAction actionType = modeInfo.actions[i];
                switch(actionType){
                    case VTA_Repair:
                        SetCursorGraphics(CM_3_REPAIR);
                        Cur.IsTradeService = true;
                        break;
                    case VTA_RepairAll:
                        VisualTrade_RepairAll();
                        break;
                    case VTA_Recharge:
                        SetCursorGraphics(CM_4_STAFF_RECHARGE);
                        Cur.IsTradeService = true;
                        break;
                    case VTA_RechargeAll:
                        VisualTrade_RechargeAll();
                        break;
                    case VTA_Identify:
                        SetCursorGraphics(CM_2_IDENTIFY);
                        Cur.IsTradeService = true;
                        break;
                    case VTA_IdentifyAll:
                        VisualTrade_IdentifyAll();
                        break;
					case VTA_Restock:
						VisualTrade_Restock();
						break;
					case VTA_Upgrade:
                        VisualTrade_GriswoldUpgrade();
						break;
                }
                PlayGlobalSound(S_75_I_TITLEMOV);
            }
        }
    }
    
    if( VisualTradeButtonsStates[VisualTrade_ExitButtonStartIndex] ){
        VisualTradeButtonsStates[VisualTrade_ExitButtonStartIndex] = false;
        const DisplayObject& obj = VisualTradeButtons[VisualTrade_ExitButtonStartIndex];
        if( CursorIntoDisplayObject(obj) ){
            CloseInventoryPanel();
            VisualTrade_Close();
            PlayGlobalSound(S_75_I_TITLEMOV);
        }
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall VisualTrade_MouseClick()
{
    VisualTrade_ActionButtonsMouseClick();
}

//----- (th2) -------------------------------------------------------------
bool __fastcall VisualTrade_IsVisualTradeSlot( int slotId )
{
    return IsVisualTradePanelVisible && slotId >= Stash_SlotIDStart && slotId < Stash_SlotIDStart + VisualTrade_MaxSlot;
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall VisualTrade_GetItemBySlotId( int slotId )
{
    if( slotId < Stash_SlotIDStart ) slotId = Stash_SlotIDStart; // assert
    if( CurrentVisualTradePage == VTP_M1_None ) CurrentVisualTradePage = VTP_BlacksmithBasic; // assert
    return &VisualTradeStorages[CurrentVisualTradePage].items[ slotId - Stash_SlotIDStart ];
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetBuyPrice( const Item* item )
{
    int price = item->basePrice;
    if( item->MagicLevel != ML_0_USUAL && item->Identified ){
        price = item->Price;
    }
    if( CurrentVisualTradePage == VTP_GambleBasic ){
        price = static_cast<int>( price * 1.5 );
    }
    price = BuyPrice(price);
	LimitToMin(price, 1);
	return price;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetSellPrice( const Item* item )
{
    if( !VisualTrade_IsItemAllowedForSell(item) ){
        return 0;
    }

    int price = item->basePrice;
    if( item->MagicLevel != ML_0_USUAL && item->Identified ){
        price = item->Price;
    }
    price = SellPrice(price);
    int priceDivider = 10 + Players[CurrentPlayerIndex].CharLevel / 3;
    if (GameMode == GM_COLISEUM) { priceDivider = 2; }
    else if (GameMode == GM_CLASSIC) { priceDivider = 4; }
    price /= priceDivider;//PT_SellRatio;
    LimitToMin(price, 1);
    return price;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetRepairPrice( const Item* item )
{
    if( !ItemNeedRepair(item) ){
        return 0;
    }

    int price = item->basePrice;
    if( item->MagicLevel != ML_0_USUAL && item->Identified ){
        price = 30 * item->Price / 100;
    }
    
    int curDur = item->CurDurability;
    int baseDur = item->BaseDurability;
    int priceOfRepair = price * (25 * (baseDur - curDur) / baseDur) / 100;
    priceOfRepair /= 2;
	priceOfRepair = BuyPrice(priceOfRepair);
    LimitToMin(priceOfRepair, 1);
    return priceOfRepair;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetRechargePrice( const Item* item )
{
    int curCharges = item->CurCharges;
    int baseCharges = item->BaseCharges;
    if( baseCharges == 0 || item->ItemCode == IC_M1_NONE || curCharges == baseCharges ){
        return 0;
    }

    int price = item->basePrice;
    price += Spells[item->SpellIndex].PriceMultiplier;
    
    int priceOfRecharge = price * 100 * (baseCharges - curCharges) / baseCharges / 100 / 2;
	priceOfRecharge = BuyPrice(priceOfRecharge);
    LimitToMin(priceOfRecharge, 1);
    return priceOfRecharge;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetIdentificationPrice( const Item* item )
{
    if( !ItemNeedIdentify( item ) ){
        return 0;
    }

    int priceOfIdentification = PT_IdentifyPrice;
	priceOfIdentification = BuyPrice(priceOfIdentification);
    LimitToMin(priceOfIdentification, 1);
    return priceOfIdentification;
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_RepairItem( Item* item )
{
    int price = VisualTrade_GetRepairPrice(item);
    int gold = Players[CurrentPlayerIndex].TotalGold;
    if( price == 0 ){
        return false;
    }
    if( gold < price ){
        return false;
    }
    
    ClearGoldByInventoryAsPrice(price);
    PlayGlobalSound(91);
    item->CurDurability = item->BaseDurability;
    
	RecalcPlayer(CurrentPlayerIndex, 1);
    return true;
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_RechargeItem( Item* item )
{
    int price = VisualTrade_GetRechargePrice(item);
    int gold = Players[CurrentPlayerIndex].TotalGold;
    if( price == 0 ){
        return false;
    }
    if( gold < price ){
        return false;
    }
    
    ClearGoldByInventoryAsPrice(price);
    PlayGlobalSound(S_47_I_GOLD);
    item->CurCharges = item->BaseCharges;
    
	RecalcPlayer(CurrentPlayerIndex, 1);
    return true;
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_IdentifyItem( Item* item )
{
    int price = VisualTrade_GetIdentificationPrice(item);
    int gold = Players[CurrentPlayerIndex].TotalGold;
    if( price == 0 ){
        return false;
    }
    if( gold < price ){
        return false;
    }
    
    ClearGoldByInventoryAsPrice(price);
    PlayGlobalSound(S_47_I_GOLD);
    item->Identified = true;
    UpdateItem( *item );
	RecalcPlayer(CurrentPlayerIndex, 1);
    return true;
}

//----- (th2) -------------------------------------------------------------
Item* GetItemBySlotId( int slotId )
{
    Player& player = Players[CurrentPlayerIndex];
    if( slotId < 0 ){
        return nullptr;
	}else if( slotId < IS_50_47_Belt_Start ){
	    return &player.OnBodySlots[ slotId ]; // onBody slots and inventory
    }else if( slotId < IS_50_47_Belt_End ){
        return &player.BeltInventory[ slotId - IS_50_47_Belt_Start ];
    }else if( StashPanel_IsStashSlot( slotId ) ){
        return StashPanel_GetItemBySlotId( slotId );
    }else if( VisualTrade_IsVisualTradeSlot( slotId ) ){
        return VisualTrade_GetItemBySlotId( slotId  );
    }else{
        return nullptr;
    }
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_RepairItem( int slotId )
{
    Item* item = GetItemBySlotId( slotId );
    if( item ){
        return VisualTrade_RepairItem( item );
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_RechargeItem( int slotId )
{
    Item* item = GetItemBySlotId( slotId );
    if( item ){
        return VisualTrade_RechargeItem( item );
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_IdentifyItem( int slotId )
{
    Item* item = GetItemBySlotId( slotId );
    if( item ){
        return VisualTrade_IdentifyItem( item );
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_ForEachPlayerItem( std::function<void(Item* item)> cb )
{
    Player& player = Players[CurrentPlayerIndex];
    
    for( auto& item : player.OnBodySlots ){
        cb( &item );
    }
    
    for( auto& swapItem : WeaponSwapItems ){
        Item item;
        LoadItem( &swapItem, &item );
        cb( &item );
        SaveItem( &swapItem, &item );
    }
    
    for( int i = 0, ie = player.InvItemCount; i != ie; ++i ){
        cb( &player.InventorySlots[i] );
    }
    
    for( auto& item : player.BeltInventory ){
        cb( &item );
    }
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetRepairAllPrice()
{
    int price = 0;
    VisualTrade_ForEachPlayerItem( [&price]( Item* item )
                                   {
                                        price += VisualTrade_GetRepairPrice( item );
                                   } );
    return price;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetRechargeAllPrice()
{
    int price = 0;
    VisualTrade_ForEachPlayerItem( [&price]( Item* item )
                                   {
                                        price += VisualTrade_GetRechargePrice( item );
                                   } );
    return price;
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetIdentifyAllPrice()
{
    int price = 0;
    VisualTrade_ForEachPlayerItem( [&price]( Item* item )
                                   {
                                        price += VisualTrade_GetIdentificationPrice( item );
                                   } );
    return price;
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_RepairAll()
{
    VisualTrade_ForEachPlayerItem( []( Item* item )
                                   {
                                        VisualTrade_RepairItem( item );
                                   } );
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_RechargeAll()
{
    VisualTrade_ForEachPlayerItem( []( Item* item )
                                   {
                                        VisualTrade_RechargeItem( item );
                                   } );
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_IdentifyAll()
{
    VisualTrade_ForEachPlayerItem( []( Item* item )
                                   {
                                        VisualTrade_IdentifyItem( item );
                                   } );
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_AddGoldToInventory( int value )
{
    Player& player = Players[CurrentPlayerIndex];

    player.TotalGold += value;
    for( int i = 0; i < player.InvItemCount && value > 0; i++ ){
        Item& inventoryItem = player.InventorySlots[i];
        if( inventoryItem.ItemCode == IC_11_GOLD ){
            if( inventoryItem.amount != MaxGoldInCell ){
                int goldSum = value + inventoryItem.amount;
                if( goldSum > MaxGoldInCell ){
                    inventoryItem.amount = MaxGoldInCell;
                    value = goldSum - MaxGoldInCell;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                }else{
                    inventoryItem.amount = goldSum;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                    value = 0;
                }
            }
        }
    }

    if( value > 0 ){
        while( value > MaxGoldInCell ){
            PutOneGoldPileInInventory(MaxGoldInCell);
            value -= MaxGoldInCell;
        }
        PutOneGoldPileInInventory(value);
    }
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_IsItemAllowedForSell( const Item* item )
{
    int baseItemIndex = item->baseItemIndex;

    if( IsQuestItem( baseItemIndex ) ){
        return false;
    }
    if( IsObsoleteItem( baseItemIndex ) ){
        return false;
    }
    if( item->ItemCode == IC_11_GOLD ){
        return false;
    }
    
    return true;
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_SellItem( int slotId )
{
    const Player& player = Players[CurrentPlayerIndex];
    int price = 0;
    if( slotId == -1 ){
        price = VisualTrade_GetSellPrice(&player.ItemOnCursor);
        SetCursorGraphics(CM_1_NORMAL_HAND);
	}else if( slotId < IS_50_47_Belt_Start ){
	    price = VisualTrade_GetSellPrice(&player.OnBodySlots[ slotId ]);
	    ClearInventorySlot(CurrentPlayerIndex, slotId);
    }else if( slotId < IS_50_47_Belt_End ){
        price = VisualTrade_GetSellPrice(&player.BeltInventory[ slotId - IS_50_47_Belt_Start ]);
        ClearBeltSlot(CurrentPlayerIndex, slotId - IS_50_47_Belt_Start);
    }else{
        return;
    }
    
    PlayGlobalSound(S_47_I_GOLD);
    VisualTrade_AddGoldToInventory(price);
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_TryToSellItem( int slotId )
{
    const Player& player = Players[CurrentPlayerIndex];
    const Item* itemPtr = nullptr;
    if( slotId == -1 ){
        itemPtr = &player.ItemOnCursor;
	}else if( slotId < IS_50_47_Belt_Start ){
	    itemPtr = &player.OnBodySlots[ slotId ];
    }else if( slotId < IS_50_47_Belt_End ){
        itemPtr = &player.BeltInventory[ slotId - IS_50_47_Belt_Start ];
    }else{
        return false;
    }
    
    if( VisualTrade_IsItemAllowedForSell( itemPtr ) ){
        VisualTrade_SellItem(slotId);
        return true;
    }

    return false;
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_CheckFreeSpaceForItem( const Item* item )
{
    Player& player = Players[CurrentPlayerIndex];
    Item tempItem;
    auto prevCursorGraphics = Cur.GraphicsID;
    memcpy(&tempItem, &player.ItemOnCursor, sizeof(Item));
    
    memcpy(&player.ItemOnCursor, item, sizeof(Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventoryOrBelt(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    
    memcpy(&player.ItemOnCursor, &tempItem, sizeof(Item));
    SetCursorGraphics(prevCursorGraphics);
    
    return canPutInventoryItem;
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_BuyItem( int slotId, bool inCursor )
{
    const Item* itemPtr = VisualTrade_GetItemBySlotId( slotId );
    if( !itemPtr ){
        return;
    }
    
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    
    Item* items = nullptr;
    int itemsCount = 0;
    int forcedItemsCount = 0;
    
    VisualTrade_GetPageItemsInfo( CurrentVisualTradePage, &items, &itemsCount, &forcedItemsCount );
    
    // find bought item in shop items list
    int seed = itemPtr->seed;
    int itemIndex = -1;
    for( int i = 0; i < itemsCount; ++i ){
        const Item& shopItem = items[i];
        if( shopItem.ItemCode != IC_M1_NONE ){
            if( shopItem.seed == seed ){
                itemIndex = i;
                break;
            }
        }
    }
    if( itemIndex == -1 ) return; // assert
    
    int price = VisualTrade_GetBuyPrice(itemPtr);
    ClearGoldByInventoryAsPrice( price );
    PlayGlobalSound(S_47_I_GOLD);
    
    memcpy( &player.ItemOnCursor, itemPtr, sizeof(Item) );
    
    bool forced = itemIndex < forcedItemsCount;
    
    if( forced ){
        player.ItemOnCursor.MakeSeed();
    }
    
    if( inCursor ){
        SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
		FixItemCursor(-1);
    }else{
        AutoPutCursorToInventoryOrDrop();
    }
    
    auto removeItemFromShopList = [=]()
    {
        if(!forced){
            int i = itemIndex;
            if( i == itemsCount - 1 ){
                items[itemsCount - 1].ItemCode = IC_M1_NONE;
            }else{
                while( items[i + 1].ItemCode != IC_M1_NONE ){
                    memcpy(&items[i], &items[i + 1], sizeof(Item));
                    ++i;
                }
                items[i].ItemCode = IC_M1_NONE;
            }
        }
    };
    
    switch(CurrentVisualTradePage){
        case VTP_BlacksmithBasic:
            removeItemFromShopList();
            break;
        case VTP_BlacksmithPremium:
        {
            int lastValidItem = 0;
            for( int i = itemIndex, j = 0; i >= 0; ++j ){
                if( MagicItemsStoreArray[j].ItemCode != IC_M1_NONE ){
                    --i;
                    lastValidItem = j;
                }
            }
            MagicItemsStoreArray[lastValidItem].ItemCode = IC_M1_NONE;
            
            //MagicItemsStoreArray[itemIndex].ItemCode = IC_M1_NONE;
            if (!(player.gameChanger & BIT(GC_5_LOTL))) {
                --GriswoldMagicItemCount;
                SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon] * ms(GetTickCountTh()));
                MagicItemsGeneration(CurrentPlayerIndex);
            }
        }
            break;
        case VTP_WitchBasic:
            removeItemFromShopList();
            break;
        case VTP_GambleBasic:
            WirtItem.ItemCode = IC_M1_NONE;
            break;
        case VTP_HealerBasic:
            removeItemFromShopList();
            break;
    }
    
    VisualTrade_RefreshPage( CurrentVisualTradePage );
    RecalcPlayer(CurrentPlayerIndex, 1);
    SetupItemSizes(Cur.GraphicsID);
}

//----- (th2) -------------------------------------------------------------
bool VisualTrade_TryToBuyItem( int slotId, bool inCursor )
{
    const Item* itemPtr = nullptr;
    if( VisualTrade_IsVisualTradeSlot( slotId ) ){
        itemPtr = VisualTrade_GetItemBySlotId( slotId );
    }else{
        return false;
    }
    
    if( !inCursor && !VisualTrade_CheckFreeSpaceForItem(itemPtr) ){
        return false;
    }
    
    const Player& player = Players[CurrentPlayerIndex];
    int price = VisualTrade_GetBuyPrice(itemPtr);
    if( player.TotalGold >= price ){
        VisualTrade_BuyItem(slotId, inCursor);
        return true;
    }
    
    return false;
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_RecalcItemsReq()
{
    for( auto& storage : VisualTradeStorages ){
        for( int i = 0; i < storage.itemCount; ++i ){
            Item& item = storage.items[i];
            UpdateItem( item );
            RecalcBookReq( item, CurrentPlayerIndex );
        }
    }
}

//----- (th2) -------------------------------------------------------------
int VisualTrade_GetRestockPrice()
{
	Player& player = Players[CurrentPlayerIndex];
	int clvl = player.CharLevel;
	int restockPrice = 0;
	switch (CurrentVisualTradePage) {
	case VTP_BlacksmithBasic:   restockPrice = GetGriswoldBasicalRestockPrice(player); break;
	case VTP_BlacksmithPremium: restockPrice = GetGriswoldMagicalRestockPrice(player); break;
	case VTP_WitchBasic:        restockPrice = GetAdriaRestockPrice(player); break;
	}

	restockPrice = BuyPrice(restockPrice);
	return restockPrice;
}

//----- (th2) -------------------------------------------------------------
void VisualTrade_Restock()
{
    int price = VisualTrade_GetRestockPrice();
    if( !price ){
        return;
    }
    
    const Player& player = Players[CurrentPlayerIndex];
    if( player.TotalGold >= price ){
        ClearGoldByInventoryAsPrice( price );
        PlayGlobalSound(S_47_I_GOLD);
        
        SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon] * ms(GetTickCountTh()));
        switch(CurrentVisualTradePage){
            case VTP_BlacksmithBasic:
                for( auto& i : BasicalItemsArray ){
                    i.ItemCode = IC_M1_NONE;
                }
                BasicalItemsGeneration(ShopQlvl(player));
                break;
            case VTP_BlacksmithPremium:
                for( auto& i : MagicItemsStoreArray ){
                    i.ItemCode = IC_M1_NONE;
                }
                GriswoldMagicItemCount = 0;
                MagicItemsGeneration(CurrentPlayerIndex);
                break;
            case VTP_WitchBasic:
                for( auto& i : WitchAdriaItems ){
                    i.ItemCode = IC_M1_NONE;
                }
                WitchAdriaItemsGeneration(ShopQlvl(player));
                break;
        }
    }
}

//----- (th4) -------------------------------------------------------------
bool VisualTrade_GriswoldUpgradeAvailable()
{
	const Player& player = Players[CurrentPlayerIndex];

	bool isBarbarian = HasTrait(CurrentPlayerIndex, TraitId::Barbarian);
	bool isLOTL = player.gameChanger & BIT(GC_5_LOTL);
	bool isNightKin = player.gameChanger & BIT(GC_9_NIGHT_KIN);
    bool isClassic = GameMode == GM_CLASSIC;

	return(isBarbarian || isLOTL || isNightKin || isClassic) ? false: true;
}
void VisualTrade_GriswoldUpgrade()
{
    if (VisualTrade_GriswoldUpgradeAvailable()) 
    {
		uchar griswoldLevel = VisualTrade_GetGriswoldLevel();
        if (griswoldLevel < 25) {
            const Player& player = Players[CurrentPlayerIndex];
            int price = VisualTrade_GetGriswoldUpgradePrice();
            if (player.TotalGold >= price)
            {
                PlayGlobalSound(S_1532);
                Player& player = Players[CurrentPlayerIndex];
                player.griswoldLevel++;
                ClearGoldByInventoryAsPrice(price);
            }
			else
			{
				PlayGlobalSound(S_1515);
			}
        }
    }
}

uchar VisualTrade_GetGriswoldLevel()
{
	Player& player = Players[CurrentPlayerIndex];
	uchar griswoldLevel = player.griswoldLevel;
	return griswoldLevel;
}

int VisualTrade_GetGriswoldUpgradePrice()
{
	Player& player = Players[CurrentPlayerIndex];
    uchar griswoldLevel = player.griswoldLevel;
	int upgradePrice = (griswoldLevel + 1) * 10'000;

    upgradePrice = BuyPrice(upgradePrice);
	return upgradePrice;
}