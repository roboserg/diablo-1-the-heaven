#ifndef _structs_h_
#define _structs_h_
#else
#error reinclude structs.h
#endif

typedef char CharName[32];
typedef void __fastcall AiFunc( unsigned int );
struct Ai
{
	AI ai;
	AiFunc* aiFunc;
};

typedef int __stdcall ExceptionFilter( struct _EXCEPTION_POINTERS* );
typedef void __cdecl MenuHandler();
typedef void __fastcall DrawFunc( int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10 );
typedef void __fastcall ChampType( struct Monster& monster );

// sizeof 8
struct TileSquare
{
	ushort up;
	ushort left;
	ushort right;
	ushort down;
};

// sizeof 16
struct HashEntry
{
	uint hashcheck[2];
	uint lcid;
	uint block;
};

// sizeof 8
struct LetterStruct
{
	int StartOffset;
	int EndOffset;
};

// sizeof 0x20 32
struct UniqueMonster
{
	UNIQUE_MONSTER	id;
	int				avail; // MONSTER_AVAILABILITY 0 - not, 1 - mod, 2 - classic, 3 - all
	int				BaseMonsterIndex;
	char*			NamePtr;
	char*			ColorTranslationFile;
	ushort			DungeonLevel;
	ushort			HitPoints;
	uchar			AttackType;
	uchar			Intelligence;
	int				MinAttackDamage;
	int				MaxAttackDamage;
	MONSTER_RESIST	ResistImmune;
	ushort			PackTrigger;
	int				PackSpecials;
	int				SpecialSound;
	char*			IceAgeColorTranslationFile;
	char*			IceAgeNamePtr;
};

// sizeof 0x80 128
struct BaseMonster
{
	BASE_MONSTER id;
	int			valid; // MONSTER_AVAILABILITY 0 - not, 1 - mod, 2 - classic, 3 - all
	int			SpritePitch;				// 0	0	width of monster
	int			SeedingSize;				// 0x4	4	ImageSize originally
	const char*	AnimationFileName;			// 0x8	8
	int			HasSecondAttack;			// 0xC	12
	const char*	SoundFileName;				// 0x10	16
	int			HasSpecialSounds;			// 0x14	20
	int			HasColorTranslationFile;	// 0x18	24
	const char*	ColorTranslationFilePtr;	// 0x1C	28
	int			animationFrameCount[6];		// 0x20	32
	int			animationDelay[6];			// 0x38	56
	const char*	name;						// 0x50	80
	uchar		MinDungeonLevel;			// 0x54	84
	uchar		MaxDungeonLevel;			// 0x55	85
	int			MonsterBaseLevel;			// 0x56	86
	int			MinHitPoints;				// 0x58	88
	int			MaxHitPoints;				// 0x5C	92
	int			ai;							// 0x60	96
	int			flag;						// 0x64	100
	uchar		intelligenceFactor;			// 0x68	104
	int			FirstAtackToHit;			// 0x69	105
	int			FirstAtackToHitFrame;		// 0x6A	106
	int			FirstAtackMinDamage;		// 0x6B	107
	int			FirstAtackMaxDamage;		// 0x6C	108
	int			SecondAttackToHit;			// 0x6D	109
	int			SecondAtackToHitFrame;		// 0x6E	110
	int			SecondAtackMinDamage;		// 0x6F	111
	int			SecondAtackMaxDamage;		// 0x70	112
	int			ArmorClass;					// 0x71	113
	ushort		MonsterClass;				// 0x72	114
	MONSTER_RESIST ResistOn1And2Difficulty; // 0x74	116
	MONSTER_RESIST ResistOn3Difficulty;		// 0x76	118
	int			ItemDropSpecials;			// 0x78	120
	ushort		SelectionOutline;			// 0x7A	122
	i64			ExperiencePoints;			// 0x7C	124
	const char* IceAgeTranslationFilePtr;	
	const char* IceAgeNamePtr;					
};

// sizeof 16
struct Speech 
{
	char*	TextPtr;	// 0 
	int		HasText;	// 4 
	int		stepIndex;	// 8 
	SOUND	SoundIndex; // 12
};

struct BaseCrit
{
	int chance;
	int percent;
	int damageMin;
	int damageMax;
	int levelDiv;
	void (*func)(int& damage, int l);
};

struct BaseEffect
{
	AFFIX_EFFECT id;
	short        type;
	short        minLow;
	short        minHigh;
	short        maxLow;
	short        maxHigh;
	short        chanceLow;
	short        chanceHigh;
	uint         fullType() { return (uint(id) << 16) | type; }
};

struct Affix
{
	int				id;
	char*			Name;
	BaseEffect		effect;
	int				quality;
	ITEM_AFFIX		item;
    ITEM_SPECIALISATION itemSpecialization;
	int				ExcludedCombinations; // если в префиксе 16, а в постфиксе 1 (или наоборот), то не комбинируется
	int				DoubleChance;
	int				NotCursed;
	int				MinGoldValue;
	int				MaxGoldValue;
	int				Multiplier;
	int				minReqClvl;
	int				maxReqClvl;
};

struct BaseAction
{
	EFFECT_ACTION id;
	char*       (*name)(struct Item& i, struct Effect& e, struct Player& p);
	TRIGGER       trigger;
	int		    (*act )(uint playerIndex, int type, int minVal, int maxVal, int row, int col);
};

struct Effect
{
	AFFIX_EFFECT id;
	uchar        origin; // AFFIX_ORIGIN
	short        type;
	short        minVal;
	short        maxVal;
	short        chance;
	uint         fullType() { return (uint(id) << 16) | type; }
};

struct EffectAction
{
	EFFECT_ACTION id;
	short         minVal;
	short         maxVal;
	short         chance;

	void operator=(Effect&);
	void operator+=(Effect&);
};

// sizeof 512
struct Item // TH4
{
	int				seed;					void SetSeed(i64 seed, int genVersion = CurItemGenVersion); i64 MakeSeed(); // 0 у квестовых уников здесь храниться uniqueItemIndex, для индекса уже есть поле Item_v25.UniqueItemIndex
	int				seed64_reserved;
	ITEM_CODE		ItemCode;				// (-1 == no item) ItemCode from BaseItem_ID 11 - Gold 196-188
	int				dropType;				// маска типа дропа синий/белый/уник/из босса/магазин (для генерации\восстановления)
	short			MapRow;
	short			MapCol;
	char*			ItemCELFilePtr;			// sprite data ? free()
	short			FramesCount;
	short			FrameIndex;
	short			AnimWidth;
	short			subOffsetX;				// always 16, this and next may be for subcell items positioning
	char			animPhase;
	char			EquippedLocation;
	MAGIC_LEVEL		MagicLevel;				// 0 - Normal, 1 - Magic, 2 - Unique (интерпретация данных вещи)
	MAGIC_CODE		MagicCode;
	char			TypeID;
	uchar			RequiredLevel;
	ushort			GraphicValue;
	short			MinDamage;
	short			MaxDamage;
	
	// should be offset 44 to load previous Item_v25
	ushort			saveVersion;			// save file verion, for old load map info in new save version in LoadItemFull
	ushort			genVersion;				// item generation version, do not change after saving

	short			ArmorClass;
	short			overTime;
	short			damageFromEnemies;      // base value from base item
	short           minMeleeAbsorbPercent;  // base value from base item
	short           maxMeleeAbsorbPercent;  // base value from base item
	short           minArrowAbsorbPercent;  // base value from base item
	short           maxArrowAbsorbPercent;  // base value from base item
	short			minThorns;
	short			maxThorns;
	char			SpellIndex;
	short           CurCharges;
	short           BaseCharges;
	short           CurDurability;
	short           BaseDurability;
	short			quality;
	short			magicFindSeed;			// also using for shop extras quality
	short			uniqIndexSeed;
	short			UniqueItemIndex;

	int				amount;					// gold quantity (also can be used to stackable items
	int				Price;
	int				basePrice;
	short			basePriceMultiplier;
	int				priceEffect;

	// flags
	ushort gottenFromLand: 1,
	       mirrorImage   : 1,
	       IsReqMet      : 1,
	       isShadow      : 1, // flag that it's not real item yet, but preview
	       throwing      : 1,
	       Identified    : 1,
	       dropping      : 1,
		   playAnimation : 1,
		   fixedReqClvl  : 1,
	                     : 7; // unused

	char			Name[42];
	char			FullMagicalItemName[64];
	
	int				baseItemIndex;

	uchar           socketsAmount;
	array<short, SocketMax_6>  sockets;
	array<short, EnchantMax_5> enchantIndexes;

	QUENCH_AFFIX	quenchAffix;
	uchar			quenchLevel;

	char			subType; // ITEM_SUBTYPE, GEM
	char			grade;   // oils, gems
	
	uchar			difficulty; // TODO: can be 3 bits (combine with charFlag)
	uchar			charFlag;
	uchar			socketsBase;
	uchar			socketsAdded;

	short			RequiredStrength;
	short			RequiredMagic;
	short			RequiredDexterity;
	short			RequiredVitality;

	Effect			effect[ 20 + 6 ]; // up to 20 unique item affixes, up to 6 base item affixes

	//void operator=(const struct ItemInfo64&); // unused, see LoadItem

	void getStatEffects(struct StatEffects&, struct Player&);
	static constexpr StructVersion versions[] = {{26, 512}, {16, 1339}, {15, 1093}, {14, 1093}, {13, 1081}, {8, 889}, {4, 877}, {3, 581}, {2, 555}, {1, 372}}; // 21, 1339 (добавлять слева в массив индекс старшей)
};
static_assert( sizeof(Item) == StructSize<Item>(CurSaveVersion), "Item size and Item::versions[CurVersion].size are not the same");

struct AffixEffect
{
	enum Use: uchar { Min = BIT(1), Max = BIT(2), Chance = BIT(3) };

	AFFIX_EFFECT id;
	uchar use;
	void( *info )(Item& i, Effect& e, struct Player& p); // TODO: make it private and add method with pointer check

	static const AffixEffect Table[255]; // TODO: change to pointer (?) to current mod table
};

// sizeof 8
struct CharStat
{
    short Strength; // 0
    short Magic; // 2
    short Dexterity;// 4
    short Vitality;// 6
};

// sizeof 55
struct CharSaveInfo 
{
	CharSaveInfo*	next;
	char			Name[16];
	ushort			Level;
	uchar			Class;
    uchar			SubClass;
    uchar			Specialization;
	GAME_MODE		GameMode;
	uchar			allowedDifficulty;
    CharStat        Stats;
    array<TraitId,4>Traits;
	i64			    GameChanger;
	int				CurrentSeedRun;
	int				currentSeedRun_reserved;
};

// sizeof 76 0x4C
struct BaseItem
{
	int		        baseId;
	int             mode; // isClassic
	int				ChanceToDrop;		// +0 (ActivationTrigger) 0 = Item will never be drop, 1 = Item will be dropable, 2 = double chance to drop
	ITEM_TYPE		ItemType;
	EQUIPPED_LOCATION EquippedLocation;
	uchar			AnimationDelay;
	uchar			Material;
	int				GraphicValue;
	char			ItemCode;
	int				uniqLine;
	bool			throwing;
	char*			NamePtr;
	char*			Name2;
	int				itemLevel;
	short			DurabilityMin;
	short			DurabilityMax;
	short			MinDamageLow;
	short			MinDamageHigh;
	short			MaxDamageLow;
	short			MaxDamageHigh;
	short			MinimumArmor;
	short			MaximumArmor;
	short			MinDamageFromEnemy;
	short			MaxDamageFromEnemy;
    short			MinMeleeAbsorbPercentLow;
    short			MinMeleeAbsorbPercentHigh;
    short			MaxMeleeAbsorbPercentLow;
    short			MaxMeleeAbsorbPercentHigh;
    short			MinArrowAbsorbPercentLow;
    short			MinArrowAbsorbPercentHigh;
    short			MaxArrowAbsorbPercentLow;
    short			MaxArrowAbsorbPercentHigh;
    short			MinThornsLow;
    short			MinThornsHigh;
    short			MaxThornsLow;
    short			MaxThornsHigh;
	short			RequiredStrengthMin;
	short			RequiredStrengthMax; // от требований базовых вещей зависит дроп, пока используем Max при ограничении
	short			RequiredMagicMin;
	short			RequiredMagicMax;
	short			RequiredDexterityMin;
	short			RequiredDexterityMax;
	short			RequiredVitalityMin;
	short			RequiredVitalityMax;
	uchar			RequiredLevelMin;
	uchar			RequiredLevelMax;
	uint            RequiredClassMask;
	bool            ForbiddenForExiles;
    ITEM_SPECIALISATION	Specialization;
	MAGIC_CODE		MagicCode;
    int      		FlaskOrTrapCode;
	PLAYER_SPELL	SpellNumber;
	int				UseOnceFlag;
	int             IsUncommon;
	int				MinPrice;
	int				MaxPrice;
	char            EffectCount;
    BaseEffect      Effects[6];
};

struct FlaskInfo
{
	int splashHitDamagePercentage;
	int DamageOverTimePercentage;
	int DamageOverTimeDuration;
	int MaxFlaskThrowDistance;
	short MinOverTime;
	short MaxOverTime;
};

struct TrapInfo
{
	int MaxTrapThrowDistance;
	int TimeToLive;
	MISSILE MissileIndex;
	int ShootDelay;
	int TargetDetectRadius;
	int ArrowSpeed;
};

struct DungeonType
{
	DUNGEON_TYPE id;
	char*        name;
	uchar        floor;
};

struct BaseDungeon // may be BaseDungeon
{
	DUNGEON      id;
	DUNGEON_TYPE genType;
	DUNGEON_TYPE graphType;
	int          level;
	bool         isQuest;
	char*        name;
	char*        warpName;
	char*        warpName2;
};

struct CurDungeon
{
	operator DUNGEON() const;
	operator int() const = delete;
	void    operator=(DUNGEON);
	DUNGEON operator+(int i);
	DUNGEON operator-(int i);
	const BaseDungeon* operator->();
	DUNGEON next;
	DUNGEON prev;
	DUNGEON returnLvl;
	int     returnRow;
	int     returnCol;
};

struct BaseQuest
{
	QUEST        id;              // Which quest it is
	QUEST_FLAG   flag;            // Quest flags
	DUNGEON      spDungeonStart;  // level the quest will appear on
	DUNGEON      mpDungeonStart;  // Which level the quest will appear on in multiplayer
	DUNGEON      classicStart;
	DUNGEON_TYPE dungeonType;     // Quest dungeon level type (255 is current)
	DUNGEON      spQuestDungeon;  // sp set dungeon id, DUN_NO for non quest dungeon
	DUNGEON      mpQuestDungeon;  // mp set dungeon id, DUN_NO for non quest dungeon
	char         retRow;          // return offset rows from entry position
	char         retCol;          // return offset columns from entry position
	SPEECH       speechIndex;     // initial quest message
	char*        questName;       // String description for log
};

//sizeof 28 
struct Quest 
{
	DUNGEON	dungeonStart;		// 0 Which level the quest will appear on
	char	unused;				// 1
	QUEST_STATUS status;		// 2 general quest activation state
	DUNGEON_TYPE dungeonType;	// 3
	int		row;				// 4 warp row ? (should not warp if questDungeon == DUN_NO)
	int		col;				// 8 warp col ?
	DUNGEON	questDungeon;		// 12 start quest (set) dungeon, DUN_NO for non quest dungeon
	QUEST   baseId;				// 13
	uchar	status4;			// 14 quest depended state/value 3
	uchar	status5;			// 15 quest depended state/value 4
	uint	speechIndex;		// 16
	uchar	status2;			// 20 quest depended state/value 1
	uchar	status3;			// 21 quest depended state/value 2
	ushort	timer;				// 22
	uint	talkToFlag;			// 24
};
static_assert( sizeof(Quest) == 28, "sizeof Quest != 28, it stored in save file");

struct QuestPattern
{
	const char* name    = 0; // dun file name
	short       ofsRow  = 0; // entry row offset
	short       ofsCol  = 0; // entry col offset
	QUEST       id      = Q_NO;
	short       flag    = 0; // QUEST_PATTERN flag
	char        botRow  = 0; // bottom border rows
	char        botCol  = 0; // bottom border cols
	char        topRow  = 0; // top border rows
	char        topCol  = 0; // top border cols
	short       row     = 0; // pattern start row (for place)
	short       col     = 0; // pattern start col (for place)
	short       sizeRow = 0; // pattern size rows (for place)
	short       sizeCol = 0; // pattern size cols (for place)
	ushort*     dun     = 0; // dun file shorts
	uchar*      tiles   = 0; // pattern bytes
	bool        placed  = 0; // flag for generation

	QuestPattern() = default;
	
	QuestPattern(const char* name, short ofsRow = 0, short ofsCol = 0, QUEST id = Q_NO, short flag = QP_NO,
		         char botRow = 1, char botCol = 1, char topRow = 1, char topCol = 1):
		name(name), ofsRow(ofsRow), ofsCol(ofsCol), id(id), flag(flag),
		botRow(botRow), botCol(botCol), topRow(topRow), topCol(topCol) {}
	
	QuestPattern(uchar* tiles, short ofsRow = 0, short ofsCol = 0, QUEST id = Q_NO, short flag = QP_NO,
		         char botRow = 1, char botCol = 1, char topRow = 1, char topCol = 1):
		tiles(tiles), ofsRow(ofsRow), ofsCol(ofsCol), id(id), flag(flag),
		botRow(botRow), botCol(botCol), topRow(topRow), topCol(topCol) {}
};

struct UniqueItem
{
	int        uniqId;
	char*      Name;
	int        baseId;
	int        uniqLine;
	int        uniqueSetIndex;
	int        qualityLevel;
	short      effectCount;
	int        GoldValue;
	int        uniqueGraphics;
	BaseEffect Effect[20];
};

struct UniqueSet
{
    char*		Name;
    int		    UniquesCount;
    int         UniquesList[IS_Inventory];
    int		    EffectCount;
    
    struct
    {
        int         Threshold;
        BaseEffect  Effect;
    }           Effects[10];
    
    Item		FakeItemToShowEffect;
};

// sizeof
struct Player 
{
	PLAYER_CUR_ACTION	CurAction;
	char				walkPath[MaxPathStep_25];
	PLAYER_FULL_CLASS	fullClassId;
	PLAYER_COMMAND		destAction;
	int					destParam1;							// next action param1 (map row / object index )
	int					destParam2;							// next action param2 (map col / spell level )
	int					destParam3;							// next action param3
	int					destParam4;							// next action param4
	DUNGEON				dungeon;
	char				dungeon_reserved;
	short				Row;								// world row
	short				Col;								// world col
	short				NextRow;							// player row
	short				NextCol;							// player col
	short				TargetRow;							// target row / next moving pos
	short				TargetCol;							// target col / next moving pos
	short				ownerRow;							// owner row / Эти поля видно только в ии 15 и при получении сетевых сообщений используются. как row и col
	short				ownerCol;							// owner col
	short				oldRow;
	short				oldCol;

	int					xOfs;								// offset x from left of map tile
	int					yOfs;								// offset y from bottom of map tile
	int					xVel;								// current x rate
	int					yVel;								// current y rate
	char				dir;								// current player direction
	char				nextDir;							// Next direction wanted
	int					Appearance;
	short				currentAnimationDelay;
	short				currentAnimationFrameTick;
	short				currentAnimationFramesCount;
	short				currentAnimationFrame;
	short				animWidth;
	short				animWidth2;
	char				UnderArcType;						// EFlag
	short				LightIndex;
	short				lightRadiusIdentificator;
	char				spellIndex;
	char				readySpellType;
	uchar				readySpellTargetType;
	char				nextArrowElement;					// new for Do_RangeAttack
	uchar				altWeaponIndex;
	char				SpellOnCursor;
	char				spellOnCursorType;
	char				invCols;
	char				invRows;
	char				talCols;
	char				talRows;
	char				CurrentSpellIndex;
	char				SpellType;							// TODO: should name spellOrigin
	char				nextMeleeElement;
	short				lastAttacker;
	short				AdvancedUser;
	i64					AvailableSpellMask;					// TODO: add reserve to 65-128 ?
	i64					AvailableSkillMask;															   
	i64					AvailableRelictMask;
	int					activeBuffFlag;
	char				SpellOnFuncKeys[4];				   
	char				SpellTypeOnFuncKeys[4];				   

	uint				runEnabled              : 1, // 1
						meleeAttackBit          : 1, // 2
						rangeAttackBit          : 1, // 3
						IsExists                : 1, // 4
						activeInv               : 1, // 5
						IsBowInHand             : 1, // 6
						IsFlaskInHand           : 1, // 7
						running                 : 1, // 8
						CanBlock                : 1, // 9
						notHittableInTransition : 1, // 10
						lvlChanging             : 1, // 11
						IsPlayerEnemy           : 1, // 12 Automatically initialize to false. Explicitly initialized in On_PLAYER_JOINLEVEL()
						Infravision             : 1, // 13
						hasGreetedByOgden       : 1, // 14
						                        : 18;
	uchar				LightRadius;
	CharName			playerName;
	PLAYER_CLASS		ClassID;
	
	short				CurStrength;						// TODO: change to BaseStat[], CurStat[] arrays
	short				BaseStrength;
	short				CurMagic;
	short				BaseMagic;
	short				CurDexterity;
	short				BaseDexterity;
	short				CurVitality;
	short				BaseVitality;
	short				AvailableLvlPoints;
	
	int					BaseDamage;
	int					BlockBonus;
	int					BaseLife;
	int					MaxBaseLife;
	int					CurLife;
	int					MaxCurLife;
	int					RatioLifeGlobe;
	int					BaseMana;
	int					MaxBaseMana;
	int					CurMana;
	int					MaxCurMana;
	int					RatioManaGlobe;
	short				CharLevel;
	short				controlCharLevel;
	i64					Xp;
	i64					NextLevelUp;
	int					TotalGold;
	short				CursorRow;
	short				CursorCol;
	char				newDirection;
	int					HorizontalNextRowOrSomeSpellData;
	int					HorizontalNextCol;
	int					dx64;
	int					dy64;
	short				currentActionFrame;

	bitset<DungeonExpantionLimit> dungeonVisited;
	short				notHittableFramesAfterTransition;
	int					LoadPlayerAnimationMask;


	Item				OnBodySlots[IS_Inventory];
	Item				InventorySlots[IS_70_40_Inv_Count];
	int					InvItemCount;
	union{																						   
		char			InvUsed[IS_70_40_Inv_Count];
		char			InvUsedAlt[InvRows][InvCols];
	};											   
	Item				TalismanSlots[IS_70_40_Inv_Count];
	int					TalItemCount;
	union{																						   
		char			TalUsed[IS_70_40_Inv_Count];
		char			TalUsedAlt[InvRows][InvCols];
	};													   
	Item				BeltInventory[IS_8_Belt_Count];
	Item				ItemOnCursor;
	
	int					MinDamageFromItem;
	int					MaxDamageFromItem;
	int					ACFromClass;
	int					ItemsAddDamagePercents;
	int					ItemsAddToHitPercents;
	int					ACFromItems;
	int					ItemsAddDamage;
	i64					AvailableChargesMask;
	int					DamageFromEnemy;
	short				allSpellExtraLevel;
	array<TraitId,4>	traits;
	short				armorPiercing;
	short				OilEffectOnCursor;
	char				OpenedTownDoorMask;
	char				DungeonMessageFlags1;
	char				SomeSoundMuteDelay;
	char				DungeonMessageFlags2;
	ushort				CountOfReflectCharges;
	char				allowedDifficulty;
	char				Difficulty;

	short				goldFind;
	short				magicFind;
	short				spellExtraLevel[128];
	short				spellPercent[128];
	int					spellDamageMin[128];
	int					spellDamageMax[128];
	ushort				cooldowns[CD_CAP];

	float				scaleResist;
	short				SpellLevels[128];
	float				resistPercent[8];
	float				resistPercentLimit;
	float				resistPercentMeleeMin;
	float				resistPercentMeleeMax;
	float				resistPercentMeleeMinLimit;
	float				resistPercentMeleeMaxLimit;
	float				resistPercentArrowMin;
	float				resistPercentArrowMax;
	float				resistPercentArrowMinLimit;
	float				resistPercentArrowMaxLimit;
	short				PrevRow; //These 4 are used for better Teleport experience
	short				PrevCol;
	short				PrevCursorX;
	short				PrevCursorY;
	short				elementPercent[16]; // damagePercent  8 reserved
	int					elementMinDamage[16]; // element damage and to hit - AE_ELEMENT
	int					elementMaxDamage[16];
	short				elementToHit[16];
	short				percentStr;
	short				percentDex;
	short				percentMag;
	short				percentVit;
	int					lifeRegen;
	int					manaRegen;
	short				lifeRegenPercent;
	short				manaRegenPercent;
	int					xpGainMin; // xpGain from Item_v25, have to expand in both struct if need
	int					xpGainMax; // also have to update LoadPlayer() offsets
	short				xpGainPercent;

	short				resistMeleeMin;
	short				resistArrowMin;
	short				summonHpPercent;
	short				summonDamagePercent;
	short				summonAcPercent;
	short				summonToHit;
	int					summonHp;
	int					summonDamageMin;
	int					summonDamageMax;
	int					summonAc;
	short				blockChance;
	short				critChance;
	short				critPercent;
	int					critDamageMin;
	int					critDamageMax;

	int					spiciesDamageMin[3]; // TODO: MON_count == 4, check WTF
	int					spiciesDamageMax[3];
	short				spiciesDamagePercent[3];
	short				spiciesToHit[3];
	int					spiciesAc[3];
	short				spiciesAcPercent[3];
	short				spiciesDfe[3];
	short				spiciesDamageTotal[3];

	short				lifePercent;
	short				manaPercent;
	short				weaponPercent[16];
	short				resist[ ET_CAP ];
	short				stunThreshold[ CT_CAP ];
	char				attackSpeed;
	char				castSpeed;
	char				hitRecovery;
	char				lifeStealPercent;
	char				manaStealPercent;
	int					goldAbsFindMin;
	int					goldAbsFindMax;
	short				resistMeleeMax;
	short				resistArrowMax;

	// пока используется как список, использовать как маску (неповторяющиеся эффекты) можно по ненулевому id (либо chance)
	EffectAction		effectAction[200]; // 200 = 20 эффектов * 10 вещей
	// эти сразу используются как маска (по ненулевому id)
	EffectAction		effectActionPercent[EA_CAP]; // не меньше EA_COUNT
	EffectAction		effectActionValue[EA_CAP]; // не меньше EA_COUNT
	bitset<EA_CAP>		effectFlag;

	char				perk[PERKS_MAX_800];
	i64					gameChanger;
	uint				seed2; // for rand()
	int					CurrentSeedRun;
	int					CurrentSeedRun_64_reserved;
	PLAYER_SUBLASS		subclassID;
	PLAYER_SPECIALIZATION specializationID;
	GAME_MODE			gameMode; 
	ushort				AuraIndex[5];
	ushort				AurasValues[5];
	uchar				griswoldLevel;

	char				reserved[5499];

	char*				currentAnimationPointer;
	char*				StayAnimCel;
	char*				WalkAnimCel;
	char*				AttackAnimCel;
	char*				LightningSpellAnimCel;
	char*				FireSpellAnimCel;
	char*				MagicSpellAnimCel;
	char*				GotHitAnimCel;
	char*				DeathAnimCel;
	char*				BlockAnimCel;
	char*				turningAnimCel;

	char*				nothingAnimationsPointers[8];
	int					StayAnimationFrameCount;
	int					AnimPitchStay; // animWidth ?

	char*				walkAnimationsPointers[8];
	int					WalkAnimationFrameCount;
	int					AnimPitchWalk;

	char*				attackAnimationsPointers[8];
	int					AttackAnimationFrameCount;
	int					AnimPitchAttack;
	int					AttackMainFrame;

	char*				lightningSpellAnimationsPointers[8];
	char*				fireSpellAnimationsPointers[8];
	char*				magicSpellAnimationsPointers[8];
	int					SpellAnimationFrameCount;
	int					AnimPitchCast;
	int					SpellAnimationActionFrameIndex;

	char*				gotHitAnimationsPointers[8];
	int					HitRecoveryAnimationFrameCount;
	int					AnimPitchHit;

	char*				deathAnimationsPointers[8];
	int					DeathAnimationFrameCount;
	int					AnimPitchDeath;

	char*				blockAnimationsPointers[8];
	int					BlockAnimationFrameCount;
	int					AnimPitchBlock;

	char*				turningAnimationsPointers[8];
	int					tunringAnimationFrameCount;
	int					AnimPitchTurning;


	char				reservedPointers[856];

	static constexpr StructVersion versions[] = {
		{26, 0x18000 /*98304*/}, {19, 130216}, {18, 130196}, {17, 129296}, {16, 129296}, {15, 107404}, {14, 107400},
		{13, 106328}, {12, 106224}, {11,  73764}, {10,  73760}, { 9,  73752}, { 8,  60824}, { 7,  56984},
		{ 6,  56728}, { 5,  56216}, { 4,  55488}, { 3,  35952}, { 2,  34144}, { 1,  23076}, { 0,  21960}
	}; // 26, 92228
};
static_assert( sizeof(Player) == StructSize<Player>(CurSaveVersion), "sizeof Player and Player::versions[CurVersion].size are not the same");

// sizeof 91
struct Sound
{
	SOUND               id;
	underlying_type_t<SOUND_FLAG> flag;
	char                name[48];
	WAVEFORMATEX        WAVEFORMAT;
	uint                size;
	uint                offset;
	IDirectSoundBuffer* buffer;
	i64                 lastPlayTime;
};

// sizeof 8
struct SoundDup
{
	SOUND               id;
	IDirectSoundBuffer* buffer;
};

// sizeof 8
struct AnimDir
{
	char* data;
	short xOfs;
	short yOfs;
};

// sizeof 0x2C 44
struct MonsterAnim
{
	char*	animPtr;		// 0	0
	AnimDir oriented[8];		// 0x4	4
	int		frameCount;		// 0x24	36
	int		animationDelay;	// 0x28	40
};

// sizeof 0x150 336
struct MonsterSprite
{
	int			baseMonsterIndex;		//	0	0
	uint 		placeFlag;				//	0x4	4 MONSTER_PLACEMENT_FLAG, used only 3 bits, can be reused
	MonsterAnim animation[6];			//	0x8	 8 52 96 140 184 228 // временно отключено
	int			Pitch;			        //	0x130	304
	int			StartOfsX;				//	0x134	308
	int			minHitPoints;			//	0x138	312
	int			maxHitPoints;			//	0x13C	316
	int			hasSecondAttack;		//	0x140	320
	char		firstAtackToHitFrame;	//	0x144	324
	char		deadSpriteNum;				//	0x145	325
	ushort		aligment;				//	0x146	326
	BaseMonster* baseMonsterPtr;		//	0x148	328  // временно отключено
	uchar*		colorTable;				//	0x14C	332 временно отключена сверка из-за странного поведения сверки при FreeMem
};

// 0069DC88
// sizeof 228
struct Monster
{
	int  SpriteIndex;				//   0 MonsterSprites[Monsters_Sprites_Max_Count] )
	int  CurAction;					//   4
	MONSTER_GOAL	goal;			//   8
	ushort ChampionNumber;			//   9 extended for Monsters extension
	char summonOrder;				//  11 unused
	int  goalParam;					//  12
	int  curOrientation;			//  16
	int  mayRetreateTick;			//  20
	int  newBossId;					//  24 0069DCA0 uniqueMonsterIndex + 1, previously unused parameter, use under BossID (old 188)
	int  pathCount;					//  28 
	int  Row;						//  32 0069DCA8 
	int  Col;						//  36 0069DCAC 
	int  NextRow;					//  40
	int  NextCol;					//  44
	int  PrevRow;					//  48
	int  PrevCol;					//  52
	int  DeltaX;					//  56
	int  DeltaY;					//  60
	int  SpeedX;					//  64
	int  SpeedY;					//  68
	int  Orientation;				//  72
	int  TargetIndex;				//  76
	char TargetRow;					//  80
	char TargetCol;					//  81
	short TargetObjectIndex;		//  82 barrel index
	AnimDir* curAnimation;			//  84
	int  AnimationDelayFrameCount;	//  88
	int  AnimationDelayIndex;		//  92
	int  animFrameCount;			//  96
	int  CurFrame;					// 100
	uint acidDotSum;				// 104 reused underArch flag // Accumulated acid missile damage per second (for optimized MP sync), not sure if uint is large enough...
	int	 acidDotDeltaSum;			// 108 reused killed flag // Accumulated acid green bar reduction per second
	union{
		int PrevActionOrRowDeltaOrSpellEffect;					// 0x70
		int PrevAction;
		int RowDelta;
	};
	union{
		int TimerCounterOrColDeltaOrSpellDamage;// 0x74
		int TimerCounter;
		int ColDelta;
	};
	union{
		int ActionRow_ActOrient;	// 120
		int ActionOrient;
	};
	int		NextRowHz;				// 124 
	int		NextColHz;				// 128 
	int		WalkX;					// 132
	int		WalkY;					// 136
	int		WalkframeCounter;		// 140
	int		BaseLife;				// 144
	int		CurrentLife;			// 148
	uchar	ai;						// 152 index of monster AI func ( AI_28_Lazarus for example )
	uchar	intel;					// 153 разумность монстра
	uchar	_unused[2];				// 154 // unused
	int		flag;                   // 156
	uchar	ActivationCounter;		// 160
	ushort	IsSummonedByMonster;	// 161
	char	dotLastAttacker;
	int		acidDot;				// 164 acid damage over time
	int		LastTargetPositionRow;	// 168
	int		LastTargetPositionCol;	// 172
	int		RandomSeedForDrop;		// 176
	int		RandomSeedForAction;	// 180
	ushort	overTime;				// 184 acid time period
	ushort	prevOverTime;			// Unused
	uchar	BossID;					// 188 0069DD44 старый адрес, новый - 0069DCA0 ( .NewBossId +24 )
	uchar	uniqTrans;				// 189
	uchar	udeadNum;				// 190
	uchar	Attackers;				// 191 number of players
	uchar	unused_oldMonsterLevel;	// 192
	uchar	canFly;					// 193 for walk type
	i64		Xp;						// 194
	int		ToHit;					// 196
	int		MinDamage;				// 197 bug in actual TH !!! signed due to SAR (0047AB12 example) must be SHR
	int		MaxDamage;				// 198 bug in actual TH !!! signed due to SAR must be SHR
	int		SecondToHit;			// 199
	int		SecondMinDamage;		// 200 bug in actual TH !!! signed due to SAR must be SHR
	int		SecondMaxDamage;		// 201 bug in actual TH !!! signed due to SAR must be SHR
	int		ArmorClass;				// 202
	MONSTER_RESIST ResistImmune;	// 204
	int		speechIndex;			// 208
	uchar	hisBossIndex;			// 0xD4 212 
	char	isBossPack;				// 213
	uchar	BossPackCount;			// 214
	char	LightIndex;				// 215
	uchar	affixes[4];
	DUNGEON	dungeon;
	short	MonsterLevel;
	char	reserved3;
	const char*	Name;				// 216
	MonsterSprite* SpritePtr;		// 220
	BaseMonster* BasePtr;			// 224
};

struct DSummonStr
{
    uchar spriteIndex;
	uint  summonType;
    int   summonLife;
    int   toHit;
    int   minDamage;
    int   maxDamage;
    int   toHitSecond;
    int   minDamageSecond;
    int   maxDamageSecond;
    int   armorClass;
};

// sizeof 8
struct ScreenPos
{
	short x;
	short y;
};

// sizeof 24
struct DisplayObject
{
	int Left;
	int Top;
	int Width;
	int Heigth;
	int Down;
	int Right;
};

extern DisplayObject InventoryPanelRect;
struct BaseSlot
{
	ITEM_SLOT id;
	short x; // pixel offset from panel
	short y;
	short c; // columns (cell count horizontal)
	short r; // rows (cell count vertical)
	SLOT_TYPE slotType;
	EQUIPPED_LOCATION equipLoc;
	DisplayObject& panel = InventoryPanelRect; // panel
};

struct Slot
{
	ITEM_SLOT id;
	DisplayObject rect;
	BaseSlot base;
	//EQUIPPED_LOCATION equipLoc; // for PutItemToInv ?
	//Item& item; // ?
};

// sizeof 16
struct ItemCell
{
	short x; // left pixel from slot on panel
	short y; // top pixel from slot on panel (was bottom)
	short w; // pixel cell width
	short h; // pixel cell height
	short r; // cell row offset
	short c; // cell column offset
	Slot& slot;
};

// sizeof 8
struct Room
{
	ROOM_TYPE roomType;	
	int       transIndex;	
};

// sizeof 20
struct Chamber 
{
	int  startRow40; // 0 
	int  startCol40; // 4
	int  transIndex; // 8
	int  rowSize40;  //12
	int  colSize40;  //16
};

// sizeof 19
struct ItemInfo19
{
	int		seed;					// 0 
	ushort  dropType;				// 4
	short	baseItemIndex;			// 6 тут используется значение -1 так что нужно знаковое представление
	uchar	magicLevelAndIdentified;// 8
	uchar	curDurability;			// 9
	uchar	baseDurability;			// 10
	uchar	curCharges;				// 11
	uchar	baseCharges;			// 12
	ushort  amount;			// 13
	uint	genVersion;				// 15 ex earInfo
};

// New size is 32
struct ItemInfo32: ItemInfo19
{
	uchar   socketsAdded;
	array<short, SocketMax_6> sockets;
};

// New size is 42
struct ItemInfo42: ItemInfo32
{
	array<short, EnchantMax_5> enchantIndexes;
};

// sizeof 32
struct ItemGet
{
	ushort genVersion;
	ushort saveVersion;
	int dropType;
	i64 seed;
	short quality;
	short magicFind;
	short baseItemIndex;
	short uniqIndex;
	i64	timeStamp;

	void operator=(const struct ItemInfo64&);
};

// sizeof 64
struct ItemInfo64
{
	ushort  genVersion;				// 0 ex earInfo TODO: can be uchar
	ushort  saveVersion;            // 2 TODO: can be uchar
	int		dropType;				// 4
	i64		seed;					// 8 
	short   quality;				// 16 extracted from dropType 6 bits
	short	magicFind;              // 18 extracted from seed 10 bits
	short	baseItemIndex;			// 20 тут используется значение -1 так что нужно знаковое представление
	short	uniqIndex;				// 22 extracted from seed 12 bits
	uchar	magicLevelAndIdentified;// 24 TODO: used only 3 bits (combine with flag)
	int		amount;					// 25 gold quantity, can be used for stacking also
	short	curDurability;			// 29
	short	baseDurability;			// 31
	short	curCharges;				// 33
	short	baseCharges;			// 35
	uchar   socketsAdded;			// 37
	array<short, SocketMax_6> sockets;// 38
	array<short, EnchantMax_5> enchantIndexes;// 50
	QUENCH_AFFIX quenchAffix;        // 60
	uchar	quenchLevel;             // 61
	uchar	difficulty;              // 62 TODO: can be 3 bits (combine with charFlag)
	uchar	charFlag;				 // 63 TODO: move CanGrind and other player dependencies here (there is unused bits in .dropType and .magicLevelAndIdentified also)

	bool operator==(const ItemInfo64&) const;
	bool operator==(const struct Item&) const;
	bool operator==(const struct ItemGet&) const;
	void operator=(const struct Item&);

	// {
	// short toHitPercent;
	// short maxDamage;
	// short requiredStrength;
	// short requiredMagic;
	// short requiredDexterity;
	// short armorClass;
	// }
};

using LastItemInfo = ItemInfo64;

struct /*alignas(4)*/ WeaponSwapItemV16: ItemInfo19 { char align; }; // sizeof 20
struct /*alignas(4)*/ WeaponSwapItemV17: ItemInfo32 {}; // sizeof 32

struct PlayerInfo
{
	uint		signature;					//0		0
	uint		version;					//0		0
};

// sizeof 0x4F2 1266
struct PlayerInfo_TH1: PlayerInfo // TH1 player info
{
	PLAYER_COMMAND NextAction;				//0x8	8
	char		MapRow;						//0x9	9
	char		MapCol;						//0xA	10
	DUNGEON		dungeon;					//0xB	11
	char		Row;						//0xC	12
	char		Col;						//0xD	13
	char		XPos3;						//0xE	14
	char		YPos3;						//0xF	15
	char		CharName[32];				//0x10	16
	uchar		ClassId;					//0x30	48
	uchar		BaseStrength;				//0x31	49
	uchar		BaseMagic;					//0x32	50
	uchar		BaseDexterity;				//0x33	51
	uchar		BaseVitality;				//0x34	52
	uchar		CharLevel;					//0x35	53
	uchar		AvailableLvlPoints;			//0x36	54
	uint		Xp;							//0x37	55
	uint		TotalGold;					//0x3B	59
	uint		BaseLife;					//0x3F	63
	uint		MaxBaseLife;				//0x43	67
	uint		BaseMana;					//0x47	71
	uint		MaxBaseMana;				//0x4B	75
	uchar		BaseSpellLevels[37];		//0x4F	79
	i64			AvailableSpellMask;			//0x74	116
	ItemInfo19	OnbodySlots[7];				//0x7C 	124  
	ItemInfo19	InventorySlots[40];			//0x101	257
	uchar		InvUsed[40];				//0x3F9	1017
	uchar		InvItemCount;				//0x421	1057
	ItemInfo19	BeltInventory[8];			//0x422	1058
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	uchar		tmp1[6];					//0x4BA	1210 align
	ushort		CountOfReflectCharges;		//0x4C2	1218
	TraitId		traits[2];					//0x4C4	1220
	uchar		AdvancedSpellLevels[ 10 ];	//0x4C6	1222 = 1185 + 37
	uchar		tmp3[2];					//0x4D0	1232 align
	uint		allowedDifficulty;			//0x4D2	1234
	uint		Difficulty;					//0x4D6	1238
	uint		affixFlag2;					//0x4DA	1242 affixFlag не сохраняется, только новый, разобраться
	uchar		MoreAdvancedSpellLevels[17]; //0x4DE 1246
	uchar		aligment[3];				//???	??? сюда можно расширить affixFlag2 до 64 бит
};

struct OldPlayerInfo2: PlayerInfo_TH1
{
	ItemInfo19	newbodySlots[3];
};

struct OldPlayerInfo3: OldPlayerInfo2
{
	ItemInfo19	newInvSlots[30];
	uchar		newInvUsed[30];
};

struct OldPlayerInfo4: OldPlayerInfo3
{
	char		            perk[100]; // (<= v16) PERKS_MAX == 100
	PLAYER_SUBLASS          subclassID;
    PLAYER_SPECIALIZATION   specializationID;
};

struct OldPlayerInfo5: OldPlayerInfo4
{
    GAME_MODE   gameMode;
};

struct PlayerInfo_v17: PlayerInfo
{
	PLAYER_COMMAND NextAction;				//0x8	8
	char		MapRow;						//0x9	9
	char		MapCol;						//0xA	10
	DUNGEON		dungeon;					//0xB	11
	char		Row;						//0xC	12
	char		Col;						//0xD	13
	char		XPos3;						//0xE	14
	char		YPos3;						//0xF	15
	char		CharName[32];				//0x10	16
	uchar		ClassId;					//0x30	48
	uchar		BaseStrength;				//0x31	49
	uchar		BaseMagic;					//0x32	50
	uchar		BaseDexterity;				//0x33	51
	uchar		BaseVitality;				//0x34	52
	uchar		CharLevel;					//0x35	53
	uchar		AvailableLvlPoints;			//0x36	54
	uint		Xp;							//0x37	55
	uint		TotalGold;					//0x3B	59
	uint		BaseLife;					//0x3F	63
	uint		MaxBaseLife;				//0x43	67
	uint		BaseMana;					//0x47	71
	uint		MaxBaseMana;				//0x4B	75
	uchar		BaseSpellLevels[37];		//0x4F	79
	i64			AvailableSpellMask;			//0x74	116
	ItemInfo32	OnbodySlots[7];				//0x7C 	124  
	ItemInfo32	InventorySlots[40];			//0x101	257
	uchar		InvUsed[40];				//0x3F9	1017
	uchar		InvItemCount;				//0x421	1057
	ItemInfo32	BeltInventory[8];			//0x422	1058
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	uchar		tmp1[6];					//0x4BA	1210 align
	ushort		CountOfReflectCharges;		//0x4C2	1218
	TraitId		traits[2];					//0x4C4	1220
	uchar		AdvancedSpellLevels[ 10 ];	//0x4C6	1222 = 1185 + 37
	uchar		tmp3[2];					//0x4D0	1232 align
	uint		allowedDifficulty;			//0x4D2	1234
	uint		Difficulty;					//0x4D6	1238
	uint		affixFlag2;					//0x4DA	1242 affixFlag не сохраняется, только новый, разобраться
	uchar		MoreAdvancedSpellLevels[17]; //0x4DE 1246
	uchar		aligment[3];				//???	??? сюда можно расширить affixFlag2 до 64 бит
	ItemInfo32	newbodySlots[3];
	ItemInfo32	newInvSlots[30];
	uchar		newInvUsed[30];
	char		            perk[100]; // (v17) PERKS_MAX == 100
	PLAYER_SUBLASS          subclassID;
	PLAYER_SPECIALIZATION   specializationID;
	GAME_MODE   gameMode;
};

struct PlayerInfo_v18: PlayerInfo
{
	PLAYER_COMMAND NextAction;				//0x8	8
	char		MapRow;						//0x9	9
	char		MapCol;						//0xA	10
	DUNGEON		dungeon;					//0xB	11
	char		Row;						//0xC	12
	char		Col;						//0xD	13
	char		XPos3;						//0xE	14
	char		YPos3;						//0xF	15
	char		CharName[32];				//0x10	16
	uchar		ClassId;					//0x30	48
	short		BaseStrength;				//0x31	49
	short		BaseMagic;					//0x32	50
	short		BaseDexterity;				//0x33	51
	short		BaseVitality;				//0x34	52
	uchar		CharLevel;					//0x35	53
	short		AvailableLvlPoints;			//0x36	54
	uint		Xp;							//0x37	55
	uint		TotalGold;					//0x3B	59
	uint		BaseLife;					//0x3F	63
	uint		MaxBaseLife;				//0x43	67
	uint		BaseMana;					//0x47	71
	uint		MaxBaseMana;				//0x4B	75
	uchar		BaseSpellLevels[37];		//0x4F	79
	i64			AvailableSpellMask;			//0x74	116
	ItemInfo42	OnbodySlots[7];				//0x7C 	124  
	ItemInfo42	InventorySlots[40];			//0x101	257
	uchar		InvUsed[40];				//0x3F9	1017
	uchar		InvItemCount;				//0x421	1057
	ItemInfo42	BeltInventory[8];			//0x422	1058
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	uchar		tmp1[6];					//0x4BA	1210 align
	ushort		CountOfReflectCharges;		//0x4C2	1218
	TraitId		traits[2];					//0x4C4	1220
	uchar		AdvancedSpellLevels[10];	//0x4C6	1222 = 1185 + 37
	uchar		tmp3[2];					//0x4D0	1232 align
	uint		allowedDifficulty;			//0x4D2	1234
	uint		Difficulty;					//0x4D6	1238
	uint		affixFlag2;					//0x4DA	1242 affixFlag не сохраняется, только новый, разобраться
	uchar		MoreAdvancedSpellLevels[17]; //0x4DE 1246
	uchar		aligment[3];				//???	??? сюда можно расширить affixFlag2 до 64 бит
	ItemInfo42	newbodySlots[3];
	ItemInfo42	newInvSlots[30];
	uchar		newInvUsed[30];
	char		            perk[PERKS_MAX_800];
	__int64					gameChanger;
	char					gameChangerReserved[188];
	int						CurrentSeedRun;
	PLAYER_SUBLASS          subclassID;
	PLAYER_SPECIALIZATION   specializationID;
	GAME_MODE   gameMode;
};

struct PlayerInfo_v21: PlayerInfo
{
	PLAYER_COMMAND NextAction;
	char		MapRow;
	char		MapCol;
	DUNGEON		dungeon;
	char		Row;
	char		Col;
	char		XPos3;
	char		YPos3;
	char		CharName[32];
	uchar		ClassId;
	short		BaseStrength;
	short		BaseMagic;
	short		BaseDexterity;
	short		BaseVitality;
	short		CharLevel;
	short		AvailableLvlPoints;
	i64			Xp;
	uint		TotalGold;
	uint		BaseLife;
	uint		MaxBaseLife;
	uint		BaseMana;
	uint		MaxBaseMana;
	uchar		BaseSpellLevels[37];
	i64			AvailableSpellMask;
	ItemInfo42	OnbodySlots[7];
	ItemInfo42	InventorySlots[40];
	uchar		InvUsed[40];
	uchar		InvItemCount;
	ItemInfo42	BeltInventory[8];
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	ushort		CountOfReflectCharges;
	TraitId		traits[2];
	uchar		AdvancedSpellLevels[10];
	uint		allowedDifficulty;
	uint		Difficulty;
	uint		affixFlag2;					//affixFlag не сохраняется, только новый, разобраться
	uchar		MoreAdvancedSpellLevels[17];
	ItemInfo42	newbodySlots[3];
	ItemInfo42	newInvSlots[30];
	uchar		newInvUsed[30];
	char		            perk[PERKS_MAX_800];
	__int64					gameChanger;
	char					gameChangerReserved[188];
	int						CurrentSeedRun;
	PLAYER_SUBLASS          subclassID;
	PLAYER_SPECIALIZATION   specializationID;
	GAME_MODE   gameMode;
};

struct PlayerInfo_v22: PlayerInfo
{
	PLAYER_COMMAND NextAction;
	uchar		state_reserved;
	char		MapRow;
	char		mapRow_reserved;
	char		MapCol;
	char		mapCol_reserved;
	DUNGEON		dungeon;
	uchar		dungeon_reserved;
	char		Row;
	char		row_reserved;
	char		Col;
	char		col_reserved;
	char		XPos3;
	char		xPos3_reserved;
	char		YPos3;
	char		yPos3_reserved;
	char		CharName[32];
	uchar		ClassId;
	PLAYER_SUBLASS          subclassID;
	PLAYER_SPECIALIZATION   specializationID;
	short		BaseStrength;
	short		BaseMagic;
	short		BaseDexterity;
	short		BaseVitality;
	short		CharLevel;
	short		AvailableLvlPoints;
	i64			Xp;
	uint		TotalGold;
	uint		BaseLife;
	uint		MaxBaseLife;
	uint		BaseMana;
	uint		MaxBaseMana;
	uchar		SpellLevels[128];
	i64			AvailableSpellMask;
	i64			AvailableSpellMask_64_128;
	ItemInfo64	OnBodySlots[10];
	ItemInfo64	InventorySlots[70];
	uchar		InvUsed[70];
	uchar		InvItemCount;
	ItemInfo64	BeltInventory[8];
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	ushort		CountOfReflectCharges;
	array<TraitId,4> traits;
	uchar		traits_reserved[4];
	uint		allowedDifficulty; // pDiabloKillLevel player killed Diablo at what level
	uint		Difficulty;
	uint		reserved[2];
	char		perk[PERKS_MAX_800];
	i64			gameChanger;
	i64			gameChangerReserved;
	int			CurrentSeedRun;
	int			CurrentSeedRun_64_reserved;
	GAME_MODE   gameMode;
};

struct PlayerInfo_v26: PlayerInfo
{
	PLAYER_COMMAND NextAction;
	uchar		state_reserved;
	short		MapRow;
	short		MapCol;
	DUNGEON		dungeon;
	uchar		dungeon_reserved;
	short		Row;
	short		Col;
	short		XPos3;
	short		YPos3;
	char		CharName[32];
	uchar		ClassId;
	PLAYER_SUBLASS          subclassID;
	PLAYER_SPECIALIZATION   specializationID;
	short		BaseStrength;
	short		BaseMagic;
	short		BaseDexterity;
	short		BaseVitality;
	short		CharLevel;
	short		AvailableLvlPoints;
	i64			Xp;
	uint		TotalGold;
	uint		BaseLife;
	uint		MaxBaseLife;
	uint		BaseMana;
	uint		MaxBaseMana;
	uchar		SpellLevels[128];
	i64			AvailableSpellMask;
	i64			AvailableSpellMask_64_128;
	ItemInfo64	OnBodySlots[17];
	ItemInfo64	InventorySlots[70];
	uchar		InvUsed[70];
	uchar		InvItemCount;
	ItemInfo64	BeltInventory[8];
	ItemInfo64	TalismanSlots[70];
	uchar		TalUsed[70];
	uchar		TalItemCount;
	char		UberDialogKilled;			// for nightmare & ironman
	uchar		altWeaponIndex;
	ushort		CountOfReflectCharges;
	array<TraitId,4> traits;
	uchar		traits_reserved[4];
	uint		allowedDifficulty; // pDiabloKillLevel player killed Diablo at what level
	uint		Difficulty;
	uchar		griswoldLevel;
	uchar		reserved[7];
	char		perk[PERKS_MAX_800];
	i64			gameChanger;
	i64			gameChangerReserved;
	int			CurrentSeedRun;
	int			CurrentSeedRun_64_reserved;
	GAME_MODE   gameMode;

	static constexpr StructVersion versions[] = {
		{26, 11798}
	};
};

using LastPlayerInfo = PlayerInfo_v26;
static_assert( sizeof(LastPlayerInfo) == StructSize<LastPlayerInfo>(CurSaveVersion), "sizeof LastPlayerInfo and LastPlayerInfo::versions[CurVersion].size are not the same");

// sizeof 4
struct TileID
{
	ushort  Left;
	ushort  Right;
};

// sizeof 32
union TileColumn
{
	TileID Layer[8];
	ushort tile[16];
};

// sizeof 20
struct Button
{
	int    id;
	char*  name;
	char*  keyName;
	int    x;
	int    y;
	int    w; // width
	int    h; // height
	int    multiPlayer; // 1 - active in single, 2 - active in multi, 3 - active in sinlgle and multi
	bool   mainMenu;
	void   (*action)();
	char** cel;    // CEL file handler
	int    pic[2]; // frame numbers, pic[0] - unpressed, pic[1] - pressed
	int    yPicOfs; // y draw frame offset
	int    gameMode;
	bool   active;
	bool   pressed;
};

// sizeof 16
struct StatUpButton
{
	int  X;			// 0
	int  Y;			// 4
	int  Width;		// 8
	int  Height;	// 12
};

// sizeof 24
struct Portal
{
	int  open;
	int  row;
	int  col;
	DUNGEON dungeon;
	uchar dungeon_reserved;
	uchar unused[10];
};
static_assert( sizeof(Portal) == 24, "sizeof Portal != 24, it stored in save file");

// sizeof 144
struct PlayerStatusMessage
{
	i64		messageTime;
	uchar	playerIndex;
	char	str[144];
};

// sizeof 44
struct BaseObject
{
	union{ char EndMarker;
	char RegularObject; };			
	BASE_OBSECT_SPRITE SpriteIndex;	
	char minDungeonLevel;			
	char maxDungeonLevel;			
	uchar DungeonType; // unused
	ROOM_TYPE roomType;		
	QUEST QuestIndex;				
	uchar field_4_4;				
	int  hasAnimation;				
	int  AnimationDelayFrameCount;	
	int  FramesCount;				
	int  Width;						
	int  isBlockWalking;			
	int  objectAllowWalking;		
	int  field_20;					
	uchar destructable;				
	uchar selectable;				
	uchar field_24_3;				
	uchar field_24_4;				
	int  CanBeTrappedWithWallTrap;	
};

// sizeof 120
struct Object // Struct120Bytes
{ 
	BASE_OBJECT  BaseObjectIndex;	// 0
	int  Row;						// 4
	int  Col;						// 8
	int  field_C;					// 12
	int  playAnimation;				// 16
	char*CelFilePtr;				// 20 free 
	int  AnimationDelayFrameCount;  // 24
	int  AnimationDelayIndex;		// 28
	int  FramesCount;				// 32
	int  FrameIndex;				// 36
	int  Width;						// 40
	int  OffsetX;					// 44
	int  deletedObject;				// 48
	char destructable;				/*alignas(4)*/ char align1[3];	// 52
	int  isBlockWalking;			// 56
	int  objectAllowWalking;		// 60
	uchar selectable;				/*alignas(4)*/ char align2[3];	// 64
	int  InAction;					// 68
	int  IsTrapped;					// 72
	int  isDoor;					// 76
	int  lightIndex;				// 80
	int  Seed;						// 84
	union {	struct { // попробовать пронумеровать структуры, если больше одной неименованной структуры в одном скопе - ассист с ума сходит
		int  TrapedObjectRow;		// 88
		int  TrapedObjectCol;		// 92
		int  TrapSpellIndex; 		// 96
		int  OpenState2;	 		// 100
	}; struct {
		int  ShrineIndex;		// 88 // проверить пересечения с TrapedObjectRow (выше)
		int  MaybeSpeechIndex;		// 92 // проверить пересечения с TrapedObjectCol (выше)
		int  BookNameIndex;			// 96
		int  OpenState;				// 100 0x64
	}; struct {
		int  startRow40;			// 88
		int  startCol40;			// 92
		int  endRow40;				// 96
		int  endCol40;				// 100
	}; };
	int  readyState;				// 104
	int  maybeFrameIndex;			// 108
	int  SpeechIndex;				// 112
	int  bookIndex;					// 116
};

// sizeof 0xEC 236
struct MissileSprite
{
	MISSILE_SPRITE	missileSpriteIndex;
	char	animDirectionsCount;
	char*	name;
	int	fileFlag;
	short	animDelay[ANIM_DIRS_16];	// anim delay amount
	short	animFrames[ANIM_DIRS_16];	// number of anim frames
	short	animWidth[ANIM_DIRS_16];	// anim width  (pitch ?)
	short	animOffset[ANIM_DIRS_16];	// anim width2 (offsetX ?) 
	char*	anim[ANIM_DIRS_16];
};

// sizeof 12
struct MonsterFrameSpeed
{
	int  Vertical;  // 0
	int  Diagonal;  // 4
	int  Horizontal;// 8
};

// sizeof 52
struct PathStep 
{
	char		value0;		//  0
	char		value1;		//  1
	char		value2;		//  2
	char		align;		//  3
	int			row;		//  4
	int			col;		//  8
	PathStep*	prevPtr;	// 12 // пока отключим сверку возможно рекурсивных указателей
	PathStep*	subSteps[8];// 16 // а вообще нада таки сделать поиск и сравнение
	PathStep*	nextPtr;	// 48 // индексов в соотвествующих массивах вместо рекурсивной раскрутки
};

// sizeof 52
struct Light
{
	int row;		//  0
	int col;		//  4
	int radius;		//  8
	int id;			// 12
	int off;		// 16
	int unflag;		// 20
	int neg;		// 24
	int prevRow;	// 28
	int prevCol;	// 32
	int prevRadius;	// 36
	int deltaRow;	// 40
	int deltaCol;	// 44
	int flag;		// 48
};

// sizeof 11
struct ClassProperty
{
	uchar AngryStayFrames;   // 0
	uchar AttackFrames;      // 1
	uchar AngryWalkFrames;   // 2
	uchar BlockFrames;       // 3
	uchar DeathFrames;       // 4
	uchar SpellFrames;       // 5
	uchar HitFrames;         // 6
	uchar StayFrames;        // 7
	uchar WalkFrames;        // 8
	uchar AttackActionIndex; // 9
	uchar SpellActionIndex;  // 10
};

// sizeof 12
struct ClassVelocity
{
	int	horVel;// 0
	int	xVel;  // 4
	int	yVel;  // 8
};

// sizeof 4
struct ClassStartingStat
{
	ushort loWordOfIntStat;  // Must be int [?,,,,,,]
	ushort hiWordOfIntStat;  // Must be zero [?,,,,,,]
};

// sizeof 4
struct MapArea
{
	char  StartRow;		//0
	char  StartCol;		//1
	char  EndRow;		//2
	char  EndCol;		//3
};

// sizeof 0x9C 156
struct TownersDialogRow 
{
	int		position;			// 0
	int		verticalIndent;		// 4
	char	text[128];			// 8
	int		isAligned;			// 136
	uchar	color;				/*alignas(4)*/char align[3];	// 140
	int		isLine;				// 144
	int		selectable;			// 148
	int		price;				// 152
};

// sizeof 0x28 40
struct CObjectUnderCursor
{
	int  ItemHeight;        //  0
	int  InventoryCellID;	//  4
	int  ItemWidth;         //  8
	int  ItemID;            // 12
	int  ObjectIndex;       // 16
	int  playerIndex;       // 20
	int  Row;               // 24 знаковое, есть сравнение >= 0
	int  Col;               // 28 знаковое, есть сравнение >= 0
	int  MonsterID;			// 32
	uint GraphicsID;		// 36
	int UsedItemSeed;
	int UsedItemBase;
	bool IsTradeService;
	int GroundRow;			// for wall casting
	int GroundCol;			// for wall casting
};

// sizeof 48
struct DeadStruct
{
	AnimDir oriented[8];
	int  deadFrame;
	int  deadWidth;
	int  deadWidth2;
	char deadTrans;
	int  baseMonsterIndex;
};

// sizeof 3
struct TownerQuestInfo
{
	uchar questNumber;	// 0
	uchar speechIndex;	// 1
	uchar hasSpeech;	// 2
};

// sizeof 256
struct Towner
{
	int		field_0;				// 0
	int		TownerIdentificator;	// 4
	int		Row;					// 8
	int		Col;					// 12
	int		field_10;				// 16
	int		field_14;				// 20
	int		field_18;				// 24
	int		field_1C;				// 28
	int		field_20;				// 32
	char*	stayAnim;				// 36 // пока отключено из-за free()
	int		DelayFrameCount;		// 40
	int		CurDelayFrame;			// 44
	int		StayingFrameCount;		// 48
	int		CurFrame;				// 52
	int		FramesOrderCurrentIndex;// 56
	char	FramesOrderTypeIndex;	/*alignas(4)*/ char align[3];	// 60
	int		animWidth;				// 64
	int		offsetX;				// 68
	int		reserved;				// 72
	int		field_4C;				// 76
	int		isThisTownerSpeakingNow;// 80
	int		field_54;				// 84
	int		IsTownerActionSelected;	// 88 // TODO: раньше дооптимизировал до того, что от использования избавился. Надо вернуть
	TownerQuestInfo QuestInfo[24];	// 92 // TODO: check for need, unused originally
	int		seed;								// 164
	int		indexOfPlayerWhichSpeaksTownsman;	// 168
	int		field_AC;							// 172
	int		field_B0;							// 176
	int		field_B4;							// 180
	char	Name[32];							// 184
	char*	animationFrameArray[8];				// 216
	int		animationFrameCount;				// 248 // пока отключено из-за free()
	char*	spritePtr;							// 252 // пока отключено из-за free()
};

// sizeof 4
struct NetParam
{
	uchar  Param1;  // 0
	uchar  Param2;  // 1
	uchar  Param3;  // 2
	uchar  Param4;  // 3
};

// sizeof 28
struct SoundCache
{
	uint	fileSize;	// 0
	uint	seekPos;	// 4
	uint	cacheSize;	// 8
	uint	readed;		// 12
	uint	remain;		// 16
	char*	ptr;		// 20
	HANDLE	handle;		// 24
};

// sizeof 8
struct  WaveTag
{
	uint  Tag;// 0
	uint Size;// 4
};

// sizeof 8
struct  WaveTagInfo
{
	uint Size;	// 0
	uint Pos;	// 4
};

// sizeof 12
struct RiffChunkDiscriptor 
{
	int ChunkId;	// 0
	int ChunkSize;	// 4
	int Format;		// 8
};

// sizeof 13
struct Warp
{
	int  Row;			// 0
	int  Col;			// 4
	int  GameMessage;	// 8
	DUNGEON destDungeon;// 12
};

// sizeof 20
struct CataHall 
{
	int	 startRow;				// 0
	int	 startCol;				// 4
	int	 endRow;				// 8
	int	 endCol;				// 12
	uint motherCataHallIndex;	// 16
};

// sizeof 7
struct StrangeCataPattern 
{
	uchar curTileType;	// 0
	uchar upTileType;	// 1
	uchar leftTileType;	// 2
	uchar rightTileType;// 3
	uchar newUpTile;	// 4
	uchar newLeftTile;	// 5
	uchar newRightTile;	// 6
};

// sizeof 24
struct CataPassageQueue 
{
	int					firstDoorRow;		// 0
	int					firstDoorCol;		// 4
	int					secondDoorRow;		// 8
	int					secondDoorCol;		// 12
	uint				startOrientation;	// 16
	CataPassageQueue*	nextElementPtr;		// 20
};

struct Spell
{
	char           Index;
	short          Icon;
	ELEMENTAL_TYPE Element;
	uchar          InitialManaToCast;
	char           animType; // SPELL_ANIM
	char*          Name;
	char*          SkillNamePtr;
	int            BookQuality;
	int            ItemQuality;
	bool           targeted;
	int            IsEnabledInTown;
	int            RequiredMagic;
	int            RequiredClvl;
	uint           RequiredClassMask;
	int            SoundType;
	MISSILE        Effect[3];
	uchar          ManaDecrementOnLevel;
	uchar          MinimalMana;
	ushort         BookPic;
	int            MinItemCharges;
	int            MaxItemCharges;
	int            BookCost;
	int            PriceMultiplier;
};

// sizeof 176
struct Missile // 00697A58
{
	int  BaseMissileIndex;					// 0
	int  Row;								// 4
	int  Col;								// 8
	int  ColDisplacement;					// 12 wtf?! row always did place before col! Maybe error
	int  RowDisplacement;					// 16 offset
	int  DeltaY;							// 20 velocity
	int  DeltaX;							// 24
	int  CasterRow;							// 28
	int  CasterCol;							// 32
	int  WholeDeltaY;						// 36
	int  WholeDeltaX;						// 40
	int  direction;							// 44
	int  SpellLevel;						// 48
	int  IsDeleted;							// 52	00697A8C
	MISSILE_SPRITE spriteIndex;				// 56
	uchar drawDelay;
	char spellIndex;						// 58 player spell num (0 - arrow/melee elemental explosion, or not player spell )
	uchar timeFrame;
	int  spriteFileFlags;			// 60 flags
	char* curAnimationPtr;					// 64 free
	int  animationDelayFrameCount;			// 68
	int  framesCount;						// 72	697AA0
	int  animWidth;						// 76
	int  animOffset;					// 80
	int  AnimationDelayIndex;				// 84
	int  FrameStep;							// 88
	int  FrameIndex;						// 92
	int  HasActSprite;						// 96
	int  flag;								// 100 MISSILE_FLAG
	int  MayBeIsExploding;					// 104 pre flag
	int  uniqTrans;							// 108
	int  TimeToLive;						// 112 (70h)	00697AC8
	int  CasterIndex;						// 116 0x74 (74h) -1 - trap ? 00697ACC
	int  CasterType;						// 120 -1 shrine, 0 player, 1 monster and trap, 2 walls fragment
	int  Damage;							// 124
	int  isHitTarget;						// 128 can be changed to bit flag, and reuse int
	int  Distance;							// 132
	int  LightIndex;						// 136
	int  chargedBoltChangeDirectionParam;	// 140 rnd
	int  value[8];							// 144 service values
	static constexpr int zeroValue[8] = {0};
};

typedef void( __fastcall *CastFunc )(int baseMissileIndex, int casterRow, int casterCol, int targetRow, int targetCol, int casterDirection, int casterType, int casterIndex, int damage);
typedef void( __fastcall *ActFunc )(int);

// sizeof 28
struct BaseMissile // 0x004A7968
{
	uint			BaseMissileIndex;	// should be MISSILE ?
	CastFunc		castFunc;			//
	ActFunc			actFunc;			//
	int				HasSprite;			//
	MISSILE_TYPE	EffectType;			// missile origin: 0 - weapon/arrow, 1 - spells, 2 - other/explosion
	ELEMENTAL_TYPE	DamageType;			// 0 - Physical, 1 - Fire, 2 - Lightning, 3 - Arcan, 4 - Acid, 5 - Holy, 6 - Cold
	MISSILE_SPRITE	Image;				// image  (FF - None)
	int				AfterEffect;		//
	SOUND			CastSound;			//
	SOUND			ActSound;			//
};

// sizeof 12
struct Menu
{
	uint  Flags;						// 0
	char* NamePtr;						// 4
	void (__fastcall *FuncPtr)(int,int);// 8
};

// sizeof 8
struct AppendRec
{
	uint   checkValue;
	uchar  version;
	uchar  lastBlockSize;
	ushort reserved;
};

// sizeof 136
struct KeyRec
{
	ushort ideakey[3][8];
	uchar  deskey[3][8];
	uchar  shaInitVect[64];
};

// sizeof 92
struct ShaInfo
{
	int digest[5];
	int countLo;
	int countHi;
	int data[16];
};

// sizeof 8
struct RandomizeQuestsGroup
{
	int*	questsIndexesArray;	// 0
	char	questsInGroup;		// 4
	char	countInOneGame;		// 5
	short	align;				// 6
};

// sizeof 3
struct PalEntry3
{
	uchar Red;
	uchar Green;
	uchar Blue;
};

// sizeof 12
struct CompleteSetEffect
{
	int setName;	// 0
	BaseEffect effect;
};

struct PlayerStat
{
	int  Strength;
	int  Magic;
	int  Dexterity;
	int  Vitality;
};

struct GameTemplate
{
	uint	seed;	// 0
	uchar	difficulty; // 4
	uchar	unused1;
	uchar	unused2;
	uchar	unused3;
};

// ---- th2 new structs ------------------------------------------------------------

// th2
struct DamageMultipliers
{
	bool StrengthMultiplier;
	bool DexterityMultiplier;
	int  Denominator;
};

// th2
struct StartItem
{
	int LeftHandItemIndex;
	int RightHandItemIndex;
	int InventoryItemIndex;
	int BeltSlot1ItemIndex;
	int BeltSlot2ItemIndex;
};

// th2
struct PlayerClass
{
	const char* NamePtr;
	PlayerStat StartStat;
	//PlayerStat MaxStat;
	int BlockBonus;
	int NumberOfSubSteps;
	int SkillIndex;
	int LifeBase;
	int ManaBase;
	DamageMultipliers BaseDamageMultipliers;
	int Appearance;
	StartItem StartItems; // hardcoded in TH
	int Voice[5];
};

// th2---expanded structure for character parameters, including subclass data
struct FullPlayerClass
{
	const char* name;
	PlayerStat StartStat;
	PlayerStat MaxStat;
	StartItem StartItems;
};

// надписи (в городе) на знаках, могильных камнях и просто по координатам областей
// sizeof 24
struct Sign
{
	char  messageType;		// -1 = end marker, 1 = info panel text, 2 = float message, 0 = no message
	char  objectType;		// 1 = tile or coordinate (FineMap)
	short objectNum;		// if not 0, check for object(tile) type
	short objectNum2;		// for two cell tiles object, or 0
	int  left;				// if not 0, check for cell coordinate X
	int  top;				// if not 0, check for cell coordinate Y
	int  right;			// if not 0, check for square coordinate X right
	int  bottom;			// if not 0, check for square coordinate Y bottom
	char  outline;			// 0 = no, 1 = white
	char  textColor;
	char* text;
	char* sound;			// 0 = no
	void( *actionFunc )();	// 0 = no
};

// pklib types
struct TDcmpStruct; // Struct3051Dwords
struct TCmpStruct;

typedef int( __stdcall * ProcHandler )(HWND, UINT, WPARAM, LPARAM);

// storm types

// sizeof 276
struct MPQ
{
	MPQ* Next;				// 0	// Next archive
	MPQ* Prev;				// 4	// Previous archive
	char FileName[ 260 ];	// 8	// Opened archive file name [MAX_PATH]
	HANDLE        File;		// 268	// File handle
	uint		  Priority;	// 272	// Priority of the archive
};

// Network provider capabilities ( taken from dinput ? )
// sizeof 36
struct CAPS
{
	DWORD Size;                 // 0   Size of this structure  // sizeof(CAPS)
	DWORD Flags;                // 4   Some flags?
	DWORD MaxMessageSize;       // 8   Size of the packet buffer, must be beteen 128 and 512
	DWORD MaxQueueSize;         // 12  Unknown
	DWORD MaxPlayers;           // 16  Displayed player count in the mode selection list
	DWORD BytesSec;             // 20  some kind of timeout or timer related
	DWORD LatencyMs;		    // 24  ... latency?
	DWORD DefaultTurnsSec;      // 28  ?? the number of players that can participate, must be between 1 and 20
	DWORD DefaultTurnsInTransit;// 32  ?? the number of calls before data is sent over the network // between 2 and 8; single player is set to 1
};

// Network provider structures
// sizeof 44
struct client_info // original hellfire storm.dll
{
	DWORD Size;					// 0
	char* Name;					// 4
	char* Version;				// 8
	DWORD Product;				// 12
	DWORD Verbyte;				// 16
	DWORD gameCriteriaData;		// 20
	DWORD MaxPlayers;			// 24
	GameTemplate* gameTemplate;	// 28
	DWORD gameTemplateSize;		// 32
	DWORD Unk9;					// 36
	DWORD Unk10;				// 42
};

// Network provider structures
// sizeof 60
struct client_info2: client_info // version 2 in actual storm.dll
{
	// version 2 new fields
	char  *pszCdKey;
	char  *pszCdOwner;
	DWORD dwIsShareware;
	DWORD dwLangId;
};

// sizeof 12
struct user_info // original hellfire storm.dll
{
	DWORD Size;					// 0
	char* PlayerName;			// 4
	char* playerDescription;	// 8
};

// sizeof 16
struct user_info2: user_info // version 2 in actual storm.dll
{
	// version 2 new fields
	DWORD dwUnknown;
};

// sizeof 52
struct battle_info // original hellfire storm.dll
{
	DWORD Size;							// 0	52
	DWORD UnkType;						// 4
	HWND  FrameWnd;						// 8
	void* BattleGetResource;			// 12
	void* BattleGetErrorString;			// 16
	void* BattleMakeCreateGameDialog;	// 20
	void* BattleUpdateIcons;			// 24
	DWORD Unk_07;						// 28
	void* BattleErrorDialog;			// 32
	void* BattlePlaySound;				// 36
	void* BattleConnectionProgress;		// 40
	void* BattleGetCursorLink;			// 44
	void* Unk_12;						// 48
};

// sizeof 92
struct battle_info2: battle_info // version 2 in actual storm.dll
{
	// version 2 new fields
	void  *pfnUnk_13;
	DWORD dwUnk_14;
	void  *pfnBattleMakeProfileDialog;
	char  *pszProfileStrings;
	void  *pfnBattleDrawProfileInfo;
	void  *pfnUnk_18;
	DWORD dwUnk_19;
	void  *pfnUnk_20;
	void  *pfnUnk_21;
	void  *pfnBattleSetLeagueName;
};

// sizeof 20
struct module_info
{
	DWORD dwSize;			// 0
	char* VersionString;	// 4
	char* ModuleName;		// 8
	char* MainArchive;		// 12	
	char* PatchArchive;		// 16
};

// sizeof 300
struct SGame
{
	DWORD     dwIndex;					// 0
	DWORD     dwGameState;				// 4
	DWORD     dwUnk_08;					// 8
	SOCKADDR  saHost;					// 12
	DWORD     dwUnk_1C;					// 28
	DWORD     dwTimer;					// 32
	DWORD     dwUnk_24;					// 46
	char      szGameName[ 128 ];		// 40
	char      szGameStatString[ 128 ];	// 168
	SGame     *pNext;					// 296
};

// sizeof 316 // last version (storm 1611) sizeof 572
struct SGame2
{
	DWORD     dwIndex;					// 0
	DWORD     dwGameState;				// 4
	DWORD     dwUnk_08;					// 8
	SOCKADDR  saHost;					// 12
	DWORD     dwUnk_1C;					// 28
	DWORD     dwTimer;					// 32
	DWORD     dwUnk_24;					// 46
	char      szGameName[ 128 ];		// 40
	char      szGameStatString[ 128 ];	// 168
	SGame     *pNext;					// 296
	// new fields
	void      *pExtra;					// 300
	DWORD     dwExtraBytes;				// 304
	DWORD     dwProduct;				// 308
	DWORD     dwVersion;				// 312
};

// sizeof 16
struct CursorLink
{
	HCURSOR hcursor;
	int anonymous_1;
	int anonymous_2;
	int anonymous_3;
};

// sizeof 48
struct BattleIcon
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int color;
	HWND hwnd;
	HDC hdc;
	RECT rect;
	int field_2C;
};

// sizeof 16
struct TransSize
{
	int field_0;
	int width;
	int height;
	int size;
};

// sizeof 16
struct Provider_info
{
	int size;			// 0
	int provider;		// 4
	int maxPlayers;		// 8
	int field_C;		// 12
};

// sizeof 20
struct SProvider
{
	int field_0;		// 0
	int flag;			// 4
	int field_8;		// 8
	int field_C;		// 12
	int maxPlayers;		// 16
};

// sizeof 1288
struct DiabloFont
{
	uchar spaceWidth;		// 0
	uchar height;			// 1
	uchar charWidth[256];		// 2
	short unused;			// 258
	HANDLE trans[256];		// 260
	int fontLoaded;			// 1284
};

// sizeof 272
struct ProviderDesc
{
	ProviderDesc* next;		// 0
	int maybeIpx;			// 4
	int provider;			// 8
	int maxPlayers;			// 12
	char text1[128];		// 16
	char requirements[128];	// 144
};

// sizeof 264
struct ModemGame
{
	ModemGame* next;					// 0
	uint	index;						// 4
	char	szGameName[ 128 ];			// 8
	char	szGameStatString[ 128 ];	// 136
};

// sizeof 268
struct IpxGame
{
	IpxGame* next;						// 0
	uint	index;						// 4
	uint	state;						// 8
	char	szGameName[ 128 ];			// 12
	char	szGameStatString[ 128 ];	// 140
};

template<class T> struct Win
{
	uchar* surface;
	SIZE   size;
	T      val;
	char   text[ 256 ];
	
	static T Val(HWND hwnd)
	{ 
		auto win = (Win<T>*) GetWindowLongA(hwnd, GWL_USERDATA);
		return win ? win->val : 0;
	};
};

// sizeof 272
struct ModemWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	ModemGame* modemGame;		// 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct TextWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	ProviderDesc* providerDesc;	// 12
	char text[256];				// 16
};

// sizeof 272
struct IpxWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	IpxGame* ipxGame;			// 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct HeroWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	CharSaveInfo* saveInfo;		// 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct TraitWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	void* traitIndex;		    // 12
	char text[ 256 ];			// 16
};

// sizeof 272
struct GameModeWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	void* gameModeIndex;		// 12
	char text[ 256 ];			// 16
};

struct GameChangerWin
{
	uchar* surface;				// 0
	SIZE size;					// 4
	uint gameChangerIndex;		// 12
	char text[256];			// 16
};

// sizeof 60
struct ScrollWin
{
	int scrollPressed;	// 0
	uchar* surface;		// 4
	SIZE size;			// 8
	uchar* bgSprite;	// 16
	SIZE bgSize;		// 20
	uchar* thumbSprite; // 28
	SIZE thumbSize;		// 32
	uchar* arrowSprite;	// 40
	SIZE arrowSize;		// 44
	int lineCount;		// 52
	int selectedIndex;	// 56
};

// sizeof 12
struct storm_head
{
	WORD wChecksum;		// 0
	WORD wLength;		// 2
	WORD wSent;			// 4
	WORD wReceived;		// 6
	uchar bCommandClass;	// 8
	uchar bCommandType;	// 9
	uchar bPlayerId;		// 10
	uchar bFlags;		// 11
};

// Event structure
// sizeof 16
struct S_EVT
{
	uint	flags;		// 0
	int		playerIndex;// 4
	void*	data;		// 8
	uint	dataSize;	// 12
};

struct vec4 { float x, y, z, w; };
struct vec3 { float x, y, z; };
struct vec2 { float x, y; };

struct line_vertex
{
	vec4 pos;// transformed(screen space) position for the vertex
	uint color;	// vertex color
};

class CombatText {
public:
	string text;
	int color;
	int iterations;
	int angle;//0-360
	int posX, posY;
	bool showOnCenter;
	int callerID;
	i64 value;
	string description;

	void doStuff(string s, int c, int px, int py, bool center,int cc, string desc,i64 v) {
		text = s;
		color = c;
		iterations = 0;
		angle = -60 + (rand() % 121) + 180 * (rand() % 2); //angle = 60 + rand() % 61;
		showOnCenter = center;
		posX = px;
		posY = py;
		callerID = cc;
		description = desc;
		value = v;
	}

	CombatText(string s, int c, int px, int py, bool center,int cc, string desc, i64 v) {
		doStuff(s, c, px, py, center,cc,desc,v);
	}

	CombatText(string s, int c, string desc) {
		doStuff(s, c, 0,0,true,-1,desc,-1);
	}

	void IncreaseIterations() { iterations++; }
};

using MissileIndexT = int16_t;

struct PerkReq { PERKS perk; int level; };
struct SpellReq { PLAYER_SPELL spell; int level; };

struct PerkLevel
{
	int charLevel;
	uint requiredClassMask;
	PerkReq perkReq[4];
	SpellReq spellReq[2];
	int s, d, m, v;
	int value[5];
};

struct Perk {
	int id;
	char* description[5];
	char* name;
	PerkLevel level[150];
	int levels;
	char perkType;
	int valueCount;
	int secondaryCount;
	short secondary[20];
	
};

struct MapCoord
{
	int Row;
	int Col;
};

struct OnScreenMessage
{
	uchar index;
	uchar time;
	static uchar Head;
	static uchar Tail;
	static OnScreenMessage Queue[256];
	static uchar NotMainDrawing;
};

struct GemEffect // TODO: switch to Effect
{
    GemEffectType type;
    short value;
};

struct Gem
{
	int baseItemIndex;
	GEM type;
	int grade;
	GemEffect weaponEffect;
	GemEffect armorEffect;
};

struct SpellbookHotkeysInfo
{
	short hotkey[64];
	uchar type[64];
};

struct Aura
{
	ushort auraIndex;
	char auraname[64];
	ushort auraEffect;
	int baseMonsterIndex[20];
	ushort minval;
	ushort maxval;
	uchar radius;
	int sound;
};

struct AdditionalSaveInfo    // 512 byte 
{
	char creation_version[10]; // 10
	char lastsave_version[10]; // 10
	int StashTabsPurchased;    // 4
	int StashCurrentTab;       // 4
	int SpeedrunTimer;         // 4
	std::array<int, SRS_COUNT> SpeedrunTimePoints;
	int reserved_for_SpeedrunTimePoints[16 - SRS_COUNT]; // 64
	SPEEDRUN_STEP CurrentSpeedrunStep; // 4
	char unused[412];
};

struct BackLightColor
{
	uint8_t main;
	uint8_t overcells;
	uint8_t overslot;
	uint8_t notavailable;
	uint8_t cooked;
};

struct BerserkTarget
{
	int index;
	int LastRow;
	int LastCol;
	bool isReachable;
};

struct VisibleMonster {
	int monsterindex;
	int distanceToPlayer;
	int distanceToOrientationPath;
};

struct ScrollView
{
	int xOfs; // Smooth scroll x,y offsets
	int yOfs;
	int dx;	  // Delta between player and view x,y
	int dy;
	int dir;  // Direction
};

struct Portrait
{
	friend struct Pcx;
	Pcx* pcx = 0;
	int id = 0;
	Portrait() = default;
private:
	Portrait(Pcx* pcx, int id): pcx(pcx), id(id) {}
};

struct Pcx
{
	uchar* data = 0;
	SIZE size;
	const char* name;

	Pcx(const char* name): name(name) {}
	~Pcx();
	void Load();
	struct Portrait operator[](int id){ if( ! data ) Load(); return {this, id}; }
};

struct WeaponAnim
{
	PLAYER_ANIM_WEAPON id;
	char select;
	bool canPairShied;
};

struct BossPack
{
	UNIQUE_MONSTER uniq;
	BASE_MONSTER   base;
	int            count;
	int            packFlag = 0; // TODO: change to UNIQUE_PACK packFlag;
};

struct BlockEntry
{
	uint offset;
	uint sizealloc;
	uint sizefile;
	uint flags;
};

struct CompressionInfo
{
	void* sourcebuffer;
	uint  sourceoffset;
	void* destbuffer;
	uint  destoffset;
	uint  bytes;
};

struct FileHeader
{
	uint   signature;
	uint   headersize;
	uint   filesize;
	ushort version;
	ushort sectorsizeid;
	uint   hashoffset;
	uint   blockoffset;
	uint   hashcount;
	uint   blockcount;
};

enum MPQ_FILE
{
	MPQ_HIDE_ATTR       = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM,

	// extra header -- non-standard .MPQ stuff
	FILE_EXHDR_SIZE		= 72,
	FILE_EXHDR_OFFSET	= sizeof(FileHeader),

	// block table
	BLOCK_ENTRIES		= 2048,
	BLOCK_TBL_SIZE		= BLOCK_ENTRIES * sizeof(BlockEntry),
	BLOCK_TBL_OFFSET	= FILE_EXHDR_OFFSET + FILE_EXHDR_SIZE,

	// hash table
	HASH_ENTRIES		= 2048,	// must be pow2
	HASH_TBL_SIZE		= HASH_ENTRIES * sizeof(HashEntry),
	HASH_TBL_OFFSET		= BLOCK_TBL_OFFSET + BLOCK_TBL_SIZE,

	// offset past last file where next added file will start
	FIRST_FILE_START	= HASH_TBL_OFFSET + HASH_TBL_SIZE,

	// minimum space we will allow as standalone "free" block in file
	MIN_FREE_SIZE		= 1024,
	CREATION_TIME	    = 0,
	LASTWRITE_TIME	    = 1,
	NUM_TIMES		    = 2,
};

struct ExFileHeader
{
	uchar bData[FILE_EXHDR_SIZE];
};

struct FullHeader
{
	FileHeader hdr;
	ExFileHeader exhdr;
};

struct E_v25
{
	int		id; // AFFIX_EFFECT
	i64		type;
	int		minVal;
	int		maxVal;
	int		chance;};

struct EA_v25
{
	int		id; // EFFECT_ACTION
	int		minVal;
	int		maxVal;
	int		chance;
};

// sizeof 1339
struct I_v25
{
	int				seed;					// 0 у квестовых уников здесь храниться uniqueItemIndex, для индекса уже есть поле Item.UniqueItemIndex
	int				dropType;				// +4 маска типа дропа синий/белый/уник/из босса/магазин (для генерации\восстановления)
	int				ItemCode;				// +8 (-1 == no item) ItemCode from BaseItem_ID 11 - Gold 196-188
	int				MapRow;					// +12
	int				MapCol;					// +16
	int				playAnimation;			// +20
	char*			ItemCELFilePtr;			// sprite data ? free()
	int				FramesCount;			//
	int				FrameIndex;				// +32
	int				AnimWidth;				//
	int				subOffsetX;				// +40 always 16, this and next may be for subcell items positioning

	// offset 44
	ushort			saveVersion;			// save file verion, for old load map info in new save version in LoadItemFull
	ushort			genVersion;				// item generation version, do not change after saving

	int				animPhase;				// +48
	char			dropping;				// +52
	char			throwing;
	short			resistAll;
	int				Identified;				// TODO: change to bit flag ?
	MAGIC_LEVEL		MagicLevel;				// +60 0 - Normal, 1 - Magic, 2 - Unique (интерпретация данных вещи)
	char			Name[64];				// +61
	char			FullMagicalItemName[64];// +125
	char			EquippedLocation;		// +189
	char			TypeID;					// +190
	uchar			RequiredLevel;			// +191
	int				GraphicValue;			// +192
	int				amount;					// +196 0x00686344 Gold Quantity ?
	int				Price;					// +200
	int				MinDamage;				//-38 +204
	int				MaxDamage;				//-37 +208
	int				ArmorClass;				//-36 +212
	uint			affixFlag;				// + 216
	int				MagicCode;				// +220 -34
	int				SpellIndex;				// +224 -33
	int				CurCharges;				// +228 -32
	int				BaseCharges;			// +232 -31
	int				CurDurability;			// +236 -30 1144
	int				BaseDurability;			// +240 -29 1148
	int				DamagePercent;			// +244 -28
	int				ToHitPercent;			// +248 -27
	int				ArmorPercent;			//252
	int				AddsStrength;			//256
	int				AddsMagic;				//260
	int				AddsDexterity;			//264
	int				AddsVitality;			//268
	int				quality;				// was unused_ResistFire		//272 see item.resist[ ET_1_FIRE ]
	int				magicFindSeed;			// also using for shop extras quality
	int				uniqIndexSeed;			// was unused_ResistArcan		//280 see item.resist[ ET_3_ARCAN ]
	int				AddsManaBy64;			//284
	int				AddsHitPointsBy64;		//288
	int				AddsDamage;				//292
	int				addsDamageFromEnemies;		//296
	int				LightRadiusDiv10;		//300
	char			unused_oldAllSpellLevel;// 304
	uchar			gottenFromLand: 1,		// 305 bit flags
		mirrorImage: 1,
		: 6;					// unused
	short			allSpellLevel;			// 306
	int				UniqueItemIndex;		// +308 -12
	int				MinFireDamage;			//312
	int				MaxFireDamage;			//316
	int				MinLightningDamage;		//320
	int				MaxLightningDamage;		//324
	int				ArmorPiercing;			// +328 -7
	uchar			PrefixEffect;			// TODO: switch to Item.effect[], Effect.id already int
	uchar			PostfixEffect;			// TODO: switch to Item.effect[]
	uchar			Effect3;				// TODO: switch to Item.effect[]
	uchar			Effect4;				// TODO: switch to Item.effect[]
	int				price2;					//-5
	int				multiplier2;			//-4
	int				price1;					// -3
	int				multiplier1;			// -2
	uchar			unused_oldReqStat[4];
	int				IsReqMet;				// +356 bit flags
	int				baseItemIndex;			// +360
	uint			reserved;				// +364 сюда можно расширить affixFlag2 до 64 бит
	uint			affixFlag2;				// +364
	short			goldFind;
	short			magicFindEffect;
	short			overTime;
	uchar           isShadow;     // flag that it's not real item yet, but preview
	uchar           socketsAmount;
	array<short, SocketMax_6> sockets;
	uchar			Effect5;				// TODO: switch to Item.effect[]
	array<short, EnchantMax_5> enchantIndexes;
	QUENCH_AFFIX	quenchAffix;
	uchar			quenchLevel;
	uchar			unused[28];
	char			subType; // ITEM_SUBTYPE, GEM
	char			grade; // oils, gems
	uchar			charFlag;
	uchar			difficulty;
	uchar			socketsBase;
	uchar			socketsAdded;
	i64				seed64_reserved;
	short			RequiredStrength;
	short			RequiredMagic;
	short			RequiredDexterity;
	short			RequiredVitality;
	short			percentStats; // в Player идет в отдельные статы
	short			percentStr;
	short			percentDex;
	short			percentMag;
	short			percentVit;
	int				lifeRegen;
	int				manaRegen;
	short			lifeRegenPercent;
	short			manaRegenPercent;
	int				xpGainMin;
	//int			xpGainMax;
	short			xpGainPercent;
	short			addsMeleeAbsorbPercent;
	short			addsArrowAbsorbPercent;
	short			summonHpPercent;
	short			summonDamagePercent;
	short			summonAcPercent;
	short			summonToHit;
	int				summonHp;
	int				summonDamageMin;
	int				summonDamageMax;
	int				summonAc;
	short			blockChance;
	short			critChance;
	short			critPercent;
	int				critDamageMin;
	int				critDamageMax;
	ushort			spiciesDamageMask;
	int				spiciesDamageMin;
	int				spiciesDamageMax;
	ushort			spiciesDamagePercentMask;
	short			spiciesDamagePercent;
	ushort			spiciesToHitMask;
	short			spiciesToHit;
	ushort			spiciesAcMask;
	int				spiciesAc;
	ushort			spiciesAcPercentMask;
	short			spiciesAcPercent;
	ushort			spiciesDfeMask;
	short			spiciesDfe;
	short			lifePercent;
	short			manaPercent;
	ushort			weaponPercentMask;
	short			weaponPercent;  // в Player нужны отдельные поля типам оружия
	short			resist[ 8 ];
	short			resistControl[ 8 ];
	short			attackSpeed;
	short			castSpeed;
	E_v25			effect[ 20 + 10 ]; // up to 20 unique item affixes, up to 10 base item affixes
	int             damageFromEnemies; // base value from base item
	short           minMeleeAbsorbPercent; // base value from base item
	short           maxMeleeAbsorbPercent; // base value from base item
	short           minArrowAbsorbPercent; // base value from base item
	short           maxArrowAbsorbPercent; // base value from base item

	short			minThorns;
	short			maxThorns;
	short			addsThornsPercent;
};

// sizeof 21960 0x55C8 // *006EAE14
struct P_v25
{
	int				CurAction;							//		0
	char			walkPath[MaxPathStep_25];			// 0x4  4
	unsigned char	IsExists;							// 0x1D	29 
	unsigned char	SomeNewFlags;						// 0x1E	30 тут я в первом байте храню бег в городе
	PLAYER_FULL_CLASS	fullClassId;					// 0x1F 31 бывшее выравнивание (char)
	int				destAction;							// 0x20	32
	int				destParam1;							// 0x24	36 next action param1 (map row / object index )
	int				destParam2;							// 0x28	40 next action param2 (map col / spell level )
	int				destParam3;							// 0x2C	44 next action param3
	int				destParam4;							// 0x30	48 next action param4
	DUNGEON			dungeon;							// 0x34	52 
	char			dungeon_reserved;
	char			unused_2[2];
	int				Row;								// 0x38 56 world row (int*)(*(int*)0x006EAE14+56)
	int				Col;								// 0x3C 60 world col (int*)(*(int*)0x006EAE14+60)
	int				NextRow;							// 0x40	64 player row
	int				NextCol;							// 0x44	68 player col
	int				TargetRow;							// 0x48	72 target row / next moving pos
	int				TargetCol;							// 0x4C	76 target col / next moving pos
	int				ownerRow;							// 0x50 80 owner row / Эти поля видно только в ии 15 и при получении сетевых сообщений используются. как row и col
	int				ownerCol;							// 0x54	84 owner col
	int				oldRow;								// 0x58	88  
	int				oldCol;								// 0x5C	92

	int				xOfs;								// 0x60	96  offset x from left of map tile
	int				yOfs;								// 0x64	100	offset y from bottom of map tile
	int				xVel;								// 0x68	104 current x rate
	int				yVel;								// 0x6C	108 current y rate
	int				dir;								// 0x70 112 current player direction
	int				nextDir;							// 0x74	116 Next direction wanted
	int				Appearance;							// 0x78	120
	char*			currentAnimationPointer;			//___(currentAnimationPointer,void*);		// 0x7C	124
	int				currentAnimationDelay;				// 0x80	128
	int				currentAnimationFrameTick;			// 0x84	132
	int				currentAnimationFramesCount;		// 0x88	136
	int				currentAnimationFrame;				// 0x8C	140
	int				animWidth;							// 0x90	144
	int				animWidth2;							// 0x94	148
	int				UnderArcType;						// 0x98	152 EFlag
	int				LightIndex;							// 0x9C	156
	int				lightRadiusIdentificator;			// 0xA0	160
	int				spellIndex;							// 0xA4	164
	char			readySpellType;						// 0xA8	168
	uchar			readySpellTargetType;				// 0xA9	169
	char			nextArrowElement;					// 0xAA	170 new for Do_RangeAttack
	uchar			altWeaponIndex;						// 0xAB	171 align
	int				SpellOnCursor;						// 0xAC	172
	char			spellOnCursorType;					// 0xB0	176
	char			invCols;
	char			unused_SPT[2];
	int				CurrentSpellNumber;					// 0xB4	180
	int				SpellType;							// 0xB8	184 // TODO: should name spellOrigin
	int				unused_SBKSpell;					// 0xBC	188 Current spell book spell
	char			unused_SBKSpellType;				// 0xC0	192 Spell book spell type
	char			unused_oldSpellLevels[64];			// 0xC1	193,194,195...256
	char			nextMeleeElement;					// unused 
	short			lastAttacker;					    //
	int				AdvancedUser;						//
	union {																						   
		struct {																				   
			int		AvailableSpellMaskLo;				// 0x108	264
			int		AvailableSpellMaskHi;				// 
		};																						   
		__int64		AvailableSpellMask;															   
	};					   
	union {																						   
		struct {																				   
			int		AvailableSkillMaskLo;				// 
			int		AvailableSkillMaskHi;				// 
		};																						   
		__int64		AvailableSkillMask;															   
	};													   
	__int64			AvailableRelictMask;				// 0x118	280
	int				activeBuffFlag;						// 0x120	288
	union {																						   
		struct {																				   
			int		SpellOnF5;							// 0x124	292
			int		SpellOnF6;							// 0x128	296
			int		SpellOnF7;							// 0x12C	300
			int		SpellOnF8;							// 0x130	304
		};												   
		int			SpellsOnFuncKeys[4];				   
	};													   
	uchar			Is_F5_Enabled;						// 0x134	308 // TODO: should be F5_F8_spellOrigin[4]
	uchar			Is_F6_Enabled;						// 0x135	309
	uchar			Is_F7_Enabled;						// 0x136	310
	uchar			Is_F8_Enabled;						// 0x137	311
	char			IsBowInHand;						// 0x138	312
	char			IsFlaskInHand;
	char			throwRight;
	char			running;
	uchar			CanBlock;							// 0x13C	316
	bool			notHittableInTransition;			// 0x13D	317
	uchar			LightRadius;						// 0x13E	318
	uchar			lvlChanging;						// 0x13F	319
	CharName		playerName;							// 0x140	320
	PLAYER_CLASS	ClassID;							/*alignas(4)*/
	char align[3]; // 0x160	352 (int*)(*(int*)0x006EAE14+352)
	int				CurStrength;						// 0x164	356 (int*)(*(int*)0x006EAE14+356)
	int				BaseStrength;						// 0x168	360
	int				CurMagic;							// 0x16C	364
	int				BaseMagic;							// 0x170	368
	int				CurDexterity;						// 0x174	372 (int*)(*(int*)0x006EAE14+372)
	int				BaseDexterity;						// 0x178	376
	int				CurVitality;						// 0x17C	380 (int*)(*(int*)0x006EAE14+380)
	int				BaseVitality;						// 0x180	384
	int				AvailableLvlPoints;					// 0x184	388
	int				BaseDamage;							// 0x188	392
	int				BlockBonus;							// 0x18C	396
	int				BaseLife;							// 0x190	400
	int				MaxBaseLife;						// 0x194	404
	int				CurLife;							// 0x198	408 *(int*)(*(int*)0x006EAE14+408)/64
	int				MaxCurLife;							// 0x19C	412 *(int*)(*(int*)0x006EAE14+412)/64
	int				RatioLifeGlobe;						// 0x1A0	416
	int				BaseMana;							// 0x1A4	420
	int				MaxBaseMana;						// 0x1A8	424
	int				CurMana;							// 0x1AC	428 *(int*)(*(int*)0x006EAE14+428)/64
	int				MaxCurMana;							// 0x1B0	432
	int				RatioManaGlobe;						//_(RatioManaGlobe);					// 0x1B4	436 отключено из-за плясок флага NeedDrawMana (точная сверка отрисовки не нужна)
	short			CharLevel;							// 0x1B8	440 (int*)(*(int*)0x006EAE14+440) 
	short			ReservedCharLevel;					// 0x1B9
	i64				Xp;									// 0x1BC	444
	i64				NextLevelUp;						// 0x1C4	452
	int				TotalGold;							// 0x1CC	460
	int				Infravision;						// 0x1D0	464
	int				CursorRow;							// 0x1D4	468
	int				CursorCol;							// 0x1D8	472
	int				newDirection;						// 0x1DC	476
	int				HorizontalNextRowOrSomeSpellData;	// 0x1E0	480
	int				HorizontalNextCol;					// 0x1E4	484
	int				dx64;								// 0x1E8	488
	int				dy64;								// 0x1EC	492
	int				CurAnimFrameCount;					// 0x1F0	496

	bitset<DungeonExpantionLimit> dungeonVisited;		// 0x1F4	500 1f4
	char			unused[32];
	short			notHittableFramesAfterTransition;

	char			hasGreetedByOgden;					// 0x226	550
	char			field_227;							// 0x227	551

	PLAYER_ANIM_FLAG LoadPlayerAnimationMask;			// 0x228	552

	char*			nothingAnimationsPointers[8];		// 0x22C	556
	int				StayAnimationFrameCount;			// 0x24C	588
	int				AnimPitchStay;						// 0x250	592 AnimStay ( animationCL2filePtr ? )

	char*			walkAnimationsPointers[8];			// 0x254	596
	int				WalkAnimationFrameCount;			// 0x274	628
	int				AnimPitchWalk;						// 0x278	632

	char*			attackAnimationsPointers[8];		// 0x27C	636
	int				AttackAnimationFrameCount;			// 0x29C	668
	int				AnimPitchAttack;					// 0x2A0	672
	int				AttackMainFrame;                    // 0x2A4	676

	char*			lightningSpellAnimationsPointers[8];// 0x2A8	680
	char*			fireSpellAnimationsPointers[8];     // 0x2C8	712
	char*			magicSpellAnimationsPointers[8];    // 0x2E8	744
	int				SpellAnimationFrameCount;			// 0x308	776
	int				AnimPitchCast;						// 0x30C	780
	int				SpellAnimationActionFrameIndex;		// 0x310	784

	char*			gotHitAnimationsPointers[8];		// 0x314	788
	int				HitRecoveryAnimationFrameCount;		// 0x334	820
	int				AnimPitchHit;						// 0x338	824

	char*			deathAnimationsPointers[8];			// 0x33C	828
	int				DeathAnimationFrameCount;			// 0x35C	860
	int				AnimPitchDeath;						// 0x360	864

	char*			blockAnimationsPointers[8];			// 0x364	868
	int				BlockAnimationFrameCount;			// 0x384	900
	int				AnimPitchBlock;						// 0x388	904

	I_v25		OnBodySlots[10];					// 0x38C	908 +1280 +1652 +2024 +/*OnBodySlot[ONBODY_WEAPON]*/2396(оружие, посох - 4 элем) +2768 +3140
	I_v25		InventorySlots[IS_70_40_Inv_Count];	// 0xDB8		3512/*InventorySlots[40]*/
	int				InvItemCount;						// 0x47D8	8392
	union{																						   
		char		InvUsed[IS_70_40_Inv_Count];		// 0x47DC	18396
		char		InvUsedAlt[InvRows][InvCols];		// 0x47DC	18396
	};													   
	I_v25		BeltInventory[IS_8_Belt_Count];					// 0x4804	18436 +18808 ...
	I_v25		ItemOnCursor;						// 0x53A4	21412
	int				MinDamageFromItem;					// 0x5518	21784
	int				MaxDamageFromItem;					// 0x551C	21788
	int				ACFromClass;						// 0x5520	21792
	int				ItemsAddDamagePercents;				// 0x5524	21796
	int				ItemsAddToHitPercents;				// 0x5528	21800
	int				ACFromItems;						// 0x552C	21804
	int				ItemsAddDamage;						// 0x5530	21808
	int				field_5534;							// 0x5534	21812
	union{
		struct{
			int		AvailableChargesMaskLo;				// 0x5538	21816 64 битная маска доступных заклинанй в вещах
			int		AvailableChargesMaskHi;				// 0x553C	21820
		};
		__int64		AvailableChargesMask;
	};													
	int				affixFlag;							// 0x5540	21824
	int				DamageFromEnemy;					// 0x5544	21828
	short			allSpellExtraLevel;					// 0x5548	21832
	array<TraitId,4> traits;							// 0x554A	21834
	short			unused_spellDuration;				// 0x554C	21836
	int				armorPiercing;						// 0x5550	21840
	int				MinFireDamage;						// 0x5554	21844
	int				MaxFireDamage;						// 0x5558	21848
	int				MinLightningDamage;					// 0x555C	21852
	int				MaxLightningDamage;					// 0x5560	21856
	int				OilEffectOnCursor;					// 0x5564	21860
	char			OpenedTownDoors;					// 0x5568	21864
	char			DungeonMessageFlags1;				// 0x5569	21865
	char			SomeSoundMuteDelay;					// 0x556A	21866
	char			DungeonMessageFlags2;				// 0x556B	21867
	char			cooldown;							// 0x556C	21868
	uchar			reserved_1[3];						// 0x556D	21869
	ushort			CountOfReflectCharges;				// 0x5570	21872
	ushort			ReflectCounter;						// 0x5572	21874
	ushort			reserved_2[6];						// 0x5574	21876
	int				allowedDifficulty;					// 0x5580	21888
	int				Difficulty;							// 0x5584	21892
	uint			affixFlag2;							// 0x5588	21896 // можно расширить вниз до 64 бит (сдвинув ниже неипользуемое)
	int				reserved_3[5];						// 0x558C	21900

	char*			StayAnimCel;						//
	char*			WalkAnimCel;						//
	char*			AttackAnimCel;						//
	char*			LightningSpellAnimCel;				//
	char*			FireSpellAnimCel;					//
	char*			MagicSpellAnimCel;					//
	char*			GotHitAnimCel;						//
	char*			DeathAnimCel;						//
	char*			BlockAnimCel;						//
	char*			turningAnimCel;						//

	short			goldFind;
	short			magicFind;
	short			spellExtraLevel[128];
	short			spellPercent[128];
	int				spellDamageMin[128];
	int				spellDamageMax[128];
	ushort			cooldowns[CD_COUNT];
	short			_reserved[65];
	char*			turningAnimationsPointers[8];

	float			scaleResist;
	short			SpellLevels[128];
	float			resistPercent[8];
	float			resistPercentLimit;
	float			resistPercentMeleeMin;
	float			resistPercentMeleeMax;
	float			resistPercentMeleeMinLimit;
	float			resistPercentMeleeMaxLimit;
	float			resistPercentArrowMin;
	float			resistPercentArrowMax;
	float			resistPercentArrowMinLimit;
	float			resistPercentArrowMaxLimit;
	bool			IsPlayerEnemy;//Automatically initialize to false. Explicitly initialized in On_PLAYER_JOINLEVEL()
	uchar			unused1;
	uchar			unused2;
	uchar			unused3;
	uchar			PrevRow; //These 4 are used for better Teleport experience
	uchar			PrevCol;
	uchar			PrevCursorX;
	uchar			PrevCursorY;
	short			elementPercent[16]; // damagePercent  8 reserved
	int				elementMinDamage[16]; // element damage and to hit - AE_ELEMENT
	int				elementMaxDamage[16];
	short			elementToHit[16];
	short			percentStr;
	short			percentDex;
	short			percentMag;
	short			percentVit;
	int				lifeRegen;
	int				manaRegen;
	short			lifeRegenPercent;
	short			manaRegenPercent;
	int				xpGainMin; // xpGain from Item, have to expand in both struct if need
	int				xpGainMax; // also have to update LoadPlayer() offsets
	short			xpGainPercent;

	short			resistMeleeMin;
	short			resistArrowMin;
	short			summonHpPercent;
	short			summonDamagePercent;
	short			summonAcPercent;
	short			summonToHit;
	int				summonHp;
	int				summonDamageMin;
	int				summonDamageMax;
	int				summonAc;
	short			blockChance;
	short			critChance;
	short			critPercent;
	int				critDamageMin;
	int				critDamageMax;

	int				spiciesDamageMin[3];
	int				spiciesDamageMax[3];
	short			spiciesDamagePercent[3];
	short			spiciesToHit[3];
	int				spiciesAc[3];
	short			spiciesAcPercent[3];
	short			spiciesDfe[3];
	short			lifePercent;
	short			manaPercent;
	short			weaponPercent[16];
	short			resist[ 8 ];
	short			stunThreshold[ 8 ];
	short			attackSpeed;
	short			castSpeed;
	int				goldAbsFindMin;
	int				goldAbsFindMax;

	// пока используется как список, использовать как маску (неповторяющиеся эффекты) можно по ненулевому id (либо chance)
	EA_v25 effectAction[200]; // 200 = 20 эффектов * 10 вещей
	// эти сразу используются как маска (по ненулевому id)
	EA_v25 effectActionPercent[EA_CAP]; // не меньше EA_COUNT
	EA_v25 effectActionValue[EA_CAP]; // не меньше EA_COUNT

	char			perk[PERKS_MAX_800];
	__int64			gameChanger;
	char			gameChangerReserved[176];
	uint			seed2;
	uint			seed2_reserved;
	int				CurrentSeedRun;
	int				currentSeedRun_reserved;
	PLAYER_SUBLASS	subclassID;
	PLAYER_SPECIALIZATION   specializationID;

	short			resistMeleeMax;
	short			resistArrowMax;

	/*alignas(4)*/
	char align2; 
	GAME_MODE		gameMode; 
	char align3[3];
	ushort AuraIndex[5];
	ushort AurasValues[5];
};
