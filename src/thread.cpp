#include "stdafx.h"

#pragma pack(push, 4)
struct TInfo
{
	TInfo* pNext;
	int    pnum;
	uchar  bCmd;
	uint   dwLen;
	uchar  bData[1];
};
#pragma pack(pop)

TInfo* DeltaQueue;// 0057A1C4
CRITICAL_SECTION DeltaCriticalSection;// 0057A1A8
DWORD DeltaThreadId;
bool DeltaThreadRun;
HANDLE DeltaToSend;

//----- (00417A4E) -------------------------------------------------------- main
void SetupDeltaCriticalSection()
{
	InitializeCriticalSection(&DeltaCriticalSection);
	atexit( [](){ DeleteCriticalSection(&DeltaCriticalSection); } );
}

//----- (00417A7C) -------------------------------------------------------- main
void __fastcall DeltaRemovePlayer( int playerIndex)
{
	EnterCriticalSection(&DeltaCriticalSection);
	for( TInfo* pInfo = DeltaQueue; pInfo; pInfo = pInfo->pNext ){
		if( pInfo->pnum == playerIndex ){
			pInfo->pnum = PlayersMax_4;
		}
	}
	LeaveCriticalSection(&DeltaCriticalSection);
}

//----- (00417AAF) --------------------------------------------------------
void __fastcall DThreadSendPLayerInfoChunk( int playerIndex, char cmd, void* src, size_t size)
{
	if( MaxCountOfPlayersInGame == 1 ) return;

	// create a block of memory for the player info
	TInfo * pInfo = (TInfo *) AllocMem(sizeof(TInfo) + size);
	pInfo->pNext = NULL;
	pInfo->pnum = playerIndex;
	pInfo->bCmd = cmd;
	pInfo->dwLen = size;
	memcpy(pInfo->bData, src, size);

	// link to tail of list
	EnterCriticalSection(&DeltaCriticalSection);
	TInfo ** ppInfo = &DeltaQueue;
	while (*ppInfo) ppInfo = &(*ppInfo)->pNext;
	*ppInfo = pInfo;

	// wake up thread if necessary -- inside crit section
	SetEvent(DeltaToSend);
	LeaveCriticalSection(&DeltaCriticalSection);
}

//----- (00417B2C) -------------------------------------------------------- main
void DeltaThreadStart()
{
	if( MaxCountOfPlayersInGame != 1 ){ // only in multi
		DeltaToSend = CreateEventA(0, 1, 0, 0);
		if( !DeltaToSend ){
			TerminateWithError("dthread:1\n%s", GetErrorTextForLastError());
		}
		DeltaThreadRun = true;
		DeltaThreadHandle = (HANDLE) _beginthreadex(0, 0, DeltaThread, 0, 0, (unsigned int*)&DeltaThreadId);
		if( DeltaThreadHandle == INVALID_HANDLE_VALUE ){
			TerminateWithError("dthread2:\n%s", GetErrorTextForLastError());
		}
	}
}

//----- (00417B99) -------------------------------------------------------- main
uint __stdcall DeltaThread(void*)
{
	while( DeltaThreadRun ){
		// sleep until there is work to do
		if( !DeltaQueue && WaitForSingleObject(DeltaToSend, INFINITE) == WAIT_FAILED ){
			TerminateWithError("dthread4:\n%s", GetErrorTextForLastError());
		}

		// pull one item off the list
		EnterCriticalSection(&DeltaCriticalSection);
		TInfo* pInfo = DeltaQueue;
		if( DeltaQueue ){
			DeltaQueue = DeltaQueue->pNext;
		}else{
			ResetEvent(DeltaToSend);
		}
		LeaveCriticalSection(&DeltaCriticalSection);
		if( ! pInfo ) continue;

		// send the item if it still has a valid destination
		if( pInfo->pnum != PlayersMax_4 ){
			multi_send_mega_packet_by_parts( pInfo->pnum, pInfo->bCmd, &pInfo->bData[0], pInfo->dwLen);
		}

		// (bytes * 1000 ms/sec) / (bytes/sec) = ms sleep time
		int sleepTime = pInfo->dwLen * 1000 / DeltaBytesPerSecond;
		LimitToMax( sleepTime, 1000 );

		// free item
		FreeMem(pInfo);
			
		// wait for output queue to empty before sending again
		SleepTh(sleepTime);
	}
	return 0;
}

//----- (00417C66) --------------------------------------------------------
void DeltaThreadStop()
{
	// if the event was never initialized, the thread cannot be running
	if( !DeltaToSend ) return;

	// kill off the loader thread
	DeltaThreadRun = false;
	SetEvent(DeltaToSend);

	// wait til the thread is done
	if( DeltaThreadHandle != INVALID_HANDLE_VALUE ){
		if( DeltaThreadId != GetCurrentThreadId() ){
			if( WaitForSingleObject(DeltaThreadHandle, INFINITE) == WAIT_FAILED ){
				TerminateWithError("dthread_cleanup:\n(%s)", GetErrorTextForLastError());
			}
			CloseHandle(DeltaThreadHandle);
			DeltaThreadHandle = INVALID_HANDLE_VALUE;
		}
	}

	// clean up event
	CloseHandle(DeltaToSend);
	DeltaToSend = 0;

	// clean up linked list
	while( DeltaQueue ){
		TInfo * pNext = DeltaQueue->pNext;
		FreeMem(DeltaQueue);
		DeltaQueue = pNext;
	}
}
