#include "stdafx.h"

int QuestPanel_StartRowIndex;// 006EB348
bool IsQUESTPanelVisible;// 006EB34C
char* Data_QuestCEL;// 006EB350
char* Data_ExclamCEL;
Quest Quests[Quest_Count];// 006EB358
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
	{QuestGroup2, 4, 2, 0}, // активируем рандомизацию Fleshdoom
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

// отключение квеста по таблице RandomizeQuestsTable (fleshdoom в таблице отключен, поэтому он здесь не отключается)
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
	// тут можно форсировать заданный квест
	//if( questIndex != Q_22_MAIDEN_OF_ANGUISH ) // например andariel
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
	IsQUESTPanelVisible = false;
	QuestPanel_PentagramsPicFrame = 1;
	HorazonGrimspikeDelay = 0;
	FleshDoomDelay = 0;
	TheDenOneSecondTimer = 0;
	TheDenMonsterSpawnTimer = 0;
	Lava_Pits_OneSecondTimer = 0;
	Lava_Pits_MonsterSpawnTimer = 0;
	WaterPaletteInPoizonRiverQuestCave = 0;

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
	case 2:// только что активировали книгу
		OpenGrimspikeRoom (1);// открытие дверей
		horazonQuest.status3 = 3; // дверь открыта
		HorazonGrimspikeDelay = 40; //задержка перед звуком игрока
		break;
	case 3:// открыли двери, речи игрока
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
		horazonQuest.status3 = 4; // речь произнесена
		HorazonGrimspikeDelay = 100; //задержка перед вызовом гримспайка
		break;
	case 4:// изменение координат гримспайка
		{
			int row = 39;
			int col = 34;
			MoveGrimspike (row, col);
			PlayLocalSound(S_141_STORM, row, col);
			CastMissile( 0, 0, row, col, 0, MI_62_RISING_2, CT_0_PLAYER, 0, 0, 0, 0 );
			horazonQuest.status3 = 5; // гримспайк появился
			break;
		}
	}
}

//----- (0045F7BC) --------------------------------------------------------
void QuestsActions()
{
	Player& player = Players[CurrentPlayerIndex];

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
		if( Dungeon == poisonedWaterQuest.questDungeon
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
	if( is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN) || SaveAlwaysEnabled
		|| (MaxCountOfPlayersInGame != 1 && not(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) ){ // Easy, Speedrun, Multi, Cheat -> always can
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
		if( quest.dungeonStart == Dungeon && quest.questDungeon != DUN_NO && quest.status != QS_0_NO ){
			int row = quest.row;
			int col = quest.col;
			for( int deltaRangeIndex = 0; deltaRangeIndex < 7; ++deltaRangeIndex ){
				if( row + MapDeltaRangeRow[deltaRangeIndex] == Cur.Row 
				 && col + MapDeltaRangeCol[deltaRangeIndex] == Cur.Col ){
					sprintf(InfoPanelHeader, "To %s", Dungeons[Quests[quest.baseId].questDungeon].warpName);
					if( Dungeons[Quests[quest.baseId].questDungeon].warpName2 ){
						InfoPanel_AddLine(Dungeons[Quests[quest.baseId].questDungeon].warpName2, true);
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
		return;
	}
	if (bossIndex == UM_790_Ghost_of_Brother_Martin) {
		Quests[Q_34_GRAVE_MATTERS].status = QS_3_COMPLETE;
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
	if( !strcmp(monster.Name, UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_849_GHARBAD : UM_0_Gharbad_the_Weak].NamePtr) ){
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
	if( !strcmp(monster.Name, UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_870_ZHAR_THE_MAD : UM_2_Zhar_the_Mad].NamePtr) ){
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
	if( !strcmp(monster.Name, UniqueMonsters[UM_4_ArchBishop_Lazarus].NamePtr) && MaxCountOfPlayersInGame != 1 ){
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
	if( !strcmp(monster.Name, UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_867_LAZARUS : UM_4_ArchBishop_Lazarus].NamePtr) && MaxCountOfPlayersInGame == 1 ){
		Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
		lazarusQuest.status = QS_3_COMPLETE;
		PlayerSoundMessageDelay = 30;
		if (Dungeon != DUN_101_VOID_CHAMBERS) {
			AddWarpFromUnholyAltar();
			lazarusQuest.status2 = 7;
			lazarusQuest.status3 = 4;
			Quests[Q_5_DIABLO].status = QS_2_IN_PROGRESS;
			int row = 35;
			int col = 32;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0);
		}
		switch( player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_813_WAR_83 ;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:		SoundMessage = S_704_ROG_83;			break;
			case PC_2_MAGE:			SoundMessage = S_601_SOR_83;		break;
			case PC_3_MONK:			SoundMessage = S_921_MONK_83;			break;
		}
		return;
	}
	if( !strcmp(monster.Name, UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_862_WARLORD : UM_8_Warlord_of_Blood].NamePtr) ){
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
	if (!strcmp(monster.Name, UniqueMonsters[UM_595_The_Cerberus].NamePtr)) {
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
	ushort* dunFilePtr = (ushort*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blood2_cls.DUN" : "Levels\\L2Data\\Blood2.DUN");
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
		Quest& quest = Quests[questIndex];
		quest.row = Convert40to112(row) + 9;
		quest.col = Convert40to112(col) + 2;
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
			case Q_9_VALOR:						ApplyValorDun                      (qi, startRow, startCol); break;
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
	if( Dungeon->isQuest 
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
				if( blackMushroomQuest.status2 >= 7 ){// TODO: никогда не выполняется
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
	if (Dungeon->isQuest
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
		for( size_t i = 0; i < len; ++i ){
			textLength += FontWidthSmall[FontIndexSmall[Codepage[rowText[i]]]] + 1;
		}
		if( textLength < WIDTH ){
			offset = (WIDTH - textLength) / 2;
		}
		surfaceOffset += offset;
	}

	// Рисуем пентаграмму левую если строка выделена
	if( QuestPanel_SelectedRow == rowIndex ){
		int pentagramPos = position + Screen_LeftBorder + 12;
		if( centered ){
			pentagramPos += offset;
		}
		pentagramPos += QuestPanelRect.Left;
		Surface_DrawCEL(pentagramPos, lineYOffset + Screen_TopBorder + 44, PentSpn2CELPtr, QuestPanel_PentagramsPicFrame, 12);
	}

	// пишем все буквы строки
	for( size_t i = 0; i < len; ++i ){
		uchar currentChar = FontIndexSmall[Codepage[rowText[i]]];
		offset += FontWidthSmall[currentChar] + 1;
		if( currentChar && offset <= WIDTH ){
			DrawLetter(surfaceOffset, currentChar, color);
		}
		surfaceOffset += FontWidthSmall[currentChar] + 1;
	}

	// рисуем правую пентаграмму у выделенной строки
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

//----- (004609FF) --------------------------------------------------------
void QuestPanel_Draw()
{
	Surface_DrawCEL(Screen_LeftBorder + QuestPanelRect.Left, Screen_TopBorder + QuestPanelRect.Down, Data_QuestCEL, 1, QuestPanelRect.Width);
	
	DrawQuestText(0, 2, true, "Quest Log", C_3_Gold);
 
	for( int activeQuestIndex = 0; activeQuestIndex < QuestPanel_ActiveQuestCounts; ++activeQuestIndex ){
		int rowIndex = QuestPanel_StartRowIndex + 2 * activeQuestIndex;
		int questIndex = QuestPanel_ActiveQuestsList[activeQuestIndex];
		DrawQuestText(0, rowIndex, true, BaseQuests[questIndex].questName, C_0_White);
	}
	DrawQuestText(0, QUEST_PANEL_CLOSE_ROW_INDEX, true, "Close Quest Log", C_0_White);
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
	int startRowIndex;
	if( QuestPanel_ActiveQuestCounts <= 10 ){
		startRowIndex = 17 - (QuestPanel_ActiveQuestCounts);
	}else{
		startRowIndex = 17 - (QuestPanel_ActiveQuestCounts);
	}
	QuestPanel_StartRowIndex = startRowIndex;
	QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
	if( QuestPanel_ActiveQuestCounts ){
		QuestPanel_SelectedRow = startRowIndex;
	}
	IsQUESTPanelVisible = true;
	QuestPanel_PentagramsPicFrame = 1;
}

//----- (00460B01) --------------------------------------------------------
void QuestPanel_ShiftUp()
{
	if( QuestPanel_ActiveQuestCounts ){
		if( QuestPanel_SelectedRow == QuestPanel_StartRowIndex ){
			QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		}else if( QuestPanel_SelectedRow == QUEST_PANEL_CLOSE_ROW_INDEX ){
			QuestPanel_SelectedRow = QuestPanel_StartRowIndex + 2 * (QuestPanel_ActiveQuestCounts - 1);
		}else{
			QuestPanel_SelectedRow -= 2;
		}
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//----- (00460B48) --------------------------------------------------------
void QuestPanel_ShiftDown()
{
	if( QuestPanel_ActiveQuestCounts ){
		if( QuestPanel_SelectedRow == QUEST_PANEL_CLOSE_ROW_INDEX ){
			QuestPanel_SelectedRow = QuestPanel_StartRowIndex;
		}else if( QuestPanel_SelectedRow == QuestPanel_StartRowIndex + 2 * (QuestPanel_ActiveQuestCounts - 1) ){
			QuestPanel_SelectedRow = QUEST_PANEL_CLOSE_ROW_INDEX;
		}else{
			QuestPanel_SelectedRow += 2;
		}
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

//----- (00460B92) --------------------------------------------------------
void QuestPanel_StartSpeech()
{
	PlayGlobalSound(S_76_I_TITLSLCT);
	if( QuestPanel_ActiveQuestCounts && QuestPanel_SelectedRow != QUEST_PANEL_CLOSE_ROW_INDEX ){
		int questIndex = QuestPanel_ActiveQuestsList[(QuestPanel_SelectedRow - QuestPanel_StartRowIndex) / 2];
		StartSpeech(Quests[questIndex].speechIndex);
	}
	IsQUESTPanelVisible = false;
}

//----- (00460BD2) --------------------------------------------------------
void QuestPanel_CheckLButtonClick()
{
	int row = (CursorY - QuestPanelTextAreaRect.Top) / 12;
	for( int activeQuestIndex = 0; activeQuestIndex < QuestPanel_ActiveQuestCounts; ++activeQuestIndex ){
		if( row == QuestPanel_StartRowIndex + 2 * activeQuestIndex ){
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
		if( status > quest.status ){// только на увеличение работает
			quest.status = status;
		}
		quest.talkToFlag |= talkToFlag;// может только стать из 0 в 1 но не наоборот
		if( status2 > quest.status2 ){// тоже на увеличение
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
