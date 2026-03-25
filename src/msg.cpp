#include "stdafx.h"

i64 sgdwOwnerWait;
int sgdwRecvOffset;
int sgnCurrMegaPlayer;
char sbLastCmd;
TMegaPkt* sgpCurrPkt;

// buffer to receive level information
uchar sgbRecvLevel[SEND_LEVEL_SIZE];
uchar sgbRecvCmd;

DJunk sgJunk;
DLevel sgLevels[DungeonCount]; // sent to other players
LocalLevel sgLocals[DungeonCount]; // not sent

TMegaPkt* sgpMegaPkt;
bool sgbDeltaChanged;
uchar sgbDeltaChunks;

bool deltaload = false;
uchar gbBufferMsgs = BUFFER_OFF;
int pkt_counter = 0;
bool NetItem = false; // sharing items in multiplayer flag, TODO: if not share use separate rng for item gen (desync issue)


//----- (th3) -------------------------------------------------------------
bool ItemInfo64::operator==(const ItemInfo64& other) const
{
	return genVersion == other.genVersion
	    && dropType == other.dropType
	    && seed == other.seed
	    && quality == other.quality
	    && magicFind == other.magicFind
	    && baseItemIndex == other.baseItemIndex
	    && uniqIndex == other.uniqIndex;
}

//----- (th3) -------------------------------------------------------------
bool ItemInfo64::operator==(const Item& item) const
{
	return genVersion == item.genVersion
		&& dropType == item.dropType
		&& seed == item.seed
		&& quality == item.quality
		&& magicFind == item.magicFindSeed
		&& baseItemIndex == item.baseItemIndex
		&& uniqIndex == item.uniqIndexSeed;
}

//----- (th3) -------------------------------------------------------------
bool ItemInfo64::operator==(const ItemGet& itemGet) const
{
	return genVersion == itemGet.genVersion
		&& dropType == itemGet.dropType
		&& seed == itemGet.seed
		&& quality == itemGet.quality
		&& magicFind == itemGet.magicFind
		&& baseItemIndex == itemGet.baseItemIndex
		&& uniqIndex == itemGet.uniqIndex;
}

//----- (th3) -------------------------------------------------------------
void ItemGet::operator=(const ItemInfo64& info)
{
	genVersion = info.genVersion;
	saveVersion = info.saveVersion;
	dropType = info.dropType;
	seed = info.seed;
	quality = info.quality;
	magicFind = info.magicFind;
	baseItemIndex = info.baseItemIndex;
	uniqIndex = info.uniqIndex;
}

//----- (th3) -------------------------------------------------------------
void ItemInfo64::operator=(const Item& item)
{
	genVersion              = item.genVersion;
	saveVersion             = item.saveVersion;
	dropType                = item.dropType;
	seed                    = item.seed;
	quality                 = item.quality;
	magicFind               = item.magicFindSeed;
	baseItemIndex           = item.baseItemIndex;
	uniqIndex               = item.uniqIndexSeed;
	magicLevelAndIdentified = (item.MagicLevel << 1) | (item.Identified & 1);
	amount                  = item.amount; // originally apply only for gold
	curDurability           = item.CurDurability;
	baseDurability          = item.BaseDurability;
	curCharges              = item.CurCharges;
	baseCharges             = item.BaseCharges;
	socketsAdded            = item.socketsAdded;
	sockets                 = item.sockets;
	enchantIndexes          = item.enchantIndexes;
	quenchAffix             = item.quenchAffix;
	quenchLevel             = item.quenchLevel;
	difficulty              = item.difficulty;
	charFlag                = item.charFlag;
}

#if 0 // unused, see LoadItem
//----- (th3) -------------------------------------------------------------
void Item::operator=(const ItemInfo64& info)
{
	genVersion     = info.genVersion;
	dropType       = info.dropType;
	seed           = (int)info.seed;
	quality        = info.quality;
	magicFindSeed  = info.magicFind;
	baseItemIndex  = info.baseItemIndex;
	uniqIndexSeed  = info.uniqIndex;
	Identified     = info.magicLevelAndIdentified & 1;
	MagicLevel     = MAGIC_LEVEL(info.magicLevelAndIdentified >> 1);
	amount         = info.amount;
	CurDurability  = info.curDurability;
	BaseDurability = info.baseDurability;
	CurCharges     = info.curCharges;
	BaseCharges    = info.baseCharges;
	socketsAmount  = info.socketsAmount;
	sockets        = info.sockets;
	enchantIndexes = info.enchantIndexes;
	quenchAffix = info.quenchAffix;
	quenchLevel = info.quenchLevel;
}
#endif

//----- (00445FFF) --------------------------------------------------------
void msg_send_drop_pkt( int playerIndex, int playerEvent )
{
    TFakeDropPlr cmd{};
    cmd.bCmd = CMD_104_FAKE_DROPID;
    cmd.bPlr = playerIndex;
    cmd.dwReason = playerEvent;
    mega_add_data( playerIndex, &cmd, sizeof(cmd) );
}

//----- (0044601A) --------------------------------------------------------
void mega_add_data( int playerIndex, const void* packet, uint size)
{
	if( playerIndex != sgnCurrMegaPlayer ){
		// add a command to set the current player
		sgnCurrMegaPlayer = playerIndex;
		TFakeCmdPlr cmd{};
		cmd.bCmd = CMD_103_FAKE_SETID;
		cmd.bPlr = playerIndex;
        mega_add_data( playerIndex, &cmd, sizeof(cmd) );
	}
	
	// if there isn't enough space in the current megapkt, add another
	auto packeta = sgpCurrPkt; // TODO: check original
	if( sgpCurrPkt->dwSpaceLeft < size ){
        MegaAddPacket();
		packeta = sgpCurrPkt;
	}
	
	// add data to megapacket
	memcpy((char *)&packeta[1] - packeta->dwSpaceLeft, packet, size ); // TODO: check original
	sgpCurrPkt->dwSpaceLeft -= size;
}

//----- (0044607C) --------------------------------------------------------
void MegaAddPacket()
{
	// TODO: check original
	sgpCurrPkt = (TMegaPkt*)AllocMem( sizeof(TMegaPkt) );
	sgpCurrPkt->pNext = nullptr;
	sgpCurrPkt->dwSpaceLeft = sizeof(sgpCurrPkt->data);
	
	auto it = (TMegaPkt *)&sgpMegaPkt;
	while( it->pNext ) {
		it = it->pNext;
	}
	it->pNext = sgpCurrPkt;
}

//----- (004460B8) --------------------------------------------------------
bool WaitDeltaInfo()
{
    MegaAddPacket();
	
	sgbRecvCmd = CMD_84_DLEVEL_END;
	sgbDeltaChunks = 0;
	gbBufferMsgs = BUFFER_ON;
	sgnCurrMegaPlayer = -1;
	sgdwOwnerWait = GetTickCountTh();

	bool success = UiProgressDialog( ghMainWnd, "Waiting for game data...", 1 /*allow abort*/, DeltaProgress, ENGINE_FPS /*calls per second*/ );

	// done buffering messages, but don't go into BUFFER_PROCESS
	// mode until we've successfully loaded the level
	gbBufferMsgs = BUFFER_OFF;

	if( !success ){
		// no message -- user canceled
		MegaFreePackets();
		return false;
	}
	else if( gbGameDestroyed ){
		ErrorBoxf( "The game ended" );
        MegaFreePackets();
		return false;
	}
	else if( sgbDeltaChunks != DELTA_CHUNKS ){
		ErrorBoxf( "Unable to get level data" );
		MegaFreePackets();
		return false;
	}

	// leave the delta information around
	// until the game level has been loaded
	// and then run all the accumulated packets
	return true;
}

//----- (00446138) --------------------------------------------------------
void MegaFreePackets()
{
    while (sgpMegaPkt) {
		sgpCurrPkt = sgpMegaPkt->pNext;
		FreeMem(sgpMegaPkt);
		sgpMegaPkt = sgpCurrPkt;
	}
	// NOTE: both sgpMegaPkt & sgpCurrPkt are NULL
}

//----- (00446164) --------------------------------------------------------
int DeltaProgress()
{
	if( sgbDeltaChunks == 0 ){
		// wait until we receive master player's future
		// turn before we process our first turn.  This is
		// designed so that we don't start several turns ahead
		// of the master player and stay there forever.  We are
		// waiting for turn X from all players.  We wait for 
		// turn (X+gdwTurnsInTransit) from the master player, which
		// means that the master player processed turn X and has
		// sent turns X+1 and X+2.  When we process turn X, we will be
		// synced with the master player less the one-way packet 
		// latency from the master player.
		NThreadFillSyncQueue( 0, 0 );
        uint turns;
		if( !SNetGetOwnerTurnsWaiting(&turns) && GetLastError() == STORM_ERROR_NOT_IN_GAME ){
			return 100;
		}

		// wait a maximum of 2 seconds before we give
		// up syncing and just try for whatever we can get
		// if a turn got dropped by the owner, it is possible
		// we could wait forever for the owner, while he waits
		// forever for us to resend the dropped turn.
		if( GetTickCountTh() - sgdwOwnerWait <= 2000_ms && turns < gdwTurnsInTransit ){
			return 0;
		}
		++sgbDeltaChunks;
	}

	// handle asynchronous messages
	NetReceivePackets();

	// handle synchronous messages
    int sendAsync;
    NThreadFillSyncQueue( 0, 0 );
	// if we call NThreadMsgCheck() without
	// protection against "overcalling" then the gameclock will get
	// continually reset and other systems will drop into timeout mode
	// NThreadMsgCheck(&sendAsync);
	if( NThreadRunGameLoop(false) ){
        NThreadMsgCheck( &sendAsync );
	}

	// if the game was destroyed, return "100% complete" to exit dialog
	if( gbGameDestroyed ){
		return 100;
	}

	// if the person sending us delta info dropped out,
	// then we need to re-request the delta information
	if( gbDeltaSender >= PlayersMax_4 ){
		sgbRecvCmd = CMD_84_DLEVEL_END;
		sgbDeltaChunks = 0;
		gbDeltaSender = CurrentPlayerIndex;
		NThreadSetDeltaRequest();
	}

	// make bar "full"
	if( sgbDeltaChunks == DELTA_CHUNKS - 1 ){
		sgbDeltaChunks = DELTA_CHUNKS;
		return 99;
	}

	// return completion status
	return 100 * sgbDeltaChunks / DELTA_CHUNKS;
}

//----- (00446234) --------------------------------------------------------
void RunDeltaInfo()
{
	if( MaxCountOfPlayersInGame != 1 ){

		gbBufferMsgs = BUFFER_PROCESS;
        MegaRunPackets();
		gbBufferMsgs = BUFFER_OFF;

		MegaFreePackets();
	}
}

//----- (00446256) --------------------------------------------------------
void MegaRunPackets()
{
	TMegaPkt* mega = sgpMegaPkt;
	for( int playerIndex = -1; mega; mega = mega->pNext ){
		uchar* data = mega->data;
		int size = sizeof(mega->data);
		while( size != mega->dwSpaceLeft ){
			if( *data == CMD_103_FAKE_SETID ){
				TFakeCmdPlr* setIdCmd = (TFakeCmdPlr*) data;
			    data += sizeof(*setIdCmd);
				size -= sizeof(*setIdCmd);
				playerIndex = setIdCmd->bPlr;
			}
			else if( *data == CMD_104_FAKE_DROPID ){
				TFakeDropPlr* dropCmd = (TFakeDropPlr*) data;
				data += sizeof(*dropCmd);
				size -= sizeof(*dropCmd);
                UnbufferRemovePlayer( dropCmd->bPlr, dropCmd->dwReason );
			}
			else{
				TCmd* cmd = (TCmd*) data;
				int parsedSize = ParseCmd( playerIndex, cmd );
				data += parsedSize;
				size -= parsedSize;
			}
		}
	}
}

//----- (004462BD) --------------------------------------------------------
void DeltaSendAllLevels( int playerIndex) // dx: DeltaExportData
{
	// send all delta information to playerIndex
	if( sgbDeltaChanged ){
		uchar* base = (uchar*) AllocMem( SEND_LEVEL_SIZE );
		for( int dungeon = 0; dungeon < DungeonCount; ++dungeon ){
			
			// save 1 byte for compress/uncompress flag
		    uchar* data = base + 1;
			
			// build level data
			DLevel& netLevelData = sgLevels[dungeon];
			if( NetItem ) data = DeltaBuildItems( data, netLevelData.items );
			data = DeltaBuildObjects( data, netLevelData.objects );
			data = DeltaBuildMonsters( data, netLevelData.monsters );
			data = DeltaBuildSummons( data, netLevelData.summons );
			int size = CompressChunk( base, data );
			
			// NetCMD enum must match total number of dungeons, mp quest area included
			DThreadSendPLayerInfoChunk( playerIndex, dungeon | CMD_DLEVEL /*+ CMD_58_DLEVEL_0*/, base, size );
		}

		// send junk chunk -- save one byte for compress/uncompress flag
		uchar* data = DeltaExportJunk( base + 1 );
		int size = CompressChunk( base, data );
        DThreadSendPLayerInfoChunk( playerIndex, CMD_83_DLEVEL_JUNK, base, size );
		FreeMem(base);
	}
	
	// send 1 byte terminator chunk
	uchar chunk = 0; // mark as "uncompressed"
    DThreadSendPLayerInfoChunk( playerIndex, CMD_84_DLEVEL_END, &chunk, 1 );
}

//----- (00446374) --------------------------------------------------------
uchar* DeltaBuildItems( uchar* dst, TCmdPItem* src )
{
	for( int i = 0; i < MaxItems_255; i++, src++ ){
		if( src->bCmd == INIT_VAL ){
			*dst++ = INIT_VAL;
		}else{
			memcpy(dst, src, sizeof( *src ));
			dst += sizeof( *src );
		}
	}
	return dst;
}

//----- (0044639F) --------------------------------------------------------
uchar* DeltaBuildObjects( uchar* dst, DObjectStr* src )
{
    size_t size = sizeof(*src) * MaxObjects_127;
	memcpy(dst, src, size);
	return dst + size;
}

//----- (004463B3) --------------------------------------------------------
uchar* DeltaBuildMonsters( uchar* dst, DMonsterStr* src )
{
    for( int i = 0; i < Monsters_Max_Count; ++i, ++src ){
		if( *(uchar *)src == INIT_VAL ){
			*dst++ = INIT_VAL;
		}else{
			memcpy(dst, src, sizeof( *src ));
			dst += sizeof( *src );
		}
	}
	return dst;
}

uchar* DeltaBuildSummons( uchar* dst, DSummonStr* src )
{
    for( int i = 0; i < SummonMonsters_Count; ++i, ++src ){
		if( *(uchar *)src == INIT_VAL ){
			*dst = INIT_VAL;
			++dst;
		}else{
			memcpy(dst, src, sizeof( *src ));
			dst += sizeof( *src );
		}
	}
	return dst;
}

//----- (004463DA) --------------------------------------------------------
uchar* DeltaExportJunk( uchar* dst )
{
	for( auto& portal : sgJunk.portals ){
		const DPortal* pD = &portal;
		if( pD->row == INIT_VAL ){
			*dst = INIT_VAL;
			dst++;
		}else{
			memcpy(dst, pD, sizeof( *pD ));
			dst += sizeof( *pD );
		}
	}

	//This original code below indexes sgJunk.quests[] differently from Quests[], which causes wrong data to be exported to other newly joined players!
	#ifdef BrokenCode
	MultiQuests* mq = sgJunk.quests;
	for( int i = 0; i < Quest_Count; ++i ){
        if( BaseQuests[i].flag & QF_MULTI ){
            Quest& quest = Quests[i];
            mq->status = quest.status;
            mq->talkToFlag = quest.talkToFlag;
            mq->status2 = quest.status2;
            
            memcpy( dst, mq, sizeof( *mq ) );
            dst += sizeof( *mq );
            ++mq;
        }
    }
	#endif

	int q = 0;
	for (auto& quest : Quests) {
		if( BaseQuests[q].flag & QF_MULTI ){
			sgJunk.quests[q].status = quest.status;
			sgJunk.quests[q].talkToFlag = quest.talkToFlag;
			sgJunk.quests[q].status2 = quest.status2;
			sgJunk.quests[q].status3 = quest.status3;//I hope this won't screw up Izual Red TP
			sgJunk.quests[q].status4 = quest.status4;
			sgJunk.quests[q].status5 = quest.status5;
		}
		memcpy(dst, &sgJunk.quests[q], sizeof(MultiQuests)); // Now sgJunk.quests fields are correctly passed.
		dst += sizeof(MultiQuests);//However, I'm not too sure about this pointer arithmetic here...
		q++;
	}

	return dst;
}

//----- (00446446) --------------------------------------------------------
int CompressChunk( uchar* buffer, const uchar* end ) // dx: CompressData
{
	// calculate length excluding leading byte
	int size = end - buffer - 1;
	// compress data excluding leading byte
	int pkSize = Compress(buffer + 1, size); // here
	// set compress flag ==> TRUE = compressed
	*buffer = size != pkSize;
	// add one extra byte for compress flag
	return pkSize + 1;
}

//----- (00446464) --------------------------------------------------------
void delta_init()
{
	sgbDeltaChanged = false;
	memset(&sgJunk, INIT_VAL, sizeof( sgJunk ));
	memset(sgLevels, INIT_VAL, sizeof( sgLevels ));
	memzero(sgLocals);
	deltaload = false;
}

//----- (004464B1) --------------------------------------------------------
void delta_kill_monster( int monsterIndex, uchar row, uchar col, uchar dungeon )
{
	if( MaxCountOfPlayersInGame != 1 ){
		sgbDeltaChanged = true;
		DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[monsterIndex];
		Monster& monster = Monsters[monsterIndex];
		monsterInfo.row = row;
		monsterInfo.col = col;
		monsterInfo.orientation = monster.Orientation;
		monsterInfo.life = 0;
		monster.acidDot = 0;//Fix for acid-related invincible monsters in MP, e.g. Hydramancer
	}
}

//----- (004464FC) --------------------------------------------------------
void delta_monster_hp( int monsterIndex, int life, uchar dungeon )
{
	if( MaxCountOfPlayersInGame != 1 ){
		sgbDeltaChanged = true;
		DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[monsterIndex];
		if( monsterInfo.life > life ){
			monsterInfo.life = life;
		}
	}
}

//----- (00446530) --------------------------------------------------------
void delta_sync_monster( const TSyncMonster* pSync, uchar dungeon )
{
	if( MaxCountOfPlayersInGame != 1 ){
		DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[pSync->monsterIndex];
		sgbDeltaChanged = true;
		if( monsterInfo.life > 0 ){
			monsterInfo.row = pSync->row;
			monsterInfo.col = pSync->col;
			monsterInfo.target = pSync->targetIndex;
			monsterInfo.activationCounter = 255;
		}
	}
}

//----- (00446576) --------------------------------------------------------
void delta_sync_summon( const TCmdAwakeSummon* pSync )
{
	if( MaxCountOfPlayersInGame != 1 ){
		sgbDeltaChanged = true;
		uchar dungeon = pSync->dungeon;
		DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[pSync->summonIndex];
		monsterInfo.row = pSync->row;
		monsterInfo.col = pSync->col;
		monsterInfo.orientation = 0;
		monsterInfo.activationCounter = 255;
		// DD's fix. Player summons will no longer die from taking any hit after another player joins Dlvl.
		// The first Golem with monsterIndex = 0 can now be seen by players joined the game after summoning is done.
		monsterInfo.life = pSync->params.summonLife; // was monsterInfo.life = pSync->summonIndex;
		sgLevels[dungeon].summons[pSync->summonIndex] = pSync->params;
	}
}

//----- (th2) -------------------------------------------------------------
void delta_leave_sync( uchar dungeon )
{
    if( !dungeon ){
		DungeonRandSeed[DUN_0_TOWN] = GetRndSeed();
	}else{
		for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
			int monsterIndex = MonsterIndexes[monsterIndexIndex];
			Monster& monster = Monsters[monsterIndex];
			if (monster.CurrentLife <= 0) {//Bugged invincible monsters with HP < 0 will be erased from existence as player leaves Dlvl, and no longer be kept alive as player comes back.
				continue;
			}
			sgbDeltaChanged = true;
			DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[monsterIndex];
			monsterInfo.row = (uchar)monster.Row;
			monsterInfo.col = (uchar)monster.Col;
			monsterInfo.orientation = (uchar)monster.Orientation;
			monsterInfo.target = encode_enemy( monsterIndex );
			monsterInfo.activationCounter = monster.ActivationCounter;
			monsterInfo.life = monster.CurrentLife;
		}

		memcpy( sgLocals[dungeon].automapsv, AutoMap, sizeof( AutoMap ) );
	}
	
	// Store all items info
    sgLocals[dungeon].countItemsOnMap = ItemsCount;
    for( int i = 0; i < MaxItems_255; i++ ){
        sgLocals[dungeon].mapItemsFreeIndexes[i] = ItemActive[i];
    }
    for( int i = 0; i < MaxItems_255; i++ ){
        sgLocals[dungeon].itemsOnMapIndexesInvers[i] = ItemAvailable[i];
    }
    for( int i = 0; i < MaxItems_255; i++ ){
        memcpy( &sgLocals[dungeon].items[i], &Items[i], sizeof(Items[i]) );
    }
    
    memcpy( sgLocals[dungeon].itemsOnGroundMap, ItemMap, sizeof( ItemMap ) );
    
    sgLocals[dungeon].inited = true;
}

//----- (00449A54) --------------------------------------------------------
void delta_sync_object( int objectOnMapIndex, char ncCode, int dungeon )
{
	if( MaxCountOfPlayersInGame != 1 ){
		sgbDeltaChanged = true;
		sgLevels[dungeon].objects[objectOnMapIndex].bCmd = ncCode; // TODO: Must pass the correct dlvl here, or opened chests in MP quest area will respawn upon re-entry
	}
}

//----- (00448337) --------------------------------------------------------
int delta_get_item( const TCmdGItem* packet, uchar dungeon )
{
	if( MaxCountOfPlayersInGame == 1 ){
		return 1;
	}
	DLevel& dlevel = sgLevels[dungeon];
	for( int itemInfoIndex = 0; itemInfoIndex < MaxItems_255; itemInfoIndex++ ){
		TCmdPItem& item = dlevel.items[itemInfoIndex];
		if( item.bCmd != ITEM_FREE && item.info == packet->info ){
			if( item.bCmd != ITEM_GEN_TAKEN ){
				if( item.bCmd != ITEM_GEN_FLOOR ){
					if( item.bCmd != ITEM_DROP_FLOOR ){
						TerminateWithError("delta:1");
					}
					sgbDeltaChanged = 1;
					item.bCmd = ITEM_FREE; // TODO: continue here, check for CMD_LEVEL = 127 potential intersection
				}else{
					sgbDeltaChanged = 1;
					item.bCmd = ITEM_GEN_TAKEN;
				}
			}
			return 1;
		}
	}
	if( ! (packet->info.dropType & D_PREGEN) ){
		return 0;
	}
	for( int itemInfoIndex = 0; itemInfoIndex < MaxItems_255; itemInfoIndex++ ){
		TCmdPItem& item = dlevel.items[itemInfoIndex];
		if( item.bCmd == ITEM_FREE ){ 
			sgbDeltaChanged = 1;
			item.bCmd = ITEM_GEN_TAKEN;
			item.mapRow = packet->mapRow;
			item.mapCol = packet->mapCol;
			item.info = packet->info;
			return 1;
		}
	}
	return 1;
}

//----- (00448850) --------------------------------------------------------
void delta_put_item(const TCmdPItem* packet, int row, int col, char dungeon)
{
	if( MaxCountOfPlayersInGame == 1 ) return;

	// see if the item was already part of the generated dungeon
	TCmdPItem* item = &sgLevels[dungeon].items[0];
	for (int i = 0; i < MaxItems_255; i++, item++ ){
		if( item->bCmd == ITEM_GEN_TAKEN) continue;
		if( item->bCmd == ITEM_FREE) continue;
		if( ! ( item->info == packet->info ) ) continue;
		
		if( item->bCmd == ITEM_DROP_FLOOR ){ // Already placed?
			return;
		}
		TerminateWithError("Trying to drop a floor item?");
	}

	// find a location to drop this item
	item = &sgLevels[dungeon].items[0];
	for( int i = 0; i < MaxItems_255; i++,item++ ){
		if (item->bCmd != ITEM_FREE) continue;

		// put the item back onto the floor at its 
		// new location and with its new state
		sgbDeltaChanged = TRUE;
		*item = *packet;	// struct copy.
		item->bCmd = ITEM_DROP_FLOOR;
		item->mapRow = row;
		item->mapCol = col;
		return;
	}
}

//----- (0044669D) --------------------------------------------------------
bool delta_portal_inited( int playerIndex )
{
	DPortal& portalInfo = sgJunk.portals[playerIndex];
	return portalInfo.row == INIT_VAL;
}

//----- (004466AB) --------------------------------------------------------
bool delta_quest_inited( int mpQuestIndex )
{
	MultiQuests& questInfo = sgJunk.quests[mpQuestIndex];
	return questInfo.status != INIT_VAL;
}

//----- (004466B9) --------------------------------------------------------
void DeltaAddItem(int itemIndex)
{
	if( MaxCountOfPlayersInGame == 1) return;

	// Already in delta?
	TCmdPItem* item = &sgLevels[(DUNGEON)Dungeon].items[0];
	for (int i = 0; i < MaxItems_255; i++, item++) {
		if (item->bCmd == ITEM_FREE) continue;
		if( ! (item->info == Items[itemIndex]) ) continue;

		// taken?
		if (item->bCmd == ITEM_GEN_TAKEN) return;
		// 2nd time down?
		if (item->bCmd == ITEM_GEN_FLOOR) return;
	}


	// find a location to drop this item
	item = &sgLevels[(DUNGEON)Dungeon].items[0];
	for( int i = 0; i < MaxItems_255; i++, item++ ){
		if( item->bCmd != ITEM_FREE ) continue;

		// put the item back onto the floor at its 
		// new location and with its new state

		sgbDeltaChanged = TRUE;
		item->bCmd = ITEM_GEN_FLOOR;
		item->mapRow = Items[itemIndex].MapRow;
		item->mapCol = Items[itemIndex].MapCol;
		item->info = Items[itemIndex];
		return;
	}
}

//----- (004467FD) --------------------------------------------------------
void DeltaSaveLevel()
{
    if( MaxCountOfPlayersInGame != 1 ){
        for( int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++ ){
            Player& player = Players[playerIndex];
            if( playerIndex != CurrentPlayerIndex ){
                player.LoadPlayerAnimationMask = PAF_0_NO;
            }
        }
		Players[CurrentPlayerIndex].dungeonVisited[(DUNGEON)Dungeon] = 1;
		delta_leave_sync((DUNGEON)Dungeon);
    }
}

//----- (th2) -------------------------------------------------------------
void DeltaLoadLevelMonster( int monsterIndex, uchar dungeon )
{
    const DMonsterStr& monsterInfo = sgLevels[dungeon].monsters[monsterIndex];
    const auto& summonInfo = sgLevels[dungeon].summons[monsterIndex];

    Monster& monster = Monsters[monsterIndex];
    if( monsterInfo.row == INIT_VAL ){
        return;
    }
    ClearMonsterOnMap(monsterIndex);
    int row = monsterInfo.row;
    int col = monsterInfo.col;
    monster.Row = row;
    monster.Col = col;
    monster.PrevRow = row;
    monster.PrevCol = col;
    monster.NextRow = row;
    monster.NextCol = col;
    int life = monsterInfo.life;
    
    if( life != -1 ){
        monster.CurrentLife = life;
    }
	if( life ){ // Or life > 0?
        decode_enemy( monsterIndex, monsterInfo.target );
        if( (row != 0 && row != 1) || col != 0 ){// непонятная проверка
            MonsterMap[ row ][ col ] = monsterIndex + 1;
        }
        if( monsterIndex < SummonMonsters_Count && summonInfo.spriteIndex != INIT_VAL ){
            AwakeSummon( monsterIndex, row, col, life, sgLevels[dungeon].summons[monsterIndex], true );
        }
        FixMonsterPosition(monsterIndex, monster.Orientation);
        monster.ActivationCounter = monsterInfo.activationCounter;
        // тут надо добавить сброс speechIndex (например для Lazarus) если он активирован
        //if( )
    }else{
        monster.PrevRow = row;
        monster.PrevCol = col;
        ClearMonsterOnMap(monsterIndex);
        if( monster.ai != AI_27_Diablo ){
            AddMonsterCorpse(monster.Row, monster.Col, monster.newBossId ? monster.udeadNum : monster.SpritePtr->deadSpriteNum, monster.Orientation);
        }
		monster.flag |= MF_15_KILLED;
        MayBe_KillPackBoss(monsterIndex);
    }
}

//----- (th2) -------------------------------------------------------------
void DeltaRestoreGroundItems( uchar dlvl )
{
    if( !sgLocals[dlvl].inited ){
        return;
    }
    // Restore all items info
    ItemsCount = sgLocals[dlvl].countItemsOnMap;
    for( int i = 0; i < MaxItems_255; i++ ){
        ItemActive[i] = sgLocals[dlvl].mapItemsFreeIndexes[i];
    }
    for( int i = 0; i < MaxItems_255; i++ ){
        ItemAvailable[i] = sgLocals[dlvl].itemsOnMapIndexesInvers[i];
    }
    for( int i = 0; i < MaxItems_255; i++ ){
        memcpy( &Items[i], &sgLocals[dlvl].items[i], sizeof(Items[i]) );
    }
    memcpy( ItemMap, sgLocals[dlvl].itemsOnGroundMap, sizeof( ItemMap ) );
}

//----- (0044685B) --------------------------------------------------------
void DeltaLoadLevel()
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	deltaload = true;

	uchar dungeon = (DUNGEON)Dungeon;
	DLevel& netLevelData = sgLevels[dungeon];

	if( dungeon != DUN_0_TOWN ){ // What about RavenHolm???
		for( int i = 0; i < MonstersCount; ++i ){
			DeltaLoadLevelMonster( i, dungeon );
		}
		// Unexplored map region in Dlvl 2 gets revealed as player comes back from Poisoned Water Supply,
		// because PWS shares the same Dlvl ID as Dlvl 2.  Fixed.
		memcpy( AutoMap, sgLocals[dungeon].automapsv, sizeof(AutoMap) );
	}

	// Items dropped in PWS get copied to Dlvl 2 for the same reason as the above. Fixed.
	// Not sure about the dlvl parameter in functions like On_MONSTDEATH(), though.
	DeltaRestoreGroundItems(dungeon);

	if( dungeon != DUN_0_TOWN ){ //What about RavenHolm???
		for( int i = 0; i < MaxObjects_127; ++i ){
		    auto cmd = netLevelData.objects[i].bCmd;
            switch( cmd ){
                case CMD_43_OPENDOOR:
                case CMD_44_CLOSEDOOR:
                case CMD_45_OPERATEOBJ:
                case CMD_46_PLROPOBJ:
                    SyncOpObject(-1, cmd, i);
                    break;
                case CMD_47_BREAKOBJ:
                    SyncBreakObj(-1, i);
                    break;
            }
		}

		for( int ii = 0; ii < ObjectsCount; ++ii ){
			int objectIndex = ObjectActive[ii];
			int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
			if( baseObjectIndex == BO_53_WALL_TRAP_FIRES_IN_ROW
				|| baseObjectIndex == BO_54_WALL_TRAP_FIRES_IN_COL ){
					ActivateWallTrap(objectIndex);
			}
		}
	}

	deltaload = false;
}

//----- (00446D7E) --------------------------------------------------------
void NetSendCmd( bool bHiPri, uchar bCmd )
{
	TCmd cmd{};
	cmd.bCmd = bCmd;
	if( bHiPri ){
        NetSendHiPri( (uchar*)&cmd, sizeof( cmd ) );
	}else{
        NetSendLoPri( (uchar*)&cmd, sizeof( cmd ) );
	}
}

//----- (00446D9C) --------------------------------------------------------
void NetSendCmdAwakeSummon( uint summonIndex, int row, int col, int dungeon, const DSummonStr& params )
{
	TCmdAwakeSummon cmd{};
	cmd.bCmd = CMD_98_AWAKEGOLEM;
	cmd.summonIndex = summonIndex;
	cmd.row = row;
	cmd.col = col;
	cmd.dungeon = (DUNGEON)dungeon;
	cmd.params = params;
    NetSendLoPri( (uchar*)&cmd, sizeof( cmd ) );
}

//----- (00446DD2) --------------------------------------------------------
void NetSendCmdLoc( int highPriority, char bCmd, short row, short col )
{
	TCmdLoc cmd{};
	cmd.bCmd = bCmd;
	cmd.row = (char) row;
	cmd.col = (char) col;
	if( highPriority ){
        NetSendHiPri( (uchar*)&cmd, sizeof( cmd ) );
	}else{
        NetSendLoPri( (uchar*)&cmd, sizeof( cmd ) );
	}
}

//----- (00446DFE) --------------------------------------------------------
void NetSendCmdLocParam1( int someFlag, char netCmdIndex, short row, short col, ushort firstArg )
{
	TCmdLocParam1 src{};
	src.netCmdIndex = netCmdIndex;
	src.row = (char) row;
	src.col = (char) col;
	src.firstArg = firstArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446E33) --------------------------------------------------------
void NetSendCmdLocParam2( int someFlag, char netCmdIndex, short row, short col, ushort firstArg, ushort secondArg )
{
	TCmdLocParam2 src{};
	src.netCmdIndex = netCmdIndex;
	src.row = (char) row;
	src.col = (char) col;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446E70) --------------------------------------------------------
void NetSendCmdLocParam3( int someFlag, char netCmdIndex, short row, short col, ushort firstArg, ushort secondArg, ushort thirdArg )
{
	TCmdLocParam3 src{};
	src.netCmdIndex = netCmdIndex;
	src.row = (char) row;
	src.col = (char) col;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	src.thirdArg = thirdArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446EB6) --------------------------------------------------------
void NetSendCmdParam1( int someFlag, char netCmdIndex, ushort firstArg )
{
	TCmdParam1 src{};
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446EDE) --------------------------------------------------------
void NetSendCmdParam2( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg )
{
	TCmdParam2 src{};
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446F0F) --------------------------------------------------------
void NetSendCmdParam3( int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg, ushort thirdArg )
{
	TCmdParam3 src{};
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	src.thirdArg = thirdArg;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446F48) --------------------------------------------------------
void NetSendCmdQuest( int someFlag, char questIndex )
{
	TCmdQuest src{};
	Quest& quest = Quests[questIndex];
	src.netCmdIndex = CMD_96_SYNCQUEST;
	src.questIndex = questIndex;
	src.status = quest.status;
	src.talkToFlag = quest.talkToFlag;
	src.status2 = quest.status2;
	src.status3 = quest.status3;
	src.status4 = quest.status4;
	src.status5 = quest.status5;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00446F8C) --------------------------------------------------------
void NetSendCmdGItem( int someFlag, uchar netCmdIndex, char master, char playerIndex, uchar itemIndex )
{
	Item& item = Items[itemIndex];

	TCmdGItem src{};
	src.netCmdIndex = netCmdIndex;
	src.bMaster = master;
	src.bPnum = playerIndex;
	src.itemIndex = itemIndex;
	src.dungeonLevel = Dungeon;
	src.mapRow = item.MapRow;
	src.mapCol = item.MapCol;
	src.timeStamp = 0;
	src.info = item;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (004471AB)---------------------------------------------------------
void NetSendCmdExtra(const TCmdGItem * p)
{
	TCmdGItem cmd;
	cmd = *p;	// struct copy
	cmd.netCmdIndex = CMD_93_ITEMEXTRA;
	cmd.timeStamp = 0;
	NetSendHiPri((uchar*)&cmd, sizeof(cmd));
}

//----- (004486C8) --------------------------------------------------------
uint On_ITEMEXTRA( const TCmdGItem* packet, int playerIndex )
{
	if (gbBufferMsgs == 1) {
		mega_add_data(playerIndex, packet, sizeof(TCmdGItem));
	}
	else {
		//DROPLOG("CMD_ITEMEXTRA(%d):  index->0x%8.8x ci->0x%8.8x seed->0x%8.8x xy->(%d,%d)\n",pnum,p->baseItemIndex,p->dropType,p->seed,p->x,p->y);
		// Check if in the delta table
		// drb.patch1.start.2/05/97
		//delta_get_item(p, plr[p->bPnum].plrlevel);
		//DROPLOG("  Looking in delta tbl[%d]\n",p->bLevel);
		delta_get_item(packet, packet->dungeonLevel);
		if( Dungeon == Players[playerIndex].dungeon ){
			//if (DungeonLevel == p->bLevel) {
			// drb.patch1.end.2/05/97
			SyncGetItem(packet->mapRow, packet->mapCol, packet->info.baseItemIndex, packet->info.dropType, (int)packet->info.seed,
				packet->info.genVersion, packet->info.quality, packet->info.magicFind, packet->info.uniqIndex);
		}
	}
	return sizeof(TCmdGItem);
}

//----- (004471D5) --------------------------------------------------------
void NetSendCmdPItem( int someFlag, char netCmdIndex, short row, short col )
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;

	TCmdPItem src{};

	src.bCmd = netCmdIndex;
	src.mapRow = row;
	src.mapCol = col;
	src.info = item;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00447382) --------------------------------------------------------
void NetSendCmdChItem( int isHand, char onBodyIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;
	TCmdChItem src{};
	src.netCmdIndex = CMD_48_CHANGEPLRITEMS;
	src.onBodyIndex = onBodyIndex;
	src.info = item; // Fixing item affixes got all messed up for other players after current player re-equip item.
	if( isHand ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (004473E1) --------------------------------------------------------
void NetSendCmdDelItem( int isHand, char onBodyIndex )
{
	TCmdDelItem src{};
	src.netCmdIndex = CMD_49_DELPLRITEMS;
	src.onBodyIndex = onBodyIndex;
	if( isHand ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00447403) --------------------------------------------------------
void NetSendCmdDItem(bool hiPri,int itemIndex)
{
	Item& item = Items[itemIndex];
	TCmdPItem cmd;
	cmd.bCmd = CMD_52_DROPITEM;
	cmd.mapRow = item.MapRow;
	cmd.mapCol = item.MapCol;
	cmd.info = item;
	if( hiPri ){
		NetSendHiPri( (uchar*)&cmd, sizeof(cmd) );
	}else{
		NetSendLoPri( (uchar*)&cmd, sizeof(cmd) );
	}
}


//----- (004475AC) --------------------------------------------------------
void NetSendCmdDamage( int someFlag, char targetPlayerIndex, int damage )
{
	TCmdDamage src{};
	src.netCmdIndex = CMD_50_PLRDAMAGE;
	src.targetPlayerIndex = targetPlayerIndex;
	src.damage = damage;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//----- (00447603) --------------------------------------------------------
void NetSendString( DWORD pmask, char* message )
{
	TCmdString cmd;

	cmd.bCmd = CMD_86_STRING;
//	int len = strlen( message );
//	strcpy( cmd.str, message );
	strncpy(cmd.str, message, sizeof(cmd.str) - 1); // Copy safely to avoid out of bound
	cmd.str[sizeof(cmd.str) - 1] = '\0'; // Null-terminated
	// 2 - cmd byte + str terminator byte
//	NetSendMask( pmask, (uchar*)&cmd, len + 2 );
	int sendLen = (int)strlen(cmd.str); // Send only what fits in cmd.str and pass length of truncated string
	NetSendMask(pmask, (uchar*)&cmd, sendLen + 2);
}

//----- (004475D7) --------------------------------------------------------
void SendCmdDamageMonster( int someFlag, ushort targetMonsterIndex, int damage )
{
	TCmdDamageMonster src{};
	src.netCmdIndex = CMD_37_MONSTDAMAGE;
	src.targetMonsterIndex = targetMonsterIndex;
	src.damage = damage;
	if( someFlag ){
        NetSendHiPri( (uchar*)&src, sizeof( src ) );
	}else{
        NetSendLoPri( (uchar*)&src, sizeof( src ) );
	}
}

//Used for syncing monster.acidDot
void SendCmdAcidDotMonster(int someFlag, ushort targetMonsterIndex, int DeltaMonAcidDot, DUNGEON MonsterMainDlvl, int MonAcidDot, char dotLastAttacker)
{
	TCmdAcidDotMonster src{};
	src.netCmdIndex = CMD_106_MONSTACIDDOT;
	src.targetMonsterIndex = targetMonsterIndex;
	src.DeltaMonAcidDot = DeltaMonAcidDot;
	src.dungeon = MonsterMainDlvl;
	src.MonAcidDot = MonAcidDot;
	src.dotLastAttacker = dotLastAttacker;
	if (someFlag) {
		NetSendHiPri((uchar*)&src, sizeof(src));
	}
	else {
		NetSendLoPri((uchar*)&src, sizeof(src));
	}
}

//Used for syncing player friendly/hostile state
//----- (th2) -------------------------------------------------------------
void SendCmdPvP(int someFlag, char playerIndex, bool IsPlayerEnemy)
{
	TCmdPvP src{};
	src.netCmdIndex = CMD_108_PVP;
	src.playerIndex = playerIndex;
	src.IsPlayerEnemy = IsPlayerEnemy;
	if (someFlag) {
		NetSendHiPri((uchar*)&src, sizeof(src));
	}
	else {
		NetSendLoPri((uchar*)&src, sizeof(src));
	}
}

void SendCmdWallRecast(int someFlag, char netCmdIndex, ushort firstArg, ushort secondArg, ushort thirdArg, uchar casterX, uchar casterY, uchar targetX, uchar targetY, uchar spellLevel, uchar direction)
{
	TCmdWallRecast src{};
	src.netCmdIndex = netCmdIndex;
	src.firstArg = firstArg;
	src.secondArg = secondArg;
	src.thirdArg = thirdArg;
	src.casterX = casterX;
	src.casterY = casterY;
	src.targetX = targetX;
	src.targetY = targetY;
	src.spellLevel = spellLevel;
	src.direction = direction;
	if (someFlag) {
		NetSendHiPri((uchar*)&src, sizeof(src));
	}
	else {
		NetSendLoPri((uchar*)&src, sizeof(src));
	}
}

//----- (0044763C) --------------------------------------------------------
void DeltaClosePortal( int playerIndex )
{
	DPortal& portalInfo = sgJunk.portals[playerIndex];
	memset(&portalInfo, INIT_VAL, sizeof( DPortal ));
	sgbDeltaChanged = true;
}

//----- (0044765B) --------------------------------------------------------
int ParseCmd( int playerIndex, const TCmd* packet)
{
	sbLastCmd = packet->bCmd;
	//if( is(sbLastCmd, CMD_2_ACK_PLRINFO, 51, 53, 54) ) __debugbreak();
	if( sgwPackPlrOffsetTbl[playerIndex] != 0 && sbLastCmd != CMD_2_ACK_PLRINFO && sbLastCmd != CMD_54_SEND_PLRINFO ){
		return 0;
	}
	#if 0
	static ofstream cmdLog("cmd.log");
	cmdLog << (int)packet->bCmd << endl;
	#endif
	switch( packet->bCmd ){
	case CMD_1_WALKXY                     : return On_WALKXY( (TCmdLoc*)packet, playerIndex );
	case CMD_2_ACK_PLRINFO                : return On_ACK_PLRINFO( (TCmdPlrInfoHdr*)packet, playerIndex );
	case CMD_3_ADDSTR                     : return On_ADDSTR( (TCmdParam1*)packet, playerIndex );
	case CMD_4_ADDMAG                     : return On_ADDDEX( (TCmdParam1*)packet, playerIndex );
	case CMD_5_ADDDEX                     : return On_ADDMAG( (TCmdParam1*)packet, playerIndex );
	case CMD_6_ADDVIT                     : return On_ADDVIT( (TCmdParam1*)packet, playerIndex );
	case CMD_7_SBSPELL                    : return On_SBSPELL( (TCmdParam1*)packet, playerIndex );
	case CMD_8_GETITEM                    : return On_GETITEM( (TCmdGItem*)packet, playerIndex );
	case CMD_9_AGETITEM                   : return On_AGETITEM( (TCmdGItem*)packet, playerIndex );
	case CMD_10_PUTITEM                   : return On_PUTITEM( (TCmdPItem*)packet, playerIndex );
	case CMD_11_RESPAWNITEM               : return On_RESPAWNITEM( (TCmdPItem*)packet, playerIndex );
	case CMD_12_ATTACKXY                  : return On_ATTACKXY( (TCmdLoc*)packet, playerIndex );
	case CMD_13_RATTACKXY                 : return On_RATTACKXY( (TCmdLoc*)packet, playerIndex );
	case CMD_14_SPELLXY                   : return On_SPELLXY( (TCmdLocParam2*)packet, playerIndex );
	case CMD_15_TSPELLXY                  : return On_TSPELLXY( (TCmdLocParam2*)packet, playerIndex );
	case CMD_16_OPOBJXY                   : return On_OPOBJXY( (TCmdLocParam1*)packet, playerIndex );
	case CMD_17_DISARMXY                  : return On_DISARMXY( (TCmdLocParam1*)packet, playerIndex );
	case CMD_18_ATTACKID                  : return On_ATTACKID( (TCmdParam1*)packet, playerIndex );
	case CMD_19_ATTACKPID                 : return On_ATTACKPID( (TCmdParam1*)packet, playerIndex );
	case CMD_20_RATTACKID                 : return On_RATTACKID( (TCmdParam1*)packet, playerIndex );
	case CMD_21_RATTACKPID                : return On_RATTACKPID( (TCmdParam1*)packet, playerIndex );
	case CMD_22_SPELLID                   : return On_SPELLID( (TCmdParam3*)packet, playerIndex );
	case CMD_23_SPELLPID                  : return On_SPELLPID( (TCmdParam3*)packet, playerIndex );
	case CMD_24_TSPELLID                  : return On_TSPELLID( (TCmdParam3*)packet, playerIndex );
	case CMD_25_TSPELLPID                 : return On_TSPELLPID( (TCmdParam3*)packet, playerIndex );
	case CMD_26_RESURRECT                 : return On_RESURRECT( (TCmdParam1*)packet, playerIndex );
	case CMD_27_OPOBJT                    : return On_OPOBJT( (TCmdParam1*)packet, playerIndex );
	case CMD_28_KNOCKBACK                 : return On_KNOCKBACK( (TCmdParam2*)packet, playerIndex );
	case CMD_29_TALKXY                    : return On_TALKXY( (TCmdLocParam1*)packet, playerIndex );
	case CMD_30_NEWLVL                    : return On_NEWLVL( (TCmdParam2*)packet, playerIndex );
	case CMD_31_WARP                      : return On_WARP( (TCmdParam1*)packet, playerIndex );
	case CMD_35_SYNCDATA                  : return On_SYNCDATA( (TCmd*)packet, playerIndex );
	case CMD_36_MONSTDEATH                : return On_MONSTDEATH( (TCmdLocParam2*)packet, playerIndex );
	case CMD_37_MONSTDAMAGE               : return On_MONSTDAMAGE( (TCmdDamageMonster*)packet, playerIndex );
	case CMD_38_PLRDEAD                   : return On_PLRDEAD( (TCmdParam1*)packet, playerIndex );
	case CMD_39_REQUESTGITEM              : return On_REQUESTGITEM( (TCmdGItem*)packet, playerIndex );
	case CMD_40_REQUESTAGITEM             : return On_REQUESTAGITEM( (TCmdGItem*)packet, playerIndex );
	case CMD_41_GOTOGETITEM               : return On_GOTOGETITEM( (TCmdLocParam1*)packet, playerIndex );
	case CMD_42_GOTOAGETITEM              : return On_GOTOAGETITEM( (TCmdLocParam1*)packet, playerIndex );
	case CMD_43_OPENDOOR                  : return On_OPENDOOR( (TCmdParam1*)packet, playerIndex );
	case CMD_44_CLOSEDOOR                 : return On_CLOSEDOOR( (TCmdParam1*)packet, playerIndex );
	case CMD_45_OPERATEOBJ                : return On_OPERATEOBJ( (TCmdParam1*)packet, playerIndex );
	case CMD_46_PLROPOBJ                  : return On_PLROPOBJ( (TCmdParam2*)packet, playerIndex );
	case CMD_47_BREAKOBJ                  : return On_BREAKOBJ( (TCmdParam2*)packet, playerIndex );
	case CMD_48_CHANGEPLRITEMS            : return On_CHANGEPLRITEMS( (TCmdChItem*)packet, playerIndex );
	case CMD_49_DELPLRITEMS               : return On_DELPLRITEMS( (TCmdDelItem*)packet, playerIndex );
	case CMD_50_PLRDAMAGE                 : return On_PLRDAMAGE( (TCmdDamage*)packet, playerIndex );
	case CMD_51_PLRLEVEL                  : return On_PLRLEVEL( (TCmdParam1*)packet, playerIndex );
	case CMD_52_DROPITEM                  : return On_DROPITEM( (TCmdPItem*)packet, playerIndex );
	case CMD_53_PLAYER_JOINLEVEL          : return On_PLAYER_JOINLEVEL( (TCmdLocParam1*)packet, playerIndex );
	case CMD_54_SEND_PLRINFO              : return On_SEND_PLRINFO( (TCmdPlrInfoHdr*)packet, playerIndex );
	case CMD_55_SATTACKXY                 : return On_SATTACKXY( (TCmdLoc*)packet, playerIndex );
	case CMD_56_ACTIVATEPORTAL            : return On_ACTIVATEPORTAL( (TCmdLocParam3*)packet, playerIndex );
	case CMD_57_DEACTIVATEPORTAL          : return On_DEACTIVATEPORTAL( packet, playerIndex );

	case CMD_83_DLEVEL_JUNK or CMD_84_DLEVEL_END: return DReceiveChunk( playerIndex, (TCmdPlrInfoHdr*)packet );

	case CMD_85_HEALOTHER				  : return On_HEALOTHER( (TCmdParam1*)packet, playerIndex );
	case CMD_86_STRING					  : return On_STRING( (TCmdString*)packet, playerIndex );
	case CMD_87_SETSTR					  : return On_SETSTR( (TCmdParam1*)packet, playerIndex );
	case CMD_88_SETMAG					  : return On_SETMAG( (TCmdParam1*)packet, playerIndex );
	case CMD_89_SETDEX					  : return On_SETDEX( (TCmdParam1*)packet, playerIndex );
	case CMD_90_SETVIT					  : return On_SETVIT( (TCmdParam1*)packet, playerIndex );
	case CMD_91_RETOWN					  : return On_RETOWN( packet, playerIndex );
	case CMD_93_ITEMEXTRA                 : return On_ITEMEXTRA( (TCmdGItem*)packet, playerIndex );
	case CMD_92_SPELLXYD				  : return On_SPELLXYD( (TCmdLocParam3*)packet, playerIndex );
	case CMD_94_SYNCPUTITEM				  : return On_SYNCPUTITEM( (TCmdPItem*)packet, playerIndex );
	case CMD_95_KILLGOLEM				  : return On_KILLGOLEM( (TCmdLocParam2*)packet, playerIndex );
	case CMD_96_SYNCQUEST				  : return On_SYNCQUEST( (TCmdQuest*)packet, playerIndex );
	case CMD_97_ENDSHIELD				  : return On_ENDSHIELD( packet, playerIndex );
	case CMD_98_AWAKEGOLEM				  : return On_AWAKEGOLEM( (TCmdAwakeSummon*)packet, playerIndex );
	case CMD_99_DESTROY_REFLECT			  : return On_DESTROY_REFLECT( packet, playerIndex);
	case CMD_100_OPEN_WEAK_UBER			  : return On_OPEN_WEAK_UBER( packet, playerIndex);
	case CMD_105_SETPERK				  : return On_SETPERK( (TCmdParam2*)packet, playerIndex );
	case CMD_106_MONSTACIDDOT			  : return On_MONSTACIDDOT((TCmdAcidDotMonster*)packet, playerIndex);
	case CMD_107_TSPELLXYD				  : return On_TSPELLXYD((TCmdLocParam3*)packet, playerIndex);
	case CMD_108_PVP					  : return On_PVP((TCmdPvP*)packet, playerIndex);
	case CMD_109_WallRecast				  : return On_WallRecast((TCmdWallRecast*)packet, playerIndex);
	default:
		if( packet->bCmd | CMD_DLEVEL && (packet->bCmd & ~CMD_DLEVEL) < DungeonCount ){
			return DReceiveChunk( playerIndex, (TCmdPlrInfoHdr*)packet );
		}else{
			SNetDropPlayer(playerIndex, 0x40000006); return 0; // wrong network command
		}
	}
}

// TODO: update to osc
// dreceive_chunk
//----- (00447B88) --------------------------------------------------------
int DReceiveChunk( int playerIndex, const TCmdPlrInfoHdr* packet)
{
    if( gbDeltaSender != playerIndex ){
		// we are unexpectedly receiving delta info from somebody
		// if they aren't sending us the starting information, ignore it
		// otherwise, make them the new delta source
        if( packet->bCmd == CMD_84_DLEVEL_END ){
			// we're getting an end chunk without any intervening delta
			// which means there is no delta information
        }
		// TODO: check condition for town quest dlevel validity
		else if( packet->bCmd != CMD_DLEVEL /*CMD_58_DLEVEL_0*/ || packet->wOffset != 0 ){
            return packet->wBytes + sizeof( *packet );
        }
		gbDeltaSender = playerIndex;
        sgbRecvCmd = CMD_84_DLEVEL_END;
    }
    
    if( sgbRecvCmd == CMD_84_DLEVEL_END ){
        if( packet->bCmd == CMD_84_DLEVEL_END ){
			// we received the end command immediately, which 
			// means there was no level delta info
			sgbDeltaChunks = DELTA_CHUNKS - 1; // set 99% done
            return packet->wBytes + sizeof( *packet );
        }
        // TODO: check condition for town quest dlevel validity
		if( packet->bCmd != CMD_DLEVEL /*CMD_58_DLEVEL_0*/ || packet->wOffset != 0 ){
			// someone is in the middle of sending us info
			// but we don't have the beginning
			// just ignore the message
			return packet->wBytes + sizeof( *packet );
        }
        
		// we just started receiving new data from the player
		sgdwRecvOffset = 0;
        sgbRecvCmd = packet->bCmd;
    }
    else if( sgbRecvCmd != packet->bCmd ){
		// since we are receiving a new delta chunk, 
		// we must have finished receiving the last delta chunk.
		// process last chunk before handling next chunk.
		DParseChunk(sgbRecvCmd, sgdwRecvOffset);

		// what's the new command?
		if( packet->bCmd == CMD_84_DLEVEL_END ){
			// set 99% done flag
			sgbDeltaChunks = DELTA_CHUNKS - 1;
            sgbRecvCmd = CMD_84_DLEVEL_END;
            return packet->wBytes + sizeof( *packet );
        }

		sgdwRecvOffset = 0;
        sgbRecvCmd = packet->bCmd;
    }
    
    memcpy( &sgbRecvLevel[packet->wOffset], &packet[1], packet->wBytes );
    
	sgdwRecvOffset += packet->wBytes;
    return packet->wBytes + sizeof( *packet );
}

// dparse_chunk
//----- (00447C46) --------------------------------------------------------
void DParseChunk( uchar cmd, int len )
{
	// first byte is a flag which indicates whether the data is compressed
	if( sgbRecvLevel[0] ){
		Expand( &sgbRecvLevel[1], len, sizeof(sgbRecvLevel) - 1);
	}
	uchar* data = &sgbRecvLevel[1];

	if( cmd == CMD_83_DLEVEL_JUNK ){
        DParseJunk( data );
	}
	else if( cmd & CMD_DLEVEL ){
		DLevel& netLevelData = sgLevels[cmd & ~CMD_DLEVEL];
		if( NetItem ) data = DeltaParseItems( data, netLevelData.items );
		data = DeltaParseObjects( data, netLevelData.objects );
        data = DeltaParseMonsters( data, netLevelData.monsters );
        data = DeltaParseSummons( data, netLevelData.summons );
    }
    else{
        TerminateWithError( "msg:1" );
    }
	++sgbDeltaChunks;
	sgbDeltaChanged = true;
}

//----- (00447CCE) --------------------------------------------------------
uchar* DeltaParseItems( uchar* src, TCmdPItem* dst)
{
	for( int i = 0; i < MaxItems_255; i++, dst++ ){
		if( *src == INIT_VAL ){
			memset(dst, INIT_VAL, sizeof(*dst));
			src++;
		}else{
			memcpy(dst, src, sizeof(*dst));
			src += sizeof(*dst);
		}
	}
	return src;
}

//----- (00447D16) --------------------------------------------------------
uchar* DeltaParseObjects( uchar* src, DObjectStr* dst )
{
    size_t size = sizeof(*src) * MaxObjects_127;
	memcpy(dst, src, size);
	return src + size;
}

//----- (00447D2A) --------------------------------------------------------
uchar* DeltaParseMonsters( uchar* src, DMonsterStr* dst )
{
	for( int i = 0; i < Monsters_Max_Count; ++i, ++dst ){
		if( *src == INIT_VAL ){
			memset(dst, INIT_VAL, sizeof(*dst));
			++src;
		}else{
			memcpy(dst, src, sizeof(*dst));
			src += sizeof(*dst);
		}
	}
	return src;
}

//----- (th3) -------------------------------------------------------------
uchar* DeltaParseSummons( uchar* src, DSummonStr* dst )
{
	for( int i = 0; i < SummonMonsters_Count; ++i, ++dst ){
		if( *src == INIT_VAL ){
			memset(dst, INIT_VAL, sizeof(*dst));
			++src;
		}else{
			memcpy(dst, src, sizeof(*dst));
			src += sizeof(*dst);
		}
	}
	return src;
}

//----- (00447D6E) --------------------------------------------------------
void DParseJunk( uchar* src )
{
	for( int i = 0; i < PlayersMax_4; ++i ){
		DPortal& portalInfo = sgJunk.portals[i];
		if( *src == INIT_VAL ){
			memset(&portalInfo, -1, sizeof(DPortal));
			SyncPortal( i, 0, 0, 0, DUN_0_TOWN );
			++src;
		}else{
			memcpy(&portalInfo, src, sizeof( DPortal ));
			SyncPortal( i, 1, portalInfo.row, portalInfo.col, portalInfo.dungeon );
			src += sizeof( DPortal );
		}
	}
	int q = 0;
	for( auto& quest : Quests ){
		memcpy( &sgJunk.quests[q], src, sizeof(MultiQuests) );//Copy over host data first
		if( BaseQuests[q].flag & QF_MULTI ){
			quest.status = sgJunk.quests[q].status; // Then sync initial quest states for newly joined player.  This is now done correctly.  Note that ResetQuests() is called after this initial quest status sync, and the original ResetQuests() logic was broken too, FIXED.
			quest.talkToFlag = sgJunk.quests[q].talkToFlag;
			quest.status2 = sgJunk.quests[q].status2;
			quest.status3 = sgJunk.quests[q].status3;
			quest.status4 = sgJunk.quests[q].status4;
			quest.status5 = sgJunk.quests[q].status5;
		}
		src += sizeof(MultiQuests);//However, I'm not too sure about this pointer arithmetic here...
		q++;
	}
}

//----- (00447E27) --------------------------------------------------------
int On_SYNCDATA( void* packet, int playerIndex)
{
	return sync_applyData( playerIndex, (const uchar*)packet );
}

//----- (00447E32) --------------------------------------------------------
uint On_WALKXY( const TCmdLoc* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		ClearPlayerPath(playerIndex);
		MakePlayerPath(playerIndex, packet->row, packet->col, 1);
		player.destAction = PCMD_M1_NOTHING;
	}
	return sizeof(*packet);
}

//----- (00447E87) --------------------------------------------------------
uint On_ADDSTR( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof(*packet) );
	}else if( packet->firstArg <= 256 ){
		ModifyPlayerStrength(playerIndex, packet->firstArg);
	}
	return sizeof(*packet);
}

//----- (00447EB8) --------------------------------------------------------
uint On_ADDDEX( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof(*packet) );
	}else if( packet->firstArg <= 256 ){
		ModifyPlayerMagic(playerIndex, packet->firstArg);
	}
	return sizeof(*packet);
}

//----- (00447EE9) --------------------------------------------------------
uint On_ADDMAG( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof(*packet) );
	}else if( packet->firstArg <= 256 ){
		ModifyPlayerDexterity(playerIndex, packet->firstArg);
	}
	return sizeof(*packet);
}

//----- (00447F1A) --------------------------------------------------------
uint On_ADDVIT( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof(*packet) );
	}else if( packet->firstArg <= 256 ){
		ModifyPlayerVitality(playerIndex, packet->firstArg);
	}
	return sizeof(*packet);
}

//----- (00447F4B) --------------------------------------------------------
uint On_SBSPELL( const TCmdParam1* packet, int playerIndex) // TODO: looks like unused, find out why
{
	if( gbBufferMsgs != BUFFER_ON ){
	    Player& player = Players[playerIndex];
	    int spellIndex = packet->firstArg;
		if( IsSpellEnabledHere(spellIndex, playerIndex) ){
			player.spellIndex = spellIndex;
			player.readySpellType = player.SpellType; //LOBYTE(player.unused_SBKSpellType);
			player.readySpellTargetType = SO_1_SPELL;
			player.destAction = PCMD_12_CAST_SPELL_TO_NONE;
		}else{
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
		}
	}
	return sizeof(*packet);
}

//----- (00447FDE) --------------------------------------------------------
void msg_errorf( const char* format, ...)
{
    static i64 msg_err_timer;
	char msg[256];
	va_list va;
	
	va_start(va, format);
	i64 ticks = GetTickCountTh();
	if( ticks - msg_err_timer >= 5000_ms ){
		msg_err_timer = ticks;
		vsprintf(msg, format, va);
		ErrorPlrMsg(msg);
	}
	va_end(va);
}

//----- (00448025) --------------------------------------------------------
uint On_GOTOGETITEM( const TCmdLocParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, packet->row, packet->col, 0);
		player.destAction = PCMD_15_GET_ITEM_FROM_LAND_ON_CURSOR;
		player.destParam1 = packet->firstArg;
	}
	return sizeof(*packet);
}

//----- (00448082) --------------------------------------------------------
uint On_REQUESTGITEM( const TCmdGItem* packet, int playerIndex )
{
	if( NetItem ){
		auto src = (TCmdGItem*)packet;
		Player& player = Players[playerIndex];

		if( gbBufferMsgs == BUFFER_ON 
		 || !i_own_level(player.dungeon)
			// make sure we don't start resending an item we just picked up.
		 || !CheckGetRecord(src->info) ){
			return sizeof TCmdGItem;
		}
		// TODO: вроде почти одинаковы, но алгоритм в критических мелочах отличался. Наверняка либо тут либо выше ошибка
		int itemIndex = FindGetItem(src->info.baseItemIndex, src->info.dropType, (int)src->info.seed, src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
		if( itemIndex != -1 ){
			// Item found
			NetSendCmdGItem2(0, CMD_8_GETITEM, CurrentPlayerIndex, src->bPnum, packet);
			if( src->bPnum != CurrentPlayerIndex ){
				SyncGetItem(src->mapRow, src->mapCol, src->info.baseItemIndex, src->info.dropType, (int)src->info.seed,
					src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
			}else{
				AutoGetItem(CurrentPlayerIndex, src->itemIndex); 
			}
			AddGetRecord(src->info);
		}else{
			// Item not found. Resending
			if( !NetSendCmdReq2(CMD_39_REQUESTGITEM, CurrentPlayerIndex, src->bPnum, packet) ){
				NetSendCmdExtra(packet);
			}
		}
		return sizeof TCmdGItem;
	}else{
		if( playerIndex == CurrentPlayerIndex && packet->bPnum == CurrentPlayerIndex && gbBufferMsgs != BUFFER_ON ){
			int ii = FindGetItem( packet->info.baseItemIndex, packet->info.dropType, (int)packet->info.seed, packet->info.genVersion, // TODO: expand seed
								  packet->info.quality, packet->info.magicFind, packet->info.uniqIndex );
			if( ii != -1 )
			{
				InvGetItem( CurrentPlayerIndex, ii );
			}
		}
	}
	return sizeof(TCmdGItem);
}

//----- (0044A37C) --------------------------------------------------------
void NetSendMyselfPri(uchar* pbMsg, int bLen)
{
	if (! pbMsg) return;
	if (! bLen) return;
	tmsg_add(pbMsg, bLen);
}

//----- (00448155) --------------------------------------------------------
void NetSendCmdGItem2( int usOnly, char netCmdIndex, char master, char playerIndex, const TCmdGItem* packet )
{
	TCmdGItem src;
	memcpy(&src, packet, sizeof TCmdGItem);
	src.netCmdIndex = netCmdIndex;
	src.bMaster = master;
	src.bPnum = playerIndex;

	if( !usOnly ){
		src.timeStamp = 0;
		NetSendHiPri((uchar*)&src, sizeof TCmdGItem);
		return;
	}

	i64 time = GetTickCountTh(); // перехватываем вызов
	if( src.timeStamp ){
		if( time - src.timeStamp > 5000_ms ){
			return;
		}
	}else{
		src.timeStamp = time;
	}
	NetSendMyselfPri((uchar*)&src, sizeof(src));
}

//----- (00447158) --------------------------------------------------------
bool NetSendCmdReq2(BYTE bCmd,BYTE mast,BYTE pnum,const TCmdGItem * p)
{
	TCmdGItem cmd;

	cmd = *p;	// struct copy.
	cmd.netCmdIndex = bCmd;
	cmd.bPnum = pnum;
	cmd.bMaster = mast;

	// Have I been sending this back to myself for more than 5 seconds?
	i64 dwCurr = GetTickCountTh();
	if( cmd.timeStamp == 0 ){            // Timer inited in original send
		cmd.timeStamp = dwCurr;
	}else if( dwCurr - cmd.timeStamp > RESEND_TIME ){
		return false;
	}
	NetSendMyselfPri((uchar*)&cmd,sizeof(cmd));
	return true;
}

//----- (004481B9) --------------------------------------------------------
bool i_own_level( int dungeonLevel )
{
	int playerIndex = 0;
	for( ; playerIndex < PlayersMax_4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && !player.lvlChanging && player.dungeon == dungeonLevel
		 && (playerIndex != CurrentPlayerIndex || !gbBufferMsgs) ){
			break;
		}
	}
	return playerIndex == CurrentPlayerIndex;
}

//----- (00448203) --------------------------------------------------------
uint On_GETITEM( TCmdGItem* packet, int playerIndex )
{
	auto src = (TCmdGItem*)packet;

	if( gbBufferMsgs == 1 ){
		mega_add_data(playerIndex, packet, sizeof TCmdGItem);
	}else{
		int itemIndex = FindGetItem(src->info.baseItemIndex, src->info.dropType, (int)src->info.seed, src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
		// Check if in the delta table
		if( delta_get_item(packet, src->dungeonLevel) ){
			// Found in delta
			if( Dungeon == src->dungeonLevel || src->bPnum == CurrentPlayerIndex ){
				if( src->bMaster == CurrentPlayerIndex ){
					// item has already been picked up
				}else if( src->bPnum == CurrentPlayerIndex ){
					if( Dungeon != src->dungeonLevel ){
						int hitem = SyncPutItem(playerIndex, src->mapRow, src->mapCol, src->info);
						if( hitem != -1 ) InvGetItem(CurrentPlayerIndex, hitem);
					}else{
						// Local player got item
						InvGetItem(CurrentPlayerIndex, itemIndex);
					}
				}else{
					// "Remote player (%d) got item.\n", packet->bPnum
					SyncGetItem(src->mapRow, src->mapCol, src->info.baseItemIndex, src->info.dropType, (int)src->info.seed,
					            src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
				}
			}
		}else{
			// Not found, so let's send it to ourselves again
			NetSendCmdGItem2(true, CMD_8_GETITEM, src->bMaster, src->bPnum, packet);
		}
	}
	return sizeof(TCmdGItem);
}

//----- (00448596) --------------------------------------------------------
uint On_AGETITEM( const TCmdGItem* packet, int playerIndex )
{
	auto src = packet;
	Player& player = Players[CurrentPlayerIndex];

	if( gbBufferMsgs == 1 ){
		mega_add_data(playerIndex, packet, sizeof TCmdGItem);
		return sizeof TCmdGItem;
	}
	// TODO: check why it originally unused
	int nIndex = FindGetItem(src->info.baseItemIndex, src->info.dropType, (int)src->info.seed,
		src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
	if( !delta_get_item(packet, src->dungeonLevel) ){
		NetSendCmdGItem2(1, CMD_9_AGETITEM, src->bMaster, src->bPnum, packet);
		return sizeof TCmdGItem;
	}
	DUNGEON dungeon = src->dungeonLevel;
	if( (Dungeon != dungeon && src->bPnum != CurrentPlayerIndex) || src->bMaster == CurrentPlayerIndex ){
		return sizeof TCmdGItem;
	}
	if( src->bPnum != CurrentPlayerIndex ){
		SyncGetItem(src->mapRow, src->mapCol, src->info.baseItemIndex, src->info.dropType, (int)src->info.seed,
			src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
		return sizeof TCmdGItem;
	}
	int itemIndex;
	if( Dungeon == dungeon ){
		itemIndex = src->itemIndex;
	}else{
		itemIndex = SyncPutItem(CurrentPlayerIndex, player.Row, player.Col, src->info);
		if( itemIndex == -1 ){
			return sizeof TCmdGItem;
		}
	}
	AutoGetItem(CurrentPlayerIndex, itemIndex);
	return sizeof TCmdGItem;
}

//----- (00448A25) --------------------------------------------------------
uint On_RESPAWNITEM( const TCmdPItem* packet, int playerIndex )
{
	auto src = (TCmdPItem*)packet;
	Player& player = Players[playerIndex];

	if( gbBufferMsgs == 1 ){ // BUFFER_ON
		mega_add_data(playerIndex, packet, sizeof TCmdPItem);
		return sizeof TCmdPItem;
	}
	if( Dungeon == player.dungeon && playerIndex != CurrentPlayerIndex ){
		SyncPutItem(playerIndex, src->mapRow, src->mapCol, src->info); // -AC
	}
	RemoveGetRecord(src->info);
	// Adding to delta
	delta_put_item(packet, src->mapRow, src->mapCol, player.dungeon);
	// Dropping into delta tbl
	return sizeof TCmdPItem;
}

//----- (00448468) --------------------------------------------------------
uint On_GOTOAGETITEM( const TCmdLocParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, packet->row, packet->col, 0);
		player.destAction = PCMD_16_GET_ITEM_FROM_LAND_IN_INVENTORY;
		player.destParam1 = packet->firstArg;
	}
	return sizeof( *packet );
}

//----- (004484C5) --------------------------------------------------------
uint On_REQUESTAGITEM( const TCmdGItem* packet, int playerIndex )
{
	if( NetItem ){
		auto src = (TCmdGItem*)packet;
		Player& player = Players[playerIndex];

		if( gbBufferMsgs == BUFFER_ON 
		 || !i_own_level(player.dungeon)
		 || !CheckGetRecord(src->info) ){
			return sizeof TCmdGItem;
		}
		// TODO: вроде почти одинаковы, но алгоритм в критических мелочах отличался. Наверняка либо тут либо выше ошибка
		int itemIndex = FindGetItem(src->info.baseItemIndex, src->info.dropType, (int)src->info.seed, src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
		if( itemIndex == -1 ){
			if( !NetSendCmdReq2(CMD_40_REQUESTAGITEM, CurrentPlayerIndex, src->bPnum, packet) ){
				NetSendCmdExtra(packet);
			}
		}else{
			NetSendCmdGItem2(0, CMD_9_AGETITEM, CurrentPlayerIndex, src->bPnum, packet);
			if( src->bPnum == CurrentPlayerIndex ){
				AutoGetItem(CurrentPlayerIndex, src->itemIndex); 
			}else{
				SyncGetItem(src->mapRow, src->mapCol, src->info.baseItemIndex, src->info.dropType, (int)src->info.seed,
					src->info.genVersion, src->info.quality, src->info.magicFind, src->info.uniqIndex);
			}
			AddGetRecord(src->info);
		}
		return sizeof TCmdGItem;
	}else{
		if( playerIndex == CurrentPlayerIndex && packet->bPnum == CurrentPlayerIndex && gbBufferMsgs != BUFFER_ON ){
			int ii = FindGetItem( packet->info.baseItemIndex, packet->info.dropType, (int)packet->info.seed, packet->info.genVersion, // TODO: expand seed
				packet->info.quality, packet->info.magicFind, packet->info.uniqIndex);
			if( ii != -1 ){
				AutoGetItem( CurrentPlayerIndex, packet->itemIndex );
			}
		}
		return sizeof( *packet );
	}
}

//----- (00448727) --------------------------------------------------------
uint On_PUTITEM( const TCmdPItem* packet, int playerIndex )
{
	if( NetItem ){
		if( gbBufferMsgs == BUFFER_ON ){
			mega_add_data( playerIndex, packet, sizeof( *packet ) );
		}else{
			Player& player = Players[playerIndex];
			if( Dungeon == player.dungeon ){
				int itemIndex = 0;
				if( playerIndex == CurrentPlayerIndex ){
					itemIndex = InvPutItem( playerIndex, packet->mapRow, packet->mapCol );
				}else{
					itemIndex = SyncPutItem(CurrentPlayerIndex, player.Row, player.Col, packet->info);
				}
				if( itemIndex != -1 ){
					// Put successful. Putting item into delta
					RemoveGetRecord(packet->info);
					// Dropping into delta tbl
					delta_put_item(packet, Items[itemIndex].MapRow, Items[itemIndex].MapCol, player.dungeon);
					check_update_plr(playerIndex);
				}else{
					// Put unsuccessful
				}
			}else{
				RemoveGetRecord(packet->info);
				// Item not on my level. Adding to delta
				delta_put_item(packet, packet->mapRow, packet->mapCol, player.dungeon);
				check_update_plr(playerIndex);
			}
		}
	}else{
		Player& player = Players[playerIndex];
		if( gbBufferMsgs == BUFFER_ON ){
			mega_add_data( playerIndex, packet, sizeof( *packet ) );
		}else if( playerIndex == CurrentPlayerIndex ){
			if( Dungeon == player.dungeon ){
				int itemIndex = InvPutItem( playerIndex, packet->mapRow, packet->mapCol );
				if( itemIndex != -1 ){
					check_update_plr( playerIndex );
				}
			}else{
				check_update_plr( playerIndex );
			}
		}
	}
	return sizeof( *packet );
}

//----- (004488FC) --------------------------------------------------------
void check_update_plr( int playerIndex )
{
	if( MaxCountOfPlayersInGame != 1 && playerIndex == CurrentPlayerIndex ){
		AutoSaveTickCount = GetTickCountTh();
		UpdatePlayerFile();
	}
}

//----- (00448916) --------------------------------------------------------
uint On_SYNCPUTITEM( const TCmdPItem* packet, int playerIndex )
{
	Player& player = Players[playerIndex];

	if( gbBufferMsgs == BUFFER_ON ){
		mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex == CurrentPlayerIndex ){
		if( Dungeon == player.dungeon ){
			int itemIndex = SyncPutItem( playerIndex, packet->mapRow, packet->mapCol, packet->info);
			if( itemIndex != -1 ){
				check_update_plr( playerIndex );
			}
		}else{
			check_update_plr( playerIndex );
		}
	}
	return sizeof( *packet );
}

//----- (00448AF4) --------------------------------------------------------
uint On_ATTACKXY( const TCmdLoc* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, packet->row, packet->col, 0);
		player.destAction = PCMD_9_MELEE_ATTACK_TO_NONE;
		player.destParam1 = packet->row;
		player.destParam2 = packet->col;
	}
	return sizeof( *packet );
}

//----- (00448B5F) --------------------------------------------------------
uint On_SATTACKXY( const TCmdLoc* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		ClearPlayerPath(playerIndex);
		player.destAction = PCMD_9_MELEE_ATTACK_TO_NONE;
		player.destParam1 = packet->row;
		player.destParam2 = packet->col;
	}
	return sizeof( *packet );
}

//----- (00448BBD) --------------------------------------------------------
uint On_RATTACKXY( const TCmdLoc* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		ClearPlayerPath(playerIndex);
		player.destAction = PCMD_10_RANGE_ATTACK_TO_NONE;
		player.destParam1 = packet->row;
		player.destParam2 = packet->col;
	}
	return sizeof( *packet );
}

//----- (00448C1B) --------------------------------------------------------
uint On_SPELLXYD(const TCmdLocParam3* packet, int playerIndex)
{
	Player& player = Players[playerIndex];
	if (gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon)
	{
		if (IsSpellEnabledHere(packet->firstArg, playerIndex))
		{
			ClearPlayerPath(playerIndex);
			player.destAction = PCMD_26_CAST_NOT_ORIENTED_SPELL;
			player.destParam1 = packet->row;
			player.destParam2 = packet->col;
			player.destParam3 = packet->secondArg;
			player.destParam4 = packet->thirdArg;
			player.spellIndex = (char) packet->firstArg;
			player.readySpellType = player.SpellType;
			player.readySpellTargetType = SO_0_SKILL;
		}
		else
		{
			msg_errorf("%s has cast an illegal spell.", player.playerName);
		}
	}
	return sizeof(*packet);
}

//----- (00448C1B) --------------------------------------------------------
uint On_WallRecast(const TCmdWallRecast* packet, int playerIndex)
{
	Player& player = Players[playerIndex];
	if (gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon && CurrentPlayerIndex == packet->secondArg)
	{
		if (IsSpellEnabledHere(packet->firstArg, playerIndex))
		{
			ClearPlayerPath(playerIndex);
			PlayerWall[playerIndex][SpellIdxToWallType[packet->firstArg]][5] = packet->direction; // Update orientation info.
			PlayerWall[playerIndex][SpellIdxToWallType[packet->firstArg]][6] = packet->thirdArg; // Update time to live info.
			ReCastPlayerSpell(playerIndex, packet->firstArg, packet->casterX, packet->casterY, packet->targetX, packet->targetY, 0, packet->spellLevel); // The remote player (wall owner) does recast without cast animation.
		}
		else
		{
			msg_errorf("%s has cast an illegal spell.", player.playerName);
		}
	}

	return sizeof(*packet);
}

//For right clicking on Relict to cast Wall-like spells
uint On_TSPELLXYD(const TCmdLocParam3* packet, int playerIndex)
{
	Player& player = Players[playerIndex];
	if (gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon )
	{
		if (IsSpellEnabledHere(packet->firstArg, playerIndex))
		{
			ClearPlayerPath(playerIndex);
			player.destAction = PCMD_26_CAST_NOT_ORIENTED_SPELL;
			player.destParam1 = packet->row;
			player.destParam2 = packet->col;
			player.destParam3 = packet->secondArg;
			player.destParam4 = packet->thirdArg;
			player.spellIndex = (char) packet->firstArg;
			player.readySpellType = player.spellOnCursorType; // TODO: sould pass in packet to avoid spell desync
			player.readySpellTargetType = SO_2_RELIC;
		}
		else
		{
			msg_errorf("%s has cast an illegal spell.", player.playerName);
		}
	}
	return sizeof(*packet);
}

//----- (00448CFC) --------------------------------------------------------
uint On_SPELLXY( const TCmdLocParam2* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon )
    {
		if (IsSpellEnabledHere(packet->firstArg, playerIndex))
        {
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_12_CAST_SPELL_TO_NONE;
            player.destParam1 = packet->row;
            player.destParam2 = packet->col;
            player.destParam3 = packet->secondArg;
            player.spellIndex = (char) packet->firstArg;
            player.readySpellType = player.SpellType; // TODO: sould pass in packet to avoid spell desync
            player.readySpellTargetType = SO_0_SKILL;
        }
        else
        {
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (00448DCF) --------------------------------------------------------
uint On_TSPELLXY( const TCmdLocParam2* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon )
    {
		if (IsSpellEnabledHere(packet->firstArg, playerIndex))
        { // DungeonLevel || Spells[packet->firstArg].IsEnabledInTown ){
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_12_CAST_SPELL_TO_NONE;
            player.destParam1 = packet->row;
            player.destParam2 = packet->col;
            player.destParam3 = packet->secondArg;
            player.spellIndex = (char) packet->firstArg; // TODO: change firstArg to int ?
            player.readySpellType = player.spellOnCursorType; // TODO: sould pass in packet to avoid spell desync
            player.readySpellTargetType = SO_2_RELIC;
        }
        else
        {
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (00448EA2) --------------------------------------------------------
uint On_OPOBJXY( const TCmdLocParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		Object& object = Objects[packet->firstArg];
		MakePlayerPath(playerIndex, packet->row, packet->col, !object.isBlockWalking && !object.isDoor);
		player.destAction = PCMD_13_OBJECT_ACTIVATE;
		player.destParam1 = packet->firstArg;
	}
	return sizeof( *packet );
}

//----- (00448F1D) --------------------------------------------------------
uint On_DISARMXY( const TCmdLocParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	Object& object = Objects[packet->firstArg];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, packet->row, packet->col, !object.isBlockWalking && !object.isDoor);
		player.destAction = PCMD_14_OBJECT_SAFE_ACTIVATE;
		player.destParam1 = packet->firstArg;
	}
	return sizeof( *packet );
}

//----- (00448F98) --------------------------------------------------------
uint On_OPOBJT( const TCmdParam1* packet, int playerIndex )
{
	int objectOnMapIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		player.destAction = PCMD_18_ACTIVATE_OBJECT_WITH_TELEKINES;
		player.destParam1 = objectOnMapIndex;
	}
	return sizeof( *packet );
}

//----- (00448FD8) --------------------------------------------------------
uint On_ATTACKID( const TCmdParam1* packet, int playerIndex )
{
	int monsterIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		if( abs(player.Row - monster.NextRow) > 1 || abs(player.Col - monster.NextCol) > 1 ){
			MakePlayerPath(playerIndex, monster.NextRow, monster.NextCol, 0);
		}
		player.destAction = PCMD_20_MELEE_ATTACK_TO_MONSTER;
		player.destParam1 = monsterIndex;
	}
	return sizeof( *packet );
}

//----- (00449095) --------------------------------------------------------
uint On_ATTACKPID( const TCmdParam1* packet, int playerIndex )
{
	int targetPlayerIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	Player& playerTarger = Players[targetPlayerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, playerTarger.NextRow, playerTarger.NextCol, 0);
		player.destAction = PCMD_21_MELEE_ATTACK_TO_PLAYER;
		player.destParam1 = targetPlayerIndex;
	}
	return sizeof( *packet );
}

//----- (004490FD) --------------------------------------------------------
uint On_RATTACKID( const TCmdParam1* packet, int playerIndex )
{
	int monsterIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		ClearPlayerPath(playerIndex);
		player.destAction = PCMD_22_RANGE_ATTACK_TO_MONSTER;
		player.destParam1 = monsterIndex;
	}
	return sizeof( *packet );
}

//----- (0044914D) --------------------------------------------------------
uint On_RATTACKPID( const TCmdParam1* packet, int playerIndex )
{
	int monsterIndex = packet->firstArg;
	int targetPlayerIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		ClearPlayerPath(playerIndex);
		player.destAction = PCMD_23_RANGE_ATTACK_TO_PLAYER;
		player.destParam1 = targetPlayerIndex;
	}
	return sizeof( *packet );
}

//----- (0044919D) --------------------------------------------------------
uint On_SPELLID( const TCmdParam3* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
        ushort spellIndex = packet->secondArg;
		if (IsSpellEnabledHere(spellIndex, playerIndex)) {
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_24_CAST_SPELL_TO_MONSTER;
            player.destParam1 = packet->firstArg; // targetIndex
            player.destParam2 = packet->thirdArg; // slvl
            player.spellIndex = (char) spellIndex;
            player.readySpellType = player.SpellType;
            player.readySpellTargetType = SO_0_SKILL;
        }else{
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (00449262) --------------------------------------------------------
uint On_SPELLPID( const TCmdParam3* packet, int playerIndex )
{
	short spellIndex = packet->secondArg;

	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		if (IsSpellEnabledHere(spellIndex, playerIndex)) {
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_25_CAST_SPELL_TO_PLAYER;
            player.destParam1 = packet->firstArg; // targetIndex
            player.destParam2 = packet->thirdArg; // slvl
            player.spellIndex = (char) spellIndex;
            player.readySpellType = player.SpellType;
            player.readySpellTargetType = SO_0_SKILL;
        }else{
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (00449327) --------------------------------------------------------
uint On_TSPELLID( const TCmdParam3* packet, int playerIndex )
{
	short spellIndex = packet->secondArg;

	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		if (IsSpellEnabledHere(spellIndex, playerIndex)) {
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_24_CAST_SPELL_TO_MONSTER;
            player.destParam1 = packet->firstArg; // targetIndex
            player.destParam2 = packet->thirdArg; // slvl
            player.spellIndex = (char) spellIndex;
            player.readySpellType = player.spellOnCursorType; // TODO: sould pass in packet to avoid spell desync
            player.readySpellTargetType = SO_2_RELIC;
        }else{
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (004493EC) --------------------------------------------------------
uint On_TSPELLPID( const TCmdParam3* packet, int playerIndex )
{
	short spellIndex = packet->secondArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		if (IsSpellEnabledHere(spellIndex, playerIndex)) {
            ClearPlayerPath( playerIndex );
            player.destAction = PCMD_25_CAST_SPELL_TO_PLAYER;
            player.destParam1 = packet->firstArg; // targetIndex
            player.destParam2 = packet->thirdArg; // slvl
            player.spellIndex = (char) spellIndex;
            player.readySpellType = player.spellOnCursorType; // TODO: sould pass in packet to avoid spell desync
            player.readySpellTargetType = SO_2_RELIC;
        }else{
            msg_errorf( "%s has cast an illegal spell.", player.playerName );
        }
    }
    return sizeof( *packet );
}

//----- (004494B1) --------------------------------------------------------
uint On_KNOCKBACK( const TCmdParam2* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
	    int monsterIndex = packet->firstArg;
		int range = packet->secondArg;
		KnockbackMonster(monsterIndex, OrientationToTarget( Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, Players[playerIndex].Row, Players[playerIndex].Col ), range );
		DamageMonsterByPlayer(monsterIndex, playerIndex, 0);
	}
	return sizeof( *packet );
}

//----- (004494F7) --------------------------------------------------------
uint On_RESURRECT( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		RisingPlayer(playerIndex, packet->firstArg);
        check_update_plr( playerIndex );
	}
	return sizeof( *packet );
}

//----- (00449527) --------------------------------------------------------
uint On_HEALOTHER( const TCmdParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		HealOther(playerIndex, packet->firstArg);
	}
	return sizeof( *packet );
}

//----- (0044955E) --------------------------------------------------------
uint On_TALKXY( const TCmdLocParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon ){
		MakePlayerPath(playerIndex, packet->row, packet->col, 0);
		player.destAction = PCMD_17_TOWNER_DIALOG;
		player.destParam1 = packet->firstArg;
	}
	return sizeof( *packet );
}

//----- (004495BB) --------------------------------------------------------
uint On_NEWLVL( const TCmdParam2* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
		mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex != CurrentPlayerIndex ){
		StartNewLvl(playerIndex, packet->firstArg, (DUNGEON)packet->secondArg);
		// Wall recast after taking stairs.
		Player& player = Players[playerIndex];
		Player& MyPlayer = Players[CurrentPlayerIndex];
		if (Dungeon == player.dungeon && Dungeon != DUN_0_TOWN) {
			static short playerWallType[WallTypeCount] = { PS_6_FIRE_WALL, PS_19_FLAME_RING, PS_40_LIGHTING_WALL, PS_17_LIGHTING_RING };
			bool recastWall[WallTypeCount] = {}; // Set to 1 if we are going to recast this type of wall.
			int TimeToLive[WallTypeCount] = {}; // Store wall's remaining time.
			std::fill(std::begin(TimeToLive), std::end(TimeToLive), std::numeric_limits<int>::max()); // Initialize to max.
			for (int spellIndexIndex = 0; spellIndexIndex < MissileAmount; ++spellIndexIndex) { // May break out loop earlier if all 2/4 types are found...
				Missile& missile = Missiles[MissileIndexes[spellIndexIndex]];
				if (SpellIdxToWallType.find(missile.spellIndex) != SpellIdxToWallType.end()) { // Found wall spell and set parameters.
					uchar wallType = SpellIdxToWallType[missile.spellIndex];
					recastWall[wallType] = true;
					TimeToLive[wallType] = (missile.TimeToLive < TimeToLive[wallType]) ? missile.TimeToLive : TimeToLive[wallType]; // Look for the min TTL to reduce the unwanted effect of map loading time.
				}
			}
			for (int i = 0; i < WallTypeCount; ++i) {
				if (recastWall[i]) {
					SendCmdWallRecast(1, CMD_109_WallRecast, playerWallType[i], playerIndex, TimeToLive[i], PlayerWall[CurrentPlayerIndex][i][0], PlayerWall[CurrentPlayerIndex][i][1], PlayerWall[CurrentPlayerIndex][i][2], PlayerWall[CurrentPlayerIndex][i][3], PlayerWall[CurrentPlayerIndex][i][4], PlayerWall[CurrentPlayerIndex][i][5]); // Packet's 1st arg = spell type, 2nd arg = index of player who just enters my current dungeon.
				}
			}
		}
	}
	return sizeof( *packet );
}

//----- (004495EF) --------------------------------------------------------
uint On_WARP( const TCmdParam1* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	Player& MyPlayer = Players[CurrentPlayerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
	    int playerTargetIndex = packet->firstArg;
		StartWarpDungeon(playerIndex, playerTargetIndex);
		// При телепорте пихаем вещь с курсора в инвентарь
		if( playerIndex == CurrentPlayerIndex && Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
			#if 0 // TODO: continue here, find out why changed
			Items[MaxItems_255] = player.ItemOnCursor;
			AutoGetItem(CurrentPlayerIndex, MaxItems_255);
			#else
			AutoPutCursorToInventoryOrDrop();
			#endif
		}
		// Wall recast after taking TP.
		if (playerIndex == CurrentPlayerIndex) {
			StartNewLvl(playerIndex, WM_1030_TOWN_PORTAL, Dungeon.next); // Must need this for recast to work, watch out for potential bugs...  In original code, case WM_1030_TOWN_PORTAL was never triggered!
		}
		if (playerIndex != CurrentPlayerIndex && Dungeon == player.dungeon && Dungeon != DUN_0_TOWN) {
			static short playerWallType[WallTypeCount] = { PS_6_FIRE_WALL, PS_19_FLAME_RING, PS_40_LIGHTING_WALL, PS_17_LIGHTING_RING };
			bool recastWall[WallTypeCount] = {}; // Set to 1 if we are going to recast this type of wall.
			int TimeToLive[WallTypeCount] = {}; // Store wall's remaining time.
			std::fill(std::begin(TimeToLive), std::end(TimeToLive), std::numeric_limits<int>::max()); // Initialize to max.
			for (int spellIndexIndex = 0; spellIndexIndex < MissileAmount; ++spellIndexIndex) { // May break out loop earlier if all 2/4 types are found...
				Missile& missile = Missiles[MissileIndexes[spellIndexIndex]];
				if (SpellIdxToWallType.find(missile.spellIndex) != SpellIdxToWallType.end()) { // Found wall spell and set parameters.
					uchar wallType = SpellIdxToWallType[missile.spellIndex];
					recastWall[wallType] = true;
					TimeToLive[wallType] = (missile.TimeToLive < TimeToLive[wallType]) ? missile.TimeToLive : TimeToLive[wallType]; // Look for the min TTL to reduce the unwanted effect of map loading time.
				}
			}
			for (int i = 0; i < WallTypeCount; ++i) {
				if (recastWall[i]) {
					SendCmdWallRecast(1, CMD_109_WallRecast, playerWallType[i], playerIndex, TimeToLive[i], PlayerWall[CurrentPlayerIndex][i][0], PlayerWall[CurrentPlayerIndex][i][1], PlayerWall[CurrentPlayerIndex][i][2], PlayerWall[CurrentPlayerIndex][i][3], PlayerWall[CurrentPlayerIndex][i][4], PlayerWall[CurrentPlayerIndex][i][5]); // Packet's 1st arg = spell type, 2nd arg = index of player who just enters my current dungeon.
				}
			}
		}
	}
	return sizeof( *packet );
}

//----- (00449655) --------------------------------------------------------
uint On_MONSTDEATH( const TCmdLocParam2* packet, int playerIndex )
{
	const Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex != CurrentPlayerIndex ){
		// Call monster death animation and item drop only if both players are in the same Dlvl AND quest level.  Otherwise, only mask the monster as dead.
		if( (DUNGEON) Dungeon == packet->secondArg ){
			Monster& monster = Monsters[packet->firstArg];
			monster.acidDot = 0; // Fix for acid-related invincible monsters in MP, e.g. Hydramancer
			MonsterKilledByOtherPlayer(packet->firstArg, packet->row, packet->col, playerIndex);
		}
		delta_kill_monster(packet->firstArg, packet->row, packet->col, (DUNGEON)packet->secondArg);
	}
	return sizeof( *packet );
}

//----- (004496C7) --------------------------------------------------------
uint On_KILLGOLEM( const TCmdLocParam2* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex != CurrentPlayerIndex ){
		if( (DUNGEON)Dungeon == packet->secondArg ){
			MonsterKilledByOtherPlayer(packet->firstArg, packet->row, packet->col, playerIndex);
		}
		delta_kill_monster(packet->firstArg, packet->row, packet->col, (DUNGEON)packet->secondArg);
	}
	return sizeof( *packet );
}

//----- (00449731) --------------------------------------------------------
uint On_AWAKEGOLEM( const TCmdAwakeSummon* packet, int playerIndex )
{
    if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
    }
    else{
        delta_sync_summon( packet );
		if( packet->dungeon == Dungeon ){
            AwakeSummon(packet->summonIndex, packet->row, packet->col, packet->params.summonLife, packet->params, false);
        }
    }
    return sizeof( *packet );
}

//----- (004497E5) --------------------------------------------------------
uint On_MONSTDAMAGE( const TCmdDamageMonster* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
    if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
    }
	else if( playerIndex != CurrentPlayerIndex && Dungeon == player.dungeon ){
        Monster& monster = Monsters[packet->targetMonsterIndex];
        monster.Attackers |= 1 << playerIndex;
    
        int curLife = monster.CurrentLife;
        if( curLife >= 0 ){
            monster.CurrentLife -= packet->damage;
            if( ( monster.CurrentLife & ~63 ) < 64 ){
                monster.CurrentLife = 64;
            }
			delta_monster_hp(packet->targetMonsterIndex, monster.CurrentLife, player.dungeon);
        }
    }
    return sizeof( *packet );
}

//Used for syncing monster.acidDot
//----- (th2) -------------------------------------------------------------
uint On_MONSTACIDDOT(const TCmdAcidDotMonster* packet, int playerIndex)
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
		mega_add_data(playerIndex, packet, sizeof(*packet));
	} else if (playerIndex != CurrentPlayerIndex && Dungeon == packet->dungeon) { // player.dungeon is 0 the moment when caster goes back to town, will sync if the acid attacker hits a monster and at the same time goes to a different dungeon.
		Monster& monster = Monsters[packet->targetMonsterIndex];
		monster.Attackers |= 1 << playerIndex;
		monster.dotLastAttacker = packet->dotLastAttacker;
		bool monsterAlive = (monster.CurrentLife & ~63) > 0 && monster.CurAction != A_6_DEATH;
		if (monsterAlive && monster.acidDot == 0) { // If remote player enters the Dlvl, he should get the total amount of monster.acidDot sync'd, instead of 0.
			monster.acidDot = packet->MonAcidDot;
		} else if (monsterAlive) { // Remote player will never see a negative monster.acidDot (full green bar), especially on Dlvl re-entry.
			monster.acidDot = std::max(monster.acidDot + packet->DeltaMonAcidDot, 0);
		} else if (!monsterAlive) { // Additional safety measure to make sure dying monsters have acid dot removed for all players.
			monster.acidDot = 0;
		}
	}
	return sizeof(*packet);
}

//----- (th2) -------------------------------------------------------------
uint On_PVP(const TCmdPvP* packet, int playerIndex)
{
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
		mega_add_data(playerIndex, packet, sizeof(*packet));
	}else if( playerIndex != CurrentPlayerIndex ){
		player.IsPlayerEnemy = packet->IsPlayerEnemy;
	}
	return sizeof(*packet);
}

//----- (004498A6) --------------------------------------------------------
uint On_PLRDEAD( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex == CurrentPlayerIndex ){
        check_update_plr( playerIndex );
	}else{
		StartPlayerKill(playerIndex, packet->firstArg);
	}
	return sizeof( *packet );
}

//----- (004498DE) --------------------------------------------------------
uint On_PLRDAMAGE( const TCmdDamage* packet, int playerIndex )
{
	Player& player = Players[playerIndex];
	Player& attackedPlayer = Players[CurrentPlayerIndex];
	int damage = packet->damage;
    
	if( packet->targetPlayerIndex == CurrentPlayerIndex && Dungeon != DUN_0_TOWN && gbBufferMsgs != BUFFER_ON && Dungeon == player.dungeon
	 && damage <= 192000 && (attackedPlayer.CurLife & (-64)) > 0){
        attackedPlayer.CurLife -= damage;
        attackedPlayer.BaseLife -= damage;
        if( attackedPlayer.CurLife > attackedPlayer.MaxCurLife )
        {
            attackedPlayer.CurLife = attackedPlayer.MaxCurLife;
            attackedPlayer.BaseLife = attackedPlayer.MaxBaseLife;
        }
        if( ( attackedPlayer.CurLife & ( -64 ) ) <= 0 )
        {
            TryToDie( CurrentPlayerIndex, 1 );
        }
    }
    return sizeof( *packet );
}

//----- (004499F3) --------------------------------------------------------
uint On_OPENDOOR( const TCmdParam1* packet, int playerIndex )
{
	int objectOnMapIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( Dungeon == player.dungeon ){
            SyncOpObject( playerIndex, CMD_43_OPENDOOR, objectOnMapIndex );
		}
		delta_sync_object(objectOnMapIndex, CMD_43_OPENDOOR, player.dungeon);
	}
	return sizeof( *packet );
}

//----- (00449A79) --------------------------------------------------------
uint On_CLOSEDOOR( const TCmdParam1* packet, int playerIndex )
{
	int objectOnMapIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( Dungeon == player.dungeon ){
            SyncOpObject( playerIndex, CMD_44_CLOSEDOOR, objectOnMapIndex );
		}
		delta_sync_object(objectOnMapIndex, CMD_44_CLOSEDOOR, player.dungeon);
	}
	return sizeof( *packet );
}

//----- (00449ADA) --------------------------------------------------------
uint On_OPERATEOBJ( const TCmdParam1* packet, int playerIndex )
{
	int objectOnMapIndex = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( Dungeon == player.dungeon ){
            SyncOpObject( playerIndex, CMD_45_OPERATEOBJ, objectOnMapIndex );
		}
		delta_sync_object(objectOnMapIndex, CMD_45_OPERATEOBJ, player.dungeon);
	}
	return sizeof( *packet );
}

//----- (00449B3B) --------------------------------------------------------
uint On_PLROPOBJ( const TCmdParam2* packet, int playerIndex )
{
	ushort targetPlayerIndex = packet->firstArg;
	ushort objectIndex = packet->secondArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( Dungeon == player.dungeon ){
            SyncOpObject( targetPlayerIndex, CMD_46_PLROPOBJ, objectIndex );
		}
		delta_sync_object(objectIndex, CMD_46_PLROPOBJ, player.dungeon);
	}
	return sizeof( *packet );
}

//----- (00449B9C) --------------------------------------------------------
uint On_BREAKOBJ( const TCmdParam2* packet, int playerIndex )
{
	ushort targetPlayerIndex = packet->firstArg;
	ushort objectIndex = packet->secondArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( Dungeon == player.dungeon ){
            SyncBreakObj( targetPlayerIndex, objectIndex );
		}
		delta_sync_object(objectIndex, CMD_47_BREAKOBJ, player.dungeon);
	}
	return sizeof( *packet );
}

//----- (00449BF9) --------------------------------------------------------
uint On_CHANGEPLRITEMS( const TCmdChItem* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex != CurrentPlayerIndex ){
		EquipItemFromNetwork(playerIndex, packet->onBodyIndex, packet->info);
	}
	return sizeof( *packet );
}

//----- (00449C39) --------------------------------------------------------
uint On_DELPLRITEMS( const TCmdDelItem* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( playerIndex != CurrentPlayerIndex ){
		DestroyOnBodyItem(playerIndex, packet->onBodyIndex);
	}
	return sizeof( *packet );
}

//----- (00449C67) --------------------------------------------------------
uint On_PLRLEVEL( const TCmdParam1* packet, int playerIndex )
{
	int charLevel = packet->firstArg;
	Player& player = Players[playerIndex];
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( charLevel <= PlayerMaxLevel() && playerIndex != CurrentPlayerIndex ){
		player.CharLevel = charLevel;
	}
	return sizeof( *packet );
}

//----- (00449CA8) --------------------------------------------------------
uint On_DROPITEM( const TCmdPItem* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_1_ON ){
		mega_add_data(playerIndex, packet, sizeof(*packet));
	}else{
		delta_put_item(packet, packet->mapRow, packet->mapCol, Players[playerIndex].dungeon);
	}
	return sizeof( *packet );
}

//----- (00449CE3) --------------------------------------------------------
uint On_SEND_PLRINFO( const TCmdPlrInfoHdr* packet, int playerIndex)
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, packet->wBytes + sizeof(*packet) );
	}else{
        recv_plrinfo( playerIndex, packet, packet->bCmd == CMD_2_ACK_PLRINFO );
	}
	return packet->wBytes + sizeof(*packet);
}

//---- (00449D1F) ---------------------------------------------------------
uint On_ACK_PLRINFO( const TCmdPlrInfoHdr* packet, int playerIndex )
{
	return On_SEND_PLRINFO( packet, playerIndex );
}

//----- (00449D24) --------------------------------------------------------
uint On_PLAYER_JOINLEVEL( const TCmdLocParam1* packet, int playerIndex )
{
	// Initialization, so PvP is correctly processed after player rejoins the game.
	Players[CurrentPlayerIndex].IsPlayerEnemy = !PlayerFriendly();
	SendCmdPvP(0, CurrentPlayerIndex, Players[CurrentPlayerIndex].IsPlayerEnemy);
  
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
    }
    else{
        Player& player = Players[playerIndex];
        player.lvlChanging = 0;
        if( player.playerName[0] ){
            if( !player.IsExists ){
                player.IsExists = 1;
                NetPlayerCount++;
                AddNewPlayerStatusFormattedMessage( "Player '%s' (level %d) just joined The Game", &player.playerName, player.CharLevel );
            }
        }
        if( player.IsExists && CurrentPlayerIndex != playerIndex ){
            player.Row = packet->row;
            player.Col = packet->col;
            player.dungeon = (DUNGEON)packet->firstArg;
            player.LoadPlayerAnimationMask = PAF_0_NO;
			if( Dungeon == player.dungeon ){
                LoadPlayerGFX( playerIndex, PAF_1_STAY );
                SyncInitPlr( playerIndex );
                if( ( player.CurLife & ~63 ) <= 0 ){
                    player.Appearance = 0;
                    LoadPlayerGFX( playerIndex, PAF_8_DEATH );
                    player.CurAction = PCA_8_DEATH;
                    NewPlayerAnimation( playerIndex, player.deathAnimationsPointers[0], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath );
                    player.currentAnimationFrame = player.currentAnimationFramesCount - 1;
                    player.currentActionFrame = 2 * player.currentAnimationFramesCount;
                    FlagMap[player.Row][player.Col] |= CF_4_DEAD_PLAYER;
                }
                else{
                    StartStand( playerIndex, 0 );
                }
                player.lightRadiusIdentificator = AddVision( player.Row, player.Col, player.LightRadius, playerIndex == CurrentPlayerIndex );
                player.LightIndex = -1;
            }
        }
    }
    return sizeof( *packet );
}

//----- (00449EFC) --------------------------------------------------------
uint On_ACTIVATEPORTAL( const TCmdLocParam3* packet, int playerIndex )
{
	char row = packet->row;
	char col = packet->col;
	short dlvl = packet->firstArg;
	short dungeonType = packet->secondArg;
	bool isQuestFloor = packet->thirdArg;
    
    if( gbBufferMsgs == BUFFER_ON )
    {
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
    }
    else
    {
        ActivatePortal( playerIndex, row, col, (DUNGEON)dlvl );
        if( playerIndex == CurrentPlayerIndex )
        {
            delta_open_portal( playerIndex, row, col, (DUNGEON)dlvl );
        }
        else if( Dungeon == DUN_0_TOWN )
        {
            AddinTownPortal( playerIndex );
        }
		//else if( DungeonLevel == Players[playerIndex].DungeonLevel && QuestFloorIndex == Players[playerIndex].QuestFloorIndex) // P1 and P2 are both on Dlvl 2, P1 opens TP and goes inside PWS, P2 will no longer see P1's TP on Dlvl 2, as it should.
		else if( Dungeon == Players[playerIndex].dungeon && Dungeon->isQuest == isQuestFloor) // player.QuestFloorIndex is only updated after the player fully loaded, but we need an instantaneous update via player.IsQuestFloor.  isQuestFloor works well enough here, as it's the caster's IsQuestFloor.
        {
            bool thereIsNoTownPortalForPlayer = true;
            for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ )
            {
                int spellIndex = MissileIndexes[spellIndexIndex];
                Missile& spell = Missiles[spellIndex];
                if( spell.BaseMissileIndex == MI_10_TOWN_PORTAL && spell.CasterIndex == playerIndex )
                {
                    thereIsNoTownPortalForPlayer = false;
                }
            }
            if( thereIsNoTownPortalForPlayer )
            {
                AddWarpMissile( playerIndex, row, col );
            }
        }
        else
        {
            ClearAllTownPortalsOwnedThisPlayer( playerIndex );
        }
    }
    return sizeof( *packet );
}

//----- (00449FDB) --------------------------------------------------------
void delta_open_portal( int playerIndex, short row, short col, DUNGEON dungeon )
{
	sgbDeltaChanged = true;
	DPortal& portalInfo = sgJunk.portals[playerIndex];
	portalInfo.row = (char) row;
	portalInfo.col = (char) col;
	portalInfo.dungeon = dungeon;
}

//----- (0044A016) --------------------------------------------------------
uint On_DEACTIVATEPORTAL( const TCmd* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( PortalOnLevel(playerIndex) ){
			ClearAllTownPortalsOwnedThisPlayer(playerIndex);
		}
		DeactivatePortal(playerIndex);
        DeltaClosePortal(playerIndex);
	}
	return sizeof( *packet );
}

//----- (0044A054) --------------------------------------------------------
uint On_RETOWN( const TCmd* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( playerIndex == CurrentPlayerIndex ){
			IsPlayerDead = 0;
			ResetMainMenu();
		}
		RestartInTown(playerIndex);
	}
	return sizeof( *packet );
}

//----- (0044A08D) --------------------------------------------------------
uint On_SETSTR( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( packet->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerStrength(playerIndex, packet->firstArg);
		}
	}
	return sizeof( *packet );
}

//----- (0044A0FF) --------------------------------------------------------
uint On_SETMAG( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( packet->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerMagic(playerIndex, packet->firstArg);
		}
	}
	return sizeof( *packet );
}

//----- (0044A0C6) --------------------------------------------------------
uint On_SETDEX( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( packet->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerDexterity(playerIndex, packet->firstArg);
		}
	}
	return sizeof( *packet );
}

//----- (0044A138) --------------------------------------------------------
uint On_SETVIT( const TCmdParam1* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else if( packet->firstArg <= 750 ){
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerVitality(playerIndex, packet->firstArg);
		}
	}
	return sizeof( *packet );
}

//----- (0044A171) --------------------------------------------------------
uint On_STRING( const TCmdString* packet, int playerIndex )
{
	size_t len = strlen(packet->str);
	if( gbBufferMsgs == BUFFER_OFF ){
		AddNewPlayerDescriptionSatusMessage(playerIndex, packet->str);
	}
	return len + 2;
}

//----- (0044A1A6) --------------------------------------------------------
uint On_SYNCQUEST( const TCmdQuest* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( playerIndex != CurrentPlayerIndex ){
			UpdateQuestStatus(packet->questIndex, packet->status, packet->talkToFlag, packet->status2, packet->status3, packet->status4, packet->status5);
		}
		sgbDeltaChanged = true;
	}
	return sizeof( *packet );
}

//----- (0044A1E8) --------------------------------------------------------
uint On_ENDSHIELD( const TCmd* packet, int playerIndex )
{
	if( (gbBufferMsgs != BUFFER_ON || playerIndex != CurrentPlayerIndex) && Dungeon == Players[playerIndex].dungeon )
    {
        for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
            int spellIndex = MissileIndexes[spellIndexIndex];
            Missile& spell = Missiles[spellIndex];
            if( spell.BaseMissileIndex == MI_13_MANA_SHIELD && spell.CasterIndex == playerIndex ){
                ClearMissileCell( spellIndex );
                RemoveMissile( spellIndex, spellIndexIndex );
            }
        }
    }
    return sizeof( *packet );
}

//----- (0044A26F) --------------------------------------------------------
uint On_DESTROY_REFLECT( const TCmd* packet, int playerIndex )
{
	if( (gbBufferMsgs != BUFFER_ON || playerIndex != CurrentPlayerIndex) && Dungeon == Players[playerIndex].dungeon )
    {
		Players[playerIndex].CountOfReflectCharges = 0;//This should fix Reflect charge desync after effect is worn off
        for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ )
        {
            int spellIndex = MissileIndexes[spellIndexIndex];
            Missile& spell = Missiles[spellIndex];
            if( spell.BaseMissileIndex == MI_79_REFLECT && spell.CasterIndex == playerIndex )
            {
                ClearMissileCell( spellIndex );
                RemoveMissile( spellIndex, spellIndexIndex );
            }
        }
    }
    return sizeof( *packet );
}

//----- (0044A2F6) --------------------------------------------------------
uint On_DEBUG( const TCmd* packet, int playerIndex )
{
	return sizeof( *packet );
}

//----- (0044A2FA) --------------------------------------------------------
uint On_OPEN_WEAK_UBER( const TCmd* packet, int playerIndex )
{
	if( gbBufferMsgs != BUFFER_ON ){
		OpenUberDiabloRoom();
		IsUberRoomOpened = 1;
//		Quests[Q_21_FLESHDOOM].status = QS_3_COMPLETE;
		OpenWeakUber();
	}
	return sizeof( *packet );
}

uint On_SETPERK( const TCmdParam2* packet, int playerIndex )
{
	if( gbBufferMsgs == BUFFER_ON ){
        mega_add_data( playerIndex, packet, sizeof( *packet ) );
	}else{
		if( playerIndex != CurrentPlayerIndex ){
			SetPlayerPerk(playerIndex, packet->firstArg, packet->secondArg);
		}
	}
	return sizeof( *packet );
}

void CheckNetworkDataIntegrity()
{
    if( MaxCountOfPlayersInGame == 1 ){
        return;
    }
    
	for( int dlvl = 0; dlvl < DungeonCount; ++dlvl ){
        const DLevel& levelData = sgLevels[dlvl];
        for( int i = 0; i < Monsters_Max_Count; ++i ){
            const DMonsterStr& m = levelData.monsters[i];
            if( m.row != INIT_VAL ){
                if( m.row >= FineMap_112
                 || m.col >= FineMap_112
                 || m.orientation >= 8 ){
                    __debugbreak();
                }
            }
        }
        
        for( int i = 0; i < SummonMonsters_Count; ++i ){
            const DSummonStr& m = levelData.summons[i];
            if( m.spriteIndex != INIT_VAL ){
                if( m.spriteIndex >= Monsters_Sprites_Max_Count
                 || m.summonType >= 10 ){
                    __debugbreak();
                }
            }
        }
    }
}
