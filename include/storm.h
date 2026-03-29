#ifndef _storm_h_
#define _storm_h_
#else
#error reinclude storm.h
#endif

#ifdef storm_cpp
// в windows компилируется и работает без StormPort.h
#include "../lib/StormPort.h" // для компиляции StormLib в Linux
#include "../lib/StormLib.h"
#include "../lib/pklib/pklib.h"

#endif

enum ADDITIONAL_SURFACE_TYPE
{
    astNone = 0,
    astBackBuffer = 2,
    astOffscreenSurface = 3
};

typedef union {
    LPDIRECTDRAWSURFACE Surfaces[4];
    struct {
        LPDIRECTDRAWSURFACE mainSurface;
        LPDIRECTDRAWSURFACE backBuffer;
        LPDIRECTDRAWSURFACE offscreenSurface;
        LPDIRECTDRAWSURFACE unknownSurface;
    };
} StormSurfaces, *PStormSurfaces;

#ifndef storm_cpp
// Original dll funcs
extern "C" {
#endif

// SNet*
int __stdcall	eSNetCreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameType, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a9, int *playerID);	// 101
int __stdcall	eSNetDestroy();		// 102
int __stdcall	eSNetEnumProviders(int (__stdcall *setNetGame)(unsigned int gameIndex, char *gameName, char *gameStat));	// 103 // Modem providers (games in AOL may be)
int __stdcall	eSNetEnumDevices(int a1, int a2, int (__stdcall *setNetGame)(unsigned int gameIndex, char *gameName, char *gameStat), int a4);	// 104 // Local network devices (games)
int __stdcall	eSNetEnumGames(int a1, int (__stdcall *initProviderDesc)(int provider, char (&text1)[128], char (&requirements)[128], SProvider *sProvider));	// 105
int __stdcall	eSNetDropPlayer(int a1, unsigned int a2);	// 106
int __stdcall	eSNetGetGameInfo(int a1, void *a2, unsigned int a3, int* byteswritten);	// 107
int __stdcall	eSNetGetNetworkLatency(unsigned int a1, int a2);	// 108
int __stdcall	eSNetGetNumPlayers(int *a1, int *a2, int a3);	// 109
int __stdcall	eSNetGetOwnerTurnsWaiting(uint* turns);	// 110 // SNetGetOwnerTurnsWaiting retrieves the number of anticipated turns.
int __stdcall	eSNetGetPerformanceData(int a1, int a2, int a3, int a4, LPFILETIME lpFileTime, int a6); // 111
int __stdcall	eSNetGetPlayerCaps(int a1, int a2);				// 112
int __stdcall	eSNetGetPlayerName(int a1, char *Dest, size_t Count); // 113
int __stdcall	eSNetGetProviderCaps( CAPS* providerCaps );	// 114 // SNetGetProviderCaps retrieves network provider capacity information.
int __stdcall	eSNetGetTurnsInTransit( uint* turns );	// 115 // SNetGetTurnsInTransit retrives the number of turns queued for sending over the network.
int __stdcall	eSNetInitializeDevice(int gameIndex, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo);	// 116
int __stdcall	eSNetInitializeProvider( int provider, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo);	// 117
int __stdcall	eSNetJoinGame(int id, char *gameName, char *gamePassword, char *playerName, char *userStats, int *playerid); // 118
int __stdcall	eSNetLeaveGame(int a1);	// 119
int __stdcall	eSNetPerformUpgrade(int* upgradestatus);	// 120
int __stdcall	eSNetReceiveMessage( unsigned int* senderPlayerIndex, void** packet, unsigned int* size);	// 121
int __stdcall	eSNetReceiveTurns(unsigned int a1, int a2, void *arraydata, void *arraydatabytes, void *arrayplayerstatus);	// 122
int __stdcall	eSNetRegisterEventHandler(int a1, int a2);	// 123
int __stdcall	eSNetResetLatencyMeasurements();		// 124
int __stdcall	eSNetSelectGame(int flag, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerIndex);	// 125
int __stdcall	eSNetSelectProvider(LPARAM a1,  void *a2,  void *a3,  void *a4, void *a5, int a6);	// 126
int __stdcall	eSNetSendMessage(int playerID, void *packet, size_t databytes);	// 127
int __stdcall	eSNetSendTurn( char *data, size_t databytes );	// 128 Sends a turn (data packet) to all players in the game. Network data is sent using class 02 and is retrieved by the other client using SNetReceiveTurns().
int __stdcall	eSNetSetBasePlayer(int a1);	// 129
int __stdcall	eSNetSetGameMode(int a1);	// 130
int __stdcall	eSNetUnregisterEventHandler(int a1, int a2);	// 131

int __stdcall	NetReceiveMessage( unsigned int* senderPlayerIndex, void** packet, unsigned int* size);	// 121
int __stdcall	NetReceiveTurns(unsigned int a1, int a2, void *arraydata, void *arraydatabytes, void *arrayplayerstatus);	// 122
int __stdcall	NetSendMessage(int playerID, void *packet, size_t databytes);	// 127
int __stdcall	NetSendTurn( char *data, size_t databytes );	// 128 Sends a turn (data packet) to all players in the game. Network data is sent using class 02 and is retrieved by the other client using SNetReceiveTurns().

// SDlg*
void *__stdcall	eSDlgBeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint); // 201
int __stdcall	eSDlgBltToWindow(HWND hWnd, HRGN a2, char *a3, int a4, void *buffer, RECT *rct, SIZE *size, int a8, int a9, DWORD rop); // 202
int __stdcall	eSDlgCheckTimers();	// 203
void *__stdcall	eSDlgCreateDialog(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam); // 204
int __stdcall	eSDlgCreateDialogParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam); // 205
int __stdcall	eSDlgDefDialogProc(HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam); // 206
int __stdcall	eSDlgDestroy();	// 207
int __stdcall	eSDlgDialogBoxIndirectParam(int a1, int a2, HWND hWndParent, LPVOID lpParam, LPARAM lParam); // 208
int __stdcall	eSDlgDialogBoxParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam); // 209
int __stdcall	eSDlgDrawBitmap(HWND hWnd, int a2, HRGN hrgnSrc2, int a4, int a5, int a6, char a7); // 210
int __stdcall	eSDlgEndDialog(HWND hDlg, int result); // 211
int __stdcall	eSDlgEndPaint(HWND hWnd, PAINTSTRUCT *lpPaint); // 212
int __stdcall	eSDlgKillTimer(HWND hWnd, int timerId);	// 213
int __stdcall	eSDlgSetBaseFont(int a1, int a2, int a3, int a4, char *Source); // 214
int __stdcall	eSDlgSetBitmap(HWND hWnd, HWND parent, char *src, int mask1, int flags, void *picBuf, RECT *rect, int width, int height, int mask2); // 215
int __stdcall	eSDlgSetControlBitmaps(HWND parentwindow, int *controlIdLIst, int a3, char *buffer, SIZE *size, int flags, int mask); // 216
int __stdcall	eSDlgSetCursor(HWND hWnd, HCURSOR hcursor, char *cursorName, int *maybeResult);		// 217
int __stdcall	eSDlgSetSystemCursor(unsigned __int8 *cursorAnim, unsigned __int8 *cursorBuf, SIZE *size, char *cursorName);	// 218
int __stdcall	eSDlgSetTimer(HWND hWnd, int timerId, int interval, void *timerProc);					// 219
int __stdcall	eSDlgUpdateCursor();										// 220

// SFile*
int __stdcall	eSFileAuthenticateArchive(int a1, int a2);		// 251
int __stdcall	eSFileCloseArchive_lib(HANDLE hMpq);
int __stdcall	eSFileCloseArchive( HANDLE hMpq );							// 252
int __stdcall	eSFileCloseFile_lib(HANDLE hFile);
int __stdcall	eSFileCloseFile(HANDLE hFile);							// 253
int __stdcall	eSFileDdaBegin(int a1, int a2, unsigned int a3);		// 254
int __stdcall	eSFileDdaBeginEx( HANDLE directsound, uint bufferSize, uint flags, uint offset, int volume, int pan, void* reserved );	// 255
int __stdcall	eSFileDdaDestroy();										// 256
int __stdcall	eSFileDdaEnd(HANDLE directsound);										// 257
int	__stdcall	eSFileDdaGetPos(HANDLE, uint*, uint*);								// 258
int __stdcall	eSFileDdaGetVolume(int a1, int a2, int a3);					// 259
int __stdcall	eSFileDdaInitialize(int a1);								// 260
int __stdcall	eSFileDdaSetVolume(int a1, int a2, int a3);	// 261
int __stdcall	eSFileDestroy();										// 262
int __stdcall	eSFileEnableDirectAccess(int a1);							// 263
int __stdcall	eSFileGetFileArchive(HANDLE handle, int a2);				// 264
uint __stdcall	eSFileGetFileSize_lib(HANDLE hFile, uint * pdwFileSizeHigh);
DWORD __stdcall	eSFileGetFileSize( HANDLE hFile, LPDWORD lpFileSizeHigh );	// 265
int __stdcall	eSFileOpenArchive_lib( char * szMpqName, uint dwPriority, uint dwFlags, HANDLE * phMpq);
int __stdcall	eSFileOpenArchive( char* szMpqName, uint dwPriority, uint dwFlags, MPQ** mpq );	// 266
int __stdcall	eSFileOpenFile( const char *fileName, HANDLE *fileHandle );								// 267
int __stdcall	eSFileOpenFileEx_lib(HANDLE hMpq,  char * szFileName, uint dwSearchScope, HANDLE * phFile);
int __stdcall	eSFileOpenFileEx( HANDLE handle, char *name, int a3, HANDLE* fHandle );		// 268
int __stdcall	eSFileReadFile_lib(HANDLE hFile, VOID * lpBuffer, uint dwToRead, uint * pdwRead, LPOVERLAPPED lpOverlapped);
int __stdcall	eSFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG lpDistanceToMoveHigh);	// 269
int __stdcall	eSFileSetBasePath( char* path);	// 270
uint __stdcall	eSFileSetFilePointer_lib(HANDLE hFile, LONG lFilePos, LONG * pdwFilePosHigh, uint dwMethod);
DWORD __stdcall	eSFileSetFilePointer( HANDLE fhandle, LONG lDistanceToMove, int a3, DWORD dwMoveMethod );	// 271
LCID  __stdcall eSFileSetLocale_lib(LCID lcNewLocale);
int __stdcall	eSFileSetLocale(int a1);				// 272
//				eSFileGetBasePath(int, int);			// 273
//				eSFileGetArchiveName(int, int, int);	// 275
//				eSFileGetFileName(int, int, int);	// 276
//				eSFileAuthenticateArchiveEx(int, int, int, LONG lDistanceToMove, int, DWORD NumberOfBytesRead); // 299
int __stdcall	eStormDestroy();	// 301

// SBlt*
int __stdcall	eSBltDestroy();															// 311
int __stdcall	eSBltGetSCode(int a1, char *Dest, size_t Count, int a4);					// 312
void __stdcall	eSBltROP3(void *lpDstBuffer, void *lpSrcBuffer, int srcDrawWidth, int srcDrawHeight, int dstWidth, int srcWidth, int a7, DWORD rop);	// 313
int __stdcall	eSBltROP3Clipped(void *lpDstBuffer, RECT *lpDstRect, POINT *lpDstPt, int a4, void *lpSrcBuffer, RECT *lpSrcRect, POINT *lpSrcPt, int a8, int a9, DWORD rop);	// 314
int __stdcall	eSBltROP3Tiled(unsigned __int8 *dstSurface, RECT *dstRect, int dstPitch, unsigned __int8 *srcSurface, RECT *srcRect, int srcPitch, POINT *lpSrcPt, int a8, int a9, unsigned int rop);	// 315

// SBmp*
int __stdcall	eSBmpDecodeImage(int a1, int a2, int a3, int a4, int a5, unsigned int a6, int a7, int a8, int a9);	// 321
int __stdcall	eSBmpLoadImage(const char *pszFileName, PALETTEENTRY *pPalette, void *pBuffer, DWORD dwBuffersize, int *pdwWidth, int *pdwHeight, int *pdwBpp);					// 323
int __stdcall	eSBmpSaveImage(char* Str, int a2, uint a3, int a4, int a5, int a6);							// 324
//						SBmpAllocLoadImage(char *filename,int,int,int,int,int,int,int)								// 325
//						SBmpSaveImageEx(char *str,int,int,int,DWORD NumberOfBytesWritten,int,LPCVOID lpBuffer)		// 326

// SCode*
int __stdcall	eSCodeCompile(int a1,  char *a2, int a3, unsigned int a4, unsigned int a5, int a6);	// 331
int __stdcall	eSCodeDelete(void* a1);			// 332
int __stdcall	eSCodeDestroy();			// 333
void __stdcall	eSCodeExecute(int a1/*ebp*/, int a2, int a3);	// 334
int __stdcall	eSCodeGetPseudocode(int a1, char *Dest, size_t Count); // 335

// SDraw*
int __stdcall	eSDrawAutoInitialize(HINSTANCE hInstance, LPCSTR lpClassName, LPCSTR lpCaption, WNDPROC wndProc, ADDITIONAL_SURFACE_TYPE additionalSurfaceType, int displayWidth, int sHeight, int bytesPerPixel); // 341
int __stdcall	eSDrawCaptureScreen(void *a1, int a2, char *Source);					// 342
int __stdcall	eSDrawShowCursor(int surfaceNumber);															// 343	SDrawClearSurface ???
int __stdcall	eSDrawDestroy();																// 344
int __stdcall	eSDrawFlipPage();																// 345
HWND __stdcall	eSDrawGetFrameWindow(HWND *sdraw_framewindow);														// 346
int __stdcall	eSDrawGetObjects(LPDIRECTDRAW* a1, int a2, int a3, int a4, int a5, int a6, int a7);			// 347
int __stdcall	eSDrawGetScreenSize(int a1, int a2, int a3);										// 348
int __stdcall	eSDrawGetServiceLevel(int a1);														// 349
int __stdcall	eSDrawLockSurface( int surfaceNumber, LPRECT rect, int a3, int a4, int a5 );					// 350
int __stdcall	eSDrawManualInitialize( HWND hwnd, LPDIRECTDRAW ddrawPtr, LPDIRECTDRAWSURFACE screenSurfacePtr, LPDIRECTDRAWSURFACE( __fastcall *convertToTrueColorFunc )(uchar*), LPDIRECTDRAWSURFACE offscreenSurfacePtr, LPDIRECTDRAWSURFACE unknownSurfacePtr, LPDIRECTDRAWPALETTE ddrawPalettePtr, HPALETTE systemPalette );	// 351
int __stdcall	eSDrawMessageBox(LPCSTR lpText, LPCSTR lpCaption, UINT uType);						// 352
HWND __stdcall	eSDrawPostClose();																// 353
int __stdcall	eSDrawRealizePalette();															// 354
int __stdcall	eSDrawSelectGdiSurface(void* This, int a2, int a3);									// 355
int __stdcall   eSDrawUnlockSurface(int a1, int surfaceNumber, int a3, int sizeOfArray, LPRECT arrayOfRect);	// 356
int __stdcall	eSDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);	// 357
int __stdcall	eSDrawUpdateScreen(int a1, int a2, int a3);									// 358

// SEvt*
int __stdcall	eSEvtDestroy();																		// 371
int __stdcall	eSEvtDispatch(int a1, int a2, int a3, int a4);											// 372
int __stdcall	eSEvtRegisterHandler(int This, int a2, int a3, int a4, int a5, int a6);	// 373
int __stdcall	eSEvtUnregisterHandler(int a1, int a2, int a3, int a4);									// 374
int __stdcall	eSEvtUnregisterType(int a1, int a2);													// 375

// SGdi*
int __stdcall	eSGdiBitBlt(int a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned int a8, int a9);	// 381
int __stdcall	eSGdiCreateFont( void *a1, int a2, int a3, int a4, int a5, int a6,  void *a7, int a8);	// 382
int __stdcall	eSGdiDeleteObject(HANDLE handle);														// 383
int __stdcall	eSGdiDestroy();																// 384
int __stdcall	eSGdiExtTextOut(int a1, int a2, int a3, int a4, unsigned int a5, int a6, int a7, int a8, unsigned int a9);	// 385
int __stdcall	eSGdiImportFont(HFONT handle, void *sFont);												// 386
int __stdcall	eSGdiLoadFont(char *Str, int a2, int a3, int a4, int a5, int a6);				// 387
void __stdcall	eSGdiRectangle(int a1, int a2, int a3, int a4, int a5, unsigned int a6);		// 388
int __stdcall	eSGdiSelectObject(int handle);														// 389
int __stdcall	eSGdiSetPitch(int pitch);															// 390
void __stdcall	eSGdiTextOut(unsigned __int8 *bmp, int x, int y, unsigned int flag, char *text, unsigned int lenght);	// 391

// SMem*
int __stdcall	eSMemAlloc(size_t amount, char *logfilename, int logline, char defaultValue);	// 401
int __stdcall	eSMemDestroy();														// 402
int __stdcall	eSMemFree(void *location, char *logfilename, int logline, int defaultValue);		// 403
//404			eSMemGetSize()
//405			eSMemReAlloc(int,int,int,int,int);

// SMsg*
int __stdcall	eSMsgDestroy();																// 411
int __stdcall	eSMsgDispatchMessage(int a1, int a2, unsigned int a3, int a4, int a5, int a6);	// 412
WPARAM __stdcall eSMsgDoMessageLoop(int (__stdcall *checkFunct)(uint), int a2);						// 413
int __stdcall	eSMsgRegisterCommand(int a1, int a2, int a3);							// 414
int __stdcall	eSMsgRegisterKeyDown(int a1, int a2, int a3);							// 415
int __stdcall	eSMsgRegisterKeyUp(int a1, int a2, int a3);										// 416
int __stdcall	eSMsgRegisterMessage(int a1, int a2, int a3);									// 417

// SReg*
int __stdcall	eSRegLoadData( char* Source, char* valueName, int a3, void* lpData, int a5, uint* lpcbData);	// 421
int __stdcall	eSRegLoadString( char *keyname, char *valuename, unsigned __int8 flags, char *buffer, size_t buffersize );	// 422
int __stdcall	eSRegLoadValue( char *key, char *value, int flags, int* data );						// 423 
int __stdcall	eSRegSaveData( char* Source, char* valueName, int a3, void *lpData, DWORD cbData );	// 424
int __stdcall	eSRegSaveString(char *keyname, char *valuename, unsigned int flags, char *string);			// 425
int __stdcall	eSRegSaveValue( char* Source, char* valueName, int a3, int Data );					// 426

// STrans*
int __stdcall	eSTransBlt(void *lpSurface, int x, int y, int width, HANDLE hTrans);							// 431
int __stdcall	eSTransBltUsingMask(void *a1,  void *a2, int a3, int a4, int a5);				// 432
int __stdcall	eSTransCreate(unsigned __int8 *pBuffer, int width, int height, int bpp, TransSize *transSize, int bufferSize, void **phTransOut);		// 433
int __stdcall	eSTransDelete(HANDLE hTrans);																// 434
int __stdcall	eSTransDestroy();																// 435
int __stdcall	eSTransDuplicate(int a1, int a2);													// 436
int __stdcall	eSTransIntersectDirtyArray(int a1, int a2, unsigned __int8 a3, int a4);				// 437
int __stdcall	eSTransInvertMask(int a1, int a2);													// 438
int __stdcall	eSTransLoadI(char *Str, int a2, int a3, int a4);									// 439
int __stdcall	eSTransSetDirtyArrayInfo(int a1, int a2, int a3, int a4);		// 440
int __stdcall	eSTransUpdateDirtyArray(int a1, char a2, int a3, int a4, int a5, int a6);			// 441
int __stdcall	eSTransLoadE(int a1, int a2, int a3);												// 442

// SVid*
int __stdcall	eSVidDestroy();																	// 451
int __stdcall	eSVidGetSize(int a1, int a2, int a3, int a4);										// 452
int __stdcall	eSVidInitialize(int a1);															// 453
int __stdcall	eSVidPlayBegin(char *Str, int a2, int a3, int a4, int a5, int a6, void* videoHandler);			// 454
int __stdcall	eSVidPlayBeginFromMemory(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, int a3, int a4, int a5, int a6, int a7, int a8); // 455
int __stdcall	eSVidPlayContinue();	// 456
int __stdcall	eSVidPlayContinueSingle(int a1, int a2, int a3);	// 457
int __stdcall	eSVidPlayEnd(void* videoHandler);	// 458

// SErr*
int __stdcall	eSErrDisplayError(uint a1, int a2, uint exitCode, char* a4, int a5, UINT uExitCode); // 461
int __stdcall	eSErrGetErrorStr( DWORD dwMessageId, LPSTR lpBuffer, DWORD nSize);	// 462
DWORD __stdcall	eSErrGetLastError();							// 463
int __stdcall	eSErrAddErrorStr(WORD Errcode, char* text, int a3);	// 464
void __stdcall	eSErrSetLastError(DWORD dwErrCode);	// 465

// SCmd*
int __stdcall	strm2_471();	// 471
int __stdcall	strm2_472(int a1);	// 472
int __stdcall	strm2_473(int a1);	// 473
int __stdcall	strm2_474(int a1, char *Dest, int a3); // 474
int __stdcall	strm2_475(int a1, int a2, int a3, int a4);	// 475 ProcessToken???
int __stdcall	strm2_476(int a1, unsigned int a2);	// 476
int __stdcall	strm2_477(int a1, int a2, char *a3, int a4, int a5, int a6, int a7, int a8);	// 477

int __stdcall	InitStorm(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

#ifndef storm_cpp
}
#endif

//*
#define SNetCreateGame eSNetCreateGame
#define SNetDestroy eSNetDestroy
#define SNetDropPlayer eSNetDropPlayer
#define SNetGetGameInfo eSNetGetGameInfo
#define SNetGetOwnerTurnsWaiting eSNetGetOwnerTurnsWaiting
#define SNetGetProviderCaps eSNetGetProviderCaps
#define SNetGetTurnsInTransit eSNetGetTurnsInTransit
#define SNetInitializeProvider eSNetInitializeProvider
#define SNetLeaveGame eSNetLeaveGame
#define SNetPerformUpgrade eSNetPerformUpgrade
#define SNetRegisterEventHandler eSNetRegisterEventHandler
#define SNetSetBasePlayer eSNetSetBasePlayer
#define SNetUnregisterEventHandler eSNetUnregisterEventHandler
#define SFileCloseArchive eSFileCloseArchive
#define SFileCloseFile eSFileCloseFile
#define SFileDdaBeginEx eSFileDdaBeginEx
#define SFileDdaDestroy eSFileDdaDestroy
#define SFileDdaEnd eSFileDdaEnd
#define SFileDdaGetPos eSFileDdaGetPos
#define SFileDdaInitialize eSFileDdaInitialize
#define SFileDdaSetVolume eSFileDdaSetVolume
#define SFileEnableDirectAccess eSFileEnableDirectAccess
#define SFileGetFileArchive eSFileGetFileArchive
#define SFileGetFileSize eSFileGetFileSize
#define SFileOpenArchive eSFileOpenArchive
#define SFileOpenFile eSFileOpenFile
#define SFileOpenFileEx eSFileOpenFileEx
#define SFileReadFile eSFileReadFile
#define SFileSetBasePath eSFileSetBasePath
#define SFileSetFilePointer eSFileSetFilePointer
#define SDrawDestroy eSDrawDestroy
#define SDrawGetFrameWindow eSDrawGetFrameWindow
#define SDrawManualInitialize eSDrawManualInitialize
#define SDrawMessageBox eSDrawMessageBox
#define SDrawRealizePalette eSDrawRealizePalette
#define SDrawUpdatePalette eSDrawUpdatePalette
#define SMemAlloc eSMemAlloc
#define SMemFree eSMemFree
#define SRegLoadData eSRegLoadData
#define SRegLoadString eSRegLoadString
#define SRegLoadValue eSRegLoadValue
#define SRegSaveData eSRegSaveData
#define SRegSaveValue eSRegSaveValue
#define SVidDestroy eSVidDestroy
#define SVidInitialize eSVidInitialize
#define SVidPlayBegin eSVidPlayBegin
#define SVidPlayContinue eSVidPlayContinue
#define SVidPlayEnd eSVidPlayEnd
#define SErrGetErrorStr eSErrGetErrorStr
#define StormDestroy eStormDestroy
#ifdef NET_LOG
#define SNetReceiveMessage NetReceiveMessage
#define SNetReceiveTurns NetReceiveTurns
#define SNetSendMessage NetSendMessage
#define SNetSendTurn NetSendTurn
#else
#define SNetReceiveMessage eSNetReceiveMessage
#define SNetReceiveTurns eSNetReceiveTurns
#define SNetSendMessage eSNetSendMessage
#define SNetSendTurn eSNetSendTurn
#endif

// функции используещиеся только в hell2.dll
#define SBltROP3 eSBltROP3
#define SBltROP3Tiled eSBltROP3Tiled
#define SBmpLoadImage eSBmpLoadImage
#define SDlgBeginPaint eSDlgBeginPaint
#define SDlgBltToWindow eSDlgBltToWindow
#define SDlgCheckTimers eSDlgCheckTimers
#define SDlgCreateDialogParam eSDlgCreateDialogParam
#define SDlgDefDialogProc eSDlgDefDialogProc
#define SDlgDialogBoxParam eSDlgDialogBoxParam
#define SDlgDrawBitmap eSDlgDrawBitmap
#define SDlgEndDialog eSDlgEndDialog
#define SDlgEndPaint eSDlgEndPaint
#define SDlgKillTimer eSDlgKillTimer
#define SDlgSetBitmap eSDlgSetBitmap
#define SDlgSetControlBitmaps eSDlgSetControlBitmaps
#define SDlgSetCursor eSDlgSetCursor
#define SDlgSetSystemCursor eSDlgSetSystemCursor
#define SDlgSetTimer eSDlgSetTimer
#define SDlgUpdateCursor eSDlgUpdateCursor
#define SDrawShowCursor eSDrawShowCursor
#define SGdiDeleteObject eSGdiDeleteObject
#define SGdiImportFont eSGdiImportFont
#define SGdiSelectObject eSGdiSelectObject
#define SGdiSetPitch eSGdiSetPitch
#define SGdiTextOut eSGdiTextOut
#define SNetEnumProviders eSNetEnumProviders
#define SNetEnumDevices eSNetEnumDevices
#define SNetEnumGames eSNetEnumGames
#define SNetInitializeDevice eSNetInitializeDevice
#define SNetJoinGame eSNetJoinGame
#define SNetSelectGame eSNetSelectGame
#define SRegSaveString eSRegSaveString
#define STransBlt eSTransBlt
#define STransCreate eSTransCreate
#define STransDelete eSTransDelete

//*/
