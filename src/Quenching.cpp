#include "stdafx.h"

// Закаливание предметов у Гризволда. Это надо делать в ветке. План работ:
//
// - Сделать дополнительное меню у Гризволда (внизу от Crafting/Socketing): Quenching
// - Выводить для этого меню такой же визуальный интерфейс как у Энчанта Адрии.
// - Туда помещается любой тип предмета (магический, сетовый, белый, квестовый, итд) и за золото игрок апгрейдит статы предметов.
// - Тип предмета должен быть или оружием, или частью брони.  Амулеты и кольца - нельзя. Неидентифицированное нельзя
// - Апгрейды предметов происходят путем добавления экстра аффикса предмету. Синего аффикса.
// - Для оружия апгрейды дают +1 к урону за каждый из 9ти уровней апгрейда (+1 на первом. +9 на 9м.). То есть, суммарно 9й левел апгрейда даст оружию +9 урона.
// - для траппера и бомбардира оружие получит не +1 к урону, а +10% к урону оружия (у нее целочисленные бонусы не работают), для кастеров +1% урону спеллов за грейд. и +1% к урону монстров для саммонеров
// - пишется аффикс: quenched: +X damage или quenched: +X% damage
// - Для брони то же самое но +1 к армору.
// - пишется аффикс: quenched: +X armor . 
// - цена за закаливание равна 1000 х левел закалки во 2 степени (1000 первый, 4000 второй, 9000 девятый, итд). Т.е. полная прокачка предмета стоит 285к (бешеные деньги)
// - звуки закаливания взять из энчанта Адрии. такие же.
// - все уровни закалки доступны сразу. требований по уровню игрока нет. т.е., всё упирается просто в деньги (их не так уж просто скопить)
// - в Колизее это тоже должно быть доступно
// - как и с прочими торговыми опциями, на цены должны влиять все те факторы, которые влияют на цены. Перки на скидки, и прочее.
// 
// 
// дырявить энчантовые предметы у адрии или гризволда.
// так как аффиксы энчанта останавливаются на уровне конца дума. то их можно улучшать сокетами чтобы повысить привлекательность
// и так можно будет делать из любой белой вещи помесь энчантовой и сокетной. и еще свободно управлять сокетный частью. улучшая и заменяя в ней камни
// вот план:
// Allow adding sockets on enchanted items (service at Adria,  or maybe at Griswold's shop):
// 	clvl  70: +1 socket
// 	clvl  90: +2 sockets
// 	clvl 110: +3 sockets
// 	clvl 130: +4 sockets 
// 	по идее, это больше подходит кузнецу чем ведьме
// 
// если энчантить можно было сразу 9й левел  пять раз. то заккаливать надо разрешать только последовательно. 1й. 2й уровень. 3й. и так до 9го
// и каждый использованный грейд уже показывать серым цветом.
// а по сверление дырок в энчантных предметах я подумаю как сделать. там только 4 уровня может быть.
// но принцип, похоже, должен будет быть похожим.
// просверлить можно 4 сокета. и каждый грейд добавляет +1 сокет. использовать каждый грейд на айтеме можно лишь раз. использовал все 4 грейда - всё. больше нельзя
// почему 4, в текущей структуре 6 дырок заложено?
// ну я расчитал балансировку так
// нельзя допускать того чтобы какойто тип айтема сумарно мог иметь мощности больше чем другие типы. иначе он лишит смысла существования прочие.
// поэтому философия такова что типы предметов просто разные. потолок везде един, более-менее
// ну и если тебе интересно техническое объяснение...
// энчантов моет быть макс = 5.
// 9 грейд энчантов каждый имеет 40% мощности от обычных аффиксов дума.
// конечные аффиксы инферно примерно на 100% мощнее думовских.
// то есть имеем 1 аффикс уровня 9й сложности
// каждые 2 дырки дают 2 камня. каждый джем 10 грейда (хер докачаешь еще) дает 1 аффикс
// то есть уже как бы 2 дырки есть от энчантовых аффиксов. остается 4
// ну план был таков что доступность высверливания новой дырки тупо от уровня игрока зависит
// 
// Allow adding sockets on enchanted items (service at Adria,  or maybe at Griswold's shop):
// clvl  70: +1 socket
// clvl  90: +2 sockets
// clvl 110: +3 sockets
// clvl 130: +4 sockets
// спутал. каждый джем 10 уровня = 50% аффикса 
// то есть 6 джемов 10 грейда это как три аффикса. мощно. но заебешься это качать. апгрейдить камни все эти.. это на 1-2 года работы. поэтому такой потолок
// кстати. сделай универсальные развилки по типу функции CanGrind() - дико удобно в одном месте условия ставить где сервис доступен
// для закалки. и сверления энчантов

bool IsQuenchPanelVisible;
DisplayObject QuenchPanel;
DisplayObject QuenchSlot;
array <DisplayObject, 9> QuenchButtons;
DisplayObject QuenchLeaveButton;
Item QuenchItem;
char QuenchString[256];
bool IsQuenchPressed;

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
			sprintf(QuenchString, "T%i: %s, %i gold", nextLevel, InfoPanelBuffer, BuyPrice(QuenchPrice(nextLevel)));
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
	SetupDisplayObject(QuenchSlot, QuenchPanel.Left + 133, QuenchPanel.Top + 63, CELL_SIZE * 2, CELL_SIZE * 4);
	SetupDisplayObject(QuenchLeaveButton, QuenchPanel.Left + 278, QuenchPanel.Top + 450 - 32, 33, 32);
	for( int i = 0; i < QuenchesMax_1; i++ ){
		SetupDisplayObject(QuenchButtons[i], QuenchPanel.Left + 15, QuenchPanel.Top - 1 + 238 - 20 + 21 * i, 292, 20);
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Quench_Draw()
{
	Surface_DrawCEL(QuenchPanel.Left + Screen_LeftBorder, QuenchPanel.Down + Screen_TopBorder, EnchantPanelCEL , 1, QuenchPanel.Width);
	Quench_DrawQuenchButtons();
	Surface_DrawCEL(QuenchLeaveButton.Left + Screen_LeftBorder, QuenchLeaveButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, QuenchButtonPicNumber(0,0), 33); // leave button
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
    CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
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
	if( CursorIntoDisplayObject(QuenchLeaveButton) ){
        Quench_Close();
    }else{
        for( int buttonIndex = 0; buttonIndex < QuenchesMax_1; buttonIndex++ ){
            if( CursorIntoDisplayObject(QuenchButtons[buttonIndex]) && QuenchButtonPicNumber(1, buttonIndex) != ENCH_INACTIVE ){
				if( QuenchItem.quenchLevel < QuenchMaxLevel()){
					QuenchItem.quenchLevel++;
					QuenchItem.dropType |= D_QUENCHED;
					QuenchItem.quenchAffix = GetQuenchAffix(player, QuenchItem, buttonIndex);
					UpdateItem(QuenchItem);
					PlayGlobalSound(S_1516_adria_enchant_sfx);
					ClearGoldByInventoryAsPrice(BuyPrice(QuenchPrice(QuenchItem.quenchLevel)));
				}
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
        Tooltip_AddLine("Exit");
    }
}

//----- (th3) -------------------------------------------------------------
void Quench_RetrieveAllItems()
{
    Item& cur = Players[CurrentPlayerIndex].ItemOnCursor;
    if( QuenchItem.ItemCode != IC_M1_NONE ){
		Item prevCur;
        prevCur = cur;
        cur = QuenchItem;
        if( ! AutoPutCursorToInventoryOrDrop(false) ){
            InvPutItem(CurrentPlayerIndex);
        }
        QuenchItem.ItemCode = IC_M1_NONE;
        cur = prevCur;
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
		}else for( int i = 0; i < 9; i++ ){
			if( CursorIntoDisplayObject(QuenchButtons[i]) && QuenchButtonPicNumber(1, i) != ENCH_INACTIVE ){
				PlayGlobalSound(S_75_I_TITLEMOV);
				IsQuenchPressed = true;
			}
		}
	}
}
