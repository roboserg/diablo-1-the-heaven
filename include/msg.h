#ifndef _msg_h_
#define _msg_h_
#else
#error reinclude msg.h
#endif
#pragma pack(push, 1)

// sizeof 1
struct TCmd
{
	char bCmd;// 0
};

// sizeof 3
struct TCmdParam1
{
	char netCmdIndex;// 0
	short firstArg;// 1
};

// sizeof 5
struct TCmdParam2
{
	char netCmdIndex;// 0
	short firstArg;// 1
	short secondArg;// 3
};

// sizeof 7
struct TCmdParam3
{
	char netCmdIndex;// 0
	short firstArg;// 1
	short secondArg;// 3
	short thirdArg;// 5
};

// sizeof 3
struct TCmdLoc
{
	char bCmd;// 0
	char row;// 1
	char col;// 2
};

// sizeof 5
struct TCmdLocParam1
{
	char netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
};

// sizeof 7
struct TCmdLocParam2
{
	uchar netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
	short secondArg; // 5
};

// sizeof 9
struct TCmdLocParam3
{
	char netCmdIndex;// 0
	char row;// 1
	char col;// 2
	short firstArg; // 3
	short secondArg; // 5
	short thirdArg; // 7
};

// sizeof 2
struct TCmdDelItem
{
	char netCmdIndex;// 0
	char onBodyIndex;// 1
};

// sizeof 8
struct TCmdQuest
{
	char netCmdIndex;	// 0
	char questIndex;	// 1
	QUEST_STATUS status;// 2
	char talkToFlag;	// 3
	char status2;		// 4
	char status3;		// 5
	char status4;		// 6
	char status5;		// 7
};

// sizeof 6
struct TCmdDamage
{
	char netCmdIndex;	// 0
	char targetPlayerIndex;	// 1
	int damage;		// 2
};

struct TCmdString {
	uchar bCmd;
	char str[MaxChatString_128];
};

// sizeof 7
struct TCmdDamageMonster
{
	char netCmdIndex;			// 0
	short targetMonsterIndex;	// 1
	int damage;				// 3
};

//Used for syncing monster.acidDot between players
struct TCmdAcidDotMonster
{
	char netCmdIndex;
	short targetMonsterIndex;
	int DeltaMonAcidDot;
	DUNGEON dungeon;
	int MonAcidDot;
	char dotLastAttacker;
};

//Used for syncing PvP state between players
struct TCmdPvP
{
	char netCmdIndex;
	char playerIndex;
	bool IsPlayerEnemy;
};

struct TCmdWallRecast
{
	char netCmdIndex;
	short firstArg;
	short secondArg;
	short thirdArg;
	uchar casterX;
	uchar casterY;
	uchar targetX;
	uchar targetY;
	uchar spellLevel;
	uchar direction; // Maybe can be re-computed...
};

// sizeof 11
struct TCmdChItem // player item
{
	char netCmdIndex;			// 0
	char onBodyIndex;			// 1
	LastItemInfo info;
};

// sizeof 38
struct TCmdGItem // ground item
{
	uchar netCmdIndex;
	char bMaster;
	char bPnum;
	uchar itemIndex;
	uchar itemIndex_reserved;
	DUNGEON dungeonLevel;
	char dungeon_reserved;
	short mapRow;
	short mapCol;
	LastItemInfo info;
	i64 timeStamp;
};

struct TFakeDropPlr
{
	uchar bCmd;
	uchar bPlr;
	DWORD dwReason;
};

static_assert( sizeof( TFakeDropPlr ) == 6, "Wrong TFakeDropPlr size" );

struct TFakeCmdPlr
{
	uchar bCmd;
	uchar bPlr;
};

static_assert( sizeof( TFakeCmdPlr ) == 2, "Wrong TFakeCmdPlr size" );

struct DObjectStr
{
	uchar bCmd;
};

static_assert( sizeof( DObjectStr ) == 1, "Wrong DObjectStr size" );

struct DMonsterStr
{
	uchar row;
	uchar col;
	uchar orientation;
	uchar target;
	uchar activationCounter;
	int life;
};

static_assert( sizeof( DMonsterStr ) == 9, "Wrong DMonsterStr size" );

// sizeof 10
struct TCmdAwakeSummon
{
	uchar bCmd;
	uchar summonIndex;
	char row;
	char col;
	DUNGEON dungeon;
	DSummonStr params;
};

// sizeof 69 th3 (th2 - 40, th1 - 30)
struct TCmdPItem
{
    uchar bCmd;
    short mapRow;
	short mapCol;
	LastItemInfo info;
};

struct DLevel
{
	TCmdPItem items[MaxItems_255];
	DObjectStr objects [MaxObjects_127];
	DMonsterStr monsters [Monsters_Max_Count];
	DSummonStr summons [SummonMonsters_Count];
};

struct LocalLevel
{
	bool inited;
	uchar automapsv[Map_40][Map_40];
	uchar countItemsOnMap;
	uchar mapItemsFreeIndexes[MaxItems_255];
	uchar itemsOnMapIndexesInvers[MaxItems_255];
	Item items[MaxItems_255];
	uchar itemsOnGroundMap[FineMap_112][FineMap_112];
};

struct DPortal
{
	uchar row;
	uchar col;
	DUNGEON dungeon;
};

static_assert( sizeof( DPortal ) == 3, "Wrong DPortal size" );

struct MultiQuests
{
	QUEST_STATUS status;
	uchar talkToFlag;
	uchar status2;
	uchar status3;
	uchar status4;
	uchar status5;
};

static_assert( sizeof( MultiQuests ) == 6, "Wrong MultiQuests size" );

struct DJunk
{
	DPortal portals [PlayersMax_4];
	MultiQuests quests [Quest_Count]; // was MultiQuestsCount_10
};

#pragma pack(pop)

enum NET_CONST
{
	INIT_VAL = 0xFF,

	// number of chunks of info we must receive to get all delta info
	// 24 levels + 1 town level + 20 quest levels = DungeonCount_45
	// + 1 global "junk" chunk
	// + 1 terminator
	// + 1 startup
	// + 1 "99%" complete
	DELTA_CHUNKS = DungeonCount + 4,

	// allocate extra space to account for flag byte
	SEND_LEVEL_SIZE = std::max(sizeof(DLevel), sizeof(DJunk)) + 1,

	// item flags which go into sgLevel[].item[].bCmd
	// item generated on floor of dungeon -- presently on floor
	ITEM_GEN_FLOOR = 0,

	// item generated on floor of dungeon -- presently not on floor
	ITEM_GEN_TAKEN = 1,

	// item was dropped -- presently on floor
	ITEM_DROP_FLOOR = 2,

	// we don't have to keep track of items which were not originally
	// part of the dungeon and were dropped and then picked up
	// item slot not in use
	ITEM_FREE = INIT_VAL,
};

constexpr i64 RESEND_TIME = 6000_ms;

extern bool NetItem;
extern bool deltaload;
extern uchar gbBufferMsgs;
extern int pkt_counter;

void msg_send_drop_pkt( int playerIndex, int playerEvent );
void mega_add_data( int playerIndex, const void* packet, uint size );
void  MegaAddPacket();
bool WaitDeltaInfo();
void  MegaFreePackets();
int  DeltaProgress();
void  RunDeltaInfo();
void  MegaRunPackets();
void DeltaSendAllLevels( int playerIndex );
uchar* DeltaBuildItems( uchar* dst, TCmdPItem* src );
uchar* DeltaBuildObjects( uchar* dst, DObjectStr* src );
uchar* DeltaBuildMonsters( uchar* dst, DMonsterStr* src );
uchar* DeltaBuildSummons( uchar* dst, DSummonStr* src );
uchar* DeltaExportJunk( uchar* dst );
int CompressChunk( uchar* buffer, const uchar* end );
void delta_init();
void delta_kill_monster( int monsterIndex, uchar row, uchar col, uchar dlvl );
void delta_monster_hp( int monsterIndex, int life, uchar dlvl );
void delta_sync_monster( const TSyncMonster* pSync, uchar dlvl );
void delta_sync_summon( const TCmdAwakeSummon* pSync );
void delta_leave_sync( uchar dlvl);
bool delta_portal_inited( int playerIndex );
bool delta_quest_inited( int mpQuestIndex );
void DeltaAddItem(int itemIndex);
void DeltaSaveLevel();
void DeltaLoadLevel();
void NetSendCmd( bool bHiPri, uchar bCmd );
void NetSendCmdAwakeSummon( uint summonIndex, int row, int col, int dlvl, const DSummonStr& params );
void NetSendCmdLoc( int highPriority, char bCmd, short row, short col );
void NetSendCmdLocParam1( int someFlag, char netCmdIndex, short row, short col, ushort firstArg );
void NetSendCmdLocParam2( int someFlag, char netCmdIndex, short row, short col, ushort firstArg, ushort secondArg );
void NetSendCmdLocParam3( int someFlag, char netCmdIndex, short row, short col, ushort firstArg, ushort secondArg, ushort thirdArg );
void NetSendCmdParam1( int someFlag, char netCmdIndex, ushort firstArg );
void NetSendCmdParam2( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg );
void NetSendCmdParam3( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg, ushort thirdArg );
void NetSendCmdQuest( int someFlag, char questIndex );
void NetSendCmdGItem( int someFlag, uchar netCmdIndex, char master, char playerIndex, uchar itemIndex );
void NetSendCmdPItem( int someFlag, char netCmdIndex, short row, short col );
void NetSendCmdChItem( int isHand, char onBodyIndex );
void NetSendCmdDelItem( int isHand, char onBodyIndex );
void NetSendCmdDItem(bool hiPri,int itemIndex);
void NetSendCmdDamage( int someFlag, char targetPlayerIndex, int damage );
void SendCmdDamageMonster( int someFlag, ushort targetMonsterIndex, int damage	);
void SendCmdAcidDotMonster(int someFlag, ushort targetMonsterIndex, int DeltaMonAcidDot, DUNGEON MonsterMainDlvl, int MonAcidDot, char dotLastAttacker);
void SendCmdPvP(int someFlag, char playerIndex, bool IsPlayerEnemy);
void NetSendString( DWORD targetPlayersMask, char* message );
void DeltaClosePortal( int playerIndex );
int ParseCmd( int playerIndex, const TCmd* packet );
int DReceiveChunk( int playerIndex, const TCmdPlrInfoHdr* packet );
void DParseChunk( uchar messageNumber, int bufferPtr );
uchar* DeltaParseItems( uchar* src, TCmdPItem* dst);
uchar* DeltaParseObjects( uchar* src, DObjectStr* dst );
uchar* DeltaParseMonsters( uchar* src, DMonsterStr* dst );
uchar* DeltaParseSummons( uchar* src, DSummonStr* dst );
void DParseJunk( uchar* src );
int On_SYNCDATA( void* packet, int playerIndex );
uint On_WALKXY( const TCmdLoc* packet, int playerIndex );
uint On_ADDSTR( const TCmdParam1* packet, int playerIndex );
uint On_ADDDEX( const TCmdParam1* packet, int playerIndex );
uint On_ADDMAG( const TCmdParam1* packet, int playerIndex );
uint On_ADDVIT( const TCmdParam1* packet, int playerIndex );
uint On_SBSPELL( const TCmdParam1* packet, int playerIndex );
void  msg_errorf( const char* format, ... );
uint On_GETITEM( TCmdGItem* packet, int playerIndex );
uint On_GOTOGETITEM( const TCmdLocParam1* packet, int playerIndex );
uint On_REQUESTGITEM( const TCmdGItem* packet, int playerIndex );
uint On_GOTOAGETITEM( const TCmdLocParam1* packet, int playerIndex );
uint On_REQUESTAGITEM( const TCmdGItem* packet, int playerIndex );
uint On_PUTITEM( const TCmdPItem* packet, int playerIndex );
uint On_AGETITEM( const TCmdGItem* packet, int playerIndex );
uint On_GETITEM( const TCmdGItem* packet, int playerIndex );
uint On_ITEMEXTRA( const TCmdGItem* packet, int playerIndex );
uint On_RESPAWNITEM( const TCmdPItem* packet, int playerIndex );
void check_update_plr( int playerIndex );
uint On_SYNCPUTITEM( const TCmdPItem* packet, int playerIndex );
uint On_ATTACKXY( const TCmdLoc* packet, int playerIndex );
uint On_SATTACKXY( const TCmdLoc* packet, int playerIndex );
uint On_RATTACKXY( const TCmdLoc* packet, int playerIndex );
uint On_SPELLXYD( const TCmdLocParam3* packet, int playerIndex );
uint On_WallRecast(const TCmdWallRecast* packet, int playerIndex);
uint On_TSPELLXYD(const TCmdLocParam3* packet, int playerIndex);
uint On_SPELLXY( const TCmdLocParam2* packet, int playerIndex );
uint On_TSPELLXY( const TCmdLocParam2* packet, int playerIndex );
uint On_OPOBJXY( const TCmdLocParam1* packet, int playerIndex );
uint On_DISARMXY( const TCmdLocParam1* packet, int playerIndex );
uint On_OPOBJT( const TCmdParam1* packet, int playerIndex );
uint On_ATTACKID( const TCmdParam1* packet, int playerIndex );
uint On_ATTACKPID( const TCmdParam1* packet, int playerIndex );
uint On_RATTACKID( const TCmdParam1* packet, int playerIndex );
uint On_RATTACKPID( const TCmdParam1* packet, int playerIndex );
uint On_SPELLID( const TCmdParam3* packet, int playerIndex );
uint On_SPELLPID( const TCmdParam3* packet, int playerIndex );
uint On_TSPELLID( const TCmdParam3* packet, int playerIndex );
uint On_TSPELLPID( const TCmdParam3* packet, int playerIndex );
uint On_KNOCKBACK( const TCmdParam2* packet, int playerIndex );
uint On_RESURRECT( const TCmdParam1* packet, int playerIndex );
uint On_HEALOTHER( const TCmdParam1* packet, int playerIndex );
uint On_TALKXY( const TCmdLocParam1* packet, int playerIndex );
uint On_NEWLVL( const TCmdParam2* packet, int playerIndex );
uint On_WARP( const TCmdParam1* packet, int playerIndex );
uint On_MONSTDEATH(const TCmdLocParam2* packet, int playerIndex);
uint On_KILLGOLEM(const TCmdLocParam2* packet, int playerIndex);
uint On_AWAKEGOLEM( const TCmdAwakeSummon* packet, int playerIndex );
uint On_MONSTDAMAGE( const TCmdDamageMonster* packet, int playerIndex );
uint On_MONSTACIDDOT(const TCmdAcidDotMonster* packet, int playerIndex);
uint On_PVP(const TCmdPvP* packet, int playerIndex);
uint On_PLRDEAD( const TCmdParam1* packet, int playerIndex );
uint On_PLRDAMAGE( const TCmdDamage* packet, int playerIndex );
uint On_OPENDOOR( const TCmdParam1* packet, int playerIndex );
void delta_sync_object( int objectOnMapIndex, char ncCode, int dungeon );
uint On_CLOSEDOOR( const TCmdParam1* packet, int playerIndex );
uint On_OPERATEOBJ( const TCmdParam1* packet, int playerIndex );
uint On_PLROPOBJ( const TCmdParam2* packet, int playerIndex );
uint On_BREAKOBJ( const TCmdParam2* packet, int playerIndex );
uint On_CHANGEPLRITEMS( const TCmdChItem* packet, int playerIndex );
uint On_DELPLRITEMS( const TCmdDelItem* packet, int playerIndex );
uint On_PLRLEVEL( const TCmdParam1* packet, int playerIndex );
uint On_DROPITEM( const TCmdPItem* packet, int playerIndex );
uint On_SEND_PLRINFO( const TCmdPlrInfoHdr* packet, int playerIndex );
uint On_ACK_PLRINFO( const TCmdPlrInfoHdr* packet, int playerIndex );
uint On_PLAYER_JOINLEVEL(const TCmdLocParam1* packet, int playerIndex);
uint On_ACTIVATEPORTAL( const TCmdLocParam3* packet, int playerIndex );
void delta_open_portal( int playerIndex, short row, short col, DUNGEON dungeon );
uint On_DEACTIVATEPORTAL( const TCmd* packet, int playerIndex );
uint On_RETOWN( const TCmd* packet, int playerIndex );
uint On_SETSTR( const TCmdParam1* packet, int playerIndex );
uint On_SETDEX( const TCmdParam1* packet, int playerIndex );
uint On_SETMAG( const TCmdParam1* packet, int playerIndex );
uint On_SETVIT( const TCmdParam1* packet, int playerIndex );
uint On_STRING( const TCmdString* packet, int playerIndex );
uint On_SYNCQUEST( const TCmdQuest* packet, int playerIndex );
uint On_ENDSHIELD( const TCmd* packet, int playerIndex );
uint On_DESTROY_REFLECT( const TCmd* packet, int playerIndex );
uint On_DEBUG( const TCmd* packet, int playerIndex );
uint On_OPEN_WEAK_UBER( const TCmd* packet, int playerIndex );
uint On_SETPERK( const TCmdParam2* packet, int playerIndex );
bool i_own_level( int dungeonLevel );
void NetSendCmdGItem2( int usOnly, char netCmdIndex, char master, char playerIndex, const TCmdGItem* packet );
bool NetSendCmdReq2(BYTE bCmd,BYTE mast,BYTE pnum,const TCmdGItem * p);
