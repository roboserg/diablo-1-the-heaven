#include "stdafx.h"

bool gbSomebodyWonGameKludge = false;
TBuffer sgHiPriBuf;
char szPlayerName[128];
char szPlayerDescript[128];
__int16 sgwPackPlrOffsetTbl[PlayersMax_4];//
static LastPlayerInfo netplr[PlayersMax_4];// 006DBBB8
static char sgbPlayerTurnBitTbl[PlayersMax_4];
static char sgbPlayerLeftGameTbl[PlayersMax_4];
static uint sgbSentThisCycle; // 006DCF88
static bool gbShouldValidatePackage = false;
char NetPlayerCount;// 006DCF90 gbActivePlayers
char gbGameDestroyed;
static uchar sgbSendDeltaTbl[PlayersMax_4]; //sgbNetUpdateRate
static GameTemplate sgGameInitInfo;
char gbSelectProvider; // byte_678640
static i64 sglTimeoutStart;
static int sgdwPlayerLeftReasonTbl[PlayersMax_4];
static TBuffer sgLoPriBuf;
static unsigned int sgdwGameLoops;
uchar MaxCountOfPlayersInGame; // 006DDFC8 gbMaxPlayers
static bool sgbTimeout = false;
uchar gbDeltaSender;
static int sgbNetInited;
uint player_state[PlayersMax_4];

//----- (0044A37C) --------------------------------------------------------
void multi_msg_add( uchar* packet, uchar size )
{
    if( packet && size ){
        tmsg_add( packet, size );
    }
}

//----- (0044A38A) --------------------------------------------------------
void NetSendLoPri( uchar* src, uchar size )
{
    if( src && size ){
        multi_copy_packet( &sgLoPriBuf, src, size );
        multi_send_packet( src, size );
    }
}

//----- (0044A3B8) --------------------------------------------------------
void multi_copy_packet( TBuffer* buf, void* packet, uchar size )
{
    if( buf->dwNextWriteOffset + size + 2 <= 0x1000 ){
        uchar* p = &buf->bData[buf->dwNextWriteOffset];
        buf->dwNextWriteOffset += size + 1;
        *p = size;
        p++;
        memcpy( p, packet, size );
        p[size] = 0;
    }
}

//----- (0044A3FA) --------------------------------------------------------
void multi_send_packet( void* packet, uchar size )
{
    TPkt pkt;
    build_pkt_hdr( &pkt );
    pkt.hdr.wLen = size + sizeof( pkt.hdr );
    memcpy( pkt.body, packet, size );
    if( !SNetSendMessage( CurrentPlayerIndex, &pkt.hdr, pkt.hdr.wLen ) ){
        nthread_terminate_game( "SNetSendMessage0" );
    }
}

//----- (0044A462) --------------------------------------------------------
void build_pkt_hdr( TPkt* packet )
{
    const auto& player = Players[CurrentPlayerIndex];
    
    packet->hdr.wCheck = 'ip';
    packet->hdr.px = player.Row;
    packet->hdr.py = player.Col;
    packet->hdr.targx = player.TargetRow;
    packet->hdr.targy = player.TargetCol;
    packet->hdr.php = player.CurLife;
    packet->hdr.pmhp = player.MaxCurLife;
    packet->hdr.bstr = player.BaseStrength;
    packet->hdr.bmag = player.BaseMagic;
    packet->hdr.bdex = player.BaseDexterity;
    packet->hdr.bvit = player.BaseVitality;
}

//----- (0044A54F) --------------------------------------------------------
void NetSendHiPri( uchar* msg, uchar size )
{
    if( msg && size ){
        multi_copy_packet( &sgHiPriBuf, msg, size );
        multi_send_packet( msg, size );
    }
    if( !gbShouldValidatePackage ){
        gbShouldValidatePackage = true;
        TPkt pkt;
        build_pkt_hdr( &pkt );
        int maxSize = gdwNormalMsgSize - sizeof( pkt.hdr );
        uchar* hipri_body = multi_recv_packet( &sgHiPriBuf, pkt.body, &maxSize );
        uchar* lowpri_body = multi_recv_packet( &sgLoPriBuf, hipri_body, &maxSize );
        int finalSize = sync_packData( lowpri_body, maxSize );
        int len = gdwNormalMsgSize - finalSize;
        pkt.hdr.wLen = len;
        if( !SNetSendMessage( SNPLAYER_OTHERS, &pkt, len ) )
        {
            nthread_terminate_game( "SNetSendMessage" );
        }
    }
}

//----- (0044A609) --------------------------------------------------------
uchar* multi_recv_packet( TBuffer* packet, uchar* body, int* size )
{
    uchar* src_ptr;
    int chunk_size;
    
    if( packet->dwNextWriteOffset != 0 ){
        src_ptr = packet->bData;
        while( true ){
            if( *src_ptr == 0 ){
                break;
            }
            chunk_size = *src_ptr;
            if( chunk_size > *size ){
                break;
            }
            src_ptr++;
            memcpy( body, src_ptr, chunk_size );
            body += chunk_size;
            src_ptr += chunk_size;
            *size -= chunk_size;
        }
        memcpy( packet->bData, src_ptr, ( packet->bData - src_ptr ) + packet->dwNextWriteOffset + 1 );
        packet->dwNextWriteOffset += ( packet->bData - src_ptr );
        return body;
    }
    return body;
}

//----- (0044A672) --------------------------------------------------------
void NetSendMask( uint pmask, uchar* body, uchar bodySize )
{
    TPkt pkt;
    build_pkt_hdr( &pkt );
    WORD fullMsgSize = bodySize + sizeof( pkt.hdr );
    pkt.hdr.wLen = fullMsgSize;
    memcpy( pkt.body, body, bodySize );
    uint pbit = 1;
    for( int p = 0; p < PlayersMax_4; p++ ){
        if( pbit & pmask ){
            if( !SNetSendMessage( p, &pkt.hdr, fullMsgSize ) && GetLastError() != STORM_ERROR_INVALID_PLAYER ){
                nthread_terminate_game( "SNetSendMessage" );
                return;
            }
        }
        pbit <<= 1;
    }
}

//----- (0044A6F3) --------------------------------------------------------
void process_turn()
{
    for( int playerIndex = 0; playerIndex < PlayersMax_4; ++playerIndex ){
        if( player_state[playerIndex] & 0x20000 ){
            if( gdwMsgLenTbl[playerIndex] == 4 ){
                multi_parse_turn( playerIndex, *glpMsgTbl[playerIndex] );
            }
        }
    }
}

//----- (0044A72A) --------------------------------------------------------
void multi_parse_turn( int playerIndex, int turn )
{
    if( turn & 0x80000000 ){ // if (turn >> 31)
        multi_handle_turn_upper_bit( playerIndex );
    }
    int absTurns = turn & ~0x80000000; // & 0x7FFFFFFF
    if( sgbSentThisCycle < absTurns + gdwTurnsInTransit ){
        if( absTurns >= 0x7FFFFFFF ){
            absTurns &= 0xFFFF;
        }
        sgbSentThisCycle = absTurns + gdwTurnsInTransit;
        sgdwGameLoops = 4 * sgbNetUpdateRate * absTurns;
    }
}

//----- (0044A777) --------------------------------------------------------
void multi_handle_turn_upper_bit( int playerIndex )
{
    int serverPlayer = 0;
    for( ; serverPlayer < PlayersMax_4; ++serverPlayer ){
        if( player_state[serverPlayer] & 0x10000 && serverPlayer != playerIndex ){
            break;
        }
    }
    if( serverPlayer == CurrentPlayerIndex ){
        sgbSendDeltaTbl[playerIndex] = true;
    }else if( playerIndex == CurrentPlayerIndex ){
        gbDeltaSender = serverPlayer;
    }
}

//----- (0044A7B0) --------------------------------------------------------
void UnbufferRemovePlayer( int playerIndex, int reason )
{
    sgbPlayerLeftGameTbl[playerIndex] = true;
    sgdwPlayerLeftReasonTbl[playerIndex] = reason;
    multi_clear_left_tbl();
}

//----- (0044A7C3) --------------------------------------------------------
void multi_clear_left_tbl()
{
    for( int i = 0; i < PlayersMax_4; i++ ){
        if( sgbPlayerLeftGameTbl[i] ){
            if( gbBufferMsgs == BUFFER_ON ){
                msg_send_drop_pkt( i, sgdwPlayerLeftReasonTbl[i] );
            }else{
                multi_player_left_msg( i, true );
            }
            sgbPlayerLeftGameTbl[i] = false;
            sgdwPlayerLeftReasonTbl[i] = 0;
        }
    }
}

// replaced with 0044A7C3
//----- (0044A80C) --------------------------------------------------------
void multi_player_left_msg( int playerIndex, bool verbose )
{
    Player& player = Players[playerIndex];
    if( player.IsExists ){
        RemoveGolemAndMissilesMayBe( playerIndex );
        RemovePlayerFromMap( playerIndex );
        ClearAllTownPortalsOwnedThisPlayer( playerIndex );
        DeactivatePortal( playerIndex );
        DeltaClosePortal( playerIndex );
        RemovePlayerMissiles( playerIndex );
        if( verbose ){
            const char* fmt = "Player '%s' just left The Game";
            if( sgdwPlayerLeftReasonTbl[playerIndex] == 0x40000004 ){
                fmt = "Player '%s' killed Diablo and left The Game!";
                gbSomebodyWonGameKludge = true;
            }else if( sgdwPlayerLeftReasonTbl[playerIndex] == 0x40000006 ){
                fmt = "Player '%s' dropped due to timeout";
            }
            AddNewPlayerStatusFormattedMessage( fmt, player.playerName );
        }
        player.IsExists = false;
        player.playerName[0] = 0;
        --NetPlayerCount;
    }
}

//----- (0044A8B2) --------------------------------------------------------
void multi_net_ping()
{
    sgbTimeout = true;
    sglTimeoutStart = GetTickCountTh(); // сетевая игра, перехватываем вызов
}

//----- (0044A8C5) --------------------------------------------------------
bool NetEndSendCycle()
{
    if( gbGameDestroyed ){
        gbRunGame = false;
        return false;
    }
    for( int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++ ){
        if( sgbSendDeltaTbl[playerIndex] ){
            sgbSendDeltaTbl[playerIndex] = false;
            DeltaSendAllLevels( playerIndex );
        }
    }
    sgbSentThisCycle = NThreadFillSyncQueue( sgbSentThisCycle, 1 );
    int recieved;
    if( !NThreadMsgCheck( &recieved ) ){
        multi_begin_timeout();
        return false;
    }
    sgbTimeout = false;
    if( recieved ){
        if( gbShouldValidatePackage ){
            gbShouldValidatePackage = false;
            if( !multi_check_pkt_valid( &sgHiPriBuf ) ){
                NetSendHiPri();
            }
        }else{
            NetSendHiPri();
            gbShouldValidatePackage = false;
        }
    }
#ifdef MONSTER_NET_RANDOMIZE // отключена дополнительную рандомизацию монстров
    multi_mon_seeds();
#endif
    return true;
}

//----- (0044A971) --------------------------------------------------------
bool multi_check_pkt_valid( TBuffer* buffer )
{
    return buffer->dwNextWriteOffset == 0;
}

//----- (0044A979) --------------------------------------------------------
void multi_mon_seeds()
{
    int j = _rotr( ++sgdwGameLoops, 8 );
    for( int i = 0; i < Monsters_Max_Count; ++i )
    {
        Monsters[i].RandomSeedForAction = i + j;
    }
}

//----- (0044A9AC) --------------------------------------------------------
void multi_begin_timeout()
{
    if( !sgbTimeout ) return;
    
    i64 ticks = GetTickCountTh() - sglTimeoutStart;
    if( ticks > 20000_ms ){
        gbRunGame = false;
        return;
    }
    if( ticks < 10000_ms ){
        return;
    }
    int groupPlayers = 0;
    int groupCount = 0;
    int lowestActive = -1;
    int lowestPlayer = -1;
    
    for( int i = 0; i < PlayersMax_4; i++ ){
        auto state = player_state[i];
        if( state & 0x10000 ){
            if( lowestPlayer == -1 ){
                lowestPlayer = i;
            }
            if( state & 0x40000 ){
                groupPlayers++;
                if( lowestActive == -1 ){
                    lowestActive = i;
                }
            }else{
                groupCount++;
            }
        }
    }
    if( groupPlayers < groupCount ){
        gbGameDestroyed = true;
    }else if( groupPlayers == groupCount ){
        if( lowestPlayer != lowestActive ){
            gbGameDestroyed = true;
        }else if( lowestActive == CurrentPlayerIndex ){
            multi_check_drop_player();
        }
    }else if( lowestActive == CurrentPlayerIndex ){
        multi_check_drop_player();
    }
}

//----- (0044AA54) --------------------------------------------------------
void multi_check_drop_player()
{
    for( int playerIndex = 0; playerIndex < PlayersMax_4; ++playerIndex ){
        uint playerStatus = player_state[playerIndex];
        if( !( playerStatus & 0x40000 ) && playerStatus & 0x10000 ){
            SNetDropPlayer( playerIndex, 0x40000006 );
        }
    }
}

//----- (0044AA87) --------------------------------------------------------
void NetReceivePackets()
{
    multi_clear_left_tbl();
    HandleLocalMsgs();
    
    uint playerIndex;
    char* packet;
    uint size;
    while( SNetReceiveMessage( &playerIndex, (void**)&packet, &size ) ){
        ++pkt_counter;
		//if( size > 30 ) __debugbreak();
		multi_clear_left_tbl();
        auto pkt = (TPktHdr*)packet;
        
        if( size < sizeof( TPktHdr )
         || playerIndex >= PlayersMax_4
         || pkt->wCheck != 'ip'
         || pkt->wLen != size ){
            continue;
        }
        auto& player = Players[playerIndex];
        player.ownerRow = pkt->px;
        player.ownerCol = pkt->py;
        if( playerIndex != CurrentPlayerIndex ){
            player.CurLife = pkt->php;
            player.MaxCurLife = pkt->pmhp;
            player.BaseStrength = pkt->bstr;
            player.BaseMagic = pkt->bmag;
            player.BaseDexterity = pkt->bdex;
            player.BaseVitality = pkt->bvit;
            if( gbBufferMsgs != BUFFER_ON ){
                if( player.IsExists ){
                    if( player.CurLife ){
                        if( Dungeon != player.dungeon || player.lvlChanging ){
                            player.Row = pkt->px;
                            player.Col = pkt->py;
                            player.NextRow = pkt->px;
                            player.NextCol = pkt->py;
                            player.TargetRow = pkt->targx;
                            player.TargetCol = pkt->targy;
                        }else{
                            int dRow = abs( player.Row - pkt->px );
                            int dCol = abs( player.Col - pkt->py );
                            if( !( dRow <= 3 && dCol <= 3 ) && !PlayerMap[pkt->px][pkt->py] ){
                                FixPlrWalkTags( playerIndex );
                                player.oldRow = player.Row;
                                player.oldCol = player.Col;
                                FixPlrWalkTags( playerIndex );
                                player.Row = pkt->px;
                                player.Col = pkt->py;
                                player.NextRow = pkt->px;
                                player.NextCol = pkt->py;
                                PlayerMap[player.Row][player.Col] = playerIndex + 1;
                            }
                            dRow = abs( player.NextRow - player.Row );
                            dCol = abs( player.NextCol - player.Col );
                            if( dRow > 1 || dCol > 1 ){
                                player.NextRow = player.Row;
                                player.NextCol = player.Col;
                            }
                            MakePlayerPath( playerIndex, pkt->targx, pkt->targy, 1 );
                        }
                    }
                }
            }
        }
		//if( *(uchar*)( pkt + 1 ) == CMD_2_ACK_PLRINFO ) __debugbreak();
		ProcessMsgBody( playerIndex, (uchar*)( pkt + 1 ), size - sizeof( *pkt ) );
    }
    if( GetLastError() != STORM_ERROR_NO_MESSAGES_WAITING ){
        nthread_terminate_game( "SNetReceiveMsg" );
    }
}

//----- (0044AE35) --------------------------------------------------------
void ProcessMsgBody( int playerIndex, uchar* data, int size )
{
    while( size > 0 ){
        int cmdLen = ParseCmd( playerIndex, (TCmd*)data );

        // zero is an error condition:
        // skip the rest of the packet
        if( cmdLen == 0 ){
            break;
        }

        data += cmdLen;
        size -= cmdLen;
    }
}

//----- (0044AE5C) --------------------------------------------------------
void HandleLocalMsgs()
{
    uchar msg[MAX_MSG_SIZE];
    while( int size = tmsg_get( msg, sizeof(msg) ) ){
        ProcessMsgBody( CurrentPlayerIndex, msg, size );
    }
}

//----- (0044AE8F) --------------------------------------------------------
void multi_send_mega_packet_by_parts( uint playerIndex, uchar netCmdIndex, uchar* srcBodyPtr, int size )
{
    TMegaPktPartPkt pkt;
    ushort offset = 0;
    while( size ){
        pkt.hdr.hdr.wCheck = 'ip';
        pkt.hdr.hdr.px = 0;
        pkt.hdr.hdr.py = 0;
        pkt.hdr.hdr.targx = 0;
        pkt.hdr.hdr.targy = 0;
        pkt.hdr.hdr.php = 0;
        pkt.hdr.hdr.pmhp = 0;
        pkt.hdr.hdr.bstr = 0;
        pkt.hdr.hdr.bmag = 0;
        pkt.hdr.hdr.bdex = 0;
        pkt.hdr.hdr.bvit = 0;

        pkt.hdr.cmdhdr.bCmd = netCmdIndex;
        pkt.hdr.cmdhdr.wOffset = offset;
        
        int bodySize = MaxNetMsgSize - sizeof( TMegaPktPartHdr );
        if( bodySize > size ){
            bodySize = size;
        }
        pkt.hdr.cmdhdr.wBytes = bodySize;
        memcpy( &pkt.body, srcBodyPtr, bodySize );
        pkt.hdr.hdr.wLen = bodySize + sizeof( TMegaPktPartHdr );
        if( !SNetSendMessage( playerIndex, &pkt, pkt.hdr.hdr.wLen ) ){
            nthread_terminate_game( "SNetSendMessage2" );
            return;
        }
        srcBodyPtr += bodySize;
        size -= bodySize;
        offset += bodySize;
    }
}

//----- (0044AF73) --------------------------------------------------------
void NetClose()
{
    if( sgbNetInited ){
        sgbNetInited = false;
        NetThreadStop();
        DeltaThreadStop();
        tmsg_cleanup();
        RegisterEventHandler( false );
        SNetLeaveGame( 3 );
        if( MaxCountOfPlayersInGame > 1 ){
            SleepTh( 1000 ); // give storm enough time to send leave pkt // original 2000
        }
    }
}

//----- (0044AFB5) --------------------------------------------------------
void RegisterEventHandler( bool reg )
{
    static const int events[] = { NE_3_PLAYER_LEAVE, NE_1_INITDATA, NE_4_SERVERMESSAGE };
    int (__stdcall* regFn)( int, int ) = reg ? SNetRegisterEventHandler : SNetUnregisterEventHandler;
    for( int evnt : events ){
        if( ! regFn( evnt, (int)multi_handle_events ) && reg ){
            TerminateWithError( "SNetRegisterEventHandler:\n%s", GetErrorTextForLastError() );
        }
    }
}

//----- (0044AFFF) --------------------------------------------------------
void __stdcall multi_handle_events( S_EVT* event )
{
    int leaveReason; // ecx
	GameTemplate* eventData; // eax
    
    switch( event->flags ){
        case NE_1_INITDATA:
            eventData = (GameTemplate*) event->data;
            sgGameInitInfo.seed = eventData->seed;
            sgGameInitInfo.difficulty = eventData->difficulty;
            sgbPlayerTurnBitTbl[event->playerIndex] = 1;
            break;
        case NE_3_PLAYER_LEAVE:
            leaveReason = 0;
            sgbPlayerLeftGameTbl[event->playerIndex] = true;
            sgbPlayerTurnBitTbl[event->playerIndex] = false;
            if( event->data && event->dataSize >= sizeof(int) )
            {
                leaveReason = *(int*)event->data;
            }
            sgdwPlayerLeftReasonTbl[event->playerIndex] = leaveReason;
            if( leaveReason == 0x40000004 )
            {
                gbSomebodyWonGameKludge = true;
            }
            sgbSendDeltaTbl[event->playerIndex] = 0;
            DeltaRemovePlayer( event->playerIndex );
            if( gbDeltaSender == event->playerIndex )
            {
                gbDeltaSender = PlayersMax_4;
            }
            break;
        case NE_4_SERVERMESSAGE:
            ErrorPlrMsg( (char*)event->data );
            break;
    }
}

//----- (0044B0A7) --------------------------------------------------------
bool NetInit( bool singleGame, bool* exitProgram )
{
    while( true ){
        *exitProgram = false;
        char playerDescription[256];// [sp+Ch] [bp-134h]@2
        CharSaveInfo charInfo;        // [sp+8Ch] [bp-B4h]@3
        client_info clientInfo;        // [sp+F0h] [bp-50h]@4
        user_info userInfo;            // [sp+128h] [bp-18h]@4
        battle_info battleInfo;        // [sp+BCh] [bp-84h]@4
        memset( playerDescription, 0, sizeof( playerDescription ) );
        if( !singleGame ){
            CurrentPlayerIndex = 0;
            LoadCurrentPlayerInfo();
            InitCharData( Players[0], *( (CharSaveInfo*)&charInfo ), gbValidSaveFile );
            if( !UiCreatePlayerDescription( &charInfo, SaveSignature, playerDescription ) ){
                return false;
            }
        }
        SetRndSeed( 0 );
        sgGameInitInfo.seed = (uint) time( nullptr );
        sgGameInitInfo.difficulty = Difficulty;
        
        memset( &clientInfo, 0, sizeof( client_info ) );
        // вылетало при выборе второй сложности из-за порчи памяти в hell.dll при загрузке по не своему адресу
        // срабатывало через раз запрет на выбор второй сложности там же
        // проверить почему не реагирует на нажатия кнопок при проигрывании видео
        clientInfo.Size = sizeof( client_info ); // пробую с оригинальным размером 44, в th1 сейчас 60 (client_info2)
        clientInfo.Name = GameClientName;
        clientInfo.Version = GameVersionString;
        // clientInfo.Product = 'HRTL'; // Hellfire Retail (original hellfire)
        clientInfo.Product = (('4H' + charInfo.GameMode) << 16) | TH4_REVISION; //th2 ? TH2_REVISION : TH1_REVISION; // 1216 - The Hell version
        clientInfo.Verbyte = 42; // 43 // раньше было 34
        clientInfo.gameCriteriaData = 0;
        clientInfo.MaxPlayers = 4;
        clientInfo.gameTemplate = &sgGameInitInfo;
        clientInfo.gameTemplateSize = 8;
        clientInfo.Unk9 = 0;
        clientInfo.Unk10 = 15;
        
        memset( &userInfo, 0, sizeof( user_info ) ); // этого вызова нет в оригинальном коде
        userInfo.Size = sizeof( user_info ); // 12
        userInfo.PlayerName = HeroName;
        userInfo.playerDescription = playerDescription;
        
        memset( &battleInfo, 0, sizeof( battle_info ) );
        battleInfo.Size = sizeof( battle_info ); // 52
        battleInfo.UnkType = 0;
        battleInfo.FrameWnd = SDrawGetFrameWindow( 0 );
        battleInfo.BattleGetResource = UiArtCallback;
        battleInfo.BattleGetErrorString = UiAuthCallback;
        battleInfo.BattleMakeCreateGameDialog = UiCreateGameCallback;
        battleInfo.BattleUpdateIcons = UiDrawDescCallback;
        battleInfo.Unk_07 = 0;
        battleInfo.BattleErrorDialog = UiMessageBoxCallback;
        battleInfo.BattlePlaySound = UiSoundCallback;
        battleInfo.BattleConnectionProgress = 0;
        battleInfo.BattleGetCursorLink = UiGetDataCallback;
        battleInfo.Unk_12 = UiCategoryCallback;
        
        memset( sgbPlayerTurnBitTbl, 0, sizeof( sgbPlayerTurnBitTbl ) );
        gbGameDestroyed = 0;
        memset( sgbPlayerLeftGameTbl, 0, sizeof( sgbPlayerLeftGameTbl ) );
        memset( sgdwPlayerLeftReasonTbl, 0, sizeof( sgdwPlayerLeftReasonTbl ) );
        memset( sgbSendDeltaTbl, 0, sizeof( sgbSendDeltaTbl ) );
        memset( Players, 0, sizeof( Player ) * 4 );
        memset( sgwPackPlrOffsetTbl, 0, sizeof( sgwPackPlrOffsetTbl ) );
        SNetSetBasePlayer( 0 );
        if( singleGame ){
            if( !multi_init_single( &clientInfo, &userInfo, &battleInfo ) ){
                return false;
            }
        }else{
            if( !multi_init_multi( &clientInfo, &userInfo, &battleInfo, exitProgram ) ){
                return false;
            }
        }
        sgbTimeout = 0;
        sgbNetInited = 1;
        delta_init();
        ClearPlayerStatusMessages();
        buffer_init( &sgHiPriBuf );
        buffer_init( &sgLoPriBuf );
        gbShouldValidatePackage = 0;
        sync_init();
        NetThreadStart( sgbPlayerTurnBitTbl[CurrentPlayerIndex] );
        DeltaThreadStart();
        tmsg_start();
        sgdwGameLoops = 0;
        sgbSentThisCycle = 0;
        gbDeltaSender = CurrentPlayerIndex;
        gbSomebodyWonGameKludge = 0;
        LoadCurrentPlayerInfo();
        NThreadFillSyncQueue( 0, 0 );
        SetupLocalCoords();
        multi_send_pinfo( -2, CMD_54_SEND_PLRINFO );
        Players[CurrentPlayerIndex].IsExists = 1;
        NetPlayerCount = 1;
        if( !sgbPlayerTurnBitTbl[CurrentPlayerIndex] ){
            break;
        }
        if( WaitDeltaInfo() )        {
            break;
        }
        NetClose();
        gbSelectProvider = 0;
    }
    Difficulty = sgGameInitInfo.difficulty;
    if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_16_GROUNDHOG)) {
        SetRndSeed(1 /*Players[CurrentPlayerIndex].CurrentSeedRun*/);    
    }else{
        SetRndSeed(sgGameInitInfo.seed);
        //InitRandomSeed(Players[CurrentPlayerIndex].CurrentSeedRun); <--- use this line if you want to restore Autosave player dungeon run (for debugging purpose)
        Players[CurrentPlayerIndex].CurrentSeedRun = sgGameInitInfo.seed;
    }
    for( int dun = 0; dun < DungeonCount; dun++ ){
        DungeonRandSeed[dun] = GetRndSeed(); // buggy dun 16 mobs seed 1946629605
    }
	//DungeonRandSeed[11] = 1351001788; // debug specific seed example
	//Players[CurrentPlayerIndex].dungeonVisited.reset();
    int byteswritten;
    if( !SNetGetGameInfo( GI_1_NAME, szPlayerName, sizeof( szPlayerName ), &byteswritten ) ){
        nthread_terminate_game( "SNetGetGameInfo1" );
    }
    if( !SNetGetGameInfo( GI_2_PASSWORD, szPlayerDescript, sizeof( szPlayerDescript ), &byteswritten ) ){
        nthread_terminate_game( "SNetGetGameInfo2" );
    }
    return 1;
}

//----- (th2) -------------------------------------------------------------
bool multi_init_single( client_info* client_info, user_info* user_info, battle_info* ui_info )
{
    if( !SNetInitializeProvider( 0, client_info, user_info, ui_info, &ModuleInfo ) ){
        TerminateWithError( "SNetInitializeProvider:\n%s", GetErrorTextForLastError() );
        return false;
    }
    int unused = 0;
    if( !SNetCreateGame( "local", "local", "local", 0, (char*)&sgGameInitInfo, 8, 1, "local", "local", &unused ) ){
        TerminateWithError( "SNetCreateGame1:\n%s", GetErrorTextForLastError() );
        return false;
    }
    CurrentPlayerIndex = 0;
    MaxCountOfPlayersInGame = 1;
    return true;
}

//----- (th2) -------------------------------------------------------------
bool multi_init_multi( client_info* client_info, user_info* user_info, battle_info* ui_info, bool* pfExitProgram )
{
    if( gbSelectProvider ){
        bool checkUpdate = true;
        int type;
        while( !UiSelectProvider( 0, client_info, user_info, ui_info, &ModuleInfo, &type ) ){
            if( !checkUpdate || GetLastError() != STORM_ERROR_REQUIRES_UPGRADE || !multi_upgrade( pfExitProgram ) ){
                return false;
            }
            checkUpdate = false;
        }
    }
    RegisterEventHandler( true );
    int playerIndex = 0;
    int selectGame = UiSelectGame( 1, client_info, user_info, ui_info, &ModuleInfo, &playerIndex );
    if( !selectGame || (uint)playerIndex >= PlayersMax_4 ){
        return false;
    }
    CurrentPlayerIndex = playerIndex;
    MaxCountOfPlayersInGame = PlayersMax_4;
    return true;
}

//----- (0044B4ED) --------------------------------------------------------
void buffer_init( TBuffer* buffer )
{
    buffer->dwNextWriteOffset = 0;
    buffer->bData[0] = 0;
}

//----- (0044B4F5) --------------------------------------------------------
void multi_send_pinfo( int playerIndex, char netCmdIndex )
{
    LastPlayerInfo playerInfo;
    SavePlayerInfo( &playerInfo, CurrentPlayerIndex );
    DThreadSendPLayerInfoChunk( playerIndex, netCmdIndex, &playerInfo, sizeof( playerInfo ) );
}

//----- (0044B580) --------------------------------------------------------
void SetupLocalCoords()
{
    // do not reset dungeon in debug single mode
    if( !DungeonDebug || MaxCountOfPlayersInGame > 1 ){
        Dungeon = DUN_0_TOWN;
    }
    Player& player = Players[CurrentPlayerIndex];

    // adjust character position for character number
    // so everyone doesn't start on the same location
    int row = PlrXoff[CurrentPlayerIndex] + 75;
    int col = PlrYoff[CurrentPlayerIndex] + 68;
    player.Row = row;
    player.Col = col;
    player.NextRow = row;
    player.NextCol = col;
    player.TargetRow = row;
    player.TargetCol = col;
    player.lvlChanging = 1;
    player.SomeSoundMuteDelay = 0;
    player.CurAction = PCA_10_NEW_LEVEL;
    player.destAction = PCMD_M1_NOTHING;
}

//----- (0044B6BD) --------------------------------------------------------
bool multi_upgrade( bool* pfExitProgram )
{
    int upgradestatus = 0;
    SNetPerformUpgrade( &upgradestatus );
    bool result = true;
    if( upgradestatus ){
        if( upgradestatus != 1 ){
            if( upgradestatus == 2 ){
                *pfExitProgram = true;
            }else{
                if( upgradestatus == -1 ){
                    ErrorBoxf( "Network upgrade failed" );
                }
            }
            result = false;
        }
    }
    return result;
}

//----- (0044B6F9) --------------------------------------------------------
void recv_plrinfo( int playerIndex, const TCmdPlrInfoHdr* p, bool recv )
{
    if( CurrentPlayerIndex == playerIndex ){
        return;
    }
    if( sgwPackPlrOffsetTbl[playerIndex] != p->wOffset ){
        sgwPackPlrOffsetTbl[playerIndex] = 0;
        if( p->wOffset != 0 ){
            return;
        }
    }
    if( !recv && !sgwPackPlrOffsetTbl[playerIndex] ){
        multi_send_pinfo( playerIndex, CMD_2_ACK_PLRINFO );
    }
    memcpy( ( (char*)&netplr[playerIndex] ) + p->wOffset, &p[1] /*data after header*/, p->wBytes );
    sgwPackPlrOffsetTbl[playerIndex] += p->wBytes;
    
    if( sgwPackPlrOffsetTbl[playerIndex] != sizeof( LastPlayerInfo ) ){
        return;// Если не вся информация пришла, ждём ещё
    }
    sgwPackPlrOffsetTbl[playerIndex] = 0;// зачем то обнуляем значение количества загруженной информации
    multi_player_left_msg( playerIndex, false );
    Players[playerIndex].LoadPlayerAnimationMask = PAF_0_NO;
    LoadPlayerInfo( &netplr[playerIndex], playerIndex, true );
    if( !recv ){
        // Если код команды не 2 выходим
        return;
    }
    Player& player = Players[playerIndex];
    player.IsExists = true;
    NetPlayerCount++;
    
    const char* fmt;
    if( sgbPlayerTurnBitTbl[playerIndex] == 0 ){
        fmt = "Player '%s' (level %d) just joined The Game";
    }else{
        fmt = "Player '%s' (level %d) is already in The Game";
    }
    AddNewPlayerStatusFormattedMessage( fmt, &player.playerName, player.CharLevel );
    LoadPlayerGFX( playerIndex, PAF_1_STAY );
    SyncInitPlr( playerIndex );
    if( player.dungeon == Dungeon ){
        if( ( player.CurLife & ~63 ) > 0 ){
            StartStand( playerIndex, 0 );
        }else{
            player.Appearance = 0;
            LoadPlayerGFX( playerIndex, PAF_8_DEATH );
            player.CurAction = PCA_8_DEATH;
            NewPlayerAnimation( playerIndex, player.deathAnimationsPointers[0], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath );
            player.currentAnimationFrame = player.currentAnimationFramesCount - 1;
            player.currentActionFrame = 2 * player.currentAnimationFramesCount;
            FlagMap[player.Row][player.Col] |= CF_4_DEAD_PLAYER;
        }
    }
}
