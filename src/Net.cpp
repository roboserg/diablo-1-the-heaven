#include "stdafx.h"

bool NetThreadRun;

//----- (0044B8CE) --------------------------------------------------------
void SetupTimingCritSection()
{
	InitializeCriticalSection(&FrameSyncCritSection);
	atexit( [](){ DeleteCriticalSection(&FrameSyncCritSection); } );
}

//----- (0044B8FC) --------------------------------------------------------
void __fastcall nthread_terminate_game( char* errorText)
{
	uint error = GetLastError();
	if( error != STORM_ERROR_INVALID_PLAYER ){
		if( is( error, STORM_ERROR_GAME_TERMINATED, STORM_ERROR_NOT_IN_GAME ) ){
			gbGameDestroyed = 1;
		}else{
			TerminateWithError("%s:\n%s", errorText, GetErrorTextForLastError());
		}
	}
}

//----- (0044B939) --------------------------------------------------------
uint __fastcall NThreadFillSyncQueue( uint netTurnCount, int turnDelta )
{
	uint turnsInTransit;
	if( ! SNetGetTurnsInTransit( &turnsInTransit ) ){
		nthread_terminate_game("SNetGetTurnsInTransit");
	}
	while( turnsInTransit++ < gdwTurnsInTransit ){
		uint turn = NetTurnNeedSync | netTurnCount & ~0x80000000;
		NetTurnNeedSync = 0;
		if( ! SNetSendTurn( (char *)&turn, sizeof(turn) ) ) nthread_terminate_game("SNetSendTurn");
		netTurnCount += turnDelta;
		if( netTurnCount >= 0x7FFFFFFF ){
			netTurnCount &= 0x0000FFFF;
		}
	}
	return netTurnCount;
}

//----- (0044B9BE) --------------------------------------------------------
int __fastcall NThreadMsgCheck( int* sendAsync )
{
	*sendAsync = 0;
	// count down until time to send async packet
	if( --sgbPacketCountdown ){
		// advance the game clock to the next
		// time we will need to run a game loop
		MainFrameTimer += MAX_FPS / ENGINE_FPS; // 20 fps
		return 1;
	}
	// reset async countdown timer
	sgbPacketCountdown = sgbNetUpdateRate;
	// is it time to receive sync message?
	if( ! --sgbSyncCountdown ){
		if( ! SNetReceiveTurns(0, PlayersMax_4, glpMsgTbl, gdwMsgLenTbl, player_state) ){
			if( GetLastError() != STORM_ERROR_NO_MESSAGES_WAITING ){
				nthread_terminate_game( "SNetReceiveTurns" );
			}
			// we didn't successfully receive a turn, so go run some
			// user stuff, and come back here to try again next time
			sgbPacketCountdown = 1;
			sgbSyncCountdown = 1;
			sgbGotPacketOK = 0;
			return 0;
		}else if( ! sgbGotPacketOK ){
			// we were waiting for the packet.  Since
			// there should be a bunch of messages in the
			// queue, we should never have to wait.
			sgbGotPacketOK = 1;
			MainFrameTimer = GetTickCountTh(); // перехватываем вызов
		}
		// reset sync countdown timer
		sgbSyncCountdown = 4;
		// process synchronous turn
		process_turn();
	}
	// it's time to send an async message
	*sendAsync = 1;
	// advance the game clock to the next
	// time we will need to run a game loop
	MainFrameTimer += MAX_FPS / ENGINE_FPS; // 20 fps
	return 1;
}

//----- (0044BA6B) --------------------------------------------------------
void NThreadSetDeltaRequest()
{
	NetTurnNeedSync = 0x80000000;
}

//----- (0044BA76) --------------------------------------------------------
void __fastcall NetThreadStart(int slotToConnect)
{
	uchar v1;
	uint maxNetMsgSize;
	uint packetSize;
	CAPS caps;
	MainFrameTimer = GetTickCountTh();
	sgbPacketCountdown = 1;
	sgbSyncCountdown = 1;
	sgbGotPacketOK = 1;
	if( slotToConnect ){
		NThreadSetDeltaRequest();
	}else{
		NetTurnNeedSync = 0;
	}
	caps.Size = 36;
	if( !SNetGetProviderCaps( &caps ) ){
		TerminateWithError("SNetGetProviderCaps:\n%s", GetErrorTextForLastError());
	}
	// UDPN                       SCBL
    // Size: 36                   Size: 36
    // Flags: 3                   Flags: 0
    // maxmessagesize: 488        maxmessagesize: 496
    // Unk_0x0C: 16               Unk_0x0C: 16
    // DisplayedPlayerCount: 256  DisplayedPlayerCount: 4
    // Timeout: 100000            Timeout: 1500
    // PlayerLatency: 50          PlayerLatency: 500
    // PlayerCount: 8             PlayerCount: 4
    // CallDelay: 2               CallDelay: 2
	gdwTurnsInTransit = caps.DefaultTurnsInTransit;
	if( ! caps.DefaultTurnsInTransit )
		gdwTurnsInTransit = 1;
	if( caps.DefaultTurnsSec <= ENGINE_FPS && caps.DefaultTurnsSec )
		v1 = uchar( ENGINE_FPS / caps.DefaultTurnsSec );
	else
		v1 = 1;
	maxNetMsgSize = 512;
	sgbNetUpdateRate = v1;
	if( caps.MaxMessageSize < 512 ){
		maxNetMsgSize = caps.MaxMessageSize;
	}
	DeltaBytesPerSecond = caps.BytesSec / 4;
	MaxNetMsgSize = maxNetMsgSize;
	if( caps.MaxPlayers > PlayersMax_4 ){
		caps.MaxPlayers = PlayersMax_4;
	}
	packetSize = ((3 * (v1 * caps.BytesSec / 20)) / 4) / caps.MaxPlayers;
	gdwNormalMsgSize = ((3 * (v1 * caps.BytesSec / 20)) / 4) / caps.MaxPlayers;
	if( packetSize < 128 ){
		do{
			packetSize *= 2;
			v1 *= 2;
		}while( packetSize < 128 );
		sgbNetUpdateRate = v1;
		gdwNormalMsgSize = packetSize;
	}
	if( packetSize > maxNetMsgSize ){
		gdwNormalMsgSize = maxNetMsgSize;
	}
	if( MaxCountOfPlayersInGame > 1 ){
		FrameSyncCritSecState = 0;
		EnterCriticalSection(&FrameSyncCritSection);
		NetThreadRun = true;
		NetFrameSyncThreadHandle = (HANDLE) _beginthreadex(0, 0, NetSyncThread, 0, 0, &SyncNetFrameTimeThreadId);
		if( NetFrameSyncThreadHandle == INVALID_HANDLE_VALUE ){
			TerminateWithError("nthread2:\n%s", GetErrorTextForLastError());
		}
		SetThreadPriority(NetFrameSyncThreadHandle, THREAD_PRIORITY_HIGHEST );
	}
}

//----- (0044BBE3) --------------------------------------------------------
uint __stdcall NetSyncThread(void* arg1)
{
	int sendAsync = 0;
	while( NetThreadRun ){
		EnterCriticalSection(&FrameSyncCritSection);
		NThreadFillSyncQueue( 0, 0 );
		i64 tickDelta = NThreadMsgCheck( &sendAsync ) ? MainFrameTimer - GetTickCountTh() : MAX_FPS / ENGINE_FPS; // 20 fps
		LeaveCriticalSection(&FrameSyncCritSection);
		SleepTh( tickDelta );
	}
	return 0;
}

//----- (0044BC59) --------------------------------------------------------
void NetThreadStop()
{
	NetThreadRun = false;
	gdwTurnsInTransit = 0;
	gdwNormalMsgSize = 0;
	MaxNetMsgSize = 0;
	if( NetFrameSyncThreadHandle != INVALID_HANDLE_VALUE && SyncNetFrameTimeThreadId != GetCurrentThreadId() ){
		if( !FrameSyncCritSecState ){
			LeaveCriticalSection(&FrameSyncCritSection);
		}
		if( WaitForSingleObject(NetFrameSyncThreadHandle, INFINITE) == WAIT_FAILED ){
			TerminateWithError("nthread_cleanup:\n(%s)", GetErrorTextForLastError());
		}
		CloseHandle(NetFrameSyncThreadHandle);
		NetFrameSyncThreadHandle = INVALID_HANDLE_VALUE;
	}
}

//----- (0044BCD8) --------------------------------------------------------
void __fastcall ResetFrameSyncCritSection(int leave)
{
	if( NetFrameSyncThreadHandle != INVALID_HANDLE_VALUE ){
		if( leave ){
			LeaveCriticalSection(&FrameSyncCritSection);
		}else{
			EnterCriticalSection(&FrameSyncCritSection);
		}
		FrameSyncCritSecState = leave;
	}
}

//----- (0044BD03) --------------------------------------------------------
bool NThreadRunGameLoop(bool reloop)
{
	i64 msNow = GetTickCountTh();
	i64 delta = msNow - MainFrameTimer;

	// if it has been too long since we last ran a game loop,
	// then pretend we have caught up and are running normally
	if( MaxCountOfPlayersInGame == 1 && delta > CatchUpDelta_500 ){
		MainFrameTimer = msNow;
		delta = 0;
	}

	return delta >= 0;
}
