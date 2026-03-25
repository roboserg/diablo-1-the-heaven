#include "stdafx.h"

bool IsStashPanelVisible = false;
bool Is_LMB_Pressed = false;

static constexpr int Stash_ChangePageButtonsAmount = 2;
static constexpr int Stash_TabButtonsAmount = 10;

static constexpr int Stash_TabCostRatio = 10'000; // 10k gold times number of tabs

static char Stash_ItemsSortSwitchState = 0;

int & StashCurrentTab = AdditionalStateData.StashCurrentTab; // save
int & StashTabsPurchased = AdditionalStateData.StashTabsPurchased; // save
int StashTabPrice = 0;
int StashLastAvailablePageIndex = 0;
int StashCurrentPageIndex = 0;
bool AvailableStashTabs[Stash_TotalTabs];
StashTab StashTabs[Stash_TotalTabs];

DisplayObject StashChangePageButtons[Stash_ChangePageButtonsAmount];
char StashChangePageButtonsStates[Stash_ChangePageButtonsAmount];
DisplayObject StashTabButtons[Stash_TabButtonsAmount];
char StashTabButtonsStates[Stash_TabButtonsAmount];

DisplayObject StashExitButton;
bool StashExitButtonState = false;
DisplayObject StashBuyButton;
bool StashBuyButtonState = false;
DisplayObject StashSortButton;
bool StashSortButtonState = false;

DisplayObject StashInventory;
DisplayObject VisualStashPanel;

const bool StashPageWrappingAllowed = false;

enum {
	BTN_ACTIVE   = 0,
	BTN_PRESSED  = 1,
	BTN_HOVER    = 2,
	BTN_DISABLED = 3
};

//----- (th2) -------------------------------------------------------------
void __fastcall RecalcAvailableTabs() {
	StashTabsPurchased = StashTabsPurchased < 1 ? 1 : StashTabsPurchased > Stash_TotalTabs ? Stash_TotalTabs : StashTabsPurchased;
	for (int i = 0; i < StashTabsPurchased; ++i) AvailableStashTabs[i] = true;
	for (int i = StashTabsPurchased; i < Stash_TotalTabs; ++i) AvailableStashTabs[i] = false;
}

//----- (th2) -------------------------------------------------------------
void __fastcall RecalcPageIndex() {
	StashLastAvailablePageIndex = (StashTabsPurchased - 1) / Stash_TabButtonsAmount;
}

//----- (th2) -------------------------------------------------------------
void __fastcall RecalcTabButtonsState() {
	StashCurrentPageIndex = StashCurrentTab / Stash_TabButtonsAmount;
	for (int i = 0; i < Stash_TabButtonsAmount; ++i) {
		StashTabButtonsStates[i] = AvailableStashTabs[StashCurrentPageIndex * Stash_TabButtonsAmount + i] ? BTN_ACTIVE : BTN_DISABLED;
	}
}

//----- (th2) -------------------------------------------------------------
int __fastcall StashTabCost()
{
	return BuyPrice(StashTabsPurchased * Stash_TabCostRatio);
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashReInit()
{
	RecalcAvailableTabs();
	RecalcPageIndex();
	RecalcTabButtonsState();

	StashTabPrice = StashTabCost();
}

//----- (th2) -------------------------------------------------------------
bool IsItemAllowedForStash( const Item* item )
{
    if( item->ItemCode == IC_11_GOLD ){
        return false;
    }
    if( IsItemForbiddenToStore( item->baseItemIndex ) ){
        return false;
    }
    return true;
}

//----- (th2) -------------------------------------------------------------
void Stash_RecalcTab( int index)
{
    StashCurrentTab = index;
    StashPanel_RecalcItemsReq();
	StashReInit();
}

//----- (th2) -------------------------------------------------------------
int CalcCurrentVisualStashPage()
{
    return StashCurrentTab / Stash_TabsOnPage;
}

//----- (th2) -------------------------------------------------------------
int CalcCurrentVisualStashTabOnPage()
{
    return StashCurrentTab % Stash_TabsOnPage;
}

//----- (th2) -------------------------------------------------------------
int CalcStashTabOnPageStartIndex()
{
    return CalcCurrentVisualStashPage() * Stash_TabsOnPage;
}

//----- (th2) -------------------------------------------------------------
int CalcStashTabOnPageIndex()
{
    return StashCurrentTab - CalcStashTabOnPageStartIndex() + 1;
}

//----- (th2) -------------------------------------------------------------
void VisualStashGotoPrevPage()
{
    int pageIndex = CalcCurrentVisualStashPage();
    if( (pageIndex > 0) || StashPageWrappingAllowed ){
        --pageIndex;
        Wrap(pageIndex, StashLastAvailablePageIndex + 1);
        Stash_RecalcTab(pageIndex * Stash_TabsOnPage + (Stash_TabsOnPage - 1));
		PlayGlobalSound(S_75_I_TITLEMOV);
    }
}

//----- (th2) -------------------------------------------------------------
void VisualStashGotoNextPage()
{
	int pageIndex = CalcCurrentVisualStashPage();
	if( ((pageIndex+1) < (StashLastAvailablePageIndex+1)) || StashPageWrappingAllowed ){
        ++pageIndex;
        Wrap(pageIndex, StashLastAvailablePageIndex + 1);
        Stash_RecalcTab(pageIndex * Stash_TabsOnPage);
		PlayGlobalSound(S_75_I_TITLEMOV);
    }
}

//----- (th2) -------------------------------------------------------------
void VisualStashGotoPrevTab()
{
    --StashCurrentTab;
    Wrap(StashCurrentTab, StashTabsPurchased);
    Stash_RecalcTab(StashCurrentTab);
}

//----- (th2) -------------------------------------------------------------
void VisualStashGotoNextTab()
{
    ++StashCurrentTab;
    Wrap(StashCurrentTab, StashTabsPurchased);
    Stash_RecalcTab(StashCurrentTab);
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_PrevTab()
{
	VisualStashGotoPrevTab();
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_NextTab()
{
	VisualStashGotoNextTab();
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void VisualStashGotoTabOnPage( int index )
{
    Stash_RecalcTab(CalcStashTabOnPageStartIndex() + index);
}

//----- (th2) -------------------------------------------------------------
int GetCellIndex(DisplayObject& object, int x, int y)
{
	if( object.Left < x && object.Right > x && object.Top < y && object.Down > y ){
		int widthInCells = (object.Width + 1) / (CELL_SIZE + 1); // fixed for inventory cell size
		int xInCells = (x - object.Left) / (CELL_SIZE + 1);
		int yInCells = (y - object.Top) / (CELL_SIZE + 1);
		int cellIndex = xInCells + yInCells * widthInCells;
		return cellIndex;
	}
	return -1;
}

//----- (th2) -------------------------------------------------------------
void GetSuccessionArray(uchar inventoryWidth, uchar inventoryHeight, uchar itemWidth, uchar itemHeight, vector<uchar>& successionArray)
{
	int successinWidth = inventoryWidth - itemWidth + 1;
	int successinHeight = inventoryHeight - itemHeight + 1;
	int successinCount = successinWidth * successinHeight;

	int sizeOfItem = ItemHeightCells + ItemWidthCells * InvCols;


	switch (sizeOfItem ){ // TODO: change switch to bound calculation for support any sizes
		case 11:
			for( int i = (successinHeight - 1); i >= 0; i-- ){// снизу вверх
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 12:
			for( int i = 0; i < successinHeight; i++ ){// снизу вверх
				// Хитроумная конструкция для того чтобы вначале ряды шли от нижнего до верхнего через 1
				// А потом опять таки снизу вверх оставшиеся
				char i2 = (successinHeight - 1) - i * 2;
				if( i2 < 0 ){
					i2 += successinHeight;
					if( !(successinHeight % 2) ){
						i2--;
					}
				}
				for( int j = (successinWidth - 1); j >= 0; j-- ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i2 * inventoryWidth + j);
				}
			}
			break;
		case 13:
			for( int i = 0; i < successinHeight; i++ ){// сверху вниз
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 22:
			for( int j = (successinWidth - 1); j >= 0; j -= 2 ){// справа налево через 1
				for( int i = 0; i < successinHeight; i += 1 ){// Снизу вверх. Заполняя вначале всю колонку а потом уже переходя на следующую
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			for( int i = (successinHeight - 1); i >= 0; i -= 2 ){// Сверху вниз.
				for( int j = (successinWidth % 2); j < successinWidth; j += 2 ){// слева направо через 1. Начиная с 0 при четной ширине и с 1 при нечётной. Заполняем вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			for( int i = (successinHeight - 2); i >= 0; i -= 2 ){// Сверху вниз оставшиеся ряды
				for( int j = 0; j < successinWidth; j++ ){// слева направо. Заполняем вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 21:
		case 23:
		case 24:
        default:
			for( int i = 0; i < successinHeight; i++ ){// сверху вниз
				for( int j = 0; j < successinWidth; j++ ){// Справа налево. Заполняя вначале весь ряд а потом уже переходя на следующий
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall DrawStashItems()
{
    const auto& stashTab = StashTabs[StashCurrentTab];
	extern bool InventoryBackgroundOldStyle;
	extern bool InventoryTurnOffRedColorItems;

	for( int row = 0; row < Stash_SlotsY; ++row ){
		for( int col = 0; col < Stash_SlotsX; ++col ){
			if (stashTab.usedFlagsAlt[row][col]) {
				if (InventoryBackgroundOldStyle) {
					ShadowItemSlot(StashInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), StashInventory.Down + Screen_TopBorder - (Stash_SlotsY - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
				} else {
					const char curItemIndex = abs(stashTab.usedFlagsAlt[row][col]) - 1;
					if (curItemIndex >= 0 && curItemIndex < Stash_MaxSlot) {
						const Item& curItem = stashTab.items[curItemIndex];
						ShadowItemSlot(StashInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), StashInventory.Down + Screen_TopBorder - (Stash_SlotsY - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE, GetItemBackgroundColorType(curItem));
					}
				}
			}
		}
	}

	if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START && Players[CurrentPlayerIndex].ItemOnCursor.ItemCode != IC_M1_NONE) {
		ChangeItemBacklight( StashInventory.Left + Screen_LeftBorder, StashInventory.Top + Screen_TopBorder, Stash_SlotsX, Stash_SlotsY, -1, -1, 2);
	}

	for( int row = 0; row < Stash_SlotsY; ++row ){
		for( int col = 0; col < Stash_SlotsX; ++col ){
            int invItemIndex = stashTab.usedFlagsAlt[row][col];
            if( invItemIndex <= 0 ){
                continue;
            }
            const Item& inventorySlot = stashTab.items[invItemIndex - 1];
            int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
            char itemWidthTable = ItemWidthTable[graphicValue];
            
            int x = StashInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1);
			int y = StashInventory.Down + Screen_TopBorder - (Stash_SlotsY - 1 - row) * (CELL_SIZE + 1);
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
void __fastcall StashPanel_Open()
{
	IsStashPanelVisible = true;
	IsINVPanelVisible = true;
	Stash_ItemsSortSwitchState = 0;
	Stash_RecalcTab(StashCurrentTab);
	CurrentDialogIndex = PD_0_None;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_Close()
{
	IsStashPanelVisible = false;
	IsINVPanelVisible = false;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_BuyTab()
{
	int totalGold = Players[CurrentPlayerIndex].TotalGold;
	if (totalGold >= StashTabPrice)	{
		ClearGoldByInventoryAsPrice(StashTabPrice);
		++StashTabsPurchased;
		Stash_RecalcTab(StashTabsPurchased - 1);
		PlayGlobalSound(S_1532);
	}
	else {
		PlayGlobalSound(S_1515);
	}
}

//----- (th2) -------------------------------------------------------------
short __fastcall Stash_GetFreeCells(char width, char height, char direction)
{
	StashTab& currentTab = StashTabs[StashCurrentTab];
	short(&cellsIn2D)[Stash_SlotsY][Stash_SlotsX] = currentTab.usedFlagsAlt;

	short index = -1;
	auto checkFreeCellsForItem = [&](short index)->bool {
		int x = index % Stash_SlotsX; // index % 10
		int y = index / Stash_SlotsX; // index / 10
		if ((x + width > Stash_SlotsX) || (y + height > Stash_SlotsY))
			return false;
		for (int i = 0, j; i < height; ++i)
			for (j = 0; j < width; ++j)
				if (cellsIn2D[y + i][x + j] != 0)
					return false;
		return true;
	};
	if (direction != 1) {
		for (int i = 0, j; i < Stash_SlotsY; ++i)
			for (j = 0; j < Stash_SlotsX; ++j)
				if (cellsIn2D[i][j] == 0) {
					index = i * 10 + j;
					if (checkFreeCellsForItem(index))
						return index;
				}
	}
	else {
		for (int i = 0, j; i < Stash_SlotsX; ++i)
			for (j = Stash_SlotsY - 1; j >= 0; --j)
				if (cellsIn2D[j][i] == 0) {
					index = j * 10 + i;
					if (checkFreeCellsForItem(index))
						return index;
				}
	}
	return 0;

}

//----- (th2) -------------------------------------------------------------
void Stash_SortItemsInTabBySize()
{
	StashTab& currentTab = StashTabs[StashCurrentTab];

	memset(currentTab.usedFlags, 0, sizeof(currentTab.usedFlags)); // clear tab

	const Item (&itemList)[Stash_MaxSlot] = currentTab.items;
	const int& itemCount = currentTab.itemCount;

	short (&cellsInLine)[Stash_MaxSlot] = currentTab.usedFlags;
	short (&cellsIn2D)[Stash_SlotsY][Stash_SlotsX] = currentTab.usedFlagsAlt;

	struct {
		char priority; // 1 - low, 9 - high
		char width;    // [1-2]
		char height;   // [1-4]
	} itemPriorityList[Stash_MaxSlot] = {0};
	
	for (int i = 0; i < itemCount; ++i) {
		const Item& item = itemList[i];
		const int itemGraphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;

		const char itemWidth = itemPriorityList[i].width = ItemWidthTable[itemGraphicValue] / CELL_SIZE;
		const char itemHeight = itemPriorityList[i].height = ItemHeightTable[itemGraphicValue] / CELL_SIZE;
		char& priority = itemPriorityList[i].priority;

		switch (itemWidth) {
		case 1:
			switch(itemHeight){
			case 1: priority = 1; break;
			case 2: priority = 3; break;
			case 3: priority = 4; break;
			case 4: priority = 6; break;
			}
			break;
		case 2:
			switch (itemHeight) {
			case 1: priority = 2; break;
			case 2: priority = 5; break;
			case 3: priority = 7; break;
			case 4: priority = 8; break;
			}
			break;
		default:
			if (itemHeight > 1)
				priority = 9;
			else
				priority = 1;
		}
	}

	auto putItemToCells = [&cellsIn2D, &itemPriorityList](int cellIndex, int itemIndex) {
		if (cellIndex < 0) return;
		int itemWidth = itemPriorityList[itemIndex].width;
		int itemHeight = itemPriorityList[itemIndex].height;
		int x = cellIndex % Stash_SlotsX; // index % 10
		int y = cellIndex / Stash_SlotsX; // index / 10
		for (int i = 0, j; i < itemHeight; ++i)
			for (j = 0; j < itemWidth; ++j)
				if(j == 0 && i+1 == itemHeight)
					cellsIn2D[y + i][x + j] = itemIndex+1;
				else
					cellsIn2D[y + i][x + j] = -(itemIndex+1);
	};

	for (char currentPriorityValue = 9; currentPriorityValue > 0; --currentPriorityValue) {
		for (int itemIndex = 0; itemIndex < itemCount; ++itemIndex) {
			if (itemPriorityList[itemIndex].priority == currentPriorityValue) {
				putItemToCells(Stash_GetFreeCells(itemPriorityList[itemIndex].width, itemPriorityList[itemIndex].height, currentPriorityValue), itemIndex);
			}
		}
	}

}

//----- (th2) -------------------------------------------------------------
void Stash_SortItemsInList()
{
	StashTab& currentTab = StashTabs[StashCurrentTab];
	Item* first = &currentTab.items[0];
	Item* last = &currentTab.items[currentTab.itemCount];

	if (currentTab.itemCount > 1){
		switch (Stash_ItemsSortSwitchState) {
		case 0: std::sort(first, last, [](const Item& a, const Item& b) { return a.quality > b.quality; }); break;
		case 1: std::sort(first, last, [](const Item& a, const Item& b) { return a.MagicLevel > b.MagicLevel; }); break;
		case 2: std::sort(first, last, [](const Item& a, const Item& b) { return a.Price > b.Price; }); break;
		case 3: std::sort(first, last, [](const Item& a, const Item& b) { return a.GraphicValue > b.GraphicValue; });
			break;
		}
		if (++Stash_ItemsSortSwitchState > 3)
			Stash_ItemsSortSwitchState = 0;
	}

	 Stash_SortItemsInTabBySize();
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_SortItems()
{
	Stash_SortItemsInList();
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall ClearVisualStash()
{
    for(auto & stashTab : StashTabs){
	    memset(&stashTab, 0, sizeof(StashTab));
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall WriteStashInfoToSave()
{
	CurSaveData = (char*)SaveBuffer_Stash;
	
	uint stashSignature = SaveSignature;
	PutNextHtonl2(stashSignature);
	PutNextHtonl(CurSaveVersion);
    StashPanel_Save();
    int actualSize = (uchar*)CurSaveData - SaveBuffer_Stash;
	int blockSize = CalcEncodeDstBytes(actualSize);
	EncodeFile( SaveBuffer_Stash, actualSize, blockSize, CryptKey );
	MpqAddFile( SaveFileStash(), SaveBuffer_Stash, blockSize );
	
    CurSaveData = nullptr;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall ReadStashInfoFromSave( HANDLE archiveHandle )
{
	ClearVisualStash();

    HANDLE heroHandle;
	if( !SFileOpenFileEx( archiveHandle, SaveFileStash(), 0, &heroHandle ) ){
		return false;
	}

	int isSuccess = false;
	uchar* heroData = 0;
	DWORD fileSize = SFileGetFileSize( heroHandle, 0 );
	if( fileSize ){
		heroData = (uchar*) AllocMem( fileSize );
		uint bytesReaded;
		if( SFileReadFile( heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0 ) ){
			int readSize = DecodeFile( heroData, fileSize, CryptKey );
			if( readSize > 0 ){
			    CurSaveData = (char*)heroData;
			    int stashSignature = GetNextHtonl2();
                if( stashSignature == SaveSignature ){
                    SaveVersion = GetNextHtonl();
                }else{
                    TerminateWithError("Invalid stash file");
                }
                StashPanel_Load();
			    CurSaveData = nullptr;
            }
		}
		FreeMem( heroData );
	}
	SFileCloseFile( heroHandle );
	return isSuccess;
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_Load()
{
	auto loadItem = LoadItem<ItemInfo64>; auto itemSize = sizeof(ItemInfo64); 
	if(      SaveVersion >= 22 ){ }
	else if( SaveVersion >= 18 ){ loadItem = (decltype(loadItem))LoadItem<ItemInfo42>; itemSize = sizeof(ItemInfo42); }
	else if( SaveVersion >= 17 ){ loadItem = (decltype(loadItem))LoadItem<ItemInfo32>; itemSize = sizeof(ItemInfo32); }
	else                        { loadItem = (decltype(loadItem))LoadItem<ItemInfo19>; itemSize = sizeof(ItemInfo19); }
	for( auto& stashTab : StashTabs ){
		stashTab.itemCount = GetNextHtonl();

		memcpy(stashTab.usedFlags, CurSaveData, sizeof(stashTab.usedFlags));
		CurSaveData += sizeof(stashTab.usedFlags);
		
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashTab.itemCount; ++currentStashItemIndex ){
			loadItem( (LastItemInfo*)CurSaveData, &stashTab.items[currentStashItemIndex], true );
			CurSaveData += itemSize;
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_Save()
{
	for( const auto& stashTab : StashTabs ){
		PutNextHtonl(stashTab.itemCount);

		memcpy(CurSaveData, stashTab.usedFlags, sizeof(stashTab.usedFlags));
		CurSaveData += sizeof(stashTab.usedFlags);
		
		for( int currentStashItemIndex = 0; currentStashItemIndex < stashTab.itemCount; ++currentStashItemIndex ){
		    const Item& item = stashTab.items[currentStashItemIndex];
		    SaveItem( (LastItemInfo*)CurSaveData, &item );
			CurSaveData += sizeof(LastItemInfo);
		}
	}
}

//----- (th2) -------------------------------------------------------------
bool __fastcall CursorIntoStashButton(const DisplayObject& button)
{
	if( CursorX >= button.Left - Screen_LeftBorder && CursorX <= button.Right - Screen_LeftBorder
	    && CursorY >= button.Top - Screen_TopBorder && CursorY <= button.Down - Screen_TopBorder ){
		return true;
	}else{
		return false;
	}
}

//----- (th2) -------------------------------------------------------------
int GetSlotMouseAbove_Stash()
{
	int cellIndex = GetCellIndex(StashInventory, CursorX, CursorY);
	if( cellIndex == -1 ){
		return -1;
	}
	Item* item = nullptr;

	int itemNumber = abs(StashTabs[StashCurrentTab].usedFlags[cellIndex]);
	if( !itemNumber ){
		return -1;
	}
	int itemIndex = itemNumber - 1;
	int inventoryCellIndex = itemIndex + Stash_SlotIDStart;
	item = &StashTabs[StashCurrentTab].items[itemIndex];

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
	return inventoryCellIndex;
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_MouseMove()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();

	Cur.InventoryCellID = GetSlotMouseAbove_Stash();
	Tooltip_ShowItemInfo(Cur.InventoryCellID);

	bool currsorOverButton = false;

	currsorOverButton = CursorIntoDisplayObject(StashBuyButton);
	StashBuyButtonState = Is_LMB_Pressed && currsorOverButton;
	if (StashTabsPurchased < Stash_TotalTabs && currsorOverButton) {
		int totalGold = Players[CurrentPlayerIndex].TotalGold;
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine("BUY ANOTHER STASH TAB");
		sprintf(InfoPanelBuffer, "price: %i", StashTabPrice);
		Tooltip_AddLine(InfoPanelBuffer, totalGold >= StashTabPrice ? C_0_White : C_2_Red);
	}

	currsorOverButton = CursorIntoDisplayObject(StashExitButton);
	StashExitButtonState = Is_LMB_Pressed && currsorOverButton;
	if (CursorIntoDisplayObject(StashExitButton)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine("EXIT");
	}

	currsorOverButton = CursorIntoDisplayObject(StashSortButton);
	StashSortButtonState = Is_LMB_Pressed && currsorOverButton;
	if (currsorOverButton) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine("AUTO-SORT ITEMS ON THIS TAB");
		constexpr char stash_sort_info[][16] = { "Quality Level", "Magic Level", "Price", "Image" };
		sprintf(InfoPanelBuffer, "by %s", stash_sort_info[Stash_ItemsSortSwitchState]);
		Tooltip_AddLine(InfoPanelBuffer, C_0_White);
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_MouseClick()
{
	for( int buttonIndex = 0; buttonIndex < Stash_TabButtonsAmount; buttonIndex++ ){
		if( StashTabButtonsStates[buttonIndex] != BTN_DISABLED ){
            if( CursorIntoDisplayObject(StashTabButtons[buttonIndex]) ){
                VisualStashGotoTabOnPage( buttonIndex );
				PlayGlobalSound(S_75_I_TITLEMOV);
            }
		}
	}

	if (CursorIntoDisplayObject(StashChangePageButtons[0])) VisualStashGotoPrevPage();
	if (CursorIntoDisplayObject(StashChangePageButtons[1])) VisualStashGotoNextPage();

	if (CursorIntoDisplayObject(StashExitButton)) StashPanel_Close();
	if (StashTabsPurchased < Stash_TotalTabs && CursorIntoDisplayObject(StashBuyButton)) StashPanel_BuyTab();
	if (CursorIntoDisplayObject(StashSortButton)) StashPanel_SortItems();

	Is_LMB_Pressed = false;
}

//----- (th2) -------------------------------------------------------------
void SetupStashPanel()
{
	SetupPanelRect( VisualStashPanel, LeftPanel, Stash_Width, Stash_Height );
	
	const int stepX = 4;
    const int stepY = 11;

    const int scrollButtonsX = VisualStashPanel.Left + stepX;
    const int scrollButtonsY = VisualStashPanel.Top + stepY;
    const int scrollButtonsXSize = 25;
    const int scrollButtonsYSize = 23;

	const int tabButtonsX = VisualStashPanel.Left + stepX;
	const int tabButtonsY = VisualStashPanel.Top + stepY;
	const int tabButtonsXSize = 25;
	const int tabButtonsYSize = 23;
    
	// Arrow buttons
	SetupDisplayObject(StashChangePageButtons[0], scrollButtonsX, scrollButtonsY, scrollButtonsXSize, scrollButtonsYSize);
	SetupDisplayObject(StashChangePageButtons[1], scrollButtonsX + (tabButtonsXSize + 1)*11, scrollButtonsY, scrollButtonsXSize, scrollButtonsYSize);

	// Tab buttons
	for( int i = 0; i < Stash_TabButtonsAmount; ++i ){
	    SetupDisplayObject(StashTabButtons[i], tabButtonsX + (tabButtonsXSize + 1)*i + scrollButtonsXSize + 1, tabButtonsY, tabButtonsXSize, tabButtonsYSize);
    }

	// Stash cels
	SetupDisplayObject(StashInventory, VisualStashPanel.Left + 15, VisualStashPanel.Top + 9 + stepY + tabButtonsYSize, 29 * Stash_SlotsX, 29 * Stash_SlotsY);

	const int buttonsStepX = 13;
	const int buttonsStepY = 422;

	// Sort button
	SetupDisplayObject(StashSortButton, VisualStashPanel.Left + buttonsStepX, VisualStashPanel.Top + buttonsStepY, 33, 32);
	// Buy button
	SetupDisplayObject(StashBuyButton, StashSortButton.Right + 10, VisualStashPanel.Top + buttonsStepY, 33, 32);
	// Exit button
	SetupDisplayObject(StashExitButton, StashBuyButton.Right + 185, VisualStashPanel.Top + buttonsStepY, 33, 32);
}

//----- (th2) -------------------------------------------------------------
void DrawButtons()
{
	int tabOnPageStartIndex = CalcStashTabOnPageStartIndex();
	// tabs
    for( int i = 0; i < Stash_TabButtonsAmount; ++i ){
		const auto& button = StashTabButtons[i];
		char& tabState = StashTabButtonsStates[i];

		int textOffset = 0;
		char textColor = 0;

		if (tabState == BTN_DISABLED) {
			textOffset = 2;
			textColor = C_7_Grey;
		}
		else if (StashCurrentTab % Stash_TabsOnPage == i) {
			tabState = BTN_PRESSED;
			textOffset = 0;
			textColor = C_0_White;
		}
		else if(CursorIntoDisplayObject(button)) {
			if (Is_LMB_Pressed) {
				tabState = BTN_PRESSED;
				textOffset = 0;
				textColor = C_0_White;
			}
			else {
				tabState = BTN_HOVER;
				textOffset = 1;
				textColor = C_0_White;
			}
		}
		else {
			tabState = BTN_ACTIVE;
			textOffset = 2;
			textColor = C_3_Gold;
		}

		Surface_DrawCEL(button.Left + Screen_LeftBorder, button.Down + Screen_TopBorder, VisualStashButtCEL, tabState + 1, button.Width);
		sprintf(InfoPanelBuffer, "%i", tabOnPageStartIndex + i + 1);
	    DrawText(button.Left + 1, button.Top + 18 + textOffset, button.Right+1, InfoPanelBuffer, textColor);
    }
	// arrows
	if (StashLastAvailablePageIndex) {
		char* arrowIconCel[2] = { VisualStashButtLeftCEL, VisualStashButtRightCEL };
		for (int i = 0; i < Stash_ChangePageButtonsAmount; ++i) {
			const auto& button = StashChangePageButtons[i];
			if (!i && StashCurrentPageIndex == 0)
				StashChangePageButtonsStates[0] = BTN_DISABLED;
			else if (i && StashCurrentPageIndex == StashLastAvailablePageIndex)
				StashChangePageButtonsStates[1] = BTN_DISABLED;
			else if (CursorIntoDisplayObject(button)) {
				if (Is_LMB_Pressed)
					StashChangePageButtonsStates[i] = BTN_PRESSED;
				else
					StashChangePageButtonsStates[i] = BTN_HOVER;
			}
			else StashChangePageButtonsStates[i] = BTN_ACTIVE;

			Surface_DrawCEL(button.Left + Screen_LeftBorder, button.Down + Screen_TopBorder, arrowIconCel[i], StashChangePageButtonsStates[i] + 1, button.Width);
		}
	}

	Surface_DrawCEL(StashSortButton.Left + Screen_LeftBorder, StashSortButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, StashSortButtonState ? 28 : 27, StashSortButton.Width);
	if(StashTabsPurchased < Stash_TotalTabs)
		Surface_DrawCEL(StashBuyButton.Left + Screen_LeftBorder, StashBuyButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, StashBuyButtonState ? 12 : 11, StashBuyButton.Width);
	Surface_DrawCEL(StashExitButton.Left + Screen_LeftBorder, StashExitButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, StashExitButtonState ? 20 : 19, StashExitButton.Width);
}


//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_Draw()
{
	Surface_DrawCEL(VisualStashPanel.Left + Screen_LeftBorder, VisualStashPanel.Down + Screen_TopBorder, VisualStashPanelCEL, 1, VisualStashPanel.Width);// Тут рисуется сама картинка инвентаря
	DrawStashItems(); // Тут рисуются вещи инвентаря
	DrawButtons(); // Тут рисуются кнопочки
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_RecalcItemsReq()
{
	StashTab& stashTab = StashTabs[StashCurrentTab];
	for( int currentStashItemIndex = 0; currentStashItemIndex < stashTab.itemCount; ++currentStashItemIndex ){
	    Item& item = stashTab.items[currentStashItemIndex];
	    UpdateItem( item );
		RecalcBookReq( item, CurrentPlayerIndex );
	}
}

//----- (th2) -------------------------------------------------------------
bool __fastcall ItemPanel_PutItemInStorage( int playerIndex, int aCellID, int aWidth, int aHeight, int isNotRoomTest)
{
	StashTab& stash = StashTabs[StashCurrentTab];

	int countFreeInvSlots = 0;
	for(short usedFlag : stash.usedFlags){
		if( !usedFlag ){
			countFreeInvSlots++;
		}
	}

	if( countFreeInvSlots < aWidth * aHeight ){
		return false;
	}

	Player& player = Players[playerIndex];
	bool haveRoomForItem = true;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = aCellID / Stash_SlotsX;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; haveRoomForItem; i++ ){// Вычисление есть ли место для вещи
		if( currentInvYPosition >= Stash_SlotsY ){
			haveRoomForItem = false;
			return haveRoomForItem;// На нет и суда нет
		}
		currentInvXPosition = aCellID % Stash_SlotsX;
		if( currentInvXPosition < 0 ){// Если отрицательный - обнуляем
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( !haveRoomForItem ){// То есть по сути чтобы цикл вернул 1 нужно чтобы клетки инвентаря от текущей по х размер ширины были пусты.
				return haveRoomForItem;// На нет и суда нет
			}
			int inventorySlotsIndex = abs(stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX]);
			if( inventorySlotsIndex ){
				int graphicsIndex = stash.items[inventorySlotsIndex - 1].GraphicValue + CM_12_ITEMS_PICS_START;
				haveRoomForItem = false;
			}else{
				haveRoomForItem = true;
			}
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
	stash.items[stash.itemCount] = player.ItemOnCursor;
	stash.itemCount++;
	currentInvYPosition = aCellID / Stash_SlotsX;

	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; i < aHeight; i++ ){
		currentInvXPosition = aCellID % Stash_SlotsX;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( j || i != aHeight - 1 ){// Обозначаем клетки как занятые вещью
				stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] = -stash.itemCount; // Если одна из остальных клеток
			}else{
				stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] = stash.itemCount; // Если клетка с картинкой
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}
	return haveRoomForItem;
}

//----- (th2) -------------------------------------------------------------
void __fastcall Stash_PutCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];
	int currentY;

	int itemForReplacing;
	Item tempItem {};

	SetupItemSizes(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	int yPos = mY + (Cur.ItemHeight / 2);
	int xPos = mX + (Cur.ItemWidth / 2);
	int newCursorGraphics = 0;

	//displayObject BeltSize; Нужно использовать эти переменные, чтобы просто и очевидно сделать проверку
	//displayObject InventorySlotsSize;
	//displayObject OnBodySlots[IS_10_7_Inventory];

	bool isHasGotToTheSlot = false;
	//int cellID;

	if( !(ItemWidthCells & 1) ){ // even
		xPos -= CELL_SIZE / 2; // Отступ влево на полклетки для вещей с чётной шириной. Чтобы ставились на клетки под вещьюа не правее
	}
	if( !(ItemHeightCells & 1) ){ // even
		yPos -= CELL_SIZE / 2; // Отступ вверх для вещей с чётной высотой. Для тех же целей
	}

	int cellIndex = GetCellIndex(StashInventory, xPos, yPos);

	if( cellIndex == -1 ){
		return;
	}

	StashTab& stash = StashTabs[StashCurrentTab];
	newCursorGraphics = 1;
	itemForReplacing = 0;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = cellIndex / Stash_SlotsX - (ItemHeightCells - 1) / 2;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}
	for( int i = 0; i < ItemHeightCells; i++ ){
		if( currentInvYPosition >= Stash_SlotsY ){
			return;
		}
		currentInvXPosition = cellIndex % Stash_SlotsX - (ItemWidthCells - 1) / 2;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < ItemWidthCells; j++ ){
			if( currentInvXPosition >= Stash_SlotsX ){
				return;
			}
			bool haveRoomForItem = !stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX];
			if( !haveRoomForItem ){ // Запоминаем какая вещь нам попалась под нашу вещь, если их несколько - выходим
				if( !itemForReplacing ){
					itemForReplacing = stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX];
					if( itemForReplacing < 0 ){
						itemForReplacing = -itemForReplacing;
					}
				}
				else if( itemForReplacing != stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] && itemForReplacing != -stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] ){
					return;
				}
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}

	if( playerIndex == CurrentPlayerIndex ){ // Если кладём текущему игроку - играем соответствующий звук
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[player.ItemOnCursor.GraphicValue]]);
	}

	if( !itemForReplacing ){
		//если нечем заменять
		currentY = cellIndex / Stash_SlotsX - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % Stash_SlotsX + Stash_SlotsX * currentY;
        ItemPanel_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 );
	}
	else if( itemForReplacing ){
	 //если есть чем заменять
		tempItem =  stash.items[itemForReplacing - 1]; // Сохранили в темп заменяемую вещь
		ClearStashSlot(StashCurrentTab, itemForReplacing - 1); // Удаляемая вещь становится под номером равным количеству вещей
		currentY = cellIndex / Stash_SlotsX - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % Stash_SlotsX + Stash_SlotsX * currentY;
        ItemPanel_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 ); // Сюда вещь копируется с курсора
		newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&tempItem, &player.ItemOnCursor); // Поменяли вещи
	}

	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
		if( newCursorGraphics == 1 ){
		    // PlayGlobalSound( S_56_I_INVGRAB );
			FixItemCursor(1);
		}
		SetCursorGraphics(newCursorGraphics);
	}
}

//----- (th2) -------------------------------------------------------------
bool __fastcall Stash_MoveItemToInventory(int itemIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	if( (uint) itemIndex >= Stash_MaxSlot ) return false; // TODO: change to >= StashTabs[StashCurrentTab].itemCount

	player.ItemOnCursor = StashTabs[StashCurrentTab].items[itemIndex];
	PlayGlobalSound( S_56_I_INVGRAB );
	if( AutoPutCursorToInventoryOrDrop( false ) ){
		ClearStashSlot(StashCurrentTab, itemIndex);
	}else{
		SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
		ClearStashSlot(StashCurrentTab, itemIndex);
	}
	return true;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall AddItemToStashFromCursor()
{
	Player& player = Players[CurrentPlayerIndex];
	if( IsItemAllowedForStash( &Players[CurrentPlayerIndex].ItemOnCursor ) ){
	    return AutoPutCursorItemToStash(CurrentPlayerIndex);
    }
    return false;
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashTakeCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];

	if( player.CurAction > PCA_3_WALK_HORISONTAL ){// Если мы не ходим и не стоим (т.е. атакуем, блокируем и т.п.)
		return;
	}

	IsGoldSplitPanelVisible = false;

	int cellIndex = GetCellIndex(StashInventory, CursorX, CursorY);
	if( cellIndex == -1 ){
		return;
	}
	
	StashTab& stash = StashTabs[StashCurrentTab];
	
	if( IsShiftPressed ){
	    int itemIndex = stash.usedFlags[cellIndex];
	    if( itemIndex != 0 ){
	        
            Stash_MoveItemToInventory( abs(itemIndex) - 1 );
            return;
        }
    }
    
	// тут чистая копия саба клеар инвентори слот. Только до рекалк плеер
	int takeItemIndex;

	player.ItemOnCursor.ItemCode = IC_M1_NONE;

	if( stash.usedFlags[cellIndex] ){
		takeItemIndex = abs(stash.usedFlags[cellIndex]);
		for(auto& invUsed : stash.usedFlags){
				if( abs(invUsed) == takeItemIndex ){
                invUsed = 0;
			}
		}
		int invItemIndex = takeItemIndex - 1;
		memcpy(&player.ItemOnCursor, &stash.items[takeItemIndex - 1], sizeof(Item)); // из инвенторя в курсор
		//NeedDrawBelt = 1;                                                                                               // для глюка с дублем
		stash.itemCount--;
		if( stash.itemCount > 0 && stash.itemCount != (takeItemIndex - 1) ){
			memcpy(&stash.items[takeItemIndex - 1], &stash.items[stash.itemCount], sizeof(Item));
			for(auto& invUsed : stash.usedFlags){
                	if( invUsed == (stash.itemCount + 1) ){
                    invUsed = invItemIndex + 1;
				}
				if( invUsed == -(stash.itemCount + 1) ){
                    invUsed = -(invItemIndex + 1);
				}
			}
		}
	}

	if( player.ItemOnCursor.ItemCode != IC_M1_NONE ){ //  если что то подцепили на курсор
		if( player.ItemOnCursor.ItemCode == IC_11_GOLD ){
			player.TotalGold = CalcTotalGold(playerIndex);
		}
		RecalcPlayer(playerIndex, 1);
		CheckItemStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound(56);
			SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
			FixItemCursor(-1);
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall ClearStashSlot(int pageIndex, int itemIndex)
{
	StashTab& stash = StashTabs[pageIndex];
    if( stash.itemCount <= 0 ) return;

	for(auto& usedFlag : stash.usedFlags){
		if( abs(usedFlag) == itemIndex + 1 ){
			usedFlag = 0;
		}
	}
	--stash.itemCount;
	if( stash.itemCount > 0 && stash.itemCount != itemIndex ){// Если вещи есть и удаляемая вещь не последняя
		stash.items[itemIndex] = stash.items[stash.itemCount]; // перенос крайнего в массиве предмета на индекс удалённого

		for(auto& invUsed : stash.usedFlags){
			if( invUsed == stash.itemCount + 1 ){
                invUsed = itemIndex + 1;
			}
			if( invUsed == -(stash.itemCount + 1) ){
                invUsed = -(itemIndex + 1);
			}
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Stash_LButtonDown()
{
	if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		StashTakeCursorItem(CurrentPlayerIndex, CursorX, CursorY);
		
	}else{
		if( IsItemAllowedForStash( &Players[CurrentPlayerIndex].ItemOnCursor ) ){
			Stash_PutCursorItem(CurrentPlayerIndex, CursorX, CursorY);
		}
	}
	Is_LMB_Pressed = true;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall AutoPutCursorItemToStash(int playerIndex)
{
	SetupItemSizes(Players[CurrentPlayerIndex].ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	vector<uchar> successionArray; // Сюда копируется приоритет места инвентаря для каждой позиции.
	successionArray.reserve(Stash_MaxSlot * 2);

	GetSuccessionArray(Stash_SlotsX, Stash_SlotsY, ItemWidthCells, ItemHeightCells, successionArray);

	for( auto cellId: successionArray ){
		if( ItemPanel_PutItemInStorage(CurrentPlayerIndex, cellId, ItemWidthCells, ItemHeightCells, 1) ) return true;
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall StashPanel_IsStashSlot( int slotId )
{
    return IsStashPanelVisible && slotId >= Stash_SlotIDStart && slotId < Stash_SlotIDStart + Stash_MaxSlot;
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall StashPanel_GetItemBySlotId( int slotId )
{
    if( slotId < Stash_SlotIDStart ) slotId = Stash_SlotIDStart; // assert
    return &StashTabs[StashCurrentTab].items[ slotId - Stash_SlotIDStart ];
}
