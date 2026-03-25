#pragma once


#ifdef ZFS_DLL
#ifdef ZFS_INTERNAL
#define ZFSEXPORT __declspec(dllexport)
#else
#define ZFSEXPORT __declspec(dllimport)
#endif //ZFS_INTERNAL
#else
#define ZFSEXPORT
#endif