#ifndef _game_h_
#define _game_h_
#else
#error reinclude game.h
#endif

// TODO: try use itemInfo in StashTab instead of Item ?
struct StashInfo
{
	uint signature;
	uint version;
	struct {
		uint itemCount;
		short usedFlags[Stash_MaxSlot];
		LastItemInfo itemInfo[Stash_MaxSlot];
	} tabInfo[Stash_TotalTabs];
};

struct TransmuteInfo
{
	uint signature;
	uint version;
	uint count;
	decltype(TransmuteSlots.used) used;
	LastItemInfo itemInfo[TransmuteSlots.Max];
};

struct DungeonInfo
{
	// swap weapon info сохраняется отдельным от игрока блоком в сейве
	Player                          _1;
	LastPlayerInfo                  _2;
	decltype(PlayerMap            ) _3;
	decltype(MagicItemsStoreArray ) _4;
	decltype(Items                ) _5;
	decltype(ItemMap              ) _6;
	decltype(ItemAvailable        ) _7;
	decltype(ItemActive           ) _8;
	decltype(Missiles             ) _9;
	decltype(MissilesMap          ) _10;
	decltype(MissileIndexes       ) _11;
	decltype(MissileIndexList     ) _12;
	decltype(DungeonRandSeed      ) _13;
	decltype(Portals              ) _14;
	decltype(TotalSlain           ) _15;
	decltype(LightIndexes         ) _16;
	decltype(LightMap             ) _17;
	decltype(StaticLightMap       ) _18;
	decltype(VisionList           ) _19;
	decltype(Objects              ) _20;
	decltype(ObjectsMap           ) _21;
	decltype(ObjectActive         ) _22;
	decltype(ObjectAvailable      ) _23;
	decltype(Monsters             ) _24;
	decltype(MonsterIndexes       ) _25;
	decltype(MonsterMap           ) _26;
	decltype(DeathMonstersMap     ) _27;
	decltype(FlagMap              ) _28;
	decltype(AutoMap              ) _29;
	decltype(BloodMap             ) _30;
	uchar _[256 * 1024]; // counters + buffer zone 
};

extern HashEntry  HashTbl [HASH_ENTRIES];
extern BlockEntry BlockTbl[BLOCK_ENTRIES];
extern uchar SaveBuffer_Transmute [CalcEncodeDstBytes(sizeof(TransmuteInfo))];
extern uchar SaveBuffer_Stash     [CalcEncodeDstBytes(sizeof(StashInfo))];
extern uchar SaveBuffer_WeaponSwap[CalcEncodeDstBytes(sizeof(WeaponSwapItems))];
extern uchar SaveBuffer_Dungeon   [CalcEncodeDstBytes(sizeof(DungeonInfo))];