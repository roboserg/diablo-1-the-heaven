#include "stdafx.h"

// Модуль всех действий с горожанами

int Put2x2ItemToInventoryCellsOrder [10] = { 8, 28, 6, 26, 4, 24, 2, 22, 0, 20}; // 004A0DE8
int TopSelectedRow;// 006EE898 // индекс строки на экране в которой находится самая верхнаяя из показанных вещей
int DialogRowsCount;// 006EE89C
int SelectedDialogRowInPrevDialog;// 006EE8A0
Item WirtItem;// 006EE8A8
PLAYER_DIALOG ConfirmationCaller;// 006EEA1C
Item MagicItemsStoreArray[PT_MagicItemsCount];// 006EEA20
char* TextBox2CELPtr;// 006EFFEC
int GriswoldItemsLevelParam;// 006EFFF0
int CurrentTowner;// 006EFFF4
TownersDialogRow TownersDialogRows[PT_DialogRowsCount];// 006EFFFC
char IsBigMenuBox;// 006F3F58
int ItemCount;// 006F3F5C
int CursorPentAnimationNumber;// 006F3F60
Item PlayerItemsForSomeAction[PT_48_PlayerItemsCount];// 006F3F68
int StartIndexOfGossibSpeechByCurrentCityzen;// 006F8528
Item WitchAdriaItems[PT_25_WitchItemsCount];// 006F8530
int DialogHasScroolBox;// 006FA984
int GriswoldMagicItemCount;// 006FA988
Item PepinItems[PT_HealerItemsCount];// 006FA990
Item ItemGoldBuffer;// 006FC6A0
char IndexOfPlayerItemInDialog[PT_48_PlayerItemsCount];// 006FC818
char* TextSlidCELPtr;// 006FC848
int ScrolledItemsCountInPrevDialog;// 006FC84C
int SelectedDialogRow;// 006FC850
char DialogScrollDownDelay;// 006FC854
int EndIndexOfGossibSpeechByCurrentCityzen;// 006FC858
char* PentSpn2CELPtr;// 006FC85C
int FirstVisibleItem;// 006FC860 // индекс самой первой из показанных вещей
int WirtItemHalfQlvl;// 006FC864
Item BasicalItemsArray[PT_BasicItemsCount];// 006FC868
int BottomSelectedRow;// 006FECBC // индекс строки на экране в которой находится самая нижняя из показанных вещей
char DialogScrollUpDelay;// 006FECC0
PLAYER_DIALOG CurrentDialogIndex;// 006FECC4

#define GET_ITEM_INDEX (ScrolledItemsCountInPrevDialog + ((SelectedDialogRowInPrevDialog - TopSelectedRow) / TradeRows))
#define GET_SELECTED_ITEM_INDEX (FirstVisibleItem + ((SelectedDialogRow - TopSelectedRow) / TradeRows))

void GriswoldRestockMenu(PLAYER_DIALOG callerDialog);
int GetGriswoldBasicalRestockPrice(Player &player);
int GetGriswoldMagicalRestockPrice(Player &player);

void AdriaRestockMenu();
int GetAdriaRestockPrice(Player &player);

//----- (0046C43D) --------------------------------------------------------
void InitInterface()
{
    TextBox2CELPtr = (char*)LoadFile("Data\\TextBox2.CEL");
    PentSpn2CELPtr = (char*)LoadFile("Data\\PentSpn2.CEL");
    TextSlidCELPtr = (char*)LoadFile("Data\\TextSlid.CEL");
    ClearRowsInInterval(0, PT_DialogRowsCount);
    CurrentDialogIndex = PD_0_None;
    IsBigMenuBox = false;
    CursorPentAnimationNumber = 1;
    GriswoldItemsLevelParam = 1;
    DialogHasScroolBox = false;
    GriswoldMagicItemCount = 0;
    for( int itemIndex = 0; itemIndex < PT_MagicItemsCount; itemIndex++ ){
        MagicItemsStoreArray[itemIndex].ItemCode = IC_M1_NONE;
    }
    WirtItem.ItemCode = IC_M1_NONE;
    WirtItemHalfQlvl = 0;
}

//----- (th2) -------------------------------------------------------------
int GetGriswoldBasicalRestockPrice(Player &player)
{
	if( is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) ){
		return player.CharLevel * 5;
	}else{
		return player.CharLevel * 2;
	}
}

int GetGriswoldMagicalRestockPrice(Player &player)
{
	if( is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) ){
		return player.CharLevel * player.CharLevel + 9;
	}else{
		return player.CharLevel * 6;
	}
}

int GetAdriaRestockPrice(Player &player)
{
	if( is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) ){
		return player.CharLevel * player.CharLevel + 9;
	}else{
		return player.CharLevel * 6;
	}
}

void ClearShopsForLOTL() {
    for (int i = 0; i < PT_BasicItemsCount; i++)
    {
        BasicalItemsArray[i].ItemCode = IC_M1_NONE;
        WitchAdriaItems[i].ItemCode = IC_M1_NONE;
    }
    for (int i = 0; i < PT_HealerItemsCount; i++)
    {
        PepinItems[i].ItemCode = IC_M1_NONE;
    }
    WirtItem.ItemCode = IC_M1_NONE;
    for (int i = 0; i < PT_MagicItemsCount; i++)
    {
        MagicItemsStoreArray[i].ItemCode = IC_M1_NONE;
    }
    for (int i = 0; i < 5; i++)
    {
        VisualTrade_RefreshPage((VisualTradePage)i);
    }
}

//----- (0046C4C3) --------------------------------------------------------
void ShopItemsGenerationByPlayerLevel()
{
    Player& player = Players[CurrentPlayerIndex];
    if (player.gameChanger & BIT(GC_5_LOTL)) {
        ClearShopsForLOTL();
        return;
    }

    SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon] * ms(GetTickCountTh())); // перехватываем вызов
    int itemQualityLevel = ShopQlvl(player);

    PutGoldBaseData();
    BasicalItemsGeneration(itemQualityLevel);
    WitchAdriaItemsGeneration(itemQualityLevel);
    PepinItemsGeneration(itemQualityLevel);
    WirtItemGeneration(player.CharLevel);
    MagicItemsGeneration(CurrentPlayerIndex);
}

//----- (th2) -------------------------------------------------------------
int ShopQlvl(const Player& player)
{
    int itemQualityLevel = 0;

    if (GameMode == GM_COLISEUM) {
        itemQualityLevel = player.CharLevel * 2;
    }
    else if( MaxCountOfPlayersInGame > 1 || is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_SPEEDRUN, GM_NIGHTMARE) ){
        itemQualityLevel = player.CharLevel / 2;
    }
    else{
        //for( int dun = 0; dun < DungeonCount; dun++ ){ // TODO: check for only non-quest dungeon needs
        //    if( player.dungeonVisited[dun] && Dungeons[dun].level > itemQualityLevel ){
        //        itemQualityLevel = Dungeons[dun].level;
        //    }
        //}
        itemQualityLevel = player.CharLevel;
    }
    itemQualityLevel += GameMode == GM_COLISEUM ? 5 : 2;
    // mor: might be a good time to re-arrnage max limit here after adding new items. remember to exclude elixirs!
    int max_qlvl_limit = 16;
    if (GameMode == GM_COLISEUM ) { max_qlvl_limit = 312; }
    LimitToRange(itemQualityLevel, 6, max_qlvl_limit); // TODO: review to new clvl

    return itemQualityLevel;
}

//----- (th3) -------------------------------------------------------------
double Discount(int playerIndex)
{
	int haggleValue = PerkValue(PERK_HAGGLER, CurrentPlayerIndex) + PerkValue(SYNERGY_SILVER_TONGUE, CurrentPlayerIndex);
	int negotiantTraitBenefit = HasTrait(CurrentPlayerIndex, TraitId::Negotiant) ? 10 : 0;
	return haggleValue + negotiantTraitBenefit;
}

//----- (th3) -------------------------------------------------------------
int BuyPrice(int price)
{
    double priceModifier = HasTrait(CurrentPlayerIndex, TraitId::NastyDisposition) ? 300.0 : 100.0;
	return (int)(price * (priceModifier - Discount(CurrentPlayerIndex)) / 100.);
}
int BuyPrice(Item& item)
{
	return BuyPrice(item.Price);
}

//----- (th3) -------------------------------------------------------------
int SellPrice(int price)
{
    double priceModifier = HasTrait(CurrentPlayerIndex, TraitId::NastyDisposition) ? 33.33 : 100.0;
	return (int)(price * (priceModifier + Discount(CurrentPlayerIndex)) / 100.);
}
int SellPrice(Item& item)
{
	return SellPrice(item.Price);
}

//----- (0046C588) --------------------------------------------------------
void FreeCELPtr()
{
    FreeMemZero(TextBox2CELPtr);
    FreeMemZero(PentSpn2CELPtr);
    FreeMemZero(TextSlidCELPtr);
}

//Interface

//----- (th2) -------------------------------------------------------------
void __fastcall DrawDialogBox( int width, int height, char* bitmap, int x, int y )
{
	uchar* currentPixel;
	int rollback = 0;
	if( ! x ) x = (/*ScreenWidth*/ GUI_Width - width) / 2;
	if( ! y ) y = height + ((GUI_Height/*ScreenHeight*/ - height - GUI_MainPanelHeight) / 2) + 8;
	extern RECT DialogWin; DialogWin = { x, y - height + 1, x + width - 1, y };
	x += Screen_LeftBorder;
	y += Screen_TopBorder;
	Surface_DrawCEL( x, y, bitmap, 1, width );
	currentPixel = &WorkingSurface[ x ] + YOffsetHashTable[ y - 3 ] + 3;
	for( int j = 0; j < (height - 5); j++ ){
		rollback = 0;
		if( (j % 2) == 0 ){
			currentPixel++;
			rollback++;
		}
		for( int i = 0; i < ((width - 5) / 2); i++ ){
			if( (i % 2) == 1 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}else if( (i % 2) == 0 ){
				*currentPixel = 0;
				currentPixel += 2;
				rollback += 2;
			}
		}
		currentPixel -= rollback + YOffsetHashTable[ 1 ];
	}
}

//----- (0046C61C) --------------------------------------------------------
void __fastcall DrawGameDialogTitleText( int position, int rowIndex, int isAligned, char* rowText, int color, int gold )
{
    int height;
    int width;
	int xPos;
	int yPos;
    if( IsBigMenuBox ){
        width = 577;//588;
		height = 302;
		xPos = Screen_LeftBorder + 32;//((WorkingWidth - width) >> 1);
		yPos = Screen_TopBorder + 44 + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ];
    }else{
        width = 257;//268;
		height = 302;
		xPos = Screen_LeftBorder + 346;//((WorkingWidth - width) >> 1);
		yPos = Screen_TopBorder + 44 + TownersDialogRows[ rowIndex ].verticalIndent + StringRowYPosition[ rowIndex ];
    }
    int charPos = YOffsetHashTable[yPos] + position + xPos;
    size_t len = strlen(rowText);
    
    int indent = 0;
    if( isAligned ){
        indent = width;
        // Из длинны отведенной под строку отнимаем ширину каждой буквы и ширину расстояния между буквами
        for( size_t i = 0; i < len; i++ ){
            indent -= FontWidthSmall[FontIndexSmall[Codepage[rowText[i]]]] + 1;
        }
        // Получаем разницу между максимальной шириной и используемой. Поделив пополам получаем отступ
        indent = indent >> 1;
        charPos += indent;
    }

    // Рисуем пентаграмму левую если строка выделена
    if( SelectedDialogRow == rowIndex ){
        int pentagramPos = indent + xPos + position - 20;
        Surface_DrawCEL(pentagramPos, yPos + 1, PentSpn2CELPtr, CursorPentAnimationNumber, 12);
    }

    // пишем все буквы строки
    for( size_t i = 0; i < len; i++ ){
        uchar currentChar = FontIndexSmall[Codepage[rowText[i]]];
        indent += FontWidthSmall[currentChar] + 1;
        if( currentChar && indent <= width ){
            DrawLetter(charPos, currentChar, color);
        }
        charPos += FontWidthSmall[currentChar] + 1;
    }

    // с правого края, справа пишем цифру золота, с выравниванием по правому краю.
    if( !isAligned && gold >= 0 ){
        char goldString[32];
        sprintf(goldString, "%i", gold);
        charPos = YOffsetHashTable[yPos] - position + Screen_LeftBorder + 592; // + xPos + width - CELL_SIZE;
        for( int i = strlen(goldString) - 1; i >= 0; i-- ){
            uchar currentChar = FontIndexSmall[Codepage[goldString[i]]];
            charPos += - 1 - FontWidthSmall[currentChar];
            if( currentChar ){
                DrawLetter(charPos, currentChar, color);
            }
        }
    }

    // рисуем правую пентаграмму у выделенной строки
    if( SelectedDialogRow == rowIndex ){
        int pentagramPos;
        if( isAligned ){
            pentagramPos = indent + xPos + position + 4;
        }else{
            pentagramPos = Screen_LeftBorder + 596 - position; // xPos + width - CELL_SIZE;
        }
        Surface_DrawCEL(pentagramPos, yPos + 1, PentSpn2CELPtr, CursorPentAnimationNumber, 12);
    }
}

//----- (0046C82A) --------------------------------------------------------
void __fastcall DrawDialogLine( int rowIndex )
{
    // Линия получается путем копирования верхней линии бокса
    int width;
    int height;
	int xPos;
	int yPos;
    if( IsBigMenuBox ){
        width = 586;
        height = 302;
		xPos = Screen_LeftBorder + 26;//(WorkingWidth - width) / 2;
		yPos = Screen_TopBorder + 25;//Screen_TopBorder + ((ScreenHeight - height - 144) / 2);
    }else{
        width = 266;
        height = 302;
		xPos = Screen_LeftBorder + 346;// (WorkingWidth - width) / 2;
		yPos = Screen_TopBorder + 25;//Screen_TopBorder + ((ScreenHeight - height - 144) / 2);
    }
    uchar* dst = &WorkingSurface[xPos] + YOffsetHashTable[yPos + StringRowYPosition[rowIndex] + 13];
    uchar* src = &WorkingSurface[xPos] + YOffsetHashTable[yPos];
    for( int i = 0; i < 3; i++ ){
        memcpy(dst, src, width);
        dst += WorkingWidth;
        src += WorkingWidth;
    }
}

//----- (0046C8B4) --------------------------------------------------------
void __fastcall DrawDialogScroolBox( int firstRow, int lastRow )
{
    int width = 588;
    int height = 302;
    int scrollWidth = 12;
    int scrollXOffset = 2;
    int scrollYOffset = 5;
    int xPos = ((WorkingWidth - width) >> 1);
    int yPos = Screen_TopBorder + 44; // Screen_TopBorder + ((ScreenHeight - height - 144) >> 1) + scrollYOffset;
    int scrollUp = StringRowYPosition[firstRow] + yPos;
    int scroolBottom = StringRowYPosition[lastRow] + yPos;

    // стрелка вверх
    int scrollBoxPicIndex;
    if( DialogScrollUpDelay == -1 ){
        scrollBoxPicIndex = 10;
    }else{
        scrollBoxPicIndex = 12;
    }
    Surface_DrawCEL(Screen_LeftBorder + 601/*xPos + width - scrollWidth - scrollXOffset*/, scrollUp, TextSlidCELPtr, scrollBoxPicIndex, 12);

    // стрелка вниз
    if( DialogScrollDownDelay == -1 ){
        scrollBoxPicIndex = 9;
    }else{
        scrollBoxPicIndex = 11;
    }
    Surface_DrawCEL(Screen_LeftBorder + 601/*xPos + width - scrollWidth - scrollXOffset*/, scroolBottom, TextSlidCELPtr, scrollBoxPicIndex, 12);

	// полоска
	for( int i = scrollUp + 12; i < scroolBottom; i += 12 ){
		Surface_DrawCEL(Screen_LeftBorder + 601/*xPos + width - scrollWidth - scrollXOffset*/, i, TextSlidCELPtr, 14, 12);
	}

	// ползунок
	int v11 = SelectedDialogRow;
	if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
		v11 = SelectedDialogRowInPrevDialog;
	}
	int scroolBoxPos;
	if( DialogRowsCount <= 1 ){
		scroolBoxPos = 0;
	}else{
		scroolBoxPos = (StringRowYPosition[lastRow] - StringRowYPosition[firstRow] - 24) * 1000 * (FirstVisibleItem + ((v11 - TopSelectedRow) / TradeRows)) / (DialogRowsCount - 1) / 1000;
	}
	Surface_DrawCEL(Screen_LeftBorder + 601/*xPos + width - scrollWidth - scrollXOffset*/, scrollUp + scroolBoxPos + 12, TextSlidCELPtr, 13, 12);
}

//----- (0046C9A5) --------------------------------------------------------
void SetGameHelpBigMenuBox()
{
    SelectedDialogRow = -1;
    IsBigMenuBox = true;
}

//----- (0046C9B4) --------------------------------------------------------
void __fastcall ClearRowsInInterval( int startRowIndex, int endRowIndex )
{
    for( int curRowIndex = startRowIndex; curRowIndex < endRowIndex; curRowIndex++){
        TownersDialogRow& curDialogRow = TownersDialogRows[curRowIndex];
        curDialogRow.position = PT_DialogRows_NoIndent;
        curDialogRow.verticalIndent = 0;
        *curDialogRow.text = '\0';
        curDialogRow.isAligned = false;
        curDialogRow.color = C_0_White;
        curDialogRow.isLine = false;
        curDialogRow.selectable = false;
        curDialogRow.price = -1;
    }
}

//----- (0046C9F9) --------------------------------------------------------
void __fastcall SetDialogLine( int rowIndex )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.position = PT_DialogRows_NoIndent;
    curDialogRow.verticalIndent = 0;
    *curDialogRow.text = '\0';
    curDialogRow.isLine = true;
}

//----- (0046CA1E) --------------------------------------------------------
void __fastcall SetItemPrice( int rowIndex, int price )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.price = price;
}

//----- (0046CA2B) --------------------------------------------------------
void __fastcall LowerRow( int rowIndex, int indent )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.verticalIndent = indent;
}

//----- (0046CA38) --------------------------------------------------------
void __fastcall SetDialogRow( int position, int rowIndex, int isAligned, char* text, char color, int selectable )
{
    TownersDialogRow& curDialogRow = TownersDialogRows[rowIndex];
    curDialogRow.position = position;
    curDialogRow.verticalIndent = 0;
    strcpy(curDialogRow.text, text);
    curDialogRow.isAligned = isAligned;
    curDialogRow.color = color;
    curDialogRow.isLine = false;
    curDialogRow.selectable = selectable;
}

//----- (0046CA89) --------------------------------------------------------
bool AutoPutCursorToInventoryOrDrop( bool allowDropWhenInvFull /*= true*/, int cell /*= -1*/ )
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.ItemOnCursor;

    SetupItemSizes(item.GraphicValue + CM_12_ITEMS_PICS_START);
    bool put = AutoPutCursorToInventory(player, cell);
	if( put ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
		player.ItemOnCursor.ItemCode = IC_M1_NONE;
	}
	if( ! put && allowDropWhenInvFull ){
		put = DropCursorItemBeforeTrig();
		if( put ) FixItemCursor(-1);
	}
    return put;
}

//----- (0046CD2A) --------------------------------------------------------
void GriswoldMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;

    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Logo2, true, "Blacksmith's shop", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Talk, true, "Talk to Griswold", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Trade, true, "Trade / Repair", C_0_White, true);
    if( CanGrind() ){
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Craft, true, "Crafting / Socketing", C_0_White, true);
    }
	if( (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM))) && (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN))) && GameMode != GM_CLASSIC){
		SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Quench, true, "Quench Items", C_0_White, true); 
	}
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_1_BlacksmithMain_Leave, true, "Leave the shop", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//----- (0046CDF4) --------------------------------------------------------
void __fastcall DrawBuyBasicalItems( int firstItem )
{
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart, itemNum = firstItem; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& basicItem = BasicalItemsArray[itemNum];
        if( basicItem.ItemCode != IC_M1_NONE ){
            // Выбор цвета
            uchar color = C_0_White;
            if( basicItem.MagicLevel == ML_1_MAGIC ){
                color = C_1_Blue;
            }
            if( !basicItem.IsReqMet ){
                color = C_2_Red;
            }
            // Выбор полного или сокращённого имени
            char* name; 
            if( basicItem.MagicLevel != ML_0_USUAL && basicItem.Identified ){
                name = basicItem.FullMagicalItemName;
            }else{
                name = basicItem.Name;
            }
            SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);
            SetItemPrice(dialogRow, BuyPrice(basicItem));
            PrintItemParamToTownsmenTradeDialog(&basicItem, dialogRow + 1, color);
            BottomSelectedRow = dialogRow;
        }
    }

    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//----- (0046CEB0) --------------------------------------------------------
void __fastcall PrintItemParamToTownsmenTradeDialog(Item* itemPtr, int row, char color)
{
    char buffer[128];
    Item& item = *itemPtr;
    buffer[0] = '\0';
    if( item.Identified ){
        if( item.MagicLevel < ML_2_UNIQUE && not(item.effect[0].id, AE_NO, AE_SPELL) ){
            PrintItemEffectToStringBuffer(item.effect[0].id, itemPtr, 0);
            strcat(buffer, InfoPanelBuffer);
        }
        if( not(item.effect[1].id, AE_NO, AE_SPELL) ){
            PrintItemEffectToStringBuffer(item.effect[1].id, itemPtr, 1);
            if( buffer[0] ){
                strcat(buffer, ",  ");
            }
            strcat(buffer, InfoPanelBuffer);
        }
    }

    if( item.BaseCharges ){
        sprintf(InfoPanelBuffer, "%s Charges: %i/%i", getSpellName( item.SpellIndex ), item.CurCharges, item.BaseCharges);
        if( buffer[0] ){
            strcat(buffer, ",  ");
        }
        strcat(buffer, InfoPanelBuffer);
    }

    if( buffer[0] ){
        SetDialogRow(PT_DialogRows_BigIndent, row++, false, buffer, color, false);
    }

    buffer[0] = '\0';

    if( item.TypeID == ITEM_1_WEAPON ){
        sprintf(buffer,BaseItems[item.baseItemIndex].AnimationDelay ? "Slow dmg: %i-%i  " : "Damage: %i-%i  ", item.MinDamage, item.MaxDamage);
    }

    if( item.TypeID == ITEM_2_ARMOR ){
        sprintf(buffer, "Armor: %i  ", item.ArmorClass);
    }

    char* durBuffer = "";
    if( item.BaseDurability != ITEM_DUR_INDESTRUBLE && item.BaseDurability && is(item.TypeID, ITEM_1_WEAPON, ITEM_2_ARMOR) ){
        sprintf(InfoPanelBuffer, "Dur: %i/%i,  ", item.CurDurability, item.BaseDurability);
        durBuffer = InfoPanelBuffer;
    }else{
        durBuffer = "Indestructible,  ";
    }
    strcat(buffer, durBuffer);

    if( item.ItemCode == IC_0_OTHER ){
        buffer[0] = '\0';
    }

    bool isItemHaveReqs = item.RequiredVitality || item.RequiredStrength || item.RequiredDexterity || item.RequiredMagic || item.RequiredLevel;

    if( item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u ){
        strcat(buffer, "Spell is maxed");
    }else if( !isItemHaveReqs){
        strcat(buffer, "No required attributes");
    }else{
        strcpy(InfoPanelBuffer, "Req:");
        if( item.RequiredLevel ){
            sprintf(InfoPanelBuffer, "%s %i Lvl", InfoPanelBuffer, item.RequiredLevel);
        }
        if( item.RequiredStrength ){
            sprintf(InfoPanelBuffer, "%s %i Str", InfoPanelBuffer, item.RequiredStrength);
        }
        if( item.RequiredMagic ){
            sprintf(InfoPanelBuffer, "%s %i Mag", InfoPanelBuffer, item.RequiredMagic);
        }
        if( item.RequiredDexterity ){
            sprintf(InfoPanelBuffer, "%s %i Dex", InfoPanelBuffer, item.RequiredDexterity);
        }
        if( item.RequiredVitality ){
            sprintf(InfoPanelBuffer, "%s %i Vit", InfoPanelBuffer, item.RequiredVitality);
        }
        strcat(buffer, InfoPanelBuffer);
    }
    SetDialogRow(PT_DialogRows_BigIndent, row++, false, buffer, color, false);

    if( item.MagicLevel >= ML_2_UNIQUE && item.Identified ){
        char* itemMagicLevelName;
        if( item.MagicLevel == ML_2_UNIQUE ){
            itemMagicLevelName = item.dropType & D_RARE ? "Rare Item" : "Unique Item";
        }else{
            itemMagicLevelName = "set item";
        }
        SetDialogRow(PT_DialogRows_BigIndent, row++, false, itemMagicLevelName, color, false);
    }
}

//----- (0046D125) --------------------------------------------------------
void BasicalItemBuyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawBuyBasicalItems(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_BasicItemsCount; ++DialogRowsCount){
        Item& item = BasicalItemsArray[DialogRowsCount];
        if( item.ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//----- (0046D1EC) --------------------------------------------------------
void __fastcall DrawBuyMagicalItems(int firstItem)
{
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    
    int magicalItemIndex = 0;
    for( int i = firstItem; i > 0; magicalItemIndex++){
        if( MagicItemsStoreArray[magicalItemIndex].ItemCode != IC_M1_NONE ){
            --i;
        }
    }

    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && magicalItemIndex < PT_MagicItemsCount; dialogRow += TradeRows, magicalItemIndex++ ){
        Item& item = MagicItemsStoreArray[magicalItemIndex];
        if( item.ItemCode == IC_M1_NONE ){
            dialogRow -= 4;
            continue;
        }
        uchar color = C_0_White;
        if( item.MagicLevel != ML_0_USUAL ){
            color = C_1_Blue;
        }
        if( !item.IsReqMet ){
            color = C_2_Red;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, item.FullMagicalItemName, color, true);
        SetItemPrice(dialogRow, BuyPrice(item));
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }

    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//----- (0046D2D5) --------------------------------------------------------
bool MagicItemsBuyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    
    DialogRowsCount = 0;
    for( int magicalItemIndex = 0; magicalItemIndex < PT_MagicItemsCount; magicalItemIndex++){
        Item& currentItem = MagicItemsStoreArray[magicalItemIndex];
        if( currentItem.ItemCode != IC_M1_NONE ){
            DialogRowsCount++;
        }
    }

    if( !DialogRowsCount ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        return false;
    }

    FirstVisibleItem = 0;
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    sprintf(InfoPanelBuffer, "I have these magical items for sale :   Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
    DrawBuyMagicalItems(FirstVisibleItem);
    return true;
}

//----- (0046D3BF) --------------------------------------------------------
bool __fastcall CanSellItemToGriswold( int inventoryIndex )
{
    Player& player = Players[CurrentPlayerIndex];
    Item* currentItem;
    if( inventoryIndex < 0 ){
        currentItem = &player.BeltInventory[-(inventoryIndex + 1)];
    }else{
        currentItem = &player.InventorySlots[inventoryIndex];
    }
    Item& item = *currentItem;
    int itemCode = item.ItemCode;
    if( itemCode == IC_M1_NONE ){
        return false;
    }
    if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HARDENING ){
        return true;
    }
    if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == IC_14_EAR ){
        return false;
    }
    if( itemCode == IC_10_STAFF && item.SpellIndex ){
        return false;
    }
    if( item.MagicCode == MC_23_STAFF && BaseItems[item.baseItemIndex].Specialization == IS_MAGIC) {
        return false;
    }
    if( item.TypeID == ITEM_5_NOVELTY ){
        return false;
    }
    if( item.baseItemIndex == BI_33_STAFF_OF_LAZARUS ){
        return false;
    }
    return true;
}

//----- (0046D458) --------------------------------------------------------
void __fastcall DrawPlayersItems(int firstItem, PLAYER_DIALOG Caller)
{
    int itemNum = firstItem;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& item = PlayerItemsForSomeAction[itemNum];
        if( item.ItemCode == IC_M1_NONE ){
            continue;
        }
        char color = ItemColor(item);
        // Выбор полного или сокращённого имени
        char* name ; 
        if( item.MagicLevel != ML_0_USUAL && item.Identified ){
            name = item.FullMagicalItemName;
        }else{
            name = item.Name;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);

        int itemPrice = 0;
        if (Caller == PD_3_BlacksmithSale ||
            Caller == PD_7_WitchSell) {
            itemPrice = SellPrice(item);
        } else if (Caller == PD_4_RepairItem ||
                    Caller == PD_8_RechargeItem
        ) {
            itemPrice = item.Price;
        } else {
            itemPrice = BuyPrice(item);
        }

        SetItemPrice(dialogRow, itemPrice);
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin( ItemCount, 0 );
}

//----- (0046D53F) --------------------------------------------------------
void GriswoldSellMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    bool isHaveItemForSell = false;

    for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSellItemToGriswold(inventoryItemIndex) ){
            memcpy(&item, &player.InventorySlots[inventoryItemIndex], sizeof (Item));
            isHaveItemForSell = true;
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.basePrice = item.Price;
            }
            item.basePrice /= PT_SellRatio;
            LimitToMin(item.basePrice, 1);
            item.Price = item.basePrice;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
        }
    }

    for( int beltIndex = 0; beltIndex < IS_8_Belt_Count && DialogRowsCount < PT_48_PlayerItemsCount; beltIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSellItemToGriswold(-(beltIndex + 1)) ){
            memcpy(&item, &player.BeltInventory[beltIndex], sizeof (Item));
            isHaveItemForSell = true;
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.basePrice = item.Price;
            }
            item.basePrice /= PT_SellRatio;
            LimitToMin(item.basePrice, 1);
            item.Price = item.basePrice;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = -(beltIndex + 1);
        }
    }

    if( !isHaveItemForSell ){
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer,	"You have nothing I want.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }else{
        DialogHasScroolBox = true;
        FirstVisibleItem = 0;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Which item is for sale?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem, PD_3_BlacksmithSale);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

bool __fastcall ItemNeedRepair( const Item* item )
{
    int itemCode = item->ItemCode;
    if( itemCode == IC_M1_NONE || itemCode == IC_0_OTHER 
        || itemCode == IC_11_GOLD || itemCode == IC_14_EAR ){
            return false;
    }
    return item->CurDurability != item->BaseDurability && item->TypeID != ITEM_3_JEWELRY_POTION_RELICT;
}

//----- (0046D806) --------------------------------------------------------
bool __fastcall ItemNeedRepair( int invItemIndex )
{
    const Player& player = Players[CurrentPlayerIndex];
    const Item& item = player.InventorySlots[invItemIndex];
    return ItemNeedRepair(&item);
}

char RepairSlot[] = { IS_Head, IS_Torso, IS_LeftHand, IS_RightHand, IS_Waist, IS_Hands, IS_Feet };
char RepairId[] = { PT_Repair_Head, PT_Repair_Body, PT_Repair_LeftHand, PT_Repair_RightHand, PT_Repair_Waist, PT_Repair_Hands, PT_Repair_Feet };
//----- (0046D852) --------------------------------------------------------
void RepairItemMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    bool hasItemToRepair = false;
    DialogRowsCount = 0;
    IsBigMenuBox = true;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    for( int repairId = 0; repairId < countof(RepairSlot); ++repairId ){ // в оригинале без цикла. Небольшая оптимизация
        Item& item = player.OnBodySlots[RepairSlot[repairId]];
        if( item.ItemCode != IC_M1_NONE && item.CurDurability != item.BaseDurability ){
            hasItemToRepair = true;
            AddItemToRepairList(&item, RepairId[repairId] );
        }
    }
	// добавление второго комплекта в список починки
	memcpy( &TempItem, &player.ItemOnCursor, sizeof( Item ) );

	auto addAltWeapon = [&]( int altItemIndex, int inventoryItemIndex )
    {
	    LoadItem( &WeaponSwapItems[ altItemIndex ], &player.ItemOnCursor );
        CheckItemStats( CurrentPlayerIndex );
        if( player.ItemOnCursor.ItemCode != IC_M1_NONE && player.ItemOnCursor.CurDurability != player.ItemOnCursor.BaseDurability){
            hasItemToRepair = 1;
            AddItemToRepairList( &player.ItemOnCursor, inventoryItemIndex );
        }
    };
    
    addAltWeapon( AW_LeftHand, PT_Repair_SecondLeftHand );
    addAltWeapon( AW_RightHand, PT_Repair_SecondRightHand );
    addAltWeapon( AW_LeftHand_2, PT_Repair_SecondLeftHand_2 );
    addAltWeapon( AW_RightHand_2, PT_Repair_SecondRightHand_2 );
    
	memcpy( &player.ItemOnCursor, &TempItem, sizeof( Item ) );
	// добавление инвентаря в список починки
    for( int repairId = 0; repairId < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; repairId++ ){// Проходим по всем вещам в инвентаре
        if( ItemNeedRepair(repairId) ){
            hasItemToRepair = true;
            AddItemToRepairList(&player.InventorySlots[repairId], repairId);
        }
    }
    if( hasItemToRepair ){
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Repair which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem, PD_4_RepairItem);
    }else{
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to repair.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//----- (0046DAB9) --------------------------------------------------------
void __fastcall AddItemToRepairList( Item* linkOfItemSlot, int inventoryItemIndex )
{   
    Item& item = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&item, linkOfItemSlot, sizeof (Item));
    if( item.MagicLevel != ML_0_USUAL && item.Identified ){// Если это магическая идентифицированая вещь
        item.basePrice = 30 * item.Price / 100;
    }
    int priceOfRepair = item.basePrice * (25 * (item.BaseDurability - item.CurDurability) / item.BaseDurability) / 100;
    if( !priceOfRepair ){
		#ifdef CLASSIC_NO_REPAIR // непочинка нераспознанных вещей отключена в th1
		if( item.MagicLevel != ML_0_USUAL && item.Identified ) return;
		#endif
        priceOfRepair = 1;
    }
    if( priceOfRepair > 1 ){
        priceOfRepair /= 2;
    }
    item.Price = priceOfRepair;
    item.basePrice = priceOfRepair;
    IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
}

//----- (0046DB5E) --------------------------------------------------------
void WitchAdriaMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    if (GameMode != GM_COLISEUM && GameMode != GM_CLASSIC) {
        if (player.CurMana != player.MaxCurMana) {
            PlayGlobalSound(S_88_CAST9);
        }
        player.CurMana = player.MaxCurMana;
        player.BaseMana = player.MaxBaseMana;
    }
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Logo, true, "Witch's shack", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Talk, true, "Talk to Adria", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Trade, true, "Trade / Recharge", C_0_White, true);
    if( GameMode != GM_COLISEUM && GameMode != GM_CLASSIC && !HasTrait(CurrentPlayerIndex, TraitId::Barbarian) && (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN)))
        && (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)))){
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Enchant, true, "Enchant Items", C_0_White, true); 
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_5_WitchMain_Leave, true, "Leave the shack", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//----- (0046DC04) --------------------------------------------------------
void __fastcall DrawAdriaItems( int firstItem )
{
    int itemNum = firstItem;                              
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itemNum < DialogRowsCount; dialogRow += TradeRows, itemNum++ ){
        Item& item = WitchAdriaItems[itemNum];
        if( item.ItemCode == IC_M1_NONE ){
            continue;
        }
        // Выбор цвета
        uchar color = C_0_White;
        if( item.MagicLevel != ML_0_USUAL ){
            color = C_1_Blue;
        }
        if( !item.IsReqMet ){
            color = C_2_Red;
        }
        // Выбор полного или сокращённого имени
        char* name; 
        if( item.MagicLevel != ML_0_USUAL && item.Identified ){
            name = item.FullMagicalItemName;
        }else{
            name = item.Name;
        }
        SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, name, color, true);
        SetItemPrice(dialogRow, BuyPrice(item));
        PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
        BottomSelectedRow = dialogRow;
    }
    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//----- (0046DCC0) --------------------------------------------------------
void WitchAdriaStore()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    ItemCount = 20;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawAdriaItems(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_25_WitchItemsCount; DialogRowsCount++){
        if( WitchAdriaItems[DialogRowsCount].ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//----- (0046DD91) --------------------------------------------------------
int __fastcall CanSellItemToAdria( int inventoryIndex )
{
    int result = 0;
	Player& player = Players[CurrentPlayerIndex];
    Item* itemPtr;
    if( inventoryIndex < 0 ){// Если индекс отрцательный - смотрим пояс
        itemPtr = &player.BeltInventory[-(inventoryIndex + 1)];
    }else{
        itemPtr = &player.InventorySlots[inventoryIndex];
    }
    Item& item = *itemPtr;
    if( item.ItemCode == IC_0_OTHER ){// Potions, relicts and books
        result = 1;
    }
    if( item.MagicCode >= MC_30_OIL_OF_SOMETHING && item.MagicCode <= MC_40_OIL_OF_HARDENING ){
        result = 0;
    }
    if( item.TypeID == ITEM_5_NOVELTY ){
        result = 0;
    }
    if( item.ItemCode == IC_10_STAFF && item.SpellIndex ){
        result = 1;
    }
    if( item.MagicCode == MC_23_STAFF && BaseItems[item.baseItemIndex].Specialization == IS_MAGIC) {
        result = 1;
    }
    if( item.baseItemIndex >= BI_6_HATCHET && item.baseItemIndex <= BI_22_CRYPT_MAP ){
        result = 0;
    }
    if( item.baseItemIndex == BI_33_STAFF_OF_LAZARUS ){
        result = 0;
    }
    return result;
}

//----- (0046DE2F) --------------------------------------------------------
void WitchAdriaSellMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }

    bool hasItemToWitchSell = false;

    for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        if( CanSellItemToAdria(inventoryItemIndex) ){
            hasItemToWitchSell = true;
            memcpy(&item, &player.InventorySlots[inventoryItemIndex], sizeof(Item));
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.basePrice = item.Price;
            }
            item.basePrice /= PT_SellRatio;
            LimitToMin(item.basePrice, 1); // в 1.216 минимум 1 монетка
            item.Price = item.basePrice;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = inventoryItemIndex;
        }
    }

    for( int beltItemIndex = 0; beltItemIndex < IS_8_Belt_Count && DialogRowsCount < PT_48_PlayerItemsCount; beltItemIndex++ ){
        Item& item = PlayerItemsForSomeAction[DialogRowsCount];
        Item& beltItem = player.BeltInventory[beltItemIndex];
        if( beltItem.ItemCode != IC_M1_NONE && CanSellItemToAdria(-(beltItemIndex + 1) ) ){
            hasItemToWitchSell = true;
            memcpy(&item, &beltItem, sizeof(Item));
            if( item.MagicLevel != ML_0_USUAL && item.Identified ){
                item.basePrice = item.Price;
            }
            item.basePrice /= PT_SellRatio;
            LimitToMin(item.basePrice, 1); // в 1.216 минимум 1 монетка
            item.Price = item.basePrice;
            IndexOfPlayerItemInDialog[DialogRowsCount++] = -(beltItemIndex + 1);
        }
    }

    if( !hasItemToWitchSell ){
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing I want.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }else{
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Which item is for sale?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem, PD_7_WitchSell);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//----- (0046E12F) --------------------------------------------------------
bool __fastcall StaffNeedRecharge( int invItemIndex )
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.InventorySlots[invItemIndex];
    if( item.CurCharges != item.BaseCharges ){
        return true;
    }
    return false;
}

//----- (0046E18D) --------------------------------------------------------
void __fastcall AddItemToRechargeList(char itemInventoryIndex, int edx_unused, const Item& item)
{
    Item& shopItem = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&shopItem, &item, sizeof (Item));
    shopItem.basePrice += Spells[item.SpellIndex].PriceMultiplier;
    int calculatedPrice;
    calculatedPrice = shopItem.basePrice * 100 * (shopItem.BaseCharges - shopItem.CurCharges) / shopItem.BaseCharges / 100 / 2;
    shopItem.basePrice = calculatedPrice;
    shopItem.Price = calculatedPrice;
    IndexOfPlayerItemInDialog[DialogRowsCount] = itemInventoryIndex;
    DialogRowsCount++;
}

//----- (0046E213) --------------------------------------------------------
void RechargeItemMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogRowsCount = 0;
    for( int i = 0; i < PT_48_PlayerItemsCount; i++ ){// Обнуление
        PlayerItemsForSomeAction[i].ItemCode = IC_M1_NONE;
    }
    bool hasStaffToRecharge = false;
	Item tempItem;
	for( int rechargeId = 0; rechargeId < IS_Inventory; ++rechargeId ){ // в оригинале без цикла. Небольшая оптимизация
		Item& item = player.OnBodySlots[rechargeId];
		if( item.ItemCode != IC_M1_NONE && item.CurCharges != item.BaseCharges ){// Палка надета и нуждается в подзарядке
			hasStaffToRecharge = true;
			memcpy(&tempItem, &item, sizeof (Item));
			AddItemToRechargeList(-(rechargeId + 1), 0, tempItem);
		}
    }
	// добавление второго комплекта в список зарядки
	auto addAltWeapon = [&]( int altItemIndex, int inventoryItemIndex )
    {
	    LoadItem( &WeaponSwapItems[ altItemIndex ], &tempItem );
        if( tempItem.ItemCode != IC_M1_NONE && tempItem.CurCharges != tempItem.BaseCharges ){
            hasStaffToRecharge = 1;
            tempItem.IsReqMet = CheckItemReq( CurrentPlayerIndex, &tempItem );
            AddItemToRechargeList( inventoryItemIndex, 0, tempItem );
        }
    };
	
	addAltWeapon( AW_LeftHand, PT_Recharge_SecondLeftHand );
    addAltWeapon( AW_RightHand, PT_Recharge_SecondRightHand );
    addAltWeapon( AW_LeftHand_2, PT_Recharge_SecondLeftHand_2 );
    addAltWeapon( AW_RightHand_2, PT_Recharge_SecondRightHand_2 );
    
	// добавление инвентаря в список зарядки
	for( int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; inventoryItemIndex++ ){// Палка в инвентаре
        if( StaffNeedRecharge(inventoryItemIndex) ){
            hasStaffToRecharge = true;
            memcpy(&tempItem, &player.InventorySlots[inventoryItemIndex], sizeof (Item));
            AddItemToRechargeList(inventoryItemIndex, 0, tempItem);
        }
    }
    if( hasStaffToRecharge ){
        DialogHasScroolBox = true;
        FirstVisibleItem = 0;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Recharge which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem, PD_8_RechargeItem);
    }else{							
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to recharge.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//----- (0046E3FF) --------------------------------------------------------
void NoGold()
{
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    IsBigMenuBox = true;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough gold", C_0_White, true);
}

//----- (0046E439) --------------------------------------------------------
void NoRoom()
{
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough room in inventory", C_0_White, true);
}

//----- (0046E46C) --------------------------------------------------------
void ConfirmActionMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    // Расчистка места
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd); 
    char itemMagicLevel = player.ItemOnCursor.MagicLevel;
    // Задание цвета вещи
    int color = ItemColor(player.ItemOnCursor);
    if( !player.ItemOnCursor.IsReqMet ){
        color = C_2_Red;
    }
    // Choosing to use the full or short name of the item
    bool useFullItemName = itemMagicLevel != ML_0_USUAL;
    if( ConfirmationCaller == PD_17_IdentifyItem ){
        useFullItemName = false;
    }

    if( itemMagicLevel != ML_0_USUAL && !player.ItemOnCursor.Identified ){
        if( ConfirmationCaller == PD_3_BlacksmithSale ||
            ConfirmationCaller == PD_4_RepairItem ||
            ConfirmationCaller == PD_7_WitchSell ||
            ConfirmationCaller == PD_8_RechargeItem ){
                useFullItemName = false;
        }
    }
    char* itemName;
    if( useFullItemName ){
        itemName = player.ItemOnCursor.FullMagicalItemName;
    }else{
        itemName = player.ItemOnCursor.Name;
    }
    // Outputting the string with the name of the item and its properties
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_11_Confirm_Item, false, itemName, color, false);

    int itemPrice = 0;
    if (ConfirmationCaller == PD_3_BlacksmithSale ||
        ConfirmationCaller == PD_7_WitchSell) {
        itemPrice = SellPrice(player.ItemOnCursor);
    } else if (ConfirmationCaller == PD_4_RepairItem ||
               ConfirmationCaller == PD_8_RechargeItem
            ) {
        itemPrice = player.ItemOnCursor.Price;
    } else {
        itemPrice = BuyPrice(player.ItemOnCursor);
    }

    SetItemPrice(PT_DialogRows_11_Confirm_Item, itemPrice);
    PrintItemParamToTownsmenTradeDialog(&player.ItemOnCursor, PT_DialogRows_11_Confirm_Item + 1, color);
    // Select a question to confirm
    char* confirmQwestion = "";
    switch (ConfirmationCaller){
        case PD_2_BasicalItems:														
        case PD_16_HealerBuy:																		
        case PD_6_WitchBuy:																
        case PD_18_MagicalItems:		confirmQwestion = "Are you sure you want to buy this item?";		break;
        case PD_7_WitchSell:						
        case PD_3_BlacksmithSale:		confirmQwestion = "Are you sure you want to sell this item?";		break;
        case PD_4_RepairItem:			confirmQwestion = "Are you sure you want to have it repaired?";		break;
        case PD_13_WirtBuy:			confirmQwestion = "Do we have a deal?";								break;
        case PD_8_RechargeItem:		confirmQwestion = "Are you sure you want to recharge this item?";	break;
        case PD_17_IdentifyItem:		confirmQwestion = "Are you sure you want to identify this item?";	break;
    }
    // Выдача вопроса + вариантов ответа
    strcpy(InfoPanelBuffer, confirmQwestion);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_ConfirmQuestion, true, InfoPanelBuffer, C_0_White, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_Yes, true, "Yes", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_11_Confirm_No, true, "No", C_0_White, true);
}

//----- (0046E603) --------------------------------------------------------
void WirtMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Logo, true, "Wirt the Peg-legged boy", C_3_Gold, false);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    if( WirtItem.ItemCode == IC_M1_NONE ){
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_NoItem_Talk, true, "Talk to Wirt", C_1_Blue, true);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_NoItem_Leave, true, "Say goodbye", C_0_White, true);
        return;
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Talk, true, "Talk to Wirt", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel1, true, "I have something for sale,", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel2, true, "but it will cost 50 gold", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_ActionLabel3, true, "just to take a look. ", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_BuyItem, true, "What have you got?", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_12_WirtMain_Leave, true, "Say goodbye", C_0_White, true);
}

//----- (0046E6C2) --------------------------------------------------------
void WirtSellMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    // Настройки окна
    IsBigMenuBox = true;
    DialogHasScroolBox = false;
    sprintf(InfoPanelBuffer, "Let's see what I have got :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    // Выбор цвета
    char color = C_0_White;
    if( WirtItem.MagicLevel != ML_0_USUAL ){
        color = C_1_Blue;
    }
    if( !WirtItem.IsReqMet ){
        color = C_2_Red;
    }
    // Выбор имени
    char* name;
    if( WirtItem.MagicLevel != ML_0_USUAL ){
        name = WirtItem.FullMagicalItemName;
    }else{
        name = WirtItem.Name;
    }
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_13_WirtBuy_Item, false, name, color, true);
    SetItemPrice(PT_DialogRows_13_WirtBuy_Item, (int) (BuyPrice(WirtItem) * 1.5));
    PrintItemParamToTownsmenTradeDialog(&WirtItem, PT_DialogRows_13_WirtBuy_Item + 1, color);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Leave", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//----- (0046E799) --------------------------------------------------------
void PepinMainMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    if (GameMode != GM_COLISEUM /*&& GameMode != GM_CLASSIC*/) {
        if (player.CurLife != player.MaxCurLife) {// лечение
            PlayGlobalSound(S_88_CAST9);
        }
        player.CurLife = player.MaxCurLife;
        player.BaseLife = player.MaxBaseLife;
    }
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Logo2, true, "Healer Pepin", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Talk, true, "Talk to Pepin", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Trade, true, "Trade", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_14_HealerMain_Leave, true, "Say Goodbye", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//----- (0046E89E) --------------------------------------------------------
void __fastcall DrawBuyItemsFromPepin( int firstItem )
{
    int itermNum = firstItem;                                                          
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
    TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
    for( int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itermNum < DialogRowsCount; dialogRow += TradeRows){
        Item& item = PepinItems[itermNum];
        if( item.ItemCode != IC_M1_NONE ){
            char color = C_0_White;
            if( !item.IsReqMet ){
                color = C_2_Red;
            }
            SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, item.Name, color, true);
            SetItemPrice(dialogRow, BuyPrice(item));
            PrintItemParamToTownsmenTradeDialog(&item, dialogRow + 1, color);
            BottomSelectedRow = dialogRow;
            itermNum++;// косяк или фича. При встрече первой пустой вещи, дальше заполнять не будет
        }
    }
    if( !TownersDialogRows[SelectedDialogRow].selectable ){
        if( SelectedDialogRow != PT_DialogRows_BigDialog_Back ){
            SelectedDialogRow = BottomSelectedRow;
        }
    }
}

//----- (0046E94E) --------------------------------------------------------
void HealerPepinShop()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;
    DialogHasScroolBox = true;
    FirstVisibleItem = 0;
    sprintf(InfoPanelBuffer, "I have these items for sale :           Your gold : %i", player.TotalGold);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
    SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    DrawBuyItemsFromPepin(FirstVisibleItem);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
    for( DialogRowsCount = 0; DialogRowsCount < PT_HealerItemsCount; DialogRowsCount++){
        if( PepinItems[DialogRowsCount].ItemCode == IC_M1_NONE){
            break;
        }
    }
    ItemCount = DialogRowsCount - TradeRows;
    LimitToMin (ItemCount, 0);
}

//----- (0046EA15) --------------------------------------------------------
void TownElderMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Logo, true, "The Town Elder", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Talk, true, "Talk to Cain", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_IdentifyItems, true, "Identify items", C_0_White, true);
	if( CanTransmute() ){
    	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Transmute, true, "Transmute items", C_0_White, true);
	}
	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_15_ElderMain_Leave, true, "Say goodbye", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
}

//----- (0046EA8F) --------------------------------------------------------
bool __fastcall ItemNeedIdentify( const Item* itemPtr )
{
    const Item& item = *itemPtr;
    if( item.ItemCode != IC_M1_NONE && item.MagicLevel != ML_0_USUAL ){
        return !item.Identified;
    }
    return false;
}

//----- (0046EAA7) --------------------------------------------------------
void __fastcall AddItemToIdentifyList(char itemInventoryIndex, int edx_unused, const Item& item)
{
    Item& shopItem = PlayerItemsForSomeAction[DialogRowsCount];
    memcpy(&shopItem, &item, sizeof (Item));
    IndexOfPlayerItemInDialog[DialogRowsCount] = itemInventoryIndex;
    shopItem.basePrice = PT_IdentifyPrice;
    shopItem.Price = PT_IdentifyPrice;
	++DialogRowsCount;
}

int BodyItemIndexes[ 10 ] = { 0, 6, 4, 5, 1, 2, 3, 7, 8, 9 }; // для сохранения порядка при сверке
//----- (0046EAEB) --------------------------------------------------------
void IdentifyMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    Item tempItem;
    bool hasItemToIdentify = false;
    DialogRowsCount = 0;
    IsBigMenuBox = true;
    for( int itemIndex = 0; itemIndex < PT_48_PlayerItemsCount; itemIndex++ ){// Обнуление
        PlayerItemsForSomeAction[itemIndex].ItemCode = IC_M1_NONE;
    }
    for( int i = 0; i < IS_Inventory; ++i ){ // оптимизация. Было 7 проверок отдельных
        int bodyItemIndex = BodyItemIndexes[i];
		Item& item = player.OnBodySlots[bodyItemIndex];
        if( ItemNeedIdentify(&item) ){
            int itemIndex;
            switch (bodyItemIndex){
            case IS_Head:		itemIndex = PT_1_Identify_Head; break;
            case IS_Torso:	itemIndex = PT_2_Identify_Body; break;
            case IS_LeftHand:	itemIndex = PT_3_Identify_LeftHand; break;
            case IS_RightHand:itemIndex = PT_4_Identify_RightHand; break;
            case IS_RightRing:itemIndex = PT_5_Identify_RightRing; break;
            case IS_LeftRing:	itemIndex = PT_6_Identify_LeftRing; break;
            case IS_Neck:		itemIndex = PT_7_Identify_Neck; break;
			case IS_Waist:	itemIndex = PT_10_Identify_Waist; break;
			case IS_Hands:	itemIndex = PT_11_Identify_Hands; break;
			case IS_Feet:		itemIndex = PT_12_Identify_Feet; break;
            }
            hasItemToIdentify = true;
            memcpy(&tempItem, &item, sizeof(Item));
            AddItemToIdentifyList(itemIndex, 0, tempItem);
        }
    }
	// идентификация второго комплекта
	auto addAltWeapon = [&]( int altItemIndex, char inventoryItemIndex )
    {
	    LoadItem( &WeaponSwapItems[altItemIndex], &tempItem );
        tempItem.IsReqMet = CheckItemReq( CurrentPlayerIndex, &tempItem );
        if( ItemNeedIdentify( &tempItem ) ){
            hasItemToIdentify = true;
            AddItemToIdentifyList( inventoryItemIndex, 0, tempItem );
        }
    };
    
    addAltWeapon( AW_LeftHand, PT_8_Identify_SecondLeftHand );
    addAltWeapon( AW_RightHand, PT_9_Identify_SecondRightHand );
    addAltWeapon( AW_LeftHand_2, PT_Identify_SecondLeftHand_2 );
    addAltWeapon( AW_RightHand_2, PT_Identify_SecondRightHand_2 );
    
    for( int itemIndex = 0; itemIndex < player.InvItemCount && DialogRowsCount < PT_48_PlayerItemsCount; itemIndex++ ){// We pass on all things in the inventory
        Item& item = player.InventorySlots[itemIndex];
        if( ItemNeedIdentify(&item) ){
            hasItemToIdentify = true;
            memcpy(&tempItem, &item, sizeof (Item));
            AddItemToIdentifyList(itemIndex, 0, tempItem);
        }
    }
    if( hasItemToIdentify ){
        FirstVisibleItem = 0;
        DialogHasScroolBox = true;
        ItemCount = player.InvItemCount;
        sprintf(InfoPanelBuffer, "Identify which item?            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
        DrawPlayersItems(FirstVisibleItem, PD_17_IdentifyItem);
    }else{
        DialogHasScroolBox = false;
        sprintf(InfoPanelBuffer, "You have nothing to identify.            Your gold : %i", player.TotalGold);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
        SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
        SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, true);
    LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

//----- (0046EEE5) --------------------------------------------------------
void ItemIdentifiedMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ChangeTownerDialog(ConfirmationCaller);
    DialogHasScroolBox = false;
    ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
    // Выбор цвета
    int color = ItemColor(cursorItem);
    if( !cursorItem.IsReqMet ){
        color = C_2_Red;
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_20_IdentifiedItemInfo_Label, true, "This item is:", C_0_White, false);
    SetDialogRow(PT_DialogRows_SmallIndent, PT_DialogRows_20_IdentifiedItemInfo_Item, false, cursorItem.FullMagicalItemName, color, false);
    PrintItemParamToTownsmenTradeDialog(&cursorItem, PT_DialogRows_20_IdentifiedItemInfo_Item + 1, color);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_20_IdentifiedItemInfo_Done, true, "Done", C_0_White, true);
}

//----- (0046EFAC) --------------------------------------------------------
void TalkToMenu()
{    
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    sprintf(InfoPanelBuffer, "Talk to %s", TownersNames[CurrentTowner]);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_19_Talk_Label, true, InfoPanelBuffer, C_3_Gold, false);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    size_t countOfActiveQuests = 0;
    for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1 && quest.talkToFlag ){
                ++countOfActiveQuests;
        }
    }
    int topQwestRow;   
    int interval;  
    if( countOfActiveQuests <= 6 ){
        topQwestRow = 15 - countOfActiveQuests;
        interval = 2;
    }else{
        topQwestRow = 14 - (countOfActiveQuests / 2);
        interval = 1;
    }
    int gossipRow = topQwestRow - 2;
    for( int questIndex = 0; questIndex < Quest_Count; questIndex++){
        if( Quests[questIndex].status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1
            && Quests[questIndex].talkToFlag ){
                SetDialogRow(PT_DialogRows_NoIndent, topQwestRow, true, BaseQuests[questIndex].questName, C_0_White, true);
                topQwestRow += interval;
        }
    }

    SetDialogRow(PT_DialogRows_NoIndent, gossipRow, true, "Gossip", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_19_Talk_Back, true, "Back", C_0_White, true);
}

//----- (0046F0D6) --------------------------------------------------------
void OgdenMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Logo1, true, "Welcome to the", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Logo2, true, "Rising Sun", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Talk, true, "Talk to Ogden", C_1_Blue, true);
    
    {
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( theRampagingDemonQuest.status == QS_2_IN_PROGRESS )
        {
            if( theRampagingDemonQuest.status3 == 0 ){
				SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_TravelToRavenholm, true, "Travel to Ravenholm", C_1_Blue, true);
			}
            //SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_TravelToTheDen, true, "Travel to the Den", C_1_Blue, true);
        }
    }
    
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_21_OgdenMain_Leave, true, "Leave the tavern", C_0_White, true);
	SetDialogLine( PT_DialogRows_5_SmallDialog_Separator );
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//----- (th3) -------------------------------------------------------------
bool StashEnabled()
{
	return not(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE);
}

//----- (0046F15A) --------------------------------------------------------
void GillianMainMenu()
{
	IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_2_Logo, true, "Gillian", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_9_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_12_Talk, true, "Talk to Gillian", C_1_Blue, true);
    if( StashEnabled() ){
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_14_Stash, true, "Open stash", C_0_White, true);
    }
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_GilianMain_18_Leave, true, "Say goodbye", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

//----- (0046F1CC) --------------------------------------------------------
void FarnhamMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Logo, true, "Farnham the Drunk", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Talk, true, "Talk to Farnham", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_22_FarnhamMain_Leave, true, "Say Goodbye", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

void ColiseumMainMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumMain_Logo, true, "Coliseum", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumMain_ActionLabel, true, "Would you like to:", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumMain_Talk, true, "Fight on the arena", C_1_Blue, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumMain_Leave, true, "Say Goodbye", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

void ColiseumDifficultyMenu()
{
    IsBigMenuBox = false;
    DialogHasScroolBox = false;
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_Logo, true, "Coliseum", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_ActionLabel, true, "Ready?", C_3_Gold, false);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_5, true, "Step Inside", C_0_White, true);
   /* SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_1, true, "1 - 5", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_2, true, "6 - 10", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_3, true, "11 - 15", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_4, true, "16 - 20", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_5, true, "21 - 25", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_6, true, "26 - 30", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_7, true, "31 - 35", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_8, true, "36 - 40", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_9, true, "41 - 45", C_0_White, true);
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_10, true,"46 - 50", C_0_White, true);*/
    SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_ColiseumDiff_Leave, true, "Back", C_0_White, true);
    SetDialogLine(PT_DialogRows_5_SmallDialog_Separator);
    DialogRowsCount = PT_DialogRows_20_SmallDialog_RowsCount;
}

int DialogBackIndex = -1;
//----- (0046F23E) --------------------------------------------------------
void __fastcall ChangeTownerDialog(PLAYER_DIALOG dialogIndex)
{
	IsSpellBookVisible = false;
	IsINVPanelVisible = false;
	IsStashPanelVisible = false;
	VisualTrade_Close();
	Craft_Close();
	// Enchant_Close();
	IsPerksPanelVisible = false;
	IsCHARPanelVisible = false;
	IsQUESTPanelVisible = false;
	IsGoldSplitPanelVisible = false;
	IsInfoWindowVisible = false;
	ClearRowsInInterval(0, PT_DialogRowsCount);
	Dialog_MouseClick();
	switch( dialogIndex ){
	case PD_1_BlacksmithMain           : GriswoldMainMenu();                             break;
	case PD_2_BasicalItems             : if( DialogRowsCount > 0 ) BasicalItemBuyMenu(); break;
	case PD_3_BlacksmithSale           : GriswoldSellMenu();                             break;
	case PD_4_RepairItem               : RepairItemMenu();                               break;
	case PD_5_WitchMain                : WitchAdriaMenu();                               break;
	case PD_6_WitchBuy                 : if( DialogRowsCount > 0 ) WitchAdriaStore();    break;
	case PD_7_WitchSell                : WitchAdriaSellMenu();                           break;
	case PD_8_RechargeItem             : RechargeItemMenu();                             break;
	case PD_9_NoGold                   : NoGold();                                       break;
	case PD_10_NoRoom                  : NoRoom();                                       break;
	case PD_11_Confirm                 : ConfirmActionMenu();                            break;
	case PD_12_WirtMain                : WirtMainMenu();                                 break;
	case PD_13_WirtBuy                 : WirtSellMenu();                                 break;
	case PD_14_HealerMain              : PepinMainMenu();                                break;
	case PD_15_ElderMain               : TownElderMainMenu();                            break;
	case PD_16_HealerBuy               : if( DialogRowsCount > 0 ) HealerPepinShop();    break;
	case PD_17_IdentifyItem            : IdentifyMenu();                                 break;
	case PD_18_MagicalItems            : if( ! MagicItemsBuyMenu() ) return;             break;
	case PD_19_Talk                    : TalkToMenu();                                   break;
	case PD_20_IdentifiedItemInfo      : ItemIdentifiedMenu();                           break;
	case PD_21_OgdenMain               : OgdenMainMenu();                                break;
	case PD_22_FarnhamMain             : FarnhamMainMenu();                              break;
	case PD_23_GilianMain              : GillianMainMenu();                              break;
	case PD_27_BlacksmithBasicalRestock:
	case PD_24_BlacksmithMagicalRestock: GriswoldRestockMenu(dialogIndex);               break;
	case PD_25_MessageLog              : ShowMessageLog(FirstVisibleItem);               break;
	case PD_26_AdriaRestock            : AdriaRestockMenu();                             break;
	case PD_28_ColiseumMain            : ColiseumMainMenu();                             break;
	case PD_29_ColiseumDiff            : ColiseumDifficultyMenu();                       break;
    }

    int selectedDialogRow;
	DialogBackIndex = -1;
    for( selectedDialogRow = 0; selectedDialogRow < PT_DialogRowsCount; ++selectedDialogRow ){
        if( TownersDialogRows[selectedDialogRow].selectable ){
            break;
        }
		if( !strcmp(TownersDialogRows[selectedDialogRow].text, "Back") ){
			DialogBackIndex = selectedDialogRow;
		}
    }

    if( selectedDialogRow == PT_DialogRowsCount ){
		SelectedDialogRow = DialogBackIndex != -1 ? DialogBackIndex : -1;
    }else{
        SelectedDialogRow = selectedDialogRow;
		DialogBackIndex = -1;
    }

    CurrentDialogIndex = dialogIndex;
    if( dialogIndex == PD_2_BasicalItems && !DialogRowsCount ){
        ChangeTownerDialog(PD_1_BlacksmithMain);// оптимизировал. В оригинале было циклом некрасивым
    }
}

//----- (0046F3FF) --------------------------------------------------------
void DrawTownerDialog()
{
    if( IsBigMenuBox ){// Выбор размера бокса
		DrawDialogBox( Dialog_591_Width, Dialog_302_Height, TextBoxCel );
    }else{
		DrawDialogBox( Dialog_HalfWidth, Dialog_302_Height, TextBox2CELPtr, 344, 327 );
    }
    if( DialogHasScroolBox ){// Если это список вещей - нарисовать их
        switch( CurrentDialogIndex ){
            case PD_2_BasicalItems:	DrawBuyBasicalItems		(FirstVisibleItem); break;
            case PD_3_BlacksmithSale:
            case PD_4_RepairItem:
            case PD_7_WitchSell:
            case PD_8_RechargeItem:
            case PD_17_IdentifyItem:    DrawPlayersItems(FirstVisibleItem, CurrentDialogIndex); break;
            case PD_6_WitchBuy:		DrawAdriaItems			(FirstVisibleItem); break;
            case PD_16_HealerBuy:	DrawBuyItemsFromPepin	(FirstVisibleItem); break;
            case PD_18_MagicalItems:	DrawBuyMagicalItems		(FirstVisibleItem); break;
			case PD_25_MessageLog:	ShowMessageLog(FirstVisibleItem);  break;
        }
    }
    for( int rowIndex = 0; rowIndex < PT_DialogRowsCount; rowIndex++ ){
        TownersDialogRow& currentDlgRow = TownersDialogRows[rowIndex];
        if( currentDlgRow.isLine ){// Если это линия, рисуем её
            DrawDialogLine(rowIndex);
        }
        if( *currentDlgRow.text ){// Если есть текст, пишем его
            DrawGameDialogTitleText(currentDlgRow.position, rowIndex, currentDlgRow.isAligned, currentDlgRow.text, currentDlgRow.color, currentDlgRow.price);
        }
    }
    if( DialogHasScroolBox ){// Если есть скрол, рисуем его
        DrawDialogScroolBox(4, 20);
    }
    if( IsMainDraw ) CursorPentAnimationNumber = (CursorPentAnimationNumber & 7) + 1;
}

//----- (0046F4FB) --------------------------------------------------------
void PrevTownerDialog()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( Dungeon->genType == DT_0_TOWN ){
            StopSpeech();
        }
        return;
    }
    switch( CurrentDialogIndex ){
        case PD_1_BlacksmithMain:
        case PD_5_WitchMain:
        case PD_12_WirtMain:
        case PD_13_WirtBuy:
        case PD_14_HealerMain:
        case PD_15_ElderMain:
        case PD_21_OgdenMain:
        case PD_22_FarnhamMain:
			CurrentDialogIndex = PD_0_None;
			break;
        case PD_23_GilianMain:
			CurrentDialogIndex = PD_0_None;
			break;
        case PD_19_Talk:
            ChangeTownerDialog(ConfirmationCaller);
            SelectedDialogRow = SelectedDialogRowInPrevDialog;
            break;
        case PD_2_BasicalItems:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            break;
        case PD_18_MagicalItems:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            break;
        case PD_3_BlacksmithSale:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            break;
        case PD_4_RepairItem:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            break;
        case PD_27_BlacksmithBasicalRestock:
            ChangeTownerDialog(PD_1_BlacksmithMain);
            break;
		case PD_24_BlacksmithMagicalRestock:
			ChangeTownerDialog(PD_1_BlacksmithMain);
			break;
		case PD_25_MessageLog:
			SelectedDialogRow = PT_SomeDialogRowForMessageLog;
			break;
        case PD_6_WitchBuy:
            ChangeTownerDialog(PD_5_WitchMain);
            break;
        case PD_7_WitchSell:
            ChangeTownerDialog(PD_5_WitchMain);
            break;
        case PD_8_RechargeItem:
            ChangeTownerDialog(PD_5_WitchMain);
            break;
        case PD_26_AdriaRestock:
            ChangeTownerDialog(PD_5_WitchMain);
            break;
        case PD_16_HealerBuy:
            ChangeTownerDialog(PD_14_HealerMain);
            SelectedDialogRow = PT_DialogRows_14_HealerMain_Leave;
            break;
        case PD_17_IdentifyItem:
            ChangeTownerDialog(PD_15_ElderMain);
            SelectedDialogRow = PT_DialogRows_15_ElderMain_IdentifyItems;
            break;
        case PD_20_IdentifiedItemInfo:
            ChangeTownerDialog(PD_17_IdentifyItem);
            break;
        case PD_9_NoGold:
        case PD_10_NoRoom:
        case PD_11_Confirm:
            ChangeTownerDialog(ConfirmationCaller);
            SelectedDialogRow = SelectedDialogRowInPrevDialog;
            FirstVisibleItem = ScrolledItemsCountInPrevDialog;
            break;
        case PD_28_ColiseumMain:
            CurrentDialogIndex = PD_0_None;
            break;
        case PD_29_ColiseumDiff:
            ChangeTownerDialog(ConfirmationCaller);
            SelectedDialogRow = SelectedDialogRowInPrevDialog;
            break;
    }
}

//----- (0046F630) --------------------------------------------------------
void DialogUp()
{
    PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 ){
        return;
    }
    if( DialogHasScroolBox ){
        if( SelectedDialogRow == TopSelectedRow ){
            if( FirstVisibleItem ){
                --FirstVisibleItem;
            }
            return;
        }
        --SelectedDialogRow;
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow ){
                --SelectedDialogRow;
            }else{
                SelectedDialogRow = PT_DialogRowsCount - 1;
            }
        }
    }else{
        if( SelectedDialogRow ){
            --SelectedDialogRow;
        }else{
            SelectedDialogRow = PT_DialogRowsCount - 1;
        }
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow ){
                --SelectedDialogRow;
            }else{
                SelectedDialogRow = PT_DialogRowsCount - 1;
            }
        }
    }
}

//----- (0046F6D8) --------------------------------------------------------
void DialogDown()
{
    PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 ){
        return;
    }
    if( DialogHasScroolBox ){
        if( SelectedDialogRow == BottomSelectedRow ){
            if( FirstVisibleItem < ItemCount ){
                ++FirstVisibleItem;
            }
            return;
        }
        ++SelectedDialogRow;
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
                SelectedDialogRow = 0;
            }else{
                ++SelectedDialogRow;
            }
        }
    }else{
        if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
            SelectedDialogRow = 0;
        }else{
            ++SelectedDialogRow;
        }
        while( !TownersDialogRows[SelectedDialogRow].selectable ){
            if( SelectedDialogRow == PT_DialogRowsCount - 1 ){
                SelectedDialogRow = 0;
            }else{
                ++SelectedDialogRow;
            }
        }
    }
}

//----- (0046F785) --------------------------------------------------------
int DialogPageUp()
{
    int prevFirst = FirstVisibleItem;
	int prevSelected = SelectedDialogRow;
	PlayGlobalSound( S_75_I_TITLEMOV );
	if( SelectedDialogRow == -1 || DialogBackIndex != -1 || !DialogHasScroolBox ){
        return 0;
    }
    if( SelectedDialogRow == TopSelectedRow ){
        if( FirstVisibleItem ){
            FirstVisibleItem -= 4;
            LimitToMin(FirstVisibleItem, 0);
        } 
    }else{
        SelectedDialogRow = TopSelectedRow;
    }
	return prevFirst != FirstVisibleItem || prevSelected != SelectedDialogRow;
}

//----- (0046F7CF) --------------------------------------------------------
int DialogPageDown()
{
    int prevFirst = FirstVisibleItem;
	int prevSelected = SelectedDialogRow;
	PlayGlobalSound(S_75_I_TITLEMOV);
    if( SelectedDialogRow == -1 || DialogBackIndex != -1 || !DialogHasScroolBox ){
        return 0;
    }
    if( SelectedDialogRow == BottomSelectedRow ){
        if( FirstVisibleItem < ItemCount ){
            FirstVisibleItem += TradeRows;
        }
        LimitToMax(FirstVisibleItem, ItemCount);
    }else{
        SelectedDialogRow = BottomSelectedRow;
    }
	return prevFirst != FirstVisibleItem || prevSelected != SelectedDialogRow;
}

//----- (0046F893) --------------------------------------------------------
void __fastcall ChangeGoldGraphicOnQuantity( int playerIndex, int itemIndex )
{
    Player& player = Players[playerIndex];
    Item& item = player.InventorySlots[itemIndex];
    int pictureIndex;
    if( item.amount >= Gold_2500_HiLimit ){
        pictureIndex = Gold_6_BigPic;
    }else if( item.amount > Gold_1000_LowLimit ){
        pictureIndex = Gold_5_MediumPic;
    }else{
        pictureIndex = Gold_4_SmallPic;
    }
    item.GraphicValue = pictureIndex;
}

//----- (0046F8E0) --------------------------------------------------------
void __fastcall ChangeGraphValueInBelt( int playerIndex, int itemIndex )
{
    Player& player = Players[playerIndex];
    Item& item = player.BeltInventory[itemIndex];
    int pictureIndex;
    if( item.amount >= Gold_2500_HiLimit ){
        pictureIndex = Gold_6_BigPic;
    }else if( item.amount > Gold_1000_LowLimit ){
        pictureIndex = Gold_5_MediumPic;
    }else{
        pictureIndex = Gold_4_SmallPic;
    }
    item.GraphicValue = pictureIndex;
}

//----- (0046F92D) --------------------------------------------------------
void __fastcall ClearGoldByInventoryAsPrice( int price )
{
    Player& player = Players[CurrentPlayerIndex];
    player.TotalGold = CalcTotalGold(CurrentPlayerIndex) - price;

    int priceLeft = price;

    // Первый проход - вычитаем из неполных кучек пояса
    for( int i = 0; i < IS_8_Belt_Count && priceLeft > 0; ++i ){
        Item& item = player.BeltInventory[i];
        if( item.ItemCode == IC_11_GOLD ){
            if( item.amount != MaxGoldInCell ){
                if( priceLeft >= item.amount ){
                    priceLeft -= item.amount;
                    ClearBeltSlot(CurrentPlayerIndex, i);
                    i = -1; // reset loop
                }else{
                    item.amount -= priceLeft;
                    ChangeGraphValueInBelt(CurrentPlayerIndex, i);
                    priceLeft = 0;
                }
            }
        }
    }

    // Второй проход, из оставшихся кучек пояса
    for( int i = 0; i < IS_8_Belt_Count && priceLeft > 0; ++i ){
        Item& item = player.BeltInventory[i];
        if( item.ItemCode == IC_11_GOLD ){
			if( priceLeft >= item.amount ){
				priceLeft -= item.amount;
				ClearBeltSlot(CurrentPlayerIndex, i);
				i = -1; // reset loop
			}else{
				item.amount -= priceLeft;
                ChangeGraphValueInBelt(CurrentPlayerIndex, i);
                priceLeft = 0;
            }
        }
    }

    // Третий проход - вычитаем из неполных кучек инвентаря
    for( int i = 0; i < player.InvItemCount && priceLeft > 0; ++i ){
        Item& item = player.InventorySlots[i];
        if( item.ItemCode == IC_11_GOLD ){
			if( item.amount != MaxGoldInCell ){
				if( priceLeft >= item.amount ){
					priceLeft -= item.amount;
					ClearInventorySlot(CurrentPlayerIndex, i);
					i = -1; // reset loop
				}else{
					item.amount -= priceLeft;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                    priceLeft = 0;
                }
            }
        }
    }

    // Четвертый проход, из оставшихся кучек инвентаря
    for( int i = 0; i < player.InvItemCount && priceLeft > 0; ++i ){
        Item& item = player.InventorySlots[i];
        if( item.ItemCode == IC_11_GOLD ){
			if( priceLeft >= item.amount ){
				priceLeft -= item.amount;
				ClearInventorySlot(CurrentPlayerIndex, i);
				i = -1; // reset loop
			}else{
				item.amount -= priceLeft;
                ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                priceLeft = 0;
            }
        }
    }
}

//----- (0046FB68) --------------------------------------------------------
void BuyBasicalItemFromGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ClearGoldByInventoryAsPrice(BuyPrice(cursorItem));
    if( cursorItem.MagicLevel == ML_0_USUAL ){
        cursorItem.Identified = false;
    }
    AutoPutCursorToInventoryOrDrop();
    // удаляем вещь из середины массива со сдвигом всех вещей
    int itemIndex = GET_ITEM_INDEX;
    if( itemIndex == PT_BasicItemsCount - 1 ){
        BasicalItemsArray[PT_BasicItemsCount - 1].ItemCode = IC_M1_NONE;
    }else{
        while( BasicalItemsArray[itemIndex + 1].ItemCode != IC_M1_NONE ){
            Item& curBasicalItem = BasicalItemsArray[itemIndex];
            Item& nextBasicalItem = BasicalItemsArray[itemIndex + 1];
            memcpy(&curBasicalItem, &nextBasicalItem, sizeof(Item));
            ++itemIndex;
        }
        BasicalItemsArray[itemIndex].ItemCode = IC_M1_NONE;
    }
    VisualTrade_RefreshPage( VTP_BlacksmithBasic );
    
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (0046FD18) --------------------------------------------------------
void BuyMagicalItemFromGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    ClearGoldByInventoryAsPrice(BuyPrice(cursorItem));
    if( cursorItem.MagicLevel == ML_0_USUAL ){
        cursorItem.Identified = false;
    }
    AutoPutCursorToInventoryOrDrop();

    int itemIndex = GET_ITEM_INDEX;
    int lastValidItem = 0;
    for( int i = itemIndex, j = 0; i >= 0; ++j ){
        if( MagicItemsStoreArray[j].ItemCode != IC_M1_NONE ){
            --i;
            lastValidItem = j;
        }
    }

    MagicItemsStoreArray[lastValidItem].ItemCode = IC_M1_NONE;
    --GriswoldMagicItemCount;
    MagicItemsGeneration(CurrentPlayerIndex);
}

//----- (0046FFA1) --------------------------------------------------------
void __fastcall PutOneGoldPileInInventory(int amount)
{
    Player& player = Players[CurrentPlayerIndex];
    for( int y = InvRows - 1; y >= 0; --y ){
        for( int x = 0, xe = player.invCols; x < xe; ++x ){
            if( !player.InvUsedAlt[y][x] ){
                int lastItemIndex = player.InvItemCount;
                Item& lastItem = player.InventorySlots[lastItemIndex];
                Item_MakeNonCollisionSeed(CurrentPlayerIndex, ItemGoldBuffer);
                memcpy(&lastItem, &ItemGoldBuffer, sizeof(Item));
                ++player.InvItemCount;
                player.InvUsedAlt[y][x] = player.InvItemCount;
                lastItem.amount = amount;
                ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, lastItemIndex);
                return;
            }
        }
    }
	if( ItemsCount < MaxItems_255 && CreateTypeItem(player.Row, player.Col, 0, IC_11_GOLD, 0, 0, 1) != -1 ){
		Items[ ItemActive[ ItemsCount - 1 ] ].amount = amount;
	}
}

//----- (004700BD) --------------------------------------------------------
void SellItem()
{
    Player& player = Players[CurrentPlayerIndex];
    int itemIndex = GET_ITEM_INDEX;
    int itemIndexInDialog = IndexOfPlayerItemInDialog[itemIndex];
    if( itemIndexInDialog < 0 ){
        ClearBeltSlot(CurrentPlayerIndex, -( itemIndexInDialog + 1 ) );
    }else{
        ClearInventorySlot(CurrentPlayerIndex, itemIndexInDialog);
    }

    --DialogRowsCount;

    int itemPrice = SellPrice(PlayerItemsForSomeAction[GET_ITEM_INDEX]);
    
    for( ; itemIndex < DialogRowsCount; ++itemIndex ){
        memcpy(&PlayerItemsForSomeAction[itemIndex], &PlayerItemsForSomeAction[itemIndex + 1], sizeof (Item));
        IndexOfPlayerItemInDialog[itemIndex] = IndexOfPlayerItemInDialog[itemIndex + 1];
    }

    player.TotalGold += itemPrice;
    for( int i = 0; i < player.InvItemCount && itemPrice > 0; i++ ){
        Item& inventoryItem = player.InventorySlots[i];
        if( inventoryItem.ItemCode == IC_11_GOLD ){
            if( inventoryItem.amount != MaxGoldInCell ){
                int goldSum = itemPrice + inventoryItem.amount;
                if( goldSum > MaxGoldInCell ){
                    inventoryItem.amount = MaxGoldInCell;
                    itemPrice = goldSum - MaxGoldInCell;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                }else{
                    inventoryItem.amount = goldSum;
                    ChangeGoldGraphicOnQuantity(CurrentPlayerIndex, i);
                    itemPrice = 0;
                }
            }
        }
    }

    if( itemPrice > 0 ){
        while( itemPrice > MaxGoldInCell ){
            PutOneGoldPileInInventory(MaxGoldInCell);
            itemPrice -= MaxGoldInCell;
        }
        PutOneGoldPileInInventory(itemPrice);
    }
}

//----- (004702C1) --------------------------------------------------------
void RepairItemByGriswold()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    int itemIndex = GET_ITEM_INDEX;
    int indexOfRepairedItem = IndexOfPlayerItemInDialog[itemIndex];
    PlayerItemsForSomeAction[itemIndex].CurDurability  = PlayerItemsForSomeAction[itemIndex].BaseDurability;
    Item* repairedItem = nullptr;
    LastItemInfo* repairedSwapItem = nullptr;
    if( indexOfRepairedItem >= 0 ){
        repairedItem = &player.InventorySlots[indexOfRepairedItem];
    }else{
		switch( indexOfRepairedItem ){
		case PT_Repair_Head:      repairedItem = &player.OnBodySlots[ IS_Head ];      break;
		case PT_Repair_Body:      repairedItem = &player.OnBodySlots[ IS_Torso ];		break;
		case PT_Repair_LeftHand:  repairedItem = &player.OnBodySlots[ IS_LeftHand ];	break;
		case PT_Repair_RightHand: repairedItem = &player.OnBodySlots[ IS_RightHand ];	break;
		case PT_Repair_Waist:     repairedItem = &player.OnBodySlots[ IS_Waist ];		break;
		case PT_Repair_Hands:     repairedItem = &player.OnBodySlots[ IS_Hands ];		break;
		case PT_Repair_Feet:      repairedItem = &player.OnBodySlots[ IS_Feet ];		break;
		case PT_Repair_SecondLeftHand:  repairedSwapItem = &WeaponSwapItems[AW_LeftHand]; break;
		case PT_Repair_SecondRightHand: repairedSwapItem = &WeaponSwapItems[AW_RightHand]; break;
		case PT_Repair_SecondLeftHand_2:  repairedSwapItem = &WeaponSwapItems[AW_LeftHand_2]; break;
		case PT_Repair_SecondRightHand_2: repairedSwapItem = &WeaponSwapItems[AW_RightHand_2]; break;
		}
	}
	if( repairedItem ){
        repairedItem->CurDurability = repairedItem->BaseDurability;
    }else if( repairedSwapItem ){
        repairedSwapItem->curDurability = repairedSwapItem->baseDurability;
    }else{
        //Assert( false );
    }
}

//----- (004704E2) --------------------------------------------------------
void BuyItemFromAdria()
{
    Player& player = Players[CurrentPlayerIndex];
    int witchItemIndex = GET_ITEM_INDEX;
    if( witchItemIndex < PT_Witch_ForcedItemsCount ){
        player.ItemOnCursor.MakeSeed();
    }
    ClearGoldByInventoryAsPrice(BuyPrice(player.ItemOnCursor));
    AutoPutCursorToInventoryOrDrop();
    if( witchItemIndex >= PT_Witch_ForcedItemsCount){ 
        if( witchItemIndex < PT_25_WitchItemsCount - 1 ){
            for( ; WitchAdriaItems[witchItemIndex + 1].ItemCode != IC_M1_NONE; ++witchItemIndex ){
                Item& curWitchItem = WitchAdriaItems[witchItemIndex];
                Item& nextWitchItem = WitchAdriaItems[witchItemIndex + 1];
                memcpy(&curWitchItem, &nextWitchItem, sizeof (Item));
            }
        }
        WitchAdriaItems[witchItemIndex].ItemCode = IC_M1_NONE;
    }
    VisualTrade_RefreshPage(VTP_WitchBasic);
    RecalcPlayer(CurrentPlayerIndex, true);
}

//----- (00470721) --------------------------------------------------------
void RechargeStaffByAdria()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(player.ItemOnCursor.Price);
    int itemIndex = GET_ITEM_INDEX;
    int rechargedItemIndex = IndexOfPlayerItemInDialog[itemIndex];
    PlayerItemsForSomeAction[itemIndex].CurCharges = PlayerItemsForSomeAction[itemIndex].BaseCharges;
    if( rechargedItemIndex >= 0 ){
        player.InventorySlots[rechargedItemIndex].CurCharges = player.InventorySlots[rechargedItemIndex].BaseCharges;
    }else{
        Item* rechargedItem = nullptr;
        LastItemInfo* rechargedSwapItem = nullptr;
        switch( rechargedItemIndex ){
		case PT_Recharge_Head:              rechargedItem = &player.OnBodySlots[ IS_Head ]; break;
		case PT_Recharge_LeftRing:          rechargedItem = &player.OnBodySlots[ IS_LeftRing ]; break;
		case PT_Recharge_RightRing:         rechargedItem = &player.OnBodySlots[ IS_RightRing ]; break;
		case PT_Recharge_Neck:              rechargedItem = &player.OnBodySlots[ IS_Neck ]; break;
		case PT_Recharge_LeftHand:          rechargedItem = &player.OnBodySlots[ IS_LeftHand ]; break;
		case PT_Recharge_RightHand:         rechargedItem = &player.OnBodySlots[ IS_RightHand ]; break;
		case PT_Recharge_Torso:             rechargedItem = &player.OnBodySlots[ IS_Torso ]; break;
		case PT_Recharge_Waist:             rechargedItem = &player.OnBodySlots[ IS_Waist ]; break;
		case PT_Recharge_Hands:             rechargedItem = &player.OnBodySlots[ IS_Hands ]; break;
		case PT_Recharge_Feet:              rechargedItem = &player.OnBodySlots[ IS_Feet ]; break;
		case PT_Recharge_SecondLeftHand:    rechargedSwapItem = &WeaponSwapItems[ AW_LeftHand ]; break;
		case PT_Recharge_SecondRightHand:   rechargedSwapItem = &WeaponSwapItems[ AW_RightHand ]; break;
		case PT_Recharge_SecondLeftHand_2:  rechargedSwapItem = &WeaponSwapItems[ AW_LeftHand_2 ]; break;
		case PT_Recharge_SecondRightHand_2: rechargedSwapItem = &WeaponSwapItems[ AW_RightHand_2 ]; break;
		}
		if( rechargedItem ){
            rechargedItem->CurCharges = rechargedItem->BaseCharges;
        }else if( rechargedSwapItem ){
            rechargedSwapItem->curCharges = rechargedSwapItem->baseCharges;
        }else{
            //Assert( false );
        }
    }
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (00470912) --------------------------------------------------------
void BuyItemFromWirt()
{
    Player& player = Players[CurrentPlayerIndex];
    ClearGoldByInventoryAsPrice(BuyPrice(player.ItemOnCursor));
    AutoPutCursorToInventoryOrDrop();
    WirtItem.ItemCode = IC_M1_NONE;
    ConfirmationCaller = PD_12_WirtMain;
    VisualTrade_RefreshPage(VTP_GambleBasic);
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (00470953) --------------------------------------------------------
void BuyItemFromPepin()
{
	Player& player = Players[ CurrentPlayerIndex ];
	Item& cursorItem = player.ItemOnCursor;

	int	healerItemIndex = GET_ITEM_INDEX;

	int countOfForsedItems;
	if( MaxCountOfPlayersInGame == 1 ){
		countOfForsedItems = PT_2_Healer_Singleplayer_ForcedItemsCount;
	}else{
		countOfForsedItems = PT_3_Healer_Multiplayer_ForcedItemsCount;
	}

	if( healerItemIndex < countOfForsedItems ){
		cursorItem.MakeSeed();
	}
	ClearGoldByInventoryAsPrice(BuyPrice(player.ItemOnCursor));
	if( cursorItem.MagicLevel == ML_0_USUAL ){
		cursorItem.Identified = false;
	}
	AutoPutCursorToInventoryOrDrop();

	if( healerItemIndex >= countOfForsedItems ){
		if( healerItemIndex < PT_HealerItemsCount - 1 ){
			for( ; PepinItems[ healerItemIndex + 1 ].ItemCode != IC_M1_NONE; ++healerItemIndex ){
				Item& curHealerItem = PepinItems[ healerItemIndex ];
				Item& nextHealerItem = PepinItems[ healerItemIndex + 1 ];
				memcpy( &curHealerItem, &nextHealerItem, sizeof( Item ) );
			}
		}
		PepinItems[ healerItemIndex ].ItemCode = IC_M1_NONE;
		RecalcPlayer( CurrentPlayerIndex, 1 );
	}
	
	VisualTrade_RefreshPage(VTP_HealerBasic);
}

//----- (00470B6E) --------------------------------------------------------
void IdentifyItemByKain()
{
    Player& player = Players[CurrentPlayerIndex];
    int itemIndex = GET_ITEM_INDEX;
    
    int identifyItemIndex = IndexOfPlayerItemInDialog[itemIndex]; 
    Item* identifyItem = nullptr;
    LastItemInfo* identifySwapItem = nullptr;
    if( identifyItemIndex >= 0 ){
        identifyItem = &player.InventorySlots[identifyItemIndex];
    }else{
        Item* onBodySlots = player.OnBodySlots;
        switch( identifyItemIndex ){
		case PT_1_Identify_Head:		identifyItem = &onBodySlots[IS_Head];      break;
		case PT_2_Identify_Body:		identifyItem = &onBodySlots[IS_Torso];	 break;
		case PT_3_Identify_LeftHand:	identifyItem = &onBodySlots[IS_LeftHand];	 break;
		case PT_4_Identify_RightHand:	identifyItem = &onBodySlots[IS_RightHand]; break;
		case PT_6_Identify_LeftRing:	identifyItem = &onBodySlots[IS_LeftRing];	 break;
		case PT_5_Identify_RightRing:	identifyItem = &onBodySlots[IS_RightRing]; break;
		case PT_7_Identify_Neck:		identifyItem = &onBodySlots[IS_Neck];		 break;
		case PT_10_Identify_Waist:		identifyItem = &onBodySlots[IS_Waist];	 break;
		case PT_11_Identify_Hands:		identifyItem = &onBodySlots[IS_Hands];	 break;
		case PT_12_Identify_Feet:		identifyItem = &onBodySlots[IS_Feet];		 break;
		case PT_8_Identify_SecondLeftHand:	identifySwapItem = &WeaponSwapItems[AW_LeftHand]; break;
		case PT_9_Identify_SecondRightHand: identifySwapItem = &WeaponSwapItems[AW_RightHand]; break;
		case PT_Identify_SecondLeftHand_2:	identifySwapItem = &WeaponSwapItems[AW_LeftHand_2]; break;
		case PT_Identify_SecondRightHand_2: identifySwapItem = &WeaponSwapItems[AW_RightHand_2]; break;
		}
    }
    if( identifyItem ){
        identifyItem->Identified = true;
    }else if( identifySwapItem ){
        identifySwapItem->magicLevelAndIdentified |= 1;
    }else{
        // Assert(false);
    }
    
    player.ItemOnCursor.Identified = true;
    ClearGoldByInventoryAsPrice(BuyPrice(player.ItemOnCursor));
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//!---- (004711D0) --------------------------------------------------------
void HandleGriswoldMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_1_BlacksmithMain_Talk:
            CurrentTowner = TI_0_GRISWOLD;
            SelectedDialogRowInPrevDialog = PT_DialogRows_1_BlacksmithMain_Talk;
            ConfirmationCaller = PD_1_BlacksmithMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_189;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_199;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_1_BlacksmithMain_Trade: VisualTrade_Open( VTM_Blacksmith ); break;
		case PT_DialogRows_1_BlacksmithMain_Craft: if( CanGrind() ) Craft_Open();      break;
		case PT_DialogRows_1_BlacksmithMain_Quench: Quench_Open();                     break;
        case PT_DialogRows_1_BlacksmithMain_Leave: CurrentDialogIndex = PD_0_None;	   break;
    }
}

//!---- (004711D5) --------------------------------------------------------
void HandleBasicalItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_2_BasicalItems;
    Item &selectedItem = BasicalItemsArray[GET_SELECTED_ITEM_INDEX];
    if (player.TotalGold < BuyPrice(selectedItem)) {
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &selectedItem, sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; ++i ){
        canPutInventoryItem = AutoPutCursorToCell(player, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//!---- (004711DA) --------------------------------------------------------
void HandleMagicalItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_18_MagicalItems;

    int itemCurIndex = 0;
    int i = GET_SELECTED_ITEM_INDEX;
    for( int itemNextIndex = 0; i >= 0; ++itemNextIndex ){
        if( MagicItemsStoreArray[itemNextIndex].ItemCode != IC_M1_NONE ){
            --i;
            itemCurIndex = itemNextIndex;
        }
    }
    Item &selectedItem = MagicItemsStoreArray[itemCurIndex];
    if (player.TotalGold < BuyPrice(selectedItem)) {
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &selectedItem, sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);

    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = AutoPutCursorToCell(player, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//!---- (004711DF) --------------------------------------------------------
void HandleGriswoldItemSale ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_3_BlacksmithSale;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    ChangeTownerDialog(PD_11_Confirm);
}

void GriswoldRestockMenu(PLAYER_DIALOG callerDialog)
{
	Player& player = Players[CurrentPlayerIndex];
	IsBigMenuBox = true;

	int restockPrice = 1;

    if (PD_27_BlacksmithBasicalRestock == callerDialog) {
        restockPrice = GetGriswoldBasicalRestockPrice(player);
    } else if (PD_24_BlacksmithMagicalRestock == callerDialog) {
        restockPrice = GetGriswoldMagicalRestockPrice(player);
    }

	if (player.TotalGold >= restockPrice){
		ClearGoldByInventoryAsPrice(restockPrice);

		ChangeTownerDialog(ConfirmationCaller);
		DialogHasScroolBox = false;
		IsBigMenuBox = true;
		ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
		SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "Items have been restocked!", C_0_White, true);

        SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon] * ms(GetTickCountTh()));
        if (PD_27_BlacksmithBasicalRestock == callerDialog) {
            for(int i = 0; i < PT_BasicItemsCount; i++ ){
                BasicalItemsArray[i].ItemCode = IC_M1_NONE;
            }
            BasicalItemsGeneration(ShopQlvl(player));
        } else if (PD_24_BlacksmithMagicalRestock == callerDialog) {
            for (int i = 0; i < GriswoldMagicItemCount; ++i) {
                MagicItemsStoreArray[i].ItemCode = IC_M1_NONE;
            }
            GriswoldMagicItemCount = 0;
            MagicItemsGeneration(CurrentPlayerIndex);
        }
	}
	else {
		ChangeTownerDialog(ConfirmationCaller);
		DialogHasScroolBox = false;
		IsBigMenuBox = true;
		ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
		SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough gold", C_0_White, true);
	}

	//SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
//	LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
}

void HandleGriswoldItemRestock(PLAYER_DIALOG callerDialog)
{
	ChangeTownerDialog(PD_1_BlacksmithMain);
    if (PD_27_BlacksmithBasicalRestock == callerDialog) {
    } else if (PD_24_BlacksmithMagicalRestock == callerDialog) {
    }
}

void HandleGriswoldItemRepair ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_1_BlacksmithMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_4_RepairItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//!---- (004711E9) --------------------------------------------------------
void HandleAdriaMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_5_WitchMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_5_WitchMain_Talk;
            CurrentTowner = TI_6_ADRIA;
            ConfirmationCaller = PD_5_WitchMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_213;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_223;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_5_WitchMain_Trade:           VisualTrade_Open( VTM_Witch ); break;
        case PT_DialogRows_5_WitchMain_Enchant:         Enchant_Open(); break;
        case PT_DialogRows_5_WitchMain_Leave:			CurrentDialogIndex = PD_0_None; break;
    }
}

//!---- (004711EE) --------------------------------------------------------
void HandleAdriaItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_6_WitchBuy;
	Item &item = WitchAdriaItems[GET_SELECTED_ITEM_INDEX];
    if( player.TotalGold < BuyPrice(item) ){
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
	memcpy(&player.ItemOnCursor, &item, sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = 0;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventoryOrBelt(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//!---- (004711F3) --------------------------------------------------------
void HandleAdriaItemSell ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_7_WitchSell;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    ChangeTownerDialog(PD_11_Confirm);
}

void AdriaRestockMenu()
{
    Player& player = Players[CurrentPlayerIndex];
    IsBigMenuBox = true;

    int restockPrice = GetAdriaRestockPrice(player);

    if (player.TotalGold >= restockPrice){
        ClearGoldByInventoryAsPrice(restockPrice);

        ChangeTownerDialog(ConfirmationCaller);
        DialogHasScroolBox = false;
        IsBigMenuBox = true;
        ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "Items have been restocked!", C_0_White, true);

        for(int i = 0; i < PT_25_WitchItemsCount; i++ ){
            WitchAdriaItems[i].ItemCode = IC_M1_NONE;
        }
        SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon] * ms(GetTickCountTh()));
        WitchAdriaItemsGeneration(ShopQlvl(player));
    }
    else {
        ChangeTownerDialog(ConfirmationCaller);
        DialogHasScroolBox = false;
        IsBigMenuBox = true;
        ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_DialogEnd);
        SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Info, true, "You do not have enough gold", C_0_White, true);
    }
}

void HandleAdriaItemRestock()
{
    ChangeTownerDialog(PD_5_WitchMain);
}

//!---- (004711F8) --------------------------------------------------------
void HandleAdriaItemRecharge ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_5_WitchMain);
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_8_RechargeItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//!---- (004711FD) --------------------------------------------------------
void HandleNoGoldOrRoomMenu ()
{
    ChangeTownerDialog(ConfirmationCaller);
    SelectedDialogRow = SelectedDialogRowInPrevDialog;
    FirstVisibleItem = ScrolledItemsCountInPrevDialog;
}

//!---- (0047121D) --------------------------------------------------------
void HandleConfirmMenu ()
{
    if( SelectedDialogRow != PT_DialogRows_11_Confirm_Yes ){
        ChangeTownerDialog(ConfirmationCaller);
        SelectedDialogRow = SelectedDialogRowInPrevDialog;
        FirstVisibleItem = ScrolledItemsCountInPrevDialog;
        return;
    }
	int OldRowsCount = DialogRowsCount;
	bool doStuff = false;
    switch (ConfirmationCaller){
	case PD_2_BasicalItems:		BuyBasicalItemFromGriswold();	doStuff = true; break;
        case PD_18_MagicalItems:		BuyMagicalItemFromGriswold();	doStuff = true; break;
        case PD_4_RepairItem:		RepairItemByGriswold();			break;
        case PD_13_WirtBuy:			BuyItemFromWirt();				break;
        case PD_16_HealerBuy:		BuyItemFromPepin();				doStuff = true; break;
        case PD_6_WitchBuy:			BuyItemFromAdria();				doStuff = true; break;
        case PD_3_BlacksmithSale:
        case PD_7_WitchSell:			SellItem();						break;
        case PD_8_RechargeItem:		RechargeStaffByAdria();			break;
        case PD_17_IdentifyItem:
            IdentifyItemByKain();
            ChangeTownerDialog(PD_20_IdentifiedItemInfo);
            return;
    }

    ChangeTownerDialog(ConfirmationCaller);
	if (doStuff == true) {
		if (OldRowsCount == DialogRowsCount) {
			SelectedDialogRow = SelectedDialogRowInPrevDialog;
			FirstVisibleItem = ScrolledItemsCountInPrevDialog;
		}
		else {
			SelectedDialogRow -= TradeRows;
			if (SelectedDialogRow < 0) { SelectedDialogRow = 0; }
			FirstVisibleItem = ScrolledItemsCountInPrevDialog - 1;
			if (FirstVisibleItem < 0) { FirstVisibleItem = 0; }
		}
	}
}

//!---- (00471222) --------------------------------------------------------
void HandleWirtMainMenu ()
{
    Player& player = Players[CurrentPlayerIndex];
    if( WirtItem.ItemCode != IC_M1_NONE && (SelectedDialogRow == PT_DialogRows_12_WirtMain_BuyItem) ){
        if( player.TotalGold >= PT_WirtItemPrice ){
            ClearGoldByInventoryAsPrice(PT_WirtItemPrice);
            VisualTrade_Open( VTM_Gamble );
        }else{
            ConfirmationCaller = PD_12_WirtMain;
            SelectedDialogRowInPrevDialog = PT_DialogRows_12_WirtMain_BuyItem;
            ScrolledItemsCountInPrevDialog = FirstVisibleItem;
            ChangeTownerDialog(PD_9_NoGold);
        }
    }else if( SelectedDialogRow == PT_DialogRows_12_WirtMain_Talk && WirtItem.ItemCode != IC_M1_NONE
        || SelectedDialogRow == PT_DialogRows_12_WirtMain_NoItem_Talk && WirtItem.ItemCode == IC_M1_NONE ){
            CurrentTowner = TI_8_WIRT;
            ConfirmationCaller = PD_12_WirtMain;
            SelectedDialogRowInPrevDialog = SelectedDialogRow;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_225;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_234;
            ChangeTownerDialog(PD_19_Talk);
    }else{
        CurrentDialogIndex = PD_0_None;
    }
}

//!---- (00471227) --------------------------------------------------------
void HandleWirtItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow != PT_DialogRows_13_WirtBuy_Item ){
        CurrentDialogIndex = PD_0_None;
        return;
    }
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_13_WirtBuy;
    SelectedDialogRowInPrevDialog = PT_DialogRows_13_WirtBuy_Item;
    int itemPrice = (int) (BuyPrice(WirtItem) * 1.5);
    if (player.TotalGold < itemPrice) {
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &WirtItem, sizeof (Item));
    player.ItemOnCursor.Price = itemPrice;
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);

    bool canPutInventoryItem = false;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = AutoPutCursorToCell(player, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_10_NoRoom);
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//!---- (0047122C) --------------------------------------------------------
void HandlePepinMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_14_HealerMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_14_HealerMain_Talk;
            CurrentTowner = TI_1_PEPIN;
            ConfirmationCaller = PD_14_HealerMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_170;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_178;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_14_HealerMain_Trade: VisualTrade_Open( VTM_Healer ); break;
        case PT_DialogRows_14_HealerMain_Leave:	CurrentDialogIndex = PD_0_None;	break;
    }
}

//!---- (00471231) --------------------------------------------------------
void HandleKainMainMenu ()
{
    switch( SelectedDialogRow ){
        case PT_DialogRows_15_ElderMain_Talk:
            SelectedDialogRowInPrevDialog = PT_DialogRows_15_ElderMain_Talk;
            CurrentTowner = TI_4_CAIN;
            ConfirmationCaller = PD_15_ElderMain;
            StartIndexOfGossibSpeechByCurrentCityzen = SP_151;
            EndIndexOfGossibSpeechByCurrentCityzen = SP_159;
            ChangeTownerDialog(PD_19_Talk);
            break;
        case PT_DialogRows_15_ElderMain_IdentifyItems:	VisualTrade_Open( VTM_Elder ); break;
		case PT_DialogRows_15_ElderMain_Transmute: if( CanTransmute() ) Transmute_Open(); break;

		case PT_DialogRows_15_ElderMain_Leave:			CurrentDialogIndex = PD_0_None;	break;
    }
}

//!---- (00471236) --------------------------------------------------------
void HandlePepinItemBuy ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_14_HealerMain);
        SelectedDialogRow = PT_DialogRows_14_HealerMain_Leave;// TODO: косяк оригинала. Должно быть PT_DialogRows_14_HealerMain_BuyItems ( 14 )
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_16_HealerBuy;
    Item &selectedItem = PepinItems[GET_SELECTED_ITEM_INDEX];
    if (player.TotalGold < BuyPrice(selectedItem)) {
        ChangeTownerDialog(PD_9_NoGold);
        return;
    }
    memcpy(&player.ItemOnCursor, &selectedItem, sizeof (Item));
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
    bool canPutInventoryItem = 0;
    for( int i = 0; i < IS_70_40_Inv_Count && !canPutInventoryItem; i++ ){
        canPutInventoryItem = TryPutItemInInventoryOrBelt(CurrentPlayerIndex, i, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, false);
    }
    if( canPutInventoryItem ){
        ChangeTownerDialog(PD_11_Confirm); 
    }else{
        ChangeTownerDialog(PD_10_NoRoom); 
    }
    SetCursorGraphics(CM_1_NORMAL_HAND);
}

//!---- (0047123B) --------------------------------------------------------
void HandleKainItemIdentify ()
{
    Player& player = Players[CurrentPlayerIndex];
    
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(PD_15_ElderMain);
        SelectedDialogRow = PT_DialogRows_15_ElderMain_IdentifyItems;
        return;
    }
    SelectedDialogRowInPrevDialog = SelectedDialogRow;
    ScrolledItemsCountInPrevDialog = FirstVisibleItem;
    ConfirmationCaller = PD_17_IdentifyItem;
    int selectedItem = GET_SELECTED_ITEM_INDEX;
    memcpy(&player.ItemOnCursor, &PlayerItemsForSomeAction[selectedItem], sizeof (Item));
    if( player.TotalGold >= PlayerItemsForSomeAction[selectedItem].Price ){
        ChangeTownerDialog(PD_11_Confirm);
    }else{
        ChangeTownerDialog(PD_9_NoGold);
    }
}

//!---- (00471240) --------------------------------------------------------
void HandleTalkMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_BigDialog_Back ){
        ChangeTownerDialog(ConfirmationCaller);
        SelectedDialogRow = SelectedDialogRowInPrevDialog;
        return;
    }
    int numberOfActiveQwest = 0;
    for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS 
            && TownersSpeechAboutQuestArray[CurrentTowner][questIndex] != SP_M1 && quest.talkToFlag ){
                ++numberOfActiveQwest;
        }
    }
    int topQwestRow, interval;
    if( numberOfActiveQwest <= 6 ){
        topQwestRow = 15 - numberOfActiveQwest;
        interval = 2;
    }else{
        topQwestRow = 14 - (numberOfActiveQwest / 2);
        interval = 1;
    }
    if( SelectedDialogRow == topQwestRow - 2 ){
        SetRndSeed(Towners[CurrentTowner].seed);
        StartSpeech( RngFromRange( StartIndexOfGossibSpeechByCurrentCityzen, EndIndexOfGossibSpeechByCurrentCityzen ) );
        return;
    }
    for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
        Quest& quest = Quests[questIndex];
        if( quest.status == QS_2_IN_PROGRESS ){
            int indexOfTownersSpeechAboutQwest = TownersSpeechAboutQuestArray[CurrentTowner][questIndex];
            if( indexOfTownersSpeechAboutQwest != SP_M1  && quest.talkToFlag ){
                if( topQwestRow == SelectedDialogRow ){
                    StartSpeech(indexOfTownersSpeechAboutQwest);
                    return;
                }
                topQwestRow += interval;
            }
        }
    }
}

//!---- (00471245) --------------------------------------------------------
void HandleKainIdentifiedItemParamShow ()
{
    ChangeTownerDialog(PD_17_IdentifyItem);
}

//!---- (0047124C) --------------------------------------------------------
void HandleOgdenMainMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_21_OgdenMain_Talk;
        CurrentTowner = TI_3_OGDEN;
        ConfirmationCaller = PD_21_OgdenMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_161;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_168;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_TravelToRavenholm ){
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( theRampagingDemonQuest.status == QS_2_IN_PROGRESS ){
            //if( theRampagingDemonQuest.dungeonType != INVALID_SET_FILE_POINTER ){
            //    WithDungeonType = theRampagingDemonQuest.dungeonType;
            //}
			//NewQuestLocIndex = 0;
			//PrevQuestLocIndex = 0;
			//StartNewLvl(CurrentPlayerIndex, GM_1029_MAYBE_GOING_TO_QUEST_DUNGEON, theRampagingDemonQuest.questFloorNumber);
			int row = 53, col = 63;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
			theRampagingDemonQuest.status3 = 1;
            CurrentDialogIndex = PD_0_None;
        }
    }else if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_TravelToTheDen ){
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( theRampagingDemonQuest.status == QS_2_IN_PROGRESS ){
            StartNewLvl(CurrentPlayerIndex, WM_1029_GOTO_QUEST_DUNGEON, DUN_45_THE_DEN);
			CurrentDialogIndex = PD_0_None;
        }
    }else if( SelectedDialogRow == PT_DialogRows_21_OgdenMain_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

//!---- (00471251) --------------------------------------------------------
void HandleGilianMainMenu ()
{
	if( SelectedDialogRow == PT_DialogRows_GilianMain_12_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_GilianMain_12_Talk;
        CurrentTowner = TI_7_GILLIAN;
        ConfirmationCaller = PD_23_GilianMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_180;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_187;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_GilianMain_14_Stash && StashEnabled() ){
        StashPanel_Open();
    }else if( SelectedDialogRow == PT_DialogRows_GilianMain_18_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

//!---- (00471256) --------------------------------------------------------
void HandleFarnhamMainMenu ()
{
    if( SelectedDialogRow == PT_DialogRows_22_FarnhamMain_Talk ){
        SelectedDialogRowInPrevDialog = PT_DialogRows_22_FarnhamMain_Talk;
        CurrentTowner = TI_5_FARNHAM;
        ConfirmationCaller = PD_22_FarnhamMain;
        StartIndexOfGossibSpeechByCurrentCityzen = SP_201;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_211;
        ChangeTownerDialog(PD_19_Talk);
    }else if( SelectedDialogRow == PT_DialogRows_22_FarnhamMain_Leave ){
        CurrentDialogIndex = PD_0_None;
    }
}

void HandleColiseumMainMenu() 
{
    if (SelectedDialogRow == PT_DialogRows_ColiseumMain_Talk) {
        SelectedDialogRowInPrevDialog = PT_DialogRows_ColiseumMain_Talk;
        CurrentTowner = TI_14_COLISEUM;
        ConfirmationCaller = PD_28_ColiseumMain;
       /* StartIndexOfGossibSpeechByCurrentCityzen = SP_201;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_211;*/
        /*InitRandomSeed(Towners[CurrentTowner].seed);
        StartSpeech(RandFromRange(StartIndexOfGossibSpeechByCurrentCityzen, EndIndexOfGossibSpeechByCurrentCityzen));
        return;*/
        ChangeTownerDialog(PD_29_ColiseumDiff);
    }
    else if (SelectedDialogRow == PT_DialogRows_ColiseumMain_Leave) {
        CurrentDialogIndex = PD_0_None;
    }
}

void HandleColiseumDiff()
{
    if (SelectedDialogRow >= PT_DialogRows_ColiseumDiff_1 && SelectedDialogRow <= PT_DialogRows_ColiseumDiff_10) {
        SelectedDialogRowInPrevDialog = PT_DialogRows_ColiseumMain_Talk;
        CurrentTowner = TI_14_COLISEUM;
        ConfirmationCaller = PD_28_ColiseumMain;

        StartNewLvl(CurrentPlayerIndex, WM_1029_GOTO_QUEST_DUNGEON, DUN_46_COLISEUM);
        CurrentDialogIndex = PD_0_None;
        /*StartIndexOfGossibSpeechByCurrentCityzen = SP_201;
        EndIndexOfGossibSpeechByCurrentCityzen = SP_211;
        InitRandomSeed(Towners[CurrentTowner].seed);
        StartSpeech(RandFromRange(StartIndexOfGossibSpeechByCurrentCityzen, EndIndexOfGossibSpeechByCurrentCityzen));*/

       // ChangeTownerDialog(PD_29_ColiseumDiff);
    }
    else if (SelectedDialogRow == PT_DialogRows_ColiseumDiff_Leave) {
        ChangeTownerDialog(ConfirmationCaller);
        SelectedDialogRow = SelectedDialogRowInPrevDialog;
    }
}

//----- (00471198) --------------------------------------------------------
void HandleTownersMenu()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( Dungeon->genType == DT_0_TOWN ){
            StopSpeech();
        }
        return;
    }
    PlayGlobalSound(S_76_I_TITLSLCT);
    switch( CurrentDialogIndex ) {
        //Griswold the Blacksmith
        case PD_1_BlacksmithMain:		HandleGriswoldMainMenu();				return;
        case PD_2_BasicalItems:			HandleBasicalItemBuy();					return;
        case PD_18_MagicalItems:		HandleMagicalItemBuy();            		return;
        case PD_3_BlacksmithSale:		HandleGriswoldItemSale();				return;
        case PD_4_RepairItem:			HandleGriswoldItemRepair();				return;
        case PD_27_BlacksmithBasicalRestock:
        case PD_24_BlacksmithMagicalRestock:
            HandleGriswoldItemRestock(CurrentDialogIndex);
            return;
        //Adria the Witch
        case PD_5_WitchMain:			HandleAdriaMainMenu();					return;
        case PD_6_WitchBuy:				HandleAdriaItemBuy();					return;
        case PD_7_WitchSell:			HandleAdriaItemSell();					return;
        case PD_8_RechargeItem:			HandleAdriaItemRecharge();				return;
        case PD_26_AdriaRestock:   HandleAdriaItemRestock();           	return;
        //Pepin the Healer
        case PD_14_HealerMain:			HandlePepinMainMenu();					return;
        case PD_16_HealerBuy:			HandlePepinItemBuy();					return;
        //Wirt
        case PD_12_WirtMain:			HandleWirtMainMenu();					return;
        case PD_13_WirtBuy:				HandleWirtItemBuy();					return;
        //Cain the Elder
        case PD_15_ElderMain:			HandleKainMainMenu();					return;
        case PD_17_IdentifyItem:		HandleKainItemIdentify();				return;
        case PD_20_IdentifiedItemInfo:	HandleKainIdentifiedItemParamShow();	return;
        //Others
        case PD_21_OgdenMain:			HandleOgdenMainMenu();					return;
        case PD_23_GilianMain:			HandleGilianMainMenu();					return;
        case PD_22_FarnhamMain:			HandleFarnhamMainMenu();				return;
        //Common
        case PD_9_NoGold:
        case PD_10_NoRoom:				HandleNoGoldOrRoomMenu();				return;
        case PD_11_Confirm:				HandleConfirmMenu();					return;
        case PD_19_Talk:				HandleTalkMenu();						return;
        case PD_28_ColiseumMain:        HandleColiseumMainMenu();               return;
        case PD_29_ColiseumDiff:        HandleColiseumDiff();               return;
    }
}

RECT DialogWin; int OutDialog;

//----- (004712B7) --------------------------------------------------------
int CheckDialogMouseClick()
{
    if( Speech_IsPanelVisible ){
        Speech_IsPanelVisible = false;
        if( Dungeon->genType == DT_0_TOWN ){
            StopSpeech();
        }
        return 1;
    }
    if( SelectedDialogRow == -1 ){
        return 1;
    }
	if( ! PtInRect( &DialogWin, { CursorX, CursorY } ) ){
		CurrentDialogIndex = PD_0_None; // completely close the dialog by clicking the mouse outside the dialog
		//OutDialog = 1;
		return 1; // если тут вернуть 0, то будет сразу идти (в нажатую мимо диалога точку)
    }
	//OutDialog = 0;
    int row = (CursorY - DialogWin.top - 6) / 12;
    if( DialogHasScroolBox && CursorX > DialogWin.right - 14 ){
        if( row == 4 ){
            if( DialogScrollUpDelay > 0 ){
                --DialogScrollUpDelay;
            }else{
                DialogUp();
                DialogScrollUpDelay = 10;
            }
        }
        if( row == 20 ){
            if( DialogScrollDownDelay > 0 ){
                --DialogScrollDownDelay;
            }else{
                DialogDown();
                DialogScrollDownDelay = 10;
            }
        }
    }else if( row >= 5 ){
		if( row >= 23 ){
			row = 22;
		}
		if( DialogHasScroolBox && row < 21 && !TownersDialogRows[row].selectable ){
            if( TownersDialogRows[row - 2].selectable ){
                row -= 2;
            }else if( TownersDialogRows[row - 1].selectable ){
                --row;
            }
        }
        if( TownersDialogRows[row].selectable || DialogHasScroolBox && row == 22 ){
            SelectedDialogRow = row;
            HandleTownersMenu();
        }
    }
	return 1;
}

//----- (0047140A) --------------------------------------------------------
void Dialog_MouseClick()
{
    DialogScrollUpDelay = -1;
    DialogScrollDownDelay = -1;
}
