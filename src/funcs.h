#ifndef _funcs_h_
#define _funcs_h_
#else
#error reinclude funcs.h
#endif

char* __fastcall GetErrorText( DWORD dwMessageId	);//	Error		
char* __fastcall FormatDDError( HRESULT hr	);//	Error		
char* __fastcall FormatDSError( HRESULT hr	);//	Error		
char*  GetErrorTextForLastError( );//	Error		
__declspec(noreturn) void  TerminateWithError( LPCSTR a1, ...	);//	Error		
void __fastcall ShowErrorMessageBox( LPCSTR a1, va_list arglist	);//	Error		
void  ErrorExitGame( );//	Error		
void  ErrorBoxf( LPCSTR a1, ...	);//	Error		
void __fastcall ShowDirectDrawError( DWORD _this, int a2, char *arglist	);//	Error		
void __fastcall DirectSoundError( DWORD a1, int a2, char *arglist	);//	Error		
void DirectSoundErrorCheck();
void __fastcall CenterWindow( HWND hwnd	);//	Error		
void __fastcall ErrorIn_CPP_File( int dialogid, DWORD dwMessageId, char *pCppFile, int line	);//	Error		
int __stdcall DialogFunc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam	);//	Error		
void __fastcall SetErrorDialogTitle( HWND hDlg, LPCSTR lpString	);//	Error		
void __fastcall FileErrDlg( LPARAM dwInitParam	);//	Error		
void __fastcall DiskFreeDlg( LPARAM dwInitParam	);//	Error		
bool  InsertCDDlg( );//	Error		
void  ResetAutoMap( );//	AutoMap		
void  InitDungeonAutomapData( );//	AutoMap		
void  ShiftAutomapUp( );//	AutoMap		
void  ShiftAutomapDown( );//	AutoMap		
void  ShiftAutomapLeft( );//	AutoMap		
void  ShiftAutomapRight( );//	AutoMap		
void  ZoomAutomapPlus( );//	AutoMap		
void  ZoomAutomapMinus( );//	AutoMap
void CleanAutoMapGraphics();
void  DrawAutoMap( );//	AutoMap		
void __fastcall AutomapDrawStaticObject( int X, int Y, int a3	);//	AutoMap		
void  AutomapDrawItems( );//	AutoMap		
void __fastcall AutomapDrawOneItem( int X, int Y, uchar a3	);//	AutoMap		
void __fastcall AutomapDrawOnePlayer( int PlayerId	);//	AutoMap		void __usercall AutomapDrawOnePlayer(int playerIndex@<eax>)
void AutomapDrawOneMonster( int monsterIndex, int color );
ushort __fastcall GetStaticObjectFlags( int x, int y, bool getAutomapCellVisibilityFlag	);//	AutoMap		
void  DrawLevelInfo( );//	AutoMap		
void __fastcall SetAutomapView( int row112, int col112	);//	AutoMap		
void  SyncAutomap( );//	AutoMap		
void  SaveScreenShot( );//	Screenshot		
bool __fastcall WriteScreenShotHeader( HANDLE hFile, int width, int height	);//	Screenshot		
bool __fastcall WritePaletteToPCX( HANDLE hFile, PALETTEENTRY* a2	);//	Screenshot		
bool __fastcall PCXWriteRow( HANDLE hFile, int width, int height, long pitch, unsigned char* pixels	);//	Screenshot		
int __fastcall PCXConvertRow( unsigned char* sourcePixels, unsigned char* destBuffer, long rowWidth	);//	Screenshot		
HANDLE __fastcall CreateScreenShot( char (&pOutputFilename)[MAX_PATH]	);//	Screenshot		
int __fastcall MakeScreenRedWhileSavingScreenshot( PALETTEENTRY* screenPalette	);//	Screenshot		
uint __fastcall DecodeFile( uchar* data, uint size, char* key	);//	CodeSave		
void __fastcall InitializeKeys( bool encrypt, char* key	);//	CodeSave		
void __fastcall EncodeFile( uchar* data, uint sizeSrc, uint sizeDst, char* password	);//	CodeSave		
void __fastcall DrawSpellIcon( int surfaceX, int surfaceY, char* iconCel, int iconIndex, int size, int spellType = 0	);//	Panel		
void __fastcall DrawSpellColor( int spellType	);//	Panel		
void  DrawSpellButton( );//	Panel		
void  DrawSpellsPanel( );//	Panel		
void  SelectCurrentSpell( );//	Panel		
void __fastcall AssignNewSpellToKey( int a1	);//	Panel		
void __fastcall AssignNewSpellToKeyExpanded( int key );//	Panel		
void __fastcall SelectSpellByHotKey( int a1	);//	Panel		
int __fastcall SelectSpellByHotKeyExpanded( int keyOffset );//	Panel		
void __fastcall DrawLetter( int aSurfaceOffset, int letterID, char fontColor, bool outline = false	);//	Panel		
void __fastcall InfoPanel_AddLine( const char *str, int centered, const char* str2 = 0	);//	Panel		
void  InfoPanel_ClearBody( );//	Panel		
int __fastcall CopyFromMainPanelToWorkingSurface( int SrcX, int SrcY, int Width, int Height, int DstX, int DstY	);//	Panel		
void __fastcall DrawEmptyGlobeBottom( uchar *aMap88xNPtr, int aStartRow, int aEndRow, int aStartOffset, int aStartY	);//	Panel		
void __fastcall PutWithAlpha( uchar *aSrcSurface, int a2, int aSrcOffset, uchar *aDstSurface, int aDstOffset, int a6	);//	Panel		
void  DrawLifeGlobeTop( );//	Panel		
void  DrawLifeGlobeBottom( );//	Panel		
void  DrawManaGlobeTop( );//	Panel		
void  RecalcLifeManaGlobes( );//	Panel		
void  DrawManaGlobeBottom( );//	Panel		
void  MayBeViewInit( );//	Panel		
void  InitPanelButtons();
void  FreePanelButtons();
void  DrawMainPanel( );//	Panel		
void  DrawMainButtons( );//	Panel		
void  SpellPanel_SetupCursorPos( );//	Panel		
void  CheckMainPanelRegion( );//	Panel		
void __fastcall ButtonPress( Button& b	);//	Panel		
void  Check_MenuORSendMessage( );//	Panel		
void  ToggleAutomap( );//	Panel		
void  PopUpHelpOnButton( );//	Panel		
void  MainPanelButtonControl( );//	Panel		
void  FreePanels( );//	Panel		
int __fastcall CheckLetterWidth( char *StrPtr	);//	Panel		
void  DrawInfoPanel( );//	Panel		
void  InfoPanel_Draw( );//	Panel		
void __fastcall InfoPanel_DrawLine( int line, char *text, bool centered, int infoLinesCount	);//	Panel		
int __fastcall DrawLevelInfoText( int xPos, int yPos, const char *stringPtr, int fontColor, bool outline = false	);//	Panel
void  CharPanel_Draw();//	Panel
void __fastcall DrawText( int xPos, int yPos, int xPosEnd, const char *text, int fontColor	);//	Panel
char __fastcall DrawTextWithScale( int xPos, int yPos, int xPosEnd, char *text, char fontSize, int scale	);//	Panel		
void  CheckLevelUpButton( );//	Panel		
void  LevelUPButton_MouseClick( );//	Panel		
void  DrawLevelUpButton( );//	Panel		
void  CharPanel_MouseDown();//	Panel
void  CharPanel_MouseClick();//	Panel
void  DrawAllDamagedItemsPics( );//	Panel		
void __fastcall DrawOneDamagedItemPic( const Item& a1, int& edx0, int a2	);//	Panel
int  DrawRedScreen( );//	Panel		
int __fastcall GetColorNumberWithSpellBook( int spellNumber, int flag	);//	Panel		
void  DrawSpellBook( );//	Panel		
void  BookPanel_CkeckLButtonClick( );//	Panel		
char* __fastcall formatGoldAmount( int currentGold	);//	Panel		
void __fastcall SplitGoldPile( int currentAmount	);//	Panel		
int __fastcall GoldSplitHandle( char Key	);//	Panel		
void __fastcall GoldSplit( int playerIndex, int cellIndex	);//	Panel		
void __fastcall SetGoldViewOnCursor( int playerIndex	);//	Panel		
void  DrawTalkPanel( );//	Panel		
int __fastcall DrawTalkText( int a1, int a2, int a3, int *a4, char aFontSize	);//	Panel		
bool  TalkPanel_CheckMouseClick( );//	Panel		
void  TalkPanel_LButtonUp( );//	Panel		
void  OpenTalkPanel( );//	Panel		
void  CloseTalkPanel( );//	Panel		
void  ToggleTalkPanel( );//	Panel
void  TogglePvP( );//	Panel
bool  PlayerFriendly();
bool __fastcall PutNewCharInTalkPanelPromt( unsigned int a1	);//	Panel		
bool __fastcall HandleTalkPanelControlKeys( int a1	);//	Panel		
void  AddNewStringInTalkPanel( );//	Panel		
void __fastcall PutSomeSpecialCharInTalkPanel( char a1	);//	Panel		
void  InitItemsCELArrays( );//	Main		
void  FreeItemsCELArrays( );//	Main		
void __fastcall SetupItemSizes( int graphicsIndex	);//	Main		
void __fastcall SetCursorGraphics( int graphicsIndex	);//	Main		
void  ResetCursorOnPlayer( );//	Main		
void  ShowTownPortalDescription( );//	Main		
void  ShowUnholyPortalDescription( );//	Main		
void  DispatchCursorAboveIsoland( );//	Main		
void  InitDead();//	Main
void __fastcall AddMonsterCorpse( int row, int col, int monsterSpriteNum, int orient	);//	Main		
void  SyncUniqDead( );//	Main
void  SetBossLightActive( );//	Main
void  LoadSquareCEL( );//	Main		
void  FreeSquareSprite( );//	Main		
void  CheckMonsterAndPlayerMapsClearForMultiplayer( );//	Main		
void  FreeDungeonResources( );//	Main		
int __fastcall StartGame( int newGame, int singleGame	);//	Main		
void __fastcall RunGameLoop( int a1	);//	Main		
void __fastcall ClearPrevSessionData( int msg	);//	Main		
void  FreeGame( );//	Main		
bool  NoMyExec( );//	Main		
int __stdcall WinMain( HINSTANCE HInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd	);//	Main		
void  InitSomeBoolFlags( );//	Main		
void __fastcall Character_Passive_Life_and_Mana_Regeneration( int playerIndex	);//	Main	
void  InitOnScreenData( );//	Main		
int __stdcall TopLevelExceptionFilter( struct _EXCEPTION_POINTERS *a1	);//	Main		
HWND __fastcall AlreadyHasWindow( char* lpClassName	);//	Main		
int __fastcall MyLoadModule( HMODULE hModule	);//	Main		
int  EscHandle( );//	Main		
int __stdcall DisableInputWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Main		
int __stdcall GameWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Main		
int __fastcall Mouse_LButtonDown( int wParam	);//	Main		
int __fastcall IsoLand_CheckLButtonClick( int shiftPressed, int repeat	);//	Main		
bool  CursorSkillApply( );//	Main		
void  OnMouseLButtonUp( );//	Main		
int __fastcall Mouse_RButtonDown( int repeat	);//	Main		
int __fastcall CheckF10Message( int a1	);//	Main		
void __fastcall SendNetGroupChatMesage( int a1	);//	Main		
void __fastcall KeyUpHandler( int );//	Main
void __fastcall KeyPressHandler( WPARAM aKeyCode	);//	Main		
int __fastcall F1F4PressHandler( WPARAM fIndex, bool flag	);//	Main		int __usercall F1F4PressHandler@<ecx>(int fIndex@<ecx>, bool flag@<zf>)
void  HandlePause( );//	Main		
void __fastcall HandleCharHotkey( int a1	);//	Main		
void  LoadLvlGFX( );//	Main		
void  LoadMissilesAndObjects( );//	Main		
void __fastcall CreateLevel( int loadingMode	);//	Main	
void __fastcall LoadGameLevel( int startNewGame, int lvlDir	);//	Main		
void __fastcall GameLoop( int startup	);//	Main		
void  GameLogic( );//	Main		
void __fastcall TimeOutCursor( bool timeout	);//	Main		
void  AnimateLevelPalette( );//	Main		 
void  AllocPlayers( );//	Main		
Player* __fastcall AllocPlayersChunk( Player* memory	);//	Main		
int  GetMapOfTheStarsMinutsPassedCount( );//	PopupScreens		
void  OpenPopupScreen( );//	PopupScreens		
bool  ReadPopupScreen( );//	PopupScreens		
void  ClosePopupScreen( );//	PopupScreens		
void  DrawPopupScreen( );//	PopupScreens		
void  ClearAllMaps( );//	GenerateMap		
void __fastcall LoadQuestDungeonMainMapChurch( char* dunFileName, int playerRowPos, int playerColPos	);//	GenerateMap		
void  RandomizeChurchFloorTiles( );//	GenerateMap		
void  InitLevelTiles( int beyondTile );//	GenerateMap		
void  InitChurchArchs( );//	GenerateMap		
void __fastcall LoadQuestDungeonAltMapChurch( char* fileName, int a2, int a3	);//	GenerateMap		
void __fastcall CreateChurchTypeLabyrinth( int levelRandomSeed, int loadingMode	);//	GenerateMap		
bool LoadQuestPattern(QuestPattern& qp);
void FreeQuestPattern(QuestPattern& qp);
void  LoadQuestPatterns( );//	GenerateMap		
void  ClearQuestPatterns( );//	GenerateMap		
void  InitHellfireArchs( );//	GenerateMap		
void __fastcall ChurchLevelGeneration( int loadingMode	);//	GenerateMap		
void __fastcall ChurchPlaceDoorTiles( int row, int col	);//	GenerateMap		
void  InitSomeCryptLavaFloorCells( );//	GenerateMap		
void  InitChurchStaticShadows( );//	GenerateMap		
int __fastcall GenerateChurchPatternObject( uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, bool resetPlayerPosition, int a7, int a8	);//	GenerateMap		
void  ClearGenerationMap( );//	GenerateMap		
void  ClearWallFlagMapBit7( );//	GenerateMap		
void  ConstructChurchRooms( );//	GenerateMap		
void __fastcall BuildChurchRectangleRoom( int startRow, int startCol, int rows, int cols	);//	GenerateMap		
void __fastcall RecursiveChurchRoomsConstruct( int startRow, int startCol, int rows, int cols, int quarterProbability	);//	GenerateMap		
bool __fastcall ThereIsNoRoom( int startRow, int startCol, int rows, int cols	);//	GenerateMap		
int  AreaOfLevelRoomsChurch( );//	GenerateMap		
void  CopyChurchInitalRoomsScheme( );//	GenerateMap		
void  InitChurchWallAndFloorMapFromBynaryPattern( );//	GenerateMap		
void  ChurchMakeSomeCrossWalls( );//	GenerateMap		
int __fastcall ChurchCountWallInRowFrom( int row, int col	);//	GenerateMap		
int __fastcall ChurchCountWallInColFrom( int row, int col	);//	GenerateMap		
void __fastcall ChurchSetVerticalWall( int startRow, int startCol, char startWallType, int wallsCount	);//	GenerateMap		
void __fastcall ChurchSetHorisontalWall( int startRow, int startCol, char startWallType, int wallsCount	);//	GenerateMap		
void  ConvertToChurchWalls( );//	GenerateMap		
void __fastcall PlaceCryptPatternOnMap( uchar* aYSize, int MayBeSize	);//	GenerateMap		
void  ChurchTilesVariationsChange( );//	GenerateMap		
void  SetupChurchHallsPassagesAndPlaceSpecialPatternsInRandomHall( );//	GenerateMap		
void __fastcall SetupChurchHall( int row, int col, int hasCoridorInNE, int hasCoridorInSW, int hasCoridorInNW, int hasCoridorInSE	);//	GenerateMap		
void __fastcall SetupChurchPassage( int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
bool __fastcall FindPatternPlace(QuestPattern& qp);
void __fastcall PlaceQuestPattern( QuestPattern& qp, int row = -1, int col = -1	);//	GenerateMap		
bool PlaceQuestPatterns( int oldRow = -1, int oldCol = -1 );
bool PlaceSmallQuestPatterns();
void __fastcall CopyCryptLastBossPatternToMap( int row, int col	);//	GenerateMap		
void __fastcall CopyCornerstoneRoomPatternToMap( int row, int col	);//	GenerateMap		
void  InitChurchWallsTransparency( );//	GenerateMap		
void __fastcall SetChurchCellTransparency( int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
void  ChurchSetAdditionalWallsTransparency( );//	GenerateMap		
void  SomeConvertChurchAndCryptWalls( );//	GenerateMap		
void  SomeMoreChurchWallsConvert( );//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_1( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_2( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_3( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_4( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_5( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_7( int a1	);//	GenerateMap		
void __fastcall PlaceSomeCryptPatterns_6( int a1	);//	GenerateMap		
void  InitCataMaps( );//	GenerateMap		
void  AddAdditionalCataDoors( );//	GenerateMap		
void  ReplaceEnderCataDoorsWithEnderWalls( );//	GenerateMap		
void __fastcall LoadQuestDungeonMapMainCatacomb( char* dun, int a2, int a3	);//	GenerateMap
void __fastcall LoadQuestDungeonMapAltCatacomb( char* dun	);//	GenerateMap
void __fastcall CreateCatacombTypeLabyrinth( int randomSeed, int loadingMode	);//	GenerateMap		
void __fastcall CatacombLevelGeneration( int loadingMode	);//	GenerateMap		
int __fastcall GenerateCataPatternObject( uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	);//	GenerateMap		
void __fastcall PlaceCataPattern( uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
void  CataTilesVariationsChange( );//	GenerateMap		
void  ApplyStrangeCataPatterns( );//	GenerateMap		
void  SomeCataWallAndFroorConvert( );//	GenerateMap		
bool  MakePrimaryCataMap( );//	GenerateMap		
void __fastcall RecursiveCataHallsGeneration( int startRow2, int startCol2, int endRow2, int endCol2, int motherCataHallIndex, int orientation, int needQuestPatternRoom, int questPatternRoomRowSize, int questPatternRoomColSize	);//	GenerateMap		
void __fastcall ConstructCataHall( int startRow, int startCol, int endRow, int endCol, int isFixedHall	);//	GenerateMap		
void __fastcall SaveCataPassageParamsToQueue( int firstDoorRow, int firstDoorCol, int secondDoorRow, int secondDoorCol, int startOrientation	);//	GenerateMap		
void __fastcall LoadNextCataPassageParams( int& firstDoorRow, int& firstDoorCol, int& secondDoorRow, int& secondDoorCol, int& startOrientation	);//	GenerateMap		
bool __fastcall MakeCataPassage(int firstDoorRow, int firstDoorCol, int secondDoorRow, int secondDoorCol, int startOrientation);
void __fastcall MakeCataDoor( int row, int col	);//	GenerateMap		
void __fastcall MakeCataPassageFloor( int row, int col	);//	GenerateMap		
void __fastcall CataFillWallAndFloorMapWithUsingSomePatterns( int row, int col	);//	GenerateMap		
bool  CataHallsExpand( );//	GenerateMap		
bool __fastcall IsValidCombination( int mayDecRow, int mayDecCol, int mayIncRow, int mayIncCol	);//	GenerateMap		
int  CountOfCataBeyond( );//	GenerateMap		
void __fastcall ExpandCataHall( int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
void __fastcall CompleteExpandCataHall( int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
void  InitCataWallsTransparency( );//	GenerateMap		
void __fastcall SetCataCellTransparency( int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
void  CataSetAdditionalWallsTransparency( );//	GenerateMap		
void  SomeCataCornersConvert( );//	GenerateMap		
void  InitCataArchs( );//	GenerateMap		
void  MakeDoorsInWoodWalls( );//	GenerateMap		
void  DeleteUnwantedWoodenDoorTiles( );//	GenerateMap		
void  CaveConvertTempTailToEnterToTownTiles( );//	GenerateMap		
void  SomeStrangeCaveTilesConvert( );//	GenerateMap		
void __fastcall CaveBinPatternSomeStrangeRecursiveCalc( int row, int col	);//	GenerateMap		
bool  IsAllCaveFloorTilesAreLinked( );//	GenerateMap		
void __fastcall CreateCaveTypeLabyrinth( int levelRandomSeed, int loadingMode	);//	GenerateMap		
void __fastcall CaveLevelGeneration( int loadingMode	);//	GenerateMap		
bool __fastcall MakeSerratedCaveBinaryPatternRoom( int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
void __fastcall RecursiveCaveRoomsGeneration( int startRow, int startCol, int somePrevRoomSize, int maybeOrientation	);//	GenerateMap		
void __fastcall CaveMakeRectRoomInBinPattern( int startRow, int startCol, int endRow, int endCol	);//	GenerateMap		
void  SomeCaveBinPatternConvert_MaybeLavaRiverConvert( );//	GenerateMap		
void  CaveBinLavaLakeConvert( );//	GenerateMap		
void  MakeSomeLavaBridgeInCaveBinPattern( );//	GenerateMap		
void  SetToZeroLastLinesInCaveBinPattern( );//	GenerateMap		
int  AreaOfLevelRoomsCave( );//	GenerateMap		
void  InitCaveWallAndFloorMapFromBynaryPattern( );//	GenerateMap		
void  CaveMakeLavaRivers( );//	GenerateMap		
void  CaveMakeSomeLavaLakes( );//	GenerateMap		
int __fastcall IsCaveTileLinkedWithOutOfMap( int row, int col, int* someRadius	);//	GenerateMap		
int __fastcall IsCaveTileLinkedWithOutOfMap_2( int row, int col, int* someRadius	);//	GenerateMap		
void  CaveLavaConvert_SomeBeyondIntoLava( );//	GenerateMap		
bool __fastcall PlaceCavePatMap( uchar* patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	);//	GenerateMap		
void __fastcall PlaceCavePattern( uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
int __fastcall PlaceAbyssWallPattern( uchar* patternPtr, int chanceToPlace	);//	GenerateMap		
void  MakeSomeCaveWoodenWallsCrossWallsAndDoors( );//	GenerateMap		
bool __fastcall IsNotMainDiagonalWoodenWallStart( int row, int col	);//	GenerateMap		
bool __fastcall IsNotMainDiagonalWoodenWallEnd( int row, int col	);//	GenerateMap		
bool __fastcall IsNotUnmainDiagonalWoodenWallStart( int row, int col	);//	GenerateMap		
bool __fastcall IsNotUnmainDiagonalWoodenWallEnd( int row, int col	);//	GenerateMap		
void __fastcall LoadQuestDungeonMainMapCave( char* dunNamePtr, int a2, int a3	);//	GenerateMap		
void __fastcall LoadQuestDungeonAltMapCave( char* dunNamePtr, int a2, int a3	);//	GenerateMap		
void  FixingHellPandemoniumTiles( );//	GenerateMap		
void __fastcall CopyQuestPatternToHellMap2( char* patternPtr, int row, int col	);//	GenerateMap		
void __fastcall MakePandemonium( int isAltDun	);//	GenerateMap		
bool __fastcall IsHellBeyondTileWithWallsContinueInLuAndLd( char hellWafm	);//	GenerateMap		
bool __fastcall IsHellBeyondTileWithWallsContinueInRuAndRd( char hellWafm	);//	GenerateMap		
void  SetToZeroFirstLinesInHellBinPattern( );//	GenerateMap		
void  SomeStrangeHellTilesConvert( );//	GenerateMap		
void __fastcall CreateHellTypeLabyrinth( int levelRandomSeed, int loadingMode	);//	GenerateMap		
void __fastcall HellLevelGeneration( int loadingMode	);//	GenerateMap		
void  CreateSomeStaticHellShadow( );//	GenerateMap		
void  InitHellMaps( );//	GenerateMap		
void  InitHellWallAndFloorMapFromBynaryPattern( );//	GenerateMap		
void  HellMakeSomeCrossWalls( );//	GenerateMap		
int __fastcall HellCountWallInRowFrom( int row, int col	);//	GenerateMap		
int __fastcall HellCountWallInColFrom( int row, int col	);//	GenerateMap		
void __fastcall HellSetVerticalWall( int startRow, int startCol, int wallsCount	);//	GenerateMap		
void __fastcall HellSetHorisontalWall( int startRow, int startCol, int wallsCount	);//	GenerateMap		
void  HellLevelWallsConverting( );//	GenerateMap		
void  HellTilesVariationsChange( );//	GenerateMap		
void  CopyHellInitalRoomsScheme( );//	GenerateMap		
void  SomeStrangeHellBinaryMapChenges( );//	GenerateMap		
int  AreaOfLevelRoomsHell( );//	GenerateMap		
void  ConstructHellRooms( );//	GenerateMap		
void __fastcall BuildHellRectangleRoom( int startRow, int startCol, int rowSize, int colSize	);//	GenerateMap		
void __fastcall RecursiveHellRoomsConstruct( int startRow, int startCol, int rows, int cols, int quarterProbability	);//	GenerateMap		
bool __fastcall ThereIsNoHellRoom( int startRow, int startCol, int rowSize, int colSize	);//	GenerateMap		
bool __fastcall PlaceHellPatMap( int patternPtr, int minPatternsToPlace, int maxPatternsToPlace, int a4, int a5, int resetPlayerPosition, int a7	); // GenerateMap		
void  InitHellWallsTransparency( );//	GenerateMap		
void __fastcall SetHellCellTransparency( int row40, int col40, int row112, int col112, int orientation	);//	GenerateMap		
void  HellSetAdditionalWallsTransparency( );//	GenerateMap		
void  SomeHellBeyondTilesConvert( );//	GenerateMap		
void  SetupDeltaCriticalSection( );//	thread		
void __fastcall DeltaRemovePlayer( int a1 );//	thread
void __fastcall DThreadSendPLayerInfoChunk( int playerIndex, char cmd, void* src, size_t size);//	thread
void  DeltaThreadStart( );//	thread		
unsigned int __stdcall DeltaThread( void*	);//	thread		отдельный сетевой поток
void  DeltaThreadStop();//	thread
void  SetupDDSurfaceCriticalSection( );//	thread		
int __fastcall SetupScreenResolution( HWND ahWnd	);//	dx		
void  CreateWorkingSurface( );//	dx		
HRESULT  CreateScreenSurface( );//	dx		
HRESULT __fastcall CallDirectDrawCreate( GUID *guid, LPDIRECTDRAW *ddraw, IUnknown* iid	);//	dx		
void  LockSrcSurface( );//	dx		
void  UnlockSrcSurface( );//	dx		
void  ReleaseDDSurface( );//	dx		ее можно было бы пометить как сверяемую, поскольку тут были вылеты в TH 2
void  ReturnToDDSurface( );//	dx
void SwitchVirtualShader(); // DX
int __fastcall SoundIsPlaying( int soundIndex	);//	Engine, Sound		
void  StopSpeech( );//	Engine, Sound		
void __fastcall LoadMonsterSounds( int spriteIndex	);//	Engine, Sound		
void  FreeMonsterSounds( );//	Engine, Sound		
void __fastcall PlayMonsterSound( int monsterIndex, MONSTER_SOUND_TYPE soundType	);//	Engine, Sound		
bool __fastcall SoundPos( int row, int col, int& maxDelta, int& angle	);//	Engine, Sound		
void __fastcall PlayGlobalSound( int soundIndex	);//	Engine, Sound		
void __fastcall LoadAndPlaySoundFile( Sound& soundFile, int isLocal, int row, int col	);//	Engine, Sound		
void __fastcall PlaySpeech( Sound& soundFile, int range, int angle	);//	Engine, Sound		
int __fastcall RandomizeSound( int sound	);//	Engine, Sound		
void __fastcall PlayLocalSound( int soundIndex, int row, int col	);//	Engine, Sound		
void  StopAllSounds( );//	Engine, Sound		
void  UpdateSound( );//	Engine, Sound		
void  FreeAllSound( );//	Engine, Sound		
void  LoadClassSoundEffect( );//	Engine, Sound		
void __fastcall LoadSoundType( uchar soundType	);//	Engine, Sound		
void  MenuSoundInit( );//	Engine, Sound		
void __stdcall PlaySoundFile( int soundIndex	);//	Engine, Sound		
void __fastcall Decrypt( uint* aBuffer, uint aSize, uint aHashKey	);//	Engine, MpqHash		
void __fastcall Encrypt(uint* data, uint bytes, uint key);
uint __fastcall Hash( char* key, int hashType	);//	Engine, MpqHash		
void  InitializeHashSource( );//	Engine, MpqHash		
size_t __fastcall Compress( uchar* Dst, size_t size	);//	Engine, MpqHash		
unsigned int __cdecl CompBufferRead( char* buf, unsigned int* size, void* param	);//	Engine, MpqHash		
void __cdecl CompBufferWrite( char* buf, unsigned int* size, void* param	);//	Engine, MpqHash		
void __fastcall Expand( uchar* data, uint bytes, uint dwMaxBytes	);//	Engine, MpqHash		
void __fastcall Surface_DrawCEL( int x, int y, char* cel, int frame, int width	);//	Engine, Draw?		
void __fastcall Surface_DrawCelTo( uchar* dst, char* cel, int frame, int width	);//	Engine, Draw?		
void __fastcall CEL_DrawTo(uchar* to, char* celBuf, uint frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag = R_DIRECT, int outlineColor =-1);
void __fastcall CEL_Draw( int x, int y, char* celBuf, uint frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag = R_DIRECT, int outlineColor =-1   );//	Engine, Draw?		
void __fastcall WriteTileToSurface( void* dst, char* src, int frameNumber, int width, int a5, int sizeOffset	);//	Engine, Draw?		
void __fastcall Font_DrawChar( int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize	);//	Engine, Draw?		
void __fastcall DrawItemOnMap( int offsetX, int offsetY, char* celFile, int celIndex, int celWidth, int upHalfCel, int screenCellRow, int outlineColor = -1	);//	Engine, Draw?		
void __fastcall CEL_DrawShadow( uchar* dst, char* celBuf, int frameNum, int drawWidth, int offsetStart, int offsetEnd, int flag = 0	);//	Engine, Draw?		
void __fastcall ParseCELFile( uchar* aDestPtr, int aDstStartX, int aDstStartY, int aDstMaxX, char* aCELFilePtr, int aFrameNumber, int aSrcMaxX	);//	Engine, Draw?		
void __fastcall AutomapDrawPoint1( int xPos, int yPos, uchar color	);//	Engine, Draw?		
void __fastcall AutomapDrawPoint( int xPos, int yPos	);//	Engine, Draw?		
void __fastcall AutomapDrawLine( int x1, int y1, int x2, int y2, uchar color	);//	Engine, Draw?		
int __fastcall OrientationToTarget( int row, int col, int targetRow, int targetCol	);//	Engine, Geometry		
void __fastcall SetRndSeed( int seed	);//	Engine, Random		
int  GetRndSeed( );//	Engine, Random		
int __fastcall RNG( int range, int callerId = 0	);//	Engine, Random		
i64 Rng32to64(i64 range);
float __fastcall RngPercent(float rangePercent);
u64 TrueRandom64();
void __fastcall InitRandomSeed64( u64 seed	);//	Engine, Random		
u64 GetRndSeed64( );//	Engine, Random		
uint __fastcall Rng64( uint range = 0xFFFF'FFFFU );//	Engine, Random		
void  InitLocking( );//	Engine, Lock		
void __fastcall ActNullSpell( int spellCastIndex	);//	Engine, Memory		
char* __fastcall SoundNameDup(const char* str);
void* __fastcall AllocMem( uint a1	);//	Engine, Memory		
void __fastcall FreeMem( void* a1	);//	Engine, Memory		отдельный сетевой поток
void* __fastcall LoadFile( const char* fileName, int* byteReaded = 0	);//	Engine, Memory		
int __fastcall LoadFileWithMem( char* arglist, void* a2	);//	Engine, Memory		
void __fastcall ColorSpriteAnimation( char* anim, uchar* colorTable, int frameCount	);//	Engine, Draw?		
void __fastcall Rle2Draw(uchar* dst, char* src, int size, int width);// Draw CL2 (Player animation)		
void __fastcall CL2_DrawOutline( uchar color, int x, int y, char* celBuf, uint frameNum, int celWidth, int offsetStart, int offsetEnd	);//	Engine, Draw?		
void __fastcall CL2_DrawDark( int x, int y, char* cl2Buf, uint frameNum, int pitch, int offsetStart, int offsetEnd, char lightType	);//	Engine, Draw?		
void __fastcall Rle2DrawPal( uchar* surf, char* sprite, int size, int pitch, uchar* palette	);//	Engine, Draw?		
void __fastcall CL2_Draw( int x, int y, char* curAnim, uint frameNum, int pitch, int shiftY, int tileInCellIndex, bool darken = false	);//	Engine, Draw?		
void __fastcall PlayVideo2(char* name);//	Engine, Draw?		
void __fastcall AddOnScreenMessage( char messageIndex, int messageTime = 70	);//	Engine, Draw?		
void  ClearOnScreenMessages( );//	Engine, Draw?		
void DrawOnScreenMessage( );//	Engine, Draw?		
void  SelectMainMenu( );//	Menu		
void  SPMainMenuHandler( );//	Menu
void  MPMainMenuHandler( );//	Menu
void  ResetMainMenu( );//	Menu
void  ToggleMenu( );//	Menu		
void __fastcall GoBackMenuHandler( int a1, int a2	);//			
void __fastcall NewGameMenuHandler( int a1, int a2	);//	Menu
void __fastcall MurderHeroMenuHandler(int a1, int a2);
void __fastcall NullMenuHandler(int a1, int a2);
void __fastcall ExitGameMenuHandler( int a1, int a2	);//	Menu		
void __fastcall LoadGameMenuHandler( int a1, int a2	);//	Menu		
void __fastcall SaveGameMenuHandler( int a1, int a2	);//	Menu		
void __fastcall RestartMenuHandler( int a1, int a2	);//	Menu		
void __fastcall SetupMenuHandler( int a1, int a2	);//	Menu
void MurderHeroInGame();
void __fastcall ConfigMenuHandler(int a1, int a2);//	Menu
void __fastcall VideoOptions1MenuHandler(int a1, int a2);
void __fastcall VideoOptions2MenuHandler(int a1, int a2);
void __fastcall GameplayOptions1MenuHandler(int a1, int a2);
void __fastcall ScreenHeightHandler(int a1, int a2);
void __fastcall ScreenWidthHandler(int a1, int a2);
void __fastcall FullScreenHandler(int a1, int a2);
void __fastcall FPSHandler(int a1, int a2);
void __fastcall ShowFPSHandler(int a1, int a2);
void __fastcall RunindungeonHandler(int a1, int a2);
void __fastcall AutopickupHandler(int a1, int a2);
void __fastcall EasyclickHandler(int a1, int a2);
void __fastcall AltholdHandler(int a1, int a2);
void __fastcall SwapSpellsOnScrollHandler(int a1, int a2);
void __fastcall GameplayOptions2MenuHandler(int a1, int a2);
void __fastcall VisualOptions1MenuHandler(int a1, int a2);
void __fastcall VisualOptions2MenuHandler(int a1, int a2);
void __fastcall VisualOptions3MenuHandler(int a1, int a2);
void __fastcall PanelHandler(int a1, int a2);
void __fastcall PotionHandler(int a1, int a2);
void __fastcall MapblendHandler(int a1, int a2);
void __fastcall ShowNumbersOnHealthHandler(int a1, int a2);
void __fastcall ShowNumbersOnManaHandler(int a1, int a2);
void __fastcall AlwaysHighlightGolemHandler(int a1, int a2);
void __fastcall ShowMonsterHealthBarHandler(int a1, int a2);
void __fastcall ShowVersionOnScreenHandler(int a1, int a2);
void __fastcall CombatTextOptions1MenuHandler(int a1, int a2);
void __fastcall CombatTextOptions2MenuHandler(int a1, int a2);
void __fastcall CombatTextOptions3MenuHandler(int a1, int a2);
void __fastcall showCombatTextHandler(int a1, int a2);
void __fastcall showCombatExpHandler(int a1, int a2);
void __fastcall ShowPercentExpGainHandler(int a1, int a2);
void __fastcall ShowReceivedDamageHandler(int a1, int a2);
void __fastcall ReceivedDamageThresholdHandler(int a1, int a2);
void __fastcall mergeCombatTextsHandler(int a1, int a2);
void __fastcall showInflictedDamageHandler(int a1, int a2);
void __fastcall MonsterReceivedDamageThresholdHandler(int a1, int a2);
void __fastcall FloatingHPRegenThresholdHandler(int a1, int a2);
void __fastcall FloatingManaRegenThresholdHandler(int a1, int a2);
void __fastcall ShowBreakWarningHandler(int a1, int a2);
void __fastcall RedInactiveSpellIconsHandler(int a1, int a2);
void __fastcall SaveToConfigHandler(int a1, int a2);
void SetMusicMenu( );//	Menu		
void SetAmbientMenu( );//	Menu		
void __fastcall SetVolumeMenu( char** text, Menu* menu, int volume	);//	Menu		
void SetSoundMenu( );//	Menu		
void SetWalkMenu( );//	Menu		
void SetGammaMenu( );//	Menu		
void SetContrastMenu( );//	Menu		
void __fastcall MusicMenuHandler( int a1, int a2	);//	Menu		
void __fastcall AmbientMenuHandler( int a1, int a2	);//	Menu		
int __fastcall Menu_GetVolume( Menu* menuItem	);//	Menu		
void __fastcall SoundMenuHandler( int a1, int a2	);//	Menu		
void __fastcall WalkMenuHandler( int a1, int a2	);//	Menu		
void __fastcall GammaMenuHandler( int a1, int a2	);//	Menu		
void __fastcall ContrastMenuHandler( int a1, int a2	);//	Menu		
void  FillSolidBlockTables( );//	Halls		
void  InitDarkenDungeonTiles( );//	Halls		
void __fastcall SortTileLightmap( int count	);//	Halls		
void __fastcall SwapTileData( int counter, int nextCounter	);//	Halls		
int __fastcall IsometricMapOffset( int row, int col	);//	Halls		
void  InitDrawingTileWithLight( );//	Halls		
void  InitDungeonTiles( );//	Halls		
void  ClearWallsTransparency( );//	Halls		
void __fastcall SetAreaTransparencyByIndex_40( int startRow40, int startCol40, int endRow40, int endCol40	);//	Halls		
void __fastcall SetRectTrans( int startRow112, int startCol112, int endRow112, int endCol112	);//	Halls		
void __fastcall CopyCellTransparency( int rowFrom, int colFrom, int rowTo, int colTo	);//	Halls		
void __fastcall FillAreasDiffOnMap112x112u( int aAreaNumber, MapArea* aAreaPtr	);//	Halls		
void __fastcall FillAreasSomeOnMap112x112u( int AreaNumber, MapArea* aAreaPtr	);//	Halls		
void  ClearCurPattern( );//	Halls		
void  MarkPatternPositionOnMap(QuestPattern& qp = CurPattern);//	Halls		
void __fastcall MarkPandemoniumPositionOnMap( int startRow40, int startCol40, int rowSize40, int colSize40	);//	Halls		
bool __fastcall GetMaxSizeForRoom( int floorTilesIndex, int starRow, int startCol, int minSizeOfRooms, int maxSizeOfRooms, int& row40Size, int& col40Size	);//	Halls		
void __fastcall SetupRoom( int roomIndex	);//	Halls		
void __fastcall CreateRooms( int minSizeOfRooms, int maxSizeOfRooms, int floorTilesIndex, int skipTilesParam, int needSizeRandomization	);//	Halls		
void  SetForbiddenObjectFlagForAloneRooms2( );//	Halls		
bool __fastcall IsCoordinateNotNearAloneRoom( int row, int col	);//	Halls		
void  InitLevels( );//	Halls		
void  Menu_ShowPause( );//	MenuEngine		
char __fastcall Font_DrawText( int aXpos, int aYpos, char* aText	);//	MenuEngine		
void  FreeMenuGraphics( );//	MenuEngine		
void  Menu_Init( );//	MenuEngine		
bool  IsGameMenuActive( );//	MenuEngine
bool IsPausedInMenu();
void __fastcall Menu_SetupMenuWindow( Menu *a1, void (__cdecl *aMenuHandlerPtr)()	);//	MenuEngine		
void __fastcall SelectMenuItem( int aDirection	);//	MenuEngine		
void  Menu_Draw( );//	MenuEngine		
void __fastcall Menu_DrawItem( Menu* itemPtr, int itemYPos	);//	MenuEngine		
void* __fastcall Menu_DrawItemBackground( int a1, int a2, size_t Size, int a4	);//	MenuEngine		
int __fastcall Menu_GetItemTextWidth( Menu* aMenu	);//	MenuEngine		
int __fastcall Menu_DispatchKey( int KeyPressed	);//	MenuEngine		
void __fastcall Menu_RegulateItem( int aDirection	);//	MenuEngine		
int __fastcall Menu_ItemMouseClick( int a1	);//	MenuEngine		
bool __fastcall Menu_SetNewRegulatorPos( uint* a1	);//	MenuEngine		
int __fastcall Menu_CheckMouseClick( bool mouseButtonState	);//	MenuEngine		
void __fastcall Menu_SetItemAvailability( Menu* menuItem, int isActivated	);//	MenuEngine		
unsigned int __fastcall Menu_SetSlider( Menu* aMenu, int aMin, int aMax, int aCur	);//	MenuEngine		
int __fastcall Menu_GetRegulatorPos( Menu* aMenu, int min, int max	);//	MenuEngine		
void __fastcall Menu_SetSliderTicks( Menu* menuItem, __int16 pos	);//	MenuEngine		
void  ResetHelp( );//	Help		
void  GameHelp( );//	Help		
char __fastcall DrawHelpText( int a1, int a2, int a3, char aFontSize	);//	Help		
void  ShiftHelpUp( );//	Help		
void  ShiftHelpDown( );//	Help		
void __fastcall CloseGameArchives( bool exitGame	);//	Windows		
void __fastcall DisableScreenSaver( char a1	);//	Windows		
void  CreateMainWindow( );//	Windows		
void  CloseOldWinExplorer( );//	Windows		
HWND  FindingOldWinExplorer( );//	Windows		
void  WaitForOldWinExplorerClosed( );//	Windows		
void  OpenGameArchives( );//	Windows		
char* __fastcall OpenMPQ( char (&Dest)[MAX_PATH], char* edx0, char* aRegistryKey, int mpqId, int a3	);//	Windows		
char* __fastcall FindFileNameInPath( char* Str	);//	Windows		
int __fastcall OpenMPQfromCD( char (&str)[MAX_PATH], char* edx0, int a2, int aHandle	);//	Windows		
void  GetGameVersion( );//	Windows		
int __stdcall DefaultHandler( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Windows		
void __fastcall ActivateWindow( HWND hWnd, WPARAM wParam	);//	Windows		
int __stdcall CallHandler1IfExists( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Windows		
ProcHandler __fastcall SetWindowProc( ProcHandler handler	);//	Windows		
BOOL  HandleSystemMessage( );//	Windows		
bool  ProgressUpdate( );//	LoadingScreen		
void  ShowOperationProgress( );//	LoadingScreen		
void __fastcall DrawProgressBar( int x, int y, int progressBarIndex	);//	LoadingScreen		
void __fastcall ShowProgress( uint msg	);//	LoadingScreen		
void  FreeLoadingScreenCEL( );//	LoadingScreen		
void __fastcall ProgressLoad( int a1	);//	LoadingScreen		
void  FreeInvPanelCEL( );//	Item		
void  LoadPanelsGraphic( );//	Item		
void __fastcall ShadowItemSlot( int leftPosition, int downPosition, ushort picWidth, ushort  picHeight, uint8_t mark = 0 );//	Item		
void __fastcall CustomShadowItemSlot(int xPosition, int yPosition, ushort width, ushort height, int colorGradient, uint8_t stroke = 0);
void __fastcall ChangeItemBacklight(int xPos, int yPos, int width, int height, int xShift = 0, int yShift = 0, uint8_t stroke = 0);

void SetupInvPanel(int playerIndex);
void  DrawINVPanel( );//	Item		
void  DrawBeltInventory( );//	Item		
Item* __fastcall CanPutCursorToBelt(Player& player);
bool __fastcall AutoPutCursorToCell( Player& player, int cellIndex, int width, int height, bool needPlaceItem	);//	Item		
bool __fastcall AutoPutCursorToInventory(Player& player, int cell = -1);
bool __fastcall TryPutItemInInventoryOrBelt( int playerIndex, int cellIndex, int width, int height, bool needPlaceItem	);//	Item		
bool __fastcall GoldAutoPlace( int playerIndex	);//	Item		
bool __fastcall PutWeaponFromCursorToArm( int playerIndex, int takenCell = -1	);//	Item		
bool PutArmorToBodySlots();
void HandleGearSwap(int takenCell);
bool RightSwapAllowed(int playerIndex, Item& item);
bool OneHandSpecial(int playerIndex, Item& item);
int __fastcall SwapItemsAndGetFirstItemGraphicsValue( Item* firstItem, Item* secondItem	);//	Item		
void __fastcall PutCursorItemToInv( int playerIndex, int mX, int mY	);//	Item		
void __fastcall EquipItemFromNetwork( int playerIndex, int onBodySlotIndex, LastItemInfo info);//	Item		
int __fastcall TakeInvItemToCursor( int playerIndex, int mX, int mY, bool gearSwapping	);//	Item		
void __fastcall DestroyOnBodyItem( int playerIndex, int onBodySlot	);//	Item		
void __fastcall ClearInventorySlot( int playerIndex, int invItemIndex, bool mouse = false	);//	Item		
void __fastcall StripPlayer( int playerIndex	);//	Item		
void __fastcall ClearBeltSlot( int playerIndex, int beltSlot	);//	Item		
void  Inventory_LButtonClick( );//	Item		
bool CursorIntoBelt();
void __fastcall CheckItemStats( int playerIndex	);//	Item		
void __fastcall CheckBookLevel( int playerIndex	);//	Item		
void __fastcall CheckQuestItem( int playerIndex	);//	Item		
void __fastcall InvGetItem( int playerIndex, int itemIndex	);//	Item		
void __fastcall AutoGetItem( int playerIndex, int itemIndex	);//	Item		
int __fastcall FindGetItem( int baseItemIndex, int dropType, int seed, int genVersion, int quality, int magicFind, int uniqIndex );//	Item
void __fastcall SyncGetItem( int row, int col, int baseItemIndex, int dropType, int seed, int genVersion, int quality, int magicFind, int uniqIndex );//	Item
bool __fastcall ItemSpacePathOk( int row, int col, int playerIndex 	);//	Item		
void __fastcall AddWorldMessage( char* Source	);//	Item		
int __fastcall InvPutItem( int playerIndex, int row = -1, int col = -1	);//	Item		
int __fastcall SyncPutItem( uint playerIndex, int row, int col, LastItemInfo info);//	Item
int  GetSlotMouseAbove_Inv( );//	Item		
bool __fastcall MinusRelictFromInventoryOrBelt( int playerIndex	);//	Item		
bool  IsHasRelicForSpell( );//	Item		
void __fastcall MinusChargeFromStaff( int casterIndex, int spellIndex);//	Item		
bool  IsHasChargesForSpell(int spellIndex);//	Item		
uint __fastcall CalcTotalGold( int playerIndex	);//	Item		
bool __fastcall RightClickOnItem( int playerIndex, int invCellIndex, bool mouse = false, bool xinput = false);//	Item		
void TelekinesApply( );
void OpenGoldSplitPanel( );//	Item		
int  DropCursorItemBeforeTrig( );//	Item		
int __fastcall MaxItemPrice(Player& player, int itemCodes	);//	Item		
int __fastcall GetMaxPriceofPlayerBows( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerStaffs( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerSwords( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerHelms( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerShields( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerArmors( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerMaces( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerAmulets( int playerIndex	);//	Item		
int __fastcall GetMaxPriceofPlayerAxes( int playerIndex	);//	Item		
void  InitDoublePrice( );//	Item		
int  ItemLevelByDungeonLevel( );//	Item		
void  LoadFlipItemCELFiles( );//	Item		
bool __fastcall ItemPlace( int row, int col, int playerIndex = -1	);//	Item		
void  AddInitItems( );//	Item		
void  InitItems( );//	Item		
void  PutFragmentsOfReconstructedNote( );//	Item		
void __fastcall CalcCharParams( int playerIndex, int canLoadAnimFlag	);//	Item		
void __fastcall UpdateRelictsState( Player& player );//	Item		
void __fastcall SetAvailableChargesMask( int playerIndex	);//	Item		
void __fastcall RecalcReqMetForOnBodyItems( int playerIndex	);//	Item		
void __fastcall CheckAllItemsReq( int playerIndex	);//	Item		
int __fastcall CheckItemReq( uint playerIndex, Item* item	);//	Item		
void __fastcall RecalcBooksReq( int playerIndex	);//	Item		
void __fastcall RecalcPlayer( int playerIndex, int canLoadAnimFlag, bool forceItemRecalc = true	);//	Item		
void __fastcall SetPlayerHandItem( Item& item, int baseItemIndex	);//	Item		
void __fastcall SetItemGrade(Item& item);
void __fastcall Item_MakeNonCollisionSeed( int playerIndex, Item& item	);//	Item		
void __fastcall SetGraphicToGold( Item& gold	);//	Item		в th2 переиспользуемая функция
void __fastcall CreateInitialItems( int playerIndex	);//	Item		
bool __fastcall ItemSpaceOkIntoWall( int row, int col, int playerIndex = -1	);//	Item		
bool __fastcall ItemSpaceOk( int row, int col, int playerIndex = -1	);//	Item		
bool __fastcall GetItemSpace( int row, int col, int& itemIndex, bool randomNear = true, bool (__fastcall *itemSpaceOk)(int,int,int) = ItemSpaceOk, Item* src = 0    );//	Item		
bool RadialSpaceFound(int& row, int& col, int playerIndex, bool (__fastcall *itemSpaceOk)(int,int,int) = ItemSpaceOk);
void __fastcall Item_RecalculatePrice( Item& item	);//	Item		
void __fastcall Item_InitBooks( int itemIndex, int qlvl	);//	Item		
void __fastcall Item_AddPrefix( Item& item, int MaxAffixQLVL, int Spell_ID, int IsMagicItem	);//	Item		
void __fastcall Item_AddPrefixRare( Item& item, int MaxAffixQLVL, int Spell_ID, int IsMagicItem, int rare );
void __fastcall Item_AddSpellCastAbility( Item& item, int maxAffixQLVL, int isMagicItem, int rare = -1	);//	Item
void __fastcall Item_InitOils( int itemIndex, int qlvl	);//	Item		 
void __fastcall GetItemAttrs( int itemIndex, int baseItemIndex, int qlvl	);//	Item		
void AdjustItemLook(Item& item);
int __fastcall RngFromRange( int min, int max	);//	Item		
float __fastcall RngFromRangePercent( float min, float max );
int __fastcall GetEffectPrice( int effectValue, int effectMin, int effectMax, int goldMin, int goldMax	);//	Item		
void __fastcall Item_ApplyAffix( Item& item, BaseEffect effect, int goldMin, int goldMax, int multiplier, int effectIndex, int affixMinReqClvl, int affixMaxReqClvl, bool forceApply /*= false*/	);//	Item
void __fastcall Item_AddAffixes( Item& item, int minQLVL, int maxQLVL, int occuranceMask, int specializationMask, int isGoodMagicItem	);//	Item		
ITEM_AFFIX __fastcall ItemAffix(Item& item);
void __fastcall ApplyAffixesToItem( int itemIndex, int baseItemIndex, int minQuality, int maxQuality, int isGoodMagicItem, char enableSpellCastAbility	);//	Item		
void __fastcall SetupItemAnim( int itemIndex	);//	Item		
int __fastcall GetRandomBaseForMonsterDrop( int monsterIndex	);//	Item		
int __fastcall GetRandomBaseForHiQualityObjectAndBossDrop( int monsterIndex	);//	Item		
int  GetRandomBaseForLowQualityObjectDrop( );//	Item		
int __fastcall RndTypeItems( int itemCode, int magicCode, int qlvl, int flag = 0, bool minQlvlLimit = false, uchar specialization = 0 );//	Item		
int __fastcall SelectRandomUnique( int itemIndex, int qlvl, int chanceInProcent, int allowDuplicate	);//	Item		
void __fastcall GetUniqueItem( int itemIndex, int uniqueItemIndex	);//	Item		
int __fastcall SpawnUnique( int uniqueItemIndex, int xPos, int yPos, bool sendMsg = false, int baseId = -1	);//	Item		
void __fastcall SetRandomDurability( int itemIndex	);//	Item		
void __fastcall SetupAllItems( int itemIndex, int baseItemIndex, i64 seed, int qlvl, int chanceInProcent, int IsGoodMagicItem, int allowDup, int pregen, bool identified, int playerMask, int magicFind, int rare, int genVersion, int uniqIndex, uchar difficulty, uchar magicLevel = 0  );//	Item
void __fastcall SetupAllItemsSmartdrop(int itemIndex, int baseItemIndex, i64 seed, int qlvl, int chanceInProcent, int IsGoodMagicItem, int allowDup, int pregen, int playerMask = 0, int magicFind = 0, int rare = 0);//	Item
int __fastcall SpawnItem( int monsterIndex, int row, int col, int isNetwork	);//	Item		
int __fastcall CreateRndItem( int row, int col, int isGoodMagicItem, int sendMsg, int delta, int playerMaks = 0	);//	Item		
void __fastcall SetupAllUseful( int itemIndex, int seed, int itemLevel	);//	Item		
int __fastcall CreateRndUseful( int playerIndex, int objectRow, int objectCol, bool sendMsg);//	Item		
int __fastcall CreateTypeItem( int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int a6, int a7, int flag = 0, bool minQlvlLimit =false   );//	Item		
void __fastcall RecreateItem( int itemIndex, short baseItemIndex, int dropType, i64 seed, int amount, uchar mlIdentified, int genVersion, int quality, int magicFind, int uniqIndex, array<short, EnchantMax_5> enchantIndexes, QUENCH_AFFIX quenchAffix, uchar quenchLevel, uchar socketsAdded, array<short, SocketMax_6> sockets, uchar difficulty, uchar flag );//	Item
void  SpawnMagicRock( );//	Item		
void RandomArea(int& row, int& col, int randomAreaSize, bool (__fastcall *itemSpaceOk)(int,int,int) = ItemSpaceOk);
int __fastcall SpawnSomethingQlvl(int baseItemIndex, int row, int col, int qlvl, bool sendMsg, int isGoodMagicItem = 1, bool delta = false, int playerMask = 0);
int __fastcall SpawnSomething( int baseItemIndex, int row, int col, bool specQuest = 0, int randomArea = 0, int selFlag = 0, char dropping = 0, int amount = 0, bool sendMsg = 0);//	Item		
void __fastcall RespawnItem( int itemIndex, int flipFlag	);//	Item		
void __fastcall DeleteItem( int itemIndex, int freeIndex	);//	Item		
void  ClearMapFromInvalidItems( );//	Item		
void  ProcessItems( );//	Item		
void  FreeUniqueItemCELFilesPtrs( );//	Item		
void __fastcall SetUniqueItemCELFile( int itemIndex	);//	Item		
TEXT_COLOR ItemColor(const Item& item);
void __fastcall Item_ShowInfo( int itemIndex	);//	Item		
void __fastcall IdentifyItemWithCursor( uint playerIndex, uint onBodySlotIndex	);//	Item		
void __fastcall RepairItemWithCursor( uint playerIndex, uint onBodySlotIndex	);//	Item		
void __fastcall RepairItemWithBreaking( Item& item, uint playerIndex	);//	Item		
void __fastcall RechargeStaffWithCursor( int playerIndex, int itemCellIndex	);//	Item		
void __fastcall RechargeStaffWithBreaking( Item& item, int chargesPerOneDur	);//	Item		
void __fastcall UseOilWithCursor( int playerIndex, int itemCellIndex	);//	Item		
bool __fastcall ApplyOilToItem( Item& item, Player& player	);//	Item		
bool __fastcall PrintItemEffectToStringBuffer( int affixEffect, Item * itemPtr, int effectIndex	);//	Item		
bool __fastcall UsePotionOrScroll( int playerIndex, Item* item	);//	Item
bool  IsValidBaseForGriswoldBasicalItemsOld( int baseItemIndex	);//	Item		
int __fastcall GetRandomBaseForGriswoldBasicalItems( int maximumQualityLevel	);//	Item		
void __fastcall SwapItems( Item* firstItem, Item* secondItem	);//	Item		
void  BasicalItemsSorting( );//	Item		
void __fastcall BasicalItemsGeneration( int qlvl	);//	Item		
int __fastcall IsValidBaseForGriswoldMagicalItems( int baseItemIndex	);//	Item		
int __fastcall GetRandomBaseItemIndexForGriswoldMagicalItem( int minQualityLevel, int maxQualityLevel	);//	Item		
void __fastcall MagicItemsGeneration( int playerIndex	);//	Item		
void __fastcall GenerateNewMagicItemForSale( int itemIndex, int qlvl, int playerIndex, char someFlag	);//	Item		
int __fastcall IsValidBaseForAdriaItems( int baseItemIndex	);//	Item		
int __fastcall GetRandomBaseForAdriaItems( int maxQlvl	);//	Item		
void  AdriaItemsSorting( );//	Item		
void __fastcall CalculateRequiredMagicForBooks( int currentAdriaItemIndex	);//	Item		
void __fastcall WitchAdriaItemsGeneration( int qlvl	);//	Item		
int __fastcall GetRandomBaseItemIndexForWirtItem( int qlvl	);//	Item		
void __fastcall WirtItemGeneration( int qlvl	);//	Item		
bool __fastcall IsValidBaseForPepinItems( int baseItemIndex	);//	Item		
int __fastcall GetRandomBaseForPepinItems( int maximumQualityLevel	);//	Item		
void  PepinItemsSorting( );//	Item		
void __fastcall PepinItemsGeneration( int qlvl	);//	Item		
void  PutGoldBaseData( );//	Item		
void __fastcall RestoreGriswoldBasicalItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion	);//	Item		
void __fastcall RestoreAdriaItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion	);//	Item		
void __fastcall RestorePepinItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion	);//	Item		
void __fastcall RestoreTownersItems( int itemIndex, int baseItemIndex, int dropType, int seed, int qualityLevel, int genVersion, int quality, int extras	);//	Item
void  CheckAllItemsReqMets( );//	Item
int  SomeActionWithDropItemsInAloneRooms( );//	Item		
int __fastcall CreateSpellBook( int row, int col, int spellIndex, int netCommandFlag, int someFlag	);//	Item
int __fastcall DropItemFromSlain(int row, int col, int isGoodMagicItem, int a4, int maybeIsGenerationTime, int playerMask, int itemCode); //Item
int __fastcall CreateMagicItemFormSlainHero( int row, int col, int magicCode, int itemGfxId, int isNetwork, int isInternal	);//	Item
void DeleteGetRecord( int itemIndex );//	Item
bool CheckGetRecord( const LastItemInfo& info );//	Item
void AddGetRecord( const LastItemInfo& info );//	Item
void RemoveGetRecord( const LastItemInfo& info );//	Item
void InitRare( int itemIndex, int minQuality, int maxQuality, int uniqueChance, int isGoodMagicItem );

void __fastcall RotateOffset( int& x, int& y, int& rx, int& ry, int& ox, int& oy, int& ax, int& ay	);//	Lights		
void __fastcall DoLighting( int row, int col, int radius, int lightIndex	);//	Lights		
void __fastcall DoUnLight( int row, int col, int radius	);//	Lights		
void __fastcall DoUnVision( int row, int col, int lightRadius	);//	Lights		
void __fastcall DoVision( int row, int col, int lightRadius, int someFlag1, int someFlag2	);//	Lights		
void __fastcall LovecraftDoVision( int row, int col, int lightRadius, int someFlag1, int someFlag2 );//	Lights (Gribnik)
void  MakeLightTable( );//	Lights		
void  InitLightMax( );//	Lights		
void  InitLighting( );//	Lights		
int __fastcall AddLight( int casterRow, int casterCol, int radius	);//	Lights		
int __fastcall LightOff( int lightIndex	);//	Lights		
void __fastcall ChangeLightRadius( int lightIndex, int radius	);//	Lights		
void __fastcall ChangeLightPos( int lightIndex, int row, int col	);//	Lights		
void __fastcall ChangeLightOffset( int lightIndex, int deltaRow, int deltaCol	);//	Lights		
void __fastcall ChangeLight( int lightIndex, int row, int col, int radius	);//	Lights		
void  ProcessLightList( );//	Lights		
void  SavePreLighting( );//	Lights		
void  InitVision( );//	Lights		
int __fastcall AddVision( unsigned int row, int col, int lightRadius, int isCurrentPlayer	);//	Lights		
void __fastcall ChangeVisionRadius( unsigned int playerIdentificator, unsigned int lightRadius	);//	Lights		
void __fastcall ChangeVision( uint _field_A0, uint row, uint col	);//	Lights		
void  ProcessVisionList( );//	Lights		
void  AnimateHellPalette( );//	Lights		jmp-call, учесть при сверке
void __fastcall LoadGame( int loadFromMainMenu	);//	Saving		
char  GetNextByte( );//	Saving
ushort GetNextWord();
uint  GetNextHtonl( );//	Saving		
int  GetNextHtonl2( );//	Saving		
bool  GetNextBool( );//	Saving		
void __fastcall LoadPlayer( int playerIndex	);//	Saving		
void __fastcall LoadMonster( int monsterIndex	);//	Saving		
void __fastcall LoadMissile( int missileIndex	);//	Saving		
void __fastcall LoadObject( int objectIndex	);//	Saving		
void __fastcall LoadItems( int itemOnGroundIndex	);//	Saving		
void __fastcall LoadMagicStoreItem( int magicStoreItemIndex	);//	Saving		
void __fastcall LoadQuest( int questIndex	);//	Saving		
void __fastcall LoadObjectLight( int objectLightIndex	);//	Saving		
void __fastcall LoadPlayerVisibility( int playerVisibilityIndex	);//	Saving		
void __fastcall LoadPortal( int entryPointIndex	);//	Saving		
void  SaveGame( );//	Saving		
void __fastcall PutNextByte( char value	);//	Saving
void __fastcall PutNextWord( ushort value );
void __fastcall PutNextHtonl( uint value	);//	Saving		
void __fastcall PutNextHtonl2( int value	);//	Saving		
void __fastcall PutNextBool( int value	);//	Saving		
void __fastcall SavePlayer( int playerIndex	);//	Saving		
void __fastcall SaveMonster( int monsterIndex	);//	Saving		
void __fastcall SaveMissile( int missileIndex	);//	Saving		
void __fastcall SaveObject( int objectIndex	);//	Saving		
void __fastcall SaveItemsOnGround( int itemOnGroundIndex	);//	Saving		
void __fastcall SaveMagicStoreItem( int magicStoreItemIndex	);//	Saving		
void __fastcall SaveQuest( int questIndex	);//	Saving		
void __fastcall SaveObjectLight( int objectLightIndex	);//	Saving		
void __fastcall SavePlayerVisibility( int playerVisibilityIndex	);//	Saving		
void __fastcall SavePortal( int entryPointIndex	);//	Saving		
void  SaveLevel( );//	Saving		
void  LoadLevel( );//	Saving		
void  StartMenuMusic( );//	MainMenu		
void  DisplayMainMenu( );//	MainMenu		
int  SinglePlayerGame( );//	MainMenu		
int __fastcall DoMenu( int heroMenuSelection	);//	MainMenu		
int  MultiPlayerGame( );//	MainMenu		
void  ShowVideo( );//	MainMenu		
void  FreeSpeechPanelPtrs( );//	Speeches		
void  LoadSpeechPanelData( );//	Speeches		
void __fastcall StartSpeech( int speechIndex	);//	Speeches		
void __fastcall DrawLetterInSpeech( int xPos, int yPos, void* fontSet, int letterNumber	);//	Speeches		
void  DrawSpeech( );//	Speeches		
int __fastcall SpellElement(Player& player, int spellIndex);
int __fastcall GetDamageOfPlayerSpell( int playerIndex, int spellIndex, int spellLevel, int element, int* minDamage = 0, int* maxDamage = 0 );//	Missile		
int __fastcall IsPathBlockedToMoveSpell( int row1, int col1, int row2, int col2	);//	Missile		
int __fastcall FindMonsterTargetInRadius( int startRow, int startCol, int radius, std::function<bool(uint monsterIndex)> check	);//	Missile
int __fastcall PlayerSpellLevel( uint playerIndex, uint spellIndex, int spellOrigin =-1	);//	Missile		
void __fastcall RemoveMissile( int castIndex, int castIndexIndex	);//	Missile		
int __fastcall MvM_Ranged( int defenderIndex, int attackerIndex, int minDamage, int maxDamage, int distance, int spellEffect, char isInternalHp, bool reflected = false	);//	Missile
int __fastcall PvM_Ranged( int playerIndex, int monsterIndex, int minDamage, int maxDamage, int distance, int baseMissileIndex, char isInternalHP, int missileIndex, int overTime = 0 );//	Missile		
int __fastcall MvP_Ranged( int playerIndex, int monsterIndex, int castLevel, int minDamage, int maxDamage, int effectIndex, char isInternalHP, int isLightningOrFirewall, uchar* attackBlockedPtr, uchar* missileReflected = 0 );//	Missile		
void __fastcall SetMissileDirection( int missileIndex, int direction	);//	Missile		
void __fastcall SetMissileSprite( int castIndex, uint spriteIndex	);//	Missile		
void __fastcall LoadMissileSprite( uint spriteIndex	);//	Missile		
void  LoadMissileGFX( );//	Missile		
void  FreePlayerMissileSprites( );//	Missile		
void __fastcall FreeMissileSprite( uint spriteIndex	);//	Missile		
void  FreeNotPlayerMissileSprites( );//	Missile		
void  InitMissiles( );//	Missile		
void __fastcall CastAbyssEnterExplode( int spellCastIndex, int a2, int a3, int a4, int a5, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRuneOfFire( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
bool __fastcall FoundCellForTargetInRadius( int spellCastIndex, int* targetRow, int* targetCol, int checkRadius	);//	Missile		
void __fastcall CastRuneOfLighting( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRuneOfGreatLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRuneOfImmolation( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRuneOfStone( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastReflect( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastBerserk( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
int __fastcall LimitPvMMeleeDamage( int playerIndex, int damage	);//	Player		int __usercall LimitPvMMeleeDamage@<edi>(int playerOffset@<ecx>, int result@<edi>)
void __fastcall CastSpawnMeatBall( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall SetMissileMovementDelta( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int castSpeed	);//	Missile		
void __fastcall PutMissile( int castIndex	);//	Missile		
void __fastcall CastJester( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell82_StealBottlesFromBelt( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastManaTrap( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMagicArrow( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
int __fastcall SelectMissileDirectionFrom16( int a1, int a2, int a3, int a4	);//	Missile		
void __fastcall CastArrow( int spellCastIndex, int casterX, int casterY, int targetX, int targetY, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile
void __fastcall CastSpecialArrowSpell( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall FoundFreeCellForTeleportPlayer( int a1, int a2, int a3	);//	Missile		
void __fastcall CastPhasing( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastWarp( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFirebolt( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMagmaBall( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall MoveMissile( int castIndex	);//	Missile		
void __fastcall CastSpell51_KRULL( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastTeleport( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastLightingNovaSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell70LightingWallSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell5_FireWallSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFireblast( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastExplosiveRuneExplode( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastExploder( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall MissileAttack( int castIndex, int minDamage, int maxDamage, char isInternalHP, int attackRow, int attackCol, char isPierce, int overTime = 0	);//	Missile		
bool __fastcall PvP_Ranged( int attackerPlayerIndex, int targetPlayerIndex, int minDamage, int maxDamage, int distance, int spellIndex, char mayBeIsTrap, uchar& attackBlocked	);//	Missile		
void __fastcall CastSpell88FireBall( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFireNovaSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastLightningAsArrow( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall Cast_Lightning_Segment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMagicExplosion( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastWeaponElementalDamage( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastAuraElementalGFX(int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage);//	Missile
bool __fastcall IsPointNearWarp( int row, int col	);//	Missile		
void __fastcall CastTownPortal( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFlashForward( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastNullSpell( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSomeAnotherFlashBk( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFlashBack( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastManaShield( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHellfireSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHydra( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastChainLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastBlood( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastBone( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMetlhit( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRunAttack( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSomeAnotherMonsterRun( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastArcaneStar( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastAcidMissile( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell29( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell59AcidPools( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastStoneCurse( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastGolem( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile
void __fastcall CastUnsummon( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile
void __fastcall CastEthereal( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell31_Emply( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFurySplash( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastApocExplode( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHealing( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastManaRecharge( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMagi( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHealOther( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastElemental( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
int __fastcall SelectMissileDirectionFrom8( int casterRow, int casterCol, int targetRow, int targetCol	);//	Missile		
void __fastcall CastIdentify( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
bool LocateWallSegments(int casterRow, int casterCol, int targetRow, int targetCol, int* row = 0, int* col = 0);
void __fastcall CastMagicWall( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall Cast_Fire_or_Lightning_Ring( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastInfravision( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall Cast_Forcewave( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastNova( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastFury( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastItemRepair( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastStaffRecharge( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastTrapDisarm( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastApocalypse( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastIncinerateSegment( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastIncinerate( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastChargedBolt( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHydramancerChargedBolt(int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage);//	Missile
void __fastcall CastChargedBoltArrow( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastHolyBolt( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpell77HolyBoltArrow( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastSpellRising( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall Cast62_RisingLight( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastTelekinesis( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastBoneSpirit( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastRedPortal( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastMonsterApoc( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		
void __fastcall CastTentacle(int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage);
void __fastcall ActTentacle(int missileIndex);
int __fastcall CastMissile(int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection_missileIndex, int baseMissileIndex, int casterType, int casterIndex, int damage, int spellLevel, int spellIndex, const int(&value)[8] = Missile::zeroValue, bool isSpriteToBeHidden = false, int flag = 0);//	Missile
bool __fastcall AttackCellByHydra( int spellCastIndex, int row, int col	);//	Missile		
void __fastcall ActSpawnMeatBall( int	);//	Missile		
void __fastcall ActRune( int	);//	Missile		
void __fastcall ActGolem( int	);//	Missile		
void __fastcall ActManaShield( int	);//	Missile		
void __fastcall ActMagicArrow( int	);//	Missile		
void __fastcall ActArrow( int castIndex	);//	Missile		
void __fastcall ActFireArcanBolt( int	);//	Missile		
void __fastcall ActLightningNovaSegment( int	);//	Missile		
void __fastcall ActSpell70LightningWallSegment( int	);//	Missile		
void __fastcall ActSpell51_KRULL( int castIndex	);//	Missile		
void __fastcall ActSpell59AcidPools( int spellIndex	);//	Missile		
void __fastcall ActFireWallSegment( int	);//	Missile		
void __fastcall ActFireblast( int	);//	Missile		
void __fastcall ActAbyssEnterExplodeFragment( int spellCastIndex	);//	Missile		
void __fastcall ActExploder( int spellCastIndex	);//	Missile		
void __fastcall ActSpell88FireBall( int spellCastIndex	);//	Missile		
void __fastcall ActLightning( int	);//	Missile		
void __fastcall ActLightningAsArrow( int	);//	Missile		
void __fastcall ActLightningSegment( int	);//	Missile		
void __fastcall ActTownPortal( int spellIndex	);//	Missile		
void __fastcall ActFlashForward( int	);//	Missile		
void __fastcall MordorsFlashAttackHack1( int missileIndex, int row, int col	);//	Missile		int __usercall MordorsFlashAttackHack1@<eax>(int a1@<eax>, int a2@<ecx>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
void __fastcall MordorsFlashAttackHack2( int missileIndex, int row, int col	);//	Missile		void __usercall MordorsFlashAttackHack2(int a1@<eax>, int a2@<ecx>, int a3@<ebx>, int a4@<edi>, int a5@<esi>)
void __fastcall ActSomeAnotherFlashFr( int _this	);//	Missile		
void __fastcall ActSomeAnotherFlashBk( int _this	);//	Missile		
void __fastcall ActFlashBack( int	);//	Missile		
void __fastcall ActReflect( int	);//	Missile		
void __fastcall ManaShieldAction( int a1	);//	Missile		
void __fastcall ActEthereal( int	);//	Missile		
void __fastcall ActForcewaveSegment( int	);//	Missile		
void __fastcall ActHydra( int	);//	Missile		
void __fastcall ActChainLightning( int	);//	Missile		отключена в 1.150 
void __fastcall ActSpecialEffect( int	);//	Missile		
void __fastcall ActWeaponElementalDamage( int	);//	Missile		
void __fastcall ActAuraElementalGFX(int);//     Missile
void __fastcall ActMagicExplosion( int	);//	Missile		
void __fastcall ActAcidSplash( int	);//	Missile		
void __fastcall ActTeleport( int	);//	Missile		
void __fastcall ActStoneCurse( int	);//	Missile		
void __fastcall ActApocExplode( int	);//	Missile		
void __fastcall ActMonstersRun( int	);//	Missile		
void __fastcall MonsterRunSpellMoving( int spellCastIndex	);//	Missile		
void __fastcall ActSomeAnotherMonsterRun( int	);//	Missile		
void __fastcall ActFireWall( int	);//	Missile		
void __fastcall ActFireRing( int	);//	Missile		
void __fastcall ActLightningRing( int	);//	Missile		
void __fastcall ActLightningWall( int	);//	Missile		
void __fastcall ActInfravision( int	);//	Missile		
void __fastcall ActApocalypse( int	);//	Missile		
void __fastcall ActForcewave( int	);//	Missile		
void __fastcall ActNova( int	);//	Missile		
void __fastcall ActSpecialArrowSpell( int	);//	Missile		
void __fastcall ActFury( int	);//	Missile		
void __fastcall ActIncinerateSegment( int	);//	Missile		
void __fastcall ActIncinerate( int	);//	Missile		
void __fastcall ActChargedBolt( int	);//	Missile		
void __fastcall ActHydramancerChargedBolt(int);//	Missile
void __fastcall ActHolyBolt( int	);//	Missile		
void __fastcall ActElemental( int	);//	Missile		
void __fastcall ActBoneSpirit( int	);//	Missile		
void __fastcall ActSpell62_RisingLight( int	);//	Missile		
void __fastcall ActRedPortal( int	);//	Missile		
void __fastcall CastFlaskThrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage );
void __fastcall ActFlaskThrow(int missileIndex);// , int playerIndex );
void  ProcessMissiles( );//	Missile		
void  SetupMissilesAnimationsAfterLoading( );//	Missile		
void __fastcall ClearMissileCell( int castIndex	);//	Missile		
void  InitMonsterSlots( );//	Monster		
int __fastcall AddMonsterTypeToDungeon( int baseMonsterIndex, MONSTER_PLACEMENT_FLAG mpFlag	);//	Monster		
void  GetDungeonMonsterTypes( );//	Monster		
void __fastcall LoadMonsterAnimation( int num_in_ctx	);//	Monster		
void __fastcall ColorMonsterSprite( int spriteIndex, int haveSecondAttack	);//	Monster		
void __fastcall CreateMonster( int monsterIndex, int direction, int monsterSpriteIndex, int row, int col	);//	Monster		
void  ClearMonsters( );//	Monster		
void __fastcall ClearMonsterWalkCounters( int a1	);//	Monster		
bool __fastcall IsCellFreeForMonster( int a1, int a2	);//	Monster		
void  OpenWeakUber( );//	Monster		
void  AddMonstersToPandemonium( );//	Monster		
void  InitMonsters( );//	Monster		
void  LoadBossesOnLevel( );//	Monster		
void __fastcall CreateBoss( int uniqMonsterIndex, int bossPackSpriteIndex, int packCount, int packFlag = 0	);//	Monster		
void __fastcall CreateBossBaseMonster( int monsterIndex, int spriteIndex, int row, int col	);//	Monster		
void  PlaceQuestMonsters( );//	Monster		
void __fastcall CreateMonstersPack( int spriteIndex, int count, int flag, int monsterBossIndex	);//	Monster		
void AddBossPack(BossPack bp);
void __fastcall AddQuestMonsters( ushort* dunData, int startRow, int startCol	);//	Monster		
void __fastcall DelMonster( int monsterIndexIndex	);//	Monster		
int __fastcall AddMonster( int row, int col, int direction, int spiteIndex, int newMonsterOnMap	);//	Monster		
void __fastcall CloneMonsterByDoppelganger( int monsterIndex	);//	Monster		
bool __fastcall ToGolemAttackType( int monsterIndex	);//	Monster		
bool __fastcall IsAttackTypes_1( int monsterIndex	);//	Monster		
void __fastcall MonsterLookAround( int monsterIndex	);//	Monster		
void __fastcall FixMonsterPosition( int monsterIndex, int direction	);//	Monster		
void __fastcall SetMonsterAnimation( int monsterIndex, MonsterAnim* animPtr, int direction	);//	Monster		
void __fastcall CheckMonsterUnderArch( int monsterIndex	);//	Monster		
void __fastcall ClearMonsterOnMap( int monsterIndex	);//	Monster		
void __fastcall KnockbackMonster( int monsterIndex, int orientation, int range	);//	Monster		
void __fastcall DamageMonsterByPlayer( int monsterIndex, int playerIndex, int damage, int dot = 0 	);//	Monster		
std::tuple<int, int> GetMonsterStunThresholdRange(const Monster& monster);//	MonsterStunThreshold		
std::tuple<int, int> GetPlayerStunThreshold(const Player& player);// Player stun threshold calculation
void __fastcall ScaleMonsterStunThreshold(int& monster_stun_threshold);//	MonsterStunThreshold		
int __fastcall GetMonsterLifeStunThreshold(const Monster& monster);//	MonsterStunThreshold		
int __fastcall RollMonsterStunThreshold(const Monster& monster);//	MonsterStunThreshold		
int __fastcall MonsterOrientationToTarget( int monsterIndex	);//	Monster		
void __fastcall KillMonsterByPlayer( uint monsterIndex, int playerIndex	);//	Monster		
void __fastcall KillPlayerByPlayerEffect(int victimIndex, int killerIndex);
void __fastcall MonsterKilledByPlayer(int monsterIndex, int playerIndex, bool sendMsg);//	Monster		
void __fastcall KillDiabloEffect( int monsterIndex, int a1	);//	Monster		
void __fastcall MonsterKillEffect( int monsterIndex, bool sendMsg	);//	Monster		
void __fastcall MonsterKilledByOtherPlayer( int monsterIndex, int row, int col, int playerIndex	);//	Monster		
int __fastcall MonsterStaying( int monsterIndex	);//	Monster		
int __fastcall MonsterWalkUp( uint monsterIndex	);//	Monster		
void __fastcall OffsetMonsterLight( uint monsterIndex	);//	Monster		
int __fastcall MonsterWalkDown( uint monsterIndex	);//	Monster		
int __fastcall MonsterWalkHorizontal( uint monsterIndex	);//	Monster		
bool Necropathy(int attackerIndex, int defenderIndex);
bool MinionCrits(int attackerIndex, int defenderIndex);
void __fastcall MvM_Melee( int attackerIndex, int defenderIndex, int hitChance, int minDamage, int maxDamage	);//	Monster		
void __fastcall DamageMonsterByMonster( int monsterIndex, int casterIndex, int damage );//	Monster		
void __fastcall KillMonsterByMonster( int attackerIndex, int targetIndex	);//	Monster
int __fastcall MonsterMeleeAttack( int a1	);//	Monster		
void __fastcall MvP_Melee( int monsterIndex, int playerIndex, int toHit, int minDamage, int maxDamage	);//	Monster		
int __fastcall MonsterRangedAttack( uint monsterIndex	);//	Monster		
int __fastcall MonsterSecondRangedAttack( uint monsterIndex	);//	Monster		
int __fastcall MonsterSecondMeleeAttack( int a1	);//	Monster		
bool __fastcall MonsterEndMeleeAttack( int monsterIndex	);//	Monster		
bool __fastcall SecondaryAnimation( uint monsterIndex	);//	Monster		
int __fastcall HealMode( uint monsterIndex );//	Monster		
int __fastcall MonsterTalk( int a1	);//	Monster		
void __fastcall MonsterJumpAroundTarget( uint monsterIndex	);//	Monster		
int __fastcall MonsterHit( uint monsterIndex	);//	Monster		
void __fastcall MayBe_KillPackBoss( uint monsterIndex	);//	Monster		
int  DoEnding( );//	Monster		
void  PrepDoEnding( );//	Monster		
int __fastcall MonsterHitToDeath( uint monsterIndex	);//	Monster		
int __fastcall MonsterRangedHit( uint monsterIndex	);//	Monster		
int __fastcall MonsterOrientTarget( uint monsterIndex	);//	Monster		
int __fastcall MonsterStoneCursed( uint monsterIndex	);//	Monster		
void __fastcall StartMonsterWalk( uint monsterIndex, int orientation	);//	Monster		
void __fastcall StartMonsterWalkUp( int monsterIndex, int speedX, int speedY, int rowDelta, int colDelta, int orient	);//	Monster		
void __fastcall StartMonsterWalkDown( int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int orient	);//	Monster		
void __fastcall StartMonsterWalkHorizontal( int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int deltaRowHz, int deltaColHz, int orient	);//	Monster		
void __fastcall BossPackActivation( uint monsterIndex	);//	Monster		
void __fastcall InterruptStoneState( int monsterIndex	);//	Monster		void __usercall InterruptStoneState(int monsterOffset@<eax>)
int __fastcall MonsterWalksTowardsTarget( int monsterIndex, int orientation	);//	Monster		
int __fastcall MakeMonsterPath( uint monsterIndex	);//	Monster		
int __fastcall MoveInOrientOrAbove( int monsterIndex, int orientation	);//	Monster		
bool __fastcall MoveToDirectionIfMay( int monsterIndex, int orientation	);//	Monster		
int __fastcall MonsterCircleTarget( int monsterIndex, int orientationToTarget, int& curOrientation	);//	Monster		
void __fastcall Ai_0_Zombie( uint monsterIndex	);//	Monster		
void __fastcall StartMonsterMeleeAttack(int monsterIndex, int dir = -1 );//	Monster	
void __fastcall Ai_2_Skeleton_Melee( uint monsterIndex	);//	Monster		
void __fastcall MonsterStartDelay( int monsterIndex, int delay	);//	Monster		
int __fastcall MonsterChangeTarget( int monsterIndex	);//	Monster		
void __fastcall Ai_24_Viper( uint monsterIndex	);//	Monster		
void __fastcall Ai_11_Bat( uint monsterIndex	);//	Monster		
void __fastcall Ai_3_ArcherRarelyRetreats( uint monsterIndex	);//	Monster		
int __fastcall StartMonsterRangedAttack( int monsterIndex, int spellEffect, int damage	);//	Monster		
void __fastcall Ai_1_MeleeRandomSecondAttackSwing( uint monsterIndex	);//	Monster		
void __fastcall StartMonsterSecondMeleeAttack( int monsterIndex	);//	Monster		
void __fastcall Ai_15_Hidden( uint monsterIndex	);//	Monster		
void __fastcall Monster_Fade_In( int a1, int a2, int a3	);//	Monster		
void __fastcall Monster_Fade_Out( int a1, int a2, int a3	);//	Monster		
void __fastcall Ai_17_Incinerator( uint monsterIndex	);//	Monster		
void __fastcall Ai_8_Fallen( uint monsterIndex	);//	Monster		
void __fastcall StartMonsterSecondAttack_4_orBlock( int monsterIndex, int orient	);//	Monster		
void __fastcall Ai_13_Butcher( uint monsterIndex	);//	Monster		
void __fastcall Ai_53_Exploder(uint monsterIndex);//	Monster		
void __fastcall AttackTypeMelee( int ecx0, int edx0	);//	Monster		
void __fastcall Ai_6_Goat_Melee( uint monsterIndex	);//	Monster		
void __fastcall RangerRetreatsOften( int a1, int a2, int a3	);//	Monster		
int __fastcall StartMonsterSecondRangedAttack( int monsterIndex, int spellEffect, int damage	);//	Monster		
void __fastcall Ai_7_Goat_Archer( uint monsterIndex	);//	Monster		
void __fastcall Ai_41_RangerRetreatsOften_ColdSnakes( uint monsterIndex	);//	Monster		
void __fastcall Ai_14_RangerRetreatsOften_ArcaneStar( uint monsterIndex	);//	Monster		
void __fastcall Ai_46_RangerRetreatsOften_ArcaneStarBlue( uint monsterIndex	);//	Monster		
void __fastcall Ai_35_RangerRetreatsOften_LichOrangeBlast( uint monsterIndex	);//	Monster		
void __fastcall Ai_36_RangerRetreatsOften_YelowBlast( uint monsterIndex	);//	Monster		
void __fastcall Ai_37_RangerRetreatsOften_FireArrow( uint monsterIndex	);//	Monster		fire arrows!
void __fastcall Ai_38_RangerRetreatsOften_RedBlast( uint monsterIndex	);//	Monster		
void __fastcall Ai_20_RangerRetreatsOften_AcidMissile( uint monsterIndex	);//	Monster		
void __fastcall Ai_32_RangerRetreatsOften_Firebolt( uint monsterIndex	);//	Monster		
void __fastcall Ai_33_RangerRetreatsOften_LightningArrow(uint monsterIndex);//	Monster		
void __fastcall Ai_49_RangerRetreatsOften_Fireblast(uint monsterIndex);//	Monster		
void __fastcall Ai_55_RangerRetreatsOften_Wyrm(uint monsterIndex);//	Monster		- ranger, spits acid (as primary attack)
void __fastcall Ai_4_Scavenger( uint monsterIndex	);//	Monster		
void __fastcall StartMonsterSecondAttack_1_2( int monsterIndex	);//	Monster		
void __fastcall Ai_12_Gargoyle( uint monsterIndex	);//	Monster		
void __fastcall Ai_56_Rammer(uint monsterIndex);
void __fastcall StartMonsterStoneMode( uint monsterIndex	);//	Monster		
void __fastcall Universal_RangeSecondAttack( uint monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility	);//	Monster		
void __fastcall Universal_OnlyRangeSecondAttack(uint monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility);
void __fastcall Ai_9_Universal_RangeSecondAttack_MagmaBall( uint monsterIndex	);//	Monster		
void __fastcall Ai_16_Universal_RangeSecondAttack_Lightning( uint monsterIndex	);//	Monster		
void __fastcall Ai_39_Universal_RangeSecondAttack_Physical_Blue_Blast( uint monsterIndex	);//	Monster		
void __fastcall Ai_40_Universal_RangeSecondAttack_ColdBlast( uint monsterIndex	);//	Monster		cold serpents
void __fastcall Ai_42_Universal_RangeSecondAttack_Firebolt( uint monsterIndex	);//	Monster		
void __fastcall Ai_43_RangerRetreatsOften_Acid_Arrow( uint monsterIndex	);//	Monster		
void __fastcall Ai_44_RangerRetreatsOften_Cold_Arrow( uint monsterIndex	);//	Monster		
void __fastcall Ai_45_RangerRetreatsOften_Arcane_Arrow( uint monsterIndex	);//	Monster		
void __fastcall Ai_47_Universal_RangeSecondAttack_Bluestar(uint monsterIndex);//	Monster		orange star
void __fastcall Ai_48_Universal_RangeSecondAttack_Yellowstar(uint monsterIndex);//	Monster		
void __fastcall Ai_50_Universal_RangeSecondAttack_Fireblast(uint monsterIndex);//	Monster		
void __fastcall Ai_51_Universal_RangeSecondAttack_Arcanestar(uint monsterIndex);//	Monster		
void __fastcall Ai_58_Universal_RangeSecondAttack_Psychic(uint monsterIndex);//	Monster		
void __fastcall Ai_57_Hybrid_ArcaneStar(uint monsterIndex);
void __fastcall Ai_52_Universal_RangeSecondAttack_SnowBolt(uint monsterIndex);//	Monster		
void __fastcall Ai_54_Universal_RangeSecondAttack_Charged_Bolt(uint monsterIndex);//	Monster		
void __fastcall Ai_19_Universal_RangeSecondAttack_AcidMissile( uint monsterIndex	);//	Monster		
void __fastcall Ai_27_Universal_RangeSecondAttack_Apoc( uint monsterIndex	);//	Monster		
void __fastcall Ai_59_Tentaclist(uint monsterIndex);
void __fastcall Universal_ShortRangeSecondAttack( uint monsterIndex, int spellEffect, int damage	);//	Monster		
void __fastcall Ai_26_Universal_ShortRangeSecondAttack_Incinerate( uint monsterIndex	);//	Monster		
void __fastcall Ai_21_Golem( uint monsterIndex	);//	Monster		
void __fastcall Ai_10_SkeletonKing( uint monsterIndex	);//	Monster		
void __fastcall Ai_5_ChargeAttack( uint monsterIndex	);//	Monster		
void __fastcall Ai_34_HorkDemon( uint monsterIndex	);//	Monster		
void __fastcall Ai_25_Mage( uint monsterIndex	);//	Monster		
void __fastcall Ai_18_Garbud( uint monsterIndex	);//	Monster		
void __fastcall Ai_22_Zhar( uint monsterIndex	);//	Monster		
void __fastcall Ai_23_Snotspill( uint monsterIndex	);//	Monster		
void __fastcall Ai_28_Lazarus( uint monsterIndex	);//	Monster		
void __fastcall Ai_29_Lazarus_s_Slut( uint monsterIndex	);//	Monster		
void __fastcall Ai_30_Lachdanan( uint monsterIndex	);//	Monster		
void __fastcall Ai_31_Warlord( uint monsterIndex	);//	Monster		
void  RemoveKilledMonsters( );//	Monster		
void  ProcessMonsters( );//	Monster		
void  FreeMonsterResources( );//	Monster		
int __fastcall MayMoveMonsterToDirection( int monsterIndex, int direction, int range = 1	);//	Monster		
int __fastcall IsCellNotBlockSight( int row, int col	);//	Monster		
int __fastcall IsCellNotBlockWalking( int row, int col	);//	Monster		
int __fastcall CheckLineWithTwoArgumentsCheckFunction( int(__fastcall *checkFunct )( int, int ), int startRow, int startCol, int endRow, int endCol	);//	Monster		
int __fastcall CheckCastInSight( int a1, int a2, int a3, int a4	);//	Monster		
int __fastcall CheckLineWithThreeArgumentsCheckFunction( int(__fastcall *callingFunction )( int, int, int ), int monsterIndex, int monsterRow, int monsterCol, int targetRow, int targetCol	);//	Monster		
int __fastcall MonsterAnimIndex(int curAction);
void __fastcall PrepareLoadedMonster( uint monsterIndex	);//	Monster		
void __fastcall BewareNearMonsters( int row, int col	);//	Monster		
void __fastcall ShowMonsterInfo( int monsterIndex	);//	Monster		
void  ShowBossHP( );//	Monster		
void  BossShowResImun( );//	Monster		
void __fastcall ChargeAttackImpact( uint castIndex, int castRow, int castCol	);//	Monster		
int __fastcall CheckCellForMeleeMonsterSafeMove( int monsterIndex, int row, int col	);//	Monster		
uchar __fastcall CheckCellForMonsterResist( int monsterIndex, int row, int col	);//	Monster		
int __fastcall CheckCellForSomeMonsterSafeMove( int monsterIndex, int row, int col	);//	Monster		
int __fastcall CheckCellForMonsterCanInteractWithDoorsSafeMove( int monsterIndex, int row, int col	);//	Monster		
int __fastcall IsMonsterSummonable( int baseMonsterIndex	);//	Monster		
int __fastcall IsMonsterClan( int baseMonsterIndex	);//	Monster		
void __fastcall SummonMinion( int row, int col, int orientation	);//	Monster		
void __fastcall PutAmbushMonsterAndStartAttack( int monsterIndex, int row, int col, int orientation	);//	Monster		
int __fastcall ActivateAmbushMonster( int monsterIndex, int row, int col	);//	Monster		
int  AddAmbushMonster( );//	Monster		
void __fastcall TalkWithMonster( uint monsterIndex	);//	Monster
void __fastcall MakeGolem( unsigned int summonIndex, int row, int col, int spellIndex	);//	Monster
void __fastcall AwakeSummon( uint summonIndex, int row, int col, int currentLife, const DSummonStr& params, bool restoring );
bool __fastcall MonsterTalking( uint monsterIndex	);//	Monster		
int __fastcall CheckMonsterPhaseOrBlock( uint monsterIndex, int* isMonsterHit	);//	Monster		
int __fastcall encode_enemy( int a1 );//	Monster
void __fastcall decode_enemy( int monsterIndex, int targetIndex );//	Monster
void __fastcall PlayVideo( char *Str, int a2	);//	Monster		
int __stdcall MovieWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam	);//	Monster		
bool IsPlayerSummon(int monsterIndex);
void ClearMapFromInvalidMonsters();
bool __fastcall MpqSetAttributes( char* archiveName, bool hide	);//	MpqSave		
void __fastcall MpqUpdateCreationTimeStamp( char* lpFileName, int heroSlot	);//	MpqSave		
bool __fastcall RegGetStamps( FILETIME* ftime, uint size	);//	MpqSave		
void __fastcall XorTimeStamp( FILETIME* ftime	);//	MpqSave		
void __fastcall RegSetStamps( FILETIME* ftime, uint size	);//	MpqSave		
void __fastcall MpqDeleteFile( char* fineName	);//	MpqSave		
void __fastcall AddFreeBlock( uint offset, uint size	);//	MpqSave		
BlockEntry* __fastcall GetFreeBlock(uint* pdwBlockIndex);
uint __fastcall SearchHashName( char *aStr	);//	MpqSave		
uint __fastcall SearchHashEntry( uint hashIndex, uint hash1, uint hash2, uint lcid	);//	MpqSave		
void __fastcall MpqDeleteFiles( int( __stdcall *getFileNames )( int, char(&)[MAX_PATH] )	);//	MpqSave		
bool __fastcall MpqAddFile( char* aStr, uchar *data, uint len	);//	MpqSave		
BlockEntry* __fastcall InsertIntoHash(char* pszName, BlockEntry* pBlk, uint dwBlock);
bool __fastcall WriteFileData( char* fileName, uchar* data, uint len, BlockEntry* block	);//	MpqSave		
uint __fastcall MpqGetFreeSpace( uint size, uint* sizeAlloc	);//	MpqSave		
void __fastcall MpqRenameFile( char* pszOld, char* pszNew	);//	MpqSave		
bool __fastcall MpqFileExists( char* a1	);//	MpqSave		
bool __fastcall MpqOpenArchive(char * pszArchive, bool bHide, uint dwChar);
bool __fastcall ReadMpqFileHdr( FullHeader* hdr, uint* nextFileStart	);//	MpqSave		
void __fastcall CloseSaveArchive( char* archiveName, bool clearHashBuffer, int heroSlot	);//	MpqSave		
void __fastcall MPQUpdateLastWriteTimeStamp( char* lpFileName, int heroSlot	);//	MpqSave		
bool __fastcall MpqCloseArchive( char* saveArchivePath, bool clearHashTables, int heroSlot	);//	MpqSave		
bool  WriteMpqFileHeader( );//	MpqSave		
bool  WriteBlockTable( );//	MpqSave		
bool  WriteHashTable( );//	MpqSave		
bool  MpqSetEOF( );//	MpqSave
void  SetupTimingCritSection( );//	Net		
void __fastcall nthread_terminate_game( char* errorText );//	Net		отдельный сетевой поток
uint __fastcall NThreadFillSyncQueue( uint netTurnCount, int turnsDelta );//	Net		отдельный сетевой поток
int __fastcall NThreadMsgCheck( int* a1 );//	Net		отдельный сетевой поток
void  NThreadSetDeltaRequest( );//	Net		
void __fastcall NetThreadStart( int ecx0	);//	Net		
uint __stdcall NetSyncThread( void* arg1	);//	Net		отдельный сетевой поток
void  NetThreadStop();//	Net
void __fastcall ResetFrameSyncCritSection( int leave	);//	Net		
bool  NThreadRunGameLoop(bool reloop);//	Net		
void  LoadObjectSprites( );//	Object		
void  FreeObjectsSprites( );//	Object		
int __fastcall AddRandFromRangeObjectsOnMap( int ecx0, int a2, BASE_OBJECT a1	);//	Object		
char __fastcall IsCellFree( int row, int col	);//	Object		
void __fastcall AddDoorsAndLibrazObjectInChurch( int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
int __fastcall AddCryptDoors( int a1, int a2, int a3, int a4	);//	Object		
int __fastcall AddDoorsObjectInCatacomb( int aStartRow, int aStartCol, int EndRow, int aEndCol	);//	Object		
void __fastcall InitObjects();//	Object		
void __fastcall AddSomeTwoTilesSizeObjectOnMap( int minCount, int maxCount, BASE_OBJECT baseObjectIndex	);//	Object		
void __fastcall AddRandFromRangeCountOfObjects( int minCount, int maxCount, BASE_OBJECT baseObjectIndex	);//	Object		
void  ClearAllObjects( );//	Object		
int  AddButcherObjects( );//	Object		
void  AddSomeObjectsOnMapWithPoisonWaterQuest( );//	Object		
char __stdcall AddQuestBookThatChangeMap( int a1, int a2, int a3, int a4, int a5, int a6, int a7	);//	Object		
int  AddSomeBarrelsOnLevel( );//	Object		
int __fastcall AddCaveDoors( int a1, int a2, int a3, int a4	);//	Object		
void  AddCatacombTorches( );//	Object		
char __fastcall CheckCatacombForTorch( int a1, int a2	);//	Object		
void  TrapingObjectsWithWallTraps( );//	Object		
bool __fastcall IsGoodPlaceForWallTrap( int row, int col	);//	Object		
void  TrapingChestsWithContactTrap( );//	Object		
void __fastcall LoadMapObjects( uchar* dun, int row, int col	);//	Object		
void  AddDiabloLevelObjects( );//	Object		
int __fastcall AddDiabloDunObject( int a1, int a2, int aCol, int a4, int a5, int a6, int a7, int a8	);//	Object		
void __fastcall PutCryptQuestBookOnMap( int bookIndex	);//	Object		
void  PutQuestBookOnMap( );//	Object		
void  AddHellDecor( );//	Object		
void __fastcall AddHellArch( int a1	);//	Object		
void  AddLazarusVileStand( );//	Object		
void __fastcall SetMapObjects( int dunData, int startRow, int startCol	);//	Object		
void __fastcall SetChangeRect( int objectIndex, int startRow40, int startCol40, int endRow40, int endCol40, int bookIndex	);//	Object		
void __fastcall SetObjectSpeechIndex( int objectIndex, int speechIndex	);//	Object		
void __fastcall AddAncientTomeOrBookOfVileness( int objectIndex	);//	Object		
void __fastcall AddMcirl( int objectIndex	);//	Object		
void __fastcall SetObjectSeed2( int objectIndex	);//	Object		
void __fastcall AddPedestalOfBlood( int objectIndex	);//	Object		
void  AddMushroomPathObject( );//	Object		
void __fastcall FindFreeMapRegion( int radius, int& row, int& col	);//	Object		
void  AddSlainHeroOnMap( );//	Object		
void __fastcall AddQuestBook86( BASE_OBJECT objectType, int bookIndex, int row, int col	);//	Object		
void __fastcall SetupObject( int objectIndex, int row, int col, BASE_OBJECT baseObjectIndex	);//	Object		
void __fastcall MaybeInitQuestBook( int objectIndex, int bookIndex	);//	Object		
void __fastcall AddObject( BASE_OBJECT baseObject, int row, int col	);//	Object		
void __fastcall AddChurchDoor( int objectIndex, int row, int col, int baseObjectIndex	);//	Object		
void __fastcall AddMythicalBook( int objectIndex	);//	Object		
void __fastcall AddChest( int objectIndex, int baseObject	);//	Object		
void __fastcall AddCatacombDoor( int objectIndex, int row, int col, int baseObject	);//	Object		
void __fastcall AddCaveDoor( int objectIndex, int row, int col, int baseObject	);//	Object		
void __fastcall AddSarcofagusOnMap( int objectIndex	);//	Object		
void __fastcall AddFlameOnMap( int objectIndex	);//	Object		
void __fastcall AddLever2OnMap( int objectIndex	);//	Object		
void __fastcall AddTrapHoleOnMap( int objectIndex	);//	Object		
void __fastcall PutSomeLightSorce( int objectIndex, int lightLevel	);//	Object		
void __fastcall AddBarrelOnMap( int objectIndex	);//	Object		
void __fastcall AddShrineOnMap( int objectIndex	);//	Object		
void __fastcall AddBookCaseOnMap( int objectIndex	);//	Object		
void __fastcall SetObjectSeed( int objectIndex	);//	Object		
void __fastcall AddArmorRack( int objectIndex	);//	Object		
void __fastcall AddFontain( int objectIndex	);//	Object		
void __fastcall AddDecapitatedBody( int objectIndex	);//	Object		
void __fastcall AddQuestBook( int objectIndex	);//	Object		
void __fastcall AddWeaponRack( int objectIndex	);//	Object		
void __fastcall AddTNude( int objectIndex	);//	Object		
void __fastcall LightObjectAction( int objectIndex, int lightRadius	);//	Object		
void __fastcall ReadBookInLazarusAltar( int objectIndex	);//	Object		
void __fastcall StopAnimationAfterComplete( int objectIndex	);//	Object		
void __fastcall DoorAction( int objectIndex	);//	Object		
void __fastcall StopAnimationAfterComplete2( int objectIndex	);//	Object		
void __fastcall FireAllSomeFlames( int baseObjectIndex, int flameGroupIndex	);//	Object		
void __fastcall ActivateFlame( int objectIndex	);//	Object		
void __fastcall ActivateWallTrap( int objectIndex	);//	Object		
void  ProcessObjects( );//	Object		
void __fastcall DeleteObjectOnMap( int deletedObject, int a2	);//	Object		
void __fastcall BurnCrossDamage( int objectIndex	);//	Object		
int  hack_ModifyTrapDamage( );//	Object		int __usercall hack_ModifyTrapDamage@<edx>()
void __fastcall SetDungeonMapNumber( int row, int col, int DungeonMapNumber	);//	Object		
void  ChangeAllPlayersVisibility( );//	Object		
void __fastcall ActivateChurchDoorCollateralDiagonal( int a1, int a2, char a3	);//	Object		
void __fastcall ApplyChangeFromDunMapToTileMap( int row, int col	);//	Object		
void __fastcall ChangeDungeonMapWithActivateDoor( int objectIndex, int row, int col	);//	Object		
void __fastcall ActivateChurchDoorMainDiagonal( int playerIndex, int objectOnMapIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivateCatacombDoorMainDiagonal( int playerIndex, int objectIndex, int isCurrentPlayer	);//	Object		
void __fastcall MonstersInteractWithDoors( int monsterIndex	);//	Object		
void __fastcall ActivateCatacombDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivateCaveDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivateCaveDoorMainDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ChangeMapRectAfterScriptEvent( int startRow40, int startCol40, int endRow40, int endCol40	);//	Object		
void __fastcall SetNewTileNumber( int row40, int col40, uint someTileNumber	);//	Object		
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapChurch( int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapCatacomb( int minRow112, int minCol112, int maxRow112, int maxCol112	);//	Object		
void __fastcall ChangeMapRectAfterScriptEvent2( int startRow40, int startCol40, int endRow40, int endCol40	);//	Object		
void  DisarmTrap( int playerIndex, int objectIndex	);//	Object		
BASE_ITEM __fastcall FindBaseItemWithMagicCode( int findableMagicCode	);//	Object		
void __fastcall OperateObject( int playerIndex, int objectIndex, int telekines	);//	Object		
void __fastcall ActivateChurchDoor( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivateLever( int playerIndex, int objectsOnMapIndex	);//	Object		
void __fastcall ActivateAncientTomeOrBookOfVileness( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateQuestBook( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateMythicalBook( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateChest( int playerIndex, int objectIndex, int isCurrentIndex	);//	Object	
void __fastcall OperateChest(int playerIndex, int objectIndex, int isCurrentIndex, int telekines);//	Object	
void __fastcall ActivateMushroomPatch( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateLargeOgdenQuestChest( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateSlainHero( int playerIndex, int objectIndex	);//	Object		заменено на 0x00706CE0 ActivateSlainHero_TH
int  GetUniqMonsterSound( int monsterIndex	);//	Monster		int __usercall GetUniqMonsterSound@<ecx>(int monsterIndex@<esi>)
void __fastcall ActivateLever2( int objectIndex	);//	Object		
void __fastcall ActivateSarcofagus( int playerIndex, int objectIndex, unsigned __int8 a3, int telekines = 0	);//	Object		
void __fastcall ActivateCatacombDoor( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivateCaveDoor( int playerIndex, int objectIndex, char isCurrentPlayer	);//	Object		
void __fastcall ActivatePedestalOfBlood( int playerIndex, int objectIndex	);//	Object		
void __fastcall ApplyShrineEffect( int playerIndex, int objectIndex, int soundIndex	);//	Object		
void __fastcall ActivateLibraryBook( int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
void __fastcall ActivateBookcase( int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
void __fastcall ActivateDecapitatedBody( int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
void __fastcall ActivateArmorRack( int playerIndex, int objectIndex, uchar isCurrentPlayer	);//	Object		
void __fastcall ActivateGoatShrine( int playerIndex, int objectIndex, int soundIndex	);//	Object		
int  GetRandomValidShrineEffect( );//	Object		
void __fastcall ActivateCauldron( int playerIndex, int objectIndex, int soundIndex	);//	Object		
void __fastcall ActivateFountain( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateWeaponRack( int playerIndex, int objectIndex, int isCurrentPlayer	);//	Object		
void __fastcall ActivateQuestBook2( int playerIndex, int objectIndex	);//	Object		
void __fastcall ActivateVileStand( int playerIndex, int objectIndex	);//	Object		
void __fastcall SyncOpObject( int playerIndex, int netCommand, int objectIndex );//	Object
void __fastcall ActivateChurcheDoorFromNetwork( int playerIndex, int netCommand, int objectIndex	);//	Object		
void __fastcall ActivateCatacombDoorFromNetwork( int playerIndex, int netCommand, int objectIndex	);//	Object		
void __fastcall ActivateCaveDoorFromNetwork( int playerIndex, int netCommand, int objectIndex	);//	Object		
void __fastcall DamageDestroyableOnMapObject( int playerIndex, int objectIndex	);//	Object		
void __fastcall InteractWithCrucifiedSkeleton(int playerIndex, int objectIndex	);//	Object		
void __fastcall InteractWithBarrel( int playerIndex, int objectIndex, int damage, int oneHitDestruction, int a5	);//	Object		
void __fastcall SyncBreakObj( int playerIndex, int objectOnMapIndex );//	Object
void __fastcall SetDunTileForObject( int objectIndex	);//	Object		
void __fastcall WithDoors_1( int objectIndex	);//	Object		
void __fastcall WithCrucifiedSkeleton( int objectIndex	);//	Object		
void __fastcall WithLever( int objectIndex	);//	Object		
void __fastcall WithQuestBooks( int objectIndex	);//	Object		
void __fastcall WithPedestalOfBlood( int objectIndex	);//	Object		
void __fastcall WithDoors_2( int objectIndex	);//	Object		
void __fastcall WithDoors_3( int objectIndex	);//	Object		
void __fastcall IdentifyObjectOnMapInFocus( int objectIndex	);//	Object		
void  Floor24QuestMapObjectsGeneration( );//	DiabloQuest		
void __fastcall AddQuestBookOnMap( int bookIndex, int row112, int col112	);//	DiabloQuest		
bool __fastcall ReadUberBook( int a1	);//	DiabloQuest		
void  OpenUberDiabloRoom( );//	DiabloQuest		
void  LoadOpennedUberRoom( );//	DiabloQuest		
void  AddQuestLeverOn24Level( );//	DiabloQuest		
void __fastcall SaveItem( LastItemInfo* itemInfo, const Item *item	);//	SaveInfo		
void __fastcall SavePlayerInfo( LastPlayerInfo* playerInfo, int playerIndex	);//	SaveInfo		
template<class ItemInfo> void LoadItem(ItemInfo* itemInfo, Item* item, bool keepCharges = true);//	SaveInfo		
int __fastcall RandomizeGoldsSeed( Player* player	);//	SaveInfo		
template <typename Pi> bool __fastcall LoadPlayerInfo(Pi* playerInfo, int playerIndex, int isNetPlayer);
template<> bool __fastcall LoadPlayerInfo<uchar>( uchar* playerInfo, int playerIndex, int isNetplayer	);//	SaveInfo		
void SavePaletteSettings( );//	Palette		
void  SetScreenSurfacePalette( );//	Palette		
void InitGammaCorrectionValue( );//	Palette		
uchar*  InitSystemPalette( );//	Palette		
void __fastcall LoadPalette( char* paletteFileName	);//	Palette		
void __fastcall SelectRandomLevelPalette( int dungeonType	);//	Palette		
void  RestoreScreenSurface( );//	Palette		
void  IncPaletteGamma( );//	Palette		
void  UpdatePalette( );//	Palette		
void __fastcall ApplyGammaToPalette( PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount	);//	Palette		
void __fastcall ApplyContrastToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount);
void __fastcall ApplyGammaAndContrastToPalette(PALETTEENTRY *screenPalette, PALETTEENTRY *basePalette, int paletteCount);
void  DecPaletteGamma( );//	Palette		
int __fastcall SetGammaByPos( int gammaPos	);//	Palette		
int __fastcall SetContrastByPos( int contrastPos	);//	Palette		
void  BlackPalette( );//	Palette		
void __fastcall SetFadeLevel( int brightness	);//	Palette		
void* __fastcall PaletteFadeIn( int delta	);//	Palette		
void __fastcall PaletteFadeOut( int delta	);//	Palette		
void  AnimateCavePalette( );//	Palette		jmp-call, учесть при сверке
void  AnimateCryptPalette( );//	Palette		jmp-call, учесть при сверке
void  AnimateAbyssPalette( );//	Palette		jmp-call, учесть при сверке
void __fastcall ApplyAnimFilePalette( int animPalReserve	);//	Palette		
int __fastcall FindPath( int ( __fastcall *stepFunc )( int, int, int ), int monsterIndex, int startRow, int startCol, int targetRow, int targetCol, char* steps	);//	Path		
int __fastcall GetPerimeter( int aDestX, int aDestY, int X, int Y	);//	Path		
int __fastcall Step_IsOnLine( PathStep *Step, int X, int Y	);//	Path		
PathStep*  GetNextStep( );//	Path		
bool __fastcall CheckBlockStep( PathStep* step, int row, int col	);//	Path		
int __fastcall GenerateSuccessors( int( __fastcall *aFuncPtr )( int, int, int ), int a2, PathStep *aStep, int a4, int a5	);//	Path		
int __fastcall NextAstarWave( PathStep *aStep, int a2, int a3, int a4, int a5	);//	Path		
PathStep* __fastcall FindCurrentStepForXY( int row, int col	);//	Path		
PathStep* __fastcall FindPrevStepForXY( int row, int col	);//	Path		
void __fastcall InsertNextPathStep( PathStep *LastPtr	);//	Path		
void __fastcall LookNextStep( PathStep* a1	);//	Path		
int __fastcall AddToDword300Stack( PathStep* a1	);//	Path		
PathStep*  DeleteFromDword300Stek( );//	Path		
PathStep*  NewStepFromPool( );//	Path		
void  InitSaveDirectory( );//	CharSlots		
void __fastcall CheckDiskFreeSpace( char* dwInitParam	);//	CharSlots		
void  UpdatePlayerFile( );//	CharSlots		
int __fastcall GetSlotByHeroName( char *aCharName	);//	CharSlots		
void __fastcall SaveCharacter( LastPlayerInfo* heroData	);//	CharSlots		
void __fastcall WriteHotkeysDataToSave( void ); // CharSlots SaveHotkeysToSave
bool __fastcall ReadHotkeysInfoFromSave(HANDLE save); // CharSlots LoadHotkeysFromSave
void __fastcall WriteAdditionalHeroDataToSave(void); // Stash tabs
bool __fastcall ReadAdditionalHeroDataFromSave(HANDLE save); // Stash tabs
bool __fastcall OpenArchiveWrite( bool notUsedFlag, int heroSlot	);//	CharSlots		
int SaveType();
void __fastcall GetSaveArchivePath( char (&saveFullName)[MAX_PATH], int stringSize, int slotNum, int isHellfire	);//	CharSlots		
void __fastcall CloseArchiveWrite( bool clearHashTables, int slotNum	);//	CharSlots		
void  ReleasePlayerFile( );//	CharSlots		
void __fastcall InitCharData( Player& playerPtr, CharSaveInfo& charInfo, int isHellfire	);//	CharSlots		 
int __stdcall LoadCharSlots( void( __stdcall *allocFunc )( CharSaveInfo* )	);//	CharSlots		
uchar* __fastcall ReadPlayerInfo( HANDLE saveHandle	);//	CharSlots		
HANDLE __fastcall OpenArchive( int* ecx_unused, int slotIndex	);//	CharSlots		
void __fastcall CloseArchiveRead( HANDLE handle	);//	CharSlots		
int __fastcall CheckSavefileSignature( HANDLE archiveHandle	);//	CharSlots		
int __stdcall GetStartingStatsByClass( uint classId, ushort* stats	);//	CharSlots		
int __fastcall GetClassByClass( int charClass	);//	CharSlots		
int __stdcall UiCreateHero( CharSaveInfo* newSaveInfo	);//	CharSlots		
int __stdcall GetPlayerFileNames( int keyIndex, char (&key)[MAX_PATH]	);//	CharSlots		
int __stdcall DeleteCharSlot( CharSaveInfo* saveInfo	);//	CharSlots		
void  LoadCurrentPlayerInfo( );//	CharSlots		
void __fastcall CreateSaveLevelName( char (&key)[MAX_PATH]	);//	CharSlots		
void __fastcall CreateLoadLevelName( char (&key)[MAX_PATH]	);//	CharSlots		
void  DestroyTempSaves( );//	CharSlots		
int __stdcall GetTempSaveNames( int keyIndex, char (&key)[MAX_PATH]	);//	CharSlots		
void  MoveTempSavesToPermanent( );//	CharSlots		
int __stdcall GetPermSaveNames( int keyIndex, char (&key)[MAX_PATH]	);//	CharSlots		
void __fastcall WriteSaveFile( char* fileName, uchar* data, uint realSaveSize, int saveSizeRounded	);//	CharSlots		
void* __fastcall ReadSaveFile( char *decodeKey, uint *loadedSize, int loadSwapWeapon	);//	CharSlots		
void __fastcall SplitAnimationPointerToDirections( char* animPtr, char** dirAnimPtr	);//	Player		
void __fastcall LoadPlayerGFX( uint playerIndex, int loadingMask	);//	Player		
void __fastcall InitPlayerGFX( uint playerIndex	);//	Player		
void __fastcall InitPlrGFXMem( uint playerIndex	);//	Player		
uint __fastcall SelectPlrAnimation( char* anim	);//	Player		
void __fastcall FreePlayerGFX( uint playerIndex	);//	Player		
void __fastcall NewPlayerAnimation( int playerIndex, char* animation, int framesCount, int animationDelay, int animationCL2filePtr	);//	Player		
void __fastcall ClearPlrPVars( int playerIndex	);//	Player		
void __fastcall SetPlrAnims( uint playerIndex	);//	Player		
void __fastcall ClearAuras( Player& player	);//	Player		
void __fastcall CreatePlayer( int playerIndex, char classId, char subclassId, char specializationId, GAME_MODE gameMode, __int64 gameChanger, decltype(Player::traits) traits	);//	Player
void SetCols(int playerIndex);
int __fastcall LevelPointsToMax( int playerIndex	);//	Player		
void __fastcall NextPlayerLevel( int playerIndex	);//	Player		
void __fastcall AddPlayerExperience( int playerIndex, i64 monsterXp, int row = 0, int col = 0	);//	Player		
void __fastcall AddXpForMonsterKill( int monsterLevel, i64 monsterXp, char attacker, int monsterIndex	);//	Player		
void __fastcall InitPlayer( uint playerIndex, int loadFromIngameMenu	);//	Player		
void InitMultiView( );//	Player		
void __fastcall CheckPlayerUnderArch( int playerIndex, int a2	);//	Player		
int __fastcall IsCellBlocking( int row, int col, uchar canFly = 0 );//	Player		
int __fastcall IsDirectionClearForPlayer( int playerIndex, int direction	);//	Player		
void __fastcall SomeWithTeleportAndTransparent2( int row, int col	);//	Player		
void __fastcall SomeWithTeleportAndTransparent( int row, int col	);//	Player		
void __fastcall SetPlayerOld( int playerIndex	);//	Player		
void __fastcall FixPlayerLocation( int playerIndex, int orient, bool stunWalkDown = false	);//	Player		
void __fastcall StartStand( int playerIndex, int direction	);//	Player		
void __fastcall StartWalkStand( int playerIndex	);//	Player		
void __fastcall PM_ChangeLightOff( int playerIndex	);//	Player		
void __fastcall PM_ChangeOffset( int playerIndex	);//	Player		
void __fastcall StartWalkUp( int playerIndex, int xVel, int yVel, int changeRow, int changeCol, int direction, int scrollDir	);//	Player		
void __fastcall StartWalkDown( int playerIndex, int xVel, int yVel, int xOfs, int yOfs, int changeRow, int changeCol, int direction, int scrollDir	);//	Player		
void __fastcall StartWalkHorizontal( int playerIndex, int xVel, int yVel, int xOfs, int yOfs, int changeRow, int changeCol, int changeRow2, int changeCol2, int direction, int scrollDir	);//	Player		
void __fastcall StartAttack( int arglist, int orientationToTarget	);//	Player		
void __fastcall StartRangeAttack( int arglist, int a2, int a3, int a4	);//	Player		
void __fastcall StartPlayerBlock( uint playerIndex, int orientation	);//	Player		
void __fastcall StartSpell( int arglist, int a2, int a3, int a4	);//	Player		
void __fastcall FixPlrWalkTags( int playerIndex	);//	Player		
char* __fastcall RemovePlayerFromMap( int playerIndex	);//	Player		
void __fastcall StartPlayerHit( int playerIndex, int damage, int needStun	);//	Player		
void __fastcall StartPlayerKill( int playerIndex, int earFlag	);//	Player		
int  ModifyFireBoltDamage( );//	Player		int __usercall ModifyFireBoltDamage@<edx>()
int __fastcall PlayerDeadItem( uint playerIndex, Item *item    );//	Player		
void __fastcall StripTopGold( int playerIndex	);//	Player		
int __fastcall TryToDie( int playerIndex, int earFlag	);//	Player		
void __fastcall RemovePlayerMissiles( int playerIndex	);//	Player		
void __fastcall InitLevelChange( int playerIndex	);//	Player		
bool __fastcall StartNewLvl( uint playerIndex, uint msg, DUNGEON dungeonLevel	);//	Player		
void __fastcall RestartInTown( uint playerIndex	);//	Player		
void __fastcall StartWarpDungeon( int playerIndex, int otherPlayerIndex	);//	Player		
int __fastcall PM_DoNothing( int playerIndex	);//	Player		
int __fastcall PM_DoWalkUp( int playerIndex	);//	Player		
int __fastcall PM_DoWalkDown( int playerIndex	);//	Player		
int __fastcall PM_DoWalkHorizontal( int playerIndex	);//	Player		
bool __fastcall WeaponDur( uint playerIndex, int durDecrParam	);//	Player
int __fastcall PvM_Melee( int playerIndex, uint monsterIndex, bool isSweepAttack = false, bool isThorns = false, int minDamage = -1, int maxDamage = -1 );//	Player
int __fastcall PvP_Melee( uint attackerPlayerIndex, int targetPlayerIndex	);//	Player		
bool __fastcall MeleeAttackBarrelObject( int playerIndex, int row, int col	);//	Player		
bool __fastcall PM_DoAttack( uint playerIndex	);//	Player		
bool __fastcall PM_DoRangedAttack( int playerIndex	);//	Player
int __fastcall ThrowableMissile( Item* flask );
bool __fastcall PM_DoThrow( int playerIndex );//	Player
void __fastcall ShieldDur( int playerIndex	);//	Player
int __fastcall PM_DoBlock( int playerIndex	);//	Player		
int __fastcall PM_DoSpell( uint playerIndex	);//	Player		
int __fastcall PM_DoGotHit( int playerIndex	);//	Player		
void __fastcall ArmorDur( uint playerIndex, int durDecrParam	);//	Player		
bool __fastcall PM_DoDeath( int playerIndex	);//	Player		
void __fastcall CheckNewPath( int playerIndex	);//	Player		
bool __fastcall PlrDeathModeOK( int playerIndex	);//	Player		
void  ValidateCurPlayer( bool forceItemsRecalc = false );//	Player		
void ValidateMonsters();
void  ProcessPlayers( );//	Player		
void __fastcall LimitPlayerParams( int playerIndex	);//	Player		
void __fastcall ClearPlayerPath( int playerIndex	);//	Player		
int __fastcall CellFreeForPlayer( int playerIndex, int row, int col	);//	Player		
void __fastcall MakePlayerPath( int playerIndex, int row, int col, int endSpace	);//	Player		
void __fastcall SyncPlrAnim( uint playerIndex	);//	Player		
void __fastcall SyncInitPlrPos( int playerIndex	);//	Player		
void __fastcall SyncInitPlr( int playerIndex	);//	Player		
void __fastcall LimitPlayerStats( uint PlrIndex	);//	Player		
bool __fastcall ModifyPlayerStrength( int playerIndex, int modifyValue	);//	Player
bool __fastcall ModifyPlayerMagic( int playerIndex, int modifyValue	);//	Player
bool __fastcall ModifyPlayerDexterity( int playerIndex, int modifyValue	);//	Player
bool __fastcall ModifyPlayerVitality( int PlayerIndex, int Value	);//	Player
void __fastcall SetPlayerHitPoints( int playerIndex, int hitPoints	);//	Player		
void __fastcall SetPlayerStrength( uint playerIndex, int value	);//	Player		
void __fastcall SetPlayerMagic( uint playerIndex, int value	);//	Player		
void __fastcall SetPlayerDexterity( uint playerIndex, int value	);//	Player		
void __fastcall SetPlayerVitality( uint playerIndex, int value	);//	Player		
void __fastcall SetPlayerPerk( uint playerIndex, int perkId, int perkValue );//	Player
void __fastcall InitDungMsgs( uint playerIndex	);//	Player		
void  PlayDungMsgs( );//	Player		
void __fastcall PlayerMessageHold( bool start	);//	Player		
void __fastcall ErrorPlrMsg( char* Source );//	Player
void  AddNewPlayerStatusFormattedMessage( const char *Format, ...	);//	Player
void __fastcall AddNewPlayerDescriptionSatusMessage( int playerIndex, const char* message );//	Player
void CheckPlayerStatusMessagesTimeout( );//	Player		
void  ClearPlayerStatusMessages( );//	Player		
void  DrawPlayerStatusMessages( );//	Player		
void __fastcall DrawPlayerDescriptionText( int x, int y, unsigned int width, char* str, char color	);//	Player		
void  InitPortals( );//	Entry		
void __fastcall SyncPortal(  int playerIndex, int exists, int xPos, int yPos, DUNGEON dungeon   );//	Entry
void __fastcall AddWarpMissile( int playerIndex, int row, int col	);//	Entry		
void  SyncPortals( );//	Entry		
void __fastcall AddinTownPortal( int playerIndex	);//	Entry		
void __fastcall ActivatePortal( int playerIndex, int row, int col, DUNGEON dungeon	);//	Entry		
void __fastcall DeactivatePortal( int a1	);//	Entry		
int __fastcall PortalOnLevel( int a1	);//	Entry		
void __fastcall ClearAllTownPortalsOwnedThisPlayer( int playerIndex	);//	Entry		
void  GetLevelEntryPoint( );//	Entry		
void  FixTPandStairEntryPoint(int lvlDir);//MPstairPlayerPositionClashFix
void  ShiftPositionStairToTown();//Pre-defined position shift when players arriving in Town
int __fastcall IsStairEntryCellFreeFromPlayer(int row, int col);//MPstairPlayerPositionClashFix
int __fastcall IsCellTownPortal(int row, int col);
void  InitQuests( );//	Quest		
void  QuestsActions( );//	Quest		
bool  ShowDungeonEntryDescriptionsFromQuestLocEntries( );//	Quest		
bool __fastcall IsQuestOnLevel( int questIndex	);//	Quest		
void __fastcall SayMessageOnBossKill( int monsterIndex, int isNetData	);//	Quest		
void  AddButcherHallTrans( );//	Quest		
void __fastcall SetWarpToLeoricQuestFloor( int questIndex, int row, int col	);//	Quest		
void __fastcall ApplyWarlordDun( int row, int col	);//	Quest		
void __fastcall ApplyBonechamberDunAndSetWarp( int questIndex, int row, int col	);//	Quest		
void __fastcall ApplyBannerAltDun( int row, int col	);//	Quest		
void __fastcall ApplyBlindAltDun( int row, int col	);//	Quest		
void SetWarpToQuestPattern(QUEST qi);
void __fastcall AddDungeonQuestPatternToWallMap( int startRow, int startCol	);//	Quest		
void  SetReturnDungeonPos(DUNGEON dungeon);//	Quest		
void  GetReturnLvlPos( );//	Quest		
void  ChangeNearestQuestStatus( );//	Quest		
void  ResyncQuests( );//	Quest		
void  MPResyncQuests();
void __fastcall DrawQuestText( int dstX, int rowIndex, int centered, char* str, int color	);//	Quest		
void  QuestPanel_Draw( );//	Quest		
void  QuestPanel_Open( );//	Quest		
void  QuestPanel_ShiftUp( );//	Quest		
void  QuestPanel_ShiftDown( );//	Quest		
void  QuestPanel_StartSpeech( );//	Quest		
void  QuestPanel_CheckLButtonClick( );//	Quest		
void __fastcall UpdateQuestStatus( int questIndex, QUEST_STATUS status, int talkToFlag, int status2, int status3, int status4, int status5	);//	Quest		
void __fastcall DrawTransparentWall( int a1, int a2	);//	Draw		
void __fastcall DrawTransparentCorner( int a1, int a2	);//	Draw		
void __fastcall DrawDunCelTrans( uchar* surfacePtr	);//	Draw		
void __fastcall DrawTransparency( uchar* dst	);//	Draw		
void  ZeroCursorImageWidth( );//	Draw		
void __fastcall DrawMissileInTile( int row, int col, int playerOffsetX, int playerOffsetY, int shiftY, int tileInCellIndex, int mayBeIsExploding	);//	Draw		
void __fastcall DrawDiedPlayer( int row, int col, int arg0, int a4, int a9, int a10, int zero	);//	Draw		
void __fastcall DrawPlayer( int playerIndex, int row, int col, int rowOffset, int colOffset, char* currentAnimationPointer, int currentAnimationFrame, int a8, int a9, int screenCellRow	);//	Draw		
void __fastcall DrawIsoLand( int cameraX, int cameraY, bool dun, bool zoom );//	Draw		
void __fastcall DrawSceneDungeonTop( int row, int col, int offsetX, int offsetY, int areaWidth, int screenCellRow, int a7	);//	Draw		
void __fastcall MonsterSpritePosAdjust(int baseMonsterIndex, int anim, int dir, short& x, short& y );
void __fastcall MonsterPosAdjust(AnimDir* animDir, int& monsterX, int& monsterY, int& tilesInCell, int monsterIndex = 0 );
void __fastcall DrawDynamicObjects( uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, bool firstDraw	);//	Draw		
void __fastcall DrawMonster( int row, int col, int a3, int a4, unsigned int monsterIndex, int a6, int a7	);//	Draw		
void __fastcall DrawObjectSprite( int row, int col, int playerOffsetX, int playerOffsetY, int someFlag, int upHalfCell, int screenCellRow	);//	Draw		
void __fastcall DrawMapCell( uchar* dst, int row, int col, int curTileInCell, int a5, int a6, int a7	);//	Draw		
void  ClearScreen( );//	Draw		
void __fastcall DrawNotGameplayScreen( int showCursor	);//	Draw		
void  DrawCursor( );//	Draw		
void  RenderCursor( );//	Draw		
void __fastcall DrawGameplayToScreen();//	Draw		
void  MainDrawFunc( );//	Draw		
int __fastcall ObjIndex( int row, int col	);//	GenerateQuestFloor		
void  SetupLeoricTombLevers( );//	GenerateQuestFloor		
void  SetupChamberOfBoneLevers( );//	GenerateQuestFloor		
void  SetupUnholyAltarLevers( );//	GenerateQuestFloor		
void __fastcall LoadQuestHallMap( char* dunFileName	);//	GenerateQuestFloor		
void  LoadSetMap( );//	GenerateQuestFloor		
void  ShaDestroy( );//	SHA		
void __fastcall ShaGetLastHash( int streamNum, uchar* out	);//	SHA		
void __fastcall ShaHash( int streamNum, uchar* in, uchar* out	);//	SHA		
void __fastcall UpdateHash( ShaInfo& shaInfo, uchar* buffer, int size	);//	SHA		
void __fastcall TransformHash( ShaInfo& ctx	);//	SHA		
void __fastcall ShaInitialize( int part	);//	SHA		
void __fastcall DupSoundUpdate( bool stopAll	);//	SoundBuffer		
int __fastcall ThisSoundIsPlaying( Sound& sound	);//	SoundBuffer		
void __fastcall PlaySound( Sound& sound, int range, int angle	);//	SoundBuffer		
IDirectSoundBuffer* __fastcall SoundDuplicate( Sound& sound	);//	SoundBuffer		
bool __fastcall RestoreSoundBuffer( Sound& sound, IDirectSoundBuffer* buffer	);//	SoundBuffer		
void __fastcall LoadSound_Wav( Sound& sound	);//	SoundBuffer		
bool __fastcall CreateDirectSoundBuffer( Sound& sound	);//	SoundBuffer		
void __fastcall FreeSoundData( Sound& sound	);//	SoundBuffer		
bool __fastcall AllocateDirectSound( HWND aHWND	);//	SoundBuffer		
void __fastcall GetVolumes( char* key, int* value	);//	SoundBuffer		
void __fastcall SetDirectSoundBufferFormat( HANDLE aSoundFilePtr	);//	SoundBuffer		
HRESULT __fastcall CallDirectSoundCreate( LPGUID guid, IDirectSound** ppDS, IUnknown* pUnkOuter	);//	SoundBuffer		
void  ReleaseDirectSound( );//	SoundBuffer		
int __fastcall SaveVolume( char* valueName, int value	);//	SoundBuffer		
void  StopMusic( );//	SoundBuffer		
void  StopAmbient( );//	SoundBuffer		
void __fastcall PlayMusic( char* musicName = 0	);//	SoundBuffer		
void __fastcall PlayAmbient( char* ambientName = 0	);//	SoundBuffer		
void __fastcall PauseMusic( bool aPause	);//	SoundBuffer		
void __fastcall PauseAmbient( bool aPause	);//	SoundBuffer		
int __fastcall GetOrSetMusicVolume( int volume	);//	SoundBuffer		
int __fastcall GetOrSetAmbientVolume( int volume	);//	SoundBuffer		
int __fastcall GetOrSetSoundVolume( int volume	);//	SoundBuffer		
int __fastcall CalculateManaRequiredToCastSpell( int casterIndex, int spellIndex	);//	SomeSpell		
void __fastcall MinusManaOrChargeOrRelicByPriceOfSSpell( int playerIndex, int spellIndex	);//	SomeSpell		
bool __fastcall IsManaEnoughtForSpell( int a1, int a2, char a3, int a4	);//	SomeSpell		
void __fastcall CastPlayerSpell( int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel	);//	SomeSpell		
void __fastcall ReCastPlayerSpell(int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel);
void __fastcall RisingPlayer( uint casterPlayerIndex, uint revivedPlayerIndex	);//	SomeSpell		
void __fastcall HealOther( int casterIndex, int healedPlayerIndex	);//	SomeSpell		
void  InitInterface( );//	PlayerTrade		
int GetGriswoldBasicalRestockPrice(Player &player);
int GetGriswoldMagicalRestockPrice(Player &player);
int GetAdriaRestockPrice(Player &player);
void  ShopItemsGenerationByPlayerLevel( );//	PlayerTrade		
void  FreeCELPtr( );//	PlayerTrade		
void __fastcall DrawGameDialogTitleText( int position, int rowIndex, int isAligned, char* rowText, int color, int gold	);//	PlayerTrade		
void __fastcall DrawDialogLine( int rowIndex	);//	PlayerTrade		
void __fastcall DrawDialogScroolBox( int firstRow, int lastRow	);//	PlayerTrade		
void  SetGameHelpBigMenuBox( );//	PlayerTrade		
void __fastcall ClearRowsInInterval( int startRowIndex, int endRowIndex	);//	PlayerTrade		
void __fastcall SetDialogLine( int rowIndex	);//	PlayerTrade		
void __fastcall SetItemPrice( int rowIndex, int price	);//	PlayerTrade		
void __fastcall LowerRow( int rowIndex, int indent	);//	PlayerTrade		
void __fastcall SetDialogRow( int position, int rowIndex, int isAligned, char* text, char color, int selectable	);//	PlayerTrade		
bool  AutoPutCursorToInventoryOrDrop( bool allowDropWhenInvFull = true, int cell = -1 );//	PlayerTrade
void  GriswoldMainMenu( );//	PlayerTrade		
void __fastcall DrawBuyBasicalItems( int firstItem	);//	PlayerTrade		
void __fastcall PrintItemParamToTownsmenTradeDialog( Item* item, int row, char color	);//	PlayerTrade		
void  BasicalItemBuyMenu( );//	PlayerTrade		
void __fastcall DrawBuyMagicalItems( int firstItem	);//	PlayerTrade		
bool  MagicItemsBuyMenu( );//	PlayerTrade		
bool __fastcall CanSellItemToGriswold( int inventoryIndex	);//	PlayerTrade		
void  GriswoldSellMenu( );//	PlayerTrade		
bool __fastcall ItemNeedRepair( int invItemIndex	);//	PlayerTrade		
void  RepairItemMenu( );//	PlayerTrade		
void __fastcall AddItemToRepairList( Item* linkOfItemSlot, int inventoryItemIndex	);//	PlayerTrade		
void  WitchAdriaMenu( );//	PlayerTrade		
void __fastcall DrawAdriaItems( int firstItem	);//	PlayerTrade		
void  WitchAdriaStore( );//	PlayerTrade		
int __fastcall CanSellItemToAdria( int inventoryIndex	);//	PlayerTrade		
void  WitchAdriaSellMenu( );//	PlayerTrade		
bool __fastcall StaffNeedRecharge( int invItemIndex	);//	PlayerTrade		
void __fastcall AddItemToRechargeList( char itemInventoryIndex, int edx_unused, const Item& item	);//	PlayerTrade		
void  RechargeItemMenu( );//	PlayerTrade		
void  NoGold( );//	PlayerTrade		
void  NoRoom( );//	PlayerTrade		
void  ConfirmActionMenu( );//	PlayerTrade		
void  WirtMainMenu( );//	PlayerTrade		
void  WirtSellMenu( );//	PlayerTrade		
void  PepinMainMenu( );//	PlayerTrade		
void __fastcall DrawBuyItemsFromPepin( int firstItem	);//	PlayerTrade		
void  HealerPepinShop( );//	PlayerTrade		
void  TownElderMainMenu( );//	PlayerTrade		
bool __fastcall ItemNeedIdentify( const Item* itemPtr	);//	PlayerTrade
void __fastcall AddItemToIdentifyList( char itemInventoryIndex, int edx_unused, const Item& item	);//	PlayerTrade		Оптимизированы вызовы для каждого нательного слота в цикл. 
void  IdentifyMenu( );//	PlayerTrade		
void  ItemIdentifiedMenu( );//	PlayerTrade		
void  TalkToMenu( );//	PlayerTrade		
void  OgdenMainMenu( );//	PlayerTrade		
void  GillianMainMenu( );//	PlayerTrade		
void  FarnhamMainMenu( );//	PlayerTrade		
void __fastcall ChangeTownerDialog( PLAYER_DIALOG dialogIndex	);//	PlayerTrade
void  DrawTownerDialog( );//	PlayerTrade		
void  PrevTownerDialog( );//	PlayerTrade		
void  DialogUp( );//	PlayerTrade		
void  DialogDown( );//	PlayerTrade		
int  DialogPageUp( );//	PlayerTrade		
int  DialogPageDown( );//	PlayerTrade		
void __fastcall ChangeGoldGraphicOnQuantity( int playerIndex, int itemIndex	);//	PlayerTrade		
void __fastcall ChangeGraphValueInBelt( int playerIndex, int itemIndex	);//	PlayerTrade		
void __fastcall ClearGoldByInventoryAsPrice( int price	);//	PlayerTrade		
void  BuyBasicalItemFromGriswold( );//	PlayerTrade		
void  BuyMagicalItemFromGriswold( );//	PlayerTrade		
void __fastcall PutOneGoldPileInInventory( int a1	);//	PlayerTrade		
void  SellItem( );//	PlayerTrade		
void  RepairItemByGriswold( );//	PlayerTrade		
void  BuyItemFromAdria( );//	PlayerTrade		
void  RechargeStaffByAdria( );//	PlayerTrade		
void  BuyItemFromWirt( );//	PlayerTrade		
void  BuyItemFromPepin( );//	PlayerTrade		
void  IdentifyItemByKain( );//	PlayerTrade		
void  HandleTownersMenu( );//	PlayerTrade		
int  CheckDialogMouseClick( );//	PlayerTrade		
void  Dialog_MouseClick( );//	PlayerTrade
bool __fastcall SetRoomCursoreToCenterOfTopWall( int roomIndex	);//	Room		
bool __fastcall SetRoomCursoreToCenterOfRoom( int roomIndex	);//	Room		
bool __fastcall SelectSceletonAndCenterOfRoom( int roomIndex	);//	Room		
bool __fastcall SelectClanAndCenterOfRoom( int roomIndex	);//	Room		
bool __fastcall IsFloorAroundCellIsFree( int row, int col, int roomIndex, int successChance	);//	Room		
bool __fastcall SetRoomCursorToCellAtDistanceFromWalls( int roomIndex	);//	Room		
bool __fastcall IsRoomTypeSuitableForDLVL( int roomType	);//	Room		
bool __fastcall IsRoomTypeSuitable( int roomIndex, int roomType	);//	Room		
bool __fastcall HallIsRoom( int hallIndex	);//	Room		
void  InitRooms( );//	Room		
void  HoldThemeRooms( );//	Room		
void __fastcall FillRoomWithMonsters( int roomIndex, int successChance	);//	Room		
void __fastcall MakeAloneRoomType0Barrels( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType1Shrine( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType2MagicItem( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType3AncientTomes( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType4Gold( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType5Libryary( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType6Tnudems( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType7BloodFountan( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType8DecapitatedBodies( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType9PurifyingSpring( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType10ArmorStand( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType11GoatShrine( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType12Cauldron( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType13MurkyPool( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType14FountainOfTears( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType15BurnCrosses( int roomIndex	);//	Room		
void __fastcall MakeAloneRoomType16WeaponStand( int roomIndex	);//	Room		
void  UniteAllHalls( );//	Room		
void  FillAloneRooms( );//	Room		
void __fastcall DrawTileGrid(uchar* dst, uchar color = 194  );
void __fastcall DrawTileFill( uchar* dst, uchar color = 0	);//	TownMap		
void __fastcall DrawTransparencyTown( uchar* surfaceOffset	);//	TownMap		
void __fastcall DrawMostTopTile( uchar* dst, int a2, int a3, int a4, int a5, int a6, int a7	);//	TownMap		
void __fastcall DrawDynamicObjectsTown( uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, int firstDraw	);//	TownMap		
void __fastcall DrawSceneTownTop( int row, int col, int offsetX, int offsetY, int horCellDrawCount, int screenCellRow, int oddCell	);//	TownMap		
void  InitIsometricTileMap( );//	TownMap		
void __fastcall LoadTownSector( TileSquare* tilesFilePtr, char* dunFilePtr, int startRow, int startCol, int endRow, int endCol	);//	TownMap		
void __fastcall SetTileSquare( TileSquare* tiles, int row, int col, int tileNumber	);//	TownMap		
void  FillDMap3OpenedAbyss( );//	TownMap		
void  FillTownMap1ClosedAbyss( );//	TownMap		
void  FillTownMap2ClosedCrypt( );//	TownMap		
void  FillDMap1OpenedCrypt( );//	TownMap		
void  LoadTownMap( );//	TownMap		
void __fastcall CreateTown( int loadingMode	);//	TownMap		
int __fastcall GetTowner( int townerIdentificator	);//	TownMap		
void __fastcall SetupCowAnimation( char* cowCELFile, char** animationArray	);//	TownMap		
void __fastcall InitTownerAnimation( int townerIndex, char* townerAnimationPtr, int stayFrameCount, int delayFrameCount	);//	TownMap		
void __fastcall InitTownerStruct( int townerIndex, int maybePicSize, int maybeHasDialog, int townerIdentificator, int row, int col, char framesOrderTypeIndex, int always10	);//	TownMap		
void __fastcall SetTownerQuestInfo( int townerIndex	);//	TownMap		
void  LoadGriswold( );//	TownMap		
void  LoadOgden( );//	TownMap		
void  LoadWoundedTownsman( );//	TownMap		
void  LoadAdria( );//	TownMap		
void  LoadGilian( );//	TownMap		
void  LoadWirt( );//	TownMap		
void  LoadPepin( );//	TownMap		
void  LoadCain( );//	TownMap		
void  LoadFarnham( );//	TownMap		
void  LoadCows( );//	TownMap		
void  LoadLester( );//	TownMap		
void  LoadNut( );//	TownMap		
void  LoadCelia( );//	TownMap		
void  InitTowners( );//	TownMap		
void  FreeTowners( );//	TownMap		
void __fastcall StopTownerSpeechWhenPlayerGoAway( int townerIndex	);//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_0_Griswold( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_3_Ogden( );//	TownMap		
void  SlainTownsmanAction( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_1_Pepin( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_4_Cain( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_5_Farnham( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_8_Wirt( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_6_Adria( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_7_Gilian( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_9_Cows( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_10_Lester( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_12_Nut( );//	TownMap		
void  StopTownerSpeechWhenPlayerGoAway_11_Celia( );//	TownMap		
void  ProcessTowners( );//	TownMap		
Item* __fastcall FindItemInInventory( int playerIndex, int baseItemIndex, int& itemInventoryIndex	);//	TownMap		
void __fastcall NPCGreeting( int speechOffset	);//	TownMap		
void __fastcall TalkToTowner( int playerIndex, int townerIndex	);//	TownMap
bool IsTownerHasActiveSpeech( int playerIndex, int townerIndex );
void __fastcall CowClick( int playerIndex	);//	TownMap		
void  ClearWarps( );//	Warp
void  AddWarp(int row, int col, int gameMessage, DUNGEON destDungeon = DUN_NO);
void  AddWarpsToTown( );//	Warp		
void  AddWarpsToChurchTypeLabyrinth( );//	Warp		
void  AddWarpsToCatacombTypeLabyrinth( );//	Warp		
void  AddWarpsToCaveTypeLabyrinth( );//	Warp		
void  AddWarpsToHellTypeLabyrinth( );//	Warp		
void  AddWarpFromSceletonKing( );//	Warp		
void  AddWarpFromPoisonWell( );//	Warp		
void  AddWarpFromUnholyAltar( );//	Warp		
bool  ShowDungeonEntryDescriptionsInTown( );//	Warp		
bool  ShowDungeonEntryDescriptionsInChurchAndCrypt( );//	Warp		
bool  ShowDungeonEntryDescriptionsCatacombs( );//	Warp		
int  ShowDungeonEntryDescriptions_Caves_And_Abyss( );//	Warp		
bool  ShowDungeonEntryDescriptions_Hell( );//	Warp		
void  ClearAreasAroundWarps( );//	Warp		
bool  ShowDungeonEntryDescriptions_LeoricTomb( );//	Warp		
bool  ShowDungeonEntryDescriptions_HallOfHeroes( );//	Warp		
bool  ShowDungeonEntryDescriptions_ChamberOfBone();
bool  ShowDungeonEntryDescriptions_VaultKeep();//	Warp		
bool  ShowDungeonEntryDescriptions_PoisonedWaterSupply( );//	Warp		
bool  ShowDungeonEntryDescriptions_FetidCavern();//	Warp		
void  ShowDungeonEntryDescriptions( );//	Warp		
void  CheckTriggers( );//	Warp		
int __fastcall File_Close( HANDLE aHandle	);//	File		
DWORD __fastcall File_GetSize( HANDLE aHandle, LPDWORD lpFileSizeHigh	);//	File		
void __fastcall File_RestoreAccess( HANDLE aHandle, int *aTryCountPtr, LPARAM dwInitParam	);//	File		
int __fastcall File_Open( const char *fileName, HANDLE *fileHandle, int oneTry	);//	File		
int __fastcall File_Read( HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead	);//	File		
DWORD __fastcall File_Seek( HANDLE aHandle, LONG lDistanceToMove, LONG* lplDistanceToMoveHigh, DWORD dwMoveMethod	);//	File		
bool __fastcall CreateFilePlayCTX2( HANDLE handle, WAVEFORMATEX* waveFormat	);//	SoundCache		
void __fastcall SoundCache_Init( HANDLE file, SoundCache* soundCache, uint maxCache	);//	SoundCache		
void __fastcall SoundCache_Free( SoundCache* soundCache	);//	SoundCache		
bool __fastcall SoundCache_FillWaveFormat( SoundCache* soundCache, WAVEFORMATEX* waveFormat, uint* bufferSize	);//	SoundCache		
bool __fastcall SoundCache_Read( SoundCache* soundCache, char* aBufPtr, int size	);//	SoundCache		
void __fastcall SoundCache_LoadFile( SoundCache* soundCache	);//	SoundCache		
int __fastcall SoundCache_Skip( SoundCache* soundCache, uint size	);//	SoundCache		
bool __fastcall SoundCache_GetWaveTagInfo( SoundCache* soundCache, unsigned int tag, WaveTagInfo* tagInfo	);//	SoundCache		
char* __fastcall CreateFilePlayCTX_Wav( HANDLE handle, WAVEFORMATEX* waveFormat, uint* bufferSize	);//	SoundCache		
void  WriteCmpData( TCmpStruct *pWork	);//	pklib		
void  OutputBits( TCmpStruct* pWork, unsigned int nbits, unsigned long bit_buff	);//	pklib		
void  lmemset( void * buff, int c, size_t count	);//	pklib		
void  FlushBuf( TCmpStruct* pWork	);//	pklib		
void  SortBuffer( TCmpStruct* pWork, unsigned char* uncmp_data, unsigned char* work_end	);//	pklib		
unsigned long  Expand_pklib( TDcmpStruct* pWork	);//	pklib		
unsigned long  DecodeLit( TDcmpStruct* pWork	);//	pklib		
unsigned long  DecodeDist( TDcmpStruct* pWork, unsigned long dwLength	);//	pklib		
int  WasteBits( TDcmpStruct* pWork, unsigned long nBits	);//	pklib		
void  GenDecodeTabs( long count, unsigned char* bits, unsigned char* pCode, unsigned char* buffer2	);//	pklib		
void  GenAscTabs( TDcmpStruct *pWork	);//	pklib		unused_BigArg
int  ModifyBarrelExplodeDamage( );//	Object		int __usercall ModifyBarrelExplodeDamage@<edx>()
bool __fastcall ActTrash16SpellEffect( int missileOffset	);//	Missile		bool __usercall ActTrash16SpellEffect@<zf>(int missileOffset@<esi>) отключенная магия, в таблице кастов не должно быть этого адреса так как сигнатура не совпадает
int __fastcall ChooseHolyBoltTypeExSpriteIndex( int missileOffset	);//	Missile		int __usercall ChoiseHolyBoltTypeExSpriteIndex@<edx>(int missileOffset@<esi>)
void __fastcall SetHolyBoltExSpriteHack( int missileIndex, int spriteIndex, int missileOffset	);//	Missile		void __usercall SetHolyBoltExSpriteHack(int missileIndex@<ecx>, int spriteIndex@<edx>, int missileOffset@<esi>)
void __fastcall CastBallLightning( int spellCastIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage	);//	Missile		Пересечение с CastFireBall, переразбирать пока не буду, оставлю до сверки
int __fastcall LimitSlvlParamForFireWallTimeliveCalc( int missileIndex	);//	Missile		int __usercall LimitSlvlParamForFireWallTimeliveCalc@<eax>(int missileOffset@<esi>)
int __fastcall DoesMissileKnockback( int playerIndex, int effectIndex	);//	Missile		bool __usercall IsMissleMayKnockback@<al>(int a1@<eax>, int a2@<ebp>)
bool __fastcall IsValidBaseForGriswoldBasicalItems( int baseItemIndex	);//	Item		
int  CheckUberCoord( );//	Monster		POINT __usercall __spoils<zf> CheckUberCoord@<ebx:eax>()
void __fastcall ActivateSlainHero_TH( int playerIndex, int objectIndex	);//	Quest		 раненый герой у церкви замена для 0x0045121C ActivateSlainHero
void  AutomapDrawPlayers( );//	Monster ?		0043F0AB было
int  GetGoldModificator( );//	Item		
int __fastcall CountDestructible( Item* item, int flag	);//	Object		int __usercall HiddenShrineFix@<edx>(int item@<ecx>, int flag@<edx>)
bool  Belt5_8Fix( );//	Item		
void __fastcall AddBaseDurWithLimit( Item* Item	);//	Object		int __usercall AddBaseDurWithLimit@<eax>(int players@<eax>, int playerItemOffset@<edx>)
void __fastcall Limit_MvP_Ranged_Magical_ToHit_Chance(int playerIndex, int& realToHitChance);//	Missile		int __usercall LimitNonmeleeRealToHitMagic@<ebx>(Player *player@<ecx>, int realToHitChance@<ebx>)
void __fastcall MonsterLifeSteal( int delLife, int monsterIndex, int playerIndex	);//	Monster		void __usercall MonsterLifeSteal(int a1@<ebx>, int a2@<edi>)
bool PlayerKnockbackResistFail( int monsterIndex, uint playerIndex, bool charge = false	);
int __fastcall VariatePotionsByDifficulty( int randAdd, int addFromMax	);//			POINT __usercall VariatePotionsByDifficulty@<eax:edi>(int a1@<eax>, int a2@<edi>, int a3@<esi>)
int  CannotIncreaseStat( int playerIndex );//	Player		
int __fastcall CalcCharBaseStatSum( Player& player	);//	Player		
void __fastcall SetPlayerHitPointsOnRestart( int playerIndex, int hitPoints	);//	Player		переделаная SetPlayerHitPoints 0045E9AC
void __fastcall MpRestartAvailability( Menu* menuItem, int isActivated	);//	Menu		
void __fastcall DisarmTrapFix( int playerIndex, int objectIndex	);//	Object		
int  SpellLevelCap( );//	Player		
uchar __fastcall GetBookLevelReq( int spellIndex, int playerIndex	);//	Item
void  LimitSpellLevels( );//	Player
void LimitPlayerPerks( int playerIndex );
bool __fastcall NewToGolemAttackType( int monsterIndex	);//	Monster		
char __fastcall CountMonsterAttackers( int monsterIndex	);//	Monster		
void __fastcall MonsterChainActivation( int monster	);//	Monster		void __usercall MonsterChainActivation(int monsterIndex@<esi>)
bool __fastcall ReadySpellAllowed( int casterIndex, int spellIndex	);//			
void __fastcall RemoveGolemAndMissilesMayBe( int playerIndex	);//	Player		
void __fastcall ClearPlayerSpells( int playerIndex	);//	Player		
int __fastcall MaxCountOfHydrasForPlayer( int playerIndex	);//	Missile		
int __fastcall CountOfPlayerHydras( int playerIndex	);//	Missile		
void __fastcall ClearOneOldestPlayerHydra( int playerIndex	);//	Missile		
void __fastcall LimitPlayerHydras( int playerIndex	);//	Missile		
void __fastcall ComeHereMrGolem( int monsterIndex	);//			
void __fastcall OnMouseWheel( uint wParam	);//	Input		
POINT __fastcall InfoPanelManaHeal( int spellNum	);//	Panel		POINT __usercall InfoPanelManaHeal@<edx:ecx>(void *a2@<ecx>)
int __fastcall GetHealManaCost( int healMana, int minHealManaLimit	);//	SomeSpell		
int __fastcall GetHealOtherManaCost( int healMana, int minHealManaLimit	);//	SomeSpell		
bool  ShowDungeonEntryDescriptions_InfestedCellar( );//	Warp		
int __fastcall CreatePepinRing( int row, int col, int itemQlvl, int a4, int a5	);//
void __fastcall DropLairQuestAward( int row, int col	);//	TownMap			
void __fastcall Drop_DITT_QuestAward(int row, int col);//	TownMap		
void __fastcall LoadQuestDungeonAltMapHell( char* dunNamePtr, int playerRow, int playerCol	);//	GenerateMap		копия LoadQuestDungeonAltMapCave 00414BC7 с изменениями 
void __fastcall LoadQuestDungeonMainMapHell( char* dunFileName, int playerRowPos, int playerColPos	);//	GenerateMap		копия LoadQuestDungeonMainMapChurch 0040BAFD с изменениями
void  AddWarpFromHallsOfAnguish( );//	Warp		
bool  ShowDungeonEntryDescriptions_AndarielHalls( );//	Warp		
bool  ShowDungeonEntryDescriptions_AndarielThrone( );//	Warp		
void  AddAndyQuestAward( );//	TownMap		
bool __fastcall IsThisBoss( int monsterIndex, int bossIndex	);//	Monster		
void __fastcall OffRandQuestFromGroup( int groupIndex	);//	Quest
int __fastcall CountOfActiveQuestInGroup( int groupIndex	);//	Quest		
void  RandomizeQuests( );//	Quest		
void __fastcall LoadSPTownMap( TileSquare* townTiles	);//	TownMap		
void __fastcall SetWarpToHorazonQuestFloor( int questIndex, int row, int col	);//	Quest		
bool __fastcall IsValidWarpNearCursor( int gameMessage	);//	Warp		
void __fastcall CannotEnterBeforeClearing( int flags, int messageIndex	);//	Warp		меняет ecx
void AddWarpFrom_BloodHive();// Warp
void  QuestVideoOnEnter( );//			
void __fastcall InitHorazonQuestBook( int objectIndex, int bookNumber	);//	GenerateQuestFloor		
void  SetupHorazon1Books( );//	GenerateQuestFloor		
void  SetupHorazon3Books( );//	GenerateQuestFloor		
void  SetupHorazon2Books( );//	GenerateQuestFloor		
void  AddObjectOnHorazonEnter( );//	Quest		
bool __fastcall IsCursorNearCoordinate( int row, int col	);//	Warp		
bool  ShowDungeonEntryDescriptions_Horazon( );//	Warp		
bool  LeverClick( );//	Object		
void __fastcall AddBossTypeToQuestLoc( int uniqueMonsterIndex	);//	Monster		
void __fastcall OpenCryptQuestDoor( int row, int col	);//	GenerateQuestFloor		
void __fastcall OpenGrimspikeRoom( int needPlaySound	);//	GenerateQuestFloor		
void __fastcall MoveGrimspike( int row, int col	);//	Quest		
void  GrimspikeOpenScript( );//	Quest		
void  HorazonQuestCompleteCheck( );//	Quest		
void __fastcall OpenNakrulRoom( int needPlaySound	);//	GenerateQuestFloor		
void __fastcall OpenNakrul( bool isWeak	);//			
int __fastcall IsSpellEnabledHere(int currentSpellNumber, int playerIndex);
void  UpdateItem(Item& item, bool keepCharges = true);//	Player		
void  UpdateAllItems( );//			
void __fastcall DropItemWithQlvlFilteredByMagicLevel( int itemIndex, int baseItemIndex, int seedOrQuestUniqIndex, int levelParam, int chanceInProcent, int IsGoodMagicItem, int allowDup, int a8, int monsterIndex	);//	Item		
int __fastcall QlvlForMagicItem( int qlvl	);//	Item		
int __fastcall CountItemFromSet( int playerIndex, int setId	);//	Item		
bool __fastcall IsItemFromSet( Item& item, int setId	);//	Item		
CompleteSetEffect* __fastcall FindCompleteSetEffect( int name	);//	Item		
void  ClearBufferItemForShowEffect( );//	Item		
void __fastcall ApplyEffectToBufferItemForShowEffect( BaseEffect effect	);//	Item		
void __fastcall ApplyFullSetEffectToBufferItem( int playerIndex	);//			
float __fastcall ResistPercentMelee( int playerIndex	);//	Monster
float __fastcall ResistPercentArrow (int playerIndex);
int __fastcall AddDamageReduction( int playerIndex, int damage	);//	Missile		
bool  IsPinkyOnLevel( );//	Monster		
void  UnMarkPatternPositionOnMap(QuestPattern& qp = CurPattern);//			
QuestPattern* GetQuestPattern(QUEST qi);
bool SetQuestPatternWarp(int row, int col);
bool SetCurPattern(QUEST qi);
void RestoreCurPattern();
bool  IsDregbezOnLevel( );//	Monster		
bool  IsUberXoreneOnLevel( );//	Monster		
bool  IsCeromorthOnLevel( );//	Monster		
int  CountOfItemsOnBelt( );//	Item		
int __fastcall GetMonsterAcAfterAp( int playerIndex, int monsterArmorClass	);//	Missile		
void  Repeat_Mouse_LButtonDown( );//	Input		
void  Repeat_Mouse_RButtonDown( );//	Input		
int __fastcall IsChest( int a1	);//			
void  TakeCurrentPlayerDeathToll( );//			
void __fastcall AddWarpFromFirePassage( int isPortalOpen	);//	Warp		
bool  ShowDungeonEntryDescriptions_Izual( );//	Warp		
void  AddWarpFromIzualValey( );//	Warp		
void  AddIzualQuestAward( );//	TownMap
void  DrawInfoLeft();//
void  DrawInfoRight( );//
std::pair<COOLDOWN_TYPE, int> GetSpellCooldownValue( int playerIndex, int spellIndex );
bool __fastcall CooldownCheck( int casterIndex, int spellIndex	);//
void __fastcall FillDMapClosedDisusedReliquary( int row, int col	);//	TownMap		
void __fastcall FillDMapOpenedDisusedReliquary( int row, int col	);//	TownMap		
void  AddWarpFromGlacialCaverns( );//	Warps		
void  AddWarpFromDisusedReliquary( );//	Warps		
void  AddWarpFromFrozenLoch( );//	Warps		
bool  ShowDungeonEntryDescriptions_Treasure( );//	Warps		
void  Frozen_Loch_BlockSet( );//	Quest		
void  Ice_Age_Abyss_Jesus_mode();
int  AdjFrameQ_14_15( );//	Quest		
void __fastcall ApplyUberAltDun( int row, int col	);//	Quest		
void __fastcall MonsterGreetings( int monster	);//	Monster		void __usercall MonsterGreetings(int monsterOffset@<esi>)
void  GenerateRandomSequenceOfAngels( );//	Quest
void  SetupAngels( );//	Quest
int __fastcall CheckAngelClick( uchar angelIndex	);//	Quest
void __fastcall ActivateAngels( int playerIndex, int objectIndex	);//	Quest		Island of the Sunless Sea quest
void  AddWarpFromIsland( );//	Warps		
void  AddWarpFromFetidCavern();
bool  ShowDungeonEntryDescriptions_Island( );//	Warps		
int __fastcall DoesMonsterHaveCritSplash( int monsterIndex	);//	Monster		
void __fastcall CastMonsterCritSplash( int monsterIndex, int deadlyStrike, int playerIndex	);//	Monster		
void  LoadPriest( );//	TownMap		
void  DrawXpBar( );//	TownMap		
void  AddWarpFromFleshdoom1( );//	Warps		
void  AddWarpFromFleshdoom2( );//	Warps		
void  AddWarpFromTheDen( );//	Warps
void AddWarpFromRavenholm();
void AddWarpFromTheColiseum();
bool  ShowDungeonEntryDescriptions_Fleshdoom( );//	Warps		
int __fastcall IsBaseValidForSaleToPlayer( int baseIndex, int townerIndex	);//	TownMap
void __fastcall DropBossGold( int monsterIndex	);//	Monster		void __userpurge DropBossGold(int col@<edx>, int row@<ecx>, int a3@<ebp>, int bossId)
int __fastcall IsOilQuality( int baseItemIndex	);//			
int __fastcall IsPotencialChamp( int row112, int col112	);//	Monster		
void __fastcall RecrutMonster( int row112, int col112	);//	Monster		
int __fastcall RecruitPack( int row112, int col112	);//	Monster		
void __fastcall MakeChampions( int minCount, int maxCount	);//	Monster		
void __fastcall DefenderChamp( Monster& monster	);//	Monster		
void __fastcall FanaticChamp( Monster& monster	);//	Monster		
void __fastcall BerserkerChamp( Monster& monster	);//	Monster		
void __fastcall AcidPoolLimiter( int missileIndex	);//			
void  ClearAltWeapons( );//	Item		
void  DoSwap( );//	Item
void PlayAltWeaponPairSound();
void DoPostAltWeaponPairSwapActions();
void ChangeAltWeaponPairTo( int which );
void ChangeAltWeaponPairToNext( bool skipEmpty );
void __fastcall WriteWeaponSwapInfoToSave( LastItemInfo* swapItems	);//	Item		
int __fastcall ReadWeaponSwapInfoFromSave( HANDLE save, LastItemInfo* swapItems	);//	Item		
int __fastcall IsCursorInInvPanelRect( RECT* rect	);//	Item		
void __fastcall ContinueStay( int playerIndex	);//	Item
void __fastcall ContinueWalk( int playerIndex	);//	Item		
void __fastcall DoTremainDeadEffect( int townerIndex, int baseMissileIndex	);//			
void __fastcall MonsterHorizontalHitFix( int& monsterNum, int row, int col );
void  InquisitionScript( );//	Quest
void CloseDenPortals();
void TheDenScript();
void Lava_Pits_Script();
int  IsSomeMonstersActivated( );//	Warp		
bool  IsSaveAndLeavingEnabled( );//	Warp		
i64 GetTickCountTh();

void GenerateQuestFloor_44_Ravenholm();// Quest
void GenerateQuestFloor_45_TheDen();// Quest
void GenerateQuestFloor_46_Coliseum();
void __fastcall AdjustDDrawWindow( HWND hwnd );//   
void __fastcall CalculatePerPixelLight( int Row, int Col );//   
void __fastcall GoldAutoPickup( int playerIndex );//   
LPDIRECTDRAWSURFACE __fastcall ConvertPaletteSurfaceToTrueColor( uchar* lpSurface );//   
void DrawDebugInfo(int& y);//   
void DrawMenuInfo();//   
char** enum_split( char* names, size_t count );//   
uint GetCooperativeLevel();//   
unsigned char* __fastcall GetCurrentLightTable( int y, int x );// Draw  
void GetUserParameters();//   
void __fastcall linecpyPerPixel( void* dst, void* src, int size, int h, unsigned mask, void* tbl );// Draw  
void LoadDragonMenuPalette();//   
void LoadMainMenuPalette();//   
void LoadQuestDungeonMainMapAbyss( char* dunNamePtr, int a2 );//   
void LoadQuestDungeonMainMapTown( char* dunFileName, int playerRowPos, int playerColPos );//   
void SetupDisplayObjects();//   
BOOL __stdcall SinglePlayerDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );//   
void FreeCharacterTable();//
void ProcessBerserk(Player& player, bool& autoClick);
void GetParamsFromConfig();
void InitTH2( void* arg );//
void ExitTH2();//
void MotionInterpolationBegin( int& offsetX, int& offsetY );
void MotionInterpolationEnd();
void ChangeScreenRes();
void SetScreenDesc(DDSURFACEDESC& screenDesc);
void CreateMiniDump( EXCEPTION_POINTERS* pep );

void VisualStashGotoPrevPage();
void VisualStashGotoNextPage();
void VisualStashGotoPrevTab();
void VisualStashGotoNextTab();
void __fastcall StashPanel_PrevTab();
void __fastcall StashPanel_NextTab();
void SetupStashPanel();

void CursorOn();
void CursorOff();
int __stdcall SecWinHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void SwitchFullscreen( bool forceWindowed = false );
void SetFlipTable();
int __fastcall GetTextWidth( const char* s );
int __fastcall GetTextWidthMid( const char* s );
int __fastcall GetTextWidthBig( const char* s );
void HighlightItemsNameOnMap();

void DrawCombatTextAbovePlayer();
void DrawFloatingHealthAndMana();
int MaxAcidDPS(int monsterIndex, int playerIndex);
void DamageMonstersByAuras();
void ShowMessageLog(int firstItem);
void DrawFloatingDamage(int damage, int row, int col, int callerID, int color = C_9_RedGold);
void CreateItemIgnoringDungeonLevel(int row, int col, int isGoodMagicItem, int itemCode, int magicCode, bool sendMsg);
int MonsterDropLevel( int monsterIndex );
int GetNumberOfMonstersInDungeon();
int GetNumberOfObjectsInDungeon();
bool IsIronmanObject(Object& obj);
void IronmanLevelCleared();
int GetSuperGameMode(char* name = 0);
const char* getGameModeName( GAME_MODE gameMode, GAME_CHANGER GameChanger = GC_0_CONTINUE);
void UpdateScaleResist(int playerIndex);
void CalcResistPercentMelee(int playerIndex);
void CalcResistPercentArrow(int playerIndex);
BaseCrit GetBaseCrits(int critType, int playerIndex);
int GetPlayerKnockbackResistChance( uint playerIndex );
void DrawPerksPanel();
void PerksPanel_MouseMove();
int PerkValue(PERKS perk,int playerIndex, int effectIndex = 0);
bool checkRequirementsForPerk(int p, int l);
void addUnlimitedXpToPlayer(i64 xpGain, int index);
i64 getPassiveXpFormula();

bool CursorInMainPanel();
bool ActionClick();
void CheckPanelClick();
bool CursorIntoLevelUpButton();
void HandlePerksWindowClick();
void InitPerks();
int getPlayerPerksPointsLeft(PERK_TYPE perkType = PERK_TYPE_COMMON);
void PerkPanel_Shift(int delta);
void PerkPanel_ShiftLeft();
void PerkPanel_ShiftRight();
void PerkPanel_ImprovePerk();
void getAvailablePerksList();
bool PerkAvailable( int playerIndex, int perkIndex, int perkLevel = 0 );

void __fastcall ActGoldFind( int& gold );
void __fastcall ActCriticalHit( int& damage, int playerIndex, int row, int column, bool playSound,int damageType, int baseMissileIndex = -1 );

void __fastcall DrawBoxTransparent(int xPosition, int yPosition, int width, int height);

void CheckPerkUpButton();
void PerkUPButton_MouseClick();
void DrawPerkUpButton();

void OpenInventoryPanel();
void CloseInventoryPanel();
void ToggleCharPanel();
void ToggleQuestPanel();
void ToggleInventoryPanel();
void ToggleSpellBook();
void OpenPerksPanel();
void ClosePerksPanel();
void TogglePerksPanel();
void ToggleSynergyPanel();
void ToggleInfoWindow();

const char* getSpellName( int spell );
int getSpellIcon( int spell );

uint GetRequiredClassMask( const Item* item );
bool CheckBaseItemClassMaskReq( uint playerIndex, int baseItemIndex );
bool CheckBaseItemExilesReq( uint playerIndex, int baseItemIndex );

bool CheckBaseItemClassReq( uint playerIndex, int baseItemIndex );

int Calc_PvM_Ranged_Hit_Chance( int playerIndex, int monsterIndex, int distance, int baseMissileIndex);
int CalcPvM_MeleeHitChance( int playerIndex, int monsterIndex, bool isSweepAttack );
int GetPlayerArrowSpeed( int playerIndex );
int CalculateArrowDistance( int playerIndex, int monsterIndex );

void OnTakeMeleeTrigger( int monsterIndex, int playerIndex );

PLAYER_FULL_CLASS GetPlayerFullClass( PLAYER_CLASS classId, PLAYER_SUBLASS subclassId, PLAYER_SPECIALIZATION specializationId );

Item* findThrowableItem( int playerIndex );

void __fastcall CastTrapThrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage );
void __fastcall ActTrapThrow( int missileIndex );

void __fastcall CastTrapperTrap( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage );
void __fastcall ActTrapperTrap( int missileIndex );

void __fastcall CastTrapperTrapArrow( int missileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage );
void __fastcall ActTrapperTrapArrow( int missileIndex );
void __fastcall ClearOneOldestPlayerTrap(int playerIndex);

int CalculateLesserSummonsMaxAmount( int slvl );
int CalculateCommonSummonsMaxAmount( int slvl );
int CalculateSummonsMaxAmount( int summonType, int slvl, int playerindex );
void GetSummonsTypeOffsets( int summonType, int* summonsStartIndex, int* summonsSlotsAmount, int playerindex );
void KillExtraSummons( int ownerIndex, int summonType, int slvl );
void RecalcSummonOrder( int ownerIndex, int summonType );

void __fastcall Ai_Summon( uint monsterIndex, AiFunc* original );
bool TeleportSummonToOwner( uint monsterIndex, uint ownerIndex );
int FindBodySlotWithSpell( int playerIndex, int spellIndex, int startItem = 0 );
int SumBodySlotSpellCharges(int playerIndex, int spellIndex);

void	__fastcall	WriteStashInfoToSave();	//	th2	Stash	3
bool	__fastcall	ReadStashInfoFromSave( HANDLE archiveHandle );	//	th2	Stash	4
void	__fastcall	StashPanel_Draw();	//	th2	Stash	97
bool	__fastcall	Stash_MoveItemToInventory(int itemIndex);	//	th2	Stash	106
bool	__fastcall	AddItemToStashFromCursor();	//	th2	Stash	107
void	__fastcall	ClearStashSlot(int pageIndex, int invCellIndex);	//	th2	Stash	112
void	__fastcall	Stash_LButtonDown();	//	th2	Stash	116
bool	__fastcall	AutoPutCursorItemToStash(int playerIndex);	//	th2	Stash	124
void __fastcall RecalcBookReq(Item& item, int playerIndex);
void	__fastcall	StashPanel_RecalcItemsReq();	//	th2	Stash	333
void	__fastcall	StashPanel_Open();	//	th2	Stash	1116
bool __fastcall CursorIntoStashButton(const DisplayObject& button);
void __fastcall StashPanel_MouseMove();
void __fastcall StashPanel_MouseClick();
void __fastcall StashPanel_Load();
void __fastcall StashPanel_Save();
bool __fastcall StashPanel_IsStashSlot( int slotId );
Item* __fastcall StashPanel_GetItemBySlotId( int slotId );

char* GetItemSprites( int& spriteId );

bool IsQuestItem( int baseItemIndex );
bool IsObsoleteItem( int baseItemIndex );
bool IsValueStoneItem( int baseItemIndex );
bool IsItemForbiddenToStore( int baseItemIndex );
bool IsItemAllowedForStash( const Item* item );

void SetupDisplayObject(DisplayObject& displayObjectRef, int xPos, int yPos, int width, int height);
bool CursorIntoDisplayObject(const DisplayObject& button);
bool CursorIntoPerkUpButton();
void GetSuccessionArray(uchar inventoryWidth, uchar inventoryHeight, uchar itemWidth, uchar itemHeight, vector<uchar>& successionArray);
int GetCellIndex(DisplayObject& object, int x, int y);
void printItemClassReqInfoInBuffer( const Item* aItem );
bool IsItemAllowedForBelt(const Item& item);
bool __fastcall ItemNeedRepair( const Item* item );
int ShopQlvl(const Player& player);
int BuyPrice(int price);
int BuyPrice(Item& item);
int SellPrice(int price);
int SellPrice(Item& item);
Item* __fastcall FindItemInInventoryOrBeltBySeed(int playerIndex, int seed, int baseItemIndex);
void ClearPlayerItemBySeed(int playerIndex, int seed, int baseItemIndex );

void VisualTrade_RefreshPage( VisualTradePage pageId );
void VisualTrade_ResetPage( VisualTradePage pageId );
void __fastcall VisualTrade_Open( VisualTradeMode mode );
void __fastcall VisualTrade_Close();
void __fastcall VisualTrade_Draw();
void __fastcall VisualTrade_ClearInfostring();
void __fastcall VisualTrade_MouseMove();
void __fastcall VisualTrade_MouseClick();
void __fastcall VisualTrade_LButtonDown();
bool __fastcall VisualTrade_IsVisualTradeSlot( int slotId );
Item* __fastcall VisualTrade_GetItemBySlotId( int slotId );
int VisualTrade_GetBuyPrice(const Item* item);
int VisualTrade_GetSellPrice(const Item* item);
int VisualTrade_GetRepairPrice( const Item* item );
int VisualTrade_GetRechargePrice( const Item* item );
int VisualTrade_GetIdentificationPrice( const Item* item );
bool VisualTrade_RepairItem( Item* item );
bool VisualTrade_RechargeItem( Item* item );
bool VisualTrade_IdentifyItem( Item* item );
bool VisualTrade_RepairItem( int slotId );
bool VisualTrade_RechargeItem( int slotId );
bool VisualTrade_IdentifyItem( int slotId );
int VisualTrade_GetRepairAllPrice();
int VisualTrade_GetRechargeAllPrice();
int VisualTrade_GetIdentifyAllPrice();
void VisualTrade_RepairAll();
void VisualTrade_RechargeAll();
void VisualTrade_IdentifyAll();
void VisualTrade_AddGoldToInventory( int value );
bool VisualTrade_IsItemAllowedForSell( const Item* item );
void VisualTrade_SellItem( int slotId = -1 );
bool VisualTrade_TryToSellItem( int slotId = -1 );
void VisualTrade_BuyItem( int slotId, bool inCursor = true );
bool VisualTrade_TryToBuyItem( int slotId, bool inCursor = true );
void VisualTrade_RecalcItemsReq();
int VisualTrade_GetRestockPrice();
void VisualTrade_Restock();
bool VisualTrade_CheckFreeSpaceForItem( const Item* item );
void SetupTradePanel();
bool VisualTrade_GriswoldUpgradeAvailable();
void VisualTrade_GriswoldUpgrade();
uchar VisualTrade_GetGriswoldLevel();
int VisualTrade_GetGriswoldUpgradePrice();

uint8_t __fastcall GetItemBackgroundColorType(const Item& item);

void __fastcall Tooltip_Draw();
void __fastcall Tooltip_Draw_2();
void __fastcall Tooltip_Draw_3();
void __fastcall Tooltip_Clear();
void __fastcall Tooltip_AddLine( const char* text, int color = C_0_White );
void __fastcall Tooltip_AddLine_2( const char* text, int color = C_0_White );
void __fastcall Tooltip_AddLine_3(const char* text, int color = C_0_White);
void __fastcall Tooltip_ShowItemInfo( int slot );
void __fastcall Tooltip_SetOffsetLeft();
void __fastcall Tooltip_SetOffsetRight();
void __fastcall Tooltip_SetOffsetTop();
void __fastcall Tooltip_SetOffsetBottom();

void InitBlood();
void AddBlood( int row, int col, int missileSpriteIndex, int variation );

void SetupPanelRect(DisplayObject& displayObjectRef, PanelLayout layout, int width, int height);
void __fastcall DrawDialogBox( int width, int height, char* bitmap, int x = 0, int y = 0 );

void CheckNetworkDataIntegrity();

void SetItem_Init();
bool SetItem_IsItemFromSetEquipped( int playerIndex, int uniqueItemIndex );
int SetItem_GetCountOfEquippedItemsFromSet( int playerIndex, int uniqueSetIndex );
void SetItem_RecalculateSetInfo( int playerIndex );
void SetItem_GetActiveSetItemEffects( uint playerIndex, std::vector<Item*>& result );

bool isTownPortalForbidden();
GAME_MODE_TYPE GameModeType();

const char* GetTraitName( TraitId id );
const char* GetTraitBenefitDescription( TraitId id );
const char* GetTraitPenaltyDescription( TraitId id );
const char* GetTraitDescription( TraitId id );
int GetTraitSpellIcon( TraitId id );
template<typename... T> __forceinline bool HasTrait(uint playerIndex, T&&... val){ return playerIndex > 3 ? false : has(Players[playerIndex].traits, val...); }
template<typename... T> __forceinline bool CurTrait(T&&... val){ return has(Players[CurrentPlayerIndex].traits, val...); }

bool IsMonsterImmuneToMissile(int monsterIndex, int damageType, int playerIndex);

PlayerStat GetPlayerMaxStat( int playerIndex );
bool __fastcall MoveInOrientOrAboveNew(int monsterIndex, int orientation, uint orientIndent);

void ResetSpeedrunTimer();
void AdvanceSpeedrunTimer();
const char* GetSpeedrunStepName( SPEEDRUN_STEP step );
int GetSpeedrunStepValue( SPEEDRUN_STEP step );
const char* GetTimeValueFormated( int secondsTotal );

int __fastcall GetTextWidthColored( const char* s );
void __fastcall DrawTextColored( int xPos, int yPos, int xPosEnd, const char* text, int baseFontColor = C_0_White );

std::optional<Gem> FindGemInfoByBaseItemIndex( const int baseItemIndex );
std::optional<Gem> FindGemInfoByTypeAndGrade( GEM gemType, int grade );
void CollectSocketEffects( std::vector<GemEffect>& effectsOut, Item& item );
void ApplyGemEffects( Item& item );

bool CanGrind();
void __fastcall Craft_Open();
void __fastcall Craft_Close();
void __fastcall Craft_Draw();
void __fastcall Craft_MouseMove();
void __fastcall Craft_MouseClick();
void __fastcall Craft_LButtonDown();
bool __fastcall Craft_IsCraftSlot( int slotId );
Item* __fastcall Craft_GetItemBySlotId( int slotId );
void PrintGemEffectToStringBuffer( GemEffect& effect, bool twoHandedWeapon = false );
void __fastcall TryToDropGem(int row, int col);
void __fastcall DropRandomGemForSpecificGrade( int row, int col, int grade );
void __fastcall TryToAddSocketsToItem( Item& item, int qlvl, uchar difficulty );
Item& __fastcall Craft_GetSocketedItem();
void Craft_RecalcItemsReq();
bool Craft_Socketing_IsSocketingItemReady();
bool Craft_Socketing_IsGemSuitableForCurrentItem( Gem& gemInfo );

int Craft_Gems_CalculateUpgradeFinalPrice( int currentGrade );
int Craft_Socketing_CalculateGemInsertFinalPrice( int currentGrade );
int Craft_Socketing_CalculateGemExtractionFinalPrice( int currentGrade );
void SetupCraftPanel();

void __fastcall Enchant_Draw();
void __fastcall Enchant_Open();
void __fastcall Enchant_Close();
void Enchant_LButtonDown();
void Enchant_DrawItem(const DisplayObject& rect, const Item& item);
void Enchant_RetrieveAllItems();
void ApplyEnchants(int itemIndex, int baseItemIndex, i64 seed, int quality, int genVersion, array<short, EnchantMax_5> enchantIndexes, bool v1, uchar difficulty);
void __fastcall Enchant_MouseMove();
void __fastcall Enchant_MouseClick();
void SetupEnchantPanel();

void __fastcall Quench_Draw();
void __fastcall Quench_Open();
void __fastcall Quench_Close();
void Quench_LButtonDown();
void Quench_DrawItem(const DisplayObject& rect, const Item& item);
void Quench_RetrieveAllItems();
void ApplyQuench(int itemIndex, int baseItemIndex, i64 seed, int quality, int genVersion, QUENCH_AFFIX quenchAffix, uchar quenchLevel);
void __fastcall Quench_MouseMove();
void __fastcall Quench_MouseClick();
void SetupQuenchPanel();

void __fastcall Transmute_Open();
void __fastcall Transmute_Close();
void __fastcall Transmute_Draw();
void __fastcall Transmute_MouseMove();
void Transmute_LButtonDown();
void __fastcall Transmute_MouseClick();
void SetupTransmutePanel();
bool __fastcall Transmute_ReadFromSave( HANDLE archiveHandle );
void __fastcall Transmute_WriteToSave();
bool __fastcall Transmute_AutoPutCursorItem(int playerIndex, bool force = false);
Item* __fastcall TransmuteSlots_GetItemBySlotId( int slotId );
bool Transmute_Allowed( const Item& item );
bool CanTransmute();
void TransmuteRecipeListDown();
void TransmuteRecipeListUp();
void Transmute_Start();

void __fastcall DrawRegularCelViaTRN(int Xpos, int Ypos, const char* image, int frame, int width, const uchar*);

void InfoWindow_Init();
void InfoWindow_Free();
void __fastcall InfoWindow_Draw();
void __fastcall InfoWindow_MouseMove();
void __fastcall InfoWindow_MouseDown();
void InfoWindow_Open();
void InfoWindow_Close();
void __fastcall InfoWindow_MouseUp();
void InfoWindow_Prev();
void InfoWindow_Next();
void PlayWalkSFX(int monsterIndex);
void Open_the_Gates_Sacristy();
void Open_the_Gates_Cellar();
void Open_the_Gates_Glacial_Caverns();
void Open_the_Gates_Frozen_Loch();
void Open_the_Gates_Web_Tunnels();
void Open_the_Gates_Arachnid_Lair();
void Open_the_Gates_Fleshdoom_level();
void Open_the_Gates_Fleshdoom_level_2();
void Open_the_Gates_Temple_of_Sunset();
void Open_the_Gates_Fetid_Cavern();
void Open_the_Gates_Springs();
void Open_the_Gates_Mossy_Grotto();
void Open_the_Gates_Blood_Hive();

void TimeReset();
void TimeChange( double shift );
void TimeToggle();
void ChangeMapTile(int row, int col, uint tile);
void DrawBox(const DisplayObject& displayObjectRef, uchar color);
void DrawRect(const DisplayObject& displayObjectRef, uchar color);
void ChangeCursorPos(int dx, int dy);
void FixItemCursor(int sign);

void BerserkRecalcAllTargets(Player& player);
void BerserkInit();
void BerserkRecalcSingleMonster(Player& player, int i);
void BerserkRecalcSingleObject(Player& player, int i);

void __fastcall ResetButtonText(HWND hdlg, int* elemList, int fontIndex, int limitWidth);
void __fastcall WriteTextToElemData(TextWin* elemData, const char* srcText);
void __cdecl PlaySoundTitleMove();
void __fastcall PaintElem(HWND elem);
int __stdcall SDlgDefDialogProc_(HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam);
int __stdcall CallWindowProcA_(WNDPROC lpPrevWndFunc, HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam);
int __fastcall AnimateSelectionPentagram(HWND hdlg, HWND curButton);
int __fastcall CheckScrollPressAndRelease(HWND scroll);
int __fastcall CheckCursorOnButton(HWND hdlg, HWND button, int cursorX, int cursorY);
int __stdcall DefWindowProcA_(HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam);
void __cdecl PlaySoundTitleSelect();
int __fastcall GetScrollAction(HWND scroll, int cursorX, int cursorY);
void __fastcall SetScrollOnElem(HWND hDlg, int nIDDlgItem, int scrollLineCount, int elemIndex);
void __fastcall InitTextElemList(HWND hWnd, int* elemList, int fontIndex);
int __fastcall InitButtonListText(HWND hdlg, int* elemList, int fontIndex, int limitWidth);
void __fastcall InitSelectionAndLoadPentagram(char* pentagramName);
void __cdecl DeletePentagram();
void __fastcall InitScroll(HWND hDlg, int nIDDlgItem);
void __fastcall DeleteScroll(HWND hDlg, int nIDDlgItem);
void __fastcall DeleteElementListData(HWND hDlg, int* elementList);
void __fastcall WriteAndDrawDialogTitle(HWND hdlg, char* text);
void __fastcall LeaveButton(HWND hdlg, HWND button);
void __fastcall SelectButton(HWND button);
void ShowWindowList(HWND hDlg, int* list, int nCmdShow);
void __fastcall DrawTextToElemList(HWND hdlg, int* elementList, int fontIndex);
void __fastcall AdjustScrollRect(RECT* rect, int newLeft, int newTop);
void __cdecl CheckEnter(uint& msg, WPARAM& wParam, LPARAM& lParam);
void __fastcall InitButtonListProc(HWND hwnd, int* elemList);
LPARAM IsMultiplayer();
Portrait getHeroPortrait(CharSaveInfo *saveInfo, CLASS_SELECTION_MODE mode = CSM_SPECIALIZATION );
const char* GetFullClassName( int playerIndex );
void __fastcall DrawPlusCEL(int x, int y, int pic);
bool IsNoNeedToDropItemUsingCtrl();
void XinputVirtualKeyboard();
void HandleSwapSpellOnScroll(int wheelDelta);
void XinputHandleMouseMove();
void XinputFixCursorPosition();
void XinputMain();
void XinputSpellCastingPositioning(Player& player,int currentSpellNumber,int& row, int& col);
void XinputHandleControllerForPlayerAction(int playerIndex);
__forceinline bool IsController(){ return XinputEnabled && curXboxController.IsConnected(); }
void DrawXinputGuide();
bool XinputNotInUse(bool isTooltip = false);
void RestrictMouseToWindow(HWND wWindow);
void ModifyAuraDamageForCoop(int& damage);
int GetNumberOfRelicWithCurrentSpell(int currentSpellNumber);
void BackupSaveIfNeed( int slotIndex );
int MaxDurability(Item& item);
int ResistColor(float resistPercent);
void Player_AC(int playerIndex, int& playerAC);
void MvP_Melee_Max_Block_Chance(int playerIndex, int& maxBlockChance);
void MvP_Ranged_Max_Block_Chance(int playerIndex, int& maxBlockChanceRanged);
void MvP_Melee_Autohit_chance(int playerIndex, int& autoHit, int& automiss);
void MvP_Melee_Block_Chance(int monsterIndex, int playerIndex, int& playerBlockChance);
void __fastcall Limit_MvP_Ranged_Arrow_ToHit_Chance(int playerIndex, int& realToHitChance, int& Autohit, int& Automiss);
void GetIronmanSaverate(int& ironman_saverate);
ONSCREEN_MESSAGE BlockMessageToQuestDungeon();
Affix* CurAffixesMagic();
uint grand();
void ReverseNumber(int& revNumber);//mor: gets a number and returns the reversed number, for instance: 1675 -> 5761
int NewHeroTitle();
int CharSelectTitle();
char* SaveFolderName();
char* SaveFileStash();
int PlayerMaxLevel();
i64 LevelUp(uint charLevel);
int SpellLevelBookMax();
