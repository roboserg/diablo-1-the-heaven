#include "stdafx.h"

int EntrisMapXxN[239] = {// 004CBA00
	716,	715,	719,	720,	721,	723,	724,	725,	726,	727, 	-1,		0,						// 0
	1171,	1172,	1173,	1174,	1175,	1176,	1177,	1178, 	1179,	1181,	1183,	1185,	-1,		0,		// 12
	1331,	1332,	1333,	1334,	1335,	1336,	1337,	1338,	-1,		0,										// 26
	1307,	1308,	1309,	1310, 	-1,		0,																		// 36
	127,	129,	130,	131,	132,	133,	135,	137, 	138,	139,	140,	-1,						// 42
	106,	107,	108,	109,	110,	112, 	114,	115,	118,	-1,										// 54
	266,	267,	-1,		0,																						// 64
	269,	270, 	271,	272,	-1,		0,																		// 68
	558,	559,	-1,		0,																						// 74
	170,	171,	172,	173,	174,	175,	176,	177,	178,	179,	180,	181,	182,	183,	-1,		0,			// 78
	162,	163,	164,	165,	166,	167,	168,	169,	-1,		0,										// 94
	548,	549,	550,	551,	552,	553, 	554,	555,	556,	557,	558,	559,	560,	-1,		// 104
	82,		83, 	90,		-1,																						// 118
	120,	130,	131,	132,	133,	-1,																		// 122
	421,	422, 	429,	-1,																						// 128
	353,	354,	355,	356,	357,	358,	359,	360, 	361,	362,	363,	364,	365,	366,	367,	368,	369,	370, 	371,	372,	373,	374,	375,	376,	377,	378,	379,	380, 	381,	382,	383,	384,	-1,		0,		// 132
	172,	173,	174,	175, 	176,	177,	178,	179,	184,	-1,										// 166
	149,	150,	151,	152, 	153,	154,	155,	157,	158,	159,	-1,		0,						// 176
	125,	126, 	129,	131,	132,	135,	136,	140,	142,	-1,										// 188
	79,		80,		81,		82,		83,		84,		85,		86,		87,		88,		89,		90, 	91,		92,		-1,		0,			// 198
	65,		66,		67,		68,		69,		70, 	71,		72,		73,		74,		75,		76,		77,		78,		-1,		0,			// 214
	57,		58,		59,		60,		61,		62,		63,		64,		-1												// 230
};

int TownDoorOpen[DD_COUNT];
int IswarpUnderCursor;// 0070021C
vector<Warp> Warps(8);// 00700228

uchar ClvlReqsForEnterInDungeon [9][6] = {// 00A09900
	0, 8, 13, 17, 19, 20,
	0, 23, 25, 27, 29, 31,
	0, 34, 35, 36, 37, 38,
	0, 39, 41, 43, 45, 47,
	0, 49, 51, 53, 55, 57,
	0, 59, 60, 61, 62, 64,
	0, 67, 69, 71, 72, 73,
	0, 75, 76, 77, 78, 80,
	0, 83, 84, 85, 86, 87
};

// this is onscreen message indexes
char ClvlNotReqForEnterInDungeonMessagesIndexesTable [9][6] = {// 00A09920
	0,  40,  41,  42,  55,  56,
	0,  57,  58,  59,  60,  61,
	0,  62,  63,  64,  65,  66,
	0,  80,  81,  82,  83,  84,
	0,  85,  86,  87,  88,  89,
	0,  90,  91,  92,  93,  94,
	0,  95,  96,  97,  98,  99,
	0, 100, 101, 102, 103, 104,
	0, 105, 106, 107, 108, 109
};

// новая функция. Оптимизация для функций отображения надписи при наведении
bool SetCursorToWarp(int gameMessage)
{
	for( auto& warp: Warps ){
		if( warp.GameMessage == gameMessage && abs(int(Cur.Row - warp.Row)) < 4 && abs(int(Cur.Col - warp.Col)) < 4 ){
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (00477B87) --------------------------------------------------------
void ClearWarps()
{
	Warps.clear();
	IswarpUnderCursor = false;
}

//----- (00477B96) --------------------------------------------------------
void AddWarpsToTown()
{
	static Warp townDoor[DD_COUNT] = {
		{25, 29, WM_1026_NEXT_DUNGEON, DUN_1_CHURCH_1  }, // DD_0_CHURCH
		{49, 21, WM_1031_GOTO_DUNGEON, DUN_5_CATACOMB_1}, // DD_1_CATACOMB
		{17, 69, WM_1031_GOTO_DUNGEON, DUN_9_CAVE_1    }, // DD_2_CAVE
		{41, 80, WM_1031_GOTO_DUNGEON, DUN_13_HELL_1   }, // DD_3_HELL
		{80, 62, WM_1031_GOTO_DUNGEON, DUN_17_ABYSS_1  }, // DD_4_ABYSS
		{36, 24, WM_1031_GOTO_DUNGEON, DUN_21_CRYPT_1  }, // DD_5_CRYPT
	};
	int ddCount = GameMode == GM_CLASSIC ? DD_COUNT - 2 : DD_COUNT;
	for( int door = 0; door < ddCount; ++door ){
		TownDoorOpen[door] = false;
	}
	Warps.push_back(townDoor[DD_0_CHURCH]);
	TownDoorOpen[DD_0_CHURCH] = true;
	
	if( not(GameMode, GM_NIGHTMARE, GM_SPEEDRUN) ){
		Player& player = Players[CurrentPlayerIndex];
		auto& clvlReq = ClvlReqsForEnterInDungeon[ Difficulty ];
		for( int door = DD_1_CATACOMB; door < ddCount; ++door ){
			if( MaxCountOfPlayersInGame > 1 || player.CharLevel >= clvlReq[door] || player.OpenedTownDoorMask & BIT(door) ){
				Warps.push_back(townDoor[door]);
				TownDoorOpen[door] = true;
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void AddWarp(int row, int col, int gameMessage, DUNGEON destDungeon /*= DUN_NO*/)
{
	if( destDungeon == DUN_NO ){
		switch( gameMessage ){
		case WM_1026_NEXT_DUNGEON             : destDungeon = Dungeon + 1;       break;
		case WM_1027_PREV_DUNGEON             : destDungeon = Dungeon - 1;	     break;
		case WM_1028_RETURN_FROM_QUEST_DUNGEON: destDungeon = Dungeon.returnLvl; break;
		default                               : destDungeon = DUN_0_TOWN;        break;
		}
	}
	Warps.push_back({row, col, gameMessage, destDungeon});
}

//----- (00477DE4) --------------------------------------------------------
void AddWarpsToChurchTypeLabyrinth()
{
	for( int col = 0; col < 112; col++ ) for( int row = 0; row < 112; row++ ){
		int fineMapTile = FineMap[ row ][ col ];
		if( Dungeon->graphType >= DT_5_CRYPT ){
			switch( fineMapTile ){
			case 184:	
				if( not(GameMode, GM_NIGHTMARE, GM_SPEEDRUN) ){ 
					AddWarp(row, col, WM_1032_RETURN_TO_TOWN, DUN_0_TOWN);
				} 
				break;
			case 158: AddWarp(row, col, WM_1027_PREV_DUNGEON);	break;
			case 317: 
				if( Quest& a = Quests[Q_32_A_SIMPLE_TASK]; IsQuestOnLevel(Q_32_A_SIMPLE_TASK) && row == a.row && col == a.col && Dungeon == DUN_21_CRYPT_1){
					AddWarp(row, col, WM_1029_GOTO_QUEST_DUNGEON, DUN_54_ABANDONED_FANE);
				}
				break;
			case 126:
				if( Quest &h = Quests[Q_16_HORAZON_S_DEMONS], &v = Quests[Q_28_VISIONS_OF_VICTORY];
				 ( ! IsQuestOnLevel(Q_16_HORAZON_S_DEMONS)   || row != h.row || col != h.col) &&
				 ( ! IsQuestOnLevel(Q_28_VISIONS_OF_VICTORY) || row != v.row || col != v.col) ){
					AddWarp(row, col, WM_1026_NEXT_DUNGEON);	
				}
				break;
			}
		}else{
			switch( fineMapTile ){
			case 130: AddWarp(row, col, WM_1027_PREV_DUNGEON); break;
			case 115: AddWarp(row, col, WM_1026_NEXT_DUNGEON); break;
			}
		}
	}
}

//----- (00477F7C) --------------------------------------------------------
void AddWarpsToCatacombTypeLabyrinth()
{
	for( int col = 0; col < 112; col++ ) for( int row = 0; row < 112; row++ ){
		int fineMapTile = FineMap[ row ][ col ];
		switch (fineMapTile ){
		case 267:
			if( Quest& c = Quests[Q_14_THE_CHAMBER_OF_BONE]; ! IsQuestOnLevel(Q_14_THE_CHAMBER_OF_BONE) || row != c.row || col != c.col){
				AddWarp(row, col, WM_1027_PREV_DUNGEON);	
			}
			break;
		case 559:
			if( not(GameMode, GM_NIGHTMARE, GM_SPEEDRUN) ){ 
				AddWarp(row, col, WM_1032_RETURN_TO_TOWN, DUN_0_TOWN); 
			} 
			break;
		case 271: 
			if( Quest& g = Quests[Q_31_GOOD_DEAL]; IsQuestOnLevel(Q_31_GOOD_DEAL) && row == g.row && col == g.col) { 
				AddWarp(row, col, WM_1029_GOTO_QUEST_DUNGEON, DUN_53_VAULTKEEP);
			}
			else if( Quest& v = Quests[Q_9_VALOR]; IsQuestOnLevel(Q_9_VALOR) && row == v.row && col == v.col) {
				AddWarp(row, col, WM_1029_GOTO_QUEST_DUNGEON, DUN_58_GATEWAY_OF_BLOOD);
			}
			else{
				AddWarp(row, col, WM_1026_NEXT_DUNGEON); 
			}
			break;
		}
	}
}

//----- (00478084) --------------------------------------------------------
void AddWarpsToCaveTypeLabyrinth()
{
	int nextCount = 0;
	if( Dungeon->graphType >= DT_5_CRYPT ){
		for( int col = 0; col < 112; col++ ) for( int row = 0; row < 112; row++ ){
			switch( FineMap[row][col] ){
			case 66: AddWarp(row, col, WM_1027_PREV_DUNGEON); break;
			case 63: if( ! SetQuestPatternWarp(row, col) ) AddWarp(row, col, WM_1026_NEXT_DUNGEON); break;
			case 80: AddWarp(row, col, WM_1032_RETURN_TO_TOWN); break;
			}
		}
	}else{
		for( int col = 0; col < 112; col++ ) for( int row = 0; row < 112; row++ ){
			switch( FineMap[row][col] ){
			case 171: AddWarp(row, col, WM_1027_PREV_DUNGEON);	break;
			case 168: if( ! SetQuestPatternWarp(row, col) ) AddWarp(row, col, WM_1026_NEXT_DUNGEON); break;
			case 549: if( not(GameMode, GM_NIGHTMARE, GM_SPEEDRUN) ) AddWarp(row, col, WM_1032_RETURN_TO_TOWN); break;
			}
		}
	}
}

//----- (0047822C) --------------------------------------------------------
void AddWarpsToHellTypeLabyrinth()
{
	for( int col = 0; col < FineMap_112; col++ ) for( int row = 0; row < FineMap_112; row++ ){
		switch( FineMap[row][col] ){
		case  83: AddWarp(row, col, WM_1027_PREV_DUNGEON); break;
		case 422: if( not(GameMode, GM_NIGHTMARE, GM_SPEEDRUN) ) AddWarp(row, col, WM_1032_RETURN_TO_TOWN, DUN_0_TOWN); break;
		case 120: if( ! SetQuestPatternWarp(row, col) ) AddWarp(row, col, WM_1026_NEXT_DUNGEON); break;
		}
	}
	// TODO: looks like splited adding is not needed anymore
	// ? this will ensure that the first trigger is not the trigger down to 16
	for( int col = 0; col < FineMap_112; col++ ) for( int row = 0; row < FineMap_112; row++ ){
		if( FineMap[row][col] == 370 && Quests[Q_15_ARCHBISHOP_LAZARUS].status == QS_3_COMPLETE ){
			AddWarp(row, col, WM_1026_NEXT_DUNGEON);
		}
	}
}

//----- (00478371) --------------------------------------------------------
void AddWarpFromSceletonKing()
{
	AddWarp(83, 42, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_3_CHURCH_3);
}

//----- (004783D1) --------------------------------------------------------
void AddWarpFromPoisonWell()
{
	AddWarp(30, 83, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_2_CHURCH_2);
}

void AddWarpFromFetidCavern()
{
	AddWarp(33, 54, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_12_CAVE_4);
	if( is(GameMode, GM_IRONMAN, GM_SURVIVAL) ){
		AddWarp(41, 20, WM_1029_GOTO_QUEST_DUNGEON, DUN_27_THE_INFESTED_CELLAR);
	}
}

void AddWarpFrom_BloodHive()
{
	AddWarp(88, 19, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_20_ABYSS_4);
}

//----- (00478401) --------------------------------------------------------
void AddWarpFromUnholyAltar()
{
	AddWarp(35, 32, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_15_HELL_3);
}

//----- (00715B40) --------------------------------------------------------
void AddWarpFromHallsOfAnguish()
{
	if( is(GameMode, GM_IRONMAN, GM_SURVIVAL) ){
		AddWarp(89, 88, WM_1029_GOTO_QUEST_DUNGEON, DUN_54_ABANDONED_FANE);
	}else{
		AddWarp(89, 88, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_0_TOWN); // Will this lead to map morph upon re-entry???
	}
}

//----- (0071A450) -------------------------------------------------------
void __fastcall AddWarpFromFirePassage(int isPortalOpen)
{
	AddWarp(77, 40, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_19_ABYSS_3);
	if( isPortalOpen ){
		AddWarp(27, 40, WM_1029_GOTO_QUEST_DUNGEON, DUN_36_VALLEY_OF_DESPAIR);
	}
}

//----- (0071A700) -------------------------------------------------------
void AddWarpFromIzualValey()
{
	AddWarp(18, 88, WM_1029_GOTO_QUEST_DUNGEON, DUN_35_PASSAGE_OF_FIRE);
}

//----- (0071B720) -------------------------------------------------------
void AddWarpFromDisusedReliquary()
{
	if( is(GameMode, GM_IRONMAN, GM_SURVIVAL) ){
		AddWarp(25, 86, WM_1029_GOTO_QUEST_DUNGEON, DUN_52_MOSSY_GROTTO);
	}else{
		AddWarp(25, 86, WM_1028_RETURN_FROM_QUEST_DUNGEON);
	}
	AddWarp(23, 18, WM_1029_GOTO_QUEST_DUNGEON, DUN_38_GLACIAL_CAVERNS);
}

//----- (0071B65F) -------------------------------------------------------
void AddWarpFromGlacialCaverns()
{
	AddWarp(63, 18, WM_1029_GOTO_QUEST_DUNGEON, DUN_37_DISUSED_RELIQUARY);
	AddWarp(18, 87, WM_1029_GOTO_QUEST_DUNGEON, DUN_39_FROZEN_LOCH);
}

//----- (0071B7E0) -------------------------------------------------------
void AddWarpFromFrozenLoch()
{
	AddWarp(25, 18, WM_1029_GOTO_QUEST_DUNGEON, DUN_38_GLACIAL_CAVERNS);
}

//----- (0071D000) -------------------------------------------------------
void AddWarpFromIsland()
{
	AddWarp(55, 55, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_11_CAVE_3);
}

//----- (0071EB90) -------------------------------------------------------
void AddWarpFromFleshdoom1()
{
	AddWarp(17, 55, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_14_HELL_2);
	AddWarp(17, 83, WM_1029_GOTO_QUEST_DUNGEON, DUN_42_WIELDER);
}

//----- (0071EC00) -------------------------------------------------------
void AddWarpFromFleshdoom2()
{
	AddWarp(17, 55, WM_1029_GOTO_QUEST_DUNGEON, DUN_41_FLESHDOOM);
	AddWarp(17, 83, WM_1029_GOTO_QUEST_DUNGEON, DUN_43_HEPHASTO);
}

//----- (th2) -------------------------------------------------------------
void AddWarpFromTheDen()
{
	AddWarp(56, 57, WM_1029_GOTO_QUEST_DUNGEON, DUN_44_RAVENHOLM);
}

//----- (th2) -------------------------------------------------------------
void AddWarpFromRavenholm()
{
	AddWarp(83, 55, WM_1029_GOTO_QUEST_DUNGEON, DUN_45_THE_DEN);
	AddWarp(93, 89, WM_1032_RETURN_TO_TOWN, DUN_0_TOWN);
} 

//----- (th3) -------------------------------------------------------------
void AddWarpFromTheColiseum()
{
	AddWarp(60, 60, WM_1028_RETURN_FROM_QUEST_DUNGEON, DUN_0_TOWN);
}

//----- (00478431) --------------------------------------------------------
bool ShowDungeonEntryDescriptionsInTown()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_26_CRYPT_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			int row = Cur.Row;
			int col = Cur.Col;
			if( abs(row - 36) < 4 && abs(col - 24) < 4 ){
				if( GameMode != GM_CLASSIC ){
					strcpy(InfoPanelHeader, "Down to Crypt");
					Cur.Row = 36;
					Cur.Col = 24;
				}
				return true;
			}else{
				strcpy(InfoPanelHeader, "to Disused Reliquary");
				InfoPanel_AddLine("(Danger: high)", true);
				Cur.Row = 20;
				Cur.Col = 33;
				return true;
			}
		}
	}
	if( GameMode != GM_CLASSIC ){
		for( int enterisMapIndex = DEI_36_ABYSS_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Down to Abyss");
				Cur.Row = 80;
				Cur.Col = 62;
				return true;
			}
		}
	}
	for( int enterisMapIndex = DEI_0_CHURCH_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			strcpy(InfoPanelHeader, "Down to Church");
			Cur.Row = 25;
			Cur.Col = 29;
			return true;
		}
	}
	if( TownDoorOpen[DD_1_CATACOMB] ){
		for( int enterisMapIndex = DEI_12_CATACOMBS_ENTER_FROM_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Down to catacombs");
				Cur.Row = 49;
				Cur.Col = 21;
				return true;
			}
		}
	}
	if( TownDoorOpen[DD_2_CAVE] ){
		for( int dungeonMapIndex = 1199; dungeonMapIndex < 1221; dungeonMapIndex++ ){
			if( cursorFineMapTile == dungeonMapIndex ){
				strcpy(InfoPanelHeader, "Down to caves");
				Cur.Row = 17;
				Cur.Col = 69;
				return true;
			}
		}
	}
	if( TownDoorOpen[DD_3_HELL] ){
		for( int dungeonMapIndex = 1240; dungeonMapIndex < 1256; dungeonMapIndex++ ){
			if( cursorFineMapTile == dungeonMapIndex ){
				strcpy(InfoPanelHeader, "Down to hell");
				Cur.Row = 41;
				Cur.Col = 80;
				return true;
			}
		}
	}	
	
	for (int dungeonMapIndex = 1134; dungeonMapIndex < 1152; dungeonMapIndex++) {
		if (cursorFineMapTile == dungeonMapIndex) {
			strcpy(InfoPanelHeader, "to the Infested Cellar");
			InfoPanel_AddLine("(Danger: low)", true);
			Cur.Row = 73;
			Cur.Col = 80;
			return true;
		}
	}

	for( int dungeonMapIndex = 249; dungeonMapIndex < 253; dungeonMapIndex++ ){
		if( cursorFineMapTile == dungeonMapIndex ){
			strcpy(InfoPanelHeader, "to the Halls of Anguish");
			InfoPanel_AddLine("(Danger: high)", true);
			Cur.Row = 80;
			Cur.Col = 14;
			return true;
		}
	}

	return false;
}

//----- (007169FB) --------------------------------------------------------
bool __fastcall IsValidWarpNearCursor(int gameMessage)
{
	for( auto& warp: Warps ){
		if( warp.GameMessage == gameMessage ){
			if( abs(int(Cur.Row - warp.Row)) < 4 
			 && abs(int(Cur.Col - warp.Col)) < 4 ){
				return true;
			}
		}
	}
	return false;
}

//----- (00478651) --------------------------------------------------------
bool ShowDungeonEntryDescriptionsInChurchAndCrypt()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	if( Dungeon->graphType < DT_5_CRYPT ){
		for( int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				if( Dungeon == DUN_1_CHURCH_1 ){
					strcpy(InfoPanelHeader, "Up to town");
				}else{
					sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
				}
				if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
					return true;
				}
			}
		}

		for( int enterisMapIndex = DEI_54_CHURCH_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
				if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
		return false;
	}

	for( int enterisMapIndex = DEI_176_CRYPT_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
			if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_188_CRYPT_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			if( IsValidWarpNearCursor(WM_1026_NEXT_DUNGEON) ){// для хоразон квеста обертка
				sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
				if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
	}

	if( Dungeon == DUN_21_CRYPT_1 && (GameMode != GM_NIGHTMARE && GameMode != GM_SPEEDRUN)){
		for( int enterisMapIndex = DEI_166_CRYPT_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(WM_1032_RETURN_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;	
}

//----- (0047895F) --------------------------------------------------------
bool ShowDungeonEntryDescriptionsCatacombs()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	for( int enterisMapIndex = DEI_64_CATACOMB_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
			if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_68_CATACOMB_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
			if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
				return true;
			}
		}
	}
	if( Dungeon == DUN_5_CATACOMB_1 && (GameMode != GM_NIGHTMARE && GameMode != GM_SPEEDRUN)){
		for( int enterisMapIndex = DEI_74_CATACOMB_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(WM_1032_RETURN_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;
}

//----- (00478B65) --------------------------------------------------------
int ShowDungeonEntryDescriptions_Caves_And_Abyss()
{
	Cell cursorFineMapTileP( FineMap, Cur.Row, Cur.Col);
	int cursorFineMapTile = cursorFineMapTileP(0, 0);
	if( Dungeon->graphType < DT_5_CRYPT ){

		for( int enterisMapIndex = DEI_78_CAVE_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
				if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
					return true;
				}
			}
		}

		for (int enterisMapIndex = DEI_94_CAVE_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
			int entrisMap = EntrisMapXxN[enterisMapIndex];
			if (cursorFineMapTileP(0, 0) == entrisMap ||
				cursorFineMapTileP(1, 0) == entrisMap ||
				cursorFineMapTileP(2, 0) == entrisMap) {
				sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
				if (SetCursorToWarp(WM_1026_NEXT_DUNGEON)) {
					return true;
				}
			}
		}
		if( Dungeon == DUN_9_CAVE_1 ){
			for( int enterisMapIndex = DEI_104_CAVE_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
				if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
					strcpy(InfoPanelHeader, "Up to town");
					if( SetCursorToWarp(WM_1032_RETURN_TO_TOWN) ){
						return true;
					}
				}
			}
		}
		return false;
	}

	for( int enterisMapIndex = DEI_214_ABYSS_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
			if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_230_ABYSS_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		int entrisMap = EntrisMapXxN[enterisMapIndex];
		if( cursorFineMapTileP(0, 0) == entrisMap ||
			cursorFineMapTileP(1, 0) == entrisMap || 
			cursorFineMapTileP(2, 0) == entrisMap ){
				sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
				if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
					return true;
				}
		}
	}
		
	if( Dungeon == DUN_17_ABYSS_1 ){
		for( int enterisMapIndex = DEI_198_ABYSS_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(WM_1032_RETURN_TO_TOWN) ){
					return true;
				}
			}
		}
	}
	return false;
}

//----- (00478EE9) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Hell()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];

	for( int enterisMapIndex = DEI_118_HELL_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Up to level %i", Dungeon - 1);
			if( SetCursorToWarp(WM_1027_PREV_DUNGEON) ){
				return true;
			}
		}
	}

	for( int enterisMapIndex = DEI_122_HELL_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Down to level %i", Dungeon + 1);
			if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
				return true;
			}
		}
	}

	if( Dungeon == DUN_13_HELL_1 && (GameMode != GM_NIGHTMARE && GameMode != GM_SPEEDRUN)){
		for( int enterisMapIndex = DEI_128_HELL_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Up to town");
				if( SetCursorToWarp(WM_1032_RETURN_TO_TOWN) ){
					return true;
				}
			}
		}
	}

	if( Dungeon == DUN_15_HELL_3 ){
		for( int enterisMapIndex = DEI_132_HELL_PANDEMONIUM_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
			if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
				strcpy(InfoPanelHeader, "Down to Pandemonium");
				if( SetCursorToWarp(WM_1026_NEXT_DUNGEON) ){
					return true;
				}
			}
		}
	}
	return false;
}

//----- (0047912E) --------------------------------------------------------
void ClearAreasAroundWarps()
{
	for( auto& warp: Warps ){
		for( int rowOffset = -2; rowOffset <= 2; rowOffset++ ){
			for( int colOffset = -2; colOffset <= 2; colOffset++ ){
				int col = warp.Col + colOffset;
				int row = warp.Row + rowOffset;
				FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
			}
		}
	}
}

//----- (00479176) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_LeoricTomb()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonStart);
			if( SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_BlackChapel()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_34_GRAVE_MATTERS].dungeonStart);
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Springs()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_78_CAVE_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_36_CLEAR_WATER].dungeonStart);
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Maze()
{
	for (auto& warp : Warps) {
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if (warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON) {
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}
			/*else {
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}*/
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Lower_Scriptorium()
{
	for (auto& warp : Warps) {
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if (warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON) {
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}
			/*else {
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}*/
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;	
	
	/*int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_54_CHURCH_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_38_OGDENS_SIGN_MP].dungeonStart);
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;*/
}
bool ShowDungeonEntryDescriptions_Sacristy()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_42_CHURCH_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_35_IRON_OATH].dungeonStart);
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

//----- (004791DF) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_HallOfHeroes()
{
	for (auto& warp : Warps) {
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if (warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON) {
				sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_14_THE_CHAMBER_OF_BONE].dungeonStart);
			}
			else {
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_ChamberOfBone()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_68_CATACOMB_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Hall of Heroes");
			if (SetCursorToWarp(WM_1029_GOTO_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_VaultKeep()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col]; 
	for (int enterisMapIndex = DEI_64_CATACOMB_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) { 
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) { 
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_31_GOOD_DEAL].dungeonStart);  
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) { 
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Valor()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_64_CATACOMB_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_9_VALOR].dungeonStart);
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

//----- (00715000) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_InfestedCellar()
{
	for( auto& warp: Warps ){
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				if( warp.destDungeon == DUN_0_TOWN ){
					sprintf(InfoPanelHeader, "Back to town");
				}else{
					sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
				}
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Basement()
{
	int cursorFineMapTile = FineMap[Cur.Row][Cur.Col];
	for (int enterisMapIndex = DEI_78_CAVE_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++) {
		if (cursorFineMapTile == EntrisMapXxN[enterisMapIndex]) {
			sprintf(InfoPanelHeader, "Up to town");
			if (SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON)) {
				return true;
			}
		}
	}
	return false;
}

//----- (00479248) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_PoisonedWaterSupply()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_94_CAVE_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "Back to Level %i", Quests[Q_13_POISONED_WATER_SUPPLY].dungeonStart);
			if( SetCursorToWarp(WM_1028_RETURN_FROM_QUEST_DUNGEON) ){
				return true;
			}
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Alternatives()
{
	for (auto& warp : Warps) {
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if (warp.destDungeon == Dungeon - 1 ) {
				sprintf(InfoPanelHeader, "Back To %s", Dungeons[warp.destDungeon].warpName);
			}
			else {
				sprintf(InfoPanelHeader, "down To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Alternative_Route()
{
	for (auto& warp : Warps) {
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if (warp.destDungeon == DUN_60_HAUNTED_LABYRINTH) {
				sprintf(InfoPanelHeader, "to Haunted Labyrinth");
				InfoPanel_AddLine("(Enter alternative route)", true);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_FetidCavern()
{
	for( auto& warp: Warps ){
		if (IsCursorNearCoordinate(warp.Row, warp.Col)) {
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Mossy_Grotto()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (00715C80) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_AndarielHalls()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_128_HELL_TOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			if (is(GameMode, GM_IRONMAN, GM_SURVIVAL)) {
				sprintf(InfoPanelHeader, "Back to Abandoned Fane");
			}
			else {
				sprintf(InfoPanelHeader, "Up to town");
			}
			Cur.Row = 89;
			Cur.Col = 88;
			return true;
		}
	}
	for( int enterisMapIndex = DEI_122_HELL_DOWN; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "to Andariel's Throne");
			Cur.Row = 88;
			Cur.Col = 27;
			return true;
		}
	}
	return false;
}

//----- (00715DC0) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_AndarielThrone()
{
	int cursorFineMapTile = FineMap[ Cur.Row ][ Cur.Col ];
	for( int enterisMapIndex = DEI_118_HELL_UP; EntrisMapXxN[enterisMapIndex] != -1; enterisMapIndex++ ){
		if( cursorFineMapTile == EntrisMapXxN[enterisMapIndex] ){
			sprintf(InfoPanelHeader, "to the Halls of Anguish");
			Cur.Row = 49;
			Cur.Col = 26;
			return true;
		}
	}
	return false;
}

//----- (00717400) --------------------------------------------------------
bool __fastcall IsCursorNearCoordinate (int row, int col)
{
	for( int deltaRangeIndex = 0; deltaRangeIndex < 7; deltaRangeIndex++ ){
		if( row + MapDeltaRangeRow[deltaRangeIndex] == Cur.Row 
			&& col + MapDeltaRangeCol[deltaRangeIndex] == Cur.Col ){
				return true;
		}
	}
	return false;
}

//----- (00717440) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Horazon()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (0071A500) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Izual()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_DarkNest()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Deep_Hollow()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_BloodHive()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Web_Tunnels()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Gateway_of_Blood()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Hall_of_Fire()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_Arachnid_Lair()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_TempleOfSunset()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

bool ShowDungeonEntryDescriptions_AbandonedFane()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (0071B890) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Treasure()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Up to town");
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (0071D400) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Island()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (0071EE90) --------------------------------------------------------
bool ShowDungeonEntryDescriptions_Fleshdoom()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to Level %i", warp.destDungeon);
			}else{
				sprintf(InfoPanelHeader, "To %s", Dungeons[warp.destDungeon].warpName);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
bool ShowDungeonEntryDescriptions_TheRampagingDemon()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate (warp.Row, warp.Col) ){
			if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				sprintf(InfoPanelHeader, "Back to town");
			}else if( warp.destDungeon == DUN_44_RAVENHOLM ){
				sprintf(InfoPanelHeader, "Back to Ravenholm");
			}else if( warp.destDungeon == DUN_45_THE_DEN ){
				sprintf(InfoPanelHeader, "To the Den");
				InfoPanel_AddLine("(Danger: very high)", true);
			}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
bool ShowDungeonEntryDescriptions_Coliseum()
{
	for( auto& warp: Warps ){
		if( IsCursorNearCoordinate(warp.Row, warp.Col) ){
			//if (warp.GameMessage == GM_1028_GOING_FROM_QUEST_DUNGEON) {
			//	sprintf(InfoPanelHeader, "Back to town");
			//}
			Cur.Row = warp.Row;
			Cur.Col = warp.Col;
			return true;
		}
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
bool __fastcall IsCursorNearWarp(Warp& warp)
{
	static char ofsRow[8] = { 0,-1, 0,-1,-2,-1,-2 }; // TODO: save pattern coord list to Warp struct in AddWarp
	static char osfCol[8] = { 0, 0,-1,-1,-1,-2,-2 };

	for( int offset = 0; offset < countof(ofsRow); offset++ ){
		if( warp.Row + ofsRow[offset] == Cur.Row && warp.Col + osfCol[offset] == Cur.Col ) return true;
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
bool ShowRemainWarpDescriptions()
{
	Warp* nearWarp = 0;
	for( auto& warp: Warps ){
		if( IsCursorNearWarp(warp) ){
			nearWarp = &warp;
			break;
		}
	}
	if( nearWarp ){

		// here you can limit the list of processed warps
		// TODO: remove limits after entry pattern coord list saving in Warp struct
		switch( nearWarp->destDungeon ){
		case DUN_60_HAUNTED_LABYRINTH: break;
		default: return false;
		}

		char* info = "to %s";
		switch( nearWarp->GameMessage ){
		case WM_1026_NEXT_DUNGEON             : info = "down to %s"; break;
		case WM_1027_PREV_DUNGEON             : info = "up to %s";   break;
		case WM_1028_RETURN_FROM_QUEST_DUNGEON: info = "back to %s"; break;
		}
		sprintf(InfoPanelHeader, info, Dungeons[nearWarp->destDungeon].warpName);
		return true;
	}
	return false;
}

//----- (004792B1) --------------------------------------------------------
void ShowDungeonEntryDescriptions()
{
	IswarpUnderCursor = 0;
	InfoPanel_ClearBody();
	if( CursorY > 351 && ScreenHeight == GUI_Height ) return;
    
    if( !Dungeon->isQuest ){
        switch( Dungeon->genType ){
        case DT_0_TOWN    : IswarpUnderCursor = ShowDungeonEntryDescriptionsInTown();           break;
        case DT_1_CHURCH  : IswarpUnderCursor = ShowDungeonEntryDescriptionsInChurchAndCrypt(); break;
        case DT_2_CATACOMB: IswarpUnderCursor = ShowDungeonEntryDescriptionsCatacombs();        break;
        case DT_3_CAVE    : IswarpUnderCursor = ShowDungeonEntryDescriptions_Caves_And_Abyss(); break;
        case DT_4_HELL    : IswarpUnderCursor = ShowDungeonEntryDescriptions_Hell();            break;
        }
		if (Dungeon == DUN_1_CHURCH_1 && MaxCountOfPlayersInGame == 1) { // mor: my try to show warp to dun 60
		IswarpUnderCursor = ShowDungeonEntryDescriptions_Alternative_Route();
		}
        if( !IswarpUnderCursor ) IswarpUnderCursor = ShowDungeonEntryDescriptionsFromQuestLocEntries();
    }else{
        switch( (DUNGEON)Dungeon ){
        case DUN_25_KING_LEORICS_TOMB      : IswarpUnderCursor = ShowDungeonEntryDescriptions_LeoricTomb();          break;
        case DUN_109_HALL_OF_HEROES        : IswarpUnderCursor = ShowDungeonEntryDescriptions_HallOfHeroes();        break;
		case DUN_26_BONE_CHAMBER		   : IswarpUnderCursor = ShowDungeonEntryDescriptions_ChamberOfBone();        break;
		case DUN_53_VAULTKEEP              : IswarpUnderCursor = ShowDungeonEntryDescriptions_VaultKeep();           break;
		case DUN_27_THE_INFESTED_CELLAR    : IswarpUnderCursor = ShowDungeonEntryDescriptions_InfestedCellar();      break;
		case DUN_49_SICK_VILLAGERS_BASEMENT: IswarpUnderCursor = ShowDungeonEntryDescriptions_Basement();            break;
		case DUN_28_POISONED_WATER_SUPPLY  : IswarpUnderCursor = ShowDungeonEntryDescriptions_PoisonedWaterSupply(); break;
		case DUN_30_HALLS_OF_ANGUISH       : IswarpUnderCursor = ShowDungeonEntryDescriptions_AndarielHalls();       break;
        case DUN_31_ANDARIELS_THRONE       : IswarpUnderCursor = ShowDungeonEntryDescriptions_AndarielThrone();      break;
        case DUN_32_HORAZONS_HAVEN         :
        case DUN_33_SUMMONING_ROOMS        :
        case DUN_34_THE_PIT                : IswarpUnderCursor = ShowDungeonEntryDescriptions_Horazon();             break;
        case DUN_35_PASSAGE_OF_FIRE        :
        case DUN_36_VALLEY_OF_DESPAIR      : IswarpUnderCursor = ShowDungeonEntryDescriptions_Izual();               break;
        case DUN_37_DISUSED_RELIQUARY      :
        case DUN_38_GLACIAL_CAVERNS        :
        case DUN_39_FROZEN_LOCH            : IswarpUnderCursor = ShowDungeonEntryDescriptions_Treasure();            break;
        case DUN_40_ISLAND                 : IswarpUnderCursor = ShowDungeonEntryDescriptions_Island();              break;
        case DUN_41_FLESHDOOM              :
        case DUN_42_WIELDER                :
        case DUN_43_HEPHASTO               : IswarpUnderCursor = ShowDungeonEntryDescriptions_Fleshdoom();           break;
        case DUN_44_RAVENHOLM              :
        case DUN_45_THE_DEN                : IswarpUnderCursor = ShowDungeonEntryDescriptions_TheRampagingDemon();   break;
		case DUN_46_COLISEUM               : IswarpUnderCursor = ShowDungeonEntryDescriptions_Coliseum();            break;
		case DUN_47_DARK_NEST              : IswarpUnderCursor = ShowDungeonEntryDescriptions_DarkNest();            break;
		case DUN_55_DEEP_HOLLOW            : IswarpUnderCursor = ShowDungeonEntryDescriptions_Deep_Hollow();         break;
		case DUN_48_BLOOD_HIVE             : IswarpUnderCursor = ShowDungeonEntryDescriptions_BloodHive();           break; 
		case DUN_56_WEB_TUNNELS            : IswarpUnderCursor = ShowDungeonEntryDescriptions_Web_Tunnels();         break;
		case DUN_58_GATEWAY_OF_BLOOD       : IswarpUnderCursor = ShowDungeonEntryDescriptions_Gateway_of_Blood();    break;
		case DUN_59_HALL_OF_FIRE           : IswarpUnderCursor = ShowDungeonEntryDescriptions_Hall_of_Fire();        break;
		case DUN_57_ARACHNID_LAIR          : IswarpUnderCursor = ShowDungeonEntryDescriptions_Arachnid_Lair();       break;
		case DUN_50_TEMPLE_OF_SUNSET       : IswarpUnderCursor = ShowDungeonEntryDescriptions_TempleOfSunset();      break;
		case DUN_54_ABANDONED_FANE         : IswarpUnderCursor = ShowDungeonEntryDescriptions_AbandonedFane();       break;
		case DUN_51_FETID_CAVERN           : IswarpUnderCursor = ShowDungeonEntryDescriptions_FetidCavern();         break;
		case DUN_52_MOSSY_GROTTO           : IswarpUnderCursor = ShowDungeonEntryDescriptions_Mossy_Grotto();        break; 
		case DUN_110_BLACK_CHAPEL		   : IswarpUnderCursor = ShowDungeonEntryDescriptions_BlackChapel();		 break;
		case DUN_111_CHURCH_SACRISTY	   : IswarpUnderCursor = ShowDungeonEntryDescriptions_Sacristy();			 break;
		case DUN_112_STILL_SPRINGS		   : IswarpUnderCursor = ShowDungeonEntryDescriptions_Springs();			 break;
		case DUN_113_THE_MAZE			   : IswarpUnderCursor = ShowDungeonEntryDescriptions_Maze();				 break;
		case DUN_114_LOWER_SCRIPTORIUM	   : IswarpUnderCursor = ShowDungeonEntryDescriptions_Lower_Scriptorium();	 break;
		case DUN_60_HAUNTED_LABYRINTH	   :
		case DUN_61_CURSED_SANCTUM		   :
		case DUN_62_DIM_UNDERCROFT		   :
		case DUN_63_FORGOTTEN_OSSUARY	   :
		case DUN_64_MAUSOLEUM			   :
		case DUN_65_RUINED_TEMPLE		   :
		case DUN_66_SEWERS				   :
		case DUN_67_SEPULCHER			   :
		case DUN_68_OUBLIETTE			   :
		case DUN_69_GIBBET				   :
		case DUN_71_GRIM_VAULTS:
		case DUN_72_NECROPOLIS:
		case DUN_73_LAVA_PITS:
		case DUN_74_CRATER:
		case DUN_75_RIVER_OF_FIRE:
		case DUN_76_DEN_OF_EVIL:
		case DUN_77_VILE_TUNNELS:
		case DUN_78_THE_BRIDGE:
		case DUN_79_COLONNADE:
		case DUN_80_HELL_GATE:
		case DUN_81_UNDERWORLD:
		case DUN_82_HALLS_OF_SUFFERING:
		case DUN_83_BLOOD_KEEP:
		case DUN_84_ARMORIES_OF_HELL:
		case DUN_85_DEMON_STRONGHOLD:
		case DUN_86_VALLEY_OF_WRATH:
		case DUN_87_PANDEMONIUM:
		case DUN_88_REALM_OF_TERROR:
		case DUN_89_BEDLAM:
		case DUN_90_FIELDS_OF_MADNESS:
		case DUN_91_STYGIAN_PASSAGE:
		case DUN_92_FLESH_PIT:
		case DUN_93_BLEEDING_VEINS_PATH:
		case DUN_94_ROT_CHAMBERS:
		case DUN_95_MARROW_GARDEN:
		case DUN_96_BONE_MIRE:
		case DUN_97_GLOOMY_DEPTHS:
		case DUN_98_EBON_CHASM:
		case DUN_99_ANCIENT_CRYPTS:
		case DUN_100_HORADRIM_TREASURY:
		case DUN_101_VOID_CHAMBERS:
		case DUN_102_OBLIVION_TRAIL:
		case DUN_103_ALTAR_OF_IMMOLATION:
		case DUN_104_NETHERWORLD_GATES:
		case DUN_105_DARK_DESCENT:
		case DUN_106_BASTION_OF_NIGHT:
		case DUN_107_DREAD_SANCTUARY:
		case DUN_108_ARCANE_PRISON:
		case DUN_70_CITADEL				   : IswarpUnderCursor = ShowDungeonEntryDescriptions_Alternatives();		 break;
		}
    }
	//if( ! IswarpUnderCursor ) IswarpUnderCursor = ShowRemainWarpDescriptions();
}

//----- (00716CA0) --------------------------------------------------------
void __fastcall CannotEnterBeforeClearing( int flags, int messageIndex )
{
	AddOnScreenMessage(messageIndex);
	Player& player = Players[CurrentPlayerIndex];
	int row = player.Row;
	if( flags & 1 ){
		row++;
	}
	int col = player.Col;
	if( flags & 2 ){
		col++;
	}
    NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, row, col ); // TODO: check for correct usage net command here (command sent to next dungeon after StartNewLvl)
	int soundIndex;
	switch (player.ClassID ){
	case PC_0_WARRIOR:
	case PC_5_SAVAGE:		soundIndex = S_772_WAR_43;	break;
	case PC_1_ARCHER:
	case PC_4_ROGUE:		soundIndex = S_663_ROG_43;	break;
	case PC_2_MAGE:			soundIndex = S_560_SOR_43;	break;
	case PC_3_MONK:			soundIndex = S_880_MONK_43;	break;
	}
	PlayGlobalSound(soundIndex);
}

//----- (00777777) -------------------------------------------------------
int IsSomeMonstersActivated()
{
	for( int monsterIndexIndex = SummonMonsters_Count; monsterIndexIndex < MonstersCount; monsterIndexIndex++ ){
		Monster& monster = Monsters[MonsterIndexes[monsterIndexIndex]];
		if( !monster.speechIndex && monster.ActivationCounter ){
			return 1;
		}
	}
	return 0;
}

//----- (00777888) --------------------------------------------------------
bool IsSaveAndLeavingEnabled()
{
	return is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN) || SaveAlwaysEnabled || !IsSomeMonstersActivated();
}

//----- (0047935E) --------------------------------------------------------
void CheckTriggers() // CheckPlayersOnDunEntries
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.CurAction != PCA_0_STAND ){
		return;
	}
	for( auto& warp: Warps ){
		if( player.Row != warp.Row || player.Col != warp.Col ) continue;

		if( GameMode == GM_COLISEUM ){
			if( is(warp.GameMessage, WM_1028_RETURN_FROM_QUEST_DUNGEON, WM_1032_RETURN_TO_TOWN) ){
				StartNewLvl(CurrentPlayerIndex, warp.GameMessage, warp.destDungeon);
			}else{
				AddOnScreenMessage(OM_111_Only_Arena);
			}
			return;
		}
		if( warp.GameMessage == WM_1026_NEXT_DUNGEON ){
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags;
				if( Dungeon->genType <= DT_2_CATACOMB ){
					flags = BIT(2);
				}else{
					if( Dungeon == DUN_15_HELL_3 ){
						flags = BIT(1)|BIT(2);
					}else{
						flags = BIT(1);
					}
				}
				CannotEnterBeforeClearing(flags, OM_4_battle_not_over);
				return;
			}
			if (GameModeType() == GAME_MODE_TYPE::IRON && (GetNumberOfMonstersInDungeon() > 0 || GetNumberOfObjectsInDungeon() > MaxIronmanObjects )) {
				int flags;
				if (Dungeon->genType <= DT_2_CATACOMB) {
					flags = BIT(2);
				}
				else {
					if (Dungeon == DUN_15_HELL_3) {
						flags = BIT(1) | BIT(2);
					}
					else {
						flags = BIT(1);
					}
				}
				CannotEnterBeforeClearing(flags, OM_71_Clear_Objects_Kill_Monsters);
				return;
			}
			if( ! StartNewLvl(CurrentPlayerIndex, warp.GameMessage, Dungeon + 1) ) return;
		}else if( warp.GameMessage == WM_1027_PREV_DUNGEON ){
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_4_battle_not_over);
				return;
			}

			if (GameMode == GM_NIGHTMARE) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_68_Nightmare_Walk_Up);
				return;
			}
			if (GameMode == GM_SPEEDRUN) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_74);
				return;
			}
			if (GameModeType() == GAME_MODE_TYPE::IRON) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_70_Ironman_Walk_Up);
				return;
			}
			if (!StartNewLvl(CurrentPlayerIndex, warp.GameMessage, Dungeon - 1)) {
				return;
			}
		}else if( warp.GameMessage == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
			if( MonstersCount == SummonMonsters_Count || SaveAlwaysEnabled || is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN) 
				|| (MaxCountOfPlayersInGame != 1 && not(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE) ) ){ // Players can leave quest area (back to non-quest area) via stairs anytime in MP
			//if( true || SaveAlwaysEnabled || is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN) || MaxCountOfPlayersInGame != 1 ){ // For faster testing quest area
				if (!StartNewLvl(CurrentPlayerIndex, warp.GameMessage, Dungeon.returnLvl)) {
					return;
				} // TODO: continue here, check for change to Warp.destDungeon
			}else{
				int row = player.Row;
				int col = player.Col;
				int flags = 0;
				if( Dungeon.returnLvl == Quests[Q_12_THE_CURSE_OF_KING_LEORIC].dungeonStart ){
					++col;
				}
				else if (Dungeon.returnLvl == Quests[Q_34_GRAVE_MATTERS].dungeonStart) {
					++col;
				}
				else if (Dungeon.returnLvl == Quests[Q_35_IRON_OATH].dungeonStart) {
					++col;
				}
				else if (Dungeon.returnLvl == Quests[Q_36_CLEAR_WATER].dungeonStart) {// check if needed
					++row;
				}
				else if( Dungeon.returnLvl == Quests[Q_14_THE_CHAMBER_OF_BONE].dungeonStart ){
					--row;
				}
				else if (Dungeon.returnLvl == Quests[Q_31_GOOD_DEAL].dungeonStart) {
					++row;
				}
				/*else if (Dungeon.returnLvl == Quests[Q_38_OGDENS_SIGN_MP].dungeonStart) {
					++col;
				}*/
				else if( Dungeon.returnLvl == Quests[Q_13_POISONED_WATER_SUPPLY].dungeonStart ){
					++row;
				}
				else if (Dungeon.returnLvl == DUN_1_CHURCH_1) {//  must be wrong, needs check
					++row;
				}
				else{
					++row;
					++col;
				}
				AddOnScreenMessage(OM_2_area_not_cleared);
                NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, row, col );

				int soundIndex;
				switch (player.ClassID ){
					case PC_0_WARRIOR:
					case PC_5_SAVAGE:		soundIndex = S_772_WAR_43;	break;
					case PC_1_ARCHER:
					case PC_4_ROGUE:		soundIndex = S_663_ROG_43;		break;
					case PC_2_MAGE:			soundIndex = S_560_SOR_43;		break;
					case PC_3_MONK:			soundIndex = S_880_MONK_43;			break;
				}
				PlayGlobalSound(soundIndex);
			}
		}
		else if (warp.GameMessage == WM_1029_GOTO_QUEST_DUNGEON) {
			if( auto msg = BlockMessageToQuestDungeon(); msg == OM_0_NO_MESSAGE ){
//			if (true || SaveAlwaysEnabled || is(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN)) { // For faster testing quest area
				// fleshdoom patch (5) did
				if (warp.destDungeon == DUN_42_WIELDER && Quests[Q_21_THE_CURSED_BLADE].status2 <= 2 && Dungeon == DUN_41_FLESHDOOM
					|| warp.destDungeon == DUN_43_HEPHASTO && Quests[Q_21_THE_CURSED_BLADE].status2 <= 4) {
					// mor: these modes dont need you to talk to Tremain to go down because you cant go to town there anyway
					if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL, GM_SPEEDRUN)) {
						goto SKIP_TREMAIN;
					}
					else {
						CannotEnterBeforeClearing(BIT(1), OM_67_Talk_Tremain);
					}					
				}else{
					SKIP_TREMAIN:
					StartNewLvl(CurrentPlayerIndex, warp.GameMessage, /*Dungeon*/warp.destDungeon); // TODO: continue here, check for Dungeon needing here
				}
			}else{
				int flags = BIT(1)|BIT(2);
				if( Dungeon >= DUN_32_HORAZONS_HAVEN && Dungeon <= DUN_34_THE_PIT ){
					flags = BIT(2);
				}
				// fleshdoom patch (6) did
				if( Dungeon >= DUN_41_FLESHDOOM && Dungeon <= DUN_43_HEPHASTO ){
					flags = BIT(1);
				}
				CannotEnterBeforeClearing (flags, msg);
			}
		}else if( warp.GameMessage == WM_1031_GOTO_DUNGEON ){
			if( MaxCountOfPlayersInGame == 1 ){
				if( ! StartNewLvl(CurrentPlayerIndex, warp.GameMessage, warp.destDungeon) ) return;
				continue;
			}
			int clvl = player.CharLevel;
			int targetDungeonType = (warp.destDungeon - 1) / 4;

			if( clvl >= ClvlReqsForEnterInDungeon[ Difficulty ][ targetDungeonType ] ){
				if( ! StartNewLvl(CurrentPlayerIndex, warp.GameMessage, warp.destDungeon) ) return;
				continue;
			}

			int row = player.Row;
			int col = player.Col;
			switch (warp.destDungeon ){
				case 1:
				case 5:
				case 13:	
				case 21:	col++;	break;
				case 9:		row++;	break;
				case 17:	row--;	break;
			}
            NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, row, col );

			int soundIndex;
			switch (player.ClassID ){
				case PC_0_WARRIOR:
				case PC_5_SAVAGE:		soundIndex = S_772_WAR_43;	break;
				case PC_1_ARCHER:
				case PC_4_ROGUE:		soundIndex = S_663_ROG_43;		break;
				case PC_2_MAGE:			soundIndex = S_560_SOR_43;		break;
				case PC_3_MONK:			soundIndex = S_880_MONK_43;			break;
			}
			PlayGlobalSound(soundIndex);

			int messageIndex = ClvlNotReqForEnterInDungeonMessagesIndexesTable[ Difficulty ][ targetDungeonType ];
			AddOnScreenMessage(messageIndex);

		}else if( warp.GameMessage == WM_1032_RETURN_TO_TOWN ){
			if( Dungeon->isQuest && MonstersCount > SummonMonsters_Count && !SaveAlwaysEnabled && not(GameMode, GM_EASY, GM_CLASSIC, GM_SPEEDRUN) ){
				CannotEnterBeforeClearing(2, OM_2_area_not_cleared);
				return;
			}
			if (GameMode == GM_NIGHTMARE) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_68_Nightmare_Walk_Up);
				return;
			}
			/*if (GameMode == GM_SPEEDRUN) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_74);
				return;
			}*/
			if (GameModeType() == GAME_MODE_TYPE::IRON) {
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_70_Ironman_Walk_Up);
				return;
			}
			if( MaxCountOfPlayersInGame == 1 && !IsSaveAndLeavingEnabled() ){
				int flags = BIT(2);
				CannotEnterBeforeClearing(flags, OM_4_battle_not_over);
				return;
			}
			if( ! StartNewLvl(CurrentPlayerIndex, warp.GameMessage, DUN_0_TOWN) ) return;
		}else{
			TerminateWithError("Unknown trigger msg");
		}
	}
}
