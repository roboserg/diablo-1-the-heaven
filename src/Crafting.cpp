#include "stdafx.h"

bool IsCraftPanelVisible;
DisplayObject CraftPanel;
char* CraftPanelCEL;
char* CraftTabsCEL;
//char* CraftBtnsCEL;
char* CraftPanel2CEL;
char* CraftSocketsLockCEL;
char* CraftSocketCEL;

enum Craft_Param
{
	Craft_TabsStartIndex = 0,
	Craft_TabsAmount = 3,
	Craft_ActionButtonsStartIndex = Craft_TabsStartIndex + Craft_TabsAmount,
	Craft_ActionButtonsAmount = 4,
	Craft_ExitButtonStartIndex = Craft_ActionButtonsStartIndex + Craft_ActionButtonsAmount,
	Craft_ExitButtonsAmount = 1,
	Craft_ButtonsAmount = Craft_ExitButtonStartIndex + Craft_ExitButtonsAmount,
	
	Craft_ExitButtonIcon = 9,

	GemCombineConsumablesAmount = 3,
	GemCombineSlotAmount = GemCombineConsumablesAmount + 1,
	GemCombineResultSlotIndex = GemCombineSlotAmount - 1,

	SocketingSocketsAmount = 6,
	SocketingSlotAmount = SocketingSocketsAmount + 1,
	SocketingItemSlotIndex = SocketingSlotAmount - 1,
};

// Craft action. Used via icon on bottom
enum CraftAction
{
    CA_M1_None = -1,
    CA_Combine = 0,
    CA_GetGem = 1,
    CA_GetTestItem = 2,
    CA_BreakItem = 3,
    
    CA_Count
};

array CraftPages = {
    CP_Gems,
    CP_Socketing,
};

CraftPage CurrentCraftPage = CraftPage::CP_M1_None;

char* CraftPagesNames[CP_Count] = {
    /* CP_Gems      */ "GEMS",
    /* CP_Items    */  "ITEMS"
};

DisplayObject CraftButtons[Craft_ButtonsAmount];
bool CraftButtonsStates[Craft_ButtonsAmount];

DisplayObject SocketingItemSlot;
DisplayObject SocketingSocketsSlot;

array<DisplayObject, GemCombineSlotAmount> GemCombineSlots;
array<Item, GemCombineSlotAmount>          GemCombineItems;

array<DisplayObject, SocketingSlotAmount> SocketingSlots;
array<Item, SocketingSlotAmount>          SocketingItems;

// Сокетинг. План реализации.
// 1. У Гризволда добавляется еще одно меню. Если персонаж достиг уровня 60: Add Sockets.
//    Если неудобно добавлять с привязкой к уровню игрока, можно и просто так добавить.
// 3. Оно открывает такой же интерфейс как у квенча. Не будем изобретать велосипед.
// 4. Там так же, одна кнопка. С надписью add socket
// 5. Туда вставляются только предметы класса enchanted
// 6. Разрешено добавлять до 4 сокетов
// 7. Требования по количеству сокетов в зависимости от уровня игрока: {60, 80, 100, 120}
// 8. Разрешить вставлять (сокетные && энчантовые) предметы Гризволду в сервисе крафтинга, во вкладке Items

char AddSocketString[256];
bool AddSocketPressed;
DisplayObject AddSocketButton;

//----- (th3) -------------------------------------------------------------
bool AllowSocketing(Item& item)
{
	return item.socketsAmount > 0 || item.MagicLevel == ML_0_USUAL && item.dropType & D_ENCHANT;
}

//----- (th3) -------------------------------------------------------------
int SocketPrice(int socketAmount)
{
	return by(socketAmount - 1, 25'000, 75'000, 150'000, 250'000);
}

//----- (th3) -------------------------------------------------------------
int SocketReqClvlToAdd(int socketAmount)
{
	return by(socketAmount - 1, 55, 70, 85, 100);
}

//----- (th3) -------------------------------------------------------------
int AddSocketPicNumber()
{
	Item& item = SocketingItems[SocketingItemSlotIndex];
	if( item.ItemCode == IC_M1_NONE || !(item.dropType & D_ENCHANT) || item.socketsAmount >= MaxAddSockets_4
	 || (uint)BuyPrice(SocketPrice(item.socketsAmount + 1)) > CalcTotalGold(CurrentPlayerIndex)
	 || SocketReqClvlToAdd(item.socketsAmount + 1) > Players[CurrentPlayerIndex].CharLevel ){
		return SOC_INACTIVE;
	}else{
		if( CursorIntoDisplayObject(AddSocketButton) ){
			return AddSocketPressed ? SOC_PRESS : SOC_HIGH;
		}else{
			return SOC_AVAILABLE;
		}
	}
	return BT_NONE;
}

//----- (th3) -------------------------------------------------------------
void ClickAddSocketButton()
{
	if( AddSocketPicNumber() != SOC_INACTIVE ){
		Item& item = SocketingItems[SocketingItemSlotIndex];
		if( item.socketsAmount < MaxAddSockets_4 && item.socketsAdded < MaxScrollCount_6 ){
			item.socketsAdded++;
			UpdateItem(item);
			PlayGlobalSound(S_1516_adria_enchant_sfx);
			ClearGoldByInventoryAsPrice(BuyPrice(SocketPrice(item.socketsAmount)));
		}
	}
}

//----- (th3) -------------------------------------------------------------
void DrawAddSocketButton(Item& item)
{
    Surface_DrawCEL(AddSocketButton.Left + Screen_LeftBorder, AddSocketButton.Top + Screen_TopBorder + AddSocketButton.Heigth, CraftPanel2CEL, AddSocketPicNumber(), AddSocketButton.Width);

    int nextAmount = item.socketsAmount + 1;

    if (CursorIntoDisplayObject(AddSocketButton)) {
        Tooltip_Clear();
        Tooltip_SetOffsetBottom();

        if( item.ItemCode == IC_M1_NONE || nextAmount > MaxAddSockets_4 ){
            Tooltip_AddLine("Add sockets to enchanted item,");
            Tooltip_AddLine("Clvl 55 required for adding 1 socket,");
            Tooltip_AddLine("clvl 70 for 2, clvl 85 for 3, clvl 100 for 4");
		}else if( !(item.dropType & D_ENCHANT) ){
			Tooltip_AddLine("Adding sockets is possible only for enchanted items", C_2_Red);
		}else{
            sprintf(InfoPanelBuffer, "Add socket to enchanted item, for %i gold", BuyPrice(SocketPrice(nextAmount))/*, SocketReqClvlToAdd(nextAmount)*/);
            Tooltip_AddLine(InfoPanelBuffer);
        }
    }
}

struct CraftActionInfo
{
    char* name;
    int icon;
};

CraftActionInfo CraftActionInfos[CA_Count] = {
    /* CA_Combine      */ { "Combine",          8 },
};

array<vector<CraftAction>, CP_Count> CraftActions = {{
    { CA_Combine },
    {}
}};

//----- (th2) -------------------------------------------------------------
Gem Gems[] =
{
	{ 1238, GEM::Ruby, 1,        { GemEffectType::FireElementPercent, 10 },      { GemEffectType::Life,  10 } }, // BI_1238_GEM_RUBY_1
	{ 1239, GEM::Ruby, 2,        { GemEffectType::FireElementPercent, 12 },      { GemEffectType::Life,  20 } },
	{ 1240, GEM::Ruby, 3,        { GemEffectType::FireElementPercent, 13 },      { GemEffectType::Life,  30 } },
	{ 1241, GEM::Ruby, 4,        { GemEffectType::FireElementPercent, 14 },      { GemEffectType::Life,  40 } },
	{ 1242, GEM::Ruby, 5,        { GemEffectType::FireElementPercent, 15 },      { GemEffectType::Life,  50 } },
	{ 1243, GEM::Ruby, 6,        { GemEffectType::FireElementPercent, 16 },      { GemEffectType::Life,  60 } },
	{ 1244, GEM::Ruby, 7,        { GemEffectType::FireElementPercent, 17 },      { GemEffectType::Life,  70 } },
	{ 1245, GEM::Ruby, 8,        { GemEffectType::FireElementPercent, 18 },      { GemEffectType::Life,  80 } },
	{ 1246, GEM::Ruby, 9,        { GemEffectType::FireElementPercent, 19 },      { GemEffectType::Life,  90 } },
	{ 1247, GEM::Ruby, 10,       { GemEffectType::FireElementPercent, 20 },      { GemEffectType::Life, 100 } },
	{ 1248, GEM::Amber, 1,       { GemEffectType::ToHit,  10 },                  { GemEffectType::ArmorClass,  2 } },
	{ 1249, GEM::Amber, 2,       { GemEffectType::ToHit,  20 },                  { GemEffectType::ArmorClass,  4 } },
	{ 1250, GEM::Amber, 3,       { GemEffectType::ToHit,  30 },                  { GemEffectType::ArmorClass,  6 } },
	{ 1251, GEM::Amber, 4,       { GemEffectType::ToHit,  40 },                  { GemEffectType::ArmorClass,  8 } },
	{ 1252, GEM::Amber, 5,       { GemEffectType::ToHit,  50 },                  { GemEffectType::ArmorClass, 10 } },
	{ 1253, GEM::Amber, 6,       { GemEffectType::ToHit,  60 },                  { GemEffectType::ArmorClass, 12 } },
	{ 1254, GEM::Amber, 7,       { GemEffectType::ToHit,  70 },                  { GemEffectType::ArmorClass, 14 } },
	{ 1255, GEM::Amber, 8,       { GemEffectType::ToHit,  80 },                  { GemEffectType::ArmorClass, 16 } },
	{ 1256, GEM::Amber, 9,       { GemEffectType::ToHit,  90 },                  { GemEffectType::ArmorClass, 18 } },
	{ 1257, GEM::Amber, 10,      { GemEffectType::ToHit, 100 },                  { GemEffectType::ArmorClass, 20 } },
	{ 1258, GEM::Sapphire, 1,    { GemEffectType::LightningElementPercent, 10 }, { GemEffectType::Mana,  10 } },
	{ 1259, GEM::Sapphire, 2,    { GemEffectType::LightningElementPercent, 12 }, { GemEffectType::Mana,  20 } },
	{ 1260, GEM::Sapphire, 3,    { GemEffectType::LightningElementPercent, 13 }, { GemEffectType::Mana,  30 } },
	{ 1261, GEM::Sapphire, 4,    { GemEffectType::LightningElementPercent, 14 }, { GemEffectType::Mana,  40 } },
	{ 1262, GEM::Sapphire, 5,    { GemEffectType::LightningElementPercent, 15 }, { GemEffectType::Mana,  50 } },
	{ 1263, GEM::Sapphire, 6,    { GemEffectType::LightningElementPercent, 16 }, { GemEffectType::Mana,  60 } },
	{ 1264, GEM::Sapphire, 7,    { GemEffectType::LightningElementPercent, 17 }, { GemEffectType::Mana,  70 } },
	{ 1265, GEM::Sapphire, 8,    { GemEffectType::LightningElementPercent, 18 }, { GemEffectType::Mana,  80 } },
	{ 1266, GEM::Sapphire, 9,    { GemEffectType::LightningElementPercent, 19 }, { GemEffectType::Mana,  90 } },
	{ 1267, GEM::Sapphire, 10,   { GemEffectType::LightningElementPercent, 20 }, { GemEffectType::Mana, 100 } },
	{ 1268, GEM::Topaz, 1,       { GemEffectType::AcidElementPercent, 10 },      { GemEffectType::ResistAllPercent,  2 } },
	{ 1269, GEM::Topaz, 2,       { GemEffectType::AcidElementPercent, 12 },      { GemEffectType::ResistAllPercent,  4 } },
	{ 1270, GEM::Topaz, 3,       { GemEffectType::AcidElementPercent, 13 },      { GemEffectType::ResistAllPercent,  6 } },
	{ 1271, GEM::Topaz, 4,       { GemEffectType::AcidElementPercent, 14 },      { GemEffectType::ResistAllPercent,  8 } },
	{ 1272, GEM::Topaz, 5,       { GemEffectType::AcidElementPercent, 15 },      { GemEffectType::ResistAllPercent, 10 } },
	{ 1273, GEM::Topaz, 6,       { GemEffectType::AcidElementPercent, 16 },      { GemEffectType::ResistAllPercent, 12 } },
	{ 1274, GEM::Topaz, 7,       { GemEffectType::AcidElementPercent, 17 },      { GemEffectType::ResistAllPercent, 14 } },
	{ 1275, GEM::Topaz, 8,       { GemEffectType::AcidElementPercent, 18 },      { GemEffectType::ResistAllPercent, 16 } },
	{ 1276, GEM::Topaz, 9,       { GemEffectType::AcidElementPercent, 19 },      { GemEffectType::ResistAllPercent, 18 } },
	{ 1277, GEM::Topaz, 10,      { GemEffectType::AcidElementPercent, 20 },      { GemEffectType::ResistAllPercent, 20 } },
	{ 1278, GEM::Amethyst, 1,    { GemEffectType::ArcaneElementPercent, 10 },    { GemEffectType::LifeRegenPercent,  50 } },
	{ 1279, GEM::Amethyst, 2,    { GemEffectType::ArcaneElementPercent, 12 },    { GemEffectType::LifeRegenPercent,  60 } },
	{ 1280, GEM::Amethyst, 3,    { GemEffectType::ArcaneElementPercent, 13 },    { GemEffectType::LifeRegenPercent,  65 } },
	{ 1281, GEM::Amethyst, 4,    { GemEffectType::ArcaneElementPercent, 14 },    { GemEffectType::LifeRegenPercent,  70 } },
	{ 1282, GEM::Amethyst, 5,    { GemEffectType::ArcaneElementPercent, 15 },    { GemEffectType::LifeRegenPercent,  75 } },
	{ 1283, GEM::Amethyst, 6,    { GemEffectType::ArcaneElementPercent, 16 },    { GemEffectType::LifeRegenPercent,  80 } },
	{ 1284, GEM::Amethyst, 7,    { GemEffectType::ArcaneElementPercent, 17 },    { GemEffectType::LifeRegenPercent,  85 } },
	{ 1285, GEM::Amethyst, 8,    { GemEffectType::ArcaneElementPercent, 18 },    { GemEffectType::LifeRegenPercent,  90 } },
	{ 1286, GEM::Amethyst, 9,    { GemEffectType::ArcaneElementPercent, 19 },    { GemEffectType::LifeRegenPercent,  95 } },
	{ 1287, GEM::Amethyst, 10,   { GemEffectType::ArcaneElementPercent, 20 },    { GemEffectType::LifeRegenPercent, 100 } },
	{ 1288, GEM::Diamond, 1,     { GemEffectType::ColdElementPercent, 10 },      { GemEffectType::MeleeDamageResistPercent,  2 } },
	{ 1289, GEM::Diamond, 2,     { GemEffectType::ColdElementPercent, 12 },      { GemEffectType::MeleeDamageResistPercent,  4 } },
	{ 1290, GEM::Diamond, 3,     { GemEffectType::ColdElementPercent, 13 },      { GemEffectType::MeleeDamageResistPercent,  6 } },
	{ 1291, GEM::Diamond, 4,     { GemEffectType::ColdElementPercent, 14 },      { GemEffectType::MeleeDamageResistPercent,  8 } },
	{ 1292, GEM::Diamond, 5,     { GemEffectType::ColdElementPercent, 15 },      { GemEffectType::MeleeDamageResistPercent, 10 } },
	{ 1293, GEM::Diamond, 6,     { GemEffectType::ColdElementPercent, 16 },      { GemEffectType::MeleeDamageResistPercent, 12 } },
	{ 1294, GEM::Diamond, 7,     { GemEffectType::ColdElementPercent, 17 },      { GemEffectType::MeleeDamageResistPercent, 14 } },
	{ 1295, GEM::Diamond, 8,     { GemEffectType::ColdElementPercent, 18 },      { GemEffectType::MeleeDamageResistPercent, 16 } },
	{ 1296, GEM::Diamond, 9,     { GemEffectType::ColdElementPercent, 19 },      { GemEffectType::MeleeDamageResistPercent, 18 } },
	{ 1297, GEM::Diamond, 10,    { GemEffectType::ColdElementPercent, 20 },      { GemEffectType::MeleeDamageResistPercent, 20 } },
	{ 1298, GEM::Sodalite, 1,    { GemEffectType::HolyElementPercent, 10 },      { GemEffectType::ManaRegenPercent,  50 } },
	{ 1299, GEM::Sodalite, 2,    { GemEffectType::HolyElementPercent, 12 },      { GemEffectType::ManaRegenPercent,  60 } },
	{ 1300, GEM::Sodalite, 3,    { GemEffectType::HolyElementPercent, 13 },      { GemEffectType::ManaRegenPercent,  65 } },
	{ 1301, GEM::Sodalite, 4,    { GemEffectType::HolyElementPercent, 14 },      { GemEffectType::ManaRegenPercent,  70 } },
	{ 1302, GEM::Sodalite, 5,    { GemEffectType::HolyElementPercent, 15 },      { GemEffectType::ManaRegenPercent,  75 } },
	{ 1303, GEM::Sodalite, 6,    { GemEffectType::HolyElementPercent, 16 },      { GemEffectType::ManaRegenPercent,  80 } },
	{ 1304, GEM::Sodalite, 7,    { GemEffectType::HolyElementPercent, 17 },      { GemEffectType::ManaRegenPercent,  85 } },
	{ 1305, GEM::Sodalite, 8,    { GemEffectType::HolyElementPercent, 18 },      { GemEffectType::ManaRegenPercent,  90 } },
	{ 1306, GEM::Sodalite, 9,    { GemEffectType::HolyElementPercent, 19 },      { GemEffectType::ManaRegenPercent,  95 } },
	{ 1307, GEM::Sodalite, 10,   { GemEffectType::HolyElementPercent, 20 },      { GemEffectType::ManaRegenPercent, 100 } },
	{ 1308, GEM::Onyx, 1,        { GemEffectType::DamagePercent,  50 },          { GemEffectType::ArrowDamageResistPercent,  2 } },
	{ 1309, GEM::Onyx, 2,        { GemEffectType::DamagePercent,  65 },          { GemEffectType::ArrowDamageResistPercent,  4 } },
	{ 1310, GEM::Onyx, 3,        { GemEffectType::DamagePercent,  80 },          { GemEffectType::ArrowDamageResistPercent,  6 } },
	{ 1311, GEM::Onyx, 4,        { GemEffectType::DamagePercent,  90 },          { GemEffectType::ArrowDamageResistPercent,  8 } },
	{ 1312, GEM::Onyx, 5,        { GemEffectType::DamagePercent, 100 },          { GemEffectType::ArrowDamageResistPercent, 10 } },
	{ 1313, GEM::Onyx, 6,        { GemEffectType::DamagePercent, 110 },          { GemEffectType::ArrowDamageResistPercent, 12 } },
	{ 1314, GEM::Onyx, 7,        { GemEffectType::DamagePercent, 120 },          { GemEffectType::ArrowDamageResistPercent, 14 } },
	{ 1315, GEM::Onyx, 8,        { GemEffectType::DamagePercent, 130 },          { GemEffectType::ArrowDamageResistPercent, 16 } },
	{ 1316, GEM::Onyx, 9,        { GemEffectType::DamagePercent, 140 },          { GemEffectType::ArrowDamageResistPercent, 18 } },
	{ 1317, GEM::Onyx, 10,       { GemEffectType::DamagePercent, 150 },          { GemEffectType::ArrowDamageResistPercent, 20 } },
	{ 1318, GEM::Spinel, 1,      { GemEffectType::SummonDamage, 2 },             { GemEffectType::SummonHP,  20 } },
	{ 1319, GEM::Spinel, 2,      { GemEffectType::SummonDamage, 4 },             { GemEffectType::SummonHP,  30 } },
	{ 1320, GEM::Spinel, 3,      { GemEffectType::SummonDamage, 6 },             { GemEffectType::SummonHP,  40 } },
	{ 1321, GEM::Spinel, 4,      { GemEffectType::SummonDamage, 8 },             { GemEffectType::SummonHP,  50 } },
	{ 1322, GEM::Spinel, 5,      { GemEffectType::SummonDamage, 10 },            { GemEffectType::SummonHP,  60 } },
	{ 1323, GEM::Spinel, 6,      { GemEffectType::SummonDamage, 11 },            { GemEffectType::SummonHP,  70 } },
	{ 1324, GEM::Spinel, 7,      { GemEffectType::SummonDamage, 12 },            { GemEffectType::SummonHP,  90 } },
	{ 1325, GEM::Spinel, 8,      { GemEffectType::SummonDamage, 13 },            { GemEffectType::SummonHP, 110 } },
	{ 1326, GEM::Spinel, 9,      { GemEffectType::SummonDamage, 14 },            { GemEffectType::SummonHP, 130 } },
	{ 1327, GEM::Spinel, 10,     { GemEffectType::SummonDamage, 15 },            { GemEffectType::SummonHP, 150 } },
	{ 1328, GEM::Garnet, 1,      { GemEffectType::CritDamagePercent, 35 },       { GemEffectType::LifePercent,  6 } },
	{ 1329, GEM::Garnet, 2,      { GemEffectType::CritDamagePercent, 40 },       { GemEffectType::LifePercent,  6 } },
	{ 1330, GEM::Garnet, 3,      { GemEffectType::CritDamagePercent, 45 },       { GemEffectType::LifePercent,  7 } },
	{ 1331, GEM::Garnet, 4,      { GemEffectType::CritDamagePercent, 50 },       { GemEffectType::LifePercent,  7 } },
	{ 1332, GEM::Garnet, 5,      { GemEffectType::CritDamagePercent, 55 },       { GemEffectType::LifePercent,  8 } },
	{ 1333, GEM::Garnet, 6,      { GemEffectType::CritDamagePercent, 60 },       { GemEffectType::LifePercent,  8 } },
	{ 1334, GEM::Garnet, 7,      { GemEffectType::CritDamagePercent, 65 },       { GemEffectType::LifePercent,  9 } },
	{ 1335, GEM::Garnet, 8,      { GemEffectType::CritDamagePercent, 70 },       { GemEffectType::LifePercent, 10 } },
	{ 1336, GEM::Garnet, 9,      { GemEffectType::CritDamagePercent, 75 },       { GemEffectType::LifePercent, 11 } },
	{ 1337, GEM::Garnet, 10,     { GemEffectType::CritDamagePercent, 80 },       { GemEffectType::LifePercent, 12 } },
	{ 1338, GEM::Citrine, 1,     { GemEffectType::MagicfindPercent, 15 },        { GemEffectType::GoldfindPercent, 10 } },
	{ 1339, GEM::Citrine, 2,     { GemEffectType::MagicfindPercent, 17 },        { GemEffectType::GoldfindPercent, 12 } },
	{ 1340, GEM::Citrine, 3,     { GemEffectType::MagicfindPercent, 18 },        { GemEffectType::GoldfindPercent, 13 } },
	{ 1341, GEM::Citrine, 4,     { GemEffectType::MagicfindPercent, 19 },        { GemEffectType::GoldfindPercent, 14 } },
	{ 1342, GEM::Citrine, 5,     { GemEffectType::MagicfindPercent, 20 },        { GemEffectType::GoldfindPercent, 15 } },
	{ 1343, GEM::Citrine, 6,     { GemEffectType::MagicfindPercent, 21 },        { GemEffectType::GoldfindPercent, 16 } },
	{ 1344, GEM::Citrine, 7,     { GemEffectType::MagicfindPercent, 22 },        { GemEffectType::GoldfindPercent, 17 } },
	{ 1345, GEM::Citrine, 8,     { GemEffectType::MagicfindPercent, 23 },        { GemEffectType::GoldfindPercent, 18 } },
	{ 1346, GEM::Citrine, 9,     { GemEffectType::MagicfindPercent, 24 },        { GemEffectType::GoldfindPercent, 19 } },
	{ 1347, GEM::Citrine, 10,    { GemEffectType::MagicfindPercent, 25 },        { GemEffectType::GoldfindPercent, 20 } },
	{ 1348, GEM::Chrysolite, 1,  { GemEffectType::Damage,  5 },                  { GemEffectType::DFE, -1 } },
	{ 1349, GEM::Chrysolite, 2,  { GemEffectType::Damage, 10 },                  { GemEffectType::DFE, -2 } },
	{ 1350, GEM::Chrysolite, 3,  { GemEffectType::Damage, 15 },                  { GemEffectType::DFE, -3 } },
	{ 1351, GEM::Chrysolite, 4,  { GemEffectType::Damage, 20 },                  { GemEffectType::DFE, -4 } },
	{ 1352, GEM::Chrysolite, 5,  { GemEffectType::Damage, 25 },                  { GemEffectType::DFE, -5 } },
	{ 1353, GEM::Chrysolite, 6,  { GemEffectType::Damage, 30 },                  { GemEffectType::DFE, -7 } },
	{ 1354, GEM::Chrysolite, 7,  { GemEffectType::Damage, 40 },                  { GemEffectType::DFE, -9 } },
	{ 1355, GEM::Chrysolite, 8,  { GemEffectType::Damage, 50 },                  { GemEffectType::DFE, -11 } },
	{ 1356, GEM::Chrysolite, 9,  { GemEffectType::Damage, 65 },                  { GemEffectType::DFE, -13 } },
	{ 1357, GEM::Chrysolite, 10, { GemEffectType::Damage, 80 },                  { GemEffectType::DFE, -15 } }, // BI_1357_GEM_CHRYSOLITE_10
};

//----- (th2) -------------------------------------------------------------
void PrintGemEffectToStringBuffer(GemEffect& effect, bool twoHandedWeapon)
{
	char* desc = "";
	auto value = effect.value;
	switch (effect.type){ // TODO: move to Gem::EffectType[]
	case GemEffectType::FireElementPercent:		        desc = "+%i%% fire element";		          break;
	case GemEffectType::LightningElementPercent:        desc = "+%i%% lightning element";		      break;
	case GemEffectType::AcidElementPercent:		        desc = "+%i%% acid element";		          break;
	case GemEffectType::HolyElementPercent:		        desc = "+%i%% holy element";		          break;
	case GemEffectType::ColdElementPercent:		        desc = "+%i%% cold element";		          break;
	case GemEffectType::ArcaneElementPercent:		    desc = "+%i%% arcane element";		          break;
	case GemEffectType::ResistAllPercent:		        desc = "+%i resist all";		              break;
	case GemEffectType::Life:		                    desc = "+%i life";		                      break;
	case GemEffectType::LifePercent:		            desc = "+%i%% life";		                  break;
	case GemEffectType::LifeRegenPercent:		        desc = "+%i%% life regeneration";		      break;
	case GemEffectType::Mana:		                    desc = "+%i mana";		                      break;
	case GemEffectType::ManaRegenPercent:		        desc = "+%i%% mana regeneration";		      break;
	case GemEffectType::Damage:		                    desc = "+%i damage"; if( twoHandedWeapon || CurTrait(TraitId::Pistoleer)) value *= 2; break;
	case GemEffectType::DamagePercent:		            desc = "+%i%% weapon damage";		          break;
	case GemEffectType::CritDamagePercent:		        desc = "+%i%% critical damage";	              break;
	case GemEffectType::ToHit:		                    desc = "+%i accuracy";                        break;
	case GemEffectType::ArmorClass:		                desc = "+%i armor class";		              break;
	case GemEffectType::DFE:		                    desc = "%i damage from enemies";		      break;
	case GemEffectType::MeleeDamageResistPercent:		desc = "+%i melee damage resistance";		  break;
	case GemEffectType::ArrowDamageResistPercent:		desc = "+%i arrow damage resistance";		  break;
	case GemEffectType::MagicfindPercent:		        desc = "+%i%% chance of finding magic items"; break;
	case GemEffectType::GoldfindPercent:		        desc = "+%i%% to Gold Found";                 break;
	case GemEffectType::SummonDamage:		            desc = "+%i to damage of summoned minions";	  break;
	case GemEffectType::SummonHP:		                desc = "+%i to health of summoned minions";	  break;
	}
	sprintf(InfoPanelBuffer, desc, value);
}

//----- (th2) -------------------------------------------------------------
bool ApplyGemEffect(Item& item, GemEffect& effect)
{
	int effectIndex = 0;
	for (; effectIndex < countof(item.effect); ++effectIndex) if (item.effect[effectIndex].id == AE_NO) break;
	if (effectIndex >= countof(item.effect)) return false;
	Effect& e = item.effect[effectIndex];
	
	switch (effect.type) { // duplicating code, TODO: move to Gems[] with value funcs
	case GemEffectType::FireElementPercent:       e = { AE_ELEMENT_PERCENT,0, ET_1_FIRE, effect.value };      break;
	case GemEffectType::LightningElementPercent:  e = { AE_ELEMENT_PERCENT,0, ET_2_LIGHTNING, effect.value }; break;
	case GemEffectType::AcidElementPercent:       e = { AE_ELEMENT_PERCENT,0, ET_4_ACID, effect.value };      break;
	case GemEffectType::HolyElementPercent:       e = { AE_ELEMENT_PERCENT,0, ET_5_HOLY, effect.value };      break;
	case GemEffectType::ColdElementPercent:       e = { AE_ELEMENT_PERCENT,0, ET_6_COLD, effect.value };      break;
	case GemEffectType::ArcaneElementPercent:     e = { AE_ELEMENT_PERCENT,0, ET_3_ARCAN, effect.value };     break;
	case GemEffectType::ResistAllPercent:         e = { AE_RESIST_All,0, 0, effect.value };                   break;
	case GemEffectType::Life:                     e = { AE_HIT_POINTS,0, 0, effect.value };                   break;
	case GemEffectType::LifePercent:              e = { AE_LIFE_PERCENT,0, 0, effect.value };                 break;
	case GemEffectType::LifeRegenPercent:         e = { AE_LIFE_REGEN_PERCENT,0, 0, effect.value };           break;
	case GemEffectType::Mana:                     e = { AE_MANA,0, 0, effect.value };                         break;
	case GemEffectType::ManaRegenPercent:         e = { AE_MANA_REGEN_PERCENT,0, 0, effect.value };           break;
	case GemEffectType::Damage:                   e = { AE_DAMAGE,0, 0, effect.value }; if( item.EquippedLocation == EL_2_TWO_HANDED || CurTrait(TraitId::Pistoleer)) e.minVal *= 2; break; // mor: idk if CurrentPlayerIndex is ok to use here...
	case GemEffectType::DamagePercent:            e = { AE_PERCENT_DAMAGE,0, 0, effect.value };               break;
	case GemEffectType::CritDamagePercent:        e = { AE_CRIT_DAMAGE_PERCENT,0, 0, effect.value };          break;
	case GemEffectType::ToHit:                    e = { AE_TO_HIT,0, 0, 0, 0, effect.value };                 break;
	case GemEffectType::ArmorClass:               e = { AE_AC,0, 0, effect.value };                           break;
	case GemEffectType::DFE:                      e = { AE_DFE,0, 0, effect.value };                          break;
	case GemEffectType::MeleeDamageResistPercent: e = { AE_ABSORB_MELEE_DAMAGE,0, 0, effect.value };          break;
	case GemEffectType::ArrowDamageResistPercent: e = { AE_ABSORB_RANGED_DAMAGE,0, 0, effect.value };         break;
	case GemEffectType::MagicfindPercent:         e = { AE_MAGIC_FIND,0, 0, effect.value };                   break;
	case GemEffectType::GoldfindPercent:          e = { AE_GOLD_FIND,0, 0, effect.value };                    break;
	case GemEffectType::SummonDamage:             e = { AE_SUMMON_DAMAGE,0, 0, effect.value, effect.value};   break;
	case GemEffectType::SummonHP:                 e = { AE_SUMMON_HP,0, 0, effect.value };                    break;
	}
	e.origin = AO_GEM;
	return true;
}

//----- (th2) -------------------------------------------------------------
optional<Gem> FindGemInfoByBaseItemIndex( int baseItemIndex )
{
    auto it = find_if( begin(Gems), end(Gems),
        [baseItemIndex]( Gem& info ){ return info.baseItemIndex == baseItemIndex; } );
    if( it == end(Gems) ){
        return nullopt;
    }else{
        return *it;
    }
}

//----- (th2) -------------------------------------------------------------
optional<Gem> FindGemInfoByTypeAndGrade( GEM gemType, int grade )
{
    auto it = find_if( begin(Gems), end(Gems),
        [gemType, grade]( Gem& info ){ return info.type == gemType && info.grade == grade; } );
    if( it == end(Gems) ){
        return nullopt;
    }else{
        return *it;
    }
}

//----- (th2) -------------------------------------------------------------
optional<Gem> FindRandomGemInfoForSpecificGrade( int grade )
{
    uint validGemsList[size(Gems)];
    uint validGemsAmount = 0;
    for( uint i = 0; i < size(Gems); ++i ){
        Gem& info = Gems[i];
        if( info.grade == grade ){
            validGemsList[validGemsAmount++] = i;
        }
    }
    
    if( validGemsAmount > 0 ){
        size_t choisenIndex = validGemsList[GetRndSeed() % validGemsAmount];
        return Gems[choisenIndex];
    }else{
        return nullopt;
    }
}

//----- (th2) -------------------------------------------------------------
void CollectSocketEffects( vector<GemEffect>& effectsOut, Item& item )
{
	ITEM_TYPE itemType = BaseItems[item.baseItemIndex].ItemType;
	array<short, size_t(GemEffectType::GE_COUNT)> effectsValues {};
    
    auto registerGemEffect = [&]( GemEffect e ){
        effectsValues.at( size_t(e.type) ) += e.value;
    };
 
    for( int i = 0; i < item.socketsAmount; ++i ){
        int gemBaseItemIndex = item.sockets[i];
        if( gemBaseItemIndex != 0 ){
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( gemBaseItemIndex );
            if( gemInfoOpt ){
                if( itemType == ITEM_1_WEAPON ){
                    registerGemEffect( gemInfoOpt->weaponEffect );
                }else{
                    registerGemEffect( gemInfoOpt->armorEffect );
                }
            }
        }
    }
    
    for( size_t i = 0, ie = effectsValues.size(); i != ie; ++i ){
        short value = effectsValues.at( i );
        if( value ){
            effectsOut.push_back( GemEffect{ static_cast<GemEffectType>( i ), value } );
        }
    }
}

//----- (th2) -------------------------------------------------------------
void ApplyGemEffects( Item& item )
{
    vector<GemEffect> effects; effects.reserve((uint)GemEffectType::GE_COUNT);
    CollectSocketEffects( effects, item );
    
    if (effects.empty() ) {
		if( item.MagicLevel == ML_0_USUAL ) sprintf(item.FullMagicalItemName, "Socketed %s", item.Name);
	}
	else {
        for( GemEffect& e : effects ){
            ApplyGemEffect(item, e);			
		}
		if( item.MagicLevel == ML_0_USUAL ) sprintf(item.FullMagicalItemName, "Gemmed %s", item.Name);
    }
}

//----- (th2) -------------------------------------------------------------
bool CanGrind()
{
	#if CHEATS
	return GameMode != GM_CLASSIC;
	#endif
    // mor: 3 lines below is old code. i left it for reference
    //return not(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON)
     //   && !(Players[CurrentPlayerIndex].gameChanger & BIT( GC_9_NIGHT_KIN ))
     //   && !HasTrait(CurrentPlayerIndex, TraitId::Barbarian);
    
    if( is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE, GM_COLISEUM, GM_CLASSIC) ){ // There is no sense in grinding in special modes (original crafting condition)
        return false;
    }
    
    if( Players[CurrentPlayerIndex].gameChanger & BIT( GC_9_NIGHT_KIN ) ){ // Night Kin (GC) does not use items at all
        return false;
    }

    if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)) { // Ascets lose items
        return false;
    }

    if( HasTrait(CurrentPlayerIndex, TraitId::Barbarian) ){ // Barbarian (trait) uses only white (non-socketed items)
        return false;
    }

    if (HasTrait(CurrentPlayerIndex, TraitId::Leper)) { // Leper doesn't talk to any NPC except Gillian, so he can't socket items..
        return false;
    }

    return true;
}

//----- (th2) -------------------------------------------------------------
void DrawCraftTab( DisplayObject& obj, bool state, char* label )
{
	Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, CraftTabsCEL, state ? 1 : 2, obj.Width);
	DrawText(obj.Left, obj.Down - 5, obj.Right, label, state ? C_0_White : C_7_Grey);
}

//----- (th2) -------------------------------------------------------------
void SetupCraftPanel()
{
	SetupPanelRect( CraftPanel, LeftPanel, GUI_PanelWidth, GUI_PanelHeight );

    int tabsY = CraftPanel.Top + 6;
    int tabsWidth = 73;
    int tabsHeight = 25;

    for( int i = 0; i < Craft_TabsAmount; ++i ){
        SetupDisplayObject(CraftButtons[Craft_TabsStartIndex + i], CraftPanel.Left + 25 + (tabsWidth+10)*i, tabsY, tabsWidth, tabsHeight);
    }
    
    int buttonsWidth = 33;
    int buttonsHeight = 32;
    int buttonsY = CraftPanel.Down - buttonsHeight - 12;
	
	for( int i = 0; i < Craft_ActionButtonsAmount; ++i ){
	    SetupDisplayObject(CraftButtons[Craft_ActionButtonsStartIndex + i], CraftPanel.Left + 12 + (buttonsWidth+6)*i, buttonsY, buttonsWidth, buttonsHeight);
    }
    
    SetupDisplayObject(CraftButtons[Craft_ExitButtonStartIndex], CraftPanel.Right - 12 - buttonsWidth, buttonsY, buttonsWidth, buttonsHeight);
    
    SetupDisplayObject(GemCombineSlots[0], CraftPanel.Left + 84, CraftPanel.Top + 164, CELL_SIZE, CELL_SIZE);
    SetupDisplayObject(GemCombineSlots[1], CraftPanel.Left + 208, CraftPanel.Top + 164, CELL_SIZE, CELL_SIZE);
    SetupDisplayObject(GemCombineSlots[2], CraftPanel.Left + 146, CraftPanel.Top + 270, CELL_SIZE, CELL_SIZE);
    SetupDisplayObject(GemCombineSlots[3], CraftPanel.Left + 146, CraftPanel.Top + 200, CELL_SIZE, CELL_SIZE);
    
    int socketingSlotWidth  = 90;
    int socketingSlotHeight = 130;
    int socketingSlotY = 66;
    
    SetupDisplayObject(SocketingItemSlot,    CraftPanel.Left + 46,  CraftPanel.Top + socketingSlotY, socketingSlotWidth, socketingSlotHeight);
    SetupDisplayObject(SocketingSocketsSlot, CraftPanel.Left + 186, CraftPanel.Top + socketingSlotY, socketingSlotWidth, socketingSlotHeight);
    SocketingSlots[SocketingItemSlotIndex] = SocketingItemSlot;
    
	SetupDisplayObject(AddSocketButton, CraftPanel.Left + 125, CraftPanel.Top + 256, 74, 77);
}

//----- (th2) -------------------------------------------------------------
void Craft_Gems_DrawOneItem( int slotIndex, DisplayObject& rect, Item& item )
{
//    DrawBox( rect, 1 );
//
    if( item.ItemCode == IC_M1_NONE ){
        return;
    }
    
    // ShadowItemSlot(rect.Left + Screen_LeftBorder, rect.Down + Screen_TopBorder, rect.Width, rect.Heigth);
    
    int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
    int itemWidth = ItemWidthTable[graphicValue];
    int itemHeight = ItemHeightTable[graphicValue];
    char* itemsCELArray = GetItemSprites( graphicValue );
    
    int x = rect.Left + Screen_LeftBorder + (rect.Width - itemWidth) / 2;
    int y = rect.Down + Screen_TopBorder - (rect.Heigth - itemHeight) / 2;

    int colorOfBorder = -1;
    if( Cur.InventoryCellID == (slotIndex + Stash_SlotIDStart) ){
        colorOfBorder = 197;
        if( item.MagicLevel ){
            colorOfBorder = 181;
        }
        if( !item.IsReqMet ){
            colorOfBorder = 229;
        }
    }
    if( item.isShadow ){
        DarkLevel = 0;
        TransparentMode = 1;
        
        uchar* surfaceDest = WorkingSurface + YOffsetHashTable[y] + x;
        CEL_DrawShadow(surfaceDest, itemsCELArray, graphicValue, itemWidth, 0, 8 /*, colorOfBorder*/); 
        
        TransparentMode = 0;
    }else{
        CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_Gems_Draw()
{
    for( int i = 0; i < GemCombineSlotAmount; ++i ){
        Craft_Gems_DrawOneItem( i, GemCombineSlots[i], GemCombineItems[i] );
    }
}

//----- (th2) -------------------------------------------------------------
int Craft_Gems_GetSlotMouseAbove()
{
    for( int i = 0; i < GemCombineSlotAmount; ++i ){
        if( CursorIntoDisplayObject( GemCombineSlots[i] ) ){
            return i;
        }
    }
    return -1;
}

//----- (th2) -------------------------------------------------------------
int Craft_Gems_FindFreeConsumableSlot()
{
    for( int i = 0; i < GemCombineConsumablesAmount; ++i ){
        Item& item = GemCombineItems[i];
        if( item.ItemCode == IC_M1_NONE ){
            return i;
        }
    }
    return -1;
}

//----- (th2) -------------------------------------------------------------
Item* Craft_Gems_GetItemBySlotId( int slotId )
{
    if( slotId >= Stash_SlotIDStart && slotId < Stash_SlotIDStart+GemCombineSlotAmount ){
        return &GemCombineItems[slotId - Stash_SlotIDStart];
    }
    return nullptr;
}

//----- (th2) -------------------------------------------------------------
Item* Craft_Socketing_GetItemBySlotId( int slotId )
{
    if( slotId >= Stash_SlotIDStart && slotId < Stash_SlotIDStart+SocketingSlotAmount ){
        return &SocketingItems[slotId - Stash_SlotIDStart];
    }
    return nullptr;
}

//----- (th2) -------------------------------------------------------------
void Craft_RetrieveAllItems()
{
    Player& player = Players[CurrentPlayerIndex];
    Item tmpItem;
    memcpy( &tmpItem, &player.ItemOnCursor, sizeof(Item) );
    
    auto retrieveItemToPlayer = [&player]( Item* itemPtr ){
        memcpy( &player.ItemOnCursor, itemPtr, sizeof(Item) );
        if( !AutoPutCursorToInventoryOrDrop( false /*allowDropWhenInvFull*/ ) ){
            InvPutItem( CurrentPlayerIndex, player.Row, player.Col );
        }
        itemPtr->ItemCode = IC_M1_NONE;
    };
    
    for( Item& item : GemCombineItems ){
        if( item.ItemCode != IC_M1_NONE && !item.isShadow ){
            retrieveItemToPlayer( &item );
        }
    }
    
    {
        Item& item = SocketingItems[SocketingItemSlotIndex];
        if( item.ItemCode != IC_M1_NONE ){
            retrieveItemToPlayer( &item );
        }
    }
    
    memcpy( &player.ItemOnCursor, &tmpItem, sizeof(Item) );
}

//----- (th2) -------------------------------------------------------------
bool Craft_IsCombinationReady()
{
    Item& combineResultItem = GemCombineItems[GemCombineResultSlotIndex];
    return combineResultItem.ItemCode != IC_M1_NONE && combineResultItem.isShadow;
}

//----- (th2) -------------------------------------------------------------
bool Craft_IsCombinationResultReady()
{
    Item& combineResultItem = GemCombineItems[GemCombineResultSlotIndex];
    return combineResultItem.ItemCode != IC_M1_NONE && !combineResultItem.isShadow;
}

//----- (th2) -------------------------------------------------------------
int Craft_GetCombiningPrice()
{
    Item& combineResultItem = GemCombineItems[GemCombineResultSlotIndex];
    if( Craft_IsCombinationReady() ){
        return combineResultItem.basePrice;
    }else{
        return 0;
    }
}

//----- (th2) -------------------------------------------------------------
constexpr int Craft_Gems_CalculateUpgradePrice( int currentGrade )
{
    // апгрейд тира сокетабла : (5 * грейд камней) ^ 3
    // (т.е., от 125 за 1й грейд до 91125 за 9й)
    int n = 5 * currentGrade;
    return n * n * n;
}

// Static formulae check
static_assert( Craft_Gems_CalculateUpgradePrice(1) == 125, "" );
static_assert( Craft_Gems_CalculateUpgradePrice(9) == 91125, "" );

//----- (th2) -------------------------------------------------------------
int Craft_Gems_CalculateUpgradeFinalPrice( int currentGrade )
{
    int price = Craft_Gems_CalculateUpgradePrice( currentGrade );
    price = BuyPrice(price);
    LimitToMin(price, 1);
    return price;
}

//----- (th2) -------------------------------------------------------------
constexpr int Craft_Socketing_CalculateGemInsertPrice( int currentGrade )
{
    // вставка предмета в сокет : (10 * грейд) ^ 2
    int n = 10 * currentGrade;
    return n * n;
}

// Static formulae check
static_assert( Craft_Socketing_CalculateGemInsertPrice(1) == 100, "" );
static_assert( Craft_Socketing_CalculateGemInsertPrice(10) == 10000, "" );

//----- (th2) -------------------------------------------------------------
int Craft_Socketing_CalculateGemInsertFinalPrice( int currentGrade )
{
    int price = Craft_Socketing_CalculateGemInsertPrice( currentGrade );
    price = BuyPrice(price);
    LimitToMin(price, 1);
    return price;
}

//----- (th2) -------------------------------------------------------------
constexpr int Craft_Socketing_CalculateGemExtractionPrice( int currentGrade )
{
    // изъятие предмета из сокета : (20 * грейд) ^ 2
    int n = 20 * currentGrade;
    return n * n;
}

// Static formulae check
static_assert( Craft_Socketing_CalculateGemExtractionPrice(1) == 400, "" );
static_assert( Craft_Socketing_CalculateGemExtractionPrice(10) == 40000, "" );

//----- (th2) -------------------------------------------------------------
int Craft_Socketing_CalculateGemExtractionFinalPrice( int currentGrade )
{
    int price = Craft_Socketing_CalculateGemExtractionPrice( currentGrade );
    price = BuyPrice(price);
    LimitToMin(price, 1);
    return price;
}

//----- (th2) -------------------------------------------------------------
void Craft_Gems_RefreshCombineResult()
{
    Item& combineResultItem = GemCombineItems[GemCombineResultSlotIndex];
    if (Craft_IsCombinationResultReady()) {
        // Combine result is ready. Don't need to alter it
        return;
    }

    combineResultItem.ItemCode = IC_M1_NONE; // clear result item

    bool allItemsAreInserted = true;
    for (int i = 0; i < GemCombineConsumablesAmount; ++i) {
        Item& item = GemCombineItems[i];
        if (item.ItemCode == IC_M1_NONE) {
            allItemsAreInserted = false;
            break;
        }
    }

    if (allItemsAreInserted) {
        int baseItemIndex = GemCombineItems[0].baseItemIndex;
        bool areTheSame = all_of(GemCombineItems.begin(), GemCombineItems.begin() + GemCombineConsumablesAmount,
            [baseItemIndex](Item& item) { return item.baseItemIndex == baseItemIndex; });
        if (areTheSame) {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex(baseItemIndex);
            if (gemInfoOpt) {
                int currentGrade = gemInfoOpt->grade;
                optional<Gem> nextGemInfoOpt = FindGemInfoByTypeAndGrade(gemInfoOpt->type, currentGrade + 1);
                if (nextGemInfoOpt) {
                    SetPlayerHandItem(combineResultItem, nextGemInfoOpt->baseItemIndex);
                    combineResultItem.MakeSeed();
                    combineResultItem.IsReqMet = true;
                    combineResultItem.isShadow = true;
                    combineResultItem.basePrice = Craft_Gems_CalculateUpgradeFinalPrice(currentGrade);
                }
            }
        }
    }
}

enum Socket_Param
{
	SocketImageWidth     = 40,

	Sockets_LeftX         = 4,
	Sockets_MiddleX       = 25,
	Sockets_RightX        = 45,
	Sockets_TopY          = 4,
	Sockets_MiddleY       = 45,
	Sockets_BottomY       = 86,
	Sockets_MiddleTopY    = 17,
	Sockets_MiddleBottomY = 73,
};

ScreenPos SocketConfiguration[6][6] = {
	/* 1 */ { {Sockets_MiddleX, Sockets_MiddleY}       },
	/* 2 */ { {Sockets_MiddleX, Sockets_MiddleTopY},
	          {Sockets_MiddleX, Sockets_MiddleBottomY} },
	/* 3 */ { {Sockets_MiddleX, Sockets_TopY},
	          {Sockets_MiddleX, Sockets_MiddleY},
              {Sockets_MiddleX, Sockets_BottomY}       },
	/* 4 */ { {Sockets_LeftX,   Sockets_MiddleTopY},
	          {Sockets_RightX,  Sockets_MiddleTopY},
	          {Sockets_LeftX,   Sockets_MiddleBottomY},
	          {Sockets_RightX,  Sockets_MiddleBottomY} },
	/* 5 */ { {Sockets_LeftX,   Sockets_TopY},
	          {Sockets_RightX,  Sockets_TopY},
	          {Sockets_MiddleX, Sockets_MiddleY},
	          {Sockets_LeftX,   Sockets_BottomY},
	          {Sockets_RightX,  Sockets_BottomY}       },
	/* 6 */ { {Sockets_LeftX,   Sockets_TopY},
	          {Sockets_RightX,  Sockets_TopY},
	          {Sockets_LeftX,   Sockets_MiddleY},
	          {Sockets_RightX,  Sockets_MiddleY},
	          {Sockets_LeftX,   Sockets_BottomY},
	          {Sockets_RightX,  Sockets_BottomY}       },
};

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_RefreshSocketSlots( size_t socketAmount )
{
    if( socketAmount > 0 ){
        auto& slots = SocketConfiguration[socketAmount-1];
        for( size_t i = 0; i < socketAmount; ++i ){
            ScreenPos pos = SocketConfiguration[socketAmount-1][i];
            SetupDisplayObject(SocketingSlots.at(i), SocketingSocketsSlot.Left + pos.x, SocketingSocketsSlot.Top + pos.y, SocketImageWidth, SocketImageWidth);
        }
    }
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_RefreshSockets()
{
    Item& item = SocketingItems[SocketingItemSlotIndex];
    if( item.ItemCode != IC_M1_NONE ){
        Craft_Socketing_RefreshSocketSlots( item.socketsAmount );
        
        for( int i = 0; i < SocketingSocketsAmount; ++i ){
            Item& socketItem = SocketingItems.at(i);
            if( i < item.socketsAmount ){
                int baseItemIndex = item.sockets[i];
                if( baseItemIndex != 0 ){
                    SetPlayerHandItem(socketItem, baseItemIndex);
                    socketItem.MakeSeed();
                    socketItem.IsReqMet = true;
                    continue;
                }
            }
            socketItem.ItemCode = IC_M1_NONE;
        }
    }else{
        for( int i = 0; i < SocketingSocketsAmount; ++i ){
            Item& socketItem = SocketingItems.at(i);
            socketItem.ItemCode = IC_M1_NONE;
        }
    }
}

//----- (th2) -------------------------------------------------------------
Item& __fastcall Craft_GetSocketedItem()
{
    return SocketingItems[SocketingItemSlotIndex];
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_DrawItemBacklight() {
    Item& socketedItem = SocketingItems[SocketingItemSlotIndex];

    uchar backlightColor = BLC_NONE;

    if (socketedItem.ItemCode != IC_M1_NONE)
            backlightColor = blColor.main;

    if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START) {
        Item& item = Players[CurrentPlayerIndex].ItemOnCursor;

        if( AllowSocketing(item) ){
            backlightColor = blColor.overcells;
            if (CursorIntoDisplayObject(SocketingItemSlot)){
				backlightColor = blColor.overslot;
			}
        }else if (CursorIntoDisplayObject(SocketingItemSlot)){
			backlightColor = blColor.notavailable;
		}
    }

    if (backlightColor)
        CustomShadowItemSlot(Screen_LeftBorder + SocketingItemSlot.Left, Screen_TopBorder + SocketingItemSlot.Down, SocketingItemSlot.Width, SocketingItemSlot.Heigth, backlightColor);
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_Socketing_Draw()
{
    Item& socketedItem = SocketingItems[SocketingItemSlotIndex];

    Craft_DrawItemBacklight();
    
    if( socketedItem.ItemCode != IC_M1_NONE ){
        Item& item = socketedItem;
        DisplayObject& rect = SocketingItemSlot;
        
        // ShadowItemSlot(rect.Left + Screen_LeftBorder, rect.Down + Screen_TopBorder, rect.Width, rect.Heigth);
        
        int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
        int itemWidth = ItemWidthTable[graphicValue];
        int itemHeight = ItemHeightTable[graphicValue];
        char* itemsCELArray = GetItemSprites( graphicValue );
        
        int x = rect.Left + Screen_LeftBorder + (rect.Width - itemWidth) / 2;
        int y = rect.Down + Screen_TopBorder - (rect.Heigth - itemHeight) / 2;
        
        int colorOfBorder = -1;
        if( Cur.InventoryCellID == (SocketingItemSlotIndex + Stash_SlotIDStart) ){
            colorOfBorder = 197;
            if( item.MagicLevel ){
                colorOfBorder = 181;
            }
            if( !item.IsReqMet ){
                colorOfBorder = 229;
            }
        }
        CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
    }
    
    if( socketedItem.ItemCode != IC_M1_NONE && socketedItem.socketsAmount > 0 ){
        for( int i = 0; i < socketedItem.socketsAmount; ++i ){
            DisplayObject& rect = SocketingSlots.at(i);
            Item& item = SocketingItems.at(i);
            
            int socketPicIndex = (item.ItemCode != IC_M1_NONE) ? 3 : 1;
            Surface_DrawCEL(rect.Left + Screen_LeftBorder, rect.Down + Screen_TopBorder, CraftSocketCEL, socketPicIndex, rect.Width);
            
            if( item.ItemCode != IC_M1_NONE ){
                int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
                int itemWidth = ItemWidthTable[graphicValue];
                int itemHeight = ItemHeightTable[graphicValue];
                char* itemsCELArray = GetItemSprites( graphicValue );
                
                int x = rect.Left + Screen_LeftBorder + (rect.Width - itemWidth) / 2;
                int y = rect.Down + Screen_TopBorder - (rect.Heigth - itemHeight) / 2;
                
				int colorOfBorder = -1;
				if( Cur.InventoryCellID == (i + Stash_SlotIDStart) ){
                    colorOfBorder = 197;
                    if( item.MagicLevel ){
                        colorOfBorder = 181;
                    }
                    if( !item.IsReqMet ){
                        colorOfBorder = 229;
                    }
                }
                CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!item.IsReqMet | item.mirrorImage << 1, colorOfBorder);
            }
        }
    }else{
        Surface_DrawCEL(SocketingSocketsSlot.Left + Screen_LeftBorder, SocketingSocketsSlot.Down + Screen_TopBorder, CraftSocketsLockCEL, 1, SocketingSocketsSlot.Width);
    }

	DrawAddSocketButton(socketedItem);
}

//----- (th2) -------------------------------------------------------------
int Craft_Socketing_GetSlotMouseAbove()
{
    for( int i = 0; i < SocketingSlotAmount; ++i ){
        if( CursorIntoDisplayObject( SocketingSlots[i] ) ){
            return i;
        }
    }
    return -1;
}

//----- (th2) -------------------------------------------------------------
void Craft_ChangeCraftPage( CraftPage craftPage )
{
    Craft_RetrieveAllItems();
    
    switch( craftPage )
    {
        default:
        case CP_Gems:
            Craft_Gems_RefreshCombineResult();
            break;
        case CP_Socketing:
            Craft_Socketing_RefreshSockets();
            break;
    }
    
    CurrentCraftPage = craftPage;
}

//----- (th2) -------------------------------------------------------------
void Craft_RecalcItemsReq()
{
    switch( CurrentCraftPage )
    {
        default:
        case CP_Gems:
            for( int i = 0; i < GemCombineSlotAmount; ++i ){
                Item& item = GemCombineItems[i];
                if( item.ItemCode != IC_M1_NONE ){
					bool isShadow = item.isShadow;
					UpdateItem(item);
					item.isShadow = isShadow;
                }
            }
            Craft_Gems_RefreshCombineResult();
            break;
        case CP_Socketing:
        {
            Item& socketedItem = SocketingItems[SocketingItemSlotIndex];
            if( socketedItem.ItemCode != IC_M1_NONE ){
                UpdateItem( socketedItem );
            }
            Craft_Socketing_RefreshSockets();
            break;
        }
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_Open()
{
    for( auto& item : GemCombineItems ){
        item.ItemCode = IC_M1_NONE;
    }
    
    for( auto& item : SocketingItems ){
        item.ItemCode = IC_M1_NONE;
    }
    
    IsCraftPanelVisible = true;
	IsINVPanelVisible = true;
	
	Craft_ChangeCraftPage( CraftPages[0] );
	
	CurrentDialogIndex = PD_0_None;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_Close()
{
    if( IsCraftPanelVisible ){
        Craft_RetrieveAllItems();
    }
    IsCraftPanelVisible = false;
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_Draw()
{
    char* panelBackground = nullptr;
    switch( CurrentCraftPage ){
    default:
    case CP_Gems:      panelBackground = CraftPanelCEL;  break;
    case CP_Socketing: panelBackground = CraftPanel2CEL; break;
    }
    Surface_DrawCEL(CraftPanel.Left + Screen_LeftBorder, CraftPanel.Down + Screen_TopBorder, panelBackground, 1, CraftPanel.Width);
    
    for( int i = 0, ie = CraftPages.size(); i != ie; ++i ){
        CraftPage pageType = CraftPages[i];
        int index = Craft_TabsStartIndex + i;
        DrawCraftTab( CraftButtons[index], pageType == CurrentCraftPage, CraftPagesNames[pageType] );
    }
    switch( CurrentCraftPage ){
    case CP_Gems     : Craft_Gems_Draw();      break;
    case CP_Socketing: Craft_Socketing_Draw(); break;
    }
    auto& craftActions = CraftActions.at( CurrentCraftPage );
    for( int i = 0, ie = craftActions.size(); i != ie; ++i ){
        CraftAction actionType = craftActions[i];
        int icon = CraftActionInfos[actionType].icon;
        int index = Craft_ActionButtonsStartIndex + i;
        DisplayObject& obj = CraftButtons[index];
        int pressed = CraftButtonsStates[index] ? 2 : 1;
        Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, VisualTradeBtnsCEL, pressed + icon*2, obj.Width);
    }
    int icon = Craft_ExitButtonIcon;
    int index = Craft_ExitButtonStartIndex;
    DisplayObject& obj = CraftButtons[index];
    int pressed = CraftButtonsStates[index] ? 2 : 1;
    Surface_DrawCEL(obj.Left + Screen_LeftBorder, obj.Down + Screen_TopBorder, VisualTradeBtnsCEL, pressed + icon*2, obj.Width);
}

//----- (th2) -------------------------------------------------------------
int Craft_GetSlotMouseAbove()
{
    int result = -1;
    switch( CurrentCraftPage )
    {
        case CP_Gems:
        {
            int i = Craft_Gems_GetSlotMouseAbove();
            if( i >= 0 ){
                Item& item = GemCombineItems[i];
                if( item.ItemCode != IC_M1_NONE ){
                    return i + Stash_SlotIDStart;
                }
            }
            break;
        }
        case CP_Socketing:
        {
            int i = Craft_Socketing_GetSlotMouseAbove();
            if( i >= 0 ){
                Item& item = SocketingItems[i];
                if( item.ItemCode != IC_M1_NONE ){
                    return i + Stash_SlotIDStart;
                }
            }
            break;
        }
        default:
            break;
    }
    return result;
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_MouseMove()
{
    int totalGold = Players[CurrentPlayerIndex].TotalGold;
    
    auto& craftActions = CraftActions.at( CurrentCraftPage );
    for( int i = 0, ie = craftActions.size(); i != ie; ++i ){
        int index = Craft_ActionButtonsStartIndex + i;
        DisplayObject& obj = CraftButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            Tooltip_Clear();
            Tooltip_SetOffsetTop();
            switch(craftActions[i]){
                case CA_Combine:
                {
                    if( Craft_IsCombinationReady() ){
                        int price = Craft_GetCombiningPrice();
                        sprintf(InfoPanelBuffer, "combine for %i gold", price);
                        Tooltip_AddLine(InfoPanelBuffer, totalGold >= price ? C_0_White : C_2_Red);
                    }else if( Craft_IsCombinationResultReady() ){
                        Tooltip_AddLine("retrieve upgraded gem first", C_2_Red);
                    }else{
                        Tooltip_AddLine("place 3 gems of the same grade and type to combine", C_2_Red);
                    }
                    break;
                }
            }
            return;
        }
    }
    
    {
        int index = Craft_ExitButtonStartIndex;
        DisplayObject& obj = CraftButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            Tooltip_Clear();
            Tooltip_SetOffsetTop();
            Tooltip_AddLine("Exit");
        }
    }
    
    MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();
	Cur.InventoryCellID = Craft_GetSlotMouseAbove();
	Tooltip_ShowItemInfo(Cur.InventoryCellID);
}

//----- (th2) -------------------------------------------------------------
void Craft_ButtonsMouseClick()
{
    for( int i = 0, ie = CraftPages.size(); i != ie; ++i ){
        int index = Craft_TabsStartIndex + i;
        DisplayObject& obj = CraftButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            CraftPage pageType = CraftPages[i];
            if( pageType != CurrentCraftPage ){
                Craft_ChangeCraftPage( pageType );
                PlayGlobalSound(S_75_I_TITLEMOV);
            }
        }
    }
    
    auto& craftActions = CraftActions.at( CurrentCraftPage );
    for( int i = 0, ie = craftActions.size(); i != ie; ++i ){
        int index = Craft_ActionButtonsStartIndex + i;
        DisplayObject& obj = CraftButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            CraftButtonsStates[index] = true;
        }
    }
    
    {
        int index = Craft_ExitButtonStartIndex;
        DisplayObject& obj = CraftButtons[index];
        if( CursorIntoDisplayObject(obj) ){
            CraftButtonsStates[index] = true;
        }
    }
}

//----- (th2) -------------------------------------------------------------
bool Craft_Gems_IsItemAllowedToBeConsumable( Item* itemPtr )
{
    if( itemPtr->MagicCode != MC_GEM ){
        return false;
    }
    
    int graphicValue = itemPtr->GraphicValue + CM_12_ITEMS_PICS_START;
    int itemWidth = ItemWidthTable[graphicValue];
    int itemHeight = ItemHeightTable[graphicValue];
    
    if( (itemWidth / CELL_SIZE) != 1 || (itemHeight / CELL_SIZE) != 1 ){
        return false;
    }
    
    return true;
}

//----- (th2) -------------------------------------------------------------
void Craft_Gems_PlaceItemIntoCraftSlot( int dstSlotId )
{
    int srcItemGraphicValue = -1;
    Item* dstItemPtr = &GemCombineItems.at( dstSlotId );
    Player& player = Players[CurrentPlayerIndex];
    srcItemGraphicValue = player.ItemOnCursor.GraphicValue;
    auto newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( dstItemPtr, &player.ItemOnCursor );
    if( player.ItemOnCursor.ItemCode == IC_M1_NONE ){
		FixItemCursor(1);
        SetCursorGraphics(CM_1_NORMAL_HAND);
    }else{
        SetCursorGraphics(newCursorGraphics);
    }
    
    Craft_Gems_RefreshCombineResult();
    
    if( srcItemGraphicValue != -1 ){
        PlayGlobalSound(Flip_InventorySound[ItemFlipTable[srcItemGraphicValue]]);
    }
}

//----- (th2) -------------------------------------------------------------
bool __fastcall Craft_Gems_TryToPlaceItemIntoCraftSlot()
{
    Player& player = Players[CurrentPlayerIndex];
    Item* itemPtr = &player.ItemOnCursor;
    
    int dstSlotId = Craft_Gems_GetSlotMouseAbove();
    if( dstSlotId == GemCombineResultSlotIndex ){
        return false; // can't place item in the cobine result slot
    }

    if( dstSlotId == -1 ){
        return false; // can't place item from the cursor not in the specific slot
    }
    
    if( Craft_Gems_IsItemAllowedToBeConsumable( itemPtr ) ){
        Craft_Gems_PlaceItemIntoCraftSlot( dstSlotId );
        return true;
    }

    return false;
}

//----- (th2) -------------------------------------------------------------
void Craft_Gems_GetItemFromCraftSlot( int slotId )
{
    Item* itemPtr = Craft_GetItemBySlotId( slotId );
    if( !itemPtr ){
        return;
    }
    
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    
    PlayGlobalSound( S_56_I_INVGRAB );
    
    memcpy( &cursorItem, itemPtr, sizeof(Item) );
    itemPtr->ItemCode = IC_M1_NONE;
    
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	FixItemCursor(-1);
    
    Craft_Gems_RefreshCombineResult();
    
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (th2) -------------------------------------------------------------
bool Craft_Gems_TryGetItemFromCraftSlot( int slotId )
{
    Item* itemPtr = nullptr;
    if( Craft_IsCraftSlot( slotId ) ){
        itemPtr = Craft_GetItemBySlotId( slotId );
    }else{
        return false;
    }
    
    if( itemPtr->ItemCode == IC_M1_NONE || itemPtr->isShadow ){
        return false;
    }
    
    Craft_Gems_GetItemFromCraftSlot(slotId);
    
    return true;
}

//----- (th2) -------------------------------------------------------------
void Craft_Gems_LButtonDown()
{
    if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		Craft_Gems_TryGetItemFromCraftSlot(Cur.InventoryCellID);
	}else{
        Craft_Gems_TryToPlaceItemIntoCraftSlot();
    }
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_GetItemFromItemSlot()
{
    Item* itemPtr = &SocketingItems.at( SocketingItemSlotIndex );
    
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    
    PlayGlobalSound( S_56_I_INVGRAB );
    
    memcpy( &cursorItem, itemPtr, sizeof(Item) );
    itemPtr->ItemCode = IC_M1_NONE;
    
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	FixItemCursor(-1);

    Craft_Socketing_RefreshSockets();
    
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_GetItemFromSocketSlot( int dstSlotId )
{
    Item* itemPtr = &SocketingItems.at( dstSlotId );
    
    Player& player = Players[CurrentPlayerIndex];
    Item& cursorItem = player.ItemOnCursor;
    
    Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
    socketedItem.sockets.at( dstSlotId ) = 0;
    UpdateItem( socketedItem );
    
    auto baseItemIndex = itemPtr->baseItemIndex;
    
    optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( baseItemIndex );
    int price = Craft_Socketing_CalculateGemExtractionFinalPrice( gemInfoOpt->grade );
    ClearGoldByInventoryAsPrice( price );
    
    memcpy( &cursorItem, itemPtr, sizeof(Item) );
    
    SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	FixItemCursor(-1);

    Craft_Socketing_RefreshSockets();
    
    PlayGlobalSound( S_1514 ); // OR S_1514
    
    RecalcPlayer(CurrentPlayerIndex, 1);
}

//----- (th2) -------------------------------------------------------------
bool Craft_Socketing_TryGetItemFromCraftSlot( int slotId )
{
    Item* itemPtr = nullptr;
    if( Craft_IsCraftSlot( slotId ) ){
        itemPtr = Craft_GetItemBySlotId( slotId );
    }else{
        return false;
    }
    
    if( itemPtr->ItemCode == IC_M1_NONE ){
        return false;
    }
    
    if( slotId == Stash_SlotIDStart + SocketingItemSlotIndex ){
        // item slot
        Craft_Socketing_GetItemFromItemSlot();
        return true;
    }else{
        // socket slot
        Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
        int dstSlotId = slotId - Stash_SlotIDStart;
        if( socketedItem.ItemCode != IC_M1_NONE && dstSlotId < socketedItem.socketsAmount ){
            auto baseItemIndex = socketedItem.sockets.at( dstSlotId );
            if( baseItemIndex != 0 ){ // Socket slot should not be empty
                optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( baseItemIndex );
                if( gemInfoOpt ){
                    int totalGold = Players[CurrentPlayerIndex].TotalGold;
                    int price = Craft_Socketing_CalculateGemExtractionFinalPrice( gemInfoOpt->grade );
                    if( totalGold >= price ){
                        Craft_Socketing_GetItemFromSocketSlot( dstSlotId );
                        return true;
                    }
                }
            }
        }
        
        // Can't extract gem
        PlayGlobalSound( S_1515 );
    }
    
    return true;
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_PlaceItemIntoItemSlot()
{
    Item* dstItemPtr = &SocketingItems.at( SocketingItemSlotIndex );
    Player& player = Players[CurrentPlayerIndex];
    int srcItemGraphicValue = player.ItemOnCursor.GraphicValue;
    auto newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue( dstItemPtr, &player.ItemOnCursor );
    if( player.ItemOnCursor.ItemCode == IC_M1_NONE ){
		FixItemCursor(1);
		SetCursorGraphics(CM_1_NORMAL_HAND);
    }else{
        SetCursorGraphics(newCursorGraphics);
    }
    
    Craft_Socketing_RefreshSockets();
    
    PlayGlobalSound(Flip_InventorySound[ItemFlipTable[srcItemGraphicValue]]);
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_PlaceItemIntoSocketSlot( int dstSlotId )
{
    Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
    Player& player = Players[CurrentPlayerIndex];
    int srcItemGraphicValue = player.ItemOnCursor.GraphicValue;
    
    auto baseItemIndex = player.ItemOnCursor.baseItemIndex;
    
    socketedItem.sockets.at( dstSlotId ) = baseItemIndex;
    UpdateItem( socketedItem );
    
    optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( baseItemIndex );
    int price = Craft_Socketing_CalculateGemInsertFinalPrice( gemInfoOpt->grade );
    ClearGoldByInventoryAsPrice( price );
    
    player.ItemOnCursor.ItemCode = IC_M1_NONE;
    
    FixItemCursor(1);
	SetCursorGraphics(CM_1_NORMAL_HAND);
    
    Craft_Socketing_RefreshSockets();
    
    PlayGlobalSound(S_1512); // OR S_1512
}

//----- (th2) -------------------------------------------------------------
bool Craft_Socketing_IsGemSuitableForItem( Gem& gemInfo, Item& item )
{
    BaseItem& baseItem = BaseItems[item.baseItemIndex];
    
	// -DFE effect is suatable for body armors only
	if (gemInfo.armorEffect.type == GemEffectType::DFE && baseItem.ItemType == ITEM_2_ARMOR && item.EquippedLocation != EL_3_TORSO) {
        return false;
    }
    
    // Flat +damage effect is forbiden for special class weapons
    if( gemInfo.weaponEffect.type == GemEffectType::Damage || gemInfo.weaponEffect.type == GemEffectType::ToHit){
        if( is(item.ItemCode, IC_18_FLASK, IC_19_TRAP) ){
            return false;
        }
    }
    
    return true;
}

//----- (th2) -------------------------------------------------------------
bool Craft_Socketing_IsSocketingItemReady()
{
    Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
    return socketedItem.ItemCode != IC_M1_NONE;
}

//----- (th2) -------------------------------------------------------------
bool Craft_Socketing_IsGemSuitableForCurrentItem( Gem& gemInfo )
{
    Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
    if( socketedItem.ItemCode != IC_M1_NONE ){
        return Craft_Socketing_IsGemSuitableForItem( gemInfo, socketedItem );
    }
    return false;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall Craft_Socketing_TryToPlaceItemIntoCraftSlot()
{
    Player& player = Players[CurrentPlayerIndex];
    Item& item = player.ItemOnCursor;
    
    int dstSlotId = Craft_Socketing_GetSlotMouseAbove();
    if( dstSlotId == SocketingItemSlotIndex ){ // item slot
        if( AllowSocketing(item) ){
            Craft_Socketing_PlaceItemIntoItemSlot();
            return true;
        }
    }else if( dstSlotId != -1 ){ // socket slot
        if( item.ItemCode != IC_M1_NONE && item.MagicCode == MC_GEM ){
            Item& socketedItem = SocketingItems.at( SocketingItemSlotIndex );
            if( socketedItem.ItemCode != IC_M1_NONE && dstSlotId < socketedItem.socketsAmount ){
                if( socketedItem.sockets.at( dstSlotId ) == 0 ){ // Socket slot should be empty
                    auto baseItemIndex = item.baseItemIndex;
                    optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( baseItemIndex );
                    if( gemInfoOpt ){
                        if( Craft_Socketing_IsGemSuitableForItem( *gemInfoOpt, socketedItem) ){
                            int totalGold = Players[CurrentPlayerIndex].TotalGold;
                            int price = Craft_Socketing_CalculateGemInsertFinalPrice( gemInfoOpt->grade );
                            if( totalGold >= price ){
                                Craft_Socketing_PlaceItemIntoSocketSlot( dstSlotId );
                                return true;
                            }
                        }
                    }
                }
            }
        }
        
        // Can't place gem
        PlayGlobalSound( S_1515 );
    }

    
    return false;
}

//----- (th2) -------------------------------------------------------------
void Craft_Socketing_LButtonDown()
{
    if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
		Craft_Socketing_TryGetItemFromCraftSlot(Cur.InventoryCellID);
	}else{
        Craft_Socketing_TryToPlaceItemIntoCraftSlot();
    }

	if( CursorIntoDisplayObject(AddSocketButton) && AddSocketPicNumber() != SOC_INACTIVE ){
		PlayGlobalSound(S_75_I_TITLEMOV);
		AddSocketPressed = true;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_LButtonDown()
{
	// allow switch craft tabs with item in cursor
	//if( Cur.GraphicsID < CM_12_ITEMS_PICS_START )
	Craft_ButtonsMouseClick();
	switch( CurrentCraftPage ){
	case CP_Gems:      Craft_Gems_LButtonDown();      break;
	case CP_Socketing: Craft_Socketing_LButtonDown(); break;
	}
}

//----- (th2) -------------------------------------------------------------
void Craft_TryToCombine()
{
    if( Craft_IsCombinationReady() ){
        int totalGold = Players[CurrentPlayerIndex].TotalGold;
        int price = Craft_GetCombiningPrice();
        if( totalGold >= price ){
            ClearGoldByInventoryAsPrice( price );
            
            // Remove consumables
            for( int i = 0; i < GemCombineConsumablesAmount; ++i ){
                Item& item = GemCombineItems[i];
                item.ItemCode = IC_M1_NONE;
            }
            
            // Make combine result less shadow
            GemCombineItems[GemCombineResultSlotIndex].isShadow = false;
            
            PlayGlobalSound(S_1510_gem_upgrade_sound);
            return;
        }
    }
    
    // Can't combine
    PlayGlobalSound( S_1515 );
}

//----- (th2) -------------------------------------------------------------
void __fastcall Craft_MouseClick()
{
    auto& craftActions = CraftActions.at( CurrentCraftPage );
    for( int i = 0, ie = craftActions.size(); i != ie; ++i ){
        int index = Craft_ActionButtonsStartIndex + i;
        if( CraftButtonsStates[index] ){
            CraftButtonsStates[index] = false;
            DisplayObject& obj = CraftButtons[index];
            if( CursorIntoDisplayObject(obj) ){
                switch(craftActions[i]){
                    case CA_Combine:
                        Craft_TryToCombine();
                        break;
                }
            }
        }
    }
    
    if( CraftButtonsStates[Craft_ExitButtonStartIndex] ){
        CraftButtonsStates[Craft_ExitButtonStartIndex] = false;
        DisplayObject& obj = CraftButtons[Craft_ExitButtonStartIndex];
        if( CursorIntoDisplayObject(obj) ){
			if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ) CloseInventoryPanel();
            Craft_Close();
            PlayGlobalSound(S_75_I_TITLEMOV);
        }
    }

	if( CurrentCraftPage == CP_Socketing ){
		if( CursorIntoDisplayObject(AddSocketButton) ){
			ClickAddSocketButton();
		}
		AddSocketPressed = false;
	}
}

//----- (th2) -------------------------------------------------------------
bool __fastcall Craft_IsCraftSlot( int slotId )
{
    return IsCraftPanelVisible && slotId >= Stash_SlotIDStart;
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall Craft_GetItemBySlotId( int slotId )
{
    switch( CurrentCraftPage )
    {
        case CP_Gems:
            return Craft_Gems_GetItemBySlotId( slotId );
        case CP_Socketing:
            return Craft_Socketing_GetItemBySlotId( slotId );
        default:
            return nullptr;
    }
}

//----- (th2) -------------------------------------------------------------
int __fastcall JustDropGem( int baseItemIndex, int row, int col )
{
    int itemIndex = -1;
    if( GetItemSpace(row, col, itemIndex) ){
		int qlvl = 0;
		SetupAllItems(itemIndex, baseItemIndex, GetRndSeed(), qlvl, 0, false, 0, 0, false, 0, 0, 0, -1, 0, Difficulty);
	}
    return itemIndex;
}

//----- (th2) -------------------------------------------------------------
void __fastcall DropRandomGemForSpecificGrade( int row, int col, int grade )
{
    optional<Gem> gemInfoOpt = FindRandomGemInfoForSpecificGrade( grade );
    if( gemInfoOpt ){
        JustDropGem( gemInfoOpt->baseItemIndex, row, col );
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall TryToDropGem(int row, int col)
{
    auto dropRandomGemForSpecificGrade = [row, col]( int grade )
    {
        DropRandomGemForSpecificGrade( row, col, grade );
	};
    
    if( !CanGrind() ){
        return;
    }
    
    // вероятность дропа камушков грейда 3: только на думе. с вероятностью RNG( 400 - ( DLVL * 6) )
    //if( Difficulty == DL_2_DOOM ){
    //    auto randomFactor = RNG(400 - ( 6 * DungeonLevel) );
    //    if( randomFactor == 0 ){
    //        dropRandomGemForSpecificGrade( 3 );
    //        return;
    //    }
    //}
    //
    //// вероятность дропа камушков грейда 2 на пурге = 0,5%, на думе: 1%
    //if( Difficulty >= DL_1_PURGATORY ){
    //    auto randomFactor = RNG(100'0);
    //    auto dropChanceInTensOfPercent = (Difficulty == DL_1_PURGATORY) ? 5 : 10;
    //    if( randomFactor < dropChanceInTensOfPercent ){
    //        dropRandomGemForSpecificGrade( 2 );
    //        return;
    //    }
    //}
    //
    //// вероятность дропа камушков 1го грейда = 1% + ( DLVL / 12 ) + ( Difficulty * 2 )
    //{
    //    auto randomFactor = RNG(100);
    //    auto dropChanceInPercent = 1 + (DungeonLevel / 12) + (Difficulty * 2);
    //    if( randomFactor < dropChanceInPercent ){
    //        dropRandomGemForSpecificGrade( 1 );
    //        return;
    //    }
    //}

	// new formula : need 14 tier 1 gems per dlvl by the end of doom + 2 tier 2 gems (+6) + 1/250 chance for grade 3 (+7)
	// all in all worth: from 1/150 chance on dlvl 1 horror to 30/1 by doom's dlvl-24.   that's times 27!
	{
		auto randomFactor = RNG(15'000); //  150 is average amount of monsters per dlvl.. thus, 1/150 chance is roughly 1% chance of drop
		/*const*/ auto dropChance = 60 + Dungeon->level * 10 ; //  0,6% on dlvl-1, 1,8% on dlvl-12, 3% on dlvl-24
        switch (Difficulty) {
            case DL_0_HORROR:       dropChance =  60 + Dungeon->level * 10; break; // (grade 1,  70 - 300) 0.46% - 2%
            case DL_1_PURGATORY:    dropChance = 300 + Dungeon->level * 10; break; // (grade 1, 310 - 540) 2.06% - 3.6%
            case DL_2_DOOM:         dropChance = 180 + Dungeon->level *  5; break; // (grade 2, 190 - 300) 1.26% - 2%
            case DL_3_CRUCIBLE:     dropChance = 300 + Dungeon->level * 10; break; // (grade 2, 310 - 540) 2.06% - 3.6%
            case DL_4_TORMENT:      dropChance = 180 + Dungeon->level *  5; break; // (grade 3, 190 - 300) 1.26% - 2%
            case DL_5_AGONY:        dropChance = 300 + Dungeon->level * 10; break; // (grade 3, 310 - 540) 2.06% - 3.6%
            case DL_6_TERROR:       dropChance = 180 + Dungeon->level *  5; break; // (grade 4, 190 - 300) 1.26% - 2%
            case DL_7_DESTRUCTION:  dropChance = 300 + Dungeon->level * 10; break; // (grade 4, 310 - 540) 2.06% - 3.6%
            case DL_8_INFERNO:      dropChance = 170 + Dungeon->level *  4; break; // (grade 5, 174 - 266) 1.16% - 1.77%
        }
        if (GameMode == GM_SURVIVAL) {
            dropChance /= 3;
        }
		if (randomFactor < dropChance) {
            int droppedGemGrade = by(Difficulty, 1, 1, 2, 2, 3, 3, 4, 4, 5);
			dropRandomGemForSpecificGrade( droppedGemGrade );
			return;
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall TryToAddSocketsToItem( Item& item, int qlvl, uchar difficulty ) // TODO: remove difficulty dependencied from regeneration (move it to smart drop)
{
	// сокетовыми могут быть только белые айтемы. оружие и броня. кольца - без крафта
	if( item.MagicLevel == ML_0_USUAL && is(item.TypeID, ITEM_1_WEAPON, ITEM_2_ARMOR) ){
        // белый айтем будет сокетовый с вероятностью ( (50 - ( ilvl / 2 )) / (Difficulty + 2))%
        // т.е. если уровень предмета = 54, то шанс что он станет сокетовым, равен 27% на хорроре, 13% на пурге и 6% на думе.
        int randomFactor = RNG(100);
        int becomeSocketableChanceInPercent = (40 - ( qlvl / 10 )) / (2 + difficulty / 2); // needs better formula for 9 difs (updated once but could need more)
        if( randomFactor < becomeSocketableChanceInPercent ){
            // количество сокетов на любом айтеме генерируется по формуле:
            // (difficulty + 1) + (цикл на 50% шанс добавить +1 сокет, повторяемый (difficulty + 1) количество ) раз
            // тогда если начальный шанс получить сокетовый айтем на думе с 3 дырками = 6%,
            // то шанс на 4 дырки будет равен 3%, на 5: 1,5% и на 6: 0,75%
            int sockets = by(difficulty, 1, 1, 2, 2, 3, 3, 3, 3, 3);
            //LimitToMax(socketsAmount, 3);
            int mfier =         by(difficulty, 2, 3, 3, 4, 3, 3, 3, 3, 3);
            for( int i = 0, ie = mfier; i != ie; ++i ){ // 1-3, 1-4, 2-5, 2-6, 3-6, 3-6, 3-6, 3-6, 3-6
                int addSocketChance = 320 + difficulty * 10;
                if( RNG(1'000) < addSocketChance ){
                    ++sockets;
                }else{
                    break; // no more sockets
                }
            }
            //LimitToMax(socketsAmount, by(Difficulty, 3, 4, 5, 6, 6, 6, 6, 6, 6));
            item.socketsBase = sockets;
			item.socketsAmount = sockets;
			sprintf(item.FullMagicalItemName, "Socketed %s", item.Name);
        }
    }
}


