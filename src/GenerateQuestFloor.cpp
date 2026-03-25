#include "stdafx.h"

MapArea MapAreas[2] = {// 004B7280
	{19, 47, 26, 55},
	{26, 49, 30, 53},
};
MapArea MapAreas6[2] = {// 004B7288
	{33, 19, 47, 29},
	{37, 29, 43, 39},
};
MapArea MapAreas2[5] = {// 004B7290
	{27, 53, 35, 61},
	{27, 35, 34, 42},
	{45, 35, 53, 43},
	{45, 53, 53, 61},
	{31, 39, 49, 57},
};
MapArea MapAreas4[7] = {// 004B72A8
	{ 49, 45, 58, 51 },
	{ 57, 31, 62, 37 },
	{ 63, 31, 69, 40 },
	{ 59, 41, 73, 55 },
	{ 63, 55, 69, 65 },
	{ 73, 45, 78, 51 },
	{ 79, 43, 89, 53 }
};
MapArea MapAreas7[5] = {// 004B72C8
	{43, 19, 50, 26},
	{51, 19, 59, 26},
	{35, 27, 42, 34},
	{43, 27, 49, 34},
	{50, 27, 59, 34},
};
MapArea MapAreas3[2] = {// 004B72E0
	{19, 31, 34, 47},
	{34, 35, 42, 42}
};
MapArea MapAreas5[9] = {// 004B72E8
	{43, 35, 50, 42},
	{51, 35, 62, 42},
	{63, 31, 66, 46},
	{67, 31, 78, 34},
	{67, 35, 78, 42},
	{67, 43, 78, 46},
	{35, 43, 42, 51},
	{43, 43, 49, 51},
	{50, 43, 59, 51}
};

int HorazonQuestBookSpeechTable [16][6] = {// 00A35510
	SP_323, SP_332, SP_329, SP_326, SP_335, SP_323,
	SP_324, SP_333, SP_330, SP_327, SP_336, SP_324,
	SP_325, SP_334, SP_331, SP_328, SP_337, SP_325,
	SP_339, SP_340, SP_341, SP_338, SP_340, SP_339,
	SP_343, SP_344, SP_345, SP_342, SP_344, SP_343,
	SP_347, SP_348, SP_349, SP_346, SP_348, SP_347,
	SP_351, SP_352, SP_353, SP_350, SP_352, SP_351,
	SP_355, SP_356, SP_357, SP_354, SP_356, SP_355,
	SP_351, SP_352, SP_353, SP_350, SP_352, SP_351,
	SP_317, SP_317, SP_317, SP_317, SP_317, SP_317,
	SP_318, SP_318, SP_318, SP_318, SP_318, SP_318,
	SP_319, SP_319, SP_319, SP_319, SP_319, SP_319,
	SP_320, SP_320, SP_320, SP_320, SP_320, SP_320,
	SP_321, SP_321, SP_321, SP_321, SP_321, SP_321,
	SP_322, SP_322, SP_322, SP_322, SP_322, SP_322,
	SP_359, SP_360, SP_361, SP_358, SP_360, SP_359,
};

char HorazonBooksCombinationTable [6][3] = {// 00A35CB5
	{1, 2, 3},
	{1, 3, 2},
	{2, 1, 3},
	{2, 3, 1},
	{3, 1, 2},
	{3, 2, 1},
};

//----- (0046B092) --------------------------------------------------------
int __fastcall ObjIndex( int row, int col )
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; ++objectIndexIndex ){
		int objectIndex = ObjectActive[objectIndexIndex];
		Object& object = Objects[objectIndex];
		if( object.Row == row && object.Col == col ){
			return objectIndex;
		}
	}
	TerminateWithError("ObjIndex: Active object not found at (%d,%d)", row, col);
}

//----- (0046B0DD) --------------------------------------------------------
void SetupLeoricTombLevers()
{
	SetChangeRect(	ObjIndex(64, 34),	20, 7, 23, 10, 1);
	SetChangeRect(	ObjIndex(64, 59),	20, 14, 21, 16, 2);
	SetChangeRect(	ObjIndex(27, 37),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(46, 35),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(49, 53),	8, 1, 15, 11, 3);
	SetChangeRect(	ObjIndex(27, 53),	8, 1, 15, 11, 3);
}

//----- (0046B18C) --------------------------------------------------------
void SetupChamberOfBoneLevers()
{
	SetChangeRect(	ObjIndex(37, 30),	17, 0, 21, 5, 1);
	SetChangeRect(	ObjIndex(37, 46),	13, 0, 16, 5, 2);
}

//----- (0046B1C7) --------------------------------------------------------
void SetupUnholyAltarLevers()
{
	SetChangeRect(	ObjIndex(26, 45),	1, 1, 9, 10, 1);
	SetChangeRect(	ObjIndex(45, 46),	11, 1, 20, 10, 2);
	SetChangeRect(	ObjIndex(35, 36),	7, 11, 13, 18, 3);
}

//----- (00717080) --------------------------------------------------------
void __fastcall InitHorazonQuestBook( int objectIndex, int bookNumber )
{
	Player& player = Players[CurrentPlayerIndex];
	Object& object = Objects[objectIndex];

	object.ShrineIndex = 1;
	object.FrameIndex = 5 - 2 * object.ShrineIndex;// 3
	object.OpenState = 5 - 2 * object.ShrineIndex + 1;// 4
	object.bookIndex = 0;

	object.MaybeSpeechIndex = HorazonQuestBookSpeechTable[bookNumber - 1][player.ClassID];

	if( bookNumber > 3 ){
		object.BookNameIndex = bookNumber + 7;
	}else{
		object.bookIndex = bookNumber;
		object.BookNameIndex = 10;
	}
}

//----- (00717100) --------------------------------------------------------
void SetupHorazon1Books()
{
	InitHorazonQuestBook (ObjIndex(76, 68), 4);
	InitHorazonQuestBook (ObjIndex(37, 73), 5);
	InitHorazonQuestBook (ObjIndex(25, 73), 6);
}

//----- (007171C0) --------------------------------------------------------
void SetupHorazon2Books()
{
	InitHorazonQuestBook (ObjIndex(66, 86), 10);
	InitHorazonQuestBook (ObjIndex(19, 68), 12);
	InitHorazonQuestBook (ObjIndex(66, 58), 13);
	InitHorazonQuestBook (ObjIndex(52, 51), 14);
	InitHorazonQuestBook (ObjIndex(85, 40), 11);
	InitHorazonQuestBook (ObjIndex(60, 37), 15);
	int combination = RNG(6);
	InitHorazonQuestBook (ObjIndex(43, 19), HorazonBooksCombinationTable[combination][0]);
	InitHorazonQuestBook (ObjIndex(43, 20), HorazonBooksCombinationTable[combination][1]);
	InitHorazonQuestBook (ObjIndex(43, 21), HorazonBooksCombinationTable[combination][2]);
}

//----- (00717160) --------------------------------------------------------
void SetupHorazon3Books()
{
	InitHorazonQuestBook (ObjIndex(48, 38), 7);
	InitHorazonQuestBook (ObjIndex(21, 34), 8);
	InitHorazonQuestBook (ObjIndex(48, 28), 9);
}

//----- (00717810) --------------------------------------------------------
void __fastcall OpenCryptQuestDoor (int row, int col)
{
	FineMap[ row ][ col - 1 ] = 301;
	FineMap[ row ][ col ] = 298;
	FineMap[ row ][ col + 1 ] = 299;
	InitDungeonTiles();
}

//----- (00717B70) --------------------------------------------------------
void __fastcall OpenNakrulRoom (int needPlaySound)
{
	OpenCryptQuestDoor (40, 24);
	if( needPlaySound ){
		PlayLocalSound(S_1020_I_CROPEN, 40, 24);
	}
}

//----- (00717840) --------------------------------------------------------
void __fastcall OpenGrimspikeRoom(int needPlaySound)
{
	OpenCryptQuestDoor (44, 28);
	OpenCryptQuestDoor (44, 38);
	if( needPlaySound ){
		PlayLocalSound(S_1020_I_CROPEN, 44, 28);
		PlayLocalSound(S_1020_I_CROPEN, 44, 38);
	}
}

//----- (0046B21F) --------------------------------------------------------
void __fastcall LoadQuestHallMap( char* dunFileName )
{
	ushort* dunData = (ushort*)LoadFile(dunFileName);
	ushort rowSize = dunData[0];
	ushort colSize = dunData[1];
	ushort doubleRowSize = rowSize * 2;
	ushort doubleColSize = colSize * 2;
	int mapSize = rowSize * colSize;
	ushort* dunArray = &dunData[2 + 13 * mapSize];

	for( int col = 0; col < doubleColSize; col++ ){
		for( int row = 0; row < doubleRowSize; row++ ){
			ushort roomIndex = dunArray[row + col * doubleRowSize];
			TransMap[ row + 16 ][ col + 16 ] = (uchar)roomIndex;
		}
	}

	FreeMem(dunData);
}

//!---- (0046B400) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_25_KingLeoricsTomb()
{
	Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];
	if( kingLeoricQuest.status == QS_1_ACCEPT ){
		kingLeoricQuest.status = QS_2_IN_PROGRESS;
		kingLeoricQuest.status2 = 1;
	}
	LoadQuestDungeonAltMapChurch("Levels\\L1Data\\SklKng1.DUN", 83, 45);
	char* dunName; 
	switch (RNG(8)) {
	case 1:  dunName = "Levels\\L1Data\\SklKng2.DUN";	break;
	case 2:  dunName = "Levels\\L1Data\\SklKng3.DUN";	break;
	case 3:  dunName = "Levels\\L1Data\\SklKng4.DUN";	break;
	case 4:  dunName = "Levels\\L1Data\\SklKng5.DUN";	break;
	case 5:  dunName = "Levels\\L1Data\\SklKng7.DUN";	break;
	case 6:  dunName = "Levels\\L1Data\\SklKng8.DUN";	break;
	case 7:  dunName = "Levels\\L1Data\\SklKng9.DUN";	break;
	default: dunName = "Levels\\L1Data\\SklKng6.DUN";	break;
	}
	if (GameMode == GM_CLASSIC) { dunName = "Levels\\L1Data\\SklKng_base.DUN"; }
	LoadQuestDungeonMainMapChurch(dunName, 83, 45);
	if (GameMode == GM_CLASSIC) {
		LoadPalette("Levels\\L1Data\\L1_2.pal");
	}
	else {
		LoadPalette("Levels\\L1Data\\Leo_v02.pal");
	}
	FillAreasSomeOnMap112x112u(2, MapAreas);
	FillAreasDiffOnMap112x112u(2, MapAreas6);
	FillAreasSomeOnMap112x112u(5, MapAreas2);
	FillAreasDiffOnMap112x112u(7, MapAreas4);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	SetupLeoricTombLevers();
	AddWarpFromSceletonKing();
}

void GenerateQuestFloor_109_Hall_of_Heroes()
{
	LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Bonecha2.DUN");
	char* dunName;	
	dunName = GameMode == GM_CLASSIC ? "Levels\\L2Data\\HoH_v01_cls.DUN" : "Levels\\L2Data\\HoH_v01.DUN";
	int x = 69, y = 39;
	if (Dungeon.prev == DUN_26_BONE_CHAMBER) {
		x = 21; y = 41;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	char* palletteName;
	switch (Difficulty) {
	case DL_0_HORROR:		palletteName = "Levels\\L2Data\\FUCKb.pal";		break;
	case DL_1_PURGATORY:	palletteName = "Levels\\L2Data\\Chamber4.pal";	break;
	default:				palletteName = "Levels\\L2Data\\Chamber5.pal";	break;
	}
	if (GameMode == GM_CLASSIC) { palletteName = "Levels\\L2Data\\l2_2.pal"; }
	LoadPalette(palletteName);
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(70, 39, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_6_CATACOMB_2);
	AddWarp(22, 41, WM_1029_GOTO_QUEST_DUNGEON, DUN_26_BONE_CHAMBER);
}

//!---- (0046B398) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_26_BoneChamber()
{
	Quest& COBQuest = Quests[Q_14_THE_CHAMBER_OF_BONE];
	if (COBQuest.status == QS_1_ACCEPT) {
		COBQuest.status = QS_2_IN_PROGRESS;
		COBQuest.talkToFlag = true;
	}

	LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Bonecha2.DUN");
	char* dunName;

	if (MaxCountOfPlayersInGame > 1) {// mp
		if (Difficulty < DL_2_DOOM) {
			dunName = "Levels\\L2Data\\bonecha_MP_01.DUN";
		}
		else{
			dunName = "Levels\\L2Data\\bonecha_MP_03plus.DUN";
		}
	}
	else { // sp
		switch (Difficulty) {
		case DL_0_HORROR:
			switch (RNG(7)) {
			case  1: dunName = "Levels\\L2Data\\Bonechahoror2.DUN";	break;
			case  2: dunName = "Levels\\L2Data\\Bonechahoror3.DUN";	break;
			case  3: dunName = "Levels\\L2Data\\Bonechahoror4.DUN";	break;
			case  4: dunName = "Levels\\L2Data\\Bonechahoror5.DUN";	break;
			case  5: dunName = "Levels\\L2Data\\Bonechahoror7.DUN";	break;
			case  6: dunName = "Levels\\L2Data\\Bonecha1.DUN";		break;
			default: dunName = "Levels\\L2Data\\Bonechahoror6.DUN";	break;
			} 
			break;

		case DL_1_PURGATORY:	dunName = "Levels\\L2Data\\Bonechax.DUN";	break;
		case DL_2_DOOM:			dunName = "Levels\\L2Data\\Bonechaz.DUN";	break;
		case DL_3_CRUCIBLE:
		case DL_4_TORMENT:
		case DL_5_AGONY:
		case DL_6_TERROR:
		case DL_7_DESTRUCTION:
		case DL_8_INFERNO:	
			switch (RNG(3)) {
			case  1: dunName = "Levels\\L2Data\\Bonechaz2.DUN";	break;
			case  2: dunName = "Levels\\L2Data\\Bonechaz3.DUN";	break;
			default: dunName = "Levels\\L2Data\\Bonechaz.DUN";	break;
			}
			break;

		}
	}
	if (GameMode == GM_CLASSIC) { dunName = "Levels\\L2Data\\bonecha1_orig.DUN"; }
	LoadQuestDungeonMapMainCatacomb(dunName, 69, 39);
	char* palletteName;
	switch (Difficulty ){
	case DL_0_HORROR:		palletteName = "Levels\\L2Data\\FUCKb.pal";		break;
	case DL_1_PURGATORY:	palletteName = "Levels\\L2Data\\Chamber4.pal";	break;
	case DL_2_DOOM:
	case DL_3_CRUCIBLE:
	case DL_4_TORMENT:
	case DL_5_AGONY:
	case DL_6_TERROR:
	case DL_7_DESTRUCTION:
	case DL_8_INFERNO:			palletteName = "Levels\\L2Data\\Chamber5.pal";	break;
	}
	if (GameMode == GM_CLASSIC) { palletteName = "Levels\\L2Data\\l2_2.pal"; }
	LoadPalette(palletteName);
	if (MaxCountOfPlayersInGame == 1) {
		FillAreasDiffOnMap112x112u(5, MapAreas7);
		FillAreasSomeOnMap112x112u(2, MapAreas3);
		FillAreasDiffOnMap112x112u(9, MapAreas5);
		SetupChamberOfBoneLevers();
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(70, 39, WM_1029_GOTO_QUEST_DUNGEON, DUN_109_HALL_OF_HEROES);	
}

void GenerateQuestFloor_53_VaultKeep()
{
	Quest& good_Deal_Quest = Quests[Q_31_GOOD_DEAL];
	if (good_Deal_Quest.status == QS_1_ACCEPT) {
		good_Deal_Quest.status = QS_2_IN_PROGRESS;
		good_Deal_Quest.talkToFlag = true;
	}

	LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Vaultkeep_01.DUN");
	char* dunName = "Levels\\L2Data\\Vaultkeep_01.DUN";
	LoadQuestDungeonMapMainCatacomb(dunName, 23, 33);
	char* palletteName = "Levels\\L2Data\\Chamber5.pal";//adjust
	LoadPalette(palletteName);
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(22, 33, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_8_CATACOMB_4);
}

void GenerateQuestFloor_58_Gateway_of_Blood()
{
	Quest& valor_Quest = Quests[Q_9_VALOR];
	if (valor_Quest.status == QS_1_ACCEPT) {
		valor_Quest.status = QS_2_IN_PROGRESS;
		valor_Quest.talkToFlag = true;
	}

	//LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Vaultkeep_01.DUN");
	char* dunName = "Levels\\L2Data\\gob_v01.DUN";
	LoadQuestDungeonMapMainCatacomb(dunName, 23, 33);
	char* palletteName = "Levels\\L2Data\\gob_v1.pal";//adjust
	LoadPalette(palletteName);
	//AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(48, 79, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_5_CATACOMB_1);
	AddWarp(55, 45, WM_1029_GOTO_QUEST_DUNGEON, DUN_59_HALL_OF_FIRE);

	if (Dungeon.prev == DUN_59_HALL_OF_FIRE) {
		PlayerRowPos = 56;
		PlayerColPos = 46;
	}
	else {
		PlayerRowPos = 49;
		PlayerColPos = 80;
	}

}

void GenerateQuestFloor_59_Hall_of_Fire()
{
	/*Quest& valor_Quest = Quests[Q_9_VALOR];
	if (valor_Quest.status == QS_1_ACCEPT) {
		valor_Quest.status = QS_2_IN_PROGRESS;
		valor_Quest.talkToFlag = true;
	}*/

	//LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\Vaultkeep_01.DUN");
	char* dunName = "Levels\\L2Data\\hof_v01.DUN";
	LoadQuestDungeonMapMainCatacomb(dunName, 23, 33);
	char* palletteName = "Levels\\L2Data\\hof_v1.pal";//adjust
	LoadPalette(palletteName);
	//AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(48, 79, WM_1029_GOTO_QUEST_DUNGEON, DUN_58_GATEWAY_OF_BLOOD);
	PlayerRowPos = 49;
	PlayerColPos = 80;
}

//!---- (0046B358) --------------------------------------------------------
void GenerateQuestFloor_27_TheInfestedCellar()
{
	Player& player = Players[CurrentPlayerIndex];
	
	Quest& lairQuest = Quests[Q_23_INFESTED_CELLAR];
	if( lairQuest.status == QS_1_ACCEPT ){
		lairQuest.status = QS_2_IN_PROGRESS;
		lairQuest.talkToFlag = 1;//If player enters Infested Cellar before talking to Pepin, make sure quest log still shows Cellar quest.
	}	

	char* dunName;
	switch (RNG(3)) {
	case 1:	 dunName = "Levels\\L3Data\\lair02x1.DUN";	break;
	case 2:	 dunName = "Levels\\L3Data\\lair02x2.DUN";	break;
	default: dunName = "Levels\\L3Data\\lair02.DUN";	break;
	}
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l3data\\worms01.pal");
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)){
		AddWarp(43, 64, WM_1029_GOTO_QUEST_DUNGEON, DUN_51_FETID_CAVERN);
	}else{
		AddWarp(43, 64, WM_1028_RETURN_FROM_QUEST_DUNGEON);
	}

	PlayerRowPos = 43;
	PlayerColPos = 65;
	if (lairQuest.status == QS_3_COMPLETE) {
		Open_the_Gates_Cellar();
	}
}

void GenerateQuestFloor_49_Sick_Villagers_Basement()
{
	Player& player = Players[CurrentPlayerIndex];

	Quest& demon_In_The_Town_Quest = Quests[Q_27_DEMON_IN_THE_TOWN];
	if (demon_In_The_Town_Quest.status == QS_1_ACCEPT) {
		demon_In_The_Town_Quest.status = QS_2_IN_PROGRESS;
		demon_In_The_Town_Quest.talkToFlag = 1;//If player enters basement before talking to Pepin, make sure quest log still shows DITT quest.
	}

	char* dunName;
	dunName = "Levels\\L3Data\\svb_01.dun";
	/*switch (RNG(3)) {
	case 1:	 dunName = "Levels\\L3Data\\lair02x1.DUN";	break;
	case 2:	 dunName = "Levels\\L3Data\\lair02x2.DUN";	break;
	default: dunName = "Levels\\L3Data\\lair02.DUN";	break;
	}*/

	LoadQuestDungeonAltMapCave(dunName, 20, 50);
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	int palNum = RNG(50) + 1; 
	char palName[100];
	sprintf(palName, "Levels\\L3Data\\l3_%i.pal", palNum);
	LoadPalette(palName);
	AddWarp(43, 64, WM_1028_RETURN_FROM_QUEST_DUNGEON);

	PlayerRowPos = 43;
	PlayerColPos = 65;
}

//!---- (0046B31A) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_28_PoisonedWaterSupply()
{
	Quest& poisonedWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	if( poisonedWaterQuest.status == QS_1_ACCEPT ){
		poisonedWaterQuest.status = QS_2_IN_PROGRESS;
		poisonedWaterQuest.talkToFlag = 1;//If player enters PWS before talking to Pepin, make sure quest log still shows PWS quest.
	}
	char* dunName;
	if (MaxCountOfPlayersInGame == 1) {
		switch (Difficulty) {
		case DL_0_HORROR:
			switch (RNG(10)) {
			case  1: dunName = "Levels\\L3Data\\Foulwatr.DUN";		break;
			case  2: dunName = "Levels\\L3Data\\Foulwatr02.DUN";	break;
			case  3: dunName = "Levels\\L3Data\\Foulwatr03.DUN";	break;
			case  4: dunName = "Levels\\L3Data\\Foulwatr04.DUN";	break;
			case  5: dunName = "Levels\\L3Data\\Foulwatr05.DUN";	break;
			case  6: dunName = "Levels\\L3Data\\Foulwatr06.DUN";	break;
			case  7: dunName = "Levels\\L3Data\\Foulwatr07.DUN";	break;
			case  8: dunName = "Levels\\L3Data\\Foulwatr08.DUN";	break;
			case  9: dunName = "Levels\\L3Data\\Foulwatr10.DUN";	break;
			default: dunName = "Levels\\L3Data\\Foulwatr09.DUN";	break;
			} 
			break;
		case DL_1_PURGATORY:	dunName = "Levels\\L3Data\\Foulwatx.DUN";	break;
		case DL_2_DOOM:
			switch (RNG(2)) {
			case 1:  dunName = "Levels\\L3Data\\Foulwatz.DUN";			break;
			default: dunName = "Levels\\L3Data\\Foulwatx_sp_01.DUN";	break;
			}
			break;
		case DL_3_CRUCIBLE:
		case DL_4_TORMENT:
		case DL_5_AGONY:
		case DL_6_TERROR:
		case DL_7_DESTRUCTION:
		case DL_8_INFERNO:
			switch (RNG(4)) {
			case 1:  dunName = "Levels\\L3Data\\Foulwatz.DUN";			break;
			case 2:  dunName = "Levels\\L3Data\\Foulwatz2.DUN";			break;
			default: dunName = "Levels\\L3Data\\Foulwatx_mp_01.DUN";	break;
			}
			break;
		}
	}
	else {// ----- MULTIPLAYER -----
		switch (Difficulty) {
		case DL_0_HORROR:		dunName = "Levels\\L3Data\\Foulwatr08_mp_01.DUN";	break;
		case DL_1_PURGATORY:	
		case DL_2_DOOM:			dunName = "Levels\\L3Data\\Foulwatx_mp_01.DUN";	break;
		case DL_3_CRUCIBLE:
		case DL_4_TORMENT:
		case DL_5_AGONY:
		case DL_6_TERROR:
		case DL_7_DESTRUCTION:
		case DL_8_INFERNO:		
			switch (RNG(4)) {
			default: dunName = "Levels\\L3Data\\Foulwatz2.DUN";			break;
			case 2:	 dunName = "Levels\\L3Data\\Foulwatx_mp_01.DUN";	break;
			}
			break;
		}
	}
	if (GameMode == GM_CLASSIC) { dunName = "Levels\\L3Data\\Foulwatr_orig.DUN"; }
	LoadQuestDungeonAltMapCave(dunName, 19, 50);
	LoadQuestDungeonMainMapCave(dunName, 20, 50);
	LoadPalette("Levels\\L3Data\\L3pfoul_th3.pal");
	AddWarpFromPoisonWell();
}

void GenerateQuestFloor_110_Black_Chapel()
{
	Quest& graveMattersQuest = Quests[Q_34_GRAVE_MATTERS];
	if (graveMattersQuest.status == QS_1_ACCEPT) {
		graveMattersQuest.status = QS_2_IN_PROGRESS;
		graveMattersQuest.status2 = 1;
	}
	//LoadQuestDungeonAltMapChurch("Levels\\L1Data\\SklKng1.DUN", 83, 45);
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L1Data\\gm1.DUN";	break;
	default: dunName = "Levels\\L1Data\\gm1.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, 23, 28);
	LoadPalette("Levels\\L1Data\\zContrastique02.pal");
	//FillAreasSomeOnMap112x112u(2, MapAreas);
	//FillAreasDiffOnMap112x112u(2, MapAreas6);
	//FillAreasSomeOnMap112x112u(5, MapAreas2);
	//FillAreasDiffOnMap112x112u(7, MapAreas4);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	//SetupLeoricTombLevers();
	AddWarp(23, 26, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_2_CHURCH_2);
}

void GenerateQuestFloor_111_Sacristy()
{
	Quest& oathQuest = Quests[Q_35_IRON_OATH];
	if (oathQuest.status == QS_1_ACCEPT) {	
		oathQuest.status = QS_2_IN_PROGRESS; 
		oathQuest.status2 = 1;	
	}
	LoadQuestDungeonMainMapChurch("Levels\\L1Data\\church_Cellar_01.DUN", 47, 54);
	LoadPalette("Levels\\L1Data\\zContrastique.pal");
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(47, 52, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_1_CHURCH_1);

	if (oathQuest.status == QS_3_COMPLETE) {
		Open_the_Gates_Sacristy();
	}
}

void GenerateQuestFloor_60_Haunted_Labyrinth()
{
	int x = 83, y = 44;
	if (Dungeon.prev == DUN_61_CURSED_SANCTUM) {
		x = 23; y = 41;
	}
	char* dunName;
	switch (RNG(3)) {
	case 2:  dunName = "Levels\\L1Data\\dun_60_03.DUN";	break;
	case 1:  dunName = "Levels\\L1Data\\dun_60_01.DUN";	break;
	default: dunName = "Levels\\L1Data\\dun_60_02.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_1.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(23, 40, WM_1029_GOTO_QUEST_DUNGEON, DUN_61_CURSED_SANCTUM);
}

void GenerateQuestFloor_61_Cursed_Sanctum()
{
	int x = 25, y = 36;
	if (Dungeon.prev == DUN_62_DIM_UNDERCROFT) {
		x = 81; y = 25;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L1Data\\dun_61_01.DUN";	break;
	default: dunName = "Levels\\L1Data\\dun_61_02.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_95.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(25, 34, WM_1029_GOTO_QUEST_DUNGEON, DUN_60_HAUNTED_LABYRINTH);
	AddWarp(81, 24, WM_1029_GOTO_QUEST_DUNGEON, DUN_62_DIM_UNDERCROFT);
}

void GenerateQuestFloor_62_Dim_Undercroft()
{
	int x = 81, y = 24;
	if (Dungeon.prev == DUN_63_FORGOTTEN_OSSUARY) {
		x = 23; y = 29;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L1Data\\dun_62_01.DUN";	break;
	default: dunName = "Levels\\L1Data\\dun_62_02.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_3.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(23, 28, WM_1029_GOTO_QUEST_DUNGEON, DUN_63_FORGOTTEN_OSSUARY);
	AddWarp(81, 22, WM_1029_GOTO_QUEST_DUNGEON, DUN_61_CURSED_SANCTUM);
}

void GenerateQuestFloor_63_Forgotten_Ossuary()
{
	int x = 81, y = 27;
	if (Dungeon.prev == DUN_62_DIM_UNDERCROFT) {
		x = 21; y = 19;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L1Data\\dun_63_01.DUN";	break;
	default: dunName = "Levels\\L1Data\\dun_63_02.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_75.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(21, 18, WM_1029_GOTO_QUEST_DUNGEON, DUN_62_DIM_UNDERCROFT);
	AddWarp(81, 26, WM_1029_GOTO_QUEST_DUNGEON, DUN_64_MAUSOLEUM);
}

void GenerateQuestFloor_64_Mausoleum()
{
	int x = 35, y = 33;
	if (Dungeon.prev == DUN_63_FORGOTTEN_OSSUARY) {
		x = 81; y = 22;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L1Data\\dun_64_01.DUN";	break;
	default: dunName = "Levels\\L1Data\\dun_64_02.DUN";	break;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_33.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(81, 20, WM_1029_GOTO_QUEST_DUNGEON, DUN_63_FORGOTTEN_OSSUARY);
	AddWarp(33, 31, WM_1029_GOTO_QUEST_DUNGEON, DUN_65_RUINED_TEMPLE);
}

void GenerateQuestFloor_114_LOWER_SCRIPTORIUM()
{
	Quest& osMP = Quests[Q_38_OGDENS_SIGN_MP];
	if (osMP.status == QS_1_ACCEPT) {
		osMP.status = QS_2_IN_PROGRESS;
		osMP.status2 = 1;
	}	
	char* dunName = "Levels\\L1Data\\lower_scriptorium_v01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, 23, 25);
	char pal[50] = "Levels\\L1Data\\l1_33.pal";
	/*if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}*/
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(23, 24, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_4_CHURCH_4);	
}

void GenerateQuestFloor_65_Ruined_Temple()
{
	char* dunName = "Levels\\L1Data\\dun_65_01.DUN";
	int x = 81, y = 23;
	if (Dungeon.prev == DUN_64_MAUSOLEUM) {
		x = 60; y = 58;
	}
	LoadQuestDungeonMainMapChurch(dunName, x, y);
	char pal[50] = "Levels\\L1Data\\l1_33.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(6) + 1;
		sprintf(pal, "Levels\\iceage\\1church_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarp(58, 58, WM_1029_GOTO_QUEST_DUNGEON, DUN_64_MAUSOLEUM);
	AddWarp(81, 22, WM_1029_GOTO_QUEST_DUNGEON, DUN_66_SEWERS);
}

void GenerateQuestFloor_66_Sewers()
{
	int x = 35, y = 53;
	if (Dungeon.prev == DUN_65_RUINED_TEMPLE) {
		x = 25; y = 25;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L2Data\\dun_66_01.DUN";	break;
	default: dunName = "Levels\\L2Data\\dun_66_02.DUN";	break;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	char pal[50] = "Levels\\L2Data\\l2_95.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
	}
	LoadPalette(pal); 
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(24, 24, WM_1029_GOTO_QUEST_DUNGEON, DUN_65_RUINED_TEMPLE);
	AddWarp(36, 53, WM_1029_GOTO_QUEST_DUNGEON, DUN_67_SEPULCHER);
}

void GenerateQuestFloor_67_Sepulcher()
{
	int x = 25, y = 25;
	if (Dungeon.prev == DUN_66_SEWERS) {
		x = 85; y = 85;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L2Data\\dun_67_01.DUN";	break;
	default: dunName = "Levels\\L2Data\\dun_67_02.DUN";	break;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	char pal[50] = "Levels\\L2Data\\l2_11.pal";
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
	}
	LoadPalette(pal);
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(84, 85, WM_1029_GOTO_QUEST_DUNGEON, DUN_66_SEWERS);
	AddWarp(26, 25, WM_1029_GOTO_QUEST_DUNGEON, DUN_68_OUBLIETTE);
}

void GenerateQuestFloor_68_Oubliette()
{
	char* dunName = "Levels\\L2Data\\dun_68_01.DUN";
	int x = 81, y = 21;
	if (Dungeon.prev == DUN_67_SEPULCHER) {
		x = 21; y = 21;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(80) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L2Data\\l2_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(20, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_67_SEPULCHER);
	AddWarp(82, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_69_GIBBET);
}

void GenerateQuestFloor_69_Gibbet()
{
	char* dunName = "Levels\\L2Data\\dun_69_01.DUN";
	int x = 61, y = 21;
	if (Dungeon.prev == DUN_68_OUBLIETTE) {
		x = 83; y = 21;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(80) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L2Data\\l2_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(82, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_68_OUBLIETTE);
	AddWarp(62, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_70_CITADEL);
}

void GenerateQuestFloor_70_Citadel()
{
	char* dunName = "Levels\\L2Data\\dun_70_01.DUN";
	int x = 53, y = 21;
	if (Dungeon.prev == DUN_69_GIBBET) {
		x = 51; y = 21;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(80) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L2Data\\l2_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(50, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_69_GIBBET);
	AddWarp(54, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_71_GRIM_VAULTS);
}

void GenerateQuestFloor_71_Grim_Vaults()
{
	char* dunName = "Levels\\L2Data\\dun_71_01.DUN";
	int x = 21, y = 87;
	if (Dungeon.prev == DUN_70_CITADEL) {
		x = 53; y = 59;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(80) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L2Data\\l2_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(52, 59, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1);
	AddWarp(22, 87, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1);
}

void GenerateQuestFloor_72_Necropolis()
{
	char* dunName = "Levels\\L2Data\\dun_72_01.DUN";
	int x = 47, y = 29;
	if (Dungeon.prev == Dungeon - 1) {
		x = 23; y = 85;
	}
	LoadQuestDungeonMapMainCatacomb(dunName, x, y);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\2catas_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(80) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L2Data\\l2_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarp(22, 85, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1);
	AddWarp(48, 29, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1);
}

void GenerateQuestFloor_73_Lava_Pits()
{
	// dun selection
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L3Data\\dun_73_01.DUN";	break;
	default: dunName = "Levels\\L3Data\\dun_73_02.DUN";	break;
	}
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function
	
	LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	// arrive coordinates
	int x = 19, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 23; y = 19;
	}
	PlayerRowPos = x; PlayerColPos = y;
	// warps
	AddWarp(23, 18, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 87, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	Lava_Pits_OneSecondTimer = 0;
	Lava_Pits_MonsterSpawnTimer = 0;
}

void GenerateQuestFloor_74_Crater()
{
	// dun selection
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L3Data\\dun_74_02.DUN"; break;
	default: dunName = "Levels\\L3Data\\dun_74_01.DUN"; break;
	}
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function
	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	// arrive coordinates
	int x = 19, y = 21;
	if (Dungeon.prev == Dungeon - 1) {
		x = 87; y = 85;
	}
	PlayerRowPos = x; PlayerColPos = y;
	// warps
	AddWarp(87, 84, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 21, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_75_River_of_Fire()
{
	// dun selection
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L3Data\\dun_75_02.DUN"; break;
	default: dunName = "Levels\\L3Data\\dun_75_01.DUN"; break;
	}
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function
	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}
	// arrive coordinates
	int x = 39, y = 83;
	if (Dungeon.prev == Dungeon - 1) {
		x = 79; y = 29;
	}
	PlayerRowPos = x; PlayerColPos = y;
	// warps
	AddWarp(79, 28, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(38, 83, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_76_Den_of_Evil()
{
	// dun selection
	char* dunName = "Levels\\L3Data\\dun_76_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// arrive coordinates
	int x = 20, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 23; y = 19;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// warps
	AddWarp(23, 18, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 87, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_77_Vile_Tunnels()
{
	// dun selection
	char* dunName = "Levels\\L3Data\\dun_77_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// arrive coordinates
	int x = 86, y = 79;
	if (Dungeon.prev == Dungeon - 1) {
		x = 23; y = 85;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// warps
	AddWarp(23, 84, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(84, 79, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_78_Bridge()
{
	// dun selection
	char* dunName = "Levels\\L3Data\\dun_78_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// arrive coordinates
	int x = 19, y = 57;
	if (Dungeon.prev == Dungeon - 1) {
		x = 73; y = 87;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// warps
	AddWarp(73, 86, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 57, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_79_Colonnade()
{
	// dun selection
	char* dunName = "Levels\\L3Data\\dun_79_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// arrive coordinates
	int x = 26, y = 57;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 30;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// warps
	AddWarp(85, 28, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(24, 57, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

void GenerateQuestFloor_80_Hell_Gate()
{
	// dun selection
	char* dunName = "Levels\\L3Data\\dun_80_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 1, 1);// coordinates don't work for this cave function

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\L3Data\\lava_pits.pal");
	}
	else {
		int pal = RNG(94) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L3Data\\l3_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// arrive coordinates
	int x = 45, y = 43;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 19;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// warps
	AddWarp(85, 18, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(44, 43, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
	AddCaveDoors(0, 0, 112, 112);
}

//------------------------------------------------- AR Hell duns ---------------------------------------------------

void GenerateQuestFloor_81_Underworld()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_81_01.DUN";

	// arrive coordinates
	int x = 25, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 88; y = 26;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(89, 26, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(24, 23, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_82_Halls_of_Suffering()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_82_01.DUN";

	// arrive coordinates
	int x = 89, y = 71;
	if (Dungeon.prev == Dungeon - 1) {
		x = 24; y = 24;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(23, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(88, 71, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_83_Blood_Keep()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_83_01.DUN";

	// arrive coordinates
	int x = 25, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 24; y = 88;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L4Data\\l4_86x2bloodwalls_bones_bloodfloor_darkerfloor.pal");
	}

	// warps
	AddWarp(23, 88, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(24, 23, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_84_Armories_of_Hell()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_84_01.DUN";

	// arrive coordinates
	int x = 33, y = 35;
	if (Dungeon.prev == Dungeon - 1) {
		x = 72; y = 36;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L4Data\\l4_64-Prototype-of-darkness_blue_floor.pal");
	}

	// warps
	AddWarp(73, 36, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(32, 35, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_85_Demon_Stronghold()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_85_01.DUN";

	// arrive coordinates
	int x = 89, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 86; y = 88;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L4Data\\Demon_Stronghold_byatsar_pual.pal");
	}

	// warps
	AddWarp(87, 88, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(88, 23, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_86_Valley_of_Wrath()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_86_01.DUN";

	// arrive coordinates
	int x = 25, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 88; y = 24;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(89, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(24, 23, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_87_Pandemonium()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_87_01.DUN";

	// arrive coordinates
	int x = 33, y = 77;
	if (Dungeon.prev == Dungeon - 1) {
		x = 88; y = 27;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(88, 26, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(32, 77, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_88_Realm_of_Terror()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_88_01.DUN";

	// arrive coordinates
	int x = 87, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 54; y = 54;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(55, 54, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(86, 87, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_89_Bedlam()
{
	// dun selection
	char* dunName = "Levels\\L4Data\\dun_89_01.DUN";

	// arrive coordinates
	int x = 25, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 56; y = 58;
	}
	LoadQuestDungeonMainMapHell(dunName, x, y);

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(4) + 1;
		sprintf(pal, "Levels\\iceage\\4hell_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(72) + 1;
		char paletteFile[260];
		sprintf(paletteFile, "Levels\\L4Data\\l4_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(57, 58, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(24, 87, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

//------------------------------------------------- AR Abyss duns ---------------------------------------------------

void GenerateQuestFloor_90_Fields_of_Madness()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_90_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 19, y = 21;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 87;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {		
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		int pal = RNG(166) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "nLevels\\L6Data\\l6base%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(85, 86, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 21, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_91_Stygian_Passage()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_91_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 19, y = 21;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 87;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		int pal = RNG(166) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "nLevels\\L6Data\\l6base%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(85, 86, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 21, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_92_Flesh_Pit()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_92_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 31, y = 35;
	if (Dungeon.prev == Dungeon - 1) {
		x = 21; y = 55;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		int pal = RNG(166) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "nLevels\\L6Data\\l6base%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(21, 54, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(30, 35, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_93_Bleeding_Veins_Path()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_93_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 89, y = 85;
	if (Dungeon.prev == Dungeon - 1) {
		x = 53; y = 21;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette	
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		LoadPalette("nLevels\\L6Data\\l6base161.pal");
	}

	// warps
	AddWarp(53, 20, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(88, 85, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_94_Rot_Chambers()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_94_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 19, y = 49;
	if (Dungeon.prev == Dungeon - 1) {
		x = 89; y = 55;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		int pal = RNG(166) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "nLevels\\L6Data\\l6base%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(89, 54, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 49, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_95_Marrow_Garden()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_95_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 17, y = 25;
	if (Dungeon.prev == Dungeon - 1) {
		x = 19; y = 21;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		int pal = RNG(166) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "nLevels\\L6Data\\l6base%i.pal", pal);
		LoadPalette(paletteFile);
	}

	// warps
	AddWarp(19, 20, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(16, 25, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_96_Bone_Mire()
{
	// dun selection
	char* dunName = "Levels\\L6Data\\dun_96_01.DUN";
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 89, y = 55;
	if (Dungeon.prev == Dungeon - 1) {
		x = 53; y = 89;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// level color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		LoadPalette("nlevels\\l6data\\l6base93.pal");
	}

	// warps
	AddWarp(53, 88, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(88, 55, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_97_Gloomy_Depths()
{
	// dun selection
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "Levels\\L6Data\\dun_97_01.DUN";	break;
	default: dunName = "Levels\\L6Data\\dun_97_02.DUN";	break;
	}
	LoadQuestDungeonMainMapCave(dunName, 0, 0);

	// arrive coordinates
	int x = 17, y = 89;
	if (Dungeon.prev == Dungeon - 1) {
		x = 89; y = 23;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// dun color palette	
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		LoadPalette("nlevels\\l6data\\l6base96.pal");
	}

	// warps
	AddWarp(89, 22, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(16, 89, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_98_Black_Chasm()
{
	// dun selection
	LoadQuestDungeonMainMapCave("Levels\\L6Data\\dun_98_01.DUN", 0, 0);

	// arrive coordinates
	int x = 61, y = 49;
	if (Dungeon.prev == Dungeon - 1) {
		x = 63; y = 59;
	}
	PlayerRowPos = x; PlayerColPos = y;

	// dun color palette	
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("Levels\\iceage\\6abyss.PAL");
	}
	else {
		LoadPalette("levels\\l6data\\zBlakKazm.pal");
	}

	// warps
	AddWarp(63, 58, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(60, 49, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

//------------------------------------------------- AR Crypt duns ---------------------------------------------------

void GenerateQuestFloor_99_Ancient_Crypts()
{	
	// arrive coordinates
	int x = 85, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 23; y = 19;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_99_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		int pal = RNG(312) + 1;
		char paletteFile[70];
		sprintf(paletteFile, "Levels\\L5Data\\l5_%i.pal", pal);
		LoadPalette(paletteFile);
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(22, 20, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(85, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_100_Horadrim_Treasury()
{
	// arrive coordinates
	int x = 55, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 83;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_100_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_289.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(85, 84, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(55, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_101_Void_Chambers()
{
	// arrive coordinates
	int x = 19, y = 17;
	if (Dungeon.prev == Dungeon - 1) {
		x = 53; y = 19;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_101_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette	
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_294.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(52, 20, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 19, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_102_Oblivion_Trail()
{
	// arrive coordinates
	int x = 71, y = 41;
	if (Dungeon.prev == Dungeon - 1) {
		x = 19; y = 83;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_102_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette	
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_284.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(18, 85, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(71, 42, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_103_Altar_of_Immolation()
{
	// arrive coordinates
	int x = 21, y = 23;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 85;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_103_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_243.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(84, 86, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(21, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_104_Netherworld_Gates()
{
	// arrive coordinates
	int x = 19, y = 51;
	if (Dungeon.prev == Dungeon - 1) {
		x = 85; y = 53;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_104_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_234.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(84, 54, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(18, 53, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_105_Dark_Descent()
{
	// arrive coordinates
	int x = 87, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 21; y = 55;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_105_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_203.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(20, 56, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(87, 88, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_106_Bastion_of_Night()
{
	// arrive coordinates
	int x = 21, y = 87;
	if (Dungeon.prev == Dungeon - 1) {
		x = 57; y = 57;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_106_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_301.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(56, 58, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(21, 88, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_107_Dread_Sanctuary()
{
	// arrive coordinates
	int x = 43, y = 77;
	if (Dungeon.prev == Dungeon - 1) {
		x = 87; y = 23;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_107_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_123.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(86, 24, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
	AddWarp(43, 78, WM_1029_GOTO_QUEST_DUNGEON, Dungeon + 1); // warp to next dungeon
}

void GenerateQuestFloor_108_Arcane_Prison()
{
	// arrive coordinates
	int x = 43, y = 79;
	if (Dungeon.prev == Dungeon - 1) {
		x = 81; y = 59;
	}

	// dun select
	char* dunName = "Levels\\L5Data\\dun_108_01.DUN";
	LoadQuestDungeonMainMapChurch(dunName, x, y);

	// dun color palette
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		char pal[50];
		int pal_number = RNG(3) + 1;
		sprintf(pal, "Levels\\iceage\\5crypt_%i.PAL", pal_number);
		LoadPalette(pal);
	}
	else {
		LoadPalette("Levels\\L5Data\\l5_52.pal");
	}

	AddCryptDoors(0, 0, 112, 112);

	// warps
	AddWarp(80, 60, WM_1029_GOTO_QUEST_DUNGEON, Dungeon - 1); // warp to previous dungeon
}

//--------- end of AR =------------------

void GenerateQuestFloor_44_Ravenholm()
{
	int row = 93, col = 90;
	if (Dungeon.prev == DUN_45_THE_DEN) {
		row = 83; col = 56;
	}
	LoadQuestDungeonMainMapTown("LEVELS\\TOWNDATA\\RAVENHOLM.DUN", row, col);
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		LoadPalette("LEVELS\\iceage\\0town.PAL");
	}
	else {
		LoadPalette("LEVELS\\L0DATA\\L0_DARK04.PAL");
	}
	//SelectRandomLevelPalette( 0 );
	AddWarpFromRavenholm();

}

void GenerateQuestFloor_45_TheDen()
{
	LoadQuestDungeonMapAltCatacomb("Levels\\L2Data\\cerb2.DUN");
	LoadQuestDungeonMapMainCatacomb("Levels\\L2Data\\cerb.DUN", 57, 57);
	LoadPalette("Levels\\L2Data\\cerb.pal");
	AddDoorsObjectInCatacomb(0, 0, 112, 112);
	AddWarpFromTheDen();
	if (Quests[Q_24_THE_RAMPAGING_DEMON].status2 > 2) {
		ChangeMapRectAfterScriptEvent2(0, 0, 39, 39);
	}
	TheDenOneSecondTimer = 0;
	TheDenMonsterSpawnTimer = 0;
}

void GenerateQuestFloor_46_Coliseum()
{
	LoadQuestDungeonMapAltCatacomb("levels\\l2data\\ar_1_1.dun");
	LoadQuestDungeonMapMainCatacomb("levels\\l2data\\ar_1_1.dun", 61, 61);
	//LoadPalette("Levels\\L2Data\\cerb.pal");
	SelectRandomLevelPalette(DT_2_CATACOMB);
	AddWarpFromTheColiseum();
	CastColiseumPortal = true;
}

// ---- (th4) -------------------------------------------------------------
void GenerateQuestFloor_112_StillSprings()
{
	Quest& clearWaterQuest = Quests[Q_36_CLEAR_WATER];
	if (clearWaterQuest.status == QS_1_ACCEPT) {
		clearWaterQuest.status = QS_2_IN_PROGRESS;
		clearWaterQuest.talkToFlag = 1;
	}
	char* dunName = "Levels\\L3Data\\springs_1.DUN";
	/*if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)) {
		dunName = "Levels\\L3Data\\fetid_cavern_01-im.DUN";
	}*/

	LoadQuestDungeonAltMapCave(dunName, 19, 50);
	LoadQuestDungeonMainMapCave(dunName, 20, 50);
	LoadPalette("Levels\\L3Data\\mossy_grotto_v03.pal");
	
	PlayerRowPos = 55;
	PlayerColPos = 57;

	AddWarp(55, 56, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_11_CAVE_3);

	if (clearWaterQuest.status == QS_3_COMPLETE) {
		Open_the_Gates_Springs();
	}
}

void GenerateQuestFloor_51_FetidCavern()
{
	Quest& ironFigurineQuest = Quests[Q_29_IRON_FIGURINE];
	if (ironFigurineQuest.status == QS_1_ACCEPT) {
		ironFigurineQuest.status = QS_2_IN_PROGRESS;
		ironFigurineQuest.talkToFlag = 1;//If player enters SC before talking to Griz, make sure quest log still shows IF quest.
	}
	char* dunName = "Levels\\L3Data\\fetid_cavern_01.DUN";
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)) {
		dunName = "Levels\\L3Data\\fetid_cavern_01-im.DUN"; 
	}

	LoadQuestDungeonAltMapCave(dunName, 19, 50);
	LoadQuestDungeonMainMapCave(dunName, 20, 50);
	LoadPalette("Levels\\L3Data\\swamp_v02.pal");
	AddWarpFromFetidCavern();

	if (Dungeon.prev == DUN_27_THE_INFESTED_CELLAR) {
		PlayerRowPos = 41;
		PlayerColPos = 21;
	}
	else {
		PlayerRowPos = 33;
		PlayerColPos = 55;
	}

	if (ironFigurineQuest.status == QS_3_COMPLETE) {
		Open_the_Gates_Fetid_Cavern();
	}
}

void GenerateQuestFloor_52_Mossy_Grotto()
{
	Quest& demon_Plague_Quest = Quests[Q_30_DEMON_PLAGUE];
	if (demon_Plague_Quest.status == QS_1_ACCEPT) {
		demon_Plague_Quest.status = QS_2_IN_PROGRESS;
		demon_Plague_Quest.talkToFlag = 1;
	}
	char* dunName; 
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)) {
		switch (RNG(2)) {
		case 1:		dunName = "Levels\\L3Data\\mossy_grotto_01-im.DUN"; break;
		default:	dunName = "Levels\\L3Data\\mossy_grotto_02-im.DUN"; break;
		}
	}
	else {
		switch (RNG(2)) {
		case 1:		dunName = "Levels\\L3Data\\mossy_grotto_01.DUN"; break;
		default:	dunName = "Levels\\L3Data\\mossy_grotto_02.DUN"; break;
		}
	}

	LoadQuestDungeonAltMapCave(dunName, 19, 50);
	LoadQuestDungeonMainMapCave(dunName, 20, 50);
	LoadPalette("Levels\\L3Data\\mossy_grotto_v05_deep_purple_greener_grass.pal");
	AddWarp(33, 54, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_9_CAVE_1);

	if (Dungeon.prev == DUN_37_DISUSED_RELIQUARY) {
		PlayerRowPos = 44;
		PlayerColPos = 22;
	}
	else {
		PlayerRowPos = 33;
		PlayerColPos = 55;
	}

	if( is(GameMode, GM_IRONMAN, GM_SURVIVAL) ){
		AddWarp(44, 21, WM_1029_GOTO_QUEST_DUNGEON, DUN_37_DISUSED_RELIQUARY);
		// PlayerRowPos = 33;
		// PlayerColPos = 55;
	}
	if (demon_Plague_Quest.status == QS_3_COMPLETE) {
		Open_the_Gates_Mossy_Grotto();
	}
}

//!---- (0046B2B3) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_29_ArchbishopLazarusLair()
{
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	if( lazarusQuest.status == QS_3_COMPLETE ){
		lazarusQuest.status3 = LQAS_4;
	}else if( lazarusQuest.status == QS_2_IN_PROGRESS ){
		lazarusQuest.status3 = LQAS_3;
	}
	LoadQuestDungeonAltMapChurch("Levels\\L1Data\\Vile1.DUN", 35, 36);
	char* dunName;
	switch (Difficulty ){
		case DL_0_HORROR:	dunName = "Levels\\L1Data\\Vileh.DUN";	break;
		case DL_1_PURGATORY:	dunName = "Levels\\L1Data\\Vile2.DUN";	break;
		case DL_2_DOOM or 3 or 4 or 5 or 6 or 7 or 8:	dunName = "Levels\\L1Data\\Viled.DUN";	break;
	}
	if (GameMode == GM_CLASSIC) dunName = "Levels\\L1Data\\Vile2_CLS.DUN";
	LoadQuestDungeonMainMapChurch(dunName, 35, 36);
	LoadPalette("Levels\\L1Data\\Laz1.pal");
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	SetupUnholyAltarLevers();
	LoadQuestHallMap("Levels\\L1Data\\Vile1.DUN");
	ClearWarps(); // TODO: check diz out
}

//!---- (00715A90) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_30_HallsOfAnguish()
{
	Player& player = Players[CurrentPlayerIndex];
	
	Quest& andiQuest = Quests[Q_22_MAIDEN_OF_ANGUISH];
	if( andiQuest.status == QS_1_ACCEPT ){
		andiQuest.status = QS_2_IN_PROGRESS;
		andiQuest.talkToFlag = 1;//If player enters Halls of Anguish before talking to Gillian, make sure quest log still shows Andariel quest.
	}
	char* dunName; // = HallsOfAnguishRandomization();
	switch ( RNG(8) ) {
	case 1:		dunName = "levels\\l4data\\andariel3.dun"; break;
	case 2:		dunName = "levels\\l4data\\andariel4.dun"; break;
	case 3:		dunName = "levels\\l4data\\andariel5.dun"; break;
	case 4:		dunName = "levels\\l4data\\andariel6.dun"; break;
	case 5:		dunName = "levels\\l4data\\andariel7.dun"; break;
	case 6:		dunName = "levels\\l4data\\andariel8.dun"; break;
	case 7:		dunName = "levels\\l4data\\andariel9.dun"; break;
	default:	dunName = "levels\\l4data\\andariel10.dun"; break;
	}
	LoadQuestDungeonAltMapHell(dunName, 20, 50);
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_31_ANDARIELS_THRONE ){
		playerRow = 88;
		playerCol = 26;
	}else{
		playerRow = 89;
		playerCol = 89;
	}	
	LoadQuestDungeonMainMapHell(dunName, playerRow, playerCol);			
	LoadPalette("levels\\l4data\\And1.pal");
	AddWarpFromHallsOfAnguish();
}

//!---- (00715A5A) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_31_AndarielsThrone()
{
	Player& player = Players[CurrentPlayerIndex];
	LoadQuestDungeonAltMapHell("levels\\l4data\\AndaThroneHorror1.dun", 20, 50);
	LoadQuestDungeonMainMapHell("levels\\l4data\\AndaThroneHorror1.dun", 48, 26);
	LoadPalette("levels\\l4data\\And2.pal");
	ClearWarps(); // TODO: check diz out
}

/*__forceinline*/ void GenerateQuestFloor_113_Maze()
{
	Player& player = Players[CurrentPlayerIndex];

	Quest& hubrisQuest = Quests[Q_37_PRICE_OF_HUBRIS];
	if (hubrisQuest.status == QS_1_ACCEPT) {
		hubrisQuest.status = QS_2_IN_PROGRESS;
		hubrisQuest.talkToFlag = 1;
	}
	LoadQuestDungeonMainMapHell("levels\\l4data\\maze1.dun", 55, 57);
	LoadPalette("levels\\l4data\\maze_01.pal");
	AddWarp(55, 56, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_13_HELL_1);
}

void GenerateQuestFloor_50_Temple_of_Sunset()
{
	Quest& vov_Quest = Quests[Q_28_VISIONS_OF_VICTORY];
	if (vov_Quest.status == QS_1_ACCEPT) {
		vov_Quest.status = QS_2_IN_PROGRESS;
		vov_Quest.talkToFlag = 1;
	}
	LoadQuestDungeonAltMapChurch("levels\\l5data\\tos_01.DUN", 83, 45);	// is it needed?
	int playerRow = 83, playerCol = 83;
	
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\tos_01.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\tos_02.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarp(83, 84, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_22_CRYPT_2); 
	
	if (vov_Quest.status == QS_3_COMPLETE) {
		Open_the_Gates_Temple_of_Sunset();
	}
}

void GenerateQuestFloor_54_Abandoned_Fane()
{
	Quest& ast_Quest = Quests[Q_32_A_SIMPLE_TASK];
	if (ast_Quest.status == QS_1_ACCEPT) {
		ast_Quest.status = QS_2_IN_PROGRESS;
		ast_Quest.talkToFlag = 1;
	}
	char* af_dun = "levels\\l5data\\abandoned_fane_01.DUN";
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)){ af_dun = "levels\\l5data\\abandoned_fane_01-im.DUN"; }
	LoadQuestDungeonAltMapChurch(af_dun, 22, 22);	// is it needed?
	int playerRow = 83, playerCol = 83;

	LoadQuestDungeonMainMapChurch(af_dun, playerRow, playerCol);
	LoadPalette("levels\\l5data\\abandoned_fane_02.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarp(18, 46, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_21_CRYPT_1);

	if( is(GameMode, GM_IRONMAN, GM_SURVIVAL) ){
		AddWarp(21, 34, WM_1029_GOTO_QUEST_DUNGEON, DUN_30_HALLS_OF_ANGUISH);
	}

	if (Dungeon.prev == DUN_30_HALLS_OF_ANGUISH) {
		PlayerRowPos = 22;
		PlayerColPos = 32;
	}else{
		PlayerRowPos = 19;
		PlayerColPos = 44;
	}
	
}

//!---- (00716B20) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_32_HorazonsHaven()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status == QS_1_ACCEPT ){
		horazonQuest.status = QS_2_IN_PROGRESS;
		horazonQuest.talkToFlag = 1;//If player enters Horazon Haven before reading the Book, make sure quest log still shows Horazon quest.
	}
	if (MaxCountOfPlayersInGame != 1) {
		LoadQuestDungeonAltMapChurch("levels\\l5data\\Horazon01_MP.DUN", 83, 45);
	}
	else {
		LoadQuestDungeonAltMapChurch("levels\\l5data\\Horazon01.DUN", 83, 45);
	}
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_33_SUMMONING_ROOMS ){
		playerRow = 59;
		playerCol = 55;
	}else if( Dungeon.prev == DUN_34_THE_PIT ){
		playerRow = 25;
		playerCol = 19;
	}else{
		playerRow = 81;
		playerCol = 23;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon01.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon01.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarp(81, 22, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_23_CRYPT_3);
	AddWarp(59, 54, WM_1029_GOTO_QUEST_DUNGEON, DUN_33_SUMMONING_ROOMS);
	AddWarp(25, 17, WM_1029_GOTO_QUEST_DUNGEON, DUN_34_THE_PIT);
	SetupHorazon1Books();
	if( horazonQuest.status == QS_3_COMPLETE ){
		ChangeMapTile(18, 1, 11);
		InitDungeonTiles();
	}
}

//!---- (00716EF0) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_33_SummoningRooms()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	LoadQuestDungeonAltMapChurch("Levels\\L5Data\\Horazon02.DUN", 83, 45);
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_32_HORAZONS_HAVEN ){
		playerRow = 27;
		playerCol = 85;
	}else{
		playerRow = 43;
		playerCol = 41;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon02.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon02.PAL");
	AddCryptDoors(0, 0, 112, 112);
	AddWarp(27, 84, WM_1029_GOTO_QUEST_DUNGEON, DUN_32_HORAZONS_HAVEN);
	AddWarp(43, 39, WM_1029_GOTO_QUEST_DUNGEON, DUN_34_THE_PIT);
	SetupHorazon2Books();
	if( horazonQuest.status2 >= 2 ){
		OpenNakrulRoom(false);
	}
}

//!---- (00716F70) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_34_ThePit()
{
	Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
	if( horazonQuest.status3 == 0 ){// не заходили ранее
		horazonQuest.status3 = 1;// только зашли
		PlayerSoundMessageDelay = 10;
		SoundMessage = S_967_M_WLOCK01;
	}
	LeverClickPlayerSound = -1;
	LeverClickPlayerSoundIndex = 0;
	LoadQuestDungeonAltMapChurch("levels\\l5data\\Horazon03.DUN", 83, 45);
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_32_HORAZONS_HAVEN ){
		playerRow = 25;
		playerCol = 23;
	}else{
		playerRow = 19;
		playerCol = 45;
	}
	LoadQuestDungeonMainMapChurch("Levels\\L5Data\\Horazon03.DUN", playerRow, playerCol);
	LoadPalette("levels\\l5data\\Horazon03.pal");
	AddCryptDoors(0, 0, 112, 112);
	AddWarp(25, 22, WM_1029_GOTO_QUEST_DUNGEON, DUN_32_HORAZONS_HAVEN);
	AddWarp(17, 45, WM_1029_GOTO_QUEST_DUNGEON, DUN_33_SUMMONING_ROOMS);
	SetupHorazon3Books();
	if( horazonQuest.status3 >= 3 ){// уже открыли
		OpenGrimspikeRoom(false);// бесшумное открытие дверей
	}
}

//!---- (0071A135) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_35_PassageOfFire()
{
	Quest& izualQuest = Quests[Q_17_FALLEN_ANGEL];
	if( izualQuest.status == QS_1_ACCEPT ){
		izualQuest.status = QS_2_IN_PROGRESS;
		izualQuest.talkToFlag = 1;//If player enters Passage of Fire before talking to Gillian, make sure quest log still shows Izual quest.
	}
	LoadQuestDungeonAltMapCave("levels\\l3data\\passfire.dun", 20, 50);
	char* dunName;
	if (MaxCountOfPlayersInGame == 1) {// single player
		if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN)) {
			switch (RNG(5)) {
			case 1:		dunName = "levels\\l3data\\passfire03.dun";	break;
			case 2:		dunName = "levels\\l3data\\passfire04.dun";	break;
			case 3:		dunName = "levels\\l3data\\passfire05.dun";	break;
			case 4:		dunName = "levels\\l3data\\passfire06.dun";	break;
			default:	dunName = "levels\\l3data\\passfire07.dun";	break;
			//default:	dunName = "levels\\l3data\\passfire08.dun";	break;
			//default:	dunName = "levels\\l3data\\passfire.dun";	break; // this map was too tough for ranged characters
			}
		}
		else {
			switch (RNG(8)) {
			case 1:		dunName = "levels\\l3data\\passfire02.dun";	break;
			case 2:		dunName = "levels\\l3data\\passfire03.dun";	break;
			case 3:		dunName = "levels\\l3data\\passfire04.dun";	break;
			case 4:		dunName = "levels\\l3data\\passfire05.dun";	break;
			case 5:		dunName = "levels\\l3data\\passfire06.dun";	break;
			case 6:		dunName = "levels\\l3data\\passfire07.dun";	break;
			case 7:		dunName = "levels\\l3data\\passfire08.dun";	break;
			default:	dunName = "levels\\l3data\\passfire.dun";	break;
			}
		}
	}
	else { // multi player
		switch (RNG(4)) {
		case 1:		dunName = "levels\\l3data\\passfire.dun"; break;
		case 2:		dunName = "levels\\l3data\\passfire08.dun";	break;
		default:	dunName = "levels\\l3data\\passfire02.dun";	break; // difficult map (onslaught)
		}
	}
	LoadQuestDungeonMainMapCave(dunName, 19, 50);

	LoadPalette("levels\\l3data\\passfire.pal");
	bool isPortalOpen = izualQuest.status2 != 0;
	if( isPortalOpen ){
		izualQuest.status3 = 1;
	}
	AddWarpFromFirePassage(isPortalOpen);
	if( Dungeon.prev == DUN_36_VALLEY_OF_DESPAIR ){
		PlayerRowPos = 28;
		PlayerColPos = 41;
	}else{
		PlayerRowPos = 77;
		PlayerColPos = 41;
	}
}

//!---- (0071A750) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_36_ValleyOfDespair()
{
	Quest& izualQuest = Quests[Q_17_FALLEN_ANGEL];
	if( izualQuest.status2 < 2 ){
		PlayerSoundMessageDelay = 30;
		SoundMessage = S_992_M_IZUAL01;
		izualQuest.status2 = 2;
	}
	LoadQuestDungeonAltMapCave("levels\\l6data\\valley01.dun", 20, 50);
	char* dunName;
	switch (RNG(2)) {
	case 1:		dunName = "levels\\l6data\\valley01.dun";	break;
	default:	dunName = "levels\\l6data\\valley02.dun";	break;
	}
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\valley01.pal");
	if( izualQuest.status2 ){
		izualQuest.status3 = 1;
	}
	AddWarpFromIzualValey();
	PlayerRowPos = 19;
	PlayerColPos = 89;
}

void GenerateQuestFloor_56_Web_Tunnels()
{
	Quest& strangeBookQuest = Quests[Q_33_STRANGE_BOOK];
	//LoadQuestDungeonAltMapCave("levels\\l6data\\silkee_01.dun", 20, 50);
	char* dunName = "levels\\l6data\\webs_01.dun";
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\zWebs_v1.pal");
	AddWarp(86, 59, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_17_ABYSS_1);

	if (Dungeon.prev == DUN_57_ARACHNID_LAIR) {
		PlayerRowPos = 44;
		PlayerColPos = 81;
	}else{
		PlayerRowPos = 88;// done
		PlayerColPos = 59;
	}

	AddWarp(42, 81, WM_1029_GOTO_QUEST_DUNGEON, DUN_57_ARACHNID_LAIR); 

	if (strangeBookQuest.status == QS_4_TOOWEAK1 && is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_SURVIVAL, GM_NIGHTMARE)) {
		Open_the_Gates_Web_Tunnels();
	}
}

void GenerateQuestFloor_57_Arachnid_Lair()
{
	Quest& strange_book = Quests[Q_33_STRANGE_BOOK]; 
	char* dunName = "levels\\l6data\\arachnid_lair_01.dun";
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\zAra_v1.pal");
	AddWarp(42, 81, WM_1029_GOTO_QUEST_DUNGEON, DUN_56_WEB_TUNNELS);

	//coordinates for arriving to level:
	PlayerRowPos = 44;
	PlayerColPos = 81;

	if (strange_book.status == QS_4_TOOWEAK1 && is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_SURVIVAL, GM_NIGHTMARE)) {
		Open_the_Gates_Arachnid_Lair();
	}

}

void GenerateQuestFloor_47_Dark_Nest()
{
	Quest& lockedChestQuest = Quests[Q_25_THE_LOCKED_CHEST];	
	LoadQuestDungeonAltMapCave("levels\\l6data\\dd_01.dun", 20, 50);
	char* dunName; 	
	switch (RNG(3)) {
	case 2:		dunName = "levels\\l6data\\dd_03.dun";	break;
	case 1:		dunName = "levels\\l6data\\dd_02.dun";	break;
	default:	dunName = "levels\\l6data\\dd_01.dun";	break;
	}
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\zCaramel_avec_blood_v3.pal");
	if (lockedChestQuest.status2) {
		lockedChestQuest.status3 = 1;
	}
	AddWarp(88, 19, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_18_ABYSS_2);

	if (Dungeon.prev == DUN_55_DEEP_HOLLOW) {
		PlayerRowPos = 17;
		PlayerColPos = 87;
	}
	else {
		PlayerRowPos = 89;
		PlayerColPos = 19;
	}

	AddWarp(16, 87, WM_1029_GOTO_QUEST_DUNGEON, DUN_55_DEEP_HOLLOW);
}

void GenerateQuestFloor_55_Deep_Hollow()
{
	//Quest& lockedChestQuest = Quests[Q_25_THE_LOCKED_CHEST];
	LoadQuestDungeonAltMapCave("levels\\l6data\\deep_hollow_01.dun", 20, 50);
	char* dunName = "levels\\l6data\\deep_hollow_01.dun";
	/*switch (RNG(3)) {
	case 2:		dunName = "levels\\l6data\\dd_03.dun";	break;
	case 1:		dunName = "levels\\l6data\\dd_02.dun";	break;
	default:	dunName = "levels\\l6data\\dd_01.dun";	break;
	}*/
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\zCaramel_avec_blood_v3.pal");
	/*if (lockedChestQuest.status2) {
		lockedChestQuest.status3 = 1;
	}*/
	AddWarp(16, 19, WM_1029_GOTO_QUEST_DUNGEON, DUN_47_DARK_NEST);
	
	//coordinates for arriving to level. this works
	PlayerRowPos = 17;
	PlayerColPos = 19;	
}

void GenerateQuestFloor_48_Blood_Hive()
{
	Quest& lockedChestQuest = Quests[Q_26_SPECIAL_WEAPON];
	
	LoadQuestDungeonAltMapCave("levels\\l6data\\blood_hive_01.dun", 20, 50);
	char* dunName;
	dunName = "levels\\l6data\\blood_hive_01.dun";
	/*switch (RNG(3)) {
	case 2:		dunName = "levels\\l6data\\dd_03.dun";	break;
	case 1:		dunName = "levels\\l6data\\dd_02.dun";	break;
	default:	dunName = "levels\\l6data\\dd_01.dun";	break;
	}*/
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l6data\\zBlood_Hive_v04.pal");
	if (lockedChestQuest.status2) {
		lockedChestQuest.status3 = 1;
	}
	AddWarpFrom_BloodHive();
	PlayerRowPos = 89;
	PlayerColPos = 19; 

	if (lockedChestQuest.status == QS_3_COMPLETE) {
		Open_the_Gates_Blood_Hive();
	}	
}

//!---- (0071B5EA) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_37_DisusedReliquary()
{
	
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_38_GLACIAL_CAVERNS ){
		playerRow = 23;
		playerCol = 19;
	}else{
		playerRow = 25;
		playerCol = 88;
	}
	
	char* dunName;
	switch (Difficulty) {
	case DL_0_HORROR:	
	case DL_1_PURGATORY:
		switch (RNG(2)) {
		case 1:  dunName = "levels\\l1data\\reliquary.dun";	break;
		default: dunName = "levels\\l1data\\reliquary2.dun";	break;
		} break;

	case DL_2_DOOM or 3 or 4 or 5 or 6 or 7 or 8:
		switch (RNG(2)) {
		case 1:  dunName = "levels\\l1data\\reliquary.dun";	break;
		default: dunName = "levels\\l1data\\reliquary2.dun";	break;
		} break;

	}
	LoadQuestDungeonAltMapChurch(dunName, 0, 0); 
	LoadQuestDungeonMainMapChurch(dunName, playerRow, playerCol);

	LoadPalette("levels\\l1data\\l1_85.pal");
	AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
	AddWarpFromDisusedReliquary();
}

//!---- (0071B5CB) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_38_GlacialCaverns()
{
	LoadQuestDungeonAltMapCave("levels\\l3data\\glacialcaverns.dun", 0, 0);
	LoadQuestDungeonMainMapCave("levels\\l3data\\glacialcaverns.dun", 0, 0);
	if (Dungeon.prev == DUN_37_DISUSED_RELIQUARY) {
		PlayerRowPos = 63;
		PlayerColPos = 19;
	}
	else {
		PlayerRowPos = 19;
		PlayerColPos = 87;
	}
	LoadPalette("levels\\l3data\\glacial01.pal");
	AddWarpFromGlacialCaverns();
	if (Quests[Q_19_LOST_TREASURE].status == QS_3_COMPLETE) {
		Open_the_Gates_Glacial_Caverns();
	}
}

//!---- (0071B830) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_39_FrozenLoch()
{
	char* dunName;
	switch (RNG(2)) {
	case 1:		dunName = "levels\\l3data\\frozenlake.dun"; break;
	default:	dunName = "levels\\l3data\\frozenlake_v2.dun"; break;
	}
	LoadQuestDungeonAltMapCave(dunName, 0, 0);
	LoadQuestDungeonMainMapCave(dunName, 0, 0);
	PlayerRowPos = 25;
	PlayerColPos = 19;
	LoadPalette("levels\\l3data\\glacial03.pal");
	AddWarpFromFrozenLoch();
	if (Quests[Q_19_LOST_TREASURE].status == QS_3_COMPLETE) {
		Open_the_Gates_Frozen_Loch();
	}
}

//!---- (0071CF00) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_40_Island()
{
	Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];
	if( islandQuest.status == QS_1_ACCEPT ){
		islandQuest.status = QS_2_IN_PROGRESS;
		islandQuest.talkToFlag = 1;//If player enters Island before talking to Farnham, make sure quest log still shows Island quest.
	}
	islandQuest.status3 = 1;

	char* dunName;
	if (MaxCountOfPlayersInGame == 1) {
		switch (Difficulty) {
		case DL_0_HORROR:			
		case DL_1_PURGATORY:
			switch (RNG(5)) {
			case  1: dunName = "levels\\l3data\\island.dun";	break;
			case  2: dunName = "levels\\l3data\\island3.dun";	break;
			case  3: dunName = "levels\\l3data\\island4.dun";	break;
			case  4: dunName = "levels\\l3data\\island5.dun";	break;
			default: dunName = "levels\\l3data\\island2.dun";	break;
			} 
			break;
		case DL_2_DOOM or 3 or 4 or 5 or 6 or 7 or 8:
			switch (RNG(5)) {
			case  1: dunName = "levels\\l3data\\island.dun";	break;
			case  2: dunName = "levels\\l3data\\island3.dun";	break;
			case  3: dunName = "levels\\l3data\\island4.dun";	break;
			case  4: dunName = "levels\\l3data\\island5.dun";	break;
			default: dunName = "levels\\l3data\\island2.dun";	break;
			} 
			break;
		}
	}
	else{ 
		dunName = "levels\\l3data\\island2_mp.dun"; 
	}
	LoadQuestDungeonAltMapCave(dunName, 20, 50);	
	LoadQuestDungeonMainMapCave(dunName, 19, 50);
	LoadPalette("levels\\l3data\\island02.pal");
	AddWarpFromIsland();
	PlayerRowPos = 56;
	PlayerColPos = 56;
}

void ChangeMapTile(int row, int col, uint tile)
{
	SetNewTileNumber(row, col, tile);
	Map[row][col] = tile;
}

void Open_the_Gates_Glacial_Caverns()
{
	ChangeMapTile(13, 31, 12);
	ChangeMapTile(14, 30,  5);
	ChangeMapTile(13, 30,  9);
	ChangeMapTile(14, 31, 13);

	ChangeMapTile(13, 20, 12);
	ChangeMapTile(14, 19,  5);
	ChangeMapTile(13, 19, 14);
	ChangeMapTile(14, 20, 13);

	ChangeMapTile(14, 14, 12);
	ChangeMapTile(15, 13,  5);
	ChangeMapTile(14, 13,  9);
	ChangeMapTile(15, 14, 13);

	ChangeMapTile(17, 7,  5);
	ChangeMapTile(18, 6, 12);
	ChangeMapTile(17, 6, 13);
	ChangeMapTile(18, 7, 14);
	InitDungeonTiles();
}

void Open_the_Gates_Frozen_Loch()
{
	ChangeMapTile( 9, 7,  5);
	ChangeMapTile(10, 6, 12);
	ChangeMapTile( 9, 6, 13);
	ChangeMapTile(10, 7, 14);
	InitDungeonTiles();
}

void Open_the_Gates_Arachnid_Lair()
{
	ChangeMapTile(12, 34, 13);
	ChangeMapTile(13, 34, 12);
	ChangeMapTile(12, 35,  7);
	ChangeMapTile(13, 35,  7);
	ChangeMapTile(12, 36,  5);
	ChangeMapTile(13, 36, 14);
	InitDungeonTiles();
}

void Open_the_Gates_Web_Tunnels()
{
	ChangeMapTile(16, 34, 13);
	ChangeMapTile(17, 34, 12);
	ChangeMapTile(16, 35, 7);
	ChangeMapTile(17, 35, 7);
	ChangeMapTile(16, 36, 5);
	ChangeMapTile(17, 36, 14);

	ChangeMapTile(35, 25,  9);
	ChangeMapTile(36, 25,  7);
	ChangeMapTile(37, 25,  5);
	ChangeMapTile(35, 26, 12);
	ChangeMapTile(36, 26,  7);
	ChangeMapTile(37, 26, 13);
	InitDungeonTiles();
}

void Open_the_Gates_Cellar()
{
	ChangeMapTile( 9, 27, 13);
	ChangeMapTile(10, 27, 10);
	ChangeMapTile(11, 27, 12);
	ChangeMapTile( 9, 28,  5);
	ChangeMapTile(10, 28,  2);
	ChangeMapTile(11, 28, 14);
	InitDungeonTiles();
}

void Open_the_Gates_Sacristy()
{
	ChangeMapTile(19, 18, 2);
	ChangeMapTile(20, 18, 2);
	ChangeMapTile(21, 18, 3);
	ChangeMapTile(19, 19, 13);
	ChangeMapTile(20, 19, 13);
	ChangeMapTile(21, 19, 13);
	ChangeMapTile(19, 20, 21);
	ChangeMapTile(20, 20, 19);
	ChangeMapTile(21, 20, 6);
	InitDungeonTiles();
}

void Open_the_Gates_Fleshdoom_level()
{
	ChangeMapTile(2, 23, 5);
	ChangeMapTile(3, 23, 6);
	ChangeMapTile(4, 23, 6);
	ChangeMapTile(5, 23, 8);
	InitDungeonTiles();
}

void Open_the_Gates_Fleshdoom_level_2()
{
	ChangeMapTile(11, 23, 5);
	ChangeMapTile(12, 23, 6);
	ChangeMapTile(13, 23, 6);
	ChangeMapTile(14, 23, 8);
	InitDungeonTiles();
}

void Open_the_Gates_Temple_of_Sunset()
{
	ChangeMapTile(27, 31, 12);
	ChangeMapTile(28, 31, 12);
	InitDungeonTiles();
}

void Open_the_Gates_Fetid_Cavern()
{
	ChangeMapTile(10,  5, 13);
	ChangeMapTile(11,  5, 12);
	ChangeMapTile(10,  6,  5);
	ChangeMapTile(11,  6, 14);
	InitDungeonTiles();
}

void Open_the_Gates_Springs()
{
	ChangeMapTile(17, 19, 44);
	InitDungeonTiles();
}

void Open_the_Gates_Mossy_Grotto()
{
	ChangeMapTile(15, 18,  7);
	ChangeMapTile(15, 19,  7);
	ChangeMapTile(14, 18, 14);
	ChangeMapTile(14, 19, 12);
	ChangeMapTile(16, 18,  5);
	ChangeMapTile(16, 19, 13);
	ChangeMapTile(15, 17, 22);
	ChangeMapTile(14, 17, 16);
	ChangeMapTile(13, 17, 23);
	InitDungeonTiles();
}

void Open_the_Gates_Blood_Hive()
{
	ChangeMapTile(32, 5, 13);
	ChangeMapTile(33, 5, 12);
	ChangeMapTile(32, 6, 5);
	ChangeMapTile(33, 6, 14);
	InitDungeonTiles();
}

//!---- (0071ED00) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_41_Fleshdoom_1()
{
	Quest& fleshdoomQuest = Quests[Q_21_THE_CURSED_BLADE];
	if( fleshdoomQuest.status == QS_1_ACCEPT ){
		fleshdoomQuest.status = QS_2_IN_PROGRESS;
		fleshdoomQuest.status2 = 1;
		fleshdoomQuest.talkToFlag = 1;
	}
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "levels\\l4data\\FDdun1v2.dun";	break;
	default: dunName = "levels\\l4data\\FDdun1v1.dun";	break;
	}
	LoadQuestDungeonAltMapHell(dunName, 20, 50);
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_42_WIELDER ){
		playerRow = 19;
		playerCol = 83;
	}else{
		playerRow = 19;
		playerCol = 55;
	}
	LoadQuestDungeonMainMapHell(dunName, playerRow, playerCol);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarpFromFleshdoom1();
	// opening up a wall to go back to level 14 quickly
	if (Quests[Q_21_THE_CURSED_BLADE].status2 >= 2) {
		Open_the_Gates_Fleshdoom_level();
	}
}

//!---- (0071EDD0) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_42_Fleshdoom_2()
{
	char* dunName;
	switch (RNG(2)) {
	case 1:  dunName = "levels\\l4data\\FDdun2v1.dun";	break;
	default: dunName = "levels\\l4data\\FDdun2v2.dun";	break;
	}
	LoadQuestDungeonAltMapHell(dunName, 20, 50);
	int playerRow, playerCol;
	if( Dungeon.prev == DUN_41_FLESHDOOM ){
		playerRow = 19;
		playerCol = 55;
	}else{
		playerRow = 19;
		playerCol = 83;
	}
	LoadQuestDungeonMainMapHell(dunName, playerRow, playerCol);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarpFromFleshdoom2();
	// opening up a wall to go back to previous level faster
	if (Quests[Q_21_THE_CURSED_BLADE].status4 >= 1) {
		Open_the_Gates_Fleshdoom_level_2();
	}
}

//!---- (0071EE40) --------------------------------------------------------
/*__forceinline*/ void GenerateQuestFloor_43_Hellforge()
{
	char* dunName;

	if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN)) {

		switch (RNG(4)) {
		case 1:		dunName = "levels\\l4data\\FDdun3v3.DUN";	break;
		case 2:		dunName = "levels\\l4data\\FDdun3v5.DUN";	break;
		case 3:		dunName = "levels\\l4data\\FDdun3v7.DUN";	break;
		default:	dunName = "levels\\l4data\\FDdun3v6.DUN";	break;			
		}
	}
	else{
		switch (Difficulty) {

		case DL_0_HORROR:
			switch (RNG(3)) {
			case  1: dunName = "levels\\l4data\\FDdun3v1.DUN";	break;
			case  2: dunName = "levels\\l4data\\FDdun3v3.DUN";	break;
			default: dunName = "levels\\l4data\\FDdun3v2.DUN";	break;
			}
			break;

		case DL_1_PURGATORY:
		case DL_2_DOOM:
		case DL_3_CRUCIBLE:
			switch (RNG(5)) {
			case  1: dunName = "levels\\l4data\\FDdun3v1.DUN";	break;
			case  2: dunName = "levels\\l4data\\FDdun3v4.DUN";	break;
			case  3: dunName = "levels\\l4data\\FDdun3v3.DUN";	break;
			case  4: dunName = "levels\\l4data\\FDdun3v5.DUN";	break;
			default: dunName = "levels\\l4data\\FDdun3v2.DUN";	break;
			}
			break;

		case DL_4_TORMENT:
		case DL_5_AGONY:
		case DL_6_TERROR:
		case DL_7_DESTRUCTION:
		case DL_8_INFERNO:
			switch (RNG(5)) {
			case  1: dunName = "levels\\l4data\\FDdun3v1.DUN";	break;
			case  2: dunName = "levels\\l4data\\FDdun3v4.DUN";	break;
			case  3: dunName = "levels\\l4data\\FDdun3v6.DUN";	break;
			case  4: dunName = "levels\\l4data\\FDdun3v5.DUN";	break;
			default: dunName = "levels\\l4data\\FDdun3v2.DUN";	break;
			}
			break;

		}
	}
	LoadQuestDungeonAltMapHell(dunName, 20, 50);
	LoadQuestDungeonMainMapHell(dunName, 19, 55);
	LoadPalette("levels\\l4data\\Fleshdoom02.pal");
	AddWarp(17, 55, WM_1029_GOTO_QUEST_DUNGEON, DUN_42_WIELDER);

}

//----- (0046B28C) --------------------------------------------------------
void LoadSetMap()
{
	switch( (DUNGEON)Dungeon ){ // TODO: may be add dungeon generation funcs to Dungeons[]
	case DUN_25_KING_LEORICS_TOMB      : GenerateQuestFloor_25_KingLeoricsTomb();			break;
	case DUN_26_BONE_CHAMBER           : GenerateQuestFloor_26_BoneChamber();				break;
	case DUN_27_THE_INFESTED_CELLAR    : GenerateQuestFloor_27_TheInfestedCellar();			break;
	case DUN_28_POISONED_WATER_SUPPLY  : GenerateQuestFloor_28_PoisonedWaterSupply();		break;
	case DUN_29_ARCHBISHOP_LAZARUS_LAIR: GenerateQuestFloor_29_ArchbishopLazarusLair();		break;
	case DUN_30_HALLS_OF_ANGUISH       : GenerateQuestFloor_30_HallsOfAnguish();			break;
	case DUN_31_ANDARIELS_THRONE       : GenerateQuestFloor_31_AndarielsThrone();			break;
	case DUN_32_HORAZONS_HAVEN         : GenerateQuestFloor_32_HorazonsHaven();				break;
	case DUN_33_SUMMONING_ROOMS        : GenerateQuestFloor_33_SummoningRooms();			break;
	case DUN_34_THE_PIT                : GenerateQuestFloor_34_ThePit();					break;
	case DUN_35_PASSAGE_OF_FIRE        : GenerateQuestFloor_35_PassageOfFire();				break;
	case DUN_36_VALLEY_OF_DESPAIR      : GenerateQuestFloor_36_ValleyOfDespair();			break;
	case DUN_37_DISUSED_RELIQUARY      : GenerateQuestFloor_37_DisusedReliquary();			break;
	case DUN_38_GLACIAL_CAVERNS        : GenerateQuestFloor_38_GlacialCaverns();			break;
	case DUN_39_FROZEN_LOCH            : GenerateQuestFloor_39_FrozenLoch();				break;
	case DUN_40_ISLAND                 : GenerateQuestFloor_40_Island();					break;
	case DUN_41_FLESHDOOM              : GenerateQuestFloor_41_Fleshdoom_1();				break;
	case DUN_42_WIELDER                : GenerateQuestFloor_42_Fleshdoom_2();				break;
	case DUN_43_HEPHASTO               : GenerateQuestFloor_43_Hellforge();					break;
	case DUN_44_RAVENHOLM              : GenerateQuestFloor_44_Ravenholm();					break;
	case DUN_45_THE_DEN                : GenerateQuestFloor_45_TheDen();					break;
	case DUN_46_COLISEUM               : GenerateQuestFloor_46_Coliseum();					break;
	case DUN_47_DARK_NEST			   : GenerateQuestFloor_47_Dark_Nest();					break;
	case DUN_48_BLOOD_HIVE			   : GenerateQuestFloor_48_Blood_Hive();				break;
	case DUN_49_SICK_VILLAGERS_BASEMENT: GenerateQuestFloor_49_Sick_Villagers_Basement();	break;
	case DUN_50_TEMPLE_OF_SUNSET	   : GenerateQuestFloor_50_Temple_of_Sunset();			break;
	case DUN_51_FETID_CAVERN		   : GenerateQuestFloor_51_FetidCavern();				break;
	case DUN_52_MOSSY_GROTTO		   : GenerateQuestFloor_52_Mossy_Grotto();				break;
	case DUN_53_VAULTKEEP			   : GenerateQuestFloor_53_VaultKeep();					break;
	case DUN_54_ABANDONED_FANE		   : GenerateQuestFloor_54_Abandoned_Fane();			break;
	case DUN_55_DEEP_HOLLOW			   : GenerateQuestFloor_55_Deep_Hollow();				break;
	case DUN_56_WEB_TUNNELS			   : GenerateQuestFloor_56_Web_Tunnels();				break;
	case DUN_57_ARACHNID_LAIR		   : GenerateQuestFloor_57_Arachnid_Lair();				break;
	case DUN_58_GATEWAY_OF_BLOOD	   : GenerateQuestFloor_58_Gateway_of_Blood();			break;
	case DUN_59_HALL_OF_FIRE		   : GenerateQuestFloor_59_Hall_of_Fire();				break;
		// AR Church duns
	case DUN_60_HAUNTED_LABYRINTH	   : GenerateQuestFloor_60_Haunted_Labyrinth();			break;
	case DUN_61_CURSED_SANCTUM		   : GenerateQuestFloor_61_Cursed_Sanctum();			break;
	case DUN_62_DIM_UNDERCROFT		   : GenerateQuestFloor_62_Dim_Undercroft();			break; 
	case DUN_63_FORGOTTEN_OSSUARY	   : GenerateQuestFloor_63_Forgotten_Ossuary();			break; 
	case DUN_64_MAUSOLEUM			   : GenerateQuestFloor_64_Mausoleum();					break;
	case DUN_65_RUINED_TEMPLE		   : GenerateQuestFloor_65_Ruined_Temple();				break; 
		// AR Catacomb duns
	case DUN_66_SEWERS				   : GenerateQuestFloor_66_Sewers();					break; 
	case DUN_67_SEPULCHER			   : GenerateQuestFloor_67_Sepulcher();					break;
	case DUN_68_OUBLIETTE			   : GenerateQuestFloor_68_Oubliette();					break;
	case DUN_69_GIBBET				   : GenerateQuestFloor_69_Gibbet();					break; 
	case DUN_70_CITADEL				   : GenerateQuestFloor_70_Citadel();					break;
	case DUN_71_GRIM_VAULTS			   : GenerateQuestFloor_71_Grim_Vaults();				break;
	case DUN_72_NECROPOLIS			   : GenerateQuestFloor_72_Necropolis();				break;
		// AR Cave duns
	case DUN_73_LAVA_PITS			   : GenerateQuestFloor_73_Lava_Pits();					break;
	case DUN_74_CRATER				   : GenerateQuestFloor_74_Crater();					break; 
	case DUN_75_RIVER_OF_FIRE		   : GenerateQuestFloor_75_River_of_Fire();				break;
	case DUN_76_DEN_OF_EVIL			   : GenerateQuestFloor_76_Den_of_Evil();				break;
	case DUN_77_VILE_TUNNELS		   : GenerateQuestFloor_77_Vile_Tunnels();				break;
	case DUN_78_THE_BRIDGE			   : GenerateQuestFloor_78_Bridge();					break;
	case DUN_79_COLONNADE			   : GenerateQuestFloor_79_Colonnade();					break;
	case DUN_80_HELL_GATE			   : GenerateQuestFloor_80_Hell_Gate();					break;
		// AR Hell duns
	case DUN_81_UNDERWORLD			   : GenerateQuestFloor_81_Underworld();				break;
	case DUN_82_HALLS_OF_SUFFERING	   : GenerateQuestFloor_82_Halls_of_Suffering();		break; 
	case DUN_83_BLOOD_KEEP			   : GenerateQuestFloor_83_Blood_Keep();				break;
	case DUN_84_ARMORIES_OF_HELL	   : GenerateQuestFloor_84_Armories_of_Hell();			break;
	case DUN_85_DEMON_STRONGHOLD	   : GenerateQuestFloor_85_Demon_Stronghold();			break;
	case DUN_86_VALLEY_OF_WRATH		   : GenerateQuestFloor_86_Valley_of_Wrath();			break;
	case DUN_87_PANDEMONIUM			   : GenerateQuestFloor_87_Pandemonium();				break;
	case DUN_88_REALM_OF_TERROR		   : GenerateQuestFloor_88_Realm_of_Terror();			break;
	case DUN_89_BEDLAM				   : GenerateQuestFloor_89_Bedlam();					break;
		// AR Abyss duns
	case DUN_90_FIELDS_OF_MADNESS	   : GenerateQuestFloor_90_Fields_of_Madness();			break;
	case DUN_91_STYGIAN_PASSAGE		   : GenerateQuestFloor_91_Stygian_Passage();			break;
	case DUN_92_FLESH_PIT			   : GenerateQuestFloor_92_Flesh_Pit();					break;
	case DUN_93_BLEEDING_VEINS_PATH	   : GenerateQuestFloor_93_Bleeding_Veins_Path();		break;
	case DUN_94_ROT_CHAMBERS		   : GenerateQuestFloor_94_Rot_Chambers();				break;
	case DUN_95_MARROW_GARDEN		   : GenerateQuestFloor_95_Marrow_Garden();				break;
	case DUN_96_BONE_MIRE			   : GenerateQuestFloor_96_Bone_Mire();					break;
	case DUN_97_GLOOMY_DEPTHS		   : GenerateQuestFloor_97_Gloomy_Depths();				break;
	case DUN_98_EBON_CHASM			   : GenerateQuestFloor_98_Black_Chasm();				break;
		// AR Crypt duns
	case DUN_99_ANCIENT_CRYPTS		   : GenerateQuestFloor_99_Ancient_Crypts();			break;
	case DUN_100_HORADRIM_TREASURY	   : GenerateQuestFloor_100_Horadrim_Treasury();		break;
	case DUN_101_VOID_CHAMBERS		   : GenerateQuestFloor_101_Void_Chambers();			break;
	case DUN_102_OBLIVION_TRAIL		   : GenerateQuestFloor_102_Oblivion_Trail();			break;
	case DUN_103_ALTAR_OF_IMMOLATION   : GenerateQuestFloor_103_Altar_of_Immolation();		break;
	case DUN_104_NETHERWORLD_GATES	   : GenerateQuestFloor_104_Netherworld_Gates();		break;
	case DUN_105_DARK_DESCENT		   : GenerateQuestFloor_105_Dark_Descent();				break;
	case DUN_106_BASTION_OF_NIGHT	   : GenerateQuestFloor_106_Bastion_of_Night();			break;
	case DUN_107_DREAD_SANCTUARY	   : GenerateQuestFloor_107_Dread_Sanctuary();			break;
	case DUN_108_ARCANE_PRISON		   : GenerateQuestFloor_108_Arcane_Prison();			break;
		//------- AR end --------------------------------------------------
	case DUN_109_HALL_OF_HEROES		   : GenerateQuestFloor_109_Hall_of_Heroes();			break;
	case DUN_110_BLACK_CHAPEL		   : GenerateQuestFloor_110_Black_Chapel();				break;
	case DUN_111_CHURCH_SACRISTY	   : GenerateQuestFloor_111_Sacristy();					break; 
	case DUN_112_STILL_SPRINGS		   : GenerateQuestFloor_112_StillSprings();				break;
	case DUN_113_THE_MAZE			   : GenerateQuestFloor_113_Maze();						break;
	case DUN_114_LOWER_SCRIPTORIUM	   : GenerateQuestFloor_114_LOWER_SCRIPTORIUM();		break;

	}
}
