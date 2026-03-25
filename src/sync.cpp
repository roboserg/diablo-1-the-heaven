#include "stdafx.h"

#pragma pack(push, 1)

struct TSyncHeader
{
	uchar	netCmdIndex;
	DUNGEON	dungeon;
	uchar	dungeon_reserved;
	WORD	wLen;
	uchar	objectIndex;
	uchar	objectCmd;

	// ItemDrop
	uchar   itemIndex;
	uchar   itemIndex_reserved;
	short   mapRow;
	short   mapCol;
	LastItemInfo dropInfo;

	// ItemEquip
	uchar   onBodyIndex;
	LastItemInfo equipInfo;
	
	#pragma warning(disable : 4200)
	TSyncMonster monsters[];
};

//static_assert( sizeof( TSyncHeader ) == 46, "Wrong TSyncHeader size" );

#pragma pack(pop)

static ushort monsterSyncPriorityMask[Monsters_Max_Count];
static int sgnMonsters;
static ushort sgwLRU[Monsters_Max_Count];
static int sgnSyncItem;
static int sgnSyncPInv = 0;

void sync_one_monster();
bool sync_monster_active( TSyncMonster* activation );
void sync_monster_pos( TSyncMonster* activation, int monsterIndex );
bool sync_monster_active2( TSyncMonster* activation );
void SyncPlrInv( TSyncHeader* header );
void sync_monster( uint playerIndex, const TSyncMonster* packet );

//----- (00471419) --------------------------------------------------------
DWORD sync_packData( const uchar* pbBuf, DWORD maxSize )
{
    if( MonstersCount < 1 ){
        return maxSize;
    }
    if (maxSize < sizeof(TSyncHeader) + sizeof(TSyncMonster)) {
		return maxSize;
	}
	if( Players[CurrentPlayerIndex].lvlChanging ){
		return maxSize;
	}

	TSyncHeader* hdr = (TSyncHeader *)pbBuf;
    maxSize -= sizeof(TSyncHeader);
    hdr->netCmdIndex = CMD_35_SYNCDATA;
    TSyncMonster* monsterSync = hdr->monsters;
    hdr->dungeon = Dungeon;
    hdr->wLen = 0;
    SyncPlrInv( hdr );
    sync_one_monster();
    
    for (int monsterIndex = 0; monsterIndex < MonstersCount && maxSize >= sizeof(TSyncMonster); ++monsterIndex ) {
        int sync = 0;
        if( monsterIndex < 2 ){
            sync = sync_monster_active2( monsterSync );
        }
        if( !sync ){
            sync = sync_monster_active( monsterSync );
        }
        if (!sync) {
            break;
        }
        hdr->wLen += sizeof(TSyncMonster);
        maxSize -= sizeof(TSyncMonster);
        ++monsterSync;
    }
	return maxSize;
}

//----- (004714A0) --------------------------------------------------------
void sync_one_monster()
{
    int playerRow = Players[CurrentPlayerIndex].Row;
    int playerCol = Players[CurrentPlayerIndex].Col;
    
    for( int i = 0; i < MonstersCount; ++i )
    {
        int m = MonsterIndexes[i];
        Monster& monster = Monsters[m];
        // less distance to current player - more prioryty for sync
        monsterSyncPriorityMask[m] = abs( playerCol - monster.Col ) + abs( playerRow - monster.Row );
        if( monster.ActivationCounter == 0 )
        {
            monsterSyncPriorityMask[m] += 0x1000; // inactive monsters have less priority
        }
        else if( sgwLRU[m] != 0 )
        {
            --sgwLRU[m];
        }
    }
}

//----- (00471567) --------------------------------------------------------
bool sync_monster_active( TSyncMonster* activation)
{
	DWORD lru = -1;
	int monsterIndex = -1;
	
	for ( int i = 0; i < MonstersCount; ++i ) {
	    int m = MonsterIndexes[i];
	    if( monsterSyncPriorityMask[ m ] < lru && sgwLRU[ m ] < 0xFFFE ){
            lru = monsterSyncPriorityMask[ m ];
            monsterIndex = m;
        }
    }
	
	if( monsterIndex == -1 ){
		return false;
	} else{
        sync_monster_pos( activation, monsterIndex );
		return true;
	}
}

//----- (004715C3) --------------------------------------------------------
void sync_monster_pos( TSyncMonster* activation, int monsterIndex )
{
    const Monster monster = Monsters[monsterIndex];
	activation->monsterIndex = monsterIndex;
	activation->row = monster.Row;
	activation->col = monster.Col;
	activation->targetIndex = encode_enemy( monsterIndex );
	unsigned __int16 activationTimer = monsterSyncPriorityMask[ monsterIndex ];
	// it looks as bug. If we call it for inactive monster, it becomes active
	activation->activationTimer = activationTimer > 0xFFu ? 0xFFu : activationTimer;
	monsterSyncPriorityMask[ monsterIndex ] = 0xFFFF;
	sgwLRU[monsterIndex] = monster.ActivationCounter == 0 ? 0xFFFF : 0xFFFE;
}

//----- (00471632) --------------------------------------------------------
bool sync_monster_active2( TSyncMonster* activation )
{
    int monsterIndex = -1;
    DWORD lru = 0xFFFE;
    
    for( int i = 0; i < MonstersCount; ++i )
    {
        if( sgnMonsters >= MonstersCount )
        {
            sgnMonsters = 0;
        }
        int m = MonsterIndexes[sgnMonsters];
        if( sgwLRU[m] < lru )
        {
            lru = sgwLRU[m];
            monsterIndex = MonsterIndexes[sgnMonsters];
        }
        ++sgnMonsters;
    }
    
    if( monsterIndex == -1 )
    {
        return false;
    }
    else
    {
        sync_monster_pos( activation, monsterIndex );
        return true;
    }
}

//----- (00471692) --------------------------------------------------------
void SyncPlrInv( TSyncHeader* header )
{
	if( ItemsCount <= 0 ){
		header->itemIndex = -1;
	}else{
		if( sgnSyncItem >= ItemsCount ){
			sgnSyncItem = 0;
		}
		++sgnSyncItem;
		int ii = ItemActive[sgnSyncItem-1];
		const Item& item = Items[ii];
		
		header->itemIndex = ii;
		header->mapRow = item.MapRow;
		header->mapCol = item.MapCol;
		header->dropInfo = item;
	}

	{
        const Item& bodyItem = Players[ CurrentPlayerIndex ].OnBodySlots[ sgnSyncPInv ];
        if( bodyItem.ItemCode == IC_M1_NONE ){
            header->onBodyIndex = IC_M1_NONE;
        } else{
            header->onBodyIndex = sgnSyncPInv;
			header->equipInfo = bodyItem;
        }
        ++sgnSyncPInv;
        if( sgnSyncPInv >= IS_Inventory ){
            sgnSyncPInv = 0;
        }
    }
}

DWORD sync_applyData( int playerIndex, const uchar* pbBuf )
{
    auto pHdr = (TSyncHeader *)pbBuf;
    pbBuf += sizeof(*pHdr);
    
	if( pHdr->netCmdIndex != CMD_35_SYNCDATA ){
		TerminateWithError( "bad sync command" );
	}
 
	if( gbBufferMsgs != BUFFER_ON && playerIndex != CurrentPlayerIndex ){
	    for( int wLen = pHdr->wLen; wLen >= sizeof(TSyncMonster); wLen -= sizeof(TSyncMonster) ){
			// Only sync monsters if players are on the same map.
			// Without the last condition, monsters in quest level can disappear/out of map then reappear,
			// as another player interacts with monsters in the parent dungeon/non-quest level.
			if( Dungeon == pHdr->dungeon && ! Players[CurrentPlayerIndex].lvlChanging ){
                sync_monster( playerIndex, (TSyncMonster*)pbBuf );
            }
			delta_sync_monster((TSyncMonster*)pbBuf, pHdr->dungeon);
            pbBuf += sizeof( TSyncMonster );
        }
	}
	return pHdr->wLen + sizeof(*pHdr);
}

//----- (004718F3) --------------------------------------------------------
void sync_monster( uint playerIndex, const TSyncMonster* packet)
{
	// needs stuct NetMonsterData
	int monsterIndex = packet->monsterIndex;
	int monsterRow	 = packet->row;
	int monsterCol	 = packet->col;
	int monsterTarget= packet->targetIndex;
	int summOfDelta2 = packet->activationTimer;

	Monster& monster = Monsters[monsterIndex];

	if( monster.CurrentLife <= 0 ){
		return;
	}

	// Does nothing
//	for( int i = 0; i < MonstersCount; ++i )
//	{
//		if( MonsterIndexes[i] == monsterIndex )
//		{
//			break;
//		}
//	}

	int deltaRow = abs(Players[CurrentPlayerIndex].Row - monster.Row);
	int deltaCol = abs(Players[CurrentPlayerIndex].Col - monster.Col);
	int summOfDelta = deltaCol + deltaRow;
	if( summOfDelta > 255 ){
		summOfDelta = 255;
	}

	if( summOfDelta < summOfDelta2 ){
		return;
	}
	if( summOfDelta == summOfDelta2 && playerIndex > CurrentPlayerIndex ){
		return;
	}
	if( monster.NextRow == monsterRow && monster.NextCol == monsterCol ){
		return;
	}
	int curAction = monster.CurAction;
	if (curAction == A_14_CHARGE || curAction == A_15_STONE || (curAction == A_11_SPELL_STAND && monsterIndex < SummonMonsters_Count)) {//Do NOT sync position if a newly summoned player monster is playing spawning animation, so that it will spawn in the correct position.
		return;
	}
	deltaRow = abs(monster.Row - monsterRow);
	deltaCol = abs(monster.Col - monsterCol);
	if(deltaRow <= 2 && deltaCol <= 2 && curAction != A_6_DEATH) {
		if( !( curAction >= A_1_WALK_UP && curAction <= A_3_WALK_HORIZ ) ){
			int orientation = OrientationToTarget( monster.Row, monster.Col, monsterRow, monsterCol );
			if (MayMoveMonsterToDirection(monsterIndex, orientation)) {
				ClearMonsterOnMap( monsterIndex );
				MonsterMap[monster.Row][monster.Col] = monsterIndex + 1;
				StartMonsterWalk( monsterIndex, orientation );//This sets monster animation to ANIM_1_WALK, can cause dying monster to die again and produce duped items drop
				monster.ActivationCounter = 255;
			}
		}
	}
	else if( !MonsterMap[monsterRow][monsterCol] ) {//This checks for != 0
		ClearMonsterOnMap( monsterIndex );
		MonsterMap[monsterRow][monsterCol] = monsterIndex + 1;
		monster.Row = monsterRow;
		monster.Col = monsterCol;
		if (curAction != A_6_DEATH) {
			decode_enemy(monsterIndex, monsterTarget);
			int orientation = OrientationToTarget(monsterRow, monsterCol, monster.TargetRow, monster.TargetCol);
			FixMonsterPosition(monsterIndex, orientation);//This sets monster animation to ANIM_0_STAY, can cause dying monster to die again and produce duped items drop
			monster.ActivationCounter = 255;
		}
	}

	decode_enemy( monsterIndex, monsterTarget );
}

//----- (00471B0E) --------------------------------------------------------
void sync_init()
{
	sgnMonsters = 16 * CurrentPlayerIndex;
	memset(sgwLRU, 0xFF, sizeof( sgwLRU ));
}
