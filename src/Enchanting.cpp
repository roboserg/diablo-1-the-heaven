#include "stdafx.h"

bool IsEnchantPanelVisible;
DisplayObject EnchantPanel;
DisplayObject EnchantSlot;
std::array <DisplayObject, 9> EnchantAffixButtons;
DisplayObject EnchantLeaveButton;
Item EnchantItem;
char EnchantString[64];
bool IsEnchantPressed;
int EnchantPrices[9] = { 150/*1*/, 300/*2*/, 600/*3*/, 1050/*4*/, 1650/*5*/, 2400/*6*/, 3300/*7*/, 4500/*8*/, 6000/*9*/ };
char* EnchantPanelCEL = nullptr;
char* LongButtonCEL = nullptr;

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_Open()
{
    EnchantItem.ItemCode = IC_M1_NONE;
    IsEnchantPanelVisible = true;
    IsINVPanelVisible = true;
    CurrentDialogIndex = PD_0_None;
    PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_Close()
{
    if( IsEnchantPanelVisible ){
        Enchant_RetrieveAllItems();
        IsEnchantPanelVisible = false;
        IsINVPanelVisible = false;
    }
}

//----- (th2) -------------------------------------------------------------
bool IsCharacterLevelReqMetForEnchant(int affixButtonIndex)
{
    Player player = Players[CurrentPlayerIndex];   
    return player.CharLevel / 6 + 1 >= (affixButtonIndex + 1);
}

//----- (th2) -------------------------------------------------------------
int SlotToEnchant(Item& item)
{
	for(uint i = 0; i < size(item.enchantIndexes); ++i) if( item.enchantIndexes[i] == -1 ) return i;
	return -1;
}

//----- (th2) -------------------------------------------------------------
bool AllowEnchant(Item& item)
{
	return !(item.dropType & D_ENCHANT) && item.MagicLevel == ML_0_USUAL
		&& (!item.socketsAmount || item.dropType & D_ENCHANT && item.socketsAmount <= MaxAddSockets_4)
		&& not(item.ItemCode, IC_11_GOLD, IC_12_RING, IC_13_AMULET, IC_0_OTHER)
		|| item.dropType & D_ENCHANT && SlotToEnchant(item) != -1;
}

//----- (th2) -------------------------------------------------------------
int EnchantButtonPicNumber(bool isAffixButton, int affixButtonIndex)
{
	if( ! isAffixButton ){
		return IsEnchantPressed && CursorIntoDisplayObject(EnchantLeaveButton) ? N_ENCH_PRESS : N_ENCH;
	}else{
		if( EnchantItem.ItemCode == IC_M1_NONE 
		 || !IsCharacterLevelReqMetForEnchant(affixButtonIndex) 
		 || (EnchantItem.ItemCode != IC_M1_NONE && SlotToEnchant(EnchantItem) == -1
		 || (uint)BuyPrice(EnchantPrices[affixButtonIndex]) > CalcTotalGold(CurrentPlayerIndex)) ){
			return ENCH_INACTIVE;
		}else if( IsCharacterLevelReqMetForEnchant(affixButtonIndex) ){
			if( CursorIntoDisplayObject(EnchantAffixButtons[affixButtonIndex]) ){
				return IsEnchantPressed ? ENCH_PRESS : ENCH_HIGH;
			}else{
				return ENCH_AVAILABLE;
			}
		}
	}
	return BT_NONE;
}

//----- (th3) -------------------------------------------------------------
void SetupEnchantPanel()
{
	SetupPanelRect( EnchantPanel, LeftPanel, GUI_PanelWidth, GUI_PanelHeight);
    SetupDisplayObject(EnchantSlot, EnchantPanel.Left + 133, EnchantPanel.Top + 63, CELL_SIZE * 2, CELL_SIZE * 4);
	SetupDisplayObject(EnchantLeaveButton, EnchantPanel.Left + 278, EnchantPanel.Top + 450 - 32, 33, 32);
	for( int i = 0; i < 9; i++ ){
		SetupDisplayObject(EnchantAffixButtons[i], EnchantPanel.Left + 15, EnchantPanel.Top - 1 + 238 - 20 + 21 * i, 292, 20);
	}
}
//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_DrawEnchantButtons()
{
	for( int i = 0; i < 9; i++ ){
		auto& b = EnchantAffixButtons[i];
		Surface_DrawCEL(b.Left + Screen_LeftBorder, b.Down + Screen_TopBorder, LongButtonCEL, EnchantButtonPicNumber(1, i), b.Width);
		if( EnchantItem.ItemCode != IC_M1_NONE
		 &&( EnchantButtonPicNumber(1, i) != ENCH_INACTIVE
		  || EnchantButtonPicNumber(1, i) == ENCH_INACTIVE && (SlotToEnchant(EnchantItem) != -1 || !(EnchantItem.dropType & D_ENCHANT) && EnchantItem.MagicLevel == ML_0_USUAL) && IsCharacterLevelReqMetForEnchant(i)) ){
			sprintf(EnchantString, "Tier %i enchant, %i gold", i + 1, BuyPrice(EnchantPrices[i]));
			DrawText(EnchantPanel.Left + 161 - GetTextWidth(EnchantString) / 2, EnchantPanel.Top + 238 + 21 * i - 5, 80 + EnchantPanel.Left, EnchantString, 0); // TDOD: change to EnchantAffixButtons
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_DrawItemBacklight()
{
	uchar backlightColor = BLC_NONE;
	if( EnchantItem.ItemCode != IC_M1_NONE ){
		backlightColor = SlotToEnchant(EnchantItem) == -1 ? blColor.cooked : blColor.main;
	}else if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		bool improper = ! AllowEnchant(Players[CurrentPlayerIndex].ItemOnCursor);
		if( EnchantItem.ItemCode == IC_M1_NONE ){
			backlightColor = improper ? BLC_NONE : blColor.overcells;
		}else{
			backlightColor = blColor.notavailable;
		}
		if( CursorIntoDisplayObject(EnchantSlot) ){
			if( improper ){
				backlightColor = blColor.notavailable;
			}else{
				backlightColor = EnchantItem.ItemCode == IC_M1_NONE ? blColor.overslot : blColor.notavailable;
			}
		}
	}
	if( backlightColor != BLC_NONE ){
		CustomShadowItemSlot( Screen_LeftBorder + EnchantSlot.Left, Screen_TopBorder + EnchantSlot.Down, EnchantSlot.Width, EnchantSlot.Heigth, backlightColor );
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_Draw()
{
    Surface_DrawCEL(EnchantPanel.Left + Screen_LeftBorder, EnchantPanel.Down + Screen_TopBorder, EnchantPanelCEL, 1, EnchantPanel.Width);
    Enchant_DrawEnchantButtons();
    Surface_DrawCEL(EnchantLeaveButton.Left + Screen_LeftBorder, EnchantLeaveButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, EnchantButtonPicNumber(0,0), 33);
    Enchant_DrawItemBacklight();
    Enchant_DrawItem(EnchantSlot, EnchantItem);
}

//----- (th2) -------------------------------------------------------------
void Enchant_DrawItem(const DisplayObject& rect, const Item& item)
{
    if( item.ItemCode == IC_M1_NONE ) return;
    int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
    int itemWidth = ItemWidthTable[graphicValue];
    int itemHeight = ItemHeightTable[graphicValue];
    char* itemsCELArray = GetItemSprites(graphicValue);
    int x = rect.Left + Screen_LeftBorder + (rect.Width - itemWidth) / 2;
    int y = rect.Down + Screen_TopBorder - (rect.Heigth - itemHeight) / 2;
	int colorOfBorder = -1;
	if( CursorIntoDisplayObject(EnchantSlot) ){
        colorOfBorder = 197;
        if( item.MagicLevel != ML_0_USUAL || item.dropType & D_ENCHANT ){
            colorOfBorder = 181;
        }
        if( ! item.IsReqMet ){
            colorOfBorder = 229;
        }
    }
    CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
}

//----- (th2) -------------------------------------------------------------
int RollEnchantAffix(int curPower)
{
	static const int minLvls[DL_COUNT] = { 1,  8, 15, 22, 29, 36, 43, 50, 57 },        
	                 maxLvls[DL_COUNT] = { 7, 14, 21, 28, 35, 42, 49, 56, 63 }; // TODO: review max qlvl
	int minLvl = minLvls[curPower];
	int maxLvl = maxLvls[curPower];
	int relCount = 0;
	ITEM_AFFIX itemAffix = ItemAffix(EnchantItem);
	int relevant[countof(AffixesEnchant)];
	for( Affix& affix: AffixesEnchant ){
		if( affix.id > 0 && *affix.Name && (itemAffix & affix.item) && BaseItems[EnchantItem.baseItemIndex].Specialization & affix.itemSpecialization
		  && affix.quality >= minLvl && affix.quality <= maxLvl
		  && not(affix.effect.fullType(), EnchantItem.effect[0].fullType(), EnchantItem.effect[1].fullType(), EnchantItem.effect[2].fullType(), EnchantItem.effect[3].fullType(), EnchantItem.effect[4].fullType()) ){
			relevant[relCount++] = affix.id - 1;
		}
	}
	return relevant[RNG(relCount)];
}

//----- (th2) -------------------------------------------------------------
void ApplyEnchants(int itemIndex, int baseItemIndex, i64 seed, int quality, int genVersion, array<short, EnchantMax_5> enchantIndexes, bool v1, uchar difficulty)
{
	Item& item = Items[itemIndex];
	item.quality = quality;
	item.Identified = true;
	if( v1 ){
		SetupAllItems(itemIndex, baseItemIndex, seed, quality, 0, 1, 0, 0, 0, 0, 0, 0, genVersion, 0, difficulty);
		item.MagicLevel = ML_0_USUAL;
		item.dropType |= D_ENCHANT_V1;
	}
	item.dropType |= D_ENCHANT;
	item.enchantIndexes = enchantIndexes;
	int qualitySum = 0;
	for( uint i = 0; i < EnchantMax_5; i++ ){
		if( item.enchantIndexes[i] != -1 ){
			Affix& affix = AffixesEnchant[item.enchantIndexes[i]];
			Item_ApplyAffix(item, affix.effect, affix.MinGoldValue, affix.MaxGoldValue, affix.Multiplier, i, affix.minReqClvl, affix.maxReqClvl, true);
			qualitySum += AffixesEnchant[i].quality;
		}else{
			item.effect[i].id = AE_NO;
		}
	}
	item.Price += qualitySum * 200;
	item.quality = quality;
	item.Identified = true;
	char name[256];
	sprintf(name, "Enchanted %s", BaseItems[item.baseItemIndex].NamePtr );
	strcpy_s(item.FullMagicalItemName, name);
}

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_MouseClick()
{
    if( CursorIntoDisplayObject(EnchantLeaveButton) ){
        Enchant_Close();
    }else{
        for( int power = 0; power < 9; power++ ){
            if( CursorIntoDisplayObject(EnchantAffixButtons[power]) && EnchantButtonPicNumber(1, power) != ENCH_INACTIVE ){
				int slot = SlotToEnchant(EnchantItem);
				if( slot != -1 ){
					EnchantItem.enchantIndexes[ slot ] = RollEnchantAffix(power);
					EnchantItem.dropType |= D_ENCHANT;
					UpdateItem(EnchantItem);
					PlayGlobalSound(S_1516_adria_enchant_sfx);
					ClearGoldByInventoryAsPrice(BuyPrice(EnchantPrices[power]));
				}
            }
        }
    }
    IsEnchantPressed = false;
}

//----- (th2) -------------------------------------------------------------
void Enchant_PlaceItemIntoCraftSlot()
{
    int srcItemGraphicValue = -1;
    Player& player = Players[CurrentPlayerIndex];
    srcItemGraphicValue = player.ItemOnCursor.GraphicValue;
    const auto newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&EnchantItem, &player.ItemOnCursor);
    if( player.ItemOnCursor.ItemCode == IC_M1_NONE ){
		FixItemCursor(1);
        SetCursorGraphics(CM_1_NORMAL_HAND);
    }else{
        SetCursorGraphics(newCursorGraphics);
    }
    if( srcItemGraphicValue != -1 ){
        PlayGlobalSound(Flip_InventorySound[ItemFlipTable[srcItemGraphicValue]]);
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall Enchant_MouseMove()
{
    MaybeNeedDrawInfoPanel = 0;
    InfoPanel_ClearBody();
    if( CursorIntoDisplayObject(EnchantSlot) && EnchantItem.ItemCode != IC_M1_NONE ){
        Tooltip_ShowItemInfo(101);
    }else if( CursorIntoDisplayObject(EnchantLeaveButton)) {
        Tooltip_SetOffsetTop();
        Tooltip_Clear();
        Tooltip_AddLine("Exit");
    }
}

//----- (th2) -------------------------------------------------------------
void Enchant_RetrieveAllItems()
{
    Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
    if( EnchantItem.ItemCode != IC_M1_NONE ){
		Item prevCur;
        prevCur = cur;
        cur = EnchantItem;
        if( ! AutoPutCursorToInventoryOrDrop(false) ){
            InvPutItem(CurrentPlayerIndex);
        }
        EnchantItem.ItemCode = IC_M1_NONE;
        cur = prevCur;
    }  
}

//----- (th2) -------------------------------------------------------------
void Enchant_GetItemFromItemSlot()
{
	if( ! CursorIntoDisplayObject(EnchantSlot) || EnchantItem.ItemCode == IC_M1_NONE ) return;
    PlayGlobalSound(S_56_I_INVGRAB);
	Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
	cur = EnchantItem;
	EnchantItem.ItemCode = IC_M1_NONE;
    SetCursorGraphics(cur.GraphicValue + CM_12_ITEMS_PICS_START);
	FixItemCursor(-1);
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (th2) -------------------------------------------------------------
void Enchant_LButtonDown()
{
    if( CursorIntoDisplayObject(EnchantSlot) ){
        if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
			Enchant_GetItemFromItemSlot();
		}else if( EnchantItem.ItemCode == IC_M1_NONE && AllowEnchant(Players[CurrentPlayerIndex].ItemOnCursor) ){
			Enchant_PlaceItemIntoCraftSlot();        }
    }else{
		if( CursorIntoDisplayObject(EnchantLeaveButton) ){
			PlayGlobalSound(S_75_I_TITLEMOV);
			IsEnchantPressed = true;
		}else for( int i = 0; i < 9; i++ ){
			if( CursorIntoDisplayObject(EnchantAffixButtons[i]) && EnchantButtonPicNumber(1, i) != ENCH_INACTIVE
			 && (EnchantItem.effect[4].id == AE_NO || EnchantItem.MagicLevel == ML_0_USUAL && !(EnchantItem.dropType & D_ENCHANT) ) ){
				PlayGlobalSound(S_75_I_TITLEMOV);
				IsEnchantPressed = true;
			}
		}
	}
}
