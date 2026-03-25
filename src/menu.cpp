#include "stdafx.h"

Menu* CurrentMenuItem;
Menu* CurrentMenuWindowPtr;
void(__cdecl * MainMenuHandlerPtr)();
char* Hf_logo3CELPtr;
char* FontSpriteBig;
char* PentSpinCELPtr;
char* OptionCELPtr;
char* OptbarCELPtr;
char PentagramFrameCount;
char LogoFrameCount;
int CurMenuSize;
bool IsMenuVisible;
i64 OldTickCount2;
i64 OldTickCount3;
int tempHeight, tempWidth, tempFPS, tempReceivedDamageThreshold, tempMonsterReceivedDamageThreshold, tempFloatingHPRegenThreshold, tempFloatingManaRegenThreshold;
bool tempFullscreen, tempShowFPS, tempMapblend, tempShowNumbersOnHealth,
tempShowNumbersOnMana,tempAlwaysHighlightGolem,tempShowMonsterHealthBar,tempShowVersionOnScreen,
tempshowCombatText,tempshowCombatExp,tempShowPercentExpGain, tempShowReceivedDamage,
tempmergeCombatTexts, tempshowInflictedDamage, tempShowBreakWarning, tempRedInactiveSpellIcons,
tempRunindungeon, tempAutopickup, tempEasyclick, tempAlthold, tempSwapSpellsOnScroll, tempPanel, tempPotion;
bool firstTimeFS = true;

//----- (0041B279) -------------------------------------------------------- menu
void SelectMainMenu()
{
	void(__cdecl * mainMenuHandler)();
	Menu* mainMenu;
	if( MaxCountOfPlayersInGame == 1 && !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_1_AUTO_SAVE)) && !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM))){
		mainMenuHandler = SPMainMenuHandler;
		mainMenu = SPMainMenu;
	}else{
		mainMenuHandler = MPMainMenuHandler;
		mainMenu = MPMainMenu;
	}
	Tooltip_Clear();
	Menu_SetupMenuWindow(mainMenu, mainMenuHandler);
	EscHandle();
}

//----- (0041B2A2) -------------------------------------------------------- menu
void SPMainMenuHandler()
{
	Menu_SetItemAvailability(&SPMainMenu[3], gbValidSaveFile);
	bool isSaveGameEnabled = 0;
		
	if( Players[CurrentPlayerIndex].CurAction != PCA_8_DEATH
	 && (SaveAlwaysEnabled || is(GameMode, GM_EASY, GM_CLASSIC) || !Dungeon->isQuest) && IsSaveAndLeavingEnabled() && !IsPlayerDead  && EnforceNoSave == false ){
		isSaveGameEnabled = 1;
	}

	Menu_SetItemAvailability(&SPMainMenu[0], isSaveGameEnabled );
	Menu_SetItemAvailability(&SPMainMenu[1], 1);
	Menu_SetItemAvailability(&SPMainMenu[2], 1);
	Menu_SetItemAvailability(&SPMainMenu[4], 1);
	SPMainMenu[0].NamePtr = "Save Game";
	SPMainMenu[1].NamePtr = "Setup";
	SPMainMenu[2].NamePtr = "New Game";
	SPMainMenu[3].NamePtr = "Load Game";
	SPMainMenu[4].NamePtr = "Quit";
    SPMainMenu[2].FuncPtr = NewGameMenuHandler;

	if (GameMode == GM_IRONMAN) {
		Menu_SetItemAvailability(&SPMainMenu[0], 0);
		Menu_SetItemAvailability(&SPMainMenu[2], 1);
		Menu_SetItemAvailability(&SPMainMenu[3], 0);
		Menu_SetItemAvailability(&SPMainMenu[4], 0);
		SPMainMenu[2].NamePtr = "Murder Hero";
		SPMainMenu[2].FuncPtr = MurderHeroMenuHandler;		
		int ironman_saverate = 4;
		GetIronmanSaverate(ironman_saverate);
		if (
			(!Dungeon->isQuest || (Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON))
			&& Dungeon != DUN_0_TOWN
			&& (DUNGEON)Dungeon % ironman_saverate == 0 // TODO: change to save flag in Dungeons
			&& GetNumberOfObjectsInDungeon() <= MaxIronmanObjects
			&& GetNumberOfMonstersInDungeon() == 0
			&& Players[CurrentPlayerIndex].CurAction != PCA_8_DEATH
			&& !IsPlayerDead
			) {
			SPMainMenu[4].NamePtr = "Save & Quit";
			Menu_SetItemAvailability(&SPMainMenu[4], 1); 
		}
	}
	else if (GameMode == GM_SURVIVAL) {
		Menu_SetItemAvailability(&SPMainMenu[0], 0);
		Menu_SetItemAvailability(&SPMainMenu[2], 1);
		Menu_SetItemAvailability(&SPMainMenu[3], gbValidSaveFile);
		isSaveGameEnabled = (Players[CurrentPlayerIndex].CurAction != PCA_8_DEATH
			&& !IsPlayerDead
			&& GetNumberOfObjectsInDungeon() <= MaxIronmanObjects
			&& GetNumberOfMonstersInDungeon() == 0
			&& EnforceNoSave == false)
			|| SaveAlwaysEnabled;
		Menu_SetItemAvailability(&SPMainMenu[0], isSaveGameEnabled);
	}
	else if( GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN){
		EnforceNoSave = ! UberDiabloKilled; 
		Menu_SetItemAvailability(&SPMainMenu[0], 0);
		Menu_SetItemAvailability(&SPMainMenu[3], 0);
		Menu_SetItemAvailability(&SPMainMenu[4], 0);
		SPMainMenu[2].NamePtr = "Abandon Game";
		SPMainMenu[2].FuncPtr = MurderHeroMenuHandler;
		Menu_SetItemAvailability(&SPMainMenu[2], 1);
	}else if( GameMode == GM_HARDCORE ){
		if( Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead ){
			Menu_SetItemAvailability(&SPMainMenu[0], 0);
			Menu_SetItemAvailability(&SPMainMenu[2], 1);
			Menu_SetItemAvailability(&SPMainMenu[3], 0);
			Menu_SetItemAvailability(&SPMainMenu[4], 0);
			SPMainMenu[2].NamePtr = "Murder Hero";
			SPMainMenu[2].FuncPtr = MurderHeroMenuHandler;
		}
		else if (IsSomeMonstersActivated()) {
			Menu_SetItemAvailability(&SPMainMenu[0], 0);
			Menu_SetItemAvailability(&SPMainMenu[2], 0);
			Menu_SetItemAvailability(&SPMainMenu[3], 0);
			Menu_SetItemAvailability(&SPMainMenu[4], 0);
		}
		else {
			Menu_SetItemAvailability(&SPMainMenu[0], isSaveGameEnabled);
			Menu_SetItemAvailability(&SPMainMenu[2], 1);
			Menu_SetItemAvailability(&SPMainMenu[3], gbValidSaveFile);
			Menu_SetItemAvailability(&SPMainMenu[4], 1);
		}
	}
	if( is( GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_IRONMAN ) && UberDiabloKilled ){
		SPMainMenu[ 4 ].NamePtr = "Save & Quit";
		Menu_SetItemAvailability( &SPMainMenu[ 4 ], 1 );
	}
}

void MurderHeroMenuHandlerMurderHeroMenuHandler()
{

}

//----- (007102E4) -------------------------------------------------------- menu
void __fastcall MpRestartAvailability( Menu* menuItem, int isActivated )
{
	if( Dungeon != DUN_0_TOWN || isActivated ){
		menuItem->Flags &= ~MENU_ENABLED;
	}else{
		menuItem->Flags |= MENU_ENABLED;
	}
}

//----- (0041B2E0) -------------------------------------------------------- menu
void MPMainMenuHandler()
{
	MpRestartAvailability(&MPMainMenu[1], IsPlayerDead);
	Menu_SetItemAvailability(&MPMainMenu[2], IsPlayerDead);
	MpRestartAvailability(&MPMainMenu[3], IsPlayerDead);
	MPMainMenu[0].NamePtr = "Setup";
	MPMainMenu[1].NamePtr = "New Game";
	MPMainMenu[2].NamePtr = "Restart In Town";
	MPMainMenu[3].NamePtr = "Quit";
    MPMainMenu[1].FuncPtr = NewGameMenuHandler;

	if (is(GameMode, GM_NORMAL, GM_HARD)) {
		if ( (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) && Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)) {
			Menu_SetItemAvailability(&MPMainMenu[2], 0);
			Menu_SetItemAvailability(&MPMainMenu[1], 1);
			Menu_SetItemAvailability(&MPMainMenu[3], 1);
		}
	}
	else if (GameMode == GM_IRONMAN) {
	//	if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
            Menu_SetItemAvailability(&MPMainMenu[1], 1);
            Menu_SetItemAvailability(&MPMainMenu[2], 0);
			Menu_SetItemAvailability(&MPMainMenu[3], 0);
			
			MPMainMenu[1].NamePtr = "Murder Hero";
			MPMainMenu[1].FuncPtr = MurderHeroMenuHandler;
			if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			}
			else {
				int ironman_saverate = 4;
				GetIronmanSaverate(ironman_saverate);
				if( Dungeon != DUN_0_TOWN && (DUNGEON)Dungeon % ironman_saverate == 0 // TODO: change to save flag in Dungeons
				 && GetNumberOfObjectsInDungeon() <= MaxIronmanObjects && GetNumberOfMonstersInDungeon() == 0) {
					MPMainMenu[3].NamePtr = "Save & Quit";
					Menu_SetItemAvailability(&MPMainMenu[1], 0);
					Menu_SetItemAvailability(&MPMainMenu[3], 1);
				}
			}
	}
	else if (GameMode == GM_SURVIVAL) {
		//	if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
		Menu_SetItemAvailability(&MPMainMenu[1], 0); // no new game
		Menu_SetItemAvailability(&MPMainMenu[2], 0); // no restart
		Menu_SetItemAvailability(&MPMainMenu[3], 0); // no quit

		//MPMainMenu[1].NamePtr = "Murder Hero";
		//MPMainMenu[1].FuncPtr = MurderHeroMenuHandler;
		if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
			Menu_SetItemAvailability(&MPMainMenu[1], 1);
		}
		else {
			if( /*Dungeon != DUN_0_TOWN &&*/ (DUNGEON)Dungeon % 1 == 0 && GetNumberOfObjectsInDungeon() <= MaxIronmanObjects && GetNumberOfMonstersInDungeon() == 0) {
				MPMainMenu[3].NamePtr = "Quit";
				Menu_SetItemAvailability(&MPMainMenu[1], 1);
				Menu_SetItemAvailability(&MPMainMenu[3], 1);
			}
		}
	}
	else if (GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN) {
		EnforceNoSave = ! UberDiabloKilled;
		Menu_SetItemAvailability(&MPMainMenu[0], 1);

        MPMainMenu[1].NamePtr = "Abandon Game";
        MPMainMenu[1].FuncPtr = MurderHeroMenuHandler;
        Menu_SetItemAvailability(&MPMainMenu[1], 1);

		Menu_SetItemAvailability(&MPMainMenu[2], 0);
		Menu_SetItemAvailability(&MPMainMenu[3], 0);
	}

	else if (GameMode == GM_HARDCORE) {
		if (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead) {
            Menu_SetItemAvailability(&MPMainMenu[1], 1);
            MPMainMenu[1].NamePtr = "Murder Hero";
            MPMainMenu[1].FuncPtr = MurderHeroMenuHandler;

			Menu_SetItemAvailability(&MPMainMenu[2], 0);
			Menu_SetItemAvailability(&MPMainMenu[3], 0);
		}
	}
	if( is( GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_IRONMAN ) && UberDiabloKilled ){
		MPMainMenu[ 3 ].NamePtr = "Save & Quit";
		Menu_SetItemAvailability( &MPMainMenu[ 1 ], 0 );
		Menu_SetItemAvailability( &MPMainMenu[ 3 ], 1 );
	}
}

//----- (0041B2F0) -------------------------------------------------------- menu
void ResetMainMenu()
{
	Menu_SetupMenuWindow(0, 0);
}

//----- (0041B2F9) -------------------------------------------------------- menu
void ToggleMenu()
{
	Speech_IsPanelVisible = 0; 
	if( IsGameMenuActive() ){
		ResetMainMenu();
	}else{
		SelectMainMenu();
	}
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (0041B311) -------------------------------------------------------- menu
void __fastcall NewGameMenuHandler(int a1, int a2)
{
	for( int i = 0; i < PlayersMax_4; ++i ){
		Players[i].CurAction = PCA_11_QUIT;
		Players[i].notHittableInTransition = 1;
		PlayersSpellcastQueue[i] = {};
	}
	PlayerWall = {};
	IsPlayerDead = 0;
	DrawNotGameplayScreen(1);
	gbRunGame = 0;
	ResetMainMenu();
}

void __fastcall MurderHeroMenuHandler(int a1, int a2)
{
    Menu_SetupMenuWindow(MurderConfirmationMenu, 0);
}

void __fastcall NullMenuHandler(int a1, int a2)
{
}

//----- (th2) -------------------------------------------------------------
void MurderHeroInGame()
{
	InstantMurderHero = (is(GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_IRONMAN)
		|| (GameMode == GM_HARDCORE && (Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead)));
	strcpy(MurderHeroName, Players[CurrentPlayerIndex].playerName);
	if( UberDiabloKilled ) InstantMurderHero = false;
	for( int i = 0; i < PlayersMax_4; ++i ){
		Players[i].CurAction = PCA_11_QUIT;
		Players[i].notHittableInTransition = 1;
	}
	IsPlayerDead = 0;
	DrawNotGameplayScreen(1);
	gbRunGame = 0;
	ResetMainMenu();
}

//----- (0041B365) -------------------------------------------------------- menu
void __fastcall ExitGameMenuHandler(int a1, int a2)
{
	if( GameMode == GM_IRONMAN ){
		if( Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead ){
		}else{
			// TODO: continue here, change to save flag in Dungeons
			int ironman_saverate = 4;
			GetIronmanSaverate(ironman_saverate);
			if( (DUNGEON)Dungeon % ironman_saverate == 0 && GetNumberOfObjectsInDungeon() <= MaxIronmanObjects && GetNumberOfMonstersInDungeon() == 0 ){
				if( MaxCountOfPlayersInGame == 1 ){
					if( Cur.GraphicsID == CM_1_NORMAL_HAND ) SaveGameMenuHandler(0, 0); else return;
				}
			}
		}
	}
	if( is( GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_IRONMAN ) && UberDiabloKilled ){
		if( Cur.GraphicsID == CM_1_NORMAL_HAND ) SaveGameMenuHandler(0, 0); else return;
	}
	NewGameMenuHandler(a1, a2);
	gbRunGameResult = UberDiabloKilled || is(GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_IRONMAN); // 0;
}

//----- (0041B372) -------------------------------------------------------- menu
void __fastcall LoadGameMenuHandler(int a1, int a2)
{
	PlayersSpellcastQueue[CurrentPlayerIndex] = {};
	PlayerWall[CurrentPlayerIndex] = {};
	ProcHandler prevHandler = SetWindowProc(DisableInputWindowProc);
	ResetMainMenu();
	SetCursorGraphics(CM_0_INVISIBLE);
	AddOnScreenMessage(OM_10_Loading);
	MainDrawFunc();
	LoadGame(0); // load from ingame menu
	Scroll.dir = 0;
	ClearOnScreenMessages();
	PaletteFadeOut(8);
	IsPlayerDead = 0;
	MainDrawFunc();
	PaletteFadeIn(8);
	SetCursorGraphics(CM_1_NORMAL_HAND);
	HandleSystemMessage();
	SetWindowProc(prevHandler);
	MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE; // prevent autoclick after load
}

//----- (0041B3F0) -------------------------------------------------------- menu
void __fastcall SaveGameMenuHandler(int a1, int a2)
{
	if( Cur.GraphicsID == CM_1_NORMAL_HAND ){
		if( Players[CurrentPlayerIndex].CurAction == PCA_8_DEATH || IsPlayerDead ){
			ResetMainMenu();
		}else{
			ProcHandler prevHandler = SetWindowProc(DisableInputWindowProc);
			SetCursorGraphics(CM_0_INVISIBLE);
			ResetMainMenu();
			AddOnScreenMessage(OM_11_Saving);
			MainDrawFunc();
			SaveGame();
			ClearOnScreenMessages();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			HandleSystemMessage();
			SetWindowProc(prevHandler);
		}
		MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE; // prevent autoclick after save
	}
}

//----- (0041B48B) -------------------------------------------------------- menu
void __fastcall RestartMenuHandler(int a1, int a2)
{
	if( NetPlayerCount == 1 || true ){ // в th1 в мульти тоже в городе восстанавливаемся
        NetSendCmd( 1, CMD_91_RETOWN );
	}else{
        NetSendCmdParam1( 1, CMD_26_RESURRECT, CurrentPlayerIndex );
	}
}

//----- (0041B495) -------------------------------------------------------- menu
void __fastcall SetupMenuHandler(int a1, int a2)
{
	SetMusicMenu();
	SetAmbientMenu();
	SetSoundMenu();
	SetWalkMenu();
	SetGammaMenu();
	SetContrastMenu();

	if (!tempHeight) tempHeight = ScreenHeight;
	if (!tempWidth) tempWidth = ScreenWidth;
	if (!tempFullscreen) tempFullscreen = IsFullScreen;
	if (!tempFPS) tempFPS = MaxFPS;
	if (!tempShowFPS) tempShowFPS = ShowFps;
	if (!tempRunindungeon) tempRunindungeon = RunInDungeon;
	if (!tempAutopickup) tempAutopickup = IsAutoPickup;
	if (!tempEasyclick) tempEasyclick = EasyClick;
	if (!tempAlthold) tempAlthold = NeedAltHold;
	if (!tempSwapSpellsOnScroll) tempSwapSpellsOnScroll = SwapSpellsOnScroll;
	if (!tempPanel) tempPanel = UserPanelB;
	if (!tempPotion) tempPotion = UserPotionB;
	if (!tempMapblend) tempMapblend = MapBlend;
	if (!tempShowNumbersOnHealth) tempShowNumbersOnHealth = ShowNumbersOnHealth;
	if (!tempShowNumbersOnMana) tempShowNumbersOnMana = ShowNumbersOnMana;
	if (!tempAlwaysHighlightGolem) tempAlwaysHighlightGolem = AlwaysHighlightSummons;
	if (!tempShowMonsterHealthBar) tempShowMonsterHealthBar = ShowMonsterHealthBar;
	if (!tempShowVersionOnScreen) tempShowVersionOnScreen = ShowVersionOnScreen;
	if (!tempshowCombatText) tempshowCombatText = showCombatText;
	if (!tempshowCombatExp) tempshowCombatExp = showCombatExp;
	if (!tempShowPercentExpGain) tempShowPercentExpGain = ShowPercentExpGain;
	if (!tempShowReceivedDamage) tempShowReceivedDamage = ShowReceivedDamage;
	if (!tempReceivedDamageThreshold) tempReceivedDamageThreshold = ReceivedDamageThreshold;
	if (!tempmergeCombatTexts) tempmergeCombatTexts = mergeCombatTexts;
	if (!tempshowInflictedDamage) tempshowInflictedDamage = showInflictedDamage;
	if (!tempMonsterReceivedDamageThreshold) tempMonsterReceivedDamageThreshold = MonsterReceivedDamageThreshold;
	if (!tempFloatingHPRegenThreshold) tempFloatingHPRegenThreshold = FloatingHPRegenThreshold;
	if (!tempFloatingManaRegenThreshold) tempFloatingManaRegenThreshold = FloatingManaRegenThreshold;
	if (!tempShowBreakWarning) tempShowBreakWarning = ShowBreakWarning;
	if (!tempRedInactiveSpellIcons) tempRedInactiveSpellIcons = RedInactiveSpellIcons;
	
	Menu_SetupMenuWindow(OptionsMenu, 0);
}

void __fastcall SaveToConfig()
{
	return;
	if (FILE* ini = fopen("config.ini", "w")) {
		fprintf(ini, "\n");
		fprintf(ini, "//VIDEO OPTIONS\n");
		fprintf(ini, "\n");
		fprintf(ini, "set screenwidth %d\n", tempWidth);
		fprintf(ini, "set screenheight %d\n", tempHeight);
		fprintf(ini, "set fullscreen %s\n", tempFullscreen ? "on" : "off");
		fprintf(ini, "set fps %d\n", tempFPS);
		fprintf(ini, "set showfps %s\n", tempShowFPS ? "on" : "off");
		fprintf(ini, "\n");
		fprintf(ini, "//GAMEPLAY OPTIONS\n");
		fprintf(ini, "\n");
		fprintf(ini, "set runindungeon  %s\n", tempRunindungeon ? "on" : "off");
		fprintf(ini, "set autopickup  %s\n", tempAutopickup ? "on" : "off");
		fprintf(ini, "set easyclick  %s\n", tempEasyclick ? "on" : "off");
		fprintf(ini, "set althold  %s\n", tempAlthold ? "on" : "off");
		fprintf(ini, "set swapSpellsOnScroll  %s\n", tempSwapSpellsOnScroll ? "on" : "off");
		fprintf(ini, "\n");
		fprintf(ini, "//VISUAL OPTIONS\n");
		fprintf(ini, "\n");
		fprintf(ini, "set panel %s\n", tempPanel ? "on" : "off");
		fprintf(ini, "set potion %s\n", tempPotion ? "on" : "off");
		fprintf(ini, "set map open off // ??\n");
		fprintf(ini, "set mapblend %s\n", tempMapblend ? "on" : "off");
		fprintf(ini, "set showNumbersOnHealth %s\n", tempShowNumbersOnHealth ? "on" : "off");
		fprintf(ini, "set showNumbersOnMana %s\n", tempShowNumbersOnMana ? "on" : "off");
		fprintf(ini, "set alwaysHighlightGolem %s\n", tempAlwaysHighlightGolem ? "on" : "off");
		fprintf(ini, "set showMonsterHealthBar %s\n", tempShowMonsterHealthBar ? "on" : "off");
		fprintf(ini, "set showVersionOnScreen %s\n", tempShowVersionOnScreen ? "on" : "off");
		fprintf(ini, "set RedInactiveSpellIcons %s\n", tempRedInactiveSpellIcons ? "on" : "off");
		fprintf(ini, "\n");
		fprintf(ini, "//FLOATING TEXTS\n");
		fprintf(ini, "\n");
		fprintf(ini, "set showCombatText %s\n", tempshowCombatText ? "on" : "off");
		fprintf(ini, "set showCombatExp %s\n", tempshowCombatExp ? "on" : "off");
		fprintf(ini, "set showPercentExpGain %s\n", tempShowPercentExpGain ? "on" : "off");
		fprintf(ini, "set showReceivedDamage %s\n", tempShowVersionOnScreen ? "on" : "off");
		fprintf(ini, "set receivedDamageThreshold %d\n", tempReceivedDamageThreshold);
		fprintf(ini, "set mergeCombatTexts %s\n", tempmergeCombatTexts ? "on" : "off");
		fprintf(ini, "set showInflictedDamage %s\n", tempshowInflictedDamage ? "on" : "off");
		fprintf(ini, "set MonsterReceivedDamageThreshold %d\n", tempMonsterReceivedDamageThreshold);
		fprintf(ini, "set floatingHPRegenThreshold %d\n", tempFloatingHPRegenThreshold);
		fprintf(ini, "set floatingManaRegenThreshold %d\n", tempFloatingManaRegenThreshold);
		fprintf(ini, "set showBreakWarning %s\n", tempShowBreakWarning ? "on" : "off");
		fprintf(ini, "\n");
		fprintf(ini, "// OTHER OPTIONS (don't change these ever! changes will cripple your character with no way to repair them)\n");
		fprintf(ini, "\n");
		fprintf(ini, "set devmode off\n");
		fprintf(ini, "set godmode off\n");
		fprintf(ini, "set drawmode surface\n");
		fprintf(ini, "set filter on\n");
		fprintf(ini, "set render d3d\n");
		fprintf(ini, "set freespell off\n");
		fprintf(ini, "set stash on\n");
		fprintf(ini, "set startvideo on\n");
		fprintf(ini, "set menu old\n");
		fprintf(ini, "set sound on\n");
		fprintf(ini, "set map close\n");
		fprintf(ini, "set dropundermonsters on\n");
		fprintf(ini, "set savefolder on\n");
		fprintf(ini, "set autobackup off\n");
		fprintf(ini, "set exception off\n");
		fprintf(ini, "set savealways on\n");
		fprintf(ini, "set arguments off\n");
		fprintf(ini, "set startpoint 0 //999999999 // MainWorldTick\n");
		fprintf(ini, "set secondwin off\n");
		fprintf(ini, "set exevalidating off\n");
		fprintf(ini, "set newgame off\n");
		fprintf(ini, "set difficulty 0\n");
		fprintf(ini, "set loadonstart off\n");
		fprintf(ini, "-set vars LightMap, OriginalLightMap\n");
		fprintf(ini, "-set vars Affixes\n");
		fprintf(ini, "-set vars LightIndexes\n");
		fprintf(ini, "-set vars Missiles\n");
		fprintf(ini, "-set vars Quests\n");
		fprintf(ini, "=set vars UniqueItems\n");
		fprintf(ini, "-set vars Speeches, SoundFiles, BaseQuests, BaseItems, UniqueItems, UniqueMonsters, BaseMonsters, BaseMissiles, BaseObjects, LevelUpTable, ItemFlipTable, ItemWidthTable, ItemHeightTable\n");
		fprintf(ini, "-set vars TileLightMapTop, LevelCELsAmount\n");
		fprintf(ini, "-set vars MapEndRow, MapEndCol, MapStartRow, MapStartCol\n");
		fprintf(ini, "-set vars FloorItemsIndexes, Objects\n");
		fprintf(ini, "-set vars Players\n");
		fprintf(ini, "-set vars Missiles\n");
		fprintf(ini, "-set vars NetLevelsData1\n");
		fprintf(ini, "-set vars Monsters1\n");
		fprintf(ini, "-set vars CurSaveData, -MpqSeekDistanceToMove\n");
		fprintf(ini, "-set funcs MainWorldTick\n");
		fprintf(ini, "-set seed 1177915638,943851795,136162032,1063681103,1558040154,1267784543,1178229932,1832282211,1172293246,333808619,1481894312,1482649655,172555618,725331639,1173155300,816887861,463976198,832092221,818759008,1768070369,447674218,987727089,125701148,1386285517,1801813362\n");
		fprintf(ini, "\n");
		fprintf(ini, "-set playertrn Monsters\\Bat\\orange.trn\n");
		fprintf(ini, "-set weapondelay 394, 1\n");
		fclose(ini);
	}
	RedInactiveSpellIcons = tempRedInactiveSpellIcons;
	IsFullScreen = tempFullscreen;
	MaxFPS = tempFPS;
	ShowFps = tempShowFPS;
	RunInDungeon = tempRunindungeon;
	IsAutoPickup = tempAutopickup;
	EasyClick = tempEasyclick;
	NeedAltHold = tempAlthold;
	SwapSpellsOnScroll = tempSwapSpellsOnScroll;
	UserPanelB = tempPanel;
	UserPotionB = tempPotion;
	MapBlend = tempMapblend;
	ShowNumbersOnHealth = tempShowNumbersOnHealth;
	ShowNumbersOnMana = tempShowNumbersOnMana;
	AlwaysHighlightSummons = tempAlwaysHighlightGolem;
	ShowMonsterHealthBar = tempShowMonsterHealthBar;
	ShowVersionOnScreen = tempShowVersionOnScreen;
	showCombatText = tempshowCombatText;
	showCombatExp = tempshowCombatExp;
	ShowPercentExpGain = tempShowPercentExpGain;
	ShowReceivedDamage = tempShowReceivedDamage;
	ReceivedDamageThreshold = tempReceivedDamageThreshold;
	mergeCombatTexts = tempmergeCombatTexts;
	showInflictedDamage = tempshowInflictedDamage;
	MonsterReceivedDamageThreshold = tempMonsterReceivedDamageThreshold;
	FloatingHPRegenThreshold = tempFloatingHPRegenThreshold;
	FloatingManaRegenThreshold = tempFloatingManaRegenThreshold;
	ShowBreakWarning = tempShowBreakWarning;
	RedInactiveSpellIcons = tempRedInactiveSpellIcons;
	MainDrawFunc();
	MayBeViewInit();
}



void __fastcall ConfigMenuHandler(int a1, int a2)
{
	Menu_SetupMenuWindow(ConfigPage1, 0);
}


void __fastcall CombatTextOptions1MenuHandler(int a1, int a2){
	switch (tempshowCombatText) {
	case true: CombatTextOptions1[0].NamePtr = "Show floating text: on"; break;
	case false: CombatTextOptions1[0].NamePtr = "Show floating text: off"; break;
	}
	switch (tempshowCombatExp) {
	case true: CombatTextOptions1[1].NamePtr = "show Floating Exp: on"; break;
	case false: CombatTextOptions1[1].NamePtr = "show Floating Exp: off"; break;
	}
	switch (tempShowPercentExpGain) {
	case true: CombatTextOptions1[2].NamePtr = "show Percent Exp Gain: on"; break;
	case false: CombatTextOptions1[2].NamePtr = "show Percent Exp Gain: off"; break;
	}
	switch (tempShowReceivedDamage) {
	case true: CombatTextOptions1[3].NamePtr = "show Received Damage: on"; break;
	case false: CombatTextOptions1[3].NamePtr = "show Received Damage: off"; break;
	}
	switch (tempReceivedDamageThreshold) {
	case 0: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 0"; break;
	case 1: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 1"; break;
	case 2: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 2"; break;
	case 3: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 3"; break;
	case 4: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 4"; break;
	case 5: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 5"; break;
	case 7: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 7"; break;
	case 10: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 10"; break;
	case 15: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 15"; break;
	case 20: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 20"; break;
	case 30: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 30"; break;
	case 50: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 50"; break;
	case 70: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 70"; break;
	case 100: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 100"; break;
	case 150: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 150"; break;
	case 200: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 200"; break;
	case 300: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 300"; break;
	case 500: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 500"; break;
	default: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 0"; tempReceivedDamageThreshold = 0; break;
	}
	switch (tempmergeCombatTexts) {
	case true: CombatTextOptions2[1].NamePtr = "merge Floating Texts: on"; break;
	case false: CombatTextOptions2[1].NamePtr = "merge Floating Texts: off"; break;
	}
	switch (tempshowInflictedDamage) {
	case true: CombatTextOptions2[2].NamePtr = "show Floating Damage: on"; break;
	case false: CombatTextOptions2[2].NamePtr = "show Floating Damage: off"; break;
	}
	switch (tempMonsterReceivedDamageThreshold) {
	case 0: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 0"; break;
	case 1: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 1"; break;
	case 2: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 2"; break;
	case 3: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 3"; break;
	case 4: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 4"; break;
	case 5: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 5"; break;
	case 7: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 7"; break;
	case 10: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 10"; break;
	case 15: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 15"; break;
	case 20: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 20"; break;
	case 30: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 30"; break;
	case 50: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 50"; break;
	case 70: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 70"; break;
	case 100: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 100"; break;
	case 150: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 150"; break;
	case 200: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 200"; break;
	case 300: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 300"; break;
	case 500: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 500"; break;
	default: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 0"; tempMonsterReceivedDamageThreshold = 0; break;
	}
	switch (tempFloatingHPRegenThreshold) {
	case 0: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 0"; break;
	case 1: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 1"; break;
	case 2: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 2"; break;
	case 3: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 3"; break;
	case 4: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 4"; break;
	case 5: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 5"; break;
	case 7: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 7"; break;
	case 10: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 10"; break;
	case 15: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 15"; break;
	case 20: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 20"; break;
	case 30: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 30"; break;
	case 50: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 50"; break;
	case 70: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 70"; break;
	case 100: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 100"; break;
	case 150: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 150"; break;
	case 200: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 200"; break;
	case 300: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 300"; break;
	case 500: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 500"; break;
	default: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 0"; tempFloatingHPRegenThreshold = 0; break;
	}
	switch (tempFloatingManaRegenThreshold) {
	case 0: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 0"; break;
	case 1: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 1"; break;
	case 2: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 2"; break;
	case 3: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 3"; break;
	case 4: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 4"; break;
	case 5: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 5"; break;
	case 7: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 7"; break;
	case 10: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 10"; break;
	case 15: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 15"; break;
	case 20: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 20"; break;
	case 30: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 30"; break;
	case 50: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 50"; break;
	case 70: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 70"; break;
	case 100: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 100"; break;
	case 150: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 150"; break;
	case 200: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 200"; break;
	case 300: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 300"; break;
	case 500: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 500"; break;
	default: CombatTextOptions3[1].NamePtr = "Floating ManaRegen Threshold: 0"; tempFloatingManaRegenThreshold = 0; break;
	}
	switch (tempShowBreakWarning) {
	case true: CombatTextOptions3[2].NamePtr = "show Break Warning: on"; break;
	case false: CombatTextOptions3[2].NamePtr = "show Break Warning: off"; break;
	}
	Menu_SetupMenuWindow(CombatTextOptions1, 0);
}
void __fastcall CombatTextOptions2MenuHandler(int a1, int a2){
	Menu_SetupMenuWindow(CombatTextOptions2, 0);
}
void __fastcall CombatTextOptions3MenuHandler(int a1, int a2){
	Menu_SetupMenuWindow(CombatTextOptions3, 0);
}
void __fastcall showCombatTextHandler(int a1, int a2){
	switch (tempshowCombatText) {
	case true: CombatTextOptions1[0].NamePtr = "Show floating text: off"; tempshowCombatText = false; break;
	case false: CombatTextOptions1[0].NamePtr = "Show floating text: on"; tempshowCombatText = true; break;
	}
	SaveToConfig();
}
void __fastcall showCombatExpHandler(int a1, int a2){
	switch (tempshowCombatExp) {
	case true: CombatTextOptions1[1].NamePtr = "show Floating Exp: off"; tempshowCombatExp = false; break;
	case false: CombatTextOptions1[1].NamePtr = "show Floating Exp: on"; tempshowCombatExp = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowPercentExpGainHandler(int a1, int a2){
	switch (tempShowPercentExpGain) {
	case true: CombatTextOptions1[2].NamePtr = "show Percent Exp Gain: off"; tempShowPercentExpGain = false; break;
	case false: CombatTextOptions1[2].NamePtr = "show Percent Exp Gain: on"; tempShowPercentExpGain = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowReceivedDamageHandler(int a1, int a2){
	switch (tempShowReceivedDamage) {
	case true: CombatTextOptions1[3].NamePtr = "show Received Damage: off"; tempShowReceivedDamage = false; break;
	case false: CombatTextOptions1[3].NamePtr = "show Received Damage: on"; tempShowReceivedDamage = true; break;
	}
	SaveToConfig();
}
void __fastcall ReceivedDamageThresholdHandler(int a1, int a2){
	switch (tempReceivedDamageThreshold) {
	case 0: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 1"; tempReceivedDamageThreshold = 1; break;
	case 1: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 2"; tempReceivedDamageThreshold = 2; break;
	case 2: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 3"; tempReceivedDamageThreshold = 3; break;
	case 3: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 4"; tempReceivedDamageThreshold = 4; break;
	case 4: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 5"; tempReceivedDamageThreshold = 5; break;
	case 5: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 7"; tempReceivedDamageThreshold = 7; break;
	case 7: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 10"; tempReceivedDamageThreshold = 10; break;
	case 10: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 15"; tempReceivedDamageThreshold = 15; break;
	case 15: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 20"; tempReceivedDamageThreshold = 20; break;
	case 20: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 30"; tempReceivedDamageThreshold = 30; break;
	case 30: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 50"; tempReceivedDamageThreshold = 50; break;
	case 50: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 70"; tempReceivedDamageThreshold = 70; break;
	case 70: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 100"; tempReceivedDamageThreshold = 100; break;
	case 100: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 150"; tempReceivedDamageThreshold = 150; break;
	case 150: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 200"; tempReceivedDamageThreshold = 200; break;
	case 200: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 300"; tempReceivedDamageThreshold = 300; break;
	case 300: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 500"; tempReceivedDamageThreshold = 500; break;
	case 500: CombatTextOptions2[0].NamePtr = "Received Damage Threshold: 0"; tempReceivedDamageThreshold = 0; break;
	}
	SaveToConfig();
}
void __fastcall mergeCombatTextsHandler(int a1, int a2){
	switch (tempmergeCombatTexts) {
	case true: CombatTextOptions2[1].NamePtr = "merge Floating Texts: off"; tempmergeCombatTexts = false; break;
	case false: CombatTextOptions2[1].NamePtr = "merge Floating Texts: on"; tempmergeCombatTexts = true; break;
	}
	SaveToConfig();
}
void __fastcall showInflictedDamageHandler(int a1, int a2){
	switch (tempshowInflictedDamage) {
	case true: CombatTextOptions2[2].NamePtr = "show Floating Damage: off"; tempshowInflictedDamage = false; break;
	case false: CombatTextOptions2[2].NamePtr = "show Floating Damage: on"; tempshowInflictedDamage = true; break;
	}
	SaveToConfig();
}
void __fastcall MonsterReceivedDamageThresholdHandler(int a1, int a2){
	switch (tempMonsterReceivedDamageThreshold) {
	case 0: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 1"; tempMonsterReceivedDamageThreshold = 1; break;
	case 1: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 2"; tempMonsterReceivedDamageThreshold = 2; break;
	case 2: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 3"; tempMonsterReceivedDamageThreshold = 3; break;
	case 3: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 4"; tempMonsterReceivedDamageThreshold = 4; break;
	case 4: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 5"; tempMonsterReceivedDamageThreshold = 5; break;
	case 5: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 7"; tempMonsterReceivedDamageThreshold = 7; break;
	case 7: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 10"; tempMonsterReceivedDamageThreshold = 10; break;
	case 10: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 15"; tempMonsterReceivedDamageThreshold = 15; break;
	case 15: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 20"; tempMonsterReceivedDamageThreshold = 20; break;
	case 20: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 30"; tempMonsterReceivedDamageThreshold = 30; break;
	case 30: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 50"; tempMonsterReceivedDamageThreshold = 50; break;
	case 50: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 70"; tempMonsterReceivedDamageThreshold = 70; break;
	case 70: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 100"; tempMonsterReceivedDamageThreshold = 100; break;
	case 100: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 150"; tempMonsterReceivedDamageThreshold = 150; break;
	case 150: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 200"; tempMonsterReceivedDamageThreshold = 200; break;
	case 200: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 300"; tempMonsterReceivedDamageThreshold = 300; break;
	case 300: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 500"; tempMonsterReceivedDamageThreshold = 500; break;
	case 500: CombatTextOptions2[3].NamePtr = "Monster Received Damage Threshold: 0"; tempMonsterReceivedDamageThreshold = 0; break;
	}
	SaveToConfig();
}
void __fastcall FloatingHPRegenThresholdHandler(int a1, int a2){
	switch (tempFloatingHPRegenThreshold) {
	case 0: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 1"; tempFloatingHPRegenThreshold = 1; break;
	case 1: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 2"; tempFloatingHPRegenThreshold = 2; break;
	case 2: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 3"; tempFloatingHPRegenThreshold = 3; break;
	case 3: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 4"; tempFloatingHPRegenThreshold = 4; break;
	case 4: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 5"; tempFloatingHPRegenThreshold = 5; break;
	case 5: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 7"; tempFloatingHPRegenThreshold = 7; break;
	case 7: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 10"; tempFloatingHPRegenThreshold = 10; break;
	case 10: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 15"; tempFloatingHPRegenThreshold = 15; break;
	case 15: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 20"; tempFloatingHPRegenThreshold = 20; break;
	case 20: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 30"; tempFloatingHPRegenThreshold = 30; break;
	case 30: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 50"; tempFloatingHPRegenThreshold = 50; break;
	case 50: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 70"; tempFloatingHPRegenThreshold = 70; break;
	case 70: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 100"; tempFloatingHPRegenThreshold = 100; break;
	case 100: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 150"; tempFloatingHPRegenThreshold = 150; break;
	case 150: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 200"; tempFloatingHPRegenThreshold = 200; break;
	case 200: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 300"; tempFloatingHPRegenThreshold = 300; break;
	case 300: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 500"; tempFloatingHPRegenThreshold = 500; break;
	case 500: CombatTextOptions3[0].NamePtr = "Floating HP Regen Threshold: 0"; tempFloatingHPRegenThreshold = 0; break;
	}
	SaveToConfig();
}
void __fastcall FloatingManaRegenThresholdHandler(int a1, int a2){
	switch (tempFloatingManaRegenThreshold) {
	case 0: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 1"; tempFloatingManaRegenThreshold = 1; break;
	case 1: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 2"; tempFloatingManaRegenThreshold = 2; break;
	case 2: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 3"; tempFloatingManaRegenThreshold = 3; break;
	case 3: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 4"; tempFloatingManaRegenThreshold = 4; break;
	case 4: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 5"; tempFloatingManaRegenThreshold = 5; break;
	case 5: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 7"; tempFloatingManaRegenThreshold = 7; break;
	case 7: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 10"; tempFloatingManaRegenThreshold = 10; break;
	case 10: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 15"; tempFloatingManaRegenThreshold = 15; break;
	case 15: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 20"; tempFloatingManaRegenThreshold = 20; break;
	case 20: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 30"; tempFloatingManaRegenThreshold = 30; break;
	case 30: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 50"; tempFloatingManaRegenThreshold = 50; break;
	case 50: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 70"; tempFloatingManaRegenThreshold = 70; break;
	case 70: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 100"; tempFloatingManaRegenThreshold = 100; break;
	case 100: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 150"; tempFloatingManaRegenThreshold = 150; break;
	case 150: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 200"; tempFloatingManaRegenThreshold = 200; break;
	case 200: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 300"; tempFloatingManaRegenThreshold = 300; break;
	case 300: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 500"; tempFloatingManaRegenThreshold = 500; break;
	case 500: CombatTextOptions3[1].NamePtr = "Floating Mana Regen Threshold: 0"; tempFloatingManaRegenThreshold = 0; break;
	}
	SaveToConfig();
}
void __fastcall ShowBreakWarningHandler(int a1, int a2){
	switch (tempShowBreakWarning) {
	case true: CombatTextOptions3[2].NamePtr = "show Break Warning: off"; tempShowBreakWarning = false; break;
	case false: CombatTextOptions3[2].NamePtr = "show Break Warning: on"; tempShowBreakWarning = true; break;
	}
	SaveToConfig();
}

void __fastcall VisualOptions1MenuHandler(int a1, int a2) {
	switch(tempPanel){
	case true: VisualOptions1[0].NamePtr = "Mana: blue"; break;
	case false: VisualOptions1[0].NamePtr = "Mana: dark"; break;
	}
	switch (tempPotion) {
	case true: VisualOptions1[1].NamePtr = "Mana potions: blue"; break;
	case false: VisualOptions1[1].NamePtr = "Mana potions: dark"; break;
	}
	switch (tempMapblend) {
	case true: VisualOptions1[2].NamePtr = "map transparency: on"; break;
	case false: VisualOptions1[2].NamePtr = "map transparency: off"; break;
	}
	switch (tempShowNumbersOnHealth) {
	case true: VisualOptions1[3].NamePtr = "show Numbers On Health: on"; break;
	case false: VisualOptions1[3].NamePtr = "show Numbers On Health: off"; break;
	}
	switch (tempShowNumbersOnMana) {
	case true: VisualOptions2[0].NamePtr = "show Numbers On Mana: on"; break;
	case false: VisualOptions2[0].NamePtr = "show Numbers On Mana: off"; break;
	}
	switch (tempAlwaysHighlightGolem) {
	case true: VisualOptions2[1].NamePtr = "always Highlight Golem: on"; break;
	case false: VisualOptions2[1].NamePtr = "always Highlight Golem: off"; break;
	}
	switch (tempShowMonsterHealthBar) {
	case true: VisualOptions2[2].NamePtr = "show Monster Health Bar: on"; break;
	case false: VisualOptions2[2].NamePtr = "show Monster Health Bar: off"; break;
	}
	switch (tempShowVersionOnScreen) {
	case true: VisualOptions3[0].NamePtr = "show Version On Screen: on"; break;
	case false: VisualOptions3[0].NamePtr = "show Version On Screen: off"; break;
	}
	switch (tempRedInactiveSpellIcons) {
	case true: VisualOptions3[1].NamePtr = "Inactive Spell Icons: red"; break;
	case false: VisualOptions3[1].NamePtr = "Inactive Spell Icons: gray"; break;
	}
	Menu_SetupMenuWindow(VisualOptions1, 0);
}
void __fastcall PanelHandler(int a1, int a2) {
	switch (tempPanel) {
	case true: VisualOptions1[0].NamePtr = "Mana: dark"; tempPanel = false; break;
	case false: VisualOptions1[0].NamePtr = "Mana: blue"; tempPanel = true; break;
	}
	SaveToConfig();
}
void __fastcall PotionHandler(int a1, int a2) {
	switch (tempPotion) {
	case true: VisualOptions1[1].NamePtr = "Mana potions: dark"; tempPotion = false; break;
	case false: VisualOptions1[1].NamePtr = "Mana potions: blue"; tempPotion = true; break;
	}
	SaveToConfig();
}
void __fastcall MapblendHandler(int a1, int a2) {
	switch (tempMapblend) {
	case true: VisualOptions1[2].NamePtr = "map transparency: off"; tempMapblend = false; break;
	case false: VisualOptions1[2].NamePtr = "map transparency: on"; tempMapblend = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowNumbersOnHealthHandler(int a1, int a2) {
	switch (tempShowNumbersOnHealth) {
	case true: VisualOptions1[3].NamePtr = "show Numbers On Health: off"; tempShowNumbersOnHealth = false; break;
	case false: VisualOptions1[3].NamePtr = "show Numbers On Health: on"; tempShowNumbersOnHealth = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowNumbersOnManaHandler(int a1, int a2) {
	switch (tempShowNumbersOnMana) {
	case true: VisualOptions2[0].NamePtr = "show Numbers On Mana: off"; tempShowNumbersOnMana = false; break;
	case false: VisualOptions2[0].NamePtr = "show Numbers On Mana: on"; tempShowNumbersOnMana = true; break;
	}
	SaveToConfig();
}
void __fastcall AlwaysHighlightGolemHandler(int a1, int a2) {
	switch (tempAlwaysHighlightGolem) {
	case true: VisualOptions2[1].NamePtr = "always Highlight Golem: off"; tempAlwaysHighlightGolem = false; break;
	case false: VisualOptions2[1].NamePtr = "always Highlight Golem: on"; tempAlwaysHighlightGolem = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowMonsterHealthBarHandler(int a1, int a2) {
	switch (tempShowMonsterHealthBar) {
	case true: VisualOptions2[2].NamePtr = "Show Monster Health Bar: off"; tempShowMonsterHealthBar = false; break;
	case false: VisualOptions2[2].NamePtr = "Show Monster Health Bar: on"; tempShowMonsterHealthBar = true; break;
	}
	SaveToConfig();
}
void __fastcall ShowVersionOnScreenHandler(int a1, int a2) {
	switch (tempShowMonsterHealthBar) {
	case true: VisualOptions3[0].NamePtr = "Show Version On Screen: off"; tempShowMonsterHealthBar = false; break;
	case false: VisualOptions3[0].NamePtr = "Show Version On Screen: on"; tempShowMonsterHealthBar = true; break;
	}
	SaveToConfig();
}
void __fastcall RedInactiveSpellIconsHandler(int a1, int a2) {
	switch (tempRedInactiveSpellIcons) {
	case true: VisualOptions3[1].NamePtr = "Inactive Spell Icons: gray"; tempRedInactiveSpellIcons = false; break;
	case false: VisualOptions3[1].NamePtr = "Inactive Spell Icons: red"; tempRedInactiveSpellIcons = true; break;
	}
	SaveToConfig();
}
void __fastcall VisualOptions2MenuHandler(int a1, int a2) {
	Menu_SetupMenuWindow(VisualOptions2, 0);
}
void __fastcall VisualOptions3MenuHandler(int a1, int a2) {
	Menu_SetupMenuWindow(VisualOptions3, 0);
}
void __fastcall GameplayOptions1MenuHandler(int a1, int a2) {
	switch (tempRunindungeon) {
	case true: GameplayOptions1[0].NamePtr = "run in dungeon: on"; break;
	case false: GameplayOptions1[0].NamePtr = "run in dungeon: off"; break;
	}
	switch (tempAutopickup) {
	case true: GameplayOptions1[1].NamePtr = "auto pickup: on"; break;
	case false: GameplayOptions1[1].NamePtr = "auto pickup: off"; break;
	}
	switch (tempEasyclick)	{
	case true: GameplayOptions1[2].NamePtr = "easy click: on"; break;
	case false: GameplayOptions1[2].NamePtr = "easy click: off"; break;
	}
	switch (tempAlthold){
	case true: GameplayOptions1[3].NamePtr = "alt hold: on"; break;
	case false: GameplayOptions1[3].NamePtr = "alt hold: off"; break;
	}
	switch (tempSwapSpellsOnScroll){
	case true: GameplayOptions2[0].NamePtr = "mouse roll spells: on"; break;
	case false: GameplayOptions2[0].NamePtr = "mouse roll spells: off"; break;
	}
	Menu_SetupMenuWindow(GameplayOptions1, 0);
}
void __fastcall GameplayOptions2MenuHandler(int a1, int a2) {
	Menu_SetupMenuWindow(GameplayOptions2, 0);
}
void __fastcall RunindungeonHandler(int a1, int a2) {
	switch (tempRunindungeon) {
	case true: GameplayOptions1[0].NamePtr = "run in dungeon: off"; tempRunindungeon = false; break;
	case false: GameplayOptions1[0].NamePtr = "run in dungeon: on"; tempRunindungeon = true;  break;
	}
	SaveToConfig();
}
void __fastcall AutopickupHandler(int a1, int a2) {
	switch (tempAutopickup) {
	case true: GameplayOptions1[1].NamePtr = "auto pickup: off"; tempAutopickup = false; break;
	case false: GameplayOptions1[1].NamePtr = "auto pickup: on"; tempAutopickup = true;  break;
	}
	SaveToConfig();
}
void __fastcall EasyclickHandler(int a1, int a2) {
	switch (tempEasyclick) {
	case true: GameplayOptions1[2].NamePtr = "easy click: off"; tempEasyclick = false; break;
	case false: GameplayOptions1[2].NamePtr = "easy click: on"; tempEasyclick = true;  break;
	}
	SaveToConfig();
}
void __fastcall AltholdHandler(int a1, int a2) {
	switch (tempAlthold) {
	case true: GameplayOptions1[3].NamePtr = "alt hold: off"; tempAlthold = false; break;
	case false: GameplayOptions1[3].NamePtr = "alt hold: on"; tempAlthold = true;  break;
	}
	SaveToConfig();
}
void __fastcall SwapSpellsOnScrollHandler(int a1, int a2) {
	switch (tempSwapSpellsOnScroll) {
	case true: GameplayOptions2[0].NamePtr = "mouse roll spells: off"; tempSwapSpellsOnScroll = false; break;
	case false: GameplayOptions2[0].NamePtr = "mouse roll spells: on"; tempSwapSpellsOnScroll = true;  break;
	}
	SaveToConfig();
}

void __fastcall VideoOptions1MenuHandler(int a1, int a2)
{
	switch (tempWidth) {
	case 640: VideoOptions1[0].NamePtr = "screen width: 640";   break;
	case 800: VideoOptions1[0].NamePtr = "screen width: 800";   break;
	case 1024: VideoOptions1[0].NamePtr = "screen width: 1024"; break;
	case 1152: VideoOptions1[0].NamePtr = "screen width: 1152"; break;
	case 1280: VideoOptions1[0].NamePtr = "screen width: 1280"; break;
	case 1366: VideoOptions1[0].NamePtr = "screen width: 1366"; break;
	case 1400: VideoOptions1[0].NamePtr = "screen width: 1400"; break;
	case 1440: VideoOptions1[0].NamePtr = "screen width: 1440"; break;
	case 1600: VideoOptions1[0].NamePtr = "screen width: 1600"; break;
	case 1680: VideoOptions1[0].NamePtr = "screen width: 1680"; break;
	case 1920: VideoOptions1[0].NamePtr = "screen width: 1920"; break;
	case 2048: VideoOptions1[0].NamePtr = "screen width: 2048"; break;
	case 2560: VideoOptions1[0].NamePtr = "screen width: 2560"; break;
	case 3840: VideoOptions1[0].NamePtr = "screen width: 3840"; break;
	default: VideoOptions1[0].NamePtr = "screen width: 640"; tempWidth = 640; break;
	}

	switch (tempHeight) {
	 case 480: VideoOptions1[1].NamePtr = "screen height: 480";    break;
	 case 486: VideoOptions1[1].NamePtr = "screen height: 486";    break;
	 case 540: VideoOptions1[1].NamePtr = "screen height: 540";    break;
	 case 576: VideoOptions1[1].NamePtr = "screen height: 576";    break;
	 case 600: VideoOptions1[1].NamePtr = "screen height: 600";    break;
	 case 648: VideoOptions1[1].NamePtr = "screen height: 648";    break;
	 case 720: VideoOptions1[1].NamePtr = "screen height: 720";    break;
	 case 768: VideoOptions1[1].NamePtr = "screen height: 768";    break;
	 case 800: VideoOptions1[1].NamePtr = "screen height: 800";    break;
	 case 900: VideoOptions1[1].NamePtr = "screen height: 900";    break;
	case 1024: VideoOptions1[1].NamePtr = "screen height: 1024";   break;
	case 1050: VideoOptions1[1].NamePtr = "screen height: 1050";   break;
	case 1080: VideoOptions1[1].NamePtr = "screen height: 1080";   break;
	case 1200: VideoOptions1[1].NamePtr = "screen height: 1200";   break;
	case 1440: VideoOptions1[1].NamePtr = "screen height: 1440";   break;
	case 1536: VideoOptions1[1].NamePtr = "screen height: 1536";   break;
	case 1600: VideoOptions1[1].NamePtr = "screen height: 1600";   break;
	case 2048: VideoOptions1[1].NamePtr = "screen height: 2048";   break;
	case 2160: VideoOptions1[1].NamePtr = "screen height: 2160";   break;
	default: VideoOptions1[1].NamePtr = "screen height: 480"; tempHeight = 480; break;
	}
	switch (tempFullscreen)
	{
	case true: VideoOptions1[2].NamePtr = "Fullscreen: YES"; break;
	case false: VideoOptions1[2].NamePtr = "Fullscreen: NO"; break;
	}
	switch (tempFPS) {
	case 20: VideoOptions1[3].NamePtr = "FPS: 20"; break;
	case 40: VideoOptions1[3].NamePtr = "FPS: 40"; break;
	case 60: VideoOptions1[3].NamePtr = "FPS: 60"; break;
	case 80: VideoOptions1[3].NamePtr = "FPS: 80"; break;
	default: VideoOptions1[3].NamePtr = "FPS: 60"; tempFPS = 60; break;
	}
	switch (tempShowFPS)
	{
	case true: VideoOptions2[0].NamePtr = "Show FPS: YES"; break;
	case false: VideoOptions2[0].NamePtr = "Show FPS: NO"; break;
	}
	if (firstTimeFS) {
		SwitchFullscreen(); //alt enter works second time
		firstTimeFS = false;
	}
	Menu_SetupMenuWindow(VideoOptions1, 0);
}
void __fastcall VideoOptions2MenuHandler(int a1, int a2)
{
	Menu_SetupMenuWindow(VideoOptions2, 0);
}
void __fastcall StayIdle(int a1, int a2)
{
}
void __fastcall ScreenHeightHandler(int a1, int a2)
{
	switch (tempHeight) {
	 case 480: VideoOptions1[1].NamePtr = "screen height: 486";  tempHeight = 486; break;
	 case 486: VideoOptions1[1].NamePtr = "screen height: 540";  tempHeight = 540; break;
	 case 540: VideoOptions1[1].NamePtr = "screen height: 576";  tempHeight = 576; break;
	 case 576: VideoOptions1[1].NamePtr = "screen height: 600";  tempHeight = 600; break;
	 case 600: VideoOptions1[1].NamePtr = "screen height: 648";  tempHeight = 648; break;
	 case 648: VideoOptions1[1].NamePtr = "screen height: 720";  tempHeight = 720; break;
	 case 720: VideoOptions1[1].NamePtr = "screen height: 768";  tempHeight = 768; break;
	 case 768: VideoOptions1[1].NamePtr = "screen height: 800";  tempHeight = 800; break;
	 case 800: VideoOptions1[1].NamePtr = "screen height: 900";  tempHeight = 900; break;
	 case 900: VideoOptions1[1].NamePtr = "screen height: 1024"; tempHeight = 1024; break;
	case 1024: VideoOptions1[1].NamePtr = "screen height: 1050"; tempHeight = 1050; break;
	case 1050: VideoOptions1[1].NamePtr = "screen height: 1080"; tempHeight = 1080; break;
	case 1080: VideoOptions1[1].NamePtr = "screen height: 1200"; tempHeight = 1200; break;
	case 1200: VideoOptions1[1].NamePtr = "screen height: 1440"; tempHeight = 1440; break;
	case 1440: VideoOptions1[1].NamePtr = "screen height: 1536"; tempHeight = 1536; break;
	case 1536: VideoOptions1[1].NamePtr = "screen height: 1600"; tempHeight = 1600; break;
	case 1600: VideoOptions1[1].NamePtr = "screen height: 2048"; tempHeight = 2048; break;
	case 2048: VideoOptions1[1].NamePtr = "screen height: 2160"; tempHeight = 2160; break;
	case 2160: VideoOptions1[1].NamePtr = "screen height: 480";  tempHeight = 480; break;
	}
	SaveToConfig();
	ScreenHeight = tempHeight;
	ChangeScreenRes();
}
void __fastcall ScreenWidthHandler(int a1, int a2) {
	switch (tempWidth) {
	case 640: VideoOptions1[0].NamePtr = "screen width: 800";  tempWidth = 800; break;
	case 800: VideoOptions1[0].NamePtr = "screen width: 1024";  tempWidth = 1024; break;
	case 1024: VideoOptions1[0].NamePtr = "screen width: 1152";  tempWidth = 1152; break;
	case 1152: VideoOptions1[0].NamePtr = "screen height: 1280"; tempWidth = 1280; break;
	case 1280: VideoOptions1[0].NamePtr = "screen width: 1366";  tempWidth = 1366; break;
	case 1366: VideoOptions1[0].NamePtr = "screen width: 1400";  tempWidth = 1400; break;
	case 1400: VideoOptions1[0].NamePtr = "screen width: 1440";  tempWidth = 1440; break;
	case 1440: VideoOptions1[0].NamePtr = "screen width: 1600";  tempWidth = 1600; break;
	case 1600: VideoOptions1[0].NamePtr = "screen width: 1680";  tempWidth = 1680; break;
	case 1680: VideoOptions1[0].NamePtr = "screen width: 1920";  tempWidth = 1920; break;
	case 1920: VideoOptions1[0].NamePtr = "screen width: 2048";  tempWidth = 2048; break;
	case 2048: VideoOptions1[0].NamePtr = "screen width: 2560";  tempWidth = 2560; break;
	case 2560: VideoOptions1[0].NamePtr = "screen width: 3840";  tempWidth = 3840; break;
	case 3840: VideoOptions1[0].NamePtr = "screen width: 640";  tempWidth = 640; break;
	}
	SaveToConfig();
	ScreenWidth = tempWidth;
	ChangeScreenRes();
}

void __fastcall FullScreenHandler(int a1, int a2) {
	switch (tempFullscreen)
	{
	case true: VideoOptions1[2].NamePtr = "Fullscreen: NO";  tempFullscreen = false; break;
	case false: VideoOptions1[2].NamePtr = "Fullscreen: YES";  tempFullscreen = true; break;
	}
	SaveToConfig();
	SwitchFullscreen();
	MainDrawFunc();
}
void __fastcall FPSHandler(int a1, int a2) {
	switch (tempFPS) {
	case 20: VideoOptions1[3].NamePtr = "FPS: 40"; tempFPS = 40; break;
	case 40: VideoOptions1[3].NamePtr = "FPS: 60"; tempFPS = 60; break;
	case 60: VideoOptions1[3].NamePtr = "FPS: 80"; tempFPS = 80; break;
	case 80: VideoOptions1[3].NamePtr = "FPS: 20"; tempFPS = 20; break;
	}
	SaveToConfig();
}
void __fastcall ShowFPSHandler(int a1, int a2) {
	switch (tempShowFPS)
	{
	case true: VideoOptions2[0].NamePtr = "Show FPS: NO";  tempShowFPS = false; break;
	case false: VideoOptions2[0].NamePtr = "Show FPS: YES";  tempShowFPS = true; break;
	}
	SaveToConfig();
}

void __fastcall SaveToConfigHandler (int a1, int a2) {
	
}

//----- (0041B4B5) -------------------------------------------------------- menu
void SetMusicMenu()
{
	SetVolumeMenu(MusicMenu, &OptionsMenu[OP_MUSIC], GetOrSetMusicVolume(1));
}

//----- (th3) ------------------------------------------------------------- menu
void SetAmbientMenu()
{
	SetVolumeMenu(AmbientMenu, &OptionsMenu[OP_AMBIENT], GetOrSetAmbientVolume(1));
}

//----- (0041B4CE) -------------------------------------------------------- menu
void __fastcall SetVolumeMenu(char** text, Menu* menu, int volume)
{
	if( IsDirectSoundAllocated ){
		menu->Flags |= MENU_ENABLED | MENU_SLIDER;
		menu->NamePtr = text[SLIDER_ON];
		Menu_SetSliderTicks(menu, VOL_TICKS);
		Menu_SetSlider(menu, VOL_MIN, 0, volume);
	}else{
		menu->Flags &= ~(MENU_ENABLED | MENU_SLIDER);
		menu->NamePtr = text[SLIDER_DISABLED];
	}
}

//----- (0041B50F) -------------------------------------------------------- menu
void SetSoundMenu()
{
	SetVolumeMenu(SoundMenu, &OptionsMenu[OP_SOUND], GetOrSetSoundVolume(1));
}

//----- (0041B528) -------------------------------------------------------- menu
void SetWalkMenu()
{
	Menu_SetSliderTicks(&OptionsMenu[OP_JOG], 2);
	Menu_SetSlider(&OptionsMenu[OP_JOG], 0, 1, IsFastWalkEnabled);
	OptionsMenu[OP_JOG].NamePtr = WalkMenu[IsFastWalkEnabled == 0];
}

//----- (0041B562) -------------------------------------------------------- menu
void SetGammaMenu()
{
	Menu_SetSliderTicks(&OptionsMenu[OP_GAMMA], 15);
	Menu_SetSlider(&OptionsMenu[OP_GAMMA], 30, 100, SetGammaByPos(0));
}

//----- (th2) ------------------------------------------------------------- menu
void SetContrastMenu()
{
	Menu_SetSliderTicks(&OptionsMenu[OP_CONTRAST], 15);
	Menu_SetSlider(&OptionsMenu[OP_CONTRAST], 30, 100, SetContrastByPos(0));
}

//----- (0041B588) -------------------------------------------------------- menu
void __fastcall MusicMenuHandler(int a1, int a2)
{
	int v3; // eax@5
	int v4; // esi@5
	if( a1 ){
		if( IsMusicEnabled ){
			IsMusicEnabled = 0;
			StopMusic();
			GetOrSetMusicVolume(VOL_MIN);
			goto LABEL_14;
		}
		IsMusicEnabled = 1;
		GetOrSetMusicVolume(0);
		goto LABEL_10;
	}
	v3 = Menu_GetVolume(&OptionsMenu[OP_MUSIC]);
	v4 = v3;
	GetOrSetMusicVolume(v3);
	if( v4 != VOL_MIN ){
		if( IsMusicEnabled )
			goto LABEL_14;
		IsMusicEnabled = 1;
		LABEL_10:
		PlayMusic();
		goto LABEL_14;
	}
	if( IsMusicEnabled ){
		IsMusicEnabled = 0;
		StopMusic();
	}
	LABEL_14:
	SetMusicMenu();
}

//----- (0041B588) -------------------------------------------------------- menu
void __fastcall AmbientMenuHandler(int activate, int a2)
{
	if( activate ){
		if( IsAmbientEnabled ){
			IsAmbientEnabled = 0;
			StopAmbient();
			GetOrSetAmbientVolume(VOL_MIN);
		}else{
			IsAmbientEnabled = 1;
			GetOrSetAmbientVolume(VOL_MAX);
			PlayAmbient();
		}
	}else{
		int volume = Menu_GetVolume(&OptionsMenu[OP_AMBIENT]);
		GetOrSetAmbientVolume(volume);
		if( volume == VOL_MIN ){
			if( IsAmbientEnabled ){
				IsAmbientEnabled = 0;
				StopAmbient();
			}
		}else{
			if( ! IsAmbientEnabled ){
				IsAmbientEnabled = 1;
				PlayAmbient();
			}
		}
	}
	SetAmbientMenu();
}

//----- (0041B626) -------------------------------------------------------- menu
int __fastcall Menu_GetVolume(Menu* menuItem)
{
	return Menu_GetRegulatorPos(menuItem, VOL_MIN, 0);
}

//----- (0041B633) -------------------------------------------------------- menu
void __fastcall SoundMenuHandler(int a1, int a2)
{
	int v2; // ecx@3
	int v3; // eax@6
	int v4; // esi@6
	if( a1 ){
		if( IsSoundEnable ){
			IsSoundEnable = 0;
			StopAllSounds();
			v2 = VOL_MIN;
		}else{
			IsSoundEnable = 1;
			v2 = 0;
		}
		GetOrSetSoundVolume(v2);
	}else{
		v3 = Menu_GetVolume(&OptionsMenu[OP_SOUND]);
		v4 = v3;
		GetOrSetSoundVolume(v3);
		if( v4 == VOL_MIN ){
			if( IsSoundEnable ){
				IsSoundEnable = 0;
				StopAllSounds();
			}
		}else{
			if( !IsSoundEnable )
				IsSoundEnable = 1;
		}
	}
	PlayGlobalSound(S_75_I_TITLEMOV);
	SetSoundMenu();
}

//----- (0041B6B4) -------------------------------------------------------- menu
void __fastcall WalkMenuHandler(int a1, int a2)
{
	if( MaxCountOfPlayersInGame == 1 ){
		IsFastWalkEnabled = IsFastWalkEnabled == 0;
		#ifndef NO_FASTWALK
		SRegSaveValue("Hellfire", "Fast Walk", 0, IsFastWalkEnabled);
		#endif
		PlayGlobalSound( S_75_I_TITLEMOV );
		SetWalkMenu();
	}
}

//----- (0041B6EE) -------------------------------------------------------- menu
void __fastcall GammaMenuHandler(int a1, int a2)
{
	int pos = a1 ? (((SetGammaByPos(0) != 30) - 1) & 0x46) + 30
	             : Menu_GetRegulatorPos(&OptionsMenu[OP_GAMMA], 30, 100);
	SetGammaByPos(pos);
	SetGammaMenu();
}

//----- (th2) ------------------------------------------------------------- menu
void __fastcall ContrastMenuHandler(int a1, int a2)
{
	int pos = a1 ? (((SetContrastByPos(0) != 30) - 1) & 0x46) + 30
		: Menu_GetRegulatorPos(&OptionsMenu[OP_CONTRAST], 30, 100);
	SetContrastByPos(pos);
	SetContrastMenu();
}
