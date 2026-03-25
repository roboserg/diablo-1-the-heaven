#include "stdafx.h"

// 0x004A08C0
char TheHeavenVersion[MAX_PATH] = THE_HELL_VERSION_HUMAN_STRING;

//----- (00720700) --------------------------------------------------------
void __fastcall AcidPoolLimiter( int missileIndex )
{
	int v1; // edi@1
	int v2; // ecx@1
	int v3; // eax@1
	int v4; // ebx@1
	int v5; // edx@1
	int v6; // ebp@2
	int *v7; // ecx@2
	int v8; // esi@4

	v1 = missileIndex;
	v2 = missileIndex;
	v3 = Missiles[ v2 ].Row;
	v4 = Missiles[ v2 ].Col;
	v5 = Missiles[ v2 ].BaseMissileIndex;
	if( MissileAmount > 0 ){
		v6 = MissileAmount;
		v7 = MissileIndexes;
		do{
			if( *v7 != v1 ){
				v8 = *v7;
				if( Missiles[ v8 ].BaseMissileIndex == v5 && Missiles[ v8 ].Row == v3 && Missiles[ v8 ].Col == v4 ){
					Missiles[ v8 ].TimeToLive = 1;
				}
			}
			++v7;
			--v6;
		}while( v6 );
	}
}

//----- (00721E00) --------------------------------------------------------
void __fastcall DoTremainDeadEffect( int townerIndex, int baseMissileIndex )
{
	CastMissile( Towners[townerIndex].Row, Towners[townerIndex].Col, Towners[townerIndex].Row, Towners[townerIndex].Col, 0, baseMissileIndex, 0, 0, 0, 0, 0 );
}

//----- (00724444) --------------------------------------------------------
void InquisitionScript()
{
	if (MaxCountOfPlayersInGame > 1 && Dungeon == DUN_0_TOWN && GetTowner(TI_13_TREMAIN) == 0 && Quests[Q_21_THE_CURSED_BLADE].status != QS_0_NO && Quests[Q_21_THE_CURSED_BLADE].status != QS_3_COMPLETE) {//In MP, must load priest as current player joins game, if another player in game just sets Tremain on fire.  Not doing so will result in Griswold being erased cuz GetTowner() will return an index of 0 = Griswold's ID!
		LoadPriest();//This call should only be made inside Town to avoid stash tab pop out crash, and should only be made once (by the GetTowner(TI_13_TREMAIN) == 0 check)!
	}

	Towner* towner;
//	if( HorazonGrimspikeDelay <= 0 ){
	if (FleshDoomDelay <= 0) {
		switch( Quests[ Q_21_THE_CURSED_BLADE ].status3 ){
		case 1:
			Quests[ Q_21_THE_CURSED_BLADE ].status3 = 2;
//			HorazonGrimspikeDelay = 66;
			FleshDoomDelay = 66;
			DoTremainDeadEffect( GetTowner(TI_13_TREMAIN), MI_5_FIRE_WALL_SEGMENT );
			break;
		case 2:
			Quests[ Q_21_THE_CURSED_BLADE ].status3 = 3;
//			HorazonGrimspikeDelay = 81;
			FleshDoomDelay = 81;
			towner = &Towners[GetTowner(TI_13_TREMAIN)];
			SpawnSomething(BI_399_SHADOWFANG, towner->Row + 1, towner->Col);//Other players may miss NPC Shadow Fang drop if they are not in town on time.  Not a big issue.
			break;
		case 3:
			Quests[ Q_21_THE_CURSED_BLADE ].status3 = 4;
//			HorazonGrimspikeDelay = 7;
			FleshDoomDelay = 7;
			DoTremainDeadEffect( GetTowner(TI_13_TREMAIN), MI_48_INCINERATE_SEGMENT );
			break;
		case 4:
			Quests[ Q_21_THE_CURSED_BLADE ].status2 = 8;
			towner = &Towners[GetTowner(TI_13_TREMAIN)];
			MonsterMap[ towner->Row ][ towner->Col ] = 0;//If P1 sets Tremain on fire and P2 joins immediately after, GetTowner(TI_13_TREMAIN) returns 0 = Griswold for P2, and Griswold in P2's game gets erased, Shadow Fang drops near Griswold.  LoadPriest() should probably not check for .status2 < 7, but <= 7 instead...
			RemoveGolemAndMissilesMayBe( 0 );
			break;
		}
	}else{
//		--HorazonGrimspikeDelay;
		--FleshDoomDelay;
	}
}

MapCoord TheDenPortalPositions[] = { { 51, 51 }, { 51, 59 }, { 59, 51 }, { 59, 59 } };
//----- (th2) -------------------------------------------------------------
void OpenDenPortals()
{
	for( auto& portal: TheDenPortalPositions ){
		CastMissile( portal.Row, portal.Col, portal.Row, portal.Col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
	}
}

//----- (th2) -------------------------------------------------------------
void CloseDenPortals()
{
    for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; ++missileIndexIndex ){
        int missileIndex = MissileIndexes[missileIndexIndex];
        Missile& missile = Missiles[missileIndex];
        if( missile.BaseMissileIndex == MI_65_RED_PORTAL ){
            missile.TimeToLive = 0;
        }
    }
}

//----- (th2) -------------------------------------------------------------
int FindSpriteIndexByBaseMonsterIndex( int baseMonsterIndex )
{
    for( int i = 0; i < MonsterSpritesCount; ++i){
	    const MonsterSprite& sprite = MonsterSprites[i];
	    if( sprite.baseMonsterIndex == baseMonsterIndex ){
	        return i;
        }
    }
    return -1;
}

//----- (th2) -------------------------------------------------------------
bool IfCellFreeToSummonMonster( int row, int col )
{
	return !PlayerMap[row][col] && !ObjectsMap[row][col] && !IsCellBlocking(row, col) && !MonsterMap[row][col];
}

//----- (th2) -------------------------------------------------------------
template<typename T, int N> inline void TryToSummonTheDenMonster(const T(&arr)[N]){ TryToSummonTheDenMonster(arr, N); }
void TryToSummonTheDenMonster( const BASE_MONSTER baseMonsterIndices[], int n )
{
    int monsterLeft = MonstersCount - SummonMonsters_Count;
	int mon_limit = DUN_73_LAVA_PITS ? 150 : 70;// change if required for Lava Pits
    if( monsterLeft < mon_limit){
		for( int i = 0; i < 100; ++i ){
			MapCoord coord = TheDenPortalPositions[RNG(countof(TheDenPortalPositions))];
			//if( IfCellFreeToSummonMonster( coord.Row, coord.Col ) ){
				int baseMonsterIndex = baseMonsterIndices[ RNG(n) ];
				int direction = 0;
				int spiteIndex = FindSpriteIndexByBaseMonsterIndex( baseMonsterIndex );
				int monsterIndex = AddMonster( coord.Row, coord.Col, direction, spiteIndex, 1 );
				Monsters[monsterIndex].ActivationCounter = 255;
				MonsterWalksTowardsTarget( monsterIndex, direction );
				break;
			//}
		}
	}
}

//----- (th2) -------------------------------------------------------------
MapCoord FindFreeCellForSummonMonster( MapCoord target )
{
	for( int radius = 1; radius < 50; radius++ ){
		for( int colOffset = -radius; colOffset <= radius; colOffset++ ){
			for( int rowOffset = -radius; rowOffset <= radius; rowOffset++ ){
				int row = target.Row + rowOffset;
				int col = target.Col + colOffset;
				if( IfCellFreeToSummonMonster( row, col ) ){
					return { row, col };
				}
			}
		}
	}
	
	return {};
}

//----- (th2) -------------------------------------------------------------
void SummonCerberus( int row, int col )
{
	ClearMonsterOnMap(CerberusMonsterIndex);
	Monster& cerberus = Monsters[CerberusMonsterIndex];
	cerberus.Row = row;
	cerberus.Col = col;
	MonsterMap[ row ][ col ] = CerberusMonsterIndex + 1;
	FixMonsterPosition( CerberusMonsterIndex, 0 );
	
	PlayLocalSound(S_141_STORM, row, col);
	CastMissile( 0, 0, row, col, 0, MI_62_RISING_2, CT_0_PLAYER, 0, 0, 0, 0 );
}

void Summon_Orzog(int row, int col)
{
	ClearMonsterOnMap(OrzogMonsterIndex);
	Monster& orzog = Monsters[OrzogMonsterIndex];
	orzog.Row = row;
	orzog.Col = col;
	MonsterMap[row][col] = OrzogMonsterIndex + 1;
	FixMonsterPosition(OrzogMonsterIndex, 0);

	PlayLocalSound(S_141_STORM, row, col);
	CastMissile(0, 0, row, col, 0, MI_62_RISING_2, CT_0_PLAYER, 0, 0, 0, 0);
}

//----- (th2) -------------------------------------------------------------
void TheDenScript()
{
    auto make_time = []( int minutes, int seconds )
        {
            return minutes * 60 + seconds;
        };

    ++TheDenOneSecondTimer;
    if( TheDenOneSecondTimer == 20 ){
        TheDenOneSecondTimer = 0;
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( theRampagingDemonQuest.status == QS_2_IN_PROGRESS ){
            ++theRampagingDemonQuest.timer;
            
            /*
             * 1. прибываем на уровень по центру. стоим. тишина. никого нет. но с нашим прибытием запущен таймер_0
               2. когда таймер_0 = 7 секунд, запускаем на уровень порталы. красные порталы на 4х площадках вокруг портала и запускаем таймер_1
               3. начинается запуск 1й волны монстров. если таймер_1 = 3 секунды, то (если общее количество монстров на уровне не больше 120) запускаем монстра в рандомный портал и ставим таймер_1 на 0 секунд. монстры выбираются рандомно из этих:
                  а) #4 - Familiar
                  б) #57 - pit demon
                  в) #33 - invisible one
                  г) #20 - ravager
               4. когда таймер_0 = 4 минуты ровно, прекращается наплыв монстров и закрывается таймер_1
               5. когда таймер_0 = 4 минуты и 21 секунда, открываем второй дун и запускаем таймер_2. и (если общее количество монстров на уровне не больше 120) продолжаем запуск каждые 3 секунды следующих монстров из порталов (так же рандомно выбираемых):
                  а) #132 - fire bat
                  б) #62 - blood stone
                  в) #19 - vortex rogue
               6. когда таймер_0 = 5 минут и 08 секунд, запускаем на уровень Цербера - ставим по центру открытой комнаты на юго-западе. (он - последний босс в таблице сейчас.) и наплыв второй волны уже не прекращается пока цербер не будет убит.
               7. как только Цербер мертв, генерация второй волны прекращается. порталы закрываются. закрываются все таймеhы.
               8. когда монстров остается на уровне 0, то игрок победил.
             */
			if( Dungeon == DUN_45_THE_DEN && Quests[Q_24_THE_RAMPAGING_DEMON].status2 > 0 && Quests[Q_24_THE_RAMPAGING_DEMON].status2 < 5 ){
				bool redPortalOpened = false;
				for( int missileIndexIndex = 0; missileIndexIndex < MissileAmount; ++missileIndexIndex ){
					int missileIndex = MissileIndexes[missileIndexIndex];
					Missile& missile = Missiles[missileIndex];
					if( missile.BaseMissileIndex == MI_65_RED_PORTAL ){
						redPortalOpened = true;
						break;
					}
				}
				if( ! redPortalOpened ){
					OpenDenPortals();
				}
			}
			if( theRampagingDemonQuest.status2 == 0 ){
                if( theRampagingDemonQuest.timer == make_time(0, 7) ){
                    // Open 4 portals
                    OpenDenPortals();
                    ++theRampagingDemonQuest.status2;
                    TheDenMonsterSpawnTimer = 0;
                }
            }else if( theRampagingDemonQuest.status2 == 1 ){
                if( theRampagingDemonQuest.timer == make_time(4, 0) ){
                    ++theRampagingDemonQuest.status2;
                }
            }else if( theRampagingDemonQuest.status2 == 2 ){
                if( theRampagingDemonQuest.timer == make_time(4, 21) ){
                    ++theRampagingDemonQuest.status2;
                    TheDenMonsterSpawnTimer = 0;
                    // Open dun
                    ChangeMapRectAfterScriptEvent2(0, 0, 39, 39);
					PlayGlobalSound(S_1278_THE_DEN_OPEN_DUN);
                }
            }else if( theRampagingDemonQuest.status2 == 3 ){
                if( theRampagingDemonQuest.timer == make_time(5, 8) ){
                    ++theRampagingDemonQuest.status2;
                    // Summon Cerber
                    // We can't be sure if cell is free. Need to find nearest free cell before
                    auto target = FindFreeCellForSummonMonster( { 58, 58 } );
                    SummonCerberus( target.Row, target.Col );
					PlayGlobalSound(S_1279);
                }
            }
            
            ++TheDenMonsterSpawnTimer;
			int spawn_speed = 4; // monster summon speed set to 8 for Easy, 5 for Normal, 2 for Hard
			if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				spawn_speed = 6;
			}
			else if (GameMode == GM_HARD) {
				spawn_speed = 2;
			}
			if( TheDenMonsterSpawnTimer == spawn_speed ){
				TheDenMonsterSpawnTimer = 0;
                if( theRampagingDemonQuest.status2 == 1 ){
                    // Summon weak monster from portal if can
                    TryToSummonTheDenMonster( { BM_3_FAMILIAR, BM_56_PIT_DEMON, BM_31_UNSEEN, BM_19_RAVAGER } );
                }else if( is( theRampagingDemonQuest.status2, 3, 4 ) ){
                    // Summon strong monster from portal if can
                    TryToSummonTheDenMonster( { BM_131_FIRE_BAT, BM_36_DARKNESS_CLAN, BM_32_INVISIBLE_ONE } );
                }
            }
        }
    }
}

void Lava_Pits_Script()
{
	if (MonstersCount - SummonMonsters_Count > 0) {
		++Lava_Pits_OneSecondTimer;
		if (Lava_Pits_OneSecondTimer > 341 && Dungeon == DUN_73_LAVA_PITS) {
			++Lava_Pits_MonsterSpawnTimer;
			int spawn_speed = 27, time_minutes = 7;
			if (Lava_Pits_OneSecondTimer < 20 * 60 * time_minutes) {
				if (Lava_Pits_MonsterSpawnTimer == spawn_speed) {
					Lava_Pits_MonsterSpawnTimer = 0;
					// if required, change monster limit inside func "TryToSummonTheDenMonster"
					TryToSummonTheDenMonster({ BM_93_FLAYED_ONE, BM_29_HIDDEN, BM_97_DARK_CHARGER, BM_150_FELLTWIN, BM_202_SUCCUBUS, BM_113_ASH_GOLEM });
				}
			}
			if (Lava_Pits_OneSecondTimer == 20 * 60 * time_minutes + 1) {
				auto target = FindFreeCellForSummonMonster({ 59, 59 });
				Summon_Orzog(target.Row, target.Col);
			}
		}
	}
}

//----- (0071FE20) --------------------------------------------------------
int __fastcall IsOilQuality( int baseItemIndex )
{
	int magicCode = BaseItems[ baseItemIndex ].MagicCode;
	int result = 1;
	if( magicCode >= MC_31_OIL_OF_REPAIR_I && magicCode <= MC_40_OIL_OF_HARDENING ){
		if( OilQuality[ magicCode - MC_31_OIL_OF_REPAIR_I ] > 24 * Difficulty + Dungeon->level ){
			result = 0;
		}
	}
	return result;
}

//----- (0071F730) --------------------------------------------------------
int __fastcall IsBaseValidForSaleToPlayer( int baseIndex, int townerIndex )
{
	BaseItem& baseItem = BaseItems[baseIndex];
	Player& player = Players[CurrentPlayerIndex]; //GetCurrentPlayerOffset();
	int level = player.CharLevel / 2 + 15;
		
	// TODO: check this out, was 255
	if( min( level + player.BaseStrength,  255 * 3 ) < baseItem.RequiredStrengthMax
	 || min( level + player.BaseMagic,     255 * 3 ) < baseItem.RequiredMagicMax
	 || min( level + player.BaseDexterity, 255 * 3 ) < baseItem.RequiredDexterityMax
	 || min( level + player.BaseVitality,  255 * 3 ) < baseItem.RequiredVitalityMax ){
		return 0;
	}
	
	if( !CheckBaseItemClassReq( CurrentPlayerIndex, baseIndex ) )
	{
		return 0;
	}
	
	// advanced class checks below that were impossible to do with current segregation system (checks too complex)
	if (baseItem.ItemCode == IC_3_BOW && (player.fullClassId == PFC_TRAPPER || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Paladin))) {
		return 0;
	}
	if (baseItem.ItemCode == IC_22_MALLET && HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
		return 0;
	}
	if (is(baseItem.ItemCode, IC_3_BOW, IC_1_SWORD, IC_4_MACE) && HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
		return 0;
	}	
	if (baseItem.ItemCode == IC_23_PISTOL && !HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
		return 0;
	}
	if ((player.fullClassId == PFC_GUARDIAN) && (baseItem.ItemCode == IC_1_SWORD || baseItem.ItemCode == IC_4_MACE) && (baseItem.EquippedLocation/*ItemType*/ != EL_2_TWO_HANDED)) {
		return 0;
	}
	if ((player.fullClassId == PFC_IRON_MAIDEN) && (baseItem.ItemCode == IC_7_HELM || baseItem.ItemCode == IC_6_LIGHT_ARMOR
		|| baseItem.ItemCode == IC_8_MID_ARMOR || baseItem.ItemCode == IC_9_HEAVY_ARMOR) && baseItem.MaxThornsHigh == 0) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister) && baseItem.ItemCode == IC_5_SHIELD) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::Ranger) && (baseItem.ItemCode == IC_5_SHIELD || baseItem.ItemCode == IC_1_SWORD || baseItem.ItemCode == IC_4_MACE)) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
		if (baseItem.ItemCode == IC_5_SHIELD) return 0;
		if (baseItem.ItemCode == IC_10_STAFF) return 0;
		if (baseItem.ItemCode == IC_6_LIGHT_ARMOR && baseItem.RequiredMagicMax > 0) return 0;
		if (baseItem.ItemCode == IC_8_MID_ARMOR && baseItem.RequiredMagicMax > 0) return 0;
		if (baseItem.ItemCode == IC_9_HEAVY_ARMOR && baseItem.RequiredMagicMax > 0) return 0;
		if (baseItem.ItemCode == IC_7_HELM && baseItem.RequiredMagicMax > 0) return 0;
		if (baseItem.ItemCode == IC_16_BOOTS && baseItem.RequiredMagicMax > 0) return 0;
		if (baseItem.ItemCode == IC_15_GLOVE && baseItem.RequiredMagicMax > 0) return 0;
	}

	// Getting rid of Mamluk's battle items for Elementalist
	if (player.fullClassId == PFC_ELEMENTALIST && !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
		if (is(baseItem.ItemCode, IC_6_LIGHT_ARMOR, IC_7_HELM, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_1_SWORD, IC_3_BOW) && baseItem.Specialization == IS_BATTLE) {
			return 0;
		}
	}

	if ((player.fullClassId == PFC_WARRIOR) && GameMode != GM_CLASSIC && (baseItem.ItemCode == IC_3_BOW) && (baseItem.AnimationDelay != 1) && (!(HasTrait(CurrentPlayerIndex, TraitId::Ranger)))) {
		return 0;
	}
	if (baseItem.ItemCode == IC_3_BOW && baseItem.AnimationDelay > 0 && HasTrait(CurrentPlayerIndex, TraitId::Ranger)) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery) && baseItem.ItemCode == IC_21_KNIFE) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned) && is(baseItem.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
		return 0;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers) && is(baseItem.ItemCode, IC_1_SWORD, IC_4_MACE)) {
		return 0;
	}
	if (player.fullClassId == PFC_ROGUE) {
		if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
			if (is(baseItem.ItemCode, IC_1_SWORD, IC_4_MACE)) {
				return 0; // rogue with axepertise gets no sharps and blunts
			}
		}
		else {
			if (is(baseItem.ItemCode, IC_2_AXE)) {
				return 0; // rogue without axepertise gets no axe
			}
		}
	}
	return 1;
}

//----- (0071E800) --------------------------------------------------------
void DrawXpBar()
{
	if( GameMode == GM_CLASSIC ) return;
	const Player& player = Players[CurrentPlayerIndex];
	const short& charLevel = player.CharLevel;
	const i64& nextLvlUp = LevelUp(charLevel);
	const i64& prevLvlUp = LevelUp(charLevel-1);

	if (charLevel >= PlayerMaxLevel() || player.Xp < prevLvlUp) return;

	int barSize;
	uchar barColor;
	i64 topValue = nextLvlUp - prevLvlUp;
	i64 curValue = player.Xp - prevLvlUp;

	if (curValue < topValue) {
		barSize = int(curValue * 300 / topValue);
		barColor = barSize > 297 ? 146 : 196 - barSize / 60 % 5;
	}
	else {
		barSize = 300;
		barColor = 138;
	}

	int xPos = (ScreenWidth - GUI_Width) / 2 + 172 + Screen_LeftBorder;
	int yPos = ScreenHeight - GUI_Height + 472 + Screen_TopBorder;
	uchar* barStart;

	for (int i = 0; i < 2; ++i) {
		barStart = &WorkingSurface[YOffsetHashTable[yPos + i] + xPos];
		memset(barStart, barColor, barSize);
	}
}

//----- (0070E380) --------------------------------------------------------
int __fastcall VariatePotionsByDifficulty( int randAdd, int addFromMax )
{
	switch( Difficulty ){
	case 2 or 3 or 4 or 5 or 6 or 7 or 8: addFromMax >>= 1;
	case 0: addFromMax += randAdd; randAdd >>= 1; break;
	case 1: randAdd *= 2; break;
	}
	return (randAdd + addFromMax) << 6; // * 64
}



//----- (00708E60) --------------------------------------------------------
bool Belt5_8Fix()
{
	return CursorY >= ScreenHeight - GUI_Height + 352 || CursorX <= GUI_PanelWidth + ScreenWidth - GUI_Width;
}

//----- (00708E90) --------------------------------------------------------
void __fastcall AddBaseDurWithLimit( Item* item )
{
	int maxDur = MaxDurability(*item); // was 220 in th2
	if( ++item->BaseDurability > (maxDur + 200) ){
		item->BaseDurability = (maxDur + 200);
	}
}

//----- (00708D40) --------------------------------------------------------
int __fastcall CountDestructible(Item* item, int count )
{
	if( item->ItemCode != IC_M1_NONE && item->BaseDurability != ITEM_DUR_INDESTRUBLE && item->BaseDurability){
		++count;
	}
	return count;
}

//----- (00712F00) --------------------------------------------------------
char __fastcall CountMonsterAttackers( int monsterIndex )
{
	uchar attackersMask = Monsters[ monsterIndex ].Attackers;
	char attackers = 0;
	for( int i = 0; i < PlayersMax_4; ++i ){
		attackers += attackersMask & 1;
		attackersMask >>= 1;
	}
	return attackers;
}

//----- (0071FA50) --------------------------------------------------------
void __fastcall DropBossGold( int monsterIndex )// #boss gold drop, #gold from bosses
{
	Monster& monster = Monsters[monsterIndex];
	if( ItemsCount < MaxItems_255
	 && CreateTypeItem(monster.Row, monster.Col, 0, IC_11_GOLD, 0, 0, 1) != -1 ){
		int gold = 35 + 2 * (Dungeon->level + 1) + Difficulty * 65;
		gold += RNG((Dungeon->level + 1) * 2 );
		switch( monster.newBossId - 1 ){
		case UM_0_Gharbad_the_Weak		: gold *= 2;  break;
		case UM_1_Skeleton_King			: gold *= 3;  break;
		case UM_2_Zhar_the_Mad			:
		case UM_3_Snotspill				: gold *= 2;  break;
		case UM_4_ArchBishop_Lazarus	: gold *= 3;  break;
		case UM_8_Warlord_of_Blood:
		case UM_9_The_Butcher			: gold *= 3;  break;
		case UM_11_The_Defiler			: gold *= 4;  break;
		case UM_12_Uber_Diablo			: gold *= 6;  break;
		case UM_154_Diablo				: gold *= 4;  break;
		case UM_156_Zhar_the_Mad		:
		case UM_718_Guardian_of_Flames:
		case UM_157_Warlord_of_Blood	: gold *= 2;  break;
		case UM_535_Abatan				:
		case UM_536_Uber_Butcher		:
		case UM_537_Ceromorth			:
		case UM_556_Pinky				:
		case UM_557_Dregbez				: gold *= 5;  break;
		case UM_578_Uber_Xorene			:
		case UM_579_Andariel			:
		case UM_580_NaKrul				:
		case UM_581_Grimspike			:
		case UM_582_Izual				:
		case UM_583_Lich_King			:
		case UM_584_Mordessa			:
		case UM_587_Hephasto_the_Armorer: gold *= 3;  break;
		case UM_705_Bloodhoof_Twinaxe:	  gold *= 10; break;
		case UM_767_BerUlmeth:			  gold *= 20; break;
		case UM_764_Lord_Nysallor:		  gold *= 40; break;
		case UM_773_The_Dark_Lord:		  gold *= 50; break;
		}
		ActGoldFind( gold );
		Items[ ItemActive[ ItemsCount - 1 ] ].amount = gold;
	}
}

//----- (00706C00) --------------------------------------------------------
int CheckUberCoord()
{
	return UberRow == 0 || UberCol == 0;
}

//----- (0070E6D0) --------------------------------------------------------
int __fastcall CalcCharBaseStatSum( Player& player )
{
	return player.BaseStrength + player.BaseVitality + player.BaseDexterity + player.BaseMagic;
}

//----- (0070E500) --------------------------------------------------------
int CannotIncreaseStat( int playerIndex )
{
	Player& player = Players[playerIndex]; // MSL: MORDOR'S STAT LIMITATION
	int statSum = CalcCharBaseStatSum(player);
	int clvl = player.CharLevel;

	//-------------------------------- NEW #MSL ----------------------------------------------------

	int blueblood_limit_raise = HasTrait(playerIndex, TraitId::BlueBlood) ? 40 : 0; 
	int gifted_limit = HasTrait(playerIndex, TraitId::Gifted) ? 100 : 0; 
	int giant_limit = HasTrait(playerIndex, TraitId::Giant) ? 100 : 0;
	int paladin_limit = HasTrait(playerIndex, TraitId::Paladin) ? (200 + PerkValue(SYNERGY_THAUMATURGY, playerIndex)) : 0;
	int nightkin_limit = 0; 
	if (MaxCountOfPlayersInGame == 1 && (Players[playerIndex].gameChanger & BIT(GC_9_NIGHT_KIN))) { 
		nightkin_limit = 140; 
	}
	int oldfashioned = HasTrait(playerIndex, TraitId::OldFashioned) ? 40 : 0; 
	int leper = HasTrait(playerIndex, TraitId::Leper) ? 150 : 0; 
	int lithe = HasTrait(playerIndex, TraitId::LitheBuild) ? 100 : 0;
	int extra_mode_boost = 0;
	if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) {
		extra_mode_boost = clvl / 2 + 50;
	}
	int statSumLimit = (10 * clvl)
		+ extra_mode_boost
		+ 90
		+ (2 * clvl / 3)
		+ blueblood_limit_raise 
		+ gifted_limit 
		+ giant_limit
		+ paladin_limit 
		+ nightkin_limit 
		+ oldfashioned 
		+ leper
		+ lithe; 
	if (HasTrait(playerIndex, TraitId::BendTheRules)) {
		if (HasTrait(playerIndex, TraitId::Paladin)) { 
			if (statSumLimit > (1400 + paladin_limit)) { 
				statSumLimit = 1400 + paladin_limit; 
			}
		}
		else {
			if (statSumLimit > 1400) { 
				statSumLimit = 1400; 
			}
		}
		statSumLimit += PerkValue(PERK_RAISE_THE_BAR, playerIndex); 
	}
	statSumLimit += 4 * PerkValue(SYNERGY_OSTENTATION, playerIndex); 
	if (GameMode == GM_CLASSIC) { statSumLimit = 460; }
	if (statSum < statSumLimit) {
		return 0;
	}
	else {
		return 1;
	}

	//------------------------------------------------------------------------------------------

#if (0)// old code. turned off
	
	int blueblood_additional_stat_limit_raise = HasTrait(playerIndex, TraitId::BlueBlood) ? 40 : 0;
	if (HasTrait(playerIndex, TraitId::BendTheRules)) {
		statSum = 1400 + PerkValue(PERK_RAISE_THE_BAR, playerIndex);
		return (player->BaseStrength + player->BaseDexterity + player->BaseMagic + player->BaseVitality) >= statSum;
	}
	else if (clvl < 16) {
		return  7 * clvl + 90 + blueblood_additional_stat_limit_raise <= statSum;
	}
	else if (clvl < 21) {
		return  8 * clvl + 100 + blueblood_additional_stat_limit_raise <= statSum;
	}
	else if (clvl < 26) {
		return  9 * clvl + 110 + blueblood_additional_stat_limit_raise <= statSum;
	}
	else if (clvl < 31) {
		return  10 * clvl + 130 <= statSum;
	}
	else if (clvl < 36) {
		return  11 * clvl + 150 <= statSum;
	}
	else if (clvl < 41) {
		return  12 * clvl + 170 <= statSum;
		//}else if( clvl < 46 ){ return 10 * clvl + 130 <= statSum;
	}
	else {
		return 0;
	}
#endif
}

// Calculate spell level cap for prevent reading book
//----- (00711E00) --------------------------------------------------------
int SpellLevelCap()
{
	int clvl = Players[CurrentPlayerIndex].CharLevel;
	return 39 * clvl / 60 + 3;
}
//----- (00718466) --------------------------------------------------------
int __fastcall QlvlForMagicItem( int qlvl )
{
	int result = (qlvl / 3) + 20 * Difficulty + 3;
	//LimitToMax( result, 63 ); // qlvl expanded to int from 6 bits
	return result;
}

//----- (00718440) --------------------------------------------------------
void __fastcall DropItemWithQlvlFilteredByMagicLevel( int itemIndex, int baseItemIndex, int seed, int qlvl, int chanceInProcent, int IsGoodMagicItem, int allowDup, int isInternal, int monsterIndex ) // SmartDropFilter ?
{
	MAGIC_LEVEL* itemML = &Items[ itemIndex ].MagicLevel;
	Monster& monster = Monsters[monsterIndex];
	int qlvlForMagicItem = QlvlForMagicItem( qlvl );

	// filter dropped item by magic level and qlvl
	while( 1 ){
		// not drop magic and rare with simple qlvl for magic and rare
		SetupAllItemsSmartdrop( itemIndex, baseItemIndex, seed, qlvl, chanceInProcent, IsGoodMagicItem, allowDup, isInternal, monster.Attackers );
		if( *itemML != ML_1_MAGIC && !(Items[ itemIndex ].dropType & D_RARE) ){
			return;
		}
		// not drop unique wit qlvlForMagicItem
		SetupAllItemsSmartdrop( itemIndex, baseItemIndex, seed, qlvlForMagicItem, chanceInProcent, IsGoodMagicItem, allowDup, isInternal, monster.Attackers );
		if( *itemML == ML_1_MAGIC || *itemML == ML_0_USUAL || Items[ itemIndex ].dropType & D_RARE ){
			return;
		}
		seed = GetRndSeed();
	}
}

// черновик по быстрой смене оружия
#pragma region swap_weapon

LastItemInfo WeaponSwapItems[ 4 ];
char AltWeaponIndex; // 00A3A4E8
char* WpnSwapBtnImg; // 00A3A660

// ReadWpnDataInfo, копия ReadPlayerInfo (004564E4)
//----- (00721000) ------------------------------------------
int __fastcall ReadWeaponSwapInfoFromSave( HANDLE save, LastItemInfo* swapItems )
{
	HANDLE heroHandle;
	if( !SFileOpenFileEx( save, SaveFile_Swap, 0, &heroHandle ) ){
		return false;
	}

	int isSuccess = false;
	uchar* heroData = 0;
	DWORD fileSize = SFileGetFileSize( heroHandle, 0 );
	if( fileSize ){
		heroData = (uchar*) AllocMem( fileSize );
		uint bytesReaded;
		if( SFileReadFile( heroHandle, heroData, fileSize, (DWORD*)&bytesReaded, 0 ) ){
			int readSize = DecodeFile( heroData, fileSize, CryptKey );
			if( is( readSize, sizeof(WeaponSwapItemV16[2]), sizeof(WeaponSwapItemV16[4]),sizeof(WeaponSwapItemV17[4]), sizeof(LastItemInfo[4]) ) ){
				if( readSize == sizeof(LastItemInfo[4]) ){
					memcpy( swapItems, heroData, readSize );
				}
				else if (readSize == sizeof(WeaponSwapItemV17[4])) {
					auto oldSwap = (WeaponSwapItemV17*)heroData;
					for (uint i = 0; i < readSize / sizeof(WeaponSwapItemV17); ++i) {
						memcpy(&swapItems[i], &oldSwap[i], sizeof(ItemInfo32));
						memset((char*)&swapItems[i] + sizeof(ItemInfo32), 0, sizeof(ItemInfo42) - sizeof(ItemInfo32));
					}
					for (uint i = readSize / sizeof(WeaponSwapItemV17); i < countof(WeaponSwapItems); ++i) {
						memset(&swapItems[i], 0, sizeof(LastItemInfo));
					}
				}
				else{
					auto oldSwap = (WeaponSwapItemV16*)heroData;
					for( uint i = 0; i < readSize / sizeof(WeaponSwapItemV16); ++i ){
						memcpy( &swapItems[i], &oldSwap[i], sizeof(ItemInfo19) );
						memset( (char*) &swapItems[i] + sizeof(ItemInfo19), 0, sizeof(ItemInfo42) - sizeof(ItemInfo19) );
					}
					for( uint i = readSize / sizeof(WeaponSwapItemV16); i < countof(WeaponSwapItems); ++i ){
						memset( &swapItems[i], 0, sizeof(LastItemInfo) );
					}
				}
				isSuccess = true;
			}
		}
	}
	FreeMem( heroData );
	SFileCloseFile( heroHandle );
	return isSuccess;
}

// copied from SaveCharacter (0045619E)
//----- (00720E00) -------------------------------------------
void __fastcall WriteWeaponSwapInfoToSave( LastItemInfo* swapItems )
{
	int blockSize = sizeof(SaveBuffer_WeaponSwap);
	uchar* heroBuffer = SaveBuffer_WeaponSwap;
	memcpy( heroBuffer, swapItems, sizeof( WeaponSwapItems ) );
	EncodeFile( heroBuffer, sizeof( WeaponSwapItems ), blockSize, CryptKey );
	MpqAddFile( "wpnswp", heroBuffer, blockSize );
}

//----- (007217A0) -----------------------------------------------------------
void __fastcall ContinueStay( int playerIndex )
{
	Player& player = Players[playerIndex];
	int currentAnimationFrame = player.currentAnimationFrame;
	int currentAnimationFrameTick = player.currentAnimationFrameTick;
	NewPlayerAnimation( playerIndex, player.nothingAnimationsPointers[ player.dir ], player.StayAnimationFrameCount, 3, player.AnimPitchStay );
	player.currentAnimationFrame = currentAnimationFrame;
	player.currentAnimationFrameTick = currentAnimationFrameTick;
}

//----- (00721800) -----------------------------------------------------------
void __fastcall ContinueWalk( int playerIndex )
{
	Player& player = Players[ playerIndex ];
	int currentAnimationFrame = player.currentAnimationFrame;
	int currentAnimationFrameTick = player.currentAnimationFrameTick;
	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[ player.dir ], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk );
	player.currentAnimationFrame = currentAnimationFrame;
	player.currentAnimationFrameTick = currentAnimationFrameTick;
}

//----- (007216D0) -----------------------------------------------------------
int __fastcall IsCursorInInvPanelRect(RECT* rect)
{
	int inCursor = 0;
	int left = rect->left + InventoryPanelRect.Left;
	if( CursorX >= left && CursorX <= left + rect->right ){
		int top = rect->top + InventoryPanelRect.Top;
		if( CursorY <= top && CursorY >= top - rect->bottom ){
			inCursor = 1;
		}
	}
	return inCursor;
}

//----- (00720900) -----------------------------------------------------------
void ClearAltWeapons()
{
	WeaponSwapItems[ AW_LeftHand ].baseItemIndex = -1;
	WeaponSwapItems[ AW_RightHand ].baseItemIndex = -1;
	WeaponSwapItems[ AW_LeftHand_2 ].baseItemIndex = -1;
	WeaponSwapItems[ AW_RightHand_2 ].baseItemIndex = -1;
}

//----- (00720AAA) -----------------------------------------------------------
void DoSwap()
{
	LastItemInfo swapBuf[ 2 ];

	Player& player = Players[CurrentPlayerIndex];
	
	// tmp <= alt1 <= alt2 <= onBody
	// onBody <= tmp
	
    swapBuf[ 0 ] = WeaponSwapItems[ AW_LeftHand ];
    swapBuf[ 1 ] = WeaponSwapItems[ AW_RightHand ];
    WeaponSwapItems[ AW_LeftHand ] = WeaponSwapItems[ AW_LeftHand_2 ];
    WeaponSwapItems[ AW_RightHand ] = WeaponSwapItems[ AW_RightHand_2 ];
    SaveItem( &WeaponSwapItems[ AW_LeftHand_2 ], &player.OnBodySlots[ IS_LeftHand ] );
    SaveItem( &WeaponSwapItems[ AW_RightHand_2 ], &player.OnBodySlots[ IS_RightHand ] );
    LoadItem( &swapBuf[ 0 ], &player.OnBodySlots[ IS_LeftHand ] );
    LoadItem( &swapBuf[ 1 ], &player.OnBodySlots[ IS_RightHand ] );

	player.altWeaponIndex = (player.altWeaponIndex + 1) % 3;
}

void PlayAltWeaponPairSound()
{
    int GraphicValue = 0;

    const Player& player = Players[CurrentPlayerIndex];
    const Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
    const Item& rightHand = player.OnBodySlots[ IS_RightHand ];

    if( leftHand.ItemCode != IC_M1_NONE ){
        GraphicValue = leftHand.GraphicValue;
    }else if( rightHand.ItemCode != IC_M1_NONE ){
        GraphicValue = rightHand.GraphicValue;
    }
    
    if( GraphicValue ){
        PlayGlobalSound(Flip_InventorySound[ItemFlipTable[GraphicValue]]);
    }
}

void DoPostAltWeaponPairSwapActions()
{
    PlayAltWeaponPairSound();
    
    Player& player = Players[CurrentPlayerIndex];
    Item cursorItemBuf;
    SwapItems( &cursorItemBuf, &player.ItemOnCursor );

    // через курсор все вещи по сети синхронизируются
    memcpy(&player.ItemOnCursor, &player.OnBodySlots[ IS_LeftHand ], sizeof(Item));
    if( player.ItemOnCursor.ItemCode >= 0 ){
        NetSendCmdChItem( 1, IS_LeftHand );
    }else{
        NetSendCmdDelItem( 1, IS_LeftHand );
    }
    memcpy(&player.ItemOnCursor, &player.OnBodySlots[ IS_RightHand ], sizeof(Item));
    if( player.ItemOnCursor.ItemCode >= 0 ){
        NetSendCmdChItem( 1, IS_RightHand );
    }else{
        NetSendCmdDelItem( 1, IS_RightHand );
    }
    SwapItems( &cursorItemBuf, &player.ItemOnCursor );
    RecalcPlayer( CurrentPlayerIndex, 1 );
}

void ChangeAltWeaponPairTo( int which )
{
	if( which < 0 || which >= 3 ) return; // assert
     const Player& player = Players[CurrentPlayerIndex];
    if( player.altWeaponIndex == which ){
        return;
    }
   
    if( player.CurAction > PCA_3_WALK_HORISONTAL || CurrentDialogIndex ){
        return;
    }
    
    PlayGlobalSound( S_75_I_TITLEMOV );
    
    while( player.altWeaponIndex != which ){
        DoSwap();
    }
    
    DoPostAltWeaponPairSwapActions();
}

void ChangeAltWeaponPairToNext( bool skipEmpty )
{
	if( /*Berserk*/HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
		return;
	}
    const Player& player = Players[CurrentPlayerIndex];
    if( player.CurAction > PCA_3_WALK_HORISONTAL || CurrentDialogIndex ){
        return;
    }
    
    const Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
    const Item& rightHand = player.OnBodySlots[ IS_RightHand ];
    
    int altWeaponIndexOld = player.altWeaponIndex;
    if( skipEmpty ){
    	for( int i = 0; i < 3; ++i )
		{
			DoSwap();
			if( leftHand.ItemCode != IC_M1_NONE || rightHand.ItemCode != IC_M1_NONE ){
				break;
			}
		}
	}else{
		DoSwap();
	}
    
    if( player.altWeaponIndex != altWeaponIndexOld ){
        DoPostAltWeaponPairSwapActions();
    }
}

#pragma endregion swap_weapon

#pragma region champion

// 00A3A440
char* ChampionsNameTable[ 10 ] = {
	"%s Defender", "%s Fanatic", "%s Berserker", 0, 0, 0, 0, 0, 0, 0
};

//----- (00720400) ----------------------------------------------
void __fastcall DefenderChamp( Monster& monster )// @Champ
{
	monster.BaseLife *= 3;
	monster.CurrentLife = monster.BaseLife;
    monster.Xp += monster.Xp / 2;
}

//----- (00720500) ----------------------------------------------
void __fastcall FanaticChamp( Monster& monster )// @Champ
{
	int baseToHit = 24 * Difficulty + Dungeon->level + 5;
	monster.ToHit += baseToHit;
	monster.SecondToHit += baseToHit;
	monster.BaseLife *= 2;
	monster.CurrentLife = monster.BaseLife;
    monster.Xp += monster.Xp / 2;
}

//----- (00720600) ----------------------------------------------
void __fastcall BerserkerChamp( Monster& monster )// @Champ
{
	int damage = 30 * Difficulty + (5 * Dungeon->level / 4);
	monster.MinDamage += damage;
	monster.MaxDamage += damage;
	monster.SecondMinDamage += damage;
	monster.SecondMaxDamage += damage;
	monster.BaseLife *= 2;
	monster.CurrentLife = monster.BaseLife;
    monster.Xp += monster.Xp / 2;
}

// 00A3A3A3
void(__fastcall *ChampTypeTable[ 10 ] )(Monster& monster) = {// @Champ
	DefenderChamp,
	FanaticChamp,
	BerserkerChamp,
	0,	0,	0,	0,	0,	0,	0
};

//----- (0071FF00) -----------------------------------------------------------
int __fastcall IsPotencialChamp( int row112, int col112 )// @Champ
{
	//_EXE(MonsterMap) EXE(Monsters) 
	//if( CurVP > 212912 && isEXE ) __debugbreak();
	int monsterNum = MonsterMap[ row112 ][ col112 ];
	int isChamp = 0;
	if( monsterNum > 0 ){
		Monster& monster = Monsters[ monsterNum - 1 ];
		if( !monster.newBossId && !monster.hisBossIndex && !monster.ChampionNumber ){
			isChamp = 1;
		}
	}
	return isChamp;
}

//----- (00720000) ----------------------------------------------------------
void __fastcall RecrutMonster( int row112, int col112 )// @Champ
{
	if( IsPotencialChamp( row112, col112 ) ){
		int monsterNumber = MonsterMap[ row112 ][ col112 ];
		if( monsterNumber <= 0 ){
			return;
		}
		int monsterIndex = monsterNumber - 1;
		Monster& monster = Monsters[ monsterIndex ];
		const int CHAMPION_TYPE_COUNT = 3;
		int champIndex = RNG(CHAMPION_TYPE_COUNT);
		monster.ChampionNumber = champIndex + 1;
		++monster.MonsterLevel;
		ChampTypeTable[ champIndex ]( monster );
	}
}

//----- (00720090) ----------------------------------------------------------
int __fastcall RecruitPack( int row112, int col112 )// @Champ
{
	int potencialChampsCount = 0;
	potencialChampsCount += IsPotencialChamp( row112, col112 );
	potencialChampsCount += IsPotencialChamp( row112 - 1, col112 );
	potencialChampsCount += IsPotencialChamp( row112 - 1, col112 - 1 );
	potencialChampsCount += IsPotencialChamp( row112, col112 - 1 );
	if( potencialChampsCount < 2 ){
		return 0;
	}
	//if( CurVP > 214375) __debugbreak();
	// нужно как минимум 2
	RecrutMonster( row112, col112 );
	RecrutMonster( row112 - 1, col112 );
	RecrutMonster( row112 - 1, col112 - 1 );
	RecrutMonster( row112, col112 - 1 );
	return 1;
}

//----- (007201A0) --------------------------------------------
void __fastcall MakeChampions( int minCount, int maxCount )// @Champ
{
	int realCount = RngFromRange( minCount, maxCount );
	int tryesLeft = 255; // большое число защита от зацикливания
	int championsPlaced = 0;
	do{
		int row112 = RngFromRange( 1, 110 );
		int col112 = RngFromRange( 1, 110 );
		championsPlaced += RecruitPack( row112, col112 );
		--tryesLeft;
	}while( championsPlaced < realCount && tryesLeft > 0 );
}

#pragma endregion champion

//----- (0041B30C) --------------------------------------------------------
void __fastcall GoBackMenuHandler(int a1, int a2)
{
	SelectMainMenu();
}

//----- (th2) -------------------------------------------------------------
std::pair<COOLDOWN_TYPE, int> GetSpellCooldownValue( int playerIndex, int spellIndex )
{
	if( DevelopMode && FreeSpell ) return { CD_NONE, 0 };
	const Player& player = Players[playerIndex];
	switch (spellIndex) {
	case PS_16_REFLECT:
		return { COOLDOWN_TYPE::CD_REFLECT, 120 - PerkValue(SYNERGY_RESUMPTION, playerIndex) };
	case PS_22_FURY:
	{
		const int basefurytime = 180; // default Fury is 3 minutes
		const int battletrancepenalty = 180; // SP Kensei's Battle Trance extends it to 5 minutes (by adding 120 sec) because the effect is very powerful

		int fury_cooldown_timer = basefurytime - PerkValue(PERK_HOT_TEMPERED, playerIndex) - PerkValue(SYNERGY_CYCLE_OF_FURY, playerIndex);
		if (player.gameChanger & BIT(GC_12_MY_SECRET)) {
			fury_cooldown_timer = 0; // cooldown is always 0 seconds for the GC that removes cooldown for Fury
		}
		else if (player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
			fury_cooldown_timer += battletrancepenalty;
		}
		else {
			LimitToMin(fury_cooldown_timer, 30);
		}
		return { COOLDOWN_TYPE::CD_FURY, fury_cooldown_timer };
	}
	case PS_8_STONE_CURSE:
	{
		int sc_cooldown_timer = 0;
		if (HasTrait(playerIndex, TraitId::Petrifier)) {
			sc_cooldown_timer = 60;
		}
		else if (HasTrait(playerIndex, TraitId::BreakerOfStones)) { 
			sc_cooldown_timer = 120;
		}
		if (HasTrait(playerIndex, TraitId::Petrifier) || HasTrait(playerIndex, TraitId::BreakerOfStones)) {
			sc_cooldown_timer -= PerkValue(PERK_STONING_MASTERY, playerIndex);
			LimitToMin(sc_cooldown_timer, 10);
		}
		return { COOLDOWN_TYPE::CD_STONE, sc_cooldown_timer };
	}
	case PS_24_APOCALYPSE:
	{
		int apocCd = 0;
		if (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)) && (!HasTrait(playerIndex, TraitId::Feral))) {
			apocCd = MaxCountOfPlayersInGame == 1 ? 180 : 240;
		}
		else {
			apocCd = MaxCountOfPlayersInGame == 1 ? 60 : 70;
		}
		return { COOLDOWN_TYPE::CD_APOCALYPSE, apocCd };
	}
	case PS_15_FORCE_WAVE:
	{
		int fwCd = 0;
		if (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)) && (!HasTrait(playerIndex, TraitId::Feral))) {
			fwCd = MaxCountOfPlayersInGame == 1 ? 30 : 60;
		}
		else {
			fwCd = MaxCountOfPlayersInGame == 1 ? 5 : 10;
		}
		return { COOLDOWN_TYPE::CD_FORCEWAVE, fwCd };
	}
	case PS_18_LIGHTNING_NOVA
		or PS_49_RUNE_OF_NOVA
		or PS_50_RUNE_OF_IMMOLATION
		or PS_39_HOLY_NOVA
		or PS_41_FIERY_NOVA
		or PS_43_ARCANE_NOVA
		or PS_58_FROST_NOVA
		or PS_61_ACID_NOVA:
	{
		int novaCd = 0;
		if (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)) && (!HasTrait(playerIndex, TraitId::Feral)) ) {
			novaCd = MaxCountOfPlayersInGame == 1 ? 33 : 60;
		}
		else {
			novaCd = MaxCountOfPlayersInGame == 1 ? 12 : 17;
		}
		if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {novaCd -= 1;}
		else if (GameMode == GM_HARD) {novaCd += 1;}
		novaCd -= PerkValue(SYNERGY_IMPATIENCE, playerIndex);
		LimitToMin(novaCd, 3);
		return { COOLDOWN_TYPE::CD_NOVA, novaCd };
	}
	case PS_1_FIREBOLT
		or PS_12_FIREBLAST
		or PS_35_ARCANE_STAR
		or PS_30_CHARGED_BOLT
		or PS_3_LIGHTNING
		or PS_14_BALL_LIGHTNING
		or PS_29_ELEMENTAL
		or PS_20_INCINERATE
		or PS_4_FLASH
		or PS_31_HOLY_BOLT
		//or PS_15_FORCE_WAVE
		or PS_59_RANCID_BOLT
		or PS_60_TOXIC_BALL
		/*or PS_36_BONE_SPIRIT*/:
		if (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)) && (!HasTrait(playerIndex, TraitId::Feral)) ) {
			return { COOLDOWN_TYPE::CD_OFFENSIVE, 5 };
		} break;
	case PS_36_BONE_SPIRIT:  // Bone Spirit has 5 second cooldown for every class, and 10 sec for summoners
		{
			int BScd;
			if (MaxCountOfPlayersInGame == 1 && (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)))) {
				BScd = 3;
			}
			else if (MaxCountOfPlayersInGame != 1 && (player.ClassID == PC_2_MAGE && (player.subclassID == PSC_MAGE_SUMMONER || HasTrait(playerIndex, TraitId::Hydramancer)))) {
				BScd = 6;
			}
			else if (MaxCountOfPlayersInGame != 1){
				BScd = 3;
			}
			else { 
				BScd = 0;
			}
			return { COOLDOWN_TYPE::CD_OFFENSIVE, BScd };
		}
	}
    return { CD_NONE, 0 };
}

//----- (0071B020) --------------------------------------------------------
bool __fastcall CooldownCheck( int casterIndex, int spellIndex )
{
	// Caster in other players' view has casterIndex != CurrentPlayerIndex, therefore player index check is NOT needed in MP.
	//if( casterIndex != CurrentPlayerIndex ) return false;

	if( const auto [cdType, cooldown] = GetSpellCooldownValue(casterIndex, spellIndex); cdType != CD_NONE ){
		bool HasCooldown = Players[casterIndex].cooldowns[cdType] > 0;
		#ifdef WRONG_PLACE_FOR_CODE // shall not change state in read-only check function, also this can lead to net desync
		#if SPELL_QUEUE_AND_COOLDOWN_FIX
		if( HasCooldown ){ // Disable wasted casting animation immediately if spell is under cooldown.  This works for the current player, but other players can still see 2x CD spell being cast.
			Players[casterIndex].destAction = PCMD_M1_NOTHING;
		}
		#endif
		#endif
		return HasCooldown;
	}else{
		return false; // No cooldown on spell
	}
}

//----- (007186A0) --------------------------------------------------------
bool __fastcall IsItemFromSet( Item& item, int setId )
{
	if( !item.UniqueItemIndex || !item.IsReqMet || item.ItemCode == IC_M1_NONE || item.MagicLevel != ML_3_SET ){
		return false;
	}
	UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
	if( *(int*)uniq.Name == setId ){
		return true;
	}else{
		return false;
	}
}

//----- (00718550) --------------------------------------------------------
int __fastcall CountItemFromSet( int playerIndex, int setId )
{
	Player& player = Players[playerIndex];
	int countOfItemsFromSet = 0;
	for( int currentOnBodyItemIndex = 0; currentOnBodyItemIndex < IS_Inventory; ++currentOnBodyItemIndex ){
		Item& onBodySlot = player.OnBodySlots[currentOnBodyItemIndex];
		if( IsItemFromSet(onBodySlot, setId) ){
			if( currentOnBodyItemIndex == IS_LeftHand && onBodySlot.EquippedLocation == EL_2_TWO_HANDED ){
				++countOfItemsFromSet;
			}
			++countOfItemsFromSet;
		}
	}
	return countOfItemsFromSet;
}

//---- (00718B00) --------------------------------------------------------
bool IsPinkyOnLevel ()
{
	return Difficulty >= 2 && Dungeon == DUN_6_CATACOMB_2 && MaxCountOfPlayersInGame != 1;
}

//---- (00718D80) --------------------------------------------------------
bool IsDregbezOnLevel ()
{
	return Difficulty >= 2 && Dungeon == DUN_2_CHURCH_2 && MaxCountOfPlayersInGame != 1;
}

//---- (00718E60) --------------------------------------------------------
bool IsUberXoreneOnLevel()
{
	return Difficulty >= 2 && Dungeon == DUN_22_CRYPT_2 && MaxCountOfPlayersInGame != 1;
}

//---- (00718E90) --------------------------------------------------------
bool IsCeromorthOnLevel()
{
	return Difficulty >= 2 && Dungeon == DUN_8_CATACOMB_4 && MaxCountOfPlayersInGame != 1;
}

//----- (0070A1D0) --------------------------------------------------------
Player* GetCurrentPlayerOffset()
{
	return &Players[CurrentPlayerIndex];
}

//----- (00430549) --------------------------------------------------------
int __fastcall LimitPvMMeleeDamage(int playerIndex, int damage)
{
	Player& player = Players[playerIndex];
	int m = (player.CharLevel * player.CharLevel / 100) + 100 + (player.CharLevel / 2); // modifier
	int maxDamage = (int)player.CharLevel * m; // 512 * clvl (100 at clvl 1 and 82'500 at clvl 150)
	maxDamage *= 64; // internal hp
	if( maxDamage < damage ){
		damage = maxDamage;
	}
	return damage;
}

// th2, новая функция, вероятно для какого-то еще не реализованного в TH квеста
void LoadQuestDungeonMainMapAbyss(char* dunNamePtr, int a2)
{
	ClearGenerationMap();
	MapStartRow = 16;
	MapStartCol = 16;
	MapEndRow = 96;
	MapEndCol = 96;
	ClearWallsTransparency();

	ushort* dunData = (ushort*)LoadFile(dunNamePtr);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];
	// заполняем карту архитектуры из дуна
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				Map[ row ][ col ] = (uchar)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = CAVE_WAFM_7_FLOOR;
			}
		}
	}


	// не заполненные тайлы заполняем бейондом
	for( int col = 0; col < Map_40; col++ ){
		for( int row = 0; row < Map_40; row++ ){
			uchar& wafm = Map[ row ][ col ];
			if( !wafm ){
				wafm = CAVE_WAFM_8_BEYOND;
			}
		}
	}

	MapSize = 112;
	InitLevelTiles( CAVE_WAFM_8_BEYOND );
	ClearAllMaps();
	PlayerRowPos = 31;
	PlayerColPos = 83;
	AddQuestMonsters((ushort*)dunData, 0, 0);
	SetMapObjects((int)dunData, 0, 0);

	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int dmap = FineMap[ row ][ col ];
			if( dmap >= 56 && dmap <= 147 || dmap >= 154 && dmap <= 161 || dmap == 150 || dmap == 152 ){
				DoLighting(row, col, 7, -1);
			}
		}
	}

	FreeMem(dunData);
}

//----- (00715880) --------------------------------------------------------
void __fastcall LoadQuestDungeonAltMapHell( char* dunNamePtr, int playerRow, int playerCol )
{
	InitHellMaps(); // тут
	ClearWallsTransparency();
	ushort* dunData = (ushort*)LoadFile(dunNamePtr);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];

	// заполняем карту архитектуры из дуна
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				Map[ row ][ col ] = (char)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = HELL_WAFM_6_FLOOR;// тут
			}
		}
	}

	// не заполненные тайлы заполняем бейондом
	for( int col = 0; col < Map_40; col++ ){
		for( int row = 0; row < Map_40; row++ ){
			uchar& wafm = Map[ row ][ col ];
			if( !wafm ){
				wafm = HELL_WAFM_30_BEYOND;// тут
			}
		}
	}

	memcpy( AltMap, Map, sizeof( Map ) );
	FreeMem(dunData);
}

//----- (00715940) --------------------------------------------------------
void __fastcall LoadQuestDungeonMainMapHell(char* dunFileName, int playerRowPos, int playerColPos)
{
	MapStartRow = 16;
	MapStartCol = 16;
	MapEndRow = 96;
	MapEndCol = 96;
	ClearWallsTransparency();
	InitHellMaps(); // тут вставка
	ushort* dunData = (ushort*)LoadFile(dunFileName);

	for( int col = 0; col < Map_40; col++ ){
		for( int row = 0; row < Map_40; row++ ){
			FixedMap[ row ][ col ] = FF_NO;// тут
			Map[ row ][ col ] = HELL_WAFM_30_BEYOND;// тут
		}
	}

	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];

	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue ){
				FixedMap[ row ][ col ] |= FF_8_FIXED;// тут
				Map[ row ][ col ] = (char)wallAndFloorMapValue;
			}else{
				Map[ row ][ col ] = HELL_WAFM_6_FLOOR;// тут
			}
		}
	}

	PlayerRowPos = playerRowPos;
	PlayerColPos = playerColPos;
	InitLevelTiles( HELL_WAFM_30_BEYOND );// тут
	if (is(Dungeon, DUN_41_FLESHDOOM, DUN_42_WIELDER, DUN_43_HEPHASTO/*, DUN_30_HALLS_OF_ANGUISH, DUN_31_ANDARIELS_THRONE*/)) {
		InitHellWallsTransparency();
		HellSetAdditionalWallsTransparency();
	}	
	ClearAllMaps();
	AddQuestMonsters((ushort*)dunData, 0, 0);
	SetMapObjects((int)dunData, 0, 0);
	FreeMem(dunData);
}

//---- (00717670) ---------------------------------------------------------
bool LeverClick()
{
	if( LeverClickPlayerSound != -1 && SoundIsPlaying(LeverClickPlayerSound) ){
		return 1;
	}
	Player& player = Players[CurrentPlayerIndex];
	int soundIndex = LeverSounds[player.ClassID + 6 * LeverClickPlayerSoundIndex];
	LeverClickPlayerSound = soundIndex;
	PlayLocalSound(soundIndex, player.Row, player.Col);
	if( LeverClickPlayerSoundIndex++ >= 2 ){
		return 0;// блокировка рычага
	}
	return 1;
}

//----- (00717C00) --------------------------------------------------------
void __fastcall OpenNakrul(bool isWeak)
{
	Monster& nakrul = Monsters[NakrulMonsterIndex];
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status2 != 4 ){ // In MP, make sure Na-Krul .status2 won't reset upon re-entry after kill.
		if( ! isWeak ){
			horazonQuest.status2 = 3; // strong na-krul
		} else {
			horazonQuest.status2 = 2; // weak na-krul
		}
	}
	SoundMessage = 0;
	PlayerSoundMessageDelay = 0;
	OpenNakrulRoom (1);			// открытие двери
	// дезактивация всех объектов которыми открывается дверь
	Objects[ObjIndex(43, 19)].selectable = 0;
	Objects[ObjIndex(43, 20)].selectable = 0;
	Objects[ObjIndex(43, 21)].selectable = 0;

	Objects[ObjIndex(44, 24)].selectable = 0;
	Objects[ObjIndex(44, 26)].selectable = 0;
}

// ---- (007198E0) ---------------------------------------------
void TakeCurrentPlayerDeathToll ()
{
	Player& player = Players[CurrentPlayerIndex];
	int deathGoldToll = RNG ( (player.CharLevel * Dungeon->level) + by (Difficulty, 0, 1200, 3800, 6000, 9000, 12000, 15000, 18000, 21000) ) + 1;	
	if( player.TotalGold >= deathGoldToll ){
		ClearGoldByInventoryAsPrice(deathGoldToll);
        msg_errorf( "Death takes its toll of %d gold", deathGoldToll );
	}
	else{
		uint deathExpToll = (player.CharLevel * Dungeon->level + 500 * Difficulty) * by(Difficulty, 8, 16, 32, 64, 96, 128, 160, 192, 256);
		player.Xp -= deathExpToll;
		if( deathExpToll >= player.Xp ){ // XP unsigned patch (1)
			player.Xp = 0;
		}
        msg_errorf( "Death takes its toll of %d experience", deathExpToll );
	}
}

//---- (th2) --------------------------------------------------------------
bool TeleportSummonToOwner( uint monsterIndex, uint ownerIndex )
{
    const Player& owner = Players[ownerIndex];
    Monster& monster = Monsters[monsterIndex];
    if( monsterIndex >= Monsters_Max_Count || !is( monster.CurAction, A_0_STAY, A_1_WALK_UP, A_2_WALK_DOWN, A_3_WALK_HORIZ, A_13_DELAY ) ){
		return false;
	}

    int targetRow = owner.Row;
	int targetCol = owner.Col;

	for( int circleOffsetIndex = 2; circleOffsetIndex < 6; circleOffsetIndex++ ){
		int circleOffset = CircleOffsets[circleOffsetIndex];
		int coordinatesCount = (uchar)CircleAndWallSegmentDeltas[circleOffset];
		for( int coordinateIndex = 0; coordinateIndex < coordinatesCount; coordinateIndex++ ){
			int row = targetRow + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2];
			int col = targetCol + CircleAndWallSegmentDeltas[1 + circleOffset + coordinateIndex*2 + 1];
			if( Dungeon->genType == DT_0_TOWN ){
				LimitToRange( row, 15, 111 - 16 );
				LimitToRange( col, 15, 111 - 16 );
			}
			if( To112(row, col)
			 && CheckCastInSight(owner.Row, owner.Col, row, col)
			 && CheckCellForMeleeMonsterSafeMove( monsterIndex, row, col ) ){
				FixMonsterPosition( monsterIndex, monster.Orientation );
				ClearMonsterOnMap( monsterIndex );
				MonsterMap[ monster.Row ][ monster.Col ] = 0;
				MonsterMap[ row ][ col ] = monsterIndex + 1;
				monster.Row = row;
				monster.Col = col;
				FixMonsterPosition( monsterIndex, monster.Orientation );
				return true;
			}
		}
	}
	return false;
}

//----- (007141C0) --------------------------------------------------------
void __fastcall ComeHereMrGolem (int monsterIndex)  // mor: is  this even used? doesn't seem so
{
}

//----- (0071C740) --------------------------------------------------------
void __fastcall MonsterGreetings(int monsterIndex)// #monster greetings
{
	int sound = GetUniqMonsterSound(monsterIndex);
	if (sound == S_M1_NO_SOUND) {
		switch (Monsters[monsterIndex].SpritePtr->baseMonsterIndex) {
		case BM_51_THE_BUTCHER or BM_195_UBER_BUTCHER: sound = S_951_M_BUTCHER; break;
		case BM_137_UBER_DIABLO: sound = S_997_M_UBERGRT; break;
		case BM_50_SKELETON_LORD: sound = S_577_M_LEO; break;
		case BM_17_SCAVENGER or BM_19_RAVAGER or BM_138_SHADOW_BEAST or BM_139_BONE_GASHER:
			switch (rand() % 14) {
			case 1: sound = S_1042; break;
			case 2: sound = S_1041;	break;
			} break;
		case BM_89_AZURE_DRAKE or BM_90_CAVE_SNAKE or BM_161_CHASM_DRAKE or BM_118_DRAGON_KIN or BM_91_GOLD_SERPENT or BM_68_GORE_VIPER:
			switch (rand() % 21) {
			case 1: sound = S_1043; break;
			case 2: sound = S_1044; break;
			case 3: sound = S_1045; break;
			} break;
		case BM_0_BURNED_CORPSE or BM_215_CADAVER or BM_2_ROTTING_CARCASS or BM_140_GHOUL or BM_169_ZOMBIE:
			switch (rand() % 65) {
			case  4: sound = S_1046; break;
			case  6: sound = S_1047; break;
			case 11: sound = S_1048; break;
			case 17: sound = S_1049; break;
			case 21: sound = S_1428; break;
			case 24: sound = S_1429; break;
			case 26: sound = S_1430; break;
			case 29: sound = S_1431; break;
			} break;
		case BM_152_BLACK_REAPER or BM_136_BLOOD_SLAYER or BM_87_DARK_REAPER or BM_153_DEATH_MINION or BM_154_DEATH_MINION:
			switch (rand() % 19) {
			case 1: sound = S_1050; break;
			case 3: sound = S_1051; break;
			case 7: sound = S_1052; break;
			} break;
		case BM_80_DEVIL_KIN or BM_207_DREGBEZ or BM_128_GREATER_IMP or BM_116_IMP_BRUTE or BM_148_TURBAK_THE_DEUCE or BM_149_THRESHER:
			switch (rand() % 39) {
			case  8: sound = S_1053; break;
			case 14: sound = S_1054; break;
			case 18: sound = S_1055; break;
			case 22: sound = S_1056; break;
			case 24: sound = S_1057; break;
			} break;
		case BM_187_BLOOD_CLAN or BM_36_DARKNESS_CLAN or BM_186_DEATH_CLAN or BM_35_FIRE_CLAN or BM_34_FLESH_CLAN or BM_209_HATRED_CLAN or
				BM_204_SHADOW_CLAN or BM_37_STONE_CLAN or BM_175_TERROR_CLAN or BM_165_APOCALYPSE_CLAN or BM_43_BLOOD_CLAN or BM_44_DOOM_CLAN or
			BM_124_GLOOM_CLAN or BM_199_HELL_CLAN or BM_198_HORN_CLAN or BM_42_NIGHT_CLAN or BM_45_VENOM_CLAN:
			switch (rand() % 39) {
			case 3: sound = S_1058; break;
			case 9: sound = S_1059; break;
			case 24: sound = S_1150; break;
			case 30: sound = S_1151; break;
			} break;
		case BM_33_MOON_CLAN or BM_188_HELL_CLAN or BM_57_HELL_CLAN or BM_166_GOATLORD_BOSS or BM_214_SATYR_LORD:
			switch (rand() % 16) {
			case 0: sound = S_1060; break;
			case 1: sound = S_1061; break;
			} break;
		case BM_120_NECROMORPH or BM_135_GROTESQUE or BM_141_SHAMBLER or BM_142_HELL_MEAT:
			switch (rand() % 22) {
			case  9: sound = S_1062; break;
			case 10: sound = S_1063; break;
			case 13: sound = S_1064; break;
			} break;
		case BM_286_BRINE_CORPSE:
			switch (rand() % 19) {
			case 3: sound = S_1818; break;
			case 5: sound = S_1819; break;
			} break;
		case BM_292_BUGBEAR:
			switch (rand() % 2) {
			case 1: sound = S_1828; break;
			default: sound = S_1827; break;
			} break;
		case BM_287_ARMORED_SKELETON:
			switch (rand() % 19) {
			case 3: sound = S_1820; break;
			case 5: sound = S_1821; break;
			} break;
		case BM_288_EARTH_GOLEM:
			switch (rand() % 17) {
			case 2: sound = S_1822; break;
			case 9: sound = S_1823; break;
			} break;
		case BM_83_LASH_WORM:
			switch (rand() % 21) {
			case 17: sound = S_1065; break;
			case 16: sound = S_1066; break;
			case 15: sound = S_1067; break;
			} break;
		case BM_127_WYRM or BM_211_TORCHANT:
			switch (rand()%19) {
			case 14: sound = S_1068; break;
			case 13: sound = S_1069; break;
			case 12: sound = S_1070; break;
			} break;
		case BM_85_BALOR or BM_88_BALROG or BM_193_ABATAN or BM_176_DESTROYER or BM_159_GUARDIAN or BM_86_SLAYER or BM_18_VORTEX_ROGUE:
			switch (rand()%20) {
			case 11: sound = S_1071; break;
			case 10: sound = S_1072; break;
			case  9: sound = S_1073; break;
			} break;
		case BM_47_ACID_HOUND or BM_49_BILE_SPITTER or BM_172_SPITTING_TERROR or BM_206_ACID_SPITTER or BM_241_ACID_TERROR:
			switch(rand()%19){
			case 10: sound = S_1439; break;
			case 13: sound = S_1440; break;					
			} break;
		case BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER:
			switch (rand()%25) {
			case 1: sound = S_1130; break;
			case 7: sound = S_1131; break;
			case 11: sound = S_1132; break;
			case 15: sound = S_1133; break;
			} break;
		case BM_212_CACODEMON:
			switch (rand()%27) {
			case 1: sound = S_1134; break;
			case 7: sound = S_1135; break;
			case 11: sound = S_1136; break;
			case 15: sound = S_1137; break;
			} break;
		case BM_112_WAILING_BEAST or BM_134_MUD_RUNNER or BM_97_DARK_CHARGER:
			switch (rand()%22) {
			case 1: sound = S_1138; break;
			case 7: sound = S_1139; break;
			case 11: sound = S_1140; break;
			} break;
		case BM_41_DARK_SERAPH or BM_3_FAMILIAR or BM_147_FIEND or BM_40_QUASIT:
			switch (rand()%42) {
			case 1: sound = S_1141; break;
			case 7: sound = S_1142; break;
			case 11: sound = S_1143; break;
			case 21: sound = S_1144; break;
			case 35: sound = S_1145; break;
			} break;
		case BM_98_IMMORTAL or BM_129_LICH or BM_220_SNOW_LICH:
			switch (rand()%29) {
			case 2: sound = S_1146; break;
			case 4: sound = S_1147; break;
			case 6: sound = S_1148; break;
			case 9: sound = S_1149; break;
			} break;
		case BM_60_HELL_STONE or BM_61_BLOOD_STONE or BM_62_MAULER or BM_63_LAVA_LORD or BM_180_CRYPT_STONE:
			switch (rand()%27) {
			case 7: sound = S_1421; break;
			case 11: sound = S_1422; break;
			case 16: sound = S_1423; break;
			} break;
		case BM_16_HORNED_BEAST or BM_65_HELL_SPAWN or BM_66_HORNED_DEATH or BM_67_BLACK_HORN or BM_171_MUD_LORD or BM_185_BLOOD_HORN:
			switch (rand()%38) {
			case 7: sound = S_1424; break;
			case 11: sound = S_1425; break;
			case 16: sound = S_1426; break;
			case 21: sound = S_1427; break;
			} break;
		case BM_52_BILE_DEMON or BM_56_PIT_DEMON or BM_93_FLAYED_ONE or BM_178_OVERLORD or BM_179_FILTH_DEMON or BM_55_GRIM_DEMON:
			switch (rand()%32) {
			case 7: sound = S_1500; break;
			case 11: sound = S_1502; break;
			case 19: sound = S_1501; break;
			case 26: sound = S_1503; break;
			} break; 
		case BM_232_ABYSSAL_SUCCUBUS:
			switch (rand()%21) {
			case 1: sound = S_1441; break;
			case 2: sound = S_1442; break;
			} break;
		case BM_233_BLOOD_LORD or BM_234_STEEL_LORD:
			switch (rand()%58) {
			case 1: sound = S_1443; break;
			case 2: sound = S_1444; break;
			case 7: sound = S_1445; break;
			case 8: sound = S_1446; break;
			case 9: sound = S_1447; break;
			} break; 
		case BM_235_STYGIAN_FURY:
			switch (rand()%28) {
			case 25: sound = S_1448; break;
			case 26: sound = S_1449; break;
			} break; 
		case BM_236_CORPSE_EATER:
			switch (rand()%29) {
			case 24: sound = S_1450; break;
			case 25: sound = S_1451; break;
			case 26: sound = S_1452; break;
			} break; 
		case BM_237_CONTAMINATOR:
			switch (rand()%45) {
			case 24: sound = S_1453; break;
			case 25: sound = S_1454; break;
			case 26: sound = S_1455; break;
			case 29: sound = S_1456; break;
			} break;
		case BM_222_NIGHT_DEMON:
			switch (rand()%35) {
			case 11: sound = S_1457; break;
			case 15: sound = S_1458; break;
			case 21: sound = S_1459; break;
			case 24: sound = S_1460; break;
			case 33: sound = S_1461; break;
			} break;
		case BM_240_DEMON_PRIEST:
			switch (rand()%33) {
			case 10: sound = S_1462; break;
			case 17: sound = S_1463; break;
			case 24: sound = S_1464; break;
			case 31: sound = S_1465; break;
			} break; 
		case BM_242_REANIMATED_HORROR:
			switch (rand()%16) {
			case 11: sound = S_1472; break;
			case 15: sound = S_1473; break;
			case 21: sound = S_1474; break;
			case 24: sound = S_1475; break;
			case 33: sound = S_1476; break;
			} break;
		case BM_243_IZUAL:
			switch (rand()%6) {
			case 1: sound = S_1478; break;
			case 2: sound = S_1479; break;
			case 3: sound = S_1480; break;
			case 4: sound = S_1481; break;
			case 5: sound = S_1482; break;
			} break;
		case BM_248_SALAMANDER:
			switch (rand()%32) {
			case 1: sound = S_1489; break;
			case 2: sound = S_1490; break;
			case 3: sound = S_1491; break;
			case 5: sound = S_1492; break;
			} break;
		case BM_249_SCARAB or BM_250_BONE_SCARAB:
			switch (rand()%30) {
			case 11: sound = S_1493; break;
			case 21: sound = S_1494; break;
			case 23: sound = S_1495; break;
			case 25: sound = S_1496; break;
			} break; 
		case BM_251_WENDIGO:
			switch (rand()%8) {
			case 1: sound = S_1497; break;
			case 2: sound = S_1498; break;
			case 3: sound = S_1499; break;
			} break;
		case BM_291_MYCONID:
			switch (rand() % 29) {
			case 1: sound = S_1824; break;
			case 2: sound = S_1825; break;
			case 3: sound = S_1826; break;
			} break;
		case BM_253_EXPLODER:
			switch (rand()%7) {
			case 1: sound = S_1505; break;
			case 2: sound = S_1506; break;
			case 3: sound = S_1507; break;
			case 4: sound = S_1508; break;
			} break;		
		case BM_259_WATER_WYRM or BM_263_RED_WYRM:
			switch (rand()%41) {
			case 1: sound = S_1533; break;
			case 2: sound = S_1534; break;
			case 3: sound = S_1535; break;
			case 4: sound = S_1536; break;
			case 5: sound = S_1537; break;
			} break;
		case BM_296_ICE_TROLL:
			switch (rand() % 16) {
			case 1: sound = S_1836_ICE_TROLL_ACT1; break;
			case 6: sound = S_1837_ICE_TROLL_ACT2; break;
			case 9: sound = S_1838_ICE_TROLL_ACT3; break;
			} break;
		case BM_267_BONE_BLADE or BM_268_BONE_BLADE_2:
			switch (rand()%35) {
			case 1: sound = S_1739; break;
			case 2: sound = S_1740; break;
			case 3: sound = S_1741; break;
			case 4: sound = S_1742; break;
			case 5: sound = S_1743; break;
			} break;
		case BM_269_BOG_CRAWLER or BM_270_MUD_CRAWLER:
			switch (rand()%100) {
			case 11: sound = S_1744; break;
			case 12: sound = S_1745; break;
			case 13: sound = S_1746; break;
			case 14: sound = S_1747; break;
			case 21: sound = S_1748; break;
			case 22: sound = S_1749; break;
			case 23: sound = S_1750; break;
			case 24: sound = S_1751; break;
			case 35: sound = S_1752; break;
			case 36: sound = S_1753; break;
			} break;
		case BM_271_THORNED_HULK:
			switch (rand()%4) {
			case 3: sound = S_1757; break;
			case 2: sound = S_1758; break;
			case 1: sound = S_1759; break;
			} break;
		case BM_272_SPIDER_QUEEN:
			switch (rand()%7) {
			case 3: sound = S_1773; break;
			case 2: sound = S_1776; break;
			case 1: sound = S_1775; break;
			case 4: sound = S_1774; break;
			} break;
		case BM_274_FLESH_DEVOURER:
			switch (rand()%25) {
			case 10: sound = S_1785; break;
			case 12: sound = S_1786; break;
			case 17: sound = S_1787; break;
			case 24: sound = S_1788; break;
			} break;
		case BM_275_BEHEMOTH:
			switch (rand()%9) {
			case 1: sound = S_1791; break;
			case 3: sound = S_1792; break;
			case 5: sound = S_1793; break;
			case 7: sound = S_1794; break;
			} break; 
		case BM_278_LEAPER:
			switch (rand()%29) {
			case 11: sound = S_1799; break;
			case 23: sound = S_1800; break;
			} break;
		case BM_279_FLOATER or BM_280_FLOATER_X2:
			switch (rand()%16) {
			case 10: sound = S_1803; break;
			case 12: sound = S_1802; break;
			case 14: sound = S_1801; break;
			} break;
		case BM_283_FROST_WARDEN:
			switch (rand() % 8) {
			case 3: sound = S_1808_FROST_GOL_ACT1; break;
			case 5: sound = S_1809_FROST_GOL_ACT2; break;
			} break;		
		case BM_299_TENTACLIST2 or BM_300_TENTACLIST_ABYSS or BM_298_TENTACLIST_BOSS:
			switch (rand() % 40) {
			case 3: sound = S_1844_TENTACLIST_GREET_1; break;
			case 5: sound = S_1846_TENTACLIST_GREET_3; break;
			case 6: sound = S_1847_TENTACLIST_GREET_4; break;
			case 7: sound = S_1848_TENTACLIST_GREET_5; break;
			case 9: sound = S_1845_TENTACLIST_GREET_2; break;
			} break;
		}
	}
	if (sound != S_M1_NO_SOUND && GameMode != GM_CLASSIC) {
		//int base_monster = Monsters[monsterIndex].newBossId;
		PlayLocalSound(sound, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col);
	}
}

//----- (004512D2) --------------------------------------------------------
int GetUniqMonsterSound(int monsterIndex) // boss greet sounds
{
	switch( Monsters[monsterIndex].newBossId - 1 ){ 
	case UM_821_Snotspill_MP		   :return S_965_M_SNOT03;
	case UM_11_The_Defiler			   :return S_993_HF_NAKRUL2;
	case UM_29_Shaav				   :return S_576_M_IZUALX;
	//case UM_92_Emperor_Morgoth	   :return S_992_M_IZUAL01;
	case UM_97_Baal					   :return S_575_M_LFBAAL;
	case UM_114_Rotflesh_Gorefeast	   :return S_582_M_RFGFST;
	case UM_15_Durzu_the_Slayer		   :return S_581_M_GYFAX;
	case UM_22_Soulhew				   :return S_584_M_AMDUCI;
	case UM_153_UrAnnon				   :return S_583_M_UA;
	case UM_10_Aym_the_Lord_of_Torment :return S_846_M_AY;
	case UM_4_ArchBishop_Lazarus	: 
		if (Dungeon == DUN_101_VOID_CHAMBERS) { 
			return S_1804_AR_LAZARUS; 
		}
		break;
	case UM_156_Zhar_the_Mad:
	case UM_766_Zhar_the_Mad_AR:
		if (Dungeon == DUN_15_HELL_3 || Dungeon == DUN_100_HORADRIM_TREASURY) {
			return S_969_M_ZHAR02;
		}
		break;
	case UM_86_Varaya				   :return S_989_M_VAR;
	case UM_136_Sarogyl_the_Decapitator:return S_611_M_SAR;
	case UM_66_Soulpus				   :return S_608_M_XC;
	case UM_19_Jaldabaoth			   :return S_604_M_BZ;
	case UM_250_Shulgoth			   :return S_1209_SHULGOTH;
	case UM_74_Gutboil_the_Spiller	   :return S_1210_GUTBOIL;
	case UM_466_Goredamn_the_Slasher   :return S_1211_GOREDAMN;
	case UM_71_Ratbag_the_Gentle	   :return S_603_M_DN;
	case UM_154_Diablo				   :return S_1412;
	case UM_57_Gorash				   :return S_599_M_50CENT;
	case UM_199_Grimstomp			   :return S_595_M_GSPIKE;
	case UM_13_Katrina				   :return S_594_M_XORENE;
	case UM_65_Malantor				   :return S_593_M_MALANT;
	case UM_142_Tantorus			   :return S_592_X_TNTRS;
	case UM_405_Scareblade			   :return S_48_M_TAUNT3;
	case UM_582_Izual				   :return S_576_M_IZUALX;
	case UM_579_Andariel			   :return S_1039_M_ND;
	case UM_0_Gharbad_the_Weak		   :	
		if (Dungeon == DUN_24_CRYPT_4 && MaxCountOfPlayersInGame != 1) {
			return S_955_M_GARBUD04;
		}
		break;	
	case UM_8_Warlord_of_Blood		   :
		if (Dungeon == DUN_15_HELL_3 && MaxCountOfPlayersInGame != 1) {
			return S_966_M_WARLRD01;
		}
		break;
	case UM_587_Hephasto_the_Armorer	:return S_1089;// hephasto fix (5)
	case UM_463_Hellcinder				:return S_1413;
	case UM_450_Bloodrayne				:return S_1410_BLOODREIGN;
	case UM_321_Magnate					:return S_1411_MAGNAT;
	case UM_423_Gravehorror				:return S_1414;
	case UM_181_Gammorin_the_Hell_Baron	:return S_1415;
	case UM_331_Windspawn				:return S_1416;
	case UM_264_Ghostflame				:return S_1417;
	case UM_290_Wrathfire				:return S_1418;
	case UM_465_Blazerage				:return S_1419;
	case UM_197_Blacktongue_Deathbite	:return S_1420;
	case UM_130_Medammu_the_Hell_Stone	:return S_1432; // dlvl 9
	case UM_225_Melchior_the_Fat		:return S_1433; // dlvl 7
	case UM_628_Moltenstorm_the_Grim:
	case UM_629_Nightburn_the_Mad:
	case UM_630:
	case UM_631: 	
		switch (rand()%7) {
		case 1: return S_1466; break;
		case 2: return S_1467; break;
		case 3: return S_1468; break;
		case 4: return S_1469; break;
		case 5: return S_1470; break;
		case 6: return S_1471; break;
		}
		break;
	case UM_639:
	case UM_640:
	case UM_641:
	case UM_642:
	case UM_643:
		switch (rand()%5) {
		case 1: return S_1486; break;
		case 2: return S_1487; break;
		case 3: return S_1488; break;
		}
		break;
	case UM_692_Blackbite_the_Destroyer:
	case UM_695_Dreadmaul_the_Dark:
	case UM_694_Dragonskull_the_Grim:
	case UM_693_Firedrinker:
		switch (rand()%30) {
		case 1: return S_1655; break;
		case 2: return S_1656; break;
		case 3: return S_1657; break;
		case 4: return S_1658; break;
		case 5: return S_1659; break;
		case 6: return S_1660; break;
		case 7: return S_1661; break;
		case 8: return S_1662; break;
		case 9: return S_1663; break;
		case 12: return S_1664; break;
		case 13: return S_1665; break;
		case 14: return S_1666; break;
		case 15: return S_1667; break;
		case 23: return S_1668; break;
		}
		break;
	}
	return S_M1_NO_SOUND;
}

//----- (00716320) --------------------------------------------------------
bool __fastcall IsThisBoss(int monsterIndex, int bossIndex)
{
	Monster& monster = Monsters[monsterIndex];
	return bossIndex == (monster.newBossId - 1);
}

//---- (th2) ---------------------------------------------------------
int MonstersActivated()
{
	int activatedMonsters = 0;
	for( int monsterIndexIndex = SummonMonsters_Count; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
		Monster& monster = Monsters[ MonsterIndexes[ monsterIndexIndex ] ];
		if( !monster.speechIndex && monster.ActivationCounter ){
			++activatedMonsters;
		}
	}
	return activatedMonsters;
}

//---- (th2) --------------------------------------------------------------
TEXT_COLOR FindElementColor()
{
	Player& player = Players[CurrentPlayerIndex];
    Item& leftHand = player.OnBodySlots[IS_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_RightHand];
	if( all(IC_M1_NONE, leftHand.ItemCode, rightHand.ItemCode) ) return C_0_White;
	auto& hand = leftHand.ItemCode == IC_M1_NONE ? rightHand : leftHand;

	if( hand.ItemCode == IC_18_FLASK ) switch( hand.MagicCode ){ // flasks
		case MC_55_FLASK_OF_FIRE       or MC_58_FLASK_OF_FIREBOMB     : return C_4_Orange;
		case MC_56_FLASK_OF_LIGHTNING  or MC_59_FLASK_OF_LIGHTNINGBOMB: return C_1_Blue;
		case MC_57_FLASK_OF_ACID       or MC_60_FLASK_OF_ACIDBOMB     : return C_5_Yellow;
		case MC_61_FLASK_OF_ARCANEBOMB                                : return C_2_Red;
	}
	for( auto& e: hand.effect ) if( e.id == AE_ELEM_HIT ) switch( e.type ){ // elem weapon
		case EA_MELEE_FIRE      or EA_ARROW_FIRE     : return C_4_Orange;
		case EA_MELEE_LIGHTNING or EA_ARROW_LIGHTNING: return C_1_Blue;
		case EA_MELEE_ARCAN     or EA_ARROW_ARCAN    : return C_2_Red;
		case EA_MELEE_ACID      or EA_ARROW_ACID     : return C_5_Yellow;
		case EA_MELEE_HOLY      or EA_ARROW_HOLY     : return C_3_Gold;
		case EA_MELEE_COLD      or EA_ARROW_COLD     : return C_7_Grey;
		default                                      : return C_0_White;
	}
	return C_0_White;
}

//---- (th2) --------------------------------------------------------------
void DrawInfoLeft()
{
	int curY = 33;
	auto draw = [&]( int color, const char* text, auto... a ){
		sprintf( InfoPanelBuffer, text, a... );
		DrawLevelInfoText( 8, curY, InfoPanelBuffer, color, OnScreenTextOutline );
		curY += 15;
	};
    Player& player = Players[CurrentPlayerIndex];
	if( Dungeon != DUN_0_TOWN && player.notHittableFramesAfterTransition > 0 ){
		draw( C_4_Orange, "Invulnerability period: %d sec", player.notHittableFramesAfterTransition / ENGINE_FPS + 1 );
	}
	if( ShowSystemTime ){
		time_t timer;
		char t[32];
		struct tm* tm_info;
		timer = time(NULL);
		tm_info = localtime(&timer);
		strftime( t, 32, "%Y-%m-%d %H:%M:%S", tm_info );
		draw( C_3_Gold, "%s", t );
	}
	if (IsController() && XWelcomeMessageTime > 0) {
		draw(C_3_Gold, "Gamepad connected");
		draw(C_3_Gold, "(Hold) Select + Start for HELP");
	}

    if( ShowFps ){
        draw( C_3_Gold, "FPS = %i", FPS );
    }

	if( DevelopMode ) draw( C_3_Gold, OriginalLightLevels ? "Original Light Levels" : "Improved Light Levels" );

    if( FreeSpell ){
        draw( C_3_Gold, "Free spells" );
    }
    
    if( IsGodMode ){
        draw( C_3_Gold, "God mode" );
    }

	if( 0 ){
		extern int LastDupSoundIndex, MaxDupSoundIndex;
		draw(C_3_Gold, "LastDupSoundIndex = %i", LastDupSoundIndex);
		draw(C_3_Gold, "MaxDupSoundIndex  = %i", MaxDupSoundIndex);
	}

    if( DevelopMode ){
        DrawDebugInfo(curY); // TODO: include DrawDebugInfo content here and rewrite it using "draw()"
    }
    
    if( /*!is(GameMode, GM_NORMAL, GM_EASY) && */ ShowGameMode) {
		if (player.gameChanger & BIT(GC_24_RUTHLESS)) {
			draw(C_4_Orange, "game mode : ruthless iron");
		}
		else {
			draw(C_4_Orange, "game mode: %s", getGameModeName(GameMode));
		}
    }

	if( ShowResolution ){ draw( C_3_Gold, "Resolution: %i x %i", ScreenWidth, ScreenHeight ); }

    if( is( GameMode, GM_SPEEDRUN ) ) {
        auto drawTimer = [&]( int color, const char* stepName, const char* timerValue ){
            sprintf( InfoPanelBuffer, "%s:", stepName );
            DrawLevelInfoText( 8, curY, InfoPanelBuffer, color );
            DrawLevelInfoText( 110, curY, (char*)timerValue, color );
            curY += 15;
        };
        
        for( int i = 0; i < SRS_COUNT; ++i ){
            const SPEEDRUN_STEP step = SPEEDRUN_STEP( i );
            const int secondsTotal = GetSpeedrunStepValue( step );
            const char* stepName = GetSpeedrunStepName( step );
            
            if( step < CurrentSpeedrunStep ){
                drawTimer( C_3_Gold, stepName, GetTimeValueFormated( secondsTotal ) );
            }else if( step == CurrentSpeedrunStep ){
                const auto color = (player.CurAction == PCA_8_DEATH || IsPlayerDead) ? C_2_Red : C_1_Blue;
                drawTimer( color, stepName, GetTimeValueFormated( secondsTotal ) );
            }else{
                drawTimer( C_0_White, stepName, "-:--:--" );
            }
        }
    }
	if( TimeSpeed != 1.0 && (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL) && MaxCountOfPlayersInGame == 1
	 || DevelopMode) ){
		draw( C_3_Gold, "Time speed: %0.2f", TimeSpeed );
	}
	//extern int fullCellDelta;
	//extern int nextCellPCmd;
	//draw( C_3_Gold, "fullCellDelta = %i", fullCellDelta );
	//draw( C_3_Gold, "nextCellPCmd = %i", nextCellPCmd );
}

array Cooldowns = {
	"Nova",
	"Fury",
	"Reflect",
	"Offensive",
	"Stone Curse",
	"Apocalypse",
	"Force Wave",
};
void DrawInfoRight()
{
	const Player& player = Players[CurrentPlayerIndex];
	int curY = 25;
	auto draw = [&](int color, const char* text, auto... a) {
		sprintf(InfoPanelBuffer, text, a...);
		DrawLevelInfoText(ScreenWidth - GetTextWidth(InfoPanelBuffer) - 3, curY, InfoPanelBuffer, color, OnScreenTextOutline);
		curY += 15;
		};

	for (int i = 0; i < CD_COUNT; ++i) {
		if (const int cd = player.cooldowns[i]; cd > 0) {
			if (i == CD_FURY) {
				if (is(player.fullClassId, PFC_ELEMENTALIST, PFC_MAGE, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK)) {
					draw(C_4_Orange, "Concentration cooldown: %02i:%02i", cd / 60, cd % 60);
				}
				else if (player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
					draw(C_4_Orange, "Battle Trance cooldown: %02i:%02i", cd / 60, cd % 60);
				}
				else {
					draw(C_4_Orange, "%s cooldown: %02i:%02i", Cooldowns[i], cd / 60, cd % 60);
				}
			}
			else {
				draw(C_4_Orange, "%s cooldown: %02i:%02i", Cooldowns[i], cd / 60, cd % 60);
			}
		}
	}

	// monsters left counter display 
	if (is(GameMode, GM_COLISEUM, GM_CLASSIC)) {
		// draw nothing
	}
	else if (GameModeType() == GAME_MODE_TYPE::IRON) { // IM/SU logic
		if (Dungeon != DUN_0_TOWN) {
			int monstersLeft = GetNumberOfMonstersInDungeon();
			int objLeft = GetNumberOfObjectsInDungeon();
			//if (!Dungeon->isQuest) {
			if (IsSomeMonstersActivated()) {
				draw(C_2_Red, "In battle... %i monsters", MonstersActivated());
			}
				draw(monstersLeft > 0 ? C_2_Red : C_1_Blue, "monsters left: %i", monstersLeft);
			//}
			draw(objLeft > MaxIronmanObjects ? C_2_Red : C_1_Blue, "objects left: %i", objLeft);
		}
	}
	else {// other GMs
		if (Dungeon == DUN_0_TOWN) {
			// do nothing
		}
		else if (DisplayMonstersLeft) {
			if (IsSomeMonstersActivated()) {
				draw(C_2_Red, "In battle... %i monsters", MonstersActivated());
			}
			int monstersLeft = GetNumberOfMonstersInDungeon();
			draw(monstersLeft > 0 ? C_2_Red : C_1_Blue, monstersLeft == 1 ? "%i monster left" : "% i monsters left", monstersLeft);
		}
		else if (MaxCountOfPlayersInGame == 1 && !Dungeon->isQuest && (!IsSaveAndLeavingEnabled() || SaveAlwaysEnabled && IsSomeMonstersActivated())) {
			draw(C_2_Red, "In battle... %i monsters", MonstersActivated());
		}
		else if (Dungeon->isQuest) {
			int monstersLeft = MonstersCount - SummonMonsters_Count;
			draw(monstersLeft > 0 ? C_2_Red : C_1_Blue, monstersLeft == 1 ? "%i monster left" : "%i monsters left", MonstersCount - SummonMonsters_Count);
		}
	}
	
	if( DevelopMode && Dungeon == DUN_45_THE_DEN )
    {
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        draw( C_0_White, "Quest Stage %i", theRampagingDemonQuest.status2 );
        draw( C_0_White, "One_Second_Timer %i", TheDenOneSecondTimer );
        draw( C_0_White, "Timer_0 %02i:%02i", theRampagingDemonQuest.timer / 60, theRampagingDemonQuest.timer % 60 );
        draw( C_0_White, "Timer_1 %02i:%02i", TheDenMonsterSpawnTimer / 60, TheDenMonsterSpawnTimer % 60 );
    }
	
	if( Dungeon != DUN_0_TOWN ){
        const int playerSummonsOffset = CurrentPlayerIndex * SummonMonstersPerPlayer_Count;
        
        for( size_t i = 0, ie = SUM_COUNT; i < ie; ++i ){
            int summonsStartIndex = 0;
            int summonsSlotsAmount = 0;
            GetSummonsTypeOffsets( i, &summonsStartIndex, &summonsSlotsAmount, CurrentPlayerIndex);
            const int summonsBeginIndex = summonsStartIndex;
            const int summonsEndIndex = summonsBeginIndex + summonsSlotsAmount;
            
            const Monster* summonPtr = nullptr;
            int aliveCount = 0u;
            for( int summonIndex = summonsBeginIndex; summonIndex < summonsEndIndex; ++summonIndex  ){
                const Monster& summon = Monsters[ summonIndex + playerSummonsOffset ];
                if( (summon.Row != 1 || summon.Col) && (summon.CurrentLife & (-64)) > 0 ){
                    summonPtr = &summon;
                    ++aliveCount;
                }
            }
            
            if( aliveCount == 1 ){
                draw( C_0_White, "%s: %d of %d HP", summonPtr->Name, summonPtr->CurrentLife / 64, summonPtr->BaseLife / 64 );
            }else if( aliveCount > 1 ){
                draw( C_0_White, "%ss: %d", summonPtr->Name, aliveCount );
            }
		}

		if(HasTrait(CurrentPlayerIndex, TraitId::Hydramancer))
		{
			int countOfHydras = CountOfPlayerHydras(CurrentPlayerIndex);
			if (countOfHydras > 0)
			{
				draw( C_0_White, "Hydras: %d", countOfHydras );
			}
		}
    }
    
	#if 0 // CHEATS
    if( DungeonLevel ){
        int buggyMonstersCount = 0;
        for( int row = 0; row < 112; ++row ){
            for( int col = 0; col < 112; ++col ){
                int monsterNumber = MonsterMap[row][col];
                if( monsterNumber ){
                    int monsterIndex = abs(monsterNumber) - 1;
                    const Monster& monster = Monsters[monsterIndex];
                    if( abs(monster.Row - row) > 2 || abs(monster.Col - col) > 2 ){
                        if( row != 1 || col != 0 ){
                            ++buggyMonstersCount;
                            draw( C_2_Red, "Buggy monster %s: real location at (%i, %i), ghoust at (%i, %i)", monster.Name, monster.Row, monster.Col, row, col );
                        }
                    }
                }
            }
        }
        if( buggyMonstersCount > 0 ){
            draw( C_2_Red, "Buggy monsters amount: %i", buggyMonstersCount );
        }
    }
	#endif
    
	int fireWall = 0, lightningWall = 0, fireRing = 0, lightningRing = 0;
	for( int i = 0; i < MissileAmount; ++i ){
		Missile& missile = Missiles[ MissileIndexes[ i ] ];
		if( ! missile.IsDeleted && is( missile.CasterType, CT_0_PLAYER, CT_2_WALL ) && missile.CasterIndex == CurrentPlayerIndex ){
			switch( missile.BaseMissileIndex ){
			case MI_43_FURY:
				if( player.activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) ){
					if (is(player.fullClassId, PFC_ELEMENTALIST, PFC_MAGE, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK)) {
						draw(C_4_Orange, "Concentration: %i", missile.TimeToLive / ENGINE_FPS);
					}
					else if (player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
						draw(C_4_Orange, "Battle Trance: %i", missile.TimeToLive / ENGINE_FPS);
					}
					else {
						draw(C_4_Orange, "Fury: %i", missile.TimeToLive / ENGINE_FPS);
					}
				}
				break;
			case MI_5_FIRE_WALL_SEGMENT:
				if( missile.spellIndex == PS_6_FIRE_WALL ){
					if( ! fireWall++ ){ draw( C_4_Orange, "Fire wall: %i", missile.TimeToLive / ENGINE_FPS ); }
				}else if( is( missile.spellIndex, PS_45_RING_OF_FIRE, PS_48_RUNE_OF_LIGHT, PS_19_FLAME_RING) ){ 
					if( ! fireRing++ ){ draw( C_4_Orange, "Fire ring: %i", missile.TimeToLive / ENGINE_FPS ); }
				}
				break;
			case MI_70_LIGHTNING_WALL_SEGMENT:
				if( missile.spellIndex == PS_40_LIGHTING_WALL ){
					if( ! lightningWall++ ){ draw( C_4_Orange, "Lightning wall: %i", missile.TimeToLive / ENGINE_FPS ); }
				}else if( missile.spellIndex == PS_17_LIGHTING_RING ){ 
					if( ! lightningRing++ ){ draw( C_4_Orange, "Lightning ring: %i", missile.TimeToLive / ENGINE_FPS ); }
				}
				break;
			case MI_39_INFRAVISION:
				draw(C_4_Orange, "Seeing: %i", missile.TimeToLive / ENGINE_FPS);
				break;
			}
		}
	}
	if( player.activeBuffFlag & BF_4_ARMOR_BLESS ){
		draw( C_1_Blue, "Armor Bless" );
	}
	if( player.activeBuffFlag & BF_5_DAMAGE_BLESS ){
		draw( C_1_Blue, "Damage Bless" );
	}
	if (player.activeBuffFlag & BF_6_RESIST_BLESS) {
		draw(C_1_Blue, "Resistance Bless");
	}
	if (player.activeBuffFlag & BF_7_EXP_BLESS) {
		draw(C_1_Blue, "Experience Bless");
	}
	if (player.activeBuffFlag & BF_8_GOLD_BLESS) {
		draw(C_1_Blue, "Greed Bless");
	}
	if (player.activeBuffFlag & BF_9_MAGIC_FIND) {
		draw(C_1_Blue, "Magic find Bless");
	}
	if (player.activeBuffFlag & BF_10_REGEN_BLESS) {
		draw(C_1_Blue, "regeneration Bless");
	}
	if (player.activeBuffFlag & BF_11_SPELL_BLESS) {
		draw(C_1_Blue, "Spell Bless");
	}
	if (player.activeBuffFlag & BF_12_LIGHT_CURSE) {
		draw(C_1_Blue, "light Curse");
	}
	if (player.activeBuffFlag & BF_13_CRIT_BLESS) {
		draw(C_1_Blue, "crit chance Bless");
	}
	if (player.activeBuffFlag & BF_14_ACCU_BLESS) {
		draw(C_1_Blue, "accuracy Bless");
	}
	
	draw(FindElementColor(), "weapon slot: %i", player.altWeaponIndex + 1);

	//extern int nearbyAttackersCount; draw(C_3_Gold, "nearby attackers: %i", nearbyAttackersCount);
	//extern int AutoHitShow; draw(C_3_Gold, "melee autohit: %i%%", AutoHitShow);
}

//---- (00717000) ---------------------------------------------------------
void QuestVideoOnEnter ()
{
	if( Dungeon.next == DUN_32_HORAZONS_HAVEN ){
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		if( horazonQuest.status2 < 1 ){
			horazonQuest.status2 = 1;
			PlayVideo("gendata\\hellfire.smk", 1);
			PaletteFadeIn(8);// фикс сбитой палитры
		}
	}
}

// th2 новое меню для "не смогли в окне старое" --------------------------
BOOL CALLBACK SinglePlayerDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ){
	case WM_INITDIALOG:
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_HEROLIST);
			bool isInitialCharSlot = false;
			int initialCharSlot = 0;
			for( int i = 0; i < MaxSaveSlots - 1; i++ ){
				if( CharNameSlots[i][0] ){
					if( !isInitialCharSlot ){
						isInitialCharSlot = true;
						initialCharSlot = i;
					}
					int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)CharNameSlots[i]);
					SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
				}
			}
			SetFocus(hwndList);
			SendMessage(hwndList, LB_SELECTSTRING, -1, (LPARAM)CharNameSlots[initialCharSlot]);
			strcpy(HeroName, CharNameSlots[initialCharSlot]);
			return false;
		}
	case WM_COMMAND:
		switch( LOWORD(wParam) ){
		case IDC_LOADGAME:
			EndDialog(hDlg, HM_2_LOAD_GAME);
			return true;
		case IDC_NEWGAME:
			EndDialog(hDlg, HM_1_NEW_GAME);
			return true;
		case IDCANCEL:
			EndDialog(hDlg, HM_4_CANCEL);
			return true;
		case IDC_HEROLIST:
			switch( HIWORD(wParam) ){
			case LBN_SELCHANGE:
				{
				HWND hwndList = GetDlgItem(hDlg, IDC_HEROLIST);
				int nItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, nItem, 0);
				strcpy(HeroName, CharNameSlots[i]);
				return true;
				}
			case LBN_DBLCLK:
				EndDialog(hDlg, HM_2_LOAD_GAME);
				return true;
			case WM_VKEYTOITEM:
				if( LOWORD(wParam) == VK_RETURN ){
					EndDialog(hDlg, HM_2_LOAD_GAME);
					return true;
				}
			}
			return true;
		}
	case WM_CLOSE:
		EndDialog(hDlg, HM_4_CANCEL);
		return true;
	}
	return false;
}

// функция из 1.145

//----- (th2) -------------------------------------------------------------
int GetPlayerKnockbackResistChance( uint playerIndex )
{
	const Player& player = Players[playerIndex];

	int safeChance = 0;
	if (HasTrait(playerIndex, TraitId::Mamluk)) {
		safeChance += (player.BaseStrength / 30) + 40;
	}
	else if (player.ClassID != PC_1_ARCHER && player.ClassID != PC_2_MAGE) {
		safeChance += (player.BaseStrength / 30) + 40;
	}
	int ratelhideTraitPenalty = HasTrait(playerIndex, TraitId::RatelHide) ? 35 : 0;
	safeChance += PerkValue(PERK_STONEWALL, playerIndex) 
				- ratelhideTraitPenalty 
				+ PerkValue(SYNERGY_STOICISM, playerIndex)
				+ PerkValue(SYNERGY_ANCHOR, playerIndex);
	
	if (HasTrait(playerIndex, TraitId::Juggernaut)) {
		safeChance = 100;
	}
	if (GameMode == GM_CLASSIC) safeChance = 0;
	LimitToMin( safeChance,   0 );
	LimitToMax( safeChance, 100 );
	return safeChance;
}
//----- (0070B6A0) --------------------------------------------------------
bool PlayerKnockbackResistFail(int monsterIndex, uint playerIndex, bool charge /*= false*/)
{
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	if( !(monster.flag & MF_8_KNOCKBACK) && ! charge ){
		return false;// monster attack doesn't have KB flag
	}
	if( player.ClassID == PC_1_ARCHER || player.ClassID == PC_2_MAGE ){
		if (HasTrait(playerIndex, TraitId::Mamluk)) {
			// nothing, chance to resist KB is processed normally for Mamluk
		}
		else {
			return true;// archers and mages have no KB resist whatsoever
		}
	}
	int safeChance = GetPlayerKnockbackResistChance( playerIndex );
	if( RNG(100) > safeChance ){
		return true;
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
void UpdateScaleResist(int playerIndex)
{
	Player& player = Players[playerIndex];
	// scaling range of full resist (76 - 300) = 76 + DLVL + Difficulty * 25
	player.scaleResist = 1.0f / (100 - DungeonMaxLevel + Dungeons[player.dungeon].level + Difficulty * (DungeonMaxLevel + 1)); // TODO: DungeonLevel change to Dungeons[Dungeon].level
}

// getting min,max values for %dr, #MDR basic, #melee damage resistance
//----- (th2) -------------------------------------------------------------
void CalcResistPercentMelee(int playerIndex)
{
	Player& player = Players[playerIndex];
	// player.resistMeleeMin = player.BaseVitality / 50 /*+ player.CharLevel / 8 + 1*/;
	// player.resistMeleeMax = player.BaseVitality / 25 /*+ player.CharLevel / 3 + 5*/;
	
	int smallFrameTraitPenalty = HasTrait(playerIndex, TraitId::SmallFrame) ? 10 : 0;
	int sandmanTraitBenefitCur = HasTrait(playerIndex, TraitId::Sandman) ? 5 : 0;
	player.resistMeleeMin += sandmanTraitBenefitCur - smallFrameTraitPenalty;
	player.resistMeleeMax += sandmanTraitBenefitCur - smallFrameTraitPenalty;
	if( player.fullClassId == PFC_EXECUTIONER && !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) ){
		player.resistMeleeMin += 5 + player.CharLevel / 15;
		player.resistMeleeMax += 5 + player.CharLevel / 15;
	}
	// mor: these 2 lines are dragon's work, and are not used in the balancing
	player.resistPercentMeleeMin = player.scaleResist * player.resistMeleeMin;
	player.resistPercentMeleeMax = player.scaleResist * player.resistMeleeMax;

	int sandmanTraitBenefitMin = HasTrait(playerIndex, TraitId::Sandman) ? 10 : 0;
	int sandmanTraitBenefitMax = HasTrait(playerIndex, TraitId::Sandman) ? 10 : 0;
	int durable_syn = PerkValue(SYNERGY_DURABLE, playerIndex);
	int axepertiseTraitPlus = HasTrait(playerIndex, TraitId::Axepertise) ? 5 : 0;
	int fecht = HasTrait(playerIndex, TraitId::Fechtmeister) ? 5 : 0;
	int momLuke = HasTrait(playerIndex, TraitId::Mamluk) ? 15 : 0;
	int synergyHardenedSteel = PerkValue(SYNERGY_HARDENED_STEEL, playerIndex, 0);
	int pisto = HasTrait(playerIndex, TraitId::Pistoleer) ? 5 : 0;
	int crup = HasTrait(playerIndex, TraitId::Crupellarius) ? 15 : 0;

	switch( player.fullClassId ){
	case PFC_WARRIOR or PFC_INQUISITOR or PFC_TEMPLAR or PFC_TRAPPER or PFC_SHINOBI
	  or PFC_MONK or PFC_ASSASSIN or PFC_BOMBARDIER or PFC_THRAEX or PFC_SECUTOR or PFC_MURMILLO:
		player.resistPercentMeleeMinLimit = 0.60f + 0.01f * (-smallFrameTraitPenalty + sandmanTraitBenefitMin + fecht + synergyHardenedSteel + durable_syn + crup);
		player.resistPercentMeleeMaxLimit = 0.75f + 0.01f * (-smallFrameTraitPenalty + sandmanTraitBenefitMax + fecht + synergyHardenedSteel + durable_syn + crup);// 32%, sm 22%, 42,5%
		break;
	case PFC_IRON_MAIDEN or PFC_ROGUE or PFC_DIMACHAERUS or PFC_EXECUTIONER:
		player.resistPercentMeleeMinLimit = 0.65f + 0.01f * (sandmanTraitBenefitMin + axepertiseTraitPlus + synergyHardenedSteel);
		player.resistPercentMeleeMaxLimit = 0.75f + 0.01f * (sandmanTraitBenefitMax + axepertiseTraitPlus + synergyHardenedSteel);
		break;
	case PFC_ARCHER or PFC_SCOUT or PFC_SHARPSHOOTER or PFC_MAGE or PFC_ELEMENTALIST or PFC_WARLOCK or PFC_DEMONOLOGIST or PFC_NECROMANCER or PFC_BEASTMASTER:
		player.resistPercentMeleeMinLimit = 0.50f + 0.01f * (-smallFrameTraitPenalty + momLuke + synergyHardenedSteel);
		player.resistPercentMeleeMaxLimit = 0.70f + 0.01f * (-smallFrameTraitPenalty + momLuke + synergyHardenedSteel); // 40%, sf 50%
		break;
	default: // GUARDIAN, KENSEI, SHUGOKI, SAVAGE, BERSERKER, DRUID ?
		player.resistPercentMeleeMinLimit = 0.70f + 0.01f * (-smallFrameTraitPenalty + sandmanTraitBenefitMin + synergyHardenedSteel - pisto);
		player.resistPercentMeleeMaxLimit = 0.80f + 0.01f * (-smallFrameTraitPenalty + sandmanTraitBenefitMax + synergyHardenedSteel - pisto);// 25%, sm 15%, sf 35%
		break;
	}
	if( 0 ){ // for for additional sync resist points and percents
		player.resistMeleeMin = int(player.resistPercentMeleeMin / player.scaleResist);
		player.resistMeleeMax = int(player.resistPercentMeleeMax / player.scaleResist);
	}
	LimitToRange(player.resistPercentMeleeMin, 0.0f, player.resistPercentMeleeMinLimit );
	LimitToRange(player.resistPercentMeleeMax, 0.0f, player.resistPercentMeleeMaxLimit );
}

// новая функция (007189A0) -------------------------------------------------------
float __fastcall ResistPercentMelee(int playerIndex)
{
	Player& player = Players[playerIndex];
	return RngFromRangePercent(player.resistPercentMeleeMin, player.resistPercentMeleeMax);
}

//----- (th2) -------------------------------------------------------------
void CalcResistPercentArrow(int playerIndex)// #ADR, #arrow damage resistance
{
    Player& player = Players[playerIndex];
	
	int perkFightInTheShade = PerkValue(PERK_FIGHT_IN_THE_SHADE, playerIndex, 0);
	int smallFrameTraitPenalty = HasTrait(playerIndex, TraitId::SmallFrame) ? 10 : 0;
	int crup = HasTrait(playerIndex, TraitId::Crupellarius) ? 10 : 0;
	int durable_syn = PerkValue(SYNERGY_DURABLE, playerIndex); 
	player.resistArrowMin += perkFightInTheShade - smallFrameTraitPenalty;
	player.resistArrowMax += perkFightInTheShade - smallFrameTraitPenalty;
	if( player.fullClassId == PFC_EXECUTIONER && !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) ){
		player.resistArrowMin += 5 + player.CharLevel / 15;
		player.resistArrowMax += 5 + player.CharLevel / 15;
	}
	// mor: these 2 lines are dragon's work, and are not used in the balancing
	player.resistPercentArrowMin = player.scaleResist * player.resistArrowMin;
	player.resistPercentArrowMax = player.scaleResist * player.resistArrowMax;

	int synergyHardenedSteel = PerkValue(SYNERGY_HARDENED_STEEL, playerIndex, 0);
	int perkFightInTheShadeCap = PerkValue(PERK_FIGHT_IN_THE_SHADE, playerIndex, 1);
	switch( player.fullClassId ){
	case PFC_EXECUTIONER: 
		player.resistPercentArrowMinLimit = 0.65f + 0.01f * (synergyHardenedSteel);
		player.resistPercentArrowMaxLimit = 0.75f + 0.01f * (synergyHardenedSteel);
		break;
	default:
		player.resistPercentArrowMinLimit = 0.70f + 0.01f * (-smallFrameTraitPenalty + perkFightInTheShadeCap + synergyHardenedSteel + durable_syn + crup);
		player.resistPercentArrowMaxLimit = 0.80f + 0.01f * (-smallFrameTraitPenalty + perkFightInTheShadeCap + synergyHardenedSteel + durable_syn + crup); // 20%, 30% sf
		break;
	}
	if( 0 ){ // for additional sync resist points and percents
		player.resistArrowMin = int(player.resistPercentArrowMin / player.scaleResist);
		player.resistArrowMax = int(player.resistPercentArrowMax / player.scaleResist);
	}
	LimitToRange(player.resistPercentArrowMin, 0.0f, player.resistPercentArrowMinLimit);
	LimitToRange(player.resistPercentArrowMax, 0.0f, player.resistPercentArrowMaxLimit);
}

//----- (th2) -------------------------------------------------------------
float __fastcall ResistPercentArrow(int playerIndex)
{
	Player& player = Players[playerIndex];
	return RngFromRangePercent(player.resistPercentArrowMin, player.resistPercentArrowMax);
}

//---------------------------------------------------------------------------------
Ai AiFuncs[AI_count] = { // 004B37C0
	{ AI_0_Zombie,                     Ai_0_Zombie },
	{ AI_1_Overlord,                   Ai_1_MeleeRandomSecondAttackSwing },
	{ AI_2_Skel_Melee,                 Ai_2_Skeleton_Melee },
	{ AI_3_Skel_Archer,                Ai_3_ArcherRarelyRetreats },
	{ AI_4_Scavenger,                  Ai_4_Scavenger },
	{ AI_5_Charger,                    Ai_5_ChargeAttack },
	{ AI_6_Goat_Melee,                 Ai_6_Goat_Melee },
	{ AI_7_Goat_Archer,                Ai_7_Goat_Archer },
	{ AI_8_Fallen_One,                 Ai_8_Fallen },
	{ AI_9_Magma,                      Ai_9_Universal_RangeSecondAttack_MagmaBall },
	{ AI_10_Skel_King,                 Ai_10_SkeletonKing },
	{ AI_11_Bat,                       Ai_11_Bat },
	{ AI_12_Gargoyle,                  Ai_12_Gargoyle },
	{ AI_13_Butcher,                   Ai_13_Butcher },
	{ AI_14_Witch,                     Ai_14_RangerRetreatsOften_ArcaneStar },
	{ AI_15_Hidden,                    Ai_15_Hidden },
	{ AI_16_Thunder_Demon,             Ai_16_Universal_RangeSecondAttack_Lightning },
	{ AI_17_Incinerator,               Ai_17_Incinerator },
	{ AI_18_Gharbad,                   Ai_18_Garbud },
	{ AI_19_Acid_Beast,                Ai_19_Universal_RangeSecondAttack_AcidMissile },
	{ AI_20_Acid_Beast_Ranger,         Ai_20_RangerRetreatsOften_AcidMissile },
	{ AI_21_Golem,                     Ai_21_Golem },
	{ AI_22_Zhar_the_Mad,              Ai_22_Zhar },
	{ AI_23_Snotspll,                  Ai_23_Snotspill },
	{ AI_24_Viper,                     Ai_24_Viper },
	{ AI_25_Advocate,                  Ai_25_Mage },
	{ AI_26_Balrog,                    Ai_26_Universal_ShortRangeSecondAttack_Incinerate },
	{ AI_27_Diablo,                    Ai_27_Universal_RangeSecondAttack_Apoc },
	{ AI_28_Lazarus,                   Ai_28_Lazarus },
	{ AI_29_RedVex,                    Ai_29_Lazarus_s_Slut },
	{ AI_30_Lachdanan,                 Ai_30_Lachdanan },
	{ AI_31_Warlord_of_Blood,          Ai_31_Warlord },
	{ AI_32_Ranger_Firebolt,           Ai_32_RangerRetreatsOften_Firebolt },
	{ AI_33_Ranger_Lite_Arrow,         Ai_33_RangerRetreatsOften_LightningArrow },
	{ AI_34_Hork_Demon_spawner_unused, Ai_34_HorkDemon },
	{ AI_35_Lich,                      Ai_35_RangerRetreatsOften_LichOrangeBlast },
	{ AI_36_Arch_Lich,                 Ai_36_RangerRetreatsOften_YelowBlast },
	{ AI_37_Ranger_Fire_Arrow,         Ai_37_RangerRetreatsOften_FireArrow },
	{ AI_38_Beholder,                  Ai_38_RangerRetreatsOften_RedBlast },
	{ AI_39_Mixed_White_Star_phys,     Ai_39_Universal_RangeSecondAttack_Physical_Blue_Blast }, //melee + blue-white physical star
	{ AI_40_Bonewing_cold_blasts,      Ai_40_Universal_RangeSecondAttack_ColdBlast }, // greater skel (melee +circle and habibi shots)
	{ AI_41_Ranger_snowbolt,           Ai_41_RangerRetreatsOften_ColdSnakes }, // shooter of ice bolt ?
	{ AI_42_Mixed_Firebolt,            Ai_42_Universal_RangeSecondAttack_Firebolt }, // shredded
	{ AI_43_Ranger_Acid_Arrow,         Ai_43_RangerRetreatsOften_Acid_Arrow },
	{ AI_44_Ranger_Cold_Arrow,         Ai_44_RangerRetreatsOften_Cold_Arrow },
	{ AI_45_Ranger_Arcane_Arrow,       Ai_45_RangerRetreatsOften_Arcane_Arrow },
	{ AI_46_Ranger_Blue_Star,          Ai_46_RangerRetreatsOften_ArcaneStarBlue }, // shooter of blue arc.star
	{ AI_47_Mixed_Beholder_Star,       Ai_47_Universal_RangeSecondAttack_Bluestar }, // melee + red star
	{ AI_48_Mixed_Yellow_Star,         Ai_48_Universal_RangeSecondAttack_Yellowstar }, // melee + yellow star
	{ AI_49_Ranger_Fireblast,          Ai_49_RangerRetreatsOften_Fireblast },
	{ AI_50_Mixed_Fireblast,           Ai_50_Universal_RangeSecondAttack_Fireblast },
	{ AI_51_Mixed_Arcane_Star,         Ai_51_Universal_RangeSecondAttack_Arcanestar }, // melee + arcStar
	{ AI_52_Mixed_Snowbolt,            Ai_52_Universal_RangeSecondAttack_SnowBolt },
	{ AI_53_Exploder,                  Ai_53_Exploder }, // exploder
	{ AI_54_Mixed_Chargedbolt,         Ai_54_Universal_RangeSecondAttack_Charged_Bolt },
	{ AI_55_Wyrm,                      Ai_55_RangerRetreatsOften_Wyrm }, // wyrm
	{ AI_56_Rammer,                    Ai_56_Rammer },
	{ AI_57_Hybrid_Arc_Star,           Ai_57_Hybrid_ArcaneStar },
	{ AI_58_Mixed_Psychic_Bite,        Ai_58_Universal_RangeSecondAttack_Psychic }, // melee + psychic (distant) hit with no animation (invisible arrow)
	{ AI_59_Tentaclist,                Ai_59_Tentaclist },
};

//---- (th2) ------------------------------------------------------------------
void LoadQuestDungeonMainMapTown(char* dunFileName, int playerRowPos, int playerColPos)
{
	MapStartRow = 0;
	MapStartCol = 0;
	MapEndRow = 112;
	MapEndCol = 112;
	ClearWallsTransparency();
	ushort* dunData = (ushort*)LoadFile(dunFileName);
	
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort* dunArray = &dunData[2];
	TileSquare* townTiles = (TileSquare*)LoadFile("Levels\\TownData\\Town.TIL");
	for( int col = 0; col < colSize; col++ ){
		for( int row = 0; row < rowSize; row++ ){
			ushort wallAndFloorMapValue = dunArray[row + col * rowSize];
			if( wallAndFloorMapValue == 0 ){
				wallAndFloorMapValue = RngFromRange(1, 4);
			}
			SetTileSquare(townTiles, row*2, col*2, wallAndFloorMapValue);
		}
	}
	FreeMem(townTiles);

	PlayerRowPos = playerRowPos;
	PlayerColPos = playerColPos;
    FullLighting = 1;
	ClearAllMaps();

	for( int i = 0; i < 112 * 112; i++ ){
		char& archGraphicsMap = ArchGraphicsMap[ 0 ][ i ];
		switch( FineMap[ 0 ][ i ] ){ // копия блока генерации города
			case 360:			archGraphicsMap = 1;			break;
			case 358:			archGraphicsMap = 2;			break;
			case 129:			archGraphicsMap = 6;			break;
			case 130:			archGraphicsMap = 7;			break;
			case 128:			archGraphicsMap = 8;			break;
			case 117:			archGraphicsMap = 9;			break;
			case 157:			archGraphicsMap = 10;			break;
			case 158:			archGraphicsMap = 11;			break;
			case 156:			archGraphicsMap = 12;			break;
			case 162:			archGraphicsMap = 13;			break;
			case 160:			archGraphicsMap = 14;			break;
			case 214:			archGraphicsMap = 15;			break;
			case 212:			archGraphicsMap = 16;			break;
			case 217:			archGraphicsMap = 17;			break;
			case 216:			archGraphicsMap = 18;			break;
		}
	}
	InitIsometricTileMap();

	AddQuestMonsters((ushort*)dunData, 0, 0);
	SetMapObjects((int)dunData, 0, 0);
	FreeMem(dunData);
}

//----- (00705126) --------------------------------------------------------
int ModifyBarrelExplodeDamage()
{
	int damage = Dungeon->level;
	if( Difficulty >= DL_1_PURGATORY ){
		damage <<= 3;
	}
	if( Difficulty == DL_2_DOOM ){
		damage <<= 2;
	}
	if (Difficulty >= DL_3_CRUCIBLE) {
		damage *= Difficulty - 1;
	}
	return damage;// возврат через edx
}

//----- (00705554) --------------------------------------------------------
int __fastcall ChooseHolyBoltTypeExSpriteIndex( int missileOffset )
{
	Missile* missile = (Missile*)((int)Missiles + missileOffset);
	int spriteIndex;
	int effectIndex = missile->BaseMissileIndex;
	if( effectIndex == MI_53_HOLY_BOLT ){
		spriteIndex = MIS_28_HOLYEXPL;
	}else{
		spriteIndex = MIS_58_EX_BLU3;
	}
	return spriteIndex;// edx
}

//----- (0070556E) --------------------------------------------------------
void __fastcall SetHolyBoltExSpriteHack (int missileIndex, int spriteIndex, int missileOffset)
{
	SetMissileSprite (missileIndex, ChooseHolyBoltTypeExSpriteIndex (missileOffset));
}

//----- (00705548) --------------------------------------------------------
bool __fastcall ActTrash16SpellEffect( int missileOffset )
{
	if( ! IsMainDraw ) return false;
	Missile* missile = (Missile*)((int)Missiles + missileOffset);
	return missile->spriteIndex == ChooseHolyBoltTypeExSpriteIndex (missileOffset);// zf
}

//----- (00718700) --------------------------------------------------------
CompleteSetEffect* __fastcall FindCompleteSetEffect (int name)
{
	for( CompleteSetEffect* completeSetEffect = CompleteSetEffects; completeSetEffect->setName != -1; ++completeSetEffect ){
		if( completeSetEffect->setName == name ){
			return completeSetEffect;
		}
	}
	return 0;
}

//----- (00718720) --------------------------------------------------------
void ClearBufferItemForShowEffect ()
{
	memset(&SetBufferItemForShowEffect, 0, sizeof( SetBufferItemForShowEffect ));
	SetBufferItemForShowEffect.ItemCode = IC_M1_NONE;
}

//----- (00718750) --------------------------------------------------------
void __fastcall ApplyEffectToBufferItemForShowEffect (BaseEffect effect)
{
	Item_ApplyAffix(SetBufferItemForShowEffect, effect, 0, 0, 1, 0, 0, 0, false);
	SetBufferItemForShowEffect.ItemCode = IC_0_OTHER;
}

//----- (007187A0) --------------------------------------------------------
void __fastcall ApplyFullSetEffectToBufferItem (int playerIndex)
{
	ClearBufferItemForShowEffect ();
	for( int setEffectIndex = 0; CompleteSetEffects[setEffectIndex].setName != -1; ++setEffectIndex ){
		if( CountItemFromSet( playerIndex, CompleteSetEffects[setEffectIndex].setName ) == 7 ){
			ApplyEffectToBufferItemForShowEffect( CompleteSetEffects[setEffectIndex].effect );
			return;
		}
	}
}

// th2, вывод информации о меню. Еще некоторое время последить что меню грамотно отрабатывает, потом стереть. 09.03
void DrawMenuInfo()
{
	//Расчеты из 004712B7 чтобы вывести результат на экран. ПОменяв код там, следует поменять и тут.
	int width, height;
	if( IsBigMenuBox ){
		width = Dialog_591_Width;
		height = Dialog_302_Height;
	}else{
		width = Dialog_HalfWidth;
		height = Dialog_302_Height;
	}
	int leftBorderX = ((ScreenWidth >> 1) - (width >> 1));
	int upperBorderY = ((ScreenHeight - height - GUI_MainPanelHeight) >> 1);

	char dest[256];
	sprintf(dest, "leftX = %i, upY = %i, Big = %d, curX = %i, curY = %i  ", leftBorderX, upperBorderY, IsBigMenuBox, CursorX, CursorY); //-V510
	DrawLevelInfoText(8, 33, dest, 3, OnScreenTextOutline);
}

// th2, тут можно выполнять различный отладочный вывод на экран.
void DrawDebugInfo(int& y)
{
	//DrawLevelInfo(); 		
	//DrawMenuInfo();
	
	// сниппет вывода отладочной информации
	char dest[ 256 ];
	//static int i = 0;

	//sprintf( dest, "cursorX = %i, cursorY = %i"/*, DrawnCursorStartY = %i  "*/, CursorX, CursorY/*, DrawnCursorStartY*/ );
	//DrawLevelInfoText( 8, y, dest, 3 ); y += 15;
 
	int x = 8;

	extern int PlayerNextRow, PlayerNextCol;
	sprintf( dest, "PlayerNextRow = %i, PlayerNextCol = %i", PlayerNextRow, PlayerNextCol );
	DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;

	sprintf( dest, "Player row = %i, col = %i", PlayerRowPos, PlayerColPos );
	DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;

	//extern int attackers;
	//sprintf( dest, "attackers = %i", attackers );
	//DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;

	if( IsShowTileNum ){
		sprintf( dest, "FineMap = %d", CurFineMap );
		DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;
	}
	
	#if CHEATS
	extern volatile int ShiftX;
	extern volatile int ShiftY;
	extern volatile int Dir;
	extern volatile int CurDir;
	sprintf( dest, "ShiftX = %i, ShiftY = %i, Dir = %i, CurDir = %i", ShiftX, ShiftY, Dir, CurDir );
	DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;
	//if( IsCtrlPressed ) DrawLevelInfoText( x, y, "Ctrl pressed", 3, OnScreenTextOutline ); y += 15;
	#endif

	sprintf( dest, "Dungeon seed = %i", DungeonRandSeed[(DUNGEON)Dungeon] );
	DrawLevelInfoText( x, y, dest, 3, OnScreenTextOutline ); y += 15;


	//extern int OutDialog;
	//if( OutDialog ){
	//	sprintf( dest, "               Out dialog" );
	//	DrawLevelInfoText( 8, y, dest, 3 ); y += 15;
	//}

	//sprintf( dest, "PlayerMoveInsideTileDir = %i", PlayerMoveInsideTileDir );
	//DrawLevelInfoText( 8, y, dest, 3 ); y += 15;

	//static int nmdc[ 32 ]{};
	//nmdc[i] = NotMainDrawCount;
	//for( int i = 0; i < 32; ++i ){
	//	sprintf( dest, "nmdc = %i", nmdc[ i ] );
	//	DrawLevelInfoText( 8, y, dest, 3 ); y += 15;
	//}
	
	//static int movedX[ 32 ]{};
	//static int movedY[ 32 ]{};
	//movedX[ i ] = PlayerMovedX;
	//movedY[ i ] = PlayerMovedY;
	//for( int i = 0; i < 32; ++i ){
	//	sprintf( dest, "offset X = %i offset Y = %i", movedX[ i ], movedY[ i ] );
	//	DrawLevelInfoText( 8, y, dest, 3 ); y += 15;
	//}
	
	//i = (i + 1) & 31;
	// ...
}

//----(th2)--------------------------------------------------------
bool ActionClick()
{
	return !( CurrentMenuWindowPtr
	 || IsPlayerDead
	 || GamePaused == 2
	 || IsPopupScreenOpen
	 || SelectCurSpellMode
	 || CurrentDialogIndex
	 || TalkPanelMode
	 || IsGoldSplitPanelVisible
	 || Cur.GraphicsID >= CM_2_IDENTIFY && Cur.GraphicsID <= CM_10_HEAL_OTHER 
	 || Cur.GraphicsID >= CM_12_ITEMS_PICS_START
	 || Speech_IsPanelVisible );
}

bool PanelClick = false;
bool EnteredPanelWithAutoClick = false;

//----(th2)--------------------------------------------------------
void CheckPanelClick()
{
	bool prevPanelClick = PanelClick;
	PanelClick = CursorInMainPanel()
	 || IsQUESTPanelVisible       && CursorIntoDisplayObject(QuestPanelRect)
	 || IsCHARPanelVisible        && CursorIntoDisplayObject(CharPanelRect)
	 || IsINVPanelVisible         && CursorIntoDisplayObject(InventoryPanelRect)
	 || IsSpellBookVisible        && CursorIntoDisplayObject(SpellBookRect)
	 || IsStashPanelVisible       && CursorIntoDisplayObject(VisualStashPanel)
	 || IsVisualTradePanelVisible && CursorIntoDisplayObject(VisualTradePanel)
	 || IsCraftPanelVisible       && CursorIntoDisplayObject(CraftPanel)
	 || IsEnchantPanelVisible     && CursorIntoDisplayObject(EnchantPanel)
	 || IsQuenchPanelVisible      && CursorIntoDisplayObject(QuenchPanel)
	 || IsTransmutePanelVisible   && CursorIntoDisplayObject(TransmutePanel)
	 || IsPerksPanelVisible       && CursorIntoDisplayObject(PerksPanelPos)
	 || IsInfoWindowVisible       && CursorIntoDisplayObject(InfoWindowRect)
	 || IsPerksPanelVisible       && getPlayerPerksPointsLeft(PERK_TYPE_COMMON) > 0 && !SelectCurSpellMode && CursorIntoPerkUpButton()
	 || Players[ CurrentPlayerIndex ].AvailableLvlPoints && !SelectCurSpellMode && CursorIntoLevelUpButton();
	if( prevPanelClick != PanelClick && PanelClick && IsShiftPressed && !ShiftReleasedEvent ){
		EnteredPanelWithAutoClick = true;
	}
	if( !PanelClick || EnteredPanelWithAutoClick && (!IsShiftPressed || ShiftPressedEvent || ShiftReleasedEvent) ){
		EnteredPanelWithAutoClick = false;
	}
}

//----(00719220)--------------------------------------------------------
void Repeat_Mouse_LButtonDown()
{
	if( MouseButtonPressed == MBP_1_LEFT ){
		if( ! RButtonEvent && (EasyClick || IsShiftPressed) ){ // разобраться с этими условиями ниже, возможно вытащить !EasyClick выше
			// Mouse_LButtonDown обрабатывает клики в интерфейсе даже при "спец"проверке, пробую исправить
			if( !IsGameMenuActive() && (/*!ShiftPressedEvent && !ShiftReleasedEvent ||*/ !PanelClick || EnteredPanelWithAutoClick) ){ // проверка что не интерфейс и не дроп
				int isGameObject = IsoLand_CheckLButtonClick( ShiftPressedEvent ? ShiftPressedEvent : IsShiftPressed, EasyClick ? (ShiftPressedEvent ? 1 : !ShiftReleasedEvent) : 0 );
				if( ShiftReleasedEvent /*&& ! ShiftPressedEvent*/ ){
                    TrackInit( true /*isGameObject*/ );
					ShiftReleasedEvent = false;
				}
				//else{
				//	track_repeat_walk( isGameObject );
				//}
			}
		}
	}
}

//----(0071925D)--------------------------------------------------------
void Repeat_Mouse_RButtonDown()
{
	// TODO: may be change one-by-one spell comparing to spell mask comparing
	if( MouseButtonPressed == MBP_2_RIGHT ){
		int spell = Players[CurrentPlayerIndex].spellIndex;
		if( (EasyClick	
		 && not(spell,PS_2_HEALING
		             ,PS_5_IDENTIFY
		             ,PS_6_FIRE_WALL
		             ,PS_7_TOWN_PORTAL
		             ,PS_9_INFRAVISION
		             ,PS_10_PHASING
		             ,PS_11_MANA_SHIELD
		             ,PS_16_REFLECT
		             ,PS_17_LIGHTING_RING
		             ,PS_19_FLAME_RING
		             ,PS_21_GOLEM
		             ,PS_22_FURY
		             ,PS_23_TELEPORT
		             ,PS_25_ETHEREAL
		             ,PS_26_ITEM_REPAIR
		             ,PS_27_STAFF_RECHARGE
		             ,PS_28_TRAP_DISARM
		             ,PS_33_TELEKINES
		             ,PS_34_HEAL_OTHER
		             ,PS_40_LIGHTING_WALL
		             ,PS_42_WARP
		             ,PS_45_RING_OF_FIRE
		             ,PS_48_RUNE_OF_LIGHT)
		 && (spell != PS_13_HYDRA || HasTrait(CurrentPlayerIndex, TraitId::Hydramancer)))
		 || IsShiftPressed ){
			Mouse_RButtonDown( EasyClick );
		}
	}
}

// ---- (007146B0) -------------------------------------
void __fastcall OnMouseWheel (uint wParam)
{
	short wheelDelta = (uint)wParam >> 16;
	if( wheelDelta > 0 ){
		if( CurrentDialogIndex ){
			DialogUp();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftUp();
        }else if( IsPerksPanelVisible ){
            PerkPanel_Shift(-1);
		}else if (IsInfoWindowVisible) {
			InfoWindow_Prev();
		}else if( IsHELPVisible ){
			ShiftHelpUp();
		}else if( IsStashPanelVisible ){
			StashPanel_PrevTab();
		}else if( IsTransmutePanelVisible ){
			if( TransmuteShowRecipeList && ! TransmuteSlots.count ){
				TransmuteRecipeListUp();
			}
		}
	}else if( wheelDelta < 0 ){
		if( CurrentDialogIndex ){
			DialogDown();
		}else if( IsQUESTPanelVisible ){
			QuestPanel_ShiftDown();
        }else if( IsPerksPanelVisible ){
            PerkPanel_Shift(1);
		}else if (IsInfoWindowVisible) {
			InfoWindow_Next();
		}else if( IsHELPVisible ){
			ShiftHelpDown();
		}else if( IsStashPanelVisible ){
			StashPanel_NextTab();
		}else if( IsTransmutePanelVisible ){
			if( TransmuteShowRecipeList && ! TransmuteSlots.count ){
				TransmuteRecipeListDown();
			}
		}
	}
	if (SwapSpellsOnScroll) {
		HandleSwapSpellOnScroll(wheelDelta);
	}
}

//----- (th2) -------------------------------------------------------------
void HandleSwapSpellOnScroll(int wheelDelta)
{
	if (SwapSpellsOnScroll && !CurrentDialogIndex && !IsQUESTPanelVisible && !IsPerksPanelVisible && !IsHELPVisible && !IsStashPanelVisible && !IsInfoWindowVisible) {
		Player& player = Players[CurrentPlayerIndex];
		bool newSpell = true;
		int lifeCycle;

		if( MoreManualHotkeys ){
			static int hotkeyAmound = ExpandedSpellsOnScroll ? 40 : 8;
			int bindNum = 0;

			while( bindNum < hotkeyAmound ){
				if( player.CurrentSpellIndex == SpellHotkeys.hotkey[bindNum] && player.SpellType == SpellHotkeys.type[bindNum] ){
					newSpell = false;
					break;
				}
				++bindNum;
			}
			if( newSpell ) bindNum = 0;

			int move = wheelDelta > 0 ? 1 : -1;
			int i = bindNum;
			do{
				i += move;
				if( i < 0 ) i = hotkeyAmound - 1;
				if( i > hotkeyAmound - 1 ) i = 0;
				if( SpellHotkeys.hotkey[i] > 0 ){
					if( SelectSpellByHotKeyExpanded(i) ) break;
				}
			}while( i != bindNum );
		}else{
			for (int bindIndex = 0; bindIndex <= 3; ++bindIndex){
				if( player.CurrentSpellIndex == player.SpellOnFuncKeys[bindIndex] ){
					for( wheelDelta > 0 ? lifeCycle = 3 : lifeCycle = 1; wheelDelta > 0 ? lifeCycle > 0 : lifeCycle <= 3; wheelDelta > 0 ? --lifeCycle : ++lifeCycle ){
						SelectSpellByHotKey((lifeCycle + bindIndex) % 4);
					}
					newSpell = false;
					break;
				}
			}
			if( newSpell ){
				for( wheelDelta > 0 ? lifeCycle = 3 : lifeCycle = 0; wheelDelta > 0 ? lifeCycle >= 0 : lifeCycle <= 3; wheelDelta > 0 ? --lifeCycle : ++lifeCycle ){
					SelectSpellByHotKey((lifeCycle) % 4);
				}
			}

		}
	}
}

int & SpeedrunTimer = AdditionalStateData.SpeedrunTimer;
SPEEDRUN_STEP & CurrentSpeedrunStep = AdditionalStateData.CurrentSpeedrunStep;
std::array<int, SRS_COUNT> & SpeedrunTimePoints = AdditionalStateData.SpeedrunTimePoints;

//----- (th2) -------------------------------------------------------------
void ResetSpeedrunTimer()
{
    SpeedrunTimer = 0;
    CurrentSpeedrunStep = SRS_TOWN;
    std::fill( std::begin( SpeedrunTimePoints ), std::end( SpeedrunTimePoints ), 0 );
}

//----- (th2) -------------------------------------------------------------
void AdvanceSpeedrunTimer()
{
    if( CurrentSpeedrunStep < SRS_COUNT ){
        SpeedrunTimePoints[CurrentSpeedrunStep] = SpeedrunTimer / 20;
        CurrentSpeedrunStep = SPEEDRUN_STEP( int(CurrentSpeedrunStep) + 1 );
    }
}

//----- (th2) -------------------------------------------------------------
const char* GetSpeedrunStepName( const SPEEDRUN_STEP step )
{
    switch( step )
    {
        default:             return "";
        case SRS_TOWN:       return "town";
        case SRS_CATHEDRAL:  return "cathedral";
        case SRS_CATACOMBS:  return "catacombs";
        case SRS_CAVES:      return "caves";
        case SRS_HELL:       return "hell";
        case SRS_ABYSS:      return "abyss";
        case SRS_CRYPT:      return "crypt";
    }
}

//----- (th2) -------------------------------------------------------------
int GetSpeedrunStepValue( const SPEEDRUN_STEP step )
{
    if( step < CurrentSpeedrunStep ){
        return SpeedrunTimePoints[step];
    }else if( step == CurrentSpeedrunStep ){
        return SpeedrunTimer / 20;
    }else{
        return 0;
    }
}

//----- (th2) -------------------------------------------------------------
const char* GetTimeValueFormated( const int secondsTotal )
{
    static char BUFFER[256];
    
    const int seconds = secondsTotal % 60;
    const int minutesTotal = secondsTotal / 60;
    const int minutes = minutesTotal % 60;
    const int hours = minutesTotal / 60;
    sprintf( BUFFER, "%i:%02i:%02i", hours, minutes, seconds );
    
    return BUFFER;
}
