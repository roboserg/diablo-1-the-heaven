#include "stdafx.h"

HANDLE diabdatMPQPtr;
HANDLE CurSpeechHandle;
Sound* CurSpeechSound;

//----- (00479553) --------------------------------------------------------
int __fastcall File_Close(HANDLE aHandle)
{
	return SFileCloseFile(aHandle);
}

//----- (0047955A) --------------------------------------------------------
DWORD __fastcall File_GetSize(HANDLE aHandle, LPDWORD lpFileSizeHigh)
{
	DWORD* v2;     // edi@1
	HANDLE Handle; // esi@1
	DWORD result = 0;  // eax@2
	int v5;        // [sp+8h] [bp-4h]@1
	v5 = 0;
	v2 = lpFileSizeHigh;
	Handle = aHandle;
	int tryes = 10;
	while( tryes-- ){
		result = SFileGetFileSize(Handle, v2);
		if( result )
			break;
		File_RestoreAccess(Handle, &v5, result);
	}
	return result;
}

//----- (00479584) --------------------------------------------------------
void __fastcall File_RestoreAccess(HANDLE aHandle, int* aTryCountPtr, LPARAM dwInitParam)
{
	int* TryCountPtr; // esi@1
	char* a2;         // [sp+8h] [bp-4h]@4
	TryCountPtr = aTryCountPtr;
	if( (unsigned int) * aTryCountPtr >= 5 )
		FileErrDlg(dwInitParam);
	if( aHandle && SFileGetFileArchive(aHandle, (int)&a2) && a2 != diabdatMPQPtr ){
		SleepTh(20);
		++*TryCountPtr;
	}else{
		if( !InsertCDDlg() )
			FileErrDlg(dwInitParam);
	}
}

//----- (004795DB) --------------------------------------------------------
int __fastcall File_Open(const char* fileName, HANDLE* fileHandle, int oneTry)
{
	int TryCount = 0;
	int tryes = 10;
	while( tryes-- ){
		int opened = SFileOpenFile(fileName, fileHandle);
		if( opened )
			return 1;
		if( oneTry != opened && GetLastError() == 2 )
			break;
		File_RestoreAccess(0, &TryCount, (LPARAM)fileName);
	}
	return 0;
}

//----- (0047961E) --------------------------------------------------------
int __fastcall File_Read(HANDLE aHandle, LPVOID aBufPtr, DWORD nNumberOfBytesToRead)
{
	void* BufPtr;         // ebx@1
	HANDLE Handle;        // edi@1
	int result = 0;       // eax@2
	int TryCount;         // [sp+14h] [bp-4h]@1
	LONG lDistanceToMove; // [sp+10h] [bp-8h]@1
	uint BytesReaded;     // [sp+Ch] [bp-Ch]@2
	BufPtr = aBufPtr;
	Handle = aHandle;
	TryCount = 0;
	lDistanceToMove = File_Seek(aHandle, 0, 0, 1u);
	int tryes = 10;
	while( tryes-- ){
		result = SFileReadFile(Handle, BufPtr, nNumberOfBytesToRead, (DWORD*)&BytesReaded, 0);
		if( result ) break; //return result;
		File_RestoreAccess(Handle, &TryCount, 0);
		File_Seek(Handle, lDistanceToMove, 0, 0);
	}
	//TerminateWithError("Can't read file");
	return result;
}

// 1 - from curPos, 0 - from begin 
//----- (00479670) --------------------------------------------------------
DWORD __fastcall File_Seek(HANDLE handle, LONG posToMove, LONG* posToMoveHigh, DWORD moveMethod)
{
	DWORD pos;
	int tryCount = 0;
	int tryes = 10;
	while( tryes-- ){
		pos = SFileSetFilePointer(handle, posToMove, (int)posToMoveHigh, moveMethod);
		if( pos != INVALID_SET_FILE_POINTER ) break;
		File_RestoreAccess(handle, &tryCount, 0);
	}
	return pos;
}