#pragma once

#define TH4_REVISION 6
#define THE_HELL_VERSION_INT 0,2,0,0

#ifdef _DEBUG
#define _THE_HEAVEN_BUILD_SUFFIX " [DEBUG]"
#else
#define _THE_HEAVEN_BUILD_SUFFIX ""
#endif

#define THE_HELL_VERSION_STRING "v0.2.0" _THE_HEAVEN_BUILD_SUFFIX "\0"
#define THE_HELL_VERSION_HUMAN_STRING "Diablo: The Heaven v0.2.0" _THE_HEAVEN_BUILD_SUFFIX "\0"

#ifdef _DEBUG
#define CHEATS 1  // Cheats auto-enabled in debug builds
#else
#define CHEATS 0  // Cheats auto-disabled in release builds
#endif
