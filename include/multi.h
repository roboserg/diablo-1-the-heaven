#ifndef _multi_h_
#define _multi_h_
#else
#error reinclude multi.h
#endif

#pragma pack(push, 1)

// this structure contains a whole bunch of link packets
struct TMegaPkt
{
    struct TMegaPkt* pNext;
    DWORD dwSpaceLeft;
    uchar data[32000];
};

static_assert( sizeof( TMegaPkt ) == 32008, "Wrong TMegaPkt size" );

struct TBuffer
{
    DWORD dwNextWriteOffset;
    uchar bData[4096];
};

static_assert( sizeof( TBuffer ) == 4100, "Wrong TBuffer size" );

struct TPktHdr
{
    short px;
    short py;
    short targx;
    short targy;
    DWORD php;
    DWORD pmhp;
    short bstr;// Trait Bruiser can cause an overflow once BaseStrength exceeds 255, need larger packet size
    short bmag;
    short bdex;
    short bvit;// This could potentially fix player stun animation desync
    WORD wCheck;
    WORD wLen;
};

//static_assert( sizeof( TPktHdr ) == 20, "Wrong TPktHdr size" );

enum { MAX_MSG_SIZE = 512 };

struct TPkt
{
    TPktHdr hdr;
    uchar body[MAX_MSG_SIZE - sizeof( TPktHdr )];
};

static_assert( sizeof( TPkt ) == MAX_MSG_SIZE, "Wrong TPkt size" );

struct TCmdPlrInfoHdr
{
    uchar bCmd;
    WORD wOffset;
    WORD wBytes;
};

static_assert( sizeof( TCmdPlrInfoHdr ) == 5, "Wrong TCmdBigDataHdr size" );

struct TMegaPktPartHdr
{
    TPktHdr hdr;
    TCmdPlrInfoHdr cmdhdr;
};

//static_assert( sizeof( TMegaPktPartHdr ) == 25, "Wrong TMegaPktPartHdr size" );

struct TMegaPktPartPkt
{
    TMegaPktPartHdr hdr;
    uchar body[sizeof( TPkt ) - sizeof( TMegaPktPartHdr )];
};

static_assert( sizeof( TMegaPktPartPkt ) == MAX_MSG_SIZE, "Wrong TMegaPktPartPkt size" );

#pragma pack(pop)

enum BUFFER
{
    // msg buffering off when program running in normal game mode
    BUFFER_OFF = 0,

    // msg buffering on when program waiting for level delta info,
    // because program cannot handle any other messages besides deltas
    BUFFER_ON = 1,

    // processing msgs which were stored in the message buffer
    BUFFER_PROCESS = 2
};

extern bool gbSomebodyWonGameKludge;//
extern char szPlayerName[128];
extern char szPlayerDescript[128];//
extern __int16 sgwPackPlrOffsetTbl[PlayersMax_4];//
extern char NetPlayerCount;//
extern char gbGameDestroyed;//
extern char gbSelectProvider;//
extern uchar MaxCountOfPlayersInGame;//	1 == Single Player
extern uchar gbDeltaSender;//
extern uint player_state[PlayersMax_4];//

void multi_msg_add( uchar* packet, uchar size );
void NetSendLoPri( uchar* src, uchar size );
void multi_copy_packet( TBuffer* buf, void* packet, uchar size );
void multi_send_packet( void* packet, uchar size );
void build_pkt_hdr( TPkt* packet );
void NetSendHiPri( uchar* msg = nullptr, uchar size = 0 );
uchar* multi_recv_packet( TBuffer* packet, uchar* body, int* size );
void NetSendMask( uint pmask, uchar* body, uchar bodySize );
void process_turn();
void multi_parse_turn( int playerIndex, int turn );
void multi_handle_turn_upper_bit( int playerIndex );
void UnbufferRemovePlayer( int playerIndex, int reason );
void multi_clear_left_tbl();
void multi_player_left_msg( int playerIndex, bool verbose );
void multi_net_ping();
bool NetEndSendCycle();
bool multi_check_pkt_valid( TBuffer* buffer );
void multi_mon_seeds();
void multi_begin_timeout();
void multi_check_drop_player();
void NetReceivePackets();
void ProcessMsgBody( int playerIndex, uchar* data, int size );
void HandleLocalMsgs();
void multi_send_mega_packet_by_parts( uint playerIndex, uchar netCmdIndex, uchar* srcBodyPtr, int size );
void NetClose();
void RegisterEventHandler( bool add );
void __stdcall multi_handle_events( S_EVT* event );
bool NetInit( bool singleGame, bool* exitProgram );
bool multi_init_single( client_info* client_info, user_info* user_info, battle_info* ui_info );
bool multi_init_multi( client_info* client_info, user_info* user_info, battle_info* ui_info, bool* pfExitProgram );
void buffer_init( TBuffer* buffer );
void multi_send_pinfo( int playerIndex, char netCmdIndex );
void SetupLocalCoords();
bool multi_upgrade( bool* pfExitProgram );
void recv_plrinfo( int playerIndex, const TCmdPlrInfoHdr* p, bool recv );
