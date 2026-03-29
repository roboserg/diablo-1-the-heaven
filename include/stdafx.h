#ifndef _stdafx_h_
#define _stdafx_h_
#else
#error reinclude stdafx.h
#endif

// 4701 - potentially uninitialized variable, have to check later
#pragma warning(3: /*4701*/ 4706; disable: 5055 4996 4091 4201 4324 4211 4389 4100 4702 4189; error: 4551 4552 4553 4554 4555 4706 4834 4309)

#if MSVC_TOOLSET < 142 && _MSC_VER >= 1920
#error You have selected MSVC toolset v141_xp, but v141 compiler is not installed, add it in Visual Studio Installer
#endif

//#define NET_LOG
#define WINDOWS_IGNORE_PACKING_MISMATCH

#define USE_32BITCOLOR
#define OLD_PCX_SCREENSHOT
#define HIRES
#define SPELL_QUEUE_AND_COOLDOWN_FIX 0
#define DEBUG_MEMORY 0
#define MULTI_PLAYER_DEBUG_CHECK 0
//#define DIRECTDRAW_VERSION 0x0300
//#define DIRECTSOUND_VERSION 0x0300
//#define DIRECTINPUT_VERSION 0x0300

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define NOMINMAX
#include <windows.h>
#undef DrawText
#include <io.h>
//#include <ctype.h>
#include <ddraw.h>
//#include <ddrawi.h> // используется для storm.cpp ?
#include <d3d9.h>
#include <mmreg.h>
//#include <austream.h>
#include <dsound.h>
#include <time.h>
#include <process.h>

//#include <SHLOBJ.h>
//#include <SHELLAPI.h>

//#include <math.h>
#include <tchar.h>
#include <stdio.h>
//#include <stdarg.h>
//#include <CommDlg.h>
#include <iostream>
//#include <iomanip>
//#include <type_traits>
#include <vector>
//#include <cmath>
//#include <string.h>
//#include <stdlib.h>
//#include <eh.h>
//#include <conio.h>
#include <fstream>
#include <sstream>
//#include <strstream>
#include <bitset>
#include <array>
#include <functional>
//#include <assert.h>
#include <queue>

#include <winsock2.h>
#include <ws2tcpip.h>
//#include <iphlpapi.h>
#include <fcntl.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <algorithm>
#include <intrin.h>
//#include <atomic>
#include <map>
#include <set>
#include <stack>
#include <filesystem>
#include <chrono>
#include <optional>

//#include <boost/pfr.hpp>
//using namespace boost::pfr::ops;

using namespace std;

#pragma warning( error: 4548 )

#ifdef _DEBUG
//#include <assert.h>
#endif

#define countof( a ) __crt_countof( a ) // only first rank, TODO: change to std::size(a)
#define fullcountof( a ) ( sizeof( a ) / sizeof( remove_all_extents_t< decltype( a ) > ) ) // считает сразу все измерения
#define flat( a ) reinterpret_cast<remove_all_extents_t< decltype( a )> (&) [ fullcountof( a ) ]>(a)

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned long long u64;
typedef long long i64;
#define _UNKNOWN char // could not determine their types

#ifdef _MSC_VER
#if _MSC_VER < 1500
#define snprintf _snprintf
#define vsnprintf _vsnprintf
typedef int LSTATUS;
#endif
#endif

#ifdef NDEBUG
//#define Assert(expression) ((void)0)
#else
//#define Assert(expression) (void)((!!(expression)) || ( SwitchFullscreen(1),_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) )
#endif

//#include "serialize.h"
#include "tools/version.h"
#include "bitsbytes.h"
#include "enums.h"
#include "common.h"
#include "engine.h"
#include "structs.h"
#include "item.h"
#include "IniFile.h"
#include "lib/pklib/pklib.h"
#include "vars.h" 
#include "funcs.h"
#include "game.h"
//#define ALLOW_TH1_CONVERTATION
//#define MORE_COLOR
//#include "fmt_core.h" // Don't remove it unless you find out how on the earth it can fix bug with 4th floor portal

#include "resource.h"
#include "difficulty.h"
#include "sync.h"
#include "multi.h"
#include "msg.h"
#include "tmsg.h"
#include "track.h"
#include "storm.h"
#include "hell.h"

// minidump on crash functional
#include <dbghelp.h>
#include <crtdbg.h>
