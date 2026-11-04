#include "stdafx.h"

int QuestPanel_StartRowIndex;// 006EB348
bool IsQUESTPanelVisible;// 006EB34C
char* Data_QuestCEL;// 006EB350
char* Data_ExclamCEL;
Quest Quests[Quest_Count];// 006EB358

// (th4) DARK CAMPAIGN mission states, one per mission: DCM_0_UNTOUCHED / DCM_1_BRIEFED /
// DCM_2_COMPLETE. This is WORLD state, not character state -- it lives in the game save
// beside Quests[] and is reset by New Game, so a player stuck on a hard mission can start
// a new session to farm and comes back to mission one, keeping the levels and gold he
// earned. Putting it in the character file instead would make New Game resume the
// campaign, which removes exactly that escape.
uchar DarkMissionState[DARK_MISSION_MAX_50];
int QuestPanel_SelectedRow;// 006EB5F8
int QuestPanel_ActiveQuestsList[Quest_Count];// 006EB600
int QuestPanel_ActiveQuestCounts;// 006EB660
int WaterPaletteInPoizonRiverQuestCave;// 006EB668
int QuestPanel_PentagramsPicFrame;// 006EB678

const int QUEST_PANEL_CLOSE_ROW_INDEX = 22 + 8;
int HorazonGrimspikeDelay = 0;// 00A35F58
int FleshDoomDelay = 0;

int TheDenOneSecondTimer = 0;
int TheDenMonsterSpawnTimer = 0;

int Lava_Pits_OneSecondTimer = 0;
int Lava_Pits_MonsterSpawnTimer = 0;


bool CastColiseumPortal = false;

int QuestGroup1[4] = {// 00A33E70
	Q_12_THE_CURSE_OF_KING_LEORIC, 
	Q_13_POISONED_WATER_SUPPLY, 
	Q_23_INFESTED_CELLAR,
	Q_19_LOST_TREASURE,
};
int QuestGroup2[4] = {// 00A3A160
	Q_6_THE_BUTCHER, 
	Q_7_OGDENS_SIGN, 
	Q_2_GHARBAD_THE_WEAK,
	Q_21_THE_CURSED_BLADE,
};
int QuestGroup3[3] = {// 004B6B20
	Q_8_HALLS_OF_THE_BLIND, 
	Q_0_THE_MAGIC_ROCK, 
	Q_9_VALOR,
};
int QuestGroup4[4] = {// 00A37E10
	Q_1_BLACK_MUSHROOM, 
	Q_3_ZHAR_THE_MAD, 
	Q_10_ANVIL_OF_FURY,
	Q_20_SUNLESS_SEA,
};
int QuestGroup5[5] = {// 00A35050
	Q_4_LACHDANAN, 
	Q_11_WARLORD_OF_BLOOD,
	Q_22_MAIDEN_OF_ANGUISH,
	Q_16_HORAZON_S_DEMONS,
	Q_17_FALLEN_ANGEL,
};
int QuestGroup6[5] = {// 00A35068
	Q_21_THE_CURSED_BLADE,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
	Q_0_THE_MAGIC_ROCK,
};

RandomizeQuestsGroup RandomizeQuestsTable [10] = {// 00A350A0
	{QuestGroup1, 4, 2, 0},
	{QuestGroup2, 4, 2, 0}, // (rt) enable Fleshdoom randomization
	{QuestGroup3, 3, 2, 0},
	{QuestGroup4, 4, 2, 0},
	{QuestGroup5, 5, 2, 0},
	{QuestGroup6, 1, -1, 0},
	{0, -1, -1, -1}
};

// 00A38888
uchar IslandPattern[52] = {
	5, 5,

	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,

	0x6B, 0x6C, 0x6A, 0x00, 0x00,
	0x6A, 0x00, 0x6B, 0x00, 0x6B,
	0x6B, 0x6C, 0x6B, 0x00, 0x6C,
	0x36, 0x6B, 0x6C, 0x6B, 0x00,
	0x6B, 0x00, 0x6B, 0x43, 0x6B
};

//00A38900
char AngelSeq[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uchar CurAngelSeqIndex = 0; //00A3890A

DisplayObject QuestPanelRect;
DisplayObject QuestPanelTextAreaRect;

uchar SwampPattern[52] = {
	5, 5,

	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,

	0x6B, 0x6C, 0x6A, 0x00, 0x00,
	0x6A, 0x05, 0x02, 0x0E, 0x6B,
	0x6B, 0x04, 0x08, 0x2F, 0x6C,
	0x36, 0x04, 0x5F, 0x2E, 0x00,
	0x6B, 0x0D, 0x5E, 0x0C, 0x6B
};

uchar GrottoPattern[52] = {
	5, 5,

	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,
	7, 7, 7, 7, 7,

	0x6B, 0x6C, 0x6A, 0x00, 0x00,
	0x6A, 0x05, 0x0E, 0x05, 0x0E,
	0x6B, 0x0D, 0x0C, 0x0D, 0x0C,
	0x36, 0x05, 0x0E, 0x6B, 0x00,
	0x6B, 0x0D, 0x0C, 0x6C, 0x6B
};

//----- (0071CB70) --------------------------------------------------------
int __fastcall CheckAngelClick( uchar angelIndex )
{
	int result; // eax@3

	if( angelIndex == AngelSeq[ CurAngelSeqIndex ] ){
		if( CurAngelSeqIndex == 8 ){
			result = 2;
		}else{
			++CurAngelSeqIndex;
			result = 1;
		}
	}else{
		result = 0;
		CurAngelSeqIndex = 0;
	}
	// AngelSeq[] is the same for all players.  Flash missile and Red TP are automatically sync'd nicely between all players!
	return result;
}
							
//----- (0071CB00) --------------------------------------------------------
void __cdecl SetupAngels()
{
	int v0; // edi@1
	int v1; // edx@1
	int v2; // ecx@1
	int *v3; // ebx@2
	int v4; // esi@3

	v0 = ObjectsCount;
	v1 = 0;
	v2 = 1;
	if( ObjectsCount > 0 ){
		v3 = ObjectActive;
		do{
			v4 = *v3;
			if( Objects[ v4 ].BaseObjectIndex == BO_24_ANGEL ){
				Objects[ v4 ].bookIndex = v2++;
			}
			++v1;
			++v3;
		}while( v1 < v0 );
	}
	GenerateRandomSequenceOfAngels();
	CurAngelSeqIndex = 0;
}

//----- (0071CA90) --------------------------------------------------------
void GenerateRandomSequenceOfAngels()
{
	SetRndSeed(Players[CurrentPlayerIndex].CurrentSeedRun);

	//int v0; // ecx@0
	int v1; // esi@1
	char *v2; // edi@1

	v1 = 9;
	v2 = AngelSeq;
	do{
		*v2++ = RNG(3) + 1;
		--v1;
	}while( v1 );
}

//----- (0071BE50) --------------------------------------------------------
int AdjFrameQ_14_15()
{
	if( Dungeon->isQuest ){
		if( Dungeon == DUN_38_GLACIAL_CAVERNS ){
			return 2;
		}
		if( Dungeon == DUN_39_FROZEN_LOCH ){
			return 4;
		}
	}
	return 0;
}

//----- (0071BA50) --------------------------------------------------------
void Frozen_Loch_BlockSet()
{	
	int tile = 56;
	do{
		TileBlockWalking[ tile++ ] = 0;
	}while( tile < 147 );
	TileBlockWalking[ 150 ] = 0;
	TileBlockWalking[ 152 ] = 0;
	tile = 154;
	do{
		TileBlockWalking[ tile++ ] = 0;
	}while( tile < 162 );
}

// (rt) disabling a quest via RandomizeQuestsTable (fleshdoom is disabled in the table, so it isn't disabled here)
//----- (00716400) --------------------------------------------------------
void __fastcall OffRandQuestFromGroup (int groupIndex)
{
	RandomizeQuestsGroup& group = RandomizeQuestsTable[groupIndex];
	int* questsIndexesArray = group.questsIndexesArray;
	int questsInGroup = group.questsInGroup;
	Quest* offedQuest;
	int questIndex = -1;
	do{
		questIndex = questsIndexesArray[RNG(questsInGroup)];
		offedQuest = &Quests[questIndex];
	}while( offedQuest->status == QS_0_NO);
	// (rt) a given quest can be forced here
	//(rt) if( questIndex != Q_22_MAIDEN_OF_ANGUISH ) // e.g. andariel
	//if( questIndex != Q_12_THE_CURSE_OF_KING_LEORIC )
	offedQuest->status = QS_0_NO;
}

//----- (00716440) --------------------------------------------------------
int __fastcall CountOfActiveQuestInGroup (int groupIndex)
{
	int count = 0;
	RandomizeQuestsGroup& group = RandomizeQuestsTable[groupIndex];
	int* questsIndexesArray = group.questsIndexesArray;
	for( int i = 0; i < group.questsInGroup; i++ ){
		if( Quests[questsIndexesArray[i]].status ){
			count++;
		}
	}
	return count;
}

//----- (00716480) --------------------------------------------------------
void RandomizeQuests()
{
	for( int groupIndex = 0; RandomizeQuestsTable[groupIndex].countInOneGame != -1; groupIndex++ ){
		int countInOneGame = RandomizeQuestsTable[groupIndex].countInOneGame;
		while( CountOfActiveQuestInGroup(groupIndex) > countInOneGame ){
			OffRandQuestFromGroup (groupIndex);
		}
	}
}

//----- (0045F607) --------------------------------------------------------
void InitQuests()
{
	// (th4) Dark Campaign missions reset with the rest of the world on New Game.
	for( auto& m: DarkMissionState ) m = DCM_0_UNTOUCHED;
	IsQUESTPanelVisible = false;
	QuestPanel_PentagramsPicFrame = 1;
	HorazonGrimspikeDelay = 0;
	FleshDoomDelay = 0;
	TheDenOneSecondTimer = 0;
	TheDenMonsterSpawnTimer = 0;
	Lava_Pits_OneSecondTimer = 0;
	Lava_Pits_MonsterSpawnTimer = 0;
	WaterPaletteInPoizonRiverQuestCave = 0;
	// OB-----------------------
	OutbreakState = 0;
	OutbreakWave = 0;
	OutbreakSpawned = 0;
	OutbreakPauseTimer = 0;
	OutbreakSpawnTimer = 0;
	//--------------------------

	//int multiPlayerCounter = 0;
	for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
		Quest& quest = Quests[questIndex];
		BaseQuest& baseQuest = BaseQuests[questIndex];
		quest.baseId = baseQuest.id;
		if( GameMode == GM_CLASSIC       && !(baseQuest.flag & QF_CLASSIC)
		 || MaxCountOfPlayersInGame == 1 && !(baseQuest.flag & QF_SINGLE )
		 || MaxCountOfPlayersInGame >  1 && !(baseQuest.flag & QF_MULTI  ) ){ 
			quest.status = QS_0_NO;
			continue;
		}else{
			if( MaxCountOfPlayersInGame == 1 ){ // single player
				quest.status = QS_1_ACCEPT;
				quest.dungeonStart = baseQuest.spDungeonStart;
				quest.questDungeon = baseQuest.spQuestDungeon;
				quest.status2 = 0;
				quest.talkToFlag = false;
				quest.status3 = 0;
				quest.status4 = 0;
				quest.status5 = 0;
			}else{ // multi player
				//In BaseQuests.cpp, fill BaseQuests[].mpDungeonLevel with BaseQuests[].spDungeonLevel value to enable quest entrance generation in MP
				quest.dungeonStart = baseQuest.mpDungeonStart;
				quest.questDungeon = baseQuest.mpQuestDungeon;
				//if( ! delta_quest_inited( multiPlayerCounter ) ){
				// Broken logic, this func here should take questIndex as argument, and not some shifted index...
				// This would result in wrong quest initialization and sync...  2nd time this happens, God bless Blizzard!
				if( ! delta_quest_inited(questIndex) ){
					quest.status = QS_1_ACCEPT;
					quest.status2 = 0;
					quest.talkToFlag = false;
					quest.status3 = 0;
					quest.status4 = 0;
					quest.status5 = 0;
				}
				//multiPlayerCounter++;
			}
			if( GameMode == GM_CLASSIC ){
				quest.dungeonStart = baseQuest.classicStart;
			}
		}
		quest.row = 0;
		quest.col = 0;
		quest.dungeonType = baseQuest.dungeonType;
		//quest.status3 = 0; // Need to initialize sync for .status3 for MP Horazon quest, don't want 0 here...
		quest.speechIndex = baseQuest.speechIndex;
		if( questIndex == Q_16_HORAZON_S_DEMONS ){
			quest.speechIndex = HorazonQuestBookSpeechTable[15][Players[CurrentPlayerIndex].ClassID];
			if (Players[CurrentPlayerIndex].fullClassId == PFC_EXECUTIONER) {
				quest.speechIndex = SP_358;
			}
		}			
		quest.timer = 0;//What for?
		//quest.status4 = 0;
		//quest.status5 = 0;
	}

	if( MaxCountOfPlayersInGame == 1){
		SetRndSeed(DungeonRandSeed[DUN_15_HELL_3]);
		if( RandomizeSPQuests && GameMode != GM_IRONMAN && GameMode != GM_SURVIVAL){
			RandomizeQuests();
		}
	}

	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( kingLeoricQuest.status == QS_0_NO ){
		kingLeoricQuest.status3 = 2;
	}

	Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];
	if( magicRockQuest.status == QS_0_NO ){
		magicRockQuest.status3 = 2;
	}

	Quests[Q_7_OGDENS_SIGN].status2 = 1;

	if( MaxCountOfPlayersInGame != 1 ){
		Quests[Q_15_ARCHBISHOP_LAZARUS].status2 = LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN;
	}

	// Forced initial quest status desync to make sure players who joined late can still access quest area.
	// For MP quest sync purpose only.  If host already has Treasure Map dropped on the floor (.status/.status2 = 2/1) before non-host joined game,
	// must reset quest state for non-host, otherwise non-host will never be able to enter quest map if host leaves game without opening the Crypt.
//	if( MaxCountOfPlayersInGame > 1 && Quests[Q_19_LOST_TREASURE].status >= 2 && Quests[Q_19_LOST_TREASURE].status2 >= 1 && ! Players[CurrentPlayerIndex].dungeonVisited[DUN_7_CATACOMB_3] ){
	if (MaxCountOfPlayersInGame > 1 && Quests[Q_19_LOST_TREASURE].status >= 2 && Quests[Q_19_LOST_TREASURE].status2 < 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_7_CATACOMB_3]) {
		Quests[Q_19_LOST_TREASURE].status = QS_1_ACCEPT;
		Quests[Q_19_LOST_TREASURE].status2 = 0;
		Quests[Q_19_LOST_TREASURE].talkToFlag = 0;
	}

	// force open to test mp quest sync
	#if 0
	if( DevelopMode ){
		Quests[Q_19_LOST_TREASURE].status = QS_2_IN_PROGRESS;
		Quests[Q_19_LOST_TREASURE].talkToFlag = 1;
		Quests[Q_19_LOST_TREASURE].status2 = 3;
		Quests[Q_19_LOST_TREASURE].row = 20;
		Quests[Q_19_LOST_TREASURE].col = 33;

		Quests[Q_24_THE_RAMPAGING_DEMON].status = QS_2_IN_PROGRESS;
		Quests[Q_24_THE_RAMPAGING_DEMON].status3 = 0;
	}
	#endif

	// Force .status2 desync in MP to make sure players joined late can still see the Horazon cinematics.  This will not work after Na-Krul is done.
	if( MaxCountOfPlayersInGame > 1 && Quests[Q_16_HORAZON_S_DEMONS].status2 == 1 && ! Players[CurrentPlayerIndex].dungeonVisited[DUN_23_CRYPT_3] ){
		Quests[Q_16_HORAZON_S_DEMONS].status2 = 0;
	}

	// Quest reward abuse prevention: a player cannot get reward for free by repeatedly join a finished game.  Not needed for Q_16_HORAZON_S_DEMONS, Q_19_LOST_TREASURE, Q_20_ISLAND cuz no reward.
	if( MaxCountOfPlayersInGame > 1 && Quests[Q_13_POISONED_WATER_SUPPLY].status == 3 && ! Players[CurrentPlayerIndex].dungeonVisited[DUN_1_CHURCH_1] && ! Players[CurrentPlayerIndex].dungeonVisited[DUN_5_CATACOMB_1] ){
		Quests[Q_13_POISONED_WATER_SUPPLY].status2 = 2;//PWS
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_29_IRON_FIGURINE].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_11_CAVE_3] && !Players[CurrentPlayerIndex].dungeonVisited[DUN_12_CAVE_4]) {
		Quests[Q_29_IRON_FIGURINE].status2 = 2;
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_40_SECRETS_OF_MAGIC].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_9_CAVE_1] && !Players[CurrentPlayerIndex].dungeonVisited[DUN_10_CAVE_2]) {
		Quests[Q_40_SECRETS_OF_MAGIC].status2 = 2;
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_34_GRAVE_MATTERS].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_1_CHURCH_1] && !Players[CurrentPlayerIndex].dungeonVisited[DUN_2_CHURCH_2]) {
		Quests[Q_34_GRAVE_MATTERS].status2 = 2;
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_35_IRON_OATH].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_1_CHURCH_1] /*&& !Players[CurrentPlayerIndex].dungeonVisited[DUN_2_CHURCH_2]*/) {
		Quests[Q_35_IRON_OATH].status2 = 2;
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_30_DEMON_PLAGUE].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_8_CATACOMB_4] && !Players[CurrentPlayerIndex].dungeonVisited[DUN_9_CAVE_1]) {
		Quests[Q_30_DEMON_PLAGUE].status2 = 2;
	}

	if( MaxCountOfPlayersInGame > 1 && Quests[Q_17_FALLEN_ANGEL].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_17_ABYSS_1] ){
		Quests[Q_17_FALLEN_ANGEL].status2 = 3; // If someone else in game killed Izual before current player joins game, this player will NOT get Izual quest reward for free.
	}

	if( MaxCountOfPlayersInGame > 1 && Quests[Q_22_MAIDEN_OF_ANGUISH].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_12_CAVE_4] ){
		Quests[Q_22_MAIDEN_OF_ANGUISH].status2 = 2; // If someone else in game killed Andariel before current player joins game, this player will NOT get Andariel quest reward for free.
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_37_PRICE_OF_HUBRIS].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_12_CAVE_4]) {
		Quests[Q_37_PRICE_OF_HUBRIS].status2 = 2; // If someone else in game killed boss before current player joins game, this player will NOT get Hubris quest reward for free.
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_28_VISIONS_OF_VICTORY].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_22_CRYPT_2]) {
		Quests[Q_28_VISIONS_OF_VICTORY].status2 = 2;
	}

	if (MaxCountOfPlayersInGame > 1 && Quests[Q_32_A_SIMPLE_TASK].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_21_CRYPT_1]) {
		Quests[Q_32_A_SIMPLE_TASK].status2 = 2;
	}

	if( MaxCountOfPlayersInGame > 1 && Quests[Q_23_INFESTED_CELLAR].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_9_CAVE_1] ){
		Quests[Q_23_INFESTED_CELLAR].status2 = 2; // Infested Cellar
	}
	if (MaxCountOfPlayersInGame > 1 && Quests[Q_27_DEMON_IN_THE_TOWN].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_4_CHURCH_4]) {
		Quests[Q_27_DEMON_IN_THE_TOWN].status2 = 2;
	}
	if (MaxCountOfPlayersInGame > 1 && Quests[Q_38_OGDENS_SIGN_MP].status == 3 && !Players[CurrentPlayerIndex].dungeonVisited[DUN_4_CHURCH_4]) {
		Quests[Q_38_OGDENS_SIGN_MP].status2 = 2;
	}
}

//----- (00717890) --------------------------------------------------------
void __fastcall MoveGrimspike( int row, int col )
{
	ClearMonsterOnMap(GrimspikeMonsterIndex);
	Monster& grimspike = Monsters[GrimspikeMonsterIndex];
	grimspike.Row = row;
	grimspike.Col = col;
	MonsterMap[ row ][ col ] = GrimspikeMonsterIndex + 1;
	FixMonsterPosition( GrimspikeMonsterIndex, 0 );
}

//----- (007178F0) --------------------------------------------------------
void GrimspikeOpenScript ()
{
	if( HorazonGrimspikeDelay > 0 ){
		HorazonGrimspikeDelay--;
		return;
	}
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	switch( horazonQuest.status3 ){
	case 2:// (rt) the book was just activated
		OpenGrimspikeRoom (1);// (rt) opening the doors
		horazonQuest.status3 = 3; // (rt) the door is open
		HorazonGrimspikeDelay = 40; //(rt) delay before the player's sound
		break;
	case 3:// (rt) doors opened, player's speech
		{
			int soundIndex = S_M1_NO_SOUND;
			Player& player = Players[CurrentPlayerIndex];
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:
				soundIndex = S_786_WAR_57;
				break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:
				soundIndex = S_677_ROG_57;
				break;
			case PC_2_MAGE:
				soundIndex = S_574_SOR_57;
				break;
			}
			if( soundIndex != S_M1_NO_SOUND ){
				PlayLocalSound (soundIndex, player.Row, player.Col);
			}
		}
		horazonQuest.status3 = 4; // (rt) the speech has been said
		HorazonGrimspikeDelay = 100; //(rt) delay before summoning Grimspike
		break;
	case 4:// (rt) change of Grimspike's coordinates
		{
			int row = 39;
			int col = 34;
			MoveGrimspike (row, col);
			PlayLocalSound(S_141_STORM, row, col);
			CastMissile( 0, 0, row, col, 0, MI_62_RISING_2, CT_0_PLAYER, 0, 0, 0, 0 );
			horazonQuest.status3 = 5; // (rt) Grimspike has appeared
			break;
		}
	}
}

//----- (0045F7BC) --------------------------------------------------------
void QuestsActions()
{
	Player& player = Players[CurrentPlayerIndex];

	// (th4) DARK CAMPAIGN HUB: the outbound mission portal lives here rather than in the hub
	// generator, because its visible half is a red-portal missile and InitMissiles() runs
	// after level generation -- a missile cast there would be wiped. Same reason the Lazarus
	// and Sunless Sea portals are cast from this function. OpenDarkCampaignPortal is
	// idempotent, so running it every tick simply does nothing once the portal exists.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_117_DARKLORD_HIDEOUT ){
		int dcMission = DarkCampaignCurrentMission();
		if( dcMission >= 0 && DarkMissionState[dcMission] == DCM_1_BRIEFED ){
			OpenDarkCampaignPortal( dcMission );
		}
	}

	// (th4) DC MISSION 13: the way home is NOT earned by clearing the room -- Lazarus has to
	// be thanked for it. Three steps, all here so the order is visible:
	//   1. the player clicked him and has now walked adjacent  -> play the thanks
	//   2. the thanks panel has closed                          -> hand over his set pieces
	//   3. mark him thanked                                     -> releases the block below
	// DarkCampaignLazarusThanked is transient, but it does not need saving: once step 3 runs
	// the mission flips to DCM_2_COMPLETE, which IS saved, and the gate below tests that too.
	// So a reload after the portal exists still finds the way home, and a reload BEFORE the
	// talk simply requires talking again -- the reward cannot be taken twice.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_121_UNHOLY_ALTAR_DC ){
		int lazIndex = DarkCampaignLazarusIndex();
		if( lazIndex < 0 ){
			DarkCampaignLazarusTalkWanted = false;
		}
		else if( DarkCampaignLazarusTalkWanted && DarkCampaignLazarusLairClear() ){
			const Monster& laz = Monsters[lazIndex];
			const Player& pl = Players[CurrentPlayerIndex];
			// Test both the tile he is on and the one he is stepping to: a walk-to only
			// updates Row/Col when the step lands, and he may be standing on the last tile
			// of the path for a frame before that happens.
			int distNow  = max( abs(pl.Row - laz.Row), abs(pl.Col - laz.Col) );
			int distNext = max( abs(pl.NextRow - laz.Row), abs(pl.NextCol - laz.Col) );
			if( distNow <= 1 || distNext <= 1 ){
				DarkCampaignLazarusTalkWanted = false;
				StartSpeech( SP_DC_LAZARUS_THANKS );
				DarkCampaignLazarusRewardPending = true;
			}
		}
		// Speech_IsPanelVisible is the whole timer: this line has no VO, so the panel closing
		// is the only "it finished" there is.
		if( DarkCampaignLazarusRewardPending && !Speech_IsPanelVisible ){
			DarkCampaignLazarusRewardPending = false;
			DarkCampaignLazarusThanked = true;
			// His ordinary set pieces, on his ordinary conditions -- SpecifiedUniqueDrop reads
			// Difficulty, fullClassId and traits itself and spawns at monster.Row/Col. It is
			// normally called from MonsterDeath; nothing in it requires the monster to be dead.
			SpecifiedUniqueDrop( lazIndex, true );
			// (th4) ...plus four ordinary drops, exactly as if he had been killed. SpawnItem
			// is what MonsterDeath's dropNItems lambda calls, and it derives quality from the
			// monster and the usual level/difficulty factors on its own -- so these roll the
			// same as any Lazarus kill. Four rather than the three his death case gives.
			const Monster& lazDrop = Monsters[lazIndex];
			for( int n = 0; n < 4; ++n ){
				SpawnItem( lazIndex, lazDrop.Row, lazDrop.Col, true );
			}
		}
	}

	// (th4) DC MISSION 26: the Dark Lord waits in the ruins. Clicking him walks the player
	// over; standing next to him plays his last words; dismissing them ends the run.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_44_RAVENHOLM ){
		int lordIndex = DarkCampaignEndLordIndex();
		if( lordIndex < 0 ){
			DarkCampaignEndTalkWanted = false;
		}
		else if( DarkCampaignEndTalkWanted && !DarkCampaignEndIsPending() ){
			const Monster& lord = Monsters[lordIndex];
			Player& pl = Players[CurrentPlayerIndex];
			int distNow  = max( abs(pl.Row - lord.Row), abs(pl.Col - lord.Col) );
			int distNext = max( abs(pl.NextRow - lord.Row), abs(pl.NextCol - lord.Col) );
			if( distNow <= 2 || distNext <= 2 ){
				DarkCampaignEndTalkWanted = false;
				DarkCampaignEndSetPending( true );
				ClearPlayerPath( CurrentPlayerIndex );
				pl.destAction = PCMD_M1_NOTHING;
				StartSpeech( SP_DC_CAMPAIGN_END );
			}
		}
		// The campaign ends when he has finished, not when he starts. PrepDoEnding is the
		// engine's own way out -- the same one the vanilla ending uses.
		if( DarkCampaignEndIsPending() && !Speech_IsPanelVisible ){
			DarkCampaignEndSetPending( false );
			DarkCampaignEndArrived = false;
			// PrepDoEnding restores sound with IsSoundEnable = SaveSound, and SaveSound is
			// only ever written by KillDiabloEffect, which mutes the game for Diablo's death
			// cinematic. Nothing on this path goes through that, so SaveSound would still be
			// its dataseg zero and the main menu would come back silent. Stash the real value
			// here, exactly as the Diablo sequence does before its own ending.
			SaveSound = IsSoundEnable;
			PrepDoEnding();
		}
	}

	// (th4) DC MISSION 25: the Butcher is caged until the player speaks to him. Same three
	// steps as 13 and 15 -- click arms, proximity fires, and the latch flips him from parked
	// to following. Unlike those two there is nothing to clear first: freeing him IS the job.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_111_CHURCH_SACRISTY ){
		int butIndex = DarkCampaignButcherIndex();
		if( butIndex < 0 ){
			DarkCampaignButcherTalkWanted = false;
		}
		else if( DarkCampaignButcherTalkWanted && !DarkCampaignButcherIsFreed() ){
			// The click walked him to the tile in front of the cell; the line waits until he
			// is actually standing on it and has stopped moving.
			Player& pl = Players[CurrentPlayerIndex];
			if( pl.Row == DC_BUTCHER_TALK_ROW && pl.Col == DC_BUTCHER_TALK_COL
			 && pl.NextRow == DC_BUTCHER_TALK_ROW && pl.NextCol == DC_BUTCHER_TALK_COL ){
				DarkCampaignButcherTalkWanted = false;
				DarkCampaignButcherJoinPending = true;
				StartSpeech( SP_DC_BUTCHER_FREED );
			}
		}
		// He gets up when the player DISMISSES the line, not when it opens -- same shape as
		// the armorer's reward. This line has no VO, so the panel closing is the whole timer.
		if( DarkCampaignButcherJoinPending && !Speech_IsPanelVisible ){
			DarkCampaignButcherJoinPending = false;
			DarkCampaignSetButcherFreed();
		}
	}

	// (th4) DC MISSION 15: same shape as mission 13. The way home is not earned by clearing
	// the forge -- Hephasto has to be thanked for it, and he hands over the Anvil of Fury
	// reward when he does. Same three steps, same reasoning about the latch: once step 3
	// runs the mission flips to DCM_2_COMPLETE, which IS saved, so the gate below survives
	// a reload and the reward cannot be taken twice.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO ){
		int hepIndex = DarkCampaignHephastoIndex();
		if( hepIndex < 0 ){
			DarkCampaignHephastoTalkWanted = false;
		}
		// The talk waits for him to be back at the anvil, same as the "!" mark -- clicking
		// him mid-walk arms the request and it fires when he arrives.
		else if( DarkCampaignHephastoTalkWanted && DarkCampaignHellforgeClear()
		      && DarkCampaignHephastoAtForge() ){
			const Monster& hep = Monsters[hepIndex];
			const Player& pl = Players[CurrentPlayerIndex];
			int distNow  = max( abs(pl.Row - hep.Row), abs(pl.Col - hep.Col) );
			int distNext = max( abs(pl.NextRow - hep.Row), abs(pl.NextCol - hep.Col) );
			if( distNow <= 1 || distNext <= 1 ){
				DarkCampaignHephastoTalkWanted = false;
				StartSpeech( SP_DC_HEPHASTO_THANKS );
				DarkCampaignHephastoRewardPending = true;
			}
		}
		// This line has no VO either, so the panel closing is the whole timer.
		if( DarkCampaignHephastoRewardPending && !Speech_IsPanelVisible ){
			DarkCampaignHephastoRewardPending = false;
			DarkCampaignHephastoThanked = true;
			// Exactly what Griswold hands over -- SpawnAnvilOfFuryReward is his own code,
			// lifted into a shared function, so class, full class, traits, Game Changers
			// and Difficulty are all honoured without being restated here.
			const Monster& hep = Monsters[hepIndex];
			SpawnAnvilOfFuryReward( CurrentPlayerIndex, hep.Row, hep.Col );
		}
	}

	// (th4) DARK CAMPAIGN MISSION COMPLETION. When the current mission's level is free of real
	// monsters (its own summons excepted), the mission is done: mark it complete and open the
	// way home. Runs in the mission level, not the hub.
	// The mission 10 escort are real monsters too, so they must be discounted here or a cleared
	// level would sit forever at two "heroes" remaining and never place the return portal.
	if( GameMode == GM_DARK_CAMPAIGN && Dungeon != DUN_117_DARKLORD_HIDEOUT
	 && MonstersCount - DarkCampaignEscortCountInLevel() <= SummonMonsters_Count ){
		for( int m = 0; m < DarkMissionCount && m < DARK_MISSION_MAX_50; ++m ){
			if( Dungeon != DarkMissions[m].dungeon ) continue;
			if( DarkMissionState[m] == DCM_0_UNTOUCHED ) continue; // not this mission's run

			// (th4) DC mission 13 only: a lair full of dead heroes is not finished. The Dark
			// Lord's man has to be spoken to first, so hold everything -- completion, the
			// on-screen message and the portal -- until then. Once complete this test passes
			// on the saved state alone, so the portal survives a reload.
			if( Dungeon == DUN_121_UNHOLY_ALTAR_DC
			 && DarkMissionState[m] != DCM_2_COMPLETE && !DarkCampaignLazarusThanked ) continue;
			// (th4) DC mission 15: and the same for the Hellforge.
			if( Dungeon == DUN_43_HEPHASTO
			 && DarkMissionState[m] != DCM_2_COMPLETE && !DarkCampaignHephastoThanked ) continue;
			// (th4) DC mission 25: a cellar full of dead heroes is not the job either -- the
			// Butcher has to be freed. COMPLETE is saved, so what follows can read it as
			// "the player got him out" and bring him along from here on.
			// Only freeing him matters -- losing him afterwards does NOT fail the mission.
			// He is with the player for the rest of the campaign either way.
			if( Dungeon == DUN_111_CHURCH_SACRISTY
			 && DarkMissionState[m] != DCM_2_COMPLETE && !DarkCampaignButcherIsFreed() ) continue;

			if( DarkMissionState[m] == DCM_1_BRIEFED ){
				DarkMissionState[m] = DCM_2_COMPLETE;
				// (th4) Announce the way home. Inside the BRIEFED->COMPLETE branch, so it fires
				// exactly once on the clearing tick -- the portal placement below runs every
				// tick to survive reloads, and would otherwise spam the message.
				// Not on mission 26: Ravenholm is the last one and has no way home, so there
				// is nothing to announce.
				if( Dungeon != DUN_44_RAVENHOLM ){
					AddOnScreenMessage( OM_112_DC_Exit_Portal );
				}
			}

			// (th4) DC MISSION 26, the last one. No way home: instead a portal opens beside
			// the player and the Dark Lord steps out of it to close the campaign in person.
			// He then stands idle with the mark over his head until spoken to.
			if( Dungeon == DUN_44_RAVENHOLM ){
				if( DarkCampaignEndLordIndex() < 0 ){
					if( !DarkCampaignEndArrived ){
						// The portal opens the instant the last hero falls; he comes through
						// it a beat later. Not both on the same tick -- the point of the
						// scene is that the portal is there first and he walks out of it.
						bool havePortal = false;
						for( int i = 0; i < MissileAmount; ++i ){
							if( Missiles[ MissileIndexes[i] ].BaseMissileIndex == MI_65_RED_PORTAL ){
								havePortal = true; break;
							}
						}
						if( !havePortal ){
							CastMissile( DC_END_PORTAL_ROW, DC_END_PORTAL_COL,
							             DC_END_PORTAL_ROW, DC_END_PORTAL_COL, 0,
							             MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
						}
						DarkCampaignEndArrived = true;
						DarkCampaignEndLordDelay = DC_END_LORD_DELAY;
					}
					else if( DarkCampaignEndLordDelay > 0 && --DarkCampaignEndLordDelay == 0 ){
						DarkCampaignEndSpawnLord( DC_END_PORTAL_ROW, DC_END_PORTAL_COL );
					}
				}
				break;
			}

			

			// The way home, on THIS mission's own tile (DarkMissions[m].returnRow/Col) -- a
			// single global tile would land in a wall on some maps, and there is no way out
			// of a mission until it is cleared. Idempotent, exactly like
			// OpenDarkCampaignPortal: both halves are re-placed if missing, so it survives a
			// save/reload inside the cleared level -- the warp is dropped by ClearWarps(),
			// the red-portal missile by InitMissiles().
			int retRow = DarkMissions[m].returnRow;
			int retCol = DarkMissions[m].returnCol;
			// (th4) A mission whose return tile has not been chosen yet (0,0) would aim the
			// warp and the portal missile off the map. Neither would ever register, so the
			// block below would re-cast every tick -- audible as a portal stuck retrying.
			// Fall back to wherever the player is standing when the level comes clear.
			if( retRow == 0 && retCol == 0 ){
				retRow = Players[CurrentPlayerIndex].Row;
				retCol = Players[CurrentPlayerIndex].Col;
			}
			// Test for "already placed" by identity rather than by tile: with the fallback
			// above the target moves with the player, so a coordinate match would miss and
			// place a fresh portal every step.
			bool haveWarp = false;
			for( auto& w: Warps ){
				if( w.destDungeon == DUN_117_DARKLORD_HIDEOUT ){ haveWarp = true; break; }
			}
			if( !haveWarp ){
				AddWarp( retRow, retCol, WM_1029_GOTO_QUEST_DUNGEON, DUN_117_DARKLORD_HIDEOUT );
			}
			bool havePortal = false;
			for( int i = 0; i < MissileAmount; ++i ){
				Missile& mis = Missiles[ MissileIndexes[i] ];
				if( mis.BaseMissileIndex == MI_65_RED_PORTAL ){
					havePortal = true; break;
				}
			}
			if( !havePortal ){
				CastMissile( retRow, retCol, retRow, retCol, 0,
				             MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
			}
			break;
		}
	}

	if (CastColiseumPortal) {
		CastMissile(60, 60, 60, 60, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0);
		CastColiseumPortal = false;
	}

	if( IsQuestOnLevel(Q_19_LOST_TREASURE)
	 && Quests[ Q_19_LOST_TREASURE ].status2 == QS_3_COMPLETE
	 && Quests[ Q_19_LOST_TREASURE ].row != 20 ){
		Quests[ Q_19_LOST_TREASURE ].row = 20;
		Quests[ Q_19_LOST_TREASURE ].col = 33;
		FillDMapOpenedDisusedReliquary( 18, 30 );
		PlayGlobalSound( 72 );
	}

	if( IsQuestOnLevel(Q_15_ARCHBISHOP_LAZARUS) && MaxCountOfPlayersInGame > 1 ){
		Quest& lazarusQuest = Quests[ Q_15_ARCHBISHOP_LAZARUS ];
		if( lazarusQuest.status2 == LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN ){
			AddObject( BO_83_ALTBOY, Convert40to112( CurPattern.row ) + 4, Convert40to112( CurPattern.col ) + 6 );
			lazarusQuest.status2 = LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR;
            NetSendCmdQuest( true, Q_15_ARCHBISHOP_LAZARUS );
		}
	}

//	if( ! IsQuestOnLevel(Q_20_ISLAND) || islandQuest.status3 == 1 ){//This makes no sense...  We doing Flesh Doom quest, NOT Island quest...
//		if( Quests[ Q_21_FLESHDOOM ].status && Quests[ Q_21_FLESHDOOM ].status2 <= 7 ){//Performance hog, this gets executed at the start of the game.
		if (Dungeon == DUN_0_TOWN && Quests[Q_21_THE_CURSED_BLADE].status && Quests[Q_21_THE_CURSED_BLADE].status2 <= 7 && Quests[Q_21_THE_CURSED_BLADE].status4 == 1) {//Only after Wielder of Shadow Fang is dead, can player sets Tremain on Fire in Town.  This also resolves the issue where if player carries Shadow Fang to the next game, then player can no longer set Tremain on fire before going into quest map.
			InquisitionScript();
		}
//	}

	Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
	if( IsQuestOnLevel(Q_20_SUNLESS_SEA) ){
		if( islandQuest.status3 == 1 ){
			islandQuest.status3 = 0;
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				int objectIndex = ObjectActive[objectIndexIndex];
				Object& object = Objects[objectIndex];
				if( object.BaseObjectIndex == BO_24_ANGEL ){
					object.selectable = 0;
				}
			}
			int row = islandQuest.row = Convert40to112(CurPattern.row) + 5; // TODO: change to QuestPatterns(Q_20_SUNLESS_SEA)
			int col = islandQuest.col = Convert40to112(CurPattern.col) + 5;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
		}
	}
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( Dungeon == lazarusQuest.dungeonStart && MaxCountOfPlayersInGame == 1
		&& !Dungeon->isQuest
		&& lazarusQuest.status2 >= LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN
		&& ( lazarusQuest.status == QS_2_IN_PROGRESS || lazarusQuest.status == QS_3_COMPLETE ) ){
		if( lazarusQuest.status3 == LQAS_0 || lazarusQuest.status3 == LQAS_2 ){
			int row = lazarusQuest.row;
			int col = lazarusQuest.col;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
			lazarusQuest.status3 = LQAS_1;
			if (lazarusQuest.status == QS_2_IN_PROGRESS && lazarusQuest.status2 < LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR) {
				lazarusQuest.status2 = LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR;
			}
		}
	}

	// (rt) Add a call for Outbreak
	if (GameMode == GM_OUTBREAK && Dungeon == DUN_1_CHURCH_1) {
		Outbreak_Script();
	}

	if( Dungeon->isQuest ){
		if( Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
			Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
			if( lazarusQuest.status == QS_3_COMPLETE && lazarusQuest.status3 == 4 ){
				int row = 35;
				int col = 32;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
				lazarusQuest.status3 = LQAS_3;
			}
		}
		if( Dungeon == DUN_45_THE_DEN ){
			TheDenScript();
		}
		if (Dungeon == DUN_73_LAVA_PITS) {
			Lava_Pits_Script();
		}

		Quest& poisonedWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
		if( GameMode != GM_DARK_CAMPAIGN								// (th4) DC keeps the water foul, never cleanses
			&& Dungeon == poisonedWaterQuest.questDungeon
			&& poisonedWaterQuest.status != QS_1_ACCEPT
			&& Dungeon->genType == poisonedWaterQuest.dungeonType
			&& MonstersCount == SummonMonsters_Count
			&& poisonedWaterQuest.status != QS_3_COMPLETE ){
				poisonedWaterQuest.status = QS_3_COMPLETE;
				LoadPalette("Levels\\L3Data\\L3pwater.pal");
				WaterPaletteInPoizonRiverQuestCave = 32;
		}
		if( Dungeon == DUN_35_PASSAGE_OF_FIRE ){
			Quest& izualQuest = Quests[Q_17_FALLEN_ANGEL];
			if( izualQuest.status3 == 1 ){ // Seems risky to sync .status3, as it changes quickly from 0->1->0 when taking Red TP.
				int row = 27;
				int col = 40;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
				AddWarpFromFirePassage(1);
				izualQuest.status3 = 0;
			}
		}
		if( Dungeon == DUN_36_VALLEY_OF_DESPAIR ){
			Quest& izualQuest = Quests[Q_17_FALLEN_ANGEL];
			if( izualQuest.status3 == 1 ){
				int row = 18;
				int col = 88;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
				izualQuest.status3 = 0;
			}
		}
		if( Dungeon == DUN_40_ISLAND ){
			Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
			if( islandQuest.status3 == 1 ){
				islandQuest.status3 = 0; // Reset just like Izual Red TP, be careful syncing...
				int row = 55;
				int col = 55;
				CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
			}
		}
		if( Dungeon == DUN_34_THE_PIT ){
			GrimspikeOpenScript(); // In MP, the script only runs in The Pit.  If a player is on a different map, GrimSpike will spawn late for this player!
		}
		Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
		if( islandQuest.questDungeon == Dungeon 
			&& islandQuest.status != QS_3_COMPLETE 
			&& MonstersCount == SummonMonsters_Count ){
				islandQuest.status = QS_3_COMPLETE;
		}
		Quest& clWQuest = Quests[Q_36_CLEAR_WATER];
		if (clWQuest.questDungeon == Dungeon
			&& clWQuest.status != QS_3_COMPLETE
			&& MonstersCount == SummonMonsters_Count) {
			clWQuest.status = QS_3_COMPLETE;
		}
		Quest& iron_Figurine_Quest = Quests[Q_29_IRON_FIGURINE];
		if (iron_Figurine_Quest.questDungeon == Dungeon
			&& iron_Figurine_Quest.status != QS_3_COMPLETE
			&& MonstersCount == SummonMonsters_Count) {
			iron_Figurine_Quest.status = QS_3_COMPLETE;
		}
		Quest& som_Quest = Quests[Q_40_SECRETS_OF_MAGIC];
		if (som_Quest.questDungeon == Dungeon
			&& som_Quest.status != QS_3_COMPLETE
			&& MonstersCount == SummonMonsters_Count) {
			som_Quest.status = QS_3_COMPLETE;
		}
		Quest& demon_Plague_Quest = Quests[Q_30_DEMON_PLAGUE];
		if (demon_Plague_Quest.questDungeon == Dungeon
			&& demon_Plague_Quest.status != QS_3_COMPLETE
			&& MonstersCount == SummonMonsters_Count) {
			demon_Plague_Quest.status = QS_3_COMPLETE;
		}
		Quest& ditt_Quest = Quests[Q_27_DEMON_IN_THE_TOWN];
		if (ditt_Quest.questDungeon == Dungeon
			&& ditt_Quest.status != QS_3_COMPLETE
			&& MonstersCount == SummonMonsters_Count) {
			ditt_Quest.status = QS_3_COMPLETE;
		}
		Quest& lairQuest = Quests[Q_23_INFESTED_CELLAR];
		if( lairQuest.questDungeon == Dungeon 
			&& lairQuest.status != QS_3_COMPLETE 
			&& MonstersCount == SummonMonsters_Count ){
				lairQuest.status = QS_3_COMPLETE;
				// open gates!
				Open_the_Gates_Cellar();
				int soundMessage = S_M1_NO_SOUND;
				switch(player.ClassID){
				case PC_0_WARRIOR:
				case PC_5_SAVAGE:	soundMessage = S_811_WAR_81;	break;
				case PC_1_ARCHER:
				case PC_4_ROGUE:	soundMessage = S_702_ROG_81;	break;
				case PC_2_MAGE:		soundMessage = S_979_SOR_81;	break; // Where Monk? (mor: he doesn't have anything to say about it)
				}
				if( soundMessage != S_M1_NO_SOUND ){
					SoundMessage = soundMessage;
					PlayerSoundMessageDelay = 65;
				}
		}
		if( player.CurAction == PCA_0_STAND ){ // TODO: move this block to CheckTriggers
			DUNGEON nextLocIndex = DUN_0_TOWN;
			if( Dungeon == DUN_30_HALLS_OF_ANGUISH && player.Row == 88 && player.Col == 27 ){
				nextLocIndex = DUN_31_ANDARIELS_THRONE;
			}else if( Dungeon == DUN_31_ANDARIELS_THRONE && player.Row == 49 && player.Col == 26 ){
				nextLocIndex = DUN_30_HALLS_OF_ANGUISH;
			}
			if( nextLocIndex ){
				if( MonstersCount <= SummonMonsters_Count || is(GameMode, GM_EASY, GM_CLASSIC)){
					StartNewLvl(CurrentPlayerIndex, WM_1029_GOTO_QUEST_DUNGEON, nextLocIndex);
				}else{
					AddOnScreenMessage(OM_2_area_not_cleared);
                    NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, player.Row + 1, player.Col + 1 );
					int soundMessage = S_M1_NO_SOUND;
					switch( player.ClassID ){
					case PC_0_WARRIOR:
					case PC_5_SAVAGE:soundMessage = S_772_WAR_43;	break;
					case PC_1_ARCHER:
					case PC_4_ROGUE:	soundMessage = S_663_ROG_43;	break;
					case PC_2_MAGE:		soundMessage = S_560_SOR_43;	break;
					case PC_3_MONK:		soundMessage = S_880_MONK_43;	break;
					}
					if( soundMessage != S_M1_NO_SOUND ){
						PlayGlobalSound(soundMessage);
					}
				}
			}
		}
		if( WaterPaletteInPoizonRiverQuestCave > 0 ){
			ApplyAnimFilePalette(WaterPaletteInPoizonRiverQuestCave--);
		}
		return;
	}
	if( player.CurAction == PCA_0_STAND ){
		if( Dungeon == DUN_0_TOWN && player.Row == 53 && player.Col == 63 ){
            Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
			if( theRampagingDemonQuest.status == QS_2_IN_PROGRESS && theRampagingDemonQuest.status3 == 1 ){
				StartNewLvl(CurrentPlayerIndex, WM_1029_GOTO_QUEST_DUNGEON, theRampagingDemonQuest.questDungeon);
				return;
			}
		}
		for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
			Quest& quest = Quests[questIndex];
			if( Dungeon == quest.dungeonStart 
			 && quest.questDungeon != DUN_NO
			 && quest.status != QS_0_NO 
			 && player.Row == quest.row 
			 && player.Col == quest.col ){
				if (questIndex == Q_15_ARCHBISHOP_LAZARUS && Quests[Q_15_ARCHBISHOP_LAZARUS].status2 < LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR) {
					continue;
				}
				if( auto msg = BlockMessageToQuestDungeon(); msg == OM_0_NO_MESSAGE ){
					StartNewLvl(CurrentPlayerIndex, WM_1029_GOTO_QUEST_DUNGEON, quest.questDungeon); // TODO: check for same logic duplication in CheckTriggers()
				}
				else{
					CannotEnterBeforeClearing(BIT(1)|BIT(2), msg);
				}
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
ONSCREEN_MESSAGE BlockMessageToQuestDungeon()
{	
	ONSCREEN_MESSAGE msg = OM_0_NO_MESSAGE;
	if( is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN, GM_DARK_CAMPAIGN) || SaveAlwaysEnabled
		|| (MaxCountOfPlayersInGame != 1 && not(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) ){ // Easy, Speedrun, Dark Campaign, Multi, Cheat -> always can
		// can go
	}
	else if (GameMode == GM_SURVIVAL) {
		if (GetNumberOfMonstersInDungeon() > 0 || GetNumberOfObjectsInDungeon() > MaxIronmanObjects) msg = OM_2_area_not_cleared;
	}
	else if( ! Dungeon->isQuest ){ // from Non-Quest -> if not in battle
		if( IsSomeMonstersActivated() ) msg = OM_4_battle_not_over;
	}
	else if( is(GameMode, GM_NORMAL, GM_HARD, GM_HARDCORE, GM_NIGHTMARE) ){ // Normal, Hard, HC, NM from Quest level -> fully cleared of monsters
		if( GetNumberOfMonstersInDungeon() > 0 ) msg = OM_2_area_not_cleared;
	}	
	else { //if (is(GM_IRONMAN, GM_SURVIVAL)) { // Ironman & Survival from quest level -> fully cleared of monsters and objects
		if( GetNumberOfMonstersInDungeon() > 0 || GetNumberOfObjectsInDungeon() > MaxIronmanObjects ) msg = OM_2_area_not_cleared;
	}
	return msg;
}

//----- (0045F9F2) --------------------------------------------------------
bool ShowDungeonEntryDescriptionsFromQuestLocEntries()
{
	// Displays quest area entrance info in MP
	for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
		if (questIndex == Q_15_ARCHBISHOP_LAZARUS && Quests[Q_15_ARCHBISHOP_LAZARUS].status2 < LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR) continue; // Don't highlight entrance on cursor before handing staff to Cain.  Same condition applies to changing warp access.
		Quest& quest = Quests[questIndex];
		if (quest.dungeonStart == Dungeon && quest.questDungeon != DUN_NO && quest.status != QS_0_NO) {
			// While the Valor vault is still closed, its teleport pad (to Gateway of Blood)
			// must not reveal transition info on hover.
			if ((questIndex == Q_9_VALOR /*|| questIndex == Q_39_VALOR_MP*/) && !IsValorVaultOpen()) {
				continue;
			}
			int row = quest.row;
			int col = quest.col;
			for( int deltaRangeIndex = 0; deltaRangeIndex < 7; ++deltaRangeIndex ){
				if( row + MapDeltaRangeRow[deltaRangeIndex] == Cur.Row 
				 && col + MapDeltaRangeCol[deltaRangeIndex] == Cur.Col ){
					sprintf(InfoPanelHeader, L("To %s"), LocalizedWarpNameAccusative(Dungeons[Quests[quest.baseId].questDungeon].warpName)); // (th4) localize warp hover
					if( Dungeons[Quests[quest.baseId].questDungeon].warpName2 ){
						InfoPanel_AddLine(LocalizedWarpName(Dungeons[Quests[quest.baseId].questDungeon].warpName2), true); // (th4) localize 2nd line
					}
					Cur.Row = row;
					Cur.Col = col;
					return true;
				}
			}
		}
	}
	return false;
}

//----- (0045FA93) --------------------------------------------------------
bool __fastcall IsQuestOnLevel( int questIndex ) // QuestStatus
{
	Quest& quest = Quests[questIndex];
	return ! Dungeon->isQuest 
		&& Dungeon == quest.dungeonStart 
		&& quest.status != QS_0_NO
		&& BaseQuests[questIndex].flag & (MaxCountOfPlayersInGame == 1 ? QF_SINGLE : QF_MULTI); // TODO: looks like this line is not needed, we set status in InitQuests
}


//----- (00717B20) --------------------------------------------------------
void HorazonQuestCompleteCheck ()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status3 == 6 && horazonQuest.status2 == 4 ){
		horazonQuest.status = QS_3_COMPLETE;
	}
}

//----- (0045FAD2) --------------------------------------------------------
void __fastcall SayMessageOnBossKill(int monsterIndex, int isNetData)
{
	Monster& monster = Monsters[monsterIndex];
	Player& player = Players[CurrentPlayerIndex];
	int baseMonsterIndex = monster.SpritePtr->baseMonsterIndex;
	int bossIndex = monster.newBossId - 1;

	// TODO: change all this ifs to switch
	if( baseMonsterIndex == BM_50_SKELETON_LORD  ||  baseMonsterIndex == BM_C_391_LEORIC){
		Quests[Q_12_THE_CURSE_OF_KING_LEORIC].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:		SoundMessage = S_812_WAR_82 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_703_ROG_82;		break;
			case PC_2_MAGE:			SoundMessage = S_600_SOR_82;		break;
			case PC_3_MONK:			SoundMessage = S_920_MONK_82;		break;
		}
		if( isNetData ){
            NetSendCmdQuest( true, Q_12_THE_CURSE_OF_KING_LEORIC );
		}
		return;
	}
	if( bossIndex == UM_579_Andariel ){
		//if( MaxCountOfPlayersInGame != 1 ){ // I think this should be commented out...
		//	return;
		//}
		Quests[Q_22_MAIDEN_OF_ANGUISH].status = QS_3_COMPLETE;
//		if (isNetData) {
//			NetSendCmdQuest(true, Q_22_MAIDEN_OF_ANGUISH); // This may be needed to prevent an edge case quest reward abuse.  Will need >= 3 players to test...  This will have the drawback that other players staying in town can finish quest for free...
//		}
		int soundMessage = S_M1_NO_SOUND;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:		soundMessage = S_814_WAR_84 ;	break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		soundMessage = S_705_ROG_84;	break;
			case PC_2_MAGE:			soundMessage = S_981_SOR_84;	break;
		}
		if( soundMessage != S_M1_NO_SOUND ){
			PlayerSoundMessageDelay = 30;
			SoundMessage = soundMessage;
		}
		return;
	}
	if( bossIndex == UM_580_NaKrul ){
		Quests[Q_16_HORAZON_S_DEMONS].status2 = 4;
		HorazonQuestCompleteCheck();
		return;
	}
	if( bossIndex == UM_581_Grimspike ){
		Quests[Q_16_HORAZON_S_DEMONS].status3 = 6;
		HorazonQuestCompleteCheck();
		return;
	}
	if( bossIndex == UM_582_Izual ){
		Quests[Q_17_FALLEN_ANGEL].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 120;
		switch( player.ClassID ){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:	SoundMessage = S_815_WAR_85;	break;
		case PC_1_ARCHER:	
		case PC_4_ROGUE:	SoundMessage = S_706_ROG_85;	break;
		case PC_2_MAGE:		SoundMessage = S_1024;			break;
		case PC_3_MONK:		SoundMessage = S_609_SOR_SL;	break;
		}
		return;
	}
	if( bossIndex == UM_583_Lich_King ){
		// (th4) DC mission 19 borrows him for the Necropolis. His vanilla death handler opens
		// the Frozen Loch gates, which rewrites tiles wherever the player happens to be standing,
		// and completes Q_19 in a save that has no business carrying it. Neither belongs in DC.
		if( GameMode == GM_DARK_CAMPAIGN ) return;
		Quests[Q_19_LOST_TREASURE].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_19_LOST_TREASURE);//This makes sure easy map access is enabled also for players on different maps.
		}
		if (Dungeon == DUN_38_GLACIAL_CAVERNS) {
			Open_the_Gates_Glacial_Caverns();
		}
		Open_the_Gates_Frozen_Loch();
		return;
	}
	if (bossIndex == UM_725_Web_Queen) {
		if (is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_SURVIVAL, GM_NIGHTMARE)) {
			Quests[Q_33_STRANGE_BOOK].status = QS_4_TOOWEAK1;
			if (isNetData) {
				NetSendCmdQuest(true, Q_33_STRANGE_BOOK);//This makes sure easy map access is enabled also for players on different maps.
			}
			Open_the_Gates_Arachnid_Lair();
			if (Dungeon == DUN_56_WEB_TUNNELS) {
				Open_the_Gates_Web_Tunnels();
			}
		}
		return;
	}
	if (bossIndex == UM_706_Mudsnout) {
		Quests[Q_29_IRON_FIGURINE].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_29_IRON_FIGURINE);//This makes sure easy map access is enabled also for players on different maps.
		}
		Open_the_Gates_Fetid_Cavern();
		return;
	}
	if (bossIndex == UM_805_Festerus) {
		Quests[Q_36_CLEAR_WATER].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_36_CLEAR_WATER);
		}
		Open_the_Gates_Springs();
		return;
	}
	if (bossIndex == UM_707_Lichen_Axe_the_Fungal) {
		Quests[Q_30_DEMON_PLAGUE].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_30_DEMON_PLAGUE);//This makes sure easy map access is enabled also for players on different maps.
		}
		Open_the_Gates_Mossy_Grotto();
		return;
	}
	// not sure how to check for two boss kills here
	/*if (bossIndex == UM_903_AZGOR && bossIndex == UM_904_DOBAZZU) {
		Quests[Q_40_SECRETS_OF_MAGIC].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_40_SECRETS_OF_MAGIC);//This makes sure easy map access is enabled also for players on different maps.
		}
		//Open_the_Gates_Shimmer_Flats();
		return;
	}*/
	if (bossIndex == UM_708_Grimwing) {
		Quests[Q_31_GOOD_DEAL].status = QS_3_COMPLETE;
		return;
	}
	if (bossIndex == UM_713_Sir_Valon) {
		Quests[Q_32_A_SIMPLE_TASK].status = QS_3_COMPLETE;
		return;
	}
	if (bossIndex == UM_703_Hive_Mother) {
		Quests[Q_26_SPECIAL_WEAPON].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 51;
		switch (player.ClassID) {
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:	SoundMessage = S_807_WAR_77;	break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_698_ROG_77;	break;
		case PC_2_MAGE:		SoundMessage = S_1789;			break;
		case PC_3_MONK:		SoundMessage = S_1790;			break;
		}
		if (isNetData) {
			NetSendCmdQuest(true, Q_26_SPECIAL_WEAPON);//This makes sure easy map access is enabled also for players on different maps.
		}
		Open_the_Gates_Blood_Hive();
		return;
	}
	if (bossIndex == UM_705_Bloodhoof_Twinaxe) {
		Quests[Q_28_VISIONS_OF_VICTORY].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 43;
		switch (player.ClassID) {
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:	SoundMessage = S_787_WAR_58;	break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_678_ROG_58;	break;
		case PC_2_MAGE:		SoundMessage = S_1729;			break;
		case PC_3_MONK:		SoundMessage = S_1360;			break;
		}
		if (isNetData) {
			NetSendCmdQuest(true, Q_28_VISIONS_OF_VICTORY);//This makes sure easy map access is enabled also for players on different maps.
		}
		Open_the_Gates_Temple_of_Sunset();
		return;
	}
	if (bossIndex == UM_726_Apparition_of_Arkaine) {
		if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian, TraitId::OldFashioned)) {
			Quests[Q_9_VALOR].status = QS_3_COMPLETE;			
		}
		Quests[Q_39_VALOR_MP].status = QS_3_COMPLETE;
		return;
	}
	if (bossIndex == UM_790_Ghost_of_Brother_Martin) {
		Quests[Q_34_GRAVE_MATTERS].status = QS_3_COMPLETE;
		if (MaxCountOfPlayersInGame == 1) {
			Open_the_Gates_Black_Chapel();
		}
		return;
	}
	if (bossIndex == UM_791_Festergnaw) {
		Quests[Q_35_IRON_OATH].status = QS_3_COMPLETE;
		Open_the_Gates_Sacristy();
		return;
	}
	if( bossIndex == UM_585_Fleshdoom ){
		if( Quests[ Q_21_THE_CURSED_BLADE ].status2 < 2 ){
			Quests[ Q_21_THE_CURSED_BLADE ].status2 = 2;
			if (isNetData) {
				NetSendCmdQuest(true, Q_21_THE_CURSED_BLADE);//This makes sure easy map access is enabled also for players on different maps.
			}
			Open_the_Gates_Fleshdoom_level();
		}
		return;
	}
	if (bossIndex == UM_586_Wielder_of_Shadowfang) {
		if (Quests[Q_21_THE_CURSED_BLADE].status4 < 1) {
			Quests[Q_21_THE_CURSED_BLADE].status4 = 1;
			if (isNetData) {
				NetSendCmdQuest(true, Q_21_THE_CURSED_BLADE);//This should be required for InquisitionScript() to work properly as it now checks for .status4 to reduce performance hog.
			}
			Open_the_Gates_Fleshdoom_level_2();//This shortcut is sync'd even if player is on a different map.
		}
		return;
	}
	if( bossIndex == UM_805_Festerus ){
		Quests[ Q_36_CLEAR_WATER].status = QS_3_COMPLETE;
		if( isNetData ){
			NetSendCmdQuest(true, Q_36_CLEAR_WATER);
		}
	}
	if (bossIndex == UM_809_Borosteron) {
		Quests[Q_37_PRICE_OF_HUBRIS].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_37_PRICE_OF_HUBRIS);
		}
	}
	if (bossIndex == 821) {
		Quests[Q_38_OGDENS_SIGN_MP].status = QS_3_COMPLETE;
		if (isNetData) {
			NetSendCmdQuest(true, Q_38_OGDENS_SIGN_MP);
		}
	}
	if( bossIndex == UM_154_Diablo ){
		Quests[Q_5_DIABLO].status = QS_3_COMPLETE;
		return;
	}
	if( bossIndex == UM_678_Warden_of_Anger && MaxCountOfPlayersInGame != 1 ){
		Quests[Q_10_ANVIL_OF_FURY].status = QS_3_COMPLETE; // This check is for preventing quest reward abusing in MP, see InitItemsOnMap().
		if (isNetData) {
			NetSendCmdQuest(true, Q_10_ANVIL_OF_FURY);
		}
		return;
	}
	if( is(baseMonsterIndex, BM_51_THE_BUTCHER, BM_C_390_BUTCHER) ){
		// (th4) DC mission 25: the Butcher is an ally there, and the heroes killing him is a
		// failure, not a quest completed. No "the spirits of the dead are now avenged", and
		// Q_6 is left alone -- it is not the player's kill and not his quest.
		if( GameMode == GM_DARK_CAMPAIGN ){
			return;
		}
		Quests[Q_6_THE_BUTCHER].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:SoundMessage = S_810_WAR_80 ;	break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_701_ROG_80;	break;
		case PC_2_MAGE:		SoundMessage = S_598_SOR_80;	break;
		case PC_3_MONK:		SoundMessage = S_918_MONK_80;	break;
		}
		if( isNetData ){
            NetSendCmdQuest( true, Q_6_THE_BUTCHER );
		}
		return;
	}
	if( is( bossIndex, UM_0_Gharbad_the_Weak, UM_C_849_GHARBAD ) ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quests[Q_2_GHARBAD_THE_WEAK].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_790_WAR_61 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_681_ROG_61;			break;
			case PC_2_MAGE:			SoundMessage = S_578_SOR_61;		break;
			case PC_3_MONK:			SoundMessage = S_898_MONK_61;			break;
		}
		return;
	}
	if( is( bossIndex, UM_2_Zhar_the_Mad, UM_C_870_ZHAR_THE_MAD ) ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quests[Q_3_ZHAR_THE_MAD].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_791_WAR_62 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_682_ROG_62;			break;
			case PC_2_MAGE:			SoundMessage = S_579_SOR_62;		break;
			case PC_3_MONK:			SoundMessage = S_899_MONK_62;			break;
		}
		return;
	}
	if( is( bossIndex, UM_4_ArchBishop_Lazarus, UM_C_867_LAZARUS ) && MaxCountOfPlayersInGame != 1 ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		lazarusQuest.status = QS_3_COMPLETE;
		lazarusQuest.status2 = 7;
		PlayerSoundMessageDelay = 30;
		Quests[Q_5_DIABLO].status = QS_2_IN_PROGRESS;

		for( int col = 0; col < 112; ++col ) for( int row = 0; row < 112; ++row ){
			if( FineMap[ row ][ col ] == 370 ) AddWarp(row, col, WM_1026_NEXT_DUNGEON);
		}
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_813_WAR_83 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_704_ROG_83;			break;
			case PC_2_MAGE:			SoundMessage = S_601_SOR_83;		break;
			case PC_3_MONK:			SoundMessage = S_921_MONK_83;			break;
		}
		if( isNetData ){
            NetSendCmdQuest( true, Q_15_ARCHBISHOP_LAZARUS );
            NetSendCmdQuest( true, Q_5_DIABLO );
		}
		return;
	}
	if( is( bossIndex, UM_4_ArchBishop_Lazarus, UM_C_867_LAZARUS ) && MaxCountOfPlayersInGame == 1 ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		lazarusQuest.status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		// (th4) DC mission 13: DUN_121 is the Dark Campaign's own lair, not the vanilla one.
		// If Lazarus dies there this block would run the whole normal-campaign aftermath --
		// AddWarpFromUnholyAltar() drops a WM_1028 warp to DUN_15_HELL_3 on 35,32 and the
		// missile below puts a red portal on top of it, so a failed mission handed the player
		// a way out to dungeon level 15. It also completes the Lazarus quest and starts the
		// Diablo one, in a campaign where neither exists. Excluded the same way the Void
		// Chambers Lazarus already is.
		// (th4) DC mission 13: the Arch-Bishop was the mission. Losing him is a failure, so
		// say so -- the same confirmation sting F12 plays for a hero murder in Ironman
		// (MenuEngine.cpp), plus the on-screen line. Nothing else happens: no portal, no
		// quest progress, no victory shout, all of which the guard below withholds.
		if( Dungeon == DUN_121_UNHOLY_ALTAR_DC ){
			PlayGlobalSound( S_1216_PIECE_1 );
			AddOnScreenMessage( OM_113_DC_Mission_Failed );
		}
		if (Dungeon != DUN_101_VOID_CHAMBERS && Dungeon != DUN_121_UNHOLY_ALTAR_DC) {
			AddWarpFromUnholyAltar();
			lazarusQuest.status2 = 7;
			lazarusQuest.status3 = 4;
			Quests[Q_5_DIABLO].status = QS_2_IN_PROGRESS;
			int row = 35;
			int col = 32;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0);
		}
		// (th4) DC mission 13: "Your madness ends here, betrayer" is the normal campaign's
		// line for slaying the traitor Arch-Bishop. On DUN_121 he is the player's own side
		// and killing him is a failed mission, not a triumph -- so no gloating.
		if( Dungeon != DUN_121_UNHOLY_ALTAR_DC ){
			switch( player.ClassID ){
				case PC_0_WARRIOR:
				case PC_5_SAVAGE:	SoundMessage = S_813_WAR_83 ;		break;
				case PC_1_ARCHER:
				case PC_4_ROGUE:		SoundMessage = S_704_ROG_83;			break;
				case PC_2_MAGE:			SoundMessage = S_601_SOR_83;		break;
				case PC_3_MONK:			SoundMessage = S_921_MONK_83;			break;
			}
		}
		return;
	}
	// (th4) DC MISSION 15: Hephasto was the mission. Losing him is a failure -- same
	// treatment as Lazarus in mission 13: the Ironman hero-murder sting, the on-screen
	// line, and nothing else. The completion gate in QuestsActions holds the way home
	// because DarkCampaignHephastoThanked can never be set once he is dead, so the run
	// is over. Deliberate.
	if( bossIndex == UM_587_Hephasto_the_Armorer
	 && GameMode == GM_DARK_CAMPAIGN && Dungeon == DUN_43_HEPHASTO ){
		PlayGlobalSound( S_1216_PIECE_1 );
		AddOnScreenMessage( OM_113_DC_Mission_Failed );
	}

	if( is( bossIndex, UM_8_Warlord_of_Blood, UM_C_862_WARLORD ) ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quests[Q_11_WARLORD_OF_BLOOD].status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_824_WAR_94 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_715_ROG_94;			break;
			case PC_2_MAGE:			SoundMessage = S_612_SOR_94;		break;
			case PC_3_MONK:			SoundMessage = S_932_MONK_94;			break;
		}
		return;
	}
	if( bossIndex == UM_595_The_Cerberus ){ // identity check, not monster.Name (localized name breaks strcmp)
		Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
		if (theRampagingDemonQuest.status2 == 4) {
			theRampagingDemonQuest.status2 = 5;
			CloseDenPortals();
		}
		return;
	}
}

// TODO: check WTF is this
//----- (0045FF59) --------------------------------------------------------
void AddButcherHallTrans() // DrawButcher
{
	for( QuestPattern& qp: QuestPatterns ) if( qp.id == Q_6_THE_BUTCHER && qp.placed ){
		SetRectTrans(Convert40to112(qp.row) + 3, Convert40to112(qp.col) + 3, Convert40to112(qp.row) + 10, Convert40to112(qp.col) + 10);
	}

}

//----- (0045FF80) --------------------------------------------------------
void __fastcall SetWarpToLeoricQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 12;
	quest.col = Convert40to112(col) + 7;
}

//----- (th3) -------------------------------------------------------------
void __fastcall SetWarpTo_VaultKeep_QuestFloor(int questIndex, int row, int col)
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 6;
	quest.col = Convert40to112(col) + 7;
}

//----- (0045FF9E) --------------------------------------------------------
void __fastcall ApplyWarlordDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L4Data\\Warlord2.DUN");
	CurPattern.row = row;
	CurPattern.col = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	CurPattern.sizeRow = patternMapRows;
	CurPattern.sizeCol = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}else{
				Map[ row + rowOffset ][ col + colOffset ] = HELL_WAFM_6_FLOOR;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//----- (0046002F) --------------------------------------------------------
void __fastcall ApplyBonechamberDunAndSetWarp(int questIndex, int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L2Data\\Bonestr1.DUN");
	CurPattern.row = row;
	CurPattern.col = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	CurPattern.sizeRow = patternMapRows;
	CurPattern.sizeCol = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}else{
				Map[ row + rowOffset ][ col + colOffset ] = CATA_WAFM_3_FLOOR;
			}
		}
	}
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 6;
	quest.col = Convert40to112(col) + 7;
	FreeMem(dunFilePtr);
}

//----- (004600E7) --------------------------------------------------------
void __fastcall ApplyBannerAltDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L1Data\\Banner1.DUN");
	CurPattern.row = row;
	CurPattern.col = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	CurPattern.sizeRow = patternMapRows;
	CurPattern.sizeCol = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				AltMap[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//----- (00460174) --------------------------------------------------------
void __fastcall ApplyBlindAltDun(int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile("Levels\\L2Data\\Blind1.DUN");
	CurPattern.row = row;
	CurPattern.col = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	CurPattern.sizeRow = patternMapRows;
	CurPattern.sizeCol = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				AltMap[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
}

//----- (00460201) --------------------------------------------------------
void __fastcall ApplyValorDun(int questIndex, int row, int col)
{
	ushort* dunFilePtr = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blood2_cls.DUN" : 
		MaxCountOfPlayersInGame == 1 ? "Levels\\L2Data\\Blood2.DUN" : 
		"Levels\\L2Data\\blood_MP_Ntree.dun");// MP
	CurPattern.row = row;
	CurPattern.col = col;
	ushort patternMapRows = dunFilePtr[0];
	ushort patternMapCols = dunFilePtr[1];
	CurPattern.sizeRow = patternMapRows;
	CurPattern.sizeCol = patternMapCols;
	int patMapOffset = 2;
	for( int colOffset = 0; colOffset < patternMapCols; colOffset++ ){
		for( int rowOffset = 0; rowOffset < patternMapRows; rowOffset++ ){
			int newTile = dunFilePtr[ 2 + rowOffset + colOffset * patternMapRows ];
			if( newTile ){
				Map[ row + rowOffset ][ col + colOffset ] = newTile;
			}
		}
	}
	FreeMem(dunFilePtr);
	//addon
	if (GameMode != GM_CLASSIC) {
		int x = 9, y = 2;
		if (MaxCountOfPlayersInGame != 1) {
			x -= 3; y += 3;
		}
		Quest& quest = Quests[questIndex];
		quest.row = Convert40to112(row) + x;
		quest.col = Convert40to112(col) + y;
	}
	
}

//----- (00716950) --------------------------------------------------------
void __fastcall SetWarpToHorazonQuestFloor( int questIndex, int row, int col )
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 5;
	quest.col = Convert40to112(col) + 8;
}

void __fastcall SetWarpToTempleOfSunsetQuestFloor(int questIndex, int row, int col)
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 5;
	quest.col = Convert40to112(col) + 8;
}

void __fastcall SetWarpTo_Abandoned_Fane_QuestFloor(int questIndex, int row, int col)
{
	Quest& quest = Quests[questIndex];
	quest.row = Convert40to112(row) + 2;
	quest.col = Convert40to112(col) + 2;
}

//----- (th4) -------------------------------------------------------------
void SetWarpToQuestPattern(QUEST qi)
{
	for( QuestPattern& qp: QuestPatterns ){
		if( qp.id == qi && qp.placed && !(qp.flag & QP_PORTAL) ){
			Quest& quest = Quests[qi];
			quest.row = Convert40to112(qp.row) + qp.ofsRow;
			quest.col = Convert40to112(qp.col) + qp.ofsCol;
		}
	}
}

//----- (0046028E) --------------------------------------------------------
void __fastcall AddDungeonQuestPatternToWallMap( int startRow, int startCol ) // DRLG_CheckQuests
{
	for( BaseQuest& bq: BaseQuests ){
		QUEST qi = bq.id;
		if( IsQuestOnLevel(qi) ){
			switch(qi){
			case Q_6_THE_BUTCHER:				AddButcherHallTrans                ();                       break;
			case Q_7_OGDENS_SIGN:				ApplyBannerAltDun                  (    startRow, startCol); break;
			case Q_8_HALLS_OF_THE_BLIND:		ApplyBlindAltDun                   (    startRow, startCol); break;
			case Q_9_VALOR or Q_39_VALOR_MP:	ApplyValorDun                      (qi, startRow, startCol); break;
			case Q_11_WARLORD_OF_BLOOD:			ApplyWarlordDun                    (    startRow, startCol); break;
			case Q_12_THE_CURSE_OF_KING_LEORIC:	SetWarpToLeoricQuestFloor          (qi, startRow, startCol); break;
			case Q_14_THE_CHAMBER_OF_BONE:		ApplyBonechamberDunAndSetWarp      (qi, startRow, startCol); break;
			case Q_16_HORAZON_S_DEMONS:			SetWarpToHorazonQuestFloor         (qi, startRow, startCol); break;
			case Q_28_VISIONS_OF_VICTORY:		SetWarpToTempleOfSunsetQuestFloor  (qi, startRow, startCol); break;
			case Q_31_GOOD_DEAL:				SetWarpTo_VaultKeep_QuestFloor     (qi, startRow, startCol); break;
			case Q_32_A_SIMPLE_TASK:			SetWarpTo_Abandoned_Fane_QuestFloor(qi, startRow, startCol); break;
			default: SetWarpToQuestPattern(qi); break;
			}
		}
	}
}

//----- (0046031F) --------------------------------------------------------
void SetReturnDungeonPos(DUNGEON dungeon) // TODO: fix double call on change level (then in LoadGameLevel)
{
	QUEST quest = Q_NO;
	int row = 0, col = 0;

	// set special dungeon return position/quest (e.g. dungeon not presented in BaseQuests[])
	switch( dungeon ){
	case DUN_59_HALL_OF_FIRE: quest = Q_9_VALOR; break;
	case DUN_44_RAVENHOLM:
		// After SP player saving in Ravenholm, exiting game, loading save, would end up at Town's (1,0) coordinates after taking red portal
		// since NPC doesn't exist in Ravenholm so its coordinates = (0,0)
		#if 0
		row = Towners[TI_3_OGDEN].Row + 1;
		rol = Towners[TI_3_OGDEN].Col;
		#endif
		row = 54; // Need to match red portal position, with a small shift.
		col = 64;
		break;
	case DUN_46_COLISEUM:
		// May have return coords bug as well if allowed in SP, but keep it unchanged for now...
		row = Towners[TI_3_OGDEN].Row + 1;
		col = Towners[TI_3_OGDEN].Col;
		break;
	}

	// find quest for dungeon
	if( quest == Q_NO ) for( Quest& q: Quests ){
		if( q.questDungeon == dungeon ){
			quest = q.baseId;
			break;
		}
	}
	if( quest == Q_NO ) quest = Q_24_THE_RAMPAGING_DEMON;
	
	// set return position by quest entry coord + base return offsets
	Dungeon.returnRow = row ? row : Quests[quest].row + BaseQuests[quest].retRow;
	Dungeon.returnCol = col ? col : Quests[quest].col + BaseQuests[quest].retCol;
	Dungeon.returnLvl = Quests[quest].dungeonStart;

	// avoid zero coordinates crazy jumps
	if( ! Dungeon.returnRow || ! Dungeon.returnCol ){
		Dungeon.returnRow = 54;
		Dungeon.returnCol = 64;
		Dungeon.returnLvl = DUN_0_TOWN;
	}
}

//----- (004603DE) --------------------------------------------------------
void GetReturnLvlPos()
{
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( lazarusQuest.status == QS_3_COMPLETE ){
		lazarusQuest.status3 = LQAS_2;
	}
	PlayerRowPos = Dungeon.returnRow;
	PlayerColPos = Dungeon.returnCol;
}

//----- (00460417) --------------------------------------------------------
void ChangeNearestQuestStatus()
{
	{
		Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
		if( kingLeoricQuest.status == QS_1_ACCEPT 
			&& (DUNGEON)Dungeon >= kingLeoricQuest.dungeonStart - 1 
			&& (DUNGEON)Dungeon <= kingLeoricQuest.dungeonStart + 1 ){
				kingLeoricQuest.status = QS_2_IN_PROGRESS;
            NetSendCmdQuest( true, Q_12_THE_CURSE_OF_KING_LEORIC );
		}
	}

	{
		Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
		if( butcherQuest.status == QS_1_ACCEPT 
			&& (DUNGEON)Dungeon >= butcherQuest.dungeonStart - 1 
			&& (DUNGEON)Dungeon <= butcherQuest.dungeonStart + 1 ){
				butcherQuest.status = QS_2_IN_PROGRESS;
            NetSendCmdQuest( true, Q_6_THE_BUTCHER );
		}
	}

	{
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		if( lazarusQuest.status == QS_1_ACCEPT 
			&& (DUNGEON)Dungeon == lazarusQuest.dungeonStart - 1 ){
				lazarusQuest.status = QS_2_IN_PROGRESS;
            NetSendCmdQuest( true, Q_15_ARCHBISHOP_LAZARUS );
		}
	}

	if( IsQuestOnLevel(Q_15_ARCHBISHOP_LAZARUS) ){
		AddObject(BO_83_ALTBOY, Convert40to112(CurPattern.row) + 4, Convert40to112(CurPattern.col) + 6);
	}
}

//----- (0046057F) --------------------------------------------------------
void ResyncQuests()
{
	Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	if( GameMode != GM_DARK_CAMPAIGN								// (th4) DC manages this level's palette itself (always foul)
	 && Dungeon->isQuest 
	 && Dungeon == poisonWaterQuest.questDungeon 
	 && poisonWaterQuest.status != QS_1_ACCEPT 
	 && Dungeon->genType == poisonWaterQuest.dungeonType ){
		char* palette;
		if( poisonWaterQuest.status == QS_3_COMPLETE ){
			palette = "Levels\\L3Data\\L3pwater.pal";
		}else{
			palette = "Levels\\L3Data\\L3pfoul_th3.pal";
		}
		LoadPalette(palette);
	
		/*for( int i = 0; i <= 32; ++i ){ //Duke - I dont see the difference with and without, except the loading screen no longer has distored colors
			ApplyAnimFilePalette(i); // Water is colored somewhere else
		}*/
	}

	Quest& ogdenSignQuest = Quests[Q_7_OGDENS_SIGN];
	if( IsQuestOnLevel(Q_7_OGDENS_SIGN) ){
		if( ogdenSignQuest.status2 == 1 ){
			ChangeMapRectAfterScriptEvent2(	CurPattern.sizeRow + CurPattern.row - 2, 
											CurPattern.sizeCol + CurPattern.col - 2, 
											CurPattern.sizeRow + CurPattern.row + 1, 
											CurPattern.sizeCol + CurPattern.col + 1);
		}else if( ogdenSignQuest.status2 == 2 ){
			ChangeMapRectAfterScriptEvent2(	CurPattern.sizeRow + CurPattern.row - 2, 
											CurPattern.sizeCol + CurPattern.col - 2, 
											CurPattern.sizeRow + CurPattern.row + 1, 
											CurPattern.sizeCol + CurPattern.col + 1);
			ChangeMapRectAfterScriptEvent2(	CurPattern.row, 
											CurPattern.col, 
											(CurPattern.sizeRow / 2) + CurPattern.row + 2, 
											(CurPattern.sizeCol / 2) + CurPattern.col - 2);
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				SetDunTileForObject(ObjectActive[objectIndexIndex]);
			}
			char temp = TransValue;
			TransValue = 9;
			SetAreaTransparencyByIndex_40(	CurPattern.row, 
											CurPattern.col, 
											(CurPattern.sizeRow / 2) + CurPattern.row + 4, 
											(CurPattern.sizeCol / 2) + CurPattern.col);
			TransValue = temp;
		}else if( ogdenSignQuest.status2 == 3 ){
			ChangeMapRectAfterScriptEvent2(	CurPattern.row, 
											CurPattern.col, 
											CurPattern.sizeRow + CurPattern.row + 1, 
											CurPattern.sizeCol + CurPattern.col + 1);
			for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
				SetDunTileForObject(ObjectActive[objectIndexIndex]);
			}
			char temp = TransValue;
			TransValue = 9;
			SetAreaTransparencyByIndex_40(	CurPattern.row, 
											CurPattern.col, 
											(CurPattern.sizeRow / 2) + CurPattern.row + 4, 
											(CurPattern.sizeCol / 2) + CurPattern.col);
			TransValue = temp;
		}
	}

	Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
	if( Dungeon == blackMushroomQuest.dungeonStart && !Dungeon->isQuest){
		if( blackMushroomQuest.status == QS_1_ACCEPT && blackMushroomQuest.status2 == 0 ){
			SpawnSomething(BI_19_FUNGAL_TOME, 0, 0, 0, 5, 1, 1);
			blackMushroomQuest.status2 = 1;
		}else if( blackMushroomQuest.status == QS_2_IN_PROGRESS ){
			if( blackMushroomQuest.status2 < 5 ){
				if( blackMushroomQuest.status2 >= 7 ){// (rt) TODO: never runs
					TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_M1;
				}
			}else{
				TownersSpeechAboutQuestArray[TI_6_ADRIA][Q_1_BLACK_MUSHROOM] = SP_M1;
				TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_123;
			}
		}
	}
	if(0){ // turned off respawning of golden elixir
		Quest& lachdananQuest = Quests[Q_4_LACHDANAN];
		if( (DUNGEON)Dungeon == lachdananQuest.dungeonStart + 1 // drop on the same level with Lachdanan
		 && lachdananQuest.status == QS_2_IN_PROGRESS 
		 && lachdananQuest.status2 == 0 ){
			lachdananQuest.status2 = 1;
			SpawnSomething(BI_328_GOLDEN_ELIXIR, 0, 0, 0, 5, 1, 1);
		}
	}
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( Dungeon->isQuest && Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
		if( lazarusQuest.status2 >= LQS_4_READ_BOOK_IN_UNHOLY_ALTAR ){
			ChangeMapRectAfterScriptEvent2(1, 11, 20, 18);
		}
		if( lazarusQuest.status2 >= LQS_6_FIGHT_WITH_LAZARUS ){
			ChangeMapRectAfterScriptEvent2(1, 18, 20, 24);
		}
		if( lazarusQuest.status2 >= LQS_7_LAZARUS_DEAD ){
			AddWarpFromUnholyAltar();
		}
		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			SetDunTileForObject(ObjectActive[objectIndexIndex]);
		}
	}

	Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
	if( IsQuestOnLevel(Q_20_SUNLESS_SEA) && islandQuest.status2 >= 1 ){
		islandQuest.status3 = 1;
	}

	Quest& iron_Figurine_Quest = Quests[Q_29_IRON_FIGURINE];
	if (IsQuestOnLevel(Q_29_IRON_FIGURINE) && iron_Figurine_Quest.status2 >= 1) {
		iron_Figurine_Quest.status3 = 1;
	}

	Quest& som_Quest = Quests[Q_40_SECRETS_OF_MAGIC];
	if (IsQuestOnLevel(Q_40_SECRETS_OF_MAGIC) && som_Quest.status2 >= 1) {
		som_Quest.status3 = 1;
	}

	Quest& clW_Quest = Quests[Q_36_CLEAR_WATER];
	if (IsQuestOnLevel(Q_36_CLEAR_WATER) && clW_Quest.status2 >= 1) {
		clW_Quest.status3 = 1;
	}

	Quest& demon_Plague_Quest = Quests[Q_30_DEMON_PLAGUE];
	if (IsQuestOnLevel(Q_30_DEMON_PLAGUE) && demon_Plague_Quest.status2 >= 1) {
		demon_Plague_Quest.status3 = 1;
	}

	if( ( Dungeon == lazarusQuest.dungeonStart && !Dungeon->isQuest ) 
		&& ( lazarusQuest.status3 == LQAS_1 || lazarusQuest.status3 >= LQAS_3 ) 
		&& ( lazarusQuest.status == QS_2_IN_PROGRESS || lazarusQuest.status == QS_3_COMPLETE ) ){
			lazarusQuest.status3 = LQAS_2;
	}
}

// PWS water will remain blue after player finishes quest and re-enters PWS.
// This function is reserved for MP quests only, in addition to ChangeNearestQuestStatus().  Can copy over more code in the SP version of ResyncQuests() as we add more quests to MP.
// No more conflicts in Lazarus quest status update.
void MPResyncQuests()
{
	Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	if (GameMode != GM_DARK_CAMPAIGN								// (th4) DC keeps the water foul
		&& Dungeon->isQuest
		&& Dungeon == poisonWaterQuest.questDungeon
		&& poisonWaterQuest.status != QS_1_ACCEPT
		&& Dungeon->genType == poisonWaterQuest.dungeonType) {
		char* palette;
		if (poisonWaterQuest.status == QS_3_COMPLETE) {
			palette = "Levels\\L3Data\\L3pwater.pal";
		}
		else {
			palette = "Levels\\L3Data\\L3pfoul_th3.pal";
		}
		LoadPalette(palette);

		/*for( int i = 0; i <= 32; ++i ){ //Duke - I dont see the difference with and without, except the loading screen no longer has distored colors
			ApplyAnimFilePalette(i); // Water is colored somewhere else
		}*/
	}

	Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
	if (IsQuestOnLevel(Q_20_SUNLESS_SEA) && islandQuest.status2 >= 1) {
		islandQuest.status3 = 1;
	}
}

//----- (004608B2) --------------------------------------------------------
void __fastcall DrawQuestText( int position, int rowIndex, int centered, char* rowText, int color )
{
	int WIDTH = 257;

	int lineYOffset = StringRowYPosition[rowIndex] + QuestPanelRect.Top;
	int defaultPos = YOffsetHashTable[lineYOffset + Screen_TopBorder + 44] + QuestPanelRect.Left + position + Screen_LeftBorder + 32;
	int surfaceOffset = defaultPos;
	size_t len = strlen(rowText);
	int offset = 0;
	if( centered ){
		int textLength = 0;
		for( size_t i = 0; i < len; ){ // (th4) CHI: step by CHARACTER (1 or 2 bytes), not byte
			int lid, w;
			i += ChiNextSmall( (const uchar*)rowText + i, &lid, &w );
			textLength += w + 1;
		}
		if( textLength < WIDTH ){
			offset = (WIDTH - textLength) / 2;
		}
		surfaceOffset += offset;
	}

	// (rt) Draw the left pentagram if the line is selected
	if( QuestPanel_SelectedRow == rowIndex ){
		int pentagramPos = position + Screen_LeftBorder + 12;
		if( centered ){
			pentagramPos += offset;
		}
		pentagramPos += QuestPanelRect.Left;
		Surface_DrawCEL(pentagramPos, lineYOffset + Screen_TopBorder + 44, PentSpn2CELPtr, QuestPanel_PentagramsPicFrame, 12);
	}

	// (rt) write all the letters of the line
	for( size_t i = 0; i < len; ){ // (th4) CHI: step by CHARACTER, not byte
		int currentChar, w;
		i += ChiNextSmall( (const uchar*)rowText + i, &currentChar, &w );
		offset += w + 1;
		if( currentChar && offset <= WIDTH ){
			DrawLetter(surfaceOffset, currentChar, color);
		}
		surfaceOffset += w + 1;
	}

	// (rt) draw the right pentagram for the selected line
	if( QuestPanel_SelectedRow == rowIndex ){
		int pentagramPos;
		if( centered ){
			pentagramPos = offset + position + Screen_LeftBorder + 36;
		}else{
			pentagramPos = Screen_LeftBorder + 276 - position;
		}
		pentagramPos += QuestPanelRect.Left;
		Surface_DrawCEL(pentagramPos, lineYOffset + Screen_TopBorder + 44, PentSpn2CELPtr, QuestPanel_PentagramsPicFrame, 12);
	}
}

// (th4) QUEST LOG SCROLLING. The log has room for QUEST_PANEL_VISIBLE entries (rows 6..26, two
// rows per entry), keeping a free entry-slot of space under the "Quest Log" title (row 2) and
// above "Close Quest Log" (row QUEST_PANEL_CLOSE_ROW_INDEX = 30). Up to that many active quests
// are centred vertically as in the original (start row = 17 - count, so rows 17-n .. 15+n). More than that used to spill over the title and the close
// line; now the list shows a window of QUEST_PANEL_VISIBLE entries starting at QuestPanel_Scroll,
// with up/down arrows at the right edge, centred (same art as the perk window, scrlarrw1..4.cel).
// QuestPanel_SelectedRow stays a SCREEN row (Start + 2*slot, or the close row); the quest behind
// a slot is QuestPanel_ActiveQuestsList[ QuestPanel_Scroll + slot ].
enum { QUEST_PANEL_VISIBLE = 11, QUEST_PANEL_FIRST_ROW = 6 };
int QuestPanel_Scroll = 0;
bool questUpArrowPressed = false, questDownArrowPressed = false;	// visual press state, cleared on mouse up (Main.cpp OnMouseLButtonUp)
static char* QuestArrowCel[4] = { 0, 0, 0, 0 };	// up, down, up pressed, down pressed

static bool QuestPanel_Scrolls() { return QuestPanel_ActiveQuestCounts > QUEST_PANEL_VISIBLE; }
static int QuestPanel_VisibleCount() { return QuestPanel_Scrolls() ? QUEST_PANEL_VISIBLE : QuestPanel_ActiveQuestCounts; }

// Arrow geometry in CURSOR coordinates (no screen borders). Right edge of the panel, centred on
// the entry area. Tune QUEST_ARROW_FROM_RIGHT to move them left/right.
enum { QUEST_ARROW_SIZE = 17, QUEST_ARROW_FROM_RIGHT = 44, QUEST_ARROW_GAP = 6 };	// 44: clear of the stone frame
static int QuestArrowX() { return QuestPanelRect.Left + QuestPanelRect.Width - QUEST_ARROW_FROM_RIGHT; }
static int QuestArrowCenterY()	// middle of the entry rows, in the same space DrawQuestText uses (+44)
{
	const int midRow = QUEST_PANEL_FIRST_ROW + QUEST_PANEL_VISIBLE - 1;	// rows 6..26 -> 16
	return QuestPanelRect.Top + StringRowYPosition[midRow] + 44 - 6;
}
static int QuestArrowUpTop()   { return QuestArrowCenterY() - QUEST_ARROW_GAP / 2 - QUEST_ARROW_SIZE; }
static int QuestArrowDownTop() { return QuestArrowCenterY() + QUEST_ARROW_GAP / 2; }
static bool CursorIn( int x, int y ){ return CursorX >= x && CursorX < x + QUEST_ARROW_SIZE && CursorY >= y && CursorY < y + QUEST_ARROW_SIZE; }

// keep the selected entry inside the visible window
static void QuestPanel_ScrollToEntry( int entry )
{
	if( !QuestPanel_Scrolls() ){ QuestPanel_Scroll = 0; return; }
	if( entry < QuestPanel_Scroll ) QuestPanel_Scroll = entry;
	if( entry >= QuestPanel_Scroll + QUEST_PANEL_VISIBLE ) QuestPanel_Scroll = entry - QUEST_PANEL_VISIBLE + 1;
	LimitToRange( QuestPanel_Scroll, 0, QuestPanel_ActiveQuestCounts - QUEST_PANEL_VISIBLE );
}
static int QuestPanel_SelectedEntry()	// -1 = the close line
{
	if( QuestPanel_SelectedRow == QUEST_PANEL_CLOSE_ROW_INDEX || !QuestPanel_ActiveQuestCounts ) return -1;
	return QuestPanel_Scroll + ( QuestPanel_SelectedRow - QuestPanel_StartRowIndex ) / 2;
}
static void QuestPanel_SelectEntry( int entry )
{
	if( entry < 0 ){ QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX; return; }
	QuestPanel_ScrollToEntry( entry );
	QuestPanel_SelectedRow = QuestPanel_StartRowIndex + 2 * ( entry - QuestPanel_Scroll );
}

// scroll the window by delta entries (arrows); the selection moves with its quest, or to the
// nearest visible entry if it scrolled out
void QuestPanel_ScrollBy( int delta )
{
	if( !QuestPanel_Scrolls() ) return;
	const int sel = QuestPanel_SelectedEntry();
	const int prev = QuestPanel_Scroll;
	QuestPanel_Scroll += delta;
	LimitToRange( QuestPanel_Scroll, 0, QuestPanel_ActiveQuestCounts - QUEST_PANEL_VISIBLE );
	if( prev == QuestPanel_Scroll ) return;
	PlayGlobalSound( S_75_I_TITLEMOV );
	if( sel >= 0 ){
		int keep = sel;
		LimitToRange( keep, QuestPanel_Scroll, QuestPanel_Scroll + QUEST_PANEL_VISIBLE - 1 );
		QuestPanel_SelectedRow = QuestPanel_StartRowIndex + 2 * ( keep - QuestPanel_Scroll );
	}
}

// Arrow clicks. Called for any click inside QuestPanelRect, before the text-area handling
// (the arrows sit outside QuestPanelTextAreaRect). Returns true if an arrow took the click.
bool QuestPanel_CheckArrowClick()
{
	if( !IsQUESTPanelVisible || !QuestPanel_Scrolls() ) return false;
	if( CursorIn( QuestArrowX(), QuestArrowUpTop() ) ){ questUpArrowPressed = true; QuestPanel_ScrollBy( -1 ); return true; }
	if( CursorIn( QuestArrowX(), QuestArrowDownTop() ) ){ questDownArrowPressed = true; QuestPanel_ScrollBy( 1 ); return true; }
	return false;
}

//----- (004609FF) --------------------------------------------------------
void QuestPanel_Draw()
{
	Surface_DrawCEL(Screen_LeftBorder + QuestPanelRect.Left, Screen_TopBorder + QuestPanelRect.Down, Data_QuestCEL, 1, QuestPanelRect.Width);
	
	DrawQuestText(0, 2, true, (char*)L("Quest Log"), C_3_Gold);
 
	const int visible = QuestPanel_VisibleCount();
	for( int slot = 0; slot < visible; ++slot ){
		int rowIndex = QuestPanel_StartRowIndex + 2 * slot;
		int questIndex = QuestPanel_ActiveQuestsList[QuestPanel_Scroll + slot];
		DrawQuestText(0, rowIndex, true, LocalizedQuestName( questIndex, BaseQuests[questIndex].questName ), C_0_White);
	}
	DrawQuestText(0, QUEST_PANEL_CLOSE_ROW_INDEX, true, (char*)L("Close Quest Log"), C_0_White);
	if( QuestPanel_Scrolls() ){
		if( !QuestArrowCel[0] ){
			QuestArrowCel[0] = (char*)LoadFile("Data\\scrlarrw1.cel");
			QuestArrowCel[1] = (char*)LoadFile("Data\\scrlarrw2.cel");
			QuestArrowCel[2] = (char*)LoadFile("Data\\scrlarrw3.cel");
			QuestArrowCel[3] = (char*)LoadFile("Data\\scrlarrw4.cel");
		}
		const int x = Screen_LeftBorder + QuestArrowX();
		Surface_DrawCEL( x, Screen_TopBorder + QuestArrowUpTop() + QUEST_ARROW_SIZE,   QuestArrowCel[ questUpArrowPressed ? 2 : 0 ], 1, QUEST_ARROW_SIZE );
		Surface_DrawCEL( x, Screen_TopBorder + QuestArrowDownTop() + QUEST_ARROW_SIZE, QuestArrowCel[ questDownArrowPressed ? 3 : 1 ], 1, QUEST_ARROW_SIZE );
	}
	if( IsMainDraw ) QuestPanel_PentagramsPicFrame = (QuestPanel_PentagramsPicFrame & 7) + 1;
}

//----- (00460A90) --------------------------------------------------------
void QuestPanel_Open()
{
	QuestPanel_ActiveQuestCounts = 0;
	int questPanel_ActiveQuestCounts = 0;
	for( int questIndex = 0; questIndex < Quest_Count; ++questIndex ){
		const Quest& quest = Quests[questIndex];
		if( quest.status == QS_2_IN_PROGRESS && quest.talkToFlag ){
			QuestPanel_ActiveQuestsList[questPanel_ActiveQuestCounts++] = questIndex;
		}
	}

	QuestPanel_ActiveQuestCounts = questPanel_ActiveQuestCounts;
	// up to QUEST_PANEL_VISIBLE entries: centred as in the original (rows 17-n .. 15+n);
	// more: a scrolling window starting on the first entry row
	QuestPanel_StartRowIndex = QuestPanel_Scrolls() ? QUEST_PANEL_FIRST_ROW : 17 - QuestPanel_ActiveQuestCounts;
	QuestPanel_Scroll = 0;
	questUpArrowPressed = questDownArrowPressed = false;
	QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
	if( QuestPanel_ActiveQuestCounts ){
		QuestPanel_SelectedRow = QuestPanel_StartRowIndex;
	}
	IsQUESTPanelVisible = true;
	QuestPanel_PentagramsPicFrame = 1;
}

//----- (00460B01) --------------------------------------------------------
void QuestPanel_ShiftUp()
{
	if( QuestPanel_ActiveQuestCounts ){
		const int sel = QuestPanel_SelectedEntry();
		if( sel == 0 ) QuestPanel_SelectEntry( -1 );								// first -> close line
		else if( sel < 0 ) QuestPanel_SelectEntry( QuestPanel_ActiveQuestCounts - 1 );	// close -> last
		else QuestPanel_SelectEntry( sel - 1 );
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//----- (00460B48) --------------------------------------------------------
void QuestPanel_ShiftDown()
{
	if( QuestPanel_ActiveQuestCounts ){
		const int sel = QuestPanel_SelectedEntry();
		if( sel < 0 ) QuestPanel_SelectEntry( 0 );												// close -> first
		else if( sel == QuestPanel_ActiveQuestCounts - 1 ) QuestPanel_SelectEntry( -1 );		// last -> close line
		else QuestPanel_SelectEntry( sel + 1 );
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//----- (00460B92) --------------------------------------------------------
void QuestPanel_StartSpeech()
{
	PlayGlobalSound(S_76_I_TITLSLCT);
	const int sel = QuestPanel_SelectedEntry();
	if( sel >= 0 && sel < QuestPanel_ActiveQuestCounts ){
		int questIndex = QuestPanel_ActiveQuestsList[sel];
		StartSpeech(Quests[questIndex].speechIndex);
	}
	IsQUESTPanelVisible = false;
}

//----- (00460BD2) --------------------------------------------------------
void QuestPanel_CheckLButtonClick()
{
	int row = (CursorY - QuestPanelTextAreaRect.Top) / 12;
	const int visible = QuestPanel_VisibleCount();
	for( int slot = 0; slot < visible; ++slot ){
		if( row == QuestPanel_StartRowIndex + 2 * slot ){
			QuestPanel_SelectedRow = row;
			QuestPanel_StartSpeech();
			break;
		}
	}
	if( row == QUEST_PANEL_CLOSE_ROW_INDEX ){
		QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		QuestPanel_StartSpeech();
	}
}

//----- (00460C27) --------------------------------------------------------
void __fastcall UpdateQuestStatus( int questIndex, QUEST_STATUS status, int talkToFlag, int status2, int status3, int status4, int status5 )
{
	Quest& quest = Quests[questIndex];
	if( quest.status != QS_3_COMPLETE ){
		if( status > quest.status ){// (rt) works for increases only
			quest.status = status;
		}
		quest.talkToFlag |= talkToFlag;// (rt) can only change from 0 to 1, not the other way round
		if( status2 > quest.status2 ){// (rt) also for increases
			quest.status2 = status2;
		}
		if (status3 > quest.status3) {
			quest.status3 = status3;
		}
		if (status4 > quest.status4) {
			quest.status4 = status4;
		}
		if (status5 > quest.status5) {
			quest.status5 = status5;
		}
	}
}
