#include "stdafx.h"

bool IsPowerupPanelVisible;
DisplayObject PowerupPanel;
std::array<DisplayObject, 15> PowerupButtons;   // 9 upgrade buttons
DisplayObject PowerupLeaveButton;
Item PowerupItem;            
char PowerupString[64];
bool IsPowerupPressed;
char* PowerupPanelCEL = nullptr;
extern char* LongButtonCEL;
int PowerupLevelPrices[20] = {
	5'000,        // level 1
	15'000,       // level 2
	35'000,       // level 3
	65'000,       // level 4
	105'000,      // level 5
	165'000,      // level 6
	245'000,      // level 7
	350'000,      // level 8
	500'000,      // level 9
	700'000,      // level 10
	1'000'000,    // level 11
	2'000'000,    // level 12
	3'000'000,    // level 13
	4'000'000,    // level 14
	5'000'000,    // level 15
	6'000'000,    // level 16
	7'000'000,    // level 17
	8'000'000,    // level 18
	9'000'000,    // level 19
	10'000'000    // level 20
};

// Custom prices for Ironman/Nightmare/Speedrun modes
int PowerupLevelPrices_ExtraModes[20] = {
	4'000,        // level 1
	10'000,       // level 2
	18'000,       // level 3
	28'000,       // level 4
	40'000,       // level 5
	55'000,       // level 6
	73'000,       // level 7
	95'000,       // level 8
	121'000,      // level 9
	152'000,      // level 10
	188'000,      // level 11
	230'000,      // level 12
	278'000,      // level 13
	333'000,      // level 14
	396'000,      // level 15
	468'000,      // level 16
	550'000,      // level 17
	644'000,      // level 18
	752'000,      // level 19
	1'000'000     // level 20
};

const int* GetPowerupPriceArray()
{
	if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN)) {
		return PowerupLevelPrices_ExtraModes;
	}
	return PowerupLevelPrices;
}

int GetPowerupPrice(int level)
{
	if (level >= 20) level = 19;  // Clamp to max index
	return GetPowerupPriceArray()[level];
}

void __fastcall Powerups_Open()
{
	SetupPowerupPanel();
	IsChatHistoryVisible = false;
	IsPowerupPanelVisible = true;
	IsINVPanelVisible = true;
	CurrentDialogIndex = PD_0_None;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

void __fastcall Powerups_Close()
{
	if (IsPowerupPanelVisible) {
		IsPowerupPanelVisible = false;
		IsINVPanelVisible = false;
	}
}

int PowerupButtonPicNumber(bool isLeaveButton, int buttonIndex)
{
	if (isLeaveButton) {
		return IsPowerupPressed && CursorIntoDisplayObject(PowerupLeaveButton) ? N_ENCH_PRESS : N_ENCH;
	}
	else {
		Player& player = Players[CurrentPlayerIndex];
		int level = player.powerupLevel[buttonIndex];
		int finalPrice = BuyPrice(GetPowerupPrice(level));

		if (level >= 20 || (uint)finalPrice > CalcTotalGold(CurrentPlayerIndex)) {
			return ENCH_INACTIVE;
		}
		else {
			if (CursorIntoDisplayObject(PowerupButtons[buttonIndex])) {
				return IsPowerupPressed ? ENCH_PRESS : ENCH_HIGH;
			}
			else {
				return ENCH_AVAILABLE;
			}
		}
	}
	return BT_NONE;
}

int GetPowerupDisplayValue(int powerupIndex, int level)
{
	Player& player = Players[CurrentPlayerIndex];

	// If at max level (20), show level 20 value (not level 21)
	int displayLevel = level;
	if (level >= 20) {
		displayLevel = 19; // Show level 20 value
	}

	// Special case for damage (index 1) on Trapper and Bombardier
	if (powerupIndex == 1 && (player.fullClassId == PFC_TRAPPER || player.fullClassId == PFC_BOMBARDIER)) {
		return (displayLevel + 1) * 5; // +5% per level
	}

	// Custom display scaling per powerup type
	switch (powerupIndex) {
	case 0: // armor
		return (displayLevel + 1) * 2;
	case 1: // damage (for other classes)
		return (displayLevel + 1) * 2;
	case 2: // accuracy
		return (displayLevel + 1) * 2;
	case 3: // resistance
		return displayLevel + 1;
	case 4: // life
		return (displayLevel + 1) * 5;
	case 5: // mana
		return (displayLevel + 1) * 5;
	case 6: // %gold find
		return (displayLevel + 1) * 3;
	case 7: // stun thr
		return displayLevel + 1;
	case 8: // dfe
		return displayLevel + 1;
	case 9: // life regen
		return displayLevel + 1;
	case 10: // mana regen
		return displayLevel + 1;
	case 11: // melee res.
		return displayLevel + 1;
	case 12: // arrow res.
		return displayLevel + 1;
	case 13: // %xp gain
		return (displayLevel + 1) * 3;
	case 14: // all stats
		return displayLevel + 1;
	default:
		return displayLevel + 1;
	}
}

void __fastcall Powerups_DrawButtons()
{
	Player& player = Players[CurrentPlayerIndex];
	for (int i = 0; i < 15; i++) {
		auto& btn = PowerupButtons[i];

		Surface_DrawCEL(
			btn.Left + Screen_LeftBorder,
			btn.Down + Screen_TopBorder,
			LongButtonCEL,
			PowerupButtonPicNumber(false, i),
			btn.Width
		);

		const char* powerupNames[15] = {
			"armor", "damage", "accuracy", "resistance", "life",
			"mana", "gold find", "stun thr", "dfe", "life regen",
			"mana regen", "melee res.", "arrow res.", "xp gain", "all stats"
		};

		int currentLevel = player.powerupLevel[i];
		int finalPrice = BuyPrice(GetPowerupPrice(currentLevel));

		// Get current total value using YOUR function
		int currentValue = GetPowerupDisplayValue(i, currentLevel);

		// Check if we need to show % sign
		bool showPercent = false;

		// Gold find and XP gain always show %
		if (i == 6 || i == 13) {
			showPercent = true;
		}

		// Damage shows % for Trapper and Bombardier
		if (i == 1 && (player.fullClassId == PFC_TRAPPER || player.fullClassId == PFC_BOMBARDIER)) {
			showPercent = true;
		}

		// NEW: for DFE (index 8), show minus instead of plus
		bool isDFE = (i == 8);
		char plusMinus = isDFE ? '-' : '+';

		if (currentLevel >= 20) {
			if (showPercent) {
				sprintf(PowerupString, L("%c%d%% %s (MAX)"), plusMinus, currentValue, L(powerupNames[i]));
			}
			else {
				sprintf(PowerupString, L("%c%d %s (MAX)"), plusMinus, currentValue, L(powerupNames[i]));
			}
		}
		else {
			if (showPercent) {
				sprintf(PowerupString, L("%c%d%% %s, %d gold"), plusMinus, currentValue, L(powerupNames[i]), finalPrice);
			}
			else {
				sprintf(PowerupString, L("%c%d %s, %d gold"), plusMinus, currentValue, L(powerupNames[i]), finalPrice);
			}
		}

		DrawText(
			PowerupPanel.Left + 161 - GetTextWidth(PowerupString) / 2,
			PowerupPanel.Top + 112 + 21 * i - 5,
			80 + PowerupPanel.Left,
			PowerupString,
			0
		);
	}
}

void __fastcall Powerups_Draw()
{
	Player& player = Players[CurrentPlayerIndex];
	int panel_frame = 5;
	switch (player.fullClassId) {
	case PFC_WARLOCK:
		panel_frame = 3; // pentagram version
		break;
	case PFC_TEMPLAR:
		panel_frame = 7;
		break;
	default:
		panel_frame = 5;
	}
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN)) {
		panel_frame = 3;
	}
	if (player.ClassID == PC_3_MONK) { panel_frame = 8; }
	// (th4) Dark Campaign: Jaldabaoth's powerups in the hub always use the dark
	// pentagram panel, whatever the class -- last, so it wins over the class frames.
	if (GameMode == GM_DARK_CAMPAIGN) { panel_frame = 3; }
	Surface_DrawCEL(PowerupPanel.Left + Screen_LeftBorder, PowerupPanel.Down + Screen_TopBorder, PowerupPanelCEL, panel_frame, PowerupPanel.Width);
	Powerups_DrawButtons();
	Surface_DrawCEL(PowerupLeaveButton.Left + Screen_LeftBorder, PowerupLeaveButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, PowerupButtonPicNumber(true, 0), 33);
}

void SetupPowerupPanel()
{
	SetupPanelRect(PowerupPanel, LeftPanel, GUI_PanelWidth, GUI_PanelHeight);
	SetupDisplayObject(PowerupLeaveButton, PowerupPanel.Left + 278, PowerupPanel.Top + 450 - 32, 33, 32);
	for (int i = 0; i < 15; i++) {
		SetupDisplayObject(PowerupButtons[i],
			PowerupPanel.Left + 15,
			PowerupPanel.Top - 1 + 112 - 20 + 21 * i,
			292,
			20);
	}
}

//------------------

void __fastcall Powerup_MouseClick()
{
	if (CursorIntoDisplayObject(PowerupLeaveButton)) {
		Powerups_Close();
	}
	IsPowerupPressed = false;
}

void __fastcall Powerups_LButtonDown()
{
	Player& player = Players[CurrentPlayerIndex];

	// Leave button
	if (CursorIntoDisplayObject(PowerupLeaveButton)) {
		PlayGlobalSound(S_75_I_TITLEMOV);
		IsPowerupPressed = true;
		Powerups_Close();
		return;
	}

	// upgrade buttons
	for (int i = 0; i < 15; i++) {
		if (CursorIntoDisplayObject(PowerupButtons[i])) {

			if (player.powerupLevel[i] >= 20) {
				// maxxed, can't buy
				return;
			}		
			int finalPrice = BuyPrice(GetPowerupPrice(player.powerupLevel[i]));
			if (player.TotalGold >= finalPrice) {
				ClearGoldByInventoryAsPrice(finalPrice);
				player.powerupLevel[i]++;
				PlayGlobalSound(S_1850_POWERUP);
				IsPowerupPressed = true;
			}
			return;
		}
	}
}

void __fastcall Powerup_MouseMove()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();
	if (CursorIntoDisplayObject(PowerupLeaveButton)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine(L("Exit"));
	}
}