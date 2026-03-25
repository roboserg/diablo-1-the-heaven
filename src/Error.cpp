#include "stdafx.h"

#define DS_ERRORS_MASK 0x08780000
#define DD_ERRORS_MASK 0x08760000

//----- (00401000) --------------------------------------------------------
char* __fastcall GetErrorText(DWORD dwMessageId)
{
	if( (dwMessageId & 0x1FFF0000) == DS_ERRORS_MASK ){
		FormatDSError(dwMessageId);
	}else if( (dwMessageId & 0x1FFF0000) == DD_ERRORS_MASK ){
		FormatDDError(dwMessageId);
	}else{
		int getStormErrorTextResult = SErrGetErrorStr(dwMessageId, ErrorTextBuffer, sizeof( ErrorTextBuffer ));
		if( !getStormErrorTextResult ){
			if( !FormatMessageA(0x1000u, (LPCVOID)0, dwMessageId, 0x400u, ErrorTextBuffer, sizeof( ErrorTextBuffer ), (va_list*)0) ){
				sprintf(ErrorTextBuffer, "unknown error 0x%08x", dwMessageId);
			}
		}
	}
	// Зачем то очистка от лидирующих переносов строк
	size_t len = strlen(ErrorTextBuffer);
	for( int i = len - 1; i >= 0; ++i ){
		char letter = ErrorTextBuffer[i];
		if( letter != '\r' && letter != '\n' ){
			break;
		}
		ErrorTextBuffer[i] = '\0';
	}
	return ErrorTextBuffer;
}

//----- (004010AE) --------------------------------------------------------
char* __fastcall FormatDDError(HRESULT hr)
{
	switch( hr ){
	case DDERR_CANTPAGEUNLOCK:				strcpy(ErrorTextBuffer, "DDERR_CANTPAGEUNLOCK");
	case DDERR_NOTPAGELOCKED:				strcpy(ErrorTextBuffer, "DDERR_NOTPAGELOCKED");
	case DD_OK:								strcpy(ErrorTextBuffer, "DD_OK");
	case DDERR_CANTPAGELOCK:				strcpy(ErrorTextBuffer, "DDERR_CANTPAGELOCK");
	case DDERR_BLTFASTCANTCLIP:				strcpy(ErrorTextBuffer, "DDERR_BLTFASTCANTCLIP");
	case DDERR_CANTCREATEDC:				strcpy(ErrorTextBuffer, "DDERR_CANTCREATEDC");
	case DDERR_CANTDUPLICATE:				strcpy(ErrorTextBuffer, "DDERR_CANTDUPLICATE");
	case DDERR_CLIPPERISUSINGHWND:			strcpy(ErrorTextBuffer, "DDERR_CLIPPERISUSINGHWND");
	case DDERR_DIRECTDRAWALREADYCREATED:	strcpy(ErrorTextBuffer, "DDERR_DIRECTDRAWALREADYCREATED");
	case DDERR_EXCLUSIVEMODEALREADYSET:		strcpy(ErrorTextBuffer, "DDERR_EXCLUSIVEMODEALREADYSET");
	case DDERR_HWNDALREADYSET:				strcpy(ErrorTextBuffer, "DDERR_HWNDALREADYSET");
	case DDERR_HWNDSUBCLASSED:				strcpy(ErrorTextBuffer, "DDERR_HWNDSUBCLASSED");
	case DDERR_IMPLICITLYCREATED:			strcpy(ErrorTextBuffer, "DDERR_IMPLICITLYCREATED");
	case DDERR_INVALIDDIRECTDRAWGUID:		strcpy(ErrorTextBuffer, "DDERR_INVALIDDIRECTDRAWGUID");
	case DDERR_INVALIDPOSITION:				strcpy(ErrorTextBuffer, "DDERR_INVALIDPOSITION");
	case DDERR_NOBLTHW:						strcpy(ErrorTextBuffer, "DDERR_NOBLTHW");
	case DDERR_NOCLIPPERATTACHED:			strcpy(ErrorTextBuffer, "DDERR_NOCLIPPERATTACHED");
	case DDERR_NODC:						strcpy(ErrorTextBuffer, "DDERR_NODC");
	case DDERR_NODDROPSHW:					strcpy(ErrorTextBuffer, "DDERR_NODDROPSHW");
	case DDERR_NODIRECTDRAWHW:				strcpy(ErrorTextBuffer, "DDERR_NODIRECTDRAWHW");
	case DDERR_NOEMULATION:					strcpy(ErrorTextBuffer, "DDERR_NOEMULATION");
	case DDERR_NOHWND:						strcpy(ErrorTextBuffer, "DDERR_NOHWND");
	case DDERR_NOOVERLAYDEST:				strcpy(ErrorTextBuffer, "DDERR_NOOVERLAYDEST");
	case DDERR_NOPALETTEATTACHED:			strcpy(ErrorTextBuffer, "DDERR_NOPALETTEATTACHED");
	case DDERR_NOPALETTEHW:					strcpy(ErrorTextBuffer, "DDERR_NOPALETTEHW");
	case DDERR_NOTAOVERLAYSURFACE:			strcpy(ErrorTextBuffer, "DDERR_NOTAOVERLAYSURFACE");
	case DDERR_NOTFLIPPABLE:				strcpy(ErrorTextBuffer, "DDERR_NOTFLIPPABLE");
	case DDERR_NOTLOCKED:					strcpy(ErrorTextBuffer, "DDERR_NOTLOCKED");
	case DDERR_NOTPALETTIZED:				strcpy(ErrorTextBuffer, "DDERR_NOTPALETTIZED");
	case DDERR_OVERLAYNOTVISIBLE:			strcpy(ErrorTextBuffer, "DDERR_OVERLAYNOTVISIBLE");
	case DDERR_PRIMARYSURFACEALREADYEXISTS:	strcpy(ErrorTextBuffer, "DDERR_PRIMARYSURFACEALREADYEXISTS");
	case DDERR_REGIONTOOSMALL:				strcpy(ErrorTextBuffer, "DDERR_REGIONTOOSMALL");
	case DDERR_WRONGMODE:					strcpy(ErrorTextBuffer, "DDERR_WRONGMODE");
	case DDERR_XALIGN:						strcpy(ErrorTextBuffer, "DDERR_XALIGN");
	case DDERR_DCALREADYCREATED:			strcpy(ErrorTextBuffer, "DDERR_DCALREADYCREATED");
	case DDERR_INVALIDSURFACETYPE:			strcpy(ErrorTextBuffer, "DDERR_INVALIDSURFACETYPE");
	case DDERR_NOMIPMAPHW:					strcpy(ErrorTextBuffer, "DDERR_NOMIPMAPHW");
	case DDERR_WASSTILLDRAWING:				strcpy(ErrorTextBuffer, "DDERR_WASSTILLDRAWING");
	case DDERR_VERTICALBLANKINPROGRESS:		strcpy(ErrorTextBuffer, "DDERR_VERTICALBLANKINPROGRESS");
	case DDERR_UNSUPPORTEDMASK:				strcpy(ErrorTextBuffer, "DDERR_UNSUPPORTEDMASK");
	case DDERR_UNSUPPORTEDFORMAT:			strcpy(ErrorTextBuffer, "DDERR_UNSUPPORTEDFORMAT");
	case DDERR_TOOBIGWIDTH:					strcpy(ErrorTextBuffer, "DDERR_TOOBIGWIDTH");
	case DDERR_TOOBIGSIZE:					strcpy(ErrorTextBuffer, "DDERR_TOOBIGSIZE");
	case DDERR_TOOBIGHEIGHT:				strcpy(ErrorTextBuffer, "DDERR_TOOBIGHEIGHT");
	case DDERR_SURFACENOTATTACHED:			strcpy(ErrorTextBuffer, "DDERR_SURFACENOTATTACHED");
	case DDERR_SURFACELOST:					strcpy(ErrorTextBuffer, "DDERR_SURFACELOST");
	case DDERR_SURFACEISOBSCURED:			strcpy(ErrorTextBuffer, "DDERR_SURFACEISOBSCURED");
	case DDERR_CANTLOCKSURFACE:				strcpy(ErrorTextBuffer, "DDERR_CANTLOCKSURFACE");
	case DDERR_SURFACEBUSY:					strcpy(ErrorTextBuffer, "DDERR_SURFACEBUSY");
	case DDERR_SURFACEALREADYDEPENDENT:		strcpy(ErrorTextBuffer, "DDERR_SURFACEALREADYDEPENDENT");
	case DDERR_SURFACEALREADYATTACHED:		strcpy(ErrorTextBuffer, "DDERR_SURFACEALREADYATTACHED");
	case DDERR_COLORKEYNOTSET:				strcpy(ErrorTextBuffer, "DDERR_COLORKEYNOTSET");
	case DDERR_OUTOFVIDEOMEMORY:			strcpy(ErrorTextBuffer, "DDERR_OUTOFVIDEOMEMORY");
	case DDERR_OVERLAYCANTCLIP:				strcpy(ErrorTextBuffer, "DDERR_OVERLAYCANTCLIP");
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:strcpy(ErrorTextBuffer, "DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
	case DDERR_PALETTEBUSY:					strcpy(ErrorTextBuffer, "DDERR_PALETTEBUSY");
	case DDERR_OUTOFCAPS:					strcpy(ErrorTextBuffer, "DDERR_OUTOFCAPS");
	case DDERR_NOZOVERLAYHW:				strcpy(ErrorTextBuffer, "DDERR_NOZOVERLAYHW");
	case DDERR_NOZBUFFERHW:					strcpy(ErrorTextBuffer, "DDERR_NOZBUFFERHW");
	case DDERR_NOVSYNCHW:					strcpy(ErrorTextBuffer, "DDERR_NOVSYNCHW");
	case DDERR_NOTEXTUREHW:					strcpy(ErrorTextBuffer, "DDERR_NOTEXTUREHW");
	case DDERR_NOT4BITCOLOR:				strcpy(ErrorTextBuffer, "DDERR_NOT4BITCOLOR");
	case DDERR_NOT4BITCOLORINDEX:			strcpy(ErrorTextBuffer, "DDERR_NOT4BITCOLORINDEX");
	case DDERR_NOT8BITCOLOR:				strcpy(ErrorTextBuffer, "DDERR_NOT8BITCOLOR");
	case DDERR_NOSTRETCHHW:					strcpy(ErrorTextBuffer, "DDERR_NOSTRETCHHW");
	case DDERR_NOROTATIONHW:				strcpy(ErrorTextBuffer, "DDERR_NOROTATIONHW");
	case DDERR_NORASTEROPHW:				strcpy(ErrorTextBuffer, "DDERR_NORASTEROPHW");
	case DDERR_NOOVERLAYHW:					strcpy(ErrorTextBuffer, "DDERR_NOOVERLAYHW");
	case DDERR_NOTFOUND:					strcpy(ErrorTextBuffer, "DDERR_NOTFOUND");
	case DDERR_NOMIRRORHW:					strcpy(ErrorTextBuffer, "DDERR_NOMIRRORHW");
	case DDERR_NOGDI:						strcpy(ErrorTextBuffer, "DDERR_NOGDI");
	case DDERR_NOFLIPHW:					strcpy(ErrorTextBuffer, "DDERR_NOFLIPHW");
	case DDERR_NOEXCLUSIVEMODE:				strcpy(ErrorTextBuffer, "DDERR_NOEXCLUSIVEMODE");
	case DDERR_NOCOLORKEYHW:				strcpy(ErrorTextBuffer, "DDERR_NOCOLORKEYHW");
	case DDERR_NOCOLORKEY:					strcpy(ErrorTextBuffer, "DDERR_NOCOLORKEY");
	case DDERR_NOCOOPERATIVELEVELSET:		strcpy(ErrorTextBuffer, "DDERR_NOCOOPERATIVELEVELSET");
	case DDERR_NOCOLORCONVHW:				strcpy(ErrorTextBuffer, "DDERR_NOCOLORCONVHW");
	case DDERR_NOCLIPLIST:					strcpy(ErrorTextBuffer, "DDERR_NOCLIPLIST");
	case DDERR_NOALPHAHW:					strcpy(ErrorTextBuffer, "DDERR_NOALPHAHW");
	case DDERR_NO3D:						strcpy(ErrorTextBuffer, "DDERR_NO3D");
	case DDERR_LOCKEDSURFACES:				strcpy(ErrorTextBuffer, "DDERR_LOCKEDSURFACES");
	case DDERR_INVALIDRECT:					strcpy(ErrorTextBuffer, "DDERR_INVALIDRECT");
	case DDERR_INVALIDPIXELFORMAT:			strcpy(ErrorTextBuffer, "DDERR_INVALIDPIXELFORMAT");
	case DDERR_INVALIDOBJECT:				strcpy(ErrorTextBuffer, "DDERR_INVALIDOBJECT");
	case DDERR_INVALIDMODE:					strcpy(ErrorTextBuffer, "DDERR_INVALIDMODE");
	case DDERR_INVALIDCLIPLIST:				strcpy(ErrorTextBuffer, "DDERR_INVALIDCLIPLIST");
	case DDERR_INVALIDCAPS:					strcpy(ErrorTextBuffer, "DDERR_INVALIDCAPS");
	case DDERR_INCOMPATIBLEPRIMARY:			strcpy(ErrorTextBuffer, "DDERR_INCOMPATIBLEPRIMARY");
	case DDERR_HEIGHTALIGN:					strcpy(ErrorTextBuffer, "DDERR_HEIGHTALIGN");
	case DDERR_EXCEPTION:					strcpy(ErrorTextBuffer, "DDERR_EXCEPTION");
	case DDERR_CURRENTLYNOTAVAIL:			strcpy(ErrorTextBuffer, "DDERR_CURRENTLYNOTAVAIL");
	case DDERR_CANNOTDETACHSURFACE:			strcpy(ErrorTextBuffer, "DDERR_CANNOTDETACHSURFACE");
	case DDERR_CANNOTATTACHSURFACE:			strcpy(ErrorTextBuffer, "DDERR_CANNOTATTACHSURFACE");
	case DDERR_ALREADYINITIALIZED:			strcpy(ErrorTextBuffer, "DDERR_ALREADYINITIALIZED");
	case DDERR_INVALIDPARAMS:				strcpy(ErrorTextBuffer, "DDERR_INVALIDPARAMS");
	case DDERR_OUTOFMEMORY:					strcpy(ErrorTextBuffer, "DDERR_OUTOFMEMORY");
	case DDERR_GENERIC:						strcpy(ErrorTextBuffer, "DDERR_GENERIC");
	case DDERR_UNSUPPORTED:					strcpy(ErrorTextBuffer, "DDERR_UNSUPPORTED");
	default:								sprintf(ErrorTextBuffer, "DDERR unknown 0x%x", hr);
	}
	return ErrorTextBuffer;
}

//----- (00401848) --------------------------------------------------------
char* __fastcall FormatDSError(HRESULT hr)
{
	switch( hr ){
	case DSERR_INVALIDCALL:					strcpy(ErrorTextBuffer, "DSERR_INVALIDCALL");
	case DSERR_PRIOLEVELNEEDED:				strcpy(ErrorTextBuffer, "DSERR_PRIOLEVELNEEDED");
	case DSERR_BADFORMAT:					strcpy(ErrorTextBuffer, "DSERR_BADFORMAT");
	case DSERR_NODRIVER:					strcpy(ErrorTextBuffer, "DSERR_NODRIVER");
	case DSERR_ALREADYINITIALIZED:			strcpy(ErrorTextBuffer, "DSERR_ALREADYINITIALIZED");
	case DSERR_BUFFERLOST:					strcpy(ErrorTextBuffer, "DSERR_BUFFERLOST");
	case DS_OK:								strcpy(ErrorTextBuffer, "DS_OK");
	case DSERR_CONTROLUNAVAIL:				strcpy(ErrorTextBuffer, "DSERR_CONTROLUNAVAIL");
	case DSERR_ALLOCATED:					strcpy(ErrorTextBuffer, "DSERR_ALLOCATED");
	case DSERR_INVALIDPARAM:				strcpy(ErrorTextBuffer, "DSERR_INVALIDPARAM");
	case DSERR_OUTOFMEMORY:					strcpy(ErrorTextBuffer, "DSERR_OUTOFMEMORY");
	case DSERR_NOAGGREGATION:				strcpy(ErrorTextBuffer, "DSERR_NOAGGREGATION");
	case E_NOINTERFACE:						strcpy(ErrorTextBuffer, "E_NOINTERFACE");
	default:								sprintf(ErrorTextBuffer, "DSERR unknown 0x%x", hr);
	}
	return ErrorTextBuffer;
}

//----- (00401958) --------------------------------------------------------
char* GetErrorTextForLastError()
{
	return GetErrorText(GetLastError());
}

//----- (00401965) --------------------------------------------------------
__declspec(noreturn) void TerminateWithError(LPCSTR format, ...)
{
	if( IsFullScreen ) SwitchFullscreen(1);
	if( IsDebuggerPresent() ) __debugbreak();
	va_list va;
	va_start(va, format);
	ErrorExitGame();
	if( format ){
		ShowErrorMessageBox(format, va);
	}

	// copy potential bugged save file for debug
	int slot = GetSlotByHeroName(Players[CurrentPlayerIndex].playerName);
	char saveArchivePath[MAX_PATH];
	GetSaveArchivePath(saveArchivePath, MAX_PATH, slot, true);
	string path = saveArchivePath;
	if( exists( path ) ){
		copy_options op = copy_options::overwrite_existing;
		copy_file( path, path + ".err", op );
	}

	CloseGameArchives(0);
	if( ! IsDebuggerPresent() ){
		*(int*)0 = 0; // force exception for crashdump
	}
	exit(1);
}

//----- (00401998) --------------------------------------------------------
void __fastcall ShowErrorMessageBox(LPCSTR format, va_list arglist)
{
	char text[256];
	wvsprintfA(text, format, arglist);
	if( ghMainWnd ){
		SetWindowPos(ghMainWnd, (HWND)0xFFFFFFFE, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	}
	MessageBoxA(ghMainWnd, text, "ERROR", MB_TASKMODAL|MB_ICONERROR);
}

//----- (004019EA) --------------------------------------------------------
void ErrorExitGame()
{
	if( IsErrorExitThreadPresent ){
		if( ErrorExitThread != GetCurrentThreadId() ){
			SleepTh(20000);
		}
	}
	IsErrorExitThreadPresent = 1;
	ErrorExitThread = GetCurrentThreadId();
	ReleaseDDSurface();
	if( MaxCountOfPlayersInGame > 1 ){
		if( SNetLeaveGame(3) ){
			SleepTh(2000);
		}
	}
	SNetDestroy();
	ShowCursor(1);
}

//----- (00401A53) --------------------------------------------------------
void ErrorBoxf(LPCSTR format, ...)
{
	char text[256];
	va_list va;
	va_start(va, format);
	wvsprintfA(text, format, va);
	SDrawMessageBox(text, "Hellfire", MB_TASKMODAL|MB_ICONEXCLAMATION);
}

//----- (00401A88) --------------------------------------------------------
void __fastcall ShowDirectDrawError(DWORD errorCode, int lineNumber, char* file)
{
	if( errorCode ){
		TerminateWithError("Direct draw error (%s:%d)\n%s", file, lineNumber, GetErrorText(errorCode));
	}
}

int DirectSoundErrorCount = 0;
bool DirectSoundNeedRestart = false;
//----- (00401AAB) --------------------------------------------------------
void __fastcall DirectSoundError(DWORD errorCode, int lineNumber, char* file)
{
	if( errorCode ){
		DirectSoundNeedRestart = true;
		++DirectSoundErrorCount;
		FreeAllSound();
		AllSound = false;
		SoundVolume   = VOL_MIN; IsSoundEnable    = 0;
		MusicVolume   = VOL_MIN; IsMusicEnabled   = 0;
		AmbientVolume = VOL_MIN; IsAmbientEnabled = 0;
		// do not terminate on sound errors, just turn sound off
		if(0) TerminateWithError("Direct sound error (%s:%d)\n%s", file, lineNumber, GetErrorText(errorCode));
	}
}

//----- (th3) -------------------------------------------------------------
void DirectSoundErrorCheck()
{
	if( DirectSoundNeedRestart && DirectSoundErrorCount < 1000 ){ // avoid buggy dsound reload each frame, try 1000 times max
		// try to do full restart direct sound on the fly
		if( DirectSoundModule ){
			FreeLibrary(DirectSoundModule);
			DirectSoundModule = 0;
		}
		AllSound = true;
		if( AllocateDirectSound(ghMainWnd) ){
			DirectSoundNeedRestart = false;
			PlayMusic();
			PlayAmbient();
		}else{
			DirectSoundNeedRestart = true;
			++DirectSoundErrorCount;
		}
	}
}

//----- (00401ACE) --------------------------------------------------------
void __fastcall CenterWindow(HWND hwnd)
{
	tagRECT rc;
	GetWindowRect(hwnd, &rc);
	int lWindowWidth = rc.right - rc.left;
	int lWindowHeight = rc.bottom - rc.top;

	int lScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int lScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	/*
	  HDC dc = GetDC(hWnd);
	  lScreenWidth = GetDeviceCaps(dc, HORZRES); //these return width and heigh of display resolution, but it is better pratice to use GetSystemMetrics to obtain 'em
	  lScreenHeight = GetDeviceCaps(dc, VERTRES);
	  ReleaseDC(hWnd, dc);
	*/
	BOOL setWindowPosResult = SetWindowPos(hwnd, HWND_TOP, (lScreenWidth - lWindowWidth) / 2, (lScreenHeight - lWindowHeight) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	if( !setWindowPosResult ){
		TerminateWithError("center_window: %s", GetErrorTextForLastError());
	}
}

//----- (00401B60) --------------------------------------------------------
void __fastcall ErrorIn_CPP_File(int dialogid, DWORD dwMessageId, char* pCppFile, int line)
{
	ErrorExitGame();
	char* cppName = strrchr(pCppFile, '\\');
	if( cppName ){
		++cppName;
	}else{
		cppName = pCppFile;
	}
	char dwInitParam[512];
	sprintf(dwInitParam, "%s\nat: %s line %d", GetErrorText(dwMessageId), cppName, line);

	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)(dialogid), ghMainWnd, DialogFunc, (LPARAM)(dwInitParam));
	if( nResult == -1 ){
		TerminateWithError("ErrDlg: %d", dialogid);
	}
	TerminateWithError(0);
}

//----- (00401BED) --------------------------------------------------------
int __stdcall DialogFunc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_INITDIALOG ){
		SetErrorDialogTitle(hDlg, (LPCSTR)lParam);
		return TRUE;
	}
	if( uMsg != WM_COMMAND ){
		return FALSE;
	}
	if( (ushort)wParam == 1 ){// ok?
		EndDialog(hDlg, TRUE);
		return TRUE;
	}
	if( (ushort)wParam == 2 ){// cancel?
		EndDialog(hDlg, FALSE);
		return TRUE;
	}
	return TRUE;
}

//----- (00401C32) --------------------------------------------------------
void __fastcall SetErrorDialogTitle(HWND hDlg, LPCSTR lpString)
{
	CenterWindow(hDlg);
	if( lpString ){
		SetDlgItemTextA(hDlg, 1000, lpString);
	}
}

//----- (00401C51) --------------------------------------------------------
void __fastcall FileErrDlg(LPARAM dwInitParam)
{
	ErrorExitGame();
	if( !dwInitParam ){
		dwInitParam = (LPARAM)&ZeroString;
	}
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG3, ghMainWnd, DialogFunc, dwInitParam);
	if( nResult == -1 ){
		TerminateWithError("FileErrDlg");
	}
	TerminateWithError(0);
}

//----- (00401C96) --------------------------------------------------------
void __fastcall DiskFreeDlg(LPARAM dwInitParam)
{
	ErrorExitGame();
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG6, ghMainWnd, DialogFunc, dwInitParam);
	if( nResult == -1 ){
		TerminateWithError("DiskFreeDlg");
	}
	TerminateWithError(0);
}

//----- (00401CD2) --------------------------------------------------------
bool InsertCDDlg()
{
	ShowCursor(1);
	INT_PTR nResult = DialogBoxParamA(HInstance, (LPCSTR)ID_DIALOG8, ghMainWnd, DialogFunc, (LPARAM)&ZeroString);
	if( nResult == -1 ){
		TerminateWithError("InsertCDDlg");
	}
	ShowCursor(0);
	return nResult == 1 ? true : false;
}

