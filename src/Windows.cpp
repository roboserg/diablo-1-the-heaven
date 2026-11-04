#include "stdafx.h"

//----- (0041D2FF) -------------------------------------------------------- FileIO
void __fastcall CloseGameArchives(bool exitGame)
{
	ReleasePlayerFile();
	DisableScreenSaver(0);
	if( diabdatMPQPtr ){
		SFileCloseArchive(diabdatMPQPtr);
		diabdatMPQPtr = 0;
	}
	if( helldataMPQPtr ){
		SFileCloseArchive(helldataMPQPtr);
		helldataMPQPtr = 0;
	}
	if( hellfireMPQPtr ){
		SFileCloseArchive(hellfireMPQPtr);
		hellfireMPQPtr = 0;
	}
	if( hfmonkMPQPtr ){
		SFileCloseArchive(hfmonkMPQPtr);
		hfmonkMPQPtr = NULL;
	}
	if( thassaMORPtr ){
		SFileCloseArchive(thassaMORPtr);
		thassaMORPtr = NULL;
	}
	if( thgladMPQPtr ){
		SFileCloseArchive(thgladMPQPtr);
		thgladMPQPtr = NULL;
	}
	if( thmusicMPQPtr ){
		SFileCloseArchive(thmusicMPQPtr);
		thmusicMPQPtr = NULL;
	}
	if (thmusi3MPQPtr) {
		SFileCloseArchive(thmusi3MPQPtr);
		thmusi3MPQPtr = NULL;
	}
	if( hellgfxMPQPtr ){
		SFileCloseArchive(hellgfxMPQPtr);
		hellgfxMPQPtr = NULL;
	}
	if( hellmfMPQPtr ){
		SFileCloseArchive(hellmfMPQPtr);
		hellmfMPQPtr = NULL;
	}
	if( helldfMPQPtr ){
		SFileCloseArchive(helldfMPQPtr);
		helldfMPQPtr = NULL;
	}
	if( thehell2MPQPtr ){// (rt) closing the mpq
		SFileCloseArchive(thehell2MPQPtr);
		thehell2MPQPtr = NULL;
	}
	if (th2devMPQPtr) { // dev mpq
		SFileCloseArchive(th2devMPQPtr);
		th2devMPQPtr = NULL;
	}
	UiDestroy();
	FreeAllSound();
    NetClose();
	ReleaseDDSurface();
	StormDestroy();
	if( exitGame ) ShowCursor(1);
}


//----- (0041D602) --------------------------------------------------------
void __fastcall DisableScreenSaver(char a1)
{
	LSTATUS result; // eax@1
	char v2;        // al@5
	char v3;        // [sp+23h] [bp-1h]@1
	HKEY hKey;      // [sp+1Ch] [bp-8h]@1
	DWORD cbData;   // [sp+18h] [bp-Ch]@3
	DWORD Type;     // [sp+14h] [bp-10h]@3
	uchar Data[16];  // [sp+4h] [bp-20h]@3
	v3 = a1;
	result = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, 0x2001Fu, &hKey);
	if( !result ){
		if( v3 ){
			cbData = 16;
			if( !RegQueryValueExA(hKey, "ScreenSaveActive", 0, &Type, Data, &cbData) )
				SomeBoolFlag_38 = Data[0] != 48;
			v2 = 0;
		}else{
			v2 = SomeBoolFlag_38;
		}
		Data[1] = 0;
		Data[0] = (v2 != 0) + 48;
		RegSetValueExA(hKey, "ScreenSaveActive", 0, 1u, Data, 2u);
		RegCloseKey(hKey);
	}
}

bool VerticalWin = true;
int SystemWidth, SystemHeight;
//----- (0041D69E) --------------------------------------------------------
void CreateMainWindow()
{
	WNDCLASSEXA Dst; // [sp+Ch] [bp-34h]@1
	InitSaveDirectory();
	memset(&Dst, 0, sizeof( Dst ));
	Dst.cbSize = sizeof( Dst );
	Dst.style = CS_VREDRAW | CS_HREDRAW;
	Dst.lpfnWndProc = (WNDPROC)CallHandler1IfExists;
	Dst.hInstance = HInstance;
	Dst.hIcon = LoadIcon(HInstance, (LPCSTR)ID_ICON1);
	Dst.hCursor = LoadCursor(0, (LPCSTR)IDC_ARROW);
	Dst.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	Dst.lpszMenuName = PRJ_NAME;
	Dst.lpszClassName = "THEHELL4";
	Dst.hIconSm = (struct HICON__*)LoadImage(HInstance, (LPCSTR)ID_ICON1, 1, 16, 16, 0);
	if( !RegisterClassExA(&Dst) ){
		TerminateWithError("Unable to register window class");
	}
	if( IsSecondWin ){
		memset( &Dst, 0, sizeof( Dst ) );
		Dst.cbSize = sizeof( Dst );
		Dst.style = CS_VREDRAW | CS_HREDRAW;
		Dst.lpfnWndProc = (WNDPROC) SecWinHandler;
 		Dst.hInstance = 0;
		Dst.hIcon = LoadIcon( HInstance, (LPCSTR) ID_ICON1 );
		Dst.hCursor = LoadCursor( 0, (LPCSTR) IDC_ARROW );
		Dst.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		Dst.lpszMenuName = PRJ_NAME;
		Dst.lpszClassName = "TH4SEC";
		Dst.hIconSm = (struct HICON__*)LoadImage( HInstance, (LPCSTR) ID_ICON1, 1, 16, 16, 0 );
		if( !RegisterClassExA( &Dst ) ){
			TerminateWithError( "Unable to register window class" );
		}
	}
	int startX = 0; //-1920 for second monitor (doesn't work yet)
	int startY = 0; // (rt) for now a window created not at zero is positioned wrongly relative to the desktop mouse
	int startX2 = 0;
	int startY2 = 0;
	if( IsSecondWin ){
		if( VerticalWin ){
			startY2 += ScreenHeight;// + 10;
		} else{
			startX2 += ScreenWidth;// + 10;
		}
	}
	#if centerwindow
	startX = GetSystemMetrics(SM_CXSCREEN) / 2 - ScreenWidth / 2;
	startY = GetSystemMetrics(SM_CYSCREEN) / 2 - ScreenHeight / 2 - 20;
	if( startY < 20 || startX < 20 ){
		startX = startY = 0; 
	}
	#endif
	extern int PrevWinWidth,PrevWinHeight;
	SystemWidth = GetSystemMetrics( SM_CXSCREEN );
	SystemHeight = GetSystemMetrics( SM_CYSCREEN );
	// (rt) restore the window position
	if( !SRegLoadValue( "Hellfire", "Start Window W", 0, &PrevWinWidth )
	 || !SRegLoadValue( "Hellfire", "Start Window H", 0, &PrevWinHeight ) ){
		PrevWinWidth = 0;
		PrevWinHeight = 0;
	}
	if( BigWin && IsFullScreen ){
		WinWidth = SystemWidth;
		WinHeight = SystemHeight;
	}else{
		WinWidth = PrevWinWidth ? PrevWinWidth : ScreenWidth;
		WinHeight = PrevWinHeight ? PrevWinHeight : ScreenHeight;
	}
	if( !SRegLoadValue( "Hellfire", "Start Window X", 0, &StartWindowX )
	 || !SRegLoadValue( "Hellfire", "Start Window Y", 0, &StartWindowY ) ){
		StartWindowX = 0;
		StartWindowY = 0;
	}
	//if( StartWindowX + ScreenWidth/2 > GetSystemMetrics( SM_CXSCREEN )
	// || StartWindowY + ScreenHeight/2 > GetSystemMetrics( SM_CYSCREEN )
	// || StartWindowX < 0 || StartWindowY < 0 ){
	//	StartWindowX = 0;
	//	StartWindowY = 0;
	//}
	startX = StartWindowX;
	startY = StartWindowY;
	if( IsSecondWin ){
		WinWidth = PrevWinWidth = ScreenWidth;
		WinHeight = PrevWinHeight = ScreenHeight;
	}
	if( IsFullScreen || IsSecondWin ){
		startX = 0;
		startY = 0;
	}
	RECT r = { 0, 0, (IsFullScreen && !BigWin ? ScreenWidth : WinWidth)-1, (IsFullScreen && !BigWin ? ScreenHeight : WinHeight )-1 };
	// bad try to get low res on start
	//if( ScreenWidth < 640 && ScreenHeight < 480 ){
	//	WinWidth = ScreenWidth; WinHeight = ScreenHeight;
	//	ScreenWidth = 640; ScreenHeight = 480;
	//	r.right = WinWidth - 1; r.bottom = WinHeight - 1;
	//	PrevWinWidth = WinWidth;
	//	PrevWinHeight = WinHeight;
	//}
	bool popup = IsFullScreen || IsSecondWin;
	if( !popup ){
		AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, 0, 0 );
	}
	HWND hWnd = CreateWindowExA( 0, "THEHELL4", PRJ_NAME,
		popup ? WS_POPUP : WS_OVERLAPPEDWINDOW, startX, startY, r.right - r.left + 1, r.bottom - r.top + 1, 0, 0, HInstance, 0);
	if( !hWnd ){
		TerminateWithError("Unable to create main window");
	}
	if( IsSecondWin ){
		HWND secWnd = CreateWindowExA( 0, "TH4SEC", "TH4 Second",
			WS_POPUP, startX2, startY2, r.right - r.left + 1, r.bottom - r.top + 1, 0, 0, 0, 0 );
		if( !secWnd ){
			TerminateWithError( "Unable to create second window" );
		}
		ShowWindow(secWnd, SW_SHOWNORMAL);
		UpdateWindow(secWnd);
	}
	// wndmode.dll nevermore
	ShowWindow( hWnd, SW_SHOWNORMAL );
	UpdateWindow( hWnd ); // (rt) no visible effect, probably because WM_PAINT isn't handled
	#ifndef USE_32BITCOLOR
	WaitForOldWinExplorerClosed();
	#endif
	SetupScreenResolution(hWnd);
	BlackPalette();
	AllocateDirectSound(hWnd);
	OpenGameArchives();
	DisableScreenSaver(1);
}

//----- (0041D7E1) --------------------------------------------------------
void CloseOldWinExplorer()
{
	HWND momParent = FindingOldWinExplorer();
	if( momParent ){
		momParent = (HWND)PostMessageA(momParent, WM_CLOSE, 0, 0);
		MOMParentFound = 1;
	}
}

//----- (0041D802) --------------------------------------------------------
HWND FindingOldWinExplorer()
{
	HWND foreground;        // eax@1
	HWND next;        // esi@2
	char className[256]; // [sp+4h] [bp-100h]@3
	foreground = GetForegroundWindow();
	while( 1 ){
		next = foreground;
		if( !foreground ){
			break;
		}
		GetClassNameA(foreground, className, 255);
		// (rt) Possibly the window class name of explorer.exe in Windows 95
		if( !_strcmpi(className, "MOM Parent") ){
			break;
		}
		foreground = GetWindow(next, GW_HWNDNEXT);
	}
	return next;
}

//----- (0041D852) -------------------------------------------------------- game
void WaitForOldWinExplorerClosed()
{
	i64 tick = GetTickCountTh(); // (rt) this timer doesn't affect gameplay, not intercepted
	do{
		if( !FindingOldWinExplorer() ){
			break;
		}
		SleepTh( 250 );
	}while( GetTickCountTh() - tick <= 4000_ms ); // (rt) this timer doesn't affect gameplay, not intercepted
}

//----- (0041D880) -------------------------------------------------------- FileIO
void OpenGameArchives()
{
	HANDLE titlePcx; // [sp+Ch] [bp-8h]@5
	memset( &ModuleInfo, 0, sizeof(module_info) );
	ModuleInfo.dwSize = sizeof(module_info);
	ModuleInfo.VersionString = "internal version unknown";
	ModuleInfo.ModuleName = ThFilePath;
	ModuleInfo.MainArchive = MpqPath_Main;
	ModuleInfo.PatchArchive = MpqPath_3_Patch;
	GetGameVersion();
	while( 1 ){
		// (rt) the original hellfire passed the string "DiabloCD"
		diabdatMPQPtr = OpenMPQ(MpqPath_Main, "\\diabdat.mpq", GameCDKey, 1000, 1);
		if( diabdatMPQPtr ){
			break;
		}
		int error;
		UiCopyProtError(&error);
		if( error == 2 ){
			FileErrDlg((LPARAM)"diabdat.mpq");
		}
	}
	if( !File_Open("ui_art\\title.pcx", &titlePcx, ONE_TRY) )
		FileErrDlg((LPARAM)"Main program archive: diabdat.mpq");
	File_Close(titlePcx);
	// (rt) the original hellfire passed the string "DiabloInstall" everywhere below
	thmusicMPQPtr	= OpenMPQ( MpqPath_2, "\\THmusic2.mor", InstallKey, 8200, 0 );
	thmusi3MPQPtr   = OpenMPQ( MpqPath_a, "\\THmusic3.mor", InstallKey, 8300, 0 );
	hellgfxMPQPtr	= OpenMPQ( MpqPath_7, "\\TH4data.mor",	InstallKey, 8500, 0 );
	hellmfMPQPtr	= OpenMPQ( MpqPath_8, "\\THmusic.mor",	InstallKey, 8600, 0 );
	th2devMPQPtr	= OpenMPQ( MpqPath_dev, "\\th2dev.mpq",	InstallKey, 8700, 0 );  // dev mpq
	//----- (th4) ----- Russian localization audio pack: higher priority than
	// TH4data.mor (8500) so its sfx override the originals when language == RU.
	if( GameLanguage == LANG_RU )
		thlocRusMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_rus.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Bulgarian localization datafile. Same Cyrillic font set as the
	// Russian pack (Bulgarian's alphabet is a strict subset of Russian's, so the CEL
	// frames already cover it) but with the SFX folder removed, so Bulgarian keeps the
	// English voice/sfx instead of inheriting the Russian dub. Mounted at 8900 (above
	// TH4data.mor 8500) ONLY when language == BG, so it can never affect any other
	// language. Absent file => OpenMPQ no-ops and BG falls back to the English fonts,
	// which have no Cyrillic glyphs -- so this mount is REQUIRED for readable Bulgarian,
	// unlike the optional ID audio hook above.
	// (th4) Ukrainian mounts this SAME pack, for the same reason and with the same
	// benefit: it rides the Russian Cyrillic font path (see the LANG_UK case in
	// InitLocalization) and needs those CEL/atlas glyphs, while the missing SFX folder
	// keeps the ENGLISH voice/sfx instead of the Russian dub. Mounting TH4loc_rus.mor
	// here instead would drag that dub in, so use the Bulgarian pack.
	// (th4) NOTE: Japanese is deliberately NOT in this list. BG and UK borrow the
	// Bulgarian pack because they ride the Russian Cyrillic CEL path; Japanese ships
	// its own TH4loc_jpn.mor (mounted below) with all three CELs and the CJK grid
	// sheets, so mounting the Bulgarian pack here would only shadow them.
	if( GameLanguage == LANG_BG || GameLanguage == LANG_UK )
		thlocBlgMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_blg.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Indonesian localization audio pack (OPTIONAL). Unlike RU,
	// the Indonesian build does not ship a dedicated high-priority datafile: a
	// fully translated retail DIABDAT.MPQ (dropped in the game folder) is picked
	// up by the engine's normal archive path and needs no code here. This hook is
	// only for the case where you later want to override select sfx/voice via a
	// small pack -- drop "TH4loc_id.mor" in the game folder and it is mounted at
	// 8900 (above TH4data.mor). Absent file => OpenMPQ no-ops, English/base audio.
	if( GameLanguage == LANG_ID )
		thlocIdMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_id.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Portuguese localization datafile. Ships override fonts
	// (smaltext/medtexts/bigtgold.cel with added accented glyphs) under the stock
	// English filenames, plus UI graphics. Mounted at 8900 (above TH4data.mor 8500)
	// when language == PT or FR. French reuses the Portuguese font/codepage path
	// (Codepage_Portuguese + FontIndex*_Portuguese) because those CEL fonts carry
	// the Latin accent glyphs French needs; without this mount the MID/BIG fonts
	// would lack accented glyphs in French. Never affects EN/RU/ID/ES.
	if( GameLanguage == LANG_PT || GameLanguage == LANG_FR || GameLanguage == LANG_DE
	 || GameLanguage == LANG_NL )	// (th4) German and Dutch reuse the PT/FR accent font pack
		// (th4) Dutch needs this mount: it declares Codepage_Portuguese and
		// FontIndex*_Portuguese in Localization.cpp, and those tables point MID/BIG at
		// frames 67/69/73/86 for e-acute / e-diaeresis / i-diaeresis / u-diaeresis.
		// Those frames only exist in TH4loc_prt.mor. Without the mount the tables index
		// into the STOCK English CELs, where those frames are unpainted, so every
		// "malienkolder", "reeel" and "een" would draw a blank mid-word.
		thlocPrtMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_prt.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Turkish localization datafile. Ships the PT/FR CEL fonts with
	// frames 74/87/88 repainted as G-breve / I-dot / S-cedilla, under the STOCK English
	// filenames, so it shadows them via the archive and no font loader needed changing --
	// same trick as TH4loc_pol.mor. Mounted at 8900 ONLY when language == TR. Absent file
	// => OpenMPQ no-ops and the folded glyph tables keep the text readable.
	// NOTE: take ONLY the three font CELs from the PT pack. Any UI art in TH4loc_prt.mor
	// with Portuguese words baked in must NOT be copied here.
	if( GameLanguage == LANG_TR )
		thlocTurMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_tur.mor", InstallKey, 8900, 0 );

	//----- (th4) ----- Spanish localization datafile. Mounted at 8900 (above
	// TH4data.mor 8500) ONLY when language == ES, so its contents never affect
	// EN/RU/ID/PT. Absent file => OpenMPQ no-ops. It holds the Spanish voice dub AND, since 2026-10-07, the
	// Spanish accent fonts (ctrlpan\\smaltext.cel, data\\medtexts.cel, data\\bigtgold.cel, built with the
	// FontIndex*_SpanishAccents tables by the loc toolkit, fonts/es_fonts.py). REQUIRED for Spanish text: the ES
	// font tables point past the stock CELs.
	if( GameLanguage == LANG_ES )
		thlocEspMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_esp.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Polish localization datafile. Ships the FULL Polish font set
	// under the STOCK English filenames (ctrlpan\smaltext.cel, data\medtexts.cel,
	// data\bigtgold.cel, ui_art\font16/24/30/42 .bin + g/s/y .pcx), so it shadows
	// them via the archive and NO font loader needed a language branch. Mounted at
	// 8900 (above TH4data.mor 8500) ONLY when language == PL, so its contents can
	// never affect EN/RU/ID/PT/ES/IT/FR. Absent file => OpenMPQ no-ops and Polish
	// falls back to the English fonts (diacritics blank, everything else fine).
	if( GameLanguage == LANG_PL )
		thlocPolMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_pol.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Czech localization datafile, the Polish way: everything under the STOCK
	// filenames. The in-game CEL fonts (ctrlpan\smaltext.cel, data\medtexts.cel,
	// data\bigtgold.cel) carry the 15 Czech capitals; the meta-menu atlas (ui_art\font16/24/30/42
	// .bin + g/s/y .pcx) carries the 18 caron/ring letters, upper and lower case, at their CP1250
	// positions (the acute letters were already there, shared with CP1252). REQUIRED for Czech:
	// the Czech font tables (dataseg.cpp) point at frames only these CELs have, and the menus
	// would draw Western letters (e-grave for c-caron...) or blanks without the atlas.
	// Mounted at 8900 ONLY when language == CS.
	if( GameLanguage == LANG_CS )
		thlocCzeMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_cze.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Croatian localization datafile: the same set of files as the Czech pack (in-game
	// CEL fonts + meta atlas), with C-acute and D-stroke added. REQUIRED for Croatian. Mounted at 8900
	// ONLY when language == HR.
	if( GameLanguage == LANG_HR )
		thlocHrvMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_hrv.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Chinese localization datafile. Ships the CJK fonts under the
	// STOCK English filenames (ctrlpan\smaltext.cel, data\medtexts.cel,
	// data\bigtgold.cel), so it shadows them via the archive and NO font loader
	// needed a language branch. Each CEL keeps frames 0..255 = the original base
	// font (Latin pixel-identical) and appends GB2312 CJK at frames 256..7023, so
	// letterID = 257 + gbIndex. Mounted at 8900 (above TH4data.mor 8500) ONLY when
	// language == CHI, so its contents can never affect EN/RU/ID/PT/ES/IT/FR/PL.
	// Absent file => OpenMPQ no-ops and Chinese falls back to the English fonts.
	if( GameLanguage == LANG_CHI )
		thlocChiMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_chi.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Korean localization datafile. Same arrangement as the Chinese pack:
	// the three in-game CELs ship under the STOCK English filenames (CtrlPan\SmalText.CEL,
	// Data\MedTextS.CEL, Data\BigTGold.CEL) so they shadow TH4data.mor via the archive and NO
	// font loader needed a language branch. Each CEL keeps frames 0..255 = the original base
	// font (Latin pixel-identical) and appends EUC-KR Hangul from frame KOR_FIRST_ID, so
	// letterID = 257 + korIndex. The META-menu sheets are the exception: they use NEW names
	// (ui_art\font16k/24k/30k .pcx + .wid) because the 94-column grid needs LoadKorSheet(),
	// exactly as the Chinese sheets need LoadChiSheet(). Mounted at 8900 (above TH4data.mor
	// 8500) ONLY when language == KO, so its contents can never affect any other language.
	// Absent file => OpenMPQ no-ops and Korean falls back to the English fonts.
	if( GameLanguage == LANG_KO )
		thlocKorMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_kor.mor", InstallKey, 8900, 0 );
	//----- (th4) ----- Japanese localization datafile. Third member of the CJK family and
	// arranged exactly like the Chinese and Korean packs: the three in-game CELs ship under
	// the STOCK English filenames (CtrlPan\SmalText.CEL, Data\MedTextS.CEL, Data\BigTGold.CEL)
	// so they shadow TH4data.mor and NO font loader needs a language branch. Frames 0..255 stay
	// the base Latin font and EUC-JP kana/kanji are appended from JA_FIRST_ID, so
	// letterID = 257 + jaIndex. Encoding is EUC-JP, NOT Shift-JIS -- Shift-JIS lead bytes
	// 0x81..0x9F collide with the colour-control window 0x80..0x8B (see vars.h). Mounted at
	// 8900 (above TH4data.mor 8500) ONLY when language == JA, so its contents can never affect
	// any other language. Absent file => OpenMPQ no-ops and Japanese falls back to the English
	// fonts, which is also what happens while the pack still carries only source art.
	if( GameLanguage == LANG_JA )
		thlocJpnMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_jpn.mor", InstallKey, 8900, 0 );

	// (th4) Arabic. Same shape as Japanese: its own pack, mounted at 8900 only when the
	// language is AR, so nothing else can ever see it. The pack carries the three stock-named
	// CELs plus 94-column grid sheets for font16/24/30/42 in gold and silver, and a .wid per
	// size -- the .wid matters more here than for CJK because Arabic is drawn PROPORTIONALLY:
	// the cells are fixed width so the frames line up, but the advance comes from the real ink
	// width or joined script reads as spaced-out capitals.
	if( GameLanguage == LANG_AR )
		thlocAraMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_ara.mor", InstallKey, 8900, 0 );

	// (th4) Hindi. Same shape again, but the pack it carries is built differently: the
	// text in LocalizationHIN.cpp is PRE-SHAPED (see vars.h), so what the sheets hold is
	// one image per Devanagari CLUSTER, not per letter -- roughly 1200 cells across 20
	// lead rows instead of Arabic's 188 across two. The .wid files matter for the same
	// reason as Arabic: the advance is proportional, because a bare consonant and one
	// carrying a left matra differ by about half a cell.
	if( GameLanguage == LANG_HIN )
		thlocHinMPQPtr = OpenMPQ( MpqPath_7, "\\TH4loc_hin.mor", InstallKey, 8900, 0 );
}

// (rt) then storm.dll goes through all open mpqs in turn every time
// (rt) and looks for every loaded file in all of them
// (rt) check whether SFileOpenArchive was rewritten in stormlib
// (rt) ideally this should be reworked to return MPQ* 
//----- (0041DA2C) --------------------------------------------------------
char* __fastcall OpenMPQ(char (&dest)[MAX_PATH], char* edx0, char* aRegistryKey, int mpqId, int a3)
{
	char* v6;         // eax@7
	int v7;           // ebx@9
	char* v9;   // [sp+318h] [bp-8h]@1
	char Str2[MAX_PATH];   // [sp+110h] [bp-210h]@1
	char Source[MAX_PATH]; // [sp+Ch] [bp-314h]@5
	char* curMpq = 0; // [sp+31Ch] [bp-4h]@9
	char Str[MAX_PATH];    // [sp+214h] [bp-10Ch]@12
	v9 = edx0;
	if( !GetCurrentDirectoryA(MAX_PATH, Str2) )
		TerminateWithError("Can't get program path");
	FindFileNameInPath(Str2);
	if( !SFileSetBasePath(Str2) ){
		TerminateWithError("SFileSetBasePath");
	}
	if( !GetModuleFileNameA(HInstance, Source, MAX_PATH) ){
		TerminateWithError("Can't get program name");
	}
	v6 = strrchr(Source, '\\');
	if( v6 )
		*v6 = 0;
	FindFileNameInPath(Source);
	strcpy(dest, Str2);
	strcat(dest, v9);
	v7 = a3;
	if( SFileOpenArchive( dest, mpqId, a3, (MPQ**)&curMpq)
	 || strcmp(Source, Str2) && (strcpy(dest, Source), strcat(dest, v9), SFileOpenArchive( dest, mpqId, v7, (MPQ**)&curMpq))
	 || (Str[0] = 0, aRegistryKey)
	 && SRegLoadString( "Archives", aRegistryKey, 0, Str, MAX_PATH)
	 && (FindFileNameInPath(Str), strcpy(dest, Str), strcat(dest, v9), SFileOpenArchive( dest, mpqId, v7, (MPQ**)&curMpq)) )
		return curMpq;
	if( v7 && OpenMPQfromCD(Str, v9, mpqId, (int)&curMpq) ){
		strcpy(dest, Str);
		return curMpq;
	}
	return 0;
}

//----- (0041DBDF) --------------------------------------------------------
char* __fastcall FindFileNameInPath(char* Str)
{
	char* result; // eax@1
	result = strrchr(Str, '\\' );
	if( result ){
		if( !*(result + 1) )
			*result = 0;
	}
	return result;
}

//----- (0041DBF7) -------------------------------------------------------- FileIO
int __fastcall OpenMPQfromCD(char (&str)[MAX_PATH], char* edx0, int a2, int aHandle)
{
	DWORD v4;               // eax@1
	const char* v5;          // edi@1
	const char* v6;          // esi@5
	const char* v7;          // ebx@6
	int result;             // eax@11
	char v9;                // al@7
	char RootPathName[MAX_PATH]; // [sp+Ch] [bp-108h]@1
	v5 = edx0;
	v4 = GetLogicalDriveStringsA(MAX_PATH, RootPathName);
	if( !v4 )
		goto LABEL_11;
	if( v4 > 0x104 )
		goto LABEL_11;
	while( *v5 == 92 )
		++v5;
	v6 = RootPathName;
	if( RootPathName[0] ){
		while( 1 ){
			v7 = v6;
			do
			v9 = * v6++;
			while( v9 );
			if( GetDriveTypeA(v7) == 5 ){
				strcpy(str, v7);
				strcat(str, v5);
				if( SFileOpenArchive(str, a2, 1, (MPQ**)aHandle) )
					break;
			}
			if( !*v6 )
				goto LABEL_11;
		}
		result = 1;
	}else{
LABEL_11:
		result = 0;
	}
	// For NoCD fix
	// return 1;
	return result;
}

//----- (0041DC8C) -------------------------------------------------------- FileIO
void GetGameVersion()
{
	int v0;             // eax@2
	DWORD v1;           // edi@2
	void* v2;			// ebx@3
	void* v3;           // eax@3
	DWORD dwHandle;     // [sp+Ch] [bp-8h]@2
	LPVOID lpBuffer;    // [sp+10h] [bp-4h]@4
	unsigned int puLen; // [sp+8h] [bp-Ch]@4
	
	uint getExe = TRUE;
	getExe = GetModuleFileNameA( HInstance, ThFilePath, 260 );
	if( getExe ){
		// (rt) desynchronization here
		// (rt) in TH2 a size of 1500 bytes is returned for the version information from th2-debug.dll
		// (rt) AllocMem is not called in TH 1
		v0 = GetFileVersionInfoSizeA(ThFilePath, &dwHandle);
		// (rt) old TH versions stored the program version information (The Hell, 1.0.0.0, Synergistic Software, 98.1.13.1, 2007)
		// (rt) in the latest one all version data in the standard exe section was removed
		// (rt) the V key now swaps weapons in game instead of showing the version; this may be related
		v1 = v0;
		if( v0 ){
			v3 = AllocMem(v0);
			v2 = v3;
			if( GetFileVersionInfo(ThFilePath, 0, v1, v3) ){
				if( VerQueryValue((void * const)v2, "\\", &lpBuffer, &puLen) )
					sprintf(GameVersionString, "version %d.%d.%d.%d", *((uint*)lpBuffer + 4) >> 16, *((uint*)lpBuffer + 4) & 0xFFFF, *((uint*)lpBuffer + 5) >> 16, *((uint*)lpBuffer + 5) & 0xFFFF);
			}
			FreeMem(v2);
		}
	}
}

//----- (0041DD2C) --------------------------------------------------------
int __stdcall DefaultHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg <= WM_PAINT ){
		switch( msg ){
		case WM_PAINT:
			break;
		case WM_CREATE:
			ghMainWnd = hWnd;
			break;
		case WM_DESTROY:
			CloseGameArchives(1);
			ghMainWnd = 0;
			PostQuitMessage(0);
			break;
		}
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
	if( msg != WM_CLOSE && msg != WM_ERASEBKGND ){
		if( msg == WM_ACTIVATEAPP ){
			ActivateWindow(hWnd, wParam);
		//#ifndef USE_32BITCOLOR
		}else{
			if( msg == WM_QUERYNEWPALETTE ){
				SDrawRealizePalette();
				return 1;
			}
			if( msg == WM_PALETTECHANGED ){
				if( wParam != (uint)hWnd ){
					SDrawRealizePalette();
				}
			}
		//#endif
		}
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//----- (th2) -------------------------------------------------------------
int __stdcall SecWinHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg ){
	case WM_CREATE:		SecWnd = hWnd;	break;
	case WM_DESTROY:	SecWnd = 0;		break;
	}
	return DefWindowProcA( hWnd, msg, wParam, lParam );
}

//----- (th2) ------------------------------------------------------------
void SetBackgroundFps()
{
	extern int OldMaxFps;
	if( ! IsMainWindowActive || IsMainWindowMinimized ){
		if( MaxFPS > ENGINE_FPS ){ OldMaxFps = MaxFPS; }
		MaxFPS = ENGINE_FPS;
	}else{
		if( OldMaxFps > ENGINE_FPS ){ MaxFPS = OldMaxFps; }
		OldMaxFps = ENGINE_FPS;
	}
}

//----- (0041DDCA) --------------------------------------------------------
void __fastcall ActivateWindow(HWND hWnd, WPARAM wParam)
{
	IsMainWindowActive = wParam;
	// (th4) 2026-09-10 Phase 0 diagnostics (MP "New Game" crash follow-up):
	// correlate device-lost episodes with the window losing/regaining
	// activation. Compiled out unless TH4_D3D_DIAG_ENABLED is defined; the
	// runtime side additionally needs "d3d_diag_on.flag" (same gate as the
	// other D3DDIAG lines in dx_utility.cpp / dx.cpp).
	D3DDIAG("WM_ACTIVATEAPP: active=%d fg=%p main=%p minimized=%d",
		(int)wParam, (void*)GetForegroundWindow(), (void*)ghMainWnd, (int)IsMainWindowMinimized);
	SetBackgroundFps();
	UiAppActivate(wParam); // (rt) in the current th.dll/hell2.dll version this is an empty function (ret)
	//(rt) the original disabled the active window's system menu
	#ifdef NOSYSMENU
	long style = GetWindowLongA(hWnd, GWL_STYLE);
	#endif
	if( IsMainWindowActive ){
		RestoreScreenSurface();
		IsCtrlPressed = GetAsyncKeyState(VK_CONTROL) < 0;
		if( ForceMoveOnCtrl ){
			ForceMove = IsCtrlPressed;
		}
	}
}

bool SizeMaximized = false;
bool IsSizingWindow = false;
bool D3dReset();

//----- (th2) -------------------------------------------------------------
void CursorOn()
{
	CURSORINFO ci = { 0 };
	ci.cbSize = sizeof( ci );
	GetCursorInfo( &ci );
	if( ci.flags == 0 ){
		while( ShowCursor( 1 ) <= 0 );
	}
}

//----- (th2) -------------------------------------------------------------
void CursorOff()
{
	CURSORINFO ci = { 0 };
	ci.cbSize = sizeof( ci );
	GetCursorInfo( &ci );
	if( ci.flags == CURSOR_SHOWING ){
		while( ShowCursor( 0 ) > 0 );
	}
}

//----- (0041DE24) --------------------------------------------------------
int __stdcall CallHandler1IfExists(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int result; // eax@2
	RECT rect{0};
	static UINT prevMsg = 0;
	// do black window background on start
	if( /*msg == WM_ERASEBKGND ||*/ (msg == WM_WINDOWPOSCHANGING && prevMsg < 2) ){
		++prevMsg;
		RECT rc;
		HDC dc = GetDC( hWnd );
		GetWindowRect( hWnd, &rc );
		ScreenToClient(hWnd, (LPPOINT)&rc);
		FillRect( dc, &rc, (HBRUSH)GetStockObject( BLACK_BRUSH ) );
		ReleaseDC(hWnd, dc);
		//return 1;
	}
	//if( msg == WM_NCPAINT ) return DefaultHandler( hWnd, msg, wParam, lParam );
	if( msg == WM_ENTERSIZEMOVE ){
		IsSizingWindow = true;
		PrevRestrictState = false;
		return DefaultHandler( hWnd, msg, wParam, lParam );
	}
	if( msg == WM_EXITSIZEMOVE ){ // (rt) Remember the window position
		IsSizingWindow = false;
		PrevRestrictState = false;
		if( ! IsFullScreen ){
			GetWindowRect( ghMainWnd, &rect );
			StartWindowX = rect.left;
			StartWindowY = rect.top;
			SRegSaveValue( "Hellfire", "Start Window X", 0, StartWindowX /*>= 0 ? StartWindowX : 0*/ );
			SRegSaveValue( "Hellfire", "Start Window Y", 0, StartWindowY /*>= 0 ? StartWindowY : 0*/ );
			SRegSaveValue( "Hellfire", "Start Window W", 0, WinWidth );
			SRegSaveValue( "Hellfire", "Start Window H", 0, WinHeight );
		}
		return DefaultHandler( hWnd, msg, wParam, lParam );
	}
	// (rt) tracking the window size and restoring it 
	if( msg == WM_SIZE ){
		IsMainWindowMinimized = wParam == SIZE_MINIMIZED;
		SetBackgroundFps();
		if( !IsFullScreen ){
			if( wParam == SIZE_MAXIMIZED ){
				SizeMaximized = true;
			}else if( wParam == SIZE_RESTORED ){
				if( SizeMaximized ){
					SizeMaximized = false;
					SetWindowPos( ghMainWnd, HWND_NOTOPMOST, 0, 0, ScreenWidth, ScreenHeight, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
					AdjustDDrawWindow( ghMainWnd );
					WinWidth = ScreenWidth;
					WinHeight = ScreenHeight;
					if( !D3dReset() ){} // TerminateWithError( "Direct 3D device restoring window size error" );
					return DefaultHandler( hWnd, msg, wParam, lParam );
				}
			}else{
				SizeMaximized = false;
			}
		}
		WinWidth = (short) lParam;
		WinHeight = (short) ((uint) lParam >> 16);
		if( ! D3dReset() ){} // TerminateWithError( "Direct 3D device changing window size error" );
		PrevRestrictState = false;
		return DefaultHandler( hWnd, msg, wParam, lParam );
	}
	// (rt) showing/hiding the cursor for window interaction
	if( IsInterface && !IsFullScreen && gbRunGame){
		if( msg == WM_NCHITTEST ){
			int hit = DefaultHandler( hWnd, msg, wParam, lParam );
			if( hit != HTNOWHERE ){
				if( hit != HTCLIENT || !IsMainWindowActive ){
					CursorOn();
				}else{
					CursorOff();
				}
			}
			return hit;
		}
		if( msg == WM_NCMOUSEMOVE ){
			CursorOn();
			return DefaultHandler( hWnd, msg, wParam, lParam );
		}
	}
	if( Handler1 )
		result = Handler1(hWnd, msg, wParam, lParam);
	else
		result = DefaultHandler(hWnd, msg, wParam, lParam);
	return result;
}

//----- (0041DE49) --------------------------------------------------------
ProcHandler __fastcall SetWindowProc( ProcHandler handler)
{
	ProcHandler result = Handler1;
	Handler1 = handler;
	return result;
}

//----- (0041DE55) --------------------------------------------------------
BOOL HandleSystemMessage()
{
	MSG* pMsg; // ST00_4@1
	HWND hwnd;           // ST04_4@1
	UINT msgFilterMin;           // ST08_4@1
	UINT msgFilterMax;           // ST0C_4@1
	UINT removeMsg;           // ST10_4@1
	BOOL result;       // eax@2
	MSG Msg; // [sp+8h] [bp-1Ch]@1
	removeMsg = 1;
	msgFilterMax = 0;
	msgFilterMin = 0;
	hwnd = 0;
	pMsg = &Msg;
	while( 1 ){
		result = PeekMessageA(pMsg, hwnd, msgFilterMin, msgFilterMax, removeMsg);
		if( !result )
			break;
		if( Msg.message != WM_QUIT ){
			TranslateMessage(&Msg);
			DispatchMessageA(&Msg);
		}
		removeMsg = 1;
		msgFilterMax = 0;
		msgFilterMin = 0;
		hwnd = 0;
		pMsg = &Msg;
	}
	return result;
}
