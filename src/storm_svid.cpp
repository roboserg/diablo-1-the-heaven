// storm.dll sources

#include "storm.h"
#include "smack.h"

HMODULE smack32_dll =0;

typedef void			(__stdcall *tSmackBufferClose)			( SmackBuf PTR4* sbuf);
typedef void			(__stdcall *tSmackBufferNewPalette)		( SmackBuf PTR4* sbuf, const void PTR4* pal, u32 paltype );
typedef SmackBuf PTR4*	(__stdcall *tSmackBufferOpen)			( HWND wnd, u32 BlitType, u32 width, u32 height, u32 ZoomW, u32 ZoomH );
typedef void			(__stdcall *tSmackClose)				( Smack PTR4* smk);
typedef u32				(__stdcall *tSmackDoFrame)				( Smack PTR4* smk);
typedef void			(__stdcall *tSmackGoto)					( Smack PTR4* smk,u32 frame);
typedef void			(__stdcall *tSmackNextFrame)			( Smack PTR4* smk);
typedef Smack PTR4*		(__stdcall *tSmackOpen)					( const char PTR4* name,u32 flags,u32 extrabuf);
typedef u8				(__stdcall *tSmackSoundUseDirectSound)	( void PTR4* dd);
typedef void			(__stdcall *tSmackToBuffer)				( Smack PTR4* smk,u32 left,u32 top,u32 Pitch,u32 destheight,const void PTR4* buf,u32 Flags);
typedef u32				(__stdcall *tSmackToBufferRect)			( Smack PTR4* smk, u32 SmackSurface);
typedef u32				(__stdcall *tSmackWait)					( Smack PTR4* smk);
tSmackBufferClose			_SmackBufferClose;			// 0
tSmackBufferNewPalette		_SmackBufferNewPalette;		// 1
tSmackBufferOpen			_SmackBufferOpen;			// 2
tSmackClose					_SmackClose;				// 3
tSmackDoFrame				_SmackDoFrame;				// 4
tSmackGoto					_SmackGoto;					// 5
tSmackNextFrame				_SmackNextFrame;			// 6
tSmackOpen					_SmackOpen;					// 7
tSmackSoundUseDirectSound	_SmackSoundUseDirectSound;	// 8
tSmackToBuffer				_SmackToBuffer;				// 9
tSmackToBufferRect			_SmackToBufferRect;			// 10
tSmackWait					_SmackWait;					// 11


// extern "C" int dword_15021BB0;
// extern "C" int dword_15021BA8;
// extern "C" int dword_15021BAC;
// extern "C" char ZeroByte;
// extern "C" int OpenedMpqArchives;
// extern "C" int dword_1501F0D4;		// MpqDecryptBuffer
// extern "C" int dword_1501F0D8;
// extern "C" __int16 word_1501CAFC;
// extern "C" int dword_15021BC0;

//char *__fastcall sub_1500DDD0( int a1, char *a2);

//----- (150196E0) --------------------------------------------------------
// int __stdcall SVidInitialize( int a1) 
// {
// 	int v3; // eax@31
// 	int result; // eax@36
// 	int v5; // eax@32
// 
// 	int v6 = 0;
// 	SDrawGetServiceLevel( (int)&v6);
// 	if( v6 < 1 ){
// 		return 0;
// 	}
// 	if( !smack32_dll ){
// 		smack32_dll =LoadLibraryA( "smackw32.dll");
// 		if( !smack32_dll ){
// 			return 0;
// 		}
// 	}
// 	_SmackBufferClose			=(tSmackBufferClose)GetProcAddress( smack32_dll, "_SmackBufferClose@4");
// 	if( !_SmackBufferClose)		return 0;
// 	_SmackBufferNewPalette		=(tSmackBufferNewPalette)GetProcAddress( smack32_dll, "_SmackBufferNewPalette@12");
// 	if( !_SmackBufferNewPalette) return 0;
// 	_SmackBufferOpen			=(tSmackBufferOpen)GetProcAddress( smack32_dll, "_SmackBufferOpen@24");
// 	if( !_SmackBufferOpen)		return 0;
// 	_SmackClose					=(tSmackClose)GetProcAddress( smack32_dll, "_SmackClose@4");
// 	if( !_SmackClose)			return 0;
// 	_SmackDoFrame				=(tSmackDoFrame)GetProcAddress( smack32_dll, "_SmackDoFrame@4");
// 	if( !_SmackDoFrame)			return 0;
// 	_SmackGoto					=(tSmackGoto)GetProcAddress( smack32_dll, "_SmackGoto@8");
// 	if( !_SmackGoto)			return 0;
// 	_SmackNextFrame				=(tSmackNextFrame)GetProcAddress( smack32_dll, "_SmackNextFrame@4");
// 	if( !_SmackNextFrame)		return 0;
// 	_SmackOpen					=(tSmackOpen)GetProcAddress( smack32_dll, "_SmackOpen@12");
// 	if( !_SmackOpen)			return 0;
// 	_SmackSoundUseDirectSound	=(tSmackSoundUseDirectSound)GetProcAddress( smack32_dll, "_SmackSoundUseDirectSound@4");
// 	if( !_SmackSoundUseDirectSound) return 0;
// 	_SmackToBuffer				=(tSmackToBuffer)GetProcAddress( smack32_dll, "_SmackToBuffer@28");
// 	if( !_SmackToBuffer)		return 0;
// 	_SmackToBufferRect			=(tSmackToBufferRect)GetProcAddress( smack32_dll, "_SmackToBufferRect@8");
// 	if( !_SmackToBufferRect)	return 0;
// 	_SmackWait					=(tSmackWait)GetProcAddress( smack32_dll, "_SmackWait@4");
// 	if( !_SmackWait)			return 0;
// 
// 	v3 = dword_15021BB0;
// 	if( !dword_15021BB0 ){
// 		v5 = SMemAlloc( 12, "SVID.CPP", 560, dword_15021BB0);
// 		dword_15021BB0 = v5;
// 		SCodeCompile( (int)"1 W2=S W1=W2 2 D=W", "1 W1=W2 W2=S W1=TW 2 D=W", 0, 0x140u, 0, v5);
// 		SCodeCompile( 0, "1 W1=S W2=SC D=TW", 0, 0x140u, 0, dword_15021BB0 + 4);
// 		SCodeCompile( 0, "1 W1=S W2=W1 2 D=W", 0, 0x140u, 0x4000000u, dword_15021BB0 + 8);
// 		v3 = dword_15021BB0;
// 	}
// 	if( *(_DWORD *)v3 && *(_DWORD *)(v3 + 4) && *(_DWORD *)(v3 + 8) ){
// 		_SmackSoundUseDirectSound( (void*)a1);
// 		return 1;
// 	}
// 	return 0;
// }

//----- (15019570) --------------------------------------------------------
// int __stdcall SVidDestroy() {
//   int i; // eax@1
// 
//   for( i = dword_15021BA8; dword_15021BA8; i = dword_15021BA8 ){
//     SVidPlayEnd( i);
//   }
//   if( dword_15021BB0 )
//   {
//     SCodeDelete( *(_DWORD *)dword_15021BB0);
//     SCodeDelete( *(_DWORD *)(dword_15021BB0 + 4));
//     SCodeDelete( *(_DWORD *)(dword_15021BB0 + 8));
//     SMemFree( (void*)dword_15021BB0, "SVID.CPP", 500, 0);
//     dword_15021BB0 = 0;
//   }
//   if( smack32_dll ){
//     FreeLibrary( smack32_dll);
//     smack32_dll = 0;
//   }
//   return 1;
// }

//----- (1500E5E0) --------------------------------------------------------
// int __stdcall sub_1500E5E0(char *Str, int a2)
// {
//   int v2; // ebx@1
//   char v3; // dl@3
//   char *v4; // ebp@3
//   int result; // eax@4
//   char *v6; // ebx@16
//   int v7; // edi@16
//   int i; // esi@16
//   char *v9; // ebx@19
//   int j; // esi@19
//   char *v11; // ebx@22
//   int v12; // ebp@22
//   int v13; // esi@22
//   int v14; // eax@25
//   int k; // edx@25
//   int v16; // ecx@25
//   int v17; // ebx@25
//   int v18; // esi@25
//   int v19; // ecx@29
//   int v20; // ecx@35
//   int v21; // esi@40
//   int v22; // eax@41
//   HANDLE v23; // eax@43
//   HANDLE v24; // eax@10
//   int v25; // ST18_4@18
//   char v26; // al@18
//   int v27; // ST18_4@21
//   char v28; // al@21
//   int v29; // ecx@21
//   int v30; // ST18_4@24
//   char v31; // al@24
//   int v32; // edi@25
//   CHAR FileName; // [sp+24h] [bp-104h]@5
//   char v34; // [sp+25h] [bp-103h]@5
//   __int16 v35; // [sp+125h] [bp-3h]@5
//   char v36; // [sp+127h] [bp-1h]@5
//   int v37; // [sp+10h] [bp-118h]@14
//   int v38; // [sp+20h] [bp-108h]@16
//   int v39; // [sp+14h] [bp-114h]@19
//   int v40; // [sp+18h] [bp-110h]@25
//   int v41; // [sp+1Ch] [bp-10Ch]@25
// 
//   v2 = a2;
//   if( a2 )
//     *(_DWORD *)a2 = -1;
//   v4 = Str;
//   v3 = *Str;
//   if( -(*Str != 0) & -(a2 != 0) & -(Str != 0) )
//   {
//     FileName = ZeroByte;
//     memset(&v34, 0, 0x100u);
//     v35 = 0;
//     v36 = 0;
//     if( v3 == 92 || strstr(Str, ":\\") || strstr(v4, "\\\\") )
//       strcpy((char *)&FileName, v4);
//     else
//       sub_1500DDD0((int)v4, (char *)&FileName);
//     v24 = CreateFileA(&FileName, 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
//     *(_DWORD*)v2 = (_DWORD)v24;
//     if( v24 == (HANDLE)-1 )
//     {
//       if( OpenedMpqArchives && dword_1501F0D4 )
//       {
//         v37 = OpenedMpqArchives;
//         while( 1 )
//         {
//           v6 = v4;
//           v38 = dword_1501F0D8;
//           v7 = 2146271213;
//           for( i = -286331154; v6; i += 32 * i + v7 + v26 + 3 )
//           {
//             if( !*v6 )
//               break;
//             v25 = *v6++;
//             v26 = toupper(v25);
//             v7 = (i + v7) ^ *(_DWORD *)(dword_1501F0D4 + 4 * v26);
//           }
//           v9 = v4;
//           v39 = 2146271213;
//           for( j = -286331154; v9; j += 32 * j + v29 + v28 + 3 )
//           {
//             if( !*v9 )
//               break;
//             v27 = *v9++;
//             v28 = toupper(v27);
//             v29 = (j + v39) ^ *(_DWORD *)(dword_1501F0D4 + 4 * v28 + 1024);
//             v39 = (j + v39) ^ *(_DWORD *)(dword_1501F0D4 + 4 * v28 + 1024);
//           }
//           v11 = v4;
//           v12 = 2146271213;
//           v13 = -286331154;
//           if( Str )
//           {
//             do
//             {
//               if( !*v11 )
//                 break;
//               v30 = *v11++;
//               v31 = toupper(v30);
//               v12 = (v13 + v12) ^ *(_DWORD *)(dword_1501F0D4 + 4 * v31 + 2048);
//               v13 += 32 * v13 + v12 + v31 + 3;
//             }
//             while( v11 );
//           }
//           v40 = -1;
//           v18 = *(_DWORD *)(v37 + 312);
//           v17 = *(_DWORD *)(*(_DWORD *)(v37 + 308) + 24) - 1;
//           v32 = v17 & v7;
//           v14 = v32;
//           v41 = v32;
//           v16 = v18 + 16 * v32;
//           for( k = *(_DWORD *)(v16 + 12); k != -1; k = *(_DWORD *)(16 * v14 + v18 + 12) )
//           {
//             if( *(_DWORD *)v16 == v39 && *(_DWORD *)(v16 + 4) == v12 && k != -2 )
//             {
//               v19 = *(_DWORD *)(v16 + 8);
//               if( v19 == v38 )
//                 goto LABEL_35;
//               if( !v19 )
//                 v40 = v14;
//             }
//             v14 = v17 & (v14 + 1);
//             if( v14 == v41 )
//               break;
//             v16 = 16 * v14 + v18;
//           }
//           v14 = v40;
// LABEL_35:
//           v20 = v37;
//           if( v14 == -1 )
//           {
//             v20 = *(_DWORD *)(v37 + 4);
//             v37 = *(_DWORD *)(v37 + 4);
//           }
//           if( !v20 )
//             break;
//           if( v14 != -1 )
//           {
//             if( !v37 )
//               break;
//             v21 = *(_DWORD *)(v37 + 304) + 16 * *(_DWORD *)(16 * v14 + *(_DWORD *)(v37 + 312) + 12);
//             if( *(_DWORD *)(v21 + 8) && (v22 = *(_DWORD *)(v21 + 12), !(v22 & 0x10100)) && v22 & 0x80000000 )
//             {
//               v23 = CreateFileA((LPCSTR)(v37 + 8), 0x80000000u, 1u, 0, 3u, 0x8000000u, 0);
//               *(_DWORD *)a2 = (_DWORD)v23;
//               if( v23 == (HANDLE)-1 )
//                 goto LABEL_47;
//               SetFilePointer(v23, *(_DWORD *)v21, 0, 0);
//               result = 1;
//             }
//             else
//             {
//               SErrSetLastError( 0x3EEu);
//               result = 0;
//             }
//             return result;
//           }
//           v4 = Str;
//         }
//       }
//       SErrSetLastError( 2u);
// LABEL_47:
//       result = 0;
//     }
//     else
//     {
//       result = 1;
//     }
//   }
//   else
//   {
//     SErrSetLastError( 0x57u);
//     result = 0;
//   }
//   return result;
// }

//----- (15019EF0) --------------------------------------------------------
// original function can't be C-disassempled bcoz positive sp values found.
// this function isn't from hellfire, its from diablo dll
// void sub_15019EF0( int a1, PALETTEENTRY* a2) 

//----- (150199C0) --------------------------------------------------------
//int __stdcall SVidPlayBegin( char *Str, int a2, int a3, int a4, int a5, int a6, int a7)
//  {
//   int v7; // ebx@3
//   int v8; // ecx@5
//   int v9; // eax@7
//   int v10; // ecx@9
//   int v11; // eax@11
//   int v12; // ebp@11
//   int v13; // ecx@13
//   __int16 v15; // ax@19
//   int v16; // edi@20
//   unsigned __int32 v17; // esi@21
//   int v18; // eax@32
//   int v19; // esi@32
//   unsigned int v20; // edi@35
//   int v21; // eax@41
//   int v22; // eax@43
//   int v23; // ebx@48
//   char *v24; // eax@53
//   int v25; // eax@56
//   int v26; // ecx@56
//   int v27; // eax@58
//   DWORD v28; // ebp@21
//   int v29; // eax@23
//   int v30; // ebx@25
//   int v31; // eax@35
//   int v32; // ebx@35
//   int v33; // edx@41
//   char v34; // zf@41
//   int v35; // eax@42
//   int v36; // eax@58
//   int v37; // ecx@58
//   unsigned int v38; // eax@64
//   HWND v39; // eax@66
//   HDC v40; // eax@66
//   HDC v41; // ebx@66
//   HWND v42; // eax@66
//   int v43; // edx@67
//   int v44; // [sp+4Ch] [bp-404h]@18
//   int Data; // [sp+3Ch] [bp-414h]@18
//   int v46; // [sp+44h] [bp-40Ch]@20
//   int v47; // [sp+48h] [bp-408h]@20
//   int v48; // [sp+38h] [bp-418h]@20
//   int v49; // [sp+40h] [bp-410h]@20
//   PALETTEENTRY pPalEntries; // [sp+50h] [bp-400h]@66
// 
//   if( a7 )
//     *(_DWORD *)a7 = 0;
//   v7 = a6;
//   if( a2 || !(a6 & 0x20000000) )
//     v8 = -1;
//   else
//     v8 = 0;
//   v9 = v8 & -(a7 != 0) & -(Str != 0);
//   if( a4 || !a2 )
//     v10 = -1;
//   else
//     v10 = 0;
//   v12 = a5;
//   v11 = v10 & v9;
//   if( a5 && *(_DWORD *)a5 != 12 )
//     v13 = 0;
//   else
//     v13 = -1;
//   if( !(v13 & v11) )
//   {
//     SErrSetLastError( 0x57u);
//     return 0;
//   }
//   if( a6 & 0x808 )
//   {
//     v44 = 0;
//     Data = 0;
//     SRegLoadValue( "Video Player", "Version", 0, (int)&v44);
//     SRegLoadValue( "Video Player", "Mode", 0, (int)&Data);
//     if( v44 != 3 || (v15 = Data, !Data) )
//     {
//       Sleep( 0x1F4);
//       SDrawGetScreenSize( (int)&v46, (int)&v47, 0);
//       v16 = SMemAlloc( v46 * v47, "SVID.CPP", 622, 0);
//       if( SDrawLockSurface( 0, 0, (int)&v48, (int)&v49, 0) ){
//         v28 = GetTickCount();
//         SBltROP3( v16, v48, v46, v47, v46, v49, 0, 13369376);
//         SBltROP3( v48, v16, v46, v47, v49, v46, 0, 13369376);
//         v17 = GetTickCount() - v28;
//         SDrawUnlockSurface( v7, v16, 0, v48, 0, 0);
//         v12 = a5;
//       }
//       else
//       {
//         v17 = -1;
//       }
//       SMemFree( (void*)v16, "SVID.CPP", 648, 0);
//       v29 = -(v17 > 0x46);
//       LOBYTE(v29) = v29 & 0xFE;
//       Data = v29 + 514;
//       SRegSaveValue( "Video Player", "Version", 0, 3);
//       SRegSaveValue( "Video Player", "Benchmark", 0, v17);
//       SRegSaveValue( "Video Player", "Mode", 0, Data);
//       v15 = Data;
//     }
//     if( BYTE1(v7) & 8 )
//     {
//       BYTE1(v30) = BYTE1(v7) & 0xF4;
//       v7 = v15 & 0x300 | v30;
//     }
//     if( v7 & 8 )
//       v7 = v7 & 0xFFFFFFF4 | v15 & 3;
//   }
//   if( (!smack32_dll || !dword_15021BB0) && !SVidInitialize(0) )
//     return 0;
//   v18 = SMemAlloc( 132, ".?AUVIDEOREC@@", -2, 8);
//   v19 = v18;
//   *(_DWORD *)v18 = dword_15021BAC;
//   if( dword_15021BA8 )
//   {
//     *(_DWORD *)(dword_15021BAC + 4) = v18;
//     dword_15021BAC = v18;
//   }
//   else
//   {
//     dword_15021BAC = v18;
//     dword_15021BA8 = v18;
//   }
//   v31 = v7;
//   BYTE1(v32) = BYTE1(v7) & 0xFE;
//   *(_DWORD *)(v19 + 8) = -1;
//   v20 = (v32 | ((unsigned int)~v31 >> 1) & 0x100) & 0xEFFFFFFF | (~(v32 | ((unsigned int)~v31 >> 1) & 0x100) >> 1) & 0x10000000;
//   if( v20 & 0x100 )
//     v20 &= 0xFFFFFFFCu;
//   if( v20 & 0x20000000 )
//     v20 &= 0xFFF7FFFFu;
//   if( v20 & 0x10000000 )
//     a2 = 0;
//   v21 = a3;
//   v33 = a2;
//   v34 = a3 == 0;
//   *(_DWORD *)(v19 + 128) = v20;
//   *(_DWORD *)(v19 + 92) = v33;
//   if( !v34 )
//   {
//     *(_DWORD *)(v19 + 96) = *(_DWORD *)v21;
//     *(_DWORD *)(v19 + 100) = *(_DWORD *)(v21 + 4);
//     *(_DWORD *)(v19 + 104) = *(_DWORD *)(v21 + 8);
//     v35 = *(_DWORD *)(v21 + 12);
//     *(_DWORD *)(v19 + 112) = v19 + 96;
//     *(_DWORD *)(v19 + 108) = v35;
//   }
//   v22 = a4;
//   if( a4 )
//   {
//     *(_DWORD *)(v19 + 116) = *(_DWORD *)a4;
//     *(_DWORD *)(v19 + 120) = *(_DWORD *)(v22 + 4);
//   }
//   if( v12 )
//   {
//     *(_DWORD *)(v19 + 80) = *(_DWORD *)(v12 + 4);
//     *(_DWORD *)(v19 + 84) = *(_DWORD *)(v12 + 8);
//   }
//   else
//   {
//     *(_DWORD *)(v19 + 80) = 1;
//     *(_DWORD *)(v19 + 84) = 254;
//   }
//   *(_DWORD *)(v19 + 88) = SMemAlloc( 16, "SVID.CPP", 713, 8);
//   v23 = 1044480;
//   v48 = -1;
//   if( v20 & 0x20000 )
//     v23 = 1044992;
//   if( v20 & 0x400000 )
//     BYTE1(v23) |= 4u;
//   if( v20 & 0x10000 )
//   {
//     v24 = Str;
//     v48 = (int)Str;
//   }
//   else
//   {
//     if( !sub_1500E5E0(Str, (int)&v48) )
//     {
// LABEL_70:
//       SVidPlayEnd( v19);
//       return 0;
//     }
//     v24 = (char *)v48;
//     *(_DWORD *)(v19 + 8) = v48;
//   }
//   *(_DWORD *)(*(_DWORD *)(v19 + 88) + 4) = (_DWORD)_SmackOpen( v24, v23, -1);
//   v25 = *(_DWORD *)(v19 + 88);
//   v26 = *(_DWORD *)(v25 + 4);
//   if( !v26 )
//     goto LABEL_70;
//   if( !(v20 & 0x200) )
//     goto LABEL_60;
//   v36 = *(_DWORD *)(v25 + 4);
//   LOWORD(v37) = *(_WORD *)(v26 + 8);
//   LOWORD(v36) = 2 * *(_WORD *)(v36 + 4);
//   **(_DWORD **)(v19 + 88) = (_DWORD)_SmackBufferOpen( 0, 3, v36, v37, v36, v37);
//   v27 = *(_DWORD *)(v19 + 88);
//   if( !*(_DWORD *)v27 ){
//     goto LABEL_70;
//   }
//   _SmackToBuffer( (Smack*)*(_DWORD *)(v27 + 4), 0, 0,
// 		2 * *(_DWORD *)(*(_DWORD *)(v27 + 4) + 4),
// 		*(_DWORD *)(*(_DWORD *)(v27 + 4) + 8),
// 		(const void*)*(_DWORD *)(*(_DWORD *)v27 + 1096),
// 		0);
// LABEL_60:
//   if( v20 & 0x80000 && SDrawGetScreenSize( 0, (int)&v49, 0) )
//     *(_DWORD *)(*(_DWORD *)(v19 + 88) + 12) = (unsigned int)(v49
//                                                            - (*(_DWORD *)(*(_DWORD *)(*(_DWORD *)(v19 + 88) + 4) + 8) << ((v20 >> 9) & 1))) >> 1;
//   else
//     *(_DWORD *)(*(_DWORD *)(v19 + 88) + 12) = 0;
//   v38 = v20;
//   if( (v38 & 0x100002) == 1048578 )
//   {
//     if( SDrawGetFrameWindow(0) )
//     {
//       v39 = SDrawGetFrameWindow(0);
//       v40 = GetDC(v39);
//       v41 = v40;
//       GetSystemPaletteEntries( v40, 0, 0x100u, &pPalEntries);
//       v42 = SDrawGetFrameWindow( 0);
//       ReleaseDC( v42, v41);
//       sub_15019EF0( v19, &pPalEntries);
//     }
//   }
//   v43 = a7;
//   *(_DWORD *)(v19 + 12) = 64;
//   *(_DWORD *)v43 = v19;
//   if( v20 & 0x200000 )
//     SDrawShowCursor(0);
//   return 1;
// }

//----- (150196C0) --------------------------------------------------------
// int __fastcall sub_150196C0(int a1)
// {
//   int result; // eax@1
// 
//   result = dword_15021BA8;
//   if( dword_15021BA8 )
//   {
//     while( result != a1 )
//     {
//       result = *(_DWORD *)(result + 4);
//       if( !result )
//         return result;
//     }
//     result = 1;
//   }
//   else
//   {
//     result = 0;
//   }
//   return result;
// }

//----- (15019620) --------------------------------------------------------
// int __stdcall SVidGetSize( int a1, int a2, int a3, int a4) 
// {
//   int v4; // ebx@1
//   int v5; // edi@3
//   int v6; // ebp@5
//   int v7; // esi@7
//   int result; // eax@8
// 
//   v4 = a2;
//   if( a2 )
//     *(_DWORD *)a2 = 0;
//   v5 = a3;
//   if( a3 )
//     *(_DWORD *)a3 = 0;
//   v6 = a4;
//   if( a4 )
//     *(_DWORD *)a4 = 8;
//   v7 = a1;
//   if( a1 )
//   {
//     result = sub_150196C0(a1);
//     if( result )
//     {
//       if( v4 )
//         *(_DWORD *)v4 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(v7 + 88) + 4) + 4);
//       if( v5 )
//         *(_DWORD *)v5 = *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(v7 + 88) + 4) + 8);
//       result = v4 || v5 || v6;
//     }
//   }
//   else
//   {
//     SErrSetLastError( 0x57u);
//     result = 0;
//   }
//   return result;
// }

//----- (1501A480) --------------------------------------------------------
// int __stdcall SVidPlayContinue() {
//   int v0; // eax@1
//   int v1; // edi@1
//   int v2; // esi@2
// 
//   v0 = dword_15021BA8;
//   v1 = 0;
//   if( dword_15021BA8 )
//   {
//     do
//     {
//       v2 = *(_DWORD *)(v0 + 4);
//       if( SVidPlayContinueSingle( v0, 0, 0) )
//         v1 = 1;
//       v0 = v2;
//     }
//     while( v2 );
//   }
//   return v1;
// }

//----- (1501A760) --------------------------------------------------------
//int sub_1501A760( int This, double a2)

//----- (1501AEC0) --------------------------------------------------------
// int sub_1501AEC0( int a1, int a2, int a3) 
// {
//   int v3; // esi@1
//   int result; // eax@2
// 
//   v3 = a1;
//   if( *(_DWORD *)(a1 + 128) & 0x10000000 )
//   {
//     result = SDrawUnlockSurface( a2, a3, 0, *(_DWORD *)(a1 + 124), 0, 0);
//     *(_DWORD *)(v3 + 124) = 0;
//   }
//   return result;
// }

//----- (1501A8F0) --------------------------------------------------------
// int sub_1501A8F0( int a1, int a2, int a3)
// {
//   int v3; // ebp@1
//   int v4; // esi@1
//   int result; // eax@2
//   int v6; // edx@2
//   int v7; // edi@2
//   int v8; // ecx@4
//   int v9; // edi@4
//   int v10; // ecx@3
//   int v11; // eax@8
// 
//   v4 = a1;
//   v3 = a2;
//   if( *(_DWORD *)(a1 + 128) & 0x10000000 )
//   {
//     v7 = a3;
//     *(_DWORD *)a3 = ORIGINAL_RESOLUTION_WIDTH;
//     *(_DWORD *)(a3 + 4) = 480;
//     SDrawGetScreenSize( a3, a3 + 4, 0);
//     result = SDrawLockSurface( 0, *(_DWORD *)(v4 + 112), v4 + 124, v7, 0);
//     v6 = *(_DWORD *)(v4 + 124) + *(_DWORD *)v7 * *(_DWORD *)(*(_DWORD *)(v4 + 88) + 12);
//     *(_DWORD *)v3 = v6;
//     if( !*(_DWORD *)(v4 + 112) )
//     {
//       v10 = *(_DWORD *)(v4 + 128);
//       if( BYTE1(v10) & 1 )
//       {
//         v9 = *(_DWORD *)v7;
//         v8 = ORIGINAL_RESOLUTION_WIDTH;
//         if( v9 <= ORIGINAL_RESOLUTION_WIDTH )
//           v8 = v9;
//         *(_DWORD *)v3 = v6 + ((unsigned int)(v8 - *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(v4 + 88) + 4) + 4)) >> 1);
//       }
//     }
//   }
//   else
//   {
//     v11 = a3;
//     *(_DWORD *)a2 = *(_DWORD *)(a1 + 96) + *(_DWORD *)(a1 + 92) + *(_DWORD *)(a1 + 116) * *(_DWORD *)(a1 + 100);
//     *(_DWORD *)v11 = *(_DWORD *)(a1 + 116);
//     *(_DWORD *)(v11 + 4) = *(_DWORD *)(a1 + 120);
//     result = 1;
//   }
//   return result;
// }

//----- (1501ADA0) --------------------------------------------------------
// int sub_1501ADA0( int a1, int a2, int a3) 
// {
//   int v3; // esi@1
//   int v4; // ST08_4@3
//   int v5; // ST10_4@3
//   int v6; // edx@4
//   int v7; // ecx@4
//   unsigned int v9; // eax@4
//   int v10; // edi@6
//   int v11; // ebx@6
//   int v12; // eax@6
//   int v13; // edx@6
//   int v14; // ebx@6
//   int v15; // edx@6
//   int v16; // edi@6
//   int v17; // eax@6
//   int v18; // eax@6
//   int v19; // [sp+8h] [bp-14h]@1
//   int v20; // [sp+14h] [bp-8h]@1
//   int v21; // [sp+10h] [bp-Ch]@4
//   int v22; // [sp+Ch] [bp-10h]@6
// 
//   v3 = a1;
//   if( sub_1501A8F0(a1, (int)&v19, (int)&v20) )
//   {
//     if( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) ){
//       v5 = a3;
//       v4 = a2;
//       do
//       {
//         v7 = *(_DWORD *)(v3 + 88);
//         v6 = *(_DWORD *)(v7 + 4);
//         v9 = *(_DWORD *)(v6 + 892);
//         v21 = *(_DWORD *)(v6 + 892);
//         if( v9 > 1 )
//         {
//           if( *(_DWORD *)(v6 + 896) >= 1u )
//           {
//             v10 = *(_DWORD *)(v6 + 888);
//             v11 = *(_DWORD *)(v6 + 884);
//             v12 = v20;
//             v13 = v10 * *(_DWORD *)(v6 + 4);
//             v22 = v11;
//             v14 = v19 + 2 * (v11 + v10 * v20);
//             v15 = v22 + *(_DWORD *)(*(_DWORD *)v7 + 1096) + 2 * v13;
//             *(_DWORD *)(v3 + 20) = v21;
//             v16 = *(_DWORD *)(*(_DWORD *)(v7 + 4) + 896);
//             *(_DWORD *)(v3 + 36) = v14;
//             *(_DWORD *)(v3 + 24) = v16;
//             *(_DWORD *)(v3 + 40) = v15;
//             v17 = 2 * (v12 - *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892));
//             *(_DWORD *)(v3 + 28) = v17;
//             *(_DWORD *)(v3 + 60) = v17;
//             v18 = 2 * *(_DWORD *)(*(_DWORD *)(v7 + 4) + 4) - *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892);
//             *(_DWORD *)(v3 + 32) = v18;
//             *(_DWORD *)(v3 + 64) = v18;
//             SCodeExecute( *(_DWORD *)(dword_15021BB0 + 8), v3 + 12);
//           }
//         }
//       }
//       while( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) );
//       a2 = v4;
//       a3 = v5;
//     }
//     sub_1501AEC0( v3, a2, a3);
//   }
//   return 1;
// }

//----- (1501AC60) --------------------------------------------------------
// int sub_1501AC60( int a1, int a2, int a3) 
// {
//   int v3; // esi@1
//   int v4; // ST08_4@3
//   int v5; // ST10_4@3
//   int v6; // eax@4
//   int v7; // edx@4
//   unsigned int v9; // ecx@4
//   int v10; // edi@6
//   int v11; // ebx@6
//   int v12; // ecx@6
//   int v13; // edx@6
//   int v14; // ebx@6
//   int v15; // edx@6
//   int v16; // edi@6
//   int v17; // [sp+8h] [bp-14h]@1
//   int v18; // [sp+14h] [bp-8h]@1
//   int v19; // [sp+10h] [bp-Ch]@4
//   int v20; // [sp+Ch] [bp-10h]@6
// 
//   v3 = a1;
//   if( sub_1501A8F0(a1, (int)&v17, (int)&v18) )
//   {
//     if( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) )
//     {
//       v5 = a3;
//       v4 = a2;
//       do
//       {
//         v6 = *(_DWORD *)(v3 + 88);
//         v7 = *(_DWORD *)(v6 + 4);
//         v9 = *(_DWORD *)(v7 + 892);
//         v19 = *(_DWORD *)(v7 + 892);
//         if( v9 > 1 )
//         {
//           if( *(_DWORD *)(v7 + 896) >= 1u )
//           {
//             v10 = *(_DWORD *)(v7 + 888);
//             v11 = *(_DWORD *)(v7 + 884);
//             v12 = v18;
//             v13 = v10 * *(_DWORD *)(v7 + 4);
//             v20 = v11;
//             v14 = v17 + 2 * (v11 + v10 * v18);
//             v15 = v20 + *(_DWORD *)(*(_DWORD *)v6 + 1096) + 2 * v13;
//             *(_DWORD *)(v3 + 20) = v19;
//             v16 = *(_DWORD *)(*(_DWORD *)(v6 + 4) + 896);
//             *(_DWORD *)(v3 + 36) = v14;
//             *(_DWORD *)(v3 + 24) = 2 * v16;
//             *(_DWORD *)(v3 + 40) = v15;
//             *(_DWORD *)(v3 + 28) = v12 - 2 * *(_DWORD *)(*(_DWORD *)(v6 + 4) + 892);
//             *(_DWORD *)(v3 + 60) = v12 - 2 * *(_DWORD *)(*(_DWORD *)(v6 + 4) + 892);
//             *(_DWORD *)(v3 + 32) = -*(_DWORD *)(*(_DWORD *)(v6 + 4) + 892);
//             *(_DWORD *)(v3 + 64) = 2 * *(_DWORD *)(*(_DWORD *)(v6 + 4) + 4) - *(_DWORD *)(*(_DWORD *)(v6 + 4) + 892);
//             SCodeExecute( *(_DWORD *)(dword_15021BB0 + 8), v3 + 12);
//           }
//         }
//       }
//       while( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) );
//       a2 = v4;
//       a3 = v5;
//     }
//     sub_1501AEC0(v3, a2, a3);
//   }
//   return 1;
// }

//----- (1501AB30) --------------------------------------------------------
// int sub_1501AB30( int a1, int a2, int a3)
// {
//   int v3; // esi@1
//   int result; // eax@2
//   int v5; // ST10_4@5
//   int v6; // eax@6
//   int v7; // edx@6
//   unsigned int v8; // ecx@6
//   int v9; // edi@8
//   int v10; // ecx@8
//   int v11; // eax@8
//   int v12; // ecx@8
//   int v13; // ebp@8
//   int v14; // eax@8
//   int v15; // edi@8
//   int v16; // [sp+8h] [bp-14h]@3
//   int v17; // [sp+14h] [bp-8h]@3
//   int v18; // [sp+10h] [bp-Ch]@6
//   int v19; // [sp+Ch] [bp-10h]@8
// 
//   v3 = a1;
//   if( *(_DWORD *)(a1 + 76) )
//   {
//     if( sub_1501A8F0(a1, (int)&v16, (int)&v17) )
//     {
//       if( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) ){
//         v5 = a3;
//         do
//         {
//           v7 = *(_DWORD *)(v3 + 88);
//           v6 = *(_DWORD *)(v7 + 4);
//           v8 = *(_DWORD *)(v6 + 892);
//           v18 = *(_DWORD *)(v6 + 892);
//           if( v8 > 1 )
//           {
//             if( *(_DWORD *)(v6 + 896) >= 1u )
//             {
//               v9 = 2 * *(_DWORD *)(v6 + 888);
//               v10 = *(_DWORD *)(v6 + 884);
//               v11 = *(_DWORD *)(v6 + 4);
//               v19 = v10;
//               v12 = v17;
//               v13 = v9 * v17 + v16 + 2 * v19;
//               v14 = v19 + *(_DWORD *)(*(_DWORD *)v7 + 1096) + v9 * v11;
//               *(_DWORD *)(v3 + 20) = v18 - 1;
//               v15 = *(_DWORD *)(*(_DWORD *)(v7 + 4) + 896);
//               *(_DWORD *)(v3 + 36) = v13;
//               *(_DWORD *)(v3 + 24) = v15;
//               *(_DWORD *)(v3 + 40) = v14;
//               *(_DWORD *)(v3 + 28) = 2 * (v12 - *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892));
//               *(_DWORD *)(v3 + 32) = 2 * *(_DWORD *)(*(_DWORD *)(v7 + 4) + 4) - *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892);
//               SCodeExecute( *(_DWORD *)dword_15021BB0, v3 + 12);
//             }
//           }
//         }
//         while( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4),
//                   1) );
//         a3 = v5;
//       }
//       sub_1501AEC0(v3, a2, a3);
//     }
//     result = 1;
//   }
//   else
//   {
//     result = 0;
//   }
//   return result;
// }

//----- (1501A9B0) --------------------------------------------------------
// int sub_1501A9B0( int a1, int a2, int a3) 
// {
//   int v3; // esi@1
//   int result; // eax@2
//   int v5; // ST0C_4@5
//   int v6; // eax@6
//   int v7; // ecx@6
//   int v8; // eax@3
//   unsigned int v9; // edx@6
//   int v10; // edi@8
//   int v11; // edx@8
//   int v12; // eax@8
//   int v13; // edx@8
//   int v14; // ebp@8
//   int v15; // eax@8
//   int v16; // edi@8
//   int v17; // [sp+10h] [bp-14h]@3
//   int v18; // [sp+1Ch] [bp-8h]@3
//   int v19; // [sp+18h] [bp-Ch]@6
//   int v20; // [sp+14h] [bp-10h]@8
// 
//   v3 = a1;
//   if( *(_DWORD *)(a1 + 76) )
//   {
//     v8 = *(_DWORD *)(a1 + 88);
//     *(_DWORD *)(a1 + 20) = *(_DWORD *)(*(_DWORD *)(v8 + 4) + 4);
//     *(_DWORD *)(a1 + 24) = *(_DWORD *)(*(_DWORD *)(v8 + 4) + 8) - 1;
//     *(_DWORD *)(a1 + 36) = *(_DWORD *)(*(_DWORD *)(v8 + 4) + 4) + *(_DWORD *)(*(_DWORD *)v8 + 1096);
//     *(_DWORD *)(a1 + 40) = *(_DWORD *)(*(_DWORD *)v8 + 1096);
//     *(_DWORD *)(a1 + 28) = *(_DWORD *)(*(_DWORD *)(v8 + 4) + 4);
//     *(_DWORD *)(a1 + 32) = *(_DWORD *)(*(_DWORD *)(v8 + 4) + 4);
//     SCodeExecute( *(_DWORD *)(dword_15021BB0 + 4), a1 + 12);
//     if( sub_1501A8F0(v3, (int)&v17, (int)&v18) )
//     {
//       if( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 1) ){
//         v5 = a3;
//         do
//         {
//           v7 = *(_DWORD *)(v3 + 88);
//           v6 = *(_DWORD *)(v7 + 4);
//           v9 = *(_DWORD *)(v6 + 892);
//           v19 = *(_DWORD *)(v6 + 892);
//           if( v9 > 1 )
//           {
//             if( *(_DWORD *)(v6 + 896) >= 1u )
//             {
//               v10 = 2 * *(_DWORD *)(v6 + 888);
//               v11 = *(_DWORD *)(v6 + 884);
//               v12 = *(_DWORD *)(v6 + 4);
//               v20 = v11;
//               v13 = v18;
//               v14 = v10 * v18 + v17 + 2 * v20;
//               v15 = v20 + *(_DWORD *)(*(_DWORD *)v7 + 1096) + v10 * v12;
//               *(_DWORD *)(v3 + 20) = v19 - 1;
//               v16 = *(_DWORD *)(*(_DWORD *)(v7 + 4) + 896);
//               *(_DWORD *)(v3 + 36) = v14;
//               *(_DWORD *)(v3 + 24) = 2 * v16;
//               *(_DWORD *)(v3 + 40) = v15;
//               *(_DWORD *)(v3 + 28) = v13 - 2 * *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892);
//               *(_DWORD *)(v3 + 32) = *(_DWORD *)(*(_DWORD *)(v7 + 4) + 4) - *(_DWORD *)(*(_DWORD *)(v7 + 4) + 892);
//               SCodeExecute( *(_DWORD *)dword_15021BB0, v3 + 12);
//             }
//           }
//         }
//         while( _SmackToBufferRect( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4),
//                   1) );
//         a3 = v5;
//       }
//       sub_1501AEC0(v3, a2, a3);
//     }
//     result = 1;
//   }
//   else
//   {
//     result = 0;
//   }
//   return result;
// }

//----- (1501A880) --------------------------------------------------------
// int sub_1501A880( int a1, int a2)
// {
//   int v2; // edi@1
//   int v3; // esi@1
//   int v5; // eax@2
//   int v6; // [sp+8h] [bp-Ch]@1
//   int v7; // [sp+Ch] [bp-8h]@1
//   int v8; // [sp+10h] [bp-4h]@2
// 
//   v3 = a1;
//   v2 = 1;
//   if( sub_1501A8F0(a1, (int)&v6, (int)&v7) ){
//     _SmackToBuffer( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4), 0, 0, v7, v8, (const void*)v6, 0);
//     v5 = _SmackDoFrame( (Smack*)*(_DWORD *)(*(_DWORD *)(v3 + 88) + 4));
//     v2 = v5;
//     sub_1501AEC0(v3, a2, v5);
//   }
//   return v2;
// }

//----- (1501A4B0) --------------------------------------------------------
// int __stdcall SVidPlayContinueSingle( int a1, int a2, int a3) 
// {
//   int v3; // ebx@1
//   int v4; // edi@3
//   int result; // eax@4
//   int v6; // eax@6
//   int v7; // eax@7
//   int v8; // eax@11
//   PALETTEENTRY* v9; // esi@11
//   int v10; // eax@15
//   char *v11; // ecx@15
//   int v12; // ebp@15
//   int v13; // esi@15
//   int v14; // ebp@24
//   int v15; // eax@34
//   int v16; // eax@37
//   int v17; // eax@46
//   int v18; // et0@12
//   char v19; // zf@12
//   int v20; // eax@16
//   int v21; // eax@17
//   int v22; // eax@23
//   int v23; // eax@35
//   int v24; // ecx@46
//   int v25; // et0@48
//   char v26; // zf@48
//   char v27; // [sp+11h] [bp-3FFh]@15
//   PALETTEENTRY v28; // [sp+10h] [bp-400h]@17
// 
//   v3 = a3;
//   if( a3 )
//     *(_DWORD *)a3 = 0;
//   v4 = a1;
//   if( !a1 )
//   {
//     SErrSetLastError( 0x57u);
//     return 0;
//   }
//   if( sub_150196C0(a1) )
//   {
//     v6 = *(_DWORD *)(v4 + 88);
//     if( v6 )
//     {
//       v7 = *(_DWORD *)(v6 + 4);
//       if( v7 )
//       {
//         if( !a2 && _SmackToBufferRect( (Smack*)a2, v7) )
//           return 1;
//         v8 = *(_DWORD *)(v4 + 88);
//         v9 = 0;
//         if( *(_DWORD *)(*(_DWORD *)(v8 + 4) + 104) )
//         {
//           v18 = *(_DWORD *)(v4 + 128);
//           v18 &= 0x100000u;
//           v19 = v18 == 0;
//           if( v19 ){
// 			  if( *(_DWORD *)v8 ){
// 				_SmackBufferNewPalette( (SmackBuf*)*(_DWORD *)v8, (const void*)(*(_DWORD *)(v8 + 4) + 108), 0);
// 			  }
//             v13 = *(_DWORD *)(v4 + 88);
//             v10 = 0;
//             v11 = &v27;
//             v12 = 256;
//             do
//             {
//               v11 += 4;
//               *(v11 - 5) = 4 * *(_BYTE *)(v10 + *(_DWORD *)(v13 + 4) + 108);
//               v20 = v10 + 1;
//               *(v11 - 4) = 4 * *(_BYTE *)(v20++ + *(_DWORD *)(v13 + 4) + 108);
//               *(v11 - 3) = 4 * *(_BYTE *)(v20 + *(_DWORD *)(v13 + 4) + 108);
//               v10 = v20 + 1;
//               --v12;
//             }
//             while( v12 );
//             v21 = *(_DWORD *)(v4 + 128);
//             v9 = &v28;
//             if( v21 & 0x200000 )
//               SDrawShowCursor(0);
//             if( (*(_DWORD *)(v4 + 128) & 0x101) == 256 )
//               sub_1501A760((int)&v28, 4605380978949069210i64);
//             if( *(_BYTE *)(v4 + 128) & 2 )
//               sub_15019EF0(v4, &v28);
//           }
//         }
//         v22 = *(_DWORD *)(v4 + 128);
//         if( BYTE1(v22) & 2 )
//         {
//           v14 = _SmackDoFrame( (Smack*)*(_DWORD *)(*(_DWORD *)(v4 + 88) + 4));
//           if( v9 )
//             SDrawUpdatePalette( *(_DWORD *)(v4 + 80), *(_DWORD *)(v4 + 84),
//               (PALETTEENTRY *)&v9[4 * *(_DWORD *)(v4 + 80)],
//               0);
//         }
//         else
//         {
//           if( v9 )
//             SDrawUpdatePalette(
//               *(_DWORD *)(v4 + 80),
//               *(_DWORD *)(v4 + 84),
//               (PALETTEENTRY *)&v9[4 * *(_DWORD *)(v4 + 80)],
//               0);
//           v14 = sub_1501A880( v4, v3);
//         }
//         if( v3 )
//         {
//           if( !v14 )
//             *(_DWORD *)v3 = 1;
//         }
//         if( *(_BYTE *)(v4 + 128) & 2 )
//         {
//           if( v14 )
//           {
//             v15 = GetTickCount();
//             if( (unsigned int)(v15 - dword_15021BC0) >= 0x3E8 )
//             {
//               dword_15021BC0 = v15;
//             }
//             else
//             {
//               v23 = *(_DWORD *)(v4 + 128);
//               LOBYTE(v23) = v23 & 0xFD;
//               *(_DWORD *)(v4 + 128) = v23;
//             }
//           }
//         }
//         v16 = *(_DWORD *)(v4 + 128);
//         if( BYTE1(v16) & 2 )
//         {
//           if( !v14 )
//           {
//             if( v16 & 2 )
//             {
//               if( v16 & 1 )
//                 sub_1501A9B0( v4, v3, v4);
//               else
//                 sub_1501AB30( v4, v3, v4);
//             }
//             else
//             {
//               if( v16 & 1 )
//                 sub_1501AC60( v4, v3, v4);
//               else
//                 sub_1501ADA0( v4, v3, v4);
//             }
//           }
//         }
//         ++*(_DWORD *)(*(_DWORD *)(v4 + 88) + 8);
//         v24 = *(_DWORD *)(v4 + 88);
//         v17 = *(_DWORD *)(v24 + 4);
//         if( *(_DWORD *)(v24 + 8) < *(_DWORD *)(v17 + 12) )
//         {
//           _SmackNextFrame( (Smack*)v17);
//           return 1;
//         }
//         v25 = *(_DWORD *)(v4 + 128);
//         v25 &= 0x40000u;
//         v26 = v25 == 0;
//         if( !v26 )
//         {
//           _SmackGoto( (Smack*)v17, 0);
//           result = 1;
//           *(_DWORD *)(*(_DWORD *)(v4 + 88) + 8) = 0;
//           return result;
//         }
//         SVidPlayEnd( v4);
//       }
//     }
//   }
//   return 0;
// }

//----- (1501AEF0) --------------------------------------------------------
// int __stdcall SVidPlayEnd( int a1) 
// {
//   int v1; // esi@1
//   int result; // eax@2
//   int v3; // eax@5
//   int v4; // eax@7
//   int v5; // ecx@9
//   void *v6; // eax@11
//   int v7; // eax@19
// 
//   v1 = a1;
//   if( a1 )
//   {
//     result = sub_150196C0(a1);
//     if( result )
//     {
//       v3 = *(_DWORD *)(v1 + 88);
//       if( v3 )
//       {
//         v4 = *(_DWORD *)v3;
// 		if( v4 ){
//           _SmackBufferClose( (SmackBuf*)v4);
// 		}
//         v5 = *(_DWORD *)(v1 + 88);
// 		if( *(_DWORD *)(v5 + 4) ){
//           _SmackClose( (Smack*)(v5 + 4));
// 		}
//         SMemFree( (void*)*(_DWORD *)(v1 + 88), "SVID.CPP", 989, 0);
//         v6 = *(void **)(v1 + 8);
//         *(_DWORD *)(v1 + 88) = 0;
//         if( v6 != (void *)-1 )
//         {
//           CloseHandle(v6);
//           *(_DWORD *)(v1 + 8) = -1;
//         }
//         if( *(_DWORD *)(v1 + 76) )
//         {
//           SMemFree( (void*)*(_DWORD *)(v1 + 76), "SVID.CPP", 999, 0);
//           *(_DWORD *)(v1 + 76) = 0;
//           *(_DWORD *)(v1 + 44) = 0;
//         }
//         if( *(_DWORD *)v1 )
//         {
//           *(_DWORD *)(*(_DWORD *)v1 + 4) = *(_DWORD *)(v1 + 4);
//         }
//         else
//         {
//           if( dword_15021BA8 == v1 )
//             dword_15021BA8 = *(_DWORD *)(v1 + 4);
//         }
//         v7 = *(_DWORD *)(v1 + 4);
//         if( v7 )
//         {
//           *(_DWORD *)v7 = *(_DWORD *)v1;
//         }
//         else
//         {
//           if( dword_15021BAC == v1 )
//             dword_15021BAC = *(_DWORD *)v1;
//         }
//         *(_DWORD *)v1 = 0;
//         *(_DWORD *)(v1 + 4) = 0;
//         SMemFree( (void*)v1, ".?AUVIDEOREC@@", -2, 0);
//         result = 1;
//       }
//       else
//       {
//         result = 0;
//       }
//     }
//   }
//   else
//   {
//     SErrSetLastError( 0x57u);
//     result = 0;
//   }
//   return result;
// }

//----- (1501A2C0) --------------------------------------------------------
//int __stdcall SVidPlayBeginFromMemory( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, int a3, int a4, int a5, int a6, int a7, int a8) 
// {
//   int v8; // ebx@3
//   int v9; // ecx@5
//   int v10; // eax@7
//   int v11; // ecx@9
//   int v12; // eax@11
//   int v13; // ebp@11
//   int v14; // ecx@13
//   int result; // eax@16
//   HANDLE v16; // eax@17
//   void *v17; // esi@17
//   int v18; // edi@19
//   __int16 PathName; // [sp+118h] [bp-104h]@17
//   char v20; // [sp+11Ah] [bp-102h]@17
//   __int16 v21; // [sp+21Ah] [bp-2h]@17
//   CHAR FileName; // [sp+14h] [bp-208h]@17
//   char v23; // [sp+15h] [bp-207h]@17
//   __int16 v24; // [sp+115h] [bp-107h]@17
//   char v25; // [sp+117h] [bp-105h]@17
//   DWORD NumberOfBytesWritten; // [sp+10h] [bp-20Ch]@19
// 
//   if( a8 )
//     *(_DWORD *)a8 = 0;
//   v8 = a7;
//   if( a3 || !(a7 & 0x20000000) )
//     v9 = -1;
//   else
//     v9 = 0;
//   v10 = v9 & -(nNumberOfBytesToWrite != 0) & -(lpBuffer != 0);
//   if( a5 || !a3 )
//     v11 = -1;
//   else
//     v11 = 0;
//   v13 = a6;
//   v12 = v11 & v10;
//   if( a6 && *(_DWORD *)a6 != 12 )
//     v14 = 0;
//   else
//     v14 = -1;
//   if( v14 & v12 )
//   {
//     PathName = word_1501CAFC;
//     memset(&v20, 0, 0x100u);
//     v21 = 0;
//     FileName = ZeroByte;
//     memset(&v23, 0, 0x100u);
//     v24 = 0;
//     v25 = 0;
//     GetTempPathA(0x104u, (CHAR *)&PathName);
//     GetTempFileNameA((const CHAR *)&PathName, "BN_", 0, &FileName);
//     v16 = CreateFileA(&FileName, 0xC0000000u, 3u, 0, 2u, 0x4000100u, 0);
//     v17 = v16;
//     if( v16 == (HANDLE)-1 )
//     {
//       result = 0;
//     }
//     else
//     {
//       WriteFile(v16, lpBuffer, nNumberOfBytesToWrite, &NumberOfBytesWritten, 0);
//       SetFilePointer(v17, 0, 0, 0);
//       v18 = SVidPlayBegin( (char *)v17, a3, a4, a5, v13, v8 | 0x30000, a8);
//       CloseHandle(v17);
//       DeleteFileA(&FileName);
//       result = v18;
//     }
//   }
//   else
//   {
//     SErrSetLastError( 0x57u);
//     result = 0;
//   }
//   return result;
// }
