#include "stdafx.h"
#include "ItemOverlay.h"

// (rt) Quenching items at Griswold's. This should be done in a branch. Work plan:
//
// (rt) - Add an extra menu at Griswold's (below Crafting/Socketing): Quenching
// (rt) - Use the same visual interface for this menu as Adria's Enchant.
// (rt) - Any item type goes in there (magic, set, white, quest, etc.) and the player upgrades the item's stats for gold.
// (rt) - The item must be a weapon or a piece of armour. Amulets and rings are not allowed. Unidentified items are not allowed
// (rt) - Upgrades are done by adding an extra (blue) affix to the item.
// (rt) - For weapons each of the 9 upgrade levels gives +1 damage (+1 at the first, +9 at the 9th). So upgrade level 9 gives the weapon +9 damage in total.
// (rt) - for the Trapper and Bombardier the weapon gets +10% weapon damage instead of +1 (integer bonuses don't work for her), casters get +1% spell damage per grade, and summoners +1% minion damage
// (rt) - the affix reads: quenched: +X damage or quenched: +X% damage
// (rt) - The same for armour, but +1 armour.
// (rt) - the affix reads: quenched: +X armor . 
// (rt) - the quenching price is 1000 x the quench level squared (1000 for the first, 4000 for the second, 9000 for the third, etc.). A full upgrade of an item costs 285k (a very large sum)
// (rt) - take the quenching sounds from Adria's enchant. The same ones.
// (rt) - all quench levels are available at once. No player level requirements, so it all comes down to money (which isn't easy to save up)
// (rt) - this should also be available in the Coliseum
// (rt) - as with other trade options, prices must be affected by all the factors that affect prices: discount perks, etc.
// 
// 
// (rt) socketing enchanted items at Adria's or Griswold's.
// (rt) since enchant affixes stop at the end-of-Doom level, they can be improved with sockets to make them more attractive
// (rt) this way any white item can become a mix of enchanted and socketed, with free control over the socket part: upgrading and replacing its gems
// (rt) here is the plan:
// Allow adding sockets on enchanted items (service at Adria,  or maybe at Griswold's shop):
// 	clvl  70: +1 socket
// 	clvl  90: +2 sockets
// 	clvl 110: +3 sockets
// 	clvl 130: +4 sockets 
// 	(rt) in principle this suits the blacksmith better than the witch
// 
// (rt) whereas enchanting allowed level 9 five times straight away, quenching should only be allowed in sequence: level 1, level 2, level 3, and so on up to 9
// (rt) and each used grade should be shown in grey.
// (rt) as for drilling sockets in enchanted items, I'll think about how to do it. Only 4 levels are possible there.
// (rt) but the principle will probably have to be similar.
// (rt) 4 sockets can be drilled, each grade adds +1 socket. Each grade can be used on an item only once. Once all 4 grades are used, no more are possible
// (rt) why 4, when the current structure provides 6 sockets?
// (rt) this is how I worked out the balance
// (rt) no item type may have more total power than the other types, or it makes the others pointless.
// (rt) so the philosophy is that item types are simply different, while the ceiling is roughly the same everywhere
// (rt) and if you want the technical explanation...
// (rt) there can be at most 5 enchants.
// (rt) each grade-9 enchant has 40% of the power of normal Doom affixes.
// (rt) the final Inferno affixes are about 100% stronger than the Doom ones.
// (rt) so we have 1 affix of difficulty-9 level
// (rt) every 2 sockets give 2 gems, and each grade-10 gem (very hard to reach) gives 1 affix
// (rt) so in effect 2 sockets are already covered by the enchant affixes, which leaves 4
// (rt) the plan was that the availability of drilling a new socket depends simply on the player's level
// 
// Allow adding sockets on enchanted items (service at Adria,  or maybe at Griswold's shop):
// clvl  70: +1 socket
// clvl  90: +2 sockets
// clvl 110: +3 sockets
// clvl 130: +4 sockets
// (rt) correction: each grade-10 gem = 50% of an affix 
// (rt) so 6 grade-10 gems equal three affixes. Powerful, but levelling all those gems takes 1-2 years of play, hence this ceiling
// (rt) by the way, make universal branch points like the CanGrind() function - it is very convenient to keep the conditions for where a service is available in one place
// (rt) for quenching and drilling enchanted items

bool IsQuenchPanelVisible;
DisplayObject QuenchPanel;
DisplayObject QuenchSlot;
array <DisplayObject, 9> QuenchButtons;
DisplayObject QuenchLeaveButton;
Item QuenchItem;
char QuenchString[256];
bool IsQuenchPressed;
DisplayObject QuenchButtonRect; char* QuenchButtonCEL = nullptr;

//int QuenchMaxLevel = 9 + PerkValue(SYNERGY_STRONGER_QUENCHING, CurrentPlayerIndex);
//----- (th3) -------------------------------------------------------------
int QuenchMaxLevel()
{ 
	return 9 + PerkValue(SYNERGY_STRONGER_QUENCHING, CurrentPlayerIndex); 
}

//----- (th3) -------------------------------------------------------------
int QuenchPrice(int level)
{
	return level * level * 1000;
}

//----- (th3) -------------------------------------------------------------
QUENCH_AFFIX __fastcall GetQuenchAffix(Player& player, Item& item, int affixIndex = 0)
{
	if( item.dropType & D_QUENCHED && item.quenchAffix != QA_NO ) return item.quenchAffix;
	if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && item.TypeID == ITEM_1_WEAPON) {
		return QA_1_DAMAGE; 
	}
	switch( item.TypeID ){
	case ITEM_2_ARMOR:         return QA_2_ARMOR;
	case ITEM_1_WEAPON:
		switch( player.fullClassId ){
		case PFC_TRAPPER:
		case PFC_BOMBARDIER:   return QA_3_DAMAGE_PERCENT;
		case PFC_MAGE:
		case PFC_WARLOCK:
		case PFC_ELEMENTALIST: return QA_4_SPELL_PERCENT;
		case PFC_DEMONOLOGIST:
		case PFC_NECROMANCER:
		case PFC_BEASTMASTER:  return QA_5_SUMMON_DAMAGE_PERCENT;
		default:               return QA_1_DAMAGE;
		}
	default:                   return QA_NO;
	}
}

//----- (th3) -------------------------------------------------------------
Effect GetQuenchEffect(Item& item, QUENCH_AFFIX quenchAffix, uchar quenchLevel)
{
	Effect e = {};
	switch( quenchAffix ){
	case QA_1_DAMAGE:
		e.id     = AE_ACTION_VALUE;
		e.type   = EA_QUENCH_DAMAGE;
		e.minVal = 1 * quenchLevel; // +1 weapon damage per level
		//e.minVal = (item.EquippedLocation == EL_2_TWO_HANDED ? 2 : 1) * quenchLevel; // possible formula for 2 handed weapons balance
		e.maxVal = e.minVal;
		break;
	case QA_2_ARMOR:
		e.id     = AE_ACTION_VALUE;
		e.type   = EA_QUENCH_ARMOR;
		e.minVal = 1 * quenchLevel; // +1 armor per level
		e.maxVal = e.minVal;
		break;
	case QA_3_DAMAGE_PERCENT:
		e.id     = AE_ACTION_PERCENT;
		e.type   = EA_QUENCH_DAMAGE;
		e.minVal = 5 + 5 * quenchLevel; // 5 + 5% damage per level
		e.maxVal = e.minVal;
		break;
	case QA_4_SPELL_PERCENT:
		e.id     = AE_ACTION_PERCENT;
		e.type   = EA_QUENCH_SPELL_DAMAGE;
		e.minVal = 1 * quenchLevel; // +1% spell damage per level
		e.maxVal = e.minVal;
		break;
	case QA_5_SUMMON_DAMAGE_PERCENT:
		e.id     = AE_ACTION_PERCENT;
		e.type   = EA_QUENCH_SUMMON_DAMAGE;
		e.minVal = 1 * quenchLevel; // +1% summon damage per level
		e.maxVal = e.minVal;
		break;
	}
	e.origin = AO_QUENCH;
	return e;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_Open()
{
    QuenchItem.ItemCode = IC_M1_NONE;
	// Free and reload to match current character
	FreeMemZero(QuenchButtonCEL); 
	if (!QuenchButtonCEL) {
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_28_SUPERMUTANT) || Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
			QuenchButtonCEL = (char*)LoadFile("data\\quench\\enclave_quench.cel");
		}
		else {
			QuenchButtonCEL = (char*)LoadFile("data\\quench\\quench_panel_new_v2.cel");
		}
	}
	IsChatHistoryVisible = false;
	IsQuenchPanelVisible = true;
    IsINVPanelVisible = true;
    CurrentDialogIndex = PD_0_None;
    PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_Close()
{
    if( IsQuenchPanelVisible ){
        Quench_RetrieveAllItems();
        IsQuenchPanelVisible = false;
        IsINVPanelVisible = false;
    }
}

//----- (th3) -------------------------------------------------------------
int QuenchButtonPicNumber(bool isUpgradeButton, int buttonIndex)
{
	if( ! isUpgradeButton ){
		return IsQuenchPressed && CursorIntoDisplayObject(QuenchLeaveButton) ? N_ENCH_PRESS : N_ENCH;
	}else{
		if( QuenchItem.ItemCode == IC_M1_NONE 
		 || QuenchItem.quenchLevel >= QuenchMaxLevel()
		 || (uint)BuyPrice(QuenchPrice(QuenchItem.quenchLevel + 1)) > CalcTotalGold(CurrentPlayerIndex) ){
			return ENCH_INACTIVE;
		}else{
			if( CursorIntoDisplayObject(QuenchButtons[buttonIndex]) ){
				return IsQuenchPressed ? ENCH_PRESS : ENCH_HIGH;
			}else{
				return ENCH_AVAILABLE;
			}
		}
	}
	return BT_NONE;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_DrawQuenchButtons()
{
	Player& player = Players[CurrentPlayerIndex];
	for( int i = 0; i < QuenchesMax_1; i++ ){
		Surface_DrawCEL(QuenchPanel.Left + Screen_LeftBorder + 15, QuenchPanel.Top + Screen_TopBorder - 1 + 238 + 21 * i, LongButtonCEL, QuenchButtonPicNumber(1, i), 292);
		SetupDisplayObject(QuenchButtons[i], QuenchPanel.Left + 15, QuenchPanel.Top - 1 + 238 - 20 + 21 * i, 292, 20);
		if( QuenchItem.ItemCode != IC_M1_NONE && QuenchButtonPicNumber(1, i) != ENCH_INACTIVE ){
			int nextLevel = QuenchItem.quenchLevel + 1;
			Effect e = GetQuenchEffect(QuenchItem, GetQuenchAffix(player, QuenchItem, i), nextLevel);
			InfoPanelBuffer[0] = 0;
			if( AffixEffect::Table[e.id].info ) AffixEffect::Table[e.id].info(QuenchItem, e, player);
			// (th4) localized. %s is the (already localized) affix-effect line from InfoPanelBuffer.
			sprintf(QuenchString, L("T%i: %s, %i gold"), nextLevel, InfoPanelBuffer, BuyPrice(QuenchPrice(nextLevel)));
			DrawText(QuenchPanel.Left + 161 - GetTextWidth(QuenchString) / 2, QuenchPanel.Top + 238 + 21 * i - 5, 80 + QuenchPanel.Left, QuenchString, 0);
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_DrawItemBacklight()
{
	uchar backlightColor = BLC_NONE;
	if( QuenchItem.ItemCode != IC_M1_NONE ){
		backlightColor = QuenchItem.quenchLevel >= QuenchMaxLevel() ? blColor.cooked : blColor.main;
	}else if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		Item& curItem = Players[CurrentPlayerIndex].ItemOnCursor;
		bool improper = is(curItem.ItemCode, IC_M1_NONE, IC_11_GOLD, IC_12_RING, IC_13_AMULET, IC_0_OTHER) || QuenchItem.quenchLevel >= QuenchMaxLevel();
		if( QuenchItem.ItemCode == IC_M1_NONE ){
			backlightColor = improper ? BLC_NONE : blColor.overcells;
		}else{
			backlightColor = blColor.notavailable;
		}
		if( CursorIntoDisplayObject(QuenchSlot) ){
			if( improper ){
				backlightColor = blColor.notavailable;
			}else{
				backlightColor = QuenchItem.ItemCode == IC_M1_NONE ? blColor.overslot : blColor.notavailable;
			}
		}
	}
	if( backlightColor != BLC_NONE ){
		CustomShadowItemSlot( Screen_LeftBorder + QuenchSlot.Left, Screen_TopBorder + QuenchSlot.Down, QuenchSlot.Width, QuenchSlot.Heigth, backlightColor );
	}
}

//----- (th3) -------------------------------------------------------------
void SetupQuenchPanel()
{
	SetupPanelRect( QuenchPanel, LeftPanel, GUI_PanelWidth, GUI_PanelHeight);
	//if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_28_SUPERMUTANT) || Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
	if ((Players && (CurrentPlayerIndex >= 0 && CurrentPlayerIndex < PlayersMax_4)) 
		&& ((Players[CurrentPlayerIndex].gameChanger & BIT(GC_28_SUPERMUTANT)) || (Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)))) {
		SetupDisplayObject(QuenchSlot, QuenchPanel.Left + 105, QuenchPanel.Top + 6, CELL_SIZE * 4, CELL_SIZE * 7);
	}
	else {
		SetupDisplayObject(QuenchSlot, QuenchPanel.Left + 133, QuenchPanel.Top + 63, CELL_SIZE * 2, CELL_SIZE * 4);
	}
	SetupDisplayObject(QuenchLeaveButton, QuenchPanel.Left + 278, QuenchPanel.Top + 450 - 32, 33, 32);
	SetupDisplayObject(QuenchButtonRect, QuenchPanel.Left + 123, QuenchPanel.Top + 272, 74, 77);
	for( int i = 0; i < QuenchesMax_1; i++ ){
		SetupDisplayObject(QuenchButtons[i], QuenchPanel.Left + 15, QuenchPanel.Top - 1 + 238 - 20 + 21 * i, 292, 20);
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_Draw()
{
	// (rt) 1. Background - the enchant panel
	//Surface_DrawCEL(QuenchPanel.Left + Screen_LeftBorder, QuenchPanel.Down + Screen_TopBorder, EnchantPanelCEL, 1, QuenchPanel.Width);
	Surface_DrawCEL(
		QuenchPanel.Left + Screen_LeftBorder,
		QuenchPanel.Down + Screen_TopBorder,
		QuenchButtonCEL,  // (rt) the same file as the button
		1,                // (rt) frame 1 - the panel
		QuenchPanel.Width
	);
	// (rt) 2. New button in the middle
	bool canQuench = (QuenchItem.ItemCode != IC_M1_NONE &&
		QuenchItem.quenchLevel < QuenchMaxLevel() &&
		Players[CurrentPlayerIndex].TotalGold >= BuyPrice(QuenchPrice(QuenchItem.quenchLevel + 1)));

	int frame;
	// (rt) temporary
	//DrawText(100, 100, 0, QuenchItem.ItemCode == IC_M1_NONE ? "Empty" : "Has item", C_2_Red);

	if (canQuench) {
		// (rt) Red button (available)
		frame = IsQuenchPressed ? 3 : 2;
	}
	else {
		// (rt) Grey button (unavailable)
		frame = IsQuenchPressed ? 5 : 4;
	}

	Surface_DrawCEL(
		QuenchButtonRect.Left + Screen_LeftBorder,
		QuenchButtonRect.Down + Screen_TopBorder,
		QuenchButtonCEL,
		frame,
		QuenchButtonRect.Width
	);

	// (rt) 3. Exit button
	Surface_DrawCEL(QuenchLeaveButton.Left + Screen_LeftBorder, QuenchLeaveButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, QuenchButtonPicNumber(0, 0), 33);

	// (rt) 4. Highlight and item
	Quench_DrawItemBacklight();
	Quench_DrawItem(QuenchSlot, QuenchItem);
}

//----- (th3) -------------------------------------------------------------
void Quench_DrawItem(const DisplayObject& rect, const Item& item)
{
    if( item.ItemCode == IC_M1_NONE ) return;
    int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
    int itemWidth = ItemWidthTable[graphicValue];
    int itemHeight = ItemHeightTable[graphicValue];
    const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
    char* itemsCELArray = GetItemSprites(graphicValue);
    int x = rect.Left + Screen_LeftBorder + (rect.Width - itemWidth) / 2;
    int y = rect.Down + Screen_TopBorder - (rect.Heigth - itemHeight) / 2;
	int colorOfBorder = -1;
	if( CursorIntoDisplayObject(QuenchSlot) ){
        colorOfBorder = 197;
        if( item.MagicLevel != ML_0_USUAL || item.dropType & D_QUENCHED ){
            colorOfBorder = 181;
        }
        if( ! item.IsReqMet ){
            colorOfBorder = 229;
        }
    }
    ItemCEL_Draw( overlayGfx, x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
    ItemOverlay_Queue(overlayGfx, x, y, itemWidth, itemHeight, item.mirrorImage != 0, colorOfBorder, (int)!item.IsReqMet | item.mirrorImage << 1);	// (th4)
}

//----- (th3) -------------------------------------------------------------
bool QuenchIsItemSuitableForQuenching()
{
	Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
	return not(cur.ItemCode, IC_11_GOLD, IC_12_RING, IC_13_AMULET, IC_0_OTHER) && (cur.Identified || cur.MagicLevel == ML_0_USUAL);
}

//----- (th3) -------------------------------------------------------------
bool Item_ApplyQuenchAffix(Item& item, QUENCH_AFFIX quenchAffix, uchar quenchLevel)
{
	for( auto& ie: item.effect ){
		if( ie.id == AE_NO ){
			ie = GetQuenchEffect(item, quenchAffix, quenchLevel);
			return true;
		}
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
void ApplyQuench(int itemIndex, int baseItemIndex, i64 seed, int quality, int genVersion, QUENCH_AFFIX quenchAffix, uchar quenchLevel)
{
	Item& item = Items[itemIndex];
	item.quality = quality;
	item.Identified = true;
	item.dropType |= D_QUENCHED;
	item.quenchAffix = quenchAffix;
	item.quenchLevel = quenchLevel;
	int qualitySum = 0;
	for( uint i = 0; i < QuenchesMax_1; i++ ){
		if( item.quenchAffix != QA_NO ){
			Item_ApplyQuenchAffix(item, quenchAffix, quenchLevel);
			qualitySum += item.quenchLevel;
		}else{
			//item.*effect[i] = -1;
		}
	}
	item.Price += qualitySum * qualitySum * 100;
	item.quality = quality;
	item.Identified = true;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_MouseClick()
{
	Player& player = Players[CurrentPlayerIndex];

	if (CursorIntoDisplayObject(QuenchLeaveButton)) {
		Quench_Close();
	}
	else if (CursorIntoDisplayObject(QuenchButtonRect)) {
		if (QuenchItem.ItemCode != IC_M1_NONE && QuenchItem.quenchLevel < QuenchMaxLevel()) {
			int nextLevel = QuenchItem.quenchLevel + 1;
			int price = BuyPrice(QuenchPrice(nextLevel));
			if (player.TotalGold >= price) {
				QuenchItem.quenchLevel++;
				QuenchItem.dropType |= D_QUENCHED;
				QuenchItem.quenchAffix = GetQuenchAffix(player, QuenchItem);
				UpdateItem(QuenchItem);
				PlayGlobalSound(S_1516_adria_enchant_sfx);
				ClearGoldByInventoryAsPrice(price);
			}
		}
	}

	IsQuenchPressed = false;
}

//----- (th3) -------------------------------------------------------------
void Quench_PlaceItemIntoCraftSlot()
{
    int srcItemGraphicValue = -1;
    Player& player = Players[CurrentPlayerIndex];
    srcItemGraphicValue = player.ItemOnCursor.GraphicValue;
    const auto newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&QuenchItem, &player.ItemOnCursor);
    if( player.ItemOnCursor.ItemCode == IC_M1_NONE ){
		FixItemCursor(1);
        SetCursorGraphics(CM_1_NORMAL_HAND);
    }else{
        SetCursorGraphics(newCursorGraphics);
    }
    if( srcItemGraphicValue != -1 ){
        PlayGlobalSound(Flip_InventorySound[ItemFlipTable[srcItemGraphicValue]]);
    }
	if( !(QuenchItem.dropType & D_QUENCHED) || QuenchItem.quenchAffix == QA_NO ){
		QuenchItem.quenchLevel = 0;
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_MouseMove()
{
    MaybeNeedDrawInfoPanel = 0;
    InfoPanel_ClearBody();
    if( CursorIntoDisplayObject(QuenchSlot) && QuenchItem.ItemCode != IC_M1_NONE ){
        Tooltip_ShowItemInfo(102);
    }else if( CursorIntoDisplayObject(QuenchLeaveButton)) {
        Tooltip_SetOffsetTop();
        Tooltip_Clear();
        Tooltip_AddLine(L("Exit"));
    }
	else if (CursorIntoDisplayObject(QuenchButtonRect)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		if (QuenchItem.ItemCode == IC_M1_NONE) {
			Tooltip_AddLine(L("Place an item first"), C_2_Red);// (th4) localized
		}
		else if (QuenchItem.quenchLevel >= QuenchMaxLevel()) {
			Tooltip_AddLine(L("Item is already max level"), C_2_Red);// (th4) localized
		}
		else {
			int nextLevel = QuenchItem.quenchLevel + 1;
			sprintf(QuenchString, L("Upgrade to level %i, %i gold"), nextLevel, BuyPrice(QuenchPrice(nextLevel)));// (th4) localized
			Tooltip_AddLine(QuenchString);
		}
	}
}

//----- (th3) -------------------------------------------------------------
void Quench_RetrieveAllItems()
{
    Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
    bool retrievedAny = false;
    if( QuenchItem.ItemCode != IC_M1_NONE ){
		Item prevCur;
        prevCur = cur;
        cur = QuenchItem;
        if( ! AutoPutCursorToInventoryOrDrop(false) ){
            InvPutItem(CurrentPlayerIndex);
        }
        QuenchItem.ItemCode = IC_M1_NONE;
        cur = prevCur;
        retrievedAny = true;
    }
    if( retrievedAny && cur.ItemCode != IC_M1_NONE ){
        SetCursorGraphics(cur.GraphicValue + CM_12_ITEMS_PICS_START);
        FixItemCursor(-1);
    }
}

//----- (th3) -------------------------------------------------------------
void Quench_GetItemFromItemSlot()
{
	if( ! CursorIntoDisplayObject(QuenchSlot) || QuenchItem.ItemCode == IC_M1_NONE ) return;
    PlayGlobalSound(S_56_I_INVGRAB);
	Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
	cur = QuenchItem;
	QuenchItem.ItemCode = IC_M1_NONE;
    SetCursorGraphics(cur.GraphicValue + CM_12_ITEMS_PICS_START);
	FixItemCursor(-1);
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (th3) -------------------------------------------------------------
void Quench_LButtonDown()
{
    if( CursorIntoDisplayObject(QuenchSlot) ){
        if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
			Quench_GetItemFromItemSlot();
		}else if( QuenchIsItemSuitableForQuenching() ){
			Quench_PlaceItemIntoCraftSlot();
		}
    }else{
		if( CursorIntoDisplayObject(QuenchLeaveButton) ){
			PlayGlobalSound(S_75_I_TITLEMOV);
			IsQuenchPressed = true;
		}
		else if (CursorIntoDisplayObject(QuenchButtonRect)) {
			if (QuenchItem.ItemCode != IC_M1_NONE && QuenchItem.quenchLevel < QuenchMaxLevel()) {
				PlayGlobalSound(S_75_I_TITLEMOV);
				IsQuenchPressed = true;
			}
		}
		/*else for( int i = 0; i < 9; i++ ){
			if( CursorIntoDisplayObject(QuenchButtons[i]) && QuenchButtonPicNumber(1, i) != ENCH_INACTIVE ){
				PlayGlobalSound(S_75_I_TITLEMOV);
				IsQuenchPressed = true;
			}
		}*/
	}
}
