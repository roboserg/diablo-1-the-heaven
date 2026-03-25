#include "stdafx.h"

struct GraphicsMode
{
	int Width;
	int Height;
} GraphicsModes[] = {
	{ 640, 480 },         //  0
	{ 720, 576 },         //  1
	{ 800, 600 },         //  2
	{ 1024, 768 },        //  3
	{ 1152, 864 },        //  4
	{ 1280, 1024 },       //  5
	{ 1600, 1200 },       //  6
	{ 640, 480 },         //  7
	{ 720, 480 },         //  8   best for 1680x1050
	{ 848, 480 },         //  9
	{ 960, 600 },         // 10   best for 1680x1050
	{ 1280, 720 },        // 11
	{ 1280, 768 },        // 12   best for 1680x1050
	{ 1280, 800 },        // 13
	{ 1280, 960 },        // 14
	{ 1366, 768 },        // 15   best for my very cheap laptop
	{ 1440, 900 },        // 16   best for 1680x1050
	{ 1600, 900 },        // 17
	{ 1680, 1050 },       // 18
	{ 1920, 1080 },       // 19
	{ 1920, 1200 },       // 20   the best of the best
};

int DefaultVideoMode = 0; // Don't change this !!! For high resolution use config.ini file editing
bool IsNotMapOpen;
int ScreenWidth = GraphicsModes[ DefaultVideoMode ].Width;// в оригинале константа 640 неименованная подставлена везде
int ScreenHeight = GraphicsModes[ DefaultVideoMode ].Height;// в оригинале константа неименованная подставлена везде
int HeightShift = 0;
int WorkingWidth = ScreenWidth + Screen_LeftBorder + Screen_RightBorder; // в оригинале 768
int WorkingHeight = ScreenHeight + Screen_TopBorder + Screen_BottomBorder; // в оригинале 656
int MaxScreenWidth, MaxScreenHeight, MaxWorkingWidth, MaxWorkingHeight;
int DrawMode = 1; // textured quad mode by default
_D3DTEXTUREFILTERTYPE FilteringMode = D3DTEXF_LINEAR; // bilinear filtration by default

bool IsAutoPickup = false;
bool BigWin = false;
int WinHeight;
int WinWidth;
int StartWindowX;
int StartWindowY;

int MaxFPS = 60;
bool ShowFps = false;
bool Vsync = true;

bool ExternalStorm = true;
bool OriginalMenu = true;
bool AllSound = true;
bool DropUnderMonsters = false;
bool IsFullScreen = false;
bool IsThException = false;
bool IsStartVideo = true;
bool LoadOnStart = false;
bool NewGameOnStart = false;
int DifficultyOnStart = 0;
bool LoadMulti = false;
int AutoLoadSlot = -1;
bool AutoCopySave = false;
bool VisualOnly = false;
bool SyncTH = true;
bool SyncTimer = true;
int SingleLog = 0;
int ArgCheckMode = 0;
bool SaveFolder = false;
bool AutoBackup = false;
#if CHEATS
bool DevelopMode = false;
bool FreeSpell = false;
bool IsGodMode = false;
bool OneShoot = false;
bool Hedgehog = false;
bool SaveAlwaysEnabled = false;
#endif
bool IsSecondWin = false;
bool RunInDungeon = false;
bool PanelsOnCenter = false;
bool MapItem = true;
bool Lovecraft = false;   // Gribnik
bool MapBlend = true;
bool showCombatText = true;
int FloatingHPRegenThreshold = 0;
int FloatingManaRegenThreshold = 0;
int ScreenCenterUserShiftVertical = 0;
bool ShowBreakWarning = true;
bool showInflictedDamage = true;
bool ShowNumbersOnHealth = true;
bool ShowNumbersOnMana = true;
bool ShowReceivedDamage = true;
int ReceivedDamageThreshold = 0;
bool showCombatExp = true;
bool AlwaysHighlightSummons = true;
int MonsterReceivedDamageThreshold = 0;
bool ShowMonsterHealthBar=true;
bool ShowVersionOnScreen = false;
bool mergeCombatTexts = true;
bool ShowPercentExpGain = false;
bool NeedAltHold = true;
bool CanSkipVictoryVideo = false;
bool SwapSpellsOnScroll = false;
bool RedInactiveSpellIcons = false;
int FullscreenFX = 0; // tumbler for fullscreen shader
bool RandomizeSPQuests = true;
bool NpcSign = true;
bool ShowObjectsOnMiniMap = false;
bool BlueTownPortal = false;
bool NewShortcuts = true;
bool SkipUnusableSpells = true;
bool MoreManualHotkeys = true;
bool BackgroundSound = true;
bool ConfigLoaded = false;
bool InstantHotkeys = false;
bool ExpandedSpellsOnScroll = true;
bool DDraw = false;
bool NewDunConfirm = false;
bool AltOnlyItemPick = false;
bool BackgroundDraw = true;
bool OriginalMusic = false;
bool Doom_Music = false;
bool original_palettes = false;
bool SecondaryTooltip = true;
bool AlternativeMonsterBarBorder = true;
bool ColoredMonsterBarBorder = false;
bool ExtraMouseButtons = false;
bool ShowMonsterStunThreshold = false;
bool DisableESCinBattle = false;
bool MonsterWalkSounds = true;
int PlayerRunningSounds = 2;
bool CanSwitchPerkSynergyPanel = false;
bool ShowSecondaryPerks = false;
bool RememberPerkSelection = false;
bool InventoryBackgroundColorBlue = false;
bool InventoryBackgroundOldStyle = false;
bool InventoryTurnOffRedColorItems = false;
bool ShowSystemTime = false;
bool ForceMoveOnCtrl = false;
bool DropItemOnCtrl = false;
bool KeepSpellsOnCtrls = false;
bool ShowPlayerAuraHitVisual = false;
bool IsRestrictMouseToWindow = false;
bool XinputEnabled = true;
char UserPlayerTrn[ 256/*4096*/ ] = "";
bool UserPanelB = false;
bool UserPotionB = false;
char UserSeed[1024] = "";
bool ShowGameMode = false;
bool LowCpuUsage = true;
bool ForceZoom = false;
bool ShowResolution = false;
bool FastInventoryItemSwap = true;
bool OnScreenTextOutline = true;
bool FastSwapRightOnAlt = false;
bool OriginalLightLevels = true;
bool DropRares = true;
bool RelixToScrolls = false;
bool DisplayMonstersLeft = false;
bool TransmuteAllowExtra = false;
bool TransmuteKeepItems = false;
bool TransmuteAllowMultiple = false;
bool TransmuteShowRecipeList = true;
bool DuplicateSounds = true;
bool LimitCpuCore = false;
bool ClassicInMainMenu = true;
bool ColiseumInMainMenu = true;
bool ClassicSaveFolder = true;
bool ColiseumSaveFolder = true;
bool SelectGameModeLater = true;

// терерь используется cfg<ParamName>, вместо ручного со!ния переменной под каждый буфер строки параметра
// также добавлена возможность задавать значение по умолчнию, пока только для bool параметров
ConfigLine ConfigStructure[] = {
	// line type, in game menu avialability, param key name, param reference, default value
	{ cfgString, MO_INT, "screenwidth", "Screen width", &cfg<ScreenWidth> },
	{ cfgString, MO_INT, "screenheight", "Screen height", &cfg<ScreenHeight> },
	{ cfgString, 0, "storm", 0, &cfg<ExternalStorm> },
	{ cfgString, 0, "menu", 0, &cfg<OriginalMenu> },
	{ cfgString, 0, "sound", 0, &cfg<AllSound>, ON },
	{ cfgString, 0, "map", 0, &cfg<IsNotMapOpen> },
#if CHEATS
	{ cfgString, 0, "devmode", 0, &cfg<DevelopMode>, OFF },
	{ cfgString, 0, "freespell", 0, &cfg<FreeSpell>, OFF },
	{ cfgString, 0, "godmode", 0, &cfg<IsGodMode>, OFF },
	{ cfgString, 0, "savealways", 0, &cfg<SaveAlwaysEnabled>, OFF },
#endif
	{ cfgString, 0, "dropundermonsters", 0, &cfg<DropUnderMonsters>, OFF },
	{ cfgString, MO_LIST, "Fullscreen", 0, &cfg<IsFullScreen>, OFF },
	{ cfgString, MO_BOOL, "filter", "Texture filter", &cfg<FilteringMode> },
	{ cfgString, MO_BOOL, "vsync", "V-Sync", &cfg<Vsync>, ON },
	{ cfgString, MO_INT, "fps", "Max FPS", &cfg<MaxFPS> },
	{ cfgString, MO_BOOL, "showfps", "Show FPS", &cfg<ShowFps>, OFF },
	{ cfgString, 0, "exception", 0, &cfg<IsThException>, OFF },
	{ cfgString, MO_BOOL, "startvideo", 0, &cfg<IsStartVideo>, ON },
	{ cfgString, 0, "loadonstart", 0, &cfg<LoadOnStart> },
	{ cfgString, 0, "visualonly", 0, &cfg<VisualOnly>, OFF },
	{ cfgString, 0, "sync", 0, &cfg<SyncTH>, ON },
	{ cfgString, 0, "autocopysave", 0, &cfg<AutoCopySave>, OFF },
	{ cfgString, MO_BOOL, "Autopickup", "Gold Auto Pickup", &cfg<IsAutoPickup>, OFF },
	{ cfgString, MO_BOOL, "EasyClick", "EasyClick mode", &cfg<EasyClick>, ON },
	{ cfgString, MO_BOOL, "althold", 0, &cfg<NeedAltHold>, ON }, 
	{ cfgString, 0, "savefolder", 0, &cfg<SaveFolder>, OFF },
	{ cfgString, 0 /*MO_BOOL*/, "autobackup", "Auto backup save files", &cfg<AutoBackup>, OFF },
	{ cfgString, 0, "singlelog", 0, &cfg<SingleLog> },
	{ cfgString, 0, "playertrn", 0, &cfg<UserPlayerTrn> },
	{ cfgString, 0, "newgame", 0, &cfg<NewGameOnStart>, OFF },
	{ cfgString, 0, "difficulty", 0, &cfg<DifficultyOnStart> },
	{ cfgString, MO_BOOL, "panel", 0, &cfg<UserPanelB>, OFF },
	{ cfgString, 0, "drawmode", 0, &cfg<DrawMode> },
	{ cfgString, MO_BOOL, "potion", 0, &cfg<UserPotionB>, OFF },
	{ cfgString, 0, "vertical", 0, &cfg<VerticalWin>, ON },
	{ cfgString, 0, "seed", 0, &cfg<UserSeed> },
	{ cfgString, 0, "secondwin", 0, &cfg<IsSecondWin>, OFF },
	{ cfgString, 0, "runindungeon", 0, &cfg<RunInDungeon>, ON },
	{ cfgString, 0, "centeredpanels", 0, &cfg<PanelsOnCenter>, OFF },
	{ cfgString, MO_BOOL, "mapitem", 0, &cfg<MapItem>, ON },
	{ cfgString, MO_BOOL, "lovecraft", 0, &cfg<Lovecraft>, OFF },      // Gribnik
	{ cfgString, MO_BOOL, "mapblend", 0, &cfg<MapBlend>, ON },
	{ cfgString, MO_BOOL, "showCombatText", 0, &cfg<showCombatText>, ON },
	{ cfgString, MO_INT, "floatingHPRegenThreshold", 0, &cfg<FloatingHPRegenThreshold> },
	{ cfgString, MO_INT, "floatingManaRegenThreshold", 0, &cfg<FloatingManaRegenThreshold> },
	{ cfgString, MO_INT, "ScreenCenterUserShiftVertical", 0, &cfg<ScreenCenterUserShiftVertical> },
	{ cfgString, MO_BOOL, "showBreakWarning", 0, &cfg<ShowBreakWarning>, ON },
	{ cfgString, MO_BOOL, "showInflictedDamage", 0, &cfg<showInflictedDamage>, ON },
	{ cfgString, MO_BOOL, "showNumbersOnHealth", 0, &cfg<ShowNumbersOnHealth>, ON },
	{ cfgString, MO_BOOL, "showNumbersOnMana", 0, &cfg<ShowNumbersOnMana>, ON },
	{ cfgString, MO_BOOL, "showReceivedDamage", 0, &cfg<ShowReceivedDamage>, ON },
	{ cfgString, MO_INT, "receivedDamageThreshold", 0, &cfg<ReceivedDamageThreshold> },
	{ cfgString, MO_BOOL, "showCombatExp", 0, &cfg<showCombatExp>, ON },
	{ cfgString, MO_BOOL, "alwaysHighlightSummons", 0, &cfg<AlwaysHighlightSummons>, ON },
	{ cfgString, MO_INT, "monsterReceivedDamageThreshold", 0, &cfg<MonsterReceivedDamageThreshold> },
	{ cfgString, MO_BOOL, "showMonsterHealthBar", 0, &cfg<ShowMonsterHealthBar>, ON },
	{ cfgString, MO_BOOL, "showVersionOnScreen", 0, &cfg<ShowVersionOnScreen>, OFF },
	{ cfgString, MO_BOOL, "mergeCombatTexts", 0, &cfg<mergeCombatTexts>, ON },
	{ cfgString, MO_BOOL, "showPercentExpGain", 0, &cfg<ShowPercentExpGain>, OFF }, 
	{ cfgString, MO_BOOL, "skipvictory", "Can skip victory video", &cfg<CanSkipVictoryVideo>, OFF }, 
	{ cfgString, MO_BOOL, "swapSpellsOnScroll", 0, &cfg<SwapSpellsOnScroll>, OFF },
	{ cfgString, MO_BOOL, "RedInactiveSpellIcons", 0, &cfg<RedInactiveSpellIcons>, OFF },
	{ cfgString, MO_INT, "FullscreenFX", 0, &cfg<FullscreenFX> },
	{ cfgString, MO_BOOL, "RandomizeSPQuests", 0, &cfg<RandomizeSPQuests>, ON },
	{ cfgString, MO_BOOL, "NpcSign", 0, &cfg<NpcSign>, ON },
	{ cfgString, MO_BOOL, "ShowObjectsOnMiniMap", 0, &cfg<ShowObjectsOnMiniMap>, OFF },
	{ cfgString, MO_BOOL, "BlueTownPortal", 0, &cfg<BlueTownPortal>, OFF },
	{ cfgString, MO_BOOL, "NewShortcuts", 0, &cfg<NewShortcuts>, ON },
	{ cfgString, MO_BOOL, "SkipUnusableSpells", 0, &cfg<SkipUnusableSpells>, ON },
	{ cfgString, MO_BOOL, "MoreManualHotkeys", 0, &cfg<MoreManualHotkeys>, ON },
	{ cfgString, MO_BOOL, "InstantHotkeys", 0, &cfg<InstantHotkeys>, OFF },
	{ cfgString, MO_BOOL, "ExpandedSpellsOnScroll", 0, &cfg<ExpandedSpellsOnScroll>, ON },
	{ cfgString, MO_BOOL, "BackgroundSound", 0, &cfg<BackgroundSound>, ON },
	{ cfgString, MO_BOOL, "DDraw", 0, &cfg<DDraw>, OFF },
	{ cfgString, MO_BOOL, "NewDunConfirm", 0, &cfg<NewDunConfirm>, OFF },
	{ cfgString, MO_BOOL, "AltOnlyItemPick", 0, &cfg<AltOnlyItemPick>, OFF },
	{ cfgString, MO_BOOL, "BackgroundDraw", 0, &cfg<BackgroundDraw>, ON },
	{ cfgString, MO_BOOL, "OriginalMusic", 0, &cfg<OriginalMusic>, OFF },
	{ cfgString, MO_BOOL, "Doom_Music", 0, &cfg<Doom_Music>, OFF },
	{ cfgString, MO_BOOL, "original_palettes", 0, &cfg<original_palettes>, OFF },
	{ cfgString, MO_BOOL, "SecondaryTooltip", 0, &cfg<SecondaryTooltip>, ON },
	{ cfgString, MO_BOOL, "AlternativeMonsterBarBorder", 0, &cfg<AlternativeMonsterBarBorder>, ON },
	{ cfgString, MO_BOOL, "ColoredMonsterBarBorder", 0, &cfg<ColoredMonsterBarBorder>, OFF },
	{ cfgString, MO_BOOL, "ExtraMouseButtons", 0, &cfg<ExtraMouseButtons>, OFF },
	{ cfgString, MO_BOOL, "ShowMonsterStunThreshold", 0, &cfg<ShowMonsterStunThreshold>, OFF },
	{ cfgString, MO_BOOL, "DisableESCinBattle", 0, &cfg<DisableESCinBattle>, OFF },
	{ cfgString, MO_BOOL, "MonsterWalkSounds", 0, &cfg<MonsterWalkSounds>, ON },
	{ cfgString, MO_INT, "PlayerRunningSounds", 0, &cfg<PlayerRunningSounds> },
	{ cfgString, MO_BOOL, "CanSwitchPerkSynergyPanel", 0, &cfg<CanSwitchPerkSynergyPanel>, OFF },
	{ cfgString, MO_BOOL, "ShowSecondaryPerks", 0, &cfg<ShowSecondaryPerks>, OFF },
	{ cfgString, MO_BOOL, "RememberPerkSelection", 0, &cfg<RememberPerkSelection>, OFF },
	{ cfgString, MO_BOOL, "InventoryBackgroundColorBlue", 0, &cfg<InventoryBackgroundColorBlue>, OFF },
	{ cfgString, MO_BOOL, "InventoryBackgroundOldStyle", 0, &cfg<InventoryBackgroundOldStyle>, OFF },
	{ cfgString, MO_BOOL, "InventoryTurnOffRedColorItems", 0, &cfg<InventoryTurnOffRedColorItems>, OFF },
	{ cfgString, MO_BOOL, "ShowSystemTime", 0, &cfg<ShowSystemTime>, OFF },
	{ cfgString, MO_BOOL, "ForceMoveOnCtrl", 0, &cfg<ForceMoveOnCtrl>, OFF },
	{ cfgString, MO_BOOL, "DropItemOnCtrl", 0, &cfg<DropItemOnCtrl>, OFF },
	{ cfgString, MO_BOOL, "KeepSpellsOnCtrls", 0, &cfg<KeepSpellsOnCtrls>, OFF },
	{ cfgString, MO_BOOL, "ShowPlayerAuraHitVisual", 0, &cfg<ShowPlayerAuraHitVisual>, OFF },
	{ cfgString, MO_BOOL, "RestrictMouseToWindow", 0, &cfg<IsRestrictMouseToWindow>, OFF },
	{ cfgString, MO_BOOL, "XinputEnabled", 0, &cfg<XinputEnabled>, ON },
	{ cfgString, MO_BOOL, "ShowGameMode", 0, &cfg<ShowGameMode>, OFF },
	{ cfgString, MO_BOOL, "LowCpuUsage", 0, &cfg<LowCpuUsage>, ON },
	{ cfgString, MO_BOOL, "ForceZoom", 0, &cfg<ForceZoom>, OFF },
	{ cfgString, MO_BOOL, "ShowResolution", 0, &cfg<ShowResolution>, OFF },
	{ cfgString, MO_BOOL, "FastInventoryItemSwap", 0, &cfg<FastInventoryItemSwap>, ON },
	{ cfgString, MO_BOOL, "OnScreenTextOutline", 0, &cfg<OnScreenTextOutline>, ON },
	{ cfgString, MO_BOOL, "FastSwapRightOnAlt", 0, &cfg<FastSwapRightOnAlt>, OFF },
	{ cfgString, MO_BOOL, "OriginalLightLevels", 0, &cfg<OriginalLightLevels>, ON },
	{ cfgString, MO_BOOL, "DropRares", 0, &cfg<DropRares>, ON },
	{ cfgString, MO_BOOL, "RelixToScrolls", 0, &cfg<RelixToScrolls>, OFF },
	{ cfgString, MO_BOOL, "DisplayMonstersLeft", 0, &cfg<DisplayMonstersLeft>, OFF },
	{ cfgString, MO_BOOL, "TransmuteAllowExtra", 0, &cfg<TransmuteAllowExtra>, OFF },
	{ cfgString, MO_BOOL, "TransmuteKeepItems", 0, &cfg<TransmuteKeepItems>, OFF },
	{ cfgString, MO_BOOL, "TransmuteAllowMultiple", 0, &cfg<TransmuteAllowMultiple>, OFF },
	{ cfgString, MO_BOOL, "TransmuteShowRecipeList", 0, &cfg<TransmuteShowRecipeList>, OFF },
	{ cfgString, MO_BOOL, "DuplicateSounds", 0, &cfg<DuplicateSounds>, ON },
	{ cfgString, MO_BOOL, "LimitCpuCore", 0, &cfg<LimitCpuCore>, OFF },
	{ cfgString, MO_BOOL, "ClassicInMainMenu", 0, &cfg<ClassicInMainMenu>, ON },
	{ cfgString, MO_BOOL, "ColiseumInMainMenu", 0, &cfg<ColiseumInMainMenu>, ON },
	{ cfgString, MO_BOOL, "ClassicSaveFolder", 0, &cfg<ClassicSaveFolder>, ON },
	{ cfgString, MO_BOOL, "ColiseumSaveFolder", 0, &cfg<ColiseumSaveFolder>, ON },
	{ cfgString, MO_BOOL, "SelectGameModeLater", 0, &cfg<SelectGameModeLater>, ON },

	{ cfgEnd }
};

// инлайнить имеет смысл только функции оптимизации оригинала и небольшие функции без вызовов других функци
// остальное надо помечать специальным образом в таблице (например адрес th2 со спец.значением)
// чтобы учитывать их при автоматической сверке
// ---- th2 ----------------------------------------
void GetUserParameters()
{
	int x = atoi(cfg<ScreenWidth>);
	int y = atoi(cfg<ScreenHeight>);
	if( x > 0 && y > 0 ){
		ScreenWidth = x;
		ScreenHeight = y;
	}else{
		ScreenWidth = GraphicsModes[DefaultVideoMode].Width;
		ScreenHeight = GraphicsModes[DefaultVideoMode].Height;
	}
	ScreenCenterUserShiftVertical = atoi(cfg<ScreenCenterUserShiftVertical>);
	LimitToRange(ScreenCenterUserShiftVertical, -2, +2);
	ChangeScreenRes();
	ExternalStorm = _stricmp(cfg<ExternalStorm>, "internal") != 0; // default external
	OriginalMenu = _stricmp(cfg<OriginalMenu>, "new") != 0; // default original
	IsNotMapOpen = _stricmp(cfg<IsNotMapOpen>, "open") != 0; // default map close
	DrawMode = !(_stricmp( cfg<DrawMode>, "surface" ) == 0); // default on
	FilteringMode = _stricmp( cfg<FilteringMode>, "off" ) == 0 ? D3DTEXF_NONE : D3DTEXF_LINEAR; // default on
	if( !IsFullScreen ){
		if( ! _stricmp( cfg<IsFullScreen>, "win" ) || ! _stricmp( cfg<IsFullScreen>, "winon" ) ){
			BigWin = true;
			IsFullScreen = true;
		}else if( !_stricmp( cfg<IsFullScreen>, "winoff" ) ){
			BigWin = true;
			IsFullScreen = false;
		}
	}
	if( DDraw ){
		BigWin = true;
		MapBlend = false;
	}
	MaxFPS = atoi( cfg<MaxFPS> );
	if(      MaxFPS ==   0 ){ MaxFPS =   60; }
	else if( MaxFPS >10000 ){ MaxFPS =10000; }
	else if( MaxFPS <    0 ){ MaxFPS =   20; }
	else{ MaxFPS = ( ( MaxFPS + 19 ) / 20 ) * 20; } // round up (eg 21 -> 40)

	int singleLog = atoi( cfg<SingleLog> );
	if( singleLog == 1 || singleLog == 2 ) SingleLog = singleLog;
	
	if (InventoryBackgroundColorBlue) {
		extern BackLightColor blColor;
		blColor = { BLC_BLUE, BLC_YELLOW, BLC_ORANGE, BLC_RED, BLC_MAGENTA };
	} else {
		InventoryBackgroundOldStyle = true;
		InventoryTurnOffRedColorItems = false;
	}
	FullscreenFX = atoi(cfg<FullscreenFX>);
	FloatingHPRegenThreshold = atoi(cfg<FloatingHPRegenThreshold>);
	FloatingManaRegenThreshold = atoi(cfg<FloatingManaRegenThreshold>);
	ReceivedDamageThreshold = atoi(cfg<ReceivedDamageThreshold>);
	MonsterReceivedDamageThreshold = atoi(cfg<MonsterReceivedDamageThreshold>);
	PlayerRunningSounds = atoi(cfg<PlayerRunningSounds>);
	#if CHEATS
    if( !DevelopMode ){
        SaveAlwaysEnabled = false;
        FreeSpell = false;
        IsGodMode = false;
        IsNotMapOpen = true;
    }
	#endif
	if( !_stricmp( cfg<LoadOnStart>, "off" ) ){
		LoadOnStart = false;
	}else if( !_stricmp( cfg<LoadOnStart>, "on" ) ){
		LoadOnStart = true;
		AutoLoadSlot = 7; // для совместимости с первой версией конфига
		LoadMulti = false;
	}else{
		int i = 0;
		LoadMulti = false;
		if( strlen(cfg<LoadOnStart>) > 0 ){
			if(      is( cfg<LoadOnStart>[0], 'm', 'M') ){ i = 1; LoadMulti = true; }
			else if( is( cfg<LoadOnStart>[0], 's', 'S') ){ i = 1; }
			AutoLoadSlot = atoi( &cfg<LoadOnStart>[i] );
		}
		LoadOnStart = AutoLoadSlot >= 0 && AutoLoadSlot <= MaxSaveSlots - 1;
	}
	if( cfg<NewGameOnStart>[0] ){
		DifficultyOnStart = cfg<DifficultyOnStart>[0] - '0';
		if( DifficultyOnStart < 0 ) DifficultyOnStart = 0;
		if( DifficultyOnStart > 2 ) DifficultyOnStart = 2;
	}
	if( LoadOnStart && (NewGameOnStart || LoadMulti) ){
		Difficulty = DifficultyOnStart;
	}
	SyncTH = false;
	VisualOnly = false;
	SingleLog = 0;
	if( IsSecondWin ){
		IsFullScreen = false;
		DrawMode = 1;
	}
	if( SingleLog ){
		SyncTH = false;
		VisualOnly = false;
	}
	SetFlipTable();
	void LoadMapConfig(); LoadMapConfig();
}

void SaveUserParameters()
{
	void SaveMapConfig(); SaveMapConfig();
}

//----- (th2) ---------------------------------------------------------------
void GetParamsFromConfig()
{
	if( !ConfigLoaded ){
		ConfigLoad( ConfigStructure, "config.ini" );
		GetUserParameters();// Два саба отвечающие за работу с конфигом. Один читает конфиг а другой разбирает прочитанное
		ConfigLoaded = true;
	}
}
