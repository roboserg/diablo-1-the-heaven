#ifndef _hell_h_
#define _hell_h_
#else
#error reinclude hell.h
#endif

enum STRING_TABLE
{
	STR_1_Paladin = 1,
	STR_2_Scout = 2,
	STR_3_Mage = 3,
	STR_4_Monk = 4,
	STR_5_Assassin = 5,
	STR_6_Gladiator = 6,
	STR_7_Sure_murder = 7,
	STR_8_Char_already_overwrite = 8,
	STR_15_Invalid_name = 15,
	STR_16_Unable_create_char = 16,
	STR_17_Murder_failed = 17,
	STR_18_Unable_read_save = 18,
	STR_19_gvdl = 19,
	STR_27_Insert_CD = 27,
	STR_28_Multi_char = 28,
	STR_29_Single_char = 29,
	STR_30_New_hero = 30,
	STR_31_New_hero_single = 31,
	STR_32_New_hero_multi = 32,
	STR_33_Max_players = 33,
	STR_34_Murder_single = 34,
	STR_35_Murder_multi = 35,
	STR_36_Create_Game = 36,
	STR_37_Unable_join = 37,
	STR_38_Unable_create = 38,
	STR_42_Create_new_game = 42,
	STR_43_Game_not_detected = 43,
	STR_44_Unable_init_connection = 44,
	STR_46_No_modem = 46,
	STR_47_Direct_link_not_detected = 47,
	STR_49_CreatedBy_Level_Class = 49,
	STR_50_Incompatible_connect_game_version = 50,
	STR_51_No_dialup_server = 51,
	STR_52_Call_num = 52,
	STR_53_Unable_init_device = 53,
	STR_54_Create_dial_desc = 54,
	STR_55_Enter_phone_desc = 55,
	STR_56_Dial_join_desc = 56,
	STR_57_Description = 57,
	STR_58_Enter_call_num = 58,
	STR_59_Description = 59,
	STR_63_Unable_locate_file = 63,
	STR_64_Unable_connect_dialup = 64,
	STR_65_No_dial_tone = 65,
	STR_66_Number_busy = 66,
	STR_67_Create_game_desc = 67,
	STR_68_Join_desc = 68,
	STR_69_Rogue_Sorc_only_in_full = 69,
	STR_70_Only_in_full_game = 70,
	STR_71_Game_num = 71,
	STR_73_Game_already_exists = 73,
	STR_74_Join_previous = 74,
	STR_75_Join_new = 75,
	STR_76_No_modem = 76,
	STR_77_Join_games = 77,
	STR_78_Not_ehough_memory = 78,
	STR_79_Coliseum_char = 79,
	STR_80_New_hero_coliseum = 80,
	STR_81_Murder_coliseum = 81,
	STR_82_Coliseum_game = 82,
	STR_83_New_hero_classic = 83,
	STR_84_Classic_char = 84,
	STR_85_Murder_classic = 85,

	STR_1003_Horror_difficulty = 1003,
	STR_1008_Enter_game_name = 1008,
	STR_1028_Invalid_game_name = 1028,
	STR_1029_Horror_difficulty_desc = 1029,
	STR_1032_Wrong_class = 1032,
	STR_1033_Creator = 1033,
	STR_1034_am_pm = 1034,
	STR_1036_Date_format = 1036,
	STR_1037_Time = 1037,
	STR_1039_Game_already_run = 1039,
	STR_1040_Unable_create_game = 1040,
	STR_1041_Too_low_difficulty = 1041,
	STR_1043_Un_auth_String_too_long = 1043,
	STR_1044_Un_auth_Unrecognized = 1044,
	STR_1045_Restricted_channel = 1045,

	STR_1046_Classes_Descriptions_Start = 1046,

	STR_1052_Warriors_Descriptions_Start = 1052,
	STR_1056_Archers_Descriptions_Start = 1056,
	STR_1060_Mages_Descriptions_Start = 1060,
	STR_1064_Monks_Descriptions_Start = 1064,
	STR_1068_Rogues_Descriptions_Start = 1068,
	STR_1072_Savages_Descriptions_Start = 1072,
	STR_1084_Druid_Description = 1084,

	STR_1076_Summoners_Descriptions_Start = 1076,
	STR_1079_Gladiators_Descriptions_Start = 1079,

	STR_1083_GameMode_Descriptions_Start = 1083,
};

enum MAIN_MENU_CONST
{
	MaxScrollCount_6 = 6,
};

enum MSG_META_MENU
{
	MSG_LIMIT                   = 1025,
	MSG_CURSOR                  = 1027,
	MSG_RESTRICTED              = 1028,

	MSG_BEGIN_FADE              = 2024,

	MSG_SELECT_HERO             = 3024,    // Choise save or new char
	MSG_SELECT_CLASS            = 3025,
	MSG_SELECT_SUBCLASS         = 3026,
	MSG_SELECT_SPECIALIZATION   = 3027,
	MSG_SELECT_TRAITS           = 3028,
	MSG_SELECT_TRAIT            = 3029,
	MSG_SELECT_GAME_MODE        = 3030,
	MSG_ENTER_NAME              = 3031,
	MSG_LOAD_OR_NEW             = 3032,
	MSG_NEW_GAME                = 3033,
	MSG_LOAD_GAME               = 3034,
	MSG_WTF                     = 3035,
	MSG_MURDER_HERO             = 3036,
	MSG_GAME_CHANGERS           = 3037,

	MSG_SELECT_DIALUP           = 3024,
	MSG_ENTER_DIALUP            = 3025,
	MSG_CRE8JOIN_DIALUP         = 3026,
};

union Rect_Text
{
	RECT rect;
	char text[60];
};

extern user_info *UserInfo_1;
extern int HasPentagram;
extern int isMultiGame; // weak
extern battle_info *CableBattleInfo;
extern client_info *CableClientInfo;
extern user_info *CableUserInfo;
extern int *CablePlayerIndex;
extern module_info *CableModuleInfo;
extern Provider_info *ProviderInfo_1;
extern client_info *ClientInfo_1;
extern int *PlayerId;
extern char *GameName_1;
extern DWORD ErrorCode;

int __fastcall SelectOk_2( HWND hdlg, char *errorString, char *dlgTitleText, int dlgHasPentagram );
int InitSelection();
TextWin* __cdecl AllocTextWin();
int __fastcall LoadDialogBackground( HWND hwnd, HWND parent, char *dlgTypeName, int mask, int flag, char *PicFileName, unsigned __int8 **dlgDataSurface, SIZE *size, int isMenuPalette );
void __cdecl CheckFadeTimer();
int __fastcall FadePalette( int fadeSteps );

void AttachMetaMenu();
void DetachMetaMenu();

void UiDestroy();
void UiInitialize();
int __stdcall UiCopyProtError( int *result_1 );
int __stdcall UiAppActivate(int);
int __stdcall UiSupportDialog(int);
int __stdcall UiCreditsDialog(int);
int __stdcall UiMainMenuDialog(char* Source, int* dialogResult, char a3, void( __stdcall* playSoundFile )(int), int a5);
int __stdcall UiSelHeroSingDialog(int (__stdcall *loadCharSlots)(void (__stdcall *allocFunc)(CharSaveInfo *)), int (__stdcall *createCharSlot)(CharSaveInfo *newSaveInfo), int (__stdcall *deleteCharSlot)(CharSaveInfo *saveInfo), int (__stdcall *getStartingStatsByClass)(unsigned int classId, unsigned __int16 *stats), int *singlePlayerSelection, char *heroName, int *difficulty, char enable5class, char enable6class);
int __stdcall UiSelHeroMultDialog(int (__stdcall *loadCharSlots)(void (__stdcall *allocFunc)(CharSaveInfo *)), int (__stdcall *createCharSlot)(CharSaveInfo *newSaveInfo), int (__stdcall *deleteCharSlot)(CharSaveInfo *saveInfo), int (__stdcall *getStartingStatsByClass)(unsigned int classId, unsigned __int16 *stats), int *multiPlayerSelection, char *heroName, char enable5class, char enable6class);
//int __stdcall UiSelHeroColiseumDialog(int(__stdcall* loadCharSlots)(void(__stdcall* allocFunc)(CharSaveInfo*)), int(__stdcall* createCharSlot)(CharSaveInfo* newSaveInfo), int(__stdcall* deleteCharSlot)(CharSaveInfo* saveInfo), int(__stdcall* getStartingStatsByClass)(unsigned int classId, unsigned __int16* stats), int* multiPlayerSelection, char* heroName, char enable5class, char enable6class);
//int __stdcall UiSelHeroClassicDialog(int (__stdcall *loadCharSlots)(void (__stdcall *allocFunc)(CharSaveInfo *)), int (__stdcall *createCharSlot)(CharSaveInfo *newSaveInfo), int (__stdcall *deleteCharSlot)(CharSaveInfo *saveInfo), int (__stdcall *getStartingStatsByClass)(unsigned int classId, unsigned __int16 *stats), int *classicSelection, char *heroName, int *difficulty, char enable5class, char enable6class);
int __stdcall UiProgressDialog( HWND parent, const char* lParam, BOOL buttonEnable, int( * getProgressPercent )( void ), int progressFps );
int __stdcall UiSelectGame( int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex );
int __stdcall UiSelectProvider( int unusedZero, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *provider );

// Battle unk_12
int __stdcall UiCategoryCallback(int a1, int a2, user_info* userInfo, int a4, int a5, int* charLevelCategory, int* categoryMask);

// BattleGetCursorLink
HCURSOR __stdcall UiGetDataCallback( int provider, int cursorType, CursorLink *cursorLink, unsigned int curSizeNeed, unsigned int *curSizeGet );

// BattleGetErrorString, возможно это авторизация для battle.net, так как в ней есть только три основных класса (даже без монаха)
int __stdcall UiAuthCallback( int isCallbackIf1, char *playerName, char *aPlayerDescription, char flag, char *gameStatString, LPSTR errorTextBuffer, int cchBufferMax );

// BattlePlaySound
int __stdcall UiSoundCallback(int a1, int a2, int a3);

// BattleErrorDialog
int __stdcall UiMessageBoxCallback(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

// BattleUpdateIcons
int __stdcall UiDrawDescCallback( int provider, int a2, LPCSTR lpString, int a4, int a5, char a6, time_t a7, BattleIcon *battleIcon_1 );

// BattleMakeCreateGameDialog
int __stdcall UiCreateGameCallback(Provider_info* providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex);

// BattleGetResource
int __stdcall UiArtCallback( int provider, unsigned int artId, PALETTEENTRY *palette, void *buffer, unsigned int bufferSize, int *width, int *height, int *bpp );

int __stdcall UiCreatePlayerDescription( CharSaveInfo *charSaveInfo, int gameId, char (&playerDescription)[256] );
int __stdcall UiCreateGameCriteria(client_info *clientInfo, char *gameCriteria);

void __fastcall DeleteWinData( TextWin *winData );
void __fastcall SetGameStatString( GameTemplate *gameTemplate, char *playerName, char *playerDescription, char (&gameStatString)[256], int size );
int __fastcall GetCharSaveInfo( char *playerDescription, CharSaveInfo *charSaveInfo );
