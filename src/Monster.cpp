#include "stdafx.h"

bool AuraSoundStates[];
int AuraCooldown[];
bool BattleTranceSlowMonster = 0;
bool BattleTrance = 0;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// инлайн новая функция в 1.145 чанк (0070B600)
__forceinline bool DoesMonsterStealMana(int baseMonsterIndex, int playerIndex)
{
	bool result = true;
	if (PerkValue(PERK_GIVE_THEM_NOTHING, playerIndex) > 0) {
		result = false;
	}
	else if (HasTrait(playerIndex, TraitId::Paladin)) {
		result = false;
	}
	else if (baseMonsterIndex == BM_0_BURNED_CORPSE
		|| baseMonsterIndex == BM_2_ROTTING_CARCASS
		|| baseMonsterIndex == BM_6_GRAVE_DIGGER
		|| baseMonsterIndex == BM_15_TOMB_SLAVE
		|| baseMonsterIndex == BM_58_DEFILER
		|| baseMonsterIndex == BM_144_DARK_SERVANT
		|| baseMonsterIndex == BM_145_BLACK_MOURNER) {
		if (Difficulty >= 1) { // 0070B605 someAction
			result = true; // jmp someAction2
		}
		else {
			result = false; // jmp someAction3
		}
	}
	else if (baseMonsterIndex == BM_83_LASH_WORM
		|| baseMonsterIndex == BM_111_OBSIDIAN_GOLEM
		|| baseMonsterIndex == BM_113_ASH_GOLEM
		|| baseMonsterIndex == BM_120_NECROMORPH
		|| baseMonsterIndex == BM_130_BLACK_GOLEM
		|| baseMonsterIndex == BM_135_GROTESQUE
		|| baseMonsterIndex == BM_140_GHOUL
		|| baseMonsterIndex == BM_141_SHAMBLER
		|| baseMonsterIndex == BM_142_HELL_MEAT
		|| baseMonsterIndex == BM_166_GOATLORD_BOSS
		|| baseMonsterIndex == BM_193_ABATAN
		|| baseMonsterIndex == BM_137_UBER_DIABLO
		|| baseMonsterIndex == BM_110_DIABLO
		|| baseMonsterIndex == BM_194_BLOODIED_HIDDEN
		|| baseMonsterIndex == BM_197_CEROMORTH
		|| baseMonsterIndex == BM_215_CADAVER
		|| baseMonsterIndex == BM_218_ICE_GOLEM
		|| baseMonsterIndex == BM_245_BLUNDERER
		|| baseMonsterIndex == BM_286_BRINE_CORPSE
		|| baseMonsterIndex == BM_C_304_BLACK_DEATH) {
		result = true; // 0070B613 someAction2
	}
	else if (baseMonsterIndex == BM_125_GOLD_GOLEM) {
		if( Dungeon == DUN_24_CRYPT_4 ){ // 0070B660 someAction4
			result = false; // jmp someAction2
		}else{
			result = true; // jmp someAction3
		}
	}
	else {
		result = false; // 0070B60E someAction3
	}
	return result;
}

// Новая полезная функция, оптимизация
__forceinline bool Distance_To_Target_Is_Less_Than(uint monsterIndex, int maxDistance)
{
	if( monsterIndex >= Monsters_Max_Count ) return false;
	Monster& monster = Monsters[ monsterIndex ];
	return (abs(monster.Row - monster.TargetRow) < maxDistance) && (abs(monster.Col - monster.TargetCol) < maxDistance);
}

// Новая полезная функция, оптимизация
__forceinline bool Max_Distance_To_Target_Last_Position(uint monsterIndex, int maxDistance)
{
	if( monsterIndex >= Monsters_Max_Count ) return false;
	Monster& monster = Monsters[monsterIndex];
	return (abs(monster.Row - monster.LastTargetPositionRow) < maxDistance) && (abs(monster.Col - monster.LastTargetPositionCol) < maxDistance);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (0071D890) --------------------------------------------------------
int __fastcall DoesMonsterHaveCritSplash( int monsterIndex )
{
	return CritSplashNumbers[ Monsters[ monsterIndex ].SpritePtr->baseMonsterIndex ];
}

//----- (0071D900) --------------------------------------------------------
void __fastcall CastMonsterCritSplash( int monsterIndex, int deadlyStrike, int playerIndex )
{
	int critSplashNumber = DoesMonsterHaveCritSplash( monsterIndex );
	if( critSplashNumber ){		
		int splashChance = deadlyStrike ? 10 : 30;
		if (HasTrait(playerIndex, TraitId::BloodyMess)) {
			splashChance = 100;
		}
		if( RNG(100) <= splashChance && critSplashNumber <= 3 ){
		    int critSplashIndex = critSplashNumber - 1;
		    int variation = 0;
			if( deadlyStrike ){
				variation = CritAnimationsIndexTable[ critSplashIndex ];
			}else{
				if( RNG(2) ){
					variation = CritAnimationsIndexTable[ critSplashIndex + 6 ];
				}else{
					variation = CritAnimationsIndexTable[ critSplashIndex + 3 ];
				}
			}
			if (GameMode != GM_CLASSIC) {
				CastMissile(Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, variation, 0, 0, critSplashIndex + MI_17_BLOOD, 1, monsterIndex, 0, 0, 0);
			}
		}
	}
}

//----- (0043A905) --------------------------------------------------------
void InitMonsterSlots() // InitLevelMonsters
{
	MonsterSpritesCount = 0;
	MonstersSeedingSizeTotal = 0;
	for( auto& monsterSprite: MonsterSprites ) monsterSprite.placeFlag = 0;
	ClearMonsters();
	SummonMonstersAdded = false; // TODO: not original, should be sync of common monsters list, same to items mp sync
	MonstersCount = 0;
	MonsterArraySize = Monsters_Max_Count; // this value gets further restricted later on
	for( int i = 0; i < Monsters_Max_Count; ++i ) MonsterIndexes[i] = i; // active monsters list
	BossLightIndex = 0;
}

//----- (0043A95E) --------------------------------------------------------
int __fastcall AddMonsterTypeToDungeon(int baseMonsterIndex, MONSTER_PLACEMENT_FLAG mpFlag)
{
	bool done = false;
	int spriteIndex = 0;
	for(; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		done = MonsterSprites[spriteIndex].baseMonsterIndex == baseMonsterIndex;
		if( done ) break;
	}
	if( ! done ){
		if( MonsterSpritesCount >= Monsters_Sprites_Max_Count ) return -1;
		MonsterSprites[MonsterSpritesCount++].baseMonsterIndex = baseMonsterIndex;
		LoadMonsterAnimation(spriteIndex);
		LoadMonsterSounds(spriteIndex);
	}
	// Add image size once for monsters that fill the level
	// not original condition, TODO: find out wtf - placeFlag using before initialized, may be this skips golem & summons
	if( ! (MonsterSprites[spriteIndex].placeFlag & MPF_1_SCATTER) && (mpFlag & MPF_1_SCATTER) ){
	    MonstersSeedingSizeTotal += BaseMonsters[baseMonsterIndex].SeedingSize;
    }
	MonsterSprites[spriteIndex].placeFlag |= mpFlag;
	return spriteIndex;
}

//----- (0043A9DB) --------------------------------------------------------
void GetDungeonMonsterTypes() // GetLevelMTypes
{
	// в th1 ошибка затирающая локальный dungeonLevel
	// пока возспроизводим для сверки и загрузки сингловых записей th1 без вылетов
	//bool bug = IsExeValidating ? true : true; //false;
	// было ниже несколько раз if( bug ) dungeonLevel = d;
	AddMonsterTypeToDungeon(BM_109_GOLEM, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_223_DEMONOLOG_SUMMON_1, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_224_DEMONOLOG_SUMMON_2, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_225_DEMONOLOG_SUMMON_3, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_226_NECROMANT_SUMMON_1, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_227_NECROMANT_SUMMON_2, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_228_NECROMANT_SUMMON_3, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_229_BEASTMASTER_SUMMON_1, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_230_BEASTMASTER_SUMMON_2, MPF_2_DONT);
	AddMonsterTypeToDungeon(BM_231_BEASTMASTER_SUMMON_3, MPF_2_DONT);
	
	switch( Dungeon->level ){ // TODO: check for DUNGEON id needing instead of Dungeon->level
	case DUN_2_CHURCH_2:
		// look up thehell2.cpp where `IsPinkyOnLevel()` starts, there are 4 values to update if i want to move ubers to further diffs (and 5 values here in monster.cpp)
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= 2) {
			AddMonsterTypeToDungeon(BM_207_DREGBEZ, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_208_WARPED_ONE, MPF_2_DONT);
		}
		break;
	case DUN_4_CHURCH_4:
		AddMonsterTypeToDungeon(BM_1_OBLIVION_KNIGHT, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_170_LICH, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_175_TERROR_CLAN, MPF_2_DONT);
		if (Difficulty >= 2 && MaxCountOfPlayersInGame != 1) {
			AddMonsterTypeToDungeon(BM_195_UBER_BUTCHER, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_196_EVIL_ONE, MPF_2_DONT);
		}
		break;
	case DUN_5_CATACOMB_1:
		AddMonsterTypeToDungeon(BM_190_WINGED_DEMON, MPF_2_DONT);
		break;
	case DUN_6_CATACOMB_2:
		AddMonsterTypeToDungeon(BM_57_HELL_CLAN, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_191_DARK_MISTRESS, MPF_2_DONT);
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= 2) {
			AddMonsterTypeToDungeon(BM_205_PINKY, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_206_ACID_SPITTER, MPF_2_DONT);
		}		
		break;
	case DUN_7_CATACOMB_3:
		AddMonsterTypeToDungeon(BM_99_CLEAVER_BOSS, MPF_2_DONT);
		break;
	case DUN_8_CATACOMB_4:
		AddMonsterTypeToDungeon(BM_148_TURBAK_THE_DEUCE, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_246_GREATER_MUMMY, MPF_2_DONT);
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= 2) {
			AddMonsterTypeToDungeon(BM_197_CEROMORTH, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_198_HORN_CLAN, MPF_2_DONT);			
		}
		break;
	case DUN_9_CAVE_1:
		AddMonsterTypeToDungeon(BM_246_GREATER_MUMMY, MPF_2_DONT);
		break;
	case DUN_10_CAVE_2:
		AddMonsterTypeToDungeon(BM_246_GREATER_MUMMY, MPF_2_DONT);
		break;
	case DUN_11_CAVE_3:
		AddMonsterTypeToDungeon(BM_105_IMP_ROGUE, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_246_GREATER_MUMMY, MPF_2_DONT);
		break;
	case DUN_12_CAVE_4:
		AddMonsterTypeToDungeon(BM_285_PHANTOM_KNIGHT, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_105_IMP_ROGUE, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_246_GREATER_MUMMY, MPF_2_DONT);
		if (Difficulty >= 2 && MaxCountOfPlayersInGame != 1) { // MP Doom: add Abatan and his minions
			AddMonsterTypeToDungeon(BM_193_ABATAN, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_194_BLOODIED_HIDDEN, MPF_2_DONT);
		}
		break;
	case DUN_13_HELL_1:
		AddMonsterTypeToDungeon(BM_146_CORRUPT_ORDER, MPF_2_DONT);
		break;
	case DUN_14_HELL_2:
		AddMonsterTypeToDungeon(BM_285_PHANTOM_KNIGHT, MPF_2_DONT);
		break;
	case DUN_15_HELL_3:
		AddMonsterTypeToDungeon(BM_285_PHANTOM_KNIGHT, MPF_2_DONT);
		break;
	case DUN_16_HELL_4:
		AddMonsterTypeToDungeon(BM_110_DIABLO, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_162_CORRUPT_PALADIN, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_188_HELL_CLAN, MPF_2_DONT);
		{
			if (GameMode == GM_CLASSIC) {
				AddMonsterTypeToDungeon(BM_C_389_DIABLO, MPF_4_UNIQ);
			}
		}
		break;
	case DUN_17_ABYSS_1:
		AddMonsterTypeToDungeon(BM_117_STINGER, MPF_1_SCATTER);
		AddMonsterTypeToDungeon(BM_298_TENTACLIST_BOSS, MPF_2_DONT);
		break;
	case DUN_18_ABYSS_2:
		AddMonsterTypeToDungeon(BM_166_GOATLORD_BOSS, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_117_STINGER, MPF_1_SCATTER);
		AddMonsterTypeToDungeon(BM_298_TENTACLIST_BOSS, MPF_2_DONT);
		break;
	case DUN_19_ABYSS_3:
		AddMonsterTypeToDungeon(BM_163_STINGER_LORD, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_166_GOATLORD_BOSS, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_117_STINGER, MPF_1_SCATTER);
		AddMonsterTypeToDungeon(BM_298_TENTACLIST_BOSS, MPF_2_DONT);
		if (MaxCountOfPlayersInGame != 1) {
			// do nothing
		}
		else {
		AddMonsterTypeToDungeon(BM_75_BLOOD_HULK, MPF_1_SCATTER);
		}
		break;
	case DUN_20_ABYSS_4:
		AddMonsterTypeToDungeon(BM_117_STINGER, MPF_1_SCATTER);
		AddMonsterTypeToDungeon(BM_58_DEFILER, MPF_4_UNIQ);
		AddMonsterTypeToDungeon(BM_298_TENTACLIST_BOSS, MPF_2_DONT);
		break;
	// CRYP
	case DUN_21_CRYPT_1:
		AddMonsterTypeToDungeon(BM_154_DEATH_MINION, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_64_DEATH_GUARDIAN, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_158_DEATH_RETINUE, MPF_2_DONT);
		if (MaxCountOfPlayersInGame != 1) {
			AddMonsterTypeToDungeon(BM_50_SKELETON_LORD, MPF_2_DONT);
		}
		break;
	case DUN_22_CRYPT_2:
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, MPF_2_DONT );
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, MPF_2_DONT );		
		break;
	case DUN_23_CRYPT_3:
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, MPF_2_DONT );
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, MPF_2_DONT );		
		break;
	case DUN_24_CRYPT_4:
		AddMonsterTypeToDungeon( BM_137_UBER_DIABLO, MPF_2_DONT); 
		AddMonsterTypeToDungeon( BM_143_DOOM_TAIL, MPF_2_DONT );
		AddMonsterTypeToDungeon( BM_154_DEATH_MINION, MPF_2_DONT );		
		if( MaxCountOfPlayersInGame != 1 && Difficulty >= 2 ){		
			AddMonsterTypeToDungeon( BM_194_BLOODIED_HIDDEN, MPF_2_DONT );
		}
		break;	
	}

	if( Dungeon->isQuest ){

		if( Dungeon == DUN_25_KING_LEORICS_TOMB ){
			AddMonsterTypeToDungeon(GameMode == GM_CLASSIC ? BM_C_391_LEORIC : BM_50_SKELETON_LORD, MPF_4_UNIQ);
		}
		else if (Dungeon == DUN_26_BONE_CHAMBER) {
			AddMonsterTypeToDungeon(BM_242_REANIMATED_HORROR, MPF_2_DONT); // mor: i add undead summoners to Chamber and they should revive this monster type
		}
		else if (Dungeon == DUN_28_POISONED_WATER_SUPPLY) {
			AddMonsterTypeToDungeon(BM_47_ACID_HOUND, MPF_2_DONT);
		}
		else if (Dungeon == DUN_49_SICK_VILLAGERS_BASEMENT) {
			AddMonsterTypeToDungeon(BM_264_BITELING, MPF_2_DONT);
		}
		else if (Dungeon == DUN_47_DARK_NEST || Dungeon == DUN_48_BLOOD_HIVE) {
			AddMonsterTypeToDungeon(BM_253_EXPLODER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_55_DEEP_HOLLOW) {
			AddMonsterTypeToDungeon(BM_253_EXPLODER, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_270_MUD_CRAWLER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_56_WEB_TUNNELS || Dungeon == DUN_57_ARACHNID_LAIR) {
			AddMonsterTypeToDungeon(BM_277_SPIDERLING, MPF_2_DONT);
		}
		else if (Dungeon == DUN_78_THE_BRIDGE) {
			AddMonsterTypeToDungeon(BM_82_GARGOYLE, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_253_EXPLODER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_80_HELL_GATE || Dungeon == DUN_87_PANDEMONIUM || Dungeon == DUN_89_BEDLAM || Dungeon == DUN_96_BONE_MIRE) {
			AddMonsterTypeToDungeon(BM_253_EXPLODER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_83_BLOOD_KEEP) {
			AddMonsterTypeToDungeon(BM_248_SALAMANDER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_86_VALLEY_OF_WRATH) {
			AddMonsterTypeToDungeon(BM_158_DEATH_RETINUE, MPF_2_DONT);
		}
		else if (Dungeon == DUN_90_FIELDS_OF_MADNESS) {
			AddMonsterTypeToDungeon(BM_117_STINGER, MPF_2_DONT);
		}
		else if (Dungeon == DUN_92_FLESH_PIT) {
			AddMonsterTypeToDungeon(BM_270_MUD_CRAWLER, MPF_2_DONT);
		}
		return;
	}

	if( IsQuestOnLevel(Q_6_THE_BUTCHER) ){
		AddMonsterTypeToDungeon(GameMode == GM_CLASSIC ? BM_C_390_BUTCHER : BM_51_THE_BUTCHER, MPF_2_DONT);
	}
	if( IsQuestOnLevel(Q_2_GHARBAD_THE_WEAK) ){
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_849_GHARBAD : UM_0_Gharbad_the_Weak].BaseMonsterIndex, MPF_4_UNIQ);
	}
	if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_870_ZHAR_THE_MAD : UM_2_Zhar_the_Mad].BaseMonsterIndex, MPF_4_UNIQ);
	}
	if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_829_SNOTSPILL : UM_3_Snotspill].BaseMonsterIndex, MPF_4_UNIQ);
	}
	if( IsQuestOnLevel(Q_4_LACHDANAN) ){
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_865_LACHDANAN : UM_7_Lachdanan].BaseMonsterIndex, MPF_4_UNIQ);
	}
	if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) ){
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_862_WARLORD : UM_8_Warlord_of_Blood].BaseMonsterIndex, MPF_4_UNIQ);
	}
	//if (IsQuestOnLevel(Q_8_HALLS_OF_THE_BLIND)) { // looks like this was not needed, turned off for now
	//	AddMonsterTypeToDungeon(UniqueMonsters[UM_676_HOTB_Boss].BaseMonsterIndexMPF_,_ 4);
	//}

	int mode = (GameMode == GM_CLASSIC) + 1;
	if( MaxCountOfPlayersInGame != 1 && Dungeon == Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonStart ){
		AddMonsterTypeToDungeon(GameMode == GM_CLASSIC ? BM_C_391_LEORIC : BM_50_SKELETON_LORD, MPF_4_UNIQ);
		int countOfSkeletons = 0;
		int skeletonsIndexes[50];
		for( int baseMonsterIndex = BM_8_BONE_CREEPER; baseMonsterIndex <= BM_27_HORROR; baseMonsterIndex++ ){
			BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
			if( IsMonsterSummonable(baseMonsterIndex) 
			 && Dungeon->level >= 15 * baseMonster.MinDungeonLevel / 30 + 1 
			 && Dungeon->level <= 15 * baseMonster.MaxDungeonLevel / 30 + 1 
			 && baseMonster.valid & mode ){
				skeletonsIndexes[countOfSkeletons++] = baseMonsterIndex;
			}
		}
		AddMonsterTypeToDungeon(skeletonsIndexes[RNG(countOfSkeletons)], MPF_1_SCATTER);
	}

	int validMonstersIndexes[BM_COUNT];
	int countOfValidMonsters = 0;
	for( int baseMonsterIndex = 0; baseMonsterIndex < BM_COUNT; baseMonsterIndex++ ){
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		if( Dungeon->level >= 15 * baseMonster.MinDungeonLevel / 30 + 1 
			&& Dungeon->level <= 15 * baseMonster.MaxDungeonLevel / 30 + 1 
			&& baseMonster.valid & mode ){
				validMonstersIndexes[countOfValidMonsters++ + 1] = baseMonsterIndex;
		}
	}

	// TODO: check for needing
	if( SomeBoolFlag_12 && MapRowArraySize > 0 ){ // SomeBoolFlag_12 вроде нулевое всегда
		for( int i = 0; i < MapRowArraySize; i++ ){
			AddMonsterTypeToDungeon(MapRowArray[i], MPF_1_SCATTER);
		}
		return;
	}

	while( countOfValidMonsters > 0 
		&& MonsterSpritesCount < Monsters_Sprites_Max_Count
		&& MonstersSeedingSizeTotal < Monsters_SeedingSize_Max ){

			if( countOfValidMonsters ){
				// prune excessively large monsters
				int curValidMonsterIndex = 0;
				int leftSeedingSize = Monsters_SeedingSize_Max - MonstersSeedingSizeTotal;
				do{
					if( BaseMonsters[validMonstersIndexes[curValidMonsterIndex + 1]].SeedingSize <= leftSeedingSize ){
						++curValidMonsterIndex;
					}else{
						--countOfValidMonsters;
						validMonstersIndexes[curValidMonsterIndex + 1] = validMonstersIndexes[countOfValidMonsters + 1];
					}
				}while( curValidMonsterIndex < countOfValidMonsters );
			}
			if( countOfValidMonsters ){
				// добавляем монстра и выкидываем из списка
				int selectedBaseMonsterIndex = RNG(countOfValidMonsters);
				AddMonsterTypeToDungeon(validMonstersIndexes[selectedBaseMonsterIndex + 1], MPF_1_SCATTER);
				validMonstersIndexes[selectedBaseMonsterIndex + 1] = validMonstersIndexes[countOfValidMonsters--];
			}
	}
}

//----- (0043AD39) --------------------------------------------------------
void __fastcall LoadMonsterAnimation(int monsterSpriteIndex)
{
	MonsterSprite& monsterSprite = MonsterSprites[monsterSpriteIndex];
	int baseMonsterIndex = monsterSprite.baseMonsterIndex;
	BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
	for( int animationIndex = 0; animationIndex < 6; animationIndex++ ){
		MonsterAnim& monsterAnim = monsterSprite.animation[animationIndex];
		int frameCount = baseMonster.animationFrameCount[animationIndex];
		char animType = TypeOfAnimation[animationIndex];
		if( (animType != 's' || baseMonster.HasSecondAttack) && frameCount > 0 ){
			char animFileName[256];
			sprintf(animFileName, baseMonster.AnimationFileName, animType);
			char* anim = (char*)LoadFile(animFileName);
			monsterAnim.animPtr = anim;
			if( monsterSprite.baseMonsterIndex != BM_109_GOLEM || animType != 's' && animType != 'd' ){
				// тут уже различная анимация в разных направлениях
				uint* animHeader = (uint*)anim;
				for( int dir = 0; dir < 8; dir++ ){
					uint curAnimationOffset = animHeader[dir];
					AnimDir& animDir = monsterAnim.oriented[dir];
					animDir.data = anim + curAnimationOffset;
					animDir.xOfs = 0;
					animDir.yOfs = 0;
					MonsterSpritePosAdjust(baseMonsterIndex, animationIndex, dir, animDir.xOfs, animDir.yOfs);
				}
			}else{
				// одинаковая анимация. как ни крути)
				for( int dir = 0; dir < 8; dir++ ){
					AnimDir& animDir = monsterAnim.oriented[dir];
					animDir.xOfs = 0;
					animDir.yOfs = 0;
					monsterAnim.oriented[dir].data = anim;
					MonsterSpritePosAdjust(baseMonsterIndex, animationIndex, dir, animDir.xOfs, animDir.yOfs);
				}
			}
		}else{
			monsterAnim.animPtr = 0;
			for( int dir = 0; dir < 8; dir++ ){
				monsterAnim.oriented[dir].data = 0;
			}
		}
		monsterAnim.frameCount = frameCount;
		monsterAnim.animationDelay = baseMonster.animationDelay[animationIndex];
	}

	monsterSprite.Pitch = baseMonster.SpritePitch;
	monsterSprite.StartOfsX = (baseMonster.SpritePitch - 64) >> 1;
	monsterSprite.minHitPoints = baseMonster.MinHitPoints;
	monsterSprite.maxHitPoints = baseMonster.MaxHitPoints;
	monsterSprite.hasSecondAttack = baseMonster.HasSecondAttack;
	monsterSprite.firstAtackToHitFrame = baseMonster.FirstAtackToHitFrame;
	monsterSprite.baseMonsterPtr = &baseMonster;
	
	// раскрашиваем монстров
	if (baseMonster.IceAgeTranslationFilePtr && !Dungeon->isQuest && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		monsterSprite.colorTable = (uchar*)LoadFile(baseMonster.IceAgeTranslationFilePtr);
		ColorMonsterSprite(monsterSpriteIndex, baseMonster.HasSecondAttack);
		FreeMemZero(monsterSprite.colorTable);
	} else if( baseMonster.HasColorTranslationFile ){
		monsterSprite.colorTable = (uchar*)LoadFile(baseMonster.ColorTranslationFilePtr);
		ColorMonsterSprite(monsterSpriteIndex, baseMonster.HasSecondAttack);
		FreeMemZero(monsterSprite.colorTable);
	}

	

}

//----- (0043B0CB) --------------------------------------------------------
void __fastcall ColorMonsterSprite(int spriteIndex, int haveSecondAttack)
{
	MonsterSprite& sprite = MonsterSprites[spriteIndex];
	for( uchar* pal = sprite.colorTable; pal < sprite.colorTable + 256; pal++ ){
		if( *pal == 255u ){
			*pal = 0;
		}
	}
	for( int animType = 0; animType < (haveSecondAttack != 0) + 5; animType++ ){
		if( animType != 1 || sprite.baseMonsterIndex < 105 || sprite.baseMonsterIndex > 108 ){
			for( int dir = 0; dir < 8; dir++ ){
				ColorSpriteAnimation( sprite.animation[animType].oriented[dir].data, sprite.colorTable, sprite.animation[animType].frameCount );
			}
		}
	}
}

void __fastcall VaccinateMonster( Monster& monster )
{
	if( monster.ResistImmune & MR_4_IM_ARCAN ){ monster.ResistImmune &= ~MR_4_IM_ARCAN; monster.ResistImmune |= MR_1_ARCAN; }
	if( monster.ResistImmune & MR_5_IM_FIRE	 ){ monster.ResistImmune &= ~MR_5_IM_FIRE;  monster.ResistImmune |= MR_2_FIRE;  }
	if( monster.ResistImmune & MR_6_IM_LIGHT ){ monster.ResistImmune &= ~MR_6_IM_LIGHT; monster.ResistImmune |= MR_3_LIGHT; }
	if( monster.ResistImmune & MR_8_IM_ACID	 ){ monster.ResistImmune &= ~MR_8_IM_ACID;  /*monster.ResistImmune |= MR_9_ACID;*/  }
	if( monster.ResistImmune & MR_11_IM_COLD ){ monster.ResistImmune &= ~MR_11_IM_COLD; /*monster.ResistImmune |= MR_10_COLD;*/ }
}

//----- (0043B159) --------------------------------------------------------
void __fastcall CreateMonster(int monsterIndex, int direction, int monsterSpriteIndex, int row, int col)
{
	Monster& monster = Monsters[monsterIndex];
	monster.Orientation = direction;
	monster.Row = row;
	monster.Col = col;
	monster.NextRow = row;
	monster.NextCol = col;
	monster.PrevRow = row;
	MonsterSprite* sprite = &MonsterSprites[monsterSpriteIndex];
	monster.PrevCol = col;
	monster.SpriteIndex = monsterSpriteIndex;
	monster.CurAction = A_0_STAY;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && (char*)sprite->baseMonsterPtr->IceAgeNamePtr) {
		monster.Name = (char*)sprite->baseMonsterPtr->IceAgeNamePtr;
	}
	else {
		monster.Name = (char*)sprite->baseMonsterPtr->name;
	}
	monster.SpritePtr = sprite;
	monster.BasePtr = sprite->baseMonsterPtr;
	monster.curAnimation = &sprite->animation[ANIM_0_STAY].oriented[direction];
	int animDelay = sprite->animation[ANIM_0_STAY].animationDelay;
	monster.AnimationDelayFrameCount = animDelay;
	monster.AnimationDelayIndex = RNG(animDelay - 1);
	int frameCount = sprite->animation[ANIM_0_STAY].frameCount;
	monster.animFrameCount = frameCount;
	monster.CurFrame = RNG(frameCount - 1) + 1;
	monster.BaseLife = RngFromRange(sprite->minHitPoints, sprite->maxHitPoints) << 6;
	if (MaxCountOfPlayersInGame == 1) {
		monster.BaseLife /= 2;
		LimitToMin(monster.BaseLife, 64);
	}
	monster.CurrentLife = monster.BaseLife;
	monster.ai = sprite->baseMonsterPtr->ai;
	monster.intel = sprite->baseMonsterPtr->intelligenceFactor;
	monster.goal = MG_1_ATTACK;
	monster.goalParam = 0;
	monster.curOrientation = 0;
	monster.mayRetreateTick = 0;
	monster.BossID = 0;
	monster.pathCount = 0;
	monster.newBossId = 0;
	monster.ActivationCounter = 0;
	monster.LightIndex = 0;
	monster.RandomSeedForDrop = GetRndSeed();
	monster.RandomSeedForAction = GetRndSeed();
	monster.Attackers = 0;
	monster.acidDot = 0; // Monsters should be initialized with monster.acidDot = 0, but it should not reset to 0 upon player re-enter the same Dlvl, especially in MP. If commented out, however, can make a monster on a different Dlvl also carry the green bar of the originally acid-attacked monster...
	monster.acidDotSum = 0;
	monster.acidDotDeltaSum = 0;
	monster.dotLastAttacker = -1; // playerIndex = 0 should not be assigned to monster.dotLastAttacker when monster is not attacked by acid dot
	monster.overTime = 0;
	monster.prevOverTime = 0;
	monster.dungeon = Dungeon;
	monster.unused_oldMonsterLevel = 0;
	monster.MonsterLevel = sprite->baseMonsterPtr->MonsterBaseLevel;
	monster.Xp = 4 * sprite->baseMonsterPtr->ExperiencePoints / 5 + Rng32to64(2 * sprite->baseMonsterPtr->ExperiencePoints / 5 + 1);
	monster.ToHit = sprite->baseMonsterPtr->FirstAtackToHit;
	monster.MinDamage = sprite->baseMonsterPtr->FirstAtackMinDamage;
	monster.MaxDamage = sprite->baseMonsterPtr->FirstAtackMaxDamage;
	monster.SecondToHit = sprite->baseMonsterPtr->SecondAttackToHit;
	monster.SecondMinDamage = sprite->baseMonsterPtr->SecondAtackMinDamage;
	monster.SecondMaxDamage = sprite->baseMonsterPtr->SecondAtackMaxDamage;
	monster.ArmorClass = sprite->baseMonsterPtr->ArmorClass;
	monster.ResistImmune = sprite->baseMonsterPtr->ResistOn1And2Difficulty;
	monster.ChampionNumber = 0;
	monster.hisBossIndex = 0;
	monster.isBossPack = 0;
	monster.flag = sprite->baseMonsterPtr->flag;
	monster.speechIndex = 0;
	monster.TargetObjectIndex = -1;
	monster.IsSummonedByMonster = 0;
	memzero(monster.affixes);
	if (monster.ai == AI_12_Gargoyle) {
		monster.curAnimation = &sprite->animation[ANIM_5_SECOND].oriented[direction];
		monster.CurFrame = 1;
		monster.flag |= MF_3_STONE;
		monster.CurAction = A_7_SECOND;
	}
	int baseMonster = sprite->baseMonsterIndex;

	//Original formulas :
	/*if (gnDifficulty == D_NIGHTMARE) {
		monster[i]._mmaxhp = (monster[i]._mmaxhp * 3) + (((gbMaxPlayers == 1) ? 50 : 100) << HP_SHIFT);
		monster[i]._mhitpoints = monster[i]._mmaxhp;
		monster[i].mLevel += 15;
		monster[i].mExp = (monster[i].mExp << 1) + 2000;
		monster[i].mHit += 85;
		monster[i].mMinDamage = (monster[i].mMinDamage * 2) + 4;
		monster[i].mMaxDamage = (monster[i].mMaxDamage * 2) + 4;
		monster[i].mHit2 += 85;
		monster[i].mMinDamage2 = (monster[i].mMinDamage2 * 2) + 4;
		monster[i].mMaxDamage2 = (monster[i].mMaxDamage2 * 2) + 4;
		monster[i].mArmorClass += 50;
	}
	else if (gnDifficulty == D_HELL) {
		monster[i]._mmaxhp = (monster[i]._mmaxhp * 4) + (((gbMaxPlayers == 1) ? 100 : 200) << HP_SHIFT);
		monster[i]._mhitpoints = monster[i]._mmaxhp;
		monster[i].mLevel += 30;
		monster[i].mExp = (monster[i].mExp << 2) + 4000;
		monster[i].mHit += 120;
		monster[i].mMinDamage = (monster[i].mMinDamage * 4) + 6;
		monster[i].mMaxDamage = (monster[i].mMaxDamage * 4) + 6;
		monster[i].mHit2 += 120;
		monster[i].mMinDamage2 = (monster[i].mMinDamage2 * 4) + 6;
		monster[i].mMaxDamage2 = (monster[i].mMaxDamage2 * 4) + 6;
		monster[i].mArmorClass += 80;
		monster[i].mMagicRes = monst->MData->mMagicRes2;
	}*/
	if (GameMode == GM_CLASSIC) {
		if (Difficulty == 1) {
			int life = (monster.BaseLife * 3) + (((MaxCountOfPlayersInGame != 1) ? 100 : 50) << 6); //2 * (((MaxCountOfPlayersInGame != 1 ? 100 : 50) * 128) + monster.BaseLife) + 16'000;
			monster.BaseLife = life;
			monster.CurrentLife = life;
			monster.MonsterLevel += 15;
			monster.Xp = 2 * monster.Xp + 2'000;
			monster.ToHit += 85;
			monster.MinDamage += monster.MinDamage * 2 + 4;
			monster.MaxDamage += monster.MaxDamage * 2 + 4;
			monster.SecondToHit += 85;
			monster.SecondMinDamage += monster.SecondMinDamage * 2 + 4;
			monster.SecondMaxDamage += monster.SecondMaxDamage * 2 + 4;
			monster.ArmorClass += 50;
		}
		if (Difficulty == 2) {
			int life = (monster.BaseLife * 4) + (((MaxCountOfPlayersInGame != 1) ? 200 : 100) << 6); //2 * (((MaxCountOfPlayersInGame != 1 ? 100 : 50) * 128) + monster.BaseLife) + 16'000;
			monster.BaseLife = life;
			monster.CurrentLife = life;
			monster.MonsterLevel += 30;
			monster.Xp = 4 * monster.Xp + 4'000;
			monster.ToHit += 120;
			monster.MinDamage += monster.MinDamage * 4 + 6;
			monster.MaxDamage += monster.MaxDamage * 4 + 6;
			monster.SecondToHit += 120;
			monster.SecondMinDamage += monster.SecondMinDamage * 4 + 6;
			monster.SecondMaxDamage += monster.SecondMaxDamage * 4 + 6;
			monster.ArmorClass += 80;
			monster.ResistImmune = sprite->baseMonsterPtr->ResistOn3Difficulty;
		}
	}
	else {
		if (Difficulty == DL_1_PURGATORY) {
			int life = 2 * (((MaxCountOfPlayersInGame != 1 ? 250 : 125) * 128) + monster.BaseLife) + 16'000;
			switch (baseMonster) {
				case BM_3_FAMILIAR or BM_14_SOUL_HUNTER or BM_20_RISEN_SKELETON or BM_21_SKELETON_WARRIOR or BM_22_BURNING_DEAD or BM_23_MALICE
					or BM_40_QUASIT or BM_41_DARK_SERAPH or BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_48_ACID_BAT
					or BM_54_BEHOLDER or BM_98_IMMORTAL or BM_165_APOCALYPSE_CLAN or BM_168_HELL_BAT or BM_181_ADVOCATE or BM_199_HELL_CLAN or BM_202_SUCCUBUS
					or BM_200_CABALIST or BM_201_ADEPT or BM_212_CACODEMON or BM_102_SUCCUBUS or BM_103_BLOOD_WITCH or BM_104_DARK_WITCH or BM_105_IMP_ROGUE
					or BM_106_CORRUPT_ORDER or BM_107_DARK_MAGE or BM_108_MAGISTRATE or BM_114_FROZEN_HORROR or BM_117_STINGER or BM_119_DARK_GHOST or BM_124_GLOOM_CLAN
					or BM_129_LICH or BM_131_FIRE_BAT or BM_133_SOUL_BURNER or BM_156_IMP_ROGUE or BM_143_DOOM_TAIL or BM_147_FIEND or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN
					or BM_279_FLOATER or BM_280_FLOATER_X2 or BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW or BM_284_SIN_SPAWN or BM_220_SNOW_LICH or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE:
					life /= 2;
					break;
					case BM_4_GREMLIN or BM_219_FROST_BONE or BM_5_FALLEN_ONE or BM_8_BONE_CREEPER or BM_12_IMP or BM_13_DARK_ONE or BM_17_SCAVENGER or BM_19_RAVAGER
						or BM_49_BILE_SPITTER or BM_155_FALLEN_ONE or BM_172_SPITTING_TERROR or BM_177_PHASE_DEMON or BM_182_BLINK_DEMON or BM_29_HIDDEN or BM_30_SHADOW
						or BM_31_UNSEEN or BM_38_UNDEAD_WEREWOLF or BM_39_WARPING_DEMON or BM_53_HELL_HOUND or BM_80_DEVIL_KIN or BM_83_LASH_WORM or BM_92_ILLUSION_WEAVER
						or BM_94_LURKER or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER or BM_149_THRESHER or BM_150_FELLTWIN or BM_151_HYBRID
						or BM_157_YELLOW_ONE or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_232_ABYSSAL_SUCCUBUS or BM_235_STYGIAN_FURY or BM_237_CONTAMINATOR
					or BM_241_ACID_TERROR or BM_248_SALAMANDER or BM_257_SHROUDED:
						life -= life / 4;
						break;
					case BM_208_WARPED_ONE or BM_253_EXPLODER:
						life /= 4;
						break;
			}
			monster.BaseLife = life;
			monster.CurrentLife = life;
			monster.MonsterLevel += 15;
			monster.Xp = 3 * (monster.Xp + 700) + RNG(101);
			monster.ToHit += 90 * Difficulty + RNG(2 + 2 * Difficulty);
			int minDmg = 65 * Difficulty, maxDmg = 85 * Difficulty + RNG(2 + 2 * Difficulty);
			if (is(baseMonster, BM_117_STINGER, BM_277_SPIDERLING, BM_143_DOOM_TAIL)) {
				minDmg -= 2 * minDmg / 5;
				maxDmg -= 2 * maxDmg / 5;
				LimitToMin(maxDmg, minDmg);
			}
			monster.MinDamage += minDmg;
			monster.MaxDamage += maxDmg;
			monster.SecondToHit += 90 * Difficulty + RNG(2 + 2 * Difficulty);
			monster.SecondMinDamage += 70 * Difficulty;
			monster.SecondMaxDamage += 90 * Difficulty + RNG(2 + 2 * Difficulty);
			monster.ArmorClass += 85 * Difficulty + RNG(2 + 2 * Difficulty);
		}
		else if (Difficulty >= DL_2_DOOM) {
			int diff_modifier = Difficulty * 2;
			int flat_addons = by(Difficulty, 0, 0, 4'000, 12'000, 24'000, 36'000, 48'000, 60'000, 72'000);
			if (MaxCountOfPlayersInGame == 1) {
				flat_addons /= 2;
			}
			int life = (monster.BaseLife * diff_modifier) + (flat_addons * 64);

			switch (baseMonster) {
				case BM_3_FAMILIAR or BM_14_SOUL_HUNTER or BM_20_RISEN_SKELETON or BM_21_SKELETON_WARRIOR or BM_22_BURNING_DEAD or BM_23_MALICE
					or BM_40_QUASIT or BM_41_DARK_SERAPH or BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_48_ACID_BAT
					or BM_54_BEHOLDER or BM_98_IMMORTAL or BM_101_DARK_MISTRESS or BM_102_SUCCUBUS or BM_103_BLOOD_WITCH or BM_104_DARK_WITCH or BM_105_IMP_ROGUE
					or BM_106_CORRUPT_ORDER or BM_107_DARK_MAGE or BM_108_MAGISTRATE or BM_114_FROZEN_HORROR or BM_117_STINGER or BM_119_DARK_GHOST
					or BM_124_GLOOM_CLAN or BM_129_LICH or BM_131_FIRE_BAT or BM_133_SOUL_BURNER or BM_156_IMP_ROGUE or BM_165_APOCALYPSE_CLAN or BM_168_HELL_BAT
					or BM_181_ADVOCATE or BM_198_HORN_CLAN or BM_199_HELL_CLAN or BM_202_SUCCUBUS or BM_200_CABALIST or BM_206_ACID_SPITTER or BM_143_DOOM_TAIL
					or BM_147_FIEND or BM_201_ADEPT or BM_212_CACODEMON or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN
					or BM_279_FLOATER or BM_280_FLOATER_X2 or BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW or BM_284_SIN_SPAWN or BM_220_SNOW_LICH or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE:
					life /= 2;
					break;
					case BM_49_BILE_SPITTER or BM_155_FALLEN_ONE or BM_172_SPITTING_TERROR or BM_177_PHASE_DEMON or BM_182_BLINK_DEMON or BM_4_GREMLIN or BM_219_FROST_BONE
						or BM_5_FALLEN_ONE or BM_8_BONE_CREEPER or BM_12_IMP or BM_13_DARK_ONE or BM_17_SCAVENGER or BM_19_RAVAGER or BM_29_HIDDEN or BM_30_SHADOW or BM_31_UNSEEN
						or BM_38_UNDEAD_WEREWOLF or BM_39_WARPING_DEMON or BM_53_HELL_HOUND or BM_80_DEVIL_KIN or BM_83_LASH_WORM
						or BM_92_ILLUSION_WEAVER or BM_94_LURKER or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER or BM_149_THRESHER
						or BM_150_FELLTWIN or BM_151_HYBRID or BM_157_YELLOW_ONE or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_232_ABYSSAL_SUCCUBUS or BM_235_STYGIAN_FURY
					or BM_237_CONTAMINATOR or BM_241_ACID_TERROR or BM_248_SALAMANDER or BM_257_SHROUDED:
						life -= life / 4;
						break;
					case BM_208_WARPED_ONE or BM_253_EXPLODER:
						life /= 4;
						break;
			}
			monster.BaseLife = life;
			monster.CurrentLife = life;
			monster.MonsterLevel += 15 * Difficulty;
			// old XP formula (for TH3 v0.469), leaving it here in case it needs to be restored
			//monster.Xp = (2 * Difficulty) * (monster.Xp * Difficulty + ( (90 * Difficulty) * (Difficulty * 5) ) );

			// new XP formula (TH4 v0.125+)
			int xp_multiplier = by(Difficulty, 1, 1, 30, 300, 1'200, 1'500, 2'000, 2'400, 2'800);
			int xp_flat_addon = by(Difficulty, 0, 0, 6'000, 65'000, 600'000, 2'410'000, 5'290'000, 9'140'000, 13'900'000);
			monster.Xp = monster.Xp * xp_multiplier + xp_flat_addon;

			int addups = 0;
			//if (Difficulty == DL_8_INFERNO ? (Dungeon->level * 2) : 0;
			switch (Difficulty) {
			case DL_5_AGONY:		addups = Dungeon->level; break;
			case DL_6_TERROR:		addups = 24 + 3 * Dungeon->level / 2; break;
			case DL_7_DESTRUCTION:	addups = 24 + 36 + 2 * Dungeon->level; break;
			case DL_8_INFERNO:		addups = 24 + 36 + 48 + 3 * Dungeon->level; break;
			}
			monster.ToHit += 90 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			int minDmg = 65 * Difficulty + addups, maxDmg = 85 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			if (is(baseMonster, BM_117_STINGER, BM_277_SPIDERLING, BM_143_DOOM_TAIL)) {
				minDmg -= 2 * minDmg / 5;
				maxDmg -= 2 * maxDmg / 5;
				LimitToMin(maxDmg, minDmg);
			}
			monster.MinDamage += minDmg;
			monster.MaxDamage += maxDmg;
			monster.SecondToHit += 90 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			monster.SecondMinDamage += 70 * Difficulty + addups;
			monster.SecondMaxDamage += 90 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			monster.ArmorClass += 85 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			monster.ResistImmune = sprite->baseMonsterPtr->ResistOn3Difficulty;
		}
	}
	// -----------special manual monster boosts:---------------
	if (Dungeon == DUN_108_ARCANE_PRISON) {
		switch (baseMonster) {
		case BM_240_DEMON_PRIEST:
			monster.MonsterLevel += 16;
			monster.Xp *= 3;
			monster.BaseLife += monster.BaseLife / 2;
			monster.CurrentLife = monster.BaseLife;
			monster.ToHit += 20;
			monster.MinDamage += 14; 
			monster.MaxDamage += 16;
			monster.ArmorClass += 10;
		}
	}
	else if (Dungeon == DUN_86_VALLEY_OF_WRATH) {
		if (baseMonster == BM_158_DEATH_RETINUE) {
			monster.MonsterLevel -= 5;
			monster.BaseLife -= monster.BaseLife / 3;
			monster.CurrentLife = monster.BaseLife;
			monster.ToHit -= 10;
			monster.MaxDamage -= 10;
			monster.ArmorClass -= 10;
		}
	}
	//--------------------end of special manual monster boosts------------------------

	if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_11_VACCINATED) && MaxCountOfPlayersInGame == 1){
		VaccinateMonster(monster);
	}
	if (GameMode == GM_HARD) { // +50% HP on Hard mode
		monster.BaseLife += monster.BaseLife / 2;
		monster.CurrentLife = monster.BaseLife;
	}
	else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) { // -40% HP on Hard mode
		monster.BaseLife -= 2 * monster.BaseLife / 5;
		monster.CurrentLife = monster.BaseLife;
	}
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_17_TOUGH_MNSTR)) {
		monster.BaseLife *= 3;
		monster.CurrentLife = monster.BaseLife;
	}
	if (GameMode != GM_CLASSIC) {
		monster.canFly = (baseMonster == BM_82_GARGOYLE && Dungeon == DUN_112_STILL_SPRINGS)
			|| (is(baseMonster,
				BM_3_FAMILIAR,
				BM_40_QUASIT,
				BM_41_DARK_SERAPH,
				BM_48_ACID_BAT,
				BM_54_BEHOLDER,
				BM_98_IMMORTAL,
				BM_100_TORTURED_SPIRIT,
				BM_119_DARK_GHOST,
				BM_121_SPECTRE,
				BM_129_LICH,
				BM_131_FIRE_BAT,
				BM_147_FIEND,
				BM_168_HELL_BAT,
				BM_170_LICH,
				BM_192_UNRAVELLER,
				BM_212_CACODEMON,
				BM_235_STYGIAN_FURY,
				BM_237_CONTAMINATOR,
				BM_273_FLAME_FIEND,
				BM_223_DEMONOLOG_SUMMON_1,
				BM_279_FLOATER,
				BM_280_FLOATER_X2
			))
			|| (MaxCountOfPlayersInGame == 1 && !Dungeon->isQuest && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE));
	}
}

//----- (0043B554) --------------------------------------------------------
void ClearMonsters()
{
	for( int monsterIndex = 0; monsterIndex < Monsters_Max_Count; monsterIndex++ ){
		Monster& monster = Monsters[monsterIndex];
		ClearMonsterWalkCounters(monsterIndex);
		monster.Name = "Invalid Monster";
		monster.goal = MG_0_DEATH;
		monster.CurAction = A_0_STAY;
		monster.RowDelta = 0;
		monster.ColDelta = 0;
		monster.Row = 0;
		monster.Col = 0;
		monster.NextRow = 0;
		monster.NextCol = 0;
		monster.PrevRow = 0;
		monster.PrevCol = 0;
		monster.Orientation = RNG(8);
		monster.SpeedX = 0;
		monster.SpeedY = 0;
		monster.curAnimation = 0;
		monster.AnimationDelayFrameCount = 0;
		monster.AnimationDelayIndex = 0;
		monster.animFrameCount = 0;
		monster.CurFrame = 0;
		monster.flag = MF_15_KILLED;
		monster.TargetIndex = RNG(NetPlayerCount);
		Player& targetPlayer = Players[monster.TargetIndex];
		monster.TargetRow = (char) targetPlayer.NextRow;
		monster.TargetCol = (char) targetPlayer.NextCol;
		monster.SpriteIndex = 0;
		monster.SpritePtr = &MonsterSprites[0];
		monster.BasePtr = &BaseMonsters[BM_109_GOLEM];
	}
}

//----- (0043B601) --------------------------------------------------------
void __fastcall ClearMonsterWalkCounters(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	monster.PrevActionOrRowDeltaOrSpellEffect = A_0_STAY;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.ActionRow_ActOrient = 0;
	monster.NextRowHz = 0;
	monster.NextColHz = 0;
	monster.WalkX = 0;
	monster.WalkY = 0;
	monster.WalkframeCounter = 0;
}

//----- (0043B63A) --------------------------------------------------------
bool __fastcall IsCellFreeForMonster(int row, int col)
{
	int cell = col + 112 * row;
	uchar flag = FlagMap[ 0 ][ cell ];
	if( ! In112(row, col) || MonsterMap[ 0 ][ cell ] || PlayerMap[ 0 ][ cell ] || flag & CF_2_VISIBLE_BY_PLAYER || flag & CF_8_FORBIDDEN ){
		return 0;
	}else{
		return IsCellBlocking(row, col) == 0;
	}
}

//----- (0043B682) --------------------------------------------------------
void OpenWeakUber()
{
	Monster* monster; // esi@4
	if( Dungeon == DUN_24_CRYPT_4 ){
		if( UberDiabloMonsterIndex >= 0 && UberDiabloMonsterIndex < MonstersCount ){
			monster = &Monsters[UberDiabloMonsterIndex];
			PlayMonsterSound(UberDiabloMonsterIndex, MST_2_DEATH);
//			Quests[Q_21_FLESHDOOM].talkToFlag = 0;
			monster->CurrentLife = monster->BaseLife; // / 2; // было уменьшение жизни убера в два раза (отключено в текущей версии th1)
		}
	}
}

//----- (0043B6E5) --------------------------------------------------------
void AddMonstersToPandemonium() // LoadDiabMonsts
{
	ushort* d1 = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab1_cl.DUN" : "Levels\\L4Data\\diab1.DUN" );
	AddQuestMonsters(d1, 2 * DiabloQuad1row, 2 * DiabloQuad1col);
	FreeMem(d1);
	ushort* d2 = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab2a_cl.DUN" : "Levels\\L4Data\\diab2a.DUN");
	AddQuestMonsters(d2, 2 * DiabloQuad2row, 2 * DiabloQuad2col);
	FreeMem(d2);
	ushort* d3 = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab3a_cl.DUN" : "Levels\\L4Data\\diab3a.DUN");
	AddQuestMonsters(d3, 2 * DiabloQuad3row, 2 * DiabloQuad3col);
	FreeMem(d3);
	ushort* d4 = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab4a_cl.DUN" : "Levels\\L4Data\\diab4a.DUN");
	AddQuestMonsters(d4, 2 * DiabloQuad4row, 2 * DiabloQuad4col);
	FreeMem(d4);
}

//----- (0043B798) --------------------------------------------------------
void InitMonsters()
{
	#if MULTI_PLAYER_DEBUG_CHECK
	if( MaxCountOfPlayersInGame != 1 && GameMode != GM_COLISEUM){
		CheckMonsterAndPlayerMapsClearForMultiplayer(); // Disable this to enter MP quest area without crash.  This may create other issues...
	}
	#endif
	if( !Dungeon->isQuest ){
	    if( !SummonMonstersAdded ){
            for( int playerIndex = 0; playerIndex < PlayersMax_4; ++playerIndex ){
                for( int summonIndex = 0; summonIndex < SummonMonstersPerPlayer_Count; ++summonIndex ){
                    AddMonster( 1, 0, 0, 0, 0 );
                }
            }
            SummonMonstersAdded = true;
        }
        
		if( Dungeon == DUN_16_HELL_4 ){
			AddMonstersToPandemonium();
		}
	}

    const int WARPS_DEAD_ZONE_SIZE = 5;
    const int WARPS_DEAD_ZONE_OFFSET = WARPS_DEAD_ZONE_SIZE / 2;
    const int WARPS_DEAD_ZONE_LIGHT_RADIUS = 15;
    
    // Mark visible tiles around warps as prohibited to generate monsters
    for( auto& warp: Warps ){
        if( Dungeon == DUN_15_HELL_3 && warp.GameMessage == WM_1026_NEXT_DUNGEON ) continue;
		for( int warpRowOffset = -WARPS_DEAD_ZONE_OFFSET; warpRowOffset <= +WARPS_DEAD_ZONE_OFFSET; ++warpRowOffset ){
            for( int warpColOffset = -WARPS_DEAD_ZONE_OFFSET; warpColOffset <= +WARPS_DEAD_ZONE_OFFSET; ++warpColOffset ){
                DoVision(warp.Row + warpRowOffset, warp.Col + warpColOffset, WARPS_DEAD_ZONE_LIGHT_RADIUS, 0, 0);
            }
        }
    }

	PlaceQuestMonsters();

	if( !Dungeon->isQuest ){
		LoadBossesOnLevel();
		
		// Calc freeCellsAmount
		const int MAP_CELLS_BEGIN = 16;
		const int MAP_CELLS_END = 96;
		int freeCellsAmount = 0;
		for( int row = MAP_CELLS_BEGIN; row < MAP_CELLS_END; ++row ){
            for( int col = MAP_CELLS_BEGIN; col < MAP_CELLS_END; ++col ){
                if( !IsCellBlocking(row, col) ){
					++freeCellsAmount;
				}
            }
        }
        
        // Choice how many monsters need to be generated
		int maxMonstersToGenerate = freeCellsAmount / 30;
		if( MaxCountOfPlayersInGame != 1 ){
			maxMonstersToGenerate += maxMonstersToGenerate / 2;
        }
			
		int monstersSlotsLeft = Monsters_Max_Count - MonstersCount - 10;
		if( maxMonstersToGenerate > monstersSlotsLeft ){
			maxMonstersToGenerate = monstersSlotsLeft;
        }
        
        MonsterArraySize = MonstersCount + maxMonstersToGenerate;
		
        // Find all valid monster sprites for level
		uint validSprites[BM_COUNT];
	    int validSpritesAmount = 0;
		for( int spriteIndex = 0; spriteIndex < MonsterSpritesCount; ++spriteIndex ){
		    if( MonsterSprites[spriteIndex].placeFlag & MPF_1_SCATTER ){
                validSprites[validSpritesAmount++] = spriteIndex;
            }
        }
        
        // Add monster packs up to MonsterArraySize
		if( validSpritesAmount > 0 ) while( MonstersCount < MonsterArraySize ){
            int spriteIndex = validSprites[RNG(validSpritesAmount)];
            int count = 1;
            if( Dungeon != DUN_1_CHURCH_1 && RNG(2) ){
                if( Dungeon == DUN_2_CHURCH_2 || (Dungeon >= DUN_21_CRYPT_1 && Dungeon <= DUN_24_CRYPT_4) ){
                    count = RNG(2) + 2;
                }else{
                    count = RNG(3) + 3;
                }
            }
            CreateMonstersPack(spriteIndex, count, 0, 0);
        }
	}
 
	// Unmark visible tiles around warps
	for( auto& warp: Warps ){
		if( Dungeon == DUN_15_HELL_3 && warp.GameMessage == WM_1026_NEXT_DUNGEON ) continue;
		for( int warpRowOffset = -WARPS_DEAD_ZONE_OFFSET; warpRowOffset <= +WARPS_DEAD_ZONE_OFFSET; ++warpRowOffset ){
            for( int warpColOffset = -WARPS_DEAD_ZONE_OFFSET; warpColOffset <= +WARPS_DEAD_ZONE_OFFSET; ++warpColOffset ){
                DoUnVision(warp.Row + warpRowOffset, warp.Col + warpColOffset, WARPS_DEAD_ZONE_LIGHT_RADIUS);
            }
        }
    }
	
	MakeChampions(2, 4);// @Champ
}

//----- (0043B9B0) --------------------------------------------------------
void LoadBossesOnLevel()
{
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( UniqueMonster& boss: UniqueMonsters ){
		if( !(boss.avail & mode) || boss.DungeonLevel != Dungeon->level ) continue; // TODO: check for DUNGEON id needing instead of level
		bool isNeedCreateBoss = false;
		int bossPackSpriteIndex = 0;
		for( ; bossPackSpriteIndex < MonsterSpritesCount; ++bossPackSpriteIndex ){
			if( MonsterSprites[bossPackSpriteIndex].baseMonsterIndex == boss.BaseMonsterIndex ){
				isNeedCreateBoss = true; // MonsterSprites[bossPackSpriteIndex].flags & 1; // only sprites for non summons counts
				break;
			}
		}
		if (GameMode == GM_CLASSIC) {
			switch (boss.id) {
			case UM_C_849_GHARBAD: if (!Quests[Q_2_GHARBAD_THE_WEAK].status) isNeedCreateBoss = false; break;
			case UM_C_870_ZHAR_THE_MAD: if (!Quests[Q_3_ZHAR_THE_MAD].status) isNeedCreateBoss = false; break;
			case UM_C_829_SNOTSPILL: if (!Quests[Q_7_OGDENS_SIGN].status) isNeedCreateBoss = false; break;
			case UM_C_902_RAGEHORN: if (!Quests[Q_9_VALOR].status) isNeedCreateBoss = false; break;
			//case UM_678_Warden_of_Anger: if (!Quests[Q_10_ANVIL_OF_FURY].status) isNeedCreateBoss = false; break;
			case UM_C_865_LACHDANAN: if (!Quests[Q_4_LACHDANAN].status) isNeedCreateBoss = false; break;
			case UM_C_871_HAZEWEAVER: if (!Quests[Q_8_HALLS_OF_THE_BLIND].status) isNeedCreateBoss = false; break;
			case UM_C_862_WARLORD: if (!Quests[Q_11_WARLORD_OF_BLOOD].status) isNeedCreateBoss = false; break;
			}
		}
		else {
			switch (boss.id) {
			case UM_0_Gharbad_the_Weak: if (!Quests[Q_2_GHARBAD_THE_WEAK].status) isNeedCreateBoss = false; break;
			case UM_2_Zhar_the_Mad: if (!Quests[Q_3_ZHAR_THE_MAD].status) isNeedCreateBoss = false; break;
			case UM_3_Snotspill: if (!Quests[Q_7_OGDENS_SIGN].status) isNeedCreateBoss = false; break;
			case UM_677_Guardian_of_Valor: if (!Quests[Q_9_VALOR].status) isNeedCreateBoss = false; break;
			case UM_678_Warden_of_Anger: if (!Quests[Q_10_ANVIL_OF_FURY].status) isNeedCreateBoss = false; break;
			case UM_7_Lachdanan: if (!Quests[Q_4_LACHDANAN].status) isNeedCreateBoss = false; break;
			case UM_676_Hazeweaver: if (!Quests[Q_8_HALLS_OF_THE_BLIND].status) isNeedCreateBoss = false; break;
			case UM_8_Warlord_of_Blood: if (!Quests[Q_11_WARLORD_OF_BLOOD].status) isNeedCreateBoss = false; break;
			}
		}
		if( isNeedCreateBoss ){
			int packCount;
			if( Difficulty == 1 ){
				if( MaxCountOfPlayersInGame == 1 ){// purgatory sp
					packCount = 8;
				}
				else{
					packCount = 12;// purg mp
				}
			}
			else if( Difficulty >= 2 ){
				if( MaxCountOfPlayersInGame == 1 ){// doom+ sp
					packCount = 10;
				}
				else{
					packCount = 15;//doom+ mp
				}
			}
			else if( MaxCountOfPlayersInGame == 1 ){// horror sp
				switch( Dungeon->level ){
				case 1: packCount = 2; break;
				case 2: packCount = 3; break;
				case 3: packCount = 4; break;
				case 4: packCount = 5; break;
				default:packCount = 6; break;
				}
			}
			else{// horror mp				
				packCount = Dungeon->level + 1;
				LimitToMax(packCount, 9);
			}
			CreateBoss( boss.id, bossPackSpriteIndex, packCount );
		}
	}
}

int BossRow = -1, BossCol = -1;
//----- (0043BA78) --------------------------------------------------------
void __fastcall CreateBoss(int uniqMonsterIndex, int bossPackSpriteIndex, int packCount, int packFlag /*= 0*/ ) // TODO: clean up decompilation
{
	if( MonstersCount >= Monsters_Max_Count ) return; // was memory corruption
	if( IHateMonsters ) return;
	Monster* boss;              // esi@1
	int v7;                           // eax@6
	int col;                          // ebx@6
	int row;                          // edi@6
	int v10;                          // eax@7
	int v11;                          // edx@8
	int* v13;                         // ecx@48
	int* v14;                         // eax@50
	const UniqueMonster* uniqMonster; // edi@58
	int v18;                          // eax@6
	int* v28;                         // [sp+58h] [bp-14h]@1
	int bossId;						  // [sp+68h] [bp-4h]@1
	int spriteIndex;                  // [sp+60h] [bp-Ch]@2
	int v33;                          // [sp+5Ch] [bp-10h]@6
	int v34;                          // [sp+64h] [bp-8h]@6
	int baseMonsterIndex;                          // [sp+54h] [bp-18h]@8
	char trnFileName[64];                 // [sp+Ch] [bp-60h]@71
	v28 = 0;
	bossId = uniqMonsterIndex;
	boss = &Monsters[MonstersCount];
	if( (BossLightIndex + 19) << 8 >= 6912 ){
		return;
	}
	uniqMonster = &UniqueMonsters[uniqMonsterIndex];
	int ecx = uniqMonster->BaseMonsterIndex;
	if( uniqMonster->PackSpecials > 0 ){
		ecx = uniqMonster->PackSpecials;
	}
	bool isSpriteFind = false;
	for( spriteIndex = 0; spriteIndex < MonsterSpritesCount; spriteIndex++ ){
		if( MonsterSprites[spriteIndex].baseMonsterIndex == ecx ){
			isSpriteFind = true;
			break;
		}
	}
	#ifdef PACK_ON_BOSS
	if( !isSpriteFind ){
		AddMonsterTypeToDungeon(ecx, MPF_1_SCATTER);
	}
	#endif

	// In any MP quest maps with a special boss in it, Boss's randomized stats (e.g. AC) and affix (e.g. Armored) will shuffle upon player re-entry via TP.
	// This is not an issue for non-quest MP maps as they follow a different boss generation code sequence.  It's not clear to me why there are no problems in this case though...
	// For MP quest map boss, the call sequence that affects boss stats is AddQuestMonsters() -> AddBossTypeToQuestLoc() -> CreateBoss() -> CreateBossBaseMonster() -> CreateMonster().
	// In CreateBoss(), the while(1) code here loops over cells to check if a cell is free for monster.  It keeps calling RNG() as it loops.
	// The problem occurs when monsters move around, the cell free state changes, and this changes the number of iterations in the loop, and then changes the number of RNG calls, hence RandomSeed cannot be reproduced upon player re-entry, and we get boss stats and affix shuffle.
	// Prevent random generator from damage, so the seed is reproducible the next time player loads map
	auto randUsingCounter = RandUsingCounter;
	auto randomSeed = RandomSeed;
	auto lastSeed = LastSeed;

	while( 1 ){
		v18 = RNG(80);
		row = v18 + 16;
		v7 = RNG(80);
		v33 = 0;
		int cl = row - 3;
		col = v7 + 16;
		if( BossRow > 0 && BossCol > 0){
			row = BossRow; col = BossCol;
		}
		BossRow = BossCol = -1;
		v34 = row - 3;
		if( row - 3 < row + 3 ){
			v10 = v7 + 19;
			do{
				v11 = col - 3;
				baseMonsterIndex = col - 3;
				while( v11 < v10 ){
					if( In112(v11, v34) && IsCellFreeForMonster(v34, v11) ){
						++v33;
					}
					v10 = col + 3;
					v11 = baseMonsterIndex++ + 1;
				}
				++v34;
				cl = row + 3;
			}while( v34 < row + 3 );
		}
		if( v33 < 9 ){
			v28 = (int*)((char*)v28 + 1);
			if( (int)v28 < 1000 ){
				continue;
			}
		}
		if( IsCellFreeForMonster(row, col) ){
			break;
		}
	}

	RandUsingCounter = randUsingCounter;
	RandomSeed = randomSeed;
	LastSeed = lastSeed;

	if( bossId == UM_3_Snotspill || bossId == UM_C_829_SNOTSPILL ){
		row = 2 * CurPattern.row + 24;
		col = 2 * CurPattern.col + 28;
	}
	if (bossId == UM_677_Guardian_of_Valor || bossId == UM_C_902_RAGEHORN) {
		row = 2 * CurPattern.row + 25;
		col = 2 * CurPattern.col + 19;
	}
	if (bossId == UM_676_Hazeweaver || bossId == UM_C_871_HAZEWEAVER) {
		row = 2 * CurPattern.row + 22;
		col = 2 * CurPattern.col + 22;
	}
	if (bossId == UM_678_Warden_of_Anger) {
		row = 2 * CurPattern.row + 27;
		col = 2 * CurPattern.col + 27;
	}
	if( bossId == UM_8_Warlord_of_Blood || bossId == UM_C_862_WARLORD ){
		row = 2 * CurPattern.row + 22;
		col = 2 * CurPattern.col + 23;
	}
	if( (bossId == UM_2_Zhar_the_Mad || bossId == UM_C_870_ZHAR_THE_MAD) && ChambersCount > 0 ){
		v34 = 0;
		baseMonsterIndex = 1;
		v28 = &Chambers[0].startCol40;
		do{
			if( v34 == ZharTheMadRoomIndex && baseMonsterIndex == 1 ){
				baseMonsterIndex = 0;
				row = 2 * *(v28 - 1) + 20;
				col = 2 * *v28 + 20;
			}
			++v34;
			v28 += 5;
		}while( v34 < ChambersCount );
	}
	if( MaxCountOfPlayersInGame == 1 ){
		switch (bossId) {		
		case UM_579_Andariel:				row = 21;	col = 21;	break;
		case UM_580_NaKrul:					row = 19;	col = 23;	NakrulMonsterIndex = MonstersCount;	break;
		case UM_581_Grimspike:				row = 0;	col = 0;	GrimspikeMonsterIndex = MonstersCount; break;
		case UM_595_The_Cerberus:			row = 0;	col = 0;	CerberusMonsterIndex = MonstersCount; break;
		case UM_582_Izual:					row = 18;	col = 18;	break;
		case UM_585_Fleshdoom:				row = 22;	col = 83;	break;
		case UM_586_Wielder_of_Shadowfang:	row = 22;	col = 83;	break;
		case UM_587_Hephasto_the_Armorer:	row = 22;	col = 83;	break;
		case UM_583_Lich_King:				row = 60;	col = 53;	break;
		case UM_674_Guardian_of_Bones:		if (GameMode != GM_CLASSIC)					   { row = 24;	col = 38; }	break;
		case UM_675_PWS_Boss:				if (Difficulty >= 1 && GameMode != GM_CLASSIC) { row = 19;	col = 19; }	break;
		case UM_584_Mordessa:				row = 30;	col = 30;	break;
		case UM_5_Red_Vex or UM_C_868_RED_VEX:					row = 40;	col = 45;	break;
		case UM_6_Dark_Jade or UM_C_869_BLACKJADE:				row = 38;	col = 49;	break;
		case UM_1_Skeleton_King or UM_C_872_SKING:
		{
			if (Dungeon == DUN_99_ANCIENT_CRYPTS) { 
				row = 85;	col = 27; 
			}
			else { 
				row = 35;	col = 47; 
			}
		}
			break;
		case UM_702_Grimjaw:				row = 23;	col = 90;	break;
		case UM_703_Hive_Mother:			row = 18;	col = 18;	break;
		case UM_704_Borje_the_Ogre:			row = 71;	col = 31;	break;
		case UM_705_Bloodhoof_Twinaxe:		row = 59;	col = 56;	break;
		case UM_706_Mudsnout:				row = 42;	col = 20;	break;
		case UM_707_Lichen_Axe_the_Fungal:	row = 42;	col = 20;	break;
		case UM_708_Grimwing:				row = 72;	col = 38;	break;
		case UM_713_Sir_Valon:				row = 20;	col = 36;	break;
		case UM_714_Sir_Darion:				if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 70;	col = 85; }
											else { row = 20;	col = 32; }	
											break;
		case UM_715_Bishop_Zenda:			if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 67;	col = 85; }
											else { row = 19;	col = 35; }	
											break;
		case UM_716_Bishop_Zamal:			if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 67;	col = 87; }
											else { row = 19;	col = 33; }	
											break;
		case UM_717_Priestess_Varna:		row = 19;	col = 34;	break;
		case UM_718_Guardian_of_Flames:		row = 27;	col = 41;	break;
		case UM_723_The_Infester:			row = 21;	col = 34;	break;
		case UM_724_Silkstrider:			row = 20;	col = 65;	break;
		case UM_725_Web_Queen:				row = 31;	col = 82;	break;
		case UM_726_Apparition_of_Arkaine:	row = 48;	col = 44;	break;
		case UM_740_Don_Carnage:			row = 24;	col = 46;	break;
		case UM_741_Gloom_Prowler:			row = 23;	col = 36;	break;
		case UM_742_Ironhoof:				if (Dungeon == DUN_61_CURSED_SANCTUM) { row = 81;	col = 25; }	break;
		case UM_743_Undertaker:				row = 26;	col = 27;	break;
		case UM_744_Dustwing:				row = 60;	col = 33;	break;
		case UM_416_Pusfroth:				if (Dungeon == DUN_63_FORGOTTEN_OSSUARY) { row = 81;	col = 27; }	break;
		case UM_347_Boneskull_the_Deadly:	if (Dungeon == DUN_63_FORGOTTEN_OSSUARY) { row = 82;	col = 27; }	break;
		case UM_170_Nightrend_the_Dark:		if (Dungeon == DUN_63_FORGOTTEN_OSSUARY) { row = 83;	col = 27; }	break;
		case UM_152_Ashraiser:				if (Dungeon == DUN_63_FORGOTTEN_OSSUARY) { row = 81;	col = 28; }	break;
		case UM_239_Plaguefist_Blackheart:	if (Dungeon == DUN_64_MAUSOLEUM) { row = 45;	col = 31; }	break;
		case UM_220_Blackwillow:			if (Dungeon == DUN_64_MAUSOLEUM) { row = 45;	col = 30; }	break;
		case UM_34_Pukerat_the_Unclean:		if (Dungeon == DUN_64_MAUSOLEUM) { row = 28;	col = 26; }	break;
		case UM_54_Kobal_the_Duke:			if (Dungeon == DUN_65_RUINED_TEMPLE) { row = 81;	col = 23; }	break;
		case UM_191_Shadowcrow:				if (Dungeon == DUN_65_RUINED_TEMPLE) { row = 81;	col = 22; }	break;
		case UM_380_Duskstone:				if (Dungeon == DUN_66_SEWERS) { row = 34;	col = 53; }	break;
		case UM_237_Meatchop_the_Flayer:	if (Dungeon == DUN_66_SEWERS) { row = 33;	col = 53; }	break;
		case UM_368_Goredrool:				if (Dungeon == DUN_66_SEWERS) { row = 32;	col = 53; }	break;
		case UM_419_Deathpuke:				if (Dungeon == DUN_67_SEPULCHER) { row = 25;	col = 25; }	break;
		case UM_53_Viletouch:				if (Dungeon == DUN_67_SEPULCHER) { row = 24;	col = 25; }	break;
		case UM_745_Xamutgor:				row = 81;	col = 38;	break;
		case UM_746_Snarlak:				row = 61;	col = 21;	break;
		case UM_747_Tronzug:				row = 60;	col = 21;	break;
		case UM_748_Fangscar:				row = 54;	col = 31;	break;
		case UM_440_Painwraps:				if (Dungeon == DUN_70_CITADEL) { row = 51;	col = 30; }	break;
		case UM_377_Blazeshock:				if (Dungeon == DUN_70_CITADEL) { row = 52;	col = 31; }	break;
		case UM_143_Bloodscythe_Gloomslayer:if (Dungeon == DUN_71_GRIM_VAULTS) { row = 22;	col = 67; }	break;
		case UM_652_Griefbang:				if (Dungeon == DUN_71_GRIM_VAULTS) { row = 23;	col = 67; }	break;
		case UM_322_Mindgloom:				if (Dungeon == DUN_71_GRIM_VAULTS) { row = 23;	col = 47; }	break;
		case UM_749_Sethmer:				row = 29;	col = 29;	break;
		case UM_750_Fleshbinder:			row = 29;	col = 28;	break;
		case UM_751_Orzog:					row = 0;	col = 0;	OrzogMonsterIndex = MonstersCount; break;
		case UM_379_Arcstorm:				if (Dungeon == DUN_74_CRATER) { row = 38;	col = 21; }	break;
		case UM_132_Phugantar_the_Glowing:	if (Dungeon == DUN_74_CRATER) { row = 37;	col = 21; }	break;
		case UM_133_Fangsoul_the_Hisser:	if (Dungeon == DUN_74_CRATER) { row = 36;	col = 21; }	break;
		case UM_424_Stenchooze:				if (Dungeon == DUN_75_RIVER_OF_FIRE) { row = 47;	col = 83; }	break;
		case UM_52_Firefist_the_Grim:		if (Dungeon == DUN_75_RIVER_OF_FIRE) { row = 47;	col = 82; }	break;
		case UM_542_Steeltouch:				if (Dungeon == DUN_75_RIVER_OF_FIRE) { row = 47;	col = 84; }	break;
		case UM_562_Deathfire:				if (Dungeon == DUN_76_DEN_OF_EVIL) { row = 18 + 44;	col = 87; }	break;
		case UM_436_Gorethrasher:			if (Dungeon == DUN_76_DEN_OF_EVIL) { row = 18 + 44;	col = 87 - 1; }	break;
		case UM_391_Fleshreaper:			if (Dungeon == DUN_76_DEN_OF_EVIL) { row = 18 + 44;	col = 87 - 2; }	break;
		case UM_63_Doomgaze:				if (Dungeon == DUN_76_DEN_OF_EVIL) { row = 18 + 44;	col = 87 - 3; }	break;
		case UM_427_Deathpus_the_Uninterred:if (Dungeon == DUN_77_VILE_TUNNELS) { row = 84;	col = 79 - 21; }	break;
		case UM_628_Moltenstorm_the_Grim:	if (Dungeon == DUN_77_VILE_TUNNELS) { row = 84;	col = 79 - 22; }	break;
		case UM_752_Deathcaller: { row = 18 + 28;	col = 57 - 30; }break;
		case UM_629_Nightburn_the_Mad:		if (Dungeon == DUN_79_COLONNADE) { row = 24 + 1;	col = 57; }break;
		case UM_753_Sister_Priscilla: { row = 24 + 2;	col = 57; }break;
		case UM_754_Madame_Pompadour: { row = 24 + 3;	col = 57; }break;
		case UM_384_Lionsteel_the_Bent:		if (Dungeon == DUN_79_COLONNADE) { row = 24 + 4;	col = 57; }break;
		case UM_169_Urzul_the_Trampler:		if (Dungeon == DUN_80_HELL_GATE) { row = 23;	col = 43; } break;
		case UM_544_Darkhoof:				if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 23; } break;
		case UM_633_Fleshvenom_the_Toxic:	if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 24; } break;
		case UM_19_Jaldabaoth:				if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 25; } break;
		case UM_755_Redbark_Elderroot: { row = 88;	col = 74; } break;
		case UM_49_Oozeshredder:			if (Dungeon == DUN_82_HALLS_OF_SUFFERING) { row = 87;	col = 74; } break;
		case UM_55_Gutooze_the_Foul:		if (Dungeon == DUN_82_HALLS_OF_SUFFERING) { row = 86;	col = 74; } break;
		case UM_613_Rotwound_the_Dark:		if (Dungeon == DUN_83_BLOOD_KEEP) { row = 25;	col = 23; } break;
		case UM_151_Bloodfury:				if (Dungeon == DUN_83_BLOOD_KEEP) { row = 25;	col = 22; } break;
		case UM_624_Nightclaw_the_Sharp:	if (Dungeon == DUN_83_BLOOD_KEEP) { row = 25;	col = 24; } break;
		case UM_756_Paladin_Solo: { row = 32;	col = 35; } break; ///////
		case UM_757_Skullreaper: { row = 88;	col = 23 + 3; } break;
		case UM_136_Sarogyl_the_Decapitator:if (Dungeon == DUN_86_VALLEY_OF_WRATH) { row = 24;	col = 23; } break;
		case UM_495_Asmoday:				if (Dungeon == DUN_87_PANDEMONIUM) { row = 32;	col = 51; } break;
		case UM_758_Lord_of_Terror: { row = 35;	col = 35; } break;
		case UM_575_Hoofrazor:				if (Dungeon == DUN_89_BEDLAM) { row = 30;	col = 87; } break;
		case UM_112_Wreckfist_Gutmauler:	if (Dungeon == DUN_89_BEDLAM) { row = 31;	col = 87; } break;
		case UM_574_Mindzap:				if (Dungeon == DUN_89_BEDLAM) { row = 32;	col = 87; } break;
		case UM_517_Fangrage:				if (Dungeon == DUN_89_BEDLAM) { row = 29;	col = 87; } break;
		case UM_477_Grimblight:				if (Dungeon == DUN_89_BEDLAM) { row = 33;	col = 87; } break;
		case UM_614_Deadpus_the_Unholy:		if (Dungeon == DUN_89_BEDLAM) { row = 31;	col = 88; } break;
		case UM_616_Deathbreak_the_Hammer:	if (Dungeon == DUN_90_FIELDS_OF_MADNESS) { row = /*20*/40;	col = 21; } break;
		case UM_599_Barbossa:				if (Dungeon == DUN_90_FIELDS_OF_MADNESS) { row = /*20*/60;	col = 20; } break;
		case UM_720_The_Marauder:			if (Dungeon == DUN_90_FIELDS_OF_MADNESS) { row = /*20*/80;	col = 22; } break;
		case UM_617_Doomfang_the_Cold:		if (Dungeon == DUN_91_STYGIAN_PASSAGE) { row = 27;	col = 32; } break;
		case UM_627_Hazespawn_the_Shade:	if (Dungeon == DUN_91_STYGIAN_PASSAGE) { row = 38;	col = 38; } break;
		case UM_609_Gutfeast_the_Unclean:	if (Dungeon == DUN_91_STYGIAN_PASSAGE) { row = 49;	col = 49; } break;
		case UM_311_Hulk_Lord:				if (Dungeon == DUN_91_STYGIAN_PASSAGE) { row = 60;	col = 60; } break;
		case UM_656_Devilcrow:				if (Dungeon == DUN_91_STYGIAN_PASSAGE) { row = 71;	col = 71; } break;
		case UM_693_Firedrinker:			if (Dungeon == DUN_92_FLESH_PIT) { row = 37;	col = 35; } break;
		case UM_610_Pitmaim_the_Tainted:	if (Dungeon == DUN_92_FLESH_PIT) { row = 35 + 28;	col = 35; } break;
		case UM_620_Fleshbender:			if (Dungeon == DUN_92_FLESH_PIT) { row = 35 + 19;	col = 35 + 42; } break;
		case UM_250_Shulgoth:				if (Dungeon == DUN_92_FLESH_PIT) { row = 35 + 38;	col = 35 + 17; } break;
		case UM_759_Bonecleaver:			row = 53; col = 20 + 30; break;
		case UM_472_Shadethunder:			if (Dungeon == DUN_93_BLEEDING_VEINS_PATH) { row = 53;	col = 20 + 64; } break;
		case UM_603_Ragespike_the_Violent:	if (Dungeon == DUN_93_BLEEDING_VEINS_PATH) { row = 53 - 22;	col = 20 + 68; } break;
		case UM_478_Fleshdrool_the_Terror:	if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29;	col = 54; } break;
		case UM_474_Sharprage:				if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29;	col = 54 + 24; } break;
		case UM_278_Tartog:					if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29;	col = 54 - 24; } break;
		case UM_492_Fleshfeast:				if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29 - 24;	col = 54; } break;
		case UM_698_Ghoulbone_Headhunter:	if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29 - 24;	col = 54 + 24; } break;
		case UM_502_Scumfeet_the_Foul:		if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 89 - 29 - 24;	col = 54 - 24; } break;
		case UM_611_Plaguepox_the_Destroyer:if (Dungeon == DUN_94_ROT_CHAMBERS) { row = 21;	col = 49; } break;
		case UM_760_DrimMog:				row = 19 + 44;	col = 20 + 14; break;
		case UM_247_Banevenom_the_Toxic:	if (Dungeon == DUN_95_MARROW_GARDEN) { row = 19 + 44 - 24;	col = 20 + 14; } break;
		case UM_761_Jalvathoz:				row = 19 + 44 + 14;	col = 20 + 14 + 8; break;
		case UM_762_Krong:					row = 19 + 44 + 14;	col = 20 + 14 + 8 + 35; break;
		case UM_185_Kasadeus_the_Fangskin:	if (Dungeon == DUN_95_MARROW_GARDEN) { row = 19 + 44 + 14 - 36;	col = 20 + 14 + 8 + 35 - 4; } break;
		case UM_72_Nephalon_the_Cursed:		if (Dungeon == DUN_95_MARROW_GARDEN) { row = 19 + 44 + 14 - 36;	col = 20 + 14 + 8 + 35 - 4 + 15; } break;
		case UM_601_Blackmane_the_Hunter:	if (Dungeon == DUN_95_MARROW_GARDEN) { row = 19 + 44 + 14 - 36 - 20;	col = 20 + 14 + 8 + 35 + 2; } break;
		case UM_615_Fleshspawn_the_Dead:	if (Dungeon == DUN_96_BONE_MIRE) { row = 53;	col = 88 - 35; } break;
		case UM_484_Freezelock:				if (Dungeon == DUN_96_BONE_MIRE) { row = 53;	col = 88 - 35 - 32; } break;
		case UM_648_Forktongue_the_Mangler:	if (Dungeon == DUN_96_BONE_MIRE) { row = 88 - 10;	col = 55 - 32; } break;
		case UM_519_Black_Death:			if (Dungeon == DUN_97_GLOOMY_DEPTHS) { row = 16 + 4; col = 89; } break;
		case UM_621_Moonvex:				if (Dungeon == DUN_97_GLOOMY_DEPTHS) { row = 16 + 4; col = 89 - 12; } break;
		case UM_763_Beelzebub:				row = 16 + 4 + 10; col = 89 - 12; break;
		case UM_764_Lord_Nysallor:			row = 60 - 4; col = 49 - 5; break;
		case UM_765_Duke_of_Abyss:			row = 60 - 4; col = 49 - 7; break;
		case UM_554_Gloomshank:				if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 85 - 40;					col = 27; } break;
		case UM_577_Gorefear:				if (Dungeon == DUN_99_ANCIENT_CRYPTS) {	row = 85 - 40;					col = 27 + 16; } break;
		case UM_76_Sether:					if (Dungeon == DUN_99_ANCIENT_CRYPTS) {	row = 85 - 40;					col = 27 + 16 + 30;	} break;
		case UM_81_Uriel:					if (Dungeon == DUN_99_ANCIENT_CRYPTS) {	row = 85 - 40 + 27;				col = 27 + 16 + 30;	} break;
		case UM_504_Grimblood_the_Frenzied:	if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 85 - 40 + 27 + 14;		col = 27 + 16 + 30 - 13; } break;
		case UM_670_Drilmog:				if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 85 - 40 + 27 + 14 - 22;	col = 27 + 16 + 30 - 13 - 32; } break;
		case UM_766_Zhar_the_Mad_AR:		if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 55 - 30;	col = 24 + 19; } break;
		case UM_89_Alystar:					if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 55 - 30 + 15;	col = 24 + 31; } break;
		case UM_488_Firespawn:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 55 - 30 + 15;	col = 24 + 19 - 14; } break;
		case UM_124_Torvatus:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 54;	col = 30; } break;
		case UM_210_Tombstone_the_Evil:		if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 68;	col = 30; } break;
		case UM_102_Gorestone:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 68 + 17;	col = 30; } break;
		case UM_86_Varaya:					if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 85 - 16;	col = 84; } break;
		case UM_139_Zabulon:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 85 - 16 - 30;	col = 84; } break;
		case UM_767_BerUlmeth:				{ row = 18 + 1;   col = 19; } break;
		case UM_4_ArchBishop_Lazarus or UM_C_867_LAZARUS:
			{
				if (Dungeon == DUN_101_VOID_CHAMBERS) { 
					row = 18 + 1;	col = 19 + 29; 
				}
				else { // standard level 15 quest location
					row = 32;	col = 46;
				}
			}
			break;
		case UM_496_Phaseclaw_the_Vile:		if (Dungeon == DUN_101_VOID_CHAMBERS) { row = 18 + 4 + 10;	col = 19 + 29 + 21; } break;
		case UM_553_Executioner_of_Death:	if (Dungeon == DUN_101_VOID_CHAMBERS) { row = 18 + 4 + 10;	col = 19 + 29 + 21 + 18; } break;
		case UM_671_Turze:					if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 18 + 24;	col = 85 - 9; } break;
		case UM_701_Boneskull_the_Undying:	if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 18 + 40;	col = 85 - 28; } break;
		case UM_768_Blood_Medley_the_Thing:	{ row = 18 + 10;	col = 85 - 46; } break;
		case UM_479_Ironthunder:			if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 32;	col = 21; } break;
		case UM_506_The_Cremator:			if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 63;	col = 19; } break;
		case UM_555_Ghostmace:				if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 63 + 25;	col = 19 + 15; } break;
		case UM_512_Blackslice:				if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 63 + 25;	col = 19 + 21; } break;
		case UM_97_Baal:					if (Dungeon == DUN_102_OBLIVION_TRAIL) { row = 63 + 25 - 19;	col = 19 + 21 + 6; } break;
		case UM_576_Evilshock:				if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) { row = 69;	col = 64; } break;
		case UM_485_Goredrown:				if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) { row = 25;	col = 82; } break;
		case UM_450_Bloodrayne:				if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) { row = 66;	col = 24; } break;
		case UM_508_Blazestorm:				if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) { row = 22;	col = 40; } break;
		case UM_769_Boneslash_the_Revenant:	row = 72;	col = 23; break;
		case UM_770_Graveblade:				row = 56;	col = 23; break;
		case UM_77_Stonelava_Bashfist:		if (Dungeon == DUN_104_NETHERWORLD_GATES) { row = 35;	col = 25; } break;
		case UM_79_Gloomclaw:				if (Dungeon == DUN_104_NETHERWORLD_GATES) { row = 43;	col = 53; } break;
		case UM_83_Doomfang_Bloodgutter:	if (Dungeon == DUN_104_NETHERWORLD_GATES) { row = 22;	col = 53; } break;
		case UM_493_Fleshblight:			if (Dungeon == DUN_104_NETHERWORLD_GATES) { row = 25;	col = 53; } break;
		case UM_138_Lionskull_the_Black:	if (Dungeon == DUN_105_DARK_DESCENT) { row = 23;	col = 85; } break;
		case UM_321_Magnate:				if (Dungeon == DUN_105_DARK_DESCENT) { row = 53;	col = 75; } break;
		case UM_153_UrAnnon:				if (Dungeon == DUN_105_DARK_DESCENT) { row = 53;	col = 45; } break;
		case UM_771_Nightbite:				row = 83;	col = 43; break;
		case UM_196_Vatar_the_Void_Guardian:if (Dungeon == DUN_105_DARK_DESCENT) {	row = 87;	col = 79;	} break;
		case UM_200_Xelith:					if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 21;	col = 21; } break;
		case UM_405_Scareblade:				if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 26;	col = 26; } break;
		case UM_513_Duskshroud:				if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 83;	col = 27; } break;
		case UM_316_Burzul:					if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 89;	col = 21; } break;
		case UM_199_Grimstomp:				if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 89;	col = 89; } break;
		case UM_94_Setebos:					if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 83;	col = 83; } break;
		case UM_623_Griefburn_the_Destroyer:if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 27;	col = 83; } break;
		case UM_201_Blackstorm:				if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 66;	col = 26; } break;
		case UM_99_Belial_the_Disemboweler:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 47;	col = 38; } break;
		case UM_109_Stoneburn_the_Crusher:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 50;	col = 58; } break;
		case UM_197_Blacktongue_Deathbite:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 68;	col = 58; } break;
		case UM_772_Vexaroth:				if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 88;	col = 78; } break;
		case UM_773_The_Dark_Lord:			row = 81;	col = 21; break;
		case UM_774_Swarmbringer:			row = 28;	col = 64; break;
		case UM_788_Grimgar_the_Fallen:		row = 24;	col = 38; break;
		case UM_789_Bonecrunch_the_Risen:	row = 24;	col = 36; break;
		case UM_790_Ghost_of_Brother_Martin:/*if (Dungeon == DUN_110_BLACK_CHAPEL)	*/{ row = 69;	col = 54; } break;
		case UM_791_Festergnaw:				/*if (Dungeon == DUN_110_BLACK_CHAPEL)	*/ { row = 69;	col = 54; } break;
		case UM_805_Festerus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 29;	col = 31; } break;
		case UM_806_Obfuscus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 78;	col = 78; } break;
		case UM_807_Stormire:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 30;	col = 74; } break;
		case UM_808_Winggore:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 85;	col = 48; } break;
		case UM_809_Borosteron:				if (Dungeon == DUN_113_THE_MAZE)	  { row = 87;	col = 23; } break;
		}
	}else{ // MP
		switch( bossId ){
		case UM_556_Pinky:					row = 2 * CurPattern.row + 23; col = 2 * CurPattern.col + 23; break;
		case UM_4_ArchBishop_Lazarus:		row = 2 * CurPattern.row + 22; col = 2 * CurPattern.col + 22; break;
		case UM_557_Dregbez:				row = 2 * CurPattern.row + 22; col = 2 * CurPattern.col + 22; break;
		case UM_578_Uber_Xorene:			row = 2 * CurPattern.row + 22; col = 2 * CurPattern.col + 14; break;
		case UM_537_Ceromorth:				row = 2 * CurPattern.row + 24; col = 2 * CurPattern.col + 24; break;
		case UM_5_Red_Vex:					row = 2 * CurPattern.row + 21; col = 2 * CurPattern.col + 19; break;
		case UM_6_Dark_Jade:				row = 2 * CurPattern.row + 21; col = 2 * CurPattern.col + 25; break;
		case UM_579_Andariel:				row = 21;	col = 21;	break;
		case UM_580_NaKrul:					row = 19;	col = 23;	NakrulMonsterIndex = MonstersCount;	break;
		case UM_581_Grimspike:				row = 0;	col = 0;	GrimspikeMonsterIndex = MonstersCount; break; // Watch out for desync in spawn script
		case UM_582_Izual:					row = 18;	col = 18;	break;
		case UM_585_Fleshdoom:				row = 22;	col = 83;	break;
		case UM_586_Wielder_of_Shadowfang:	row = 22;	col = 83;	break;
		case UM_587_Hephasto_the_Armorer:	row = 22;	col = 83;	break;
		case UM_583_Lich_King:				row = 60;	col = 53;	break;
		case UM_674_Guardian_of_Bones:		if (GameMode != GM_CLASSIC)					   { row = 24;	col = 38; }	break;
		case UM_584_Mordessa:				row = 30;	col = 30;	break;
		case UM_675_PWS_Boss:				if (Difficulty >= 1 && GameMode != GM_CLASSIC) { row = 19;	col = 19; }	break;
		case UM_702_Grimjaw:				row = 23;	col = 90;	break;
		case UM_703_Hive_Mother:			row = 18;	col = 18;	break;
		case UM_704_Borje_the_Ogre:			row = 71;	col = 31;	break;
		case UM_705_Bloodhoof_Twinaxe:		row = 59;	col = 56;	break;
		case UM_706_Mudsnout:				row = 42;	col = 20;	break;
		case UM_707_Lichen_Axe_the_Fungal:	row = 42;	col = 20;	break;
		case UM_708_Grimwing:				row = 72;	col = 38;	break;
		case UM_713_Sir_Valon:				row = 20;	col = 36;	break;
		case UM_714_Sir_Darion:				row = 20;	col = 32;	break;
		case UM_715_Bishop_Zenda:			row = 19;	col = 35;	break;
		case UM_716_Bishop_Zamal:			row = 19;	col = 33;	break;
		case UM_717_Priestess_Varna:		row = 19;	col = 34;	break;
		case UM_718_Guardian_of_Flames:		row = 27;	col = 41;	break;
		case UM_723_The_Infester:			row = 21;	col = 34;	break;
		case UM_724_Silkstrider:			row = 20;	col = 65;	break;
		case UM_725_Web_Queen:				row = 31;	col = 82;	break;
		case UM_774_Swarmbringer:			row = 28;	col = 64;	break;
		case UM_788_Grimgar_the_Fallen:		row = 24;	col = 38;	break;
		case UM_789_Bonecrunch_the_Risen:	row = 24;	col = 36;	break;
		case UM_790_Ghost_of_Brother_Martin:/*if (Dungeon == DUN_110_BLACK_CHAPEL)*/ { row = 69;	col = 54; } break;
		case UM_791_Festergnaw:				/*if (Dungeon == DUN_110_BLACK_CHAPEL)	*/ { row = 69;	col = 54; } break;
		case UM_805_Festerus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 29;	col = 31; } break;
		case UM_806_Obfuscus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 78;	col = 78; } break;
		case UM_807_Stormire:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 30;	col = 74; } break;
		case UM_808_Winggore:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 85;	col = 48; } break;
		case UM_809_Borosteron:				if (Dungeon == DUN_113_THE_MAZE)	  { row = 87;	col = 23; } break;
		case UM_821_Snotspill_MP:			if (Dungeon == DUN_114_LOWER_SCRIPTORIUM) { row = 81;	col = 24; } break;
		}
	}
	int mine = 0;
	switch( bossId ){
	case UM_9_The_Butcher or UM_C_822_The_Butcher:	mine = 367; break;
	case UM_80_Hissabal:	mine = 317; break;
	case UM_32_Lothamaarg:	mine = 319; break;
	}
	if( mine ){
		col = 0;
		v13 = FineMap[ 0 ];
		bool found = false;
		do{
			if( found ){
				break;
			}
			row = 0;
			v14 = v13;
			do{
				if( found ){
					break;
				}
				found = *v14 == mine;
				++row;
				v14 += 112;
			}while( row < 112 );
			++v13;
			++col;
		}while( (int)v13 < (int)&FineMap[ 0 ][ 112 ] );
		++row;
		++col;
	}
	if( bossId == UM_12_Uber_Diablo ){
		if( CheckUberCoord() ){
			UberDiabloMonsterIndex = -1;
			return;
		}
		row = UberRow - 2;
		col = UberCol;
		UberDiabloMonsterIndex = MonstersCount;
	}
	//Gloomclaw cannot spawn under Ubers room now
	/*if( uniqueMonsterIndex == UM_79_Gloomclaw ) {
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow + 6;
		}
	}*/
	// belial spawned to the south from uber's room and too close to stairs sometimes, this makes him spawn on dlvl 24 randomly
	/*else if( uniqueMonsterIndex == UM_99_Belial_the_Disemboweler ){
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow + 6;
		}
	}*/
	switch( bossId ){
	case UM_538_Goreshade:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 1;
		}
		break;
	case UM_539_Nightcarve:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 3;
		}
		break;
	case UM_540_Mistflayer:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 2;
			col = UberCol + 1;
		}
		break;
	case UM_541_Fogweaver:
		col = UberCol;
		if( !CheckUberCoord() ){
			row = UberRow - 2;
			col = UberCol - 1;
		}
		break;
	case UM_C_859_DIABLO:
		row = DiabloQuad4row * 2 + 24;
		col = DiabloQuad4col * 2 + 24;
		break;
	}
	CreateBossBaseMonster(MonstersCount, spriteIndex, row, col);
	boss->newBossId = bossId + 1;// (5) расширение номера босса 0043BCD9
	boss->MonsterLevel += 4 + 3 * Difficulty / 4;
	//boss->Xp *= 2;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && uniqMonster->IceAgeNamePtr) {
		boss->Name = uniqMonster->IceAgeNamePtr;
	}
	else {
		boss->Name = uniqMonster->NamePtr;
	}
	int hp = uniqMonster->HitPoints << 6;
	boss->BaseLife = hp; 
	if( MaxCountOfPlayersInGame == 1 ){
		hp /= 2; // halve HP for SP
		boss->BaseLife = hp;
		if( hp < 64 ){
			boss->BaseLife = 64; // minimal HP = 1
		}
	}
	boss->CurrentLife = boss->BaseLife;
	boss->ai = uniqMonster->AttackType;
	boss->intel = uniqMonster->Intelligence;
	boss->MinDamage = uniqMonster->MinAttackDamage;
	boss->MaxDamage = uniqMonster->MaxAttackDamage;
	boss->SecondMinDamage = uniqMonster->MinAttackDamage;
	boss->SecondMaxDamage = uniqMonster->MaxAttackDamage;
	boss->ResistImmune = uniqMonster->ResistImmune;
	if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_11_VACCINATED) && MaxCountOfPlayersInGame == 1){
		VaccinateMonster(*boss);
	}
	// здесь Lazarus опять входит в состояние разговора
	boss->speechIndex = uniqMonster->SpecialSound;
	if( bossId == UM_10_Aym_the_Lord_of_Torment ){ // mor: some obvious oversight
		boss->LightIndex = 0;
	}else{
		boss->LightIndex = AddLight(boss->Row, boss->Col, 3);
		if (is(bossId, UM_C_868_RED_VEX, UM_C_867_LAZARUS, UM_C_869_BLACKJADE, UM_4_ArchBishop_Lazarus, 
			UM_5_Red_Vex, UM_6_Dark_Jade, UM_676_Hazeweaver, UM_677_Guardian_of_Valor, UM_C_871_HAZEWEAVER, UM_C_902_RAGEHORN)) { //special condition to make lights not be not visible at the very beggining for a fractal of second
			ChangeLightPos(boss->LightIndex, 0, 0);
		}
	}
	if( MaxCountOfPlayersInGame != 1 && boss->ai == AI_29_RedVex ){ // Ai_29_Lazarus_s_Slut / MAI_LazarusHelp
		boss->speechIndex = 0;
	}
	// вторая часть патча на Лазаруса из Д1 1.07-1.09
	if( MaxCountOfPlayersInGame == 1
	 || boss->ai != AI_28_Lazarus
	 || Quests[Q_15_ARCHBISHOP_LAZARUS].status2 <= QS_3_COMPLETE ){
		if( boss->speechIndex ){
			if (Dungeon == DUN_101_VOID_CHAMBERS) { // mor: Lazarus in Void Chambers attacks normally, not engaging in talks
				goto OTAKE; 
			}
			else { 
				boss->goal = MG_6_TALK; 
			}
		}
	}else{
		OTAKE:
		boss->goal = MG_1_ATTACK;
	}
	int netLife;
	int baseLife;
	int life;
	int bm;

	/* ORIG:
	if (gnDifficulty == D_NIGHTMARE) {
		Monst->_mmaxhp = (Monst->_mmaxhp * 3) + (((gbMaxPlayers == 1) ? 50 : 100) << HP_SHIFT);
		Monst->_mhitpoints = Monst->_mmaxhp;
		Monst->mLevel += 15;
		Monst->mExp = (Monst->mExp << 1) + 2000;
		Monst->mMinDamage = (Monst->mMinDamage * 2) + 4;
		Monst->mMaxDamage = (Monst->mMaxDamage * 2) + 4;
		Monst->mMinDamage2 = (Monst->mMinDamage2 * 2) + 4;
		Monst->mMaxDamage2 = (Monst->mMaxDamage2 * 2) + 4;
	}
	else if (gnDifficulty == D_HELL) {
		Monst->_mmaxhp = (Monst->_mmaxhp * 4) + (((gbMaxPlayers == 1) ? 100 : 200) << HP_SHIFT);
		Monst->_mhitpoints = Monst->_mmaxhp;
		Monst->mLevel += 30;
		Monst->mExp = (Monst->mExp << 2) + 4000;
		Monst->mMinDamage = (Monst->mMinDamage * 4) + 6;
		Monst->mMaxDamage = (Monst->mMaxDamage * 4) + 6;
		Monst->mMinDamage2 = (Monst->mMinDamage2 * 4) + 6;
		Monst->mMaxDamage2 = (Monst->mMaxDamage2 * 4) + 6;
	}
	*/

	if (GameMode == GM_CLASSIC) {
		if (Difficulty == 1) {
			boss->Xp *= 2; boss->Xp += 2'000;
			life = (MaxCountOfPlayersInGame == 1 ? 50 : 100) << 6;
			boss->BaseLife = boss->BaseLife * 3 + life;
			boss->CurrentLife = boss->CurrentLife * 3 + life;
			boss->MinDamage = boss->MinDamage * 2 + 4;
			boss->MaxDamage = boss->MaxDamage * 2 + 4;
			boss->SecondMinDamage = boss->SecondMinDamage * 2 + 4;
			boss->SecondMaxDamage = boss->SecondMaxDamage * 2 + 4;
		}
		else if (Difficulty == 2) {
			boss->Xp *= 4; boss->Xp += 4'000;
			life = (MaxCountOfPlayersInGame == 1 ? 100 : 200) << 6;
			boss->BaseLife = boss->BaseLife * 4 + life;
			boss->CurrentLife = boss->CurrentLife * 4 + life;
			boss->MinDamage = boss->MinDamage * 4 + 6;
			boss->MaxDamage = boss->MaxDamage * 4 + 6;
			boss->SecondMinDamage = boss->SecondMinDamage * 4 + 6;
			boss->SecondMaxDamage = boss->SecondMaxDamage * 4 + 6;
		}
	}
	else {
		switch (Difficulty) {
		case DL_0_HORROR:
			boss->Xp *= 2;
			switch (bossId) {
			case UM_9_The_Butcher:
				life = (MaxCountOfPlayersInGame == 1 ? 200 : 400) << 6;
				boss->BaseLife = life;
				boss->CurrentLife = life;
				boss->MinDamage = 8;
				boss->MaxDamage = 14;
				boss->ArmorClass = 17;
				boss->Xp = RngFromRange(700, 900);
				boss->MonsterLevel = 15;
				boss->ToHit = 2;
				break;
			case UM_674_Guardian_of_Bones:
				if (MaxCountOfPlayersInGame != 1) {
					boss->ToHit += 20;
					boss->ArmorClass += 10;
					boss->MonsterLevel += 5;
					boss->MinDamage += 6;
					boss->MaxDamage += 12;
					boss->SecondMinDamage += 6;
					boss->SecondMaxDamage += 12;
				}
				break;
			case UM_678_Warden_of_Anger:
				if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN)) {
					//life = (MaxCountOfPlayersInGame == 1 ? 2000 : 4000) << 6;
					//boss->BaseLife = life;
					//boss->CurrentLife = life;
					boss->MinDamage = 33;
					boss->MaxDamage = 35;
				}
				break;
			case UM_1_Skeleton_King:
				if (MaxCountOfPlayersInGame == 1 && Dungeon != DUN_99_ANCIENT_CRYPTS) {
					boss->BaseLife = 500 * 64;
					boss->CurrentLife = 500 * 64;
					boss->MinDamage = 16;
					boss->MaxDamage = 23;
					boss->ArmorClass = 36;
					boss->Xp = RngFromRange(1800, 2200);
					boss->MonsterLevel = 25;
					boss->ToHit = 5;
				}
				break;
			case UM_8_Warlord_of_Blood:
				boss->BaseLife = 4400 * 64;
				boss->CurrentLife = 4400 * 64;
				boss->MonsterLevel = 50;
				boss->ToHit = 45;
				boss->MinDamage = 60;
				boss->MaxDamage = 62;
				boss->ArmorClass = 75;
				break;
			case UM_587_Hephasto_the_Armorer: // hephasto fix (1) did
				boss->MonsterLevel = 59;		boss->ToHit = 75;	boss->ArmorClass = 70;	break;
			case UM_3_Snotspill:				boss->ToHit = 38;							break;
			case UM_227_Bullseye_Dogmaster:		boss->ToHit += 25;							break;
			case UM_27_Goreskin_the_Striker:	boss->ToHit = 38;							break;
			case UM_65_Malantor:				boss->ToHit = 57;							break;
			case UM_77_Stonelava_Bashfist:		boss->ToHit = 59;							break;
			case UM_97_Baal:					boss->ToHit = 54;							break;
			case UM_99_Belial_the_Disemboweler: boss->ToHit = 49;							break;
			case UM_122_Tahu_the_Dangerous:		boss->ToHit = 45;	boss->ArmorClass = 44;	break;
			case UM_157_Warlord_of_Blood:		boss->ToHit = 30;	boss->ArmorClass = 73;	break;
			case UM_211_Shadowclaw_the_Eater:	boss->ToHit = 32;	boss->ArmorClass = 22;	break;
			case UM_256_Thundercharge:			boss->ToHit = 68;							break;
			case UM_321_Magnate:					boss->ToHit = 53;							break;
			case UM_384_Lionsteel_the_Bent:						boss->ArmorClass = 71;	break;
			case UM_403_Black_Marquis:			boss->ToHit = 45;	boss->ArmorClass = 66;	break;
			case UM_418_Arcturus:				boss->ToHit = 50;	boss->ArmorClass = 34;	break;
			case UM_702_Grimjaw:
			case UM_706_Mudsnout:
			case UM_585_Fleshdoom:				boss->ToHit += 15;	boss->ArmorClass += 10;	break;
			case UM_703_Hive_Mother:
			case UM_586_Wielder_of_Shadowfang:	boss->ToHit += 25;	boss->ArmorClass += 15;	break;
			case UM_713_Sir_Valon:
			case UM_714_Sir_Darion:
			case UM_708_Grimwing:				boss->ToHit += 30;	boss->ArmorClass += 20;	break;
			case UM_725_Web_Queen:
			case UM_723_The_Infester:
			case UM_705_Bloodhoof_Twinaxe:
			case UM_764_Lord_Nysallor:
			case UM_765_Duke_of_Abyss:
				boss->ToHit += 30;
				boss->ArmorClass += 20;
				boss->MinDamage += 10;
				boss->MaxDamage += 15;
				break;
			case UM_707_Lichen_Axe_the_Fungal:	boss->ToHit += 5;	boss->ArmorClass += 10; boss->MaxDamage += 10;	break;
			}
			break;
		case DL_1_PURGATORY:
			netLife = MaxCountOfPlayersInGame == 1 ? 0 : 125;
			baseMonsterIndex = uniqMonster->BaseMonsterIndex;
			if (is(baseMonsterIndex, BM_1_OBLIVION_KNIGHT, BM_50_SKELETON_LORD, BM_51_THE_BUTCHER, BM_58_DEFILER, BM_110_DIABLO, BM_137_UBER_DIABLO, BM_C_391_LEORIC, BM_C_390_BUTCHER)) {
				boss->MonsterLevel += 15;
			}
			else if (is(baseMonsterIndex, BM_99_CLEAVER_BOSS, BM_57_HELL_CLAN, BM_148_TURBAK_THE_DEUCE)) {
				boss->MonsterLevel += 10;
			}
			baseLife = 2 * (boss->BaseLife + 500 * 64) + ((netLife + 125) << 10);
			boss->BaseLife = baseLife;
			boss->CurrentLife = baseLife;
			boss->Xp *= 3;// 3 * (boss->Xp + 1400);
			boss->MinDamage += 75;
			boss->MaxDamage += 90;
			boss->SecondMinDamage += 80;
			boss->SecondMaxDamage += 100;
			switch (bossId) {
			case UM_587_Hephasto_the_Armorer: // hephasto fix (2) did
				boss->MonsterLevel = 90;		boss->ToHit = 170; boss->ArmorClass = 170;	break;
			case UM_3_Snotspill:				boss->ToHit += 20;							break;
			case UM_227_Bullseye_Dogmaster:		boss->ToHit += 25;							break;
			case UM_8_Warlord_of_Blood:			boss->ToHit += 40; boss->ArmorClass += 20;	break;
			case UM_27_Goreskin_the_Striker:	boss->ToHit += 20;							break;
			case UM_65_Malantor:				boss->ToHit += 40;							break;
			case UM_77_Stonelava_Bashfist:		boss->ToHit += 25;							break;
			case UM_97_Baal:					boss->ToHit += 20;							break;
			case UM_99_Belial_the_Disemboweler: boss->ToHit += 15;							break;
			case UM_675_PWS_Boss:
			case UM_122_Tahu_the_Dangerous:		boss->ToHit += 25; boss->ArmorClass += 20;	break;
			case UM_702_Grimjaw:
			case UM_706_Mudsnout:
			case UM_157_Warlord_of_Blood:		boss->ToHit += 15; boss->ArmorClass += 15;	break;
			case UM_211_Shadowclaw_the_Eater:	boss->ToHit += 15; boss->ArmorClass += 5;	break;
			case UM_256_Thundercharge:			boss->ToHit += 40;							break;
			case UM_321_Magnate:					boss->ToHit += 15;							break;
			case UM_384_Lionsteel_the_Bent:					   boss->ArmorClass += 25;	break;
			case UM_403_Black_Marquis:			boss->ToHit += 30; boss->ArmorClass += 20;	break;
			case UM_418_Arcturus:				boss->ToHit += 25; boss->ArmorClass += 15;	break;
			case UM_585_Fleshdoom:				boss->ToHit += 25;	boss->ArmorClass += 15;	break;
			case UM_703_Hive_Mother:
			case UM_708_Grimwing:
			case UM_713_Sir_Valon:
			case UM_714_Sir_Darion:
			case UM_586_Wielder_of_Shadowfang:	boss->ToHit += 30;	boss->ArmorClass += 20;	break;
			case UM_725_Web_Queen:
			case UM_723_The_Infester:
			case UM_705_Bloodhoof_Twinaxe:
			case UM_764_Lord_Nysallor:
			case UM_765_Duke_of_Abyss:
				boss->ToHit += 40;
				boss->ArmorClass += 30;
				boss->MinDamage += 20;
				boss->MaxDamage += 30;
				break;
			case UM_707_Lichen_Axe_the_Fungal:	boss->ToHit += 10;	boss->ArmorClass += 15; boss->MaxDamage += 20;	break;
			case UM_674_Guardian_of_Bones:
				if (MaxCountOfPlayersInGame != 1) {
					boss->ToHit += 25;
					boss->ArmorClass += 15;
					boss->MonsterLevel += 7;
					boss->MinDamage += 8;
					boss->MaxDamage += 16;
					boss->SecondMinDamage += 8;
					boss->SecondMaxDamage += 16;
				}
				break;
			case UM_9_The_Butcher:
				if (is(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON) || is(GameMode, GM_HARDCORE, GM_EASY)) {
					life = (MaxCountOfPlayersInGame == 1 ? 4500 : 9000) << 6;
					boss->BaseLife = life;
					boss->CurrentLife = life;
					boss->MinDamage = 69;
					boss->MaxDamage = 86;
					boss->ArmorClass = 100;
					//boss->Xp = 50000;
					boss->MonsterLevel = 37;
					boss->ToHit = 101;
				}
				break;
			case UM_1_Skeleton_King:
				if ((is(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON) || is(GameMode, GM_HARDCORE, GM_EASY))
					&& (MaxCountOfPlayersInGame == 1 && Dungeon != DUN_99_ANCIENT_CRYPTS)) {
					life = 6000 << 6;
					boss->BaseLife = life;
					boss->CurrentLife = life;
					boss->MinDamage = 80;
					boss->MaxDamage = 104;
					boss->ArmorClass = 113;
					//boss->Xp = 50000;
					boss->MonsterLevel = 43;
					boss->ToHit = 125;
				}
				break;
			}
			break;// case DL_1_PURGATORY
		case DL_2_DOOM:
		case DL_3_CRUCIBLE:
		case DL_4_TORMENT:
		case DL_5_AGONY:
		case DL_6_TERROR:
		case DL_7_DESTRUCTION:
		case DL_8_INFERNO:

			// ------------- special extra monster level boosts -------------
			bm = uniqMonster->BaseMonsterIndex;
			if (is(bm, BM_1_OBLIVION_KNIGHT, BM_50_SKELETON_LORD, BM_51_THE_BUTCHER, BM_58_DEFILER, BM_110_DIABLO, BM_137_UBER_DIABLO, BM_C_390_BUTCHER, BM_C_391_LEORIC)) {
				boss->MonsterLevel += (15 * Difficulty);
			}
			else if (is(bm, BM_99_CLEAVER_BOSS, BM_57_HELL_CLAN, BM_148_TURBAK_THE_DEUCE, BM_193_ABATAN, BM_195_UBER_BUTCHER, BM_197_CEROMORTH)) {
				boss->MonsterLevel += (10 * Difficulty);
			}
			else if (bm == BM_205_PINKY) {
				boss->MonsterLevel += (12 * Difficulty);
			}

			//----------------------------- boss HP ----------------------------
			int internal_hp_multiplier = 64 * (2 * Difficulty - 1);
			baseLife = (5 * Difficulty / 2) // was * 3 before. *2 gave not enough difference in HP as compared to minions (not alway above 200%), 250% is okayish
				* (boss->BaseLife + ((MaxCountOfPlayersInGame != 1 ? 1000 : 500) * internal_hp_multiplier))
				+ (250 * 64 * Difficulty);
			boss->BaseLife = baseLife;
			boss->CurrentLife = baseLife;

			// ------------------- boss XP formula (TH3 v0.662+) --------------------------
			if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) {
				switch (Difficulty) {
				case 0:
				case 1:
				case 2: boss->Xp += boss->Xp * 4;		break;// 5x
				case 3: boss->Xp += boss->Xp * 3;		break;// 4x
				case 4: boss->Xp += 5 * boss->Xp / 2;	break;// 3,5x
				case 5: boss->Xp += boss->Xp * 2;		break;// 3x
				case 6: boss->Xp += 3 * boss->Xp / 2;	break;// 2,5x
				case 7: boss->Xp += boss->Xp;			break;// 2x
				case 8: boss->Xp += boss->Xp / 2;		break;// 1,5x: max XP = 15'000 (Uber). turns into 80M on Inferno. x15 (1,5-2B) on boosted XP in these modes. Near the limit
				}
			}
			else {
				switch (Difficulty) {
				case 0:
				case 1:
				case 2:	boss->Xp += 5 * boss->Xp / 2;	break;// 3,5x
				case 3:	boss->Xp += 3 * boss->Xp;		break;// 4x
				case 4:	boss->Xp += 7 * boss->Xp / 2;	break;// 4,5x
				case 5:	boss->Xp += 4 * boss->Xp;		break;// 5x
				case 6:	boss->Xp += 9 * boss->Xp / 2;	break;// 5,5x
				case 7:	boss->Xp += 5 * boss->Xp;		break;// 6x
				case 8:	boss->Xp += 11 * boss->Xp / 2;	break;// 6,5x
				}
			}

			//int addups_boss = Difficulty == DL_8_INFERNO ? (Dungeon->level * 2) : 0;// old addon
			// ADDUPS
			int addups_boss = 0;
			switch (Difficulty) {
			case DL_5_AGONY:		addups_boss = Dungeon->level; break;
			case DL_6_TERROR:		addups_boss = 24 + 3 * Dungeon->level / 2; break;
			case DL_7_DESTRUCTION:	addups_boss = 24 + 36 + 2 * Dungeon->level; break;
			case DL_8_INFERNO:		addups_boss = 24 + 36 + 48 + 3 * Dungeon->level; break;
			}
			boss->MinDamage += 75 * Difficulty + addups_boss;
			boss->MaxDamage += 90 * Difficulty + addups_boss;
			boss->SecondMinDamage += 80 * Difficulty + addups_boss;
			boss->SecondMaxDamage += 100 * Difficulty + addups_boss;
			switch (bossId) {
			case UM_587_Hephasto_the_Armorer: // hephasto fix (3) did
				boss->MonsterLevel = 60 + (30 * Difficulty);
				boss->ToHit = 100 + (80 * Difficulty);
				boss->ArmorClass = 100 + (80 * Difficulty); break;
			case UM_3_Snotspill:				boss->ToHit += 20;							break;
			case UM_227_Bullseye_Dogmaster:		boss->ToHit += 25;							break;
			case UM_8_Warlord_of_Blood:			boss->ToHit += 45;	boss->ArmorClass += 20;	break;
			case UM_27_Goreskin_the_Striker:	boss->ToHit += 20;							break;
			case UM_65_Malantor:				boss->ToHit += 40;							break;
			case UM_77_Stonelava_Bashfist:		boss->ToHit += 25;							break;
			case UM_97_Baal:					boss->ToHit += 20;							break;
			case UM_99_Belial_the_Disemboweler: boss->ToHit += 15;							break;
			case UM_675_PWS_Boss:
			case UM_122_Tahu_the_Dangerous:		boss->ToHit += 25;	boss->ArmorClass += 20;	break;
			case UM_702_Grimjaw:
			case UM_706_Mudsnout:
			case UM_157_Warlord_of_Blood:		boss->ToHit += 15;	boss->ArmorClass += 15;	break;
			case UM_211_Shadowclaw_the_Eater:	boss->ToHit += 15;	boss->ArmorClass += 5;	break;
			case UM_256_Thundercharge:			boss->ToHit += 40;							break;
			case UM_321_Magnate:					boss->ToHit += 15;							break;
			case UM_384_Lionsteel_the_Bent:						boss->ArmorClass += 25;	break;
			case UM_713_Sir_Valon:
			case UM_714_Sir_Darion:
			case UM_403_Black_Marquis:			boss->ToHit += 30;	boss->ArmorClass += 20;	break;
			case UM_418_Arcturus:				boss->ToHit += 25;	boss->ArmorClass += 15;	break;
			case UM_585_Fleshdoom:				boss->ToHit += 35;	boss->ArmorClass += 25;	break;
			case UM_707_Lichen_Axe_the_Fungal:
				boss->ToHit += 5 + Difficulty * 5;
				boss->ArmorClass += 10 + Difficulty * 5;
				boss->MaxDamage += 10 + Difficulty * 10;
				break;
			case UM_703_Hive_Mother:
			case UM_708_Grimwing:
			case UM_586_Wielder_of_Shadowfang:
				boss->ToHit += 40;
				boss->ArmorClass += 30;
				boss->MinDamage += 15;
				boss->MaxDamage += 30;
				break;
			case UM_725_Web_Queen:
			case UM_723_The_Infester:
			case UM_705_Bloodhoof_Twinaxe:
			case UM_764_Lord_Nysallor:
			case UM_765_Duke_of_Abyss:
				boss->ToHit += 30 + 10 * Difficulty;
				boss->ArmorClass += 20 + 10 * Difficulty;
				boss->MinDamage += 10 + 10 * Difficulty;
				boss->MaxDamage += 15 + 15 * Difficulty;
				break;
			case UM_674_Guardian_of_Bones:
				boss->ToHit += 30;
				boss->ArmorClass += 20;
				boss->MonsterLevel += 10;
				boss->MinDamage += 10;
				boss->MaxDamage += 20;
				boss->SecondMinDamage += 10;
				boss->SecondMaxDamage += 20;
				break;
			case UM_9_The_Butcher:
				if (is(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON) || is(GameMode, GM_HARDCORE, GM_EASY)) {
					int butch_hp = (by(Difficulty, 200, 7'000, 17'000, 38'000, 67'000, 95'000, 130'000, 170'000, 220'000)) << 6;
					life = MaxCountOfPlayersInGame == 1 ? butch_hp : (butch_hp * 2);
					boss->BaseLife = life;
					boss->CurrentLife = life;
					boss->MinDamage = 131 + ((Difficulty - 2) * 75) + addups_boss;
					boss->MaxDamage = 164 + ((Difficulty - 2) * 90) + addups_boss;
					boss->ArmorClass = 200 + ((Difficulty - 2) * 80) + addups_boss;
					//boss->Xp = 50000;
					boss->MonsterLevel = 49 + ((Difficulty - 2) * 25);
					boss->ToHit = 208 + ((Difficulty - 2) * 85) + addups_boss;
				}
				break;
			case UM_1_Skeleton_King:
				if ((is(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON) || is(GameMode, GM_HARDCORE, GM_EASY))
					&& (MaxCountOfPlayersInGame == 1 && Dungeon != DUN_99_ANCIENT_CRYPTS)) {
					life = (by(Difficulty, 300, 8'000, 20'000, 41'000, 72'000, 105'000, 145'000, 180'000, 230'000)) << 6;;
					boss->BaseLife = life;
					boss->CurrentLife = life;
					boss->MinDamage = 152 + ((Difficulty - 2) * 75) + addups_boss;
					boss->MaxDamage = 194 + ((Difficulty - 2) * 90) + addups_boss;
					boss->ArmorClass = 217 + ((Difficulty - 2) * 80) + addups_boss;
					//boss->Xp = 50000;
					boss->MonsterLevel = 56 + ((Difficulty - 2) * 25);
					boss->ToHit = 237 + ((Difficulty - 2) * 85) + addups_boss;
				}
				break;
			case UM_12_Uber_Diablo:
			case UM_773_The_Dark_Lord:
				boss->Xp *= 2;
				break;
			}
			break;
		}

		// other (universal, not per-difficulty) boosts
		switch (bossId) {
		case UM_12_Uber_Diablo:
			boss->ToHit += 10 + Difficulty * 10;
			boss->ArmorClass += 15 + Difficulty * 10;
			boss->MinDamage += 10 + Difficulty * 10;
			boss->MaxDamage += 15 + Difficulty * 15;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------

	if (GameMode == GM_HARD) {
		boss->BaseLife += boss->BaseLife / 2;
		boss->CurrentLife = boss->BaseLife;
	}
	else if (GameMode == GM_EASY) { // -40% HP in Easy mode
		boss->BaseLife -= 2 * boss->BaseLife / 5;
		boss->CurrentLife = boss->BaseLife;
	}
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_17_TOUGH_MNSTR)) {
		boss->BaseLife *= 3;
		boss->CurrentLife = boss->BaseLife;
	}
	if( is( GameMode, GM_NORMAL, GM_HARD )) { // boss affixes
		int affixChance = Difficulty * 6 + Dungeon->level / 4 + 1; // 1-7, 7-13, 13-19, 19-25, 25-31, 31-37, 37-43, 43-49, 49-55
		if( not(bossId, 
		 UM_587_Hephasto_the_Armorer,
		 UM_227_Bullseye_Dogmaster,
		 UM_27_Goreskin_the_Striker,
		 UM_65_Malantor,
		 UM_77_Stonelava_Bashfist,
		 UM_97_Baal,
		 UM_99_Belial_the_Disemboweler,
		 UM_122_Tahu_the_Dangerous,
		 UM_8_Warlord_of_Blood,
		 UM_157_Warlord_of_Blood,
		 UM_211_Shadowclaw_the_Eater,
		 UM_256_Thundercharge,
		 UM_321_Magnate,
		 UM_384_Lionsteel_the_Bent,
		 UM_403_Black_Marquis,
		 UM_418_Arcturus,
		 UM_154_Diablo,
		 UM_12_Uber_Diablo,
		 UM_9_The_Butcher,
		 UM_536_Uber_Butcher,
		 UM_0_Gharbad_the_Weak,
		 UM_1_Skeleton_King,
		 UM_2_Zhar_the_Mad,
		 UM_3_Snotspill,
		 UM_4_ArchBishop_Lazarus,
		 UM_5_Red_Vex,
		 UM_6_Dark_Jade,
		 UM_7_Lachdanan,
		 UM_11_The_Defiler,
		 UM_595_The_Cerberus,
		 UM_586_Wielder_of_Shadowfang,
		 UM_585_Fleshdoom,
		 UM_584_Mordessa,
		 UM_583_Lich_King,
		 UM_582_Izual,
		 UM_581_Grimspike,
		 UM_579_Andariel,
		 UM_578_Uber_Xorene,
		 UM_557_Dregbez,
		 UM_556_Pinky,
		 UM_725_Web_Queen,
		 UM_726_Apparition_of_Arkaine,
		 UM_723_The_Infester,
		 UM_718_Guardian_of_Flames,
		 UM_717_Priestess_Varna,
		 UM_716_Bishop_Zamal,
		 UM_715_Bishop_Zenda,
		 UM_714_Sir_Darion,
		 UM_713_Sir_Valon,
		 UM_704_Borje_the_Ogre,
		 UM_703_Hive_Mother,
		 UM_702_Grimjaw,
		 UM_724_Silkstrider,
		 UM_537_Ceromorth,
		 UM_535_Abatan,
		 UM_580_NaKrul)) { //list of prohibited monster to get affixes	{
			int affixAmount = by(Difficulty, 1, 2, 3, 3, 3, 3, 3, 3, 3);
			for( int i = 0; i < affixAmount; i++ ){
				if( affixChance > RNG(100) ){
					bool duplicate = false;
					do{
						uchar nextAffix = RNG(MAF_LAST) + 1;
						for( int j = 0; j < i; j++ ){ duplicate = boss->affixes[j] == nextAffix; if( duplicate ) break; }
						if( !duplicate ){ boss->affixes[i] = nextAffix; }
					}while( duplicate );
				}else{
					--affixAmount;
					--i;
				}
			}
			for( int i = 0; i < affixAmount; ++i ){
				switch( boss->affixes[i] ){
				case MAF_ACCURATE:
					boss->ToHit = boss->ToHit * 110 / 100 + 10;
					boss->SecondToHit = boss->SecondToHit * 110 / 100 + 10;
					break;
				case MAF_STRONG:
					boss->MinDamage = boss->MinDamage * 13 / 10 + 2;
					boss->MaxDamage = boss->MaxDamage * 13 / 10 + 2;
					boss->SecondMinDamage = boss->SecondMinDamage * 13 / 10 + 2;
					boss->SecondMaxDamage = boss->SecondMaxDamage * 13 / 10 + 2;
					break;
				case MAF_TOUGH:
					boss->BaseLife += boss->BaseLife / 2;
					boss->CurrentLife = boss->BaseLife;
					break;
				case MAF_DODGING:
					boss->ArmorClass = boss->ArmorClass * 110 / 100 + 10;
					break;
				}
			}
		}
	}
	const char* trn = MaxCountOfPlayersInGame == 1
		? (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) ? "CF\\ICEAGE\\%s.TRN" : by(Difficulty, "CF\\SH\\%s.TRN", "CF\\SP\\%s.TRN", "CF\\SD\\%s.TRN"))
		: by( Difficulty, "Monsters\\Monsters\\%s.TRN", "CF\\MP\\%s.TRN", "CF\\MD\\%s.TRN" );
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && uniqMonster->IceAgeColorTranslationFile) {
		sprintf(trnFileName, trn, uniqMonster->IceAgeColorTranslationFile);
	}
	else {
		sprintf(trnFileName, trn, uniqMonster->ColorTranslationFile);
	}
	LoadFileWithMem( trnFileName, &LightTable[256 * (BossLightIndex + 19)] );
	boss->uniqTrans = BossLightIndex++;
	++MonstersCount;
	packFlag = packFlag ? packFlag : uniqMonster->PackTrigger; // if passed non zero packFlag use it, instead use PackTrigger from UniqueMonsters table
	if( packFlag & UN_1_PACK ){
		CreateMonstersPack(bossPackSpriteIndex, packCount, packFlag, MonstersCount - 1);
	}
	if( boss->ai != AI_12_Gargoyle ){
		boss->curAnimation = &boss->SpritePtr->animation[ANIM_0_STAY].oriented[boss->Orientation];
		boss->flag &= ~MF_3_STONE;
		boss->CurAction = A_0_STAY;
		boss->CurFrame = RNG(boss->animFrameCount - 1) + 1;
	}
	return;
}

//----- (0043BFD5) --------------------------------------------------------
void __fastcall CreateBossBaseMonster(int monsterIndex, int spriteIndex, int row, int col)
{
	if( MonsterSprites[spriteIndex].baseMonsterIndex != BM_137_UBER_DIABLO || MonstersCount <= 0 ){
		MonsterMap[ row ][ col ] = monsterIndex + 1; // TODO: add check and adjust for already occupied cell
		CreateMonster(monsterIndex, RNG(8), spriteIndex, row, col);
	}else{
		int i = 0;
		while( Monsters[i].SpriteIndex != spriteIndex && Monsters[i].SpritePtr->baseMonsterIndex != BM_137_UBER_DIABLO ){
			if( ++i >= MonstersCount ){
				MonsterMap[ row ][ col ] = monsterIndex + 1;
				CreateMonster(monsterIndex, RNG(8), spriteIndex, row, col);
			}
		}
	}
}

//----- (0043C057) --------------------------------------------------------
void PlaceQuestMonsters()
{
	//int v0; // esi@7
	//MonsterSprite *fn; // edi@8
	void *ogdens_sign_dun; // esi@54
	void *valor_dun; // esi@15
	void *hotb_dun; // esi@17
	void *anvil_dun; // esi@60
	void *wob_dun; // esi@21
	void *lazarus_entry_dun; // esi@29
	int v8; // ecx@31
	MonsterSprite *v9; // edx@32
	int v10; // esi@36
	Monster *v11; // eax@37
	//char *v12; // ecx@51
	//char *v13; // ecx@57
	int bossPackSpriteIndex; // edx@62
	//int bossPackSpriteIndex_3; // eax@66
	//int bossPackSpriteIndex_1; // eax@68
	//int bossPackSpriteIndex_2; // eax@70
	char *uber_diablo_room_type; // ecx@75
	void *uber_d_dun; // esi@76
	void *sunless_sea_dun; // esi@83
	void* iron_figurine_dun;
	void* demon_plague_entry_dun;
	int boss_id; // [sp-8h] [bp-14h]@44

	if( Dungeon->isQuest ){
		if( Dungeon != DUN_25_KING_LEORICS_TOMB ){ // TODO: clean up by original
			return;
		}
		boss_id = GameMode == GM_CLASSIC ? UM_C_872_SKING : UM_1_Skeleton_King;
		goto LABEL_47;
	}
	if( IsPinkyOnLevel() ){
		UnMarkPatternPositionOnMap();
		bossPackSpriteIndex = AddMonsterTypeToDungeon( BM_206_ACID_SPITTER, MPF_2_DONT );
		if (bossPackSpriteIndex > -1) {
			CreateBoss(UM_556_Pinky, bossPackSpriteIndex, 18);
		}
		MarkPatternPositionOnMap();
	}
	if( IsUberXoreneOnLevel() ){
		UnMarkPatternPositionOnMap();
		bossPackSpriteIndex = AddMonsterTypeToDungeon( BM_216_ZEALOT, MPF_2_DONT );
		if (bossPackSpriteIndex > -1) {
			CreateBoss(UM_578_Uber_Xorene, bossPackSpriteIndex, 8);
		}
		MarkPatternPositionOnMap();
	}
	else if( IsCeromorthOnLevel() ){
		UnMarkPatternPositionOnMap();
		AddMonsterTypeToDungeon( BM_197_CEROMORTH, MPF_2_DONT );
		bossPackSpriteIndex = AddMonsterTypeToDungeon( BM_198_HORN_CLAN, MPF_2_DONT );
		if (bossPackSpriteIndex > -1) {
			CreateBoss(UM_537_Ceromorth, bossPackSpriteIndex, 18);
		}
		MarkPatternPositionOnMap();
	}
	if( IsDregbezOnLevel() && SetCurPattern(Q_NO) ){
		UnMarkPatternPositionOnMap();
		AddMonsterTypeToDungeon( BM_207_DREGBEZ, MPF_2_DONT );
		bossPackSpriteIndex = AddMonsterTypeToDungeon( BM_208_WARPED_ONE, MPF_2_DONT );
		if (bossPackSpriteIndex > -1) {
			CreateBoss(UM_557_Dregbez, bossPackSpriteIndex, 8);
		}
		MarkPatternPositionOnMap();
		RestoreCurPattern();
	}
	if( IsQuestOnLevel(Q_6_THE_BUTCHER) && SetCurPattern(Q_6_THE_BUTCHER) ){
		CreateBoss( GameMode == GM_CLASSIC ? UM_C_822_The_Butcher : UM_9_The_Butcher, 0, 0 );
		RestoreCurPattern();
	}
	if( Dungeon == Quests[ Q_12_THE_CURSE_OF_KING_LEORIC ].dungeonStart && MaxCountOfPlayersInGame != 1 ){
		int bossPackSpriteIndex;
		bool isPackSpriteChanged = false;
		for( bossPackSpriteIndex = 0; bossPackSpriteIndex < MonsterSpritesCount; bossPackSpriteIndex++ ){
			if( IsMonsterSummonable( MonsterSprites[ bossPackSpriteIndex ].baseMonsterIndex ) ){
				isPackSpriteChanged = true;
				break;
			}
		}
		if( !isPackSpriteChanged ){
			TerminateWithError( "CreateBosses (0043C057) error: PackSpecials not changed" );
		}
		CreateBoss( UM_1_Skeleton_King, bossPackSpriteIndex, 30 );
	}
	if( !Dungeon->isQuest && Dungeon == DUN_24_CRYPT_4 ){
		if( MaxCountOfPlayersInGame == 1 || Difficulty < 2 ){
			uber_diablo_room_type = "levels\\l5data\\uberoom.dun";
		}else{
			uber_diablo_room_type = "levels\\l5data\\uberoom1.dun";
		}
		uber_d_dun = LoadFile( uber_diablo_room_type);
		AddQuestMonsters( (ushort*) uber_d_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( uber_d_dun );
	}
	if( IsQuestOnLevel( Q_7_OGDENS_SIGN ) ){
		ogdens_sign_dun = LoadFile( by(Difficulty, 
			"Levels\\L1Data\\Banner1.DUN", 
			"Levels\\L1Data\\Bannerpg.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN", 
			"Levels\\L1Data\\Bannerdm.DUN") );
		if (GameMode == GM_CLASSIC) ogdens_sign_dun = LoadFile("Levels\\L1Data\\Banner_classique.DUN");
		AddQuestMonsters( (ushort*) ogdens_sign_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( ogdens_sign_dun );
	}
	if( IsQuestOnLevel( Q_9_VALOR ) ){
		valor_dun = LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blood2_cls.DUN" : "Levels\\L2Data\\Blood2.DUN" );
		AddQuestMonsters( (ushort*) valor_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( valor_dun );
	}
	if( IsQuestOnLevel( Q_8_HALLS_OF_THE_BLIND ) ){
		hotb_dun = LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blind2_cl.DUN" : "Levels\\L2Data\\Blind2.DUN" );
		AddQuestMonsters( (ushort*) hotb_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( hotb_dun );
	}
	if( IsQuestOnLevel( Q_10_ANVIL_OF_FURY ) ){
		if (GameMode == GM_CLASSIC) {
			anvil_dun = LoadFile("Levels\\L3Data\\Anvil_cl.DUN");
		}
		else {
			anvil_dun = LoadFile(by(Difficulty,
				"Levels\\L3Data\\Anvil.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN",
				"Levels\\L3Data\\Anvil2.DUN"));
		}
		AddQuestMonsters( (ushort*) anvil_dun, 2 * CurPattern.row + 2, 2 * CurPattern.col + 2 );
		FreeMem( anvil_dun );
	}else if( IsQuestOnLevel( Q_20_SUNLESS_SEA ) ){
		sunless_sea_dun = LoadFile( "levels\\L3data\\islandentry.dun" );
		AddQuestMonsters( (ushort*) sunless_sea_dun, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1 );
		FreeMem( sunless_sea_dun );
	}
	//if( QuestPattern* qp = GetQuestPattern(Q_36_CLEAR_WATER) ){
	if( IsQuestOnLevel(Q_36_CLEAR_WATER) && SetCurPattern(Q_36_CLEAR_WATER) ){
		iron_figurine_dun = LoadFile("levels\\L3data\\swampy_cavern_entry.dun", 0);
		//AddQuestMonsters((ushort*)iron_figurine_dun, 2 * qp->row + 1, 2 * qp->col + 1);
		AddQuestMonsters((ushort*)iron_figurine_dun, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1);
		FreeMem(iron_figurine_dun);
		RestoreCurPattern();
	}
	else if (IsQuestOnLevel(Q_29_IRON_FIGURINE)) {
		iron_figurine_dun = LoadFile("levels\\L3data\\swampy_cavern_entry.dun");
		AddQuestMonsters((ushort*)iron_figurine_dun, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1);
		FreeMem(iron_figurine_dun);
	}
	else if (IsQuestOnLevel(Q_30_DEMON_PLAGUE)) {
		demon_plague_entry_dun = LoadFile("levels\\L3data\\mossy_grotto_entry.dun"); 
		AddQuestMonsters((ushort*)demon_plague_entry_dun, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1); 
		FreeMem(demon_plague_entry_dun); 
	}

	if( IsQuestOnLevel( Q_11_WARLORD_OF_BLOOD ) ){
		wob_dun = LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\Warlord_CL.DUN" : "Levels\\L4Data\\Warlord.DUN");
		AddQuestMonsters( (ushort*) wob_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( wob_dun );
		AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_862_WARLORD : UM_8_Warlord_of_Blood ].BaseMonsterIndex, MPF_1_SCATTER );
	}
	if( IsQuestOnLevel( Q_4_LACHDANAN ) ){
		AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_865_LACHDANAN : UM_7_Lachdanan ].BaseMonsterIndex, MPF_1_SCATTER );
		Quests[Q_4_LACHDANAN].status4 = 0;
	}
	if( IsQuestOnLevel( Q_3_ZHAR_THE_MAD ) && ZharTheMadRoomIndex == -1 ){
		Quests[ Q_3_ZHAR_THE_MAD ].status = QS_0_NO;
	}
	if( Dungeon == Quests[ Q_15_ARCHBISHOP_LAZARUS ].dungeonStart && MaxCountOfPlayersInGame != 1 ){
		AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus ].BaseMonsterIndex, MPF_4_UNIQ );
		AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_868_RED_VEX : UM_5_Red_Vex ].BaseMonsterIndex, MPF_4_UNIQ );
		CreateBoss(GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus, 0, 0 );
		CreateBoss(GameMode == GM_CLASSIC ? UM_C_868_RED_VEX : UM_5_Red_Vex, 0, 0 );
		CreateBoss(GameMode == GM_CLASSIC ? UM_C_869_BLACKJADE : UM_6_Dark_Jade, 0, 0 );
		lazarus_entry_dun = LoadFile( "Levels\\L4Data\\Vile1.DUN" );
		AddQuestMonsters( (ushort*) lazarus_entry_dun, 2 * CurPattern.row, 2 * CurPattern.col );
		FreeMem( lazarus_entry_dun );
	}
	if( Dungeon == DUN_24_CRYPT_4 ){
		UberDiabloMonsterIndex = -1;
		v8 = 0;
		if( MonsterSpritesCount > 0 ){
			v9 = MonsterSprites;
			do{
				if( v9->baseMonsterIndex == UniqueMonsters[ UM_12_Uber_Diablo ].BaseMonsterIndex ){
					break;
				}
				++v8;
				++v9;
			}while( v8 < MonsterSpritesCount );
		}
		if( v8 < MonsterSpritesCount ){
			v10 = 0;
			if( MonstersCount > 0 ){
				v11 = Monsters;
				while( !v11->newBossId && v11->SpriteIndex != v8 ){
					++v10;
					++v11;
					if( v10 >= MonstersCount ){
						goto LABEL_43;
					}
				}
				UberDiabloMonsterIndex = v10;
			}
		}
	LABEL_43:
		if( UberDiabloMonsterIndex == -1 ){
			boss_id = UM_12_Uber_Diablo;
		LABEL_47:
			CreateBoss( boss_id, 0, 0 );
			return;
		}
	}
}

//----- (0043C33D) --------------------------------------------------------
void __fastcall CreateMonstersPack(int spriteIndex, int count, int leaderFlag, int monsterBossIndex)
{
	if (IHateMonsters) {
		return;
	}
	Monster& boss = Monsters[monsterBossIndex];
	int row = 0, col = 0, packRow = 0, packCol = 0;
	int placed = 0;
	for( int try1 = 0; try1 < 50; ++try1 ){ // extra general try coung after boss pack range lowering (was 10)
		while( placed ){
			--MonstersCount;
			--placed;
			MonsterMap[Monsters[MonstersCount].Row][Monsters[MonstersCount].Col] = 0;
		}
		if( leaderFlag & UN_1_PACK ){
			int dir = RNG(8);
			row = packRow = boss.Row + RowDelta[dir];
			col = packCol = boss.Col + ColDelta[dir];
		}else{
			do{
				row = packRow = RNG(80) + 16;
				col = packCol = RNG(80) + 16;
			}while( !IsCellFreeForMonster(row, col) );
		}
		if( count + MonstersCount > MonsterArraySize ){
			count = MonsterArraySize - MonstersCount;
		}
		int m = 0;
		int split_pack_distance = 6;
		if (is(Dungeon, DUN_91_STYGIAN_PASSAGE, DUN_96_BONE_MIRE, DUN_97_GLOOMY_DEPTHS, DUN_100_HORADRIM_TREASURY, DUN_103_ALTAR_OF_IMMOLATION)) { 
			split_pack_distance = 12;
		}
		else if (Dungeon == DUN_108_ARCANE_PRISON) {
			split_pack_distance = 18;
		}
		for( int try2 = 0; m < count && try2 < 500; ){
			if( ! IsCellFreeForMonster(row, col) || TransMap[row][col] != TransMap[packRow][packCol]
			 || leaderFlag & UN_2_STICK && (abs(row - packRow) >= 5 || abs(col - packCol) >= 5)
			 || leaderFlag & UN_1_PACK  && (abs(row - packRow) >= split_pack_distance || abs(col - packCol) >= split_pack_distance) // fixing too far boss pack mobs positions
				){
				++try2;
			}else{
				CreateBossBaseMonster(MonstersCount, spriteIndex, row, col);
				if (leaderFlag & UN_1_PACK) {
					Monster& monster = Monsters[MonstersCount];
					monster.BaseLife *= 2;
					monster.CurrentLife = monster.BaseLife;
					monster.intel = boss.intel;
					monster.hisBossIndex = monsterBossIndex;
					monster.isBossPack = 1;
					if( leaderFlag & UN_2_STICK ){ // TODO: change to UN_5_ATTACK
						monster.ai = boss.ai;
					}
					if( monster.ai != AI_12_Gargoyle ){
						monster.curAnimation = &monster.SpritePtr->animation[ANIM_0_STAY].oriented[monster.Orientation];;
						monster.CurFrame = RNG(monster.animFrameCount - 1) + 1;
						monster.flag &= ~MF_3_STONE;
						monster.CurAction = A_0_STAY;
					}
				}
				++MonstersCount;
				++placed;
				++m;
			}
			// проверка на видимость, чтобы часть босспака не оказывалась в другой (CheckCastInSight variant)
			// trying to avoid place pack monsters over the wall
			int dir = RNG(8);
			int r, c;
			for( int i = 0; i < 9; ++i, ++dir){
				r = row + RowDelta[dir & 7];
				c = col + ColDelta[dir & 7];
				if( ! IsCellBlocking(r, c) ) break;
			}
			row = r;
			col = c;
		}
		if( placed >= count ){
			//if( boss.newBossId - 1 == UM_154_Diablo ) __debugbreak();
			break;
		}
	}
	if( leaderFlag & UN_2_STICK ){
		Monsters[monsterBossIndex].BossPackCount = placed;
	}
}


//----- (00717750) --------------------------------------------------------
void __fastcall AddBossTypeToQuestLoc( int uniqueMonsterIndex )
{
	AddMonsterTypeToDungeon( UniqueMonsters[ uniqueMonsterIndex ].BaseMonsterIndex, MPF_4_UNIQ );
	CreateBoss( uniqueMonsterIndex, 0, 0 );
}

//----- (th3) -------------------------------------------------------------
void AddBossPack(BossPack bp)
{
	bool mixed = UniqueMonsters[bp.uniq].PackSpecials > 0;
	int uniqBaseMonster = mixed ? UniqueMonsters[bp.uniq].PackSpecials : UniqueMonsters[bp.uniq].BaseMonsterIndex;
	if( AddMonsterTypeToDungeon(uniqBaseMonster, MPF_4_UNIQ) > -1 ){
		auto bossPackSpriteIndex = AddMonsterTypeToDungeon(bp.base, MPF_2_DONT);
		if( bossPackSpriteIndex > -1 ){
			CreateBoss(bp.uniq, bossPackSpriteIndex, bp.count, bp.packFlag);
		}else{
			CreateBoss(bp.uniq, 0, 0);
		}
	}
}

//----- (0043C5CC) --------------------------------------------------------
void __fastcall AddQuestMonsters(ushort* dunData, int startRow, int startCol) // SetMapMonsters
{
	if( IHateMonsters ) return;
	if( Dungeon->isQuest ) FillSolidBlockTables();

	// summon monster adding
	if( !SummonMonstersAdded ){
        AddMonsterTypeToDungeon(BM_109_GOLEM, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_223_DEMONOLOG_SUMMON_1, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_224_DEMONOLOG_SUMMON_2, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_225_DEMONOLOG_SUMMON_3, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_226_NECROMANT_SUMMON_1, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_227_NECROMANT_SUMMON_2, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_228_NECROMANT_SUMMON_3, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_229_BEASTMASTER_SUMMON_1, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_230_BEASTMASTER_SUMMON_2, MPF_2_DONT);
        AddMonsterTypeToDungeon(BM_231_BEASTMASTER_SUMMON_3, MPF_2_DONT);
     
        for( int playerIndex = 0; playerIndex < PlayersMax_4; ++playerIndex ){
            for( int summonIndex = 0; summonIndex < SummonMonstersPerPlayer_Count; ++summonIndex ){
                AddMonster( 1, 0, 0, 0, 0 );
            }
        }
        SummonMonstersAdded = true;
    }

	if (Dungeon == DUN_5_CATACOMB_1 && !Dungeon->isQuest) {
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_902_RAGEHORN : UM_677_Guardian_of_Valor].BaseMonsterIndex, MPF_4_UNIQ);
		CreateBoss(GameMode == GM_CLASSIC ? UM_C_902_RAGEHORN : UM_677_Guardian_of_Valor, 0, 0);
	}
	if (Dungeon == DUN_7_CATACOMB_3 && !Dungeon->isQuest) {
		AddMonsterTypeToDungeon(UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_871_HAZEWEAVER : UM_676_Hazeweaver].BaseMonsterIndex, MPF_4_UNIQ);
		CreateBoss(GameMode == GM_CLASSIC ? UM_C_871_HAZEWEAVER : UM_676_Hazeweaver, 0, 0);
	}
	if (Dungeon == DUN_10_CAVE_2 && !Dungeon->isQuest) {
		if (GameMode != GM_CLASSIC) {
			AddMonsterTypeToDungeon(UniqueMonsters[UM_678_Warden_of_Anger].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_678_Warden_of_Anger, 0, 0);
		}
	}

	// добавили боссов в анхоли алтарь
	if( Dungeon->isQuest ){
		if( Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
			AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus].BaseMonsterIndex, MPF_4_UNIQ);
			AddMonsterTypeToDungeon( UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_868_RED_VEX : UM_5_Red_Vex].BaseMonsterIndex, MPF_4_UNIQ);
			//AddMonsterTypeToDungeon( UniqueMonsters[UM_6_Dark_Jade].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus, 0, 0);
			CreateBoss(GameMode == GM_CLASSIC ? UM_C_868_RED_VEX : UM_5_Red_Vex, 0, 0);
			CreateBoss(GameMode == GM_CLASSIC ? UM_C_869_BLACKJADE : UM_6_Dark_Jade, 0, 0);
		}
		else if( Dungeon == DUN_31_ANDARIELS_THRONE ){
			AddMonsterTypeToDungeon( UniqueMonsters[ UM_579_Andariel ].BaseMonsterIndex, MPF_4_UNIQ );
			CreateBoss( UM_579_Andariel, 0, 0 );
		}
		else if (Dungeon == DUN_54_ABANDONED_FANE) {
			auto dun54Bosses = { UM_717_Priestess_Varna, UM_713_Sir_Valon, UM_714_Sir_Darion, UM_715_Bishop_Zenda, UM_716_Bishop_Zamal };
			for (int i : dun54Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_60_HAUNTED_LABYRINTH) {
			AddMonsterTypeToDungeon(UniqueMonsters[UM_740_Don_Carnage].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_740_Don_Carnage, 0, 0);
			AddMonsterTypeToDungeon(UniqueMonsters[UM_741_Gloom_Prowler].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_741_Gloom_Prowler, 0, 0);
		}
		else if (Dungeon == DUN_61_CURSED_SANCTUM) {
			AddMonsterTypeToDungeon(UniqueMonsters[UM_742_Ironhoof].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_742_Ironhoof, 0, 0);
		}
		else if (Dungeon == DUN_62_DIM_UNDERCROFT) {
			AddMonsterTypeToDungeon(UniqueMonsters[UM_743_Undertaker].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_743_Undertaker, 0, 0); 
			AddMonsterTypeToDungeon(UniqueMonsters[UM_744_Dustwing].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_744_Dustwing, 0, 0);
		}
		else if (Dungeon == DUN_63_FORGOTTEN_OSSUARY) {
			auto dun63Bosses = { UM_416_Pusfroth, UM_347_Boneskull_the_Deadly, UM_170_Nightrend_the_Dark, UM_152_Ashraiser };
			for (int i : dun63Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_64_MAUSOLEUM) {
			auto dun64Bosses = { UM_239_Plaguefist_Blackheart, UM_220_Blackwillow, UM_34_Pukerat_the_Unclean };
			for (int i : dun64Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_65_RUINED_TEMPLE) {
			auto dun65Bosses = { UM_54_Kobal_the_Duke, UM_191_Shadowcrow };
			for (int i : dun65Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_66_SEWERS) {
			auto dun66Bosses = { UM_380_Duskstone, UM_237_Meatchop_the_Flayer, UM_368_Goredrool };
			for (int i : dun66Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_67_SEPULCHER) {
			auto dun67Bosses = { UM_419_Deathpuke, UM_53_Viletouch };
			for (int i : dun67Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_68_OUBLIETTE) {
			auto dun68Bosses = { UM_745_Xamutgor };
			for (int i : dun68Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_69_GIBBET) {
			auto dun69Bosses = { UM_746_Snarlak, UM_747_Tronzug };
			for (int i : dun69Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_70_CITADEL) {
			auto dun70Bosses = { UM_748_Fangscar, UM_440_Painwraps, UM_377_Blazeshock };
			for (int i : dun70Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_71_GRIM_VAULTS) {
			auto dun71Bosses = { UM_143_Bloodscythe_Gloomslayer, UM_652_Griefbang, UM_322_Mindgloom };
			for (int i : dun71Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_72_NECROPOLIS) {
			auto dun72Bosses = { UM_749_Sethmer, UM_750_Fleshbinder };
			for (int i : dun72Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_73_LAVA_PITS) {
			auto lpMonsters = { 
				BM_93_FLAYED_ONE, 
				BM_29_HIDDEN, 
				BM_97_DARK_CHARGER, 
				BM_150_FELLTWIN,
				BM_202_SUCCUBUS,
				BM_113_ASH_GOLEM};

			for (int i : lpMonsters) {
				AddMonsterTypeToDungeon(i, MPF_2_DONT);
			}
			auto dun73Bosses = { UM_751_Orzog };
			for (int i : dun73Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_74_CRATER) {
			auto dun74Bosses = { UM_379_Arcstorm, UM_132_Phugantar_the_Glowing, UM_133_Fangsoul_the_Hisser };
			for (int i : dun74Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_75_RIVER_OF_FIRE) {
			auto dun75Bosses = { UM_424_Stenchooze, UM_542_Steeltouch, UM_52_Firefist_the_Grim };
			for (int i : dun75Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_76_DEN_OF_EVIL) {
			auto dun76Bosses = { UM_562_Deathfire, UM_436_Gorethrasher, UM_391_Fleshreaper, UM_63_Doomgaze };
			for (int i : dun76Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_77_VILE_TUNNELS) {
			auto dun77Bosses = { UM_427_Deathpus_the_Uninterred, UM_628_Moltenstorm_the_Grim };
			for (int i : dun77Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_78_THE_BRIDGE) {
			auto dun78Bosses = { UM_752_Deathcaller };
			for (int i : dun78Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_79_COLONNADE) {
			auto dun79Bosses = { UM_629_Nightburn_the_Mad, UM_753_Sister_Priscilla, UM_754_Madame_Pompadour, UM_384_Lionsteel_the_Bent };
			for (int i : dun79Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_80_HELL_GATE) {
			auto dun80Bosses = { UM_169_Urzul_the_Trampler };
			for (int i : dun80Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_81_UNDERWORLD) {
			auto dun81Bosses = { UM_544_Darkhoof, UM_633_Fleshvenom_the_Toxic, UM_19_Jaldabaoth };
			for (int i : dun81Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_82_HALLS_OF_SUFFERING) {
			auto dun82Bosses = { UM_755_Redbark_Elderroot, UM_49_Oozeshredder, UM_55_Gutooze_the_Foul };
			for (int i : dun82Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_83_BLOOD_KEEP) {
			auto dun83Bosses = { UM_613_Rotwound_the_Dark, UM_151_Bloodfury, UM_624_Nightclaw_the_Sharp };
			for (int i : dun83Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_84_ARMORIES_OF_HELL) {
			auto dun84Bosses = { UM_756_Paladin_Solo };
			for (int i : dun84Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_85_DEMON_STRONGHOLD) {
			auto dun85Bosses = { UM_757_Skullreaper };
			for (int i : dun85Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_86_VALLEY_OF_WRATH) {
			auto dun86Bosses = { UM_136_Sarogyl_the_Decapitator };
			for (int i : dun86Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_87_PANDEMONIUM) {
			auto dun87Bosses = { UM_495_Asmoday };
			for (int i : dun87Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_88_REALM_OF_TERROR) {
			auto dun88Bosses = { UM_758_Lord_of_Terror };
			for (int i : dun88Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_89_BEDLAM) {
			auto dun89Bosses = { UM_575_Hoofrazor, UM_112_Wreckfist_Gutmauler, UM_574_Mindzap, UM_517_Fangrage, UM_477_Grimblight, UM_614_Deadpus_the_Unholy };
			for (int i : dun89Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_90_FIELDS_OF_MADNESS) {
			static BossPack dun90Bosses[] = { 
				{UM_616_Deathbreak_the_Hammer, BM_168_HELL_BAT, 8, 1}, 
				{UM_599_Barbossa, BM_248_SALAMANDER, 8, 1}, 
				{UM_720_The_Marauder, BM_232_ABYSSAL_SUCCUBUS, 8, 1} 
			};
			for( auto& i : dun90Bosses ){
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_91_STYGIAN_PASSAGE) {
			auto packSize1 = Difficulty / 2 + 13;
			BossPack dun91Bosses[] = { 
				{ UM_617_Doomfang_the_Cold, BM_28_BLOOD_WARPER, packSize1 + 20, 1 },
				{ UM_609_Gutfeast_the_Unclean, BM_135_GROTESQUE, packSize1, 1 },
				{ UM_311_Hulk_Lord, BM_46_PIT_SPIDER, packSize1, 1 },
				{ UM_627_Hazespawn_the_Shade, BM_222_NIGHT_DEMON, packSize1 },
				{ UM_656_Devilcrow, BM_254_TWILIGHT_CLAN, packSize1, 1 }
			};
			for (auto& i : dun91Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_92_FLESH_PIT) {
			auto packSize1 = Difficulty / 2 + 13;
			BossPack dun92Bosses[] = {				
				{ UM_693_Firedrinker, BM_83_LASH_WORM, packSize1 + 15, 1 },
				{ UM_610_Pitmaim_the_Tainted, BM_237_CONTAMINATOR, packSize1, 1 },
				{ UM_620_Fleshbender, BM_278_LEAPER, packSize1, 1 },
				{ UM_250_Shulgoth, BM_279_FLOATER, packSize1, 1 }
			};
			for (auto& i : dun92Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_93_BLEEDING_VEINS_PATH) {
			BossPack dun93Bosses[] = {
				{ UM_759_Bonecleaver, BM_120_NECROMORPH, 14, 1 },
				{ UM_472_Shadethunder, BM_165_APOCALYPSE_CLAN, 9, 1 },
				{ UM_603_Ragespike_the_Violent, BM_233_BLOOD_LORD, 19, 1 }
			};
			for (auto& i : dun93Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_94_ROT_CHAMBERS) {
			BossPack dun94Bosses[] = {
				{ UM_478_Fleshdrool_the_Terror, BM_164_LIGHTNING_DEMON, 29, 1 },
				{ UM_474_Sharprage, BM_161_CHASM_DRAKE, 29, 1 },
				{ UM_278_Tartog, BM_80_DEVIL_KIN, 29, 1 },
				{ UM_492_Fleshfeast, BM_171_MUD_LORD, 29, 1 },
				{ UM_698_Ghoulbone_Headhunter, BM_261_SKELETAL_KNIGHT, 29, 1 },
				{ UM_502_Scumfeet_the_Foul, BM_179_FILTH_DEMON, 29, 1 },
				{ UM_611_Plaguepox_the_Destroyer, BM_236_CORPSE_EATER, 12, 1 }
			};
			for (auto& i : dun94Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_95_MARROW_GARDEN) {
			BossPack dun95Bosses[] = {
				{ UM_760_DrimMog, BM_273_FLAME_FIEND, 12, 1 },
				{ UM_247_Banevenom_the_Toxic, BM_46_PIT_SPIDER, 12, 1 },
				{ UM_761_Jalvathoz, BM_117_STINGER, 16, 1 },
				{ UM_762_Krong, BM_135_GROTESQUE, 9, 1 },
				{ UM_185_Kasadeus_the_Fangskin, BM_68_GORE_VIPER, 21, 1 },
				{ UM_72_Nephalon_the_Cursed, BM_73_DEATH_ANGEL, 24, 1 },
				{ UM_601_Blackmane_the_Hunter, BM_233_BLOOD_LORD, 18, 1 }
			};
			for (auto& i : dun95Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_96_BONE_MIRE) {
			BossPack dun96Bosses[] = {
				{ UM_615_Fleshspawn_the_Dead, BM_237_CONTAMINATOR, 48, 1 },
				{ UM_484_Freezelock, BM_167_SKULLWING, 11, 1 },
				{ UM_648_Forktongue_the_Mangler, BM_248_SALAMANDER, 24, 1 }
			};
			for (auto& i : dun96Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_97_GLOOMY_DEPTHS) {
			BossPack dun97Bosses[] = {
				{ UM_519_Black_Death, BM_185_BLOOD_HORN, 11, 1 },
				{ UM_621_Moonvex, BM_239_AFFLICTED, 11, 1 },
				{ UM_763_Beelzebub, BM_232_ABYSSAL_SUCCUBUS, 48, 1 }
			};
			for (auto& i : dun97Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_98_EBON_CHASM) {
			BossPack dun98Bosses[] = {
				{ UM_764_Lord_Nysallor, BM_185_BLOOD_HORN, 0, 1 },
				{ UM_765_Duke_of_Abyss, BM_239_AFFLICTED, 0, 1 }
			};
			for (auto& i : dun98Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_99_ANCIENT_CRYPTS) {
			BossPack dun99Bosses[] = {
				{ UM_1_Skeleton_King, BM_64_DEATH_GUARDIAN, 9, 3 },
				{ UM_554_Gloomshank, BM_204_SHADOW_CLAN, 11, 1 },
				{ UM_577_Gorefear, BM_208_WARPED_ONE, 8, 1 },
				{ UM_76_Sether, BM_39_WARPING_DEMON, 12, 1 },
				{ UM_81_Uriel, BM_84_DEATH_WING, 9, 1 },
				{ UM_504_Grimblood_the_Frenzied, BM_179_FILTH_DEMON, 10, 1 },
				{ UM_670_Drilmog, BM_256_RAGE_CLAN, 17, 1 }
			};
			for (auto& i : dun99Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_100_HORADRIM_TREASURY) {
			BossPack dun100Bosses[] = {
				{ UM_766_Zhar_the_Mad_AR, BM_125_GOLD_GOLEM, 8, 1 },
				{ UM_89_Alystar, BM_108_MAGISTRATE, 11, 3 },
				{ UM_488_Firespawn, BM_168_HELL_BAT, 24, 1 },
				{ UM_124_Torvatus, BM_98_IMMORTAL, 17, 1 },
				{ UM_210_Tombstone_the_Evil, BM_125_GOLD_GOLEM, 14, 3 },
				{ UM_102_Gorestone, BM_124_GLOOM_CLAN, 11, 1 },
				{ UM_86_Varaya, BM_104_DARK_WITCH, 24, 3 },
				{ UM_139_Zabulon, BM_104_DARK_WITCH, 11, 1 }
			};
			for (auto& i : dun100Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_101_VOID_CHAMBERS) {
			BossPack dun101Bosses[] = {
				{ UM_767_BerUlmeth, BM_234_STEEL_LORD, 14, 1 },
				{ UM_4_ArchBishop_Lazarus, BM_107_DARK_MAGE, 17, 1 },
				{ UM_496_Phaseclaw_the_Vile, BM_177_PHASE_DEMON, 11, 1 },
				{ UM_553_Executioner_of_Death, BM_167_SKULLWING, 19, 1 }
			};
			for (auto& i : dun101Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_102_OBLIVION_TRAIL) {
			BossPack dun102Bosses[] = {
				{ UM_671_Turze, BM_256_RAGE_CLAN, 24, 1 },
				{ UM_701_Boneskull_the_Undying, BM_261_SKELETAL_KNIGHT, 14, 1 },
				{ UM_768_Blood_Medley_the_Thing, BM_49_BILE_SPITTER, 15, 1 },
				{ UM_479_Ironthunder, BM_164_LIGHTNING_DEMON, 12, 1 },
				{ UM_506_The_Cremator, BM_180_CRYPT_STONE, 12, 1 },
				{ UM_555_Ghostmace, BM_204_SHADOW_CLAN, 17, 1 },
				{ UM_512_Blackslice, BM_184_REALITY_WEAVER, 13, 1 },
				{ UM_97_Baal, BM_85_BALOR, 22, 1 }
			};
			for (auto& i : dun102Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) {
			BossPack dun103Bosses[] = {
				{ UM_576_Evilshock, BM_254_TWILIGHT_CLAN, 39, 1 },
				{ UM_485_Goredrown, BM_167_SKULLWING, 39, 1 },
				{ UM_450_Bloodrayne, BM_133_SOUL_BURNER, 39, 1 },
				{ UM_508_Blazestorm, BM_181_ADVOCATE, 39, 1 }
			};
			for (auto& i : dun103Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_104_NETHERWORLD_GATES) {
			BossPack dun104Bosses[] = {
				{ UM_769_Boneslash_the_Revenant, BM_203_DEATH_KNIGHT, 9, 1 },
				{ UM_770_Graveblade, BM_203_DEATH_KNIGHT, 9, 1 },
				{ UM_77_Stonelava_Bashfist, BM_62_MAULER, 11, 1 },
				{ UM_79_Gloomclaw, BM_71_THUNDER_LORD, 13, 1 },
				{ UM_83_Doomfang_Bloodgutter, BM_66_HORNED_DEATH, 14, 1 },
				{ UM_493_Fleshblight, BM_172_SPITTING_TERROR, 14, 3 }
			};
			for (auto& i : dun104Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_105_DARK_DESCENT) {
			BossPack dun105Bosses[] = {
				{ UM_138_Lionskull_the_Black, BM_122_BLOOD_SPECTRE, 24, 3 },
				{ UM_321_Magnate, BM_125_GOLD_GOLEM, 24, 3 },
				{ UM_153_UrAnnon, BM_108_MAGISTRATE, 16, 3 },
				{ UM_771_Nightbite, BM_198_HORN_CLAN, 24, 1 },
				{ UM_196_Vatar_the_Void_Guardian, BM_85_BALOR, 35, 1 }
			};
			for (auto& i : dun105Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_106_BASTION_OF_NIGHT) {
			BossPack dun106Bosses[] = {
				{ UM_200_Xelith, BM_133_SOUL_BURNER, 24, 1 },
				{ UM_405_Scareblade, BM_122_BLOOD_SPECTRE, 24, 1 },
				{ UM_513_Duskshroud, BM_184_REALITY_WEAVER, 24, 1 },
				{ UM_316_Burzul, BM_66_HORNED_DEATH, 24, 1 },
				{ UM_199_Grimstomp, BM_66_HORNED_DEATH, 24, 1 },
				{ UM_94_Setebos, BM_125_GOLD_GOLEM, 24, 1 },
				{ UM_623_Griefburn_the_Destroyer, BM_239_AFFLICTED, 24, 3 }
			};
			for (auto& i : dun106Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_107_DREAD_SANCTUARY) {
			BossPack dun107Bosses[] = {
				{ UM_714_Sir_Darion, BM_162_CORRUPT_PALADIN, 4, 1 },
				{ UM_715_Bishop_Zenda, BM_181_ADVOCATE, 11, 3 },
				{ UM_716_Bishop_Zamal, BM_181_ADVOCATE, 11, 3 },
				{ UM_201_Blackstorm, BM_71_THUNDER_LORD, 17, 1 },
				{ UM_99_Belial_the_Disemboweler, BM_78_EVISCERATOR, 17, 1 },
				{ UM_109_Stoneburn_the_Crusher, BM_62_MAULER, 19, 1 },
				{ UM_197_Blacktongue_Deathbite, BM_85_BALOR, 19, 1 },
				{ UM_772_Vexaroth, BM_198_HORN_CLAN, 24, 1 }
			};
			for (auto& i : dun107Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_108_ARCANE_PRISON) {
			BossPack dun108Bosses[] = {
				{ UM_773_The_Dark_Lord, BM_240_DEMON_PRIEST, 99 - Difficulty * 6, 1 }
			};
			for (auto& i : dun108Bosses) {
				AddBossPack(i);
			}
		}

		//----------------------------------------------end of AR duns----------------

		else if (Dungeon == DUN_27_THE_INFESTED_CELLAR) {
			BossPack dun_27_Boss_Pack[] = {
				{ UM_774_Swarmbringer, BM_91_GOLD_SERPENT, 7, 1 }
			};
			for (auto& i : dun_27_Boss_Pack) {
				AddBossPack(i);
			}
		}

		else if (Dungeon == DUN_109_HALL_OF_HEROES && GameMode != GM_CLASSIC) {
			BossPack dun_109_Bosses[] = {
				{ UM_788_Grimgar_the_Fallen, BM_27_HORROR, 7, 1 },
				{ UM_789_Bonecrunch_the_Risen, BM_27_HORROR, 7, 1 }
			};
			for (auto& i : dun_109_Bosses) {
				AddBossPack(i);
			}
		}

		else if (Dungeon == DUN_110_BLACK_CHAPEL) {
			BossPack dun_110_Bosses[] = {
				{ UM_790_Ghost_of_Brother_Martin, BM_121_SPECTRE, 27, 1 },
			};
			for (auto& i : dun_110_Bosses) {
				AddBossPack(i);
			}
		}

		else if (Dungeon == DUN_111_CHURCH_SACRISTY) {
			BossPack dun_111_Boss[] = {
				{ UM_791_Festergnaw, BM_138_SHADOW_BEAST, 11, 1 },
			};
			for (auto& i : dun_111_Boss) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_112_STILL_SPRINGS) {
			BossPack dun112Bosses[] = {
				{ UM_805_Festerus, BM_199_HELL_CLAN, 15, 1 },
				{ UM_806_Obfuscus, BM_288_EARTH_GOLEM, 13, 3 },
				{ UM_807_Stormire, BM_77_STORM_DEMON, 6, 3 },
				{ UM_808_Winggore, BM_82_GARGOYLE, 11, 3 }
			};
			for (auto& i : dun112Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_113_THE_MAZE) {
			BossPack dun113Bosses[] = {
				{ UM_809_Borosteron, BM_233_BLOOD_LORD, 14, 3 },
			};
			for (auto& i : dun113Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_114_LOWER_SCRIPTORIUM) {
			BossPack dun114Bosses[] = {
				{ UM_821_Snotspill_MP, BM_5_FALLEN_ONE, 19, 3 },
			};
			for (auto& i : dun114Bosses) {
				AddBossPack(i);
			}
		}

		else if( Dungeon == DUN_45_THE_DEN ){
		    auto denMonsters = {
		        BM_3_FAMILIAR,
		        BM_56_PIT_DEMON,
		        BM_31_UNSEEN,
		        BM_19_RAVAGER,
		        BM_131_FIRE_BAT,
		        BM_36_DARKNESS_CLAN,
		        BM_32_INVISIBLE_ONE };
		
			for( int i : denMonsters ){
			    AddMonsterTypeToDungeon( i, MPF_2_DONT );
            }
			AddBossTypeToQuestLoc( UM_595_The_Cerberus );
		}else{
			int boss = -1;
			switch( (DUNGEON)Dungeon ){
			case DUN_26_BONE_CHAMBER:
			{
				if (GameMode != GM_CLASSIC) {
					boss = UM_674_Guardian_of_Bones;
				}
			} break;
			case DUN_28_POISONED_WATER_SUPPLY: 
			{
				if (Difficulty >= DL_1_PURGATORY && GameMode != GM_CLASSIC) {
					boss = UM_675_PWS_Boss;
				}
			} break;
			case DUN_33_SUMMONING_ROOMS: boss = UM_580_NaKrul; break;
			case DUN_34_THE_PIT: boss = UM_581_Grimspike; break;
			case DUN_36_VALLEY_OF_DESPAIR: boss = UM_582_Izual; break;
			case DUN_39_FROZEN_LOCH: boss = UM_583_Lich_King; break;
			case DUN_40_ISLAND: boss = UM_584_Mordessa; break;
			case DUN_41_FLESHDOOM: boss = UM_585_Fleshdoom; break;
			case DUN_42_WIELDER: boss = UM_586_Wielder_of_Shadowfang; break;
			case DUN_43_HEPHASTO: boss = UM_587_Hephasto_the_Armorer; break;
			case DUN_47_DARK_NEST: boss = UM_702_Grimjaw; break;
			case DUN_48_BLOOD_HIVE: boss = UM_703_Hive_Mother; break;
			case DUN_49_SICK_VILLAGERS_BASEMENT: boss = UM_704_Borje_the_Ogre; break;
			case DUN_50_TEMPLE_OF_SUNSET: boss = UM_705_Bloodhoof_Twinaxe; break;
			case DUN_51_FETID_CAVERN: boss = UM_706_Mudsnout; break;
			case DUN_52_MOSSY_GROTTO: boss = UM_707_Lichen_Axe_the_Fungal; break;
			case DUN_53_VAULTKEEP: boss = UM_708_Grimwing; break;
			case DUN_35_PASSAGE_OF_FIRE: boss = UM_718_Guardian_of_Flames; break;
			case DUN_55_DEEP_HOLLOW: boss = UM_723_The_Infester; break;
			case DUN_56_WEB_TUNNELS: boss = UM_724_Silkstrider; break;
			case DUN_57_ARACHNID_LAIR: boss = UM_725_Web_Queen; break;
			case DUN_59_HALL_OF_FIRE: boss = UM_726_Apparition_of_Arkaine; break;
			}
			if( boss != -1 ){
				AddBossTypeToQuestLoc( boss );
			}
		}
	}

	ushort rowSize  = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunMap = &dunData[2];
	ushort* monsterMap = &dunMap[5 * rowSize * colSize ];
	if( Dungeon->genType != DT_0_TOWN ){
		startCol += 16;
		startRow += 16;
	}

	//Any MP quest map with a special boss has a high chance > 50% of monster HP desync, e.g. COB, Purg+ PWS, Pit of Flame, etc.
	//Upon re-entry of said quest map via TP (NOT stairs), monsters HP get shuffled around, some end up with CurrentLife > BaseLife, while others will have CurrentLife < BaseLife without taking any hit.  The symptoms are similar to fixed Catacomb level morph.
	//Those quest maps do not have special bosses are not affected, e.g. Hall of Anguish, Infested Cellar, Horror PWS.
	//Debug prints reveals that as the player leaves this quest map for the first time, then re-enters via TP, delta_leave_sync() and delta_load_level() return the same monster.CurrentLife, while the monsters' HP is already shuffled.  The second+ time when delta_leave_sync() is called, it displays the shuffled monster HP value.  So something is broken right after the first delta_load_level() call.
	//I tracked down this bug by setting data breakpoint for Monsters[128].CurrentLife and Monsters[128].BaseLife while loading COB.  The #128th monster is 1 of the 3 skeletons near the stairs.
	//The data breakpoint reveals that AddQuestMonsters() is at fault.
	//The lines below calls CreateBossBaseMonster(), which calls CreateMonster(), which sets monster.BaseLife via RNG.
	//But RNG here cannot be reproduced as player re-enters via TP, hence the 1 line solution here.
	//Furthermore, RNG is already broken and cannot be reproduced upon TP re-entry before the current function AddQuestMonsters(), and somewhere inside LoadSetMap() call inside LoadGameLevel() in Main.cpp.  The reason is not clear.  But this does not affect monster HP desync.
	//It will be useful to figure out why the bug was triggered by TP re-entry into MP quest area, and NOT stair re-entry.
//	SetRndSeed(DungeonRandSeed[(DUNGEON)Dungeon]); // Looks like we can keep this change commented out, because the final commit in branch MPQuestMapMonsterHPShuffleFix should be sufficient, which recovers corrupted RNG seed and makes it reproducible.  No need to lock the RNG seed here then.

	for( int col = 0; col < colSize*2; col++ ){
		for( int row = 0; row < rowSize*2; row++ ){
			ushort monsterNum = monsterMap[row + rowSize * 2 * col];
			if( monsterNum && MonstersCount < Monsters_Max_Count ){ // was memory corruption
				int spriteIndex = AddMonsterTypeToDungeon( BaseMonsterIndexes[monsterNum - 1], MPF_2_DONT );
				if( spriteIndex > -1 ) CreateBossBaseMonster(MonstersCount++, spriteIndex, startRow + row, startCol + col);
			}
		}
	}
}

//----- (0043C70C) --------------------------------------------------------
void __fastcall DelMonster(int monsterIndexIndex)
{
	--MonstersCount;
	int leftIndex = MonsterIndexes[MonstersCount];
	MonsterIndexes[MonstersCount] = MonsterIndexes[monsterIndexIndex];
	MonsterIndexes[monsterIndexIndex] = leftIndex;
	if( Dungeon->isQuest && MonstersCount <= SummonMonsters_Count ){
		Player& player = Players[CurrentPlayerIndex];
		PlayLocalSound( S_14_QUESTDON, player.Row, player.Col );
	}
	else {
		if( GameModeType() == GAME_MODE_TYPE::IRON && GetNumberOfMonstersInDungeon() == 0 && GetNumberOfObjectsInDungeon() <= MaxIronmanObjects && Dungeon != DUN_0_TOWN ){
			IronmanLevelCleared();
		}
	}
}

//----- (0043C735) --------------------------------------------------------
int __fastcall AddMonster(int row, int col, int direction, int spiteIndex, int newMonsterOnMap)
{
	if( IHateMonsters && spiteIndex ){
		return -1;
	}
	
	int result = -1;
	if( MonstersCount < Monsters_Max_Count ){
		int monsterIndex = MonsterIndexes[MonstersCount++];
		if( newMonsterOnMap )
			MonsterMap[ row ][ col ] = monsterIndex + 1;
		CreateMonster(monsterIndex, direction, spiteIndex, row, col);
		result = monsterIndex;
	}
	return result;
}

//----- (0043C785) --------------------------------------------------------
void __fastcall CloneMonsterByDoppelganger(int monsterIndex)
{
	int v1;            // esi@1
	int v2;            // ebx@3
	int v3;            // edi@3
	int v4;            // eax@4
	char v5;           // al@6
	int v6;            // eax@8
	int v7;            // edx@13
	MonsterSprite* v8; // ecx@13
	int v9;            // eax@2
	int v10;           // [sp+18h] [bp-4h]@2
	int v11;           // [sp+14h] [bp-8h]@2
	int v12;           // [sp+10h] [bp-Ch]@2
	int v13;           // [sp+Ch] [bp-10h]@2
	v1 = sizeof(Monster) * monsterIndex;
	if( Monsters[monsterIndex].SpritePtr ){
		v9 = *(int*)((char*)&Monsters[0].Row + v1);
		v10 = 0;
		v11 = v9;
		v12 = *(int*)((char*)&Monsters[0].Col + v1);
		v13 = *(int*)((char*)&Monsters[0].Orientation + v1);
		do{
			v2 = v11 + RowDelta[v10];
			v3 = v12 + ColDelta[v10];
			if( !IsCellBlocking(v11 + RowDelta[v10], v12 + ColDelta[v10]) ){
				v4 = v3 + 112 * v2;
				if( !PlayerMap[ 0 ][ v4 ] ){
					if( !MonsterMap[ 0 ][ v4 ] ){
						v5 = ObjectsMap[ 0 ][ v4 ];
						if( !v5 )
							break;
						if( v5 <= 0 )
							v6 = -1 - v5;
						else
							v6 = v5 - 1;
						if( !Objects[v6].isBlockWalking )
							break;
					}
				}
			}
			++v10;
		}while( v10 < 8 );
		if( v10 < 8 ){
			v7 = 0;
			v8 = MonsterSprites;
			do{
				if( v8->baseMonsterIndex == Monsters[monsterIndex].SpritePtr->baseMonsterIndex )
					break;
				++v8;
				++v7;
			}while( (unsigned int)v8 < (unsigned int)&MonsterSprites[Monsters_Sprites_Max_Count] );
			if( v7 < 24 )
				AddMonster(v2, v3, v13, v7, 1);
		}
	}
}

//----- (0043C871) --------------------------------------------------------
bool __fastcall ToGolemAttackType( int monsterIndex )
{
	return B(Monsters[monsterIndex].ai) &
		( B(AI_3_Skel_Archer)
		| B(AI_7_Goat_Archer)
		| B(AI_14_Witch)
		| B(AI_29_RedVex)
		);
}

//----- (00712EA0) --------------------------------------------------------
bool __fastcall NewToGolemAttackType( int monsterIndex )
{
	return B(Monsters[monsterIndex].ai) &
		( B(AI_3_Skel_Archer)
		| B(AI_7_Goat_Archer)
		| B(AI_14_Witch)
		| B(AI_20_Acid_Beast_Ranger)
		| B(AI_25_Advocate)
		| B(AI_29_RedVex)
		| B(AI_32_Ranger_Firebolt)
		| B(AI_33_Ranger_Lite_Arrow)
		| B(AI_35_Lich)
		| B(AI_36_Arch_Lich)
		| B(AI_37_Ranger_Fire_Arrow)
		| B(AI_38_Beholder)
		//| B(AI_39_Mixed_White_Star_phys)
		| B(AI_41_Ranger_snowbolt)
		//| B(AI_42_Mixed_Firebolt)
		| B(AI_43_Ranger_Acid_Arrow)
		| B(AI_44_Ranger_Cold_Arrow)
		| B(AI_45_Ranger_Arcane_Arrow)
		| B(AI_46_Ranger_Blue_Star)
		//| B(AI_47_Mixed_Beholder_Star)
		//| B(AI_48_Mixed_Yellow_Star)
		| B(AI_49_Ranger_Fireblast)
		);
		
}

//----- (0043C898) --------------------------------------------------------
bool __fastcall IsAttackTypes_1(int monsterIndex)
{
	return B(Monsters[monsterIndex].ai) &
		( B(AI_28_Lazarus)
		| B(AI_31_Warlord_of_Blood)
		| B(AI_18_Gharbad)
		| B(AI_22_Zhar_the_Mad)
		| B(AI_23_Snotspll)
		| B(AI_30_Lachdanan)
		| B(AI_29_RedVex)
		);
}

//----- (0043C8CE) --------------------------------------------------------
void __fastcall MonsterLookAround(int monsterIndex)
{
	int targetRow = 0;
	int targetCol = 0;
	int targetIndex = -1;
	int distanceToSelectedTarget = -1;
	bool isSelectedTargetInThisRoom = false;
	Monster& monster = Monsters[monsterIndex];
	bool isSummon = monsterIndex < SummonMonsters_Count;

	if( !(monster.flag & MF_6_FRIENDLY) ){
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( !player.IsExists || Dungeon != player.dungeon || player.lvlChanging || !(player.CurLife & (-64)) ){
				continue;
			}
			bool monsterAndTargetInOneRoom = TransMap[ monster.Row ][ monster.Col ] == TransMap[ player.Row ][ player.Col ];

			int deltaRow = abs(monster.Row - player.Row);
			int deltaCol = abs(monster.Col - player.Col);
			int distanceToCurrentTarget = max(deltaRow, deltaCol);
			bool isCurrentPlayerChangeAsTarget = false;
			if( monsterAndTargetInOneRoom ){
				if( !isSelectedTargetInThisRoom ){
					isCurrentPlayerChangeAsTarget = true;
				}
				if( distanceToCurrentTarget < distanceToSelectedTarget ){
					isCurrentPlayerChangeAsTarget = true;
				}
			}
			if( !isSelectedTargetInThisRoom && distanceToCurrentTarget < distanceToSelectedTarget ){
				isCurrentPlayerChangeAsTarget = true;
			}
			if( targetIndex == -1 ){
				isCurrentPlayerChangeAsTarget = true;
			}
			if( isCurrentPlayerChangeAsTarget ){
				targetIndex = playerIndex;
				monster.flag &= ~MF_5_TARGET_MONSTER;
				distanceToSelectedTarget = distanceToCurrentTarget;
				isSelectedTargetInThisRoom = monsterAndTargetInOneRoom;
				targetRow = player.NextRow;
				targetCol = player.NextCol;
			}
		}
	}

	for( int targetMonsterIndexIndex = 0; targetMonsterIndexIndex < MonstersCount; targetMonsterIndexIndex++ ){
		int targetMonsterIndex = MonsterIndexes[targetMonsterIndexIndex];
		const Monster& targetMonster = Monsters[targetMonsterIndex];

		if( targetMonsterIndex == monsterIndex // это и есть сам монстр
			|| (targetMonster.CurrentLife & (-64)) <= 0 // противник сдох
			|| targetMonster.Row == 1 && targetMonster.Col == 0 //сидящие в засаде
			|| IsAttackTypes_1(targetMonsterIndex) && targetMonster.speechIndex // квестовый монстр?
			|| !((monster.flag & MF_6_FRIENDLY)	&& !(monster.flag & MF_12_BERSERK)) && (abs(targetMonster.Row - monster.Row) >= 2 || abs(targetMonster.Col - monster.Col) >= 2)  && !NewToGolemAttackType(monsterIndex) // противник не вблизи и ещё какое то условие
			|| !((monster.flag & MF_6_FRIENDLY)   && !(monster.flag & MF_12_BERSERK)) &&   !(targetMonster.flag & MF_6_FRIENDLY) ){ // неясное условие
				continue;
		}
		
		// Forbid summons from hitting each other
		if( (monster.flag & MF_6_FRIENDLY) && (targetMonster.flag & MF_6_FRIENDLY) ){
		    continue;
        }
        
        // Forbid summons from targeting non activated targets
		if( isSummon && targetMonster.ActivationCounter == 0 ){
			continue;
        }

      if (targetMonster.CurAction == A_15_STONE) {
		 bool canHitStoneCursed = false;
		 if (monsterIndex < SummonMonsters_Count) {
			 const int ownerIndex = monsterIndex / SummonMonstersPerPlayer_Count;
			 if (HasTrait(ownerIndex, TraitId::BreakerOfStones) || GameMode == GM_CLASSIC) {
				 canHitStoneCursed = true;
			 }
		 }

		 if (!canHitStoneCursed) {
			 continue;
		 }
	 }

		bool monsterAndTargetInOneRoom = TransMap[ monster.Row ][ monster.Col ] == TransMap[ targetMonster.Row ][ targetMonster.Col ];
		int deltaRow = abs(monster.Row - targetMonster.Row);
		int deltaCol = abs(monster.Col - targetMonster.Col);
		int distanceToCurrentTarget = max(deltaRow, deltaCol);
		bool isCurrentMonsterChangeAsTarget = false;
		if( monsterAndTargetInOneRoom ){
			if( !isSelectedTargetInThisRoom ){
				isCurrentMonsterChangeAsTarget = true;
			}
			if( distanceToCurrentTarget < distanceToSelectedTarget ){
				isCurrentMonsterChangeAsTarget = true;
			}
		}
		if( !isSelectedTargetInThisRoom && distanceToCurrentTarget < distanceToSelectedTarget ){
			isCurrentMonsterChangeAsTarget = true;
		}
		if( targetIndex == -1 ){
			isCurrentMonsterChangeAsTarget = true;
		}
		if( isCurrentMonsterChangeAsTarget ){
			monster.flag |= MF_5_TARGET_MONSTER;
			targetIndex = targetMonsterIndex;
			targetRow = targetMonster.NextRow;
			targetCol = targetMonster.NextCol;
			distanceToSelectedTarget = distanceToCurrentTarget;
			isSelectedTargetInThisRoom = monsterAndTargetInOneRoom;
		}
	}

	if( targetIndex == -1 ){
		monster.flag |= MF_11_NOENEMY;
	}else{
		monster.flag &= ~MF_11_NOENEMY;
		monster.TargetIndex = targetIndex;
		monster.TargetRow = targetRow;
		monster.TargetCol = targetCol;
	}
}

//----- (0043CBFD) --------------------------------------------------------
void __fastcall FixMonsterPosition( int monsterIndex, int direction )
{
	Monster& monster = Monsters[monsterIndex];
	ClearMonsterWalkCounters(monsterIndex);
	if( !monster.SpritePtr ){
		if( IsDebuggerPresent() ){ __debugbreak(); }else{ return; }
	}
	MonsterSprite& sprite = *monster.SpritePtr;
	MonsterAnim* animPtr = &sprite.animation[ANIM_0_STAY];
	if( sprite.baseMonsterIndex == BM_109_GOLEM ){
		animPtr = &sprite.animation[ANIM_1_WALK];
	}
	SetMonsterAnimation(monsterIndex, animPtr, direction);
	monster.PrevActionOrRowDeltaOrSpellEffect = monster.CurAction;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.CurAction = A_0_STAY;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = direction;
	CheckMonsterUnderArch(monsterIndex);
	MonsterLookAround(monsterIndex);
}

//----- (0043CC9A) --------------------------------------------------------
void __fastcall SetMonsterAnimation(int monsterIndex, MonsterAnim* animPtr, int direction)
{
	Monster& monster = Monsters[monsterIndex];
	monster.curAnimation = &animPtr->oriented[direction];
	monster.AnimationDelayIndex = 0;
	monster.animFrameCount = animPtr->frameCount;
	monster.CurFrame = 1;
	if( monster.flag & MF_16_TRACK_ATTACK ){
		for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; missileIndexIndex++ ){
			int missileIndex = MissileIndexes[missileIndexIndex];
			Missile& missile = Missiles[missileIndex];
			if( missile.BaseMissileIndex == MI_145_TENTACLE && missile.CasterIndex == monsterIndex ){
				ClearMissileCell( missileIndex );
				RemoveMissile( missileIndex, missileIndexIndex );
			}
		}
	}
	monster.flag &= ~(MF_2_BACKWARDS | MF_3_STONE | MF_16_TRACK_ATTACK);
	monster.AnimationDelayFrameCount = animPtr->animationDelay;
	monster.Orientation = direction;
}

//----- (0043CCD8) --------------------------------------------------------
void __fastcall CheckMonsterUnderArch(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( monster.Row <= 0 ){
		monster.flag &= ~MF_14_UNDER_ARCH;
		return;
	}
	bool someFlag = false;
	TileColumn& tileColumn = TileLightMap[ monster.Row - 1 ][ monster.Col + 1 ];// out of bound if monster.Row == 0
	ushort* tiles = tileColumn.tile;
	for( int i = 2; i < 10; ++i ){
		if( tiles[i] ){
			someFlag = true;
			break;
		}
	}
	if( someFlag || ArchGraphicsMap[ monster.Row - 1 ][ monster.Col + 1 ] ){
		monster.flag |= MF_14_UNDER_ARCH;
	}else{
		monster.flag &= ~MF_14_UNDER_ARCH;
	}
}

//----- (0043CD32) --------------------------------------------------------
void __fastcall ClearMonsterOnMap(int monsterIndex)
{
	Monster& monster = Monsters[ monsterIndex ];
	int prevRow = Monsters[ monsterIndex ].PrevRow;
	int prevCol = Monsters[ monsterIndex ].PrevCol;
	// был bug, выход за границы карты монстров и возможная порча карты уровня и массива тайлов подземелья в th1
	// и порча случайной памяти в th2 (в данном случае портилась память системных переменных
	// отвечающих за активность приложения в винде из-за чего и подвисало)
	for( int col = prevCol - 1; col <= prevCol + 1; ++col ){
		for( int row = prevRow - 1; row <= prevRow + 1; ++row ){
			auto cell = &MonsterMap[ row ][ col ];
			if( In112(row, col) && (*cell == -monsterIndex - 1 || *cell == monsterIndex + 1) ){
				*cell = 0;
			}
		}
	}
	if( prevRow < FineMap_112 - 1 ) FlagMap[ prevRow + 1 ][ prevCol ] &= ~CF_16_MONSTER_HORIZONTAL; // вправо вниз
	if( prevCol < FineMap_112 - 1 ) FlagMap[ prevRow ][ prevCol + 1 ] &= ~CF_16_MONSTER_HORIZONTAL;   // влево вниз
	if( HorizontalHit && monster.NextRowHz > 0 && monster.NextRowHz < 112 && monster.NextColHz > 0 && monster.NextColHz < 112 ){
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER_HORIZONTAL; // влево/вправо
	}
}

//----- (0043CDC1) --------------------------------------------------------
void __fastcall KnockbackMonster(int monsterIndex, int orientation, int range ) //#monster knockback resist, #KB rez
{
	int revers = ((uchar) orientation - 4) & 7;
	Monster& monster = Monsters[monsterIndex];
	if (monster.CurAction == A_6_DEATH) {//Do NOT reset monster animation to A_5_GOT_HIT when a monster dies, as overriding monster death animation can cause duped items to drop in MP (monster dies multiple times).
		return;
	}
	uint bm = monster.SpritePtr->baseMonsterIndex;
	auto _1  = [&](int range){ return RNG(range) == 1; }; // more range = less knockback
	auto _n2 = [&](int range){ return RNG(range) != 2; }; // more range = more knockback
	bool kb = true; // monster can be knockbacked
	if (GameMode == GM_CLASSIC) goto SKIPPE; // skip all kb rez in Classic
	if( MaxCountOfPlayersInGame == 1 ){ // single
		switch( bm ){
		case BM_0_BURNED_CORPSE or BM_2_ROTTING_CARCASS or BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_57_HELL_CLAN or BM_113_ASH_GOLEM 
			or BM_120_NECROMORPH or BM_125_GOLD_GOLEM or BM_130_BLACK_GOLEM or BM_135_GROTESQUE or BM_140_GHOUL or BM_141_SHAMBLER
			or BM_142_HELL_MEAT or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER or BM_166_GOATLORD_BOSS or BM_169_ZOMBIE 
			or BM_188_HELL_CLAN or BM_214_SATYR_LORD or BM_173_GHARBAD_BASEMNSTR or BM_110_DIABLO or BM_58_DEFILER/* or BM_51_THE_BUTCHER*/: kb = _1(4); break;
		case BM_217_GRIMSPIKE or BM_245_BLUNDERER or BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS: kb = _1(10); break; // grimspkike
		case BM_137_UBER_DIABLO: kb = _1(30); break; // uber diablo
		case BM_243_IZUAL or BM_231_BEASTMASTER_SUMMON_3 or BM_252_HEPHASTO:  kb = _1(29); break; // izual + siegebreaker + hephasto
		case BM_218_ICE_GOLEM or BM_236_CORPSE_EATER: kb = _1(7); break; // ice golem + regurgitator
		case BM_251_WENDIGO or BM_275_BEHEMOTH: kb = _1(20); break;
		case BM_244_HORADRIM_ANCIENT or BM_246_GREATER_MUMMY or BM_272_SPIDER_QUEEN or BM_288_EARTH_GOLEM or BM_292_BUGBEAR: kb = _1(3); break;
		case BM_215_CADAVER or BM_297_OROG: kb = _1(5); break;
		case BM_259_WATER_WYRM or BM_263_RED_WYRM or BM_269_BOG_CRAWLER or BM_270_MUD_CRAWLER or BM_271_THORNED_HULK or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE: kb = 0; break; // wyrm cant be KB'ed
		case BM_283_FROST_WARDEN or BM_260_ARMAGEDDON_WALKER or BM_261_SKELETAL_KNIGHT or BM_286_BRINE_CORPSE: kb = _1(6); break; // armageddon walker: 83% rez
		case BM_265_CROC or BM_266_CROC_BERS or BM_287_ARMORED_SKELETON or BM_298_TENTACLIST_BOSS: kb = _n2(3); break;
		case BM_285_PHANTOM_KNIGHT: kb = _n2(4); break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2: kb = _1(14); break;
		case BM_274_FLESH_DEVOURER: kb = _1(2); break;
		}
	}else{ // multi
		// if( bm <= 218 ){ kb = _1(2); }else // exclusive show-stopper, may be error, commented now
		switch(bm){
		case BM_218_ICE_GOLEM: kb = _1(7); break; // got from single player part
		case BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_16_HORNED_BEAST or BM_18_VORTEX_ROGUE or BM_52_BILE_DEMON or BM_55_GRIM_DEMON 
			or BM_56_PIT_DEMON or BM_59_PIT_FIEND or BM_65_HELL_SPAWN or BM_66_HORNED_DEATH or BM_67_BLACK_HORN or BM_68_GORE_VIPER or BM_287_ARMORED_SKELETON
			or BM_69_WINGED_DEMON or BM_70_FROST_WING or BM_71_THUNDER_LORD or BM_72_FIREWING or BM_73_DEATH_ANGEL or BM_74_GREATER_HULK 
			or BM_75_BLOOD_HULK or BM_76_DEATH_CLAW or BM_77_STORM_DEMON or BM_79_FEAR_HUNTER or BM_80_DEVIL_KIN or BM_81_FLYING_DEMON or BM_298_TENTACLIST_BOSS
			or BM_82_GARGOYLE or BM_83_LASH_WORM or BM_84_DEATH_WING or BM_85_BALOR or BM_86_SLAYER or BM_87_DARK_REAPER or BM_88_BALROG 
			or BM_93_FLAYED_ONE or BM_95_GHOST_WARRIOR or BM_96_CURSE_BEARER or BM_99_CLEAVER_BOSS or BM_122_BLOOD_SPECTRE or BM_132_BONEWING or BM_136_BLOOD_SLAYER
			or BM_159_GUARDIAN or BM_160_CHASM_DEMON or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER or BM_148_TURBAK_THE_DEUCE or BM_176_DESTROYER 
			or BM_178_OVERLORD or BM_179_FILTH_DEMON or BM_203_DEATH_KNIGHT or BM_210_HELLWING or BM_211_TORCHANT or BM_212_CACODEMON or BM_285_PHANTOM_KNIGHT: kb = _1(2); break;
		case BM_33_MOON_CLAN or BM_57_HELL_CLAN or BM_60_HELL_STONE or BM_61_BLOOD_STONE or BM_62_MAULER or BM_63_LAVA_LORD or BM_127_WYRM 
			or BM_153_DEATH_MINION or BM_162_CORRUPT_PALADIN or BM_180_CRYPT_STONE or BM_197_CEROMORTH or BM_214_SATYR_LORD: kb = _1(4); break;
		case BM_236_CORPSE_EATER: kb = _1(90); break; // corpse eater: chance to be KB'ed in MP = 1/90
		case BM_110_DIABLO or BM_50_SKELETON_LORD or BM_51_THE_BUTCHER or BM_244_HORADRIM_ANCIENT or BM_246_GREATER_MUMMY: kb = _1(8); break;
		case BM_58_DEFILER or BM_231_BEASTMASTER_SUMMON_3: kb = _1(12); break; // defiler + siegebreaker
		case BM_297_OROG: kb = _1(10); break;
		case BM_209_HATRED_CLAN or BM_204_SHADOW_CLAN or BM_187_BLOOD_CLAN or BM_34_FLESH_CLAN or BM_35_FIRE_CLAN or BM_36_DARKNESS_CLAN 
			or BM_37_STONE_CLAN or BM_46_PIT_SPIDER or BM_97_DARK_CHARGER or BM_112_WAILING_BEAST or BM_115_GRUNT_BEAST or BM_118_DRAGON_KIN 
			or BM_123_BICLOPS or BM_134_MUD_RUNNER or BM_161_CHASM_DRAKE or BM_173_GHARBAD_BASEMNSTR or BM_175_TERROR_CLAN or BM_186_DEATH_CLAN
			or BM_265_CROC or BM_266_CROC_BERS: kb = _n2(3); break;
		case BM_140_GHOUL or BM_152_BLACK_REAPER: kb = _1(3); break;
		case BM_141_SHAMBLER or BM_142_HELL_MEAT or BM_166_GOATLORD_BOSS or BM_137_UBER_DIABLO or BM_120_NECROMORPH or BM_125_GOLD_GOLEM 
			or BM_135_GROTESQUE or BM_169_ZOMBIE or BM_188_HELL_CLAN or BM_193_ABATAN or BM_195_UBER_BUTCHER or BM_205_PINKY or BM_207_DREGBEZ: kb = 0; break;
		case BM_154_DEATH_MINION or BM_272_SPIDER_QUEEN or BM_292_BUGBEAR: kb = _1(6); break;
		case BM_274_FLESH_DEVOURER or BM_288_EARTH_GOLEM: kb = _1(4); break;
		case BM_64_DEATH_GUARDIAN: 
			if( Dungeon == DUN_16_HELL_4 ){ 
				kb = 0; 
			}
			else{ 
				kb = _1(2); 
			} 
			break;
		case BM_111_OBSIDIAN_GOLEM or BM_113_ASH_GOLEM or BM_130_BLACK_GOLEM: 
			if( Difficulty == DL_0_HORROR ){ 
				kb = _1(5); 
			}
			else{ 
				kb = 0; 
			} 
			break;
		case BM_7_RISEN_SKELETON or BM_8_BONE_CREEPER or BM_9_SKELETON_WARRIOR or BM_10_BURNING_DEAD or BM_11_HATRED or BM_24_RISEN_SKELETON or BM_25_SKELETON_WARRIOR 
			or BM_26_BURNING_DEAD or BM_27_HORROR or BM_106_CORRUPT_ORDER or BM_116_IMP_BRUTE or BM_126_PLAGUE_EATER or BM_128_GREATER_IMP: kb = _n2(5); break;		
		case BM_215_CADAVER: kb = _1(9); break; 
		case BM_259_WATER_WYRM or BM_263_RED_WYRM or BM_269_BOG_CRAWLER or BM_270_MUD_CRAWLER or BM_271_THORNED_HULK or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE: kb = 0; break; // wyrms cant be KB'ed
		case BM_283_FROST_WARDEN or BM_260_ARMAGEDDON_WALKER or BM_261_SKELETAL_KNIGHT or BM_286_BRINE_CORPSE: kb = _1(12); break; // 91.5% rez
		case BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS: kb = _1(20); break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2: kb = _1(28); break;
		case BM_275_BEHEMOTH: kb = _1(40); break;
		}
	}
	SKIPPE:
	if( kb ){
		bool walkUpHoriz = is(monster.CurAction, A_1_WALK_UP, A_3_WALK_HORIZ);
		bool walkDown = monster.CurAction == A_2_WALK_DOWN;
		if( range == 2 ){
			kb = monster.Orientation == orientation && walkUpHoriz && MayMoveMonsterToDirection(monsterIndex, revers)
			  || monster.Orientation == orientation && walkDown && MayMoveMonsterToDirection(monsterIndex, revers, 2)
			  || (monster.Orientation != orientation || !walkDown && !walkUpHoriz) && MayMoveMonsterToDirection(monsterIndex, revers);
			if( kb ){
				ClearMonsterOnMap( monsterIndex );
				int d = 1 + (monster.Orientation == orientation && walkDown); // double knockback
				monster.Row += RowDelta [ revers ] * d;
				monster.Col += ColDelta [ revers ] * d;
			}else{
				range = 1;
			}
		}
		if( range == 1 ){
			kb = walkUpHoriz && monster.Orientation == orientation || MayMoveMonsterToDirection(monsterIndex, revers);
			if( kb ){
				ClearMonsterOnMap( monsterIndex );
				if( !walkUpHoriz || monster.Orientation != orientation ){ // fix original double knockback
					monster.Row += RowDelta [ revers ]; // original d1 was prevRow, higly likely it was a bug
					monster.Col += ColDelta [ revers ]; // original d1 was prevCol
				}
			}
		}
		if( kb ){
			SetMonsterAnimation( monsterIndex, &monster.SpritePtr->animation[ ANIM_3_HIT ], monster.Orientation );
			monster.CurAction = A_5_GOT_HIT;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch( monsterIndex );
			ClearMonsterOnMap( monsterIndex );
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
			if (!(monster.flag & MF_1_INVISIBLE) && monster.LightIndex) {
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
			}
		}
	}
}

//----- (0043CEAF) --------------------------------------------------------
void __fastcall DamageMonsterByPlayer(int monsterIndex, int playerIndex, int damage, int dot /*= 0*/ ) // # monster stun formula, #monster stun threshold
{
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];

	if (monster.CurAction == A_6_DEATH) {//Duped drops fix for MP, additional safety check
		return;
	}

	if (playerIndex >= 0) {
		monster.Attackers |= 1 << playerIndex;
	}
	if (playerIndex == CurrentPlayerIndex) {
		delta_monster_hp(monsterIndex, monster.CurrentLife, (DUNGEON)Dungeon);
		DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
		SendCmdDamageMonster(0, monsterIndex, damage);
	}
	if( dot && RNG(120) ){ return; }// 1/120 chance to hit and stun monster with acid flask per frame damage
	PlayMonsterSound(monsterIndex, MST_1_GOTHIT);
	int baseMon = monster.SpritePtr->baseMonsterIndex;
	if ((baseMon != BM_195_UBER_BUTCHER) && (!HasTrait(playerIndex, TraitId::ChromaticSkin))) {
		int extDamage = damage / 64;
		if (playerIndex >= 0) { // not a trap
			extDamage += extDamage * PerkValue(PERK_CONCUSSIVE_BLOWS, playerIndex) / 100; // warrior stun improver
			extDamage += extDamage * PerkValue(PERK_DEADLY_FLETCHERY, playerIndex) / 100; // scout stun improver
			extDamage += extDamage * PerkValue(PERK_STOPPING_POWER, playerIndex) / 100; // PISTOLEER'S stun improver
		}
		int monster_stun_threshold = RollMonsterStunThreshold(monster);
		if (baseMon == BM_39_WARPING_DEMON && extDamage < 5 * (monster.ArmorClass - 25)
			|| (extDamage < monster_stun_threshold ? RNG(100) > 98 : RNG(100) != 99)
			|| extDamage >= GetMonsterLifeStunThreshold(monster)) {
			if (playerIndex >= 0) {
				monster.TargetIndex = playerIndex;
				monster.TargetRow = (char) player.NextRow;
				monster.TargetCol = (char) player.NextCol;
				monster.flag &= ~MF_5_TARGET_MONSTER;
				monster.Orientation = MonsterOrientationToTarget(monsterIndex);
			}
			if (is(baseMon, BM_150_FELLTWIN, BM_151_HYBRID) && RNG(2) == 1 // warpers' dance
				|| is(baseMon, BM_28_BLOOD_WARPER, BM_39_WARPING_DEMON, BM_177_PHASE_DEMON, BM_182_BLINK_DEMON, BM_194_BLOODIED_HIDDEN, BM_208_WARPED_ONE)) {
				if ( (!HasTrait(playerIndex, TraitId::Psion)) && (!HasTrait(playerIndex, TraitId::Hydramancer)) ) { // mor: i really hope this won't give problems if player index turns out to be below 0
					MonsterJumpAroundTarget(monsterIndex);
				}
			}
			else if (monster.ai == AI_4_Scavenger) {
				monster.goal = MG_1_ATTACK;
				monster.goalParam = 0;
				monster.curOrientation = 0;
			}
			if (monster.CurAction != A_15_STONE) {
				SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_3_HIT], monster.Orientation);
				monster.CurAction = A_5_GOT_HIT;
				monster.DeltaX = 0;
				monster.DeltaY = 0;
				monster.Row = monster.PrevRow;
				monster.Col = monster.PrevCol;
				monster.NextRow = monster.Row;
				monster.NextCol = monster.Col;
				monster.PrevRow = monster.Row;
				monster.PrevCol = monster.Col;
				CheckMonsterUnderArch(monsterIndex);
				ClearMonsterOnMap(monsterIndex);
				MonsterMap[monster.Row][monster.Col] = monsterIndex + 1;
			}
		}
	}
}

//----- (0043D064) --------------------------------------------------------
int __fastcall MonsterOrientationToTarget(int monsterIndex)
{
	return OrientationToTarget(Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, Monsters[monsterIndex].TargetRow, Monsters[monsterIndex].TargetCol);
}

//----- (0043D08C) --------------------------------------------------------
void __fastcall KillMonsterByPlayer(uint monsterIndex, int playerIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];

	if (monster.CurAction == A_6_DEATH) {//Duped drops fix for MP
		return;
	}

	if( CurrentPlayerIndex == playerIndex ){
		delta_kill_monster(monsterIndex, monster.Row, monster.Col, (DUNGEON)Dungeon);

		if( monsterIndex < SummonMonsters_Count ){
			NetSendCmdLocParam2(0, CMD_95_KILLGOLEM, monster.Row, monster.Col, monsterIndex, monster.dungeon);
		}else{
			NetSendCmdLocParam2(0, CMD_36_MONSTDEATH, monster.Row, monster.Col, monsterIndex, monster.dungeon);
		}
	}
	MonsterKilledByPlayer(monsterIndex, playerIndex, 1);
}

//----- (0043D109) --------------------------------------------------------
void __fastcall MonsterKilledByPlayer(int monsterIndex, int playerIndex, bool sendMsg)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count || !monster.SpritePtr || monster.CurAction == A_6_DEATH ){
		return;
	}
	MonsterSprite& sprite = *monster.SpritePtr;	
	if( playerIndex >= 0 ){
		monster.Attackers |= 1 << playerIndex;
	}
	if( playerIndex >= 0 && playerIndex < 4 && monsterIndex >= SummonMonsters_Count && !monster.IsSummonedByMonster){
		AddXpForMonsterKill(monster.MonsterLevel, monster.Xp, monster.Attackers, monsterIndex );
	}
	TotalSlain[sprite.baseMonsterIndex]++;
	monster.CurrentLife = 0;
	if (!monster.IsSummonedByMonster) {
		int seed = monster.RandomSeedForDrop;
		// if( MaxCountOfPlayersInGame == 1) // MP smart drop
		{
			seed ^= Players[CurrentPlayerIndex].magicFind; // Randomize base item depending on mf value
		}
		SetRndSeed(seed);
		MonsterKillEffect(monsterIndex, sendMsg);
	}
	if( (MaxCountOfPlayersInGame == 1 && sprite.baseMonsterIndex == BM_137_UBER_DIABLO && (uint)Difficulty < 1
	 && !is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) 
		|| GameMode == GM_CLASSIC && sprite.baseMonsterIndex == BM_C_389_DIABLO){
		KillDiabloEffect(monsterIndex, 1);
	}else{
		PlayMonsterSound(monsterIndex, MST_2_DEATH);
	}
	if( playerIndex >= 0 ){
		monster.Orientation = MonsterOrientationToTarget(monsterIndex);
	}
	SetMonsterAnimation(monsterIndex, &sprite.animation[ANIM_4_DIE], monster.Orientation);
	monster.Col = monster.PrevCol;
	monster.Row = monster.PrevRow;
	monster.NextCol = monster.PrevCol;
	monster.NextRow = monster.PrevRow;
	monster.CurAction = A_6_DEATH;
	monster.goal = MG_0_DEATH;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.PrevActionOrRowDeltaOrSpellEffect = A_0_STAY;
	CheckMonsterUnderArch(monsterIndex);
	ClearMonsterOnMap(monsterIndex);
	MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
	SayMessageOnBossKill(monsterIndex, sendMsg);
	BewareNearMonsters(monster.Row, monster.Col);
	int baseMon = sprite.baseMonsterIndex;
	// monsters that leave acid pools on death:
	if( is( baseMon, BM_46_PIT_SPIDER, BM_47_ACID_HOUND, BM_48_ACID_BAT, BM_49_BILE_SPITTER, BM_172_SPITTING_TERROR, BM_206_ACID_SPITTER, BM_127_WYRM, BM_241_ACID_TERROR, 
		BM_C_380_LAVA_MAW, BM_C_379_PIT_BEAST, BM_C_378_POSON_SPITTER, BM_C_377_ACID_BEAST ) ){
		CastMissile(monster.Row, monster.Col, 0, 0, 0, MI_59_ACID_POOLS, CT_1_MONSTER_AND_TRAP, monsterIndex, (Dungeon->level / 6) + (Difficulty * 4)/*monster.intelligenceFactor + 1*/, 0, 0 );
	}
	if (MaxCountOfPlayersInGame == 1 && is(baseMon, BM_212_CACODEMON, BM_71_THUNDER_LORD, BM_76_DEATH_CLAW, BM_77_STORM_DEMON, BM_78_EVISCERATOR, BM_79_FEAR_HUNTER, BM_160_CHASM_DEMON, BM_164_LIGHTNING_DEMON )) {
		CastMissile(monster.Row, monster.Col, 0, 0, 0, MI_70_LIGHTNING_WALL_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, (Dungeon->level / 6) + (Difficulty * 4), 0, 0);
	}
	if (MaxCountOfPlayersInGame == 1 && is(baseMon, BM_60_HELL_STONE, BM_61_BLOOD_STONE, BM_62_MAULER, BM_63_LAVA_LORD, BM_180_CRYPT_STONE )) {
		CastMissile(monster.Row, monster.Col, 0, 0, 0, MI_5_FIRE_WALL_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, (Dungeon->level / 6) + (Difficulty * 4), 0, 0);
	}
}

// эффект анимации на убере в 24 уровне работает криво, разобраться (для проверки можно отключить PlayDiabloVictory в MainGamePlayLoop на DL_0_HORROR
//----- (0043D2B1) --------------------------------------------------------
void __fastcall KillDiabloEffect(int monsterIndex, int a1)
{
	int v2; // esi@1
	int v3; // edi@1
	int v4; // ebx@5
	int v5; // esi@7
	int v6; // esi@9
	int v7; // eax@10
	int v8; // ecx@14
	double v9; // st7@14
	int v11; // ecx@14
	int v12; // [sp+Ch] [bp-Ch]@1
	int v13; // [sp+10h] [bp-8h]@3
	int *v14; // [sp+14h] [bp-4h]@4
	int v15; // [sp+14h] [bp-4h]@12

	v2 = a1;
	v12 = monsterIndex;
	v3 = monsterIndex;
	PlayGlobalSound( 970 );
	Quests[ Q_5_DIABLO ].status = QS_3_COMPLETE;
	if( v2 ){
        NetSendCmdQuest( 1, Q_5_DIABLO );
	}
	SaveSound = IsSoundEnable;
	BoolWithPlayer = 0;
	v13 = 0;
	if( MonstersCount > 0 ){
		v14 = MonsterIndexes;
		do{
			v4 = *v14;
			if( *v14 != v12 && Monsters[ v3 ].ActivationCounter ){
				v5 = v4;
				SetMonsterAnimation( *v14, &Monsters[ v4 ].SpritePtr->animation[ANIM_4_DIE], Monsters[ v4 ].Orientation );
				Monsters[ v5 ].CurAction = A_6_DEATH;
				Monsters[ v5 ].DeltaX = 0;
				Monsters[ v5 ].DeltaY = 0;
				Monsters[ v5 ].PrevActionOrRowDeltaOrSpellEffect = A_0_STAY;
				Monsters[ v5 ].Row = Monsters[ v5 ].PrevRow;
				Monsters[ v5 ].Col = Monsters[ v5 ].PrevCol;
				Monsters[ v5 ].NextRow = Monsters[ v5 ].Row;
				Monsters[ v5 ].NextCol = Monsters[ v5 ].Col;
				Monsters[ v5 ].PrevRow = Monsters[ v5 ].Row;
				Monsters[ v5 ].PrevCol = Monsters[ v5 ].Col;
				CheckMonsterUnderArch( v4 );
				ClearMonsterOnMap( v4 );
				MonsterMap[ 0 ][ Monsters[ v5 ].Col + 112 * Monsters[ v5 ].Row ] = v4 + 1;
			}
			++v13;
			++v14;
		} while( v13 < MonstersCount );
	}
	AddLight( Monsters[ v3 ].Row, Monsters[ v3 ].Col, 8 );
	DoVision( Monsters[ v3 ].Row, Monsters[ v3 ].Col, 8, 0, 1 );
	v6 = abs( PlayerColPos - Monsters[ v3 ].Col );
	if( abs( PlayerRowPos - Monsters[ v3 ].Row ) <= v6 ){
		v7 = PlayerColPos - Monsters[ v3 ].Col;
	}else{
		v7 = PlayerRowPos - Monsters[ v3 ].Row;
	}
	v15 = abs( v7 );
	if( v15 > 20 ){
		v15 = 20;
	}
	Monsters[ v3 ].ActionRow_ActOrient = PlayerRowPos << 16;
	v8 = Monsters[ v3 ].ActionRow_ActOrient;
	v9 = (double) v15;
	Monsters[ v3 ].NextRowHz = PlayerColPos << 16;
	Monsters[ v3 ].NextColHz = ftol ((double) (v8 - (Monsters[ v3 ].Row << 16)) / v9);
	v11 = Monsters[ v3 ].NextRowHz;
	Monsters[ v3 ].WalkX = ftol ((double) (v11 - (Monsters[ v3 ].Col << 16)) / v9);
}

//----- (th3) -------------------------------------------------------------
void SpecifiedUniqueDrop(int monsterIndex, bool sendMsg) // #boss unique drops
{
	int playerIndex = CurrentPlayerIndex;
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[ monsterIndex ];
	
	int uniqIndex = -1;

	if (GameMode == GM_CLASSIC && monster.newBossId) {
		int bossIndex = monster.newBossId - 1;
		if (bossIndex == UM_C_822_The_Butcher)	 { uniqIndex = 4674 + Difficulty; }
		else if (bossIndex == UM_C_872_SKING)	 { uniqIndex = 4686 + Difficulty; }
		else if (bossIndex == UM_C_902_RAGEHORN) { uniqIndex = 4773 + Difficulty; }
			goto SKIP_CL;
	}

	if (monster.newBossId) {
		int bossIndex = monster.newBossId - 1;
		switch (bossIndex) {
		case UM_54_Kobal_the_Duke:
			if (Dungeon == DUN_65_RUINED_TEMPLE) {
				uniqIndex = by(Difficulty, 160, 161, 162, 2365, 2366, 2367, 2368, 2369, 2370);
			}
			break;
		case UM_4_ArchBishop_Lazarus:
			{
				bool traitorsApparel = false;
				if (
					player.fullClassId == PFC_MAGE && (!(HasTrait(playerIndex, TraitId::Psion)))
					|| player.fullClassId == PFC_ELEMENTALIST && (!(HasTrait(playerIndex, TraitId::Mamluk)))
					|| player.fullClassId == PFC_WARLOCK 
					) {
					traitorsApparel = true;
				}
				if (traitorsApparel) {
					uniqIndex = by(Difficulty, 4599, 4600, 4601, 4602, 4603, 4604, 4605, -1, -1);
				}
			}
			break;// << lazarus block ends here
		case UM_9_The_Butcher:
			if ((player.fullClassId == PFC_ROGUE && HasTrait(playerIndex, TraitId::Axepertise))
				|| player.fullClassId == PFC_BERSERKER
				|| player.fullClassId == PFC_DRUID
				|| player.fullClassId == PFC_EXECUTIONER) {
				switch (Difficulty) {
				case DL_0_HORROR: uniqIndex = player.CharLevel < 7 ? 2503 : -1; break; // RngFrom(5, 6, 7, 10); break;
					// uniqIndex = RngFrom( 2503, 2503, -1, -1, -1); <<< if you want to drop this unique with 40% probability
				case DL_1_PURGATORY: uniqIndex = player.CharLevel < 33 ? 2508 : -1; break;
				case DL_2_DOOM:		 uniqIndex = player.CharLevel < 43 ? 2509 : -1; break;
				case DL_3_CRUCIBLE:  uniqIndex = player.CharLevel < 53 ? 2510 : -1; break;
				case DL_4_TORMENT:   uniqIndex = player.CharLevel < 59 ? 2511 : -1; break;
				}
				break;
			}
			else if (player.fullClassId == PFC_WARRIOR
				|| player.fullClassId == PFC_GUARDIAN
				|| player.fullClassId == PFC_TEMPLAR
				|| player.fullClassId == PFC_ARCHER
				|| player.fullClassId == PFC_SHARPSHOOTER
				|| player.fullClassId == PFC_MONK
				|| player.fullClassId == PFC_KENSEI
				|| player.fullClassId == PFC_SHUGOKI
				|| player.fullClassId == PFC_SHINOBI
				|| player.fullClassId == PFC_ROGUE
				|| player.fullClassId == PFC_ASSASSIN
				|| player.fullClassId == PFC_SAVAGE
				|| player.fullClassId == PFC_THRAEX
				|| player.fullClassId == PFC_SECUTOR
				|| player.fullClassId == PFC_DIMACHAERUS
				|| player.fullClassId == PFC_MURMILLO
				|| player.fullClassId == PFC_DRUID
				|| ( player.fullClassId == PFC_ELEMENTALIST && HasTrait( playerIndex, TraitId::Mamluk ) ) ){
				if (Difficulty == DL_0_HORROR && player.CharLevel <= 6) {
					uniqIndex = RngFrom(2564, -1, -1, -1, -1);
				}
				else if (Difficulty == DL_1_PURGATORY && player.CharLevel <= 33) {
					uniqIndex = RngFrom(2565, -1, -1, -1, -1);
				}
				else if (Difficulty == DL_2_DOOM && player.CharLevel <= 42) {
					uniqIndex = RngFrom(2566, -1, -1, -1, -1);
				}
				else if (Difficulty == DL_3_CRUCIBLE && player.CharLevel <= 52) {
					uniqIndex = RngFrom(2567, -1, -1, -1);
				}
			}
			break;// << butcher block ends here
		case UM_1_Skeleton_King:
			if (MaxCountOfPlayersInGame != 1 || Dungeon == DUN_99_ANCIENT_CRYPTS) {
				break;
			}
			if (HasTrait(playerIndex, TraitId::Mamluk, TraitId::Ranger) && Difficulty == DL_0_HORROR) {
				uniqIndex = 2504; break;
			}
			if (HasTrait(playerIndex, TraitId::Bloodless)) { // Monk who can't lifesteal, doesn't get it
				break;
			}
			if (HasTrait(playerIndex, TraitId::Pistoleer)) { // Pistoleer cant wield anything but pistols
				break;
			}
			if (HasTrait(playerIndex, TraitId::Paladin) && Difficulty == DL_0_HORROR) { // Paladin gets special version
				uniqIndex = 4590; break;
			}

			switch (player.fullClassId) {
			case PFC_SAVAGE or PFC_GUARDIAN:
				switch (Difficulty) {
				case DL_0_HORROR:	  uniqIndex = RngFrom(2512, 2504); break; // get 2H sword or battle Crown
				case DL_1_PURGATORY:  uniqIndex = RngFrom(2513, -1); break;
				case DL_2_DOOM:		  uniqIndex = RngFrom(2514, -1); break;
				case DL_3_CRUCIBLE:   uniqIndex = RngFrom(2515, -1); break;
				case DL_4_TORMENT:	  uniqIndex = RngFrom(2516, -1); break;
				} break;
				case PFC_WARRIOR or PFC_INQUISITOR or PFC_TEMPLAR
					or PFC_MONK or PFC_KENSEI or PFC_SHUGOKI or PFC_SHINOBI
					or PFC_ROGUE or PFC_ASSASSIN or PFC_IRON_MAIDEN
				or PFC_BERSERKER or PFC_THRAEX or PFC_DIMACHAERUS or PFC_MURMILLO or PFC_SECUTOR:
					switch (Difficulty) {
					case DL_0_HORROR: uniqIndex = 2504; break;
					} break;
				case PFC_DEMONOLOGIST or PFC_NECROMANCER or PFC_BEASTMASTER:// summoners
					// this is summoner set drop (The Royal Relics)
					if (is(player.fullClassId, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_NECROMANCER) && Difficulty != DL_8_INFERNO) {
						SpawnUnique(Difficulty + 4610, monster.Row, monster.Col, sendMsg);
					}
					switch (Difficulty) {// standard SK drop on Horror: Undead Crown
					case DL_0_HORROR: uniqIndex = 2505; break;
					} break;
				case PFC_ARCHER or PFC_SHARPSHOOTER or PFC_TRAPPER:
					switch (Difficulty) {
					case DL_0_HORROR: uniqIndex = 2506; break;
					} break;
				case PFC_MAGE or PFC_ELEMENTALIST or PFC_WARLOCK:
					switch (Difficulty) {
					case DL_0_HORROR: uniqIndex = 2507; break;
					} break;
			}
			break;// << leoric block ends here
		case UM_0_Gharbad_the_Weak:
			switch (player.fullClassId) {
			case PFC_WARRIOR or PFC_TEMPLAR or PFC_THRAEX or PFC_DIMACHAERUS or PFC_MURMILLO or PFC_SECUTOR:
				if (HasTrait(playerIndex, TraitId::Ranger)) {
					break;
				}
				switch (Difficulty) {
				case DL_0_HORROR:		uniqIndex = RngFrom(2517, -1); break;
				case DL_1_PURGATORY:	uniqIndex = RngFrom(2518, -1); break;
				case DL_2_DOOM:			uniqIndex = RngFrom(2519, -1); break;
				case DL_3_CRUCIBLE:		uniqIndex = RngFrom(2520, -1); break;
				case DL_4_TORMENT:		uniqIndex = RngFrom(2521, -1); break;
				case DL_5_AGONY:		uniqIndex = RngFrom(2522, -1); break;
				case DL_6_TERROR:		uniqIndex = RngFrom(2523, -1); break;
				case DL_7_DESTRUCTION:	uniqIndex = RngFrom(2524, -1); break;
				case DL_8_INFERNO:		uniqIndex = RngFrom(2525, -1); break;
				} break;
			case PFC_INQUISITOR:				
				switch (Difficulty) {
				case DL_0_HORROR:		uniqIndex = 2539; break;
				case DL_1_PURGATORY:	uniqIndex = 2540; break;
				case DL_2_DOOM:			uniqIndex = 2541; break;
				case DL_3_CRUCIBLE:		uniqIndex = 2542; break;
				case DL_4_TORMENT:		uniqIndex = 2543; break;
				case DL_5_AGONY:		uniqIndex = 2544; break;
				case DL_6_TERROR:		uniqIndex = 2545; break;
				case DL_7_DESTRUCTION:	uniqIndex = 2546; break;
				case DL_8_INFERNO:		uniqIndex = 2547; break;
				} break;
			} 
			break;// << gharbad block ends here
		case UM_8_Warlord_of_Blood:
			switch (player.fullClassId) {
			case PFC_WARRIOR or PFC_TEMPLAR or PFC_INQUISITOR or PFC_THRAEX or PFC_MURMILLO or PFC_SECUTOR:
				if (HasTrait(playerIndex, TraitId::Ranger))			{ break; }
				if (HasTrait(playerIndex, TraitId::Fechtmeister))	{ break; }
				if (Difficulty <= DL_3_CRUCIBLE) {
					SpawnUnique(RngFrom(Difficulty + 2526, -1, -1), monster.Row, monster.Col, sendMsg);
				}
			}
			{
				bool rob = false;
				if (is(player.fullClassId, PFC_KENSEI, PFC_SHUGOKI, PFC_SHINOBI, PFC_ROGUE, PFC_ASSASSIN, PFC_BERSERKER, PFC_MURMILLO, PFC_THRAEX, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID)
					|| player.fullClassId == PFC_WARRIOR && (!(HasTrait(playerIndex, TraitId::Ranger)))
					|| player.fullClassId == PFC_GUARDIAN && (!(HasTrait(playerIndex, TraitId::Pistoleer)))
					|| player.fullClassId == PFC_MONK && (!(HasTrait(playerIndex, TraitId::Bloodless)))
					|| player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)
					|| player.fullClassId == PFC_TEMPLAR && (!(HasTrait(playerIndex, TraitId::Paladin)))) {
					rob = true;
				}
				if (HasTrait(playerIndex, TraitId::OldFashioned)) { rob = false; }
				if (rob) {
					uniqIndex = by(Difficulty, 4592, 4593, 4594, 4595, 4596, 4597, 4598, -1, -1);
				}
			}
			break;// << warlord block ends here
		case UM_57_Gorash:
			switch (player.fullClassId) {
			case PFC_WARRIOR or PFC_GUARDIAN or PFC_TEMPLAR or PFC_INQUISITOR 
							 or PFC_SAVAGE or PFC_BERSERKER or PFC_THRAEX or PFC_MURMILLO or PFC_DIMACHAERUS or PFC_SECUTOR:				
				if (HasTrait(playerIndex, TraitId::OldFashioned)) {
					break;
				}
				switch (Difficulty) {
				case DL_2_DOOM:		uniqIndex = RngFrom(2530, -1, -1, -1); break;
				} break;
			} 
			break;// << gorash block ends here
		case UM_257_Bloodraven:
			switch (player.fullClassId) {
				case PFC_SCOUT:					
					switch (Difficulty) {
					case DL_0_HORROR:		uniqIndex = RngFrom(2531, -1); break;
					case DL_1_PURGATORY:	uniqIndex = RngFrom(2532, -1); break;
					case DL_2_DOOM:			uniqIndex = RngFrom(2533, -1); break;
					case DL_3_CRUCIBLE:		uniqIndex = RngFrom(2534, -1); break;
					case DL_4_TORMENT:		uniqIndex = RngFrom(2535, -1); break;
					} break;
			} 
			break;// << bloodraven block ends here
		case UM_191_Shadowcrow:
			if ((player.fullClassId == PFC_WARRIOR && ((!HasTrait(playerIndex, TraitId::Fechtmeister)) && (!HasTrait(playerIndex, TraitId::Ranger))))
				|| player.fullClassId == PFC_SHINOBI
				|| player.fullClassId == PFC_THRAEX) {
				switch (Difficulty) {
				case DL_0_HORROR:		uniqIndex = RngFrom(2568, -1, -1); break;
				case DL_1_PURGATORY:	uniqIndex = RngFrom(2570, -1); break;// << may want to continue these lines
				} break;// << diff switch end
			}	
			else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_ASSASSIN) {
				switch (Difficulty) {
				case DL_0_HORROR:		uniqIndex = RngFrom(2569, -1, -1); break;
				case DL_1_PURGATORY:	uniqIndex = RngFrom(2571 , -1); break;
				} break;// << diff switch end
			}
			break;// << shadowcrow block ends here
		case UM_703_Hive_Mother:
		{
			bool fighters = is(player.fullClassId, PFC_WARRIOR, PFC_TEMPLAR, PFC_SHINOBI, PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR, PFC_DIMACHAERUS);
			if (HasTrait(playerIndex, TraitId::Ranger)) { 
				fighters = false; 
			}
			bool rogueWithoutAxe = player.fullClassId == PFC_ROGUE && (!HasTrait(playerIndex, TraitId::Axepertise));
			bool maidenWithout2Towers = player.fullClassId == PFC_IRON_MAIDEN && (!HasTrait(playerIndex, TraitId::TwoTowers));
			if (HasTrait(playerIndex, TraitId::Mamluk) || fighters || rogueWithoutAxe || maidenWithout2Towers) {
				uniqIndex = Difficulty + 2834;// 2834-2842 depending on difficulty mode
			}
		}
			break;
		case UM_726_Apparition_of_Arkaine:
		case UM_745_Xamutgor:
			if (HasTrait(playerIndex, TraitId::OldFashioned)) {
				break;
			}
			if (HasTrait(playerIndex, TraitId::Mamluk)) {
				uniqIndex = by(Difficulty, 178, 179, 180, 139, 140, 141, 142, 143, 144);
			}
			else{
				uniqIndex = valorRewards[player.ClassID][Difficulty];
			}
			break;
		case UM_714_Sir_Darion:
			if (Dungeon != DUN_107_DREAD_SANCTUARY) { break; }
			uniqIndex = mundaneInventoryRewards[Difficulty];
			break;
		case UM_747_Tronzug:
			uniqIndex = by(Difficulty, 152, 153, 154, 2383, 2384, 2385, 2386, 2387, 2388);
			break;
		case UM_584_Mordessa:
			if (is(player.fullClassId, PFC_ARCHER, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN)) {
				uniqIndex = by(Difficulty, 4606, 4607, 4608, 4609, -1, -1, -1, -1, -1);
			}
			break;
		case UM_809_Borosteron:
			if (player.fullClassId == PFC_MONK) {
				uniqIndex = Difficulty + 4618;
			}
			else if (
				is(player.fullClassId, PFC_TEMPLAR, PFC_SHINOBI, PFC_THRAEX, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_MURMILLO)
				|| (player.fullClassId == PFC_IRON_MAIDEN && (!HasTrait(playerIndex, TraitId::TwoTowers)))
				|| (player.fullClassId == PFC_WARRIOR && (!HasTrait(playerIndex, TraitId::Ranger)))
				|| (player.fullClassId == PFC_ROGUE && (!HasTrait(playerIndex, TraitId::Axepertise)))
				) {
				uniqIndex = Difficulty + 4627;
			}
			else if (
				is(player.fullClassId, PFC_GUARDIAN, PFC_KENSEI, PFC_SAVAGE) 
				&& (!HasTrait(playerIndex, TraitId::Pistoleer))
				) {
				uniqIndex = Difficulty + 4636;
			}
			else if (
				is(player.fullClassId, PFC_SHUGOKI, PFC_EXECUTIONER, PFC_DRUID, PFC_BERSERKER)
				|| (player.fullClassId == PFC_ROGUE && (HasTrait(playerIndex, TraitId::Axepertise)))
				) {
				uniqIndex = Difficulty + 4645;
			}
			else if (HasTrait(playerIndex, TraitId::Mamluk)){ 
				uniqIndex = Difficulty + 4655; 
			}
			break;
		case UM_752_Deathcaller:
			if (Dungeon == DUN_78_THE_BRIDGE) {
				uniqIndex = 2807 + Difficulty;
			}
			break;
		case UM_753_Sister_Priscilla:
			uniqIndex = 2843 + Difficulty;
			break;
			// <<<  insert next boss block here
		}
	}
	SKIP_CL:
	SpawnUnique( uniqIndex, monster.Row, monster.Col, sendMsg );
}

// {Na-Krul drop was here}. BOSS DROPS routine ( # drops from bosses, # boss drop, # sierra magic drop, # unique drop )
//----- (0043D4E1) --------------------------------------------------------
void __fastcall MonsterKillEffect(int monsterIndex, bool sendMsg)
{
	const Player& player = Players[CurrentPlayerIndex];
	const Monster& monster = Monsters[ monsterIndex ];
	const int monsterDropLevel = MonsterDropLevel(monsterIndex);
	const int bossIndex = monster.newBossId - 1;
	
	// Completly skip drop if end game cinematic requested
	if( monster.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO ){
        if( MaxCountOfPlayersInGame == 1 && Difficulty == DL_0_HORROR ){
            if( !is(GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON) ){
                return; // then skip drops (end cinematics is played, no drop required)
            }
        }
    }
	if (GameMode == GM_CLASSIC && monster.SpritePtr->baseMonsterIndex == BM_C_389_DIABLO) return;// skip drops if Diablo is killed in Classic mode
    
    // No drop from summons
    if( monsterIndex < SummonMonsters_Count ){
        return;
    }
	
	// new unique drop type
	if ( (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS))) && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))) && (!HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) ) {
		SpecifiedUniqueDrop(monsterIndex, sendMsg);
	}

    if( monster.newBossId ){        
        // Special boss drop for non town game modes
        if( (is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) || HasTrait(CurrentPlayerIndex, TraitId::Leper) ){
            if( !is(player.fullClassId, PFC_ASSASSIN, PFC_ROGUE, PFC_BOMBARDIER) 
				&& (!HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) 
				&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
				&& (!(player.gameChanger & BIT(GC_24_RUTHLESS)))){
                SpawnSomethingQlvl( BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, monsterDropLevel, sendMsg );
				if (RNG(10) > 5) { // 40% probability to get an oil
					SpawnSomethingQlvl(BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				}
				if (
					(is(Dungeon, DUN_47_DARK_NEST, DUN_48_BLOOD_HIVE, DUN_39_FROZEN_LOCH, DUN_84_ARMORIES_OF_HELL, DUN_85_DEMON_STRONGHOLD, DUN_87_PANDEMONIUM)
					|| bossIndex == UM_8_Warlord_of_Blood)
					){
					int numberOfIDs = 4 + RNG(3);
					for (int i = 0; i < numberOfIDs; ++i) {
						SpawnSomethingQlvl(BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
					}
				}
				if (Dungeon >= DUN_60_HAUNTED_LABYRINTH) {// +1 ID relict from bosses in AR dungeons
					SpawnSomethingQlvl(BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				}
				// in IM/SU/NM/SR modes, bosses of Hellgate, Hellforge and Andariel Throne drop +2 ID relicts;
				if (Difficulty >= DL_2_DOOM && is(Dungeon, DUN_43_HEPHASTO, DUN_31_ANDARIELS_THRONE, DUN_80_HELL_GATE)) {
					int numberOfIDs = 2;
					for (int i = 0; i < numberOfIDs; ++i) {
						SpawnSomethingQlvl(BI_187_RELICT_OF_IDENTIFY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
					}
				}
            }
			if ( ( !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) ) && ( !(player.gameChanger & BIT(GC_24_RUTHLESS)) ) ) {
				int oil;
				if		(Difficulty == 0 && Dungeon->level <= 2)	oil = MC_31_OIL_OF_REPAIR_I;
				else if (Difficulty == 0 && Dungeon->level <= 13)	oil = MC_32_OIL_OF_REPAIR_II;
				else if (Difficulty == 0 && Dungeon->level <= 24)	oil = RNG(2) == 1 ? MC_33_OIL_OF_REPAIR_III : MC_32_OIL_OF_REPAIR_II;
				else if (Difficulty == 1)							oil = RNG(2) == 1 ? MC_33_OIL_OF_REPAIR_III : MC_34_OIL_OF_REPAIR_IV;
				else if (Difficulty == 2 || Difficulty == 3)		oil = RNG(2) == 1 ? MC_34_OIL_OF_REPAIR_IV : MC_35_OIL_OF_REPAIR_V;
				else if (Difficulty == 4 || Difficulty == 5)		oil = RNG(2) == 1 ? MC_38_OIL_OF_REPAIR_VI : MC_35_OIL_OF_REPAIR_V;
				else												oil = RNG(2) == 1 ? MC_39_OIL_OF_REPAIR_VII : MC_38_OIL_OF_REPAIR_VI;
				if (player.ClassID != PC_2_MAGE || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) { // oils are dropped freely for non-Mage subclasses and for Mamluk
					if (RNG(10) > 2) { // 70% probability to get an oil
						CreateItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, sendMsg);
					}
				}
				else {
					if (RNG(10) > 6) { // Mage classes get them with 30% probability
						CreateItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, sendMsg);
					}
				}
				if (
					(player.ClassID != PC_2_MAGE || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) 
					&& is(Dungeon, DUN_47_DARK_NEST, DUN_48_BLOOD_HIVE, DUN_39_FROZEN_LOCH, DUN_57_ARACHNID_LAIR, DUN_85_DEMON_STRONGHOLD, DUN_87_PANDEMONIUM)
					) {
					int numberOfOils = 4 + RNG(3);
					for (int i = 0; i < numberOfOils; ++i) {
						CreateItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, sendMsg);
					}
				}
				if (Dungeon >= DUN_61_CURSED_SANCTUM) {// +1 oil from bosses in AR dungeons
					CreateItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, sendMsg);
				}
				// in IM/SU/NM/SR modes, bosses of Hellgate, Hellforge and Andariel Throne drop +2 oils 
				if (Difficulty >= DL_2_DOOM && is(Dungeon, DUN_43_HEPHASTO, DUN_31_ANDARIELS_THRONE, DUN_80_HELL_GATE)) {
					int numberOfIDs = 2;
					for (int i = 0; i < numberOfIDs; ++i) {
						CreateItemIgnoringDungeonLevel(monster.Row, monster.Col, 0, IC_0_OTHER, oil, sendMsg);
					}
				}
			}
		}

        // Heavy drinker drop
        if( RNG(100) < PerkValue(PERK_HEAVY_DRINKER, CurrentPlayerIndex) ){
            SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
        }
        
        // Additional gold drop
        // Leper makes you unable to talk to NPCs, gold is of no use to you...
		if (player.gameChanger & BIT(GC_9_NIGHT_KIN) || GameMode == GM_CLASSIC) {
			goto SKIP_BOSS_GOLD_DROP;
		}
		if( !HasTrait(CurrentPlayerIndex, TraitId::Leper) ){
            DropBossGold(monsterIndex);
        }

		SKIP_BOSS_GOLD_DROP:
        // Additional potions drop
        bool drop_potions_allowed = true;
        if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery)) {
            drop_potions_allowed = false;
        }
        if (HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
            drop_potions_allowed = false;
        }
		if (is(GameMode, GM_HARD, GM_CLASSIC)) {
			drop_potions_allowed = false;
		}
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN)) {
			drop_potions_allowed = false;
		}
        
        if (drop_potions_allowed) {
            int magicCode2;
            int magicCode1;
            if (Difficulty >= DL_2_DOOM ){
                magicCode1 = MC_19_POTION_OF_FULL_REJUVENATION;
                magicCode2 = MC_19_POTION_OF_FULL_REJUVENATION;
            }else if (Difficulty == DL_1_PURGATORY) {
                magicCode1 = MC_2_POTION_OF_FULL_HEALING;
                magicCode2 = MC_7_POTION_OF_FULL_MANA;
            }else{
                magicCode1 = MC_3_POTION_OF_HEALING;
                magicCode2 = MC_6_POTION_OF_MANA;
            }
            CreateTypeItem(monster.Row, monster.Col, 0, 0, magicCode1, 0, 1);
            CreateTypeItem(monster.Row, monster.Col, 0, 0, magicCode2, 0, 1);
        }
        
        // Additional item drop for TreasureHunter
        if (HasTrait(CurrentPlayerIndex, TraitId::TreasureHunter)) {
			SpawnItem(monsterIndex, monster.Row, monster.Col, sendMsg);
		}
		// Additional check for +1 item drop from boss with 'Find Item' perk
		if (RNG(100) < PerkValue(PERK_FIND_ITEM, CurrentPlayerIndex)) {
			SpawnItem(monsterIndex, monster.Row, monster.Col, sendMsg);
		}
		//for( int i = 0; i < 20; ++i) SpawnItem(monsterIndex, monster.Row, monster.Col, sendMsg); // boss drop testing
    }else{ // !monster.newBossId
        TryToDropGem( monster.Row, monster.Col );
    }
    
	// Gharbad drop (random boss item)
	if( IsQuestOnLevel(Q_2_GHARBAD_THE_WEAK) && bossIndex == (GameMode == GM_CLASSIC) ? UM_C_849_GHARBAD : UM_0_Gharbad_the_Weak ){
		SpawnItem(monsterIndex, monster.Row, monster.Col, sendMsg);
		return;
	}
	
	auto dropNItems = [&]( const size_t amount )
        {
	        for( size_t i = 0; i < amount; ++i ){
	            SpawnItem( monsterIndex, monster.Row, monster.Col, sendMsg );
            }
        };
    
    switch( bossIndex )
    {
        default: // No boss or boss without special handling
            dropNItems( 1 );
            break;
		case UM_C_902_RAGEHORN:
			dropNItems(0);
			break;
        case UM_11_The_Defiler:
        {
            if( SoundIsPlaying( S_989_M_VAR ) ){
                StopSpeech();
            }
            BASE_ITEM gemIndex;
            if( Difficulty == DL_1_PURGATORY ){
                gemIndex = BI_303_RHODOCHROSITE;
            }
			else if( Difficulty >= DL_2_DOOM ){
                gemIndex = BI_304_ZIRCON;
            }
			else{
                gemIndex = BI_302_FLUORITE;//horror
            }
			SpawnSomethingQlvl(gemIndex, monster.Row, monster.Col, monsterDropLevel, sendMsg);
            dropNItems( 5 );
            break;
        }
		case  UM_790_Ghost_of_Brother_Martin:
		{
			int numberOfItemsToDrop = 4;
			if (CanGrind()) {
				numberOfItemsToDrop = 2;
				for (int i = 0; i < 2; ++i) {
					SpawnSomethingQlvl(RngFrom(1238, 1248, 1258, 1268, 1278, 1288, 1298, 1308, 1318, 1328, 1338, 1348) + Difficulty / 2, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				}
			}
			dropNItems(numberOfItemsToDrop);
			break;
		}
		case  UM_791_Festergnaw:
		{		
			SpawnSomethingQlvl(BI_2510_DUSTY_CRATE, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(2);
			break;
		}
		case UM_759_Bonecleaver:
			{
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				SpawnSomethingQlvl(BI_334_THEODORE, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
			dropNItems(2);
			break;
		case UM_65_Malantor:
			if (MaxCountOfPlayersInGame == 1) {
				SpawnSomethingQlvl(BI_334_THEODORE, monster.Row, monster.Col, monsterDropLevel, sendMsg); 
			}
			dropNItems(1); 
			break;
		case UM_586_Wielder_of_Shadowfang:
			if (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL, GM_SPEEDRUN)) {				
				SpawnSomethingQlvl(BI_399_SHADOWFANG, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
            dropNItems( 4 );
            break;
        case UM_595_The_Cerberus:
			SpawnSomethingQlvl(BI_1204_THE_PROOF, monster.Row, monster.Col, monsterDropLevel, sendMsg);
            dropNItems( 3 );
            break;
		case UM_751_Orzog: 
			{
				for (int i = 0; i < 3; i++) {
					SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				}
			}
			dropNItems(4);
			break;
		case UM_702_Grimjaw:
			SpawnSomethingQlvl(BI_2456_CHEST_KEY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(3);
			break;
		case UM_723_The_Infester: 
			SpawnSomethingQlvl(BI_2472_THE_LOCKED_CHEST, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(3);
			break;
		case UM_706_Mudsnout:
			SpawnSomethingQlvl(BI_2457_IRON_FIGURINE, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(2);
			break;
		case UM_707_Lichen_Axe_the_Fungal:
			SpawnSomethingQlvl(BI_2459_REAGENTS, monster.Row, monster.Col, monsterDropLevel, sendMsg); 
			dropNItems(2); 
			break; 
		case UM_708_Grimwing: 
			SpawnSomethingQlvl(BI_2460_THE_RIGHT_GOODS, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(2);
			break;
		case UM_713_Sir_Valon:
			SpawnSomethingQlvl(BI_2461_MUNDANE_INVENTORY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(2);
			break;
		case UM_714_Sir_Darion:
			if (Dungeon == DUN_107_DREAD_SANCTUARY) {
				SpawnSomethingQlvl(BI_20_SPECTRAL_ELIXIR, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				//SpawnSomethingQlvl(BI_20_SPECTRAL_ELIXIR, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				dropNItems(5);
			}
			else {
				dropNItems(1);
			}
			break;
		case UM_169_Urzul_the_Trampler:
			{
				if (Dungeon == DUN_80_HELL_GATE) {
					dropNItems(5);
				}
				else {
					dropNItems(1);
				}
			}
			break;
		case UM_579_Andariel:
		{
			if (is(GameMode, GM_IRONMAN, GM_SURVIVAL) && RNG(100) < Difficulty * 5 + 60) {// chance by diff: 60, 65, 70, 75, 80, 85, 90, 95, 100
				SpawnSomethingQlvl(BI_20_SPECTRAL_ELIXIR, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
		}
			dropNItems(4);
			break;
		case UM_758_Lord_of_Terror:
		{
			for (int i = 0; i < 3; i++) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
		}
			dropNItems(5);
			break;
		case UM_724_Silkstrider:
		{
			bool no_town = isTownPortalForbidden();
			if (no_town || HasTrait(CurrentPlayerIndex, TraitId::Leper) ) {
				SpawnSomethingQlvl(BI_2476_THE_STRANGE_BOOK, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
			else {
				SpawnSomethingQlvl(BI_2474_TOOL_CHEST, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
		}
			dropNItems(2);
			break;
		case UM_821_Snotspill_MP:
			SpawnSomethingQlvl(BI_327_TAVERN_SIGN, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(3);
			break;
		case UM_725_Web_Queen:
			SpawnSomethingQlvl(BI_2475_CRYSTAL_EYEPIECE, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(5);
			break;
		case UM_754_Madame_Pompadour:
			SpawnSomethingQlvl(BI_16_ANVIL_OF_FURY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(2);
			break;
		case UM_136_Sarogyl_the_Decapitator:
		{
			if (Dungeon == DUN_86_VALLEY_OF_WRATH) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				dropNItems(4);
			}
			else {
				dropNItems(1);
			}
		}
			break;
		case UM_615_Fleshspawn_the_Dead:
		{
			if (Dungeon == DUN_96_BONE_MIRE) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				dropNItems(4);
			}
			else {
				dropNItems(1);
			}
		}
			break; 
		case UM_763_Beelzebub:
		{
			for (int i = 0; i < 4; i++) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
			dropNItems(5);		
		}
			break;
		case UM_765_Duke_of_Abyss:
		{
			for (int i = 0; i < 2; i++) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
			dropNItems(4);
		}
			break;
		case UM_575_Hoofrazor:
		{
			if (Dungeon == DUN_89_BEDLAM) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				dropNItems(2);
			}
			else {
				dropNItems(1);
			}
		}
			break;
		case UM_495_Asmoday:
		{
			if (Dungeon == DUN_87_PANDEMONIUM) {
				SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
				dropNItems(4);
			}
			else { 
				dropNItems(1); 
			}
		}
			break;
		case UM_757_Skullreaper:
		case UM_756_Paladin_Solo:
		{
			SpawnSomethingQlvl(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), monster.Row, monster.Col, monsterDropLevel, sendMsg);
		}
			dropNItems(4);
			break;

		case UM_805_Festerus:
		{
			if (isTownPortalForbidden() || HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
				SpawnSomethingQlvl(BI_2512_FULL_CONTAINERS, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
		}
			dropNItems(4);
			break;

		case UM_9_The_Butcher:
		case UM_158_Akrikta:
        case UM_19_Jaldabaoth:
        case UM_57_Gorash:
        case UM_534_Spiritmurk_the_Black:
        case UM_553_Executioner_of_Death:
        case UM_403_Black_Marquis:
        case UM_242_Breakspine:
        case UM_243_Urdukk:
        case UM_269_Electro_Cute:
		case UM_167_Hazeshifter:
		case UM_675_PWS_Boss:
		case UM_674_Guardian_of_Bones:
		case UM_678_Warden_of_Anger:
		case UM_704_Borje_the_Ogre:
		case UM_740_Don_Carnage: 
		case UM_741_Gloom_Prowler: 
		case UM_744_Dustwing:
		case UM_743_Undertaker:
			dropNItems( 2 );
            break;
        case UM_4_ArchBishop_Lazarus:
        case UM_1_Skeleton_King:
		case UM_323_Lord_of_the_Order:
		case UM_718_Guardian_of_Flames:
		case UM_726_Apparition_of_Arkaine:
		case UM_742_Ironhoof:
		case UM_755_Redbark_Elderroot:
		case UM_809_Borosteron:
			dropNItems( 3 );
            break;
		case UM_556_Pinky:
		case UM_535_Abatan:
		case UM_578_Uber_Xorene:
		case UM_536_Uber_Butcher:
		case UM_537_Ceromorth:
		case UM_557_Dregbez:
		case UM_584_Mordessa:
		case UM_587_Hephasto_the_Armorer:
		case UM_585_Fleshdoom:
		case UM_581_Grimspike:
		case UM_703_Hive_Mother:
		case UM_8_Warlord_of_Blood:
			//case UM_157_Warlord_of_Blood:
		case UM_582_Izual:
		case UM_764_Lord_Nysallor:
		case UM_752_Deathcaller:		
			dropNItems( 4 );
			break;
		case UM_583_Lich_King:
			dropNItems(5);
			break;
		case UM_580_NaKrul:
        case UM_154_Diablo:
		case UM_705_Bloodhoof_Twinaxe:
            dropNItems( 6 );
            break;
			// experimental, for checking drops:
		//case UM_39_Bloodbeak:
		//	dropNItems(120); break;
		case UM_773_The_Dark_Lord:
        case UM_12_Uber_Diablo:
            PlayGlobalSound(S_956_M_DIABLOD); // uber's deathcry sound played
            UberDiabloMonsterIndex = -2;
            if( Difficulty >= DL_2_DOOM ){
                dropNItems( Difficulty / 2 + 6 );
                if( CanGrind() ){
					int gemGrade = by(Difficulty, 2, 2, 3, 3, 4, 4, 5, 5, 6);
					DropRandomGemForSpecificGrade( monster.Row, monster.Col, gemGrade );
                }
            }else{
                dropNItems( 6 );
            }
            break;
    }
}

//----- (0043D655) --------------------------------------------------------
void __fastcall MonsterKilledByOtherPlayer(int monsterIndex, int row, int col, int playerIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( (unsigned int)monsterIndex < Monsters_Max_Count ){
//		if( monster.CurrentLife && monster.CurAction != A_6_DEATH ){
		if (monster.CurAction != A_6_DEATH) {//Do NOT check for monster.CurrentLife != 0 as it may desync.  Experimental change.
			if( !MonsterMap[ row ][ col ] ){
				ClearMonsterOnMap(monsterIndex);
				monster.Row = row;
				monster.Col = col;
				monster.PrevRow = row;
				monster.PrevCol = col;
			}
			MonsterKilledByPlayer(monsterIndex, playerIndex, 0);
		}
	}
}

//----- (0043D6C8) --------------------------------------------------------
int __fastcall MonsterStaying(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < Monsters_Max_Count && monster.SpritePtr ){
		if( monster.SpritePtr->baseMonsterIndex == BM_109_GOLEM ){ 
			monster.curAnimation = &monster.SpritePtr->animation[ANIM_1_WALK].oriented[monster.Orientation];
		}else{
			//SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_0_STAY], monster.Orientation);
			monster.curAnimation = &monster.SpritePtr->animation[ANIM_0_STAY].oriented[monster.Orientation];
		}
		if( monster.CurFrame == monster.animFrameCount ){ // последний кадр анимации
			MonsterLookAround(monsterIndex);
		}
		++monster.TimerCounterOrColDeltaOrSpellDamage;
	}
	return 0;
}

//----- (th2) -------------------------------------------------------------
bool IsSnowStep(int monsterIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	bool snow = false;
	switch(monsterIndex){
	case BM_208_WARPED_ONE or BM_12_IMP or BM_13_DARK_ONE or BM_155_FALLEN_ONE or BM_157_YELLOW_ONE or BM_4_GREMLIN or BM_5_FALLEN_ONE or BM_105_IMP_ROGUE 
		or BM_156_IMP_ROGUE or BM_189_CARVER or BM_196_EVIL_ONE or BM_213_POISONED_ONE: // Fallen (sword & spear)
		snow = player.gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && is(Dungeon->graphType, DT_3_CAVE);
		break;
	case BM_97_DARK_CHARGER:
		snow = player.gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && is(Dungeon->graphType, DT_3_CAVE, DT_4_HELL, DT_6_ABYSS)
			|| Dungeon->isQuest && is(Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH);
		break;
	case BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_124_GLOOM_CLAN or BM_165_APOCALYPSE_CLAN or BM_198_HORN_CLAN or BM_199_HELL_CLAN 
		or BM_224_DEMONOLOG_SUMMON_2 or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN or BM_34_FLESH_CLAN or BM_35_FIRE_CLAN or BM_36_DARKNESS_CLAN or BM_37_STONE_CLAN 
		or BM_173_GHARBAD_BASEMNSTR or BM_175_TERROR_CLAN or BM_186_DEATH_CLAN or BM_187_BLOOD_CLAN or BM_204_SHADOW_CLAN 
		or BM_209_HATRED_CLAN or BM_254_TWILIGHT_CLAN: // Goatmace + Goatbow + Goat_2h_Morgenstern
		snow = player.gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && is(Dungeon->graphType, DT_3_CAVE, DT_4_HELL);
		break;
	case BM_283_FROST_WARDEN or BM_296_ICE_TROLL:
		snow = true;//Dungeon == DUN_38_GLACIAL_CAVERNS || Dungeon == DUN_39_FROZEN_LOCH;
		break;
	default:
		snow = player.gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && is(Dungeon->graphType, DT_3_CAVE, DT_4_HELL, DT_6_ABYSS);
		break;
	}
	return snow;
}

// get non gameplay random int from 0 to range - 1 ------------------------
int RandSound(int range)
{
	return rand() % range;
}

// get non gameplay random int from min to max ----------------------------
int RandSound(int soundMin, int soundMax)
{
	return soundMin + rand() % (soundMax - soundMin + 1);
}

// mor: this function plays monster sounds when they are walking ----------// #monster walk sounds, #monster walking sounds, #monster walk sfx, #monster footstep sounds
void PlayWalkSFX(int monsterIndex)
{
	if( ! MonsterWalkSounds ) return;
	Monster& monster = Monsters[monsterIndex];
	Player& player = Players[CurrentPlayerIndex];
	char steps[28];
	// if monster could walk to player, player should hear his steps 
	auto checkCellFunc = monster.flag & MF_10_OPEN_DOORS ? CheckCellForMonsterCanInteractWithDoorsSafeMove
	                                                                            : CheckCellForMeleeMonsterSafeMove;
	if( FindPath(checkCellFunc, monsterIndex, monster.Row, monster.Col, player.Row, player.Col, steps) ){
		switch( MonsterSprites[monster.SpriteIndex].baseMonsterIndex ){
		case BM_291_MYCONID:
			if (is(monster.WalkframeCounter, 6, 15)) PlayLocalSound(RandSound(1543, 1546), monster.Row, monster.Col); // 1543-1546: light-stone (4)
			break;
		case BM_126_PLAGUE_EATER or BM_221_RAT_MAN or BM_258_RAT_HORDE: // Rats
			if( is(monster.WalkframeCounter, 2,  6) ) PlayLocalSound(                             RandSound(1543, 1546)                        , monster.Row, monster.Col ); // 1543-1546: light-stone (4)
			break;
		case BM_153_DEATH_MINION or BM_154_DEATH_MINION: // cryptic reapers
			if( is(monster.WalkframeCounter, 4,  9) ) PlayLocalSound(                             RandSound(1539, 1542)                        , monster.Row, monster.Col ); // 1539-1542: med - stone (4)
			break;
		case BM_87_DARK_REAPER or BM_136_BLOOD_SLAYER or BM_152_BLACK_REAPER: // other reapers
			if( is(monster.WalkframeCounter, 4,  9) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1547-1550: med - dirt (4) // 1619-1622: med snow run (4)
			break;
		case BM_117_STINGER or BM_143_DOOM_TAIL or 229: // stingers (including summons)
			if( is(monster.WalkframeCounter, 1,  3) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1551-1553: light - dirt (3) // 1611-1613: light snow (3)
			break;
		case BM_163_STINGER_LORD: // stinger lord
			if( is(monster.WalkframeCounter, 3,  8) ) PlayLocalSound(                             RandSound(1554, 1557)                        , monster.Row, monster.Col ); // 1554-1557: light - dirtRun (4)
			break;
		case BM_58_DEFILER: // defiler/nakrul
			if( is(monster.WalkframeCounter, 2,  5) ) PlayLocalSound(                             RandSound(1558, 1565)                        , monster.Row, monster.Col ); // 1558-1565: ginormous (8)
			break;
		case BM_74_GREATER_HULK or BM_75_BLOOD_HULK: // undead hulks
			if( is(monster.WalkframeCounter, 2,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1566-1569: heavy - dirt (4) // heavy snow run-----
			break;
		case BM_59_PIT_FIEND or BM_217_GRIMSPIKE: // pit fiend / grimspike
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1570-1573: heavy - stoneRun (4) // heavy snow run----
			break;
		case BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER: // grave diggers
			if( is(monster.WalkframeCounter, 6, 18) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? 1                     : RandSound(1611, 1614), monster.Row, monster.Col ); // 1611-1614: light snow (4)
			break;
		case BM_46_PIT_SPIDER or BM_112_WAILING_BEAST or BM_134_MUD_RUNNER or BM_205_PINKY or 230: // pit spider + boars
			if( is(monster.WalkframeCounter, 3,  8) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med - dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_97_DARK_CHARGER:
			if( is(monster.WalkframeCounter, 3,  8) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med - dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_120_NECROMORPH: // cryptic flesh thing
			if( is(monster.WalkframeCounter, 8    ) ) PlayLocalSound(                             RandSound(1570, 1573)                        , monster.Row, monster.Col ); // heavy - stoneRun-----
			break;
		case BM_135_GROTESQUE or BM_141_SHAMBLER or BM_142_HELL_MEAT: // other flesh things
			if( is(monster.WalkframeCounter, 8    ) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy - dirt----- // heavy snow run-----
			break;
		case BM_83_LASH_WORM: // lash worm
			if( is(monster.WalkframeCounter, 4, 10) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1554, 1557) : RandSound(1611, 1613), monster.Row, monster.Col ); // light - dirtRun----- // 1611-1613: light snow (3)
			break;
		case BM_115_GRUNT_BEAST or BM_123_BICLOPS: // biclops
			if( is(monster.WalkframeCounter, 2,  7) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1574-1577: med - dirtRun // 1619-1622: med snow run (4)
			break;
		case BM_111_OBSIDIAN_GOLEM or BM_125_GOLD_GOLEM or BM_130_BLACK_GOLEM: // stone golems
			if( is(monster.WalkframeCounter, 6, 15) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy - stoneRun----- // heavy snow run-----
			break;
		case BM_38_UNDEAD_WEREWOLF or BM_47_ACID_HOUND or BM_49_BILE_SPITTER or BM_53_HELL_HOUND or BM_172_SPITTING_TERROR or BM_206_ACID_SPITTER or BM_241_ACID_TERROR: // Hounds
			if( is(monster.WalkframeCounter, 5    ) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1578, 1581) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1578-1581: light-stoneRun (4) // 1611-1613: light snow (3)
			break;
		case BM_80_DEVIL_KIN or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_148_TURBAK_THE_DEUCE or BM_149_THRESHER or BM_207_DREGBEZ: // Big Fallen
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1582-1585: med-stoneRun (4) // 1619-1622: med snow run (4)
			break;
		case BM_95_GHOST_WARRIOR or BM_96_CURSE_BEARER or BM_122_BLOOD_SPECTRE or BM_285_PHANTOM_KNIGHT: // Black Knights
			if( is(monster.WalkframeCounter, 2,  6) ) PlayLocalSound( RandSound(1765, 1772) /*!IsSnowStep(monsterIndex) ? RandSound(1586, 1589) : RandSound(1623, 1626)*/, monster.Row, monster.Col ); // 1586-1589: Heavy Knight (4) // 1623-1626: heavy snow knight (4)
			break;
		case BM_70_FROST_WING or BM_72_FIREWING or BM_73_DEATH_ANGEL or BM_132_BONEWING or BM_167_SKULLWING or BM_210_HELLWING or 228: // Skullwings
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_219_FROST_BONE:// Frost Bone
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound(                             RandSound(1594, 1597)                        , monster.Row, monster.Col ); // med snow run-----
			break;
		case BM_110_DIABLO or BM_137_UBER_DIABLO: // Diablo
			if( is(monster.WalkframeCounter, 2,  5) ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_208_WARPED_ONE or BM_12_IMP or BM_13_DARK_ONE or BM_155_FALLEN_ONE or BM_157_YELLOW_ONE or BM_4_GREMLIN or BM_5_FALLEN_ONE or BM_105_IMP_ROGUE or BM_156_IMP_ROGUE or BM_189_CARVER or BM_196_EVIL_ONE or BM_213_POISONED_ONE: // Fallen (sword & spear)
			if( is(monster.WalkframeCounter, 1,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col );  // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW:
			if (is(monster.WalkframeCounter, 2, 9)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col);  // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_52_BILE_DEMON or BM_55_GRIM_DEMON or BM_56_PIT_DEMON or BM_93_FLAYED_ONE or BM_178_OVERLORD or BM_179_FILTH_DEMON: // Fat
			if( is(monster.WalkframeCounter, 4,  9) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy stone run----- // heavy snow run-----
			break;
		case BM_51_THE_BUTCHER or BM_99_CLEAVER_BOSS or BM_195_UBER_BUTCHER: // FatC
			if( is(monster.WalkframeCounter, 2,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy stone run----- // heavy snow run-----
			break;
		case BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_124_GLOOM_CLAN or BM_165_APOCALYPSE_CLAN or BM_198_HORN_CLAN or BM_199_HELL_CLAN or 224 or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN or BM_34_FLESH_CLAN
		  or BM_35_FIRE_CLAN or BM_36_DARKNESS_CLAN or BM_37_STONE_CLAN or BM_173_GHARBAD_BASEMNSTR or BM_175_TERROR_CLAN or BM_186_DEATH_CLAN or BM_187_BLOOD_CLAN or BM_204_SHADOW_CLAN or BM_209_HATRED_CLAN or BM_254_TWILIGHT_CLAN: // Goatmace + Goatbow + Goat_2h_Morgenstern
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1615, 1617), monster.Row, monster.Col ); // med stone----- // 1615-1617: med snow (3)
			break;
		case BM_214_SATYR_LORD or BM_33_MOON_CLAN or BM_57_HELL_CLAN or BM_166_GOATLORD_BOSS or BM_188_HELL_CLAN or 225: // Goatlord
			if( is(monster.WalkframeCounter, 2,  8) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_109_GOLEM or BM_113_ASH_GOLEM: // Golems
			if( is(monster.WalkframeCounter, 2, 10) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col ); // med dirt Run----- // 1619-1622: med snow run (4)
			break;
		case BM_218_ICE_GOLEM: // Golem Ice
			if( is(monster.WalkframeCounter, 2, 10) ) PlayLocalSound(                             RandSound(1594, 1597)                        , monster.Row, monster.Col ); // 1594-1597: Med Snow Run (4)
			break;
		case BM_60_HELL_STONE or BM_61_BLOOD_STONE or BM_62_MAULER or BM_63_LAVA_LORD or BM_180_CRYPT_STONE: // Magma
			if( is(monster.WalkframeCounter, 1,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1598, 1601) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1598-1601: Heavy I Stone (4) // heavy snow run-----
			break;
		case BM_18_VORTEX_ROGUE or BM_85_BALOR or BM_86_SLAYER or BM_88_BALROG or BM_159_GUARDIAN or BM_176_DESTROYER or BM_193_ABATAN: // Mega
			if( is(monster.WalkframeCounter, 2,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_16_HORNED_BEAST or BM_65_HELL_SPAWN or BM_66_HORNED_DEATH or BM_67_BLACK_HORN or BM_171_MUD_LORD or BM_185_BLOOD_HORN or BM_197_CEROMORTH: // Rhino
			if( is(monster.WalkframeCounter, 3,  7) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_17_SCAVENGER or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER: // Scav
			if( is(monster.WalkframeCounter, 3,  7) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_19_RAVAGER: // Ravager (fast Scav)
			if( is(monster.WalkframeCounter, 1,  3) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_14_SOUL_HUNTER or BM_20_RISEN_SKELETON or BM_21_SKELETON_WARRIOR or BM_22_BURNING_DEAD or BM_23_MALICE or BM_114_FROZEN_HORROR or 226 or 227 or BM_158_DEATH_RETINUE or BM_11_HATRED
		  or BM_9_SKELETON_WARRIOR or BM_10_BURNING_DEAD or BM_7_RISEN_SKELETON or BM_8_BONE_CREEPER or BM_24_RISEN_SKELETON or BM_25_SKELETON_WARRIOR or BM_26_BURNING_DEAD or BM_27_HORROR: // Small Skellies (axes, sword_shield, bow)
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound(                             RandSound(1602, 1605)                        , monster.Row, monster.Col ); // 1602-1605: light o stone (4)
			break;
		case BM_1_OBLIVION_KNIGHT or BM_50_SKELETON_LORD or BM_64_DEATH_GUARDIAN or BM_203_DEATH_KNIGHT: // Leoric
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound(                             RandSound(1590, 1593)                        , monster.Row, monster.Col ); // Med O Stone Run-----
			break;
		case BM_29_HIDDEN or BM_30_SHADOW or BM_31_UNSEEN or BM_32_INVISIBLE_ONE or BM_92_ILLUSION_WEAVER or BM_94_LURKER or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_194_BLOODIED_HIDDEN or BM_257_SHROUDED : // Hidden
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_101_DARK_MISTRESS or BM_102_SUCCUBUS or BM_103_BLOOD_WITCH or BM_104_DARK_WITCH or BM_133_SOUL_BURNER or BM_191_DARK_MISTRESS or BM_202_SUCCUBUS or BM_284_SIN_SPAWN: // Succ
			if( is(monster.WalkframeCounter, 2,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_71_THUNDER_LORD or BM_76_DEATH_CLAW or BM_77_STORM_DEMON or BM_78_EVISCERATOR or BM_79_FEAR_HUNTER or BM_160_CHASM_DEMON or BM_164_LIGHTNING_DEMON: // Thin
			if( is(monster.WalkframeCounter, 3,  7) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_28_BLOOD_WARPER or BM_39_WARPING_DEMON or BM_150_FELLTWIN or BM_151_HYBRID or BM_182_BLINK_DEMON: // Tsneak
			if( is(monster.WalkframeCounter, 1,  8) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_2_ROTTING_CARCASS or BM_140_GHOUL or BM_169_ZOMBIE or BM_0_BURNED_CORPSE or BM_215_CADAVER: // Zombies
			if( is(monster.WalkframeCounter, 9, 21) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(   1,    1) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1611-1613: light snow (3)
			break;
		case BM_232_ABYSSAL_SUCCUBUS: // Abyssal Succubus
			if( is(monster.WalkframeCounter, 4,  9) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_245_BLUNDERER: // Warden
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_233_BLOOD_LORD or BM_234_STEEL_LORD: // Minot
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_236_CORPSE_EATER: // Corpse Eater
			if( is(monster.WalkframeCounter, 1,  7) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_106_CORRUPT_ORDER or BM_146_CORRUPT_ORDER or BM_162_CORRUPT_PALADIN or BM_216_ZEALOT: // Corrupts
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(   0,    3) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1619-1622: med snow run (4)
			break;
		case BM_253_EXPLODER: // Exploder
			if( is(monster.WalkframeCounter, 1    ) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone----- // 1619-1622: med snow run (4)
			break;
		case BM_244_HORADRIM_ANCIENT or BM_246_GREATER_MUMMY: // Mummies
			if( is(monster.WalkframeCounter, 2, 11) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_222_NIGHT_DEMON: // Baboon
			if( is(monster.WalkframeCounter, 1,  6) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case 231 or 247: // Siege
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_243_IZUAL: // Izual
			if(     is(monster.WalkframeCounter, 1) ) PlayLocalSound(                             RandSound(1586, 1589)                        , monster.Row, monster.Col ); // ginormous-----
			else if(is(monster.WalkframeCounter, 5) ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // Heavy Knight-----
			break;
		case BM_252_HEPHASTO:
			// Hephasto // ginormous-----
			if( is(monster.WalkframeCounter, 1,  7) ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col );
			break;
		case BM_251_WENDIGO:
			// Wend // 1606-1609: heavy snow run (4)
			if( is(monster.WalkframeCounter, 1,  7) ) PlayLocalSound(                             RandSound(1606, 1609)                        , monster.Row, monster.Col );
			break;
		case BM_238_TAINTED or BM_239_AFFLICTED: // Misshapen
			if( is(monster.WalkframeCounter, 1,  5) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_240_DEMON_PRIEST: // Demon priest
			if( is(monster.WalkframeCounter, 9    ) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_242_REANIMATED_HORROR: // Reanimated
			if( is(monster.WalkframeCounter, 1    ) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_249_SCARAB or BM_250_BONE_SCARAB: // Scarabs
			if( is(monster.WalkframeCounter, 1,  8) ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_260_ARMAGEDDON_WALKER: // Abi
			if (is(monster.WalkframeCounter, 1, 9)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_261_SKELETAL_KNIGHT:
			if (is(monster.WalkframeCounter, 1, 9)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1586, 1589) : RandSound(1623, 1626), monster.Row, monster.Col); // knight 'boot' sounds
			break;
		case BM_264_BITELING:
			if (is(monster.WalkframeCounter, 11)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_265_CROC or BM_266_CROC_BERS:
			if (is(monster.WalkframeCounter, 5, 13)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2:// try to find heavier footstep sounds. heavy and bony
			if (is(monster.WalkframeCounter, 7, 18)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_271_THORNED_HULK:
			if (is(monster.WalkframeCounter, 5, 11)) PlayLocalSound(RandSound(1760, 1764), monster.Row, monster.Col);
			break;
		case BM_272_SPIDER_QUEEN:
			if (is(monster.WalkframeCounter, 2, 9/*1, 8*/)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col); // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_273_FLAME_FIEND:
			if (is(monster.WalkframeCounter, 4, 9)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1781, 1784) : RandSound(1777, 1780), monster.Row, monster.Col);
			break;
		case BM_274_FLESH_DEVOURER:
			if (is(monster.WalkframeCounter, 7, 15)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_275_BEHEMOTH:
			if (is(monster.WalkframeCounter, 7, 15)) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_276_SWORD_SPIDER or BM_277_SPIDERLING: // stingers (including summons)
			if (is(monster.WalkframeCounter, 2, 5, 11)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col); // 1551-1553: light - dirt (3) // 1611-1613: light snow (3)
			break;
		case BM_278_LEAPER:
			if (is(monster.WalkframeCounter, 1, 9)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col); // med dirt Run----- // 1619-1622: med snow run (4)
			break;
		case BM_283_FROST_WARDEN:
			if (is(monster.WalkframeCounter, 9, 19)) PlayLocalSound(/*!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : */ RandSound(1832, 1835), monster.Row, monster.Col); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_286_BRINE_CORPSE:
			if (is(monster.WalkframeCounter, 4, 14)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_287_ARMORED_SKELETON:
			if (is(monster.WalkframeCounter, 1, 10)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_288_EARTH_GOLEM:
			if (is(monster.WalkframeCounter, 7, 17)) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS:
			if (is(monster.WalkframeCounter, 7, 15)) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_292_BUGBEAR:
			if (is(monster.WalkframeCounter, 1, 9)) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_297_OROG:
			if (is(monster.WalkframeCounter, 4, 13)) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_293_PIT_FIEND:
			if (is(monster.WalkframeCounter, 3, 8)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_296_ICE_TROLL:
			if (is(monster.WalkframeCounter, 1, 11)) PlayLocalSound(RandSound(1026, 1029), monster.Row, monster.Col); // med sand
			if (is(monster.WalkframeCounter, 3, 13)) PlayLocalSound(RandSound(1839, 1842), monster.Row, monster.Col); // lght sand
			break;
		case BM_298_TENTACLIST_BOSS or BM_299_TENTACLIST2 or BM_300_TENTACLIST_ABYSS :
			if (is(monster.WalkframeCounter, 1, 7)) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1615, 1617), monster.Row, monster.Col); // med stone----- // 1615-1617: med snow (3)
			break;

		}
	}
}

//----- (0043D720) --------------------------------------------------------
int __fastcall MonsterWalkUp(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int result = 0; // CurAction != A_15_STONE_CURSED
	if( monsterIndex < Monsters_Max_Count && monster.SpritePtr ){
		MonsterSprite& sprite = *monster.SpritePtr;
		if( monster.WalkframeCounter == sprite.animation[ANIM_1_WALK].frameCount ){ // завершение движения
			MonsterMap[ monster.Row ][ monster.Col ] = 0;
			monster.Row += monster.PrevActionOrRowDeltaOrSpellEffect;
			monster.Col += monster.TimerCounterOrColDeltaOrSpellDamage;
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
			if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
			}
			FixMonsterPosition(monsterIndex, monster.Orientation);
			result = 1;
		}else{                                                                     // движение между клетками
			if( !monster.AnimationDelayIndex ){				
				PlayWalkSFX(monsterIndex);
				++monster.WalkframeCounter;
				monster.WalkX += monster.SpeedX;
				monster.WalkY += monster.SpeedY;
				monster.DeltaX = monster.WalkX >> 4;
				monster.DeltaY = monster.WalkY >> 4;
			}
		}
		if( !((char)monster.flag & MF_1_INVISIBLE) && Monsters[monsterIndex].LightIndex ){
			OffsetMonsterLight(monsterIndex);
		}
	}
	return result;
}

//----- (0043D84D) --------------------------------------------------------
void __fastcall OffsetMonsterLight(uint monsterIndex)
{
	if (monsterIndex >= Monsters_Max_Count) return;

	const Monster& monster = Monsters[monsterIndex];
	int deltaX = monster.DeltaX;
	int deltaY = monster.DeltaY * 2;
	int deltaSum = deltaX + deltaY;
	int deltaSub = deltaY - deltaX;
	int row = 1;
	int col = 1;

	if (deltaSum < 0) {
		row = -1;
		deltaSum = -deltaSum;
	}

	if (deltaSub < 0) {
		col = -1;
		deltaSub = -deltaSub;
	}

	if (monster.LightIndex) {
		ChangeLightOffset(monster.LightIndex, row * (deltaSum >> 3), col * (deltaSub >> 3));
	}
}

//----- (0043D8AC) --------------------------------------------------------
int __fastcall MonsterWalkDown( uint monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monsterIndex >= Monsters_Max_Count || !monster.SpritePtr ){
		return 0;
	}
	int walkFrameCounter = monster.WalkframeCounter;

	int moreAction;
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// Если это окончание движения
		MonsterMap[ monster.RowDelta ][ monster.ColDelta ] = 0;
		if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
			ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		moreAction = 1;
	}else{
		if( !monster.AnimationDelayIndex ){
			PlayWalkSFX(monsterIndex);
			monster.WalkframeCounter++;
			monster.WalkX += monster.SpeedX;
			monster.WalkY += monster.SpeedY;
			monster.DeltaX = monster.WalkX >> 4;
			monster.DeltaY = monster.WalkY >> 4;
		}
		moreAction = 0;
	}
	if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
		OffsetMonsterLight(monsterIndex);
	}
	return moreAction;
}

//----- (0043D9B6) --------------------------------------------------------
int __fastcall MonsterWalkHorizontal( uint monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monsterIndex >= Monsters_Max_Count || !monster.SpritePtr ){
		return 0;
	}
	int moreAction;
	int walkFrameCounter = monster.WalkframeCounter;
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// Если это окончание движения
		MonsterMap[ monster.Row ][ monster.Col ] = 0;
		monster.Row = monster.RowDelta;
		monster.Col = monster.ColDelta;
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER_HORIZONTAL;
		MonsterMap[ monster.RowDelta ][ monster.ColDelta ] = monsterIndex + 1;
		if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
			ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		moreAction = 1;
	}else{
		if( !monster.AnimationDelayIndex ){
			PlayWalkSFX(monsterIndex);
			monster.WalkframeCounter++;
			monster.WalkX += monster.SpeedX;
			monster.WalkY += monster.SpeedY;
			monster.DeltaX = monster.WalkX >> 4;
			monster.DeltaY = monster.WalkY >> 4;
		}
		moreAction = 0;
	}
	// тут был босс айди, но я уверен что это лайт радиус. Везде в других местах лайт радиус проверяется а не босс айди
	if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){// (7) расширение номера босса 0043DAD5
		OffsetMonsterLight(monsterIndex);
	}
	return moreAction;
}

//----- (th2) -------------------------------------------------------------
bool Necropathy(int attackerIndex, int defenderIndex)
{
	if (attackerIndex < SummonMonsters_Count) {
		int playerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
		if( HasTrait(playerIndex, TraitId::Necropathy) ){
			Player& player = Players[playerIndex];
			Monster& defender = Monsters[defenderIndex];
			if( abs(player.Row - defender.Row) <= player.LightRadius
			 && abs(player.Col - defender.Col) <= player.LightRadius
			 && FlagMap[defender.Row][defender.Col] & CF_2_VISIBLE_BY_PLAYER ){
				return true;
			}
		}
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
bool MinionCrits(int attackerIndex, int defenderIndex)
{
	if (attackerIndex < SummonMonsters_Count) {
		int playerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
		Player& player = Players[playerIndex];
		Monster& defender = Monsters[defenderIndex];
		if (RNG(100) < PerkValue(PERK_MINION_CRITS, playerIndex)) {
			return true;
		}
	}
	return false;
}

bool IsMasterHasTrait(int monsterIndex, TraitId traitId)
{
	if (monsterIndex < SummonMonsters_Count) {
		const int ownerIndex = monsterIndex / SummonMonstersPerPlayer_Count;
		return HasTrait(ownerIndex, traitId);
	}
	return false;
}

//----- (0043DAF4) --------------------------------------------------------
void __fastcall MvM_Melee( int attackerIndex, int defenderIndex, int hitChance, int minDamage, int maxDamage )
{
	if( (uint) defenderIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& defender = Monsters[defenderIndex];
	if( !defender.SpritePtr ){
		return;
	}
	if( (defender.CurrentLife & (-64)) <= 0 ){
		return;
	}
	if( defender.SpritePtr->baseMonsterIndex == BM_92_ILLUSION_WEAVER 
		&& defender.goal == MG_2_RETREAT ){
			return;
	}
	
	Monster& attacker = Monsters[attackerIndex];
	
	if( attacker.flag & MF_6_FRIENDLY && !defender.ActivationCounter ){
		defender.ActivationCounter = 255;
		defender.LastTargetPositionRow = attacker.Row;
		defender.LastTargetPositionCol = attacker.Col;
		MonsterChainActivation( defenderIndex );
	}
	
	// activate zombie on his hit
	if( defender.flag & MF_6_FRIENDLY && !attacker.ActivationCounter ){
		attacker.ActivationCounter = 255;
		attacker.LastTargetPositionRow = attacker.Row;
		attacker.LastTargetPositionCol = attacker.Col;
		MonsterChainActivation( attackerIndex );
	}
	
	// 0043DB47
	int missChance = RNG(100);
	if (defender.CurAction == A_15_STONE) {
		bool canHitStoneCursed = false;
		if (attackerIndex < SummonMonsters_Count) {
			const int ownerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
			if (HasTrait(ownerIndex, TraitId::BreakerOfStones) || GameMode == GM_CLASSIC) {
				canHitStoneCursed = true;
			}
		}
		if (!canHitStoneCursed) {
			missChance = 100;
		}
	}
	/*if( defender.CurAction == A_15_STONE ){ // old SC check
		missChance = 100;
	}*/
	int defender_monster_AC = defenderIndex < SummonMonsters_Count ? defender.ArmorClass : (defender.ArmorClass / 2);
	hitChance = 35 + hitChance - defender_monster_AC /*/ 2*/; //mor: was / 2, trying new balance solution with no armor reduction (because there was negative feed with ac doing nothing later
	int autohit = 20;
	int automiss = 80;
	if(IsMasterHasTrait(defenderIndex, TraitId::Cannibalism)) {
			autohit += 9;
			automiss += 9;
	}
	LimitToRange( hitChance, autohit, automiss );
	if( missChance >= hitChance ){
	    return;
	}
	int isMonsterHit;
	if( CheckMonsterPhaseOrBlock(defenderIndex, &isMonsterHit) ){
		return;
	}
	int damage = RngFromRange(minDamage, maxDamage) << 6;

	if (MinionCrits(attackerIndex, defenderIndex)) {
		damage *= 2;
	}
	if( Necropathy(attackerIndex, defenderIndex) ){
		damage += damage / 5;
	}

	if (defender.CurAction == A_15_STONE) {
		damage /= 2;
	}

	if( defender.newBossId ){
		damage /= 2;
	}
	if (defender.newBossId - 1  == UM_12_Uber_Diablo || defender.newBossId - 1 == UM_773_The_Dark_Lord) {
		damage = 2 * damage / 5; 
	} 

	if( has(defender.affixes, MAF_ARMORED) ){ //33% melee damage reduce from monster affix
		damage -= damage / 3;
	}
	if( attackerIndex != -1 && Monsters[attackerIndex].flag & MF_6_FRIENDLY ){
	    if( MaxCountOfPlayersInGame != 1 ){
            if(       NetPlayerCount == 2 ){ damage -= damage / 4; // -25% damage
            }else if( NetPlayerCount == 3 ){ damage -= 2*damage/5; // -40% damage
            }else if( NetPlayerCount == 4 ){ damage -= damage / 2; // - 50% damage
            }
        }
    }		

	if (attackerIndex < SummonMonsters_Count) {
		const int ownerIndex = attackerIndex / SummonMonstersPerPlayer_Count;
		if (HasTrait(ownerIndex, TraitId::Cannibalism)) {
			const int healedPoints = damage / 5; // 20%
			attacker.CurrentLife += healedPoints;
			if (attacker.CurrentLife > attacker.BaseLife) {
				attacker.CurrentLife = attacker.BaseLife;
			}
		}
	}

	defender.CurrentLife -= damage;
	if( (defender.CurrentLife & ~63) <= 0 ){
        KillMonsterByMonster(attackerIndex, defenderIndex);
        if( attackerIndex < SummonMonsters_Count ){
            DrawFloatingDamage( damage >> 6, defender.Row, defender.Col, defenderIndex );
        }
	}else{
        if( defenderIndex < SummonMonsters_Count ){
            DrawFloatingDamage( damage >> 6, defender.Row, defender.Col, defenderIndex, C_2_Red );
        }else{
            DrawFloatingDamage( damage >> 6, defender.Row, defender.Col, defenderIndex );
        }
    
        DamageMonsterByMonster(defenderIndex, attackerIndex, damage);
    }
}

//----- (0043DBDE) --------------------------------------------------------
void __fastcall DamageMonsterByMonster(int monsterIndex, int casterIndex, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite* monsterSprite = monster.SpritePtr;

	if (monster.CurAction == A_6_DEATH) {//Duped drops fix for MP, additional safety check
		return;
	}

	if( monsterIndex < Monsters_Max_Count ){
		if( monsterSprite ){
			delta_monster_hp(monsterIndex, monster.CurrentLife, (DUNGEON)Dungeon);
			SendCmdDamageMonster(0, monsterIndex, damage);
			PlayMonsterSound(monsterIndex, MST_1_GOTHIT);
			if( /*is ( monsterSprite->baseMonsterIndex, BM_29_HIDDEN, BM_30_SHADOW, BM_31_UNSEEN, BM_32_NIGHTMARE) // hidden, always stunned
				// MvM stun formula: (if) damage >= mAC / 2 + MLVL + Rnd [ DLVL/4 + 6*DIFF + 2 ]
				||*/ (damage >> 6) >= monster.ArmorClass / 2 + monster.MonsterLevel + RNG(Dungeon->level / 4 + 6 * Difficulty + 2) ){ 
				if( casterIndex >= 0 ){ // not M1_TRAP
					monster.Orientation = ((uchar) Monsters[ casterIndex ].Orientation - 4) & 7;
				}
				/*if( monsterSprite->baseMonsterIndex == BM_39_WARPING_DEMON ){ // these monsters warped to master when attacked by summons
					MonsterJumpAroundTarget( monsterIndex_1 );
				}else*/ 
				if( monster.ai == AI_4_Scavenger ){
					monster.goal = MG_1_ATTACK;
					monster.goalParam = 0;
					monster.curOrientation = DIR_0_D;
				}
				if( monster.CurAction != A_15_STONE){
					if( monsterSprite->baseMonsterIndex != BM_109_GOLEM ){ // Do NOT modify Golem position otherwise it may disappear since it doesn't have idle animation.
						SetMonsterAnimation(monsterIndex, &monsterSprite->animation[ ANIM_3_HIT ], monster.Orientation );
						monster.CurAction = A_5_GOT_HIT;
						monster.DeltaX = 0;
						monster.DeltaY = 0;
						monster.Row = monster.PrevRow;
						monster.Col = monster.PrevCol;
						monster.NextRow = monster.Row;
						monster.NextCol = monster.Col;
						monster.PrevRow = monster.Row;
						monster.PrevCol = monster.Col;
						CheckMonsterUnderArch(monsterIndex);
						ClearMonsterOnMap(monsterIndex);
						MonsterMap[0][monster.Col + 112 * monster.Row] = monsterIndex + 1;
					}
				}
			}
		}
	}
}

//----- (0043DD8A) --------------------------------------------------------
void __fastcall KillMonsterByMonster(int attackerIndex, int targetIndex)
{
	Monster& target = Monsters[ targetIndex ];

	if( attackerIndex != M1_TRAP && (attackerIndex < 0 || attackerIndex >= Monsters_Max_Count)
 	 || targetIndex >= Monsters_Max_Count
	 || target.CurAction == A_6_DEATH ){ //Duped drops fix for MP
	    return;
    }
	delta_kill_monster(targetIndex, target.Row, target.Col, (DUNGEON)Dungeon);

	NetSendCmdLocParam2(0, CMD_36_MONSTDEATH, target.Row, target.Col, targetIndex, target.dungeon);

    if( attackerIndex != M1_TRAP && attackerIndex < SummonMonsters_Count && !target.IsSummonedByMonster){
        target.Attackers |= 1 << (attackerIndex / SummonMonstersPerPlayer_Count );
        AddXpForMonsterKill( target.MonsterLevel, target.Xp, target.Attackers, targetIndex );
    }
    ++TotalSlain[target.SpritePtr->baseMonsterIndex];
    target.CurrentLife = 0;
	if (!target.IsSummonedByMonster) {
		int seed = target.RandomSeedForDrop;
		// if( MaxCountOfPlayersInGame == 1) // MP smart drop
		{
			seed ^= Players[CurrentPlayerIndex].magicFind; // Randomize base item depending on mf value
		}
		SetRndSeed(seed);
		MonsterKillEffect(targetIndex, 1);
	}
    if( (MaxCountOfPlayersInGame == 1 && target.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO && (unsigned int) Difficulty < 1
     && !is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON ))
		|| GameMode == GM_CLASSIC && target.SpritePtr->baseMonsterIndex == BM_C_389_DIABLO){
        KillDiabloEffect( targetIndex, 1 );
    }else{
        PlayMonsterSound( targetIndex, MST_2_DEATH );
    }
    int orientation = target.Orientation;
    if( attackerIndex != M1_TRAP ){
        orientation = (Monsters[ attackerIndex ].Orientation - 4) & 7;
    }
    if( target.SpritePtr->baseMonsterIndex == BM_109_GOLEM ){
        orientation = 0;
    }
    target.Orientation = orientation;
    SetMonsterAnimation( targetIndex, &target.SpritePtr->animation[ANIM_4_DIE], orientation );
    target.CurAction = A_6_DEATH;
    target.DeltaX = 0;
    target.DeltaY = 0;
    target.Row = target.PrevRow;
    target.Col = target.PrevCol;
    target.NextRow = target.Row;
    target.NextCol = target.Col;
    target.PrevRow = target.Row;
    target.PrevCol = target.Col;
    CheckMonsterUnderArch( targetIndex );
    ClearMonsterOnMap( targetIndex );
    MonsterMap[ target.Row ][ target.Col ] = targetIndex + 1;
    SayMessageOnBossKill( targetIndex, 1 );
    BewareNearMonsters( target.Row, target.Col );
    int baseMonsterIndex = target.SpritePtr->baseMonsterIndex;
    if( baseMonsterIndex >= BM_46_PIT_SPIDER && baseMonsterIndex <= BM_49_BILE_SPITTER ){
        CastMissile( target.Row, target.Col, 0, 0, 0, MI_59_ACID_POOLS, 1, targetIndex, target.intel + 1, 0, 0 );
    }
}

//----- (0043DF9E) --------------------------------------------------------
int __fastcall MonsterMeleeAttack(int monsterIndex) // special attack features for monsters in melee vs player.. (#monster dual attack support, #double attacks, #dual attacks)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count || ! monster.SpritePtr ){ 
		return false; 
	}
	if( monster.CurFrame == monster.BasePtr->FirstAtackToHitFrame && monster.TargetObjectIndex != -1 && Objects[monster.TargetObjectIndex].selectable ){
		InteractWithBarrel(CurrentPlayerIndex, monster.TargetObjectIndex, 1, 0, 1);
		monster.TargetObjectIndex = -1;
		PlayMonsterSound( monsterIndex, MST_0_ATTACK );
	}else{
		if( monster.CurFrame == monster.BasePtr->FirstAtackToHitFrame ){
			MvP_Melee(monsterIndex, monster.TargetIndex, monster.ToHit, monster.MinDamage, monster.MaxDamage);
			if( monster.newBossId - 1 == UM_582_Izual && abs( monster.Row - monster.TargetRow ) < 2 && abs( monster.Col - monster.TargetCol) < 2 ){
				int minDamage = 72 + Difficulty * 76;
				int maxDamage = 110 + Difficulty * 77;
				if(is(GameMode, GM_EASY, GM_IRONMAN, GM_SURVIVAL)){ // -20% melee damage on IM for Izual
					minDamage -= minDamage / 5;
					maxDamage -= maxDamage / 5;
				}				
				CastMissile( monster.TargetRow, monster.TargetCol, 0, 0, -1, MI_8_LIGHTING_SEGMENT, 1, monsterIndex, RngFromRange(minDamage << 6, maxDamage << 6), 0, 0 );
			}
			/*int bmi = monster.SpritePtr->baseMonsterIndex;
			if (is(bmi, BM_71_THUNDER_LORD, BM_76_DEATH_CLAW, BM_77_STORM_DEMON, BM_78_EVISCERATOR, BM_79_FEAR_HUNTER, BM_160_CHASM_DEMON, BM_164_LIGHTNING_DEMON)  
				&& abs(monster.Row - monster.TargetRow) < 2 && abs(monster.Col - monster.TargetCol) < 2) {
				int minDamage = Dungeon->level + Difficulty * 20;
				int maxDamage = 2 * Dungeon->level + Difficulty * 40;
				CastMissile(monster.TargetRow, monster.TargetCol, 0, 0, -1, MI_23_MONSTER_LIGHTING_SEGMENT, 1, monsterIndex, RandFromRange(minDamage << 6, maxDamage << 6), 0, 0);
			}*/
			if( monster.ai != AI_24_Viper ){
				PlayMonsterSound(monsterIndex, MST_0_ATTACK);
			}
		}
		auto melee = [&](int frame, int toHit, int minDamage, int maxDamage){
			if (monster.CurFrame == frame && monster.TargetObjectIndex != -1 && Objects[monster.TargetObjectIndex].selectable) {
				InteractWithBarrel(CurrentPlayerIndex, monster.TargetObjectIndex, 1, 0, 1);
				monster.TargetObjectIndex = -1;
				PlayMonsterSound(monsterIndex, MST_0_ATTACK);
			}
			else if (frame == monster.CurFrame) {
				MvP_Melee(monsterIndex, monster.TargetIndex, toHit, minDamage, maxDamage);
				PlayMonsterSound(monsterIndex, MST_0_ATTACK);
			}
		};
		int hit = monster.ToHit, min = monster.MinDamage, max = monster.MaxDamage;
		switch( monster.SpritePtr->baseMonsterIndex ){ // extra attacks and/or special/substitute attacks
		case BM_64_DEATH_GUARDIAN:	if( Dungeon == DUN_16_HELL_4 )  melee( 9, hit + 37, min, 2 * max ); break; //  albrecht? 
		case BM_66_HORNED_DEATH:	
			if (Dungeon->level == 22) { 
				melee(8, hit - 25, min - 20, max); 
			} 
			// third attack
			if (Dungeon->isQuest && Dungeon == DUN_26_BONE_CHAMBER) {
				melee(9, hit - 10, 2 * min / 3, 3 * max / 4);
			}
			else {
				melee(9, hit + 10, min - 2, max - 2);
			}
			break;
		case BM_85_BALOR:						   melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_159_GUARDIAN: if( RNG(2) == 1 )     melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_166_GOATLORD_BOSS:                          melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_176_DESTROYER: if( RNG(2) == 1 )     melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_243_IZUAL: if (GameMode == GM_HARD)	   melee(10, hit + 15, min + 2, max + 4);  break;
		case BM_162_CORRUPT_PALADIN:                          melee(10, hit + 37, min, 5 * max / 3 ); break; // corrupt paladin
		case BM_163_STINGER_LORD:						   melee( 8, hit + 15, min + 9, max + 12); break; // stinger lord
		case BM_180_CRYPT_STONE:                          melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_188_HELL_CLAN:						   melee( 9, hit + 20, min, 4 * max / 3 ); break; // dabbuz
		case BM_193_ABATAN:						   melee( 9, hit + 37, min, 2 * max ); break; // abatan
		case BM_197_CEROMORTH:                          melee( 7, hit + 37, min, 2 * max ); break; // ceromorth
		case BM_203_DEATH_KNIGHT or BM_33_MOON_CLAN or BM_50_SKELETON_LORD or BM_122_BLOOD_SPECTRE:       
											melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_57_HELL_CLAN or BM_59_PIT_FIEND or BM_60_HELL_STONE or BM_61_BLOOD_STONE or BM_62_MAULER or BM_63_LAVA_LORD
			or BM_C_365_MAGMA_DEMON or BM_C_368_LAVA_LORD or BM_C_367_HELL_STONE or BM_C_366_BLOOD_STONE:
											melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_58_DEFILER or BM_137_UBER_DIABLO:                    
											melee( 9, hit + 10, min, max ); // third attack
											melee(13, hit + 5, min, max ); break;
		case BM_205_PINKY:                   melee( 8, hit + 10, 2 * (min - 2), 2 * max ); break; // pinky
		case BM_207_DREGBEZ:                 melee( 5, hit + 10, 3 * (min - 2) / 2, 3 * max / 2 ); break; // dregbez
		case BM_214_SATYR_LORD:              melee( 9, hit + 10, min - 2, max - 2 ); break;
		case BM_217_GRIMSPIKE:               melee( 8, hit + 30, min - 1, 3 * max ); break;
		case BM_70_FROST_WING or BM_71_THUNDER_LORD or BM_72_FIREWING or BM_73_DEATH_ANGEL or BM_74_GREATER_HULK
		  or BM_75_BLOOD_HULK or BM_76_DEATH_CLAW or BM_77_STORM_DEMON or BM_78_EVISCERATOR or BM_79_FEAR_HUNTER
			or BM_C_361_RED_STORM or BM_C_362_STORM_RIDER or BM_C_363_STORM_LORD or BM_C_364_MAELSTORM:   
											melee(13, hit, min, max ); break;
		case BM_87_DARK_REAPER:                           melee(13, hit - 9, min - 10, max - 10 ); break;
		case BM_120_NECROMORPH:						   melee(13, hit, min, max ); break;
		case BM_286_BRINE_CORPSE:			 melee(9, hit-10, min/2, max/2); break;
		case BM_110_DIABLO:						   melee(13, hit + 12, 4 * min / 3, 4 * max / 3); break; // diablo's 2nd attack a little more precise and deadly
		case BM_136_BLOOD_SLAYER:                          melee(13, hit - 9, min - 10, max - 10 ); break;
		case BM_132_BONEWING:		                   melee(13, hit, min, max ); break;
		case BM_125_GOLD_GOLEM: if( Dungeon->level == 24 ) melee(12, hit + 10, min - 25, max - 20 ); break;
		case BM_152_BLACK_REAPER or BM_153_DEATH_MINION or BM_154_DEATH_MINION:            melee(13, hit - 9, min - 10, max - 10 ); break;
		case BM_160_CHASM_DEMON or BM_167_SKULLWING:                   melee(13, hit, min, max ); break;
		case BM_195_UBER_BUTCHER:                          melee( 7, hit, min, max ); break;
		case BM_210_HELLWING or BM_211_TORCHANT or BM_212_CACODEMON or BM_219_FROST_BONE:     melee(14, hit, min, max ); break;
		case BM_233_BLOOD_LORD or BM_234_STEEL_LORD:                   melee(15, hit, min, max); break;
		case BM_236_CORPSE_EATER:		                   melee(10, hit + 15, min + 2, max + 5); break; // corpse eater
		case BM_227_NECROMANT_SUMMON_2:		melee(10, hit - 10, 4 * min / 3, 4 * max / 3); break;
		case BM_222_NIGHT_DEMON:			melee(7, hit - 15, min / 3, max / 3); break;
		case BM_240_DEMON_PRIEST:			melee(6, hit + 30, min / 4, max / 3); break;
		case BM_245_BLUNDERER:				melee(8, hit - 2, min * 2, max * 2); break;
		case BM_246_GREATER_MUMMY: if (Dungeon->level >= 11)  melee(14, hit - 15, 2 * min / 3, 3 * max / 4); break;
		case BM_247_SIEGEBREAKER_BEAST or BM_231_BEASTMASTER_SUMMON_3:	melee( 8, hit, 2 * min, 7 * max / 3); break;
		case BM_228_NECROMANT_SUMMON_3:		melee(14, hit, 3 * min / 2, 3 * max / 2); break;
		case BM_225_DEMONOLOG_SUMMON_3:			melee( 9, hit, min, max); break;
		case BM_254_TWILIGHT_CLAN:			melee(12, hit, 2*min, 2*max); break;
		case BM_265_CROC or BM_266_CROC_BERS:
		case BM_260_ARMAGEDDON_WALKER:		melee(8, hit, 7 * min / 4, 7 * max / 4); break;
		case BM_261_SKELETAL_KNIGHT:		melee(9, hit, 7 * min / 4, 7 * max / 4); break;// hits on frame 9, standard accuracy, doubled min and max damage
		case BM_267_BONE_BLADE:				melee(6, hit + 10, 5 * min / 4, 5 * max / 4); break;
		case BM_268_BONE_BLADE_2:			melee(9, hit + 10, 11 * min / 5, 11 * max / 5); break;
		case BM_271_THORNED_HULK:			melee(6, hit + 10, 11 * min / 5, 11 * max / 5); break;
		case BM_274_FLESH_DEVOURER:			melee(10, hit + 20, 6 * min / 5, 7 * max / 5); break;
		case BM_275_BEHEMOTH:				melee(8, hit + 25, 12 * min / 5, 13 * max / 5); break;
		case BM_283_FROST_WARDEN:			melee(10, hit - 5, min, max + 2); break;
		case BM_294_BELHIFET_UNO:			melee(9, hit, 2*min, 2*max); break;
		case BM_295_BELHIFET_DOS:			melee(8, hit, min, max); break;
		}
	}
	if( monster.ai == AI_24_Viper && monster.CurFrame == 1 ){ PlayMonsterSound( monsterIndex, MST_0_ATTACK ); }

	// extra elemental attacks for monsters (by base monster index)
	int bmi = monster.SpritePtr->baseMonsterIndex;
	if (is(bmi, BM_71_THUNDER_LORD, BM_76_DEATH_CLAW, BM_77_STORM_DEMON, BM_78_EVISCERATOR, BM_79_FEAR_HUNTER, BM_160_CHASM_DEMON, BM_164_LIGHTNING_DEMON)
		&& abs(monster.Row - monster.TargetRow) < 2 && abs(monster.Col - monster.TargetCol) < 2
		&& is(monster.CurFrame, 4, 13) ) {
		int el_damaggio = 1 + (Dungeon->level / 8) + Difficulty * 3;
		if (MaxCountOfPlayersInGame == 1) { 
			el_damaggio += RNG(2 + Difficulty * 2); 
		}
		CastMissile(monster.TargetRow, monster.TargetCol, 0, 0, -1, MI_23_MONSTER_LIGHTING_SEGMENT, 1, monsterIndex, el_damaggio, 0, 0);
	}
	/*else if (is(bmi, BM_238_TAINTED, BM_239_AFFLICTED )
		&& abs(monster.Row - monster.TargetRow) < 2 && abs(monster.Col - monster.TargetCol) < 2
		&& monster.CurFrame == 4 ) {
		int el_damaggio = 1 + (Dungeon->level / 6) + Difficulty * 4;
		CastMissile(monster.TargetRow, monster.TargetCol, 0, 0, -1, MI_130_SNOW_EXPL, 1, monsterIndex, el_damaggio, 0, 0);
	}*/

	if( monster.CurFrame == monster.animFrameCount ){
		FixMonsterPosition( monsterIndex, monster.Orientation );
		return 1;
	}
	return 0;
}

//int nearbyAttackersCount = 0;
//----- (th3) -------------------------------------------------------------
void AmplifyByNearAttackers(int playerIndex, int& autoHit)
{
	Player& player = Players[playerIndex];
	int nearMonsters[8] = {};
	int nearbyAttackersCount = 0;
	bool nearBossOrChampion = false;
	for( int i = 0; i < 8; ++i ){
		int row = player.Row + RowDelta[i];
		int col = player.Col + ColDelta[i];
		if( int monsterNumber = MonsterMap[row][col] ){
			int monsterIndex = abs(monsterNumber) - 1;
			if( !has(nearbyAttackersCount, nearMonsters, monsterIndex) ){
				Monster& monster = Monsters[monsterIndex];
				if( !(monster.flag & MF_5_TARGET_MONSTER)
				 && monster.TargetIndex == playerIndex
				 && is(monster.goal, MG_1_ATTACK, MG_3_EAT, MG_5_ATTACK2) ){
					nearBossOrChampion |= monster.newBossId || monster.ChampionNumber;
					nearMonsters[nearbyAttackersCount++] = monsterIndex;
				}
			}
		}
	}
	if( ! nearBossOrChampion ){
		int fca_adjuster_1 = 0, fca_adjuster_2 = 0;
		if		(GameMode == GM_HARD)	{ fca_adjuster_1 = 1500; fca_adjuster_2 = 375; }// Hard:		+150% for 1,	+27.5% for 2
		else if (GameMode == GM_EASY)	{ fca_adjuster_1 =  500; fca_adjuster_2 = 125; }// Easy:		+50% for 1,		+22.5% for 2
		else if (GameMode == GM_NORMAL)	{ fca_adjuster_1 = 1000; fca_adjuster_2 = 250; }// Normal:		+100% for 1,	+25% for 2
		else if (GameMode == GM_CLASSIC){ fca_adjuster_1 =	  0; fca_adjuster_2 =   0; }// Classic:		+0% for 1,		+0% for 2
		else							{ fca_adjuster_1 =  250; fca_adjuster_2 =  62; }// other:		+25% for 1,		+6% for 2
		
		switch( nearbyAttackersCount ){
		case 1: autoHit += fca_adjuster_1 * autoHit / 1000; break; // +?%
		case 2: autoHit += fca_adjuster_2 * autoHit / 1000; break; // +?%
		}
	}
}

void MvP_Melee_Max_Block_Chance(int playerIndex, int& maxBlockChance) {

	Player& player = Players[playerIndex];
	
	switch (player.fullClassId) {
	case PFC_WARRIOR:		
		if (HasTrait(playerIndex, TraitId::Ranger, TraitId::Fechtmeister)) {
			maxBlockChance = 0;
		}
		else {
			maxBlockChance = 75;
		}
		break;
	case PFC_INQUISITOR:	maxBlockChance = 70; break;
	case PFC_GUARDIAN:		
		if (HasTrait(playerIndex, TraitId::Pistoleer)){
			maxBlockChance = 0;
		}
		else {
			maxBlockChance = 20;
		} 
		break;
	case PFC_TEMPLAR:		maxBlockChance = 70; break;

	case PFC_ARCHER:		
	case PFC_SCOUT:			
	case PFC_SHARPSHOOTER:	maxBlockChance =  GameMode == GM_CLASSIC ? 80 : 0; break;
	case PFC_TRAPPER:		maxBlockChance = 65; break;

	case PFC_MAGE:	
	case PFC_ELEMENTALIST:	
	case PFC_WARLOCK:		
	case PFC_DEMONOLOGIST:
	case PFC_NECROMANCER:		
	case PFC_BEASTMASTER:	maxBlockChance = GameMode == GM_CLASSIC ? 70 : 0; break;

	case PFC_MONK:			maxBlockChance = 70; break;
	case PFC_KENSEI:		maxBlockChance = 40; break;
	case PFC_SHUGOKI:		maxBlockChance = 30; break;
	case PFC_SHINOBI:		maxBlockChance = 70; break;

	case PFC_ROGUE:			maxBlockChance =  0; break;
	case PFC_ASSASSIN:		maxBlockChance = 25; break;
	case PFC_IRON_MAIDEN:	maxBlockChance = 75; break;
	case PFC_BOMBARDIER:	maxBlockChance = 30; break;

	case PFC_SAVAGE:		
	case PFC_BERSERKER:		maxBlockChance = 20; break;
	case PFC_EXECUTIONER:	maxBlockChance = 10; break;
	case PFC_THRAEX:		maxBlockChance = 50; break;
	case PFC_MURMILLO:		maxBlockChance = 60; break;
	case PFC_DIMACHAERUS:	maxBlockChance = 0 + PerkValue(PERK_GEMINI, playerIndex); break;
	case PFC_SECUTOR:		maxBlockChance = 70; break;
	case PFC_DRUID:			maxBlockChance = 40; break;
	}
}

void MvP_Melee_Autohit_chance(int playerIndex, int& autoHit, int& automiss) {

	Player& player = Players[playerIndex];

	// TODO: change to switch
	if (player.fullClassId == PFC_ASSASSIN) {
		autoHit = 18;
	}
	else if (player.fullClassId == PFC_ROGUE
		|| player.fullClassId == PFC_MONK
		|| player.fullClassId == PFC_SHINOBI) {
			int big_frame_trait_penalty = HasTrait(playerIndex, TraitId::BigFrame) ? 3 : 0;
		autoHit = 18 + big_frame_trait_penalty;
	}
	else if (player.fullClassId == PFC_KENSEI
		//|| player.fullClassId == PFC_GUARDIAN
		|| player.fullClassId == PFC_DIMACHAERUS) {
		autoHit = 19;
	}
	else if (player.ClassID == PC_2_MAGE) {
		autoHit = HasTrait(playerIndex, TraitId::Mamluk) ? 15 : 30;// Mamluk has 15% autohit
	}
	else if (player.fullClassId == PFC_BOMBARDIER
		|| player.fullClassId == PFC_TRAPPER) {
		autoHit = 22;
	}
	else if (player.fullClassId == PFC_SAVAGE
		|| player.fullClassId == PFC_BERSERKER) { // might need a revision later, check again according to plr feedback
		autoHit = 21;
	}
	else if (player.fullClassId == PFC_EXECUTIONER) {
		autoHit = 30 - PerkValue(PERK_ENHANCED_AGILITY, playerIndex);
	}
	else if (player.fullClassId == PFC_MURMILLO
		|| player.fullClassId == PFC_THRAEX
		|| player.fullClassId == PFC_SECUTOR) { // "sword & shield" type gladiators
		autoHit = 20;
	}
	else if (player.fullClassId == PFC_IRON_MAIDEN) {
		autoHit = 25;
	}
	if (HasTrait(playerIndex, TraitId::Sandman)) {
		autoHit += (1 + player.CharLevel / 15);
	}
	autoHit -= PerkValue(PERK_ACROBATICS, playerIndex);	

	if (HasTrait(playerIndex, TraitId::Pistoleer)) {
		autoHit += 2; // 22% effectively
	}
	else if (HasTrait(playerIndex, TraitId::BloodyMess)) {
		autoHit += 2;
	}
	else if (HasTrait(playerIndex, TraitId::Fechtmeister)) {
		autoHit -= 4;
	}
	else if (HasTrait(playerIndex, TraitId::Ranger)) {
		autoHit += 3;
	}
	else if (HasTrait(playerIndex, TraitId::TwoTowers)) {
		autoHit += 10;
	}
	else if (HasTrait(playerIndex, TraitId::Crupellarius)) {
		autoHit -= 1;
	}
	int small_frame_trait_benefit = HasTrait(playerIndex, TraitId::SmallFrame) ? 5 : 0;
	autoHit -= PerkValue(PERK_SHIELD_PARRY, playerIndex);
	autoHit -= PerkValue(SYNERGY_IMPROVED_EVASION, playerIndex);
	autoHit -= PerkValue(SYNERGY_DEATH_DENIER, playerIndex);
	autoHit -= small_frame_trait_benefit;
	autoHit += PerkValue(SYNERGY_HARDENED_STEEL, playerIndex, 1);
	autoHit -= PerkValue(SYNERGY_DAZZLING_LIGHT, playerIndex);
	AmplifyByNearAttackers(playerIndex, autoHit);

	automiss -= PerkValue(PERK_ACROBATICS, playerIndex);
	automiss -= PerkValue(SYNERGY_IMPROVED_EVASION, playerIndex);
	automiss -= PerkValue(PERK_HIT_DEFLECTION, playerIndex);
	// limitations
	LimitToMin(automiss, autoHit + 10);
	LimitToMin(autoHit, 8);
	LimitToMin(automiss, 40);
	if (GameMode == GM_CLASSIC) {
		automiss = 100;
		autoHit = 15;
		if (Dungeon->level == 14) autoHit = 20;
		if (Dungeon->level == 15) autoHit = 25;
		if (Dungeon->level == 16) autoHit = 30;
	}
}

void MvP_Melee_Block_Chance(int monsterIndex, int playerIndex, int& playerBlockChance)
{
	Player& player = Players[playerIndex]; 
	Monster& monster = Monsters[monsterIndex];

	int a_rose_with_thorns_trait_penalty = HasTrait(playerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
	int two_towers_trait_benefit_1 = HasTrait(playerIndex, TraitId::TwoTowers) ? (player.CharLevel / 6) : 0;
	playerBlockChance = player.BlockBonus + player.blockChance
		+ ((player.BaseDexterity + player.BaseStrength - monster.ArmorClass) / 2)
		- a_rose_with_thorns_trait_penalty
		+ two_towers_trait_benefit_1
		- PerkValue(SYNERGY_AGGRESSION, playerIndex, 0);
	LimitToMin(playerBlockChance, 0);
}
//int AutoHitShow = 0;
//----- (0043E0EC) --------------------------------------------------------
void __fastcall MvP_Melee(int monsterIndex, int playerIndex, int toHit, int minDamage, int maxDamage) // mvp melee
{
	if( (uint)monsterIndex >= Monsters_Max_Count || (uint)playerIndex >= Monsters_Max_Count ){ return; }
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if (!monster.SpritePtr) {
		return;
	}
	if (monster.flag & MF_5_TARGET_MONSTER) {// 0043E11C
		//0043E125
		MvM_Melee(monsterIndex, playerIndex, toHit, minDamage, maxDamage);
		return;
	}
	if( (uint)playerIndex >= PlayersMax_4 ){ return; }
	if (Hedgehog) {
		if (!((monster.CurrentLife&(-64)) <= 0
			|| monster.SpritePtr->baseMonsterIndex == BM_92_ILLUSION_WEAVER && monster.goal == MG_2_RETREAT
			|| monster.CurAction == A_14_CHARGE)) {
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
#if GODMODE_ALLOWED
	if( IsGodMode )	return;
#endif
	if ((player.CurLife&(-64)) <= 0 || player.notHittableInTransition || player.notHittableFramesAfterTransition > 0 ) {// 0043E138
		return;
	}
	if ((player.activeBuffFlag & BF_1_ETHEREAL)) { // 0043E16B
		//return; // Ethereal в th1 не дает бессмертия против физических аттак (мили и стрел)
	}
	int rowDelta = abs(monster.Row - player.Row);
	int colDelta = abs(monster.Col - player.Col);
	if( player.CurAction == PCA_2_WALK_DOWN ){ --rowDelta; -- colDelta; }
	if (rowDelta >= 2 || colDelta >= 2) {
		return;
	}
	// 0043E1AC
	int playerACsumm = player.ACFromItems + player.ACFromClass;
	if (/*HasTrait(playerIndex, TraitId::Kamikaze) ||*/ HasTrait(playerIndex, TraitId::Pyromaniac)) {
	}
	else {
		int basic_AC_divider = 5;
		if (HasTrait(playerIndex, TraitId::SmallFrame)) {
			basic_AC_divider = 3;
		}
		playerACsumm += (player.CurDexterity / basic_AC_divider) /* + HasTrait(playerIndex, TraitId::Mamluk) ? player.CharLevel : 0*/;
	}
	if (player.effectFlag[EA_AC_DEMON_MELEE] && monster.BasePtr->MonsterClass == MON_1_DEMON) { // 0043E1D5
		playerACsumm += 20;// 1.145
	}
	if (player.effectFlag[EA_AC_UNDEAD_MELEE] && monster.BasePtr->MonsterClass == MON_0_UNDEAD) { // 0043E1EF
		playerACsumm += 15;// 1.145
	}

	playerACsumm += (playerACsumm*player.spiciesAcPercent[monster.BasePtr->MonsterClass]) / 100; //  +%AC affixes vs undead/demons/beasts
	playerACsumm += player.spiciesAc[monster.BasePtr->MonsterClass];//  flat affixes vs undead/demons/beasts
	// 0043E206
	int lvlsDiffToHitbonus = 2 * (monster.MonsterLevel - player.CharLevel) + 20;// 128 clvl vs 178 mlvl= 70 (below- inferno case is continued in comments)
	int realToHitChance = toHit + lvlsDiffToHitbonus - playerACsumm;// 753 + 70 - 700 = 823 - 700 = 123
	int range;
	if (Difficulty == DL_0_HORROR || GameMode == GM_CLASSIC) {
		range = 21;	}
	/*else if (Difficulty == 1) {// th2 code (left for reference)
		realToHitChance -= 65;
		rngArg = 106;
	}
	else if (Difficulty == 2) {
		realToHitChance -= 150;
		rngArg = 201;
	}*/
	else {
		realToHitChance -= Difficulty * 85 - 20;//123-660= -537
		range = Difficulty * 95 + 11;// 771
	}
	// 0043E239
	realToHitChance += RNG(range);// range: -537 to 233

	//------ autohit ------------

	int autoHit = 20, automiss = 100;
	MvP_Melee_Autohit_chance(playerIndex, autoHit, automiss);
	//AutoHitShow = autoHit;
	LimitToMin(realToHitChance, autoHit);// no automiss limits here

	// 0043E2C5
	if (RNG(100) >= realToHitChance) {
		return;// не попали и ладно
	}

	if (HasTrait(playerIndex, TraitId::LitheBuild)) {
		if (RNG(100) > 91) {
			return;
		}
	}

	// ----------blocking logic-----------
	// 0043E265
	int blockRandFactor;
	bool canBlock = player.CanBlock;
	if (player.CurAction && player.CurAction != PCA_4_ATTACK || !canBlock) {
		blockRandFactor = 100;
	}
	else {
		blockRandFactor = RNG(100);
	}
	   	 
	//  -- BLOCKING --
	int playerBlockChance = 0;
	MvP_Melee_Block_Chance(monsterIndex, playerIndex, playerBlockChance);
	
	// -- max block chance --
	int maxBlockChance = 0;
	MvP_Melee_Max_Block_Chance(playerIndex, maxBlockChance);
	int a_rose_with_thorns_trait_penalty = HasTrait(playerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 3) : 0;
	int two_towers_trait_benefit_1 = HasTrait(playerIndex, TraitId::TwoTowers) ? (player.CharLevel / 6) : 0; 
	LimitToMax(playerBlockChance, (maxBlockChance - a_rose_with_thorns_trait_penalty + two_towers_trait_benefit_1 - PerkValue(SYNERGY_AGGRESSION, playerIndex, 0)));
	
	
	OnTakeMeleeTrigger( monsterIndex, playerIndex );
	
	if( blockRandFactor >= playerBlockChance ){
		// 0043E3B4
		// по игроку попали и он не смог блокировать
		// отнятие макс жизни желтым зомби убрал нафиг

		int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
		// 0070B600
		if( DoesMonsterStealMana (baseMonsterIndex,playerIndex) ){
			// 0043E3C3
			if( playerIndex == CurrentPlayerIndex ){
				#ifdef comment
				int msIndex = -1;
				for( int spellCastIndexIndex = 0; spellCastIndexIndex < SpellCastAmount; spellCastIndexIndex++ ){
					int spellCastIndex = CastIndexes[spellCastIndexIndex];
					SpellCast& cast = SpellCasts[spellCastIndex];
					if( cast.EffectIndex == MI_13_MANA_SHIELD && cast.CasterIndex == playerIndex ){
						msIndex = spellCastIndex;
						break;
					}
				}
				#endif
				if( player.CurMana > 64 ){
					int quarterCurMana = player.CurMana >> 2;
					player.CurMana -= quarterCurMana;
					player.BaseMana -= player.CurMana - quarterCurMana - quarterCurMana;
				}
			}
		}
		// 0043E4BB
		int damage = (minDamage << 6) + RNG((maxDamage - minDamage + 1) << 6);
		int realDamageToPlayer = ((player.DamageFromEnemy + player.spiciesDfe[monster.BasePtr->MonsterClass]) << 6) + damage;
		
		if (HasTrait(playerIndex, TraitId::BloodAndSand)) {
			LimitToMin(realDamageToPlayer, damage / 4);
		}
		else if (GameMode == GM_CLASSIC) {
			LimitToMin(realDamageToPlayer, 64);
		}
		else {
			int dfe_soak_factor = 50;
			// dfe_soak_factor + PerkValue(PERK_DAMAGE_SOAK, playerIndex);
			LimitToMin(realDamageToPlayer, dfe_soak_factor * damage / 100);
		}
		
		//LimitToMin(realDamageToPlayer, 64);

		// 0070D84D executioner mode for characters who failed block chance
		//if( MaxCountOfPlayersInGame != 1 ){
			/*if( Difficulty == 0 && player.CharLevel >= 37 
				|| Difficulty == 1 && player.CharLevel >= 50 ){
					realDamageToPlayer <<= 5;// дикая прибавка
			}
		}*/
		if (is(GameMode, GM_EASY)) {
			realDamageToPlayer = realDamageToPlayer * 80 / 100; // -20% easy mode
			if (player.gameChanger & BIT(GC_4_EFFORTLESS) && MaxCountOfPlayersInGame == 1) {
				realDamageToPlayer /= 2;
			}
		}
		else if (GameMode == GM_HARD) {
			realDamageToPlayer = realDamageToPlayer * 120 / 100; //+-20% hard mode
			if (player.gameChanger & BIT(GC_6_IMPOSSIBLE)) {
				realDamageToPlayer *= 2;
			}
		}
		else if (GameMode == GM_NIGHTMARE && RNG(100) < 3) {
			realDamageToPlayer *= 2;
			PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
		}
		if (MaxCountOfPlayersInGame == 1 && HasTrait(playerIndex, TraitId::Fatality)) {
			if (RNG(100) < 10) {
				realDamageToPlayer *= 3;
				PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
			}
		}
		// executioning mode:
		if (
			(HasTrait(playerIndex, TraitId::Psion) && player.fullClassId != PFC_MAGE)
			|| (HasTrait(playerIndex, TraitId::Cleric) && player.fullClassId != PFC_MAGE)
			|| (HasTrait(playerIndex, TraitId::Hydramancer) && player.fullClassId != PFC_WARLOCK)
			|| (HasTrait(playerIndex, TraitId::LordOfTheRings) && player.fullClassId != PFC_MONK)
			) {
			if (RNG(100) < 10) {
				realDamageToPlayer *= 20;
			}
		}

		// "Thick-Skinned" trait extra defense (-80% dmg) from "Critical Defense" perk:
		if (RNG(100) < PerkValue(PERK_CRITICAL_DEFENSE, playerIndex)) {
			realDamageToPlayer /= 5;
		}

		if (RNG(100) < PerkValue(SYNERGY_AUXILIARY_ARMOR, playerIndex, 0)) {
			realDamageToPlayer = realDamageToPlayer * (100 - PerkValue(SYNERGY_AUXILIARY_ARMOR, playerIndex, 1)) / 100;
		}

		if (RNG(100) < PerkValue(SYNERGY_LAST_STAND, playerIndex, 0) && (player.CurLife < PerkValue(SYNERGY_LAST_STAND, playerIndex, 2) * player.MaxCurLife / 100)) {
			realDamageToPlayer = realDamageToPlayer * (100 - PerkValue(SYNERGY_LAST_STAND, playerIndex, 1)) / 100;
		}

		if (player.fullClassId == PFC_BERSERKER) { 
			damage = damage * (100 - PerkValue(SYNERGY_INVINCIBLE, playerIndex)) / 100; 
		} 

		// "Emerge Unscathed" perk nullifies melee damage from monsters if the roll is successful:
		if (RNG(100) < PerkValue(PERK_EMERGE_UNSCATHED, playerIndex)) {
			realDamageToPlayer = 0;
		}

		// realDamageToPlayer is used for stun calc, stun desync is mainly due to other players assume the attacked player having a melee resist % = 0
		// Separate damage reduction calc and damage Reflect calc, so that both players see roughly consistent damage reduction to avoid stun desync, while only the attacked player processes damage taken and Reflect damage via network sync
		// Note that block chance desync and hit chance desync can also affect the processing of stun calculation
		// player.CountOfReflectCharges can desync due to chance desync, set player.CountOfReflectCharges = 0 in ReceiveCmdDestroyReflect() for partial fix
		float reflectPercent = ResistPercentMelee(playerIndex);
		int reflectDamagebyPlayer = ftol((double)realDamageToPlayer * reflectPercent);
		ActCriticalHit(reflectDamagebyPlayer, playerIndex, monster.Row, monster.Col, true, ET_0_PHYSICAL, -2); // - 2 reflect damage
		if (player.CountOfReflectCharges) {
			realDamageToPlayer = 0;
		} else {
			realDamageToPlayer -= reflectDamagebyPlayer;
			LimitToMin(realDamageToPlayer, 0);
		}

		// 0043E4F9 рефлект
		if( playerIndex == CurrentPlayerIndex ){
			// 0043E51A
//			int reflectPercent = CalcReflectPercent(playerIndex);
//			int reflectDamagebyPlayer = ftol(double(reflectPercent) * _0_01_OnePercent * double(realDamageToPlayer));
//			ActCriticalHit( reflectDamagebyPlayer, playerIndex, monster.Row, monster.Col, true, ET_0_PHYSICAL, -2 ); // - 2 reflect damage
			// 00710BA0
			if( player.CountOfReflectCharges ){
				--player.CountOfReflectCharges;
				int mulReflectDamage;
//				int reflectDamage;
				if( monster.newBossId ){
					// 00710BEE
					mulReflectDamage = reflectDamagebyPlayer / 4;
//					monster.CurrentLife -= mulReflectDamage;
//					reflectDamage = mulReflectDamage >> 2;
				}else{
					// 00710BE9
					mulReflectDamage = reflectDamagebyPlayer / 8;
//					monster.CurrentLife -= mulReflectDamage;
//					reflectDamage = mulReflectDamage >> 3;
				}
				monster.CurrentLife -= mulReflectDamage;
				LimitToMin(monster.CurrentLife, 0);
				// 0043E541
				//realDamageToPlayer -= reflectDamage;// >> old (dont delete)
//				realDamageToPlayer = 0;
//				LimitToMin(realDamageToPlayer, 0);
				if( (monster.CurrentLife & ~63) > 0 ){
//					DamageMonsterByPlayer(monsterIndex, playerIndex, reflectDamage);// Not mulReflectDamage?  Using reflectDamage causes monster HP desync
					DamageMonsterByPlayer(monsterIndex, playerIndex, mulReflectDamage);
				}else{
//					DrawFloatingDamage(reflectDamage, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
					DrawFloatingDamage(mulReflectDamage, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
					KillMonsterByPlayer(monsterIndex, playerIndex);
				}
			}
//			else{
				// 00710BD0
//				realDamageToPlayer -= reflectDamagebyPlayer;
//				LimitToMin(realDamageToPlayer, 0);
//			}
			// 0043E570
			player.CurLife -= realDamageToPlayer;
			player.BaseLife -= realDamageToPlayer;
		}
		// 0043E596
		MonsterLifeSteal(realDamageToPlayer, monsterIndex, playerIndex); // в 1.145 перемещено 0070B550 и переделано
		// неясна эта фигня. Игрока ведь ударили, как у него могут быть текущие жизни больше максимальных?
		// 0043E5FF
		if( player.CurLife > player.MaxCurLife ){ // here was the bug with the curLife checking in case of Players[0].CurLife negative value
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
		}
		player.lastAttacker = 1 + PlayersMax_4 + monsterIndex;
		if( (player.CurLife & ~63) > 0 || ! TryToDie(playerIndex, 0) ){ /*fix to Reflect*/
			StartPlayerHit(playerIndex, realDamageToPlayer, 0);
			if( PlayerKnockbackResistFail (monsterIndex, playerIndex)  ){
				if( player.CurAction != PCA_7_GOT_HIT ){
					StartPlayerHit(playerIndex, 0, 1);
				}
				// 0043E68B
				int newPlayerRow = player.Row + RowDelta[monster.Orientation];
				int newPlayerCol = player.Col + ColDelta[monster.Orientation];
				if( CellFreeForPlayer(playerIndex, newPlayerRow, newPlayerCol) ){
					// 00712960 фикс выкидывания из радиуса света
					ChangeLightPos(player.LightIndex, newPlayerRow, newPlayerCol);
					ChangeVision(player.lightRadiusIdentificator, newPlayerRow, newPlayerCol);
					player.Row = newPlayerRow;
					player.Col = newPlayerCol;
					// 0043E6C2
					FixPlayerLocation(playerIndex, player.dir);
					FixPlrWalkTags(playerIndex);
					PlayerMap[ newPlayerRow ][ newPlayerCol ] = (uchar)playerIndex + 1;
					SetPlayerOld(playerIndex);
				}
			}
		}
	}else{ // player is blocking...
		// 0043E2D7
		int orientation = OrientationToTarget(player.Row, player.Col, monster.Row, monster.Col);
		StartPlayerBlock(playerIndex, orientation);
		
		//---------------------wrecking block-------------------------------------

		if (HasTrait(playerIndex, TraitId::WreckingBlock)) {
			int wreck_damage_modifier = player.CharLevel / 2; // wrecking block returns monsters' damage back to them, increased by clvl%
			int wreck_damage = (minDamage << 6) + RNG((maxDamage - minDamage + 1) << 6);
			LimitToMin(wreck_damage, 64);
			wreck_damage += wreck_damage * wreck_damage_modifier / 100;
			if (monster.newBossId) {				
				wreck_damage /= 2; // bosses get half of this damage
			}
			monster.CurrentLife -= wreck_damage;
			if ((monster.CurrentLife & (-64)) > 0) {
				DamageMonsterByPlayer(monsterIndex, playerIndex, wreck_damage);
			}
			else {
				DrawFloatingDamage(wreck_damage / 64, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
				KillMonsterByPlayer(monsterIndex, playerIndex);
			}
		}

		//----------------end of wrecking block-------------

		if( playerIndex == CurrentPlayerIndex ){
			return;
		}
		if( !player.CountOfReflectCharges /*|| !HasTrait(playerIndex, TraitId::WreckingBlock)*/){ // mor: exit if 0 reflect charges or no trait of wreckingblock (right?)
			return;
		}
		player.CountOfReflectCharges--;
		int damage = (minDamage << 6) + RNG((maxDamage - minDamage + 1) << 6);
		int realDamageToPlayer = ((player.DamageFromEnemy + player.spiciesDfe[monster.BasePtr->MonsterClass]) << 6) + damage;
		// 0070D7EF режим палача
		/*if( MaxCountOfPlayersInGame != 1 ){ // executioner mode, turned off now. don't erase!
			if( Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46 ){
					realDamageToPlayer <<= 5;// дикая прибавка
			}
		}*/
		LimitToMin(realDamageToPlayer, damage/2);// 1.145

		float reflectPercent = ResistPercentMelee(playerIndex);
		int reflectDamage = ftol((double)realDamageToPlayer * reflectPercent);
		ActCriticalHit( reflectDamage, playerIndex, monster.Row, monster.Col, true, ET_0_PHYSICAL, -2 ); // - 2 reflect damage
		//int reflect_block_damage_multiplier = HasTrait(playerIndex, TraitId::WreckingBlock) ? (player.CharLevel / 3 + 2) : 1;
		if( monster.newBossId ){
			monster.CurrentLife -= realDamageToPlayer * 2; //* reflect_block_damage_multiplier;
		}else{
			monster.CurrentLife -= realDamageToPlayer * 4; //* reflect_block_damage_multiplier;
		}
		if( (monster.CurrentLife & (-64)) > 0 ){
			DamageMonsterByPlayer(monsterIndex, playerIndex, reflectDamage);
		}else{
			DrawFloatingDamage(reflectDamage, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
}

//----- (0043E705) --------------------------------------------------------
int __fastcall MonsterRangedAttack(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count || ! monster.SpritePtr ) return false;

	if( monster.CurFrame == monster.BasePtr->FirstAtackToHitFrame && ! (monster.flag & MF_16_TRACK_ATTACK) ){
		int baseMissile = monster.PrevActionOrRowDeltaOrSpellEffect;
		if( baseMissile != -1 ){ // TODO: check wtf is this, MISSILE enum has no -1 element
			int missileIndex = -1;
			int spellCount = baseMissile == MI_52_CHARGED_BOLT ? 3 : 1;
			for( int i = 0; i < spellCount; i++ ){
				missileIndex = CastMissile( monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, monster.Orientation, monster.PrevActionOrRowDeltaOrSpellEffect, CT_1_MONSTER_AND_TRAP, monsterIndex, monster.TimerCounterOrColDeltaOrSpellDamage, 0, 0);
			}
		}
		PlayMonsterSound(monsterIndex, MST_0_ATTACK);
	}
	if( monster.CurFrame == monster.animFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return true;
	}else{
		return false;
	}
}

//----- (0043E7D5) --------------------------------------------------------
int __fastcall MonsterSecondRangedAttack(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count || ! monster.SpritePtr ) return 0;

	if( monster.CurFrame == monster.BasePtr->SecondAtackToHitFrame ){
		if( !monster.AnimationDelayIndex ){
			int castCount = 1;
			int baseMissile = monster.PrevActionOrRowDeltaOrSpellEffect;
			if( baseMissile == MI_52_CHARGED_BOLT ){
				switch( GameMode ){
				case GM_EASY: castCount = 3; break;
				case GM_HARD: castCount = 5; break;
				default:      castCount = 4; break;
				}
				if (monster.SpritePtr->baseMonsterIndex == BM_274_FLESH_DEVOURER) castCount *= 2;
			}
			//if( is(monster.SpritePtr->baseMonsterIndex, BM_279_FLOATER, BM_280_FLOATER_X2) && baseMissile == MI_24_ARCANE_STAR ) castCount = 4;
			for( int i = 0; i < castCount; i++ ){
				CastMissile( monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, monster.Orientation, monster.PrevActionOrRowDeltaOrSpellEffect, CT_1_MONSTER_AND_TRAP, monsterIndex, monster.ActionRow_ActOrient, 0, 0);
			}
			if( Sounds[S_COUNT + monster.SpriteIndex * Monster_Sound_Max_8 + MST_3_SATTACK * 2].buffer ){
				PlayMonsterSound(monsterIndex, MST_3_SATTACK);
			}
		}
	}
	if( monster.ai == AI_26_Balrog ){
		if( monster.CurFrame == 3 ){
			int breathDelay = monster.TimerCounterOrColDeltaOrSpellDamage;
			monster.TimerCounterOrColDeltaOrSpellDamage = breathDelay + 1;
			if( breathDelay ){
				int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex; 
				if (baseMonsterIndex == BM_243_IZUAL) {
					if (breathDelay == 1) { // balrog AI delay for incinerate cast?
						monster.flag &= ~MF_3_STONE;
					}
				}
				else if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
					if (breathDelay == 7) {
						monster.flag &= ~MF_3_STONE;
					}
				}
				else {
					if (breathDelay == 14) { 
						monster.flag &= ~MF_3_STONE;
					}
				}
			}else{
				monster.flag |= MF_3_STONE;
			}
		}
	}
	if( monster.CurFrame == monster.animFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return 1;
	}else{
		return 0;
	}
}

//----- (0043E8DB) --------------------------------------------------------
int __fastcall MonsterSecondMeleeAttack(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < Monsters_Max_Count && monster.SpritePtr ){
		if( monster.CurFrame == monster.BasePtr->SecondAtackToHitFrame ){
			MvP_Melee(monsterIndex, monster.TargetIndex, monster.SecondToHit, monster.SecondMinDamage, monster.SecondMaxDamage);
			if( Sounds[S_COUNT + monster.SpriteIndex * Monster_Sound_Max_8 + MST_3_SATTACK * 2].buffer ){
				PlayMonsterSound(monsterIndex, MST_3_SATTACK);
			}
		}
		if( monster.CurFrame == monster.animFrameCount ){
			FixMonsterPosition(monsterIndex, monster.Orientation);
			return 1;
		}
	}
	return 0;
}

//----- (0043E952) --------------------------------------------------------
bool __fastcall MonsterEndMeleeAttack( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];

	bool isFinalFrame = false;
	if( monster.flag & MF_2_BACKWARDS ){
		isFinalFrame = monster.CurFrame == 1;
	}else{
		isFinalFrame = monster.CurFrame == monster.animFrameCount;
	}
	if( monsterIndex < Monsters_Max_Count && isFinalFrame ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		monster.flag &= ~MF_2_BACKWARDS;
		return true;
	}
	return false;
}

//----- (0043E9A4) --------------------------------------------------------
bool __fastcall SecondaryAnimation( uint monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	bool isFinalFrame = false;
	if( monster.flag & MF_2_BACKWARDS ){
		isFinalFrame = monster.CurFrame == 1;
	}else{
		isFinalFrame = monster.CurFrame == monster.animFrameCount;
	}
	if( monsterIndex < Monsters_Max_Count && isFinalFrame ){
		int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
		if( baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK ){ // mor: this is wrong
			monster.flag = monster.flag & ~MF_2_BACKWARDS;
		}else{
			monster.flag = monster.flag & ~MF_2_BACKWARDS | MF_1_INVISIBLE;
		}
		FixMonsterPosition(monsterIndex, monster.Orientation);
		return true;
	}
	return false;
}

//----- (0043EA14) --------------------------------------------------------
int __fastcall HealMode(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex < Monsters_Max_Count){
		if( !(monster.flag & MF_4_NOHEAL) ){
			if( monster.CurFrame == 1 ){
				monster.flag = monster.flag & ~MF_2_BACKWARDS | MF_3_STONE;
				if( monster.PrevActionOrRowDeltaOrSpellEffect + monster.CurrentLife >= monster.BaseLife){
					monster.CurrentLife = monster.BaseLife;
					monster.flag = monster.flag & ~MF_2_BACKWARDS & ~MF_3_STONE;
					monster.CurAction = A_7_SECOND;
				}else{
					monster.CurrentLife += monster.PrevActionOrRowDeltaOrSpellEffect;
				}
			}
		}
	}
	return 0;
}

//----- (0043EA85) --------------------------------------------------------
int __fastcall MonsterTalk( int monsterIndex )
{
	if( (uint) monsterIndex >= Monsters_Max_Count ) return 0;
	Monster& monster = Monsters[monsterIndex];
	FixMonsterPosition( monsterIndex, monster.Orientation );
	monster.goal = MG_7_WAIT_TO_TALK;
	if( SoundIsPlaying(Speeches[ monster.speechIndex ].SoundIndex) ) return 0;
	StartSpeech( monster.speechIndex );
	switch( monster.newBossId - 1 ){
	case UM_0_Gharbad_the_Weak or UM_C_849_GHARBAD:
		if( monster.speechIndex == 144 ){
			Quests[ Q_2_GHARBAD_THE_WEAK ].status = QS_2_IN_PROGRESS;
		}
		Quests[ Q_2_GHARBAD_THE_WEAK ].talkToFlag = 1;
		if( monster.speechIndex == 145 && ! (monster.flag & MF_7_DROP) ){
			SpawnItem( monsterIndex, monster.Row + 1, monster.Col + 1, 1 );
			monster.flag |= MF_7_DROP;
		}
		break;
	case UM_2_Zhar_the_Mad or UM_C_870_ZHAR_THE_MAD:
		if( monster.speechIndex == 148 && ! (monster.flag & MF_7_DROP) ){
			Quests[ Q_3_ZHAR_THE_MAD ].status = QS_2_IN_PROGRESS;
			Quests[ Q_3_ZHAR_THE_MAD ].talkToFlag = 1;
			CreateTypeItem( monster.Row + 1, monster.Col + 1, 0, 0, 24, 1, 0 );
			monster.flag |= MF_7_DROP;
		}
		break;
	case UM_3_Snotspill or UM_C_829_SNOTSPILL:
		if( monster.speechIndex == 20 && ! (monster.flag & MF_7_DROP) ){
			ChangeMapRectAfterScriptEvent(CurPattern.row, CurPattern.col, CurPattern.sizeRow/2 + CurPattern.row + 2, CurPattern.sizeCol/2 + CurPattern.col - 2 );
			uchar prevHallIndex = TransValue;
			TransValue = 9;
			SetAreaTransparencyByIndex_40(CurPattern.row, CurPattern.col, CurPattern.sizeRow/2 + CurPattern.row + 4, CurPattern.sizeCol/2 + CurPattern.col );
			TransValue = prevHallIndex;
			Quests[ Q_7_OGDENS_SIGN ].status2 = 2;
			if( Quests[ Q_7_OGDENS_SIGN ].status == QS_1_ACCEPT ){
				Quests[ Q_7_OGDENS_SIGN ].status = QS_2_IN_PROGRESS;
			}
			monster.flag |= MF_7_DROP;
		}
		if( Quests[ Q_7_OGDENS_SIGN ].status2 < 2 ){
			sprintf(InfoPanelBuffer, "SS Talk = %i, Flags = %i", monster.speechIndex, monster.flag);
			TerminateWithError( InfoPanelBuffer );
		}
		break;
	case UM_7_Lachdanan or UM_C_865_LACHDANAN:
		if( monster.speechIndex == 81 ){
			Quests[ Q_4_LACHDANAN ].status = QS_2_IN_PROGRESS;
			Quests[ Q_4_LACHDANAN ].talkToFlag = 1;
			Quests[Q_4_LACHDANAN].status2 = 1;
			if (Quests[Q_4_LACHDANAN].status4 == 0) //checks if golden elixir is already dropped while quest status is the same
			SpawnSomething(BI_328_GOLDEN_ELIXIR, 0, 0, 0, 5, 1, 1);
			Quests[Q_4_LACHDANAN].status4 = 1;
		}
		if( monster.speechIndex == 83 && ! (monster.flag & MF_7_DROP) ){
			int uniqIndex = 0;
			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				uniqIndex = by(Difficulty, 184, 185, 186, 2389, 2390, 2391, 2392, 2393, 2394); // str line
			}
			else {
				switch (Players[CurrentPlayerIndex].ClassID) { // Veil of Steel rewards
				case PC_1_ARCHER or PC_3_MONK or PC_4_ROGUE:
					uniqIndex = by(Difficulty, 441, 442, 443, 2395, 2396, 2397, 2398, 2399, 2400); break; // dex line
				case PC_2_MAGE:
					uniqIndex = by(Difficulty, 444, 445, 446, 2401, 2402, 2403, 2404, 2405, 2406); break; // mag line
				case PC_0_WARRIOR or PC_5_SAVAGE:
					uniqIndex = by(Difficulty, 184, 185, 186, 2389, 2390, 2391, 2392, 2393, 2394); break; // str line
				}
			}
			if (GameMode == GM_CLASSIC) uniqIndex = Difficulty + 4722;
			SpawnUnique( uniqIndex, monster.Row + 1, monster.Col + 1 );
			monster.flag |= MF_7_DROP;
		}
		break;
	case UM_8_Warlord_of_Blood or UM_C_862_WARLORD:
		Quests[ Q_11_WARLORD_OF_BLOOD ].status2 = 2;
		break;
	case UM_4_ArchBishop_Lazarus or UM_C_867_LAZARUS :
		if( MaxCountOfPlayersInGame != 1 ){
			monster.goal = MG_1_ATTACK;
			monster.ActivationCounter = 255;
			monster.speechIndex = 0;
			Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 = 6;
		}
		break;
	}
	return 0; // RUN_DONE (1: RUN_AGAIN)
}
//----- (0043ED00) --------------------------------------------------------
void __fastcall MonsterJumpAroundTarget(uint monsterIndex)
{
	#ifndef REFUCKTOR
	Monster *monster; // esi@2
	int row; // edi@3
	int col; // ebx@5
	int col_1; // [sp+Ch] [bp-24h]@0
	int targetRow; // [sp+10h] [bp-20h]@3
	int r_minus_1_plus_1; // [sp+14h] [bp-1Ch]@3
	int targetCol; // [sp+18h] [bp-18h]@3
	uint monsterIndex_1; // [sp+1Ch] [bp-14h]@1
	int c_minus_1_plus_1; // [sp+20h] [bp-10h]@3
	int found; // [sp+24h] [bp-Ch]@2
	int deltaCol; // [sp+28h] [bp-8h]@5
	int deltaRow; // [sp+2Ch] [bp-4h]@3

	monsterIndex_1 = monsterIndex;
	if( monsterIndex < Monsters_Max_Count ){
		found = 0;
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction != A_15_STONE ){
			targetRow = monster->TargetRow;
			targetCol = monster->TargetCol;
			r_minus_1_plus_1 = 2 * RNG(2) - 1;
			deltaRow = -1;
			//row = col_1; // что это блин
			c_minus_1_plus_1 = 2 * RNG(2) - 1;
			while( !found ){
				deltaCol = -1;
				col = targetCol - c_minus_1_plus_1;
				do{
					if( found ){
						break;
					}
					if( deltaRow || deltaCol ){
						col_1 = col;
						row = targetRow + r_minus_1_plus_1 * deltaRow;
						if( In112(row, col) && row != monster->Row && col != monster->Col ){
							if( CheckCellForMeleeMonsterSafeMove( monsterIndex_1, targetRow + r_minus_1_plus_1 * deltaRow, col ) ){
								found = 1;
							}
						}
					}
					++deltaCol;
					col += c_minus_1_plus_1;
				}while( deltaCol < 1 );
				if( ++deltaRow > 1 ){
					if( !found ){
						return;
					}
					break;
				}
			}
			ClearMonsterOnMap( monsterIndex_1 );
			MonsterMap[ 0 ][ 112 * monster->Row + monster->Col ] = 0;
			MonsterMap[ 0 ][ col_1 + 112 * row ] = monsterIndex_1 + 1;
			monster->PrevRow = row;
			monster->PrevCol = col_1;
			monster->Orientation = MonsterOrientationToTarget( monsterIndex_1 );
			CheckMonsterUnderArch( monsterIndex_1 );
		}
	}
	#else
	Monster& monster = Monsters[monsterIndex];
	int row;
	int col;
	bool found = false;    
	if( monsterIndex >= Monsters_Max_Count || monster.CurAction == A_15_STONE ){
		return;
	}
	int targetRow = monster.TargetRow;
	int targetCol = monster.TargetCol;
	int r_minus_1_plus_1 = 2 * RNG(2) - 1;
	int c_minus_1_plus_1 = 2 * RNG(2) - 1;
	for( int deltaRow = -1; deltaRow <= 1 && !found; ++deltaRow ){
		for( int deltaCol = -1; deltaCol <= 1 && !found; ++deltaCol ){
			if( deltaRow || deltaCol ){
				row = targetRow + r_minus_1_plus_1 * deltaRow;
				col = targetCol + c_minus_1_plus_1 * deltaCol;
				if( In112(row, col) && row != monster.Row && col != monster.Col 
				 && CheckCellForMeleeMonsterSafeMove(monsterIndex, row, col) ){
					found = true;
				}
			}
		}
	}
	if( found ){
		ClearMonsterOnMap(monsterIndex);
		MonsterMap[ monster.Row ][ monster.Col ] = 0;
		MonsterMap[ row ][ col ] = monsterIndex + 1;
		monster.PrevRow = row;
		monster.PrevCol = col;
		//monster.Row = row; // откуда это взялось? в th1 нету этих строчек
		//monster.Col = col; // 
		monster.Orientation = MonsterOrientationToTarget(monsterIndex);
		CheckMonsterUnderArch(monsterIndex);
	}
	#endif
}

//----- (0043EE33) --------------------------------------------------------
int __fastcall MonsterHit(uint monsterIndex)
{
	int result = 0;
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex < Monsters_Max_Count && monster.SpritePtr && monster.CurFrame == monster.animFrameCount ){
		FixMonsterPosition(monsterIndex, monster.Orientation);
		result = 1;
	}
	return result;
}

//----- (0043EE6C) --------------------------------------------------------
void __fastcall MayBe_KillPackBoss(uint monsterIndex)
{
	Monster& boss = Monsters[monsterIndex];
	if( monsterIndex < Monsters_Max_Count ){
		for( int i = 0; i < MonstersCount; i++ ){
			Monster& monster = Monsters[MonsterIndexes[i]];
			if( monster.isBossPack == 1 && monster.hisBossIndex == monsterIndex ){
				monster.isBossPack = 0;
			}
		}
		if( boss.isBossPack == 1 ){
			--Monsters[boss.hisBossIndex].BossPackCount; // пока нигде не используется, для чего планировалось не ясно
		}
	}
}

//----- (0043EEDE) --------------------------------------------------------
int DoEnding()
{
	char v0;    // al@5
	char* v2;   // ecx@9
	int result; // eax@12
	char v4;    // bl@12
	int v5;     // esi@12
	if( MaxCountOfPlayersInGame > 1u )
		SNetLeaveGame(0x40000004);
	StopMusic();
	StopAmbient();
	if( MaxCountOfPlayersInGame > 1u )
		SleepTh(1000);
	v0 = Players[CurrentPlayerIndex].ClassID;
	if( v0 && v0 != 5 ){
		if( v0 != 2 && v0 != 3 ){
			v2 = "gendata\\DiabVic3.smk";
		}else{
			v2 = "gendata\\DiabVic1.smk";
		}
	}else{
		v2 = "gendata\\DiabVic2.smk";
	}
	extern bool CanSkipVictoryVideo;
	PlayVideo(v2, CanSkipVictoryVideo );
	PlayVideo("gendata\\Diabend.smk", CanSkipVictoryVideo );
	v4 = IsMusicEnabled;
	IsMusicEnabled = 1;
	v5 = GetOrSetMusicVolume(1);
	GetOrSetMusicVolume(0);
	PlayMusic("Music\\RazB.wav");
	LoopMovie = 1;
	PlayVideo("gendata\\loopdend.smk", 1); 
	LoopMovie = 0;
	StopMusic();
	result = GetOrSetMusicVolume(v5);
	IsMusicEnabled = v4;
	return result;
}

//----- (0043EFB8) --------------------------------------------------------
void PrepDoEnding()
{
	// enable sound again
	IsSoundEnable = SaveSound;
	gbDoEnding = 1;
	gbRunGame = 0;
	IsPlayerDead = 0;

	for( int i = 0; i < PlayersMax_4; ++i ){
		Player& player = Players[i];
		player.CurAction = PCA_11_QUIT;
		player.notHittableInTransition = 1;
		if( MaxCountOfPlayersInGame > 1 ){
			if( !(player.CurLife & ~63) ){
				player.CurLife = 64;
			}
			if( !(player.CurMana & ~63) ){
				player.CurMana = 64;
			}
		}
	}
}

bool UberDiabloKilled = false;
//----- (0043F06E) --------------------------------------------------------
int __fastcall MonsterHitToDeath(uint monsterIndex)
{
	if( monsterIndex < Monsters_Max_Count ){
		Monster& monster = Monsters[monsterIndex];
		if( monster.SpritePtr ){
			++monster.PrevActionOrRowDeltaOrSpellEffect;
			if( is(monster.SpritePtr->baseMonsterIndex, BM_137_UBER_DIABLO, BM_C_389_DIABLO) ){
				UpdateAllowedDifficulty();
				UberDiabloKilled = true;
				AdvanceSpeedrunTimer();
			}
			if( (MaxCountOfPlayersInGame == 1 && monster.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO && Difficulty == DL_0_HORROR
			 && !is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON ))
				|| monster.SpritePtr->baseMonsterIndex == BM_C_389_DIABLO){
				int rowDelta = monster.Row - PlayerRowPos;
				PlayerRowPos += rowDelta >= 0 ? rowDelta > 0 : -1;
				int colDelta = monster.Col - PlayerColPos;
				PlayerColPos += colDelta >= 0 ? colDelta > 0 : -1;
				if( monster.PrevActionOrRowDeltaOrSpellEffect == 140 ){ // таймер анимации смерти дьябло ?
					PrepDoEnding();
				}
			}else if( monster.CurFrame == monster.animFrameCount ){
				AddMonsterCorpse( monster.Row, monster.Col, monster.newBossId ? monster.udeadNum : monster.SpritePtr->deadSpriteNum, monster.Orientation );
				if (monster.newBossId) {
					ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
				}
				monster.flag |= MF_15_KILLED;
				MonsterMap[ monster.Row ][ monster.Col ] = 0;
				MayBe_KillPackBoss( monsterIndex );
			}else if( monster.ai == AI_53_Exploder ){ // exploder damage
				int dlvl_modifier = 6, addon = 20, diff_modifier = 90;
				if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) { 
					dlvl_modifier = 5, addon = 15, diff_modifier = 75; 
				}
				else if (GameMode == GM_HARD) { 
					dlvl_modifier = 8, addon = 25, diff_modifier = 120; 
				}
				int damage = 3 * (Dungeon->level * dlvl_modifier + addon + Difficulty * diff_modifier) / 2;
				int row = monster.Row;
				int col = monster.Col;
				switch( monster.CurFrame ){
				case 1:
					//CastMissile(row, col, row, col, 0, MI_94_BIG_FIRE_EX, CT_1_MONSTER_AND_TRAP, monsterIndex, damage, 0);
					for( int i = 0; i < 8; i++ ){
						CastMissile(row + RowDelta[i], col + ColDelta[i], row + RowDelta[i], col + ColDelta[i], 0, MI_140_EXPLODER, CT_1_MONSTER_AND_TRAP, monsterIndex, damage, 0, 0);
					}
					break;
				case 5:
					for( int i = 0; i < 8; i++ ){
						int rd = RowDelta[i], cd = ColDelta[i];
						CastMissile(row + rd * 2, col + cd * 2, row + rd * 2, col + cd * 2, 0, MI_140_EXPLODER, CT_1_MONSTER_AND_TRAP, monsterIndex, damage / 2, 0, 0);
						if( rd && cd ){
							CastMissile(row + rd, col + cd * 2, row + rd, col + cd * 2, 0, MI_140_EXPLODER, CT_1_MONSTER_AND_TRAP, monsterIndex, damage / 2, 0, 0);
							CastMissile(row + rd * 2, col + cd, row + rd * 2, col + cd, 0, MI_140_EXPLODER, CT_1_MONSTER_AND_TRAP, monsterIndex, damage / 2, 0, 0);
						}
					}
					break;
				}
			}
		}
	}
	return 0;
}

//----- (0043F168) --------------------------------------------------------
int __fastcall MonsterRangedHit(uint monsterIndex)
{
	int v1;     // ebx@1
	int v2;     // esi@2
	int result; // eax@6
	v1 = monsterIndex;
	if( (unsigned int)monsterIndex >= Monsters_Max_Count )
		goto LABEL_10;
	v2 = sizeof(Monster) * monsterIndex;
	if( !Monsters[monsterIndex].SpritePtr )
		goto LABEL_10;
	if( Monsters[monsterIndex].CurFrame == Monsters[monsterIndex].BasePtr->SecondAtackToHitFrame )
		PlayMonsterSound(monsterIndex, MST_3_SATTACK);
	if( *(int*)((char*)&Monsters[0].CurFrame + v2) == *(int*)((char*)&Monsters[0].animFrameCount + v2) ){
		FixMonsterPosition(v1, *(int*)((char*)&Monsters[0].Orientation + v2));
		result = 1;
	}else{
LABEL_10:
		result = 0;
	}
	return result;
}

//----- (0043F1C4) --------------------------------------------------------
int __fastcall MonsterOrientTarget(uint monsterIndex)
{
	int result = 0;
	int monsterOffset = sizeof(Monster) * monsterIndex;
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex < Monsters_Max_Count && monster.SpritePtr ){
		monster.curAnimation = &monster.SpritePtr->animation[ANIM_0_STAY].oriented[MonsterOrientationToTarget(monsterIndex)];
		if( monster.ai == AI_28_Lazarus ){
			if( monster.TimerCounterOrColDeltaOrSpellDamage > 8 || monster.TimerCounterOrColDeltaOrSpellDamage < 0 ){
				monster.TimerCounterOrColDeltaOrSpellDamage = 8;
			}
		}
		int flag = monster.TimerCounterOrColDeltaOrSpellDamage;
		--monster.TimerCounterOrColDeltaOrSpellDamage;
		if( ! flag ){
			int prevCurFrame = monster.CurFrame;
			FixMonsterPosition(monsterIndex, monster.Orientation);
			monster.CurFrame = prevCurFrame;
			result = 1;
		}
	}
	return result;
}

//----- (0043F258) --------------------------------------------------------
int __fastcall MonsterStoneCursed(uint monsterIndex)
{
	if (monsterIndex < Monsters_Max_Count) {
		Monster& monster = Monsters[monsterIndex];
		uint monsterOffset = sizeof(Monster) * monsterIndex;
		if (monster.CurrentLife <= 0) {
			monster.flag |= MF_15_KILLED;
			MonsterMap[ monster.Row ][ monster.Col ] = 0;
		}
	}
	return 0;
}

//----- (0043F296) --------------------------------------------------------
void __fastcall StartMonsterWalk(uint monsterIndex, int orientation)
{
	int lastFrame;
	int speedX;           
	int speedY;           
	int rowDelta;           
	int colDelta;           
	int orient;           
	int speedx;           
	int speedy;           
	int deltax;          
	int deltay;          
	int deltarow;          
	int deltacol;          
	int deltaRowHz;          
	int deltaColHz;          
	int orient__;          
	int speedX_;          
	int speedY_;          
	int deltaX;          
	int deltaY;          
	int deltaRow;          
	int deltaCol;          
	int orient_;          
	int v24;          
	int v25;          
	int v26;          
	int v27;          
	if( monsterIndex < Monsters_Max_Count ){
		lastFrame = Monsters[monsterIndex].SpritePtr->animation[ANIM_1_WALK].frameCount - 1;
		switch( orientation ){
		case DIR_4_U:
			orient = 4;
			colDelta = -1;
			rowDelta = -1;
			speedY = -MonsterFramesSpeed[lastFrame].Diagonal;
			speedX = 0;
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		case DIR_5_UR:
			orient = 5;
			v24 = 12 * lastFrame;
			colDelta = -1;
			rowDelta = 0;
			speedY = -*(int*)((char*)&MonsterFramesSpeed[0].Vertical + v24);
			speedX = *(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v24);
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		case DIR_6_R:
			orient__ = 6;
			deltaColHz = 0;
			deltaRowHz = 1;
			deltacol = -1;
			deltarow = 1;
			deltay = -16;
			deltax = -32;
			speedx = MonsterFramesSpeed[lastFrame].Horizontal;
			speedy = 0;
			StartMonsterWalkHorizontal(monsterIndex, speedx, speedy, deltax, deltay, deltarow, deltacol, deltaRowHz, deltaColHz, orient__);
			break;
		case DIR_7_DR:
			orient_ = 7;
			deltaCol = 0;
			deltaRow = 1;
			v25 = 12 * lastFrame;
			deltaY = -16;
			deltaX = -32;
			speedY_ = *(int*)((char*)&MonsterFramesSpeed[0].Vertical + v25);
			speedX_ = *(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v25);
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case DIR_0_D:
			orient_ = 0;
			deltaCol = 1;
			deltaRow = 1;
			deltaY = -32;
			deltaX = 0;
			speedX_ = 0;
			speedY_ = MonsterFramesSpeed[lastFrame].Diagonal;
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case DIR_1_DL:
			orient_ = 1;
			v26 = 12 * lastFrame;
			deltaCol = 1;
			deltaRow = 0;
			deltaY = -16;
			deltaX = 32;
			speedY_ = *(int*)((char*)&MonsterFramesSpeed[0].Vertical + v26);
			speedX_ = -*(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v26);
			StartMonsterWalkDown(monsterIndex, speedX_, speedY_, deltaX, deltaY, deltaRow, deltaCol, orient_);
			break;
		case DIR_2_L:
			orient__ = 2;
			deltaColHz = 1;
			deltaRowHz = 0;
			deltacol = 1;
			deltarow = -1;
			deltay = -16;
			deltax = 32;
			speedy = 0;
			speedx = -MonsterFramesSpeed[lastFrame].Horizontal;
			StartMonsterWalkHorizontal(monsterIndex, speedx, speedy, deltax, deltay, deltarow, deltacol, deltaRowHz, deltaColHz, orient__);
			break;
		case DIR_3_UL:
			orient = 3;
			v27 = 12 * lastFrame;
			colDelta = 0;
			rowDelta = -1;
			speedY = -*(int*)((char*)&MonsterFramesSpeed[0].Vertical + v27);
			speedX = -*(int*)((char*)&MonsterFramesSpeed[0].Diagonal + v27);
			StartMonsterWalkUp(monsterIndex, speedX, speedY, rowDelta, colDelta, orient);
			break;
		}
	}
}

//----- (0043F3D9) --------------------------------------------------------
void __fastcall StartMonsterWalkUp(int monsterIndex, int speedX, int speedY, int rowDelta, int colDelta, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	monster.NextRow = monster.Row + rowDelta;
	monster.NextCol = monster.Col + colDelta;
	monster.CurAction = A_1_WALK_UP; 
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = -1 - monsterIndex;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.PrevActionOrRowDeltaOrSpellEffect = rowDelta;
	monster.TimerCounterOrColDeltaOrSpellDamage = colDelta;
	monster.ActionOrient = orient;
	monster.Orientation = orient; 
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 0;
	monster.WalkY = 0;
	monster.WalkframeCounter  = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F4A6) --------------------------------------------------------
void __fastcall StartMonsterWalkDown(int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
	monster.RowDelta = monster.Row;
	monster.ColDelta = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.NextRow = monster.Row + deltaRow;
	monster.NextCol = monster.Col + deltaCol;
	monster.Row = monster.NextRow;
	monster.Col = monster.NextCol;
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = monsterIndex + 1;
	if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
		ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
	}
	monster.DeltaX = deltaX;
	monster.DeltaY = deltaY;
	monster.CurAction = A_2_WALK_DOWN;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.ActionOrient = orient;
	monster.Orientation = orient;
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 16 * deltaX;
	monster.WalkY = 16 * deltaY;
	monster.WalkframeCounter = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F5DC) --------------------------------------------------------
void __fastcall StartMonsterWalkHorizontal( int monsterIndex, int speedX, int speedY, int deltaX, int deltaY, int deltaRow, int deltaCol, int deltaRowHz, int deltaColHz, int orient )
{
	Monster& monster = Monsters[monsterIndex];
	if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
		ChangeLightPos(monster.LightIndex, monster.Row + deltaRowHz, monster.Col + deltaColHz);
	}
	monster.NextRow = monster.Row + deltaRow;
	monster.NextCol = monster.Col + deltaCol;
	monster.NextRowHz = monster.Row + deltaRowHz;
	monster.NextColHz = monster.Col + deltaColHz;
	MonsterMap[ monster.Row ][ monster.Col ] = -1 - monsterIndex;
	FlagMap[ monster.NextRowHz ][ monster.NextColHz ] |= CF_16_MONSTER_HORIZONTAL; // why not next cell as player ???
	MonsterMap[ monster.NextRow ][ monster.NextCol ] = -1 - monsterIndex;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.DeltaX = deltaX;
	monster.DeltaY = deltaY;
	monster.CurAction = A_3_WALK_HORIZ;
	monster.SpeedX = speedX;
	monster.SpeedY = speedY;
	monster.RowDelta = monster.NextRow;
	monster.ColDelta = monster.NextCol;
	monster.ActionOrient = orient;
	monster.Orientation = orient;
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_1_WALK], orient);
	monster.WalkX = 16 * deltaX;
	monster.WalkY = 16 * deltaY;
	monster.WalkframeCounter = 0;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043F735) --------------------------------------------------------
void __fastcall BossPackActivation(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	int bossIndex = monsterIndex;
	if( monster.isBossPack ){
		bossIndex = monster.hisBossIndex;
		Monster& bossMonster = Monsters[bossIndex];
		if( abs(monster.Row - bossMonster.Row) >= 6 || abs(monster.Col - bossMonster.Col) >= 6 ){
			monster.isBossPack = 2;
		}else{
			monster.isBossPack = 1;
		}
	}
	if( monster.isBossPack ){
		Monster& bossMonster = Monsters[bossIndex];
		if( monster.ActivationCounter > bossMonster.ActivationCounter ){
			bossMonster.LastTargetPositionRow = monster.Row;
			bossMonster.LastTargetPositionCol = monster.Col;
			bossMonster.ActivationCounter = monster.ActivationCounter - 1;
		}
		if( bossMonster.ai == AI_12_Gargoyle ){
			InterruptStoneState(bossIndex);			
		}
		return;
	}
	int bossNumber = monster.newBossId;
	if( ! bossNumber ){ // (9) расширение номера босса 0043F877
		return;
	}
	#ifdef OLD_CODE
	if( ! ( UniqueMonsters[bossNumber - 1].PackTrigger & 1 ) ){
		return;
	}
	#endif
	for( int monsterIndexIndex = 0; monsterIndexIndex < MonstersCount; ++monsterIndexIndex ){
		int bossMonsterIndex = MonsterIndexes[monsterIndexIndex];
		Monster& bossMonster = Monsters[bossMonsterIndex];
		//if( bossMonsterIndex == 14 ) __debugbreak();
		if( bossMonster.isBossPack && bossMonster.hisBossIndex == monsterIndex ){
			if( monster.ActivationCounter > bossMonster.ActivationCounter ){
				if( bossMonster.isBossPack == 1 ){
					bossMonster.LastTargetPositionRow = monster.LastTargetPositionRow;
					bossMonster.LastTargetPositionCol = monster.LastTargetPositionCol;
				}else{
					bossMonster.LastTargetPositionRow = monster.Row;
					bossMonster.LastTargetPositionCol = monster.Col;
				}
				bossMonster.ActivationCounter = monster.ActivationCounter - 1;
			}
			if( bossMonster.ai == AI_12_Gargoyle ){
				InterruptStoneState(bossMonsterIndex);
			}
		}
	}
}

//----- (0043F777) --------------------------------------------------------
void __fastcall InterruptStoneState(int monsterIndex/*monsterOffset<eax>*/)
{
	Monster& monster = Monsters[monsterIndex];
	if( monster.flag & MF_3_STONE && monster.CurrentLife == monster.BaseLife ){
		if( monster.ActivationCounter ){
			monster.flag &= ~MF_3_STONE;
			monster.CurAction = A_7_SECOND;
		}
	}
}

//----- (0043F937) --------------------------------------------------------
int __fastcall MonsterWalksTowardsTarget( int monsterIndex, int orientation )
{
	//#ifdef NEW_ORIENT
	if (not(GameMode, GM_EASY, GM_CLASSIC)) {
		return MoveInOrientOrAboveNew(monsterIndex, orientation, 3); // anti-cornering behavior
	}
	//#endif
	int nextOrient; // esi@1
	int orientation_4; // ebp@1
	int mayMove; // eax@1
	int mayMove_1; // ebx@1
	int orientation_2; // ebx@3
	int orientation_3; // ebx@6
	int preResult; // ebx@9
	int result; // ebx@19

	nextOrient = orientation;
	orientation_4 = orientation;
	mayMove = MayMoveMonsterToDirection( monsterIndex, orientation );
	mayMove_1 = mayMove;
	if( RNG(2) ){
		if( mayMove_1 ){
			goto LABEL_10;
		}
		orientation_2 = nextOrient;
		nextOrient = OrientMinusOne[ nextOrient ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_10;
		}
		nextOrient = OrientPlusOne[ orientation_2 ];
	}else{
		if( mayMove_1 ){
			goto LABEL_10;
		}
		orientation_3 = nextOrient;
		nextOrient = OrientPlusOne[ nextOrient ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_10;
		}
		nextOrient = OrientMinusOne[ orientation_3 ];
	}
	if( !MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
		preResult = 0;
		goto LABEL_11;
	}
LABEL_10:
	preResult = 1;
LABEL_11:
	if( RNG(2) ){
		if( preResult ){
			goto LABEL_20;
		}
		nextOrient = OrientPlusOne[ OrientPlusOne[ orientation_4 ] ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_20;
		}
		nextOrient = OrientMinusOne[ OrientMinusOne[ orientation_4 ] ];
	}else{
		if( preResult ){
			goto LABEL_20;
		}
		nextOrient = OrientMinusOne[ OrientMinusOne[ orientation_4 ] ];
		if( MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
			goto LABEL_20;
		}
		nextOrient = OrientPlusOne[ OrientPlusOne[ orientation_4 ] ];
	}
	if( !MayMoveMonsterToDirection( monsterIndex, nextOrient ) ){
		result = 0;
		goto LABEL_21;
	}
LABEL_20:
	result = 1;
LABEL_21:
	if( result ){
		StartMonsterWalk( monsterIndex, nextOrient );
	}
	return result;
}

//----- (0043FA48) --------------------------------------------------------
int __fastcall MakeMonsterPath(uint monsterIndex)
{
	int(__fastcall * stepFunc)(int, int, int);// ecx@2
	int result = 0;                           // eax@5
	char steps[28];                           // [sp+4h] [bp-1Ch]@4

	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count ){
		return false;
	}
	if( monster.flag & MF_10_OPEN_DOORS ){ // ranger ?
		stepFunc = CheckCellForMonsterCanInteractWithDoorsSafeMove;
	}else{
		stepFunc = CheckCellForMeleeMonsterSafeMove;
	}
	// генерация пути для монстра
	// тут бывали отрицательные координаты у монстра
	if( FindPath(stepFunc, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, steps) ){
			MonsterWalksTowardsTarget(monsterIndex, StepOrientation[steps[0]]);// То есть составляется полный путь до цели а используется только первый шаг и так каждый раз?                                        // была неправильная адресация
			result = 1;
	}
	return result;
}

//----- (0043FABB) --------------------------------------------------------
int __fastcall MoveInOrientOrAbove( int monsterIndex, int orientation )
{
	int v2; // esi@1
	unsigned int v3; // ebx@1
	int v4; // eax@1
	int v6; // edi@1
	int v7; // edi@3
	int v8; // edi@6
	int v9; // edi@9

	v2 = orientation;
	v3 = monsterIndex;
	v4 = MayMoveMonsterToDirection( monsterIndex, orientation );
	v6 = v4;
	if( RNG(2) ){
		if( v6 ){
			goto LABEL_10;
		}
		v7 = v2;
		v2 = OrientMinusOne[ v2 ];
		if( MayMoveMonsterToDirection( v3, v2 ) ){
			goto LABEL_10;
		}
		v2 = OrientPlusOne[ v7 ];
	}else{
		if( v6 ){
			goto LABEL_10;
		}
		v8 = v2;
		v2 = OrientPlusOne[ v2 ];
		if( MayMoveMonsterToDirection( v3, v2 ) ){
			goto LABEL_10;
		}
		v2 = OrientMinusOne[ v8 ];
	}
	if( !MayMoveMonsterToDirection( v3, v2 ) ){
		v9 = 0;
		goto LABEL_11;
	}
LABEL_10:
	v9 = 1;
LABEL_11:
	if( v9 ){
		StartMonsterWalk( v3, v2 );
	}
	return v9;
}

//----- (0043FB44) --------------------------------------------------------
bool __fastcall MoveToDirectionIfMay( int monsterIndex, int orientation )
{
	Wrap(orientation, 8);
	if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
		StartMonsterWalk(monsterIndex, orientation);
		return true;
	}
	return false;
}

//----- (0043FB65) --------------------------------------------------------
int __fastcall MonsterCircleTarget( int monsterIndex, int orientationToTarget, int& curOrientation )
{
	int orientation;
	// this fixes Izual without breaking other monsters' circling.  Not gonna touch it for MP Izual.
	// 2 questions now. 1. what is it? and 2. does it need to be applied to mp izual now that the quest is present in mp?
	if (MaxCountOfPlayersInGame == 1 && (Dungeon->isQuest && Dungeon == DUN_36_VALLEY_OF_DESPAIR)) {
		if (curOrientation) {
			orientation = OrientMinusOne[orientationToTarget];
		}
		else {
			orientation = OrientPlusOne[orientationToTarget];
		}
	}
	else {
		if (curOrientation) {
			orientation = OrientMinusOne[OrientMinusOne[orientationToTarget]];
		}
		else {
			orientation = OrientPlusOne[OrientPlusOne[orientationToTarget]];
		}
	}
	int orientationFirst = orientation;
	if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
		StartMonsterWalk(monsterIndex, orientation);
		return 1;
	}

	if( curOrientation ){
		orientation = OrientPlusOne[orientation];
		if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
			StartMonsterWalk(monsterIndex, orientation);
			return 1;
		}
		orientation = OrientPlusOne[orientation];
	}else{
		orientation = OrientMinusOne[orientation];
		if( MayMoveMonsterToDirection(monsterIndex, orientation) ){
			StartMonsterWalk(monsterIndex, orientation);
			return 1;
		}
		orientation = OrientMinusOne[orientation];
	}

	if( !MayMoveMonsterToDirection(monsterIndex, orientation) ){
		curOrientation = curOrientation == 0;
		return MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationFirst]);
	}else{
		StartMonsterWalk(monsterIndex, orientation);
		return 1;
	}
}

//----- (0043FC36) --------------------------------------------------------
void __fastcall Ai_0_Zombie(uint monsterIndex) // Zombie AI
{
	#if 0
	Zombie AI, by Jarulf Guide:
	General
	
	if out of light, exit
	
	R = Rnd[100]
	
	Distance = 1
	if R < 2·Intf + 10, do melee attack, exit
	continue to stand still
	
	Distance = 2 to 2·Intf + 3
	if R < 2·Intf + 10, walk towards target, exit
		continue to stand still
	
		Distance > 2·Intf + 3
		if R >= 2·Intf + 10, continue to stand still, exit
		calculate new R = Rnd[100]
		if R < 2·Intf + 20, walk in random direction(if blocked, continue to stand still), exit
		continue to walk in the same direction as last time(if blocked, continue to stand still)
	#endif

	Monster& mon = Monsters[ monsterIndex ];
	if( monsterIndex >= Monsters_Max_Count || mon.CurAction != A_0_STAY ) return;

	int range = max(abs(mon.Row - mon.TargetRow), abs(mon.Col - mon.TargetCol));
	if( range >= 2 ){ // if not in melee range...
		int chase_factor = by(Difficulty, 10, 12, 14, 15, 16, 17, 18, 19, 20) + (GameMode == GM_HARD ? 70 : 0);
		if (RNG(100) < 2 * mon.intel + chase_factor){ //  Rnd[100] < 2 * mIntF + X
			int distanceSensitivity = GameMode == GM_HARD ? 6 : 4;
			if( range >= 2 * mon.intel + distanceSensitivity ){ // if distance is > mIntF * 2 + 3
				int dir = mon.Orientation;
				if( RNG(100) < 2 * mon.intel + 4 ){
					dir = RNG(8);
				}
				MoveToDirectionIfMay( monsterIndex, dir );
			}else{ // walk towards player
				MonsterWalksTowardsTarget( monsterIndex, MonsterOrientationToTarget(monsterIndex) );
			}
		}
	}else{ // distance = adjacent
		int attackChance;
		if( GameMode == GM_HARD ){
			attackChance = Difficulty + 91;
		}else{
			attackChance = 5 * mon.intel + by(Difficulty, 14, 30, 40, 42, 44, 46, 48, 49, 50) - (is(GameMode, GM_EASY/*, GM_CLASSIC*/) ? 5 : 0);
		}
		if( RNG(100) < attackChance ){
			StartMonsterMeleeAttack( monsterIndex );
		}
	}
	if( !mon.CurAction ){
		mon.curAnimation = &mon.SpritePtr->animation[ANIM_0_STAY].oriented[ mon.Orientation ];
	}
}

//----- (0043FD73) --------------------------------------------------------
void __fastcall StartMonsterMeleeAttack(int monsterIndex, int dir /*= -1*/)
{
	Monster& monster = Monsters[monsterIndex];
	if( dir == -1 ) dir = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_2_ATTACK], dir);
	monster.CurAction = A_4_MELEE;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = dir;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (0043FDF5) --------------------------------------------------------
void __fastcall Ai_2_Skeleton_Melee(uint monsterIndex)
{
	Monster *monster; // esi@2
	int row; // ecx@4
	int col; // edx@4
	int rowDif; // edi@4
	int colDif; // ebp@4
	int orient; // ebx@4
	int rowDist; // eax@4
	int colDist; // eax@5
	int delay_duration; // edx@22
	int mon_Index; // [sp+10h] [bp-4h]@1

	mon_Index = monsterIndex;
	if( monsterIndex < Monsters_Max_Count ){
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction == A_0_STAY ){
			if( monster->ActivationCounter ){
				row = monster->Row;
				col = monster->Col;
				rowDif = row - monster->TargetRow;
				colDif = col - monster->TargetCol;
				orient = OrientationToTarget( row, col, monster->LastTargetPositionRow, monster->LastTargetPositionCol );
				monster->Orientation = orient;
				rowDist = abs( rowDif );
				// ------------------------------------------------------------------non-melee range----------------------------------------------
				if( rowDist >= 2 || (colDist = abs( colDif ), colDist >= 2) ){
					bool keep_walking = false;
					if (monster->PrevActionOrRowDeltaOrSpellEffect == A_13_DELAY) {
						keep_walking = true;
					}
					else {
						int delayChance = 35 - (Difficulty * 3) - (2 * monster->intel);
						if (GameMode == GM_HARD) { delayChance -= 30; }
						else if (is(GameMode, GM_IRONMAN, GM_EASY/*, GM_CLASSIC*/)) { delayChance += 5; }
						LimitToMin(delayChance, 0);
						if (RNG(100) >= delayChance) {
							keep_walking = true;
						}
					}

					if (keep_walking) {
						MonsterWalksTowardsTarget( mon_Index, orient );
					LABEL_17:
						if( monster->CurAction == A_0_STAY ){
							monster->curAnimation = &monster->SpritePtr->animation[ANIM_0_STAY].oriented[ orient ];
						}
						return;
					}					
					delay_duration = 16 + RNG(30) - 2 * monster->intel - 5 * Difficulty ;
					if (GameMode == GM_HARD) {
						delay_duration = 0;// 1 + RNG(4) - Difficulty;
					}
					else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
						delay_duration += 2 + RNG(10);
					}
					LimitToMin(delay_duration, 0);
				}
				else{ /////////////////----------------------------------------------- melee range:-------------------------------------------------
					int attack_chance = 2 * monster->intel + 10 + Difficulty * 25;
					if (GameMode == GM_HARD) { attack_chance += 70; }
					if( monster->PrevActionOrRowDeltaOrSpellEffect == A_13_DELAY || RNG(100) < attack_chance ){
						StartMonsterMeleeAttack( mon_Index );
						goto LABEL_17;
					}					
					delay_duration = 6 + RNG(12 - Difficulty) - monster->intel;
					if (GameMode == GM_HARD) {
						delay_duration = 1 + RNG(4) - Difficulty;
					}
					else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
						delay_duration += 2;
					}
					LimitToMin(delay_duration, 0);
				}
				MonsterStartDelay( mon_Index, delay_duration );
				goto LABEL_17;
			}
		}
	}
}

//----- (0043FF26) --------------------------------------------------------
void __fastcall MonsterStartDelay( int monsterIndex, int delay )
{
	Monster& monster = Monsters[monsterIndex];
	if( delay > 0 && monster.ai != AI_28_Lazarus /*&& GameMode != GM_NORMAL*/){
		monster.TimerCounterOrColDeltaOrSpellDamage = delay;
		monster.CurAction = A_13_DELAY;
	}
}

//----- (0043FF4C) --------------------------------------------------------
int __fastcall MonsterChangeTarget( int monsterIndex )
{
	if( (uint) monsterIndex < Monsters_Max_Count ){
		Monster& monster = Monsters[ monsterIndex ];
		MonsterSprite& monsterSprite = MonsterSprites[ monster.SpriteIndex ];
		MONSTER_GOAL monsterGoal = monster.goal;
		uint baseMonsterIndex = monsterSprite.baseMonsterIndex;
		
		if( baseMonsterIndex == BM_109_GOLEM
		 || monster.ActivationCounter
		 && !monster.CurAction && (	monsterGoal == MG_1_ATTACK || monsterGoal == MG_4_WALK || monsterGoal == MG_5_ATTACK2) 
		 && (monster.Row != 1 || monster.Col) ){ // условие на неактивного голема
			if( ! CheckLineWithThreeArgumentsCheckFunction( CheckCellForSomeMonsterSafeMove,
					monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)
			 || InInterval( LOBYTE_IDA(monster.pathCount), 4, 8 ) ){
				if( monster.flag & MF_10_OPEN_DOORS ){// Голем ?
					MonstersInteractWithDoors(monsterIndex);
					if (monster.CurAction == A_4_MELEE)
					return 0;
				}
				if( ++LOBYTE_IDA(monster.pathCount) < 5 ){
					return 0;
				}
				if( MakeMonsterPath(monsterIndex) ){// Здесь была обработка монстра с нулевыми координатами, чего быть не должно (монстр 95, при кол-ве 94)
					return 1;
				}
			}
			if( baseMonsterIndex != BM_109_GOLEM ){
				LOBYTE_IDA(monster.pathCount) = 0;
			}
		}
	}
	return 0;
}

//----- (0044000B) --------------------------------------------------------
void __fastcall Ai_24_Viper(uint monsterIndex)
{
	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if (monster.CurAction || !LOBYTE(monster.ActivationCounter)) {
		return;
	}

	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	monster.Orientation = orientationToTarget;
	if (abs(monster.Row - monster.TargetRow) <= 1 && abs(monster.Col - monster.TargetCol) <= 1) {
		if (monster.PrevAction == A_13_DELAY || monster.PrevAction == A_14_CHARGE
			|| RNG(100) < monster.intel + by(Difficulty,
				GameMode == GM_HARD ? 55 : 9,
				GameMode == GM_HARD ? 65 : 30,
				75,
				75,
				75,
				75,
				75,
				75,
				75)) {
			StartMonsterMeleeAttack(monsterIndex);
			if (monster.CurAction == A_0_STAY) {
				monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];;
			}
			return;
		}
		MonsterStartDelay(monsterIndex, 10 - monster.intel + RNG(10));
		if (!monster.CurAction) {
			monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
		}
		return;
	}
	int chance = RNG(100);
	int viperChance = Dungeon->level / 2 + 13 + Difficulty * 10;
	if (MaxCountOfPlayersInGame != 1) { viperChance += 10; }
	if (GameMode == GM_HARD) { viperChance += 15; }
	int charge_distance = (GameMode == GM_HARD ? 5 : 3);
	if( abs(monster.Row - monster.TargetRow) <= charge_distance && abs(monster.Col - monster.TargetCol) <= charge_distance // 4 for test, and rnd 1 for test
	 && chance < viperChance ){
		bool checkLine = CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) != 0;
		if( checkLine && monster.PrevActionOrRowDeltaOrSpellEffect != A_14_CHARGE ){
			if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1 ){
				PlayMonsterSound(monsterIndex, MST_0_ATTACK);
				MonsterMap[ monster.Row ][ monster.Col ] = -1 - int(monsterIndex);
				monster.CurAction = A_14_CHARGE;
			}
			if( monster.CurAction == A_0_STAY ){
				monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
			}
			return;
		}
	}

	if( monster.PrevAction != 13 ){
		int intel = by( Difficulty, 65 - 2 * monster.intel, 35 - monster.intel, 20 - monster.intel, 20 - monster.intel, 20 - monster.intel, 20 - monster.intel, 20 - monster.intel, 20 - monster.intel, 20 - monster.intel);
		if( RNG(100) < intel ){
			MonsterStartDelay(monsterIndex, 15 - monster.intel + RNG(10));
			if( monster.CurAction == A_0_STAY ){
				monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];;
			}
			return;
		}
	}

	char wiperPathArray[6] = {1, 1, 0, -1, -1, 0};
	
	int newOrientation = orientationToTarget + wiperPathArray[monster.goalParam++];
	Wrap(newOrientation, 8);
	Wrap(monster.goalParam, 6);

	int deltaOrient = newOrientation - monster.curOrientation;
	Wrap(deltaOrient, 8);
	// Проверка отличия нового направления от текущего
	if( deltaOrient == 4 ){// Если противоположно сразу меняем направление на новое
		monster.curOrientation = newOrientation;
	}else if( InInterval(deltaOrient, 4, 8) ){// Ближе поворот в сторону увеличения ориентации
		monster.curOrientation--;
	}else if( InInterval(deltaOrient, 0, 4) ){// Ближе поворот в сторону уменьшения ориентации
		monster.curOrientation++;
	}// Примечание. При 0 оставляем текущее направление неизменным
	Wrap(monster.curOrientation, 8);
	if( !MoveToDirectionIfMay(monsterIndex, monster.curOrientation) ){
		MoveInOrientOrAbove(monsterIndex, monster.Orientation);
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	return;
}

//----- (00440298) --------------------------------------------------------
void __fastcall Ai_11_Bat(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !LOBYTE(monster.ActivationCounter) ){
		return;
	}
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	monster.Orientation = orientationToTarget;
	
	int randFactor = RNG(100);
	
	if( monster.goal == MG_2_RETREAT ){// Если отступаем
		// отступления у этого типа атаки только на одну клетку
		if( monster.goalParam ){// Когда значение ненулевое идём в направлении игрока. 
			int newOrientation;
			if( RNG(2) ){
				newOrientation = OrientMinusOne[orientationToTarget];
			}else{
				newOrientation = OrientPlusOne[orientationToTarget];
			}
			MonsterWalksTowardsTarget(monsterIndex, newOrientation);
			monster.goal = MG_1_ATTACK;// Наступление
		}else{// отступление. Идём в направлении от игрока
			MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]);
			monster.goalParam++;
		}
		return;
	}

	bool chargeAllowed = randFactor < 4 * monster.intel + (GameMode == GM_HARD ? 63 : 33); // higher chances to charge on HARD mode
	bool walkWithDelayAllowed = randFactor < monster.intel + by(Difficulty,8,14, 16, 18, 20, 22, 24, 26, 28) + (GameMode == GM_HARD ? 60 : 0); // less walk delays on HARD mode
	bool walkWithoutDelayAllowed = randFactor < monster.intel + by(Difficulty,55,64,67, 70, 73, 76, 79, 82, 85) + (GameMode == GM_HARD ? 5 : 0); // same with continuing walk on HARD
	bool meleeAttackAllowed = randFactor < by(Difficulty, 2 * monster.intel + (GameMode == GM_HARD ? 50 : 4), 
		4 * monster.intel + (GameMode == GM_HARD ? 56 : 15),
		4 * monster.intel + (GameMode == GM_HARD ? 57 : 20),
		4 * monster.intel + (GameMode == GM_HARD ? 58 : 25),
		4 * monster.intel + (GameMode == GM_HARD ? 59 : 30),
		4 * monster.intel + (GameMode == GM_HARD ? 60 : 35),
		4 * monster.intel + (GameMode == GM_HARD ? 61 : 40),
		4 * monster.intel + (GameMode == GM_HARD ? 62 : 45),
		4 * monster.intel + (GameMode == GM_HARD ? 62 : 50) );

	if( is(monsterSprite.baseMonsterIndex, BM_40_QUASIT, BM_C_383_GLOOM) && Dungeon->level != 3 && !Distance_To_Target_Is_Less_Than(monsterIndex, 5) && chargeAllowed
		&& CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
		if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1 ){
			MonsterMap[ monster.Row ][ monster.Col ] = -1 - int(monsterIndex);
			monster.CurAction = A_14_CHARGE;
		}
	}else if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > by(Difficulty,24, 18, 16, 14, 12, 11, 10, 9, 8) && walkWithDelayAllowed // После 20 фрэймовой задержки есть некоторый шанс на движение
			|| isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllowed ){// После движения шанс на последующее движение увеличен
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
		}
	}else if( meleeAttackAllowed ){// Если подошли в упор и разрешена атака
		StartMonsterMeleeAttack(monsterIndex);// удар с отступлением
		monster.goalParam = 0;
		monster.goal = MG_2_RETREAT;
		if( is(monsterSprite.baseMonsterIndex, BM_41_DARK_SERAPH, BM_C_384_FAMILIAR )){
			CastMissile( monster.TargetRow, monster.TargetCol, monster.TargetRow + 1, 0, -1, MI_8_LIGHTING_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, RNG(10) + 1, 0, 0 );
		}else if( monsterSprite.baseMonsterIndex == BM_83_LASH_WORM ){
			CastMissile( monster.TargetRow, monster.TargetCol, monster.TargetRow + 1, 0, -1, MI_8_LIGHTING_SEGMENT, CT_1_MONSTER_AND_TRAP, monsterIndex, RNG(monster.MaxDamage) + 1, 0, 0 );
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004404F7) --------------------------------------------------------
void __fastcall Ai_3_ArcherRarelyRetreats(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	monster.Orientation = orientationToTarget;
	int randFactor = RNG(100);
	
	bool walkWithDelayAllow = randFactor < by( Difficulty,	2 * monster.intel + (GameMode == GM_HARD ? 88 :  2),  // 94
															4 * monster.intel + (GameMode == GM_HARD ? 83 : 10),  // 95
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
															8 * monster.intel + (GameMode == GM_HARD ? 72 : 25)); // 96
	
	bool walkWithoutDelayAllow = randFactor < by(Difficulty,8 * monster.intel + (GameMode == GM_HARD ? 73 : 11),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 40),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 57),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 59),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 61),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 63),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 65),  // 97
															8 * monster.intel + (GameMode == GM_HARD ? 74 : 67),  // 98
															8 * monster.intel + (GameMode == GM_HARD ? 75 : 69)); // 99
	bool moveAllow = false;

	int keep_distance = Difficulty + 3 + (GameMode == GM_HARD ? 1 : 0);
	LimitToMax(keep_distance, 6);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN) && (is(monster.SpritePtr->baseMonsterIndex, BM_20_RISEN_SKELETON, BM_21_SKELETON_WARRIOR, BM_22_BURNING_DEAD, BM_23_MALICE))) {
		keep_distance = 1; // skeletal archers don't run away from Night Kin
	}
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT)) {
		keep_distance = 1;
	}
	if( Distance_To_Target_Is_Less_Than(monsterIndex, keep_distance) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > 1 /*20*/ && walkWithDelayAllow || isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow ){
			moveAllow = MoveToDirectionIfMay(monsterIndex, OrientInverted[orientationToTarget]);
		}
	}
	if( !moveAllow ){
		if( RNG(100) < by( Difficulty, 2 * monster.intel + (GameMode == GM_HARD ? 40 : 2),
											2 * monster.intel + (GameMode == GM_HARD ? 60 : 17),
			2 * monster.intel + (GameMode == GM_HARD ? 63 : 19),
				2 * monster.intel + (GameMode == GM_HARD ? 66 : 21),
			2 * monster.intel + (GameMode == GM_HARD ? 69 : 23),
					2 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
				2 * monster.intel + (GameMode == GM_HARD ? 75 : 27),
					2 * monster.intel + (GameMode == GM_HARD ? 78 : 29),
											2 * monster.intel + (GameMode == GM_HARD ? 80 : 32)) ){
			if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				StartMonsterRangedAttack(monsterIndex, MI_0_NONE_ARROW, 4);
			}
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044062C) --------------------------------------------------------
int __fastcall StartMonsterRangedAttack(int monsterIndex, int spellEffect, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	int orientation = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_2_ATTACK], orientation);
	monster.CurAction = A_10_RANGED;
	monster.PrevActionOrRowDeltaOrSpellEffect = spellEffect;
	monster.TimerCounterOrColDeltaOrSpellDamage = damage;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orientation;
	CheckMonsterUnderArch(monsterIndex);
	return 0;
}

//----- (004406C4) --------------------------------------------------------
void __fastcall Ai_1_MeleeRandomSecondAttackSwing(uint monsterIndex)
{
	Monster *monster; // esi@2
	int distToTargetRow; // edi@4
	int distToTargetCol; // ebx@4
	int idle_timer; // ecx@6
	int v5; // eax@8
	int mIntF; // eax@28
	int idle_chance; // eax@35
	int max_intel; // eax@42
	int monsterOrientToTarget; // [sp+Ch] [bp-Ch]@4
	int monster_index; // [sp+10h] [bp-8h]@1
	
	monster_index = monsterIndex;
	if( monsterIndex < Monsters_Max_Count ){
		monster = &Monsters[ monsterIndex ];
		if( !monster->CurAction ){
			if( monster->ActivationCounter ){
				distToTargetRow = monster->Row - monster->TargetRow;
				distToTargetCol = monster->Col - monster->TargetCol;
				monsterOrientToTarget = MonsterOrientationToTarget( monsterIndex );
				monster->Orientation = monsterOrientToTarget;
				if( abs( distToTargetRow ) < 2 && abs( distToTargetCol ) < 2 ){					
					mIntF = monster->intel; // this is now PERFECTLY balanced! don't change ever!
					int attack_chance = mIntF + 15 + (Difficulty * 5); // 18-64
					if (GameMode == GM_HARD) {
						attack_chance += 30; // 48-94
					}
					else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
						attack_chance -= 10; // 8-54
					}
					if (RNG(100) < attack_chance) {
						if (RNG(100) < (attack_chance + 15)) { // chance for secondary attack: 28-34, 38-44, 48-54
							StartMonsterSecondMeleeAttack(monster_index);
						}
						else {
							StartMonsterMeleeAttack(monster_index);
						}
					}
					goto STAY_IDLE; // play idle animation
				}
				idle_timer = monster->TimerCounterOrColDeltaOrSpellDamage;
				if( Difficulty == DL_1_PURGATORY ){
					if( idle_timer <= 16 ){
						goto LABEL_8;
					}
					max_intel = 4 * monster->intel + (GameMode == GM_HARD ? 52 : 39);
				}else if( Difficulty >= DL_2_DOOM ){
					if( idle_timer <= 8 ){
						goto LABEL_8;
					}
					max_intel = 4 * monster->intel + (GameMode == GM_HARD ? 64 : 63);
				}else{
					if( idle_timer <= 24 ){
						goto LABEL_8;
					}
					max_intel = 4 * monster->intel + (GameMode == GM_HARD ? 42 : 15);
				}
				if(RNG(100) < max_intel ){
				MOVE_TO_TARGET:
					MonsterWalksTowardsTarget( monster_index, monsterOrientToTarget );
				STAY_IDLE:
					if( !monster->CurAction ){
						monster->curAnimation = &monster->SpritePtr->animation[ANIM_0_STAY].oriented[ monsterOrientToTarget ];
					}
					return;
				}
			LABEL_8:
				v5 = monster->PrevActionOrRowDeltaOrSpellEffect;
				if( v5 != 1 && v5 != 2 && v5 != 3 ){
					goto STAY_IDLE;
				}
				if( idle_timer ){
					goto STAY_IDLE;
				}
				if( Difficulty == DL_1_PURGATORY ){
					idle_chance = 2 * monster->intel + 70;
				}else{
					idle_chance = Difficulty >= DL_2_DOOM ? 2 * monster->intel + 82 : 2 * monster->intel + 58;
				}
				if(RNG(100) >= idle_chance ){
					goto STAY_IDLE;
				}
				goto MOVE_TO_TARGET;
			}
		}
	}
	#ifdef REFACKTOR
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	monster.Orientation = orientationToTarget;
	int randFactor = RNG(100);
	if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
		bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
		if( monster.TimerCounter > 20 && randFactor < 4 * monster.intel + 20
			|| isAfterWalk && !monster.TimerCounter	&& randFactor < 4 * monster.intel + 70 ){
				MoveInOrientOrAbove_2(monsterIndex, orientationToTarget);
		}
	}else{
		if( randFactor < 4*monster.intel + 15 ){
			StartMonsterAttack(monsterIndex);
		}else if( randFactor < 4 * monster.intel + 20 ){
			StartMonsterSecondMeleeAttack(monsterIndex);
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	#endif
}

//----- (004407E0) --------------------------------------------------------
void __fastcall StartMonsterSecondMeleeAttack(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int orient = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_7_SECOND;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (00440865) --------------------------------------------------------
void __fastcall Ai_15_Hidden(uint monsterIndex)
{
	Monster *monster; // esi@2
	int monsterCol; // ebx@3
	int tarColDist; // ebx@4
	int intelFactor; // eax@4
	int rowDist; // eax@6
	int v8; // eax@7
	//int v9; // ST04_4@7
	int tarIndex; // eax@12
	int orient; // eax@13
	int nextOrient; // eax@17
	int intelLimit; // edi@23
	int timeFrame; // ecx@30
	int prevAction; // eax@32
	int attack_chance; // eax@51
	int hide_Radius; // edi@56
	int maxIntel_1; // eax@65
	int maxIntel; // eax@72
	int baseMonster; // eax@79
	int maxRadius; // edx@89
	int radius; // eax@89
	int v26; // [sp-4h] [bp-20h]@85
	int random_100; // [sp+Ch] [bp-10h]@20
	uint monsterIndex_1; // [sp+10h] [bp-Ch]@1
	int tarRowDist; // [sp+14h] [bp-8h]@4
	int tarOrient; // [sp+18h] [bp-4h]@4

	monsterIndex_1 = monsterIndex;
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	monster = &Monsters[ monsterIndex ];
	if( monster->CurAction ){
		return;
	}
	monsterCol = monster->Col;
	if( LightMap[ monster->Row ][ monsterCol ] == LightMax ){
		return;
	}
	tarRowDist = monster->Row - monster->TargetRow;
	tarColDist = monsterCol - monster->TargetCol;
	tarOrient = MonsterOrientationToTarget( monsterIndex );
	intelFactor = monster->intel;
	if (monster->SpritePtr->baseMonsterIndex == BM_269_BOG_CRAWLER || monster->SpritePtr->baseMonsterIndex == BM_270_MUD_CRAWLER) {
		goto LABEL_59;
	}
	if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
		hide_Radius = 4; // on Easy mode they appear 2 tiles away from player
		goto LABEL_5;
	}
	else if( Difficulty >= 2 || GameMode == GM_HARD ){
		hide_Radius = 2; // appears at melee distance
		goto LABEL_5;
	}
	else if( Difficulty == 1 ){
		hide_Radius = 4 - intelFactor;
		if( 4 - intelFactor >= 2 ){
			goto LABEL_5;
		}
		goto LABEL_59;
	}
	hide_Radius = 6 - intelFactor;
	if( 6 - intelFactor < 2 ){
	LABEL_59:
		hide_Radius = 2;
	}
LABEL_5:
	if( monster->PrevActionOrRowDeltaOrSpellEffect == A_5_GOT_HIT ){
		baseMonster = monster->SpritePtr->baseMonsterIndex;
		// these monsters don't retreat if hit (and stunned?)
		if( is( baseMonster, BM_79_FEAR_HUNTER, BM_52_BILE_DEMON, BM_113_ASH_GOLEM, BM_125_GOLD_GOLEM, BM_0_BURNED_CORPSE, BM_288_EARTH_GOLEM,
			BM_28_BLOOD_WARPER, BM_218_ICE_GOLEM, BM_237_CONTAMINATOR, BM_238_TAINTED, BM_239_AFFLICTED, BM_222_NIGHT_DEMON, BM_269_BOG_CRAWLER, BM_270_MUD_CRAWLER ) ){
			goto LABEL_10;
		}
		if( Difficulty == 1 ){
			v26 = 55;
		}else{
			v26 = Difficulty >= 2 ? 80 : 30;
		}
		maxRadius = GameMode == GM_HARD ? 81 : v26;
		radius = RNG(maxRadius);
		if( radius ){
			goto LABEL_10;
		}
		monster->goalParam = 0;
		monster->goal = MG_2_RETREAT;
	}
	rowDist = abs( tarRowDist );
	if( rowDist >= hide_Radius + 3 || (v8 = abs( tarColDist ), v8 >= hide_Radius + 3) || monster->goalParam > 8 ){
		monster->goalParam = 0;
		monster->goal = MG_1_ATTACK;
	}
LABEL_10:
	if( monster->goal == MG_2_RETREAT ){
		if( !( monster->flag & MF_3_STONE) ){
			tarIndex = monster->TargetIndex;
			orient = !(monster->flag & MF_5_TARGET_MONSTER) ? OrientationToTarget(
				monster->Row,
				monster->Col,
				Players[ tarIndex ].ownerRow,
				Players[ tarIndex ].ownerCol ) : OrientationToTarget(
					monster->Row,
					monster->Col,
					Monsters[ tarIndex ].Row,
					Monsters[ tarIndex ].Col );
			tarOrient = OrientInverted[ orient ];
			if( monster->SpritePtr->baseMonsterIndex == BM_31_UNSEEN ){// unsee retreats in zig-zag moves
				if( RNG(2) ){
					nextOrient = OrientMinusOne[ tarOrient ];
				}else{
					nextOrient = OrientPlusOne[ tarOrient ];
				}
				tarOrient = nextOrient;
			}
		}
	}
	monster->Orientation = tarOrient;
	random_100 = RNG(100);
	if (abs(tarRowDist) < hide_Radius
			&& abs(tarColDist) < hide_Radius
			&& monster->flag & MF_1_INVISIBLE
			&& CheckCastInSight(monster->Row, monster->Col, monster->TargetRow, monster->TargetCol)) {
			Monster_Fade_In(monsterIndex_1, tarOrient, 0);
			goto LABEL_39;
	}
	if (monster->SpritePtr->baseMonsterIndex == BM_269_BOG_CRAWLER 
		|| monster->SpritePtr->baseMonsterIndex == BM_270_MUD_CRAWLER) {// Bog/Mud Crawler fades out as soon as player disengages from melee distance
		intelLimit = hide_Radius;
	}
	else { 
		intelLimit = hide_Radius + 1; 
	}
	if( (abs( tarRowDist ) >= intelLimit || abs( tarColDist ) >= intelLimit) && !(monster->flag & MF_1_INVISIBLE) ){
		Monster_Fade_Out( monsterIndex_1, tarOrient, 1 );
		goto LABEL_39;
	}
	if( monster->goal == MG_2_RETREAT ){
		goto LABEL_38;
	}
	if( abs( tarRowDist ) < 2 && abs( tarColDist ) < 2 ){
		goto LABEL_39;
	}
	timeFrame = monster->TimerCounterOrColDeltaOrSpellDamage;
	if( Difficulty == 1 ){
		if( timeFrame <= 15 ){
			goto LABEL_32;
		}
		maxIntel = 4 * monster->intel + 25;
		goto LABEL_31;
	}
	if( Difficulty >= 2 ){
		if( timeFrame <= 5 ){
			goto LABEL_32;
		}
		maxIntel = 4 * monster->intel + 40;
	LABEL_31:
		if( random_100 >= maxIntel ){
			goto LABEL_32;
		}
	LABEL_38:
		++monster->goalParam;
		MonsterWalksTowardsTarget( monsterIndex_1, tarOrient );
		goto LABEL_39;
	}
	if( timeFrame > 30 ){
		maxIntel = 4 * monster->intel + 10;
		goto LABEL_31;
	}
LABEL_32:
	prevAction = monster->PrevActionOrRowDeltaOrSpellEffect;
	if( (prevAction == 1 || prevAction == 2 || prevAction == 3) && !timeFrame ){
		if (GameMode == GM_HARD) {
			maxIntel_1 = 100;
		}
		else if( Difficulty == 1 ){
			maxIntel_1 = 4 * monster->intel + 66;
		}
		else if (Difficulty >= 2) {
			maxIntel_1 = 4 * monster->intel + 83;
		}else{
			maxIntel_1 = 4 * monster->intel + 49;
		}
		if( random_100 < maxIntel_1 ){
			goto LABEL_38;
		}
	}
LABEL_39:
	if( monster->CurAction == A_0_STAY ){
		if (GameMode == GM_HARD) {
			attack_chance = 98;
		}
		else {
			attack_chance = ((4 * monster->intel) << Difficulty) + by(Difficulty, 4, 16, 23, 25, 27, 29, 31, 33, 35); // attack chances
		}
		if( abs( tarRowDist ) >= 2 || abs( tarColDist ) >= 2 || random_100 >= attack_chance) {
			monster->curAnimation = &monster->SpritePtr->animation[ANIM_0_STAY].oriented[ tarOrient ];
		}else{
			StartMonsterMeleeAttack( monsterIndex_1 );
		}
	}
}

//----- (00440ADA) --------------------------------------------------------
void __fastcall Monster_Fade_In(int monsterIndex, int orient, int stay)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < Monsters_Max_Count ){
		if( Monsters[monsterIndex].SpritePtr ){
			SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
			monster.CurAction = A_8_FADE_IN;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch(monsterIndex);
			monster.Orientation = orient;
			monster.flag &= ~MF_1_INVISIBLE;
			if (monster.newBossId) {
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
			}
			if( stay ){
				monster.flag |= MF_2_BACKWARDS;
				monster.CurFrame = monster.animFrameCount;
			}
		}
	}
}

//----- (00440B95) --------------------------------------------------------
void __fastcall Monster_Fade_Out(int monsterIndex, int orient, int stay)
{
	Monster& monster = Monsters[monsterIndex];
	if( (uint)monsterIndex < Monsters_Max_Count ){
		if( Monsters[monsterIndex].SpritePtr ){
			SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
			monster.CurAction = A_9_FADE_OUT;
			monster.DeltaX = 0;
			monster.DeltaY = 0;
			monster.NextRow = monster.Row;
			monster.NextCol = monster.Col;
			monster.PrevRow = monster.Row;
			monster.PrevCol = monster.Col;
			CheckMonsterUnderArch(monsterIndex);
			monster.Orientation = orient;
			if (monster.newBossId) {
				ChangeLightPos(monster.LightIndex, 0, 0);
			}
			if( stay ){
				monster.flag |= MF_2_BACKWARDS;
				monster.CurFrame = monster.animFrameCount;
			}
		}
	}
}

//----- (00440C3F) --------------------------------------------------------
void __fastcall Ai_17_Incinerator(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);

	if( monster.goal == MG_1_ATTACK ){
		if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) 
			&& CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_50_RUN_ATTACK_2, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1 ){
				monster.goalParam = 0;
				monster.CurAction = A_14_CHARGE;
				monster.goal = MG_5_ATTACK2;
		}
	}else if( monster.goal == MG_5_ATTACK2 ){
		if( monster.goalParam != 3 ){
			if( !CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				MonsterStartDelay(monsterIndex, RNG(10) + 5);
			}else{
				StartMonsterRangedAttack(monsterIndex, MI_51_KRULL, 4);
			}
			monster.goalParam++;
		}else{
			monster.goal = MG_1_ATTACK;
			Monster_Fade_Out(monsterIndex, orientationToTarget, 1);
		}
	}else{
		if( monster.goal == MG_2_RETREAT ){
			Monster_Fade_In(monsterIndex, orientationToTarget, 0);
			monster.goal = MG_5_ATTACK2;
		}
	}
	monster.Orientation = orientationToTarget;
	RNG(100);
	if( monster.CurAction == A_0_STAY ){
		return;
	}
	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) && monster.goal == MG_1_ATTACK ){
		MvP_Melee(monsterIndex, monster.TargetIndex, monster.ToHit, monster.MinDamage, monster.MaxDamage);
		monster.goal = MG_2_RETREAT;
		if( MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]) ){
			return;
		}
		Monster_Fade_In(monsterIndex, orientationToTarget, 0);
		monster.goal = MG_5_ATTACK2;
		return;
	}
	if( MonsterWalksTowardsTarget(monsterIndex, orientationToTarget) ){
		return;
	}
	if( monster.goal == MG_1_ATTACK || monster.goal == MG_2_RETREAT ){
		Monster_Fade_In(monsterIndex, orientationToTarget, 0);
		monster.goal = MG_5_ATTACK2;
		return;
	}
}

//----- (00440E5F) --------------------------------------------------------
void __fastcall Ai_8_Fallen(uint monsterIndex)
{
	uint monsterIndex_1; // edi@1
	uint monsterIndex_3; // esi@2
	int unknown2; // ecx@3
	int unknown2_1; // ecx@9
	int orient; // eax@10
	int ac; // ecx@14
	int minLife; // edx@47
	int radius; // ebx@15
	int colOfs; // edi@15
	int rowOfs; // edx@17
	int monNum; // eax@23
	int monIndex; // eax@24
	char state; // al@28
	int orient_1; // edx@29
	int monsterIndex_4; // ecx@29
	int colDif; // edi@31
	int orient_2; // eax@34
	int curAc; // [sp-4h] [bp-14h]@43
	int monsterIndex_2; // [sp+Ch] [bp-4h]@1

	monsterIndex_1 = monsterIndex;
	monsterIndex_2 = monsterIndex;
	if( monsterIndex < Monsters_Max_Count ){
		monsterIndex_3 = monsterIndex;
		if( Monsters[ monsterIndex ].goal == 5 ){
			unknown2 = Monsters[ monsterIndex_3 ].goalParam;
			if( unknown2 ){
				Monsters[ monsterIndex_3 ].goalParam = unknown2 - 1;
			}else{
				Monsters[ monsterIndex_3 ].goal = MG_1_ATTACK;
			}
		}
		if( !Monsters[ monsterIndex_3 ].CurAction && Monsters[ monsterIndex_3 ].ActivationCounter ){
			if( Monsters[ monsterIndex_3 ].goal == MG_2_RETREAT ){
				unknown2_1 = Monsters[ monsterIndex_3 ].goalParam;
				Monsters[ monsterIndex_3 ].goalParam = unknown2_1 - 1;
				if( !unknown2_1 ){
					orient = Monsters[ monsterIndex_3 ].Orientation;
					Monsters[ monsterIndex_3 ].goal = MG_1_ATTACK;
					FixMonsterPosition( monsterIndex_1, OrientInverted[ orient ] );
				}
			}
			if( Monsters[ monsterIndex_3 ].goal == MG_5_ATTACK2 || RNG(35) ){
				state = Monsters[ monsterIndex_3 ].goal;
				if( state == MG_2_RETREAT ){
					orient_1 = Monsters[ monsterIndex_3 ].Orientation;
					monsterIndex_4 = monsterIndex_1;
				}else{
					if( state != MG_5_ATTACK2 ){
						Ai_2_Skeleton_Melee( monsterIndex_1 );
						return;
					}
					colDif = Monsters[ monsterIndex_3 ].Col - Monsters[ monsterIndex_3 ].TargetCol;
					if( abs( Monsters[ monsterIndex_3 ].Row - Monsters[ monsterIndex_3 ].TargetRow ) < 2 && abs( colDif ) < 2 ){
						StartMonsterMeleeAttack( monsterIndex_2 );
						return;
					}
					orient_2 = MonsterOrientationToTarget( monsterIndex_2 );
					monsterIndex_4 = monsterIndex_2;
					orient_1 = orient_2;
				}
				MonsterWalksTowardsTarget( monsterIndex_4, orient_1 );
				return;
			}
			if( Monsters[ monsterIndex_3 ].flag & MF_4_NOHEAL ){
				goto LABEL_15;
			}
			StartMonsterSecondAttack_4_orBlock( monsterIndex_1, Monsters[ monsterIndex_3 ].Orientation );
			ac = Monsters[ monsterIndex_3 ].ArmorClass;
			if( (unsigned __int8) Difficulty < 1u ){
				if( Dungeon->level == 2 ){
					curAc = 9;
				}else if( Dungeon->level == 3 ){
					curAc = 12;
				}else{
					if( Dungeon->level != 4 ){
						goto LABEL_47;
					}
					curAc = 15;
				}
				ac = curAc;
			}
		LABEL_47:
			MonsterSprite monstersprite = *Monsters[monsterIndex_3].SpritePtr;
			int baseMonsterIndex = monstersprite.baseMonsterIndex; 
			minLife = (ac << 6) + Monsters[ monsterIndex_3 ].CurrentLife;
			if (baseMonsterIndex == BM_245_BLUNDERER) {  // Warden of Anger gets 3x life from warcries!
				minLife += (ac * 3) << 6;
			}
			LimitToMax(minLife, Monsters[monsterIndex_3].BaseLife + (Monsters[monsterIndex_3].BaseLife >> 2));
//			if( minLife < Monsters[ monsterIndex_3 ].BaseLife + (Monsters[ monsterIndex_3 ].BaseLife >> 2) ){
				Monsters[ monsterIndex_3 ].CurrentLife = minLife;
	//		}
		LABEL_15:
			radius = 2 * Monsters[ monsterIndex_3 ].intel + 4;
			for( colOfs = -radius; colOfs <= radius; ++colOfs ){
				for( rowOfs = -radius; rowOfs <= radius; ++rowOfs ){
					if( In112(colOfs, rowOfs) ){
						monNum = MonsterMap[ 0 ][ Monsters[ monsterIndex_3 ].Col + colOfs + 112 * (Monsters[ monsterIndex_3 ].Row + rowOfs) ];
						if( monNum > 0 ){
							monIndex = monNum - 1;
							if( Monsters[ monIndex ].ai == AI_8_Fallen_One ){
								Monsters[ monIndex ].goal = MG_5_ATTACK2;
								Monsters[ monIndex ].goalParam = 30 * Monsters[ monsterIndex_3 ].intel + 64;
							}
						}
					}
				}
			}
			return;
		}
	}
}

//----- (00441059) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_4_orBlock(int monsterIndex, int orient)
{
	Monster& monster = Monsters[monsterIndex];
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_11_SPELL_STAND;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (004410D7) --------------------------------------------------------
void __fastcall Ai_13_Butcher(uint monsterIndex)
{
	Monster *monster; // esi@2
	int row; // ecx@4
	int col; // edx@4
	int rowDif; // edi@4
	int colDif; // ebp@4
	int orient; // ebx@4
	
	if( monsterIndex < Monsters_Max_Count ){
		monster = &Monsters[ monsterIndex ];
		if( monster->CurAction == A_0_STAY ){
			if( monster->ActivationCounter ){
				row = monster->Row;
				col = monster->Col;
				rowDif = row - monster->TargetRow;
				colDif = col - monster->TargetCol;
				orient = OrientationToTarget( row, col, monster->LastTargetPositionRow, monster->LastTargetPositionCol );
				monster->Orientation = orient;
				if( abs( rowDif ) >= 2 || abs( colDif ) >= 2 ){
					MonsterWalksTowardsTarget( monsterIndex, orient );
				}else{
					StartMonsterMeleeAttack( monsterIndex );
				}
				if( monster->CurAction == A_0_STAY ){
					monster->curAnimation = &monster->SpritePtr->animation[ANIM_0_STAY].oriented[ orient ];
				}
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Ai_53_Exploder(uint monsterIndex)
{
	if( monsterIndex < Monsters_Max_Count ){
		Monster& monster = Monsters[monsterIndex];
		if( monster.CurAction == A_0_STAY ){
			if( monster.ActivationCounter ){
				int row = monster.Row;
				int col = monster.Col;
				int rowDif = row - monster.TargetRow;
				int colDif = col - monster.TargetCol;
				int orient = OrientationToTarget(row, col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
				monster.Orientation = orient;
				if (abs(rowDif) >= 2 || abs(colDif) >= 2) {
					MonsterWalksTowardsTarget(monsterIndex, orient);
				}
				else {
					int playerIndex = -1;
					if( monster.flag & MF_5_TARGET_MONSTER && monster.TargetIndex < SummonMonsters_Count ){ // target is monster
						playerIndex = monster.TargetIndex / SummonMonstersPerPlayer_Count;
					}else{ // target is player
						playerIndex = monster.TargetIndex;
					}
					if( playerIndex >= 0){
						KillMonsterByPlayer(monsterIndex, playerIndex);
					}
					//StartMonsterAttack(monsterIndex);
				}
				if (monster.CurAction == A_0_STAY) {
					monster.curAnimation = &monster.SpritePtr->animation[ANIM_0_STAY].oriented[orient];
				}
			}
		}
	}
}

//----- (0044117C) --------------------------------------------------------
void __fastcall AttackTypeMelee(int monsterIndex, int hasSecondAttack)
{
	if( (unsigned int)monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);

	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) || monster.ActivationCounter != 255
	 || TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
		monster.goal = MG_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.goal != MG_4_WALK && ((flag = !Distance_To_Target_Is_Less_Than(monsterIndex, 4)) != 0 && ! RNG(10) ) ){
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);
		}
		if( monster.goal == MG_4_WALK || flag ){
			monster.goal = MG_4_WALK;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.goalParam++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget)
			 || TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.goal = MG_1_ATTACK;
			}else if( !MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterStartDelay(monsterIndex, RNG(10) + 10);
			}
		}
	}

	if( monster.goal == MG_1_ATTACK ){
		if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( monster.TimerCounter > by(Difficulty, 22, 10, 1) && randFactor < by(Difficulty, 2 * monster.intel + (GameMode == GM_HARD ? 50 : 20),
																								2 * monster.intel + (GameMode == GM_HARD ? 55 : 40),
																								2 * monster.intel + 60 )
			 || isAfterWalk && !monster.TimerCounter && randFactor < by(Difficulty, monster.intel + 70,
																					monster.intel + 80,
																					monster.intel + 90) )
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
		}else if( randFactor < by( Difficulty,	2 * monster.intel + (GameMode == GM_HARD ? 45 : 20),
												4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
			4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
				4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
			4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
				4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
			4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
				4 * monster.intel + (GameMode == GM_HARD ? 50 : 35),
												4 * monster.intel + 55 ) ){
			monster.Orientation = orientationToTarget;
			if( !hasSecondAttack || monster.CurrentLife >= monster.BaseLife >> 1 || !RNG(2) ){
				StartMonsterMeleeAttack(monsterIndex);
			}else{
				StartMonsterSecondMeleeAttack(monsterIndex);
			}
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044140E) --------------------------------------------------------
void __fastcall Ai_6_Goat_Melee(uint monsterIndex)
{
	AttackTypeMelee(monsterIndex, 1);
}

//----- (00441416) --------------------------------------------------------
void __fastcall RangerRetreatsOften(int monsterIndex, int missileIndex, int isSecondAttack)
{
	if( (unsigned int)monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	
	bool isTargetVisible = false;
	if( monster.ActivationCounter ){ // prevent monsters from attack without activation
        if( monster.flag & MF_5_TARGET_MONSTER ){
            auto deltaRow = abs( monster.TargetCol - monster.Col );
            auto deltaCol = abs( monster.TargetRow - monster.Row );
            auto maxDistance = std::max( deltaRow, deltaCol );
            if( maxDistance < 15 ){
                isTargetVisible = true;
            }
        }else{
            if( monster.ActivationCounter == 255 ){ // direct stay within player's light radius
                isTargetVisible = true;
            }
        }
    }
    
	if( isTargetVisible ){
		int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
		if( monster.ActivationCounter < 255 ){
			MonstersInteractWithDoors(monsterIndex);
		}
		monster.Orientation = orientationToTarget;

		int distance_of_retreat = by(Difficulty, 3, 4, 5, 5, 5, 5, 5, 5, 5);
		if (GameMode == GM_HARD) {
			distance_of_retreat += 1;
		}
		/*else if (is(GameMode, GM_EASY, GM_IRONMAN)) {
			distance_of_retreat -= 1;
		}
		LimitToMin(distance_of_retreat, 2);*/

		int attack_delays = by(Difficulty, 36, 16, 14, 13, 11, 9, 7, 5, 3);
		if (GameMode == GM_HARD) {
			attack_delays = by(Difficulty, 4, 3, 2, 2, 2, 2, 2, 2, 2);
		}
		else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
			attack_delays = by(Difficulty, 48, 45, 42, 39, 36, 33, 30, 27, 24);
		}

		int retreat_chances = 2 * monster.intel // range: 0-3 (0, 2, 4, 6)
							+ 70
							+ (Difficulty * 3) // 70-76, 73-79, 76-82, 79-85, 82-88, 85-91, 88-94, 91-97, 94-100
							- (MaxCountOfPlayersInGame == 1 ? 5 : 0);
		if (GameMode == GM_CLASSIC) {
			retreat_chances = monster.intel * 10 + 70;// original Diablo retreat settings restored
		}
		else if (GameMode == GM_EASY) {
			retreat_chances -= 15;
		}
		else if (GameMode == GM_IRONMAN || GameMode == GM_SPEEDRUN || GameMode == GM_SURVIVAL) {
			retreat_chances -= 5;
		}
		else if (GameMode == GM_HARD) {
			retreat_chances = 100;
		}

		// monsters who don't retreat >>>>
		if (monster.SpritePtr->baseMonsterIndex == BM_259_WATER_WYRM || monster.SpritePtr->baseMonsterIndex == BM_263_RED_WYRM || monster.SpritePtr->baseMonsterIndex == BM_289_FUNGAL_ROOT
			|| monster.SpritePtr->baseMonsterIndex == BM_290_FUNGAL_ROOT_LITE
			|| (Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT))) { // no retreat
			retreat_chances = 0;
		}
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN) && (is(monster.SpritePtr->baseMonsterIndex, 
			BM_20_RISEN_SKELETON, BM_21_SKELETON_WARRIOR, BM_22_BURNING_DEAD, BM_23_MALICE, 
			BM_14_SOUL_HUNTER, BM_98_IMMORTAL, BM_114_FROZEN_HORROR, BM_119_DARK_GHOST, BM_129_LICH, BM_170_LICH, BM_220_SNOW_LICH))) {
			retreat_chances = 0; // undead shooters don't run from Night Kin
		}
		// old code for action after shooting >>>>>>>>>
		/*if( monster.PrevAction == A_10_RANGED ){
			MonsterSetAct_13(monsterIndex, RNG(attack_delays));
		}else if( Distance_To_Target_Is_Less_Than(monsterIndex, distance_of_retreat )
		 && RNG(100) < retreat_chances){
			MoveInOrientOrAbove_2(monsterIndex, OrientInverted[orientationToTarget]);
		}*/	

		if (monster.PrevAction == A_10_RANGED ) { // monster retreats and (is not a summon - cancelled, removes attack delays)
			int circling_chance = 10 + (Difficulty * 4) + (Dungeon->level / 7) ; // 10-13, 14-17, 18-21, 22-25, 26-29, 30-33, 34-37, 38-41, 42-45
			if (GameMode == GM_HARD) {
				circling_chance += 10; // 20 - 55
			}
			else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				circling_chance /= 2; // 5 - 22
			}
			else if (GameMode == GM_IRONMAN || GameMode == GM_SPEEDRUN) {
				circling_chance -= 7; // 3 - 38
			}
			if (MaxCountOfPlayersInGame != 1) { // mp hard: 62-65 (it will be hell...)
				circling_chance += 10;
			}
			// monsters who don't circle around target >>>
			if (monster.SpritePtr->baseMonsterIndex == BM_259_WATER_WYRM || monster.SpritePtr->baseMonsterIndex == BM_263_RED_WYRM || monster.SpritePtr->baseMonsterIndex == BM_289_FUNGAL_ROOT
				|| monster.SpritePtr->baseMonsterIndex == BM_290_FUNGAL_ROOT_LITE || GameMode == GM_CLASSIC) {
				circling_chance = 0;
			}
			// Stay if we don't want to circle, or if we want, but can't circle
			if (!(RNG(100) < circling_chance && monsterIndex >= SummonMonsters_Count) 
				|| !MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation)) {
				MonsterStartDelay(monsterIndex, RNG(attack_delays));
			}
		}
		else if (Distance_To_Target_Is_Less_Than(monsterIndex, distance_of_retreat)
			&& RNG(100) < retreat_chances) {
			MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]);
		}
		
		if( !monster.CurAction ){
			if( CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				if( isSecondAttack ){
					StartMonsterSecondRangedAttack(monsterIndex, missileIndex, 4);
				}else{
					StartMonsterRangedAttack(monsterIndex, missileIndex, 4);
				}
			}else{
				monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
			}
		}
	}
	else if (monster.ActivationCounter) {	
		if (monster.SpritePtr->baseMonsterIndex != BM_259_WATER_WYRM && monster.SpritePtr->baseMonsterIndex != BM_263_RED_WYRM && monster.SpritePtr->baseMonsterIndex != BM_289_FUNGAL_ROOT
			&& monster.SpritePtr->baseMonsterIndex != BM_290_FUNGAL_ROOT_LITE) {
			int orientation = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
			MonsterWalksTowardsTarget(monsterIndex, orientation);
		}
	}
}

void __fastcall Ai_56_Rammer(uint monsterIndex)
{
	//---------------------------------------------------------------charger ai---------------------------------------------------------------------
	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	
	if (!Distance_To_Target_Is_Less_Than(monsterIndex, 7)) {// if distance to target >= 6
		Ai_2_Skeleton_Melee(monsterIndex); // use Skeleton AI 
		return;
	}

	if (monster.CurAction || !monster.ActivationCounter) {
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);// distance to player
	int deltaCol = abs(monster.Col - monster.TargetCol);

	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if (monster.ActivationCounter < 255) {// TODO : всегда выполняется
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);
	int int_F = monster.intel;
	bool is_charge_allowed = randFactor < by(Difficulty, 3 * int_F + (GameMode == GM_HARD ? 70 : 23), 3 * int_F + (GameMode == GM_HARD ? 75 : 50), 2 * int_F + 80);
	bool is_HtH_allowed = randFactor < by(Difficulty, 2 * int_F + (GameMode == GM_HARD ? 50 : 4), 4 * int_F + (GameMode == GM_HARD ? 60 : 26), 2 * int_F + 70);
	//bool is_HtH_allowed = false;

	if (Distance_To_Target_Is_Less_Than(monsterIndex, 6)) {
		monster.goal = MG_1_ATTACK;
	}
	else {  //                  walk procedure
		bool walk_allowed = false;
		if (monster.goal != MG_4_WALK && (walk_allowed = !Distance_To_Target_Is_Less_Than(monsterIndex, 4) && RNG(4)) != 0) {// circling radius
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);// randomly chosen walk orientation (left or right)
		}
		if (monster.goal == MG_4_WALK || walk_allowed) {
			monster.goal = MG_4_WALK;
			int maxDelta = max(deltaCol, deltaRow);
			if (monster.goalParam++ >= 2 * maxDelta || TransMap[monster.Row][monster.Col] != TransMap[monster.TargetRow][monster.TargetCol]) {
				monster.goal = MG_1_ATTACK;
			}
			else if (!MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation)) {
				MonsterStartDelay(monsterIndex, RNG(10) + 10);
			}
		}
	}
	if (monster.goal == MG_1_ATTACK) {
		if (!Distance_To_Target_Is_Less_Than(monsterIndex, 4) && is_charge_allowed // charge range minimum = 4
			&& CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)) { // атака с разбегу
			if (CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1) {
				if (monster.BasePtr->HasSpecialSounds) {
					PlayMonsterSound(monsterIndex, MST_3_SATTACK);
				}
				MonsterMap[monster.Row][monster.Col] = -1 - int(monsterIndex);
				monster.CurAction = A_14_CHARGE;
			}
			return;
		}
		if (!Distance_To_Target_Is_Less_Than(monsterIndex, 2)) {// not in melee range?
			randFactor = RNG(100);
			int walk_delay_chance = by(Difficulty, 2 * int_F + 32, 2 * int_F + 56, 2 * int_F + 80);
			if (GameMode == GM_HARD) {
				walk_delay_chance = 98;
			}
			bool walkWithDelayAllow = randFactor < walk_delay_chance;
			int continued_walking = 2 * monster.intel + 83;
			if (GameMode == GM_HARD) {
				continued_walking = 98;
			}
			bool walkWithoutDelayAllow = randFactor < continued_walking;
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if (walkWithDelayAllow || (isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow)) {
				MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]); 
				//Monster_Walks_Towards_Target(monsterIndex, orientationToTarget);
			}
			else {
				int another_attack_chance_modifier = by(Difficulty, 14, 11, 8, 6, 4, 3, 2, 1, 0);
				if (GameMode == GM_HARD) {
					another_attack_chance_modifier = 0;
				}
				MonsterStartDelay(monsterIndex, RNG(10) + another_attack_chance_modifier);
			}
			return;
		}
		if (is_HtH_allowed ) {
			monster.Orientation = orientationToTarget;
			StartMonsterMeleeAttack(monsterIndex);
			return;
		}
		if (Distance_To_Target_Is_Less_Than(monsterIndex, 4) 
			&& RNG(100) < 33 /*retreat_chances*/) { 
			MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]); // retreat
		}
	}
	if (!monster.CurAction) {
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00441593) --------------------------------------------------------
int __fastcall StartMonsterSecondRangedAttack(int monsterIndex, int spellEffect, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	int orient = MonsterOrientationToTarget(monsterIndex);
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], orient);
	monster.CurAction = A_12_SPELL_ATTACK;
	monster.PrevActionOrRowDeltaOrSpellEffect = spellEffect;
	monster.TimerCounterOrColDeltaOrSpellDamage = 0;
	monster.ActionRow_ActOrient = damage;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	monster.Orientation = orient;
	CheckMonsterUnderArch(monsterIndex);
	return 0;
}

//----- (00441634) --------------------------------------------------------
void __fastcall Ai_7_Goat_Archer(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_0_NONE_ARROW, 0);
}

//----- (new) --------------------------------------------------------
void __fastcall Ai_41_RangerRetreatsOften_ColdSnakes(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_129_SNOW_BOLT, 0);
}

//----- (0044163E) --------------------------------------------------------
void __fastcall Ai_14_RangerRetreatsOften_ArcaneStar(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex]; 
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex]; 
	int missile;
	if (monster.SpritePtr->baseMonsterIndex == BM_212_CACODEMON) {
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
			int rnd_missile = RNG(2);
			switch (rnd_missile) {
			case 1: missile = MI_101_YELOW_BLAST; break;
			default: missile = MI_100_RED_BLAST; break;
			}
		}
		else {
			missile = MI_7_LIGHTNING;
		}
	}
	else  {
		missile = MI_24_ARCANE_STAR;
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
			missile = MI_131_ARCANE_STAR_BLUE;
		}
		else if (GameMode == GM_CLASSIC && monster.SpritePtr->baseMonsterIndex == BM_C_322_SNOW_WITCH) {
			missile = MI_131_ARCANE_STAR_BLUE;
		}
	}
	RangerRetreatsOften(monsterIndex, missile, 0);
}

void __fastcall Ai_46_RangerRetreatsOften_ArcaneStarBlue(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_131_ARCANE_STAR_BLUE, 0);
}

//----- (00441649) --------------------------------------------------------
void __fastcall Ai_35_RangerRetreatsOften_LichOrangeBlast(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_98_LICH_ORA_BLAST, 0);
}

//----- (00441654) --------------------------------------------------------
void __fastcall Ai_36_RangerRetreatsOften_YelowBlast(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_101_YELOW_BLAST, 0);
}

//----- (0044165F) --------------------------------------------------------
void __fastcall Ai_37_RangerRetreatsOften_FireArrow(uint monsterIndex)
{
	int x = MI_27_FIRE_ARROW;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		x = MI_126_COLD_ARROW;
	}
	RangerRetreatsOften(monsterIndex, x /*MI_99_BLUE_BLAST*/, 0);
}

void __fastcall Ai_43_RangerRetreatsOften_Acid_Arrow(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_109_ACID_ARROW, 0);
}

void __fastcall Ai_44_RangerRetreatsOften_Cold_Arrow(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_126_COLD_ARROW, 0);
}

void __fastcall Ai_45_RangerRetreatsOften_Arcane_Arrow(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_108_ARCANE_ARROW, 0);
}

void __fastcall Ai_49_RangerRetreatsOften_Fireblast(uint monsterIndex)
{
	//int jestering = RngFrom(MI_6_FIREBLAST, MI_127_HABIBI, MI_21_MAGMA_BALL, MI_24_ARCANE_STAR, MI_49_INCINERATE);
	RangerRetreatsOften(monsterIndex, MI_6_FIREBLAST, 0);
}

void __fastcall Ai_55_RangerRetreatsOften_Wyrm(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_57_ACID_MISSILE, 0);
}

//----- (0044166A) --------------------------------------------------------
void __fastcall Ai_38_RangerRetreatsOften_RedBlast(uint monsterIndex)
{
	int missile;
	if( Dungeon->level >= 6 ){
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
			missile = MI_102_BLUE_BLAST_2;
		}
		else {
			missile = MI_100_RED_BLAST;
		}
	}else{
		missile = MI_52_CHARGED_BOLT;
	}
	RangerRetreatsOften(monsterIndex, missile, 0);
}

//----- (00441675) --------------------------------------------------------
void __fastcall Ai_20_RangerRetreatsOften_AcidMissile(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_57_ACID_MISSILE, 1);
}

//----- (00441680) --------------------------------------------------------
void __fastcall Ai_32_RangerRetreatsOften_Firebolt(uint monsterIndex)
{
	int mis_type = MI_1_FIREBOLT;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		mis_type = MI_129_SNOW_BOLT;
	}
	RangerRetreatsOften( monsterIndex, mis_type, 0 );	
}

//----- (0044168B) --------------------------------------------------------
void __fastcall Ai_33_RangerRetreatsOften_LightningArrow(uint monsterIndex)
{
	RangerRetreatsOften(monsterIndex, MI_56_LIGHTING_ARROW, 0);
}

//----- (00441696) --------------------------------------------------------
void __fastcall Ai_4_Scavenger(uint monsterIndex)
{
	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if (monster.CurAction) {
		return;
	}

	if (monster.CurrentLife >= 2 * monster.BaseLife / 5 // if HP > 40% 
		|| Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT) // or with Full Contact GC	
		/*|| RNG(1000) != 666 */){
		if( monster.goal != MG_3_EAT ){ // and not digging corpse
			Ai_2_Skeleton_Melee(monsterIndex); // keep attacking in melee
			return;
		}
	}else if( monster.goal != MG_3_EAT ){ // if life < 40% and not digging
		if( monster.isBossPack ){
			Monsters[monster.hisBossIndex].BossPackCount--;
			monster.isBossPack = 0;
		}
		monster.goal = MG_3_EAT; // dig corpse
		monster.mayRetreateTick = 10; // what is that?
	}
	// ищем труп (finding corpse)
	if( !monster.mayRetreateTick ){
		Ai_2_Skeleton_Melee(monsterIndex);
		return;
	}
	monster.mayRetreateTick--;
	if( DeathMonstersMap[ monster.Row ][ monster.Col ] ){
		// уже стоим на трупе
		StartMonsterSecondAttack_1_2(monsterIndex);
		if( !(monster.flag & MF_4_NOHEAL) ){
			int healedPoints = 1;// = monster.BaseLife >> 4; // /16, раньше было /8
			if (GameMode == GM_IRONMAN || GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN) {
				healedPoints = monster.BaseLife / 50;
			}
			else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				healedPoints = monster.BaseLife / 60;
			}
			else{
				healedPoints = monster.BaseLife / 40;// Hard, Normal, Survival
			}
			LimitToMin(healedPoints, 1);
			monster.CurrentLife += healedPoints;
			if( monster.CurrentLife > monster.BaseLife ){
				monster.CurrentLife = monster.BaseLife;
			}
			if( monster.mayRetreateTick <= 0 || monster.CurrentLife == healedPoints ){
				DeathMonstersMap[ monster.Row ][ monster.Col ] = 0;
			}
		}
		if( monster.CurrentLife == monster.BaseLife ){
			monster.goal = MG_1_ATTACK;
			monster.goalParam = 0;
			monster.curOrientation = 0;
		}
	}else{
		if( !monster.goalParam ){
			// ищем труп
			int isCorpseFound = 0;
			int rowOffset, colOffset;
			int dir = RNG(2) * 2 - 1; // -1 / 1 (up or down direction)
			rowOffset = isCorpseFound;
			int radius = 4;
			int offsetStart = -radius * dir, offsetEnd = radius * dir;
			for( colOffset = offsetStart; colOffset <= offsetEnd && !isCorpseFound; colOffset += dir ){
				for( rowOffset = offsetStart; rowOffset <= offsetEnd && !isCorpseFound; rowOffset += dir ){
					int corpseRow = monster.Row + rowOffset, corpseCol = monster.Col + colOffset;
					if( In112(corpseRow, corpseCol) ){
						isCorpseFound = DeathMonstersMap[ corpseRow ][ corpseCol ] && CheckLineWithTwoArgumentsCheckFunction(IsCellNotBlockWalking, monster.Row, monster.Col, corpseRow, corpseCol);
					}
				}
			}
			rowOffset -= dir;// компенсация лишней прибавки перел выходом из цикла
			colOffset -= dir;
			if( isCorpseFound ){
				monster.goalParam = rowOffset + monster.Row + 1;
				monster.curOrientation = colOffset + monster.Col + 1;
			}
		}
		// идем к трупу
		if( monster.goalParam ){
			int orientation = OrientationToTarget(monster.Row, monster.Col, monster.goalParam - 1, monster.curOrientation - 1);
			monster.Orientation = orientation;
			MonsterWalksTowardsTarget(monsterIndex, orientation);
		}
	}
}

//----- (00441973) --------------------------------------------------------
void __fastcall StartMonsterSecondAttack_1_2( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[ANIM_5_SECOND], monster.Orientation);
	monster.CurAction = A_7_SECOND;
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	monster.NextRow = monster.Row;
	monster.NextCol = monster.Col;
	monster.PrevRow = monster.Row;
	monster.PrevCol = monster.Col;
	CheckMonsterUnderArch(monsterIndex);
}

//----- (004419EC) --------------------------------------------------------
void __fastcall Ai_12_Gargoyle(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	int stone_distance(5 + Dungeon->level / 9 + Difficulty);
	LimitToMax(stone_distance, 11); // capped on diff#5
	if (GameMode == GM_HARD) {
		stone_distance += 2;
	}
	else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
		stone_distance -= 1;
	}
	if( monster.ActivationCounter && monster.flag & MF_3_STONE ){
		MonsterLookAround(monsterIndex);
		if( Distance_To_Target_Is_Less_Than(monsterIndex, /*monster.intelligenceFactor + */ stone_distance) ){
			monster.flag &= ~MF_3_STONE;
		}
		return;
	}
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	if( monster.CurrentLife < monster.BaseLife / 2 ){
		int garg_retreat_chance = 75;
		switch (GameMode) {
		case GM_EASY:
		case GM_CLASSIC:	garg_retreat_chance = 25; break;
		case GM_NORMAL:		garg_retreat_chance = 35; break;
		case GM_HARD:		garg_retreat_chance = 20; break;
		case GM_IRONMAN:
		case GM_SPEEDRUN:
		case GM_HARDCORE:	garg_retreat_chance =  60; break;
		case GM_SURVIVAL:	garg_retreat_chance =  50; break;
		case GM_NIGHTMARE:	garg_retreat_chance =  40; break;
		}
		garg_retreat_chance -= Difficulty * 2;
		if ((Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT)) || RNG(garg_retreat_chance) != 1) {
			// skip retreating 
		}
		else {
			monster.goal = MG_2_RETREAT;
		}
	}
	if( monster.goal == MG_2_RETREAT ){
		int deltaRow = abs(monster.Row - monster.LastTargetPositionRow);
		int deltaCol = abs(monster.Col - monster.LastTargetPositionCol);
		
		if( !Max_Distance_To_Target_Last_Position(monsterIndex, /*monster.intelligenceFactor + stone_distance*/ 4) ){
			monster.goal = MG_1_ATTACK;
			StartMonsterStoneMode(monsterIndex);
		}else if( !MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]) ){
			monster.goal = MG_1_ATTACK;
		}
	}
	AttackTypeMelee(monsterIndex, 0);
}

//----- (00441B0F) --------------------------------------------------------
void __fastcall StartMonsterStoneMode(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( !monster.SpritePtr ){
		return;
	}
	monster.curAnimation = &monsterSprite.animation[ANIM_5_SECOND].oriented[monster.Orientation];
	monster.flag |= MF_2_BACKWARDS;
	monster.CurFrame = monsterSprite.animation[ANIM_5_SECOND].frameCount;
	monster.CurAction = 16;
	monster.PrevActionOrRowDeltaOrSpellEffect = monster.BaseLife / (32 * (RNG(5) + 4));// gargoyle life regen speed in stone???
}

//----- (00441B82) --------------------------------------------------------
void __fastcall Universal_RangeSecondAttack(uint monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility)
{
	Monster& mon = Monsters[ monsterIndex ];
	if( monsterIndex >= Monsters_Max_Count || mon.CurAction != A_0_STAY || ! mon.ActivationCounter ) return;

	int range = max(abs(mon.Row - mon.TargetRow), abs(mon.Col - mon.TargetCol));
	int dir = OrientationToTarget( mon.Row, mon.Col, mon.LastTargetPositionRow, mon.LastTargetPositionCol );
	if( canInteractWithObjects && mon.ActivationCounter < 255 ){
		MonstersInteractWithDoors( monsterIndex );
	}
	int rng_10000 = RNG(10000);
	if( range < 2 || mon.ActivationCounter != 255 || TransMap[ mon.Row ][ mon.Col ] != TransMap[ mon.TargetRow ][ mon.TargetCol ] ){
		mon.goal = MG_1_ATTACK;
	}else if( mon.goal == MG_4_WALK || (range >= 3 && RNG(4 << agility) == 0) ){
		if( mon.goal != MG_4_WALK ){
			mon.goalParam = 0;
			mon.curOrientation = RNG(2);
		}
		mon.goal = MG_4_WALK;
		if( mon.goalParam++ >= 2 * range && MayMoveMonsterToDirection( monsterIndex, dir ) ){
			mon.goal = MG_1_ATTACK;	
		}else if( rng_10000 < 200 * (mon.intel + 1) >> agility && CheckCastInSight( mon.Row, mon.Col, mon.TargetRow, mon.TargetCol ) ){
			StartMonsterSecondRangedAttack( monsterIndex, spellEffect, damage );
		}else{
			MonsterCircleTarget( monsterIndex, dir, mon.curOrientation );
		}
	}
	if( mon.goal == MG_1_ATTACK ){
		if( (range >= 3 && rng_10000 < 500 * (mon.intel + 2) >> agility || rng_10000 < 500 * (mon.intel + 1) >> agility)
		 && CheckCastInSight( mon.Row, mon.Col, mon.TargetRow, mon.TargetCol ) ){
			StartMonsterSecondRangedAttack( monsterIndex, spellEffect, damage );
		}else if( range >= 2 ){
			int rng_100 = RNG(100); // TODO: looks like TH1 hack (100 instead of 10000), rng_100 < 1000 condtition always true
			if( rng_100 < 1000 * (mon.intel + 5)
			 || is(mon.PrevAction, A_1_WALK_UP, A_2_WALK_DOWN, A_3_WALK_HORIZ)
			 && !mon.TimerCounterOrColDeltaOrSpellDamage
			 && rng_100 < 1000 * (mon.intel + 8) ){
				MonsterWalksTowardsTarget( monsterIndex, dir );
			}
		}else{
			if( rng_10000 < 250 * (mon.intel + by(Difficulty, 24, 30, 36)) ){
				mon.Orientation = dir;
				StartMonsterMeleeAttack( monsterIndex );
			}
		}
	}
	if( mon.CurAction == A_0_STAY ){
		int delay = RNG(10) + by(Difficulty, 20, 6, 0);
		MonsterStartDelay( monsterIndex, delay );
	}
}

//----- (00441E82) --------------------------------------------------------
void __fastcall Ai_9_Universal_RangeSecondAttack_MagmaBall(uint monsterIndex)
{
	int missileIndex; // [sp-10h] [bp-10h]@6
	int damage; // [sp-8h] [bp-8h]@5

	if (Dungeon->level < 17 || Dungeon->level > 20) {
		damage = 4; // what? damage 4 ??
		missileIndex = MI_21_MAGMA_BALL;
	}
	else {
		damage = 75 + 95 * Difficulty;
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
			missileIndex = MI_129_SNOW_BOLT;
		}
		else {
			missileIndex = MI_6_FIREBLAST;
		}
	}
	Universal_RangeSecondAttack(monsterIndex, missileIndex, 1, damage, 0);
}

//----- (th3) -------------------------------------------------------------
void __fastcall Ai_50_Universal_RangeSecondAttack_Fireblast(uint monsterIndex)
{
	int damage = 13 + Dungeon->level * 3 + Difficulty * 85; // this doesnt seem to work, needs check!	
	Universal_RangeSecondAttack(monsterIndex, MI_6_FIREBLAST, 1, damage, 0);
}

//----- (00441E91) --------------------------------------------------------
void __fastcall Ai_16_Universal_RangeSecondAttack_Lightning(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_22_MONSTER_LIGHTNING, 1, 4, 0);
}

//----- (00441EA0) --------------------------------------------------------
void __fastcall Ai_39_Universal_RangeSecondAttack_Physical_Blue_Blast(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_102_BLUE_BLAST_2, 1, 4, 0);
}

//----- (new) --------------------------------------------------------
void __fastcall Ai_40_Universal_RangeSecondAttack_ColdBlast(uint monsterIndex)
{	
	int missile = MI_127_HABIBI;
	if (Dungeon == DUN_52_MOSSY_GROTTO) {
		missile = RngFrom(MI_1_FIREBOLT, MI_24_ARCANE_STAR, MI_52_CHARGED_BOLT);
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

//----- (new) --------------------------------------------------------
void __fastcall Ai_52_Universal_RangeSecondAttack_SnowBolt(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_129_SNOW_BOLT, 1, 4, 0);
}

//----- (Doom_Imp) --------------------------------------------------------
void __fastcall Ai_42_Universal_RangeSecondAttack_Firebolt(uint monsterIndex)
{
	int mis_type = MI_1_FIREBOLT;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		mis_type = MI_129_SNOW_BOLT;
	}
	Universal_RangeSecondAttack(monsterIndex, mis_type/*MI_129_DOOM_IMP*/, 1, 4, 0);
}

//----- (Lord_Obese) --------------------------------------------------------
void __fastcall Ai_47_Universal_RangeSecondAttack_Bluestar(uint monsterIndex)
{
	int missile(MI_100_RED_BLAST);
	if (Dungeon == DUN_9_CAVE_1 && MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		missile = MI_129_SNOW_BOLT; // only Lord Obese uses this attack on dlvl-9
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

//----- (Fearshredder) --------------------------------------------------------
void __fastcall Ai_51_Universal_RangeSecondAttack_Arcanestar(uint monsterIndex)
{
	int missile(MI_24_ARCANE_STAR);
	if (MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		missile = MI_131_ARCANE_STAR_BLUE;
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

void __fastcall Ai_58_Universal_RangeSecondAttack_Psychic(uint monsterIndex)
{
	int missile(MI_143_BULLET);
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

void __fastcall Ai_57_Hybrid_ArcaneStar(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex]; 
	int missile(MI_24_ARCANE_STAR);
	if (MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		missile = MI_131_ARCANE_STAR_BLUE;
	}
	if (not(GameMode, GM_EASY, GM_CLASSIC/*, GM_IRONMAN*/) && monster.CurrentLife < monster.BaseLife / 2) {
		Ai_14_RangerRetreatsOften_ArcaneStar(monsterIndex);
	}
	else {
		Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
	}
}

//----- (Vobul) --------------------------------------------------------
void __fastcall Ai_48_Universal_RangeSecondAttack_Yellowstar(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_101_YELOW_BLAST, 1, 4, 0);
}

//----- (00441EAF) --------------------------------------------------------
void __fastcall Ai_19_Universal_RangeSecondAttack_AcidMissile(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if (monster.SpritePtr->baseMonsterIndex == BM_230_BEASTMASTER_SUMMON_2) {
		goto REGULAR;
	}
	else if (not(GameMode, GM_EASY, GM_CLASSIC/*, GM_IRONMAN*/) && monster.CurrentLife < monster.BaseLife / 3) {
		RangerRetreatsOften(monsterIndex, MI_57_ACID_MISSILE, 1);
	}
	else {
		REGULAR:
		Universal_RangeSecondAttack(monsterIndex, MI_57_ACID_MISSILE, 0, 4, 1);
	}
}

//----- (Scarab) --------------------------------------------------------
void __fastcall Ai_54_Universal_RangeSecondAttack_Charged_Bolt(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_52_CHARGED_BOLT, 1, 4, 0);
}

//----- (00441EBE) --------------------------------------------------------
void __fastcall Ai_27_Universal_RangeSecondAttack_Apoc(uint monsterIndex)
{
	// вставить новый код
	int damage; // [sp-8h] [bp-8h]@4
	int interact = 0; // [sp-Ch] [bp-Ch]@5
	int missileIndex = MI_67_MONSTER_APOC; // [sp-10h] [bp-10h]@5
	
	switch( Difficulty ){
	case 0:		damage = 110 + RNG(21);	break;
	case 1:		damage = 200 + RNG(41);	break;	
	default:	damage = 200 + Difficulty * 50 + RNG(Difficulty * 25 + 41);
		if (Dungeon->level == 24) {
			int rnd30 = RNG(30);
			if (rnd30 < 4) {
				damage = 400 + Difficulty * 150; interact = 1;	missileIndex = MI_6_FIREBLAST;
			}
			else if (rnd30 == 4) {
				damage = 650 + Difficulty * 150; missileIndex = MI_1_FIREBOLT;
			}
			else if (rnd30 > 25) {
				damage = 150 + Difficulty * 100; missileIndex = MI_22_MONSTER_LIGHTNING;
			}
			else {
				damage = 250 + Difficulty * 80 + RNG(/*Difficulty * 10 +*/ Dungeon->level * 3);// 2: 410-480, 3: 490-560, 4: 570-640, 5: 650-720, 6: 730-800, 7: 810-880, 8: 890-960
			}
		} break; 
	}	
	if (GameMode == GM_CLASSIC) { damage = Difficulty * 20 + 40; }
	Universal_RangeSecondAttack( monsterIndex, missileIndex, interact, damage, 0 );
}


//----- (00441B82) --------------------------------------------------------
void __fastcall Universal_OnlyRangeSecondAttack(uint monsterIndex, int spellEffect, int canInteractWithObjects, int damage, int agility)
{
	Monster& mon = Monsters[ monsterIndex ];
	if( monsterIndex >= Monsters_Max_Count || mon.CurAction != A_0_STAY || ! mon.ActivationCounter ) return;

	int range = max(abs(mon.Row - mon.TargetRow), abs(mon.Col - mon.TargetCol));
	int dir = OrientationToTarget( mon.Row, mon.Col, mon.LastTargetPositionRow, mon.LastTargetPositionCol );
	if( canInteractWithObjects && mon.ActivationCounter < 255 ){
		MonstersInteractWithDoors( monsterIndex );
	}
	int rng_10000 = RNG(10000);
	if( range < 2 || mon.ActivationCounter != 255 || TransMap[ mon.Row ][ mon.Col ] != TransMap[ mon.TargetRow ][ mon.TargetCol ] ){
		mon.goal = MG_1_ATTACK;
	}else if( mon.goal == MG_4_WALK || (range >= 3 && RNG(4 << agility) == 0) ){
		if( mon.goal != MG_4_WALK ){
			mon.goalParam = 0;
			mon.curOrientation = RNG(2);
		}
		mon.goal = MG_4_WALK;
		if( mon.goalParam++ >= 2 * range && MayMoveMonsterToDirection( monsterIndex, dir ) ){
			mon.goal = MG_1_ATTACK;	
		}else if( rng_10000 < 200 * (mon.intel + 1) >> agility && CheckCastInSight( mon.Row, mon.Col, mon.TargetRow, mon.TargetCol ) ){
			StartMonsterSecondRangedAttack( monsterIndex, spellEffect, damage );
		}else{
			MonsterCircleTarget( monsterIndex, dir, mon.curOrientation );
		}
	}
	if( mon.goal == MG_1_ATTACK ){
		if( (range >= 3 && rng_10000 < 500 * (mon.intel + 2) >> agility || range >= 2 && rng_10000 < 500 * (mon.intel + 1) >> agility)
		 && CheckCastInSight( mon.Row, mon.Col, mon.TargetRow, mon.TargetCol ) ){
			StartMonsterSecondRangedAttack( monsterIndex, spellEffect, damage );
		}else if( range >= 2 ){
			int rng_100 = RNG(100);
			if( rng_100 < 10 * (mon.intel + 5)
			 || is(mon.PrevAction, A_1_WALK_UP, A_2_WALK_DOWN, A_3_WALK_HORIZ) && !mon.TimerCounterOrColDeltaOrSpellDamage && rng_100 < 10 * (mon.intel + 8) ){
				MonsterWalksTowardsTarget( monsterIndex, dir );
			}
		}else{ // melee
			if( rng_10000 < 250 * (mon.intel + by(Difficulty, 24, 30, 36)) ){
				mon.Orientation = dir;
				StartMonsterMeleeAttack( monsterIndex );
			}
		}
	}
	if( mon.CurAction == A_0_STAY ){
		int delay = RNG(10) + by(Difficulty, 20, 6, 0);
		MonsterStartDelay( monsterIndex, delay );
	}
}

//----- (th4) -------------------------------------------------------------
void __fastcall Ai_59_Tentaclist(uint monsterIndex)
{
	int interact = 0;
	int agility = 0;
	int damage = 0; // damage is set in CastTentacle
	Universal_OnlyRangeSecondAttack( monsterIndex, MI_145_TENTACLE, interact, damage, agility );
}

//----- (00441ECD) --------------------------------------------------------
void __fastcall Universal_ShortRangeSecondAttack(uint monsterIndex, int spellEffect, int damage)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	int intel = monster.intel;

	if( !Distance_To_Target_Is_Less_Than(monsterIndex, 5) ){// if distance to target >= 5 (3+ tiles away from melee range)
		Ai_2_Skeleton_Melee(monsterIndex); // use Skeleton AI
		return;
	}

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);

	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2)
	 || monster.ActivationCounter != 255
	 || TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
		monster.goal = MG_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.goal != MG_4_WALK && ( flag = !Distance_To_Target_Is_Less_Than(monsterIndex, 3) ) != 0 ){
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);
		}
		if( monster.goal == MG_4_WALK || flag ){
			monster.goal = MG_4_WALK;
			monster.mayRetreateTick = 4;

			int maxDelta = max(deltaRow, deltaCol);
			if( monster.goalParam++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
				monster.goal = MG_1_ATTACK;
			}else if( randFactor < 5 * (intel + 16) ){
				MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation);
			}
		}
	}

	if( monster.goal == MG_1_ATTACK ){
		if( (!Distance_To_Target_Is_Less_Than(monsterIndex, 3) && randFactor < 5*(intel + 2) || monster.mayRetreateTick == 4) 
		 && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
			// mor: below >> balrog AI missile replacements according to various factors
			if (MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
				if (monster.SpritePtr->baseMonsterIndex == BM_53_HELL_HOUND) {
					spellEffect = MI_129_SNOW_BOLT; // ice age hell hound spits snowbolts (looks better)
				}
				else {
					spellEffect = MI_127_HABIBI; // other firebreathers use snowblast
				}
			}		
			StartMonsterSecondRangedAttack(monsterIndex, spellEffect, damage);		
		}else if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
			randFactor = RNG(100);
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( randFactor < by( Difficulty, 5 * (intel + 11), 2 * (intel + 40), 2 * (intel + 45) )
			 || isAfterWalk && !monster.TimerCounter && randFactor < 10 * (intel + 7) ){
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
			}
		}else if( RNG(100) < by( Difficulty, 5 * intel + 30, 5 * intel + 55, 5 * intel + 75 ) ){
			monster.Orientation = orientationToTarget;
			RNG(2);// TODO: результат не используется
			StartMonsterMeleeAttack(monsterIndex);
		}
		monster.mayRetreateTick = 1;
	}
	if( !monster.CurAction ){
		if( Difficulty == 0 ) MonsterStartDelay(monsterIndex, RNG(30 - Dungeon->level) + 2);
		if( Difficulty == 1 ) MonsterStartDelay(monsterIndex, RNG(6) );
	}
}

//----- (004421D5) --------------------------------------------------------
void __fastcall Ai_26_Universal_ShortRangeSecondAttack_Incinerate(uint monsterIndex)
{
	Universal_ShortRangeSecondAttack(monsterIndex, MI_49_INCINERATE, 0);
}

//----- (th3) -------------------------------------------------------------
void __fastcall Ai_Summon( uint monsterIndex, AiFunc* original )
{
    if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
    Monster& monster = Monsters[monsterIndex];
	if( monster.Row == 1 && monster.Col == 0 ){
		return;
	}
    
    // Change target if current target deactivates
    if( ! (monster.flag & MF_11_NOENEMY) && (monster.flag & MF_5_TARGET_MONSTER) ){
        const Monster& targetMonster = Monsters[monster.TargetIndex];
        if( targetMonster.ActivationCounter == 0 ){
            MonsterLookAround(monsterIndex);
        }
    }
    
    if( !monster.ActivationCounter ){
        monster.ActivationCounter = 255;
        MonsterLookAround(monsterIndex);
    }
    
    if( monster.CurAction == A_0_STAY && !IsSomeMonstersActivated() ){
        int ownerIndex = monsterIndex / SummonMonstersPerPlayer_Count;
        Player& player = Players[ownerIndex];
    
        int deltaRow = abs(monster.Row - player.Row);
        int deltaCol = abs(monster.Col - player.Col);
        int distance = static_cast<int>( std::floor( sqrt( deltaRow * deltaRow + deltaCol * deltaCol ) ) );
        
        if( distance > 13 ){ // summon is too far to follow owner
            if( TeleportSummonToOwner( monsterIndex, ownerIndex ) ){
                return;
            }
        }
        
        int orientationToOwner = OrientationToTarget(monster.Row, monster.Col, player.Row, player.Col);
        
        // Move to owner if far from him or not in his LOS
        if( distance > 5 || !CheckCastInSight(monster.Row, monster.Col, player.Row, player.Col) ){
            int targetRow = monster.TargetRow;
            int targetCol = monster.TargetCol;
            monster.TargetRow = (char) player.Row;
            monster.TargetCol = (char) player.Col;
            bool pathMade = MakeMonsterPath( monsterIndex );
            monster.TargetRow = targetRow;
            monster.TargetCol = targetCol;
            if( pathMade ){
                return;
            }
        }
        
        // Move to owner if close to him
        if( distance < 4 ){
            if( MonsterWalksTowardsTarget( monsterIndex, OrientInverted[ orientationToOwner ] ) ){
                return;
            }
        }
    }
    
    // Use AI only if target selected
    if( !(monster.flag & MF_11_NOENEMY) ){
        (*original)( monsterIndex );
    }
}

//----- (004421E0) --------------------------------------------------------
void __fastcall Ai_21_Golem(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.Row == 1 && monster.Col == 0 ){
		return;
	}
	if( monster.CurAction == A_6_DEATH 
	 || monster.CurAction == A_11_SPELL_STAND 
	 || monster.CurAction >= A_1_WALK_UP && monster.CurAction <= A_3_WALK_HORIZ ){
		return;
	}
	if( !(monster.flag & MF_5_TARGET_MONSTER) ){
		MonsterLookAround(monsterIndex);
	}
	if( monster.CurAction == A_4_MELEE ){
		return;
	}
	Monster& target = Monsters[monster.TargetIndex];
	int deltaRow = abs(monster.Row - target.NextRow);
	int deltaCol = abs(monster.Col - target.NextCol);
	uint selTargetFlag = ! (monster.flag & MF_11_NOENEMY);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, target.Row, target.Col);
	monster.Orientation = orientationToTarget;
	if( deltaRow < 2 && deltaCol < 2 && selTargetFlag ){
		monster.TargetRow = target.Row;
		monster.TargetCol = target.Col;
		StartMonsterMeleeAttack(monsterIndex);
	}else{
		if( ! selTargetFlag || ! MonsterChangeTarget(monsterIndex) ){
			if( ++LOBYTE_IDA(monster.pathCount) > 8 ){
				LOBYTE_IDA(monster.pathCount) = 5;
			}

			//if( Monster_Walks_Towards_Target(monsterIndex, Players[monsterIndex / SummonMonstersPerPlayer_Count].dir) ){
			//	return;
			//}
			//orientationToTarget = OrientMinusOne[orientationToTarget];
			//for( int triesCount = 0; triesCount < 8; triesCount++ ){
			//	orientationToTarget = OrientPlusOne[orientationToTarget];
			//	if( MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
			//		StartMonsterWalk(monsterIndex, orientationToTarget);
			//		return;
			//	}
			//}

			char targetSteps[28];
			if (FindPath(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, target.Row, target.Col, targetSteps)) {
				MonsterWalksTowardsTarget(monsterIndex, StepOrientation[targetSteps[0]]);
				return;
			}

			// This code below is only reached if Golem failed to path to monster target (no target / too far / behind wall / unreachable).
			// monster.pathCount is either 5 or 7.
			int ownerIndex = monsterIndex / SummonMonstersPerPlayer_Count;
			int deltaRowMinion = abs(monster.Row - Players[ownerIndex].Row);
			int deltaColMinion = abs(monster.Col - Players[ownerIndex].Col);
			int distance = static_cast<int>(std::floor(sqrt(deltaRowMinion * deltaRowMinion + deltaColMinion * deltaColMinion)));
			int minDistance = 4; // Same value as in Ai_Summon()
			int maxDistance = 13;

			if (distance < minDistance) { // Avoid Golem getting on top of player if too close, walks in bigger circle around player.
				MonsterWalksTowardsTarget(monsterIndex, OrientPlusOne[OrientPlusOne[OrientationToTarget(monster.Row, monster.Col, Players[ownerIndex].Row, Players[ownerIndex].Col)]]);
			} else if (distance >= minDistance) { // Golem walks to player if it's not too far away, and teleports to player if it cannot path.
				char steps[28];
				if (FindPath(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, Players[ownerIndex].Row, Players[ownerIndex].Col, steps)) {
					MonsterWalksTowardsTarget(monsterIndex, StepOrientation[steps[0]]);
				} else if (distance > maxDistance) {
					TeleportSummonToOwner(monsterIndex, ownerIndex);
				}
			}
		}
	}
}

//----- (004423F9) --------------------------------------------------------
void __fastcall Ai_10_SkeletonKing(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);

	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}

	int intF = monster.intel;
	int randFactor = RNG(100);
	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) || monster.ActivationCounter != 255
		|| TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
			monster.goal = MG_1_ATTACK;
	}
	else{
		bool flag = false;
		if( monster.goal != MG_4_WALK && (flag = !Distance_To_Target_Is_Less_Than(monsterIndex, 3)) != 0 ){
			if (RNG(MaxCountOfPlayersInGame == 1 ? 5 : 95)) {
				goto ATTACK; // разобраться на досуге как встроить в условие
			}
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);
		}
		if( monster.goal == MG_4_WALK || flag ){
			monster.goal = MG_4_WALK;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.goalParam++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget)
				|| TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
					monster.goal = MG_1_ATTACK;
			}else if( !MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterStartDelay(monsterIndex, RNG(10) + 10);
			}
		}
	}
	ATTACK:
	if( monster.goal == MG_1_ATTACK ){
		// mor: this was my experiement, but it didn't work. i may return to it later.
		/*int life_factor = GameMode == GM_HARD ? 2 : 3; 
		if (MaxCountOfPlayersInGame == 1 && monster.CurrentLife < monster.BaseLife / life_factor && Distance_To_Target_Is_Less_Than(monsterIndex, 4) && randFactor < intF + 97) { 
			Monster_Walks_Towards_Target(monsterIndex, OrientInverted[orientationToTarget]); 
		}*/
		int summon_aggressiveness = 35;
		/*if (monster.CurrentLife < monster.BaseLife / 3) {
			summon_aggressiveness = 85;
		}*/
		if (MaxCountOfPlayersInGame == 1
			&& (!Distance_To_Target_Is_Less_Than(monsterIndex, 3) && randFactor < 4 * intF + summon_aggressiveness || randFactor < 6)
			&& CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)) {
			int minionRow = monster.Row + RowDelta[orientationToTarget];
			int minionCol = monster.Col + ColDelta[orientationToTarget];
			if (CheckCellForMeleeMonsterSafeMove(-(int)monsterIndex - 1, minionRow, minionCol)) {
				if (MonstersCount < Monsters_Max_Count) {
					SummonMinion(minionRow, minionCol, orientationToTarget);
					StartMonsterSecondAttack_4_orBlock(monsterIndex, orientationToTarget);
				}
			}
		}
		// not melee range
		else if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
			randFactor = RNG(100);
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( randFactor < by( Difficulty, intF * 5 + 40, intF * 5 + 60, intF * 5 + 80)
			 || isAfterWalk && !monster.TimerCounter && randFactor < intF + 95 ){
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
			}else{
				if( Difficulty < 2 ){
					MonsterStartDelay(monsterIndex, RNG(Difficulty == 1 ? 5 : 18) + 6);
				}else{
					MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
				}
			}
		}
		// in melee range
		else if( randFactor < by(Difficulty, intF * 5 + 25, intF * 5 + 40, intF * 9 + 55 ) ){
			monster.Orientation = orientationToTarget;
			StartMonsterMeleeAttack(monsterIndex);
		}
	}

	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
	return;
}

//----- (0044271A) --------------------------------------------------------
void __fastcall Ai_5_ChargeAttack(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){// TODO : всегда выполняется
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);
	int intel = monster.intel;
	bool runAttackAllow  = randFactor < by(Difficulty, 3 * intel + (GameMode == GM_HARD ? 70 : 23), 3 * intel + (GameMode == GM_HARD ? 75 : 50), 2 * intel + 80);
	if (GameMode == GM_CLASSIC) { 
		runAttackAllow = randFactor < 2 * intel + 43; 
	}
	bool meleeAttackAllow = randFactor < by(Difficulty, 2 * intel + (GameMode == GM_HARD ? 50 :  4), 4 * intel + (GameMode == GM_HARD ? 60 : 26), 2 * intel + 70);

	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
		monster.goal = MG_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.goal != MG_4_WALK && ( flag = !Distance_To_Target_Is_Less_Than(monsterIndex, 5) && RNG(4) ) != 0 ){
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);
		}
		if( monster.goal == MG_4_WALK || flag ){
			monster.goal = MG_4_WALK;
			int maxDelta = max(deltaCol, deltaRow);
			if( monster.goalParam++ >= 2 * maxDelta || TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.goal = MG_1_ATTACK;
			}else if( !MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterStartDelay(monsterIndex, RNG(10) + 10);
			}
		}
	}
	if( monster.goal == MG_1_ATTACK ){
		if( !Distance_To_Target_Is_Less_Than( monsterIndex, by(Difficulty,6,5,4) ) && runAttackAllow 
		 && CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){ // атака с разбегу
			if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1 ){
				if( monster.BasePtr->HasSpecialSounds ){
					PlayMonsterSound(monsterIndex, MST_3_SATTACK);
				}
				MonsterMap[ monster.Row ][ monster.Col ] = -1 - int(monsterIndex);
				monster.CurAction = A_14_CHARGE;
			}
			return;
		}
		if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){// движение
			randFactor = RNG(100);
			int walk_delay_chance = GameMode == GM_HARD ? 98 : by(Difficulty, 2 * intel + 32, 2 * intel + 56, 2 * intel + 80);
			bool walkWithDelayAllow = randFactor < walk_delay_chance;
			int continued_walking = 2 * monster.intel + 83;
			if (GameMode == GM_HARD) { 
				continued_walking = 98; 
			}
			bool walkWithoutDelayAllow = randFactor < continued_walking;
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if( walkWithDelayAllow || (isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow) ){
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
			}else{
				int another_attack_chance_modifier = by(Difficulty, 14, 11, 8, 6, 4, 3, 2, 1, 0);
				if (GameMode == GM_HARD) {
					another_attack_chance_modifier = 0;
				}
				MonsterStartDelay(monsterIndex, RNG(10) + another_attack_chance_modifier);
			}
			return;
		}
		if( meleeAttackAllow ){// Ближняя атака
			monster.Orientation = orientationToTarget;
			StartMonsterMeleeAttack(monsterIndex);
			return;
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00442A04) --------------------------------------------------------
void __fastcall Ai_34_HorkDemon(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);
	if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
		monster.goal = MG_1_ATTACK;
	}else{
		bool flag = false;
		if( monster.goal != MG_4_WALK  && ( flag = !Distance_To_Target_Is_Less_Than(monsterIndex, 5) ) != 0 ){
			monster.goalParam = 0;
			monster.curOrientation = RNG(2);
		}
		if( monster.goal == MG_4_WALK || flag ){
			monster.goal = MG_4_WALK;
			int maxDelta = max(deltaRow, deltaCol);
			if( monster.goalParam++ >= 2*maxDelta || TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.goal = MG_1_ATTACK;
			}else if( !MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation) ){
				MonsterStartDelay(monsterIndex, RNG(10) + 10);
			}
		}
	}

	if( monster.goal == MG_1_ATTACK ){
		if( Distance_To_Target_Is_Less_Than(monsterIndex, 3) || randFactor >= 2 * monster.intel + 43 ){
			if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){
				randFactor = RNG(100);
				bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
				if( randFactor < 2 * monster.intel + 33
					|| isAfterWalk && !monster.TimerCounter && randFactor < 2 * monster.intel + 83 ){
						MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
				}else{
					MonsterStartDelay(monsterIndex, RNG(10) + 10);
				}
			}else if( randFactor < 2*monster.intel + 28 ){
				monster.Orientation = orientationToTarget;
				StartMonsterMeleeAttack(monsterIndex);
			}
		}else if( CheckCellForMeleeMonsterSafeMove(monsterIndex, monster.Row + RowDelta[monster.Orientation], monster.Col + ColDelta[monster.Orientation]) && MonstersCount < Monsters_Max_Count ){
			StartMonsterSecondRangedAttack(monsterIndex, MI_95_SPAWN_MEAT_BALL, 0);
		}
	}
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00442CB5) --------------------------------------------------------
void __fastcall Ai_25_Mage(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ) return;
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	if( !monster.ActivationCounter ){
		return;
	}
	int deltaRow = abs(monster.Row - monster.TargetRow);
	int deltaCol = abs(monster.Col - monster.TargetCol);
	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col, monster.LastTargetPositionRow, monster.LastTargetPositionCol);
	if( monster.ActivationCounter < 255 ){
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);
	if( monster.goal == MG_2_RETREAT){
		if( monster.goalParam++ <= 3 ){
			if (monster.SpritePtr->baseMonsterIndex != BM_290_FUNGAL_ROOT_LITE) {
				MonsterWalksTowardsTarget(monsterIndex, OrientInverted[orientationToTarget]);
			}
		}
		else {
			monster.goal = MG_1_ATTACK;
			if( monster.SpritePtr->baseMonsterIndex != BM_290_FUNGAL_ROOT_LITE){
				Monster_Fade_In(monsterIndex, orientationToTarget, 1); 
			}
		}
	}else if( monster.goal == MG_4_WALK ){
		if (monster.SpritePtr->baseMonsterIndex == BM_290_FUNGAL_ROOT_LITE) {
			goto BOOP;
		}
		int maxDelta = max(deltaRow, deltaCol);
		if( Distance_To_Target_Is_Less_Than(monsterIndex, 2) 
			|| monster.ActivationCounter != 255 
			|| TransMap[ monster.Row ][ monster.Col ] != TransMap[ monster.TargetRow ][ monster.TargetCol ] ){
				monster.goal = MG_1_ATTACK;
				Monster_Fade_In(monsterIndex, orientationToTarget, 1);
		}else if( monster.goalParam++ >= 2 * maxDelta && MayMoveMonsterToDirection(monsterIndex, orientationToTarget) ){
			monster.goal = MG_1_ATTACK;
			Monster_Fade_In(monsterIndex, orientationToTarget, 1);
		}else{
			MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation);
		}
	}else if( monster.goal == MG_1_ATTACK ){
		BOOP:
		if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){// chances to attack
			if( randFactor < by(Difficulty, 9 * (monster.intel + (GameMode == GM_HARD ? 15 :  2)),
											5 * (monster.intel + (GameMode == GM_HARD ? 15 : 10)),
											5 * (monster.intel + 15) )
			 && CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){
				int damage = RngFromRange( monster.MinDamage, monster.MaxDamage);
				int missile;
				if (GameMode == GM_CLASSIC) {
					missile = by(monster.intel, MI_1_FIREBOLT, MI_52_CHARGED_BOLT, MI_7_LIGHTNING, MI_6_FIREBLAST);
				}
				else if (monster.SpritePtr->baseMonsterIndex == BM_290_FUNGAL_ROOT_LITE) {
					missile = RngFrom(MI_6_FIREBLAST, MI_57_ACID_MISSILE, MI_129_SNOW_BOLT);
				}
				else if( Dungeon->level != 13 && Dungeon->level != 14 && ! IsThisBoss(monsterIndex, UM_578_Uber_Xorene) ){// mor: TH1 legacy, i have to rewrite this properly!
					missile = MonsterMissiles[monster.intel];
				}else{
					if( RNG(2) ){
						missile = MonsterIntel_2[ monster.intel ];
					}else{
						missile = MonsterIntel_1[ monster.intel ];
					}
				}
				StartMonsterRangedAttack(monsterIndex, missile, damage);
			}else if( RNG(100) < 30 ){
				monster.goalParam = 0;
				monster.goal = MG_4_WALK;
				if (monster.SpritePtr->baseMonsterIndex != BM_290_FUNGAL_ROOT_LITE) {
					Monster_Fade_Out(monsterIndex, orientationToTarget, 0);
				}
			}else{
				MonsterStartDelay( monsterIndex, RNG(10) + 2 * (5 - monster.intel) );
			}
		}else{// behavior if distance to target is less than 2 tiles...(iow, retreat mechanics)
			monster.Orientation = orientationToTarget;			
			int retreat_chance = 30;
			switch (GameMode) {
				case GM_HARD:		retreat_chance = 91 + Difficulty;	break;
				case GM_NIGHTMARE:	retreat_chance = 40 + Difficulty;	break;
				case GM_IRONMAN:
				case GM_SPEEDRUN:	retreat_chance = 30 + Difficulty;	break;
				case GM_EASY:		retreat_chance = 20 + Difficulty;	break;
				default:			retreat_chance = 35 + Difficulty;	break;
			}

			if( ((monster.CurrentLife < monster.BaseLife * 3 / 4) && GameMode == GM_HARD) // on Hard mode retreat if life below 75%, always
				|| ((monster.CurrentLife < monster.BaseLife / 2) // other modes: retreat at 50% and below
				&& (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT))) // and if there is no 'full contact' GC
				&& (RNG(100) < retreat_chance) )){ // and with 12% chance
				monster.goalParam = 0;
				monster.goal = MG_2_RETREAT;
				if (monster.SpritePtr->baseMonsterIndex != BM_290_FUNGAL_ROOT_LITE) {
					Monster_Fade_Out(monsterIndex, orientationToTarget, 0);
				}
			}			
			else if( monster.PrevAction == A_13_DELAY
			 ||(RNG(100) < by(Difficulty, 4 * monster.intel + (GameMode == GM_HARD ? 50 :  8),
											   8 * monster.intel + (GameMode == GM_HARD ? 60 : 30),
											   8 * monster.intel + 70) ) ){
				StartMonsterRangedAttack(monsterIndex, -1, 0);
				CastMissile( monster.Row, monster.Col, 0, 0, monster.Orientation, MI_11_FLASH_FR, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0, 0 );
				CastMissile( monster.Row, monster.Col, 0, 0, monster.Orientation, MI_12_FLASH_BACK, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0, 0 );
			}else{
				MonsterStartDelay(monsterIndex, RNG(10) + 2 * (5 - monster.intel));
			}
		}
	}

	if( !monster.CurAction ){// do nothing and stay idle ?
		MonsterStartDelay(monsterIndex, RNG(10) + 5);
	}
}

//----- (00443020) --------------------------------------------------------
void __fastcall Ai_18_Garbud(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int row = monster.Row;
	int col = monster.Col;
	int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex >= SP_144 && monster.speechIndex <= SP_146 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.goal == MG_7_WAIT_TO_TALK ){
				monster.goal = MG_6_TALK;
				monster.speechIndex++;
			}
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_147 ){
			if( !SoundIsPlaying(S_955_M_GARBUD04) ){
				if( monster.goal == MG_7_WAIT_TO_TALK ){
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.goal = MG_1_ATTACK;
				}
			}
		}
	}
	if( monster.goal == MG_1_ATTACK || monster.goal == MG_4_WALK ){
		AttackTypeMelee(monsterIndex, 1);
	}
	monster.Orientation = orientationToTarget;
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443121) --------------------------------------------------------
void __fastcall Ai_22_Zhar(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_148 
		&& !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) 
		&& monster.goal == MG_7_WAIT_TO_TALK ){
			monster.speechIndex = SP_149;
			monster.goal = MG_6_TALK;
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_149 && !SoundIsPlaying(S_969_M_ZHAR02) && monster.goal == MG_7_WAIT_TO_TALK ){
			monster.ActivationCounter = 255;
			monster.speechIndex = 0;
			monster.goal = MG_1_ATTACK;
		}
	}
	if( monster.goal == MG_1_ATTACK || monster.goal == MG_2_RETREAT || monster.goal == MG_4_WALK ){
		Ai_25_Mage(monsterIndex);
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443260) --------------------------------------------------------
void __fastcall Ai_23_Snotspill(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_20 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.goal == MG_7_WAIT_TO_TALK ){
				monster.speechIndex = SP_21;
				monster.goal = MG_6_TALK;
			}
		}
	}
	if( monster.speechIndex == SP_21 ){
		if( Quests[Q_7_OGDENS_SIGN].status2 == 3 ){
			monster.speechIndex = 0;
			monster.goal = MG_1_ATTACK;
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_22 ){
			if( !SoundIsPlaying(S_965_M_SNOT03) ){
				if( monster.goal == MG_7_WAIT_TO_TALK ){
					ChangeMapRectAfterScriptEvent(CurPattern.row, CurPattern.col, CurPattern.sizeRow + CurPattern.row + 1, CurPattern.sizeCol + CurPattern.col + 1);
					Quests[Q_7_OGDENS_SIGN].status2 = 3;
					ChangeAllPlayersVisibility();
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.goal = MG_1_ATTACK;
				}
			}
		}
		if( Quests[Q_7_OGDENS_SIGN].status2 == 3 ){
			if( monster.goal == MG_1_ATTACK || monster.goal == MG_5_ATTACK2 ){
				Ai_8_Fallen(monsterIndex);
			}
		}
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004433AB) --------------------------------------------------------
void __fastcall Ai_28_Lazarus(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}

	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ monster.Row ][ monster.Col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( MaxCountOfPlayersInGame != 1 ){
			if( monster.speechIndex == SP_35 && monster.goal == MG_6_TALK ){
				if( Quests[Q_15_ARCHBISHOP_LAZARUS].status2 <= LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR ){
					monster.CurAction = A_17_TALK;
				}
			}
		}else{
			if( monster.speechIndex == SP_35 ){
				if( monster.goal == MG_6_TALK ){
					Player& player = Players[CurrentPlayerIndex];
					if( player.Row == 35 && player.Col == 46 ){
						PlayVideo2("gendata\\fprst3.smk");

						for (int i = 0; i < MonstersCount; i++)
						{
							if (is(Monsters[i].newBossId - 1, UM_4_ArchBishop_Lazarus, UM_5_Red_Vex, UM_6_Dark_Jade, UM_C_867_LAZARUS, UM_C_868_RED_VEX, UM_C_869_BLACKJADE)) {
								ChangeLightPos(Monsters[i].LightIndex, Monsters[i].Row, Monsters[i].Col);
							}
						}

						monster.CurAction = A_17_TALK;
						Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_5_TALK_WITH_LAZARUS;
					}
				}
				if( monster.speechIndex == SP_35 && !SoundIsPlaying(S_960_M_LAZ01) && monster.goal == MG_7_WAIT_TO_TALK ){
					ChangeMapRectAfterScriptEvent2(1, 18, 20, 24);
					ChangeAllPlayersVisibility();
					Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_6_FIGHT_WITH_LAZARUS;
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.goal = MG_1_ATTACK;
				}
			}
		}
	}

	int state = monster.goal;
	if( state == MG_1_ATTACK || state == MG_2_RETREAT || state == MG_4_WALK ){
		monster.speechIndex = 0; // патч на Лазаруса из D1 1.07-1.09
		Ai_25_Mage(monsterIndex); // ООП, Lazarus наследуется от Conselour :)
	}
	monster.Orientation = orientation;
	if( !monster.CurAction || monster.CurAction == A_17_TALK ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044351B) --------------------------------------------------------
void __fastcall Ai_29_Lazarus_s_Slut(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( MaxCountOfPlayersInGame == 1 ){
			if( Quests[Q_15_ARCHBISHOP_LAZARUS].status2 > LQS_5_TALK_WITH_LAZARUS ){
				monster.speechIndex = 0;
				monster.goal = MG_1_ATTACK;
			}else{
				monster.goal = MG_6_TALK;
			}
		}else{
			monster.goal = MG_1_ATTACK;
		}
	}

	if( monster.goal == MG_1_ATTACK ){
		Ai_14_RangerRetreatsOften_ArcaneStar(monsterIndex);
	}

	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004435D5) --------------------------------------------------------
void __fastcall Ai_30_Lachdanan(uint monsterIndex) // Lachdanan AI 
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if( monster.CurAction ){
		return;
	}
	int row = monster.Row;
	int col = monster.Col;
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( monster.speechIndex == SP_81 ){
		if( !(FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER) ){
			if( monster.goal == MG_7_WAIT_TO_TALK ){
				monster.speechIndex = SP_82;
				monster.goal = MG_6_TALK;
			}
		}
	}
	if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_83 ){
			if( !SoundIsPlaying(S_959_M_LACH03) ){
				if( monster.goal == MG_7_WAIT_TO_TALK ){
					monster.speechIndex = 0;
					Quests[Q_4_LACHDANAN].status = QS_3_COMPLETE;
					KillMonsterByPlayer(monsterIndex, -1);
				}
			}
		}
	}
	monster.Orientation = orientation;
	if( !monster.CurAction ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (004436B1) --------------------------------------------------------
void __fastcall Ai_31_Warlord(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}

	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];

	if( monster.CurAction ){
		return;
	}
	int orientation = MonsterOrientationToTarget(monsterIndex);
	if( FlagMap[ monster.Row ][ monster.Col ] & CF_2_VISIBLE_BY_PLAYER ){
		if( monster.speechIndex == SP_110_MONSTERS_WARLRD01 ){
			if( monster.goal == MG_6_TALK ){
				monster.CurAction = A_17_TALK;
			}
			if( !SoundIsPlaying(S_966_M_WARLRD01) ){
				if( monster.goal == MG_7_WAIT_TO_TALK ){
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.goal = MG_1_ATTACK;
				}
			}
		}
	}
	if( monster.goal == MG_1_ATTACK ){
		Ai_2_Skeleton_Melee(monsterIndex);
	}
	monster.Orientation = orientation;
	if( !monster.CurAction || monster.CurAction == A_17_TALK ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (00443789) --------------------------------------------------------
void RemoveKilledMonsters()
{
	for( int summonIndex = 0; summonIndex < SummonMonsters_Count; ++summonIndex ){
		Monster& summon = Monsters[summonIndex];
		if( summon.flag & MF_15_KILLED ){
			summon.Row = 1;
			summon.Col = 0;
			summon.NextRow = 0;
			summon.NextCol = 0;
			summon.PrevRow = 0;
			summon.PrevCol = 0;
		}
	}
	
	int monsterIndexIndex = SummonMonsters_Count;
	while( monsterIndexIndex < MonstersCount ){
		if( Monsters[MonsterIndexes[monsterIndexIndex]].flag & MF_15_KILLED ){
			DelMonster(monsterIndexIndex);
			monsterIndexIndex = SummonMonsters_Count;
		}else{
			++monsterIndexIndex;
		}
	}
}
/*
//bool DotAcidMechanic = true;
//----- (th2) -------------------------------------------------------------
void MonsterDot( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	int playerIndex = monster.dotLastAttacker;
	if( monster.acidDot > 0 && playerIndex == CurrentPlayerIndex ){
		Player& player = Players[playerIndex];
		int damage = 0;
		if( monster.overTime > 1 ){
			damage = (int) round((double) monster.acidDot / monster.overTime--);
			//LimitToMin( damage, 0 );
		}else{
			damage = monster.acidDot;
			monster.overTime = 0;
		}
		LimitToRange( damage, 1, (int) round( (double) MaxAcidDPS(monsterIndex, playerIndex) / 20 ) );
		monster.acidDot -= damage;
		SendCmdAcidDotMonster(0, monsterIndex, -damage, monster.dungeon, monster.acidDot); // Negative because damage is subtracted from monster.acidDot
		monster.CurrentLife -= damage;
		if( monster.acidDot > 0 && monster.overTime <= 0 ){
			monster.overTime = 1;
		}
		if( (monster.CurrentLife & ~63) > 0) {
			DamageMonsterByPlayer(monsterIndex, playerIndex, damage, 1); // 1 - no monster hit/stun on dot, 0 - monster has chance to hit/stun (classic behavior)
		}else{
			monster.acidDot = 0;
			DrawFloatingDamage( damage >> 6, monster.Row, monster.Col, monsterIndex );
			KillMonsterByPlayer( monsterIndex, playerIndex );
		}
	}

}
*/

// New MP acid sync: caster sees monster green bar and hp reduction per tick, while remote players see them changing once per second.
void MonsterDot(int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int playerIndex = monster.dotLastAttacker;
//	Player& player = Players[playerIndex];
//	if (monster.acidDot > 0 && playerIndex == CurrentPlayerIndex && (player.MaxCurLife & ~63) > 0 && player.CurLife > 0) { // Do not damage monster if acid caster is dead but stays on map.
	if (monster.acidDot > 0 && playerIndex == CurrentPlayerIndex) {
		int damage = 0;
		if (monster.overTime > 1) {
			damage = (int)round((double)monster.acidDot / monster.overTime--);
			//LimitToMin( damage, 0 );
		} else {
			damage = monster.acidDot;
			monster.overTime = 0;
		}
		LimitToRange(damage, 1, (int)round((double)MaxAcidDPS(monsterIndex, playerIndex) / 20));
		monster.acidDot -= damage;
		monster.acidDotDeltaSum += damage;
		monster.CurrentLife -= damage;
		if (monster.acidDot > 0 && monster.overTime <= 0) {
			monster.overTime = 1;
		}
		if (monster.acidDot <= 0) { // If green bar runs out, immediately sync the accumulated green bar, and reset last attacker for all players.
			monster.acidDot = 0;
			monster.dotLastAttacker = -1;
			SendCmdAcidDotMonster(0, monsterIndex, -monster.acidDotDeltaSum, monster.dungeon, monster.acidDot, monster.dotLastAttacker);
		} else if (UpdateAuraDamage % ENGINE_FPS == monsterIndex % ENGINE_FPS) { // Reduce packet spam down to 1 per second.  Can be a little inconvenient if acid attacker ID is changed before broadcasting.
			SendCmdAcidDotMonster(0, monsterIndex, -monster.acidDotDeltaSum, monster.dungeon, monster.acidDot, playerIndex); // Negative because damage is subtracted from monster.acidDot
		}
		if ((monster.CurrentLife & ~63) > 0) {
			if (monster.acidDot <= 0) { // If green bar runs out, immediately sync the accumulated damage.
				DamageMonsterByPlayer(monsterIndex, playerIndex, monster.acidDotDeltaSum, 1);
				monster.acidDotDeltaSum = 0;
			} else if (UpdateAuraDamage % ENGINE_FPS == monsterIndex % ENGINE_FPS) {
				DamageMonsterByPlayer(monsterIndex, playerIndex, monster.acidDotDeltaSum, 1); // 1 - no monster hit/stun on dot, 0 - monster has chance to hit/stun (classic behavior)
				monster.acidDotDeltaSum = 0;
			}
		} else {
			monster.acidDot = 0;
			DrawFloatingDamage(monster.acidDotDeltaSum >> 6, monster.Row, monster.Col, monsterIndex);
			monster.acidDotDeltaSum = 0;
			KillMonsterByPlayer(monsterIndex, playerIndex);
		}
	}
}

//----- (th2) -------------------------------------------------------------
ushort AuraMordorExtraChecks(ushort auraIndex, int baseMonsterIndex)
{
	ushort valueToAdd = 0;


	switch (auraIndex)
	{
	/* ------------------------
		STENCH OF DEATH
	-------------------------*/
	case AUI_1_:
		if (is(baseMonsterIndex, BM_0_BURNED_CORPSE, BM_2_ROTTING_CARCASS)) {
			valueToAdd += by(Difficulty, 0, 3, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_169_ZOMBIE)) {
			valueToAdd += by(Difficulty, 1, 4, 7, 9, 11, 13, 15, 17, 19);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_140_GHOUL)) {
			valueToAdd += by(Difficulty, 2, 5, 8, 10, 12, 14, 16, 18, 20);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_215_CADAVER)) {
			valueToAdd += by(Difficulty, 4, 8, 12, 15, 18, 21, 24, 27, 30);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_244_HORADRIM_ANCIENT, BM_246_GREATER_MUMMY)) {
			valueToAdd += by(Difficulty, 6, 10, 18, 24, 30, 36, 42, 48, 54);
			valueToAdd += Dungeon->level;
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_141_SHAMBLER, BM_286_BRINE_CORPSE)) {
			valueToAdd += by(Difficulty, 3, 6, 9, 11, 13, 15, 17, 19, 21);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_142_HELL_MEAT)) {
			valueToAdd += by(Difficulty, 4, 7, 10, 12, 14, 16, 18, 20, 22);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_135_GROTESQUE)) {
			valueToAdd += by(Difficulty, 5, 8, 11, 13, 15, 17, 19, 21, 23);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_120_NECROMORPH)) {
			valueToAdd += by(Difficulty, 6, 9, 12, 14, 16, 18, 20, 22, 24);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	//Perplexity (accuracy reduction)
	case AUI_2_:
		if (is(baseMonsterIndex, BM_248_SALAMANDER)) {
			valueToAdd += by(Difficulty, 3, 5, 7, 9, 11, 13, 15, 17, 20);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	//Mana Burn
	case AUI_3_:
		if (is(baseMonsterIndex, BM_1_OBLIVION_KNIGHT)) { // dlvl-4 bosses
			valueToAdd += by(Difficulty, 0, 3, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd *= 2;
			}
		}
		else if (is(baseMonsterIndex, BM_64_DEATH_GUARDIAN, BM_260_ARMAGEDDON_WALKER)) { // 21
			valueToAdd += by(Difficulty, 0, 2, 4, 5, 6, 7, 8, 9, 10);
			if (GameMode == GM_HARD) {
				valueToAdd += valueToAdd / 2;
			}
		}
		else if (is(baseMonsterIndex, BM_203_DEATH_KNIGHT)) { // 24
			valueToAdd += by(Difficulty, 1, 3, 5, 6, 7, 8, 9, 10, 11);
			if (GameMode == GM_HARD) {
				valueToAdd += valueToAdd / 2;
			}
		}
		else if (is(baseMonsterIndex, BM_50_SKELETON_LORD)) { // leo
			valueToAdd += by(Difficulty, 1, 7, 17, 25, 33, 41, 39, 47, 55);
			if (GameMode == GM_HARD) {
				valueToAdd *= 2;
			}
		}
		break;

	// -AC
	case AUI_4_:
		if (is(baseMonsterIndex, BM_221_RAT_MAN)) { // rat man
			valueToAdd += by(Difficulty, 0, 2, 4, 6, 8, 10, 12, 14, 16);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_258_RAT_HORDE)) { // rat horde
			valueToAdd += by(Difficulty, 1, 3, 5, 7, 9, 11, 13, 15, 17);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_126_PLAGUE_EATER)) { // plague eater
			valueToAdd += by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_274_FLESH_DEVOURER)) {
			valueToAdd += by(Difficulty, 4, 6, 8, 10, 12, 14, 16, 18, 20);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
			else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY, /*GM_CLASSIC,*/ GM_NIGHTMARE)) {
				valueToAdd /= 2;
			}
		}
		break;
	
	// - lite rez
	case AUI_5_: 
		if (is(baseMonsterIndex, BM_249_SCARAB)) {
			valueToAdd += by(Difficulty, 0, 2, 4, 6, 8, 10, 12, 14, 16);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_250_BONE_SCARAB)) {
			valueToAdd += by(Difficulty, 1, 3, 5, 7, 9, 11, 13, 15, 17);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	// - fire rez
	case AUI_6_:
		if (is(baseMonsterIndex, BM_61_BLOOD_STONE)) { // 8-10
			valueToAdd += by(Difficulty, 0, 2, 4, 6, 8, 10, 12, 14, 16);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_63_LAVA_LORD)) { // 10-12
			valueToAdd += by(Difficulty, 1, 3, 5, 7, 9, 11, 13, 15, 17);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_60_HELL_STONE)) { // 12-14
			valueToAdd += by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_180_CRYPT_STONE)) { // 21-23
			valueToAdd += by(Difficulty, 3, 5, 7, 9, 11, 13, 15, 17, 19);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_62_MAULER)) { // 24
			valueToAdd += by(Difficulty, 4, 6, 8, 10, 12, 14, 16, 18, 20);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_273_FLAME_FIEND)) { // passage
			valueToAdd += by(Difficulty, 5, 7, 9, 11, 13, 15, 17, 19, 21);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	// - cold rez
	case AUI_7_:
		if (is(baseMonsterIndex, BM_238_TAINTED)) {
			valueToAdd += by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		else if (is(baseMonsterIndex, BM_239_AFFLICTED)) {
			valueToAdd += by(Difficulty, 3, 5, 7, 9, 11, 13, 15, 17, 19);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	// - acid rez
	case AUI_8_:
		if (is(baseMonsterIndex, BM_45_VENOM_CLAN)) {
			valueToAdd += by(Difficulty, 2, 4, 6, 8, 10, 12, 14, 16, 18);
			if (GameMode == GM_HARD) {
				valueToAdd += 2;
			}
		}
		break;

	// - arcane rez
	case AUI_9_:
		valueToAdd += by(Difficulty, 1, 3, 5, 7, 9, 11, 13, 15, 17);
		if (GameMode == GM_HARD) { valueToAdd += 2; }
		break;

	// - melee DR rez
	case AUI_10_:
		if (is(baseMonsterIndex, BM_276_SWORD_SPIDER)) {
			valueToAdd += by(Difficulty, 2, 3, 4, 5, 6, 7, 8, 9, 10);
		}
		else {
			valueToAdd += by(Difficulty, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		}
		// modes
		if (GameMode == GM_HARD) { 
			valueToAdd += 2; 
		}
		else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY, /*GM_CLASSIC,*/ GM_NIGHTMARE)) {
			valueToAdd /= 2;
		}
		break;

	// - arrow DR rez
	case AUI_11_:
		valueToAdd += by(Difficulty, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		if (GameMode == GM_HARD) { valueToAdd += 2; }
		break;

	// - stun threshold
	case AUI_12_:
		if (is(baseMonsterIndex, BM_125_GOLD_GOLEM)) { // dlvl-24, gold golem
			valueToAdd += by(Difficulty, 1, 2, 3, 4, 5, 6, 7, 8, 9);
			if (GameMode == GM_HARD) {
				valueToAdd += 1;
			}
		}
		else { // caves-hell, other rock golems (not hidden)
			valueToAdd += by(Difficulty, 0, 1, 2, 3, 4, 5, 6, 7, 8);
			if (GameMode == GM_HARD) {
				valueToAdd += 1;
			}
		}
		break;

	// - damage
	case AUI_13_:
		if (is(baseMonsterIndex, BM_52_BILE_DEMON)) { // dlvls 5-9
			valueToAdd += by(Difficulty, 0, 4, 8, 10, 12, 14, 16, 18, 20); // 1-3 | 5-7 | 9-11 (by difficulty)			
		}
		else if (is(baseMonsterIndex, BM_56_PIT_DEMON, BM_93_FLAYED_ONE)) { // 8-12, 10-13
			valueToAdd += by(Difficulty, 1, 5, 9, 12, 15, 18, 21, 24, 27); // 2-4 | 6-8 | 10-12
		}
		else if (is(baseMonsterIndex, BM_178_OVERLORD)) { // 14-17
			valueToAdd += by(Difficulty, 2, 6, 10, 13, 16, 19, 22, 25, 28); // 3-5 | 7-9 | 11-13
		}
		else if (is(baseMonsterIndex, BM_179_FILTH_DEMON)) { // 18-21
			valueToAdd += by(Difficulty, 3, 7, 11, 14, 17, 20, 23, 26, 29); // 4-6 | 8-10 | 12-14
		}
		else if (is(baseMonsterIndex, BM_55_GRIM_DEMON)) { // 22-23
			valueToAdd += by(Difficulty, 4, 8, 12, 15, 18, 21, 24, 27, 30); // 5-7 | 9-11 | 13-15
		}
		// hard mode buff:
		if (GameMode == GM_HARD) {
			valueToAdd += valueToAdd / 2;
		}
		break;

	// + DFE
	case AUI_14_:
		if (is(baseMonsterIndex, BM_277_SPIDERLING)) {
			valueToAdd += by(Difficulty, 0, 3, 6, 9, 12, 15, 18, 21, 24);
		}
		if (GameMode == GM_HARD) {
			valueToAdd += 2;
		}
		else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY, /*GM_CLASSIC,*/ GM_NIGHTMARE)) {
			valueToAdd /= 2;
		}
		break;
	//case AUI_14_:
	//	break;
	//
	default:
		break;
	}

	return valueToAdd;
}

//----- (004437EE) --------------------------------------------------------
void ProcessMonsters() // ProcessMonsters
{
	if (BattleTrance) {
		BattleTranceSlowMonster = !BattleTranceSlowMonster;
		if (BattleTranceSlowMonster)
			return;
	}

	RemoveKilledMonsters();
	Player& auraTargetPlayer = Players[CurrentPlayerIndex];
	for (int j = 0; j < countof(Player::AuraIndex); j++)
	{
		auraTargetPlayer.AuraIndex[j] = AUI_0_NONE;
		auraTargetPlayer.AurasValues[j] = 0;
	}
	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[i];
		Monster& monster = Monsters[monsterIndex];

		if( MaxCountOfPlayersInGame > 1 ){
			SetRndSeed(monster.RandomSeedForAction);
			monster.RandomSeedForAction = GetRndSeed();
		}
		if( monster.CurAction != A_15_STONE && monster.CurrentLife < monster.BaseLife && (monster.CurrentLife & ~63) > 0 && monsterIndex >= SummonMonsters_Count ){
			monster.CurrentLife += 1 + monster.MonsterLevel / 4 + Difficulty * 12;
		}
		uchar flag = FlagMap[ monster.Row ][ monster.Col ];
		if( (flag & CF_2_VISIBLE_BY_PLAYER) && !monster.ActivationCounter ){
			MonsterLookAround(monsterIndex);
			monster.LastTargetPositionRow = monster.TargetRow;
			monster.LastTargetPositionCol = monster.TargetCol;
			if( ! ( monster.flag & MF_6_FRIENDLY ) ){ // target is monster
				MonsterChainActivation( monsterIndex );
			}
		}
		if( monster.flag & MF_5_TARGET_MONSTER ){ // target is monster
			if( monster.TargetIndex < Monsters_Max_Count ){
				Monster& targetMonster = Monsters[monster.TargetIndex];
                monster.TargetRow = monster.LastTargetPositionRow = targetMonster.NextRow;
                monster.TargetCol = monster.LastTargetPositionCol = targetMonster.NextCol;
				//monster.ActivationCounter = monster.newBossId ? 255 : 254; // fix monster don't attack summons
			}
		}else{ // target is player
			if( monster.TargetIndex < 4 ){
				Player& targetPlayer = Players[monster.TargetIndex];
				monster.TargetRow = (char) targetPlayer.NextRow;
				monster.TargetCol = (char) targetPlayer.NextCol;
				if( flag & CF_2_VISIBLE_BY_PLAYER || monster.newBossId && monster.ActivationCounter == 255
					/*|| monster.isBossPack && Monsters[monster.hisBossIndex].ActivationCounter == 255*/) {					
					monster.ActivationCounter = 255;
					monster.LastTargetPositionRow = targetPlayer.NextRow;
					monster.LastTargetPositionCol = targetPlayer.NextCol;
				}else if( monster.ActivationCounter && !monster.newBossId ){ // (10) расширение номера босса 004439CD
					monster.ActivationCounter--;
				}
			}
		}
		int moreAction = 1;
		while( moreAction ){
			if( !(monster.flag & MF_9_CHASE_TARGET) || !MonsterChangeTarget(monsterIndex) ){
			    auto iiFunc = AiFuncs[monster.ai].aiFunc;
			    if( monsterIndex < SummonMonsters_Count ){
			        Ai_Summon( monsterIndex, iiFunc );
                }else{
				    iiFunc(monsterIndex);
                }
			}
			//if( (uint) monster.Orientation > 7 ) __debugbreak();
			switch( monster.CurAction ){
			case A_0_STAY:			moreAction = MonsterStaying				( monsterIndex );	break;
			case A_1_WALK_UP:		moreAction = MonsterWalkUp				( monsterIndex );	break;
			case A_2_WALK_DOWN:		moreAction = MonsterWalkDown			( monsterIndex );	break;
			case A_3_WALK_HORIZ:	moreAction = MonsterWalkHorizontal		( monsterIndex );	break;
			case A_4_MELEE:			moreAction = MonsterMeleeAttack			( monsterIndex );	break;
			case A_5_GOT_HIT:		moreAction = MonsterHit					( monsterIndex );	break;
			case A_6_DEATH:			moreAction = MonsterHitToDeath			( monsterIndex );	break;
			case A_7_SECOND:		moreAction = MonsterSecondMeleeAttack	( monsterIndex );	break;
			case A_8_FADE_IN:		moreAction = MonsterEndMeleeAttack		( monsterIndex );	break;
			case A_9_FADE_OUT:		moreAction = SecondaryAnimation			( monsterIndex );	break;
			case A_10_RANGED:		moreAction = MonsterRangedAttack		( monsterIndex );	break;
			case A_11_SPELL_STAND:	moreAction = MonsterRangedHit			( monsterIndex );	break;
			case A_12_SPELL_ATTACK:	moreAction = MonsterSecondRangedAttack	( monsterIndex );	break;
			case A_13_DELAY:		moreAction = MonsterOrientTarget		( monsterIndex );	break;
			case A_14_CHARGE:		moreAction = 0;												break;
			case A_15_STONE:		moreAction = MonsterStoneCursed			( monsterIndex );	break;
			case A_16_HEAL:			moreAction = HealMode					( monsterIndex );	break;
			case A_17_TALK:			moreAction = MonsterTalk				( monsterIndex );	break;
			default: moreAction = 0; break;
			}
			if( moreAction ){
				BossPackActivation(monsterIndex);
			}
		}

		if( monster.CurAction != A_15_STONE ){
			monster.AnimationDelayIndex++;
			if( ! (monster.flag & (MF_3_STONE | MF_16_TRACK_ATTACK) ) ){
				if( monster.AnimationDelayIndex >= monster.AnimationDelayFrameCount ){  // TODO: change to > and fix the BaseMonsters animDelay data
					monster.AnimationDelayIndex = 0;
					if( monster.flag & MF_2_BACKWARDS ){
						monster.CurFrame--;
						if( monster.CurFrame == 0 ){
							monster.CurFrame = monster.animFrameCount;
						}
					}else{
						monster.CurFrame++;
						if( monster.CurFrame > monster.animFrameCount ){
							monster.CurFrame = 1;
						}
					}
				}
			}
		}
		/*if( DotAcidMechanic )*/ MonsterDot(monsterIndex);

		MonsterSprite* monsterSprite = monster.SpritePtr;
		int baseMonsterIndex = monsterSprite->baseMonsterIndex;

		for (ushort au = AUI_1_; au < AUI_COUNT; au++)
		{
			for (int auraBaseMonsterIndex : Auras[au].baseMonsterIndex)
			{
				if (auraBaseMonsterIndex == -1)
					break;

				if (auraBaseMonsterIndex == baseMonsterIndex
					&& (abs(monster.Row - auraTargetPlayer.Row) <= Auras[au].radius
					&& abs(monster.Col - auraTargetPlayer.Col) <= Auras[au].radius)) {
					int auraPlayerIndex = 0;
					bool isValueAddedToExisting = false;
					for (short auraIndex : auraTargetPlayer.AuraIndex)
					{
						if (auraIndex == Auras[au].auraIndex) {
							auraTargetPlayer.AuraIndex[auraPlayerIndex] = Auras[au].auraIndex;
							int distance = (abs(monster.Row - auraTargetPlayer.Row) > abs(monster.Col - auraTargetPlayer.Col)) ? abs(monster.Row - auraTargetPlayer.Row) : abs(monster.Col - auraTargetPlayer.Col);
							LimitToMin(distance, 1);
							ushort valueToAdd = ((100 / distance) * (Auras[au].maxval + AuraMordorExtraChecks(au, baseMonsterIndex))) / 100;
							LimitToMin(valueToAdd, Auras[au].minval);
							auraTargetPlayer.AurasValues[auraPlayerIndex] += valueToAdd;
							isValueAddedToExisting = true;
							break;
						}
						auraPlayerIndex++;
					}
					if (!isValueAddedToExisting) {
						auraPlayerIndex = 0;
						for (short auraIndex : auraTargetPlayer.AuraIndex)
						{
							if (auraIndex == AUI_0_NONE) {
								auraTargetPlayer.AuraIndex[auraPlayerIndex] = Auras[au].auraIndex;
								int distance = (abs(monster.Row - auraTargetPlayer.Row) > abs(monster.Col - auraTargetPlayer.Col)) ? abs(monster.Row - auraTargetPlayer.Row) : abs(monster.Col - auraTargetPlayer.Col);
								LimitToMin(distance, 1);
								ushort valueToAdd = ((100 / distance) * (Auras[au].maxval + AuraMordorExtraChecks(au, baseMonsterIndex))) / 100;
								LimitToMin(valueToAdd, Auras[au].minval);
								auraTargetPlayer.AurasValues[auraPlayerIndex] = valueToAdd;
								if (AuraSoundStates[auraPlayerIndex] == false && AuraCooldown[auraPlayerIndex] == 0 ) {
									if (GameMode != GM_CLASSIC) {
										PlayGlobalSound(Auras[au].sound);
									}
									AuraSoundStates[auraPlayerIndex] = true;
									AuraCooldown[auraPlayerIndex] = 29 * ENGINE_FPS;
								}
								break;
							}
							auraPlayerIndex++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < countof(Player::AuraIndex); i++)
	{
		if (auraTargetPlayer.AuraIndex[i] == AUI_0_NONE) {
			AuraSoundStates[i] = false;
		}

		AuraCooldown[i]--;
		LimitToMin(AuraCooldown[i], 0);

	}
		
	RemoveKilledMonsters();
	ClearMapFromInvalidMonsters();
}

//----- (00443B98) --------------------------------------------------------
void FreeMonsterResources()
{
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		MonsterSprite& monsterSprite = MonsterSprites[monsterSpriteIndex];
		int baseMonsterIndex = monsterSprite.baseMonsterIndex;
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		for( int animationIndex = 0; animationIndex < 6; animationIndex++ ){
			MonsterAnim& monsterAnim = monsterSprite.animation[animationIndex];
			if( TypeOfAnimation[animationIndex] != 's' || baseMonster.HasSecondAttack ){
				FreeMemZero(monsterAnim.animPtr);
			}
		}
	}
	FreeNotPlayerMissileSprites();
}

//----- (00443C02) --------------------------------------------------------
int __fastcall MayMoveMonsterToDirection(int monsterIndex, int direction, int range /*= 1*/ )
{
	Monster& monster = Monsters[monsterIndex];
	int nextRow = monster.Row + RowDelta[ direction ] * range;
	int nextCol = monster.Col + ColDelta[ direction ] * range;
	if( (uint)monsterIndex >= Monsters_Max_Count || ! In112(nextCol, nextRow)
	 || ! CheckCellForMeleeMonsterSafeMove(monsterIndex, nextRow, nextCol) ){
		return 0;
	}
	if( direction == 6 ){
		if( IsCellBlocking(nextRow, nextCol + 1, monster.canFly) || FlagMap[ nextRow ][ nextCol + 1 ] & CF_16_MONSTER_HORIZONTAL ){
			return 0;
		}
	}else if( direction == 2 ){
		if( IsCellBlocking(nextRow + 1, nextCol, monster.canFly) || FlagMap[ nextRow + 1 ][ nextCol ] & CF_16_MONSTER_HORIZONTAL ){
			return 0;
		}
	}else if( direction == 4 ){
		if( IsCellBlocking(nextRow + 1, nextCol, monster.canFly) || IsCellBlocking(nextRow, nextCol + 1, monster.canFly)){
			return 0;
		}
	}else if( direction == 0 ){
		if( IsCellBlocking(nextRow - 1, nextCol, monster.canFly) || IsCellBlocking(nextRow, nextCol + 1, monster.canFly)){
			return 0;
		}
	}
	return 1;
}

//----- (00443DF8) --------------------------------------------------------
int __fastcall IsCellNotBlockSight( int row, int col )
{
	int cell = 112 * row + col;
	return !TileBlockMissle[FineMap[ 0 ][ cell ]] && !(FlagMap[ 0 ][ cell ] & CF_16_MONSTER_HORIZONTAL);
}

//----- (00443E1E) --------------------------------------------------------
int __fastcall IsCellNotBlockWalking( int row, int col )
{
	return TileBlockWalking[FineMap[ row ][ col ]] == 0;
}

//----- (00443E38) --------------------------------------------------------
int __fastcall CheckLineWithTwoArgumentsCheckFunction( int (__fastcall *checkFunct)(int, int), int startRow, int startCol, int endRow, int endCol )
{
	#ifndef TH2_CODE
	int v5; // esi@1
	int v6; // edi@1
	int v7; // ebx@1
	int v8; // eax@3
	int v9; // eax@3
	int v10; // eax@4
	int v11; // esi@5
	int v12; // ebx@5
	int v13; // edi@5
	int v14; // esi@5
	int startCol_1; // edx@7
	int startRow_1; // ecx@12
	int v18; // eax@20
	int v19; // eax@20
	int v20; // eax@21
	int v21; // edi@22
	int v22; // ebx@22
	int v23; // esi@22
	int v24; // edi@22
	int( __fastcall *checkFunct_1 )(int, int); // [sp+Ch] [bp-14h]@1
	int endRow_1; // [sp+10h] [bp-10h]@1
	int endCol_1; // [sp+14h] [bp-Ch]@1
	int v30; // [sp+18h] [bp-8h]@5
	int v31; // [sp+18h] [bp-8h]@22
	int startRow_2; // [sp+1Ch] [bp-4h]@1

	v5 = endCol - startCol;
	checkFunct_1 = checkFunct;
	startRow_2 = startRow;
	endRow_1 = startRow;
	endCol_1 = startCol;
	v6 = endRow - startRow;
	v7 = abs( endCol - startCol );
	if( abs( v6 ) <= v7 ){
		if( v5 < 0 ){
			v18 = startCol;
			startCol = endCol;
			endCol = v18;
			v19 = startRow_2;
			v5 = -v5;
			startRow_2 = endRow;
			endRow = v19;
			v6 = -v6;
		}
		v20 = 2 * v6;
		if( v6 <= 0 ){
			v22 = v5 + v20;
			v24 = 2 * (v5 + v6);
			v31 = v20;
			v23 = -1;
		}else{
			v21 = v6 - v5;
			v22 = v20 - v5;
			v31 = v20;
			v23 = 1;
			v24 = 2 * v21;
		}
		while( startCol != endCol || startRow_2 != endRow ){
			if( (v23 < 0) ^ (v22 <= 0) ){
				v22 += v31;
			}else{
				v22 += v24;
				startRow_2 += v23;
			}
			if( (++startCol != endCol_1 || startRow_2 != endRow_1) && !checkFunct_1(startRow_2,startCol) ){
				break;
			}
		}
	}else{
		if( v6 < 0 ){
			v8 = startRow_2;
			startRow_2 = endRow;
			endRow = v8;
			v9 = startCol;
			v6 = -v6;
			startCol = endCol;
			endCol = v9;
			v5 = -v5;
		}
		v10 = 2 * v5;
		if( v5 <= 0 ){
			v12 = v6 + v10;
			v14 = 2 * (v6 + v5);
			v30 = v10;
			v13 = -1;
		}else{
			v11 = v5 - v6;
			v12 = v10 - v6;
			v30 = v10;
			v13 = 1;
			v14 = 2 * v11;
		}
		do{
			startCol_1 = startCol;
			if( startRow_2 == endRow && startCol == endCol ){
				break;
			}
			if( (v13 < 0) ^ (v12 <= 0) ){
				v12 += v30;
			}else{
				v12 += v14;
				startCol_1 = v13 + startCol;
				startCol += v13;
			}
			startRow_1 = startRow_2 + 1;
		}while( ++startRow_2 == endRow_1 && startCol_1 == endCol_1 || checkFunct_1( startRow_1, startCol_1 ) );
	}
	return startRow_2 == endRow && startCol == endCol;
	
	#else
	int deltaCol = endCol - startCol;
	int deltaRow = endRow - startRow;

	/*
		Я не понимаю как это работает но суть в следующем -
		проходим по всем клеткам ближайшим к диагонали между монстром и целью
		и возвращаем 0 если хоть одна из них недоступна
		и 1 если все доступны
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // Расстояние до цели по col больше или равно расстоянию до цели по row
		if( deltaCol < 0 ){ // если col цели меньше чем col монстра, меняем значения координат местами
			int temp;
			temp = endCol;
			endCol = startCol;
			startCol = temp;

			temp = endRow;
			endRow = startRow;
			startRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}
		int rowTab, i, j;

		if( deltaRow <= 0 ){
			i = deltaCol + 2 * deltaRow;
			j = 2 * (deltaCol + deltaRow);
			rowTab = -1;
		}else{
			i = 2 * deltaRow - deltaCol;
			j = 2 * (deltaRow - deltaCol);
			rowTab = 1;
		}
		while( endCol != startCol || endRow != startRow ){
			if( (i > 0) == (rowTab >= 0) ){
				i += j;
				startRow += rowTab;
			}else{
				i += 2 * deltaRow;
			}
			startCol++;
			if( endCol != startCol || endRow != startRow ){
				if( !checkFunct(startRow, startCol) ){
					return false;
				}
			}
		}
	}else{
		if( deltaRow < 0 ){
			int temp;
			temp = endCol;
			endCol = startCol;
			startCol = temp;

			temp = endRow;
			endRow = startRow;
			startRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}

		int colTab, i, j;

		if( deltaCol <= 0 ){
			i = deltaRow + 2 * deltaCol;
			j = 2 * (deltaRow + deltaCol);
			colTab = -1;
		}else{
			i = 2 * deltaCol - deltaRow;
			j = 2 * (deltaCol - deltaRow);
			colTab = 1;
		}
		while( endCol != startCol || endRow != startRow ){
			if( (i > 0) == (colTab >= 0) ){
				i += j;
				startCol += colTab;
			}else{
				i += 2 * deltaCol;
			}
			startRow++;
			if( endCol != startCol || endRow != startRow ){
				if( !checkFunct(startRow, startCol) ){
					return false;
				}
			}
		}
	}
	return endCol == startCol && endRow == startRow;
	#endif
}

//----- (00443FC7) --------------------------------------------------------
int __fastcall CheckCastInSight(int startRow, int startCol, int endRow, int endCol)
{
	return CheckLineWithTwoArgumentsCheckFunction(IsCellNotBlockSight, startRow, startCol, endRow, endCol);
}

// TODO: complete decompilation
//----- (00443FE1) --------------------------------------------------------
int __fastcall CheckLineWithThreeArgumentsCheckFunction( int (__fastcall *callingFunction) (int, int, int), int monsterIndex, int monsterRow, int monsterCol, int targetRow, int targetCol )
{
	#ifndef TH2_CODE
	int v6; // esi@1
	int v7; // edi@1
	int v8; // ebx@1
	int v9; // eax@3
	int v10; // eax@3
	int v11; // eax@4
	int v12; // esi@5
	int v13; // ebx@5
	int colTab; // edi@5
	int v15; // esi@5
	int monsterRow_1; // eax@7
	int v18; // eax@20
	int v19; // eax@20
	int v20; // eax@21
	int v21; // edi@22
	int v22; // ebx@22
	int v23; // esi@22
	int v24; // edi@22
	int( __fastcall *v27 )(int, int, int); // [sp+Ch] [bp-14h]@1
	int monsterIndex_1; // [sp+10h] [bp-10h]@1
	int v29; // [sp+14h] [bp-Ch]@1
	int v30; // [sp+18h] [bp-8h]@1
	int v31; // [sp+1Ch] [bp-4h]@5
	int v32; // [sp+1Ch] [bp-4h]@22

	v27 = callingFunction;
	v6 = targetCol - monsterCol;
	monsterIndex_1 = monsterIndex;
	v29 = monsterRow;
	v30 = monsterCol;
	v7 = targetRow - monsterRow;
	v8 = abs( targetCol - monsterCol );
	if( abs( targetRow - monsterRow ) <= v8 ){
		if( v6 < 0 ){
			v18 = monsterCol;
			monsterCol = targetCol;
			targetCol = v18;
			v19 = monsterRow;
			v6 = -v6;
			monsterRow = targetRow;
			targetRow = v19;
			v7 = -v7;
		}
		v20 = 2 * v7;
		if( v7 <= 0 ){
			v22 = v6 + v20;
			v24 = 2 * (v6 + v7);
			v32 = v20;
			v23 = -1;
		}else{
			v21 = v7 - v6;
			v22 = v20 - v6;
			v32 = v20;
			v23 = 1;
			v24 = 2 * v21;
		}
		while( monsterCol != targetCol || monsterRow != targetRow ){
			if( v22 <= 0 == v23 < 0 ){
				v22 += v24;
				monsterRow += v23;
			}else{
				v22 += v32;
			}
			if( (++monsterCol != v30 || monsterRow != v29) && !v27( monsterIndex_1, monsterRow, monsterCol ) ){
				break;
			}
		}
	}else{
		if( v7 < 0 ){
			v9 = monsterRow;
			monsterRow = targetRow;
			targetRow = v9;
			v10 = monsterCol;
			v7 = -v7;
			monsterCol = targetCol;
			targetCol = v10;
			v6 = -v6;
		}
		v11 = 2 * v6;
		if( v6 <= 0 ){
			v13 = v7 + v11;
			v15 = 2 * (v7 + v6);
			v31 = v11;
			colTab = -1;
		}else{
			v12 = v6 - v7;
			v13 = v11 - v7;
			v31 = v11;
			colTab = 1;
			v15 = 2 * v12;
		}
		do{
			monsterRow_1 = monsterCol;
			if( monsterRow == targetRow && monsterCol == targetCol ){
				break;
			}
			if( v13 <= 0 == colTab < 0 ){
				v13 += v15;
				monsterRow_1 = colTab + monsterCol;
				monsterCol += colTab;
			}else{
				v13 += v31;
			}
		}while( ++monsterRow == v29 && monsterRow_1 == v30
				  || v27(monsterIndex_1, monsterRow, monsterRow_1) );
	}
	return monsterRow == targetRow && monsterCol == targetCol;
	
	#else
	int deltaCol = targetCol - monsterCol;
	int deltaRow = targetRow - monsterRow;

	/*
		Я не понимаю как это работает но суть в следующем -
		проходим по всем клеткам ближайшим к диагонали между монстром и целью
		и возвращаем 0 если хоть одна из них недоступна
		и 1 если все доступны
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // Расстояние до цели по col больше или равно расстоянию до цели по row
		if( deltaCol < 0 ){ // если col цели меньше чем col монстра, меняем значения координат местами
			int temp;
			temp = targetCol;
			targetCol = monsterCol;
			monsterCol = temp;

			temp = targetRow;
			targetRow = monsterRow;
			monsterRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}
		int rowTab, i, j;

		if( deltaRow <= 0 ){
			i = deltaCol + 2 * deltaRow;
			j = 2 * (deltaCol + deltaRow);
			rowTab = -1;
		}else{
			i = 2 * deltaRow - deltaCol;
			j = 2 * (deltaRow - deltaCol);
			rowTab = 1;
		}
		while( targetCol != monsterCol || targetRow != monsterRow ){
			if( (i > 0) == (rowTab >= 0) ){
				i += j;
				monsterRow += rowTab;
			}else{
				i += 2 * deltaRow;
			}
			monsterCol++;
			if( targetCol != monsterCol || targetRow != monsterRow ){
				if( !callingFunction(monsterIndex, monsterRow, monsterCol) ){
					return false;
				}
			}
		}
	}else{
		if( deltaRow < 0 ){
			int temp;
			temp = targetCol;
			targetCol = monsterCol;
			monsterCol = temp;

			temp = targetRow;
			targetRow = monsterRow;
			monsterRow = temp;

			deltaCol = -deltaCol;
			deltaRow = -deltaRow;
		}

		int colTab, i, j;

		if( deltaCol <= 0 ){
			i = deltaRow + 2 * deltaCol;
			j = 2 * (deltaRow + deltaCol);
			colTab = -1;
		}else{
			i = 2 * deltaCol - deltaRow;
			j = 2 * (deltaCol - deltaRow);
			colTab = 1;
		}
		while( targetCol != monsterCol || targetRow != monsterRow ){
			if( (i > 0) == (colTab >= 0) ){
				i += j;
				monsterCol += colTab;
			}else{
				i += 2 * deltaCol;
			}
			monsterRow++;
			if( targetCol != monsterCol || targetRow != monsterRow ){
				if( !callingFunction(monsterIndex, monsterRow, monsterCol) ){
					return false;
				}
			}
		}
	}
	return targetCol == monsterCol && targetRow == monsterRow;
	#endif
}

//----- (th2) -------------------------------------------------------------
int __fastcall MonsterAnimIndex(int curAction)
{
	int animationIndex = A_0_STAY;
	switch( curAction ){
	case A_1_WALK_UP or A_2_WALK_DOWN or A_3_WALK_HORIZ:	animationIndex = ANIM_1_WALK;	break;
	case A_4_MELEE or A_10_RANGED or A_14_CHARGE:			animationIndex = ANIM_2_ATTACK;	break;
	case A_5_GOT_HIT:										animationIndex = ANIM_3_HIT;	break;
	case A_6_DEATH:											animationIndex = ANIM_4_DIE;	break;
	case A_7_SECOND or A_8_FADE_IN or A_9_FADE_OUT
	  or A_11_SPELL_STAND or A_12_SPELL_ATTACK or A_16_HEAL:animationIndex = ANIM_5_SECOND;	break;
	case A_0_STAY or A_13_DELAY or A_17_TALK or A_15_STONE: animationIndex = ANIM_0_STAY;	break;
	}
	return animationIndex;
}

//----- (0044417C) --------------------------------------------------------
void __fastcall PrepareLoadedMonster(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ) return;
	Monster& monster = Monsters[monsterIndex];
	
	// Try to fix invalid monster sprites indexes on load level
	if( monster.SpriteIndex >= MonsterSpritesCount ){
		int baseMonsterIndex = -1;
		for( int i = 0; i < BM_COUNT; i++ ){
			if( ! strcmp(monster.Name, BaseMonsters[i].name) ){
				baseMonsterIndex = i;
				break;
			}
		}
		if( baseMonsterIndex >= 0 ){
			for( int i = 0; i < MonsterSpritesCount; ++i ){
				if( MonsterSprites[i].baseMonsterIndex == baseMonsterIndex ){
					monster.SpriteIndex = i;
					break;
				}
			}
		}
		if( monster.SpriteIndex >= MonsterSpritesCount ){
			monster.SpriteIndex = MonsterSpritesCount - 1;
		}
	}

	monster.SpritePtr = &MonsterSprites[monster.SpriteIndex];
	monster.BasePtr = MonsterSprites[monster.SpriteIndex].baseMonsterPtr;
	int bossNumber = monster.newBossId;
	if( bossNumber ){ // (11) расширение номера босса 004441C9
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && UniqueMonsters[bossNumber - 1].IceAgeNamePtr) {
			monster.Name = UniqueMonsters[bossNumber - 1].IceAgeNamePtr;
		}
		else {
			monster.Name = UniqueMonsters[bossNumber - 1].NamePtr;
		}
	}else{
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && monster.BasePtr->IceAgeNamePtr) {
			monster.Name = monster.BasePtr->IceAgeNamePtr;
		}
		else {
			monster.Name = monster.BasePtr->name;
		}
	}
	uchar orientation = monster.Orientation;
	uchar animationIndex = MonsterAnimIndex(monster.CurAction);
	if( is(monster.CurAction, A_14_CHARGE, A_15_STONE) ){
		monster.CurFrame = 1;
		monster.animFrameCount = monster.SpritePtr->animation[animationIndex].frameCount;
	}
	monster.curAnimation = &monster.SpritePtr->animation[animationIndex].oriented[orientation];
}

// Fallen Ones run away after a nearby monster kill, #fallen run away, #fallen retreat
//----- (004442F6) --------------------------------------------------------
void __fastcall BewareNearMonsters( const int row, const int col )
{
    for( int i = 0; i < MonstersCount; i++ ){
	    Monster& monster = Monsters[MonsterIndexes[i]];
	    
	    int rundist = 0;
        switch( monster.SpritePtr->baseMonsterIndex ){
            case BM_213_POISONED_ONE or BM_5_FALLEN_ONE:												rundist = 6;	break;
            case BM_155_FALLEN_ONE or BM_157_YELLOW_ONE or BM_C_305_FALLEN_ONE or BM_C_309_FALLEN_ONE:  rundist = 5;	break;
            case BM_4_GREMLIN or BM_189_CARVER or BM_C_306_CARVER or BM_C_310_CARVER:					rundist = 4;    break;
            case BM_12_IMP or BM_C_307_DEVIL_KIN or BM_C_311_DEVIL_KIN:									rundist = 3;    break;
            case BM_13_DARK_ONE or BM_C_308_DARK_ONE or BM_C_312_DARK_ONE:								rundist = 2;    break;
        }
        
        if( monster.ai == AI_8_Fallen_One
            && ( Difficulty == DL_0_HORROR || GameMode == GM_CLASSIC)
			&& GameMode != GM_HARD // hard mode fallen are fearless ^_^
            && rundist
            && abs( row - monster.Row ) < 5
            && abs( col - monster.Col ) < 5
            && ( monster.CurrentLife & ~63) > 0 ){
                monster.goal = MG_2_RETREAT;
                monster.goalParam = rundist;
                monster.Orientation = OrientationToTarget( row, col,
                    monster.Row, monster.Col
                    // broken original code
                    // Monsters[i].Row, Monsters[i].Col
                    );
        }
    }
}

//----- (0044440C) --------------------------------------------------------
void __fastcall ShowMonsterInfo(int monsterIndex)
{
	int monsterResistMask = 0;
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	int totalSlain = TotalSlain[baseMonsterIndex];
	#if CHEATS	
	sprintf(InfoPanelBuffer, "Total slain: %i, i %i rc %i %i", totalSlain, monsterIndex, monster.Row, monster.Col);
	#else
	sprintf(InfoPanelBuffer, "Total slain : %i", totalSlain);
	#endif
	InfoPanel_AddLine(InfoPanelBuffer, true);
	int hpShow = GameMode == GM_CLASSIC ? 0 : 5;
	if( totalSlain >= hpShow ){
		sprintf(InfoPanelBuffer, "Hit Points : %i of %i", Monsters[CurMon].CurrentLife >> 6, Monsters[CurMon].BaseLife >> 6); // 1 показываемый хит поинт равен 64 реальных
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
	// Желательно использовать не маску, а точные значения для всех 3х сопротивлений...
	int rezShow = GameMode == GM_CLASSIC ? 0 : 3;
	if( totalSlain >= rezShow ){
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		if( Difficulty >= DL_2_DOOM ){
			monsterResistMask = baseMonster.ResistOn3Difficulty;
		}else{
			monsterResistMask = baseMonster.ResistOn1And2Difficulty;
		}
		InfoPanel_AddLine("Monster type : ", true, Spicies[ min( baseMonster.MonsterClass, 3ui16) ] );
		#ifdef comment // 1.150
		if( !(monsterResistMask & 0x3F) ){  // 0011 1111 в маске учитываются только первые 6 битов
			InfoPanel_AddLine("No magic resistance", true);
			return;
		}
		if( monsterResistMask & (MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT) ){ // 0111 За сопротивления отвечают первые 3 бита. За магию, огонь и молнию соответственно
			strcpy(InfoPanelBuffer, "Resists : ");
			if( monsterResistMask & MR_1_ARCAN ){
				strcat(InfoPanelBuffer, "Magic ");
			}
			if( monsterResistMask & MR_2_FIRE ){
				strcat(InfoPanelBuffer, "Fire ");
			}
			if( monsterResistMask & MR_3_LIGHT ){
				strcat(InfoPanelBuffer, "Lightning ");
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = 0;
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		#endif
		if( monsterResistMask & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID|MR_11_IM_COLD) ){ // 0011 1000 За имунитеты отвечают следующие 3 бита. За магию, огонь и молнию соответственно
			strcpy(InfoPanelBuffer, "Immune : ");
			if( monsterResistMask & MR_4_IM_ARCAN ){
				strcat(InfoPanelBuffer, "Magic ");
			}
			if( monsterResistMask & MR_5_IM_FIRE ){
				strcat(InfoPanelBuffer, "Fire ");
			}
			if( monsterResistMask & MR_6_IM_LIGHT ){
				strcat(InfoPanelBuffer, "Lightning ");
			}
			if( monsterResistMask & MR_8_IM_ACID ){
				strcat(InfoPanelBuffer, "Acid ");
			}
			if( monsterResistMask & MR_11_IM_COLD ){
				strcat(InfoPanelBuffer, "Cold ");
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = '\0';
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}else{
			InfoPanel_AddLine("No immunity", true);
		}
	}
}

//----- (00444497) --------------------------------------------------------
void ShowBossHP()
{
	sprintf( InfoPanelBuffer, "Hit Points : %i of %i", Monsters[ CurMon ].CurrentLife >> 6, Monsters[ CurMon ].BaseLife >> 6 );
	InfoPanel_AddLine( InfoPanelBuffer, 1 );
	InfoPanelUsed = 1;
	//BossShowResImun(); // отключено в th1
}

//----- (004445CA) --------------------------------------------------------
void BossShowResImun()
{
	Monster& monster = Monsters[CurMon];
	uchar resImunMask = monster.ResistImmune & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID|MR_11_IM_COLD
	                                           |MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_9_ACID|MR_10_COLD);
	if( resImunMask ){
		if( resImunMask & (MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_9_ACID|MR_10_COLD) ){
			InfoPanel_AddLine("Some Magic Resistances", true);
		}else{
			InfoPanel_AddLine("No resistances", true);
		}
		if( resImunMask & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID|MR_11_IM_COLD) ){
			InfoPanel_AddLine("Some Magic Immunities", true);
		}
	}else{
		InfoPanel_AddLine("No resistances", true);
		InfoPanel_AddLine("No Immunities", true);
	}
}

//----- (00444656) --------------------------------------------------------
void __fastcall ChargeAttackImpact(uint castIndex, int castRow, int castCol)
{
	if( castIndex >= MissilesMax ){
		return;
	}

	Missile& spellCast = Missiles[castIndex];
	uint monsterIndex = spellCast.CasterIndex;
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;

	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	MonsterMap[ castRow ][ castCol ] = monsterIndex + 1;
	monster.Orientation = spellCast.direction;
	monster.Row = castRow;
	monster.Col = castCol;
	FixMonsterPosition(monsterIndex, monster.Orientation);
	if( baseMonsterIndex < BM_72_FIREWING || baseMonsterIndex > BM_75_BLOOD_HULK ){
		if( monster.flag & MF_5_TARGET_MONSTER ){
			DamageMonsterByMonster(monsterIndex, -1, 0);
		}else{
			// for summon may need use this
			// int playerIndex = ( monsterIndex / SummonMonstersPerPlayer_Count ) & 3;
			DamageMonsterByPlayer(monsterIndex, -1, 0); // trap/barrel ? original d1 code
		}
	}else{
		Monster_Fade_In(monsterIndex, monster.Orientation, 0);
	}

	if( monster.flag & MF_5_TARGET_MONSTER ){
		int monsterNum = MonsterMap[ spellCast.Row ][ spellCast.Col ];
		if( monsterNum <= 0 || baseMonsterIndex == BM_40_QUASIT || (baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK) ){
			return;
		}
		MvM_Melee(monsterIndex, monsterNum - 1, 1250, monster.SecondMinDamage, monster.SecondMaxDamage);
		if( baseMonsterIndex >= BM_90_CAVE_SNAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
			return;
		}
		int deltaRow = spellCast.Row + RowDelta[monster.Orientation];
		int deltaCol = spellCast.Col + ColDelta[monster.Orientation];
		if( CheckCellForMeleeMonsterSafeMove(monsterNum - 1, deltaRow, deltaCol) ){
			MonsterMap[ deltaRow ][ deltaCol ] = monsterNum;
			Monsters[monsterNum - 1].Row = deltaRow;
			Monsters[monsterNum - 1].NextRow = deltaRow;
			Monsters[monsterNum - 1].Col = deltaCol;
			Monsters[monsterNum - 1].NextCol = deltaCol;
		}
	}else{// player gets hit with charge attack
		int playerNum = PlayerMap[ spellCast.Row ][ spellCast.Col ];
		int playerIndex = playerNum - 1;
		if( playerNum <= 0 ){
			return;
		}
		if( baseMonsterIndex == BM_40_QUASIT || baseMonsterIndex == BM_C_383_GLOOM || ( baseMonsterIndex >= BM_72_FIREWING && baseMonsterIndex <= BM_75_BLOOD_HULK )){
			return;
		}
		int c_accuracy = 100 + Difficulty * 150 + Dungeon->level * 8;
		int min_dmg = monster.SecondMinDamage + Difficulty * 24 + Dungeon->level / 2;
		int max_dmg = monster.SecondMaxDamage + Difficulty * 24 + Dungeon->level;
		MvP_Melee(monsterIndex, playerNum - 1, c_accuracy, min_dmg, max_dmg);
		if( playerIndex != PlayerMap[ spellCast.Row ][ spellCast.Col ] - 1 ){ // проверка что игрок не отброшен (?)
			return;
		}
		if( baseMonsterIndex >= BM_89_AZURE_DRAKE && baseMonsterIndex <= BM_92_ILLUSION_WEAVER ){
			return;
		}
		Player& player = Players[playerIndex]; 
		if (player.CurAction != PCA_7_GOT_HIT && player.CurAction != PCA_8_DEATH) {
			StartPlayerHit(playerIndex, 0, 0);
		}
		if (PlayerKnockbackResistFail(monsterIndex, playerIndex, true)) {
			StartPlayerHit(playerIndex, 0, 1);
			int deltaRow = spellCast.Row + RowDelta[monster.Orientation];
			int deltaCol = spellCast.Col + ColDelta[monster.Orientation];
			if (CellFreeForPlayer(playerIndex, deltaRow, deltaCol)) {
				// fix to character light getting update after monster charge hit
				ChangeLightPos(player.LightIndex, deltaRow, deltaCol);
				ChangeVision(player.lightRadiusIdentificator, deltaRow, deltaCol);
				player.Row = deltaRow;
				player.Col = deltaCol;
				FixPlayerLocation(playerIndex, player.dir);
				FixPlrWalkTags(playerIndex);
				PlayerMap[deltaRow][deltaCol] = playerIndex + 1;
				SetPlayerOld(playerIndex);
			}
		}
	}
}

//----- (00444901) --------------------------------------------------------
int __fastcall CheckCellForMeleeMonsterSafeMove( int monsterIndex, int row, int col )
{
	int cell; // eax@2
	int result; // eax@4
	int objNum; // ecx@6
	int objIndex; // ecx@9
	result = 0;
	bool ambush = monsterIndex < 0;
	if( ambush ){ // ambush monster special condition
		monsterIndex = -monsterIndex - 1;
	}
	uchar canFly = ambush ? 0 : Monsters[monsterIndex].canFly;
	if( !IsCellBlocking( row, col, canFly) ){
		cell = col + 112 * row;
		if( (!PlayerMap[0][cell] || !ambush && IsPlayerSummon(monsterIndex)) && (MonsterMap[0][cell] == 0 || !ambush && abs(MonsterMap[0][cell]) - 1/* knockback fix */ == monsterIndex) ){
			result = 1;
		}
	}
	objNum = ObjectsMap[ row ][ col ];
	if( result ){
		if( ObjectsMap[ row ][ col ] ){
			if( objNum <= 0 ){
				objIndex = -1 - objNum;
			}else{
				objIndex = objNum - 1;
			}
			if( Objects[ objIndex ].isBlockWalking ){
				result = 0;
			}
		}
		if( result ){
			result = CheckCellForMonsterResist( monsterIndex, row, col );
		}
	}
	return result;
}

//----- (00444981) --------------------------------------------------------
uchar __fastcall CheckCellForMonsterResist( int monsterIndex, int row, int col )
{
	uchar result = 1;
	MissileIndexT castNum = MissilesMap[ row ][ col ];
	if( !castNum || monsterIndex < 0 ){
		return result;
	}
	bool isFireWallCell = 0;
	bool isLightningWallCell = 0;
	if( castNum <= 0 ){ // == Missile_Many
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& spell = Missiles[spellIndex];
			if( spell.Row != row || spell.Col != col ){
				continue;
			}
			int effectIndex = spell.BaseMissileIndex;
			if( effectIndex == MI_5_FIRE_WALL_SEGMENT ){
				isFireWallCell = 1;
			}else if( effectIndex == MI_70_LIGHTNING_WALL_SEGMENT ){
				isLightningWallCell = 1;
			}
		}
	}else{
		int effectIndex = Missiles[castNum-1].BaseMissileIndex;
		if( effectIndex == MI_5_FIRE_WALL_SEGMENT ){
			isFireWallCell = 1;
		}else if( effectIndex == MI_70_LIGHTNING_WALL_SEGMENT ){
			isLightningWallCell = 1;
		}
	}
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = Monsters[monsterIndex].SpritePtr->baseMonsterIndex;
	if( isFireWallCell ){
		if( baseMonsterIndex == 38 || GameMode == GM_HARD){ // on HARD mode firwalls don't stop monsters
			goto LABEL_27;
		}
		switch( baseMonsterIndex ){
		case 110:
			if( MaxCountOfPlayersInGame != 1 ){
			LABEL_27:
				result = 1;
				goto LABEL_20;
			}
			break;
		case 124:
			result = 0;
			break;
		case 195:
			goto LABEL_45;
		}
		if( isFireWallCell && !(monster.ResistImmune & 0x10) ){
			result = 0;
		}
	}
LABEL_20:
	if( !isLightningWallCell || GameMode == GM_HARD){
		return result;
	}
	if( baseMonsterIndex == 38 ){
		goto LABEL_45;
	}
	if( baseMonsterIndex == 58 ){
		if( MaxCountOfPlayersInGame == 1 ){
			goto LABEL_22;
		}
	LABEL_45:
		result &= 1u;
		return result;
	}
	if( baseMonsterIndex == 51 || baseMonsterIndex == 196 || baseMonsterIndex == 197 ){
		goto LABEL_45;
	}
LABEL_22:
	if( isLightningWallCell && !(monster.ResistImmune & 0x20) ){
		result = 0;
	}
	return result;
}

//----- (00444AA0) --------------------------------------------------------
int __fastcall CheckCellForSomeMonsterSafeMove( int monsterIndex, int row, int col )
{
	int cell = 112 * row + col;
	if( IsCellBlocking(row, col, Monsters[monsterIndex].canFly) ){
		return false;
	}
	int objectNumber = ObjectsMap[ 0 ][ cell ];
	if( objectNumber && Objects[abs(objectNumber) - 1].isBlockWalking ){
		return false;
	}
	if( ! CheckCellForMonsterResist(monsterIndex, row, col) ){
		return false;
	}
	return true;
}

//----- (00444AFF) ---------------------X-------Y----------------------------
int __fastcall CheckCellForMonsterCanInteractWithDoorsSafeMove( int monsterIndex, int row, int col )
{
	bool isDoor = false;
	int cell = 112 * row + col;
	int objectNumber = ObjectsMap[ 0 ][ cell ];
	int objectIndex = abs(objectNumber) - 1;

	if( objectNumber ){
		int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
		isDoor = baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL
			|| baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL
			|| baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL
			|| baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL
			|| baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL
			|| baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL; //door

		if (GameMode == GM_HARD && !isDoor) {
			isDoor = baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP
				|| baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE;
		}

		if( Objects[objectIndex].isBlockWalking && !isDoor ){
			return false;
		}
	}
	if (!IsPlayerSummon(monsterIndex) && PlayerMap[0][cell]) {
		return false;
	}
	if( IsCellBlocking(row, col, Monsters[monsterIndex].canFly) && !isDoor || MonsterMap[ 0 ][ cell ] ){
		return false;
	}
	if( !CheckCellForMonsterResist(monsterIndex, row, col) ){
		return false;
	}
	return true;
}

//----- (00444BB8) --------------------------------------------------------
int __fastcall IsMonsterSummonable(int baseMonsterIndex)// монстры в засаде :)
{	// the list of monsters below is used to check if monster is summonable
	if( baseMonsterIndex == BM_7_RISEN_SKELETON
		|| baseMonsterIndex == BM_8_BONE_CREEPER
		|| baseMonsterIndex == BM_9_SKELETON_WARRIOR
		|| baseMonsterIndex == BM_10_BURNING_DEAD
		|| baseMonsterIndex == BM_11_HATRED
		|| baseMonsterIndex == BM_14_SOUL_HUNTER
		|| baseMonsterIndex == BM_20_RISEN_SKELETON
		|| baseMonsterIndex == BM_21_SKELETON_WARRIOR
		|| baseMonsterIndex == BM_22_BURNING_DEAD
		|| baseMonsterIndex == BM_23_MALICE
		|| baseMonsterIndex == BM_24_RISEN_SKELETON
		|| baseMonsterIndex == BM_25_SKELETON_WARRIOR
		|| baseMonsterIndex == BM_26_BURNING_DEAD
		|| baseMonsterIndex == BM_27_HORROR
		|| baseMonsterIndex == BM_92_ILLUSION_WEAVER
		|| baseMonsterIndex == BM_114_FROZEN_HORROR
		|| (baseMonsterIndex >= BM_C_329_SKELETON && baseMonsterIndex <= BM_C_340_HORROR_CAPTAIN)
		|| ( baseMonsterIndex == BM_117_STINGER 
			&& (Dungeon != DUN_48_BLOOD_HIVE && Dungeon != DUN_55_DEEP_HOLLOW && Dungeon != DUN_56_WEB_TUNNELS && Dungeon != DUN_57_ARACHNID_LAIR && Dungeon != DUN_92_FLESH_PIT 
			&& Dungeon != DUN_96_BONE_MIRE))
		|| baseMonsterIndex == BM_100_TORTURED_SPIRIT ){
		return 1;
	}
	//if (is(baseMonsterIndex, 30, 31, 32) && !IsQuestFloor) { // hidden on DLVLs 5-12 allowed to be summoned if it's not a quest floor
	//	return 1;
	//}
	/*if( baseMonsterIndex == 29 ){ // hidden levels 10-20 (now they actually can hide in barrels, bug is fixed)
		if( (char) DungeonLevel < 17 ){ // excluded in abyss to spawn stingers instead
			return 1;
		}
	}*/
	if( baseMonsterIndex == BM_143_DOOM_TAIL || baseMonsterIndex == BM_158_DEATH_RETINUE ){
		return 1;
	}	
	if (baseMonsterIndex == BM_242_REANIMATED_HORROR) {
		return 1;
	}
	if (baseMonsterIndex == BM_47_ACID_HOUND && Dungeon == DUN_28_POISONED_WATER_SUPPLY) {
		return 1;
	}
	if (baseMonsterIndex == BM_253_EXPLODER && (Dungeon == DUN_47_DARK_NEST || Dungeon == DUN_48_BLOOD_HIVE || Dungeon == DUN_89_BEDLAM || Dungeon == DUN_90_FIELDS_OF_MADNESS
		|| Dungeon == DUN_55_DEEP_HOLLOW || Dungeon == DUN_78_THE_BRIDGE || Dungeon == DUN_80_HELL_GATE || Dungeon == DUN_87_PANDEMONIUM || Dungeon == DUN_96_BONE_MIRE)) {
		return 1;
	}
	if (baseMonsterIndex == BM_264_BITELING && Dungeon == DUN_49_SICK_VILLAGERS_BASEMENT) {
		return 1;
	}
	if (baseMonsterIndex == BM_270_MUD_CRAWLER && (Dungeon == DUN_55_DEEP_HOLLOW || Dungeon == DUN_92_FLESH_PIT)) {
		return 1;
	}
	if (baseMonsterIndex == BM_277_SPIDERLING && (Dungeon == DUN_56_WEB_TUNNELS || Dungeon == DUN_57_ARACHNID_LAIR)) {
		return 1;
	}
	if (baseMonsterIndex == BM_82_GARGOYLE && Dungeon == DUN_78_THE_BRIDGE) { return 1; }
	if (baseMonsterIndex == BM_248_SALAMANDER && Dungeon == DUN_83_BLOOD_KEEP) { return 1; }
	return 0;
}

//----- (00444BDD) --------------------------------------------------------
int __fastcall IsMonsterClan(int baseMonsterIndex)
{
	int result = 0;
	if (GameMode == GM_CLASSIC) {
		switch (baseMonsterIndex) {
			case BM_C_353_FLESH_CLAN or BM_C_354_STONE_CLAN or BM_C_355_FIRE_CLAN or BM_C_356_NIGHT_CLAN
			or BM_C_357_FLESH_CLAN_ARCHER or BM_C_358_STONE_CLAN_ARCHER or BM_C_359_FIRE_CLAN_ARCHER or BM_C_360_NIGHT_CLAN_ARCHER:	result = 1;	break;
				break;
		}
	}
	else {
		switch (baseMonsterIndex) {
			case BM_33_MOON_CLAN or BM_34_FLESH_CLAN or BM_35_FIRE_CLAN or BM_36_DARKNESS_CLAN or BM_37_STONE_CLAN
				or BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_124_GLOOM_CLAN
				or BM_165_APOCALYPSE_CLAN or BM_199_HELL_CLAN or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN
				or BM_57_HELL_CLAN
			or BM_175_TERROR_CLAN or BM_186_DEATH_CLAN or BM_187_BLOOD_CLAN or BM_204_SHADOW_CLAN or BM_209_HATRED_CLAN:	result = 1;	break;
				break;
		}
	}
	return result;
}

//----- (00444BF8) --------------------------------------------------------
void __fastcall SummonMinion( int row, int col, int orientation )
{
	int monsterMayTrapCount = 0;// считаем сколько монстров умеют сидеть в засаде
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}

	if( !monsterMayTrapCount ){
		return;
	}

	int randFactor = RNG(monsterMayTrapCount);// выбираем случайного 
	monsterMayTrapCount = 0;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
		if( monsterMayTrapCount > randFactor ){
			int monsterIndex = AddMonster(row, col, orientation, monsterSpriteIndex, 1);
			if( monsterIndex != -1 ){
				Monsters[monsterIndex].IsSummonedByMonster = 1;
				StartMonsterSecondAttack_4_orBlock(monsterIndex, orientation);
			}
			return;
		}
	}
}

//----- (00444CA8) --------------------------------------------------------
void __fastcall PutAmbushMonsterAndStartAttack( int monsterIndex, int row, int col, int orientation )
{
	Monster& monster = Monsters[monsterIndex];
	MonsterMap[ row ][ col ] = monsterIndex + 1;
	monster.Row = row;
	monster.Col = col;
	monster.NextRow = row;
	monster.NextCol = col;
	monster.PrevRow = row;
	monster.PrevCol = col;
	StartMonsterSecondAttack_4_orBlock(monsterIndex, orientation);
}

//----- (00444CF9) --------------------------------------------------------
int __fastcall ActivateAmbushMonster( int monsterIndex, int row, int col )
{
	int v3; // edi@1
	int v4; // ebx@2
	int v5; // eax@3
	int *v6; // esi@7
	int v7; // eax@8
	int loopCount; // eax@12
	int v10; // ecx@12
	int v11; // edx@12
	int v12; // esi@21
	int v13; // edi@21
	int v14; // eax@21
	int checksMap[ 9 ]; // [sp+Ch] [bp-38h]@5
	int v16; // [sp+30h] [bp-14h]@1
	int v17; // [sp+34h] [bp-10h]@1
	int v18; // [sp+38h] [bp-Ch]@4
	int *v19; // [sp+3Ch] [bp-8h]@5
	int v20; // [sp+40h] [bp-4h]@4
	int cola; // [sp+4Ch] [bp+8h]@6

	v17 = monsterIndex;
	v3 = row;
	v16 = row;
	if( monsterIndex == -1 ){
		return 0;
	}
	v4 = col;
	if( !CheckCellForMeleeMonsterSafeMove( -1, row, col ) ){
		v18 = 0;
		v20 = col - 1;
		if( col - 1 <= col + 1 ){
			v19 = checksMap;
			do{
				cola = v3 - 1;
				if( v3 - 1 <= v3 + 1 ){
					v6 = v19;
					do{
						v7 = CheckCellForMeleeMonsterSafeMove( -1, cola, v20 );
						v18 |= v7;
						*v6 = v7;
						v6 += 3;
						++cola;
					}while( cola <= v3 + 1 );
				}
				++v19;
				++v20;
			}while( v20 <= v4 + 1 );
		}
		if( v18 ){
			loopCount = RNG(15) + 1;
			v10 = 0;
			v11 = 0;
			if( loopCount > 0 ){
				do{
					if( checksMap[ v11 + 2 * v10 + v10 ] ){
						--loopCount;
					}
					if( loopCount <= 0 ){
						break;
					}
					if( ++v10 == 3 ){
						v10 = 0;
						if( ++v11 == 3 ){
							v11 = 0;
						}
					}
				}while( loopCount > 0 );
				v3 = v16;
			}
			v12 = v10 + v3 - 1;
			v13 = v11 + v4 - 1;
			v14 = OrientationToTarget( v12, v13, v16, v4 );
			PutAmbushMonsterAndStartAttack( v17, v12, v13, v14 );
			return 1;
		}
		return 0;
	}
	v5 = OrientationToTarget( v3, col, v3, col );
	PutAmbushMonsterAndStartAttack( v17, v3, col, v5 );
	return 1;
	
	#ifdef REFACTOR
	if( monsterIndex == -1 ){
		return 0;
	}
	if( CheckCellForMeleeMonsterSafeMove(-1, row, col) ){ // если можно расположить прямо тут, то располагаем
		int orientation = OrientationToTarget(row, col, row, col);
		PutAmbushMonsterAndStartAttack(monsterIndex, row, col, orientation);
		return 1;
	}
	int checkFlag = 0;// проверяем на доступность все клетки на расстоянии 1 от нужной. составляем карту
	uint checksMap[9];
	for( int colOffset = -1; colOffset <= 1; colOffset++ ){
		for( int rowOffset = -1; rowOffset <= 1; rowOffset++ ){
			int isSafe = CheckCellForMeleeMonsterSafeMove(-1, row + rowOffset, col + colOffset);
			checkFlag |= isSafe;
			checksMap[3*(rowOffset+1)+colOffset+1] = isSafe;
		}
	}
	if( !checkFlag ){// если все заняты, то не добавляем
		return 0;
	}
	int loopCount = RNG(15) + 1;// выбираем случайную клетку из доступных
	int mapIndex = 0;
	do{
		if( checksMap[mapIndex] ){
			loopCount--;
		}
		if( loopCount <= 0 ){
			break;
		}
		Wrap(++mapIndex, 9);
	}while( loopCount > 0 );

	int rowOffset = mapIndex / 3 - 1;// и туда сажаем монстра
	int colOffset = mapIndex % 3 - 1;
	int monsterRow = row + rowOffset;
	int monsterCol = col + colOffset;
	int orientation = OrientationToTarget(monsterRow, monsterCol, row, col);
	PutAmbushMonsterAndStartAttack(monsterIndex, monsterRow, monsterCol, orientation);
	return 1;
	#endif
}

//----- (00444DFD) --------------------------------------------------------
int AddAmbushMonster()
{
	int monsterMayTrapCount = 0;// считаем сколько монстров умеют сидеть в засаде
	if( Dungeon->isQuest ) return -1;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}
	if( monsterMayTrapCount ){
		int randFactor = RNG(monsterMayTrapCount);// выбираем случайного 
		monsterMayTrapCount = 0;
		for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; ++monsterSpriteIndex ){
			if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
				monsterMayTrapCount++;
			}
			if( monsterMayTrapCount > randFactor ){
				int monsterIndex = AddMonster(0, 0, 0, monsterSpriteIndex, 0);
				if( monsterIndex != -1 ){
					FixMonsterPosition(monsterIndex, 0);
				}
				return monsterIndex;
			}
		}

	}
	return -1;
}

//----- (00444E97) --------------------------------------------------------
void __fastcall TalkWithMonster(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count )return;

	Monster& monster = Monsters[monsterIndex];
    int playerIndex = CurrentPlayerIndex; // was monster.TargetIndex
    monster.CurAction = A_17_TALK;
    if( B(monster.ai) & (B(AI_23_Snotspll) | B(AI_30_Lachdanan)) ){
        if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
            if( Quests[Q_7_OGDENS_SIGN].status2 == 2 ){
                int itemInventoryIndex;
                if( FindItemInInventory(playerIndex, BI_327_TAVERN_SIGN, itemInventoryIndex) ){
                    ClearInventorySlot(playerIndex, itemInventoryIndex);
                    Quests[Q_7_OGDENS_SIGN].status = QS_3_COMPLETE;
                    monster.speechIndex = SP_22;
                    monster.goal = MG_6_TALK;
                }
            }
        }
        if( IsQuestOnLevel(Q_4_LACHDANAN) ){
            if( monster.speechIndex >= SP_81 ){
                int itemInventoryIndex;
                if( FindItemInInventory(playerIndex, BI_328_GOLDEN_ELIXIR, itemInventoryIndex) ){
                    ClearInventorySlot(playerIndex, itemInventoryIndex);
                    monster.speechIndex = SP_83;
                    monster.goal = MG_6_TALK;
                }
            }
        }
    }
}

//----- (00444F5C) --------------------------------------------------------
void __fastcall MakeGolem( unsigned int summonIndex, int row, int col, int spellIndex )
{
	if( summonIndex >= Monsters_Max_Count ){
		return;
	}
 
	DSummonStr params;
	const int ownerIndex = summonIndex / SummonMonstersPerPlayer_Count;
	const Player& owner = Players[ownerIndex];
	const Missile& missile = Missiles[spellIndex];
	
	int spriteIndex = 0;
    switch( missile.BaseMissileIndex ){
        case MI_33_GOLEM:           spriteIndex = 0; break;
        case MI_122_LESSER_SUMMON:  spriteIndex = 1; break;
        case MI_123_COMMON_SUMMON:  spriteIndex = 2; break;
        case MI_124_GREATER_SUMMON: spriteIndex = 3; break;
    }
	
	if( spriteIndex != 0 ){
        switch( owner.fullClassId ){
        case PFC_DEMONOLOGIST:      spriteIndex += 0; break;
        case PFC_NECROMANCER:       spriteIndex += 3; break;
        case PFC_BEASTMASTER:       spriteIndex += 6; break;
        }
    }
    
    params.spriteIndex = spriteIndex;
    
    int summonType = 0;
    switch( missile.BaseMissileIndex ){
        case MI_33_GOLEM:           summonType = 0; break;
        case MI_122_LESSER_SUMMON:  summonType = 1; break;
        case MI_123_COMMON_SUMMON:  summonType = 2; break;
        case MI_124_GREATER_SUMMON: summonType = 3; break;
    }
    params.summonType = summonType;
    
    int slvl = missile.SpellLevel;
	int clvl = owner.CharLevel;
	
	int summonLife = 0;
	int toHit = 0;
	int minDamage = 0;
	int maxDamage = 0;
	int toHitSecond = 0;
	int minDamageSecond = 0;
	int maxDamageSecond = 0;
	int armorClass = 0;
    
	// Affix modifiers:
	// +HP: found: all armor (5 items): up to +100 HP (max + 500)
	// +damage: found: staff/shield: up to +30 (max +60) 
	
	// +AC: found: all armor: up to +20 (max +100)    : 150
	// +tohit: found: jewelry: up to +30 (max +90) : 160
	// +%ED: found: staff/shield: up to +25% (max +50% ) - +90 max: 160
	// +HP%: found: all armor (5 items): up to +25% (total +125% ) - HPs are even if arrives at 400 max by end of horror (900 if +125% HP or with +500 HP)
    
	// max params are considered: 400 cMAG, 50 CLVL, 2000 mana
    switch( MonsterSprites[spriteIndex].baseMonsterIndex ){ //   *************** ADD +CLVL/2 to damage to increase it early on! **************************
        case BM_223_DEMONOLOG_SUMMON_1: // impling
		{
			summonLife = (((owner.CurMagic * clvl) / 22) << 6) + ((3 * clvl) << 6) + (owner.MaxCurMana / 3) + RNG(((clvl + 15) << 6)); // start with: 25-40, end with: 1200
			toHit = owner.CurMagic * clvl / 666 /* 30 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 30 + 25 + 25 + 12 +[0-11] = [92-103]
			minDamage = owner.CurMagic * clvl / 500 /* 40 */ // divide by 400!
				+ (GameMode == GM_HARD ? (clvl / 8) : (clvl / 4))
				+ slvl / 2
				+ 2
				+ RNG(((clvl / 5) + 3)); // starts with: [2-4], ends with: 40 + 12 + 25 + 2 + [0-12] = [79-91]
			maxDamage =
				owner.CurMagic * clvl / 400 /* 50 */
				+ (GameMode == GM_HARD ? (clvl / 8) : (clvl / 4))
				+ slvl / 2
				+ (is(GameMode, GM_EASY, GM_CLASSIC) ? 7 : 6)
					+ RNG(((clvl / 4) + 4 )) ; // starts with: [6-9], ends with: 50 + 12 + 25 + 6 + [0-28] = [93-108]
			toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156...  they don't really use it....
            minDamageSecond = owner.CurMagic * clvl / 337 /* 37 */ + clvl / 2 + slvl + RNG(( ( clvl / 5) + 2 ) ); // starts with: [1-2], ends with: 37 + 25 + 55 + [0-11] = [117-128]
            maxDamageSecond = owner.CurMagic * clvl / 250 /* 50 */ + clvl / 2 + slvl + 2 + RNG(( ( clvl / 3) + 2 ) ) ; // starts with: [3-4], ends with: 50 + 25 + 55 + 2 + [0-17] = [132-149]
			armorClass = owner.CurMagic * clvl / 2000 /* 10 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 10 + 25 + 50 +[0-9] = [85-94]
                break;
            }
        case BM_224_DEMONOLOG_SUMMON_2: // goat archer
            {
                summonLife =  (((owner.CurMagic * clvl) / 13) << 6) + (owner.MaxCurMana / 3) + RNG((((2 * clvl) + 15) << 6)); // start with: 24, end with: 2200
                toHit = owner.CurMagic * clvl / 666 + clvl / 3 + slvl + RNG(( clvl / 10) + 7); //  from: 1 + [0-6] = [1-7]  to: 30 + 16 + 50 + [0-11] = [96-107]
                minDamage = owner.CurMagic * clvl / 500 /* 40 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 3
					+ 4 
					+ RNG(( ( clvl / 5) + 4 ) ); // starts with: [5-8], ends with: 40 + 50 + 16 + 4 + [0-13] = [85-98]
                maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 3
					+ (is(GameMode, GM_EASY, GM_CLASSIC) ? 15 : 13)
					+ RNG(( ( clvl / 8) + 5 ) ) ; // starts with: [14-18], ends with: 50 + 50 + 16 + 13 + [0-10] = [104-114]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 260 /* 48 */ + clvl / 2 + slvl + RNG((( clvl / 5) + 2 )); // starts with: [1-2], ends with: 48 + 25 + 55 + [0-11] = [128-139]
                maxDamageSecond = owner.CurMagic * clvl / 208 /* 60 */ + clvl / 2 + slvl + 2 + RNG((( clvl / 3) + 2 )) ; // starts with: [3-4], ends with: 60 + 25 + 55 + 2 + [0-17] = [142-159]
                armorClass = owner.CurMagic * clvl / 2000 /* 10 */ + clvl / 2 + slvl + RNG((( clvl / 10) + 5 )); // from [1-5]  to = 10 + 25 + 50 +[0-9] = [85-94]
                break;
            }
        case BM_225_DEMONOLOG_SUMMON_3: // satyr lord
            {
                summonLife =  ((owner.CurMagic * clvl / 5) << 6) + (owner.MaxCurMana * 2) + RNG((((10 * clvl) + 15) << 6)); // start with: [120-144], end with: 12500 + 1000 + [0-514] = [9500-10500]
				toHit = owner.CurMagic * clvl / 400 /* 50 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 50 + 25 + 25 + 12 +[0-11] = [112-123]
				minDamage = owner.CurMagic * clvl / 500 /* 40 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 2 
					+ 4 
					+ RNG(((clvl / 7) + 4)); // starts with: [5-8], ends with: 40 + 50 + 25 + 4 + [0-10] = [95-104]
				maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 2 
					+ 9 
					+ RNG(((clvl / 8) + 5)); // starts with: [9-13], ends with: 50 + 50 + 25 + 8 + [0-10] = [109-119]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 260 /* 48 */ + clvl / 2 + slvl + RNG(( ( clvl  / 5) + 2 ) ); // starts with: [1-2], ends with: 48 + 25 + 55 + [0-11] = [128-139]
                maxDamageSecond = owner.CurMagic * clvl / 208 /* 60 */ + clvl / 2 + slvl + 12 + RNG(( ( clvl / 3) + 2 ) ) ; // starts with: [3-4], ends with: 60 + 25 + 55 + 2 + [0-17] = [152-169]
				armorClass = owner.CurMagic * clvl / 300 /* 40 */ + clvl / 2 + 2 * slvl + 1 + RNG(((clvl / 10) + 5)); // from [2-6]  to = 40 + 25 + 51 +[0-9] = [116-125]
				break;
            }        
		case BM_226_NECROMANT_SUMMON_1: // skeletal archer
            {
                summonLife =  (((owner.CurMagic * clvl) / 16) << 6) + (owner.MaxCurMana / 5) + RNG(((clvl + 5) << 6) ); // start with: 15-20, end with: [1500-1600]
				toHit = owner.CurMagic * clvl / 500 /* 40 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 40 + 25 + 25 + 12 +[0-11] = [102-113]
				minDamage = owner.CurMagic * clvl / 500 /* 40 */
					+ (GameMode == GM_HARD ? (clvl / 6) : (clvl / 3))
					+ (slvl / 2) 
					+ (is(GameMode, GM_EASY, GM_CLASSIC) ? 6 : 5)
					+ RNG(((clvl / 10) + 2 )); // starts with: [5-6], ends with: 40 + 16 + 25 + 4 + [0-6] = [85-91]
                maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 6) : (clvl / 3))
					+ (slvl / 2) 
					+ 8 
					+ RNG(((clvl / 7) + 5)) ; // starts with: [7-11], ends with: 50 + 16 + 25 + 8 + [0-10] = [99-109]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 337 /* 37 */ + clvl / 2 + slvl + RNG(( ( clvl / 5) + 2 ) ); // starts with: [1-2], ends with: 37 + 25 + 55 + [0-11] = [117-128]
                maxDamageSecond = owner.CurMagic * clvl / 250 /* 50 */ + clvl / 2 + slvl + 2 + RNG(( ( clvl / 3) + 2 ) ) ; // starts with: [3-4], ends with: 50 + 25 + 55 + 2 + [0-17] = [132-149]
				armorClass = owner.CurMagic * clvl / 2000 /* 10 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 10 + 25 + 50 +[0-9] = [85-94]
				break;
            }
        case BM_227_NECROMANT_SUMMON_2: // skeletal warrior
            {
                summonLife =  (((owner.CurMagic * clvl) / 7) << 6) + (owner.MaxCurMana / 2) + RNG((((2 * clvl) + 15) << 6) ); // start with: 24, end with: 4166 + 250 + [0-115] = [4400-4500]
				toHit = owner.CurMagic * clvl / 333 /* 60 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18] to: 60 + 25 + 25 + 12 +[0-11] = [122-133]
				minDamage = owner.CurMagic * clvl / 666 /* 30 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2))
					+ slvl / 2 
					+ (is(GameMode, GM_EASY, GM_CLASSIC) ? 8 : 7)
					+ RNG(((clvl / 5) + 6)); // starts with: [6-11], ends with: 30 + 25 + 25 + 7 + [0-15] = [87-102]
				maxDamage = owner.CurMagic * clvl / 500 /* 40 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2))
					+ slvl / 2 
					+ (is(GameMode, GM_EASY, GM_CLASSIC) ? 16 : 14)
					+ RNG(((clvl / 8) + 5)); // starts with: [15-19], ends with: 40 + 25 + 25 + 14 + [0-10] = [104-114]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 260 /* 48 */ + clvl / 2 + slvl + RNG(( ( clvl / 5) + 2 ) ); // starts with: [1-2], ends with: 48 + 25 + 55 + [0-11] = [128-139]
                maxDamageSecond = owner.CurMagic * clvl / 208 /* 60 */ + clvl / 2 + slvl + 2 + RNG(( ( clvl / 3) + 2 ) ) ; // starts with: [3-4], ends with: 60 + 25 + 55 + 2 + [0-17] = [142-159]
				armorClass = owner.CurMagic * clvl / 500 /* 40 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 40 + 25 + 50 +[0-9] = [115-124]
				break;
            }
        case BM_228_NECROMANT_SUMMON_3: // greater skeleton
            {
                summonLife =  ((owner.CurMagic * clvl / 5) << 6) + (owner.MaxCurMana * 2) + RNG((((10 * clvl) + 15) << 6) ); // start with: [120-144], end with: 12500 + 1000 + [0-514] = [9500-10500]
				toHit = owner.CurMagic * clvl / 300 /* 66 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 66 + 25 + 25 + 12 +[0-11] = [128-139]
				minDamage = owner.CurMagic * clvl / 500 /* 40 */
					+ (GameMode == GM_HARD ? (clvl / 2) : (clvl / 2))
					+ slvl / 2 
					+ 5 
					+ RNG(((clvl / 7) + 6)); // starts with: [6-11], ends with: 40 + 25 + 25 + 5 + [0-12] = [95-107]
				maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 2) : (clvl / 2))
					+ slvl / 2 
					+ 20 
					+ RNG(((clvl / 8) + 5)); // starts with: [15-19], ends with: 50 + 25 + 25 + 20 + [0-10] = [110-120]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 250 /* 80 */ + clvl + slvl / 2 + 13 + RNG((clvl / 2) + 4); //  from: 15 +[0-3] = [15-18]  to: 80 + 50 + 25 + 13 +[0-29] = [168-196]
				maxDamageSecond = owner.CurMagic * clvl / 200 /* 100 */ + clvl + slvl / 2 + 24 + RNG((clvl + 6)); // starts with: [24-30], ends with: 100 + 50 + 25 + 23 + [0-10] = [199-254]
				armorClass = owner.CurMagic * clvl / 500 /* 40 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 40 + 25 + 50 +[0-9] = [115-124]
				break;
            }
        case BM_229_BEASTMASTER_SUMMON_1: // stinger
            {
                summonLife =  (((owner.CurMagic * clvl) / 17) << 6) + ((5 * clvl) << 6) + (owner.MaxCurMana / 3) + RNG(((clvl + 15) << 6) ); // start with: 28-43, end with: 2250
				toHit = owner.CurMagic * clvl / 666 /* 30 */ 
					+ clvl / 2
					+ slvl / 2 
					+ 12 
					+ RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 30 + 25 + 25 + 12 +[0-11] = [92-103]
				minDamage = owner.CurMagic * clvl / 500 /* 40 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 3))
					+ slvl / 2 
					+ 5 
					+ RNG(( ( clvl / 5) + 3 ) ); // starts with: [5-7], ends with: 40 + 16 + 25 + 5 + [0-12] = [86-98]
                maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 3))
					+ slvl / 2 
					+ 8 
					+ RNG(( ( clvl / 3) + 4 ) ) ; // starts with: [8-11], ends with: 50 + 16 + 25 + 8 + [0-19] = [99-118]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 337 /* 37 */ + clvl / 2 + slvl + RNG(( ( clvl / 5) + 2 ) ); // starts with: [1-2], ends with: 37 + 25 + 55 + [0-11] = [117-128]
                maxDamageSecond = owner.CurMagic * clvl / 250 /* 50 */ + clvl / 2 + slvl + 2 + RNG(( ( clvl / 3) + 2 ) ) ; // starts with: [3-4], ends with: 50 + 25 + 55 + 2 + [0-17] = [132-149]
				armorClass = owner.CurMagic * clvl / 800 /* 25 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 25 + 25 + 50 +[0-9] = [100-109]
				break;
            }
        case BM_230_BEASTMASTER_SUMMON_2: // boar
            {
                summonLife =  (((owner.CurMagic * clvl) / 6 ) << 6) + (owner.MaxCurMana / 2) + RNG((((2 * clvl) + 15) << 6) ); // start with: 24, end with: 4166 + 250 + [0-115] = [4400-4500]
				toHit = owner.CurMagic * clvl / 500 /* 40 */ + clvl / 2 + slvl / 2 + 12 + RNG((clvl / 10) + 7); //  from: 12 +[0-6] = [12-18]  to: 40 + 25 + 25 + 12 +[0-11] = [102-113]
				minDamage = owner.CurMagic * clvl / 500 /* 40 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2))
					+ slvl / 2 
					+ 5 
					+ RNG(((clvl / 5) + 5)); // starts with: [6-10], ends with: 40 + 25 + 25 + 5 + [0-14] = [95-109]
				maxDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2))
					+ slvl / 2 
					+ 14 
					+ RNG(((clvl / 8) + 5)); // starts with: [15-19], ends with: 50 + 25 + 25 + 14 + [0-10] = [114-124]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 500 /* 40 */ + clvl + slvl / 2 + 5 + RNG(((clvl / 5) + 5)); // starts with: [6-10], ends with: 40 + 50 + 25 + 5 + [0-14] = [120-134]
				maxDamageSecond = owner.CurMagic * clvl / 400 /* 50 */ + clvl + slvl / 2 + 14 + RNG(((clvl / 8) + 5)); // starts with: [15-19], ends with: 50 + 50 + 25 + 14 + [0-10] = [139-149]
				armorClass = owner.CurMagic * clvl / 500 /* 40 */ + clvl / 2 + slvl + RNG(((clvl / 10) + 5)); // from [1-5]  to = 40 + 25 + 50 +[0-9] = [115-124]
				break;
            }
        case BM_231_BEASTMASTER_SUMMON_3: // siegebreaker
            {
                summonLife =  ((owner.CurMagic * clvl / 2) << 6) + (owner.MaxCurMana * 2) + RNG((((10 * clvl) + 15) << 6) ); // start with: [120-144], end with: 12500 + 1000 + [0-514] = [11000-12000]
				toHit = owner.CurMagic * clvl / 266 /* 75 */ + clvl / 2 + slvl / 2 + 15 + RNG((clvl / 10) + 7); //  from: 15 +[0-6] = [15-21] to: 75 + 25 + 25 + 15 +[0-11] = [140-151]
				minDamage = owner.CurMagic * clvl / 400 /* 50 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 2 
					+ 7 
					+ RNG(((clvl / 5) + 5)); // starts with: [8-14], ends with: 50 + 25 + 25 + 7 + [0-14] = [107-121]
				maxDamage = owner.CurMagic * clvl / 333 /* 60 */ 
					+ (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2))
					+ slvl / 2 
					+ 18 
					+ RNG(((clvl / 8) + 5)); // starts with: [18-22], ends with: 60 + 25 + 25 + 18 + [0-10] = [128-138]
				toHitSecond = (3 * slvl / 2) + 3 * clvl / 2; // from 4 to 156
				minDamageSecond = owner.CurMagic * clvl / 400 /* 50 */ + clvl + slvl / 2 + 14 + RNG((clvl / 10) + 5); //  from: 15 +[0-4] = [15-19]  to: 50 + 50 + 25 + 14 +[0-9] = [139-148]
				maxDamageSecond = owner.CurMagic * clvl / 300 /* 66 */ + clvl + slvl / 2 + 23 + RNG(((clvl / 8) + 5)); // starts with: [24-28], ends with: 66 + 50 + 25 + 23 + [0-10] = [164-174]
				armorClass = owner.CurMagic * clvl / 400 /* 50 */ + clvl / 2 + slvl + 1 + RNG(((clvl / 10) + 5)); // from [2-6]  to = 50 + 25 + 51 +[0-9] = [126-135]
				break;
            }
        case BM_109_GOLEM:
		default:
		{
			if (GameMode == GM_CLASSIC) {
				/*
				JG:
				HP: 2·maxmana/3 + 10·slvl
				AC: 25
				To Hit: 40 + 2·clvl + 5·slvl
				Min. Damage: 8 + 2·slvl
				Max. Damage: 16 + 2·slvl
				*/
				summonLife = 2 * owner.MaxCurMana / 3 + ((10 * slvl) << 6);
				armorClass = 25;
				toHit = 40 + 2 * clvl + 5 * slvl;
				minDamage = 8 + 2 * slvl;
				maxDamage = 16 + 2 * slvl; 
				// the rest is not used but i'll leave it here just because
				toHitSecond = slvl + 2 * clvl;
				minDamageSecond = 2 * (slvl + 1);
				maxDamageSecond = 4 * (slvl + 10);
			}
			else {
				summonLife = (((owner.CurMagic * clvl) / 10) << 6) // 400 * 50 / 10 = 2000
					+ ((clvl * clvl) << 6) // from 1 on clvl 1 to 2500 on clvl 50
					+ 6400 // + 100
					+ owner.MaxCurMana // 1000
					+ RNG(((10 * clvl) << 6)); // start with: 100 + 100 + 50 = 250, end with: 4000 + 1000 + [0-500] = [5000-5500]
				//summonLife = (((slvl + DungeonLevel) * 20) << 6) + ( owner.MaxCurMana / 2 );
				toHit = slvl + 2 * clvl;
				minDamage = owner.CurMagic * clvl / 1000 /* 20 */
					+ clvl / 2
					+ 1
					+ slvl
					+ RNG(((clvl / 5) + 2)); // starts: [1-2], ends: 20 + 25 + 51 + [0-11] = [96-107]
				maxDamage = owner.CurMagic * clvl / 500 /* 40 */
					+ 3
					+ clvl / 2
					+ slvl
					+ RNG(((clvl / 3) + 2)); // starts: [3-4], ends: 40 + 3 + 25 + 50 + [0-17]= [118-135]
				toHitSecond = slvl + 2 * clvl;
				minDamageSecond = 2 * (slvl + 1);
				maxDamageSecond = 4 * (slvl + 10);
				armorClass = owner.CurMagic * clvl / 500 /* 40 */ + (clvl / 2) + slvl + RNG(((clvl / 10) + 2)); // from [1-2]  to 40 + 25 + 50 +[0-6] = [115-121]
				break;
			}
		}
    }
    
	// relocated from here .... ^

	int best_defense_trait_penalty = HasTrait(ownerIndex, TraitId::BestDefense) ? + ( 25 + (owner.CharLevel / 15)) : 0;
	summonLife += summonLife * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	summonLife += ((summonLife * owner.summonHpPercent) / 100 + (owner.summonHp << 6)) ; // hp increase from affixes
	summonLife += PerkValue(PERK_TOUGH_MINIONS, ownerIndex) << 6; // +hp from perks
	params.summonLife = summonLife * (100 - best_defense_trait_penalty) / 100; //added here because it changes TOTAL hit points. including affix and perk buffs
 
	toHit += owner.summonToHit;
	toHit += toHit * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	toHit += PerkValue(PERK_ACCURATE_MINIONS, ownerIndex); // +accuracy from perks
	params.toHit = toHit;
 
	int best_defense_benefit = HasTrait(ownerIndex, TraitId::BestDefense) ? (25 + owner.CharLevel / 15) : 0;
	minDamage += minDamage * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100; 
	minDamage += minDamage * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMin;
	minDamage += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, ownerIndex); // +damage from perks

	maxDamage += maxDamage * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	maxDamage += maxDamage * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMax;
	maxDamage += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, ownerIndex); // +damage from perks

	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
		minDamage *= 2;
		maxDamage *= 2;
	}
	params.minDamage = minDamage;
	params.maxDamage = maxDamage;
 
	toHitSecond += owner.summonToHit;
	toHitSecond += PerkValue(PERK_ACCURATE_MINIONS, ownerIndex); // +accuracy from perks
	params.toHitSecond = toHitSecond;
 	
	minDamageSecond += (minDamageSecond * owner.summonDamagePercent) / 100 + owner.summonDamageMin;
	minDamageSecond += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + best_defense_benefit; // +damage from perks
	maxDamageSecond += (maxDamageSecond * owner.summonDamagePercent) / 100 + owner.summonDamageMax;
	maxDamageSecond += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + best_defense_benefit; // +damage from perks
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
		minDamageSecond *= 2;
		maxDamageSecond *= 2;
	}
	params.minDamageSecond = minDamageSecond;
	params.maxDamageSecond = maxDamageSecond;

	armorClass += armorClass * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	armorClass += (armorClass * owner.summonAcPercent) / 100 + owner.summonAc;
	armorClass += PerkValue(PERK_ARMORED_MINIONS, ownerIndex); // +armor from perks
	params.armorClass = armorClass;

	LimitToMin(params.summonLife     , 1);
	LimitToMin(params.toHit          , 1);
	LimitToMin(params.minDamage		 , 1);
	LimitToMin(params.maxDamage		 , 1);
	LimitToMin(params.toHitSecond	 , 1);
	LimitToMin(params.minDamageSecond, 1);
	LimitToMin(params.maxDamageSecond, 1);
	LimitToMin(params.armorClass	 , 1);
	
	if( ownerIndex == CurrentPlayerIndex ){
		NetSendCmdAwakeSummon( summonIndex, row, col, (DUNGEON)Dungeon, params );
	}
}

//---- (th2) --------------------------------------------------------------
void __fastcall AwakeSummon( uint summonIndex, int row, int col, int currentLife, const DSummonStr& params, bool restoring )
{
    if( summonIndex >= Monsters_Max_Count ){
		return;
	}
 
	Monster& summon = Monsters[summonIndex];
	const int ownerIndex = summonIndex / SummonMonstersPerPlayer_Count;
	const Player& owner = Players[ownerIndex];
	
	int spriteIndex = params.spriteIndex;
	CreateMonster( summonIndex, 0, spriteIndex, 0, 0 );
    
    if( ownerIndex == CurrentPlayerIndex && !restoring ){
        summon.summonOrder = 100;
        RecalcSummonOrder( ownerIndex, params.summonType );
    }
	if( Dungeon->genType == DT_0_TOWN ){
		LimitToRange( row, 15, 111 - 16 );
		LimitToRange( col, 15, 111 - 16 );
	}
	MonsterMap[ row ][ col ] = summonIndex + 1;
 
	summon.Row = row;
	summon.Col = col;
	summon.NextRow = row;
	summon.NextCol = col;
	summon.PrevRow = row;
	summon.PrevCol = col;
 
	LOBYTE_IDA(summon.pathCount) = 0;

	summon.BaseLife = params.summonLife;
	summon.CurrentLife = currentLife;
	summon.ToHit = params.toHit;
	summon.MinDamage = params.minDamage;
	summon.MaxDamage = params.maxDamage;
	summon.SecondMinDamage = params.minDamageSecond;
	summon.SecondMaxDamage = params.maxDamageSecond;
	summon.ArmorClass = params.armorClass;
 
	summon.flag |= MF_6_FRIENDLY | MF_11_NOENEMY; // fix summon don't attack monster
	
	if( !restoring ){
        StartMonsterSecondAttack_4_orBlock(summonIndex, 0);
     
        int soundIndex = -1;
        if( summon.SpritePtr->baseMonsterIndex == BM_109_GOLEM ){
            soundIndex = S_116_GOLUM;
        }else{
            PlayMonsterSound( summonIndex, MST_1_GOTHIT );
        }
        
        if( soundIndex != -1 ){
            PlayLocalSound( soundIndex, summon.Row, summon.Col );
        }
    }
	
	MonsterLookAround(summonIndex);
}

//----- (0044508C) --------------------------------------------------------
bool __fastcall MonsterTalking(uint monsterIndex)
{
	return monsterIndex < Monsters_Max_Count && is( Monsters[monsterIndex].goal, MG_6_TALK, MG_7_WAIT_TO_TALK);
}

//----- (004450B5) --------------------------------------------------------
int __fastcall CheckMonsterPhaseOrBlock(uint monsterIndex, int* isMonsterHit)
{
	Monster& monster = Monsters[monsterIndex];
	int result = 0;
	if( monsterIndex < Monsters_Max_Count ){
		if( monster.ai == AI_12_Gargoyle && monster.flag & MF_3_STONE ){ // горгулья в стоун моде
			monster.flag &= ~MF_3_STONE;
			monster.CurAction = A_7_SECOND;
			*isMonsterHit = 1;
			return 1;
		}
		int baseMonster = monster.SpritePtr->baseMonsterIndex;
		if( baseMonster == BM_107_DARK_MAGE
		 || baseMonster == BM_108_MAGISTRATE
		 || baseMonster == BM_174_ZHAR_BASEMNSTR
		 || baseMonster == BM_181_ADVOCATE
		 || baseMonster == BM_200_CABALIST
		 || baseMonster == BM_201_ADEPT ){ // неуязвимость магов в фазе (телепорт?) 
			result = 1;
			if( monster.goal != MG_1_ATTACK ){
				*isMonsterHit = 0;
				return result;
			}
		}
		int monBlockRate = 70; // Normal & Survival
		switch(GameMode){
		case GM_IRONMAN or GM_EASY or GM_CLASSIC:			monBlockRate = 60; break;
		case GM_HARD:										monBlockRate = 75; break;
		case GM_SPEEDRUN or GM_NIGHTMARE or GM_HARDCORE:	monBlockRate = 65; break;
			break;
		}
		if( (baseMonster == BM_162_CORRUPT_PALADIN || baseMonster == BM_208_WARPED_ONE)
		 && monster.CurAction < A_15_STONE
		 && monster.CurAction != A_1_WALK_UP 
		 && monster.CurAction != A_2_WALK_DOWN 
		 && monster.CurAction != A_3_WALK_HORIZ
		 && RNG(100) < monBlockRate ){
			StartMonsterSecondAttack_4_orBlock( monsterIndex, monster.Orientation ); // There block 
			*isMonsterHit = 1;
			result = 1;
		}else{
			result = 0;
		}
	}
	return result;
}

//----- (00445118) --------------------------------------------------------
int __fastcall encode_enemy( int monsterIndex )
{
	Monster& monster = Monsters[monsterIndex];
	int result = monster.TargetIndex;
	if (monster.flag & MF_5_TARGET_MONSTER) {
		result += 4;
	}
	return result;
}

//----- (00445138) --------------------------------------------------------
void __fastcall decode_enemy( int monsterIndex, int targetIndex )
{
	LimitToRange(targetIndex, 0, Monsters_Max_Count + PlayersMax_4);
	Monster& monster = Monsters[monsterIndex];
	if( targetIndex >= 4 ){
		int targetMonsterIndex = targetIndex - 4;
		Monster& targetMonster = Monsters[targetMonsterIndex];
		monster.flag |= MF_5_TARGET_MONSTER;
		monster.TargetIndex = targetMonsterIndex;
		monster.TargetRow = targetMonster.NextRow;
		monster.TargetCol = targetMonster.NextCol;
	}else{
		Player& targetPlayer = Players[targetIndex];
		monster.flag &= ~MF_5_TARGET_MONSTER;
		monster.TargetIndex = targetIndex;
		monster.TargetRow = (char) targetPlayer.NextRow;
		monster.TargetCol = (char) targetPlayer.NextCol;
	}
	LimitToRange(monster.TargetRow, 0, FineMap_112);
	LimitToRange(monster.TargetCol, 0, FineMap_112);
}

//int OldWorldProcess;
//----- (004451A3) --------------------------------------------------------
void __fastcall PlayVideo(char* videoName, int isSkipable)
{
	void* vidHanler = 0;
	struct tagMSG Msg;
	if( IsMainWindowActive ){
		//OldWorldProcess = gbRunGame;
		//gbRunGame = 0;
		ProcHandler prevHandler = SetWindowProc(MovieWindowProc);
		InvalidateRect(ghMainWnd, 0, 0);
		UpdateWindow(ghMainWnd);
		VideoWindowExists = 1;
		PauseMusic(true);
		PauseAmbient(true);
		StopSpeech();
		PlaySoundFile(S_77_BLANK);
		SVidPlayBegin(videoName, 0, 0, 0, 0, LoopMovie != 0 ? (SVID_AUTOCUTSCENE|SVID_FLAG_LOOP) & ~SVID_FLAG_CLEARSCREEN : SVID_AUTOCUTSCENE, &vidHanler);
		if( vidHanler ){
			do{
				XinputVirtualKeyboard();
				if( !IsMainWindowActive )
					break;
				if( isSkipable && !VideoWindowExists )
					break;
				while( PeekMessageA(&Msg, 0, 0, 0, 1u) ){
					if( Msg.message != WM_QUIT ){
						TranslateMessage(&Msg);
						DispatchMessageA(&Msg);
					}
				}
			}while( SVidPlayContinue() && vidHanler );
			if( vidHanler )
				SVidPlayEnd(vidHanler);
		}
		VideoWindowExists = 0; // иногда не сбрасывается в PlayVideoHandler (в th1 тоже)
		if( DDraw && ScreenSurface32 ){ // clear screen
			POINT p = { 0, 0 };
			ClientToScreen(ghMainWnd, &p);
			RECT windowRect;
			GetClientRect(ghMainWnd, &windowRect);
			OffsetRect(&windowRect, p.x, p.y);
			DDBLTFX ddbltfx;
			ZeroMemory(&ddbltfx, sizeof(ddbltfx));
			ddbltfx.dwSize = sizeof(ddbltfx);
			ddbltfx.dwFillColor = 0;
			ScreenSurface32->Blt(&windowRect, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		}
		SetWindowProc( prevHandler );
		PauseMusic(false);
		PauseAmbient(false);
		//gbRunGame = OldWorldProcess;
	}
}

//----- (0044529E) --------------------------------------------------------
int __stdcall MovieWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_KEYUP && wParam == VK_SNAPSHOT ){
		extern int DoMenuScreenShot; DoMenuScreenShot = 1;
	}
	if( msg == WM_KEYDOWN || msg == WM_CHAR ){
LABEL_9:
		VideoWindowExists = 0; // это, видимо, вместо WM_QUIT/WM_CLOSE
		return DefaultHandler(hWnd, msg, wParam, lParam);
	}
	if( msg != WM_SYSCOMMAND ){
		if( msg != WM_LBUTTONDOWN && msg != WM_RBUTTONDOWN ){
			return DefaultHandler(hWnd, msg, wParam, lParam);
		}
		goto LABEL_9;
	}
	if( wParam == SC_CLOSE ){
		VideoWindowExists = 0;
		return 0;
	}
	return DefaultHandler(hWnd, msg, wParam, lParam);
}

//----- (th3) -------------------------------------------------------------
bool IsPlayerSummon(int monsterIndex)
{
	return monsterIndex >= 0 && monsterIndex < SummonMonsters_Count;
}

//----- (th3) -------------------------------------------------------------
void ClearMapFromInvalidMonsters()
{
	for (int scanedMapCol = 16; scanedMapCol < 96; scanedMapCol++) {
		for (int scanedMapRow = 16; scanedMapRow < 96; scanedMapRow++) {
			int m = MonsterMap[scanedMapRow][scanedMapCol];
			if (m) {
				Monster& monster = Monsters[abs(m) - 1];
				if( monster.flag & MF_15_KILLED || abs(monster.Row - scanedMapRow) > 1 || abs(monster.Col - scanedMapCol) > 1) {
					MonsterMap[scanedMapRow][scanedMapCol] = 0;
				}
			}
		}
	}
}
