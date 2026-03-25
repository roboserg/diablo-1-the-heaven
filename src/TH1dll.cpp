#include "stdafx.h"

// обработка ошибок 
enum ERROR_CODE: int {
	ER_OK					= 0 ,
	ER_invalid_parametrs	= 1 ,
	ER_no_exe_data_map_file	= 2 ,
	ER_struct_not_found		= 3 ,
	ER_dll_not_found		= 4 ,
	ER_cannot_inject_dll	= 5 ,
	ER_exe_not_found		= 6 ,
	ER_IMPLIB_NOT_LOADED	= 11,
	ER_IMAGE_NO_IMPORT		= 12,
	ER_SEC_PROTECT_FAILED	= 13,
	ER_ldr_heap_not_found	= 14,
	ER_ldr_entry_not_found	= 15,
};

#define DOSHEADER( lpbImage ) ((PIMAGE_DOS_HEADER)lpbImage)
#define PEHEADER( lpbImage ) ((PIMAGE_NT_HEADERS32)((DWORD)lpbImage + DOSHEADER(lpbImage)->e_lfanew))
#define IMAGE_GET_DIRECTORY( lpbImage, DIRECTORY_ID ) (&PEHEADER(lpbImage)->OptionalHeader.DataDirectory[DIRECTORY_ID])

using filesystem::rename;
using filesystem::exists;
using filesystem::copy_file;
using filesystem::create_directory;
using filesystem::copy_options;

HMODULE Storm2Base;

void LoadDlls()
{
	Storm2Base = GetModuleHandle( "strm2.dll" );
	StormReloc = (uint) Storm2Base - 0x1500'0000;

	//if( exists( "ddraw.dll" ) ){
	//	ErrorExit( "You must remove ddraw.dll from the TH 2 directory" );
	//}
	TCHAR ddrawPath[ MAX_PATH ];
	GetWindowsDirectoryA( ddrawPath, MAX_PATH );
	strcat( ddrawPath, "\\system32\\ddraw.dll" );
	HModule = LoadLibraryA( ddrawPath );
}

uint CreateWindowEx_storm_adr;

HWND _stdcall CreateWindowEx_storm( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
BOOL __stdcall GetCursorPos_( LPPOINT lpPoint );
BOOL __stdcall ClientToScreen_( HWND hwnd, LPPOINT lppoint );
BOOL __stdcall ScreenToClient_( HWND hwnd, LPPOINT lppoint );
BOOL __stdcall GetWindowRect_( HWND hwnd, LPRECT lprect );
BOOL __stdcall EnableWindow_( HWND hWnd, BOOL bEnable );
HWND __stdcall GetDesktopWindow_();
HDC __stdcall GetDC_( HWND hwnd );
int __stdcall ReleaseDC_( HWND hwnd, HDC hdc );
HDC __stdcall BeginPaint_( HWND hwnd, LPPAINTSTRUCT lppaint );
int __stdcall EndPaint_( HWND hwnd, const LPPAINTSTRUCT lppaint );

// установка режимов защиты секциям exe 
uint PeSetSectionProtection( HMODULE hModule, ulong flag )
{
	PIMAGE_SECTION_HEADER pish = IMAGE_FIRST_SECTION( PEHEADER( hModule ) ); 
	for( ulong i = 0; i < PEHEADER( hModule )->FileHeader.NumberOfSections; i++, pish++ ){
		if( pish->VirtualAddress ){
			ulong dwOldProtection = 0;
			ulong size = pish->Misc.VirtualSize ? pish->Misc.VirtualSize : pish->SizeOfRawData;
			if( !VirtualProtect( (void*) ((uint) hModule + pish->VirtualAddress), size, flag, &dwOldProtection ) ){
				return ER_SEC_PROTECT_FAILED;
			}
		}
	}
	return ER_OK;
}

void PatchStormWin()
{
	if( PeSetSectionProtection( Storm2Base, PAGE_EXECUTE_READWRITE ) != ER_OK ){
		TerminateWithError( "Can't set PAGE_EXECUTE_READWRITE to memory of: strm2.dll" );
	}
	*(uint*) (0x15045304 /*0x1503329C*/ + StormReloc) = (uint) GetCursorPos_;
	*(uint*) (0x15045284 /*0x15033294*/ + StormReloc) = (uint) ClientToScreen_;
	*(uint*) (0x15045294 /*0x150332A4*/ + StormReloc) = (uint) ScreenToClient_;
	*(uint*) (0x15045340 /*0x150332A8*/ + StormReloc) = (uint) GetWindowRect_;
	*(uint*) (0x15045318 /*0x150332F4*/ + StormReloc) = (uint) EnableWindow_;
	//*(uint*)(0x15033288 + StormReloc) = (uint) GetDesktopWindow_;


	CreateWindowEx_storm_adr = (uint) CreateWindowEx_storm;
	*(uint*) (0x1501238A /*0x150105EF*/ + StormReloc) = (uint) &CreateWindowEx_storm_adr;

	// отключение перехвата PrintScreen storm'ом
	*(uchar*) (0x15010878 + StormReloc) = 0xEB; // short jmp
	
	if( BackgroundSound ){
		*(uchar*)( 0x150176BC + StormReloc ) = 0x80;
		*(uchar*)( 0x150176C5 + StormReloc ) = 0x80;
	}
}

uint StormReloc = 0;

//----- (th3) ---------------------------------------------------------------
void ValidateAffixesEffectId()
{
	char text[ 128 ];
	for( uint i = 0; i < size(AffixEffect::Table) && AffixEffect::Table[i].id != AE_NO; ++i ){
		if( AffixEffect::Table[i].id != i ){
			sprintf( text, "AffixEffect %i has wrong index/posion", i );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
	for( uint i = 0; i < size(BaseActions) && BaseActions[i].id != EA_END_MARKER; ++i ){
		if( BaseActions[i].id != i ){
			sprintf( text, "BaseActions %i has wrong index/posion", i );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
}

//----- (th3) ---------------------------------------------------------------
void ValidateAffixesEnchantId()
{
	char text[ 128 ];
	bitset<countof(AffixesEnchant)> hasId;
	for( int i = 0; i < countof(AffixesEnchant); ++i ){
		Affix& affix = AffixesEnchant[i];
		if( ! affix.Name ) affix.Name = "";
		int index = affix.id - 1;
		if( index < 0 ) continue;
		if( index >= countof(AffixesEnchant) ){
			sprintf( text, "AffixesEnchant[%i] has id %i > AffixesEnchant size %i", i, affix.id, countof(AffixesEnchant) );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
		if( hasId[index] ){ // checking non-unique id
			if( index != i ){
				sprintf( text, "AffixesEnchant[%i] has non-unique id %i", i, affix.id );
				MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
				exit(1);
			}
		}else{
			hasId[index] = 1;
			if( index != i ){ // sort AffixesEnchant by id
				Affix temp = AffixesEnchant[index];
				AffixesEnchant[index] = affix;
				affix = temp;
				--i; // repeat sort for potential unsorted element
			}
		}
	}
}

//----- (th3) ---------------------------------------------------------------
void ValidateBaseItemsId()
{
	char text[ 128 ];
	uint maxBaseId = 0;
	extern BaseItem BaseItemsData[];
	for( size_t i = 0; i < count_BaseItems; ++i ) if( (uint)BaseItemsData[i].baseId > maxBaseId ) maxBaseId = BaseItemsData[i].baseId;
	if( NextNewBaseId != maxBaseId + 1 ){
		sprintf( text, "NextNewBaseId = %i is not equal MaxBaseId = %i + 1", NextNewBaseId, maxBaseId );
		MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
		exit(1);
	}
	if( maxBaseId > count_BaseItems * 2 ){
		sprintf( text, "max BaseItem id = %i is twice more than count_BaseItems = %i, or negative", maxBaseId, count_BaseItems );
		MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
		exit(1);
	}
	size_t fullCount_baseItems = maxBaseId + 1;
	BaseItems = new BaseItem[fullCount_baseItems](); // should zero memory
	for( size_t i = 0; i < fullCount_baseItems; ++i ) BaseItems[i].baseId = -1;
	for( size_t i = 0; i < count_BaseItems; ++i ){
		uint id = BaseItemsData[i].baseId;
		if( BaseItems[id].baseId < 0 ){
			BaseItems[id] = BaseItemsData[i];
		}else{
			sprintf( text, "BaseItemsData[%i] has duplicated baseId = %i", i, id );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
	GOOD_ITEM_COUNT = count_BaseItems * 2;
}

//----- (th3) ---------------------------------------------------------------
void ValidateUniqItemsId()
{
	char text[ 128 ];
	uint maxUniqId = 0;
	extern UniqueItem UniqueItemsData[];
	for( size_t i = 0; i < count_UniqueItems; ++i ) if( (uint)UniqueItemsData[i].uniqId > maxUniqId ) maxUniqId = UniqueItemsData[i].uniqId;
	if( NextNewUniqId != maxUniqId + 1 ){
		sprintf( text, "NextNewUniqId = %i is not equal MaxUniqId = %i + 1", NextNewUniqId, maxUniqId );
		MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
		exit(1);
	}
	if( maxUniqId > count_UniqueItems * 2 ){
		sprintf( text, "max UniqueItem id = %i is twice more than count_UniqueItems = %i, or negative", maxUniqId, count_UniqueItems );
		MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
		exit(1);
	}
	size_t fullCount_uniqueItems = maxUniqId + 1;
	UniqueItems = new UniqueItem[fullCount_uniqueItems](); // should zero memory
	for( size_t i = 0; i < fullCount_uniqueItems; ++i ) UniqueItems[i].uniqId = -1;
	for( size_t i = 0; i < count_UniqueItems; ++i ){
		uint id = UniqueItemsData[i].uniqId;
		if( UniqueItems[id].uniqId < 0 ){
			UniqueItems[id] = UniqueItemsData[i];
		}else{
			sprintf( text, "UniqueItemsData[%i] has duplicated uniqId = %i", i, id );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
	count_UniqueItems = fullCount_uniqueItems;
}

//----- (th3) ---------------------------------------------------------------
void ValidateUniqueItemsToBaseLinks()
{
	char text[ 128 ];
	for( size_t i = 0; i < count_UniqueItems; ++i ){
		int uniqId = UniqueItems[i].uniqId;
		if( uniqId >= 0 ){
			int baseId = UniqueItems[i].baseId;
			if( (uint)baseId >= count_BaseItems || BaseItems[baseId].baseId < 0 ){
				sprintf( text, "UniqueItem %s with uniqId = %i has invalid baseId = %i", UniqueItems[i].Name, uniqId, baseId );
				MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
				exit(1);
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void ValidateSounds()
{
	char text[ 128 ];
	for( int id = 0; id < S_COUNT; ++id ){
		Sound& sound = Sounds[id];
		if( sound.id != id ){
			sprintf( text, "Sounds[%i].id != %i", id, sound.id );
			MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
			exit(1);
		}
	}
	for( int id = S_COUNT; id < countof(Sounds); ++id ){
		Sounds[id].id = (SOUND) id;
		Sounds[id].flag = SF_2_MIX;
	}
}

//----- (th2) ---------------------------------------------------------------
void InitTH2( void* arg )
{
	// код для анализа двух спрайтов и выгрузки trn между ними	
	//if( FILE* trn = fopen( "assa.trn", "wb" ) ){
	//	for( int i = 0; i < 256; ++i ){
	//		fputc( a[i], trn );
	//	}
	//	fclose( trn );
	//}
	//uchar trn[256]; for( int i = 0; i < 256; trn[i] = i++ );
	//if( FILE* light = fopen( R"(C:\Games\plrgfx\assassin\assa_light000)", "rb" ) ){
	//	if( FILE* dark = fopen( R"(C:\Games\plrgfx\assassin\assa_dark000)", "rb" ) ){
	//		FILE* deltaF = fopen( R"(C:\Games\plrgfx\assassin\delta)", "w" );
	//		while( ! feof(light) ){
	//			int l = (uchar)fgetc( light ), d = (uchar)fgetc( dark );
	//			if( l >= 128 && d >= 128 /*&& d > l*/ && d != 255 && l != 255
	//			&&( l >> 3 == 16 && d >> 3 == 16 || l >> 3 == 17 && d >> 3 == 17
	//			 || l >> 3 == 18 && d >> 3 == 18 || l >> 3 == 19 && d >> 3 == 19
	//			 || l >> 4 == 10 && d >> 4 == 10 || l >> 4 == 11 && d >> 4 == 11
	//			 || l >> 4 == 12 && d >> 4 == 12 || l >> 4 == 13 && d >> 4 == 13
	//			 || l >> 4 == 14 && d >> 4 == 14 || l >> 4 == 15 && d >> 4 == 15 ) ){
	//				//int delta = d - l;
	//				fprintf( deltaF, "%d\t%d\n", l, d );
	//			}
	//		}
	//		fclose( deltaF );
	//		fclose( dark );
	//	}
	//	fclose( light );
	//}
	//__debugbreak();

	ValidateAffixesEffectId();
	ValidateAffixesEnchantId();
	ValidateBaseItemsId();
	//ValidateBaseItemsId_Classic();
	ValidateUniqItemsId();
	ValidateUniqueItemsToBaseLinks();
	ValidateSounds();

	std::ios::sync_with_stdio( false );
	GetParamsFromConfig();
	LoadDlls();
	PatchStormWin();
	//SetupDisplayObjectsStructs(); // новый саб, отвечает за присвоение переменным нужных координат и размеров
	// для переключение между v-sync в fullscreen в game и меню
	extern int OldWorldProcess; OldWorldProcess = LoadOnStart ? 1 : 0;

	// динамическая инициализация статических переменных (вызов с++ конструкторов)
	// InitDoublePrice вынести наружу, crit sects переделать в std _lockit, с exception еще не доразобрался
	SetupDeltaCriticalSection();
	SetupDDSurfaceCriticalSection();
	InitLocking(); // это статические конструкторы вызываемые для всех переменных инициализируемых вне main()
	//InitializeExceptionHandler();
	InitDoublePrice();
	SetupTimingCritSection();
	
	uint ver = GetVersion(); 
	WinMajorMinorVer = BYTE1(ver) + ((uchar)ver << 8);

	// init meta menu (ex-hell.dll)
	AttachMetaMenu();
	
	#if fileDirectAccess
	InitStorm( HInstance, 1, 0 ); // для подготовки storm для вызова функции directaccess ниже
	SFileEnableDirectAccess( 1 ); // тут включается чтение файлов из папок напрямую если их нет в .mpq 
	#endif
	if( SaveFolder ){
		for( int i = 0; i < 2; ++i ) create_directory( SaveFolders[i] );
		if( ColiseumSaveFolder ) create_directory( SaveFolders[2] );
		if( ClassicSaveFolder  ) create_directory( SaveFolders[3] );
	}
	InitPerks();
	if( XinputEnabled && ! LoadLibraryA("xinput9_1_0.dll") ){ XinputEnabled = false; }
	if( LimitCpuCore ) SetProcessAffinityMask(GetCurrentProcess(), 1);
}

// выход из th2 -------------------------------------------------------------
void ExitTH2()
{
	void SaveUserParameters(); SaveUserParameters();
	FreeCharacterTable();
	DetachMetaMenu();
}

HWND DlgHwnd;
POINT StormCursor;
extern bool MainMainMenu;

BOOL __stdcall GetCursorPos_( LPPOINT lpPoint )
{
	POINT p;
	BOOL result = GetCursorPos( &p );
	ScreenToClient( ghMainWnd, &p );
	// меню пока не умеет в изменяемое окно
	static POINT lastValid = {	p.x < 0 ? 0 : (p.x >= ScreenWidth ? ScreenWidth : p.x),
								p.y < 0 ? 0 : (p.x >= ScreenHeight ? ScreenHeight : p.y) };
	if( p.x < 0 || p.y < 0 || p.x >= ScreenWidth || p.y >= ScreenHeight ){
		p = lastValid;
	}else{
		lastValid = p;
	}
	*lpPoint = p;
	//p.x = ( WinWidth == ScreenWidth) ? p.x : (int) (double( ScreenWidth ) / WinWidth * p.x);
	//p.y = ( WinHeight == ScreenHeight) ? p.y : (int) (double( ScreenHeight ) / WinHeight * p.y);

	return result;
}

BOOL __stdcall ClientToScreen_( HWND hwnd, LPPOINT lppoint )
{
	POINT p{ 0, 0 };
	ClientToScreen( ghMainWnd, &p );
	static POINT prev = p;
	if( prev.x != p.x || prev.y != p.y ){
		//if( MainMainMenu ) MoveWindow( DlgHwnd, p.x, p.y, 640, 48, FALSE );
	}
	prev = p;
	BOOL result = ClientToScreen( hwnd, lppoint );
	lppoint->x -= p.x;
	lppoint->y -= p.y;
	return result;
}

// для фикса оконного режима в hell2.dll
BOOL __stdcall ScreenToClient_( HWND hwnd, LPPOINT lppoint )
{
	POINT p{ 0, 0 };
	ClientToScreen( ghMainWnd, &p );
	BOOL result = ScreenToClient( hwnd, lppoint );
	lppoint->x += p.x;
	lppoint->y += p.y;
	return result;
}

// для фикса оконного режима в hell2.dll
BOOL __stdcall GetWindowRect_( HWND hwnd, LPRECT lprect )
{
	POINT p{ 0, 0 };
	ClientToScreen( ghMainWnd, &p );
	static POINT prev = p;
	if( prev.x != p.x || prev.y != p.y ){
		//if( MainMainMenu ) MoveWindow( DlgHwnd, p.x, p.y, 640, 482, FALSE );
	}
	prev = p;
	BOOL result = GetWindowRect( hwnd, lprect );
	lprect->bottom -= p.y;
	lprect->top -= p.y;
	lprect->left -= p.x;
	lprect->right -= p.x;
	return result;
}

HWND _stdcall CreateWindowEx_storm( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	POINT p { 0/*(ScreenWidth - GUI_Width)/2*/, 0/*(ScreenHeight - GUI_Height)/2*/ };
	ClientToScreen( ghMainWnd, &p );
	//return DlgHwnd = CreateWindowExA( dwExStyle, lpClassName, lpWindowName, (dwExStyle == 65536) ? (WS_CHILD /*| WS_CLIPSIBLINGS*/ | (dwStyle & ~WS_POPUP)) : dwStyle, X /*p.x*/, Y /*p.y*/, nWidth, nHeight /*-2*/, global_hWnd /*hWndParent*/, hMenu, hInstance, lpParam );
	
	// сделать приращения здесь и в MoveWindow (там еще убрать изменение размеров)
	HWND dlgHwnd = CreateWindowExA( dwExStyle, lpClassName, lpWindowName, dwStyle, X + p.x, Y + p.y, nWidth, nHeight /*-2*/, hWndParent, hMenu, hInstance, lpParam );
	#ifdef META_CHILD
	if( MainMainMenu )
	#endif
		DlgHwnd = dlgHwnd;
	return dlgHwnd;
}

HDC hdcGet;
HDC __stdcall GetDC_( HWND hwnd )
{
	return GetDC( hwnd );
	ScreenSurface->GetDC( &hdcGet );
	return hdcGet;
}

int __stdcall ReleaseDC_( HWND hwnd, HDC hdc )
{
	return ReleaseDC( hwnd, hdc );
	return ScreenSurface->ReleaseDC( hdcGet );
}

HDC hdcBegin;
HDC __stdcall BeginPaint_( HWND hwnd, LPPAINTSTRUCT lppaint )
{
	return BeginPaint( hwnd, lppaint );
	ScreenSurface->GetDC( &hdcBegin );
	return hdcBegin;
}

int __stdcall EndPaint_( HWND hwnd, const LPPAINTSTRUCT lppaint )
{
	return EndPaint( hwnd, lppaint );
	return ScreenSurface->ReleaseDC( hdcBegin );
}

HWND _stdcall GetDesktopWindow_()
{
	return ghMainWnd;
}

BOOL __stdcall EnableWindow_( HWND hWnd, BOOL bEnable )
{
	// если отключить disabling, то он все равно становится неактивным
	// так окна главного меню это отдельный WS_POPUP сверху
	if( false && hWnd == ghMainWnd && bEnable == FALSE && MainMainMenu ){
		return 0;
	}else{
		return EnableWindow( hWnd, bEnable );
	}
}
