#include "stdafx.h"

bool AuraSoundStates[];
int AuraCooldown[];
bool BattleTranceSlowMonster = 0;
bool BattleTrance = 0;

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// (rt) inline new function in the 1.145 chunk (0070B600)
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
		if (Difficulty >= DL_1_PURGATORY) { // 0070B605 someAction
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

// (rt) New useful function, optimization
__forceinline bool Distance_To_Target_Is_Less_Than(uint monsterIndex, int maxDistance)
{
	if( monsterIndex >= Monsters_Max_Count ) return false;
	Monster& monster = Monsters[ monsterIndex ];
	return (abs(monster.Row - monster.TargetRow) < maxDistance) && (abs(monster.Col - monster.TargetCol) < maxDistance);
}

// (rt) New useful function, optimization
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
	// (th4) Dark Campaign scene latches belong to a level, not to the process. Without this
	// they survive from one loaded game into the next within a single run of the executable:
	// finish mission 26 once, load again, and the Dark Lord's portal never fires a second
	// time because DarkCampaignEndArrived is still set from the previous session. The saved
	// state is elsewhere -- the Butcher's freed flag and the closing speech both live in
	// quest data -- so nothing persistent is lost by clearing these here.
	DarkCampaignEndArrived = false;
	DarkCampaignEndLordDelay = 0;
	DarkCampaignEndWalkSteps = 0;
	DarkCampaignEndTalkWanted = false;
	DarkCampaignButcherTalkWanted = false;
	DarkCampaignButcherJoinPending = false;
	DarkCampaignButcherNeedsArrival = false;
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
	// (rt) th1 has a bug that overwrites the local dungeonLevel
	// (rt) reproduced for now, for comparison and for loading th1 single-player saves without crashes
	//bool bug = IsExeValidating ? true : true; //false;
	// (rt) below there were several if( bug ) dungeonLevel = d;
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
	AddMonsterTypeToDungeon(BM_392_WARLORD_MERC, MPF_2_DONT);
	
	switch( Dungeon->level ){ // TODO: check for DUNGEON id needing instead of Dungeon->level
	case DUN_2_CHURCH_2:
		// look up thehell2.cpp where `IsPinkyOnLevel()` starts, there are 4 values to update if i want to move ubers to further diffs (and 5 values here in monster.cpp)
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= DL_2_DOOM) {
			AddMonsterTypeToDungeon(BM_207_DREGBEZ, MPF_2_DONT);
			AddMonsterTypeToDungeon(BM_208_WARPED_ONE, MPF_2_DONT);
		}
		break;
	case DUN_4_CHURCH_4:
		AddMonsterTypeToDungeon(BM_1_OBLIVION_KNIGHT, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_170_LICH, MPF_2_DONT);
		AddMonsterTypeToDungeon(BM_175_TERROR_CLAN, MPF_2_DONT);
		if (Difficulty >= DL_2_DOOM && MaxCountOfPlayersInGame != 1) {
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
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= DL_2_DOOM) {
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
		if (MaxCountOfPlayersInGame != 1 && Difficulty >= DL_2_DOOM) {
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
		if (Difficulty >= DL_2_DOOM && MaxCountOfPlayersInGame != 1) { // MP Doom: add Abatan and his minions
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
		if( MaxCountOfPlayersInGame != 1 && Difficulty >= DL_2_DOOM){
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
	if( SomeBoolFlag_12 && MapRowArraySize > 0 ){ // (rt) SomeBoolFlag_12 seems to be always zero
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
				// (rt) add the monster and remove it from the list
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
				// (rt) here the animation differs per direction
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
				// (rt) the same animation in every direction
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
	
	// (rt) colour the monsters
	if (baseMonster.IceAgeTranslationFilePtr && !Dungeon->isQuest && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		monsterSprite.colorTable = (uchar*)LoadFile(baseMonster.IceAgeTranslationFilePtr);
		ColorMonsterSprite(monsterSpriteIndex, baseMonster.HasSecondAttack);
		FreeMemZero(monsterSprite.colorTable);
	// (th4) Classic must look like vanilla Diablo, and there the golem wears no TRN at all --
	// the red one is a TH addition.
	} else if( baseMonster.HasColorTranslationFile
	        && !(GameMode == GM_CLASSIC && baseMonsterIndex == BM_109_GOLEM) ){
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
	// (th4) A monster that has just been created is standing on its tile, so it can have no
	// walk offset. These were never cleared here, and Monsters[] slots are reused across
	// levels, so a monster inherited whatever pixel offset the previous occupant of its slot
	// was left mid-stride with. Ordinary monsters hid it -- the first AI tick overwrites the
	// offset -- but the Dark Campaign hub kin are frozen (ActivationCounter 0, CurAction
	// A_0_STAY) and never take one, so a vendor kept the stale offset for good and drew a
	// tile away from where it stood, inside the wall behind it.
	monster.DeltaX = 0;
	monster.DeltaY = 0;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && (char*)sprite->baseMonsterPtr->IceAgeNamePtr) {
		monster.Name = (char*)LocalizedIceAgeName((char*)sprite->baseMonsterPtr->IceAgeNamePtr);
	}
	else {
		monster.Name = (char*)LocalizedMonsterName( sprite->baseMonsterPtr );
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
			monster.MinDamage += monster.MinDamage + 4;
			monster.MaxDamage += monster.MaxDamage + 4;
			monster.SecondToHit += 85;
			monster.SecondMinDamage += monster.SecondMinDamage + 4;
			monster.SecondMaxDamage += monster.SecondMaxDamage + 4;
			monster.ArmorClass += 50;
		}
		if (Difficulty == 2) {
			int life = (monster.BaseLife * 4) + (((MaxCountOfPlayersInGame != 1) ? 200 : 100) << 6); //2 * (((MaxCountOfPlayersInGame != 1 ? 100 : 50) * 128) + monster.BaseLife) + 16'000;
			monster.BaseLife = life;
			monster.CurrentLife = life;
			monster.MonsterLevel += 30;
			monster.Xp = 4 * monster.Xp + 4'000;
			monster.ToHit += 120;
			monster.MinDamage += monster.MinDamage * 3 + 6;
			monster.MaxDamage += monster.MaxDamage * 3 + 6;
			monster.SecondToHit += 120;
			monster.SecondMinDamage += monster.SecondMinDamage * 3 + 6;
			monster.SecondMaxDamage += monster.SecondMaxDamage * 3 + 6;
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
					or BM_279_FLOATER or BM_280_FLOATER_X2 or BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW or BM_284_SIN_SPAWN or BM_220_SNOW_LICH or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE
					// (th4) Dark Campaign heroes: casters and archers, same as any other ranged monster
					or BM_H_393_SORC_L_T or BM_H_396_SORC_L_T or BM_H_400_SORC_M_T or BM_H_413_SOR_H_T or BM_H_418_SOR_H_T_SORC_ELITE
					or BM_H_394_ROG_L_B or BM_H_397_ROG_L_B or BM_H_403_ROG_M_B or BM_H_406_SCOUT or BM_H_411_ROG_H_B
					or BM_H_415_ROG_H_B_ELITE_ARCHER or BM_H_419_ROG_H_B_SS_ELITE or BM_H_423_ROG_H_B_BLACK_ARCHER:
					life /= 2;
					break;
					case BM_4_GREMLIN or BM_219_FROST_BONE or BM_5_FALLEN_ONE or BM_8_BONE_CREEPER or BM_12_IMP or BM_13_DARK_ONE or BM_17_SCAVENGER or BM_19_RAVAGER
						or BM_49_BILE_SPITTER or BM_155_FALLEN_ONE or BM_172_SPITTING_TERROR or BM_177_PHASE_DEMON or BM_182_BLINK_DEMON or BM_29_HIDDEN or BM_30_SHADOW
						or BM_31_UNSEEN or BM_38_UNDEAD_WEREWOLF or BM_39_WARPING_DEMON or BM_53_HELL_HOUND or BM_80_DEVIL_KIN or BM_83_LASH_WORM or BM_92_ILLUSION_WEAVER
						or BM_94_LURKER or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER or BM_149_THRESHER or BM_150_FELLTWIN or BM_151_HYBRID
						or BM_157_YELLOW_ONE or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_232_ABYSSAL_SUCCUBUS or BM_235_STYGIAN_FURY or BM_237_CONTAMINATOR
					or BM_241_ACID_TERROR or BM_248_SALAMANDER or BM_257_SHROUDED
					or BM_H_405_SOR_M_T_HYBRID:	// (th4) DC hybrid Battle Mage -- fights in melee too, so only a quarter off
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
					or BM_279_FLOATER or BM_280_FLOATER_X2 or BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW or BM_284_SIN_SPAWN or BM_220_SNOW_LICH or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE
					// (th4) Dark Campaign heroes: casters and archers, same as any other ranged monster
					or BM_H_393_SORC_L_T or BM_H_396_SORC_L_T or BM_H_400_SORC_M_T or BM_H_413_SOR_H_T or BM_H_418_SOR_H_T_SORC_ELITE
					or BM_H_394_ROG_L_B or BM_H_397_ROG_L_B or BM_H_403_ROG_M_B or BM_H_406_SCOUT or BM_H_411_ROG_H_B
					or BM_H_415_ROG_H_B_ELITE_ARCHER or BM_H_419_ROG_H_B_SS_ELITE or BM_H_423_ROG_H_B_BLACK_ARCHER:
					life /= 2;
					break;
					case BM_49_BILE_SPITTER or BM_155_FALLEN_ONE or BM_172_SPITTING_TERROR or BM_177_PHASE_DEMON or BM_182_BLINK_DEMON or BM_4_GREMLIN or BM_219_FROST_BONE
						or BM_5_FALLEN_ONE or BM_8_BONE_CREEPER or BM_12_IMP or BM_13_DARK_ONE or BM_17_SCAVENGER or BM_19_RAVAGER or BM_29_HIDDEN or BM_30_SHADOW or BM_31_UNSEEN
						or BM_38_UNDEAD_WEREWOLF or BM_39_WARPING_DEMON or BM_53_HELL_HOUND or BM_80_DEVIL_KIN or BM_83_LASH_WORM
						or BM_92_ILLUSION_WEAVER or BM_94_LURKER or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER or BM_149_THRESHER
						or BM_150_FELLTWIN or BM_151_HYBRID or BM_157_YELLOW_ONE or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_232_ABYSSAL_SUCCUBUS or BM_235_STYGIAN_FURY
					or BM_237_CONTAMINATOR or BM_241_ACID_TERROR or BM_248_SALAMANDER or BM_257_SHROUDED
					or BM_H_405_SOR_M_T_HYBRID:	// (th4) DC hybrid Battle Mage -- fights in melee too, so only a quarter off
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
			case DL_1_PURGATORY:	addups = Dungeon->level / 6; break;// +(0 - 4)
			case DL_2_DOOM:			addups = 4 + Dungeon->level / 3; break;// +(4 - 12)
			case DL_3_CRUCIBLE:		addups = 4 + 8 + Dungeon->level / 2; break;// +(12 - 24)
			case DL_4_TORMENT:		addups = 4 + 8 + 12 + Dungeon->level / 2; break;// +(24 - 36)
			case DL_5_AGONY:		addups = 4 + 8 + 12 + 12 + Dungeon->level / 2; break;// +(36 - 48)
			case DL_6_TERROR:		addups = 4 + 8 + 12 + 12 + 12 + Dungeon->level / 2; break;// +(48 - 60)
			case DL_7_DESTRUCTION:	addups = 4 + 8 + 12 + 12 + 12 + 12 + Dungeon->level;	  break;// +(60 - 84)
			case DL_8_INFERNO:		addups = 4 + 8 + 12 + 12 + 12 + 12 + 24 + 2 * Dungeon->level; break;// +(84 - 132)
			}
			// temporarily cancelled addups, until sigils and powerups are added
			//addups = 0;
			monster.ToHit += 90 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
			int minDmg = 65 * Difficulty + addups;
			int maxDmg = 85 * Difficulty + RNG(2 + 2 * Difficulty) + addups;
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
	if (GameMode == GM_DARK_CAMPAIGN) {
		if (Dungeon == DUN_72_NECROPOLIS) {
			monster.MonsterLevel += 20;
			monster.Xp += 3'400;
			monster.BaseLife += monster.BaseLife / 2;
			monster.CurrentLife = monster.BaseLife;
			monster.ToHit += 14;
			monster.MinDamage += 19;
			monster.MaxDamage += 22;
			monster.ArmorClass += 25;
		}
		// (th4) DC mission 26: the cadavers the Risen Blacksmith brought up with him.
		if (Dungeon == DUN_44_RAVENHOLM && baseMonster == BM_215_CADAVER) {
			monster.MonsterLevel += 42;
			monster.Xp += 3'500;
			monster.BaseLife += 1'000 << 6;
			monster.CurrentLife = monster.BaseLife;
			monster.ToHit += 20;
			monster.MinDamage += 30;
			monster.MaxDamage += 30;
			monster.ArmorClass += 58;
		}
	}
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
		// (th4) The pixel offset of a walk in progress. ClearMonsterWalkCounters does NOT
		// touch these two, and nothing else reset them either -- so they survived from one
		// level to the next in whichever Monsters[] slot they were left in, and the monster
		// created into that slot on the new level inherited them. DrawMonsterInTile adds
		// them straight to the screen position, so a stale -32 in DeltaY draws the monster
		// a whole tile above the cell it actually occupies.
		monster.DeltaX = 0;
		monster.DeltaY = 0;
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
			monster->CurrentLife = monster->BaseLife; // (rt) / 2; // Uber's life used to be halved (disabled in the current th1 version)
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
	// (rt) Outbreak: don't generate procedural monsters
	if (GameMode == GM_OUTBREAK && Dungeon == DUN_1_CHURCH_1) {
		return;
	}
	
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

// (th4) How far the escort may drift from the player before closing the gap. Small enough
// that they stay useful, large enough that they are not glued to the player's back.
const int DC_ESCORT_FOLLOW_GAP = 3;

// (th4) DARK CAMPAIGN mission 10 escort. The Dark Lord lends the player his two succubi --
// Red Vex and Dark Jade -- for the return to the water supply. They are the real hub uniques,
// not summons: the summon slot machinery (Monsters[0..SummonMonsters_Count]) derives its
// owner from the slot index and carries no unique identity, so putting them there would
// strip their names and TRNs. Instead they spawn as normal uniques flagged MF_6_FRIENDLY and
// get an explicit follow-the-owner step in ProcessMonsters.
//
// State is derived, not saved: the escort is active exactly while mission 10 is briefed and
// not yet complete, which is already persisted in DarkMissionState[].
bool DarkCampaignEscortActive()
{
	if( GameMode != GM_DARK_CAMPAIGN ) return false;
	const int mission10 = 9;	// index into DarkMissions[] / DarkMissionState[]
	if( mission10 >= DARK_MISSION_MAX_50 ) return false;
	return DarkMissionState[mission10] == DCM_1_BRIEFED;
}

// (th4) DC mission 13: the Dark Lord sends the girls ahead to Lazarus the moment mission
// 12 is done, so they are missing from the hub until 13 is complete. Indices into
// DarkMissionState[] are 0-based, so mission 12 is 11 and mission 13 is 12 -- named rather
// than written inline, because getting one of them off by one silently misaims the feature.
bool DarkCampaignEscortAwayAtLazarus()
{
	const int DC_MISSION_12_INDEX = 11;
	const int DC_MISSION_13_INDEX = 12;
	if( GameMode != GM_DARK_CAMPAIGN ) return false;
	if( DC_MISSION_13_INDEX >= DARK_MISSION_MAX_50 ) return false;
	return DarkMissionState[DC_MISSION_12_INDEX] == DCM_2_COMPLETE
	    && DarkMissionState[DC_MISSION_13_INDEX] != DCM_2_COMPLETE;
}

// (th4) DC mission 15. Index of the living armorer on this level, or -1.
int DarkCampaignHephastoIndex()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_43_HEPHASTO ) return -1;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		const Monster& m = Monsters[mi];
		if( m.newBossId && m.newBossId - 1 == UM_587_Hephasto_the_Armorer ) return mi;
	}
	return -1;
}

// (th4) DC mission 15, Doom and above: the armorer takes 80 % less damage from monsters, on top of every other reduction
// (Mordor, 2026-10-12: players reported the hero force killing him there -- the mission fails and the campaign cannot go
// on). Monsters are the only thing that can hurt him -- the player and his summons never target him (his clause in
// IsPlayerSideMonster) -- and a monster hurts a monster through the two monster-vs-monster functions, which both apply
// this: MvM_Melee (this file) and MvM_Ranged (Missile.cpp).
bool DarkCampaignHephastoShielded( int monsterIndex )
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_43_HEPHASTO || Difficulty < DL_2_DOOM ) return false;
	const Monster& m = Monsters[ monsterIndex ];
	return m.newBossId && m.newBossId - 1 == UM_587_Hephasto_the_Armorer;
}

// (th4) DC mission 15: is every hero on this level dead? Same arithmetic the mission
// completion uses -- whatever is left is a player summon or Hephasto himself, and
// DarkCampaignEscortCountInLevel() discounts him.
bool DarkCampaignHellforgeClear()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_43_HEPHASTO ) return false;
	return MonstersCount - DarkCampaignEscortCountInLevel() <= SummonMonsters_Count;
}

// (th4) DC mission 13. Index of the living Arch-Bishop on this level, or -1.
int DarkCampaignLazarusIndex()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_121_UNHOLY_ALTAR_DC ) return -1;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		const Monster& m = Monsters[mi];
		if( m.newBossId && m.newBossId - 1 == UM_4_ArchBishop_Lazarus ) return mi;
	}
	return -1;
}

// (th4) DC mission 13: is every hero on this level dead? Same arithmetic the mission
// completion uses -- everything left is either a player summon or one of the three
// friendlies, both of which DarkCampaignEscortCountInLevel() discounts.
bool DarkCampaignLazarusLairClear()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_121_UNHOLY_ALTAR_DC ) return false;
	return MonstersCount - DarkCampaignEscortCountInLevel() <= SummonMonsters_Count;
}

// (th4) DC mission 25. Index of the living Butcher on this level, or -1. He is the vanilla
// UM_9_The_Butcher reused, spawned friendly and parked in his cell until the player frees him.
int DarkCampaignButcherIndex()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_111_CHURCH_SACRISTY ) return -1;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		const Monster& m = Monsters[mi];
		if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;	// dead is not found
		return mi;
	}
	return -1;
}

// (th4) DC mission 25: same search as above but on any level -- the one above is deliberately
// Sacristy-only, because the mark, the click and the talk are all Sacristy business.
int DarkCampaignButcherAnywhereIndex()
{
	if( GameMode != GM_DARK_CAMPAIGN ) return -1;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		const Monster& m = Monsters[mi];
		if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;
		return mi;
	}
	return -1;
}

// (th4) DC mission 25: has the player spoken to the Butcher and let him out?
//
// Stored in QUEST data rather than in a global, because quests are written to the savegame
// and globals are not: without this he came back caged after every save/load, mark and all.
// The Dark Campaign runs none of the ordinary quests, so the Quest array is free there.
//
// status5 of his own quest, NOT status2: TownMap.cpp already uses Q_6's status2 for the
// wounded townsman's dialogue. Unreachable in DC -- there is no town -- but sharing a field
// with live vanilla code is asking for the two to meet later. status5 is touched nowhere for
// this quest. Once set it stays set for the rest of the campaign, hub and mission 26 included.
enum { DC_BUTCHER_FREED = 1 };

bool DarkCampaignButcherIsFreed()
{
	return GameMode == GM_DARK_CAMPAIGN
	    && Quests[Q_6_THE_BUTCHER].status5 == DC_BUTCHER_FREED;
}

void DarkCampaignSetButcherFreed()
{
	if( GameMode != GM_DARK_CAMPAIGN ) return;
	Quests[Q_6_THE_BUTCHER].status5 = DC_BUTCHER_FREED;
}

// (th4) DC mission 25: he follows the player only once he has been spoken to. Before that he
// stands in his cell, which is what the parked branch in ProcessMonsters relies on.
bool DarkCampaignButcherEscorting()
{
	return DarkCampaignButcherIsFreed();
}

// (th4) DC mission 25: put the Butcher on this level if he belongs here and is not already
// standing on it. Needed because a level the player has already visited is restored by
// LoadLevel(), which replaces the whole monster array with the saved set -- so a Butcher
// created during generation is thrown away on every hub visit after the first. Called at the
// end of the level load, once that restore has happened.
// (th4) DC mission 25: put the Butcher's colours back into his LightTable row.
//
// uniqTrans is only an INDEX; the row contents are runtime state that CreateBoss fills.
// Generation refills the rows of the monsters IT creates, so every boss that was placed by
// AddQuestMonsters comes back correctly -- but the Butcher is added after generation, so on
// a reload his row is whatever was left in it. Empty row means he draws solid black.
void DarkCampaignRefreshButcherTrn()
{
	int bi = DarkCampaignButcherAnywhereIndex();
	if( bi < 0 ) return;
	const Monster& b = Monsters[bi];
	const UniqueMonster& um = UniqueMonsters[UM_9_The_Butcher];
	// Same path selection CreateBoss uses, so he matches every other boss on the level.
	const char* trn = MaxCountOfPlayersInGame == 1
		? (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) ? "CF\\ICEAGE\\%s.TRN" : by(Difficulty, "CF\\SH\\%s.TRN", "CF\\SP\\%s.TRN", "CF\\SD\\%s.TRN"))
		: by( Difficulty, "Monsters\\Monsters\\%s.TRN", "CF\\MP\\%s.TRN", "CF\\MD\\%s.TRN" );
	char trnFileName[64];
	sprintf(trnFileName, trn, um.ColorTranslationFile);
	if( b.uniqTrans >= LightTable_BossRowCount ) return;	// uchar field, never negative
	LoadFileWithMem( trnFileName, &LightTable[256 * (b.uniqTrans + LightTable_BossRowFirst)] );
}

// (th4) A monster created by CreateBoss AFTER level generation needs its slot put back into
// the live half of MonsterIndexes.
//
// CreateBoss appends at Monsters[MonstersCount] and increments MonstersCount, which is right
// only while MonsterIndexes is still the identity mapping it starts life as. RemoveKilledMonsters
// breaks that: it swaps each corpse's entry to the back and drops MonstersCount, so after any
// death the live monsters are MonsterIndexes[0..MonstersCount) in arbitrary order. The freshly
// appended slot is then sitting in the dead half -- the monster exists in the array but nothing
// iterates it, so it is never processed, never drawn and never found by an index lookup.
//
// Call this straight after CreateBoss for anything spawned mid-level.
void DarkCampaignFixupAppendedMonster()
{
	if( MonstersCount <= 0 || MonstersCount > Monsters_Max_Count ) return;
	const int newIndex = MonstersCount - 1;	// the slot CreateBoss just wrote
	if( MonsterIndexes[newIndex] == newIndex ) return;	// already live, nothing to do
	for( int i = 0; i < Monsters_Max_Count; ++i ){
		if( MonsterIndexes[i] != newIndex ) continue;
		MonsterIndexes[i] = MonsterIndexes[newIndex];
		MonsterIndexes[newIndex] = newIndex;
		return;
	}
}

// (th4) DC mission 26: is the Dark Lord's closing speech on screen right now?
//
// In QUEST data, not a global, so that saving mid-speech still ends the run when the panel is
// dismissed after the reload. status4 of Q_36_CLEAR_WATER: the Dark Campaign runs none of the
// ordinary quests, and that quest touches only status, status2, status3 and talkToFlag, so
// the field is dead space here and in normal play alike.
enum { DC_END_SPEAKING = 1 };

bool DarkCampaignEndIsPending()
{
	return GameMode == GM_DARK_CAMPAIGN
	    && Quests[Q_36_CLEAR_WATER].status4 == DC_END_SPEAKING;
}

void DarkCampaignEndSetPending( bool on )
{
	if( GameMode != GM_DARK_CAMPAIGN ) return;
	Quests[Q_36_CLEAR_WATER].status4 = on ? DC_END_SPEAKING : 0;
}

// (th4) DC mission 26: put the Dark Lord's colours back into his LightTable row. Exactly the
// problem DarkCampaignRefreshButcherTrn solves: uniqTrans is only an index, the row contents
// are runtime state that CreateBoss fills, and he is created after level generation -- so on
// a reload nothing refills his row and he draws solid black.
void DarkCampaignRefreshEndLordTrn()
{
	int li = DarkCampaignEndLordIndex();
	if( li < 0 ) return;
	const Monster& b = Monsters[li];
	const UniqueMonster& um = UniqueMonsters[UM_773_The_Dark_Lord];
	const char* trn = MaxCountOfPlayersInGame == 1
		? (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) ? "CF\\ICEAGE\\%s.TRN" : by(Difficulty, "CF\\SH\\%s.TRN", "CF\\SP\\%s.TRN", "CF\\SD\\%s.TRN"))
		: by( Difficulty, "Monsters\\Monsters\\%s.TRN", "CF\\MP\\%s.TRN", "CF\\MD\\%s.TRN" );
	char trnFileName[64];
	sprintf(trnFileName, trn, um.ColorTranslationFile);
	if( b.uniqTrans >= LightTable_BossRowCount ) return;
	LoadFileWithMem( trnFileName, &LightTable[256 * (b.uniqTrans + LightTable_BossRowFirst)] );
}

// (th4) DC mission 26: index of the Dark Lord standing in the ruins of Ravenholm, or -1.
int DarkCampaignEndLordIndex()
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_44_RAVENHOLM ) return -1;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		const Monster& m = Monsters[mi];
		if( !m.newBossId || m.newBossId - 1 != UM_773_The_Dark_Lord ) continue;
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;
		return mi;
	}
	return -1;
}

// (th4) DC mission 26: Ravenholm is clear, so the Dark Lord comes through the portal himself
// to close the campaign. He is spawned exactly like the hub kin -- friendly, chase cleared,
// no service -- and given no AI at all in ProcessMonsters, so he simply stands there with the
// mark over his head until the player walks up and speaks to him.
static void __fastcall DisplaceMonsterFromCell(int occupantIndex, int row, int col);	// defined below

void DarkCampaignEndSpawnLord( int row, int col )
{
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_44_RAVENHOLM ) return;
	if( DarkCampaignEndLordIndex() >= 0 ) return;	// already here
	if( AddMonsterTypeToDungeon( UniqueMonsters[UM_773_The_Dark_Lord].BaseMonsterIndex, MPF_4_UNIQ ) < 0 ) return;
	CreateBoss( UM_773_The_Dark_Lord, 0, 0 );
	DarkCampaignFixupAppendedMonster();	// spawned mid-level, after monsters have died
	for( int i = 0; i < MonstersCount; ++i ){
		const int li = MonsterIndexes[i];
		Monster& m = Monsters[ li ];
		if( !m.newBossId || m.newBossId - 1 != UM_773_The_Dark_Lord ) continue;

		// Put him on the portal tile by hand. BossRow/BossCol cannot do it: CreateBoss reads
		// them on the first pass of its placement loop, clears them, then validates the tile
		// with IsCellFreeForMonster -- which rejects anything flagged CF_2_VISIBLE_BY_PLAYER.
		// The portal is in plain sight by design, so the check fails, the loop spins again
		// with the override already spent, and he lands on a random RNG(80)+16 tile instead.
		ClearMonsterOnMap( li );
		if( In112(row, col) && MonsterMap[row][col] > 0 && MonsterMap[row][col] - 1 != li ){
			DisplaceMonsterFromCell( MonsterMap[row][col] - 1, row, col );
		}
		MonsterMap[row][col] = li + 1;
		m.Row = m.NextRow = m.PrevRow = row;
		m.Col = m.NextCol = m.PrevCol = col;
		FixMonsterPosition( li, DIR_0_D );
		m.flag |= MF_6_FRIENDLY;
		m.flag &= ~(MF_9_CHASE_TARGET | MF_1_INVISIBLE);
		m.speechIndex = 0;
		m.ActivationCounter = 255;
		// He walks out of the portal rather than being teleported clear of it, so the player
		// sees him arrive. The steps are counted down in his branch of ProcessMonsters, one
		// per landing; StartMonsterWalk is used directly because the anti-cornering in
		// MonsterWalksTowardsTarget would second-guess a scripted step. When the count runs
		// out he has no AI at all and simply holds the tile facing DIR_0_D.
		m.Orientation = DIR_0_D;
		m.curAnimation = &m.SpritePtr->animation[ANIM_0_STAY].oriented[DIR_0_D];
		DarkCampaignEndWalkSteps = DC_END_LORD_STEPS;
	}
	DarkCampaignEndArrived = true;
}

// (th4) DC: the levels the freed Butcher comes along to. The Sacristy is not in the list --
// he is placed there by the mission's own spawn block, in his cell, before he is freed.
bool DarkCampaignButcherFollowsHere()
{
	return DarkCampaignButcherJoined()
	    && is( (DUNGEON)Dungeon, DUN_117_DARKLORD_HIDEOUT, DUN_44_RAVENHOLM );
}

void DarkCampaignSpawnButcherIfMissing()
{
	if( !DarkCampaignButcherFollowsHere() ) return;
	if( DarkCampaignButcherAnywhereIndex() >= 0 ) return;	// already here
	if( AddMonsterTypeToDungeon( UniqueMonsters[UM_9_The_Butcher].BaseMonsterIndex, MPF_4_UNIQ ) < 0 ) return;
	CreateBoss( UM_9_The_Butcher, 0, 0 );
	DarkCampaignFixupAppendedMonster();	// spawned after LoadLevel, same reason
	DarkCampaignButcherNeedsArrival = true;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		if( mi < 0 || mi >= Monsters_Max_Count ) continue;
		Monster& m = Monsters[ mi ];
		if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
		// (th4) Skip a Butcher who is already dead or retired. The guard above only
		// returns early on a LIVE one, so a corpse -- the player killed him, or
		// DarkCampaignCollapseDuplicateButchers retired him -- still sits in the array
		// and this loop would otherwise re-apply the boosts to it. Worse, CurrentLife =
		// BaseLife below would give it full life while it is still flagged MF_15_KILLED,
		// and anything testing life without testing the flag would read it as alive.
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;
		m.flag |= MF_6_FRIENDLY;
		m.flag &= ~MF_9_CHASE_TARGET;
		m.speechIndex = 0;
		m.ActivationCounter = 255;
		// (th4) The escort boosts. This path used to set the flags and nothing else, so a
		// Butcher who arrived through it was a raw UM_9 -- around a hundred life instead
		// of twenty thousand, which is what a player saw when a duplicate appeared.
		// Copied from the DUN_44_RAVENHOLM branch of AddQuestMonsters; keep the two in
		// step. NOTE the DUN_117_DARKLORD_HIDEOUT branch there sets the flags WITHOUT
		// these boosts, so this helper does boost him in the hub where generation would
		// not -- deliberate and harmless, since nothing fights in the hub.
		m.BaseLife += 5'800 << 6;
		m.CurrentLife = m.BaseLife;
		m.MinDamage += 92;
		m.MaxDamage += 186;
		m.ArmorClass += 103;
		m.MonsterLevel += 50;
		m.ToHit += 120;
	}
}

// (th4) DC mission 25: collapse duplicate Butchers down to one.
//
// Reported from the last mission: after reloading a save the player had TWO, one with the
// full escort life and one with about a hundred. He can be created by two independent
// paths -- AddQuestMonsters when DUN_44/DUN_117 are generated, and
// DarkCampaignSpawnButcherIfMissing after a level or savegame restore -- and the second
// one only stands down if its "is he already here" scan sees the first. That scan walks
// MonsterIndexes[0..MonstersCount-1], and both of those are rewritten from the savegame
// partway through LoadGame, so it is not reliable at the moment it runs.
//
// Rather than chase the ordering, this collapses whatever ended up in the level. Cheap,
// runs once per load, and correct no matter which path produced the extra.
//
// The keeper is the one with the MOST life, which is always a fully boosted Butcher --
// never the raw copy.
//
// EXTRAS ARE REMOVED FROM THE LIVE SET, not merely flagged. An earlier draft only set
// MF_15_KILLED and left the slot in MonsterIndexes. That does not work in this engine:
// ProcessMonsters does not skip MF_15_KILLED -- its ghost guard REQUIRES the flag to be
// clear, so a flagged monster falls straight through to the AI dispatch. There the
// FRIENDLY+Butcher branch still matched him, ran Ai_DarkCampaignButcher, and its
// arrival step (TeleportSummonToOwner) put him straight back onto MonsterMap -- and
// rendering is map-driven, so the duplicate simply reappeared.
//
// So this does what DelMonster does: swap the tail of MonsterIndexes into the freed
// position and shrink MonstersCount. That moves POSITIONS in MonsterIndexes only; every
// monster keeps its own index into Monsters[], which is what the rest of the level
// holds, so nothing is invalidated. DelMonster itself is not called because it also
// plays the quest-done sound and runs the Ironman clear check -- both wrong on a load.
// The loop walks BACKWARDS so the entry swapped into a freed position has already
// been examined. The freed slot keeps MF_15_KILLED and zero life so any stale
// TargetIndex pointing at it reads as an invalid target.
void DarkCampaignCollapseDuplicateButchers()
{
	if( GameMode != GM_DARK_CAMPAIGN ) return;
	int keep = -1;
	int found = 0;
	for( int i = 0; i < MonstersCount; ++i ){
		int mi = MonsterIndexes[i];
		// (th4) Bounds first, as RepairMonsterMap does. This runs while MonstersCount and
		// MonsterIndexes are half-rewritten from a savegame -- the very condition that
		// makes the duplicate possible -- so a stale entry here is exactly the case to
		// survive, not to index Monsters[] with.
		if( mi < 0 || mi >= Monsters_Max_Count ) continue;
		const Monster& m = Monsters[mi];
		if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;
		++found;
		if( keep < 0 || m.BaseLife > Monsters[keep].BaseLife ) keep = mi;
	}
	if( found < 2 ) return;				// nothing to do, the normal case
	for( int i = MonstersCount - 1; i >= 0; --i ){	// backwards: see the header comment
		int mi = MonsterIndexes[i];
		if( mi < 0 || mi >= Monsters_Max_Count ) continue;
		Monster& m = Monsters[mi];
		if( mi == keep ) continue;
		if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) continue;
		// Take him off the map the way the engine does everywhere else. NOT a bare
		// "MonsterMap[Row][Col] = 0": a duplicate is placed by CreateBoss, which stomps
		// the cell when DisplaceMonsterFromCell cannot find room, so the entry there may
		// belong to ANOTHER monster -- clearing it blind orphans that one (still drawn,
		// still alive, but off MonsterMap, so nothing can click or hit it, which is a
		// stuck kill counter and no exit portal -- see RepairMonsterMap).
		// ClearMonsterOnMap sweeps the 3x3 around Prev* and matches BOTH signs, since a
		// cell may hold -index-1 as well as +index+1; the second clear catches the case
		// where Row/Col have already diverged from Prev*. In112 bounds both -- Row/Col
		// are only ever range-checked through it, and an unchecked write here would land
		// outside a [112][112] array.
		ClearMonsterOnMap( mi );
		if( In112(m.Row, m.Col) && MonsterMap[m.Row][m.Col] == mi + 1 ){
			MonsterMap[m.Row][m.Col] = 0;
		}
		m.flag |= MF_15_KILLED;
		m.CurrentLife = 0;
		// Out of the live set -- DelMonster's swap, without its sound and Ironman side
		// effects. The slot itself stays in the pool for AddMonster to reuse, as always.
		--MonstersCount;
		MonsterIndexes[i] = MonsterIndexes[MonstersCount];
		MonsterIndexes[MonstersCount] = mi;
	}
}

// (th4) DC mission 25: is the Butcher with the player from here on? Read from the SAVED
// saved quest flag, so it survives a reload and holds for the rest of the campaign.
bool DarkCampaignButcherJoined()
{
	return DarkCampaignButcherIsFreed();
}

// (th4) True for the two succubi while they are acting as the player's escort.
// NB: use || here, not `or` -- in this codebase `or` is a case-fallthrough macro
// (common.h: #define or : [[fallthrough]]; case), not the boolean operator.
bool DarkCampaignIsEscortBoss( int bossId )
{
	return bossId == UM_5_Red_Vex || bossId == UM_6_Dark_Jade;
}

// (th4) The escort's post in the Deep Sanctum -- where they stand when not on a mission.
// Kept in one place because both the spawn switch and the walk-home step need them.
bool DarkCampaignEscortHubPost( int bossId, int& row, int& col )
{
	if( bossId == UM_5_Red_Vex ){ row = 20; col = 17; return true; }
	if( bossId == UM_6_Dark_Jade ){ row = 17; col = 20; return true; }
	return false;
}

// (th4) Set while the succubi are walking back to their posts after mission 10. They come
// through the portal with the player rather than being standing in place, so for the length
// of that walk they are exempt from the hub's kin freeze (see ProcessMonsters). Deliberately
// not saved: on a reload the hub simply places them at their posts, already home.
bool DarkCampaignEscortWalkingHome = false;

// (th4) Set together with the latch above and cleared on the first hub tick, once the
// escort has been moved next to the player. It cannot be done at spawn time: the hub
// generator never assigns PlayerRowPos/PlayerColPos (FixTPandStairEntryPoint settles the
// arrival tile only AFTER generation), so reading them in CreateBoss gave a stale position
// -- which is why the two of them turned up at their posts instead of walking in with him.
bool DarkCampaignEscortNeedsArrival = false;

// (th4) True for anything on the player's side that a left-click must never attack. Own
// summons qualify by living below SummonMonsters_Count; the Dark Campaign escort are friendly
// uniques sitting above it, so they have to be named explicitly.
bool IsPlayerSideMonster( int monsterIndex )
{
	if( monsterIndex < 0 || monsterIndex >= Monsters_Max_Count ) return false;
	if( monsterIndex < SummonMonsters_Count ) return true;
	const Monster& m = Monsters[monsterIndex];
	if( GameMode != GM_DARK_CAMPAIGN || !(m.flag & MF_6_FRIENDLY) || !m.newBossId ) return false;
	if( DarkCampaignIsEscortBoss( m.newBossId - 1 ) ) return true;
	// (th4) DC mission 13: Lazarus fights on the player's side in his own lair. Kept as a
	// separate clause rather than widening DarkCampaignIsEscortBoss, which drives the hub
	// posts and the walk-home route and has nothing to do with him.
	if( Dungeon == DUN_121_UNHOLY_ALTAR_DC
	 && m.newBossId - 1 == UM_4_ArchBishop_Lazarus ) return true;
	// (th4) DC mission 15: the armorer is not a target either. Same clause, same reasons --
	// the cursor must offer to walk to him rather than swing, and nothing the player or his
	// summons throw may touch him.
	if( Dungeon == DUN_43_HEPHASTO
	 && m.newBossId - 1 == UM_587_Hephasto_the_Armorer ) return true;
	// (th4) DC mission 25: the Butcher, same again -- caged, then following. No dungeon test:
	// he stays with the player from the Sacristy onwards, hub included.
	if( m.newBossId - 1 == UM_9_The_Butcher ) return true;
	// (th4) DC mission 26: and the Dark Lord, standing in the ruins at the end of it all.
	return Dungeon == DUN_44_RAVENHOLM
	    && m.newBossId - 1 == UM_773_The_Dark_Lord;
}

// (th4) How many of the escort are currently standing in this level. They are real monsters,
// so they inflate every "how much is left to kill" count -- including the one that decides a
// mission is finished, which is why a cleared level could sit at "2 heroes left" and never
// open the way home. Anything counting quarry subtracts this.
int DarkCampaignEscortCountInLevel()
{
	if( GameMode != GM_DARK_CAMPAIGN ) return 0;
	int count = 0;
	for( int i = 0; i < MonstersCount; ++i ){
		const Monster& m = Monsters[ MonsterIndexes[i] ];
		if( m.newBossId && DarkCampaignIsEscortBoss( m.newBossId - 1 ) ) ++count;
		// (th4) DC mission 25: the Butcher is on the player's side from the moment he is
		// spawned, so he must never be counted as quarry -- otherwise the cleared cellar
		// sits at "1 hero left" and the way home never opens. Holds on every level he
		// follows the player to, not just the Sacristy.
		else if( m.newBossId && m.newBossId - 1 == UM_9_The_Butcher ) ++count;
		// (th4) DC mission 13: Lazarus is on the player's side too, so he must be discounted
		// exactly like the girls. Otherwise a cleared lair sits at "1 hero left" for ever and
		// the way home never appears. Deliberately NOT written as IsPlayerSideMonster(), which
		// also returns true for summons -- the caller accounts for those separately.
		else if( m.newBossId && Dungeon == DUN_121_UNHOLY_ALTAR_DC
		      && m.newBossId - 1 == UM_4_ArchBishop_Lazarus ) ++count;
		// (th4) DC mission 15: and the armorer, for exactly the same reason -- he is the
		// thing being defended, not a hero left standing.
		else if( m.newBossId && Dungeon == DUN_43_HEPHASTO
		      && m.newBossId - 1 == UM_587_Hephasto_the_Armorer ) ++count;
	}
	return count;
}

int BossRow = -1, BossCol = -1;
//----- (0043BA78) --------------------------------------------------------
// (th4) Bosses that carry NO light at all (CreateBoss gives them LightIndex -1). One list, used by CreateBoss
// and by FixBossLightInSlot0 below -- add a boss here, not in a copy.
static bool BossHasNoLight( int bossId )
{
	return is( bossId, /*UM_10_Aym_the_Lord_of_Torment, */UM_C_902_RAGEHORN, UM_677_Guardian_of_Valor, UM_C_871_HAZEWEAVER, UM_676_Hazeweaver );
}

// (th4) BOSS LIGHT IN SLOT 0. LightList slot 0 is a real light, and AddLight hands it out first -- but for a
// monster, LightIndex 0 also means "no light" (every ordinary monster starts with 0), so all the monster walk
// code moves a light only `if( monster.LightIndex )` (MonsterWalkUp/Down/Horizontal, OffsetMonsterLight, the
// walk starts). A boss whose light got slot 0 therefore walked away from it, and only a charge (which copies
// the index into its missile, Missile.cpp CastMonsterRun) brought it back -- reported for Borosteron in The
// Maze. Quest maps make it likely: LoadGameLevel creates their monsters BEFORE the hero (LoadSetMap ->
// InitMonsters, then InitPlayer), so the first boss light there is slot 0 (proven by TH4_bosslight.log,
// 2026-10-06). Saves store that 0, so fixing only the creation would not help existing games.
// FIX: every monster tick, a living boss on slot 0 gets its light moved to another slot -- an exact copy
// (position, radius, offset, tint, on/off; bosses whose light is parked at 0,0 stay hidden) -- and slot 0 is
// released with LightOff. Never touches slot 0 while it is a hero's light (normal levels give the hero slot 0),
// nor a boss that has no light (BossHasNoLight: old saves still have 0 there instead of -1). Not done instead:
// "-1 means no light for every monster, check >= 0" -- old saves keep 0 for lightless monsters, and every one
// of them would then drive slot 0, usually the hero's torch.
static void FixBossLightInSlot0()
{
	if( FullLighting ) return;
	for( int i = 0; i < MonstersCount; ++i ){
		Monster& m = Monsters[ MonsterIndexes[ i ] ];
		if( m.LightIndex != 0 || !m.newBossId || m.CurrentLife <= 0 || BossHasNoLight( m.newBossId - 1 ) ) continue;
		bool heroSlot = false;
		for( int p = 0; p < PlayersMax_4; ++p ){
			if( Players[ p ].IsExists && Players[ p ].dungeon == (DUNGEON)Dungeon && Players[ p ].LightIndex == 0 ) heroSlot = true;
		}
		if( heroSlot ) continue;
		const Light old = LightList[ 0 ];
		const LightTint tint = LightTintList[ 0 ];
		const int slot = AddLight( old.row, old.col, old.radius );
		if( slot <= 0 ) continue;	// no free slot (or lighting off): leave it as it was
		LightTintList[ slot ] = tint;
		ChangeLightOffset( slot, old.deltaRow, old.deltaCol );
		if( old.off ) LightOff( slot );
		LightOff( 0 );
		m.LightIndex = slot;
	}
}

// (th4) HIDDEN BOSS LIGHT. A boss that is invisible (MF_1_INVISIBLE -- the fading "hidden" bosses, AI_15_Hidden:
// Hazeshifter, Bentdusk the Grey, Gloomstrike...) must not light the map: the game parks his light at (0,0) when
// he fades out (Monster_Fade_Out path) and brings it back in Monster_Fade_In / the instant-visibility AI. But
// these bosses START invisible -- the flag comes with their base monster row -- while CreateBoss lights their
// spawn tile, so until the first fade-out a light shone on an empty spot (found with TH4_bosslight.log,
// 2026-10-06; a Thief notices it at once). Same after a level revisit. FIX: every monster tick, an invisible
// boss's light that is not parked gets parked at (0,0); the existing fade-in restores it. Slot 0 is skipped:
// after FixBossLightInSlot0 a boss can only still be on 0 when 0 is a hero's light, which must not be moved.
static void ParkHiddenBossLights()
{
	if( FullLighting ) return;
	for( int i = 0; i < MonstersCount; ++i ){
		const Monster& m = Monsters[ MonsterIndexes[ i ] ];
		if( !m.newBossId || !( m.flag & MF_1_INVISIBLE ) || m.CurrentLife <= 0 ) continue;
		const int slot = m.LightIndex;
		if( slot <= 0 || slot >= Lights_128_Count ) continue;
		if( LightList[ slot ].row || LightList[ slot ].col ) ChangeLightPos( slot, 0, 0 );
	}
}

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
	// (th4) One LightTable row per boss, from LightTable_BossRowFirst up -- Monster::uniqTrans
	// stores the offset. The range used to be 8 rows, so the 9th boss on a level was rejected
	// outright (that is why Torvatus/Gloomshredder/Salinus never appeared in the DC hub) and
	// the hub worked around it by making all 13 uniques share row 19, which is why every
	// vendor rendered wearing whichever TRN happened to load last. The range is now 21 rows,
	// so the hub fits and the workaround is gone; the cap applies uniformly to every level.
	if( BossLightIndex >= LightTable_BossRowCount ){
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
		row = 2 * CurPattern.row + 26;
		col = 2 * CurPattern.col + 20;
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
		case UM_587_Hephasto_the_Armorer:	row = 22;	col = (GameMode == GM_DARK_CAMPAIGN) ? 88 : 83;	break;	// (th4) DC m15 forge tile
		case UM_583_Lich_King:				if (Dungeon == DUN_72_NECROPOLIS && GameMode == GM_DARK_CAMPAIGN) { row = 85;	col = 22; }	// (th4) DC mission 19
									else { row = 60;	col = 53; }
									break;
		case UM_674_Guardian_of_Bones:		if (GameMode != GM_CLASSIC)					   { row = 24;	col = 38; }	break;
		case UM_675_PWS_Boss:				if (Difficulty >= DL_1_PURGATORY && GameMode != GM_CLASSIC && GameMode != GM_DARK_CAMPAIGN) { 
												row = 19;	col = 19; 
											} break;
		case UM_906_DC_LORD_ANTONIUS:		if (Dungeon == DUN_28_POISONED_WATER_SUPPLY && GameMode == GM_DARK_CAMPAIGN) { row = 20; col = 30; } break;
		case UM_584_Mordessa:				row = 30;	col = 30;	break;
		case UM_5_Red_Vex or UM_C_868_RED_VEX:					// (th4) hub entourage; elsewhere unchanged
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){
				// (th4) Returning from mission 10 they arrive with the player and walk to
				// their post; otherwise they are simply standing at it.
				row = 20; col = 17;	// (th4) arrival beside the player is done on the first hub tick
			}
			else if( Dungeon == DUN_118_POISONED_WATER_SUPPLY_DC ){ row = 33; col = 82; }	// (th4) DC m10 escort, beside the entry
			else if( Dungeon == DUN_121_UNHOLY_ALTAR_DC ){ row = 39; col = 44; }	// (th4) DC m13
			else									 { row = 40; col = 45; }
			break;
		case UM_6_Dark_Jade or UM_C_869_BLACKJADE:				// (th4) hub entourage; elsewhere unchanged
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){
				row = 17; col = 20;	// (th4) arrival beside the player is done on the first hub tick
			}
			else if( Dungeon == DUN_118_POISONED_WATER_SUPPLY_DC ){ row = 34; col = 83; }	// (th4) DC m10 escort, beside the entry
			else if( Dungeon == DUN_121_UNHOLY_ALTAR_DC ){ row = 39; col = 49; }	// (th4) DC m13
			else									 { row = 38; col = 49; }
			break;
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
		case UM_915_DC_SIR_CABBOT:			row = 49;	col = 46;	break;	// (th4) DC mission 6
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
		case UM_652_Griefzap:				if (Dungeon == DUN_71_GRIM_VAULTS) { row = 23;	col = 67; }	break;
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
		case UM_752_Deathcaller:			if (Dungeon == DUN_72_NECROPOLIS && GameMode == GM_DARK_CAMPAIGN) { row = 86;	col = 87; }	// (th4) DC mission 19
									else { row = 18 + 28;	col = 57 - 30; }
									break;
		case UM_629_Nightburn_the_Mad:		if (Dungeon == DUN_79_COLONNADE) { row = 24 + 1;	col = 57; }break;
		case UM_753_Sister_Priscilla: { row = 24 + 2;	col = 57; }break;
		case UM_754_Madame_Pompadour: { row = 24 + 3;	col = 57; }break;
		case UM_384_Lionsteel_the_Bent:		if (Dungeon == DUN_79_COLONNADE) { row = 24 + 4;	col = 57; }break;
		case UM_169_Urzul_the_Trampler:		if (Dungeon == DUN_80_HELL_GATE) { row = 23;	col = 43; } break;
		case UM_940_DC_Darius_the_Ronin:	if (Dungeon == DUN_80_HELL_GATE) { row = 88;	col = 24; } break;	// (th4) DC mission 18
		case UM_941_DC_Lord_Tenabris:		if (Dungeon == DUN_69_GIBBET) { row = 80;	col = 34; } break;	// (th4) DC mission 20
		case UM_942_DC_Venturius_the_Inquisitor:	if (Dungeon == DUN_67_SEPULCHER) { row = 40;	col = 40; } break;	// (th4) DC mission 21
		case UM_943_DC_Paladin_Orphius:		if (Dungeon == DUN_65_RUINED_TEMPLE) { row = 81;	col = 51; } break;	// (th4) DC mission 22
		case UM_944_DC_Knight_Grimald:		if (Dungeon == DUN_65_RUINED_TEMPLE) { row = 53;	col = 48; } break;	// (th4) DC mission 22
		case UM_945_DC_Prophetess_Lyssia:	if (Dungeon == DUN_65_RUINED_TEMPLE) { row = 64;	col = 62; } break;	// (th4) DC mission 22
		case UM_946_DC_Cleric_Dwaynor:		if (Dungeon == DUN_62_DIM_UNDERCROFT) { row = 75;	col = 37; } break;	// (th4) DC mission 23
		case UM_947_DC_High_Exemplar_Kastor:	if (Dungeon == DUN_60_HAUNTED_LABYRINTH) { row = 85;	col = 49; } break;	// (th4) DC mission 24
		case UM_9_The_Butcher:			if (GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_111_CHURCH_SACRISTY) { row = 67;	col = 53; } break;	// (th4) DC mission 25: his cell
		case UM_948_DC_Warden_Gaspax:		if (Dungeon == DUN_111_CHURCH_SACRISTY) { row = 26;	col = 44; } break;	// (th4) DC mission 25
		case UM_949_DC_Paladin_Godfrey:		if (Dungeon == DUN_44_RAVENHOLM) { row = 77;	col = 19; } break;	// (th4) DC mission 26
		case UM_950_DC_Captain_Dorian:		if (Dungeon == DUN_44_RAVENHOLM) { row = 89;	col = 57; } break;	// (th4) DC mission 26
		case UM_951_DC_Grand_Justicar_Rosalind:	if (Dungeon == DUN_44_RAVENHOLM) { row = 51;	col = 60; } break;	// (th4) DC mission 26
		case UM_952_DC_Elara_Longshot:		if (Dungeon == DUN_44_RAVENHOLM) { row = 21;	col = 70; } break;	// (th4) DC mission 26
		case UM_953_DC_Grandmaster_Ortus:	if (Dungeon == DUN_44_RAVENHOLM) { row = 65;	col = 84; } break;	// (th4) DC mission 26
		case UM_905_DC_Griswold:			if (Dungeon == DUN_44_RAVENHOLM) { row = 88;	col = 88; } break;	// (th4) DC mission 26
		case UM_544_Darkhoof:				if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 23; } break;
		case UM_633_Fleshvenom_the_Toxic:	if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 24; } break;
		case UM_19_Jaldabaoth:				if (Dungeon == DUN_81_UNDERWORLD) { row = 27;	col = 25; }
											else if (Dungeon == DUN_117_DARKLORD_HIDEOUT) { row = 48; col = 17; } // (th4) hub powerups vendor
											break;
		case UM_755_Redbark_Elderroot: { row = 88;	col = 74; } break;
		case UM_49_Oozeshredder:			if (Dungeon == DUN_82_HALLS_OF_SUFFERING) { row = 87;	col = 74; } break;
		case UM_55_Gutooze_the_Foul:		if (Dungeon == DUN_82_HALLS_OF_SUFFERING) { row = 86;	col = 74; } break;
		case UM_932_DC_Malachar_the_Warden:	if (Dungeon == DUN_83_BLOOD_KEEP) { row = 54;	col = 36; } break;	// (th4) DC mission 14
		case UM_933_DC_Zerath_the_Sage:		if (Dungeon == DUN_83_BLOOD_KEEP) { row = 19;	col = 45; } break;	// (th4) DC mission 14
		case UM_934_DC_Faldir_the_Keeper:	if (Dungeon == DUN_83_BLOOD_KEEP) { row = 30;	col = 87; } break;	// (th4) DC mission 14
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
		case UM_89_Alystar:					if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 55 - 30 + 15;	col = 24 + 31; }
										else if (Dungeon == DUN_117_DARKLORD_HIDEOUT) { row = 45; col = 44; } // (th4) hub identify
										break;
		case UM_488_Firespawn:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 55 - 30 + 15;	col = 24 + 19 - 14; } break;
		case UM_124_Torvatus:				if (Dungeon == DUN_100_HORADRIM_TREASURY) { row = 54;	col = 30; }
										else if (Dungeon == DUN_117_DARKLORD_HIDEOUT) { row = 27; col = 31; } // (th4) hub witch
										break;
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
				else if (Dungeon == DUN_121_UNHOLY_ALTAR_DC) {	// (th4) DC m13
					row = 32;	col = 46;
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
		case UM_94_Setebos:					if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 83;	col = 83; }
										else if (Dungeon == DUN_117_DARKLORD_HIDEOUT) { row = 17; col = 34; } // (th4) hub enchanter
										break;
		case UM_623_Griefburn_the_Destroyer:if (Dungeon == DUN_106_BASTION_OF_NIGHT) { row = 27;	col = 83; } break;
		case UM_201_Blackstorm:				if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 66;	col = 26; } break;
		case UM_99_Belial_the_Disemboweler:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 47;	col = 38; } break;
		case UM_109_Stoneburn_the_Crusher:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 50;	col = 58; } break;
		case UM_197_Blacktongue_Deathbite:	if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 68;	col = 58; } break;
		case UM_772_Vexaroth:				if (Dungeon == DUN_107_DREAD_SANCTUARY) { row = 88;	col = 78; } break;
		case UM_773_The_Dark_Lord:			// (th4) Dark Campaign hub places him at his throne; Arcane Prison keeps the old spot
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 17; col = 17; }
			// (th4) DC mission 26: Ravenholm places him itself, after CreateBoss returns --
			// see DarkCampaignEndSpawnLord. Whatever this switch picks is overwritten there.
			else if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_44_RAVENHOLM ){ /* set later */ }
			else									 { row = 81; col = 21; }
			break;
		case UM_0_Gharbad_the_Weak:			// (th4) Dark Campaign hub blacksmith; elsewhere he keeps his quest placement
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 45; col = 27; }
			break;
		case UM_30_Boilrot:					// (th4) Dark Campaign hub stash
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 28; col = 17; }
			break;
		case UM_48_Molianta_the_Copperwing:	// (th4) Dark Campaign hub quench
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 37; col = 25; }
			break;
		case UM_51_Galahad_the_Bonewhite:	// (th4) Dark Campaign hub crafting
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 37; col = 35; }
			break;
		case UM_126_Gloomshredder:			// (th4) Dark Campaign hub transmute
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 29; col = 44; }
			break;
		case UM_792_Salinus_the_Salt_King:	// (th4) Dark Campaign hub healer
			if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){ row = 37; col = 50; }
			break;
		case UM_774_Swarmbringer:			row = 28;	col = 64; break;
		case UM_788_Grimgar_the_Fallen:		row = 24;	col = 38; break;
		case UM_789_Bonecrunch_the_Risen:	row = 24;	col = 36; break;
		case UM_790_Ghost_of_Brother_Martin:/*if (Dungeon == DUN_110_BLACK_CHAPEL)	*/{ row = 69;	col = 54; } break;
		case UM_791_Festergnaw:				/*if (Dungeon == DUN_110_BLACK_CHAPEL)	*/ { row = 69;	col = 54; } break;
		case UM_805_Festerus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 29;	col = 31; } break;
		case UM_806_Obfuscus:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 78;	col = 78; } break;
		case UM_807_Stormire:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 30;	col = 74; } break;
		case UM_907_DC_ZYPHERYS:			if (Dungeon == DUN_112_STILL_SPRINGS) { row = 30;	col = 74; } break;	// (th4) DC: takes Stormire's spot
		case UM_908_DC_LORD_KINDRETT:		if (Dungeon == DUN_114_LOWER_SCRIPTORIUM) { row = 59;	col = 30; } break;	// (th4) DC
		case UM_909_DC_SCRIBE_VABIUS:		if (Dungeon == DUN_114_LOWER_SCRIPTORIUM) { row = 23;	col = 41; } break;	// (th4) DC
		case UM_910_DC_AZDOG:				if (Dungeon == DUN_64_MAUSOLEUM) { row = 46;	col = 32; } break;	// (th4) DC
		case UM_911_DC_COMMANDER_ALDRIC:	if (Dungeon == DUN_70_CITADEL) { row = 74;	col = 37; } break;	// (th4) DC
		case UM_912_DC_CAPTAIN_ILSE:		if (Dungeon == DUN_70_CITADEL) { row = 32;	col = 41; } break;	// (th4) DC
		case UM_913_DC_MAGISTER_CORVANE:	if (Dungeon == DUN_70_CITADEL) { row = 66;	col = 66; } break;	// (th4) DC
		case UM_914_DC_BROTHER_SILAS:		if (Dungeon == DUN_70_CITADEL) { row = 38;	col = 66; } break;	// (th4) DC
		case UM_916_DC_PALADIN_LOXLEY:		if (Dungeon == DUN_66_SEWERS)  { row = 36;	col = 36; } break;	// (th4) DC mission 7
		case UM_917_DC_PRIEST_GWIDEON:		if (Dungeon == DUN_66_SEWERS)  { row = 65;	col = 63; } break;	// (th4) DC mission 7
		case UM_918_DC_MADAME_PERESIDE:		if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 87;	col = 51; } break;	// (th4) DC mission 8
		case UM_919_DC_Archimandrite_Bozu:	if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 48;	col = 79; } break;	// (th4) DC mission 8
		case UM_920_DC_Bishop_Lucian:		if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 45;	col = 32; } break;	// (th4) DC mission 8
		case UM_921_DC_Deacon_Raduel:		if (Dungeon == DUN_99_ANCIENT_CRYPTS) { row = 25;	col = 51; } break;	// (th4) DC mission 8
		case UM_922_DC_Donna_Paloosa:		if (Dungeon == DUN_47_DARK_NEST) { row = 20;	col = 20; } break;	// (th4) DC mission 9
		case UM_923_DC_Abbot_Hirosh:		if (Dungeon == DUN_47_DARK_NEST) { row = 43;	col = 31; } break;	// (th4) DC mission 9
		case UM_924_DC_Commander_Latham:	if (Dungeon == DUN_47_DARK_NEST) { row = 75;	col = 23; } break;	// (th4) DC mission 9
		case UM_925_DC_Abbot_Balatt:		if (Dungeon == DUN_118_POISONED_WATER_SUPPLY_DC) { row = 20;	col = 33; } break;	// (th4) DC mission 10
		case UM_931_DC_Madame_Sarbine:	if (Dungeon == DUN_121_UNHOLY_ALTAR_DC) { row = 35; col = 58; } break;	// (th4) DC mission 13
		case UM_935_DC_Captain_Nagvot:	if (Dungeon == DUN_43_HEPHASTO) { row = 28; col = 53; } break;	// (th4) DC mission 15
		case UM_926_DC_Black_Hawk:			if (Dungeon == DUN_119_DEEPER_CAVES) { row = 47;	col = 83; } break;	// (th4) DC mission 11
		case UM_928_DC_Darkmoor:			if (Dungeon == DUN_120_PANDEMONIUM_DC) { row = 75;	col = 73; } break;	// (th4) DC mission 12
		case UM_929_DC_Moonrose:			if (Dungeon == DUN_120_PANDEMONIUM_DC) { row = 33;	col = 33; } break;	// (th4) DC mission 12
		case UM_930_DC_Commander_Degroe:	if (Dungeon == DUN_120_PANDEMONIUM_DC) { row = 33;	col = 75; } break;	// (th4) DC mission 12
		case UM_927_DC_General_Carnabus:	if (Dungeon == DUN_120_PANDEMONIUM_DC) { row = 77;	col = 31; } break;	// (th4) DC mission 12
		case UM_808_Winggore:				if (Dungeon == DUN_112_STILL_SPRINGS) { row = 85;	col = 48; } break;
		case UM_809_Borosteron:				if (Dungeon == DUN_113_THE_MAZE)	  { row = 87;	col = 23; } break;
		case UM_903_AZGOR:					if (Dungeon == DUN_116_MINING_CAVES) { row = 77;	col = 20; } break;
		case UM_904_DOBAZZU:				if (Dungeon == DUN_116_MINING_CAVES) { row = 80;	col = 20; } break;
		case UM_938_DC_Lord_Tiberius:		if (Dungeon == DUN_116_MINING_CAVES) { row = 77;	col = 20; } break;	// (th4) DC mission 17
		case UM_939_DC_Ravenshot:			if (Dungeon == DUN_116_MINING_CAVES) { row = 80;	col = 20; } break;	// (th4) DC mission 17
		case UM_936_DC_Ludwig_the_Paranoid:	if (Dungeon == DUN_51_FETID_CAVERN) { row = 33;	col = 51; } break;	// (th4) DC mission 16
		case UM_937_DC_Lord_Voss:			if (Dungeon == DUN_51_FETID_CAVERN) { row = 23;	col = 51; } break;	// (th4) DC mission 16
		case UM_644:						if (Dungeon == DUN_72_NECROPOLIS && GameMode == GM_DARK_CAMPAIGN) { row = 20;	col = 88; } break;	// (th4) DC mission 19
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
		case UM_587_Hephasto_the_Armorer:	row = 22;	col = (GameMode == GM_DARK_CAMPAIGN) ? 88 : 83;	break;	// (th4) DC m15 forge tile
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
		case UM_726_Apparition_of_Arkaine:	row = 48;	col = 44;	break;
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
		case UM_903_AZGOR:					if (Dungeon == DUN_116_MINING_CAVES) { row = 77;	col = 20; } break;
		case UM_904_DOBAZZU:				if (Dungeon == DUN_116_MINING_CAVES) { row = 80;	col = 20; } break;
		}
	}
	int mine = 0;
	switch( bossId ){
	// (th4) DC mission 25: the Butcher is placed by the switch above -- on his cell tile in
	// the Sacristy, beside the player everywhere else. This search puts him on map tile 367,
	// the butcher's room from the vanilla rnd7.DUN quest pattern, which no Dark Campaign map
	// contains: it found nothing and left row/col at 112, off the map entirely.
	case UM_9_The_Butcher or UM_C_822_The_Butcher:
		if( GameMode != GM_DARK_CAMPAIGN ) mine = 367;
		break;
	case UM_80_Hissabal:	mine = 317; break;
	case UM_32_Lothamaarg:	mine = 319; break;
	}
	if( mine ){
		// (th4) Find the marker tile, and touch row/col ONLY if it is really there.
		//
		// The decompiled loops that used to be here ran to completion on a miss and left row and col
		// at 112, which their trailing ++ turned into 113 -- two past the end of the 112x112
		// MonsterMap that CreateBossBaseMonster then writes. The Butcher already hit exactly
		// this on Dark Campaign maps; see the comment on his case above. Hissabal and
		// Lothamaarg can still hit it any time a .dun stops carrying tile 317 or 319.
		//
		// On a miss the boss now simply keeps whatever position it already had, which is the
		// random one the placement loop picked and validated.
		//
		// Scan order and result are unchanged: first column that contains the marker, first row
		// within it, then +2 on each axis. The +2 is not arbitrary -- the old inner loop left
		// row one past the hit, the outer left col one past, and then both were incremented
		// again after the loops.
		int foundRow = -1, foundCol = -1;
		for( int c = 0; c < FineMap_112 && foundRow < 0; ++c ){
			for( int r = 0; r < FineMap_112; ++r ){
				if( FineMap[ r ][ c ] == mine ){ foundRow = r; foundCol = c; break; }
			}
		}
		if( foundRow >= 0 ){
			row = foundRow + 2;
			col = foundCol + 2;
		}
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
	// (th4) DARK CAMPAIGN HUB: kin hold a fixed heading instead of CreateBossBaseMonster's
	// random RNG(8). Orientation AND curAnimation are set as a pair, exactly as CreateMonster
	// does -- setting orientation alone would leave the sprite drawing its old heading.
	if( Dungeon == DUN_117_DARKLORD_HIDEOUT ){
		// (th4) Some hub kin inherit MF_1_INVISIBLE from their BASE MONSTER row -- it is baked
		// into BaseMonster.cpp for the whole Hidden family (BM_32_INVISIBLE_ONE etc), not set
		// by AI, so CreateMonster copies it at spawn and the hub's AI gate never gets a chance
		// to clear it. Invisible means both undrawn (Draw.cpp) and unclickable (Main.cpp), so
		// a vendor with that flag is unusable. Hub kin never ambush anyone; strip it.
		boss->flag &= ~MF_1_INVISIBLE;

		int hubFacing = -1;
		switch( bossId ){
		case UM_773_The_Dark_Lord:				hubFacing = DIR_0_D;	break;	// south
		case UM_0_Gharbad_the_Weak:				hubFacing = DIR_7_DR;	break;	// south-east
		case UM_19_Jaldabaoth:					hubFacing = DIR_1_DL;	break;	// south-west
		case UM_30_Boilrot:						hubFacing = DIR_1_DL;	break;	// south-west
		case UM_48_Molianta_the_Copperwing:		hubFacing = DIR_7_DR;	break;	// south-east
		case UM_51_Galahad_the_Bonewhite:		hubFacing = DIR_7_DR;	break;	// south-east
		case UM_89_Alystar:						hubFacing = DIR_7_DR;	break;	// south-east
		case UM_94_Setebos:						hubFacing = DIR_7_DR;	break;	// south-east
		case UM_124_Torvatus:					hubFacing = DIR_1_DL;	break;	// south-west
		case UM_126_Gloomshredder:				hubFacing = DIR_7_DR;	break;	// south-east
		case UM_792_Salinus_the_Salt_King:		hubFacing = DIR_7_DR;	break;	// south-east
		case UM_6_Dark_Jade:					hubFacing = DIR_0_D;	break;	// south
		case UM_5_Red_Vex:						hubFacing = DIR_0_D;	break;	// south
		}
		if( hubFacing >= 0 ){
			boss->Orientation = hubFacing;
			boss->curAnimation = &boss->SpritePtr->animation[ANIM_0_STAY].oriented[hubFacing];
		}
	}
	boss->newBossId = bossId + 1;// (rt) (5) boss number extension 0043BCD9
	boss->MonsterLevel += 4 + 3 * Difficulty / 4;
	//boss->Xp *= 2;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && uniqMonster->IceAgeNamePtr) {
		boss->Name = (char*)LocalizedIceAgeName(uniqMonster->IceAgeNamePtr);
	}
	else {
		boss->Name = (char*)LocalizedUniqueMonsterName( uniqMonster );
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
	// (rt) here Lazarus enters the talking state again
	boss->speechIndex = uniqMonster->SpecialSound;
	if( BossHasNoLight( bossId ) ){ // full deactivation of boss light (the list: BossHasNoLight, above)
		//boss->LightIndex = 0; // (th4) Rev 2.46 ORIGINAL (buggy): 0 is the no-light sentinel BUT also a real LightList slot (usually the player's torch) — the death/fade paths (Monster_Fade_Out etc.) called ChangeLightPos(0, ...) with it and teleported the player's light to the boss tile / (0,0), the reported "killed a Hidden boss -> all lights off including my own" bug.
		boss->LightIndex = -1; // (th4) Rev 2.46: no-light sentinel that ChangeLightPos/LightOff also reject — the boss light paths become no-ops instead of hijacking slot 0.
	}else{
		boss->LightIndex = AddLight(boss->Row, boss->Col, 3);
		// the two bosses from above were included here, but they never worked? didn't I test this before? I should have... Was this broken during lighting work?
		if (is(bossId, UM_C_868_RED_VEX, UM_C_867_LAZARUS, UM_C_869_BLACKJADE, UM_4_ArchBishop_Lazarus, 
			UM_5_Red_Vex, UM_6_Dark_Jade, UM_676_Hazeweaver, UM_677_Guardian_of_Valor, UM_C_871_HAZEWEAVER, UM_C_902_RAGEHORN)) { //special condition to make lights not be not visible at the very beggining for a fractal of second
			ChangeLightPos(boss->LightIndex, 0, 0);
		}
	}
	if( MaxCountOfPlayersInGame != 1 && boss->ai == AI_29_RedVex ){ // Ai_29_Redvex / MAI_LazarusHelp
		boss->speechIndex = 0;
	}
	// (rt) second part of the Lazarus patch from D1 1.07-1.09
	// (th4) DARK CAMPAIGN escort: Red Vex and Dark Jade carry Lazarus' SpecialSound, so the
	// speechIndex test below would park them on MG_6_TALK -- standing still and reciting the
	// "abandon your foolish quest" line when clicked. While they are serving as the player's
	// escort they are soldiers, not NPCs: drop the speech and send them down the MG_1_ATTACK
	// path (same escape the Void Chambers Lazarus uses).
	if( GameMode == GM_DARK_CAMPAIGN && DarkCampaignIsEscortBoss( bossId )
	 && (DarkCampaignEscortActive() || DarkCampaignEscortWalkingHome) ){
		boss->speechIndex = 0;
		goto OTAKE;
	}
	// (th4) DC mission 13: Lazarus carries the same SpecialSound (35) as the girls, so
	// without this he parks on MG_6_TALK and recites the "abandon your foolish quest"
	// line when clicked -- and AI_28_Lazarus then runs the vanilla lair script when the
	// audio ends, which blits an AltMap this dungeon never loads. Drop the speech and
	// send him down the same path the escort takes, so he simply behaves like them.
	// The line itself is played once on arrival instead, from the DUN_121 spawn block.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
	 && bossId == UM_4_ArchBishop_Lazarus ){
		boss->speechIndex = 0;
		goto OTAKE;
	}
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
			if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE, GM_OUTBREAK)) {
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
			case DL_1_PURGATORY:	addups_boss = Dungeon->level / 6; break;// +(0 - 4)
			case DL_2_DOOM:			addups_boss = 4 + Dungeon->level / 3; break;// +(4 - 12)
			case DL_3_CRUCIBLE:		addups_boss = 4 + 8 + Dungeon->level / 2; break;// +(12 - 24)
			case DL_4_TORMENT:		addups_boss = 4 + 8 + 12 + Dungeon->level / 2; break;// +(24 - 36)
			case DL_5_AGONY:		addups_boss = 4 + 8 + 12 + 12 + Dungeon->level / 2; break;// +(36 - 48)
			case DL_6_TERROR:		addups_boss = 4 + 8 + 12 + 12 + 12 + Dungeon->level / 2; break;// +(48 - 60)
			case DL_7_DESTRUCTION:	addups_boss = 4 + 8 + 12 + 12 + 12 + 12 + Dungeon->level;	  break;// +(60 - 84)
			case DL_8_INFERNO:		addups_boss = 4 + 8 + 12 + 12 + 12 + 12 + 24 + 2 * Dungeon->level; break;// +(84 - 132)
			}
			// cancelling addups until sigils and powerups are added
			//addups_boss = 0;

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
	// (th4) Every unique gets its own LightTable row. uniqTrans is the offset from
	// LightTable_BossRowFirst; DrawMonster passes uniqTrans + 4 and CL2_DrawDark maps it
	// back to row 19 + uniqTrans, so the row IS the monster's colour identity.
	LoadFileWithMem( trnFileName, &LightTable[256 * (BossLightIndex + LightTable_BossRowFirst)] );
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

#define TH4_MONSTERMAP_DEBUG 0

// ===================== DEBUG INSTRUMENTATION (th4) -- disabled ===================
// Set TH4_MONSTERMAP_DEBUG to 1 to re-enable. Kept because this bug class (a monster
// written onto an already-occupied MonsterMap cell -> invisible + unclickable until it
// moves) can recur: several writers still claim cells without checking.
// Logs every time a monster is written onto a MonsterMap cell that is already taken.
// Writes to TH4_stomp.log next to the exe. REMOVE THIS once the invisible-boss bug
// is understood -- it changes no behaviour, it only observes.
#if TH4_MONSTERMAP_DEBUG
static void StompLog(const char* what, int monsterIndex, int row, int col, int occupant)
{
	if( FILE* f = fopen("TH4_stomp.log", "a") ){
		const char* victimName = "?";
		int victimBoss = 0;
		if( occupant > 0 && (occupant - 1) < Monsters_Max_Count ){
			Monster& v = Monsters[occupant - 1];
			if( v.Name ) victimName = v.Name;
			victimBoss = v.newBossId;
		}
		const char* newName = "?";
		int newBoss = 0;
		if( monsterIndex >= 0 && monsterIndex < Monsters_Max_Count ){
			Monster& n = Monsters[monsterIndex];
			if( n.Name ) newName = n.Name;
			newBoss = n.newBossId;
		}
		fprintf(f, "[%s] dungeon=%d mode=%d  cell(%d,%d)  NEW idx=%d boss=%d '%s'  OVERWRITES occupant=%d (idx=%d) boss=%d '%s'\n",
			what, (int)(DUNGEON)Dungeon, (int)GameMode, row, col,
			monsterIndex, newBoss, newName,
			occupant, occupant - 1, victimBoss, victimName);
		fclose(f);
	}
}
#endif
// =================== END DEBUG INSTRUMENTATION ===================================

// ===================== DEBUG INSTRUMENTATION (th4) -- disabled ===================
// After a level is fully built, check every live monster actually owns its own tile in
// MonsterMap[][]. Any monster whose tile points elsewhere is invisible AND unclickable
// (both the renderer and the cursor hit-test look monsters up through MonsterMap), and
// will only "appear" once it walks and re-registers itself.
// Writes to TH4_stomp.log next to the exe. REMOVE once the bug is understood.
#if TH4_MONSTERMAP_DEBUG
void AuditMonsterMap(const char* when)
{
	if( FILE* f = fopen("TH4_stomp.log", "a") ){
		fprintf(f, "=== AUDIT (%s) dungeon=%d mode=%d monstersCount=%d ===\n",
			when, (int)(DUNGEON)Dungeon, (int)GameMode, MonstersCount);
		int ghosts = 0;
		// skip the reserved summon slots, they live at (0,0) by design
		for( int i = SummonMonsters_Count; i < MonstersCount; ++i ){
			Monster& m = Monsters[i];
			if( !In112(m.Row, m.Col) ){
				fprintf(f, "  OUT-OF-BOUNDS idx=%d boss=%d '%s' at (%d,%d)\n",
					i, m.newBossId, m.Name ? m.Name : "?", m.Row, m.Col);
				++ghosts;
				continue;
			}
			int owner = MonsterMap[m.Row][m.Col];
			if( owner != i + 1 ){
				const char* thiefName = "<empty>";
				if( owner > 0 && (owner - 1) < Monsters_Max_Count && Monsters[owner-1].Name ){
					thiefName = Monsters[owner-1].Name;
				}
				fprintf(f, "  GHOST idx=%d boss=%d '%s' at (%d,%d): tile owned by %d (idx=%d '%s')\n",
					i, m.newBossId, m.Name ? m.Name : "?", m.Row, m.Col,
					owner, owner - 1, thiefName);
				++ghosts;
			}
		}
		fprintf(f, "=== AUDIT done: %d ghost(s) ===\n\n", ghosts);
		fclose(f);
	}
}
#endif
// =================== END DEBUG INSTRUMENTATION ===================================

// (th4) Deterministic search for the nearest cell a monster may stand on, spiralling
// outward from (row,col) in rings of increasing Chebyshev distance. Returns true and
// writes the result to outRow/outCol; false if nothing suitable is in range.
//
// IsCellFreeForMonster() is exactly the predicate we want and is itself deterministic:
// in-bounds + no monster + no player + not CF_2_VISIBLE_BY_PLAYER/CF_8_FORBIDDEN
// + IsCellBlocking()==0 (i.e. TileBlockWalking[FineMap[][]], so walls are rejected).
//
// IMPORTANT: this must stay RNG-free. The number of RNG calls made while building a
// level has to be identical every time it is rebuilt, or boss stats/affixes shuffle on
// TP re-entry (see the long comment in CreateBoss). A fixed scan order draws no RNG.
static bool __fastcall FindNearestFreeCellForMonster(int row, int col, int& outRow, int& outCol)
{
	const int MaxRadius = 8;
	for( int radius = 1; radius <= MaxRadius; ++radius ){
		for( int dRow = -radius; dRow <= radius; ++dRow ){
			for( int dCol = -radius; dCol <= radius; ++dCol ){
				if( abs(dRow) != radius && abs(dCol) != radius ) continue; // ring only
				int r = row + dRow;
				int c = col + dCol;
				if( IsCellFreeForMonster(r, c) ){
					outRow = r; outCol = c;
					return true;
				}
			}
		}
	}
	return false;
}

// (th4) Move an already-created monster off (row,col) to the nearest free walkable cell.
//
// Why this exists: MonsterMap[][] holds one monster per tile and is how BOTH the renderer
// (DrawMonsterInTile) and the cursor hit-test find monsters. Bosses with hardcoded spawn
// coordinates (see the switch in CreateBoss) claim their cell without checking it, so they
// can overwrite a monster that is already standing there -- typically a pack member of an
// earlier boss placed nearby. The overwritten monster keeps its Row/Col but owns no tile,
// so it is invisible AND unclickable until it happens to walk somewhere and re-register.
// Observed on DUN_104: Doomfang is forced to (22,53) and his Horned Death pack spreads
// around him; Fleshblight is then forced to (25,53) and stomps whichever pack member
// landed there.
//
// The boss keeps its authored cell (those coordinates are deliberate level design) and the
// occupant is nudged aside instead. No RNG is drawn: the monster already exists, we only
// move it, so the level's RNG sequence is unchanged.
static void __fastcall DisplaceMonsterFromCell(int occupantIndex, int row, int col)
{
	if( occupantIndex < 0 || occupantIndex >= Monsters_Max_Count ) return;
	Monster& m = Monsters[occupantIndex];
	int newRow, newCol;
	if( !FindNearestFreeCellForMonster(row, col, newRow, newCol) ){
		return; // nowhere to go: leave it, the stomp below is no worse than before
	}
	ClearMonsterOnMap(occupantIndex); // clears the cells around PrevRow/PrevCol
	if( In112(m.Row, m.Col) && MonsterMap[m.Row][m.Col] == occupantIndex + 1 ){
		MonsterMap[m.Row][m.Col] = 0; // in case Prev* had already diverged from Row/Col
	}
	m.Row = m.NextRow = m.PrevRow = newRow;
	m.Col = m.NextCol = m.PrevCol = newCol;
	MonsterMap[newRow][newCol] = occupantIndex + 1;
	CheckMonsterUnderArch(occupantIndex);
}

//----- (th4) -------------------------------------------------------------
// (th4) Re-register any live monster whose own tile no longer points back at it.
//
// Forced placement (bosses, packs, and the .dun map's own monsters, which are laid down LAST
// in AddQuestMonsters) overwrites MonsterMap after trying to displace whoever was standing
// there. When DisplaceMonsterFromCell cannot find a free cell within its 8-tile search it
// gives up and lets the stomp happen -- and the monster it failed to move is then orphaned:
// still drawn, still alive, but absent from MonsterMap, so the cursor cannot pick it and
// nothing can attack it. On a crowded level that is a stuck "monsters left" counter and no
// exit portal. ClearMapFromInvalidMonsters only clears stale cells; it never restores one.
//
// This runs after all placement and puts every orphan back on the map -- reclaiming its own
// tile if free, otherwise moving it to the nearest free one.
void RepairMonsterMap()
{
	for( int i = 0; i < MonstersCount; ++i ){
		const int mi = MonsterIndexes[i];
		if( mi < 0 || mi >= Monsters_Max_Count ) continue;
		Monster& m = Monsters[mi];
		if( !m.SpritePtr ) continue;
		if( m.flag & MF_15_KILLED ) continue;
		if( (m.CurrentLife & ~63) <= 0 ) continue;
		if( m.Row == 1 && m.Col == 0 ) continue;	// dormant summon slot, not on the map yet
		if( !In112(m.Row, m.Col) ) continue;
		if( MonsterMap[m.Row][m.Col] == mi + 1 ) continue;	// already correct

		if( MonsterMap[m.Row][m.Col] == 0 ){
			MonsterMap[m.Row][m.Col] = mi + 1;		// its tile is free: just reclaim it
			continue;
		}
		int newRow, newCol;							// someone else holds it: step aside
		if( FindNearestFreeCellForMonster(m.Row, m.Col, newRow, newCol) ){
			ClearMonsterOnMap(mi);
			m.Row = m.NextRow = m.PrevRow = newRow;
			m.Col = m.NextCol = m.PrevCol = newCol;
			MonsterMap[newRow][newCol] = mi + 1;
			CheckMonsterUnderArch(mi);
		}
	}
}

//----- (th4) -------------------------------------------------------------
// (th4) MONSTERS UNDER DOORS (DSF 1.09 bug list). InitMonsters places the monsters before InitObjects
// builds the doors, and IsCellFreeForMonster knows nothing about objects, so a monster could be
// generated on a door tile: the door was then built shut around it, and it stood "inside" the door
// until it walked out at the hero. Main.cpp LoadGameLevel calls this right after InitObjects on a newly
// generated random level (quest maps place their monsters by hand): a monster standing on a door, or on
// any other object that blocks walking, moves to the nearest free cell without such an object. RNG-free,
// like FindNearestFreeCellForMonster, so the level's RNG sequence stays the same; a boss takes his light.
static bool CellHasBlockingObject( int row, int col )
{
	const int objNum = ObjectsMap[ row ][ col ];
	if( !objNum ) return false;
	const Object& object = Objects[ abs( objNum ) - 1 ];
	return object.isDoor || object.isBlockWalking;
}
void MoveMonstersOffBlockingObjects()
{
	for( int i = 0; i < MonstersCount; ++i ){
		const int mi = MonsterIndexes[i];
		if( mi < 0 || mi >= Monsters_Max_Count ) continue;
		Monster& m = Monsters[mi];
		if( !m.SpritePtr ) continue;
		if( m.Row == 1 && m.Col == 0 ) continue;	// dormant summon slot, not on the map yet
		if( !In112( m.Row, m.Col ) || !CellHasBlockingObject( m.Row, m.Col ) ) continue;
		bool found = false;
		int newRow = 0, newCol = 0;
		for( int radius = 1; radius <= 8 && !found; ++radius ){
			for( int dRow = -radius; dRow <= radius && !found; ++dRow ){
				for( int dCol = -radius; dCol <= radius; ++dCol ){
					if( abs( dRow ) != radius && abs( dCol ) != radius ) continue;	// ring only
					const int r = m.Row + dRow, c = m.Col + dCol;
					if( In112( r, c ) && IsCellFreeForMonster( r, c ) && !CellHasBlockingObject( r, c ) ){
						newRow = r; newCol = c; found = true;
						break;
					}
				}
			}
		}
		if( !found ) continue;
		ClearMonsterOnMap( mi );
		if( MonsterMap[ m.Row ][ m.Col ] == mi + 1 ) MonsterMap[ m.Row ][ m.Col ] = 0;
		m.Row = m.NextRow = m.PrevRow = newRow;
		m.Col = m.NextCol = m.PrevCol = newCol;
		MonsterMap[ newRow ][ newCol ] = mi + 1;
		CheckMonsterUnderArch( mi );
		if( m.LightIndex > 0 && !( m.flag & MF_1_INVISIBLE ) ) ChangeLightPos( m.LightIndex, newRow, newCol );
	}
}

//----- (th4) -------------------------------------------------------------
// (th4) Put one of the escort back on her post, map bookkeeping and all. Used both when the
// walk home times out and when the hub notices one standing somewhere she should not be --
// after a save/reload, for instance, where monster positions come back from the delta and
// the walk-home latch (deliberately unsaved) is gone, so nothing would otherwise move her.
// (th4) DC mission 15: the forge tile the armorer belongs on.
const int DC_HEPHASTO_POST_ROW = 22;
const int DC_HEPHASTO_POST_COL = 88;

// (th4) Ticks spent walking home. Reset when he arrives and when the level generates --
// see the DUN_43 block in AddQuestMonsters, which is why this is a file static rather
// than a function-local one.
static int gHephastoWalkHomeTicks = 0;

// (th4) Distance-to-forge field, in steps, for every tile on the level. DC_HOME_UNREACHED means
// "not reachable" (or not built yet). Built lazily by the BFS below and thrown away when
// the level regenerates, alongside the walk-home timer.
//
// Why this exists: FindPath() is an A* capped at MaxPathStep_25, and at the cap it does not
// return a shortened route -- it fails outright ("if( maxDistance == 25 ) return 0"). The
// forge room is wide enough that from most of it the armorer is over 24 steps from his post,
// so MakeMonsterPath() refused every single tick and he fell through to the straight-line
// fallback, which knows nothing about walls. He would walk face-first into one and stay
// there until the 50s give-up teleported him. A flood fill from the post has no range limit
// and encodes the whole room's geometry, so stepping downhill on it walks him around walls.
const unsigned short DC_HOME_UNREACHED = 0xFFFF;
static unsigned short gHephastoHomeDist[112][112];
static bool gHephastoHomeDistBuilt = false;

// (th4) What the field was built for. DarkCampaignResetHephastoWalkHome() only fires from the
// DUN_43 branch of AddQuestMonsters, i.e. on generation -- but a level entered from a save
// comes back off the delta without regenerating, so the built flag alone would happily hand
// out a field flooded through a DIFFERENT layout of this level from earlier in the session
// and march him into walls that are not there. Cheap to key it and rebuild on any mismatch.
static DUNGEON gHephastoHomeDistDungeon = DungeonCount;	// DungeonCount == "no level", never a real one
static int gHephastoHomeDistSeed = 0;

// (th4) Last direction walked, so a sidestep cannot immediately undo itself and leave him
// jittering between two tiles. Cleared with the rest of the walk-home state.
static int gHephastoLastStepOrient = -1;

// (th4) Would terrain and scenery let him stand here? IsCellBlocking() alone is NOT enough:
// it only consults TileBlockWalking, so it happily reports an open floor tile that has a
// blocking object sitting on it. The fill believed those tiles were walkable, sent him
// straight at one, MayMoveMonsterToDirection() refused the step, and with no other downhill
// neighbour he simply stood there -- the corner he was found stuck in.
static bool DarkCampaignHomeTileOpen( int row, int col, uchar canFly )
{
	if( !In112(row, col) ) return false;
	if( IsCellBlocking(row, col, canFly) ) return false;
	const int objNum = ObjectsMap[row][col];
	if( objNum ){
		const int objIndex = objNum <= 0 ? -1 - objNum : objNum - 1;
		if( Objects[objIndex].isBlockWalking ) return false;
	}
	return true;
}

void DarkCampaignResetHephastoWalkHome()
{
	gHephastoWalkHomeTicks = 0;
	gHephastoHomeDistBuilt = false;	// geometry is per-level: rebuild on the next walk home
	gHephastoHomeDistDungeon = DungeonCount;
	gHephastoHomeDistSeed = 0;
	gHephastoLastStepOrient = -1;
}

// (th4) Flood fill outward from the forge tile over ground the armorer could stand on.
// Terrain and scenery only -- monsters, players and missiles move, and baking them in would
// freeze a snapshot of them into the field. Those are checked against the live world by
// MayMoveMonsterToDirection() at the point of use instead.
static void DarkCampaignBuildHephastoHomeDist( uchar canFly )
{
	gHephastoHomeDistDungeon = (DUNGEON)Dungeon;	// CurDungeon deletes operator int on purpose -- go through DUNGEON
	gHephastoHomeDistSeed = DungeonRandSeed[(DUNGEON)Dungeon];
	for( int r = 0; r < 112; ++r ){
		for( int c = 0; c < 112; ++c ){
			gHephastoHomeDist[r][c] = DC_HOME_UNREACHED;
		}
	}

	static short queue[112 * 112][2];	// static: 50KB is too much for the stack
	int head = 0, tail = 0;

	gHephastoHomeDist[DC_HEPHASTO_POST_ROW][DC_HEPHASTO_POST_COL] = 0;
	queue[tail][0] = (short)DC_HEPHASTO_POST_ROW;
	queue[tail][1] = (short)DC_HEPHASTO_POST_COL;
	++tail;

	while( head < tail ){
		const int r = queue[head][0], c = queue[head][1];
		++head;
		const unsigned short next = (unsigned short)(gHephastoHomeDist[r][c] + 1);

		fori( 8 ){
			const int nr = r + RowDelta[i];
			const int nc = c + ColDelta[i];
			if( !In112(nr, nc) ) continue;
			if( gHephastoHomeDist[nr][nc] != DC_HOME_UNREACHED ) continue;	// already reached, and BFS got there first with a shorter count
			if( !DarkCampaignHomeTileOpen(nr, nc, canFly) ) continue;
			gHephastoHomeDist[nr][nc] = next;
			queue[tail][0] = (short)nr;
			queue[tail][1] = (short)nc;
			++tail;
		}
	}
	gHephastoHomeDistBuilt = true;
}

// (th4) One step downhill on the field. Returns false when he is walled in for the moment --
// a corpse or another monster on the only good tile, say -- in which case the caller simply
// tries again next tick.
static bool DarkCampaignStepHephastoHome( int mi )
{
	// (th4) Defence in depth. Every caller is already inside a
	// GM_DARK_CAMPAIGN && DUN_43_HEPHASTO branch, but the field is a 25KB global that must
	// never be built or consulted outside mission 15 -- the forge post below is a DC-only
	// coordinate and would be meaningless anywhere else.
	if( GameMode != GM_DARK_CAMPAIGN || Dungeon != DUN_43_HEPHASTO ) return false;

	Monster& m = Monsters[mi];
	if( !gHephastoHomeDistBuilt
	 || gHephastoHomeDistDungeon != (DUNGEON)Dungeon
	 || gHephastoHomeDistSeed != DungeonRandSeed[(DUNGEON)Dungeon] ){
		DarkCampaignBuildHephastoHomeDist( m.canFly );
	}
	if( !In112(m.Row, m.Col) ) return false;
	const unsigned short here = gHephastoHomeDist[m.Row][m.Col];
	if( here == 0 ) return false;	// already home

	// Pick the reachable neighbour with the lowest distance -- the lowest, not the first
	// improvement, so he does not zig-zag around pillars. RowDelta/ColDelta are indexed by
	// orientation, so the loop counter IS the direction to walk.
	//
	// "here" is the bar to beat normally. But when he is standing OFF the field entirely
	// (DC_HOME_UNREACHED -- pushed into a nook the fill considers closed, which is possible
	// since the fill is terrain+scenery and the shove that put him there was not) there is
	// nothing to beat, so accept any field tile at all and let him rejoin it.
	const bool offField = ( here == DC_HOME_UNREACHED );
	int bestOrient = -1;
	unsigned short bestDist = here;
	fori( 8 ){
		const int nr = m.Row + RowDelta[i];
		const int nc = m.Col + ColDelta[i];
		if( !In112(nr, nc) ) continue;
		const unsigned short there = gHephastoHomeDist[nr][nc];
		if( there == DC_HOME_UNREACHED ) continue;
		if( !offField && there >= bestDist ) continue;
		if( offField && bestOrient >= 0 && there >= bestDist ) continue;
		if( !MayMoveMonsterToDirection( mi, i ) ) continue;	// live check: something is standing there
		bestDist = there;
		bestOrient = i;
	}

	// Nothing downhill was free. Rather than stand and wait -- which is what left him wedged
	// in a corner -- slide to the best sideways-or-slightly-worse tile that IS free and try
	// again from there. Never straight back the way he came, or he would oscillate forever.
	if( bestOrient < 0 && !offField ){
		unsigned short sideDist = DC_HOME_UNREACHED;
		fori( 8 ){
			if( gHephastoLastStepOrient >= 0 && i == OrientInverted[gHephastoLastStepOrient] ) continue;
			const int nr = m.Row + RowDelta[i];
			const int nc = m.Col + ColDelta[i];
			if( !In112(nr, nc) ) continue;
			const unsigned short there = gHephastoHomeDist[nr][nc];
			if( there == DC_HOME_UNREACHED || there >= sideDist ) continue;
			if( !MayMoveMonsterToDirection( mi, i ) ) continue;
			sideDist = there;
			bestOrient = i;
		}
	}
	if( bestOrient < 0 ) return false;

	gHephastoLastStepOrient = bestOrient;
	StartMonsterWalk( mi, bestOrient );	// direct: the anti-cornering in MonsterWalksTowardsTarget would second-guess the field
	return true;
}

// (th4) Is he actually standing at his anvil? The talk and the "!" mark both wait on this,
// so he cannot be spoken to while he is still crossing the room.
bool DarkCampaignHephastoAtForge()
{
	int mi = DarkCampaignHephastoIndex();
	if( mi < 0 ) return false;
	const Monster& m = Monsters[mi];
	return m.Row == DC_HEPHASTO_POST_ROW && m.Col == DC_HEPHASTO_POST_COL;
}

// (th4) Walk the armorer back to his forge once the fighting is over.
//
// He WALKS -- same mechanism Ai_DarkCampaignEscort uses to close on the player: set
// TargetRow/TargetCol and let MakeMonsterPath do the routing, with a single step toward
// the target as the fallback when a path is refused. None of that needs his combat AI,
// which is what the guard on this level withholds.
//
// The give-up timer is the same idea as the escort's: a route can be blocked by a corpse
// pile or simply refused for length, and the mission cannot be completed until he is home,
// so something has to guarantee he gets there. Only ticks while he is walking -- the
// caller invokes this only when nothing is left to fight.
static void DarkCampaignReturnHephastoToForge( int mi )
{
	const int postRow = DC_HEPHASTO_POST_ROW, postCol = DC_HEPHASTO_POST_COL;
	if( mi < 0 || mi >= Monsters_Max_Count ) return;
	Monster& m = Monsters[mi];
	if( !m.SpritePtr ) return;

	if( m.Row == postRow && m.Col == postCol ){
		gHephastoWalkHomeTicks = 0;
		gHephastoLastStepOrient = -1;	// arrived: nothing for a later trip to inherit
		if( m.Orientation != DIR_7_DR ) FixMonsterPosition( mi, DIR_7_DR );	// home: face the forge
		return;
	}

	const int GiveUp = 1000;	// ~50s at 20 ticks/s
	if( ++gHephastoWalkHomeTicks > GiveUp ){
		// Safety net: put him on the tile outright rather than leave the mission stuck.
		gHephastoWalkHomeTicks = 0;
		int destRow = postRow, destCol = postCol;
		if( In112(destRow, destCol) && MonsterMap[destRow][destCol]
		 && MonsterMap[destRow][destCol] != mi + 1 ){
			if( !FindNearestFreeCellForMonster(postRow, postCol, destRow, destCol) ){
				destRow = m.Row; destCol = m.Col;	// nowhere at all: stay put
			}
		}
		if( destRow != m.Row || destCol != m.Col ){
			ClearMonsterOnMap( mi );
			if( In112(m.Row, m.Col) && MonsterMap[m.Row][m.Col] == mi + 1 ){
				MonsterMap[m.Row][m.Col] = 0;
			}
			m.Row = m.NextRow = m.PrevRow = destRow;
			m.Col = m.NextCol = m.PrevCol = destCol;
			MonsterMap[destRow][destCol] = mi + 1;
			CheckMonsterUnderArch( mi );
		}
		FixMonsterPosition( mi, DIR_7_DR );
		return;
	}

	if( m.CurAction != A_0_STAY ) return;	// mid-step: let it finish first

	m.TargetRow = (char)postRow;	// char fields, coords are <= 111
	m.TargetCol = (char)postCol;
	if( MakeMonsterPath( mi ) ) return;	// short enough for A*: it handles live obstacles best
	// Refused -- almost always because the post is past FindPath's 25-step cap. Walk the
	// flood fill instead. Only if that is blocked too do we resort to the straight line,
	// which is now just a last-gasp nudge rather than the main mechanism.
	if( DarkCampaignStepHephastoHome( mi ) ) return;
	MonsterWalksTowardsTarget( mi, OrientationToTarget( m.Row, m.Col, postRow, postCol ) );
}


static void DarkCampaignSnapEscortToPost( int ki )
{
	if( ki < 0 || ki >= Monsters_Max_Count ) return;
	Monster& kin = Monsters[ki];
	if( !kin.SpritePtr || !kin.newBossId ) return;
	int postRow = 0, postCol = 0;
	if( !DarkCampaignEscortHubPost( kin.newBossId - 1, postRow, postCol ) ) return;
	if( kin.Row == postRow && kin.Col == postCol ){
		FixMonsterPosition( ki, DIR_0_D );
		return;
	}
	int destRow = postRow, destCol = postCol;
	if( In112(destRow, destCol) && MonsterMap[destRow][destCol]
	 && MonsterMap[destRow][destCol] != ki + 1 ){
		// post taken (the other one overshot onto it): settle for the next best tile
		if( !FindNearestFreeCellForMonster(postRow, postCol, destRow, destCol) ){
			destRow = kin.Row; destCol = kin.Col;	// nowhere at all: stay put
		}
	}
	if( destRow != kin.Row || destCol != kin.Col ){
		ClearMonsterOnMap( ki );
		if( In112(kin.Row, kin.Col) && MonsterMap[kin.Row][kin.Col] == ki + 1 ){
			MonsterMap[kin.Row][kin.Col] = 0;
		}
		kin.Row = kin.NextRow = kin.PrevRow = destRow;
		kin.Col = kin.NextCol = kin.PrevCol = destCol;
		MonsterMap[destRow][destCol] = ki + 1;
		CheckMonsterUnderArch( ki );
	}
	FixMonsterPosition( ki, DIR_0_D );	// stop walking, face south
}

//----- (new by th4) ------------------------------------------------------
// (th4) Claim a cell for a boss and create it there.
//
// The MonsterMap write is inside the In112 guard, not after it. It used to sit outside: the
// guard covered only the displace, so an off-map row/col still wrote MonsterMap[row][col] --
// and the array is int[112][112], so row 113 lands 225 ints past the end of it and quietly
// corrupts whatever follows in the data segment. CreateBoss's marker-tile search could produce
// exactly that (see the note there). That search no longer can, but this is the place the
// damage actually happened, and it should not depend on every caller being careful.
//
// An off-map cell now simply goes unclaimed. The monster is still created -- RepairMonsterMap
// skips anything failing its own In112 test, so it degrades to an unregistered monster rather
// than to memory corruption.
static void __fastcall ClaimCellForBoss(int monsterIndex, int spriteIndex, int row, int col, const char* what)
{
	#if TH4_MONSTERMAP_DEBUG
	if( In112(row, col) && MonsterMap[row][col] ) StompLog(what, monsterIndex, row, col, MonsterMap[row][col]);
	#else
	(void)what;
	#endif
	if( In112(row, col) ){
		// (th4) don't stack: move whoever is already here out of the way first
		if( MonsterMap[row][col] > 0 ){
			DisplaceMonsterFromCell(MonsterMap[row][col] - 1, row, col);
		}
		MonsterMap[ row ][ col ] = monsterIndex + 1;
	}
	CreateMonster(monsterIndex, RNG(8), spriteIndex, row, col);
}

//----- (0043BFD5) --------------------------------------------------------
void __fastcall CreateBossBaseMonster(int monsterIndex, int spriteIndex, int row, int col)
{
	if( MonsterSprites[spriteIndex].baseMonsterIndex != BM_137_UBER_DIABLO || MonstersCount <= 0 ){
		ClaimCellForBoss(monsterIndex, spriteIndex, row, col, "CreateBossBaseMonster");
		return;
	}
	// (th4) Uber Diablo only: walk the live monsters looking for one that already carries this
	// sprite, or is already an Uber Diablo, and place him only if there is none.
	//
	// The decompiled loop had no break. Once ++i passed MonstersCount it created the monster and
	// carried on looping, and what stopped it was incidental: CreateMonster writes SpriteIndex
	// into Monsters[monsterIndex], so the next pass over that same slot failed the condition and
	// fell out. That works only while the loop index and the created index line up.
	//
	// They do not always. Two callers pass MonstersCount, and for those the created slot IS the
	// one the loop lands on. AddQuestMonsters passes MonstersCount++ (4941), so MonstersCount is
	// already one ahead: the loop creates at Monsters[N] but then goes on to inspect N+1, N+2...
	// none of which the creation touched. It creates again on every pass and walks Monsters[]
	// off the end, dereferencing SpritePtr the whole way, until some stale slot happens to match.
	//
	// A plain bounded scan does the same job with none of that: check the live monsters, place
	// him once if none matches.
	for( int i = 0; i < MonstersCount && i < Monsters_Max_Count; ++i ){
		const Monster& m = Monsters[i];
		if( m.SpriteIndex == spriteIndex ) return;								// already placed
		if( m.SpritePtr && m.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO ) return;
	}
	ClaimCellForBoss(monsterIndex, spriteIndex, row, col, "CreateBossBaseMonster/uber");
}

//----- (0043C057) --------------------------------------------------------
void PlaceQuestMonsters()
{
	//int v0; // esi@7
	//MonsterSprite *fn; // edi@8
	void *ogdens_sign_dun; // esi@54
	void *valor_dun; // esi@15
	void* valorMP_dun;
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
	if (IsQuestOnLevel(Q_39_VALOR_MP)) {
		valorMP_dun = LoadFile("Levels\\L2Data\\blood_MP_Ntree.DUN");
		AddQuestMonsters((ushort*)valorMP_dun, 2 * CurPattern.row, 2 * CurPattern.col);
		FreeMem(valorMP_dun);
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
			// (rt) line-of-sight check, so part of a boss pack doesn't end up in another (CheckCastInSight variant)
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

//----- (th4) -------------------------------------------------------------

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
		AddMonsterTypeToDungeon(BM_392_WARLORD_MERC, MPF_2_DONT);
     
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

	if (GameMode == GM_OUTBREAK && Dungeon == DUN_1_CHURCH_1) {
		auto ob_Monsters = {
					BM_0_BURNED_CORPSE, BM_147_FIEND, BM_5_FALLEN_ONE, //wave 1
					BM_9_SKELETON_WARRIOR, BM_139_BONE_GASHER, BM_281_FALLEN_BOW, // wave 2
					BM_140_GHOUL, BM_126_PLAGUE_EATER, BM_189_CARVER, // wave 3-4
					BM_30_SHADOW, BM_33_MOON_CLAN, BM_42_NIGHT_CLAN, // wave 5-7
					BM_56_PIT_DEMON, BM_76_DEATH_CLAW, BM_18_VORTEX_ROGUE, // wave 8-10
					BM_90_CAVE_SNAKE, BM_106_CORRUPT_ORDER, BM_156_IMP_ROGUE, // wave 11-13
					BM_284_SIN_SPAWN, BM_122_BLOOD_SPECTRE, BM_299_TENTACLIST2, // wave 14-16
					BM_278_LEAPER, BM_135_GROTESQUE, BM_260_ARMAGEDDON_WALKER, // wave 17-20
					BM_234_STEEL_LORD, BM_85_BALOR, BM_73_DEATH_ANGEL // wave 21-24
		};

		for (int i : ob_Monsters) {
			AddMonsterTypeToDungeon(i, MPF_2_DONT);
		}
	}

	// quest dungeons
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
		else if (Dungeon == DUN_47_DARK_NEST && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DARK CAMPAIGN mission 9: the hero expedition in the Dark Nest. Grimjaw does
			// not spawn in DC (see the boss switch below) -- this level gets its own packs.
			BossPack dun47BossesDC[] = {
				{ UM_922_DC_Donna_Paloosa,    BM_H_406_SCOUT,     9, UN_1_PACK },
				{ UM_923_DC_Abbot_Hirosh,     BM_H_406_SCOUT,    10, UN_1_PACK },
				{ UM_924_DC_Commander_Latham, BM_H_408_WAR_M_SS, 14, UN_1_PACK },
			};
			for (auto& i : dun47BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_118_POISONED_WATER_SUPPLY_DC && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) The escort's monster TYPE is registered UNCONDITIONALLY, before anything
			// else on this level, and must stay that way. Monster::SpriteIndex is an index
			// into MonsterSprites[], and that table is rebuilt from scratch every time the
			// level is generated -- including on load, BEFORE the saved monsters are read
			// back over the generated ones. DarkCampaignEscortActive() goes false the moment
			// mission 10 is marked complete, so a save taken after clearing the level used to
			// regenerate WITHOUT the succubus type: every type registered after it shifted
			// down one slot, and the saved succubi came back wearing the next sprite in the
			// table -- the hero monk from dun118BossesDC below. Registering the type in both
			// states keeps the table identical; only the CreateBoss part is conditional.
			const int escorts[] = { UM_5_Red_Vex, UM_6_Dark_Jade };
			for (int e : escorts) {
				AddMonsterTypeToDungeon(UniqueMonsters[e].BaseMonsterIndex, MPF_4_UNIQ);
			}
			// (th4) DC mission 10: the Dark Lord's two succubi come along. Spawn them as real
			// uniques (keeps their names/TRNs) and flag them friendly so they fight the heroes
			// and the heroes fight them. CreateBoss appends at MonstersCount-1, which is how we
			// get hold of the index to flag. Placement comes from the switch in CreateBoss.
			if (DarkCampaignEscortActive()) {
				for (int e : escorts) {
					AddBossTypeToQuestLoc(e);
					if (MonstersCount > 0) {
						Monster& succubus = Monsters[MonstersCount - 1];
						if (succubus.newBossId - 1 == e) {
							succubus.flag |= MF_6_FRIENDLY;
							succubus.ActivationCounter = 1;	// awake, so the AI runs for them
						}
					}
				}
			}
			// (th4) The hero defenders holding the water supply.
			BossPack dun118BossesDC[] = {
				{ UM_925_DC_Abbot_Balatt, BM_H_409_MONK_H_T, 19, UN_1_PACK },
			};
			for (auto& i : dun118BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_121_UNHOLY_ALTAR_DC && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 13: three friendlies. Tiles come from the CreateBoss switch
			// above; MF_6_FRIENDLY and the facing are set here because CreateBoss does not
			// take either. NW/SE are read as the isometric screen directions UL and DR.
			struct { int uniq; int orient; } dun121Friendly[] = {
				{ UM_5_Red_Vex,              DIR_3_UL },	// 39, 44
				{ UM_6_Dark_Jade,            DIR_3_UL },	// 39, 49
				{ UM_4_ArchBishop_Lazarus,   DIR_7_DR },	// 32, 46
			};
			// (th4) Reset the mission 13 talk latches on every generation of this level.
			// They are plain globals: nothing clears them between runs, so a Thanked left
			// true by an earlier attempt made the completion gate pass the moment the last
			// hero fell -- the portal appearing without Lazarus ever being spoken to.
			// Safe to clear unconditionally: once the talk has really happened the mission
			// is DCM_2_COMPLETE, which is saved, and the gate accepts that on its own.
			DarkCampaignLazarusTalkWanted = false;
			DarkCampaignLazarusRewardPending = false;
			DarkCampaignLazarusThanked = false;

			// (th4) Lazarus' own line, SP_35 ("Abandon your foolish quest..." / S_960_M_LAZ01).
			// He no longer says it on click, so it is delivered once as the player walks in.
			StartSpeech( SP_35 );

			// (th4) DC mission 13: the hero strike force sent to storm the lair.
			BossPack dun121BossesDC[] = {
				{ UM_931_DC_Madame_Sarbine, BM_H_411_ROG_H_B, 14, UN_1_PACK },
			};
			for (auto& i : dun121BossesDC) {
				AddBossPack(i);
			}

			for (auto& f : dun121Friendly) {
				AddMonsterTypeToDungeon(UniqueMonsters[f.uniq].BaseMonsterIndex, MPF_4_UNIQ);
				int firstIndex = MonstersCount;
				CreateBoss(f.uniq, 0, 0);
				for (int i = firstIndex; i < MonstersCount; ++i) {
					Monster& m = Monsters[i];
					if (m.newBossId && m.newBossId - 1 == f.uniq) {
						m.flag |= MF_6_FRIENDLY;
						m.ActivationCounter = 1;	// awake, so the AI runs for them
						m.goal = MG_1_ATTACK;
						m.speechIndex = 0;
						m.Orientation = f.orient;
						// (th4) AI_28_Lazarus is a bespoke AI that walks straight at the player,
						// never consults the generic target picker (the one that honours
						// MF_6_FRIENDLY), and on speech end runs the vanilla lair script. So he
						// cannot keep it here. AI_25_Advocate (whose handler is Ai_25_Mage) is what
						// his own AI delegates to for combat anyway -- it reads monster.intel to
						// pick the missile, and his
						// UniqueMonsters row already sets intel 3, which is MonsterMissiles[3] =
						// MI_6_FIREBLAST. It is also the AI that calls Monster_Fade_In/Out, so
						// the phase animation comes back with it. DC-only: everywhere else he is
						// still the hostile Arch-Bishop with his own AI.
						if (f.uniq == UM_4_ArchBishop_Lazarus) m.ai = AI_25_Advocate;	// -> Ai_25_Mage
					}
				}
			}
		}
		else if (Dungeon == DUN_119_DEEPER_CAVES && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 11: the squad hauling the Anvil of Fury out of the caves.
			// Black Hawk leads; his escort is a rogue pack, not his own base type.
			BossPack dun119BossesDC[] = {
				{ UM_926_DC_Black_Hawk, BM_H_411_ROG_H_B, 17, UN_1_PACK },
			};
			for (auto& i : dun119BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_51_FETID_CAVERN && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 16: the troops sent into the cavern.
			BossPack dun51BossesDC[] = {
				{ UM_936_DC_Ludwig_the_Paranoid, BM_H_414_WAR_H_MS, 24, UN_3_LEAD },
				{ UM_937_DC_Lord_Voss,           BM_H_411_ROG_H_B,  13, UN_1_PACK },
			};
			for (auto& i : dun51BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_120_PANDEMONIUM_DC && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 12: the new general's squad, waiting at the four corners.
			// Darkmoor, Moonrose and Carnabus each lead their own base type; Degroe leads
			// rogues rather than Executioners, the way Black Hawk did in mission 11.
			BossPack dun120BossesDC[] = {
				{ UM_928_DC_Darkmoor,          BM_H_410_MONK_H_SS,    29, UN_1_PACK },
				{ UM_929_DC_Moonrose,          BM_H_411_ROG_H_B,      29, UN_1_PACK },
				{ UM_930_DC_Commander_Degroe,  BM_H_411_ROG_H_B,      34, UN_1_PACK },
				{ UM_927_DC_General_Carnabus,  BM_H_412_WAR_H_X_EXEC, 49, UN_1_PACK },
			};
			for (auto& i : dun120BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_54_ABANDONED_FANE) {
			auto dun54Bosses = { UM_717_Priestess_Varna, UM_713_Sir_Valon, UM_714_Sir_Darion, UM_715_Bishop_Zenda, UM_716_Bishop_Zamal };
			for (int i : dun54Bosses) {
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				CreateBoss(i, 0, 0);
			}
		}
		else if (Dungeon == DUN_60_HAUNTED_LABYRINTH && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 24. Don Carnage and Gloom Prowler below are NOT spawned here --
			// this branch replaces them rather than adding to them.
			BossPack dun60BossesDC[] = {
				{ UM_947_DC_High_Exemplar_Kastor, BM_H_417_WAR_H_SS_WARRIOR_ELITE, 14, UN_1_PACK },
			};
			for (auto& i : dun60BossesDC) {
				AddBossPack(i);
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
		else if (Dungeon == DUN_62_DIM_UNDERCROFT && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 23. Undertaker and Dustwing below are NOT spawned here --
			// this branch replaces them rather than adding to them.
			BossPack dun62BossesDC[] = {
				{ UM_946_DC_Cleric_Dwaynor, BM_H_418_SOR_H_T_SORC_ELITE, 9, UN_3_LEAD },
			};
			for (auto& i : dun62BossesDC) {
				AddBossPack(i);
			}
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
			// (th4) DARK CAMPAIGN runs its own scenario here: Az'Dog alone. None of the vanilla
			// Mausoleum bosses belong to it, and each would keep MonstersCount above the clear
			// threshold. CreateBoss(i, 0, 0) is the no-pack form -- packCount 0 and Az'Dog's
			// PackTrigger is 0, so nothing is summoned around him.
			auto dun64BossesDC = { UM_910_DC_AZDOG };
			auto dun64Bosses   = { UM_239_Plaguefist_Blackheart, UM_220_Blackwillow, UM_34_Pukerat_the_Unclean };
			if (GameMode == GM_DARK_CAMPAIGN) {
				for (int i : dun64BossesDC) {
					AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
					CreateBoss(i, 0, 0);
				}
			}
			else {
				for (int i : dun64Bosses) {
					AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
					CreateBoss(i, 0, 0);
				}
			}
		}
		else if (Dungeon == DUN_65_RUINED_TEMPLE && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 22. Kobal and Shadowcrow are NOT spawned here -- this branch
			// replaces them rather than adding to them.
			BossPack dun65BossesDC[] = {
				{ UM_943_DC_Paladin_Orphius,    BM_H_419_ROG_H_B_SS_ELITE,   19, UN_1_PACK },
				{ UM_944_DC_Knight_Grimald,     BM_H_418_SOR_H_T_SORC_ELITE, 19, UN_1_PACK },
				{ UM_945_DC_Prophetess_Lyssia,  BM_H_419_ROG_H_B_SS_ELITE,   19, UN_1_PACK },
			};
			for (auto& i : dun65BossesDC) {
				AddBossPack(i);
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
			// (th4) DARK CAMPAIGN mission 7: the cornered knight squad. Paladin Loxley (placement
			// 36,36) leads a company of 39 Templars (BM_H_404_WAR_M_MS); Priest Gwideon (placement
			// 65,63) leads 7 of his own kin (BM_H_405_SOR_M_T_HYBRID) with UN_3_LEAD so they cling
			// to him. The three regular Sewers bosses do not spawn in DC -- this is a hero last
			// stand, not the usual dungeon.
			BossPack dun66BossesDC[] = {
				{ UM_916_DC_PALADIN_LOXLEY,  BM_H_404_WAR_M_MS,       39, UN_1_PACK },
				{ UM_917_DC_PRIEST_GWIDEON,  BM_H_405_SOR_M_T_HYBRID,  7, UN_3_LEAD },
			};
			auto dun66Bosses = { UM_380_Duskstone, UM_237_Meatchop_the_Flayer, UM_368_Goredrool };
			if (GameMode == GM_DARK_CAMPAIGN) {
				for (auto& i : dun66BossesDC) {
					AddBossPack(i);
				}
			}
			else {
				for (int i : dun66Bosses) {
					AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
					CreateBoss(i, 0, 0);
				}
			}
		}
		else if (Dungeon == DUN_67_SEPULCHER && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 21. Deathpuke and Viletouch below are NOT spawned here --
			// this branch replaces them rather than adding to them.
			BossPack dun67BossesDC[] = {
				{ UM_942_DC_Venturius_the_Inquisitor, BM_H_418_SOR_H_T_SORC_ELITE, 29, UN_3_LEAD },
			};
			for (auto& i : dun67BossesDC) {
				AddBossPack(i);
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
		else if (Dungeon == DUN_69_GIBBET && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 20: the barracks commander.
			BossPack dun69BossesDC[] = {
				{ UM_941_DC_Lord_Tenabris, BM_H_415_ROG_H_B_ELITE_ARCHER, 24, UN_1_PACK },
			};
			for (auto& i : dun69BossesDC) {
				AddBossPack(i);
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
			// (th4) DARK CAMPAIGN: the war council. Four captains, one per quadrant, each
			// leading a company of a DIFFERENT troop type than his own -- so the pack base
			// deliberately does not match the boss's. UN_1_PACK keeps each mob on its own
			// ai/intF (it is also the default when packFlag is 0 and PackTrigger is 0).
			BossPack dun70BossesDC[] = {
				{ UM_911_DC_COMMANDER_ALDRIC,  BM_H_400_SORC_M_T,  19, UN_1_PACK },
				{ UM_912_DC_CAPTAIN_ILSE,      BM_H_402_BERS_M_X,  19, UN_1_PACK },
				{ UM_913_DC_MAGISTER_CORVANE,  BM_H_401_MONK_M_MS, 19, UN_1_PACK },
				{ UM_914_DC_BROTHER_SILAS,     BM_H_403_ROG_M_B,   19, UN_1_PACK },
			};
			auto dun70Bosses = { UM_748_Fangscar, UM_440_Painwraps, UM_377_Blazeshock };
			if (GameMode == GM_DARK_CAMPAIGN) {
				for (auto& i : dun70BossesDC) {
					AddBossPack(i);
				}
			}
			else {
				for (int i : dun70Bosses) {
					AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
					CreateBoss(i, 0, 0);
				}
			}
		}
		else if (Dungeon == DUN_71_GRIM_VAULTS) {
			auto dun71Bosses = { UM_143_Bloodscythe_Gloomslayer, UM_652_Griefzap, UM_322_Mindgloom };
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
			if (GameMode == GM_DARK_CAMPAIGN) {	// (th4) DC mission 19: added on top of the level's own bosses, not instead of them
				BossPack dun72BossesDC[] = {
					{ UM_644, BM_242_REANIMATED_HORROR, 19, UN_1_PACK },	// his own base: keeps the table's minions, PackSpecials and AI
					{ UM_752_Deathcaller, BM_23_MALICE, 19, UN_1_PACK },
					{ UM_583_Lich_King, BM_23_MALICE, 29, UN_1_PACK },
				};
				for (auto& i : dun72BossesDC) {
					AddBossPack(i);
				}
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
		else if (Dungeon == DUN_80_HELL_GATE && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 18: the human resistance squad holding the gate.
			BossPack dun80BossesDC[] = {
				{ UM_940_DC_Darius_the_Ronin, BM_H_416_MON_H_MS, 34, UN_1_PACK },
			};
			for (auto& i : dun80BossesDC) {
				AddBossPack(i);
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
				// (th4) DC mission 14 reuses this level with its own roster, so the Alternative
				// Route bosses are not created here. The monster TYPE is still registered either
				// way: MonsterSprites[] is rebuilt on every generation INCLUDING on load, before
				// the saved monsters are read back, so a table that differed between modes would
				// shift every later sprite index and bring saved monsters back wearing the wrong
				// graphics -- exactly the bug that turned the hub succubi into Salinus.
				AddMonsterTypeToDungeon(UniqueMonsters[i].BaseMonsterIndex, MPF_4_UNIQ);
				if (GameMode != GM_DARK_CAMPAIGN) {
					CreateBoss(i, 0, 0);
				}
			}
			if (GameMode == GM_DARK_CAMPAIGN) {
				// (th4) DC mission 14: the three arrogant Sorcerers. Malachar leads
				// Executioners; the other two lead their own kind, and Faldir's stick to him
				// and take over his attack (UN_3_LEAD = UN_1_PACK | UN_2_STICK).
				BossPack dun83BossesDC[] = {
					{ UM_932_DC_Malachar_the_Warden, BM_H_412_WAR_H_X_EXEC, 14, UN_1_PACK },
					{ UM_933_DC_Zerath_the_Sage,     BM_H_413_SOR_H_T,      19, UN_1_PACK },
					{ UM_934_DC_Faldir_the_Keeper,   BM_H_413_SOR_H_T,      19, UN_3_LEAD },
				};
				for (auto& i : dun83BossesDC) {
					AddBossPack(i);
				}
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
			if (GameMode == GM_DARK_CAMPAIGN) {
				// (th4) DARK CAMPAIGN mission 8: the regular Ancient Crypts bosses (see the else
				// branch) do NOT spawn -- this is a hero-storming mission, not the usual dungeon.
				// DC-only boss packs (Loxley/Gwideon pattern: rows here, placement cases in the
				// switch above, the loop spawns them).
				BossPack dun99BossesDC[] = {
					{ UM_918_DC_MADAME_PERESIDE, BM_H_406_SCOUT, 15, UN_1_PACK },
					{ UM_919_DC_Archimandrite_Bozu, BM_H_407_MONK_M_X, 15, UN_1_PACK },
					{ UM_920_DC_Bishop_Lucian, BM_H_405_SOR_M_T_HYBRID, 15, UN_1_PACK },
					{ UM_921_DC_Deacon_Raduel, BM_H_404_WAR_M_MS, 14, UN_1_PACK },
				};
				for (auto& i : dun99BossesDC) {
					AddBossPack(i);
				}
			}
			else {
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

		else if (Dungeon == DUN_117_DARKLORD_HIDEOUT) { // (th4) Dark Campaign hub: the Dark Lord and his vendor kin, no packs
			// (th4) If the player just came back from mission 10, the two succubi return with
			// him rather than being found at their posts. Latch it before the packs are built,
			// because the placement switch in CreateBoss reads this to decide where to put them.
			DarkCampaignEscortWalkingHome =
				Dungeon.prev == DUN_118_POISONED_WATER_SUPPLY_DC
				&& DarkMissionState[9] == DCM_2_COMPLETE;
			DarkCampaignEscortNeedsArrival = DarkCampaignEscortWalkingHome;
			BossPack dun117Bosses[] = {
				{ UM_773_The_Dark_Lord,             BM_240_DEMON_PRIEST,      0, 1 },
				{ UM_0_Gharbad_the_Weak,            BM_173_GHARBAD_BASEMNSTR, 0, 1 },	// hub blacksmith
				{ UM_19_Jaldabaoth,                 BM_136_BLOOD_SLAYER,      0, 1 },	// hub powerups
				{ UM_30_Boilrot,                    BM_2_ROTTING_CARCASS,     0, 1 },	// hub stash
				{ UM_48_Molianta_the_Copperwing,    BM_82_GARGOYLE,           0, 1 },	// hub quench
				{ UM_51_Galahad_the_Bonewhite,      BM_101_DARK_MISTRESS,     0, 1 },	// hub crafting
				{ UM_89_Alystar,                    BM_107_DARK_MAGE,         0, 1 },	// hub identify
				{ UM_94_Setebos,                    BM_125_GOLD_GOLEM,        0, 1 },	// hub enchant
				{ UM_124_Torvatus,                  BM_98_IMMORTAL,           0, 1 },	// hub witch trade
				{ UM_126_Gloomshredder,             BM_32_INVISIBLE_ONE,      0, 1 },	// hub transmute
				{ UM_792_Salinus_the_Salt_King,     BM_286_BRINE_CORPSE,      0, 1 },		// hub healer trade
				{ UM_6_Dark_Jade,                   BM_103_BLOOD_WITCH,       0, 1 },	// entourage, no service
				{ UM_5_Red_Vex,                     BM_103_BLOOD_WITCH,       0, 1 }		// entourage, no service
			};
			for (auto& i : dun117Bosses) {
				if( DarkCampaignEscortAwayAtLazarus() && DarkCampaignIsEscortBoss( i.uniq ) ){
					// (th4) DC mission 13: they are away at Lazarus' lair, so do NOT create
					// them here -- but DO register their monster TYPE anyway. This is the
					// Salinus bug: simply skipping AddBossPack left BM_103_BLOOD_WITCH out of
					// MonsterSprites[], every type registered after it shifted down a slot,
					// and saved hub monsters came back wearing the wrong sprite. The two
					// AddMonsterTypeToDungeon calls below are exactly the pair AddBossPack
					// makes before CreateBoss, so the table stays byte-identical either way.
					const UniqueMonster& um = UniqueMonsters[i.uniq];
					AddMonsterTypeToDungeon( um.PackSpecials > 0 ? um.PackSpecials : um.BaseMonsterIndex, MPF_4_UNIQ );
					AddMonsterTypeToDungeon( i.base, MPF_2_DONT );
					continue;
				}
				AddBossPack(i);
			}

			// (th4) DC mission 25: once freed, the Butcher comes home with the player and
			// stays with him for the rest of the campaign.
			//
			// His TYPE is registered unconditionally, and AFTER the vendor kin -- this is the
			// Salinus bug again. MonsterSprites[] is built in call order and saved hub
			// monsters store a sprite INDEX into it, so registering him only on the runs
			// where he is present, or registering him ahead of the vendors, makes the table
			// differ between visits. Every index then points a slot off and the whole Sanctum
			// comes back wearing the wrong sprites -- the Dark Lord as a Butcher, the succubi
			// as Hidden. Appending is also safe for hubs saved before he existed.
			AddMonsterTypeToDungeon(UniqueMonsters[UM_9_The_Butcher].BaseMonsterIndex, MPF_4_UNIQ);
			if( DarkCampaignButcherJoined() ){
				CreateBoss(UM_9_The_Butcher, 0, 0);
				DarkCampaignButcherNeedsArrival = true;
				for( int i = 0; i < MonstersCount; ++i ){
					Monster& m = Monsters[ MonsterIndexes[i] ];
					if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
					m.flag |= MF_6_FRIENDLY;
					m.flag &= ~MF_9_CHASE_TARGET;
					m.speechIndex = 0;
					m.ActivationCounter = 255;
				}
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

		else if (Dungeon == DUN_44_RAVENHOLM && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 26, the last one.
			BossPack dun44BossesDC[] = {
				{ UM_949_DC_Paladin_Godfrey,          BM_H_422_WAR_H_ROYAL_GUARD,    24, UN_1_PACK },
				{ UM_950_DC_Captain_Dorian,           BM_H_423_ROG_H_B_BLACK_ARCHER, 24, UN_1_PACK },
				{ UM_951_DC_Grand_Justicar_Rosalind,  BM_H_423_ROG_H_B_BLACK_ARCHER, 24, UN_1_PACK },
				{ UM_952_DC_Elara_Longshot,           BM_H_422_WAR_H_ROYAL_GUARD,    24, UN_1_PACK },
				{ UM_953_DC_Grandmaster_Ortus,        BM_H_418_SOR_H_T_SORC_ELITE,   24, UN_1_PACK },
				{ UM_905_DC_Griswold,                 BM_215_CADAVER,                44, UN_3_LEAD },
			};
			for (auto& i : dun44BossesDC) {
				AddBossPack(i);
			}

			// The Butcher comes with the player if he was freed in mission 25 -- his type is
			// registered either way, so the sprite table is the same on every visit and saved
			// monsters keep their indices.
			AddMonsterTypeToDungeon(UniqueMonsters[UM_9_The_Butcher].BaseMonsterIndex, MPF_4_UNIQ);
			if( DarkCampaignButcherJoined() ){
				CreateBoss(UM_9_The_Butcher, 0, 0);
				DarkCampaignButcherNeedsArrival = true;
				for( int i = 0; i < MonstersCount; ++i ){
					Monster& m = Monsters[ MonsterIndexes[i] ];
					if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
					m.flag |= MF_6_FRIENDLY;
					m.flag &= ~MF_9_CHASE_TARGET;
					m.speechIndex = 0;
					m.ActivationCounter = 255;
					m.BaseLife += 5'800 << 6;	// same boosts as his cell in the Sacristy
					m.CurrentLife = m.BaseLife;
					m.MinDamage += 92;
					m.MaxDamage += 186;
					m.ArmorClass += 103;
					m.MonsterLevel += 50;
					m.ToHit += 120;
				}
			}
		}
		else if (Dungeon == DUN_111_CHURCH_SACRISTY && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 25. Festergnaw below is NOT spawned here -- this branch
			// replaces him. The heroes hold the Butcher in a cell: he is the vanilla
			// UM_9_The_Butcher, spawned friendly and parked on his tile until the player
			// opens the room and speaks to him.
			AddMonsterTypeToDungeon(UniqueMonsters[UM_9_The_Butcher].BaseMonsterIndex, MPF_4_UNIQ);
			CreateBoss(UM_9_The_Butcher, 0, 0);

			BossPack dun111BossesDC[] = {
				{ UM_948_DC_Warden_Gaspax, BM_H_421_WAR_H_X_EXEC_ELITE, 24, UN_1_PACK },
			};
			for (auto& i : dun111BossesDC) {
				AddBossPack(i);
			}

			// Transient click latches only. The freed state itself lives in quest data and
			// is deliberately NOT cleared here -- it has to outlive both this generation and
			// the savegame.
			DarkCampaignButcherTalkWanted = false;
			DarkCampaignButcherJoinPending = false;
			for( int i = 0; i < MonstersCount; ++i ){
				Monster& m = Monsters[ MonsterIndexes[i] ];
				if( !m.newBossId || m.newBossId - 1 != UM_9_The_Butcher ) continue;
				m.flag |= MF_6_FRIENDLY;        // never targets the player, see IsPlayerSideMonster
				m.flag &= ~MF_9_CHASE_TARGET;   // ...and MonsterChangeTarget cannot walk him out
				m.speechIndex = 0;              // no line on approach -- the talk is scripted
				m.ActivationCounter = 255;      // awake, so he can answer a hero that attacks him
				// Orientation AND curAnimation as a pair, exactly as CreateMonster does --
				// setting the heading alone leaves the sprite drawing its old one.
				m.Orientation = DIR_7_DR;       // facing down-right, in his cell
				m.curAnimation = &m.SpritePtr->animation[ANIM_0_STAY].oriented[DIR_7_DR];
				// (th4) DC only: he fights on the player's side, so he is boosted well past
				// the quest boss. Life is stored as HP << 6, so the raw figure is shifted.
				m.BaseLife += 5'800 << 6;
				m.CurrentLife = m.BaseLife;
				m.MinDamage += 92;
				m.MaxDamage += 186;
				m.ArmorClass += 103;
				m.MonsterLevel += 50;
				m.ToHit += 120;
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
			// (th4) DARK CAMPAIGN runs its own scenario here: the four vanilla Still Springs
			// bosses are replaced by Deacon Zypherys, who takes Stormire's spot (30,74) and
			// leads a pack of Savages instead of Storm Demons.
			BossPack dun112BossesDC[] = {
				{ UM_907_DC_ZYPHERYS, BM_H_398_WAR_L_X, 7, 1 }
			};
			BossPack dun112Bosses[] = {
				{ UM_805_Festerus, BM_199_HELL_CLAN, 15, 1 },
				{ UM_806_Obfuscus, BM_288_EARTH_GOLEM, 13, 3 },
				{ UM_807_Stormire, BM_77_STORM_DEMON, 6, 3 },
				{ UM_808_Winggore, BM_82_GARGOYLE, 11, 3 }
			};
			if (GameMode == GM_DARK_CAMPAIGN) {
				for (auto& i : dun112BossesDC) {
					AddBossPack(i);
				}
			}
			else {
				for (auto& i : dun112Bosses) {
					AddBossPack(i);
				}
			}
		}
		else if (Dungeon == DUN_113_THE_MAZE) {
			BossPack dun113Bosses[] = {
				{ UM_809_Borosteron, BM_233_BLOOD_LORD, 10 - Difficulty / 2, 3 },
			};
			for (auto& i : dun113Bosses) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_114_LOWER_SCRIPTORIUM) {
			// (th4) DARK CAMPAIGN runs its own scenario here -- Snotspill and his pack belong to
			// the vanilla Ogden's Sign quest and have no place in it. Pack flag UN_1_PACK keeps
			// each mob on its OWN ai/intF; UN_3_LEAD would make them inherit the boss's.
			BossPack dun114BossesDC[] = {
				{ UM_908_DC_LORD_KINDRETT, BM_H_397_ROG_L_B,  14, UN_1_PACK },
				{ UM_909_DC_SCRIBE_VABIUS, BM_H_399_WAR_L_SS, 21, UN_1_PACK },
			};
			BossPack dun114Bosses[] = {
				{ UM_821_Snotspill_MP, BM_5_FALLEN_ONE, 19, 3 },
			};
			if (GameMode == GM_DARK_CAMPAIGN) {
				for (auto& i : dun114BossesDC) {
					AddBossPack(i);
				}
			}
			else {
				for (auto& i : dun114Bosses) {
					AddBossPack(i);
				}
			}
		}
		else if (Dungeon == DUN_116_MINING_CAVES && GameMode == GM_DARK_CAMPAIGN) {
			// (th4) DC mission 17: the elite archer squad, on the two Azgor/Dobazzu spots.
			BossPack dun116BossesDC[] = {
				{ UM_938_DC_Lord_Tiberius, BM_H_414_WAR_H_MS,             24, UN_1_PACK },
				{ UM_939_DC_Ravenshot,     BM_H_415_ROG_H_B_ELITE_ARCHER, 24, UN_1_PACK },
			};
			for (auto& i : dun116BossesDC) {
				AddBossPack(i);
			}
		}
		else if (Dungeon == DUN_116_MINING_CAVES) {
			BossPack dun116Bosses[] = {
				{ UM_903_AZGOR, BM_3_FAMILIAR, 7, 1 },
				{ UM_904_DOBAZZU, BM_3_FAMILIAR, 7, 1 },
			};
			for (auto& i : dun116Bosses) {
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
		}
		// (th4) DARK CAMPAIGN mission 6: the one who came to wear Arkaine's Valor. Sir Cabbot
		// leads a company of 14 Sharpshooters (BM_H_403_ROG_M_B). Placement is set in CreateBoss
		// (row 49, col 46). Normal Valor play / Valor Tomb are untouched -- they keep the
		// Apparition of Arkaine via the generic switch below.
		else if( Dungeon == DUN_59_HALL_OF_FIRE && GameMode == GM_DARK_CAMPAIGN ){
			BossPack dun59BossDC = { UM_915_DC_SIR_CABBOT, BM_H_403_ROG_M_B, 14, UN_1_PACK };
			AddBossPack( dun59BossDC );
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
				if (Difficulty >= DL_1_PURGATORY && GameMode != GM_CLASSIC && GameMode != GM_DARK_CAMPAIGN) {
					boss = UM_675_PWS_Boss;
				}
				else if (GameMode == GM_DARK_CAMPAIGN){ 
					boss = UM_906_DC_LORD_ANTONIUS; 
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
			case DUN_47_DARK_NEST: if (GameMode != GM_DARK_CAMPAIGN) { boss = UM_702_Grimjaw; } break;	// (th4) DC mission 9 uses its own boss packs
			case DUN_48_BLOOD_HIVE: boss = UM_703_Hive_Mother; break;
			case DUN_49_SICK_VILLAGERS_BASEMENT: boss = UM_704_Borje_the_Ogre; break;
			case DUN_50_TEMPLE_OF_SUNSET: boss = UM_705_Bloodhoof_Twinaxe; break;
			case DUN_51_FETID_CAVERN: if (GameMode != GM_DARK_CAMPAIGN) { boss = UM_706_Mudsnout; } break;	// (th4) DC mission 16 gets its own boss
			case DUN_52_MOSSY_GROTTO: boss = UM_707_Lichen_Axe_the_Fungal; break;
			case DUN_53_VAULTKEEP: boss = UM_708_Grimwing; break;
			case DUN_35_PASSAGE_OF_FIRE: boss = UM_718_Guardian_of_Flames; break;
			case DUN_55_DEEP_HOLLOW: boss = UM_723_The_Infester; break;
			case DUN_56_WEB_TUNNELS: boss = UM_724_Silkstrider; break;
			case DUN_57_ARACHNID_LAIR: boss = UM_725_Web_Queen; break;
			case DUN_59_HALL_OF_FIRE or DUN_115_VALOR_TOMB: boss = UM_726_Apparition_of_Arkaine; break;
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

	// (th4) DC mission 15: Hephasto is at his forge, not fighting. This must run AFTER the
	// whole placement chain above, NOT as another branch inside it -- that chain ends in the
	// else that holds the generic boss switch, so a DUN_43 branch of its own would claim the
	// dungeon and the switch would never create him at all.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO ){
		// (th4) DC mission 15: the hero strike force sent against the forge -- the armored
		// knights the briefing warns about. Created BEFORE the loop below, which walks the
		// living-monster list looking for Hephasto; adding to that list mid-scan would be
		// asking for trouble.
		BossPack dun43BossesDC[] = {
			{ UM_935_DC_Captain_Nagvot, BM_H_414_WAR_H_MS, 7, UN_1_PACK },
		};
		for (auto& i : dun43BossesDC) {
			AddBossPack(i);
		}

		// Reset the talk latches on every generation of this level: they are plain globals,
		// and a Thanked left true by an earlier attempt would open the way home the moment
		// the last hero fell. Safe to clear -- a real talk leaves the mission COMPLETE,
		// which is saved, and the gate accepts that on its own.
		DarkCampaignHephastoTalkWanted = false;
		DarkCampaignHephastoRewardPending = false;
		DarkCampaignHephastoThanked = false;
		DarkCampaignResetHephastoWalkHome();
		for( int i = 0; i < MonstersCount; ++i ){
			Monster& m = Monsters[ MonsterIndexes[i] ];
			if( !m.newBossId || m.newBossId - 1 != UM_587_Hephasto_the_Armorer ) continue;
			m.flag |= MF_6_FRIENDLY;        // never targets the player, and see IsPlayerSideMonster
			m.flag &= ~MF_9_CHASE_TARGET;   // ...and MonsterChangeTarget cannot walk him off
			m.speechIndex = 0;              // no line on approach
			m.ActivationCounter = 255;      // awake, so he can answer a hero that attacks him
			m.Orientation = DIR_7_DR;       // facing down-right, at the forge
		}
	}

	// (th4) Last thing after every placement: put back anything a forced placement orphaned.
	// See RepairMonsterMap -- an orphan is invisible to the cursor and to every attack, which
	// on a mission level means the clear check never fires and the way home never opens.
	RepairMonsterMap();
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
		if( newMonsterOnMap ){
			#if TH4_MONSTERMAP_DEBUG
			if( In112(row, col) && MonsterMap[row][col] ) StompLog("AddMonster", monsterIndex, row, col, MonsterMap[row][col]);
			#endif
			// (th4) don't stack: move whoever is already here out of the way first
			if( In112(row, col) && MonsterMap[row][col] > 0 ){
				DisplaceMonsterFromCell(MonsterMap[row][col] - 1, row, col);
			}
			MonsterMap[ row ][ col ] = monsterIndex + 1;
		}
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

// (th4) WARLORD MERC REACH TEST, every summon's since 2026-10-13 (MonsterLookAround, the SUMMONS ONLY
// FIGHT WHAT THEY CAN SEE AND REACH filter). Path step for FindPath (Path.cpp): terrain and blocking
// objects only. Monsters and heroes standing in the way are NOT obstacles here, unlike the movement steps
// (CheckCellForMeleeMonsterSafeMove): they move, and a corridor full of the merc's own friends must
// not make the fight beyond it "unreachable". IsCellBlocking without the fly flag: the merc walks.
static int __fastcall MercReachStep( int monsterIndex, int row, int col )
{
	if( !In112( row, col ) || IsCellBlocking( row, col ) ) return 0;
	const int obj = ObjectsMap[ row ][ col ];
	if( obj && Objects[ abs( obj ) - 1 ].isBlockWalking ) return 0;
	return 1;
}
static const int MERC_PATH_RANGE = 12;	// tiles: beyond this a target needs the straight walkable line
// Can the merc walk to where it can hit `target`? The straight walkable line first (cheap, the usual
// case); for a FLIER (Monster::canFly: bats, ghosts... hovering over water, lava or a chasm, cells no
// walker may enter) the line goes to the cell next to it on the merc's side, where a melee merc
// stands to hit it. If the line is blocked -- a water pocket, a pillar, a bend -- the real A* path
// (FindPath, at most 24 steps): its goal cell may itself be blocked (GenerateSuccessors' target
// exemption), so a flier over water next to the shore is reachable and one deep over the water is not.
static bool MercCanReach( int mercIndex, const Monster& target )
{
	const Monster& m = Monsters[ mercIndex ];
	int reachRow = target.Row, reachCol = target.Col;
	if( target.canFly ){
		const int dRow = target.Row - m.Row, dCol = target.Col - m.Col;
		reachRow -= ( dRow > 0 ) - ( dRow < 0 );
		reachCol -= ( dCol > 0 ) - ( dCol < 0 );
	}
	if( CheckLineWithTwoArgumentsCheckFunction( IsCellNotBlockWalking, m.Row, m.Col, reachRow, reachCol ) ) return true;
	if( max( abs( target.Row - m.Row ), abs( target.Col - m.Col ) ) > MERC_PATH_RANGE ) return false;
	char steps[ MaxPathStep_25 + 3 ];
	return FindPath( MercReachStep, mercIndex, m.Row, m.Col, target.Row, target.Col, steps ) > 0;
}

// (th4) THE WAY TO THE FIGHT (MonsterLookAround, Ai_Summon; Mordor, 2026-10-13: "if summon cant see active monster it
// still should know the battle is ON and has to try to get to nearest active monster, just not through walls"). A summon
// that has no awake monster in sight and reach (the SUMMONS ONLY FIGHT WHAT THEY CAN SEE AND REACH filter) takes the
// NEAREST BY WALKING of the awake ones the filter turned down -- a breadth-first search between them and it over the cells
// MercReachStep lets a walker cross (terrain and blocking objects; monsters are no obstacle, they move), a diagonal step
// only past two open corners (Path.cpp CheckBlockStep, FindPath's own rule) -- and keeps the first step of that way in
// SummonFarStep; Ai_Summon takes it while the summon stands idle, instead of its fight AI (which would walk straight at
// the target, into the wall). Every finished step runs MonsterLookAround again (FixMonsterPosition), so the way is found
// again from the new cell, and once a monster is in sight and reach the normal choice takes over. The search stops at
// the first awake monster it meets, or FAR_WAY_MAX steps out, so it costs as much as the way is long. Not for Warlord
// mercs: with nothing in sight they go back to their master (Ai_Summon), as approved on 2026-09-16.
static const int FAR_WAY_MAX = 150;				// steps: an awake monster farther than this by walking is not looked for (a detour round half a level)
static uchar SummonFarStep[ SummonMonsters_Count ];	// 0 = no far target, else the orientation of the next step + 1
static uint FarSeen[ 112 * 112 ], FarGen = 0;		// this search's cells (a generation stamp: nothing to clear)
static int FarQueue[ 112 * 112 ], FarWho[ 112 * 112 ];	// the search's queue; per cell the awake monster its distance is to
static short FarDist[ 112 * 112 ];					// steps to that monster's cell
// cands = the awake monsters the filter turned down. The search spreads from all of them at once and stops when it reaches
// the summon, so the first one to get there is the nearest by walking, and every cell of the summon's last ring is
// known: of its neighbours one step closer, the summon takes the one pointing most directly at its monster (no zig-zag
// along the many equally short ways). Returns that monster, -1 = none on the way; stepRow = -1: already next to it.
static int SummonWayToNearest( int monsterIndex, const int* cands, int candN, int& stepRow, int& stepCol )
{
	const Monster& m = Monsters[ monsterIndex ];
	if( !In112( m.Row, m.Col ) || candN <= 0 ) return -1;
	if( !++FarGen ){ memset( FarSeen, 0, sizeof( FarSeen ) ); FarGen = 1; }
	static const int dRow[ 8 ] = { -1, -1, 1, 1, -1, 0, 1, 0 }, dCol[ 8 ] = { -1, 1, -1, 1, 0, -1, 0, 1 };	// diagonals first, as FindPath
	auto closedCorner = []( int row, int col, int dr, int dc ){	// a diagonal step past a closed corner (Path.cpp CheckBlockStep)
		return dr && dc && ( TileBlockWalking[ FineMap[ row + dr ][ col ] ] || TileBlockWalking[ FineMap[ row ][ col + dc ] ] );
	};
	const int start = m.Row * 112 + m.Col;
	int head = 0, tail = 0;
	for( int k = 0; k < candN; ++k ){	// the monsters' own cells (a flier's may be water: only the cells round it must be walkable)
		const Monster& t = Monsters[ cands[ k ] ];
		if( !In112( t.Row, t.Col ) ) continue;
		const int cell = t.Row * 112 + t.Col;
		if( cell == start || FarSeen[ cell ] == FarGen ) continue;
		FarSeen[ cell ] = FarGen; FarDist[ cell ] = 0; FarWho[ cell ] = cands[ k ];
		FarQueue[ tail++ ] = cell;
	}
	while( head < tail ){
		const int c = FarQueue[ head++ ];
		const int row = c / 112, col = c % 112, L = FarDist[ c ];
		if( L >= FAR_WAY_MAX ) continue;
		for( int dir = 0; dir < 8; ++dir ){
			const int nr = row + dRow[ dir ], nc = col + dCol[ dir ];
			if( !In112( nr, nc ) || closedCorner( row, col, dRow[ dir ], dCol[ dir ] ) ) continue;
			const int n = nr * 112 + nc;
			if( n == start ){	// reached the summon: its ring L is complete -- the best step into it
				if( !L ){ stepRow = -1; stepCol = -1; return FarWho[ c ]; }	// next to its monster: the fight AI takes over
				int best = c, bestTurn = 9;
				for( int d2 = 0; d2 < 8; ++d2 ){
					const int br = m.Row + dRow[ d2 ], bc = m.Col + dCol[ d2 ];
					if( !In112( br, bc ) || closedCorner( m.Row, m.Col, dRow[ d2 ], dCol[ d2 ] ) ) continue;
					const int b2 = br * 112 + bc;
					if( FarSeen[ b2 ] != FarGen || FarDist[ b2 ] != L ) continue;
					const Monster& t = Monsters[ FarWho[ b2 ] ];
					int turn = abs( OrientationToTarget( m.Row, m.Col, br, bc ) - OrientationToTarget( m.Row, m.Col, t.Row, t.Col ) ) % 8;
					if( turn > 4 ) turn = 8 - turn;
					if( turn < bestTurn ){ bestTurn = turn; best = b2; }
				}
				stepRow = best / 112; stepCol = best % 112;
				return FarWho[ best ];
			}
			if( FarSeen[ n ] == FarGen || !MercReachStep( monsterIndex, nr, nc ) ) continue;
			FarSeen[ n ] = FarGen; FarDist[ n ] = (short)( L + 1 ); FarWho[ n ] = FarWho[ c ];
			FarQueue[ tail++ ] = n;
		}
	}
	return -1;
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
	// (th4) the monster this one is already fighting, -1 if none (Warlord reach filter below: kept without sight)
	const int fightingNow = ( (monster.flag & MF_5_TARGET_MONSTER) && !(monster.flag & MF_11_NOENEMY) ) ? monster.TargetIndex : -1;
	// (th4) THE WAY TO THE FIGHT (above SummonWayToNearest): the awake monsters the sight / reach filter turns down
	const bool wayToFight = isSummon && !(monster.flag & MF_12_BERSERK) && !HasTrait( monsterIndex / SummonMonstersPerPlayer_Count, TraitId::Warlord );
	static int farCands[ Monsters_Max_Count ];
	int farN = 0;
	if( isSummon ) SummonFarStep[ monsterIndex ] = 0;

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

		if( targetMonsterIndex == monsterIndex // (rt) this is the monster itself
			|| (targetMonster.CurrentLife & (-64)) <= 0 // (rt) the enemy is dead
			|| targetMonster.Row == 1 && targetMonster.Col == 0 //(rt) the ones lying in ambush
			|| IsAttackTypes_1(targetMonsterIndex) && targetMonster.speechIndex // (rt) quest monster?
			|| !((monster.flag & MF_6_FRIENDLY)	&& !(monster.flag & MF_12_BERSERK)) && (abs(targetMonster.Row - monster.Row) >= 2 || abs(targetMonster.Col - monster.Col) >= 2)  && !NewToGolemAttackType(monsterIndex) // (rt) the enemy is not nearby, plus some other condition
			|| !((monster.flag & MF_6_FRIENDLY)   && !(monster.flag & MF_12_BERSERK)) &&   !(targetMonster.flag & MF_6_FRIENDLY) ){ // (rt) unclear condition
				continue;
		}
		
		// Forbid summons from hitting each other
		if( (monster.flag & MF_6_FRIENDLY) && (targetMonster.flag & MF_6_FRIENDLY) ){
		    continue;
        }

		// (th4) DC mission 13, Lazarus' lair. Two extra rejections for the three friendlies:
		//  - A_6_DEATH. The life test at the top of this loop is (CurrentLife & -64) <= 0,
		//    which a hero playing his death animation does not always satisfy yet -- so he
		//    stayed a legal target and they kept swinging at the body.
		//  - IsPlayerSideMonster. Belt and braces over the friendly-vs-friendly test just
		//    above, so the player's Golem and any other summon can never be picked.
		// Scoped to this level; every other monster selects targets exactly as before.
		if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
		 && (monster.flag & MF_6_FRIENDLY)
		 && ( targetMonster.CurAction == A_6_DEATH || IsPlayerSideMonster( targetMonsterIndex ) ) ){
			continue;
		}

		// (th4) DC mission 15: the armorer only fights what he can actually SEE. Without
		// this he picks heroes through the forge walls and sets off across the level after
		// them -- and since he is meant to hold his ground, that reads as him wandering off
		// for no reason. CheckCastInSight walks the line with IsCellNotBlockSight, the same
		// test the game uses to decide whether a spell may be cast at a spot.
		if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
		 && monster.newBossId && monster.newBossId - 1 == UM_587_Hephasto_the_Armorer
		 && !CheckCastInSight( monster.Row, monster.Col, targetMonster.Row, targetMonster.Col ) ){
			continue;
		}
        
        // Forbid summons from targeting non activated targets
		if( isSummon && targetMonster.ActivationCounter == 0 ){
			continue;
        }

		// (th4) SUMMONS ONLY FIGHT WHAT THEY CAN SEE AND REACH.
		//
		// A summon accepted ANY live hostile anywhere on the level: the two distance and
		// "is hostile" rejections at the top of this loop are both written
		// !(FRIENDLY && !BERSERK) && ..., so for a friendly non-berserk monster they are
		// false and never reject anything. Only the same-room PREFERENCE below then
		// applies, and a preference still picks a target when nothing is in the room.
		// The result is mercs walking off to a fight on the far side of a wall, which is
		// what this rejects.
		//
		// Exactly the treatment Hephasto already gets above, generalised: sight is the
		// same IsCellNotBlockSight walk the game uses to decide whether a spell may be
		// cast at a spot, so "my merc can attack it" now means the same as "I could cast
		// at it".
		//
		// REACH is MercCanReach (above MonsterLookAround): the straight walkable line, and the
		// real A* path (FindPath, terrain only, within MERC_PATH_RANGE) when the line is
		// blocked. A straight line alone is not enough: a water pocket in the caves breaks it
		// between a merc and bats that mercs standing a few tiles away walk round to and
		// fight, and that merc stood idle through the whole fight. Fliers: see MercCanReach.
		//
		// The target a merc is ALREADY FIGHTING (fightingNow) is kept while it stays
		// reachable even when sight breaks -- a bat dodging round a pillar must not make the
		// merc drop it, stand idle for a beat and pick it up again.
		//
		// RANGED summons are included: a bolt cannot cross a wall either, and the sight
		// test is what CastMissile itself checks before letting the shot happen. But a ranged
		// summon (skeleton archers: NewToGolemAttackType, the ranged AIs) shoots across water,
		// lava or a chasm it cannot walk over, so for it SIGHT is enough; only the target it is
		// already fighting, gone out of sight, still needs the reach test to be kept.
		// BERSERK monsters are deliberately excluded -- a berserked enemy is not the
		// player's minion and keeps its original behaviour.
		//
		// EVERY SUMMON (Mordor, 2026-10-13, after a player report: skeleton archers, golems and
		// greater skeletons went for a boss behind a wall and ignored the monsters in plain view;
		// it was Warlord mercs only from 2026-09-16, where it had been proven). TARGET CHOICE ONLY: when
		// a summon follows its owner is Ai_Summon's own rule, unchanged (Warlord mercs: no target;
		// every other summon: nothing awake on the level -- Mordor wants them to stay in the fight).
		bool farOnly = false;	// (th4) THE WAY TO THE FIGHT: turned down here, kept for the way to the nearest (below the loop)
		if( isSummon && !(monster.flag & MF_12_BERSERK) ){
			const bool inSight = CheckCastInSight( monster.Row, monster.Col, targetMonster.Row, targetMonster.Col );
			if( NewToGolemAttackType( monsterIndex ) ){	// ranged: sight, or the current target while reachable
				if( !inSight && ( targetMonsterIndex != fightingNow || !MercCanReach( monsterIndex, targetMonster ) ) ){
					farOnly = true;
				}
			}else{										// melee (the Warlord mercs' rule as it was): sight or the current target, and reach
				if( ( targetMonsterIndex != fightingNow && !inSight ) || !MercCanReach( monsterIndex, targetMonster ) ){
					farOnly = true;
				}
			}
			if( farOnly && !wayToFight ){	// Warlord mercs: no way to the fight (above SummonWayToNearest)
				continue;
			}
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
		if( farOnly ){	// (th4) THE WAY TO THE FIGHT: a candidate only if nothing passes the filter (below the loop)
			farCands[ farN++ ] = targetMonsterIndex;
			continue;
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

	// (th4) THE WAY TO THE FIGHT (above SummonWayToNearest): nothing in sight and reach -- the nearest awake monster by walking
	if( targetIndex == -1 && farN > 0 ){
		int stepRow = 0, stepCol = 0;
		const int farIdx = SummonWayToNearest( monsterIndex, farCands, farN, stepRow, stepCol );
		if( farIdx >= 0 ){
			monster.flag |= MF_5_TARGET_MONSTER;
			targetIndex = farIdx;
			targetRow = Monsters[ farIdx ].NextRow;
			targetCol = Monsters[ farIdx ].NextCol;
			if( stepRow >= 0 ) SummonFarStep[ monsterIndex ] = (uchar)( OrientationToTarget( monster.Row, monster.Col, stepRow, stepCol ) + 1 );
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
	// (rt) there was a bug: going out of the monster map bounds and possibly corrupting the level map and dungeon tile array in th1
	// (rt) and corrupting random memory in th2 (here it corrupted the system variables' memory
	// (rt) responsible for the application being active in Windows, which is why it froze)
	for( int col = prevCol - 1; col <= prevCol + 1; ++col ){
		for( int row = prevRow - 1; row <= prevRow + 1; ++row ){
			auto cell = &MonsterMap[ row ][ col ];
			if( In112(row, col) && (*cell == -monsterIndex - 1 || *cell == monsterIndex + 1) ){
				*cell = 0;
			}
		}
	}
	if( prevRow < FineMap_112 - 1 ) FlagMap[ prevRow + 1 ][ prevCol ] &= ~CF_16_MONSTER_HORIZONTAL; // (rt) right down
	if( prevCol < FineMap_112 - 1 ) FlagMap[ prevRow ][ prevCol + 1 ] &= ~CF_16_MONSTER_HORIZONTAL;   // (rt) left down
	if( HorizontalHit && monster.NextRowHz > 0 && monster.NextRowHz < 112 && monster.NextColHz > 0 && monster.NextColHz < 112 ){
		FlagMap[ monster.NextRowHz ][ monster.NextColHz ] &= ~CF_16_MONSTER_HORIZONTAL; // (rt) left/right
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
		case BM_259_WATER_WYRM or BM_263_RED_WYRM or BM_269_BOG_CRAWLER or BM_270_MUD_CRAWLER or BM_271_THORNED_HULK or BM_289_FUNGAL_ROOT or BM_290_FUNGAL_ROOT_LITE or BM_NPC_420_GRISWOLD: kb = 0; break; // wyrm cant be KB'ed
		case BM_283_FROST_WARDEN or BM_260_ARMAGEDDON_WALKER or BM_261_SKELETAL_KNIGHT or BM_286_BRINE_CORPSE: kb = _1(6); break; // armageddon walker: 83% rez
		case BM_265_CROC or BM_266_CROC_BERS or BM_287_ARMORED_SKELETON or BM_298_TENTACLIST_BOSS: kb = _n2(3); break;
		case BM_285_PHANTOM_KNIGHT: kb = _n2(4); break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2: kb = _1(14); break;
		case BM_274_FLESH_DEVOURER: kb = _1(2); break;
		case BM_H_412_WAR_H_X_EXEC or BM_H_421_WAR_H_X_EXEC_ELITE or BM_H_422_WAR_H_ROYAL_GUARD: kb = _1(5); break; // (th4) Executioner: 80% KB rez
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
//			NetSendCmdLocParam2Direct(CMD_36_MONSTDEATH, monster.Row, monster.Col, monsterIndex, monster.dungeon);
		}
	}
	MonsterKilledByPlayer(monsterIndex, playerIndex, 1);
}

// (th4) Credit one kill to a player's lifetime counter.
// SP only, and never in Coliseum (PvP-only mode -- those aren't "monsters killed").
// Callers must already have verified the monster wasn't dead, so each death counts once.
void __fastcall CreditMonsterKill(int playerIndex)
{
	if( MaxCountOfPlayersInGame != 1 ) return;		// single player feature
	if( GameMode == GM_COLISEUM ) return;			// excluded mode
	if( playerIndex < 0 || playerIndex >= PlayersMax_4 ) return;
	uint& kills = Players[playerIndex].totalKills;
	if( kills < UINT_MAX ) ++kills;					// saturate rather than wrap
}

//----- (0043D109) --------------------------------------------------------
void __fastcall MonsterKilledByPlayer(int monsterIndex, int playerIndex, bool sendMsg)
{
	Monster& monster = Monsters[monsterIndex];
	if( monsterIndex >= Monsters_Max_Count || !monster.SpritePtr || monster.CurAction == A_6_DEATH ){
		return;
	}
	// (th4) lifetime kill count. Mirror the XP-credit rule below: indices under
	// SummonMonsters_Count are the players' own summons, so their deaths aren't kills.
	if( monsterIndex >= SummonMonsters_Count ){
		CreditMonsterKill( playerIndex );
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
	if (monster.LightIndex) {
		LightOff(monster.LightIndex);
	}
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
	}else if( !(SummonCleanupSilent && monsterIndex < SummonMonsters_Count) ){
		// (th4) silent during the level-transition summon cleanup -- that is not a real death
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

// (rt) the animation effect on Uber on level 24 works wrong, look into it (to check, PlayDiabloVictory in MainGamePlayLoop can be disabled on DL_0_HORROR
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
		case UM_914_DC_BROTHER_SILAS:
			if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
				break;
			}
			if (Dungeon == DUN_65_RUINED_TEMPLE || Dungeon == DUN_70_CITADEL) {
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
		case UM_924_DC_Commander_Latham:
			switch (player.fullClassId) {
			case PFC_WARRIOR or PFC_TEMPLAR or PFC_INQUISITOR or PFC_THRAEX or PFC_MURMILLO or PFC_SECUTOR:
				if (HasTrait(playerIndex, TraitId::Ranger))			{ break; }
				if (HasTrait(playerIndex, TraitId::Fechtmeister))	{ break; }
				if (HasTrait(playerIndex, TraitId::Scissor))		{ break; }
				if (Difficulty <= DL_3_CRUCIBLE) {
					SpawnUnique(RngFrom(Difficulty + 2526, -1, -1), monster.Row, monster.Col, sendMsg);
				}
			}
			{
				bool rob = false;
				if (is(player.fullClassId, PFC_KENSEI, PFC_SHUGOKI, PFC_SHINOBI, PFC_ROGUE, PFC_ASSASSIN, PFC_BERSERKER, PFC_MURMILLO, PFC_THRAEX, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID, PFC_INQUISITOR )
					|| player.fullClassId == PFC_WARRIOR && (!(HasTrait(playerIndex, TraitId::Ranger)))
					|| player.fullClassId == PFC_GUARDIAN && (!(HasTrait(playerIndex, TraitId::Pistoleer)))
					|| player.fullClassId == PFC_MONK && (!(HasTrait(playerIndex, TraitId::Bloodless)))
					|| player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)
					|| player.fullClassId == PFC_TEMPLAR && (!(HasTrait(playerIndex, TraitId::Paladin)))) {
					rob = true;
				}
				if (HasTrait(playerIndex, TraitId::OldFashioned)) { rob = false; }
				if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) { rob = false; } // he is sorted out at Pistoleer trait check, no?...
				if (rob) {
					uniqIndex = by(Difficulty, 4592, 4593, 4594, 4595, 4596, 4597, 4598, -1, -1);
					if (player.fullClassId == PFC_INQUISITOR && Difficulty <= DL_6_TERROR) { 
						uniqIndex += 309; // 4901 - 4907
						if (HasTrait(playerIndex, TraitId::Warlord)) {
							uniqIndex += 7;// 4908 - 4914
							// test drops
							//for (uniqIndex = 4908; uniqIndex <= 4914; uniqIndex++) {
							//	SpawnUnique(uniqIndex, monster.Row, monster.Col, sendMsg);
							//}
						}
					}
				}
				else if (player.fullClassId == PFC_IRON_MAIDEN) {
					uniqIndex = 4917 + Difficulty; // 4917 - 4925
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
				if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
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
		case UM_915_DC_SIR_CABBOT:	// (th4) DC mission 6: Cabbot drops Arkaine's Valor like the Apparition
			if (player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
				break;
			}
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
		case UM_910_DC_AZDOG:
			uniqIndex = by(Difficulty, 152, 153, 154, 2383, 2384, 2385, 2386, 2387, 2388);
			break;
		case UM_922_DC_Donna_Paloosa:
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
				is(player.fullClassId, PFC_SHUGOKI, PFC_EXECUTIONER, PFC_DRUID, PFC_BERSERKER, PFC_GLADIATRIX)
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
				uniqIndex = 2807 + Difficulty; // empyrean bracers
			}
			break;
		case UM_753_Sister_Priscilla:
			uniqIndex = 2843 + Difficulty; // The Wanderer's Boots
			break;
			// DC mode -----------------
		case UM_906_DC_LORD_ANTONIUS:
			uniqIndex = by(Difficulty, 149, 150, 151, 2377, 2378, 2379, 2380, 2381, 2382); // Rings of Purity
			break;
		case UM_920_DC_Bishop_Lucian:
			uniqIndex = 2807 + Difficulty; // empyrean bracers
			break;
		case UM_927_DC_General_Carnabus:
			uniqIndex = 2843 + Difficulty; // The Wanderer's Boots
			break;
		case UM_925_DC_Abbot_Balatt:
			uniqIndex = 2825+ Difficulty; // Adventurer's Belt
			break;
		case UM_944_DC_Knight_Grimald:	// (th4) DC mission 22: Veil of Steel, same lines as Lachdanan
			switch (player.ClassID) {
			case PC_1_ARCHER or PC_3_MONK or PC_4_ROGUE:
				uniqIndex = by(Difficulty, 441, 442, 443, 2395, 2396, 2397, 2398, 2399, 2400); break; // dex line
			case PC_2_MAGE:
				uniqIndex = by(Difficulty, 444, 445, 446, 2401, 2402, 2403, 2404, 2405, 2406); break; // mag line
			case PC_0_WARRIOR or PC_5_SAVAGE:
				uniqIndex = by(Difficulty, 184, 185, 186, 2389, 2390, 2391, 2392, 2393, 2394); break; // str line
			}
			break;
			// end DC -----------------
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
    // (th4) DARK CAMPAIGN: the player's own people pay out nothing when they fall -- losing
    // one is a loss, not loot. Placed with the summon check because it is the same idea, and
    // it covers everything below: gold, potions, boss drops and SpecifiedUniqueDrop.
    // The Butcher (m25), the two succubi (m10), Lazarus (m13) and the armorer (m15).
    if( GameMode == GM_DARK_CAMPAIGN
     && is( bossIndex, UM_9_The_Butcher, UM_5_Red_Vex, UM_6_Dark_Jade,
                       UM_4_ArchBishop_Lazarus, UM_587_Hephasto_the_Armorer ) ){
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
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN) || Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
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

		if (player.gameChanger & BIT(GC_27_TERMINATOR) && RNG(100) < ( 15 + (Difficulty * 3) ) ) { 
			SpawnSomethingQlvl(BI_2888_SELF_REPAIR_MODULE, monster.Row, monster.Col, monsterDropLevel, sendMsg); 
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
		if (GameMode != GM_CLASSIC && GameMode != GM_COLISEUM 
			&& !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR))
			&& !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER))
			) {
			TryToDropSigil(monster.Row, monster.Col);
		}
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
		case UM_936_DC_Ludwig_the_Paranoid:	// (th4) DC mission 16
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
		case UM_926_DC_Black_Hawk:
			// (th4) DC mission 11: the Anvil of Fury is what the hero squad came down here
			// for. Same drop as Madame Pompadour in the Colonnade -- he is carrying it, so
			// killing him puts it back on the floor.
			SpawnSomethingQlvl(BI_16_ANVIL_OF_FURY, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			dropNItems(4);
			break;
		case UM_927_DC_General_Carnabus:	// (th4) DC mission 12: the heroes' new general
			dropNItems(5);
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

		case UM_903_AZGOR:
			SpawnSomethingQlvl(BI_2706_SCROLL_OF_MAGIC, monster.Row, monster.Col, monsterDropLevel, sendMsg);	
			dropNItems(2);
			break;

		case UM_904_DOBAZZU:
		{
			if (not(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE, GM_SURVIVAL)) {
				SpawnSomethingQlvl(BI_2707_SCROLL_OF_MAGIC, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
		}
			dropNItems(2);
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
		case UM_906_DC_LORD_ANTONIUS:
			dropNItems( 2 );
            break;
        case UM_4_ArchBishop_Lazarus:
        case UM_1_Skeleton_King:
		case UM_323_Lord_of_the_Order:
		case UM_718_Guardian_of_Flames:
		case UM_726_Apparition_of_Arkaine:
		case UM_915_DC_SIR_CABBOT:	// (th4) DC mission 6: same generic drop count as the Apparition
		case UM_742_Ironhoof:
		case UM_755_Redbark_Elderroot:
		case UM_809_Borosteron:
		case UM_916_DC_PALADIN_LOXLEY:
		case UM_924_DC_Commander_Latham:	// (th4) DC mission 9
		case UM_925_DC_Abbot_Balatt:		// (th4) DC mission 10
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
		case UM_907_DC_ZYPHERYS:
		case UM_938_DC_Lord_Tiberius:	// (th4) DC mission 17
		case UM_940_DC_Darius_the_Ronin:	// (th4) DC mission 18
		case UM_941_DC_Lord_Tenabris:
		case UM_942_DC_Venturius_the_Inquisitor:	// (th4) DC mission 21
			dropNItems( 4 );
			break;
		case UM_945_DC_Prophetess_Lyssia:	// (th4) DC mission 22
			dropNItems(5);
			break;
		case UM_946_DC_Cleric_Dwaynor:	// (th4) DC mission 23
			dropNItems(8);
			break;
		case UM_947_DC_High_Exemplar_Kastor:	// (th4) DC mission 24
			dropNItems(7);
			break;
		case UM_948_DC_Warden_Gaspax:	// (th4) DC mission 25
			dropNItems(8);
			break;
		case UM_905_DC_Griswold:	// (th4) DC mission 26
			for (int elixir = 0; elixir < 2; ++elixir) {
				SpawnSomethingQlvl(BI_20_SPECTRAL_ELIXIR, monster.Row, monster.Col, monsterDropLevel, sendMsg);
			}
			dropNItems(10);
			break;
		case UM_583_Lich_King:
			if (GameMode == GM_DARK_CAMPAIGN) {	// (th4) DC mission 19
				for (int elixir = 0; elixir < 3; ++elixir) {
					SpawnSomethingQlvl(BI_20_SPECTRAL_ELIXIR, monster.Row, monster.Col, monsterDropLevel, sendMsg);
				}
			}
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
		if( monster.CurFrame == monster.animFrameCount ){ // (rt) last animation frame
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
	// if monster could walk to player, player should hear his steps
	// (th4) HEARD: that test is an A* search (FindPath, Path.cpp) and it used to run first, on EVERY walk frame of every
	// walking monster, though only one or two frames of a walk play a step. In Ravenholm (dozens of zombies always on
	// the move) that was ~110 searches per game tick, ~13 ms -- the map's stutter (measured 2026-10-09).
	// Now each case asks heard() only on a frame that plays a step; it searches at most once per call, and not at all
	// when the step could not be heard anyway: SoundPos() is the same test PlayLocalSound -> LoadAndPlaySoundFile makes
	// (Engine.cpp), and a hero 25+ cells away needs 25+ steps, which FindPath always refuses. Same sounds as before.
	// A NEW CASE below must keep the "&& heard()" in its frame test.
	int reach = -1;
	auto heard = [&]() -> bool {
		if( reach < 0 ){
			char steps[28];
			int range, angle;
			auto checkCellFunc = monster.flag & MF_10_OPEN_DOORS ? CheckCellForMonsterCanInteractWithDoorsSafeMove
			                                                     : CheckCellForMeleeMonsterSafeMove;
			reach = SoundPos( monster.Row, monster.Col, range, angle )
			     && max( abs( monster.Row - player.Row ), abs( monster.Col - player.Col ) ) < MaxPathStep_25
			     && FindPath( checkCellFunc, monsterIndex, monster.Row, monster.Col, player.Row, player.Col, steps );
		}
		return reach != 0;
	};
	{
		switch( MonsterSprites[monster.SpriteIndex].baseMonsterIndex ){
		case BM_291_MYCONID:
			if (is(monster.WalkframeCounter, 6, 15) && heard()) PlayLocalSound(RandSound(1543, 1546), monster.Row, monster.Col); // 1543-1546: light-stone (4)
			break;
		case BM_126_PLAGUE_EATER or BM_221_RAT_MAN or BM_258_RAT_HORDE: // Rats
			if( is(monster.WalkframeCounter, 2,  6) && heard() ) PlayLocalSound(                             RandSound(1543, 1546)                        , monster.Row, monster.Col ); // 1543-1546: light-stone (4)
			break;
		case BM_153_DEATH_MINION or BM_154_DEATH_MINION: // cryptic reapers
			if( is(monster.WalkframeCounter, 4,  9) && heard() ) PlayLocalSound(                             RandSound(1539, 1542)                        , monster.Row, monster.Col ); // 1539-1542: med - stone (4)
			break;
		case BM_87_DARK_REAPER or BM_136_BLOOD_SLAYER or BM_152_BLACK_REAPER: // other reapers
			if( is(monster.WalkframeCounter, 4,  9) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1547-1550: med - dirt (4) // 1619-1622: med snow run (4)
			break;
		case BM_117_STINGER or BM_143_DOOM_TAIL or 229: // stingers (including summons)
			if( is(monster.WalkframeCounter, 1,  3) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1551-1553: light - dirt (3) // 1611-1613: light snow (3)
			break;
		case BM_163_STINGER_LORD: // stinger lord
			if( is(monster.WalkframeCounter, 3,  8) && heard() ) PlayLocalSound(                             RandSound(1554, 1557)                        , monster.Row, monster.Col ); // 1554-1557: light - dirtRun (4)
			break;
		case BM_58_DEFILER: // defiler/nakrul
			if( is(monster.WalkframeCounter, 2,  5) && heard() ) PlayLocalSound(                             RandSound(1558, 1565)                        , monster.Row, monster.Col ); // 1558-1565: ginormous (8)
			break;
		case BM_74_GREATER_HULK or BM_75_BLOOD_HULK: // undead hulks
			if( is(monster.WalkframeCounter, 2,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1566-1569: heavy - dirt (4) // heavy snow run-----
			break;
		case BM_59_PIT_FIEND or BM_217_GRIMSPIKE: // pit fiend / grimspike
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1570-1573: heavy - stoneRun (4) // heavy snow run----
			break;
		case BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER: // grave diggers
			if( is(monster.WalkframeCounter, 6, 18) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? 1                     : RandSound(1611, 1614), monster.Row, monster.Col ); // 1611-1614: light snow (4)
			break;
		case BM_46_PIT_SPIDER or BM_112_WAILING_BEAST or BM_134_MUD_RUNNER or BM_205_PINKY or 230: // pit spider + boars
			if( is(monster.WalkframeCounter, 3,  8) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med - dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_97_DARK_CHARGER:
			if( is(monster.WalkframeCounter, 3,  8) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med - dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_120_NECROMORPH: // cryptic flesh thing
			if( is(monster.WalkframeCounter, 8    ) && heard() ) PlayLocalSound(                             RandSound(1570, 1573)                        , monster.Row, monster.Col ); // heavy - stoneRun-----
			break;
		case BM_135_GROTESQUE or BM_141_SHAMBLER or BM_142_HELL_MEAT: // other flesh things
			if( is(monster.WalkframeCounter, 8    ) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy - dirt----- // heavy snow run-----
			break;
		case BM_83_LASH_WORM: // lash worm
			if( is(monster.WalkframeCounter, 4, 10) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1554, 1557) : RandSound(1611, 1613), monster.Row, monster.Col ); // light - dirtRun----- // 1611-1613: light snow (3)
			break;
		case BM_115_GRUNT_BEAST or BM_123_BICLOPS: // biclops
			if( is(monster.WalkframeCounter, 2,  7) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1574-1577: med - dirtRun // 1619-1622: med snow run (4)
			break;
		case BM_111_OBSIDIAN_GOLEM or BM_125_GOLD_GOLEM or BM_130_BLACK_GOLEM: // stone golems
			if( is(monster.WalkframeCounter, 6, 15) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy - stoneRun----- // heavy snow run-----
			break;
		case BM_38_UNDEAD_WEREWOLF or BM_47_ACID_HOUND or BM_49_BILE_SPITTER or BM_53_HELL_HOUND or BM_172_SPITTING_TERROR or BM_206_ACID_SPITTER or BM_241_ACID_TERROR: // Hounds
			if( is(monster.WalkframeCounter, 5    ) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1578, 1581) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1578-1581: light-stoneRun (4) // 1611-1613: light snow (3)
			break;
		case BM_80_DEVIL_KIN or BM_116_IMP_BRUTE or BM_128_GREATER_IMP or BM_148_TURBAK_THE_DEUCE or BM_149_THRESHER or BM_207_DREGBEZ: // Big Fallen
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1582-1585: med-stoneRun (4) // 1619-1622: med snow run (4)
			break;
		case BM_95_GHOST_WARRIOR or BM_96_CURSE_BEARER or BM_122_BLOOD_SPECTRE or BM_285_PHANTOM_KNIGHT: // Black Knights
			if( is(monster.WalkframeCounter, 2,  6) && heard() ) PlayLocalSound( RandSound(1765, 1772) /*!IsSnowStep(monsterIndex) ? RandSound(1586, 1589) : RandSound(1623, 1626)*/, monster.Row, monster.Col ); // 1586-1589: Heavy Knight (4) // 1623-1626: heavy snow knight (4)
			break;
		case BM_70_FROST_WING or BM_72_FIREWING or BM_73_DEATH_ANGEL or BM_132_BONEWING or BM_167_SKULLWING or BM_210_HELLWING or 228: // Skullwings
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_219_FROST_BONE:// Frost Bone
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound(                             RandSound(1594, 1597)                        , monster.Row, monster.Col ); // med snow run-----
			break;
		case BM_110_DIABLO or BM_137_UBER_DIABLO: // Diablo
			if( is(monster.WalkframeCounter, 2,  5) && heard() ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_208_WARPED_ONE or BM_12_IMP or BM_13_DARK_ONE or BM_155_FALLEN_ONE or BM_157_YELLOW_ONE or BM_4_GREMLIN or BM_5_FALLEN_ONE or BM_105_IMP_ROGUE or BM_156_IMP_ROGUE or BM_189_CARVER or BM_196_EVIL_ONE or BM_213_POISONED_ONE: // Fallen (sword & spear)
			if( is(monster.WalkframeCounter, 1,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col );  // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_281_FALLEN_BOW or BM_282_GREMLIN_BOW:
			if (is(monster.WalkframeCounter, 2, 9) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col);  // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_52_BILE_DEMON or BM_55_GRIM_DEMON or BM_56_PIT_DEMON or BM_93_FLAYED_ONE or BM_178_OVERLORD or BM_179_FILTH_DEMON: // Fat
			if( is(monster.WalkframeCounter, 4,  9) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy stone run----- // heavy snow run-----
			break;
		case BM_51_THE_BUTCHER or BM_99_CLEAVER_BOSS or BM_195_UBER_BUTCHER: // FatC
			if( is(monster.WalkframeCounter, 2,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy stone run----- // heavy snow run-----
			break;
		case BM_42_NIGHT_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or BM_45_VENOM_CLAN or BM_124_GLOOM_CLAN or BM_165_APOCALYPSE_CLAN or BM_198_HORN_CLAN or BM_199_HELL_CLAN or 224 or BM_255_RAGE_CLAN or BM_256_RAGE_CLAN or BM_34_FLESH_CLAN
		  or BM_35_FIRE_CLAN or BM_36_DARKNESS_CLAN or BM_37_STONE_CLAN or BM_173_GHARBAD_BASEMNSTR or BM_175_TERROR_CLAN or BM_186_DEATH_CLAN or BM_187_BLOOD_CLAN or BM_204_SHADOW_CLAN or BM_209_HATRED_CLAN or BM_254_TWILIGHT_CLAN: // Goatmace + Goatbow + Goat_2h_Morgenstern
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1615, 1617), monster.Row, monster.Col ); // med stone----- // 1615-1617: med snow (3)
			break;
		case BM_214_SATYR_LORD or BM_33_MOON_CLAN or BM_57_HELL_CLAN or BM_166_GOATLORD_BOSS or BM_188_HELL_CLAN or 225: // Goatlord
			if( is(monster.WalkframeCounter, 2,  8) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_109_GOLEM or BM_113_ASH_GOLEM: // Golems
			if( is(monster.WalkframeCounter, 2, 10) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col ); // med dirt Run----- // 1619-1622: med snow run (4)
			break;
		case BM_218_ICE_GOLEM: // Golem Ice
			if( is(monster.WalkframeCounter, 2, 10) && heard() ) PlayLocalSound(                             RandSound(1594, 1597)                        , monster.Row, monster.Col ); // 1594-1597: Med Snow Run (4)
			break;
		case BM_60_HELL_STONE or BM_61_BLOOD_STONE or BM_62_MAULER or BM_63_LAVA_LORD or BM_180_CRYPT_STONE: // Magma
			if( is(monster.WalkframeCounter, 1,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1598, 1601) : RandSound(1606, 1609), monster.Row, monster.Col ); // 1598-1601: Heavy I Stone (4) // heavy snow run-----
			break;
		case BM_18_VORTEX_ROGUE or BM_85_BALOR or BM_86_SLAYER or BM_88_BALROG or BM_159_GUARDIAN or BM_176_DESTROYER or BM_193_ABATAN: // Mega
			if( is(monster.WalkframeCounter, 2,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_16_HORNED_BEAST or BM_65_HELL_SPAWN or BM_66_HORNED_DEATH or BM_67_BLACK_HORN or BM_171_MUD_LORD or BM_185_BLOOD_HORN or BM_197_CEROMORTH: // Rhino
			if( is(monster.WalkframeCounter, 3,  7) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_17_SCAVENGER or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER: // Scav
			if( is(monster.WalkframeCounter, 3,  7) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_19_RAVAGER: // Ravager (fast Scav)
			if( is(monster.WalkframeCounter, 1,  3) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col ); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_14_SOUL_HUNTER or BM_20_RISEN_SKELETON or BM_21_SKELETON_WARRIOR or BM_22_BURNING_DEAD or BM_23_MALICE or BM_114_FROZEN_HORROR or 226 or 227 or BM_158_DEATH_RETINUE or BM_11_HATRED
		  or BM_9_SKELETON_WARRIOR or BM_10_BURNING_DEAD or BM_7_RISEN_SKELETON or BM_8_BONE_CREEPER or BM_24_RISEN_SKELETON or BM_25_SKELETON_WARRIOR or BM_26_BURNING_DEAD or BM_27_HORROR: // Small Skellies (axes, sword_shield, bow)
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound(                             RandSound(1602, 1605)                        , monster.Row, monster.Col ); // 1602-1605: light o stone (4)
			break;
		case BM_1_OBLIVION_KNIGHT or BM_50_SKELETON_LORD or BM_64_DEATH_GUARDIAN or BM_203_DEATH_KNIGHT: // Leoric
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound(                             RandSound(1590, 1593)                        , monster.Row, monster.Col ); // Med O Stone Run-----
			break;
		case BM_29_HIDDEN or BM_30_SHADOW or BM_31_UNSEEN or BM_32_INVISIBLE_ONE or BM_92_ILLUSION_WEAVER or BM_94_LURKER or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_194_BLOODIED_HIDDEN or BM_257_SHROUDED : // Hidden
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_101_DARK_MISTRESS or BM_102_SUCCUBUS or BM_103_BLOOD_WITCH or BM_104_DARK_WITCH or BM_133_SOUL_BURNER or BM_191_DARK_MISTRESS or BM_202_SUCCUBUS or BM_284_SIN_SPAWN: // Succ
			if( is(monster.WalkframeCounter, 2,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_71_THUNDER_LORD or BM_76_DEATH_CLAW or BM_77_STORM_DEMON or BM_78_EVISCERATOR or BM_79_FEAR_HUNTER or BM_160_CHASM_DEMON or BM_164_LIGHTNING_DEMON: // Thin
			if( is(monster.WalkframeCounter, 3,  7) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1547, 1550) : RandSound(1619, 1622), monster.Row, monster.Col ); // med dirt----- // 1619-1622: med snow run (4)
			break;
		case BM_28_BLOOD_WARPER or BM_39_WARPING_DEMON or BM_150_FELLTWIN or BM_151_HYBRID or BM_182_BLINK_DEMON: // Tsneak
			if( is(monster.WalkframeCounter, 1,  8) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_2_ROTTING_CARCASS or BM_140_GHOUL or BM_169_ZOMBIE or BM_0_BURNED_CORPSE or BM_215_CADAVER: // Zombies
			if( is(monster.WalkframeCounter, 9, 21) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(   1,    1) : RandSound(1611, 1613), monster.Row, monster.Col ); // 1611-1613: light snow (3)
			break;
		case BM_232_ABYSSAL_SUCCUBUS: // Abyssal Succubus
			if( is(monster.WalkframeCounter, 4,  9) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1543, 1546) : RandSound(1611, 1613), monster.Row, monster.Col ); // light stone----- // 1611-1613: light snow (3)
			break;
		case BM_245_BLUNDERER: // Warden
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_233_BLOOD_LORD or BM_234_STEEL_LORD: // Minot
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_236_CORPSE_EATER: // Corpse Eater
			if( is(monster.WalkframeCounter, 1,  7) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_106_CORRUPT_ORDER or BM_146_CORRUPT_ORDER or BM_162_CORRUPT_PALADIN or BM_216_ZEALOT or BM_392_WARLORD_MERC
				or BM_H_393_SORC_L_T or BM_H_394_ROG_L_B or BM_H_395_WAR_L_X or BM_H_396_SORC_L_T or BM_H_397_ROG_L_B or BM_H_398_WAR_L_X or BM_H_399_WAR_L_SS
				or BM_H_400_SORC_M_T or BM_H_401_MONK_M_MS or BM_H_402_BERS_M_X or BM_H_403_ROG_M_B or BM_H_404_WAR_M_MS or BM_H_405_SOR_M_T_HYBRID or BM_H_406_SCOUT
				or BM_H_407_MONK_M_X or BM_H_408_WAR_M_SS or BM_H_409_MONK_H_T or BM_H_410_MONK_H_SS or BM_H_411_ROG_H_B or BM_H_412_WAR_H_X_EXEC or BM_H_413_SOR_H_T
				or BM_H_414_WAR_H_MS or BM_H_415_ROG_H_B_ELITE_ARCHER or BM_H_416_MON_H_MS or BM_H_417_WAR_H_SS_WARRIOR_ELITE or BM_H_418_SOR_H_T_SORC_ELITE or BM_H_419_ROG_H_B_SS_ELITE
				or BM_H_421_WAR_H_X_EXEC_ELITE or BM_NPC_420_GRISWOLD or BM_H_422_WAR_H_ROYAL_GUARD or BM_H_423_ROG_H_B_BLACK_ARCHER: // Corrupts
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(   0,    3) : RandSound(1619, 1622), monster.Row, monster.Col ); // 1619-1622: med snow run (4)
			break;
		case BM_253_EXPLODER: // Exploder
			if( is(monster.WalkframeCounter, 1    ) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone----- // 1619-1622: med snow run (4)
			break;
		case BM_244_HORADRIM_ANCIENT or BM_246_GREATER_MUMMY: // Mummies
			if( is(monster.WalkframeCounter, 2, 11) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1566, 1569) : RandSound(1606, 1609), monster.Row, monster.Col ); // heavy dirt----- // heavy snow run-----
			break;
		case BM_222_NIGHT_DEMON: // Baboon
			if( is(monster.WalkframeCounter, 1,  6) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case 231 or 247: // Siege
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // ginormous-----
			break;
		case BM_243_IZUAL: // Izual
			if(     is(monster.WalkframeCounter, 1) && heard() ) PlayLocalSound(                             RandSound(1586, 1589)                        , monster.Row, monster.Col ); // ginormous-----
			else if(is(monster.WalkframeCounter, 5) && heard() ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col ); // Heavy Knight-----
			break;
		case BM_252_HEPHASTO:
			// Hephasto // ginormous-----
			if( is(monster.WalkframeCounter, 1,  7) && heard() ) PlayLocalSound(                             RandSound(1558, 1561)                        , monster.Row, monster.Col );
			break;
		case BM_251_WENDIGO:
			// Wend // 1606-1609: heavy snow run (4)
			if( is(monster.WalkframeCounter, 1,  7) && heard() ) PlayLocalSound(                             RandSound(1606, 1609)                        , monster.Row, monster.Col );
			break;
		case BM_238_TAINTED or BM_239_AFFLICTED: // Misshapen
			if( is(monster.WalkframeCounter, 1,  5) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_240_DEMON_PRIEST: // Demon priest
			if( is(monster.WalkframeCounter, 9    ) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_242_REANIMATED_HORROR: // Reanimated
			if( is(monster.WalkframeCounter, 1    ) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_249_SCARAB or BM_250_BONE_SCARAB: // Scarabs
			if( is(monster.WalkframeCounter, 1,  8) && heard() ) PlayLocalSound( !IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col ); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_260_ARMAGEDDON_WALKER: // Abi
			if (is(monster.WalkframeCounter, 1, 9) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1582, 1585) : RandSound(1619, 1622), monster.Row, monster.Col); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_261_SKELETAL_KNIGHT:
			if (is(monster.WalkframeCounter, 1, 9) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1586, 1589) : RandSound(1623, 1626), monster.Row, monster.Col); // knight 'boot' sounds
			break;
		case BM_264_BITELING:
			if (is(monster.WalkframeCounter, 11) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col); // light dirt----- // 1611-1613: light snow (3)
			break;
		case BM_265_CROC or BM_266_CROC_BERS:
			if (is(monster.WalkframeCounter, 5, 13) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2:// try to find heavier footstep sounds. heavy and bony
			if (is(monster.WalkframeCounter, 7, 18) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_271_THORNED_HULK:
			if (is(monster.WalkframeCounter, 5, 11) && heard()) PlayLocalSound(RandSound(1760, 1764), monster.Row, monster.Col);
			break;
		case BM_272_SPIDER_QUEEN:
			if (is(monster.WalkframeCounter, 2, 9/*1, 8*/) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1543, 1545) : RandSound(1611, 1613), monster.Row, monster.Col); // light stone----- (without 4th!) // 1611-1613: light snow (3)
			break;
		case BM_273_FLAME_FIEND:
			if (is(monster.WalkframeCounter, 4, 9) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1781, 1784) : RandSound(1777, 1780), monster.Row, monster.Col);
			break;
		case BM_274_FLESH_DEVOURER:
			if (is(monster.WalkframeCounter, 7, 15) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_275_BEHEMOTH:
			if (is(monster.WalkframeCounter, 7, 15) && heard()) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_276_SWORD_SPIDER or BM_277_SPIDERLING: // stingers (including summons)
			if (is(monster.WalkframeCounter, 2, 5, 11) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1551, 1553) : RandSound(1611, 1613), monster.Row, monster.Col); // 1551-1553: light - dirt (3) // 1611-1613: light snow (3)
			break;
		case BM_278_LEAPER:
			if (is(monster.WalkframeCounter, 1, 9) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1574, 1577) : RandSound(1619, 1622), monster.Row, monster.Col); // med dirt Run----- // 1619-1622: med snow run (4)
			break;
		case BM_283_FROST_WARDEN:
			if (is(monster.WalkframeCounter, 9, 19) && heard()) PlayLocalSound(/*!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : */ RandSound(1832, 1835), monster.Row, monster.Col); // med stone Run----- // 1619-1622: med snow run (4)
			break;
		case BM_286_BRINE_CORPSE:
			if (is(monster.WalkframeCounter, 4, 14) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_287_ARMORED_SKELETON:
			if (is(monster.WalkframeCounter, 1, 10) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1590, 1593) : RandSound(1619, 1622), monster.Row, monster.Col); // 1590-1593: Med O Stone Run a.k.a. 'Big Skel' // (4) 1619-1622: med snow run (4)
			break;
		case BM_288_EARTH_GOLEM:
			if (is(monster.WalkframeCounter, 7, 17) && heard()) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS:
			if (is(monster.WalkframeCounter, 7, 15) && heard()) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_292_BUGBEAR:
			if (is(monster.WalkframeCounter, 1, 9) && heard()) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_297_OROG:
			if (is(monster.WalkframeCounter, 4, 13) && heard()) PlayLocalSound(RandSound(1558, 1561), monster.Row, monster.Col); // ginormous-----
			break;
		case BM_293_PIT_FIEND:
			if (is(monster.WalkframeCounter, 3, 8) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1570, 1573) : RandSound(1606, 1609), monster.Row, monster.Col); // heavy stone run----- // heavy snow run-----
			break;
		case BM_296_ICE_TROLL:
			if (is(monster.WalkframeCounter, 1, 11) && heard()) PlayLocalSound(RandSound(1026, 1029), monster.Row, monster.Col); // med sand
			if (is(monster.WalkframeCounter, 3, 13) && heard()) PlayLocalSound(RandSound(1839, 1842), monster.Row, monster.Col); // lght sand
			break;
		case BM_298_TENTACLIST_BOSS or BM_299_TENTACLIST2 or BM_300_TENTACLIST_ABYSS :
			if (is(monster.WalkframeCounter, 1, 7) && heard()) PlayLocalSound(!IsSnowStep(monsterIndex) ? RandSound(1539, 1542) : RandSound(1615, 1617), monster.Row, monster.Col); // med stone----- // 1615-1617: med snow (3)
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
		if( monster.WalkframeCounter == sprite.animation[ANIM_1_WALK].frameCount ){ // (rt) end of movement
			MonsterMap[ monster.Row ][ monster.Col ] = 0;
			monster.Row += monster.PrevActionOrRowDeltaOrSpellEffect;
			monster.Col += monster.TimerCounterOrColDeltaOrSpellDamage;
			MonsterMap[ monster.Row ][ monster.Col ] = monsterIndex + 1;
			if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
			}
			FixMonsterPosition(monsterIndex, monster.Orientation);
			result = 1;
		}else{                                                                     // (rt) movement between cells
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
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// (rt) If this is the end of the movement
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
	if( walkFrameCounter == monsterSprite.animation[ANIM_1_WALK].frameCount ){// (rt) If this is the end of the movement
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
	// (rt) there was a boss id here, but I'm sure it's the light radius. Everywhere else the light radius is checked, not the boss id
	if( !(monster.flag & MF_1_INVISIBLE) && monster.LightIndex ){// (rt) (7) boss number extension 0043DAD5
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
		if (RNG(100) < PerkValue(PERK_MERC_CRITS, playerIndex)) {
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
		attacker.LastTargetPositionRow = defender.Row;
		attacker.LastTargetPositionCol = defender.Col;
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

/*	if (defender.SpritePtr->baseMonsterIndex == BM_392_WARLORD_MERC && MaxCountOfPlayersInGame == 1) {
		damage -= (Dungeon->level / 2 + Difficulty * 12 + PerkValue(PERK_MERC_DFE, CurrentPlayerIndex)) << 6;
		LimitToMin(damage, 64);
		damage -= PerkValue(PERK_DAMAGE_SOAK, defenderIndex) * damage / 100;
	}*/
	// new version (previous commented above)
	if (defender.SpritePtr->baseMonsterIndex == BM_392_WARLORD_MERC && MaxCountOfPlayersInGame == 1) {
		if (defenderIndex < SummonMonsters_Count) {
			const int ownerIndex = defenderIndex / SummonMonstersPerPlayer_Count;
			damage -= ((Dungeon->level / 2 + Difficulty * 12 + PerkValue(PERK_MERC_DFE, ownerIndex)) << 6);
			LimitToMin(damage, 64);
			damage -= PerkValue(PERK_DAMAGE_SOAK, ownerIndex) * damage / 100;
		}
	}

	if( defender.newBossId ){
		damage /= 2;
	}
	if (defender.newBossId - 1  == UM_12_Uber_Diablo || defender.newBossId - 1 == UM_773_The_Dark_Lord) {
		damage = 2 * damage / 5; 
	} 
	if( DarkCampaignHephastoShielded( defenderIndex ) ){	// (th4) DC mission 15, Doom+: Hephasto takes 80 % less (same line in MvM_Ranged)
		damage /= 5;
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
			// (th4) THIS is the monster-vs-monster stun threshold, and it is the ONLY one a
			// merc or any summon can ever be subject to -- they fight monsters, and the
			// player never attacks his own minion. It shares NO code with the player-vs-
			// monster system (DamageMonsterByPlayer -> RollMonsterStunThreshold, 2x..6x AC
			// plus a 6-43%-of-HP trigger); reading that one for a minion gives numbers an
			// order of magnitude too high. The spellbook merc panel (SpellPanel.cpp) mirrors
			// the formula below -- keep the two in step.
			int merc_ST = MaxCountOfPlayersInGame == 1 ? PerkValue(PERK_INDOMITABLE_SPIRIT, CurrentPlayerIndex) : 0;
			if( /*is ( monsterSprite->baseMonsterIndex, BM_29_HIDDEN, BM_30_SHADOW, BM_31_UNSEEN, BM_32_NIGHTMARE) // hidden, always stunned
				// MvM stun formula: (if) damage >= mAC / 2 + MLVL + Rnd [ DLVL/4 + 6*DIFF + 2 ]
				||*/ (damage >> 6) >= monster.ArmorClass / 2 + monster.MonsterLevel + RNG(Dungeon->level / 4 + 6 * Difficulty + 2) + merc_ST ){ 
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
	// (th4) lifetime kill count: credit kills made by the player's own summons and by
	// traps. A wild monster killing another monster (attackerIndex >= SummonMonsters_Count)
	// is infighting, not the player's doing, so it isn't counted. Targets under
	// SummonMonsters_Count are the players' own summons dying -- also not a kill.
	if( targetIndex >= SummonMonsters_Count ){
		if( attackerIndex == M1_TRAP ){
			CreditMonsterKill( CurrentPlayerIndex );
		}else if( attackerIndex >= 0 && attackerIndex < SummonMonsters_Count ){
			CreditMonsterKill( attackerIndex / SummonMonstersPerPlayer_Count );
		}
	}

	delta_kill_monster(targetIndex, target.Row, target.Col, (DUNGEON)Dungeon);

	NetSendCmdLocParam2(0, CMD_36_MONSTDEATH, target.Row, target.Col, targetIndex, target.dungeon);
//	NetSendCmdLocParam2Direct(CMD_36_MONSTDEATH, target.Row, target.Col, targetIndex, target.dungeon);

    // (th4) DC mission 10: the escort succubi are friendly UNIQUES, so they live ABOVE
    // SummonMonsters_Count and the summon-only test below skipped them entirely -- every
    // hero they killed paid the player no experience at all. Credit their kills the same
    // way a summon's kill is credited. DC is single-player, so the owner is always
    // CurrentPlayerIndex; IsPlayerSideMonster already checks GM_DARK_CAMPAIGN + MF_6_FRIENDLY
    // + escort bossId, so no wild monster's infighting can slip through here.
    const bool escortKill = attackerIndex >= SummonMonsters_Count
                         && IsPlayerSideMonster( attackerIndex );
    if( attackerIndex != M1_TRAP && (attackerIndex < SummonMonsters_Count || escortKill) && !target.IsSummonedByMonster){
        const int ownerIndex = escortKill ? CurrentPlayerIndex : attackerIndex / SummonMonstersPerPlayer_Count;
        target.Attackers |= 1 << ownerIndex;
        AddXpForMonsterKill( target.MonsterLevel, target.Xp, target.Attackers, targetIndex );
    }
    ++TotalSlain[target.SpritePtr->baseMonsterIndex];
    target.CurrentLife = 0;
	if (target.LightIndex) {
		LightOff(target.LightIndex);
	}
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
		case BM_H_402_BERS_M_X:
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
		case BM_H_417_WAR_H_SS_WARRIOR_ELITE:
		case BM_H_414_WAR_H_MS:
		case BM_H_408_WAR_M_SS:
		case BM_H_404_WAR_M_MS:				melee(9, hit, 4 * min / 3, 4 * max / 3); break;
		case BM_H_407_MONK_M_X:				melee(14, hit, 2 * min, 2 * max); break;		
		case BM_H_416_MON_H_MS:
		case BM_H_410_MONK_H_SS:			melee(10, hit, 4 * min / 3, 4 * max / 3); break;
		case BM_H_422_WAR_H_ROYAL_GUARD:
		case BM_H_421_WAR_H_X_EXEC_ELITE:
		case BM_H_412_WAR_H_X_EXEC:			melee( 8, hit, 2 * min, 2 * max); break;	// (th4) Executioner: frame 8, double damage
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
	case PFC_GLADIATRIX:	maxBlockChance = 45; break;

	case PFC_SAVAGE:		
	case PFC_BERSERKER:		maxBlockChance = 20; break;
	case PFC_EXECUTIONER:	maxBlockChance = 10; break;
	case PFC_THRAEX:		maxBlockChance = 50; break;
	case PFC_MURMILLO:		maxBlockChance = 60; break;
	case PFC_DIMACHAERUS:	maxBlockChance = 0 + PerkValue(PERK_GEMINI, playerIndex); break;
	case PFC_SECUTOR:		maxBlockChance = HasTrait(playerIndex, TraitId::Scissor) ? 0 : 70; break;
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
		|| player.fullClassId == PFC_GLADIATRIX
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
	if (HasTrait(playerIndex, TraitId::Scissor)) {
		autoHit -= 2; // 18% effectively
	}
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
	else if (HasTrait(playerIndex, TraitId::Thief)) {
		autoHit += 8;
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
	playerBlockChance = player.blockChance	// (th4) class block bonus retired
		+ ((player.BaseDexterity + player.BaseStrength - monster.ArmorClass) / 2)
		- a_rose_with_thorns_trait_penalty
		+ two_towers_trait_benefit_1
		- PerkValue(SYNERGY_AGGRESSION, playerIndex, 0);
	if (HasTrait(playerIndex, TraitId::Scissor)) { 
		playerBlockChance = 0;
	}
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
		//(rt) return; // Ethereal in th1 gives no immortality against physical attacks (melee and arrows)
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
		return;// (rt) missed
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
		// (rt) the player was hit and couldn't block
		// (rt) removed the yellow zombie's max life drain altogether

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
		else if (GameMode == GM_CLASSIC || ( player.gameChanger & BIT (GC_29_ENCLAVE_SOLDIER ))) {
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
					(rt) realDamageToPlayer <<= 5;// large bonus
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
			realDamageToPlayer = realDamageToPlayer * (100 - PerkValue(SYNERGY_INVINCIBLE, playerIndex)) / 100;
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

		// (rt) 0043E4F9 reflect
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
		MonsterLifeSteal(realDamageToPlayer, monsterIndex, playerIndex); // (rt) in 1.145 moved to 0070B550 and reworked
		// (rt) this thing is unclear. The player was hit, so how can his current life be above max?
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
					// (rt) 00712960 fix for being thrown out of the light radius
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
		// (rt) 0070D7EF executioner mode
		/*if( MaxCountOfPlayersInGame != 1 ){ // executioner mode, turned off now. don't erase!
			if( Difficulty == 0 && player.CharLevel >= 32 
				|| Difficulty == 1 && player.CharLevel >= 46 ){
					(rt) realDamageToPlayer <<= 5;// large bonus
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

//----- (th4) --------------------------------------------------------
// Display-only estimate of a monster's melee chance to hit a player, as a 0-100 percent.
// Mirrors the deterministic part of MvP_Melee (AC, level diff, difficulty), then converts
// the engine's two-stage roll ( realToHitChance += RNG(range); hit iff RNG(100) < that )
// into an expected hit percentage. Blocking is intentionally NOT accounted for (raw to-hit).
// Not used for gameplay - only for the ALT cursor readout. Returns -1 if not applicable.
int __fastcall CalcMvP_MeleeHitChance( int monsterIndex, int playerIndex )
{
	if( (uint)monsterIndex >= Monsters_Max_Count || (uint)playerIndex >= PlayersMax_4 ){ return -1; }
	Player&  player  = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( !monster.SpritePtr || !monster.BasePtr ){ return -1; }

	int toHit = monster.ToHit; // primary attack to-hit (matches the common MvP_Melee call sites)

	int playerACsumm = player.ACFromItems + player.ACFromClass;
	if( !HasTrait(playerIndex, TraitId::Pyromaniac) ){
		int basic_AC_divider = HasTrait(playerIndex, TraitId::SmallFrame) ? 3 : 5;
		playerACsumm += player.CurDexterity / basic_AC_divider;
	}
	if( player.effectFlag[EA_AC_DEMON_MELEE]  && monster.BasePtr->MonsterClass == MON_1_DEMON  ){ playerACsumm += 20; }
	if( player.effectFlag[EA_AC_UNDEAD_MELEE] && monster.BasePtr->MonsterClass == MON_0_UNDEAD ){ playerACsumm += 15; }
	playerACsumm += (playerACsumm * player.spiciesAcPercent[monster.BasePtr->MonsterClass]) / 100;
	playerACsumm += player.spiciesAc[monster.BasePtr->MonsterClass];

	int lvlsDiffToHitbonus = 2 * (monster.MonsterLevel - player.CharLevel) + 20;
	int realToHitChance = toHit + lvlsDiffToHitbonus - playerACsumm;
	int range;
	if( Difficulty == DL_0_HORROR || GameMode == GM_CLASSIC ){
		range = 21;
	}else{
		realToHitChance -= Difficulty * 85 - 20;
		range = Difficulty * 95 + 11;
	}
	if( range < 1 ){ range = 1; }

	// autohit floor (min 20 baseline; MvP_Melee_Autohit_chance may raise it)
	int autoHit = 20, automiss = 100;
	MvP_Melee_Autohit_chance(playerIndex, autoHit, automiss);

	// Expected value of clamp( realToHitChance + U, 0, 100 ) / 100, U ~ uniform[0, range-1],
	// with the post-add value floored to autoHit. Integrated exactly over the discrete range.
	int hits = 0;
	for( int u = 0; u < range; ++u ){
		int v = realToHitChance + u;
		if( v < autoHit ) v = autoHit;   // LimitToMin(realToHitChance, autoHit)
		if( v > 100 ) v = 100;           // RNG(100) < v can never exceed 100
		if( v < 0 )   v = 0;
		hits += v;                       // P(RNG(100) < v) == v/100 for v in [0,100]
	}
	int percent = hits / range;          // == average of clamped v == expected percent
	// (th4) LITHE BUILD: the real melee attack (this file ~8165) rolls a SECOND time after the
	// hit roll passes, and RNG(100) > 91 turns 8 of 100 hits into misses. Mirror it here or
	// the tooltip overstates the monster's chance for Lithe Build characters. The ranged
	// replica (Draw.cpp CalcMvP_RangedHitChance_Display) applies the same factor; the real
	// ranged roll is Missile.cpp MvP_Ranged ~2640. Keep all four in step.
	if( HasTrait(playerIndex, TraitId::LitheBuild) ) percent = percent * 92 / 100;
	LimitToRange( percent, 0, 100 );
	return percent;
}

//----- (0043E705) --------------------------------------------------------
// (th4) DC mission 7: BM_H_405 is a hybrid whose DEFAULT ai (AI_16_Thunder_Demon) does melee +
// lightning and keeps its normal attack sounds. Gwideon's pack instead runs it under AI_25 (Mage),
// and that fireblast wants a different attack sound. The per-sprite sound slots are shared by every
// instance of the base, so this cannot be picked at load time -- it is decided per live monster, by
// the monster's actual ai. Plays the Mage attack sound positionally, loading it once into a cached
// slot. Local to this file: MonsterRangedAttack (just below) is the only caller.
static void PlayMageFireblastSound(int monsterIndex)
{
	static Sound mageAttackSound = {};
	if( ! mageAttackSound.name[0] ){
		strcpy(mageAttackSound.name, "DC\\Heroes\\Sorc_L_T\\MageFA1.wav");
	}
	Monster& monster = Monsters[monsterIndex];
	LoadAndPlaySoundFile(mageAttackSound, 1, monster.Row, monster.Col);
}

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
		// (th4) DC mission 7: a BM_H_405 running the Mage ai (Gwideon's pack) uses a dedicated
		// fireblast sound; the same base under its default AI_16 keeps the normal attack sound.
		if( monster.ai == AI_25_Advocate && monster.BasePtr && monster.BasePtr->id == BM_H_405_SOR_M_T_HYBRID ){
			PlayMageFireblastSound(monsterIndex);
		}else{
			PlayMonsterSound(monsterIndex, MST_0_ATTACK);
		}
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
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
				break;
			}
			else if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				uniqIndex = by(Difficulty, 184, 185, 186, 2389, 2390, 2391, 2392, 2393, 2394); // str line
			}
			else {
				switch (Players[CurrentPlayerIndex].ClassID) { // Veil of Steel rewards, Lachdanan rewards
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
			//(rt) row = col_1; // purpose unclear
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
		//(rt) monster.Row = row; // where did this come from? th1 doesn't have these lines
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
			--Monsters[boss.hisBossIndex].BossPackCount; // (rt) not used anywhere yet, unclear what it was planned for
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
	PlayMusic("Music\\RazB.ogg");
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
			// (th4) DARK CAMPAIGN has its own difficulty-advancing boss. Checked by unique id,
			// not base monster, so it is independent of whatever base sprite he ends up using.
			if( GameMode == GM_DARK_CAMPAIGN && monster.newBossId - 1 == UM_905_DC_Griswold ){
				UpdateAllowedDifficulty();
			}
			if( (MaxCountOfPlayersInGame == 1 && monster.SpritePtr->baseMonsterIndex == BM_137_UBER_DIABLO && Difficulty == DL_0_HORROR
			 && !is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON ))
				|| monster.SpritePtr->baseMonsterIndex == BM_C_389_DIABLO){
				int rowDelta = monster.Row - PlayerRowPos;
				PlayerRowPos += rowDelta >= 0 ? rowDelta > 0 : -1;
				int colDelta = monster.Col - PlayerColPos;
				PlayerColPos += colDelta >= 0 ? colDelta > 0 : -1;
				if( monster.PrevActionOrRowDeltaOrSpellEffect == 140 ){ // (rt) Diablo death animation timer?
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
// (th4) DC mission 8: Archimandrite Bozu and his kin are Shugoki (BM_H_407_MONK_M_X). When they
// run AI_8 (Fallen One) their warcry/rage secondary effect uses a dedicated sound instead of the
// normal special-attack sound. Same pattern as PlayMageFireblastSound: cached, positional, and
// local to this file (MonsterRangedHit just below is the only caller).
static void PlayShugokiRageSound(int monsterIndex)
{
	static Sound shugokiRageSound = {};
	if( ! shugokiRageSound.name[0] ){
		strcpy(shugokiRageSound.name, "sfx\\monk\\monk70.wav");
	}
	Monster& monster = Monsters[monsterIndex];
	LoadAndPlaySoundFile(shugokiRageSound, 1, monster.Row, monster.Col);
}

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
	if( Monsters[monsterIndex].CurFrame == Monsters[monsterIndex].BasePtr->SecondAtackToHitFrame ){
		// (th4) DC mission 8: a Shugoki (BM_H_407) running AI_8 uses a dedicated rage sound for its
		// warcry secondary effect; every other second-attack user keeps the normal special sound.
		Monster& m = Monsters[monsterIndex];
		if( m.ai == AI_8_Fallen_One && m.BasePtr && m.BasePtr->id == BM_H_407_MONK_M_X ){
			PlayShugokiRageSound(monsterIndex);
		}else{
			PlayMonsterSound(monsterIndex, MST_3_SATTACK);
		}
	}
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
	if( ! bossNumber ){ // (rt) (9) boss number extension 0043F877
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
	// (rt) path generation for a monster
	// (rt) monsters sometimes had negative coordinates here
	if( FindPath(stepFunc, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, steps) ){
			MonsterWalksTowardsTarget(monsterIndex, StepOrientation[steps[0]]);// (rt) So the full path to the target is built but only the first step is used, every time?                                        // there was wrong addressing
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
bool __fastcall Move_in_the_Direction_If_allowed( int monsterIndex, int orientation )
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
				Move_in_the_Direction_If_allowed( monsterIndex, dir );
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
// by (th4): true if this monster's current target is a dead player (corpse). Monsters
// keep the last acquired TargetIndex between MonsterLookAround passes; if that player
// dies mid-pursuit the AI would otherwise keep swinging at the corpse (MvP_Melee already
// blocks the damage, but the attack animation still plays). Used to suppress starting a
// melee/ranged attack on a dead player. Monster-vs-monster targets (MF_5_TARGET_MONSTER)
// are unaffected.
static bool IsMonsterTargetingDeadPlayer( const Monster& monster )
{
	if( monster.flag & MF_5_TARGET_MONSTER ){
		return false; // target is a monster, not a player
	}
	int targetIndex = monster.TargetIndex;
	if( (uint)targetIndex >= PlayersMax_4 ){
		return false; // not a valid player slot
	}
	return (Players[targetIndex].CurLife & (-64)) <= 0; // integer HP <= 0 => dead
}

void __fastcall StartMonsterMeleeAttack(int monsterIndex, int dir /*= -1*/)
{
	Monster& monster = Monsters[monsterIndex];
	if( IsMonsterTargetingDeadPlayer(monster) ){ return; } // by (th4): don't swing at a corpse
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
						// (th4) The DC Butcher walks like the merc -- he is the player's follower
						// there too. Walk only: the melee roll below is left alone.
						if (monster->SpritePtr->baseMonsterIndex == BM_392_WARLORD_MERC
						 || (GameMode == GM_DARK_CAMPAIGN && monster->newBossId - 1 == UM_9_The_Butcher)) { delayChance -= 15; }
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
					if(monster->SpritePtr->baseMonsterIndex == BM_392_WARLORD_MERC) { attack_chance += 30; }
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
		 && (monster.Row != 1 || monster.Col) ){ // (rt) condition for an inactive golem
			if( ! CheckLineWithThreeArgumentsCheckFunction( CheckCellForSomeMonsterSafeMove,
					monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)
			 || InInterval( LOBYTE_IDA(monster.pathCount), 4, 8 ) ){
				if( monster.flag & MF_10_OPEN_DOORS ){// (rt) Golem?
					MonstersInteractWithDoors(monsterIndex);
					if (monster.CurAction == A_4_MELEE)
					return 0;
				}
				if( ++LOBYTE_IDA(monster.pathCount) < 5 ){
					return 0;
				}
				if( MakeMonsterPath(monsterIndex) ){// (rt) Here a monster with zero coordinates was processed, which shouldn't happen (monster 95, with a count of 94)
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
	// (rt) Check whether the new direction differs from the current one
	if( deltaOrient == 4 ){// (rt) If opposite, change the direction to the new one at once
		monster.curOrientation = newOrientation;
	}else if( InInterval(deltaOrient, 4, 8) ){// (rt) Turning toward increasing orientation is shorter
		monster.curOrientation--;
	}else if( InInterval(deltaOrient, 0, 4) ){// (rt) Turning toward decreasing orientation is shorter
		monster.curOrientation++;
	}// (rt) Note. At 0 the current direction is kept unchanged
	Wrap(monster.curOrientation, 8);
	if( !Move_in_the_Direction_If_allowed(monsterIndex, monster.curOrientation) ){
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
	
	if( monster.goal == MG_2_RETREAT ){// (rt) If retreating
		// (rt) this attack type retreats by only one cell
		if( monster.goalParam ){// (rt) When the value is non-zero, move toward the player. 
			int newOrientation;
			if( RNG(2) ){
				newOrientation = OrientMinusOne[orientationToTarget];
			}else{
				newOrientation = OrientPlusOne[orientationToTarget];
			}
			MonsterWalksTowardsTarget(monsterIndex, newOrientation);
			monster.goal = MG_1_ATTACK;// (rt) Advance
		}else{// (rt) retreat. Move away from the player
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
		if( monster.TimerCounter > by(Difficulty,24, 18, 16, 14, 12, 11, 10, 9, 8) && walkWithDelayAllowed // (rt) After a 20-frame delay there is some chance of moving
			|| isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllowed ){// (rt) After moving, the chance of moving again is increased
				MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
		}
	}else if( meleeAttackAllowed ){// (rt) If we came up close and attacking is allowed
		StartMonsterMeleeAttack(monsterIndex);// (rt) hit and retreat
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
void __fastcall Ai_3_Skeleton_Archer(uint monsterIndex)
{
	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if (monster.CurAction) {
		return;
	}
	if (monster.ActivationCounter) { // gate AI actions on activation, but still fall through to refresh idle anim (fixes invisible-until-move)
		int orientationToTarget = MonsterOrientationToTarget(monsterIndex);
		monster.Orientation = orientationToTarget;
		int randFactor = RNG(100);

		bool walkWithDelayAllow = randFactor < by(Difficulty, 2 * monster.intel + (GameMode == GM_HARD ? 88 : 2),  // 94
			4 * monster.intel + (GameMode == GM_HARD ? 83 : 10),  // 95
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
			8 * monster.intel + (GameMode == GM_HARD ? 72 : 25)); // 96

		bool walkWithoutDelayAllow = randFactor < by(Difficulty, 8 * monster.intel + (GameMode == GM_HARD ? 73 : 11),  // 97
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
		if (Distance_To_Target_Is_Less_Than(monsterIndex, keep_distance)) {
			bool isAfterWalk = monster.PrevAction == A_1_WALK_UP || monster.PrevAction == A_2_WALK_DOWN || monster.PrevAction == A_3_WALK_HORIZ;
			if (monster.TimerCounter > 1 /*20*/ && walkWithDelayAllow || isAfterWalk && !monster.TimerCounter && walkWithoutDelayAllow) {
				moveAllow = Move_in_the_Direction_If_allowed(monsterIndex, OrientInverted[orientationToTarget]);
			}
		}
		if (!moveAllow) {
			if (RNG(100) < by(Difficulty, 2 * monster.intel + (GameMode == GM_HARD ? 40 : 2),
				2 * monster.intel + (GameMode == GM_HARD ? 60 : 17),
				2 * monster.intel + (GameMode == GM_HARD ? 63 : 19),
				2 * monster.intel + (GameMode == GM_HARD ? 66 : 21),
				2 * monster.intel + (GameMode == GM_HARD ? 69 : 23),
				2 * monster.intel + (GameMode == GM_HARD ? 72 : 25),
				2 * monster.intel + (GameMode == GM_HARD ? 75 : 27),
				2 * monster.intel + (GameMode == GM_HARD ? 78 : 29),
				2 * monster.intel + (GameMode == GM_HARD ? 80 : 32))) {
				if (CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)) {
					StartMonsterRangedAttack(monsterIndex, MI_0_NONE_ARROW, 4);
				}
			}
		}
	} // end ActivationCounter gate
	if (!monster.CurAction) {
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044062C) --------------------------------------------------------
int __fastcall StartMonsterRangedAttack(int monsterIndex, int spellEffect, int damage)
{
	Monster& monster = Monsters[monsterIndex];
	if( IsMonsterTargetingDeadPlayer(monster) ){ return 0; } // by (th4): don't fire at a corpse
	int orientation = MonsterOrientationToTarget(monsterIndex);
	// (th4) DC mission 7: Priest Gwideon and his kin (BM_H_405_SOR_M_T_HYBRID) cast their
	// fireblast on the second-attack animation (ANIM_5_SECOND, 12 frames) rather than the
	// primary attack pose. Every other ranged caster keeps ANIM_2_ATTACK.
	int rangedAnim = (monster.BasePtr && monster.BasePtr->id == BM_H_405_SOR_M_T_HYBRID) ? ANIM_5_SECOND : ANIM_2_ATTACK;
	SetMonsterAnimation(monsterIndex, &monster.SpritePtr->animation[rangedAnim], orientation);
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
void __fastcall Ai_1_Overlord(uint monsterIndex)
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
	if( IsMonsterTargetingDeadPlayer(monster) ){ return; } // by (th4): don't attack a corpse
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
	// (th4) The Hidden AI normally SHUTS OFF in full darkness (LightMax == full dark: it lurks
	// dormant in unlit corners). But a Hidden that has woken to hunt the THIEF must keep acting
	// in the dark -- the dark is where it hunts her. So skip this self-disable for an activated
	// Hidden while the Thief trait is in play; every other case keeps the vanilla lurk.
	bool thiefHunter = MaxCountOfPlayersInGame == 1 && monster->ActivationCounter
	                && HasTrait(CurrentPlayerIndex, TraitId::Thief);
	if( LightMap[ monster->Row ][ monsterCol ] == LightMax && !thiefHunter ){
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
	// (th4) Cleaned up from the decompiler output. Behaviour is unchanged; this is
	// purely de-obfuscation:
	//   - monsterIndex_1/_2/_3/_4 were ALL just `monsterIndex` -- collapsed.
	//   - `goal == 5` was MG_5_ATTACK2 spelled as a magic number.
	//   - the two gotos are gone: LABEL_47 only skipped an assignment (`ac = curAc`),
	//     and LABEL_15 only skipped the warcry+heal block. Both became plain structure.
	//   - declarations moved to first use (they were a C89-style block of IDA temps).
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[ monsterIndex ];

	// Post-warcry rush cooldown. Note this one floors at 0...
	if( monster.goal == MG_5_ATTACK2 ){
		if( monster.goalParam ){
			--monster.goalParam;
		}else{
			monster.goal = MG_1_ATTACK;
		}
	}

	if( monster.CurAction || !monster.ActivationCounter ){
		return;
	}

	// ...whereas the retreat timer decrements UNCONDITIONALLY and can reach -1, and it
	// tests the value from BEFORE the decrement. Asymmetric with the block above, but
	// that is the original behaviour -- do not "fix" it without checking in game.
	if( monster.goal == MG_2_RETREAT ){
		const int prevParam = monster.goalParam;
		monster.goalParam = prevParam - 1;
		if( !prevParam ){
			monster.goal = MG_1_ATTACK;
			FixMonsterPosition( monsterIndex, OrientInverted[ monster.Orientation ] );
		}
	}

	if( monster.goal == MG_5_ATTACK2 || RNG(35) ){
		if( monster.goal == MG_2_RETREAT ){
			// keep running, facing the way we already are
			MonsterWalksTowardsTarget( monsterIndex, monster.Orientation );
			return;
		}
		if( monster.goal != MG_5_ATTACK2 ){
			Ai_2_Skeleton_Melee( monsterIndex );
			return;
		}
		// rushing the target: swing if adjacent, otherwise close the distance
		if( abs( monster.Row - monster.TargetRow ) < 2 && abs( monster.Col - monster.TargetCol ) < 2 ){
			StartMonsterMeleeAttack( monsterIndex );
			return;
		}
		MonsterWalksTowardsTarget( monsterIndex, MonsterOrientationToTarget( monsterIndex ) );
		return;
	}

	// Warcry: heal self, then rally every nearby fallen into the rush state.
	if( !(monster.flag & MF_4_NOHEAL) ){
		StartMonsterSecondAttack_4_orBlock( monsterIndex, monster.Orientation );

		// On the lowest difficulty the heal is a flat per-level amount instead of the
		// monster's armor class. Levels other than 2/3/4 keep ArmorClass (this is what
		// the `goto LABEL_47` expressed: skip the override).
		int ac = monster.ArmorClass;
		if( (unsigned __int8) Difficulty < 1u ){
			if( Dungeon->level == 2 ){
				ac = 9;
			}else if( Dungeon->level == 3 ){
				ac = 12;
			}else if( Dungeon->level == 4 ){
				ac = 15;
			}
		}

		int minLife = (ac << 6) + monster.CurrentLife;
		if( monster.SpritePtr->baseMonsterIndex == BM_245_BLUNDERER ){
			minLife += (ac * 3) << 6;  // Warden of Anger gets 3x life from warcries!
		}
		LimitToMax( minLife, monster.BaseLife + (monster.BaseLife >> 2) );
		monster.CurrentLife = minLife;
	}

	const int radius = 2 * monster.intel + 4;
	for( int colOfs = -radius; colOfs <= radius; ++colOfs ){
		for( int rowOfs = -radius; rowOfs <= radius; ++rowOfs ){
			// (th4) DO NOT DELETE THIS TEST -- it looks wrong and it is not a bounds check
			// (the real one is below), but In112 casts to uint, so every NEGATIVE offset fails
			// it: this scan has only ever covered the south-east quadrant.  Removing it makes
			// the warcry rally roughly four times as many Fallen. That may well be the original
			// intent, but it is a balance change and must be a deliberate one.
			if( !In112( colOfs, rowOfs ) ){
				continue;
			}
			// (th4) The actual bounds check, on the real coordinates. The old code had none:
			// it tested the offsets above and then indexed with monster.Row/Col PLUS them,
			// through a flat MonsterMap[0][col + 112*row], which hid two defects.
			//   1. Off the end. A warcrier near the south or east edge read past
			//      MonsterMap[112][112], took whatever array follows as a monster number and
			//      dereferenced Monsters[thatGarbage - 1]. From the crash dump: a Fallen One
			//      with intel 7 (radius 18) at row 94 -> index 12584 of 12544, value
			//      0x0D0D0D5D, faulting read at 0x451BDBD0.
			//   2. Column wrap. With monster.Col + colOfs >= 112 the flat index stayed inside
			//      the array but addressed the NEXT row -- a silently wrong tile, no crash.
			// Indexing two-dimensionally below keeps the guard and the access naming the same
			// two values, so neither can come back.
			const int scanRow = monster.Row + rowOfs;
			const int scanCol = monster.Col + colOfs;
			if( !In112( scanRow, scanCol ) ){
				continue;
			}
			const int monNum = MonsterMap[ scanRow ][ scanCol ];
			if( monNum <= 0 ){
				continue;
			}
			Monster& other = Monsters[ monNum - 1 ];
			if( other.ai == AI_8_Fallen_One ){
				other.goal = MG_5_ATTACK2;
				other.goalParam = 30 * monster.intel + 64;
			}
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

// True for monsters whose PRIMARY attack is ranged (pure "ranger/shooter" AIs that
// kite via RangerRetreatsOften, plus skeleton/goat archers, succubus, acid ranger).
// Melee+ranged hybrids and exploders are intentionally excluded.
bool IsShooterAi(int ai)
{
	switch (ai) {
	case AI_3_Skel_Archer:
	case AI_7_Goat_Archer:
	case AI_14_Witch:               // succubus
	case AI_20_Acid_Beast_Ranger:
	case AI_32_Ranger_Firebolt:
	case AI_33_Ranger_Lite_Arrow:
	case AI_35_Lich:
	case AI_36_Arch_Lich:
	case AI_37_Ranger_Fire_Arrow:
	case AI_38_Beholder:
	case AI_41_Ranger_snowbolt:
	case AI_43_Ranger_Acid_Arrow:
	case AI_44_Ranger_Cold_Arrow:
	case AI_45_Ranger_Arcane_Arrow:
	case AI_46_Ranger_Blue_Star:
	case AI_49_Ranger_Fireblast:
	case AI_55_Wyrm:
		return true;
	default:
		return false;
	}
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
		// Warlord's mercs face less kiting from monsters..
		if (MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Warlord)) {
			retreat_chances -= (20 + Difficulty * 3);
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
	if (monster.ActivationCounter < 255) {// (rt) TODO : always true
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
			&& CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)) { // (rt) charge attack
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
	if( IsMonsterTargetingDeadPlayer(monster) ){ return 0; } // by (th4): don't cast at a corpse
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
void __fastcall Ai_14_Succubus(uint monsterIndex)
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
	int missile = MI_109_ACID_ARROW;
	if (Dungeon == DUN_116_MINING_CAVES) {
		missile = MI_56_LIGHTING_ARROW;
	}
	RangerRetreatsOften(monsterIndex, missile, 0);
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
void __fastcall Ai_20_Acid_Beast_Ranger(uint monsterIndex)
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
	if (Dungeon == DUN_116_MINING_CAVES) {
		mis_type = MI_52_CHARGED_BOLT;
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
	// (rt) looking for a corpse (finding corpse)
	if( !monster.mayRetreateTick ){
		Ai_2_Skeleton_Melee(monsterIndex);
		return;
	}
	monster.mayRetreateTick--;
	if( DeathMonstersMap[ monster.Row ][ monster.Col ] ){
		// (rt) already standing on a corpse
		StartMonsterSecondAttack_1_2(monsterIndex);
		if( !(monster.flag & MF_4_NOHEAL) ){
			int healedPoints = 1;// (rt) = monster.BaseLife >> 4; // /16, used to be /8
			if (GameMode == GM_IRONMAN || GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN) {
				healedPoints = monster.BaseLife / 60;
			}
			else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				healedPoints = monster.BaseLife / 70;
			}
			else{
				healedPoints = monster.BaseLife / 50;// Hard, Normal, Survival
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
			// (rt) looking for a corpse
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
			rowOffset -= dir;// (rt) compensate for the extra increment before leaving the loop
			colOffset -= dir;
			if( isCorpseFound ){
				monster.goalParam = rowOffset + monster.Row + 1;
				monster.curOrientation = colOffset + monster.Col + 1;
			}
		}
		// (rt) walk to the corpse
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
		case GM_OUTBREAK:
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
void __fastcall Ai_9_Magma(uint monsterIndex)
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
	if (Dungeon == DUN_116_MINING_CAVES) {
		switch (RNG(4)) {
			case 0:	missileIndex = MI_22_MONSTER_LIGHTNING; break;
			case 1:	missileIndex = MI_6_FIREBLAST; break;
			case 2:	missileIndex = MI_129_SNOW_BOLT; break;
			case 3:	missileIndex = MI_24_ARCANE_STAR; break;
		}
	}
	Universal_RangeSecondAttack(monsterIndex, missileIndex, 1, damage, 0);
}

//----- (th3) -------------------------------------------------------------
void __fastcall Ai_50_Hybrid_Fireblast(uint monsterIndex)
{
	int damage = 13 + Dungeon->level * 3 + Difficulty * 85; // this doesnt seem to work, needs check!	
	Universal_RangeSecondAttack(monsterIndex, MI_6_FIREBLAST, 1, damage, 0);
}

//----- (00441E91) --------------------------------------------------------
void __fastcall Ai_16_Lightning_Demon(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex]; 
	if (GameMode == GM_DARK_CAMPAIGN && monster.SpritePtr->baseMonsterIndex == BM_H_405_SOR_M_T_HYBRID && monster.CurrentLife < 9 * monster.BaseLife / 10) {
		RangerRetreatsOften(monsterIndex, MI_22_MONSTER_LIGHTNING, 1);
	}
	else {
		Universal_RangeSecondAttack(monsterIndex, MI_22_MONSTER_LIGHTNING, 1, 4, 0);
	}
}

//----- (00441EA0) --------------------------------------------------------
void __fastcall Ai_39_Hybrid_Physical_Blue_Blast(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_102_BLUE_BLAST_2, 1, 4, 0);
}

//----- (new) --------------------------------------------------------
void __fastcall Ai_40_Hybrid_ColdBlast(uint monsterIndex)
{	
	int missile = MI_127_HABIBI;
	if (Dungeon == DUN_52_MOSSY_GROTTO) {
		missile = RngFrom(MI_1_FIREBOLT, MI_24_ARCANE_STAR, MI_52_CHARGED_BOLT);
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

//----- (new) --------------------------------------------------------
void __fastcall Ai_52_Hybrid_SnowBolt(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_129_SNOW_BOLT, 1, 4, 0);
}

//----- (Doom_Imp) --------------------------------------------------------
void __fastcall Ai_42_Hybrid_Firebolt(uint monsterIndex)
{
	int mis_type = MI_1_FIREBOLT;
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		mis_type = MI_129_SNOW_BOLT;
	}
	Universal_RangeSecondAttack(monsterIndex, mis_type/*MI_129_DOOM_IMP*/, 1, 4, 0);
}

//----- (Lord_Obese) --------------------------------------------------------
void __fastcall Ai_47_Hybrid_Bluestar(uint monsterIndex)
{
	int missile(MI_100_RED_BLAST);
	if (Dungeon == DUN_9_CAVE_1 && MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		missile = MI_129_SNOW_BOLT; // only Lord Obese uses this attack on dlvl-9
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

//----- (Fearshredder) --------------------------------------------------------
void __fastcall Ai_51_Hybrid_Arcanestar(uint monsterIndex)
{
	int missile(MI_24_ARCANE_STAR);
	if (MaxCountOfPlayersInGame == 1 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest) {
		missile = MI_131_ARCANE_STAR_BLUE;
	}
	Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
}

void __fastcall Ai_58_Hybrid_Psychic(uint monsterIndex)
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
	if (not(GameMode, GM_EASY, GM_CLASSIC, GM_IRONMAN) && monster.CurrentLife < monster.BaseLife / 5) {
		Ai_14_Succubus(monsterIndex);
	}
	else {
		Universal_RangeSecondAttack(monsterIndex, missile, 1, 4, 0);
	}
}

//----- (Vobul) --------------------------------------------------------
void __fastcall Ai_48_Hybrid_Yellowstar(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_101_YELOW_BLAST, 1, 4, 0);
}

//----- (00441EAF) --------------------------------------------------------
void __fastcall Ai_19_Acid_Beast(uint monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	MonsterSprite& monsterSprite = MonsterSprites[monster.SpriteIndex];
	if (monster.SpritePtr->baseMonsterIndex == BM_230_BEASTMASTER_SUMMON_2) {
		goto REGULAR;
	}
	else if (not(GameMode, GM_EASY, GM_CLASSIC, GM_IRONMAN ) && monster.CurrentLife < monster.BaseLife / 5) {
		RangerRetreatsOften(monsterIndex, MI_57_ACID_MISSILE, 1);
	}
	else {
		REGULAR:
		Universal_RangeSecondAttack(monsterIndex, MI_57_ACID_MISSILE, 0, 4, 1);
	}
}

//----- (Scarab) --------------------------------------------------------
void __fastcall Ai_54_Hybrid_Charged_Bolt(uint monsterIndex)
{
	Universal_RangeSecondAttack(monsterIndex, MI_52_CHARGED_BOLT, 1, 4, 0);
}

//----- (00441EBE) --------------------------------------------------------
void __fastcall Ai_27_Diablo(uint monsterIndex)
{
	// (rt) insert new code
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
// Invisible Stalker: permanently invisible except within VISIBLE_RANGE tiles of
// its target. Circles and harasses (50/50 spit MI_57 / charge MI_20), both using
// the PRIMARY attack animation. After a charge: HP>50% -> retreat & re-hide and
// resume circling; HP<=50% -> melee-lock, delegating to AI#2 until HP climbs back
// above 50%. Visibility is toggled INSTANTLY via MF_1_INVISIBLE (no fade anim).
void __fastcall Ai_60_Invisible_Stalker(uint monsterIndex)
{
	// --- tunables ---------------------------------------------------------
	const int VISIBLE_RANGE = 3; // visible when Chebyshev dist < 3, i.e. <=2 tiles
	// ("one tile in-between, or less"). Lower to 2 for
	// melee-only visibility. THIS is the number to tweak.
	const int SPIT_DAMAGE = 4; // acid missile damage tier, as other spitters use
	const int ATTACK_MIN_RANGE = 5; // charge AND spit only when target is >= this
	// far away. Closer than this -> keep circling.
// ----------------------------------------------------------------------

	if (monsterIndex >= Monsters_Max_Count) {
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if (monster.CurAction || !monster.ActivationCounter) {
		return;
	}

	bool hpAboveHalf = monster.CurrentLife * 2 > monster.BaseLife;

	// --- instant visibility (no fade animation) ---------------------------
	// Visible at/within VISIBLE_RANGE; invisible otherwise. Toggle the flag
	// directly so there is no ANIM_5_SECOND appear/vanish animation.
	bool inSight = Distance_To_Target_Is_Less_Than(monsterIndex, VISIBLE_RANGE);
	if (inSight) {
		if (monster.flag & MF_1_INVISIBLE) {
			monster.flag &= ~MF_1_INVISIBLE;
			if (monster.newBossId) { // restore boss light when it pops into view
				ChangeLightPos(monster.LightIndex, monster.Row, monster.Col);
			}
		}
	}
	else {
		if (!(monster.flag & MF_1_INVISIBLE)) {
			monster.flag |= MF_1_INVISIBLE;
			if (monster.newBossId) { // kill boss light while hidden
				ChangeLightPos(monster.LightIndex, 0, 0);
			}
		}
	}

	int orientationToTarget = OrientationToTarget(monster.Row, monster.Col,
		monster.LastTargetPositionRow, monster.LastTargetPositionCol);

	if (monster.ActivationCounter < 255) {
		MonstersInteractWithDoors(monsterIndex);
	}

	// --- HP-driven mode transitions ---------------------------------------
	// Melee-lock engages the moment HP drops to/under half, from any state;
	// it releases (back to circle/harass) once HP recovers above half.
	if (!hpAboveHalf) {
		monster.goal = MG_5_ATTACK2; // melee-lock
	}
	else if (monster.goal == MG_5_ATTACK2) {
		monster.goal = MG_1_ATTACK;  // recovered: resume stalking
		monster.goalParam = 0;
	}

	// --- MELEE-LOCK: hand off entirely to AI#2 ----------------------------
	if (monster.goal == MG_5_ATTACK2) {
		Ai_2_Skeleton_Melee(monsterIndex);
		return;
	}

	// --- RETREAT: run away from target, then re-hide and resume circling --
	if (monster.goal == MG_2_RETREAT) {
		// far enough away (beyond visibility, plus margin) -> back to stalking
		if (!Distance_To_Target_Is_Less_Than(monsterIndex, VISIBLE_RANGE + 3)
			|| monster.goalParam > 8) {
			monster.goalParam = 0;
			monster.goal = MG_1_ATTACK;
		}
		else {
			monster.Orientation = OrientInverted[orientationToTarget]; // away
			++monster.goalParam;
			MonsterWalksTowardsTarget(monsterIndex, monster.Orientation);
			return;
		}
	}

	// --- ATTACK / STALK: circle, then 50/50 spit-or-charge ----------------
	// While healthy (HP > 50%) the stalker NEVER settles into melee. If circling
	// brings it adjacent, it skips the rest of the circle and commits to an
	// attack immediately rather than biting. Melee only ever happens in the
	// HP<=50% lock above (handed to AI#2), which returns before reaching here.
	bool adjacent = Distance_To_Target_Is_Less_Than(monsterIndex, 2);

	// Only commit to an attack when the target is far enough (>= ATTACK_MIN_RANGE).
	// If closer than that, keep circling/repositioning instead of firing point-blank.
	bool farEnough = !Distance_To_Target_Is_Less_Than(monsterIndex, ATTACK_MIN_RANGE);

	if (!farEnough || (!adjacent && monster.goalParam < 6)) {
		// too close to attack, or still circling before commit
		++monster.goalParam;
		if (!MonsterCircleTarget(monsterIndex, orientationToTarget, monster.curOrientation)) {
			MonsterStartDelay(monsterIndex, RNG(10) + 5);
		}
		return;
	}
	monster.goalParam = 0;

	// commit: 50/50 charge vs spit. Both use the PRIMARY attack animation.
	// (Target is guaranteed >= ATTACK_MIN_RANGE here, so neither fires point-blank.)
	if (RNG(2)) {
		// CHARGE (like horned beasts): MI_20_RUN_ATTACK, A_14_CHARGE, primary anim.
		if (CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove,
			monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)
			&& CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol,
				orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1) {
			if (monster.BasePtr->HasSpecialSounds) {
				PlayMonsterSound(monsterIndex, MST_3_SATTACK);
			}
			MonsterMap[monster.Row][monster.Col] = -1 - int(monsterIndex);
			monster.CurAction = A_14_CHARGE;
			// after the charge resolves: HP<=50% -> melee-lock. HP>50% -> ROLL the
			// retreat chance (formula + modifiers lifted from RangerRetreatsOften):
			// pass -> retreat & re-hide; fail -> just resume circling, no retreat.
			if (!hpAboveHalf) {
				monster.goal = MG_5_ATTACK2; // melee-lock
			}
			else {
				int retreat_chances = 2 * monster.intel + 70 + (Difficulty * 3)
					- (MaxCountOfPlayersInGame == 1 ? 5 : 0);
				/*if (GameMode == GM_CLASSIC) {
					retreat_chances = monster.intel * 10 + 70;
				}
				else */if (GameMode == GM_EASY) {
					retreat_chances -= 10;
				}
				else if (GameMode == GM_IRONMAN || GameMode == GM_SPEEDRUN || GameMode == GM_SURVIVAL) {
					retreat_chances -= 5;
				}
				else if (GameMode == GM_HARD) {
					retreat_chances = 100;
				}
		//		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_8_FULL_CONTACT)) {
		//			retreat_chances = 0; // Full Contact: no retreat
		//		}
				// Warlord's mercs face less kiting from monsters
				if (MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Warlord)) {
					retreat_chances -= (15 + Difficulty * 2);
				}
				monster.goal = (RNG(100) < retreat_chances) ? MG_2_RETREAT : MG_1_ATTACK;
			}
			monster.goalParam = 0;
			return;
		}
		// no clear charge lane -> fall through to spit this tick
	}

	// SPIT poison: MI_57_ACID_MISSILE via StartMonsterRangedAttack -> PRIMARY anim.
	if (CheckCastInSight(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol)) {
		StartMonsterRangedAttack(monsterIndex, MI_57_ACID_MISSILE, SPIT_DAMAGE);
	}
	else {
		// no line of sight to spit: close the gap a little
		MonsterWalksTowardsTarget(monsterIndex, orientationToTarget);
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
			RNG(2);// (rt) TODO: the result isn't used
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
void __fastcall Ai_26_Balrog(uint monsterIndex)
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
    
    // (th4) WARLORD MERCS FOLLOW THEIR OWNER WHEN THEY HAVE NOTHING TO FIGHT.
    //
    // IsSomeMonstersActivated() is GLOBAL: it answers "is any monster on the level
    // awake", not "is there a fight near me". So the moment anything anywhere wakes up
    // -- including the pack on the far side of a wall -- this whole follow block is
    // skipped and a merc with no reachable target just stands where it is.
    // Since the sight/reach filter in MonsterLookAround now denies Warlord mercs those
    // unreachable targets, they end up with MF_11_NOENEMY and nothing to do, which is
    // exactly the standing-still the fix was meant to avoid.
    //
    // For a Warlord merc the gate is therefore "have I got a target", not "is the level
    // quiet". Everyone else keeps IsSomeMonstersActivated() untouched.
    // Bounded even though the only caller already guards monsterIndex <
    // SummonMonsters_Count: this now runs on EVERY call, not just inside the block
    // below, so a future caller passing a non-summon must not index Players out of range.
    const int summonOwnerIndex = monsterIndex / SummonMonstersPerPlayer_Count;
    const bool isWarlordMerc = (uint) summonOwnerIndex < PlayersMax_4
                            && HasTrait( summonOwnerIndex, TraitId::Warlord );
    const bool freeToFollow  = isWarlordMerc ? (monster.flag & MF_11_NOENEMY) != 0
                                             : !IsSomeMonstersActivated();
    if( monster.CurAction == A_0_STAY && freeToFollow ){
        int ownerIndex = summonOwnerIndex;
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
    
    // (th4) THE WAY TO THE FIGHT (above SummonWayToNearest): the target is out of sight -- its way's next step, not the
    // fight AI (it walks straight at the target, into the wall); MonsterLookAround after the step finds the way again
    if( !(monster.flag & MF_11_NOENEMY) && SummonFarStep[ monsterIndex ] ){
        if( monster.CurAction == A_0_STAY ){
            MonsterWalksTowardsTarget( monsterIndex, SummonFarStep[ monsterIndex ] - 1 );
        }
        return;
    }

    // Use AI only if target selected
    if( !(monster.flag & MF_11_NOENEMY) ){
        (*original)( monsterIndex );
    }
}

//----- (th4) -------------------------------------------------------------
// (th4) DARK CAMPAIGN mission 10 escort AI wrapper -- the escort's answer to Ai_Summon.
// The main loop re-runs the AI at the top of every iteration of its while(moreAction) loop,
// which is how a summon starts its next step the instant the previous one lands: that is
// what makes minions stride smoothly. Doing the follow once per tick outside the loop
// instead leaves a dead tick after every tile, which reads as a stutter.
//
// This is deliberately NOT Ai_Summon itself: that derives the owner from the summon slot
// index (meaningless for a unique above SummonMonsters_Count) and teleports the minion to
// its owner past 13 tiles, which is exactly what we do not want here.
void __fastcall Ai_DarkCampaignEscort( uint monsterIndex, AiFunc* original )
{
	if( monsterIndex >= Monsters_Max_Count ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if( monster.Row == 1 && monster.Col == 0 ){
		return;
	}
	if( !monster.ActivationCounter ){
		monster.ActivationCounter = 255;
		MonsterLookAround( monsterIndex );
	}
	// Close on the player whenever there is no fight on -- same gate Ai_Summon uses.
	if( monster.CurAction == A_0_STAY && !IsSomeMonstersActivated() ){
		const Player& owner = Players[CurrentPlayerIndex];
		int gap = max( abs(monster.Row - owner.Row), abs(monster.Col - owner.Col) );
		if( gap > DC_ESCORT_FOLLOW_GAP ){
			monster.TargetRow = (char)owner.Row;	// char field, coords <= 111
			monster.TargetCol = (char)owner.Col;
			if( MakeMonsterPath( monsterIndex ) ){
				return;
			}
			// Path refused (too long, or blocked by the pack) -- take a plain step instead.
			if( MonsterWalksTowardsTarget( monsterIndex,
					OrientationToTarget( monster.Row, monster.Col, owner.Row, owner.Col ) ) ){
				return;
			}
		}
	}
	// Otherwise behave exactly as the succubus normally does: fight.
	(*original)( monsterIndex );
}

//----- (th4) -------------------------------------------------------------
// (th4) DARK CAMPAIGN mission 25: the Butcher's AI once the player has freed him.
//
// Deliberately NOT Ai_DarkCampaignEscort: the succubi hand control to their own AI the
// moment ANYTHING on the level is awake, which for him meant permanent combat -- and the
// underlying AI picks targets by raw distance, so he charged heroes through walls and stood
// grinding at the stone between them.
//
// Here he only fights what he can actually see: a hero must be awake (the player aggroed it)
// AND in line of sight AND within DC_BUTCHER_SIGHT tiles. Anything else and he closes on the
// player instead. Everything below is Dark Campaign only -- the caller gates on it too.
const int DC_BUTCHER_SIGHT = 12;	// tiles he will engage across, when he can see them
const int DC_BUTCHER_REACH_TRIES = 3;	// how many candidates he will path-test in one tick

// (th4) Can he actually WALK to that tile? Line of sight is not enough: Ravenholm is a town
// map full of fences, water and hedges, where plenty of heroes are perfectly visible across
// something he cannot cross. Without this he picks one, walks to the barrier and grinds
// against it for good. FindPath is the engine's own 28-step search and the same one
// MakeMonsterPath uses, so "reachable" here means exactly what it means when he walks.
static bool DarkCampaignButcherCanReach( uint monsterIndex, int row, int col )
{
	const Monster& m = Monsters[monsterIndex];
	char steps[28];
	auto stepFunc = (m.flag & MF_10_OPEN_DOORS)
		? CheckCellForMonsterCanInteractWithDoorsSafeMove
		: CheckCellForMeleeMonsterSafeMove;
	return FindPath( stepFunc, monsterIndex, m.Row, m.Col, row, col, steps ) != 0;
}

void __fastcall Ai_DarkCampaignButcher( uint monsterIndex, AiFunc* original )
{
	if( monsterIndex >= Monsters_Max_Count || GameMode != GM_DARK_CAMPAIGN ){
		return;
	}
	Monster& monster = Monsters[monsterIndex];
	if( monster.Row == 1 && monster.Col == 0 ){
		return;
	}
	if( !monster.ActivationCounter ){
		monster.ActivationCounter = 255;
	}
	// (th4) First tick on a mission level he was carried to: CreateBoss dropped him on a
	// random free tile, so pull him to the player instead. The hub does the same thing from
	// its kin loop; an ordinary level has no such loop, so it happens here. One-shot.
	if( DarkCampaignButcherNeedsArrival && Dungeon != DUN_117_DARKLORD_HIDEOUT ){
		DarkCampaignButcherNeedsArrival = false;
		TeleportSummonToOwner( monsterIndex, CurrentPlayerIndex );
	}

	// Nearest awake hostile he can both see AND reach. Candidates are taken closest first;
	// each is path-tested, and one that cannot be walked to is skipped rather than chased.
	// The number of tests per tick is capped -- FindPath is not free, and the rest of the
	// roster is still there next tick.
	int quarry = -1;
	int rejected[DC_BUTCHER_REACH_TRIES] = { -1, -1, -1 };
	for( int attempt = 0; attempt < DC_BUTCHER_REACH_TRIES && quarry < 0; ++attempt ){
		int best = -1;
		int bestGap = DC_BUTCHER_SIGHT + 1;
		for( int i = 0; i < MonstersCount; ++i ){
			const int mi = MonsterIndexes[i];
			if( mi == (int)monsterIndex || mi < SummonMonsters_Count ) continue;
			bool skip = false;
			for( int r = 0; r < attempt; ++r ) if( rejected[r] == mi ) skip = true;
			if( skip ) continue;
			const Monster& foe = Monsters[mi];
			if( !foe.SpritePtr || (foe.flag & MF_15_KILLED) ) continue;
			if( foe.Row == 1 && foe.Col == 0 ) continue;
			if( IsPlayerSideMonster( mi ) || (foe.flag & MF_6_FRIENDLY) ) continue;
			if( !foe.ActivationCounter ) continue;	// asleep: the player has not aggroed it
			const int gap = max( abs(monster.Row - foe.Row), abs(monster.Col - foe.Col) );
			if( gap >= bestGap ) continue;
			if( !CheckCastInSight( monster.Row, monster.Col, foe.Row, foe.Col ) ) continue;
			best = mi;
			bestGap = gap;
		}
		if( best < 0 ) break;	// nothing visible left to consider
		// Already next to him: no walking needed, so no path test.
		if( bestGap <= 1 || DarkCampaignButcherCanReach( monsterIndex, Monsters[best].Row, Monsters[best].Col ) ){
			quarry = best;
			break;
		}
		rejected[attempt] = best;	// visible but walled off -- look past him
	}

	if( quarry >= 0 ){
		// Something to kill and a clear line to it: his own AI takes over and does the
		// pathing, so he walks round corners rather than into them.
		monster.TargetIndex = quarry;
		monster.LastTargetPositionRow = Monsters[quarry].Row;
		monster.LastTargetPositionCol = Monsters[quarry].Col;
		(*original)( monsterIndex );
		return;
	}

	// Nothing in sight: stay with the player. TargetIndex is deliberately LEFT ALONE -- it is
	// dereferenced unguarded in several places (ProcessMonsters, MonsterStaying), so a -1
	// there is a wild read. Stale is harmless, because his own AI only runs on the branch
	// above, where the target has just been set to something he can actually see.
	if( monster.CurAction == A_0_STAY ){
		const Player& owner = Players[CurrentPlayerIndex];
		const int gap = max( abs(monster.Row - owner.Row), abs(monster.Col - owner.Col) );
		if( gap > DC_ESCORT_FOLLOW_GAP ){
			monster.TargetRow = (char)owner.Row;	// char field, coords <= 111
			monster.TargetCol = (char)owner.Col;
			if( MakeMonsterPath( monsterIndex ) ){
				return;
			}
			// Path refused (too long, or blocked) -- take a plain step instead.
			if( MonsterWalksTowardsTarget( monsterIndex,
					OrientationToTarget( monster.Row, monster.Col, owner.Row, owner.Col ) ) ){
				return;
			}
		}
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
				goto ATTACK; // (rt) sometime figure out how to fold this into the condition
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
void __fastcall Ai_5_Charger(uint monsterIndex)
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
	if( monster.ActivationCounter < 255 ){// (rt) TODO : always true
		MonstersInteractWithDoors(monsterIndex);
	}
	int randFactor = RNG(100);
	int intel = monster.intel;
	int charge_chance = 20 + intel + Dungeon->level / 6 + Difficulty * 9;// 20-33, 29-42, 38-51, 47-60, 56-69, 65-78, 73-86, 81-94, 90-103
	if (Dungeon == DUN_113_THE_MAZE) { charge_chance = 20; }
	if (GameMode == GM_EASY) { charge_chance /= 2; }
	else if (GameMode == GM_HARD) { charge_chance = 100; }
	bool runAttackAllow  = randFactor < charge_chance;
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
		 && CheckLineWithThreeArgumentsCheckFunction(CheckCellForMeleeMonsterSafeMove, monsterIndex, monster.Row, monster.Col, monster.TargetRow, monster.TargetCol) ){ // (rt) charge attack
			if( CastMissile(monster.Row, monster.Col, monster.TargetRow, monster.TargetCol, orientationToTarget, MI_20_RUN_ATTACK, monster.TargetIndex, monsterIndex, 0, 0, 0) != -1 ){
				if( monster.BasePtr->HasSpecialSounds ){
					PlayMonsterSound(monsterIndex, MST_3_SATTACK);
				}
				MonsterMap[ monster.Row ][ monster.Col ] = -1 - int(monsterIndex);
				monster.CurAction = A_14_CHARGE;
			}
			return;
		}
		if( !Distance_To_Target_Is_Less_Than(monsterIndex, 2) ){// (rt) movement
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
		if( meleeAttackAllow ){// melee
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
				else if (GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_83_BLOOD_KEEP) {
					missile = by(monster.intel, MI_1_FIREBOLT, MI_24_ARCANE_STAR, MI_7_LIGHTNING, MI_6_FIREBLAST);
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
				// by (th4): the flash CastMissile calls below are not routed through a
				// StartMonster*Attack guard, so a dead-player target would still get flashed
				// (corpse-flashing). Suppress the whole flash cast when the target is dead.
				if( !IsMonsterTargetingDeadPlayer(monster) ){
					StartMonsterRangedAttack(monsterIndex, -1, 0);
					CastMissile( monster.Row, monster.Col, 0, 0, monster.Orientation, MI_11_FLASH_FR, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0, 0 );
					CastMissile( monster.Row, monster.Col, 0, 0, monster.Orientation, MI_12_FLASH_BACK, CT_1_MONSTER_AND_TRAP, monsterIndex, 4, 0, 0 );
				}
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
		monster.speechIndex = 0; // (rt) Lazarus patch from D1 1.07-1.09
		Ai_25_Mage(monsterIndex); // (rt) Lazarus reuses the Counselor behaviour
	}
	monster.Orientation = orientation;
	if( !monster.CurAction || monster.CurAction == A_17_TALK ){
		monster.curAnimation = &monsterSprite.animation[ANIM_0_STAY].oriented[monster.Orientation];
	}
}

//----- (0044351B) --------------------------------------------------------
void __fastcall Ai_29_Redvex_Blackjade(uint monsterIndex)
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
	// (th4) DARK CAMPAIGN escort: this AI re-derives the goal from the Lazarus quest every
	// tick, which would drag the succubi back onto MG_6_TALK no matter what CreateBoss set.
	// While they are escorting the player they always fight.
	const bool dcEscort = GameMode == GM_DARK_CAMPAIGN && monster.newBossId
					   && DarkCampaignIsEscortBoss( monster.newBossId - 1 )
					   && (DarkCampaignEscortActive() || DarkCampaignEscortWalkingHome);
	// (th4) DC mission 13: same problem, different mission. dcEscort above is gated on the
	// mission 10 flags, so on DUN_121 it is false and every friendly here -- both succubi and
	// Lazarus, who now runs this AI too -- falls into the branch below and gets parked on
	// MG_6_TALK the instant the player can SEE them. That is why they stood still while the
	// player was in the room but fought when he was not. In Lazarus' own lair they always fight.
	const bool dcLair = GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
					 && (monster.flag & MF_6_FRIENDLY);
	if( dcEscort || dcLair ){
		monster.speechIndex = 0;
		monster.goal = MG_1_ATTACK;
	}
	else if( FlagMap[ row ][ col ] & CF_2_VISIBLE_BY_PLAYER ){
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
		Ai_14_Succubus(monsterIndex);
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
		else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY, /*GM_CLASSIC,*/ GM_NIGHTMARE, GM_OUTBREAK)) {
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
		else if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_EASY, /*GM_CLASSIC,*/ GM_NIGHTMARE, GM_OUTBREAK)) {
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
// (th4) THIEF: her tile's illumination gates monster SIGHT. At full darkness (lightLevel 0)
// monsters can never sight-activate on her at all; in partial light they only notice her
// within lightLevel tiles (Chebyshev). Being HIT still activates (damage path, untouched),
// and already-active monsters are unaffected -- this gates only the dormant->active-by-sight
// transition in ProcessMonsters below.
static bool ThiefBlocksSightWake( const Monster& monster )
{
	if( MaxCountOfPlayersInGame != 1 || !HasTrait(CurrentPlayerIndex, TraitId::Thief) ) return false;
	// (th4) Monsters running the HIDDEN AI are predators of darkness themselves -- they hunt
	// by more than sight, so the Thief's shadows do NOT conceal her from them. Keyed on the AI
	// behaviour, not the base monster: a bosspack of any species can carry Hidden AI (see
	// monster.ai = boss.ai), and those must see her too. Every other AI keeps the sight gate.
	if( monster.ai == AI_15_Hidden ) return false;
	Player& p = Players[CurrentPlayerIndex];
	if( !In112(p.Row, p.Col) ) return false;
	int lightLevel = 15 - LightMap[p.Row][p.Col];
	LimitToRange(lightLevel, 0, 15);
	// (th4) residual glow from distant light sources still counts as HIDDEN: anything at or
	// below this level is treated as full darkness (per Mordor's playtest -- standing in the
	// faint spill of a far torch must not expose her). Tune freely.
	enum { THIEF_HIDDEN_LIGHT_MAX = 2 };
	if( lightLevel <= THIEF_HIDDEN_LIGHT_MAX ) return true;
	int dist = max( abs(monster.Row - p.Row), abs(monster.Col - p.Col) );
	return dist > lightLevel;
}

// (th4) DARK CAMPAIGN HUB: every monster in the Dark Lord's Hideout is kin. They never
// fight, never wake, and never turn. The hard gate lives in ProcessMonsters below and works
// by clamping ActivationCounter to 0 every tick -- ActivationCounter is what all ~30 AI
// functions gate their behaviour on, so zeroing it here neutralises the whole roster in one
// place, no matter which of the many wake sites fired (sight, damage, chain activation,
// bosspack refresh...). Orientation is likewise re-asserted every tick from the value the
// monster was created/loaded with, so kin hold whatever heading they were placed at.
static bool DarkCampaignKin()
{
	return Dungeon == DUN_117_DARKLORD_HIDEOUT;
}

void ProcessMonsters() // ProcessMonsters
{
	if (BattleTrance) {
		BattleTranceSlowMonster = !BattleTranceSlowMonster;
		if (BattleTranceSlowMonster)
			return;
	}

	RemoveKilledMonsters();
	FixBossLightInSlot0();	// (th4) a boss light on slot 0 never followed him when walking (BOSS LIGHT IN SLOT 0, above CreateBoss)
	ParkHiddenBossLights();	// (th4) an invisible boss lit his empty spawn spot (HIDDEN BOSS LIGHT, above CreateBoss)
	Player& auraTargetPlayer = Players[CurrentPlayerIndex];
	for (int j = 0; j < countof(Player::AuraIndex); j++)
	{
		auraTargetPlayer.AuraIndex[j] = AUI_0_NONE;
		auraTargetPlayer.AurasValues[j] = 0;
	}
	// (th4) DARK CAMPAIGN HUB: kin are inert. Clamp them dormant and frozen-facing before any
	// AI runs, then skip the rest of the tick for them entirely -- no wake, no target refresh,
	// no AI dispatch, no action processing. Damage-path wakes elsewhere are undone here on the
	// very next tick, which is what makes this a hard gate rather than a sight-only one.
	if( DarkCampaignKin() ){
		// (th4) Kin idle at HALF speed. Done with a tick divider rather than by doubling each
		// monster's AnimationDelayFrameCount, because that field is part of the saved Monster
		// blob -- mutating it would persist, and would compound every time the level reloaded.
		static int hubIdleTick = 0;
		bool advanceFrames = (++hubIdleTick & 1) == 0;	// advance on every second tick == -50% speed
		for( int i = 0; i < MonstersCount; ++i ){
			Monster& kin = Monsters[ MonsterIndexes[i] ];
			if( !kin.SpritePtr ) continue;

			// (th4) The escort is the one exception to the freeze. Two cases, both of which
			// need them moving while every other kin stands still:
			//   - mission 10 briefed  -> they fall in and follow the player around the Sanctum
			//   - just back from it   -> they walk from the portal to their posts, then stop
			// Once home the latch clears and this whole branch stops firing.
			// (th4) Self-heal: an escort standing anywhere but her post while no mission is
			// running has been stranded -- most often by a save/reload, where positions come
			// back from the delta and the (unsaved) walk-home latch is gone, so nothing else
			// would ever move her. Re-latch the walk rather than teleporting her: she then
			// finishes the route on foot exactly as she would have without the save, and the
			// give-up timer below still rescues her if the way home is genuinely impassable.
			if( kin.newBossId && DarkCampaignIsEscortBoss( kin.newBossId - 1 )
			 && !DarkCampaignEscortActive() && !DarkCampaignEscortWalkingHome ){
				int hr = 0, hc = 0;
				if( DarkCampaignEscortHubPost( kin.newBossId - 1, hr, hc )
				 && (kin.Row != hr || kin.Col != hc) ){
					DarkCampaignEscortWalkingHome = true;
					// Resuming, not arriving: they are already in the hub, so do NOT pull them
					// to the player -- that is only for stepping out of the mission portal.
					DarkCampaignEscortNeedsArrival = false;
				}
			}

			// (th4) DC mission 25: the Butcher is the other exception to the freeze. He has
			// no post -- he simply shadows the player around the Sanctum, exactly as he does
			// on a mission.
			const bool butcherFollows = kin.newBossId
			                         && kin.newBossId - 1 == UM_9_The_Butcher
			                         && DarkCampaignButcherJoined();
			if( butcherFollows
			 || (kin.newBossId && DarkCampaignIsEscortBoss( kin.newBossId - 1 )
			  && (DarkCampaignEscortActive() || DarkCampaignEscortWalkingHome)) ){
				int postRow = 0, postCol = 0;
				if( !butcherFollows ){
					DarkCampaignEscortHubPost( kin.newBossId - 1, postRow, postCol );
				}

				// (th4) First tick back from mission 10: pull them off their posts to the
				// player, so they arrive with him and walk home from there. Done here, not in
				// CreateBoss, because the player's tile is only settled after generation.
				// TeleportSummonToOwner is the engine's own relocate -- it picks a free ring
				// tile in sight of the owner and keeps MonsterMap correct.
				if( butcherFollows ? DarkCampaignButcherNeedsArrival : DarkCampaignEscortNeedsArrival ){
					TeleportSummonToOwner( MonsterIndexes[i], CurrentPlayerIndex );
				}

				// Where they are trying to be: the player while escorting, their post after.
				int wantRow = postRow, wantCol = postCol;
				int stopGap = 0;
				if( butcherFollows || DarkCampaignEscortActive() ){
					wantRow = Players[CurrentPlayerIndex].Row;
					wantCol = Players[CurrentPlayerIndex].Col;
					stopGap = DC_ESCORT_FOLLOW_GAP;
				}

				int gap = max( abs(kin.Row - wantRow), abs(kin.Col - wantCol) );
				const int ki = MonsterIndexes[i];

				// (th4) Two reasons to keep this one moving rather than freezing it:
				//   - it is mid-stride, and cutting a walk short would strand the tile it had
				//     reserved in MonsterMap (that stale reservation is what made their names
				//     pop up under the cursor over empty floor)
				//   - it is simply too far away and needs to close the gap
				if( kin.CurAction != A_0_STAY || gap > stopGap ){
					kin.ActivationCounter = 255;	// awake, as Ai_Summon does for minions

					// (th4) Mirror the main loop's shape: issue the next step at the TOP of every
					// iteration, then run the action handler. Completing a tile leaves CurAction
					// back at A_0_STAY, so starting the step only once before the loop meant the
					// walk ended there and nothing new began until the next tick -- a visible
					// hitch after every single tile. Re-issuing inside the loop lets them stride
					// continuously, exactly like a summon.
					int moreAction = 1;
					int guard = 0;
					while( moreAction && ++guard < 8 ){
						if( kin.CurAction == A_0_STAY ){
							// Arrived? Then stop cleanly rather than starting another step.
							int stepGap = max( abs(kin.Row - wantRow), abs(kin.Col - wantCol) );
							if( stepGap <= stopGap ) break;

							// Start the next step the same way player summons do: set the
							// destination and let MakeMonsterPath route there. It pathfinds
							// properly (around the Dark Lord, pillars, each other) and starts
							// the walk through the normal StartMonsterWalk path, so MonsterMap
							// bookkeeping stays correct.
							kin.TargetRow = (char)wantRow;	// char field, coords <= 111
							kin.TargetCol = (char)wantCol;
							if( !MakeMonsterPath( ki ) ){
								// (th4) FindPath gives up past 28 steps and treats the other kin
								// as solid, and the posts sit in the middle of the Dark Lord's
								// crowd -- so walking home fails exactly where it matters. Fall
								// back to a plain step in the right direction; it shuffles around
								// the group over a few ticks instead of standing there forever.
								if( !MonsterWalksTowardsTarget( ki,
										OrientationToTarget( kin.Row, kin.Col, wantRow, wantCol ) ) ){
									break;	// blocked this tick; retry on the next one
								}
							}
						}
						switch( kin.CurAction ){
						case A_1_WALK_UP:		moreAction = MonsterWalkUp		 ( ki );	break;
						case A_2_WALK_DOWN:		moreAction = MonsterWalkDown	 ( ki );	break;
						case A_3_WALK_HORIZ:	moreAction = MonsterWalkHorizontal( ki );	break;
						default:				moreAction = 0;								break;
						}
					}

					// (th4) And advance the animation, which the main loop does after its own
					// action switch -- the hub path returns long before reaching that code.
					kin.AnimationDelayIndex++;
					if( kin.AnimationDelayIndex >= kin.AnimationDelayFrameCount ){
						kin.AnimationDelayIndex = 0;
						kin.CurFrame++;
						if( kin.CurFrame > kin.animFrameCount ){
							kin.CurFrame = 1;
						}
					}
					continue;	// skip the freeze for this one this tick
				}
			}

			kin.ActivationCounter = 0;			// never awake -> no AI does anything
			kin.CurAction = A_0_STAY;			// never walking/attacking
			// (th4) ...and no leftover pixel offset. A frozen kin never takes an AI tick, so
			// nothing else here would ever clear one -- and a stale offset out of a reused
			// Monsters[] slot (see CreateMonster) or read back from a save taken while it was
			// wrong draws the vendor a whole tile off, into the wall behind its post. Cheap
			// to reassert every tick, and it repairs hubs saved before the fix in CreateMonster.
			kin.DeltaX = 0;
			kin.DeltaY = 0;
			kin.curAnimation = &kin.SpritePtr->animation[ANIM_0_STAY].oriented[ kin.Orientation ];
			// Idle breathing still plays; the frame walk is lifted from the normal path below,
			// minus MonsterStaying's MonsterLookAround call -- that is what would turn them.
			if( !advanceFrames ) continue;
			kin.AnimationDelayIndex++;
			if( kin.AnimationDelayIndex >= kin.AnimationDelayFrameCount ){
				kin.AnimationDelayIndex = 0;
				kin.CurFrame++;
				if( kin.CurFrame > kin.animFrameCount ) kin.CurFrame = 1;
			}
		}
		// (th4) The arrival relocate is a one-shot: both succubi have now been pulled to the
		// player, so from here on they simply walk home. Same for the Butcher.
		DarkCampaignEscortNeedsArrival = false;
		DarkCampaignButcherNeedsArrival = false;

		// (th4) Escort is home once neither succubus is still off her post; drop the latch so
		// the exemption above stops firing and they behave as ordinary kin from here on.
		if( DarkCampaignEscortWalkingHome ){
			// (th4) Give up on walking after a while and just put them on their posts. The
			// route home is a plain step when MakeMonsterPath refuses (28-step cap, and it
			// treats the standing kin as solid), and a plain step cannot get round a pillar
			// or a wall corner -- so a succubus dropped on the wrong side of one would walk
			// into it forever. This is the hub, not a fight: snapping is invisible to the
			// player and strictly better than one of them stuck in the scenery.
			static int escortWalkHomeTicks = 0;
			const int EscortWalkHomeGiveUp = 700;	// ~35s at 20 ticks/s
			const bool forceHome = ++escortWalkHomeTicks > EscortWalkHomeGiveUp;

			bool anyStillWalking = false;
			for( int i = 0; i < MonstersCount; ++i ){
				const int ki = MonsterIndexes[i];
				Monster& kin = Monsters[ ki ];
				if( !kin.SpritePtr || !kin.newBossId ) continue;
				if( !DarkCampaignIsEscortBoss( kin.newBossId - 1 ) ) continue;
				int postRow = 0, postCol = 0;
				if( !DarkCampaignEscortHubPost( kin.newBossId - 1, postRow, postCol ) ) continue;

				if( forceHome && (kin.Row != postRow || kin.Col != postCol) ){
					DarkCampaignSnapEscortToPost( ki );
				}

				if( kin.Row != postRow || kin.Col != postCol ){
					anyStillWalking = true;
				}else if( kin.CurAction == A_0_STAY ){
					// (th4) Back on post: face south again, the way CreateBoss stood them when
					// the hub was built (hubFacing = DIR_0_D for both succubi). Walking home
					// leaves them looking whichever way they last stepped, so restore it here
					// or they end up facing off at an angle beside the Dark Lord.
					kin.Orientation = DIR_0_D;
				}
			}
			if( !anyStillWalking || forceHome ){
				DarkCampaignEscortWalkingHome = false;
				escortWalkHomeTicks = 0;
			}
		}
		return;
	}

	for( int i = 0; i < MonstersCount; ++i ){
		int monsterIndex = MonsterIndexes[i];
		Monster& monster = Monsters[monsterIndex];

		// (th4) GHOST GUARD. Every attack path treats a monster as already dead once
		// (CurrentLife & -64) <= 0 -- Player.cpp returns IMMUNE_TO_MELEE_ATTACK, and the
		// regen tick skips it too. If some damage path drops a monster into that window
		// without also running the death sequence, it is stranded: it keeps acting and can
		// still hit the player, but nothing can hit it back and it never dies. On a Dark
		// Campaign level it also holds the "heroes left" counter above zero forever, so the
		// way home never opens. Finish the job here, whatever path leaked it.
		if( monsterIndex >= SummonMonsters_Count
		 && monster.SpritePtr && !(monster.flag & MF_15_KILLED)
		 && !(monster.Row == 1 && monster.Col == 0)
		 && monster.CurAction != A_6_DEATH
		 && (monster.CurrentLife & (-64)) <= 0 ){
			KillMonsterByMonster( M1_TRAP, monsterIndex );
			continue;
		}

		if( MaxCountOfPlayersInGame > 1 ){
			SetRndSeed(monster.RandomSeedForAction);
			monster.RandomSeedForAction = GetRndSeed();
		}
		if( monster.CurAction != A_15_STONE && monster.CurrentLife < monster.BaseLife && (monster.CurrentLife & ~63) > 0){
			if (monsterIndex >= SummonMonsters_Count) {
				monster.CurrentLife += 1 + monster.MonsterLevel / 4 + Difficulty * 12;
			}
			else if (monster.SpritePtr->baseMonsterIndex == BM_392_WARLORD_MERC && MaxCountOfPlayersInGame == 1) {
				int regenPerk = PerkValue(SYNERGY_REGENERATION, CurrentPlayerIndex);
				if (regenPerk > 0) {
					monster.CurrentLife += regenPerk * 64 / 20; //(monster.BaseLife * regenPerk) / (28 * 64);
				}
			}
		}
		uchar flag = FlagMap[ monster.Row ][ monster.Col ];
		// (th4) THIEF: pure light+distance predicate -- true whenever this monster cannot
		// currently perceive her (she is in shadow and/or beyond her light's reach). Always
		// false outside SP-with-Thief, so everything below is vanilla for everyone else.
		bool thiefUnseen = ThiefBlocksSightWake( monster );
		// dormant monsters may fail to notice her at all (the wake gate)
		bool thiefHidesFromThis = !monster.ActivationCounter && thiefUnseen;
		// (th4) THIEF vs the HIDDEN AI: her stealth normally works by never entering the
		// CF_2_VISIBLE_BY_PLAYER wake path -- in darkness she casts no such flag, so NOTHING
		// near her wakes. That is exactly why the Hidden ignored her in the black chapel: not
		// because they refuse to, but because the ordinary wake requires her to be "visible",
		// and she never is. So the Hidden AI needs its OWN positive sniff: it wakes on raw
		// PROXIMITY, no visibility flag required, precisely because it hunts in the dark. This
		// is the mirror of her own sight rule -- she smells to them the way monsters look to her.
		bool hiddenSniffedHer = false;
		if( monster.ai == AI_15_Hidden && !monster.ActivationCounter && !(monster.flag & MF_6_FRIENDLY)
		 && MaxCountOfPlayersInGame == 1 && HasTrait(CurrentPlayerIndex, TraitId::Thief) ){
			Player& tp = Players[CurrentPlayerIndex];
			if( tp.IsExists && Dungeon == tp.dungeon ){
				enum { THIEF_HIDDEN_SNIFF_RADIUS = 9 }; // how close she must get before the Hidden smell her; tune freely
				int sniff = max( abs(monster.Row - tp.Row), abs(monster.Col - tp.Col) );
				if( sniff <= THIEF_HIDDEN_SNIFF_RADIUS ) hiddenSniffedHer = true;
			}
		}
		if( ((flag & CF_2_VISIBLE_BY_PLAYER) && !thiefHidesFromThis || hiddenSniffedHer) && !monster.ActivationCounter ){
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
				// (th4) DC mission 13: the three friendlies in Lazarus' lair kept swinging at
				// corpses. MonsterLookAround() -- the only thing that picks a NEW target, and
				// the only place a dead one is rejected -- is called above ONLY while
				// !ActivationCounter. These three are spawned awake, so they never re-pick:
				// the lines just below happily keep refreshing the target coordinates from a
				// monster that is already dead. Force a fresh look when that happens.
				// Scoped to this level so no other monster's targeting behaviour shifts.
				if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
				 && (monster.flag & MF_6_FRIENDLY)
				 && ( (targetMonster.CurrentLife & (-64)) <= 0
				   || targetMonster.CurAction == A_6_DEATH ) ){
					MonsterLookAround( monsterIndex );
					monster.LastTargetPositionRow = monster.TargetRow;
					monster.LastTargetPositionCol = monster.TargetCol;
				}
				else{
                monster.TargetRow = monster.LastTargetPositionRow = targetMonster.NextRow;
                monster.TargetCol = monster.LastTargetPositionCol = targetMonster.NextCol;
				}
				//monster.ActivationCounter = monster.newBossId ? 255 : 254; // fix monster don't attack summons
			}
			// (th4) ACTIVATE ON THE PLAYER, NOT ON WHOEVER HAPPENS TO BE NEAREST.
			//
			// A ranged monster listed in NewToGolemAttackType() may pick a SUMMON as its
			// target -- that is what the list is for -- and MonsterLookAround prefers it
			// whenever the summon is the closer of the two. Until now that put the monster
			// down this branch, where the only activation line is the commented-out one
			// above, so ActivationCounter stayed 0 FOREVER and the monster froze:
			//   - it never attacked, because RangerRetreatsOften and friends gate on
			//     ActivationCounter ("prevent monsters from attack without activation");
			//   - the summon never fought back, because MonsterLookAround makes summons
			//     skip un-activated targets;
			//   - and it bleated once per FRAME, because the entry test a few lines above
			//     is exactly !ActivationCounter, so MonsterChainActivation -- which greets
			//     unconditionally at its top -- was re-entered every tick.
			//
			// The old commented-out line is NOT the fix: it would wake a monster because a
			// SUMMON came near, which is how a golem ends up pulling the whole level. This
			// activates on the PLAYER instead, using the very same visibility test the
			// player-target branch below already uses, so who the monster happens to be
			// TARGETING no longer decides whether it may wake up. A summon wandering off
			// alone still wakes nothing.
			if( !(monster.flag & MF_6_FRIENDLY) && (flag & CF_2_VISIBLE_BY_PLAYER) && !thiefUnseen ){
				monster.ActivationCounter = 255;
			}
		}else{ // target is player
			if( monster.TargetIndex < 4 ){
				Player& targetPlayer = Players[monster.TargetIndex];
				monster.TargetRow = (char) targetPlayer.NextRow;
				monster.TargetCol = (char) targetPlayer.NextCol;
				// (th4) THIEF: while she is unseen (in shadows), aggro must be allowed to
				// cool off -- so the visibility refresh is suppressed, and the boss
				// self-refresh (newBossId && 255, which normally makes bosspacks aggro
				// FOREVER) is broken too. The moment she is perceivable again, the normal
				// refresh restores 255 for everyone, bosses included.
				if( (flag & CF_2_VISIBLE_BY_PLAYER && !thiefUnseen) || monster.newBossId && monster.ActivationCounter == 255 && !thiefUnseen
					/*|| monster.isBossPack && Monsters[monster.hisBossIndex].ActivationCounter == 255*/) {					
					monster.ActivationCounter = 255;
					monster.LastTargetPositionRow = targetPlayer.NextRow;
					monster.LastTargetPositionCol = targetPlayer.NextCol;
				}else if( monster.ActivationCounter && thiefUnseen ){
					// (th4) THIEF deaggro rates while she stays hidden: regular monsters
					// lose her TWICE as fast as normal decay; bosspacks -- previously
					// incapable of deaggro at all -- wind down at normal (1x) speed.
					int thiefDecay = monster.newBossId ? 1 : 2;
					if( thiefDecay > monster.ActivationCounter ) thiefDecay = monster.ActivationCounter;
					monster.ActivationCounter -= thiefDecay;
				}else if( monster.ActivationCounter && !monster.newBossId ){ // (rt) (10) boss number extension 004439CD
					monster.ActivationCounter--;
				}
			}
		}
		int moreAction = 1;
		while( moreAction ){
			if( !(monster.flag & MF_9_CHASE_TARGET) || !MonsterChangeTarget(monsterIndex) ){
			    auto iiFunc = AiFuncs[monster.ai].aiFunc;
			    if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO
			     && monster.newBossId && monster.newBossId - 1 == UM_587_Hephasto_the_Armorer ){
			        // (th4) DC mission 15: the armorer stands at his forge and ignores the
			        // player -- MF_6_FRIENDLY means he never takes a player as a target at all
			        // -- but he is not helpless. Heroes DO pick him (a hostile accepts a
			        // MF_6_FRIENDLY monster as a target, see MonsterLookAround), and when one
			        // does he gets a target, MF_11_NOENEMY clears, and his own AI runs so he
			        // fights back. With nothing hostile near him the flag stays set, the AI
			        // never runs, and he simply stands there.
			        //
			        // Same two guards Ai_Summon gives a minion, and for the same reason: drop
			        // a target the moment it stops being valid, or he keeps swinging at a
			        // corpse -- MonsterLookAround leaves TargetRow/TargetCol untouched when it
			        // finds nobody, so only the NOENEMY test below actually stops him.
			        if( (monster.flag & MF_5_TARGET_MONSTER) && monster.TargetIndex < Monsters_Max_Count ){
			            const Monster& t = Monsters[monster.TargetIndex];
			            if( (t.CurrentLife & (-64)) <= 0 || t.CurAction == A_6_DEATH
			             || t.ActivationCounter == 0 ){
			                MonsterLookAround( monsterIndex );
			            }
			        }
			        if( !(monster.flag & MF_11_NOENEMY) ){
			            iiFunc( monsterIndex );
			        }
			        else if( !monster.CurAction ){
			            // Nothing left to fight and he is idle -- back to the forge, facing DR.
			            // No-op once he is already there, so this costs one comparison a tick.
			            DarkCampaignReturnHephastoToForge( monsterIndex );
			        }
			    }
			    else if( monsterIndex < SummonMonsters_Count ){
			        Ai_Summon( monsterIndex, iiFunc );
                }else if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC
                       && (monster.flag & MF_6_FRIENDLY) ){
                    // (th4) DC mission 13: the three friendlies get the same two guards
                    // Ai_Summon gives a minion, and for the same reason -- without them they
                    // keep casting at the tile where a hero died. Re-pick as soon as the held
                    // target stops being a valid one, and DO NOT run the attack AI at all
                    // while nothing is selected. MonsterLookAround leaves TargetRow/TargetCol
                    // untouched when it finds nobody (it only raises MF_11_NOENEMY), so the
                    // stale corpse coordinates survive and the AI happily keeps firing at
                    // them -- the NOENEMY test below is what actually stops that.
                    if( (monster.flag & MF_5_TARGET_MONSTER) && monster.TargetIndex < Monsters_Max_Count ){
                        const Monster& t = Monsters[monster.TargetIndex];
                        if( (t.CurrentLife & (-64)) <= 0 || t.CurAction == A_6_DEATH
                         || t.ActivationCounter == 0 ){
                            MonsterLookAround( monsterIndex );
                        }
                    }
                    if( !(monster.flag & MF_11_NOENEMY) ){
                        iiFunc( monsterIndex );
                    }
                    else if( (monster.flag & MF_1_INVISIBLE) && !monster.CurAction ){
                        // (th4) ...but if the last hero died while he was PHASED OUT, the
                        // guard above freezes him there for good: Monster_Fade_Out leaves
                        // MF_1_INVISIBLE set and only Monster_Fade_In clears it, and that
                        // call lives inside the AI we just refused to run. He stays invisible
                        // and unclickable, with the mission waiting on a talk that can never
                        // happen. Finish the phase for him instead. MG_1_ATTACK rather than
                        // the MG_4_WALK he was left on, so he does not resume circling a
                        // target that no longer exists.
                        Monster_Fade_In( monsterIndex, monster.Orientation, 1 );
                        monster.goal = MG_1_ATTACK;
                    }
                }else if( GameMode == GM_DARK_CAMPAIGN && (monster.flag & MF_6_FRIENDLY)
                       && monster.newBossId && DarkCampaignIsEscortBoss( monster.newBossId - 1 ) ){
                    // (th4) DC mission 10 escort: follows like a minion, then fights.
                    Ai_DarkCampaignEscort( monsterIndex, iiFunc );
                }else if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_44_RAVENHOLM
                       && monster.newBossId && monster.newBossId - 1 == UM_773_The_Dark_Lord ){
                    // (th4) DC mission 26: the Dark Lord neither fights nor wanders. The only
                    // thing this branch ever starts is his walk out of the portal, one tile
                    // per landing until DarkCampaignEndWalkSteps runs out. After that he
                    // holds the tile facing DIR_0_D and does nothing at all.
                    if( monster.CurAction == A_0_STAY ){
                        if( DarkCampaignEndWalkSteps > 0
                         && In112(monster.Row + 1, monster.Col)
                         && !MonsterMap[monster.Row + 1][monster.Col]
                         && !IsCellBlocking(monster.Row + 1, monster.Col) ){
                            --DarkCampaignEndWalkSteps;
                            StartMonsterWalk( monsterIndex, DIR_0_D );
                        }else{
                            DarkCampaignEndWalkSteps = 0;	// blocked: stop where he is
                            if( monster.Orientation != DIR_0_D ){
                                monster.Orientation = DIR_0_D;
                                monster.curAnimation = &monster.SpritePtr->animation[ANIM_0_STAY].oriented[DIR_0_D];
                            }
                            // Slow his idle right down: he is posing, not patrolling. The
                            // frame advance below steps once every AnimationDelayFrameCount
                            // ticks, so a bigger number is a slower loop. Assigned rather
                            // than incremented, so re-running it every tick -- and the fact
                            // that the field is part of the saved Monster blob -- is harmless.
                            monster.AnimationDelayFrameCount = DC_END_LORD_IDLE_DELAY;
                        }
                    }
                }else if( GameMode == GM_DARK_CAMPAIGN && (monster.flag & MF_6_FRIENDLY)
                       && monster.newBossId && monster.newBossId - 1 == UM_9_The_Butcher ){
                    // (th4) DC mission 25. Caged until spoken to: no AI at all, so he neither
                    // wanders out of the cell nor joins a fight the player has not freed him
                    // for. Once freed he follows the player and engages only what he can see.
                    if( DarkCampaignButcherEscorting() ){
                        Ai_DarkCampaignButcher( monsterIndex, iiFunc );
                    }
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
		(rt) I don't understand how this works, but the gist is this -
		(rt) we walk every cell closest to the diagonal between the monster and the target
		(rt) and return 0 if any of them is inaccessible
		(rt) and 1 if all are accessible
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // (rt) The distance to the target along col is greater than or equal to the distance along row
		if( deltaCol < 0 ){ // (rt) if the target's col is less than the monster's col, swap the coordinate values
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
		(rt) I don't understand how this works, but the gist is this -
		(rt) we walk every cell closest to the diagonal between the monster and the target
		(rt) and return 0 if any of them is inaccessible
		(rt) and 1 if all are accessible
	*/

	if( abs(deltaRow) <= abs(deltaCol) ){ // (rt) The distance to the target along col is greater than or equal to the distance along row
		if( deltaCol < 0 ){ // (rt) if the target's col is less than the monster's col, swap the coordinate values
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
	if( bossNumber ){ // (rt) (11) boss number extension 004441C9
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && UniqueMonsters[bossNumber - 1].IceAgeNamePtr) {
			monster.Name = (char*)LocalizedIceAgeName(UniqueMonsters[bossNumber - 1].IceAgeNamePtr);
		}
		else {
			monster.Name = (char*)LocalizedUniqueMonsterName( &UniqueMonsters[bossNumber - 1] );
		}
	}else{
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) && !Dungeon->isQuest && monster.BasePtr->IceAgeNamePtr) {
			monster.Name = (char*)LocalizedIceAgeName(monster.BasePtr->IceAgeNamePtr);
		}
		else {
			monster.Name = (char*)LocalizedMonsterName( monster.BasePtr );
		}
	}
	uchar orientation = monster.Orientation;
	// A monster saved mid-charge (A_14_CHARGE) is normally carried by its MI_20_RUN_ATTACK
	// missile. Missiles are wiped on level entry (InitMissiles), so a charge that was in
	// progress when the level was left has no missile to advance it after loading: the
	// monster would stay frozen in the charge frame forever ("disappeared"). Abort the
	// charge and return the monster to a normal idle state, clearing the transient
	// negative MonsterMap reservation left by the charge.
	if (monster.CurAction == A_14_CHARGE) {
		if (MonsterMap[monster.Row][monster.Col] == -1 - (int)monsterIndex) {
			MonsterMap[monster.Row][monster.Col] = monsterIndex + 1;
		}
		monster.PrevActionOrRowDeltaOrSpellEffect = monster.CurAction;
		monster.CurAction = A_0_STAY;
		monster.TimerCounterOrColDeltaOrSpellDamage = 0;
		monster.DeltaX = 0;
		monster.DeltaY = 0;
		monster.NextRow = monster.Row;
		monster.NextCol = monster.Col;
		monster.PrevRow = monster.Row;
		monster.PrevCol = monster.Col;
	}
	uchar animationIndex = MonsterAnimIndex(monster.CurAction);
	// (th4) A monster restored in A_0_STAY is standing on its tile, so it can carry no walk
	// offset -- but DeltaX/DeltaY sit at struct offsets 56/60, well inside the blob that
	// LoadMonster reads back (offsetof(Monster, Name)), so whatever they held when the level
	// was saved comes straight back with it. DrawMonsterInTile adds them to the screen
	// position, so a stale pair draws the monster away from the cell it actually occupies.
	// Ordinary monsters hide it -- their first AI tick overwrites the offset -- but the Dark
	// Campaign hub kin are frozen and never take one, so a vendor kept it for good and stood
	// in the wall behind its post. Fixing it here rather than only in the hub repairs every
	// save written before CreateMonster started zeroing these, for every monster.
	if( monster.CurAction == A_0_STAY ){
		monster.DeltaX = 0;
		monster.DeltaY = 0;
	}
	// (th4) DC mission 7: BM_H_405 (Gwideon and kin) fireblast on ANIM_5_SECOND -- keep that
	// choice when a monster is restored mid-ranged-cast from a save, matching StartMonsterRangedAttack.
	if (monster.CurAction == A_10_RANGED && monster.BasePtr && monster.BasePtr->id == BM_H_405_SOR_M_T_HYBRID) {
		animationIndex = ANIM_5_SECOND;
	}
	if (is(monster.CurAction, A_14_CHARGE, A_15_STONE)) {
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
	sprintf(InfoPanelBuffer, L("Total slain: %i, i %i rc %i %i"), totalSlain, monsterIndex, monster.Row, monster.Col);
	#else
	sprintf(InfoPanelBuffer, L("Total slain : %i"), totalSlain);
	#endif
	InfoPanel_AddLine(InfoPanelBuffer, true);
	int hpShow = GameMode == GM_CLASSIC ? 0 : 5;
	if( totalSlain >= hpShow ){
		sprintf(InfoPanelBuffer, L("Hit Points : %i of %i"), Monsters[CurMon].CurrentLife >> 6, Monsters[CurMon].BaseLife >> 6); // (rt) 1 displayed hit point equals 64 real ones
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
	// (rt) Better to use exact values for all 3 resistances rather than a mask...
	int rezShow = GameMode == GM_CLASSIC ? 0 : 3;
	if( totalSlain >= rezShow ){
		BaseMonster& baseMonster = BaseMonsters[baseMonsterIndex];
		if( Difficulty >= DL_2_DOOM ){
			monsterResistMask = baseMonster.ResistOn3Difficulty;
		}else{
			monsterResistMask = baseMonster.ResistOn1And2Difficulty;
		}
		InfoPanel_AddLine(L("Monster type : "), true, L(Spicies[ min( baseMonster.MonsterClass, 3ui16) ]) );
		#ifdef comment // 1.150
		if( !(monsterResistMask & 0x3F) ){  // (rt) 0011 1111 only the first 6 bits count in the mask
			InfoPanel_AddLine(L("No magic resistance"), true);
			return;
		}
		if( monsterResistMask & (MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT) ){ // (rt) 0111 The first 3 bits are resistances. To magic, fire and lightning respectively
			strcpy(InfoPanelBuffer, L("Resists : "));
			if( monsterResistMask & MR_1_ARCAN ){
				strcat(InfoPanelBuffer, L("Magic "));
			}
			if( monsterResistMask & MR_2_FIRE ){
				strcat(InfoPanelBuffer, L("Fire "));
			}
			if( monsterResistMask & MR_3_LIGHT ){
				strcat(InfoPanelBuffer, L("Lightning "));
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = 0;
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		#endif
		if( monsterResistMask & (MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID|MR_11_IM_COLD) ){ // (rt) 0011 1000 The next 3 bits are immunities. To magic, fire and lightning respectively
			strcpy(InfoPanelBuffer, L("Immune : "));
			if( monsterResistMask & MR_4_IM_ARCAN ){
				strcat(InfoPanelBuffer, L("Magic "));
			}
			if( monsterResistMask & MR_5_IM_FIRE ){
				strcat(InfoPanelBuffer, L("Fire "));
			}
			if( monsterResistMask & MR_6_IM_LIGHT ){
				strcat(InfoPanelBuffer, L("Lightning "));
			}
			if( monsterResistMask & MR_8_IM_ACID ){
				strcat(InfoPanelBuffer, L("Acid "));
			}
			if( monsterResistMask & MR_11_IM_COLD ){
				strcat(InfoPanelBuffer, L("Cold "));
			}
			InfoPanelBuffer[strlen(InfoPanelBuffer) - 1] = '\0';
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}else{
			InfoPanel_AddLine(L("No immunity"), true);
		}
	}
}

//----- (00444497) --------------------------------------------------------
void ShowBossHP()
{
	sprintf( InfoPanelBuffer, L("Hit Points : %i of %i"), Monsters[ CurMon ].CurrentLife >> 6, Monsters[ CurMon ].BaseLife >> 6 );
	InfoPanel_AddLine( InfoPanelBuffer, 1 );
	InfoPanelUsed = 1;
	//(rt) BossShowResImun(); // disabled in th1
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
		if( playerIndex != PlayerMap[ spellCast.Row ][ spellCast.Col ] - 1 ){ // (rt) check that the player isn't knocked back (?)
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
int __fastcall IsMonsterSummonable(int baseMonsterIndex)// (rt) monsters in ambush
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
	int monsterMayTrapCount = 0;// (rt) count how many monsters can lie in ambush
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}

	if( !monsterMayTrapCount ){
		return;
	}

	int randFactor = RNG(monsterMayTrapCount);// (rt) pick a random one 
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
	if( CheckCellForMeleeMonsterSafeMove(-1, row, col) ){ // (rt) if it can be placed right here, place it
		int orientation = OrientationToTarget(row, col, row, col);
		PutAmbushMonsterAndStartAttack(monsterIndex, row, col, orientation);
		return 1;
	}
	int checkFlag = 0;// (rt) check all cells at distance 1 from the target one for availability; build a map
	uint checksMap[9];
	for( int colOffset = -1; colOffset <= 1; colOffset++ ){
		for( int rowOffset = -1; rowOffset <= 1; rowOffset++ ){
			int isSafe = CheckCellForMeleeMonsterSafeMove(-1, row + rowOffset, col + colOffset);
			checkFlag |= isSafe;
			checksMap[3*(rowOffset+1)+colOffset+1] = isSafe;
		}
	}
	if( !checkFlag ){// (rt) if all are occupied, don't add
		return 0;
	}
	int loopCount = RNG(15) + 1;// (rt) pick a random cell from the available ones
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

	int rowOffset = mapIndex / 3 - 1;// (rt) and put the monster there
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
	int monsterMayTrapCount = 0;// (rt) count how many monsters can lie in ambush
	if( Dungeon->isQuest ) return -1;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex) ){
			monsterMayTrapCount++;
		}
	}
	if( monsterMayTrapCount ){
		int randFactor = RNG(monsterMayTrapCount);// (rt) pick a random one 
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

//----- (th4) -------------------------------------------------------------
// DARK CAMPAIGN HUB: what a click on a kin does. The Dark Lord speaks; the vendor bosses
// are vending machines -- they open their panel and say nothing. Keyed on newBossId (which
// the engine stores as bossId + 1), so each new helper boss added to the Hideout just gets
// another case here.
//
// The Dark Lord's branch deliberately does NOT go through TalkWithMonster/MonsterTalk:
// those drive the A_17_TALK monster action and the Gharbad/Snotspill/Lachdanan quest
// ladders, and the hub's hard AI gate (see ProcessMonsters) keeps kin in A_0_STAY forever,
// so that state machine would never run. StartSpeech is self-contained.
// Speech SP_255 is a PLACEHOLDER: swap for the real briefing when that system lands.
//
// Vendors open the SHARED town panels, so hub stock IS town stock -- no separate
// inventory, nothing new to persist, no save-version change.
// (th4) DARK CAMPAIGN mission table. One row per mission, in campaign order. The Dark Lord
// walks this table to find the player's current mission, and the outbound portal reads the
// same row for its destination -- so adding a mission is ONE row here, not edits in three
// places. Index into this table is also the index into Player::darkMission[], whose values
// are DCM_0_UNTOUCHED / DCM_1_BRIEFED / DCM_2_COMPLETE.
const DarkMission DarkMissions[] = {
	{ DUN_28_POISONED_WATER_SUPPLY, SP_DC_MISSION_1_BRIEF, "The Poisoned Water Supply", 32, 84 },
	{ DUN_112_STILL_SPRINGS,        SP_DC_MISSION_2_BRIEF, "Still Springs",             55, 58 },
	{ DUN_114_LOWER_SCRIPTORIUM,    SP_DC_MISSION_3_BRIEF, "The Lower Scriptorium",     20, 20 },
	{ DUN_64_MAUSOLEUM,             SP_DC_MISSION_4_BRIEF, "The Mausoleum",             33, 31 },
	{ DUN_70_CITADEL,               SP_DC_MISSION_5_BRIEF, "The Citadel",               52, 20 },
	{ DUN_59_HALL_OF_FIRE,          SP_DC_MISSION_6_BRIEF, "The Hall of Fire",          48, 79 },
	{ DUN_66_SEWERS,                SP_DC_MISSION_7_BRIEF, "The Sewers",                24, 24 },
	{ DUN_99_ANCIENT_CRYPTS,        SP_DC_MISSION_8_BRIEF, "The Ancient Crypts",        18, 22 },
	{ DUN_47_DARK_NEST,             SP_DC_MISSION_9_BRIEF, "The Dark Nest",             91, 20 },
	{ DUN_118_POISONED_WATER_SUPPLY_DC, SP_DC_MISSION_10_BRIEF, "The Poisoned Water Supply", 33, 83 },	// (th4) DC mission 10
	{ DUN_119_DEEPER_CAVES,             SP_DC_MISSION_11_BRIEF, "The Deeper Caves",          40, 81 },	// (th4) DC mission 11
	{ DUN_120_PANDEMONIUM_DC,           SP_DC_MISSION_12_BRIEF, "Pandemonium",               51, 51 },	// (th4) DC mission 12
	{ DUN_121_UNHOLY_ALTAR_DC,          SP_DC_MISSION_13_BRIEF, "The Unholy Altar",          35, 32 },	// (th4) DC mission 13
	{ DUN_83_BLOOD_KEEP,                SP_DC_MISSION_14_BRIEF, "Blood Keep",                56, 88 },	// (th4) DC mission 14
	{ DUN_43_HEPHASTO,                  SP_DC_MISSION_15_BRIEF, "Hellforge",                 32, 88 },	// (th4) DC mission 15
	{ DUN_51_FETID_CAVERN,              SP_DC_MISSION_16_BRIEF, "Fetid Cavern",              41, 20 },	// (th4) DC mission 16
	{ DUN_116_MINING_CAVES,             SP_DC_MISSION_17_BRIEF, "Mining Caves",              84, 85 },	// (th4) DC mission 17
	{ DUN_80_HELL_GATE,                 SP_DC_MISSION_18_BRIEF, "Hell Gate",                 82, 22 },	// (th4) DC mission 18
	{ DUN_72_NECROPOLIS,                SP_DC_MISSION_19_BRIEF, "Necropolis",                57, 20 },	// (th4) DC mission 19
	{ DUN_69_GIBBET,                    SP_DC_MISSION_20_BRIEF, "Gibbet",                    88, 20 },	// (th4) DC mission 20
	{ DUN_67_SEPULCHER,                 SP_DC_MISSION_21_BRIEF, "Sepulcher",                 88, 88 },	// (th4) DC mission 21
	{ DUN_65_RUINED_TEMPLE,             SP_DC_MISSION_22_BRIEF, "Ruined Temple",             58, 58 },	// (th4) DC mission 22
	{ DUN_62_DIM_UNDERCROFT,            SP_DC_MISSION_23_BRIEF, "Dim Undercroft",            84, 27 },	// (th4) DC mission 23
	{ DUN_60_HAUNTED_LABYRINTH,         SP_DC_MISSION_24_BRIEF, "Haunted Labyrinth",         84, 46 },	// (th4) DC mission 24
	{ DUN_111_CHURCH_SACRISTY,          SP_DC_MISSION_25_BRIEF, "Church Sacristy",           66, 52 },	// (th4) DC mission 25
	{ DUN_44_RAVENHOLM,                 SP_DC_MISSION_26_BRIEF, "Ravenholm",                  0,  0 },	// (th4) DC mission 26: final mission, no way home
};
const int DarkMissionCount = countof(DarkMissions);

// (th4) The mission the Dark Lord is currently concerned with: the lowest-indexed one that
// is not yet complete. A briefed-but-unfinished mission is re-offered, which is what makes
// talking to him again re-brief. Returns -1 when the whole campaign is done.
int DarkCampaignCurrentMission()
{
	for( int i = 0; i < DarkMissionCount && i < DARK_MISSION_MAX_50; ++i ){
		if( DarkMissionState[i] != DCM_2_COMPLETE ) return i;
	}
	return -1;
}

// (th4) Place the outbound mission portal in the hub. Called from TWO places -- the hub's
// generator (level load) and the Dark Lord's briefing (mid-level) -- because a warp added at
// generation cannot appear in response to something the player does afterwards. Idempotent:
// re-briefing must not stack duplicate warps on the same tile, since AddWarp just push_backs.
void OpenDarkCampaignPortal( int mission )
{
	if( mission < 0 || mission >= DarkMissionCount ) return;

	// The portal has TWO halves and they are placed independently, because they do not
	// survive the same events: the warp is cleared by ClearWarps() on level change, while
	// the visible red-portal missile is cleared by InitMissiles(). Checking each separately
	// means this is safe to call every tick and after any reload.
	bool haveWarp = false;
	for( auto& w: Warps ){
		if( w.Row == DARK_PORTAL_ROW && w.Col == DARK_PORTAL_COL ){ haveWarp = true; break; }
	}
	if( !haveWarp ){
		AddWarp( DARK_PORTAL_ROW, DARK_PORTAL_COL, WM_1029_GOTO_QUEST_DUNGEON, DarkMissions[mission].dungeon );
	}

	bool havePortal = false;
	for( int i = 0; i < MissileAmount; ++i ){
		Missile& m = Missiles[ MissileIndexes[i] ];
		if( m.BaseMissileIndex == MI_65_RED_PORTAL && m.Row == DARK_PORTAL_ROW && m.Col == DARK_PORTAL_COL ){
			havePortal = true;
			break;
		}
	}
	if( !havePortal ){
		CastMissile( DARK_PORTAL_ROW, DARK_PORTAL_COL, DARK_PORTAL_ROW, DARK_PORTAL_COL, 0,
		             MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
	}
}

// (th4) DARK CAMPAIGN HUB: every vendor service draws in the LEFT panel slot, and the draw
// chain in Draw.cpp (~3950) is an else-if ladder in which the character sheet and the quest
// log come BEFORE all of them. So clicking a vendor with the char sheet open did open his
// panel -- it just rendered underneath it and looked like nothing had happened. Town never
// had this bug because towners go through ChangeTownerDialog, which closes both first; the
// hub calls the *_Open() helpers directly, so it has to do the same itself.
//
// Only these two need clearing: the spellbook loses to IsINVPanelVisible, which every
// *_Open() sets, and the remaining panels are all later in the same ladder.
static void DarkCampaignCloseLeftPanelBlockers()
{
	IsCHARPanelVisible  = false;
	IsQUESTPanelVisible = false;
}

void __fastcall DarkCampaignTalk(uint monsterIndex)
{
	if( monsterIndex >= Monsters_Max_Count ) return;
	Monster& monster = Monsters[monsterIndex];

	switch( monster.newBossId - 1 ){
	case UM_0_Gharbad_the_Weak:				// hub blacksmith -- Griswold's Trade / Repair
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_955_M_GARBUD04, monster.Row, monster.Col );	// (th4) Gharbad greets on click
		VisualTrade_Open( VTM_Blacksmith );
		return;
	case UM_19_Jaldabaoth:					// hub powerups -- Wirt's Buy Power-Ups
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1050, monster.Row, monster.Col );	// (th4) greet on click
		Powerups_Open();
		return;
	case UM_30_Boilrot:						// hub stash -- Gillian's Open Stash
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1046, monster.Row, monster.Col );	// (th4) greet on click
		StashPanel_Open();
		return;
	case UM_48_Molianta_the_Copperwing:		// hub quench -- Griswold's Quench Items
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1498, monster.Row, monster.Col );	// (th4) greet on click
		Quench_Open();
		return;
	case UM_51_Galahad_the_Bonewhite:		// hub crafting -- Griswold's Crafting / Socketing
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1060, monster.Row, monster.Col );	// (th4) greet on click
		Craft_Open();
		return;
	case UM_89_Alystar:						// hub identify -- Cain's Identify Items
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_583_M_UA, monster.Row, monster.Col );	// (th4) greet on click
		VisualTrade_Open( VTM_Elder );
		return;
	case UM_94_Setebos:						// hub enchant -- Adria's Enchant Items
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1411_MAGNAT, monster.Row, monster.Col );	// (th4) greet on click
		Enchant_Open();
		return;
	case UM_124_Torvatus:					// hub witch -- Adria's Trade / Recharge
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1476, monster.Row, monster.Col );	// (th4) greet on click
		VisualTrade_Open( VTM_Witch );
		return;
	case UM_126_Gloomshredder:				// hub transmute -- Cain's Transmute Items
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1496, monster.Row, monster.Col );	// (th4) greet on click
		Transmute_Open();
		return;
	case UM_792_Salinus_the_Salt_King:		// hub healer -- Pepin's Trade
		DarkCampaignCloseLeftPanelBlockers();
		PlayLocalSound( S_1462, monster.Row, monster.Col );	// (th4) greet on click
		VisualTrade_Open( VTM_Healer );
		return;
	case UM_5_Red_Vex:						// (th4) hub entourage -- greets, no service
		PlayLocalSound( S_1039_M_ND, monster.Row, monster.Col );	// (th4) greet on click
		return;
	case UM_6_Dark_Jade:					// (th4) hub entourage -- greets, no service
		PlayLocalSound( S_989_M_VAR, monster.Row, monster.Col );	// (th4) greet on click
		return;
	}

	// The Dark Lord briefs. Anyone else without a service has nothing to say yet.
	if( Speech_IsPanelVisible ) return;					// already talking
	if( monster.newBossId - 1 != UM_773_The_Dark_Lord ) return;

	int mission = DarkCampaignCurrentMission();
	if( mission < 0 ) return;							// campaign finished -- he is silent

	FixMonsterPosition( monsterIndex, monster.Orientation );	// keep him facing as placed
	PlayLocalSound( S_1482, monster.Row, monster.Col );	// (th4) DL greet on click, in addition to his speech

	// (th4) DC mission 12 opens with "the anvil will do its part" -- so he takes it. Keyed on
	// the briefing constant rather than the mission index so reordering DarkMissions[] cannot
	// silently move the confiscation to the wrong mission. Idempotent: re-briefing runs this
	// path again, and by then the Anvil is already gone, so the find simply fails.
	if( DarkMissions[mission].briefSpeech == SP_DC_MISSION_12_BRIEF ){
		int anvilInventoryIndex;
		if( FindItemInInventory( CurrentPlayerIndex, BI_16_ANVIL_OF_FURY, anvilInventoryIndex ) ){
			ClearInventorySlot( CurrentPlayerIndex, anvilInventoryIndex );
		}
	}

	StartSpeech( DarkMissions[mission].briefSpeech );
	// Briefing is what opens the portal: state 1 is the signal the hub reads.
	DarkMissionState[mission] = DCM_1_BRIEFED;
	// The hub's generator also places this warp, but that only runs on level LOAD -- so the
	// portal has to be created here too, or it would not appear until the player left and
	// came back. Guarded against duplicates because re-briefing runs this path again.
	OpenDarkCampaignPortal( mission );
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
		//case PFC_INQUISITOR:		spriteIndex += 169; break;
        }
    }
    
	// (rt) ========== INSERT FOR THE WARLORD ==========
// (rt) If the player is a Warlord, use the Lesser Summon spell (MI_122_LESSER_SUMMON)
// (rt) then fully override spriteIndex with our warrior mercenary
	if (HasTrait(ownerIndex, TraitId::Warlord) && missile.BaseMissileIndex == MI_122_LESSER_SUMMON) {
		// (rt) Find the spriteIndex for BM_392_WARLORD_MERC
		int foundIdx = -1;
		for (int i = 0; i < MonsterSpritesCount; i++) {
			if (MonsterSprites[i].baseMonsterIndex == BM_392_WARLORD_MERC) {
				foundIdx = i;
				break;
			}
		}
		if (foundIdx != -1) {
			spriteIndex = foundIdx;
			//(rt) summonType = 4; // optional
		}
	}
	// =========================================

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
		case BM_392_WARLORD_MERC:
		{
			int mercLevel = clvl;
						
			summonLife = ( ( 90 + mercLevel * 5 + RNG( 11 ) + RNG( 5 * mercLevel + 1 ) + ( ( mercLevel * mercLevel ) / 5) ) << 6 );
			toHit = (2 * mercLevel) + 15 + (mercLevel * mercLevel / 42) + RNG(6);
			minDamage = mercLevel + 3 + (mercLevel * mercLevel / 80) + RNG(6);
			maxDamage = minDamage + 3 + mercLevel + (mercLevel * mercLevel / 80) + RNG(11);
			toHitSecond = toHit;
			minDamageSecond = minDamage;
			maxDamageSecond = maxDamage;
			armorClass = 2 * mercLevel + (mercLevel * mercLevel / 50) + 2 + RNG(6);
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
	summonLife += ((PerkValue(PERK_TOUGH_MINIONS, ownerIndex) + PerkValue(PERK_TOUGH_MERCENARIES, ownerIndex)) << 6); // +hp from perks
	params.summonLife = summonLife * (100 - best_defense_trait_penalty) / 100; //added here because it changes TOTAL hit points. including affix and perk buffs
 
	toHit += owner.summonToHit;
	toHit += toHit * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	toHit += PerkValue(PERK_ACCURATE_MINIONS, ownerIndex) + PerkValue(PERK_MERC_ACCURACY, ownerIndex); // +accuracy from perks
	params.toHit = toHit;
 
	int best_defense_benefit = HasTrait(ownerIndex, TraitId::BestDefense) ? (25 + owner.CharLevel / 15) : 0;
	minDamage += minDamage * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100; 
	minDamage += minDamage * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMin;
	minDamage += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, ownerIndex) + PerkValue(PERK_STRONG_MERCENARIES, ownerIndex); // +damage from perks

	maxDamage += maxDamage * PerkValue(PERK_GOLEM_MASTERY, ownerIndex, 1) / 100;
	maxDamage += maxDamage * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMax;
	maxDamage += PerkValue(PERK_STRONG_MINIONS, ownerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, ownerIndex) + PerkValue(PERK_STRONG_MERCENARIES, ownerIndex); // +damage from perks

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
	armorClass += PerkValue(PERK_ARMORED_MINIONS, ownerIndex) + PerkValue(PERK_MERC_ARMOR, ownerIndex); // +armor from perks
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

//---- (th4) --------------------------------------------------------------
// Carried summons: summons used to die on every level change. Nothing killed them --
// InitMonsterSlots() -> ClearMonsters() wipes the whole Monsters[] array and the summon
// slots go with it. Here we snapshot the local player's live summons before the wipe and
// rebuild them next to him once the new level has settled.
//
// Singleplayer only. Dark Campaign escort bosses (the mission 10 succubi, Hephasto in the
// Hellforge) are NOT summons: they are ordinary uniques living in slots >= SummonMonsters_Count
// with their own arrival and walk-home logic, so nothing below can reach them.
struct CarriedSummon
{
	bool       used;
	int        baseMonsterIndex;	// re-resolved to a sprite on the destination level
	char       summonOrder;
	int        currentLife;
	DSummonStr params;
};
static CarriedSummon CarriedSummons[SummonMonstersPerPlayer_Count];

// Which summon type owns this slot offset. The slot ranges are fixed per type, so a summon
// never changes slot -- that is what lets us carry it across a level by slot index alone.
static int SummonTypeOfSlot( int slotOffset, int playerIndex )
{
	for( int summonType = 0; summonType < SUM_COUNT; ++summonType ){
		int startIndex = 0;
		int slotsAmount = 0;
		GetSummonsTypeOffsets( summonType, &startIndex, &slotsAmount, playerIndex );
		if( slotOffset >= startIndex && slotOffset < startIndex + slotsAmount ){
			return summonType;
		}
	}
	return SUM_GOLEM;	// PERK_EXTRA_GOLEMS gives the golems every slot
}

// Carried summons are singleplayer only -- MP keeps its own per-level summon deltas -- and
// never in Classic, which must behave like vanilla Diablo: the golem stays behind.
bool CarriedSummonsEnabled()
{
	return MaxCountOfPlayersInGame == 1 && GameMode != GM_CLASSIC;
}

// A tile next to the owner that a summon may be built on: on the map, empty in both maps,
// walkable, and in his line of sight. Searched as expanding rings so summons land close.
// Nothing is created until a tile is found, so a failed search cannot leave a half-placed
// monster or a stomped MonsterMap cell behind.
static bool FindFreeTileNearOwner( const Player& owner, int& outRow, int& outCol )
{
	for( int radius = 1; radius <= 5; ++radius ){
		for( int rowDelta = -radius; rowDelta <= radius; ++rowDelta ){
			for( int colDelta = -radius; colDelta <= radius; ++colDelta ){
				if( max(abs(rowDelta), abs(colDelta)) != radius ) continue;	// ring edge only
				const int row = owner.Row + rowDelta;
				const int col = owner.Col + colDelta;
				if( ! In112(row, col) ) continue;
				if( MonsterMap[ row ][ col ] || PlayerMap[ row ][ col ] ) continue;
				if( IsCellBlocking(row, col) ) continue;
				if( ! CheckCastInSight(owner.Row, owner.Col, row, col) ) continue;
				outRow = row;
				outCol = col;
				return true;
			}
		}
	}
	return false;
}

void ClearCarriedSummons()
{
	for( auto& carried: CarriedSummons ) carried = {};
}

// Called before the level wipe, while Monsters[] still holds the level being left.
void CaptureCarriedSummons()
{
	ClearCarriedSummons();
	if( ! CarriedSummonsEnabled() ){
		return;
	}
	const int summonsOffset = CurrentPlayerIndex * SummonMonstersPerPlayer_Count;
	fori( SummonMonstersPerPlayer_Count ){
		const int summonIndex = summonsOffset + i;
		if( ! IsSummonAlive( summonIndex ) ){
			continue;
		}
		const Monster& summon = Monsters[summonIndex];
		// MF_6_FRIENDLY is set by AwakeSummon, so every genuine summon has it. A slot that
		// was only ever reserved by AddMonster() does not -- and it looks like a golem,
		// because that reservation passes sprite index 0.
		if( ! summon.SpritePtr || !(summon.flag & MF_6_FRIENDLY) || (summon.flag & MF_15_KILLED) ){
			continue;
		}
		CarriedSummon& carried = CarriedSummons[i];
		carried.used              = true;
		carried.baseMonsterIndex  = summon.SpritePtr->baseMonsterIndex;
		carried.summonOrder       = summon.summonOrder;
		carried.currentLife       = summon.CurrentLife;
		// spriteIndex is filled in on arrival -- it is an index into the destination
		// level's MonsterSprites[], not something that travels between levels.
		carried.params.spriteIndex      = 0;
		carried.params.summonType       = SummonTypeOfSlot( i, CurrentPlayerIndex );
		carried.params.summonLife       = summon.BaseLife;
		carried.params.toHit            = summon.ToHit;
		carried.params.minDamage        = summon.MinDamage;
		carried.params.maxDamage        = summon.MaxDamage;
		carried.params.toHitSecond      = summon.SecondToHit;
		carried.params.minDamageSecond  = summon.SecondMinDamage;
		carried.params.maxDamageSecond  = summon.SecondMaxDamage;
		carried.params.armorClass       = summon.ArmorClass;
	}
}

// Called at the end of the level load, once the player's tile is final.
void RestoreCarriedSummons()
{
	if( ! CarriedSummonsEnabled() ){
		ClearCarriedSummons();
		return;
	}
	// (th4) Summons are left behind on a town trip, and on a visit to the Dark Campaign
	// hub -- the Hideout is a peace zone: no attacking, and only Identify, Item Repair and
	// Healing may be cast there (see the DUN_117 gate in Item.cpp).
	if( Dungeon == DUN_0_TOWN || Dungeon == DUN_117_DARKLORD_HIDEOUT ){
		ClearCarriedSummons();
		return;
	}
	const Player& owner = Players[CurrentPlayerIndex];
	const int summonsOffset = CurrentPlayerIndex * SummonMonstersPerPlayer_Count;
	bool anyRestored = false;

	fori( SummonMonstersPerPlayer_Count ){
		CarriedSummon& carried = CarriedSummons[i];
		if( ! carried.used ){
			continue;
		}
		// Slot layout is fixed per summon type, so slot i on the old level is slot i here.
		const int summonIndex = summonsOffset + i;

		int spriteIndex = -1;
		for( int s = 0; s < MonsterSpritesCount; s++ ){
			if( MonsterSprites[s].baseMonsterIndex == carried.baseMonsterIndex ){
				spriteIndex = s;
				break;
			}
		}
		if( spriteIndex == -1 ){	// sprite not loaded here -- drop the summon rather than guess
			continue;
		}
		carried.params.spriteIndex = spriteIndex;

		// Find the tile FIRST. Building the summon on the owner's own tile and moving it
		// afterwards meant that when the move was refused the summon stayed stacked, and the
		// next one overwrote its MonsterMap entry -- an invisible, unhittable monster. Now
		// nothing is created unless there is somewhere to put it.
		int row = 0, col = 0;
		if( ! FindFreeTileNearOwner( owner, row, col ) ){
			continue;	// walled in: this summon is left behind rather than stacked
		}
		// A saved level may have left this same slot standing somewhere else. Only cells
		// holding this monster's own index are cleared, so the summons already placed by
		// this loop are not disturbed.
		ClearMonsterOnMap( summonIndex );
		AwakeSummon( summonIndex, row, col, carried.currentLife, carried.params, true );
		Monsters[summonIndex].summonOrder = carried.summonOrder;
		Monsters[summonIndex].CurAction = A_0_STAY;
		anyRestored = true;
	}

	if( anyRestored ){
		for( int summonType = 0; summonType < SUM_COUNT; ++summonType ){
			RecalcSummonOrder( CurrentPlayerIndex, summonType );
		}
	}
	ClearCarriedSummons();
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
int __fastcall CheckMonsterPhaseOrBlock(uint monsterIndex, int* isMonsterHit, int attackerPlayerIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int result = 0;
	if( monsterIndex < Monsters_Max_Count ){
		if( monster.ai == AI_12_Gargoyle && monster.flag & MF_3_STONE ){ // (rt) gargoyle in stone mode
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
		 || baseMonster == BM_201_ADEPT
		 || (baseMonster >= BM_C_313_COUNSELOR && baseMonster <= BM_C_316_ADVOCATE) ){ // mage invulnerability when phasing
			result = 1;
			// (th4) SP only: a Thief-trait attacker strikes through the mage family's
			// (Ai_25_Mage) phasing -- the stealth kit sees them mid-phase. Attacks by
			// monsters, traps, and summons (attackerPlayerIndex -1) keep the old rule.
			const bool thiefPierce = MaxCountOfPlayersInGame == 1 && attackerPlayerIndex >= 0
				&& HasTrait((uint)attackerPlayerIndex, TraitId::Thief);
			if( monster.goal != MG_1_ATTACK && !thiefPierce ){
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
		// (th4) The Dark Campaign sword+shield Warrior blocks on exactly the Corrupt Paladin's
		// terms -- same action guards, same StartMonsterSecondAttack_4_orBlock pose -- but at
		// a flat 50% instead of the mode-scaled 60-75 the Paladin and Warped One use.
		if (is(baseMonster, BM_H_399_WAR_L_SS, BM_H_401_MONK_M_MS)) { 
			monBlockRate = 50; 
		}
		// (th4) DC mission 6: Sir Cabbot (mace+shield Templar) blocks on the same terms, flat 55%.
		else if (is(baseMonster, BM_H_404_WAR_M_MS, BM_H_408_WAR_M_SS, BM_H_410_MONK_H_SS)) { 
			monBlockRate = 55; 
		}
		else if (is(baseMonster, BM_H_414_WAR_H_MS, BM_H_416_MON_H_MS)) {
			monBlockRate = 43;
		}
		else if (is(baseMonster, BM_H_417_WAR_H_SS_WARRIOR_ELITE)) {
			monBlockRate = 32;
		}

		if( (is(baseMonster, BM_162_CORRUPT_PALADIN, BM_208_WARPED_ONE
		  , BM_H_399_WAR_L_SS , BM_H_401_MONK_M_MS, BM_H_416_MON_H_MS, BM_H_417_WAR_H_SS_WARRIOR_ELITE
		 , BM_H_404_WAR_M_MS , BM_H_408_WAR_M_SS , BM_H_410_MONK_H_SS, BM_H_414_WAR_H_MS))
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
		VideoWindowExists = 0; // (rt) sometimes not reset in PlayVideoHandler (in th1 too)
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
		VideoWindowExists = 0; // (rt) this is apparently instead of WM_QUIT/WM_CLOSE
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
