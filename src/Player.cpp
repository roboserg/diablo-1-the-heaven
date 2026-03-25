#include "stdafx.h"
#include "CXBOXController.h"

vector<CombatText> CombatTextQueue;
int PlayerLastDrawnMana = -1;
int PlayerLastDrawnHealth = -1;
vector<pair<string, string> > messageLog;
bool InstantMurderHero = false;
char MurderHeroName[32] = ""; 
int ManaRegen = 0;
int HealthRegen = 0;
int UpdateAuraDamage = 0;
int AcidedMonsterFrameColor=203;
int AcidedHealthBarColor=203;

void operator += ( PlayerStat& stat, int value )
{
    stat.Strength  += value;
    stat.Magic     += value;
    stat.Dexterity += value;
    stat.Vitality  += value;
}

void operator -= ( PlayerStat& stat, int value )
{
    stat.Strength  -= value;
    stat.Magic     -= value;
    stat.Dexterity -= value;
    stat.Vitality  -= value;
}

//----- (th2) ------------------------------------------------------------
PlayerStat GetStartStat( PLAYER_FULL_CLASS fullClassId, decltype(Player::traits) traits, int playerIndex )
{
    PlayerStat result = FullPlayerClasses[fullClassId].StartStat;
	const Player& player = Players[playerIndex];

	if( has(traits, TraitId::Paladin   ) ){ result.Strength	  = 20; result.Magic = 15; result.Dexterity = 10; result.Vitality = 20; }
	if( has(traits, TraitId::Mamluk    ) ){ result.Strength	  = 20; result.Magic =  5; result.Dexterity = 20; result.Vitality = 20; }
	if( has(traits, TraitId::Pistoleer ) ){ result.Strength	  = 20; result.Magic =  0; result.Dexterity = 20; result.Vitality = 25; }
	if( has(traits, TraitId::Gifted    ) ){ result           +=  5; } // +5 to starting stats
	if( has(traits, TraitId::BlueBlood ) ){ result			 += 10; } // +10 to starting stats 
	if( has(traits, TraitId::Rudiarius ) ){ result.Vitality  -=  5; } // start game with -5 to base Vitality
	if( has(traits, TraitId::LitheBuild) ){ result.Dexterity +=  5; } // start game with +5 to base Dexterity
	if (player.gameChanger & BIT(GC_23_TOUGH_LUCK)) {
		result.Strength = 5;
		result.Magic = 5;
		result.Dexterity = 5;
		result.Vitality = 5;
	}
	if (GameMode == GM_CLASSIC) {
		switch (player.ClassID) {
		case PC_0_WARRIOR:		result.Strength = 30; result.Magic = 10; result.Dexterity = 20; result.Vitality = 25;	break;
		case PC_1_ARCHER:		result.Strength = 20; result.Magic = 15; result.Dexterity = 30; result.Vitality = 20;	break;
		case PC_2_MAGE:			result.Strength = 15; result.Magic = 35; result.Dexterity = 15; result.Vitality = 20;	break;
		}
	}
    return result;
};

//----- (th2) ------------------------------------------------------------
PlayerStat GetPlayerMaxStat( int playerIndex )
{
    const Player& player = Players[playerIndex];
	PlayerStat result = FullPlayerClasses[player.fullClassId].MaxStat;

	if( has(player.traits, TraitId::Mamluk        )){ result.Strength  =  475; result.Magic  = 150; result.Dexterity  = 475; result.Vitality  = 400; }
	if( has(player.traits, TraitId::Pistoleer     )){ result.Strength  =  600; result.Magic  =  50; result.Dexterity  = 400; result.Vitality  = 450; }
	if( has(player.traits, TraitId::Black_Witchery)){ result.Strength  =  300; result.Magic  = 400; result.Dexterity  = 500; result.Vitality  = 300; }
	if( has(player.traits, TraitId::BendTheRules  )){ result.Strength  =  900; result.Magic  = 900; result.Dexterity  = 900; result.Vitality  = 900; } // all max stats = 900
	if( has(player.traits, TraitId::Gifted        )){ result += 25; } // +25 to maximum stats
	if( has(player.traits, TraitId::OldFashioned  )){                                               result.Dexterity +=  40; } // +(CLVL*3) to AC, +(CLVL*2) to damage, +40 to maximum base Dexterity
	if( has(player.traits, TraitId::LitheBuild    )){                                               result.Dexterity += 100; }
	if( has(player.traits, TraitId::Bruiser       )){ result.Strength += 150;                       result.Dexterity -= 150; } // +150 maximum base Strength, -150 maximum base Dexterity
	if( has(player.traits, TraitId::Negotiant     )){                                                                        result.Vitality -=  50; } // -50 maximum base Vitality
	if( has(player.traits, TraitId::Paladin       )){                          result.Magic += 200 + PerkValue(SYNERGY_THAUMATURGY, playerIndex);  } // +70 maximum base Magic + value from Thaumaturgy synergy
	if( has(player.traits, TraitId::Leper         )){                          result.Magic +=  75; result.Dexterity +=  75; } // +75 maximum base Dexterity, +75 maximum base Magic
	if( has(player.traits, TraitId::Ranger        )){                          result.Magic +=  40;                          result.Vitality -=  40; }
	if (has(player.traits, TraitId::Crupellarius  )){ result.Strength -= 100;												 result.Vitality += 100; }
	if (has(player.traits, TraitId::Giant))			{																		 result.Vitality += 100; }
	if( player.gameChanger& BIT(GC_9_NIGHT_KIN)    ){                          result.Magic += 225; result.Dexterity += 125; result.Strength -= 200; }
	if( PerkValue(SYNERGY_OSTENTATION, playerIndex) > 0){ result += PerkValue(SYNERGY_OSTENTATION, playerIndex); }

	if (GameMode == GM_CLASSIC) {
		switch (player.ClassID) {
		case PC_0_WARRIOR:		result.Strength = 250; result.Magic =  50; result.Dexterity =  60; result.Vitality = 100;	break;
		case PC_1_ARCHER:		result.Strength =  55; result.Magic =  70; result.Dexterity = 250; result.Vitality =  80;	break;
		case PC_2_MAGE:			result.Strength =  45; result.Magic = 250; result.Dexterity =  85; result.Vitality =  80;	break;
		}
	}
	return result;
};

//----- (th2) ------------------------------------------------------------
void __fastcall Font_DrawChar2(int aXpos, int aYpos, char* aFontPtr, unsigned char aCharIndex, int aFontSize)
{
	int Ypos;               // ebx@1
	char* PictPtr;			// edx@3
	uchar* DstPosPtr;       // ecx@3
	int PictSize;           // ST00_4@3
	int CharOffset;         // esi@3
	int Xpos;               // [sp+Ch] [bp-4h]@1
	unsigned* FontTable = (unsigned*)aFontPtr;
	Ypos = aYpos;
	Xpos = aXpos;
	if (WorkingSurface) {
		if (aFontPtr) {
			CharOffset = FontTable[aCharIndex];
			PictPtr = &aFontPtr[CharOffset];
			PictSize = FontTable[aCharIndex + 1] - CharOffset;
			DstPosPtr = &WorkingSurface[Xpos + YOffsetHashTable[Ypos]];
			//if( DarkLevel )
			//CopyPCX_ConvColor(DstPosPtr, PictPtr, PictSize, aFontSize);
			//else
			RleDraw(DstPosPtr, PictPtr, PictSize, aFontSize);
		}
	}
}

//----- (th2) ------------------------------------------------------------
void GetIronmanSaverate(int& ironman_saverate)
{
	if (Difficulty == DL_3_CRUCIBLE) ironman_saverate = 2;
	else if (Difficulty >= DL_4_TORMENT) ironman_saverate = 1;
	else ironman_saverate = 4;
}

//----- (th2) ------------------------------------------------------------
void IronmanLevelCleared()
{
	PlayLocalSound(S_1178_IRONMAN_LEVEL_CLEARED, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col);
	int ironman_saverate = 4;
	GetIronmanSaverate(ironman_saverate);
	if( (DUNGEON)Dungeon % ironman_saverate == 0 && !Dungeon->isQuest ){ // TODO: add can save flag to Dungeons[]
		AddOnScreenMessage(OM_72_Level_Cleared_Can_Save);
	}
}

//----- (th2) ------------------------------------------------------------
void ActAura( int playerIndex, int radius, int damage, ELEMENTAL_TYPE element, int monsterClassMask = MON_ANY )
{
	Player& player = Players[playerIndex];
	for( int row = player.Row - radius, rowE = player.Row + radius; row <= rowE; ++row ){
		for( int col = player.Col - radius, colE = player.Col + radius; col <= colE; ++col ){
			if( In112(row, col) && FlagMap[row][col] & CF_2_VISIBLE_BY_PLAYER ){ // in light radius
				int monsterNumber = MonsterMap[row][col];
				MonsterHorizontalHitFix( monsterNumber, row, col );
				if( monsterNumber > SummonMonsters_Count ){ // is enemy
					int monsterIndex = monsterNumber - 1;
					Monster& monster = Monsters[monsterIndex];
					if( B(monster.BasePtr->MonsterClass) & monsterClassMask && !IsMonsterImmuneToMissile(monsterIndex, element, playerIndex) ){
						if( ShowPlayerAuraHitVisual ){
							CastMissile(row, col, element, monsterIndex, 0, MI_142_AURA_HIT_GFX, CT_0_PLAYER, playerIndex, 0, 0, 0); // Aura visuals
						}
						monster.flag &= ~MF_3_STONE;
						if( playerIndex == CurrentPlayerIndex ){
							monster.CurrentLife -= damage;
						}
						if( (monster.CurrentLife & ~63) > 0 ){
							DamageMonsterByPlayer( monsterIndex, playerIndex, damage );
						}else{
							DrawFloatingDamage( damage >> 6, monster.Row, monster.Col, monsterIndex );
							KillMonsterByPlayer( monsterIndex, playerIndex );
						}
					}
				}
			}
		}
	}
}

//----- (th2) ------------------------------------------------------------
__forceinline void TurnUndeadAura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_5_HOLY, B(MON_0_UNDEAD));
}

//----- (th2) ------------------------------------------------------------
__forceinline void PsionicAura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_0_PHYSICAL);
}

//----- (th2) ------------------------------------------------------------
__forceinline void Searing_Aura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_1_FIRE);
}

//----- (th2) ------------------------------------------------------------
__forceinline void Static_Aura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_2_LIGHTNING);
}

//----- (th2) ------------------------------------------------------------
__forceinline void Theurgic_Aura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_3_ARCAN);
}

//----- (th2) ------------------------------------------------------------
__forceinline void Toxic_Aura(int playerIndex, int radius, int damage)
{
	ActAura(playerIndex, radius, damage, ET_4_ACID);
}

//----- (th2) ------------------------------------------------------------
int MaxAcidDPS( int monsterIndex, int playerIndex )
{
	Monster& monster = Monsters[monsterIndex];
	Player& player = Players[playerIndex];
	int divider = 1;
	if( monster.newBossId > 0 ){		
		switch (monster.newBossId - 1) {
		case UM_773_The_Dark_Lord:
		case UM_12_Uber_Diablo:            divider = 70; break; // 24
		case UM_11_The_Defiler:            divider = 58; break; // 22
		case UM_582_Izual:	               divider = 56; break; // 22
		case UM_154_Diablo:                divider = 55; break; // 
		case UM_580_NaKrul
			or UM_581_Grimspike:           divider = 54; break;
		case UM_323_Lord_of_the_Order:     divider = 53; break;
		case UM_587_Hephasto_the_Armorer:  divider = 50; break; // 22
		case UM_702_Grimjaw or UM_703_Hive_Mother or UM_705_Bloodhoof_Twinaxe or UM_708_Grimwing or UM_723_The_Infester
			or UM_713_Sir_Valon or UM_714_Sir_Darion or UM_716_Bishop_Zamal or UM_717_Priestess_Varna or UM_718_Guardian_of_Flames:
										   divider = 48; break;
		case UM_595_The_Cerberus:		   divider = 46; break; // 18
		case UM_586_Wielder_of_Shadowfang: divider = 51; break;
		case UM_585_Fleshdoom
			or UM_584_Mordessa:            divider = 46; break;
		case UM_579_Andariel
			or UM_578_Uber_Xorene
			or UM_0_Gharbad_the_Weak:      divider = 44; break;
		case UM_1_Skeleton_King:           divider = 37; break;
		case UM_9_The_Butcher:             divider = 32; break;
		case UM_4_ArchBishop_Lazarus:      divider = 44; break;
		case UM_8_Warlord_of_Blood
			or UM_157_Warlord_of_Blood:    divider = 52; break;
		case UM_261_Heartburn:             divider = 41; break;
		default:
			switch (Dungeon->level) {
			case              0 or 1: divider = 14; break;
			case                   2: divider = 19; break; // 6
			case                   3: divider = 22; break; // 7
			case                   4: divider = 25; break; // 9
			case              5 or 6: divider = 30; break; // 12-15
			case              7 or 8: divider = 34; break; // ??
			case 9 or 10 or 11 or 12: divider = 37; break;
			default                 : divider = 40; break;
			}
			break;
		}
		if (Difficulty > 1) {
			divider += 13; // Doom: + 13 sec
		}
		else if (Difficulty > 0) {
			divider += 7; // Purg: + 7 sec
		} // it will be possible to add some sort of trait check here, that allows burning through bosses and/or monsters faster
		divider -= player.CharLevel / 6; // << perhaps, needs adjustment in future (was:  clvl/2)
		if (HasTrait(playerIndex, TraitId::Toxic_at_Heart)) {
			divider -= divider * 40 / 100;
		}
		divider -= (divider * (PerkValue(PERK_ACID_BURN, playerIndex)) / 100);
		LimitToMin(divider, 3 * Difficulty / 2 + 5); // minimum time to kill any boss = 5 sec + 150% Difficulty
	}else{
		switch(Difficulty){
		case DL_0_HORROR:
			switch (Dungeon->level) {
			case               0 or 1: divider =  5; break; //  1- 4
			case                    2: divider =  6; break; //  3- 7
			case                    3: divider =  7; break; //  6-10
			case                    4: divider =  8; break; //  8-12
			case               5 or 6: divider =  9; break; // 10-15
			case               7 or 8: divider = 10; break; // 14-17
			case              9 or 10: divider = 11; break; // 16-19
			case             11 or 12: divider = 12; break; // 18-21
			case       13 or 14 or 15: divider = 13; break; // 20-22
			case 16 or 17 or 18 or 19: divider = 14; break; // 21-23
			default                  : divider = 15; break; // 22+
			}
			break;
		case DL_1_PURGATORY:		   divider = 16; break;// 23+
		default: // DL_2_DOOM+
			switch (Dungeon->level) {
			case                        0 or 1 or 2 or 3 or 4: divider = 17; break; // 32+
			case      5 or 6 or 7 or 8 or 9 or 10 or 11 or 12: divider = 18; break; // 35+
			case 13 or 14 or 15 or 16 or 17 or 18 or 19 or 20: divider = 19; break; // 37+
			default                                          : divider = 20; break; // 38+
			}
			break;
		}
		divider -= player.CharLevel / 15; // << same, (was: 5), but could require fine-tuning later on
		if (HasTrait(playerIndex, TraitId::Toxic_at_Heart)) {
			divider -= divider * 40 / 100;
		}
		divider -= PerkValue(PERK_ACID_BURN, playerIndex) / 10;
		divider -= (divider * (PerkValue(PERK_ACID_BURN, playerIndex)) / 100);
		LimitToMin(divider, 1 + Difficulty / 2);
	}
	return monster.BaseLife / divider;
}


void ModifyAuraDamageForCoop(int& damage)
{
	if( MaxCountOfPlayersInGame != 1 ){
		switch( NetPlayerCount ){
		case 2: damage -= damage / 4;			break; // -25% damage // 150%
		case 3: damage -= 2 * damage / 5;		break; // -40% damage // 180%
		case 4: damage -= damage / 2;			break; // -50% damage // 200%
		}
		LimitToMin(damage, 0);
	}
}


//----- (th2) ------------------------------------------------------------
void DamageMonstersByAuras()
{
	UpdateAuraDamage++;
	if (UpdateAuraDamage >= 20) { // aura damages each 20 frame (1 per second)
		UpdateAuraDamage = 0;
		Player& player = Players[CurrentPlayerIndex];
		i64 xpGain = PerkValue(PERK_SERENITY, CurrentPlayerIndex) * getPassiveXpFormula();
		if (HasTrait(CurrentPlayerIndex, TraitId::Forgetful)) {
			uint forgetful_trait_penalty = 1 + ((player.CharLevel * player.CharLevel) / 10);
			if (forgetful_trait_penalty >= player.Xp) { // XP unsigned patch (1)
				player.Xp = 0;
			}
			else {
				player.Xp -= forgetful_trait_penalty;
			}
		}
		if (xpGain > 0) {
			addUnlimitedXpToPlayer(xpGain, CurrentPlayerIndex);
		}
		if( Dungeon != DUN_0_TOWN && (player.MaxCurLife & ~63) > 0 && player.CurLife > 0 ){
			if( HasTrait(CurrentPlayerIndex, TraitId::HolyAura) ){
				int damage = (1 + player.CharLevel * player.CurMagic / 40) << 6;
				damage += (PerkValue(PERK_RIGHTEOUS_ANGER, CurrentPlayerIndex)) << 6;
				ModifyAuraDamageForCoop(damage);
				TurnUndeadAura(CurrentPlayerIndex, 10, damage);
			}
			if( HasTrait(CurrentPlayerIndex, TraitId::Psion) ){
				int damage = (3 + player.CharLevel * player.CurMagic / 150) << 6;
				damage += (PerkValue(PERK_PSYCHOKINESIS, CurrentPlayerIndex)) << 6;
				if( player.CurAction == PCA_0_STAND ){
					int amplifier = (PerkValue(PERK_AMPLIFY_DAMAGE, CurrentPlayerIndex));
					damage = ((amplifier + 100) * damage) / 100;
				}
				ModifyAuraDamageForCoop(damage);
				PsionicAura(CurrentPlayerIndex, 10, damage);
			}
			if( HasTrait(CurrentPlayerIndex, TraitId::Devastator) ){
				int rad = 3 + (PerkValue(PERK_AURA_RADIUS, CurrentPlayerIndex) / 10) + (PerkValue(SYNERGY_BASTION_OF_ELEMENTS, CurrentPlayerIndex) / 10);
				int aura_mastery = PerkValue(SYNERGY_AURA_MASTERY, CurrentPlayerIndex);
				if (PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex) > 0) {
					int damage = PerkValue(PERK_TOXIC_AURA, CurrentPlayerIndex) << 6;
					damage += aura_mastery * damage / 100;
					ModifyAuraDamageForCoop(damage);
					Toxic_Aura(CurrentPlayerIndex, rad, damage);
				}
				if( PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex) > 0 ){
					int damage = PerkValue(PERK_SEARING_AURA, CurrentPlayerIndex) << 6;
					damage += aura_mastery * damage / 100;
					ModifyAuraDamageForCoop(damage);
					Searing_Aura(CurrentPlayerIndex, rad, damage);
				}
				if( PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) > 0 ){
					int damage = PerkValue(PERK_THEURGIC_AURA, CurrentPlayerIndex) << 6;
					damage += aura_mastery * damage / 100;
					ModifyAuraDamageForCoop(damage);
					Theurgic_Aura(CurrentPlayerIndex, rad, damage);
				}
				if (PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex) > 0) {
					int damage = PerkValue(PERK_STATIC_AURA, CurrentPlayerIndex) << 6;
					damage += aura_mastery * damage / 100;
					ModifyAuraDamageForCoop(damage);
					Static_Aura(CurrentPlayerIndex, rad, damage);
				}			
			}
			//propably also a good place to set the monsters auras, auras will be stored as an monster affix
			// for	(all alive monsters on level )
			// if (monster has aura)
			// switch (monster.affixwithaura)
			// case aura: execute aura, some copy of Undead Aura but in opposite
		}
	}
}

//----- (th2) ------------------------------------------------------------
bool IsIronmanObject(Object& obj) // this excludes specific objects from ironman mode counter for activated objects on current dlvl
{
	return (obj.selectable && !obj.isDoor &&
		obj.BaseObjectIndex != BO_23_MAGIC_ROCK_TABLE &&
		obj.BaseObjectIndex != BO_24_ANGEL &&
		obj.BaseObjectIndex != BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS &&
		obj.BaseObjectIndex != BO_41_MYTHICAL_BOOK &&
		obj.BaseObjectIndex != BO_52_BOOK1 &&
		obj.BaseObjectIndex != BO_61_ANCIENT_TOME &&
		obj.BaseObjectIndex != BO_64_LIBRARY_BOOK &&
		obj.BaseObjectIndex != BO_66_BLOOD_FOUNTAIN &&
		obj.BaseObjectIndex != BO_71_BOOK_OF_THE_BLIND &&
		obj.BaseObjectIndex != BO_72_BOOK_OF_BLOOD &&
		obj.BaseObjectIndex != BO_76_PURIFYING_SPRING &&
		obj.BaseObjectIndex != BO_81_MURKY_POOL &&
		obj.BaseObjectIndex != BO_82_FOUNTAIN_OF_TEARS &&
		obj.BaseObjectIndex != BO_83_ALTBOY &&
		obj.BaseObjectIndex != BO_84_MCIRL &&
		obj.BaseObjectIndex != BO_85_MCIRL &&
		obj.BaseObjectIndex != BO_86_QUEST_BOOK &&
		obj.BaseObjectIndex != BO_88_STEEL_TOME &&
		obj.BaseObjectIndex != BO_94_MUSHROOM_PATCH &&
		obj.BaseObjectIndex != BO_95_VILE_STAND &&
		obj.BaseObjectIndex != BO_97_OGDEN_CHEST &&
		obj.BaseObjectIndex != BO_98_HELLFORGE);
}

//----- (th2) ------------------------------------------------------------
int GetNumberOfObjectsInDungeon() {
	int activeObjects = 0;
	if( Dungeon == DUN_0_TOWN ){
		return 0;
	}
	for (int objIndex = 0; objIndex < ObjectsCount; objIndex++) {
		Object& obj = Objects[ObjectActive[objIndex]];
		if (IsIronmanObject(obj)) {
			++activeObjects;
		}
	}
	return activeObjects;
}

//----- (th2) ------------------------------------------------------------
int GetNumberOfMonstersInDungeon() {
	int monstersInDungeon = 0;
	if( Dungeon == DUN_0_TOWN ){
		return 0;
	}
	for (int monsterIndexIndex = SummonMonsters_Count; monsterIndexIndex < MonstersCount; monsterIndexIndex++) {
		const Monster& monster = Monsters[MonsterIndexes[monsterIndexIndex]];
		if (!monster.speechIndex && ! (monster.flag & MF_15_KILLED) && monster.Xp > 0 && (monster.Col > 1 || monster.Row > 1)) {
			++monstersInDungeon;
		}
	}
	return monstersInDungeon;
}

//----- (th2) ------------------------------------------------------------
void ShowMessageLog(int firstItem) {
	SelectedDialogRowInPrevDialog = 0;
	if (SelectedDialogRow == PT_DialogRows_BigDialog_Back) {
		CurrentDialogIndex = PD_0_None;
		return;
	}

	Player& player = Players[CurrentPlayerIndex];
	IsBigMenuBox = true;
	DialogHasScroolBox = true;
	FirstVisibleItem = firstItem;
	sprintf(InfoPanelBuffer, "Message log:");
	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Logo, true, InfoPanelBuffer, C_3_Gold, false);
	SetDialogLine(PT_DialogRows_BigDialog_TopSeparator);
	SetDialogLine(PT_DialogRows_BigDialog_BottomSeparator);

	int itermNum = firstItem;
	ClearRowsInInterval(PT_DialogRows_BigDialog_ContentStart, PT_DialogRows_BigDialog_ContentEnd);
	TopSelectedRow = PT_DialogRows_BigDialog_ContentStart;
	for (int dialogRow = PT_DialogRows_BigDialog_ContentStart; dialogRow < 20 && itermNum < DialogRowsCount; dialogRow += TradeRows) {
		if (messageLog.size() == 0) {
			SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, "The message log is empty.", C_3_Gold, true);
			BottomSelectedRow = dialogRow;
			break;
		}
		else {
			if (itermNum < (int)messageLog.size()) {
				int it = messageLog.size() - itermNum - 1;
				char color = C_0_White;
				if (messageLog[it].first == player.playerName) {
					color = C_1_Blue;
				}
				stringstream ss;
				ss << messageLog[it].first << ": " << messageLog[it].second;

				SetDialogRow(PT_DialogRows_SmallIndent, dialogRow, false, (char*)ss.str().c_str(), color, true);
				BottomSelectedRow = dialogRow;
				itermNum++;
			}
		}
	}
	if (!TownersDialogRows[SelectedDialogRow].selectable) {
		if (SelectedDialogRow != PT_DialogRows_BigDialog_Back) {
			SelectedDialogRow = BottomSelectedRow;
		}
	}

	SetDialogRow(PT_DialogRows_NoIndent, PT_DialogRows_BigDialog_Back, true, "Back", C_0_White, false);
	LowerRow(PT_DialogRows_BigDialog_Back, PT_DialogRows_BigDialog_Back_VerticalIndent);
	DialogRowsCount = messageLog.size();
	if (DialogRowsCount == 0) {
		DialogRowsCount = 1; // Shows the default 'No messages'
	}
	ItemCount = DialogRowsCount-TradeRows;
	LimitToMin(ItemCount, 0);
}

//----- (th2) ------------------------------------------------------------
void DrawCombatTextAbovePlayer()
{
	if (showCombatText == false || CombatTextQueue.size() == 0) {// Do not call this draw function if there is nothing in the queue to draw, should improve performance.
		return;
	}

	constexpr float DurationOfTextInSeconds = 2;// These values only need to be computed once.
	constexpr int PercentOfTheScreenToTravel = 20;
	constexpr int percentToMerge = 80;
	constexpr double PI = 3.14159265;
	static double DistanceToTravel = ScreenHeight * PercentOfTheScreenToTravel / 100;

	vector<int> indexes; indexes.reserve(CombatTextQueue.size());

	if( CombatTextQueue.size() > 1 ){
		indexes.push_back(CombatTextQueue.size() - 1);
	}

	for (;;) {//This loop is run if merge of texts is expected.
		if (!mergeCombatTexts) {
			break;
		}

		vector<int> indexesToRemove;

		for (uint j = 0; j < indexes.size(); ++j) {
			CombatText lastElem = CombatTextQueue[indexes[j]];
			if (lastElem.callerID != -1) {
				for (uint i = 0; i < CombatTextQueue.size() - 1; ++i) {
					if (i == indexes[j]) {
						continue;
					}
					if (lastElem.description == CombatTextQueue[i].description && lastElem.callerID == CombatTextQueue[i].callerID) {
						if (abs(lastElem.iterations - CombatTextQueue[i].iterations) < (FPS * DurationOfTextInSeconds * percentToMerge / 100) && CombatTextQueue[i].iterations < (int)((float)FPS * DurationOfTextInSeconds)) {
							indexes.push_back(i);
							CombatTextQueue[i].value += lastElem.value;
							CombatTextQueue[indexes[j]].iterations = INT_MAX;
							break;
						}
					}
				}
			}
			indexesToRemove.push_back(j);//The original logic erases elements of indexes while iterating through it.  This may cause index mess up.  Now we remove these indices after the current loop is done.  But I'm not 100% sure about this change.
		}

		// Remove the elements from indexes vector in reverse order to avoid index shifting
		for (int i = indexesToRemove.size() - 1; i >= 0; --i) {
			indexes.erase(indexes.begin() + indexesToRemove[i]);
		}

		if (indexes.size() == 0) {
			break;
		}
	}

	for (int i = CombatTextQueue.size() - 1; i >= 0; --i) {//There is a subtle issue also exists in the original code: if I use Psion and quickly slap some mob, the damage text has a small chance of instantly disappear right after showing up...
		string text = CombatTextQueue[i].text;
		i64 val = CombatTextQueue[i].value;
		int iterations = CombatTextQueue[i].iterations;
		int color = CombatTextQueue[i].color;
		if (iterations < (int)((float)FPS * DurationOfTextInSeconds)) {
			int x, y;
			if (CombatTextQueue[i].showOnCenter == true) {
				x = Xofs - Screen_LeftBorder - GetTextWidth((char*)text.c_str()) / 2; //(ScreenWidth - GetTextWidth((char*)text.c_str())) / 2;
				y = Yofs - Screen_TopBorder; //int((float)ScreenHeight / 2.3);
			} else {
				x = CombatTextQueue[i].posX;
				y = CombatTextQueue[i].posY;
				int row = CombatTextQueue[i].posX - PlayerRowPos;
				int col = CombatTextQueue[i].posY - PlayerColPos;
				x = 32 * (row - col) + (200 * (Scroll.xOfs + PlayerShiftY) / 100 >> 1) + Xofs - Screen_LeftBorder - GetTextWidth((char*)text.c_str()) / 2;
				y = 16 * (row + col) + (200 * (Scroll.yOfs + PlayerShiftX) / 100 >> 1) + Yofs - Screen_TopBorder - 16;
			}
			//y = ax + b
			//a = tangent(angle)
			double radian_angle = CombatTextQueue[i].angle * PI / 180.0;
			int dest_x = x + int(DistanceToTravel * cos(radian_angle));
			int dest_y = y + int(DistanceToTravel * sin(radian_angle));
			double a = tan(radian_angle);
			double b = y - a * x;
			double progress = iterations / (FPS * DurationOfTextInSeconds);
			int diff_x = dest_x - x;
			int diff_y = dest_y - y;

			int drawx = x - int(progress * diff_x);
			int drawy = y - int(progress * diff_y);
			if (drawx > 0 && drawy < ScreenWidth && drawy > 0 && drawy < ScreenHeight) {
				char bfr[256];

				//int callerID;
				//int value;
				//string description;
				sprintf(bfr, text.c_str(), val);
				DrawLevelInfoText(drawx, drawy, bfr, color, OnScreenTextOutline);
			}
			CombatTextQueue[i].IncreaseIterations();
		} else {
			CombatTextQueue.erase(CombatTextQueue.begin() + i);
		}
	}
}

//----- (th2) ------------------------------------------------------------
void DrawFloatingHealthAndMana() {
	Player& player = Players[CurrentPlayerIndex];
	int curHP = player.CurLife>>6;
	int curMana = player.CurMana >> 6;
	int diffHP =  curHP-PlayerLastDrawnHealth;
	int diffMana = curMana-PlayerLastDrawnMana;
	if (PlayerLastDrawnHealth == -1) {
		PlayerLastDrawnHealth = curHP;
		return;
	}

	if (PlayerLastDrawnMana == -1) {
		PlayerLastDrawnMana = curMana;
		return;
	}
	PlayerLastDrawnHealth = curHP;
    PlayerLastDrawnMana = curMana;

	if (diffHP < 0) {
		if (ShowReceivedDamage == true) {
			if (abs(diffHP) > ReceivedDamageThreshold){
			CombatTextQueue.push_back(CombatText("- %i HP", C_2_Red, 0, 0, true, CurrentPlayerIndex, "decreasedHealth", abs(diffHP)));
			}
		}
	}
	else if (diffHP > FloatingHPRegenThreshold) {
		CombatTextQueue.push_back(CombatText("+ %i HP", C_6_Brownish, 0, 0, true, CurrentPlayerIndex, "increasedHealth", abs(diffHP)));
	}

	if (diffMana < 0) {
		CombatTextQueue.push_back(CombatText("- %i MP", C_7_Grey, 0, 0, true, CurrentPlayerIndex, "decreasedMana", abs(diffMana)));
	}
	else if (diffMana > FloatingManaRegenThreshold) {
		CombatTextQueue.push_back(CombatText("+ %i MP", C_1_Blue, 0, 0, true, CurrentPlayerIndex, "increasedMana", abs(diffMana)));
	}

}

//----- (th2) ------------------------------------------------------------
void DrawFloatingExp(i64 xpGain, int row, int col)
{

	Player& player = Players[CurrentPlayerIndex]; 
	if (showCombatExp) {
		if (ShowPercentExpGain && player.CharLevel < PlayerMaxLevel()) {
			double levelProgress = 0;
			int clvl = player.CharLevel;
			if( clvl < PlayerMaxLevel() ){
				i64 curXp = LevelUp(clvl);
				i64 prevXp = LevelUp(clvl - 1);
				i64 prevXpDelta = curXp - prevXp;
				levelProgress = (double)xpGain * 100 / prevXpDelta;
			}
			char buff[10];
			sprintf(buff, "%.2f", levelProgress);
			stringstream ss;
			ss << "+ %lli XP (" << buff << "%%)";
			CombatTextQueue.push_back(CombatText(ss.str(), C_0_White, row, col, false, -1, "gainXP", xpGain));
		}
		else {
			CombatTextQueue.push_back(CombatText("+ %i XP", C_0_White, row, col, false, -1, "gainXP", xpGain));
		}
	}
}

//----- (th2) ------------------------------------------------------------
void DrawFloatingDamage(int damage, int row, int col, int callerID, int color)
{
	if (showInflictedDamage == false) {
		return;
	}
	if (abs(damage) > ReceivedDamageThreshold) {
		CombatTextQueue.push_back(CombatText("- %i HP", color, row, col, false, callerID, "damageDealtToMonster", abs(damage)));
	}
}

//----- (th2) ------------------------------------------------------------
string getNameFromSlot(int slot, string newname)
{
	switch (slot) {
	case IS_Head: return "helmet";
	case IS_LeftHand: return newname;
	case IS_RightHand: return newname;
	case IS_Torso: return "armor";
	case IS_Waist: return "belt";
	case IS_Hands: return "gauntlets";
	case IS_Feet: return "boots";
	}
	return "unknown_slot_name";
}

//----- (th2) ------------------------------------------------------------
void DrawFloatingDamagedArmor(int slot, int dura, string newname)
{
	stringstream ss;
	ss << getNameFromSlot(slot,newname);
	if (dura == 0) {
		//ss << " losing max durability!";
		ss << " breaking!";
	}
	else if (dura == 1) {
		ss << " breaking!";
	}
	if (ShowBreakWarning == true) {
		//CombatTextQueue.push_back(CombatText(ss.str(), C_2_Red,"breakingItems"));
		CombatTextQueue.push_back(CombatText(ss.str(), C_2_Red, 0, 0, true, -1, "breakingItems", 0));
	}
}


//----- (00456DE1) --------------------------------------------------------
void __fastcall SplitAnimationPointerToDirections( char* animPtr, char** dirAnimPtr )
{
	uint* dirAnimOfs = (uint*) animPtr;
	for( int dir = 0; dir < 8; dir++ ){
		dirAnimPtr[dir] = animPtr + dirAnimOfs[dir];
	}
}

//int TrnIndex, UseTestTrn, TestTrnCount;
//uchar TestTrn[256];
// получение следующего тестового trn из списка
uchar* GetPlayerTrn()
{
	// временная залипуха берущая тестовый .trn из конфига
	extern char UserPlayerTrn[ 256/*4096*/ ];
	char* playerTrn = 0;
	if( *UserPlayerTrn ){
		playerTrn = UserPlayerTrn;
	}else{
		return 0;
	}
	if( ! PlayerTrn[0] ){
		PlayerTrn[0] = (uchar*) LoadFile( playerTrn );
	}
	return PlayerTrn[0];
}

// coloring player animation >>>
void __fastcall SetPlayerTrn( char** dirAnimPtr, uchar** colorTable, char* fileName = 0 )
{
	if( fileName && !*colorTable ){
		*colorTable = (uchar*) AllocMem( 256 );
		LoadFileWithMem( fileName, *colorTable );
		// этот блок взят из ColorMonsterSprite, так как trn применяется только там в оригинале, и он всегда вызывается
		//for( uchar* pal = colorTable; pal < colorTable + 256; pal++ ){
		//	if( *pal == 255u ){
		//		*pal = 0;
		//	}
		//}
	}
	for( int dir = 0; dir < 8; dir++ ){
		if( dirAnimPtr[ dir ] ){
			ColorSpriteAnimation( dirAnimPtr[ dir ], *colorTable, *(int*)dirAnimPtr[0] ); // количество кадров храниться первым int'ом каждого клипа
		}
	}
}

uchar* AssaFixTrn;
uchar* AxepertiseRogueTrn[2];
uchar* IrmaTrn[3];
uchar* BombTrn[3];
uchar* AssTrn[3];
uchar* GladTrn[3];
uchar* temPalTrn[3];
uchar* InquizTrn[3];
uchar* GuardTrn[3];
uchar* WarroTrn[3];
uchar* MonkTrn;
uchar* KensTrn;
uchar* ShgTrn;
uchar* ShnbTrn;
uchar* MageTrn[3];
uchar* PsiTrn[2];
uchar* ElemTrn[7];
uchar* WarlockTrn[3];
uchar* HydrmTrn[2];
uchar* DemnlgTrn[3];
uchar* NcrmncrTrn[3];
uchar* BstmstrTrn[3];
uchar* ArchTrn[3];
uchar* SctTrn[3];
uchar* SsterTrn[3];
uchar* TrprTrn[3];
uchar* SvgTrn[3];
uchar* ExeTrn[3];
uchar* BersTrn[3];
uchar* ThrxTrn[3];
uchar* MurmTrn[3];
uchar* DimaTrn[3];
uchar* RangerTrn;
uchar* WarWHUQMFixTrn;
uchar* DruTrn[1];
uchar* AssDarkener_v3;
int ArmorIndex, WeaponIndex, UseLookIndex;

//volatile int curAnimationsPointers;
//----- (00456DFC) --------------------------------------------------------
void __fastcall LoadPlayerGFX( uint playerIndex, int loadingMask )
{
	if( playerIndex >= PlayersMax_4 ) TerminateWithError("LoadPlrGFX: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	char classId = player.ClassID;
	int appearance = player.Appearance;
	
	// проверка на вылет в мульти при битом appearance
	if( uint( appearance >> 4 ) > 2 ){
		if( IsDebuggerPresent() ) __debugbreak(); // Wrong armor appperance
		appearance = (2 << 4) | appearance & 0xF ;
	}
	if( (appearance & 0xF) >= PAW_COUNT ){
		if( IsDebuggerPresent() ) __debugbreak(); // Wrong weapon appperance
		appearance = appearance & ~0xF | (PAW_COUNT - 1);
	}
	char armor  = ArmorAnim  [ UseLookIndex && Handler1 != DisableInputWindowProc ? ArmorIndex  : appearance >> 4 ];
	char weapon = WeaponAnims[ UseLookIndex && Handler1 != DisableInputWindowProc ? WeaponIndex : appearance & 0xF ].select;

	char look[16], look2[16] = {0};
	if( weapon == 'G' && ClassAnim[classId] != 'W' ) weapon = 'N';
	if (player.fullClassId == PFC_DRUID ){
		 sprintf( look2, "%c%c%c", ClassAnim[classId], armor, 'C' );
		 if( player.activeBuffFlag & BF_2_FURY_ACTIVE ) weapon = 'C';
	}
	sprintf( look, "%c%c%c", ClassAnim[classId], armor, weapon );
	// двуручки и одноручки в единственном числе
	if( is(UseLookIndex ? WeaponIndex : (appearance & 0xF), PAW_10_SWORD_ONE, PAW_11_MACE_ONE) && is(classId, PC_5_SAVAGE, PC_4_ROGUE) ){
		strcat( look, "2H" );
		strcat( look2, "2H" );
	}

	char** dirAnimPtr = 0;
	char* animPtr = 0;
	char* szSuffix = 0;
	for( uint animMask = 1; animMask < PAF_COUNT; animMask <<= 1 ){ // 0x17F
		if( ! (loadingMask & animMask) ) continue;
		switch( animMask ){
		case PAF_1_STAY:		szSuffix	= (Dungeon->genType || Dungeon->isQuest) ? "AS" : "ST"; // angry stay / town stay
								animPtr		= player.StayAnimCel;
								dirAnimPtr	= player.nothingAnimationsPointers;			break;
		case PAF_2_WALK:		szSuffix	= (Dungeon->genType || Dungeon->isQuest) ? "AW" : "WL"; // angry walk / town walk
								animPtr		= player.WalkAnimCel;
								dirAnimPtr	= player.walkAnimationsPointers;			break;
		case PAF_3_ATTACK:		szSuffix	= "AT"; // attack
								animPtr		= player.AttackAnimCel;
								dirAnimPtr	= player.attackAnimationsPointers;			break;
		case PAF_4_GOT_HIT:		szSuffix	= "HT"; // hit
								animPtr		= player.GotHitAnimCel;
								dirAnimPtr	= player.gotHitAnimationsPointers;			break;
		case PAF_5_LIGHT_CAST:	szSuffix	= "LM"; // light magic
								animPtr		= player.LightningSpellAnimCel;
								dirAnimPtr	= player.lightningSpellAnimationsPointers;	break;
		case PAF_6_FIRE_CAST:	szSuffix	= "FM"; // fire magic
								animPtr		= player.FireSpellAnimCel;
								dirAnimPtr	= player.fireSpellAnimationsPointers;		break;
		case PAF_7_MAGIC_CAST:	szSuffix	= "QM"; // quick magic
								animPtr		= player.MagicSpellAnimCel;
								dirAnimPtr	= player.magicSpellAnimationsPointers;		break;
		case PAF_8_DEATH:		if( player.Appearance & 0xF ) continue;
								szSuffix	= "DT"; // death
								animPtr		= player.DeathAnimCel;
								dirAnimPtr	= player.deathAnimationsPointers;			break;
		case PAF_9_BLOCK:		if( !player.CanBlock ) continue;
								szSuffix	= "BL"; // block
								animPtr		= player.BlockAnimCel;
								dirAnimPtr	= player.blockAnimationsPointers;			break;
		case PAF_10_TURNING:	szSuffix	= "TI"; // turning
								animPtr		= player.turningAnimCel;
								dirAnimPtr	= player.turningAnimationsPointers;			break;
		default: TerminateWithError("PLR:2");
		}
		char fileName[256];
		// TODO: rename anim files in data to use suffix above
		if( player.fullClassId == PFC_DRUID && animMask & PAF_10_TURNING ){
			if( player.activeBuffFlag & BF_2_FURY_ACTIVE ){
				sprintf( fileName, "PlrGFX\\%s\\%s\\%s%c.CL2", ClassAnimName[classId], look2, "SS_UNTURN_", armor );
			}else{
				sprintf( fileName, "PlrGFX\\%s\\%s\\%s%c.CL2", ClassAnimName[classId], look2, "SS_TURN_", armor );
			}
		}else{
			sprintf( fileName, "PlrGFX\\%s\\%s\\%s%s.CL2", ClassAnimName[classId], look, look, szSuffix );
		}
		// was "PlrGFX\\Warrior\\W\\WAS.CL2" bug! in network (udp) game
		LoadFileWithMem( fileName, animPtr );
		SplitAnimationPointerToDirections( animPtr, dirAnimPtr );
		if( true /*! UseTestTrn*/ ){ // #player TRN
			 // исправляем светлую рогу без оружия
			if( classId == PC_4_ROGUE /* classId == PC_4_ROGUE */ ){
				//"светлая: ala alb alh aln alt alu ama amb amh amn amt amu";
				//"темная: ald alm als amd amm ams";
				if( *(int*)look == '\0NLA' && animMask != PAF_8_DEATH || ( (UseLookIndex ? WeaponIndex : (appearance & 0xF)) > 9 && is(armor, 'L', 'M') )
				 || armor == 'L' && ( weapon == 'A' || weapon == 'B' || (weapon == 'H' && !(Dungeon->genType || Dungeon->isQuest)) || weapon == 'T' || weapon == 'U' )
				 || armor == 'M' && ( weapon == 'A' || weapon == 'B' || weapon == 'H' || weapon == 'N' || weapon == 'T' || weapon == 'U' ) 	){
					SetPlayerTrn( dirAnimPtr, &AssaFixTrn, "X\\plrtrn\\Ass01fix.trn" );
				}
				else if (Dungeon->genType &&
					(
						((armor == 'H' && weapon == 'M') && (is(animMask, PAF_1_STAY, PAF_3_ATTACK)))
						|| (armor == 'H' && weapon == 'N' && (is(animMask, PAF_1_STAY, PAF_3_ATTACK, PAF_2_WALK)))
						|| (
							armor == 'H'
							&& (
								(player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
								|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
								|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
								|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE))
							&& (is(animMask, PAF_2_WALK)
								)
							)
						|| (
							armor == 'H'
							&& (
								(player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
								|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
								)
							&& (is(animMask, PAF_7_MAGIC_CAST, PAF_5_LIGHT_CAST))
							)
						)
					) {
					// rogue is darkened
					SetPlayerTrn(dirAnimPtr, &AssaFixTrn, "X\\plrtrn\\Ass02-heavy-01.trn");
				}
				else if (Dungeon->genType && (armor == 'H' && weapon == 'S') && (animMask == PAF_1_STAY)) {// need to not apply this if holding two swords
					if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD) {
						//nothing
					}
					else if (player.OnBodySlots[IS_LeftHand].ItemCode != IC_1_SWORD || player.OnBodySlots[IS_RightHand].ItemCode != IC_1_SWORD) {
						// rogue in plate and 1 sword is darkened for idle animation
						SetPlayerTrn(dirAnimPtr, &AssaFixTrn, "X\\plrtrn\\Ass02-heavy-01.trn");
					}
				}
				else if (Dungeon->genType && (armor == 'L' && weapon == 'M') && (animMask == PAF_5_LIGHT_CAST)) {
					// rogue in light armor and dual maces is darkened for lightning cast animation
					SetPlayerTrn(dirAnimPtr, &AssaFixTrn, "X\\plrtrn\\Ass02-heavy-01.trn");
				}
				else if (Dungeon->genType && (armor == 'L' && weapon == 'S') && (animMask == PAF_5_LIGHT_CAST)) {
					SetPlayerTrn(dirAnimPtr, &AssDarkener_v3, "X\\plrtrn\\Ass03-darkener_v3.trn");
				}
			}
			// --------------------////////////////////--------------------- end Rogue ------------------------------------------------------------

			if ( player.fullClassId == PFC_ROGUE  &&  HasTrait ( playerIndex, TraitId::Axepertise ) ) {
				if (armor == 'L' || armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &IrmaTrn[0], "X\\plrtrn\\Rogue05axepertLite01.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &IrmaTrn[1], "X\\plrtrn\\Rogue05axepertPlate03.TRN");
				}
			}

			else if( player.fullClassId == PFC_IRON_MAIDEN ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &IrmaTrn[0], "X\\plrtrn\\Rogue01crimzee1lite1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &IrmaTrn[1], "X\\plrtrn\\Rogue01crimzee1med1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &IrmaTrn[2], "X\\plrtrn\\Rogue01crimzee1plate1.TRN");
				}
			}

			else if( player.fullClassId == PFC_BOMBARDIER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &BombTrn[0], "X\\plrtrn\\Rogue03Bombardier1lite1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &BombTrn[1], "X\\plrtrn\\Archer05goldValkyre1litearmor.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &BombTrn[2], "X\\plrtrn\\Archer05goldValkyre2plate2.TRN");
				}
			}
			//assassin trn
			else if( player.fullClassId == PFC_ASSASSIN ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &AssTrn[0], "X\\plrtrn\\Rogue02assassin1lite1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &AssTrn[1], "X\\plrtrn\\Rogue02assassin1lite1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &AssTrn[2], "X\\plrtrn\\Rogue02assassin1lite1.TRN");
				}
			}
			// окрашиваем монаха
			else if( player.fullClassId == PFC_MONK ){
				SetPlayerTrn( dirAnimPtr, &MonkTrn, "X\\PLRTRN\\Monk01x3.TRN" );
			}
			else if( player.fullClassId == PFC_KENSEI ){
				SetPlayerTrn( dirAnimPtr, &KensTrn, "X\\PLRTRN\\Monk06BlueImproved2.TRN" );
			}
			else if( player.fullClassId == PFC_SHUGOKI ){
				SetPlayerTrn( dirAnimPtr, &ShgTrn, "X\\PLRTRN\\Monk02RedBlackSkin.TRN" );
			}
			else if( player.fullClassId == PFC_SHINOBI ){
				SetPlayerTrn( dirAnimPtr, &ShnbTrn, "X\\plrtrn\\Monk07crimzonPlusBlackSkin1.TRN" /* ex- "X\\PLRTRN\\Monk03Black.TRN" */ );
			}
			// mages
			else if( player.fullClassId == PFC_MAGE ){
				if (armor == 'L') { 
					if (HasTrait(playerIndex, TraitId::Psion)) {
						SetPlayerTrn(dirAnimPtr, &PsiTrn[0], "X\\plrtrn\\Mage07red01-psio.TRN");
					}
					else {
						SetPlayerTrn(dirAnimPtr, &MageTrn[0], "X\\plrtrn\\Mage07red01_v2.TRN");
					}
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &MageTrn[1], "X\\plrtrn\\Mage07red02chain.TRN");
				}
				else if (armor == 'H') {
					if (HasTrait(playerIndex, TraitId::Psion)) {
						SetPlayerTrn(dirAnimPtr, &PsiTrn[1], "X\\plrtrn\\Mage07red01-psio.TRN");
					}
					else {
						SetPlayerTrn(dirAnimPtr, &MageTrn[2], "X\\plrtrn\\Mage07red01.TRN");
					}
				}
			}
			else if( player.fullClassId == PFC_ELEMENTALIST ){
				if (armor == 'L') {
					if (HasTrait(playerIndex, TraitId::Mamluk)) {
						SetPlayerTrn(dirAnimPtr, &ElemTrn[6], "X\\plrtrn\\Mage12_mamluke_02_lite.trn");
					}
					else {
						if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
							SetPlayerTrn(dirAnimPtr, &ElemTrn[3], "X\\plrtrn\\Mage10_Elem_SLMQM_BB2DB_v2.trn");
						}
						else {
							SetPlayerTrn(dirAnimPtr, &ElemTrn[0], "X\\plrtrn\\Mage01blue04.TRN");
						}
					}
				}
				else if (armor == 'M') {
					if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
						SetPlayerTrn(dirAnimPtr, &ElemTrn[4], "X\\plrtrn\\Mage10_Elem_SMMQM_BB2DB.trn");
					}
					else {
						SetPlayerTrn(dirAnimPtr, &ElemTrn[1], "X\\plrtrn\\Mage01blue04mid1.TRN");
					}
				}
				else if (armor == 'H') {
					if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
						SetPlayerTrn(dirAnimPtr, &ElemTrn[5], "X\\plrtrn\\Mage10_Elem_SHMQM_BB2DB.trn");
					}
					else {
						SetPlayerTrn(dirAnimPtr, &ElemTrn[2], "X\\plrtrn\\Mage01blue04.TRN");
					}
				}
			}
			else if( player.fullClassId == PFC_WARLOCK ){
				if (HasTrait(playerIndex, TraitId::Hydramancer)) {
					if (armor == 'L') {						
						if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
							SetPlayerTrn(dirAnimPtr, &ElemTrn[1], "X\\plrtrn\\Mage11bRed01-for_Q_animation.trn");
						}
						else {
							SetPlayerTrn(dirAnimPtr, &HydrmTrn[0], "X\\plrtrn\\Mage11bRed01.TRN");
						}
					}
					else if (armor == 'M') {
						if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
							SetPlayerTrn(dirAnimPtr, &ElemTrn[1], "X\\plrtrn\\Mage11bRed01-for_Q_animation.trn");
						}
						else {
							SetPlayerTrn(dirAnimPtr, &HydrmTrn[0], "X\\plrtrn\\Mage11bRed01.TRN");
						}
					}
					else if (armor == 'H') {
						if ( /*weapon == 'M' && */animMask == PAF_7_MAGIC_CAST) {
							SetPlayerTrn(dirAnimPtr, &ElemTrn[1], "X\\plrtrn\\Mage11bRed01-for_Q_animation.trn");
						}
						else {
							SetPlayerTrn(dirAnimPtr, &HydrmTrn[0], "X\\plrtrn\\Mage11bRed01.TRN");
						}
					}
				}
				else {
					if (armor == 'L') {
						SetPlayerTrn(dirAnimPtr, &WarlockTrn[0], "X\\plrtrn\\Mage05crimzee02darker1.TRN");
					}
					else if (armor == 'M') {
						SetPlayerTrn(dirAnimPtr, &WarlockTrn[1], "X\\cadaver01.TRN");
					}
					else if (armor == 'H') {
						SetPlayerTrn(dirAnimPtr, &WarlockTrn[2], "X\\plrtrn\\Mage05crimzee02darker1.TRN");
					}
				}
			}
			else if( player.fullClassId == PFC_DEMONOLOGIST ) {
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &DemnlgTrn[0], "X\\plrtrn\\Mage06neworange02lite3.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &DemnlgTrn[1], "X\\plrtrn\\Mage06neworange01x1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &DemnlgTrn[2], "X\\plrtrn\\Mage06neworange03heavy2.TRN");
				}
			}
			else if( player.fullClassId == PFC_NECROMANCER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &NcrmncrTrn[0], "X\\plrtrn\\Mage04Black02x1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &NcrmncrTrn[1], "X\\plrtrn\\Mage04Black02x2.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &NcrmncrTrn[2], "X\\plrtrn\\Mage04Black02x2.TRN");
				}
			}
			else if( player.fullClassId == PFC_BEASTMASTER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &BstmstrTrn[0], "X\\plrtrn\\Mage02beige02x1darker1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &BstmstrTrn[1], "X\\plrtrn\\Mage02beige02x1darker1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &BstmstrTrn[2], "X\\plrtrn\\Mage02beige02x1darker1.TRN");
				}
			}
			else if( player.fullClassId == PFC_SCOUT ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &SctTrn[0], "X\\plrtrn\\Archer04red1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &SctTrn[1], "X\\plrtrn\\Archer04red1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &SctTrn[2], "X\\plrtrn\\Archer04red1.TRN");
				}
			}
			else if( player.fullClassId == PFC_SHARPSHOOTER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &SsterTrn[0], "X\\plrtrn\\Archer02black01upd1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &SsterTrn[1], "X\\plrtrn\\Archer02black01upd1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &SsterTrn[2], "X\\plrtrn\\Archer02black02plate.TRN");
				}
			}
			else if( player.fullClassId == PFC_TRAPPER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &TrprTrn[0], "X\\plrtrn\\Archer03blue3.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &TrprTrn[1], "X\\plrtrn\\Archer03blue3.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &TrprTrn[2], "X\\plrtrn\\Archer03blue3.TRN");
				}
			}
			// warriors
			else if( player.fullClassId == PFC_TEMPLAR ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &temPalTrn[0], "X\\plrtrn\\Warrior01crimzo01lite2.TRN");
				}
				else if (armor == 'M') {
					// was :  "X\\plrtrn\\Warrior01crimzo02med3.TRN" - crimson
					SetPlayerTrn(dirAnimPtr, &temPalTrn[1], "X\\plrtrn\\Templar02x2.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &temPalTrn[2], "X\\plrtrn\\Warrior01crimzo03plate1.TRN");
				}
			}
			else if( player.fullClassId == PFC_INQUISITOR ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &InquizTrn[0], "X\\plrtrn\\Warrior04blue2beigeandorng2beige2.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &InquizTrn[1], "X\\plrtrn\\Warrior04blue2beigeandorng2beige2.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &InquizTrn[2], "X\\plrtrn\\Warrior04blue2beigeandorng2beige2.TRN");
				}
			}
			else if( player.fullClassId == PFC_GUARDIAN ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &GuardTrn[0], "X\\plrtrn\\Warrior03guardian2lite1.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &GuardTrn[1], "X\\plrtrn\\Warrior03guardian2.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &GuardTrn[2], "X\\plrtrn\\Warrior03guardian2.TRN");
				}
			}
			else if( player.fullClassId == PFC_WARRIOR ){
				if (GameMode == GM_CLASSIC) {
					// nothing
				}
				else if (HasTrait(playerIndex, TraitId::Ranger)) {
					SetPlayerTrn(dirAnimPtr, &RangerTrn, "X\\plrtrn\\Warrior07_silverish_01.TRN");
				}
				else {
					if (armor == 'L') {
						SetPlayerTrn(dirAnimPtr, &WarroTrn[0], "X\\plrtrn\\Warrior05blue2.TRN");
					}
					else if (armor == 'M') {
						SetPlayerTrn(dirAnimPtr, &WarroTrn[1], "X\\plrtrn\\Warrior05blue2.TRN");
					}
					else if (armor == 'H') {
						if (Dungeon->genType && (armor == 'H' && weapon == 'U') && (animMask == PAF_7_MAGIC_CAST)) {
							// rogue in light armor and dual maces is darkened for lightning cast animation
							SetPlayerTrn(dirAnimPtr, &WarWHUQMFixTrn, "X\\plrtrn\\Warrior08_whuqm02.trn");
						}
						else {
							SetPlayerTrn(dirAnimPtr, &WarroTrn[2], "X\\plrtrn\\Warrior05blue2.TRN");
						}
					}
				}
			}
			//Savages
			else if( player.fullClassId == PFC_SAVAGE ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &SvgTrn[0], "X\\plrtrn\\Savage01fixt02.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &SvgTrn[1], "X\\plrtrn\\Savage01med2.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &SvgTrn[2], "X\\plrtrn\\Savage01med2.TRN");
				}
			}
			else if( player.fullClassId == PFC_EXECUTIONER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &ExeTrn[0], "X\\plrtrn\\Savage02exec02.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &ExeTrn[1], "X\\plrtrn\\Savage02exec03med4.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &ExeTrn[2], "X\\plrtrn\\Savage02exec02.TRN");// "X\\plrtrn\\Executioner01plate01.TRN");
				}
			}
			else if( player.fullClassId == PFC_BERSERKER ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &BersTrn[0], "X\\plrtrn\\Savage04blackish01lite2.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &BersTrn[1], "X\\plrtrn\\Savage04blackish01mid1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &BersTrn[2], "X\\plrtrn\\Savage04blackish01mid1.TRN");
				}
			}
			else if( player.fullClassId == PFC_THRAEX ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &ThrxTrn[0], "X\\plrtrn\\_Gladiator.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &ThrxTrn[1], "X\\plrtrn\\_Gladiator_Chain_1.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &ThrxTrn[2], "X\\plrtrn\\_Gladiator2.TRN");
				}
			}
			else if( player.fullClassId == PFC_MURMILLO ){
				if (armor == 'L') {
					if (!Dungeon->genType && (armor == 'L' && weapon == 'D') && (animMask == PAF_2_WALK)) {
						SetPlayerTrn(dirAnimPtr, &MurmTrn[0], "X\\plrtrn\\Mur01_wlm_wldwl.trn");
					}
					else {
						SetPlayerTrn(dirAnimPtr, &MurmTrn[1], "X\\plrtrn\\Mur01.TRN"); //"CF\\SH\\Hirrona.TRN");
					}
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &MurmTrn[1], "X\\plrtrn\\Mur01.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &MurmTrn[1], "X\\plrtrn\\Mur01.TRN");
				}
			}
			else if( player.fullClassId == PFC_DIMACHAERUS ){
				if (armor == 'L') {
					SetPlayerTrn(dirAnimPtr, &DimaTrn[0], "CF\\SH\\Belly.TRN");
				}
				else if (armor == 'M') {
					SetPlayerTrn(dirAnimPtr, &DimaTrn[1], "X\\plrtrn\\Dimachaerus02Mail01.TRN");
				}
				else if (armor == 'H') {
					SetPlayerTrn(dirAnimPtr, &DimaTrn[2], "X\\plrtrn\\Dimachaerus01Plate01.TRN");
				}
			}
			else if (player.fullClassId == PFC_DRUID) {
				SetPlayerTrn(dirAnimPtr, &DruTrn[0], "CF\\SH\\_ORNG2CRIMZEE-BLUE.trn");
			}

		//}else if( UseTestTrn ){
			// добавляем тестовый .trn к игроку из конфига
			if( uchar* colorTable = GetPlayerTrn() ){
				SetPlayerTrn( dirAnimPtr, &colorTable );
			}
		}
		player.LoadPlayerAnimationMask |= (PLAYER_ANIM_FLAG) animMask;
	}
}

//----- (004570C0) --------------------------------------------------------
void __fastcall InitPlayerGFX( uint playerIndex )
{
	Player* player; // eax@3
	int loadingMask;        // edx@4
	if( playerIndex >= 4 )
		TerminateWithError("InitPlayerGFX: illegal player %d", playerIndex);
	player = &Players[playerIndex];
	if( player->CurLife & ~63 ){
		loadingMask = 0b101111111; // 0x17F без death
	}else{
		player->Appearance = 0;
		loadingMask = PAF_8_DEATH; // TODO: check why this optimization needs (may be for multiplayer dead bodys ? )
	}
	LoadPlayerGFX( playerIndex, loadingMask );
}

//----- (0045710A) --------------------------------------------------------
void __fastcall InitPlrGFXMem( uint playerIndex )
{
	if( playerIndex >= PlayersMax_4 ) TerminateWithError( "InitPlrGFXMem: illegal player %d", playerIndex );
	Player& player = Players[playerIndex];

	// calc maximum needed memory for each anim for each classe
	if( !(AnimSizeCounted & PAF_1_STAY      )){ AnimSizeCounted |= PAF_1_STAY;       AnimSizeStay  = max(SelectPlrAnimation( "AS" ), SelectPlrAnimation( "ST" )); }
	if( !(AnimSizeCounted & PAF_2_WALK      )){ AnimSizeCounted |= PAF_2_WALK;       AnimSizeWalk  = max(SelectPlrAnimation( "WL" ), SelectPlrAnimation( "AW" )); }
	if( !(AnimSizeCounted & PAF_3_ATTACK    )){ AnimSizeCounted |= PAF_3_ATTACK;     AnimSizeAttack    = SelectPlrAnimation( "AT" ); }
	if( !(AnimSizeCounted & PAF_4_GOT_HIT   )){ AnimSizeCounted |= PAF_4_GOT_HIT;    AnimSizeHit       = SelectPlrAnimation( "HT" ); }
	if( !(AnimSizeCounted & PAF_5_LIGHT_CAST)){ AnimSizeCounted |= PAF_5_LIGHT_CAST; AnimSizeLightning = SelectPlrAnimation( "LM" ); }
	if( !(AnimSizeCounted & PAF_6_FIRE_CAST )){ AnimSizeCounted |= PAF_6_FIRE_CAST;  AnimSizeFire      = SelectPlrAnimation( "FM" ); }
	if( !(AnimSizeCounted & PAF_7_MAGIC_CAST)){ AnimSizeCounted |= PAF_7_MAGIC_CAST; AnimSizeMagic     = SelectPlrAnimation( "QM" ); }
	if( !(AnimSizeCounted & PAF_8_DEATH     )){ AnimSizeCounted |= PAF_8_DEATH;      AnimSizeDeath     = SelectPlrAnimation( "DT" ); }
	if( !(AnimSizeCounted & PAF_9_BLOCK     )){ AnimSizeCounted |= PAF_9_BLOCK;      AnimSizeBlock     = SelectPlrAnimation( "BL" ); }
	if( !(AnimSizeCounted & PAF_10_TURNING  )){ AnimSizeCounted |= PAF_10_TURNING;   AnimSizeTurning   = SelectPlrAnimation( "TI" ); }

	player.StayAnimCel           = (char*) AllocMem( AnimSizeStay );
	player.WalkAnimCel           = (char*) AllocMem( AnimSizeWalk );
	player.AttackAnimCel         = (char*) AllocMem( AnimSizeAttack );
	player.GotHitAnimCel         = (char*) AllocMem( AnimSizeHit );
	player.LightningSpellAnimCel = (char*) AllocMem( AnimSizeLightning );
	player.FireSpellAnimCel      = (char*) AllocMem( AnimSizeFire );
	player.MagicSpellAnimCel     = (char*) AllocMem( AnimSizeMagic );
	player.DeathAnimCel          = (char*) AllocMem( AnimSizeDeath );
	player.BlockAnimCel          = (char*) AllocMem( AnimSizeBlock );
	player.turningAnimCel        = (char*) AllocMem( AnimSizeTurning );
	
	player.LoadPlayerAnimationMask = PAF_0_NO;
}

//----- (00457464) --------------------------------------------------------
uint __fastcall SelectPlrAnimation(char* animType)
{
	HANDLE animFile = 0;
	char classAnim[16];
	char fileName[256];
	uint sizeLoaded = 0;
	static const char* twoHand[2] = { "", "2H" };
	for( int clas = 0; clas < PC_COUNT; clas++ ) for( auto armorAnim: ArmorAnim ) for( auto& weaponAnim: WeaponAnims ) for( auto hand: twoHand ){
		sprintf( classAnim, "%c%c%c%s", ClassAnim[clas], armorAnim, weaponAnim.select, hand );
		sprintf( fileName, "PlrGFX\\%s\\%s\\%s%s.CL2", ClassAnimName[clas], classAnim, classAnim, animType );
		if( File_Open( fileName, &animFile, 1 ) ){
			uint fileSize = File_GetSize( animFile, 0 );
			File_Close( animFile );
			if( sizeLoaded <= fileSize ){
				sizeLoaded = fileSize;
			}
		}
	}
	// TODO: remove after renaming trasformation anim
	static const char* turnAnim[2] = { "SS_UNTURN_", "SS_TURN_" };
	if( strcmp( animType, "TI" ) == 0 ){
		int clas = PC_5_SAVAGE;
		for( auto armorAnim: ArmorAnim ) for( auto hand: twoHand ) for( auto turn: turnAnim ){
			sprintf( classAnim, "%c%c%c%s", ClassAnim[clas], armorAnim, 'C', hand );
			sprintf( fileName, "PlrGFX\\%s\\%s\\%s%c.CL2", ClassAnimName[clas], classAnim, turn, armorAnim );
			if( File_Open( fileName, &animFile, 1 ) ){
				uint fileSize = File_GetSize( animFile, 0 );
				File_Close( animFile );
				if( sizeLoaded <= fileSize ){
					sizeLoaded = fileSize;
				}
			}
		}
	}
	return sizeLoaded;
}

//----- (0045758E) --------------------------------------------------------
void __fastcall FreePlayerGFX(uint playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError( "FreePlayerGFX: illegal player %d", playerIndex );
	}
	Player& player = Players[ playerIndex ];
	FreeMemZero( player.StayAnimCel );
	FreeMemZero( player.WalkAnimCel );
	FreeMemZero( player.AttackAnimCel );
	FreeMemZero( player.LightningSpellAnimCel );
	FreeMemZero( player.FireSpellAnimCel );
	FreeMemZero( player.MagicSpellAnimCel );
	FreeMemZero( player.GotHitAnimCel );
	FreeMemZero( player.DeathAnimCel );
	FreeMemZero( player.BlockAnimCel );
	FreeMemZero( player.turningAnimCel );
	player.LoadPlayerAnimationMask = PAF_0_NO;
}

//----- (004576A5) --------------------------------------------------------
void __fastcall NewPlayerAnimation(int playerIndex, char* animation, int framesCount, int animationDelay, int animPitch)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("NewPlrAnim: illegal player %d", playerIndex);
	}
	player.currentAnimationPointer = animation;
	player.currentAnimationFramesCount = framesCount;
	player.currentAnimationFrame = 1;
	player.currentAnimationFrameTick = 0;
	player.currentAnimationDelay = animationDelay;
	player.animWidth = animPitch;
	player.animWidth2 = (animPitch - 64) / 2;
}

//----- (00457738) --------------------------------------------------------
void __fastcall ClearPlrPVars(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("ClearPlrPVars: illegal player %d", playerIndex);
	}
	player.CursorRow = 0;
	player.CursorCol = 0;
	player.newDirection = 0;
	player.HorizontalNextRowOrSomeSpellData = 0;
	player.HorizontalNextCol = 0;
	player.dx64 = 0;
	player.dy64 = 0;
	player.currentActionFrame = 0;
	player.PrevCursorX = 0;//Should resolve edge case for Teleport location fix.
	player.PrevCursorY = 0;
}

//----- (004577C1) --------------------------------------------------------
void __fastcall SetPlrAnims(uint playerIndex)
{
	int classId; // edi@3
	int weaponWield; // edx@6

	if (playerIndex >= 4) {
		TerminateWithError("SetPlrAnims: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	player.AnimPitchStay = 96;
	player.AnimPitchWalk = 96;
	player.AnimPitchAttack = 128;
	player.AnimPitchHit = 96;
	player.AnimPitchCast = 96;
	player.AnimPitchDeath = 128;
	player.AnimPitchBlock = 96;
	classId = Players[playerIndex].ClassID;
	if (Dungeon->genType || Dungeon->isQuest) {
		player.StayAnimationFrameCount = ClassPropTable[classId].AngryStayFrames;
		player.WalkAnimationFrameCount = ClassPropTable[classId].AngryWalkFrames;
		player.AttackAnimationFrameCount = ClassPropTable[classId].AttackFrames;
		player.HitRecoveryAnimationFrameCount = ClassPropTable[classId].HitFrames;
		player.SpellAnimationFrameCount = ClassPropTable[classId].SpellFrames;
		player.DeathAnimationFrameCount = ClassPropTable[classId].DeathFrames;
		player.BlockAnimationFrameCount = ClassPropTable[classId].BlockFrames;
		player.AttackMainFrame = ClassPropTable[classId].AttackActionIndex;
	}
	else { // Town
		player.StayAnimationFrameCount = ClassPropTable[classId].StayFrames;
		player.WalkAnimationFrameCount = ClassPropTable[classId].WalkFrames;
		player.DeathAnimationFrameCount = ClassPropTable[classId].DeathFrames;
		player.SpellAnimationFrameCount = ClassPropTable[classId].SpellFrames;
	}
	player.SpellAnimationActionFrameIndex = ClassPropTable[classId].SpellActionIndex; // player attack speeds
	weaponWield = UseLookIndex ? WeaponIndex : player.Appearance & 0xF;
	// TODO: change to switch
	if (is(player.fullClassId, PFC_WARRIOR)) {
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE, PAW_10_SWORD_ONE, PAW_11_MACE_ONE, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD)) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 10 : 9;
			return;
		}
		/*if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD)) {
			player.AttackMainFrame = 10;
			return;
		}*/
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 10;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		int atk_frame = HasTrait(playerIndex, TraitId::Ranger)? 9 : 11;
		if (HasTrait(playerIndex, TraitId::Vigorous)) {
			atk_frame += 1;
		}
		player.AttackMainFrame = atk_frame;
		return;
	}
	else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_TEMPLAR) {
		if (weaponWield != PAW_4_BOW) {
			if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE, PAW_10_SWORD_ONE, PAW_11_MACE_ONE, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD)) {
				player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 10 : 9;
				return;
			}
			if (weaponWield == PAW_5_AXE) {// will they even ever use axe?....
				player.AttackAnimationFrameCount = 20;
				player.AttackMainFrame = 10;
				return;
			}
			if (weaponWield <= PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 12 : 11;
		return;
	}
	else if (player.fullClassId == PFC_GUARDIAN) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 9;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			//	player.AttackAnimationFrameCount = 16;
			//	player.AttackMainFrame = 11;
			//	return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 9;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_1_SHIELD, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_12_PISTOL)) {
			int atk_speed = 11;
			if (HasTrait(playerIndex, TraitId::Pistoleer)) { 
				atk_speed = 10; 
			}
			if (HasTrait(playerIndex, TraitId::Vigorous)) {
				atk_speed += 1;
			}
			player.AttackMainFrame = atk_speed;
			return;
		}
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE)) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 10 : 9;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 12 : 11;
		return;
	}
	else if (player.fullClassId == PFC_ARCHER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 8 : 7;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_1_SHIELD, PAW_2_SWORD, PAW_3_SWORD_SHIELD, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackAnimationFrameCount = 18;
			player.AttackMainFrame = 10;
			return;
		}
	}
	else if (player.fullClassId == PFC_SCOUT) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			int strafer_trait_penalty = HasTrait(playerIndex, TraitId::Strafer) ? 8 : 7;
			player.AttackMainFrame = strafer_trait_penalty;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_1_SHIELD, PAW_2_SWORD, PAW_3_SWORD_SHIELD, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackAnimationFrameCount = 18;
			player.AttackMainFrame = 10;
			return;
		}
	}
	else if (player.fullClassId == PFC_SHARPSHOOTER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 8;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_1_SHIELD, PAW_2_SWORD, PAW_3_SWORD_SHIELD, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackAnimationFrameCount = 18;
			player.AttackMainFrame = 10;
			return;
		}
	}
	else if (player.fullClassId == PFC_TRAPPER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 10;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_1_SHIELD) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 11 : 10;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_2_SWORD, PAW_3_SWORD_SHIELD, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackAnimationFrameCount = 18;
			player.AttackMainFrame = 11;
			return;
		}
	}
	else if (classId == PC_2_MAGE) {
		player.AnimPitchCast = 128;
		if (weaponWield == PAW_0_NONE) {
			player.AttackAnimationFrameCount = 20;
			return;
		}
		else if (weaponWield == PAW_1_SHIELD) {
			player.AttackMainFrame = 9;
			return;
		}
		else if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 15;
		}
		else {
			if (weaponWield != PAW_5_AXE) {
				return;
			}
			player.AttackAnimationFrameCount = 24;
		}
		player.AttackMainFrame = 16;
	}
	else if (classId == PC_3_MONK) {
		player.AnimPitchStay = 112;
		player.AnimPitchWalk = 112;
		player.AnimPitchAttack = 130;
		player.AnimPitchHit = 98;
		player.AnimPitchCast = 114;
		player.AnimPitchDeath = 160;
		player.AnimPitchBlock = 98;
		if (weaponWield < 0) { // этого по идее не может быть в принципе ( weaponWield = player.Appearance & 0xF; )
			return;
		}
		if (weaponWield <= PAW_1_SHIELD) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 8;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 20;
		}
		else {
			if (weaponWield != PAW_5_AXE) {
				if (weaponWield == PAW_8_STAFF) {
					player.AttackAnimationFrameCount = 13;
					player.AttackMainFrame = 8;
				}
				return;
			}
			player.AttackAnimationFrameCount = 23;
		}
		player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 15 : 14;
		return;
	}
	else if (player.fullClassId == PFC_BERSERKER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 9 : 8;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 12 : 11;
		return;
	}
	else if (player.fullClassId == PFC_SAVAGE) {
		if (weaponWield == PAW_10_SWORD_ONE || weaponWield == PAW_11_MACE_ONE || weaponWield == PAW_2_SWORD || weaponWield == PAW_6_MACE) {
			player.AttackAnimationFrameCount = 16;
			if (HasTrait(playerIndex, TraitId::Bouncer)) {
				player.AttackMainFrame = 11;
			}
			else {
				player.AttackMainFrame = 9;
			}
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (player.fullClassId == PFC_EXECUTIONER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 9;
			return;
		}
		if (is(weaponWield, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 10;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
	//	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
	//		if(is(player.LoadPlayerAnimationMask, PAF_32_FIRE_CAST, PAF_16_LIGHT_CAST, PAF_64_MAGIC_CAST)){
	//			player.?
	//		}
	//	}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (is(player.fullClassId, PFC_THRAEX, PFC_SECUTOR)) {
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 12;
			return;
		}
		if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD)) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 10 : 9;
			return;
		}
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 13;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 12 : 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (player.fullClassId == PFC_DRUID) {
		/*if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 12;
			return;
		}*/
		//if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD)) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 10 : 9;
			return;
		//}
		/*if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 13;
			return;
		}*/
		/*if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 12 : 11;
			return;
		}*/
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (is(player.fullClassId, PFC_MURMILLO)) {
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 12;
			return;
		}
		if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD)) {
			player.AttackMainFrame = 10 - PerkValue(PERK_FESTINA_LENTE, playerIndex);
			return;
		}
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 13;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (player.fullClassId == PFC_DIMACHAERUS) {
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE)) {
			player.AttackMainFrame = 9;
			return;
		}
		if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 20;
			player.AttackMainFrame = 13;
			return;
		}
		if (weaponWield != PAW_4_BOW) {
			if (weaponWield != PAW_8_STAFF) {
				return;
			}
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (Dungeon->genType || Dungeon->isQuest) {
			player.StayAnimationFrameCount = 8;
		}
		player.AnimPitchAttack = 96;
		player.AttackMainFrame = 11;
		return;
	}
	else if (player.fullClassId == PFC_ROGUE) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Axepertise) ? 12 : 13;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_3_SWORD_SHIELD, PAW_2_SWORD)) {
			player.AttackMainFrame = 8;
			return;
		}
	}
	else if (player.fullClassId == PFC_ASSASSIN) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 13;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = 11;
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_0_NONE, PAW_2_SWORD, PAW_3_SWORD_SHIELD, PAW_6_MACE, PAW_7_MACE_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = 12;
			return;
		}
		if (weaponWield == PAW_1_SHIELD) {
			player.AttackMainFrame = HasTrait(playerIndex, TraitId::Vigorous) ? 9 : 8;
			return;
		}
	}
	else if (player.fullClassId == PFC_IRON_MAIDEN || player.fullClassId == PFC_BOMBARDIER) {
		if (weaponWield == PAW_5_AXE) {
			player.AttackAnimationFrameCount = 22;
			player.AttackMainFrame = 13;
			return;
		}
		if (weaponWield == PAW_4_BOW) {
			player.AttackAnimationFrameCount = 12;
			player.AttackMainFrame = /*HasTrait(playerIndex, TraitId::Vigorous) ? 11 :*/ 10;// unavailable to im anymore
			return;
		}
		if (weaponWield == PAW_8_STAFF) {
			player.AttackAnimationFrameCount = 16;
			player.AttackMainFrame = 11;
			return;
		}
		if (is(weaponWield, PAW_3_SWORD_SHIELD, PAW_7_MACE_SHIELD, PAW_1_SHIELD, PAW_10_SWORD_ONE, PAW_11_MACE_ONE)) {
			player.AttackMainFrame = /*HasTrait(playerIndex, TraitId::Vigorous) ? 10 :*/ 9;// unavailable to im anymore
			return;
		}
		if (is(weaponWield, PAW_2_SWORD, PAW_6_MACE)) {
			player.AttackMainFrame = 10;
			return;
		}
	}
}

//----- (00457C58) --------------------------------------------------------
void __fastcall ClearAuras( Player& player )
{
	for( int i = 0; i < countof(Player::AuraIndex); i++ ){
		player.AuraIndex[i] = 0;
		player.AurasValues[i] = 0;
		AuraSoundStates[i] = false;
		AuraCooldown[i] = 0;
	}
}

//----- (00457CC2) --------------------------------------------------------
void __fastcall CreatePlayer( int playerIndex, char classId, char subclassId, char specializationId, GAME_MODE gameMode, __int64 gameChanger, decltype(Player::traits) traits )
{
	Player& player = Players[playerIndex];
	memset(&Players[playerIndex], 0, sizeof(Player));
	ClearAuras(player);
	SetRndSeed(ms(GetTickCountTh())); // перехватываем вызов
	if( playerIndex >= 4 ){
		TerminateWithError("CreatePlayer: illegal player %d", playerIndex);
	}
	player.ClassID = (PLAYER_CLASS)classId;
    player.subclassID = (PLAYER_SUBLASS)subclassId;
    player.specializationID = (PLAYER_SPECIALIZATION)specializationId;
	player.fullClassId = GetPlayerFullClass(player.ClassID, player.subclassID, player.specializationID);
    player.gameMode = gameMode;
	player.gameChanger = gameChanger;
    player.traits = traits;
	const PlayerClass& playerClass = PlayerClasses[classId];
	const FullPlayerClass& fullPlayerClass = FullPlayerClasses[player.fullClassId];
	const PlayerStat startStat = GetStartStat( player.fullClassId, traits, playerIndex );
	player.BaseStrength	 = player.CurStrength  = startStat.Strength;
	player.BaseMagic	 = player.CurMagic	   = startStat.Magic;
	player.BaseDexterity = player.CurDexterity = startStat.Dexterity;
	player.BaseVitality	 = player.CurVitality  = startStat.Vitality;
	player.AvailableLvlPoints = 0;
	player.OpenedTownDoorMask = 0;
	player.DungeonMessageFlags1 = 0;
	player.SomeSoundMuteDelay = 0;
	player.DungeonMessageFlags2 = 0;
	player.allowedDifficulty = 0;
	player.Difficulty = 0;
	// походу тут в оригинале предполагалась какая то проверка CharLevel при levelUp
	player.CharLevel = 1;
	player.controlCharLevel = player.CharLevel;
	const DamageMultipliers& damMult = playerClass.BaseDamageMultipliers;
	int baseDamageNumerator = player.CharLevel * (damMult.StrengthMultiplier * player.CurStrength + damMult.DexterityMultiplier * player.CurDexterity);
	player.BaseDamage = baseDamageNumerator / damMult.Denominator;
	player.BlockBonus = playerClass.BlockBonus;
	player.CurLife = playerClass.LifeBase << 6;
	player.MaxCurLife = player.CurLife;
	player.BaseLife = player.CurLife;
	player.MaxBaseLife = player.CurLife;
	player.CurMana = playerClass.ManaBase << 6;
	player.MaxBaseMana = player.BaseMana = player.MaxCurMana = player.CurMana;
	player.Xp = 0;
	player.NextLevelUp = LevelUp(1);
	player.LightRadius = 9;
	player.Infravision = 0;
	if( player.fullClassId == PFC_IRON_MAIDEN || HasTrait(playerIndex, TraitId::Mamluk) ){// (Yarik): Quick and dirty hack to override IM skill
	    player.AvailableSkillMask = BIT(PS_26_ITEM_REPAIR);
    }
	else if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		player.AvailableSkillMask = BIT(PS_9_INFRAVISION);
	}
	else if (HasTrait(playerIndex, TraitId::Hydramancer)) {
		player.AvailableSkillMask = BIT(PS_13_HYDRA);
	}
	else if (HasTrait(playerIndex, TraitId::Paladin)) {
		player.AvailableSkillMask = BIT(PS_11_MANA_SHIELD);
	}
	/*else if (player.fullClassId == PFC_ARCHER && GameMode == GM_CLASSIC) {
		player.AvailableSkillMask = BIT(PS_28_TRAP_DISARM);
	}*/
	else{
        player.AvailableSkillMask = BIT(playerClass.SkillIndex);
    }
	player.AvailableSpellMask = 0ll;
	for( int i = 0; i < 64; i++){
		player.SpellLevels[i] = 0;
	}
	player.activeBuffFlag = 0;
	BattleTrance = false;
	InitBerserk = true;
	if( player.fullClassId == PFC_MAGE || player.fullClassId == PFC_DRUID){
		if (GameMode == GM_COLISEUM) {
			player.AvailableSpellMask = BIT(PS_1_FIREBOLT) | BIT(PS_30_CHARGED_BOLT); // 0x60000001;
			player.SpellLevels[PS_1_FIREBOLT] = 1;
			player.SpellLevels[PS_30_CHARGED_BOLT] = 1;
		}
		else {
			player.AvailableSpellMask = BIT(PS_1_FIREBOLT) | BIT(PS_30_CHARGED_BOLT) | BIT(PS_31_HOLY_BOLT); // 0x60000001;
			player.SpellLevels[PS_1_FIREBOLT] = 1;
			player.SpellLevels[PS_30_CHARGED_BOLT] = 1;
			player.SpellLevels[PS_31_HOLY_BOLT] = 1;
		}
	}	
	else if (player.fullClassId == PFC_TEMPLAR && HasTrait(playerIndex, TraitId::Paladin)) {
		player.AvailableSpellMask = BIT(PS_1_FIREBOLT) | BIT(PS_30_CHARGED_BOLT) | BIT(PS_31_HOLY_BOLT); // 0x60000001;
		player.SpellLevels[PS_1_FIREBOLT] = 1;
		player.SpellLevels[PS_30_CHARGED_BOLT] = 1; 
		player.SpellLevels[PS_31_HOLY_BOLT] = 1;
	}
	else if (player.fullClassId == PFC_ELEMENTALIST && !HasTrait(playerIndex, TraitId::Mamluk) ) {
		if (GameMode == GM_COLISEUM) {// traits don't exist in Coliseum...
			player.AvailableSpellMask = BIT(PS_56_ICE_BOLT) | BIT(PS_30_CHARGED_BOLT); // 0x60000001;
			player.SpellLevels[PS_56_ICE_BOLT] = 1;
			player.SpellLevels[PS_30_CHARGED_BOLT] = 1;
		}
		else {
			player.AvailableSpellMask = BIT(PS_56_ICE_BOLT) | BIT(PS_30_CHARGED_BOLT) | BIT(PS_31_HOLY_BOLT); // 0x60000001;
			player.SpellLevels[PS_56_ICE_BOLT] = 1;
			player.SpellLevels[PS_30_CHARGED_BOLT] = 1;
			player.SpellLevels[PS_31_HOLY_BOLT] = 1;
		}
	}
	else if (player.fullClassId == PFC_WARLOCK) {
		player.AvailableSpellMask = BIT(PS_59_RANCID_BOLT) | BIT(PS_30_CHARGED_BOLT) | BIT(PS_1_FIREBOLT); // 0x60000001;
		player.SpellLevels[PS_59_RANCID_BOLT] = 1;
		player.SpellLevels[PS_30_CHARGED_BOLT] = 1;
		player.SpellLevels[PS_1_FIREBOLT] = 1;
	}
	else if (player.subclassID == PSC_MAGE_SUMMONER) {
		player.AvailableSpellMask |= BIT(PS_52_LESSER_SUMMON);
		player.SpellLevels[PS_52_LESSER_SUMMON] = 1;
		player.AvailableSkillMask |= BIT(PS_55_UNSUMMON);
	}
	for( int i = 0; i < 4; i++){
		player.SpellOnFuncKeys[i] = -1;
	}
	player.Appearance = playerClass.Appearance;
	player.dungeonVisited.reset();
	player.lvlChanging = 0;
	player.CountOfReflectCharges = 0;
	player.griswoldLevel = 0;
	SetCols(playerIndex);
	InitDungMsgs(playerIndex);
	CreateInitialItems(playerIndex);
	for (int i = 0; i < PERKS_MAX_800; ++i) {
		player.perk[i] = 0;
	}
	SetRndSeed(0);
}

// Для каждого уровня своя максимальная сумма параметров
// при ее достижении они показываются золотом
//----- (0045829F) -------------------------------------------------------- // MSL - "Mordor's stat limitations" feature
int __fastcall LevelPointsToMax(int playerIndex)
{
	Player& player = Players[playerIndex];
	const PlayerStat maxStat = GetPlayerMaxStat( playerIndex );
	int maxStatsSum = maxStat.Strength + maxStat.Magic + maxStat.Dexterity + maxStat.Vitality;
	int curStatsSum = player.BaseStrength + player.BaseDexterity + player.BaseMagic + player.BaseVitality;
	int pointsToMax = maxStatsSum - curStatsSum;
	int statSum = CalcCharBaseStatSum( player );
	int clvl = player.CharLevel;
	int maxStatSumCap;

	// #msl: mordor's stat limitation
	int blueblood_limit_raise = HasTrait(playerIndex, TraitId::BlueBlood) ? 40 : 0;
	int gifted_limit = HasTrait(playerIndex, TraitId::Gifted) ? 100 : 0;
	int giant_limit = HasTrait(playerIndex, TraitId::Giant) ? 100 : 0;
	int paladin_limit = HasTrait(playerIndex, TraitId::Paladin) ? (200 + PerkValue(SYNERGY_THAUMATURGY, playerIndex)) : 0;
	int nightkin_limit = 0; 
	if (MaxCountOfPlayersInGame == 1 && (Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN))) { 
		nightkin_limit = 140; 
	}
	int oldfashioned = HasTrait(playerIndex, TraitId::OldFashioned) ? 40 : 0;
	int leper = HasTrait(playerIndex, TraitId::Leper) ? 150 : 0;
	int lithe = HasTrait(playerIndex, TraitId::LitheBuild) ? 100 : 0;
	int extra_mode_boost = 0;
	if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) { 
		extra_mode_boost = clvl / 2 + 50;
	}
	maxStatSumCap = (10 * clvl) 
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
			if (maxStatSumCap > (1400 + paladin_limit)) { 
				maxStatSumCap = 1400 + paladin_limit; 
			}
		}
		else {
			if (maxStatSumCap > 1400) { 
				maxStatSumCap = 1400; 
			}
		}
		maxStatSumCap += PerkValue(PERK_RAISE_THE_BAR, playerIndex); 
	}
	maxStatSumCap += 4 * PerkValue(SYNERGY_OSTENTATION, playerIndex); 
	if (GameMode == GM_CLASSIC) { maxStatSumCap = 460; }

	int pointsToMaxCap = maxStatSumCap - statSum;
	if (pointsToMax > pointsToMaxCap) {
		pointsToMax = pointsToMaxCap;
	}
	return pointsToMax;

}

//----- (004582E7) --------------------------------------------------------
void __fastcall NextPlayerLevel(int playerIndex)
{
	if( (uint) playerIndex >= 4 ){
		TerminateWithError( "NextPlrLevel: illegal player %d", playerIndex );
	}
	Player& player = Players[playerIndex];
	if( player.CharLevel >= PlayerMaxLevel() ) return;
	++player.CharLevel;
	++player.controlCharLevel;
	RecalcPlayer( playerIndex, 1 );
	int adventurerTraitBenefit = HasTrait(playerIndex, TraitId::Adventurer) ? 1 : 0;
	int domesticatedTraitPenalty = HasTrait(playerIndex, TraitId::Domesticated) ? 1 : 0;
	int bluebloodTraitPenalty = HasTrait(playerIndex, TraitId::BlueBlood) ? 1 : 0;
	int default_points = 5;
	int paladin_bonus_fur_extra_modes = 0;
	if ( is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL, GM_SPEEDRUN) && HasTrait(playerIndex, TraitId::Paladin) ) {
		paladin_bonus_fur_extra_modes = 1;
	}
	if (is(GameMode, GM_EASY, /*GM_CLASSIC,*/ GM_NORMAL, GM_HARD, GM_HARDCORE) 
		&& MaxCountOfPlayersInGame == 1
		&& (player.gameChanger & BIT(GC_1_AUTO_SAVE) || player.gameChanger & BIT(GC_26_ASCETICISM)) ) {
		default_points = RngFromRange(4, 6);
	}
	else if (GameMode == GM_NIGHTMARE) {
		default_points = RngFromRange(3, 7);
	}
	else if (GameMode == GM_IRONMAN ) {
		int ironman_extra_points = 2 * player.CharLevel / 15;
		if (player.gameChanger & BIT(GC_24_RUTHLESS)) {//  with Ruthless GC no extra stats in Ironman
			ironman_extra_points = 0;
		}
		LimitToMax(ironman_extra_points, 4);
		default_points += ironman_extra_points;
	}
	else if (player.gameChanger & BIT(GC_23_TOUGH_LUCK)) {
		default_points = 2;
	}
	int points = default_points 
		//+ 160//test msl
		+ PerkValue(PERK_EDUCATED, playerIndex)
		+ PerkValue(SYNERGY_OVERWHELM, playerIndex)
		+ PerkValue(SYNERGY_HIGHER_EDUCATION, playerIndex) 
		+ adventurerTraitBenefit 
		+ paladin_bonus_fur_extra_modes
		- domesticatedTraitPenalty 
		- bluebloodTraitPenalty;
	if( LevelPointsToMax( playerIndex ) >= points ){
		player.AvailableLvlPoints += points;
	}else{
		player.AvailableLvlPoints = LevelPointsToMax( playerIndex );
	}
	player.NextLevelUp = LevelUp( player.CharLevel );
	int life = 0;
	if( MaxCountOfPlayersInGame == 1 ){ // single
		life = player.ClassID != PC_2_MAGE ? 129 : 65;
	}else{ // multi
		life = player.ClassID != PC_2_MAGE ? 128 : 64;
	}
	player.MaxCurLife += life;
	player.CurLife = player.MaxCurLife;
	player.MaxBaseLife += life;
	player.BaseLife = player.MaxBaseLife;
	int mana = 0;
	if( player.ClassID == PC_0_WARRIOR ){
		mana = 64;
	}else{
		mana = player.ClassID != PC_5_SAVAGE ? 0 : 128;
	}
	if( MaxCountOfPlayersInGame == 1 ){
		++mana;
	}
	player.MaxCurMana += mana;
	player.MaxBaseMana += mana;
	#ifdef OLD_CODE
	if( ! player.effectFlag[EA_LOSE_REGENERATION] )
	#endif
	player.CurMana = player.MaxCurMana;
	player.BaseMana = player.MaxBaseMana;
	if( playerIndex == CurrentPlayerIndex ){
		PlayGlobalSound( S_1022_LEVELUP );
	}
}

//----- (00458492) --------------------------------------------------------
void __fastcall AddPlayerExperience(int playerIndex, i64 monsterXp, int row, int col )
{
	Player& player = Players[playerIndex];
	int clvl = player.CharLevel;
	if( playerIndex == CurrentPlayerIndex ){
		if( CurrentPlayerIndex >= 4 )
			TerminateWithError("AddPlrExperience: illegal player %d", CurrentPlayerIndex);
		if( player.CurLife > 0 ){
			#if 0//previous clvl modifier used since TH1, keeping it in case I need to go back to it...
			if (Difficulty < 1) {
				clvl = 2 * Dungeon->level + clvl / 2;
			}
			else if (Difficulty == 1) {
				if (Dungeon->level >= 1 && Dungeon->level < 7) {
					clvl = clvl / 2 + 1;// 1-6
				}
				else if (Dungeon->level > 6 && Dungeon->level < 10) {
					clvl = clvl / 2 + 8;// 7-9
				}
				else if (Dungeon->level > 9 && Dungeon->level < 13) {
					clvl = clvl / 2 + 15;// 10-12
				}
				else {
					clvl = clvl / 2 + 8;// 13-24
				}
			}
			else {
				if (Dungeon->level >= 1 && Dungeon->level < 6) {
					clvl = (clvl / 2) + 15;// 1-5
				}
			}
			#endif

			#if 0// testing turned off clvl to mlvl ratio modifier for xp
			clvl = (10 * Difficulty) + (2 * Dungeon->level) + (clvl / 3); // HOR 2-56, PRG 20-70, DOM 34-83, CRC 47-96, TRM 60-109, AGN 73-122, TER 86-135, DST 99-148, INF 112-161
			#endif
			//i64 xpGain = i64((0.05 * (monsterLevel - clvl) + 1) * monsterXp);// old way
			i64 xpGain = monsterXp; //  <<<-- new way

			if( xpGain <= 0 ){ // must be signed here
				xpGain = 1;
			}
			i64 newXpValue = xpGain
                + xpGain * player.xpGainPercent / 100
                + player.xpGainMin + Rng32to64(player.xpGainMax - player.xpGainMin);
			if (HasTrait(playerIndex, TraitId::Sisyphean)) {
				newXpValue -= (75 + clvl / 10) * newXpValue / 100;
			}
			else if (player.gameChanger & BIT(GC_25_ACCELERATION) && MaxCountOfPlayersInGame == 1 && is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				newXpValue *= 2;
			}
			if (HasTrait(playerIndex, TraitId::Domesticated) && is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN)) {
				newXpValue += 2 * newXpValue / 10;
			}
			LimitToMin(newXpValue, 1ll);
			xpGain = newXpValue;
			if( MaxCountOfPlayersInGame > 1 ){ // multiplayer
				int level = clvl;
				LimitToRange(level, 0, PlayerMaxLevel());
				if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY/*, GM_CLASSIC*/, GM_COLISEUM)) {
				}
				else if( xpGain >= LevelUp(level) / 20 ){ // XP unsigned patch (2)
					xpGain = LevelUp(level) / 20; //no more than 5% of level progress per kill ?
				}
			}

			// quest map XP boosts
			if( Dungeon->isQuest && Dungeon >= DUN_41_FLESHDOOM && Dungeon <= DUN_43_HEPHASTO  ){
				int fleshDoomLevel = Dungeon - DUN_41_FLESHDOOM; // TODO: add more level type to Dungeons ?
				//1. + 75% / 100% / 125 % to XP
				xpGain += by( fleshDoomLevel, xpGain / 3, xpGain / 2, 3 * xpGain / 4 );
			}
			if (Dungeon->isQuest && Dungeon == DUN_27_THE_INFESTED_CELLAR || Dungeon == DUN_49_SICK_VILLAGERS_BASEMENT) {
				xpGain += xpGain / 5;
			}
			else if (Dungeon->isQuest && (Dungeon == DUN_47_DARK_NEST || Dungeon == DUN_48_BLOOD_HIVE || Dungeon == DUN_55_DEEP_HOLLOW)) {
				xpGain += xpGain / 3;
			}
			else if (Dungeon->isQuest && Dungeon == DUN_54_ABANDONED_FANE) {
				xpGain *= 2;
			}
			else if (
				Dungeon->isQuest && ( (Dungeon >= DUN_25_KING_LEORICS_TOMB && Dungeon <= DUN_39_FROZEN_LOCH)
					|| Dungeon == DUN_50_TEMPLE_OF_SUNSET || Dungeon == DUN_51_FETID_CAVERN	|| Dungeon == DUN_52_MOSSY_GROTTO
					|| Dungeon == DUN_53_VAULTKEEP || Dungeon == DUN_56_WEB_TUNNELS || Dungeon == DUN_57_ARACHNID_LAIR
					|| Dungeon == DUN_58_GATEWAY_OF_BLOOD || Dungeon == DUN_59_HALL_OF_FIRE || Dungeon == DUN_109_HALL_OF_HEROES || Dungeon == DUN_110_BLACK_CHAPEL || Dungeon == DUN_111_CHURCH_SACRISTY
					)
				) { // mordor: all basic quest levels give +25% experience
				xpGain += xpGain / 4;
			}
			// adjustments to AR XP (so it matches normal route)
			else if (Dungeon->isQuest && (Dungeon >= DUN_90_FIELDS_OF_MADNESS && Dungeon <= DUN_108_ARCANE_PRISON)){
				xpGain += xpGain / 10;
			}

			if( LLONG_MAX - player.Xp < xpGain ){ // защита от переполнения
				player.Xp = LLONG_MAX; 
			}else{
				player.Xp += xpGain;
			}

			DrawFloatingExp(xpGain, row, col);

			if( player.Xp < LevelUp(PlayerMaxLevel() - 1) ){ // XP unsigned patch (4)
				int nextLevel = 0;
				while( player.Xp >= LevelUp(nextLevel++) ); // XP unsigned patch (5,6)
				int deltaLevel = nextLevel - player.CharLevel - 1;
				for( int levelUpCount = 0; levelUpCount < deltaLevel; levelUpCount++ ){
					NextPlayerLevel(playerIndex);
				}
				// почему тут в оригинале используется CurrentPlayerIndex ? это ошибка, должен быть playerIndex
				//.text:004585D3 01C A1 E8 AD 6E 00                          mov     eax, CurrentPlayerIndex
				//      000579D3:    8B 45 F4 90 90
                NetSendCmdParam1( 0, CMD_51_PLRLEVEL, player.CharLevel );
			}else{
				player.CharLevel = PlayerMaxLevel();
			}
		}
	}
}

//----- (004585FE) --------------------------------------------------------
void __fastcall AddXpForMonsterKill(int monsterLevel, i64 monsterXp, char attacker, int monsterIndex)
{
	Monster& monster = Monsters[monsterIndex];
	int attackerCount; // edi@1
	int playerIndex; // ecx@1
	int curPlayerIndex; // ecx@7
	unsigned int charLevel; // ebx@12
	unsigned int rnd_100; // eax@12

	attackerCount = 0;
	playerIndex = 0;
	do{
		if( PlayerFriendly() ){
			charLevel = Players[CurrentPlayerIndex].CharLevel;
			rnd_100 = RNG(100);
			if( rnd_100 < (charLevel + 10) || is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_EASY/*, GM_CLASSIC*/) ){
				curPlayerIndex = CurrentPlayerIndex;
				AddPlayerExperience( curPlayerIndex, monsterXp, monster.Row, monster.Col );
				return;
			}
		}
		if( (1 << playerIndex) & attacker ){
			++attackerCount;
		}
		++playerIndex;
	}while( playerIndex < 4 );
	if( attackerCount ){
		curPlayerIndex = CurrentPlayerIndex;
		if( (1 << CurrentPlayerIndex) & attacker ){
			AddPlayerExperience( curPlayerIndex, monsterXp / attackerCount, monster.Row, monster.Col );
			return;
		}
	}
}

//----- (00458642) --------------------------------------------------------
void __fastcall InitPlayer( uint playerIndex, int loadFromIngameMenu )
{
	if( playerIndex >= 4 ){
		TerminateWithError( "InitPlayer: illegal player %d", playerIndex );
	}
	Player& player = Players[ playerIndex ];

	ClearAuras( player );
	unsigned int orient = 0;
	if( loadFromIngameMenu ){
		LOBYTE_IDA( player.SpellType ) = 4;
		player.CurrentSpellIndex = -1;
		player.spellIndex = player.CurrentSpellIndex;
		player.readySpellType = player.SpellType;

		const Item& leftHand = player.OnBodySlots[IS_LeftHand];
		const Item& rightHand = player.OnBodySlots[IS_RightHand];
		player.IsBowInHand = ( is( leftHand.ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL ) && leftHand.IsReqMet ) || ( is( rightHand.ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL ) && rightHand.IsReqMet );
		bool flaskInLeftHand = leftHand.IsReqMet && ( leftHand.ItemCode == IC_18_FLASK || leftHand.ItemCode == IC_19_TRAP);
		bool flaskInRightHand = rightHand.IsReqMet && ( rightHand.ItemCode == IC_18_FLASK || rightHand.ItemCode == IC_19_TRAP);
		player.IsFlaskInHand = flaskInLeftHand  || flaskInRightHand;
	}
	if( player.dungeon == Dungeon || DungeonDebug ){
		SetPlrAnims( playerIndex );
		player.xOfs = 0;
		player.yOfs = 0;
		player.xVel = 0;
		player.yVel = 0;
		ClearPlrPVars( playerIndex );
		if( (player.CurLife & ~63) <= 0 ){
			player.CurAction = PCA_8_DEATH;
			NewPlayerAnimation(
				playerIndex,
				player.deathAnimationsPointers[ 0 ],
				player.DeathAnimationFrameCount,
				1,
				player.AnimPitchDeath );
			player.currentAnimationFrame = player.currentAnimationFramesCount - 1;
			player.currentActionFrame = 2 * player.currentAnimationFramesCount;
		}else{
			player.CurAction = PCA_0_STAND;
			NewPlayerAnimation(
				playerIndex,
				player.nothingAnimationsPointers[ 0 ],
				player.StayAnimationFrameCount,
				3,
				player.AnimPitchStay );
			player.currentAnimationFrame = RNG(player.StayAnimationFrameCount - 1) + 1; // TODO: check for rand need
			player.currentAnimationFrameTick = RNG(3); // TODO: check for rand need
		}
		player.dir = 0;
		player.UnderArcType = 0;
		if( playerIndex == CurrentPlayerIndex ){
			if( !loadFromIngameMenu || Dungeon != DUN_0_TOWN ){
				//PlayerRowPos = 57; // test dungeon 16 mobs generation
				//PlayerColPos = 77;
				player.Row = PlayerRowPos;
				player.Col = PlayerColPos;
			}
			player.TargetRow = player.Row;
			player.TargetCol = player.Col;
		}else{
			player.TargetRow = player.Row;
			player.TargetCol = player.Col;
			do{
				if( CellFreeForPlayer(
					playerIndex,
					player.Row + PlrXoff2[ orient ],
					player.Col + PlrYoff2[ orient ] ) ){
					break;
				}
				++orient;
			}while( orient < 8 );
			player.Row += PlrXoff2[ orient ];
			player.Col += PlrYoff2[ orient ];
		}
		player.NextRow = player.Row;
		player.NextCol = player.Col;
		player.walkPath[0] = PCMD_M1_NOTHING;
		player.destAction = PCMD_M1_NOTHING;
		if( playerIndex == CurrentPlayerIndex ){
			player.LightIndex = AddLight( player.Row, player.Col, player.LightRadius );
		}else{
			player.LightIndex = -1;
		}
		player.lightRadiusIdentificator = AddVision(
			player.Row,
			player.Col,
			player.LightRadius,
			playerIndex == CurrentPlayerIndex );
	}
	
	
	const PlayerClass& playerClass = PlayerClasses[player.ClassID];
	if( player.fullClassId == PFC_IRON_MAIDEN || HasTrait(playerIndex, TraitId::Mamluk)){// Quick and dirty hack to override IM skill
	    player.AvailableSkillMask = BIT(PS_26_ITEM_REPAIR);
	}
	else if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		player.AvailableSkillMask = BIT(PS_9_INFRAVISION);
	}
	else if (HasTrait(playerIndex, TraitId::Hydramancer)) {
		player.AvailableSkillMask = BIT(PS_13_HYDRA);
	}
	else if (HasTrait(playerIndex, TraitId::Paladin)) {
		player.AvailableSkillMask = BIT(PS_11_MANA_SHIELD);
	}
	/*else if (player.fullClassId == PFC_ARCHER && GameMode == GM_CLASSIC) {
		player.AvailableSkillMask = BIT(PS_28_TRAP_DISARM);
	}*/
	else{
        player.AvailableSkillMask = BIT(playerClass.SkillIndex);
    }
	if( is( player.fullClassId, PFC_NECROMANCER, PFC_DEMONOLOGIST, PFC_BEASTMASTER) ){
        player.AvailableSkillMask |= BIT(PS_55_UNSUMMON);
    }
    
	player.NextLevelUp = LevelUp( player.CharLevel );
	player.notHittableInTransition = 0;
	player.notHittableFramesAfterTransition = 0;
	if(/* MaxCountOfPlayersInGame == 1 &&*/ is(GameMode, GM_IRONMAN, GM_HARDCORE, GM_NIGHTMARE, GM_SPEEDRUN, GM_COLISEUM) ){
		player.notHittableFramesAfterTransition = ENGINE_FPS * 4; // 3 seconds invincible after transition
	}
	if( playerIndex == CurrentPlayerIndex ){
		DeathDelayTimer = 0;
		IsPlayerDead = 0;
		Scroll.xOfs = 0;
		Scroll.yOfs = 0;
		Scroll.dir = 0;
		SetupInvPanel(playerIndex);
	}
}

//----- (00458A42) --------------------------------------------------------
void InitMultiView()
{
	if( (uint)CurrentPlayerIndex >= PlayersMax_4 ){
		TerminateWithError("InitPlayer: illegal player %d", CurrentPlayerIndex);
	}
	PlayerRowPos = Players[CurrentPlayerIndex].Row;
	PlayerColPos = Players[CurrentPlayerIndex].Col;
}

//----- (00458A7E) --------------------------------------------------------
void __fastcall CheckPlayerUnderArch(int playerIndex, int flag2)
{
	ushort* tileMapLeft;
	int col = 0;
	int row = 0;
	int mask = 0;
	Player& player = Players[playerIndex];

	if( (uint)playerIndex >= PlayersMax_4 ){
		TerminateWithError("InitPlayer: illegal player %d", playerIndex);
	}

	row = player.Row - 1;
	col = player.Col + 1;
	if( To112(row, col) ){
		tileMapLeft = &TileMap[IsometricMapOffset(row, col)].Layer[1].Left;
		mask = 0;
		for( int i = 8; i > 0; --i){
			mask |= *tileMapLeft;
			tileMapLeft++;// Увеличиваем указатель
		}

		if( mask | ArchGraphicsMap[ row ][ col ] | TileBlockWalking[FineMap[ row ][ col ]] ){
			player.UnderArcType = 1;
		}else{
			player.UnderArcType = 0;
		}
	}else{
		player.UnderArcType = 0;
	}
	if( flag2 != 1 || player.UnderArcType != 1 ){
		return;
	}
	
	row = player.Row;
	col = player.Col + 2;
	if( To112(row, col) ){
		tileMapLeft = &TileMap[ IsometricMapOffset( row, col ) ].Layer[ 1 ].Left;
		mask = 0;
		for( int i = 8; i > 0; i--){
			mask |= *tileMapLeft;
			tileMapLeft++;// Увеличиваем указатель
		}

		if( !(mask | ArchGraphicsMap[ row ][ col ]) ){
			row = player.Row - 2;
			col = player.Col + 1;
			if( To112(row, col) ){
				tileMapLeft = &TileMap[ IsometricMapOffset( row, col ) ].Layer[ 1 ].Left;
				mask = 0;
				for( int i = 8; i > 0; i--){
					mask |= *tileMapLeft;
					tileMapLeft++;// Увеличиваем указатель
				}

				if( mask | ArchGraphicsMap[ row ][ col ]){
					player.UnderArcType = 2;
				}
			}
		}
	}
}

//----- (00458BDB) --------------------------------------------------------
int __fastcall IsCellBlocking(int row, int col, uchar canFly /*= 0*/ )
{
	#ifdef SECURE_CELL_BLOCKING
	// вылетало при cellY = -1 (вызывает sub_444AFF, sub_455D2C)
	if( ! In112(row, col) ){
		return 1; // Если вышел монстр за границу карты выходим
	}
	#endif
	return TileBlockWalking[FineMap[ row ][ col ]] & ++canFly; // fly flag to walk type
}

//----- (00458BEF) --------------------------------------------------------
int __fastcall IsDirectionClearForPlayer(int playerIndex, int direction)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PlrDirOK: illegal player %d", playerIndex);
	}
	int colDelta = ColDelta[direction];
	int rowDelta = RowDelta[direction];
	int nextCol = player.Col + colDelta;
	int nextRow = player.Row + rowDelta;
	int cell = 112 * nextRow + nextCol;// player.MayBeImmortalFlag
	if( ! To112(nextRow, nextCol) || !FineMap[ 0 ][ cell ] || !CellFreeForPlayer(playerIndex, nextRow, nextCol) ){
		return 0;
	}
	int result = 1;
	if( direction == 6 ){
		result = !IsCellBlocking(nextRow, nextCol + 1) && !(FlagMap[ 0 ][ cell + 1 ] & CF_32_PLAYER_HORIZONTAL);
	}
	if( result && direction == 2 ){
		if( !IsCellBlocking(nextRow + 1, nextCol) && !(FlagMap[ 1 ][ cell ] & CF_32_PLAYER_HORIZONTAL) ){
			return 1;
		}
		return 0;
	}
	return result;
}

//----- (00458CB0) --------------------------------------------------------
void __fastcall SomeWithTeleportAndTransparent2( int row, int col )
{
	for( int curCol = col-1; curCol <= col+1; curCol++ ){
		for( int curRow = row-1; curRow <= row+1; curRow++ ){
			TransList[TransMap[ curRow ][ curCol ]] = 0;
		}
	}
}

//----- (00458CF3) --------------------------------------------------------
void __fastcall SomeWithTeleportAndTransparent( int row, int col )
{
	if( Dungeon->genType == DT_1_CHURCH || Dungeon->genType == DT_2_CATACOMB ){
		for( int curCol = col-1; curCol <= col+1; ++curCol ){
			for( int curRow = row-1; curRow <= row+1; ++curRow ){
				uchar roomIndex = TransMap[ curRow ][ curCol ];
				if( !TileBlockWalking[ FineMap[ curRow ][ curCol ] ] && roomIndex ){
					TransList[roomIndex] = 1; // в th1 идет обращение по signed char индексу, это явный баг
				}
			}
		}
	}else{
		TransList[1] = 1;
	}
}

//----- (00458D74) --------------------------------------------------------
void __fastcall SetPlayerOld( int playerIndex )
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("SetPlayerOld: illegal player %d", playerIndex);
	}
	player.oldRow = player.Row;
	player.oldCol = player.Col;
}

//----- (00458DB0) --------------------------------------------------------
void __fastcall FixPlayerLocation(int playerIndex, int orient, bool stunWalkDown /*=false*/ )
{
	if( playerIndex >= 4 ){
		TerminateWithError("FixPlayerLocation: illegal player %d", playerIndex);
	}

	Player& player = Players[playerIndex];
	// fix HorizontalHit
	int row = player.Row; int col = player.Col;
	int playerNum = -playerIndex - 1;
	if( HorizontalHit ){
		if( row > 0 && col < FineMap_112 - 1 && PlayerMap[ row - 1 ][ col + 1 ] == playerNum ){
			PlayerMap[ row - 1 ][ col + 1 ] = 0;
		}
		if( row < FineMap_112 - 1 && col > 0 && PlayerMap[ row + 1 ][ col - 1 ] == playerNum ){
			PlayerMap[ row + 1 ][ col - 1 ] = 0;
		}
		PlayerMap[ row ][ col ] = playerIndex + 1;
		FlagMap[ player.HorizontalNextRowOrSomeSpellData ][ player.HorizontalNextCol ] &= ~CF_32_PLAYER_HORIZONTAL;
		//CheckPlayerMap(); // вроде старая проверка для разбора функций, можно удалить
	}
	if( stunWalkDown ){
		PlayerMap[player.Row][player.Col] = 0;
		player.Row = player.CursorRow;
		player.Col = player.CursorCol;
	}
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.TargetRow = player.Row;
	player.TargetCol = player.Col;
	player.xOfs = 0;
	player.yOfs = 0;
	CheckPlayerUnderArch(playerIndex, 0);
	player.dir = orient;
	if( playerIndex == CurrentPlayerIndex ){
		Scroll.xOfs = 0;
		Scroll.yOfs = 0;
		Scroll.dir = 0;
		PlayerRowPos = player.Row; // вторичное изменение координат игрока при ходьбе 
		PlayerColPos = player.Col;
	}
}

//----- (00458E64) --------------------------------------------------------
void __fastcall StartStand(int playerIndex, int direction)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartStand: illegal player %d", playerIndex);
	}

	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}

	if( !(player.LoadPlayerAnimationMask & PAF_1_STAY) ){
		LoadPlayerGFX(playerIndex, PAF_1_STAY);
	}

	NewPlayerAnimation( playerIndex, player.nothingAnimationsPointers[direction], player.StayAnimationFrameCount, 3, player.AnimPitchStay);
	
	player.CurAction = PCA_0_STAND;
	FixPlayerLocation(playerIndex, direction);
	FixPlrWalkTags(playerIndex);
	PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
	SetPlayerOld(playerIndex);
}

//----- (00458F31) --------------------------------------------------------
void __fastcall StartWalkStand(int playerIndex)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("StartWalkStand: illegal player %d", playerIndex);
	}

	player.CurAction = PCA_0_STAND;
	player.NextRow = player.Row;
	player.NextCol = player.Col;
	player.xOfs = 0;
	player.yOfs = 0;

	CheckPlayerUnderArch(playerIndex, 0);

	if( playerIndex == CurrentPlayerIndex ){
		Scroll.xOfs = 0;
		Scroll.yOfs = 0;
		Scroll.dir = 0;
		PlayerRowPos = player.Row;
		PlayerColPos = player.Col;
	}
}

// Player Message
//----- (00458FC5) --------------------------------------------------------
void __fastcall PM_ChangeLightOff(int playerIndex)
{
	int v2;
	int v3;
	int v5;
	int v6;
	int v7;
	int v8;
	int v10;
	int v11;
	int v12;
	int v13;
	uint v14;
	int v15;
	int v16;
	int v17;
	int deltaCol;
	int deltaRow;
	int v20;
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PM_ChangeLightOff: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.LightIndex < 0 ) return; // LightIndex th1 bug (2)
	Light& light = LightList[player.LightIndex];
	v10 = player.xOfs;
	v11 = 2 * player.yOfs;
	v2 = v10 + v11;
	v3 = v11 - v10;
	if( v2 >= 0 ){
		v5 = 1;
	}else{
		v5 = -1;
		v2 = -v2;
	}
	if( v3 >= 0 ){
		v6 = 1;
	}else{
		v6 = -1;
		v3 = -v3;
	}
	v12 = v6 * (v3 >> 3);
	v13 = v5 * (v2 >> 3);
	v14 = light.deltaCol;
	v15 = v12;
	v16 = light.col;
	deltaCol = v15;
	v16 *= 8;
	deltaRow = v13;
	v7 = v16 + v15;
	v8 = v16 + v14;
	v17 = v13 - light.deltaRow;
	v20 = 8 * light.row;
	if( abs(v17) >= 3 || abs(v7 - v8) >= 3 ){
		ChangeLightOffset(player.LightIndex, deltaRow, deltaCol);
	}
}

//----- (th2) -------------------------------------------------------------
bool Running()
{
	return /*MaxCountOfPlayersInGame == 1 && */(IsFastWalkEnabled && ( !(Dungeon->genType || Dungeon->isQuest) || ( RunInDungeon && !IsSomeMonstersActivated() )) || BattleTrance );
}

//----- (0045908A) --------------------------------------------------------
void __fastcall PM_ChangeOffset(int playerIndex)
{
	Player& player = Players[playerIndex];

	if( playerIndex >= 4 ){
		TerminateWithError("PM_ChangeOffset: illegal player %d", playerIndex);
	}
	player.currentActionFrame++;
	int oldDeltaY = player.dx64 >> 8;
	int oldDeltaX = player.dy64 >> 8;

	int multiplier = player.running ? 2 : 1; // Player run

	player.dx64 += multiplier * player.xVel;
	player.dy64 += multiplier * player.yVel;

	player.xOfs = player.dx64 >> 8;
	player.yOfs = player.dy64 >> 8;

	if( playerIndex == CurrentPlayerIndex && Scroll.dir ){
		Scroll.xOfs += oldDeltaY - player.xOfs;
		Scroll.yOfs += oldDeltaX - player.yOfs;
	}

	PM_ChangeLightOff(playerIndex);
}

//----- (0045919A) --------------------------------------------------------
void __fastcall StartWalkUp(int playerIndex, int xVel, int yVel, int changeRow, int changeCol, int direction, int scrollDir )
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= PlayersMax_4 ) TerminateWithError("StartWalk: illegal player %d", playerIndex);
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}
	SetPlayerOld(playerIndex);
	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}
	player.NextRow = changeRow + player.Row;
	player.NextCol = changeCol + player.Col;
	if( playerIndex == CurrentPlayerIndex ){
		Scroll.dx = player.Row - PlayerRowPos;
		Scroll.dy = player.Col - PlayerColPos;
	}
	PlayerMap[ player.NextRow ][ player.NextCol ] = -1 - playerIndex;
	player.CurAction = PCA_1_WALK_UP;
	player.running = Running();
	player.xVel = xVel;
	player.yVel = yVel;
	player.xOfs = 0;
	player.yOfs = 0;
	player.CursorRow = changeRow;
	player.CursorCol = changeCol;
	player.newDirection = direction;
	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}
	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);
	player.dir = direction;
	player.dx64 = 0;
	player.dy64 = 0;
	player.currentActionFrame = 0;
	CheckPlayerUnderArch(playerIndex, 0);
	if( playerIndex == CurrentPlayerIndex ){
		int minScrollDelta = IsZoomDisable ? 3 : 2;
		Scroll.dir = SDIR_0_NONE;
		if( abs(Scroll.dx) < minScrollDelta && abs(Scroll.dy) < minScrollDelta ){
			Scroll.dir = scrollDir;
		}
	}
}

//----- (004593B4) --------------------------------------------------------
void __fastcall StartWalkDown( int playerIndex, int xVel, int yVel, int xOfs, int yOfs, int changeRow, int changeCol, int direction, int scrollDir )
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ) TerminateWithError("StartWalk2: illegal player %d", playerIndex);
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}
	SetPlayerOld(playerIndex);
	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}
	player.NextRow = changeRow + player.Row;;
	player.NextCol = changeCol + player.Col;
	if( playerIndex == CurrentPlayerIndex ){
		Scroll.dx = player.Row - PlayerRowPos;
		Scroll.dy = player.Col - PlayerColPos;
	}
	PlayerMap[ player.Row ][ player.Col ] = -playerIndex - 1;
	player.CursorRow = player.Row;
	player.CursorCol = player.Col;
	player.Row = player.NextRow;
	player.Col = player.NextCol;
	PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
	player.xOfs = xOfs;
	player.yOfs = yOfs;
	ChangeLightPos(player.LightIndex, player.Row, player.Col);
	PM_ChangeLightOff(playerIndex);
	player.CurAction = PCA_2_WALK_DOWN;
	player.running = Running();
	player.xVel = xVel;
	player.yVel = yVel;
	player.dx64 = xOfs << 8;
	player.dy64 = yOfs << 8;
	player.newDirection = direction;
	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}
	NewPlayerAnimation( playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);
	player.dir = direction;
	player.currentActionFrame = 0;
	CheckPlayerUnderArch(playerIndex, direction == 7);
	if( playerIndex == CurrentPlayerIndex ){
		int minScrollDelta = IsZoomDisable ? 3 : 2;
		Scroll.dir = SDIR_0_NONE;
		if( abs(Scroll.dx) < minScrollDelta && abs(Scroll.dy) < minScrollDelta ){
			Scroll.dir = scrollDir;
		}
	}
}

//----- (00459638) --------------------------------------------------------
void __fastcall StartWalkHorizontal( int playerIndex, int xVel, int yVel, int xOfs, int yOfs, int changeRow, int changeCol, int changeRow2, int changeCol2, int direction, int scrollDir )
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ) TerminateWithError("StartWalk3: illegal player %d", playerIndex);
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}
	SetPlayerOld(playerIndex);
	if( !IsDirectionClearForPlayer(playerIndex, direction) ){
		return;
	}
	player.NextRow = changeRow + player.Row;
	player.NextCol = changeCol + player.Col;
	if( playerIndex == CurrentPlayerIndex ){
		Scroll.dx = player.Row - PlayerRowPos;
		Scroll.dy = player.Col - PlayerColPos;
	}
	PlayerMap[ player.Row ][ player.Col ] = -1 - playerIndex;
	PlayerMap[ player.NextRow ][ player.NextCol ] = -1 - playerIndex;
	player.HorizontalNextRowOrSomeSpellData = changeRow2 + player.Row;
	player.HorizontalNextCol = changeCol2 + player.Col;
	FlagMap[ changeRow2 + player.Row ][ changeCol2 + player.Col ] |= CF_32_PLAYER_HORIZONTAL; 
	player.xOfs = xOfs;
	player.yOfs = yOfs;
	if( Dungeon->genType || Dungeon->isQuest ){
		ChangeLightPos(player.LightIndex, player.HorizontalNextRowOrSomeSpellData, player.HorizontalNextCol);
		PM_ChangeLightOff(playerIndex);
	}
	player.CurAction = PCA_3_WALK_HORISONTAL;
	player.running = Running();
	player.xVel = xVel;
	player.yVel = yVel;
	player.CursorRow = player.NextRow;
	player.CursorCol = player.NextCol;
	player.dx64 = xOfs << 8;
	player.dy64 = yOfs << 8;
	player.newDirection = direction;
	if( !(player.LoadPlayerAnimationMask & PAF_2_WALK) ){
		LoadPlayerGFX(playerIndex, PAF_2_WALK);
	}
	NewPlayerAnimation(playerIndex, player.walkAnimationsPointers[direction], player.WalkAnimationFrameCount, 0, player.AnimPitchWalk);
	player.dir = direction;
	player.currentActionFrame = 0;
	CheckPlayerUnderArch(playerIndex, 0);
	if( playerIndex == CurrentPlayerIndex ){
		int minScrollDelta = IsZoomDisable ? 3 : 2;
		Scroll.dir = SDIR_0_NONE;
		if( abs(Scroll.dx) < minScrollDelta && abs(Scroll.dy) < minScrollDelta ){
			Scroll.dir = scrollDir;
		}
	}
}

//----- (00777CD0) -------------------------------------------------------
int __fastcall GetAnimationDelay(uint playerIndex)
{
	int animationDelay = 0;
	Player& player = Players[ playerIndex ];
	//DualLeftHand = ! DualLeftHand;
	Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
	Item& rightHand= player.OnBodySlots[ IS_RightHand ];
	if( leftHand.ItemCode != IC_M1_NONE && leftHand.TypeID == ITEM_1_WEAPON ){
		animationDelay = BaseItems[ leftHand.baseItemIndex ].AnimationDelay;
		if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON && RNG(2) ){
			animationDelay = BaseItems[ rightHand.baseItemIndex ].AnimationDelay;
		}
	}else if( rightHand.ItemCode != IC_M1_NONE && rightHand.TypeID == ITEM_1_WEAPON ){
		animationDelay = BaseItems[ rightHand.baseItemIndex ].AnimationDelay;
	}
	return animationDelay;
}

//----- (004598EF) --------------------------------------------------------
void __fastcall StartAttack(int playerIndex, int orientationToTarget)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartAttack: illegal player %d", playerIndex);
	}
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}
	if( !(player.LoadPlayerAnimationMask & PAF_3_ATTACK) ){
		LoadPlayerGFX(playerIndex, PAF_3_ATTACK);
	}
	NewPlayerAnimation(playerIndex, player.attackAnimationsPointers[orientationToTarget], player.AttackAnimationFrameCount, GetAnimationDelay(playerIndex), player.AnimPitchAttack);
	player.CurAction = PCA_4_ATTACK;
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
}

//----- (0045999C) --------------------------------------------------------
void __fastcall StartRangeAttack(int playerIndex, int orientationToTarget, int targetRow, int targetCol)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartRangeAttack: illegal player %d", playerIndex);
	}
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}
	if( !(player.LoadPlayerAnimationMask & PAF_3_ATTACK) ){
		LoadPlayerGFX(playerIndex, PAF_3_ATTACK);
	}
	// тут можно добавить проверку на арбалеты, и задавать Frame Delay = 1 для замедления атаки в два раза, или = 3 для замедления в 4 раза
	// при 3, например, будет три кадра пропускаться, один выполняться, 25% скорости
	NewPlayerAnimation(playerIndex, player.attackAnimationsPointers[orientationToTarget], player.AttackAnimationFrameCount, GetAnimationDelay(playerIndex), player.AnimPitchAttack);
	player.CurAction = PCA_5_RANGE_ATTACK;
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
	player.CursorRow = targetRow;
	player.CursorCol = targetCol;
}

//----- (00459A72) --------------------------------------------------------
void __fastcall StartPlayerBlock( uint playerIndex, int orientation )
{
	if( playerIndex >= 4 ) TerminateWithError("StartPlrBlock: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	if ( ! player.notHittableInTransition || player.CurLife || playerIndex != CurrentPlayerIndex) {
		int BLsound = 0;
		// TODO: change to switch
		if( is( player.fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_TEMPLAR, PFC_TRAPPER, PFC_NECROMANCER,
		 PFC_SHINOBI, PFC_IRON_MAIDEN, PFC_BOMBARDIER, PFC_THRAEX, PFC_MURMILLO, PFC_SECUTOR) ){
			switch (RNG(27)) {
			case 26: BLsound = S_1219; break;
			case 25: BLsound = S_1220; break;
			case 24: BLsound = S_1221; break;
			case 23: BLsound = S_1222; break;
			case 22: BLsound = S_1223; break;
			case 21: BLsound = S_1185; break;
			case 20: BLsound = S_1186; break;
			case 19: BLsound = S_1187; break;
			case 18: BLsound = S_1188; break;
			case 17: BLsound = S_1189; break;
			case 16: BLsound = S_1190; break;
			case 15: BLsound = S_1191; break;
			case 14: BLsound = S_1224; break;
			case 13: BLsound = S_1225; break;
			case 12: BLsound = S_1226; break;
			case 11: BLsound = S_1227; break;
			case 10: BLsound = S_1228; break;
			case  9: BLsound = S_1229; break;
			case  8: BLsound = S_1230; break;
			case  7: BLsound = S_1231; break;
			case  6: BLsound = S_1232; break;
			case  5: BLsound = S_1233; break;
			case  4: BLsound = S_1234; break;
			case  3: BLsound = S_1235; break;
			case  2: BLsound = S_1236; break;
			case  1: BLsound = S_1237; break;
			default: BLsound = S_67_I_INVSWORD; break;
			}
		}else if( is( player.fullClassId, PFC_GUARDIAN, PFC_SAVAGE, PFC_KENSEI, PFC_SHUGOKI, PFC_DIMACHAERUS, PFC_ROGUE, PFC_DRUID) ){
			switch (RNG(27)) {
			case 26: BLsound = S_1238; break; //s
			case 25: BLsound = S_1239; break; //s
			case 24: BLsound = S_1240; break; //s
			case 23: BLsound = S_1241; break; //s
			case 22: BLsound = S_1242; break;
			case 21: BLsound = S_1243; break;
			case 20: BLsound = S_1244; break;
			case 19: BLsound = S_1245; break;
			case 18: BLsound = S_1246; break;
			case 17: BLsound = S_1247; break;
			case 16: BLsound = S_1248; break;
			case 15: BLsound = S_1249; break;
			case 14: BLsound = S_1250; break;
			case 13: BLsound = S_1251; break;
			case 12: BLsound = S_1252; break;
			case 11: BLsound = S_1253; break;
			case 10: BLsound = S_1254; break;
			case  9: BLsound = S_1255; break;
			case  8: BLsound = S_1256; break;
			case  7: BLsound = S_1257; break;
			case  6: BLsound = S_1258; break;
			case  5: BLsound = S_1259; break;
			case  4: BLsound = S_1260; break;
			case  3: BLsound = S_1261; break;
			case  2: BLsound = S_1262; break;
			case  1: BLsound = S_1263; break;
			default: BLsound = S_1264; break;
			}
		}else if( player.fullClassId == PFC_MONK ){
			switch (RNG(8) ){
			case 7:  BLsound = S_1265; break; //s
			case 6:  BLsound = S_1266; break; //s
			case 5:  BLsound = S_1267; break; //s
			case 4:  BLsound = S_1268; break; //s
			case 3:  BLsound = S_1269; break;
			case 2:  BLsound = S_1270; break;
			case 1:  BLsound = S_1271; break;
			default: BLsound = S_1272; break;
			}
		}else{
			switch( RNG(13) ){
			case 12: BLsound = S_1193; break; //s
			case 11: BLsound = S_1192; break; //s
			case 10: BLsound = S_1191; break; //s
			case  9: BLsound = S_1190; break; //s
			case  8: BLsound = S_1189; break;
			case  7: BLsound = S_1188; break;
			case  6: BLsound = S_1187; break;
			case  5: BLsound = S_1186; break;
			case  4: BLsound = S_1185; break;
			case  3: BLsound = S_1152; break;
			case  2: BLsound = S_1153; break;
			case  1: BLsound = S_1154; break;
			default: BLsound = S_67_I_INVSWORD; break;
			}
		}
		PlayLocalSound( BLsound, player.Row, player.Col );
		if( !( player.LoadPlayerAnimationMask & PAF_9_BLOCK ) ){
			LoadPlayerGFX(playerIndex, PAF_9_BLOCK);
		}
		NewPlayerAnimation( playerIndex, player.blockAnimationsPointers[orientation], player.BlockAnimationFrameCount, 2, player.AnimPitchBlock );
		player.CurAction = PCA_6_BLOCK;
		FixPlayerLocation(playerIndex, orientation);
		SetPlayerOld(playerIndex);
	}else{
		TryToDie(playerIndex, -1);
	}
}

//Dragon старт каста, проверить в отладчике где вызывается для шилда
//----- (00459B35) --------------------------------------------------------
void __fastcall StartSpell(int playerIndex, int orientationToTarget, int targetRow, int targetCol)
{
	// TODO: check spellIndex == -1
	//int spellIndex = MissileIndexes[MissileAmount-1]; // spellIndex points to the last missile cast
	//Missile& spell = Missiles[spellIndex]; // spell is the very last missile cast
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("StartSpell: illegal player %d", playerIndex);
	}
	if( playerIndex == CurrentPlayerIndex ){
		RButtonEvent = false;
	}
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
	}
	#ifdef FLASH_FIX // turn off buggy flash fix for a while, TODO: check other spells (firebolt, etc) and hotkeys broke, re-fix over fixing spell queue globally 
	if (player.spellIndex == PS_4_FLASH && ( spell.BaseMissileIndex == MI_11_FLASH_FR || spell.BaseMissileIndex == MI_12_FLASH_BACK ) && player.currentAnimationFramesCount < 20 ) { // Checks if player tries to cast Flash while previous Flash is in progress
		return; // Goes out
	}
	#endif

	if( SpellAnimInTown || Dungeon->genType || Dungeon->isQuest ){
		char animType = Spells[player.spellIndex].animType;
		switch( player.fullClassId ){
		case PFC_ELEMENTALIST: if( player.spellIndex == PS_13_HYDRA ) animType = SA_2_MAGIC; break; //magic animation for ice hydra
		case PFC_DRUID       : if( player.spellIndex == PS_22_FURY  ) animType = SA_3_TURN;  break; //magic animation for druid transformation
		}
		switch( animType ){
		case SA_0_FIRE : if( !(player.LoadPlayerAnimationMask & PAF_6_FIRE_CAST ) ) LoadPlayerGFX(playerIndex, PAF_6_FIRE_CAST); // TODO: move flag check into func
			NewPlayerAnimation(playerIndex, player.fireSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
			break;
		case SA_1_LIGHT: if( !(player.LoadPlayerAnimationMask & PAF_5_LIGHT_CAST) ) LoadPlayerGFX(playerIndex, PAF_5_LIGHT_CAST);
			NewPlayerAnimation(playerIndex, player.lightningSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
			break;
		case SA_2_MAGIC: if( !(player.LoadPlayerAnimationMask & PAF_7_MAGIC_CAST) ) LoadPlayerGFX(playerIndex, PAF_7_MAGIC_CAST);
			NewPlayerAnimation(playerIndex, player.magicSpellAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
			break;
		case SA_3_TURN : if( !(player.LoadPlayerAnimationMask & PAF_10_TURNING  ) ) LoadPlayerGFX(playerIndex, PAF_10_TURNING);
			NewPlayerAnimation(playerIndex, player.turningAnimationsPointers[orientationToTarget], player.SpellAnimationFrameCount, 0, player.AnimPitchCast);
			break;
		}
	}
	PlayLocalSound(Spells[player.spellIndex].SoundType, player.Row, player.Col);
	player.CurAction = PCA_9_SPELL; //dragon устанавливается режим кастования
	FixPlayerLocation(playerIndex, orientationToTarget);
	SetPlayerOld(playerIndex);
	player.CursorRow = targetRow;
	player.CursorCol = targetCol;
	player.HorizontalNextRowOrSomeSpellData = PlayerSpellLevel(playerIndex, player.spellIndex); // spell level calc at next command
	player.currentActionFrame = 1;
}

//----- (00459D00) --------------------------------------------------------
void __fastcall FixPlrWalkTags(int playerIndex)
{
	int multiPlayerIndex; // edx@3
	Player *player; // eax@3
	int oldXPos; // ecx@3
	int oldYPos; // eax@3
	int col; // esi@3
	int lastRow; // edi@4
	int row; // ebx@5
	int playerMapNum; // edi@11
	int playerNum; // [sp+Ch] [bp-Ch]@3
	int row_1; // [sp+10h] [bp-8h]@5
	char *playerMap; // [sp+14h] [bp-4h]@6

	if( (unsigned int) playerIndex >= 4 ){
		TerminateWithError( "FixPlrWalkTags: illegal player %d", playerIndex );
	}
	multiPlayerIndex = -1 - playerIndex;
	playerNum = playerIndex + 1;
	player = &Players[ playerIndex ];
	oldXPos = player->oldRow;
	oldYPos = player->oldCol;
	col = oldYPos - 1;
	if( oldYPos - 1 <= oldYPos + 1 ){ // for( int i = oldYPos - 1; i < oldYPos + 1; ++i )
		lastRow = oldXPos + 1;
		do{
			row = oldXPos - 1;
			row_1 = oldXPos - 1;
			if( oldXPos - 1 <= lastRow ){
				playerMap = &PlayerMap[ row ][ col ];
				do{
					if( In112(row, col) ){
						playerMapNum = *playerMap;
						if( playerMapNum == playerNum || playerMapNum == multiPlayerIndex ){
							*playerMap = 0;
						}
					}
					playerMap += 112;
					row = row_1 + 1;
					lastRow = oldXPos + 1;
					++row_1;
				}while( row_1 <= oldXPos + 1 );
			}
			++col;
		}while( col <= oldYPos + 1 );
	}
	if( (uint)oldXPos < FineMap_112 - 1 && (uint)oldYPos < FineMap_112 - 1 ){
		FlagMap[ oldXPos + 1 ][ oldYPos ] &= ~CF_32_PLAYER_HORIZONTAL;
		FlagMap[ oldXPos ][ oldYPos + 1 ] &= ~CF_32_PLAYER_HORIZONTAL;
	}
}

//----- (00459DCD) --------------------------------------------------------
char* __fastcall RemovePlayerFromMap(int playerIndex)
{
	int v1;             // edi@1
	int v2;             // esi@1
	int v3;             // ecx@2
	int v4;             // ebx@2
	unsigned __int8 v5; // al@5
	int v6;             // edx@9
	char* result;       // eax@10
	int v8;             // edi@10
	int v9;             // ecx@11
	int v10;            // [sp+Ch] [bp-4h]@1
	v10 = playerIndex + 1;
	v2 = -1 - playerIndex;
	v1 = 1;
	do{
		v3 = v1;
		v4 = 111;
		do{
			if( PlayerMap[ 0 ][ v3 + 111 ] == v2 || PlayerMap[ 0 ][ v3 ] == v2 ){
				v5 = FlagMap[ 0 ][ v3 + 112 ];
				if( v5 & CF_32_PLAYER_HORIZONTAL )
					FlagMap[ 0 ][ v3 + 112 ] = v5 & ~(CF_32_PLAYER_HORIZONTAL);
			}
			v3 += 112;
			--v4;
		}while( v4 );
		++v1;
	}while( v1 < 112 );
	v6 = 0;
	do{
		result = &PlayerMap[ 0 ][ v6 ];
		v8 = 112;
		do{
			v9 = *result;
			if( v9 == v10 || v9 == v2 )
				*result = 0;
			result += 112;
			--v8;
		}while( v8 );
		++v6;
	}while( v6 < 112 );
	return result;
}

//----- (00459E48) --------------------------------------------------------
void __fastcall StartPlayerHit( int playerIndex, int damage, int needStun )
{
	if( IsGodMode ){ return; }
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("StartPlrHit: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex ){
		TryToDie(playerIndex, -1);
		return;
	}
	int soundIndex = -1;
	switch (player.fullClassId ){
		case PFC_WARRIOR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_TEMPLAR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_MURMILLO:
		case PFC_SAVAGE:	soundIndex = S_798_WAR_69;	break;
		case PFC_DRUID:		soundIndex = S_798_WAR_69;	break;
		case PFC_EXECUTIONER:soundIndex = S_1123_EXECUTIONER_HIT; break;
		case PFC_ARCHER:
		case PFC_TRAPPER:
		case PFC_BOMBARDIER:
		case PFC_ASSASSIN:
		case PFC_IRON_MAIDEN:
		case PFC_SHARPSHOOTER:
		case PFC_SCOUT:
		case PFC_ROGUE:		soundIndex = S_689_ROG_69;	break;
		case PFC_ELEMENTALIST:
		case PFC_NECROMANCER:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_DEMONOLOGIST:
		case PFC_MAGE:		soundIndex = S_586_SOR_69;	break;
		case PFC_KENSEI:
		case PFC_SHINOBI:
		case PFC_SHUGOKI:
		case PFC_MONK:		soundIndex = S_906_MONK_69;	break;
	}
	if( soundIndex != -1 ){
		PlayLocalSound(soundIndex, player.Row, player.Col);
	}
	int minST, maxST;
	tie(minST, maxST) = GetPlayerStunThreshold(player);
	int stunResist = minST + RNG(maxST - minST); // stun threshold randomize
	LimitToMin(stunResist, 0);
	int oak_check = PerkValue(PERK_RESILIENCE, playerIndex) + PerkValue(PERK_FOOTWORK, playerIndex) + PerkValue(SYNERGY_PERSEVERANCE, playerIndex);
	if (RNG(100) < oak_check) {
		return;
	}
	if( damage >> 6 >= stunResist || RNG(800) == 99 || needStun ){
		int orientation = player.dir;
		if( !(player.LoadPlayerAnimationMask & PAF_4_GOT_HIT) ){
			LoadPlayerGFX(playerIndex, PAF_4_GOT_HIT);
		}
		NewPlayerAnimation(playerIndex, player.gotHitAnimationsPointers[orientation], player.HitRecoveryAnimationFrameCount, 0, player.AnimPitchHit);
		bool stunWalkDown = player.CurAction == PCA_2_WALK_DOWN;
		player.CurAction = PCA_7_GOT_HIT;
		FixPlayerLocation(playerIndex, orientation, stunWalkDown);
		player.currentActionFrame = 1;
		FixPlrWalkTags(playerIndex);
		PlayerMap[ player.Row ][ player.Col ] = (uchar)playerIndex + 1;
		SetPlayerOld(playerIndex);
	}
}

std::tuple<int, int> GetPlayerStunThreshold(const Player& player) {
	int stunResist_min, stunResist_max;
	switch (player.fullClassId) { // #character stun formula list / #player stun
		//WARRIORS
	case PFC_WARRIOR:
	case PFC_INQUISITOR:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 1; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	case PFC_GUARDIAN:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8); 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 8;
		break;
	case PFC_TEMPLAR:
		//ARCHERS
	case PFC_ARCHER:
	case PFC_SCOUT:
	case PFC_SHARPSHOOTER:
	case PFC_TRAPPER:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 1; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 5;
		break;
		//MAGES
	case PFC_MAGE:
	case PFC_ELEMENTALIST:
	case PFC_WARLOCK:
	case PFC_DEMONOLOGIST:
	case PFC_NECROMANCER:
	case PFC_BEASTMASTER:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 1; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 4;
		break;
		//MONKS
	case PFC_MONK:
	case PFC_KENSEI:
	case PFC_DRUID:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 1;
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 7;
		break;
	case PFC_SHUGOKI:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 7) - 2; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	case PFC_SHINOBI:
		//ROGUES
	case PFC_ROGUE:
	case PFC_ASSASSIN:
	case PFC_BOMBARDIER:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 2; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	case PFC_IRON_MAIDEN:
		stunResist_min = (3 * player.CharLevel / 8) + (player.BaseVitality / 8) + 1; 
		stunResist_max = stunResist_min + (8 * player.CharLevel / 11) + 7;
		break;
		//SAVAGES
	case PFC_SAVAGE:
	case PFC_BERSERKER:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 7) - 1; 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	case PFC_EXECUTIONER:
		stunResist_min = (player.CharLevel / 5) + (player.BaseVitality / 4); 
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;// left him full +clvl... keep that in mind if this ST part is de-nerfed!
		break;
	case PFC_THRAEX:
	case PFC_MURMILLO:// mor: Crupellarius could use an uplift, but Trait checks are tricky here
	case PFC_SECUTOR:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 8) - 1;	
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	case PFC_DIMACHAERUS:
		stunResist_min = (player.CharLevel / 3) + (player.BaseVitality / 7) - 1;	
		stunResist_max = stunResist_min + 2 * player.CharLevel / 3 + 6;
		break;
	}
	stunResist_min += player.stunThreshold[CT_0_STUN];
	stunResist_max += player.stunThreshold[CT_0_STUN];
	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		stunResist_min += player.BaseVitality / 10;
		stunResist_max += player.BaseVitality / 5;
	}

	if (GameMode == GM_CLASSIC) {
		stunResist_min = player.CharLevel;
		stunResist_max = stunResist_min;
	}
	
	return std::make_tuple(stunResist_min, stunResist_max);
}

//----- (00713E20) --------------------------------------------------------
void __fastcall RemoveGolemAndMissilesMayBe(int playerIndex)
{
	if( Dungeon != DUN_0_TOWN /*&& playerIndex == CurrentPlayerIndex*/ ){
	    const int playerSummonsOffset = playerIndex * SummonMonstersPerPlayer_Count;
	    for( int summonIndex = 0; summonIndex < SummonMonstersPerPlayer_Count; ++summonIndex  ){
	        const Monster& summon = Monsters[ summonIndex + playerSummonsOffset ];
	        if( summon.Row != 1 || summon.Col ){
	            KillMonsterByPlayer( summonIndex + playerSummonsOffset, playerIndex );
			}
        }
    }
	ClearPlayerSpells( playerIndex );
}

//----- (00713E70) --------------------------------------------------------
void __fastcall ClearPlayerSpells(int playerIndex)
{
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.CasterIndex == playerIndex ){
			switch (spell.BaseMissileIndex ){
				case MI_38_FIRE_WALL:
				case MI_71_LIGHTING_WALL:
				case MI_84_LIGHTING_RING:
				case MI_81_FLAME_RING:
					spell.IsDeleted = 1;
					break;
				case MI_2_HYDRA:
					spell.TimeToLive = 15;
					break;
				case MI_5_FIRE_WALL_SEGMENT:
				case MI_70_LIGHTNING_WALL_SEGMENT:
					spell.TimeToLive = spell.framesCount;
					break;
				//Might wanna add a default case here, cuz spells like Force Wave will keep going after player quits game...
//				default:
//					spell.IsDeleted = 1;
//					spell.TimeToLive = 0;
//					break;
			}
		}
	}
}

//----- (0045A0B3) --------------------------------------------------------
void __fastcall StartPlayerKill(int playerIndex, int earFlag)
{
	if( playerIndex >= 4 ){
		TerminateWithError("StartPlayerKill: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( player.CurLife <= 0 && player.CurAction == PCA_8_DEATH ){
		return;
	}

	// ClearPlayerSpells(playerIndex); // в th1 этого вызова нет, он внутри RemoveGolemAndMissilesMayBe

	if( CurrentPlayerIndex == playerIndex ){
		RemoveGolemAndMissilesMayBe( playerIndex );
        NetSendCmdParam1( 1, CMD_38_PLRDEAD, earFlag );
	}

	int soundIndex = -1;
	
	switch (player.fullClassId ){ // sounds of character death
		case PFC_WARRIOR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_TEMPLAR:	soundIndex = S_11_DEAD; break;
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	soundIndex = S_801_WAR_71;	break;
		case PFC_EXECUTIONER:soundIndex = S_1124_EXECUTIONER_DIE; break;
		case PFC_ARCHER:
		case PFC_TRAPPER:
		case PFC_BOMBARDIER:
		case PFC_ASSASSIN:
		case PFC_IRON_MAIDEN:
		case PFC_SHARPSHOOTER:
		case PFC_SCOUT:
		case PFC_ROGUE:		soundIndex = S_692_ROG_DC;	break;
		case PFC_ELEMENTALIST:
		case PFC_NECROMANCER:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_DEMONOLOGIST:
		case PFC_MAGE:		soundIndex = S_589_SOR_71;	break;
		case PFC_KENSEI:
		case PFC_SHINOBI:
		case PFC_SHUGOKI:
		case PFC_MONK:		soundIndex = S_909_MONK_DC;	break;
	}

	if (player.fullClassId == PFC_TRAPPER)
		for (int i = 0; i < 128; ++i)
			ClearOneOldestPlayerTrap(playerIndex);

	if( soundIndex != -1 && player.dungeon == Dungeon ){
		PlayLocalSound(soundIndex, player.Row, player.Col);
	}

	//if( player.Appearance ){ // в th1 нет этой проверки
		player.Appearance &= ~0xF;// обнуляем биты отвечающие за оружие
		// player.LoadPlayerAnimationMask = 0; // в th1 нет этого обнуления
		SetPlrAnims(playerIndex);
	//}
	if( !(player.LoadPlayerAnimationMask & PAF_8_DEATH ) ){
		LoadPlayerGFX(playerIndex, PAF_8_DEATH );
	}
	NewPlayerAnimation(playerIndex, player.deathAnimationsPointers[player.dir], player.DeathAnimationFrameCount, 1, player.AnimPitchDeath);
	player.CanBlock = false;
	player.CurAction = PCA_8_DEATH;
	player.notHittableInTransition = true;
	SetPlayerHitPoints(playerIndex, 0);
	player.currentActionFrame = 1;
	if( player.lastAttacker > 0 && player.lastAttacker <= PlayersMax_4 ){
		KillPlayerByPlayerEffect(playerIndex, player.lastAttacker - 1);
	}
	if( playerIndex != CurrentPlayerIndex && !earFlag && (MaxCountOfPlayersInGame <= 1) ){
		for( int itemIndex = 0; itemIndex < IS_Inventory; itemIndex++ ){
			Item& item = player.OnBodySlots[itemIndex];
			item.ItemCode = IC_M1_NONE;
		}
		RecalcPlayer(playerIndex, 0);
	}
	if( player.dungeon != Dungeon ){
		return;
	}
	FixPlayerLocation(playerIndex, player.dir);
	RemovePlayerFromMap(playerIndex);
	FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;
	SetPlayerOld(playerIndex);
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	DeathDelayTimer = 30;// Задержка смерти в фрэймах
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		PlayerDeadItem(playerIndex, &player.ItemOnCursor);
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	if (player.gameChanger & BIT(GC_3_DROP_ITMS) || player.gameChanger & BIT(GC_26_ASCETICISM)) {
		for (int itemIndex = 0; itemIndex < IS_Inventory; itemIndex++) {
			PlayerDeadItem(playerIndex, &player.OnBodySlots[itemIndex]);
		}
	}
	// в оригинале в этой функции еще был код для ушей 
	// в TH 1.57 его местое использовалось для DunLev2by9 и DunLevOn32
}

//----- (0045A2CA) --------------------------------------------------------
int ModifyFireBoltDamage()
{
	return 16 * (hack_ModifyTrapDamage() + 2);
}

//----- (0045A427) --------------------------------------------------------
int __fastcall PlayerDeadItem(uint playerIndex, Item* slot)
{
	if( slot->ItemCode == IC_M1_NONE ) return -1;
	if( playerIndex >= PlayersMax_4 ) TerminateWithError("PlrDeadItem: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	int row = player.Row;
	int col = player.Col;
	int itemIndex = -1;
	if( FindGetItem( slot->baseItemIndex, slot->dropType, slot->seed, slot->genVersion, slot->quality, slot->magicFindSeed, slot->uniqIndexSeed ) >= 0 ){
		AddWorldMessage("A duplicate item has been detected.  Destroying duplicate...");
		SyncGetItem( row, col, slot->baseItemIndex, slot->dropType, slot->seed, slot->genVersion, slot->quality, slot->magicFindSeed, slot->uniqIndexSeed );
	}
	if( GetItemSpace(row, col, itemIndex, false, ItemSpaceOk, slot) ){
		RespawnItem(itemIndex, 1);
		if( NetItem ) NetSendCmdPItem(FALSE, CMD_11_RESPAWNITEM, row, col);
	}
	return itemIndex;
}

//----- (0045AA94) --------------------------------------------------------
void __fastcall StripTopGold(int playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError("StripTopGold: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;

	Item tempItem;
	memcpy(&tempItem, &cursorItem, sizeof(Item));

	for( int i = 0; i <  player.InvItemCount; ++i ){
		Item& item = player.InventorySlots[i];
		if( item.ItemCode == IC_11_GOLD && item.amount > MaxGoldInCell ){
			int goldDelta = item.amount - MaxGoldInCell;
			item.amount = MaxGoldInCell;
			ChangeGoldGraphicOnQuantity(playerIndex, i);
			SetPlayerHandItem(cursorItem, 0);
			Item_MakeNonCollisionSeed(playerIndex, cursorItem);
			SetGraphicToGold(cursorItem);
			cursorItem.amount = goldDelta;
			if( !GoldAutoPlace(playerIndex) ){
				PlayerDeadItem(playerIndex, &cursorItem);
			}
		}
	}

	player.TotalGold = CalcTotalGold(playerIndex);
	memcpy(&cursorItem, &tempItem, sizeof(Item));
}

//----- (0045ABD7) --------------------------------------------------------
int __fastcall TryToDie( int playerIndex, int earFlag )
{
	Player& player = Players[playerIndex];
	
	#if GODMODE_ALLOWED
	if( IsGodMode ){
		SetPlayerHitPoints(playerIndex, player.MaxCurLife/*64*20*/);
		return 0;
	}
	#endif
	//if( Quests[ Q_21_FLESHDOOM ].status2 == 9 ){ // fleshdoom patch (2)
	//	Quests[ Q_21_FLESHDOOM ].status2 = 10;
	//	int soundIndex = -1;
	//	switch( player.ClassID ){
	//	case PC_5_SAVAGE:	soundIndex = S_801_WAR_71;	break;
	//	case PC_0_WARRIOR:		soundIndex = S_801_WAR_71;	break;
	//	case PC_1_ARCHER:
	//	case PC_4_ROGUE:		soundIndex = S_692_ROG_DC;	break;
	//	case PC_3_MONK:			soundIndex = S_909_MONK_DC;	break;
	//	case PC_2_MAGE:			soundIndex = S_589_SOR_71;	break;
	//	}
	//	if( soundIndex != -1 ){
	//		PlayLocalSound( soundIndex, player.Row, player.Col );
	//	}
	//	SetPlayerHitPoints( playerIndex, 0 );
	//	return 0;
	//}
	if( Dungeon == DUN_0_TOWN && player.CurLife < 64 || DevelopMode && IsGodMode ){
		SetPlayerHitPoints(playerIndex, IsGodMode ? 640 : 64);
		return 0;
	}else{
		for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
			int spellIndex = MissileIndexes[spellIndexIndex];
			Missile& spell = Missiles[spellIndex];
			if( spell.BaseMissileIndex == MI_13_MANA_SHIELD && spell.CasterIndex == playerIndex && !spell.IsDeleted ){
				if( earFlag != -1 ){ spell.value[7] = earFlag; } 
				return 0;
			}
		}
		SetPlayerHitPoints(playerIndex, 0);
		StartPlayerKill(playerIndex, earFlag);
		return 1;
	}
}

//----- (0045AC77) --------------------------------------------------------
void __fastcall RemovePlayerMissiles(int playerIndex)
{
	if( Dungeon != DUN_0_TOWN && playerIndex == CurrentPlayerIndex ){
	    const int playerSummonsOffset = playerIndex * SummonMonstersPerPlayer_Count;
        for( int summonIndex = 0; summonIndex < SummonMonstersPerPlayer_Count; ++summonIndex  ){
            Monster& summon = Monsters[ summonIndex + playerSummonsOffset ];
            if( summon.Row != 1 || summon.Col ){
                KillMonsterByPlayer( summonIndex + playerSummonsOffset, playerIndex );
                AddMonsterCorpse(summon.Row, summon.Col, summon.newBossId ? summon.udeadNum : summon.SpritePtr->deadSpriteNum, summon.Orientation);
                summon.flag |= MF_15_KILLED;
                MonsterMap[ summon.Row ][ summon.Col ] = 0;
            }
        }
        RemoveKilledMonsters();
	}

//	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){//Faulty code.  MissileAmount is a global variable, which is updated inside this loop by RemoveMissile().  This bugs out the spellIndexIndex iterator, resulting in NOT all missiles being removed.
	int MissileTotal = MissileAmount;
	for (int missileIndexIndex = 0; missileIndexIndex < MissileTotal; missileIndexIndex++) {
		int missileIndex = MissileIndexes[missileIndexIndex];
		Missile& missile = Missiles[missileIndex];
        
        int& lastMonsterTargetAction = missile.value[0];
        int& monsterTargetIndex = missile.value[1];
        
		if( missile.CasterIndex == playerIndex ){
			int spellEffect = missile.BaseMissileIndex;
//			if( spellEffect == MI_13_MANA_SHIELD || spellEffect == MI_34_ETHEREAL ){
//				ClearMissileCell(spellIndex);
//				RemoveMissile(spellIndex, spellIndexIndex);
//			}else if( spellEffect == MI_30_STONE_CURSE ){
//				Monsters[monsterTargetIndex].CurAction = lastMonsterTargetAction;
//			}
			switch (spellEffect) {
			case MI_13_MANA_SHIELD:
			case MI_34_ETHEREAL:
			case MI_79_REFLECT:
				ClearMissileCell(missileIndex);//This function should NOT be called for traveling missiles, as it may remove player Portal if the missiles to be deleted overlap with TP.
				RemoveMissile(missileIndex, missileIndexIndex);
				break;
			case MI_30_STONE_CURSE:
				Monsters[monsterTargetIndex].CurAction = lastMonsterTargetAction;
				break;
			case MI_2_HYDRA:
				missile.TimeToLive = 15;
				break;
			case MI_10_TOWN_PORTAL://TP should be retained after player changes map.
			case MI_65_RED_PORTAL:
				break;
			case MI_43_FURY:
				if( missile.BaseMissileIndex == MI_43_FURY ){
					Player& player = Players[playerIndex];
					if( player.activeBuffFlag & (BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY) ){
						player.activeBuffFlag &= ~(BF_2_FURY_ACTIVE|BF_3_FURY_LETARGY);
						if( player.fullClassId == PFC_DRUID ){
							player.LoadPlayerAnimationMask = PAF_0_NO;
						}
						CalcCharParams(playerIndex, 1);
						LimitToMin(player.CurLife, 64);
					}
				}
				missile.IsDeleted = 1;
				break;
			default:
				missile.IsDeleted = 1;
				break;
			}
		}
	}
}

//----- (0045ADC0) --------------------------------------------------------
void __fastcall InitLevelChange(int playerIndex)
{
	Player& player = Players[playerIndex];

	RemovePlayerMissiles(playerIndex);
	if( playerIndex == CurrentPlayerIndex && Speech_IsPanelVisible ){
		Speech_IsPanelVisible = 0;
		StopSpeech();
	}
	// remove player from current level immediately
	RemovePlayerFromMap(playerIndex);
	SetPlayerOld(playerIndex);
	if( playerIndex == CurrentPlayerIndex ){
		PlayerMap[ player.Row ][ player.Col ] = CurrentPlayerIndex + 1;
	}else{
		player.dungeonVisited[player.dungeon] = 1;
	}
	ClearPlayerPath(playerIndex);
	player.destAction = PCMD_M1_NOTHING;
	player.lvlChanging = 1;
	player.Row = 0;//Hacky solution to unwanted monster activation far away from the destination coordinates of stair / Town Portal, when a player enters dungeon in MP.  This changes the coordinates of the entering player to (0,1) instead of the last coordinates from the previous Dlvl.
	player.Col = 1;//Do NOT use (0,0) as it's the quest area entrance coordinate in SP.  Doing so would send player to Raven Holm upon taking Chamber of Bone stairs.  Do NOT use (111,111) as it can instantly crash the game.
	player.nextArrowElement = -1;
	player.nextMeleeElement = -1;
	if( playerIndex == CurrentPlayerIndex ){
		player.SomeSoundMuteDelay = 10;
	}
}

bool Entering = false;
//----- (0045AE7C) --------------------------------------------------------
bool __fastcall StartNewLvl( uint playerIndex, uint msg, DUNGEON newDungeon )
{
	if( playerIndex >= 4 ) TerminateWithError("StartNewLvl: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];

	if( playerIndex == CurrentPlayerIndex ){
		bool needConfirm = NewDunConfirm && MaxCountOfPlayersInGame == 1 && is(GameMode, GM_IRONMAN) && (Dungeon != DUN_0_TOWN || Dungeon->isQuest)
		 && is(msg, WM_1026_NEXT_DUNGEON, WM_1029_GOTO_QUEST_DUNGEON, WM_1031_GOTO_DUNGEON);
		if( needConfirm && !Entering ){
			bool pressedEnter = IsMainWindowActive && ((GetAsyncKeyState( VK_RETURN ) < 0 && GetAsyncKeyState( VK_MENU ) >= 0) || (curXboxController.GetXInputGetKeystroke(XINPUT_GAMEPAD_A))); // press enter without alt
			if( ! pressedEnter ){
				if (IsController() && !XinputNotInUse(true)) {
					AddOnScreenMessage(OM_79_New_Dun_Confirm_Xinput, 5);
				}
				else {
					AddOnScreenMessage(OM_75_New_Dun_Confirm, 5);
				}
				return false;
			}
			Entering = true;
			for( uchar i = OnScreenMessage::Head; i != OnScreenMessage::Tail; ++i ){
				OnScreenMessage& m = OnScreenMessage::Queue[i];
				if( m.index == OM_75_New_Dun_Confirm ){
					m.time = 0;
				}
			}
		}
		if( ( ! needConfirm || Entering ) && Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
			AutoPutCursorToInventoryOrDrop();
			return false;
		}
	}
	Entering = false;
	
	InitLevelChange(playerIndex);
	switch( msg ){
	case WM_1032_RETURN_TO_TOWN           : Players[CurrentPlayerIndex].OpenedTownDoorMask |= BIT(Dungeon->level / 4);
	case WM_1026_NEXT_DUNGEON             :
	case WM_1027_PREV_DUNGEON             :
	case WM_1028_RETURN_FROM_QUEST_DUNGEON:
	case WM_1029_GOTO_QUEST_DUNGEON       :
	case WM_1030_TOWN_PORTAL              :
	case WM_1031_GOTO_DUNGEON             :
	case WM_1033_RESTART_IN_TOWN          :
		if( playerIndex != CurrentPlayerIndex){
			player.dungeon = newDungeon;
		}else{
			Dungeon.prev = Dungeon;
			Dungeon.next = newDungeon;
		}
		break;
	default:
		TerminateWithError("StartNewLvl");
	}
	if( playerIndex == CurrentPlayerIndex ){
		// TODO: check for needing to clear net commands queue from prev/current dungeon
		player.CurAction = PCA_10_NEW_LEVEL;
		player.notHittableInTransition = 1;
		PostMessageA(ghMainWnd, msg, 0, 0); // TODO: continue here, debug current player dungeon changing
		if( MaxCountOfPlayersInGame > 1 ){
            NetSendCmdParam2( 1, CMD_30_NEWLVL, msg, newDungeon );
		}
	}
	return true;
}

//----- (0045AF7F) --------------------------------------------------------
void __fastcall RestartInTown(uint playerIndex)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("RestartTownLvl: illegal player %d", playerIndex);
	}
	InitLevelChange(playerIndex);
	if( playerIndex != CurrentPlayerIndex){
		player.dungeon = DUN_0_TOWN;
	}else{
		Dungeon.prev = Dungeon;
		Dungeon.next = DUN_0_TOWN;
	}
	player.notHittableInTransition = 0;
	SetPlayerHitPointsOnRestart(playerIndex, 64);
	player.CurMana = 0;
	player.BaseMana = player.MaxBaseMana + player.CurMana - player.MaxCurMana;
	RecalcPlayer(playerIndex, 0);
	if (is(GameMode, GM_COLISEUM, GM_CLASSIC)) {
		player.CurLife = 64;
		player.CurMana = 64;
	}
	else {
		player.CurLife = player.MaxCurLife;
		player.CurMana = player.MaxCurMana;
	}
	if( playerIndex == CurrentPlayerIndex ){
		TakeCurrentPlayerDeathToll ();
		player.CurAction = PCA_10_NEW_LEVEL;
		player.notHittableInTransition = 1;
		PostMessageA(ghMainWnd, WM_1033_RESTART_IN_TOWN, 0, 0);
	}
}

//----- (0045B030) --------------------------------------------------------
void __fastcall StartWarpDungeon( int playerIndex, int portalIndex )
{
	Player& player = Players[playerIndex];
	InitLevelChange(playerIndex);
	DUNGEON dest = player.dungeon != DUN_0_TOWN ? DUN_0_TOWN : Portals[portalIndex].dungeon;
	if( playerIndex == CurrentPlayerIndex ){
		CurrentPortal = portalIndex;
		Dungeon.prev = Dungeon;
		Dungeon.next = dest;
		player.CurAction = PCA_10_NEW_LEVEL;
		player.notHittableInTransition = 1;
		PostMessageA(ghMainWnd, WM_1030_TOWN_PORTAL, 0, 0);
	}else{
		player.dungeon = dest;
	}
}

//----- (0045B0B8) --------------------------------------------------------
int __fastcall PM_DoNothing( int playerIndex )
{
	return 0;
}

// walk вверх
//----- (0045B0BB) --------------------------------------------------------
int __fastcall PM_DoWalkUp(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk: illegal player %d", playerIndex);
	}
	if( player.running ){
		if( !(player.currentAnimationFrame % 2) ){
			player.currentAnimationFrame++;
			player.currentActionFrame++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount ){
			player.currentAnimationFrame = 0;
		}
	}
	int numberOfSubSteps = Dungeon != DUN_0_TOWN ? PlayerClasses[player.ClassID].NumberOfSubSteps : 8;
	if( player.currentActionFrame < numberOfSubSteps ){
		PM_ChangeOffset(playerIndex);
		return 0;
	}else{
		PlayerMap[ player.Row ][ player.Col ] = 0;
		player.Row += player.CursorRow;
		player.Col += player.CursorCol;
		PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){
			ChangeLightPos(player.LightIndex, player.Row, player.Col);
			ChangeVision(player.lightRadiusIdentificator, player.Row, player.Col);
		}
		if( playerIndex == CurrentPlayerIndex && Scroll.dir ){
			PlayerRowPos = player.Row - Scroll.dx;
			PlayerColPos = player.Col - Scroll.dy;
		}
		if( player.walkPath[0] == PCMD_M1_NOTHING ){
			StartStand(playerIndex, player.newDirection);
		}else{
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){
			ChangeLightOffset(player.LightIndex, 0, 0);
		}
		return 1;
	}
}

// walk вниз
//----- (0045B289) --------------------------------------------------------
int __fastcall PM_DoWalkDown(int playerIndex)
{
	int result;
	Player& player = Players[playerIndex];
	if( (uint) playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk2: illegal player %d", playerIndex);
	}
	if( player.running ){
		if( !(player.currentAnimationFrame % 2) ){
			player.currentAnimationFrame++;
			player.currentActionFrame++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount ){
			player.currentAnimationFrame = 0;
		}
	}
	int numberOfSubSteps = Dungeon != DUN_0_TOWN ? PlayerClasses[player.ClassID].NumberOfSubSteps : 8;
	if( player.currentActionFrame < numberOfSubSteps ){
		PM_ChangeOffset(playerIndex);
		result = 0;
	}else{
		PlayerMap[ player.CursorRow ][ player.CursorCol ] = 0;
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){// Подземелья
			ChangeLightPos(player.LightIndex, player.Row, player.Col);// Двигаем радиус света
			ChangeVision(player.lightRadiusIdentificator, player.Row, player.Col);// Что то непонятное 
		}
		if( playerIndex == CurrentPlayerIndex && Scroll.dir != 0 ){
			PlayerRowPos = player.Row - Scroll.dx;
			PlayerColPos = player.Col - Scroll.dy;
		}
		if( Players[playerIndex].walkPath[0] == PCMD_M1_NOTHING ){
			StartStand(playerIndex, player.newDirection);
		}else{
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){
			ChangeLightOffset(player.LightIndex, 0, 0);
		}
		result = 1;
	}
	return result;
}

// walk влево и вправо
//----- (0045B43A) --------------------------------------------------------
int __fastcall PM_DoWalkHorizontal(int playerIndex)
{
	bool result;    // eax@22
	Player& player = Players[playerIndex];
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoWalk3: illegal player %d", playerIndex);
	}
	if( player.running ){// беготня в городе
		if( !(player.currentAnimationFrame % 2) ){// Если player.field_8C чётное
			player.currentAnimationFrame++;
			player.currentActionFrame++;
		}
		if( player.currentAnimationFrame >= player.WalkAnimationFrameCount){// Если player.field_8C достигает какого-то значения
			player.currentAnimationFrame = 0;
		}
	}
	int numberOfSubSteps = Dungeon != DUN_0_TOWN ? PlayerClasses[player.ClassID].NumberOfSubSteps : 8;
	if( player.currentActionFrame < numberOfSubSteps ){// Если всё ещё идём между тайлами, вызываем саб смены картинки
		PM_ChangeOffset(playerIndex);
		result = 0;
	}else{// Иначе если достигли тайла
		PlayerMap[ player.Row ][ player.Col ] = 0;
		FlagMap[ player.HorizontalNextRowOrSomeSpellData ][ player.HorizontalNextCol ] &= ~CF_32_PLAYER_HORIZONTAL;// сбрасываем флаг занятости клетки игроком
		player.Row = player.CursorRow;
		player.Col = player.CursorCol;
		PlayerMap[ player.Row ][ player.Col ] = playerIndex + 1;
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){// Подземелья
			ChangeLightPos(player.LightIndex, player.Row, player.Col);// Двигаем радиус света
			ChangeVision(player.lightRadiusIdentificator, player.Row, player.Col);// Что то непонятное 
		}
		if( playerIndex == CurrentPlayerIndex && Scroll.dir ){
			PlayerRowPos = player.Row - Scroll.dx; // изменение координат игрока при ходьбе 
			PlayerColPos = player.Col - Scroll.dy;
		}
		if( player.walkPath[0] == PCMD_M1_NOTHING ){// Если следующего шага нет
			StartStand(playerIndex, player.newDirection);
		}else{// Если следующий шаг есть
			StartWalkStand(playerIndex);
		}
		ClearPlrPVars(playerIndex);
		if( Dungeon->genType || Dungeon->isQuest /*&& QuestFloorIndex == QF_11_PASSAGE_OF_FIRE*/ ){// Подземелья
			ChangeLightOffset(player.LightIndex, 0, 0);
		}
		result = 1;
	}

	return result;
}

//----- (th2) --------------------------------------------------------
bool DamageItem(uint playerIndex, ITEM_SLOT inventorySlot, Item &item, bool showInflictedDamage)
{
	if( item.ItemCode == IC_M1_NONE && item.CurDurability > 0 ){
		return false;
	}
	#ifdef INDESTRUCTABLE_ENABLED
	if (item.CurDurability == ITEM_DUR_INDESTRUBLE) {
		return false;
	}
	#endif
	
	if( RNG(100) < PerkValue(PERK_CONSERVATOR, playerIndex) ){
		return false;
	}
	if (HasTrait(playerIndex, TraitId::CrowdSeeker) && MaxCountOfPlayersInGame == 1) {
		if (RNG(100) < 25){
			return false;
		}
	}

	--item.CurDurability;
	if (showInflictedDamage && item.CurDurability <= 1) {
		switch (item.TypeID) {
			case ITEM_1_WEAPON:
				DrawFloatingDamagedArmor(inventorySlot, item.CurDurability, "weapon");
				break;
			case ITEM_2_ARMOR:
				if (item.ItemCode == IC_5_SHIELD) {
					DrawFloatingDamagedArmor(inventorySlot, item.CurDurability, "shield");
					break;
				}
				DrawFloatingDamagedArmor(inventorySlot, item.CurDurability, "armor");
				break;
			default:
				break;
		}
	}
	
    if( item.CurDurability <= 0 ){
        if( item.socketsAmount > 0 || Players[CurrentPlayerIndex].gameChanger & BIT(GC_18_INDESTR_ITM) ){
            // Socketed items are too valuable. So we break them only to 0 of 1 state
            if( item.BaseDurability <= 1 ){
                item.BaseDurability = 1;
                item.CurDurability = 0;
                RecalcPlayer( playerIndex, 1 );
            }else{
                item.CurDurability = 1;
                --item.BaseDurability;
            }
        }else{
            item.CurDurability = 1;
            --item.BaseDurability;
            if( item.BaseDurability <= 0 ){
                NetSendCmdDelItem( true, inventorySlot );
                item.ItemCode = IC_M1_NONE;
                RecalcPlayer( playerIndex, 1 );
                return true;
            }
        }
    }
    
    return false;
}

// INDESTRUBLE отключен в свойствах вешей и в коде в TH 1.216
//----- (0045B626) --------------------------------------------------------
bool __fastcall WeaponDur( uint playerIndex, int durDecrParam )
{
	if( playerIndex != CurrentPlayerIndex ){
		return false;
	}

	Player& player = Players[playerIndex];
	Item& leftHand = player.OnBodySlots[IS_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_RightHand];

	if( RNG(durDecrParam) ){
		return false;
	}

	if( playerIndex >= 4 ){
		TerminateWithError("WeaponDur: illegal player %d", playerIndex);
	}

	if (leftHand.TypeID == ITEM_1_WEAPON) {
		if (DamageItem(playerIndex, IS_LeftHand, leftHand, true)) {
			return true;
		}
	}
	if (rightHand.TypeID == ITEM_1_WEAPON) {
		if (DamageItem(playerIndex, IS_RightHand, rightHand, true)) {
			return true;
		}
	}
	if (leftHand.ItemCode == IC_5_SHIELD && rightHand.ItemCode == IC_M1_NONE) {
		if (DamageItem(playerIndex, IS_LeftHand, leftHand, false)) {
			return true;
		}
	}
	if (rightHand.ItemCode == IC_5_SHIELD && leftHand.ItemCode == IC_M1_NONE) {
		if (DamageItem(playerIndex, IS_RightHand, rightHand, false)) {
			return true;
		}
	}

	return false;
}

// update to 1.216
//----- (00719190) --------------------------------------------------------
int __fastcall GetMonsterAcAfterAp( int playerIndex, int monsterArmorClass )
{
	Player& player = Players[playerIndex];
	int armorPiercing = player.armorPiercing;
	if( player.fullClassId == PFC_THRAEX ){
		armorPiercing += 2;
	}
	else if (player.fullClassId == PFC_DIMACHAERUS) {
		armorPiercing += 1;
	}
	if( armorPiercing ){
		monsterArmorClass -= armorPiercing * monsterArmorClass >> 4;
		if( monsterArmorClass < 0 ){
			monsterArmorClass = 0;
		}
	}
	return monsterArmorClass;
}

const int IMMUNE_TO_MELEE_ATTACK = -1;

//----- (th2) --------------------------------------------------------
int CalcPvM_MeleeHitChance( int playerIndex, int monsterIndex, bool isSweepAttack )
{
	Player& player = Players[playerIndex];
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	ushort monsterClass = monster.BasePtr->MonsterClass;
	if( monster.speechIndex || ( monster.CurrentLife & ~63 ) <= 0 
	 || ((baseMonsterIndex == BM_92_ILLUSION_WEAVER  || baseMonsterIndex == BM_C_344_ILLUSION_WEAVER ) && monster.goal == MG_2_RETREAT)
	 || ((baseMonsterIndex == BM_184_REALITY_WEAVER ) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	 || ((baseMonsterIndex == BM_194_BLOODIED_HIDDEN) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	 || ((baseMonsterIndex == BM_183_STALKER) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	 || ((baseMonsterIndex >= BM_29_HIDDEN && baseMonsterIndex <= BM_32_INVISIBLE_ONE) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	 || ((baseMonsterIndex == BM_194_BLOODIED_HIDDEN) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	|| ((baseMonsterIndex == BM_94_LURKER) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	|| ((baseMonsterIndex == BM_257_SHROUDED) && monster.goal == MG_2_RETREAT && GameMode == GM_HARD)
	|| monsterIndex < SummonMonsters_Count
	 || monster.CurAction == A_14_CHARGE
	 || monster.CurAction == A_15_STONE && (!HasTrait(playerIndex, TraitId::Petrifier)) && !(GameMode == GM_CLASSIC) 
		){
		return IMMUNE_TO_MELEE_ATTACK;
	}
	int monsterArmorClass = monster.ArmorClass;
	monsterArmorClass = GetMonsterAcAfterAp( playerIndex, monsterArmorClass );
	int monsterMinAC = (monsterArmorClass + 8) / 4;
	monsterArmorClass -= PerkValue(PERK_VANDALISM, playerIndex);
	LimitToMin(monsterArmorClass, 0);
	monsterArmorClass += RNG(monsterMinAC) - monsterMinAC;
	if( monster.newBossId ){
		monsterArmorClass += RNG(21);
	}
	int toHitChance = player.CurDexterity + player.CharLevel - monsterArmorClass;
	if (GameMode == GM_CLASSIC) { toHitChance -= player.CurDexterity / 2; }
	if( isSweepAttack ){
		int levelDiff = monster.MonsterLevel - player.CharLevel;
		LimitToMin(levelDiff, 0);
		int swapToHitPenalty = 2 * levelDiff + 40 - PerkValue(PERK_LATERAL_PRECISION, playerIndex);
		toHitChance -= swapToHitPenalty;
	}
	switch (player.ClassID ){
		default:
		case PC_1_ARCHER:
		case PC_2_MAGE:		toHitChance += 30;	break;
		case PC_0_WARRIOR:	toHitChance += GameMode == GM_CLASSIC ? 50 : 40;	break;
		case PC_5_SAVAGE:	toHitChance += 45;	break;
		case PC_3_MONK:		toHitChance += 50;	break;
		case PC_4_ROGUE:	toHitChance += 55;	break;
	}
	if (player.fullClassId == PFC_EXECUTIONER) { // because Executioner now has max Dex of 25, he needs this boost
		toHitChance += player.CharLevel;
	}
	if (player.fullClassId == PFC_DRUID) { toHitChance += 10; }
	if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) { toHitChance += 30; }
	if (HasTrait(playerIndex, TraitId::Mamluk)) {	toHitChance += 25;	}
	if (HasTrait(playerIndex, TraitId::Paladin)) {	toHitChance += 5;	}
	if (HasTrait(playerIndex, TraitId::Crupellarius)) { toHitChance -= 10; }
	toHitChance += player.ItemsAddToHitPercents
		+ player.elementToHit[ET_0_PHYSICAL] // вся melee атака сейчас Physical, возможно это расширится в дальнейшем
		+ player.spiciesToHit[monsterClass]; //  bonus tohit vs beasts/demons/undead
	int maxToHit = 100;
	if( baseMonsterIndex == BM_29_HIDDEN ){
		switch( Dungeon->level ){
		case 11: maxToHit = 90;	break;
		case 12: maxToHit = 88;	break;
		case 13: maxToHit = 85;	break;
		case 14: maxToHit = 80;	break;
		case 15: maxToHit = 75;	break;
		case 16: maxToHit = 70;	break; // they no longer appear after DLVL-15
		case 17: maxToHit = 65;	break;
		case 18: maxToHit = 60;	break;
		case 19: maxToHit = 55;	break;
		case 20: maxToHit = 50;	break;
		}
	}else if( baseMonsterIndex == BM_162_CORRUPT_PALADIN ){
		if (is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON )) {
			maxToHit = 55;
		}
		else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
			maxToHit = 66;
		}
		else if (GameMode == GM_HARD) {
			maxToHit = 29;
		}
		else {
			maxToHit = 35;
		}
	}else if( baseMonsterIndex == BM_208_WARPED_ONE      ){
		maxToHit = 45;
	}else if( baseMonsterIndex == BM_203_DEATH_KNIGHT    ){
		if( MaxCountOfPlayersInGame != 1 ){
			maxToHit = 40;
		}
	}
	LimitToMax(maxToHit, monster.newBossId ? 80 : 95);
	if (PerkValue(PERK_DUELIST, playerIndex) >= 1) {
		maxToHit += PerkValue(PERK_DUELIST, playerIndex);
	}
	else if (HasTrait(playerIndex, TraitId::MonkeyGrip)) {
		maxToHit -= maxToHit / 5;
	}
	else if (HasTrait(playerIndex, TraitId::Zealot)) {
		maxToHit -= 15 * maxToHit / 100;
	}
	LimitToMin ( maxToHit, 25 + PerkValue(PERK_DUELIST, playerIndex) );
	int minToHit = 5;
	LimitToRange ( toHitChance, minToHit, maxToHit );
	return toHitChance;
}

//----- (0045B848) --------------------------------------------------------
int __fastcall PvM_Melee( int playerIndex, uint monsterIndex, bool isSweepAttack, bool isThorns, int minDamage, int maxDamage )
{
	Monster& monster = Monsters[monsterIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	if( monsterIndex >= Monsters_Max_Count ){
		TerminateWithError("PlrHitMonst: illegal monster %d", monsterIndex);
	}
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PlrHitMonst: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	if( !monster.ActivationCounter && !(monster.flag & MF_6_FRIENDLY) ){
		MonsterChainActivation( monsterIndex ); // цепная активация
	}
	int toHitRandFactor = RNG(100);
	int hitChance = CalcPvM_MeleeHitChance( playerIndex, monsterIndex, isSweepAttack );
	if( hitChance < 0 ){
		return 0;
	}
	if( OneShoot ){
		KillMonsterByPlayer(monsterIndex, playerIndex);
		return 1;
	}
	if( toHitRandFactor >= hitChance ){ // проверка попадания 
		return 0;
	}
	int isMonsterHit = 0;
	if( CheckMonsterPhaseOrBlock(monsterIndex, &isMonsterHit) ){ // проверка блока 
		return isMonsterHit;
	}
	int monsterClass = monster.BasePtr->MonsterClass;
	int damage = 0;
	if( isThorns ){
		damage = RngFromRange( minDamage, maxDamage); // thorns damage (combined?)
		//damage += ( damage * player.ItemsAddDamagePercents ) / 100; // +% ED from items is not going to afffect thorns damage anymore, this was stupid anyway
		int a_rose_with_thorns_trait_benefit = HasTrait(playerIndex, TraitId::ARoseWithThorns) ? (player.CharLevel / 2) : 0;
		damage += PerkValue(PERK_ANTISOCIAL, playerIndex) + a_rose_with_thorns_trait_benefit; // old>>> // + flat damage from items
	}else{
		int damageFromItem = RngFromRange( player.MinDamageFromItem, player.MaxDamageFromItem);
		damage = player.BaseDamage
			+ player.ItemsAddDamage
			+ RngFromRange(player.spiciesDamageMin[monsterClass], player.spiciesDamageMax[monsterClass])
			//ex code: RNG(player.spiciesDamageMax[monsterClass])
			+ damageFromItem * ( player.ItemsAddDamagePercents + ( player.spiciesDamagePercent[monsterClass] ) ) / 100
			+ damageFromItem; // <- new formula adding +/+% species dmg to regular +/% weapon's dmg
		// Bouncer trait benefit damage increaser: +50%
		if (HasTrait(playerIndex, TraitId::Bouncer) ) {
			damage += damage * 50 / 100;
		}
		if (HasTrait(playerIndex, TraitId::Destroyer)) {
			damage += damage * 15 / 100;
		}
		if( isSweepAttack ){
			damage = damage * ( RNG(50) + 1 ) / 100;
			//damage = damage * RNG(player.CharLevel / 3 + 1) / 100; // sweep damage = Rnd[CLVL / 3 + 1]%,   randomly (1-50)%
			if (player.fullClassId == PFC_MONK || player.fullClassId == PFC_DIMACHAERUS) {
				damage = damage * ( 100 + PerkValue(PERK_CLEAVE, playerIndex) ) / 100 ; // Monk sweep damage
			}
		}
		//new affixes with bonus dmg to beasts/demons/undead
		//damage += damage * (player.spiciesDamagePercent[monsterClass]) / 100;
		//damage += RNG(player.spiciesDamageMax[monsterClass]);
		int weaponClass = -1;
		int leftHandItemCode = player.OnBodySlots[IS_LeftHand].ItemCode;
		int rightHandItemCode = player.OnBodySlots[IS_RightHand].ItemCode;
		if( leftHandItemCode == IC_1_SWORD && rightHandItemCode != IC_4_MACE
		 || leftHandItemCode != IC_4_MACE && rightHandItemCode == IC_1_SWORD ){
			weaponClass = IC_1_SWORD;
		}
		else if( leftHandItemCode == IC_4_MACE && rightHandItemCode != IC_1_SWORD
		 || leftHandItemCode != IC_1_SWORD && rightHandItemCode == IC_4_MACE ){
			weaponClass = IC_4_MACE;
		}
		else if (leftHandItemCode == IC_20_CLAW || rightHandItemCode == IC_20_CLAW) {
			weaponClass = IC_20_CLAW;
		}
		else if (leftHandItemCode == IC_21_KNIFE || rightHandItemCode == IC_21_KNIFE) {
			weaponClass = IC_21_KNIFE;
		}
		else if (leftHandItemCode == IC_22_MALLET || rightHandItemCode == IC_22_MALLET) {
			weaponClass = IC_22_MALLET;
		}
		else if (leftHandItemCode == IC_23_PISTOL || rightHandItemCode == IC_23_PISTOL) {
			weaponClass = IC_23_PISTOL;
		}
		else if (leftHandItemCode == IC_10_STAFF || rightHandItemCode == IC_10_STAFF) {
			weaponClass = IC_10_STAFF;
		}

		// Hack & Slash GC skips damage modifiers...
		if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
			if (!(HasTrait(playerIndex, TraitId::Mamluk))) {
				// total damage modifier against undead (inquisitors skip dmg modification)
				if (monsterClass == MON_0_UNDEAD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
					int damageModPercent = 0;
					if (weaponClass == IC_1_SWORD) {
						if (player.fullClassId != PFC_THRAEX) {
							damageModPercent = -50; // -50% damage for all but thraex
						}
					}
					if (weaponClass == IC_4_MACE) {
						damageModPercent = 50;
						if (player.fullClassId == PFC_TEMPLAR) {
							damageModPercent += 20; // Templar has +70% bonus with blunts instead of standard +50%
						}
						else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
							damageModPercent -= 35;
						}
						else if (player.fullClassId == PFC_SECUTOR && HasTrait(playerIndex, TraitId::Bestiarius)) {
							damageModPercent -= 25;
						}
						else if (player.fullClassId == PFC_KENSEI) {
							damageModPercent -= 20;
						}
						else if (player.fullClassId == PFC_GUARDIAN) {
							damageModPercent -= 10;
						}
					}
					if (weaponClass == IC_20_CLAW) {
						damageModPercent = -20;
					}
					damage += damage * damageModPercent / 100;
				}

				// Monk: +20% dmg to demons
				if (monsterClass == MON_1_DEMON && player.fullClassId == PFC_MONK && weaponClass == IC_10_STAFF) { // +20% dmg to demons with staff
					damage += damage / 5;
				}

				//PvM melee: damage modifier vs beasts:
				if (monsterClass == MON_2_BEAST && (player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID)) {
					if (weaponClass == IC_4_MACE) {
						damage -= damage / 2;
					}
					else if (weaponClass == IC_1_SWORD) {
						if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
							damage += damage * 15 / 100;
						}
						else if (player.fullClassId == PFC_SECUTOR && HasTrait(playerIndex, TraitId::Bestiarius)) {
							damage += damage * 80 / 100; // Secutor with Bestiarius trait does +80% damage to beasts
						}
						else if (player.fullClassId == PFC_KENSEI) {
							damage += damage * 30 / 100;
						}
						else if (player.fullClassId == PFC_GUARDIAN) {
							damage += damage * 40 / 100;
						}
						else {
							damage += damage / 2;
						}

					}
					else if (weaponClass == IC_20_CLAW) {
						damage += damage / 5;
					}
					else if (weaponClass == IC_10_STAFF && player.fullClassId == PFC_MONK) {
						damage -= damage * 20 / 100;
					}
				}
			}
		}
		damage += damage * player.spiciesDamageTotal[monsterClass] / 100;
		damage += damage * PerkValue( by(monsterClass, PERK_UNDEAD_HUNTER, PERK_DEMON_HUNTER, PERK_BEAST_HUNTER), playerIndex) / 100; //perks increasing melee dmg vs species
	}
	if( player.effectFlag[EA_MORE_DAMAGE_CHANCE] && RNG(100) < 30 ){
		damage = 15 * damage / 10; // 30% chance for +50% damage now
	}
	bool canCrit_melee = true;
	if (player.fullClassId == PFC_SAVAGE) {
		canCrit_melee = false;
	}
	if (player.fullClassId == PFC_INQUISITOR && HasTrait(playerIndex, TraitId::Tormentor)) {
		canCrit_melee = false;
	}
	if (canCrit_melee) { // ability to score crits in melee
		ActCriticalHit(damage, playerIndex, monster.Row, monster.Col, true, 0, -1);// -1 melee damage (?)
	} 
	// if( baseMonsterIndex == BM_137_UBER_DIABLO ){ damage /= MaxCountOfPlayersInGame != 1 && Difficulty == DL_2_DOOM ? 8 : 4; // Mor: старый вариант условия не стираю.
	if (GameMode == GM_CLASSIC) { goto CLASSIC_DAMAGE_MODIFY_SKIP; }
	if( baseMonsterIndex == BM_137_UBER_DIABLO ) {
		//damage /= MaxCountOfPlayersInGame != 1 ? by (Difficulty, 4, 6, 8) : 4; // в новом варианте добавлен резист /6 на пургатори, для более гладкого прогресса силы резиста убера (в сингле /4)				
		if (GameMode == GM_HARD) {
			damage -= 50 * damage / 100; // -50% damage in melee
		}
		else if (GameMode == GM_IRONMAN || GameMode == GM_SPEEDRUN) {
			damage -= 30 * damage / 100;
		}
		else {
			damage -= 40 * damage / 100;
		}
		// additional multiplayer / singleplayer damage reduction for Uber Diablo
		if (MaxCountOfPlayersInGame != 1) {
			int reduce_factor = by (Difficulty, 50, 60, 70); // left: 25%, 20%, 15%
			damage -= reduce_factor * damage / 100;
		}
		else {
			damage /= 2;
		}
	}
	else { // monster resistance types to melee damage from player:
		int puncturing_stab_trait_benefit = HasTrait(playerIndex, TraitId::PuncturingStab) ? (player.CharLevel / 5) : 0;
		puncturing_stab_trait_benefit += PerkValue(PERK_SUNDERING_STRIKE, playerIndex);
		if (RNG(100) >= puncturing_stab_trait_benefit) { // skip monster melee damage resistance if player has trait 'puncturing stab' and armor pierce roll is a success
			if (baseMonsterIndex == BM_1_OBLIVION_KNIGHT) {
				if (Difficulty >= 1) { damage -= damage / 4; }
			}
			else if (baseMonsterIndex == BM_18_VORTEX_ROGUE) {
				if (MaxCountOfPlayersInGame != 1) {
					if( Dungeon->level == 10 ){
						damage -= damage / 8;
					}
					else if( Dungeon->level == 11 ){
						damage -= damage / 4;
					}
					else if( Dungeon->level == 12 ){
						damage -= damage / 2;
					}
				}
			}
			else if (baseMonsterIndex == BM_117_STINGER) {
				if (MaxCountOfPlayersInGame != 1) { damage += damage / 2; }
			}
			else if (baseMonsterIndex == BM_94_LURKER) {
				if (Difficulty == DL_0_HORROR) {
					if( Dungeon->level == 3 ){
						damage -= damage / 8;
					}
					else if( Dungeon->level == 4 ){
						damage -= damage / 4;
					}
					else if( Dungeon->level == 5 ){ // TODO: check for quest level monsters, may be dungeon number is more suitable
						damage -= damage / 4 + damage / 8;
					}
				}
			}
			else if (baseMonsterIndex == BM_30_SHADOW) {
				if (monster.goal == MG_2_RETREAT) { damage /= 2; }
			}
			else if (baseMonsterIndex == BM_88_BALROG) {
				if (MaxCountOfPlayersInGame != 1 && Dungeon->level == 16) { damage -= damage / 4 + damage / 8; }
			}
			else if (baseMonsterIndex == BM_40_QUASIT) {
				if (Difficulty == DL_0_HORROR && Dungeon->level == 3) { damage += damage / 4; }
			}
			else if (baseMonsterIndex == BM_110_DIABLO) { // diablo
				if (GameMode == GM_HARD) {
					damage -= damage / 3;
				}
				else {
					damage -= damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_58_DEFILER) { // defiler
				if (GameMode == GM_HARD) { 
					damage -= 44 * damage / 100; 
				}
				else { 
					damage -= 38 * damage / 100; 
				}
				if (Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS && Quests[Q_16_HORAZON_S_DEMONS].status2 == 3) {// stronger na-krul
					damage /= 4;
				}
			}
			else if (baseMonsterIndex == BM_35_FIRE_CLAN) {
				if (Dungeon->level < 4 && Difficulty == DL_0_HORROR) { damage -= damage / 4 + damage / 8; }
			}
			else if (baseMonsterIndex == BM_125_GOLD_GOLEM) {
				if (Dungeon->level == 21) {
					damage -= damage / 4;
				}
				else if (Dungeon->level == 22) {
					damage -= damage / 4 + damage / 8;
				}
				else {
					damage /= 2;
				}
			}
			else if (baseMonsterIndex == BM_64_DEATH_GUARDIAN) {
				Dungeon->level == 16 ? damage /= 8 : damage -= damage / 4;
			}
			else if (baseMonsterIndex == BM_163_STINGER_LORD) {
				damage /= 2;
			}
			else if (is(baseMonsterIndex, BM_6_GRAVE_DIGGER, BM_0_BURNED_CORPSE)) {
				switch (Difficulty) {
				case DL_0_HORROR: if (Dungeon->level == 2) { damage -= damage / 4; } break;
				case DL_1_PURGATORY: damage -= damage / 4 + damage / 8; break;
				default: damage /= 2; break;
				}
			}
			else if (is(baseMonsterIndex, BM_15_TOMB_SLAVE, BM_140_GHOUL, BM_144_DARK_SERVANT, BM_145_BLACK_MOURNER)) {
				switch (Difficulty) {
				case DL_0_HORROR: if (Dungeon->level == 3) { damage -= damage / 8; }
								  else if (Dungeon->level == 4) { damage -= damage / 4; } break;
				case DL_1_PURGATORY: damage -= damage / 4 + damage / 8; break;
				default: damage /= 2; break;
				}
			}
			else if (is(baseMonsterIndex, BM_72_FIREWING, BM_132_BONEWING)) {
				if (Dungeon->level == 5) { damage -= damage / 4; }
			}
			else if (baseMonsterIndex == BM_155_FALLEN_ONE) {
				if (Difficulty == DL_0_HORROR) {
					if (Dungeon->level == 2) {
						damage -= damage / 4;
					}
					else if (Dungeon->level == 3) {
						damage /= 2;
					}
					else if (Dungeon->level == 4) {
						damage -= damage / 2 + damage / 8;
					}
				}
			}
			else if (baseMonsterIndex == BM_203_DEATH_KNIGHT) {
				MaxCountOfPlayersInGame == 1 ? damage -= damage / 4 + damage / 8 : damage /= 2;
			}
			else if (baseMonsterIndex == BM_95_GHOST_WARRIOR || baseMonsterIndex == BM_285_PHANTOM_KNIGHT) {
				if (!isThorns) {
					if (is(GameMode, GM_EASY, /*GM_CLASSIC,*/ GM_IRONMAN, GM_SPEEDRUN)) {
						damage -= by(Difficulty, 40, 150, 300, 450, 600, 750, 900, 1050, 1200);
					}
					else {
						damage -= by(Difficulty, 60, 220, 450, 670, 890, 1100, 1300, 1500, 1700);
					}
				}
				else { 
					damage -= by(Difficulty, 3, 11, 22, 33, 44, 55, 66, 77, 88); 
				}
			}
			else if (baseMonsterIndex == BM_96_CURSE_BEARER) {
				if (!isThorns) {
					if (is(GameMode, GM_EASY, /*GM_CLASSIC,*/ GM_IRONMAN, GM_SPEEDRUN)) {
						damage -= by(Difficulty, 45, 166, 333, 500, 666, 833, 1000, 1166, 1333);
					}
					else {
						damage -= by(Difficulty, 70, 250, 500, 750, 1000, 1250, 1500, 1750, 2000);
					}
				}
				else { 
					damage -= by(Difficulty, 4, 12, 24, 36, 48, 60, 72, 84, 96); 
				}
			}
			else if (baseMonsterIndex == BM_261_SKELETAL_KNIGHT) {
				if (!isThorns) {
					if (is(GameMode, GM_EASY, /*GM_CLASSIC,*/ GM_IRONMAN, GM_SPEEDRUN)) {
						damage -= by(Difficulty, 55, 200, 400, 550, 700, 900, 1100, 1300, 1500);
					}
					else {
						damage -= by(Difficulty, 85, 290, 550, 800, 1100, 1375, 1650, 1900, 2200);
					}
				}
				else {
					damage -= by(Difficulty, 4, 14, 26, 40, 52, 66, 78, 90, 102);
				}
			}
			else if (baseMonsterIndex == BM_122_BLOOD_SPECTRE) {
				if (!isThorns) {
					if (is(GameMode, GM_EASY, /*GM_CLASSIC,*/ GM_IRONMAN, GM_SPEEDRUN)) {
						damage -= by(Difficulty, 65, 225, 450, 675, 900, 1125, 1350, 1575, 1800);
					}
					else {
						damage -= by(Difficulty, 100, 325, 600, 900, 1200, 1500, 1800, 2100, 2400);
					}
				}
				else { 
					damage -= by(Difficulty, 5, 16, 28, 42, 55, 70, 83, 96, 109); 
				}
			}
			else if( is(baseMonsterIndex, BM_233_BLOOD_LORD, BM_297_OROG) ){
				if (!isThorns) {
					if(Dungeon == DUN_113_THE_MAZE){
						damage -= damage / 8;
						damage -= 15 + Difficulty * 40;
					}
					else {
						damage -= damage / 5;
						damage -= by(Difficulty, 30, 120, 240, 360, 480, 600, 720, 840, 960);
					}
				}
				else {
					damage -= damage / 6; 
					damage -= by(Difficulty, 2, 10, 20, 30, 40, 50, 60, 70, 80);
				}
			}
			else if (is(baseMonsterIndex, BM_267_BONE_BLADE, BM_268_BONE_BLADE_2)) {
				if (!isThorns) {
					damage -= damage / 2;
					damage -= by(Difficulty, 25, 50, 75, 100, 125, 150, 175, 200, 225);
				}
				else {
					damage -= damage / 3;
					damage -= by(Difficulty, 2, 10, 20, 30, 40, 50, 60, 70, 80);
				}
			}
			else if (baseMonsterIndex == BM_275_BEHEMOTH) {
				if (!isThorns) {
					damage -= 6 * damage / 10; 
					damage -= by(Difficulty, 30, 60, 90, 120, 150, 180, 210, 240, 270); 
				}
				else {
					damage -= damage / 2; 
					damage -= 12 + Difficulty * 14; 
				}
			}
			else if (baseMonsterIndex == BM_274_FLESH_DEVOURER) {
				if (!isThorns) {
					damage -= damage / 5;
				}
				else {
					damage -= damage / 10;
				}
			}
			else if (is(baseMonsterIndex, BM_271_THORNED_HULK, BM_272_SPIDER_QUEEN)) {
				if (!isThorns) {
					damage -= damage / 3;
				}
				else {
					damage -= damage / 4;
				}
			}
			else if (baseMonsterIndex == BM_234_STEEL_LORD) {
				if (!isThorns) {
					damage -= damage / 4;
					damage -= by(Difficulty, 50, 180, 360, 540, 720, 900, 1080, 1260, 1440);
				}
				else {					
					damage -= damage / 5; 
					damage -= by(Difficulty, 5, 20, 40, 60, 80, 100, 120, 140, 160);
				}
			}
			else if (baseMonsterIndex == BM_252_HEPHASTO) {
				if (!isThorns) {
					int dfe_reduction = by(Difficulty, 50, 200, 400, 600, 800, 1000, 1200, 1400, 1600);
					if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE, GM_EASY/*, GM_CLASSIC*/)) {
						dfe_reduction /= 2; 
					}
					damage -= dfe_reduction;
				}
				else {					
					damage -= by(Difficulty, 10, 40, 80, 120, 160, 200, 240, 280, 320);
				}
				if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE, GM_EASY/*, GM_CLASSIC*/)) {
					damage -= damage / 4;// -25% damage resist in challenge modes
				}
				else {
					damage -= 2 * damage / 5; // -40%...
				}
			}
			else if (baseMonsterIndex == BM_242_REANIMATED_HORROR) {
				if (!isThorns) {
					damage -= damage / 5;
					damage -= by(Difficulty, 10, 60, 130, 200, 270, 340, 410, 480, 550);
				}
				else {
					damage -= damage / 10;
					damage -= by(Difficulty, 2, 8, 16, 24, 32, 40, 48, 56, 64);
				}
			}
			else if (baseMonsterIndex == BM_243_IZUAL) {
				if (!isThorns) {
					if (GameMode == GM_HARD) {
						damage -= by(Difficulty, 100, 300, 600, 800, 1000, 1200, 1400, 1600, 1800);
						damage -= 65 * damage / 100; // -65% damage in melee
					}
					else if (GameMode == GM_IRONMAN || GameMode == GM_SPEEDRUN || GameMode == GM_SURVIVAL) {
						damage -= by(Difficulty, 5, 20, 40, 60, 80, 100, 120, 140, 160);
						damage -= 40 * damage / 100;
					}
					else {
						damage -= by(Difficulty, 25, 100, 200, 300, 400, 500, 600, 700, 800);
						damage -= 50 * damage / 100;
					}
				}
				else {
					damage -= 55 * damage / 100;
				}
			}
			else if (baseMonsterIndex == BM_249_SCARAB) {
				if (!isThorns) {
					damage -= damage / 10;
					damage -= by(Difficulty, 2, 6, 12, 17, 22, 27, 32, 37, 42);
				}
				else {
					damage -= damage / 10;
					damage -= by(Difficulty, 1, 3, 5, 7, 9, 11, 13, 15, 18);
				}
			}
			else if (baseMonsterIndex == BM_250_BONE_SCARAB) {
				if (!isThorns) {
					damage -= damage / 5;
					damage -= by(Difficulty, 4, 12, 25, 37, 50, 62, 75, 87, 100);
				}
				else {
					damage -= damage / 8;
					damage -= by(Difficulty, 2, 5, 8, 11, 14, 17, 20, 23, 26);
				}
			}
			else if (is(baseMonsterIndex, BM_286_BRINE_CORPSE, BM_288_EARTH_GOLEM, BM_292_BUGBEAR)) {
				damage -= damage / 3;				
			}
			else if (baseMonsterIndex == BM_287_ARMORED_SKELETON) {
				if (Dungeon == DUN_9_CAVE_1) { 
					damage -= 2 * damage / 5; 
				}
				else { 
					damage -= damage / 3; 
				}
			}
			else if (baseMonsterIndex == BM_120_NECROMORPH) {
				if (Dungeon == DUN_23_CRYPT_3) { // necromorph damage resistance increased for dlvls 23 & 24
					damage -= damage / 5;
				}
				else if (Dungeon == DUN_24_CRYPT_4) {
					damage -= damage / 4;
				}
			}
		}

		if (HasTrait(playerIndex, TraitId::Hunger)) { // trait 'Hunger', only available to Shugoki class
			damage -= Difficulty * 24 + Dungeon->level; // trait 'Hunger' additionally inmproves DFE on all monsters by DLVL on Horror, DLVL+24 Purg., and DLVL+48 on Doom
		}
		else if (HasTrait(playerIndex, TraitId::BloodForBlood) && player.fullClassId == PFC_THRAEX && player.CurLife < player.MaxCurLife * 40 / 100) {
			damage += damage * 30 / 100; // improves Thraex' melee damage by 30% if life drops below 40%
		}
		else if (player.fullClassId == PFC_TEMPLAR && player.CurLife > (99 * player.MaxCurLife / 100)) {
			damage += PerkValue(PERK_HOLY_CAUSE, playerIndex) * damage / 100;
		}
		
		if (monster.newBossId) { // damage to BOSS, modification ----------------------------------------------------------------------
			// mor: this is default boss damage reduction (-50%), plus the half-swording perk improvement of that damage (not just perk here, careful ! )
			damage -= damage * (100 - PerkValue(PERK_HALF_SWORDING, playerIndex)) / 200; // this perk is only available to Guardian class
			// perk "bravado" doubles damage against bosses with a chance determined by the perk (chances are divided by 4 for bosses)
			if (RNG(400) < PerkValue(PERK_BRAVADO, playerIndex)) {
				damage += damage / 2;
			}
		}
		else if (HasTrait(playerIndex, TraitId::OldHabit)) { // trait increases damage to weak non-boss monsters, only available to Executioner
			if (monster.CurrentLife <= (25 + PerkValue(PERK_END_THEM_RIGHTLY, playerIndex)) * monster.BaseLife / 100) {
				damage *= 3;
			}
		}
		else if (player.fullClassId == PFC_ASSASSIN) { // assassin perk that instakills target
			if (monster.CurrentLife < PerkValue(PERK_CULMINATION, playerIndex) * monster.BaseLife / 100) {
				CastMonsterCritSplash(monsterIndex, 1, playerIndex);
				if (monster.CurAction != A_15_STONE) {
					DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex);
					KillMonsterByPlayer(monsterIndex, playerIndex);
					return 1;
				}
				DrawFloatingDamage(damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex); 
				KillMonsterByPlayer(monsterIndex, playerIndex);
				monster.CurAction = A_15_STONE;
				return 1;
			}
		}
		// perk "bravado" doubles damage against monsters (non-bosses) with a chance determined by the perk
		else if (RNG(100) < PerkValue(PERK_BRAVADO, playerIndex)) {
			damage += damage / 2;
		}		
		// shinobi's specific perk, that improves damage vs weak monsters
		if (monster.CurrentLife <= (20 + PerkValue(SYNERGY_START_EARLY, playerIndex)) * monster.BaseLife / 100) {
			damage += damage * PerkValue(PERK_FINAL_JUDGEMENT, playerIndex) / 100 ;
		}
		// savage's specific perk, that improves damage vs strong monsters (much hp)
		if (player.fullClassId == PFC_SAVAGE && monster.CurrentLife >= ((90 - PerkValue(SYNERGY_SNOWBALL_EFFECT, playerIndex)) * monster.BaseLife / 100)) {
			damage += damage * PerkValue(PERK_CRUSHING_BLOWS, playerIndex) / 100;
		}
		// secutor's specific perk, that improves damage with a certain chance for proc (jackal)
		if ((player.fullClassId == PFC_SECUTOR || player.fullClassId == PFC_DRUID) && RNG(100) < PerkValue(PERK_JACKAL_TECHNIQUE, playerIndex, 0)) {
			damage += damage * PerkValue(PERK_JACKAL_TECHNIQUE, playerIndex, 1) / 100;
		}
		if (MaxCountOfPlayersInGame == 1 && HasTrait(playerIndex, TraitId::Fatality)) {
			if (RNG(100) < 10) {
				damage *= 3;
				PlayLocalSound(S_972_CRIT_SOUND_01, player.Row, player.Col);
			}
		}

		if( has(monster.affixes, MAF_ARMORED) ){ //33% melee damage reduce from monster affix
			damage = damage * 67 / 100;
		}
	}
	CLASSIC_DAMAGE_MODIFY_SKIP:
	// damage modification according to number of players in game
	if( MaxCountOfPlayersInGame != 1 ){
		if(       NetPlayerCount == 2 ){ damage -= damage/4; // -25% damage // 150%
		}else if( NetPlayerCount == 3 ){ damage -= 2*damage/5; // -40% damage // 180%
		}else if( NetPlayerCount == 4 ){ damage -= damage/2; // - 50% damage // 200%
		}
	}
	#ifdef comment
	if( player.effectFlag[EA_MANACOST] && baseMonsterIndex != BM_110_GREATER_DEMON 
	 && !monster.newBossID && RNG(100) < 10 ){
		CloneMonsterByDoppelganger(monsterIndex);
	}
	#endif
	LimitToMin( damage, 1 );
	damage <<= 6;
	if( player.effectFlag[EA_RANDOM_DAMAGE] ){
		int damagePercent = RNG(71); // Rnd[0-70]
		damagePercent += 80 ;// Rnd[80-160]
		damage = damage * damagePercent / 100;// randomly [75-165]% damage now
	}
	//if( isSweepAttack ){ // sweep attack damage is again divided by 2? why?
	//	damage /= 2;
	//}
	bool damageStone = HasTrait( playerIndex, TraitId::Petrifier); 
	if( monster.CurAction == A_15_STONE && damageStone ){
		damage /= 2;
	}
	if (player.gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
		damage *= 2;
	}
	if( playerIndex == CurrentPlayerIndex ){
		damage = LimitPvMMeleeDamage( playerIndex, damage ); // limits max damage by clvl*256
		monster.CurrentLife -= damage; // здесь отнимается жизнь у ударяемого монстра
	}
	if( !isThorns ){
		if (!HasTrait(playerIndex, TraitId::Bloodless)) {
			EffectAction& lS = player.effectActionValue[EA_LIFE_STEAL];
			int lifeSteal = 64 * RngFromRange(lS.minVal, lS.maxVal);
			player.CurLife += lifeSteal;
			player.BaseLife += lifeSteal;
		}
		EffectAction& mS = player.effectActionValue[EA_MANA_STEAL];
		int manaSteal = 64 * RngFromRange(mS.minVal, mS.maxVal);
		player.CurMana += manaSteal;
		player.BaseMana += manaSteal;
		
		player.CurLife += 64*(PerkValue(SYNERGY_BLOOD_THIEVERY, playerIndex, 0)); LimitToMax(player.CurLife, player.MaxCurLife);
		player.BaseLife += 64*(PerkValue(SYNERGY_BLOOD_THIEVERY, playerIndex, 0)); LimitToMax(player.BaseLife, player.MaxBaseLife);
		player.CurMana += 64*(PerkValue(SYNERGY_BLOOD_THIEVERY, playerIndex, 1)); LimitToMax(player.CurMana, player.MaxCurMana);
		player.BaseMana += 64*(PerkValue(SYNERGY_BLOOD_THIEVERY, playerIndex, 1)); LimitToMax(player.BaseMana, player.MaxBaseMana);
		
		if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
			int NK_flat_life_leech = 64*(1 + PerkValue(PERK_LEECH_LIFE, playerIndex));
			player.CurLife += NK_flat_life_leech; LimitToMax(player.CurLife, player.MaxCurLife);
			player.BaseLife += NK_flat_life_leech; LimitToMax(player.BaseLife, player.MaxBaseLife);
			int NK_flat_mana_leech = 64*(1 + PerkValue(PERK_LEECH_MANA, playerIndex));
			player.CurMana += NK_flat_mana_leech; LimitToMax(player.CurMana, player.MaxCurMana);
			player.BaseMana += NK_flat_mana_leech; LimitToMax(player.BaseMana, player.MaxBaseMana);
		}
		if( player.manaStealPercent ){
			int restoredMp = ( player.manaStealPercent * damage )  * (100 + PerkValue(PERK_VAMPYRISM, playerIndex)) / 10000;
			LimitToRange( restoredMp, 64 * player.manaStealPercent, ((player.MaxCurMana * player.manaStealPercent) / 20 * (100 + PerkValue(PERK_VAMPYRISM, playerIndex)) / 100));
			player.CurMana  += restoredMp; LimitToMax( player.CurMana, player.MaxCurMana );
			player.BaseMana += restoredMp; LimitToMax( player.BaseMana, player.MaxBaseMana );
		}
		if (!HasTrait(playerIndex, TraitId::Bloodless)) {
			if( player.effectFlag[EA_LIFE_STEAL_RND] ){
				int restoredHp = RNG(damage / 64) + 1;
				player.CurLife += restoredHp; LimitToMax(player.CurLife, player.MaxCurLife);
				player.BaseLife += restoredHp; LimitToMax(player.BaseLife, player.MaxBaseLife);
			}
			if( player.lifeStealPercent ){
				int restoredHp = (player.lifeStealPercent * damage)  * (100 + PerkValue(PERK_BLOODSURGE, playerIndex)) / 10000;
				LimitToRange(restoredHp, 64 * player.lifeStealPercent, ((player.MaxCurLife * player.lifeStealPercent) / 20 * (100 + PerkValue(PERK_BLOODSURGE, playerIndex)) / 100));
				player.CurLife += restoredHp; LimitToMax(player.CurLife, player.MaxCurLife);
				player.BaseLife += restoredHp; LimitToMax(player.BaseLife, player.MaxBaseLife);
			}
		}
		// below is code for new Trait 'Hunger' that allows Shugoki class to leech additional life and mana, with no dependancy on leeching affixes
		if (HasTrait(playerIndex, TraitId::Hunger)) { // currently, it eats of 2% of maximum current life/mana
			if (player.MaxCurLife > 0) {
				int leech_hp = player.MaxCurLife / 100;
				player.CurLife += leech_hp; LimitToMax(player.CurLife, player.MaxCurLife);
				player.BaseLife += leech_hp; LimitToMax(player.BaseLife, player.MaxBaseLife);
			}
			if (player.MaxCurMana > 0) {
				int leech_mana = player.MaxCurMana / 100;
				player.CurMana += leech_mana; LimitToMax(player.CurMana, player.MaxCurMana);
				player.BaseMana += leech_mana; LimitToMax(player.BaseMana, player.MaxBaseMana);
			}
		}
	}
	if( (monster.CurrentLife & ~63) > 0 ){
		if( monster.CurAction != A_15_STONE ){
			CastMonsterCritSplash( monsterIndex, 0, playerIndex );
			if( (player.effectFlag[EA_KNOCK_BACK] ) && !isThorns ){
				KnockbackMonster(monsterIndex, OrientationToTarget( Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, Players[playerIndex].Row, Players[playerIndex].Col ), GameMode == GM_HARD ? 1 : 2 );
			}
			DamageMonsterByPlayer(monsterIndex, playerIndex, damage);
		}else if( damageStone ){
			DamageMonsterByPlayer(monsterIndex, playerIndex, damage);
			monster.CurAction = A_15_STONE;
		}
		return 1;
	}else{
		CastMonsterCritSplash( monsterIndex, 1, playerIndex );
		if( monster.CurAction != A_15_STONE ){
			DrawFloatingDamage( damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex );
			KillMonsterByPlayer( monsterIndex, playerIndex );
			return 1;
		}
		DrawFloatingDamage( damage >> 6, Monsters[monsterIndex].Row, Monsters[monsterIndex].Col, monsterIndex );
		KillMonsterByPlayer( monsterIndex, playerIndex );
		monster.CurAction = A_15_STONE;
		return 1;
	}
}

//----- (0045BE66) --------------------------------------------------------
int __fastcall PvP_Melee( uint attackerPlayerIndex, int targetPlayerIndex )
{
	if( targetPlayerIndex >= 4 ){
		TerminateWithError("PlrHitPlr: illegal target player %d", targetPlayerIndex);
	}
	
	Player& defender = Players[targetPlayerIndex];
	if( defender.notHittableInTransition || defender.notHittableFramesAfterTransition > 0 || defender.activeBuffFlag & BF_1_ETHEREAL ) return 0;
	
	if( attackerPlayerIndex >= 4 ){
		TerminateWithError("PlrHitPlr: illegal attacking player %d", attackerPlayerIndex);
	}
	Player& attacker = Players[attackerPlayerIndex];
	
	
	int toHitRandFactor = RNG(100);
	int basic_AC_divider = 5;
	if (HasTrait(targetPlayerIndex, TraitId::SmallFrame)) {
		basic_AC_divider = 3;
	}
	int hitChance = attacker.CurDexterity + attacker.CharLevel + 25
		- defender.CurDexterity / basic_AC_divider	- defender.ACFromItems - defender.ACFromClass;
	if( attacker.ClassID == PC_5_SAVAGE ){
		hitChance += 35;
	}
	hitChance += attacker.ItemsAddToHitPercents;
	LimitToRange(hitChance, 5, 95);

	int blockRandFactor;
	if( defender.CurAction != PCA_0_STAND && defender.CurAction != PCA_4_ATTACK || !defender.CanBlock ){
		blockRandFactor = 100;
	}else{
		blockRandFactor = RNG(100);
	}
	int blockChance = defender.BlockBonus + defender.CurDexterity + 2 * (defender.CharLevel - attacker.CharLevel);
	blockChance += defender.blockChance;
	LimitToRange( blockChance, 0, 45 );
	if( toHitRandFactor >= hitChance ){
		return 0;
	}
	if( blockRandFactor < blockChance ){
		StartPlayerBlock(targetPlayerIndex, OrientationToTarget(defender.Row, defender.Col, attacker.Row, attacker.Col));
		return 1;
	}
	
	int damageFromItem = RngFromRange( attacker.MinDamageFromItem, attacker.MaxDamageFromItem);
	
	int realDamage = attacker.BaseDamage + attacker.ItemsAddDamage
		+ damageFromItem * attacker.ItemsAddDamagePercents / 100 + damageFromItem;

	int attackerClass = attacker.ClassID;
	if( attackerClass == PC_0_WARRIOR || attackerClass == PC_5_SAVAGE ){
		if( RNG(100) < attacker.CharLevel /3 ){
			realDamage *= 2;
		}
	}else if( attackerClass == PC_3_MONK ){
		realDamage += 16;
		if( RNG(100) < attacker.CharLevel/3 ){
			realDamage *= 4;
		}
	}else if( attackerClass == PC_4_ROGUE )	{
		if( RNG(100) < attacker.CharLevel/3 ){
			realDamage *= 3;
		}
	}
	// was: realDamage = (realDamage / 16) * 64;
	realDamage <<= 6;// conversion to subpoints
	if (GameMode == GM_COLISEUM) {
		//realDamage /= 2;// lets try full damage for now!
	}
	else { 
		realDamage /= 16; 
	}
	if( attackerPlayerIndex == CurrentPlayerIndex ){
        NetSendCmdDamage( 1, targetPlayerIndex, realDamage );
	}
	StartPlayerHit(targetPlayerIndex, realDamage, 0);
	defender.lastAttacker = 1 + attackerPlayerIndex;

	return 1;
}

//----- (th3) -------------------------------------------------------------
void __fastcall KillPlayerByPlayerEffect(int victimIndex, int killerIndex)
{
	if( GameMode != GM_COLISEUM ) return;
	Player& victim = Players[victimIndex]; 
	Player& killer = Players[killerIndex]; 

	
	if (killerIndex == CurrentPlayerIndex) { // check if the current player deserves the experience and gold as a reward
	// calc xp
		i64 xp = victim.CharLevel * victim.CharLevel * victim.CharLevel * victim.CharLevel * 1000;
		i64 levelUpXp = LevelUp(killer.CharLevel) - LevelUp(killer.CharLevel - 1);
		i64 limitXp = i64(levelUpXp / (1 + 2 * killer.CharLevel) + 1);
		LimitToMax(xp, limitXp);

		AddPlayerExperience(killerIndex, xp, victim.Row, victim.Col);
	
	// calc gold
		int gold = victim.CharLevel * 50;
		int levelDiff = victim.CharLevel - killer.CharLevel; // just level difference, no percentage yet
		gold += levelDiff * gold / 10; // +10% of gold reward for every level of clvl difference (so the reward decreases if level difference is negative!)
		LimitToMin(gold, 25); // reward cannot be less the 25 gold piece
	
		// dropping piles of gold
		while( gold > 0 ){
			int goldIndex = CreateTypeItem(victim.Row, victim.Col, 0, IC_11_GOLD, 0, 0, 1);
			if( goldIndex == -1 ) break;
			int goldInCell = std::min(gold, MaxGoldInCell);
			Items[goldIndex].amount = goldInCell;
			gold -= goldInCell;
		}
	}
}

//----- (0045C100) --------------------------------------------------------
bool __fastcall MeleeAttackBarrelObject( int playerIndex, int row, int col )
{
	char objectNumber = ObjectsMap[ row ][ col ];
	int objectIndex = abs(objectNumber) - 1;
	if( Objects[objectIndex].destructable == 1 ){
		DamageDestroyableOnMapObject(playerIndex, objectIndex);
		return true;
	}
	return false;
}

//----- (0045C13D) --------------------------------------------------------
bool __fastcall PM_DoAttack(uint playerIndex)
{
	if (playerIndex >= 4) {
		TerminateWithError("PM_DoAttack: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	Item& leftHand = player.OnBodySlots[IS_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_RightHand];

	if (playerIndex == CurrentPlayerIndex && (!EasyClick || !RButtonEvent)) {
		ShiftPressedEvent = false;
		LButtonEvent = false;
		// в th1 почему то нет проверки на замедленный фрейм (в PM_DoRangedAttack есть), возможно из-за этого такие жалобы на топоры в 1.216 были
		if (player.currentAnimationFrame == player.AttackMainFrame + AutoClickDelay && !player.currentAnimationFrameTick) {
			LButtonEvent = true;
			Repeat_Mouse_LButtonDown();
		}
	}

	// Frame skips according to attack speed affixes
	if( player.currentAnimationFrame == 2 ){ 
		switch( player.attackSpeed ){
		case 4: player.currentAnimationFrame += 3; break;
		case 3: player.currentAnimationFrame += 2 + player.meleeAttackBit++; break; // was RNG(100) < 50
		case 2: player.currentAnimationFrame += 2; break;
		case 1: player.currentAnimationFrame += 1; break;
		}
		player.currentAnimationFrame += HasTrait(playerIndex, TraitId::Zealot); // ZEALOT SPEEDS -1 frame
	}
	// Fechtmeister Warrior skips 1 attack frame
	if( HasTrait(playerIndex, TraitId::Fechtmeister) ){
		if( player.currentAnimationFrame == 7 ) player.currentAnimationFrame += 1;
	}
	if( HasTrait(playerIndex, TraitId::Mamluk) ){		
		if(      player.currentAnimationFrame == 6 ) player.currentAnimationFrame += 1;
		else if( player.currentAnimationFrame == 8 ) player.currentAnimationFrame += 1;
	}
	// programmatic sprite cuts for monks with axe, sword and mace (1.150+)
	if( player.ClassID == PC_3_MONK ){
		if( leftHand.IsReqMet && leftHand.ItemCode == IC_2_AXE || rightHand.IsReqMet && rightHand.ItemCode == IC_2_AXE ){
			if( player.currentAnimationFrame == 8 ) player.currentAnimationFrame += 5; // shugoki: frames 8-12 skip
		}else if( leftHand.IsReqMet && is(leftHand.ItemCode, IC_1_SWORD, IC_4_MACE) || rightHand.IsReqMet && is(rightHand.ItemCode, IC_1_SWORD, IC_4_MACE) ){
			if( player.currentAnimationFrame == 8 ) player.currentAnimationFrame += 1; // kensei: frame 8 skipped
		}
	}
	// Rogue-specific frame skips when she uses axe
	if ( HasTrait(playerIndex, TraitId::Axepertise) && leftHand.ItemCode == IC_2_AXE) {
		if(      player.currentAnimationFrame == 7 ) player.currentAnimationFrame += 1; // axe rogue skips frames 7...
		else if( player.currentAnimationFrame == 9 ) player.currentAnimationFrame += 2; // ... and 9-10
	}
	// dimachaerus attacks 1 frame faster under fury (onslaught)
	if( (player.activeBuffFlag & BF_2_FURY_ACTIVE) && player.fullClassId == PFC_DIMACHAERUS ){ 
		if( player.currentAnimationFrame == 7 ) player.currentAnimationFrame += 1;
	}		
	// Night Kin attack speed boost
	if( player.gameChanger & BIT(GC_9_NIGHT_KIN) ){
		if( player.currentAnimationFrame == 4 ) player.currentAnimationFrame += PerkValue(PERK_RIP_AND_TEAR, playerIndex);
	}

	if (player.currentAnimationFrame == player.AttackMainFrame - 1 && !player.currentAnimationFrameTick) {
		int swingSound = 980;
		if (is(player.fullClassId, PFC_SHUGOKI, PFC_KENSEI)) {
			goto TWOH_SFX;
		}
		else if (is(player.fullClassId, PFC_SHINOBI, PFC_DIMACHAERUS) || HasTrait(playerIndex, TraitId::Fechtmeister) || HasTrait(playerIndex, TraitId::Axepertise)) {
			goto ONEH_SFX;
		}
	}

	if( player.currentAnimationFrame == player.AttackMainFrame - 2 && !player.currentAnimationFrameTick ){
		int swingSound = 980; 
		if( is(player.fullClassId, PFC_GUARDIAN, /*PFC_SHUGOKI,*/ PFC_SAVAGE, PFC_MONK, PFC_BERSERKER/*, PFC_EXECUTIONER*/) /*&& !(player.gameChanger & BIT(GC_9_NIGHT_KIN))*/ 
		 || player.fullClassId == PFC_DRUID && !(player.activeBuffFlag & BF_2_FURY_ACTIVE)
			/*|| HasTrait(playerIndex, TraitId::Axepertise)*/ ) {
			TWOH_SFX:
			if (is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_4_MACE, IC_10_STAFF)) { swingSound = S_1690_2H_SWING_SMALL_01 + RNG(8); }// 1690-1697
			else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE) { swingSound = S_1698 + RNG(6); }// 1698-1703
			else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD) { swingSound = S_1157 + RNG(5); }// 1157-1161
			else { swingSound = S_9_SWING + RNG(2); }// 9 or 10
		}
		else if (
			(is(player.fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_TEMPLAR, /*PFC_SHINOBI,*/ PFC_IRON_MAIDEN, PFC_ROGUE, PFC_THRAEX, PFC_MURMILLO, /*PFC_DIMACHAERUS,*/ PFC_SECUTOR, PFC_EXECUTIONER)
			|| HasTrait(playerIndex, TraitId::Mamluk)) && (!HasTrait(playerIndex, TraitId::Axepertise))
			) {
			ONEH_SFX:
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE || player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE) {
				switch (RNG(7)) {
					//change
				case 1:															swingSound = S_1163;	break;
				case 2:															swingSound = S_1164;	break;
				case 3:															swingSound = S_1166;	break;
				case 4:															swingSound = S_1171;	break;
				//case 5:															swingSound = S_1172;	break;// flawed sound!
				case 5:															swingSound = S_1173;	break;
				case 6:															swingSound = S_1174;	break;
				default:/*0*/													swingSound = S_1175;	break;
				}
			}
			else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD) {
				switch (RNG(5)) {
				case 1:															swingSound = S_1704;	break;
				case 2:															swingSound = S_1705;	break;
				case 3:															swingSound = S_1706;	break;
				//case 4:															swingSound = S_1707;	break;// 1707 sounds bad, excluding from the mix
				case 4:															swingSound = S_1708;	break;
				default:														swingSound = S_1709;	break;
				}
			}
			else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE) { swingSound = S_1698 + RNG(6); }// 1698-1703
			else { swingSound = S_9_SWING + RNG(2); }// 9 or 10
		}
		else {
			if (not(player.fullClassId, PFC_SHUGOKI, PFC_SHINOBI, PFC_KENSEI, PFC_DIMACHAERUS) 
				&& (!HasTrait(playerIndex, TraitId::Fechtmeister))
				&& (!HasTrait(playerIndex, TraitId::Axepertise))) {// these are excluded to prevent double hit sfx
				swingSound = S_9_SWING + RNG(2);// 9 or 10
			}
		}
		if (swingSound != 980) {
			PlayLocalSound(swingSound, player.Row, player.Col);
		}
	}
	if( player.currentAnimationFrame != player.AttackMainFrame || player.currentAnimationFrameTick ){
		if( player.currentAnimationFrame < player.AttackAnimationFrameCount ){
			return false;
		}
		StartStand(playerIndex, player.dir);
		ClearPlrPVars(playerIndex);
		return true;
	}
	int row = player.Row + RowDelta[player.dir];
	int col = player.Col + ColDelta[player.dir];
	uint mapIndex = 112 * row + col;
	int monsterNumber = MonsterMap[ 0 ][ mapIndex ];
	int playerNumber = PlayerMap[ 0 ][ mapIndex ];
	if( monsterNumber ){
		if( MonsterTalking(abs(monsterNumber) - 1) ){
			player.CursorRow = 0;
			return false;
		}
	}

	bool meleeExplosionAllInOne = true;  // elemental type explosion one hit simultaneity
	bool meleeSplashExplosion   = false; // elemental explosion splash hits in melee

	const static int elemEffect[] = { EA_MELEE_FIRE, EA_MELEE_LIGHTNING, EA_MELEE_ARCAN, EA_MELEE_ACID, EA_MELEE_HOLY, EA_MELEE_COLD };
	++player.nextMeleeElement;
	if( player.nextMeleeElement >= countof(elemEffect) || player.nextMeleeElement < 0 ) player.nextMeleeElement = 0;
	
	auto meleeExplosion = [&](){
		for( int i = player.nextMeleeElement, count = 0; count < countof(elemEffect); ++count ){
			if( player.effectFlag[elemEffect[i]] ){
				player.nextMeleeElement = i;
				CastMissile(row, col, elemEffect[i] - EA_MELEE_FIRE + 1, 0, 0, MI_64_WEAPON_ELEMENTAL_DAMAGE, CT_0_PLAYER, playerIndex, 0, 0, 0); 
				if( !meleeExplosionAllInOne ) break;
			}
			if( ++i >= countof(elemEffect) ) i = 0;
		}
	};
	meleeExplosion();

	int flagAttackDone = false;
	if( monsterNumber ){
		flagAttackDone = PvM_Melee(playerIndex, abs(monsterNumber) - 1); // physical damage to monster
//	}else if( playerNumber && !IsPlayerFriendly ){//IsPlayerFriendly is LOCAL. If player 1 hostiles player 2 while player 2 remains friendly, player 1 is forced to do block animation etc. as player 2 shift melee attacks player 1, which is not intended.
	}else if (playerNumber && player.IsPlayerEnemy) {//player.IsPlayerEnemy is force sync'd between all players. In the same case as above, player 1 does NOT do block animation etc.
		flagAttackDone = PvP_Melee(playerIndex, abs(playerNumber) - 1);
	}else if( ObjectsMap[ 0 ][ mapIndex ] > 0 ){
		flagAttackDone = MeleeAttackBarrelObject(playerIndex, row, col);
	}
	
	bool isMonkWithStaff = player.ClassID == PC_3_MONK && is( IC_10_STAFF, leftHand.ItemCode, rightHand.ItemCode );
	bool isAxeInHand = is( IC_2_AXE, leftHand.ItemCode, rightHand.ItemCode );
	bool isMonkWithAxe = player.ClassID == PC_3_MONK && isAxeInHand;
	bool isLeftMaceSword = is( leftHand.ItemCode, IC_4_MACE, IC_1_SWORD );
	bool isRightMaceSword = is( rightHand.ItemCode, IC_4_MACE, IC_1_SWORD );
	bool isTwoWeaponInHand = isLeftMaceSword && isRightMaceSword;
	bool isTwoHandedWeapon = is( EL_2_TWO_HANDED, leftHand.EquippedLocation, rightHand.EquippedLocation );
	// splash damage (sweep attack)
	auto splashDamage = [&](int dir){
		Wrap( dir, 8);
		row = player.Row + RowDelta[dir];
		col = player.Col + ColDelta[dir];
		if( meleeSplashExplosion ) meleeExplosion();
		if( MonsterMap[ row ][ col ] ){
			int monsterIndex = abs(MonsterMap[ row ][ col ]) - 1;
			Monster& monster = Monsters[monsterIndex];
			if( !MonsterTalking(monsterIndex) && monster.PrevRow == row && monster.PrevCol == col ){
				if( PvM_Melee(playerIndex, monsterIndex, true) ){// левая дополнительная атака для двуручного оружия монаха?
					flagAttackDone = true;
				}
			}
		}
	};
	if( isMonkWithStaff || isMonkWithAxe || player.ClassID == PC_5_SAVAGE && (isAxeInHand || isTwoWeaponInHand || isTwoHandedWeapon)
	 || player.fullClassId == PFC_KENSEI && isTwoHandedWeapon && (isLeftMaceSword || isRightMaceSword) ){
		splashDamage(player.dir + 1);
		splashDamage(player.dir - 1);
	}
	int weapon_dur_loss_speed = 35 + PerkValue(PERK_WEAPON_THRIFT, playerIndex);
	if (HasTrait(playerIndex, TraitId::Destroyer)) {
		weapon_dur_loss_speed /= 2;
	}
	if( !flagAttackDone || !WeaponDur(playerIndex, weapon_dur_loss_speed) ){
		if( player.currentAnimationFrame < player.AttackAnimationFrameCount ){
			return false;
		}
	}
	StartStand(playerIndex, player.dir);
	ClearPlrPVars(playerIndex);
	return true;
}

//----- (0045C53F) --------------------------------------------------------
bool __fastcall PM_DoRangedAttack( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoRangedAttack: illegal player %d", playerIndex);
	}

	Player& player = Players[playerIndex];
	int currentAnimationFrame = player.currentAnimationFrame;
	int numberOfArrows = 0;
	if( player.currentAnimationFrame == player.AttackMainFrame && !player.currentAnimationFrameTick ){
		numberOfArrows = 1;
	}
	bool multiArrowFlag = false;
	int autoClickDelay = AutoClickDelay;
	int multiArrowOffset = 0;
	if( player.effectFlag[EA_MULTISHOT] ){
		if( player.currentAnimationFrame == player.AttackMainFrame + 2 && !player.currentAnimationFrameTick ){
			numberOfArrows = 2;
			multiArrowFlag = true;
		}else if( player.currentAnimationFrame == player.AttackMainFrame + 4 && !player.currentAnimationFrameTick && HasTrait(playerIndex, TraitId::Strafer) && player.fullClassId == PFC_SCOUT ){
			numberOfArrows = 2;
			multiArrowFlag = true;
			multiArrowOffset = 1;
		}
	}		
	if( playerIndex == CurrentPlayerIndex && ( ! EasyClick || ! RButtonEvent ) ){
		LButtonEvent = false;
		ShiftPressedEvent = false;
		int autoClickFrame = AutoClickDelay + player.AttackMainFrame;
		if( player.effectFlag[EA_MULTISHOT] ){
			autoClickFrame += 2;
		}
		if( player.currentAnimationFrame == autoClickFrame && !player.currentAnimationFrameTick ){
			LButtonEvent = true;
			Repeat_Mouse_LButtonDown();
		}
	}

	// ranged faster attack modifiers
	if( player.ClassID == PC_3_MONK && currentAnimationFrame == 9 ){
		player.currentAnimationFrame += 3;
	}
	if (HasTrait(playerIndex, TraitId::Mamluk)) {		
		//if(    currentAnimationFrame ==  6 ) player.currentAnimationFrame += 1;
		if(      currentAnimationFrame ==  8 ) player.currentAnimationFrame += 2;
		else if( currentAnimationFrame == 11 ) player.currentAnimationFrame += 4;
	}

	if( ! HasTrait(playerIndex, TraitId::Zealot) ){
		switch( player.attackSpeed ){
		case 4: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1;
				if( currentAnimationFrame == 4 ) player.currentAnimationFrame += 2; break;
		case 3: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1;
				if( currentAnimationFrame == 5 ) player.currentAnimationFrame += 1; break;
		case 2: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1;
				if( currentAnimationFrame == 5 && player.rangeAttackBit++ )	player.currentAnimationFrame += 1; break;
		case 1: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1; break;
		}
	}else{ // Zealot speeds
		switch( player.attackSpeed ){
		case 4: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 4; break;
		case 3: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 3; break;
		case 2: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 2;
				if( currentAnimationFrame == 5 && player.rangeAttackBit++ ) player.currentAnimationFrame += 1; break;
		case 1: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1;
				if( currentAnimationFrame == 5 ) player.currentAnimationFrame += 1; break;
		case 0: if( currentAnimationFrame == 2 ) player.currentAnimationFrame += 1; break;
		}
	}

	if ((player.activeBuffFlag & BF_2_FURY_ACTIVE) && player.fullClassId == PFC_DIMACHAERUS) { // dimachaerus attacks 1 frame daster under fury (onslaught)
		if (/*player.currentAnimationFrame == 1 || player.currentAnimationFrame == 3 || */player.currentAnimationFrame == 7) {
			++player.currentAnimationFrame;
		}
	}

	for( uchar currentArrowIndex = 0; currentArrowIndex < numberOfArrows; currentArrowIndex++ ){
		int colOffset = 0;
		int rowOffset = 0;
		if( multiArrowFlag ){
			int deltaRow = player.CursorRow - player.Row;
			int deltaCol = player.CursorCol - player.Col;
			int oneOffset;
			int mult = multiArrowOffset + currentArrowIndex / 2 + 1;
			if( currentArrowIndex % 2 ){
				oneOffset = mult;
			}else{
				oneOffset = -mult;
			}
			if( deltaRow < 0 ){
				colOffset = oneOffset;
			}else if( deltaRow > 0 ){
				colOffset = -oneOffset;
			}

			if( deltaCol < 0 ){
				rowOffset = -oneOffset;
			}else if( deltaCol > 0 ){
				rowOffset = oneOffset;
			}
		}

		bool magicArrowExplosionAllInOne = true;  // elemental type explosion one hit simultaneity
		int missile = MI_0_NONE_ARROW;
		int explosionElement = 0;
		int curMissileElement = 0;
		if(      is(IC_21_KNIFE,  player.OnBodySlots[IS_RightHand].ItemCode, player.OnBodySlots[IS_LeftHand].ItemCode) ) missile = MI_139_THROWING_KNIFE;
		else if( is(IC_22_MALLET, player.OnBodySlots[IS_RightHand].ItemCode, player.OnBodySlots[IS_LeftHand].ItemCode) ) missile = MI_141_THROWING_MALLET;
		else if (is(IC_23_PISTOL, player.OnBodySlots[IS_RightHand].ItemCode, player.OnBodySlots[IS_LeftHand].ItemCode) ) missile = MI_143_BULLET; 

		const static int elemEffect [] = { EA_ARROW_FIRE,    EA_ARROW_LIGHTNING,   EA_ARROW_ARCAN,      EA_ARROW_ACID,     EA_ARROW_HOLY,         EA_ARROW_COLD     };
		const static int elemMissile[] = { MI_27_FIRE_ARROW, MI_56_LIGHTING_ARROW, MI_108_ARCANE_ARROW, MI_109_ACID_ARROW, MI_77_HOLY_BOLT_ARROW, MI_126_COLD_ARROW };

		if( numberOfArrows == 1 ) ++player.nextArrowElement;
		if( player.nextArrowElement >= countof(elemEffect) || player.nextArrowElement < 0 ) player.nextArrowElement = 0;

		for( int i = player.nextArrowElement, count = 0; count < countof(elemEffect); ++count ){
			int magicArrowElement = elemEffect[i] - EA_ARROW_FIRE + 1;
			if( player.effectFlag[elemEffect[i]] ){
				if( missile == MI_0_NONE_ARROW ){
					missile = elemMissile[i];
				}else{
					curMissileElement = magicArrowElement;
				}
				if( ! magicArrowExplosionAllInOne ) explosionElement = 1 << (magicArrowElement - 1);
				player.nextArrowElement = i;
				break;
			}
			if( ++i >= countof(elemEffect) ) i = 0;
		}
		if( magicArrowExplosionAllInOne ){
			for( int i = 0; i < countof(elemEffect); ++i ){
				int magicArrowElement = elemEffect[i] - EA_ARROW_FIRE + 1;
				if( player.effectFlag[elemEffect[i]] ){
					explosionElement |= 1 << (magicArrowElement - 1);
				}
			}
		}

		CastMissile( player.Row, player.Col, rowOffset + player.CursorRow, colOffset + player.CursorCol, player.dir, missile, CT_0_PLAYER, playerIndex, 0, 0, 0, {0,0,0,0,0,curMissileElement,0,explosionElement}, 0, multiArrowFlag ? MSF_MULTISHOT : 0 );
		if (currentArrowIndex == 0) {	
			if (HasTrait(playerIndex, TraitId::Pistoleer)){
				int paf_sound = 0;
				switch (player.OnBodySlots[IS_LeftHand].baseItemIndex) {
					case BI_2426_SHORT_PISTOL:													paf_sound = S_1676_LIGHT_PISTOL;		break;
					case BI_2427_SCATTERSHOT_PISTOL:											paf_sound = S_1681_PISTOL_PAF;			break;
					case BI_2428_LIGHT_PISTOL:													paf_sound = S_1687_LIGHT_PISTOL;		break;
					case BI_2429_LONG_BARRELED_PISTOL:											paf_sound = S_1688_LONG_BARRELED;		break;
					case BI_2430_BATTLE_PISTOL:													paf_sound = S_1683_RANGE_RIFLE;			break;
					case BI_2431_HEAVY_DUTY_PISTOL:												paf_sound = S_1675_SHOTGUN_X4;			break;
					case BI_2432_DOUBLE_BARRELED_PISTOL:										paf_sound = S_1682_RIFLE;				break;
					case BI_2433_WAR_PISTOL:													paf_sound = S_1680_HEAVY;				break;
					case BI_2434_TRIPLE_BARRELED_PISTOL:										paf_sound = S_1679_SHOTGUN_STANDARD;	break;
					case BI_2435_QUAD_BARRELED_PISTOL:											paf_sound = S_1684_BIG_PISTOL;			break;
					case BI_2436_BROAD_BARRELED_PISTOL:											paf_sound = S_1685_MID_PISTOL;			break;
					case BI_2437_FIVE_BARRELED_PISTOL:											paf_sound = S_1689_FIVE_BARRELED;		break;
					case BI_2439_SEVEN_BARRELED_PISTOL:											paf_sound = S_1805_7_BARREL;			break;
					case BI_2438_PEPPERBOX_PISTOL:												paf_sound = S_1807_PEPPERBOX;			break;
					case BI_2444_EIGHT_BARRELED_PISTOL:											paf_sound = S_1686_SHOTGUN;				break;
					case BI_2445_MULTI_BARRELED_PISTOL:											paf_sound = S_1806_MULTI_BARREL;		break;
					case BI_2440_GREAT_PISTOL
						or BI_2443_GRIM_PISTOL  
						or BI_2446_INFERNAL_PISTOL:												paf_sound = S_1677_SUPERWEAPON;			break;
					case BI_2441_HELL_FORGED_PISTOL
						or BI_2442_ABYSSAL_PISTOL:												paf_sound = S_1684_BIG_PISTOL;			break;
					// the rest are Anvil rewards (Griswold's Exterminator), for 9 difficulties
					case 2447 
						or 2448 
						or 2449 
						or 2450 
						or 2451 
						or 2452 
						or 2453 
						or 2454 
						or 2455:																paf_sound = S_1678_LONG_BARREL;			break;// griswold's gun
					default:																	paf_sound = S_1675_SHOTGUN_X4;			break;// just for safety
					}
					PlayLocalSound(paf_sound, player.Row, player.Col);
				}
			else if (missile < MI_139_THROWING_KNIFE) {
				PlayLocalSound(multiArrowFlag ? S_12_STING1 : (1710 + RNG(5)), player.Row, player.Col);
			}
		}

		int weapon_dur_loss_speed = 48 + PerkValue(PERK_WEAPON_THRIFT, playerIndex);
		if (HasTrait(playerIndex, TraitId::Destroyer)) {
			weapon_dur_loss_speed /= 2;
		}
		if( WeaponDur(playerIndex, weapon_dur_loss_speed) ){
			StartStand(playerIndex, player.dir);
			ClearPlrPVars(playerIndex);
			return true;
		}
	}

	if( player.currentAnimationFrame >= player.AttackAnimationFrameCount ){
		StartStand(playerIndex, player.dir);
		ClearPlrPVars(playerIndex);
		return true;
	}
	return false;
}

//----- (th2) --------------------------------------------------------
int __fastcall ThrowableMissile( Item* flask )
{
	int spellEffect = MI_0_NONE_ARROW;
  	switch( flask->MagicCode ){
	case MC_55_FLASK_OF_FIRE:			spellEffect = MI_110_FLASK_OF_FIRE; break;
	case MC_56_FLASK_OF_LIGHTNING:		spellEffect = MI_111_FLASK_OF_LIGHTNING; break;
	case MC_57_FLASK_OF_ACID:			spellEffect = MI_112_FLASK_OF_ACID; break;
	case MC_58_FLASK_OF_FIREBOMB:		spellEffect = MI_113_FLASK_OF_FIREBOMB; break;
	case MC_59_FLASK_OF_LIGHTNINGBOMB:	spellEffect = MI_114_FLASK_OF_LIGHTNINGBOMB; break;
	case MC_60_FLASK_OF_ACIDBOMB:		spellEffect = MI_115_FLASK_OF_ACIDBOMB; break;
	case MC_61_FLASK_OF_ARCANEBOMB:		spellEffect = MI_118_FLASK_OF_ARCANEBOMB; break;
	case MC_62_TRAP_OF_ARROWS:			spellEffect = MI_119_TRAP_OF_ARROWS; break;
	}
	return spellEffect;
}

//----- (th2) --------------------------------------------------------
bool __fastcall PM_DoThrow( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoThrow: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	bool needCast = false;
	//if( !player.currentAnimationFrameTick ){ // в th 1.216 сделано с таким общим условием, чтобы 3 раза не проверять
	if( player.currentAnimationFrame == player.AttackMainFrame && !player.currentAnimationFrameTick ){
		needCast = true;
	}
	if( playerIndex == CurrentPlayerIndex && ( ! EasyClick || ! RButtonEvent ) ){
		LButtonEvent = false;
		ShiftPressedEvent = false;
		int autoClickFrame = AutoClickDelay + player.AttackMainFrame;
		if( player.currentAnimationFrame == autoClickFrame && !player.currentAnimationFrameTick ){
			LButtonEvent = true;
			Repeat_Mouse_LButtonDown();
		}
	}
	if( needCast ){
		Item* flaskHand = findThrowableItem( playerIndex );
		if( !flaskHand ){
			return false;
		}
		int spellEffect = ThrowableMissile(flaskHand);
		if( spellEffect ){
			BaseItem& baseItem = BaseItems[flaskHand->baseItemIndex];
			CastMissile( player.Row, player.Col, player.CursorRow, player.CursorCol, player.dir, spellEffect, CT_0_PLAYER, playerIndex, 0, baseItem.FlaskOrTrapCode/* unused? */, 0 );
			if( WeaponDur( playerIndex, 25 ) ){
				StartStand( playerIndex, player.dir );
				ClearPlrPVars( playerIndex );
				return true;
			}
		}
	}
	if( player.currentAnimationFrame >= player.AttackAnimationFrameCount ){
		StartStand(playerIndex, player.dir);
		ClearPlrPVars(playerIndex);
		return true;
	}
	return false;
}

//----- (0045C734) --------------------------------------------------------
void __fastcall ShieldDur( int playerIndex )
{
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	if( (unsigned int) playerIndex >= 4 ){
		TerminateWithError( "ShieldDur: illegal player %d", playerIndex );
	}
	Player& player = Players[ playerIndex ];
	Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
	Item& rightHand = player.OnBodySlots[ IS_RightHand ];

	if (leftHand.ItemCode == IC_5_SHIELD) {
		DamageItem((uint)playerIndex, IS_LeftHand, leftHand, true);
	}
	if (rightHand.ItemCode == IC_5_SHIELD) {
		DamageItem((uint)playerIndex, IS_RightHand, rightHand, true);
	}
}
//----- (0045C810) --------------------------------------------------------
int __fastcall PM_DoBlock( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoBlock: illegal player %d", playerIndex);
	}

	if( EasyClick && playerIndex == CurrentPlayerIndex && MouseButtonPressed == MBP_1_LEFT ){
		if( IsShiftPressed ){
			ShiftPressedEvent = true;
		}else{
			ShiftReleasedEvent = true;
		}
	}

	Player& player = Players[playerIndex];

	if( player.effectFlag[EA_FAST_BLOCK] ){
		if( player.currentAnimationFrame != 1 ){
			player.currentAnimationFrame = player.BlockAnimationFrameCount;
		}
	}

	if( player.currentAnimationFrame < player.BlockAnimationFrameCount ){
		return false;
	}else{
		StartStand(playerIndex, player.dir);
		ClearPlrPVars(playerIndex);
		int wrecking_block_penalty = HasTrait(playerIndex, TraitId::WreckingBlock) ? 5 : 10;
		if( !RNG(wrecking_block_penalty) ){
			ShieldDur(playerIndex);
		}
		return true;
	}
}

//----- (0045C89A) --------------------------------------------------------
int __fastcall PM_DoSpell( uint playerIndex)
{
	if( playerIndex >= 4 ){
		TerminateWithError("PM_DoSpell: illegal player %d", playerIndex);
	}
	Player& player = Players[ playerIndex ];

	// faster casting - skipping frames (decreasing the frame where the spell is cast would work too?)
	switch( player.castSpeed ){
	case 3: if( player.currentActionFrame == 6 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 6 ) ++player.currentAnimationFrame;
	case 2: if( player.currentActionFrame == 4 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 4 ) ++player.currentAnimationFrame;
	case 1: if( player.currentActionFrame == 2 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 2 ) ++player.currentAnimationFrame;
	}

	if( player.gameChanger & BIT(GC_9_NIGHT_KIN) ){
		switch( PerkValue(PERK_DOMINATE, playerIndex) ){
		case 4: if( player.currentActionFrame ==10 ) ++player.currentActionFrame; if( player.currentAnimationFrame ==10 ) ++player.currentAnimationFrame;
		case 3: if( player.currentActionFrame == 8 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 8 ) ++player.currentAnimationFrame;
		case 2: if( player.currentActionFrame == 6 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 6 ) ++player.currentAnimationFrame;
		case 1: if( player.currentActionFrame == 4 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 4 ) ++player.currentAnimationFrame;
		case 0: if( player.currentActionFrame == 2 ) ++player.currentActionFrame; if( player.currentAnimationFrame == 2 ) ++player.currentAnimationFrame;
		}
	}

	if( (true || ! EasyClick) && playerIndex == CurrentPlayerIndex /*&& ! LButtonEvent*/ && player.currentAnimationFrame == player.SpellAnimationActionFrameIndex + AutoClickDelay ){
		Repeat_Mouse_RButtonDown();
	}

	// Update cursor position if double click on the same tile before 1st Teleport action completes.
	// believe updating cursor position at Frame 1 is better than at Frame player.SpellAnimationActionFrameIndex, as the latter may cause MP desync.
	if(
		#if ! SPELL_QUEUE_AND_COOLDOWN_FIX
		player.spellIndex == PS_23_TELEPORT &&
		#endif
		player.currentActionFrame == 1 && player.PrevCursorX == player.CursorRow && player.PrevCursorY == player.CursorCol ){
		player.CursorRow += player.Row - player.PrevRow;
		player.CursorCol += player.Col - player.PrevCol;
	}

	if( player.currentActionFrame == player.SpellAnimationActionFrameIndex ){
		CastPlayerSpell(playerIndex, player.spellIndex, player.Row, player.Col, player.CursorRow, player.CursorCol, 0, player.HorizontalNextRowOrSomeSpellData); // TODO: check if all spellIndex passed in packet to avoid spell desync
		if( player.readySpellTargetType == SO_0_SKILL ){
			if( player.SpellType == SO_2_RELIC && !(player.AvailableRelictMask & (1i64 << (player.CurrentSpellIndex - 1))) ){
				player.CurrentSpellIndex = -1;
				player.SpellType = SO_4_NONE;
			}
			if( player.SpellType == SO_3_EQUIPED_ITEM &&  !(player.AvailableChargesMask & (1i64 << (player.CurrentSpellIndex - 1))) ){
				player.CurrentSpellIndex = -1;
				player.SpellType = SO_4_NONE;
			}
		}
	}
	++player.currentActionFrame;
	if( Dungeon->genType || Dungeon->isQuest ){
		if( player.currentAnimationFrame >= player.SpellAnimationFrameCount ){
			StartStand( playerIndex, player.dir );
			ClearPlrPVars(playerIndex);
			return 1;
		}
	}else{
		if( player.currentActionFrame > player.SpellAnimationFrameCount ){
			StartWalkStand(playerIndex);
			if( SpellAnimInTown ) StartStand( playerIndex, player.dir ); //Avoid spell animation loop for warrior classes once spell animation is enabled in town
			ClearPlrPVars(playerIndex);
			return 1;
		}
	}
	return 0;
}

//----- (0045CA11) --------------------------------------------------------
int __fastcall PM_DoGotHit( int playerIndex )
{
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PM_DoGotHit: illegal player %d", playerIndex);
	}

	if( EasyClick && playerIndex == CurrentPlayerIndex && MouseButtonPressed == MBP_1_LEFT ){
		if( IsShiftPressed ){
			ShiftPressedEvent = true;
		}else{
			ShiftReleasedEvent = true;
		}
	}

	Player& player = Players[playerIndex];

	if( player.hitRecovery ){
		int nextFrameAfterRecovery = 2 + player.hitRecovery;
		if( player.currentActionFrame > 1 && player.currentActionFrame < nextFrameAfterRecovery ){
			player.currentActionFrame = nextFrameAfterRecovery;
		}
		if( player.currentActionFrame > player.HitRecoveryAnimationFrameCount ){
			player.currentActionFrame = player.HitRecoveryAnimationFrameCount;
		}
	}

	if( player.currentActionFrame >= player.HitRecoveryAnimationFrameCount ){
		PlayersSpellcastQueue[playerIndex] = {}; // clear std::queue
		StartStand(playerIndex, player.dir);
		ClearPlrPVars(playerIndex);
		if( int r = RNG(3) ){
			ArmorDur((uint)playerIndex, r);
		}
		return true;
	}else{
		player.currentActionFrame++;
		return false;
	}
}

ITEM_SLOT DurableArmor[] = { IS_Head, IS_Torso, IS_Torso, IS_Waist, IS_Hands, IS_Feet };
// INDESTRUBLE отключен в свойствах вешей и в коде в TH 1.216
//----- (0045CADF) --------------------------------------------------------
void __fastcall ArmorDur( uint playerIndex, int durDecrParam )
{
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	if( playerIndex >= 4 ){
		TerminateWithError("ArmorDur: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	
	Item& body = player.OnBodySlots[IS_Torso]; // тут сделать выбор по списку, и красным пока рисовать броник для новых слотов
	Item& head = player.OnBodySlots[IS_Head];
	
	ITEM_SLOT damagedArmorIndex = IS_Head;
	Item* damagedArmor = 0;
	int count = 0;
	ITEM_SLOT durableArmor[countof(DurableArmor)];
	for( int i = 0; i < countof(DurableArmor); ++i ){
		if( player.OnBodySlots[ DurableArmor[ i ] ].ItemCode != IC_M1_NONE ){
			durableArmor[count++] = DurableArmor[ i ];
		}
	}
	if( count ){
		damagedArmorIndex = durableArmor[RNG(count)];
		damagedArmor = &player.OnBodySlots[ damagedArmorIndex ];
	}
	if (damagedArmor) {
		DamageItem(playerIndex, damagedArmorIndex, *damagedArmor, true);
	}
}

//----- (0045CBA4) --------------------------------------------------------
bool __fastcall PM_DoDeath( int playerIndex )
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("PM_DoDeath: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];

	if( player.currentActionFrame >= 2*player.DeathAnimationFrameCount ){
		if( DeathDelayTimer > 1 ){
			if( playerIndex == CurrentPlayerIndex ){
				DeathDelayTimer--;
				if( DeathDelayTimer == 1 ){
					IsPlayerDead = 1;
					#if th2_crucifixion
					if( NetPlayerCount == 1 ){
						SendCmd4( 1, NC_32_START_RESSURECT_TIMER, CurrentPlayerIndex );
					}
					#endif
					if( MaxCountOfPlayersInGame == 1 ){
						SelectMainMenu();
					}
				}
			}
		}
		player.currentAnimationDelay = 10000;
		player.currentAnimationFrame = player.currentAnimationFramesCount;
		FlagMap[ player.Row ][ player.Col ] |= CF_4_DEAD_PLAYER;// Труп игрока?
		PlayersSpellcastQueue[playerIndex] = {}; //clear spell queue on player's death
		PlayerWall[playerIndex] = {};
	}
	if( player.currentActionFrame < 100 ){
		player.currentActionFrame++;
	}
	return 0;
}

//----- (0045CC67) --------------------------------------------------------
void __fastcall CheckNewPath( int playerIndex )
{
	int orientation = 0;    
	int distanceToTargetY;
	int targetRow;
	int targetCol;
	int distanceToTargetX;
	if( (uint)playerIndex >= 4 ) TerminateWithError("CheckNewPath: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	Player& playerTarget = Players[player.destParam1];
	Monster& monsterTarget = Monsters[player.destParam1];
	Towner& towner = Towners[player.destParam1];
	Object& objectOnMapTarget = Objects[player.destParam1];
	Item& itemOnGroundTarget = Items[player.destParam1];

	if( player.fullClassId == PFC_DRUID && player.activeBuffFlag & BF_3_FURY_LETARGY ) return;

	if (player.destAction == PCMD_20_MELEE_ATTACK_TO_MONSTER) {// Если следующее действие - мили нападение на монcтра, создаём путь до него 
		MakePlayerPath(playerIndex, monsterTarget.NextRow, monsterTarget.NextCol, 0);
	}else if (player.destAction == PCMD_21_MELEE_ATTACK_TO_PLAYER) {// Если следующее действие атаковать вблизи выбранного игрока. Даже точнее идти туда куда выбранный игрок направляется
		MakePlayerPath(playerIndex, playerTarget.NextRow, playerTarget.NextCol, 0);
	}
	//Xbox Controller Map
	XinputHandleControllerForPlayerAction(playerIndex);
	
	if( Players[playerIndex].walkPath[0] != PCMD_M1_NOTHING ){
		if( player.CurAction == PCA_0_STAND ){
			if( playerIndex == CurrentPlayerIndex && is(player.destAction, PCMD_20_MELEE_ATTACK_TO_MONSTER, PCMD_21_MELEE_ATTACK_TO_PLAYER) ){
				if( player.destAction == PCMD_20_MELEE_ATTACK_TO_MONSTER ){
					distanceToTargetX = abs(player.NextRow - Monsters[player.destParam1].NextRow);
					distanceToTargetY = abs(player.NextCol - Monsters[player.destParam1].NextCol);
					orientation = OrientationToTarget(player.NextRow, player.NextCol, Monsters[player.destParam1].NextRow, Monsters[player.destParam1].NextCol);
				}else{ // PCMD_21_MELEE_ATTACK_TO_PLAYER
					distanceToTargetX = abs(player.NextRow - Players[player.destParam1].NextRow);
					distanceToTargetY = abs(player.NextCol - Players[player.destParam1].NextCol);
					orientation = OrientationToTarget(player.NextRow, player.NextCol, Players[player.destParam1].NextRow, Players[player.destParam1].NextCol);
				}
				if( distanceToTargetX < 2 && distanceToTargetY < 2 ){
					ClearPlayerPath(playerIndex);
					if( not(monsterTarget.speechIndex, SP_0, SP_36) ){
						TalkWithMonster(player.destParam1);
					}else{
						StartAttack(playerIndex, orientation);
					}
					player.destAction = PCMD_M1_NOTHING;
				}
			}
			int horVel = 2048;
			int xVel   = 1024;
			int yVel   = 512;
			if( Dungeon != DUN_0_TOWN ){ // TODO: check for ravenholm values
				horVel = ClassVel[player.ClassID].horVel;
				xVel   = ClassVel[player.ClassID].xVel;
				yVel   = ClassVel[player.ClassID].yVel;
			}
			switch( player.walkPath[0] ){
			case PCMD_1_WALKUR:	StartWalkUp        (playerIndex,    xVel, -yVel,            0, -1,       DIR_5_UR, SDIR_2_UR); break;
			case PCMD_2_WALKUL:	StartWalkUp        (playerIndex,   -xVel, -yVel,           -1,  0,       DIR_3_UL, SDIR_8_UL); break;
			case PCMD_3_WALKDR:	StartWalkDown      (playerIndex,    xVel,  yVel, -32, -16,  1,  0,       DIR_7_DR, SDIR_4_DR); break;
			case PCMD_4_WALKDL:	StartWalkDown      (playerIndex,   -xVel,  yVel,  32, -16,  0,  1,       DIR_1_DL, SDIR_6_DL); break;
			case PCMD_5_WALKU :	StartWalkUp        (playerIndex,       0, -xVel,           -1, -1,       DIR_4_U,  SDIR_1_U ); break;
			case PCMD_6_WALKR :	StartWalkHorizontal(playerIndex,  horVel,     0, -32, -16,  1, -1, 1, 0, DIR_6_R,  SDIR_3_R ); break;
			case PCMD_7_WALKD :	StartWalkDown      (playerIndex,       0,  xVel,   0, -32,  1,  1,       DIR_0_D,  SDIR_5_D ); break;
			case PCMD_8_WALKL :	StartWalkHorizontal(playerIndex, -horVel,     0,  32, -16, -1,  1, 0, 1, DIR_2_L,  SDIR_7_L ); break;
			}
			for( int n = 1; n < MaxPathStep_25; ++n ){ // shift step queue
				player.walkPath[n - 1] = player.walkPath[n];
			}
			player.walkPath[MaxPathStep_25 - 1] = PCMD_M1_NOTHING;
			// if walk fails then start standing
			if( player.CurAction == PCA_0_STAND ){
				StartStand(playerIndex, player.dir);
				player.destAction = PCMD_M1_NOTHING;
			}
		}
	}else{
		if( player.destAction != PCMD_M1_NOTHING ){
			if( player.CurAction == PCA_0_STAND ){
				switch( player.destAction ){
				case PCMD_9_MELEE_ATTACK_TO_NONE:/* Атака только ближняя при зажатом шифте*/
					StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, player.destParam1, player.destParam2));
					break;
				case PCMD_20_MELEE_ATTACK_TO_MONSTER:// Ближняя атака монстра
					distanceToTargetX = abs(player.Row - monsterTarget.NextRow);
					distanceToTargetY = abs(player.Col - monsterTarget.NextCol);
					if( distanceToTargetX < 2 && distanceToTargetY < 2 ){ // near
						orientation = OrientationToTarget(player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol);
						if( not(monsterTarget.speechIndex, SP_0, SP_36) ){
							TalkWithMonster(player.destParam1);// Квестовая болтовня с монстрами
						}else{
							StartAttack(playerIndex, orientation);
						}
					}
					break;
				case PCMD_21_MELEE_ATTACK_TO_PLAYER:// Ближняя атака игрока
					distanceToTargetX = abs(player.Row - playerTarget.NextRow);
					distanceToTargetY = abs(player.Col - playerTarget.NextCol);
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
						break;
					}
					StartAttack(playerIndex, OrientationToTarget(player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol));
					break;
				case PCMD_10_RANGE_ATTACK_TO_NONE:/* Дальняя Атака с шифтом (луком)*/
					StartRangeAttack(playerIndex, OrientationToTarget(player.Row, player.Col, player.destParam1, player.destParam2), player.destParam1, player.destParam2);				break;
				case PCMD_22_RANGE_ATTACK_TO_MONSTER:/* Дальняя атака без шифта (луком) монстра */
					orientation = OrientationToTarget(player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol);
					if( not(monsterTarget.speechIndex, SP_0, SP_36) ){
						TalkWithMonster(player.destParam1);// Какая-то квестовая болтовня
					}else{
						StartRangeAttack(playerIndex, orientation, monsterTarget.NextRow, monsterTarget.NextCol);
					}
					break;
				case PCMD_23_RANGE_ATTACK_TO_PLAYER:/* Дальняя атака (луком) игрока. С шифтом или без нет возможности проверить*/
					StartRangeAttack(playerIndex, OrientationToTarget(player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol), playerTarget.NextRow, playerTarget.NextCol);			break;
				case PCMD_12_CAST_SPELL_TO_NONE:// Каст магии. Кроме стен
					StartSpell(playerIndex, OrientationToTarget(player.Row, player.Col, player.destParam1, player.destParam2), player.destParam1, player.destParam2);
					player.HorizontalNextRowOrSomeSpellData = player.destParam3; // spell level calculated at rmb click
					PlayersSpellcastQueue[playerIndex].push(player.spellIndex);
					break;
				case PCMD_24_CAST_SPELL_TO_MONSTER:// Каст магии в монстра
					if( Dungeon->genType || Dungeon->isQuest ){
						StartSpell( playerIndex, OrientationToTarget( player.Row, player.Col, monsterTarget.NextRow, monsterTarget.NextCol ), monsterTarget.NextRow, monsterTarget.NextCol );
					}else{	
						// в городе горожане помещаются в MonsterMap, и оттуда же берется index для Towners в DispatchCursorAboveIsoland если DungeonType город
						StartSpell( playerIndex, OrientationToTarget( player.Row, player.Col, towner.Row, towner.Col ), towner.Row, towner.Col );
					}
					player.HorizontalNextRowOrSomeSpellData = player.destParam2; // spell level calculated at rmb click
					PlayersSpellcastQueue[playerIndex].push(player.spellIndex);
					break;
				case PCMD_25_CAST_SPELL_TO_PLAYER:// Каст магии в другого игрока
					StartSpell(playerIndex, OrientationToTarget(player.Row, player.Col, playerTarget.NextRow, playerTarget.NextCol), playerTarget.NextRow, playerTarget.NextCol);
					player.HorizontalNextRowOrSomeSpellData = player.destParam2; // spell level calculated at rmb click
					PlayersSpellcastQueue[playerIndex].push(player.spellIndex);
					break;
				case PCMD_26_CAST_NOT_ORIENTED_SPELL:// Каст стен. В монстра или нет без разницы
					StartSpell(playerIndex, player.destParam3, player.destParam1, player.destParam2);
					player.newDirection = player.destParam3;
					player.HorizontalNextRowOrSomeSpellData = player.destParam4; // spell level calculated at rmb click
					PlayersSpellcastQueue[playerIndex].push(player.spellIndex);
					break;
				case PCMD_13_OBJECT_ACTIVATE:// Взаимодействие с предметами на карте. Сундуки, саркофаги, бочки, двери и т.п.
					distanceToTargetX = abs(player.Row - objectOnMapTarget.Row);
					distanceToTargetY = abs(player.Col - objectOnMapTarget.Col);
					if( distanceToTargetY > 1 ){// Возможно поправка на двухклеточные объекты. Например если саркофаг то активировать его за ближний тайл даже если кликнули на дальний. Пока не работает, ибо обозначение на карте саркофагов кривое.
						if( ObjectsMap[ Objects[ player.destParam1 ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.destParam1 ){
							distanceToTargetY = abs(player.Col - objectOnMapTarget.Col + 1);
						}
					}
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else if( objectOnMapTarget.destructable == 1 ){// Атака беззащитной бочки
						StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col));
					}else{
						OperateObject(playerIndex, player.destParam1, 0);// Дверь, алтарь, сундук и т.п. В общем всё кроме бочек
					}
					break;
				case PCMD_14_OBJECT_SAFE_ACTIVATE:// Тоже интерактивные предметы только вот не нашёл пока ни одного. Возможно это рычаги, трупы на крестах в гробнице леорника, пьедестал с посохом лазаря...
					distanceToTargetX = abs(player.Row - objectOnMapTarget.Row);
					distanceToTargetY = abs(player.Col - objectOnMapTarget.Col);
					distanceToTargetY = distanceToTargetY;
					if( distanceToTargetY > 1 ){
						if( ObjectsMap[ Objects[player.destParam1 ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.destParam1 ){
							distanceToTargetY = abs(player.Col - objectOnMapTarget.Col + 1);
						}
					}
					if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
					}else if( objectOnMapTarget.destructable == 1 ){
						StartAttack(playerIndex, OrientationToTarget(player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col));
					}else{
						DisarmTrapFix(playerIndex, player.destParam1);
						OperateObject(playerIndex, player.destParam1, 0);
					}
					break;
				case PCMD_18_ACTIVATE_OBJECT_WITH_TELEKINES:
					if( objectOnMapTarget.destructable != 1 ){// Не бочка
						OperateObject(playerIndex, player.destParam1, 1);
					}
					break;
				case PCMD_15_GET_ITEM_FROM_LAND_ON_CURSOR:
					if( playerIndex == CurrentPlayerIndex ){
						distanceToTargetX = abs(player.Row - itemOnGroundTarget.MapRow);
						distanceToTargetY = abs(player.Col - itemOnGroundTarget.MapCol);
						if( distanceToTargetX <= 1 && distanceToTargetY <= 1 && Cur.GraphicsID == CM_1_NORMAL_HAND && !Items[player.destParam1].gottenFromLand ){
							bool portalUnderLegs = false;
							for( int i = 0; i < MissileAmount; i++ ){
								Missile& tp = Missiles[i];
								if (tp.BaseMissileIndex == MI_10_TOWN_PORTAL && tp.Row == player.Row && tp.Col == player.Col) {
									portalUnderLegs = true;
									break;
								}
							}
							if( portalUnderLegs ){
								NetSendCmdGItem(1, CMD_40_REQUESTAGITEM, CurrentPlayerIndex, CurrentPlayerIndex, player.destParam1);
							}else{
								NetSendCmdGItem(1, CMD_39_REQUESTGITEM, CurrentPlayerIndex, CurrentPlayerIndex, player.destParam1);
								Items[player.destParam1].gottenFromLand = 1;
							}
						}
					}
					break;
				case PCMD_16_GET_ITEM_FROM_LAND_IN_INVENTORY:
					if( playerIndex == CurrentPlayerIndex ){
						distanceToTargetX = abs(player.Row - itemOnGroundTarget.MapRow);
						distanceToTargetY = abs(player.Col - itemOnGroundTarget.MapCol);
						if( distanceToTargetX <= 1 && distanceToTargetY <= 1 && Cur.GraphicsID == CM_1_NORMAL_HAND ){
							NetSendCmdGItem( 1, CMD_40_REQUESTAGITEM, CurrentPlayerIndex, CurrentPlayerIndex, player.destParam1 );
						}
					}
					break;
				case PCMD_17_TOWNER_DIALOG:// Болтовня с неписями
					if( playerIndex == CurrentPlayerIndex ){
						TalkToTowner(playerIndex, player.destParam1);
					}
					break;
				}
				FixPlayerLocation(playerIndex, player.dir);
				player.destAction = PCMD_M1_NOTHING;
			}else{
				if( player.CurAction == PCA_4_ATTACK && player.currentAnimationFrame - AdjFrameQ_14_15() > player.AttackMainFrame ){// Действия в упор
					switch( player.destAction ){
					case PCMD_9_MELEE_ATTACK_TO_NONE:
						StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, player.destParam1, player.destParam2 ) );
						player.destAction = PCMD_M1_NOTHING;
						break;
					case PCMD_20_MELEE_ATTACK_TO_MONSTER:
						distanceToTargetX = abs( player.Row - monsterTarget.NextRow );
						distanceToTargetY = abs( player.Col - monsterTarget.NextCol );
						if( distanceToTargetX > 1 || distanceToTargetY > 1 ){
						}else{
							StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, monsterTarget.NextRow, monsterTarget.NextCol ) );
						}
						player.destAction = PCMD_M1_NOTHING;
						break;
					case PCMD_21_MELEE_ATTACK_TO_PLAYER:
						distanceToTargetX = abs( player.Row - playerTarget.NextRow );
						distanceToTargetY = abs( player.Col - playerTarget.NextCol );
						if( distanceToTargetX < 2 && distanceToTargetY < 2 ){
							StartAttack( playerIndex, OrientationToTarget( player.NextRow, player.NextCol, playerTarget.NextRow, playerTarget.NextCol ) );
						}
						player.destAction = PCMD_M1_NOTHING;
						break;
					case PCMD_13_OBJECT_ACTIVATE:
						distanceToTargetX = abs( player.Row - objectOnMapTarget.Row );
						distanceToTargetY = abs( player.Col - objectOnMapTarget.Col );
						if( distanceToTargetY > 1 && ObjectsMap[ Objects[ player.destParam1 ].Row ][ objectOnMapTarget.Col - 1 ] == -1 - player.destParam1 ){
							distanceToTargetY = abs( player.Col - objectOnMapTarget.Col + 1 );
						}
						if( distanceToTargetX < 2 && distanceToTargetY < 2 ){
							if( objectOnMapTarget.destructable == 1 ){ // enum { OBJ_BREAKABLE = 1, }
								StartAttack( playerIndex, OrientationToTarget( player.Row, player.Col, objectOnMapTarget.Row, objectOnMapTarget.Col ) );
							}else{
								OperateObject( playerIndex, player.destParam1, 0 );
							}
						}
						break;
					}
				}
				if( player.CurAction == PCA_5_RANGE_ATTACK && player.currentAnimationFrame - AdjFrameQ_14_15() > player.AttackMainFrame ){// Стрельба 
					bool rangedAttack = true;
					switch( player.destAction ){
					case PCMD_10_RANGE_ATTACK_TO_NONE:    targetRow = player.destParam1;     targetCol = player.destParam2;     break;
					case PCMD_22_RANGE_ATTACK_TO_MONSTER: targetRow = monsterTarget.NextRow; targetCol = monsterTarget.NextCol; break;
					case PCMD_23_RANGE_ATTACK_TO_PLAYER:  targetRow = playerTarget.NextRow;  targetCol = playerTarget.NextCol;  break;
					default: rangedAttack = false; break;
					}
					if( rangedAttack ){
						orientation = OrientationToTarget(player.Row, player.Col, targetRow, targetCol);
						StartRangeAttack(playerIndex, orientation, targetRow, targetCol);
						player.destAction = PCMD_M1_NOTHING;
					}
				}else if( player.CurAction == PCA_9_SPELL && player.currentAnimationFrame - AdjFrameQ_14_15() > player.SpellAnimationActionFrameIndex // Каст магии
				 #if SPELL_QUEUE_AND_COOLDOWN_FIX
				 && !CooldownCheck(playerIndex, player.spellIndex) /* This further reduces the chance of double cast cooldown desync in MP */
				 #endif
					){ 
					bool spellCast = true;
					switch( player.destAction ){
					case PCMD_12_CAST_SPELL_TO_NONE:
						targetRow = player.destParam1;
						targetCol = player.destParam2;
						player.HorizontalNextRowOrSomeSpellData = player.destParam3; // ? player.HorizontalNextRowOrSomeSpellData = slvl is already set inside StartSpell()
						break;
					case PCMD_24_CAST_SPELL_TO_MONSTER:
						if( Dungeon->genType || Dungeon->isQuest ){
							targetRow = monsterTarget.NextRow;
							targetCol = monsterTarget.NextCol;
						} else {
							targetRow = towner.Row;
							targetCol = towner.Col;
						}
						player.HorizontalNextRowOrSomeSpellData = player.destParam2; // ? player.HorizontalNextRowOrSomeSpellData = slvl is already set inside StartSpell()
						break;
					case PCMD_25_CAST_SPELL_TO_PLAYER:
						targetRow = playerTarget.NextRow;
						targetCol = playerTarget.NextCol;
						player.HorizontalNextRowOrSomeSpellData = player.destParam2; // ? player.HorizontalNextRowOrSomeSpellData = slvl is already set inside StartSpell()
						break;
					case PCMD_26_CAST_NOT_ORIENTED_SPELL:
						targetRow = player.destParam1;
						targetCol = player.destParam2;
						player.newDirection = player.destParam3;
						player.HorizontalNextRowOrSomeSpellData = player.destParam4; // ? player.HorizontalNextRowOrSomeSpellData = slvl is already set inside StartSpell()
						break;
					default:
						spellCast = false;
						break;
					}
					if( spellCast ){
						#if SPELL_QUEUE_AND_COOLDOWN_FIX
						// Fix for wrong player/missile orientation when player walking action is immediately followed by Teleporting and RMB spamming another spell (cursor NOT moved).
						// In this edge case, player.CurAnimFrameCount = player.currentAnimationFrame + 1, and player's coordinate differs from target coordinate by at most 1 tile.
						// Note that player.PrevCursorX == player.CursorX && player.PrevCursorY == player.CursorY is true after Teleport completion.
						if( abs(targetRow - player.Row) <= 1 && abs(targetCol - player.Col) <= 1 && player.PrevCursorX == player.CursorRow && player.PrevCursorY == player.CursorCol ){
							// This modification should be mutually exclusive with the latter condition:
							// if( player.CurAnimFrameCount == 1 && player.PrevCursorX == player.CursorX && player.PrevCursorY == player.CursorY ){
							// since player.CursorX is updated by targetRow inside StartSpell()
							targetRow += player.Row - player.PrevRow;
							targetCol += player.Col - player.PrevCol;
						}
						#endif
						orientation = OrientationToTarget(player.Row, player.Col, targetRow, targetCol);
		
						#if SPELL_QUEUE_AND_COOLDOWN_FIX
						// Fix for the case where player RMB spams a spell (e.g. Arcane Star), then switch to a XYD type spell (e.g. Fire Wall),
						// resulting in the Fire Wall cast in the wrong direction.
						player.newDirection = orientation;
						#endif
		
						StartSpell(playerIndex, orientation, targetRow, targetCol);
		
						#if SPELL_QUEUE_AND_COOLDOWN_FIX
						// If spell animation is disabled in town, do NOT queue if player spams spells (e.g. Healing, Heal Others) in town.
						// This will get queued here and not correctly removed from queue via CastPlayerSpell(),
						// and will mess up all queued spells catastrophically even after player goes to Dungeon.
						// To avoid this, enable spell animation in town inside StartSpell()
						if( (player.dungeon || Dungeon->isQuest || SpellAnimInTown) && !player.lvlChanging ){
							// Fix for the case where player RMB spams a spell, then switch to a different spell and RMB clicks once, resulting in the new spell being cast twice.
							// Most noticeable examples: Fire Wall, Teleport, Healing.
							PlayersSpellcastQueue[playerIndex].push(player.spellIndex);
						}
						#endif
						player.destAction = PCMD_M1_NOTHING;
					}
				}
			}
		}
	}
}

//----- (0045D86B) --------------------------------------------------------
bool __fastcall PlrDeathModeOK( int playerIndex )
{
	if( playerIndex != CurrentPlayerIndex ){
		return true;
	}
	if( (unsigned int)playerIndex >= 4 ){
		TerminateWithError("PlrDeathModeOK: illegal player %d", playerIndex);
	}

	uchar playerCurAction = Players[playerIndex].CurAction;
	if( playerCurAction != PCA_8_DEATH
		&& playerCurAction != PCA_11_QUIT 
		&& playerCurAction != PCA_10_NEW_LEVEL ){
			return false;
	}
	return true;
}

//----- (0045D8AF) --------------------------------------------------------
void ValidateCurPlayer( bool forceItemsRecalc )
{
	Player& player = Players[ CurrentPlayerIndex ];

	if( CurrentPlayerIndex >= 4 ){
		TerminateWithError( "ValidatePlayer: illegal player %d", CurrentPlayerIndex );
	}
	
	// Prevent random generator from damage
    auto randUsingCounter = RandUsingCounter;
    auto randomSeed = RandomSeed;
    auto lastSeed = LastSeed;
	
	if( player.CharLevel > PlayerMaxLevel() ){
		player.CharLevel = PlayerMaxLevel();
	}
	if( player.Xp > player.NextLevelUp ){
		player.Xp = player.NextLevelUp;
	}
	int totalGold = 0;
	for( int i = 0; i < player.InvItemCount; ++i ){
		if( player.InventorySlots[i].ItemCode == IC_11_GOLD ){
			if( player.InventorySlots[i].amount > DoubleMaxGoldInCell ){
				player.InventorySlots[i].amount = DoubleMaxGoldInCell;
			}
			totalGold += player.InventorySlots[i].amount;
		}
	}
	if( player.TotalGold != totalGold ){
		player.TotalGold = totalGold;
	}
	int classId = player.ClassID;
	const PlayerStat maxStat = GetPlayerMaxStat( CurrentPlayerIndex );
	LimitToRange(player.BaseStrength, 0, maxStat.Strength);
	LimitToRange(player.BaseMagic, 0, maxStat.Magic );
	LimitToRange(player.BaseDexterity, 0, maxStat.Dexterity );
	LimitToRange(player.BaseVitality, 0, maxStat.Vitality );
	__int64 mask = 0;
	for( int i = 1; i < PS_COUNT; ++i ){
		if( Spells[i].BookQuality != -1 ){
			mask |= 1i64 << (i-1);
			LimitToMax( player.SpellLevels[i], SpellLevelBookMax() );
		}
	}
	if( !(FreeSpell && DevelopMode ) ){
		player.AvailableSpellMask &= mask;
		LimitSpellLevels();
	}
	LimitPlayerPerks( CurrentPlayerIndex );
	if( (player.CurLife & ~63) > 0 ){
		--ItemUpdateTimer;
		if( ! ItemUpdateTimer || forceItemsRecalc ){
			UpdateAllItems(); // тут "защита" вещей от хакеров
		}
		RecalcPlayer( CurrentPlayerIndex, true /*2*/, forceItemsRecalc ); // 2 было для корректной смены анимаци при проверочном перерасчете, пробуем без
		// Kill summons on spell levels change
		if( Dungeon != DUN_0_TOWN ){
	        const int summonSpells[SUM_COUNT] = { PS_21_GOLEM, PS_52_LESSER_SUMMON, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON };
	        for( size_t i = 0, ie = SUM_COUNT; i < ie; ++i ){
	            KillExtraSummons( CurrentPlayerIndex, i, PlayerSpellLevel( CurrentPlayerIndex, summonSpells[i] ) );
			}
        }
		if( ! ItemUpdateTimer ) ItemUpdateTimer = 5 * ENGINE_FPS; // validate items once per 5 seconds
	}

	RandUsingCounter = randUsingCounter;
    RandomSeed = randomSeed;
    LastSeed = lastSeed;
}

//----- (007124C0) --------------------------------------------------------
void LimitSpellLevels()
{
	Player& player = Players[CurrentPlayerIndex];
	int slvl_limit = (2 * player.CharLevel / 3) + 3; // min: 3, max: 100
	LimitToMax(slvl_limit, SpellLevelBookMax() );
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	for( int spellIndex = 1; spellIndex < PS_COUNT; ++spellIndex ){
	    LimitToMax( player.SpellLevels[spellIndex], slvl_limit );
	    if( !( Spells[spellIndex].RequiredClassMask & playerFullClassMask ) ){
	        player.SpellLevels[spellIndex] = 0;
	        player.AvailableSpellMask &= ~(1I64 << (spellIndex - 1));
        }
	}
}

//----- (007182B0) --------------------------------------------------------
void UpdateItem( Item& item, bool keepCharges /*= true*/)
{
	LastItemInfo info;
	SaveItem( &info, &item );
	LoadItem( &info, &item, keepCharges );
	item.IsReqMet = CheckItemReq(CurrentPlayerIndex, &item);
}

//----- (007182D0) --------------------------------------------------------
void UpdateAllItems()
{
	Player& player = Players[CurrentPlayerIndex];
	int itemCount = player.InvItemCount + IS_Inventory;
	for( int i = 0; i < itemCount; ++i ){
		UpdateItem( player.OnBodySlots[i] );
	}
}

//----- (0045DAD7) --------------------------------------------------------
void ProcessPlayers()
{
	if( (uint)CurrentPlayerIndex >= 4 ){
		TerminateWithError("ProcessPlayers: illegal player %d", CurrentPlayerIndex);
	}
	extern bool PerkButtonClicked; PerkButtonClicked = false;
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay ){
		player.SomeSoundMuteDelay--;
	}
	if( PlayerSoundMessageDelay > 0 && PlayerSoundMessageDelay-- == 1 ){
		switch( SoundMessage ){
			case S_985_HF_DEFILER1: StartSpeech( SP_286 ); break;
			case S_986_HF_DEFILER2: StartSpeech( SP_287 ); break;
			case S_987_MONSTER_CRY_003: StartSpeech( SP_288 ); break;
			case S_988_HF_DEFILER4: StartSpeech( SP_289 ); break;
			default: PlayGlobalSound( SoundMessage ); break;
		}
	}
	if( !CooldownDecDelay ){
        for( auto& cooldown : player.cooldowns ){
            if( cooldown > 0 ){
                --cooldown;
            }
        }
        CooldownDecDelay = 20;
	}
	--CooldownDecDelay;
	ValidateCurPlayer();
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		// обрабатываем только текущего игрока и присоединенных игроков на том же уровне что и текущий
		if( player.IsExists && Dungeon == player.dungeon && (playerIndex == CurrentPlayerIndex || !player.lvlChanging) ){
			LimitPlayerParams(playerIndex);
			if( !PlrDeathModeOK(playerIndex) && (player.CurLife & -64) <= 0 ){
				TryToDie(playerIndex, -1);
			}
			if( playerIndex == CurrentPlayerIndex ){
				if( (player.CurLife & ~63) > 0 /*&& GameMode != GM_COLISEUM*/){
					Character_Passive_Life_and_Mana_Regeneration(playerIndex);
				}
				#ifdef OLD_CODE
				if( player.effectFlag[EA_LOSE_REGENERATION] && BaseMana > 0 ){
					player.BaseMana = player.BaseMana - player.CurMana;
					player.CurMana = 0;
				}
				#endif
			}
			int moreAction = 0;
			do{
				// Play walk sound
				if( player.CurAction >= PCA_1_WALK_UP && player.CurAction <= PCA_3_WALK_HORISONTAL // если идем
				 && (player.currentActionFrame == 1 || player.currentActionFrame == 5) // каждый четвертый фрейм ходьбы
				 /* && !player.running*/ ){
					int walkSound = S_0_WALK1;
					
					if( is(Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH)
					 || player.gameChanger & BIT(GC_15_ICE_AGE) && ! Dungeon->isQuest && is(Dungeon->graphType, DT_3_CAVE, DT_4_HELL, DT_6_ABYSS) ){
						walkSound = S_1029;
					}
					else{
						switch( Dungeon->graphType ){
						case DT_0_TOWN: walkSound = S_1030; break;
						case DT_1_CHURCH: walkSound = S_1031; break;
						case DT_2_CATACOMB: walkSound = S_1032; break;
						case DT_3_CAVE: walkSound = S_1033; break;
						case DT_6_ABYSS: walkSound = S_1035; break;
						case DT_4_HELL: walkSound = S_1034; break;
						case DT_5_CRYPT: walkSound = S_0_WALK1; break;
						}
					}
				LABEL_63:
					if (PlayerRunningSounds == 2) {
						PlayLocalSound(walkSound, player.Row, player.Col);// играть звук шага
					}
					else if (PlayerRunningSounds == 1) {
						if (!player.running) {
							PlayLocalSound(walkSound, player.Row, player.Col);
						}
					}
				}
				switch( player.CurAction ){
				case PCA_0_STAND:			moreAction = PM_DoNothing		(playerIndex);	break;
				case PCA_1_WALK_UP:			moreAction = PM_DoWalkUp		(playerIndex);	break;
				case PCA_2_WALK_DOWN:		moreAction = PM_DoWalkDown		(playerIndex);	break;
				case PCA_3_WALK_HORISONTAL:	moreAction = PM_DoWalkHorizontal(playerIndex);	break;
				case PCA_4_ATTACK:			moreAction = PM_DoAttack		(playerIndex);	break;
				case PCA_5_RANGE_ATTACK:	moreAction = (player.IsBowInHand ? PM_DoRangedAttack : PM_DoThrow)( playerIndex ); break;
				case PCA_6_BLOCK:			moreAction = PM_DoBlock			(playerIndex);	break;
				case PCA_7_GOT_HIT:			moreAction = PM_DoGotHit		(playerIndex);	break;
				case PCA_8_DEATH:			moreAction = PM_DoDeath			(playerIndex);	break;
				case PCA_9_SPELL:			moreAction = PM_DoSpell			(playerIndex);	break;
				case PCA_10_NEW_LEVEL:		moreAction = PM_DoNothing		(playerIndex);	break;
				default: break;
				}
				// убрал пока "классический" ! DungeonLevel в проверке, так как добавил repeat rbutton в easy click
				if( ( ! EasyClick /*|| ! DungeonLevel*/ ) && playerIndex == CurrentPlayerIndex && moreAction ){
					Repeat_Mouse_LButtonDown();
					Repeat_Mouse_RButtonDown();
				}
				CheckNewPath(playerIndex);
			}while( moreAction );
			if( ++player.currentAnimationFrameTick > player.currentAnimationDelay ){ // ok
				player.currentAnimationFrameTick = 0;
				if( ++player.currentAnimationFrame > player.currentAnimationFramesCount ){
					player.currentAnimationFrame = player.CurAction == PCA_8_DEATH ? player.currentAnimationFramesCount : 1;
				}
			}
			if( is(player.CurAction, PCA_0_STAND, PCA_10_NEW_LEVEL) && player.currentAnimationFramesCount < player.StayAnimationFrameCount
			 || player.CurAction == PCA_1_WALK_UP && player.currentAnimationFramesCount < player.WalkAnimationFrameCount
			 || player.CurAction == PCA_2_WALK_DOWN && player.currentAnimationFramesCount < player.WalkAnimationFrameCount
			 || player.CurAction == PCA_3_WALK_HORISONTAL && player.currentAnimationFramesCount < player.WalkAnimationFrameCount
			 || player.CurAction == PCA_4_ATTACK && player.currentAnimationFramesCount < player.AttackAnimationFrameCount
			 || player.CurAction == PCA_5_RANGE_ATTACK && player.currentAnimationFramesCount < player.AttackAnimationFrameCount
			 || player.CurAction == PCA_6_BLOCK && player.currentAnimationFramesCount < player.BlockAnimationFrameCount
			 || player.CurAction == PCA_7_GOT_HIT && player.currentAnimationFramesCount < player.HitRecoveryAnimationFrameCount
			 || player.CurAction == PCA_8_DEATH && player.currentAnimationFramesCount < player.DeathAnimationFrameCount
			 || player.CurAction == PCA_9_SPELL && player.currentAnimationFramesCount < player.SpellAnimationFrameCount
			 || player.currentAnimationPointer && player.currentAnimationFramesCount > *(int*)player.currentAnimationPointer ){ // check for correct animation selected by internal cel frame counter
				StartStand(playerIndex, player.dir);
				ClearPlrPVars(playerIndex);
			}
			if( player.notHittableFramesAfterTransition > 0 ) player.notHittableFramesAfterTransition--;
		}
	}
}

enum { Limit_Stat = 1000*4, Limit_Hp_Mana = 5000*64*4 }; 
//----- (0045DD9E) --------------------------------------------------------
void __fastcall LimitPlayerParams(int playerIndex)
{
	Player& player = Players[playerIndex];
	
	LimitToRange(player.BaseStrength, 0, Limit_Stat );
	LimitToRange(player.BaseMagic, 0, Limit_Stat );
	LimitToRange(player.BaseDexterity, 0, Limit_Stat );
	LimitToRange(player.BaseVitality, 0, Limit_Stat );
	LimitToRange(player.CurStrength, 0, Limit_Stat );
	LimitToRange(player.CurMagic, 0, Limit_Stat );
	LimitToRange(player.CurDexterity, 0, Limit_Stat );
	LimitToRange(player.CurVitality, 0, Limit_Stat );
	LimitToMax(player.CurLife, Limit_Hp_Mana );
	LimitToMax(player.CurMana, Limit_Hp_Mana );

	int nextStatToDecrease = 0;
	while (1) {
		// #msl : mordor's stat limitation
		int blueblood_limit_raise = HasTrait(playerIndex, TraitId::BlueBlood) ? 40 : 0;
		int gifted_limit = HasTrait(playerIndex, TraitId::Gifted) ? 100 : 0;
		int giant_limit = HasTrait(playerIndex, TraitId::Giant) ? 100 : 0;
		int paladin_limit = HasTrait(playerIndex, TraitId::Paladin) ? (200 + PerkValue(SYNERGY_THAUMATURGY, playerIndex)) : 0;
		int nightkin_limit = 0;
		if( MaxCountOfPlayersInGame == 1 && (player.gameChanger & BIT(GC_9_NIGHT_KIN)) ){
			nightkin_limit = 140;
		}
		int oldfashioned = HasTrait(playerIndex, TraitId::OldFashioned) ? 40 : 0; 
		int leper = HasTrait(playerIndex, TraitId::Leper) ? 150 : 0; 
		int lithe = HasTrait(playerIndex, TraitId::LitheBuild) ? 100 : 0;
		int statSum = CalcCharBaseStatSum(player);
		int extra_mode_boost = 0;
		if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE)) {
			extra_mode_boost = player.CharLevel / 2 + 50;
		}
		int statSumLimit = (10 * player.CharLevel)
			+ extra_mode_boost
			+ 90
			+ (2 * player.CharLevel / 3)
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
		// below is the stat decrease loop!
		if( statSum <= statSumLimit ) break;
		
		int tryDecreaseStats = 0;
		bool statDecreased = false;
		while( ! statDecreased && tryDecreaseStats < 4 ){
			switch( nextStatToDecrease ){
			case 0: nextStatToDecrease = 1; ++tryDecreaseStats;
				if( player.BaseStrength > 15 ){
					statDecreased = true;
					--player.BaseStrength;
					--player.CurStrength;
					break;
				}
			case 1: nextStatToDecrease = 2; ++tryDecreaseStats;
				if( player.BaseMagic > 15 ){
					statDecreased = true;
					--player.BaseMagic;
					--player.CurMagic;
					int mana_increment = 64;
					if( player.ClassID == PC_2_MAGE ){
						mana_increment = 128;// 2x
					}else if( player.ClassID == PC_4_ROGUE ){
						mana_increment = 96;// 1.5x
					}
					player.BaseMana -= mana_increment;
					player.MaxBaseMana -= mana_increment;
					player.CurMana -= mana_increment;
					player.MaxCurMana -= mana_increment;
					break;
				}
			case 2: nextStatToDecrease = 3; ++tryDecreaseStats;
				if( player.BaseDexterity > 15 ){
					statDecreased = true;
					--player.BaseDexterity;
					--player.CurDexterity;
					break;
				}
			case 3: nextStatToDecrease = 0; ++tryDecreaseStats;
				if( player.BaseVitality > 15 ){
					statDecreased = true;
					--player.BaseVitality;
					--player.CurVitality;
					int hp_increment = 64;// 64 sub hit points, basically it's 1 hit point
					if( is(player.ClassID, PC_0_WARRIOR, PC_5_SAVAGE) ){
						hp_increment = 128;// 2x life decrease for big guys: warriors and savages
					}
					player.BaseLife -= hp_increment;
					player.MaxBaseLife -= hp_increment;
					player.CurLife -= hp_increment;
					player.MaxCurLife -= hp_increment;
					break;
				}
			}
		}
		if( ! statDecreased ) break;
	}

	// character base life & mana formula, by class (# player life, player hit points, player HP)
	if (player.fullClassId == PFC_THRAEX) { // with 'blood for blood' trait, no level-up life gains
		player.MaxBaseLife = (3 * player.BaseVitality + (HasTrait(playerIndex, TraitId::BloodForBlood) ? 2 : (2 * player.CharLevel)) + 18) << 6; // 3*bVit +2*clvl + 18
		player.MaxBaseMana = (player.BaseMagic + player.CharLevel - 1) << 6; // bMAG + clv - 1
	}
	else if (player.fullClassId == PFC_WARRIOR || player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_TEMPLAR 
		|| player.fullClassId == PFC_DIMACHAERUS || player.fullClassId == PFC_SECUTOR) {
		if (HasTrait(playerIndex, TraitId::Paladin)) {
			player.MaxBaseLife = (player.BaseVitality / 2 + player.CharLevel + 19) << 6; // bVit +clvl + 9
			player.MaxBaseMana = ((player.BaseMagic * 2) + (2 * player.CharLevel) + 38) << 6; // 2*bMAG + 2*clv + 38
		}
		else if (HasTrait(playerIndex, TraitId::Ranger)) {
			player.MaxBaseLife = (2 * player.BaseVitality + 2 * player.CharLevel + 18) << 6; // 2*bVit +2*clvl + 18
			player.MaxBaseMana = ((3 * player.BaseMagic / 2) + 2 * player.CharLevel + 8) << 6; // 1,5*bMAG + 2*clv + 8
		}
		else if (GameMode == GM_CLASSIC){
			player.MaxBaseLife = (2 * player.BaseVitality + 2 * player.CharLevel + 18) << 6; // 2*bVit +2*clvl + 18
			player.MaxBaseMana = (player.BaseMagic + player.CharLevel - 1) << 6; // 1*bMAG + 2*clv + 8
		}
		else {
			player.MaxBaseLife = (3 * player.BaseVitality + 2 * player.CharLevel + 18) << 6; // 3*bVit +2*clvl + 18
			player.MaxBaseMana = (player.BaseMagic + player.CharLevel - 1) << 6; // bMAG + clv - 1
		}
	}
	else if (player.fullClassId == PFC_MURMILLO) {
		player.MaxBaseLife = (3 * player.BaseVitality + (player.BaseVitality / 2) + 3 * player.CharLevel + 2) << 6; // 3,5*bVit + 3*clvl + 2
		player.MaxBaseMana = (player.BaseMagic + (player.BaseMagic / 2) + player.CharLevel - 1) << 6; // (bMAG / 2) + clvl -1
	}
	else if (player.fullClassId == PFC_ARCHER || player.fullClassId == PFC_SCOUT || player.fullClassId == PFC_SHARPSHOOTER) {
		if (GameMode == GM_CLASSIC) {
			player.MaxBaseLife = (player.BaseVitality + 2 * player.CharLevel + 23) << 6; // 2*bVit +2*clvl + 18
			player.MaxBaseMana = (player.BaseMagic + 2 * player.CharLevel + 5) << 6; // 1*bMAG + 2*clv + 8
		}
		else {
			player.MaxBaseLife = (2 * player.BaseVitality + 2 * player.CharLevel + 8) << 6; // 2*bVIT + 2*clvl + 8
			player.MaxBaseMana = (player.BaseMagic + 2 * player.CharLevel + 3) << 6; // bMAG + 2 * clvl + 3
		}
	}
	else if (player.fullClassId == PFC_BOMBARDIER) {
		player.MaxBaseLife = (player.BaseVitality * 3 / 2 + player.CharLevel + 22) << 6; // 1,5 * bVIT + clvl + 22
		player.MaxBaseMana = (player.BaseMagic + player.BaseMagic / 2 + player.CharLevel - 3) << 6; // bMAG * 1,5 + clvl - 3
	}
	else if (player.fullClassId == PFC_TRAPPER) {
		player.MaxBaseLife = ((player.BaseVitality * 7 / 4) + (HasTrait(playerIndex, TraitId::Engineer) ? 0 : (player.CharLevel * 3 / 2)) + 24) << 6; // (1,75 * bVIT) + (1,5 * clvl) + 24
		player.MaxBaseMana = ((player.BaseMagic * 7 / 4) + (HasTrait(playerIndex, TraitId::Engineer) ? 0 : (player.CharLevel * 3 / 2)) - 11) << 6; // (bMAG * 1,75) + (1,5 * clvl) + 4
	}
	else if (player.fullClassId == PFC_MAGE || player.fullClassId == PFC_ELEMENTALIST || player.fullClassId == PFC_WARLOCK) {
		if (GameMode == GM_CLASSIC) {
			player.MaxBaseLife = (player.BaseVitality + player.CharLevel + 9) << 6; // 2*bVit +2*clvl + 18
			player.MaxBaseMana = (2 * player.BaseMagic + 2 * player.CharLevel - 2) << 6; // 1*bMAG + 2*clv + 8
		}
		else if (HasTrait(playerIndex, TraitId::Mamluk)) {
			player.MaxBaseLife = (7 * player.BaseVitality / 2 + player.CharLevel + 14) << 6;	// bVIT*3,5 + clv + 19 (85 on start by default)
			player.MaxBaseMana = (player.BaseMagic + player.CharLevel + 4) << 6; // bMAG + clvl + 4
		}
		else if (HasTrait(playerIndex, TraitId::Psion) /*&& HasTrait(playerIndex, TraitId::GrimDeal)*/) {
			player.MaxBaseLife = (player.BaseVitality + player.CharLevel + 9) << 6;	// bVIT + clv + 9
			player.MaxBaseMana = (player.BaseMagic + player.CharLevel + 4) << 6; // bMAG + clvl + 4
		}
		else {
			player.MaxBaseLife = (player.BaseVitality + player.CharLevel + 9) << 6;	// bVIT + clv + 9
			player.MaxBaseMana = (2 * player.BaseMagic + 2 * player.CharLevel + 3) << 6; // 2 * bMAG + 2*clvl + 3
		}
	}
	else if (player.fullClassId == PFC_DEMONOLOGIST || player.fullClassId == PFC_NECROMANCER) {
		player.MaxBaseLife = (player.BaseVitality + 2 * player.CharLevel + 8) << 6;	// bVIT + 2*clv + 8
		player.MaxBaseMana = ((player.BaseMagic * 5 / 4) + player.CharLevel + 22) << 6; // 1,25 * bMAG + clvl + 22
	}
	else if (player.fullClassId == PFC_BEASTMASTER) {
		player.MaxBaseLife = (2 * player.BaseVitality + player.CharLevel / 2 - 5) << 6;	// 2*bVIT + clvl/2 - 5
		player.MaxBaseMana = ((player.BaseMagic * 5 / 4) + player.CharLevel + 22) << 6; // 1,25 * bMAG + clvl + 22
	}
	else if (player.fullClassId == PFC_MONK || player.fullClassId == PFC_SHINOBI) {
		player.MaxBaseLife = ((5 * player.BaseVitality / 2) + (2 * player.CharLevel + 13)) << 6;	// 2,5*bVIT + 2*clv + 13
		player.MaxBaseMana = ((player.BaseMagic * 3 / 2) + player.CharLevel + 2) << 6; // 1,5 * bMAG + clvl + 2
	}
	else if (player.fullClassId == PFC_KENSEI) {
		player.MaxBaseLife = (3 * player.BaseVitality + (2 * player.CharLevel + 13)) << 6;	// 3*bVIT + 2*clv + 13
		player.MaxBaseMana = (3 * player.BaseMagic / 2 + player.CharLevel + 2) << 6; // 1,5 * bMAG + clvl + 2
	}
	else if (player.fullClassId == PFC_SHUGOKI) {
		player.MaxBaseLife = (3 * player.BaseVitality + (player.BaseVitality / 2) + 3 * player.CharLevel + 12) << 6; // 3,5*bVit + 3*clvl + 12
		player.MaxBaseMana = (player.BaseMagic + player.CharLevel + 4) << 6; // bMAG + clvl + 4
	}
	else if (player.fullClassId == PFC_ROGUE) {
		if (HasTrait(playerIndex, TraitId::BigFrame)) {
			player.MaxBaseLife = (3 * player.BaseVitality + 3 * player.CharLevel + 22) << 6;	// 3*bVIT + 3*clv + 22
		}
		else {
			player.MaxBaseLife = (2 * player.BaseVitality + 2 * player.CharLevel + 18) << 6;	// 2*bVIT + 2*clv + 18
		}
		player.MaxBaseMana = (2 * player.BaseMagic + 2 * player.CharLevel + 18) << 6; // 2 * bMAG + 2*clvl + 18
	}
	else if (player.fullClassId == PFC_ASSASSIN) {
		player.MaxBaseLife = (2 * player.BaseVitality + 2 * player.CharLevel + 18) << 6;	// 2*bVIT + 2*clv + 18
		if (HasTrait(playerIndex, TraitId::Black_Witchery)) {
			player.MaxBaseMana = (2 * player.BaseMagic + 3 * player.CharLevel + 27) << 6; // 2 * bMAG + 3*clvl + 27
		}
		else {
			player.MaxBaseMana = ((3 * player.BaseMagic / 2) + (3 * player.CharLevel / 2) - 6) << 6; // 1,5 * bMAG + 1,5*clvl - 6
		}
	}
	else if (player.fullClassId == PFC_IRON_MAIDEN) {
		player.MaxBaseLife = (3 * player.BaseVitality + (player.CharLevel / 5) + 25) << 6;	// 3* bVIT + clvl/5 + 25
		player.MaxBaseMana = (player.BaseMagic + player.CharLevel) << 6; // bMAG + clvl
	}
	else if (player.fullClassId == PFC_SAVAGE || player.fullClassId == PFC_BERSERKER || player.fullClassId == PFC_GUARDIAN) {
		player.MaxBaseLife = (7 * player.BaseVitality / 2 + 2 * player.CharLevel + 16) << 6;	// 3,5*bVIT + 2*clv + 16
		player.MaxBaseMana = (3 * player.BaseMagic / 2 + 2 * player.CharLevel + 8) << 6; // 1,5 * bMAG + 2*clvl + 8
	}
	else if (player.fullClassId == PFC_EXECUTIONER) {
		if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
			player.MaxBaseLife = (5 * player.BaseVitality + 5 * player.CharLevel) << 6;	// 5*bVIT + 5*clv - 8
			player.MaxBaseMana = (5 * player.BaseMagic + 5 * player.CharLevel) << 6; // 5*bMAG + 5*clvl 
		}
		else{ // default exec
			player.MaxBaseLife = (9 * player.BaseVitality / 2 + 3 * player.CharLevel) << 6;	// 4,5*bVIT + 3*clv
			player.MaxBaseMana = (player.BaseMagic + player.CharLevel / 2) << 6; // bMAG + clvl/2 
		}
	}
	else if (player.fullClassId == PFC_DRUID) {
		player.MaxBaseLife = (3 * player.BaseVitality + 3 * player.CharLevel + 7) << 6;
		player.MaxBaseMana = (2 * player.BaseMagic + 2 * player.CharLevel + 13) << 6;
	}
	if (HasTrait(playerIndex, TraitId::FearTheReaper)) {
		player.MaxBaseLife = 50 << 6;
	}
	if (HasTrait(playerIndex, TraitId::Giant)) {
		player.MaxBaseLife += player.BaseVitality << 6;
	}
	// this should prevent base and baseMax life mismatches (there was a bug with Scout class having wrong values in the beginning)
	LimitToMax(player.BaseLife, player.MaxBaseLife);
	LimitToMax(player.BaseMana, player.MaxBaseMana);
}

//----- (0045DE33) --------------------------------------------------------
void __fastcall ClearPlayerPath(int playerIndex)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ClrPlrPath: illegal player %d", playerIndex);
	}
	memset(Players[playerIndex].walkPath, PCMD_M1_NOTHING, sizeof(Players[playerIndex].walkPath));
}

//----- (0045DE66) --------------------------------------------------------
int __fastcall CellFreeForPlayer(int playerIndex, int row, int col)
{
	if( ! In112(row,col) ) return false;
	int playerNum = PlayerMap[row][col];
	int otherPlayerIndex = abs(playerNum) - 1;
	int monsterNum = MonsterMap[row][col];
	int objectNum = ObjectsMap[row][col];
	int objectIndex = abs(objectNum) - 1;
	bool isDoor = false;
	
	return FineMap[row][col] && ! IsCellBlocking(row, col)
	  && ( ! playerNum || otherPlayerIndex == playerIndex || !Players[otherPlayerIndex].CurLife )
	  && ( monsterNum == 0 || (Dungeon != DUN_0_TOWN && monsterNum != 0
	  && ( (Monsters[abs(monsterNum) - 1].CurrentLife & 0xFFFFFFC0) <= 0 || (IsPlayerSummon(abs(monsterNum) - 1)) ) ) ) 
	  && ( ! objectNum || ! Objects[objectIndex].isBlockWalking || isDoor );
}

//----- (th2) -------------------------------------------------------------
int __fastcall CellFreeForPlayerForceMove(int playerIndex, int row, int col)
{
	Player& player = Players[playerIndex];
	if( row == player.TargetRow && col == player.TargetCol ){
		return true;
	}
	return CellFreeForPlayer(playerIndex, row, col);
}

//                       🡵  🡴  🡶   🡷  🡱   🡲  🡳  🡰
int RowStep[ 9 ] = { 0,  0,  1, -1,  0,  1, -1, -1,  1 };
int ColStep[ 9 ] = { 0,  1,  0,  0, -1,  1,  1, -1, -1 };

//----- (0045DF24) --------------------------------------------------------
void __fastcall MakePlayerPath(int playerIndex, int row, int col, int endSpace)
{
	if( (uint)playerIndex >= 4 ) TerminateWithError("MakePlrPath: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	player.TargetRow = row;
	player.TargetCol = col;
	if( player.NextRow == row && player.NextCol == col ) return;
	int pathLen = FindPath( ForceMove ? CellFreeForPlayerForceMove : CellFreeForPlayer, playerIndex, player.NextRow, player.NextCol, row, col, player.walkPath);
	if( pathLen ){
		if( !endSpace ){
			int stepDirection = player.walkPath[ --pathLen ];
			player.TargetRow = row + RowStep[stepDirection];
			player.TargetCol = col + ColStep[stepDirection];
		}
		player.walkPath[pathLen] = PCMD_M1_NOTHING;	
	}
}

//----- (0045E339) --------------------------------------------------------
void __fastcall SyncPlrAnim( uint playerIndex )
{
	if( playerIndex >= 4 ) TerminateWithError("SyncPlrAnim: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	int orientation = player.dir;
	char* curAnim;
	
	char animType = Spells[player.spellIndex].animType;
	switch( player.fullClassId ){
	case PFC_ELEMENTALIST: if( player.spellIndex == PS_13_HYDRA ) animType = SA_2_MAGIC; break;
	case PFC_DRUID       : if( player.spellIndex == PS_22_FURY  ) animType = SA_3_TURN;  break;
	}
	if( playerIndex != CurrentPlayerIndex ) animType = SA_0_FIRE; // TODO: check why multi not sync

	switch( player.CurAction ){
	case PCA_1_WALK_UP:
	case PCA_2_WALK_DOWN:
	case PCA_3_WALK_HORISONTAL:	curAnim = player.walkAnimationsPointers[orientation];           break;
	case PCA_4_ATTACK:
	case PCA_5_RANGE_ATTACK:	curAnim = player.attackAnimationsPointers[orientation];         break;
	case PCA_6_BLOCK:			curAnim = player.blockAnimationsPointers[orientation];          break;
	case PCA_9_SPELL:
		switch( animType ){
		case SA_0_FIRE :        curAnim = player.fireSpellAnimationsPointers[orientation];      break;
		case SA_1_LIGHT:        curAnim = player.lightningSpellAnimationsPointers[orientation]; break;
		case SA_2_MAGIC:        curAnim = player.magicSpellAnimationsPointers[orientation];     break;
		case SA_3_TURN:         curAnim = player.turningAnimationsPointers[orientation];        break;
		}
	case PCA_7_GOT_HIT:			curAnim = player.gotHitAnimationsPointers[orientation];			break;
	case PCA_8_DEATH:			curAnim = player.deathAnimationsPointers[orientation];			break;
	case PCA_0_STAND:
	case PCA_10_NEW_LEVEL:
	case PCA_11_QUIT:			curAnim = player.nothingAnimationsPointers[orientation];		break;
	default: TerminateWithError("SyncPlrAnim");
	}
	player.currentAnimationPointer = curAnim;
}

//----- (0045E49C) --------------------------------------------------------
void __fastcall SyncInitPlrPos(int playerIndex)
{
	// Force the player's target position to be where he wants it to be
	Player& player = Players[playerIndex];
	player.TargetRow = player.Row;
	player.TargetCol = player.Col;

	// Don't find a space for single player
	if( MaxCountOfPlayersInGame == 1 ) return;

	// Find a place to put him in the map
	if( player.dungeon == Dungeon ){
		uint i = 0;
		for(; i < size(PlrXoff2) - 1; ++i ){
			if( CellFreeForPlayer(playerIndex, player.Row + PlrXoff2[i], player.Col + PlrYoff2[i]) ){
				break;
			}
		}

		// stuff player into the map
		player.Row += PlrXoff2[i];
		player.Col += PlrYoff2[i];
		PlayerMap[player.Row][player.Col] = playerIndex + 1;
	}
}

//----- (0045E559) --------------------------------------------------------
void __fastcall SyncInitPlr(int playerIndex)
{
	if( (uint)playerIndex >= PlayersMax_4 ) TerminateWithError("SyncInitPlr: illegal player %d", playerIndex);
	SetPlrAnims(playerIndex);
	SyncInitPlrPos(playerIndex);
}

//----- (0045E57E) --------------------------------------------------------
void __fastcall LimitPlayerStats(uint playerIndex)
{
	if( playerIndex >= PlayersMax_4 ) TerminateWithError("CheckStats: illegal player %d", playerIndex);
	const PlayerStat maxStat = GetPlayerMaxStat(playerIndex);
	Player& player = Players[playerIndex];
	LimitToRange(player.BaseStrength,  0, maxStat.Strength);
	LimitToRange(player.BaseMagic,     0, maxStat.Magic );
	LimitToRange(player.BaseDexterity, 0, maxStat.Dexterity );
	LimitToRange(player.BaseVitality,  0, maxStat.Vitality );
}

//----- (0045E68C) --------------------------------------------------------
bool __fastcall ModifyPlayerStrength(int playerIndex, int strengthDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrStr: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	const PlayerStat maxStat = GetPlayerMaxStat( playerIndex );
	if( strengthDelta + player.BaseStrength > maxStat.Strength ){
		strengthDelta = maxStat.Strength - player.BaseStrength;
	}
	if( CannotIncreaseStat(playerIndex) != 1 || strengthDelta <= 0 ){
		player.CurStrength += strengthDelta;
		player.BaseStrength += strengthDelta;
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_87_SETSTR, player.BaseStrength );
		}
		return true;
	}
	return false;
}

//----- (0045E724) --------------------------------------------------------
bool __fastcall ModifyPlayerMagic(int playerIndex, int magicDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrMag: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	const PlayerStat maxStat = GetPlayerMaxStat( playerIndex );

	if( magicDelta + player.BaseMagic > maxStat.Magic ){
		magicDelta = maxStat.Magic - player.BaseMagic;
	}
	if( CannotIncreaseStat(playerIndex) != 1 || magicDelta <= 0 ){
		player.CurMagic += magicDelta;
		player.BaseMagic += magicDelta;
		int manaModifyValue = magicDelta << 6;
		if( player.ClassID == PC_2_MAGE ){
			manaModifyValue *= 2;
		}else if( player.ClassID == PC_4_ROGUE ){
			manaModifyValue += manaModifyValue / 2;
		}
		player.MaxBaseMana += manaModifyValue;
		player.MaxCurMana += manaModifyValue;

		int playerOffset = sizeof( Player ) * playerIndex;
		if( ! player.effectFlag[EA_LOSE_REGENERATION] ){
			player.BaseMana += manaModifyValue;
			player.CurMana += manaModifyValue;
		}
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_88_SETMAG, player.BaseMagic );
		}
		return true;
	}
	return false;
}

//----- (0045E822) --------------------------------------------------------
bool __fastcall ModifyPlayerDexterity(int playerIndex, int dexterityDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrDex: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	const PlayerStat maxStat = GetPlayerMaxStat( playerIndex );

	if( dexterityDelta + player.BaseDexterity > maxStat.Dexterity ){
		dexterityDelta = maxStat.Dexterity - player.BaseDexterity;
	}
	if( CannotIncreaseStat(playerIndex) != 1 || dexterityDelta <= 0 ){
		player.CurDexterity += dexterityDelta;
		player.BaseDexterity += dexterityDelta;
		RecalcPlayer(playerIndex, true);
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_89_SETDEX, player.BaseDexterity );
		}
		return true;
	}
	return false;
}

//----- (0045E8BA) --------------------------------------------------------
bool __fastcall ModifyPlayerVitality(int playerIndex, int vitalityDelta)
{
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("ModifyPlrVit: illegal player %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	const PlayerStat maxStat = GetPlayerMaxStat( playerIndex );
	if( vitalityDelta + player.BaseVitality > maxStat.Vitality ){
		vitalityDelta = maxStat.Vitality - player.BaseVitality;
	}
	if( CannotIncreaseStat(playerIndex) != 1 || vitalityDelta <= 0 ){
		player.CurVitality += vitalityDelta;
		player.BaseVitality += vitalityDelta;
		int lifeDelta = vitalityDelta << 6; // * 64
		switch(player.ClassID){
		case PC_0_WARRIOR: lifeDelta *= 3; break;
		case PC_1_ARCHER or PC_4_ROGUE: lifeDelta *= 2; break;
		case PC_2_MAGE: break;
		case PC_3_MONK: lifeDelta = 5 * lifeDelta / 2; break;
		case PC_5_SAVAGE: lifeDelta = 7 * lifeDelta / 2; break;
		}
		player.BaseLife += lifeDelta;
		player.MaxBaseLife += lifeDelta;
		player.CurLife += lifeDelta;
		player.MaxCurLife += lifeDelta;
		RecalcPlayer( playerIndex, true );
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_90_SETVIT, player.BaseVitality );
		}
		return true;
	}
	return false;
}

//----- (0045E9AC) --------------------------------------------------------
void __fastcall SetPlayerHitPoints(int playerIndex, int hitPoints)
{
	Player& player = Players[playerIndex];
	if( (uint)playerIndex >= 4 ){
		TerminateWithError("SetPlayerHitPoints: illegal player %d", playerIndex);
	}
	player.CurLife = hitPoints;
	player.BaseLife = hitPoints + player.MaxBaseLife - player.MaxCurLife;
}

//----- (0070FD00) --------------------------------------------------------
void __fastcall SetPlayerHitPointsOnRestart( int playerIndex, int hitPoints )
{
	Player& player = Players[ playerIndex ];
	if( (uint) playerIndex >= 4 ){
		TerminateWithError( "SetPlayerHitPoints: illegal player %d", playerIndex );
	}
	player.CurLife = hitPoints;
	player.BaseLife = hitPoints + player.MaxBaseLife - player.MaxCurLife;
	if( player.BaseLife < 100 ){
		player.BaseLife = 100;
	}
}

//----- (0045EA0A) --------------------------------------------------------
void __fastcall SetPlayerStrength( uint playerIndex, int value )
{
	if( playerIndex >= 4 ) TerminateWithError("SetPlrStr: illegal player %d", playerIndex);
	Players[playerIndex].BaseStrength = value;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EAB6) --------------------------------------------------------
void __fastcall SetPlayerDexterity( uint playerIndex, int value )
{
	if( playerIndex >= 4 ) TerminateWithError("SetPlrDex: illegal player %d", playerIndex);
	Players[playerIndex].BaseDexterity = value;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EA44) --------------------------------------------------------
void __fastcall SetPlayerMagic( uint playerIndex, int value )
{
	if( playerIndex >= 4 ) TerminateWithError("SetPlrMag: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	player.BaseMagic = value;
	value <<= 6; // mana shift
	switch(player.ClassID){
	case PC_2_MAGE : value *= 2;         break; // 2x
	case PC_4_ROGUE: value += value / 2; break; // 1.5x
	}
	player.MaxBaseMana = value;
	player.MaxCurMana  = value;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EAF0) --------------------------------------------------------
void __fastcall SetPlayerVitality( uint playerIndex, int value )
{
	if( playerIndex >= 4 ) TerminateWithError("SetPlrVit: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	player.BaseVitality = value;
	value <<= 6; // hp shift
	switch(player.ClassID){
	case PC_0_WARRIOR:
	case PC_5_SAVAGE : value *= 2; break; // 2x
	}
	player.MaxBaseLife = value;
	player.MaxCurLife  = value;
	RecalcPlayer(playerIndex, true);
}

//----- (th2) -------------------------------------------------------------
void __fastcall SetPlayerPerk( uint playerIndex, int perkId, int perkValue )
{
	if( playerIndex >= 4 ) TerminateWithError("SetPlayerPerk: illegal player %d", playerIndex);
	Players[playerIndex].perk[perkId] = perkValue;
	RecalcPlayer(playerIndex, true);
}

//----- (0045EB59) --------------------------------------------------------
void __fastcall InitDungMsgs( uint playerIndex )
{
	if( playerIndex >= 4 )TerminateWithError("InitDungMsgs: illegal player %d", playerIndex);
	Player& player = Players[playerIndex];
	player.DungeonMessageFlags1 = 0;
	player.DungeonMessageFlags2 = 0;
}

//----- (0045EB94) --------------------------------------------------------
void PlayDungMsgs()
{
	if( CurrentPlayerIndex >= 4 ){
		TerminateWithError("PlayDungMsgs: illegal player %d", CurrentPlayerIndex);
	}

	Player& player = Players[CurrentPlayerIndex];
	if( Dungeon == DUN_1_CHURCH_1 && ! player.dungeonVisited[DUN_1_CHURCH_1] && !(player.DungeonMessageFlags1 & 1) ){
		PlayerSoundMessageDelay = 40;
		switch (player.fullClassId ){
		case PFC_EXECUTIONER:	SoundMessage = S_1125_EXE_ENTER_LVL1;	break;
		case PFC_WARRIOR:
		case PFC_TEMPLAR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	SoundMessage = S_832_WAR_96B;	break;
		case PFC_SHARPSHOOTER:	
		case PFC_SCOUT:	
		case PFC_TRAPPER:	
		case PFC_ARCHER:	SoundMessage = S_718_ROG_96;	break;
		case PFC_MAGE:
		case PFC_DEMONOLOGIST:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_NECROMANCER:
		case PFC_ELEMENTALIST:		SoundMessage = S_615_SOR_96;	break;
		case PFC_MONK:	
		case PFC_SHINOBI:
		case PFC_KENSEI:
		case PFC_SHUGOKI:		SoundMessage = S_935_MONK_96;	break;
		case PFC_IRON_MAIDEN:
		case PFC_ASSASSIN:	
		case PFC_BOMBARDIER:
		case PFC_ROGUE:	SoundMessage = S_591_ASS_DLVL1;	break;
		}
		player.DungeonMessageFlags1 |= 1;
		return;
	}

	if( Dungeon == DUN_5_CATACOMB_1 && ! player.dungeonVisited[DUN_5_CATACOMB_1] && !(player.DungeonMessageFlags1 & 2) ){
		PlayerSoundMessageDelay = 40;
		switch (player.fullClassId ){
		case PFC_EXECUTIONER:	SoundMessage = S_1126_EXE_ENTER_LVL5;	break;
		case PFC_WARRIOR:
		case PFC_TEMPLAR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	SoundMessage = S_831_WAR_90;	break;
		case PFC_SHARPSHOOTER:	
		case PFC_IRON_MAIDEN:
		case PFC_ASSASSIN:	
		case PFC_BOMBARDIER:
		case PFC_ROGUE:
		case PFC_SCOUT:	
		case PFC_TRAPPER:	
		case PFC_ARCHER:	SoundMessage = S_717_ROG_90;	break;
		case PFC_MAGE:
		case PFC_DEMONOLOGIST:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_NECROMANCER:
		case PFC_ELEMENTALIST:		SoundMessage = S_614_SOR_91;	break;
		case PFC_MONK:	
		case PFC_SHINOBI:
		case PFC_KENSEI:
		case PFC_SHUGOKI:		SoundMessage = S_934_MONK_91;	break;
		}
		player.DungeonMessageFlags1 |= 2;
		return;
	}

	if( Dungeon == DUN_9_CAVE_1 && ! player.dungeonVisited[DUN_9_CAVE_1] && !(player.DungeonMessageFlags1 & 4) ){
		PlayerSoundMessageDelay = 40;
		switch (player.fullClassId ){
		case PFC_EXECUTIONER:	SoundMessage = S_1127_EXE_ENTER_LVL9;	break;
		case PFC_WARRIOR:
		case PFC_TEMPLAR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	SoundMessage = S_833_WAR_98;	break;
		case PFC_SHARPSHOOTER:	
		case PFC_IRON_MAIDEN:
		case PFC_ASSASSIN:	
		case PFC_BOMBARDIER:
		case PFC_ROGUE:
		case PFC_SCOUT:	
		case PFC_TRAPPER:	
		case PFC_ARCHER:	SoundMessage = S_719_ROG_98;	break;
		case PFC_MAGE:
		case PFC_DEMONOLOGIST:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_NECROMANCER:
		case PFC_ELEMENTALIST:		SoundMessage = S_616_SOR_SL;	break;
		case PFC_MONK:	
		case PFC_SHINOBI:
		case PFC_KENSEI:
		case PFC_SHUGOKI:		SoundMessage = S_936_MONK_98;	break;
		}
		if (player.gameChanger & BIT(GC_15_ICE_AGE)) {
			SoundMessage = S_1127_EXE_ENTER_LVL9;
		}
		player.DungeonMessageFlags1 |= 4;
		return;
	}

	if( Dungeon == DUN_13_HELL_1 && ! player.dungeonVisited[DUN_13_HELL_1] && !(player.DungeonMessageFlags1 & 8) ){
		PlayerSoundMessageDelay = 40;
		switch (player.fullClassId ){
		case PFC_EXECUTIONER:	SoundMessage = S_1128_EXE_ENTER_LVL13;	break;
		case PFC_WARRIOR:
		case PFC_TEMPLAR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	SoundMessage = S_834_WAR_99;	break;
		case PFC_SHARPSHOOTER:	
		case PFC_IRON_MAIDEN:
		case PFC_ASSASSIN:	
		case PFC_BOMBARDIER:
		case PFC_ROGUE:
		case PFC_SCOUT:	
		case PFC_TRAPPER:	
		case PFC_ARCHER:	SoundMessage = S_720_ROG_99;	break;
		case PFC_MAGE:
		case PFC_DEMONOLOGIST:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_NECROMANCER:
		case PFC_ELEMENTALIST:		SoundMessage = S_617_SOR_99;	break;
		case PFC_MONK:	
		case PFC_SHINOBI:
		case PFC_KENSEI:
		case PFC_SHUGOKI:		SoundMessage = S_937_MONK_99;	break;
		}
		player.DungeonMessageFlags1 |= 8;
		return;
	}

	if( Dungeon == DUN_16_HELL_4 && ! player.dungeonVisited[DUN_16_HELL_4] && !(player.DungeonMessageFlags1 & 16) ){
		PlayerSoundMessageDelay = 40;
		SoundMessage = S_950_LVL16INT;
		player.DungeonMessageFlags1 |= 16;
	}

	if( Dungeon == DUN_17_ABYSS_1 && ! player.dungeonVisited[DUN_17_ABYSS_1] && !(player.DungeonMessageFlags2 & 1) ){
		PlayerSoundMessageDelay = 10;
		SoundMessage = S_602_M_DU /*S_985_HF_DEFILER1*/;
	#if 0
		Quest& islandQuest = Quests[Q_20_ISLAND];
		islandQuest.status = QS_2_IN_PROGRESS;
		islandQuest.talkToFlag = true;
		islandQuest.speechIndex = S_286_T_HEALER03;
	#endif
		player.DungeonMessageFlags2 |= 1;
		return;
	}
	#if 0
	if (Dungeon == DUN_18_ABYSS_2 && ! player.dungeonVisited[DUN_18_ABYSS_2]) { // bugged! keeps repeating after game reloading. that's unacceptable
		PlayerSoundMessageDelay = 200;
		SoundMessage = S_1273_MONSTER_CRY_002;
	}
	#endif
	if( Dungeon == DUN_19_ABYSS_3 && ! player.dungeonVisited[DUN_19_ABYSS_3] && !(player.DungeonMessageFlags2 & 4) ){
		PlayerSoundMessageDelay = 90;
		SoundMessage = S_1273_MONSTER_CRY_002;
		player.DungeonMessageFlags2 |= 4;
		return;
	}



	if( Dungeon == DUN_21_CRYPT_1 && ! player.dungeonVisited[DUN_21_CRYPT_1] && !(player.DungeonMessageFlags1 & 32) ){
		PlayerSoundMessageDelay = 55;
		switch (player.fullClassId ){
		case PFC_EXECUTIONER:	SoundMessage = S_1129_EXE_ENTER_LVL21;	break;
		case PFC_WARRIOR:
		case PFC_TEMPLAR:
		case PFC_INQUISITOR:
		case PFC_GUARDIAN:
		case PFC_DIMACHAERUS:
		case PFC_SECUTOR:
		case PFC_BERSERKER:
		case PFC_THRAEX:
		case PFC_MURMILLO:
		case PFC_DRUID:
		case PFC_SAVAGE:	SoundMessage = S_822_WAR_92;	break;
		case PFC_SHARPSHOOTER:	
		case PFC_IRON_MAIDEN:
		case PFC_ASSASSIN:	
		case PFC_BOMBARDIER:
		case PFC_ROGUE:
		case PFC_SCOUT:	
		case PFC_TRAPPER:	
		case PFC_ARCHER:	SoundMessage = S_713_ROG_92;	break;
		case PFC_MAGE:
		case PFC_DEMONOLOGIST:
		case PFC_WARLOCK:
		case PFC_BEASTMASTER:
		case PFC_NECROMANCER:
		case PFC_ELEMENTALIST:		SoundMessage = S_610_SOR_92;	break;
		case PFC_MONK:	
		case PFC_SHINOBI:
		case PFC_KENSEI:
		case PFC_SHUGOKI:		SoundMessage = S_930_MONK_92;	break;
		}
		player.DungeonMessageFlags1 |= 32;
		return;
	}
}

//----- (0045EFBF) --------------------------------------------------------
void __fastcall PlayerMessageHold(bool start)
{
	static i64 startTime = 0;
	if( start ){
		startTime = -GetTickCountTh(); // save starting time
	}else{
		startTime += GetTickCountTh(); // calculate hold interval
		for( auto& msg: PlayerStatusMessages ){
			msg.messageTime += startTime;
		}
	}
}

//----- (0045EFF6) --------------------------------------------------------
void __fastcall ErrorPlrMsg( char* message)
{
	PlayerStatusMessage* v2; // esi@1
	char* v3;                // edi@1
	v2 = &PlayerStatusMessages[PlayerStatusMessageIndex];
	v3 = message;
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	v2->playerIndex = PlayersMax_4;
	v2->messageTime = GetTickCountTh(); // сетевая игра, перехватываем вызов
	strncpy(v2->str, v3, sizeof(v2->str));
	v2->str[sizeof(v2->str) - 1] = 0;
}

//----- (0045F03F) --------------------------------------------------------
void AddNewPlayerStatusFormattedMessage(const char* format, ...)
{
	PlayerStatusMessage* msg= &PlayerStatusMessages[PlayerStatusMessageIndex];
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	msg->playerIndex = PlayersMax_4;
	msg->messageTime = GetTickCountTh(); // сетевая игра, перехватываем вызов
	va_list args;
	va_start(args, format);
	vsprintf(msg->str, format, args);
	va_end(args);
}

//----- (0045F087) --------------------------------------------------------
void __fastcall AddNewPlayerDescriptionSatusMessage( int playerIndex, const char* message)
{
	PlayerStatusMessage* statusMessage;
	statusMessage = &PlayerStatusMessages[PlayerStatusMessageIndex];
	PlayerStatusMessageIndex = (PlayerStatusMessageIndex + 1) & 7;
	statusMessage->playerIndex = playerIndex;
	statusMessage->messageTime = GetTickCountTh(); // сетевая игра, перехватываем вызов
	strlen(Players[playerIndex].playerName);
	strlen(message);
	messageLog.push_back(make_pair(Players[playerIndex].playerName, message));
	sprintf(statusMessage->str, "%s (lvl %d): %s", Players[playerIndex].playerName, Dungeons[Players[playerIndex].dungeon].level, message);
}

//----- (0045F105) --------------------------------------------------------
void CheckPlayerStatusMessagesTimeout()
{
	PlayerStatusMessage* v2 = PlayerStatusMessages;
	i64 tick = GetTickCountTh(); // сетевая игра, перехыватываем вызов
	for( auto& msg: PlayerStatusMessages ){
		if( tick - msg.messageTime > 10000_ms ){
			msg.str[0] = 0;
		}
	}
}

//----- (0045F12F) --------------------------------------------------------
void ClearPlayerStatusMessages()
{
	memzero(PlayerStatusMessages);
	PlayerStatusMessageIndex = 0;
}

// отрисовка сообщений чата на экране
//----- (0045F14B) --------------------------------------------------------
void DrawPlayerStatusMessages()
{
	static const char colorTable[PlayersMax_4 + 1] = { C_0_White, C_0_White, C_0_White, C_0_White, C_3_Gold };
	int x = 10 + Screen_LeftBorder;
	int y = 70 + Screen_TopBorder;
	unsigned int rightTextCoordinate = 620;
    for( int i = 0; i < MaxMessages_8; i++ ){
		auto& msg = PlayerStatusMessages[i];
        if( msg.str ){
            DrawPlayerDescriptionText(x, y, rightTextCoordinate, msg.str, colorTable[msg.playerIndex]);
			y += 35;
        }
    }
}

//----- (0045F1D8) --------------------------------------------------------
void __fastcall DrawPlayerDescriptionText(int x, int y, unsigned int width, char* sstr, char color)
{
	unsigned int v5; // edi@1
	int v6;          // edx@2
	unsigned int v7; // eax@3
	unsigned int v8; // edx@3
	int v9;          // esi@3
	char v10;        // cl@4
	char v11;        // bl@11
	char v12;        // cl@5
	int v13;         // eax@11
	int v14;         // [sp+14h] [bp-4h]@1
	int v15;         // [sp+Ch] [bp-Ch]@1
	int v16;         // [sp+10h] [bp-8h]@2
	// a1 += Screen_LeftBorder;
	// a3 += Screen_LeftBorder;

	uint str = (uint)sstr;
	v14 = 0;
	v5 = str;
	v15 = x;
	if( *(uchar*)str ){
		v6 = (int)&YOffsetHashTable[y];
		v16 = v6;
		do{
			str = v5;
			v9 = *(uint*)v6 + v15;
			v7 = 0;
			v8 = v5;
			while( 1 ){
				v10 = *(uchar*)v8;
				if( !*(uchar*)v8 )
					break;
				++v8;
				v12 = FontIndexSmall[Codepage[v10]];
				v7 += FontWidthSmall[v12] + 1;
				if( v12 ){
					if( v7 >= width )
						goto LABEL_10;
				}else{
					str = v8;
				}
			}
			str = v8;
			LABEL_10:
			while( v5 < str ){
				v13 = *(uchar*)v5++;
				v11 = FontIndexSmall[Codepage[v13]];
				if( v11 )
					DrawLetter(v9, (unsigned __int8)v11, color, OnScreenTextOutline);
				v9 += FontWidthSmall[v11] + 1;
			}
			v6 = v16 + 40;
			++v14;
			v16 += 40;
		}while( v14 != 3 && *(uchar*)v5 );
	}
}
