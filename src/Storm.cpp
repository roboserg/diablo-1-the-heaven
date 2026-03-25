#include "stdafx.h"

// storm.dll sources

//-------------------------------------------------------------------------
// Data declarations
extern "C" char byte_1501C000[];
extern "C" char aScdddddd[9];
extern "C" char aSblt_cpp[9];
extern "C" _UNKNOWN unk_1501C09C;
extern "C" _UNKNOWN unk_1501C0A0;
extern "C" char ZeroByte;
extern "C" char a4DA[6];
extern "C" char a4DS[6];
//extern "C" char Str2[];
extern "C" char aSbmp_cpp[9];
extern "C" __int16 word_1501C0CC;
extern "C" char* off_1501C0D0[3];
extern "C" char aScmd_cpp[9];
extern "C" wchar_t asc_1501C14C[2];
extern "C" char SubStr[];
extern "C" char Str[7];
extern "C" char asc_1501C160[3];
extern "C" char byte_1501C168[];
extern "C" char byte_1501C170;
extern "C" char a[];
extern "C" char a0ffh[5];
extern "C" char a0ffffh[7];
extern "C" char a0ffffffffh[11];
extern "C" char aIncCU[];
extern "C" char aRet[5];
extern "C" char aScode_cpp[10];
extern "C" char aWabc[];
extern "C" char aWsdtabc01[];
extern "C" char asc_1501C21C[];
extern "C" char a124[];
extern "C" char a1234[];
extern "C" int dword_1501C230;
extern "C" char byte_1501C234;
extern "C" _UNKNOWN unk_1501C238;
extern "C" _UNKNOWN unk_1501C240;
//extern "C" char String[];
extern "C" char aSdlg[];
extern "C" char aSdlg_cpp[9];
extern "C" char aSdlgstatic[];
extern "C" char ClassName[];
extern "C" char aSdlgdialog[];
extern "C" char aSdlg_font[];
extern "C" char aSdlg_modal[];
extern "C" char a_oy[];
extern "C" char aListbox_0[];
extern "C" char aCombobox_0[];
extern "C" char aSdlg_wndproc[];
extern "C" char aCombobox[9];
extern "C" char aScrollbar[10];
extern "C" char aListbox[8];
extern "C" char aEdit[5];
//extern "C" char aButton[];
extern "C" char aSdlg_endresult[];
extern "C" char aSdlg_enddialog[];
//extern "C" char pszFaceName[];
extern "C" char aAbcdefghijklmn[];
extern "C" char Text[];
extern "C" char ProcName[];
extern "C" char LibFileName[];
extern "C" char aSdraw_cpp[10];
extern "C" char aSaveScreenCapt[20];
extern "C" char aPcPaintbrush_p[22];
extern "C" char Name[];
extern "C" char Type[];
extern "C" char* off_1501C500[10];
extern "C" char aSevt_cpp[9];
extern "C" LPCSTR lpName;
extern "C" int dword_1501C62C[];
extern "C" char aBlizzard_storm[15];
extern "C" char aMicrosoftBaseC[43];
extern "C" char aCryptverifysig[];
extern "C" char aCryptsignhasha[];
extern "C" char aCryptreleaseco[];
extern "C" char aCryptimportkey[];
extern "C" char aCrypthashdata[];
extern "C" char aCryptdestroyke[];
extern "C" char aCryptdestroyha[];
extern "C" char aCryptcreatehas[];
extern "C" char aCryptacquireco[];
extern "C" char aSfile_cpp[10];
extern "C" char aAdvapi32_dll[];
extern "C" char aSignature[12];
extern "C" char aBlockTable[14];
extern "C" char asc_1501C768[];
extern "C" char asc_1501C76C[];
extern "C" char aSS[];
extern "C" wchar_t asc_1501C778[2];
extern "C" char aSgdi_cpp[9];
extern "C" __int16 word_1501C788;
extern "C" char aW_y[];
extern "C" int dword_1501C798[];
extern "C" char aSmsg_cpp[9];
extern "C" _UNKNOWN unk_1501C7E8;
extern "C" char aSnet_cpp[9];
extern "C" char aSnpquery[];
extern "C" char aS_sn2[];
extern "C" char aCaps_dat[];
extern "C" char SubBlock[];
extern "C" wchar_t asc_1501C854[2];
extern "C" char aSnpbind[];
extern "C" char aSBnupdate_exe[];
extern "C" char aExecute[];
extern "C" char aExtract[];
extern "C" char aDelete[];
extern "C" char aSS_0[];
extern "C" char aPrepatch_lst[];
extern "C" char aPatch[];
extern "C" char aPatches[8];
extern "C" char aSelectprovider[];
extern "C" char aS[];
extern "C" char aU_0[];
extern "C" char aNetworkProvide[];
extern "C" char aPreferredProvi[19];
extern "C" char aSoftwareBlizza[33];
extern "C" char aSoftwareBattle[21];
extern "C" char aU[];
extern "C" char aInternal[];
extern "C" char aDebuglevel[11];
extern "C" char aStrans_cpp[11];
extern "C" char aSvid_cpp[9];
extern "C" char a1W1SW2W12DW[19];
extern "C" char a1W1SW2ScDTw[18];
extern "C" char a1W2SW1W22DW[19];
extern "C" char a1W1W2W2SW1Tw2D[25];
extern "C" char aSmackw32_dll[];
extern "C" char aBenchmark[10];
extern "C" char aMode[5];
extern "C" char aVideoPlayer[];
extern "C" char aVersion[8];
extern "C" char PrefixString[];
extern "C" char S_aucmddef[13];
extern "C" char byte_1501E018[];
extern "C" char byte_1501E019[];
extern "C" char byte_1501E048[];
extern "C" char byte_1501E04C;
extern "C" char byte_1501E057[];
extern "C" char* off_1501E070[8];
extern "C" char byte_1501E090[];
extern "C" char S_auinst[11];
extern "C" char S_austream[13];
extern "C" __int64 CursorCoord;
extern "C" int dword_1501E0D8;
extern "C" int dword_1501E0DC;
extern "C" char S_aubitmaprec[16];
extern "C" char S_autimerrec[15];
extern "C" int dword_1501E110;
extern "C" int StormScreenWidth;
extern "C" int StormScreenHeight;
extern "C" char S_aumsgsrc[13];
extern "C" HANDLE hEvent;
extern "C" HANDLE hHandle;
extern "C" char S_auarchiverec[17];
extern "C" char S_aufilerec[14];
extern "C" char S_auaudiostrea[18];
extern "C" char S_aurequest[14];
extern "C" int dword_1501E1B0;
extern "C" int dword_1501E1B4;
extern "C" int dword_1501E1B8;
extern "C" int dword_1501E1BC;
extern "C" char S_ausgdiobj[14];
extern "C" int dword_1501E1D8[];
extern "C" int dword_1501E1DC;
extern "C" int dword_1501E1E0;
extern "C" int dword_1501E1E4;
extern "C" int dword_1501E1E8;
extern "C" volatile LONG dword_1501E218;
extern "C" volatile LONG dword_1501E228;
extern "C" volatile LONG dword_1501E238;
extern "C" volatile LONG Addend;
extern "C" int dword_1501E258;
extern "C" int dword_1501E268;
extern "C" int dword_1501E278;
extern "C" int dword_1501E288;
extern "C" int dword_1501E2D8;
extern "C" int dword_1501E2DC;
extern "C" int dword_1501E2E0;
extern "C" int dword_1501E2E4;
extern "C" DWORD dwMilliseconds;
extern "C" int dword_1501E2EC;
extern "C" char S_aumessage[14];
extern "C" char S_auconnrec[14];
extern "C" char S_auproviderin[19];
extern "C" char S_auuserevent[16];
extern "C" int dword_1501E358;
extern "C" int dword_1501E35C;
extern "C" int dword_1501E360;
extern "C" int dword_1501E364;
extern "C" int dword_1501E368;
extern "C" int dword_1501E36C;
extern "C" char S_autrans[12];
extern "C" char S_auvideorec[15];
extern "C" _UNKNOWN unk_1501E4D8;
extern "C" _UNKNOWN unk_1501E518;
extern "C" _UNKNOWN unk_1501E558;
extern "C" _UNKNOWN unk_1501E568;
extern "C" _UNKNOWN unk_1501E588;
extern "C" _UNKNOWN unk_1501E598;
extern "C" _UNKNOWN unk_1501E5A8;
extern "C" _UNKNOWN unk_1501E6A8;
extern "C" _UNKNOWN unk_1501E8A6;
extern "C" uint* dword_1501E970[3];
extern "C" _UNKNOWN unk_1501E97C;
extern "C" int dword_1501E988;
extern "C" int dword_1501E990;
extern "C" int dword_1501E994;
extern "C" int dword_1501E998;
extern "C" int dword_1501E99C;
extern "C" int dword_1501E9A0;
extern "C" int dword_1501E9A8;
extern "C" int dword_1501E9AC;
extern "C" int dword_1501E9B0;
extern "C" int dword_1501E9B4;
extern "C" LPSTR Source;
extern "C" int dword_1501E9BC;
extern "C" int dword_1501E9C0;
extern "C" int dword_1501E9C8;
extern "C" int dword_1501E9CC;
extern "C" int dword_1501E9D0;
extern "C" int dword_1501E9D4;
extern "C" int dword_1501E9D8;
extern "C" int dword_1501E9DC;
extern "C" int dword_1501E9E0;
extern "C" int dword_1501E9E4;
extern "C" int dword_1501E9E8;
extern "C" int dword_1501E9EC;
extern "C" int dword_1501E9F0;
extern "C" int dword_1501E9F4;
extern "C" int dword_1501E9F8;
extern "C" int dword_1501EA00;
extern "C" int dword_1501EA04;
extern "C" int dword_1501EA08;
extern "C" HCURSOR hCursor;
extern "C" int dword_1501EA10;
extern "C" HWND S_hWnd;
extern "C" HPALETTE hPal;
extern "C" HMODULE hModule;
extern "C" LPDIRECTDRAW StormDdraw_1501EA20;
extern "C" LPDIRECTDRAWPALETTE StormPalette_1501EA24;
extern "C" struct tagPALETTEENTRY S_pPalEntries;
//extern "C" _UNKNOWN unk_1501EA53;
//extern "C" _UNKNOWN unk_1501EE03;
extern "C" int dword_1501EE28;
extern "C" void (__fastcall *ConvertToTrueColorFunc)(uchar*);
extern "C" RECT StormRectForBlt_1501EE30;

extern "C" StormSurfaces StormSurface_1501EE40;
//extern "C" LPDIRECTDRAWSURFACE StormSurface_1501EE40;
//extern "C" LPDIRECTDRAWSURFACE StormSurface_BackBuffer_1501EE44;
//extern "C" LPDIRECTDRAWSURFACE StormOffscreenSurface_1501EE48;
//extern "C" LPDIRECTDRAWSURFACE StormSurface_unknown_1501EE4C;
extern "C" struct _RTL_CRITICAL_SECTION SCriticalSection;
extern "C" int dword_1501EE68;
extern "C" int dword_1501EE6C;
extern "C" char Buffer[256];
extern "C" int dword_1501EF70;
extern "C" int dword_1501EF74;
extern "C" int dword_1501EF78;
extern "C" int dword_1501EF7C;
extern "C" int dword_1501EF80;
extern "C" struct _RTL_CRITICAL_SECTION FileCriticalSection;
extern "C" int OpenedMpqArchives;
extern "C" int dword_1501EFA4;
extern "C" int dword_1501EFA8;
extern "C" int dword_1501EFAC;
extern "C" int dword_1501EFB0;
extern "C" int dword_1501EFB4;
extern "C" int dword_1501EFB8;
extern "C" int dword_1501EFBC;
char SFilename[260];
extern "C" int dword_1501F0C4;
extern "C" int dword_1501F0C8;
extern "C" int EnabledDirectAccess;
extern "C" int dword_1501F0D0;
extern "C" int MpqDecryptBuffer;                                // dword_1501F0D4
extern "C" int dword_1501F0D8;
extern "C" int dword_1501F0DC;
extern "C" int dword_1501F0E0;
extern "C" int dword_1501F0E4;
extern "C" int dword_1501F0E8;
extern "C" int dword_1501F0EC;
extern "C" int dword_1501F0F0[];
extern "C" int dword_1501F0F4;
extern "C" int dword_1501F0F8;
extern "C" struct _RTL_CRITICAL_SECTION stru_1501F100;
extern "C" int dword_15020900[];
extern "C" int dword_15020D00[];
extern "C" int dword_15021100;
extern "C" int dword_15021104;
extern "C" int dword_15021108;
extern "C" int dword_15021110;
extern "C" int dword_15021114;
extern "C" int dword_15021118;
extern "C" int dword_1502111C;
extern "C" int dword_15021120[];
extern "C" int dword_15021158;
extern "C" int dword_1502115C;
extern "C" int dword_15021160;
extern "C" int dword_15021164;
extern "C" struct _RTL_CRITICAL_SECTION NetCritical;
extern "C" int dword_15021180;
extern "C" int dword_15021184;
extern "C" HMODULE hLibModule;
extern "C" int dword_1502118C;
extern "C" int dword_15021190;
extern "C" int dword_15021194;
extern "C" LPVOID lpAddress;
extern "C" int dword_1502119C;
extern "C" int dword_150211A0;
extern "C" int dword_150211A4;
extern "C" int dword_150211A8;
extern "C" char byte_150211B0;
extern "C" int dword_15021230;
extern "C" char Dest;
extern "C" char byte_150212B8;
extern "C" int dword_15021338;
extern "C" int dword_1502133C;
extern "C" int dword_15021340;
extern "C" HANDLE dword_15021344;
extern "C" int dword_15021348;
extern "C" HANDLE dword_1502134C;
extern "C" int dword_15021350;
extern "C" int dword_15021354;
extern "C" int dword_15021358;
extern "C" struct _RTL_CRITICAL_SECTION stru_15021360;
extern "C" int dword_15021378;
extern "C" LPVOID dword_1502137C;
extern "C" int dword_15021380;
extern "C" int StormDllHandle;
extern "C" int dword_15021388;
extern "C" int dword_1502138C;
extern "C" int dword_15021390;
extern "C" int dword_15021394;
extern "C" int dword_15021398;
extern "C" int dword_1502139C;
extern "C" int dword_150213A0;
extern "C" int dword_15021BA8;
extern "C" int dword_15021BAC;
extern "C" int dword_15021BB0;
extern "C" int dword_15021BB4;
extern "C" HMODULE dword_15021BB8;
extern "C" int dword_15021BC0;
extern "C" _UNKNOWN unk_15024000;
extern "C" _UNKNOWN unk_15024058;
extern "C" _UNKNOWN unk_1502405C;
extern "C" _UNKNOWN unk_15024060;

//-------------------------------------------------------------------------
// Function declarations

#define __thiscall __stdcall // Test compile in C mode
#define __usercall __stdcall
#define __userpurge __stdcall

//int __stdcall loc_150013D4(int, int, int, int, int, int, int, int);
int __fastcall sub_15001540(char* a1, int a2);
int __fastcall sub_15001580(int* a1, int a2);
int __thiscall sub_150019B0(int This, LPPALETTEENTRY paletteEntry, int a3, unsigned int a4, int a5, int a6, int a7);
int __fastcall sub_15001A90(int a1, int a2, LPPALETTEENTRY paletteEntry, int a4, int a5, int a6, int a7, int a8);
void __fastcall sub_15001B70(int a1, int a2, int a3, int a4);
char __fastcall sub_15001C20(int a1, int a2, int a3);
int __fastcall sub_15001D60(int a1, int a2, int a3, DWORD a4, int a5, int a6, int a7);
int __fastcall sub_15001F20(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
char* __thiscall sub_150020C0(char* Str);
int __fastcall sub_15002230(HANDLE hFile, int a2, int a3, int a4, int a5);
int __fastcall sub_150023C0(HANDLE hFile, int a2, DWORD a3, int a4, int a5);
int __fastcall sub_15002510(int a1, unsigned __int8 **a2, int a3);
int __stdcall sub_15002580();
int __stdcall sub_150025A0();
int __fastcall sub_15002820(int(__stdcall* a1)(uint), DWORD a2, int a3);
unsigned int __fastcall sub_150029B0(int a1, int a2, unsigned int a3);
int __fastcall sub_15002A80(int a1, int* a2, int a3, int a4, int(__stdcall* a5)(uint));
int __fastcall sub_15002B20(int* a1, int* a2, int a3, int a4, int(__stdcall* a5)(uint));
int __fastcall sub_15002BF0(int a1, int a2, int a3);
int __fastcall sub_15002D80(int a1, int a2, int a3);
char* __fastcall sub_15002DF0(int a1, char* a2, int a3);
char* __fastcall sub_15002E80(int a1, char* a2, int a3);
int __fastcall sub_15002F10(int a1, int* a2, int a3);
int __fastcall sub_15002F90(char* Str, int a2, int a3);
int __fastcall sub_15003010(int lpFileName, int* edx0, int a2, int a3, int(__stdcall* a4)(uint));
int __fastcall sub_150030E0(int a1, int a2, int(__stdcall* a3)(uint));
int __stdcall sub_150034E0();
int __stdcall sub_15003500();
int __fastcall sub_15003510(int a1, int a2);
int __thiscall sub_15003790(void* This);
int __fastcall sub_150037A0(char* a1, int a2);
unsigned int __thiscall sub_15003930(char* This);
void* __stdcall sub_15003970();
int __fastcall sub_150039B0(char a1);
int __fastcall sub_15003A00(char* a1, int a2, int a3);
unsigned int __fastcall sub_15004AA0(int a1, char a2, int a3, int a4, int a5);
int __stdcall sub_15005540(int a1, int a2, int a3, int a4, int a5);
int __stdcall sub_150056C0();
int __stdcall sub_150056E0();
int __fastcall sub_15005800(HWND hWnd, int a2);
int __fastcall sub_15005A30(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, unsigned int a12, HWND hWnd, HRGN hrgnSrc2, __int16 a15, int a16, int a17);
int __fastcall sub_15005ED0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, unsigned int a12, int a13, struct _RGNDATA* hrgn, __int16 a15, int a16, int a17);
void __fastcall sub_150061B0(HWND a1, HRGN a2, HWND hWnd);
int __stdcall sub_15006370();
LRESULT __stdcall sub_15006460(LPARAM lParam, UINT Msg, WPARAM wParam, LPARAM a4);
int __stdcall sub_15006730(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
HWND __thiscall sub_150069F0(void* This);
int __thiscall sub_15006AA0(int This);
HWND __fastcall S_CreateDialogWindow(HINSTANCE a1, int a2, HWND hWndParent, LPVOID lpParam, LPARAM lParam);
int __fastcall sub_15007870(HGDIOBJ h, int a2, int a3);
LRESULT __stdcall sub_15007920(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
int __thiscall sub_15007BA0(void* hDlg);
int __fastcall sub_15007C20(HWND hWnd, int a2);
unsigned int __fastcall sub_15007CC0(int a1, int a2, wchar_t** a3, int a4, int a5, int a6, int a7, unsigned int a8);
unsigned int __fastcall sub_15007DE0(int a1, int a2, wchar_t** a3, int a4, int a5, int a6, int a7, unsigned int a8);
LRESULT __fastcall sub_15007F10(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
HRSRC __fastcall S_LoadDialogRes(HMODULE hModule, LPCSTR lpName);
HWND __fastcall sub_15008CC0(HWND a1, LONG a2, DWORD dwProcessId);
__int32 __stdcall sub_15008E50();
int __stdcall SelectPaletteToHwnd();
void __stdcall sub_15009590(int a1);
int __userpurge sub_150095A0/*eax*/(void* a1/*ecx*/, int a2/*edi*/, int a3);
LRESULT __stdcall sub_150095B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
int __stdcall sub_1500A570();
int __stdcall sub_1500A580();
void __stdcall sub_1500A590();
void __stdcall sub_1500A5B0();
char* __fastcall sub_1500A8E0(int a1);
int __stdcall sub_1500A9E0();
int __fastcall sub_1500AA00(__int16 a1);
int __stdcall sub_1500B0F0();
int __stdcall sub_1500B110();
int __stdcall sub_1500B130();
int __stdcall sub_1500B150();
int __stdcall SetupCriticalStormFile();
void __stdcall InitCriticalStormFile();
void __stdcall DelCriticalStormFile();
void __thiscall sub_1500BF40(DWORD This);
//int __userpurge StartAddress/*eax*/(int a1/*ebx*/, int a2/*ebp*/, int a3/*edi*/, int a4/*esi*/, int a5);
int __fastcall sub_1500C510(int a1, int a2, int a3);
int __fastcall sub_1500C5A0(int a1, int a2, DWORD a3, unsigned int a4);
DWORD __fastcall sub_1500C720(int a1, int a2, DWORD a3, DWORD nNumberOfBytesToRead);
int __userpurge sub_1500CC10/*eax*/(int a1/*edx*/, int a2/*ecx*/, int a3/*ebp*/, int a4);
int __fastcall sub_1500CC80(void* a1, unsigned int* a2, int a3);
int __fastcall sub_1500CCC0(void* a1, unsigned int* a2, int a3);
int __fastcall sub_1500CD00(int a1, int a2);
HANDLE __fastcall sub_1500CD60(int a1, unsigned int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
void __fastcall GetMpqFullPath(char* fileName, char* fullPath);
int __thiscall sub_1500E210(int This, int a2);
int __fastcall sub_1500E580(int a1, int a2);
int __stdcall sub_1500E5E0(char* fileName, int a2);
int __stdcall sub_1500EE70();
int __fastcall sub_1500F180(int a1, __int16 a2);
int __fastcall sub_1500F2F0(int a1);
void* __fastcall MemVirtualAllocHeader(char* a1, int a2, int a3, int a4, SIZE_T dwSize);
int __fastcall MemVirtualAlloc(uint a1, uint size);
void __stdcall MemVirtualFreeHeader();
void __stdcall InitCriticalStormArray();
int __thiscall sub_15010A90(void* This);
int __thiscall sub_15010B10(void* This);
int __fastcall sub_15010CE0(int a1, int a2, int a3, int a4);
int* __thiscall sub_15010D40(void* This);
int __stdcall SetupNetCriticalSection();
void __stdcall InitNetCriticalSection();
void __stdcall DeleteNetCriticalSection();
int __stdcall sub_15010E40();
int __stdcall sub_15010E60();
int __stdcall sub_15010E80();
int __stdcall sub_15010EA0();
void __thiscall sub_15010EB0(int This);
int __fastcall sub_15011270(int a1, int a2);
void __stdcall sub_15011340();
int __thiscall sub_15011370(void* This);
void __thiscall sub_15011450(void* This);
int __fastcall sub_150114E0(void* a1, void* a2, int a3);
int __thiscall sub_150115A0(void* This);
int __stdcall sub_15011660();
int __stdcall sub_150116F0();
int __fastcall sub_150118B0(int a1, char a2, char a3, void* a4, unsigned int a5);
int __fastcall sub_15011AB0(int a1, int a2, int a3);
int __thiscall sub_15011B00(int This);
char __fastcall sub_15011B60(int a1, int a2);
int __fastcall sub_15011BB0(int a1, int a2);
int __fastcall sub_15011C00(unsigned int a1, int a2, void* a3, unsigned int a4);
int __stdcall sub_150120B0();
void* __thiscall sub_15012530(char* Str);
int __fastcall sub_150125F0(int a1, int a2);
int __thiscall sub_15012AD0(void* This);
int __fastcall sub_15012CD0(int a1, unsigned __int8 a2, int a3);
char __stdcall sub_15012E30();
void* __fastcall sub_15013010(int a1, int a2, void* a3, unsigned int a4);
int __fastcall sub_150135D0(void* a1, void* a2, void* a3, void* a4, int a5, int a6, int a7, int a8);
char __fastcall sub_15013780(char* Source, void* a2, char* Dest, int a4);
int __fastcall sub_15013B00(int a1, int a2, int a3, int a4, int a5, int a6);
void __stdcall sub_15013D20(int a1);
int __stdcall sub_15013D90(int);
int __stdcall sub_15013E10(int);
int __stdcall sub_15013E60(int);
DWORD __stdcall sub_15013EC0(int* a1);
void* __stdcall sub_15013F30(int a1);
int __fastcall sub_15014180(int a1, void* a2, unsigned int a3);
int __stdcall sub_15014200(int a1);
int __fastcall sub_150146D0(int a1, int a2, int a3);
char* __stdcall sub_15014790(int a1);
void* __stdcall sub_15014890(int a1);
void __stdcall nullsub_1(int);
int __stdcall sub_15014980(int);
int __thiscall sub_15014A00(DWORD This);
int __stdcall sub_15014A90(int a1);
int __stdcall sub_15014B20();
int __stdcall sub_15014E50();
int __stdcall sub_15014F00();
__int16
__fastcall
sub_150157B0 (int a1, unsigned __int8 a2, __int16 a3);
int __stdcall sub_15015D10();
int __fastcall sub_15015DE0(unsigned int a1, int a2, int a3, DWORD a4);
void __stdcall sub_15015FB0();
HGDIOBJ __stdcall sub_150171F0(int hDlg, unsigned int a2, HDC hdc, HWND a4);
int __thiscall sub_15017750(void* This);
int __fastcall sub_150177C0(int (__stdcall *checkFunct)(uint, uint, uint, uint, uint, uint, uint, uint), int a2, int a3, int a4, void **a5, int a6);
int __fastcall LoadRegistryValue(char* Source, CHAR* a2, char a3, LPDWORD lpType, LPBYTE lpData, DWORD maxSize, LPDWORD lpcbData);
int __fastcall SaveRegistryValue(char* Source, CHAR* a2, char a3, DWORD dwType, BYTE* lpData, DWORD cbData);
void __stdcall DeleteCriticalStorm1();
void __stdcall InitCriticalStorm1();
void __fastcall sub_150183C0(unsigned int a1, unsigned int a2);
int __cdecl AtExit(int criticalSectionDelFunc);
void* __thiscall sub_150184A0(void* This, char a2);
void __thiscall sub_150184C0(void* This);
int __stdcall InitCriticalStorm2(int hInstDll, int fdvReason, int lpReserved);
int __stdcall StormDestroy();
int __stdcall GetStormDebugLevel();
int __stdcall GetStormDllHandle();
int __stdcall sub_150185B0();
int __fastcall sub_150188C0(int a1);
int __thiscall sub_150188E0(void* This);
int __fastcall sub_15018960(int a1, int a2, int a3, int a4, int a5, int a6, char a7, int a8, int a9, int a10);
int __fastcall sub_15019450(int a1, int a2);
int __stdcall sub_15019560();
int __thiscall sub_150196C0(void* This);
void __fastcall sub_15019EF0(int a1, PALETTEENTRY* a2);
int __thiscall sub_1501A760(void* This, long double a2);
int __thiscall sub_1501A880(int This);
int __fastcall sub_1501A8F0(int a1, int a2, int a3);
int __usercall sub_1501A9B0/*eax*/(int a1/*ecx*/, int a2/*ebp*/, int a3/*edi*/);
int __usercall sub_1501AB30/*eax*/(int a1/*ecx*/, int a2/*edi*/);
int __usercall sub_1501AC60/*eax*/(int a1/*ecx*/, int a2/*edi*/);
int __usercall sub_1501ADA0/*eax*/(int a1/*ecx*/, int a2/*edi*/);
int __usercall sub_1501AEC0/*eax*/(int a1/*ecx*/, int a2/*edi*/);
int __usercall sub_1501B058/*eax*/(unsigned int a1/*eax*/, int a2/*ecx*/);
int __userpurge sub_1501B0A0/*eax*/(int a1/*edx*/, int a2/*ecx*/, int a3/*ebp*/, int a4, int a5);
int __thiscall sub_1501B210(int This);
int __thiscall sub_1501B300(int This);
int __fastcall sub_1501B4B0(int a1, int a2);
int __fastcall sub_1501B530(int a1, unsigned int a2);
void __fastcall sub_1501B5C0(int a1, int a2, int a3, int a4);
uint __usercall sub_1501B600/*eax*/(int a1/*ecx*/, int a2/*ebp*/);
unsigned int __fastcall sub_1501B700(int a1, int a2, unsigned int a3);

//////////////////////////////////////////////////////////////////////////
// Data definition

void*(__thiscall * off_1501C940)(void*, char) = sub_150184A0;

//////////////////////////////////////////////////////////////////////////
// Function definition

//----- (150182F0) --------------------------------------------------------
BOOL __stdcall StormDllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	BOOL v3; // eax@3
	BOOL v4; // edi@3

	if( fdwReason == 1 )
		InitCriticalStorm1();
	v3 = InitCriticalStorm2( (int)hinstDLL, fdwReason, (int)lpReserved );
	v4 = v3;
	if( !fdwReason || fdwReason == 1 && !v3 )
		DeleteCriticalStorm1();
	return v4;
}


//----- (15001000) --------------------------------------------------------
int __stdcall SBltDestroy()
{
	void** v0;  // esi@2
	void** v1;  // edx@3
	int v2;     // esi@4
	if( dword_1501E988 ){
		v0 = (void**)dword_1501E970;
		do{
			SCodeDelete(*v0);
			v1 = v0;
			v0 += 3;
			*v1 = 0;
			v1[1] = 0;
			v1[2] = 0;
		}while( (int)v0 < (int)&dword_1501E988 );
		v2 = 0;
		do{
			if( *(uint*)(dword_1501E988 + v2 + 4) )
				SCodeDelete(*(void**)(dword_1501E988 + v2 + 4));
			v2 += 16;
		}while( v2 < 4096 );
		SMemFree((void*)dword_1501E988, "SBLT.CPP", 88, 0);
		dword_1501E988 = 0;
	}
	return 1;
}

// 15001000: inconsistent function type and number of purged bytes
// 1501E988: using guessed type int dword_1501E988;
//----- (15001080) --------------------------------------------------------
int __stdcall SBltGetSCode(unsigned int a1, char* Dest, size_t Count, int a4)
{
	unsigned int v4;  // edx@1
	int result;       // eax@2
	DWORD v6;         // ST18_4@2
	int v7;           // eax@8
	unsigned int v8;  // ecx@8
	int v9;           // esi@9
	int v10;          // ecx@11
	_UNKNOWN* v11;    // edi@11
	char v12;         // zf@13
	int v13;          // ecx@15
	int v14;          // ecx@15
	unsigned int v15; // ebx@15
	char* v16;        // edi@15
	void* v17;        // esi@15
	char v18;         // zf@17
	int v19;          // eax@21
	int v20;          // ecx@22
	char* v21;        // edi@22
	char v22;         // zf@24
	int v23;          // ecx@26
	char* v24;        // edi@26
	char v25;         // zf@28
	int v26;          // edx@31
	char* v27;        // ecx@31
	char v28;         // bl@31
	char v29;         // al@32
	int v30;          // eax@36
	int v31;          // edx@36
	char v32;         // cl@37
	int v33;          // edx@40
	char* v34;        // ecx@40
	int v35;          // edi@40
	int v36;          // esi@40
	char v37;         // al@40
	char* v38;        // esi@40
	int v39;          // esi@42
	int v40;          // esi@45
	char v41;         // al@45
	char v42;         // al@46
	int v43;          // esi@46
	char v44;         // [sp-2h] [bp-42h]@21
	char v45[14];     // [sp+0h] [bp-40h]@9
	char v46;         // [sp+Eh] [bp-32h]@45
	char v47;         // [sp+Fh] [bp-31h]@40
	char v48[16];     // [sp+10h] [bp-30h]@8
	char Source[4];   // [sp+20h] [bp-20h]@40
	char v50;         // [sp+25h] [bp-1Bh]@40
	__int16 v51;      // [sp+3Dh] [bp-3h]@40
	char v52;         // [sp+3Fh] [bp-1h]@40
	v4 = a1;
	if( Count != 0 ? (Dest != 0 ? -(a1 != 0) : 0) : 0 ){
		if( a4 ){
			if( a1 == 66 || a1 == 983041 || a1 == 16711778 )
				v4 = 15728673;
		}
		strcpy(v48, &aScdddddd[16 * ((v4 >> 2) & 7)] + (v4 & 3));
		v8 = v4 >> 6;
		v7 = 0;
		do{
			v9 = v8 & 3;
			v8 >>= 2;
			v45[v7++] = byte_1501C000[v9];
		}while( v7 < 5 );
		v45[v7] = 0;
		if( v4 & 0x20 ){
			v11 = &unk_1501C0A0;
			v10 = -1;
			do{
				if( !v10 )
					break;
				v12 = *(uchar*)v11 == 0;
				v11 = (char*)v11 + 1;
				--v10;
			}while( !v12 );
			v14 = ~v10;
			v17 = (char*)v11 - v14;
			v15 = v14;
			v16 = v45;
			v13 = -1;
			do{
				if( !v13 )
					break;
				v18 = *v16++ == 0;
				--v13;
			}while( !v18 );
			memcpy(v16 - 1, v17, v15);
		}
		if( strlen(v45) - 1 >= 2 ){
			do{
				v19 = strcmp(&v44 + strlen(v45) - 1, (char*)&unk_1501C09C);
				if( v19 )
					break;
				v21 = v45;
				v20 = -1;
				do{
					if( !v20 )
						break;
					v22 = *v21++ == (uchar)v19;
					--v20;
				}while( !v22 );
				v24 = v45;
				*(&v44 + ~v20 - 1) = v19;
				v23 = -1;
				do{
					if( !v23 )
						break;
					v25 = *v24++ == (uchar)v19;
					--v23;
				}while( !v25 );
			}while( (unsigned int)( ~v23 - 1) >= 2 );
		}
		v28 = v45[0];
		v26 = 0;
		v27 = v45;
		if( v45[0] ){
			v29 = v45[0];
			do{
				if( v29 != 126 )
					++v26;
				v29 = (v27++)[1];
			}while( v29 );
		}
		v31 = v26 + 1;
		v30 = 0;
		while( v30 < v31 ){
			v32 = v48[v30++];
			if( v32 == 65 )
				++v31;
		}
		strcpy(Source, "# D=");
		memset(&v50, 0, 0x18u);
		v51 = 0;
		v52 = 0;
		v37 = *(&v47 + v31);
		v33 = v31 - 1;
		v38 = &Source[strlen(Source) - 1];
		v34 = v45;
		*v38 = v37;
		v36 = (int)(v38 + 1);
		if( v28 ){
			do{
				if( *v34 == 126 ){
					*(uchar*)v36 = 94;
					v39 = v36 + 1;
					*(uchar*)v39 = 49;
					v36 = v39 + 1;
					++v34;
				}else{
					if( *(&v47 + v33) != 65 || v35 ){
						*(uchar*)v36 = *v34;
						v42 = *(&v47 + v33);
						v43 = v36 + 1;
						++v34;
						--v33;
						*(uchar*)v43 = v42;
						v36 = v43 + 1;
					}else{
						Source[2] = 65;
						*(uchar*)v36 = 32;
						v40 = v36 + 1;
						v41 = *(&v46 + v33);
						v33 -= 2;
						v35 = 1;
						*(uchar*)v40++ = 68;
						*(uchar*)v40++ = 61;
						*(uchar*)v40 = v41;
						v36 = v40 + 1;
					}
				}
			}while( *v34 );
		}
		*(uchar*)v36 = 0;
		strncpy(Dest, Source, Count);
		result = Count >= v36 - (uint)Source + 1;
	}else{
		v6 = 87;
		SErrSetLastError(v6);
		result = 0;
	}
	return result;
}

// 15001080: using guessed type char var_30[16];
// 15001080: using guessed type char var_40[14];
//----- (150012E0) --------------------------------------------------------
void __stdcall SBltROP3(int a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned int a8)
{
	int ebp0;                                                     // ebp@0
	char Dest;                                                    // [sp+Ch] [bp-9Ch]@13
	char v10;                                                     // [sp+Dh] [bp-9Bh]@13
	__int16 v11;                                                  // [sp+49h] [bp-5Fh]@13
	char v12;                                                     // [sp+4Bh] [bp-5Dh]@13
	DWORD v13;                                                    // [sp+4Ch] [bp-5Ch]@13
	int v14;                                                      // [sp+50h] [bp-58h]@8
	int v15;                                                      // [sp+54h] [bp-54h]@8
	unsigned int v16;                                             // [sp+58h] [bp-50h]@8
	int (__stdcall *v17)(int, int, int, int, int, int, int, int); // [sp+5Ch] [bp-4Ch]@8
	int v18;                                                      // [sp+60h] [bp-48h]@8
	unsigned int v19;                                             // [sp+64h] [bp-44h]@12
	int v20;                                                      // [sp+68h] [bp-40h]@16
	int v21;                                                      // [sp+6Ch] [bp-3Ch]@16
	int v22;                                                      // [sp+70h] [bp-38h]@16
	int v23;                                                      // [sp+74h] [bp-34h]@16
	int v24;                                                      // [sp+78h] [bp-30h]@16
	int v25;                                                      // [sp+7Ch] [bp-2Ch]@16
	int v26;                                                      // [sp+80h] [bp-28h]@16
	int v27;                                                      // [sp+84h] [bp-24h]@16
	int v28;                                                      // [sp+94h] [bp-14h]@16
	if( !dword_1501E988 ){
		dword_1501E988 = SMemAlloc(4096, "SBLT.CPP", 220, 8);
		if( !sub_15001540("4 D=S", (int)dword_1501E970) || !sub_15001540("4 D=A", (int)&unk_1501E97C) ){
			SCodeDestroy();
			return;
		}
	}
	if( a8 == 13369376 || a8 == 15728673 ){
		if( !(((uchar)a3 | (uchar)a1) & 3) ){
			v14 = a5 - a3;
			v18 = a6 - a3;
			v17 = 0; //loc_150013D4;
			v15 = (int)&dword_1501E970[3 * (a8 == 15728673)];
			v16 = dword_1501E970[3 * (a8 == 15728673) + 1][a3 >> 2];
			*dword_1501E970[3 * (a8 == 15728673) + 2] = 0;
		// 			(char *)loc_150013D4 - (char *)(int (__stdcall *)(int, int, int, int, int, int, int, int))(dword_1501E970[3 * (a8 == 15728673) + 2] + 1);
		// 			JUMPOUT(__CS__, v16);
		}
	}
	if( a8 != 13369376 ){
		if( a8 != 15728673 )
			sub_15001580((int*)&a8, (int)&a7);
	}
	v19 = (a8 >> 16) & 0xFF;
	if( *(uint*)(dword_1501E988 + 16 * ((a8 >> 16) & 0xFF)) )
		goto LABEL_16;
	v13 = GetTickCount();
	Dest = ZeroByte;
	memset(&v10, 0, 0x3Cu);
	v11 = 0;
	v12 = 0;
	SBltGetSCode(a8, &Dest, 0x40u, 0);
	SCodeCompile((int)&Dest, &Dest, 0, 0xA0u, 0x40000u, dword_1501E988 + 16 * v19 + 4);
	if( *(uint*)(dword_1501E988 + 16*v19 + 4) ){
		*(uint*)(dword_1501E988 + 16 * v19) = 1;
		*(uint*)(dword_1501E988 + 16 * v19 + 8) = v13;
		LABEL_16:
		v20 = 64;
		v21 = 0;
		v22 = a3;
		v23 = a4;
		v26 = a1;
		v27 = a2;
		v24 = a5 - a3;
		v25 = a6 - a3;
		v28 = a7;
		SCodeExecute(ebp0, *(uint*)(dword_1501E988 + 16 * v19 + 4), (int)&v20);
	}
}

// 150013D4: using guessed type int __stdcall loc_150013D4(int, int, int, int, int, int, int, int);
// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E988: using guessed type int dword_1501E988;
//----- (15001540) --------------------------------------------------------
int __fastcall sub_15001540(char* a1, int a2)
{
	int result; // eax@1
	int v3;     // esi@1
	v3 = a2;
	result = SCodeCompile(0, a1, 0, 0xA0u, 0, a2);
	if( result )
		result = sub_15005540(*(uint*)v3, v3 + 4, 0, v3 + 8, 0) != 0;
	return result;
}

//----- (15001580) --------------------------------------------------------
int __fastcall sub_15001580(int* a1, int a2)
{
	int result; // eax@1
	result = *a1;
	if( *a1 == 66 ){
		*a1 = 15728673;
		*(uint*)a2 = 0;
	}else{
		if( result == 983041 ){
			*a1 = 15728673;
			result = *(uint*)a2 == 0;
			*(uint*)a2 = result;
		}else{
			if( result == 16711778 ){
				*a1 = 15728673;
				*(uint*)a2 = -1;
			}
		}
	}
	return result;
}

//----- (150015D0) --------------------------------------------------------
void __stdcall SBltROP3Clipped(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, unsigned int a10)
{
	int v10; // edx@1
	int v11; // ecx@1
	int v12; // ebp@1
	int v13; // edi@1
	int v14; // esi@1
	int v15; // ebx@2
	int v16; // eax@4
	int v18; // eax@6
	int v20; // esi@9
	int v21; // ebp@9
	int v22; // eax@11
	int v24; // eax@13
	int v26; // esi@22
	int v27; // [sp+10h] [bp-Ch]@1
	int v28; // [sp+14h] [bp-8h]@1
	int v29; // [sp+18h] [bp-4h]@1
	v13 = 0;
	v10 = 0;
	v11 = 0;
	v14 = 2147483647;
	v29 = 0;
	v28 = 0;
	v27 = 0;
	v12 = 2147483647;
	if( a2 ){
		v15 = *(uint*)(a2 + 4);
		LOBYTE_IDA(v10) = *(uint*)a2 < 0;
		v29 = *(uint*)a2&(v10 - 1);
		v13 = v15&((v15 < 0) - 1);
		v10 = *(uint*)(a2 + 8) - *(uint*)a2 + 1;
		v11 = *(uint*)(a2 + 12) - v15 + 1;
	}
	if( a3 ){
		v16 = *(uint*)a3 - v29;
		if( v10 >= (v16&((v16 < 0) - 1)) )
			v10 = v16&((v16 < 0) - 1);
		v18 = *(uint*)(a3 + 4) - v13;
		if( v11 >= (v18&((v18 < 0) - 1)) )
			v11 = v18&((v18 < 0) - 1);
	}
	if( a6 ){
		v20 = *(uint*)(a6 + 4);
		v28 = *(uint*)a6&((*(uint*)a6 < 0) - 1);
		v27 = v20&((v20 < 0) - 1);
		v21 = *(uint*)(a6 + 12) - v20;
		v14 = *(uint*)(a6 + 8) - *(uint*)a6 + 1;
		v12 = v21 + 1;
	}
	if( a7 ){
		v22 = *(uint*)a7 - v28;
		if( v14 >= (v22&((v22 < 0) - 1)) )
			v14 = v22&((v22 < 0) - 1);
		v24 = *(uint*)(a7 + 4) - v27;
		if( v12 >= (v24&((v24 < 0) - 1)) )
			v12 = v24&((v24 < 0) - 1);
	}
	if( v10 >= v14 )
		v10 = v14;
	if( v11 >= v12 )
		v11 = v12;
	if( v10 >= 1 ){
		if( v11 >= 1 ){
			if( a5 )
				v26 = a5 + v28 + a8 * v27;
			else
				v26 = 0;
			SBltROP3(a1 + v29 + a4 * v13, v26, v10, v11, a4, a8, a9, a10);
		}
	}
}

//----- (15001780) --------------------------------------------------------
int __stdcall SBltROP3Tiled(void *lpDstBuffer, RECT *lpDstRect, POINT *lpDstPt, int a4, void *lpSrcBuffer, RECT *lpSrcRect, POINT *lpSrcPt, int a8, int a9, DWORD rop)
{
	int v10;    // eax@1
	int v11;    // edx@1
	int v12;    // ebx@1
	int v13;    // ebp@1
	int v14;    // esi@1
	int v15;    // ecx@4
	int v16;    // esi@6
	int v17;    // eax@8
	int v18;    // ecx@8
	int v19;    // edi@8
	int v20;    // esi@8
	int v21;    // ebx@9
	int v22;    // ecx@11
	int v23;    // ebp@11
	int v24;    // eax@13
	int result; // eax@17
	int v26;    // [sp+10h] [bp-14h]@6
	int v27;    // [sp+14h] [bp-10h]@1
	int v28;    // [sp+18h] [bp-Ch]@1
	int v29;    // [sp+1Ch] [bp-8h]@1
	int v30;    // [sp+20h] [bp-4h]@8
	v13 = lpSrcBuffer;
	v11 = *(uint*)(lpSrcBuffer + 12) - *(uint*)(lpSrcBuffer + 4) + 1;
	v28 = *(uint*)(lpSrcBuffer + 8) - *(uint*)lpSrcBuffer + 1;
	v10 = lpDstRect;
	v27 = *(uint*)(lpSrcBuffer + 12) - *(uint*)(lpSrcBuffer + 4) + 1;
	v12 = *(uint*)(lpDstRect + 4);
	v14 = *(uint*)(lpDstRect + 12);
	v29 = *(uint*)(lpDstRect + 4);
	if( v12 > v14 ){
		result = 1;
	}else{
		while( 1 ){
			v15 = *(uint*)(v13 + 4);
			if( v12 == *(uint*)(v10 + 4) )
				v15 += (v11 + a8) % v27;
			v16 = v14 - v12 + 1;
			v26 = *(uint*)(v13 + 12) - v15 + 1;
			if( v26 >= v16 )
				v26 = v16;
			v19 = lpDstBuffer + lpDstPt * v12;
			v17 = lpDstRect;
			v30 = a4 + lpSrcRect * v15;
			v20 = *(uint*)lpDstRect;
			v18 = *(uint*)(lpDstRect + 8);
			if( *(uint*)lpDstRect <= v18 )
				break;
			LABEL_16:
			v14 = *(uint*)(v17 + 12);
			v12 += v26;
			v29 = v12;
			if( v12 > v14 )
				return 1;
			v10 = lpDstRect;
			v11 = v27;
		}
		while( 1 ){
			v21 = *(uint*)v13;
			if( v20 == *(uint*)lpDstRect )
				v21 += (lpSrcPt + v28) % v28;
			v22 = v18 - v20 + 1;
			v23 = *(uint*)(v13 + 8) - v21 + 1;
			if( v23 >= v22 )
				v23 = v22;
			SBltROP3(v20 + v19, v30 + v21, v23, v26, lpDstPt, lpSrcRect, a9, rop);
			if( !v24 )
				break;
			v20 += v23;
			v13 = lpSrcBuffer;
			v18 = *(uint*)(lpDstRect + 8);
			if( v20 > v18 ){
				v12 = v29;
				v17 = lpDstRect;
				goto LABEL_16;
			}
		}
		result = 0;
	}
	return result;
}

//----- (150018E0) --------------------------------------------------------
int __stdcall SBmpDecodeImage(int a1, int a2, int a3, LPPALETTEENTRY paletteEntry, int a5, unsigned int a6, int a7, int a8, int a9)
{
	int v9;     // ebp@9
	int result; // eax@12
	if( a7 )
		*(uint *)a7 = 0;
	if( a8 )
		*(uint *)a8 = 0;
	if( a9 )
		*(uint *)a9 = 0;
	if( a6 || !a5 )
		v9 = -1;
	else
		v9 = 0;
	if( (a2 != 0 ? (a1 != 0 ? -(a3 != 0) : 0) : 0) & v9 ){
		if( a1 == 1 ){
			result = sub_150019B0(a2, paletteEntry, a5, a6, a7, a8, a9);
		}else{
			if( a1 == 2 )
				result = sub_15001A90(a2, a3, paletteEntry, a5, a6, a7, a8, a9);
			else
				result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (150019B0) --------------------------------------------------------
int __thiscall sub_150019B0( int This, LPPALETTEENTRY paletteEntry, int a3, unsigned int a4, int a5, int a6, int a7 )
{
	LPPALETTEENTRY tmpPalEntry;           // edi@8
	int v8;           // eax@11
	LPPALETTEENTRY v9;           // edx@11
	int v10;          // ebp@11
	char v11;         // bl@12
	unsigned int v12; // eax@14
	int v13;          // edx@16
	int v14;          // esi@16
	int v15;          // edx@16
	if( a5 )
		*(uint *)a5 = *(uint *)(This + 18);
	if( a6 )
		*(uint *)a6 = *(uint *)(This + 22);
	if( a7 )
		*(uint *)a7 = *(ushort *)(This + 28);
	if( *(ushort*)(This + 28) <= 8u )
		tmpPalEntry = paletteEntry;
	else
		tmpPalEntry = 0;
	if( tmpPalEntry ){
		v9 = tmpPalEntry + 3;
		v8 = This + 55;
		v10 = 256;
		do{
			v11 = *(uchar*)(v8 + 1);
			v8 += 4;
			*(uchar*)(v9 - 3) = v11;
			*(uchar*)(tmpPalEntry - (This + 54) + v8 - 4) = *(uchar*)(v8 - 4);
			*(uchar*)(v9 - 1) = *(uchar*)(v8 - 5);
			*(uchar*)v9 = 0;
			v9 += 4;
			--v10;
		}while( v10 );
	}
	if( a3 ){
		v12 = *(uint*)(This + 34);
		if( a4 < v12 )
			v12 = a4;
		v15 = *(ushort*)(This + 28);
		v14 = v15 * *(uint*)(This + 18) >> 3;
		v13 = (v15 * *(uint*)(This + 18) >> 3) & 3;
		if( v13 )
			v14 += 4 - v13;
		sub_15001B70(a3, *(uint*)(This + 10) - (tmpPalEntry != 0 ? 0x400 : 0) - 54 + This, v14, v12 / v14);
	}
	return 1;
}

//----- (15001A90) --------------------------------------------------------
int __fastcall sub_15001A90( int a1, int a2, LPPALETTEENTRY paletteEntry, int a4, int a5, int a6, int a7, int a8 )
{
	int v8;   // eax@1
	int v9;   // edx@1
	int v10;  // edi@1
	int v11;  // esi@1
	int v12;  // ebp@9
	LPPALETTEENTRY v13;  // eax@14
	int v14;  // ecx@14
	int v15;  // esi@14
	char v16; // dl@15
	int v18;  // [sp+10h] [bp-8h]@1
	int v19;  // [sp+14h] [bp-4h]@1
	v11 = a1;
	v19 = a2;
	v9 = *(uchar*)(a1 + 3);
	v10 = *(ushort*)(a1 + 8) - *(ushort*)(a1 + 4) + 1;
	v8 = *(ushort*)(a1 + 10) - *(ushort*)(a1 + 6) + 1;
	v18 = *(uchar*)(a1 + 3);
	if( a6 )
		*(uint *)a6 = v10;
	if( a7 )
		*(uint *)a7 = v8;
	if( a8 )
		*(uint *)a8 = v9;
	if( a4 ){
		a6 = a1 + 128;
		a7 = a4;
		if( v8 > 0 ){
			v12 = v8;
			do{
				sub_15001C20((int)&a7, (int)&a6, v10);
				--v12;
			}while( v12 );
			v9 = v18;
		}
	}
	if( paletteEntry ){
		if( v9 == 8 ){
			v13 = paletteEntry + 2;
			v14 = v19 + v11 - 766;
			v15 = 256;
			do{
				v16 = *(uchar*)(v14 - 2);
				v14 += 3;
				*(uchar*)(v13 - 2) = v16;
				*(uchar*)(v13 - 1) = *(uchar*)(v14 - 4);
				*(uchar*)v13 = *(uchar*)(v14 - 3);
				*(uchar*)(v13 + 1) = 0;
				v13 += 4;
				--v15;
			}while( v15 );
		}
	}
	return 1;
}

//----- (15001B70) --------------------------------------------------------
void __fastcall sub_15001B70(int a1, int a2, int a3, int a4)
{
	int v4;  // esi@1
	int v5;  // eax@3
	int v6;  // ecx@4
	int v7;  // ebx@4
	int v8;  // esi@5
	int v9;  // edx@7
	int v10; // edi@8
	int v11; // ebp@12
	int v12; // edi@14
	v4 = a2;
	if( a4 != 0 ? (a3 != 0 ? (a2 != 0 ? -(a1 != 0) : 0) : 0) : 0 ){
		v5 = (a4 - 1) * a3 + a1;
		if( a3 & 3 ){
			if( a4 ){
				v11 = a4;
				do{
					if( a3 ){
						v12 = a3;
						do{
							*(uchar*)v5++ = *(uchar*)v4++;
							--v12;
						}while( v12 );
					}
					v5 -= a3 <= 1;
					--v11;
				}while( v11 );
			}
		}else{
			v7 = a3 >> 2;
			v6 = a2;
			if( a4 ){
				v8 = a4;
				do{
					if( v7 ){
						v9 = a3 >> 2;
						do{
							v10 = *(uint*)v6;
							v6 += 4;
							*(uint*)v5 = v10;
							v5 += 4;
							--v9;
						}while( v9 );
					}
					--v8;
					v5 += -8 * v7;
				}while( v8 );
			}
		}
	}else{
		SErrSetLastError(0x57u);
	}
}

//----- (15001C20) --------------------------------------------------------
char __fastcall sub_15001C20(int a1, int a2, int a3)
{
	int v3;  // edi@1
	int v4;  // eax@2
	int v5;  // esi@2
	char v6; // bl@3
	v3 = a3;
	while( 1 ){
		LOBYTE_IDA(v4) = **(uchar**)a2;
		v5 = *(uint*)a2 + 1;
		*(uint*)a2 = v5;
		if( (uchar)v4 < 0xC0u ){
			** (uchar**)a1 = v4;
			--v3;
			++*(uint*)a1;
			goto LABEL_8;
		}
		v6 = *(uchar*)v5;
		v4 &= 0x3Fu;
		*(uint*)a2 = v5 + 1;
		if( v4 )
			break;
		LABEL_8:
		if( v3 <= 0 )
			return v4;
	}
	while( v3 ){
		** (uchar**)a1 = v6;
		--v4;
		--v3;
		++*(uint*)a1;
		if( !v4 )
			goto LABEL_8;
	}
	return v4;
}

//----- (15001C70) --------------------------------------------------------
int __stdcall SBmpLoadImage(char* Str, int a2, int a3, uint a4, int a5, int a6, int a7)
{
	int v7;     // ebp@5
	int v8;     // ecx@9
	int result; // eax@12
	char* v10;  // eax@15
	int v11;    // eax@17
	int v12;    // esi@19
	if( a5 )
		*(uint *)a5 = 0;
	if( a6 )
		*(uint *)a6 = 0;
	v7 = a7;
	if( a7 )
		*(uint *)a7 = 0;
	if( a4 || !a3 )
		v8 = -1;
	else
		v8 = 0;
	if( (*Str != 0 ? -(Str != 0) : 0) & v8 ){
		result = SFileOpenFile(Str, (HANDLE*)&a7);
		if( result ){
			v10 = sub_150020C0(Str);
			if( !v10 || stricmp(v10, ".pcx") )
				v11 = sub_15001D60(a7, a2, a3, a4, a5, a6, v7);
			else
				v11 = sub_15001F20(a7, a2, a3, a4, a5, a6, v7);
			v12 = v11;
			SFileCloseFile((HANDLE)a7);
			result = v12;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15001D60) --------------------------------------------------------
int __fastcall sub_15001D60(int a1, int a2, int a3, DWORD a4, int a5, int a6, int a7)
{
	int result;                 // eax@1
	int v8;                     // ebp@1
	int v9;                     // esi@1
	unsigned __int16 v10;       // cx@9
	int v11;                    // eax@16
	char* v12;                  // ecx@16
	int v13;                    // ebp@16
	DWORD v14;                  // esi@20
	int v15;                    // ecx@22
	int v16;                    // edi@22
	int v17;                    // [sp+10h] [bp-43Ch]@1
	char v18;                   // [sp+14h] [bp-438h]@3
	int v19;                    // [sp+18h] [bp-434h]@6
	int v20;                    // [sp+1Ch] [bp-430h]@8
	int v21;                    // [sp+22h] [bp-42Ah]@9
	DWORD nNumberOfBytesToRead; // [sp+28h] [bp-424h]@20
	char NumberOfBytesRead;     // [sp+3Ch] [bp-410h]@1
	int v24;                    // [sp+46h] [bp-406h]@20
	char v25[1024];             // [sp+4Ch] [bp-400h]@14
	v8 = a1;
	v9 = a2;
	v17 = a1;
	result = SFileReadFile((HANDLE)a1, &NumberOfBytesRead, 0xEu, 0, 0);
	if( result ){
		result = SFileReadFile((HANDLE)v8, &v18, 0x28u, 0, 0);
		if( result ){
			if( a5 )
				*(uint *)a5 = v19;
			if( a6 )
				*(uint *)a6 = v20;
			v10 = v21;
			if( a7 )
				*(uint *)a7 = (unsigned __int16)v21;
			if( v10 > 8u )
				v9 = 0;
			if( v9 ){
				result = SFileReadFile((HANDLE)v8, v25, 0x400u, 0, 0);
				if( !result )
					return result;
				v12 = v25;
				v11 = v9 + 2;
				v13 = 256;
				do{
					v12 += 4;
					*(uchar*)(v11 - 2) = *(&v25[v11] - v9);
					*(uchar*)(v11 - 1) = *(v12 - 3);
					*(uchar*)v11 = *(v12 - 4);
					*(uchar*)(v11 + 1) = 0;
					v11 += 4;
					--v13;
				}while( v13 );
				v8 = v17;
			}
			if( !a3 )
				goto LABEL_29;
			SFileSetFilePointer((HANDLE)v8, v24 - (v9 != 0 ? 0x400 : 0) - 54, 0, 1u);
			v14 = nNumberOfBytesToRead;
			if( a4 < nNumberOfBytesToRead )
				v14 = a4;
			v17 = 0;
			v16 = SMemAlloc(v14, "SBMP.CPP", 127, 0);
			SFileReadFile((HANDLE)v8, (LPVOID)v16, v14, (uint*)&v17, 0);
			v15 = v19 * (unsigned __int16)v21 >> 3;
			if( (v19 * (unsigned __int16)v21 >> 3) & 3 )
				v15 += 4 - ((v19 * (unsigned __int16)v21 >> 3) & 3);
			sub_15001B70(a3, v16, v15, v14 / v15);
			SMemFree((void*)v16, "SBMP.CPP", 133, 0);
			if( v17 < v14 )
				result = 0;
			else
				LABEL_29:
				result = 1;
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15001D60: using guessed type char var_400[1024];
//----- (15001F20) --------------------------------------------------------
int __fastcall sub_15001F20(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int result;             // eax@1
	int v8;                 // ebx@1
	int v9;                 // ebp@3
	int v10;                // edi@3
	int v11;                // esi@3
	DWORD v12;              // esi@10
	unsigned int v13;       // esi@10
	int v14;                // eax@16
	char* v15;              // ecx@16
	int v16;                // esi@16
	char v17;               // dl@17
	int v18;                // [sp+4h] [bp-394h]@3
	void* v19;              // [sp+8h] [bp-390h]@10
	int v20;                // [sp+Ch] [bp-38Ch]@1
	int v21;                // [sp+10h] [bp-388h]@10
	void* v22;              // [sp+14h] [bp-384h]@10
	char NumberOfBytesRead; // [sp+18h] [bp-380h]@1
	unsigned __int8 v24;    // [sp+1Bh] [bp-37Dh]@3
	int v25;                // [sp+1Ch] [bp-37Ch]@3
	unsigned __int16 v26;   // [sp+20h] [bp-378h]@3
	int v27;                // [sp+22h] [bp-376h]@3
	char v28;               // [sp+98h] [bp-300h]@16
	char v29;               // [sp+99h] [bp-2FFh]@16
	v8 = a1;
	v20 = a2;
	result = SFileReadFile((HANDLE)a1, &NumberOfBytesRead, 0x80u, 0, 0);
	if( result ){
		v9 = v26 - (unsigned __int16)v25 + 1;
		v10 = (unsigned __int16)v27 - HIWORD(v25) + 1;
		v11 = v24;
		v18 = v24;
		if( a5 )
			*(uint *)a5 = v9;
		if( a6 )
			*(uint *)a6 = v10;
		if( a7 )
			*(uint *)a7 = v11;
		if( a3 ){
			v12 = SFileGetFileSize((HANDLE)v8, 0);
			v13 = v12 - SFileSetFilePointer((HANDLE)v8, 0, 0, 1u);
			v19 = (void*)SMemAlloc(v13, "SBMP.CPP", 221, 0);
			SFileReadFile((HANDLE)v8, v19, v13, 0, 0);
			v22 = v19;
			v21 = a3;
			if( v10 > 0 ){
				do{
					sub_15001C20((int)&v21, (int)&v22, v9);
					--v10;
				}while( v10 );
			}
			SMemFree(v19, "SBMP.CPP", 227, 0);
			v11 = v18;
		}else{
			SFileSetFilePointer((HANDLE)v8, 0, 0, 2u);
		}
		if( v20 ){
			if( v11 == 8 ){
				SFileSetFilePointer((HANDLE)v8, -768, 0, 1u);
				SFileReadFile((HANDLE)v8, &v28, 0x300u, 0, 0);
				v15 = &v29;
				v14 = v20 + 2;
				v16 = 256;
				do{
					v17 = *(v15 - 1);
					v15 += 3;
					*(uchar*)(v14 - 2) = v17;
					*(uchar*)(v14 - 1) = *(v15 - 3);
					*(uchar*)v14 = *(v15 - 2);
					*(uchar*)(v14 + 1) = 0;
					v14 += 4;
					--v16;
				}while( v16 );
			}
		}
		result = 1;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150020C0) --------------------------------------------------------
char* __thiscall sub_150020C0(char* Str)
{
	char* i; // esi@1
	char* j; // edi@3
	for( i = (char*)Str; strchr(i, 92); i = strchr(i, 92) + 1 )
	;
	for( j = i + 1; strchr(i + 1, 46); j = i + 1 )
		i = strchr(j, 46);
	return i;
}

//----- (15002130) --------------------------------------------------------
int __stdcall SBmpSaveImage(char* Str, int a2, uint a3, int a4, int a5, int a6)
{
	int result; // eax@2
	HANDLE v7;  // esi@5
	char* v8;   // eax@7
	int v9;     // edi@9
	int v10;    // edi@10
	if( *Str != 0 ? (a2 != 0 ? (Str != 0 ? ((a5 <= 0) - 1) & (a3 != 0 ? (a4 <= 0) - 1 : 0) : 0) : 0) : 0 ){
		if( a6 == 8 ){
			v7 = CreateFileA(Str, 0x40000000u, 0, 0, 2u, 0x80u, 0);
			if( v7 == INVALID_HANDLE_VALUE ){
				result = 0;
			}else{
				v8 = sub_150020C0(Str);
				if( !v8 || stricmp(v8, ".pcx") ){
					v10 = sub_15002230(v7, a2, a3, a4, a5);
					CloseHandle(v7);
					result = v10;
				}else{
					v9 = sub_150023C0(v7, a2, a3, a4, a5);
					CloseHandle(v7);
					result = v9;
				}
			}
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15002230) --------------------------------------------------------
int __fastcall sub_15002230(HANDLE hFile, int a2, int a3, int a4, int a5)
{
	int v5;                     // eax@1
	int v6;                     // edx@1
	char* v7;                   // ecx@1
	int v8;                     // ebx@1
	DWORD v9;                   // esi@1
	HANDLE v10;                 // ebp@1
	HANDLE v12;                 // ebx@3
	int v13;                    // edi@3
	DWORD NumberOfBytesWritten; // [sp+10h] [bp-440h]@1
	int Buffer;                 // [sp+14h] [bp-43Ch]@1
	int v16;                    // [sp+18h] [bp-438h]@1
	int v17;                    // [sp+1Ch] [bp-434h]@1
	HANDLE v18;                 // [sp+24h] [bp-42Ch]@1
	int v19;                    // [sp+28h] [bp-428h]@1
	int v20;                    // [sp+2Ch] [bp-424h]@1
	int v21;                    // [sp+30h] [bp-420h]@1
	__int16 v22;                // [sp+34h] [bp-41Ch]@1
	__int16 v23;                // [sp+36h] [bp-41Ah]@1
	int v24;                    // [sp+38h] [bp-418h]@1
	int v25;                    // [sp+3Ch] [bp-414h]@1
	int v26;                    // [sp+48h] [bp-408h]@1
	int v27;                    // [sp+4Ch] [bp-404h]@1
	char v28;                   // [sp+50h] [bp-400h]@1
	char v29[1023];             // [sp+51h] [bp-3FFh]@2 // под звук чтоли ? а их уже 1084
	v9 = a4 * a5;
	v10 = hFile;
	v16 = 0;
	v17 = 0;
	LOWORD_IDA(Buffer) = word_1501C0CC;
	v8 = a2;
	v18 = hFile;
	*(int*)((char*)&v17 + 2) = 1078;
	*(int*)((char*)&Buffer + 2) = a4 * a5 + 1078;
	WriteFile(hFile, &Buffer, 0xEu, &NumberOfBytesWritten, 0);
	memset(&v19, 0, 0x28u);
	v20 = a4;
	v21 = a5;
	v19 = 40;
	v22 = 1;
	v23 = 8;
	v24 = 0;
	v25 = a4 * a5;
	v26 = 256;
	v27 = 256;
	WriteFile(v10, &v19, 0x28u, &NumberOfBytesWritten, 0);
	v7 = &v28;
	v5 = v8 + 2;
	v6 = 256;
	do{
		v7 += 4;
		*(&v28 + v5 - v8) = *(uchar*)(v5 - 2);
		*(v7 - 3) = *(uchar*)(v5 - 1);
		*(v7 - 4) = *(uchar*)v5;
		*(&v29[v5] - v8) = 0;
		v5 += 4;
		--v6;
	}while( v6 );
	v12 = v18;
	WriteFile(v18, &v28, 1024u, &NumberOfBytesWritten, 0);
	v13 = SMemAlloc(v9, "SBMP.CPP", 347, 0);
	sub_15001B70(v13, a3, a4, a5);
	WriteFile(v12, (LPCVOID)v13, v9, &NumberOfBytesWritten, 0);
	SMemFree((void*)v13, "SBMP.CPP", 350, 0);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C0CC: using guessed type __int16 word_1501C0CC;
// 15002230: using guessed type char var_3FF[1023]; // звуки ?
//----- (150023C0) --------------------------------------------------------
int __fastcall sub_150023C0(HANDLE hFile, int a2, DWORD a3, int a4, int a5)
{
	int v5;                     // eax@1
	HANDLE v6;                  // ebx@1
	int v7;                     // ebp@1
	void* v8;                   // edi@1
	int v9;                     // eax@4
	char* v10;                  // ecx@4
	int v11;                    // esi@4
	char v12;                   // dl@5
	int v14;                    // [sp+10h] [bp-394h]@1
	DWORD NumberOfBytesWritten; // [sp+14h] [bp-390h]@1
	int v16;                    // [sp+18h] [bp-38Ch]@1
	char v17;                   // [sp+1Ch] [bp-388h]@4
	char Buffer;                // [sp+20h] [bp-384h]@1
	char v19;                   // [sp+21h] [bp-383h]@1
	char v20;                   // [sp+22h] [bp-382h]@1
	char v21;                   // [sp+23h] [bp-381h]@1
	__int16 v22;                // [sp+28h] [bp-37Ch]@1
	__int16 v23;                // [sp+2Ah] [bp-37Ah]@1
	__int16 v24;                // [sp+2Ch] [bp-378h]@1
	__int16 v25;                // [sp+2Eh] [bp-376h]@1
	char v26;                   // [sp+61h] [bp-343h]@1
	__int16 v27;                // [sp+62h] [bp-342h]@1
	char v28;                   // [sp+A0h] [bp-304h]@4
	char v29;                   // [sp+A2h] [bp-302h]@4
	v7 = a5;
	v6 = hFile;
	v16 = a2;
	memset(&Buffer, 0, 0x80u);
	v22 = a4 - 1;
	Buffer = 10;
	v19 = 5;
	v20 = 1;
	v21 = 8;
	v23 = a5 - 1;
	v24 = a4;
	v25 = a5;
	v26 = 1;
	v27 = a4;
	WriteFile(hFile, &Buffer, 0x80u, &NumberOfBytesWritten, 0);
	v5 = SMemAlloc(2 * a4 * a5, "SBMP.CPP", 383, 0);
	v8 = (void*)v5;
	NumberOfBytesWritten = a3;
	v14 = v5;
	if( a5 > 0 ){
		do{
			sub_15002510((int)&v14, (unsigned __int8**)&NumberOfBytesWritten, a4);
			--v7;
		}while( v7 );
		v5 = v14;
	}
	WriteFile(v6, v8, v5 - (uint)v8, (LPDWORD)&v17, 0);
	SMemFree((void*)v8, "SBMP.CPP", 390, 0);
	v10 = &v29;
	v11 = 256;
	v28 = 12;
	v9 = v16 + 2;
	do{
		v12 = *(uchar*)(v9 - 2);
		v9 += 4;
		*(v10 - 1) = v12;
		*v10 = *(uchar*)(v9 - 5);
		v10[1] = *(uchar*)(v9 - 4);
		v10 += 3;
		--v11;
	}while( v11 );
	WriteFile(v6, &v28, 0x301u, (LPDWORD)&v17, 0);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15002510) --------------------------------------------------------
int __fastcall sub_15002510(int a1, unsigned __int8 **a2, int a3)
{
	unsigned __int8* * v3; // edi@1
	int v4;                // esi@1
	int v5;                // eax@2
	unsigned __int8 v6;    // dl@2
	int v7;                // ebx@2
	int result;            // eax@9
	v4 = a3;
	v3 = a2;
	do{
		v7 = 1;
		v6 = **v3;
		v5 = (int)(*v3 + 1);
		--v4;
		for( * v3 = (unsigned __int8*)v5; v4; *v3 = (unsigned __int8*)v5 ){
			if( v6 != *(uchar*)v5 )
				break;
			if( v7 >= 63 )
				break;
			++v7;
			--v4;
			++v5;
		}
		if( v7 > 1 || v6 >= 0xC0u )
			*(uchar *)*(uint *)a1++ = v7 | 0xC0;
		result = *(uint*)a1;
		*(uchar*)*(uint*)a1++ = v6;
	}while( v4 );
	return result;
}

//----- (15002580) --------------------------------------------------------
int __stdcall sub_15002580()
{
	int result; // eax@1
	result = 0;
	dword_1501E990 = 0;
	dword_1501E994 = 0;
	return result;
}

// 1501E990: using guessed type int dword_1501E990;
// 1501E994: using guessed type int dword_1501E994;
//----- (150025A0) --------------------------------------------------------
int __stdcall sub_150025A0()
{
	int result; // eax@1
	result = 0;
	dword_1501E998 = 0;
	dword_1501E99C = 0;
	return result;
}

// 1501E998: using guessed type int dword_1501E998;
// 1501E99C: using guessed type int dword_1501E99C;
//----- (150025B0) --------------------------------------------------------
int __stdcall strm2_471()
{
	int v0;   // ebx@1
	int v1;   // esi@2
	int v2;   // eax@5
	void* v3; // eax@6
	void* v4; // eax@10
	int v5;   // esi@11
	void* v6; // eax@12
	int v7;   // esi@13
	v0 = 0;
	do{
		v1 = dword_1501E998;
		if( !v0 )
			v1 = dword_1501E990;
		for( ; v1; v1 = *(uint*)(v1 + 4) ){
			v2 = *(uint*)(v1 + 8);
			if( (v2 & 0x30000) == 131072 ){
				v3 = *(void**)(v1 + 56);
				if( v3 ){
					SMemFree(v3, "SCMD.CPP", 537, 0);
					*(uint*)(v1 + 56) = 0;
				}
			}
		}
		++v0;
	}while( v0 <= 1 );
	v4 = (void*)dword_1501E994;
	if( dword_1501E994 ){
		do{
			v5 = *(uint*)dword_1501E990;
			SMemFree(v4, ".?AUCMDDEF@@", -2, 0);
			v4 = (void*)v5;
			dword_1501E994 = v5;
		}while( v5 );
	}
	v6 = (void*)dword_1501E99C;
	dword_1501E990 = 0;
	if( dword_1501E99C ){
		do{
			v7 = *(uint*)dword_1501E998;
			SMemFree(v6, ".?AUCMDDEF@@", -2, 0);
			v6 = (void*)v7;
			dword_1501E99C = v7;
		}while( v7 );
	}
	dword_1501E998 = 0;
	dword_1501E9A0 = 0;
	return 1;
}

// 1501E990: using guessed type int dword_1501E990;
// 1501E994: using guessed type int dword_1501E994;
// 1501E998: using guessed type int dword_1501E998;
// 1501E99C: using guessed type int dword_1501E99C;
// 1501E9A0: using guessed type int dword_1501E9A0;
//----- (15002680) --------------------------------------------------------
int __stdcall strm2_472(int a1)
{
	return strm2_473(a1) != 0;
}

//----- (150026A0) --------------------------------------------------------
int __stdcall strm2_473(int a1)
{
	int v1;     // eax@1
	int result; // eax@5
	v1 = dword_1501E998;
	if( !dword_1501E998 )
		goto LABEL_9;
	do{
		if( *(uint*)(v1 + 12) == a1 )
			break;
		v1 = *(uint*)(v1 + 4);
	}while( v1 );
	if( v1 )
		result = *(uint*)(v1 + 56);
	else
		LABEL_9:
		result = 0;
	return result;
}

// 1501E998: using guessed type int dword_1501E998;
//----- (150026D0) --------------------------------------------------------
int __stdcall strm2_474(int a1, char* Dest, int a3)
{
	int result; // eax@4
	int v4;     // eax@5
	char* v5;   // eax@9
	if( Dest )
		*Dest = 0;
	if( Dest != 0 ? -(a3 != 0) : 0 ){
		v4 = dword_1501E998;
		if( !dword_1501E998 )
			goto LABEL_15;
		do{
			if( *(uint*)(v4 + 12) == a1 )
				break;
			v4 = *(uint*)(v4 + 4);
		}while( v4 );
		if( v4 ){
			v5 = *(char**)(v4 + 56);
			if( v5 ){
				strncpy(Dest, v5, a3 - 1);
				Dest[a3 - 1] = 0;
			}
			result = 1;
		}else{
			LABEL_15:
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E998: using guessed type int dword_1501E998;
//----- (15002750) --------------------------------------------------------
int __stdcall strm2_475(int a1, int a2, int a3, int (__stdcall *a4)(uint))
{
	int result;                  // eax@2
	int(__stdcall * v5)(uint); // edi@5
	int v6;                      // eax@7
	int v7;                      // esi@7
	char v8;                     // [sp+8h] [bp-18h]@5
	if( a1 ){
		if( a2 )
			sub_150029B0((int)&a1, 0, 0);
		memset(&v8, 0, 0x18u);
		v5 = a4;
		a2 = dword_1501E990;
		result = sub_15002A80((int)&a1, (int*)&v8, (int)&a2, a3, a4);
		if( result ){
			v6 = a2;
			v7 = 1;
			while( v6 ){
				if( !v7 )
					break;
				if( (*(uint*)(v6 + 8) & 0x3000000) == 33554432 ){
					v7 = 0;
				}else{
					v6 = *(uint*)(v6 + 4);
					a2 = v6;
				}
			}
			if( v5 ){
				if( !v7 )
					sub_15002820(v5, 0x8510006Du, (int)&ZeroByte);
			}
			result = v7;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E990: using guessed type int dword_1501E990;
//----- (15002820) --------------------------------------------------------
int __fastcall sub_15002820(int (__stdcall *checkFunct)(uint), DWORD a2, int a3)
{
	int result;                   // eax@1
	int v4;                       // ebp@1
	int v5;                       // edx@4
	int v6;                       // esi@4
	UINT v7;                      // ST04_4@7
	int v8;                       // eax@7
	int v9;                       // ecx@13
	wchar_t v10[2];               // edi@13
	char v11;                     // zf@15
	unsigned int v12;             // edx@17
	int v13;                      // ecx@17
	int v14;                      // ecx@17
	CHAR* v15;                    // edi@17
	void* v16;                    // esi@17
	char v17;                     // zf@19
	int(__stdcall * v18)(uint); // [sp+10h] [bp-210h]@1
	int v19;                      // [sp+14h] [bp-20Ch]@22
	int v20;                      // [sp+18h] [bp-208h]@22
	CHAR* v21;                    // [sp+1Ch] [bp-204h]@22
	CHAR Str;                     // [sp+20h] [bp-200h]@7
	char v23;                     // [sp+21h] [bp-1FFh]@7
	__int16 v24;                  // [sp+11Dh] [bp-103h]@7
	char v25;                     // [sp+11Fh] [bp-101h]@7
	CHAR v26;                     // [sp+120h] [bp-100h]@1
	char v27;                     // [sp+121h] [bp-FFh]@1
	__int16 v28;                  // [sp+21Dh] [bp-3h]@1
	char v29;                     // [sp+21Fh] [bp-1h]@1
	v18 = checkFunct;
	result = 0;
	v26 = ZeroByte;
	memset(&v27, 0, 0xFCu);
	v28 = 0;
	v4 = a2;
	v29 = 0;
	if( a2 == 110 ){
		v6 = 2;
		v5 = 20748;
	}else{
		if( a2 == -2062548891 ){
			v6 = 0;
			v5 = 20746;
		}else{
			if( a2 != -2062548883 )
				return result;
			v6 = 1;
			v5 = 20747;
		}
	}
	Str = ZeroByte;
	memset(&v23, 0, 0xFCu);
	v24 = 0;
	v25 = 0;
	v7 = v5;
	v8 = GetStormDllHandle();
	LoadStringA((HINSTANCE)v8, v7, &Str, 256);
	if( !Str )
		strcpy(&Str, off_1501C0D0[v6]);
	if( strstr(&Str, "%s") )
		wsprintfA(&v26, &Str, a3);
	else
		strcpy(&v26, &Str);
	if( v26 ){
		*(uint*)v10 = (uint)L"\n";
		v9 = -1;
		do{
			if( !v9 )
				break;
			v11 = **(uchar**)v10 == 0;
			++*(uint*)v10;
			--v9;
		}while( !v11 );
		v14 = ~v9;
		v16 = (void*)(*(uint*)v10 - v14);
		v15 = &v26;
		v12 = v14;
		v13 = -1;
		do{
			if( !v13 )
				break;
			v17 = *v15++ == 0;
			--v13;
		}while( !v17 );
		memcpy(v15 - 1, v16, v12);
	}
	SErrSetLastError(v4);
	v19 = v4;
	v20 = a3;
	v21 = &v26;
	return v18((uint)&v19);
}

// 1501C0D0: using guessed type char *off_1501C0D0[3];
// 1501C14C: using guessed type wchar_t asc_1501C14C[2];
//----- (150029B0) --------------------------------------------------------
unsigned int __fastcall sub_150029B0(int a1, int a2, unsigned int a3)
{
	char v3;             // al@1
	int v4;              // ebx@1
	int v5;              // esi@1
	int v6;              // edi@6
	char v7;             // al@7
	unsigned int result; // eax@17
	unsigned int v9;     // [sp+10h] [bp-Ch]@6
	int v10;             // [sp+14h] [bp-8h]@1
	int i;               // [sp+18h] [bp-4h]@1
	v5 = *(uint*)a1;
	v4 = 0;
	v3 = **(uchar**)a1;
	v10 = a2;
	for( i = a1; v3; v3 = *(uchar*)(v5++ + 1) ){
		if( !strchr(" ,;\"\t\n\r", v3) )
			break;
		if( *(uchar*)v5 == 34 )
			v4 = v4 == 0;
	}
	v6 = v10;
	v9 = 0;
	while( 1 ){
		v7 = *(uchar*)v5;
		if( *(uchar*)v5 == 34 )
			v4 = v4 == 0;
		if( !v7 || !v4 && strchr(" ,;\"\t\n\r", v7) )
			break;
		if( 1 - v10 + v6 < a3 ){
			*(uchar*)v6 = *(uchar*)v5;
			++v9;
			++v6;
		}
		++v5;
	}
	if( *(uchar*)v5 )
		++v5;
	result = v9;
	if( v9 < a3 )
		*(uchar *)(v9 + v10) = 0;
	*(uint*)i = v5;
	return result;
}

//----- (15002A80) --------------------------------------------------------
int __fastcall sub_15002A80(int a1, int *a2, int a3, int a4, int (__stdcall *a5)(uint))
{
	int* v5;     // ebx@1
	int v6;      // esi@1
	int v7;      // eax@3
	int v9;      // [sp+10h] [bp-104h]@2
	char v10;    // [sp+14h] [bp-100h]@2
	char v11;    // [sp+15h] [bp-FFh]@2
	__int16 v12; // [sp+111h] [bp-3h]@2
	char v13;    // [sp+113h] [bp-1h]@2
	v6 = a1;
	v5 = a2;
	if( **(uchar**)a1 ){
		do{
			v9 = *(uint*)v6;
			v10 = ZeroByte;
			memset(&v11, 0, 0xFCu);
			v12 = 0;
			v13 = 0;
			sub_150029B0((int)&v9, (int)&v10, 0x100u);
			if( !sub_15002B20((int*)&v10, v5, a3, a4, a5) )
				break;
			v7 = v9;
			*(uint*)v6 = v9;
		}while( *(uchar*)v7 );
	}
	return **(uchar**)v6 == 0;
}

//----- (15002B20) --------------------------------------------------------
int __fastcall sub_15002B20(int *a1, int *a2, int a3, int a4, int (__stdcall *a5)(uint))
{
	int* v5;                     // edi@1
	int v6;                      // esi@1
	int(__stdcall * v8)(uint); // ST08_4@4
	v6 = (int)a1;
	v5 = a2;
	if( *(uchar*)a1 == 64 )
		return sub_15003010((int)((char*)a1 + 1), a2, a3, a4, a5);
	if( strchr("-/", *(uchar*)a1) ){
		v8 = a5;
		*v5 = 0;
		return sub_150030E0(v6 + 1, (int)v5, v8);
	}
	if( *v5 )
		return sub_15002F10(v6, v5, (int)&a5);
	if( *(uint*)a3 ){
		if( sub_15002BF0(*(uint*)a3, v6, (int)&a5) ){
			*(uint*)a3 = *(uint*)(*(uint*)a3 + 4);
			return 1;
		}
	}else{
		if( a4 )
			return ((int(__stdcall*)(int))a4)(v6);
		if( a5 )
			sub_15002820(a5, 0x85100065u, v6);
	}
	return 0;
}

//----- (15002BF0) --------------------------------------------------------
int __fastcall sub_15002BF0(int a1, int a2, int a3)
{
	int v3;                              // eax@1
	int v4;                              // ebp@1
	int v5;                              // esi@1
	int v6;                              // eax@1
	int result;                          // eax@4
	int (__stdcall *v8)(uint, uint); // eax@8
	int v9;                              // ecx@9
	int v10;                             // edx@9
	int v11;                             // edx@9
	int v12;                             // ecx@9
	int v13;                             // eax@11
	int v14;                             // ebx@12
	int v15;                             // eax@16
	int v16;                             // ecx@16
	void* v17;                           // eax@19
	char* v18;                           // edx@21
	int v19;                             // [sp+10h] [bp-1Ch]@9
	int v20;                             // [sp+14h] [bp-18h]@9
	int v21;                             // [sp+18h] [bp-14h]@9
	int v22;                             // [sp+1Ch] [bp-10h]@9
	int v23;                             // [sp+20h] [bp-Ch]@9
	int v24;                             // [sp+24h] [bp-8h]@9
	int v25;                             // [sp+28h] [bp-4h]@9
	int v26;                             // [sp+30h] [bp+4h]@11
	v4 = a1;
	v5 = a2;
	*(uint*)a3 = 0;
	v6 = *(uint*)(a1 + 8);
	v3 = v6 & 0x30000;
	if( v3 ){
		if( v3 == 65536 ){
			sub_15002DF0(a1, (char*)a2, a3);
		}else{
			if( v3 != 131072 )
				return 0;
			sub_15002E80(a1, (char*)a2, a3);
		}
	}else{
		sub_15002D80(a1, a2, a3);
	}
	v8
	= *(int (__stdcall **)(uint, uint))(v4 + 52);
	if( !v8
	|| (v9 = *(uint *)(v4 + 8),
	v20 = *(uint *)(v4 + 12),
	v10 = *(uint *)(v4 + 44),
	v19 = v9,
	v22 = v10,
	v11 = *(uint *)(v4 + 40),
	v21 = v4 + 16,
	v12 = *(uint *)(v4 + 36),
	v24 = v11,
	v23 = v12,
	v25 = *(uint *)(v4 + 56),
	(result = v8((uint)&v19, v5)) != 0) ){
		v13 = 0;
		v26 = 0;
		do{
			v14 = dword_1501E998;
			if( !v13 )
				v14 = dword_1501E990;
			if( v14 ){
				do{
					if( *(uint*)(v14 + 12) == *(uint*)(v4 + 12) ){
						v15 = *(uint*)(v14 + 8);
						v16 = v15 ^ *(uint*)(v4 + 8);
						if( !(v16 & 0x30000) ){
							if( v14 != v4 ){
								if( (v15 & 0x30000) == 131072 ){
									v17 = *(void**)(v14 + 56);
									if( v17 )
										SMemFree(v17, "SCMD.CPP", 321, 0);
									v18 = (char*)SMemAlloc(strlen(*(char**)(v4 + 56)), "SCMD.CPP", 322, 0);
									*(uint*)(v14 + 56) = (uint)v18;
									strcpy(v18, *(char**)(v4 + 56));
								}else{
									*(uint*)(v14 + 56) = *(uint*)(v4 + 56);
								}
							}
						}
					}
					v14 = *(uint*)(v14 + 4);
				}while( v14 );
				v13 = v26;
			}
			++v13;
			v26 = v13;
		}while( v13 <= 1 );
		result = 1;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E990: using guessed type int dword_1501E990;
// 1501E998: using guessed type int dword_1501E998;
//----- (15002D80) --------------------------------------------------------
int __fastcall sub_15002D80(int a1, int a2, int a3)
{
	int v3;     // esi@2
	int result; // eax@6
	int v5;     // edx@6
	int v6;     // edx@8
	if( *(uchar*)a2 == 45 ){
		v3 = 0;
		*(uint*)a3 = 1;
	}else{
		if( *(uchar*)a2 == 43 ){
			v3 = 1;
			*(uint*)a3 = 1;
		}else{
			v3 = (*(uint*)(a1 + 8) & 1) != 1;
		}
	}
	result = ~*(uint*)(a1 + 40);
	v5 = result& *(uint*)(a1 + 56);
	*(uint*)(a1 + 56) = v5;
	if( v3 )
		*(uint *)(a1 + 56) = v5 | *(uint *)(a1 + 36);
	v6 = *(uint*)(a1 + 44);
	if( v6 ){
		*(uint*)v6 &= result;
		if( v3 ){
			result = *(uint*)(a1 + 44);
			*(uint*)result |= *(uint*)(a1 + 36);
		}
	}
	return result;
}

//----- (15002DF0) --------------------------------------------------------
char* __fastcall sub_15002DF0(int a1, char* a2, int a3)
{
	int v3;              // ebx@1
	char* v4;            // edi@1
	void* v5;            // esi@1
	int v6;              // eax@1
	unsigned __int32 v7; // eax@2
	char* result;        // eax@5
	void* v9;            // edi@7
	unsigned int v10;    // ecx@8
	unsigned int v11;    // ebx@8
	char* EndPtr;        // [sp+0h] [bp-4h]@1
	EndPtr = (char*)a1;
	v3 = a1;
	v4 = a2;
	v6 = *(uint*)(a1 + 8) & 1;
	EndPtr = 0;
	v5 = (void*)(a1 + 56);
	if( (uchar)v6 == 1 )
		v7 = strtol(a2, &EndPtr, 0);
	else
		v7 = strtoul(a2, &EndPtr, 0);
	*(uint*)v5 = v7;
	if( EndPtr ){
		result = (char*)(EndPtr - v4);
		*(uint*)a3 = EndPtr - v4;
	}else{
		result = 0;
		*(uint*)a3 = strlen(v4) - 1;
	}
	v9 = *(void**)(v3 + 44);
	if( v9 ){
		v11 = *(uint*)(v3 + 48);
		v10 = 4;
		if( v11 <= 4 )
			v10 = v11;
		result = (char*)v10;
		memcpy(v9, v5, v10);
	}
	return result;
}

//----- (15002E80) --------------------------------------------------------
char* __fastcall sub_15002E80(int a1, char* a2, int a3)
{
	void* v3;     // eax@1
	char* v4;     // ebx@1
	int v5;       // ebp@1
	char* result; // eax@3
	char* v7;     // edx@3
	v4 = a2;
	v5 = a1;
	*(uint*)a3 = strlen(a2) - 1;
	v3 = *(void**)(v5 + 56);
	if( v3 )
		SMemFree(v3, "SCMD.CPP", 136, 0);
	v7 = (char*)SMemAlloc(strlen(v4), "SCMD.CPP", 137, 0);
	*(uint*)(v5 + 56) = (uint)v7;
	strcpy(v7, v4);
	result = *(char**)(v5 + 44);
	if( result )
		result = strncpy(result, v4, *(uint*)(v5 + 48));
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15002F10) --------------------------------------------------------
int __fastcall sub_15002F10(int a1, int* a2, int a3)
{
	int v3;     // ebx@1
	int v4;     // ebp@1
	int* v5;    // edi@1
	int v6;     // esi@1
	int v7;     // eax@3
	int result; // eax@6
	v3 = a3;
	v5 = a2;
	*(uint*)a3 = 0;
	v4 = a1;
	v6 = *a2;
	*a2 = 0;
	if( v6 ){
		while( sub_15002BF0(v6, v4, (int)&a3) ){
			v7 = *(uint*)v3;
			if( *(uint*)v3 <= a3 )
				v7 = a3;
			*(uint*)v3 = v7;
			v6 = sub_15002F90((char*)v5 + 4, *(uint*)(v6 + 4), v5[5]);
			if( !v6 )
				return 1;
		}
		result = 0;
	}else{
		result = 1;
	}
	return result;
}

//----- (15002F90) --------------------------------------------------------
int __fastcall sub_15002F90(char* Str, int a2, int a3)
{
	char* v3;        // ebx@1
	int v4;          // ebp@1
	int v5;          // edi@1
	int i;           // esi@1
	unsigned int v7; // kr00_4@1
	int v8;          // eax@2
	int v9;          // ecx@4
	int v10;         // eax@5
	size_t v12;      // [sp-4h] [bp-18h]@4
	v3 = Str;
	v7 = strlen(Str);
	v4 = 0;
	v5 = a3 - 1;
	for( i = a2; i; i = *(uint*)(i + 4) ){
		v8 = *(uint*)(i + 32);
		if( v8 > v5 ){
			if( v8 <= (int)(v7 - 1) ){
				v9 = *(uint*)(i + 8);
				v12 = *(uint*)(i + 32);
				if( BYTE1(v9) & 1 )
					v10 = strncmp((char*)(i + 16), v3, v12);
				else
					v10 = strnicmp((char*)(i + 16), v3, v12);
				if( v10 == 0 ){
					v5 = *(uint*)(i + 32);
					v4 = i;
				}
			}
		}
	}
	return v4;
}

//----- (15003010) --------------------------------------------------------
int __fastcall sub_15003010(int lpFileName, int *edx0, int a2, int a3, int (__stdcall *a4)(uint))
{
	HANDLE v5;               // eax@1
	int* v6;                 // ebx@1
	int v7;                  // edi@1
	void* v8;                // esi@1
	int result;              // eax@4
	DWORD v10;               // edi@5
	int v11;                 // esi@5
	void* v12;               // [sp+Ch] [bp-8h]@5
	DWORD NumberOfBytesRead; // [sp+10h] [bp-4h]@5
	v7 = lpFileName;
	v6 = edx0;
	v5 = CreateFileA((LPCSTR)lpFileName, 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
	v8 = v5;
	if( v5 ){
		v10 = GetFileSize(v5, 0);
		v12 = (void*)SMemAlloc(v10 + 1, "SCMD.CPP", 374, 0);
		ReadFile(v8, v12, v10, &NumberOfBytesRead, 0);
		CloseHandle(v8);
		*((uchar*)v12 + NumberOfBytesRead) = 0;
		v11 = sub_15002A80((int)&v12, v6, a2, a3, a4);
		SMemFree(v12, "SCMD.CPP", 390, 0);
		result = v11;
	}else{
		if( a4 )
			sub_15002820(a4, 0x6Eu, v7);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150030E0) --------------------------------------------------------
int __fastcall sub_150030E0(int a1, int a2, int (__stdcall *a3)(uint))
{
	unsigned int v3;  // esi@2
	unsigned int v4;  // ebp@3
	int v5;           // edx@5
	int v6;           // ebp@5
	unsigned int v7;  // ecx@5
	int v8;           // eax@6
	void* v9;         // ebx@6
	int v10;          // ecx@8
	int v11;          // edi@8
	char v12;         // zf@10
	int v13;          // eax@12
	unsigned int v14; // ecx@12
	char v15;         // dl@12
	void* v16;        // esi@12
	void* v17;        // edi@12
	void* v18;        // esi@12
	int v19;          // edx@12
	int v20;          // ecx@14
	int v21;          // esi@18
	int result;       // eax@22
	int v23;          // [sp+10h] [bp-110h]@1
	int v24;          // [sp+10h] [bp-110h]@18
	int v25;          // [sp+14h] [bp-10Ch]@1
	int v26;          // [sp+18h] [bp-108h]@6
	int v27;          // [sp+1Ch] [bp-104h]@20
	char Str;         // [sp+20h] [bp-100h]@1
	char v29;         // [sp+21h] [bp-FFh]@1
	__int16 v30;      // [sp+11Dh] [bp-3h]@1
	char v31;         // [sp+11Fh] [bp-1h]@1
	v25 = a2;
	Str = ZeroByte;
	v23 = a1;
	memset(&v29, 0, 0xFCu);
	v30 = 0;
	v31 = 0;
	if( *(uchar*)a1 ){
		while( 1 ){
			v3 = strlen((char*)v23) - 1;
			if( strlen(&Str) - 1 >= 1 )
				v4 = strlen(&Str) - 1;
			else
				v4 = 1;
			v5 = 0;
			v7 = v4;
			v6 = v4 - 1;
			if( v7 ){
				v8 = v3 - (uint)&Str;
				v9 = &Str + v6;
				v26 = v3 - (uint)&Str;
				do{
					if( (int)((char*)v9 + v8) < 256 ){
						v11 = v23;
						v10 = -1;
						do{
							if( !v10 )
								break;
							v12 = *(uchar*)v11++ == 0;
							--v10;
						}while( !v12 );
						v14 = ~v10;
						v15 = v14;
						v16 = (void*)(v11 - v14);
						v14 >>= 2;
						memcpy(v9, v16, 4 * v14);
						v18 = (char*)v16 + 4 * v14;
						v17 = (char*)v9 + 4 * v14;
						LOBYTE_IDA(v14) = v15;
						v19 = dword_1501E998;
						memcpy(v17, v18, v14 & 3);
						v13 = sub_15002F90(&Str, v19, 0);
						v5 = v13;
						if( v13 ){
							v6 = *(uint*)(v13 + 32);
							*(&Str + v6) = 0;
							break;
						}
						v8 = v26;
					}
					v20 = v6--;
					v9 = (char*)v9 - 1;
				}while( v20 );
			}
			if( !v5 )
				break;
			v24 = v6 + v23;
			*(uint*)v25 = v5;
			*(uint*)(v25 + 20) = v6;
			strcpy((char*)(v25 + 4), &Str);
			v21 = v24;
			if( !*(uchar*)v24 && *(uint*)(v5 + 8) & 0x30000 )
				goto LABEL_26;
			if( !sub_15002F10(v24, (int*)v25, (int)&v27) )
				goto LABEL_25;
			v23 = v27 + v24;
			if( !*(uchar*)(v27 + v21) )
				return 1;
		}
		if( a3 )
			sub_15002820(a3, 0x85100065u, v23);
		LABEL_25:
		result = 0;
	}else{
		LABEL_26:
		result = 1;
	}
	return result;
}

// 1501E998: using guessed type int dword_1501E998;
//----- (15003290) --------------------------------------------------------
int __stdcall strm2_476(int a1, unsigned int a2)
{
	int v2;          // esi@1
	int result;      // eax@2
	unsigned int v4; // edi@3
	v2 = a1;
	if( a1 ){
		v4 = 0;
		if( a2 ){
			while( strm2_477(*(uint*)v2, *(uint*)(v2 + 4), *(char**)(v2 + 8), 0, 0, 1, -1, *(uint*)(v2 + 12)) ){
				v2 += 16;
				++v4;
				if( v4 >= a2 )
					return 1;
			}
			result = 0;
		}else{
			result = 1;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15003300) --------------------------------------------------------
int __stdcall strm2_477(int a1, int a2, char* a3, int a4, int a5, int a6, int a7, int a8)
{
	char* v8;        // esi@1
	unsigned int v9; // kr04_4@3
	int v10;         // ecx@5
	int v11;         // eax@7
	int v12;         // edx@7
	int v13;         // ebp@7
	char v14;        // zf@7
	int v15;         // ecx@9
	int v16;         // eax@11
	int v17;         // ecx@13
	int v18;         // eax@15
	int v19;         // ebx@15
	int v20;         // ebx@15
	int v21;         // ecx@18
	int result;      // eax@21
	int v23;         // eax@23
	int v24;         // [sp+18h] [bp+4h]@7
	v8 = (char*)a3;
	if( !a3 )
		v8 = &ZeroByte;
	v9 = strlen(v8);
	if( !a5 || a4 )
		v10 = -1;
	else
		v10 = 0;
	v13 = a1;
	v11 = v10&(((int)(v9 - 1) >= 16) - 1);
	v12 = a1 & 0x3000000;
	v14 = (a1 & 0x3000000) == 33554432;
	v24 = a1 & 0x3000000;
	if( v14 && dword_1501E9A0 )
		v15 = 0;
	else
		v15 = -1;
	v16 = v15 & v11;
	if( v12 || (int)(v9 - 1) > 0 )
		v17 = -1;
	else
		v17 = 0;
	v20 = v13;
	v18 = v17 & v16;
	v19 = v20 & 0x30000;
	if( v19 || !a4 || a5 == 4 )
		v21 = -1;
	else
		v21 = 0;
	if( v18 & v21 ){
		if( v12 ){
			v23 = SMemAlloc(60, ".?AUCMDDEF@@", -2, 8);
			*(uint*)v23 = dword_1501E994;
			if( dword_1501E990 ){
				*(uint*)(dword_1501E994 + 4) = v23;
				dword_1501E994 = v23;
			}else{
				dword_1501E994 = v23;
				dword_1501E990 = v23;
			}
		}else{
			v23 = SMemAlloc(60, ".?AUCMDDEF@@", -2, 8);
			*(uint*)v23 = dword_1501E99C;
			if( dword_1501E998 ){
				*(uint*)(dword_1501E99C + 4) = v23;
				dword_1501E99C = v23;
			}else{
				dword_1501E99C = v23;
				dword_1501E998 = v23;
			}
		}
		strcpy((char*)(v23 + 16), v8);
		*(uint*)(v23 + 32) = v9 - 1;
		*(uint*)(v23 + 12) = a2;
		*(uint*)(v23 + 48) = a5;
		*(uint*)(v23 + 44) = a4;
		*(uint*)(v23 + 8) = v13;
		*(uint*)(v23 + 40) = a7;
		*(uint*)(v23 + 36) = a6;
		*(uint*)(v23 + 52) = a8;
		if( v19 || (v13 & 1) != 1 )
			*(uint *)(v23 + 56) = 0;
		else
			*(uint*)(v23 + 56) = a6;
		if( v24 == 16777216 )
			dword_1501E9A0 = 1;
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E990: using guessed type int dword_1501E990;
// 1501E994: using guessed type int dword_1501E994;
// 1501E998: using guessed type int dword_1501E998;
// 1501E99C: using guessed type int dword_1501E99C;
// 1501E9A0: using guessed type int dword_1501E9A0;
//----- (150034E0) --------------------------------------------------------
int __stdcall sub_150034E0()
{
	int result; // eax@1
	result = 0;
	dword_1501E9B0 = 0;
	dword_1501E9B4 = 0;
	return result;
}

// 1501E9B0: using guessed type int dword_1501E9B0;
// 1501E9B4: using guessed type int dword_1501E9B4;
//----- (15003500) --------------------------------------------------------
int __stdcall sub_15003500()
{
	int result; // eax@1
	result = 0;
	dword_1501E9A8 = 0;
	dword_1501E9AC = 0;
	return result;
}

// 1501E9A8: using guessed type int dword_1501E9A8;
// 1501E9AC: using guessed type int dword_1501E9AC;
//----- (15003510) --------------------------------------------------------
int __fastcall sub_15003510(int a1, int a2)
{
	int v2;     // eax@1
	int v3;     // edx@3
	int v4;     // ecx@3
	int v5;     // ecx@3
	int result; // eax@5
	char v7;    // bl@11
	int v8;     // edi@11
	int v9;     // esi@18
	char v10;   // cl@20
	int v11;    // ebp@20
	int v12;    // esi@22
	char v13;   // al@24
	char v14;   // al@28
	char v15;   // bl@42
	char v16;   // al@45
	int v17;    // esi@47
	char v18;   // al@51
	char v19;   // [sp+10h] [bp-Ch]@20
	char v20;   // [sp+11h] [bp-Bh]@18
	char v21;   // [sp+12h] [bp-Ah]@18
	int v22;    // [sp+14h] [bp-8h]@1
	int v23;    // [sp+18h] [bp-4h]@20
	v2 = *(uint*)(a2 + 16);
	v22 = a1;
	if( v2 == 4 && !*(uchar*)(a2 + 12) ){
		*(uchar*)a1 = (((*(uint*)(a2 + 20) <= 1) - 1) & 0x82) - 1;
		v5 = a1 + 1;
		*(uchar*)v5 = byte_1501E048[*(uchar*)(a2 + 8)] | 0xC0;
		v3 = *(uint*)(a2 + 20);
		v4 = v5 + 1;
		if( v3 > 1 ){
			*(uint*)v4 = v3;
			v4 += 4;
		}
		return v4;
	}
	if( !v2 ){
		if( *(uchar*)(a2 + 8) == 8 ){
			if( !*(uchar*)(a2 + 15) ){
				if( !*(uchar*)(a2 + 14) ){
					*(uint*)(a2 + 16) = 3;
					*(uint*)(a2 + 8) = dword_1501E9C0;
				}
			}
		}
	}
	v7 = *(uchar*)(a2 + 12);
	v8 = 0;
	if( v7 != 8 && v7 != 9 && v7 ){
		if( *(uchar*)(a2 + 11) || *(uchar*)(a2 + 10) )
			v8 = 1;
	}else{
		v8 = 2;
	}
	v9 = 2 * (v8 + 2 * *(uint*)(a2 + 16) + *(uint*)(a2 + 16));
	v20 = byte_1501E018[v9];
	v21 = byte_1501E019[v9];
	if( *(uint*)(a2 + 20) == 1 )
		v20 &= 0xFEu;
	v10 = 0;
	v23 = 0;
	v19 = 0;
	v11 = a2 + 12;
	if( !v8 )
		v11 = a2 + 8;
	v12 = a2 + 8;
	if( !v8 )
		v12 = a2 + 12;
	v13 = *(uchar*)(v12 + 3);
	if( !v13 ){
		LABEL_30:
		if( !*(uchar*)(v12 + 2) ){
			if( *(uchar*)(v12 + 1) )
				v14 = *(&byte_1501E057[2 * *(uchar*)v12] + *(uchar*)(v12 + 1)) | 0xC0;
			else
				v14 = byte_1501E048[*(uchar*)v12] | 0xC0;
			goto LABEL_39;
		}
		LABEL_34:
		v14 = 4;
		v23 = 1;
		if( *(uchar*)v12 == 4 ){
			if( *(uchar*)(v12 + 2) ){
				v14 = 4;
				v10 = 8 * byte_1501E04C | byte_1501E048[*(uchar*)(v12 + 2)];
				v19 = 8 * byte_1501E04C | byte_1501E048[*(uchar*)(v12 + 2)];
			}else{
				v14 = 69;
			}
		}else{
			v10 = 8 * byte_1501E048[*(uchar*)(v12 + 2)] | byte_1501E048[*(uchar*)v12];
			v19 = 8 * byte_1501E048[*(uchar*)(v12 + 2)] | byte_1501E048[*(uchar*)v12];
		}
		goto LABEL_39;
	}
	if( *(uchar*)(v12 + 2) || *(uchar*)v12 != 3 && *(uchar*)v12 != 2 ){
		if( v13 )
			goto LABEL_34;
		goto LABEL_30;
	}
	v14 = byte_1501E048[*(uchar*)v12];
	LABEL_39:
	if( v8 == 2 || v21 ){
		v15 = 8 * v21;
	}else{
		if( *(uchar*)(v11 + 1) ){
			v10 = v19;
			v15 = 8 * *(&byte_1501E057[2 * *(uchar*)v11] + *(uchar*)(v11 + 1));
		}else{
			v15 = 8 * byte_1501E048[*(uchar*)v11];
		}
	}
	v16 = v15 | v14;
	if( *(uint*)(a2 + 20) == 2 )
		*(uchar *)v22++ = 102;
	*(uchar*)v22 = v20;
	*(uchar*)(v22 + 1) = v16;
	v17 = v22 + 2;
	if( v23 ){
		*(uchar*)v17 = v10;
		v17 = v22 + 3;
	}
	if( *(uint*)(a2 + 16) == 7 ){
		*(uchar*)v17 = 16;
		result = v17 + 1;
	}else{
		v18 = *(uchar*)(a2 + 12);
		if( v18 == 8 || v18 == 9 ){
			*(uint*)v17 = (v18 != 9) - 1;
			v17 += *(uint*)(a2 + 20);
		}
		result = v17;
	}
	return result;
}

// 1501E04C: using guessed type char byte_1501E04C;
// 1501E9C0: using guessed type int dword_1501E9C0;
//----- (15003790) --------------------------------------------------------
int __thiscall sub_15003790(void* This)
{
	char* v2; // ecx@1
	*(uchar*)This = -23;
	v2 = (char*)This + 1;
	*(uint*)v2 = 0;
	return (int)(v2 + 4);
}

//----- (150037A0) --------------------------------------------------------
int __fastcall sub_150037A0(char* a1, int a2)
{
	int v2;   // eax@1
	char* v3; // ebx@1
	char* v5; // ebx@4
	int v6;   // esi@4
	int v7;   // ebp@5
	char* v8; // edi@17
	int v9;   // eax@23
	int v10;  // [sp+10h] [bp-4h]@4
	v2 = *(uint*)(a2 + 16);
	v3 = a1;
	if( v2 == 4 && !*(uchar*)(a2 + 12) ){
		wsprintfA((LPSTR)a1, "inc  %c,%u\n", byte_1501E090[*(uchar*)(a2 + 8)], *(uint*)(a2 + 20));
		return (int)&v3[strlen(v3) - 1];
	}
	strcpy((char*)a1, off_1501E070[v2]);
	v6 = 1;
	v5 = &v3[strlen(v3) - 1];
	v10 = 1;
	do{
		v7 = a2 + 8;
		if( !v6 )
			v7 = a2 + 12;
		if( *(uchar*)v7 ){
			if( !v6 )
				*v5++ = 44;
			if( *(uchar*)(v7 + 3) || *(uchar*)(v7 + 2) )
				*v5++ = 91;
			if( *(uchar*)v7 != 9 ){
				++v5;
				*(v5 - 1) = byte_1501E090[*(uchar*)v7];
				goto LABEL_23;
			}
			if( *(uint*)(a2 + 20) == 1 ){
				v8 = "0FFh";
			}else{
				if( *(uint*)(a2 + 20) == 2 ){
					v8 = "0FFFFh";
				}else{
					if( *(uint*)(a2 + 20) != 4 ){
						LABEL_21:
						v6 = v10;
						v5 += strlen(v5) - 1;
						LABEL_23:
						v9 = *(uint*)(a2 + 20);
						if( v9 < 4 ){
							if( !*(uchar*)(v7 + 3) ){
								if( !*(uchar*)(v7 + 2) ){
									if( v9 == 2 )
										*v5 = 120;
									else
										*v5 = *(uchar*)(v7 + 1) != 1 ? 104 : 108;
									++v5;
								}
							}
						}
						if( *(uchar*)(v7 + 2) ){
							*v5 = 43;
							v5 += 2;
							*(v5 - 1) = byte_1501E090[*(uchar*)(v7 + 2)];
						}
						if( *(uchar*)(v7 + 3) || *(uchar*)(v7 + 2) )
							*v5++ = 93;
						goto LABEL_35;
					}
					v8 = "0FFFFFFFFh";
				}
			}
			strcpy((char*)v5, v8);
			goto LABEL_21;
		}
		LABEL_35:
		--v6;
		v10 = v6;
	}while( v6 >= 0 );
	*v5 = 10;
	return (int)(v5 + 1);
}

// 1501E070: using guessed type char *off_1501E070[8];
//----- (15003930) --------------------------------------------------------
unsigned int __thiscall sub_15003930(char* This)
{
	char* v2; // edx@1
	v2 = This;
	strcpy(This, "ret\n");
	return (unsigned int)&v2[strlen(v2) - 1];
}

//----- (15003970) --------------------------------------------------------
void* __stdcall sub_15003970()
{
	void* result; // eax@1
	int v1;       // esi@2
	result = (void*)dword_1501E9B4;
	if( dword_1501E9B4 ){
		do{
			v1 = *(uint*)dword_1501E9B0;
			SMemFree(result, ".?AUINST@@", -2, 0);
			result = (void*)v1;
			dword_1501E9B4 = v1;
		}while( v1 );
	}
	dword_1501E9B0 = 0;
	return result;
}

// 1501E9B0: using guessed type int dword_1501E9B0;
// 1501E9B4: using guessed type int dword_1501E9B4;
//----- (150039B0) --------------------------------------------------------
int __fastcall sub_150039B0(char a1)
{
	int result; // eax@1
	int v2;     // edx@1
	v2 = dword_1501E9B0;
	for( result = 0; v2; v2 = *(uint*)(v2 + 4) ){
		if( *(uchar*)(v2 + 8) == a1 && !*(uchar*)(v2 + 10) && *(uchar*)(v2 + 11) || *(uchar*)(v2 + 12) == a1 && !*(uchar*)(v2 + 14) && *(uchar*)(v2 + 15) ){
			if( *(uint*)(v2 + 20) > result )
				result = *(uint*)(v2 + 20);
		}
	}
	return result;
}

// 1501E9B0: using guessed type int dword_1501E9B0;
//----- (15003A00) --------------------------------------------------------
int __fastcall sub_15003A00(char* a1, int a2, int a3)
{
	char* v3;            // esi@1
	int v5;              // eax@10
	int v6;              // ebx@10
	int v7;              // ebp@10
	int v8;              // edi@10
	char* v9;            // esi@11
	char v10;            // al@13
	char v11;            // cl@22
	int v12;             // esi@23
	int v13;             // eax@25
	int v14;             // ebp@32
	int v15;             // edi@32
	char v16;            // al@41
	char v17;            // al@44
	char v18;            // bl@47
	char v19;            // al@48
	char v20;            // al@49
	int v21;             // esi@49
	char v22;            // al@51
	char v23;            // al@52
	char v24;            // cl@54
	int v25;             // esi@71
	int* v26;            // eax@72
	char v27;            // al@88
	int v28;             // ecx@89
	char v29;            // al@91
	int v30;             // ecx@91
	int v31;             // eax@95
	char v32;            // zf@98
	char v33;            // bl@98
	int v34;             // ecx@100
	char v35;            // bl@107
	char* v36;           // edi@107
	char v37;            // al@109
	int v38;             // esi@109
	int v39;             // eax@114
	char v40;            // cl@114
	char v41;            // al@127
	int v42;             // ecx@127
	char v43;            // bl@135
	int v44;             // edi@135
	int v45;             // esi@135
	int v46;             // eax@137
	unsigned __int8 v47; // zf@140
	char v48;            // sf@140
	unsigned __int8 v49; // of@140
	int v50;             // ebp@142
	char v51;            // al@143
	char v52;            // dl@143
	int v53;             // ecx@143
	int v54;             // eax@160
	int* v55;            // edi@161
	int v56;             // eax@165
	char v57;            // bl@165
	int v58;             // esi@168
	char v59;            // al@172
	int v60;             // ecx@172
	unsigned __int8 v61; // zf@177
	char v62;            // sf@177
	unsigned __int8 v63; // of@177
	char v64;            // dl@181
	int v65;             // eax@184
	char v66;            // dl@185
	int v67;             // ebx@194
	int v68;             // ebp@194
	int v69;             // edi@194
	int v70;             // esi@194
	int v71;             // edx@200
	int v72;             // ebx@200
	int v73;             // esi@200
	int v74;             // ebp@205
	unsigned __int8 v75; // al@208
	int v76;             // ebx@211
	int v77;             // edi@211
	int v78;             // eax@212
	int v79;             // ecx@212
	int v80;             // edx@212
	int v81;             // ecx@218
	int v82;             // edx@226
	int v83;             // ecx@232
	unsigned __int8 v84; // dl@240
	unsigned __int8 v85; // zf@242
	char v86;            // sf@242
	unsigned __int8 v87; // of@242
	int v88;             // edx@245
	int i;               // esi@247
	int v90;             // eax@249
	int v91;             // edi@249
	int v92;             // ecx@257
	int v93;             // eax@265
	char v94;            // cl@270
	int v95;             // eax@278
	int v96;             // edx@279
	char v97;            // cl@279
	int v98;             // ebp@279
	int v99;             // edi@279
	int v100;            // esi@279
	int v101;            // ecx@283
	int v102;            // ecx@284
	int v103;            // ebx@289
	int v104;            // ecx@294
	int v105;            // edx@302
	int v106;            // ecx@303
	int v107;            // edx@306
	int v108;            // ecx@306
	char v109;           // al@307
	char v110;           // al@309
	char v111;           // al@311
	int v112;            // eax@314
	int v113;            // ecx@321
	int v114;            // ecx@323
	int v115;            // ecx@325
	int v116;            // ecx@331
	int v117;            // ecx@336
	void* v118;          // ebx@345
	void* v119;          // edi@345
	int j;               // eax@349
	int v121;            // esi@349
	int v122;            // [sp-4h] [bp-C4h]@181
	char v123;           // [sp+13h] [bp-ADh]@32
	int v124;            // [sp+14h] [bp-ACh]@32
	int v125;            // [sp+18h] [bp-A8h]@96
	int v126;            // [sp+1Ch] [bp-A4h]@47
	char* v127;          // [sp+20h] [bp-A0h]@8
	int v128;            // [sp+24h] [bp-9Ch]@32
	int v129;            // [sp+28h] [bp-98h]@8
	int v130;            // [sp+2Ch] [bp-94h]@70
	int v131;            // [sp+30h] [bp-90h]@141
	int v132;            // [sp+34h] [bp-8Ch]@32
	int v133;            // [sp+38h] [bp-88h]@45
	int v134;            // [sp+3Ch] [bp-84h]@1
	char v135;           // [sp+40h] [bp-80h]@32
	char v136;           // [sp+41h] [bp-7Fh]@32
	char v137;           // [sp+42h] [bp-7Eh]@32
	char v138;           // [sp+43h] [bp-7Dh]@32
	char v139;           // [sp+44h] [bp-7Ch]@32
	char* Str;           // [sp+48h] [bp-78h]@1
	int v141;            // [sp+4Ch] [bp-74h]@75
	int v142;            // [sp+50h] [bp-70h]@135
	int v143;            // [sp+54h] [bp-6Ch]@96
	int v144;            // [sp+58h] [bp-68h]@74
	int v145;            // [sp+5Ch] [bp-64h]@71
	int v146;            // [sp+60h] [bp-60h]@107
	int v147;            // [sp+64h] [bp-5Ch]@167
	int v148;            // [sp+68h] [bp-58h]@98
	int v149;            // [sp+6Ch] [bp-54h]@109
	int v150;            // [sp+70h] [bp-50h]@48
	char v151;           // [sp+74h] [bp-4Ch]@8
	char v152;           // [sp+94h] [bp-2Ch]@107
	int v153[10];        // [sp+98h] [bp-28h]@314
	v3 = a1;
	v134 = a2;
	Str = a1;
	if( a2 )
		*(uint *)a2 = 0;
	if( !a1 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( strlen(a1) - 1 < 3 ){
		if( a2 ){
			*(uint*)a2 = (uint)v3;
			sub_15003970();
			return 0;
		}
		goto LABEL_354;
	}
	v127 = (char*)1;
	v129 = (int)&v151;
	while( 1 ){
		v5 = 0;
		v7 = 0;
		v6 = 0;
		v8 = 0;
		if( *v3 ){
			v9 = Str + 1;
			do{
				if( *(v9 - 1) == *(uchar*)((uint)&byte_1501C170 + (uint)v127) ){
					v10 = *v9;
					if( *v9 == 61 || v10 && strchr("1234", v10) && v9[1] == 61 ){
						v5 = 1;
						v8 = 1;
					}else{
						v5 = 1;
						if( !v6 )
							v7 = 1;
					}
				}
				if( *(v9 - 1) == 32 ){
					v6 = v8;
					v8 = 0;
				}
				v11 = *v9++;
			}while( v11 );
		}
		v12 = (int)v127;
		switch( (uint)v127 ){
		case 1u:
			*(uint*)v129 = 1;
			break;
		case 2u:
		case 3u:
		case 4u:
			v13 = v5 != 0 ? 3 : 0;
			goto LABEL_30;
		case 8u:
		case 9u:
			*(uint*)v129 = 2;
			break;
		default:
			if( v5 )
				v13 = (v7 != 0) + 1;
			else
				v13 = 0;
			LABEL_30:
			*(uint*)v129 = v13;
			break;
		}
		v127 = (char*)(v12 + 1);
		v129 += 4;
		if( v12 + 1 >= 10 )
			break;
		v3 = Str;
	}
	sub_15003970();
	v14 = (int)Str;
	v132 = 4;
	v15 = v129;
	v135 = 0;
	v136 = 0;
	v137 = 0;
	v138 = 0;
	v139 = 0;
	v127 = Str;
	v128 = 0;
	v124 = 0;
	v123 = 0;
	while( *(uchar*)v14 && (v14 == (uint)Str || *(uchar*)(v14 - 1) == 32) && strchr("124", *(uchar*)v14) ){
		v132 = *(uchar*)v14 - 48;
		LABEL_185:
		v66 = *(uchar*)v14++;
		v127 = (char*)v14;
		if( !v66 ){
			if( !dword_1501E9B0 )
				return 0;
			v70 = sub_150039B0(3);
			v69 = sub_150039B0(4);
			v68 = 0;
			v67 = sub_150039B0(2);
			v125 = 0;
			if( v70 ){
				LOBYTE_IDA(v125) = 3;
				sub_15004AA0(v70, 43, v125, dword_1501E9C0, dword_1501E9C0);
			}
			if( v69 ){
				LOBYTE_IDA(v125) = 4;
				sub_15004AA0(v69, 43, v125, dword_1501E9C0, dword_1501E9C0);
			}
			if( v67 ){
				LOBYTE_IDA(v125) = 2;
				sub_15004AA0(v67, 43, v125, dword_1501E9C0, dword_1501E9C0);
			}
			v71 = dword_1501E9B0;
			v72 = 7;
			v73 = dword_1501E9B0;
			if( dword_1501E9B0 ){
				LABEL_201:
				v130 = 0;
				while( 1 ){
					v132 = v130 ? v73 + 8 : v73 + 12;
					v74 = v73 + 12;
					if( !v130 )
						v74 = v73 + 8;
					if( *(uchar*)(v132 + 1) > 2u )
						break;
					LABEL_242:
					v87 = __SETO__(v130 + 1, 1);
					v85 = v130 == 0;
					v86 = v130++ < 0;
					if( !((unsigned __int8)(v86 ^ v87) | v85) ){
						v73 = *(uint*)(v73 + 4);
						if( !v73 ){
							v71 = dword_1501E9B0;
							v68 = 0;
							v72 = 7;
							goto LABEL_247;
						}
						goto LABEL_201;
					}
				}
				v75 = *(uchar*)v132;
				if( *(uchar*)v132 != *(uchar*)(v74 + 2) && (v75 != *(uchar*)v74 || *(uchar*)(v74 + 1) > 2u) ){
					v127 = (char*)v75;
					v77 = v75;
					v76 = 0;
					while( 1 ){
						v78 = SMemAlloc(24, ".?AUINST@@", -2, 8);
						v79 = *(uint*)v78;
						*(uint*)(v78 + 8) = v77;
						v80 = dword_1501E9C0;
						*(uint*)(v78 + 16) = 7;
						*(uint*)(v78 + 12) = v80;
						*(uint*)(v78 + 20) = 4;
						if( v79 )
							goto LABEL_360;
						if( !*(uint*)(v78 + 4) )
							goto LABEL_223;
						if( v79 ){
							LABEL_360:
							*(uint*)(v79 + 4) = *(uint*)(v78 + 4);
						}else{
							if( dword_1501E9B0 == v78 )
								dword_1501E9B0 = *(uint*)(v78 + 4);
						}
						v81 = *(uint*)(v78 + 4);
						if( v81 ){
							*(uint*)v81 = *(uint*)v78;
						}else{
							if( dword_1501E9B4 == v78 )
								dword_1501E9B4 = *(uint*)v78;
						}
						*(uint*)v78 = 0;
						*(uint*)(v78 + 4) = 0;
						LABEL_223:
						if( (v76 != 0) == 1 ){
							if( v73 ){
								*(uint*)v78 = v73;
								*(uint*)(v78 + 4) = *(uint*)(v73 + 4);
								*(uint*)(v73 + 4) = v78;
								v83 = *(uint*)(v78 + 4);
								if( v83 )
									*(uint *)v83 = v78;
								else
									dword_1501E9B4 = v78;
								goto LABEL_239;
							}
							*(uint*)(v78 + 4) = dword_1501E9B0;
							if( dword_1501E9B0 )
								*(uint *)dword_1501E9B0 = v78;
							else
								dword_1501E9B4 = v78;
							goto LABEL_238;
						}
						if( (v76 != 0) == 0 ){
							if( v73 ){
								v82 = *(uint*)v73;
								*(uint*)(v78 + 4) = v73;
								*(uint*)v78 = v82;
								*(uint*)v73 = v78;
								if( !*(uint*)v78 )
									goto LABEL_238;
								*(uint*)(*(uint*)v78 + 4) = v78;
							}else{
								*(uint*)v78 = dword_1501E9B4;
								if( !dword_1501E9B0 ){
									dword_1501E9B4 = v78;
									LABEL_238:
									dword_1501E9B0 = v78;
									goto LABEL_239;
								}
								*(uint*)(dword_1501E9B4 + 4) = v78;
								dword_1501E9B4 = v78;
							}
						}
						LABEL_239:
						++v76;
						if( v76 > 1 ){
							v84 = *(uchar*)v132;
							*(uchar*)(v132 + 1) -= 2;
							if( *(uchar*)v74 == v84 )
								*(uchar *)(v74 + 1) -= 2;
							goto LABEL_242;
						}
					}
				}
				v88 = v134;
				if( !v134 )
					goto LABEL_354;
				LABEL_353:
				*(uint*)v88 = (uint)&Str[strlen(Str) - 1];
				goto LABEL_354;
			}
			LABEL_247:
			for( i = v71; i != v68; i = *(uint*)(i + 4) ){
				if( *(uint*)(i + 16) == v72 ){
					v90 = *(uint*)(i + 4);
					v91 = 0;
					while( v90 != v68 ){
						if( v91 != v68 )
							break;
						if( *(uint*)(v90 + 16) != v72 || *(uchar*)(v90 + 8) != *(uchar*)(i + 8) ){
							v94 = *(uchar*)(i + 8);
							if( *(uchar*)(v90 + 8) == v94 || *(uchar*)(v90 + 12) == v94 || *(uchar*)(v90 + 10) == v94 || *(uchar*)(v90 + 14) == v94 )
								v91 = 1;
							else
								v90 = *(uint*)(v90 + 4);
						}else{
							if( *(uint*)v90 == v68 ){
								if( v71 == v90 )
									dword_1501E9B0 = *(uint*)(v90 + 4);
							}else{
								*(uint*)(*(uint*)v90 + 4) = *(uint*)(v90 + 4);
							}
							v92 = *(uint*)(v90 + 4);
							if( v92 == v68 ){
								if( dword_1501E9B4 == v90 )
									dword_1501E9B4 = *(uint*)v90;
							}else{
								*(uint*)v92 = *(uint*)v90;
							}
							*(uint*)v90 = v68;
							*(uint*)(v90 + 4) = v68;
							SMemFree((void*)v90, ".?AUINST@@", -2, v68);
							if( *(uint*)i == v68 ){
								if( dword_1501E9B0 == i )
									dword_1501E9B0 = *(uint*)(i + 4);
							}else{
								*(uint*)(*(uint*)i + 4) = *(uint*)(i + 4);
							}
							v93 = *(uint*)(i + 4);
							if( v93 == v68 ){
								if( dword_1501E9B4 == i )
									dword_1501E9B4 = *(uint*)i;
							}else{
								*(uint*)v93 = *(uint*)i;
							}
							*(uint*)i = v68;
							*(uint*)(i + 4) = v68;
							SMemFree((void*)i, ".?AUINST@@", -2, v68);
							i = dword_1501E9B0;
							v90 = 0;
							v71 = dword_1501E9B0;
						}
					}
				}
			}
			v95 = v71;
			if( v71 != v68 ){
				while( 1 ){
					v97 = *(uchar*)(v95 + 12);
					v96 = 0;
					v99 = 0;
					v100 = 0;
					v98 = 0;
					if( v97 == 8 ){
						switch( *(uint*)(v95 + 16) ){
						case 0:
						case 1:
							v99 = 1;
							break;
						case 2:
						case 3:
						case 4:
						case 5:
							goto LABEL_288;
						default:
							break;
						}
					}else{
						if( v97 == 9 ){
							v101 = *(uint*)(v95 + 16);
							if( v101 ){
								v102 = v101 - 2;
								if( v102 ){
									if( v102 == 1 )
										v98 = 1;
								}else{
									v100 = 1;
								}
							}else{
								LABEL_288:
								v96 = 1;
							}
						}
					}
					v103 = *(uint*)(v95 + 4);
					if( v96 ){
						if( *(uint*)v95 ){
							*(uint*)(*(uint*)v95 + 4) = *(uint*)(v95 + 4);
						}else{
							if( dword_1501E9B0 == v95 )
								dword_1501E9B0 = *(uint*)(v95 + 4);
						}
						v104 = *(uint*)(v95 + 4);
						if( v104 ){
							*(uint*)v104 = *(uint*)v95;
						}else{
							if( dword_1501E9B4 == v95 )
								dword_1501E9B4 = *(uint*)v95;
						}
						*(uint*)v95 = 0;
						*(uint*)(v95 + 4) = 0;
						SMemFree((void*)v95, ".?AUINST@@", -2, 0);
						goto LABEL_305;
					}
					if( v99 || v100 )
						break;
					if( v98 ){
						*(uint*)(v95 + 16) = 6;
						v105 = dword_1501E9C0;
						LABEL_304:
						*(uint*)(v95 + 12) = v105;
					}
					LABEL_305:
					v95 = v103;
					if( !v103 )
						goto LABEL_306;
				}
				v106 = (v100 != 0) + 8;
				v127 = (char*)(unsigned __int8)v106;
				v105 = (unsigned __int8)v106;
				*(uint*)(v95 + 16) = 0;
				goto LABEL_304;
			}
			LABEL_306:
			v108 = dword_1501E9B0;
			v107 = *(uint*)(dword_1501E9B0 + 4);
			if( !v107 ){
				LABEL_340:
				if( !Source )
					Source = (LPSTR)SMemAlloc(1024, "SCODE.CPP", 851, 0);
				if( !dword_1501E9C8 )
					dword_1501E9C8 = SMemAlloc(1024, "SCODE.CPP", 853, 0);
				if( a3 ){
					v119 = sub_150037A0;
					v118 = sub_15003930;
				}else{
					v119 = sub_15003510;
					v118 = sub_15003790;
				}
				if( v119 && v118 ){
					v121 = dword_1501E9B0;
					for( j = (int)Source; v121; v121 = *(uint*)(v121 + 4) )
						j
					= ((int (__fastcall *)(int, int))v119)(j, v121);
					dword_1501E9BC = j - (uint)Source;
					dword_1501E9CC = ((int(__thiscall*)(int))v118)(dword_1501E9C8) - dword_1501E9C8;
					sub_15003970();
					return 1;
				}
				v88 = v134;
				if( !v134 )
					goto LABEL_354;
				goto LABEL_353;
			}
			while( 1 ){
				v109 = *(uchar*)(v107 + 12);
				if( (!v109 || v109 != *(uchar *)(v108 + 8))
				&& ((v110 = *(uchar *)(v107 + 14), !v110) || v110 != *(uchar *)(v108 + 8))
				&& ((v111 = *(uchar *)(v107 + 8), !v111) || v111 != *(uchar *)(v108 + 8) || !*(uint *)(v107 + 16))
				|| (memset(&v150, 0, 0x28u), memset(v153, 0, sizeof(v153)), v112 = v107, !v107) )
					goto LABEL_339;
				while( 1 ){
					if( v112 > (unsigned int)v107 ){
						if( !*(uchar*)(v112 + 12) || !*(&v150 + *(uchar*)(v112 + 12)) ){
							if( !*(uchar*)(v112 + 14) || !*(&v150 + *(uchar*)(v112 + 14)) ){
								if( !*(uchar*)(v112 + 8) )
									break;
								v113 = 4 * *(uchar*)(v112 + 8);
								if( !*(int*)((char*)&v150 + v113) ){
									if( !*(int*)((char*)v153 + v113) )
										break;
								}
							}
						}
					}
					*(&v150 + *(uchar*)(v112 + 8)) = 1;
					v153[*(uchar*)(v112 + 12)] = 1;
					v114 = *(uchar*)(v112 + 14);
					v112 = *(uint*)(v112 + 4);
					v153[v114] = 1;
					if( !v112 )
						goto LABEL_339;
				}
				v115 = *(uint*)v112;
				if( *(uint*)v112 )
					goto LABEL_328;
				if( *(uint*)(v112 + 4) ){
					if( v115 ){
						LABEL_328:
						*(uint*)(v115 + 4) = *(uint*)(v112 + 4);
					}else{
						if( dword_1501E9B0 == v112 )
							dword_1501E9B0 = *(uint*)(v112 + 4);
					}
					v116 = *(uint*)(v112 + 4);
					if( v116 ){
						*(uint*)v116 = *(uint*)v112;
					}else{
						if( dword_1501E9B4 == v112 )
							dword_1501E9B4 = *(uint*)v112;
					}
					*(uint*)v112 = 0;
					*(uint*)(v112 + 4) = 0;
				}
				v117 = *(uint*)v107;
				*(uint*)(v112 + 4) = v107;
				*(uint*)v112 = v117;
				*(uint*)v107 = v112;
				if( *(uint*)v112 )
					*(uint *)(*(uint *)v112 + 4) = v112;
				else
					dword_1501E9B0 = v112;
				LABEL_339:
				v108 = v107;
				v107 = *(uint*)(v107 + 4);
				if( !v107 )
					goto LABEL_340;
			}
		}
	}
	if( *(uchar*)v14 && strchr("&|^+-", *(uchar*)v14) ){
		v123 = *(uchar*)v14;
		goto LABEL_185;
	}
	v16 = *(uchar*)v14;
	if( *(uchar*)v14 && v16 != 32 && !strchr("WSDTABC01", v16) ){
		LABEL_187:
		if( v134 ){
			*(uint*)v134 = v14;
			sub_15003970();
			return 0;
		}
		goto LABEL_354;
	}
	v17 = *(uchar*)v14;
	if( !*(uchar *)v14 || (v133 = 0, v17 == 32) )
		v133 = 1;
	v18 = 0;
	v126 = 0;
	if( !v133 ){
		v19 = (unsigned int)strchr(&byte_1501C170, v17);
		v18 = v19 - (unsigned int)&byte_1501C170;
		LOBYTE_IDA(v126) = v19 - (unsigned int)&byte_1501C170;
		BYTE3(v126) = *(&v150 + (unsigned __int8)(v19 - (unsigned int)&byte_1501C170)) == 3;
		do{
			v20 = *(uchar*)(v14 + 1);
			v21 = 0;
			if( v20 ){
				if( strchr("WABC", v20) ){
					v22 = (unsigned int)strchr(&byte_1501C170, *(uchar*)(v14++ + 1));
					BYTE2(v126) = v22 - (unsigned int)&byte_1501C170;
					v127 = (char*)v14;
					v21 = 1;
				}
			}
			v23 = *(uchar*)(v14 + 1);
			if( v23 ){
				if( strchr("1234", v23) ){
					v21 = 1;
					v24 = *(uchar*)(v14++ + 1) - 48;
					BYTE1(v126) = v24;
					v127 = (char*)v14;
				}
			}
			if( !v21 )
				break;
			if( v18 == 8 || v18 == 9 )
				goto LABEL_187;
		}while( v21 );
	}
	if( !(uchar)v128 ){
		if( *(uchar*)(v14 + 1) == 61 ){
			if( v18 == 8 || v18 == 9 )
				goto LABEL_187;
			++v14;
			v128 = v126;
			v124 = 0;
			v123 = 0;
		}else{
			if( !v133 ){
				if( v134 ){
					*(uint*)v134 = v14 + 1;
					sub_15003970();
					return 0;
				}
				goto LABEL_354;
			}
		}
		goto LABEL_185;
	}
	if( !(uchar)v124 ){
		v123 = 0;
		v124 = v126;
		goto LABEL_185;
	}
	if( v133 && *(&v150 + (unsigned __int8)v128) != 3 ){
		LABEL_141:
		v131 = 1;
		if( v133 ){
			v50 = v128;
			v128 = 0;
			v131 = v50;
		}else{
			v51 = *(uchar*)(v14 + 1);
			v53 = v14 + 1;
			v52 = *(&byte_1501C170 + (unsigned __int8)v128);
			v143 = v14 + 1;
			if( v51 ){
				while( v51 != 32 ){
					if( v51 == v52 && *(uchar*)(v53 + 1) != 61 )
						goto LABEL_152;
					v51 = *(uchar*)(v53++ + 1);
					if( !v51 )
						break;
				}
			}
			if( *(&v150 + (unsigned __int8)v128) == 3 || BYTE2(v128) || BYTE3(v128) ){
				LABEL_152:
				if( (uchar)v124 != 1 || BYTE2(v124) || BYTE3(v124) ){
					if( v18 != 1 || BYTE2(v126) || BYTE3(v126) ){
						v50 = v131;
						v130 = 0;
						v54 = 0;
						do{
							v55 = &v126;
							if( !v54 )
								v55 = &v124;
							if( !*((uchar*)v55 + 2) ){
								if( !*((uchar*)v55 + 3) ){
									v57 = *(uchar*)v55;
									v56 = *(&v150 + *(uchar*)v55);
									if( v56 == 1 || v56 == 4 ){
										LOBYTE_IDA(v147) = *(uchar*)v55;
										if( v56 == 4 ){
											v58 = 0;
											do{
												if( *(&v135 + v58) == v57 )
													LOBYTE_IDA(v147) = (unsigned int)strchr(&byte_1501C170, byte_1501C168[v58]) - (unsigned int)&byte_1501C170;
												++v58;
											}while( v58 < 5 );
										}
										v60 = v143;
										v59 = *(uchar*)v143;
										if( *(uchar*)v143 ){
											while( v59 != *(&byte_1501C170 + (unsigned __int8)v147) || *(uchar*)(v60 + 1) == 61 ){
												v59 = *(uchar*)(v60++ + 1);
												if( !v59 )
													goto LABEL_176;
											}
										}else{
											LABEL_176:
											v50 = *v55;
											v131 = *v55;
										}
									}
								}
							}
							v54 = v130 + 1;
							v63 = __SETO__(v130 + 1, 1);
							v61 = v130 == 0;
							v62 = v130++ < 0;
						}while( (unsigned __int8)(v62 ^ v63) | v61 );
					}else{
						v50 = v126;
						v131 = v126;
					}
				}else{
					v50 = v124;
					v131 = v124;
				}
			}else{
				v50 = v128;
				v131 = v128;
			}
		}
		if( v133 ){
			if( (uchar)v131 == (uchar)v124 && BYTE1(v131) == BYTE1(v124) )
				goto LABEL_184;
			v64 = 0;
			v122 = dword_1501E9C0;
		}else{
			v64 = v123;
			v122 = v126;
		}
		sub_15004AA0(v132, v64, v50, v124, v122);
		LABEL_184:
		v15 = v129;
		v123 = 0;
		v65 = v133 == 0 ? v50 : 0;
		v14 = (int)v127;
		v124 = v65;
		goto LABEL_185;
	}
	v130 = 0;
	while( 2 ){
		v25 = 0;
		v145 = 0;
		LABEL_72:
		v26 = &v126;
		if( !v130 )
			v26 = &v124;
		v144 = (int)v26;
		switch( v25 ){
		case 0:
			v15 = 2;
			v141 = 3;
			v129 = 2;
			break;
		case 1:
			v15 = 3;
			v129 = 3;
			v141 = 3;
			break;
		case 2:
			v15 = 4;
			v141 = 3;
			v129 = 4;
			break;
		case 3:
			v15 = 8;
			goto LABEL_80;
		case 4:
			v15 = 9;
			LABEL_80:
			v129 = v15;
			v141 = 2;
			break;
		default:
			break;
		}
		if( *(uchar*)v26 != v15 || *(&v150 + v15) != v141 || v25 && v25 != 1 && v25 != 2 && v123 && strchr("&|^+-", v123) )
			goto LABEL_139;
		v27 = *(&v135 + v25);
		if( v27 ){
			v28 = v144;
			if( !*(uchar*)(v144 + 2) ){
				*(uchar*)v144 = v27;
				*(uchar*)(v28 + 3) = 0;
				v18 = v126;
				goto LABEL_139;
			}
		}
		v29 = *(uchar*)(v14 + 1);
		v30 = v14 + 1;
		if( !v29 )
			goto LABEL_95;
		while( v29 != *(&byte_1501C170 + (unsigned __int8)v15) || *(uchar*)(v30 + 1) == 61 ){
			v29 = *(uchar*)(v30++ + 1);
			if( !v29 )
				goto LABEL_95;
		}
		if( *(uchar*)(v144 + 2) )
			LABEL_95:
			v31 = 0;
		else
			v31 = 1;
		v143 = v31;
		LOBYTE_IDA(v125) = 0;
		if( v31 || (uchar)v124 == 1 ){
			v33 = 9;
			LOBYTE_IDA(v148) = 9;
			v32 = 0;
			while( 1 ){
				if( !v32 ){
					v34 = *(&v150 + (unsigned __int8)v148);
					if( !v34 )
						break;
					if( v34 == 1 && !strchr((char*)v14, *(&byte_1501C170 + (unsigned __int8)v148)) )
						break;
				}
				--v33;
				v32 = v33 == 1;
				LOBYTE_IDA(v148) = v33;
				if( (unsigned __int8)v33 < 1u )
					goto LABEL_106;
			}
			LOBYTE_IDA(v125) = v33;
			LABEL_106:
			if( !(uchar)v125 ){
				v35 = 9;
				v146 = 9;
				v36 = &v152;
				do{
					if( *(uint*)v36 == 4 ){
						v37 = 0;
						v38 = 0;
						LOBYTE_IDA(v149) = 0;
						do{
							if( *(&v135 + v38) == v35 )
								v37 = (unsigned int)strchr(&byte_1501C170, byte_1501C168[v38]) - (unsigned int)&byte_1501C170;
							++v38;
						}while( v38 < 5 );
						LOBYTE_IDA(v149) = v37;
						if( v37 ){
							v39 = v14;
							v40 = *(uchar*)v14;
							if( *(uchar*)v14 ){
								while( v40 != *(&byte_1501C170 + (unsigned __int8)v149) || *(uchar*)(v39 + 1) == 61 ){
									v40 = *(uchar*)(v39++ + 1);
									if( !v40 )
										goto LABEL_120;
								}
								LOBYTE_IDA(v125) = 0;
							}else{
								LABEL_120:
								LOBYTE_IDA(v125) = v146;
							}
						}
					}
					--v35;
					v36 -= 4;
					--v146;
				}while( (unsigned __int8)v35 >= 1u );
				break;
			}
			LABEL_135:
			v44 = v144;
			v43 = v125;
			v142 = (unsigned __int8)v125;
			v45 = (unsigned __int8)v125;
			sub_15004AA0(v132, 0, (unsigned __int8)v125, *(uint*)v144, dword_1501E9C0);
			if( v143 ){
				if( v43 != 1 ){
					v46 = (unsigned __int8)v125;
					*(&v135 + v145) = v43;
					*(&v150 + v46) = 4;
				}
			}
			*(uint*)v44 = v45;
			v18 = v126;
			v25 = v145;
			v15 = v129;
			LABEL_139:
			++v25;
			v145 = v25;
			if( v25 > 3 ){
				v49 = __SETO__(v130 + 1, 1);
				v47 = v130 == 0;
				v48 = v130++ < 0;
				if( !((unsigned __int8)(v48 ^ v49) | v47) )
					goto LABEL_141;
				continue;
			}
			goto LABEL_72;
		}
		break;
	}
	if( (uchar)v125 )
		goto LABEL_135;
	if( !*(ushort*)((char*)&v128 + 1) && !BYTE3(v128) && *(&v150 + (unsigned __int8)v128) == 1 && (uchar)v124 != (uchar)v128 ){
		v41 = *(uchar*)v14;
		v42 = v14;
		if( *(uchar*)v14 ){
			while( v41 != 32 ){
				if( v41 == *(&byte_1501C170 + (unsigned __int8)v128) && *(uchar*)(v42 + 1) != 61 )
					goto LABEL_133;
				v41 = *(uchar*)(v42++ + 1);
				if( !v41 )
					break;
			}
		}
		LOBYTE_IDA(v125) = v128;
		goto LABEL_135;
	}
	LABEL_133:
	if( (uchar)v124 != 1 ){
		LOBYTE_IDA(v125) = 1;
		goto LABEL_135;
	}
	if( v134 ){
		*(uint*)v134 = v14;
		sub_15003970();
		return 0;
	}
	LABEL_354:
	sub_15003970();
	return 0;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E9B0: using guessed type int dword_1501E9B0;
// 1501E9B4: using guessed type int dword_1501E9B4;
// 1501E9BC: using guessed type int dword_1501E9BC;
// 1501E9C0: using guessed type int dword_1501E9C0;
// 1501E9C8: using guessed type int dword_1501E9C8;
// 1501E9CC: using guessed type int dword_1501E9CC;
// 15003A00: using guessed type int var_28[10];
//----- (15004AA0) --------------------------------------------------------
unsigned int __fastcall sub_15004AA0(int a1, char a2, int a3, int a4, int a5)
{
	int v5;              // ebp@1
	char v6;             // bl@5
	char v7;             // zf@17
	int v8;              // edx@22
	int v9;              // ecx@22
	int v10;             // esi@36
	int* v11;            // eax@39
	unsigned int result; // eax@42
	char v13;            // [sp+13h] [bp-1h]@1
	v5 = a1;
	v13 = a2;
	if( BYTE1(a3) || BYTE1(a4) || BYTE1(a5) )
		v5 = 1;
	v6 = a5;
	if( v5 != 1 )
		goto LABEL_21;
	if( !BYTE1(a3) ){
		if( BYTE3(a3) ==/*__PAIR__(*/ BYTE2(a3)/*, 0)*/ ) // штука какая то
		{
			if( (uchar)a3 == (uchar)a4 && BYTE1(a4) ){
				BYTE1(a3) = BYTE1(a4);
			}else{
				if( (uchar)a3 != (uchar)a5 || (BYTE1(a3) = BYTE1(a5), !BYTE1(a5)) )
					BYTE1(a3) = 1;
			}
		}
	}
	if( !BYTE1(a4) ){
		if( BYTE3(a4) ==/*__PAIR__(*/ BYTE2(a4)/*, 0)*/ )
			BYTE1(a4) = 1;
	}
	v7 = (uchar)a5 == 0;
	if( (uchar)a5 ){
		if( !BYTE1(a5) ){
			if( BYTE3(a5) ==/*__PAIR__(*/ BYTE2(a5)/*, 0)*/ )
				BYTE1(a5) = 1;
		}
		LABEL_21:
		v7 = (uchar)a5 == 0;
	}
	v9 = a4;
	v8 = a5;
	if( !v7 && v13 && a3 == a5 && a3 != a4 || ((uchar)a4 == 8 || (uchar)a4 == 9) && (uchar)a5 != 8 && (uchar)a5 != 9 ){
		v9 = a5;
		v8 = a4;
		a4 = a5;
		a5 = v8;
		v6 = v8;
	}
	if( v6 ){
		if( v13 ){
			if( a3 != v9 ){
				if( a3 != v8 ){
					a4 = 0;
					sub_15004AA0(v5, 0, a3, v9, 0);
					a4 = a3;
				}
			}
		}
	}
	v10 = SMemAlloc(24, ".?AUINST@@", -2, 8);
	*(uint*)v10 = dword_1501E9B4;
	if( dword_1501E9B0 ){
		*(uint*)(dword_1501E9B4 + 4) = v10;
		dword_1501E9B4 = v10;
	}else{
		dword_1501E9B4 = v10;
		dword_1501E9B0 = v10;
	}
	*(uint*)(v10 + 8) = a3;
	v11 = &a5;
	if( !v6 )
		v11 = &a4;
	*(uint*)(v10 + 12) = *v11;
	if( v13 ){
		result = strchr("=&|^+-~@", v13) - "=&|^+-~@";
		*(uint*)(v10 + 16) = result;
		*(uint*)(v10 + 20) = v5;
	}else{
		result = 0;
		*(uint*)(v10 + 20) = v5;
		*(uint*)(v10 + 16) = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E9B0: using guessed type int dword_1501E9B0;
// 1501E9B4: using guessed type int dword_1501E9B4;
//----- (15004C60) --------------------------------------------------------
int __stdcall SCodeCompile(int a1, char* a2, int a3, unsigned int a4, unsigned int a5, int a6)
{
	int result;       // eax@4
	unsigned int v7;  // ebp@5
	int v8;           // ecx@7
	int v9;           // edi@7
	char v10;         // zf@9
	char v11;         // cl@11
	unsigned int v12; // ecx@11
	void* v13;        // edi@11
	int v14;          // esi@11
	char v15;         // al@11
	void* v16;        // esi@11
	int v17;          // ecx@12
	int v18;          // edi@12
	char v19;         // zf@14
	unsigned int v20; // edx@16
	int v21;          // ecx@16
	char* v22;        // edi@16
	void* v23;        // esi@16
	unsigned int v24; // ecx@16
	char v25;         // zf@18
	void* v26;        // edi@20
	int v27;          // ebx@29
	int v28;          // eax@32
	int v29;          // ebp@37
	int v30;          // eax@37
	int v31;          // eax@38
	void* v32;        // eax@39
	unsigned int v33; // ecx@40
	int v35;          // eax@45
	int v36;          // ecx@45
	int v37;          // ebp@45
	int v38;          // esi@45
	char v39;         // dl@45
	unsigned int v40; // ecx@45
	int v41;          // edx@46
	int v42;          // eax@47
	void* v43;        // eax@52
	unsigned int v44; // eax@52
	unsigned int v45; // ecx@53
	LPSTR v46;        // esi@54
	char v47;         // dl@54
	unsigned int v48; // ecx@54
	void* v49;        // edi@54
	void* v50;        // esi@54
	unsigned int v51; // edx@54
	int v52;          // eax@55
	unsigned int v53; // edx@55
	unsigned int v54; // ecx@55
	int v55;          // esi@55
	void* v56;        // edi@55
	char v57;         // al@55
	unsigned int v58; // ecx@55
	void* v59;        // edi@55
	void* v60;        // esi@55
	int v61;          // [sp+10h] [bp-20Ch]@40
	unsigned int v62; // [sp+10h] [bp-20Ch]@53
	char* v63;        // [sp+14h] [bp-208h]@5
	void* v64;        // [sp+14h] [bp-208h]@39
	unsigned int v65; // [sp+18h] [bp-204h]@5
	char Str[4];      // [sp+1Ch] [bp-200h]@5
	char v67;         // [sp+20h] [bp-1FCh]@5
	char v68;         // [sp+21h] [bp-1FBh]@5
	__int16 v69;      // [sp+119h] [bp-103h]@5
	char v70;         // [sp+11Bh] [bp-101h]@5
	char v71;         // [sp+11Ch] [bp-100h]@5
	char v72;         // [sp+11Dh] [bp-FFh]@5
	__int16 v73;      // [sp+219h] [bp-3h]@5
	char v74;         // [sp+21Bh] [bp-1h]@5
	if( a3 )
		*(uint *)a3 = 0;
	if( *a2 != 0 ? (a6 != 0 ? (a2 != 0 ? (a4 < 1) - 1 : 0) : 0) : 0 ){
		v67 = byte_1501C234;
		*(uint*)Str = dword_1501C230;
		memset(&v68, 0, 0xF8u);
		v69 = 0;
		v70 = 0;
		v71 = ZeroByte;
		memset(&v72, 0, 0xFCu);
		v73 = 0;
		v63 = Str;
		v7 = (a5 >> 18) & 1;
		v65 = (a5 >> 18) & 1;
		v74 = 0;
		if( a1 ){
			if( v7 ){
				v9 = a1;
				v8 = -1;
				do{
					if( !v8 )
						break;
					v10 = *(uchar*)v9++ == 0;
					--v8;
				}while( !v10 );
				v12 = ~v8;
				v15 = v12;
				v16 = (void*)(v9 - v12);
				v12 >>= 2;
				memcpy(Str, v16, 4 * v12);
				v14 = (int)((char*)v16 + 4 * v12);
				v13 = &Str[4 * v12];
				v11 = v15;
			}else{
				v63 = &Str[strlen(Str) - 1];
				do{
					if( !v17 )
						break;
					v19 = *(uchar*)v18++ == 0;
					--v17;
				}while( !v19 );
				v24 = ~v17;
				v23 = (void*)(v18 - v24);
				v20 = v24;
				v22 = Str;
				v21 = -1;
				do{
					if( !v21 )
						break;
					v25 = *v22++ == 0;
					--v21;
				}while( !v25 );
				v26 = v22 - 1;
				memcpy(v26, v23, 4 * (v20 >> 2));
				v14 = (int)((char*)v23 + 4 * (v20 >> 2));
				v13 = (char*)v26 + 4 * (v20 >> 2);
				v11 = v20;
			}
			memcpy(v13, (void*)v14, v11 & 3);
		}
		if( a2 )
			strcpy(&v71, a2);
		if( v7 ){
			if( Str[0] == 35 )
				Str[0] = 49;
			if( v71 == 35 )
				v71 = 52;
		}
		strupr(Str);
		strupr(&v71);
		v27 = SMemAlloc(4 * a4 + 80, ".?AUSTREAM@@", -2, 8);
		*(uint*)(v27 + 4) = dword_1501E9A8;
		if( dword_1501E9A8 )
			*(uint *)dword_1501E9A8 = v27;
		else
			dword_1501E9AC = v27;
		dword_1501E9A8 = v27;
		*(uint*)(v27 + 8) = a5;
		*(uint*)(v27 + 12) = -1;
		v28 = sub_15003A00(Str, a3, 0);
		if( a3 ){
			if( *(uint*)a3 )
				*(uint *)a3 += a1 - (uint)v63;
		}
		if( v28 ){
			v29 = v7 != 0 ? 3 : 1;
			v30 = dword_1501E9BC * v29 + dword_1501E9CC;
			*(uint*)(v27 + 24) = v30;
			*(uint*)(v27 + 20) = SMemAlloc(v30, "SCODE.CPP", 1023, 0); // звуки? 1084 уже
			if( v65 ){
				v31 = dword_1501E9CC + dword_1501E9BC * v29;
				*(uint*)(v27 + 48) = v31;
				*(uint*)(v27 + 44) = SMemAlloc(v31, "SCODE.CPP", 1026, 0);
			}
			v32 = *(void**)(v27 + 20);
			v64 = *(void**)(v27 + 44);
			if( v29 ){
				v33 = dword_1501E9BC;
				v61 = v29;
				do{
					memcpy(v32, Source, v33);
					v33 = dword_1501E9BC;
					v32 = (char*)v32 + dword_1501E9BC;
					if( v65 ){
						memcpy(v64, Source, dword_1501E9BC);
						v33 = dword_1501E9BC;
						v64 = (char*)v64 + dword_1501E9BC;
					}
				}while( v61-- != 1 );
			}
			memcpy(v32, (void*)dword_1501E9C8, dword_1501E9CC);
			if( v65 ){
				v38 = dword_1501E9C8;
				v39 = dword_1501E9CC;
				v40 = (unsigned int)dword_1501E9CC >> 2;
				memcpy(v64, (void*)dword_1501E9C8, 4 * ((unsigned int)dword_1501E9CC >> 2));
				v35 = v27 + 52;
				memcpy((char*)v64 + 4 * v40, (void*)(v38 + 4 * v40), v39 & 3);
				v36 = v29;
				v37 = v29 + 1;
				do{
					*(uint*)(v35 - 24) = *(uint*)(v27 + 20) + v36 * dword_1501E9BC;
					v35 += 4;
					v41 = *(uint*)(v27 + 44) + v36--*dword_1501E9BC;
					*(uint*)(v35 - 4) = v41;
					--v37;
				}while( v37 );
			}
			v42 = sub_15003A00(&v71, a3, 0);
			if( a3 ){
				if( *(uint*)a3 )
					*(uint *)a3 += a2 - &v71;
			}
			if( v42 ){
				v44 = dword_1501E9CC + dword_1501E9BC * a4;
				*(uint*)(v27 + 72) = v44;
				v43 = (void*)SMemAlloc(v44, "SCODE.CPP", 1060, 0);
				*(uint*)(v27 + 68) = (uint)v43;
				if( a4 ){
					v45 = dword_1501E9BC;
					v62 = a4;
					do{
						v46 = Source;
						v47 = v45;
						v48 = v45 >> 2;
						memcpy(v43, Source, 4 * v48);
						v50 = &v46[4 * v48];
						v49 = (char*)v43 + 4 * v48;
						LOBYTE_IDA(v48) = v47;
						v51 = v62;
						memcpy(v49, v50, v48 & 3);
						v45 = dword_1501E9BC;
						v43 = (char*)v43 + dword_1501E9BC;
						--v62;
					}while( v51 != 1 );
				}
				v55 = dword_1501E9C8;
				v56 = v43;
				v57 = dword_1501E9CC;
				v58 = (unsigned int)dword_1501E9CC >> 2;
				memcpy(v56, (void*)dword_1501E9C8, 4 * ((unsigned int)dword_1501E9CC >> 2));
				v60 = (void*)(v55 + 4 * v58);
				v59 = (char*)v56 + 4 * v58;
				LOBYTE_IDA(v58) = v57;
				v52 = v27 + 76;
				v53 = a4 + 1;
				memcpy(v59, v60, v58 & 3);
				v54 = a4;
				do{
					*(uint*)v52 = *(uint*)(v27 + 68) + v54 * dword_1501E9BC;
					v52 += 4;
					--v54;
					--v53;
				}while( v53 );
				*(uint*)a6 = v27;
				result = 1;
			}else{
				result = SCodeDelete((void*)v27);
			}
		}else{
			result = SCodeDelete((void*)v27);
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C230: using guessed type int dword_1501C230;
// 1501C234: using guessed type char byte_1501C234;
// 1501E9A8: using guessed type int dword_1501E9A8;
// 1501E9AC: using guessed type int dword_1501E9AC;
// 1501E9BC: using guessed type int dword_1501E9BC;
// 1501E9C8: using guessed type int dword_1501E9C8;
// 1501E9CC: using guessed type int dword_1501E9CC;
//----- (15005100) --------------------------------------------------------
int __stdcall SCodeDelete(void* a1)
{
	int result; // eax@2
	void* v2;   // eax@3
	void* v3;   // eax@5
	void* v4;   // eax@7
	int v5;     // eax@13
	if( a1 ){
		v2 = (void*)*((uint*)a1 + 5);
		if( v2 ){
			SMemFree(v2, "SCODE.CPP", 1086, 0);
			*((uint*)a1 + 5) = 0;
		}
		v3 = (void*)*((uint*)a1 + 11);
		if( v3 ){
			SMemFree(v3, "SCODE.CPP", 1090, 0);
			*((uint*)a1 + 11) = 0;
		}
		v4 = (void*)*((uint*)a1 + 17);
		if( v4 ){
			SMemFree(v4, "SCODE.CPP", 1094, 0);
			*((uint*)a1 + 17) = 0;
		}
		if( *(uint*)a1 ){
			*(uint*)(*(uint*)a1 + 4) = *((uint*)a1 + 1);
		}else{
			if( (void*)dword_1501E9A8 == a1 )
				dword_1501E9A8 = *((uint*)a1 + 1);
		}
		v5 = *((uint*)a1 + 1);
		if( v5 ){
			*(uint*)v5 = *(uint*)a1;
		}else{
			if( (void*)dword_1501E9AC == a1 )
				dword_1501E9AC = *(uint*)a1;
		}
		*(uint*)a1 = 0;
		*((uint*)a1 + 1) = 0;
		SMemFree(a1, ".?AUSTREAM@@", -2, 0);
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E9A8: using guessed type int dword_1501E9A8;
// 1501E9AC: using guessed type int dword_1501E9AC;
//----- (150051D0) --------------------------------------------------------
int __stdcall SCodeDestroy()
{
	void* i; // eax@1
	for( i = (void*)dword_1501E9A8; dword_1501E9A8; i = (void*)dword_1501E9A8 )
		SCodeDelete(i);
	if( Source )
		SMemFree(Source, "SCODE.CPP", 1115, 0);
	Source = 0;
	dword_1501E9BC = 0;
	if( dword_1501E9C8 )
		SMemFree((void*)dword_1501E9C8, "SCODE.CPP", 1118, 0);
	dword_1501E9C8 = 0;
	dword_1501E9CC = 0;
	sub_15003970();
	return 1;
}

// 1501E9A8: using guessed type int dword_1501E9A8;
// 1501E9BC: using guessed type int dword_1501E9BC;
// 1501E9C8: using guessed type int dword_1501E9C8;
// 1501E9CC: using guessed type int dword_1501E9CC;
//int __stdcall sub_150054D4(int, int);
//int __stdcall loc_150054E7(int, int);
//----- (15005250) --------------------------------------------------------
void __usercall SCodeExecute(int a1/*ebp*/, int a2, int a3)
{
	int v3; // ecx@1
	uint tmp;
	*(uint*)(a1 - 4) = *(uint*)(a1 + 8);
	*(uint*)(a1 - 8) = *(uint*)(*(uint*)(a1 + 12) + 8);
	v3 = *(uint*)(*(uint*)(a1 - 4) + 8);
	if( v3 & 0x40000 ){
		*(uint*)(a1 - 12) = *(uint*)(*(uint*)(a1 + 12) + 24) & 3 ^ 4 * *(uint*)(a1 - 8);
		if( *(uint*)(*(uint*)(a1 - 4) + 12) != *(uint*)(a1 - 12) ){
			*(uint*)(*(uint*)(a1 - 4) + 12) = *(uint*)(a1 - 12);
			*(uint*)(a1 - 24) = (4 - (*(uint*)(*(uint*)(a1 + 12) + 24) & 3)) & 3;
			if( *(uint*)(a1 - 24) > *(uint*)(a1 - 8) )
				*(uint *)(a1 - 24) = *(uint *)(a1 - 8);
			*(uint*)(a1 - 8) -= *(uint*)(a1 - 24);
			*(uint*)(a1 - 32) = *(uint*)(a1 - 8) >> 2;
			*(uint*)(a1 - 16) = *(uint*)(a1 - 8) & 3;
			__asm{
				mov tmp, offset sub_150054D4
			}
			*(uint*)(a1 - 20) = tmp;
			__asm{
				mov tmp, offset loc_150054E7
			}
			*(uint*)(a1 - 28) = tmp;
			if( *(uint*)(a1 - 24) || *(uint*)(a1 - 16) ){
				*(uint*)(*(uint*)(a1 - 4) + 16) = *(uint*)(*(uint*)(a1 - 4) + 4 * *(uint*)(a1 - 24) + 28);
				*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 20) + *(uint*)(*(uint*)(a1 - 4) + 24) - 4) = *(uint*)(*(uint*)(a1 - 4) + 4 * *(uint*)(a1 - 32) + 76) - (*(uint*)(*(uint*)(a1 - 4) + 24) + *(uint*)(*(uint*)(a1 - 4) + 20));
				*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 68) + *(uint*)(*(uint*)(a1 - 4) + 72) - 4) = *(uint*)(*(uint*)(a1 - 4) + 4 * *(uint*)(a1 - 16) + 52) - (*(uint*)(*(uint*)(a1 - 4) + 72) + *(uint*)(*(uint*)(a1 - 4) + 68));
				if( *(uint*)(*(uint*)(a1 - 4) + 8) & 0x4000000 )
					*(uint *)(a1 - 44) = *(uint *)(a1 - 28);
				else
					*(uint*)(a1 - 44) = *(uint*)(a1 - 20);
				*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 44) + *(uint*)(*(uint*)(a1 - 4) + 48) - 4) = *(uint*)(a1 - 44) - (*(uint*)(*(uint*)(a1 - 4) + 48) + *(uint*)(*(uint*)(a1 - 4) + 44));
			}else{
				*(uint*)(*(uint*)(a1 - 4) + 16) = *(uint*)(*(uint*)(a1 - 4) + 4 * *(uint*)(a1 - 32) + 76);
				if( *(uint*)(*(uint*)(a1 - 4) + 8) & 0x4000000 )
					*(uint *)(a1 - 48) = *(uint *)(a1 - 28);
				else
					*(uint*)(a1 - 48) = *(uint*)(a1 - 20);
				*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 68) + *(uint*)(*(uint*)(a1 - 4) + 72) - 4) = *(uint*)(a1 - 48) - (*(uint*)(*(uint*)(a1 - 4) + 72) + *(uint*)(*(uint*)(a1 - 4) + 68));
			}
		}
	}else{
		if( *(uint*)(*(uint*)(a1 - 4) + 12) != *(uint*)(a1 - 8) ){
			*(uint*)(*(uint*)(a1 - 4) + 12) = *(uint*)(a1 - 8);
			__asm{
				mov tmp, offset sub_150054D4
			}
			*(uint*)(a1 - 36) = tmp;
			__asm{
				mov tmp, offset loc_150054E7
			}
			*(uint*)(a1 - 40) = tmp;
			*(uint*)(*(uint*)(a1 - 4) + 16) = *(uint*)(*(uint*)(a1 - 4) + 20);
			*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 20) + *(uint*)(*(uint*)(a1 - 4) + 24) - 4) = *(uint*)(*(uint*)(a1 - 4) + 4 * *(uint*)(a1 - 8) + 76) - (*(uint*)(*(uint*)(a1 - 4) + 24) + *(uint*)(*(uint*)(a1 - 4) + 20));
			if( *(uint*)(*(uint*)(a1 - 4) + 8) & 0x4000000 )
				*(uint *)(a1 - 52) = *(uint *)(a1 - 40);
			else
				*(uint*)(a1 - 52) = *(uint*)(a1 - 36);
			*(uint*)(*(uint*)(*(uint*)(a1 - 4) + 68) + *(uint*)(*(uint*)(a1 - 4) + 72) - 4) = *(uint*)(a1 - 52) - (*(uint*)(*(uint*)(a1 - 4) + 72) + *(uint*)(*(uint*)(a1 - 4) + 68));
		}
	}
	// 	JUMPOUT(__CS__, *(_DWORD *)(*(_DWORD *)(a1 - 4) + 16));
	tmp = *(uint*)(*(uint*)(a1 - 4) + 16);
	__asm{
		mov eax, tmp
		jmp eax
	}
	//----- (150054D4) --------------------------------------------------------
	// 	sub_150054D4    proc near
	// 		arg_0           = dword ptr  4
	// 		arg_4           = dword ptr  8
	// 		arg_8           = dword ptr  0Ch
	// 		arg_C           = dword ptr  10h
	sub_150054D4:
	__asm{
		dec     dword ptr [esp+4]
                mov     eax, [esp]
                jz      short loc_15005517
                add     esi, [esp+12]
                add     edi, [esp+16]
                jmp     eax
	}
	loc_150054E7:
	__asm{
		test    dword ptr [esp+4], 1
                jnz     short loc_15005502
                dec     dword ptr [esp+4]
                mov     eax, [esp]
                add     esi, [esp+12]
                add     edi, [esp+16]
                jmp     eax
	}
	loc_15005502:
	__asm{
		dec     dword ptr [esp+4]
                jz      short loc_15005517
                mov     eax, [esp+8]
                add     esi, [eax+52]
                add     edi, [eax+48]
                mov     eax, [esp]
                jmp     eax
	}
	loc_15005517:
	__asm{
		add     esp, 20
                pop     ebp
                pop     esi
                pop     edi
                mov     eax, [ebp+12]
                mov     [eax+36], ecx
                mov     [eax+40], edx
                mov     [eax+44], ebx
                mov     eax, 1
                pop     edi
                pop     esi
                pop     ebx
                mov     esp, ebp
                pop     ebp
                retn    8
	}
}

// 150054D4: using guessed type int __stdcall sub_150054D4(int, int);
// 150054E7: using guessed type int __stdcall loc_150054E7(int, int);
//----- (15005540) --------------------------------------------------------
int __stdcall sub_15005540(int a1, int a2, int a3, int a4, int a5)
{
	int result; // eax@2
	if( a1 ){
		if( a2 )
			*(uint *)a2 = a1 + 76;
		if( a3 )
			*(uint *)a3 = *(uint *)(a1 + 20) + *(uint *)(a1 + 24) - 4;
		if( a4 )
			*(uint *)a4 = *(uint *)(a1 + 68) + *(uint *)(a1 + 72) - 4;
		if( a5 )
			*(uint *)a5 = *(uint *)(a1 + 48) + *(uint *)(a1 + 44) - 4;
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (150055B0) --------------------------------------------------------
int __stdcall SCodeGetPseudocode(char* a1, char* Dest, size_t Count)
{
	int result; // eax@2
	char Str;   // [sp+14h] [bp-100h]@3
	char v5;    // [sp+15h] [bp-FFh]@3
	__int16 v6; // [sp+111h] [bp-3h]@3
	char v7;    // [sp+113h] [bp-1h]@3
	if( *a1 != 0 ? (Count != 0 ? (Dest != 0 ? -(a1 != 0) : 0) : 0) : 0 ){
		Str = ZeroByte;
		memset(&v5, 0, 0xFCu);
		v6 = 0;
		v7 = 0;
		strcpy(&Str, a1);
		strupr(&Str);
		result = sub_15003A00(&Str, 0, 1);
		if( result ){
			Source[dword_1501E9BC] = 0;
			strncpy(Dest, Source, Count);
			result = dword_1501E9BC < Count;
		}else{
			*Dest = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E9BC: using guessed type int dword_1501E9BC;
//----- (150056C0) --------------------------------------------------------
int __stdcall sub_150056C0()
{
	int result; // eax@1
	result = 0;
	dword_1501E9D0 = 0;
	dword_1501E9D4 = 0;
	return result;
}

// 1501E9D0: using guessed type int dword_1501E9D0;
// 1501E9D4: using guessed type int dword_1501E9D4;
//----- (150056E0) --------------------------------------------------------
int __stdcall sub_150056E0()
{
	int result; // eax@1
	result = 0;
	dword_1501E9D8 = 0;
	dword_1501E9DC = 0;
	return result;
}

// 1501E9D8: using guessed type int dword_1501E9D8;
// 1501E9DC: using guessed type int dword_1501E9DC;
//----- (150056F0) --------------------------------------------------------
void* __stdcall SDlgBeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint)
{
	HWND v2;      // esi@1
	void* hdc; // eax@2
	HRGN v4;      // edi@7
	LONG v5;      // ebp@7
	HDC v6;       // [sp+14h] [bp+4h]@7
	v2 = hWnd;
	if( hWnd != 0 ? -(lpPaint != 0) : 0 ){
		hdc = (void*)IsWindow(hWnd);
		if( hdc ){
			++dword_1501E9F4;
			if( GetWindowLongA(hWnd, -20) & 0x20 || !sub_15005800(hWnd, 1) ){
				hdc = BeginPaint(hWnd, lpPaint);
			}else{
				v4 = CreateRectRgn(0, 0, 1, 1);
				GetUpdateRgn(hWnd, v4, 0);
				v5 = GetClassLongA(hWnd, -10);
				SetClassLongA(hWnd, -10, 0);
				v6 = BeginPaint(hWnd, lpPaint);
				SetClassLongA(v2, -10, v5);
				if( lpPaint->fErase ){
					SDlgDrawBitmap(v2, 1, v4, 0, 0, 0, 3);
					lpPaint->fErase = 0;
					DeleteObject(v4);
					hdc = v6;
				}else{
					DeleteObject(v4);
					hdc = v6;
				}
			}
		}
	}else{
		SErrSetLastError(0x57u);
		hdc = 0;
	}
	return hdc;
}

// 1501E9F4: using guessed type int dword_1501E9F4;
//----- (15005800) --------------------------------------------------------
int __fastcall sub_15005800(HWND hWnd, int a2)
{
	HWND v2;     // ebx@1
	int i;       // esi@1
	CHAR* v4;    // ebp@6
	__int32 v5;  // edi@8
	HWND v6;     // eax@12
	int v7;      // eax@15
	int v9;      // [sp+10h] [bp-108h]@1
	HWND v10;    // [sp+14h] [bp-104h]@6
	CHAR Str2;   // [sp+18h] [bp-100h]@6
	char v12;    // [sp+19h] [bp-FFh]@6
	char v13;    // [sp+1Ch] [bp-FCh]@7
	__int16 v14; // [sp+115h] [bp-3h]@6
	char v15;    // [sp+117h] [bp-1h]@6
	i = dword_1501E9D0;
	v9 = a2;
	v2 = hWnd;
	if( !dword_1501E9D0 )
		goto LABEL_23;
	do{
		if( *(HWND*)(i + 8) == hWnd && a2& *(uint*)(i + 16) )
			break;
		i = *(uint*)(i + 4);
	}while( i );
	if( !i ){
		LABEL_23:
		v10 = GetParent(hWnd);
		Str2 = ZeroByte;
		memset(&v12, 0, 0xFCu);
		v14 = 0;
		v15 = 0;
		GetClassNameA(v2, &Str2, 256);
		v4 = &Str2;
		if( !strnicmp(&Str2, "SDlg", 4u) )
			v4 = &v13;
		v5 = GetWindowLongA(v2, -16);
		if( GetPropA(v2, "SDlg_OrigStyle") )
			v5 = (__int32)GetPropA(v2, "SDlg_OrigStyle");
		for( i = dword_1501E9D0; i; i = *(uint*)(i + 4) ){
			if( !*(uint*)(i + 8) ){
				v6 = *(HWND*)(i + 12);
				if( !v6 || v6 == v10 ){
					if( *(uint*)(i + 16) & v9 ){
						v7 = *(uint*)(i + 20);
						if( !((unsigned __int8)~*(uchar*)(i + 20) & (uchar)v5 & 0xF) ){
							if( (v7 & 0x10000 || (v7 & v5) == v7) && !stricmp((char*)(i + 60), v4) )
								break;
						}
					}
				}
			}
		}
	}
	return i;
}

// 1501E9D0: using guessed type int dword_1501E9D0;
//----- (15005920) --------------------------------------------------------
int __stdcall SDlgBltToWindow(HWND hWnd, HRGN hrgnSrc2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, uint a10)
{
	int result;          // eax@2
	__int16 v11;         // cx@7
	int v12;             // [sp+4h] [bp-18h]@7
	char v13;            // [sp+8h] [bp-14h]@7
	struct tagRECT Rect; // [sp+Ch] [bp-10h]@7
	if( hWnd ){
		if( IsWindow(hWnd) && IsWindowVisible(hWnd) && !IsIconic(hWnd) && a8 == -1 ){
			SDrawGetScreenSize((int)&v12, (int)&v13, 0);
			GetClientRect(hWnd, &Rect);
			Rect.left += a3;
			v11 = 258;
			Rect.top += a4;
			if( !dword_1501E9E4 ){
				v11 = 4354;
				if( !dword_1501E9F4 )
					v11 = 12546;
			}
			sub_15005A30(0, (int)&Rect, (int)&v12, 0, a5, a6, a7, *(uint*)a7, 0, 0, a9, a10, hWnd, hrgnSrc2, v11, 0, dword_1501E9E4 == 0 ? (int)&dword_1501E9E4 : 0);
			result = 1;
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E9E4: using guessed type int dword_1501E9E4;
// 1501E9F4: using guessed type int dword_1501E9F4;
//----- (15005A30) --------------------------------------------------------
int __fastcall sub_15005A30(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, unsigned int a12, HWND hWnd, HRGN hrgnSrc2, __int16 a15, int a16, int a17)
{
	LONG v18;             // ebx@9
	int v19;              // ebp@9
	LONG v20;             // edi@9
	int v21;              // esi@9
	int v22;              // ecx@12
	int v23;              // eax@13
	int v24;              // ecx@19
	LONG v25;             // edx@23
	unsigned __int8 v26;  // sf@23
	unsigned __int8 v27;  // of@23
	HRGN v28;             // ebp@25
	HWND v29;             // edi@25
	HWND i;               // esi@30
	struct _RGNDATA* v31; // edi@43
	HRGN v32;             // ebx@43
	int v33;              // esi@46
	int v34;              // [sp+10h] [bp-34h]@1
	int v35;              // [sp+14h] [bp-30h]@21
	LONG v36;             // [sp+18h] [bp-2Ch]@21
	int v37;              // [sp+1Ch] [bp-28h]@46
	__int32 v38;          // [sp+20h] [bp-24h]@46
	struct tagRECT Rect;  // [sp+24h] [bp-20h]@21
	RECT rcSrc2;          // [sp+34h] [bp-10h]@23
	int v41;              // [sp+70h] [bp+2Ch]@40
	v34 = a1;
	if( a17 )
		*(uint *)a17 = 0;
	if( !(a2 != 0 ? -(hWnd != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( !(a15 & 2) ){
		if( !a1 )
			return 0;
		if( a4 <= 0 )
			return 0;
	}
	v20 = *(uint*)a2;
	v18 = *(uint*)(a2 + 4);
	v21 = *(uint*)(a2 + 8);
	v19 = *(uint*)(a2 + 12);
	if( !a3 )
		goto LABEL_50;
	if( v21 - v20 > *(uint*)a3 )
		v21 = v20 + *(uint*)a3;
	v22 = *(uint*)(a3 + 4);
	if( v19 - v18 > v22 ){
		v23 = a6;
		v19 = v22 + *(uint*)(a6 + 12);
	}else{
		LABEL_50:
		v23 = a6;
	}
	if( v23 ){
		if( !(a15 & 1) ){
			if( v21 - v20 > *(uint*)(v23 + 8) - *(uint*)v23 )
				v21 = v20 + *(uint*)(v23 + 8) - *(uint*)v23;
			v24 = *(uint*)(v23 + 12) - *(uint*)(v23 + 4);
			if( v19 - v18 > v24 )
				v19 = v18 + v24;
		}
	}
	GetClientRect(hWnd, &Rect);
	ClientToScreen(hWnd, (LPPOINT)&Rect);
	ClientToScreen(hWnd, (LPPOINT)&Rect.right);
	v35 = Rect.left;
	v36 = Rect.top;
	if( HIBYTE(a15) & 1 ){
		v20 += Rect.left;
		v18 += Rect.top;
		v21 += Rect.left;
		v19 += Rect.top;
	}
	rcSrc2.left = v20;
	rcSrc2.top = v18;
	rcSrc2.right = v21 + 1;
	rcSrc2.bottom = v19 + 1;
	IntersectRect(&Rect, &Rect, &rcSrc2);
	v25 = Rect.bottom - 1;
	v27 = __SETO__(Rect.right - 1, Rect.left);
	v26 = Rect.right-- - 1 - Rect.left < 0;
	--Rect.bottom;
	if( !(v26 ^ v27) && v25 >= Rect.top ){
		v29 = hWnd;
		GetClientRect(hWnd, &rcSrc2);
		v28 = CreateRectRgn(rcSrc2.left, rcSrc2.top, rcSrc2.right, rcSrc2.bottom);
		if( !v28 )
			return 0;
		if( hrgnSrc2 )
			CombineRgn(v28, v28, hrgnSrc2, 1);
		sub_150061B0(hWnd, v28, hWnd);
		do{
			for( i = GetWindow(v29, 3u); i; i = GetWindow(i, 3u) )
				sub_150061B0(hWnd, v28, i);
			if( GetWindowLongA(v29, -16) & 0x40000000 )
				v29 = GetParent(v29);
			else
				v29 = 0;
		}while( v29 && v29 != GetDesktopWindow() );
		if( !dword_1501E9EC || !dword_1501E9E8 || (uint)CursorCoord < 0 || (v41 = 1, HIDWORD_IDA(CursorCoord) < 0) )
			v41 = 0;
		if( v41 ){
			v31 = (struct _RGNDATA*)CreateRectRgn(CursorCoord - v35, HIDWORD_IDA(CursorCoord) - v36, CursorCoord + dword_1501E0D8 - v35, HIDWORD_IDA(CursorCoord) + dword_1501E0DC - v36);
			v32 = CreateRectRgn(0, 0, 1, 1);
			CombineRgn(v32, v28, (HRGN)v31, 4);
			CombineRgn((HRGN)v31, v28, (HRGN)v31, 1);
			DeleteObject(v28);
			v28 = v32;
		}else{
			v31 = 0;
		}
		sub_15005ED0(v34, (int)&Rect, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, (int)&v35, (struct _RGNDATA*)v28, a15, a16, a17);
		DeleteObject(v28);
		if( v41 ){
			v33 = SMemAlloc(dword_1501E0DC * dword_1501E0D8, "SDLG.CPP", 976, 0);
			rcSrc2.bottom = dword_1501E0DC - 1;
			rcSrc2.right = dword_1501E0D8 - 1;
			v37 = v35 - CursorCoord;
			v38 = v36 - HIDWORD_IDA(CursorCoord);
			rcSrc2.left = 0;
			rcSrc2.top = 0;
			sub_15005ED0(v33, (int)&rcSrc2, (int)&dword_1501E0D8, dword_1501E0D8, a5, a6, a7, a8, a9, a10, a11, a12, (int)&v37, v31, a15 & 1, a16, 0);
			SBltROP3(v33, dword_1501E9EC, dword_1501E0D8, dword_1501E0DC, dword_1501E0D8, dword_1501E0D8, 0, 0x8800C6u);
			SBltROP3(v33, dword_1501E9E8, dword_1501E0D8, dword_1501E0DC, dword_1501E0D8, dword_1501E0D8, 0, 0xEE0086u);
			*(_QWORD*)&Rect.left = CursorCoord;
			sub_15005ED0(v34, (int)&Rect, a3, a4, v33, (int)&rcSrc2, (int)&dword_1501E0D8, dword_1501E0D8, 0, 0, 0, 0xCC0020u, (int)&v35, v31, a15 & 0xFE, a16, a17);
			DeleteObject(v31);
			SMemFree((void*)v33, "SDLG.CPP", 1041, 0);
		}
	}
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E0D0: using guessed type __int64 qword_1501E0D0;
// 1501E0DC: using guessed type int dword_1501E0DC;
// 1501E9E8: using guessed type int dword_1501E9E8;
// 1501E9EC: using guessed type int dword_1501E9EC;
//----- (15005ED0) --------------------------------------------------------
int __fastcall sub_15005ED0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, unsigned int a12, int a13, struct _RGNDATA* hrgn, __int16 a15, int a16, int a17)
{
	int v17;               // ebp@1
	void* v18;             // edi@1
	int v19;               // esi@1
	struct _RGNDATA* v20;  // ebx@1
	DWORD v21;             // edi@1
	char* v22;             // eax@4
	int v23;               // ebx@4
	int v24;               // edx@5
	int v25;               // ecx@5
	int v26;               // edx@7
	int v27;               // edx@9
	int v28;               // ecx@11
	int v29;               // ebx@15
	unsigned int v30;      // ecx@16
	char* v31;             // eax@17
	__int32 v32;           // esi@29
	int v33;               // edx@31
	int v34;               // eax@34
	unsigned int v35;      // ebx@34
	int v36;               // ebp@34
	int v37;               // edi@34
	int v38;               // esi@34
	int v39;               // eax@38
	int v40;               // ecx@38
	int v41;               // ebp@38
	int v42;               // eax@38
	char v43;              // zf@39
	int v45;               // [sp-8h] [bp-48h]@48
	int v46;               // [sp-4h] [bp-44h]@48
	int v47;               // [sp+10h] [bp-30h]@1
	DWORD v48;             // [sp+14h] [bp-2Ch]@1
	int v49;               // [sp+18h] [bp-28h]@34
	int v50;               // [sp+1Ch] [bp-24h]@15
	int i;                 // [sp+20h] [bp-20h]@34
	int v52;               // [sp+24h] [bp-1Ch]@1
	struct tagPOINT Point; // [sp+28h] [bp-18h]@16
	int v54;               // [sp+30h] [bp-10h]@38
	int v55;               // [sp+34h] [bp-Ch]@38
	int v56;               // [sp+38h] [bp-8h]@38
	int v57;               // [sp+3Ch] [bp-4h]@38
	v20 = hrgn;
	v19 = a2;
	v47 = a1;
	v52 = a2;
	v21 = GetRegionData((HRGN)hrgn, 0, 0);
	hrgn = (struct _RGNDATA*)SMemAlloc(v21, "SDLG.CPP", 716, 0);
	GetRegionData((HRGN)v20, v21, (LPRGNDATA)hrgn);
	v18 = hrgn;
	v48 = hrgn->rdh.nCount;
	v17 = v48;
	if( v48 && hrgn != (struct _RGNDATA*)-32 ){
		if( v48 ){
			v22 = (char*)&hrgn[1].rdh.iType;
			v23 = v48;
			do{
				v25 = a13;
				v24 = *((uint*)v22 - 2) + *(uint*)a13;
				if( *(uint*)v19 > v24 )
					v24 = *(uint*)v19;
				*((uint*)v22 - 2) = v24;
				v26 = *(uint*)(v25 + 4) + *((uint*)v22 - 1);
				if( *(uint*)(v19 + 4) > v26 )
					v26 = *(uint*)(v19 + 4);
				*((uint*)v22 - 1) = v26;
				v27 = *(uint*)v25 + *(uint*)v22 - 1;
				if( *(uint*)(v19 + 8) < v27 )
					v27 = *(uint*)(v19 + 8);
				*(uint*)v22 = v27;
				v28 = *((uint*)v22 + 1) + *(uint*)(v25 + 4) - 1;
				if( *(uint*)(v19 + 12) < v28 )
					v28 = *(uint*)(v19 + 12);
				*((uint*)v22 + 1) = v28;
				v22 += 16;
				--v23;
			}while( v23 );
			v18 = hrgn;
		}
		v29 = 0;
		v50 = 0;
		if( !(HIBYTE(a15) & 0x10) )
			goto LABEL_29;
		GetCursorPos(&Point);
		v30 = 0;
		if( !v17 )
			goto LABEL_28;
		v31 = (char*)&hrgn[1].rdh.iType;
		while( 1 ){
			if( Point.x + 32 >= *((uint*)v31 - 2) ){
				if( Point.y + 32 >= *((uint*)v31 - 1) && Point.x - 32 <= *(uint*)v31 && Point.y - 32 <= *((uint*)v31 + 1) ){
					do
					++v29;
					while( ShowCursor(0) >= 0 );
					v17 = v48;
					v50 = v29;
					LABEL_27:
					v18 = hrgn;
					LABEL_28:
					GdiFlush();
					LABEL_29:
					v32 = a15 & 2;
					Point.x = a15 & 2;
					if( a15 & 2 )
						SDrawLockSurface(a16, 0, (int)&v47, (int)&a4, 0);
					v33 = v47;
					if( v47 ){
						if( a4 ){
							if( v17 ){
								v35 = a12;
								v34 = a15 & 1;
								v38 = (int)((char*)v18 + 32);
								v37 = a6;
								v49 = v17;
								v36 = a11;
								for( i = a15 & 1; ; v34 = i ){
									if( v34 ){
										SBltROP3Tiled(v33, v38, a4, a5, v37, a8, *(uint*)v38 + a9 - *(uint*)a13, a10 + *(uint*)(v38 + 4) - *(uint*)(a13 + 4), v36, v35);
									}else{
										v56 = *(uint*)(v37 + 8);
										v39 = *(uint*)v37;
										v57 = *(uint*)(v37 + 12);
										v40 = *(uint*)(v37 + 4);
										v41 = *(uint*)(v52 + 4);
										v54 = *(uint*)v38 + a9 - *(uint*)v52 + v39;
										v35 = a12;
										v42 = a10 + *(uint*)(v38 + 4) - v41;
										v36 = a11;
										v55 = v42 + v40;
										SBltROP3Clipped(v33, v38, 0, a4, a5, (int)&v54, a7, a8, a11, a12);
									}
									v33 = v47;
									v38 += 16;
									v43 = v49-- == 1;
									if( v43 )
										break;
								}
								v32 = Point.x;
								v29 = v50;
								v17 = v48;
								v18 = hrgn;
							}
						}
					}
					if( v32 )
						SDrawUnlockSurface((int)v18, a16, v33, v17, (LPRECT)((char*)v18 + 32));
					if( HIBYTE(a15) & 0x20 ){
						for( ; v29; --v29 )
							ShowCursor(1);
					}
					if( a17 )
						*(uint *)a17 += v29;
					v46 = 0;
					v45 = 813;
					LABEL_51:
					SMemFree(v18, "SDLG.CPP", v45, v46);
					return 1;
				}
				v17 = v48;
			}
			++v30;
			v31 += 16;
			if( v30 >= v17 )
				goto LABEL_27;
		}
	}
	if( hrgn ){
		v46 = 0;
		v45 = 725;
		goto LABEL_51;
	}
	return 1;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150061B0) --------------------------------------------------------
void __fastcall sub_150061B0(HWND a1, HRGN a2, HWND hWnd)
{
	HRGN v3;             // ebx@1
	HWND v4;             // edi@1
	char v5;             // al@4
	HRGN v6;             // ebp@7
	HWND i;              // esi@8
	struct tagRECT Rect; // [sp+10h] [bp-10h]@6
	v3 = a2;
	v4 = a1;
	if( a1 != 0 ? (hWnd != 0 ? -(a2 != 0) : 0) : 0 ){
		if( IsWindowVisible(hWnd) ){
			v5 = GetWindowLongA(hWnd, -20);
			if( hWnd != v4 ){
				if( !(v5 & 0x20) ){
					GetWindowRect(hWnd, &Rect);
					ScreenToClient(v4, (LPPOINT)&Rect);
					ScreenToClient(v4, (LPPOINT)&Rect.right);
					if( RectInRegion(v3, &Rect) ){
						v6 = CreateRectRgn(Rect.left, Rect.top, Rect.right, Rect.bottom);
						CombineRgn(v3, v3, v6, 4);
						DeleteObject(v6);
					}
				}
			}
			for( i = GetTopWindow(hWnd); i; i = GetWindow(i, 2u) )
				sub_150061B0(v4, v3, i);
		}
	}else{
		SErrSetLastError(0x57u);
	}
}

//----- (150062A0) --------------------------------------------------------
int __stdcall SDlgCheckTimers()
{
	DWORD v0;        // edi@1
	int v1;          // esi@1
	int v2;          // eax@2
	unsigned int v3; // ecx@2
	DWORD v4;        // eax@10
	v1 = dword_1501E9D8;
	v0 = GetTickCount();
	if( dword_1501E9D8 ){
		while( 1 ){
			v2 = v0 - *(uint*)(v1 + 24);
			v3 = *(uint*)(v1 + 16);
			if( v2 >= v3 ){
				if( v2 <= 2 * v3 )
					*(uint *)(v1 + 24) += *(uint *)(v1 + 16);
				else
					*(uint*)(v1 + 24) = v0;
				if( *(uint*)(v1 + 20) ){
					LABEL_9:
					if( v1 ){
						v4 = GetTickCount();
						(*(void (__stdcall **)(uint, uint, uint, uint))(v1 + 20))(
							*(uint *)(v1 + 8),
							275,
							*(uint *)(v1 + 12),
							v4);
					}
					return 1;
				}
				PostMessageA(*(HWND*)(v1 + 8), 0x113u, *(uint*)(v1 + 12), 0);
			}
			v1 = *(uint*)(v1 + 4);
			if( !v1 )
				goto LABEL_9;
		}
	}
	return 1;
}

// 1501E9D8: using guessed type int dword_1501E9D8;
//----- (15006330) --------------------------------------------------------
void* __stdcall SDlgCreateDialog(HINSTANCE a1, int a2, HWND hWndParent, LPVOID lpParam, LPARAM lParam)
{
	void* result; // eax@2
	if( dword_1501E9F0 || (result = (void*)sub_15006370()) != 0 )
		result = S_CreateDialogWindow(a1, a2, hWndParent, lpParam, lParam);
	return result;
}

// 1501E9F0: using guessed type int dword_1501E9F0;
//----- (15006370) --------------------------------------------------------
int __stdcall sub_15006370()
{
	HCURSOR v0;         // eax@1
	int result;         // eax@2
	HMODULE v2;         // eax@3
	HBRUSH v3;          // eax@3
	WNDCLASSA WndClass; // [sp+10h] [bp-28h]@1
	memset(&WndClass, 0, sizeof(WndClass));
	dword_1501E9F0 = 1;
	WndClass.style = 8;
	WndClass.lpfnWndProc = (WNDPROC)sub_15006730;
	WndClass.cbWndExtra = 30;
	WndClass.hInstance = GetModuleHandleA(0);
	v0 = LoadCursorA(0, (LPCSTR)32512);
	WndClass.lpszClassName = "SDlgDialog";
	WndClass.hCursor = v0;
	if( RegisterClassA(&WndClass) ){
		memset(&WndClass, 0, sizeof(WndClass));
		v2 = GetModuleHandleA(0);
		GetClassInfoA(v2, "Static", &WndClass);
		WndClass.lpfnWndProc = (WNDPROC)sub_15006460;
		WndClass.hInstance = GetModuleHandleA(0);
		WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
		v3 = (HBRUSH)GetStockObject(1);
		WndClass.lpszClassName = "SDlgStatic";
		WndClass.hbrBackground = v3;
		result = RegisterClassA(&WndClass) != 0;
	}else{
		result = 0;
	}
	return result;
}

// 1501E9F0: using guessed type int dword_1501E9F0;
//----- (15006460) --------------------------------------------------------
LRESULT __stdcall sub_15006460(LPARAM lParam, UINT Msg, WPARAM wParam, LPARAM a4)
{
	WPARAM v5;           // ebx@7
	HANDLE v6;           // eax@7
	int v7;              // eax@7
	int v8;              // ST0C_4@7
	HWND v9;             // eax@7
	char v10;            // al@8
	int v11;             // ebp@8
	CHAR* v12;           // edi@8
	HWND v13;            // eax@12
	UINT v14;            // [sp+10h] [bp-84h]@8
	struct tagRECT rc;   // [sp+14h] [bp-80h]@12
	int v16;             // [sp+24h] [bp-70h]@7
	int v17;             // [sp+28h] [bp-6Ch]@7
	int v18;             // [sp+30h] [bp-64h]@7
	LPARAM v19;          // [sp+38h] [bp-5Ch]@7
	WPARAM v20;          // [sp+3Ch] [bp-58h]@7
	struct tagRECT Rect; // [sp+40h] [bp-54h]@7
	PAINTSTRUCT Paint;   // [sp+54h] [bp-40h]@7
	switch( Msg ){
	case 2u:
		RemovePropA((HWND)lParam, "SDlg_Font");
		return DefWindowProcA((HWND)lParam, Msg, wParam, a4);
	case 0x14u:
		if( GetWindowLongA((HWND)lParam, -20) & 0x20 )
			return 0;
		if( !sub_15005800((HWND)lParam, 65535) )
			return DefWindowProcA((HWND)lParam, Msg, wParam, a4);
		return 0;
	case 0x31u:
		return (LRESULT)GetPropA((HWND)lParam, "SDlg_Font");
	case 0xFu:
		v5 = (WPARAM)SDlgBeginPaint((HWND)lParam, &Paint);
		v6 = GetPropA((HWND)lParam, "SDlg_Font");
		SelectObject((HDC)v5, v6);
		SetBkMode((HDC)v5, 1);
		memset(&v16, 0, 0x30u);
		v16 = 5;
		v7 = GetDlgCtrlID((HWND)lParam);
		v18 = 1;
		v17 = v7;
		v19 = lParam;
		v20 = v5;
		GetClientRect((HWND)lParam, &Rect);
		v8 = GetDlgCtrlID((HWND)lParam);
		v9 = GetParent((HWND)lParam);
		if( !sub_15007F10(v9, 0x2Bu, v8, (LPARAM)&v16) ){
			v11 = GetWindowTextLengthA((HWND)lParam);
			v12 = (CHAR*)SMemAlloc(v11 + 1, "SDLG.CPP", 287, 0);
			GetWindowTextA((HWND)lParam, v12, v11 + 1);
			v14 = 80;
			v10 = GetWindowLongA((HWND)lParam, -16);
			if( v10 & 1 ){
				v14 = 81;
			}else{
				if( v10 & 2 )
					v14 = 82;
			}
			GetClientRect((HWND)lParam, &rc);
			++rc.left;
			++rc.top;
			SetTextColor((HDC)v5, 0);
			DrawTextA((HDC)v5, v12, v11, &rc, v14);
			GetClientRect((HWND)lParam, &rc);
			--rc.right;
			--rc.bottom;
			SetTextColor((HDC)v5, 0xFFFFFFu);
			v13 = GetParent((HWND)lParam);
			SendMessageA(v13, 0x138u, v5, lParam);
			DrawTextA((HDC)v5, v12, v11, &rc, v14);
			SMemFree(v12, "SDLG.CPP", 323, 0);
		}
		SDlgEndPaint((HWND)lParam, &Paint);
		return 0;
	case 0x30u:
		SetPropA((HWND)lParam, "SDlg_Font", (HANDLE)wParam);
		if( a4 & 1 )
			goto LABEL_16;
		break;
	case 0xCu:
		LABEL_16:
		InvalidateRect((HWND)lParam, 0, 1);
		break;
	default:
		return DefWindowProcA((HWND)lParam, Msg, wParam, a4);
	}
	return DefWindowProcA((HWND)lParam, Msg, wParam, a4);
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15006730) --------------------------------------------------------
int __stdcall sub_15006730(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HCURSOR v4;        // eax@2
	HDC v6;            // ebp@10
	HGDIOBJ v7;        // eax@10
	LRESULT v8;        // eax@10
	HWND v9;           // eax@12
	HWND v10;          // eax@13
	HWND v11;          // edi@13
	HWND v12;          // eax@21
	LRESULT v13;       // eax@23
	LONG v14;          // eax@23
	LRESULT v15;       // ebp@25
	PAINTSTRUCT Paint; // [sp+10h] [bp-40h]@11
	switch( Msg ){
	case WM_CREATE:
		v4 = LoadCursorA(0, (LPCSTR)IDC_ARROW);
		SetCursor(v4);
		break;
	case WM_DESTROY:
		RemovePropA(hWnd, "SDlg_Font");
		sub_150069F0(hWnd);
		break;
	case WM_DRAWITEM:
		if( *(uint*)lParam != 4 )
			break;
		return sub_15006AA0(lParam);
	case WM_ERASEBKGND:
		if( GetWindowLongA(hWnd, -20) & 0x20 )
			return 0;
		if( !sub_15005800(hWnd, 65535) )
			break;
		return 0;
	case WM_GETFONT:
		return (int)GetPropA(hWnd, "SDlg_Font");
	case WM_NCDESTROY:
		v6 = GetDC(hWnd);
		v7 = GetStockObject(13);
		SelectObject(v6, v7);
		ReleaseDC(hWnd, v6);
		v8 = SendMessageA(hWnd, 0x31u, 0, 0);
		DeleteObject((HGDIOBJ)v8);
		break;
	case WM_PAINT:
		SDlgBeginPaint(hWnd, &Paint);
		SDlgEndPaint(hWnd, &Paint);
		if( GetPropA(hWnd, "SDlg_Modal") ){
			v9 = GetParent(hWnd);
			if( IsWindowEnabled(v9) ){
				v10 = GetParent(hWnd);
				EnableWindow(v10, 0);
				v11 = GetParent(hWnd);
				if( GetActiveWindow() == v11 )
					SetActiveWindow(hWnd);
			}
		}
		return 0;
	case WM_SETFONT:
		SetPropA(hWnd, "SDlg_Font", (HANDLE)wParam);
		break;
	case WM_SHOWWINDOW:
		if( !wParam ){
			if( !lParam || lParam == 1 ){
				if( GetPropA(hWnd, "SDlg_Modal") ){
					v12 = GetParent(hWnd);
					EnableWindow(v12, 1);
				}
			}
		}
		break;
	default:
		break;
	}
	if( dword_1501E9F8 ){
		dword_1501E9F8 = 0;
		v14 = GetWindowLongA(hWnd, 4);
		v13 = CallWindowProcA((WNDPROC)v14, hWnd, Msg, wParam, lParam);
	}else{
		v13 = DefDlgProcA(hWnd, Msg, wParam, lParam);
	}
	v15 = v13;
	if( Msg == WM_NCCREATE )
		SetWindowLongA(hWnd, 4, *(uint*)lParam);
	return v15;
}

// 1501E9F8: using guessed type int dword_1501E9F8;
//----- (150069F0) --------------------------------------------------------
HWND __thiscall sub_150069F0(void* This)
{
	int v1;      // eax@1
	HWND v2;     // ebp@1
	int v3;      // edi@1
	int v4;      // esi@5
	int v5;      // ecx@9
	HWND result; // eax@15
	HWND i;      // esi@15
	v3 = dword_1501E9D0;
	v2 = (HWND)This;
	v1 = dword_1501E9D0;
	while( v1 ){
		if( *(HWND*)(v1 + 8) == v2 || *(HWND*)(v1 + 12) == v2 ){
			v4 = *(uint*)(v1 + 4);
			if( *(uint*)v1 ){
				*(uint*)(*(uint*)v1 + 4) = *(uint*)(v1 + 4);
			}else{
				if( v3 == v1 )
					dword_1501E9D0 = *(uint*)(v1 + 4);
			}
			v5 = *(uint*)(v1 + 4);
			if( v5 ){
				*(uint*)v5 = *(uint*)v1;
			}else{
				if( dword_1501E9D4 == v1 )
					dword_1501E9D4 = *(uint*)v1;
			}
			*(uint*)v1 = 0;
			*(uint*)(v1 + 4) = 0;
			SMemFree((void*)v1, ".?AUBITMAPREC@@", -2, 0);
			v3 = dword_1501E9D0;
			v1 = v4;
		}else{
			v1 = *(uint*)(v1 + 4);
		}
	}
	result = GetTopWindow(v2);
	for( i = result; result; i = result ){
		sub_150069F0(i);
		result = GetWindow(i, 2u);
	}
	return result;
}

// 1501E9D0: using guessed type int dword_1501E9D0;
// 1501E9D4: using guessed type int dword_1501E9D4;
//----- (15006AA0) --------------------------------------------------------
int __thiscall sub_15006AA0(int This)
{
	HDC v1;               // edi@1
	int v2;               // esi@1
	char v3;              // bl@3
	int v4;               // ebp@3
	int v5;               // ebx@5
	int v6;               // eax@8
	int v7;               // ebx@14
	CHAR* v8;             // ebx@23
	HGDIOBJ v9;           // eax@23
	int v11;              // [sp+10h] [bp-48h]@8
	int v12;              // [sp+14h] [bp-44h]@1
	int v13;              // [sp+18h] [bp-40h]@23
	LONG v14;             // [sp+1Ch] [bp-3Ch]@24
	struct tagSIZE psizl; // [sp+20h] [bp-38h]@19
	struct tagRECT rc;    // [sp+28h] [bp-30h]@23
	struct tagRECT Rect;  // [sp+38h] [bp-20h]@1
	int v18;              // [sp+48h] [bp-10h]@10
	int v19;              // [sp+4Ch] [bp-Ch]@10
	int v20;              // [sp+50h] [bp-8h]@10
	int v21;              // [sp+54h] [bp-4h]@10
	v2 = This;
	GetClientRect(*(HWND*)(This + 20), &Rect);
	v1 = GetDC(*(HWND*)(v2 + 20));
	v12 = *(uint*)(v2 + 16) & 1;
	if( SendMessageA(*(HWND*)(v2 + 20), 0xF2u, 0, 0) & 1 )
		v12 = 1;
	v4 = (*(uint*)(v2 + 16) & 6) != 0;
	v3 = GetWindowLongA(*(HWND*)(v2 + 20), -16);
	if( GetPropA(*(HWND*)(v2 + 20), "SDlg_OrigStyle") )
		v3 = (unsigned int)GetPropA(*(HWND*)(v2 + 20), "SDlg_OrigStyle");
	v5 = v3 & 0xF;
	if( v5 && v5 != 1 && v5 != 11 ){
		v6 = 0;
		v11 = 0;
	}else{
		v11 = 1;
		v6 = 0;
	}
	v18 = v6;
	v19 = v6;
	v20 = v6;
	v21 = v6;
	if( v11 == v6 ){
		v18 = 1;
		v21 = -1;
	}
	if( v12 == v6 ){
		if( v4 == v6 )
			v7 = (*(uint*)(v2 + 16) & 0x10) != 0 ? 32 : 16;
		else
			v7 = 256;
	}else{
		if( v4 == v6 )
			v7 = (*(uint*)(v2 + 16) & 0x10) != 0 ? 128 : 64;
		else
			v7 = 1024;
	}
	psizl.cx = sub_15005800(*(HWND*)(v2 + 20), v7);
	if( psizl.cx ){
		SDlgDrawBitmap(*(HWND*)(v2 + 20), v7, 0, 0, 0, (int)&v18, v11 != 0 ? 3 : 0);
	}else{
		SDlgDrawBitmap(*(HWND*)(v2 + 20), 1, 0, 1 - v12, 1 - v12, (int)&v18, v11 != 0 ? 3 : 0);
		if( v11 )
			DrawEdge(v1, &Rect, v12 != 0 ? 10 : 1, 0xFu);
	}
	v13 = GetWindowTextLengthA(*(HWND*)(v2 + 20));
	v8 = (CHAR*)SMemAlloc(v13 + 1, "SDLG.CPP", 542, 0);
	GetWindowTextA(*(HWND*)(v2 + 20), v8, v13 + 1);
	v9 = GetCurrentObject(*(HDC*)(v2 + 24), 6u);
	SelectObject(v1, v9);
	SetTextAlign(v1, 0);
	SetBkMode(v1, 1);
	rc.top = Rect.top + 3;
	rc.left = Rect.left + 3;
	rc.bottom = Rect.bottom - 1;
	rc.right = Rect.right - 1;
	if( v11 ){
		v14 = GetWindowLongA(*(HWND*)(v2 + 20), -16);
		psizl.cx = 16;
		psizl.cy = 16;
		GetTextExtentPoint32A(v1, "~,_Oy", 5, &psizl);
		if( BYTE1(v14) & 8 ){
			rc.top = rc.top + rc.bottom - psizl.cy - 6;
		}else{
			if( BYTE1(v14) & 0xC || !(BYTE1(v14) & 4) )
				rc.top += (rc.bottom - rc.top - psizl.cy) / 2 + 1;
		}
		if( v12 ){
			++rc.left;
			++rc.top;
			++rc.right;
			++rc.bottom;
		}
	}else{
		if( psizl.cx ){
			--rc.top;
			rc.left = Rect.left + 3 + *(uint*)(psizl.cx + 36) - *(uint*)(psizl.cx + 28) + 6;
		}
	}
	SetTextColor(v1, 0);
	psizl.cx = v11 != 0;
	DrawTextA(v1, v8, v13, &rc, v11 != 0);
	--rc.left;
	--rc.top;
	--rc.right;
	--rc.bottom;
	SetTextColor(v1, v4 != 0 ? 8421504 : 16777215);
	DrawTextA(v1, v8, v13, &rc, psizl.cx);
	SMemFree(v8, "SDLG.CPP", 589, 0);
	if( v11 ){
		if( *(uchar*)(v2 + 16) & 0x10 ){
			if( !sub_15005800(*(HWND*)(v2 + 20), 160) ){
				rc.left = v12 + Rect.left + 4;
				rc.top = v12 + Rect.top + 4;
				rc.right = v12 + Rect.right - 3;
				rc.bottom = v12 + Rect.bottom - 3;
				DrawFocusRect(v1, &rc);
			}
		}
	}
	ReleaseDC(*(HWND*)(v2 + 20), v1);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15006EA0) --------------------------------------------------------
HWND __fastcall S_CreateDialogWindow(HINSTANCE exeHandle, int dialogHandle, HWND hWndParent, LPVOID lpParam, LPARAM lParam)
{
	HWND parentHwnd;                  // eax@1
	int v6;                   // esi@1
	int v7;                   // eax@3
	HFONT v8;                 // esi@14
	int v9;                   // eax@16
	int v10;                  // eax@18
	int v11;                  // edi@20
	HDC v12;                  // ebp@20
	HDC v13;                  // ebx@20
	unsigned int v14;         // eax@20
	int v15;                  // edx@22
	int v16;                  // eax@24
	int v17;                  // ebp@25
	int width;                  // ebx@27
	int height;                  // ebp@27
	int dwStyle;                  // edi@27
	unsigned int style;         // esi@27
	int x_1;                  // eax@28
	int x_;                  // ecx@33
	int y_;                  // eax@39
	int v27;                  // ebx@52
	HDC v28;                  // edi@52
	int v29;                  // esi@57
	int v30;                  // ebp@60
	DWORD v31;                // ebp@77
	DWORD v32;                // edi@80
	HWND v33;                 // esi@82
	__int64 v34;              // qax@82
	int v35;                  // ST2C_4@82
	__int64 v36;              // qax@82
	int v37;                  // ST28_4@82
	__int64 v38;              // qax@82
	int v39;                  // ST24_4@82
	__int64 v40;              // qax@82
	HWND v41;                 // eax@82
	LONG v42;                 // eax@82
	unsigned __int8 v43;      // sf@91
	unsigned __int8 v44;      // of@91
	HWND i;                   // ebx@97
	int v46;                  // ebp@99
	LONG v47;                 // esi@99
	int v48;                  // [sp-3Ch] [bp-4ECh]@16
	int v49;                  // [sp-3Ch] [bp-4ECh]@22
	int v50;                  // [sp-38h] [bp-4E8h]@16
	int v51;                  // [sp-38h] [bp-4E8h]@22
	int v52;                  // [sp-34h] [bp-4E4h]@16
	int v53;                  // [sp-34h] [bp-4E4h]@22
	int v54;                  // [sp-30h] [bp-4E0h]@16
	int v55;                  // [sp-30h] [bp-4E0h]@22
	int v56;                  // [sp-2Ch] [bp-4DCh]@16
	int v57;                  // [sp-2Ch] [bp-4DCh]@22
	int v58;                  // [sp-28h] [bp-4D8h]@16
	int v59;                  // [sp-28h] [bp-4D8h]@22
	DWORD v60;                // [sp-24h] [bp-4D4h]@16
	DWORD v61;                // [sp-24h] [bp-4D4h]@22
	DWORD v62;                // [sp-20h] [bp-4D0h]@16
	DWORD v63;                // [sp-1Ch] [bp-4CCh]@16
	DWORD v64;                // [sp-18h] [bp-4C8h]@16
	DWORD v65;                // [sp-14h] [bp-4C4h]@16
	DWORD v66;                // [sp-10h] [bp-4C0h]@16
	DWORD v67;                // [sp-Ch] [bp-4BCh]@16
	DWORD v68;                // [sp-8h] [bp-4B8h]@16
	CHAR* v69;                // [sp-4h] [bp-4B4h]@16
	int y;                  // [sp+10h] [bp-4A0h]@31
	WPARAM v71;               // [sp+10h] [bp-4A0h]@52
	int x;                  // [sp+14h] [bp-49Ch]@28
	HWND desktopWindow;                 // [sp+14h] [bp-49Ch]@50
	__int16 v74;              // [sp+18h] [bp-498h]@4
	__int16 v75;              // [sp+1Ah] [bp-496h]@4
	WPARAM wParam;            // [sp+1Ch] [bp-494h]@14
	HWND v77;                 // [sp+20h] [bp-490h]@1
	__int16 v78;              // [sp+26h] [bp-48Ah]@4
	int v79;                  // [sp+28h] [bp-488h]@3
	int v80;                  // [sp+2Ch] [bp-484h]@25
	char* hGdi;               // [sp+30h] [bp-480h]@20
	int v82;                  // [sp+34h] [bp-47Ch]@25
	int xRight;                  // [sp+38h] [bp-478h]@27
	int v84;                  // [sp+3Ch] [bp-474h]@4
	HINSTANCE hInstance;      // [sp+40h] [bp-470h]@1
	int yBottom;                  // [sp+44h] [bp-46Ch]@27
	wchar_t* v87;             // [sp+48h] [bp-468h]@4
	wchar_t* Source;          // [sp+4Ch] [bp-464h]@4
	int v89;                  // [sp+50h] [bp-460h]@4
	int v90;                  // [sp+54h] [bp-45Ch]@27
	int v91;                  // [sp+58h] [bp-458h]@27
	__int16 v92;              // [sp+5Ch] [bp-454h]@27
	__int16 v93;              // [sp+5Eh] [bp-452h]@27
	__int16 v94;              // [sp+60h] [bp-450h]@27
	struct tagRECT Rect;      // [sp+64h] [bp-44Ch]@27
	int v96;                  // [sp+74h] [bp-43Ch]@1
	char ClassName;           // [sp+78h] [bp-438h]@65
	char v98;                 // [sp+79h] [bp-437h]@65
	__int16 v99;              // [sp+175h] [bp-33Bh]@65
	char v100;                // [sp+177h] [bp-339h]@65
	struct tagTEXTMETRICA tm; // [sp+178h] [bp-338h]@20
	CHAR pszFaceName;         // [sp+1B0h] [bp-300h]@6
	char v103;                // [sp+1B1h] [bp-2FFh]@6
	__int16 v104;             // [sp+2ADh] [bp-203h]@6
	char v105;                // [sp+2AFh] [bp-201h]@6
	char WindowName;          // [sp+2B0h] [bp-200h]@6
	char v107;                // [sp+2B1h] [bp-1FFh]@6
	__int16 v108;             // [sp+3ADh] [bp-103h]@6
	char v109;                // [sp+3AFh] [bp-101h]@6
	char v110;                // [sp+3B0h] [bp-100h]@65
	char v111;                // [sp+3B1h] [bp-FFh]@65
	__int16 v112;             // [sp+4ADh] [bp-3h]@65
	char v113;                // [sp+4AFh] [bp-1h]@65
	v6 = dialogHandle;
	hInstance = exeHandle;
	v96 = dialogHandle;
	parentHwnd = SDrawGetFrameWindow(0);
	v77 = parentHwnd;
	if( !hWndParent )
		hWndParent = parentHwnd;
	v7 = *(uint*)v6;
	v79 = 0;
	if( v7 == -65535 ){
		v79 = 1;
		sub_15007DE0(v6, (int)&v89, (wchar_t**)&Source, (int)&v75, (int)&v74, (int)&v78, (int)&v87, (unsigned int)&v84);
	}else{
		sub_15007CC0(v6, (int)&v89, (wchar_t**)&Source, (int)&v75, (int)&v74, (int)&v78, (int)&v87, (unsigned int)&v84);
	}
	WindowName = ZeroByte;
	pszFaceName = ZeroByte;
	memset(&v107, 0, 0xFCu);
	v108 = 0;
	v109 = 0;
	memset(&v103, 0, 0xFCu);
	v104 = 0;
	v105 = 0;
	if( Source ){
		if( *Source ){
			if( *Source != -1 )
				wcstombs(&WindowName, Source, 0xFFu);
		}
	}
	if( v87 ){
		if( *v87 ){
			if( *v87 != -1 )
				wcstombs((char*)&pszFaceName, v87, 0xFFu);
		}
	}
	v8 = 0;
	wParam = 0;
	if( pszFaceName ){
		if( dword_1501E9E0 ){
			v69 = (CHAR*)(dword_1501E9E0 + 16);
			v68 = *(uint*)(dword_1501E9E0 + 12);
			v67 = 0;
			v66 = 0;
			v65 = 0;
			v64 = 0;
			v63 = 0;
			v9 = *(uint*)dword_1501E9E0;
			v62 = 0;
			v60 = 0;
			v58 = *(uint*)(dword_1501E9E0 + 4);
			v56 = 0;
			v54 = 0;
			v52 = 0;
			v50 = 72;
			v48 = 96;
		}else{
			v69 = &pszFaceName;
			v68 = 0;
			v67 = 0;
			v66 = 0;
			v65 = 0;
			v64 = 0;
			v63 = 0;
			v62 = 0;
			v60 = v78;
			v58 = v74;
			v9 = v75;
			v56 = 0;
			v54 = 0;
			v52 = 0;
			v50 = 72;
			v48 = 96;
		}
		v10 = MulDiv(v9, v48, v50);
		v8 = CreateFontA(-v10, v52, v54, v56, v58, v60, v62, v63, v64, v65, v66, v67, v68, v69);
		wParam = (WPARAM)v8;
	}
	if( v8 ){
		v12 = GetDC(hWndParent);
		v13 = CreateCompatibleDC(v12);
		hGdi = (char*)SelectObject(v13, v8);
		memset(&tm, 0, sizeof(tm));
		v14 = GetTextMetricsA(v13, &tm);
		LOBYTE_IDA(v14) = tm.tmPitchAndFamily;
		v11 = (v14 >> 2) & 1;
		SelectObject(v13, (HGDIOBJ)hGdi);
		DeleteDC(v13);
		ReleaseDC(hWndParent, v12);
		if( !v11 ){
			DeleteObject(v8);
			if( dword_1501E9E0 ){
				v15 = *(uint*)dword_1501E9E0;
				v61 = v11;
				v59 = *(uint*)(dword_1501E9E0 + 4);
				v57 = v11;
				v55 = v11;
				v53 = v11;
				v51 = 72;
				v49 = 96;
			}else{
				v15 = v75;
				v61 = v78;
				v59 = v74;
				v57 = 0;
				v55 = 0;
				v53 = 0;
				v51 = 72;
				v49 = 96;
			}
			v16 = MulDiv(v15, v49, v51);
			v8 = CreateFontA(-v16, v53, v55, v57, v59, v61, 0, 0, 0, 0, 0, 0, 0x20u, "Arial");
			wParam = (WPARAM)v8;
		}
	}
	v17 = 8;
	v80 = 16;
	v82 = 8;
	if( v8 ){
		sub_15007870(v8, (int)&v82, (int)&v80);
		v17 = v82;
	}
	dwStyle = v89 & 0xEFFFFFFF;
	style = v90 | (((unsigned __int8)(v89 & 0x80) | 0x800000u) >> 7);
	Rect.left = v17 * SHIWORD_IDA(v91) / 4;
	Rect.right = v17 * v93 / 4 + Rect.left - 1;
	Rect.top = v80 * v92 / 8;
	Rect.bottom = v80 * v94 / 8 + Rect.top - 1;
	AdjustWindowRectEx(&Rect, v89 & 0xEFFF0000, 0, v90 | (((unsigned __int8)(v89 & 0x80) | 0x800000u) >> 7));
	SDrawGetScreenSize((int)&xRight, (int)&yBottom, 0);
	width = Rect.right - Rect.left + 1;
	height = Rect.bottom - Rect.top + 1;
	if( dwStyle & 0x800 ){
		x_1 = (xRight - width) >> 1;
		x = (xRight - width) >> 1;
	}else{
		x_1 = v82 * SHIWORD_IDA(v91) / 4;
		x = v82 * SHIWORD_IDA(v91) / 4;
	}
	if( dwStyle & 0x800 ){
		y = (yBottom - height) >> 1;
	}else{
		y = v80 * v92 / 8;
		x_1 = x;
	}
	x_ = x_1;
	if( x_1 >= xRight - width )
		x_ = xRight - width;
	if( x_ >= 0 ){
		if( x_1 >= xRight - width )
			x = xRight - width;
	}else{
		x = 0;
	}
	y_ = y;
	if( y >= yBottom - height )
		y_ = yBottom - height;
	if( y_ >= 0 ){
		if( y >= yBottom - height )
			y = yBottom - height;
	}else{
		y = 0;
	}
	if( v77 ){
		if( IsIconic(v77) ){
			ShowWindow(v77, 3);
			while( sub_15007BA0(0) )
			;
			SetActiveWindow(hWndParent);
			SetFocus(hWndParent);
		}
	}
	desktopWindow = CreateWindowExA(style, "SDlgDialog", &WindowName, dwStyle, x, y, width, height, hWndParent, 0, hInstance, lpParam);
	if( desktopWindow ){
		v28 = GetDC(desktopWindow);
		SelectObject(v28, (HGDIOBJ)wParam);
		ReleaseDC(desktopWindow, v28);
		SetPropA(desktopWindow, "SDlg_EndDialog", 0);
		SetPropA(desktopWindow, "SDlg_EndResult", 0);
		v27 = v84;
		v71 = (WPARAM)desktopWindow;
		hGdi = 0;
		if( (ushort)v91 ){
			do{
				if( (v27 - v96) & 3 )
					v27 += 4 - ((v27 - v96) & 3);
				if( v79 )
					v27 += 4;
				v29 = v27 + 18;
				if( v79 )
					v29 = v27 + 20;
				if( *(ushort*)v29 == -1 )
					v30 = v29 + 4;
				else
					v30 = v29 + 2 * wcslen((wchar_t*)v29) + 2;
				if( *(ushort*)v30 == -1 )
					v77 = (HWND)(v30 + 6);
				else
					v77 = (HWND)(v30 + 4 + 2 * wcslen((wchar_t*)v30));
				ClassName = ZeroByte;
				v110 = ZeroByte;
				memset(&v98, 0, 0xFCu);
				v99 = 0;
				v100 = 0;
				memset(&v111, 0, 0xFCu);
				v112 = 0;
				v113 = 0;
				if( *(ushort*)v29 == -1 ){
					switch( *(ushort*)(v29 + 2) ){
					case 0x80:
						strcpy(&ClassName, "Button");
						break;
					case 0x81:
						strcpy(&ClassName, "Edit");
						break;
					case 0x82:
						strcpy(&ClassName, "SDlgStatic");
						break;
					case 0x83:
						strcpy(&ClassName, "Listbox");
						break;
					case 0x84:
						strcpy(&ClassName, "Scrollbar");
						break;
					case 0x85:
						strcpy(&ClassName, "Combobox");
						break;
					default:
						break;
					}
				}else{
					wcstombs(&ClassName, (wchar_t*)v29, 0xFFu);
				}
				if( *(ushort*)v30 != -1 )
					wcstombs(&v110, (wchar_t*)v30, 0xFFu);
				if( v79 )
					v31 = *(uint*)(v27 + 4);
				else
					v31 = *(uint*)v27;
				if( v79 )
					v32 = *(uint*)v27;
				else
					v32 = *(uint*)(v27 + 4);
				v34 = v80 * (int) * (ushort*)(v27 + 14);
				v35 = ((BYTE4(v34) & 7) + (uint)v34) >> 3;
				v36 = v82 * (int) * (ushort*)(v27 + 12);
				v37 = ((BYTE4(v36) & 3) + (uint)v36) >> 2;
				v38 = v80 * (int) * (ushort*)(v27 + 10);
				v39 = ((BYTE4(v38) & 7) + (uint)v38) >> 3;
				v40 = v82 * (int) * (ushort*)(v27 + 8);
				v41 = CreateWindowExA(v32, &ClassName, &v110, v31, ((BYTE4(v40) & 3) + (uint)v40) >> 2, v39, v37, v35, desktopWindow, (HMENU) * (ushort*)(v27 + 16), hInstance, v77);
				v33 = v41;
				v42 = GetWindowLongA(v41, -4);
				SetPropA(v33, "SDlg_WndProc", (HANDLE)v42);
				SetWindowLongA(v33, -4, (LONG)sub_15007920);
				if( v33 ){
					if( wParam )
						SendMessageA(v33, 0x30u, wParam, 0);
				}
				ShowWindow(v33, 5);
				if( v33 ){
					if( (HWND)v71 == desktopWindow ){
						if( !(*(uint*)v27 & 0x8000000) ){
							if( stricmp(&ClassName, "Static") ){
								if( stricmp(&ClassName, "SDlgStatic") )
									v71 = (WPARAM)v33;
							}
						}
					}
				}
				v27 = (int)((char*)v77 + *((ushort*)v77 - 1));
				v44 = __SETO__((char*)hGdi + 1, (unsigned __int16)v91);
				v43 = (int)(hGdi++ + -(unsigned __int16)v91 + 1) < 0;
			}while( v43 ^ v44 );
		}
		if( wParam )
			SendMessageA(desktopWindow, 0x30u, wParam, 0);
		if( !SendMessageA(desktopWindow, 0x110u, v71, lParam) )
			v71 = 0;
		for( i = GetTopWindow(desktopWindow); i; i = GetWindow(i, 2u) ){
			if( sub_15005800(i, 65535) ){
				v47 = GetWindowLongA(i, -16);
				SetPropA(i, "SDlg_OrigStyle", (HANDLE)v47);
				ClassName = ZeroByte;
				memset(&v98, 0, 0xFCu);
				v99 = 0;
				v46 = v47;
				v100 = 0;
				GetClassNameA(i, &ClassName, 256);
				if( stricmp(&ClassName, "Button") || v47 & 0xF && (v47 & 0xF) != 1 ){
					if( !stricmp(&ClassName, "ComboBox") || !stricmp(&ClassName, "ListBox") )
						v46 = v47 | 0x10;
				}else{
					v46 = v47 | 0xB;
				}
				if( v46 != v47 )
					SetWindowLongA(i, -16, v46);
			}
		}
		if( !GetPropA(desktopWindow, "SDlg_EndDialog") ){
			ShowWindow(desktopWindow, 1);
			if( v71 )
				SetFocus((HWND)v71);
		}
	}
	return desktopWindow;
}

// 1501E9E0: using guessed type int dword_1501E9E0;
//----- (15007870) --------------------------------------------------------
int __fastcall sub_15007870(HGDIOBJ h, int a2, int a3)
{
	int v3;                   // ebp@1
	HDC v4;                   // edi@1
	HDC v5;                   // esi@1
	HGDIOBJ v6;               // ebx@1
	HWND v7;                  // eax@1
	HWND v9;                  // eax@5
	HGDIOBJ v10;              // [sp+10h] [bp-44h]@1
	struct tagSIZE psizl;     // [sp+14h] [bp-40h]@1
	struct tagTEXTMETRICA tm; // [sp+1Ch] [bp-38h]@3
	v3 = a2;
	v6 = h;
	v7 = GetDesktopWindow();
	v4 = GetDC(v7);
	v5 = CreateCompatibleDC(v4);
	v10 = SelectObject(v5, v6);
	if( GetTextExtentPoint32A(v5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &psizl) )
		*(uint *)v3 = (psizl.cx / 26 + 1) / 2;
	if( GetTextMetricsA(v5, &tm) )
		*(uint *)a3 = tm.tmHeight;
	SelectObject(v5, v10);
	DeleteDC(v5);
	v9 = GetDesktopWindow();
	return ReleaseDC(v9, v4);
}

//----- (15007920) --------------------------------------------------------
LRESULT __stdcall sub_15007920(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LONG v4;                                             // ebp@1
	__int32 v5;                                          // eax@4
	HWND v7;                                             // eax@15
	LRESULT(__stdcall * v8)(HWND, UINT, WPARAM, LPARAM); // eax@16
	LRESULT v9;                                          // eax@19
	LRESULT v10;                                         // edi@21
	v4 = 0;
	switch( Msg ){
	case 0xF1u:
		if( wParam == 1 )
			break;
		goto LABEL_3;
	case 0xFu:
	case 0xF3u:
		LABEL_3:
		if( sub_15007C20(hWnd, 0) ){
			v5 = GetWindowLongA(hWnd, -16) & 0xF;
			if( v5 == 4 || v5 == 9 ){
				v4 = GetWindowLongA(hWnd, -16);
				SetWindowLongA(hWnd, -16, v4 & 0xFFFFFFFB | 0xB);
			}
		}
		break;
	case 0x14u:
		if( GetWindowLongA(hWnd, -20) & 0x20 )
			return 0;
		if( !sub_15005800(hWnd, 65535) )
			break;
		return 0;
	case WM_KEYUP:
		if( wParam == VK_SNAPSHOT ){
			SDrawCaptureScreen(0, Msg, 0);
			SetFocus(hWnd);
		}
		break;
	case 0x82u:
		((void (__thiscall *)(uint, uint, uint))RemovePropA)(0, (uint)hWnd, (uint)"SDlg_WndProc");
		if( GetPropA(hWnd, "SDlg_OrigStyle") )
			RemovePropA(hWnd, "SDlg_OrigStyle");
		break;
	case 0x7Cu:
	case 0x7Du:
		return 0;
	case 0x104u:
	case 0x105u:
		v7 = GetParent(hWnd);
		SendMessageA(v7, Msg, wParam, lParam);
		break;
	default:
		break;
	}
	v8 = 0;
	if( Msg != 0x82 )
		v8
	= (LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))GetPropA(hWnd, "SDlg_WndProc");
	if( v8 )
		v9 = CallWindowProcA(v8, hWnd, Msg, wParam, lParam);
	else
		v9 = DefWindowProcA(hWnd, Msg, wParam, lParam);
	v10 = v9;
	if( v4 )
		SetWindowLongA(hWnd, -16, v4);
	return v10;
}

//----- (15007BA0) --------------------------------------------------------
int __thiscall sub_15007BA0(void* hDlg)
{
	HWND v1;           // esi@1
	int result;        // eax@3
	struct tagMSG Msg; // [sp+4h] [bp-1Ch]@1
	v1 = (HWND)hDlg;
	if( PeekMessageA(&Msg, 0, 0, 0, 1u) ){
		if( Msg.message == 18 ){
			PostQuitMessage(Msg.wParam);
			result = 1;
		}else{
			if( !v1 || !IsDialogMessageA(v1, &Msg) ){
				TranslateMessage(&Msg);
				DispatchMessageA(&Msg);
			}
			result = 1;
		}
	}else{
		SDlgCheckTimers();
		SDlgUpdateCursor();
		result = 0;
	}
	return result;
}

//----- (15007C20) --------------------------------------------------------
int __fastcall sub_15007C20(HWND hWnd, int a2)
{
	HWND v2;    // ebx@1
	int v3;     // esi@1
	int result; // eax@3
	char v5;    // al@7
	CHAR Str1;  // [sp+Ch] [bp-100h]@5
	char v7;    // [sp+Dh] [bp-FFh]@5
	__int16 v8; // [sp+109h] [bp-3h]@5
	char v9;    // [sp+10Bh] [bp-1h]@5
	v3 = a2;
	v2 = hWnd;
	if( a2 )
		*(uint *)a2 = 0;
	result = IsWindow(hWnd);
	if( result ){
		Str1 = ZeroByte;
		memset(&v7, 0, 0xFCu);
		v8 = 0;
		v9 = 0;
		GetClassNameA(v2, &Str1, 256);
		if( stricmp(&Str1, "Button") ){
			result = 0;
		}else{
			v5 = GetWindowLongA(v2, -16);
			if( v3 )
				*(uint *)v3 = (v5 & 0xF) == 11;
			result = 1;
		}
	}
	return result;
}

//----- (15007CC0) --------------------------------------------------------
unsigned int __fastcall sub_15007CC0(int a1, int a2, wchar_t** a3, int a4, int a5, int a6, int a7, unsigned int a8)
{
	__int16 v8;          // ax@1
	int v9;              // edi@1
	int v10;             // esi@3
	wchar_t** v11;       // esi@7
	wchar_t* v12;        // ecx@7
	wchar_t* v13;        // edx@8
	unsigned int result; // eax@10
	wchar_t* v15;        // eax@10
	unsigned int v16;    // edx@11
	v9 = a2;
	*(uint*)a2 = *(uint*)a1;
	*(uint*)(a2 + 4) = *(uint*)(a1 + 4);
	*(uint*)(a2 + 8) = *(uint*)(a1 + 8);
	*(uint*)(a2 + 12) = *(uint*)(a1 + 12);
	*(ushort*)(a2 + 16) = *(ushort*)(a1 + 16);
	v8 = *(ushort*)(a1 + 18);
	if( v8 && v8 != -1 )
		v10 = a1 + 18 + 2 * wcslen((wchar_t*)(a1 + 18)) + 2;
	else
		v10 = a1 + 20;
	if( *(ushort*)v10 && *(ushort*)v10 != -1 ){
		v12 = (wchar_t*)(v10 + 2 * wcslen((wchar_t*)v10) + 2);
		v11 = a3;
		*a3 = v12;
	}else{
		v13 = (wchar_t*)(v10 + 2);
		v11 = a3;
		*a3 = v13;
	}
	if( *(uchar*)v9 & 0x40 ){
		*(ushort*)a4 = (*v11)[wcslen(*v11) + 1];
		*(ushort*)a5 = 0;
		*(ushort*)a6 = 0;
		v15 = &(*v11)[wcslen(*v11) + 2];
		*(uint*)a7 = (uint)v15;
		result = *(uint*)a7 + 2 * wcslen(v15) + 2;
		*(uint*)a8 = result;
	}else{
		*(ushort*)a4 = 0;
		*(ushort*)a5 = 0;
		*(ushort*)a6 = 0;
		*(uint*)a7 = 0;
		v16 = (unsigned int)&(*v11)[wcslen(*v11) + 1];
		result = a8;
		*(uint*)a8 = v16;
	}
	return result;
}

//----- (15007DE0) --------------------------------------------------------
unsigned int __fastcall sub_15007DE0(int a1, int a2, wchar_t** a3, int a4, int a5, int a6, int a7, unsigned int a8)
{
	__int16 v8;          // ax@1
	int v9;              // esi@1
	int v10;             // eax@1
	int v11;             // edx@1
	int v12;             // edi@3
	wchar_t** v13;       // edi@7
	wchar_t* v14;        // eax@7
	wchar_t* v15;        // ecx@8
	unsigned int result; // eax@10
	size_t v17;          // eax@10
	wchar_t v18;         // dx@10
	wchar_t* v19;        // eax@10
	__int16 v20;         // dx@10
	unsigned int v21;    // edx@11
	v9 = a2;
	*(uint*)a2 = *(uint*)(a1 + 8);
	*(uint*)(a2 + 4) = *(uint*)(a1 + 12);
	*(uint*)(a2 + 8) = *(uint*)(a1 + 16);
	*(uint*)(a2 + 12) = *(uint*)(a1 + 20);
	*(ushort*)(a2 + 16) = *(ushort*)(a1 + 24);
	v10 = *(uint*)a2;
	v11 = *(uint*)(a2 + 4);
	*(uint*)(v9 + 4) = v10;
	*(uint*)v9 = v11;
	v8 = *(ushort*)(a1 + 26);
	if( v8 && v8 != -1 )
		v12 = a1 + 26 + 2 * wcslen((wchar_t*)(a1 + 26)) + 2;
	else
		v12 = a1 + 28;
	if( *(ushort*)v12 && *(ushort*)v12 != -1 ){
		v14 = (wchar_t*)(v12 + 2 * wcslen((wchar_t*)v12) + 2);
		v13 = a3;
		*a3 = v14;
	}else{
		v15 = (wchar_t*)(v12 + 2);
		v13 = a3;
		*a3 = v15;
	}
	if( *(uchar*)v9 & 0x40 ){
		v17 = wcslen(*v13);
		v18 = (*v13)[v17 + 1];
		v19 = &(*v13)[v17 + 3];
		*(ushort*)a4 = v18;
		v20 = *(v19 - 1);
		++v19;
		*(ushort*)a5 = v20;
		*(ushort*)a6 = *((uchar*)v19 - 2) & 1;
		*(uint*)a7 = (uint)v19;
		result = *(uint*)a7 + 2 * wcslen(v19) + 2;
		*(uint*)a8 = result;
	}else{
		*(ushort*)a4 = 0;
		*(ushort*)a5 = 0;
		*(ushort*)a6 = 0;
		*(uint*)a7 = 0;
		v21 = (unsigned int)&(*v13)[wcslen(*v13) + 1];
		result = a8;
		*(uint*)a8 = v21;
	}
	return result;
}

//----- (15007F10) --------------------------------------------------------
LRESULT __fastcall sub_15007F10(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	dword_1501E9F8 = 1;
	return SendMessageA(hWnd, Msg, wParam, lParam);
}

// 1501E9F8: using guessed type int dword_1501E9F8;
//----- (15007F30) --------------------------------------------------------
HRSRC __stdcall SDlgCreateDialogParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam)
{
	HRSRC result; // eax@1
	int v6;       // ST04_4@3
	HMODULE v7;   // eax@3
	result = S_LoadDialogRes(hModule, lpName);
	if( result ){
		v6 = (int)result;
		v7 = GetModuleHandleA(0);
		result = (HRSRC)SDlgCreateDialog(v7, v6, hWndParent, lpParam, lParam);
	}
	return result;
}

//----- (15007F70) --------------------------------------------------------
HRSRC __fastcall S_LoadDialogRes(HMODULE hModule, LPCSTR lpName)
{
	HRSRC result; // eax@1
	HMODULE v3;   // esi@1
	v3 = hModule;
	result = FindResourceA(hModule, lpName, (LPCSTR)5);
	if( result ){
		result = (HRSRC)LoadResource(v3, result);
		if( result )
			result = (HRSRC)LockResource(result);
	}
	return result;
}

//----- (15007FA0) --------------------------------------------------------
HGDIOBJ __stdcall SDlgDefDialogProc(int a1, int a2, HDC hdc, HWND hWnd)
{
	HGDIOBJ result; // eax@3
	CHAR Str1;      // [sp+8h] [bp-100h]@2
	char v6;        // [sp+9h] [bp-FFh]@2
	__int16 v7;     // [sp+105h] [bp-3h]@2
	char v8;        // [sp+107h] [bp-1h]@2
	switch( a2 ){
	case 0x138:
		Str1 = ZeroByte;
		memset(&v6, 0, 0xFCu);
		v7 = 0;
		v8 = 0;
		GetClassNameA(hWnd, &Str1, 256);
		if( stricmp(&Str1, "SDlgStatic") )
			goto LABEL_4;
		SetTextColor(hdc, 0xFFFFFFu);
		SetBkMode(hdc, 1);
		result = GetStockObject(5);
		break;
	case 0x133:
	case 0x134:
		LABEL_4:
		SetTextColor(hdc, 0xFFFFFFu);
		SetBkColor(hdc, 0);
		SetBkMode(hdc, 2);
		result = GetStockObject(4);
		break;
	case 0x110:
		result = (HGDIOBJ)1;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

//----- (150080E0) --------------------------------------------------------
int __stdcall SDlgDestroy()
{
	int i;    // eax@1
	void* v1; // eax@3
	int v2;   // esi@4
	SDlgSetSystemCursor(0, 0, 0, 32512);
	for( i = dword_1501E9D8; dword_1501E9D8; i = dword_1501E9D8 )
		SDlgKillTimer(*(uint*)(i + 8), *(uint*)(i + 12));
	v1 = (void*)dword_1501E9D4;
	if( dword_1501E9D4 ){
		do{
			v2 = *(uint*)dword_1501E9D0;
			SMemFree(v1, ".?AUBITMAPREC@@", -2, 0);
			v1 = (void*)v2;
			dword_1501E9D4 = v2;
		}while( v2 );
	}
	dword_1501E9D0 = 0;
	if( dword_1501E9E0 ){
		SMemFree((void*)dword_1501E9E0, "SDLG.CPP", 1738, 0);
		dword_1501E9E0 = 0;
	}
	return 1;
}

// 15008C10: using guessed type _DWORD __stdcall SDlgSetSystemCursor(_DWORD, _DWORD, _DWORD, _DWORD);
// 1501E9D0: using guessed type int dword_1501E9D0;
// 1501E9D4: using guessed type int dword_1501E9D4;
// 1501E9D8: using guessed type int dword_1501E9D8;
// 1501E9E0: using guessed type int dword_1501E9E0;
//----- (15008180) --------------------------------------------------------
int __stdcall SDlgDialogBoxIndirectParam(HINSTANCE exeHandle, int dialogHandle, HWND hWndParent, LPVOID lpParam, LPARAM lParam)
{
	int result; // eax@3
	HWND v6;    // esi@4
	HANDLE v7;  // ebp@12
	if( dword_1501E9F0 || sub_15006370() ){
		v6 = S_CreateDialogWindow(exeHandle, dialogHandle, hWndParent, lpParam, lParam); // тут рисуется вне окна 
		if( v6 ){
			if( hWndParent ){
				if( hWndParent != GetDesktopWindow() ){
					SetPropA(v6, "SDlg_Modal", (HANDLE)HANDLE_FLAG_INHERIT);
					if( !IsIconic(hWndParent) )
						EnableWindow(hWndParent, 0);
				}
			}
			while( !GetPropA(v6, "SDlg_EndDialog") )
				sub_15007BA0(v6);
			v7 = GetPropA(v6, "SDlg_EndResult");
			RemovePropA(v6, "SDlg_EndDialog");
			RemovePropA(v6, "SDlg_EndResult");
			RemovePropA(v6, "SDlg_Modal");
			DestroyWindow(v6);
			if( hWndParent ){
				if( hWndParent != GetDesktopWindow() )
					EnableWindow(hWndParent, 1);
			}
			result = (int)v7;
		}else{
			result = -1;
		}
	}else{
		result = -1;
	}
	return result;
}

// 1501E9F0: using guessed type int dword_1501E9F0;
//----- (15008270) --------------------------------------------------------
int __stdcall SDlgDialogBoxParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam)
{
	HRSRC dialogHandle;   // eax@1
	int result; // eax@2
	HMODULE mainExeHandle; // eax@3
	dialogHandle = S_LoadDialogRes(hModule, lpName);
	if( dialogHandle ){
		mainExeHandle = GetModuleHandleA(0);
		result = SDlgDialogBoxIndirectParam(mainExeHandle, (int)dialogHandle, hWndParent, lpParam, lParam);
	}else{
		result = -1;
	}
	return result;
}

//----- (150082B0) --------------------------------------------------------
int __stdcall SDlgDrawBitmap(HWND hWnd, int a2, HRGN hrgnSrc2, int a4, int a5, int a6, char a7)
{
	int result;          // eax@2
	int v8;              // esi@7
	__int32 v9;          // edi@10
	int v10;             // ecx@10
	int v11;             // ecx@10
	int v12;             // eax@10
	int v13;             // eax@16
	__int64 v14;         // qax@16
	int v15;             // edx@18
	__int16 v16;         // cx@20
	int v17;             // [sp+Ch] [bp-20h]@20
	int v18;             // [sp+10h] [bp-1Ch]@20
	int v19;             // [sp+14h] [bp-18h]@20
	char v20;            // [sp+18h] [bp-14h]@20
	struct tagRECT Rect; // [sp+1Ch] [bp-10h]@9
	if( !(hWnd != 0 ? -(a2 != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	result = IsWindow(hWnd);
	if( result ){
		if( a2 != 1 || !(GetWindowLongA(hWnd, -20) & 0x20) ){
			result = sub_15005800(hWnd, a2);
			v8 = result;
			if( !result )
				return result;
			GetClientRect(hWnd, &Rect);
			ClientToScreen(hWnd, (LPPOINT)&Rect);
			ClientToScreen(hWnd, (LPPOINT)&Rect.right);
			if( a6 ){
				v10 = *(uint*)(a6 + 4);
				Rect.left += ((*(uint*)a6 < 0) - 1) & *(uint*)a6;
				v9 = (((v10 < 0) - 1) & v10) + Rect.top;
				v11 = *(uint*)(a6 + 8);
				v12 = *(uint*)(a6 + 12);
				Rect.top = v9;
				Rect.right += ((v11 > 0) - 1) & v11;
				Rect.bottom += ((v12 > 0) - 1) & v12;
			}else{
				v9 = Rect.top;
			}
			if( !(a7 & 1) ){
				if( Rect.right >= *(uint*)(v8 + 36) - *(uint*)(v8 + 28) + Rect.left + 1 )
					Rect.right = *(uint*)(v8 + 36) - *(uint*)(v8 + 28) + Rect.left + 1;
				if( a7 & 2 ){
					v14 = *(uint*)(v8 + 32) + Rect.bottom - v9 - *(uint*)(v8 + 40);
					v13 = ((uint)v14 - HIDWORD_IDA(v14)) >> 1;
					if( v13 > 0 ){
						Rect.top = v13 + v9;
						Rect.bottom = v13 + v9 - *(uint*)(v8 + 32) + *(uint*)(v8 + 40) + 1;
					}
				}else{
					v15 = *(uint*)(v8 + 32);
					if( Rect.bottom >= *(uint*)(v8 + 40) - v15 + v9 + 1 )
						Rect.bottom = *(uint*)(v8 + 40) - v15 + v9 + 1;
				}
			}
			v17 = *(uint*)(v8 + 44);
			v18 = *(uint*)(v8 + 48);
			SDrawGetScreenSize((int)&v19, (int)&v20, 0);
			v16 = 3;
			if( !dword_1501E9E4 ){
				v16 = 4099;
				if( !dword_1501E9F4 )
					v16 = 12291;
			}
			sub_15005A30(0, (int)&Rect, (int)&v19, 0, *(uint*)(v8 + 24), v8 + 28, (int)&v17, *(uint*)(v8 + 44), a4, a5, 0, 0xCC0020u, hWnd, hrgnSrc2, v16, 0, dword_1501E9E4 == 0 ? (int)&dword_1501E9E4 : 0);
		}
		result = 1;
	}
	return result;
}

// 1501E9E4: using guessed type int dword_1501E9E4;
// 1501E9F4: using guessed type int dword_1501E9F4;
//----- (150084C0) --------------------------------------------------------
int __stdcall SDlgEndDialog(HWND hDlg, HANDLE nResult)
{
	SetPropA(hDlg, "SDlg_EndDialog", (HANDLE)HANDLE_FLAG_INHERIT);
	SetPropA(hDlg, "SDlg_EndResult", (HANDLE)nResult);
	return EndDialog(hDlg, (INT_PTR)nResult);
}

//----- (15008500) --------------------------------------------------------
int __stdcall SDlgEndPaint(HWND hWnd, PAINTSTRUCT* lpPaint)
{
	if( dword_1501E9E4 ){
		do
			ShowCursor(1);while( dword_1501E9E4-- != 1 );
	}
	--dword_1501E9F4;
	return EndPaint(hWnd, lpPaint);
}

// 1501E9E4: using guessed type int dword_1501E9E4;
// 1501E9F4: using guessed type int dword_1501E9F4;
//----- (15008550) --------------------------------------------------------
int __stdcall SDlgKillTimer(HWND hWnd, int a2)
{
	int v2; // eax@1
	int v3; // ebp@1
	int v4; // edi@1
	int v5; // esi@4
	int v6; // ecx@8
	v4 = dword_1501E9D8;
	v3 = 0;
	v2 = dword_1501E9D8;
	while( v2 ){
		if( *(HWND*)(v2 + 8) != hWnd || *(uint*)(v2 + 12) != a2 ){
			v2 = *(uint*)(v2 + 4);
		}else{
			v3 = 1;
			v5 = *(uint*)(v2 + 4);
			if( *(uint*)v2 ){
				*(uint*)(*(uint*)v2 + 4) = *(uint*)(v2 + 4);
			}else{
				if( v4 == v2 )
					dword_1501E9D8 = *(uint*)(v2 + 4);
			}
			v6 = *(uint*)(v2 + 4);
			if( v6 ){
				*(uint*)v6 = *(uint*)v2;
			}else{
				if( dword_1501E9DC == v2 )
					dword_1501E9DC = *(uint*)v2;
			}
			*(uint*)v2 = 0;
			*(uint*)(v2 + 4) = 0;
			SMemFree((void*)v2, ".?AUTIMERREC@@", -2, 0);
			v4 = dword_1501E9D8;
			v2 = v5;
		}
	}
	return v3;
}

// 1501E9D8: using guessed type int dword_1501E9D8;
// 1501E9DC: using guessed type int dword_1501E9DC;
//----- (150085F0) --------------------------------------------------------
int __stdcall SDlgSetBaseFont(int a1, int a2, int a3, int a4, char* Source)
{
	int v5;     // eax@5
	int result; // eax@7
	if( a1 && a2 && Source && *Source ){
		v5 = dword_1501E9E0;
		if( !dword_1501E9E0 ){
			v5 = SMemAlloc(48, "SDLG.CPP", 1939, dword_1501E9E0);
			dword_1501E9E0 = v5;
		}
		*(uint*)v5 = a1;
		*(uint*)(dword_1501E9E0 + 4) = a2;
		*(uint*)(dword_1501E9E0 + 8) = a3;
		*(uint*)(dword_1501E9E0 + 12) = a4;
		strncpy((char*)(dword_1501E9E0 + 16), Source, 0x1Fu);
		*(uchar*)(dword_1501E9E0 + 47) = 0;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E9E0: using guessed type int dword_1501E9E0;
//----- (15008690) --------------------------------------------------------
int __stdcall SDlgSetBitmap(int a1, int a2, char* Str2, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
	char* v10;           // ecx@2
	int v11;             // eax@4
	int v12;             // ebp@6
	int result;          // eax@7
	char v14;            // zf@12
	int v15;             // esi@12
	int v16;             // ebp@17
	int v17;             // eax@21
	int v18;             // eax@41
	int v19;             // ebx@48
	int v20;             // edx@49
	int v21;             // ecx@49
	int v22;             // eax@53
	int v23;             // edx@53
	int v24;             // edi@53
	unsigned __int8 v25; // sf@53
	unsigned __int8 v26; // of@53
	int v27;             // [sp+14h] [bp+4h]@52
	if( a1 || (v10 = Str2, Str2) && *Str2 ){
		v10 = Str2;
		v11 = -1;
	}else{
		v11 = 0;
	}
	v12 = a5;
	if( (a5 != 0 ? ((a5 & 0xFFFF0000) != 0) - 1 : 0) & v11 ){
		if( !v10 )
			Str2 = &ZeroByte;
		if( a10 == -1 ){
			v15 = dword_1501E9D0;
			v14 = dword_1501E9D0 == 0;
			if( dword_1501E9D0 ){
				do{
					if( (!*(uint*)(v15 + 8) || IsWindow(*(HWND*)(v15 + 8))) && (!*(uint*)(v15 + 12) || IsWindow(*(HWND*)(v15 + 12))) ){
						v15 = *(uint*)(v15 + 4);
					}else{
						v16 = *(uint*)(v15 + 4);
						if( *(uint*)v15 ){
							*(uint*)(*(uint*)v15 + 4) = *(uint*)(v15 + 4);
						}else{
							if( dword_1501E9D0 == v15 )
								dword_1501E9D0 = *(uint*)(v15 + 4);
						}
						v17 = *(uint*)(v15 + 4);
						if( v17 ){
							*(uint*)v17 = *(uint*)v15;
						}else{
							if( dword_1501E9D4 == v15 )
								dword_1501E9D4 = *(uint*)v15;
						}
						*(uint*)v15 = 0;
						*(uint*)(v15 + 4) = 0;
						SMemFree((void*)v15, ".?AUBITMAPREC@@", -2, 0);
						v15 = v16;
						v12 = a5;
					}
				}while( v15 );
				v15 = dword_1501E9D0;
				v14 = dword_1501E9D0 == 0;
			}
			if( !v14 ){
				do{
					if( *(uint*)(v15 + 8) == a1 && *(uint*)(v15 + 12) == a2 && *(uint*)(v15 + 16) == v12 && *(uint*)(v15 + 20) == a4 && !stricmp((char*)(v15 + 60), Str2) )
						break;
					v15 = *(uint*)(v15 + 4);
				}while( v15 );
			}
			if( v15 ){
				if( *(uint*)v15 ){
					*(uint*)(*(uint*)v15 + 4) = *(uint*)(v15 + 4);
				}else{
					if( dword_1501E9D0 == v15 )
						dword_1501E9D0 = *(uint*)(v15 + 4);
				}
				v18 = *(uint*)(v15 + 4);
				if( v18 ){
					*(uint*)v18 = *(uint*)v15;
				}else{
					if( dword_1501E9D4 == v15 )
						dword_1501E9D4 = *(uint*)v15;
				}
				*(uint*)v15 = 0;
				*(uint*)(v15 + 4) = 0;
				SMemFree((void*)v15, ".?AUBITMAPREC@@", -2, 0);
			}
			if( a6 ){
				if( a8 > 0 ){
					v19 = a9;
					if( a9 > 0 ){
						v21 = SMemAlloc(92, ".?AUBITMAPREC@@", -2, 8);
						v20 = 0;
						*(uint*)(v21 + 4) = dword_1501E9D0;
						if( dword_1501E9D0 )
							*(uint *)dword_1501E9D0 = v21;
						else
							dword_1501E9D4 = v21;
						dword_1501E9D0 = v21;
						*(uint*)(v21 + 8) = a1;
						*(uint*)(v21 + 24) = a6;
						*(uint*)(v21 + 12) = a2;
						*(uint*)(v21 + 16) = a5;
						*(uint*)(v21 + 20) = a4;
						*(uint*)(v21 + 44) = a8;
						*(uint*)(v21 + 48) = a9;
						*(uint*)(v21 + 52) = 0;
						*(uint*)(v21 + 56) = 0;
						v27 = 0;
						if( !a7 )
							goto LABEL_63;
						*(uint*)(v21 + 28) = *(uint*)a7 % a8;
						*(uint*)(v21 + 32) = *(uint*)(a7 + 4) % a9;
						v23 = *(uint*)(v21 + 28);
						v24 = *(uint*)(v21 + 32);
						*(uint*)(v21 + 36) = v23 + *(uint*)(a7 + 8) - *(uint*)a7;
						v22 = v24 + *(uint*)(a7 + 12) - *(uint*)(a7 + 4);
						v26 = __SETO__(*(uint*)(v21 + 36), a8);
						v25 = *(uint*)(v21 + 36) - a8 < 0;
						*(uint*)(v21 + 40) = v22;
						if( !(v25 ^ v26) || (v19 = a9, v22 >= a9) ){
							v27 = 1;
							*(uint*)(v21 + 52) = v23;
							*(uint*)(v21 + 56) = v24;
						}
						v20 = 0;
						if( !a7 || v27 ){
							LABEL_63:
							*(uint*)(v21 + 28) = v20;
							*(uint*)(v21 + 32) = v20;
							*(uint*)(v21 + 36) = a8 - 1;
							*(uint*)(v21 + 40) = v19 - 1;
						}
						strcpy((char*)(v21 + 60), Str2);
					}
				}
			}
			result = 1;
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E9D0: using guessed type int dword_1501E9D0;
// 1501E9D4: using guessed type int dword_1501E9D4;
//----- (15008950) --------------------------------------------------------
int __stdcall SDlgSetControlBitmaps(HWND hWnd, int* a2, char* a3, int a4, int a5, int a6, int a7)
{
	int v7;                // ebx@1
	int v8;                // edi@1
	int result;            // eax@2
	int* v10;              // eax@6
	char v11;              // zf@6
	HWND v12;              // eax@7
	int v13;               // esi@7
	_UNKNOWN* v14;         // edi@8
	int v15;               // edx@11
	int v16;               // ecx@11
	int v17;               // ecx@14
	int v18;               // eax@16
	struct tagPOINT Point; // [sp+Ch] [bp-30h]@6
	__int32 v20;           // [sp+14h] [bp-28h]@6
	LONG v21;              // [sp+18h] [bp-24h]@6
	struct tagRECT Rect;   // [sp+1Ch] [bp-20h]@8
	LONG v23;              // [sp+2Ch] [bp-10h]@8
	LONG v24;              // [sp+30h] [bp-Ch]@10
	int v25;               // [sp+50h] [bp+14h]@6
	v7 = a5;
	v8 = 0;
	if( (a5 != 0 ? (a4 != 0 ? (a2 != 0 ? (hWnd != 0 ? (*(uint*)a5 <= 0) - 1 : 0) : 0) : 0) : 0) & ((*(uint*)(a5 + 4) <= 0) - 1) ){
		if( !a3 ){
			a3 = &unk_1501C240;
			if( a6 != 1 )
				a3 = &unk_1501C238;
		}
		v10 = a2;
		v11 = *a2 == 0;
		Point.x = 0;
		Point.y = 0;
		v20 = 0;
		v21 = 0;
		v25 = 1;
		if( !v11 ){
			do{
				v12 = GetDlgItem(hWnd, *v10);
				v13 = (int)v12;
				if( v12 == (HWND)v8 ){
					v25 = v8;
				}else{
					GetClientRect(v12, &Rect);
					GetWindowRect((HWND)v13, (LPRECT)&v23);
					v14 = a3;
					if( *(uint*)a3 ){
						do{
							if( a6 == 2 ){
								Point.x = v23;
								Point.y = v24;
								ScreenToClient(hWnd, &Point);
							}
							v20 = Point.x + Rect.right - Rect.left;
							v15 = *(uint*)v7;
							v16 = *(uint*)(v7 + 4);
							v21 = Point.y + Rect.bottom - Rect.top;
							if( !SDlgSetBitmap(v13, 0, 0, -1, *(uint*)v14, a4, (int)&Point, v15, v16, a7) )
								v25 = 0;
							if( a6 == 1 ){
								v17 = *(uint*)(v7 + 4);
								Point.y = v21;
								if( v21 >= v17 )
									Point.y = 0;
							}
							v18 = *((uint*)v14 + 1);
							v14 = (char*)v14 + 4;
						}while( v18 );
					}
				}
				v8 = 0;
				v10 = a2 + 1;
				a2 = v10;
			}while( *v10 );
		}
		result = v25;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15008B10) --------------------------------------------------------
int __stdcall SDlgSetCursor(HWND hWnd, LONG dwNewLong, int a3, int a4)
{
	int* v5;  // ebx@6
	CHAR* v6; // eax@12
	HWND v7;  // esi@13
	int v8;   // [sp+10h] [bp-20h]@3
	int v9;   // [sp+14h] [bp-1Ch]@3
	int v10;  // [sp+18h] [bp-18h]@3
	int v11;  // [sp+1Ch] [bp-14h]@3
	int v12;  // [sp+20h] [bp-10h]@3
	int v13;  // [sp+24h] [bp-Ch]@3
	int v14;  // [sp+28h] [bp-8h]@3
	int v15;  // [sp+2Ch] [bp-4h]@3
	if( a4 )
		*(uint *)a4 = 0;
	v10 = (int)"Button";
	v11 = (int)"SDlgStatic";
	v12 = (int)"ListBox";
	v13 = (int)"Scrollbar";
	v14 = (int)"ComboBox";
	v15 = 0;
	v8 = (int)"Edit";
	v9 = 0;
	if( a3 == 32512 ){
		v5 = &v10;
	}else{
		if( a3 != 32513 )
			return 0;
		v5 = &v8;
	}
	if( a3 == 32512 ){
		if( a4 )
			*(uint *)a4 = GetClassLongA(hWnd, -12);
		SetClassLongA(hWnd, -12, dwNewLong);
	}
	v6 = (CHAR*)*v5;
	if( *v5 ){
		do{
			v7 = FindWindowExA(hWnd, 0, v6, 0);
			if( v7 ){
				if( a4 ){
					if( a3 != 32512 )
						*(uint *)a4 = GetClassLongA(hWnd, -12);
				}
				SetClassLongA(v7, -12, dwNewLong);
			}
			v6 = (CHAR*)v5[1];
			++v5;
		}while( v6 );
	}
	return 1;
}

//----- (15008C10) --------------------------------------------------------
int __stdcall SDlgSetSystemCursor(int a1, int a2, int a3, int a4)
{
	int result;            // eax@2
	struct tagPOINT Point; // [sp+0h] [bp-8h]@5
	if( a4 == 32512 ){
		if( a1 && a2 ){
			SDlgSetSystemCursor(0, 0, 0, 32512);
			dword_1501E9EC = a1;
			dword_1501E9E8 = a2;
			dword_1501E0D8 = *(uint*)a3;
			dword_1501E0DC = *(uint*)(a3 + 4);
			GetCursorPos(&Point);
			sub_15008CC0(0, Point.x, Point.y);
			result = 1;
		}else{
			dword_1501E9EC = 0;
			dword_1501E9E8 = 0;
			CursorCoord = -1i64;
			result = 1;
		}
	}else{
		result = 0;
	}
	return result;
}

// 15008C10: using guessed type _DWORD __stdcall SDlgSetSystemCursor(_DWORD, _DWORD, _DWORD, _DWORD);
// 1501E0D0: using guessed type __int64 qword_1501E0D0;
// 1501E0DC: using guessed type int dword_1501E0DC;
// 1501E9E8: using guessed type int dword_1501E9E8;
// 1501E9EC: using guessed type int dword_1501E9EC;
//----- (15008CC0) --------------------------------------------------------
HWND __fastcall sub_15008CC0(HWND a1, LONG a2, DWORD dwProcessId)
{
	DWORD v3;             // ebx@1
	LONG i;               // edi@1
	HWND v5;              // esi@1
	LONG v6;              // esi@2
	DWORD v7;             // ST08_4@2
	HWND v8;              // eax@2
	HWND result;          // eax@6
	HWND j;               // esi@6
	RECT rcSrc1;          // [sp+10h] [bp-30h]@4
	struct tagRECT rcDst; // [sp+20h] [bp-20h]@4
	struct tagRECT Rect;  // [sp+30h] [bp-10h]@4
	v3 = dwProcessId;
	v5 = a1;
	for( i = a2; !v5; v5 = GetDesktopWindow() ){
		v6 = CursorCoord;
		v7 = HIDWORD_IDA(CursorCoord);
		v8 = GetDesktopWindow();
		sub_15008CC0(v8, v6, v7);
		LODWORD_IDA(CursorCoord) = i;
		HIDWORD_IDA(CursorCoord) = v3;
	}
	GetWindowThreadProcessId(v5, &dwProcessId);
	if( dwProcessId == GetCurrentProcessId() ){
		rcSrc1.left = i;
		rcSrc1.top = v3;
		rcSrc1.right = i + dword_1501E0D8;
		rcSrc1.bottom = v3 + dword_1501E0DC;
		GetWindowRect(v5, &Rect);
		if( IntersectRect(&rcDst, &rcSrc1, &Rect) ){
			ScreenToClient(v5, (LPPOINT)&rcDst);
			ScreenToClient(v5, (LPPOINT)&rcDst.right);
			InvalidateRect(v5, &rcDst, 0);
		}
	}
	result = GetTopWindow(v5);
	for( j = result; result; j = result ){
		sub_15008CC0(j, i, v3);
		result = GetWindow(j, 2u);
	}
	return result;
}

// 1501E0D0: using guessed type __int64 qword_1501E0D0;
// 1501E0DC: using guessed type int dword_1501E0DC;
//----- (15008DC0) --------------------------------------------------------
int __stdcall SDlgSetTimer(HWND hWnd, int a2, int a3, int a4)
{
	int v4; // esi@1
	SDlgKillTimer(hWnd, a2);
	v4 = SMemAlloc(28, ".?AUTIMERREC@@", -2, 8);
	*(uint*)v4 = dword_1501E9DC;
	if( dword_1501E9D8 ){
		*(uint*)(dword_1501E9DC + 4) = v4;
		dword_1501E9DC = v4;
	}else{
		dword_1501E9DC = v4;
		dword_1501E9D8 = v4;
	}
	*(uint*)(v4 + 8) = (uint)hWnd;
	*(uint*)(v4 + 12) = a2;
	*(uint*)(v4 + 16) = a3;
	*(uint*)(v4 + 20) = a4;
	*(uint*)(v4 + 24) = GetTickCount();
	return a2;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E9D8: using guessed type int dword_1501E9D8;
// 1501E9DC: using guessed type int dword_1501E9DC;
//----- (15008E40) --------------------------------------------------------
int __stdcall SDlgUpdateCursor()
{
	sub_15008E50();
	return 1;
}

//----- (15008E50) --------------------------------------------------------
__int32 __stdcall sub_15008E50()
{
	__int32 result;        // eax@1
	struct tagPOINT Point; // [sp+0h] [bp-8h]@1
	GetCursorPos(&Point);
	result = CursorCoord;
	if( CursorCoord >= 0 ){
		result = Point.y;
		if( Point.x != (uint)CursorCoord || Point.y != HIDWORD_IDA(CursorCoord) )
			result = (__int32)sub_15008CC0(0, Point.x, Point.y);
	}
	return result;
}

typedef HRESULT (WINAPI *FPDirectDrawCreate)(GUID FAR *lpGUID,LPDIRECTDRAW FAR *lplpDD,IUnknown FAR *pUnkOuter);



// 1501E0D0: using guessed type __int64 qword_1501E0D0;
//----- (15008E90) --------------------------------------------------------
int __stdcall SDrawAutoInitialize( HINSTANCE hInstance, LPCSTR lpClassName, LPCSTR lpCaption, WNDPROC wndProc, ADDITIONAL_SURFACE_TYPE additionalSurfaceType, int displayWidth, int sHeight, int bytesPerPixel )
{
	void* v10;          // edx@4
	int i;              // ecx@4
	void* v12;          // ebx@4
	int curWidth;            // ebp@4
	int v14;            // edi@4
	int curHeight;            // eax@4
	unsigned int v16;   // esi@4
	HBRUSH backgroundBrush;         // eax@10
	int scrHeight;            // eax@11
	int displayHeight;            // ebx@11
	int scrWidth;            // edi@11
	DWORD dwExStyle;          // esi@11
	HMODULE hDDrawDll;        // eax@15
	FPDirectDrawCreate fpDirectDrawCreate;        // eax@17
	int v32;            // eax@49
	HDC hDC;            // esi@55
	DDSCAPS ddsCaps;        // [sp+44h] [bp-244h]@4
	WNDCLASSA WndClass; // [sp+4Ch] [bp-23Ch]@8
	DDSURFACEDESC ddSurfaceDesc;            // [sp+74h] [bp-214h]@31
	RECT lockRect;            // [sp+104h] [bp-184h]@49
	DDCAPS  ddCaps;            // [sp+14Ch] [bp-13Ch]@26

    if( additionalSurfaceType > 3 
        || additionalSurfaceType <= 0 
        || lpCaption == NULL 
        || lpClassName == NULL 
        || hInstance == NULL)
    {
        SErrSetLastError(0x57u);
        return 0;
    }
	dword_1501EA04 = 1;
	dword_1501EA10 = 0;
	if( !hCursor ){
		curWidth = GetSystemMetrics(SM_CXCURSOR);
		curHeight = GetSystemMetrics(SM_CYCURSOR);
		v16 = 4 * curHeight * (curWidth + 31) / 32;
		v12 = (void*)SMemAlloc(4 * curHeight * (curWidth + 31) / 32, __FILE__, __LINE__, 0);
		v10 = (void*)SMemAlloc(v16, __FILE__, __LINE__, 8);
		memset(v12, -1, 4 * (v16 >> 2));
		v14 = (int)((char*)v12 + 4 * (v16 >> 2));
		for( i = v16 & 3; i; --i )
			*(uchar *)v14++ = -1;
		hCursor = CreateCursor(hInstance, 0, 0, curWidth, curHeight, v12, v10);
		SMemFree(v12, __FILE__, __LINE__, 0);
		SMemFree(v10, __FILE__, __LINE__, 0);
	}
	memset(&WndClass, 0, sizeof(WndClass));
	WndClass.lpfnWndProc = wndProc;
	if( !wndProc )
		WndClass.lpfnWndProc = sub_150095B0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIconA(0, (LPCSTR)0x7F00);
	WndClass.hCursor = hCursor;
	backgroundBrush = (HBRUSH)GetStockObject(4);
	WndClass.lpszClassName = lpClassName;
	WndClass.hbrBackground = backgroundBrush;
	RegisterClassA(&WndClass);
	if( (unsigned int)GetStormDebugLevel() < 2 ){
		dwExStyle = WS_EX_TOPMOST;
		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		displayHeight = sHeight;
	}else{
		displayHeight = sHeight;
		scrWidth = curHeight;
		dwExStyle = 0;
		scrHeight = sHeight;
	}
	S_hWnd = CreateWindowExA(dwExStyle, lpClassName, lpCaption, 0x90000000u, 0, 0, scrWidth, scrHeight, 0, 0, hInstance, 0);
	if( !S_hWnd )
		return 0;
	SMsgRegisterMessage((int)S_hWnd, 2, (int)sub_15009590);
	SMsgRegisterKeyUp((int)S_hWnd, VK_SNAPSHOT, (int)sub_150095A0);
	hDDrawDll = hModule;
	if( hModule || (hDDrawDll = LoadLibraryA("ddraw.dll"), hModule = hDDrawDll, hDDrawDll) ){
		fpDirectDrawCreate = (FPDirectDrawCreate)GetProcAddress(hDDrawDll, "DirectDrawCreate");
		if( fpDirectDrawCreate )
			(*fpDirectDrawCreate)(0, (LPDIRECTDRAW*)&StormDdraw_1501EA20, 0);
		if( !StormDdraw_1501EA20 )
			goto LABEL_25;
		if( GetStormDebugLevel() < 2 ){
			StormDdraw_1501EA20->SetCooperativeLevel(S_hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
			StormDdraw_1501EA20->SetDisplayMode(displayWidth, displayHeight, bytesPerPixel);
		}else{
			StormDdraw_1501EA20->SetCooperativeLevel(S_hWnd, DDSCL_NORMAL);
		}
		dword_1501E110 = bytesPerPixel;
		StormScreenWidth = displayWidth;
		StormScreenHeight = displayHeight;
	}
	if( !StormDdraw_1501EA20 ){
		LABEL_25:
		MessageBoxA(S_hWnd, "DirectDraw services are not available.  You must install Microsoft DirectX version 2.0 or higher.  If you have difficulty installing or using Microsoft DirectX on your computer, please contact Microsoft Product Support Services.", lpCaption, 0x10010u);
		return 0;
	}
	memset(&ddCaps, 0, sizeof(DDCAPS));
	dword_1501EA00 = 0;
	ddCaps.dwSize = sizeof(DDCAPS);
	if( !StormDdraw_1501EA20->GetCaps(&ddCaps, 0) && ddCaps.dwCaps & 0x80000000 && ddCaps.dwSVBCaps & 0x80 ){
		dword_1501EA00 = 1;
	}
	memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
	ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
	ddSurfaceDesc.dwFlags = DDSD_CAPS;
	ddSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if( additionalSurfaceType == astBackBuffer ){
		ddSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddSurfaceDesc.dwBackBufferCount = 536;
	}
	if( StormDdraw_1501EA20->CreateSurface(	&ddSurfaceDesc, &StormSurface_1501EE40.mainSurface ,0) )
		return 0;
	if( additionalSurfaceType == astBackBuffer ){ 
		memset(&ddsCaps, 0, sizeof(ddsCaps));
		ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		if( !StormSurface_1501EE40.mainSurface )
			goto LABEL_39;
		StormSurface_1501EE40.mainSurface->GetAttachedSurface(&ddsCaps,	&StormSurface_1501EE40.backBuffer);
	}
	LABEL_39:
	if( additionalSurfaceType == astOffscreenSurface ){
		memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
		ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
		ddSurfaceDesc.dwWidth = displayWidth;
		ddSurfaceDesc.dwHeight = sHeight;
		ddSurfaceDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		StormDdraw_1501EA20->CreateSurface(&ddSurfaceDesc, &StormSurface_1501EE40.offscreenSurface, 0);
	}
	if( !StormSurface_1501EE40.mainSurface )
		return 0;
	if( additionalSurfaceType == astBackBuffer && !StormSurface_1501EE40.backBuffer )
		return 0;
	if( additionalSurfaceType == astOffscreenSurface && !StormSurface_1501EE40.offscreenSurface )
		return 0;
	memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
	ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
	v32 = StormSurface_1501EE40.mainSurface->Lock(0, &ddSurfaceDesc, 1, 0);
	if( lockRect.bottom != 0 || lockRect.left != 0 || lockRect.right != 0 || lockRect.top != 0 ){
		StormSurface_1501EE40.mainSurface->Unlock(&lockRect);
	}else{
		if( v32 != DDERR_SURFACELOST /*-2005532222*/ ){
			if( v32 != DDERR_WASSTILLDRAWING /*-2005532132*/ ){
				memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
				ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
				ddSurfaceDesc.dwWidth = displayWidth;
				ddSurfaceDesc.dwHeight = sHeight;
				ddSurfaceDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
				ddSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
				StormDdraw_1501EA20->CreateSurface( &ddSurfaceDesc, &StormSurface_1501EE40.unknownSurface, 0);
			}
		}
	}
	if( bytesPerPixel == 8 ){
		hDC = GetDC(0);
		GetSystemPaletteEntries(hDC, 0, 256, &S_pPalEntries);
		ReleaseDC(0, hDC);
		//it might be some correction on system palette but I can't understand the purpose
		//v35 = (int)&unk_1501EA53;
		//do{
		//	*(uchar*)v35 = 5;
		//	v35 += 4;
		//}while( v35 < (int)&unk_1501EE03 );
		StormDdraw_1501EA20->CreatePalette(
			DDPCAPS_PRIMARYSURFACE | DDPCAPS_8BIT,  //it was 64 | 8 | 4 but there is no flag with code 0x08
			&S_pPalEntries,
			&StormPalette_1501EA24,
			NULL);
		if( !StormPalette_1501EA24 ){
			SDrawDestroy();
			return 0;
		}
		SelectPaletteToHwnd();
	}

	for( int i = 0; i < 4; ++i){
		if( StormSurface_1501EE40.Surfaces[i]){
			SDrawShowCursor(i);
			if( StormPalette_1501EA24 )
				StormSurface_1501EE40.Surfaces[i]->SetPalette(StormPalette_1501EA24);
		}
	}
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EA00: using guessed type int dword_1501EA00;
// 1501EA04: using guessed type int dword_1501EA04;
// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
// 1501EE48: using guessed type int dword_1501EE48;
// 1501EE4C: using guessed type int dword_1501EE4C;
//----- (150094D0) --------------------------------------------------------
int __stdcall SelectPaletteToHwnd()
{
	int result; // eax@3
	LPLOGPALETTE logPalette;     // eax@3
	HDC hDC;     // esi@5
	if( StormPalette_1501EA24 )
		StormPalette_1501EA24->GetEntries(0, 0, 256, &S_pPalEntries);

	logPalette = (LPLOGPALETTE)SMemAlloc(sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*256, __FILE__, __LINE__, 0);
	logPalette->palVersion = 0x300; 
	logPalette->palNumEntries = 256;
	memcpy(&logPalette->palPalEntry[0], &S_pPalEntries, sizeof(PALETTEENTRY)*256);
	hPal = CreatePalette(logPalette);
	dword_1501EA08 = 1;
	SMemFree((void*)logPalette, __FILE__, __LINE__, 0);
	result = (int)S_hWnd;
	if( S_hWnd && hPal ){
		hDC = GetDC(S_hWnd);
		SelectPalette(hDC, hPal, 0);
		result = ReleaseDC(S_hWnd, hDC);
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EA08: using guessed type int dword_1501EA08;
// 1501EA24: using guessed type int StormPalette_1501EA24;
//----- (15009590) --------------------------------------------------------
void __stdcall sub_15009590(int a1)
{
	SDrawDestroy();
	PostQuitMessage(0);
}

//----- (150095A0) --------------------------------------------------------
int __userpurge sub_150095A0/*eax*/(void* a1/*ecx*/, int a2/*edi*/, int a3)
{
	return SDrawCaptureScreen(a1, a2, 0);
}

//----- (150095B0) --------------------------------------------------------
LRESULT __stdcall sub_150095B0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result; // eax@3
	int v5;         // [sp+10h] [bp-8h]@1
	LRESULT v6;     // [sp+14h] [bp-4h]@1
	v5 = 0;
	v6 = 0;
	if( SMsgDispatchMessage((int)hWnd, Msg, wParam, lParam, (int)&v5, (int)&v6) && v5 )
		result = v6;
	else
		result = DefWindowProcA(hWnd, Msg, wParam, lParam);
	return result;
}

//----- (15009620) --------------------------------------------------------
int __userpurge SDrawCaptureScreen/*eax*/(void* a1/*ecx*/, int a2/*edi*/, char* Source)
{
	DWORD v3;          // esi@5
	HMODULE v5;        // eax@13
	int v6;            // eax@13
	int v7;            // [sp+14h] [bp-158h]@7
	int v8;            // [sp+18h] [bp-154h]@7
	OPENFILENAMEA Str; // [sp+1Ch] [bp-150h]@9
	__int16 v10;       // [sp+169h] [bp-3h]@9
	char v11;          // [sp+16Bh] [bp-1h]@9
	if( StormDdraw_1501EA20 && S_hWnd && StormSurface_1501EE40.mainSurface && dword_1501E110 == 8 ){
		SDrawSelectGdiSurface(a1, 1, 1);
		v3 = SMemAlloc(StormScreenHeight * StormScreenWidth, "SDRAW.CPP", 356, 0);
		if( StormPalette_1501EA24 )
			StormPalette_1501EA24->GetEntries(
			0,
			0,
			256,
			&S_pPalEntries);
		if( SDrawLockSurface(0, 0, (int)&v7, (int)&v8, 0) ){
			SBltROP3(v3, v7, StormScreenWidth, StormScreenHeight, StormScreenWidth, v8, 0, 0xCC0020u);
			SDrawUnlockSurface(a2, 0, v7, 0, 0);
		}
		LOBYTE_IDA(Str.pvReserved) = ZeroByte;
		memset((char*)&Str.pvReserved + 1, 0, 0x100u);
		v10 = 0;
		v11 = 0;
		if( Source && *Source ){
			strncpy((char*)&Str.pvReserved, Source, 0x104u);
			LABEL_12:
			SBmpSaveImage((char*)&Str.pvReserved, (int)&S_pPalEntries, v3, StormScreenWidth, StormScreenHeight, dword_1501E110);
			SMemFree((void*)v3, "SDRAW.CPP", 414, 0);
			return 1;
		}
		memset(&Str, 0, 0x4Cu);
		Str.lStructSize = 76;
		Str.hwndOwner = S_hWnd;
		v5 = GetModuleHandleA(0);
		Str.lpstrFilter = "PC Paintbrush (*.pcx)";
		Str.hInstance = v5;
		Str.nFilterIndex = 1;
		Str.lpstrFile = (LPSTR)&Str.pvReserved;
		Str.nMaxFile = 260;
		Str.lpstrTitle = "Save Screen Capture";
		Str.Flags = 2062;
		Str.lpstrDefExt = ".pcx";
		dword_1501EA10 = 1;
		v6 = GetSaveFileNameA(&Str);
		dword_1501EA10 = 0;
		if( v6 )
			goto LABEL_12;
		SMemFree((void*)v3, "SDRAW.CPP", 401, 0);
	}
	return 0;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
//----- (15009820) --------------------------------------------------------
int __stdcall SDrawShowCursor(int surfaceNumber)
{
	int v1;     // ebx@1
	int result; // eax@2
	int internalDisplayCounter;     // edi@5
	int v4;     // ecx@9
	int v5;     // edi@9
	int v6;     // edi@10
	v1 = surfaceNumber;

	if( surfaceNumber < 0 || surfaceNumber >= 4){
		SErrSetLastError(0x57u);
		result = 0;
	}

	if( StormDdraw_1501EA20 && StormSurface_1501EE40.Surfaces[surfaceNumber] ){
		internalDisplayCounter = 0;
		do{
			++internalDisplayCounter;
		}while( ShowCursor(0) >= 0 );
		if( SDrawLockSurface(surfaceNumber, 0, (int)&v1, 0, 0) ){
			SGdiRectangle(surfaceNumber, 0, 0, StormScreenWidth - 1, StormScreenHeight - 1, 0x1000000u);
			SDrawUnlockSurface(internalDisplayCounter, v1, surfaceNumber, 0, 0);
		}

		for( int i = 0; i < internalDisplayCounter; ++i){
			ShowCursor(1);
		}
		result = 1;
	}else{
		result = 0;
	}

	return result;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
//----- (150098E0) --------------------------------------------------------
int __stdcall SDrawDestroy()
{
	//if( ExternalStorm ) return eSDrawDestroy();
	int v0;     // edi@1
	int v1;     // edi@1
	int v2;     // edi@1
	int v3;     // eax@7
	int result; // eax@9
	v1 = SDlgDestroy() & 1;
	v2 = SGdiDestroy() & v1;
	v0 = SVidDestroy() & v2;
	if( hPal ){
		if( dword_1501EA08 ){
			dword_1501EA08 = 0;
			DeleteObject(hPal);
		}
	}
	dword_1501E110 = 8;
	StormScreenWidth = GUI_Width;
	StormScreenHeight = GUI_Height;
	if( dword_1501EA04 ){
		dword_1501EA04 = 0;
		if( hCursor )
			DestroyCursor(hCursor);
	}
	hPal = 0;
	StormPalette_1501EA24 = 0;

	for( int i = 0; i < 4; ++i){
		StormSurface_1501EE40.Surfaces[i] = NULL;
	}

	result = v0;
	StormDdraw_1501EA20 = 0;
	S_hWnd = 0;
	hCursor = 0;
	return result;
}

// 1501EA04: using guessed type int dword_1501EA04;
// 1501EA08: using guessed type int dword_1501EA08;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE4C: using guessed type int dword_1501EE4C;
//----- (15009990) --------------------------------------------------------
int __stdcall SDrawFlipPage()
{
	int v1;     // esi@5
	int result; // eax@11
	if( !dword_1501EA10 && StormDdraw_1501EA20 && StormSurface_1501EE40.mainSurface && StormSurface_1501EE40.backBuffer ){
		do{
			v1 = StormSurface_1501EE40.mainSurface->Flip(NULL, 1);
			if( v1 == DDERR_SURFACELOST && StormSurface_1501EE40.mainSurface ){
				if( StormSurface_1501EE40.mainSurface->Restore() )
					goto LABEL_12;
				InvalidateRect(S_hWnd, 0, 0);
			}
		}while( v1 == DDERR_WASSTILLDRAWING || v1 == DDERR_SURFACELOST );
		result = 1;
	}else{
		LABEL_12:
		result = 0;
	}
	return result;
}

// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
//----- (15009A10) --------------------------------------------------------
HWND __stdcall SDrawGetFrameWindow(int a1)
{
	//if( ExternalStorm ) return eSDrawGetFrameWindow(a1);
	if( a1 ){
		*(uint *)a1 = (uint)S_hWnd;
	}
	return S_hWnd;
}

//----- (15009A30) --------------------------------------------------------
int __stdcall SDrawGetObjects(LPDIRECTDRAW* dDrawPtr, LPDIRECTDRAWSURFACE* screenSurfacePtr, LPDIRECTDRAWSURFACE* backSurfacePtr, LPDIRECTDRAWSURFACE* offscreenSurfacePtr, int a5, LPDIRECTDRAWPALETTE* ddPalettePtr, HPALETTE* sysPalettePtr)
{
	if( dDrawPtr )
		*dDrawPtr = StormDdraw_1501EA20;
	if( screenSurfacePtr )
		*screenSurfacePtr = StormSurface_1501EE40.mainSurface;
	if( backSurfacePtr )
		*backSurfacePtr = StormSurface_1501EE40.backBuffer;
	if( offscreenSurfacePtr )
		*offscreenSurfacePtr = StormSurface_1501EE40.offscreenSurface;
	if( ddPalettePtr )
		*ddPalettePtr = StormPalette_1501EA24;
	if( sysPalettePtr )
		*sysPalettePtr = hPal;
	return StormDdraw_1501EA20 != 0;
}

// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
// 1501EE48: using guessed type int dword_1501EE48;
//----- (15009AA0) --------------------------------------------------------
int __stdcall SDrawGetScreenSize(int a1, int a2, int a3)
{
	int v3;   // ebx@1
	int v4;   // eax@3
	int v5;   // edi@6
	int v6;   // eax@8
	int v7;   // ebp@11
	HWND v8;  // eax@14
	HDC v9;   // esi@14
	HWND v10; // eax@14
	int v12;  // [sp+1Ch] [bp+Ch]@14
	v3 = a1;
	if( a1 ){
		if( StormDdraw_1501EA20 )
			v4 = StormScreenWidth;
		else
			v4 = GetSystemMetrics(0);
		*(uint*)a1 = v4;
	}
	v5 = a2;
	if( a2 ){
		if( StormDdraw_1501EA20 )
			v6 = StormScreenHeight;
		else
			v6 = GetSystemMetrics(1);
		*(uint*)a2 = v6;
	}
	v7 = a3;
	if( a3 ){
		if( StormDdraw_1501EA20 ){
			*(uint*)a3 = dword_1501E110;
		}else{
			v8 = GetDesktopWindow();
			v9 = GetDC(v8);
			v12 = GetDeviceCaps(v9, 12);
			*(uint*)v7 = GetDeviceCaps(v9, 14) * v12;
			v10 = GetDesktopWindow();
			ReleaseDC(v10, v9);
			v5 = a2;
			v3 = a1;
		}
	}
	return v3 || v5 || v7;
}

// 1501EA20: using guessed type int StormDdraw_1501EA20;
//----- (15009B70) --------------------------------------------------------
int __stdcall SDrawGetServiceLevel(int a1)
{
	int result; // eax@2
	if( a1 ){
		if( StormSurface_1501EE40.mainSurface ){
			if( StormSurface_1501EE40.offscreenSurface ){
				*(uint*)a1 = 3;
				result = 1;
			}else{
				*(uint*)a1 = StormSurface_1501EE40.backBuffer == NULL ? 1 : 2; //  (StormSurface_1501EE40.backBuffer != NULL) + 1;
				result = 1;
			}
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
// 1501EE48: using guessed type int dword_1501EE48;
//----- (15009BD0) --------------------------------------------------------
int __stdcall SDrawLockSurface( int surfaceNumber, LPRECT rect, int a3, int a4, int a5 )
{
	int v6;  // eax@10
	int v7;  // eax@26
	DDSURFACEDESC ddSurfaceDesc;  // [sp+Ch] [bp-6Ch]@9
	int v9;  // [sp+1Ch] [bp-5Ch]@26
	int v10; // [sp+30h] [bp-48h]@16
	
	if( surfaceNumber < 0 || surfaceNumber >= 4 || a3 == 0){
		SErrSetLastError(0x57u);
		return 0;
	}
	
	if( a3 )
		*(uint *)a3 = 0;
	if( a4 )
		*(uint *)a4 = 0;

	if( !StormDdraw_1501EA20 || !StormSurface_1501EE40.Surfaces[surfaceNumber] )
		return 0;
	memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
    ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
	while( 1 ){
		v6 = StormSurface_1501EE40.Surfaces[surfaceNumber]->Lock(
			rect,
			&ddSurfaceDesc,
			1,
			0);
		if( v6 == DDERR_SURFACELOST && StormSurface_1501EE40.mainSurface ){
			if( StormSurface_1501EE40.mainSurface->Restore() )
				return 0;
			InvalidateRect(S_hWnd, 0, 0);
			goto LABEL_17;
		}
		if( v6 && v6 != DDERR_WASSTILLDRAWING && !v10 )
			break;
		LABEL_17:
		if( v10 ){
			*(uint*)a3 = v10;
			v7 = v9;
			if( a4 )
				*(uint *)a4 = v9;
			SGdiSetPitch(v7);
			return 1;
		}
	}
	if( surfaceNumber || !StormSurface_1501EE40.unknownSurface || dword_1501EE28 )
		return 0;
	dword_1501EE28 = 1;
	if( rect ){
		StormRectForBlt_1501EE30 = *rect;
	}else{
		StormRectForBlt_1501EE30.left = 0;
		StormRectForBlt_1501EE30.top = 0;
		StormRectForBlt_1501EE30.right = StormScreenWidth;
		StormRectForBlt_1501EE30.bottom = StormScreenHeight;
	}
	return SDrawLockSurface(3, rect, a3, a4, a5);
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE28: using guessed type int dword_1501EE28;
// 1501EE30: using guessed type int dword_1501EE30;
// 1501EE34: using guessed type int dword_1501EE34;
// 1501EE38: using guessed type int dword_1501EE38;
// 1501EE3C: using guessed type int dword_1501EE3C;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE4C: using guessed type int dword_1501EE4C;
//----- (15009DB0) --------------------------------------------------------
int __stdcall SDrawManualInitialize( HWND hwnd, LPDIRECTDRAW ddrawPtr, LPDIRECTDRAWSURFACE screenSurfacePtr, LPDIRECTDRAWSURFACE (__fastcall *convertToTrueColorFunc)(uchar*), LPDIRECTDRAWSURFACE offscreenSurfacePtr, LPDIRECTDRAWSURFACE unknownSurfacePtr, LPDIRECTDRAWPALETTE ddrawPalettePtr, HPALETTE systemPalette )
{
	//if( ExternalStorm ) return eSDrawManualInitialize(hwnd, ddrawPtr, screenSurfacePtr, convertToTrueColorFunc, offscreenSurfacePtr, unknownSurfacePtr, ddrawPalettePtr, systemPalette);
	S_hWnd = hwnd;
	StormDdraw_1501EA20 = ddrawPtr;
	StormSurface_1501EE40.mainSurface = screenSurfacePtr;
	ConvertToTrueColorFunc = convertToTrueColorFunc; // 15039AA4 new storm 
	StormSurface_1501EE40.backBuffer = 0;
	StormSurface_1501EE40.offscreenSurface = offscreenSurfacePtr;
	StormSurface_1501EE40.unknownSurface = unknownSurfacePtr;
	hPal = systemPalette;
	dword_1501EA04 = 0;
	dword_1501EA10 = 0;
	StormPalette_1501EA24 = ddrawPalettePtr;
	if( ddrawPalettePtr && !systemPalette ){
		SelectPaletteToHwnd();
	}
	return 1;
}

// 1501EA04: using guessed type int dword_1501EA04;
// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
// 1501EE2C: using guessed type int (__stdcall *dword_1501EE2C)(_DWORD);
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
// 1501EE48: using guessed type int dword_1501EE48;
// 1501EE4C: using guessed type int dword_1501EE4C;
//----- (15009E20) --------------------------------------------------------
int __stdcall SDrawMessageBox(LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	//if( ExternalStorm ) return eSDrawMessageBox(lpText, lpCaption, uType);
	int v4;     // eax@2
	HGLOBAL v5; // edi@2
	int v6;     // esi@2
	int v7;     // eax@2
	HRSRC v8;   // eax@2
	HRSRC v9;   // esi@2
	HRSRC v10;  // ST20_4@2
	int v11;    // eax@2
	int v12;    // eax@2
	HWND v13;   // esi@6
	HWND i;     // eax@7
	int v15;    // esi@9
	PALETTEENTRY v17;   // [sp+14h] [bp-800h]@4
	PALETTEENTRY v18;   // [sp+414h] [bp-400h]@2
	SDrawSelectGdiSurface(0/*This*/, 1, 1);
	if( StormPalette_1501EA24 ){
			StormPalette_1501EA24,
		StormPalette_1501EA24->GetEntries(
			0,
			0,
			256,
			&v18);
		v7 = GetStormDllHandle();
		v8 = FindResourceA((HMODULE)v7, "SYSTEMPALETTE", "#256");
		v9 = v8;
		v10 = v8;
		v11 = GetStormDllHandle();
		v5 = LoadResource((HMODULE)v11, v10);
		v12 = GetStormDllHandle();
		v6 = SizeofResource((HMODULE)v12, v9);
		v4 = (int)LockResource(v5);
		if( v6 ){
			if( v4 ){
				SBmpDecodeImage(2, v4, v6, &v17, 0, 0, 0, 0, 0);
				StormPalette_1501EA24->SetEntries(
					0,
					0,
					256,
					&v17);
			}
		}
		FreeResource(v5);
	}
	v13 = S_hWnd;
	if( S_hWnd ){
		for( i = GetTopWindow(S_hWnd); i; i = GetTopWindow(i) )
			v13 = i;
	}
	dword_1501EA10 = 1;
	v15 = MessageBoxA(v13, lpText, lpCaption, uType);
	dword_1501EA10 = 0;
	if( StormPalette_1501EA24 )
		StormPalette_1501EA24->SetEntries(
		0,
		0,
		256,
		&v18);
	return v15;
}

// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA24: using guessed type int StormPalette_1501EA24;
//----- (15009F60) --------------------------------------------------------
HWND __stdcall SDrawPostClose()
{
	HWND result = S_hWnd;
	if( S_hWnd ){
		PostMessageA(S_hWnd, 0x10u, 0, 0);
		result = (HWND)1;
	}
	return result;
}

//----- (15009F80) --------------------------------------------------------
int __stdcall SDrawRealizePalette()
{
	//if( ExternalStorm ) return eSDrawRealizePalette();
	int v0;              // esi@3
	HWND v1;             // edi@6
	int v2;              // eax@8
	HDC v3;              // edi@14
	struct tagRECT Rect; // [sp+14h] [bp-10h]@9
	if( StormDdraw_1501EA20 && StormPalette_1501EA24 ){
		v0
		= StormPalette_1501EA24->SetEntries(
			0,
			0,
			256,
			&S_pPalEntries) == 0;
		if( !hPal || (SetPaletteEntries(hPal, 0, 0x100u, &S_pPalEntries), !hPal) || !S_hWnd )
			return v0;
		v1 = S_hWnd;
		if( GetWindowLongA(S_hWnd, -16) & 0x1000000 )
			goto LABEL_19;
		if( !(GetWindowLongA(v1, -20) & 8) ){
			v2 = 1;
			goto LABEL_13;
		}
		GetWindowRect(v1, &Rect);
		if( Rect.right >= GetSystemMetrics(16) || Rect.bottom >= GetSystemMetrics(17) )
			LABEL_19:
			v2 = 0;
		else
			v2 = 1;
		LABEL_13:
		if( v2 ){
			v3 = GetDC(S_hWnd);
			SelectPalette(v3, hPal, 0);
			RealizePalette(v3);
			ReleaseDC(S_hWnd, v3);
		}
		return v0;
	}
	return 0;
}

// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EA24: using guessed type int StormPalette_1501EA24;
//----- (1500A0A0) --------------------------------------------------------
int __thiscall SDrawSelectGdiSurface(void* This, int a2, int a3)
{
	int v3;     // ecx@5
	int result; // eax@8
	LPDIRECTDRAWSURFACE v5;   // [sp+8h] [bp-4h]@1
	if( !dword_1501EA10 && StormDdraw_1501EA20 && StormSurface_1501EE40.mainSurface && StormSurface_1501EE40.backBuffer ){
		v5 = NULL;
		StormDdraw_1501EA20->GetGDISurface(&v5);
		if( (v5 != StormSurface_1501EE40.mainSurface) == (a2 != 0) ){
			if( a3 )
				StormSurface_1501EE40.backBuffer->Blt(
				0,
				StormSurface_1501EE40.mainSurface,
				0,
				0,
				0);
			result = SDrawFlipPage();
		}else{
			result = 1;
		}
	}else{
		result = 0;
	}
	return result;
}

// 1501EA10: using guessed type int dword_1501EA10;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE44: using guessed type int dword_1501EE44;
//----- (1500A140) --------------------------------------------------------
int __stdcall SDrawUnlockSurface( int a1/*edi*/, int surfaceNumber, int a3, int sizeOfArray, LPRECT arrayOfRect )
{
	int v5;  // esi@1
	int v7;  // ST18_4@8
	int v8;  // eax@11
	LPRECT pArr; // esi@14
	int v11; // edx@16
    RECT tmpRect; // [sp+8h] [bp-10h]@16

	if( surfaceNumber < 0 || surfaceNumber >= 4){
		SErrSetLastError(0x57u);
		return 0;
	}

	v5 = surfaceNumber;
	
	if( !StormDdraw_1501EA20 )
		return 0;
	if( !StormSurface_1501EE40.Surfaces[surfaceNumber] )
		return 0;
	if( !surfaceNumber ){
		if( dword_1501EE28 )
			v5 = 3;
	}
	ConvertToTrueColorFunc((uchar*)v5);
	if( (*(int(__stdcall* *)(int))(*(uint*)v7 + 128))(v7) )
		return 0;
    dword_1501EE28 = 0;
	if( a1 != 3 || !dword_1501EE28 || !StormSurface_1501EE40.mainSurface )
		return 1;
	if( sizeOfArray == 0){
		StormSurface_1501EE40.mainSurface->Blt(
			&StormRectForBlt_1501EE30,
			StormSurface_1501EE40.unknownSurface,
			&StormRectForBlt_1501EE30,
			16777216,
			0);
		return 1;
	}

	pArr = arrayOfRect;

	for( int i = 0; i < sizeOfArray; ++i){
		tmpRect.top = pArr->top;
		tmpRect.left = pArr->left;
		tmpRect.right = pArr->right+1;
		tmpRect.bottom = pArr->bottom+1;
		StormSurface_1501EE40.mainSurface->Blt(
			&tmpRect,
			StormSurface_1501EE40.unknownSurface,
			&tmpRect,
			16777216,
			0);
		pArr++;
	}
	return 1;
}

// 1500A140: could not find valid save-restore pair for ebx
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE28: using guessed type int dword_1501EE28;
// 1501EE2C: using guessed type int (__stdcall *dword_1501EE2C)(_DWORD);
// 1501EE30: using guessed type int dword_1501EE30;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE4C: using guessed type int dword_1501EE4C;
//----- (1500A290) --------------------------------------------------------
int __stdcall SDrawUpdatePalette(UINT iStart, UINT cEntries, PALETTEENTRY* pPalEntries, int a4)
{
	//if( ExternalStorm ) return eSDrawUpdatePalette(iStart, cEntries, pPalEntries, a4);
	return 0;
}

//----- (1500A440) --------------------------------------------------------
int __userpurge SDrawUpdateScreen/*eax*/(int a1/*edi*/, LPRECT a2)
{
	int result; // eax@5
	int v3;     // ecx@5
	int v4;     // esi@6
	int v5;     // eax@8
	int v6;     // eax@10
	int v7;     // [sp+8h] [bp-74h]@7
	int v8;     // [sp+Ch] [bp-70h]@6
	int v9;     // [sp+10h] [bp-6Ch]@7
	int v10;    // [sp+14h] [bp-68h]@6
	DDBLTFX v11;    // [sp+18h] [bp-46h]@5
	int v12;    // [sp+1Ch] [bp-60h]@5
	if( StormDdraw_1501EA20 && StormSurface_1501EE40.mainSurface && StormSurface_1501EE40.offscreenSurface ){
		if( dword_1501EA00 ){
			memset(&v11, 0, sizeof(DDBLTFX));
			v11.dwSize = sizeof(DDBLTFX);
			v11.dwDDFX = 8;
			result = StormSurface_1501EE40.mainSurface->Blt(
				a2,
				StormSurface_1501EE40.offscreenSurface,
				a2,
				2560,
				&v11) == 0;
		}else{
			v4 = 0;
			if( SDrawLockSurface(0, 0, (int)&v8, (int)&v10, 0) ){
				if( SDrawLockSurface(2, 0, (int)&v7, (int)&v9, 0) ){
					v5 = StormScreenWidth;
					if( dword_1501E110 != 8 )
						v5 = dword_1501E110 * StormScreenWidth >> 3;
					SBltROP3(v8, v7, v5, StormScreenHeight, v10, v9, 0, 0xCC0020u);
					v4 = v6;
					SDrawUnlockSurface(a1, 2, v7, 0, 0);
				}
				SDrawUnlockSurface(a1, 0, v8, 0, 0);
			}
			result = v4;
		}
	}else{
		result = 0;
	}
	return result;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 1501EA00: using guessed type int dword_1501EA00;
// 1501EA20: using guessed type int StormDdraw_1501EA20;
// 1501EE40: using guessed type int StormSurface_1501EE40[];
// 1501EE48: using guessed type int dword_1501EE48;
//----- (1500A570) --------------------------------------------------------
int __stdcall sub_1500A570()
{
	int result; // eax@1
	result = 0;
	dword_1501EE68 = 0;
	dword_1501EE6C = 0;
	return result;
}

// 1501EE68: using guessed type int dword_1501EE68;
// 1501EE6C: using guessed type int dword_1501EE6C;
//----- (1500A580) --------------------------------------------------------
int __stdcall sub_1500A580()
{
	sub_1500A590();
	return AtExit((int)sub_1500A5B0);
}

//----- (1500A590) --------------------------------------------------------
void __stdcall sub_1500A590()
{
	InitializeCriticalSection(&SCriticalSection);
}

//----- (1500A5B0) --------------------------------------------------------
void __stdcall sub_1500A5B0()
{
	DeleteCriticalSection(&SCriticalSection);
}

//----- (1500A5C0) --------------------------------------------------------
int __stdcall SErrDisplayError(uint dwMessageId, int a2, uint exitCode, char* a4, int a5, uint uExitCode)
{
	HANDLE v6;                             // eax@1
	char* v7;                              // eax@6
	CHAR* v8;                              // ebx@7
	char* v9;                              // eax@7
	CHAR* v10;                             // ebx@7
	char* v11;                             // eax@7
	char* v12;                             // eax@10
	char* v13;                             // eax@12
	char* v14;                             // eax@13
	char* v15;                             // eax@15
	int v16;                               // ecx@18
	char* v17;                             // eax@22
	void* v18;                             // ebx@22
	int v19;                               // ecx@23
	char* v20;                             // edi@23
	char v21;                              // zf@25
	char v22;                              // cl@27
	unsigned int v23;                      // ecx@27
	int v24;                               // edi@27
	int v25;                               // esi@27
	char v26;                              // dl@27as
	void* v27;                             // esi@27
	int v28;                               // ecx@28
	char* v29;                             // edi@28
	char v30;                              // zf@30
	unsigned int v31;                      // ecx@32
	char v32;                              // al@32
	void* v33;                             // esi@32
	int v34;                               // esi@33
	char* v36;                             // [sp-4h] [bp-85Ch]@18
	CHAR Buffer;                           // [sp+10h] [bp-848h]@5
	char v38;                              // [sp+11h] [bp-847h]@5
	__int16 v39;                           // [sp+10Dh] [bp-74Bh]@5
	char v40;                              // [sp+10Fh] [bp-749h]@5
	char Caption;                          // [sp+110h] [bp-748h]@1
	char v42;                              // [sp+111h] [bp-747h]@1
	__int16 v43;                           // [sp+211h] [bp-647h]@1
	char v44;                              // [sp+213h] [bp-645h]@1
	CHAR FileName;                         // [sp+214h] [bp-644h]@1
	char v46;                              // [sp+215h] [bp-643h]@1
	__int16 v47;                           // [sp+315h] [bp-543h]@1
	char v48;                              // [sp+317h] [bp-541h]@1
	struct _WIN32_FIND_DATAA FindFileData; // [sp+318h] [bp-540h]@1
	char Text[1024];                       // [sp+458h] [bp-400h]@7
	EnterCriticalSection(&SCriticalSection);
	FileName = ZeroByte;
	Caption = ZeroByte;
	memset(&v46, 0, 0x100u);
	v47 = 0;
	v48 = 0;
	memset(&v42, 0, 0x100u);
	v43 = 0;
	v44 = 0;
	GetModuleFileNameA(0, &FileName, 0x104u);
	memset(&FindFileData, 0, sizeof(FindFileData));
	v6 = FindFirstFileA(&FileName, &FindFileData);
	if( v6 )
		FindClose(v6);
	strcpy(&Caption, FindFileData.cFileName);
	if( strrchr(&Caption, 46) )
		*strrchr(&Caption, 46) = 0;
	Buffer = ZeroByte;
	memset(&v38, 0, 0xFCu);
	v39 = 0;
	v40 = 0;
	SErrGetErrorStr(dwMessageId, &Buffer, 0x100u);
	if( !Buffer ){
		v7 = sub_1500A8E0(0);
		wsprintfA(&Buffer, v7, (unsigned __int16)dwMessageId, dwMessageId);
	}
	v9 = sub_1500A8E0(1);
	wsprintfA(Text, v9, &Buffer);
	v10 = &Text[strlen(Text) - 1];
	v11 = sub_1500A8E0(2);
	wsprintfA(v10, v11, &FileName);
	v8 = &v10[strlen(v10) - 1];
	if( a2 ){
		if( *(uchar*)a2 ){
			if( exitCode == -1 ){
				v12 = sub_1500A8E0(4);
				wsprintfA(v8, v12, a2);
			}else{
				if( exitCode == -2 ){
					v13 = sub_1500A8E0(5);
					wsprintfA(v8, v13, a2);
				}else{
					v14 = sub_1500A8E0(3);
					wsprintfA(v8, v14, a2, exitCode);
				}
			}
			v8 += strlen(v8) - 1;
		}
	}
	v15 = a4;
	if( dwMessageId == -2062548992 ){
		if( !a4 )
			v15 = &ZeroByte;
		v36 = v15;
		v16 = 6;
	}else{
		if( !a4 )
			v15 = &ZeroByte;
		v36 = v15;
		v16 = 7;
	}
	v17 = sub_1500A8E0(v16);
	wsprintfA(v8, v17, v36);
	v18 = &v8[strlen(v8) - 1];
	if( a5 ){
		v20 = sub_1500A8E0(9);
		v19 = -1;
		do{
			if( !v19 )
				break;
			v21 = *v20++ == 0;
			--v19;
		}while( !v21 );
		v23 = ~v19;
		v26 = v23;
		v27 = &v20[-v23];
		v23 >>= 2;
		memcpy(v18, v27, 4 * v23);
		v25 = (int)((char*)v27 + 4 * v23);
		v24 = (int)((char*)v18 + 4 * v23);
		v22 = v26;
	}else{
		v29 = sub_1500A8E0(8);
		v28 = -1;
		do{
			if( !v28 )
				break;
			v30 = *v29++ == 0;
			--v28;
		}while( !v30 );
		v31 = ~v28;
		v32 = v31;
		v33 = &v29[-v31];
		v31 >>= 2;
		memcpy(v18, v33, 4 * v31);
		v25 = (int)((char*)v33 + 4 * v31);
		v24 = (int)((char*)v18 + 4 * v31);
		v22 = v32;
	}
	memcpy((void*)v24, (void*)v25, v22 & 3);
	v34 = SDrawMessageBox(Text, &Caption, (a5 != 0 ? 4 : 0) | 0x12010) == 7;
	LeaveCriticalSection(&SCriticalSection);
	if( !v34 )
		ExitProcess(uExitCode);
	return 1;
}

// 1500A5C0: using guessed type CHAR Text[1024];
//----- (1500A8E0) --------------------------------------------------------
char* __fastcall sub_1500A8E0(int a1)
{
	char* result; // eax@1
	int v2;       // esi@1
	UINT v3;      // ST04_4@1
	int v4;       // eax@1
	char v5;      // zf@1
	v2 = a1;
	v3 = a1 + 20736;
	v4 = GetStormDllHandle();
	v5 = LoadStringA((HINSTANCE)v4, v3, Buffer, 256) == 0;
	result = Buffer;
	if( v5 )
		result = off_1501C500[v2];
	return result;
}

// 1501C500: using guessed type char *off_1501C500[10];
//----- (1500A920) --------------------------------------------------------
int __stdcall SErrGetErrorStr(DWORD dwMessageId, LPSTR lpBuffer, DWORD nSize)
{
	//if( ExternalStorm ) return eSErrGetErrorStr(dwMessageId, lpBuffer, nSize);
	int result; // eax@2
	int v4;     // eax@5
	DWORD v5;   // ecx@5
	void* v6;   // esi@5
	if( lpBuffer != 0 ? -(nSize != 0) : 0 ){
		EnterCriticalSection(&SCriticalSection);
		if( !dword_1501EF70 ){
			dword_1501EF70 = 1;
			sub_1500A9E0();
		}
		v4 = dword_1501EE68;
		v6 = 0;
		v5 = (dwMessageId >> 16) & 0xFFF;
		if( dword_1501EE68 ){
			do{
				if( *(ushort*)(v4 + 8) == (ushort)v5 )
					break;
				v4 = *(uint*)(v4 + 4);
			}while( v4 );
			if( v4 )
				v6 = *(void**)(v4 + 12);
		}
		LeaveCriticalSection(&SCriticalSection);
		*lpBuffer = 0;
		result = FormatMessageA(v6 != 0 ? 2048 : 4096, v6, dwMessageId, 0x400u, lpBuffer, nSize, 0) != 0;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EE68: using guessed type int dword_1501EE68;
// 1501EF70: using guessed type int dword_1501EF70;
//----- (1500A9E0) --------------------------------------------------------
int __stdcall sub_1500A9E0()
{
	sub_1500AA00(1296);
	sub_1500AA00(2166);
	return sub_1500AA00(2168);
}

//----- (1500AA00) --------------------------------------------------------
int __fastcall sub_1500AA00(__int16 a1)
{
	__int16 v1; // di@1
	int v2;     // esi@1
	int result; // eax@2
	v1 = a1;
	v2 = SMemAlloc(16, ".?AUMSGSRC@@", -2, 8);
	*(uint*)v2 = dword_1501EE6C;
	if( dword_1501EE68 ){
		*(uint*)(dword_1501EE6C + 4) = v2;
		dword_1501EE6C = v2;
		*(ushort*)(v2 + 8) = v1;
		result = GetStormDllHandle();
		*(uint*)(v2 + 12) = result;
	}else{
		dword_1501EE6C = v2;
		dword_1501EE68 = v2;
		*(ushort*)(v2 + 8) = v1;
		result = GetStormDllHandle();
		*(uint*)(v2 + 12) = result;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EE68: using guessed type int dword_1501EE68;
// 1501EE6C: using guessed type int dword_1501EE6C;
//----- (1500AA60) --------------------------------------------------------
DWORD __stdcall SErrGetLastError()
{
	return GetLastError();
}

//----- (1500AA70) --------------------------------------------------------
int __stdcall SErrAddErrorStr(__int16 a1, int a2, int a3)
{
	int v3; // eax@1
	EnterCriticalSection(&SCriticalSection);
	v3 = SMemAlloc(16, ".?AUMSGSRC@@", -2, 8);
	*(uint*)(v3 + 4) = dword_1501EE68;
	if( dword_1501EE68 )
		*(uint *)dword_1501EE68 = v3;
	else
		dword_1501EE6C = v3;
	dword_1501EE68 = v3;
	*(ushort*)(v3 + 8) = a1;
	*(uint*)(v3 + 12) = a2;
	LeaveCriticalSection(&SCriticalSection);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EE68: using guessed type int dword_1501EE68;
// 1501EE6C: using guessed type int dword_1501EE6C;
//----- (1500AAD0) --------------------------------------------------------
void __stdcall SErrSetLastError(DWORD dwErrCode)
{
	SetLastError(dwErrCode);
}

//----- (1500AAE0) --------------------------------------------------------
int __stdcall SEvtDestroy()
{
	unsigned int v0; // ebp@1
	int i;           // esi@2
	unsigned int v2; // edi@3
	int v3;          // eax@4
	int j;           // ecx@4
	int v5;          // ecx@4
	int v6;          // eax@5
	v0 = 0;
	dword_1501EF74 = 1;
	if( dword_1501EF7C ){
		do{
			for( i = *(uint*)(dword_1501EF78 + 4 * v0); i; i = *(uint*)(dword_1501EF78 + 4*v0) ){
				v2 = 0;
				if( *(uint*)(i + 12) ){
					do{
						v5 = *(uint*)(i + 8);
						v3 = *(uint*)(v5 + 4 * v2);
						for( j = v5 + 4 * v2; v3; v3 = *(uint*)(v6 + 4*v2) ){
							*(uint*)j = *(uint*)(v3 + 12);
							SMemFree((void*)v3, "SEVT.CPP", 70, 0);
							v6 = *(uint*)(i + 8);
							j = v6 + 4 * v2;
						}
						++v2;
					}while( v2 < *(uint*)(i + 12) );
				}
				*(uint*)(dword_1501EF78 + 4 * v0) = *(uint*)(i + 20);
				SMemFree((void*)i, "SEVT.CPP", 74, 0);
			}
			++v0;
		}while( v0 < dword_1501EF7C );
	}
	if( dword_1501EF78 )
		SMemFree((void*)dword_1501EF78, "SEVT.CPP", 78, 0);
	dword_1501EF78 = 0;
	dword_1501EF7C = 0;
	dword_1501EF80 = 0;
	return 1;
}

// 1501EF74: using guessed type int dword_1501EF74;
// 1501EF78: using guessed type int dword_1501EF78;
// 1501EF7C: using guessed type int dword_1501EF7C;
// 1501EF80: using guessed type int dword_1501EF80;
//----- (1500ABC0) --------------------------------------------------------
int __stdcall SEvtDispatch(int a1, int a2, int a3, int a4)
{
	int v4; // edi@1
	int v5; // eax@3
	int v6; // ecx@10
	int v7; // eax@11
	int v8; // eax@12
	int v9; // esi@14
	v4 = 0;
	if( dword_1501EF78 && dword_1501EF7C ){
		v5 = *(uint*)(dword_1501EF78 + 4 * ((dword_1501EF7C - 1) & (a1 ^ a2)));
		if( !v5 )
			return 0;
		do{
			if( *(uint*)v5 == a1 && *(uint*)(v5 + 4) == a2 )
				break;
			v5 = *(uint*)(v5 + 20);
		}while( v5 );
	}else{
		v5 = 0;
	}
	if( !v5 || (v6 = *(uint *)(v5 + 8), !v6) || (v7 = *(uint *)(v5 + 12), !v7) )
		return 0;
	v8 = *(uint*)(v6 + 4 * (a3&(v7 - 1)));
	dword_1501EF74 = 0;
	if( v8 ){
		do{
			if( dword_1501EF74 )
				break;
			v9 = *(uint*)(v8 + 12);
			if( *(uint*)v8 == a3 ){
				v4 = 1;
				(*(void (__thiscall **)(uint, int))(v8 + 8))(*(uint *)v8, a4);
			}
			v8 = v9;
		}while( v9 );
	}
	return v4;
}

// 1501EF74: using guessed type int dword_1501EF74;
// 1501EF78: using guessed type int dword_1501EF78;
// 1501EF7C: using guessed type int dword_1501EF7C;
//----- (1500AC70) --------------------------------------------------------
int __stdcall SEvtRegisterHandler(int a1, int a2, int a3, int a4, int a5)
{
	int v6;           // ecx@3
	int v7;           // ebp@3
	int v8;           // ebx@5
	int v9;           // edi@5
	int v10;          // eax@13
	int v11;          // edi@13
	void* v12;        // eax@16
	int v13;          // esi@16
	int v14;          // edx@17
	unsigned int v15; // ebx@18
	int v16;          // ecx@19
	int v17;          // edi@20
	int v18;          // edx@21
	int v19;          // eax@21
	int v20;          // esi@28
	int v21;          // eax@28
	int v22;          // eax@30
	int v23;          // ebx@30
	int v24;          // esi@33
	int v25;          // eax@34
	unsigned int v26; // ecx@35
	int v27;          // eax@36
	int v28;          // ebx@37
	int v29;          // edx@38
	int v30;          // ecx@38
	void* v31;        // eax@41
	int v32;          // esi@44
	int v33;          // eax@44
	int v34;          // [sp+0h] [bp-4h]@13
	int v35;          // [sp+8h] [bp+4h]@30
	unsigned int v36; // [sp+Ch] [bp+8h]@35
	if( !a5 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	v6 = dword_1501EF7C;
	v7 = a2;
	if( dword_1501EF78 && dword_1501EF7C ){
		v8 = a1;
		v9 = *(uint*)(dword_1501EF78 + 4 * ((dword_1501EF7C - 1) & (a1 ^ a2)));
		if( !v9 ){
			LABEL_12:
			if( dword_1501EF80 >= (unsigned int)dword_1501EF7C >> 1 ){
				v11 = 1;
				v10 = 2 * dword_1501EF80 + 2;
				v34 = 1;
				if( (unsigned int)v10 >= 1 ){
					do
					v11 *= 2;
					while( v11 <= (unsigned int)v10 );
					v34 = v11;
				}
				v13 = SMemAlloc(4 * v11, "SEVT.CPP", 140, 8);
				v12 = (void*)dword_1501EF78;
				if( dword_1501EF78 ){
					v14 = dword_1501EF7C;
					if( dword_1501EF7C ){
						v15 = 0;
						if( dword_1501EF7C ){
							do{
								v16 = *((uint*)v12 + v15);
								if( v16 ){
									v17 = v11 - 1;
									do{
										v18 = *(uint*)(v16 + 20);
										v19 = v17&(*(uint*)v16 ^ *(uint*)(v16 + 4));
										*(uint*)(v16 + 20) = *(uint*)(v13 + 4 * v19);
										*(uint*)(v13 + 4 * v19) = v16;
										v16 = v18;
									}while( v18 );
									v14 = dword_1501EF7C;
									v12 = (void*)dword_1501EF78;
									v11 = v34;
									v7 = a2;
								}
								++v15;
							}while( v15 < v14 );
						}
						v8 = a1;
					}
					if( v12 )
						SMemFree(v12, "SEVT.CPP", 157, 0);
				}
				v6 = v11;
				dword_1501EF78 = v13;
				dword_1501EF7C = v11;
			}
			v20 = (v6 - 1) & (v8 ^ v7);
			v21 = SMemAlloc(24, "SEVT.CPP", 166, 8);
			*(uint*)v21 = v8;
			*(uint*)(v21 + 4) = v7;
			*(uint*)(v21 + 20) = *(uint*)(dword_1501EF78 + 4 * v20);
			*(uint*)(dword_1501EF78 + 4 * v20) = v21;
			++dword_1501EF80;
			v9 = *(uint*)(dword_1501EF78 + 4 * v20);
			goto LABEL_29;
		}
		do{
			if( *(uint*)v9 == a1 && *(uint*)(v9 + 4) == a2 )
				break;
			v9 = *(uint*)(v9 + 20);
		}while( v9 );
	}else{
		v8 = a1;
		v9 = 0;
	}
	if( !v9 )
		goto LABEL_12;
	LABEL_29:
	if( *(uint*)(v9 + 16) >= *(uint*)(v9 + 12) >> 1 ){
		v23 = 1;
		v35 = 1;
		v22 = 2 * dword_1501EF80 + 2;
		if( (unsigned int)v22 >= 1 ){
			do
			v23 *= 2;
			while( v23 <= (unsigned int)v22 );
			v35 = v23;
		}
		v24 = SMemAlloc(4 * v23, "SEVT.CPP", 182, 8);
		if( *(uint*)(v9 + 8) ){
			v25 = *(uint*)(v9 + 12);
			if( v25 ){
				v26 = 0;
				v36 = 0;
				if( v25 ){
					do{
						v27 = *(uint*)(*(uint*)(v9 + 8) + 4 * v26);
						if( v27 ){
							v28 = v23 - 1;
							do{
								v29 = *(uint*)(v27 + 12);
								v30 = v28& *(uint*)v27;
								*(uint*)(v27 + 12) = *(uint*)(v24 + 4 * v30);
								*(uint*)(v24 + 4 * v30) = v27;
								v27 = v29;
							}while( v29 );
							v23 = v35;
							v26 = v36;
						}
						++v26;
						v36 = v26;
					}while( v26 < *(uint*)(v9 + 12) );
				}
			}
		}
		v31 = *(void**)(v9 + 8);
		if( v31 )
			SMemFree(v31, "SEVT.CPP", 199, 0);
		*(uint*)(v9 + 8) = v24;
		*(uint*)(v9 + 12) = v23;
	}
	v32 = a3&(*(uint*)(v9 + 12) - 1);
	v33 = SMemAlloc(16, "SEVT.CPP", 208, 8);
	*(uint*)v33 = a3;
	*(uint*)(v33 + 4) = a4;
	*(uint*)(v33 + 8) = a5;
	*(uint*)(v33 + 12) = *(uint*)(4 * v32 + *(uint*)(v9 + 8));
	*(uint*)(4 * v32 + *(uint*)(v9 + 8)) = v33;
	++*(uint*)(v9 + 16);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EF78: using guessed type int dword_1501EF78;
// 1501EF7C: using guessed type int dword_1501EF7C;
// 1501EF80: using guessed type int dword_1501EF80;
//----- (1500AF00) --------------------------------------------------------
int __stdcall SEvtUnregisterHandler(int a1, int a2, int a3, int a4)
{
	int i;      // eax@3
	int v5;     // edi@7
	int v6;     // edx@10
	int v7;     // ecx@11
	int result; // eax@12
	int v9;     // ecx@12
	int v10;    // esi@12
	int v11;    // ecx@12
	if( dword_1501EF78 && dword_1501EF7C ){
		for( i = *(uint*)(dword_1501EF78 + 4 * ((dword_1501EF7C - 1) & (a1 ^ a2))); i; i = *(uint*)(i + 20) ){
			if( *(uint*)i == a1 && *(uint*)(i + 4) == a2 )
				break;
		}
		v5 = i;
	}else{
		v5 = 0;
	}
	if( v5 && (v6 = *(uint*)(v5 + 8)) != 0 && (v7 = *(uint*)(v5 + 12)) != 0 ){
		result = 0;
		v11 = a3&(v7 - 1);
		v10 = v6 + 4 * v11;
		v9 = *(uint*)(v6 + 4 * v11);
		if( v9 ){
			do{
				if( *(uint*)v9 != a3 || a4 && *(uint*)(v9 + 8) != a4 ){
					v10 = v9 + 12;
				}else{
					*(uint*)v10 = *(uint*)(v9 + 12);
					SMemFree((void*)v9, "SEVT.CPP", 245, 0);
					dword_1501EF74 = 1;
					result = 1;
					--*(uint*)(v5 + 16);
				}
				v9 = *(uint*)v10;
			}while( *(uint*)v10 );
		}
	}else{
		result = 0;
	}
	return result;
}

// 1501EF74: using guessed type int dword_1501EF74;
// 1501EF78: using guessed type int dword_1501EF78;
// 1501EF7C: using guessed type int dword_1501EF7C;
//----- (1500AFD0) --------------------------------------------------------
int __stdcall SEvtUnregisterType(int a1, int a2)
{
	int v2;          // esi@3
	unsigned int v4; // edi@11
	int i;           // eax@12
	int v6;          // eax@12
	int v7;          // ecx@12
	int v8;          // eax@13
	int j;           // eax@15
	int v10;         // edi@15
	if( dword_1501EF78 && dword_1501EF7C ){
		v2 = *(uint*)(dword_1501EF78 + 4 * ((dword_1501EF7C - 1) & (a1 ^ a2)));
		if( !v2 )
			return 0;
		do{
			if( *(uint*)v2 == a1 && *(uint*)(v2 + 4) == a2 )
				break;
			v2 = *(uint*)(v2 + 20);
		}while( v2 );
	}else{
		v2 = 0;
	}
	if( !v2 )
		return 0;
	v4 = 0;
	if( *(uint*)(v2 + 12) ){
		do{
			v6 = *(uint*)(v2 + 8);
			v7 = v6 + 4 * v4;
			for( i = *(uint*)(v6 + 4 * v4); i; i = *(uint*)(v8 + 4*v4) ){
				*(uint*)v7 = *(uint*)(i + 12);
				SMemFree((void*)i, "SEVT.CPP", 269, 0);
				v8 = *(uint*)(v2 + 8);
				v7 = v8 + 4 * v4;
			}
			++v4;
		}while( v4 < *(uint*)(v2 + 12) );
	}
	SMemFree(*(void**)(v2 + 8), "SEVT.CPP", 272, 0);
	dword_1501EF74 = 1;
	v10 = dword_1501EF78 + 4 * ((dword_1501EF7C - 1) & (a1 ^ a2));
	for( j = *(uint*)v10; *(uint*)v10; j = *(uint*)v10 ){
		if( j == v2 ){
			*(uint*)v10 = *(uint*)(j + 20);
			SMemFree((void*)j, "SEVT.CPP", 283, 0);
			--dword_1501EF80;
		}else{
			v10 = j + 20;
		}
	}
	return 1;
}

// 1501EF74: using guessed type int dword_1501EF74;
// 1501EF78: using guessed type int dword_1501EF78;
// 1501EF7C: using guessed type int dword_1501EF7C;
// 1501EF80: using guessed type int dword_1501EF80;
//----- (1500B0F0) --------------------------------------------------------
int __stdcall sub_1500B0F0()
{
	int result; // eax@1
	result = 0;
	OpenedMpqArchives = 0;
	dword_1501EFA4 = 0;
	return result;
}

// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFA4: using guessed type int dword_1501EFA4;
//----- (1500B110) --------------------------------------------------------
int __stdcall sub_1500B110()
{
	int result; // eax@1
	result = 0;
	dword_1501EFB8 = 0;
	dword_1501EFBC = 0;
	return result;
}

// 1501EFB8: using guessed type int dword_1501EFB8;
// 1501EFBC: using guessed type int dword_1501EFBC;
//----- (1500B130) --------------------------------------------------------
int __stdcall sub_1500B130()
{
	int result; // eax@1
	result = 0;
	dword_1501EFB0 = 0;
	dword_1501EFB4 = 0;
	return result;
}

// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501EFB4: using guessed type int dword_1501EFB4;
//----- (1500B150) --------------------------------------------------------
int __stdcall sub_1500B150()
{
	int result; // eax@1
	result = 0;
	dword_1501EFA8 = 0;
	dword_1501EFAC = 0;
	return result;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFAC: using guessed type int dword_1501EFAC;
//----- (1500B160) --------------------------------------------------------
int __stdcall SetupCriticalStormFile()
{
	InitCriticalStormFile();
	return AtExit((int)DelCriticalStormFile);
}

//----- (1500B170) --------------------------------------------------------
void __stdcall InitCriticalStormFile()
{
	InitializeCriticalSection(&FileCriticalSection);
}

//----- (1500B190) --------------------------------------------------------
void __stdcall DelCriticalStormFile()
{
	DeleteCriticalSection(&FileCriticalSection);
}

//----- (1500B1A0) --------------------------------------------------------
int __stdcall SFileAuthenticateArchive(int a1, int a2)
{
	int v3;                // eax@5
	int v4;                // ebp@9
	int v5;                // edi@9
	int i;                 // esi@9
	int v7;                // ST50_4@11
	char v8;               // al@11
	int v9;                // ebx@12
	int v10;               // edi@12
	int v11;               // esi@12
	int v12;               // ST50_4@14
	char v13;              // al@14
	int v14;               // ebx@16
	int v15;               // edi@16
	int j;                 // esi@16
	int v17;               // ST50_4@18
	char v18;              // al@18
	int v19;               // eax@19
	int v20;               // edx@19
	int v21;               // ecx@19
	int v22;               // ebp@19
	int v23;               // esi@19
	int v24;               // edi@32
	FARPROC v25;           // eax@32
	FARPROC v26;           // eax@33
	FARPROC v27;           // eax@34
	FARPROC v28;           // eax@35
	FARPROC v29;           // eax@36
	FARPROC v30;           // eax@37
	FARPROC v31;           // eax@38
	FARPROC v32;           // eax@39
	FARPROC v33;           // eax@40
	HANDLE v34;            // eax@43
	int v35;               // ebp@45
	int v36;               // edi@45
	int k;                 // esi@45
	int v38;               // ST50_4@47
	char v39;              // al@47
	int v40;               // ebx@48
	int v41;               // edi@48
	int v42;               // esi@48
	int v43;               // ST50_4@50
	char v44;              // al@50
	int v45;               // ebx@52
	int v46;               // edi@52
	int l;                 // esi@52
	int v48;               // ST50_4@54
	char v49;              // al@54
	int v50;               // eax@55
	int v51;               // edx@55
	int v52;               // ecx@55
	int v53;               // ebp@55
	int v54;               // esi@55
	int v55;               // esi@64
	LPVOID v56;            // ebx@70
	HGLOBAL v57;           // ebp@70
	HRSRC v58;             // edi@70
	LPCSTR v59;            // ST4C_4@70
	int v60;               // eax@70
	int v61;               // eax@70
	int v62;               // edi@71
	int v63;               // eax@71
	DWORD v64;             // eax@71
	int v65;               // ebp@75
	void* v66;             // ebx@76
	int v67;               // eax@77
	int v68;               // ecx@77
	int v69;               // ebx@77
	int v70;               // esi@101
	int v71;               // [sp+64h] [bp-28h]@31
	HMODULE v72;           // [sp+68h] [bp-24h]@31
	int v73;               // [sp+6Ch] [bp-20h]@31
	LPCVOID lpBaseAddress; // [sp+70h] [bp-1Ch]@12
	int v75;               // [sp+74h] [bp-18h]@31
	int v76;               // [sp+78h] [bp-14h]@31
	int v77;               // [sp+7Ch] [bp-10h]@48
	HANDLE hObject;        // [sp+80h] [bp-Ch]@31
	int v79;               // [sp+84h] [bp-8h]@19
	unsigned int v80;      // [sp+88h] [bp-4h]@55
	int v81;               // [sp+90h] [bp+4h]@76
	if( a2 )
		*(uint *)a2 = 0;
	if( !a1 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	v3 = OpenedMpqArchives;
	if( !OpenedMpqArchives ){
		LABEL_8:
		SErrSetLastError(6u);
		return 0;
	}
	while( v3 != a1 ){
		v3 = *(uint*)(v3 + 4);
		if( !v3 )
			goto LABEL_8;
	}
	v5 = (int)"(signature)";
	v4 = 2146271213;
	for( i = -286331154; v5; i += 32*i + v4 + v8 + 3 ){
		if( !*(uchar*)v5 )
			break;
		v7 = *(uchar*)v5++;
		v8 = toupper(v7);
		v4 = (i + v4) ^ *(uint*)(MpqDecryptBuffer + 4 * v8);
	}
	v10 = (int)"(signature)";
	v9 = 2146271213;
	lpBaseAddress = (LPCVOID)2146271213;
	v11 = -286331154;
	if( "(signature)" ){
		do{
			if( !*(uchar*)v10 )
				break;
			v12 = *(uchar*)v10++;
			v13 = toupper(v12);
			v9 = (v11 + v9) ^ *(uint*)(MpqDecryptBuffer + 4 * v13 + 1024);
			v11 += 32 * v11 + v9 + v13 + 3;
		}while( v10 );
		lpBaseAddress = (LPCVOID)v9;
	}
	v15 = (int)"(signature)";
	v14 = 2146271213;
	for( j = -286331154; v15; j += 32*j + v14 + v18 + 3 ){
		if( !*(uchar*)v15 )
			break;
		v17 = *(uchar*)v15++;
		v18 = toupper(v17);
		v14 = (j + v14) ^ *(uint*)(MpqDecryptBuffer + 4 * v18 + 2048);
	}
	v20 = *(uint*)(a1 + 312);
	v23 = *(uint*)(*(uint*)(a1 + 308) + 24) - 1;
	v22 = v23 & v4;
	v79 = v22;
	v19 = 16 * v22 + v20;
	v21 = *(uint*)(16 * v22 + v20 + 12);
	if( v21 == -1 ){
		LABEL_26:
		v22 = -1;
	}else{
		while( *(LPCVOID*)v19 != lpBaseAddress || *(uint*)(v19 + 4) != v14 || v21 == -2 || *(uint*)(v19 + 8) ){
			v22 = v23&(v22 + 1);
			if( v22 != v79 ){
				v19 = v20 + 16 * v22;
				v21 = *(uint*)(v19 + 12);
				if( v21 != -1 )
					continue;
			}
			goto LABEL_26;
		}
	}
	if( v22 == -1 ){
		if( a2 )
			*(uint *)a2 = 1;
		SErrSetLastError(0x4DCu);
		return 0;
	}
	v71 = 0;
	v73 = 0;
	v76 = 0;
	hObject = 0;
	v75 = 0;
	v79 = 0;
	lpBaseAddress = 0;
	v72 = LoadLibraryA("advapi32.dll");
	if( !v72
	|| (v24 = SMemAlloc(36, "SFILE.CPP", 976, 0),
	v71 = v24,
	v25 = GetProcAddress(v72, "CryptAcquireContextA"),
	*(uint *)v24 = (uint)v25,
	!v25)
	|| (v26 = GetProcAddress(v72, "CryptCreateHash"), *(uint *)(v24 + 4) = (uint)v26, !v26)
	|| (v27 = GetProcAddress(v72, "CryptDestroyHash"), *(uint *)(v24 + 8) = (uint)v27, !v27)
	|| (v28 = GetProcAddress(v72, "CryptDestroyKey"), *(uint *)(v24 + 12) = (uint)v28, !v28)
	|| (v29 = GetProcAddress(v72, "CryptHashData"), *(uint *)(v24 + 16) = (uint)v29, !v29)
	|| (v30 = GetProcAddress(v72, "CryptImportKey"), *(uint *)(v24 + 20) = (uint)v30, !v30)
	|| (v31 = GetProcAddress(v72, "CryptReleaseContext"), *(uint *)(v24 + 24) = (uint)v31, !v31)
	|| (v32 = GetProcAddress(v72, "CryptSignHashA"), *(uint *)(v24 + 28) = (uint)v32, !v32)
	|| (v33 = GetProcAddress(v72, "CryptVerifySignatureA"), *(uint *)(v24 + 32) = (uint)v33, !v33)
	|| !(*(int (__thiscall **)(int, int *, uint, uint, int, uint))v24)(
	v24,
	&v75,
	(uint)"Blizzard_Storm",
	(uint)"Microsoft Base Cryptographic Provider v1.0",
	1,
	0)
	&& !(*(int (__stdcall **)(int *, uint, uint, int, int))v24)(
	&v75,
	(uint)"Blizzard_Storm",
	(uint)"Microsoft Base Cryptographic Provider v1.0",
	1,
	8)
	|| (v34 = CreateFileMappingA(*(HANDLE *)(a1 + 268), 0, 0x8000002u, 0, 0, 0), hObject = v34, !v34)
	|| (lpBaseAddress = MapViewOfFile(v34, 4u, 0, 0, 0), !lpBaseAddress) )
		goto LABEL_86;
	v36 = (int)"(signature)";
	v35 = 2146271213;
	for( k = -286331154; v36; k += 32*k + v35 + v39 + 3 ){
		if( !*(uchar*)v36 )
			break;
		v38 = *(uchar*)v36++;
		v39 = toupper(v38);
		v35 = (k + v35) ^ *(uint*)(MpqDecryptBuffer + 4 * v39);
	}
	v41 = (int)"(signature)";
	v40 = 2146271213;
	v77 = 2146271213;
	v42 = -286331154;
	if( "(signature)" ){
		do{
			if( !*(uchar*)v41 )
				break;
			v43 = *(uchar*)v41++;
			v44 = toupper(v43);
			v40 = (v42 + v40) ^ *(uint*)(MpqDecryptBuffer + 4 * v44 + 1024);
			v42 += 32 * v42 + v40 + v44 + 3;
		}while( v41 );
		v77 = v40;
	}
	v46 = (int)"(signature)";
	v45 = 2146271213;
	for( l = -286331154; v46; l += 32*l + v45 + v49 + 3 ){
		if( !*(uchar*)v46 )
			break;
		v48 = *(uchar*)v46++;
		v49 = toupper(v48);
		v45 = (l + v45) ^ *(uint*)(MpqDecryptBuffer + 4 * v49 + 2048);
	}
	v51 = *(uint*)(a1 + 312);
	v54 = *(uint*)(*(uint*)(a1 + 308) + 24) - 1;
	v53 = v54 & v35;
	v80 = v53;
	v50 = 16 * v53 + v51;
	v52 = *(uint*)(16 * v53 + v51 + 12);
	if( v52 == -1 ){
		LABEL_62:
		v53 = -1;
	}else{
		while( *(uint*)v50 != v77 || *(uint*)(v50 + 4) != v45 || v52 == -2 || *(uint*)(v50 + 8) ){
			v53 = v54&(v53 + 1);
			if( v53 != v80 ){
				v50 = v51 + 16 * v53;
				v52 = *(uint*)(v50 + 12);
				if( v52 != -1 )
					continue;
			}
			goto LABEL_62;
		}
	}
	if( v53 == -1 )
		goto LABEL_86;
	v55 = *(uint*)(a1 + 304) + 16 * *(uint*)(16 * v53 + 12 + v51);
	if( *(uint*)(v55 + 12) & 0x10100 || *(uint*)(v55 + 8) <= 8u || !*(uint*)v55 ){
		if( a2 )
			*(uint *)a2 = 2;
		goto LABEL_86;
	}
	v80 = *(uint*)((char*)lpBaseAddress + *(uint*)v55);
	if( v80 >= 1 ){
		if( a2 )
			*(uint *)a2 = 3;
		goto LABEL_86;
	}
	v59 = (& lpName)[8 * v80];
	v60 = GetStormDllHandle();
	v58 = FindResourceA((HMODULE)v60, v59, "#256");
	v61 = GetStormDllHandle();
	v57 = LoadResource((HMODULE)v61, v58);
	v56 = LockResource(v57);
	if( v56 ){
		v63 = GetStormDllHandle();
		v64 = SizeofResource((HMODULE)v63, v58);
		v62 = v71;
		(*(void (__stdcall **)(int, LPVOID, DWORD, uint, uint, int *))(v71 + 20))(v75, v56, v64, 0, 0, &v76);
	}else{
		v62 = v71;
	}
	FreeResource(v57);
	if( !v76 )
		goto LABEL_87;
	if( !(*(int (__stdcall **)(int, int, uint, uint, int *))(v62 + 4))(v75, 32771, 0, 0, &v73)
	|| (v65 = a1,
	!(*(int (__stdcall **)(int, char *, int, uint))(v62 + 16))(
	v73,
	(char *)lpBaseAddress + *(uint *)(a1 + 300),
	*(uint *)v55 - *(uint *)(a1 + 300),
	0))
	|| (v66 = (void *)SMemAlloc(*(uint *)(v55 + 8), "SFILE.CPP", 1052, 8),
	v81 = (*(int (__stdcall **)(int, void *, uint, uint))(v62 + 16))(v73, v66, *(uint *)(v55 + 8), 0),
	SMemFree(v66, "SFILE.CPP", 1057, 0),
	!v81)
	|| (v68 = *(uint *)(v55 + 8),
	v67 = *(uint *)(*(uint *)(v65 + 308) + 8) + *(uint *)(v65 + 300),
	v69 = *(uint *)v55 + v68,
	v69 < (unsigned int)v67)
	&& !(*(int (__stdcall **)(int, char *, int, uint))(v62 + 16))(
	v73,
	(char *)lpBaseAddress + v69,
	v67 - v68 - *(uint *)v55,
	0) )
		goto LABEL_86;
	if( !(*(int (__stdcall **)(int, char*, int, int, uint, uint))(v62 + 32))(
	v73,
	(char*)lpBaseAddress + *(uint *)v55 + 8,
	*(uint *)(v55 + 8) - 8,
	v76,
	0,
	0) ){
		if( a2 ){
			*(uint*)a2 = 2;
			goto LABEL_87;
		}
		LABEL_86:
		v62 = v71;
		goto LABEL_87;
	}
	v79 = 1;
	if( !a2 )
		goto LABEL_86;
	*(uint*)a2 = dword_1501C62C[2 * v80];
	LABEL_87:
	if( v73 )
		(*(void(__stdcall* *)(int))(v62 + 8))(v73);
	if( v76 )
		(*(void(__stdcall* *)(int))(v62 + 12))(v76);
	if( lpBaseAddress )
		UnmapViewOfFile(lpBaseAddress);
	if( hObject )
		CloseHandle(hObject);
	if( v75 ){
		(*(void (__stdcall **)(int, uint))(v62 + 24))(v75, 0);
		(*(void (__stdcall **)(int *, uint, uint, int, int))v62)(
			&v75,
			(uint)"Blizzard_Storm",
			(uint)"Microsoft Base Cryptographic Provider v1.0",
			1,
			16);
	}
	if( v62 )
		SMemFree((void*)v62, "SFILE.CPP", 1098, 0);
	if( v72 )
		FreeLibrary(v72);
	v70 = v79;
	if( !v79 )
		SErrSetLastError(0x4DCu);
	return v70;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C62C: using guessed type int dword_1501C62C[];
// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501F0D4: using guessed type int dword_1501F0D4;
//----- (1500B920) --------------------------------------------------------
int __stdcall SFileCloseArchive(void* mpq)
{
	//if( ExternalStorm ) return eSFileCloseArchive(mpq);
	int result; // eax@2
	int v2;     // eax@3
	int v3;     // eax@7
	int v4;     // edi@8
	void* v5;   // eax@11
	void* v6;   // eax@13
	void* v7;   // eax@15
	void* v8;   // eax@17
	int v9;     // eax@23
	if( mpq ){
		v2 = OpenedMpqArchives;
		if( OpenedMpqArchives ){
			while( (void*)v2 != mpq ){
				v2 = *(uint*)(v2 + 4);
				if( !v2 )
					goto LABEL_6;
			}
			v3 = dword_1501EFB0;
			if( dword_1501EFB0 ){
				do{
					v4 = *(uint*)(v3 + 4);
					if( *(void**)(v3 + 12) == mpq )
						SFileCloseFile((HANDLE)v3);
					v3 = v4;
				}while( v4 );
			}
			v5 = (void*)*((uint*)mpq + 73); // 292
			if( v5 )
				SMemFree(v5, "SFILE.CPP", 1133, 0);
			v6 = (void*)*((uint*)mpq + 76); // 304
			if( v6 )
				SMemFree(v6, "SFILE.CPP", 1135, 0);
			v7 = (void*)*((uint*)mpq + 77); // 308
			if( v7 )
				SMemFree(v7, "SFILE.CPP", 1137, 0);
			v8 = (void*)*((uint*)mpq + 78); // 312
			if( v8 )
				SMemFree(v8, "SFILE.CPP", 1139, 0);
			CloseHandle(*((HANDLE*)mpq + 67)); // 268
			EnterCriticalSection(&FileCriticalSection);
			if( *(uint*)mpq ){
				*(uint*)(*(uint*)mpq + 4) = *((uint*)mpq + 1);
			}else{
				if( (void*)OpenedMpqArchives == mpq )
					OpenedMpqArchives = *((uint*)mpq + 1);
			}
			v9 = *((uint*)mpq + 1);
			if( v9 ){
				*(uint*)v9 = *(uint*)mpq;
			}else{
				if( (void*)dword_1501EFA4 == mpq )
					dword_1501EFA4 = *(uint*)mpq;
			}
			*(uint*)mpq = 0;
			*((uint*)mpq + 1) = 0;
			SMemFree(mpq, ".?AUARCHIVEREC@@", -2, 0);
			LeaveCriticalSection(&FileCriticalSection);
			result = 1;
		}else{
			LABEL_6:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFA4: using guessed type int dword_1501EFA4;
// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500BA70) --------------------------------------------------------
int __stdcall SFileCloseFile(HANDLE fHandle)
{
	//if( ExternalStorm ) return eSFileCloseFile(fHandle);
	int result; // eax@2
	int v2;     // eax@3
	int v3;     // eax@11
	void* v4;   // eax@13
	void* v5;   // eax@15
	int v6;     // eax@21
	if( fHandle ){
		v2 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v2 != (int)fHandle ){
				v2 = *(uint*)(v2 + 4);
				if( !v2 )
					goto LABEL_6;
			}
			if( *(uint*)((int)fHandle + 44) )
				SFileDdaEnd((int)fHandle);
			if( *(uint*)((int)fHandle + 8) != -1 )
				CloseHandle(*(HANDLE*)((int)fHandle + 8));
			v3 = *(uint*)((int)fHandle + 12);
			if( v3 )
				*(uint *)(v3 + 280) = 0;
			v4 = *(void**)((int)fHandle + 36);
			if( v4 )
				SMemFree(v4, "SFILE.CPP", 1178, 0);
			v5 = *(void**)((int)fHandle + 48);
			if( v5 )
				SMemFree(v5, "SFILE.CPP", 1182, 0);
			EnterCriticalSection(&FileCriticalSection);
			if( *(uint*)fHandle ){
				*(uint*)(*(uint*)fHandle + 4) = *(uint*)((int)fHandle + 4);
			}else{
				if( dword_1501EFB0 == (int)fHandle )
					dword_1501EFB0 = *(uint*)((int)fHandle + 4);
			}
			v6 = *(uint*)((int)fHandle + 4);
			if( v6 ){
				*(uint*)v6 = *(uint*)fHandle;
			}else{
				if( dword_1501EFB4 == (int)fHandle )
					dword_1501EFB4 = *(uint*)fHandle;
			}
			*(uint*)fHandle = 0;
			*(uint*)((int)fHandle + 4) = 0;
			SMemFree((void*)fHandle, ".?AUFILEREC@@", -2, 0);
			LeaveCriticalSection(&FileCriticalSection);
			result = 1;
		}else{
			LABEL_6:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501EFB4: using guessed type int dword_1501EFB4;
//----- (1500BB80) --------------------------------------------------------
int __stdcall SFileDdaBegin(int a1, int a2, unsigned int a3)
{
	return SFileDdaBeginEx(a1, a2, a3, 0, 2147483647, 2147483647, 0);
}

//----- (1500BBB0) --------------------------------------------------------
int __stdcall SFileDdaBeginEx(int a1, int a2, unsigned int a3, unsigned int a4, int a5, int a6, int a7)
{
	//if( ExternalStorm ) return eSFileDdaBeginEx(a1, a2, a3, a4, a5, a6, a7);
	int v7;                // ebx@1
	int result;            // eax@2
	int v9;                // eax@5
	int v10;               // eax@15
	DWORD v11;             // ecx@15
	int v12;               // esi@33
	unsigned int v13;      // eax@36
	int v14;               // ecx@36
	int v15;               // edx@38
	int v16;               // edi@38
	int v17;               // edx@38
	int v18;               // esi@41
	int v19;               // [sp-8h] [bp-58h]@38
	int v20;               // [sp+Ch] [bp-44h]@25
	int v21;               // [sp+10h] [bp-40h]@36
	int v22;               // [sp+14h] [bp-3Ch]@29
	int v23;               // [sp+18h] [bp-38h]@31
	int v24;               // [sp+1Ch] [bp-34h]@31
	int v25;               // [sp+20h] [bp-30h]@31
	int* v26;              // [sp+24h] [bp-2Ch]@38
	int v27;               // [sp+28h] [bp-28h]@31
	int v28;               // [sp+2Ch] [bp-24h]@31
	int v29;               // [sp+30h] [bp-20h]@31
	int v30;               // [sp+34h] [bp-1Ch]@31
	__int16 v31;           // [sp+38h] [bp-18h]@31
	int NumberOfBytesRead; // [sp+3Ch] [bp-14h]@22
	int v33;               // [sp+44h] [bp-Ch]@24
	v7 = a2;
	if( !(a1 != 0 ? -(a2 != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( !dword_1501F0C8 ){
		SErrSetLastError(0x85100071u);
		return 0;
	}
	v9 = dword_1501EFB0;
	if( !dword_1501EFB0 ){
		LABEL_8:
		SErrSetLastError(6u);
		return 0;
	}
	while( v9 != a1 ){
		v9 = *(uint*)(v9 + 4);
		if( !v9 )
			goto LABEL_8;
	}
	if( *(uint*)(a1 + 44) )
		SFileDdaEnd(a1);
	if( *(uint*)(a1 + 8) != -1 || !*(uint*)(a1 + 12) ){
		SErrSetLastError(0x8510006Fu);
		return 0;
	}
	*(uint*)(a1 + 44) = 1;
	if( !dword_1501F0DC )
		dword_1501F0DC = SMemAlloc(16384, "SFILE.CPP", 1237, 0);
	v10 = *(uint*)(a1 + 12);
	v11 = *(uint*)(v10 + 272);
	if( !v11 ){
		*(uint*)(v10 + 272) = 1;
		if( hHandle == INVALID_HANDLE_VALUE )
			sub_1500BF40(v11);
	}
	if( a2 & 0x3FFF )
		v7 = 16384 - (a2 & 0x3FFF) + a2;
	if( (unsigned int)v7 < 0x8000 )
		v7 = 32768;
	SFileSetFilePointer((HANDLE)a1, 0, 0, 0);
	if( !SFileReadFile((HANDLE)a1, &NumberOfBytesRead, 0xCu, 0, 0) )
		return 0;
	if( NumberOfBytesRead != 1179011410 || v33 != 1163280727 ){
		SErrSetLastError(0xDu);
		return 0;
	}
	if( !sub_1500C510(a1, 544501094, (int)&v20) ){
		SErrSetLastError(0xDu);
		return 0;
	}
	if( (unsigned int)v20 < 0x10 ){
		SErrSetLastError(0xDu);
		return 0;
	}
	if( !SFileReadFile((HANDLE)a1, &v22, 0x10u, 0, 0) || SFileSetFilePointer((HANDLE)a1, v20 - 16, 0, 1u) == -1 )
		return 0;
	v27 = v22;
	v29 = v24;
	v28 = v23;
	v30 = v25;
	v31 = 0;
	if( sub_1500C510(a1, 1635017060, (int)&v20) ){
		EnterCriticalSection(&FileCriticalSection);
		v12 = SMemAlloc(60, ".?AUAUDIOSTREAM@@", -2, 8);
		*(uint*)v12 = dword_1501EFAC;
		if( dword_1501EFA8 ){
			*(uint*)(dword_1501EFAC + 4) = v12;
			dword_1501EFAC = v12;
		}else{
			dword_1501EFAC = v12;
			dword_1501EFA8 = v12;
		}
		*(uint*)(v12 + 8) = a1;
		*(uint*)(v12 + 52) = v7;
		*(uint*)(v12 + 16) = v29;
		v14 = v20;
		*(uint*)(v12 + 24) = 0;
		*(uint*)(v12 + 20) = (a3 >> 18) & 1;
		v13 = v14 - v21;
		if( a4 < v14 - v21 )
			v13 = a4;
		v15 = v13 + v21;
		*(uint*)(v12 + 36) = v14;
		*(uint*)(v12 + 32) = v15;
		*(uint*)(v12 + 40) = a5;
		*(uint*)(v12 + 44) = a6;
		v16 = v12 + 48;
		v23 = 0;
		*(uchar*)(v12 + 56) = ((HIWORD(v30) != 8) - 1) & 0x80;
		v24 = 0;
		v25 = 0;
		v22 = 20;
		v26 = &v27;
		v17 = *(uint*)dword_1501F0C8;
		v23 = 224;
		v24 = v7;
		(*(void (__stdcall **)(int, int *, int, uint))(v17 + 12))(dword_1501F0C8, &v22, v12 + 48, 0);
		if( *(uint*)(v12 + 48) ){
			if( *(uint*)(v12 + 40) != 2147483647 )
				(*(void (__stdcall **)(uint, uint))(**(uint **)(v12 + 48) + 60))(
				*(uint *)(v12 + 48),
				*(uint *)(v12 + 40));
			v18 = *(uint*)(v12 + 44);
			if( v18 != 2147483647 )
				(*(void (__stdcall **)(int, int))(**(uint **)v16 + 64))(v19, v18);
		}
		LeaveCriticalSection(&FileCriticalSection);
		SetEvent(hEvent);
		result = 1;
	}else{
		SErrSetLastError(0xDu);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFAC: using guessed type int dword_1501EFAC;
// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501F0C8: using guessed type int dword_1501F0C8;
// 1501F0DC: using guessed type int dword_1501F0DC;
int __userpurge StartAddress/*eax*/(int a1/*ebx*/, int a2/*ebp*/, int a3/*edi*/, int a4/*esi*/, int a5);
//----- (1500BF40) --------------------------------------------------------
void __thiscall sub_1500BF40(DWORD This)
{
	DWORD ThreadId; // [sp+0h] [bp-4h]@1
	ThreadId = This;
	if( hHandle == INVALID_HANDLE_VALUE ){
		hEvent = CreateEventA(0, 0, 0, 0);
		hHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, 0, 0, &ThreadId);
		SetThreadPriority(hHandle, 1);
	}
}

//----- (1500BF90) --------------------------------------------------------
int __userpurge StartAddress/*eax*/(int a1/*ebx*/, int a2/*ebp*/, int a3/*edi*/, int a4/*esi*/, int a5)
{
	int v5;           // eax@1
	int v6;           // esi@3
	int v7;           // ebp@4
	int v8;           // eax@5
	int v9;           // ecx@10
	unsigned int v10; // edi@10
	int v11;          // ecx@10
	int v12;          // ST28_4@10
	int v13;          // edx@11
	int* v14;         // ebx@13
	int v15;          // edi@13
	int v16;          // ST14_4@13
	int i;            // ecx@14
	int v18;          // ebx@14
	int v19;          // edi@14
	int v20;          // edi@14
	char v21;         // dl@14
	int v22;          // eax@14
	unsigned int v23; // ecx@14
	int v24;          // edx@17
	unsigned int v25; // ecx@17
	int v26;          // eax@20
	int v27;          // ecx@21
	unsigned int v28; // ebp@21
	int v29;          // ecx@21
	int v30;          // ST28_4@21
	int v31;          // edi@22
	int v32;          // ebx@24
	__int64 v33;      // qax@26
	int v34;          // eax@28
	unsigned int v35; // ecx@28
	unsigned int v36; // ebx@31
	int v37;          // edi@31
	int v38;          // ebp@34
	int v39;          // eax@35
	int v40;          // edx@38
	DWORD v41;        // eax@40
	int v42;          // ecx@40
	int v43;          // ebx@40
	int v44;          // ebp@40
	int v45;          // edi@41
	int v46;          // edx@42
	unsigned int v47; // esi@42
	int v48;          // esi@43
	int v49;          // edx@48
	int v50;          // edx@49
	int v51;          // ebp@55
	int v52;          // eax@61
	DWORD v53;        // esi@61
	int v54;          // ecx@61
	int v55;          // eax@61
	int v56;          // eax@66
	int v57;          // eax@67
	int v58;          // edx@67
	int v59;          // ebx@67
	int v60;          // esi@67
	unsigned int v61; // edi@69
	int v62;          // edx@71
	int j;            // ecx@71
	int v64;          // edi@71
	unsigned int v65; // ecx@71
	void* v66;        // edi@71
	char v67;         // si@71
	int v68;          // eax@71
	int v69;          // eax@74
	int v70;          // edx@75
	int v71;          // ecx@75
	int v72;          // ebx@75
	int v73;          // eax@78
	int v74;          // esi@78
	int v75;          // eax@83
	int v76;          // eax@92
	int v78;          // [sp+34h] [bp-2Ch]@2
	int v79;          // [sp+38h] [bp-28h]@2
	int v80;          // [sp+3Ch] [bp-24h]@2
	int v81;          // [sp+40h] [bp-20h]@2
	unsigned int v82; // [sp+44h] [bp-1Ch]@13
	unsigned int v83; // [sp+48h] [bp-18h]@13
	unsigned int v84; // [sp+4Ch] [bp-14h]@10
	int v85;          // [sp+50h] [bp-10h]@1
	int v86;          // [sp+54h] [bp-Ch]@1
	int v87;          // [sp+58h] [bp-8h]@10
	int v88;          // [sp+5Ch] [bp-4h]@3
	v5 = 0;
	v85 = 0;
	v86 = 0;
	if( !dword_1501F0C4 ){
		v81 = a3;
		v80 = a4;
		v79 = a2;
		v78 = a1;
		while( 1 ){
			v88 = 0;
			EnterCriticalSection(&FileCriticalSection);
			v6 = dword_1501EFA8;
			if( dword_1501EFA8 ){
				v7 = 0;
				do{
					v8 = *(uint*)(v6 + 24);
					if( v8 == 3 || v8 == 2 && *(uint*)(*(uint*)(v6 + 8) + 24) >= *(uint*)(v6 + 36) ){
						if( *(uint*)(v6 + 20) == v7 ){
							v11 = **(uint**)(v6 + 48);
							v12 = *(uint*)(v6 + 48);
							v84 = v7;
							v87 = v7;
							(*(void (__stdcall **)(int, unsigned int *, int *))(v11 + 16))(v12, &v84, &v87);
							v9 = *(uint*)(v6 + 12);
							v10 = *(uint*)(v6 + 52);
							if( (v84 - v9) % v10 >= 0x4000 ){
								v13 = v10 - v9;
								if( v10 - v9 > 0x4000 )
									v13 = 16384;
								v15 = **(uint**)(v6 + 48);
								v14 = (int*)&v83;
								v16 = *(uint*)(v6 + 48);
								v83 = v7;
								v82 = v13;
								if( !(*(int (__stdcall **)(int, int, int, unsigned int *, unsigned int *, int, int, int))(v15 + 44))(
								v16,
								v9,
								v13,
								&v83,
								&v82,
								v7,
								v7,
								v7) ){
									v20 = v83;
									LOBYTE_IDA(v14) = *(uchar*)(v6 + 56);
									BYTE1(v14) = *(uchar*)(v6 + 56);
									v21 = v82;
									v22 = (uint)v14 << 16;
                  LOWORD_IDA(v22) = (ushort)v14;
									v23 = v82 >> 2;
									memset((void*)v83, v22, 4 * (v82 >> 2));
									v19 = v20 + 4 * v23;
									for( i = v21 & 3; i; --i )
										*(uchar *)v19++ = v18;
									(*(void (__stdcall **)(uint, unsigned int, unsigned int, int, int))(**(uint **)(v6 + 48) + 76))(
										*(uint *)(v6 + 48),
										v83,
										v82,
										v7,
										v7);
									v25 = *(uint*)(v6 + 52);
									v24 = v82 + *(uint*)(v6 + 12);
									*(uint*)(v6 + 28) += v82;
									*(uint*)(v6 + 12) = v24;
									if( v24 >= v25 )
										*(uint *)(v6 + 12) = v24 - v25;
								}
							}
							*(uint*)(v6 + 24) = 3;
						}else{
							*(uint*)(*(uint*)(v6 + 8) + 24) = *(uint*)(v6 + 32);
						}
					}
					v26 = *(uint*)(v6 + 24);
					if( v26 == 2 ){
						v29 = **(uint**)(v6 + 48);
						v30 = *(uint*)(v6 + 48);
						v84 = v7;
						v87 = v7;
						(*(void (__stdcall **)(int, unsigned int *, int *))(v29 + 16))(v30, &v84, &v87);
						v27 = *(uint*)(v6 + 12);
						v28 = *(uint*)(v6 + 52);
						if( (v84 - v27) % v28 >= 0x4000 ){
							v31 = v28 - v27;
							if( v28 - v27 > 0x4000 )
								v31 = 16384;
							v32 = *(uint*)(v6 + 8);
							if( v31 >= (unsigned int)(*(uint*)(v6 + 36) - *(uint*)(v32 + 24)) )
								v31 = *(uint*)(v6 + 36) - *(uint*)(v32 + 24);
							v33 = (1000 * v28 - 131072000) / *(uint*)(v6 + 16);
							sub_1500CD60(v32, HIDWORD_IDA(v33), 0, *(uint*)(v6 + 48), v27, v6, v31, v33, 0, 0);
							*(uint*)(*(uint*)(v6 + 8) + 24) += v31;
							if( !*(uint*)(v6 + 20) )
								v31 = 16384;
							v35 = *(uint*)(v6 + 52);
							v34 = v31 + *(uint*)(v6 + 12);
							*(uint*)(v6 + 12) = v34;
							if( v34 >= v35 )
								*(uint *)(v6 + 12) = v34 - v35;
						}
					}else{
						if( v26 == v7 ){
							v36 = *(uint*)(v6 + 36);
							v37 = 0;
							if( *(uint*)(v6 + 52) < v36 )
								v36 = *(uint*)(v6 + 52);
							if( v36 ){
								v38 = 1000;
								do{
									v39 = v36 - v37;
									if( v36 - v37 > 0x4000 )
										v39 = 16384;
									v38 += 500;
									sub_1500CD60(*(uint*)(v6 + 8), 0, 0, *(uint*)(v6 + 48), v37, v6, v39, v38, 0, 0);
									v37 += 16384;
									*(uint*)(*(uint*)(v6 + 8) + 24) += 16384;
								}while( v37 < v36 );
							}
							v40 = (unsigned int)v37 % *(uint*)(v6 + 52);
							*(uint*)(v6 + 24) = 1;
							*(uint*)(v6 + 12) = v40;
						}
					}
					v6 = *(uint*)(v6 + 4);
					v7 = 0;
				}while( v6 );
			}
			v43 = 0;
			v44 = -1;
			v82 = 0;
			v83 = -1;
			v84 = -1;
			v41 = GetTickCount();
			v42 = dword_1501EFB8;
			if( dword_1501EFB8 ){
				do{
					v45 = *(uint*)(*(uint*)(v42 + 12) + 12);
					if( v45 == v85 ){
						v47 = *(uint*)(v42 + 20);
						v46 = v86 - *(uint*)(v45 + 288);
						if( v47 >= v46 )
							v48 = v47 - v46;
						else
							v48 = v47 + 134217728;
					}else{
						v48 = -2;
					}
					if( v48 <= v84 ){
						v84 = v48;
						v82 = v42;
					}
					v49 = *(uint*)(v42 + 24);
					if( v41 - v49 >= 0x7FFFFFFF )
						v50 = v49 - v41;
					else
						v50 = 0;
					if( v50 <= (unsigned int)v44 ){
						v44 = v50;
						v43 = v42;
					}
					v42 = *(uint*)(v42 + 4);
				}while( v42 );
				v83 = v44;
			}
			LeaveCriticalSection(&FileCriticalSection);
			v51 = 0;
			if( v43 && v83 <= 0x2BC ){
				v51 = v43;
			}else{
				if( v82 )
					v51 = v82;
			}
			if( v51 ){
				v52 = *(uint*)(v51 + 12);
				v53 = *(uint*)(v51 + 28);
				v86 = *(uint*)(v51 + 20);
				v54 = *(uint*)(v52 + 12);
				v55 = *(uint*)(v51 + 32);
				v85 = v54;
				if( v55 )
					v53 = dword_1501F0DC;
				if( v53 )
					*(uint *)(v51 + 52) = sub_1500C5A0(*(uint *)(v51 + 12), *(uint *)(v51 + 16), v53, *(uint *)(v51 + 44));
				else
					*(uint*)(v51 + 52) = 0;
				v56 = *(uint*)(v51 + 32);
				if( v56 ){
					v59 = v51 + 44;
					(*(void (__stdcall **)(int, uint, uint, int, int, uint, uint, uint, int, int, int, int))(*(uint *)v56 + 44))(
						v56,
						*(uint *)(v51 + 36),
						*(uint *)(v51 + 44),
						v51 + 28,
						v51 + 44,
						0,
						0,
						0,
						v78,
						v79,
						v80,
						v81);
					memcpy(*(void**)(v51 + 28), (void*)v53, *(uint*)(v51 + 52));
					v60 = *(uint*)(v51 + 40);
					v58 = *(uint*)(v51 + 36);
					v57 = *(uint*)(v60 + 52) - v58;
					if( (unsigned int)v57 > 0x4000 )
						v57 = 16384;
					v61 = *(uint*)(v51 + 52);
					if( v61 < v57 ){
						if( !*(uint*)(v60 + 20) ){
							LOBYTE_IDA(v58) = *(uchar*)(v60 + 56);
							v65 = v57 - v61;
							BYTE1(v58) = *(uchar*)(v60 + 56);
							v66 = (void*)(*(uint*)(v51 + 28) + v61);
							v67 = v65;
							v68 = v58 << 16;
							LOWORD_IDA(v68) = v58;
							v65 >>= 2;
							memset(v66, v68, 4 * v65);
							v64 = (int)((char*)v66 + 4 * v65);
							for( j = v67 & 3; j; --j )
								*(uchar *)v64++ = v62;
						}
					}
					v81 = 0;
					v80 = 0;
					v79 = *(uint*)v59;
					v78 = *(uint*)(v51 + 28);
					(*(void(__stdcall**)(uint))(**(uint**)(v51 + 32) + 76))(*(uint*)(v51 + 32));
					v69 = *(uint*)(v51 + 40);
					if( *(uint*)(v69 + 24) == 1 ){
						v72 = *(uint*)v59;
						v71 = *(uint*)(v51 + 16);
						v70 = v71 + v72;
						if( (unsigned int)(v71 + v72) >= *(uint*)(v69 + 52)
						|| (v70 = *(uint*)(v69 + 36), v72 + v71 - *(uint*)(v69 + 32) >= (unsigned int)v70) ){
							(*(void (__fastcall **)(uint, int, uint, uint, uint, int))(**(uint**)(v69 + 48) + 48))(
								**(uint**)(v69 + 48),
								v70,
								*(uint*)(v69 + 48),
								0,
								0,
								1);
							*(uint*)(*(uint*)(v51 + 40) + 24) = 2;
						}
					}
				}
				EnterCriticalSection(&FileCriticalSection);
				v73 = *(uint*)v51;
				v74 = 1;
				if( *(uint*)(v51 + 48) ){
					if( v73 ){
						*(uint*)(v73 + 4) = *(uint*)(v51 + 4);
					}else{
						if( dword_1501EFB8 == v51 )
							dword_1501EFB8 = *(uint*)(v51 + 4);
					}
					v75 = *(uint*)(v51 + 4);
					if( v75 ){
						*(uint*)v75 = *(uint*)v51;
					}else{
						if( dword_1501EFBC == v51 )
							dword_1501EFBC = *(uint*)v51;
					}
					*(uint*)v51 = 0;
					*(uint*)(v51 + 4) = 0;
					LeaveCriticalSection(&FileCriticalSection);
					SetEvent(*(HANDLE*)(v51 + 8));
				}else{
					if( v73 ){
						*(uint*)(v73 + 4) = *(uint*)(v51 + 4);
					}else{
						if( dword_1501EFB8 == v51 )
							dword_1501EFB8 = *(uint*)(v51 + 4);
					}
					v76 = *(uint*)(v51 + 4);
					if( v76 ){
						*(uint*)v76 = *(uint*)v51;
					}else{
						if( dword_1501EFBC == v51 )
							dword_1501EFBC = *(uint*)v51;
					}
					*(uint*)v51 = 0;
					*(uint*)(v51 + 4) = 0;
					SMemFree((void*)v51, ".?AUREQUEST@@", -2, 0);
					LeaveCriticalSection(&FileCriticalSection);
				}
			}else{
				v74 = v88;
			}
			if( !v74 ){
				WaitForSingleObject(hEvent, (dword_1501EFA8 != 0 ? 0xFB : 0) - 1);
				if( dword_1501F0C4 )
					break;
			}
		}
		v5 = 0;
	}
	dword_1501F0C4 = v5;
	return 0;
}

// 1500BF90: could not find valid save-restore pair for ebx
// 1500BF90: could not find valid save-restore pair for ebp
// 1500BF90: could not find valid save-restore pair for edi
// 1500BF90: could not find valid save-restore pair for esi
// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFB8: using guessed type int dword_1501EFB8;
// 1501EFBC: using guessed type int dword_1501EFBC;
// 1501F0C4: using guessed type int dword_1501F0C4;
// 1501F0DC: using guessed type int dword_1501F0DC;
//----- (1500C510) --------------------------------------------------------
int __fastcall sub_1500C510(int a1, int a2, int a3)
{
	int v3;                // edi@1
	int v4;                // esi@1
	int result;            // eax@4
	DWORD v6;              // eax@6
	int NumberOfBytesRead; // [sp+8h] [bp-8h]@1
	LONG lDistanceToMove;  // [sp+Ch] [bp-4h]@3
	v4 = a1;
	v3 = a2;
	if( SFileReadFile((HANDLE)a1, &NumberOfBytesRead, 8u, 0, 0) ){
		while( NumberOfBytesRead != v3 ){
			if( SFileSetFilePointer((HANDLE)v4, lDistanceToMove, 0, 1u) == -1 )
				goto LABEL_7;
			result = SFileReadFile((HANDLE)v4, &NumberOfBytesRead, 8u, 0, 0);
			if( !result )
				return result;
		}
		*(uint*)a3 = lDistanceToMove;
		v6 = SFileSetFilePointer((HANDLE)v4, 0, 0, 1u);
		*(uint*)(a3 + 4) = v6;
		result = v6 != -1;
	}else{
		LABEL_7:
		result = 0;
	}
	return result;
}

//----- (1500C5A0) --------------------------------------------------------
int __fastcall sub_1500C5A0(int a1, int a2, DWORD a3, unsigned int a4)
{
	unsigned int v4;  // eax@1
	int v5;           // ebp@1
	int v6;           // edi@1
	int result;       // eax@2
	unsigned int v8;  // eax@3
	unsigned int v9;  // ebx@3
	int v10;          // esi@5
	int v11;          // eax@6
	unsigned int v12; // edx@6
	int v13;          // edi@6
	int v14;          // edx@6
	DWORD v15;        // eax@12
	int v16;          // esi@12
	int v17;          // eax@15
	int v18;          // eax@20
	int v19;          // ecx@22
	unsigned int v20; // [sp+10h] [bp-8h]@5
	int v21;          // [sp+14h] [bp-4h]@1
	int v22;          // [sp+20h] [bp+8h]@6
	DWORD v23;        // [sp+20h] [bp+8h]@11
	v6 = a1;
	v5 = a2;
	v21 = a1;
	v4 = *(uint*)(*(uint*)(a1 + 16) + 8);
	if( v4 <= a2 )
		return 0;
	v9 = a4;
	v8 = v4 - a2;
	if( a4 >= v8 )
		v9 = v8;
	v20 = 0;
	v10 = *(uint*)(*(uint*)(a1 + 12) + 288);
	if( !(a2&(v10 - 1)) )
		goto LABEL_11;
	v13 = *(uint*)(a1 + 12);
	v22 = sub_1500CD00(a1, ~(v10 - 1) & a2);
	v14 = *(uint*)(v13 + 288);
	v11 = v5&(v14 - 1);
	v12 = v14 - v11;
	if( v9 < v12 )
		v12 = v9;
	v9 -= v12;
	memcpy((void*)a3, (void*)(v11 + *(uint*)(v13 + 292)), v12);
	a3 += v12;
	v10 = *(uint*)(*(uint*)(v21 + 12) + 288);
	if( v22 == v10 && v9 ){
		v6 = v21;
		v20 = v12;
		v5 += v12;
		LABEL_11:
		v23 = 0;
		if( !v9
		|| (v16 = v9 & ~(v10 - 1),
		v15 = sub_1500C720(v6, v5, a3, v16),
		v9 -= v16,
		v5 += v16,
		v23 = v15,
		a3 += v16,
		v16 == v15)
		&& v9 ){
			v17 = 0;
			if( v9 ){
				v17 = sub_1500CD00(v6, v5);
				memcpy((void*)a3, *(void**)(*(uint*)(v6 + 12) + 292), v9);
				if( v9 < v17 )
					v17 = v9;
			}
			result = v20 + v23 + v17;
		}else{
			result = v20 + v15;
		}
		return result;
	}
	v18 = v5&(v10 - 1);
	if( v22 >= (unsigned int)v18 ){
		v19 = v22 - v18;
		result = v12;
		if( v12 >= v19 )
			result = v19;
	}else{
		result = 0;
	}
	return result;
}

//----- (1500C720) --------------------------------------------------------
DWORD __fastcall sub_1500C720(int a1, int a2, DWORD a3, DWORD nNumberOfBytesToRead)
{
	int v4;                  // ecx@1
	int v5;                  // ebx@1
	int v6;                  // edi@1
	int v7;                  // esi@1
	int v8;                  // eax@1
	DWORD result;            // eax@2
	int v10;                 // eax@5
	int v11;                 // edx@5
	LONG v12;                // eax@7
	void* v13;               // ecx@9
	DWORD v14;               // eax@9
	int v15;                 // edx@9
	int v16;                 // eax@10
	int v17;                 // edx@10
	int v18;                 // ecx@10
	DWORD v19;               // edi@11
	int v20;                 // eax@12
	int v21;                 // ebp@12
	int v22;                 // edx@14
	DWORD v23;               // eax@14
	int v24;                 // eax@15
	int v25;                 // edx@15
	int v26;                 // ebp@15
	char v27;                // zf@15
	int v28;                 // esi@16
	unsigned int v29;        // ecx@16
	int v30;                 // eax@19
	LONG v31;                // edi@19
	int v32;                 // eax@21
	int v33;                 // ecx@22
	unsigned int v34;        // eax@23
	unsigned int v35;        // edx@23
	unsigned int v36;        // esi@23
	int v37;                 // ecx@24
	DWORD v38;               // eax@31
	unsigned int v39;        // edx@31
	unsigned int v40;        // edi@31
	int v41;                 // esi@31
	unsigned int v42;        // ecx@31
	int v43;                 // ebp@35
	int v44;                 // eax@35
	unsigned int v45;        // eax@36
	DWORD v46;               // ecx@37
	int v47;                 // ecx@40
	int v48;                 // edx@43
	int v49;                 // ecx@43
	DWORD v50;               // esi@43
	int v51;                 // edx@45
	int v52;                 // ebp@45
	char v54;                // cf@46
	int v55;                 // ecx@47
	unsigned int v56;        // eax@48
	DWORD v57;               // ecx@48
	int v58;                 // ecx@48
	unsigned int v59;        // edi@48
	unsigned int v60;        // ecx@48
	int v61;                 // ecx@52
	int v62;                 // esi@52
	int v63;                 // edx@52
	int v64;                 // eax@53
	int v65;                 // ebp@54
	DWORD lpBuffer;          // [sp+10h] [bp-1Ch]@9
	DWORD NumberOfBytesRead; // [sp+14h] [bp-18h]@21
	unsigned int v68;        // [sp+18h] [bp-14h]@1
	unsigned int v69;        // [sp+1Ch] [bp-10h]@31
	int v70;                 // [sp+20h] [bp-Ch]@15
	unsigned int v71;        // [sp+24h] [bp-8h]@15
	DWORD v72;               // [sp+28h] [bp-4h]@33
	v5 = a1;
	v4 = *(uint*)(a1 + 12);
	v7 = nNumberOfBytesToRead;
	v6 = a2;
	v8 = *(uint*)(v4 + 288) - 1;
	v68 = a2;
	if( (((v8 & a2) != 0) - 1) & (((v8 & nNumberOfBytesToRead) != 0) - 1) ){
		if( nNumberOfBytesToRead ){
			v10 = *(uint*)(v5 + 16);
			v11 = *(uint*)(v10 + 12);
			if( BYTE1(v11) & 1 ){
				if( !*(uint*)(v5 + 40) ){
					v12 = *(uint*)v10;
					if( v12 != *(uint*)(v4 + 316) )
						SetFilePointer(*(HANDLE*)(v4 + 268), v12, 0, 0);
					v13 = *(void**)(v5 + 36);
					v14 = 4 * *(uint*)(v5 + 32) + 4;
					v15 = *(uint*)(v5 + 12);
					lpBuffer = 0;
					ReadFile(*(HANDLE*)(v15 + 268), v13, v14, &lpBuffer, 0);
					if( *(uint*)(*(uint*)(v5 + 16) + 12) & 0x10000 ){
						v17 = *(uint*)(v5 + 36);
						v18 = *(uint*)(v5 + 20) - 1;
						v16 = -286331154;
						if( lpBuffer >> 2 ){
							v19 = lpBuffer >> 2;
							do{
								v17 += 4;
								v20 = *(uint*)(MpqDecryptBuffer + 4096 + 4 * (unsigned __int8)v18) + v16;
								v21 = (v18 + v20) ^ *(uint*)(v17 - 4);
								*(uint*)(v17 - 4) = v21;
								v16 = v20 + 32 * v20 + v21 + 3;
								v18 = ((unsigned int)v18 >> 11) | (((v18 ^ 0x7FF) << 21) + 286331153);
								--v19;
							}while( v19 );
							v6 = v68;
						}
					}
					v22 = *(uint*)(v5 + 16);
					v23 = lpBuffer;
					v7 = nNumberOfBytesToRead;
					*(uint*)(v5 + 40) = 1;
					*(uint*)(*(uint*)(v5 + 12) + 316) = *(uint*)v22 + v23;
				}
			}
			v25 = *(uint*)(v5 + 16);
			v26 = v6;
			v70 = v6;
			v24 = *(uint*)(v25 + 12) & 0x100;
			v27 = (*(uint*)(v25 + 12) & 0x100) == 0;
			v71 = *(uint*)(v25 + 12) & 0x100;
			if( !v27 ){
				v28 = *(uint*)(v5 + 36);
				v29 = *(uint*)(*(uint*)(v5 + 12) + 288);
				v26 = *(uint*)(v28 + 4 * v6 / v29);
				v70 = v26;
				v24 = v71;
				v7 = *(uint*)(v28 + 4 * (nNumberOfBytesToRead + v6) / v29) - v26;
			}
			lpBuffer = a3;
			if( v24 )
				lpBuffer = SMemAlloc(v7, "SFILE.CPP", 671, 0);
			v31 = **(uint**)(v5 + 16) + v26;
			v30 = *(uint*)(v5 + 12);
			if( v31 != *(uint*)(v30 + 316) )
				SetFilePointer(*(HANDLE*)(v30 + 268), v31, 0, 0);
			v32 = *(uint*)(v5 + 12);
			NumberOfBytesRead = 0;
			ReadFile(*(HANDLE*)(v32 + 268), (LPVOID)lpBuffer, v7, &NumberOfBytesRead, 0);
			*(uint*)(*(uint*)(v5 + 12) + 316) = v31 + NumberOfBytesRead;
			if( NumberOfBytesRead < v7 ){
				v33 = *(uint*)(*(uint*)(v5 + 16) + 12);
				if( BYTE1(v33) & 1 ){
					nNumberOfBytesToRead = 0;
					v36 = *(uint*)(*(uint*)(v5 + 12) + 288);
					v35 = *(uint*)(v5 + 32);
					v34 = v68 / v36 + 1;
					if( v34 <= v35 ){
						v37 = *(uint*)(v5 + 36) + 4 * v34;
						while( NumberOfBytesRead >= *(uint*)v37 - v26 ){
							++v34;
							v37 += 4;
							nNumberOfBytesToRead += v36;
							if( v34 > v35 )
								break;
							v26 = v70;
						}
					}
				}else{
					nNumberOfBytesToRead = NumberOfBytesRead;
				}
			}
			if( *(uint*)(*(uint*)(v5 + 16) + 12) & 0x10000 ){
				v41 = 0;
				v42 = *(uint*)(*(uint*)(v5 + 12) + 288);
				v71 = 0;
				v40 = v68 / v42;
				v69 = v68 / v42;
				v38 = (nNumberOfBytesToRead + v42 - 1) / v42;
				v39 = *(uint*)(v5 + 32);
				if( v68 / v42 < v39 ){
					while( 1 ){
						v72 = v38 - 1;
						if( !v38 )
							break;
						v43 = *(uint*)(v5 + 16);
						v44 = *(uint*)(v43 + 12);
						if( BYTE1(v44) & 1 ){
							v45 = *(uint*)(*(uint*)(v5 + 36) + 4 * v40 + 4) - *(uint*)(*(uint*)(v5 + 36) + 4 * v40);
						}else{
							v45 = nNumberOfBytesToRead - v41;
							v46 = *(uint*)(*(uint*)(v5 + 12) + 288);
							if( v46 < nNumberOfBytesToRead - v41 )
								v45 = *(uint*)(*(uint*)(v5 + 12) + 288);
							if( v40 == v39 - 1 ){
								v47 = (v46 - 1) & *(uint*)(v43 + 8);
								if( v47 ){
									if( v45 >= v47 )
										v45 = v47;
								}
							}
						}
						v49 = v40 + *(uint*)(v5 + 20);
						v50 = lpBuffer + v41;
						v48 = -286331154;
						if( v45 >> 2 ){
							v70 = v45 >> 2;
							do{
								v50 += 4;
								v51 = *(uint*)(MpqDecryptBuffer + 4096 + 4 * (unsigned __int8)v49) + v48;
								v52 = (v49 + v51) ^ *(uint*)(v50 - 4);
								*(uint*)(v50 - 4) = v52;
								v48 = v51 + 32 * v51 + v52 + 3;
								v49 = ((unsigned int)v49 >> 11) | (((v49 ^ 0x7FF) << 21) + 286331153);
							}while( v70-- != 1 );
						}
						v39 = *(uint*)(v5 + 32);
						v40 = v69 + 1;
						v41 = v45 + v71;
						v54 = v69++ + 1 < v39;
						v71 += v45;
						if( !v54 )
							break;
						v38 = v72;
					}
				}
			}
			v55 = *(uint*)(*(uint*)(v5 + 16) + 12);
			if( BYTE1(v55) & 1 ){
				v58 = *(uint*)(v5 + 12);
				v69 = 0;
				v70 = 0;
				v60 = *(uint*)(v58 + 288);
				v59 = v68 / v60;
				v71 = v59;
				v57 = (nNumberOfBytesToRead + v60 - 1) / v60;
				v56 = *(uint*)(v5 + 32);
				if( v59 < v56 ){
					while( 1 ){
						v72 = v57 - 1;
						if( !v57 )
							break;
						v61 = *(uint*)(v5 + 36);
						v63 = *(uint*)(v61 + 4 * v59);
						v62 = *(uint*)(v61 + 4 * v59 + 4) - v63;
						v68 = *(uint*)(v61 + 4 * v59 + 4) - v63;
						if( v59 == v56 - 1 ){
							v64 = *(uint*)(*(uint*)(v5 + 16) + 8);
							if( !v64 || (v65 = *(uint *)(*(uint *)(v5 + 12) + 288), (v65 - 1) & v64) )
								v65 = v64&(*(uint*)(*(uint*)(v5 + 12) + 288) - 1);
						}else{
							v65 = *(uint*)(*(uint*)(v5 + 12) + 288);
						}
						if( v65 <= (unsigned int)v62 ){
							memcpy((void*)(v69 + a3), (void*)(v70 + lpBuffer), v65);
							v59 = v71;
							v62 = v68;
						}else{
							sub_1500CC10(v70 + lpBuffer, a3 + v69, v65, v62);
						}
						v69 += v65;
						v56 = *(uint*)(v5 + 32);
						++v59;
						v70 += v62;
						v71 = v59;
						if( v59 >= v56 )
							break;
						v57 = v72;
					}
				}
				SMemFree((void*)lpBuffer, "SFILE.CPP", 749, 0);
			}
			result = nNumberOfBytesToRead;
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501F0D4: using guessed type int dword_1501F0D4;
//----- (1500CC10) --------------------------------------------------------
int __userpurge sub_1500CC10/*eax*/(int a1/*edx*/, int a2/*ecx*/, int a3/*ebp*/, int a4)
{
	int v4;  // eax@1
	int v5;  // edi@1
	int v6;  // esi@1
	int v8;  // [sp+8h] [bp-14h]@3
	int v9;  // [sp+Ch] [bp-10h]@3
	int v10; // [sp+10h] [bp-Ch]@3
	int v11; // [sp+14h] [bp-8h]@3
	int v12; // [sp+18h] [bp-4h]@3
	v4 = dword_1501F0D0;
	v6 = a1;
	v5 = a2;
	if( !dword_1501F0D0 ){
		v4 = SMemAlloc(12596, "SFILE.CPP", 535, dword_1501F0D0);
		dword_1501F0D0 = v4;
	}
	v12 = a4;
	v8 = v6;
	v9 = 0;
	v10 = v5;
	v11 = 0;
	sub_1501B0A0((int)sub_1500CCC0, (int)sub_1500CC80, a3, v4, (int)&v8);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501F0D0: using guessed type int dword_1501F0D0;
//----- (1500CC80) --------------------------------------------------------
int __fastcall sub_1500CC80(void* a1, unsigned int* a2, int a3)
{
	int result; // eax@1
	int v4;     // ebp@1
	v4 = *(uint*)(a3 + 4);
	result = *(uint*)(a3 + 16) - v4;
	if( *a2 < result )
		result = *a2;
	memcpy(a1, (void*)(v4 + *(uint*)a3), result);
	*(uint*)(a3 + 4) += result;
	return result;
}

//----- (1500CCC0) --------------------------------------------------------
int __fastcall sub_1500CCC0(void* a1, unsigned int* a2, int a3)
{
	int result; // eax@1
	result = a3;
	memcpy((void*)(*(uint*)(a3 + 12) + *(uint*)(a3 + 8)), a1, *a2);
	*(uint*)(result + 12) += *a2;
	return result;
}

//----- (1500CD00) --------------------------------------------------------
int __fastcall sub_1500CD00(int a1, int a2)
{
	int v2;     // eax@1
	int v3;     // esi@1
	int result; // eax@3
	v3 = a1;
	v2 = *(uint*)(a1 + 12);
	if( *(uint*)(v2 + 280) != a1 || *(uint*)(v2 + 284) != a2 ){
		*(uint*)(v2 + 280) = a1;
		*(uint*)(*(uint*)(a1 + 12) + 284) = a2;
		*(uint*)(*(uint*)(a1 + 12) + 296) = sub_1500C720(a1, a2, *(uint*)(*(uint*)(a1 + 12) + 292), *(uint*)(*(uint*)(a1 + 12) + 288));
		result = *(uint*)(*(uint*)(v3 + 12) + 296);
	}else{
		result = *(uint*)(v2 + 296);
	}
	return result;
}

//----- (1500CD60) --------------------------------------------------------
HANDLE __fastcall sub_1500CD60(int a1, unsigned int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
	int v10;          // ebx@1
	unsigned int v11; // edi@1
	int v12;          // eax@3
	HANDLE result;    // eax@6
	int v14;          // ebp@7
	int v15;          // esi@10
	v11 = a2;
	v10 = a1;
	if( a3 || a4 )
		v12 = -1;
	else
		v12 = 0;
	if( v12 ){
		v14 = -1;
		if( !a9 || (result = CreateEventA(0, 0, 0, 0), v14 = (int)result, result) ){
			EnterCriticalSection(&FileCriticalSection);
			v15 = SMemAlloc(56, ".?AUREQUEST@@", -2, 8);
			*(uint*)v15 = dword_1501EFBC;
			if( dword_1501EFB8 ){
				*(uint*)(dword_1501EFBC + 4) = v15;
				dword_1501EFBC = v15;
			}else{
				dword_1501EFBC = v15;
				dword_1501EFB8 = v15;
			}
			*(uint*)(v15 + 8) = v14;
			*(uint*)(v15 + 12) = v10;
			*(uint*)(v15 + 16) = *(uint*)(v10 + 24) + v11;
			*(uint*)(v15 + 20) = *(uint*)(v10 + 24) + (v11 >> 1) + **(uint**)(v10 + 16);
			*(uint*)(v15 + 24) = a8 + GetTickCount();
			*(uint*)(v15 + 28) = a3;
			*(uint*)(v15 + 32) = a4;
			*(uint*)(v15 + 36) = a5;
			*(uint*)(v15 + 40) = a6;
			*(uint*)(v15 + 44) = a7;
			*(uint*)(v15 + 48) = a9;
			LeaveCriticalSection(&FileCriticalSection);
			if( a10 )
				SetEvent(hEvent);
			result = (HANDLE)v15;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFB8: using guessed type int dword_1501EFB8;
// 1501EFBC: using guessed type int dword_1501EFBC;
//----- (1500CE90) --------------------------------------------------------
int __stdcall SFileDdaDestroy()
{
	//if( ExternalStorm ) return eSFileDdaDestroy();
	int i;   // eax@1
	int* v1; // esi@2
	EnterCriticalSection(&FileCriticalSection);
	for( i = dword_1501EFA8; dword_1501EFA8; i = dword_1501EFA8 ){
		v1 = (int*)(i + 8);
		*(uint*)(*(uint*)(i + 8) + 44) = 1;
		LeaveCriticalSection(&FileCriticalSection);
		SFileDdaEnd(*v1);
		EnterCriticalSection(&FileCriticalSection);
	}
	LeaveCriticalSection(&FileCriticalSection);
	dword_1501F0C8 = 0;
	if( dword_1501F0DC ){
		SMemFree((void*)dword_1501F0DC, "SFILE.CPP", 1355, 0);
		dword_1501F0DC = 0;
	}
	return 1;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501F0C8: using guessed type int dword_1501F0C8;
// 1501F0DC: using guessed type int dword_1501F0DC;
//----- (1500CF20) --------------------------------------------------------
int __stdcall SFileDdaEnd(int a1)
{
	//if( ExternalStorm ) return eSFileDdaEnd(a1);
	int v1;     // edi@1
	int result; // eax@2
	int v3;     // eax@3
	int v4;     // ebp@7
	int v5;     // esi@7
	int v6;     // edi@8
	int v7;     // eax@9
	int v8;     // ecx@9
	int v9;     // eax@17
	int v10;    // eax@23
	v1 = a1;
	if( !a1 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	v3 = dword_1501EFB0;
	if( dword_1501EFB0 ){
		while( v3 != a1 ){
			v3 = *(uint*)(v3 + 4);
			if( !v3 )
				goto LABEL_6;
		}
		do{
			v4 = 0;
			EnterCriticalSection(&FileCriticalSection);
			v5 = dword_1501EFA8;
			if( dword_1501EFA8 ){
				while( 1 ){
					v6 = *(uint*)(v5 + 4);
					if( *(uint*)(v5 + 8) == a1 ){
						*(uint*)(v5 + 24) = 3;
						v7 = dword_1501EFB8;
						v8 = 0;
						if( dword_1501EFB8 ){
							while( !v8 ){
								if( *(uint*)(v7 + 40) == v5 || *(uint*)(v7 + 32) == *(uint*)(v5 + 48) )
									v8 = 1;
								v7 = *(uint*)(v7 + 4);
								if( !v7 )
									goto LABEL_15;
							}
						}else{
							LABEL_15:
							if( !v8 ){
								v9 = *(uint*)(v5 + 48);
								if( v9 )
									(*(void(__stdcall* *)(uint))(*(uint*)v9 + 8))(*(uint*)(v5 + 48));
								v6 = *(uint*)(v5 + 4);
								if( *(uint*)v5 ){
									*(uint*)(*(uint*)v5 + 4) = *(uint*)(v5 + 4);
								}else{
									if( dword_1501EFA8 == v5 )
										dword_1501EFA8 = *(uint*)(v5 + 4);
								}
								v10 = *(uint*)(v5 + 4);
								if( v10 ){
									*(uint*)v10 = *(uint*)v5;
								}else{
									if( dword_1501EFAC == v5 )
										dword_1501EFAC = *(uint*)v5;
								}
								*(uint*)v5 = 0;
								*(uint*)(v5 + 4) = 0;
								SMemFree((void*)v5, ".?AUAUDIOSTREAM@@", -2, 0);
								goto LABEL_28;
							}
						}
						v4 = 1;
					}
					LABEL_28:
					v5 = v6;
					if( !v6 ){
						v1 = a1;
						break;
					}
				}
			}
			LeaveCriticalSection(&FileCriticalSection);
			if( !v4 )
				break;
			Sleep(1u);
		}while( v4 );
		*(uint*)(v1 + 44) = 0;
		result = 1;
	}else{
		LABEL_6:
		SErrSetLastError(6u);
		result = 0;
	}
	return result;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFAC: using guessed type int dword_1501EFAC;
// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501EFB8: using guessed type int dword_1501EFB8;
//----- (1500D060) --------------------------------------------------------
int __stdcall SFileDdaGetPos(int a2, int a3, int a4)
{
	//if( ExternalStorm ) return eSFileDdaGetPos(a2, a3, a4);
	int v4;           // ebx@3
	int v5;           // edi@5
	int result;       // eax@6
	int v7;           // eax@7
	int v8;           // esi@11
	unsigned int v9;  // eax@16
	int v10;          // ecx@16
	int v11;          // ebx@18
	unsigned int v12; // edi@18
	int v13;          // ecx@18
	int v14;          // ST00_4@18
	void* v15;        // [sp+4h] [bp-4h]@1
	v15 = 0/*This*/;
	if( a3 )
		*(uint *)a3 = 0;
	v4 = a4;
	if( a4 )
		*(uint *)a4 = 0;
	v5 = a2;
	if( a2 ){
		v7 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v7 != a2 ){
				v7 = *(uint*)(v7 + 4);
				if( !v7 )
					goto LABEL_10;
			}
			EnterCriticalSection(&FileCriticalSection);
			v8 = dword_1501EFA8;
			if( !dword_1501EFA8 )
				goto LABEL_32;
			do{
				if( *(uint*)(v8 + 8) == v5 )
					break;
				v8 = *(uint*)(v8 + 4);
			}while( v8 );
			if( v8 ){
				v10 = *(uint*)(v8 + 24);
				v9 = 0;
				if( v10 == 2 || v10 == 3 ){
					v13 = **(uint**)(v8 + 48);
					v14 = *(uint*)(v8 + 48);
					a2 = 0;
					v15 = 0;
					(*(void (__stdcall **)(int, int *, void **))(v13 + 16))(v14, &a2, &v15);
					v11 = *(uint*)(v8 + 32);
					v12 = *(uint*)(v8 + 52);
					v9 = a2 + v12 * (*(uint*)(v8 + 28) + *(uint*)(*(uint*)(v8 + 8) + 24) - v11) / v12;
					if( v9 >= *(uint*)(v8 + 28) + *(uint*)(*(uint*)(v8 + 8) + 24) - v11 ){
						if( v9 <= v12 )
							v9 = 0;
						else
							v9 -= v12;
					}
					if( v9 >= *(uint*)(v8 + 36) - v11 )
						v9 = *(uint*)(v8 + 36) - v11;
					v4 = a4;
				}
				if( a3 )
					*(uint *)a3 = v9;
				if( v4 )
					*(uint *)v4 = *(uint *)(v8 + 36) - *(uint *)(v8 + 32);
				LeaveCriticalSection(&FileCriticalSection);
				result = 1;
			}else{
				LABEL_32:
				LeaveCriticalSection(&FileCriticalSection);
				SErrSetLastError(0x85100072u);
				result = 0;
			}
		}else{
			LABEL_10:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500D1A0) --------------------------------------------------------
int __stdcall SFileDdaGetVolume(int a1, int a2, int a3)
{
	int result; // eax@6
	int v4;     // eax@7
	int v5;     // esi@11
	int v6;     // edi@18
	int v7;     // [sp-8h] [bp-18h]@19
	if( a2 )
		*(uint *)a2 = 0;
	if( a3 )
		*(uint *)a3 = 0;
	if( a1 ){
		v4 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v4 != a1 ){
				v4 = *(uint*)(v4 + 4);
				if( !v4 )
					goto LABEL_10;
			}
			EnterCriticalSection(&FileCriticalSection);
			v5 = dword_1501EFA8;
			if( !dword_1501EFA8 )
				goto LABEL_27;
			do{
				if( *(uint*)(v5 + 8) == a1 )
					break;
				v5 = *(uint*)(v5 + 4);
			}while( v5 );
			if( v5 ){
				if( *(uint*)(v5 + 40) == 2147483647 )
					(*(void (__stdcall **)(uint, int))(**(uint **)(v5 + 48) + 24))(*(uint *)(v5 + 48), v5 + 40);
				v6 = v5 + 44;
				if( *(uint*)(v5 + 44) == 2147483647 )
					(*(void (__stdcall **)(int, int))(**(uint **)(v5 + 48) + 28))(v7, v6);
				if( a2 )
					*(uint *)a2 = *(uint *)(v5 + 40);
				if( a3 )
					*(uint *)a3 = *(uint *)v6;
				LeaveCriticalSection(&FileCriticalSection);
				result = 1;
			}else{
				LABEL_27:
				LeaveCriticalSection(&FileCriticalSection);
				SErrSetLastError(0x85100072u);
				result = 0;
			}
		}else{
			LABEL_10:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500D2A0) --------------------------------------------------------
int __stdcall SFileDdaInitialize(int a1)
{
	//if( ExternalStorm ) return eSFileDdaInitialize(a1);
	int result; // eax@2
	if( a1 ){
		dword_1501F0C8 = a1;
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501F0C8: using guessed type int dword_1501F0C8;
//----- (1500D2D0) --------------------------------------------------------
int __stdcall SFileDdaSetVolume(int a1, int a2, int a3)
{
	//if( ExternalStorm ) return eSFileDdaSetVolume(a1, a2, a3);
	int result; // eax@2
	int v4;     // eax@3
	int v5;     // esi@7
	int v6;     // [sp-8h] [bp-10h]@17
	if( a1 ){
		v4 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v4 != a1 ){
				v4 = *(uint*)(v4 + 4);
				if( !v4 )
					goto LABEL_6;
			}
			EnterCriticalSection(&FileCriticalSection);
			v5 = dword_1501EFA8;
			if( !dword_1501EFA8 )
				goto LABEL_21;
			do{
				if( *(uint*)(v5 + 8) == a1 )
					break;
				v5 = *(uint*)(v5 + 4);
			}while( v5 );
			if( v5 ){
				if( a2 != 2147483647 ){
					if( a2 != *(uint*)(v5 + 40) ){
						v6 = *(uint*)(v5 + 48);
						*(uint*)(v5 + 40) = a2;
						(*(void (__stdcall **)(int, int))(*(uint *)v6 + 60))(v6, a2);
					}
				}
				if( a3 != 2147483647 ){
					if( a3 != *(uint*)(v5 + 44) ){
						*(uint*)(v5 + 44) = a3;
						(*(void (__stdcall **)(int, int))(**(uint **)(v5 + 48) + 64))(v6, a3);
					}
				}
				LeaveCriticalSection(&FileCriticalSection);
				result = 1;
			}else{
				LABEL_21:
				LeaveCriticalSection(&FileCriticalSection);
				SErrSetLastError(0x85100072u);
				result = 0;
			}
		}else{
			LABEL_6:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFA8: using guessed type int dword_1501EFA8;
// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500D3A0) --------------------------------------------------------
int __stdcall SFileDestroy()
{
	int i;   // eax@8
	void* j; // eax@10
	while( dword_1501EFB8 )
		Sleep(0xAu);
	if( hHandle != INVALID_HANDLE_VALUE ){
		dword_1501F0C4 = 1;
		SetEvent(hEvent);
		WaitForSingleObject(hHandle, 0xFFFFFFFFu);
		if( hEvent != INVALID_HANDLE_VALUE ){
			CloseHandle(hEvent);
			hEvent = INVALID_HANDLE_VALUE;
		}
		if( hHandle != INVALID_HANDLE_VALUE ){
			CloseHandle(hHandle);
			hHandle = INVALID_HANDLE_VALUE;
		}
	}
	SFileDdaDestroy();
	for( i = dword_1501EFB0; dword_1501EFB0; i = dword_1501EFB0 )
		SFileCloseFile((HANDLE)i);
	for( j = (void*)OpenedMpqArchives; OpenedMpqArchives; j = (void*)OpenedMpqArchives )
		SFileCloseArchive(j);
	if( MpqDecryptBuffer ){
		SMemFree((void*)MpqDecryptBuffer, "SFILE.CPP", 1620, 0);
		MpqDecryptBuffer = 0;
	}
	if( dword_1501F0D0 ){
		SMemFree((void*)dword_1501F0D0, "SFILE.CPP", 1626, 0);
		dword_1501F0D0 = 0;
	}
	return 1;
}

// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501EFB8: using guessed type int dword_1501EFB8;
// 1501F0C4: using guessed type int dword_1501F0C4;
// 1501F0D0: using guessed type int dword_1501F0D0;
// 1501F0D4: using guessed type int dword_1501F0D4;
//----- (1500D4B0) --------------------------------------------------------
int __stdcall SFileEnableDirectAccess(int a1)
{
	//if( ExternalStorm ) return eSFileEnableDirectAccess( a1 );
	EnabledDirectAccess = a1;
	return 1;
}

// 1501F0CC: using guessed type int EnabledDirectAccess;
//----- (1500D4D0) --------------------------------------------------------
int __stdcall SFileGetFileArchive(HANDLE handle, int a2)
{
	//if( ExternalStorm ) return eSFileGetFileArchive(handle, a2);
	int result; // eax@2
	int v3;     // eax@3
	if( handle != 0 ? -(a2 != 0) : 0 ){
		v3 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v3 != (int)handle ){
				v3 = *(uint*)(v3 + 4);
				if( !v3 )
					goto LABEL_6;
			}
			result = 1;
			*(uint*)a2 = *(uint*)((int)handle + 12);
		}else{
			LABEL_6:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500D530) --------------------------------------------------------
DWORD __stdcall SFileGetFileSize(HANDLE fHandle, LPDWORD lpFileSizeHigh)
{
	//if( ExternalStorm ) return eSFileGetFileSize(fHandle, lpFileSizeHigh);
	DWORD result; // eax@4
	int v3;       // eax@5
	void* v4;     // eax@9
	if( lpFileSizeHigh )
		*lpFileSizeHigh = 0;
	if( fHandle ){
		v3 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v3 != (int)fHandle ){
				v3 = *(uint*)(v3 + 4);
				if( !v3 )
					goto LABEL_8;
			}
			v4 = *(void**)((int)fHandle + 8);
			if( v4 == (void*)-1 )
				result = *(uint*)(*(uint*)((int)fHandle + 16) + 8);
			else
				result = GetFileSize(v4, lpFileSizeHigh);
		}else{
			LABEL_8:
			SErrSetLastError(6u);
			result = -1;
		}
	}else{
		SErrSetLastError(0x57u);
		result = -1;
	}
	return result;
}

// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500D590) --------------------------------------------------------
// BOOL __stdcall SFileOpenArchive(
// 							 const char * szMpqName, // Archive file name
// 							 DWORD dwPriority,       // Archive priority
// 							 DWORD dwFlags,          // Open flags
// 							 HANDLE * phMPQ          // Pointer to result HANDLE
// 							 );
int __stdcall SFileOpenArchive(char* szMpqName, uint dwPriority, uint dwFlags, MPQ** mpq)
{
	//if( ExternalStorm ) return eSFileOpenArchive(szMpqName, dwPriority, dwFlags, (HANDLE*)mpq);
	if( mpq )
		*(uint *)mpq = 0;
	char* v4 = szMpqName;
	if( !(*szMpqName != 0 ? (mpq != 0 ? -(szMpqName != 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( !MpqDecryptBuffer ){
		MpqDecryptBuffer = SMemAlloc(5120, "SFILE.CPP", 1697, 0);
		if( MpqDecryptBuffer ){
			uint v6 = 1048577;
			int v7 = 0;
			do{
				int v9 = v7;
				int v8 = 5;
				do{
					v9 += 1024;
					uint v10 = 125 * v6 + 3;
					v6 = (125 * v10 % 0x2AAAAB + 3) % 0x2AAAAB;
					--v8;
					*(uint*)(MpqDecryptBuffer + v9 - 1024) = (unsigned __int16)v6 | ((unsigned __int16)(v10 % 0x2AAAAB) << 16);
				}while( v8 );
				v7 += 4;
			}while( v7 < 1024 );
			v4 = szMpqName;
		}
	}
	char fileName[260]; // [sp+2Ch] [bp-208h]@12
	fileName[0] = ZeroByte;
	memset(&fileName[1], 0, 259);
	strncpy(fileName, v4, 0x104u);
	fileName[259] = 0;
	if( GetFileAttributesA(fileName) & 0x10 ){
		if( *v4 == 92 || strstr(v4, ":\\") || strstr(v4, "\\\\") )
			strcpy(fileName, v4);
		else
			GetMpqFullPath(v4, fileName);
	}
	char RootPathName[4]; // [sp+10h] [bp-224h]@18
	strncpy(RootPathName, fileName, 3u);
	RootPathName[3] = 0;
	GetDriveTypeA(RootPathName);
	char fileSystemNameBuffer[260]; // [sp+130h] [bp-104h]@18
	memset(fileSystemNameBuffer, 0, 260);
	DWORD fileSystemFlags = 0;
	int v11 = GetVolumeInformationA(RootPathName, 0, 0, 0, 0, &fileSystemFlags, fileSystemNameBuffer, 260);
	DWORD hFile = 0;
	if( v11 ){
		DWORD sectorsPerCluster = 0;
		DWORD NumberOfFreeClusters = 0;
		DWORD totalNumberOfClusters = 0;
		v11 = GetDiskFreeSpaceA(RootPathName, &sectorsPerCluster, &hFile, &NumberOfFreeClusters, &totalNumberOfClusters);
		if( v11 )
			v11 = 1;
	}
	*(uint*)RootPathName = v11;
	if( dwFlags && !v11 ){
		SErrSetLastError(0xFu);
		return 0;
	}
	HANDLE v12 = CreateFileA(fileName, 0x80000000u, 1u, 0, 3u, 0, 0);
	hFile = (DWORD)v12;
	if( v12 == INVALID_HANDLE_VALUE )
		return 0;
	int newMpq = SMemAlloc(320, ".?AUARCHIVEREC@@", -2, 8);
	strcpy((char*)(newMpq + 8), fileName);
	int v14 = *(uint*)RootPathName;
	*(uint*)(newMpq + 268) = (uint)v12;
	*(uint*)(newMpq + 272) = v14;
	*(uint*)(newMpq + 276) = dwPriority;
	*(uint*)(newMpq + 308) = SMemAlloc(32, "SFILE.CPP", 1744, 0);
	*(uint*)(newMpq + 300) = 0;
	SetFilePointer(v12, 0, 0, 0);
	DWORD NumberOfBytesRead = 0;
	ReadFile(v12, *(LPVOID*)(newMpq + 308), 32u, &NumberOfBytesRead, 0);
	if( NumberOfBytesRead != 32 ){
		LABEL_30:
		SMemFree(*(void**)(newMpq + 308), "SFILE.CPP", 1752, 0);
		if( *(uint*)newMpq ){
			*(uint*)(*(uint*)newMpq + 4) = *(uint*)(newMpq + 4);
		}else{
			if( OpenedMpqArchives == (int)newMpq )
				OpenedMpqArchives = *(uint*)(newMpq + 4);
		}
		int v16 = *(uint*)(newMpq + 4);
		if( v16 ){
			*(uint*)v16 = *(uint*)newMpq;
		}else{
			if( dword_1501EFA4 == newMpq )
				dword_1501EFA4 = *(uint*)newMpq;
		}
		*(uint*)newMpq = 0;
		*(uint*)(newMpq + 4) = 0;
		SMemFree((void*)newMpq, ".?AUARCHIVEREC@@", -2, 0);
		CloseHandle(v12);
		SErrSetLastError(0x8510006Cu);
		return 0;
	}
	while( 1 ){
		int v15 = *(uint*)(newMpq + 308);
		if( *(uint*)v15 == 0x1A51504D )
			break;
		*(uint*)(newMpq + 300) += 512;
		if( *(uint*)v15 == 0x1A51504D )
			break;
		SetFilePointer(v12, *(uint*)(newMpq + 300), 0, 0);
		NumberOfBytesRead = 0;
		ReadFile(v12, *(LPVOID*)(newMpq + 308), 0x20u, &NumberOfBytesRead, 0);
		if( NumberOfBytesRead != 32 )
			goto LABEL_30;
	}
	char v20 = *(uchar*)(*(uint*)(newMpq + 308) + 14);
	*(uint*)(newMpq + 288) = 512 << v20;
	int v21 = SMemAlloc(512 << v20, "SFILE.CPP", 1764, 0);
	int v22 = *(uint*)(newMpq + 308);
	*(uint*)(newMpq + 292) = v21;
	int v23 = SMemAlloc(16 * *(uint*)(v22 + 24), "SFILE.CPP", 1767, 0);
	int v24 = *(uint*)(newMpq + 308);
	*(uint*)(newMpq + 312) = v23;
	uint v25 = hFile;
	SetFilePointer((HANDLE)hFile, *(uint*)(newMpq + 300) + *(uint*)(v24 + 16), 0, 0);
	ReadFile((HANDLE)v25, *(LPVOID*)(newMpq + 312), 16 * *(uint*)(*(uint*)(newMpq + 308) + 24), &NumberOfBytesRead, 0);
	int v17 = (int)"(hash table)";
	uint v18 = 0x7FED7FED;
	char v27;
	for( int i = 0xEEEEEEEE; v17; i += 32*i + v18 + v27 + 3 ){
		if( !*(uchar*)v17 )
			break;
		int v26 = *(uchar*)v17++;
		v27 = toupper(v26);
		v18 = (i + v18) ^ *(uint*)(MpqDecryptBuffer + 4 * v27 + 3072);
	}
	int v29 = *(uint*)(newMpq + 312);
	uint v30 = v18;
	uint v28 = 0xEEEEEEEE;
	for( int j = (unsigned int)(16 * *(uint*)(*(uint*)(newMpq + 308) + 24)) >> 2; j; --j ){
		v29 += 4;
		int v32 = *(uint*)(MpqDecryptBuffer + 4 * (unsigned __int8)v30 + 4096) + v28;
		int v33 = (v30 + v32) ^ *(uint*)(v29 - 4);
		*(uint*)(v29 - 4) = v33;
		v28 = v32 + 32 * v32 + v33 + 3;
		v30 = ((unsigned int)v30 >> 11) | (((v30 ^ 0x7FF) << 21) + 0x11111111);
	}
	int v37 = SMemAlloc(16 * *(uint*)(*(uint*)(newMpq + 308) + 28), "SFILE.CPP", 1773, 0);
	int v38 = *(uint*)(newMpq + 308);
	*(uint*)(newMpq + 304) = v37;
	uint v39 = hFile;
	SetFilePointer((HANDLE)hFile, *(uint*)(newMpq + 300) + *(uint*)(v38 + 20), 0, 0);
	ReadFile((HANDLE)v39, *(LPVOID*)(newMpq + 304), 16 * *(uint*)(*(uint*)(newMpq + 308) + 28), &NumberOfBytesRead, 0);
	int v34 = (int)"(block table)";
	uint v35 = 0xEEEEEEEE;
	char v41;
	for( int k = 0xEEEEEEEE; v34; k += 32*k + v35 + v41 + 3 ){
		if( !*(uchar*)v34 )
			break;
		int v40 = *(uchar*)v34++;
		v41 = toupper(v40);
		v35 = (k + v35) ^ *(uint*)(MpqDecryptBuffer + 4 * v41 + 3072);
	}
	int v43 = *(uint*)(newMpq + 304);
	uint v44 = v35;
	uint v42 = 0xEEEEEEEE;
	for( int l = (unsigned int)(16 * *(uint*)(*(uint*)(newMpq + 308) + 28)) >> 2; l; --l ){
		v43 += 4;
		int v46 = *(uint*)(MpqDecryptBuffer + 4 * (unsigned __int8)v44 + 4096) + v42;
		int v47 = (v44 + v46) ^ *(uint*)(v43 - 4);
		*(uint*)(v43 - 4) = v47;
		v42 = v46 + 32 * v46 + v47 + 3;
		v44 = ((unsigned int)v44 >> 11) | (((v44 ^ 0x7FF) << 21) + 0x11111111);
	}
	if( *(uint*)(newMpq + 300) ){
		uint v48 = 0;
		if( *(uint*)(*(uint*)(newMpq + 308) + 28) ){
			int v49 = 0;
			do{
				int v51 = *(uint*)(newMpq + 304);
				int v52 = v51 + v49;
				int v50 = *(uint*)(v51 + v49);
				if( v50 )
					*(uint *)v52 = *(uint *)(newMpq + 300) + v50;
				++v48;
				v49 += 16;
			}while( v48 < *(uint*)(*(uint*)(newMpq + 308) + 28) );
		}
	}
	EnterCriticalSection(&FileCriticalSection);
	int m;
	for( m = OpenedMpqArchives; m; m = *(uint*)(m + 4) ){
		if( *(uint*)(m + 276) <= dwPriority )
			break;
	}
	int v54 = *(uint*)newMpq;
	if( *(uint*)newMpq )
		goto LABEL_85;
	if( !*(uint*)(newMpq + 4) )
		goto LABEL_69;
	if( v54 ){
		LABEL_85:
		*(uint*)(v54 + 4) = *(uint*)(newMpq + 4);
	}else{
		if( OpenedMpqArchives == newMpq )
			OpenedMpqArchives = *(uint*)(newMpq + 4);
	}
	int v55 = *(uint*)(newMpq + 4);
	if( v55 ){
		*(uint*)v55 = *(uint*)newMpq;
	}else{
		if( dword_1501EFA4 == newMpq )
			dword_1501EFA4 = *(uint*)newMpq;
	}
	*(uint*)newMpq = 0;
	*(uint*)(newMpq + 4) = 0;
	LABEL_69:
	if( m ){
		int v56 = *(uint*)m;
		*(uint*)(newMpq + 4) = m;
		*(uint*)newMpq = v56;
		*(uint*)m = newMpq;
		if( *(uint*)newMpq ){
			*(uint*)(*(uint*)newMpq + 4) = newMpq;
			goto LABEL_76;
		}
		goto LABEL_74;
	}
	*(uint*)newMpq = dword_1501EFA4;
	if( !OpenedMpqArchives ){
		dword_1501EFA4 = newMpq;
		LABEL_74:
		OpenedMpqArchives = newMpq;
		goto LABEL_76;
	}
	*(uint*)(dword_1501EFA4 + 4) = newMpq;
	dword_1501EFA4 = newMpq;
	LABEL_76:
	LeaveCriticalSection(&FileCriticalSection);
	if( *(uint*)RootPathName && hHandle == INVALID_HANDLE_VALUE ){
		uint v57;
		sub_1500BF40(v57);
		LABEL_79:
		int result = 1;
		*(uint*)mpq = newMpq;
		return result;
	}
	if( !dwFlags || *(uint*)RootPathName )
		goto LABEL_79;
	SFileCloseArchive((void*)newMpq);
	SErrSetLastError(15);
	return 0;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFA4: using guessed type int dword_1501EFA4;
// 1501F0D4: using guessed type int dword_1501F0D4;
//----- (1500DDD0) --------------------------------------------------------
void __fastcall GetMpqFullPath(char* fileName, char* fullPath)
{
	char* v4;        // esi@2
	HMODULE v5;      // eax@2
	int v6;          // ecx@7
	wchar_t v7[2];   // edi@7
	char v8;         // zf@9
	unsigned int v9; // edx@11
	int v10;         // ecx@11
	int v11;         // ecx@11
	CHAR* v12;       // edi@11
	void* v13;       // esi@11
	char v14;        // zf@13
	CHAR Path[260];  // [sp+Ch] [bp-104h]@16
	if( !SFilename[0] ){
		v5 = GetModuleHandleA(0);
		GetModuleFileNameA(v5, SFilename, 260);
		v4 = strchr(SFilename, 92);
		if( v4 ){
			do{
				if( !strchr(v4 + 1, 92) )
					break;
				v4 = strchr(v4 + 1, 92);
			}while( v4 );
			if( v4 )
				*v4 = 0;
		}
		*(uint*)v7 = (uint)L"\\";
		v6 = -1;
		do{
			if( !v6 )
				break;
			v8 = **(uchar**)v7 == 0;
			++*(uint*)v7;
			--v6;
		}while( !v8 );
		v11 = ~v6;
		v13 = (void*)(*(uint*)v7 - v11);
		v9 = v11;
		v12 = SFilename;
		v10 = -1;
		do{
			if( !v10 )
				break;
			v14 = *v12++ == 0;
			--v10;
		}while( !v14 );
		memcpy(v12 - 1, v13, v9);
	}
	memset(Path, 0, 260);
	wsprintfA(Path, "%s%s", SFilename, fileName);
	_fullpath(fullPath, Path, 260);
}

// 1501C778: using guessed type wchar_t asc_1501C778[2];
// Open file from game folder if exists or from opened mpqs
//----- (1500DEC0) (1501A0C0 new storm with debug & tcp/ip) ------------------------
int __stdcall SFileOpenFile(char* fileName, HANDLE* fileHandle)
{
	//if( ExternalStorm ) return eSFileOpenFile(fileName, fileHandle);
	if( fileHandle )
		*(uint *)fileHandle = 0;
	if( !(fileName[0] != 0 ? (fileHandle != 0 ? -(fileName != 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( !EnabledDirectAccess ){
		if( OpenedMpqArchives ){
			goto LABEL_46;
		}
	}
	char fullPath[260];
	memset(fullPath, 0, 260);
	if( fileName[0] == 92 || strstr(fileName, ":\\") || strstr(fileName, "\\\\") ){
		strcpy(fullPath, fileName);
	}else{
		GetMpqFullPath(fileName, fullPath);
	}
	HANDLE v6 = CreateFileA(fullPath, 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
	if( v6 != INVALID_HANDLE_VALUE ){
		EnterCriticalSection(&FileCriticalSection);
		int v7 = SMemAlloc(60, ".?AUFILEREC@@", -2, 8);
		sub_1500E210((int)&dword_1501EFB0, v7);
		*(uint*)(v7 + 8) = (uint)v6;
		LeaveCriticalSection(&FileCriticalSection);
		*(uint*)fileHandle = v7;
		return 1;
	}
	if( OpenedMpqArchives ){
		LABEL_46:
		if( MpqDecryptBuffer ){
			int curMpq = OpenedMpqArchives;
			while( 1 ){
				char* v8 = fileName;
				int v32 = dword_1501F0D8;
				int v9 = 0x7FED7FED;
				char v12;
				for( int i = 0xEEEEEEEE; v8; i += 32*i + v9 + v12 + 3 ){
					if( !*v8 )
						break;
					int v11 = *v8++;
					v12 = toupper(v11);
					v9 = (i + v9) ^ *(uint*)(MpqDecryptBuffer + 4 * v12);
				}
				char* v13 = fileName;
				int v30 = 0x7FED7FED;
				char v16;
				for( int j = 0xEEEEEEEE; v13; j += 32*j + v30 + v16 + 3 ){
					if( !*v13 )
						break;
					int v15 = *v13++;
					v16 = toupper(v15);
					v30 = (j + v30) ^ *(uint*)(MpqDecryptBuffer + 4 * v16 + 1024); // буфер аллоцирован динамически 
				}
				char* v17 = fileName;
				int v18 = 0x7FED7FED;
				int v19 = 0xEEEEEEEE;
				if( fileName ){
					do{
						if( !*v17 ){
							break;
						}
						int v20 = *v17++;
						char v21 = toupper(v20);
						v18 = (v19 + v18) ^ *(uint*)(MpqDecryptBuffer + 4 * v21 + 2048);
						v19 += 32 * v19 + v18 + v21 + 3;
					}while( v17 );
				}
				int v31 = -1;
				int v26 = *(uint*)(curMpq + 312);
				int v25 = *(uint*)(*(uint*)(curMpq + 308) + 24) - 1;
				int v22 = v25 & v9;
				int v24 = v26 + 16 * (v25 & v9);
				for( int k = *(uint*)(v24 + 12); k != -1; k = *(uint*)(16*v22 + v26 + 12) ){
					if( *(uint*)v24 == v30 && *(uint*)(v24 + 4) == v18 && k != -2 ){
						int v27 = *(uint*)(v24 + 8);
						if( v27 == v32 ){
							goto LABEL_37;
						}
						if( !v27 )
							v31 = v22;
					}
					v22 = v25&(v22 + 1);
					if( v22 == (v25 & v9) )
						break;
					v24 = 16 * v22 + v26;
				}
				v22 = v31;
				LABEL_37:
				int v28 = curMpq;
				if( v22 == -1 ){
					v28 = *(uint*)(curMpq + 4);
					curMpq = *(uint*)(curMpq + 4);
				}
				if( !v28 ){
					break;
				}
				if( v22 != -1 ){
					if( !curMpq ){
						break;
					}
					return SFileOpenFileEx((HANDLE)curMpq, fileName, 0, fileHandle);
				}
			}
		}
	}
	SErrSetLastError(2u);
	return 0;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501F0CC: using guessed type int EnabledDirectAccess;
// 1501F0D4: using guessed type int dword_1501F0D4;
// 1501F0D8: using guessed type int dword_1501F0D8;
//----- (1500E210) --------------------------------------------------------
int __thiscall sub_1500E210(int This, int a2)
{
	int result; // eax@1
	result = a2;
	*(uint*)a2 = *(uint*)(This + 4);
	if( *(uint*)This ){
		*(uint*)(*(uint*)(This + 4) + 4) = a2;
		*(uint*)(This + 4) = a2;
	}else{
		*(uint*)(This + 4) = a2;
		*(uint*)This = a2;
	}
	return result;
}

//----- (1500E240) --------------------------------------------------------
int __stdcall SFileOpenFileEx(HANDLE handle, char* name, int a3, HANDLE* fHandle)
{
	//if( ExternalStorm ) return eSFileOpenFileEx(handle, name, a3, fHandle);
	int result;       // eax@4
	int v5;           // eax@5
	char* v6;         // ebx@9
	int i;            // edi@9
	int v8;           // esi@9
	char v9;          // al@10
	char v10;         // al@11
	int j;            // edx@12
	int v12;          // ecx@12
	int v13;          // ebx@12
	int v14;          // ebp@12
	int v15;          // edi@12
	int v16;          // esi@12
	int v17;          // ecx@16
	int v18;          // ebp@24
	char* k;          // ebx@26
	int v20;          // edi@30
	int l;            // esi@30
	int v22;          // ST0C_4@32
	char v23;         // al@32
	int v24;          // ebx@33
	unsigned int v25; // ebp@33
	int v26;          // ecx@33
	int v27;          // esi@35
	int v28;          // [sp+10h] [bp-10h]@12
	int v29;          // [sp+10h] [bp-10h]@24
	int v30;          // [sp+14h] [bp-Ch]@12
	int v31;          // [sp+18h] [bp-8h]@12
	int v32;          // [sp+1Ch] [bp-4h]@9
	if( fHandle )
		*(uint *)fHandle = 0;
	if( fHandle != 0 ? (name != 0 ? (handle != 0 ? -(*name != 0) : 0) : 0) : 0 ){
		v5 = OpenedMpqArchives;
		if( OpenedMpqArchives ){
			while( v5 != (int)handle ){
				v5 = *(uint*)(v5 + 4);
				if( !v5 )
					goto LABEL_8;
			}
			v6 = name;
			v32 = dword_1501F0D8;
			v8 = 2146271213;
			for( i = -286331154; v6; i += 32*i + v8 + v10 + 3 ){
				v9 = *v6;
				if( !*v6 )
					break;
				++v6;
				v10 = toupper(v9);
				v8 = (i + v8) ^ *(uint*)(MpqDecryptBuffer + 4 * v10);
			}
			v30 = sub_1500E580((int)name, 1);
			v31 = sub_1500E580((int)name, 2);
			v28 = -1;
			v15 = *(uint*)((uint)handle + 312);
			v13 = *(uint*)(*(uint*)((uint)handle + 308) + 24) - 1;
			v16 = v13 & v8;
			v14 = v16;
			v12 = v15 + 16 * v16;
			for( j = *(uint*)(v12 + 12); j != -1; j = *(uint*)(v12 + 12) ){
				if( *(uint*)v12 == v30 && *(uint*)(v12 + 4) == v31 && j != -2 ){
					v17 = *(uint*)(v12 + 8);
					if( v17 == v32 )
						goto LABEL_22;
					if( !v17 )
						v28 = v16;
				}
				v16 = v13&(v16 + 1);
				if( v16 == v14 )
					break;
				v12 = v15 + 16 * v16;
			}
			v16 = v28;
			LABEL_22:
			if( v16 == -1 ){
				SErrSetLastError(2u);
				result = 0;
			}else{
				v18 = *(uint*)((uint)handle + 304) + 16 * *(uint*)(16 * v16 + v15 + 12);
				v29 = v18;
				if( *(uint*)(v18 + 8) && *(uint*)(v18 + 12) & 0x80000000 ){
					for( k = (char*)name; strchr(k, 58); k = strchr(k, 58) + 1 )
					;
					for( ; strchr(k, 92); k = strchr(k, 92) + 1 )
					;
					v20 = 2146271213;
					for( l = -286331154; k; l += 32*l + v20 + v23 + 3 ){
						if( !*k )
							break;
						v22 = *k++;
						v23 = toupper(v22);
						v20 = (l + v20) ^ *(uint*)(MpqDecryptBuffer + 4 * v23 + 3072);
					}
					v24 = 0;
					v25 = (unsigned int)(*(uint*)((uint)handle + 288) + *(uint*)(v18 + 8) - 1) / *(uint*)((uint)handle + 288);
					v26 = *(uint*)(v29 + 12);
					if( BYTE1(v26) & 1 )
						v24 = SMemAlloc(4 * v25 + 4, "SFILE.CPP", 1922, 0);
					EnterCriticalSection(&FileCriticalSection);
					v27 = SMemAlloc(60, ".?AUFILEREC@@", -2, 8);
					*(uint*)v27 = dword_1501EFB4;
					if( dword_1501EFB0 ){
						*(uint*)(dword_1501EFB4 + 4) = v27;
						dword_1501EFB4 = v27;
					}else{
						dword_1501EFB4 = v27;
						dword_1501EFB0 = v27;
					}
					*(uint*)(v27 + 8) = -1;
					*(uint*)(v27 + 12) = (uint)handle;
					*(uint*)(v27 + 16) = v29;
					*(uint*)(v27 + 20) = v20;
					*(uint*)(v27 + 32) = v25;
					*(uint*)(v27 + 36) = v24;
					*(uint*)(v27 + 48) = SMemAlloc(4096, "SFILE.CPP", 1931, 0);
					LeaveCriticalSection(&FileCriticalSection);
					*(uint*)fHandle = v27;
					result = 1;
				}else{
					SErrSetLastError(0x3EEu);
					result = 0;
				}
			}
		}else{
			LABEL_8:
			SErrSetLastError(6u);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501EFB0: using guessed type int dword_1501EFB0;
// 1501EFB4: using guessed type int dword_1501EFB4;
// 1501F0D4: using guessed type int dword_1501F0D4;
// 1501F0D8: using guessed type int dword_1501F0D8;
//----- (1500E580) --------------------------------------------------------
int __fastcall sub_1500E580(int a1, int a2)
{
	int v2;  // ebx@1
	int v3;  // ebp@1
	int v4;  // edi@1
	int i;   // esi@1
	int v6;  // ST00_4@3
	char v7; // al@3
	v2 = a1;
	v3 = a2;
	v4 = 2146271213;
	for( i = -286331154; v2; i += 32*i + v4 + v7 + 3 ){
		if( !*(uchar*)v2 )
			break;
		v6 = *(uchar*)v2++;
		v7 = toupper(v6);
		v4 = (i + v4) ^ *(uint*)(MpqDecryptBuffer + 4 * (v7 + (v3 << 8)));
	}
	return v4;
}

// 1501F0D4: using guessed type int dword_1501F0D4;
//----- (1500E5E0) --------------------------------------------------------
int __stdcall sub_1500E5E0(char* fileName, int fileHandle)
{
	char v2;            // dl@3
	char* v3;           // ebp@3
	int result;         // eax@4
	HANDLE v5;          // eax@10
	char* v6;           // ebx@16
	int v7;             // edi@16
	int i;              // esi@16
	int v9;             // ST18_4@18
	char v10;           // al@18
	char* v11;          // ebx@19
	int j;              // esi@19
	int v13;            // ST18_4@21
	char v14;           // al@21
	char* v15;          // ebx@22
	int v16;            // ebp@22
	int v17;            // esi@22
	int v18;            // ST18_4@24
	char v19;           // al@24
	int v20;            // eax@25
	int k;              // edx@25
	int v22;            // ecx@25
	int v23;            // ebx@25
	int v24;            // esi@25
	int v25;            // ecx@29
	int v26;            // ecx@35
	int v27;            // esi@40
	int v28;            // eax@41
	HANDLE v29;         // eax@43
	int v30;            // [sp+10h] [bp-118h]@14
	int v31;            // [sp+14h] [bp-114h]@19
	int v32;            // [sp+18h] [bp-110h]@25
	int v33;            // [sp+20h] [bp-108h]@16
	char fullPath[260]; // [sp+24h] [bp-104h]@5
	if( fileHandle )
		*(uint *)fileHandle = -1;
	v3 = fileName;
	v2 = *fileName;
	if( *fileName != 0 ? (fileHandle != 0 ? -(fileName != 0) : 0) : 0 ){
		memset(fullPath, 0, 260);
		if( v2 == 92 || strstr(fileName, ":\\") || strstr(fileName, "\\\\") )
			strcpy(fullPath, fileName);
		else
			GetMpqFullPath(fileName, (char*)&fullPath);
		v5 = CreateFileA(fullPath, 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
		*(uint*)fileHandle = (uint)v5;
		if( v5 == INVALID_HANDLE_VALUE ){
			if( OpenedMpqArchives && MpqDecryptBuffer ){
				v30 = OpenedMpqArchives;
				while( 1 ){
					v6 = v3;
					v33 = dword_1501F0D8;
					v7 = 2146271213;
					for( i = -286331154; v6; i += 32*i + v7 + v10 + 3 ){
						if( !*v6 )
							break;
						v9 = *v6++;
						v10 = toupper(v9);
						v7 = (i + v7) ^ *(uint*)(MpqDecryptBuffer + 4 * v10);
					}
					v11 = v3;
					v31 = 2146271213;
					for( j = -286331154; v11; j += 32*j + v31 + v14 + 3 ){
						if( !*v11 )
							break;
						v13 = *v11++;
						v14 = toupper(v13);
						v31 = (j + v31) ^ *(uint*)(MpqDecryptBuffer + 4 * v14 + 1024);
					}
					v15 = v3;
					v16 = 2146271213;
					v17 = -286331154;
					if( fileName ){
						do{
							if( !*v15 )
								break;
							v18 = *v15++;
							v19 = toupper(v18);
							v16 = (v17 + v16) ^ *(uint*)(MpqDecryptBuffer + 4 * v19 + 2048);
							v17 += 32 * v17 + v16 + v19 + 3;
						}while( v15 );
					}
					v32 = -1;
					v24 = *(uint*)(v30 + 312);
					v23 = *(uint*)(*(uint*)(v30 + 308) + 24) - 1;
					v20 = v23 & v7;
					v22 = v24 + 16 * (v23 & v7);
					for( k = *(uint*)(v22 + 12); k != -1; k = *(uint*)(16*v20 + v24 + 12) ){
						if( *(uint*)v22 == v31 && *(uint*)(v22 + 4) == v16 && k != -2 ){
							v25 = *(uint*)(v22 + 8);
							if( v25 == v33 )
								goto LABEL_35;
							if( !v25 )
								v32 = v20;
						}
						v20 = v23&(v20 + 1);
						if( v20 == (v23 & v7) )
							break;
						v22 = 16 * v20 + v24;
					}
					v20 = v32;
					LABEL_35:
					v26 = v30;
					if( v20 == -1 ){
						v26 = *(uint*)(v30 + 4);
						v30 = *(uint*)(v30 + 4);
					}
					if( !v26 )
						break;
					if( v20 != -1 ){
						if( !v30 )
							break;
						v27 = *(uint*)(v30 + 304) + 16 * *(uint*)(16 * v20 + *(uint*)(v30 + 312) + 12);
						if( *(uint *)(v27 + 8) && (v28 = *(uint *)(v27 + 12), !(v28 & 0x10100)) && v28 & 0x80000000 ){
							v29 = CreateFileA((LPCSTR)(v30 + 8), 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
							*(uint*)fileHandle = (uint)v29;
							if( v29 == INVALID_HANDLE_VALUE )
								goto LABEL_47;
							SetFilePointer(v29, *(uint*)v27, 0, 0);
							result = 1;
						}else{
							SErrSetLastError(0x3EEu);
							result = 0;
						}
						return result;
					}
					v3 = fileName;
				}
			}
			SErrSetLastError(2u);
			LABEL_47:
			result = 0;
		}else{
			result = 1;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501EFA0: using guessed type int OpenedMpqArchives;
// 1501F0D4: using guessed type int dword_1501F0D4;
// 1501F0D8: using guessed type int dword_1501F0D8;
//----- (1500E950) --------------------------------------------------------
int __stdcall SFileReadFile(HANDLE fHandle, LPVOID dataPtr, uint nNumberOfBytesToRead, uint* BytesReaded, int a5)
{
	//if( ExternalStorm ) return eSFileReadFile(fHandle, dataPtr, nNumberOfBytesToRead, (DWORD*)BytesReaded, a5);
	int v5;           // esi@1
	int v6;           // ebx@3
	LPVOID v7;        // edi@3
	int result;       // eax@4
	int v9;           // eax@5
	unsigned int v10; // ebp@9
	void* v11;        // eax@11
	LPVOID v12;       // eax@12
	void* v13;        // ST10_4@12
	int v14;          // esi@15
	unsigned int v15; // eax@16
	unsigned int v16; // edx@16
	int v17;          // eax@17
	int v18;          // ecx@19
	void* v19;        // esi@19
	int v20;          // edx@19
	LPVOID v21;       // edi@19
	char v22;         // zf@19
	int v23;          // eax@24
	int v24;          // edx@24
	unsigned int v25; // ecx@24
	int v26;          // esi@24
	int v27;          // edi@28
	HANDLE* v28;      // eax@29
	unsigned int v29; // ebp@29
	unsigned int v30; // esi@29
	HANDLE* v31;      // ebx@30
	int v32;          // eax@31
	HANDLE v33;       // eax@33
	int v34;          // eax@36
	void** v35;       // esi@36
	unsigned int v36; // eax@42
	LPVOID v37;       // edi@44
	int v38;          // ecx@46
	char v39;         // zf@46
	int v40;          // [sp+10h] [bp-1Ch]@28
	DWORD v41;        // [sp+14h] [bp-18h]@24
	unsigned int v42; // [sp+18h] [bp-14h]@24
	int v43;          // [sp+1Ch] [bp-10h]@29
	void* v44;        // [sp+20h] [bp-Ch]@29
	void* v45;        // [sp+24h] [bp-8h]@29
	int v46;          // [sp+28h] [bp-4h]@30
	int i;            // [sp+30h] [bp+4h]@36
	int a1 = (int)fHandle;
	v5 = (int)BytesReaded;
	if( BytesReaded )
		*(uint *)BytesReaded = 0;
	v6 = a1;
	v7 = dataPtr;
	if( !(a1 != 0 ? -(dataPtr != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	v9 = dword_1501EFB0;
	if( !dword_1501EFB0 ){
		LABEL_8:
		SErrSetLastError(6u);
		return 0;
	}
	while( v9 != a1 ){
		v9 = *(uint*)(v9 + 4);
		if( !v9 )
			goto LABEL_8;
	}
	v10 = nNumberOfBytesToRead;
	if( !nNumberOfBytesToRead )
		return 1;
	v11 = *(void**)(a1 + 8);
	if( v11 == (void*)-1 ){
		v14 = *(uint*)(a1 + 48);
		if( v14 ){
			v16 = *(uint*)(a1 + 52);
			v15 = *(uint*)(a1 + 56);
			if( v16 < v15 ){
				v17 = v15 - v16;
				if( nNumberOfBytesToRead < v17 )
					v17 = nNumberOfBytesToRead;
				v19 = (void*)(v16 + v14);
				v10 = nNumberOfBytesToRead - v17;
				memcpy(dataPtr, v19, 4 * ((unsigned int)v17 >> 2));
				nNumberOfBytesToRead = v10;
				memcpy((char*)v7 + 4 * ((unsigned int)v17 >> 2), (char*)v19 + 4 * ((unsigned int)v17 >> 2), v17 & 3);
				v20 = *(uint*)(a1 + 24);
				v21 = dataPtr;
				*(uint*)(a1 + 52) += v17;
				v18 = (int)BytesReaded;
				v22 = BytesReaded == 0;
				*(uint*)(a1 + 24) = v17 + v20;
				dataPtr = (char*)v21 + v17;
				if( !v22 )
					*(uint *)v18 = v17;
				if( !v10 )
					return 1;
				v7 = dataPtr;
			}
		}
		v24 = *(uint*)(a1 + 12);
		v26 = *(uint*)(a1 + 24);
		v41 = (DWORD)v7;
		v25 = v10;
		v42 = v10;
		v23 = *(uint*)(v24 + 288) - (v26&(*(uint*)(v24 + 288) - 1));
		if( (unsigned int)v23 > 0x1000 )
			v23 = 4096;
		if( v10 < v23 ){
			v42 = v23;
			v41 = *(uint*)(a1 + 48);
			v25 = v23;
		}
		v27 = 0;
		v40 = 0;
		if( *(uint*)(v24 + 272) ){
			v29 = (v25 + 131071) >> 17;
			v44 = (void*)SMemAlloc(4 * v29, "SFILE.CPP", 2079, 0);
			v28 = (HANDLE*)SMemAlloc(4 * v29, "SFILE.CPP", 2080, 0);
			v30 = 0;
			v45 = (void*)v28;
			v43 = 1000;
			if( v29 ){
				v31 = v28;
				v46 = (char*)v44 - (char*)v28;
				do{
					v32 = v42 - v27;
					if( v42 - v27 > 0x20000 )
						v32 = 131072;
					v33 = sub_1500CD60(a1, v27, v27 + v41, 0, 0, 0, v32, v43, 1, v30 == v29 - 1);
					v27 += 131072;
					++v31;
					*(HANDLE*)((char*)v31 + v46 - 4) = v33;
					*((uint*)v31 - 1) = *((uint*)v33 + 2);
					++v30;
					v43 += 655;
				}while( v30 < v29 );
				v6 = a1;
				v28 = (HANDLE*)v45;
				v27 = 0;
			}
			WaitForMultipleObjects(v29, v28, 1, 0xFFFFFFFFu);
			if( v29 ){
				v35 = (void**)v44;
				v34 = (char*)v45 - (char*)v44;
				for( i = (int)v45 - (int)v44; ; v34 = i ){
					v27 += *((uint*)*v35 + 13);
					CloseHandle(*(void**)((char*)v35 + v34));
					SMemFree(*v35, "SFILE.CPP", 2101, 0);
					++v35;
					--v29;
					if( !v29 )
						break;
				}
				v40 = v27;
			}
			SMemFree(v44, "SFILE.CPP", 2103, 0);
			SMemFree(v45, "SFILE.CPP", 2104, 0);
			v10 = nNumberOfBytesToRead;
		}else{
			v27 = sub_1500C5A0(a1, v26, v41, v25);
			v40 = v27;
		}
		v36 = v10;
		if( v10 >= v27 )
			v36 = v27;
		v37 = dataPtr;
		if( (LPVOID)v41 != dataPtr ){
			memcpy(dataPtr, (void*)v41, 4 * (v36 >> 2));
			memcpy((char*)v37 + 4 * (v36 >> 2), (void*)(v41 + 4 * (v36 >> 2)), v36 & 3);
			*(uint*)(v6 + 52) = v36;
			*(uint*)(v6 + 56) = v40;
		}
		v38 = (int)BytesReaded;
		v39 = BytesReaded == 0;
		*(uint*)(v6 + 24) += v36;
		if( !v39 )
			*(uint *)v38 += v36;
		if( v36 == v10 ){
			result = 1;
		}else{
			SErrSetLastError(0x26u);
			result = 0;
		}
	}else{
		v13 = dataPtr;
		dataPtr = 0;
		ReadFile(v11, v13, nNumberOfBytesToRead, (LPDWORD)&dataPtr, 0);
		v12 = dataPtr;
		if( v5 )
			*(uint *)v5 = (uint)dataPtr;
		result = v12 == (LPVOID)v10;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500EC90) --------------------------------------------------------
int __stdcall SFileSetBasePath(char* a1)
{
	//if( ExternalStorm ) return eSFileSetBasePath(a1);
	int result;      // eax@2
	int v2;          // ebp@3
	int v3;          // ecx@6
	wchar_t v4[2];   // edi@6
	char v5;         // zf@8
	unsigned int v6; // edx@10
	int v7;          // ecx@10
	int v8;          // ecx@10
	CHAR* v9;        // edi@10
	void* v10;       // esi@10
	char v11;        // zf@12
	if( *a1 ){
		v2 = a1[strlen(a1) - 2] == 92;
		if( strlen(a1) - 1 + (v2 == 0) + 1 <= 0x104 ){
			strcpy(SFilename, a1);
			if( !v2 ){
				*(uint*)v4 = (uint)L"\\";
				v3 = -1;
				do{
					if( !v3 )
						break;
					v5 = **(uchar**)v4 == 0;
					++*(uint*)v4;
					--v3;
				}while( !v5 );
				v8 = ~v3;
				v10 = (void*)(*(uint*)v4 - v8);
				v6 = v8;
				v9 = SFilename;
				v7 = -1;
				do{
					if( !v7 )
						break;
					v11 = *v9++ == 0;
					--v7;
				}while( !v11 );
				memcpy(v9 - 1, v10, v6);
			}
			result = 1;
		}else{
			SErrSetLastError(0xA1u);
			result = 0;
		}
	}else{
		SFilename[0] = 0;
		result = 1;
	}
	return result;
}

// 1501C778: using guessed type wchar_t asc_1501C778[2];
//----- (1500ED50) --------------------------------------------------------
DWORD __stdcall SFileSetFilePointer(HANDLE fHandle, LONG lDistanceToMove, int a3, DWORD dwMoveMethod)
{
	//if( ExternalStorm ) return eSFileSetFilePointer(fHandle, lDistanceToMove, a3, dwMoveMethod);
	int a1 = (int)fHandle;
	int v4;       // eax@4
	DWORD result; // eax@7
	void* v6;     // eax@8
	if( !a1 || a3 && *(uint*)a3 ){
		SErrSetLastError(0x57u);
		result = -1;
	}else{
		v4 = dword_1501EFB0;
		if( dword_1501EFB0 ){
			while( v4 != a1 ){
				v4 = *(uint*)(v4 + 4);
				if( !v4 )
					goto LABEL_7;
			}
			v6 = *(void**)(a1 + 8);
			if( v6 == (void*)-1 ){
				if( dwMoveMethod ){
					if( dwMoveMethod == 1 ){
						if( lDistanceToMove >= 0 || *(uint*)(a1 + 24) >= (unsigned int)-lDistanceToMove )
							*(uint *)(a1 + 24) += lDistanceToMove;
						else
							*(uint*)(a1 + 24) = 0;
					}else{
						if( dwMoveMethod == 2 ){
							if( lDistanceToMove >= 0 || *(uint*)(*(uint*)(a1 + 16) + 8) >= (unsigned int)-lDistanceToMove )
								*(uint *)(a1 + 24) = lDistanceToMove + *(uint *)(*(uint *)(a1 + 16) + 8);
							else
								*(uint*)(a1 + 24) = 0;
						}
					}
				}else{
					*(uint*)(a1 + 24) = lDistanceToMove;
				}
				result = *(uint*)(*(uint*)(a1 + 16) + 8) - 1;
				if( *(uint*)(a1 + 24) < result )
					result = *(uint*)(a1 + 24);
				*(uint*)(a1 + 24) = result;
				*(uint*)(a1 + 52) = 0;
				*(uint*)(a1 + 56) = 0;
			}else{
				result = SetFilePointer(v6, lDistanceToMove, 0, dwMoveMethod);
			}
		}else{
			LABEL_7:
			SErrSetLastError(6u);
			result = -1;
		}
	}
	return result;
}

// 1501EFB0: using guessed type int dword_1501EFB0;
//----- (1500EE40) --------------------------------------------------------
int __stdcall SFileSetLocale(int a1)
{
	dword_1501F0D8 = a1;
	return 1;
}

// 1501F0D8: using guessed type int dword_1501F0D8;
//----- (1500EE70) --------------------------------------------------------
int __stdcall sub_1500EE70()
{
	int result; // eax@1
	result = 0;
	dword_1501F0E0 = 0;
	dword_1501F0E4 = 0;
	return result;
}

// 1501F0E0: using guessed type int dword_1501F0E0;
// 1501F0E4: using guessed type int dword_1501F0E4;
//----- (1500EE80) --------------------------------------------------------
void __stdcall SGdiBitBlt(int a1, int a2, int a3, int a4, int a5, int a6, int a7, unsigned int a8, unsigned int a9)
{
	int v9;           // eax@5
	unsigned int v10; // ecx@5
	int v11;          // ebx@7
	unsigned int v12; // eax@12
	int v13;          // ecx@16
	int v14;          // [sp+4h] [bp-30h]@3
	int v15;          // [sp+8h] [bp-2Ch]@3
	int v16;          // [sp+Ch] [bp-28h]@3
	int v17;          // [sp+10h] [bp-24h]@3
	int v18;          // [sp+14h] [bp-20h]@4
	int v19;          // [sp+18h] [bp-1Ch]@4
	int v20;          // [sp+1Ch] [bp-18h]@4
	int v21;          // [sp+20h] [bp-14h]@4
	int v22;          // [sp+24h] [bp-10h]@3
	int v23;          // [sp+28h] [bp-Ch]@3
	int v24;          // [sp+2Ch] [bp-8h]@3
	int v25;          // [sp+30h] [bp-4h]@3
	if( !(dword_1501E1B0 != 0 ? (a4 != 0 ? ((a3 < 0) - 1) & (a1 != 0 ? (a2 < 0) - 1 : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return;
	}
	v23 = a3;
	v24 = 4095;
	v25 = 4095;
	v22 = a2;
	v16 = dword_1501E1B8;
	v17 = dword_1501E1BC;
	v14 = a6;
	v15 = a7;
	if( a5 ){
		v18 = *(uint*)a5;
		v19 = *(uint*)(a5 + 4);
		v20 = *(uint*)(a5 + 8);
		v21 = *(uint*)(a5 + 12);
	}
	v10 = a8;
	v9 = 0;
	if( !a8 || a9 == 13369376 )
		goto LABEL_19;
	v11 = dword_1501E1B4;
	if( !(a8 & 0x1000000) ){
		if( dword_1501E1B4 != 8 ){
			if( dword_1501E1B4 == 16 ){
				v12 = (a8 >> 16) & 0xFF;
				LOBYTE_IDA(v12) = (a8 >> 16) & 0xF8;
				v11 = dword_1501E1B4;
				v9 = ((unsigned int)(unsigned __int8)a8 >> 3) | 4 * ((unsigned __int8)(BYTE1(v10) & 0xF8) | 32 * v12);
			}
			goto LABEL_14;
		}
		goto LABEL_13;
	}
	if( dword_1501E1B4 != 8 ){
		LABEL_13:
		v9 = 0;
		goto LABEL_14;
	}
	v9 = v10 & 0xFFFFFF;
	LABEL_14:
	if( v11 == 8 ){
		v13 = (v9 | ((v9 | (v9 << 8)) << 8)) << 8;
		goto LABEL_18;
	}
	if( v11 == 16 ){
		v13 = v9 << 16;
		LABEL_18:
		v9 |= v13;
	}
	LABEL_19:
	if( dword_1501F0EC ){
		v16 = dword_1501E1B8 << dword_1501F0EC;
		v22 = a2 << dword_1501F0EC;
		v24 = 4095 << dword_1501F0EC;
		v18 <<= dword_1501F0EC;
		v20 <<= dword_1501F0EC;
		v14 = a6 << dword_1501F0EC;
	}
	SBltROP3Clipped(a1, (int)&v22, (int)&v16, dword_1501E1B0, a4, a5 != 0 ? (int)&v18 : 0, (int)&v14, a6 << dword_1501F0EC, v9, a9);
}

// 1501E1B0: using guessed type int dword_1501E1B0;
// 1501E1B4: using guessed type int dword_1501E1B4;
// 1501E1B8: using guessed type int dword_1501E1B8;
// 1501E1BC: using guessed type int dword_1501E1BC;
// 1501F0EC: using guessed type int dword_1501F0EC;
//----- (1500F050) --------------------------------------------------------
int __stdcall SGdiCreateFont(void* a1, int a2, int a3, int a4, int a5, int a6, void* a7, int a8)
{
	int result; // eax@4
	int v9;     // ebx@5
	int v10;    // ecx@7
	void* v11;  // eax@7
	int v12;    // esi@8
	if( a8 )
		*(uint *)a8 = 0;
	if( a2 != 0 ? (a1 != 0 ? (a8 != 0 ? (a7 != 0 ? (a6 != 0 ? (a5 != 0 ? -(a3 != 0) : 0) : 0) : 0) : 0) : 0) : 0 ){
		result = sub_1500F180(2084, 0);
		v9 = result;
		if( result ){
			v11 = (void*)SMemAlloc(a2 * a3 * a4 >> 3, "SGDI.CPP", 224, 0);
			memcpy(v11, a1, a2 * a3 * a4 >> 3);
			memcpy((void*)(v9 + 36), a7, 0x800u);
			*(uint*)(v9 + 12) = (uint)v11;
			v10 = 1;
			*(uint*)(v9 + 20) = a6;
			*(uint*)(v9 + 16) = a5;
			*(uint*)(v9 + 24) = a2;
			*(uint*)(v9 + 28) = 1;
			for( *(uint*)(v9 + 32) = 0; v10 < a2 / a5; *(uint*)(v9 + 32) = v12 ){
				v10 *= 2;
				v12 = *(uint*)(v9 + 32) + 1;
				*(uint*)(v9 + 28) *= 2;
			}
			--*(uint*)(v9 + 28);
			if( a8 )
				*(uint *)a8 = v9;
			result = 1;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (1500F180) --------------------------------------------------------
int __fastcall sub_1500F180(int a1, __int16 a2)
{
	int result; // eax@1
	__int16 v3; // si@1
	v3 = a2;
	result = SMemAlloc(a1, ".?AUSGDIOBJ@@", -2, 8);
	*(uint*)(result + 4) = dword_1501F0E0;
	if( dword_1501F0E0 )
		*(uint *)dword_1501F0E0 = result;
	else
		dword_1501F0E4 = result;
	dword_1501F0E0 = result;
	*(ushort*)(result + 10) = v3;
	*(ushort*)(result + 8) = 20301;
	*(uint*)(result + 12) = 0;
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501F0E0: using guessed type int dword_1501F0E0;
// 1501F0E4: using guessed type int dword_1501F0E4;
//----- (1500F1D0) --------------------------------------------------------
int __stdcall SGdiDeleteObject(int a1)
{
	int* v1;    // eax@4
	int v2;     // eax@4
	int v3;     // ecx@4
	void* v4;   // eax@6
	int v5;     // eax@12
	int result; // eax@16
	if( a1 && *(ushort*)(a1 + 8) == 20301 && *(ushort*)(a1 + 10) < 1u ){
		v2 = *(ushort*)(a1 + 10);
		v3 = dword_1501F0F0[v2];
		v1 = &dword_1501F0F0[v2];
		if( v3 == a1 )
			*v1 = 0;
		v4 = *(void**)(a1 + 12);
		if( v4 ){
			SMemFree(v4, "SGDI.CPP", 262, 0);
			*(uint*)(a1 + 12) = 0;
		}
		if( *(uint*)a1 ){
			*(uint*)(*(uint*)a1 + 4) = *(uint*)(a1 + 4);
		}else{
			if( dword_1501F0E0 == a1 )
				dword_1501F0E0 = *(uint*)(a1 + 4);
		}
		v5 = *(uint*)(a1 + 4);
		if( v5 ){
			*(uint*)v5 = *(uint*)a1;
		}else{
			if( dword_1501F0E4 == a1 )
				dword_1501F0E4 = *(uint*)a1;
		}
		*(uint*)a1 = 0;
		*(uint*)(a1 + 4) = 0;
		SMemFree((void*)a1, ".?AUSGDIOBJ@@", -2, 0);
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 1501F0E0: using guessed type int dword_1501F0E0;
// 1501F0E4: using guessed type int dword_1501F0E4;
// 1501F0F0: using guessed type int dword_1501F0F0[];
//----- (1500F2A0) --------------------------------------------------------
int __stdcall SGdiDestroy()
{
	int i; // eax@1
	for( i = dword_1501F0E0; dword_1501F0E0; i = dword_1501F0E0 )
		SGdiDeleteObject(i);
	sub_1500F2F0(0);
	if( dword_1501F0E8 ){
		SMemFree((void*)dword_1501F0E8, "SGDI.CPP", 279, 0);
		dword_1501F0E8 = 0;
	}
	return 1;
}

// 1501F0E0: using guessed type int dword_1501F0E0;
// 1501F0E8: using guessed type int dword_1501F0E8;
//----- (1500F2F0) --------------------------------------------------------
int __fastcall sub_1500F2F0(int a1)
{
	int v1;     // esi@1
	int result; // eax@3
	v1 = a1;
	if( a1 && a1 <= dword_1501F0F8 ){
		result = 1;
	}else{
		if( dword_1501F0F4 ){
			SMemFree((void*)dword_1501F0F4, "SGDI.CPP", 59, 0);
			dword_1501F0F4 = 0;
			dword_1501F0F8 = 0;
		}
		if( v1 ){
			dword_1501F0F4 = SMemAlloc(v1, "SGDI.CPP", 66, 0);
			dword_1501F0F8 = v1;
			result = 1;
		}else{
			result = 0;
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501F0F8: using guessed type int dword_1501F0F8;
//----- (1500F360) --------------------------------------------------------
void __stdcall SGdiExtTextOut(int a1, int a2, int a3, int a4, unsigned int a5, int a6, int a7, char* a8, unsigned int a9)
{
	char* v9;         // ebx@1
	int v10;          // edx@7
	int v11;          // ecx@7
	int v12;          // ebp@7
	int v13;          // edi@7
	int v14;          // edx@13
	int v15;          // ebp@13
	int v16;          // esi@13
	int v17;          // edx@19
	int v18;          // ecx@25
	int v19;          // ebp@31
	int v20;          // ebp@35
	unsigned int v21; // eax@38
	unsigned int v22; // eax@40
	int v23;          // eax@43
	int v24;          // eax@48
	int v25;          // ecx@51
	int v26;          // esi@78
	int v27;          // eax@80
	int v28;          // edx@80
	int v29;          // ecx@80
	int v30;          // ebp@80
	int v31;          // edi@80
	int v32;          // edi@80
	unsigned int v33; // eax@80
	char v34;         // cl@80
	unsigned int v35; // eax@80
	int v36;          // edx@82
	int v37;          // ecx@82
	int v38;          // ebp@82
	int v40;          // edx@96
	int v41;          // edi@96
	int v42;          // ebp@98
	int v43;          // eax@100
	int v44;          // ecx@101
	int v45;          // ecx@104
	int v46;          // eax@106
	int v47;          // edx@106
	int v48;          // [sp+10h] [bp-40h]@78
	int v49;          // [sp+14h] [bp-3Ch]@79
	int v50;          // [sp+18h] [bp-38h]@79
	int v51;          // [sp+1Ch] [bp-34h]@79
	int v52;          // [sp+20h] [bp-30h]@79
	int* v53;         // [sp+28h] [bp-28h]@79
	int* v54;         // [sp+2Ch] [bp-24h]@79
	int v55;          // [sp+30h] [bp-20h]@7
	int v56;          // [sp+34h] [bp-1Ch]@7
	int v57;          // [sp+38h] [bp-18h]@10
	int v58;          // [sp+3Ch] [bp-14h]@16
	unsigned int v59; // [sp+40h] [bp-10h]@80
	int v60;          // [sp+44h] [bp-Ch]@80
	int v61;          // [sp+48h] [bp-8h]@82
	int v62;          // [sp+4Ch] [bp-4h]@84
	int v63;          // [sp+5Ch] [bp+Ch]@96
	int v64;          // [sp+5Ch] [bp+Ch]@106
	int v65;          // [sp+68h] [bp+18h]@38
	unsigned int v66; // [sp+70h] [bp+20h]@51
	v9 = a8;
	if( !(a4 != 0 ? (a1 != 0 ? -(a8 != 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return;
	}
	if( dword_1501F0E8 && dword_1501F0F0[0] ){
		if( (int)a9 < 0 )
			a9 = strlen(a8) - 1;
		v13 = *(uint*)(a4 + 4);
		v55 = *(uint*)a4&((*(uint*)a4 <= 0) - 1);
		v56 = v13&((v13 <= 0) - 1);
		v10 = *(uint*)(a4 + 8);
		v11 = dword_1501E1B8 - 1;
		v12 = v10;
		if( v10 >= dword_1501E1B8 - 1 )
			v12 = dword_1501E1B8 - 1;
		if( *(uint*)a4 <= v12 ){
			v57 = *(uint*)(a4 + 8);
			if( v10 >= v11 )
				v57 = dword_1501E1B8 - 1;
		}else{
			v57 = *(uint*)a4;
		}
		v16 = *(uint*)(a4 + 12);
		v15 = dword_1501E1BC - 1;
		v14 = v16;
		if( v16 >= dword_1501E1BC - 1 )
			v14 = dword_1501E1BC - 1;
		if( v13 <= v14 ){
			v58 = *(uint*)(a4 + 12);
			if( v16 >= v15 )
				v58 = dword_1501E1BC - 1;
		}else{
			v58 = *(uint*)(a4 + 4);
		}
		v17 = a2;
		if( a2 >= v11 )
			v17 = dword_1501E1B8 - 1;
		if( v55 <= v17 ){
			if( a2 >= v11 )
				a2 = dword_1501E1B8 - 1;
		}else{
			a2 = *(uint*)a4&((*(uint*)a4 <= 0) - 1);
		}
		v18 = a3;
		if( a3 >= v15 )
			v18 = dword_1501E1BC - 1;
		if( v56 <= v18 ){
			if( a3 >= v15 )
				a3 = dword_1501E1BC - 1;
		}else{
			a3 = v13&((v13 <= 0) - 1);
		}
		v19 = a6;
		if( a6 == a7 ){
			if( a6 ){
				if( a6 == 1 )
					v20 = a5;
				else
					v20 = a6 != 2 ? 0xFF : 0;
				SGdiRectangle(a1, v55, v57, v56, v58, v20);
			}
			return;
		}
		v21 = 0;
		v65 = 0;
		if( a5 & 0x1000000 ){
			if( dword_1501E1B4 == 8 ){
				v22 = a5;
				v21 = v22 & 0xFFFFFF;
				goto LABEL_45;
			}
		}else{
			if( dword_1501E1B4 != 8 ){
				if( dword_1501E1B4 != 16 )
					goto LABEL_46;
				v23 = BYTE1(a5);
				LOBYTE_IDA(v23) = BYTE1(a5) & 0xF8;
				v21 = ((unsigned int)(unsigned __int8)a5 >> 3) | 4 * (32 * ((a5 >> 16) & 0xF8) | v23);
				LABEL_45:
				v65 = v21;
				LABEL_46:
				if( dword_1501E1B4 == 8 ){
					v24 = ((v21 | ((v21 | (v21 << 8)) << 8)) << 8) | v21;
				}else{
					if( dword_1501E1B4 != 16 ){
						LABEL_51:
						v25 = 0;
						v66 = 0;
						switch( v19 ){
						case 0:
							if( a7 == 1 ){
								v25 = 14812998;
								goto LABEL_76;
							}
							if( a7 == 2 ){
								v25 = 2229030;
								goto LABEL_76;
							}
							if( a7 == 3 ){
								v25 = 15597702;
								goto LABEL_76;
							}
							goto LABEL_77;
						case 1:
							if( !a7 ){
								v25 = 12060490;
								goto LABEL_76;
							}
							if( a7 == 2 ){
								v25 = 3146538;
								goto LABEL_76;
							}
							if( a7 == 3 ){
								v25 = 16515210;
								goto LABEL_76;
							}
							goto LABEL_77;
						case 2:
							if( !a7 ){
								v25 = 8913094;
								goto LABEL_76;
							}
							if( a7 == 1 ){
								v25 = 12583114;
								goto LABEL_76;
							}
							if( a7 == 3 ){
								v25 = 13369376;
								goto LABEL_76;
							}
							goto LABEL_77;
						case 3:
							if( a7 ){
								if( a7 == 1 ){
									v25 = 15925802;
								}else{
									if( a7 != 2 )
										goto LABEL_77;
									v25 = 3342344;
								}
							}else{
								v25 = 12255782;
							}
							LABEL_76:
							v66 = v25;
							LABEL_77:
							if( v25 ){
								v26 = a2;
								v48 = dword_1501F0F0[0];
								if( a9 ){
									v50 = dword_1501F0F0[0] + 28;
									v52 = dword_1501F0F0[0] + 32;
									v49 = dword_1501F0F0[0] + 16;
									v51 = dword_1501F0F0[0] + 20;
									v53 = (int*)(dword_1501F0F0[0] + 24);
									v54 = (int*)(dword_1501F0F0[0] + 12);
									do{
										v32 = *(uint*)v49;
										v33 = (unsigned __int8) * v9;
										v30 = *(uint*)v51;
										v28 = *(uint*)v49*(v33& *(uint*)v50);
										v34 = *(uchar*)v52;
										v59 = *(uint*)v49*(v33& *(uint*)v50);
										v35 = v33 >> v34;
										v29 = v57 - v26;
										v27 = v30 * v35;
										v31 = v32 - 1;
										v60 = v27;
										if( v57 - v26 >= v31 )
											v29 = v31;
										v36 = v29 + v28;
										v37 = v58 - a3;
										v38 = v30 - 1;
										v61 = v36;
										if( v58 - a3 >= v38 )
											v37 = v38;
										v62 = v37 + v27;
										SGdiBitBlt(a1, v26, a3, *v54, (int)&v59, *v53, 32767, a5, v66);
										v26 += *(uint*)(v48 + 8 * (unsigned __int8) * v9++ + 36);
									}while( a9-- != 1 );
								}
								if( v26 >= dword_1501E1B8 - 1 )
									v26 = dword_1501E1B8 - 1;
								if( a7 ){
									switch( a7 ){
									case 1:
										v66 = 15728673;
										break;
									case 2:
										v66 = 66;
										break;
									case 3:
										v66 = 16711778;
										break;
									}
									if( a2 <= *(uint*)a4 ){
										v41 = a3;
										v42 = v65;
									}else{
										v41 = a3;
										v40 = *(uint*)(a4 + 12) - a3 + 1;
										v63 = *(uint*)(v48 + 20);
										if( *(uint*)(v48 + 20) >= v40 )
											v63 = v40;
										v42 = v65;
										SBltROP3(*(uint*)a4 + a1 + *(uint*)(dword_1501F0E8 + 4 * v41), 0, a2 - *(uint*)a4, v63, dword_1501E1B0, 0, v65, v66);
									}
									v43 = *(uint*)(a4 + 8);
									if( v26 <= v43 ){
										v44 = *(uint*)(a4 + 12) - v41 + 1;
										if( *(uint*)(v48 + 20) < v44 )
											v44 = *(uint*)(v48 + 20);
										SBltROP3(a1 + v26 + *(uint*)(dword_1501F0E8 + 4 * v41), 0, v43 - v26 + 1, v44, dword_1501E1B0, 0, v42, v66);
									}
									v45 = *(uint*)(a4 + 4);
									if( v41 > v45 )
										SBltROP3(*(uint*)a4 + a1 + *(uint*)(dword_1501F0E8 + 4 * v45), 0, *(uint*)(a4 + 8) - *(uint*)a4 + 1, v41 - v45, dword_1501E1B0, 0, v42, v66);
									v46 = *(uint*)(a4 + 12);
									v47 = *(uint*)(v48 + 20);
									v64 = v41 + v47;
									if( v41 + v47 < v46 )
										SBltROP3(*(uint*)a4 + a1 + *(uint*)(dword_1501F0E8 + 4 * v64), 0, *(uint*)(a4 + 8) - *(uint*)a4 + 1, v46 - v41 - v47 + 1, dword_1501E1B0, 0, v42, v66);
								}
							}
							return;
						default:
							goto LABEL_77;
						}
					}
					v24 = (v21 << 16) | v21;
				}
				v65 = v24;
				goto LABEL_51;
			}
		}
		v21 = 0;
		goto LABEL_45;
	}
}

// 1501E1B0: using guessed type int dword_1501E1B0;
// 1501E1B4: using guessed type int dword_1501E1B4;
// 1501E1B8: using guessed type int dword_1501E1B8;
// 1501E1BC: using guessed type int dword_1501E1BC;
// 1501F0E8: using guessed type int dword_1501F0E8;
// 1501F0F0: using guessed type int dword_1501F0F0[];
//----- (1500F8D0) --------------------------------------------------------
int __stdcall SGdiImportFont(HFONT h, int a2)
{
	int result;               // eax@4
	HDC v3;                   // eax@5
	HDC v4;                   // ebx@5
	HDC v5;                   // ebp@5
	HWND v6;                  // eax@5
	HDC v7;                   // eax@5
	int v8;                   // esi@8
	int v9;                   // ecx@9
	LONG v10;                 // edx@11
	unsigned __int8 v11;      // sf@13
	unsigned __int8 v12;      // of@13
	HBITMAP v13;              // eax@18
	int v14;                  // ebp@18
	int v15;                  // esi@18
	int v16;                  // edx@19
	HGDIOBJ v17;              // eax@19
	HGDIOBJ v18;              // eax@19
	int v19;                  // eax@21
	CHAR v20;                 // cl@21
	int v21;                  // ebp@23
	UINT v22;                 // ebp@25
	unsigned int v23;         // edi@25
	int v24;                  // esi@25
	int v25;                  // ecx@26
	int v26;                  // ebp@26
	int v27;                  // edi@26
	int v28;                  // ecx@27
	int v29;                  // eax@29
	int v30;                  // esi@30
	HWND v32;                 // eax@52
	CHAR String;              // [sp+12h] [bp-4AAh]@9
	char v34;                 // [sp+13h] [bp-4A9h]@9
	HGDIOBJ i;                // [sp+14h] [bp-4A8h]@5
	int v36;                  // [sp+18h] [bp-4A4h]@19
	int v37;                  // [sp+1Ch] [bp-4A0h]@8
	struct tagSIZE psizl;     // [sp+20h] [bp-49Ch]@8
	HBITMAP ho;               // [sp+28h] [bp-494h]@5
	void* v40;                // [sp+2Ch] [bp-490h]@5
	void* v41;                // [sp+30h] [bp-48Ch]@5
	int v42;                  // [sp+34h] [bp-488h]@19
	HDC v43;                  // [sp+38h] [bp-484h]@5
	int v44;                  // [sp+3Ch] [bp-480h]@18
	UINT cLines;              // [sp+40h] [bp-47Ch]@18
	int v46;                  // [sp+44h] [bp-478h]@5
	HGDIOBJ v47;              // [sp+48h] [bp-474h]@5
	RECT rect;                // [sp+4Ch] [bp-470h]@23
	struct tagTEXTMETRICA tm; // [sp+5Ch] [bp-460h]@7
	BITMAPINFOHEADER pbmih;   // [sp+94h] [bp-428h]@18
	if( a2 )
		*(uint *)a2 = 0;
	if( h != 0 ? -(a2 != 0) : 0 ){
		ho = 0;
		v40 = 0;
		v47 = 0;
		i = 0;
		v46 = 0;
		v41 = 0;
		v6 = GetDesktopWindow();
		v7 = GetDC(v6);
		v5 = v7;
		v43 = v7;
		v3 = CreateCompatibleDC(v7);
		v4 = v3;
		if( v5 ){
			if( v3 ){
				v47 = SelectObject(v3, h);
				memset(&tm, 0, sizeof(tm));
				GetTextMetricsA(v4, &tm);
				if( tm.tmPitchAndFamily & 4 ){
					psizl.cx = 0;
					psizl.cy = 0;
					GetTextExtentPoint32A(v4, "W_y,|'", strlen("W_y,|'") - 1, &psizl);
					psizl.cx = 0;
					v37 = 0;
					v40 = (void*)SMemAlloc(2048, "SGDI.CPP", 505, 0);
					v8 = (int)v40;
					do{
						String = v37;
						v34 = 0;
						GetTextExtentPoint32A(v4, &String, 1, (LPSIZE)v8);
						v9 = psizl.cx;
						if( psizl.cx <= *(uint*)v8 + 1 ){
							v9 = *(uint*)v8 + 1;
							psizl.cx = *(uint*)v8 + 1;
						}
						v10 = psizl.cy;
						if( psizl.cy <= *(uint*)(v8 + 4) + 1 ){
							v10 = *(uint*)(v8 + 4) + 1;
							psizl.cy = *(uint*)(v8 + 4) + 1;
						}
						v8 += 8;
						v12 = __SETO__(v37 + 1, 256);
						v11 = v37++ - 255 < 0;
					}while( v11 ^ v12 );
					if( v9 ){
						if( v10 ){
							if( v9 & 3 ){
								v9 += 4 - (v9 & 3);
								psizl.cx = v9;
							}
							v15 = 16 * v9;
							memset(&pbmih, 0, 0x428u);
							pbmih.biPlanes = 1;
							pbmih.biBitCount = 1;
							v14 = 16 * v10;
							v44 = 16 * v9;
							cLines = 16 * v10;
							pbmih.biSize = 40;
							pbmih.biWidth = 16 * v9;
							pbmih.biHeight = 16 * v10;
							pbmih.biCompression = 0;
							v13 = CreateDIBitmap(v4, &pbmih, 0, 0, (BITMAPINFO*)&pbmih, 0);
							ho = v13;
							if( v13 ){
								v42 = (int)SelectObject(v4, v13);
								v17 = GetStockObject(0);
								v36 = (int)SelectObject(v4, v17);
								v18 = GetStockObject(6);
								i = SelectObject(v4, v18);
								Rectangle(v4, 0, 0, v15, v14);
								SelectObject(v4, (HGDIOBJ)v36);
								SelectObject(v4, i);
								SetTextAlign(v4, 0);
								SetTextColor(v4, 0);
								SetBkColor(v4, 0xFFFFFFu);
								SetBkMode(v4, 2);
								LOWORD_IDA(v37) = word_1501C788;
								v16 = 0;
								for( i = 0; ; v16 = (int)i ){
									v19 = 0;
									v20 = 16 * v16;
									String = 16 * v16;
									v36 = v16 + 1;
									while( 1 ){
										v21 = v19 + 1;
										LOBYTE_IDA(v37) = v20 + v19;
										rect.top = psizl.cy * (uint)i;
										rect.left = psizl.cx * v19;
										rect.right = (v19 + 1) * psizl.cx;
										rect.bottom = v36 * psizl.cy;
										ExtTextOutA(v4, psizl.cx * v19 + 1, psizl.cy * (uint)i + 1, 2u, &rect, (LPCSTR)&v37, 1u, 0);
										v19 = v21;
										if( v21 >= 16 )
											break;
										v20 = String;
									}
									i = (HGDIOBJ)v36;
									if( v36 >= 16 )
										break;
								}
								SelectObject(v4, (HGDIOBJ)v42);
								v24 = v44;
								v22 = cLines;
								v23 = cLines * v44;
								i = (HGDIOBJ)SMemAlloc((int)(cLines * v44) / 8, "SGDI.CPP", 577, 0);
								if( GetDIBits(v4, ho, 0, v22, i, (LPBITMAPINFO)&pbmih, 0) ){
									v25 = SMemAlloc(v23, "SGDI.CPP", 582, 0);
									v41 = (void*)v25;
									v27 = (int)((char*)i + (int)(v24 * (v22 - 1)) / 8);
									v26 = v25;
									if( (int)cLines > 0 ){
										v36 = cLines;
										v28 = v24 / 8;
										v42 = v24 / 8;
										do{
											if( v28 > 0 ){
												v29 = v28;
												do{
													v30 = 128;
													do{
														*(uchar*)v26++ = -((*(uchar*)v27&(uchar)v30) != 0);
														v30 >>= 1;
													}while( v30 );
													++v27;
													--v29;
												}while( v29 );
												v24 = v44;
												v28 = v42;
											}
											v27 += v24 / -4;
										}while( v36-- != 1 );
										v25 = (int)v41;
									}
									v46 = SGdiCreateFont((void*)v25, v24, cLines, 8, psizl.cx, psizl.cy, v40, a2);
								}
							}
							v5 = v43;
						}
					}
				}
			}
		}
		if( v41 )
			SMemFree(v41, "SGDI.CPP", 609, 0);
		if( i )
			SMemFree(i, "SGDI.CPP", 611, 0);
		if( v40 )
			SMemFree((void*)v40, "SGDI.CPP", 613, 0);
		if( v4 ){
			if( v47 )
				SelectObject(v4, v47);
		}
		if( ho )
			DeleteObject(ho);
		if( v4 )
			DeleteDC(v4);
		if( v5 ){
			v32 = GetDesktopWindow();
			ReleaseDC(v32, v5);
		}
		result = v46;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C788: using guessed type __int16 word_1501C788;
//----- (1500FDA0) --------------------------------------------------------
int __stdcall SGdiLoadFont(char* Str, int a2, int a3, int a4, void* a5, int a6)
{
	int v6;     // ecx@3
	int result; // eax@6
	char* v8;   // eax@14
	int v9;     // ecx@14
	char* v10;  // eax@17
	int v11;    // [sp+10h] [bp-80Ch]@7
	int v12;    // [sp+14h] [bp-808h]@7
	int v13;    // [sp+18h] [bp-804h]@7
	char v14;   // [sp+1Ch] [bp-800h]@18
	char v15;   // [sp+20h] [bp-7FCh]@14
	if( a4 || a5 )
		v6 = -1;
	else
		v6 = 0;
	if( (a6 != 0 ? (*Str != 0 ? (a3 != 0 ? (a2 != 0 ? -(Str != 0) : 0) : 0) : 0) : 0) & v6 ){
		result = SBmpLoadImage(Str, 0, 0, 0, (int)&v11, (int)&v12, (int)&v13);
		if( result ){
			result = sub_1500F2F0(v11 * v12);
			if( result ){
				result = SBmpLoadImage(Str, 0, dword_1501F0F4, v11 * v12, 0, 0, 0);
				if( result ){
					if( a5 )
						goto LABEL_22;
					v8 = &v15;
					v9 = 256;
					do{
						*((uint*)v8 - 1) = a4;
						*(uint*)v8 = a4;
						v8 += 8;
						--v9;
					}while( v9 );
					if( a5 )
						LABEL_22:
						v10 = (char*)a5;
					else
						v10 = &v14;
					result = SGdiCreateFont((void*)dword_1501F0F4, v11, v12, v13, a2, a3, v10, a6);
				}
			}
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (1500FF00) --------------------------------------------------------
void __stdcall SGdiRectangle(int a1, int a2, int a3, int a4, int a5, unsigned int a6)
{
	int v6;          // eax@5
	int v7;          // edx@5
	unsigned int v8; // ecx@5
	int v9;          // ebx@5
	int v10;         // ebp@5
	int v11;         // edi@5
	int v12;         // esi@5
	int v13;         // ecx@14
	if( !a1 ){
		SErrSetLastError(0x57u);
		return;
	}
	if( dword_1501F0E8 ){
		v8 = a6;
		v11 = a2;
		v10 = a3;
		v6 = a4;
		v12 = a5;
		v9 = dword_1501E1B4;
		v7 = 0;
		if( a6 & 0x1000000 ){
			if( dword_1501E1B4 == 8 ){
				v7 = v8 & 0xFFFFFF;
				goto LABEL_12;
			}
		}else{
			if( dword_1501E1B4 != 8 ){
				if( dword_1501E1B4 == 16 ){
					v9 = dword_1501E1B4;
					v7 = ((unsigned int)(unsigned __int8)a6 >> 3) | 4 * (32 * ((a6 >> 16) & 0xF8) | (unsigned __int8)(BYTE1(v8) & 0xF8));
				}
				LABEL_12:
				if( v9 == 8 ){
					v13 = (v7 | ((v7 | (v7 << 8)) << 8)) << 8;
				}else{
					if( v9 != 16 ){
						LABEL_17:
						if( a2 < 0 )
							v11 = 0;
						if( a3 < 0 )
							v10 = 0;
						if( a4 >= dword_1501E1B8 )
							v6 = dword_1501E1B8 - 1;
						if( a5 >= dword_1501E1BC )
							v12 = dword_1501E1BC - 1;
						SBltROP3(a1 + (v11 << dword_1501F0EC) + *(uint*)(dword_1501F0E8 + 4 * v10), 0, (v6 - v11 + 1) << dword_1501F0EC, v12 - v10 + 1, dword_1501E1B0, 0, v7, 0xF00021u);
						return;
					}
					v13 = v7 << 16;
				}
				v7 |= v13;
				goto LABEL_17;
			}
		}
		v7 = 0;
		goto LABEL_12;
	}
}

// 1501E1B0: using guessed type int dword_1501E1B0;
// 1501E1B4: using guessed type int dword_1501E1B4;
// 1501E1B8: using guessed type int dword_1501E1B8;
// 1501E1BC: using guessed type int dword_1501E1BC;
// 1501F0E8: using guessed type int dword_1501F0E8;
// 1501F0EC: using guessed type int dword_1501F0EC;
//----- (15010040) --------------------------------------------------------
int __stdcall SGdiSelectObject(int a1)
{
	int result; // eax@4
	if( a1 && *(ushort*)(a1 + 8) == 20301 && *(ushort*)(a1 + 10) < 1u ){
		dword_1501F0F0[*(ushort*)(a1 + 10)] = a1;
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 1501F0F0: using guessed type int dword_1501F0F0[];
//----- (15010080) --------------------------------------------------------
int __stdcall SGdiSetPitch(int a1)
{
	int i;  // eax@8
	int v3; // ecx@8
	int v4; // esi@8
	if( a1 <= 0 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( a1 == dword_1501E1B0 ){
		if( dword_1501F0E8 )
			return 1;
	}else{
		if( dword_1501F0E8 )
			SMemFree((void*)dword_1501F0E8, "SGDI.CPP", 730, 0);
	}
	SDrawGetScreenSize((int)&dword_1501E1B8, (int)&dword_1501E1BC, (int)&dword_1501E1B4);
	v4 = dword_1501E1BC + 64;
	dword_1501F0EC = dword_1501E1B4 == 16;
	dword_1501F0E8 = SMemAlloc(4 * (dword_1501E1BC + 64), "SGDI.CPP", 739, 0);
	v3 = 0;
	for( i = 0; i < v4; ++i ){
		*(uint*)(dword_1501F0E8 + 4 * i) = v3;
		v3 += a1;
	}
	dword_1501E1B0 = a1;
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E1B0: using guessed type int dword_1501E1B0;
// 1501E1B4: using guessed type int dword_1501E1B4;
// 1501E1B8: using guessed type int dword_1501E1B8;
// 1501E1BC: using guessed type int dword_1501E1BC;
// 1501F0E8: using guessed type int dword_1501F0E8;
// 1501F0EC: using guessed type int dword_1501F0EC;
//----- (15010140) --------------------------------------------------------
void __stdcall SGdiTextOut(int a1, int a2, int a3, uint a4, char* a5, uint a6)
{
	int v6;  // eax@2
	int v7;  // eax@3
	int v8;  // [sp+0h] [bp-10h]@4
	int v9;  // [sp+4h] [bp-Ch]@4
	int v10; // [sp+8h] [bp-8h]@4
	int v11; // [sp+Ch] [bp-4h]@4
	if( a4 ){
		v7 = -(a4 != 0x10000FF);
		LOBYTE_IDA(v7) = v7 & 0xFE;
		v6 = v7 + 3;
	}else{
		v6 = 2;
	}
	v8 = 0;
	v10 = 0x7FFFFFFF;
	v11 = 0x7FFFFFFF;
	v9 = 0;
	SGdiExtTextOut(a1, a2, a3, (int)&v8, a4, v6, 0, a5, a6);
}

// char fillFlag : 8 - zero allocated memory
//----- (150101B0) --------------------------------------------------------
int __stdcall SMemAlloc(uint size, char* cfile_name, int cstring_number, char fillFlag)
{
	//if( ExternalStorm ) return eSMemAlloc(size, cfile_name, cstring_number, fillFlag);
	void* allocated = malloc(size);
	if( fillFlag & 8 ){
		memset(allocated, 0, size);
	}
	return (int)allocated;
	//-------------------------------------------------------------------------
	int i;                             // eax@3
	char* v6;                          // edx@3
	int v7;                            // esi@3
	char v8;                           // cl@4
	unsigned int v9;                   // eax@9
	struct _RTL_CRITICAL_SECTION* v10; // ebp@9
	int v11;                           // ebx@13
	unsigned int v12;                  // esi@13
	int v13;                           // eax@21
	unsigned int v14;                  // ecx@21
	char v15;                          // dl@24
	int j;                             // ecx@24
	int v17;                           // edi@24
	unsigned int v18;                  // ecx@24
	if( !dword_15021100 )
		return 0;
	v7 = cstring_number;
	v6 = cfile_name;
	for( i = -286331154; v6; i += 32*i + v7 + v8 + 5 ){
		v8 = *(uchar*)v6;
		if( !*(uchar*)v6 )
			break;
		++v6;
		if( v8 >= 97 ){
			if( v8 <= 122 )
				v8 -= 32;
		}
		v7 = dword_1501C798[v8 & 0xF] ^ (i + v7);
	}
	v10 = &stru_1501F100 + (unsigned __int8)v7;
	EnterCriticalSection(&stru_1501F100 + (unsigned __int8)v7);
	v9 = dword_15020D00[(unsigned __int8)v7];
	if( v9 ){
		while( *(uint*)v9 != v7 ){
			v9 = *(uint*)(v9 + 4);
			if( !v9 )
				goto LABEL_12;
		}
	}else{
		LABEL_12:
		v9 = (unsigned int)MemVirtualAllocHeader(cfile_name, cstring_number, v7, (unsigned __int8)v7, 0x10000u);
	}
	v12 = v9;
	v11 = 0;
	if( v9 )
		v11 = MemVirtualAlloc(v9, size);
	if( dword_15021104 ){
		if( dword_15021104 != v12 )
			MemVirtualFreeHeader();
	}
	LeaveCriticalSection(v10);
	if( !v11 )
		SErrDisplayError(8u, (int)cfile_name, cstring_number, 0, 0, 1u);
	if( fillFlag & 8 ){
		v14 = size;
		v13 = 0;
		goto LABEL_24;
	}
	if( GetStormDebugLevel() ){
		v14 = size;
		v13 = 0xEEEEEEEE;
		LABEL_24:
		v15 = v14;
		v18 = v14 >> 2;
		memset((void*)v11, v13, 4 * v18);
		v17 = v11 + 4 * v18;
		for( j = v15 & 3; j; --j )
			*(uchar *)v17++ = v13;
	}
	return v11;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C798: using guessed type int dword_1501C798[];
// 15020D00: using guessed type int dword_15020D00[];
// 15021100: using guessed type int dword_15021100;
// 15021104: using guessed type int dword_15021104;
//----- (150102D0) --------------------------------------------------------
void* __fastcall MemVirtualAllocHeader(char* cfile_name, int cstring_number, int a3, int a4, SIZE_T dwSize)
{
	void* result;     // eax@1
	unsigned int v7;  // ebp@1
	int v9;           // eax@5
	unsigned int v10; // ecx@5
	int v11;          // edi@7
	int v12;          // eax@7
	result = VirtualAlloc(0, dwSize, MEM_RESERVE, PAGE_NOACCESS);
	v7 = (unsigned int)result;
	if( result ){
		result = VirtualAlloc(result, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 
		if( result ){
			v10 = strlen(cfile_name);
			v9 = v10 + 43;
			if( ((uchar)v10 + 43) & 3 )
				v9 += 4 - (((uchar)v10 + 43) & 3);
			*(uint*)v7 = a3;
			v11 = dword_15020D00[a4];
			*(uint*)(v7 + 16) = v7 + v9;
			*(uint*)(v7 + 4) = v11;
			*(uint*)(v7 + 32) = dwSize;
			*(uint*)(v7 + 8) = a4;
			*(uint*)(v7 + 24) = 0;
			*(uint*)(v7 + 28) = 4096;
			*(uint*)(v7 + 36) = cstring_number;
			memcpy((void*)(v7 + 40), cfile_name, v10);
			v12 = *(uint*)(v7 + 16);
			*(ushort*)v12 = 8;
			*(uchar*)(v12 + 2) = 0;
			*(uchar*)(v12 + 3) = 16;
			*(ushort*)(v12 + 4) = v7 >> 16;
			*(ushort*)(v12 + 6) = 28525;
			*(uint*)(v7 + 20) = v12;
			dword_15020D00[a4] = v7;
			result = (void*)v7;
		}
	}
	return result;
}

// 15020D00: using guessed type int dword_15020D00[];
//----- (150103B0) --------------------------------------------------------
int __fastcall MemVirtualAlloc(uint a1, uint size)
{
	unsigned int v2;  // ebx@1
	unsigned int v3;  // edi@1
	int v4;           // esi@1
	int v5;           // eax@4
	int v6;           // eax@6
	int v7;           // ebp@6
	int v8;           // ecx@6
	int v9;           // edx@8
	int v10;          // ecx@8
	int v11;          // ebx@8
	__int16 i;        // si@8
	ushort* v13;       // esi@15
	int v14;          // eax@16
	int v15;          // eax@18
	SIZE_T v16;       // ecx@18
	int result;       // eax@21
	unsigned int v18; // ecx@24
	int v19;          // eax@25
	int v20;          // eax@25
	SIZE_T v21;       // esi@25
	unsigned int v22; // ebx@29
	LPVOID v23;       // eax@32
	int v24;          // edx@32
	ushort* v25;       // eax@33
	int v26;          // ecx@33
	int v27;          // ebp@33
	int v28;          // ebp@33
	int v29;          // ecx@36
	int v30;          // ebp@36
	int v31;          // eax@48
	int v32;          // eax@49
	int v33;          // [sp+10h] [bp-18h]@2
	int v34;          // [sp+10h] [bp-18h]@33
	int v35;          // [sp+14h] [bp-14h]@1
	int v36;          // [sp+14h] [bp-14h]@33
	int v37;          // [sp+18h] [bp-10h]@6
	unsigned int v38; // [sp+1Ch] [bp-Ch]@1
	unsigned int v39; // [sp+20h] [bp-8h]@1
	int v40;          // [sp+20h] [bp-8h]@34
	unsigned int v41; // [sp+24h] [bp-4h]@29
	v2 = size;
	v3 = a1;
	v39 = size;
	v4 = size > 0xFEBF;
	v38 = a1;
	v35 = size > 0xFEBF;
	if( !GetStormDebugLevel() || (v33 = 1, v4) )
		v33 = 0;
	v5 = 4;
	if( !v4 )
		v5 = v2;
	v8 = v33 != 0 ? 2 : 0;
	v7 = v8 + v5 + 8;
	v37 = v8 + v5 + 8;
	v6 = ((uchar)v8 + (uchar)v5 + 8) & 7;
	if( (ushort)v6 )
		v7 = v7 - v6 + 8;
	v10 = *(uint*)(v3 + 24);
	v9 = v3 + 24;
	v11 = 0;
	for( i = -1; v10; v10 = *(uint*)(v10 + 4) ){
		if( *(ushort*)v10 >= (ushort)v7 ){
			LOWORD_IDA(v6) = *(ushort*)v10;
			v6 -= v7;
			if( (ushort)v6 < (unsigned __int16)i ){
				i = v6;
				v11 = v9;
			}
			if( (ushort)v6 < 0x10u )
				break;
		}
		v9 = v10 + 4;
	}
	if( v11 ){
		v13 = *(ushort**)v11;
		if( **(ushort**)v11 - (unsigned int)(unsigned __int16)v7 < 0x10 ){
			*(uint*)v11 = *((uint*)v13 + 1);
		}else{
			v14 = (int)((char*)v13 + (unsigned __int16)v7);
			*(ushort*)v14 = **(ushort**)v11 - v7;
			*(uchar*)(v14 + 2) = 0;
			*(uchar*)(v14 + 3) = 2;
			*(uint*)(v14 + 4) = *(uint*)(*(uint*)v11 + 4);
			*v13 = v7;
			*(uint*)v11 = v14;
		}
	}else{
		v15 = (unsigned __int16)v7 - v3 + *(uint*)(v3 + 20) + 8;
		v16 = *(uint*)(v3 + 32);
		if( v15 > v16 ){
			if( v16 < 0x10000000 )
				v16 *= 2;
			result = (int)MemVirtualAllocHeader((char*)(v3 + 40), *(uint*)(v3 + 36), *(uint*)v3, *(uint*)(v3 + 8), v16);
			v3 = result;
			v38 = result;
			if( !result )
				return result;
			v15 = (unsigned __int16)v7 - result + *(uint*)(result + 20) + 8;
		}
		v18 = *(uint*)(v3 + 28);
		if( v15 > v18 ){
			v20 = v15 - v18;
			v21 = v20;
			v19 = v20 & 0xFFF;
			if( v19 )
				v21 += 4096 - v19;
			VirtualAlloc((LPVOID)(v3 + v18), v21, MEM_COMMIT, 0x40/*PAGE_EXECUTE_READWRITE*/);
			*(uint*)(v3 + 28) += v21;
		}
		v13 = *(ushort**)(v3 + 20);
		*v13 = v7;
		*(ushort*)((char*)v13 + (unsigned __int16)v7) = 8;
		*((uchar*)v13 + (unsigned __int16)v7 + 2) = 0;
		*((uchar*)v13 + (unsigned __int16)v7 + 3) = 16;
		*(ushort*)((char*)v13 + (unsigned __int16)v7 + 4) = v3 >> 16;
		*(ushort*)((char*)v13 + (unsigned __int16)v7 + 6) = 28525;
		*(uint*)(v3 + 20) = (uint)(char*)v13 + (unsigned __int16)v7;
	}
	v22 = v3 >> 16;
	*((uchar*)v13 + 2) = *(uchar*)v13 - v37;
	v13[2] = v3 >> 16;
	*((uchar*)v13 + 3) = v35 != 0 ? 4 : 0;
	v13[3] = 28525;
	v41 = v3 >> 16;
	++*(uint*)(v3 + 12);
	if( v33 ){
		*((uchar*)v13 + 3) |= 1u;
		*(ushort*)((char*)v13 + (unsigned __int16)v37 - 2) = -20206;
	}
	if( !v35 )
		return (int)(v13 + 4);
	v23 = VirtualAlloc(0, v39 + 12, MEM_COMMIT, 0x40/*PAGE_EXECUTE_READWRITE*/);
	if( v23 ){
		*(uint*)v23 = (uint)v13;
		*((ushort*)v23 + 2) = (v39 + 65535) >> 16;
		*((uchar*)v23 + 6) = 0;
		*((uchar*)v23 + 7) = 12;
		*((ushort*)v23 + 4) = v22;
		*((ushort*)v23 + 5) = 28525;
		return (int)((char*)v23 + 12);
	}
	v28 = *v13;
	*((uchar*)v13 + 3) = 2;
	v34 = v3 + 24;
	v26 = *(uint*)(v3 + 24);
	v27 = (int)((char*)v13 + v28);
	v25 = v13;
	v36 = v27;
	if( !v26 )
		goto LABEL_45;
	do{
		LOWORD_IDA(v3) = *(ushort*)v26;
		LOWORD_IDA(v24) = *v25;
		v40 = *(ushort*)v26;
		v27 = v36;
		if( (unsigned int) * v25 + v40 > 0xFFFF )
			goto LABEL_55;
		if( v26 == v36 ){
			v3 += v24;
			v24 = v34;
			HIWORD_IDA(v30) = 0;
			*v25 = v3;
			LOWORD_IDA(v30) = *v13;
			v29 = 1;
			v27 = (int)((char*)v13 + v30);
			v36 = v27;
			goto LABEL_40;
		}
		if( (ushort*)(v26 + v40) != v25 ){
			LABEL_55:
			v24 = v34;
			v29 = 0;
		}else{
			v3 += v24;
			v24 = v34;
			*(ushort*)v26 = v3;
			v29 = 1;
			v25 = *(ushort**)v34;
		}
		LABEL_40:
		if( v29 ){
			*(uint*)v24 = *(uint*)(*(uint*)v24 + 4);
		}else{
			v24 = *(uint*)v24 + 4;
			v34 = v24;
		}
		v26 = *(uint*)v24;
	}while( *(uint*)v24 );
	v3 = v38;
	LOWORD_IDA(v22) = v41;
	LABEL_45:
	if( *(uchar*)(v27 + 3) & 0x10 ){
		*v25 = 8;
		*((uchar*)v25 + 2) = 0;
		*((uchar*)v25 + 3) = 16;
		v25[2] = v22;
		v25[3] = 28525;
		*(uint*)(v3 + 20) = (uint)v25;
	}else{
		*((uint*)v25 + 1) = *(uint*)(v3 + 24);
		*(uint*)(v3 + 24) = (uint)v25;
	}
	v31 = *(uint*)(v3 + 12) - 1;
	*(uint*)(v3 + 12) = v31;
	if( !v31 ){
		v32 = *(uint*)(v3 + 8);
		dword_15021104 = v3;
		dword_15020900[v32] = 1;
	}
	return 0;
}

// 15020900: using guessed type int dword_15020900[];
// 15021104: using guessed type int dword_15021104;
//----- (15010710) --------------------------------------------------------
void __stdcall MemVirtualFreeHeader()
{
	struct _RTL_CRITICAL_SECTION* v0; // ebx@1
	int v1;                           // edi@1
	int v2;                           // esi@3
	int v3;                           // eax@4
	dword_15021104 = 0;
	v0 = &stru_1501F100;
	v1 = 0;
	do{
		if( *(int*)((char*)dword_15020900 + v1) ){
			EnterCriticalSection(v0);
			v2 = (int)((char*)dword_15020D00 + v1);
			*(int*)((char*)dword_15020900 + v1) = 0;
			if( *(int*)((char*)dword_15020D00 + v1) ){
				do{
					v3 = *(uint*)v2;
					if( *(uint*)(*(uint*)v2 + 12) ){
						v2 = v3 + 4;
					}else{
						*(uint*)v2 = *(uint*)(v3 + 4);
						VirtualFree((LPVOID)v3, 0, 0x8000u);
					}
				}while( *(uint*)v2 );
			}
			LeaveCriticalSection(v0);
		}
		++v0;
		v1 += 4;
	}while( (unsigned int)v0 < (unsigned int)dword_15020900 );
}

// 15020900: using guessed type int dword_15020900[];
// 15020D00: using guessed type int dword_15020D00[];
// 15021104: using guessed type int dword_15021104;
//----- (15010780) --------------------------------------------------------
int __stdcall SMemDestroy()
{
	int v0;                           // ebx@1
	struct _RTL_CRITICAL_SECTION* v1; // edi@2
	int v2;                           // esi@3
	int v3;                           // eax@4
	v0 = 0;
	if( dword_15021100 ){
		dword_15021100 = 0;
		v1 = &stru_1501F100;
		do{
			EnterCriticalSection(v1);
			v2 = (int)((char*)dword_15020D00 + v0);
			*(int*)((char*)dword_15020900 + v0) = 0;
			if( *(int*)((char*)dword_15020D00 + v0) ){
				do{
					v3 = *(uint*)v2;
					if( *(uint*)(*(uint*)v2 + 12) ){
						v2 = v3 + 4;
					}else{
						*(uint*)v2 = *(uint*)(v3 + 4);
						VirtualFree((LPVOID)v3, 0, 0x8000u);
					}
				}while( *(uint*)v2 );
			}
			LeaveCriticalSection(v1);
			DeleteCriticalSection(v1);
			++v1;
			v0 += 4;
		}while( (unsigned int)v1 < (unsigned int)dword_15020900 );
	}
	return 1;
}

// 15020900: using guessed type int dword_15020900[];
// 15020D00: using guessed type int dword_15020D00[];
// 15021100: using guessed type int dword_15021100;
//void* mem, char* cfile_name, int cstring_number, int a4
//----- (15010810) --------------------------------------------------------
int __stdcall SMemFree(void* mem, char* cfile_name, int cstring_number, int a4)
{
	//if( ExternalStorm ) return eSMemFree(mem, cfile_name, cstring_number, a4);
	free(mem);
	return 0;
	//-------------------------------------------------------------------------
	char* v4;         // esi@2
	char v5;          // al@3
	char v6;          // al@7
	int i;            // ecx@8
	char* v8;         // edi@8
	char v9;          // dl@8
	unsigned int v10; // ecx@8
	int v11;          // edx@13
	int v12;          // ebx@13
	unsigned int v13; // edi@13
	char* v14;        // eax@14
	int v15;          // ecx@14
	int v16;          // ebx@14
	int v17;          // ecx@17
	int v18;          // ebx@17
	int v19;          // eax@29
	int v20;          // eax@30
	int v22;          // [sp+10h] [bp-14h]@14
	char* v23;        // [sp+14h] [bp-10h]@2
	int v24;          // [sp+1Ch] [bp-8h]@13
	int v25;          // [sp+28h] [bp+4h]@14
	if( !dword_15021100
	|| (v4 = (char *)mem - 8, v23 = (char *)mem - 8, *((ushort *)mem - 1) != 28525)
	|| (v5 = v4[3], v5 & 2)
	|| v5 & 1 && *(ushort *)&v4[*(ushort *)v4 - (unsigned __int8)v4[2] - 2] != -20206 )
		return 0;
	if( GetStormDebugLevel() ){
		v6 = v4[3];
		if( !(v6 & 8) ){
			v9 = *(ushort*)v4 - v4[2] - 2 * (v6 & 1) - 8;
			v10 = (*(ushort*)v4 - (unsigned __int8)v4[2] - 2 * (v6 & 1u) - 8) >> 2;
			memset(mem, -572662307, 4 * v10);
			v8 = (char*)mem + 4 * v10;
			for( i = v9 & 3; i; --i )
				*v8++ = -35;
		}
	}
	if( v4[3] & 8 ){
		v4 = (char*)*((uint*)mem - 3);
		VirtualFree((char*)mem - 12, 0, 0x8000u);
		v23 = v4;
	}
	v13 = *((ushort*)v4 + 2) << 16;
	EnterCriticalSection(&stru_1501F100 + *(uint*)((*((ushort*)v4 + 2) << 16) + 8));
	v11 = *(uint*)(v13 + 8);
	v12 = 0;
	v24 = *(uint*)(v13 + 8);
	if( !v13 )
		goto LABEL_32;
	LOWORD_IDA(v12) = *(ushort*)v4;
	v4[3] = 2;
	v25 = v13 + 24;
	v15 = *(uint*)(v13 + 24);
	v16 = (int)&v4[v12];
	v14 = v4;
	v22 = v16;
	if( !v15 )
		goto LABEL_26;
	do{
		LOWORD_IDA(v4) = *(ushort*)v15;
		LOWORD_IDA(v11) = *(ushort*)v14;
		v16 = v22;
		if( *(ushort*)v14 + (unsigned int) * (ushort*)v15 > 0xFFFF )
			goto LABEL_36;
		if( v15 == v22 ){
			v4 += v11;
			v11 = v25;
			HIWORD_IDA(v18) = 0;
			*(ushort*)v14 = (ushort)v4;
			LOWORD_IDA(v18) = *(ushort*)v23;
			v16 = (int)&v23[v18];
			v17 = 1;
			v22 = v16;
			goto LABEL_21;
		}
		if( (char*)(v15 + *(ushort*)v15) != v14 ){
			LABEL_36:
			v11 = v25;
			v17 = 0;
		}else{
			v4 += v11;
			v11 = v25;
			*(ushort*)v15 = (ushort)v4;
			v17 = 1;
			v14 = *(char**)v25;
		}
		LABEL_21:
		if( v17 ){
			*(uint*)v11 = *(uint*)(*(uint*)v11 + 4);
		}else{
			v11 = *(uint*)v11 + 4;
			v25 = v11;
		}
		v15 = *(uint*)v11;
	}while( *(uint*)v11 );
	v11 = v24;
	LABEL_26:
	if( *(uchar*)(v16 + 3) & 0x10 ){
		v14[3] = 16;
		*(ushort*)v14 = 8;
		v14[2] = 0;
		*((ushort*)v14 + 2) = v13 >> 16;
		*((ushort*)v14 + 3) = 28525;
		*(uint*)(v13 + 20) = (uint)v14;
	}else{
		*((uint*)v14 + 1) = *(uint*)(v13 + 24);
		*(uint*)(v13 + 24) = (uint)v14;
	}
	v19 = *(uint*)(v13 + 12) - 1;
	*(uint*)(v13 + 12) = v19;
	if( !v19 ){
		v20 = *(uint*)(v13 + 8);
		dword_15021104 = v13;
		dword_15020900[v20] = 1;
	}
	v12 = 1;
	LABEL_32:
	LeaveCriticalSection(&stru_1501F100 + v11);
	return v12 != 0;
}

// 15020900: using guessed type int dword_15020900[];
// 15021100: using guessed type int dword_15021100;
// 15021104: using guessed type int dword_15021104;
//----- (15010A30) --------------------------------------------------------
void __stdcall InitCriticalStormArray()
{
	struct _RTL_CRITICAL_SECTION* v0; // esi@2
	if( !dword_15021100 ){
		dword_15021100 = 1;
		v0 = &stru_1501F100;
		do{
			InitializeCriticalSection(v0);
			++v0;
		}while( (unsigned int)v0 < (unsigned int)dword_15020900 );
	}
}

// 15020900: using guessed type int dword_15020900[];
// 15021100: using guessed type int dword_15021100;
//----- (15010A70) --------------------------------------------------------
int __stdcall SMsgDestroy()
{
	void** i; // eax@1
	for( i = (void**)dword_15021108; dword_15021108; i = (void**)dword_15021108 )
		sub_15010A90(*i);
	return 1;
}

// 15021108: using guessed type int dword_15021108;
//----- (15010A90) --------------------------------------------------------
int __thiscall sub_15010A90(void* This)
{
	int result; // eax@1
	void* v2;   // edi@1
	int v3;     // esi@1
	int i;      // eax@3
	int v5;     // ecx@3
	v3 = (int)This;
	result = sub_15010B10(This);
	v2 = (void*)result;
	if( result ){
		SEvtUnregisterType(1397576519, v3);
		SEvtUnregisterType(1397576520, v3);
		SEvtUnregisterType(1397576522, v3);
		SEvtUnregisterType(1397576521, v3);
		result = (int)&dword_15021108;
		if( &dword_15021108 ){
			v5 = (int)&dword_15021108;
			for( i = dword_15021108; i; i = *(uint*)(i + 4) ){
				if( (void*)i == v2 )
					break;
				v5 = i + 4;
			}
			if( *(uint*)v5 )
				*(uint *)v5 = *(uint *)(*(uint *)v5 + 4);
			result = SMemFree(v2, "SMSG.CPP", 49, 0);
		}
	}
	return result;
}

// 15021108: using guessed type int dword_15021108;
//----- (15010B10) --------------------------------------------------------
int __thiscall sub_15010B10(void* This)
{
	int result; // eax@1
	for( result = dword_15021108; result; result = *(uint*)(result + 4) ){
		if( *(void**)result == This )
			break;
	}
	return result;
}

// 15021108: using guessed type int dword_15021108;
//----- (15010B30) --------------------------------------------------------
int __stdcall SMsgDispatchMessage(int a1, int a2, unsigned int a3, int a4, int a5, int a6)
{
	int v7;           // [sp-10h] [bp-3Ch]@9
	int v8;           // [sp-Ch] [bp-38h]@9
	int v9;           // [sp-8h] [bp-34h]@9
	int v10;          // [sp-4h] [bp-30h]@9
	int v11;          // [sp+Ch] [bp-20h]@5
	int v12;          // [sp+10h] [bp-1Ch]@5
	unsigned int v13; // [sp+14h] [bp-18h]@5
	int v14;          // [sp+18h] [bp-14h]@5
	unsigned int v15; // [sp+1Ch] [bp-10h]@6
	int v16;          // [sp+24h] [bp-8h]@8
	int v17;          // [sp+28h] [bp-4h]@8
	if( a5 )
		*(uint *)a5 = 0;
	if( a6 )
		*(uint *)a6 = 0;
	v11 = a1;
	v12 = a2;
	v13 = a3;
	v14 = a4;
	if( a2 == 273 )
		v15 = a3 >> 16;
	else
		v15 = 0;
	v16 = 0;
	v17 = 0;
	SEvtDispatch(1397576519, a1, a2, (int)&v11);
	if( a2 == 273 ){
		v10 = (int)&v11;
		v9 = (unsigned __int16)a3;
		v8 = a1;
		v7 = 1397576520;
	}else{
		if( a2 == 256 ){
			v10 = (int)&v11;
			v9 = a3;
			v8 = a1;
			v7 = 1397576521;
		}else{
			if( a2 != 257 )
				goto LABEL_15;
			v10 = (int)&v11;
			v9 = a3;
			v8 = a1;
			v7 = 1397576522;
		}
	}
	SEvtDispatch(v7, v8, v9, v10);
	LABEL_15:
	if( a2 == 130 )
		sub_15010A90((void*)a1);
	if( a5 )
		*(uint *)a5 = v16;
	if( a6 )
		*(uint *)a6 = v17;
	return 1;
}

//----- (15010C40) --------------------------------------------------------
WPARAM __stdcall SMsgDoMessageLoop (int(__stdcall* a1)(uint), int a2)
{
	int v2;            // esi@1
	int v3;            // eax@4
	struct tagMSG Msg; // [sp+10h] [bp-1Ch]@2
	v2 = 0;
	while( 1 ){
		do{
			if( PeekMessageA(&Msg, 0, 0, 0, 0) )
				break;
			if( !a1 )
				break;
			v3 = v2++;
		}while( a1(v3) );
		v2 = 0;
		if( !GetMessageA(&Msg, 0, 0, 0) )
			break;
		TranslateMessage(&Msg);
		DispatchMessageA(&Msg);
	}
	if( a2 )
		StormDestroy();
	return Msg.wParam;
}

//----- (15010CC0) --------------------------------------------------------
int __stdcall SMsgRegisterCommand(int a1, int a2, int a3)
{
	return sub_15010CE0(1397576520, a1, a2, a3);
}

//----- (15010CE0) --------------------------------------------------------
int __fastcall sub_15010CE0(int a1, int a2, int a3, int a4)
{
	int v4;     // esi@1
	int result; // eax@2
	int v6;     // [sp+0h] [bp-4h]@1
	v6 = a2;
	v4 = a1;
	if( a4 ){
		if( !a2 ){
			SDrawGetFrameWindow((int)&v6);
			a2 = v6;
		}
		if( !sub_15010B10((void*)a2) )
			sub_15010D40((void*)v6);
		result = SEvtRegisterHandler(v4, v6, a3, 0, a4);
	}else{
		result = 0;
	}
	return result;
}

//----- (15010D40) --------------------------------------------------------
int* __thiscall sub_15010D40(void* This)
{
	int* result; // eax@1
	void* v2;    // esi@1
	int v3;      // edx@4
	char v4;     // [sp+0h] [bp-Ch]@2
	int v5;      // [sp+8h] [bp-4h]@4
	result = &dword_15021108;
	v2 = This;
	if( &dword_15021108 ){
		if( &v4 != (char*)-4 ){
			result = (int*)SMemAlloc(8, "SMSG.CPP", 30, 0);
			if( result ){
				v3 = v5;
				*result = (int)v2;
				result[1] = v3;
				result[1] = dword_15021108;
				dword_15021108 = (int)result;
			}
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021108: using guessed type int dword_15021108;
//----- (15010D90) --------------------------------------------------------
int __stdcall SMsgRegisterKeyDown(int a1, int a2, int a3)
{
	return sub_15010CE0(1397576521, a1, a2, a3);
}

//----- (15010DB0) --------------------------------------------------------
int __stdcall SMsgRegisterKeyUp(int a1, int a2, int a3)
{
	return sub_15010CE0(1397576522, a1, a2, a3);
}

//----- (15010DD0) --------------------------------------------------------
int __stdcall SMsgRegisterMessage(int a1, int a2, int a3)
{
	return sub_15010CE0(1397576519, a1, a2, a3);
}

//----- (15010DF0) --------------------------------------------------------
int __stdcall SetupNetCriticalSection()
{
	InitNetCriticalSection();
	return AtExit((int)DeleteNetCriticalSection);
}

//----- (15010E00) --------------------------------------------------------
void __stdcall InitNetCriticalSection()
{
	InitializeCriticalSection(&NetCritical);
}

//----- (15010E20) --------------------------------------------------------
void __stdcall DeleteNetCriticalSection()
{
	DeleteCriticalSection(&NetCritical);
}

//----- (15010E40) --------------------------------------------------------
int __stdcall sub_15010E40()
{
	int result; // eax@1
	result = 0;
	dword_15021110 = 0;
	dword_15021114 = 0;
	return result;
}

// 15021110: using guessed type int dword_15021110;
// 15021114: using guessed type int dword_15021114;
//----- (15010E60) --------------------------------------------------------
int __stdcall sub_15010E60()
{
	int result; // eax@1
	result = 0;
	dword_15021158 = 0;
	dword_1502115C = 0;
	return result;
}

// 15021158: using guessed type int dword_15021158;
// 1502115C: using guessed type int dword_1502115C;
//----- (15010E80) --------------------------------------------------------
int __stdcall sub_15010E80()
{
	int result; // eax@1
	result = 0;
	dword_15021160 = 0;
	dword_15021164 = 0;
	return result;
}

// 15021160: using guessed type int dword_15021160;
// 15021164: using guessed type int dword_15021164;
//----- (15010EA0) --------------------------------------------------------
int __stdcall sub_15010EA0()
{
	int result; // eax@1
	result = 0;
	dword_15021118 = 0;
	dword_1502111C = 0;
	return result;
}

// 15021118: using guessed type int dword_15021118;
// 1502111C: using guessed type int dword_1502111C;
//----- (15010EB0) --------------------------------------------------------
void __thiscall sub_15010EB0(int This)
{
	int v1;   // edi@1
	void* i;  // esi@1
	int v3;   // eax@2
	int v4;   // ecx@5
	char v5;  // dl@5
	int v6;   // edx@5
	int v7;   // eax@9
	int v8;   // [sp+10h] [bp-14h]@5
	int v9;   // [sp+14h] [bp-10h]@5
	int v10;  // [sp+18h] [bp-Ch]@5
	char v11; // [sp+1Ch] [bp-8h]@5
	int v12;  // [sp+1Dh] [bp-7h]@5
	v1 = This;
	for( i = *(void**)(This + 328); i; i = *(void**)(v1 + 328) ){
		v3 = *((uint*)i + 3);
		if( *(ushort*)(v3 + 4) != *(ushort*)(v1 + 390) )
			break;
		if( *(uchar*)(v3 + 9) < 0xEu )
			dword_15021120[*(uchar*)(v3 + 9)] = 1;
		v4 = *((uint*)i + 2);
		v11 = *(uchar*)(v3 + 10);
		v5 = *(uchar*)(v3 + 9);
		v8 = v4;
		LOBYTE_IDA(v12) = v5;
		v6 = *(ushort*)(v3 + 2);
		v9 = v3 + 12;
		v10 = v6 - 12;
		SEvtDispatch(1397638484, 2, (unsigned __int8)v12, (int)&v8);
		++*(ushort*)(v1 + 390);
		(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(
			*((uint *)i + 2),
			*((uint *)i + 3),
			*((uint *)i + 4));
		if( *(uint*)i ){
			*(uint*)(*(uint*)i + 4) = *((uint*)i + 1);
		}else{
			if( *(void**)(v1 + 328) == i )
				*(uint *)(v1 + 328) = *((uint *)i + 1);
		}
		v7 = *((uint*)i + 1);
		if( v7 ){
			*(uint*)v7 = *(uint*)i;
		}else{
			if( *(void**)(v1 + 332) == i )
				*(uint *)(v1 + 332) = *(uint *)i;
		}
		*(uint*)i = 0;
		*((uint*)i + 1) = 0;
		SMemFree(i, ".?AUMESSAGE@@", -2, 0);
	}
}

// 15021120: using guessed type int dword_15021120[];
// 15021184: using guessed type int dword_15021184;
//----- (15010FC0) --------------------------------------------------------
//int __stdcall	SNetCreateGame(char* a1, char* a2, char* a3, uint a4, int a5, int a6, int a7, char* a8, char* a9, int* playerid);	// 101
int __stdcall SNetCreateGame(char* Source, char* a2, char* a3, uint a4, void* a5, uint a6, int a7, char* a8, char* a9, int* playerId)
{
	//if( ExternalStorm ) return eSNetCreateGame(Source, a2, a3, a4, (int)a5, a6, a7, a8, a9, playerId);
	char* v11; // esi@7
	char* v12; // edi@9
	int v13;   // esi@17
	DWORD v14; // esi@23
	DWORD v15; // [sp-4h] [bp-24h]@6
	int v16;   // [sp+10h] [bp-10h]@18
	int v17;   // [sp+14h] [bp-Ch]@18
	int v18;   // [sp+18h] [bp-8h]@18
	int v19;   // [sp+1Ch] [bp-4h]@18
	if( playerId )
		*(uint *)playerId = dword_15021180 - 1;
	if( !(*a8 != 0 ? (*Source != 0 ? (a8 != 0 ? (a7 != 0 ? (Source != 0 ? -(playerId != 0) : 0) : 0) : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v15 = 1204;
		LABEL_24:
		SErrSetLastError(v15);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v11 = (char*)a2;
	if( !a2 )
		v11 = &ZeroByte;
	v12 = (char*)a3;
	if( !a3 )
		v12 = &ZeroByte;
	if( !a9 )
		a9 = &ZeroByte;
	if( (uchar)dword_1501E2EC != -1 )
		SNetLeaveGame(2);
	sub_15011340();
	dword_15021340 = a7;
	strncpy(&Dest, Source, 0x80u);
	strncpy(&byte_150211B0, v12, 0x80u);
	strncpy(&byte_150212B8, v11, 0x80u);
	dword_15021230 = *v11 != 0;
	dword_150211A0 = GetTickCount();
	dword_1502119C = a4;
	if( dword_150211A4 )
		SMemFree((void*)dword_150211A4, "SNET.CPP", 2997, 0);
	dword_150211A4 = SMemAlloc(a6, "SNET.CPP", 2998, 0);
	dword_150211A8 = a6;
	memcpy((void*)dword_150211A4, a5, a6);
	v13 = dword_15021160;
	LOBYTE_IDA(dword_1501E2EC) = 0;
	if( dword_15021160
	|| (v16 = 0, v17 = 0, v18 = 0, v19 = 0, (v13 = sub_15011270((int)&dword_15021160, (int)&v16)) != 0) ){
		*(uint*)(v13 + 280) = 0;
		*(uchar*)(v13 + 438) = dword_1501E2EC;
		*(uint*)(v13 + 424) = 1;
		strncpy((char*)(v13 + 8), a8, 0x80u);
		strncpy((char*)(v13 + 136), a9, 0x80u);
	}
	if( playerId )
		*(uint *)playerId = dword_15021180 + (unsigned __int8)dword_1501E2EC;
	if( !(*(int (__stdcall **)(char *, char *, char *, int, uint, int, int))(dword_15021184 + 64))(
	&Dest,
	&byte_150212B8,
	&byte_150211B0,
	dword_15021230,
	0,
	dword_1502119C,
	dword_1502133C) ){
		v14 = SErrGetLastError();
		SNetLeaveGame(1);
		v15 = v14;
		goto LABEL_24;
	}
	dword_15021230 |= 4u;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
// 1502119C: using guessed type int dword_1502119C;
// 150211A0: using guessed type int dword_150211A0;
// 150211A4: using guessed type int dword_150211A4;
// 150211A8: using guessed type int dword_150211A8;
// 15021230: using guessed type int dword_15021230;
// 1502133C: using guessed type int dword_1502133C;
// 15021340: using guessed type int dword_15021340;
//----- (15011270) --------------------------------------------------------
int __fastcall sub_15011270(int a1, int a2)
{
	int v2;  // eax@1
	int v3;  // ebx@1
	int v4;  // ebp@1
	int v5;  // ecx@2
	int v6;  // edi@2
	int v7;  // ecx@4
	int v8;  // edi@4
	int v9;  // ecx@6
	int v10; // edi@6
	int v11; // esi@8
	int v13; // ecx@13
	v4 = a2;
	v3 = a1;
	v2 = SMemAlloc(440, ".?AUCONNREC@@", -2, 8);
	if( v2 ){
		v5 = v2 + 304;
		v6 = 3;
		do{
			*(uint*)v5 = 0;
			*(uint*)(v5 + 4) = 0;
			v5 += 8;
			--v6;
		}while( v6 );
		v7 = v2 + 328;
		v8 = 3;
		do{
			*(uint*)v7 = 0;
			*(uint*)(v7 + 4) = 0;
			v7 += 8;
			--v8;
		}while( v8 );
		v9 = v2 + 352;
		v10 = 3;
		do{
			*(uint*)v9 = 0;
			*(uint*)(v9 + 4) = 0;
			v9 += 8;
			--v10;
		}while( v10 );
		*(uint*)(v2 + 376) = 0;
		*(uint*)(v2 + 380) = 0;
		v11 = v2;
	}else{
		v11 = 0;
	}
	*(uint*)v11 = *(uint*)(v3 + 4);
	if( *(uint*)v3 ){
		*(uint*)(*(uint*)(v3 + 4) + 4) = v11;
		*(uint*)(v3 + 4) = v11;
	}else{
		*(uint*)(v3 + 4) = v11;
		*(uint*)v3 = v11;
	}
	*(uint*)(v11 + 264) = *(uint*)v4;
	*(uint*)(v11 + 268) = *(uint*)(v4 + 4);
	*(uint*)(v11 + 272) = *(uint*)(v4 + 8);
	v13 = *(uint*)(v4 + 12);
	*(uchar*)(v11 + 438) = -1;
	*(uint*)(v11 + 276) = v13;
	*(uint*)(v11 + 284) = GetTickCount();
	return v11;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15011340) --------------------------------------------------------
void __stdcall sub_15011340()
{
	int v0;  // edi@1
	int* v1; // esi@2
	v0 = 0;
	do{
		v1 = &dword_15021160;
		if( !v0 )
			v1 = &dword_15021158;
		while( *v1 )
			sub_15011370((void*)*v1);
		++v0;
	}while( v0 <= 1 );
}

// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
//----- (15011370) --------------------------------------------------------
int __thiscall sub_15011370(void* This)
{
	int v1;   // ebx@1
	void* v2; // edi@1
	char* v3; // esi@1
	int v4;   // eax@3
	int v5;   // eax@7
	int v6;   // eax@15
	v2 = This;
	v1 = 3;
	v3 = (char*)This + 328;
	do{
		sub_15011450(v3 - 24);
		sub_15011450(v3);
		sub_15011450(v3 + 24);
		v3 += 8;
		--v1;
	}while( v1 );
	sub_15011450((char*)v2 + 376);
	v4 = *(uint*)v2;
	if( v2 == (void*)dword_15021160 ){
		if( v4 )
			*(uint *)(v4 + 4) = *((uint *)v2 + 1);
		else
			dword_15021160 = *((uint*)v2 + 1);
		v5 = *((uint*)v2 + 1);
		if( v5 ){
			*(uint*)v5 = *(uint*)v2;
		}else{
			if( (void*)dword_15021164 == v2 )
				dword_15021164 = *(uint*)v2;
		}
	}else{
		if( v4 ){
			*(uint*)(v4 + 4) = *((uint*)v2 + 1);
		}else{
			if( (void*)dword_15021158 == v2 )
				dword_15021158 = *((uint*)v2 + 1);
		}
		v6 = *((uint*)v2 + 1);
		if( v6 ){
			*(uint*)v6 = *(uint*)v2;
		}else{
			if( (void*)dword_1502115C == v2 )
				dword_1502115C = *(uint*)v2;
		}
	}
	*(uint*)v2 = 0;
	*((uint*)v2 + 1) = 0;
	return SMemFree(v2, ".?AUCONNREC@@", -2, 0);
}

// 15021158: using guessed type int dword_15021158;
// 1502115C: using guessed type int dword_1502115C;
// 15021160: using guessed type int dword_15021160;
// 15021164: using guessed type int dword_15021164;
//----- (15011450) --------------------------------------------------------
void __thiscall sub_15011450(void* This)
{
	void* v1; // edi@1
	void* v2; // esi@2
	int v3;   // eax@11
	v1 = This;
	while( 1 ){
		v2 = *(void**)v1;
		if( !*(uint*)v1 )
			break;
		if( *((uint*)v2 + 5) ){
			sub_150114E0(*((void**)v2 + 2), *((void**)v2 + 3), *((uint*)v2 + 4));
		}else{
			if( dword_15021184 )
				(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(
				*((uint *)v2 + 2),
				*((uint *)v2 + 3),
				*((uint *)v2 + 4));
		}
		if( *(uint*)v2 ){
			*(uint*)(*(uint*)v2 + 4) = *((uint*)v2 + 1);
		}else{
			if( *(void**)v1 == v2 )
				*(uint *)v1 = *((uint *)v2 + 1);
		}
		v3 = *((uint*)v2 + 1);
		if( v3 ){
			*(uint*)v3 = *(uint*)v2;
		}else{
			if( *((void**)v1 + 1) == v2 )
				*((uint *)v1 + 1) = *(uint *)v2;
		}
		*(uint*)v2 = 0;
		*((uint*)v2 + 1) = 0;
		SMemFree(v2, ".?AUMESSAGE@@", -2, 0);
	}
}

// 15021184: using guessed type int dword_15021184;
//----- (150114E0) --------------------------------------------------------
int __fastcall sub_150114E0(void* a1, void* a2, int a3)
{
	void* v3;   // esi@1
	int result; // eax@2
	v3 = a2;
	if( a1 )
		result = SMemFree(a1, "SNET.CPP", 1025, 0);
	if( v3 )
		result = SMemFree(v3, "SNET.CPP", 1027, 0);
	return result;
}

//----- (15011520) --------------------------------------------------------
int __stdcall SNetDestroy()
{
	//if( ExternalStorm ) return eSNetDestroy();
	EnterCriticalSection(&NetCritical);
	if( (uchar)dword_1501E2EC != -1 )
		SNetLeaveGame(3);
	LeaveCriticalSection(&NetCritical);
	sub_150116F0();
	EnterCriticalSection(&NetCritical);
	sub_15011660();
	SEvtUnregisterType(1397638484, 1);
	SEvtUnregisterType(1397638484, 2);
	sub_15011340();
	sub_150115A0((void*)1);
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
//----- (150115A0) --------------------------------------------------------
int __thiscall sub_150115A0(void* This)
{
	HMODULE v1; // eax@1
	void* v2;   // esi@1
	int result; // eax@8
	int v4;     // esi@12
	v1 = hLibModule;
	v2 = This;
	if( hLibModule ){
		if( dword_15021184 ){
			(*(void(* *)())(dword_15021184 + 8))();
			v1 = hLibModule;
		}
		if( v1 ){
			FreeLibrary(v1);
			hLibModule = 0;
		}
	}
	if( dword_15021184 ){
		SMemFree((void*)dword_15021184, "SNET.CPP", 587, 0);
		dword_15021184 = 0;
	}
	result = (int)lpAddress;
	if( lpAddress ){
		VirtualUnlock(lpAddress, 0x2000u);
		result = VirtualFree(lpAddress, 0, 0x8000u);
		lpAddress = 0;
	}
	if( v2 ){
		result = dword_15021114;
		if( dword_15021114 ){
			do{
				v4 = *(uint*)dword_15021110;
				SMemFree((void*)result, ".?AUPROVIDERINFO@@", -2, 0);
				result = v4;
				dword_15021114 = v4;
			}while( v4 );
		}
		dword_15021110 = 0;
		dword_15021194 = 0;
	}
	return result;
}

// 15021110: using guessed type int dword_15021110;
// 15021114: using guessed type int dword_15021114;
// 15021184: using guessed type int dword_15021184;
// 15021194: using guessed type int dword_15021194;
//----- (15011660) --------------------------------------------------------
int __stdcall sub_15011660()
{
	int result; // eax@1
	void* i;    // esi@1
	void* v2;   // ecx@2
	int v3;     // eax@8
	result = dword_15021118;
	for( i = (void*)dword_15021118; dword_15021118; i = (void*)dword_15021118 ){
		v2 = *(void**)(result + 16);
		if( v2 ){
			SMemFree(v2, "SNET.CPP", 1699, 0);
			result = dword_15021118;
		}
		if( *(uint*)i ){
			*(uint*)(*(uint*)i + 4) = *((uint*)i + 1);
		}else{
			if( (void*)result == i )
				dword_15021118 = *((uint*)i + 1);
		}
		v3 = *((uint*)i + 1);
		if( v3 ){
			*(uint*)v3 = *(uint*)i;
		}else{
			if( (void*)dword_1502111C == i )
				dword_1502111C = *(uint*)i;
		}
		*(uint*)i = 0;
		*((uint*)i + 1) = 0;
		SMemFree(i, ".?AUUSEREVENT@@", -2, 0);
		result = dword_15021118;
	}
	return result;
}

// 15021118: using guessed type int dword_15021118;
// 1502111C: using guessed type int dword_1502111C;
//----- (150116F0) --------------------------------------------------------
int __stdcall sub_150116F0()
{
	int result; // eax@1
	result = (int)dword_15021344;
	if( dword_15021344 ){
		if( dword_1502134C ){
			dword_15021348 = 1;
			SetEvent(dword_15021344);
			WaitForSingleObject(dword_1502134C, 0xFFFFFFFFu);
			dword_15021348 = 0;
			CloseHandle(dword_15021344);
			result = CloseHandle(dword_1502134C);
			dword_15021344 = 0;
			dword_1502134C = 0;
		}
	}
	return result;
}

// 15021348: using guessed type int dword_15021348;
//----- (15011760) --------------------------------------------------------
int __stdcall SNetDropPlayer(int a1, uint a2)
{
	//if( ExternalStorm ) return eSNetDropPlayer(a1, a2);
	int v2;     // ecx@7
	int* v3;    // eax@8
	int v4;     // eax@10
	int v5;     // edi@14
	int i;      // esi@17
	__int16 v7; // dx@17
	int v8;     // eax@20
	DWORD v10;  // [sp-4h] [bp-20h]@2
	int v11;    // [sp+10h] [bp-Ch]@20
	int v12;    // [sp+14h] [bp-8h]@20
	int v13;    // [sp+18h] [bp-4h]@20
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v10 = 1204;
		LABEL_24:
		SErrSetLastError(v10);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( a1 == 255 || a1 == (unsigned __int8)dword_1501E2EC ){
		v10 = 87;
		goto LABEL_24;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v10 = -2062548880;
		goto LABEL_24;
	}
	v2 = 1;
	while( 1 ){
		v3 = &dword_15021160;
		if( !v2 )
			v3 = &dword_15021158;
		v4 = *v3;
		if( v4 )
			break;
		LABEL_13:
		--v2;
		if( v2 < 0 ){
			v5 = 0;
			goto LABEL_15;
		}
	}
	while( *(uchar*)(v4 + 438) != (uchar)a1 ){
		v4 = *(uint*)(v4 + 4);
		if( !v4 )
			goto LABEL_13;
	}
	v5 = v4;
	LABEL_15:
	if( !v5 ){
		v10 = -2062548886;
		goto LABEL_24;
	}
	v7 = *(ushort*)(v5 + 394);
	*(uint*)(v5 + 280) |= 8u;
	*(ushort*)(v5 + 436) = v7;
	*(uint*)(v5 + 432) = a2;
	for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
		if( i != v5 ){
			if( *(uchar*)(i + 438) != -1 ){
				v8 = *(ushort*)(v5 + 394);
				v11 = a1;
				v12 = v8;
				v13 = a2;
				sub_150118B0(i, 0, 12, &v11, 0xCu);
			}
		}
	}
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (150118B0) --------------------------------------------------------
int __fastcall sub_150118B0(int a1, char a2, char a3, void* a4, unsigned int a5)
{
	int v5;           // eax@1
	int v6;           // ebx@1
	int v8;           // ebp@5
	int v9;           // edi@5
	unsigned int v10; // esi@5
	int v11;          // eax@5
	char v12;         // dl@5
	int v13;          // eax@13
	int v14;          // ecx@13
	int v15;          // ebp@18
	int v16;          // [sp+10h] [bp-28h]@3
	int v17;          // [sp+14h] [bp-24h]@5
	int v18;          // [sp+18h] [bp-20h]@5
	int v19;          // [sp+1Ch] [bp-1Ch]@1
	unsigned int v20; // [sp+20h] [bp-18h]@5
	int v21;          // [sp+24h] [bp-14h]@5
	int v22;          // [sp+28h] [bp-10h]@2
	int v23;          // [sp+2Ch] [bp-Ch]@2
	int v24;          // [sp+30h] [bp-8h]@2
	int v25;          // [sp+34h] [bp-4h]@2
	v5 = dword_15021160;
	LOBYTE_IDA(v19) = a2;
	v6 = a1;
	if( !dword_15021160 ){
		v22 = 0;
		v23 = 0;
		v24 = 0;
		v25 = 0;
		v5 = sub_15011270((int)&dword_15021160, (int)&v22);
	}
	v16 = v5;
	if( !v5 )
		return 0;
	v18 = 0;
	v17 = 0;
	v10 = a5 + 12;
	v20 = a5 + 12;
	sub_15011AB0((int)&v18, (int)&v17, a5 + 12);
	v11 = v18;
	v8 = (unsigned __int8)v19;
	*(uint*)v18 = *(uint*)(v6 + 264);
	*(uint*)(v11 + 4) = *(uint*)(v6 + 268);
	*(uint*)(v11 + 8) = *(uint*)(v6 + 272);
	*(uint*)(v11 + 12) = *(uint*)(v6 + 276);
	v9 = v17;
	v21 = v17;
	*(ushort*)(v17 + 2) = a5 + 12;
	*(ushort*)(v9 + 6) = *(ushort*)(v6 + 2 * v8 + 398);
	LOBYTE_IDA(v11) = v19;
	*(uchar*)(v9 + 8) = v19;
	*(uchar*)(v9 + 9) = a3;
	v12 = *(uchar*)(v16 + 438);
	*(uchar*)(v9 + 11) = 0;
	*(uchar*)(v9 + 10) = v12;
	if( (uchar)v11 || a3 != 1 ){
		*(ushort*)(v9 + 4) = (*(ushort*)(v6 + 2 * v8 + 384))++;
	}else
		*(ushort*)(v9 + 4) = 0;
	if( a4 ){
		if( a5 ){
			memcpy((void*)(v17 + 12), a4, a5);
			v10 = v20;
			v9 = v21;
		}
	}
	*(ushort*)v9 = sub_15011B00(v9);
	if( v6 != v16 ){
		v13 = SMemAlloc(32, ".?AUMESSAGE@@", -2, 8);
		v14 = v6 + 8 * v8 + 308;
		*(uint*)v13 = *(uint*)(v6 + 8 * v8 + 308);
		if( !*(uint*)(v6 + 8*v8 + 304) ){
			*(uint*)v14 = v13;
			*(uint*)(v6 + 8 * v8 + 304) = v13;
			goto LABEL_18;
		}
		LABEL_17:
		*(uint*)(*(uint*)v14 + 4) = v13;
		*(uint*)v14 = v13;
		goto LABEL_18;
	}
	v13 = SMemAlloc(32, ".?AUMESSAGE@@", -2, 8);
	v14 = v6 + 8 * v8 + 332;
	*(uint*)v13 = *(uint*)(v6 + 8 * v8 + 332);
	if( *(uint*)(v6 + 8 * v8 + 328) )
		goto LABEL_17;
	*(uint*)v14 = v13;
	*(uint*)(v6 + 8 * v8 + 328) = v13;
	LABEL_18:
	v15 = v13;
	*(uint*)(v13 + 8) = v18;
	*(uint*)(v13 + 12) = v17;
	*(uint*)(v13 + 16) = v10;
	*(uint*)(v13 + 20) = 1;
	*(uint*)(v13 + 24) = GetTickCount();
	if( v6 != v16 ){
		sub_15011BB0(v6, v9);
		dword_1501E258 += a5;
	}
	*(uint*)(v15 + 28) = dword_1502118C;
	return v15;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E258: using guessed type int dword_1501E258;
// 15021160: using guessed type int dword_15021160;
// 1502118C: using guessed type int dword_1502118C;
//----- (15011AB0) --------------------------------------------------------
int __fastcall sub_15011AB0(int a1, int a2, int a3)
{
	int v3;     // eax@1
	int v4;     // edi@1
	int result; // eax@3
	v4 = a2;
	*(uint*)a1 = SMemAlloc(16, "SNET.CPP", 999, 0);
	v3 = 4 - (a3 & 3) + a3;
	if( (unsigned int)v3 < 4 )
		v3 = 4;
	result = SMemAlloc(v3, "SNET.CPP", 1001, 0);
	*(uint*)v4 = result;
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15011B00) --------------------------------------------------------
int __thiscall sub_15011B00(int This)
{
	unsigned int v2;    // eax@1
	unsigned __int8 v3; // ST04_1@1
	LOBYTE_IDA(v2) = sub_15011B60(This + 2, *(ushort*)(This + 2) - 2);
	v2 = (unsigned __int16)v2;
	v3 = -1 - (int)((v2 >> 8) + (unsigned __int8)v2) % 255;
	return (v3 << 8) | (unsigned __int8)(-1 - (int)(v3 + (v2 >> 8)) % 255);
}

//----- (15011B60) --------------------------------------------------------
char __fastcall sub_15011B60(int a1, int a2)
{
	int v2;          // edx@1
	int v3;          // ecx@1
	unsigned int v4; // ebx@1
	unsigned int v5; // esi@1
	int v6;          // eax@1
	int v7;          // edi@2
	v4 = 0;
	v5 = 0;
	v3 = a2 + a1 - 1;
	v6 = a2;
	v2 = a2 - 1;
	if( v6 ){
		v7 = v2 + 1;
		do{
			v4 += *(uchar*)v3--;
			if( v4 >= 0xFF )
				v4 -= 255;
			v5 += v4;
			--v7;
		}while( v7 );
	}
	return v5 % 0xFF;
}

//----- (15011BB0) --------------------------------------------------------
int __fastcall sub_15011BB0(int a1, int a2)
{
	int result;      // eax@1
	int v3;          // edi@1
	int v4;          // esi@1
	unsigned int v5; // ST04_4@1
	int v6;          // [sp+0h] [bp-4h]@1
	v6 = a1;
	v3 = a1;
	v4 = a2;
	v5 = *(ushort*)(a2 + 2);
	v6 = a1 + 264;
	result = sub_15011C00(1u, (int)&v6, (void*)a2, v5);
	LOWORD_IDA(result) = *(ushort*)(v4 + 6);
	*(ushort*)(v3 + 2 * *(uchar*)(v4 + 8) + 404) = result;
	*(uint*)(v3 + 4 * *(uchar*)(v4 + 8) + 412) = 0;
	return result;
}

//----- (15011C00) --------------------------------------------------------
int __fastcall sub_15011C00(unsigned int a1, int a2, void* a3, unsigned int a4)
{
	unsigned int v4;  // ebx@1
	int v5;           // esi@1
	DWORD v7;         // eax@3
	LPVOID v8;        // eax@5
	unsigned int v9;  // ebp@5
	unsigned int v10; // ecx@8
	int v11;          // edx@9
	int v12;          // edi@10
	int v13;          // edx@12
	int v14;          // ecx@12
	unsigned int v15; // esi@12
	int v16;          // edi@13
	int v17;          // eax@13
	void* v18;        // eax@15
	int v19;          // [sp+10h] [bp-4h]@1
	v5 = a2;
	v19 = a2;
	v4 = a1;
	if( !dword_15021184 )
		return 0;
	v7 = GetTickCount();
	if( v7 - dword_1502118C < 0x7FFFFFFF )
		dword_1502118C = v7;
	v9 = a4;
	v8 = lpAddress;
	dword_1502118C += 1000 * v4 * (a4 + 64) / *(uint*)(dword_15021190 + 552);
	if( lpAddress ){
		if( v4 > 0x10 || a4 > 0x1F00 )
			return 0;
		v10 = 0;
		if( v4 ){
			v11 = 64;
			do{
				v12 = (int)((char*)v8 + v11);
				v11 += 16;
				*((uint*)v8 + v10) = v12;
				v8 = lpAddress;
				++v10;
			}while( v10 < v4 );
			if( v4 ){
				v14 = v5;
				v13 = 64;
				v15 = v4;
				do{
					v16 = *(uint*)v14;
					v17 = (int)((char*)v8 + v13);
					v14 += 4;
					v13 += 16;
					--v15;
					*(uint*)v17 = *(uint*)v16;
					*(uint*)(v17 + 4) = *(uint*)(v16 + 4);
					*(uint*)(v17 + 8) = *(uint*)(v16 + 8);
					*(uint*)(v17 + 12) = *(uint*)(v16 + 12);
					v8 = lpAddress;
				}while( v15 );
				v9 = a4;
			}
		}
		memcpy((char*)v8 + 320, a3, v9);
		v5 = v19;
		v18 = (char*)lpAddress + 320;
	}else{
		v18 = a3;
	}
	dword_1501E278 += v9;
	return (*(int (__stdcall **)(unsigned int, int,  void *, unsigned int))(dword_15021184 + 56))(v4, v5, v18, v9);
}

// 1501E278: using guessed type int dword_1501E278;
// 15021184: using guessed type int dword_15021184;
// 1502118C: using guessed type int dword_1502118C;
// 15021190: using guessed type int dword_15021190;
//----- (15011D50) --------------------------------------------------------
int __stdcall SNetEnumProviders(int (__stdcall *checkFunct)(uint, uint, uint))
{
	int (__stdcall *v1)(uint, uint, uint); // edi@1
	int v3;                                      // ebx@5
	void* v4;                                    // ebp@5
	int i;                                       // eax@7
	int v6;                                      // edx@7
	int v7;                                      // eax@10
	int v8;                                      // ebx@15
	void* v9;                                    // esi@15
	int v10;                                     // edx@19
	int v11;                                     // [sp+10h] [bp-4h]@5
	v1 = checkFunct;
	if( !checkFunct ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		SErrSetLastError(0x4B4u);
		LABEL_6:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v4 = 0;
	v3 = 0;
	if( !(*(int(__stdcall* *)(int*))(dword_15021184 + 36))(&v11) )
		goto LABEL_6;
	v6 = v11;
	for( i = v11; i; ++v3 )
		i = *(uint*)(i + 300);
	if( v3 ){
		v7 = SMemAlloc(304 * v3, "SNET.CPP", 3177, 0);
		v4 = (void*)v7;
		v6 = v11;
		if( v11 ){
			do{
				memcpy((void*)v7, (void*)v6, 0x130u);
				*(uint*)(v7 + 300) = v7 + 304;
				v6 = *(uint*)(v6 + 300);
				v7 += 304;
			}while( v6 );
			v6 = v11;
			v1 = checkFunct;
		}
	}
	(*(void(__stdcall* *)(int))(dword_15021184 + 72))(v6);
	LeaveCriticalSection(&NetCritical);
	if( v3 ){
		if( v4 ){
			v9 = v4;
			v8 = v3 - 1;
			do{
				if( v1(*(uint*)v9, (uint)((char*)v9 + 40), (uint)((char*)v9 + 168)) )
					v9 = (void*)*((uint*)v9 + 75);
				else
					v8 = 0;
				v10 = v8--;
			}while( v10 );
			SMemFree(v4, "SNET.CPP", 3219, 0);
		}
	}
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021184: using guessed type int dword_15021184;
//----- (15011E90) --------------------------------------------------------
int __stdcall SNetEnumDevices(int a1, int a2, int (__stdcall *a3)(unsigned int, unsigned int, unsigned int), int a4)
{
	int (__stdcall *v4)(uint, uint, uint); // edi@1
	int v6;                                      // esi@3
	int v7;                                      // ebx@7
	void* v8;                                    // ebp@7
	int i;                                       // eax@9
	int v10;                                     // edx@9
	int v11;                                     // eax@12
	int v12;                                     // ebx@17
	void* v13;                                   // esi@17
	int v14;                                     // ecx@21
	v4 = a3;
	if( !a3 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	v6 = a4;
	if( a4 )
		*(uint *)a4 = 0;
	if( !dword_15021184 ){
		SErrSetLastError(0x4B4u);
		LABEL_8:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v8 = 0;
	v7 = 0;
	if( !(*(int (__stdcall **)(int, int, int *))(dword_15021184 + 40))(a1, a2, &a2) )
		goto LABEL_8;
	v10 = a2;
	for( i = a2; i; ++v7 )
		i = *(uint*)(i + 296);
	if( v7 ){
		v11 = SMemAlloc(300 * v7, "SNET.CPP", 3270, 0);
		v8 = (void*)v11;
		v10 = a2;
		if( a2 ){
			do{
				memcpy((void*)v11, (void*)v10, 300);
				*(uint*)(v11 + 296) = v11 + 300;
				v10 = *(uint*)(v10 + 296);
				v11 += 300;
			}while( v10 );
			v10 = a2;
			v6 = a4;
			v4 = a3;
		}
	}
	(*(void (__stdcall **)(int, int))(dword_15021184 + 76))(v10, v6);
	LeaveCriticalSection(&NetCritical);
	if( v7 ){
		if( v8 ){
			v13 = v8;
			v12 = v7 - 1;
			do{
				if( v4(*(uint*)v13, (uint)(char*)v13 + 40, (uint)(char*)v13 + 168) )
					v13 = (void*)*((uint*)v13 + 74);
				else
					v12 = 0;
				v14 = v12--;
			}while( v14 );
			SMemFree(v8, "SNET.CPP", 3313, 0);
		}
	}
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021184: using guessed type int dword_15021184;
//----- (15011FE0) --------------------------------------------------------
int __stdcall SNetEnumGames(int a1, int (__stdcall *a2)(uint, uint, uint, uint))
{
	int v2;     // ecx@3
	int result; // eax@6
	int v4;     // esi@7
	if( a1 && *(uint*)a1 != 36 )
		v2 = 0;
	else
		v2 = -1;
	if( a2 != 0 ? v2 : 0 ){
		EnterCriticalSection(&NetCritical);
		sub_150120B0();
		v4 = dword_15021110;
		while( v4 ){
			if( *(uint*)(v4 + 272) ){
				if( v4 != -276 ){
					if( *(uchar*)(v4 + 276) ){
						if( sub_150125F0(v4 + 532, a1) ){
							if( !a2(*(uint*)(v4 + 272), v4 + 276, v4 + 404, v4 + 532) )
								v4 = 0;
						}
					}
				}
			}
			if( dword_15021194 && dword_15021110 && v4 )
				v4 = *(uint*)(v4 + 4);
			else
				v4 = 0;
		}
		LeaveCriticalSection(&NetCritical);
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 15021110: using guessed type int dword_15021110;
// 15021194: using guessed type int dword_15021194;
//----- (150120B0) --------------------------------------------------------
int __stdcall sub_150120B0()
{
	int result;                            // eax@1
	int v1;                                // ebx@1
	CHAR* i;                               // esi@2
	int v3;                                // eax@2
	void* v4;                              // edi@4
	int v5;                                // ebp@5
	HMODULE v6;                            // eax@6
	HMODULE v7;                            // esi@6
	FARPROC v8;                            // eax@8
	void* v9;                              // ebx@15
	int v10;                               // ecx@21
	int v11;                               // ebp@21
	CHAR* v12;                             // edi@21
	char v13;                              // zf@23
	char* v14;                             // eax@25
	unsigned int v15;                      // ecx@25
	char v16;                              // al@25
	void* v17;                             // esi@25
	int v18;                               // edx@25
	void* v19;                             // edi@25
	void* v20;                             // esi@25
	int v21;                               // ecx@25
	int v22;                               // ecx@29
	int v23;                               // edi@29
	int v24;                               // eax@30
	int v25;                               // edx@30
	int v26;                               // ecx@30
	int v27;                               // esi@31
	int v28;                               // eax@38
	int v29;                               // eax@43
	int v30;                               // eax@49
	int v31;                               // ecx@55
	int v32;                               // [sp+18h] [bp-480h]@10
	int v33;                               // [sp+1Ch] [bp-47Ch]@12
	int v34;                               // [sp+20h] [bp-478h]@10
	char v35;                              // [sp+24h] [bp-474h]@10
	HMODULE hLibModule;                    // [sp+28h] [bp-470h]@6
	char* Source;                          // [sp+2Ch] [bp-46Ch]@17
	void* v38;                             // [sp+30h] [bp-468h]@5
	char v39;                              // [sp+34h] [bp-464h]@10
	int v40;                               // [sp+38h] [bp-460h]@4
	int v41;                               // [sp+3Ch] [bp-45Ch]@17
	char v42;                              // [sp+40h] [bp-458h]@10
	char* v43;                             // [sp+44h] [bp-454h]@17
	FARPROC v44;                           // [sp+48h] [bp-450h]@8
	CHAR Str;                              // [sp+4Ch] [bp-44Ch]@2
	char v46;                              // [sp+4Dh] [bp-44Bh]@2
	__int16 v47;                           // [sp+14Dh] [bp-34Bh]@2
	char v48;                              // [sp+14Fh] [bp-349h]@2
	CHAR LibFileName;                      // [sp+150h] [bp-348h]@5
	CHAR FileName;                         // [sp+254h] [bp-244h]@4
	char v51;                              // [sp+255h] [bp-243h]@4
	__int16 v52;                           // [sp+355h] [bp-143h]@4
	char v53;                              // [sp+357h] [bp-141h]@4
	struct _WIN32_FIND_DATAA FindFileData; // [sp+358h] [bp-140h]@4
	result = dword_15021194;
	v1 = 0;
	if( !dword_15021194 ){
		Str = ZeroByte;
		memset(&v46, 0, 0x100u);
		v47 = 0;
		dword_15021194 = 1;
		v48 = 0;
		v3 = GetStormDllHandle();
		GetModuleFileNameA((HMODULE)v3, &Str, 0x104u);
		for( i = &Str; strchr(i, 92); i = strchr(i, 92) + 1 )
		;
		*i = 0;
		FileName = ZeroByte;
		memset(&v51, 0, 0x100u);
		v52 = 0;
		v53 = 0;
		wsprintfA(&FileName, "%s*.sn2", &Str);
		result = (int)FindFirstFileA(&FileName, &FindFileData);
		v4 = (void*)result;
		v40 = result;
		if( result != -1 ){
			while( 1 ){
				wsprintfA(&LibFileName, "%s%s", &Str, FindFileData.cFileName);
				v5 = (int)sub_15012530(&LibFileName);
				v38 = (void*)v5;
				if( v5 != v1 ){
					v6 = LoadLibraryA(&LibFileName);
					v7 = v6;
					hLibModule = v6;
					if( v6 == (HMODULE)v1 ){
						SMemFree((void*)v5, "SNET.CPP", 650, v1);
					}else{
						v8 = GetProcAddress(v6, "SnpQuery");
						v44 = v8;
						if( v8 != (FARPROC)v1 ){
							v34 = v1;
							v32 = v1;
							if( !((int (__stdcall *)(int, char *, char *, char *, int *))v8)(v1, &v35, &v42, &v39, &v32) ){
								LABEL_62:
								FreeLibrary(hLibModule);
								v1 = 0;
								SMemFree(v38, "SNET.CPP", 725, 0);
								v4 = (void*)v40;
								goto LABEL_63;
							}
							while( 2 ){
								if( !v32
								|| *(uint *)v32 < 0x24u
								|| ((v9 = 0, v5) ? (v41 = *(uint *)(v5 + 4),
								Source = (char *)(v5 + 8),
								v43 = ( char *)(v5 + 8 + strlen(( char *)(v5 + 8)) - 1 + 1),
								v9 = &v43[strlen(v43)],
								v33 = (char*)v9 + *(uint *)v9 - v5 - 4 == *( void **)v5 ? (int)((char *)v9 + *(uint *)v9) : 0) : (SErrSetLastError(0x57u), v33 = 0),
								!v33 || !v9 || *(uint *)v9 < 0x24u) )
									goto LABEL_62;
								v11 = SMemAlloc(568, ".?AUPROVIDERINFO@@", -2, 8);
								v12 = &LibFileName;
								v10 = -1;
								do{
									if( !v10 )
										break;
									v13 = *v12++ == 0;
									--v10;
								}while( !v13 );
								v15 = ~v10;
								v16 = v15;
								v17 = &v12[-v15];
								v18 = v41;
								v15 >>= 2;
								memcpy((void*)(v11 + 8), v17, 4 * v15);
								v20 = (char*)v17 + 4 * v15;
								v19 = (void*)(v11 + 8 + 4 * v15);
								LOBYTE_IDA(v15) = v16;
								v14 = Source;
								memcpy(v19, v20, v15 & 3);
								v21 = v34;
								*(uint*)(v11 + 272) = v18;
								*(uint*)(v11 + 268) = v21;
								if( v14 )
									strncpy((char*)(v11 + 276), v14, 0x80u);
								if( v43 )
									strncpy((char*)(v11 + 404), v43, 0x80u);
								memcpy((void*)(v11 + 532), v9, 0x24u);
								*(uint*)(v11 + 540) -= 16;
								v22 = dword_15021110;
								v23 = dword_15021110;
								if( dword_15021110 ){
									do{
										v25 = 2147483647;
										v26 = 0;
										v32 = 2147483647;
										v24 = (int)&unk_1501C7E8;
										do{
											v27 = *(uint*)v24;
											if( *(uint*)(v23 + 272) == *(uint*)v24 )
												v32 = v26;
											if( *(uint*)(v11 + 272) == v27 )
												v25 = v26;
											v24 += 4;
											++v26;
										}while( v24 < (int)"SNET.CPP" );
										if( v32 != 2147483647 || v25 != 2147483647 )
											v28 = v25 - v32;
										else
											v28 = stricmp((char*)(v23 + 276), (char*)(v11 + 276));
										if( v28 > 0 )
											break;
										v23 = *(uint*)(v23 + 4);
									}while( v23 );
									v22 = dword_15021110;
								}
								v29 = *(uint*)v11;
								if( *(uint*)v11 ){
									LABEL_46:
									*(uint*)(v29 + 4) = *(uint*)(v11 + 4);
									goto LABEL_49;
								}
								if( *(uint*)(v11 + 4) ){
									if( v29 )
										goto LABEL_46;
									if( v22 == v11 )
										dword_15021110 = *(uint*)(v11 + 4);
									LABEL_49:
									v30 = *(uint*)(v11 + 4);
									if( v30 ){
										*(uint*)v30 = *(uint*)v11;
									}else{
										if( dword_15021114 == v11 )
											dword_15021114 = *(uint*)v11;
									}
									*(uint*)v11 = 0;
									*(uint*)(v11 + 4) = 0;
								}
								if( v23 ){
									v31 = *(uint*)v23;
									*(uint*)(v11 + 4) = v23;
									*(uint*)v11 = v31;
									*(uint*)v23 = v11;
									if( *(uint*)v11 ){
										*(uint*)(*(uint*)v11 + 4) = v11;
										LABEL_61:
										v32 = 0;
										++v34;
										if( !((int (__stdcall *)(int, char *, char *, char *, int *))v44)(v34, &v35, &v42, &v39, &v32) )
											goto LABEL_62;
										v5 = v33;
										continue;
									}
								}else{
									*(uint*)v11 = dword_15021114;
									if( dword_15021110 ){
										*(uint*)(dword_15021114 + 4) = v11;
										dword_15021114 = v11;
										goto LABEL_61;
									}
									dword_15021114 = v11;
								}
								break;
							}
							dword_15021110 = v11;
							goto LABEL_61;
						}
						SMemFree((void*)v5, "SNET.CPP", 658, v1);
						FreeLibrary(v7);
					}
				}
				LABEL_63:
				if( !FindNextFileA(v4, &FindFileData) )
					return FindClose(v4);
			}
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021110: using guessed type int dword_15021110;
// 15021114: using guessed type int dword_15021114;
// 15021194: using guessed type int dword_15021194;
//----- (15012530) --------------------------------------------------------
void* __thiscall sub_15012530(char* Str)
{
	void* v1;        // edi@1
	unsigned int v2; // esi@5
	int v4;          // [sp+8h] [bp-Ch]@1
	int v5;          // [sp+Ch] [bp-8h]@1
	int v6;          // [sp+10h] [bp-4h]@2
	v1 = 0;
	v5 = 0;
	v4 = 0;
	if( SFileOpenArchive(Str, 0, 0, (MPQ**)&v5) ){
		SFileAuthenticateArchive(v5, (int)&v6);
		if( !v6 || (unsigned int)v6 >= 5 ){
			if( SFileOpenFileEx((HANDLE)v5, "caps.dat", 0, (HANDLE*)&v4) ){
				v2 = SFileGetFileSize((HANDLE)v4, 0);
				v1 = (void*)SMemAlloc(v2, "SNET.CPP", 848, 0);
				SFileReadFile((HANDLE)v4, v1, v2, (uint*)&v6, 0);
			}
		}
	}
	if( v4 )
		SFileCloseFile((HANDLE)v4);
	if( v5 )
		SFileCloseArchive((void*)v5);
	return v1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150125F0) --------------------------------------------------------
int __fastcall sub_150125F0(int a1, int a2)
{
	unsigned int v2; // edx@6
	return !a1
		|| !a2
		|| (*(uint *)(a2 + 4) & *(uint *)(a1 + 4)) == *(uint *)(a2 + 4)
		&& *(_QWORD *)(a2 + 8) <= *(_QWORD *)(a1 + 8)
		&& *(_QWORD *)(a2 + 16) <= *(_QWORD *)(a1 + 16)
		&& ((v2 = *(uint *)(a2 + 24), !v2) || v2 >= *(uint *)(a1 + 24));
}

//----- (15012650) --------------------------------------------------------
int __stdcall SNetGetGameInfo(int a1, void* a2, uint a3, int* bytesWritten)
{
	//if( ExternalStorm ) return eSNetGetGameInfo(a1, a2, a3, bytesWritten);
	unsigned int i;   // ecx@3
	char* v5;         // edi@3
	unsigned int v6;  // eax@12
	int v7;           // edx@12
	void* v8;         // esi@12
	char* v9;         // edi@13
	unsigned int v11; // ecx@25
	DWORD v12;        // [sp-4h] [bp-14h]@9
	EnterCriticalSection(&NetCritical);
	if( a2 ){
		if( a3 ){
			memset(a2, 0, 4 * (a3 >> 2));
			v5 = (char*)a2 + 4 * (a3 >> 2);
			for( i = a3 & 3; i; --i )
				*v5++ = 0;
		}
	}
	if( bytesWritten )
		*(uint *)bytesWritten = 0;
	if( !dword_15021184 ){
		v12 = 1204;
		LABEL_23:
		SErrSetLastError(v12);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v12 = -2062548880;
		goto LABEL_23;
	}
	v8 = 0;
	v6 = 0;
	v7 = 0;
	switch( a1 ){
	case 1:
		v8 = &Dest;
		v9 = &Dest;
		goto LABEL_16;
	case 2:
		v8 = &byte_150212B8;
		v9 = &byte_150212B8;
		goto LABEL_16;
	case 3:
		v8 = &byte_150211B0;
		v9 = &byte_150211B0;
		LABEL_16:
		v6 = strlen(v9);
		v7 = 1;
		break;
	case 4:
		v8 = &dword_15021230;
		goto LABEL_20;
	case 5:
		v8 = (void*)dword_150211A4;
		v6 = dword_150211A8;
		break;
	case 6:
		v8 = &dword_15021340;
		LABEL_20:
		v6 = 4;
		break;
	default:
		break;
	}
	if( !v8 ){
		v12 = 87;
		goto LABEL_23;
	}
	if( a2 && (v11 = a3, a3) ){
		if( a3 >= v6 )
			v11 = v6;
		memcpy(a2, v8, v11);
		if( bytesWritten ){
			if( a3 < v6 )
				v6 = a3;
			*(uint*)bytesWritten = v6;
		}
		if( v7 )
			*((uchar *)a2 + a3 - 1) = 0;
	}else{
		if( bytesWritten )
			*(uint *)bytesWritten = v6;
	}
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021184: using guessed type int dword_15021184;
// 150211A4: using guessed type int dword_150211A4;
// 150211A8: using guessed type int dword_150211A8;
// 15021230: using guessed type int dword_15021230;
// 15021340: using guessed type int dword_15021340;
//----- (150127B0) --------------------------------------------------------
int __stdcall SNetGetNetworkLatency(unsigned int a1, int a2)
{
	int i;           // ecx@11
	unsigned int v4; // eax@13
	DWORD v5;        // [sp-4h] [bp-10h]@10
	if( !((a2 != 0 ? (a1 < 1) - 1 : 0) & ((a1 > 4) - 1)) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( a2 )
		*(uint *)a2 = 0;
	if( !dword_15021184 || !dword_15021190 ){
		v5 = 1204;
		goto LABEL_24;
	}
	if( a1 == 1 ){
		*(uint*)a2 = *(uint*)(dword_15021190 + 556);
		LABEL_22:
		LeaveCriticalSection(&NetCritical);
		return 1;
	}
	if( a1 == 2 || a1 == 4 ){
		for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
			if( *(uchar*)(i + 438) != -1 ){
				v4 = 0;
				if( a1 == 2 )
					v4 = *(uint*)(i + 300);
				if( v4 < *(uint*)(i + 296) )
					v4 = *(uint*)(i + 296);
				if( v4 > *(uint*)a2 )
					*(uint *)a2 = v4;
			}
		}
		goto LABEL_22;
	}
	v5 = 87;
	LABEL_24:
	SErrSetLastError(v5);
	SErrGetLastError();
	LeaveCriticalSection(&NetCritical);
	return 0;
}

// 15021158: using guessed type int dword_15021158;
// 15021184: using guessed type int dword_15021184;
// 15021190: using guessed type int dword_15021190;
//----- (150128A0) --------------------------------------------------------
int __stdcall SNetGetNumPlayers(int* a1, int* a2, int a3)
{
	unsigned __int8 v3; // al@9
	int i;              // edx@18
	int v6;             // eax@21
	int v7;             // eax@25
	DWORD v8;           // [sp-4h] [bp-10h]@8
	EnterCriticalSection(&NetCritical);
	if( a1 )
		*a1 = dword_15021180 - 1;
	if( a2 )
		*a2 = dword_15021180 - 1;
	if( a3 )
		*(uint *)a3 = 0;
	if( !dword_15021184 ){
		v8 = 1204;
		LABEL_11:
		SErrSetLastError(v8);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v3 = dword_1501E2EC;
	if( (uchar)dword_1501E2EC == -1 ){
		v8 = -2062548880;
		goto LABEL_11;
	}
	if( a1 ){
		*a1 = dword_15021180 + (unsigned __int8)dword_1501E2EC;
		v3 = dword_1501E2EC;
	}
	if( a2 )
		*a2 = dword_15021180 + v3;
	if( a3 )
		*(uint *)a3 = 1;
	for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
		if( *(uchar*)(i + 438) != -1 ){
			if( a1 ){
				v6 = dword_15021180 + *(uchar*)(i + 438);
				if( v6 >= (unsigned int) * a1 )
					v6 = *a1;
				*a1 = v6;
			}
			if( a2 ){
				v7 = dword_15021180 + *(uchar*)(i + 438);
				if( v7 <= (unsigned int) * a2 )
					v7 = *a2;
				*a2 = v7;
			}
			if( a3 )
				++*(uint*)a3;
		}
	}
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
//----- (150129C0) --------------------------------------------------------
int __stdcall SNetGetOwnerTurnsWaiting(uint* turns)
{
	//if( ExternalStorm ) return eSNetGetOwnerTurnsWaiting(turns);
	int v2;   // eax@6
	int v3;   // esi@8
	int i;    // esi@14
	DWORD v5; // [sp-4h] [bp-1Ch]@4
	int v6;   // [sp+8h] [bp-10h]@7
	int v7;   // [sp+Ch] [bp-Ch]@7
	int v8;   // [sp+10h] [bp-8h]@7
	int v9;   // [sp+14h] [bp-4h]@7
	if( !turns ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	*(uint*)turns = 0;
	if( !dword_15021184 ){
		v5 = 1204;
		LABEL_18:
		SErrSetLastError(v5);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1
	|| (sub_15012E30(), v2 = dword_15021160, !dword_15021160)
	&& (v6 = dword_15021160,
	v7 = dword_15021160,
	v8 = dword_15021160,
	v9 = dword_15021160,
	v2 = sub_15011270((int)&dword_15021160, (int)&v6),
	!v2) )
		goto LABEL_17;
	v3 = v2;
	if( *(uint*)(v2 + 424) )
		goto LABEL_12;
	v3 = dword_15021158;
	if( !dword_15021158 ){
		LABEL_17:
		v5 = -2062548880;
		goto LABEL_18;
	}
	do{
		if( *(uint*)(v3 + 424) )
			break;
		v3 = *(uint*)(v3 + 4);
	}while( v3 );
	LABEL_12:
	if( !v3 || !*(uint*)(v3 + 424) )
		goto LABEL_17;
	sub_15012AD0((void*)v3);
	for( i = *(uint*)(v3 + 344); i; i = *(uint*)(i + 4) )
		++*(uint*)turns;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (15012AD0) --------------------------------------------------------
int __thiscall sub_15012AD0(void* This)
{
	int result;  // eax@1
	void* v2;    // esi@1
	int i;       // eax@4
	int v4;      // edx@4
	__int16 v5;  // bp@4
	int v6;      // edi@9
	int j;       // eax@16
	int v8;      // edx@17
	int v9;      // ecx@23
	void* k;     // esi@28
	int v11;     // edx@29
	int v12;     // eax@38
	int v13;     // [sp+10h] [bp-20h]@3
	int v14;     // [sp+14h] [bp-1Ch]@8
	void* v15;   // [sp+18h] [bp-18h]@1
	__int16 v16; // [sp+1Ch] [bp-14h]@4
	int v17;     // [sp+20h] [bp-10h]@2
	int v18;     // [sp+24h] [bp-Ch]@2
	int v19;     // [sp+28h] [bp-8h]@2
	int v20;     // [sp+2Ch] [bp-4h]@2
	result = dword_15021160;
	v2 = This;
	v15 = This;
	if( !dword_15021160 ){
		v17 = 0;
		v18 = 0;
		v19 = 0;
		v20 = 0;
		result = sub_15011270((int)&dword_15021160, (int)&v17);
	}
	v13 = result;
	if( result ){
		v4 = dword_15021158;
		v5 = *(ushort*)(result + 394);
		v16 = *(ushort*)(result + 394);
		for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
			if( (unsigned __int16)(v5 - *(ushort*)(i + 396)) < 0x7FFFu )
				v5 = *(ushort*)(i + 396);
		}
		result = 0;
		v14 = 0;
		do{
			v6 = v13;
			if( !result )
				v6 = v4;
			if( v6 ){
				while( 1 ){
					if( !v2 || (void*)v6 == v2 ){
						for( j = *(uint*)(v6 + 344); j; j = *(uint*)(v6 + 344) ){
							v8 = *(uint*)(j + 12);
							if( v16 == *(ushort*)(v8 + 4) )
								break;
							if( (unsigned __int16)(v16 - *(ushort*)(v8 + 4)) > 0x7FFFu )
								break;
							if( *(uint*)j ){
								*(uint*)(*(uint*)j + 4) = *(uint*)(j + 4);
							}else{
								if( *(uint*)(v6 + 344) == j )
									*(uint *)(v6 + 344) = *(uint *)(j + 4);
							}
							v9 = *(uint*)(j + 4);
							if( v9 ){
								*(uint*)v9 = *(uint*)j;
							}else{
								if( *(uint*)(v6 + 348) == j )
									*(uint *)(v6 + 348) = *(uint *)j;
							}
							*(uint*)j = 0;
							*(uint*)(j + 4) = 0;
							sub_15012CD0(v6, 2u, j);
						}
						for( k = *(void**)(v6 + 376); k; k = *(void**)(v6 + 376) ){
							v11 = *((uint*)k + 3);
							if( v5 == *(ushort*)(v11 + 4) )
								break;
							if( (unsigned __int16)(v5 - *(ushort*)(v11 + 4)) > 0x7FFFu )
								break;
							if( v14 )
								sub_150114E0(*((void**)k + 2), (void*)v11, *((uint*)k + 4));
							else
								(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(
								*((uint *)k + 2),
								*((uint *)k + 3),
								*((uint *)k + 4));
							if( *(uint*)k ){
								*(uint*)(*(uint*)k + 4) = *((uint*)k + 1);
							}else{
								if( *(void**)(v6 + 376) == k )
									*(uint *)(v6 + 376) = *((uint *)k + 1);
							}
							v12 = *((uint*)k + 1);
							if( v12 ){
								*(uint*)v12 = *(uint*)k;
							}else{
								if( *(void**)(v6 + 380) == k )
									*(uint *)(v6 + 380) = *(uint *)k;
							}
							*(uint*)k = 0;
							*((uint*)k + 1) = 0;
							SMemFree(k, ".?AUMESSAGE@@", -2, 0);
						}
					}
					v6 = *(uint*)(v6 + 4);
					if( !v6 )
						break;
					v2 = v15;
				}
				v4 = dword_15021158;
				v2 = v15;
				result = v14;
			}
			++result;
			v14 = result;
		}while( result <= 1 );
	}
	return result;
}

// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (15012CD0) --------------------------------------------------------
int __fastcall sub_15012CD0(int a1, unsigned __int8 a2, int a3)
{
	int v3;   // edx@2
	int v4;   // ecx@2
	int v5;   // edx@8
	int v7;   // edi@14
	void* v8; // esi@14
	int v9;   // eax@22
	int v10;  // ecx@27
	int v11;  // ecx@33
	if( a2 == 2 ){
		v4 = a1 + 376;
		v3 = *(uint*)a3;
		if( *(uint*)a3 )
			goto LABEL_41;
		if( !*(uint*)(a3 + 4) )
			return sub_1500E210(v4, a3);
		if( v3 ){
			LABEL_41:
			*(uint*)(v3 + 4) = *(uint*)(a3 + 4);
		}else{
			if( *(uint*)v4 == a3 )
				*(uint *)v4 = *(uint *)(a3 + 4);
		}
		v5 = *(uint*)(a3 + 4);
		if( v5 ){
			*(uint*)v5 = *(uint*)a3;
		}else{
			if( *(uint*)(v4 + 4) == a3 )
				*(uint *)(v4 + 4) = *(uint *)a3;
		}
		*(uint*)a3 = 0;
		*(uint*)(a3 + 4) = 0;
		return sub_1500E210(v4, a3);
	}
	v8 = *(void**)(a1 + 8 * a2 + 352);
	v7 = a1 + 8 * a2 + 352;
	if( v8 ){
		if( *((uint*)v8 + 5) )
			sub_150114E0(*((void**)v8 + 2), *((void**)v8 + 3), *((uint*)v8 + 4));
		else
			(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(
			*((uint *)v8 + 2),
			*((uint *)v8 + 3),
			*((uint *)v8 + 4));
		if( *(uint*)v8 ){
			*(uint*)(*(uint*)v8 + 4) = *((uint*)v8 + 1);
		}else{
			if( *(void**)v7 == v8 )
				*(uint *)v7 = *((uint *)v8 + 1);
		}
		v9 = *((uint*)v8 + 1);
		if( v9 ){
			*(uint*)v9 = *(uint*)v8;
		}else{
			if( *(void**)(v7 + 4) == v8 )
				*(uint *)(v7 + 4) = *(uint *)v8;
		}
		*(uint*)v8 = 0;
		*((uint*)v8 + 1) = 0;
		SMemFree(v8, ".?AUMESSAGE@@", -2, 0);
	}
	v10 = *(uint*)a3;
	if( *(uint*)a3 )
		goto LABEL_30;
	if( *(uint*)(a3 + 4) ){
		if( !v10 ){
			if( *(uint*)v7 == a3 )
				*(uint *)v7 = *(uint *)(a3 + 4);
			goto LABEL_33;
		}
		LABEL_30:
		*(uint*)(v10 + 4) = *(uint*)(a3 + 4);
		LABEL_33:
		v11 = *(uint*)(a3 + 4);
		if( v11 ){
			*(uint*)v11 = *(uint*)a3;
		}else{
			if( *(uint*)(v7 + 4) == a3 )
				*(uint *)(v7 + 4) = *(uint *)a3;
		}
		*(uint*)a3 = 0;
		*(uint*)(a3 + 4) = 0;
	}
	return sub_1500E210(v7, a3);
}

// 15021184: using guessed type int dword_15021184;
//----- (15012E30) --------------------------------------------------------
char __stdcall sub_15012E30()
{
	int v0;          // eax@1
	int v1;          // edi@1
	int v2;          // esi@8
	unsigned int i;  // edx@10
	int v4;          // ecx@10
	int j;           // esi@18
	int v7;          // [sp+10h] [bp-28h]@3
	unsigned int v8; // [sp+14h] [bp-24h]@20
	int v9;          // [sp+18h] [bp-20h]@2
	int v10;         // [sp+1Ch] [bp-1Ch]@2
	int v11;         // [sp+20h] [bp-18h]@2
	int v12;         // [sp+24h] [bp-14h]@2
	int v13;         // [sp+28h] [bp-10h]@24
	int v14;         // [sp+2Ch] [bp-Ch]@24
	int v15;         // [sp+30h] [bp-8h]@24
	int v16;         // [sp+34h] [bp-4h]@24
	v0 = dword_15021160;
	v1 = dword_15021158;
	if( !dword_15021160 ){
		v9 = 0;
		v10 = 0;
		v11 = 0;
		v12 = 0;
		v0 = sub_15011270((int)&dword_15021160, (int)&v9);
	}
	v7 = v0;
	if( v0 ){
		for( ; v1; v1 = *(uint*)(v1 + 4) ){
			if( *(uchar*)(v1 + 280) & 8 ){
				LOBYTE_IDA(v0) = *(uchar*)(v1 + 438);
				if( (uchar)v0 != -1 ){
					if( (unsigned __int16)(*(ushort*)(v1 + 394) - *(ushort*)(v1 + 436)) <= 0x7FFFu ){
						sub_15013010(3, (unsigned __int8)v0, (void*)(v1 + 432), 4u);
						*(uint*)(v1 + 280) = 0;
						*(uchar*)(v1 + 438) = -1;
						v2 = 0;
						if( (uchar)dword_1501E2EC != -1 ){
							if( !*(uint*)(v7 + 424) ){
								v4 = dword_15021158;
								for( i = -1; v4; v4 = *(uint*)(v4 + 4) ){
									if( *(uchar*)(v4 + 438) != -1 ){
										if( *(uint*)(v4 + 424) ){
											i = 0;
										}else{
											if( *(uchar*)(v4 + 438) < i )
												i = *(uchar*)(v4 + 438);
										}
									}
								}
								if( (unsigned __int8)dword_1501E2EC < i ){
									*(uint*)(v7 + 424) = 1;
									for( j = dword_15021158; j; j = *(uint*)(j + 4) ){
										if( *(uchar*)(j + 438) != -1 ){
											v8 = (unsigned __int8)dword_1501E2EC;
											sub_150118B0(j, 0, 13, &v8, 4u);
										}
									}
									v2 = 1;
								}
							}
						}
						v0 = dword_15021160;
						if( dword_15021160
						|| (v13 = 0, v14 = 0, v15 = 0, v16 = 0, (v0 = sub_15011270((int)&dword_15021160, (int)&v13)) != 0) ){
							if( *(uint*)(v0 + 424) ){
								v8 = 0;
								SNetGetNumPlayers(0, 0, (int)&v8);
								LOBYTE_IDA(v0) = v8;
								if( v8 < dword_15021340 ){
									v0 = dword_15021230;
									v2 = 1;
									LOBYTE_IDA(v0) = dword_15021230 & 0xFD;
									dword_15021230 = v0;
								}
							}
						}
						if( v2 )
							LOBYTE_IDA(v0) = SNetSetGameMode(dword_15021230);
					}
				}
			}
		}
	}
	return v0;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021230: using guessed type int dword_15021230;
// 15021340: using guessed type int dword_15021340;
//----- (15013010) --------------------------------------------------------
void* __fastcall sub_15013010(int a1, int a2, void* a3, unsigned int a4)
{
	int result; // eax@1
	int v5;     // ebx@1
	int v6;     // edi@1
	int v7;     // esi@1
	int v8;     // eax@1
	v7 = a2;
	v6 = a1;
	v8 = SMemAlloc(24, ".?AUUSEREVENT@@", -2, 8);
	v5 = v8;
	*(uint*)v8 = dword_1502111C;
	result = dword_15021118;
	if( dword_15021118 ){
		*(uint*)(dword_1502111C + 4) = v5;
		dword_1502111C = v5;
	}else{
		dword_1502111C = v5;
		dword_15021118 = v5;
	}
	*(uint*)(v5 + 12) = v7;
	*(uint*)(v5 + 8) = v6;
	if( a3 ){
		if( a4 ){
			result = SMemAlloc(a4, "SNET.CPP", 2129, 0);
			*(uint*)(v5 + 16) = result;
			memcpy((void*)result, a3, a4);
			*(uint*)(v5 + 20) = a4;
		}
	}
	return (void*)result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021118: using guessed type int dword_15021118;
// 1502111C: using guessed type int dword_1502111C;
//----- (150130A0) --------------------------------------------------------
int __stdcall SNetGetPerformanceData(unsigned int a1, int a2, int a3, int a4, char* lpFileTime, int a6)
{
	int result;                    // eax@2
	unsigned int v7;               // esi@3
	char* v8;                      // edi@7
	struct _SYSTEMTIME* v9;        // eax@9
	char v10;                      // [sp+8h] [bp-18h]@14
	struct _SYSTEMTIME SystemTime; // [sp+10h] [bp-10h]@8
	if( a2 != 0 ? (a1 != 0 ? -(a1 < 0x10) : 0) : 0 ){
		EnterCriticalSection(&NetCritical);
		v7 = 16 * a1;
		*(uint*)a2 = dword_1501E1D8[4 * a1];
		if( a3 )
			*(uint *)a3 = *(int *)((char *)&dword_1501E1DC + v7);
		if( a4 )
			*(uint *)a4 = *(int *)((char *)&dword_1501E1E0 + v7);
		v8 = lpFileTime;
		if( lpFileTime ){
			GetSystemTime(&SystemTime);
			SystemTimeToFileTime(&SystemTime, (LPFILETIME)lpFileTime);
		}
		v9 = (struct _SYSTEMTIME*)a6;
		if( a6 ){
			*(uint*)a6 = 10000000;
			*(uint*)(a6 + 4) = 0;
		}
		if( *(int*)((char*)&dword_1501E1E4 + v7) ){
			if( dword_15021184 ){
				if( !lpFileTime )
					v8 = &v10;
				if( !a6 )
					v9 = &SystemTime;
				(*(void (__stdcall **)(unsigned int, int, char *, struct _SYSTEMTIME *))(dword_15021184 + 24))(a1, a2, v8, v9);
			}
		}
		LeaveCriticalSection(&NetCritical);
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E1D8: using guessed type int dword_1501E1D8[];
// 1501E1DC: using guessed type int dword_1501E1DC;
// 1501E1E0: using guessed type int dword_1501E1E0;
// 1501E1E4: using guessed type int dword_1501E1E4;
// 15021184: using guessed type int dword_15021184;
//----- (15013190) --------------------------------------------------------
int __stdcall SNetGetPlayerCaps(int a1, int a2)
{
	int v3;   // ecx@7
	int v4;   // edx@8
	int* v5;  // eax@9
	int v6;   // eax@11
	int v7;   // eax@18
	DWORD v8; // [sp-4h] [bp-10h]@6
	if( !(a2 != 0 ? (*(uint*)a2 != 36) - 1 : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	memset((void*)(a2 + 4), 0, 0x20u);
	if( !dword_15021184 || !dword_15021190 ){
		v8 = 1204;
		goto LABEL_22;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v8 = -2062548880;
		LABEL_22:
		SErrSetLastError(v8);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v3 = a1 - dword_15021180;
	if( a1 - dword_15021180 == 255 ){
		LABEL_15:
		v6 = 0;
	}else{
		v4 = 1;
		while( 1 ){
			v5 = &dword_15021160;
			if( !v4 )
				v5 = &dword_15021158;
			v6 = *v5;
			if( v6 )
				break;
			LABEL_14:
			--v4;
			if( v4 < 0 )
				goto LABEL_15;
		}
		while( *(uchar*)(v6 + 438) != (uchar)v3 ){
			v6 = *(uint*)(v6 + 4);
			if( !v6 )
				goto LABEL_14;
		}
	}
	if( !v6 ){
		v8 = -2062548886;
		goto LABEL_22;
	}
	memcpy((void*)a2, (void*)(dword_15021190 + 532), 0x24u);
	v7 = *(uint*)(v6 + 296);
	if( v7 )
		*(uint *)(a2 + 24) = v7;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
// 15021190: using guessed type int dword_15021190;
//----- (150132A0) --------------------------------------------------------
int __stdcall SNetGetPlayerName(int a1, char* Dest, size_t Count)
{
	int v4;   // ecx@7
	int v5;   // edx@8
	int* v6;  // eax@9
	int v7;   // eax@11
	DWORD v8; // [sp-4h] [bp-Ch]@4
	if( !(Dest != 0 ? -(Count != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	*Dest = 0;
	if( dword_15021184 ){
		if( (uchar)dword_1501E2EC == -1 ){
			v8 = -2062548880;
		}else{
			v4 = a1 - dword_15021180;
			if( a1 - dword_15021180 == 255 ){
				LABEL_15:
				v7 = 0;
			}else{
				v5 = 1;
				while( 1 ){
					v6 = &dword_15021160;
					if( !v5 )
						v6 = &dword_15021158;
					v7 = *v6;
					if( v7 )
						break;
					LABEL_14:
					--v5;
					if( v5 < 0 )
						goto LABEL_15;
				}
				while( *(uchar*)(v7 + 438) != (uchar)v4 ){
					v7 = *(uint*)(v7 + 4);
					if( !v7 )
						goto LABEL_14;
				}
			}
			if( v7 && !(*(uchar*)(v7 + 280) & 4) ){
				strncpy(Dest, (char*)(v7 + 8), Count);
				Dest[Count - 1] = 0;
				LeaveCriticalSection(&NetCritical);
				return 1;
			}
			v8 = -2062548886;
		}
	}else{
		v8 = 1204;
	}
	SErrSetLastError(v8);
	SErrGetLastError();
	LeaveCriticalSection(&NetCritical);
	return 0;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
//----- (15013390) --------------------------------------------------------
int __stdcall SNetGetProviderCaps( CAPS* providerCaps )
{
	//if( ExternalStorm ) return eSNetGetProviderCaps(providerCaps);
	int result; // eax@2
	if( providerCaps != 0 ? (*(uint*)providerCaps != 36) - 1 : 0 ){
		EnterCriticalSection(&NetCritical);
		memset((void*)(providerCaps + 4), 0, 0x20u);
		if( dword_15021184 && dword_15021190 ){
			memcpy((void*)providerCaps, (void*)(dword_15021190 + 532), 0x24u);
			LeaveCriticalSection(&NetCritical);
			result = 1;
		}else{
			SErrSetLastError(0x4B4u);
			SErrGetLastError();
			LeaveCriticalSection(&NetCritical);
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 15021184: using guessed type int dword_15021184;
// 15021190: using guessed type int dword_15021190;
//----- (15013430) --------------------------------------------------------
int __stdcall SNetGetTurnsInTransit(int* turns)
{
	//if( ExternalStorm ) return eSNetGetTurnsInTransit(turns);
	int v2;   // eax@7
	DWORD v3; // [sp-4h] [bp-18h]@4
	int v4;   // [sp+4h] [bp-10h]@8
	int v5;   // [sp+8h] [bp-Ch]@8
	int v6;   // [sp+Ch] [bp-8h]@8
	int v7;   // [sp+10h] [bp-4h]@8
	if( !turns ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	*(uint*)turns = 0;
	if( !dword_15021184 ){
		v3 = 1204;
		LABEL_10:
		SErrSetLastError(v3);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v3 = -2062548880;
		goto LABEL_10;
	}
	v2 = dword_15021160;
	if( !dword_15021160 ){
		v4 = dword_15021160;
		v5 = dword_15021160;
		v6 = dword_15021160;
		v7 = dword_15021160;
		v2 = sub_15011270((int)&dword_15021160, (int)&v4);
		if( !v2 ){
			v3 = 8;
			goto LABEL_10;
		}
	}
	*(uint*)turns = *(ushort*)(v2 + 388) - *(ushort*)(v2 + 394);
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (15013500) --------------------------------------------------------
int __stdcall SNetInitializeDevice(int a1, void* a2, void* a3, void* a4, void* a5)
{
	char v6; // [sp+0h] [bp-80h]@3
	char v7; // [sp+Ch] [bp-74h]@3
	char v8; // [sp+20h] [bp-60h]@3
	char v9; // [sp+4Ch] [bp-34h]@3
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		SErrSetLastError(0x4B4u);
		LABEL_6:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( !sub_150135D0(a2, a3, a4, a5, (int)&v8, (int)&v6, (int)&v9, (int)&v7) ){
		SErrSetLastError(0x57u);
		goto LABEL_6;
	}
	if( !(*(int (__stdcall **)(int, char *, char *, char *, char *))(dword_15021184 + 32))(a1, &v8, &v6, &v9, &v7) )
		goto LABEL_6;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 15021184: using guessed type int dword_15021184;
//----- (150135D0) --------------------------------------------------------
int __fastcall sub_150135D0(void* a1, void* a2, void* a3, void* a4, int a5, int a6, int a7, int a8)
{
	void* v8;         // ebp@1
	void* v9;         // esi@1
	unsigned int v10; // ecx@2
	int v11;          // eax@5
	unsigned int v12; // ecx@10
	int v13;          // ecx@13
	unsigned int v14; // ecx@18
	int v15;          // eax@21
	unsigned int v16; // ecx@26
	char Dest;        // [sp+10h] [bp-100h]@7
	char v19;         // [sp+11h] [bp-FFh]@7
	__int16 v20;      // [sp+10Dh] [bp-3h]@7
	char v21;         // [sp+10Fh] [bp-1h]@7
	v9 = a1;
	v8 = a2;
	memset((void*)a5, 0, 44);
	if( a1 ){
		v10 = *(uint*)a1;
		if( *(uint*)v9 > 44 )
			v10 = 44;
		memcpy((void*)a5, v9, v10);
	}
	v11 = *(uint*)(a5 + 4);
	*(uint*)a5 = 44;
	if( !v11 )
		*(uint *)(a5 + 4) = (uint)&ZeroByte;
	Dest = ZeroByte;
	memset(&v19, 0, 0xFCu);
	v20 = 0;
	v21 = 0;
	if( !*(uint*)(a5 + 8) ){
		sub_15013780(*(char**)(a5 + 4), a4, &Dest, 256);
		*(uint*)(a5 + 8) = (uint)&Dest;
	}
	*(uint*)a6 = 0;
	*(uint*)(a6 + 4) = 0;
	*(uint*)(a6 + 8) = 0;
	if( v8 ){
		v12 = *(uint*)v8;
		if( *(uint*)v8 > 0xCu )
			v12 = 12;
		memcpy((void*)a6, v8, v12);
	}
	v13 = *(uint*)(a6 + 4);
	*(uint*)a6 = 12;
	if( !v13 )
		*(uint *)(a6 + 4) = (uint)&ZeroByte;
	if( !*(uint*)(a6 + 8) )
		*(uint *)(a6 + 8) = (uint)&ZeroByte;
	memset((void*)a7, 0, 0x34u);
	if( a3 ){
		v14 = *(uint*)a3;
		if( *(uint*)a3 > 0x34u )
			v14 = 52;
		memcpy((void*)a7, a3, v14);
	}
	v15 = *(uint*)(a7 + 8);
	*(uint*)a7 = 52;
	if( !v15 )
		*(uint *)(a7 + 8) = (uint)SDrawGetFrameWindow(0);
	if( !*(uint*)(a7 + 32) )
		*(uint *)(a7 + 32) = (uint)MessageBoxA;
	*(uint*)a8 = 0;
	*(uint*)(a8 + 4) = 0;
	*(uint*)(a8 + 8) = 0;
	*(uint*)(a8 + 12) = 0;
	*(uint*)(a8 + 16) = 0;
	if( a4 ){
		v16 = *(uint*)a4;
		if( *(uint*)a4 > 0x14u )
			v16 = 20;
		memcpy((void*)a8, a4, v16);
	}
	*(uint*)a8 = 20;
	return 1;
}

//----- (15013780) --------------------------------------------------------
char __fastcall sub_15013780(char* Source, void* a2, char* Dest, int a4)
{
	int v4;             // ecx@1
	wchar_t v5[2];      // edi@1
	char v6;            // zf@3
	unsigned int v7;    // edx@5
	int v8;             // ecx@5
	int v9;             // ecx@5
	char* v10;          // edi@5
	void* v11;          // esi@5
	char v12;           // zf@7
	char* v13;          // edx@10
	char result;        // al@12
	void* v15;          // esi@14
	DWORD v16;          // edi@14
	LPVOID v17;         // [sp+10h] [bp-110h]@1
	DWORD dwHandle;     // [sp+14h] [bp-10Ch]@14
	unsigned int puLen; // [sp+18h] [bp-108h]@15
	CHAR tstrFilename;  // [sp+1Ch] [bp-104h]@9
	char v21;           // [sp+1Dh] [bp-103h]@9
	__int16 v22;        // [sp+11Dh] [bp-3h]@9
	char v23;           // [sp+11Fh] [bp-1h]@9
	v17 = a2;
	strncpy(Dest, Source, a4 - 2);
	*(uint*)v5 = (uint)L" ";
	v4 = -1;
	Dest[a4 - 2] = 0;
	do{
		if( !v4 )
			break;
		v6 = **(uchar**)v5 == 0;
		++*(uint*)v5;
		--v4;
	}while( !v6 );
	v9 = ~v4;
	v11 = (void*)(*(uint*)v5 - v9);
	v7 = v9;
	v10 = Dest;
	v8 = -1;
	do{
		if( !v8 )
			break;
		v12 = *v10++ == 0;
		--v8;
	}while( !v12 );
	memcpy(v10 - 1, v11, v7);
	tstrFilename = ZeroByte;
	memset(&v21, 0, 0x100u);
	v22 = 0;
	v23 = 0;
	GetModuleFileNameA(0, &tstrFilename, 0x104u);
	if( v17 && (v13 = (char*)*((uint*)v17 + 1)) != 0 && *v13 ){
		result = (unsigned int)strncpy(&Dest[strlen(Dest) - 1], v13, a4 - (strlen(Dest) - 1));
		Dest[a4 - 1] = 0;
	}else{
		result = tstrFilename;
		if( tstrFilename ){
			v16 = GetFileVersionInfoSizeA(&tstrFilename, &dwHandle);
			v15 = (void*)SMemAlloc(v16, "SNET.CPP", 2635, 0);
			if( GetFileVersionInfoA(&tstrFilename, dwHandle, v16, (LPVOID)v15) ){
				v17 = 0;
				puLen = 0;
				if( VerQueryValueA(v15, "\\StringFileInfo\\040904b0\\ProductVersion", &v17, &puLen) ){
					if( v17 ){
						if( *(uchar*)v17 ){
							strncpy(&Dest[strlen(Dest) - 1], (char*)v17, a4 - (strlen(Dest) - 1));
							Dest[a4 - 1] = 0;
						}
					}
				}
			}
			result = SMemFree((void*)v15, "SNET.CPP", 2648, 0);
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501C854: using guessed type wchar_t asc_1501C854[2];
//----- (15013920) --------------------------------------------------------
int __stdcall SNetInitializeProvider(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	//if( ExternalStorm ) return eSNetInitializeProvider(a1, a2, a3, a4, a5);
	int v6;   // [sp+0h] [bp-84h]@3
	char v7;  // [sp+4h] [bp-80h]@1
	char v8;  // [sp+10h] [bp-74h]@1
	char v9;  // [sp+24h] [bp-60h]@1
	int v10;  // [sp+4Ch] [bp-38h]@7
	char v11; // [sp+50h] [bp-34h]@1
	EnterCriticalSection(&NetCritical);
	if( !sub_150135D0(a2, a3, a4, a5, (int)&v9, (int)&v7, (int)&v11, (int)&v8) ){
		SErrSetLastError(0x57u);
		LABEL_6:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	SEvtUnregisterType(1397638484, 1);
	SEvtUnregisterType(1397638484, 2);
	SEvtRegisterHandler(1397638484, 2, 2, 0, (int)sub_15013D20);
	SEvtRegisterHandler(1397638484, 2, 12, 0, (int)sub_15013D90);
	SEvtRegisterHandler(1397638484, 2, 13, 0, (int)sub_15013E10);
	SEvtRegisterHandler(1397638484, 2, 4, 0, (int)sub_15013E60);
	SEvtRegisterHandler(1397638484, 2, 5, 0, (int)sub_15013EC0);
	SEvtRegisterHandler(1397638484, 2, 6, 0, (int)sub_15013F30);
	SEvtRegisterHandler(1397638484, 2, 7, 0, (int)sub_15014200);
	SEvtRegisterHandler(1397638484, 2, 8, 0, (int)sub_15014790);
	SEvtRegisterHandler(1397638484, 2, 9, 0, (int)sub_15014890);
	SEvtRegisterHandler(1397638484, 2, 10, 0, (int)nullsub_1);
	SEvtRegisterHandler(1397638484, 2, 11, 0, (int)sub_15014980);
	v6 = 0;
	if( !sub_15014A00((DWORD)&v6) ){
		SErrSetLastError(0xA4u);
		goto LABEL_6;
	}
	if( !sub_15013B00((int)a1, (int)&v9, (int)&v7, (int)&v11, (int)&v8, v6) )
		goto LABEL_6;
	dword_1502133C = v10;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 15013D90: using guessed type int __stdcall sub_15013D90(int);
// 15013E10: using guessed type int __stdcall sub_15013E10(int);
// 15013E60: using guessed type int __stdcall sub_15013E60(int);
// 15014970: using guessed type int __stdcall nullsub_1(int);
// 15014980: using guessed type int __stdcall sub_15014980(int);
// 1502133C: using guessed type int dword_1502133C;
//----- (15013B00) --------------------------------------------------------
int __fastcall sub_15013B00(int a1, int a2, int a3, int a4, int a5, int a6)
{
	int v6;           // eax@1
	int v7;           // ebx@1
	int v8;           // esi@1
	int v9;           // ecx@5
	int v10;          // ecx@5
	int v11;          // ecx@7
	int v12;          // ecx@7
	int v13;          // ecx@9
	int v14;          // ecx@9
	unsigned int v15; // ecx@14
	void* v16;        // eax@17
	HMODULE v17;      // eax@19
	FARPROC v18;      // eax@20
	void* v19;        // esi@21
	int v20;          // eax@23
	int result;       // eax@26
	DWORD v22;        // esi@26
	int v23;          // [sp+8h] [bp-4h]@1
	v23 = a1;
	v8 = a1;
	v7 = a2;
	sub_150115A0(0);
	sub_150120B0();
	v6 = dword_15021110;
	dword_15021190 = dword_15021110;
	if( !dword_15021110 )
		goto LABEL_32;
	do{
		if( *(uint*)(v6 + 272) == v8 )
			break;
		v6 = *(uint*)(v6 + 4);
		dword_15021190 = v6;
	}while( v6 );
	if( !v6 )
		goto LABEL_32;
	v10 = *(uint*)(v6 + 556);
	dword_1501E2D8 = 250;
	v9 = 5 * v10;
	if( (unsigned int)v9 >= 0xFA )
		dword_1501E2D8 = v9;
	v12 = *(uint*)(v6 + 556);
	dword_1501E2DC = 5000;
	v11 = 12 * v12;
	if( (unsigned int)v11 >= 0x1388 )
		dword_1501E2DC = v11;
	v14 = *(uint*)(v6 + 556);
	dword_1501E2E0 = 1000;
	v13 = 4 * v14;
	if( (unsigned int)v13 >= 0x3E8 )
		dword_1501E2E0 = v13;
	if( (*(uint*)(v6 + 556) & 0xFFFFFFFEu) >= 0x32 )
		dword_1501E2E4 = *(uint*)(v6 + 556) >> 1;
	else
		dword_1501E2E4 = 25;
	v15 = *(uint*)(v6 + 556);
	dwMilliseconds = 50;
	if( v15 >= 0x32 )
		dwMilliseconds = v15;
	if( *(uchar*)(v6 + 536) & 1 ){
		v16 = VirtualAlloc(0, 0x2000u, MEM_COMMIT, 0x40/*PAGE_EXECUTE_READWRITE*/);
		lpAddress = v16;
		if( !v16 ){
			sub_150115A0(0);
			SErrSetLastError(8u);
			return 0;
		}
		VirtualLock(v16, 0x2000u);
		v6 = dword_15021190;
	}
	v17 = LoadLibraryA((LPCSTR)(v6 + 8));
	hLibModule = v17;
	if( !v17
	|| (v18 = GetProcAddress(v17, "SnpBind"), !v18)
	|| (v23 = 0,
	((void (__stdcall *)(uint, int *))v18)(*(uint *)(dword_15021190 + 268), &v23),
	v19 = ( void *)v23,
	!v23)
	|| *(uint *)v23 < 0x50u ){
		LABEL_32:
		sub_150115A0(0);
		SErrSetLastError(0x4B4u);
		return 0;
	}
	v20 = dword_15021184;
	if( !dword_15021184 ){
		v20 = SMemAlloc(80, "SNET.CPP", 802, dword_15021184);
		v19 = (void*)v23;
		dword_15021184 = v20;
	}
	memcpy((void*)v20, v19, 0x50u);
	if( (*(int (__stdcall **)(int, int, int, int, int))(dword_15021184 + 28))(v7, a3, a4, a5, a6) ){
		result = 1;
	}else{
		v22 = SErrGetLastError();
		sub_150115A0(0);
		SErrSetLastError(v22);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E2D8: using guessed type int dword_1501E2D8;
// 1501E2DC: using guessed type int dword_1501E2DC;
// 1501E2E0: using guessed type int dword_1501E2E0;
// 1501E2E4: using guessed type int dword_1501E2E4;
// 15021110: using guessed type int dword_15021110;
// 15021184: using guessed type int dword_15021184;
// 15021190: using guessed type int dword_15021190;
//----- (15013D20) --------------------------------------------------------
void __stdcall sub_15013D20(int a1)
{
	int v1;  // eax@3
	char v2; // zf@4
	int v3;  // ecx@4
	int v4;  // edi@4
	int v5;  // esi@4
	if( *(uint*)(a1 + 8) == 4 ){
		if( **(uint**)(a1 + 4) == 1 ){
			v1 = dword_15021158;
			if( !dword_15021158 )
				goto LABEL_15;
			do{
				v3 = 16;
				v4 = *(uint*)a1;
				v5 = v1 + 264;
				v2 = 1;
				do{
					if( !v3 )
						break;
					v2 = *(uchar*)v5++ == *(uchar*)v4++;
					--v3;
				}while( v2 );
				if( v2 )
					break;
				v1 = *(uint*)(v1 + 4);
			}while( v1 );
			if( !v1 )
				LABEL_15:
				v1 = sub_15011270((int)&dword_15021158, *(uint*)a1);
			sub_150118B0(v1, 0, 3, *(void**)(a1 + 4), *(uint*)(a1 + 8));
		}
	}
}

// 15021158: using guessed type int dword_15021158;
//----- (15013D90) --------------------------------------------------------
int __stdcall sub_15013D90(int a1)
{
	int v1;     // ecx@1
	int v2;     // esi@1
	int v3;     // edx@2
	int* v4;    // eax@3
	int result; // eax@5
	v2 = *(uint*)(a1 + 4);
	v1 = *(uint*)v2;
	if( *(uint*)v2 == 255 ){
		LABEL_9:
		result = 0;
	}else{
		v3 = 1;
		while( 1 ){
			v4 = &dword_15021160;
			if( !v3 )
				v4 = &dword_15021158;
			result = *v4;
			if( result )
				break;
			LABEL_8:
			--v3;
			if( v3 < 0 )
				goto LABEL_9;
		}
		while( *(uchar*)(result + 438) != (uchar)v1 ){
			result = *(uint*)(result + 4);
			if( !result )
				goto LABEL_8;
		}
	}
	if( result ){
		if( *(uchar*)(result + 438) != -1 ){
			*(uint*)(result + 280) |= 8u;
			*(ushort*)(result + 436) = *(ushort*)(v2 + 4);
			*(uint*)(result + 432) = *(uint*)(v2 + 8);
		}
	}
	return result;
}

// 15013D90: using guessed type int __stdcall sub_15013D90(int);
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
//----- (15013E10) --------------------------------------------------------
int __stdcall sub_15013E10(int a1)
{
	int v1;     // ecx@1
	int v2;     // edx@2
	int* v3;    // eax@3
	int result; // eax@5
	v1 = **(uint**)(a1 + 4);
	if( v1 == 255 ){
		LABEL_9:
		result = 0;
	}else{
		v2 = 1;
		while( 1 ){
			v3 = &dword_15021160;
			if( !v2 )
				v3 = &dword_15021158;
			result = *v3;
			if( result )
				break;
			LABEL_8:
			--v2;
			if( v2 < 0 )
				goto LABEL_9;
		}
		while( *(uchar*)(result + 438) != (uchar)v1 ){
			result = *(uint*)(result + 4);
			if( !result )
				goto LABEL_8;
		}
	}
	if( result )
		*(uint *)(result + 424) = 1;
	return result;
}

// 15013E10: using guessed type int __stdcall sub_15013E10(int);
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
//----- (15013E60) --------------------------------------------------------
int __stdcall sub_15013E60(int a1)
{
	int v1;  // eax@1
	char v2; // zf@2
	int v3;  // ecx@2
	int v4;  // edi@2
	int v5;  // esi@2
	v1 = dword_15021158;
	if( !dword_15021158 )
		goto LABEL_13;
	do{
		v3 = 16;
		v4 = *(uint*)a1;
		v5 = v1 + 264;
		v2 = 1;
		do{
			if( !v3 )
				break;
			v2 = *(uchar*)v5++ == *(uchar*)v4++;
			--v3;
		}while( v2 );
		if( v2 )
			break;
		v1 = *(uint*)(v1 + 4);
	}while( v1 );
	if( !v1 )
		LABEL_13:
		v1 = sub_15011270((int)&dword_15021158, *(uint*)a1);
	return sub_150118B0(v1, 0, 5, *(void**)(a1 + 4), *(uint*)(a1 + 8));
}

// 15013E60: using guessed type int __stdcall sub_15013E60(int);
// 15021158: using guessed type int dword_15021158;
//----- (15013EC0) --------------------------------------------------------
DWORD __stdcall sub_15013EC0(int* a1)
{
	int v1;       // ebx@1
	char v2;      // zf@2
	int v3;       // ecx@2
	int v4;       // edi@2
	int v5;       // esi@2
	DWORD result; // eax@10
	DWORD v7;     // ecx@11
	v1 = dword_15021158;
	if( !dword_15021158 )
		goto LABEL_16;
	do{
		v3 = 16;
		v4 = *a1;
		v5 = v1 + 264;
		v2 = 1;
		do{
			if( !v3 )
				break;
			v2 = *(uchar*)v5++ == *(uchar*)v4++;
			--v3;
		}while( v2 );
		if( v2 )
			break;
		v1 = *(uint*)(v1 + 4);
	}while( v1 );
	if( !v1 )
		LABEL_16:
		v1 = sub_15011270((int)&dword_15021158, *a1);
	result = GetTickCount();
	if( v1 ){
		v7 = *(uint*)(v1 + 300);
		result -= *(uint*)(v1 + 292);
		*(uint*)(v1 + 296) = result;
		if( v7 > result )
			result = v7;
		*(uint*)(v1 + 300) = result;
	}
	return result;
}

// 15021158: using guessed type int dword_15021158;
//----- (15013F30) --------------------------------------------------------
void* __stdcall sub_15013F30(int a1)
{
	void* result; // eax@1
	int v2;       // ebx@1
	int v3;       // esi@1
	int v4;       // ecx@1
	int v5;       // eax@3
	char v6;      // zf@4
	int v7;       // ecx@4
	int v8;       // edi@4
	int v9;       // esi@4
	int v10;      // ebp@13
	char v11;     // zf@14
	int v12;      // ecx@14
	int v13;      // edi@14
	int v14;      // esi@14
	int v15;      // edi@23
	int v16;      // eax@24
	__int16 v17;  // dx@27
	__int16 v18;  // cx@27
	char* v19;    // edi@30
	int v20;      // esi@31
	int v21;      // edx@32
	int v22;      // esi@36
	int v23;      // edx@37
	int v24;      // [sp+10h] [bp-14h]@23
	int v25;      // [sp+14h] [bp-10h]@24
	int v26;      // [sp+18h] [bp-Ch]@24
	int v27;      // [sp+1Ch] [bp-8h]@24
	int v28;      // [sp+20h] [bp-4h]@24
	void* v29;    // [sp+28h] [bp+4h]@1
	v3 = a1;
	result = *(void**)(a1 + 4);
	v4 = *(uchar*)(a1 + 12);
	v29 = result;
	v2 = (int)((char*)result + 20);
	if( *((uint*)result + 1) == v4 ){
		v2 = *(uint*)v3;
	}else{
		v5 = dword_15021158;
		if( !dword_15021158 )
			goto LABEL_47;
		do{
			v7 = 16;
			v8 = v2;
			v9 = v5 + 264;
			v6 = 1;
			do{
				if( !v7 )
					break;
				v6 = *(uchar*)v9++ == *(uchar*)v8++;
				--v7;
			}while( v6 );
			if( v6 )
				break;
			v5 = *(uint*)(v5 + 4);
		}while( v5 );
		if( !v5 )
			LABEL_47:
			v5 = sub_15011270((int)&dword_15021158, v2);
		sub_15011370((void*)v5);
		result = v29;
	}
	v10 = dword_15021158;
	if( !dword_15021158 )
		goto LABEL_48;
	do{
		v12 = 16;
		v13 = v2;
		v14 = v10 + 264;
		v11 = 1;
		do{
			if( !v12 )
				break;
			v11 = *(uchar*)v14++ == *(uchar*)v13++;
			--v12;
		}while( v11 );
		if( v11 )
			break;
		v10 = *(uint*)(v10 + 4);
	}while( v10 );
	if( !v10 ){
		LABEL_48:
		v10 = sub_15011270((int)&dword_15021158, v2);
		result = v29;
	}
	if( dword_15021160 ){
		v15 = dword_15021160;
		v24 = dword_15021160;
	}else{
		v25 = 0;
		v26 = 0;
		v27 = 0;
		v28 = 0;
		v16 = sub_15011270((int)&dword_15021160, (int)&v25);
		v24 = v16;
		v15 = v16;
		result = v29;
	}
	if( v10 ){
		if( v15 ){
			*(uint*)(v10 + 280) = *((uint*)result + 3);
			*(uchar*)(v10 + 438) = *((uchar*)result + 4);
			*(uint*)(v10 + 424) = *((uint*)result + 2);
			*(ushort*)(v10 + 394) = *((ushort*)result + 8);
			*(ushort*)(v10 + 402) = *((ushort*)result + 8);
			v18 = *(ushort*)(v10 + 394);
			*(ushort*)(v10 + 388) = *(ushort*)(v15 + 388);
			v17 = *(ushort*)(v15 + 394);
			if( v18 != v17 ){
				if( (unsigned __int16)(v18 - v17) < 0x7FFFu )
					*(uint *)(v10 + 280) |= 4u;
			}
			v19 = (char*)result + 36;
			*(uint*)(v10 + 264) = *(uint*)v2;
			*(uint*)(v10 + 268) = *(uint*)(v2 + 4);
			*(uint*)(v10 + 272) = *(uint*)(v2 + 8);
			*(uint*)(v10 + 276) = *(uint*)(v2 + 12);
			strncpy((char*)(v10 + 8), (char*)result + 36, 0x80u);
			strncpy((char*)(v10 + 136), (char*)v29 + strlen(v19) + 36, 0x80u);
			v15 = v24;
			result = v29;
		}
	}
	v20 = *(uint*)(v15 + 376);
	if( v20 ){
		do{
			v21 = *(uint*)(v20 + 12);
			if( (unsigned __int16)(*(ushort*)(v21 + 4) - *((ushort*)v29 + 8)) <= 0x7FFFu )
				sub_15014180(v10, (void*)v21, *(uint*)(v20 + 16));
			v20 = *(uint*)(v20 + 4);
		}while( v20 );
		result = v29;
	}
	v22 = *(uint*)(v15 + 344);
	if( v22 ){
		do{
			v23 = *(uint*)(v22 + 12);
			if( (unsigned __int16)(*(ushort*)(v23 + 4) - *((ushort*)v29 + 8)) <= 0x7FFFu )
				sub_15014180(v10, (void*)v23, *(uint*)(v22 + 16));
			v22 = *(uint*)(v22 + 4);
		}while( v22 );
		result = v29;
	}
	if( (uchar)dword_1501E2EC != -1 ){
		if( v10 ){
			if( !(*(uchar*)(v10 + 280) & 4) )
				result = sub_15013010(2, *((uint*)result + 1), 0, 0);
		}
	}
	return result;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
//----- (15014180) --------------------------------------------------------
int __fastcall sub_15014180(int a1, void* a2, unsigned int a3)
{
	void* v4; // esi@1
	int v5;   // ebp@1
	int v6;   // ebx@1
	v4 = a2;
	v5 = a1;
	v6 = SMemAlloc(a3, "SNET.CPP", 1398, 0);
	memcpy((void*)v6, v4, a3);
	*(ushort*)(v6 + 6) = *(ushort*)(v5 + 398 + 2 * *(uchar*)(v6 + 8));
	*(ushort*)v6 = sub_15011B00(v6);
	sub_15011BB0(v5, v6);
	SMemFree((void*)v6, "SNET.CPP", 1411, 0);
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15014200) --------------------------------------------------------
int __stdcall sub_15014200(int a1)
{
	char* v1;         // esi@1
	int v2;           // eax@2
	int v3;           // edx@2
	char v4;          // zf@3
	int v5;           // ecx@3
	int v6;           // edi@3
	int v7;           // esi@3
	unsigned int v8;  // esi@10
	char v9;          // zf@13
	int v10;          // ecx@13
	int v11;          // edi@13
	int v12;          // esi@13
	char v13;         // dl@21
	int v14;          // edi@23
	int* v15;         // eax@24
	int v16;          // eax@26
	int v17;          // edi@34
	int v18;          // ebx@35
	int v19;          // ebp@38
	char v20;         // zf@39
	int v21;          // ecx@39
	int v22;          // edi@39
	int v23;          // esi@39
	char v24;         // dl@45
	int v25;          // eax@46
	int result;       // eax@47
	__int16 v27;      // ax@49
	__int16 v28;      // cx@49
	char* v29;        // ST04_4@49
	int v30;          // eax@51
	int v31;          // ebx@54
	int i;            // edi@54
	int v33;          // esi@54
	char* v34;        // ebx@54
	int v35;          // eax@54
	int v36;          // eax@57
	int j;            // edi@59
	int v38;          // eax@62
	int k;            // esi@64
	char v40;         // zf@69
	int v41;          // ecx@69
	int v42;          // edi@69
	int v43;          // esi@69
	unsigned int v44; // [sp+10h] [bp-1B8h]@21
	unsigned int v45; // [sp+14h] [bp-1B4h]@1
	int v46;          // [sp+18h] [bp-1B0h]@34
	unsigned int v47; // [sp+1Ch] [bp-1ACh]@10
	char* Source;     // [sp+20h] [bp-1A8h]@1
	int v49;          // [sp+24h] [bp-1A4h]@37
	int v50;          // [sp+28h] [bp-1A0h]@37
	int v51;          // [sp+2Ch] [bp-19Ch]@37
	int v52;          // [sp+30h] [bp-198h]@37
	int v53;          // [sp+34h] [bp-194h]@54
	int v54;          // [sp+38h] [bp-190h]@54
	int v55;          // [sp+3Ch] [bp-18Ch]@54
	int v56;          // [sp+40h] [bp-188h]@54
	DWORD v57;        // [sp+44h] [bp-184h]@54
	char Dest;        // [sp+48h] [bp-180h]@54
	char v59[383];    // [sp+49h] [bp-17Fh]@54
	v1 = *(char**)(a1 + 4);
	Source = *(char**)(a1 + 4);
	v45 = 0;
	SNetGetNumPlayers(0, 0, (int)&v45);
	if( v45 >= dword_15021340 ){
		v2 = dword_15021158;
		v3 = *(uint*)a1;
		if( !dword_15021158 )
			goto LABEL_76;
		while( 1 ){
			v5 = 16;
			v6 = *(uint*)a1;
			v7 = v2 + 264;
			v4 = 1;
			do{
				if( !v5 )
					break;
				v4 = *(uchar*)v7++ == *(uchar*)v6++;
				--v5;
			}while( v4 );
			if( v4 )
				break;
			v2 = *(uint*)(v2 + 4);
			if( !v2 )
				goto LABEL_76;
		}
		LABEL_19:
		if( v2 )
			return sub_150118B0(v2, 0, 10, 0, 0);
		LABEL_76:
		v2 = sub_15011270((int)&dword_15021158, v3);
		return sub_150118B0(v2, 0, 10, 0, 0);
	}
	v8 = (unsigned int)&v1[strlen(v1)];
	v47 = v8;
	if( byte_150212B8 && stricmp(&byte_150212B8, (char*)(strlen((char*)v8) - 1 + v8 + 1)) ){
		v2 = dword_15021158;
		v3 = *(uint*)a1;
		if( !dword_15021158 )
			goto LABEL_76;
		do{
			v10 = 16;
			v11 = *(uint*)a1;
			v12 = v2 + 264;
			v9 = 1;
			do{
				if( !v10 )
					break;
				v9 = *(uchar*)v12++ == *(uchar*)v11++;
				--v10;
			}while( v9 );
			if( v9 )
				break;
			v2 = *(uint*)(v2 + 4);
		}while( v2 );
		goto LABEL_19;
	}
	v13 = 0;
	LOBYTE_IDA(v44) = 0;
	do{
		if( (unsigned __int8)v44 == 255 ){
			LABEL_30:
			v16 = 0;
		}else{
			v14 = 1;
			while( 1 ){
				v15 = &dword_15021160;
				if( !v14 )
					v15 = &dword_15021158;
				v16 = *v15;
				if( v16 )
					break;
				LABEL_29:
				--v14;
				if( v14 < 0 )
					goto LABEL_30;
			}
			while( *(uchar*)(v16 + 438) != (uchar)v44 ){
				v16 = *(uint*)(v16 + 4);
				if( !v16 )
					goto LABEL_29;
			}
		}
		if( !v16 )
			break;
		++v13;
		LOBYTE_IDA(v44) = v13;
	}while( v13 != -1 );
	if( v13 == -1
	|| (v17 = (unsigned __int8)v44, v46 = (unsigned __int8)v44, (unsigned __int8)v44 >= (unsigned int)dword_15021340) ){
		v2 = dword_15021158;
		v3 = *(uint*)a1;
		if( dword_15021158 ){
			do{
				v41 = 16;
				v42 = *(uint*)a1;
				v43 = v2 + 264;
				v40 = 1;
				do{
					if( !v41 )
						break;
					v40 = *(uchar*)v43++ == *(uchar*)v42++;
					--v41;
				}while( v40 );
				if( v40 )
					break;
				v2 = *(uint*)(v2 + 4);
			}while( v2 );
			if( v2 )
				return sub_150118B0(v2, 0, 10, 0, 0);
		}
		goto LABEL_76;
	}
	v18 = dword_15021160;
	if( dword_15021160 ){
		v45 = dword_15021160;
	}else{
		v49 = 0;
		v50 = 0;
		v51 = 0;
		v52 = 0;
		v45 = sub_15011270((int)&dword_15021160, (int)&v49);
		v18 = v45;
	}
	v19 = dword_15021158;
	if( !dword_15021158 )
		goto LABEL_80;
	do{
		v21 = 16;
		v22 = *(uint*)a1;
		v23 = v19 + 264;
		v20 = 1;
		do{
			if( !v21 )
				break;
			v20 = *(uchar*)v23++ == *(uchar*)v22++;
			--v21;
		}while( v20 );
		if( v20 )
			break;
		v19 = *(uint*)(v19 + 4);
	}while( v19 );
	v17 = v46;
	v8 = v47;
	v24 = v44;
	if( !v19 ){
		LABEL_80:
		v25 = sub_15011270((int)&dword_15021158, *(uint*)a1);
		v24 = v44;
		v19 = v25;
	}
	result = 0;
	if( v19 ){
		if( v18 ){
			*(uint*)(v19 + 280) = 0;
			*(uint*)(v19 + 428) = 0;
			v29 = Source;
			*(uchar*)(v19 + 438) = v24;
			strncpy((char*)(v19 + 8), v29, 0x80u);
			strncpy((char*)(v19 + 136), (char*)v8, 0x80u);
			v27 = *(ushort*)(v18 + 388);
			*(ushort*)(v19 + 394) = v27;
			*(ushort*)(v19 + 402) = *(ushort*)(v18 + 388);
			*(ushort*)(v19 + 388) = *(ushort*)(v18 + 388);
			v28 = *(ushort*)(v18 + 394);
			if( v27 != v28 ){
				if( (unsigned __int16)(v27 - v28) < 0x7FFFu ){
					v30 = *(uint*)(v19 + 280);
					LOBYTE_IDA(v30) = v30 | 4;
					*(uint*)(v19 + 280) = v30;
				}
			}
			v44 = 0;
			SNetGetNumPlayers(0, 0, (int)&v44);
			if( v44 >= dword_15021340 )
				SNetSetGameMode(dword_15021230 | 2);
			v53 = v17;
			v33 = *(ushort*)(v18 + 388);
			v54 = dword_15021340;
			v55 = v33;
			v56 = dword_15021230;
			v57 = (GetTickCount() - dword_150211A0) / 0x3E8;
			strncpy(&Dest, &::Dest, 0x80u);
			v34 = &v59[strlen(&Dest) - 1];
			strncpy(v34, &byte_150211B0, 0x80u);
			strncpy(&v34[strlen(v34)], &byte_150212B8, 0x80u);
			sub_150118B0(v19, 0, 8, &v53, strlen(&byte_150212B8) - 1 + strlen(&::Dest) - 1 + strlen(&byte_150211B0) - 1 + 23);
			v31 = v45;
			v35 = sub_150146D0((int)&v53, v45, v33);
			sub_150118B0(v19, 0, 6, &v53, v35);
			for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
				if( i != v19 ){
					if( *(uchar*)(i + 438) != -1 ){
						v36 = sub_150146D0((int)&v53, i, v33);
						sub_150118B0(v19, 0, 6, &v53, v36);
					}
				}
			}
			result = sub_150118B0(v19, 0, 9, (void*)dword_150211A4, dword_150211A8);
			for( j = dword_15021158; j; j = *(uint*)(j + 4) ){
				if( j != v19 ){
					if( *(uchar*)(j + 438) != -1 ){
						v38 = sub_150146D0((int)&v53, v19, v33);
						result = sub_150118B0(j, 0, 6, &v53, v38);
					}
				}
			}
			for( k = *(uint*)(v31 + 344); k; k = *(uint*)(k + 4) )
				result = sub_15014180(v19, *(void**)(k + 12), *(uint*)(k + 16));
			if( !(*(uchar*)(v19 + 280) & 4) )
				result = (int)sub_15013010(2, v46, 0, 0);
		}
	}
	return result;
}

// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 150211A0: using guessed type int dword_150211A0;
// 150211A4: using guessed type int dword_150211A4;
// 150211A8: using guessed type int dword_150211A8;
// 15021230: using guessed type int dword_15021230;
// 15021340: using guessed type int dword_15021340;
// 15014200: using guessed type char var_17F[383];
//----- (150146D0) --------------------------------------------------------
int __fastcall sub_150146D0(int a1, int a2, int a3)
{
	int v4;          // esi@1
	char* v5;        // ebx@1
	char* v6;        // ebp@1
	unsigned int v7; // kr00_4@1
	unsigned int v8; // kr04_4@1
	int v9;          // edx@1
	v4 = a1;
	v5 = (char*)(a2 + 8);
	v6 = (char*)(a2 + 136);
	v7 = strlen((char*)(a2 + 8));
	v8 = strlen((char*)(a2 + 136));
	v9 = a2 + 264;
	*(uint*)v4 = v7 - 1 + v8 - 1 + 38;
	*(uint*)(v4 + 4) = *(uchar*)(v9 + 174);
	*(uint*)(v4 + 8) = *(uint*)(v9 + 160);
	*(uint*)(v4 + 12) = *(uint*)(v9 + 16);
	*(uint*)(v4 + 16) = a3;
	*(uint*)(v4 + 20) = *(uint*)v9;
	*(uint*)(v4 + 24) = *(uint*)(v9 + 4);
	*(uint*)(v4 + 28) = *(uint*)(v9 + 8);
	*(uint*)(v4 + 32) = *(uint*)(v9 + 12);
	strncpy((char*)(v4 + 36), v5, 0x80u);
	strncpy((char*)(strlen((char*)(v4 + 36)) - 1 + v4 + 37), v6, 0x80u);
	return *(uint*)v4;
}

//----- (15014790) --------------------------------------------------------
char* __stdcall sub_15014790(int a1)
{
	int v1;   // eax@1
	int v2;   // esi@1
	DWORD v4; // ecx@4
	int v5;   // eax@4
	char* v6; // esi@4
	char* v7; // esi@4
	int v8;   // [sp+8h] [bp-10h]@2
	int v9;   // [sp+Ch] [bp-Ch]@2
	int v10;  // [sp+10h] [bp-8h]@2
	int v11;  // [sp+14h] [bp-4h]@2
	v2 = *(uint*)(a1 + 4);
	v1 = dword_15021160;
	if( dword_15021160 || (v8 = 0, v9 = 0, v10 = 0, v11 = 0, (v1 = sub_15011270((int)&dword_15021160, (int)&v8)) != 0) ){
		*(uchar*)(v1 + 438) = *(uchar*)v2;
		*(ushort*)(v1 + 394) = *(ushort*)(v2 + 8);
		*(ushort*)(v1 + 402) = *(ushort*)(v2 + 8);
		*(ushort*)(v1 + 388) = *(ushort*)(v2 + 8);
		dword_1501E1E8 = *(uint*)(v2 + 8);
	}
	dword_15021340 = *(uint*)(v2 + 4);
	dword_15021230 = *(uint*)(v2 + 12);
	v4 = GetTickCount();
	v5 = *(uint*)(v2 + 16);
	v6 = (char*)(v2 + 20);
	dword_150211A0 = v4 - 1000 * v5;
	strncpy(&Dest, v6, 0x80u);
	v7 = &v6[strlen(v6)];
	strncpy(&byte_150211B0, v7, 0x80u);
	return strncpy(&byte_150212B8, &v7[strlen(v7)], 0x80u);
}

// 1501E1E8: using guessed type int dword_1501E1E8;
// 15021160: using guessed type int dword_15021160;
// 150211A0: using guessed type int dword_150211A0;
// 15021230: using guessed type int dword_15021230;
// 15021340: using guessed type int dword_15021340;
//----- (15014890) --------------------------------------------------------
void* __stdcall sub_15014890(int a1)
{
	int v1;       // eax@1
	void* result; // eax@4
	void* v3;     // eax@8
	int v4;       // [sp+8h] [bp-10h]@2
	int v5;       // [sp+Ch] [bp-Ch]@2
	int v6;       // [sp+10h] [bp-8h]@2
	int v7;       // [sp+14h] [bp-4h]@2
	v1 = dword_15021160;
	if( dword_15021160 || (v4 = 0, v5 = 0, v6 = 0, v7 = 0, (v1 = sub_15011270((int)&dword_15021160, (int)&v4)) != 0) )
		LOBYTE_IDA(dword_1501E2EC) = *(uchar*)(v1 + 438);
	result = (void*)dword_150211A4;
	if( dword_150211A4 ){
		result = (void*)SMemFree((void*)dword_150211A4, "SNET.CPP", 2042, 0);
		dword_150211A4 = 0;
	}
	dword_150211A8 = 0;
	if( *(uint*)(a1 + 4) ){
		result = *(void**)(a1 + 8);
		if( result ){
			v3 = (void*)SMemAlloc((uint)result, "SNET.CPP", 2047, 0);
			dword_150211A4 = (int)v3;
			dword_150211A8 = *(uint*)(a1 + 8);
			memcpy(v3, *(void**)(a1 + 4), dword_150211A8);
			result = sub_15013010(1, (unsigned __int8)dword_1501E2EC, (void*)dword_150211A4, dword_150211A8);
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021160: using guessed type int dword_15021160;
// 150211A4: using guessed type int dword_150211A4;
// 150211A8: using guessed type int dword_150211A8;
//----- (15014970) --------------------------------------------------------
void __stdcall nullsub_1(int a1)
{
}

//----- (15014980) --------------------------------------------------------
int __stdcall sub_15014980(int a1)
{
	int result; // eax@1
	int v2;     // ebx@1
	char v3;    // zf@2
	int v4;     // ecx@2
	int v5;     // edi@2
	int v6;     // esi@2
	v2 = *(uint*)(a1 + 4);
	result = dword_15021158;
	if( dword_15021158 ){
		do{
			v4 = 16;
			v5 = *(uint*)a1;
			v6 = result + 264;
			v3 = 1;
			do{
				if( !v4 )
					break;
				v3 = *(uchar*)v6++ == *(uchar*)v5++;
				--v4;
			}while( v3 );
			if( v3 )
				break;
			result = *(uint*)(result + 4);
		}while( result );
		if( result )
			goto LABEL_14;
	}
	result = sub_15011270((int)&dword_15021158, *(uint*)a1);
	if( result ){
		LABEL_14:
		if( *(uchar*)(result + 438) != -1 ){
			*(uint*)(result + 280) |= 8u;
			*(ushort*)(result + 436) = *(ushort*)v2;
			*(uint*)(result + 432) = *(uint*)(v2 + 4);
		}
	}
	return result;
}

// 15014980: using guessed type int __stdcall sub_15014980(int);
// 15021158: using guessed type int dword_15021158;
//----- (15014A00) --------------------------------------------------------
int __thiscall sub_15014A00(DWORD This)
{
	DWORD v1;       // esi@1
	DWORD ThreadId; // [sp+0h] [bp-4h]@1
	ThreadId = This;
	v1 = This;
	if( !dword_15021344 )
		dword_15021344 = CreateEventA(0, 0, 0, 0);
	if( !dword_1502134C ){
		dword_15021348 = 0;
		dword_1502134C = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)sub_15014A90, 0, 0, &ThreadId);
		SetThreadPriority(dword_1502134C, 2);
	}
	if( v1 )
		*(uint *)v1 = (uint)dword_15021344;
	return dword_15021344 && dword_1502134C;
}

// 15021348: using guessed type int dword_15021348;
//----- (15014A90) --------------------------------------------------------
int __stdcall sub_15014A90(int a1)
{
	int v1; // esi@1
	int v2; // esi@2
	v1 = -1;
	if( !dword_15021348 ){
		while( 1 ){
			v2 = WaitForSingleObject(dword_15021344, v1) == 0;
			if( dword_15021348 )
				break;
			EnterCriticalSection(&NetCritical);
			if( !dword_15021184 ){
				LeaveCriticalSection(&NetCritical);
				return 0;
			}
			if( v2 ){
				sub_15014E50();
				sub_15014F00();
			}
			v1 = sub_15014B20();
			LeaveCriticalSection(&NetCritical);
			if( dword_15021348 )
				return 0;
		}
	}
	return 0;
}

// 15021184: using guessed type int dword_15021184;
// 15021348: using guessed type int dword_15021348;
//----- (15014B20) --------------------------------------------------------
int __stdcall sub_15014B20()
{
	int v0;              // eax@1
	DWORD v1;            // ebp@1
	int v2;              // esi@3
	int v3;              // edi@5
	int v4;              // edi@12
	int v5;              // eax@13
	unsigned __int16 v6; // cx@13
	unsigned __int16 v7; // dx@14
	int v8;              // edx@17
	DWORD v9;            // edx@18
	__int16 v10;         // ax@22
	unsigned __int8 v11; // sf@23
	unsigned __int8 v12; // of@23
	int v13;             // eax@24
	int v14;             // ecx@25
	int v15;             // edx@27
	unsigned int v16;    // edi@27
	DWORD v17;           // eax@28
	void* v18;           // edx@31
	unsigned __int8 v19; // sf@32
	unsigned __int8 v20; // of@32
	int v21;             // ebx@33
	int v22;             // ecx@34
	__int16 v23;         // ax@36
	int v24;             // edx@37
	int v25;             // ecx@38
	char v26;            // cl@41
	DWORD v28;           // [sp+10h] [bp-40h]@1
	int v29;             // [sp+14h] [bp-3Ch]@3
	DWORD v30;           // [sp+18h] [bp-38h]@1
	int v31;             // [sp+1Ch] [bp-34h]@12
	int v32;             // [sp+1Ch] [bp-34h]@24
	int v33;             // [sp+20h] [bp-30h]@12
	int v34;             // [sp+20h] [bp-30h]@24
	int v35;             // [sp+24h] [bp-2Ch]@5
	__int16 v36;         // [sp+28h] [bp-28h]@22
	__int16 v37;         // [sp+2Ah] [bp-26h]@22
	__int16 v38;         // [sp+2Ch] [bp-24h]@22
	__int16 v39;         // [sp+2Eh] [bp-22h]@22
	char v40;            // [sp+30h] [bp-20h]@22
	char v41;            // [sp+31h] [bp-1Fh]@22
	char v42;            // [sp+32h] [bp-1Eh]@22
	char v43;            // [sp+33h] [bp-1Dh]@22
	__int16 v44;         // [sp+34h] [bp-1Ch]@41
	__int16 v45;         // [sp+36h] [bp-1Ah]@41
	__int16 v46;         // [sp+38h] [bp-18h]@41
	__int16 v47;         // [sp+3Ah] [bp-16h]@41
	char v48;            // [sp+3Ch] [bp-14h]@41
	char v49;            // [sp+3Dh] [bp-13h]@41
	char v50;            // [sp+3Eh] [bp-12h]@41
	char v51;            // [sp+3Fh] [bp-11h]@41
	int v52;             // [sp+40h] [bp-10h]@2
	int v53;             // [sp+44h] [bp-Ch]@2
	int v54;             // [sp+48h] [bp-8h]@2
	int v55;             // [sp+4Ch] [bp-4h]@2
	v1 = GetTickCount();
	v0 = dword_15021160;
	v30 = v1;
	v28 = -1;
	if( !dword_15021160 ){
		v52 = dword_15021160;
		v53 = dword_15021160;
		v54 = dword_15021160;
		v55 = dword_15021160;
		v0 = sub_15011270((int)&dword_15021160, (int)&v52);
	}
	v2 = dword_15021158;
	v29 = v0;
	if( dword_15021158 && v0 ){
		while( 1 ){
			v3 = *(uint*)(v2 + 4);
			v35 = *(uint*)(v2 + 4);
			if( v1 - *(uint*)(v2 + 284) >= 0xC350 ){
				sub_15011370((void*)v2);
				goto LABEL_44;
			}
			if( *(uchar*)(v2 + 438) != -1 || *(uint*)(v2 + 428) )
				break;
			LABEL_44:
			v2 = v3;
			if( !v3 )
				return v28;
		}
		if( v1 - *(uint*)(v2 + 292) >= 0x4E20 ){
			if( !*(uint*)(v2 + 428) ){
				*(uint*)(v2 + 292) = v1;
				sub_150118B0(v2, 0, 4, 0, 0);
			}
		}
		v31 = 0;
		v4 = v2 + 398;
		v33 = v2 + 328;
		while( 1 ){
			v6 = *(ushort*)(v4 - 8) - 1;
			v5 = *(uint*)v33;
			if( *(uint*)v33 ){
				while( 1 ){
					v7 = *(ushort*)(*(uint*)(v5 + 12) + 4);
					if( v7 - v6 > 1 )
						break;
					v5 = *(uint*)(v5 + 4);
					v6 = v7;
					if( !v5 ){
						v1 = v30;
						goto LABEL_23;
					}
				}
				v8 = *(uint*)(v5 + 28);
				if( v8 ){
					v1 = v30;
					v9 = dwMilliseconds - v30 + v8;
					if( (int)v9 > 0 ){
						if( v28 >= v9 )
							v28 = v9;
						goto LABEL_23;
					}
				}else{
					v1 = v30;
				}
				*(uint*)(v5 + 28) = v1;
				v10 = *(ushort*)v4;
				v38 = v6 + 1;
				v39 = v10;
				LOBYTE_IDA(v10) = *(uchar*)(v29 + 438);
				v40 = v31;
				v36 = 0;
				v37 = 12;
				v41 = 0;
				v42 = v10;
				v43 = 2;
				v36 = sub_15011B00((int)&v36);
				sub_15011BB0(v2, (int)&v36);
			}
			LABEL_23:
			v4 += 2;
			v12 = __SETO__(v31 + 1, 3);
			v11 = v31++ - 2 < 0;
			v33 += 8;
			if( !(v11 ^ v12) ){
				v13 = 0;
				v32 = 0;
				v34 = v2 + 304;
				do{
					v14 = *(uint*)v34;
					if( *(uint*)v34 ){
						if( v13 != 2 ){
							v16 = *(uint*)(v14 + 16);
							v15 = *(uint*)(v14 + 28);
							if( v15
							&& (v17 = v15
							+ 1000 * (*(uint *)(dword_15021190 + 540) + v16 + 128) / *(uint *)(dword_15021190 + 552)
							+ 2 * dwMilliseconds
							+ 200
							- v1,
							(int)v17 > 0) ){
								if( v28 >= v17 )
									v28 = v15 + 1000 * (*(uint*)(dword_15021190 + 540) + v16 + 128) / *(uint*)(dword_15021190 + 552) + 2 * dwMilliseconds + 200 - v1;
							}else{
								v18 = *(void**)(v14 + 12);
								*(uint*)(v14 + 28) = v1;
								sub_15014180(v2, v18, v16);
							}
						}
					}
					v13 = v32 + 1;
					v20 = __SETO__(v32 + 1, 3);
					v19 = v32++ - 2 < 0;
					v34 += 8;
				}while( v19 ^ v20 );
				v21 = 0;
				do{
					v22 = dword_1501E2D8;
					if( v21 != 2 )
						v22 = dword_1501E2E4;
					v23 = *(ushort*)(v2 + 2 * v21 + 398);
					if( *(ushort*)(v2 + 2*v21 + 404) != v23 ){
						v24 = *(uint*)(v2 + 4 * v21 + 412);
						if( v24 ){
							v25 = v24 + v22 - v1;
							if( v25 <= 0 ){
								v46 = *(ushort*)(v2 + 2 * v21 + 398);
								v47 = v23;
								v44 = 0;
								v45 = 12;
								v48 = v21;
								v26 = *(uchar*)(v29 + 438);
								v49 = 0;
								v50 = v26;
								v51 = 1;
								v44 = sub_15011B00((int)&v44);
								sub_15011BB0(v2, (int)&v44);
							}else{
								if( v28 >= v25 )
									v28 = v25;
							}
						}
					}
					++v21;
				}while( v21 < 3 );
				v3 = v35;
				goto LABEL_44;
			}
		}
	}
	return -1;
}

// 1501E2D8: using guessed type int dword_1501E2D8;
// 1501E2E4: using guessed type int dword_1501E2E4;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021190: using guessed type int dword_15021190;
//----- (15014E50) --------------------------------------------------------
int __stdcall sub_15014E50()
{
	int result; // eax@1
	int v1;     // ecx@2
	int v2;     // edx@4
	void* v3;   // [sp+18h] [bp-Ch]@1
	int v4;     // [sp+1Ch] [bp-8h]@1
	int v5;     // [sp+20h] [bp-4h]@1
	v5 = 0;
	v4 = 0;
	v3 = 0;
	for( result = (*(int (__stdcall **)(int *, int *,  void **))(dword_15021184 + 48))(&v5, &v4, &v3); result; result = (*(int (__stdcall **)(int *, int *,  void **))(dword_15021184 + 48))(&v5, &v4, &v3) ){
		v1 = v5;
		if( !v5 )
			break;
		result = v4;
		if( !v4 )
			break;
		v2 = (int)v3;
		if( !v3 )
			break;
		if( !*(uchar*)v5 ){
			if( !*(uchar*)v4 ){
				sub_15013010(4, -1, v3, strlen((char*)v3));
				v1 = v5;
				result = v4;
				v2 = (int)v3;
			}
		}
		(*(void (__stdcall **)(int, int, int))(dword_15021184 + 16))(v1, result, v2);
		v5 = 0;
		v4 = 0;
		v3 = 0;
	}
	return result;
}

// 15021184: using guessed type int dword_15021184;
//----- (15014F00) --------------------------------------------------------
int __stdcall sub_15014F00()
{
	int result;      // eax@1
	int v1;          // esi@1
	int v2;          // edx@4
	int v3;          // ebx@5
	unsigned int v4; // ebp@6
	int v5;          // edi@7
	int v6;          // eax@8
	int v7;          // ecx@10
	int v8;          // esi@12
	int* v9;         // eax@13
	int v10;         // eax@15
	char v11;        // zf@21
	int v12;         // ecx@21
	int v13;         // edi@21
	int v14;         // esi@21
	int v15;         // eax@34
	__int16 v16;     // ax@36
	int v17;         // ecx@40
	int v18;         // edx@41
	int* v19;        // eax@42
	int v20;         // eax@44
	int v21;         // esi@49
	int i;           // eax@54
	char v23;        // zf@55
	int v24;         // ecx@55
	int v25;         // edi@55
	int v26;         // esi@55
	int j;           // eax@65
	char v28;        // zf@66
	int v29;         // ecx@66
	int v30;         // edi@66
	int v31;         // esi@66
	char v32;        // al@79
	int v33;         // esi@82
	int v34;         // ecx@85
	int v35;         // edx@86
	int* v36;        // eax@87
	int v37;         // eax@89
	int v38;         // eax@118
	int v39;         // eax@123
	int v40;         // ecx@123
	int v41;         // esi@123
	int v42;         // eax@123
	DWORD v43;       // eax@123
	int v44;         // ebp@127
	int v45;         // ecx@132
	int v46;         // eax@136
	int v47;         // edx@142
	int v48;         // edi@143
	int v49;         // edx@148
	int v50;         // edx@154
	int l;           // eax@161
	int v52;         // eax@161
	int v53;         // edx@161
	__int16 v54;     // si@161
	int v55;         // ecx@162
	int v56;         // ecx@162
	int v57;         // eax@164
	int v58;         // esi@167
	int v59;         // edx@168
	int k;           // ecx@168
	int v61;         // ebp@168
	int v62;         // eax@172
	int v63;         // eax@189
	int v64;         // [sp-4h] [bp-40h]@80
	int v65;         // [sp+0h] [bp-3Ch]@80
	int v66;         // [sp+4h] [bp-38h]@80
	int v67;         // [sp+18h] [bp-24h]@7
	int v68;         // [sp+18h] [bp-24h]@11
	int v69;         // [sp+1Ch] [bp-20h]@1
	int v70;         // [sp+20h] [bp-1Ch]@1
	int v71;         // [sp+24h] [bp-18h]@1
	int v72;         // [sp+28h] [bp-14h]@6
	int v73;         // [sp+2Ch] [bp-10h]@8
	int v74;         // [sp+30h] [bp-Ch]@8
	int v75;         // [sp+34h] [bp-8h]@8
	int v76;         // [sp+38h] [bp-4h]@8
	v1 = 0;
	v69 = 0;
	v71 = 0;
	v70 = 0;
	result
	= (*(int (__stdcall **)(int *, int *, int *))(dword_15021184 + 44))(&v69, &v71, &v70);
	if( result ){
		while( 1 ){
			v2 = v69;
			if( v69 == v1 || (v3 = v71, v71 == v1) )
				return result;
			v4 = v70;
			v72 = v71;
			dword_1501E288 += v70;
			if( dword_15021160 == v1 ){
				v73 = 0;
				v74 = 0;
				v75 = 0;
				v76 = 0;
				v6 = sub_15011270((int)&dword_15021160, (int)&v73);
				v2 = v69;
				v4 = v70;
				v5 = v6;
				v67 = v6;
			}else{
				v5 = dword_15021160;
				v67 = dword_15021160;
			}
			if( *(uchar*)(v3 + 11) & 4 ){
				v7 = *(uchar*)(v3 + 10);
				if( v7 == 255 ){
					v68 = v1;
					goto LABEL_31;
				}
				v8 = 1;
				while( 1 ){
					v9 = &dword_15021160;
					if( !v8 )
						v9 = &dword_15021158;
					v10 = *v9;
					if( v10 )
						break;
					LABEL_18:
					--v8;
					if( v8 < 0 ){
						v68 = 0;
						goto LABEL_31;
					}
				}
				while( *(uchar*)(v10 + 438) != (uchar)v7 ){
					v10 = *(uint*)(v10 + 4);
					if( !v10 )
						goto LABEL_18;
				}
			}else{
				v10 = dword_15021158;
				if( dword_15021158 != v1 ){
					do{
						v12 = 16;
						v13 = v2;
						v14 = v10 + 264;
						v11 = 1;
						do{
							if( !v12 )
								break;
							v11 = *(uchar*)v14++ == *(uchar*)v13++;
							--v12;
						}while( v11 );
						if( v11 )
							break;
						v10 = *(uint*)(v10 + 4);
					}while( v10 );
					v3 = v72;
					v5 = v67;
				}
				if( !v10 ){
					v10 = sub_15011270((int)&dword_15021158, v2);
					v2 = v69;
					v4 = v70;
				}
			}
			v68 = v10;
			LABEL_31:
			if( !v68 || !v5 ){
				v66 = v4;
				v65 = v71;
				goto LABEL_197;
			}
			if( v4 < 0xC || (v15 = *(ushort *)(v3 + 2), v4 < v15) || *(uchar *)(v3 + 8) >= 3u )
				goto LABEL_195;
			LOBYTE_IDA(v16) = sub_15011B60(v71, v15);
			if( v16 ){
				v2 = v69;
				v4 = v70;
				LABEL_195:
				v66 = v4;
				v65 = v71;
				LABEL_197:
				v64 = v2;
				LABEL_198:
				(*(void (__fastcall **)(int, int, int, int, int))(dword_15021184 + 12))(dword_15021184, v2, v64, v65, v66);
				goto LABEL_199;
			}
			if( dword_15021338 || *(uchar*)(v3 + 10) == -1 || *(uchar*)(v3 + 11) & 4 ){
				v21 = v68;
			}else{
				v17 = *(uchar*)(v3 + 10);
				if( v17 == 255 ){
					LABEL_48:
					v20 = 0;
				}else{
					v18 = 1;
					while( 1 ){
						v19 = &dword_15021160;
						if( !v18 )
							v19 = &dword_15021158;
						v20 = *v19;
						if( v20 )
							break;
						LABEL_47:
						--v18;
						if( v18 < 0 )
							goto LABEL_48;
					}
					while( *(uchar*)(v20 + 438) != (uchar)v17 ){
						v20 = *(uint*)(v20 + 4);
						if( !v20 )
							goto LABEL_47;
					}
				}
				v21 = v68;
				if( v20 != v68 ){
					(*(void (__stdcall **)(int, int, int))(dword_15021184 + 12))(v69, v71, v70);
					goto LABEL_199;
				}
			}
			if( !*(uchar*)(v3 + 8) && *(uchar*)(v3 + 9) == 1 ){
				for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
					v24 = 16;
					v25 = v69;
					v26 = i + 264;
					v23 = 1;
					do{
						if( !v24 )
							break;
						v23 = *(uchar*)v26++ == *(uchar*)v25++;
						--v24;
					}while( v23 );
					if( v23 )
						break;
				}
				if( !i )
					i = sub_15011270((int)&dword_15021158, v69);
				sub_15011370((void*)i);
				if( *(ushort*)(v3 + 2) == 16 ){
					if( *(uint*)(v3 + 12) == 1 ){
						for( j = dword_15021158; j; j = *(uint*)(j + 4) ){
							v29 = 16;
							v30 = v69;
							v31 = j + 264;
							v28 = 1;
							do{
								if( !v29 )
									break;
								v28 = *(uchar*)v31++ == *(uchar*)v30++;
								--v29;
							}while( v28 );
							if( v28 )
								break;
						}
						if( j || (j = sub_15011270((int)&dword_15021158, v69)) != 0 ){
							*(uint*)(j + 428) = 1;
							*(ushort*)(j + 390) = 1;
							*(ushort*)(j + 398) = 1;
							*(ushort*)(j + 384) = 1;
							v72 = 1;
							sub_150118B0(j, 0, 2, &v72, 4u);
						}
					}
				}
				(*(void (__stdcall **)(int, int, int))(dword_15021184 + 12))(v69, v71, v70);
				goto LABEL_199;
			}
			if( !(*(uchar*)(v3 + 11) & 4) ){
				*(uint*)(v21 + 284) = GetTickCount();
				if( !(*(uchar*)(v3 + 11) & 4) )
					sub_150157B0(v21, *(uchar*)(v3 + 8), *(ushort*)(v3 + 6));
			}
			v32 = *(uchar*)(v3 + 11);
			if( v32 & 1 ){
				v2 = v71;
				v66 = v70;
				v65 = v71;
				v64 = v69;
				goto LABEL_198;
			}
			if( v32 & 2 ){
				v33 = 0;
				if( *(ushort*)(v3 + 2) == 12 || *(uchar*)(v3 + 12) == (uchar)dword_1501E2EC ){
					if( *(uchar*)(v3 + 8) == 2 ){
						v33 = *(uint*)(v5 + 344);
						if( v33 ){
							do{
								if( *(ushort*)(*(uint*)(v33 + 12) + 4) == *(ushort*)(v3 + 4) )
									break;
								v33 = *(uint*)(v33 + 4);
							}while( v33 );
							if( v33 ){
								LABEL_117:
								sub_15014180(v68, *(void**)(v33 + 12), *(uint*)(v33 + 16));
								(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(v69, v71, v70);
								goto LABEL_199;
							}
						}
						v33 = *(uint*)(v5 + 376);
						if( !v33 )
							goto LABEL_122;
						do{
							if( *(ushort*)(*(uint*)(v33 + 12) + 4) == *(ushort*)(v3 + 4) )
								break;
							v33 = *(uint*)(v33 + 4);
						}while( v33 );
					}else{
						v33 = *(uint*)(v68 + 8 * *(uchar*)(v3 + 8) + 304);
						if( !v33 )
							goto LABEL_122;
						do{
							if( *(ushort*)(*(uint*)(v33 + 12) + 4) == *(ushort*)(v3 + 4) )
								break;
							v33 = *(uint*)(v33 + 4);
						}while( v33 );
					}
				}else{
					if( *(uchar*)(v3 + 8) == 2 ){
						v34 = *(uchar*)(v3 + 12);
						if( v34 == 255 ){
							LABEL_93:
							v37 = 0;
						}else{
							v35 = 1;
							while( 1 ){
								v36 = &dword_15021160;
								if( !v35 )
									v36 = &dword_15021158;
								v37 = *v36;
								if( v37 )
									break;
								LABEL_92:
								--v35;
								if( v35 < 0 )
									goto LABEL_93;
							}
							while( *(uchar*)(v37 + 438) != (uchar)v34 ){
								v37 = *(uint*)(v37 + 4);
								if( !v37 )
									goto LABEL_92;
							}
						}
						if( v37 ){
							v33 = *(uint*)(v37 + 344);
							if( !v33 )
								goto LABEL_202;
							do{
								if( *(ushort*)(*(uint*)(v33 + 12) + 4) == *(ushort*)(v3 + 4) )
									break;
								v33 = *(uint*)(v33 + 4);
							}while( v33 );
							if( !v33 ){
								LABEL_202:
								v33 = *(uint*)(v37 + 376);
								if( v33 ){
									do{
										if( *(ushort*)(*(uint*)(v33 + 12) + 4) == *(ushort*)(v3 + 4) )
											break;
										v33 = *(uint*)(v33 + 4);
									}while( v33 );
									goto LABEL_102;
								}
								LABEL_122:
								(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(v69, v71, v70);
								goto LABEL_199;
							}
						}else{
							LABEL_102:
							if( !v33 )
								goto LABEL_122;
						}
						** (ushort**)(v33 + 12) = 0;
						*(uchar*)(*(uint*)(v33 + 12) + 11) |= 4u;
						** (ushort**)(v33 + 12) = sub_15011B00(*(uint*)(v33 + 12));
					}
				}
				if( v33 )
					goto LABEL_117;
				goto LABEL_122;
			}
			v38 = *(uchar*)(v3 + 8);
			if( *(ushort*)(v21 + 2*v38 + 390) != *(ushort*)(v3 + 4) && (unsigned __int16)(*(ushort*)(v21 + 2*v38 + 390) - *(ushort*)(v3 + 4)) < 0x7FFFu ){
				if( *(uchar*)(v3 + 8) != 2 ){
					*(ushort*)(v21 + 2 * v38 + 404) = *(ushort*)(v21 + 2 * v38 + 398) - 1;
					*(uint*)(v21 + 4 * *(uchar*)(v3 + 8) + 412) = GetTickCount();
				}
				goto LABEL_122;
			}
			v42 = SMemAlloc(32, ".?AUMESSAGE@@", -2, 8);
			v41 = v42;
			*(uint*)(v42 + 8) = v69;
			*(uint*)(v42 + 12) = v71;
			*(uint*)(v42 + 16) = v70;
			v43 = GetTickCount();
			*(uint*)(v41 + 24) = v43;
			*(uint*)(v41 + 28) = v43 + dword_1501E2E4 - dwMilliseconds;
			dword_1501E268 += v70 - 12;
			v40 = v68 + 8 * *(uchar*)(v3 + 8) + 328;
			v39 = *(uint*)v40;
			if( *(uint*)v40 ){
				while( *(ushort*)(v3 + 4) != *(ushort*)(*(uint*)(v39 + 12) + 4) && (unsigned __int16)(*(ushort*)(v3 + 4) - *(ushort*)(*(uint*)(v39 + 12) + 4)) < 0x7FFFu ){
					v39 = *(uint*)(v39 + 4);
					if( !v39 ){
						v44 = v68;
						v3 = v72;
						goto LABEL_130;
					}
				}
				v3 = v72;
			}
			v44 = v68;
			LABEL_130:
			if( v39 ){
				if( *(ushort*)(v3 + 4) == *(ushort*)(*(uint*)(v39 + 12) + 4) ){
					(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(v69, v71, v70);
					v45 = v68 + 8 * *(uchar*)(v3 + 8) + 328;
					if( *(uint*)v41 ){
						*(uint*)(*(uint*)v41 + 4) = *(uint*)(v41 + 4);
					}else{
						if( *(uint*)v45 == v41 )
							*(uint *)v45 = *(uint *)(v41 + 4);
					}
					v46 = *(uint*)(v41 + 4);
					if( v46 ){
						*(uint*)v46 = *(uint*)v41;
					}else{
						if( *(uint*)(v45 + 4) == v41 )
							*(uint *)(v45 + 4) = *(uint *)v41;
					}
					*(uint*)v41 = 0;
					*(uint*)(v41 + 4) = 0;
					SMemFree((void*)v41, ".?AUMESSAGE@@", -2, 0);
					goto LABEL_199;
				}
				v44 = v68;
			}
			v47 = *(uint*)v41;
			if( *(uint*)v41 )
				goto LABEL_203;
			v48 = *(uint*)(v41 + 4);
			if( !v48 )
				goto LABEL_153;
			if( v47 ){
				LABEL_203:
				v48 = *(uint*)(v41 + 4);
				*(uint*)(v47 + 4) = v48;
			}else{
				if( *(uint*)v40 == v41 )
					*(uint *)v40 = *(uint *)(v41 + 4);
			}
			v49 = *(uint*)(v41 + 4);
			if( v49 ){
				v48 = *(uint*)v41;
				*(uint*)v49 = *(uint*)v41;
			}else{
				if( *(uint*)(v40 + 4) == v41 )
					*(uint *)(v40 + 4) = *(uint *)v41;
			}
			*(uint*)v41 = 0;
			*(uint*)(v41 + 4) = 0;
			LABEL_153:
			if( !v39 ){
				*(uint*)v41 = *(uint*)(v40 + 4);
				if( *(uint*)v40 ){
					*(uint*)(*(uint*)(v40 + 4) + 4) = v41;
					*(uint*)(v40 + 4) = v41;
					goto LABEL_160;
				}
				*(uint*)(v40 + 4) = v41;
				LABEL_158:
				*(uint*)v40 = v41;
				goto LABEL_160;
			}
			v50 = *(uint*)v39;
			*(uint*)(v41 + 4) = v39;
			*(uint*)v41 = v50;
			*(uint*)v39 = v41;
			if( !*(uint*)v41 )
				goto LABEL_158;
			*(uint*)(*(uint*)v41 + 4) = v41;
			LABEL_160:
			if( *(uchar*)(v3 + 8) == 2 ){
				LOWORD_IDA(v48) = *(ushort*)(v44 + 402);
				v58 = dword_15021158;
				do{
					v59 = 0;
					v61 = 1;
					for( k = dword_15021158; k; k = *(uint*)(k + 4) ){
						if( *(uchar*)(k + 438) != -1 ){
							if( !(*(uchar *)(k + 280) & 4) || (v3 = v72, (unsigned __int16)v48 - *(ushort *)(k + 394) >= 0) ){
								v62 = *(uint*)(k + 344);
								if( !v62 )
									goto LABEL_204;
								do{
									if( *(ushort*)(*(uint*)(v62 + 12) + 4) == (ushort)v48 )
										break;
									v62 = *(uint*)(v62 + 4);
								}while( v62 );
								v3 = v72;
								if( v62 )
									v59 = 1;
								else
									LABEL_204:
									v61 = 0;
							}
						}
					}
					if( !v59 )
						break;
					if( v61 )
						++v48;
				}while( v59 && v61 );
				if( dword_15021158 ){
					do{
						if( (ushort)v48 != *(ushort*)(v58 + 402) ){
							if( *(uchar*)(v58 + 438) != -1 ){
								if( !(*(uchar*)(v58 + 280) & 4) || (unsigned __int16)v48 - *(ushort*)(v58 + 394) >= 0 ){
									v63 = *(uint*)(v58 + 420);
									*(ushort*)(v58 + 402) = v48;
									if( !v63 )
										*(uint *)(v58 + 420) = GetTickCount();
								}
							}
						}
						v58 = *(uint*)(v58 + 4);
					}while( v58 );
				}
			}else{
				v52 = *(uchar*)(v3 + 8);
				v54 = *(ushort*)(v44 + 398 + 2 * v52);
				*(ushort*)(v44 + 398 + 2 * v52) = *(ushort*)(v44 + 390 + 2 * v52);
				v53 = *(uchar*)(v3 + 8);
				for( l = *(uint*)(v44 + 328 + 8 * v53); l; l = *(uint*)(l + 4) ){
					v56 = *(uchar*)(v3 + 8);
					LOWORD_IDA(v53) = *(ushort*)(v44 + 398 + 2 * v56);
					v55 = v44 + 398 + 2 * v56;
					if( *(ushort*)(*(uint*)(l + 12) + 4) != (ushort)v53 )
						break;
					++v53;
					*(ushort*)v55 = v53;
				}
				v57 = *(uchar*)(v3 + 8);
				if( *(ushort*)(v44 + 398 + 2*v57) != v54 ){
					if( !*(uint*)(v68 + 4 * v57 + 412) )
						*(uint *)(v68 + 4 * *(uchar *)(v3 + 8) + 412) = GetTickCount();
				}
			}
			if( !*(uchar*)(v3 + 8) )
				sub_15010EB0(v68);
			LABEL_199:
			v69 = 0;
			v71 = 0;
			v70 = 0;
			result
			= (*(int (__stdcall **)(int *, int *, int *))(dword_15021184 + 44))(&v69, &v71, &v70);
			if( !result )
				return result;
			v1 = 0;
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E268: using guessed type int dword_1501E268;
// 1501E288: using guessed type int dword_1501E288;
// 1501E2E4: using guessed type int dword_1501E2E4;
// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
// 15021338: using guessed type int dword_15021338;
//----- (150157B0) --------------------------------------------------------
__int16 __fastcall sub_150157B0 (int a1, unsigned __int8 a2, __int16 a3)
{
	DWORD v3;            // eax@1
	int v4;              // ebx@1
	int v5;              // edi@1
	void* v6;            // esi@1
	int v7;              // ecx@2
	int v8;              // eax@11
	void* v9;            // edi@20
	unsigned __int8 v11; // [sp+10h] [bp-8h]@1
	int v12;             // [sp+14h] [bp-4h]@1
	v11 = a2;
	LOWORD_IDA(v3) = a2;
	v6 = *(void**)(a1 + 8 * a2 + 304);
	v5 = a1 + 8 * a2 + 304;
	v4 = 0;
	v12 = a1;
	if( v6 ){
		do{
			v7 = *((uint*)v6 + 3);
			LOWORD_IDA(v3) = a3 - *(ushort*)(v7 + 4);
			if( a3 == *(ushort*)(v7 + 4) )
				break;
			if( (ushort)v3 > 0x7FFFu )
				break;
			v4 = 1;
			if( *((uint*)v6 + 5) )
				sub_150114E0(*((void**)v6 + 2), (void*)v7, *((uint*)v6 + 4));
			else
				(*(void (__stdcall **)(uint, uint, uint))(dword_15021184 + 12))(
				*((uint *)v6 + 2),
				*((uint *)v6 + 3),
				*((uint *)v6 + 4));
			if( *(uint*)v6 ){
				*(uint*)(*(uint*)v6 + 4) = *((uint*)v6 + 1);
			}else{
				if( *(void**)v5 == v6 )
					*(uint *)v5 = *((uint *)v6 + 1);
			}
			v8 = *((uint*)v6 + 1);
			if( v8 ){
				*(uint*)v8 = *(uint*)v6;
			}else{
				if( *(void**)(v5 + 4) == v6 )
					*(uint *)(v5 + 4) = *(uint *)v6;
			}
			*(uint*)v6 = 0;
			*((uint*)v6 + 1) = 0;
			LOWORD_IDA(v3) = SMemFree(v6, ".?AUMESSAGE@@", -2, 0);
			v6 = *(void**)v5;
		}while( *(uint*)v5 );
	}
	if( v11 == 2 ){
		LOWORD_IDA(v3) = a3;
		*(ushort*)(v12 + 396) = a3;
	}
	if( v4 ){
		if( *(uint*)v5 ){
			v3 = GetTickCount();
			v9 = *(void**)v5;
			if( v3 - *((uint*)v9 + 7) < 0x7FFFFFFF )
				*((uint *)v9 + 7) = v3;
		}
	}
	return v3;
}

// 15021184: using guessed type int dword_15021184;
//----- (150158C0) --------------------------------------------------------
int __stdcall SNetJoinGame(int a1, char* a2, char* a3, char* Source, char* a5, int a6)
{
	char* v6;            // edi@3
	int result;          // eax@4
	char* v8;            // esi@7
	int v9;              // ebp@16
	char v10;            // zf@17
	int v11;             // ecx@17
	char* v12;           // edi@17
	int v13;             // esi@17
	int v14;             // eax@26
	int v15;             // esi@26
	int v16;             // ebx@30
	char* v17;           // esi@30
	char v18;            // zf@31
	int v19;             // ecx@31
	char* v20;           // edi@31
	int v21;             // esi@31
	unsigned __int8 v22; // al@41
	int v23;             // edi@45
	DWORD v24;           // [sp-4h] [bp-2D0h]@6
	int v25;             // [sp+10h] [bp-2BCh]@26
	int v26;             // [sp+14h] [bp-2B8h]@39
	int v27;             // [sp+18h] [bp-2B4h]@46
	int v28;             // [sp+1Ch] [bp-2B0h]@46
	char v29;            // [sp+20h] [bp-2ACh]@15
	char v30;            // [sp+2Ch] [bp-2A0h]@17
	int v31;             // [sp+44h] [bp-288h]@45
	char Dest;           // [sp+14Ch] [bp-180h]@30
	char v33[383];       // [sp+14Dh] [bp-17Fh]@30
	if( a6 )
		*(uint *)a6 = dword_15021180 - 1;
	v6 = Source;
	if( !(a6 != 0 ? (Source != 0 ? -(*Source != 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v24 = 1204;
		LABEL_43:
		SErrSetLastError(v24);
		LABEL_44:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v8 = a2;
	if( !a2 )
		v8 = &ZeroByte;
	if( !a3 )
		a3 = &ZeroByte;
	if( !a5 )
		a5 = &ZeroByte;
	if( (uchar)dword_1501E2EC != -1 )
		SNetLeaveGame(2);
	sub_15011340();
	if( !(*(int (__stdcall **)(int, char *,  char *, char *))(dword_15021184 + 20))(a1, v8, a3, &v29) )
		goto LABEL_44;
	v9 = dword_15021158;
	dword_15021338 = 1;
	if( !dword_15021158 )
		goto LABEL_53;
	do{
		v11 = 16;
		v12 = &v30;
		v13 = v9 + 264;
		v10 = 1;
		do{
			if( !v11 )
				break;
			v10 = *(uchar*)v13++ == *v12++;
			--v11;
		}while( v10 );
		if( v10 )
			break;
		v9 = *(uint*)(v9 + 4);
	}while( v9 );
	v6 = Source;
	if( !v9 ){
		LABEL_53:
		v9 = sub_15011270((int)&dword_15021158, (int)&v30);
		if( !v9 ){
			dword_15021338 = 0;
			SNetLeaveGame(1);
			v24 = 8;
			goto LABEL_43;
		}
	}
	*(uint*)(v9 + 428) = 1;
	*(ushort*)(v9 + 390) = 1;
	*(ushort*)(v9 + 398) = 1;
	*(ushort*)(v9 + 384) = 1;
	v25 = 1;
	v14 = sub_150118B0(v9, 0, 1, &v25, 4u);
	v15 = v14;
	if( v14 ){
		sub_15014180(v9, *(void**)(v14 + 12), *(uint*)(v14 + 16));
		sub_15014180(v9, *(void**)(v15 + 12), *(uint*)(v15 + 16));
	}
	sub_150157B0(v9, 0, 1);
	v25 = 2;
	if( !sub_15015DE0(1u, (int)&v25, 0, dword_1501E2DC) ){
		dword_15021338 = 0;
		SNetLeaveGame(1);
		v24 = 1232;
		goto LABEL_43;
	}
	strncpy(&Dest, v6, 0x80u);
	v17 = &v33[strlen(&Dest) - 1];
	strncpy(v17, a5, 0x80u);
	strncpy(&v17[strlen(v17)], a3, 0x80u);
	v16 = dword_15021158;
	if( !dword_15021158 )
		goto LABEL_54;
	do{
		v19 = 16;
		v20 = &v30;
		v21 = v16 + 264;
		v18 = 1;
		do{
			if( !v19 )
				break;
			v18 = *(uchar*)v21++ == *v20++;
			--v19;
		}while( v18 );
		if( v18 )
			break;
		v16 = *(uint*)(v16 + 4);
	}while( v16 );
	if( !v16 )
		LABEL_54:
		v16 = sub_15011270((int)&dword_15021158, (int)&v30);
	sub_150118B0(v16, 0, 7, &Dest, strlen(Source) - 1 + strlen(a3) - 1 + strlen(a5) - 1 + 3);
	v25 = 9;
	v26 = 10;
	if( !sub_15015DE0(2u, (int)&v25, 0, dword_1501E2DC) ){
		*(uint*)(v9 + 428) = 0;
		dword_15021338 = 0;
		SNetLeaveGame(1);
		v24 = 1232;
		goto LABEL_43;
	}
	v22 = dword_1501E2EC;
	if( (uchar)dword_1501E2EC == -1 ){
		*(uint*)(v9 + 428) = 0;
		dword_15021338 = 0;
		SNetLeaveGame(1);
		v24 = -2062548889;
		goto LABEL_43;
	}
	v23 = dword_15021160;
	dword_1502119C = v31;
	if( dword_15021160
	|| (v25 = 0,
	v26 = 0,
	v27 = 0,
	v28 = 0,
	v23 = sub_15011270((int)&dword_15021160, (int)&v25),
	v22 = dword_1501E2EC,
	v23) ){
		*(uchar*)(v23 + 438) = v22;
		strncpy((char*)(v23 + 8), Source, 0x80u);
		strncpy((char*)(v23 + 136), a5, 0x80u);
		v22 = dword_1501E2EC;
	}
	*(uint*)a6 = dword_15021180 + v22;
	LeaveCriticalSection(&NetCritical);
	sub_15015D10();
	EnterCriticalSection(&NetCritical);
	*(uint*)(v9 + 428) = 0;
	dword_15021338 = 0;
	if( (uchar)dword_1501E2EC == -1 ){
		SErrSetLastError(0x85100069u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		result = 0;
	}else{
		LeaveCriticalSection(&NetCritical);
		result = 1;
	}
	return result;
}

// 1501E2DC: using guessed type int dword_1501E2DC;
// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
// 1502119C: using guessed type int dword_1502119C;
// 15021338: using guessed type int dword_15021338;
// 150158C0: using guessed type char var_17F[383];
//----- (15015D10) --------------------------------------------------------
int __stdcall sub_15015D10()
{
	int result; // eax@1
	void* i;    // edx@1
	int v2;     // ecx@2
	int v3;     // eax@3
	int v4;     // esi@3
	int v5;     // ecx@4
	char v6;    // [sp+0h] [bp-18h]@2
	int v7;     // [sp+8h] [bp-10h]@9
	int v8;     // [sp+Ch] [bp-Ch]@9
	void* v9;   // [sp+10h] [bp-8h]@9
	result = dword_15021118;
	for( i = (void*)dword_15021118; dword_15021118; i = (void*)dword_15021118 ){
		memcpy(&v6, (void*)result, 0x18u);
		v2 = *(uint*)result;
		if( *(uint*)result ){
			v4 = *(uint*)(result + 4);
			v3 = result + 4;
			*(uint*)(v2 + 4) = v4;
		}else{
			v5 = *(uint*)(result + 4);
			v3 = result + 4;
			dword_15021118 = v5;
		}
		if( *(uint*)v3 ){
			** (uint**)v3 = *(uint*)i;
		}else{
			if( (void*)dword_1502111C == i )
				dword_1502111C = *(uint*)i;
		}
		*(uint*)i = 0;
		*(uint*)v3 = 0;
		SMemFree(i, ".?AUUSEREVENT@@", -2, 0);
		v8 += dword_15021180;
		SEvtDispatch(1397638484, 1, v7, (int)&v7);
		if( v9 )
			SMemFree(v9, "SNET.CPP", 1719, 0);
		result = dword_15021118;
	}
	return result;
}

// 15021118: using guessed type int dword_15021118;
// 1502111C: using guessed type int dword_1502111C;
// 15021180: using guessed type int dword_15021180;
//----- (15015DE0) --------------------------------------------------------
int __fastcall sub_15015DE0(unsigned int a1, int a2, int a3, DWORD a4)
{
	unsigned int v4; // ebx@1
	int v5;          // edi@1
	unsigned int v6; // esi@4
	unsigned int v7; // edx@5
	int v8;          // ecx@5
	int v10;         // [sp+10h] [bp-8h]@1
	DWORD v11;       // [sp+14h] [bp-4h]@1
	v4 = a1;
	v10 = a2;
	memset(dword_15021120, 0, 0x38u);
	v11 = GetTickCount();
	v5 = 1;
	while( 1 ){
		if( !v5 )
			Sleep(0xAu);
		v5 = 0;
		sub_15014E50();
		sub_15014F00();
		sub_15014B20();
		v6 = 0;
		if( v4 ){
			v8 = v10;
			v7 = v4;
			do{
				if( *(uint*)v8 < 0xEu ){
					if( dword_15021120[*(uint*)v8] )
						++v6;
				}
				v8 += 4;
				--v7;
			}while( v7 );
		}
		if( v6 >= v4 || v6 && !a3 )
			break;
		if( a4 != -1 && GetTickCount() - v11 >= a4 )
			return 0;
	}
	return 1;
}

// 15021120: using guessed type int dword_15021120[];
//----- (15015E90) --------------------------------------------------------
int __stdcall SNetLeaveGame(int a1)
{
	//if( ExternalStorm ) return eSNetLeaveGame(a1);
	int v1;   // eax@3
	int i;    // esi@9
	int v4;   // eax@11
	int v5;   // edi@11
	DWORD v6; // [sp-4h] [bp-24h]@2
	int v7;   // [sp+10h] [bp-10h]@4
	int v8;   // [sp+14h] [bp-Ch]@4
	int v9;   // [sp+18h] [bp-8h]@4
	int v10;  // [sp+1Ch] [bp-4h]@4
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v6 = 1204;
		LABEL_6:
		SErrSetLastError(v6);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v1 = dword_15021160;
	if( !dword_15021160 ){
		v7 = dword_15021160;
		v8 = dword_15021160;
		v9 = dword_15021160;
		v10 = dword_15021160;
		v1 = sub_15011270((int)&dword_15021160, (int)&v7);
		if( !v1 ){
			v6 = 8;
			goto LABEL_6;
		}
	}
	if( *(uint*)(v1 + 424) )
		(*(void(* *)())(dword_15021184 + 68))();
	for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
		if( *(uchar*)(i + 438) != -1 ){
			v7 = *(ushort*)(i + 388);
			v8 = a1;
			v4 = sub_150118B0(i, 0, 11, &v7, 8u);
			v5 = v4;
			if( v4 ){
				sub_15014180(i, *(void**)(v4 + 12), *(uint*)(v4 + 16));
				sub_15014180(i, *(void**)(v5 + 12), *(uint*)(v5 + 16));
			}
		}
	}
	sub_15015FB0();
	LeaveCriticalSection(&NetCritical);
	Sleep(dwMilliseconds);
	return 1;
}

// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (15015FB0) --------------------------------------------------------
void __stdcall sub_15015FB0()
{
	if( dword_150211A4 ){
		SMemFree((void*)dword_150211A4, "SNET.CPP", 1546, 0);
		dword_150211A4 = 0;
		dword_150211A8 = 0;
	}
	Dest = 0;
	byte_150211B0 = 0;
	byte_150212B8 = 0;
	dword_15021230 = 0;
	LOBYTE_IDA(dword_1501E2EC) = -1;
	dword_15021340 = 0;
	sub_15011340();
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 150211A4: using guessed type int dword_150211A4;
// 150211A8: using guessed type int dword_150211A8;
// 15021230: using guessed type int dword_15021230;
// 15021340: using guessed type int dword_15021340;
//----- (15016010) --------------------------------------------------------
int __stdcall SNetPerformUpgrade(int* upgradestatus)
{
	//if( ExternalStorm ) return eSNetPerformUpgrade(upgradestatus);
	CHAR* v2;                                       // esi@5
	CHAR* v3;                                       // esi@8
	char* v4;                                       // esi@9
	unsigned int v5;                                // ebx@13
	int v6;                                         // esi@13
	int v7;                                         // ebp@14
	char* i;                                        // eax@15
	CHAR v9;                                        // dl@15
	char v10;                                       // cl@17
	char* v11;                                      // edx@26
	char v12;                                       // al@27
	void* v13;                                      // ebx@33
	DWORD v14;                                      // edi@33
	HANDLE v15;                                     // esi@34
	int v16;                                        // eax@37
	int v17;                                        // ecx@38
	CHAR* v18;                                      // edi@38
	char v19;                                       // zf@40
	char* v20;                                      // edx@42
	CHAR* v21;                                      // esi@49
	unsigned int v22;                               // kr14_4@51
	CHAR v23;                                       // al@51
	int v24;                                        // [sp+10h] [bp-C88h]@9
	void* v25;                                      // [sp+14h] [bp-C84h]@8
	int v26;                                        // [sp+18h] [bp-C80h]@32
	int v27;                                        // [sp+1Ch] [bp-C7Ch]@12
	int v28;                                        // [sp+20h] [bp-C78h]@13
	char* Str;                                      // [sp+24h] [bp-C74h]@8
	unsigned int v30;                               // [sp+28h] [bp-C70h]@13
	char* v31;                                      // [sp+2Ch] [bp-C6Ch]@28
	unsigned int v32;                               // [sp+30h] [bp-C68h]@10
	DWORD NumberOfBytesWritten;                     // [sp+34h] [bp-C64h]@35
	struct _PROCESS_INFORMATION ProcessInformation; // [sp+38h] [bp-C60h]@50
	struct _STARTUPINFOA StartupInfo;               // [sp+48h] [bp-C50h]@50
	char Str1;                                      // [sp+8Ch] [bp-C0Ch]@15
	char v37;                                       // [sp+8Dh] [bp-C0Bh]@15
	__int16 v38;                                    // [sp+189h] [bp-B0Fh]@15
	char v39;                                       // [sp+18Bh] [bp-B0Dh]@15
	CHAR CurrentDirectory;                          // [sp+18Ch] [bp-B0Ch]@5
	char v41;                                       // [sp+18Dh] [bp-B0Bh]@5
	__int16 v42;                                    // [sp+28Dh] [bp-A0Bh]@5
	char v43;                                       // [sp+28Fh] [bp-A09h]@5
	CHAR FileName;                                  // [sp+290h] [bp-A08h]@25
	char v45;                                       // [sp+291h] [bp-A07h]@25
	__int16 v46;                                    // [sp+391h] [bp-907h]@25
	char v47;                                       // [sp+393h] [bp-905h]@25
	char v48;                                       // [sp+394h] [bp-904h]@25
	char v49;                                       // [sp+395h] [bp-903h]@25
	__int16 v50;                                    // [sp+495h] [bp-803h]@25
	char v51;                                       // [sp+497h] [bp-801h]@25
	BYTE Data;                                      // [sp+498h] [bp-800h]@3
	char v53;                                       // [sp+499h] [bp-7FFh]@3
	__int16 v54;                                    // [sp+895h] [bp-403h]@3
	char v55;                                       // [sp+897h] [bp-401h]@3
	CHAR ApplicationName;                           // [sp+898h] [bp-400h]@8
	char v57;                                       // [sp+899h] [bp-3FFh]@8
	__int16 v58;                                    // [sp+C95h] [bp-3h]@8
	char v59;                                       // [sp+C97h] [bp-1h]@8
	if( !upgradestatus ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	Data = ZeroByte;
	memset(&v53, 0, 0x3FCu);
	v54 = 0;
	v55 = 0;
	if( !SRegLoadData("Patch", "Patches", 2, &Data, 0x3FFu, 0) ){
		*(uint*)upgradestatus = 0;
		LABEL_54:
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	CurrentDirectory = ZeroByte;
	memset(&v41, 0, 0x100u);
	v42 = 0;
	v43 = 0;
	HIBYTE_IDA(v54) = 0;
	v55 = 0;
	GetModuleFileNameA(0, &CurrentDirectory, 0x104u);
	v43 = 0;
	v2 = &CurrentDirectory;
	if( CurrentDirectory ){
		do{
			if( !strchr(v2 + 1, 92) )
				break;
			v2 = strchr(v2 + 1, 92);
		}while( *v2 );
	}
	*v2 = 0;
	ApplicationName = ZeroByte;
	memset(&v57, 0, 0x3FCu);
	v58 = 0;
	v59 = 0;
	v3 = &ApplicationName;
	v25 = &ApplicationName;
	Str = (char*)&Data;
	if( Data ){
		while( 1 ){
			v4 = Str;
			if( !SFileOpenArchive(Str, 0, 0, (MPQ**)&v24) )
				goto LABEL_53;
			SFileAuthenticateArchive(v24, (int)&v32);
			if( v32 && v32 < 5 ){
				SFileCloseArchive((void*)v24);
				*(uint*)upgradestatus = -1;
				goto LABEL_54;
			}
			if( SFileOpenFileEx((HANDLE)v24, "Prepatch.lst", 0, (HANDLE*)&v27) ){
				v5 = SFileGetFileSize((HANDLE)v27, 0);
				v30 = v5;
				v6 = SMemAlloc(v5, "SNET.CPP", 4207, 0);
				v28 = v6;
				if( SFileReadFile((HANDLE)v27, (LPVOID)v6, v5, 0, 0) ){
					v7 = v6;
					if( v5 ){
						do{
							v9 = ZeroByte;
							Str1 = ZeroByte;
							memset(&v37, 0, 0xFCu);
							v38 = 0;
							v39 = 0;
							for( i = &Str1; v7 - v6 < v5; *i = 0 ){
								if( (int)(i - &Str1) >= 255 )
									break;
								v10 = *(uchar*)v7;
								if( !*(uchar*)v7 )
									break;
								if( v10 == 13 )
									break;
								if( v10 == 10 )
									break;
								*i++ = v10;
								++v7;
							}
							for( ; v7 - v6 < v5; ++v7 ){
								if( *(uchar*)v7 != 13 && *(uchar*)v7 != 10 )
									break;
							}
							v48 = v9;
							memset(&v49, 0, 0x100u);
							v50 = 0;
							v51 = 0;
							FileName = v9;
							memset(&v45, 0, 0x100u);
							v46 = 0;
							v47 = 0;
							if( strchr(&Str1, 32) ){
								v11 = strchr(&Str1, 32) + 1;
								if( *v11 == 32 ){
									do
										v12 = (v11++)[1];while( v12 == 32 );
								}
								v31 = strcpy(&v48, v11);
								wsprintfA(&FileName, "%s\\%s", &CurrentDirectory, v11);
								v6 = v28;
							}
							if( !strnicmp(&Str1, "delete ", 7u) )
								DeleteFileA(&FileName);
							if( !strnicmp(&Str1, "extract ", 8u) ){
								if( SFileOpenFileEx((HANDLE)v24, &v48, 0, (HANDLE*)&v26) ){
									v14 = SFileGetFileSize((HANDLE)v26, 0);
									v13 = (void*)SMemAlloc(v14, "SNET.CPP", 4248, 0);
									if( SFileReadFile((HANDLE)v26, (LPVOID)v13, v14, 0, 0) ){
										v15 = CreateFileA(&FileName, 0x40000000u, 3u, 0, 2u, 0x80u, 0);
										if( v15 != INVALID_HANDLE_VALUE ){
											WriteFile(v15, v13, v14, &NumberOfBytesWritten, 0);
											CloseHandle(v15);
										}
									}
									SMemFree((void*)v13, "SNET.CPP", 4263, 0);
									SFileCloseFile((HANDLE)v26);
									v5 = v30;
									v6 = v28;
								}
							}
							v16 = strnicmp(&Str1, "execute ", 8u);
							if( !v16 ){
								v18 = &FileName;
								v17 = -1;
								do{
									if( !v17 )
										break;
									v19 = *v18++ == (uchar)v16;
									--v17;
								}while( !v19 );
								v20 = (char*)v25;
								memcpy(v25, &v18[-~v17], ~v17);
								v6 = v28;
								v25 = (void*)&v20[strlen(v20)];
							}
						}while( v7 - v6 < v5 );
					}
				}
				SMemFree((void*)v6, "SNET.CPP", 4278, 0);
				SFileCloseFile((HANDLE)v27);
				v4 = Str;
			}
			SFileCloseArchive((void*)v24);
			Str = &v4[strlen(v4)];
			if( !*Str ){
				v3 = (CHAR*)v25;
				break;
			}
		}
	}
	if( v3 == &ApplicationName ){
		wsprintfA(v3, "%s\\bnupdate.exe", &CurrentDirectory);
		v25 = &v3[strlen(v3)];
	}
	v21 = &ApplicationName;
	*(uchar*)v25 = 0;
	if( ApplicationName ){
		while( 1 ){
			memset(&StartupInfo, 0, sizeof(StartupInfo));
			StartupInfo.cb = 68;
			if( !CreateProcessA(v21, 0, 0, 0, 0, 0x20u, 0, &CurrentDirectory, &StartupInfo, &ProcessInformation) )
				break;
			CloseHandle(ProcessInformation.hThread);
			CloseHandle(ProcessInformation.hProcess);
			v22 = strlen(v21);
			v23 = v21[v22];
			v21 += v22;
			if( !v23 )
				goto LABEL_55;
		}
		LABEL_53:
		*(uint*)upgradestatus = -1;
		goto LABEL_54;
	}
	LABEL_55:
	LeaveCriticalSection(&NetCritical);
	*(uint*)upgradestatus = 2;
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15016620) --------------------------------------------------------
int __stdcall SNetReceiveMessage(uint* senderplayerid, int* data, int* databytes)
{
	//if( ExternalStorm ) return eSNetReceiveMessage(senderplayerid, data, databytes);
	int result;          // eax@8
	char v4;             // bl@11
	int v5;              // eax@16
	int v6;              // ebp@16
	int v7;              // edi@16
	int v8;              // eax@18
	int i;               // esi@21
	int v10;             // eax@23
	int v11;             // eax@25
	int v12;             // edx@25
	int v13;             // ecx@26
	unsigned __int8 v14; // zf@33
	char v15;            // sf@33
	unsigned __int8 v16; // of@33
	int v17;             // esi@35
	int v18;             // eax@39
	DWORD v19;           // [sp-4h] [bp-2Ch]@10
	int v20;             // [sp+10h] [bp-18h]@16
	DWORD v21;           // [sp+14h] [bp-14h]@16
	int v22;             // [sp+18h] [bp-10h]@19
	int v23;             // [sp+1Ch] [bp-Ch]@19
	int v24;             // [sp+20h] [bp-8h]@19
	int v25;             // [sp+24h] [bp-4h]@19
	if( senderplayerid )
		*(uint *)senderplayerid = dword_15021180 - 1;
	if( data )
		*(uint *)data = 0;
	if( databytes )
		*(uint *)databytes = 0;
	if( !(data != 0 ? -(databytes != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v19 = 1204;
		LABEL_13:
		SErrSetLastError(v19);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v4 = -1;
	if( (uchar)dword_1501E2EC == -1 ){
		v19 = -2062548880;
		goto LABEL_13;
	}
	sub_15012E30();
	LeaveCriticalSection(&NetCritical);
	sub_15015D10();
	EnterCriticalSection(&NetCritical);
	if( (uchar)dword_1501E2EC == -1 ){
		SErrSetLastError(0x85100069u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		result = 0;
	}else{
		v21 = GetTickCount();
		v7 = 0;
		v6 = 0;
		v5 = 0;
		v20 = 0;
		do{
			if( v5 ){
				v8 = dword_15021160;
				if( !dword_15021160 ){
					v22 = 0;
					v23 = 0;
					v24 = 0;
					v25 = 0;
					v8 = sub_15011270((int)&dword_15021160, (int)&v22);
				}
			}else{
				v8 = dword_15021158;
			}
			for( i = v8; i; i = *(uint*)(i + 4) ){
				if( *(uchar*)(i + 438) != v4 ){
					v10 = *(uint*)(i + 336);
					if( v10 ){
						if( *(ushort*)(*(uint*)(v10 + 12) + 4) == *(ushort*)(i + 392) ){
							v12 = *(uint*)(v10 + 24);
							v11 = *(uint*)(v10 + 4);
							if( v11 ){
								do{
									v13 = *(uint*)(v11 + 24);
									if( (unsigned int)(v12 - v13) < 0x7FFFFFFF )
										v12 = v13 - 1;
									v11 = *(uint*)(v11 + 4);
								}while( v11 );
								v4 = -1;
							}
							if( v21 - v12 >= v7 ){
								v7 = v21 - v12;
								v6 = i;
							}
						}
					}
				}
			}
			v5 = v20 + 1;
			v16 = __SETO__(v20 + 1, 1);
			v14 = v20 == 0;
			v15 = v20++ < 0;
		}while( (unsigned __int8)(v15 ^ v16) | v14 );
		if( v6 ){
			v17 = *(uint*)(v6 + 336);
			if( *(uint*)v17 ){
				*(uint*)(*(uint*)v17 + 4) = *(uint*)(v17 + 4);
			}else{
				if( *(uint*)(v6 + 336) == v17 )
					*(uint *)(v6 + 336) = *(uint *)(v17 + 4);
			}
			v18 = *(uint*)(v17 + 4);
			if( v18 ){
				*(uint*)v18 = *(uint*)v17;
			}else{
				if( *(uint*)(v6 + 340) == v17 )
					*(uint *)(v6 + 340) = *(uint *)v17;
			}
			*(uint*)v17 = 0;
			*(uint*)(v17 + 4) = 0;
			++*(ushort*)(v6 + 392);
			sub_15012CD0(v6, 1u, v17);
			if( senderplayerid )
				*(uint *)senderplayerid = dword_15021180 + *(uchar *)(v6 + 438);
			*(uint*)data = *(uint*)(v17 + 12) + 12;
			*(uint*)databytes = *(ushort*)(*(uint*)(v17 + 12) + 2) - 12;
			InterlockedIncrement(&Addend);
			LeaveCriticalSection(&NetCritical);
			result = 1;
		}else{
			LeaveCriticalSection(&NetCritical);
			SErrSetLastError(0x8510006Bu);
			result = 0;
		}
	}
	return result;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
//----- (150168B0) --------------------------------------------------------
int __stdcall SNetReceiveTurns(uint a1, int a2, void* arraydata, void* arraydatabytes, void* arrayplayerstatus)
{
	//if( ExternalStorm ) return eSNetReceiveTurns(a1, a2, arraydata, arraydatabytes, arrayplayerstatus);
	int v5;           // ebx@1
	int result;       // eax@11
	int v7;           // eax@16
	int v8;           // esi@21
	int v9;           // ebp@28
	int v10;          // esi@28
	int v11;          // eax@31
	int v12;          // edx@33
	int* v13;         // eax@34
	int v14;          // ecx@41
	int v15;          // ebx@42
	int v16;          // edx@43
	int v17;          // eax@53
	unsigned int v18; // esi@54
	int v19;          // ebx@55
	DWORD v20;        // ebp@55
	int v21;          // edi@57
	int v22;          // edx@59
	int* v23;         // eax@60
	int v24;          // ecx@69
	int i;            // ebp@72
	int v26;          // esi@72
	unsigned int v27; // ebx@89
	int v28;          // ebp@90
	int v29;          // edi@90
	int v30;          // eax@92
	int v31;          // edx@94
	int* v32;         // eax@95
	int v33;          // esi@104
	int v34;          // ecx@108
	DWORD v35;        // [sp-4h] [bp-40h]@13
	int v36;          // [sp+10h] [bp-2Ch]@18
	DWORD v37;        // [sp+14h] [bp-28h]@28
	int v38;          // [sp+18h] [bp-24h]@115
	int v39;          // [sp+1Ch] [bp-20h]@29
	int v40;          // [sp+1Ch] [bp-20h]@55
	unsigned int v41; // [sp+20h] [bp-1Ch]@54
	int v42;          // [sp+24h] [bp-18h]@55
	int v43;          // [sp+28h] [bp-14h]@28
	int v44;          // [sp+2Ch] [bp-10h]@17
	int v45;          // [sp+30h] [bp-Ch]@17
	int v46;          // [sp+34h] [bp-8h]@17
	int v47;          // [sp+38h] [bp-4h]@17
	int v48;          // [sp+40h] [bp+4h]@16
	int v49;          // [sp+44h] [bp+8h]@28
	v5 = a2;
	if( a2 ){
		if( arraydata )
			memset(arraydata, 0, 4 * a2);
	}
	if( a2 ){
		if( arraydatabytes )
			memset(arraydatabytes, 0, 4 * a2);
	}
	if( a2 ){
		if( arrayplayerstatus )
			memset(arrayplayerstatus, 0, 4 * a2);
	}
	if( !(arrayplayerstatus != 0 ? (arraydatabytes != 0 ? (arraydata != 0 ? -(a2 != 0) : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v35 = 1204;
		LABEL_20:
		SErrSetLastError(v35);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		SErrSetLastError(0x85100070u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v48 = a1 - dword_15021180;
	sub_15012E30();
	v7 = dword_15021160;
	if( !dword_15021160 ){
		v44 = 0;
		v45 = 0;
		v46 = 0;
		v47 = 0;
		v7 = sub_15011270((int)&dword_15021160, (int)&v44);
	}
	v36 = v7;
	if( !v7 ){
		v35 = -2062548880;
		goto LABEL_20;
	}
	v8 = dword_15021158;
	for( LOWORD_IDA(v38) = *(ushort*)(v7 + 394); v8; v8 = *(uint*)(v8 + 4) ){
		if( *(uchar*)(v8 + 280) & 4 ){
			if( *(ushort*)(v8 + 394) == (ushort)v38 ){
				*(uint*)(v8 + 280) &= 0xFFFFFFFBu;
				sub_15013010(2, *(uchar*)(v8 + 438), 0, 0);
			}
		}
	}
	LeaveCriticalSection(&NetCritical);
	sub_15015D10();
	EnterCriticalSection(&NetCritical);
	if( (uchar)dword_1501E2EC == -1 ){
		SErrSetLastError(0x85100069u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	sub_15012AD0(0);
	v9 = v48;
	v37 = GetTickCount();
	v43 = 1;
	v10 = v48;
	v49 = v48 + a2;
	if( v48 < (unsigned int)(v48 + v5) ){
		v39 = 4 * v48;
		do{
			if( v10 == (unsigned __int8)dword_1501E2EC ){
				v11 = v36;
			}else{
				if( (unsigned __int8)v10 == 255 ){
					LABEL_40:
					v11 = 0;
				}else{
					v12 = 1;
					while( 1 ){
						v13 = &dword_15021160;
						if( !v12 )
							v13 = &dword_15021158;
						v11 = *v13;
						if( v11 )
							break;
						LABEL_39:
						--v12;
						if( v12 < 0 )
							goto LABEL_40;
					}
					while( *(uchar*)(v11 + 438) != (uchar)v10 ){
						v11 = *(uint*)(v11 + 4);
						if( !v11 )
							goto LABEL_39;
					}
				}
			}
			v14 = 0;
			if( !v11 || (v15 = *(uint *)(v11 + 280), v15 & 4) )
				goto LABEL_53;
			v16 = *(uint*)(v11 + 344);
			v14 = 65536;
			if( !v16 )
				goto LABEL_47;
			if( *(ushort*)(*(uint*)(v16 + 12) + 4) != (ushort)v38 ){
				v9 = v48;
				LABEL_47:
				v43 = 0;
				goto LABEL_48;
			}
			v9 = v48;
			v14 = 196608;
			LABEL_48:
			if( v10 == (unsigned __int8)dword_1501E2EC || v37 - *(uint*)(v11 + 284) < dword_1501E2E0 || v15 & 8 && v14 & 0x20000 )
				v14 |= 0x40000u;
			LABEL_53:
			v17 = v39;
			++v10;
			v39 += 4;
			*(uint*)((char*)arrayplayerstatus + v17 - 4 * v9) = v14;
		}while( v10 < (unsigned int)v49 );
	}
	v18 = v9;
	v41 = v9;
	if( v9 < (unsigned int)v49 ){
		v19 = 4 * v9;
		v20 = v37;
		v40 = v19;
		v42 = v19;
		do{
			if( *(uint*)((char*)arrayplayerstatus + v19 - v40) & 0x10000 ){
				v21 = 0;
				if( v18 != (unsigned __int8)dword_1501E2EC ){
					if( (unsigned __int8)v18 == 255 ){
						LABEL_66:
						v21 = 0;
					}else{
						v22 = 1;
						while( 1 ){
							v23 = &dword_15021160;
							if( !v22 )
								v23 = &dword_15021158;
							v21 = *v23;
							if( *v23 )
								break;
							LABEL_65:
							--v22;
							if( v22 < 0 )
								goto LABEL_66;
						}
						while( *(uchar*)(v21 + 438) != (uchar)v18 ){
							v21 = *(uint*)(v21 + 4);
							if( !v21 )
								goto LABEL_65;
						}
					}
				}
				if( v21 ){
					if( !(*(uint*)((char*)arrayplayerstatus + v19 - v40) & 0x20000) ){
						v24 = *(uint*)(v21 + 288);
						if( !v24 || v20 - v24 >= dwMilliseconds && v20 - v24 <= 0x7FFFFFFF ){
							v26 = SMemAlloc(13, "SNET.CPP", 4593, 0);
							*(uint*)(v21 + 288) = v37;
							for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
								if( *(uchar*)(i + 438) != -1 ){
									if( *(uchar*)(v21 + 438) != -1 || i == v21 ){
										*(ushort*)v26 = 0;
										*(ushort*)(v26 + 2) = 12;
										*(ushort*)(v26 + 4) = *(ushort*)(v21 + 394);
										*(ushort*)(v26 + 6) = *(ushort*)(i + 402);
										*(uchar*)(v26 + 8) = 2;
										*(uchar*)(v26 + 9) = 0;
										*(uchar*)(v26 + 10) = *(uchar*)(v36 + 438);
										*(uchar*)(v26 + 11) = 2;
										if( i != v21 ){
											*(ushort*)(v26 + 2) = 13;
											*(uchar*)(v26 + 12) = *(uchar*)(v21 + 438);
										}
										*(ushort*)v26 = sub_15011B00(v26);
										sub_15011BB0(i, v26);
									}
								}
							}
							SMemFree((void*)v26, "SNET.CPP", 4618, 0);
							v19 = v42;
							v18 = v41;
							v20 = v37;
						}
					}
					if( !*(uint*)(v21 + 304) ){
						if( v20 - *(uint*)(v21 + 284) >= (unsigned int)dword_1501E2E0 >> 1 ){
							if( v20 - *(uint*)(v21 + 292) >= (unsigned int)dword_1501E2E0 >> 1 ){
								*(uint*)(v21 + 292) = v20;
								sub_150118B0(v21, 0, 4, 0, 0);
							}
						}
					}
				}
			}
			++v18;
			v19 += 4;
			v41 = v18;
			v42 = v19;
		}while( v18 < v49 );
		v9 = v48;
	}
	if( v43 ){
		v27 = v9;
		if( v9 < (unsigned int)v49 ){
			v28 = 4 * v48;
			v29 = 4 * v48;
			do{
				if( v27 == (unsigned __int8)dword_1501E2EC ){
					v30 = v36;
				}else{
					if( (unsigned __int8)v27 == 255 ){
						LABEL_101:
						v30 = 0;
					}else{
						v31 = 1;
						while( 1 ){
							v32 = &dword_15021160;
							if( !v31 )
								v32 = &dword_15021158;
							v30 = *v32;
							if( v30 )
								break;
							LABEL_100:
							--v31;
							if( v31 < 0 )
								goto LABEL_101;
						}
						while( *(uchar*)(v30 + 438) != (uchar)v27 ){
							v30 = *(uint*)(v30 + 4);
							if( !v30 )
								goto LABEL_100;
						}
					}
				}
				if( v30 && *(uint*)((char*)arrayplayerstatus + v29 - v28) & 0x20000 ){
					v33 = *(uint*)(v30 + 344);
					if( *(uint*)v33 ){
						*(uint*)(*(uint*)v33 + 4) = *(uint*)(v33 + 4);
					}else{
						if( *(uint*)(v30 + 344) == v33 )
							*(uint *)(v30 + 344) = *(uint *)(v33 + 4);
					}
					v34 = *(uint*)(v33 + 4);
					if( v34 ){
						*(uint*)v34 = *(uint*)v33;
					}else{
						if( *(uint*)(v30 + 348) == v33 )
							*(uint *)(v30 + 348) = *(uint *)v33;
					}
					*(uint*)v33 = 0;
					*(uint*)(v33 + 4) = 0;
					*(ushort*)(v30 + 394) = v38 + 1;
					sub_15012CD0(v30, 2u, v33);
					*(uint*)((char*)arraydata + v29 - v28) = *(uint*)(v33 + 12) + 12;
					*(uint*)((char*)arraydatabytes + v29 - v28) = *(ushort*)(*(uint*)(v33 + 12) + 2) - 12;
				}else{
					*(uint*)((char*)arraydata + v29 - v28) = 0;
					*(uint*)((char*)arraydatabytes + v29 - v28) = 0;
				}
				++v27;
				v29 += 4;
			}while( v27 < v49 );
		}
		*(ushort*)(v36 + 394) = v38 + 1;
		dword_1501E1E8 = (v38 + 1) & 0xFFFF;
		InterlockedIncrement(&dword_1501E228);
		LeaveCriticalSection(&NetCritical);
		result = 1;
	}else{
		SErrSetLastError(0x8510006Bu);
		LeaveCriticalSection(&NetCritical);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E1E8: using guessed type int dword_1501E1E8;
// 1501E2E0: using guessed type int dword_1501E2E0;
// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
//----- (15016F50) --------------------------------------------------------
int __stdcall SNetRegisterEventHandler(int a1, int a2)
{
	//if( ExternalStorm ) return eSNetRegisterEventHandler(a1, a2);
	int result; // eax@2
	int v3;     // esi@3
	if( a2 ){
		EnterCriticalSection(&NetCritical);
		v3 = SEvtRegisterHandler(1397638484, 1, a1, 0, a2);
		LeaveCriticalSection(&NetCritical);
		if( !v3 )
			SErrSetLastError(8u);
		result = v3;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15016FB0) --------------------------------------------------------
int __stdcall SNetResetLatencyMeasurements()
{
	int i; // eax@1
	EnterCriticalSection(&NetCritical);
	for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
		*(uint*)(i + 296) = 0;
		*(uint*)(i + 300) = 0;
		*(uint*)(i + 292) = 0;
	}
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 15021158: using guessed type int dword_15021158;
//----- (15016FF0) --------------------------------------------------------
int __stdcall SNetSelectGame(int a1, void* a2, void* a3, void* a4, void* a5, int a6)
{
	int (__stdcall *v7)(uint, uint, uint, uint, uint, uint); // edi@8
	DWORD v8;                                                            // [sp-4h] [bp-8Ch]@4
	char v9;                                                             // [sp+8h] [bp-80h]@5
	char v10;                                                            // [sp+14h] [bp-74h]@5
	char v11;                                                            // [sp+28h] [bp-60h]@5
	char v12;                                                            // [sp+54h] [bp-34h]@5
	EnterCriticalSection(&NetCritical);
	if( a6 )
		*(uint *)a6 = 0;
	if( !dword_15021184 ){
		v8 = 1204;
		LABEL_7:
		SErrSetLastError(v8);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( !sub_150135D0(a2, a3, a4, a5, (int)&v11, (int)&v9, (int)&v12, (int)&v10) ){
		v8 = 87;
		goto LABEL_7;
	}
	// в отладке в dword_15021184 + 52 пока только 0 видел, возможно это функция коннекта к серверу
	v7 = *(int (__stdcall **)(uint, uint, uint, uint, uint, uint))(dword_15021184 + 52);
	LeaveCriticalSection(&NetCritical);
	return v7(a1, (uint)&v11, (uint)&v9, (uint)&v12, (uint)&v10, a6);
}

// 15021184: using guessed type int dword_15021184;
//----- (150170C0) --------------------------------------------------------
int __stdcall SNetSelectProvider(LPARAM a1, void* a2, void* a3, void* a4, void* a5, int a6)
{
	int result;    // eax@4
	int v7;        // eax@5
	int v8;        // edi@5
	HWND v9;       // ST0C_4@5
	int v10;       // eax@5
	LPARAM lParam; // [sp+8h] [bp-94h]@5
	char* v12;     // [sp+Ch] [bp-90h]@5
	char* v13;     // [sp+10h] [bp-8Ch]@5
	char* v14;     // [sp+14h] [bp-88h]@5
	char* v15;     // [sp+18h] [bp-84h]@5
	char v16;      // [sp+1Ch] [bp-80h]@3
	char v17;      // [sp+28h] [bp-74h]@3
	char v18;      // [sp+3Ch] [bp-60h]@3
	char v19;      // [sp+68h] [bp-34h]@3
	EnterCriticalSection(&NetCritical);
	if( a6 )
		*(uint *)a6 = 0;
	if( sub_150135D0(a2, a3, a4, a5, (int)&v18, (int)&v16, (int)&v19, (int)&v17) ){
		sub_150120B0();
		lParam = a1;
		v12 = &v18;
		v13 = &v16;
		v14 = &v19;
		v15 = &v17;
		v9 = SDrawGetFrameWindow(0);
		v10 = GetStormDllHandle();
		v7 = SDlgDialogBoxParam((HMODULE)v10, "SELECTPROVIDER_DIALOG", v9, sub_150171F0, (LPARAM)&lParam);
		v8 = v7;
		if( a6 )
			*(uint *)a6 = v7;
		LeaveCriticalSection(&NetCritical);
		result = v8 && v8 != -1;
	}else{
		SErrSetLastError(0x57u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		result = 0;
	}
	return result;
}

//----- (150171F0) --------------------------------------------------------
HGDIOBJ __stdcall sub_150171F0(int hDlg, unsigned int a2, HDC hdc, HWND a4)
{
	int v4;                                               // ebp@1
	int v5;                                               // ebp@8
	int v6;                                               // esi@12
	WPARAM v7;                                            // eax@18
	int v9;                                               // edi@23
	WPARAM v10;                                           // eax@23
	unsigned int v11;                                     // eax@25
	unsigned int v12;                                     // ST24_4@27
	char* v13;                                            // eax@27
	char* v14;                                            // eax@29
	int v15;                                              // eax@36
	int v16;                                              // ebp@36
	WPARAM v17;                                           // eax@36
	int v18;                                              // eax@37
	void* v19;                                            // edx@37
	int v20;                                              // ecx@39
	int (__stdcall *v21)(uint, uint, uint, uint); // ecx@40
	unsigned int v22;                                     // eax@46
	HWND v23;                                             // eax@49
	int v24;                                              // [sp+10h] [bp-254h]@8
	int v25;                                              // [sp+14h] [bp-250h]@9
	int v26;                                              // [sp+18h] [bp-24Ch]@9
	int v27;                                              // [sp+1Ch] [bp-248h]@9
	int v28;                                              // [sp+20h] [bp-244h]@9
	CHAR String[4];                                       // [sp+24h] [bp-240h]@24
	int v30;                                              // [sp+2Ch] [bp-238h]@39
	CHAR lParam;                                          // [sp+64h] [bp-200h]@20
	char v32;                                             // [sp+65h] [bp-1FFh]@20
	__int16 v33;                                          // [sp+161h] [bp-103h]@20
	char v34;                                             // [sp+163h] [bp-101h]@20
	char Str;                                             // [sp+164h] [bp-100h]@28
	char v36;                                             // [sp+165h] [bp-FFh]@36
	__int16 v37;                                          // [sp+261h] [bp-3h]@36
	char v38;                                             // [sp+263h] [bp-1h]@36
	v4 = a2;
	if( a2 > 0x110 ){
		if( a2 != 273 ){
			if( a2 != 1024
			|| (lParam = ZeroByte,
			memset(&v32, 0, 0xFCu),
			v33 = 0,
			v34 = 0,
			v10 = SendDlgItemMessageA((HWND)hDlg, 20865, 392u, 0, 0),
			SendDlgItemMessageA((HWND)hDlg, 20865, 0x189u, v10, (LPARAM)&lParam),
			v9 = sub_15017750((void *)&lParam),
			!v9) )
				return SDlgDefDialogProc(hDlg, v4, hdc, a4);
			GetDlgItemTextA((HWND)hDlg, 20866, String, 63);
			if( strchr(String, 58) ){
				v11 = *(uint*)(*(uint*)(dword_15021358 + 4) + 24);
				if( v11 >= *(uint*)(v9 + 548) )
					v11 = *(uint*)(v9 + 548);
				v12 = v11;
				v13 = strchr(String, 58);
				wsprintfA(v13 + 1, " %u", v12);
			}
			SetDlgItemTextA((HWND)hDlg, 20866, String);
			GetDlgItemTextA((HWND)hDlg, 20867, &Str, 255);
			if( strchr(&Str, 58) ){
				v14 = strchr(&Str, 58);
				wsprintfA(v14 + 1, "\n%s", v9 + 404);
			}
			SetDlgItemTextA((HWND)hDlg, 20867, &Str);
			goto LABEL_31;
		}
		if( (ushort)hdc == 2 ){
			SDlgEndDialog((HWND)hDlg, 0);
			return SDlgDefDialogProc(hDlg, v4, hdc, a4);
		}
		if( (ushort)hdc != 1 ){
			if( (ushort)hdc == 20865 ){
				v22 = (unsigned int)hdc >> 16;
				if( (unsigned __int16)((unsigned int)hdc >> 16) == 1 ){
					PostMessageA((HWND)hDlg, 0x400u, 0, 0);
				}else{
					if( (ushort)v22 == 2 ){
						v23 = GetDlgItem((HWND)hDlg, 1);
						PostMessageA((HWND)hDlg, 0x111u, 1u, (LPARAM)v23);
					}
				}
			}
			return SDlgDefDialogProc(hDlg, v4, hdc, a4);
		}
		Str = ZeroByte;
		memset(&v36, 0, 0xFCu);
		v37 = 0;
		v38 = 0;
		v17 = SendDlgItemMessageA((HWND)hDlg, 20865, 392u, 0, 0);
		SendDlgItemMessageA((HWND)hDlg, 20865, 0x189u, v17, (LPARAM)&Str);
		v15 = sub_15017750(&Str);
		v16 = v15;
		if( v15 ){
			SRegSaveValue("Network Providers", "Preferred Provider", 0, *(uint*)(v15 + 272));
			memset(String, 0, 0x34u);
			v18 = dword_15021358;
			v19 = *(void**)(dword_15021358 + 12);
			if( v19 )
				memcpy(String, v19, 0x34u);
			*(uint*)String = 52;
			v30 = hDlg;
			v20 = *(uint*)(dword_15021358 + 12);
			if( !v20 || (v21 = *(int (__stdcall **)(uint, uint, uint, uint))(v20 + 28), !v21) )
				goto LABEL_43;
			if( v21(*(uint*)(v16 + 272), v16 + 532, (uint)String, *(uint*)(dword_15021358 + 16)) ){
				v18 = dword_15021358;
				LABEL_43:
				if( SNetInitializeProvider((void*)*(uint*)(v16 + 272), *(void**)(v18 + 4), *(void**)(v18 + 8), String, *(void**)(v18 + 16)) )
					SDlgEndDialog((HWND)hDlg, (HANDLE)*(uint*)(v16 + 272));
				goto LABEL_31;
			}
		}
		LABEL_31:
		v4 = a2;
		return SDlgDefDialogProc(hDlg, v4, hdc, a4);
	}
	if( a2 != 272 ){
		if( a2 == 2 ){
			if( dword_15021350 ){
				SMemFree((void*)dword_15021350, "SNET.CPP", 2781, 0);
				dword_15021350 = 0;
			}
			if( dword_15021354 ){
				SMemFree((void*)dword_15021354, "SNET.CPP", 2785, 0);
				dword_15021354 = 0;
			}
		}
		return SDlgDefDialogProc(hDlg, v4, hdc, a4);
	}
	dword_15021358 = (int)a4;
	v5 = 0;
	if( sub_150177C0(
	*(int (__stdcall **)(uint, uint, uint, uint, uint, uint, uint, uint))(*((uint *)a4 + 3) + 12),
	0,
	0,
	1,
	(void **)&dword_15021350,
	(int)&v24) ){
		SDlgSetBitmap(hDlg, 0, &ZeroByte, -1, 1, dword_15021350, 0, v24, v25, -1);
		v26 = 20866;
		v27 = 20867;
		v28 = 0;
		SDlgSetControlBitmaps((HWND)hDlg, &v26, 0, dword_15021350, (int)&v24, 2, -1);
	}
	if( sub_150177C0(
	*(int (__stdcall **)(uint, uint, uint, uint, uint, uint, uint, uint))(*(uint *)(dword_15021358 + 12)
	+ 12),
	0,
	1,
	0,
	(void **)&dword_15021354,
	(int)&v24) ){
		v26 = 1;
		v27 = 2;
		v28 = 0;
		SDlgSetControlBitmaps((HWND)hDlg, &v26, 0, dword_15021354, (int)&v24, 1, -1);
	}
	v6 = dword_15021110;
	v24 = 0;
	SRegLoadValue("Network Providers", "Preferred Provider", 0, &v24);
	for( ; v6; v6 = *(uint*)(v6 + 4) ){
		if( *(uint*)(v6 + 272) ){
			if( sub_150125F0(v6 + 532, *(uint*)dword_15021358) ){
				SendDlgItemMessageA((HWND)hDlg, 20865, 0x180u, 0, v6 + 276);
				if( *(uint*)(v6 + 272) == v24 )
					v5 = v6;
			}
		}
	}
	v7 = 0;
	if( v5 )
		v7 = SendDlgItemMessageA((HWND)hDlg, 20865, 0x1A2u, 0xFFFFFFFFu, v5 + 276);
	SendDlgItemMessageA((HWND)hDlg, 20865, 0x186u, v7, 0);
	lParam = ZeroByte;
	memset(&v32, 0, 0xFCu);
	v33 = 0;
	v34 = 0;
	sub_15013780(*(char**)(*(uint*)(dword_15021358 + 4) + 4), *(void**)(dword_15021358 + 16), (char*)&lParam, 256);
	SetDlgItemTextA((HWND)hDlg, 20868, &lParam);
	PostMessageA((HWND)hDlg, 0x400u, 0, 0);
	return (HGDIOBJ)1;
}

// 15021110: using guessed type int dword_15021110;
// 15021358: using guessed type int dword_15021358;
//----- (15017750) --------------------------------------------------------
int __thiscall sub_15017750(void* This)
{
	void* v1;   // ebp@1
	int v2;     // edi@1
	int result; // eax@4
	v1 = This;
	v2 = dword_15021110;
	if( dword_15021110 ){
		while( strcmp((char*)(v2 + 276), (char*)v1) ){
			v2 = *(uint*)(v2 + 4);
			if( !v2 )
				return 0;
		}
		result = v2;
	}else{
		result = 0;
	}
	return result;
}

// 15021110: using guessed type int dword_15021110;
//----- (150177C0) --------------------------------------------------------
int __fastcall sub_150177C0(int (__stdcall *checkFunct)(uint, uint, uint, uint, uint, uint, uint, uint), int a2, int a3, int a4, void **a5, int a6)
{
	int v6;                                                                              // ebx@1
	int (__stdcall *v7)(uint, uint, uint, uint, uint, uint, uint, uint); // edi@1
	int result;                                                                          // eax@2
	int v9;                                                                              // edx@5
	int v10;                                                                             // ecx@5
	int v11;                                                                             // esi@7
	void* v12;                                                                           // eax@7
	int v13;                                                                             // [sp+30h] [bp-40Ch]@3
	int v14;                                                                             // [sp+34h] [bp-408h]@3
	int v15;                                                                             // [sp+38h] [bp-404h]@3
	char v16;                                                                            // [sp+3Ch] [bp-400h]@7
	char v17;                                                                            // [sp+40h] [bp-3FCh]@10
	v7 = checkFunct;
	*a5 = 0;
	v6 = a2;
	*(uint*)a6 = 0;
	*(uint*)(a6 + 4) = 0;
	if( checkFunct ){
		result = checkFunct(a2, a3, 0, 0, 0, (uint)&v14, (uint)&v13, (uint)&v15);
		if( result ){
			v10 = v13;
			v9 = v14;
			if( a6 ){
				*(uint*)a6 = v14;
				*(uint*)(a6 + 4) = v10;
			}
			v11 = v9 * v10 * v15 / 8;
			v12 = (void*)SMemAlloc(v9 * v10 * v15 / 8, "SNET.CPP", 2688, 0);
			*a5 = v12;
			if( v7(v6, a3, (uint)&v16, (uint)v12, v11, (uint)&v14, (uint)&v13, (uint)&v15) ){
				if( a4 )
					SDrawUpdatePalette(1, 254, (PALETTEENTRY*)&v17, 0);
				result = 1;
			}else{
				SMemFree(*a5, "SNET.CPP", 2700, 0);
				*a5 = 0;
				result = 0;
			}
		}
	}else{
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150178F0) --------------------------------------------------------
int __stdcall SNetSendMessage(int a1, void* a2, unsigned int a3)
{
	//if( ExternalStorm ) return eSNetSendMessage(a1, a2, a3);
	int v4;   // ecx@7
	int v5;   // edx@13
	int* v6;  // eax@14
	int v7;   // eax@16
	int i;    // esi@24
	DWORD v9; // [sp-4h] [bp-24h]@4
	int v10;  // [sp+10h] [bp-10h]@30
	int v11;  // [sp+14h] [bp-Ch]@30
	int v12;  // [sp+18h] [bp-8h]@30
	int v13;  // [sp+1Ch] [bp-4h]@30
	if( !(a2 != 0 ? -(a3 != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v9 = 1204;
		LABEL_23:
		SErrSetLastError(v9);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v9 = -2062548880;
		goto LABEL_23;
	}
	v4 = a1;
	if( a1 == -2 )
		goto LABEL_35;
	if( a1 != -1 ){
		v4 = a1 - dword_15021180;
		a1 -= dword_15021180;
	}
	if( v4 == -2 || v4 == -1 ){
		LABEL_35:
		for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
			if( *(uchar*)(i + 438) != -1 ){
				sub_150118B0(i, 1, 0, a2, a3);
				v4 = a1;
			}
		}
		if( v4 != -1
		|| (v7 = dword_15021160, !dword_15021160)
		&& (v10 = 0, v11 = 0, v12 = 0, v13 = 0, v7 = sub_15011270((int)&dword_15021160, (int)&v10), !v7) )
			goto LABEL_32;
	}else{
		if( v4 == 255 ){
			LABEL_20:
			v7 = 0;
		}else{
			v5 = 1;
			while( 1 ){
				v6 = &dword_15021160;
				if( !v5 )
					v6 = &dword_15021158;
				v7 = *v6;
				if( v7 )
					break;
				LABEL_19:
				--v5;
				if( v5 < 0 )
					goto LABEL_20;
			}
			while( *(uchar*)(v7 + 438) != (uchar)v4 ){
				v7 = *(uint*)(v7 + 4);
				if( !v7 )
					goto LABEL_19;
			}
		}
		if( !v7 ){
			v9 = -2062548886;
			goto LABEL_23;
		}
	}
	sub_150118B0(v7, 1, 0, a2, a3);
	LABEL_32:
	InterlockedIncrement(&dword_1501E238);
	LeaveCriticalSection(&NetCritical);
	SetEvent(dword_15021344);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021180: using guessed type int dword_15021180;
// 15021184: using guessed type int dword_15021184;
//----- (15017A70) --------------------------------------------------------
int __stdcall SNetSendTurn(void* a1, unsigned int a2)
{
	//if( ExternalStorm ) return eSNetSendTurn(a1, a2);
	int v3;   // eax@7
	int v4;   // ebx@9
	int i;    // esi@12
	DWORD v6; // [sp-4h] [bp-24h]@4
	int v7;   // [sp+10h] [bp-10h]@8
	int v8;   // [sp+14h] [bp-Ch]@8
	int v9;   // [sp+18h] [bp-8h]@8
	int v10;  // [sp+1Ch] [bp-4h]@8
	if( !(a1 != 0 ? -(a2 != 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v6 = 1204;
		LABEL_11:
		SErrSetLastError(v6);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		SErrSetLastError(0x85100070u);
		SErrGetLastError();
		LeaveCriticalSection(&NetCritical);
		return 0;
	}
	v3 = dword_15021160;
	if( !dword_15021160 ){
		v7 = 0;
		v8 = 0;
		v9 = 0;
		v10 = 0;
		v3 = sub_15011270((int)&dword_15021160, (int)&v7);
	}
	v4 = v3;
	if( !v3 ){
		v6 = -2062548880;
		goto LABEL_11;
	}
	for( i = dword_15021158; i; i = *(uint*)(i + 4) ){
		if( *(uchar*)(i + 438) != -1 ){
			if( *(ushort*)(i + 388) == *(ushort*)(v4 + 388) )
				sub_150118B0(i, 2, 0, a1, a2);
		}
	}
	sub_150118B0(v4, 2, 0, a1, a2);
	InterlockedIncrement(&dword_1501E218);
	LeaveCriticalSection(&NetCritical);
	SetEvent(dword_15021344);
	return 1;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021158: using guessed type int dword_15021158;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
//----- (15017BC0) --------------------------------------------------------
int __stdcall SNetSetBasePlayer(int a1)
{
	//if( ExternalStorm ) return eSNetSetBasePlayer(a1);
	EnterCriticalSection(&NetCritical);
	dword_15021180 = a1;
	LeaveCriticalSection(&NetCritical);
	return 1;
}

// 15021180: using guessed type int dword_15021180;
//----- (15017BF0) --------------------------------------------------------
int __stdcall SNetSetGameMode(int a1)
{
	int v1;   // eax@5
	DWORD v2; // eax@8
	DWORD v4; // [sp-4h] [bp-14h]@2
	int v5;   // [sp+0h] [bp-10h]@6
	int v6;   // [sp+4h] [bp-Ch]@6
	int v7;   // [sp+8h] [bp-8h]@6
	int v8;   // [sp+Ch] [bp-4h]@6
	EnterCriticalSection(&NetCritical);
	if( !dword_15021184 ){
		v4 = 87;
		LABEL_11:
		SErrSetLastError(v4);
		goto LABEL_12;
	}
	if( (uchar)dword_1501E2EC == -1 ){
		v4 = -2062548880;
		goto LABEL_11;
	}
	v1 = dword_15021160;
	if( !dword_15021160
	&& (v5 = dword_15021160,
	v6 = dword_15021160,
	v7 = dword_15021160,
	v8 = dword_15021160,
	v1 = sub_15011270((int)&dword_15021160, (int)&v5),
	!v1)
	|| !*(uint *)(v1 + 424) ){
		v4 = 288;
		goto LABEL_11;
	}
	dword_15021230 = a1;
	v2 = GetTickCount();
	if( (*(int (__stdcall **)(char *, char *, char *, int, DWORD, int, int))(dword_15021184 + 64))(
	&Dest,
	&byte_150212B8,
	&byte_150211B0,
	dword_15021230,
	(v2 - dword_150211A0) / 0x3E8,
	dword_1502119C,
	dword_1502133C) ){
		LeaveCriticalSection(&NetCritical);
		return 1;
	}
	LABEL_12:
	SErrGetLastError();
	LeaveCriticalSection(&NetCritical);
	return 0;
}

// 1501E2EC: using guessed type int dword_1501E2EC;
// 15021160: using guessed type int dword_15021160;
// 15021184: using guessed type int dword_15021184;
// 1502119C: using guessed type int dword_1502119C;
// 150211A0: using guessed type int dword_150211A0;
// 15021230: using guessed type int dword_15021230;
// 1502133C: using guessed type int dword_1502133C;
//----- (15017CF0) --------------------------------------------------------
int __stdcall SNetUnregisterEventHandler(int a1, int a2)
{
	//if( ExternalStorm ) return eSNetUnregisterEventHandler(a1, a2);
	int result; // eax@2
	int v3;     // esi@3
	if( a2 ){
		EnterCriticalSection(&NetCritical);
		v3 = SEvtUnregisterHandler(1397638484, 1, a1, a2);
		LeaveCriticalSection(&NetCritical);
		if( !v3 )
			SErrSetLastError(0x85100073u);
		result = v3;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15017D50) --------------------------------------------------------
int __stdcall SRegLoadData(char* source, CHAR* valueName, int a3, LPBYTE lpData, int a5, LPDWORD lpcbData)
{
	//if( ExternalStorm ) return eSRegLoadData(source, valueName, a3, lpData, a5, lpcbData);
	int result; // eax@2
	DWORD* v7;  // ecx@3
	if( a5 != 0 ? (lpData != 0 ? (valueName != 0 ? (source != 0 ? (*source != 0 ? -(*valueName != 0) : 0) : 0) : 0) : 0) : 0 ){
		v7 = lpcbData;
		if( !lpcbData )
			v7 = (DWORD*)&valueName;
		result = LoadRegistryValue(source, valueName, a3, (LPDWORD)&source, lpData, a5, v7);
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15017DD0) --------------------------------------------------------
int __fastcall LoadRegistryValue(char* Source, CHAR* a2, char a3, LPDWORD lpType, LPBYTE lpData, DWORD maxSize, LPDWORD lpcbData)
{
	char* v7;           // edx@1
	int v8;             // ebp@1
	int v9;             // ecx@2
	int v10;            // edi@2
	char v11;           // zf@4
	char v12;           // al@6
	unsigned int v13;   // ecx@6
	HKEY v14;           // edi@6
	int v15;            // esi@6
	int v16;            // ecx@7
	int v17;            // edi@7
	char v18;           // zf@9
	unsigned int v19;   // ecx@12
	HKEY hKey;          // [sp+10h] [bp-10Ch]@2
	LPCSTR lpValueName; // [sp+14h] [bp-108h]@1
	char SubKey[260];   // [sp+18h] [bp-104h]@2
	lpValueName = a2;
	v8 = 0;
	v7 = Source;
	*lpcbData = 0;
	if( a3 & 2 ){
		v10 = (int)"Software\\Battle.net\\";
		v9 = -1;
		hKey = (HKEY)SubKey;
		do{
			if( !v9 )
				break;
			v11 = *(uchar*)v10++ == 0;
			--v9;
		}while( !v11 );
		v13 = ~v9;
		v15 = v10 - v13;
		v14 = hKey;
		v12 = v13;
	}else{
		v17 = (int)"Software\\Blizzard Entertainment\\";
		v16 = -1;
		hKey = (HKEY)SubKey;
		do{
			if( !v16 )
				break;
			v18 = *(uchar*)v17++ == 0;
			--v16;
		}while( !v18 );
		v13 = ~v16;
		v15 = v17 - v13;
		v14 = hKey;
		v12 = v13;
	}
	v19 = v13 >> 2;
	memcpy(v14, (void*)v15, 4 * v19);
	memcpy(v14 + v19, (void*)(v15 + 4 * v19), v12 & 3);
	strncat(SubKey, v7, 259 - (strlen(SubKey) - 1));
	if( !RegOpenKeyExA((HKEY)(-((a3 & 1) != 0) - 2147483646), SubKey, 0, 0x20019u, &hKey) ){
		*lpcbData = maxSize;
		v8 = RegQueryValueExA(hKey, lpValueName, 0, lpType, lpData, lpcbData) == 0;
		RegCloseKey(hKey);
	}
	return v8;
}

//----- (15017EF0) --------------------------------------------------------
int __stdcall SRegLoadString(char* Source, CHAR* valueName, int a3, LPSTR buff, int maxsize)
{
	//if( ExternalStorm ) return eSRegLoadString(Source, valueName, a3, buff, maxsize);
	BYTE* v5;   // ebx@1
	DWORD v6;   // edi@1
	int result; // eax@2
	CHAR* v8;   // eax@8
	v5 = (BYTE*)buff;
	v6 = maxsize;
	if( !(maxsize != 0 ? (buff != 0 ? (valueName != 0 ? (Source != 0 ? (*valueName != 0 ? -(*Source != 0) : 0) : 0) : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	result = LoadRegistryValue(Source, valueName, a3, (LPDWORD)&Source, (LPBYTE)buff, maxsize, (LPDWORD)&valueName);
	if( result ){
		if( Source == (char*)1 ){
			v8 = (CHAR*)(v6 - 1);
			if( v6 - 1 >= (unsigned int)valueName )
				v8 = valueName;
			*(uchar*)((uint)v5 + (uint)v8) = 0;
		}else{
			if( Source == (char*)4 ){
				wsprintfA((LPSTR)v5, "%u", *(uint*)v5);
				return 1;
			}
		}
		result = 1;
	}
	return result;
}

//----- (15017FB0) --------------------------------------------------------
int __stdcall SRegLoadValue(char* key, char* value, int flags, int* data)
{
	//if( ExternalStorm ) return eSRegLoadValue(key, value, flags, data);
	int result;     // eax@2
	DWORD Type;     // [sp+Ch] [bp-108h]@3
	DWORD cbData;   // [sp+10h] [bp-104h]@3
	char Data[256]; // [sp+14h] [bp-100h]@3
	if( !(value != 0 ? (key != 0 ? (data != 0 ? (*value != 0 ? -(*key != 0) : 0) : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	Data[0] = ZeroByte;
	memset(&Data[1], 0, 255);
	result = LoadRegistryValue(key, value, flags, &Type, (LPBYTE)Data, 256, &cbData);
	if( result ){
		if( Type == 1 ){
			*(uint*)data = atoi(Data);
		}else{
			if( Type == 4 ){
				result = 1;
				*(uint*)data = *(uint*)Data;
				return result;
			}
		}
		result = 1;
	}
	return result;
}

//----- (150180A0) --------------------------------------------------------
int __stdcall SRegSaveData(char* Source, CHAR* valueName, int a3, BYTE* lpData, DWORD cbData)
{
	//if( ExternalStorm ) return eSRegSaveData(Source, valueName, a3, lpData, cbData);
	int result; // eax@2
	BYTE* v6;   // eax@3
	if( valueName != 0 ? (Source != 0 ? (*valueName != 0 ? -(*Source != 0) : 0) : 0) : 0 ){
		v6 = lpData;
		if( !lpData )
			v6 = (BYTE*)&ZeroByte;
		result = SaveRegistryValue(Source, valueName, a3, ((unsigned __int8)(a3 & 0x80) | 0x60u) >> 5, v6, cbData);
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15018110) --------------------------------------------------------
int __fastcall SaveRegistryValue(char* key, CHAR* valueName, char flag, DWORD dwType, BYTE* lpData, DWORD cbData)
{
	char* v6;            // edx@1
	int v7;              // ebp@1
	int v8;              // ecx@2
	int v9;              // edi@2
	char v10;            // zf@4
	char v11;            // al@6
	unsigned int v12;    // ecx@6
	HKEY v13;            // edi@6
	int v14;             // esi@6
	int v15;             // ecx@7
	int v16;             // edi@7
	char v17;            // zf@9
	unsigned int v18;    // ecx@12
	HKEY hKey;           // [sp+10h] [bp-110h]@2
	LPCSTR lpValueName;  // [sp+14h] [bp-10Ch]@1
	DWORD dwDisposition; // [sp+18h] [bp-108h]@12
	char SubKey[260];    // [sp+1Ch] [bp-104h]@2
	lpValueName = valueName;
	v7 = 0;
	v6 = key;
	if( flag & 2 ){
		v9 = (int)"Software\\Battle.net\\";
		v8 = -1;
		hKey = (HKEY)SubKey;
		do{
			if( !v8 )
				break;
			v10 = *(uchar*)v9++ == 0;
			--v8;
		}while( !v10 );
		v12 = ~v8;
		v14 = v9 - v12;
		v13 = hKey;
		v11 = v12;
	}else{
		v16 = (int)"Software\\Blizzard Entertainment\\";
		v15 = -1;
		hKey = (HKEY)SubKey;
		do{
			if( !v15 )
				break;
			v17 = *(uchar*)v16++ == 0;
			--v15;
		}while( !v17 );
		v12 = ~v15;
		v14 = v16 - v12;
		v13 = hKey;
		v11 = v12;
	}
	v18 = v12 >> 2;
	memcpy(v13, (void*)v14, 4 * v18);
	memcpy(v13 + v18, (void*)(v14 + 4 * v18), v11 & 3);
	strncat(SubKey, v6, 259 - (strlen(SubKey) - 1));
	if( !RegCreateKeyExA((HKEY)(-((flag & 1) != 0) - 0x7FFFFFFE), SubKey, 0, 0, 0, 0x20006u, 0, &hKey, &dwDisposition) ){
		v7 = RegSetValueExA(hKey, lpValueName, 0, dwType, lpData, cbData) == 0;
		if( flag & 8 )
			RegFlushKey(hKey);
		RegCloseKey(hKey);
	}
	return v7;
}

//----- (15018230) --------------------------------------------------------
int __stdcall SRegSaveString(char* Source, LPCSTR lpValueName, int a3, BYTE* lpData)
{
	int result; // eax@2
	if( lpData != 0 ? (lpValueName != 0 ? (Source != 0 ? (*Source != 0 ? -(*lpValueName != 0) : 0) : 0) : 0) : 0 ){
		result = SaveRegistryValue(Source, (char*)lpValueName, a3, 1u, lpData, strlen((char*)lpData));
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (150182A0) --------------------------------------------------------
int __stdcall SRegSaveValue(char* Source, CHAR* valueName, int flag, int Data)
{
	//if( ExternalStorm ) return eSRegSaveValue(Source, valueName, flag, Data);
	int result; // eax@2
	if( valueName != 0 ? (Source != 0 ? (*valueName != 0 ? -(*Source != 0) : 0) : 0) : 0 ){
		result = SaveRegistryValue(Source, valueName, flag, 4u, (uchar*)&Data, 4u);
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (150182F0) --------------------------------------------------------
int __stdcall InitStorm(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	int v3; // eax@3
	int v4; // edi@3
	if( fdwReason == 1 ){
		InitCriticalStorm1();
		InitCriticalStormFile();
		SetupNetCriticalSection();
	}
	v3 = InitCriticalStorm2((int)hinstDLL, fdwReason, (int)lpReserved);
	v4 = v3;
	if( !fdwReason || fdwReason == 1 && !v3 )
		DeleteCriticalStorm1();
	return v4;
}

//----- (15018330) --------------------------------------------------------
void __stdcall DeleteCriticalStorm1()
{
	int i; // eax@1
	for( i = dword_15021380; dword_15021380; i = dword_15021380 ){
		dword_15021380 = i - 1;
		(*((void(__thiscall* *)(uint))dword_1502137C + dword_15021380))((uint)dword_1502137C);
	}
	VirtualFree(dword_1502137C, 0, 0x8000u);
	dword_15021378 = 0;
	dword_1502137C = 0;
	DeleteCriticalSection(&stru_15021360);
}

// 15021378: using guessed type int dword_15021378;
// 15021380: using guessed type int dword_15021380;
//----- (15018390) --------------------------------------------------------
void __stdcall InitCriticalStorm1()
{
	InitializeCriticalSection(&stru_15021360);
	InitCriticalStormArray();
	sub_150183C0((unsigned int)&unk_1502405C, (unsigned int)&unk_15024060);
	sub_150183C0((unsigned int)&unk_15024000, (unsigned int)&unk_15024058);
}

//----- (150183C0) --------------------------------------------------------
void __fastcall sub_150183C0(unsigned int a1, unsigned int a2)
{
	unsigned int v2; // edi@1
	unsigned int i;  // esi@1
	v2 = a2;
	for( i = a1; i < v2; i += 4 ){
		if( *(uint*)i )
			(*(void(* *)())i)();
	}
}

//----- (150183E0) --------------------------------------------------------
int __cdecl AtExit(int criticalSectionDelFunc)
{
	void* v1; // ebx@2
	int v2;   // ebp@2
	EnterCriticalSection(&stru_15021360);
	if( dword_15021380 >= (unsigned int)dword_15021378 ){
		v2 = dword_15021378 + 1024;
		v1 = VirtualAlloc(0, 4 * (dword_15021378 + 1024), MEM_COMMIT, 0x40/*PAGE_EXECUTE_READWRITE*/);
		if( !v1 ){
			LeaveCriticalSection(&stru_15021360);
			return 1;
		}
		if( dword_1502137C ){
			memcpy(v1, dword_1502137C, 4 * dword_15021378);
			VirtualFree(dword_1502137C, 0, 0x8000u);
		}
		dword_15021378 = v2;
		dword_1502137C = v1;
	}
	*((uint*)dword_1502137C + dword_15021380++) = criticalSectionDelFunc;
	LeaveCriticalSection(&stru_15021360);
	return 0;
}

// 15021378: using guessed type int dword_15021378;
// 15021380: using guessed type int dword_15021380;
//----- (150184A0) --------------------------------------------------------
void* __thiscall sub_150184A0(void* This, char a2)
{
	void* v2; // esi@1
	v2 = This;
	sub_150184C0(This);
	if( a2 & 1 )
		operator delete(v2);
	return v2;
}

//----- (150184C0) --------------------------------------------------------
void __thiscall sub_150184C0(void* This)
{
	*(uint*)This = (uint)&off_1501C940;
}

// 1501C940: using guessed type int (__thiscall *off_1501C940)(void *, char);
//----- (150184D0) --------------------------------------------------------
int __stdcall InitCriticalStorm2(int hInstDll, int fdvReason, int lpReserved)
{
	int result; // eax@2
	if( fdvReason == 1 ){
		StormDllHandle = hInstDll;
		InitCriticalStormArray();
		result = 1;
	}else{
		if( !fdvReason ){
			StormDestroy();
			SMemDestroy();
		}
		result = 1;
	}
	return result;
}

// 15021384: using guessed type int StormDllHandle;
//----- (15018510) --------------------------------------------------------
int __stdcall StormDestroy()
{
	//if( ExternalStorm ) return eStormDestroy();
	int v1; // esi@1
	int v2; // esi@1
	int v3; // esi@1
	int v4; // esi@1
	int v5; // esi@1
	int v6; // esi@1
	int v7; // esi@1
	int v8; // esi@1
	v1 = SDlgDestroy() & 1;
	v2 = SDrawDestroy() & v1;
	v3 = SBltDestroy() & v2;
	v4 = SMsgDestroy() & v3;
	v5 = SNetDestroy() & v4;
	v6 = STransDestroy() & v5;
	v7 = SCodeDestroy() & v6;
	v8 = SEvtDestroy() & v7;
	return SFileDestroy() & v8;
}

//----- (15018560) --------------------------------------------------------
int __stdcall GetStormDebugLevel()
{
	if( !dword_15021388 ){
		dword_15021388 = 1;
		SRegLoadValue("Internal", "DebugLevel", 0, &dword_1502138C);
	}
	return dword_1502138C;
}

// 15021388: using guessed type int dword_15021388;
// 1502138C: using guessed type int dword_1502138C;
//----- (15018590) --------------------------------------------------------
int __stdcall GetStormDllHandle()
{
	return StormDllHandle;
}

// 15021384: using guessed type int StormDllHandle;
//----- (150185B0) --------------------------------------------------------
int __stdcall sub_150185B0()
{
	int result; // eax@1
	result = 0;
	dword_15021390 = 0;
	dword_15021394 = 0;
	return result;
}

// 15021390: using guessed type int dword_15021390;
// 15021394: using guessed type int dword_15021394;
//----- (150185C0) --------------------------------------------------------
int __stdcall STransBlt(uchar* surface, int x, int y, int width, uchar* trans)
{
	int v5;           // edx@1
	int v6;           // ebx@1
	void* i;          // edi@1
	void* v8;         // esi@1
	uchar* v9;           // eax@1
	int v10;          // eax@5
	char v11;         // cl@5
	__int16 v12;      // cx@7
	unsigned int v13; // ecx@8
	__int16 v14;      // cx@10
	char v15;         // cl@12
	int v17;          // [sp+20h] [bp+14h]@1
	v9 = surface + x + y * width;
	v17 = width - *(uint*)(trans + 24);
	v5 = *(uint*)(trans + 28);
	v6 = *(uint*)(trans + 8) + *(uint*)(trans + 20);
	v8 = *(void**)(trans + 8);
	for( i = (void*)v9; v5; --v5 ){
		while( 1 ){
			v10 = *(ushort*)v6;
			v6 += 2;
			if( !v10 )
				break;
			if( (uchar)v10 > 3u ){
				if( (uchar)i & 1 ){
					v11 = *(uchar*)v8;
					v8 = (char*)v8 + 1;
					*(uchar*)i = v11;
					i = (char*)i + 1;
					LOBYTE_IDA(v10) = v10 - 1;
				}
				if( (uchar)i & 2 ){
					v12 = *(ushort*)v8;
					v8 = (char*)v8 + 2;
					*(ushort*)i = v12;
					i = (char*)i + 2;
          LOBYTE_IDA(v10) = v10 - 2;
				}
				v13 = (v10 & 0xFC) >> 2;
				memcpy(i, v8, 4 * v13);
				v8 = (char*)v8 + 4 * v13;
				i = (char*)i + 4 * v13;
			}
			if( v10 & 2 ){
				v14 = *(ushort*)v8;
				v8 = (char*)v8 + 2;
				*(ushort*)i = v14;
				i = (char*)i + 2;
			}
			if( v10 & 1 ){
				v15 = *(uchar*)v8;
				v8 = (char*)v8 + 1;
				*(uchar*)i = v15;
				i = (char*)i + 1;
			}
			i = (char*)i + BYTE1(v10);
		}
		i = (char*)i + v17;
	}
	return 1;
}

//----- (15018690) --------------------------------------------------------
int __stdcall STransBltUsingMask(void* a1, void* a2, int a3, int a4, int a5)
{
	int v5;           // eax@1
	int v6;           // edx@1
	int v7;           // edx@4
	int v8;           // ebx@4
	void* i;          // edi@4
	void* v10;        // esi@4
	int v11;          // eax@8
	char v12;         // cl@8
	__int16 v13;      // cx@10
	unsigned int v14; // ecx@11
	__int16 v15;      // cx@13
	char v16;         // cl@15
	int v18;          // [sp+20h] [bp+10h]@1
	int v19;          // [sp+24h] [bp+14h]@4
	int v20;          // [sp+28h] [bp+18h]@2
	v6 = a5;
	v5 = *(uint*)(a5 + 24);
	v18 = a3 - v5;
	if( a4 )
		v20 = a4 - v5;
	else
		v20 = 0;
	v19 = *(uint*)(v6 + 8) + *(uint*)(v6 + 20);
	v7 = *(uint*)(v6 + 28);
	v8 = v19;
	v10 = a2;
	for( i = a1; v7; --v7 ){
		while( 1 ){
			v11 = *(ushort*)v8;
			v8 += 2;
			if( !v11 )
				break;
			if( (uchar)v11 > 3u ){
				if( (uchar)i & 1 ){
					v12 = *(uchar*)v10;
					v10 = (char*)v10 + 1;
					*(uchar*)i = v12;
					i = (char*)i + 1;
					LOBYTE_IDA(v11) = v11 - 1;
				}
				if( (uchar)i & 2 ){
					v13 = *(ushort*)v10;
					v10 = (char*)v10 + 2;
					*(ushort*)i = v13;
					i = (char*)i + 2;
					LOBYTE_IDA(v11) = v11 - 2;
				}
				v14 = (v11 & 0xFCu) >> 2;
				memcpy(i, v10, 4 * v14);
				v10 = (char*)v10 + 4 * v14;
				i = (char*)i + 4 * v14;
			}
			if( v11 & 2 ){
				v15 = *(ushort*)v10;
				v10 = (char*)v10 + 2;
				*(ushort*)i = v15;
				i = (char*)i + 2;
			}
			if( v11 & 1 ){
				v16 = *(uchar*)v10;
				v10 = (char*)v10 + 1;
				*(uchar*)i = v16;
				i = (char*)i + 1;
			}
			v10 = (char*)v10 + BYTE1(v11);
			i = (char*)i + BYTE1(v11);
		}
		i = (char*)i + v18;
		v10 = (char*)v10 + v20;
	}
	return 1;
}

//----- (15018760) --------------------------------------------------------
int __stdcall STransCreate(char* a1, int a2, int a3, int a4, int* a5, int a6, char* a7)
{
	int v7;     // eax@1
	int v8;     // ebx@3
	int v9;     // ebp@3
	int v10;    // edi@3
	int result; // eax@4
	int v12;    // eax@7
	int v13;    // esi@7
	int v14;    // edi@7
	int v15;    // edx@7
	int v16;    // ecx@10
	int v17;    // edx@12
	int v18;    // [sp+10h] [bp-10h]@7
	int v19;    // [sp+14h] [bp-Ch]@7
	int v20;    // [sp+18h] [bp-8h]@7
	int v21;    // [sp+1Ch] [bp-4h]@7
	v7 = (int)a7;
	if( a7 )
		*(uint *)a7 = 0;
	v9 = (int)a1;
	v8 = a2;
	v10 = a3;
	if( a3 != 0 ? (a2 != 0 ? (a1 != 0 ? -(v7 != 0) : 0) : 0) : 0 ){
		if( a4 == 8 ){
			LOBYTE_IDA(a2) = sub_150188C0(a6);
			a1 = 0;
			a4 = 0;
			v13 = (int)a5;
			sub_15018960(v9, v8, v10, 8, (int)a5, (int)&v18, a2, 0, (int)&a1, (int)&a4);
			v14 = SMemAlloc((uint)a1, "STRANS.CPP", 560, 0);
			sub_15018960(v9, v8, a3, 8, v13, (int)&v18, a2, v14, 0, 0);
			v12 = sub_150188E0(0);
			*(uint*)(v12 + 32) = v18;
			*(uint*)(v12 + 36) = v19;
			*(uint*)(v12 + 40) = v20;
			v15 = v21;
			*(uint*)(v12 + 8) = v14;
			*(uint*)(v12 + 44) = v15;
			*(uint*)(v12 + 12) = (uint)a1;
			*(uint*)(v12 + 16) = (uint)a1;
			*(uint*)(v12 + 20) = a4;
			if( v13 )
				v8 = *(uint*)(v13 + 8) - *(uint*)v13 + 1;
			*(uint*)(v12 + 24) = v8;
			if( v13 )
				v16 = *(uint*)(v13 + 12) - *(uint*)(v13 + 4) + 1;
			else
				v16 = a3;
			v17 = (int)a7;
			*(uint*)(v12 + 28) = v16;
			*(uint*)v17 = v12;
			result = 1;
		}else{
			result = 0;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (150188C0) --------------------------------------------------------
int __fastcall sub_150188C0(int a1)
{
	int v1;     // eax@1
	int result; // eax@2
	v1 = a1;
	if( a1 & 0x1000000 )
		result = v1 & 0xFFFFFF;
	else
		result = 0;
	return result;
}

//----- (150188E0) --------------------------------------------------------
int __thiscall sub_150188E0(void* This)
{
	int result; // eax@1
	void* v2;   // esi@1
	int v3;     // ecx@5
	int v4;     // esi@5
	v2 = This;
	result = SMemAlloc(48, ".?AUTRANS@@", -2, 8);
	*(uint*)result = dword_15021394;
	if( dword_15021390 ){
		*(uint*)(dword_15021394 + 4) = result;
		dword_15021394 = result;
	}else{
		dword_15021394 = result;
		dword_15021390 = result;
	}
	if( v2 ){
		v3 = *((uint*)v2 + 3);
		v4 = (int)((char*)v2 + 32);
		*(uint*)(result + 12) = v3;
		*(uint*)(result + 16) = *(uint*)(v4 - 16);
		*(uint*)(result + 20) = *(uint*)(v4 - 12);
		*(uint*)(result + 24) = *(uint*)(v4 - 8);
		*(uint*)(result + 28) = *(uint*)(v4 - 4);
		*(uint*)(result + 32) = *(uint*)v4;
		*(uint*)(result + 36) = *(uint*)(v4 + 4);
		*(uint*)(result + 40) = *(uint*)(v4 + 8);
		*(uint*)(result + 44) = *(uint*)(v4 + 12);
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021390: using guessed type int dword_15021390;
// 15021394: using guessed type int dword_15021394;
//----- (15018960) --------------------------------------------------------
int __fastcall sub_15018960(int a1, int a2, int a3, int a4, int a5, int a6, char a7, int a8, int a9, int a10)
{
	int v10;    // eax@1
	int v11;    // ebx@1
	int v12;    // esi@2
	int v13;    // ebp@4
	int v14;    // ecx@5
	int v15;    // edi@8
	int v16;    // eax@11
	int v17;    // edi@11
	int v18;    // eax@27
	int v19;    // eax@29
	int v20;    // eax@31
	int v21;    // edi@31
	int v22;    // ebx@37
	int v23;    // eax@39
	char v24;   // dl@39
	int v25;    // ebp@39
	int v26;    // esi@42
	int v27;    // ecx@43
	int v28;    // edi@57
	int v29;    // ecx@59
	int v30;    // edi@62
	int result; // eax@64
	int v33;    // [sp+10h] [bp-Ch]@1
	int v34;    // [sp+14h] [bp-8h]@10
	int v35;    // [sp+18h] [bp-4h]@4
	int v36;    // [sp+20h] [bp+4h]@42
	int v37;    // [sp+28h] [bp+Ch]@5
	int v38;    // [sp+2Ch] [bp+10h]@38
	char v39;   // [sp+34h] [bp+18h]@39
	int v40;    // [sp+3Ch] [bp+20h]@38
	v10 = a5;
	v11 = 0;
	v33 = 0;
	if( a5 )
		v12 = *(uint*)a5 + a2 * *(uint*)(a5 + 4);
	else
		v12 = 0;
	v13 = a1 + v12;
	v35 = a1 + v12;
	if( a5 ){
		v14 = *(uint*)(a5 + 8) - *(uint*)a5 + 1;
		v37 = *(uint*)(a5 + 8) - *(uint*)a5 + 1;
	}else{
		v37 = a2;
		v14 = a2;
	}
	if( v10 ){
		v15 = *(uint*)(v10 + 12) - *(uint*)(v10 + 4) + 1;
		a3 = *(uint*)(v10 + 12) - *(uint*)(v10 + 4) + 1;
	}else{
		v15 = a3;
	}
	v34 = a2 - v14;
	*(uint*)a6 = 2147483647;
	*(uint*)(a6 + 4) = 2147483647;
	*(uint*)(a6 + 8) = 0;
	*(uint*)(a6 + 12) = 0;
	if( v15 > 0 ){
		do{
			v17 = 0;
			v16 = 0;
			if( v14 > 0 ){
				do{
					if( *(uchar*)v13 != a7 ){
						++v33;
						if( a8 )
							*(uchar *)a8++ = *(uchar *)v13;
						if( v16 < *(uint*)a6 )
							*(uint *)a6 = v16;
						if( v16 > *(uint*)(a6 + 8) )
							*(uint *)(a6 + 8) = v16;
						v17 = 1;
					}
					v14 = v37;
					++v13;
					++v16;
				}while( v16 < v37 );
			}
			if( v17 ){
				if( v11 < *(uint*)(a6 + 4) )
					*(uint *)(a6 + 4) = v11;
				if( v11 > *(uint*)(a6 + 12) )
					*(uint *)(a6 + 12) = v11;
			}
			v13 += v34;
			++v11;
		}while( v11 < a3 );
	}
	v18 = *(uint*)(a6 + 8);
	if( *(uint*)a6 > v18 )
		*(uint *)a6 = v18;
	v19 = *(uint*)(a6 + 12);
	if( *(uint*)(a6 + 4) > v19 )
		*(uint *)(a6 + 4) = v19;
	v21 = a8;
	v20 = v33 & 3;
	if( v33 & 3 ){
		if( a8 )
			v21 = 4 - v20 + a8;
		v33 += 4 - v20;
	}
	if( a10 )
		*(uint *)a10 = v33;
	v22 = v35;
	if( !a3 )
		goto LABEL_64;
	v38 = a3;
	v40 = v14 - 1;
	do{
		v25 = v40;
		v24 = 0;
		v39 = 0;
		v23 = 1;
		if( !v14 )
			goto LABEL_61;
		while( 1 ){
			v26 = 0;
			v36 = v22 + 1;
			v27 = *(uchar*)v22 != a7;
			if( v27 != v23 ){
				if( !v23 ){
					v26 = 1;
					++v25;
					--v36;
					goto LABEL_52;
				}
				v23 = 0;
				LABEL_50:
				++v24;
				goto LABEL_52;
			}
			if( !v23 )
				goto LABEL_50;
			++v39;
			LABEL_52:
			if( v26 || v39 == -4 || v24 == -4 || !v25 ){
				v33 += 2;
				if( v21 ){
					*(uchar*)v21 = v39;
					v28 = v21 + 1;
					*(uchar*)v28 = v24;
					v21 = v28 + 1;
				}
				v39 = 0;
				v24 = 0;
				v23 = 1;
			}
			v29 = v25--;
			if( !v29 )
				break;
			v22 = v36;
		}
		v14 = v37;
		v22 = v36;
		LABEL_61:
		v33 += 2;
		if( v21 ){
			*(uchar*)v21 = 0;
			v30 = v21 + 1;
			*(uchar*)v30 = 0;
			v21 = v30 + 1;
		}
		v22 += v34;
	}while( v38-- != 1 );
	LABEL_64:
	result = a9;
	if( a9 )
		*(uint *)a9 = v33;
	return result;
}

//----- (15018BB0) --------------------------------------------------------
int __stdcall STransDelete(void* a1)
{
	int v1; // eax@9
	if( *((uint*)a1 + 2) ){
		if( dword_1502139C )
			SMemFree((void*)dword_1502139C, "STRANS.CPP", 587, 0);
		dword_1502139C = *((uint*)a1 + 2);
		dword_150213A0 = *((uint*)a1 + 3);
		*((uint*)a1 + 2) = 0;
		*((uint*)a1 + 3) = 0;
	}
	if( *(uint*)a1 ){
		*(uint*)(*(uint*)a1 + 4) = *((uint*)a1 + 1);
	}else{
		if( (void*)dword_15021390 == a1 )
			dword_15021390 = *((uint*)a1 + 1);
	}
	v1 = *((uint*)a1 + 1);
	if( v1 ){
		*(uint*)v1 = *(uint*)a1;
	}else{
		if( (void*)dword_15021394 == a1 )
			dword_15021394 = *(uint*)a1;
	}
	*(uint*)a1 = 0;
	*((uint*)a1 + 1) = 0;
	SMemFree(a1, ".?AUTRANS@@", -2, 0);
	return 1;
}

// 15021390: using guessed type int dword_15021390;
// 15021394: using guessed type int dword_15021394;
// 1502139C: using guessed type int dword_1502139C;
// 150213A0: using guessed type int dword_150213A0;
//----- (15018C60) --------------------------------------------------------
int __stdcall STransDestroy()
{
	void* i; // eax@1
	for( i = (void*)dword_15021390; dword_15021390; i = (void*)dword_15021390 )
		STransDelete(i);
	if( dword_15021398 ){
		SMemFree((void*)dword_15021398, "STRANS.CPP", 603, 0);
		dword_15021398 = 0;
	}
	if( dword_1502139C ){
		SMemFree((void*)dword_1502139C, "STRANS.CPP", 607, 0);
		dword_1502139C = 0;
		dword_150213A0 = 0;
	}
	return 1;
}

// 15021390: using guessed type int dword_15021390;
// 15021398: using guessed type int dword_15021398;
// 1502139C: using guessed type int dword_1502139C;
// 150213A0: using guessed type int dword_150213A0;
//----- (15018CE0) --------------------------------------------------------
int __stdcall STransDuplicate(int a1, int a2)
{
	int result; // eax@4
	void* v3;   // esi@5
	void* v4;   // ebp@5
	int v5;     // eax@5
	if( a2 )
		*(uint *)a2 = 0;
	if( a1 != 0 ? -(a2 != 0) : 0 ){
		v3 = *(void**)(a1 + 8);
		v4 = (void*)SMemAlloc(*(uint*)(a1 + 12), "STRANS.CPP", 626, 0);
		memcpy(v4, v3, *(uint*)(a1 + 16));
		v5 = sub_150188E0((void*)a1);
		*(uint*)(v5 + 8) = (uint)v4;
		*(uint*)a2 = v5;
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15018D60) --------------------------------------------------------
int __stdcall STransInvertMask(int a1, int a2)
{
	int v2;     // ebx@3
	int result; // eax@4
	int v4;     // eax@5
	int v5;     // ebp@5
	int v6;     // edi@5
	int v7;     // esi@5
	char v8;    // cl@7
	char v9;    // dl@8
	int v10;    // esi@8
	int v11;    // eax@10
	int v12;    // eax@13
	int v13;    // eax@14
	int v14;    // [sp+10h] [bp-4h]@5
	int v15;    // [sp+18h] [bp+4h]@6
	if( a2 )
		*(uint *)a2 = 0;
	v2 = a1;
	if( a1 != 0 ? -(a2 != 0) : 0 ){
		v5 = *(uint*)(a1 + 16) + 2 * *(uint*)(a1 + 28) - *(uint*)(a1 + 20);
		v4 = SMemAlloc(*(uint*)(a1 + 16) + 2 * *(uint*)(a1 + 28) - *(uint*)(a1 + 20), "STRANS.CPP", 650, 0);
		v7 = *(uint*)(a1 + 20) + *(uint*)(a1 + 8);
		v6 = 0;
		v14 = v4;
		if( *(uint*)(a1 + 28) ){
			v15 = *(uint*)(a1 + 28);
			do{
				v8 = 0;
				do{
					v9 = *(uchar*)v7;
					v10 = v7 + 1;
					if( v8 || v9 ){
						*(uchar*)v4 = v8;
						v11 = v4 + 1;
						*(uchar*)v11 = v9;
						v4 = v11 + 1;
						v6 += 2;
					}
					v8 = *(uchar*)v10;
					v7 = v10 + 1;
				}while( v8 || v9 );
				*(uchar*)v4 = 0;
				v12 = v4 + 1;
				v6 += 2;
				*(uchar*)v12 = 0;
				v4 = v12 + 1;
				--v15;
			}while( v15 );
		}
		v13 = sub_150188E0((void*)v2);
		*(uint*)(v13 + 16) = v6;
		*(uint*)(v13 + 8) = v14;
		*(uint*)(v13 + 12) = v5;
		*(uint*)(v13 + 20) = 0;
		*(uint*)a2 = v13;
		result = 1;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15018E40) --------------------------------------------------------
int __stdcall STransIntersectDirtyArray(int a1, int a2, unsigned __int8 a3, int a4)
{
	unsigned int v4;     // edi@1
	int v6;              // eax@10
	int v7;              // ebx@10
	int v8;              // ebp@10
	void* v9;            // esi@10
	void* v10;           // ebx@14
	char v11;            // zf@17
	int v12;             // edx@17
	int v13;             // ecx@17
	int v14;             // edi@17
	int v15;             // esi@17
	void* v16;           // esi@22
	unsigned int v17;    // eax@22
	void* v18;           // edi@22
	int v19;             // edi@23
	unsigned __int8 v20; // al@24
	unsigned __int8 v21; // dl@24
	char v22;            // cl@25
	unsigned __int8 v23; // bl@25
	int v24;             // esi@25
	int v25;             // eax@30
	unsigned int v26;    // edi@41
	int v27;             // eax@46
	int v28;             // ebp@47
	unsigned int v29;    // [sp+14h] [bp-28h]@7
	int v30;             // [sp+18h] [bp-24h]@10
	void* v31;           // [sp+1Ch] [bp-20h]@8
	void* v32;           // [sp+20h] [bp-1Ch]@10
	int v33;             // [sp+24h] [bp-18h]@23
	int v34;             // [sp+28h] [bp-14h]@10
	int v35;             // [sp+2Ch] [bp-10h]@10
	unsigned int v36;    // [sp+30h] [bp-Ch]@8
	unsigned __int8 v37; // [sp+34h] [bp-8h]@25
	unsigned __int8 v38; // [sp+38h] [bp-4h]@24
	v4 = 0;
	if( a4 )
		*(uint *)a4 = 0;
	if( !(a4 != 0 ? (a3 != 0 ? (a2 != 0 ? -(a1 != 0) : 0) : 0) : 0) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( !dword_15021398 )
		return 0;
	v29 = 0;
	if( dword_1502139C ){
		v31 = (void*)dword_1502139C;
		v36 = dword_150213A0;
		dword_150213A0 = 0;
		dword_1502139C = 0;
	}else{
		v36 = 4096;
		v31 = (void*)SMemAlloc(4096, "STRANS.CPP", 720, 0);
	}
	v9 = v31;
	v7 = *(uint*)(a1 + 8) + *(uint*)(a1 + 20);
	v6 = *(uint*)(a1 + 8) + *(uint*)(a1 + 20);
	v30 = *(uint*)(a1 + 8) + *(uint*)(a1 + 20);
	v8 = (int)v31;
	v34 = *(uint*)(a1 + 8) + *(uint*)(a1 + 20);
	v32 = v31;
	v35 = 0;
	if( *(uint*)(a1 + 28) <= 0 )
		goto LABEL_46;
	while( 2 ){
		if( v4 + 512 > v36 ){
			v36 += 4096;
			v10 = (void*)SMemAlloc(v36, "STRANS.CPP", 736, 0);
			memcpy(v10, v9, v4);
			v8 += (uint)v10 - (uint)v31;
			v32 = (char*)v32 + (uint)v10 - (uint)v31;
			SMemFree(v31, "STRANS.CPP", 740, 0);
			v6 = v34;
			v31 = v10;
			v7 = v30;
		}
		if( (dword_1501E36C - 1) & v35 ){
			if( *(uint*)v7 == *(uint*)v6 ){
				v14 = v6 + 4;
				v12 = v7 - v6;
				v15 = v7 + 4;
				v11 = 1;
				v13 = v7 - v6 - 4;
				do{
					if( !v13 )
						break;
					v11 = *(uchar*)v15++ == *(uchar*)v14++;
					--v13;
				}while( v11 );
				if( v11 ){
					v16 = v32;
					v17 = v8 - (uint)v32;
					v18 = (void*)v8;
					v32 = (void*)v8;
					v8 += v17;
					memcpy(v18, v16, v17);
					v34 = v30;
					v30 += v12;
					v7 = v30;
					v29 += v17;
					goto LABEL_44;
				}
			}
		}
		v34 = v7;
		v32 = (void*)v8;
		v19 = 0;
		v33 = a2 + *(uint*)(dword_15021398 + 4 * (v35 >> dword_1501E368));
		do{
			v21 = *(uchar*)v7;
			v20 = *(uchar*)(v7 + 1);
			v7 += 2;
			v38 = v20;
			v30 = v7;
			if( !v21 ){
				*(uchar*)v8 = 0;
				v28 = v8 + 1;
				*(uchar*)v28 = v20;
				v8 = v28 + 1;
				v29 += 2;
				goto LABEL_40;
			}
			v23 = v21;
			v22 = 0;
			v37 = v21;
			v24 = 1;
			while( v23 ){
				LABEL_29:
				if( ((*(uchar*)v33 & a3) != 0) == v24 ){
					v25 = dword_1501E364 - v19;
					if( v37 >= (unsigned int)(dword_1501E364 - v19) ){
						v22 += v25;
						v23 -= v25;
						v19 = 0;
						v37 = v23;
						++v33;
					}else{
						v22 += v23;
						v19 += v37;
						v23 = 0;
						v37 = 0;
					}
				}else{
					LABEL_33:
					*(uchar*)v8++ = v22;
					++v29;
					v22 = 0;
					v24 = v24 == 0;
				}
			}
			if( v22 ){
				if( !v23 )
					goto LABEL_33;
				goto LABEL_29;
			}
			v20 = v38;
			if( v38 || !v24 ){
				if( v24 ){
					*(uchar*)v8++ = 0;
					++v29;
				}
				*(uchar*)v8++ = v38;
				++v29;
			}
			v7 = v30;
			LABEL_40:
			if( v20 ){
				v26 = v38 + v19;
				v33 += v26 >> dword_1501E360;
				v19 = (dword_1501E364 - 1) & v26;
			}
		}while( v21 || v20 );
		LABEL_44:
		++v35;
		if( v35 < *(uint*)(a1 + 28) ){
			v6 = v34;
			v4 = v29;
			v9 = v31;
			continue;
		}
		break;
	}
	v4 = v29;
	v9 = v31;
	LABEL_46:
	v27 = sub_150188E0((void*)a1);
	*(uint*)(v27 + 16) = v4;
	*(uint*)(v27 + 8) = (uint)v9;
	*(uint*)(v27 + 12) = v36;
	*(uint*)(v27 + 20) = 0;
	*(uint*)a4 = v27;
	return 1;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E360: using guessed type int dword_1501E360;
// 1501E364: using guessed type int dword_1501E364;
// 1501E368: using guessed type int dword_1501E368;
// 1501E36C: using guessed type int dword_1501E36C;
// 15021398: using guessed type int dword_15021398;
// 1502139C: using guessed type int dword_1502139C;
// 150213A0: using guessed type int dword_150213A0;
//----- (150191B0) --------------------------------------------------------
int __stdcall STransLoadE(int a1, __int64 a2)
{
	int result;          // eax@2
	int v3;              // esi@3
	int v4;              // eax@6
	int v5;              // ecx@7
	__int16 v6;          // dx@8
	__int16 v7;          // dx@9
	int v8;              // ecx@11
	unsigned __int8 v9;  // dl@12
	int v10;             // esi@15
	unsigned __int8 v11; // [sp+Ch] [bp+4h]@12
	unsigned __int8 v12; // [sp+10h] [bp+8h]@12
	if( a1 ){
		v3 = a2;
		if( a2 >= 0 && (uint)a2 < *(uint*)(a1 + 24) && HIDWORD_IDA(a2) < *(uint*)(a1 + 28) ){
			v4 = *(uint*)(a1 + 8) + *(uint*)(a1 + 20);
			if( HIDWORD_IDA(a2) ){
				v5 = HIDWORD_IDA(a2);
				do{
					v6 = *(ushort*)v4;
					v4 += 2;
					if( v6 ){
						do{
							v7 = *(ushort*)v4;
							v4 += 2;
						}while( v7 );
					}
					--v5;
				}while( v5 );
			}
			v8 = v4;
			while( 1 ){
				v9 = *(uchar*)(v8 + 1);
				v11 = *(uchar*)v8;
				v8 += 2;
				v12 = v9;
				if( !v11 ){
					if( !v9 )
						break;
				}
				if( v11 > v3 )
					return 1;
				v10 = v3 - v11;
				if( v12 > v10 )
					break;
				v3 = v10 - v12;
			}
		}
		result = 0;
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (15019260) --------------------------------------------------------
int __stdcall STransLoadI(int Str, int a2, int a3, int a4)
{
	int v4;     // ebx@1
	int v5;     // edi@3
	int result; // eax@4
	int v7;     // ST00_4@5
	int v8;     // esi@7
	int v9;     // eax@7
	v4 = a4;
	if( a4 )
		*(uint *)a4 = 0;
	v5 = Str;
	if( *(uchar*)Str != 0 ? (v4 != 0 ? -(Str != 0) : 0) : 0 ){
		v7 = Str;
		Str = 0;
		a4 = 0;
		result = SBmpLoadImage((char*)v7, 0, 0, 0, (int)&Str, (int)&a4, 0);
		if( result ){
			v9 = SMemAlloc(Str * a4, "STRANS.CPP", 901, 0);
			v8 = v9;
			result = SBmpLoadImage((char*)v5, 0, v9, Str * a4, 0, 0, 0);
			if( result ){
				STransCreate((char*)v8, Str, a4, 8, (int*)a2, a3, (char*)v4);
				SMemFree((void*)v8, "STRANS.CPP", 915, 0);
				result = 1;
			}
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
//----- (15019350) --------------------------------------------------------
int __stdcall STransSetDirtyArrayInfo(int a1, int a2, int a3, int a4)
{
	int result; // eax@3
	int i;      // eax@7
	int v6;     // edx@7
	if( dword_15021398 ){
		SMemFree((void*)dword_15021398, "STRANS.CPP", 928, 0);
		dword_15021398 = 0;
	}
	result = sub_15019450(a3, (int)&dword_1501E360);
	if( result ){
		result = sub_15019450(a4, (int)&dword_1501E368);
		if( result ){
			dword_1501E364 = a3;
			dword_1501E36C = a4;
			dword_1501E358 = ((1 << dword_1501E360) + a1 - 1) >> dword_1501E360;
			dword_1501E35C = ((1 << dword_1501E368) + a2 - 1) >> dword_1501E368;
			dword_15021398 = SMemAlloc(4 * dword_1501E35C, "STRANS.CPP", 943, 0);
			v6 = 0;
			for( i = 0; i < dword_1501E35C; ++i ){
				*(uint*)(dword_15021398 + 4 * i) = v6;
				v6 += a1 >> dword_1501E360;
			}
			result = 1;
		}
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 1501E358: using guessed type int dword_1501E358;
// 1501E35C: using guessed type int dword_1501E35C;
// 1501E360: using guessed type int dword_1501E360;
// 1501E364: using guessed type int dword_1501E364;
// 1501E368: using guessed type int dword_1501E368;
// 1501E36C: using guessed type int dword_1501E36C;
// 15021398: using guessed type int dword_15021398;
//----- (15019450) --------------------------------------------------------
int __fastcall sub_15019450(int a1, int a2)
{
	int v2; // eax@1
	int i;  // esi@1
	v2 = 1;
	for( i = 0; v2 < a1; ++i )
		v2 *= 2;
	*(uint*)a2 = i;
	return v2 == a1;
}

//----- (15019470) --------------------------------------------------------
int __stdcall STransUpdateDirtyArray(int a1, char a2, int a3, int a4, int a5, int a6)
{
	int result; // eax@2
	int v7;     // edx@7
	int v8;     // ebx@7
	int v9;     // ebp@7
	int i;      // edi@7
	int v11;    // eax@8
	int v12;    // esi@9
	if( a5 != 0 ? (a2 != 0 ? -(a1 != 0) : 0) : 0 ){
		if( !dword_15021398 || *(uint*)(a5 + 24) <= 0 || *(uint*)(a5 + 28) <= 0 || a6 ){
			result = 0;
		}else{
			v9 = (a3 + *(uint*)(a5 + 40)) >> dword_1501E360;
			v8 = (a4 + *(uint*)(a5 + 44)) >> dword_1501E368;
			v7 = (a3 + *(uint*)(a5 + 32)) >> dword_1501E360;
			for( i = (a4 + *(uint*)(a5 + 36)) >> dword_1501E368; i <= v8; ++i ){
				v11 = a1 + v7 + *(uint*)(dword_15021398 + 4 * i);
				if( v7 <= v9 ){
					v12 = v9 - v7 + 1;
					do{
						*(uchar*)v11++ |= a2;
						--v12;
					}while( v12 );
				}
			}
			result = 1;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 1501E360: using guessed type int dword_1501E360;
// 1501E368: using guessed type int dword_1501E368;
// 15021398: using guessed type int dword_15021398;
//----- (15019560) --------------------------------------------------------
int __stdcall sub_15019560()
{
	int result; // eax@1
	result = 0;
	dword_15021BA8 = 0;
	dword_15021BAC = 0;
	return result;
}

// 15021BA8: using guessed type int dword_15021BA8;
// 15021BAC: using guessed type int dword_15021BAC;
//----- (15019570) --------------------------------------------------------
int __stdcall SVidDestroy()
{
	//if( ExternalStorm ) return eSVidDestroy();
	int i; // eax@1
	for( i = dword_15021BA8; dword_15021BA8; i = dword_15021BA8 )
		SVidPlayEnd(i);
	if( dword_15021BB0 ){
		SCodeDelete(*(void**)dword_15021BB0);
		SCodeDelete(*(void**)(dword_15021BB0 + 4));
		SCodeDelete(*(void**)(dword_15021BB0 + 8));
		SMemFree((void*)dword_15021BB0, "SVID.CPP", 500, 0);
		dword_15021BB0 = 0;
	}
	if( dword_15021BB4 ){
		SMemFree((void*)dword_15021BB4, "SVID.CPP", 506, 0);
		dword_15021BB4 = 0;
	}
	if( dword_15021BB8 ){
		FreeLibrary(dword_15021BB8);
		dword_15021BB8 = 0;
	}
	return 1;
}

// 15021BA8: using guessed type int dword_15021BA8;
// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (15019620) --------------------------------------------------------
int __stdcall SVidGetSize(int a1, int a2, int a3, int a4)
{
	int result; // eax@8
	if( a2 )
		*(uint *)a2 = 0;
	if( a3 )
		*(uint *)a3 = 0;
	if( a4 )
		*(uint *)a4 = 8;
	if( a1 ){
		result = sub_150196C0((void*)a1);
		if( result ){
			if( a2 )
				*(uint *)a2 = *(uint *)(*(uint *)(*(uint *)(a1 + 88) + 4) + 4);
			if( a3 )
				*(uint *)a3 = *(uint *)(*(uint *)(*(uint *)(a1 + 88) + 4) + 8);
			result = a2 || a3 || a4;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (150196C0) --------------------------------------------------------
int __thiscall sub_150196C0(void* This)
{
	int result; // eax@1
	result = dword_15021BA8;
	if( dword_15021BA8 ){
		while( (void*)result != This ){
			result = *(uint*)(result + 4);
			if( !result )
				return result;
		}
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 15021BA8: using guessed type int dword_15021BA8;
//----- (150196E0) --------------------------------------------------------
int __stdcall SVidInitialize(int a1)
{
	//if( ExternalStorm ) return eSVidInitialize(a1);
	HMODULE v1; // eax@2
	int v2;     // ecx@4
	int v3;     // esi@6
	int v4;     // eax@31
	int result; // eax@36
	int v6;     // [sp+0h] [bp-4h]@1
	v6 = 0;
	SDrawGetServiceLevel((int)&v6);
	if( v6 < 1 )
		goto LABEL_40;
	v1 = dword_15021BB8;
	if( !dword_15021BB8 ){
		v1 = LoadLibraryA("smackw32.dll");
		dword_15021BB8 = v1;
		if( !v1 )
			goto LABEL_41;
	}
	v2 = dword_15021BB4;
	if( !dword_15021BB4 ){
		dword_15021BB4 = SMemAlloc(48, "SVID.CPP", 105, 0);
		v1 = dword_15021BB8;
	}
	v3 = 1;
	*(uint*)dword_15021BB4 = (uint)GetProcAddress(v1, "_SmackBufferClose@4");
	if( !*(uint*)dword_15021BB4 )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 4) = (uint)GetProcAddress(dword_15021BB8, "_SmackBufferNewPalette@12");
	if( !*(uint*)(dword_15021BB4 + 4) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 8) = (uint)GetProcAddress(dword_15021BB8, "_SmackBufferOpen@24");
	if( !*(uint*)(dword_15021BB4 + 8) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 12) = (uint)GetProcAddress(dword_15021BB8, "_SmackClose@4");
	if( !*(uint*)(dword_15021BB4 + 12) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 16) = (uint)GetProcAddress(dword_15021BB8, "_SmackDoFrame@4");
	if( !*(uint*)(dword_15021BB4 + 16) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 20) = (uint)GetProcAddress(dword_15021BB8, "_SmackGoto@8");
	if( !*(uint*)(dword_15021BB4 + 20) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 24) = (uint)GetProcAddress(dword_15021BB8, "_SmackNextFrame@4");
	if( !*(uint*)(dword_15021BB4 + 24) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 28) = (uint)GetProcAddress(dword_15021BB8, "_SmackOpen@12");
	if( !*(uint*)(dword_15021BB4 + 28) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 32) = (uint)GetProcAddress(dword_15021BB8, "_SmackSoundUseDirectSound@4");
	if( !*(uint*)(dword_15021BB4 + 32) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 36) = (uint)GetProcAddress(dword_15021BB8, "_SmackToBuffer@28");
	if( !*(uint*)(dword_15021BB4 + 36) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 40) = (uint)GetProcAddress(dword_15021BB8, "_SmackToBufferRect@8");
	if( !*(uint*)(dword_15021BB4 + 40) )
		v3 = 0;
	*(uint*)(dword_15021BB4 + 44) = (uint)GetProcAddress(dword_15021BB8, "_SmackWait@4");
	if( !*(uint*)(dword_15021BB4 + 44) )
		LABEL_41:
		v3 = 0;
	if( !v3 )
		goto LABEL_40;
	v4 = dword_15021BB0;
	if( !dword_15021BB0 ){
		dword_15021BB0 = SMemAlloc(12, "SVID.CPP", 560, dword_15021BB0);
		SCodeCompile((int)"1 W2=S W1=W2 2 D=W", "1 W1=W2 W2=S W1=TW 2 D=W", 0, 0x140u, 0, dword_15021BB0);
		SCodeCompile(0, "1 W1=S W2=SC D=TW", 0, 0x140u, 0, dword_15021BB0 + 4);
		SCodeCompile(0, "1 W1=S W2=W1 2 D=W", 0, 0x140u, 0x4000000u, dword_15021BB0 + 8);
		v4 = dword_15021BB0;
	}
	if( *(uint*)v4 && *(uint*)(v4 + 4) && *(uint*)(v4 + 8) ){
		(*(void(__stdcall* *)(int))(dword_15021BB4 + 32))(a1);
		result = 1;
	}else{
		LABEL_40:
		result = 0;
	}
	return result;
}

// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (150199C0) --------------------------------------------------------
int __stdcall SVidPlayBegin(char* Str, int a2, int a3, int a4, int a5, int a6, int a7)
{
	//if( ExternalStorm ) return eSVidPlayBegin(Str, a2, a3, a4, a5, a6, a7);
	int v7;                             // ebx@3
	int v8;                             // ecx@5
	int v9;                             // eax@7
	int v10;                            // ecx@9
	int v11;                            // eax@11
	int v12;                            // ebp@11
	int v13;                            // ecx@13
	__int16 v15;                        // ax@19
	int v16;                            // edi@20
	DWORD v17;                          // esi@21
	DWORD v18;                          // ebp@21
	int v19;                            // eax@23
	int v20;                            // esi@32
	int v21;                            // eax@35
	unsigned int v22;                   // edi@35
	int v23;                            // eax@42
	int v24;                            // ebx@48
	int v25;                            // eax@53
	int v26;                            // eax@56
	int v27;                            // ecx@56
	int v28;                            // eax@58
	int v29;                            // eax@58
	unsigned int v30;                   // eax@64
	HWND v31;                           // eax@66
	HDC v32;                            // ebx@66
	HWND v33;                           // eax@66
	int v34;                            // [sp+38h] [bp-418h]@20
	int Data;                           // [sp+3Ch] [bp-414h]@18
	int v36;                            // [sp+40h] [bp-410h]@20
	int v37;                            // [sp+44h] [bp-40Ch]@20
	int v38;                            // [sp+48h] [bp-408h]@20
	int v39;                            // [sp+4Ch] [bp-404h]@18
	struct tagPALETTEENTRY pPalEntries; // [sp+50h] [bp-400h]@66
	if( a7 )
		*(uint *)a7 = 0;
	v7 = a6;
	if( a2 || !(a6 & 0x20000000) )
		v8 = -1;
	else
		v8 = 0;
	v9 = v8&(a7 != 0 ? -(Str != 0) : 0);
	if( a4 || !a2 )
		v10 = -1;
	else
		v10 = 0;
	v12 = a5;
	v11 = v10 & v9;
	if( a5 && *(uint*)a5 != 12 )
		v13 = 0;
	else
		v13 = -1;
	if( !(v11& v13) ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( a6 & 0x808 ){
		v39 = 0;
		Data = 0;
		SRegLoadValue("Video Player", "Version", 0, &v39);
		SRegLoadValue("Video Player", "Mode", 0, &Data);
		if( v39 != 3 || (v15 = Data, !Data) ){
			Sleep(0x1F4u);
			SDrawGetScreenSize((int)&v37, (int)&v38, 0);
			v16 = SMemAlloc(v37 * v38, "SVID.CPP", 622, 0);
			if( SDrawLockSurface(0, 0, (int)&v34, (int)&v36, 0) ){
				v18 = GetTickCount();
				SBltROP3(v16, v34, v37, v38, v37, v36, 0, 0xCC0020u);
				SBltROP3(v34, v16, v37, v38, v36, v37, 0, 0xCC0020u);
				v17 = GetTickCount() - v18;
				SDrawUnlockSurface(v16, 0, v34, 0, 0);
				v12 = a5;
			}else{
				v17 = -1;
			}
			SMemFree((void*)v16, "SVID.CPP", 648, 0);
			v19 = -(v17 > 0x46);
			LOBYTE_IDA(v19) = v19 & 0xFE;
			Data = v19 + 514;
			SRegSaveValue("Video Player", "Version", 0, 3u);
			SRegSaveValue("Video Player", "Benchmark", 0, v17);
			SRegSaveValue("Video Player", "Mode", 0, Data);
			v15 = Data;
		}
		if( BYTE1(v7) & 8 )
			v7 = v15 & 0x300 | a6;
		if( v7 & 8 )
			v7 = v7 & 0xFFFFFFF4 | v15 & 3;
	}
	if( (!dword_15021BB8 || !dword_15021BB4 || !dword_15021BB0) && !SVidInitialize(0) )
		return 0;
	v20 = SMemAlloc(132, ".?AUVIDEOREC@@", -2, 8);
	*(uint*)v20 = dword_15021BAC;
	if( dword_15021BA8 ){
		*(uint*)(dword_15021BAC + 4) = v20;
		dword_15021BAC = v20;
	}else{
		dword_15021BAC = v20;
		dword_15021BA8 = v20;
	}
	v21 = v7;
	BYTE1(v7) &= 0xFEu;
	*(uint*)(v20 + 8) = -1;
	v22 = (v7 | ((unsigned int) ~v21 >> 1) & 0x100) & 0xEFFFFFFF | (~(v7 | ((unsigned int) ~v21 >> 1) & 0x100) >> 1) & 0x10000000;
	if( v22 & 0x100 )
		v22 &= 0xFFFFFFFCu;
	if( v22 & 0x20000000 )
		v22 &= 0xFFF7FFFFu;
	if( v22 & 0x10000000 )
		a2 = 0;
	*(uint*)(v20 + 128) = v22;
	*(uint*)(v20 + 92) = a2;
	if( a3 ){
		*(uint*)(v20 + 96) = *(uint*)a3;
		*(uint*)(v20 + 100) = *(uint*)(a3 + 4);
		*(uint*)(v20 + 104) = *(uint*)(a3 + 8);
		v23 = *(uint*)(a3 + 12);
		*(uint*)(v20 + 112) = v20 + 96;
		*(uint*)(v20 + 108) = v23;
	}
	if( a4 ){
		*(uint*)(v20 + 116) = *(uint*)a4;
		*(uint*)(v20 + 120) = *(uint*)(a4 + 4);
	}
	if( v12 ){
		*(uint*)(v20 + 80) = *(uint*)(v12 + 4);
		*(uint*)(v20 + 84) = *(uint*)(v12 + 8);
	}else{
		*(uint*)(v20 + 80) = 1;
		*(uint*)(v20 + 84) = 254;
	}
	*(uint*)(v20 + 88) = SMemAlloc(16, "SVID.CPP", 713, 8);
	v24 = 1044480;
	v34 = -1;
	if( v22 & 0x20000 )
		v24 = 1044992;
	if( v22 & 0x400000 )
		BYTE1(v24) |= 4u;
	if( v22 & 0x10000 ){
		v25 = (int)Str;
		v34 = (int)Str;
	}else{
		if( !sub_1500E5E0((char*)Str, (int)&v34) ){
			LABEL_70:
			SVidPlayEnd(v20);
			return 0;
		}
		v25 = v34;
		*(uint*)(v20 + 8) = v34;
	}
	*(uint *)(*(uint *)(v20 + 88) + 4) = (*(int (__stdcall **)(int, int, int))(dword_15021BB4 + 28))(
		v25,
		v24,
		-1);
	v26 = *(uint*)(v20 + 88);
	v27 = *(uint*)(v26 + 4);
	if( !v27 )
		goto LABEL_70;
	if( !(v22 & 0x200) )
		goto LABEL_60;
	v29 = *(uint*)(v26 + 4);
	LOWORD_IDA(v27) = *(ushort*)(v27 + 8);
	LOWORD_IDA(v29) = 2 * *(ushort*)(v29 + 4);
	**(uint **)(v20 + 88) = (*(int (__stdcall **)(uint, int, int, int, int, int))(dword_15021BB4 + 8))(
		0,
		3,
		v29,
		v27,
		v29,
		v27);
	v28 = *(uint*)(v20 + 88);
	if( !*(uint*)v28 )
		goto LABEL_70;
	(*(void (__stdcall **)(uint, uint, uint, int, uint, uint, uint))(dword_15021BB4 + 36))(
		*(uint *)(v28 + 4),
		0,
		0,
		2 * *(uint *)(*(uint *)(v28 + 4) + 4),
		*(uint *)(*(uint *)(v28 + 4) + 8),
		*(uint *)(*(uint *)v28 + 1096),
		0);
	LABEL_60:
	if( v22 & 0x80000 && SDrawGetScreenSize(0, (int)&v36, 0) )
		*(uint*)(*(uint*)(v20 + 88) + 12) = (unsigned int)(v36 - (*(uint*)(*(uint*)(*(uint*)(v20 + 88) + 4) + 8) << ((v22 >> 9) & 1))) >> 1;
	else
		*(uint*)(*(uint*)(v20 + 88) + 12) = 0;
	v30 = v22;
	if( (v30 & 0x100002) == 1048578 ){
		if( SDrawGetFrameWindow(0) ){
			v31 = SDrawGetFrameWindow(0);
			v32 = GetDC(v31);
			GetSystemPaletteEntries(v32, 0, 0x100u, &pPalEntries);
			v33 = SDrawGetFrameWindow(0);
			ReleaseDC(v33, v32);
			sub_15019EF0(v20, &pPalEntries);
		}
	}
	*(uint*)(v20 + 12) = 64;
	*(uint*)a7 = v20;
	if( v22 & 0x200000 )
		SDrawShowCursor(0);
	return 1;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
// 150101B0: using guessed type _DWORD __stdcall SMemAlloc(_DWORD, _DWORD, _DWORD, char);
// 15019EF0: using guessed type int __fastcall sub_15019EF0(_DWORD, _DWORD);
// 15021BA8: using guessed type int dword_15021BA8;
// 15021BAC: using guessed type int dword_15021BAC;
// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
static unsigned int dword_1501E388[] = {
	-1, 0, 0, 0, -1, 0, 0, 0, -1, 1, 0, 0, 0, 1, 0, 0, 0, 1, -1, 1, 0, -1, 0, 1, 1, -1, 0, 0, -1, 1, 1, 0, -1, 0, 1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, 1, 1, 0, 1, 0, 1, 0, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1
};

// original function can't be C-disassempled bcoz positive sp values found.
// This function isn't from hellfire, its from diablo dll
//----- (15019EF0) --------------------------------------------------------
void __fastcall sub_15019EF0(int a1, PALETTEENTRY* a2)
{
	PALETTEENTRY* v2;    // ebp@1
	int result;          // eax@4
	PALETTEENTRY* v4;    // eax@5
	unsigned int v5;     // ecx@5
	int v6;              // ebx@5
	char v7;             // cl@16
	int v8;              // eax@17
	int v9;              // ecx@19
	int v10;             // eax@23
	int v11;             // ebx@23
	int v12;             // esi@23
	unsigned int v13;    // ecx@30
	int i;               // esi@32
	int v15;             // eax@33
	int v16;             // ebx@33
	int v17;             // eax@1
	void* v18;           // eax@2
	unsigned __int8 v19; // dl@10
	int v20;             // esi@10
	int v21;             // edx@17
	int v22;             // ecx@17
	int v23;             // edx@19
	int v24;             // eax@19
	int v25;             // edx@19
	int v26;             // edx@23
	int v27;             // edx@33
	int v28;             // eax@33
	int v29;             // edx@33
	int v30;             // edi@41
	int v31;             // edx@41
	int v32;             // ecx@42
	int v33;             // [sp+14h] [bp-636Ch]@1
	uint v34[5832];    // [sp+860h] [bp-5B20h]@5
	char v35;            // [sp+60h] [bp-6320h]@5
	int v36;             // [sp+28h] [bp-6358h]@10
	int v37;             // [sp+2Ch] [bp-6354h]@10
	int v38;             // [sp+10h] [bp-6370h]@10
	int v39;             // [sp+18h] [bp-6368h]@12
	int v40;             // [sp+24h] [bp-635Ch]@12
	PALETTEENTRY* v41;   // [sp+38h] [bp-6348h]@12
	int v42;             // [sp+20h] [bp-6360h]@13
	int v43;             // [sp+5Ch] [bp-6324h]@14
	PALETTEENTRY* v44;   // [sp+34h] [bp-634Ch]@14
	int v45;             // [sp+44h] [bp-633Ch]@17
	int v46;             // [sp+40h] [bp-6340h]@17
	int v47;             // [sp+3Ch] [bp-6344h]@17
	int v48;             // [sp+1Ch] [bp-6364h]@19
	int v49;             // [sp+30h] [bp-6350h]@19
	int v50;             // [sp+48h] [bp-6338h]@19
	int v51;             // [sp+50h] [bp-6330h]@19
	int v52;             // [sp+58h] [bp-6328h]@19
	int v53;             // [sp+54h] [bp-632Ch]@33
	int v54;             // [sp+4Ch] [bp-6334h]@33
	v17 = *(uint*)(a1 + 68);
	v33 = a1;
	v2 = a2;
	if( !v17 ){
		v18 = (void*)SMemAlloc(0x10000, "SVID.CPP", 190, 0);
		*(uint*)(v33 + 68) = (uint)v18;
		*(uint*)(v33 + 36) = (uint)v18;
	}
	if( *(uint*)(v33 + 68) ){
		v6 = 0;
		memset(v34, 0, sizeof(v34));
		v5 = (unsigned int)&v35;
		v4 = v2;
		do{
			if( v5 == (uint)&v35 || *(uchar*)(v4 - 4) != *(uchar*)v4 || *(uchar*)(v4 - 3) != *(uchar*)(v4 + 1) || *(uchar*)(v4 + 2) != *(uchar*)(v4 - 2) ){
				v19 = *(uchar*)v4;
				*(uchar*)v5 = v6;
				v36 = 0;
				v37 = 0;
				v38 = v19 >> 4;
				v36 = (unsigned __int8)(*(uchar*)(v4 + 1) >> 4);
				v37 = (unsigned __int8)(*(uchar*)(v4 + 2) >> 4);
				v20 = v37 + 18 * (v36 + 9 * (2 * v38 + 2) + 1) + 1;
				*(uint*)(v5 + 4) = v34[v20];
				v34[v20] = v5;
			}
			v5 += 8;
			v4 += 4;
			++v6;
		}while( v5 < (unsigned int)v34 );
		v39 = 0;
		v40 = 0;
		v41 = v2;
		do{
			v42 = 0;
			if( v40 >= 0 ){
				v43 = 0;
				v44 = v2;
				do{
					if( v42 == v39 ){
						v7 = v39;
					}else{
						v45 = (*(uchar*)v41 + *(uchar*)v44) >> 1;
						v46 = (*(uchar*)(v41 + 1) + *(uchar*)(v44 + 1)) >> 1;
						v21 = *(uchar*)(v44 + 2);
						v22 = (v21 + *(uchar*)(v41 + 2)) >> 1;
						v47 = (v21 + *(uchar*)(v41 + 2)) >> 1;
						v38 = (v45 >> 4) + 1;
						v36 = (v46 >> 4) + 1;
						v37 = (v22 >> 4) + 1;
						v8 = v42;
						if( v42 <= v39 )
							v8 = v39;
						v48 = v8;
						v49 = 2147483647;
						v23 = *(uchar*)(v2 + 1 + 4 * v8);
						v50 = abs(v45 - *(uchar*)(v2 + 4 * v8));
						v24 = abs(v46 - v23);
						v25 = *(uchar*)(v2 + 2 + 4 * v48);
						v51 = v24;
						v52 = abs(v47 - v25);
						v9 = v34[v37 + 18 * (v36 + 18 * v38)];
						if( v9 ){
							if( *(uint*)(v9 + 4) ){
								for( ; v9; v9 = *(uint*)(v9 + 4) ){
									v11 = *(uchar*)v9;
									v12 = abs(v47 - *(uchar*)(v2 + 2 + 4 * v11));
									v26 = *(uchar*)(v2 + 4 * v11);
									v38 = abs(v46 - *(uchar*)(v2 + 1 + 4 * v11));
									v10 = abs(v45 - v26);
									if( v50 > v10 || v38 < v51 || v12 < v52 ){
										if( v10* v10 + v12* v12 + v38* v38 < v49 ){
											v50 = v10;
											v51 = v38;
											v52 = v12;
											v49 = v10 * v10 + v12 * v12 + v38 * v38;
											v48 = v11;
										}
									}
								}
							}else{
								v48 = *(uchar*)v9;
							}
						}else{
							v13 = (unsigned int)&dword_1501E388;
							for( int n = 0; n < 26; n++ ){
								if( v49 <= 768 )
									break;
								for( i = v34[v37 + *(uint*)(v13 + 8) + 18 * (v36 + *(uint*)(v13 + 4) + 18 * (v38 + *(uint*)v13))]; i; i = *(uint*)(i + 4) ){
									v16 = *(uchar*)i;
									v27 = *(uchar*)(v2 + 1 + 4 * v16);
									v53 = abs(v47 - *(uchar*)(v2 + 2 + 4 * v16));
									v28 = abs(v46 - v27);
									v29 = *(uchar*)(v2 + 4 * v16);
									v54 = v28;
									v15 = abs(v45 - v29);
									if( v50 > v15 || v51 > v54 || v52 > v53 ){
										if( v49 > v54* v54 + v15* v15 + v53* v53 ){
											v50 = v15;
											v51 = v54;
											v52 = v53;
											v49 = v54 * v54 + v15 * v15 + v53 * v53;
											v48 = v16;
										}
									}
								}
								v13 += 12;
							}
						}
						v7 = v48;
					}
					v30 = v43;
					*(uchar*)(v42 + *(uint*)(v33 + 68) + v40) = v7;
					*(uchar*)(v30 + *(uint*)(v33 + 68) + v39) = v7;
					v31 = v42 + 1;
					v44 += 4;
					v43 += 256;
					++v42;
				}while( v31 <= v39 );
			}
			v32 = v40 + 256;
			v41 += 4;
			v40 += 256;
			++v39;
		}while( v32 < 65536 );
		result = 1;
	}
/*
sub_15019EF0	proc near		; CODE XREF: storm_454+4DA
					; storm_457+14Fp

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8
arg_8		= dword	ptr  0Ch
arg_C		= dword	ptr  10h
arg_10		= dword	ptr  14h
arg_14		= dword	ptr  18h
arg_18		= dword	ptr  1Ch
arg_1C		= dword	ptr  20h
arg_20		= dword	ptr  24h
arg_24		= dword	ptr  28h
arg_28		= dword	ptr  2Ch
arg_2C		= dword	ptr  30h
arg_30		= dword	ptr  34h
arg_34		= dword	ptr  38h
arg_38		= dword	ptr  3Ch
arg_436		= byte ptr  43Ah
arg_438		= dword	ptr  43Ch
arg_836		= byte ptr  83Ah
arg_838		= dword	ptr  83Ch
arg_C38		= byte ptr  0C3Ch
arg_1438	= dword	ptr  143Ch

		mov	eax, 6F5Ch
		call	sub_1501B058
		push	ebx
		push	ebp
		push	esi
		mov	esi, ecx
		push	edi
		mov	ebx, edx
		mov	eax, [esi+4Ch]
		mov	[esp+10h+arg_20], esi
		test	eax, eax
		jnz	short loc_15019F29
		push	0
		push	86h
		push	offset aSvid_cpp ; "SVID.CPP"
		push	10000h
		call	storm_401_SMemAlloc
		mov	[esi+4Ch], eax
		mov	[esi+2Ch], eax

loc_15019F29:				; CODE XREF: sub_15019EF0+1B
		mov	eax, dword_15021BBC
		test	eax, eax
		jnz	short loc_15019F5A
		mov	dword_15021BBC,	1
		mov	eax, 0FFFFFF01h
		mov	ecx, offset unk_150213A8

loc_15019F46:				; CODE XREF: sub_15019EF0+68
		mov	edx, eax
		add	ecx, 4
		imul	edx, eax
		mov	[ecx-4], edx
		inc	eax
		cmp	ecx, offset unk_15021BA0
		jle	short loc_15019F46

loc_15019F5A:				; CODE XREF: sub_15019EF0+40
		lea	esi, [esp+10h+arg_436]
		lea	eax, [ebx+2]
		lea	edi, [esp+10h+arg_836]
		sub	esi, ebx
		lea	ecx, [esp+10h+arg_38]
		mov	[esp+10h+arg_1C], eax
		sub	edi, ebx
		mov	ebp, 100h

loc_15019F7C:				; CODE XREF: sub_15019EF0+C2
		xor	edx, edx
		add	ecx, 4
		mov	dl, [eax-2]
		add	eax, 4
		lea	edx, unk_150217A4[edx*4]
		mov	[ecx-4], edx
		xor	edx, edx
		mov	dl, [eax-5]
		lea	edx, unk_150217A4[edx*4]
		mov	[esi+eax-4], edx
		xor	edx, edx
		mov	dl, [eax-4]
		dec	ebp
		lea	edx, unk_150217A4[edx*4]
		mov	[edi+eax-4], edx
		jnz	short loc_15019F7C
		mov	ecx, 16C8h
		xor	eax, eax
		lea	edi, [esp+10h+arg_1438]
		xor	edx, edx
		rep stosd
		lea	ebp, [esp+10h+arg_C38]
		mov	edi, ebx

loc_15019FCF:				; CODE XREF: sub_15019EF0+14B
		test	edx, edx
		jz	short loc_15019FF0
		mov	al, [edi]
		mov	cl, [edi-4]
		cmp	al, cl
		jnz	short loc_15019FF0
		mov	cl, [edi+1]
		mov	al, [edi-3]
		cmp	cl, al
		jnz	short loc_15019FF0
		mov	al, [edi+2]
		mov	cl, [edi-2]
		cmp	al, cl
		jz	short loc_1501A02E

loc_15019FF0:				; CODE XREF: sub_15019EF0+E1
					; sub_15019EF0+EAj sub_15019EF0+F4
		xor	eax, eax
		xor	ecx, ecx
		mov	al, [edi]
		mov	cl, [edi+1]
		shr	eax, 4
		inc	eax
		xor	ebx, ebx
		mov	bl, [edi+2]
		mov	[ebp+4], dl
		shr	ecx, 4
		inc	ecx
		lea	eax, [eax+eax*8]
		mov	esi, ebx
		lea	eax, [ecx+eax*2]
		shr	esi, 4
		inc	esi
		lea	ecx, [eax+eax*8]
		lea	eax, [esi+ecx*2]
		mov	ecx, [esp+eax*4+10h+arg_1438]
		mov	[ebp+0], ecx
		lea	eax, [esp+eax*4+10h+arg_1438]
		mov	[eax], ebp

loc_1501A02E:				; CODE XREF: sub_15019EF0+FE
		inc	edx
		add	edi, 4
		add	ebp, 8
		cmp	edx, 100h
		jb	short loc_15019FCF
		mov	eax, [esp+10h+arg_1C]
		xor	esi, esi
		xor	ecx, ecx
		mov	[esp+10h+arg_28], esi
		mov	[esp+10h+arg_4], ecx
		mov	[esp+10h+arg_18], esi
		mov	[esp+10h+arg_10], eax

loc_1501A055:				; CODE XREF: sub_15019EF0+3AC
		xor	edx, edx
		cmp	esi, edx
		mov	[esp+10h+arg_14], edx
		jbe	loc_1501A268
		mov	ebp, [esp+10h+arg_1C]
		mov	[esp+10h+arg_24], edx
		mov	[esp+10h+arg_C], ebp
		jmp	short loc_1501A081
; ---------------------------------------------------------------------------

loc_1501A071:				; CODE XREF: sub_15019EF0+366
		mov	ebp, [esp+10h+arg_C]
		mov	eax, [esp+10h+arg_10]
		mov	edx, [esp+10h+arg_14]
		mov	ecx, [esp+10h+arg_4]

loc_1501A081:				; CODE XREF: sub_15019EF0+17F
		cmp	ecx, edx
		mov	[esp+13h], cl
		ja	short loc_1501A08D
		mov	[esp+13h], dl

loc_1501A08D:				; CODE XREF: sub_15019EF0+197
		xor	edx, edx
		xor	ecx, ecx
		mov	dl, [ebp-2]
		mov	cl, [eax-2]
		add	edx, ecx
		xor	ecx, ecx
		mov	cl, [ebp-1]
		mov	[esp+10h+arg_0], 0FFFFFFFFh
		mov	esi, ecx
		xor	ecx, ecx
		mov	cl, [eax-1]
		add	esi, ecx
		xor	ecx, ecx
		mov	cl, [eax]
		xor	eax, eax
		mov	al, [ebp+0]
		mov	edi, ecx
		shr	edx, 1
		mov	ebx, edx
		add	edi, eax
		shr	esi, 1
		shr	ebx, 4
		shr	edi, 1
		mov	eax, esi
		inc	ebx
		shr	eax, 4
		inc	eax
		lea	ecx, [ebx+ebx*8]
		mov	[esp+10h+arg_34], eax
		mov	ebp, edi
		lea	eax, [eax+ecx*2]
		mov	[esp+10h+arg_30], ebx
		shr	ebp, 4
		inc	ebp
		lea	eax, [eax+eax*8]
		mov	[esp+10h+arg_2C], ebp
		lea	ecx, [ebp+eax*2+0]
		shl	edx, 2
		mov	ebp, [esp+ecx*4+10h+arg_1438]
		shl	esi, 2
		shl	edi, 2
		test	ebp, ebp
		jz	short loc_1501A15D
		cmp	dword ptr [ebp+0], 0
		jz	short loc_1501A151

loc_1501A107:				; CODE XREF: sub_15019EF0+25A
		mov	eax, [ebp+4]
		and	eax, 0FFh
		shl	eax, 2
		mov	ecx, [esp+eax+10h+arg_838]
		mov	ebx, [esp+eax+10h+arg_438]
		mov	eax, [esp+eax+10h+arg_38]
		sub	ecx, edi
		sub	ebx, esi
		sub	eax, edx
		mov	ecx, [ecx]
		add	ecx, [ebx]
		mov	ebx, [eax]
		mov	eax, [esp+10h+arg_0]
		add	ecx, ebx
		cmp	ecx, eax
		jnb	short loc_1501A145
		mov	al, [ebp+4]
		mov	[esp+10h+arg_0], ecx
		mov	[esp+13h], al

loc_1501A145:				; CODE XREF: sub_15019EF0+248
		mov	ebp, [ebp+0]
		test	ebp, ebp
		jnz	short loc_1501A107
		jmp	loc_1501A212
; ---------------------------------------------------------------------------

loc_1501A151:				; CODE XREF: sub_15019EF0+215
		mov	cl, [ebp+4]
		mov	[esp+13h], cl
		jmp	loc_1501A212
; ---------------------------------------------------------------------------

loc_1501A15D:				; CODE XREF: sub_15019EF0+20F
		mov	byte ptr [esp+10h+arg_8], 0

loc_1501A162:				; CODE XREF: sub_15019EF0+31C
		cmp	[esp+10h+arg_0], 300h
		jbe	loc_1501A212
		mov	eax, [esp+10h+arg_8]
		and	eax, 0FFh
		lea	eax, [eax+eax*2]
		shl	eax, 2
		mov	ecx, dword_1501E388[eax]
		mov	ebp, dword_1501E38C[eax]
		add	ecx, ebx
		mov	eax, dword_1501E390[eax]
		lea	ecx, [ecx+ecx*8]
		lea	ecx, [ebp+ecx*2+0]
		mov	ebp, [esp+10h+arg_34]
		add	ecx, ebp
		mov	ebp, [esp+10h+arg_2C]
		lea	ecx, [ecx+ecx*8]
		lea	ecx, [eax+ecx*2]
		add	ecx, ebp
		mov	ebp, [esp+ecx*4+10h+arg_1438]
		test	ebp, ebp
		jz	short loc_1501A200

loc_1501A1B7:				; CODE XREF: sub_15019EF0+30A
		mov	eax, [ebp+4]
		and	eax, 0FFh
		shl	eax, 2
		mov	ecx, [esp+eax+10h+arg_838]
		mov	ebx, [esp+eax+10h+arg_438]
		mov	eax, [esp+eax+10h+arg_38]
		sub	ecx, edi
		sub	ebx, esi
		sub	eax, edx
		mov	ecx, [ecx]
		add	ecx, [ebx]
		mov	ebx, [eax]
		mov	eax, [esp+10h+arg_0]
		add	ecx, ebx
		cmp	ecx, eax
		jnb	short loc_1501A1F5
		mov	[esp+10h+arg_0], ecx
		mov	cl, [ebp+4]
		mov	[esp+13h], cl

loc_1501A1F5:				; CODE XREF: sub_15019EF0+2F8
		mov	ebp, [ebp+0]
		test	ebp, ebp
		jnz	short loc_1501A1B7
		mov	ebx, [esp+10h+arg_30]

loc_1501A200:				; CODE XREF: sub_15019EF0+2C5
		mov	al, byte ptr [esp+10h+arg_8]
		inc	al
		cmp	al, 1Ah
		mov	byte ptr [esp+10h+arg_8], al
		jb	loc_1501A162

loc_1501A212:				; CODE XREF: sub_15019EF0+25C
					; sub_15019EF0+268j sub_15019EF0+27A
		mov	ecx, [esp+10h+arg_20]
		mov	eax, [esp+10h+arg_18]
		mov	edx, [esp+10h+arg_14]
		mov	bl, [esp+13h]
		mov	esi, [ecx+4Ch]
		add	eax, edx
		inc	edx
		mov	[eax+esi], bl
		mov	eax, [esp+10h+arg_24]
		mov	esi, [esp+10h+arg_4]
		mov	ecx, [ecx+4Ch]
		mov	[esp+10h+arg_14], edx
		lea	edi, [esi+eax]
		add	eax, 100h
		mov	[esp+10h+arg_24], eax
		mov	[edi+ecx], bl
		mov	edi, [esp+10h+arg_C]
		add	edi, 4
		cmp	edx, esi
		mov	[esp+10h+arg_C], edi
		jb	loc_1501A071
		mov	esi, [esp+10h+arg_18]
		mov	eax, [esp+10h+arg_10]
		mov	ecx, [esp+10h+arg_4]

loc_1501A268:				; CODE XREF: sub_15019EF0+16D
		mov	edx, [esp+10h+arg_20]
		add	esi, 100h
		add	eax, 4
		mov	[esp+10h+arg_18], esi
		mov	edi, [edx+4Ch]
		mov	edx, [esp+10h+arg_28]
		mov	[esp+10h+arg_10], eax
		mov	[edx+edi], cl
		inc	ecx
		add	edx, 101h
		cmp	esi, 10000h
		mov	[esp+10h+arg_4], ecx
		mov	[esp+10h+arg_28], edx
		jb	loc_1501A055
		pop	edi
		pop	esi
		pop	ebp
		mov	eax, 1
		pop	ebx
		add	esp, 6F5Ch
		retn
sub_15019EF0	endp ; sp-analysis failed
*/
}

//----- (1501A2C0) --------------------------------------------------------
int __stdcall SVidPlayBeginFromMemory(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, int a3, int a4, int a5, int a6, int a7, int a8)
{
	int v8;                     // ecx@5
	int v9;                     // eax@7
	int v10;                    // ecx@9
	int v11;                    // eax@11
	int v12;                    // ecx@13
	int result;                 // eax@16
	HANDLE v14;                 // esi@17
	int v15;                    // edi@19
	DWORD NumberOfBytesWritten; // [sp+10h] [bp-20Ch]@19
	CHAR FileName;              // [sp+14h] [bp-208h]@17
	char v18;                   // [sp+15h] [bp-207h]@17
	__int16 v19;                // [sp+115h] [bp-107h]@17
	char v20;                   // [sp+117h] [bp-105h]@17
	CHAR PathName[2];           // [sp+118h] [bp-104h]@17
	char v22;                   // [sp+11Ah] [bp-102h]@17
	__int16 v23;                // [sp+21Ah] [bp-2h]@17
	if( a8 )
		*(uint *)a8 = 0;
	if( a3 || !(a7 & 0x20000000) )
		v8 = -1;
	else
		v8 = 0;
	v9 = v8&(nNumberOfBytesToWrite != 0 ? -(lpBuffer != 0) : 0);
	if( a5 || !a3 )
		v10 = -1;
	else
		v10 = 0;
	v11 = v10 & v9;
	if( a6 && *(uint*)a6 != 12 )
		v12 = 0;
	else
		v12 = -1;
	if( v11 & v12 ){
		strcpy(PathName, ".");
		memset(&v22, 0, 0x100u);
		v23 = 0;
		FileName = ZeroByte;
		memset(&v18, 0, 0x100u);
		v19 = 0;
		v20 = 0;
		GetTempPathA(0x104u, PathName);
		GetTempFileNameA(PathName, "BN_", 0, &FileName);
		v14 = CreateFileA(&FileName, 0xC0000000u, 3u, 0, 2u, 0x4000100u, 0);
		if( v14 == INVALID_HANDLE_VALUE ){
			result = 0;
		}else{
			WriteFile(v14, lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, 0);
			SetFilePointer(v14, 0, 0, 0);
			v15 = SVidPlayBegin((char*)v14, a3, a4, a5, a6, a7 | 0x30000, a8);
			CloseHandle(v14);
			DeleteFileA(&FileName);
			result = v15;
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

//----- (1501A480) --------------------------------------------------------
int __stdcall SVidPlayContinue()
{
	//if( ExternalStorm ) return eSVidPlayContinue();
	int v0; // eax@1
	int v1; // edi@1
	int v2; // esi@2
	v0 = dword_15021BA8;
	v1 = 0;
	if( dword_15021BA8 ){
		do{
			v2 = *(uint*)(v0 + 4);
			if( SVidPlayContinueSingle(v0, 0, 0) )
				v1 = 1;
			v0 = v2;
		}while( v2 );
	}
	return v1;
}

// 15021BA8: using guessed type int dword_15021BA8;
//----- (1501A4B0) --------------------------------------------------------
int __stdcall SVidPlayContinueSingle(int a1, int a2, int a3)
{
	int result; // eax@4
	int v4;     // eax@6
	int v5;     // eax@7
	int v6;     // eax@11
	char* v7;   // esi@11
	int v8;     // eax@15
	char* v9;   // ecx@15
	int v10;    // ebp@15
	int v11;    // esi@15
	int v12;    // eax@16
	int v13;    // eax@17
	int v14;    // eax@23
	int v15;    // ebp@24
	DWORD v16;  // eax@34
	int v17;    // eax@35
	int v18;    // eax@37
	int v19;    // eax@46
	int v20;    // ecx@46
	char v21;   // [sp+10h] [bp-400h]@17
	char v22;   // [sp+11h] [bp-3FFh]@15
	if( a3 )
		*(uint *)a3 = 0;
	if( !a1 ){
		SErrSetLastError(0x57u);
		return 0;
	}
	if( sub_150196C0((void*)a1) ){
		v4 = *(uint*)(a1 + 88);
		if( v4 ){
			v5 = *(uint*)(v4 + 4);
			if( v5 ){
				if( !a2 && (*(int (__thiscall **)(uint, int))(dword_15021BB4 + 44))(0, v5) )
					return 1;
				v6 = *(uint*)(a1 + 88);
				v7 = 0;
				if( *(uint*)(*(uint*)(v6 + 4) + 104) ){
					if( !(*(uint*)(a1 + 128) & 0x100000) ){
						if( *(uint*)v6 )
							(*(void (__stdcall **)(uint, int, uint))(dword_15021BB4 + 4))(
							*(uint *)v6,
							*(uint *)(v6 + 4) + 108,
							0);
						v11 = *(uint*)(a1 + 88);
						v8 = 0;
						v9 = &v22;
						v10 = 256;
						do{
							v9 += 4;
							*(v9 - 5) = 4 * *(uchar*)(v8 + *(uint*)(v11 + 4) + 108);
							v12 = v8 + 1;
							*(v9 - 4) = 4 * *(uchar*)(v12++ + *(uint*)(v11 + 4) + 108);
							*(v9 - 3) = 4 * *(uchar*)(v12 + *(uint*)(v11 + 4) + 108);
							v8 = v12 + 1;
							--v10;
						}while( v10 );
						v13 = *(uint*)(a1 + 128);
						v7 = &v21;
						if( v13 & 0x200000 )
							SDrawShowCursor(0);
						if( (*(uint*)(a1 + 128) & 0x101) == 256 )
							sub_1501A760(&v21, 0.8);
						if( *(uchar*)(a1 + 128) & 2 )
							sub_15019EF0(a1, (PALETTEENTRY*)&v21);
					}
				}
				v14 = *(uint*)(a1 + 128);
				if( BYTE1(v14) & 2 ){
					v15 = (*(int(__stdcall* *)(uint))(dword_15021BB4 + 16))(*(uint*)(*(uint*)(a1 + 88) + 4));
					if( v7 )
						SDrawUpdatePalette(*(uint*)(a1 + 80), *(uint*)(a1 + 84), (PALETTEENTRY*)&v7[4 * *(uint*)(a1 + 80)], 0);
				}else{
					if( v7 )
						SDrawUpdatePalette(*(uint*)(a1 + 80), *(uint*)(a1 + 84), (PALETTEENTRY*)&v7[4 * *(uint*)(a1 + 80)], 0);
					v15 = sub_1501A880(a1);
				}
				if( a3 ){
					if( !v15 )
						*(uint *)a3 = 1;
				}
				if( *(uchar*)(a1 + 128) & 2 ){
					if( v15 ){
						v16 = GetTickCount();
						if( v16 - dword_15021BC0 >= 0x3E8 ){
							dword_15021BC0 = v16;
						}else{
							v17 = *(uint*)(a1 + 128);
							LOBYTE_IDA(v17) = v17 & 0xFD;
							*(uint*)(a1 + 128) = v17;
						}
					}
				}
				v18 = *(uint*)(a1 + 128);
				if( BYTE1(v18) & 2 ){
					if( !v15 ){
						if( v18 & 2 ){
							if( v18 & 1 )
								sub_1501A9B0(a1, v15, a1);
							else
								sub_1501AB30(a1, a1);
						}else{
							if( v18 & 1 )
								sub_1501AC60(a1, a1);
							else
								sub_1501ADA0(a1, a1);
						}
					}
				}
				++*(uint*)(*(uint*)(a1 + 88) + 8);
				v20 = *(uint*)(a1 + 88);
				v19 = *(uint*)(v20 + 4);
				if( *(uint*)(v20 + 8) < *(uint*)(v19 + 12) ){
					(*(void(__stdcall* *)(int))(dword_15021BB4 + 24))(v19);
					return 1;
				}
				if( *(uint*)(a1 + 128) & 0x40000 ){
					(*(void (__stdcall **)(int, uint))(dword_15021BB4 + 20))(v19, 0);
					result = 1;
					*(uint*)(*(uint*)(a1 + 88) + 8) = 0;
					return result;
				}
				SVidPlayEnd(a1);
			}
		}
	}
	return 0;
}

// 15019EF0: using guessed type int __fastcall sub_15019EF0(_DWORD, _DWORD);
// 15021BB4: using guessed type int dword_15021BB4;
// 15021BC0: using guessed type int dword_15021BC0;
//----- (1501A760) --------------------------------------------------------
int __thiscall sub_1501A760(void* This, long double a2)
{
	int v2;             // edi@1
	char* v3;           // esi@1
	long double v4;     // st7@2
	__int16 v5;         // fps@2
	unsigned __int8 v6; // c0@2
	unsigned __int8 v7; // c3@2
	__int64 v8;         // qax@3
	__int64 v9;         // qax@6
	int result;         // eax@9
	double v11;         // [sp+8h] [bp-10h]@2
	v3 = (char*)This + 2;
	v2 = 256;
	do{
		v11 = pow(a2, (long double)(unsigned __int8) * (v3 - 1) * 0.00390625) * 256.0;
		v4 = pow(a2, (long double)(unsigned __int8) * v3 * 0.00390625) * 256.0;
		// 		UNDEF(v5);  // что то непонятное
		if( v6 | v7 )
			v8 = (__int64)(pow(a2, (long double)(unsigned __int8) * (v3 - 2) * 0.00390625) * 256.0);
		else
			LOBYTE_IDA(v8) = -1;
		*(v3 - 2) = v8;
		if( v11 <= 255.0 )
			v9 = (__int64)v11;
		else
			LOBYTE_IDA(v9) = -1;
		*(v3 - 1) = v9;
		if( v4 <= 255.0 )
			result = (__int64)v4 & 0xFF;
		else
			result = 255;
		*v3 = result;
		v3 += 4;
		--v2;
	}while( v2 );
	return result;
}

//----- (1501A880) --------------------------------------------------------
int __thiscall sub_1501A880(int This)
{
	int v1; // edi@1
	int v2; // esi@1
	int v4; // [sp+8h] [bp-Ch]@1
	int v5; // [sp+Ch] [bp-8h]@1
	int v6; // [sp+10h] [bp-4h]@2
	v2 = This;
	v1 = 1;
	if( sub_1501A8F0(This, (int)&v4, (int)&v5) ){
		(*(void (__stdcall **)(uint, uint, uint, int, int, int, uint))(dword_15021BB4 + 36))(
			*(uint *)(*(uint *)(v2 + 88) + 4),
			0,
			0,
			v5,
			v6,
			v4,
			0);
		v1 = (*(int(__stdcall* *)(uint))(dword_15021BB4 + 16))(*(uint*)(*(uint*)(v2 + 88) + 4));
		sub_1501AEC0(v2, v1);
	}
	return v1;
}

// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501A8F0) --------------------------------------------------------
int __fastcall sub_1501A8F0(int a1, int a2, int a3)
{
	int v3;     // ebp@1
	int v4;     // esi@1
	int result; // eax@2
	int v6;     // edx@2
	int v7;     // ecx@3
	int v8;     // ecx@4
	v4 = a1;
	v3 = a2;
	if( *(uint*)(a1 + 128) & 0x10000000 ){
		*(uint*)a3 = GUI_Width;
		*(uint*)(a3 + 4) = GUI_Height;
		SDrawGetScreenSize(a3, a3 + 4, 0);
		result = SDrawLockSurface(0, (LPRECT)(v4 + 112), v4 + 124, a3, 0);
		v6 = *(uint*)(v4 + 124) + *(uint*)a3* *(uint*)(*(uint*)(v4 + 88) + 12);
		*(uint*)v3 = v6;
		if( !*(uint*)(v4 + 112) ){
			v7 = *(uint*)(v4 + 128);
			if( BYTE1(v7) & 1 ){
				v8 = GUI_Width;
				if( *(uint*)a3 <= GUI_Width )
					v8 = *(uint*)a3;
				*(uint*)v3 = v6 + ((unsigned int)(v8 - *(uint*)(*(uint*)(*(uint*)(v4 + 88) + 4) + 4)) >> 1);
			}
		}
	}else{
		*(uint*)a2 = *(uint*)(a1 + 96) + *(uint*)(a1 + 92) + *(uint*)(a1 + 116) * *(uint*)(a1 + 100);
		*(uint*)a3 = *(uint*)(a1 + 116);
		*(uint*)(a3 + 4) = *(uint*)(a1 + 120);
		result = 1;
	}
	return result;
}

// 15009BD0: using guessed type _DWORD __stdcall SDrawLockSurface(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD);
//----- (1501A9B0) --------------------------------------------------------
int __usercall sub_1501A9B0/*eax*/(int a1/*ecx*/, int a2/*ebp*/, int a3/*edi*/)
{
	int v3;           // esi@1
	int result;       // eax@2
	int v5;           // eax@3
	int v6;           // eax@6
	int v7;           // ecx@6
	int v8;           // edi@8
	int v9;           // edx@8
	int v10;          // eax@8
	int v11;          // edx@8
	int v12;          // ebp@8
	int v13;          // eax@8
	int v14;          // edi@8
	int v15;          // [sp+4h] [bp-20h]@5
	int v16;          // [sp+10h] [bp-14h]@3
	int v17;          // [sp+14h] [bp-10h]@8
	unsigned int v18; // [sp+18h] [bp-Ch]@6
	int v19;          // [sp+1Ch] [bp-8h]@3
	v3 = a1;
	if( *(uint*)(a1 + 76) ){
		v5 = *(uint*)(a1 + 88);
		*(uint*)(a1 + 20) = *(uint*)(*(uint*)(v5 + 4) + 4);
		*(uint*)(a1 + 24) = *(uint*)(*(uint*)(v5 + 4) + 8) - 1;
		*(uint*)(a1 + 36) = *(uint*)(*(uint*)(v5 + 4) + 4) + *(uint*)(*(uint*)v5 + 1096);
		*(uint*)(a1 + 40) = *(uint*)(*(uint*)v5 + 1096);
		*(uint*)(a1 + 28) = *(uint*)(*(uint*)(v5 + 4) + 4);
		*(uint*)(a1 + 32) = *(uint*)(*(uint*)(v5 + 4) + 4);
		SCodeExecute(a2, *(uint*)(dword_15021BB0 + 4), a1 + 12);
		if( sub_1501A8F0(v3, (int)&v16, (int)&v19) ){
			if( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v3 + 88) + 4), 1) ){
				v15 = a3;
				do{
					v7 = *(uint*)(v3 + 88);
					v6 = *(uint*)(v7 + 4);
					v18 = *(uint*)(v6 + 892);
					if( v18 > 1 ){
						if( *(uint*)(v6 + 896) >= 1u ){
							v8 = 2 * *(uint*)(v6 + 888);
							v9 = *(uint*)(v6 + 884);
							v10 = *(uint*)(v6 + 4);
							v17 = v9;
							v11 = v19;
							v12 = v8 * v19 + v16 + 2 * v17;
							v13 = v17 + *(uint*)(*(uint*)v7 + 1096) + v8 * v10;
							*(uint*)(v3 + 20) = v18 - 1;
							v14 = *(uint*)(*(uint*)(v7 + 4) + 896);
							*(uint*)(v3 + 36) = v12;
							*(uint*)(v3 + 24) = 2 * v14;
							*(uint*)(v3 + 40) = v13;
							*(uint*)(v3 + 28) = v11 - 2 * *(uint*)(*(uint*)(v7 + 4) + 892);
							*(uint*)(v3 + 32) = *(uint*)(*(uint*)(v7 + 4) + 4) - *(uint*)(*(uint*)(v7 + 4) + 892);
							SCodeExecute(v12, *(uint*)dword_15021BB0, v3 + 12);
						}
					}
				}while( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(
				*(uint *)(*(uint *)(v3 + 88) + 4),
				1) );
				a3 = v15;
			}
			sub_1501AEC0(v3, a3);
		}
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501AB30) --------------------------------------------------------
int __usercall sub_1501AB30/*eax*/(int a1/*ecx*/, int a2/*edi*/)
{
	int v2;           // esi@1
	int result;       // eax@2
	int v4;           // eax@6
	int v5;           // edx@6
	int v6;           // edi@8
	int v7;           // ecx@8
	int v8;           // eax@8
	int v9;           // ecx@8
	int v10;          // ebp@8
	int v11;          // eax@8
	int v12;          // edi@8
	int v13;          // [sp+0h] [bp-1Ch]@5
	int v14;          // [sp+8h] [bp-14h]@3
	int v15;          // [sp+Ch] [bp-10h]@8
	unsigned int v16; // [sp+10h] [bp-Ch]@6
	int v17;          // [sp+14h] [bp-8h]@3
	v2 = a1;
	if( *(uint*)(a1 + 76) ){
		if( sub_1501A8F0(a1, (int)&v14, (int)&v17) ){
			if( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v2 + 88) + 4), 1) ){
				v13 = a2;
				do{
					v5 = *(uint*)(v2 + 88);
					v4 = *(uint*)(v5 + 4);
					v16 = *(uint*)(v4 + 892);
					if( v16 > 1 ){
						if( *(uint*)(v4 + 896) >= 1u ){
							v6 = 2 * *(uint*)(v4 + 888);
							v7 = *(uint*)(v4 + 884);
							v8 = *(uint*)(v4 + 4);
							v15 = v7;
							v9 = v17;
							v10 = v6 * v17 + v14 + 2 * v15;
							v11 = v15 + *(uint*)(*(uint*)v5 + 1096) + v6 * v8;
							*(uint*)(v2 + 20) = v16 - 1;
							v12 = *(uint*)(*(uint*)(v5 + 4) + 896);
							*(uint*)(v2 + 36) = v10;
							*(uint*)(v2 + 24) = v12;
							*(uint*)(v2 + 40) = v11;
							*(uint*)(v2 + 28) = 2 * (v9 - *(uint*)(*(uint*)(v5 + 4) + 892));
							*(uint*)(v2 + 32) = 2 * *(uint*)(*(uint*)(v5 + 4) + 4) - *(uint*)(*(uint*)(v5 + 4) + 892);
							SCodeExecute(v10, *(uint*)dword_15021BB0, v2 + 12);
						}
					}
				}while( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(
				*(uint *)(*(uint *)(v2 + 88) + 4),
				1) );
				a2 = v13;
			}
			sub_1501AEC0(v2, a2);
		}
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501AC60) --------------------------------------------------------
int __usercall sub_1501AC60/*eax*/(int a1/*ecx*/, int a2/*edi*/)
{
	int v2;           // esi@1
	int v3;           // eax@4
	int v4;           // edx@4
	int v5;           // edi@6
	int v6;           // ebx@6
	int v7;           // ecx@6
	int v8;           // edx@6
	int v9;           // ebp@6
	int v10;          // ebx@6
	int v11;          // edx@6
	int v12;          // edi@6
	int v14;          // [sp+0h] [bp-1Ch]@3
	int v15;          // [sp+8h] [bp-14h]@1
	int v16;          // [sp+Ch] [bp-10h]@6
	unsigned int v17; // [sp+10h] [bp-Ch]@4
	int v18;          // [sp+14h] [bp-8h]@1
	v2 = a1;
	if( sub_1501A8F0(a1, (int)&v15, (int)&v18) ){
		if( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v2 + 88) + 4), 1) ){
			v14 = a2;
			do{
				v3 = *(uint*)(v2 + 88);
				v4 = *(uint*)(v3 + 4);
				v17 = *(uint*)(v4 + 892);
				if( v17 > 1 ){
					if( *(uint*)(v4 + 896) >= 1u ){
						v5 = *(uint*)(v4 + 888);
						v6 = *(uint*)(v4 + 884);
						v7 = v18;
						v8 = v5 * *(uint*)(v4 + 4);
						v16 = v6;
						v9 = v6 + v5 * v18;
						v10 = v15 + 2 * v9;
						v11 = v16 + *(uint*)(*(uint*)v3 + 1096) + 2 * v8;
						*(uint*)(v2 + 20) = v17;
						v12 = *(uint*)(*(uint*)(v3 + 4) + 896);
						*(uint*)(v2 + 36) = v10;
						*(uint*)(v2 + 24) = 2 * v12;
						*(uint*)(v2 + 40) = v11;
						*(uint*)(v2 + 28) = v7 - 2 * *(uint*)(*(uint*)(v3 + 4) + 892);
						*(uint*)(v2 + 60) = v7 - 2 * *(uint*)(*(uint*)(v3 + 4) + 892);
						*(uint*)(v2 + 32) = -*(uint*)(*(uint*)(v3 + 4) + 892);
						*(uint*)(v2 + 64) = 2 * *(uint*)(*(uint*)(v3 + 4) + 4) - *(uint*)(*(uint*)(v3 + 4) + 892);
						SCodeExecute(v9, *(uint*)(dword_15021BB0 + 8), v2 + 12);
					}
				}
			}while( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v2 + 88) + 4), 1) );
			a2 = v14;
		}
		sub_1501AEC0(v2, a2);
	}
	return 1;
}

// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501ADA0) --------------------------------------------------------
int __usercall sub_1501ADA0/*eax*/(int a1/*ecx*/, int a2/*edi*/)
{
	int v2;           // esi@1
	int v3;           // edx@4
	int v4;           // ecx@4
	int v5;           // edi@6
	int v6;           // ebx@6
	int v7;           // eax@6
	int v8;           // edx@6
	int v9;           // ebp@6
	int v10;          // ebx@6
	int v11;          // edx@6
	int v12;          // edi@6
	int v13;          // eax@6
	int v14;          // eax@6
	int v16;          // [sp+0h] [bp-1Ch]@3
	int v17;          // [sp+8h] [bp-14h]@1
	int v18;          // [sp+Ch] [bp-10h]@6
	unsigned int v19; // [sp+10h] [bp-Ch]@4
	int v20;          // [sp+14h] [bp-8h]@1
	v2 = a1;
	if( sub_1501A8F0(a1, (int)&v17, (int)&v20) ){
		if( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v2 + 88) + 4), 1) ){
			v16 = a2;
			do{
				v4 = *(uint*)(v2 + 88);
				v3 = *(uint*)(v4 + 4);
				v19 = *(uint*)(v3 + 892);
				if( v19 > 1 ){
					if( *(uint*)(v3 + 896) >= 1u ){
						v5 = *(uint*)(v3 + 888);
						v6 = *(uint*)(v3 + 884);
						v7 = v20;
						v8 = v5 * *(uint*)(v3 + 4);
						v18 = v6;
						v9 = v6 + v5 * v20;
						v10 = v17 + 2 * v9;
						v11 = v18 + *(uint*)(*(uint*)v4 + 1096) + 2 * v8;
						*(uint*)(v2 + 20) = v19;
						v12 = *(uint*)(*(uint*)(v4 + 4) + 896);
						*(uint*)(v2 + 36) = v10;
						*(uint*)(v2 + 24) = v12;
						*(uint*)(v2 + 40) = v11;
						v13 = 2 * (v7 - *(uint*)(*(uint*)(v4 + 4) + 892));
						*(uint*)(v2 + 28) = v13;
						*(uint*)(v2 + 60) = v13;
						v14 = 2 * *(uint*)(*(uint*)(v4 + 4) + 4) - *(uint*)(*(uint*)(v4 + 4) + 892);
						*(uint*)(v2 + 32) = v14;
						*(uint*)(v2 + 64) = v14;
						SCodeExecute(v9, *(uint*)(dword_15021BB0 + 8), v2 + 12);
					}
				}
			}while( (*(int (__stdcall **)(uint, int))(dword_15021BB4 + 40))(*(uint *)(*(uint *)(v2 + 88) + 4), 1) );
			a2 = v16;
		}
		sub_1501AEC0(v2, a2);
	}
	return 1;
}

// 15021BB0: using guessed type int dword_15021BB0;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501AEC0) --------------------------------------------------------
int __usercall sub_1501AEC0/*eax*/(int a1/*ecx*/, int a2/*edi*/)
{
	int v2;     // esi@1
	int result; // eax@2
	v2 = a1;
	if( *(uint*)(a1 + 128) & 0x10000000 ){
		result = SDrawUnlockSurface(a2, 0, *(uint*)(a1 + 124), 0, 0);
		*(uint*)(v2 + 124) = 0;
	}
	return result;
}

//----- (1501AEF0) --------------------------------------------------------
int __stdcall SVidPlayEnd(int a1)
{
	//if( ExternalStorm ) return eSVidPlayEnd(a1);
	int result; // eax@2
	int v2;     // eax@5
	int v3;     // eax@7
	int v4;     // ecx@9
	void* v5;   // eax@11
	void* v6;   // eax@13
	int v7;     // eax@19
	if( a1 ){
		result = sub_150196C0((void*)a1);
		if( result ){
			v2 = *(uint*)(a1 + 88);
			if( v2 ){
				v3 = *(uint*)v2;
				if( v3 )
					(*(void(__stdcall* *)(int))dword_15021BB4)(v3);
				v4 = *(uint*)(a1 + 88);
				if( *(uint*)(v4 + 4) )
					(*(void(__stdcall* *)(uint))(dword_15021BB4 + 12))(*(uint*)(v4 + 4));
				SMemFree(*(void**)(a1 + 88), "SVID.CPP", 989, 0);
				v5 = *(void**)(a1 + 8);
				*(uint*)(a1 + 88) = 0;
				if( v5 != (void*)-1 ){
					CloseHandle(v5);
					*(uint*)(a1 + 8) = -1;
				}
				v6 = *(void**)(a1 + 76);
				if( v6 ){
					SMemFree(v6, "SVID.CPP", 999, 0);
					*(uint*)(a1 + 76) = 0;
					*(uint*)(a1 + 44) = 0;
				}
				if( *(uint*)a1 ){
					*(uint*)(*(uint*)a1 + 4) = *(uint*)(a1 + 4);
				}else{
					if( dword_15021BA8 == a1 )
						dword_15021BA8 = *(uint*)(a1 + 4);
				}
				v7 = *(uint*)(a1 + 4);
				if( v7 ){
					*(uint*)v7 = *(uint*)a1;
				}else{
					if( dword_15021BAC == a1 )
						dword_15021BAC = *(uint*)a1;
				}
				*(uint*)a1 = 0;
				*(uint*)(a1 + 4) = 0;
				SMemFree((void*)a1, ".?AUVIDEOREC@@", -2, 0);
				result = 1;
			}else{
				result = 0;
			}
		}
	}else{
		SErrSetLastError(0x57u);
		result = 0;
	}
	return result;
}

// 15021BA8: using guessed type int dword_15021BA8;
// 15021BAC: using guessed type int dword_15021BAC;
// 15021BB4: using guessed type int dword_15021BB4;
//----- (1501B058) --------------------------------------------------------
int __usercall sub_1501B058/*eax*/(unsigned int a1/*eax*/, int a2/*ecx*/)
{
	char* v2;                     // ecx@1
	int v4;                       // [sp-4h] [bp-4h]@1
	int(__thiscall * v5)(uint); // [sp+0h] [bp+0h]@4
	char v6;                      // [sp+4h] [bp+4h]@1
	v4 = a2;
	v2 = &v6;
	while( a1 >= 0x1000 ){
		v2 -= 4096;
		*(uint*)v2 = *(uint*)v2;
		a1 -= 4096;
	}
	*(uint*)&v2[-a1] = *(uint*)&v2[-a1];
	return v5(v4);
}

//----- (1501B0A0) --------------------------------------------------------
int __userpurge sub_1501B0A0/*eax*/(int a1/*edx*/, int a2/*ecx*/, int a3/*ebp*/, int a4, int a5)
{
	int v5;          // eax@1
	int result;      // eax@2
	unsigned int v7; // edx@3
	int v8;          // eax@5
	*(uint*)(a4 + 40) = a2;
	*(uint*)(a4 + 44) = a1;
	*(uint*)(a4 + 36) = a5;
	*(uint*)(a4 + 28) = 2048;
	v5
	= (*(int (__fastcall **)(int, int, uint))(a4 + 40))(a4 + 8756, a4 + 28, *(uint *)(a4 + 36));
	*(uint*)(a4 + 32) = v5;
	if( (unsigned int)v5 <= 4 )
		return 3;
	*(uint*)(a4 + 4) = *(uchar*)(a4 + 8756);
	*(uint*)(a4 + 12) = *(uchar*)(a4 + 8757);
	*(uint*)(a4 + 20) = *(uchar*)(a4 + 8758);
	*(uint*)(a4 + 24) = 0;
	*(uint*)(a4 + 28) = 3;
	v7 = *(uint*)(a4 + 12);
	if( v7 < 4 || v7 > 6 ){
		result = 1;
	}else{
		*(uint*)(a4 + 16) = 65535 >> (16 - (uchar)v7);
		v8 = *(uint*)(a4 + 4);
		if( v8 ){
			if( v8 != 1 )
				return 2;
			sub_1501B700(a4 + 12212, (int)&unk_1501E5A8, 0x100u);
			sub_1501B600(a4, a3);
		}
		sub_1501B700(a4 + 12532, (int)&unk_1501E588, 0x10u);
		sub_1501B5C0(16, a4 + 12532, (int)&unk_1501E598, a4 + 11060);
		sub_1501B700(a4 + 12548, (int)&unk_1501E558, 0x10u);
		sub_1501B700(a4 + 12564, (int)&unk_1501E568, 0x20u);
		sub_1501B700(a4 + 12468, (int)&unk_1501E4D8, 0x40u);
		sub_1501B5C0(64, a4 + 12468, (int)&unk_1501E518, a4 + 10804);
		result = (unsigned int)(sub_1501B210(a4) - 774) < 1 ? 4 : 0;
	}
	return result;
}

//----- (1501B210) --------------------------------------------------------
int __thiscall sub_1501B210(int This)
{
	int v1;  // edi@1
	int v2;  // eax@2
	int v3;  // eax@5
	int v4;  // edx@6
	int v5;  // ecx@6
	int v6;  // ebp@6
	char v7; // al@7
	int v9;  // ST00_4@9
	int v11; // ST00_4@11
	int v12; // [sp+10h] [bp-8h]@2
	int v13; // [sp+14h] [bp-4h]@11
	v1 = This;
	*(uint*)(This + 8) = 4096;
	while( 1 ){
		v2 = sub_1501B300(v1);
		v12 = v2;
		if( (unsigned int)v2 >= 0x305 )
			break;
		if( (unsigned int)v2 >= 0x100 ){
			v12 -= 254;
			v3 = sub_1501B4B0(v1, v12);
			if( !v3 ){
				v12 = 774;
				break;
			}
			v6 = *(uint*)(v1 + 8);
			v4 = v6 + 48 + v1;
			v5 = v6 + 48 + v1 - v3;
			*(uint*)(v1 + 8) = v6 + v12;
			do{
				v7 = *(uchar*)v5++;
				*(uchar*)v4++ = v7;
			}while( v12-- != 1 );
		}else{
			*(uchar*)((*(uint*)(v1 + 8))++ + v1 + 48) = v2;
		}
		if( *(uint*)(v1 + 8) >= 0x2000u ){
			v9 = *(uint*)(v1 + 36);
			v12 = 4096;
			(*(void (__fastcall **)(int, int *, int))(v1 + 44))(v1 + 4144, &v12, v9);
			sub_1501B700(v1 + 48, v1 + 4144, *(uint*)(v1 + 8) - 4096);
			*(uint*)(v1 + 8) -= 4096;
		}
	}
	v11 = *(uint*)(v1 + 36);
	v13 = *(uint*)(v1 + 8) - 4096;
	(*(void (__fastcall **)(int, int *, int))(v1 + 44))(v1 + 4144, &v13, v11);
	return v12;
}

//----- (1501B300) --------------------------------------------------------
int __thiscall sub_1501B300(int This)
{
	int v1;             // esi@1
	int v3;             // edi@4
	unsigned __int8 v4; // cl@6
	int v5;             // ebx@7
	int v6;             // edi@11
	int v7;             // ecx@18
	v1 = This;
	if( *(uchar*)(This + 20) & 1 ){
		if( sub_1501B530(This, 1u) )
			return 774;
		v3 = *(uchar*)((*(uint*)(v1 + 20) & 0xFF) + v1 + 11060);
		if( sub_1501B530(v1, *(uchar*)(v1 + v3 + 12532)) )
			return 774;
		v4 = *(uchar*)(v1 + v3 + 12548);
		if( v4 ){
			v5 = *(uint*)(v1 + 20) & ((1 << v4) - 1);
			if( sub_1501B530(v1, v4) && v3 + v5 != 270 )
				return 774;
			v3 = *(ushort*)(v1 + 2 * v3 + 12564) + v5;
		}
		return v3 + 256;
	}
	if( sub_1501B530(This, 1u) )
		return 774;
	if( !*(uint*)(v1 + 4) ){
		v6 = *(uint*)(v1 + 20) & 0xFF;
		if( sub_1501B530(v1, 8u) )
			return 774;
		return v6;
	}
	v7 = *(uint*)(v1 + 20);
	if( (unsigned __int8)*(uint*)(v1 + 20) ){
		v6 = *(uchar*)(v1 + (unsigned __int8) * (uint*)(v1 + 20) + 11316);
		if( v6 == 255 ){
			if( v7 & 0x3F ){
				if( sub_1501B530(v1, 4u) )
					return 774;
				v6 = *(uchar*)((*(uint*)(v1 + 20) & 0xFF) + v1 + 11572);
			}else{
				if( sub_1501B530(v1, 6u) )
					return 774;
				v6 = *(uchar*)((*(uint*)(v1 + 20) & 0x7F) + v1 + 11828);
			}
		}
	}else{
		if( sub_1501B530(v1, 8u) )
			return 774;
		v6 = *(uchar*)((*(uint*)(v1 + 20) & 0xFF) + v1 + 11956);
	}
	if( !sub_1501B530(v1, *(uchar*)(v1 + v6 + 12212)) )
		return v6;
	return 774;
}

//----- (1501B4B0) --------------------------------------------------------
int __fastcall sub_1501B4B0(int a1, int a2)
{
	int v2;          // ebx@1
	int v3;          // edi@1
	int v4;          // esi@1
	int v6;          // edi@4
	unsigned int v7; // edx@6
	v2 = a2;
	v4 = a1;
	v3 = *(uchar*)((*(uint*)(a1 + 20) & 0xFF) + a1 + 10804);
	if( sub_1501B530(v4, *(uchar*)(v4 + v3 + 12468)) )
		return 0;
	if( v2 == 2 ){
		v6 = *(uint*)(v4 + 20) & 3 | 4 * v3;
		if( sub_1501B530(v4, 2u) )
			return 0;
	}else{
		v7 = *(uint*)(v4 + 12);
		v6 = *(uint*)(v4 + 20) & *(uint*)(v4 + 16) | (v3 << v7);
		if( sub_1501B530(v4, v7) )
			return 0;
	}
	return v6 + 1;
}

//----- (1501B530) --------------------------------------------------------
int __fastcall sub_1501B530(int a1, unsigned int a2)
{
	int v2;          // eax@1
	unsigned int v3; // ebx@1
	int v4;          // esi@1
	int v6;          // edi@3
	int v7;          // ST00_4@4
	int v8;          // eax@4
	int v9;          // edx@7
	int v10;         // edx@7
	int v11;         // eax@7
	v2 = *(uint*)(a1 + 24);
	v3 = a2;
	v4 = a1;
	if( v2 >= a2 ){
		*(uint*)(a1 + 24) = v2 - a2;
		*(uint*)(a1 + 20) >>= a2;
		return 0;
	}
	v6 = a1 + 28;
	*(uint*)(a1 + 20) >>= v2;
	if( *(uint*)(a1 + 28) == *(uint*)(a1 + 32) ){
		v7 = *(uint*)(a1 + 36);
		*(uint*)v6 = 2048;
		v8
		= (*(int (__fastcall **)(int, int, int))(a1 + 40))(a1 + 8756, a1 + 28, v7);
		*(uint*)(v4 + 32) = v8;
		if( !v8 )
			return 1;
		*(uint*)v6 = 0;
	}
	v9 = *(uchar*)(v4 + *(uint*)v6++ + 8756) << 8;
	v10 = *(uint*)(v4 + 20) | v9;
	v11 = *(uint*)(v4 + 24);
	*(uint*)(v4 + 20) = v10;
	*(uint*)(v4 + 20) = (unsigned int)v10 >> ((uchar)v3 - v11);
	*(uint*)(v4 + 24) = v11 - v3 + 8;
	return 0;
}

//----- (1501B5C0) --------------------------------------------------------
void __fastcall sub_1501B5C0(int a1, int a2, int a3, int a4)
{
	int i;           // ebx@1
	unsigned int v5; // eax@2
	char v6;         // cl@2
	for( i = a1 - 1; i >= 0; --i ){
		v6 = *(uchar*)(a2 + i);
		v5 = *(uchar*)(a3 + i);
		do{
			*(uchar*)(a4 + v5) = i;
			v5 += 1 << v6;
		}while( v5 < 0x100 );
	}
}

//----- (1501B600) --------------------------------------------------------
uint __usercall sub_1501B600/*eax*/(int a1/*ecx*/, int a2/*ebp*/)
{
	int v2;              // edx@1
	int v3;              // edi@1
	unsigned int v4;     // esi@1
	char v5;             // cl@2
	unsigned int result; // eax@3
	char v7;             // cl@7
	char v8;             // cl@8
	char v9;             // cl@11
	char v10;            // cl@14
	v3 = a1;
	v2 = 255;
	v4 = (unsigned int)&unk_1501E8A6;
	do{
		v5 = *(uchar*)a2;
		if( *(uchar*)a2 > 8u ){
			if( *(ushort*)v4 & 0xFF ){
				*(uchar*)((*(ushort*)v4 & 0xFF) + v3 + 11316) = -1;
				v7 = *(uchar*)a2;
				if( *(uchar*)v4 & 0x3F ){
					v8 = v7 - 4;
					*(uchar*)a2 = v8;
					a2 = 1 << v8;
					result = (unsigned __int16)(*(ushort*)v4 >> 4);
					do{
						*(uchar*)(v3 + result + 11572) = v2;
						result += a2;
					}while( result < 0x100 );
				}else{
					v9 = v7 - 6;
					*(uchar*)a2 = v9;
					a2 = 1 << v9;
					result = (unsigned __int16)(*(ushort*)v4 >> 6);
					do{
						*(uchar*)(v3 + result + 11828) = v2;
						result += a2;
					}while( result < 0x80 );
				}
			}else{
				v10 = v5 - 8;
				*(uchar*)a2 = v10;
				a2 = 1 << v10;
				result = (unsigned __int16)(*(ushort*)v4 >> 8);
				do{
					*(uchar*)(v3 + result + 11956) = v2;
					result += a2;
				}while( result < 0x100 );
			}
		}else{
			a2 = 1 << v5;
			result = *(ushort*)v4;
			do{
				*(uchar*)(v3 + result + 11316) = v2;
				result += a2;
			}while( result < 0x100 );
		}
		v4 -= 2;
		--v2;
	}while( v4 >= (unsigned int)&unk_1501E6A8 );
	return result;
}

//----- (1501B700) --------------------------------------------------------
unsigned int __fastcall sub_1501B700(int a1, int a2, unsigned int a3)
{
	unsigned int i;      // esi@1
	int v4;              // edi@2
	unsigned int result; // eax@3
	char v6;             // bl@4
	for( i = a3 >> 2; i; --i ){
		v4 = *(uint*)a2;
		a2 += 4;
		*(uint*)a1 = v4;
		a1 += 4;
	}
	for( result = a3 & 3; result; --result ){
		v6 = *(uchar*)a2++;
		*(uchar*)a1++ = v6;
	}
	return result;
}