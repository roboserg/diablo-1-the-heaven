#include "stdafx.h"

char* CurSaveData = 0;// 00697630

uint SaveVersion = -1;

extern void CheckInvalidInvItems(int);

//----- (th2) -------------------------------------------------------------
void LoadItemsActive()
{
	if( SaveVersion < 20 ){
		memzero(ItemActive);
		for( int i = 0; i < MaxItems_255; i++ ){ ItemAvailable[i] = i; }
		for( int i = 0; i < 127 * 2; i++ ){ GetNextByte(); }
		for( int i = 1; i < ItemsCount; i++ ){ ItemActive[i] = MaxItems_255 - i; }
		if( ItemsCount ) ItemAvailable[0] = MaxItems_255 - ItemsCount;
	}else{
		for( int i = 0; i < MaxItems_255; i++ ){ ItemActive[i] = (uchar)GetNextByte(); }
		for( int i = 0; i < MaxItems_255; i++ ){ ItemAvailable[i] = (uchar)GetNextByte(); }
	}
	for( int i = 0; i < ItemsCount; i++ ){ LoadItems(ItemActive[i]); }
}

//----- (th2) -------------------------------------------------------------
void LoadItemMap()
{
	if( SaveVersion < 20 ){
		memzero(ItemMap);
		for( int i = 0; i < 112 * 112; i++ ) GetNextByte();
		for( int i = 0; i < ItemsCount; i++ ){
			int itemIndex = ItemActive[i];
			Item& item = Items[itemIndex];
			ItemMap[ item.MapRow ][ item.MapCol ] = itemIndex + 1;
		}
	}else{
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				ItemMap[ row ][ col ] = (uchar)GetNextByte();
			}
		}
	}
}

char* saveFileData = 0;
//----- (0042CE32) --------------------------------------------------------
void __fastcall LoadGame( int loadFromMainMenu )
{
	extern __int64 oldPlayerAvailableSpellMask; oldPlayerAvailableSpellMask = -1;
	FreeDungeonResources();
	DestroyTempSaves();

	uint loadedSize; 
	/*char**/ saveFileData = (char*)ReadSaveFile(SaveFile_Game, &loadedSize, 1);
	CurSaveData = saveFileData;
	int versionString = GetNextHtonl2();
	SaveVersion = 0;
	if( versionString != 'HELF' ){ // добавить поддержку версионности, для предотвращения морфинга в дальнейшем
		if( versionString == SaveSignature ){
			SaveVersion = GetNextHtonl();
		}else{
			TerminateWithError( "Invalid save signature" );
		}
	}
	#ifndef ALLOW_TH1_CONVERTATION 
	else{
		TerminateWithError( "Invalid version string" );
	}
	#endif
	if( SaveVersion < 24 ){
		GetNextBool(); // was IsQuestFloor
		GetNextHtonl(); // was QuestFloorIndex
		GetNextHtonl(); // was DungeonLevel
		GetNextHtonl(); // was DungeonType
	}
	int playerRow = GetNextHtonl();
	int playerCol = GetNextHtonl();
	IsINVPanelVisible = GetNextBool();
	IsCHARPanelVisible = GetNextBool();
	int monstersCount = GetNextHtonl();
	int itemsOnGroundCount = GetNextHtonl();
	int spellCastAmount = GetNextHtonl();
	int objectsCount = GetNextHtonl();
	for( int levelIndex = 0; levelIndex < (SaveVersion < 24 ? 25 : DungeonExpantionLimit); levelIndex++ ){ // TODO: continue here, add DungeonCount saving
		DungeonRandSeed[levelIndex] = GetNextHtonl2();
		if( SaveVersion < 24 ) GetNextHtonl(); // was DungeonLevelType
	}
	LoadPlayer(CurrentPlayerIndex);

	CheckInvalidInvItems( CurrentPlayerIndex ); // TODO: check if this funcs corrupt items on load
	
	Player& player = Players[CurrentPlayerIndex];

	int activeBuffFlag = player.activeBuffFlag;
	ushort countOfReflectCharges = player.CountOfReflectCharges;

	Difficulty = player.Difficulty;
	LimitToRange(Difficulty, DL_0_HORROR, DL_COUNT - 1);

	char loadQuestCount = Quest_Count;
	if(      SaveVersion < 24 ) loadQuestCount = 51;
	else if( SaveVersion < 27 ) loadQuestCount = 37;
	else loadQuestCount = GetNextByte();
	for( int index = 0; index < loadQuestCount; index++ ){
		LoadQuest(index);
	}
	for( int index = loadQuestCount; index < Quest_Count; index++ ){
		Quests[index].status = QS_0_NO;
	}
	for( int index = 0; index < PlayersMax_4; index++){		
		LoadPortal(index);
	}

	LoadGameLevel(loadFromMainMenu, LVL_4_NODIR);
	SyncInitPlr(CurrentPlayerIndex);
	SyncPlrAnim(CurrentPlayerIndex);

	PlayerRowPos = playerRow;
	MonstersCount = monstersCount;
	ItemsCount = itemsOnGroundCount;
	MissileAmount = spellCastAmount;
	PlayerColPos = playerCol;
	ObjectsCount = objectsCount;

	player.activeBuffFlag = activeBuffFlag;
	player.CountOfReflectCharges = countOfReflectCharges;
	IsFastWalkEnabled = player.runEnabled;

	{
		int* ptr = reinterpret_cast<int*>(TotalSlain);
		for( int i = 0, ie = countof(TotalSlain)/2; i < ie; i++){		
			*ptr++ = GetNextHtonl2();
		}
	}

	if( Dungeon->genType || Dungeon->isQuest ){

		for( int index = 0; index < Monsters_Max_Count;	index++){
			MonsterIndexes[index] = GetNextHtonl();
		}
		for( int index = 0; index < MonstersCount;	index++){	
			LoadMonster(MonsterIndexes[index]);		
		}

		for( int index = 0; index < MissilesMax; index++){
			MissileIndexes[index] = GetNextWord();
		}
		for( int index = 0; index < MissilesMax; index++){
			MissileIndexList[index] = GetNextWord();
		}
		for( int index = 0; index < MissileAmount; index++){			
			LoadMissile(MissileIndexes[index]);		
		}

		for( int index = 0; index < MaxObjects_127; index++){			
			ObjectActive[index] = GetNextByte();		
		}
		for( int index = 0; index < MaxObjects_127; index++){			
			ObjectAvailable[index] = GetNextByte();
		}
		for( int index = 0; index < ObjectsCount; index++){		
			LoadObject(ObjectActive[index]);			
		}

		for( int index = 0; index < ObjectsCount; index++){		
			SetDunTileForObject(ObjectActive[index]);			
		}

		LightsCount = GetNextHtonl();
		for( int index = 0; index < Lights_32_Count; index++){			
			LightIndexes[index] = GetNextByte();		
		}
		for( int index = 0; index < LightsCount; index++ ){			
			LoadObjectLight(LightIndexes[index]);	
		}

		VisionId = GetNextHtonl();
		VisionCount = GetNextHtonl();
		for( int index = 0; index < VisionCount; index++ ){		
			LoadPlayerVisibility(index);		
		}
	}

	LoadItemsActive();

	for( int index = 0; index < 128; index++ ){			
		CurSaveData++; // backward compatibility with now unused AlreadyDroppedUniqs, can be reused
	}

	// в оригинале данные в сэйве пишутся сначала по колонкам, потом по рядам
	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			LightMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			FlagMap[ row ][ col ] = GetNextByte();
		}
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PlayerMap[ row ][ col ] = GetNextByte();
		}
	}

	LoadItemMap();

	if( Dungeon->genType || Dungeon->isQuest ){
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				MonsterMap[ row ][ col ] = GetNextHtonl();
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				DeathMonstersMap[ row ][ col ] = GetNextWord();
			}
		}
		
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				BloodMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				ObjectsMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				LightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				StaticLightMap[ row ][ col ] = GetNextByte();
			}
		}

		for( int col = 0; col < Map_40; col++ ){
			for( int row = 0; row < Map_40; row++ ){
				AutoMap[ row ][ col ] = GetNextBool();
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				MissilesMap[ row ][ col ] = GetNextWord();
			}
		}
	}
	GriswoldMagicItemCount = GetNextHtonl();
	GriswoldItemsLevelParam = GetNextHtonl();
	for( int index = 0; index < PT_MagicItemsCount; index++ ){
		LoadMagicStoreItem(index);
	}
	VisualTrade_RefreshPage(VTP_BlacksmithPremium);
	AutomapMode = GetNextByte();
	AutomapZoomFactor = GetNextHtonl();
	extern int MinimapZoom;
	if( AutomapMode == 2 ) MinimapZoom = AutomapZoomFactor;
	void SetAutoMapZoom();
	SetAutoMapZoom();

	FreeMem(saveFileData);
	CurSaveData = 0;
	SyncAutomap();
	ResyncQuests();
	if( Dungeon->genType || Dungeon->isQuest ){
		ProcessLightList();
	}
	ChangeAllPlayersVisibility();
	ProcessVisionList();
	SetupMissilesAnimationsAfterLoading();
	RestoreScreenSurface();
	SetCursorGraphics(CM_1_NORMAL_HAND);
	BoolWithPlayer = 1;

	ItemUpdateTimer = 1;
	ValidateMonsters();
    ValidateCurPlayer( true );
}

//----- (0042D384) --------------------------------------------------------
char GetNextByte()
{
	return *CurSaveData++;
}

ushort GetNextWord()
{
	uchar out[2];
	out[1] = *CurSaveData++;
	out[0] = *CurSaveData++;
	return *(ushort*)out;
}

//----- (0042D392) --------------------------------------------------------
uint GetNextHtonl()
{
	uchar out[4];
	out[3] = *CurSaveData++;
	out[2] = *CurSaveData++;
	out[1] = *CurSaveData++;
	out[0] = *CurSaveData++;
	return *(uint*)out;
}

//----- (0042D3CE) --------------------------------------------------------
int GetNextHtonl2()
{
	char out[4];
	out[3] = *CurSaveData++;
	out[2] = *CurSaveData++;
	out[1] = *CurSaveData++;
	out[0] = *CurSaveData++;
	return *(int*)out;
}

//----- (0042D40A) --------------------------------------------------------
bool GetNextBool()
{
	
	return *CurSaveData++ ? true : false;
}

//----- (th2) -------------------------------------------------------------
__forceinline void ReadSaveData ( void* dst, size_t size ){ memcpy( dst, CurSaveData, size ); CurSaveData += size; }
__forceinline void WriteSaveData( void* src, size_t size ){	memcpy( CurSaveData, src, size ); CurSaveData += size; }

//----- (th2) -------------------------------------------------------------
void LoadItemFull( Item* item )
{
	Item& i = *item;
	I_v25& i25 = *(I_v25*)CurSaveData;
	I_v25 item_v25;
	
	static_assert( sizeof(I_v25) == StructSize<Item>(25), "I_v25 size and Item::versions[25].size are not the same");
	static_assert( offsetof(Item, Item::saveVersion) == offsetof(I_v25, I_v25::saveVersion), "Item.saveVersion offset should be 44");
	static_assert( offsetof(Item, Item::ItemCode) == offsetof(I_v25, I_v25::ItemCode), "Item.ItemCode offset should be 8");
	ushort itemSaveVersion = i25.saveVersion;
	if( itemSaveVersion == 0 || itemSaveVersion > CurSaveVersion ) itemSaveVersion = SaveVersion;
	
	// TODO: check all item-> using and change to i25 if need
	size_t savedItemSize = itemSaveVersion > 0 && itemSaveVersion <= CurSaveVersion ? StructSize<Item>(itemSaveVersion) : StructSize<Item>(SaveVersion);
	
	char* dst     = itemSaveVersion < 26 ? (char*) &item_v25 : (char*) item;
	uint  dstSize = itemSaveVersion < 26 ? sizeof(I_v25)  : sizeof(Item);

	ReadSaveData( dst, savedItemSize );
	if( savedItemSize < dstSize ){
		memset( dst + savedItemSize, 0, dstSize - savedItemSize );
	}
	if( itemSaveVersion < 22 ){
		item_v25.RequiredStrength  = i25.unused_oldReqStat[0];
		item_v25.RequiredMagic     = i25.unused_oldReqStat[1];
		item_v25.RequiredDexterity = i25.unused_oldReqStat[2];
		item_v25.RequiredVitality  = i25.unused_oldReqStat[3];
		item_v25.quality = i25.dropType & 63;
		item_v25.dropType &= ~63;
		if( (item_v25.dropType & D_OLD_MAGIC_FIND) == D_OLD_MAGIC_FIND ){
			item_v25.magicFindSeed = i25.seed & 511;
			if( i25.seed & 512 ) item_v25.magicFindSeed = -item_v25.magicFindSeed;
		}else{
			item_v25.magicFindSeed = 0;
		}
	}
	if( itemSaveVersion < 25 ){
		if( (item_v25.dropType & D_OLD_USEFUL ) == D_OLD_USEFUL  ){ item_v25.dropType &= ~D_OLD_USEFUL;  item_v25.dropType |=  D_USEFUL;  }
		if( (item_v25.dropType & D_OLD_RARE   ) == D_OLD_RARE    ){ item_v25.dropType &= ~D_OLD_RARE;    item_v25.dropType |=  D_RARE;    }
		if( (item_v25.dropType & D_OLD_ENCHANT) == D_OLD_ENCHANT ){ item_v25.dropType &= ~D_OLD_ENCHANT; item_v25.dropType |=  D_ENCHANT | D_ENCHANT_V1; }
		if( item_v25.MagicLevel == 4 /*ML_4_ENCHANT_OLD*/ ) item_v25.MagicLevel = ML_0_USUAL;
	}
	if( itemSaveVersion < 26 ){
		i.seed                      = item_v25.seed;
		i.dropType                  = item_v25.dropType;
		i.ItemCode      = (ITEM_CODE) item_v25.ItemCode;
		i.MapRow                    = item_v25.MapRow;
		i.MapCol                    = item_v25.MapCol;
		i.ItemCELFilePtr      = 0; // item_v25.ItemCELFilePtr;
		i.FramesCount               = item_v25.FramesCount;
		i.FrameIndex                = item_v25.FrameIndex;
		i.AnimWidth                 = item_v25.AnimWidth;
		i.subOffsetX                = item_v25.subOffsetX;
		i.animPhase                 = item_v25.animPhase;
		i.EquippedLocation          = item_v25.EquippedLocation;
		i.MagicLevel                = item_v25.MagicLevel;
		i.MagicCode    = (MAGIC_CODE) item_v25.MagicCode;
		i.TypeID                    = item_v25.TypeID;
		i.RequiredLevel             = item_v25.RequiredLevel;
		i.GraphicValue              = item_v25.GraphicValue;
		i.MinDamage                 = item_v25.MinDamage;
		i.MaxDamage                 = item_v25.MaxDamage;
		i.saveVersion               = item_v25.saveVersion;
		i.genVersion                = item_v25.genVersion;
		i.ArmorClass                = item_v25.ArmorClass;
		i.overTime                  = item_v25.overTime;
		i.damageFromEnemies         = item_v25.damageFromEnemies;
		i.minMeleeAbsorbPercent     = item_v25.minMeleeAbsorbPercent;
		i.maxMeleeAbsorbPercent     = item_v25.maxMeleeAbsorbPercent;
		i.minArrowAbsorbPercent     = item_v25.minArrowAbsorbPercent;
		i.maxArrowAbsorbPercent     = item_v25.maxArrowAbsorbPercent;
		i.minThorns                 = item_v25.minThorns;
		i.maxThorns                 = item_v25.maxThorns;
		i.SpellIndex                = item_v25.SpellIndex;
		i.CurCharges                = item_v25.CurCharges;
		i.BaseCharges               = item_v25.BaseCharges;
		i.CurDurability             = item_v25.CurDurability;
		i.BaseDurability            = item_v25.BaseDurability;
		i.quality                   = item_v25.quality;
		i.magicFindSeed             = item_v25.magicFindSeed;
		i.uniqIndexSeed             = item_v25.uniqIndexSeed;
		i.UniqueItemIndex           = item_v25.UniqueItemIndex;
		i.amount                    = item_v25.amount;
		i.Price                     = item_v25.Price;
		i.basePrice                 = item_v25.amount;
		i.basePriceMultiplier       = item_v25.multiplier1 + item_v25.multiplier2;
		i.priceEffect               = item_v25.price1 + item_v25.price2;
		i.gottenFromLand            = item_v25.gottenFromLand;
		i.mirrorImage               = item_v25.mirrorImage;
		i.IsReqMet                  = item_v25.IsReqMet;
		i.isShadow                  = item_v25.isShadow;
		i.throwing                  = item_v25.throwing;
		i.Identified                = item_v25.Identified;
		i.dropping                  = item_v25.dropping;
		i.playAnimation             = item_v25.playAnimation;
		i.fixedReqClvl              = item_v25.RequiredLevel;
		strcpy(i.Name,                item_v25.Name);
		strcpy(i.FullMagicalItemName, item_v25.FullMagicalItemName);
		i.baseItemIndex             = item_v25.baseItemIndex;
		i.socketsAmount             = item_v25.socketsAmount;
		i.sockets                   = item_v25.sockets;
		i.enchantIndexes            = item_v25.enchantIndexes;
		i.quenchAffix               = item_v25.quenchAffix;
		i.quenchLevel               = item_v25.quenchLevel;
		i.subType                   = item_v25.subType;
		i.grade                     = item_v25.grade;
		i.difficulty                = item_v25.difficulty;
		i.charFlag                  = item_v25.charFlag;
		i.socketsBase               = item_v25.socketsBase;
		i.socketsAdded              = item_v25.socketsAdded;
		i.RequiredStrength          = item_v25.RequiredStrength;
		i.RequiredMagic             = item_v25.RequiredMagic;
		i.RequiredDexterity         = item_v25.RequiredDexterity;
		i.RequiredVitality          = item_v25.RequiredVitality;
		each(i.effect).id = AE_NO;
		i.saveVersion = CurSaveVersion;
		UpdateItem(i);
	}
	i.saveVersion = CurSaveVersion;
}

//----- (0042D421) --------------------------------------------------------
void __fastcall LoadPlayer(int playerIndex)
{
	Player p;
	size_t readStart = (size_t)CurSaveData;
	if( SaveVersion < 26 ){
		P_v25 p25;
		static_assert( sizeof(P_v25) == StructSize<Player>(25), "sizeof P_v25 and Player::versions[25].size are not the same");
		ReadSaveData( &p25, offsetof( P_v25, P_v25::OnBodySlots ) ); // TODO: wrong offset for load save if xpGain expanded
		for( int i = 0; i < (SaveVersion ? 10 : 7); ++i ) LoadItemFull( &p.OnBodySlots[ i ] );
		if( SaveVersion == 0 ){ // th1, hf
			p25.OnBodySlots[IS_Waist].ItemCode = IC_M1_NONE;
			p25.OnBodySlots[IS_Hands].ItemCode = IC_M1_NONE;
			p25.OnBodySlots[IS_Feet ].ItemCode = IC_M1_NONE;
		}
		for( int i = 0; i < (SaveVersion < 12 ? 40 : 70); ++i ) LoadItemFull( &p.InventorySlots[ i ] );
		if( SaveVersion < 12 ){
			for( int i = 40; i < 70; ++i ){
				p25.InventorySlots[i].ItemCode = IC_M1_NONE;
				p25.InvUsed[i] = 0;
			}
		}
		ReadSaveData( &p25.InvItemCount, sizeof( p25.InvItemCount ) + sizeof(p25.InvUsed[0]) * (SaveVersion < 12 ? 40 : 70) );
		
		for( int i = 0; i < 8; ++i ) LoadItemFull( &p.BeltInventory[ i ] );
		LoadItemFull( &p.ItemOnCursor );

		ReadSaveData( &p25.MinDamageFromItem, offsetof(P_v25, P_v25::StayAnimCel) - offsetof( P_v25, P_v25::MinDamageFromItem) );
		size_t readed = (size_t)CurSaveData - readStart;
		if( SaveVersion <= 1 ){
			memset( &p25.goldFind, 0, sizeof(P_v25) - offsetof(P_v25, P_v25::goldFind) );
		}else{
			size_t savedPlayerSize = StructSize<Player>(SaveVersion); // TODO: fix Player_v25 load
			size_t readSize = savedPlayerSize - readed - (offsetof(P_v25, P_v25::goldFind) - offsetof(P_v25, P_v25::StayAnimCel));
			//if( (int)readSize < 0 ) __debugbreak();
			if( SaveVersion >= 18 ){
				ReadSaveData(&p25.goldFind, readSize);
				if (sizeof(P_v25) > savedPlayerSize) {
					memset((char*)&p25.goldFind + readSize, 0, sizeof(P_v25) - savedPlayerSize);
				}
			}else{
				ReadSaveData(&p25.goldFind, offsetof(P_v25, P_v25::perk) - offsetof(P_v25, P_v25::goldFind));
				ReadSaveData(&p25.perk, 100);
				ReadSaveData(&p25.subclassID, offsetof(P_v25, P_v25::align3) - offsetof(P_v25, P_v25::subclassID) + 3);
			}
			
		}
		if( SaveVersion < 21 ){
			p25.ReservedCharLevel = p25.CharLevel = (char)p25.CharLevel;
			p25.Xp = (int)p25.Xp;
			p25.NextLevelUp = (int)p25.NextLevelUp;
		}
		if( SaveVersion < 22 ){
			p25.CurrentSeedRun    = p25.currentSeedRun_reserved;
		}
		if( SaveVersion < 23 ){
			for( int i = 0; i < 64; ++i ) p25.SpellLevels[i] = p25.unused_oldSpellLevels[i];
			for( int i = 64; i < 128; ++i ) p25.SpellLevels[i] = 0;
		}

		// convert Player_v25 to Player:
		for(int i = IS_LeftRing2; i <= IS_Neck2; ++i) p.OnBodySlots[i].ItemCode = IC_M1_NONE;
		each(p.TalismanSlots).ItemCode = IC_M1_NONE;
		each(p.TalUsed) = 0;
		p.TalItemCount = 0;
		
		p.CurAction							= (PLAYER_CUR_ACTION) p25.CurAction;
		p.walkPath[MaxPathStep_25]			= p25.walkPath[MaxPathStep_25];
		p.fullClassId						= p25.fullClassId;
		p.destAction						= (PLAYER_COMMAND) p25.destAction;
		p.destParam1						= p25.destParam1;
		p.destParam2						= p25.destParam2;
		p.destParam3						= p25.destParam3;
		p.destParam4						= p25.destParam4;
		p.dungeon							= p25.dungeon;
		p.dungeon_reserved					= 0;
		p.Row								= p25.Row;
		p.Col								= p25.Col;
		p.NextRow							= p25.NextRow;
		p.NextCol							= p25.NextCol;
		p.TargetRow							= p25.TargetRow;
		p.TargetCol							= p25.TargetCol;
		p.ownerRow							= p25.ownerRow;
		p.ownerCol							= p25.ownerCol;
		p.oldRow							= p25.oldRow;
		p.oldCol							= p25.oldCol;
		p.xOfs								= p25.xOfs;
		p.yOfs								= p25.yOfs;
		p.xVel								= p25.xVel;
		p.yVel								= p25.yVel;
		p.dir								= p25.dir;
		p.nextDir							= p25.nextDir;
		p.Appearance						= p25.Appearance;
		p.currentAnimationDelay				= p25.currentAnimationDelay;
		p.currentAnimationFrameTick			= p25.currentAnimationFrameTick;
		p.currentAnimationFramesCount		= p25.currentAnimationFramesCount;
		p.currentAnimationFrame				= p25.currentAnimationFrame;
		p.animWidth							= p25.animWidth;
		p.animWidth2						= p25.animWidth2;
		p.UnderArcType						= p25.UnderArcType;
		p.LightIndex						= p25.LightIndex;
		p.lightRadiusIdentificator			= p25.lightRadiusIdentificator;
		p.spellIndex						= p25.spellIndex;
		p.readySpellType					= p25.readySpellType;
		p.readySpellTargetType				= p25.readySpellTargetType;
		p.nextArrowElement					= p25.nextArrowElement;
		p.altWeaponIndex					= p25.altWeaponIndex;
		p.SpellOnCursor						= p25.SpellOnCursor;
		p.spellOnCursorType					= p25.spellOnCursorType;
		p.invCols							= p25.invCols;
		p.CurrentSpellIndex					= p25.CurrentSpellNumber;
		p.SpellType							= p25.SpellType;
		p.nextMeleeElement					= p25.nextMeleeElement;
		p.lastAttacker						= p25.lastAttacker;
		p.AdvancedUser						= p25.AdvancedUser;
		p.AvailableSpellMask				= p25.AvailableSpellMask;
		p.AvailableSkillMask				= p25.AvailableSkillMask;
		p.AvailableRelictMask				= p25.AvailableRelictMask;
		p.activeBuffFlag					= p25.activeBuffFlag;
		fori(4) p.SpellOnFuncKeys[i]		= p25.SpellsOnFuncKeys[i];
		fori(4) p.SpellTypeOnFuncKeys[i]	= ((uchar*)&p25.Is_F5_Enabled)[i];
		p.runEnabled              			= (bool)p25.SomeNewFlags;
		p.IsExists                			= (bool)p25.IsExists;
		p.activeInv               			= 0;
		p.IsBowInHand             			= (bool)p25.IsBowInHand;
		p.IsFlaskInHand           			= (bool)p25.IsFlaskInHand;
		p.running                 			= (bool)p25.running;
		p.CanBlock                			= (bool)p25.CanBlock;
		p.notHittableInTransition 			= (bool)p25.notHittableInTransition;
		p.lvlChanging             			= (bool)p25.lvlChanging;
		p.IsPlayerEnemy           			= (bool)p25.IsPlayerEnemy;
		p.Infravision             			= (bool)p25.Infravision;
		p.hasGreetedByOgden       			= (bool)p25.hasGreetedByOgden;
		p.LightRadius						= p25.LightRadius;
		strcpy(p.playerName					, p25.playerName);
		p.ClassID							= p25.ClassID;
		p.CurStrength						= p25.CurStrength;
		p.BaseStrength						= p25.BaseStrength;
		p.CurMagic							= p25.CurMagic;
		p.BaseMagic							= p25.BaseMagic;
		p.CurDexterity						= p25.CurDexterity;
		p.BaseDexterity						= p25.BaseDexterity;
		p.CurVitality						= p25.CurVitality;
		p.BaseVitality						= p25.BaseVitality;
		p.AvailableLvlPoints				= p25.AvailableLvlPoints;
		p.BaseDamage						= p25.BaseDamage;
		p.BlockBonus						= p25.BlockBonus;
		p.BaseLife							= p25.BaseLife;
		p.MaxBaseLife						= p25.MaxBaseLife;
		p.CurLife							= p25.CurLife;
		p.MaxCurLife						= p25.MaxCurLife;
		p.RatioLifeGlobe					= p25.RatioLifeGlobe;
		p.BaseMana							= p25.BaseMana;
		p.MaxBaseMana						= p25.MaxBaseMana;
		p.CurMana							= p25.CurMana;
		p.MaxCurMana						= p25.MaxCurMana;
		p.RatioManaGlobe					= p25.RatioManaGlobe;
		p.CharLevel							= p25.CharLevel;
		p.controlCharLevel					= p25.ReservedCharLevel;
		p.Xp								= p25.Xp;
		p.NextLevelUp						= p25.NextLevelUp;
		p.TotalGold							= p25.TotalGold;
		p.CursorRow							= p25.CursorRow;
		p.CursorCol							= p25.CursorCol;
		p.newDirection						= p25.newDirection;
		p.HorizontalNextRowOrSomeSpellData	= p25.HorizontalNextRowOrSomeSpellData;
		p.HorizontalNextCol					= p25.HorizontalNextCol;
		p.dx64								= p25.dx64;
		p.dy64								= p25.dy64;
		p.currentActionFrame				= p25.CurAnimFrameCount;
		p.dungeonVisited					= p25.dungeonVisited;
		p.notHittableFramesAfterTransition	= p25.notHittableFramesAfterTransition;
		p.LoadPlayerAnimationMask			= p25.LoadPlayerAnimationMask;
		p.AvailableChargesMask				= p25.AvailableChargesMask;
		p.traits							= p25.traits;
		p.OilEffectOnCursor					= p25.OilEffectOnCursor;
		p.OpenedTownDoorMask				= p25.OpenedTownDoors;
		p.DungeonMessageFlags1				= p25.DungeonMessageFlags1;
		p.SomeSoundMuteDelay				= p25.SomeSoundMuteDelay;
		p.DungeonMessageFlags2				= p25.DungeonMessageFlags2;
		p.CountOfReflectCharges				= p25.CountOfReflectCharges;
		p.allowedDifficulty					= p25.allowedDifficulty;
		p.Difficulty						= p25.Difficulty;
		fori(CD_CAP) p.cooldowns[i]			= p25.cooldowns[i];
		p.scaleResist						= p25.scaleResist;
		fori(128) p.SpellLevels[i]			= p25.SpellLevels[i];
		p.PrevRow							= p25.PrevRow;
		p.PrevCol							= p25.PrevCol;
		p.PrevCursorX						= p25.PrevCursorX;
		p.PrevCursorY						= p25.PrevCursorY;
		fori(PERKS_MAX_800) p.perk[i]		= p25.perk[i];
		p.gameChanger						= p25.gameChanger;
		p.seed2								= p25.seed2;
		p.CurrentSeedRun					= p25.CurrentSeedRun;
		p.CurrentSeedRun_64_reserved		= 0;
		p.subclassID						= p25.subclassID;
		p.specializationID					= p25.specializationID;
		p.gameMode							= p25.gameMode;
		fori(5) p.AuraIndex[i]				= p25.AuraIndex[i];
		fori(5) p.AurasValues[i]			= p25.AurasValues[i];
	}else{ // SaveVersion >= 26
		ReadSaveData( &p, sizeof(Player) );
	}

	Player& player = Players[playerIndex];
	p.currentAnimationPointer = player.currentAnimationPointer;
	p.StayAnimCel			  = player.StayAnimCel			  ;
	p.WalkAnimCel			  = player.WalkAnimCel			  ;
	p.AttackAnimCel		      = player.AttackAnimCel		  ;
	p.LightningSpellAnimCel   = player.LightningSpellAnimCel  ;
	p.FireSpellAnimCel		  = player.FireSpellAnimCel		  ;
	p.MagicSpellAnimCel	      = player.MagicSpellAnimCel	  ;
	p.GotHitAnimCel		      = player.GotHitAnimCel		  ;
	p.DeathAnimCel			  = player.DeathAnimCel			  ;
	p.BlockAnimCel			  = player.BlockAnimCel			  ;
	p.turningAnimCel		  = player.turningAnimCel		  ;
	memcpy(p.nothingAnimationsPointers       , player.nothingAnimationsPointers       , sizeof(p.nothingAnimationsPointers       ));
	memcpy(p.walkAnimationsPointers          , player.walkAnimationsPointers          , sizeof(p.walkAnimationsPointers          ));
	memcpy(p.attackAnimationsPointers        , player.attackAnimationsPointers        , sizeof(p.attackAnimationsPointers        ));
	memcpy(p.lightningSpellAnimationsPointers, player.lightningSpellAnimationsPointers, sizeof(p.lightningSpellAnimationsPointers));
	memcpy(p.fireSpellAnimationsPointers     , player.fireSpellAnimationsPointers     , sizeof(p.fireSpellAnimationsPointers     ));
	memcpy(p.magicSpellAnimationsPointers    , player.magicSpellAnimationsPointers    , sizeof(p.magicSpellAnimationsPointers    ));
	memcpy(p.gotHitAnimationsPointers        , player.gotHitAnimationsPointers        , sizeof(p.gotHitAnimationsPointers        ));
	memcpy(p.deathAnimationsPointers         , player.deathAnimationsPointers         , sizeof(p.deathAnimationsPointers         ));
	memcpy(p.blockAnimationsPointers         , player.blockAnimationsPointers         , sizeof(p.blockAnimationsPointers         ));
	memcpy(p.turningAnimationsPointers       , player.turningAnimationsPointers       , sizeof(p.turningAnimationsPointers       ));
	p.StayAnimationFrameCount        = player.StayAnimationFrameCount;
	p.AnimPitchStay                  = player.AnimPitchStay; 
	p.WalkAnimationFrameCount        = player.WalkAnimationFrameCount;
	p.AnimPitchWalk                  = player.AnimPitchWalk;
	p.AttackAnimationFrameCount      = player.AttackAnimationFrameCount;
	p.AnimPitchAttack                = player.AnimPitchAttack;
	p.AttackMainFrame                = player.AttackMainFrame;
	p.SpellAnimationFrameCount       = player.SpellAnimationFrameCount;
	p.AnimPitchCast                  = player.AnimPitchCast;
	p.SpellAnimationActionFrameIndex = player.SpellAnimationActionFrameIndex;
	p.HitRecoveryAnimationFrameCount = player.HitRecoveryAnimationFrameCount;
	p.AnimPitchHit                   = player.AnimPitchHit;
	p.DeathAnimationFrameCount       = player.DeathAnimationFrameCount;
	p.AnimPitchDeath                 = player.AnimPitchDeath;
	p.BlockAnimationFrameCount       = player.BlockAnimationFrameCount;
	p.AnimPitchBlock                 = player.AnimPitchBlock;
	p.tunringAnimationFrameCount     = player.tunringAnimationFrameCount;
	p.AnimPitchTurning               = player.AnimPitchTurning;

	player = p;

	player.LoadPlayerAnimationMask = PAF_0_NO;
	if( player.altWeaponIndex >= 3 ) player.altWeaponIndex = 0;
	NonGameplaySeed = player.seed2;
}

//----- (0042D44B) --------------------------------------------------------
void __fastcall LoadMonster(int monsterIndex)
{
	ReadSaveData(&Monsters[monsterIndex], offsetof(Monster, Monster::Name) );
	// (20) расширение номера босса 0042D46E
	// Конвертация старой версии в новую
	Monster& monster = Monsters[monsterIndex];
	if( monster.BossID ){
	 	monster.newBossId = monster.BossID;
	 	monster.BossID = 0;
	}
	if( monster.unused_oldMonsterLevel){
		monster.MonsterLevel = monster.unused_oldMonsterLevel;
		monster.unused_oldMonsterLevel = 0;
	}
	PrepareLoadedMonster(monsterIndex);
}

//----- (0042D481) --------------------------------------------------------
void __fastcall LoadMissile(int missileIndex)
{
	ReadSaveData(&Missiles[missileIndex], sizeof(Missile) );
}

//----- (0042D4AB) --------------------------------------------------------
void __fastcall LoadObject(int objectIndex)
{
	ReadSaveData(&Objects[objectIndex], sizeof(Object) );
}

//----- (0042D4CD) --------------------------------------------------------
void __fastcall LoadItems(int itemOnGroundIndex)
{
	LoadItemFull( &Items[ itemOnGroundIndex ] );
	SetUniqueItemCELFile(itemOnGroundIndex);
}

//----- (0042D503) --------------------------------------------------------
void __fastcall LoadMagicStoreItem(int magicStoreItemIndex)
{
	LoadItemFull( &MagicItemsStoreArray[magicStoreItemIndex] );
}

//----- (0042D52D) --------------------------------------------------------
void __fastcall LoadQuest(int questIndex)
{
	Quest tmp;
	Quest& quest = questIndex < Quest_Count ? Quests[questIndex] : tmp;
	ReadSaveData( &quest, sizeof(Quest) );
	Dungeon.returnRow = GetNextHtonl(); // TODO: check for needing to save it 50 times
	Dungeon.returnCol = GetNextHtonl();
	Dungeon.returnLvl = (DUNGEON)GetNextHtonl(); // TODO: continiue here, size change to byte
	if( SaveVersion < 24 ) GetNextHtonl(); // was SPQuestDungeonType
	DiabloQuestTimer = GetNextHtonl();
}

//----- (0042D581) --------------------------------------------------------
void __fastcall LoadObjectLight(int objectLightIndex)
{
	ReadSaveData(&LightList[objectLightIndex], sizeof(Light));
}

//----- (0042D5A3) --------------------------------------------------------
void __fastcall LoadPlayerVisibility(int playerVisibilityIndex)
{
	ReadSaveData(&VisionList[playerVisibilityIndex], sizeof(Light));
}

//----- (0042D5C5) --------------------------------------------------------
void __fastcall LoadPortal(int entryPointIndex)
{
	ReadSaveData(&Portals[entryPointIndex], sizeof(Portal));
}


//----- (0042D5E8) --------------------------------------------------------
void SaveGame()
{
	uchar* ptrSaveData = SaveBuffer_Dungeon;
	SaveVersion = CurSaveVersion; // for buffer save/load 
	CurSaveData = (char*)ptrSaveData;
	PutNextHtonl2(SaveSignature);
	PutNextHtonl(CurSaveVersion); // сейв th2 версии CurVersion
	PutNextHtonl(PlayerRowPos);
	PutNextHtonl(PlayerColPos);
	PutNextBool(IsINVPanelVisible);
	PutNextBool(IsCHARPanelVisible);
	PutNextHtonl(MonstersCount);
	PutNextHtonl(ItemsCount);
	PutNextHtonl(MissileAmount);
	PutNextHtonl(ObjectsCount);

	Player& player = Players[CurrentPlayerIndex];

	for( int dun = 0; dun < DungeonExpantionLimit; dun++ ){
		PutNextHtonl2(DungeonRandSeed[dun]);
	}

	player.Difficulty = Difficulty;
	player.runEnabled = IsFastWalkEnabled;

	SavePlayer(CurrentPlayerIndex);
	PutNextByte(Quest_Count);
	for( int index = 0; index < Quest_Count; index++){ // TODO: continue here, add Quest_Count saving
		SaveQuest(index);	
	}
	for( int index = 0; index < PlayersMax_4; index++){		
		SavePortal(index);	
	}

	{
		int* ptr = reinterpret_cast<int*>(TotalSlain);
		for( int i = 0, ie = countof(TotalSlain)/2; i < ie; i++){		
			PutNextHtonl2(*ptr++);
		}
	}
	
	if( Dungeon->genType || Dungeon->isQuest ){
		for( int index = 0; index < Monsters_Max_Count; index++){
			PutNextHtonl(MonsterIndexes[index]);	
		}
		for( int index = 0; index < MonstersCount; index++){	
			SaveMonster(MonsterIndexes[index]);		
		}

		for( int index = 0; index < MissilesMax; index++){
			PutNextWord(MissileIndexes[index]);
		}
		for( int index = 0; index < MissilesMax; index++){
			PutNextWord(MissileIndexList[index]);
		}
		for( int index = 0; index < MissileAmount; index++){			
			SaveMissile(MissileIndexes[index]);		
		}

		for( int index = 0; index < MaxObjects_127; index++){			
			PutNextByte((uchar)ObjectActive[index]);		
		}
		for( int index = 0; index < MaxObjects_127; index++){			
			PutNextByte((uchar)ObjectAvailable[index]);
		}
		for( int index = 0; index < ObjectsCount; index++){		
			SaveObject(ObjectActive[index]);	
		}

		PutNextHtonl(LightsCount);
		for( int index = 0; index < Lights_32_Count;	index++){			
			PutNextByte(LightIndexes[index]);		
		}
		for( int index = 0; index < LightsCount; index++ ){			
			SaveObjectLight(LightIndexes[index]);	
		}

		PutNextHtonl(VisionId);
		PutNextHtonl(VisionCount);
		for( int index = 0; index < VisionCount; index++ ){		
			SavePlayerVisibility(index);				
		}
	}

	for( int index = 0; index < MaxItems_255; index++ ){ PutNextByte(ItemActive[index]); }
	for( int index = 0; index < MaxItems_255; index++ ){ PutNextByte(ItemAvailable[index]); }
	for( int index = 0; index < ItemsCount; index++ ){ SaveItemsOnGround(ItemActive[index]); }

	for( int index = 0; index < 128; index++ ){
		CurSaveData++; // backward compatibility with now unused AlreadyDroppedUniqs, can be reused
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PutNextByte( (char) LightMap[ row ][ col ] );
		}
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PutNextByte(FlagMap[ row ][ col ] & ~(CF_1_SPELL_CAST | CF_2_VISIBLE_BY_PLAYER | CF_4_DEAD_PLAYER)); // 1111 1000 обнуляем 3 бита. Что они значат?
		}
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PutNextByte(PlayerMap[ row ][ col ]);
		}
	}

	for( int col = 0; col < FineMap_112; ++col ){
		for( int row = 0; row < FineMap_112; ++row ){
			PutNextByte(ItemMap[ row ][ col ]);
		}
	}

	if( Dungeon->genType || Dungeon->isQuest ){
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextHtonl(MonsterMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextWord(DeathMonstersMap[ row ][ col ]);
			}
		}
		
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte(BloodMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte(ObjectsMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte((char)LightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte((char)StaticLightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < Map_40; col++ ){
			for( int row = 0; row < Map_40; row++ ){
				PutNextBool((uchar)AutoMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextWord(MissilesMap[ row ][ col ]);
			}
		}
	}
	PutNextHtonl(GriswoldMagicItemCount);
	PutNextHtonl(GriswoldItemsLevelParam);
	for( int index = 0; index < PT_MagicItemsCount; index++ ){
		SaveMagicStoreItem(index);
	}
	PutNextByte(AutomapMode);
	PutNextHtonl(AutomapZoomFactor);

	int saveSize = CalcEncodeDstBytes(CurSaveData - (char*)ptrSaveData);
	if( saveSize > sizeof(SaveBuffer_Dungeon) ) TerminateWithError( "Save buffer size exceeded by %i bytes", saveSize - sizeof(SaveBuffer_Dungeon) );
	WriteSaveFile(SaveFile_Game, ptrSaveData, CurSaveData - (char*)ptrSaveData, saveSize);
	gbValidSaveFile = true;
	MoveTempSavesToPermanent();
	UpdatePlayerFile();
}

//----- (0042DAB8) --------------------------------------------------------
void __fastcall PutNextByte(char value)
{
	*CurSaveData++ = value;
}

void __fastcall PutNextWord(ushort value)
{
	char* byteArray = (char*)&value;
	*CurSaveData++ = byteArray[1];
	*CurSaveData++ = byteArray[0];
}

//----- (0042DAC6) --------------------------------------------------------
void __fastcall PutNextHtonl(uint value)
{
	char* byteArray = (char*)&value;
	*CurSaveData++ = byteArray[3];
	*CurSaveData++ = byteArray[2];
	*CurSaveData++ = byteArray[1];
	*CurSaveData++ = byteArray[0];
}

//----- (0042DB0D) --------------------------------------------------------
void __fastcall PutNextHtonl2(int value)
{
	char* byteArray = (char*)&value;
	*CurSaveData++ = byteArray[3];
	*CurSaveData++ = byteArray[2];
	*CurSaveData++ = byteArray[1];
	*CurSaveData++ = byteArray[0];
}

//----- (0042DB54) --------------------------------------------------------
void __fastcall PutNextBool(int value)
{
	*CurSaveData++ = value ? 1 : 0;
}

//----- (0042DB6C) --------------------------------------------------------
void __fastcall SavePlayer(int playerIndex)
{
	WriteSaveData( &Players[playerIndex], sizeof(Player) );
}

//----- (0042DB96) --------------------------------------------------------
void __fastcall SaveMonster(int monsterIndex)
{
	WriteSaveData( &Monsters[monsterIndex], offsetof(Monster, Monster::Name) );
}

//----- (0042DBC0) --------------------------------------------------------
void __fastcall SaveMissile(int missileIndex)
{
	WriteSaveData( &Missiles[missileIndex], sizeof(Missile) );
}

//----- (0042DBEA) --------------------------------------------------------
void __fastcall SaveObject(int objectIndex)
{
	WriteSaveData( &Objects[objectIndex], sizeof(Object));
}

//----- (0042DC0C) --------------------------------------------------------
void __fastcall SaveItemsOnGround(int itemOnGroundIndex)
{
	WriteSaveData( &Items[itemOnGroundIndex], sizeof(Item));
}

//----- (0042DC36) --------------------------------------------------------
void __fastcall SaveMagicStoreItem(int magicStoreItemIndex)
{
	WriteSaveData( &MagicItemsStoreArray[magicStoreItemIndex], sizeof(Item));
}

//----- (0042DC60) --------------------------------------------------------
void __fastcall SaveQuest(int questIndex)
{
	WriteSaveData(&Quests[questIndex], sizeof(Quest));
	PutNextHtonl(Dungeon.returnRow);
	PutNextHtonl(Dungeon.returnCol);
	PutNextHtonl(Dungeon.returnLvl);
	PutNextHtonl(DiabloQuestTimer);
}

//----- (0042DCB8) --------------------------------------------------------
void __fastcall SaveObjectLight(int objectLightIndex)
{
	WriteSaveData( &LightList[objectLightIndex], sizeof(Light));
}

//----- (0042DCDA) --------------------------------------------------------
void __fastcall SavePlayerVisibility(int playerVisibilityIndex)
{
	WriteSaveData( &VisionList[playerVisibilityIndex], sizeof(Light));
}

//----- (0042DCFC) --------------------------------------------------------
void __fastcall SavePortal(int entryPointIndex)
{
	WriteSaveData( &Portals[entryPointIndex], sizeof(Portal));
}

//----- (0042DD1F) --------------------------------------------------------
void SaveLevel()
{
	char* ptrSaveData = (char*) SaveBuffer_Dungeon;
	CurSaveData = ptrSaveData;
	SaveVersion = CurSaveVersion; // for buffer save/load 
	PutNextHtonl(CurSaveVersion); // SaveVersion for map
	if( Dungeon->genType || Dungeon->isQuest ){
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextWord(DeathMonstersMap[ row ][ col ]);
			}
		}
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte(BloodMap[ row ][ col ]);
			}
		}
	}
	PutNextHtonl(MonstersCount);
	PutNextHtonl(ItemsCount);
	PutNextHtonl(ObjectsCount);
	if( Dungeon->genType || Dungeon->isQuest ){
		
		for( int monsterIndexIndex = 0; monsterIndexIndex < Monsters_Max_Count; monsterIndexIndex++ ){
			PutNextHtonl(MonsterIndexes[ monsterIndexIndex ]);
		}

		for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
			SaveMonster(MonsterIndexes[ monsterIndexIndex ]);
		}

		for( int objectIndexIndex = 0; objectIndexIndex < MaxObjects_127; objectIndexIndex++ ){
			PutNextByte(ObjectActive[ objectIndexIndex ]);
		}

		for( int i = 0; i < MaxObjects_127; i++ ){
			PutNextByte(ObjectAvailable[ i ]);
		}

		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			SaveObject(ObjectActive[ objectIndexIndex ]);
		}
	}

	for( int i = 0; i < MaxItems_255; i++ ){ PutNextByte(ItemActive[ i ]); }
	for( int i = 0; i < MaxItems_255; i++ ){ PutNextByte(ItemAvailable[ i ]); }
	for( int i = 0; i < ItemsCount; i++ ){ SaveItemsOnGround( ItemActive[ i ] ); }

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PutNextByte(FlagMap[ row ][ col ] & ~(CF_1_SPELL_CAST | CF_2_VISIBLE_BY_PLAYER | CF_4_DEAD_PLAYER));
		}
	}

	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			PutNextByte(ItemMap[ row ][ col ]);
		}
	}

	if( Dungeon->genType || Dungeon->isQuest ){

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextHtonl(MonsterMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte(ObjectsMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte((char)LightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextByte((char)StaticLightMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < Map_40; col++ ){
			for( int row = 0; row < Map_40; row++ ){
				PutNextBool((uchar)AutoMap[ row ][ col ]);
			}
		}

		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				PutNextWord(MissilesMap[ row ][ col ]);
			}
		}
	}

	char decodeKey[MAX_PATH];
	CreateSaveLevelName(decodeKey);
	int saveSize = CalcEncodeDstBytes(CurSaveData - ptrSaveData);
	if( saveSize > sizeof(SaveBuffer_Dungeon) ) TerminateWithError( "Save buffer size exceeded by %i bytes", saveSize - sizeof(SaveBuffer_Dungeon) );
	WriteSaveFile(decodeKey, (uchar*) ptrSaveData, CurSaveData - ptrSaveData, saveSize);
	CurSaveData = 0;
	Player& player = Players[CurrentPlayerIndex];
	player.dungeonVisited[(DUNGEON)Dungeon] = 1;
}

//----- (0042E007) --------------------------------------------------------
void LoadLevel()
{
	char fileName[MAX_PATH];
	CreateLoadLevelName(fileName); // can not version load level name, SaveVersion loads later
	uint loadedSize;
	char* saveFileData = (char*)ReadSaveFile(fileName, &loadedSize, 0);
	CurSaveData = saveFileData;
	SaveVersion = GetNextHtonl();
	if( SaveVersion < 20 ){ // for old save version (< 20) without map versioning
		SaveVersion = 19;
		CurSaveData -= 4;
	}
	if( Dungeon->genType || Dungeon->isQuest ){
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				DeathMonstersMap[ row ][ col ] = GetNextWord();
			}
		}
		SyncUniqDead();
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				BloodMap[ row ][ col ] = GetNextByte();
			}
		}
	}
	MonstersCount = GetNextHtonl();
	ItemsCount = GetNextHtonl();
	ObjectsCount = GetNextHtonl();

	if( Dungeon->genType || Dungeon->isQuest ){
		// Load monster block
		for( int index = 0; index < Monsters_Max_Count; index++) MonsterIndexes[index] = GetNextHtonl();
		for( int index = 0; index < MonstersCount; index++) LoadMonster(MonsterIndexes[index]);
		SetBossLightActive();

		// Load object block
		for( int index = 0; index < MaxObjects_127; index++) ObjectActive[index] = GetNextByte();		
		for( int index = 0; index < MaxObjects_127; index++) ObjectAvailable[index] = GetNextByte();
		for( int index = 0; index < ObjectsCount; index++) LoadObject(ObjectActive[index]);
		for( int index = 0; index < ObjectsCount; index++) SetDunTileForObject(ObjectActive[index]);
	}

	// Load item block
	LoadItemsActive();

	// Load map info
	Player& player = Players[CurrentPlayerIndex];
	for( int col = 0; col < FineMap_112; col++ ){
		for( int row = 0; row < FineMap_112; row++ ){
			FlagMap[ row ][ col ] = GetNextByte();
			if (FlagMap[row][col] & CF_64_VISIBLE_BY_CURSOR){
				if (/*(abs(row - player.Row) > player.LightRadius || abs(col - player.Col) > player.LightRadius) &&*/ !CheckCastInSight(player.Row, player.Col, row, col)){
					FlagMap[row][col] &= ~CF_64_VISIBLE_BY_CURSOR;
				}
			}
		}
	}
	LoadItemMap();

	if( Dungeon->genType || Dungeon->isQuest ){
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				MonsterMap[ row ][ col ] = GetNextHtonl();
			}
		}
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				ObjectsMap[ row ][ col ] = GetNextByte();
			}
		}
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				LightMap[ row ][ col ] = GetNextByte();
			}
		}
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				StaticLightMap[ row ][ col ] = GetNextByte();
			}
		}
		for( int col = 0; col < Map_40; col++ ){
			for( int row = 0; row < Map_40; row++ ){
				AutoMap[ row ][ col ] = GetNextBool();
			}
		}
		for( int col = 0; col < FineMap_112; col++ ){
			for( int row = 0; row < FineMap_112; row++ ){
				MissilesMap[ row ][ col ] = GetNextWord(); // в оригинале 0
			}
		}
	}
	
	for( int r = 0; r < 112; r++ ){ // removes light leftovers from warps
		for( int c = 0; c < 112; c++ ){
			LightMap[r][c] = StaticLightMap[r][c];
		}
	}

	// Misc sync routines
	SyncAutomap();
	ResyncQuests();
	SyncPortals();
	
	// player lighting needs to be reset because players enter level at different
	// location than where they leave
	NeedLightRecalc = true;
	for( int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && Dungeon == player.dungeon && player.LightIndex >= 0 ){
			LightList[player.LightIndex].unflag = 1;
		}
	}
	FreeMem(saveFileData);
	CurSaveData = 0;
}

//----- (th4) -------------------------------------------------------------
void ValidateMonsters()
{
	for( int i = 0; i < MonstersCount; ++i) Monsters[MonsterIndexes[i]].flag &= ~MF_16_TRACK_ATTACK;
	for (int i = 0; i < MissileAmount; ++i){
		Missile& m = Missiles[MissileIndexes[i]]; 
		if( m.BaseMissileIndex == MI_145_TENTACLE ) Monsters[m.CasterIndex].flag |= MF_16_TRACK_ATTACK;
	}
}
