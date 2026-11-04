#include "stdafx.h"
#include "ItemOverlay.h"

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
			for( int i = (successinHeight - 1); i >= 0; i-- ){// (rt) bottom to top
				for( int j = 0; j < successinWidth; j++ ){// (rt) Right to left. Fill the whole row first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 12:
			for( int i = 0; i < successinHeight; i++ ){// (rt) bottom to top
				// (rt) An arrangement that first takes the rows from bottom to top, skipping every other one
				// (rt) And then the remaining ones, again from bottom to top
				char i2 = (successinHeight - 1) - i * 2;
				if( i2 < 0 ){
					i2 += successinHeight;
					if( !(successinHeight % 2) ){
						i2--;
					}
				}
				for( int j = (successinWidth - 1); j >= 0; j-- ){// (rt) Right to left. Fill the whole row first, then move to the next one
					successionArray.push_back(i2 * inventoryWidth + j);
				}
			}
			break;
		case 13:
			for( int i = 0; i < successinHeight; i++ ){// (rt) top to bottom
				for( int j = 0; j < successinWidth; j++ ){// (rt) Right to left. Fill the whole row first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 22:
			for( int j = (successinWidth - 1); j >= 0; j -= 2 ){// (rt) right to left, every other one
				for( int i = 0; i < successinHeight; i += 1 ){// (rt) Bottom to top. Fill the whole column first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			for( int i = (successinHeight - 1); i >= 0; i -= 2 ){// (rt) Top to bottom.
				for( int j = (successinWidth % 2); j < successinWidth; j += 2 ){// (rt) left to right, every other one. Starting from 0 for an even width and from 1 for an odd one. Fill the whole row first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			for( int i = (successinHeight - 2); i >= 0; i -= 2 ){// (rt) The remaining rows, top to bottom
				for( int j = 0; j < successinWidth; j++ ){// (rt) left to right. Fill the whole row first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
		case 21:
		case 23:
		case 24:
        default:
			for( int i = 0; i < successinHeight; i++ ){// (rt) top to bottom
				for( int j = 0; j < successinWidth; j++ ){// (rt) Right to left. Fill the whole row first, then move to the next one
					successionArray.push_back(i * inventoryWidth + j);
				}
			}
			break;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall DrawStashItems()
{
    auto& stashTab = StashTabs[StashCurrentTab];	// (th4) not const: IsReqMet is refreshed below
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
            Item& inventorySlot = stashTab.items[invItemIndex - 1];
            // (th4) Same staleness as the shop: RecalcItemsReqMet() walks inventory, belt
            // and talismans only, so stashed items keep the flag they had when they were
            // put away and stay red after a stat-up. Re-test here.
            inventorySlot.IsReqMet = CheckItemReq( CurrentPlayerIndex, &inventorySlot );
            int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
            char itemWidthTable = ItemWidthTable[graphicValue];
            
            int x = StashInventory.Left + Screen_LeftBorder + col * (CELL_SIZE + 1);
			int y = StashInventory.Down + Screen_TopBorder - (Stash_SlotsY - 1 - row) * (CELL_SIZE + 1);
			if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
			else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;
			
            const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
            char* itemsCELArray = GetItemSprites(graphicValue);
			bool itemUnderCursor = Cur.InventoryCellID == invItemIndex - 1 + Stash_SlotIDStart;
			// (rt) Pick the outline colour
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
			ItemCEL_Draw( overlayGfx, x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /*|| itemUnderCursor*/) | inventorySlot.mirrorImage << 1, colorOfBorder);
			ItemOverlay_Queue(overlayGfx, x, y, itemWidthTable, ItemHeightTable[overlayGfx], inventorySlot.mirrorImage != 0, colorOfBorder, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /*|| itemUnderCursor*/) | inventorySlot.mirrorImage << 1);	// (th4)
        }
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall StashPanel_Open()
{
	IsChatHistoryVisible = false;
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
		sprintf(infoPanelString, L("%i gold %s"), item->amount, currency);
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
		Tooltip_AddLine(L("BUY ANOTHER STASH TAB"));
		sprintf(InfoPanelBuffer, L("price: %i"), StashTabPrice);
		Tooltip_AddLine(InfoPanelBuffer, totalGold >= StashTabPrice ? C_0_White : C_2_Red);
	}

	currsorOverButton = CursorIntoDisplayObject(StashExitButton);
	StashExitButtonState = Is_LMB_Pressed && currsorOverButton;
	if (CursorIntoDisplayObject(StashExitButton)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine(L("EXIT"));
	}

	currsorOverButton = CursorIntoDisplayObject(StashSortButton);
	StashSortButtonState = Is_LMB_Pressed && currsorOverButton;
	if (currsorOverButton) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine(L("AUTO-SORT ITEMS ON THIS TAB"));
		const char* stash_sort_info[] = { L("Quality Level"), L("Magic Level"), L("Price"), L("Image") };
		sprintf(InfoPanelBuffer, L("by %s"), stash_sort_info[Stash_ItemsSortSwitchState]);
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
	Surface_DrawCEL(VisualStashPanel.Left + Screen_LeftBorder, VisualStashPanel.Down + Screen_TopBorder, VisualStashPanelCEL, 1, VisualStashPanel.Width);// (rt) The inventory picture itself is drawn here
	DrawStashItems(); // (rt) The inventory items are drawn here
	DrawButtons(); // (rt) The buttons are drawn here
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

	for( int i = 0; haveRoomForItem; i++ ){// (rt) Calculating whether there is room for the item
		if( currentInvYPosition >= Stash_SlotsY ){
			haveRoomForItem = false;
			return haveRoomForItem;// (rt) Nothing to do in that case
		}
		currentInvXPosition = aCellID % Stash_SlotsX;
		if( currentInvXPosition < 0 ){// (rt) If negative, set to zero
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// (rt) go through the cells aWidth times
			if( !haveRoomForItem ){// (rt) So for the loop to return 1, the inventory cells from the current one across the item's width must be empty.
				return haveRoomForItem;// (rt) Nothing to do in that case
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
		if( i == aHeight - 1 ){//(rt) if there is room for the item
			break; // (rt) On exit haveRoomForItem will be 1; the next block decides what to do with it
		}
	}
	if( !haveRoomForItem || !isNotRoomTest ){
		return haveRoomForItem;
	}
	// (rt) Placing the item itself. haveRoomForItem will most likely be 1, since otherwise the previous loop exits immediately
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
		for( int j = 0; j < aWidth; j++ ){// (rt) go through the cells aWidth times
			if( j || i != aHeight - 1 ){// (rt) Mark the cells as occupied by the item
				stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] = -stash.itemCount; // (rt) If it is one of the other cells
			}else{
				stash.usedFlags[currentInvXPosition + currentInvYPosition * Stash_SlotsX] = stash.itemCount; // (rt) If it is the cell with the picture
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
	//int currentY;

	int itemForReplacing;
	Item tempItem {};

	SetupItemSizes(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	int yPos = mY + (Cur.ItemHeight / 2);
	int xPos = mX + (Cur.ItemWidth / 2);
	int newCursorGraphics = 0;

	//(rt) displayObject BeltSize; These variables should be used so the check is simple and clear
	//displayObject InventorySlotsSize;
	//displayObject OnBodySlots[IS_10_7_Inventory];

	bool isHasGotToTheSlot = false;
	//int cellID;

	if( !(ItemWidthCells & 1) ){ // even
		xPos -= CELL_SIZE / 2; // (rt) Shift left by half a cell for items of even width, so they land on the cells under the item and not to the right
	}
	if( !(ItemHeightCells & 1) ){ // even
		yPos -= CELL_SIZE / 2; // (rt) Shift up for items of even height, for the same purpose
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
			if( !haveRoomForItem ){ // (rt) Remember which item is under ours; if there are several, exit
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

	if( playerIndex == CurrentPlayerIndex ){ // (rt) If placing for the current player, play the corresponding sound
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[player.ItemOnCursor.GraphicValue]]);
	}

/*	if( !itemForReplacing ){
		(rt) //if there is nothing to replace with
		currentY = cellIndex / Stash_SlotsX - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % Stash_SlotsX + Stash_SlotsX * currentY;
        ItemPanel_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 );
	}
	else if( itemForReplacing ){
	 (rt) //if there is something to replace with
		(rt) tempItem =  stash.items[itemForReplacing - 1]; // Saved the item being replaced into temp
		(rt) ClearStashSlot(StashCurrentTab, itemForReplacing - 1); // The removed item gets the number equal to the item count
		currentY = cellIndex / Stash_SlotsX - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % Stash_SlotsX + Stash_SlotsX * currentY;
        (rt) ItemPanel_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 ); // The item is copied here from the cursor
		(rt) newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&tempItem, &player.ItemOnCursor); // Swapped the items
	}*/

	// Fixed item placement for 3+ cell wide items
	//----------NEW SOLUTION TO PREVENT 3X2 OR 4X2 OR 4X7 ITEM ERASE WHEN PUTTING IN STASH (PREVIOUS SOLUTION IS COMMENTED OUT ABOVE)---------------
	if (!itemForReplacing) {
		int currentX = cellIndex % Stash_SlotsX - (ItemWidthCells - 1) / 2;
		if (currentX < 0) currentX = 0;
		if (currentX + ItemWidthCells > Stash_SlotsX) currentX = Stash_SlotsX - ItemWidthCells;
		int currentY = cellIndex / Stash_SlotsX - (ItemHeightCells - 1) / 2;
		if (currentY < 0) currentY = 0;
		if (currentY + ItemHeightCells > Stash_SlotsY) currentY = Stash_SlotsY - ItemHeightCells;
		cellIndex = currentX + Stash_SlotsX * currentY;
		if (ItemPanel_PutItemInStorage(playerIndex, cellIndex, ItemWidthCells, ItemHeightCells, 1)) {
			// (th4) Same stale-cursor-copy dupe guard as in AutoPutCursorItemToStash().
			player.ItemOnCursor.ItemCode = IC_M1_NONE;
		} else {
			// (th4) Put failed: keep the item visibly on the cursor instead of hiding a
			// live item behind a hand cursor (the "vanished item" state).
			newCursorGraphics = player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START;
		}
	}
	else if (itemForReplacing) {
		tempItem = stash.items[itemForReplacing - 1];
		ClearStashSlot(StashCurrentTab, itemForReplacing - 1);
		int currentX = cellIndex % Stash_SlotsX - (ItemWidthCells - 1) / 2;
		if (currentX < 0) currentX = 0;
		if (currentX + ItemWidthCells > Stash_SlotsX) currentX = Stash_SlotsX - ItemWidthCells;
		int currentY = cellIndex / Stash_SlotsX - (ItemHeightCells - 1) / 2;
		if (currentY < 0) currentY = 0;
		if (currentY + ItemHeightCells > Stash_SlotsY) currentY = Stash_SlotsY - ItemHeightCells;
		cellIndex = currentX + Stash_SlotsX * currentY;
		ItemPanel_PutItemInStorage(playerIndex, cellIndex, ItemWidthCells, ItemHeightCells, 1);
		newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&tempItem, &player.ItemOnCursor);
	}
	//------------------------------- END OF THIS SOLUTION -------------------------------------------

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

	if( player.CurAction > PCA_3_WALK_HORISONTAL ){// (rt) If we are neither walking nor standing (i.e. attacking, blocking, etc.)
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
    
	// (rt) an exact copy of the ClearInventorySlot sub here, only up to RecalcPlayer
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
		memcpy(&player.ItemOnCursor, &stash.items[takeItemIndex - 1], sizeof(Item)); // (rt) from the inventory to the cursor
		//(rt) NeedDrawBelt = 1;                                                                                               // for the duplication glitch
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

	if( player.ItemOnCursor.ItemCode != IC_M1_NONE ){ //  (rt) if something was picked up onto the cursor
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
	if( stash.itemCount > 0 && stash.itemCount != itemIndex ){// (rt) If there are items and the removed item isn't the last one
		stash.items[itemIndex] = stash.items[stash.itemCount]; // (rt) move the last item in the array to the removed item's index

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
	vector<uchar> successionArray; // (rt) The inventory placement priority for each position is copied here.
	successionArray.reserve(Stash_MaxSlot * 2);

	GetSuccessionArray(Stash_SlotsX, Stash_SlotsY, ItemWidthCells, ItemHeightCells, successionArray);

	for( auto cellId: successionArray ){
		if( ItemPanel_PutItemInStorage(CurrentPlayerIndex, cellId, ItemWidthCells, ItemHeightCells, 1) ){
			// (th4) The stash now owns the item -- kill the live cursor copy. ItemOnCursor is
			// serialized in the character save and dropped by PlayerDeadItem() on death, so a
			// stale copy left here resurfaces later as a same-seed twin, which FindGetItem()
			// then "detects and destroys" as a duplicate (player report: items that went
			// through the stash get falsely marked as dupes).
			Players[CurrentPlayerIndex].ItemOnCursor.ItemCode = IC_M1_NONE;
			return true;
		}
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
