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

// Localization data and lookups (GameLanguage def, MonsterNameLoc, InitLocalization,
// LocalizedMonsterName) now live in src/Localization.cpp. Config.cpp only wires the
// "language" config option: the ConfigStructure row + the atoi parse in
// GetUserParameters(). GameLanguage itself is declared extern in vars.h.

int DefaultVideoMode = 0; // Don't change this !!! For high resolution use config.ini file editing
bool IsNotMapOpen;
int ScreenWidth = GraphicsModes[ DefaultVideoMode ].Width;// (rt) in the original the unnamed constant 640 is substituted everywhere
int ScreenHeight = GraphicsModes[ DefaultVideoMode ].Height;// (rt) in the original an unnamed constant is substituted everywhere
int HeightShift = 0;
int WorkingWidth = ScreenWidth + Screen_LeftBorder + Screen_RightBorder; // (rt) 768 in the original
int WorkingHeight = ScreenHeight + Screen_TopBorder + Screen_BottomBorder; // (rt) 656 in the original
int MaxScreenWidth, MaxScreenHeight, MaxWorkingWidth, MaxWorkingHeight;
int DrawMode = 1; // textured quad mode by default
_D3DTEXTUREFILTERTYPE FilteringMode = D3DTEXF_LINEAR; // bilinear filtration by default
bool SharpScaling = false; // (th4) sharp bilinear upscaling, see dx_utility.cpp SharpStretch
bool ShowItemLevelOnGround = false; // (th4) "(ilvl) Name" in the Alt ground labels, automap.cpp HighlightItemsNameOnMap
bool FsrUpscale = false;   // (th4) AMD FSR 1.0 upscaling, see dx_utility.cpp FsrPresent
int FsrSharpness = 2;      // (th4) FSR RCAS sharpening, 0..20 = 0.0..2.0 stops (lower = sharper)
bool IceAgeTreeSnow = true; // (th4) Ice Age: snow-covered, frosted trees in town / Ravenholm (was "IceTreeGlow", from when they glowed; renamed 2026-10-10), TownWater.cpp IceTrees_Active
bool IceAgeRiverMirror = false; // (th4) Ice Age: Tristram's river mirrors a winter sky by day (dx_utility.cpp ICE RIVER, IceRiver_Active); off (default) = the old frozen river
bool GrottoTrip = true; // (th4) Mossy Grotto forced chromatic aberration, dx_utility.cpp GrottoTripCA; off = the normal ChromaticAberration setting applies there
int ChromaticAberration = 0; // (th4) post effect, 0..10 px at the corners, 0 = off (dx_utility.cpp PostFxPresent)
int HeatHaze = 0;            // (th4) post effect, shimmer above burning lava, 0..10, 0 = off
int WaterShimmer = 0;        // (th4) post effect, ripples on cave-map water and the town river (TownWater.cpp), 0..10
bool WaterReflection = true; // (th4) heroes mirrored in the town river, TownWater.cpp (8-bit, needs no post effect)
int TiltShift = 0;           // (th4) post effect, miniature blur at the top/bottom of the world, 0..10
int Shockwave = 0;           // (th4) post effect, distortion rings from explosions and novas, 0..10
int CrtEffect = 0;           // (th4) post effect, CRT scanlines and phosphor mask, 0..10
int Perspective = 0;         // (th4) pseudo-perspective world view, 0..10, 0 = off (Draw.cpp Perspective_WarpWorld)
int FilmGrain = 0;           // (th4) post effect, 0..10, 0 = off
int IceFrost = 7;            // (th4) post effect, Ice Age frost creeping in from the screen edges, 0..10, DEFAULT ON (dx_utility.cpp Frost_Present)
int RetroMode = 0;           // (th4) post effect, whole-screen old-machine look, 0 = off, 1..19 = dx_utility.cpp RetroModes[] row
int RetroPixel = 0;          // (th4) RetroMode pixel size in game px, 0 = the mode's own, 1..8
int SpellTrails = 0;         // (th4) post effect, afterimages behind missiles and spells, 0..10 = trail length; DISABLED FOR NOW (row commented out), dx_utility.cpp ATMOSPHERE
bool LevelUpGlow = false;    // (th4) post effect, golden ring + pillar of light on a level-up
int TreeSway = 4;            // (th4) trees sway in the wind, 0..10, DEFAULT ON = 4 (dx_utility.cpp TreeSway_Present, TownWater.cpp BuildSwayLayers)
int SunRays = 2;             // (th4) sun rays through Tristram's trees on sunny days, 0..10, DEFAULT ON = 2 (Mordor's pick; dx_utility.cpp SunRays_Present)
int CloudShadows = 9;        // (th4) post effect, cloud shadows over the town by day, 0..10, DEFAULT ON = 9 (dx_utility.cpp Clouds_Present)
int WetGround = 5;           // (th4) wet town and puddles during a storm night and the day after, 0..10, DEFAULT ON = 5 (TownWater.cpp PUDDLES, dx_utility.cpp Wet_Present)
int ColdBreath = 5;          // (th4) breath vapour in the cold, 0..10, DEFAULT ON = 5 (Breath.cpp, dx_utility.cpp Breath_Present)
int GrassBlades = 10;        // (th4) blades of grass swaying in the wind, town + Ravenholm, 0..10 = how tall, DEFAULT ON = 10 (Mordor's pick; TownWater.cpp GRASS, dx_utility.cpp Grass_Present)
int GrassResolution = 2;     // (th4) where the grass blades are drawn: 0 window res, 1 game res, 2 half game res (DEFAULT) -- dx_utility.cpp Grass_Present

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
bool AutomapShadow = true; // (th4) dark outline behind the automap's map lines, automap.cpp AUTOMAP SHADOW
bool Lovecraft = false;   // Gribnik
bool MapBlend = true;
bool showCombatText = true;
int FloatingHPRegenThreshold = 0;
int FloatingManaRegenThreshold = 0;
bool ShowHPRegen = true;
bool ShowMPRegen = true;
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
int FullscreenFX = 0; // tumbler for fullscreen shader (6, the old FXAA, is retired: see Antialiasing)
int GroundMist = 5;   // (th4) town ground mist strength, fixed at 5 -- config row disabled for now (synced MP weather, Weather.cpp)
int Antialiasing = 0; // (th4) 0 off, 1 FXAA, 2 SMAA; a separate pass, dx_utility.cpp AntialiasPresent
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
bool total_kills = false;	// (th4) show lifetime kill count tooltip on the char panel name
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
bool DynamicZoom = false;	// (th4) mouse-wheel zoom of the world (DynamicZoom.cpp)
bool ShowResolution = false;
bool FastInventoryItemSwap = true;
bool OnScreenTextOutline = true;
bool FastSwapRightOnAlt = false;
bool OriginalLightLevels = true;
bool DropRares = true;
bool RelixToScrolls = false;
bool DisplayMonstersLeft = false;

bool ShowMinionBars = false;

bool Display_Player_HP_bars = false;
bool Display_Monster_HP_bars = false;
bool Show_Reflect_Layers = false;

bool TransmuteAllowExtra = false;
bool TransmuteKeepItems = false;
bool TransmuteAllowMultiple = false;
bool TransmuteShowRecipeList = true;
bool DuplicateSounds = true;
bool LimitCpuCore = false;
bool ClassicInMainMenu = true;
bool OutbreakInMainMenu = true;
bool ColiseumInMainMenu = true;
bool ClassicSaveFolder = true;
bool OutbreakSaveFolder = true;
bool DarkCampaignSaveFolder = true; // (th4)
bool ColiseumSaveFolder = true;
bool SelectGameModeLater = true;

// (rt) cfg<ParamName> is now used instead of manually creating a variable for each parameter's string buffer
// (rt) the ability to set a default value was also added, for bool parameters only for now
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
	// (th4) GroundMist row disabled FOR NOW: mist nights are part of the weather roll shared by all
	// players (Weather.cpp); a per-player switch would desync it. Restore the row and the parse below.
	//{ cfgString, MO_INT, "GroundMist", "Mist on the ground of the town at night: off, on (= 5), or 1 ... 10", &cfg<GroundMist> },
	{ cfgString, MO_INT, "Antialiasing", "Edge smoothing: off, FXAA (fast) or SMAA (best quality). Works together with FullscreenFX, FSR and the post effects", &cfg<Antialiasing> },
	{ cfgString, MO_BOOL, "FSR", "AMD FSR 1.0 upscaling: smoother edges and sharper detail when the game resolution is smaller than your screen or window. Needs filter ON. Overrides SharpScaling", &cfg<FsrUpscale>, OFF },
	{ cfgString, MO_INT, "FSRSharpness", "FSR sharpening, 0 = sharpest ... 20 = softest (default 2)", &cfg<FsrSharpness> },
	{ cfgString, MO_INT, "Perspective", "Tilted ground view similar to Diablo II's perspective mode: off, on (= 5), or 1 ... 10", &cfg<Perspective> },
	{ cfgString, MO_INT, "WaterShimmer", "Ripples on the water of cave maps and the town river: off, on (= 5), or 1 ... 10", &cfg<WaterShimmer> },
	{ cfgString, MO_BOOL, "WaterReflection", "Heroes are mirrored in the town river", &cfg<WaterReflection>, ON },
	{ cfgString, MO_INT, "TiltShift", "Miniature look: soft blur at the top and bottom of the world view: off, on (= 5), or 1 ... 10", &cfg<TiltShift> },
	{ cfgString, MO_INT, "Shockwave", "Distortion ring from explosions and novas: off, on (= 5), or 1 ... 10", &cfg<Shockwave> },
	{ cfgString, MO_INT, "CRT", "Old monitor look: scanlines and phosphor mask: off, on (= 5), or 1 ... 10", &cfg<CrtEffect> },
	{ cfgString, MO_INT, "HeatHaze", "Heat shimmer above burning lava: off, on (= 5), or 1 ... 10", &cfg<HeatHaze> },
	{ cfgString, MO_INT, "ChromaticAberration", "Colour fringing toward the screen corners: off, on (= 3), or 1 ... 10", &cfg<ChromaticAberration> },
	{ cfgString, MO_BOOL, "GrottoTrip", "Strong colour fringing in the Mossy Grotto that grows near the Fungal Roots; off = the normal ChromaticAberration setting is used there", &cfg<GrottoTrip>, ON },
	{ cfgString, MO_INT, "FilmGrain", "Animated film grain: off, on (= 3), or 1 ... 10", &cfg<FilmGrain> },
	{ cfgString, MO_INT, "IceFrost", "Ice Age, Glacial Caverns and Frozen Loch: frost creeps in from the screen edges while you stand still and melts when you move or fight: off, on (= 7), or 1 ... 10 (how far it can reach)", &cfg<IceFrost> },
	{ cfgString, MO_BOOL, "IceAgeTreeSnow", "Ice Age: the trees in Tristram and Ravenholm are covered in snow and hoarfrost", &cfg<IceAgeTreeSnow>, ON },
	{ cfgString, MO_BOOL, "IceAgeRiverMirror", "Ice Age: by day the river in Tristram mirrors a blue winter sky with drifting clouds; off = the old frozen river", &cfg<IceAgeRiverMirror>, OFF },
	{ cfgString, MO_INT, "RetroMode", "Old machine look for the whole screen: off, or 1 Game Boy, 2 Game Boy Pocket, 3 Virtual Boy, 4 CGA, 5 EGA, 6 C64, 7 NES, 8 amber monitor, 9 green monitor, 10 sepia, 11 1-bit Mac, 12 newspaper, 13 pixelate, 14 PS1, 15 PC-98, 16 ZX Spectrum, 17 ASCII, 18 Teletext", &cfg<RetroMode> },
	// (th4) SpellTrails disabled for now (code kept in dx_utility.cpp); restore the row and its parse line to bring it back
	//{ cfgString, MO_INT, "SpellTrails", "Spells and missiles leave fading afterimages: off, on (= 5), or 1 ... 10 (how long they last)", &cfg<SpellTrails> },
	{ cfgString, MO_BOOL, "LevelUpGlow", "A golden ring bursts out from the hero and a pillar of light rises on a level-up", &cfg<LevelUpGlow>, OFF },
	{ cfgString, MO_INT, "TreeSway", "Trees sway in the wind: on storm nights, in windy spells by day, always in the Ice Age and in Ravenholm: off, on (= 4), or 1 ... 10", &cfg<TreeSway> },
	{ cfgString, MO_INT, "SunRays", "Shafts of sunlight through Tristram's trees on sunny days: off, on (= 2), or 1 ... 10", &cfg<SunRays> },
	{ cfgString, MO_INT, "CloudShadows", "Shadows of clouds drift over the town by day (not under Ice Age): off, on (= 9), or 1 ... 10", &cfg<CloudShadows> },
	{ cfgString, MO_INT, "WetGround", "Tristram gets wet in the rain of storm nights and dries out over the next day: darker ground, puddles with reflections and raindrop rings: off, on (= 5), or 1 ... 10", &cfg<WetGround> },
	{ cfgString, MO_INT, "GrassBlades", "Blades of grass grow on the grass of Tristram and Ravenholm and sway when the wind blows (not in Classic, not under Ice Age): off, on (= 10), or 1 ... 10 (how tall)", &cfg<GrassBlades> },
	{ cfgString, MO_INT, "GrassResolution", "Grass blades are drawn at: 0 = the window's resolution (the sharpest, the heaviest for the graphics card), 1 = the game's resolution, 2 = half the game's resolution (default; the fastest, for weak or built-in graphics)", &cfg<GrassResolution> },
	{ cfgString, MO_INT, "ColdBreath", "Breath vapour in the cold (the Ice Age, and the Glacial Caverns and Frozen Loch in every game): heroes, townsfolk, cows and living monsters breathe out puffs of mist: off, on (= 5), or 1 ... 10", &cfg<ColdBreath> },
	{ cfgString, MO_INT, "RetroPixel", "Pixel size for RetroMode: 0 = the mode's own size, or 1 ... 8", &cfg<RetroPixel> },
	{ cfgString, MO_BOOL, "ShowItemLevelOnGround", "Show the required character level in brackets before the name of unidentified magic items on the ground (Alt highlight); not shown for unique and set items", &cfg<ShowItemLevelOnGround>, OFF },
	{ cfgString, MO_BOOL, "SharpScaling", "Sharper picture when the game resolution is smaller than your screen or window: pixels are enlarged crisply first, then smoothed only at the seams. Needs filter ON. Most visible at low game resolutions", &cfg<SharpScaling>, OFF },
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
	{ cfgString, MO_INT, "language", "Interface language (0 = English, 1 = Russian, 2 = Indonesian, 3 = Portuguese, 4 = Spanish, 5 = Italian, 6 = French, 7 = Polish, 8 = Chinese, 9 = German, 10 = Turkish, 11 = Korean, 12 = Danish, 13 = Bulgarian, 14 = Dutch, 15 = Ukrainian, 16 = Japanese, 17 = Arabic, 18 = Hindi, 19 = Swahili)", &cfg<GameLanguage> },
	{ cfgString, 0, "centeredpanels", 0, &cfg<PanelsOnCenter>, OFF },
	{ cfgString, MO_BOOL, "mapitem", 0, &cfg<MapItem>, ON },
	{ cfgString, MO_BOOL, "lovecraft", 0, &cfg<Lovecraft>, OFF },      // Gribnik
	{ cfgString, MO_BOOL, "mapblend", 0, &cfg<MapBlend>, ON },
	{ cfgString, MO_BOOL, "AutomapShadow", "Dark outline behind the automap lines, so the map stays readable on bright floors", &cfg<AutomapShadow>, ON },
	{ cfgString, MO_BOOL, "showCombatText", 0, &cfg<showCombatText>, ON },
	{ cfgString, MO_INT, "floatingHPRegenThreshold", 0, &cfg<FloatingHPRegenThreshold> },
	{ cfgString, MO_INT, "floatingManaRegenThreshold", 0, &cfg<FloatingManaRegenThreshold> },
	{ cfgString, MO_BOOL, "showHPRegen", 0, &cfg<ShowHPRegen>, ON },
	{ cfgString, MO_BOOL, "showMPRegen", 0, &cfg<ShowMPRegen>, ON },
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
	{ cfgString, MO_BOOL, "total_kills", 0, &cfg<total_kills>, OFF },
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
	{ cfgString, MO_BOOL, "DynamicZoom", 0, &cfg<DynamicZoom>, OFF },	// (th4) DynamicZoom.cpp
	{ cfgString, MO_BOOL, "ShowResolution", 0, &cfg<ShowResolution>, OFF },
	{ cfgString, MO_BOOL, "FastInventoryItemSwap", 0, &cfg<FastInventoryItemSwap>, ON },
	{ cfgString, MO_BOOL, "OnScreenTextOutline", 0, &cfg<OnScreenTextOutline>, ON },
	{ cfgString, MO_BOOL, "FastSwapRightOnAlt", 0, &cfg<FastSwapRightOnAlt>, OFF },
	{ cfgString, MO_BOOL, "OriginalLightLevels", 0, &cfg<OriginalLightLevels>, ON },
	{ cfgString, MO_BOOL, "PixelLight", "Per-pixel lighting", &cfg<PixelLight>, OFF },
	{ cfgString, MO_BOOL, "MultiThreadPixelLight", "Multi-threaded per-pixel lighting (requires PixelLight ON)", &cfg<MultiThreadPixelLight>, OFF },
//	{ cfgString, MO_BOOL, "EnrichedLight", "Deeper color range and smoother lighting, working in conjunction with PixelLight ON", &cfg<g_A8L8Lighting>, OFF },
	{ cfgString, MO_BOOL, "EnrichedLightEnhanced", "Smoother static and dynamic lighting from per-pixel light field (Recommends MultiThreadPixelLight ON, fullscreen ON for best performance)", &cfg<A8L8PixelField>, OFF },
//	{ cfgString, MO_INT, "EnrichedLightEnhancedGamma", "Brightness vs distance falloff factor in percent of 0 to 100, 100 is quadratic falloff (default), 50 is half blend, 0 is linear falloff (requires EnrichedLightEnhanced ON or EnrichedLightUltra ON, affects non-Crypt and non-Abyss type dungeon lighting)", &cfg<A8L8GammaBlendPct> },
	{ cfgString, MO_BOOL, "EnrichedLightUltra", "Smoother per-pixel lighting with proper transparency and colored lighting support (Recommends MultiThreadPixelLight ON, fullscreen ON for best performance)", &cfg<g_ARGBLighting>, OFF },
//	{ cfgString, MO_INT, "EnrichedLightUltraAlpha", "Translucency (0-255) of sprites occluded by walls in EnrichedLightUltra mode; 0 disables the overlay (default 128; requires EnrichedLightUltra ON)", &cfg<EnrichedLightUltraAlpha> },
//	{ cfgString, MO_BOOL, "EnrichedLightUltraAlphaContrast", "Contrast-weighted per-pixel translucency for wall-occluded sprites (requires EnrichedLightUltraAlpha > 0), ON multiplies the alpha by the sprite/wall color contrast, fading dark-on-dark and popping bright sprite over dark wall, occluded sprites are harder to see through (default ON)", &cfg<g_ARGBAlphaContrast>, ON },
//	{ cfgString, MO_BOOL, "EnrichedLightUltraAlphaRim", "Outline emphasis for wall-occluded sprites, the sprite's silhouette edge keeps the full EnrichedLightUltraAlpha even where color contrast is low (requires EnrichedLightUltraAlphaContrast ON), this option costs performance (default ON)", &cfg<g_ARGBAlphaRim>, ON },
	{ cfgString, MO_BOOL, "DropRares", 0, &cfg<DropRares>, ON },
	{ cfgString, MO_BOOL, "RelixToScrolls", 0, &cfg<RelixToScrolls>, OFF },
	{ cfgString, MO_BOOL, "DisplayMonstersLeft", 0, &cfg<DisplayMonstersLeft>, OFF },

	{ cfgString, MO_BOOL, "ShowMinionBars", "Draw a HP bar + name on the left border for each active minion...", &cfg<ShowMinionBars>, OFF },

	{ cfgString, MO_BOOL, "Display_Player_HP_bars", "Draw a small bluish HP bar above each player and their minions.", &cfg<Display_Player_HP_bars>, OFF },
	{ cfgString, MO_BOOL, "Display_Monster_HP_bars", "Draw a small red HP bar above each monster.", &cfg<Display_Monster_HP_bars>, OFF },

	{ cfgString, MO_BOOL, "Show_Reflect_Layers", "Show the number of remaining Reflect layers on top of the Reflect icon above the player.", &cfg<Show_Reflect_Layers>, OFF },

	{ cfgString, MO_BOOL, "TransmuteAllowExtra", 0, &cfg<TransmuteAllowExtra>, OFF },
	{ cfgString, MO_BOOL, "TransmuteKeepItems", 0, &cfg<TransmuteKeepItems>, OFF },
	{ cfgString, MO_BOOL, "TransmuteAllowMultiple", 0, &cfg<TransmuteAllowMultiple>, OFF },
	{ cfgString, MO_BOOL, "TransmuteShowRecipeList", 0, &cfg<TransmuteShowRecipeList>, OFF },
	{ cfgString, MO_BOOL, "DuplicateSounds", 0, &cfg<DuplicateSounds>, ON },
	{ cfgString, MO_BOOL, "LimitCpuCore", 0, &cfg<LimitCpuCore>, OFF },
	{ cfgString, MO_BOOL, "ClassicInMainMenu", 0, &cfg<ClassicInMainMenu>, ON },
	{ cfgString, MO_BOOL, "OutbreakInMainMenu", 0, &cfg<OutbreakInMainMenu>, ON },
	{ cfgString, MO_BOOL, "ColiseumInMainMenu", 0, &cfg<ColiseumInMainMenu>, ON },
	{ cfgString, MO_BOOL, "ClassicSaveFolder", 0, &cfg<ClassicSaveFolder>, ON },
	{ cfgString, MO_BOOL, "OutbreakSaveFolder", 0, &cfg<OutbreakSaveFolder>, ON },
	{ cfgString, MO_BOOL, "DarkCampaignSaveFolder", 0, &cfg<DarkCampaignSaveFolder>, ON }, // (th4)
	{ cfgString, MO_BOOL, "ColiseumSaveFolder", 0, &cfg<ColiseumSaveFolder>, ON },
	{ cfgString, MO_BOOL, "SelectGameModeLater", 0, &cfg<SelectGameModeLater>, ON },

	{ cfgEnd }
};

// (rt) only original-optimization functions and small functions that call nothing else are worth inlining
// (rt) the rest must be marked specially in the table (e.g. a th2 address with a special value)
// (rt) so they are accounted for in the automatic comparison
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
	// (th4) Antialiasing: off / fxaa / smaa, or 0 / 1 / 2; "on" means the best (SMAA). An absent
	// key reads as "" (ZeroCfg) = off.
	{
		const char* v = cfg<Antialiasing>;
		if( !v || !*v || !_stricmp( v, "off" ) || !strcmp( v, "0" ) ) Antialiasing = 0;
		else if( !_stricmp( v, "fxaa" ) || !strcmp( v, "1" ) ) Antialiasing = 1;
		else Antialiasing = 2;	// "smaa", "2", "on"
	}
	// (th4) FullscreenFX 6 was FXAA as a scene shader; it is now Antialiasing 1, which combines
	// with the other FullscreenFX shaders. An old config keeps its FXAA (unless Antialiasing is set).
	if( FullscreenFX == 6 ){
		FullscreenFX = 0;
		if( !Antialiasing ) Antialiasing = 1;
	}
	FloatingHPRegenThreshold = atoi(cfg<FloatingHPRegenThreshold>);
	FloatingManaRegenThreshold = atoi(cfg<FloatingManaRegenThreshold>);
	// (th4) FSR sharpening: MO_INT options are only converted here (IniFile.cpp stores text);
	// an absent key reads as "" (ZeroCfg), so keep the default 2 instead of atoi("") = 0.
	if( *(char*)cfg<FsrSharpness> ) FsrSharpness = atoi(cfg<FsrSharpness>);
	// (th4) Post-effect strengths are numbers 0..10, but players naturally write on/off like
	// every other switch in config.ini, and atoi("on") is 0. So: off / absent -> 0, on -> a
	// medium default, a number -> that number (clamped 0..10 where it is used, PostFxPresent).
	auto fxLevel = []( const char* v, int onValue ) -> int {
		if( !v || !*v || !_stricmp( v, "off" ) ) return 0;
		if( !_stricmp( v, "on" ) ) return onValue;
		return atoi( v );
	};
	HeatHaze            = fxLevel( cfg<HeatHaze>, 5 );
	// (th4) GroundMist parse disabled with its row (see the row): stays 5.
	//{ const char* v = cfg<GroundMist>; GroundMist = ( !v || !*v ) ? 5 : fxLevel( v, 5 ); }
	//if( GroundMist > 10 ) GroundMist = 10; else if( GroundMist < 0 ) GroundMist = 0;
	WaterShimmer        = fxLevel( cfg<WaterShimmer>, 5 );
	TiltShift           = fxLevel( cfg<TiltShift>, 5 );
	Shockwave           = fxLevel( cfg<Shockwave>, 5 );
	CrtEffect           = fxLevel( cfg<CrtEffect>, 5 );
	Perspective         = fxLevel( cfg<Perspective>, 5 );
	ChromaticAberration = fxLevel( cfg<ChromaticAberration>, 3 );
	FilmGrain           = fxLevel( cfg<FilmGrain>, 3 );
	RetroMode           = fxLevel( cfg<RetroMode>, 1 );	// (th4) default off; clamped in dx_utility.cpp Retro_Present
	RetroPixel          = atoi( cfg<RetroPixel> );	// absent = 0 = the mode's own size
	//SpellTrails       = fxLevel( cfg<SpellTrails>, 5 );	// (th4) disabled with its row: stays 0
	{ const char* v = cfg<TreeSway>; TreeSway = ( !v || !*v ) ? 4 : fxLevel( v, 4 ); }	// (th4) on by default: an absent key and "on" are 4 (Mordor's choice)
	{ const char* v = cfg<SunRays>; SunRays = ( !v || !*v ) ? 2 : fxLevel( v, 2 ); }	// (th4) on by default: an absent key and "on" are 2 (Mordor's pick)
	{ const char* v = cfg<CloudShadows>; CloudShadows = ( !v || !*v ) ? 9 : fxLevel( v, 9 ); }	// (th4) on by default: an absent key and "on" are 9 (Mordor's choice)
	{ const char* v = cfg<IceFrost>; IceFrost = ( !v || !*v ) ? 7 : fxLevel( v, 7 ); }	// (th4) on by default: an absent key keeps 7 (Mordor's tested look)
	{ const char* v = cfg<WetGround>; WetGround = ( !v || !*v ) ? 5 : fxLevel( v, 5 ); }	// (th4) on by default: an absent key and "on" are 5
	{ const char* v = cfg<ColdBreath>; ColdBreath = ( !v || !*v ) ? 5 : fxLevel( v, 5 ); }	// (th4) on by default: an absent key and "on" are 5
	{ const char* v = cfg<GrassBlades>; GrassBlades = ( !v || !*v ) ? 10 : fxLevel( v, 10 ); }	// (th4) on by default: an absent key and "on" are 10 (Mordor's pick)
	{ const char* v = cfg<GrassResolution>; GrassResolution = ( !v || !*v ) ? 2 : atoi( v ); if( GrassResolution < 0 || GrassResolution > 2 ) GrassResolution = 2; }	// (th4) absent = 2, half the game's resolution
	ReceivedDamageThreshold = atoi(cfg<ReceivedDamageThreshold>);
	MonsterReceivedDamageThreshold = atoi(cfg<MonsterReceivedDamageThreshold>);
	PlayerRunningSounds = atoi(cfg<PlayerRunningSounds>);
	GameLanguage = atoi(cfg<GameLanguage>);
	if( GameLanguage < 0 || GameLanguage >= LANG_COUNT ) GameLanguage = LANG_EN;
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
		AutoLoadSlot = 7; // (rt) for compatibility with the first config version
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
	// EnrichedLightEnhancedGamma: MO_INT values are loaded as raw strings by
	// ConfigLoad — the typed variable must be parsed here like the other int
	// options.  Only override the default (100) when the key is actually
	// present in config.ini: an absent key leaves the string empty and
	// atoi("") == 0, which would silently turn the falloff linear.
	if( cfg<A8L8GammaBlendPct>[0] ){
		A8L8GammaBlendPct = atoi( cfg<A8L8GammaBlendPct> );
	}
	LimitToRange( A8L8GammaBlendPct, 0, 100 );
	// A8L8Lighting requires per-pixel lighting; force it on regardless of config.ini
	if (g_A8L8Lighting) {
		PixelLight = true;
	}
	// Enhanced A8L8 lighting with per-pixel light field requires both PPL and A8L8 lighting; force them on regardless of config.ini
	if (A8L8PixelField) {
		PixelLight = true;
		g_A8L8Lighting = true;
	}
	// Ultra two-texture mode (A8R8G8B8 + L8) builds on the enhanced A8L8 field; force the whole chain on regardless of config.ini
	if (g_ARGBLighting) {
		PixelLight = true;
		g_A8L8Lighting = true;
		A8L8PixelField = true;
	}
	// EnrichedLightUltraAlpha: MO_INT values are loaded as raw strings — parse
	// like A8L8GammaBlendPct above (empty-string guard so an absent key keeps
	// the dataseg.cpp default of 128 = overlay on at half translucency;
	// 0 explicitly turns the feature off).
	if( cfg<EnrichedLightUltraAlpha>[0] ){
		EnrichedLightUltraAlpha = atoi( cfg<EnrichedLightUltraAlpha> );
	}
	LimitToRange( EnrichedLightUltraAlpha, 0, 255 );
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
		GetUserParameters();// (rt) Two subs handle the config: one reads it, the other parses what was read
		ConfigLoaded = true;
	}
}
