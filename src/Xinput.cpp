#include "stdafx.h"

CXBOXController curXboxController = 1;
bool XinputAutoTargettingOff = true;
int XinputAutoTargettingOffTimer = 0;
bool XinputOnOFF = false;
bool IsXinputGuideVisible = false;
bool XcelLoaded = false;
int XWelcomeMessageTime = 400;
char* XGuideCel;
int XinputNotInUseTime = 0;
bool XinputDontHideCursor();
int XinputArrowUPTimer = 0, XinputArrowDOWNTimer = 0;

//----- (th2) -------------------------------------------------------------
void XinputHandleMouseMove()
{
	if( !IsController() ) return;

	int x;
	int y;

	x = curXboxController.GetState().Gamepad.sThumbRX; //Get Right Joystick horizontal value -32767 32767
	y = curXboxController.GetState().Gamepad.sThumbRY; //Get Right Joystick vertical value -32767 32767

	if (x < X_DEADZONE_R && x > -X_DEADZONE_R) x = 0;
	if (y < X_DEADZONE_R && y > -X_DEADZONE_R) y = 0;
	if (abs(x) > 0) {
		if (abs(x) < 10000) {
			CursorX += x / 1800;
		}
		else {
			CursorX += x / 900;
		}
	}
	if (abs(y) > 0) {
		if (abs(y) < 10000) {
			CursorY -= y / 1800;
		}
		else {
			CursorY -= y / 900;
		}
	}
	LimitToRange(CursorX, 0, ScreenWidth);
	LimitToRange(CursorY, 0, ScreenHeight);
}

//----- (th2) -------------------------------------------------------------
void XinputVirtualKeyboard()
{
	if( ! IsController() ) return;

	INPUT ip;
	// ...
		// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = 0; // 0 for key press
	///

	curXboxController.UpdateButtonsStatus();

	if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_START) == XINPUT_KEYSTROKE_KEYDOWN || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_B) == XINPUT_KEYSTROKE_KEYDOWN) {
		ip.ki.wVk = VK_ESCAPE;
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP) == XINPUT_KEYSTROKE_KEYDOWN || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_LEFT) == XINPUT_KEYSTROKE_KEYDOWN) {
		ip.ki.wVk = VK_UP;
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN) == XINPUT_KEYSTROKE_KEYDOWN || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_RIGHT) == XINPUT_KEYSTROKE_KEYDOWN) {
		ip.ki.wVk = VK_DOWN;
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) == XINPUT_KEYSTROKE_KEYDOWN) {
		ip.ki.wVk = VK_RETURN;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

//----- (th2) -------------------------------------------------------------
void XinputFixCursorPosition()
{
	if( IsController() && !XinputNotInUse() ){
		CursorX = ScreenWidth / 2;
		CursorY = ScreenHeight / 2;
	}
}

//----- (th2) -------------------------------------------------------------
void XinputHandleAButtonLeftClick()
{
	bool lastShift = IsShiftPressed;
	IsShiftPressed = curXboxController.GetState().Gamepad.bRightTrigger > 100 || IsShiftPressed;
	if( IsCtrlPressed ){
		IsShiftPressed = false;
	}
	if( CurMon != -1 && Dungeon == DUN_0_TOWN && !Dungeon->isQuest && !CurrentDialogIndex && !IsVisualTradePanelVisible && !IsStashPanelVisible && !IsCraftPanelVisible && !IsEnchantPanelVisible && !IsQuenchPanelVisible && !IsTransmutePanelVisible) {
		NetSendCmdLocParam1(1, CMD_29_TALKXY, Cur.Row, Cur.Col, CurMon);
	}else if( true ){
		if( Menu_CheckMouseClick(MOUSE_BUTTON_DOWN) || TalkPanel_CheckMouseClick() || NetCursorItemId ){
		}else if( IsPlayerDead ){ 
			Check_MenuORSendMessage();
		}else if( GamePaused == 2 ){
		}else if( IsPopupScreenOpen ){
			ClosePopupScreen();
		}else if( SelectCurSpellMode ){ 
			SelectCurrentSpell();
		}else if( CursorInMainPanel() ){
			if( HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
			}else if( !TalkPanelMode && !IsGoldSplitPanelVisible && !IsGameMenuActive() ){
				if( CursorIntoBelt() ) Inventory_LButtonClick();
			}
			CheckMainPanelRegion();
			if (Cur.GraphicsID > CM_1_NORMAL_HAND && Cur.GraphicsID < CM_12_ITEMS_PICS_START){
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
		}else if( IsGameMenuActive() || CursorSkillApply() ){
		}else if( IsQUESTPanelVisible && CursorIntoDisplayObject(QuestPanelRect) ){
			if( CursorIntoDisplayObject(QuestPanelTextAreaRect) ){
				QuestPanel_CheckLButtonClick();
			}
		}else if( IsCHARPanelVisible && CursorIntoDisplayObject(CharPanelRect) ){
			CharPanel_MouseDown();
		}else if( IsStashPanelVisible && CursorIntoDisplayObject(VisualStashPanel) ){
			Stash_LButtonDown();
		}else if( IsEnchantPanelVisible && CursorIntoDisplayObject(EnchantPanel) ){
			Enchant_LButtonDown();
		}else if( IsQuenchPanelVisible && CursorIntoDisplayObject(QuenchPanel) ){
			Quench_LButtonDown();
		}else if( IsTransmutePanelVisible && CursorIntoDisplayObject(TransmutePanel) ){
			Transmute_LButtonDown();
		}else if (IsVisualTradePanelVisible && CursorIntoDisplayObject(VisualTradePanel) ){
			VisualTrade_LButtonDown();
		}else if( IsCraftPanelVisible && CursorIntoDisplayObject(CraftPanel) ){
			Craft_LButtonDown();
		}else if( IsINVPanelVisible && CursorIntoDisplayObject(InventoryPanelRect) ){
			if( HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
			}else if( !IsGoldSplitPanelVisible ){
				Inventory_LButtonClick();
			}
		}else if( IsSpellBookVisible && CursorIntoDisplayObject(SpellBookRect) ){
			BookPanel_CkeckLButtonClick();
		}else if( IsPerksPanelVisible && CursorIntoDisplayObject(PerksPanelPos) ){
			HandlePerksWindowClick();
		}else if( IsInfoWindowVisible && CursorIntoDisplayObject(InfoWindowRect) ){
			InfoWindow_MouseDown();
		}else if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
			DropCursorItemBeforeTrig();
			if( NetItem ) SetCursorGraphics(Players[CurrentPlayerIndex].ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
			FixItemCursor(-1);
		}else if( getPlayerPerksPointsLeft(PERK_TYPE_COMMON) > 0 && !SelectCurSpellMode ){
			CheckPerkUpButton();
		}
		if (Players[CurrentPlayerIndex].AvailableLvlPoints && !SelectCurSpellMode) {
			CheckLevelUpButton();
		}
	}
	IsShiftPressed = lastShift;
}

//----- (th2) -------------------------------------------------------------
void XinputMain()
{
	if( IsController() ){
		//XinputHandleMouseMove();
		Player& player = Players[CurrentPlayerIndex];
		curXboxController.UpdateButtonsStatus();
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_BACK) && curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_START)) IsXinputGuideVisible = true;
		else IsXinputGuideVisible = false;
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_START) == XINPUT_KEYSTROKE_KEYDOWN && !IsXinputGuideVisible) KeyPressHandler(VK_27_ESC_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_Y) == XINPUT_KEYSTROKE_KEYDOWN) IsINVPanelVisible ? ChangeAltWeaponPairToNext(false) : ChangeAltWeaponPairToNext(true);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_RIGHT_THUMB) == XINPUT_KEYSTROKE_KEYDOWN) ToggleInventoryPanel();
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_LEFT_THUMB) == XINPUT_KEYSTROKE_KEYDOWN) ToggleCharPanel();
		if (Speech_IsPanelVisible) {
			if ( curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_X) == XINPUT_KEYSTROKE_KEYDOWN
			  || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) == XINPUT_KEYSTROKE_KEYDOWN
			  || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_B) == XINPUT_KEYSTROKE_KEYDOWN
				) if (Speech_IsPanelVisible) KeyPressHandler(VK_27_ESC_KEY);
		}
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_B) == XINPUT_KEYSTROKE_KEYDOWN) {
			if (CurrentMenuWindowPtr || CurrentDialogIndex || IsVisualTradePanelVisible || IsStashPanelVisible || IsCraftPanelVisible || IsEnchantPanelVisible || IsQuenchPanelVisible || IsTransmutePanelVisible)
				KeyPressHandler(VK_27_ESC_KEY);
			else Mouse_RButtonDown(0);
		}

		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_X) == XINPUT_KEYSTROKE_KEYDOWN) {
			if (Cur.ItemID != -1) {
				NetSendCmdLocParam1(1, IsINVPanelVisible ? CMD_41_GOTOGETITEM : CMD_42_GOTOAGETITEM, Cur.Row, Cur.Col, Cur.ItemID);
			}
			if (Cur.ObjectIndex != -1) {
				NetSendCmdLocParam1(1, CMD_16_OPOBJXY, Cur.Row, Cur.Col, Cur.ObjectIndex);
			}
		}
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_LEFT_SHOULDER) == XINPUT_KEYSTROKE_KEYDOWN) {
			for (int i = 0; i < 8; ++i) {
				if (player.BeltInventory[i].ItemCode != IC_M1_NONE) {
					RightClickOnItem(CurrentPlayerIndex, IS_50_47_Belt_Start + i, false, true);
					break;
				}
				else {
					continue;
				}
			}
		}
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_RIGHT_SHOULDER) == XINPUT_KEYSTROKE_KEYDOWN) {
			for (int i = 7; i >= 0; --i) {
				if (player.BeltInventory[i].ItemCode != IC_M1_NONE) {
					RightClickOnItem(CurrentPlayerIndex, IS_50_47_Belt_Start + i, false, true);
					break;
				}
				else {
					continue;
				}
			}
		}
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) == XINPUT_KEYSTROKE_KEYDOWN) if (CurrentMenuWindowPtr > 0 || CurrentDialogIndex) KeyPressHandler(VK_RETURN);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP) == XINPUT_KEYSTROKE_KEYDOWN) if (CurrentMenuWindowPtr > 0 || CurrentDialogIndex) KeyPressHandler(VK_38_UP_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN) == XINPUT_KEYSTROKE_KEYDOWN) if (CurrentMenuWindowPtr > 0 || CurrentDialogIndex)  KeyPressHandler(VK_40_DOWN_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_LEFT) == XINPUT_KEYSTROKE_KEYDOWN) if (CurrentMenuWindowPtr > 0) KeyPressHandler(VK_37_LEFT_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_RIGHT) == XINPUT_KEYSTROKE_KEYDOWN) if (CurrentMenuWindowPtr > 0) KeyPressHandler(VK_39_RIGHT_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP)) if (AutomapMode && AltHighlight && !(CurrentMenuWindowPtr || CurrentDialogIndex)) KeyPressHandler(VK_38_UP_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) if (AutomapMode && AltHighlight && !(CurrentMenuWindowPtr || CurrentDialogIndex)) KeyPressHandler(VK_40_DOWN_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_LEFT)) if (AutomapMode && AltHighlight && !(CurrentMenuWindowPtr || CurrentDialogIndex)) KeyPressHandler(VK_37_LEFT_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_RIGHT)) if (AutomapMode && AltHighlight && !(CurrentMenuWindowPtr || CurrentDialogIndex)) KeyPressHandler(VK_39_RIGHT_ARROW_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_BACK) == XINPUT_KEYSTROKE_KEYDOWN) if ((AutomapMode < 2 || AutomapZoomFactor == 15)) { ToggleAutomap(); } else { ZoomAutomapMinus(); };
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) == XINPUT_KEYSTROKE_KEYDOWN) XinputHandleAButtonLeftClick();
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_X) == XINPUT_KEYSTROKE_KEYDOWN) 
			if (Cur.InventoryCellID != -1) {
				int ctrlOldState = IsCtrlPressed;
				IsCtrlPressed = true;
				XinputHandleAButtonLeftClick();
				IsCtrlPressed = ctrlOldState;
			}
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) == XINPUT_KEYSTROKE_KEYUP && XinputAutoTargettingOff) OnMouseLButtonUp();

		bool isRTriggerOn = curXboxController.GetState().Gamepad.bRightTrigger > 100;

		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP) == XINPUT_KEYSTROKE_KEYDOWN) if (!CurrentMenuWindowPtr && !CurrentDialogIndex && !IsPerksPanelVisible && !(AutomapMode && AltHighlight)) isRTriggerOn ? KeyPressHandler(VK_112_F1_KEY) : KeyPressHandler(VK_116_F5_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_RIGHT) == XINPUT_KEYSTROKE_KEYDOWN) if (!CurrentMenuWindowPtr && !CurrentDialogIndex && !IsPerksPanelVisible && !(AutomapMode && AltHighlight)) isRTriggerOn ? KeyPressHandler(VK_113_F2_KEY) : KeyPressHandler(VK_117_F6_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN) == XINPUT_KEYSTROKE_KEYDOWN) if (!CurrentMenuWindowPtr && !CurrentDialogIndex && !IsPerksPanelVisible && !(AutomapMode && AltHighlight)) isRTriggerOn ? KeyPressHandler(VK_114_F3_KEY) : KeyPressHandler(VK_118_F7_KEY);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_LEFT) == XINPUT_KEYSTROKE_KEYDOWN) if (!CurrentMenuWindowPtr && !CurrentDialogIndex && !IsPerksPanelVisible && !(AutomapMode && AltHighlight)) isRTriggerOn ? KeyPressHandler(VK_115_F4_KEY) : KeyPressHandler(VK_119_F8_KEY);

		if (!CurrentMenuWindowPtr) {
			if (curXboxController.GetState().Gamepad.bLeftTrigger > 100) {
				AltHighlight = true;
			}
			else {
				AltHighlight = false;
			}
		}

		/*switch (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) {
		case XINPUT_KEYSTROKE_KEYDOWN: AltHighlight = true; break;
		case XINPUT_KEYSTROKE_KEYUP: AltHighlight = false; break;
		}*/
	}
}

//----- (th2) -------------------------------------------------------------
void XinputSpellCastingPositioning(Player& player, int currentSpellNumber, int& row, int& col)
{
	if( IsController() && !XinputAutoTargettingOff ){
		if (CurMon == -1) {
			int vd, hd;
			switch (player.dir) {
			case DIR_0_D:  vd = 1;	hd = 1;  break;
			case DIR_1_DL:	 vd = 0;	hd = 1; break;
			case DIR_2_L:  vd = -1; hd = 1; break;
			case DIR_3_UL:  vd = -1; hd = 0; break;
			case DIR_4_U:  vd = -1; hd = -1; break;
			case DIR_5_UR: vd = 0;	hd = -1; break;
			case DIR_6_R:  vd = 1;	hd = -1; break;
			case DIR_7_DR:  vd = 1;	hd = 0; break;
			}
			row = player.NextRow + (vd * (currentSpellNumber == PS_23_TELEPORT ? 10 : 2));
			col = player.NextCol + (hd * (currentSpellNumber == PS_23_TELEPORT ? 10 : 2));
		}
		else {
			if (is(currentSpellNumber, PS_6_FIRE_WALL, PS_40_LIGHTING_WALL)) {
				int vd, hd;
				switch (Monsters[CurMon].Orientation) {
				case DIR_0_D:  vd = 1;	hd = 1;  break;
				case DIR_1_DL:	 vd = 0;	hd = 1; break;
				case DIR_2_L:  vd = -1; hd = 1; break;
				case DIR_3_UL:  vd = -1; hd = 0; break;
				case DIR_4_U:  vd = -1; hd = -1; break;
				case DIR_5_UR: vd = 0;	hd = -1; break;
				case DIR_6_R:  vd = 1;	hd = -1; break;
				case DIR_7_DR:  vd = 1;	hd = 0; break;
				}
				row = Monsters[CurMon].NextRow + (2 * vd);
				col = Monsters[CurMon].NextCol + (2 * hd);
			}
			else {
				row = Monsters[CurMon].NextRow;
				col = Monsters[CurMon].NextCol;
			}
		}
		Cur.GroundRow = row;
		Cur.GroundCol = col;
	}
}

//----- (th2) -------------------------------------------------------------
int XinputFindClosestObject(Player& player, bool tagretObject)
{

	int closestDistanceObjectIndex = -1;
	uint closestObjectrDistance = -1;
	uint objectDistance = -1;
	int deltaRow;
	int deltaCol;

	for (int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++) {

		int objectIndex = ObjectActive[objectIndexIndex];
		Object& object = Objects[objectIndex];

		int baseObjectIndex = object.BaseObjectIndex;
		deltaRow = abs(object.Row - player.NextRow);
		deltaCol = abs(object.Col - player.NextCol);

		if (object.selectable == 0) {
			continue;
		}

		objectDistance = deltaRow < deltaCol ? deltaCol : deltaRow;

		if (objectDistance > 1)
			continue;

		if (objectDistance < closestObjectrDistance) {
			closestObjectrDistance = objectDistance;
			closestDistanceObjectIndex = objectIndex;
		}
	}
	if (closestDistanceObjectIndex != -1 && tagretObject) {
		Cur.ObjectIndex = closestDistanceObjectIndex;
	}
	return closestObjectrDistance;
}

//----- (th2) -------------------------------------------------------------
int XinputFindClosestMonster(Player& player, bool tagretMonster)
{

	VisibleMonster visibleMonsters[Monsters_Max_Count];
	int visibleMonstersCount = 0;
	uint closestMonsterDistance = -1;
	int closestDistanceMonsterIndex = -1;

	for (int i = 0; i < MonstersCount; i++)
	{
		int monsterIndex = MonsterIndexes[i];
		Monster& monster = Monsters[monsterIndex];

		if (FlagMap[monster.NextRow][monster.NextCol] & CF_64_VISIBLE_BY_CURSOR && monster.goal != MG_0_DEATH && !IsPlayerSummon(monsterIndex)) {
			visibleMonsters[visibleMonstersCount].monsterindex = monsterIndex;
			visibleMonstersCount++;
		}
	}

	if (visibleMonstersCount > 0) {
		for (int i = 0; i < visibleMonstersCount; i++)
		{
			Monster& visibleMonster = Monsters[visibleMonsters[i].monsterindex];
			int deltaRow = abs(visibleMonster.NextRow - player.NextRow);
			int deltaCol = abs(visibleMonster.NextCol - player.NextCol);
			uint monsterDistance = deltaRow < deltaCol ? deltaCol : deltaRow;
			//visibleMonster.distanceToPlayer = monsterDistance;
			if (monsterDistance < closestMonsterDistance) {
				closestMonsterDistance = monsterDistance;
				closestDistanceMonsterIndex = visibleMonsters[i].monsterindex;
			}
		}
	}

	if (closestDistanceMonsterIndex != -1 && tagretMonster) {
		CurMon = closestDistanceMonsterIndex;
	}
	return closestMonsterDistance;
}

//----- (th2) -------------------------------------------------------------
void XinputFindClosestTowner(Player& player)
{
	int closestDistanceObjectIndex = -1;
	uint closestObjectrDistance = -1;
	uint objectDistance = -1;
	int deltaRow;
	int deltaCol;

	for (int townerIndex = 0; townerIndex < TownersCount; townerIndex++) {
		Towner& towner = Towners[townerIndex];

		deltaRow = abs(towner.Row - player.NextRow);
		deltaCol = abs(towner.Col - player.NextCol);

		objectDistance = deltaRow < deltaCol ? deltaCol : deltaRow;

		if (objectDistance > 1)
			continue;

		if (objectDistance < closestObjectrDistance) {
			closestObjectrDistance = objectDistance;
			closestDistanceObjectIndex = townerIndex;
		}
	}
	if (closestDistanceObjectIndex != -1) {
		CurMon = closestDistanceObjectIndex;
	}
	else {
		CurMon = -1;
	}
}

//----- (th2) -------------------------------------------------------------
void XinputFindClosestDrop(Player& player)
{

	int closestDistanceObjectIndex = -1;
	uint closestObjectrDistance = -1;
	uint objectDistance = -1;
	int deltaRow;
	int deltaCol;

	for (int i = 0; i < ItemsCount; i++) {
		int itemIndex = ItemActive[i];
		Item& item = Items[itemIndex];

		deltaRow = abs(item.MapRow - player.NextRow);
		deltaCol = abs(item.MapCol - player.NextCol);

		objectDistance = deltaRow < deltaCol ? deltaCol : deltaRow;

		if (objectDistance > 1)
			continue;

		if (objectDistance < closestObjectrDistance) {
			closestObjectrDistance = objectDistance;
			closestDistanceObjectIndex = itemIndex;
		}
	}
	if (closestDistanceObjectIndex != -1) {
		Cur.ItemID = closestDistanceObjectIndex;
	}
	else {
		Cur.ItemID = -1;
	}
}

//----- (th2) -------------------------------------------------------------
void XinputCalculateTrueDirection(int& vd, int& hd)
{
	if (vd == X_UP && hd == X_STAY) { vd = -1; hd = -1; }
	else if (vd == X_DOWN && hd == X_STAY) { vd = 1;	hd = 1; }
	else if (vd == X_STAY && hd == X_LEFT) { vd = -1; hd = 1; }
	else if (vd == X_STAY && hd == X_RIGHT) { vd = 1;	hd = -1; }
	else if (vd == X_DOWN && hd == X_LEFT) { vd = 0;	hd = 1; }
	else if (vd == X_DOWN && hd == X_RIGHT) { vd = 1;	hd = 0; }
	else if (vd == X_UP && hd == X_LEFT) { vd = -1; hd = 0; }
	else if (vd == X_UP && hd == X_RIGHT) { vd = 0;	hd = -1; }
}

//----- (th2) -------------------------------------------------------------
void XinputInitAutoTargeting()
{
	Cur.ObjectIndex = -1;
	Cur.Col = -1;
	Cur.Row = -1;
	Cur.ItemID = -1;
	CurMon = -1;
}

//----- (th2) -------------------------------------------------------------
void XinputHandleAutoTargeting(Player& player)
{
	if (CurMon != -1 && (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) || curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_B))) {
		Monster& monster = Monsters[CurMon];
		if (monster.goal != MG_0_DEATH) {
			return;
		}
	}
	XinputInitAutoTargeting();
	if( Dungeon == DUN_0_TOWN && !Dungeon->isQuest ){
		XinputFindClosestTowner(player);
	}
	XinputFindClosestObject(player, true);
	XinputFindClosestMonster(player, true);
	XinputFindClosestDrop(player);
}


//----- (th2) -------------------------------------------------------------
void XinputSetNotInUseTime()
{
	XINPUT_GAMEPAD& gp = curXboxController.GetState().Gamepad;

	if (gp.wButtons > 0
		|| abs(gp.sThumbRY) > X_DEADZONE_R
		|| gp.bRightTrigger > 100
		|| abs(gp.sThumbLX) > X_DEADZONE_L
		|| gp.bLeftTrigger > 100
		|| abs(gp.sThumbLY) > X_DEADZONE_L
		|| abs(gp.sThumbRX) > X_DEADZONE_R
		) {
		XinputNotInUseTime = 0;
	}
	else {
		XinputNotInUseTime++;
		LimitToMax(XinputNotInUseTime, 5 * ENGINE_FPS);
	}
}

//----- (th2) -------------------------------------------------------------
bool XinputNotInUse(bool isTooltip) {
	if (isTooltip ? XinputNotInUseTime == 5 * ENGINE_FPS : XinputNotInUseTime > (ENGINE_FPS + 1))
		return true;
	else
		return false;
}

//----- (th2) -------------------------------------------------------------
void XinputHandleControllerForPlayerAction(int playerIndex)
{
	if (playerIndex != CurrentPlayerIndex) {
		return;
	}

	if( IsController() ){
		XWelcomeMessageTime--;
		XinputSetNotInUseTime();
		XinputHandleMouseMove();
		Player& player = Players[playerIndex];

		if (!XinputAutoTargettingOff) {
			XinputHandleAutoTargeting(player);
		}

		int vd; //VerticalDirection
		int hd; //HorizontalDirection
		int x; //Temp x
		int y; //Temp y

		vd = X_STAY;
		hd = X_STAY;
		x = curXboxController.GetState().Gamepad.sThumbRX; //Get Right Joystick horizontal value -32767 32767
		y = curXboxController.GetState().Gamepad.sThumbRY; //Get Right Joystick vertical value -32767 32767

		//KEY STROKES are handled in MainGamePlayLoop();

		if (x < X_DEADZONE_R && x > -X_DEADZONE_R) x = 0;
		if (y < X_DEADZONE_R && y > -X_DEADZONE_R) y = 0;

		if ((abs(x) > 0 || abs(y) > 0) || Cur.GraphicsID != CM_1_NORMAL_HAND) {
			XinputAutoTargettingOff = true;
		}
		if (x == 0 && y == 0 && XinputAutoTargettingOff && Cur.GraphicsID == CM_1_NORMAL_HAND && !XinputDontHideCursor()) {
			XinputAutoTargettingOffTimer++;
		}
		else {
			XinputAutoTargettingOffTimer = 0;
		}
		if (XinputNotInUse()) {
			XinputAutoTargettingOffTimer = 0;
		}
		if (XinputAutoTargettingOffTimer > ENGINE_FPS) {
			XinputAutoTargettingOff = false;
			XinputAutoTargettingOffTimer = 0;
			if (CursorInMainPanel() && !InfoPanelUsed) {
				CursorX = ScreenWidth / 2;
				CursorY = ScreenHeight / 2;
			}
		}

		vd = X_STAY;
		hd = X_STAY;
		x = curXboxController.GetState().Gamepad.sThumbLX; //Get Left Joystick horizontal value -32767 32767
		y = curXboxController.GetState().Gamepad.sThumbLY; //Get Left Joystick vertical value -32767 32767

		if (x > X_DEADZONE_L) hd = X_RIGHT; //Set Joystick sensivity
		if (x < -X_DEADZONE_L) hd = X_LEFT;
		if (y > X_DEADZONE_L) vd = X_UP;
		if (y < -X_DEADZONE_L) vd = X_DOWN;

		if (vd != X_STAY || hd != X_STAY) { //Calculate true direction
			if (CurrentMenuWindowPtr || CurrentDialogIndex || IsVisualTradePanelVisible || IsStashPanelVisible || IsCraftPanelVisible || IsEnchantPanelVisible || IsQuenchPanelVisible || IsTransmutePanelVisible) // TODO: change to flag mask
				KeyPressHandler(VK_27_ESC_KEY);
			XinputCalculateTrueDirection(vd, hd);
			NetSendCmdLoc(PRIORITY_LOW, CMD_1_WALKXY, player.NextRow + (vd * 1), player.NextCol + (hd * 1));
			player.destAction = PCMD_M1_NOTHING;
		}
		else if (!XinputAutoTargettingOff) {
			NetSendCmdLoc(PRIORITY_LOW, CMD_1_WALKXY, player.NextRow, player.NextCol);
		}

		bool xinputIsShift = curXboxController.GetState().Gamepad.bRightTrigger > 100;
		int xRow, xCol;
		Monster& monster = Monsters[CurMon];
		if ((curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_B) || (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) && player.ClassID == PC_2_MAGE && !HasTrait(playerIndex, TraitId::Mamluk))) && (CurMon != -1) && (Dungeon != DUN_0_TOWN || Dungeon->isQuest)) Mouse_RButtonDown(0);
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A) && ((CurMon != -1) || (xinputIsShift && !XinputDontHideCursor())) && (Dungeon != DUN_0_TOWN || Dungeon->isQuest) && (player.ClassID != PC_2_MAGE || HasTrait(playerIndex, TraitId::Mamluk))) {
			if (!(player.IsBowInHand || player.IsFlaskInHand)) { //
				if (xinputIsShift) { //
					if (CurMon == -1 /*|| !MonsterTalking(CurMon)*/) {
						if (XinputAutoTargettingOff) {
							xRow = Cur.Row;
							xCol = Cur.Col;
						}
						else {
							XinputSpellCastingPositioning(player, PS_0_NONE, xRow, xCol);
						}
						NetSendCmdLoc(PRIORITY_LOW, CMD_55_SATTACKXY, xRow, xCol);
					}
					else {
						NetSendCmdLoc(PRIORITY_LOW, CMD_55_SATTACKXY, monster.Row, monster.Col);
					}
				}
				else if (CurMon == -1) { // 
						if (Cur.playerIndex != -1 && !PlayerFriendly()) {
							NetSendCmdParam1(0, CMD_19_ATTACKPID, Cur.playerIndex);
						}
					}
				else if (CurMon >= SummonMonsters_Count) {
					NetSendCmdParam1(0, CMD_18_ATTACKID, CurMon);
				}
			}
			else { // 
				if (xinputIsShift) {
					if (CurMon == -1 /*|| !MonsterTalking(CurMon)*/) {
						if (XinputAutoTargettingOff) {
							xRow = Cur.Row;
							xCol = Cur.Col;
						}
						else {
							XinputSpellCastingPositioning(player, PS_0_NONE, xRow, xCol);
						}
						NetSendCmdLoc(PRIORITY_LOW, CMD_13_RATTACKXY, xRow, xCol);
					}
					else {
						NetSendCmdLoc(PRIORITY_LOW, CMD_13_RATTACKXY, monster.Row, monster.Col);
					}
				}
				else { //
					if (CurMon == -1) { //
						if (Cur.playerIndex != -1 && !PlayerFriendly()) {
							NetSendCmdParam1(0, CMD_21_RATTACKPID, Cur.playerIndex);
						}
					}
					else if (CurMon >= SummonMonsters_Count) {
						NetSendCmdParam1(0, MonsterTalking(CurMon) ? CMD_18_ATTACKID : CMD_20_RATTACKID, CurMon);
					}
				}
			}
		}
		
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP)) if ((IsPerksPanelVisible) && !XinputArrowUPTimer) { KeyPressHandler(VK_38_UP_ARROW_KEY); XinputArrowUPTimer++; }
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) if ((IsPerksPanelVisible) && !XinputArrowDOWNTimer) { KeyPressHandler(VK_40_DOWN_ARROW_KEY); XinputArrowDOWNTimer++; }

		if (!curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP)) if (IsPerksPanelVisible) { XinputArrowUPTimer = 0; }
		if (!curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) if (IsPerksPanelVisible) { XinputArrowDOWNTimer = 0; }

		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP)) if ((IsPerksPanelVisible) && XinputArrowUPTimer) { XinputArrowUPTimer++; }
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) if ((IsPerksPanelVisible) && XinputArrowDOWNTimer) { XinputArrowDOWNTimer++; }

		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_UP)) if ((IsPerksPanelVisible) && XinputArrowUPTimer >= ENGINE_FPS/2) { KeyPressHandler(VK_38_UP_ARROW_KEY); LimitToMax(XinputArrowUPTimer, ENGINE_FPS/2); }
		if (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_DPAD_DOWN)) if ((IsPerksPanelVisible) && XinputArrowDOWNTimer >= ENGINE_FPS/2) { KeyPressHandler(VK_40_DOWN_ARROW_KEY); LimitToMax(XinputArrowDOWNTimer, ENGINE_FPS/2); }

	}
}

//----- (th2) -------------------------------------------------------------
void DrawXinputGuide()
{
	if (XcelLoaded == false) {
		XcelLoaded = true;
		XGuideCel = (char*)LoadFile("Data\\XinputGuide.CEL");
	}
	int offsetX = Screen_LeftBorder + PerksPanelPos.Left;
	int offsetY = Screen_TopBorder + PerksPanelPos.Down - 640;
	CurrentMenuWindowPtr = false;
	Surface_DrawCEL(offsetX, offsetY + 640, XGuideCel, 1, PerksPanelPos.Width);
}

//----- (th2) -------------------------------------------------------------
bool XinputDontHideCursor()
{
	return IsPopupScreenOpen
		|| IsPerksPanelVisible       && CursorIntoDisplayObject(PerksPanelPos)
		|| IsInfoWindowVisible       && CursorIntoDisplayObject(InfoWindowRect)
		|| IsStashPanelVisible       && CursorIntoDisplayObject(VisualStashPanel)
		|| IsVisualTradePanelVisible && CursorIntoDisplayObject(VisualTradePanel)
		|| IsCraftPanelVisible       && CursorIntoDisplayObject(CraftPanel)
		|| IsEnchantPanelVisible     && CursorIntoDisplayObject(EnchantPanel)
		|| IsQuenchPanelVisible      && CursorIntoDisplayObject(QuenchPanel)
		|| IsTransmutePanelVisible   && CursorIntoDisplayObject(TransmutePanel)
		|| IsINVPanelVisible         && CursorIntoDisplayObject(InventoryPanelRect)
		|| IsQUESTPanelVisible       && CursorIntoDisplayObject(QuestPanelRect)
		|| IsSpellBookVisible        && CursorIntoDisplayObject(SpellBookRect)
		|| IsCHARPanelVisible        && CursorIntoDisplayObject(CharPanelRect)
		|| CursorInMainPanel()       && (Cur.InventoryCellID != -1 || InfoPanelUsed)
		|| CursorIntoLevelUpButton()
		|| CursorIntoPerkUpButton()
		|| Cur.GraphicsID != CM_1_NORMAL_HAND;
}