#include "stdafx.h"

int UberBookReadParam;
// player class to difficulty - arkaine's valor rewards
static const int valorRewards[VALOR_CLASSES][VALOR_DIFFS] = { // unique item numbers
	{178, 179, 180, 139, 140, 141, 142, 143, 144 },// exp
	{368, 369, 370, 120, 121, 122, 123, 124, 125 },// exp
	{374, 375, 376,  61,  62,  63,  64,  65,  66 },// exp
	{371, 372, 373, 120, 121, 122, 123, 124, 125 },// exp
	{368, 369, 370, 120, 121, 122, 123, 124, 125 },// exp
	{178, 179, 180, 139, 140, 141, 142, 143, 144 },// exp
};

int mundaneInventoryRewards[] = { 2825, 2826, 2827, 2828, 2829, 2830, 2831, 2832, 2833 };

int blindRewards[HotB_UNIQUES] = {
				172, 173, 174, 2371, 2372, 2373, 2374, 2375, 2376
};

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация, новая функция. Нужна для загрузки графики для одного объекта карты по индексу
__forceinline void LoadOneObjectSprite(int floorItemSpriteIndex)
{
	ObjectSpriteIndexes[ObjectsSpritesCount] = floorItemSpriteIndex;
	char* objectName;
	switch(Dungeon->graphType){
	case DT_5_CRYPT: objectName = FloorObjectsNamesCrypt[floorItemSpriteIndex]; break;
	case DT_6_ABYSS: objectName = FloorObjectsNamesAbyss[floorItemSpriteIndex]; break;
	default:         objectName = FloorObjectsNamesOriginal[floorItemSpriteIndex]; break;
	}
	char fileName[32];
	sprintf(fileName, "Objects\\%s.CEL", objectName);
	ObjectsSprites[ObjectsSpritesCount++] = (char*)LoadFile(fileName);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (0044BD31) --------------------------------------------------------
void LoadObjectSprites()
{
	int dungeonLevel = Dungeon->level; // TODO: change to check Dungeon->graphType
	if( dungeonLevel >= 21 && dungeonLevel <= 24 ){// слеп прикидывается церковью
		dungeonLevel -= 20;
	}else if( dungeonLevel >= 17 && dungeonLevel <= 20 ){// а бездна пещерами
		dungeonLevel -= 8;
	}

	bool spriteSuitableForFloorFlag[57];
	memzero(spriteSuitableForFloorFlag);

	for( int baseObjectIndex = 0; BaseObjects[baseObjectIndex].RegularObject != -1; baseObjectIndex++ ){
		BaseObject& baseObject = BaseObjects[baseObjectIndex];

		if( true/*baseObject.RegularObject == 1*/ ){
			if( dungeonLevel >= baseObject.minDungeonLevel && dungeonLevel <= baseObject.maxDungeonLevel ){
				spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
			}
		}

		if( baseObject.roomType != RT_NONE ){
			for( int i = 0; i < RoomsCount; i++ ){
				if( Rooms[i].roomType == baseObject.roomType ){
					spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
				}
			}
		}

		if( baseObject.QuestIndex != -1 ){
			if( IsQuestOnLevel(baseObject.QuestIndex) ){
				spriteSuitableForFloorFlag[baseObject.SpriteIndex] = 1;
			}
		}
	}
	
	for( int floorItemSpriteIndex = 0; floorItemSpriteIndex < 57; floorItemSpriteIndex++ ){
		if( spriteSuitableForFloorFlag[floorItemSpriteIndex] ){
			LoadOneObjectSprite(floorItemSpriteIndex);
		}
	}
}

//----- (0044BEAB) --------------------------------------------------------
void FreeObjectsSprites()
{
	for( int objectSpriteIndex = 0; objectSpriteIndex < ObjectsSpritesCount; objectSpriteIndex++ ){
		FreeMemZero(ObjectsSprites[objectSpriteIndex]);
	}
	memzero(ObjectSpriteIndexes);
	ObjectsSpritesCount = 0;
}

//----- (0044BEDC) --------------------------------------------------------
int __fastcall AddRandFromRangeObjectsOnMap(int ecx0, int a2, BASE_OBJECT a1)
{
	int result; // eax@1
	int v4 = 0; // ecx@1
	int v5;     // ebx@2
	int v6;     // edi@2
	int v7;     // esi@2
	int v8 = 0; // esi@1
	int v9;     // edx@1
	char v10;   // cl@1
	int v11;    // eax@2
	char v12;   // cl@2
	int v13;    // eax@2
	int v14;    // [sp+8h] [bp-4h]@1
	int v15;    // [sp+4h] [bp-8h]@1
	v8 = ecx0;
	v9 = a2 - ecx0;
	v10 = -117;
	v14 = 0;
	result = v8 + RNG(v9);
	v15 = result;
	if( result > 0 ){
		while( 1 ){
			do{
				LOBYTE_IDA(v4) = 139;
				v11 = RNG(80);
				v12 = -117;
				v7 = v11 + 16;
				v13 = RNG(80);
				v6 = v13 + 16;
				v5 = v13 + 15;
			}while( !IsCellFree(v7 - 1, v13 + 15) );
			if( IsCellFree(v7, v5) ){
				if( IsCellFree(v7 + 1, v5) ){
					if( IsCellFree(v7 - 1, v6) ){
						if( IsCellFree(v7, v6) ){
							if( IsCellFree(v7 + 1, v6) ){
								if( IsCellFree(v7 - 1, v6 + 1) ){
									if( IsCellFree(v7, v6 + 1) ){
										if( IsCellFree(v7 + 1, v6 + 1) ){
											AddObject(a1, v7, v6);
											++v14;
											result = v14;
											if( v14 >= v15 )
												break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}

//----- (0044BFCC) --------------------------------------------------------
char __fastcall IsCellFree(int row, int col)
{
	int cell = 112 * row + col;
	int fineCell = FineMap[ 0 ][ cell ];
	if( MonsterMap[ 0 ][ cell ] || PlayerMap[ 0 ][cell] || ObjectsMap[ 0 ][cell] || (FlagMap[ 0 ][cell] & CF_8_FORBIDDEN) || TileBlockWalking[fineCell]
	|| (Dungeon->genType == DT_1_CHURCH && fineCell > 126 && fineCell < 144) ){
		return false;
	}
	return true;
}

//----- (0044C022) --------------------------------------------------------
void __fastcall AddDoorsAndLibrazObjectInChurch( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col112 = minCol112; col112 < maxCol112; ++col112 ){
		for( int row112 = minRow112; row112 < maxRow112; ++row112 ){
			uint cell112 = 112 * row112 + col112;
			BASE_OBJECT newBaseObjectIndex = BO_NONE;
			switch( FineMap[ 0 ][ cell112 ] ){
			case 270: newBaseObjectIndex = BO_0_L1BRAZ;	break;
			case 44: case 51: case 214:	newBaseObjectIndex = BO_1_CHURCH_DOOR_MAIN_DIAGONAL; break;
			case 46: case 56: newBaseObjectIndex = BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL; break;
			}
			if( newBaseObjectIndex != BO_NONE ){
				AddObject(newBaseObjectIndex, row112, col112);
			}
		}
	}
}

//----- (0044C0BA) --------------------------------------------------------
int __fastcall AddCryptDoors(int a1, int a2, int a3, int a4)
{
	int v4;     // edi@2
	int* v5;    // esi@3
	int v6;     // ebx@4
	int result; // eax@9
	int aRow;   // [sp+0h] [bp-8h]@1
	int aCol;   // [sp+4h] [bp-4h]@1
	aRow = a1;
	for( aCol = a2; aCol < a4; result = aCol ){
		v4 = aRow;
		if( aRow < a3 ){
			v5 = &FineMap[ aRow ][ aCol ];
			do{
				v6 = *v5;
				if( *v5 == 77 )
					AddObject(BO_1_CHURCH_DOOR_MAIN_DIAGONAL, v4, aCol);
				if( v6 == 80 )
					AddObject(BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL, v4, aCol);
				++v4;
				v5 += 112;
			}while( v4 < a3 );
		}
		++aCol;
	}
	return result;
}

//----- (0044C128) --------------------------------------------------------
int __fastcall AddDoorsObjectInCatacomb(int aStartRow, int aStartCol, int EndRow, int aEndCol)
{
	int Row;         // ebx@2
	int* MapCellPtr; // esi@3
	int MapVal;      // edi@4
	int result;      // eax@11
	int StartRow;    // [sp+0h] [bp-8h]@1
	int Col;         // [sp+4h] [bp-4h]@1
	StartRow = aStartRow;
	for( Col = aStartCol; Col < aEndCol; result = Col ){
		Row = StartRow;
		if( StartRow < EndRow ){
			MapCellPtr = &FineMap[ StartRow ][ Col ];
			do{
				MapVal = *MapCellPtr;
				if( *MapCellPtr == 13 || MapVal == 541 )
					AddObject(BO_42_CATACOMB_DOOR_MAIN_DIAGONAL, Row, Col);
				if( MapVal == 17 || MapVal == 542 )
					AddObject(BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL, Row, Col);
				++Row;
				MapCellPtr += 112;
			}while( Row < EndRow );
		}
		++Col;
	}
	return result;
}

//----- (00717300) --------------------------------------------------------
void AddObjectOnHorazonEnter()
{
	int row = 2 * CurPattern.row + 18;
	int col = 2 * CurPattern.col + 24;
	AddObject( BO_86_QUEST_BOOK, row, col );
	InitHorazonQuestBook( ObjectsMap[ row ][ col ] - 1, 16 );
}

// init object for non-quest dungeon
//----- (0044C1A6) --------------------------------------------------------
void __fastcall InitObjects()
{
	int speech = SP_0;
	ClearAllObjects();
	UberBookReadParam = 0;
	if( Dungeon == DUN_16_HELL_4 ){
		AddDiabloLevelObjects();
		return;
	}
	InitObjFlag = 1;
	GetRndSeed();
	if( Dungeon == DUN_9_CAVE_1 ){
		if( MaxCountOfPlayersInGame == 1 ){
			AddSlainHeroOnMap();
		}
	}
	if( Dungeon == Quests[Q_1_BLACK_MUSHROOM].dungeonStart ){
		if( Quests[Q_1_BLACK_MUSHROOM].status == QS_1_ACCEPT ){
			AddMushroomPathObject();
		}
	}
	if( is(Dungeon, DUN_4_CHURCH_4, DUN_8_CATACOMB_4, DUN_12_CAVE_4) ){
		PutQuestBookOnMap();
	}
	#ifdef CRYPT_QUEST
	if( Dungeon == DUN_21_CRYPT_1 ){
		PutCryptQuestBookOnMap(1);
	}else if( Dungeon == DUN_22_CRYPT_2 ){
		PutCryptQuestBookOnMap(2);
		PutCryptQuestBookOnMap(3);
	}else if( Dungeon == DUN_23_CRYPT_3 ){
		PutCryptQuestBookOnMap(4);
		PutCryptQuestBookOnMap(5);
	}
	#endif
	if( Dungeon == DUN_24_CRYPT_4 ){
		#ifdef CRYPT_QUEST
		Floor24QuestMapObjectsGeneration();
		#endif
		AddQuestLeverOn24Level();
	}
	if( Dungeon->genType == DT_1_CHURCH ){
		if( IsQuestOnLevel( Q_16_HORAZON_S_DEMONS ) ){
			AddObjectOnHorazonEnter();
		}
		if( IsQuestOnLevel(Q_6_THE_BUTCHER) && SetCurPattern(Q_6_THE_BUTCHER) ){
			AddButcherObjects();
			RestoreCurPattern();
		}
		if( IsQuestOnLevel( Q_13_POISONED_WATER_SUPPLY )
			// TODO: check wtf is this (was memory corruption in dungeon 1
			// || (Dungeon == DUN_1_CHURCH_1 && MaxCountOfPlayersInGame == 1 /*&& is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_NIGHTMARE, GM_SURVIVAL)*/)
			)
			AddSomeObjectsOnMapWithPoisonWaterQuest();
		if( IsQuestOnLevel( Q_7_OGDENS_SIGN ) ){
			AddObject( BO_97_OGDEN_CHEST, 2 * CurPattern.row + 26, 2 * CurPattern.col + 19);
		}
		AddSomeTwoTilesSizeObjectOnMap(10, 15, BO_48_SARCOPHAGUS);
		if( Dungeon->graphType == DT_5_CRYPT )
			AddCryptDoors(0, 0, 112, 112);
		else
			AddDoorsAndLibrazObjectInChurch(0, 0, 112, 112);
		AddSomeBarrelsOnLevel();
	}
	if( Dungeon->genType == DT_2_CATACOMB ){// явно что то не так. Там внутри оказались проверки на другие DT
		if( IsQuestOnLevel(Q_0_THE_MAGIC_ROCK) ){
			AddRandFromRangeCountOfObjects(1, 1, BO_23_MAGIC_ROCK_TABLE);
		}
		if( IsQuestOnLevel(Q_14_THE_CHAMBER_OF_BONE) ){
			AddRandFromRangeCountOfObjects(1, 1, BO_41_MYTHICAL_BOOK);
		}
		AddDoorsObjectInCatacomb(0, 0, 112, 112);
		AddCatacombTorches();
		if( IsQuestOnLevel(Q_8_HALLS_OF_THE_BLIND) ){
			switch( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR: 
			case PC_5_SAVAGE: speech = SP_237; break;
			case PC_1_ARCHER: speech = SP_245; break;
			case PC_2_MAGE:   speech = SP_241; break;
			case PC_3_MONK:   speech = SP_261; break;
			case PC_4_ROGUE:  speech = SP_265; break;
			}

			Quests[Q_8_HALLS_OF_THE_BLIND].speechIndex = speech;
			AddQuestBookThatChangeMap(112, 112, CurPattern.row, CurPattern.col, CurPattern.sizeRow + CurPattern.row + 1, CurPattern.sizeCol + CurPattern.col + 1, speech);
			uchar* blind = (uchar*) LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blind2_cl.DUN" : "Levels\\L2Data\\Blind2.DUN");
			LoadMapObjects(blind, 2 * CurPattern.row, 2 * CurPattern.col);
			FreeMem(blind);
		}
		if( IsQuestOnLevel(Q_9_VALOR) ){
			switch( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE: speech = 236; break;
			case PC_1_ARCHER: speech = 244; break;
			case PC_2_MAGE:   speech = 240; break;
			case PC_3_MONK:   speech = 260; break;
			case PC_4_ROGUE:  speech = 264; break;
			}

			Quests[Q_9_VALOR].speechIndex = speech;
			AddQuestBookThatChangeMap(112, 112, CurPattern.row, CurPattern.col + 3, CurPattern.row + 2, CurPattern.col + 7, speech);
			AddObject(BO_73_PEDESTAL_OF_BLOOD, 2 * CurPattern.row + 25, 2 * CurPattern.col + 32);
		}

		AddSomeBarrelsOnLevel();
	}
	if( Dungeon->genType == DT_3_CAVE ){
		AddCaveDoors(0, 0, 112, 112);
		if( Dungeon->level >= 9 ){ // TODO: check or remove condition, cave levels always >= 9
			AddRandFromRangeObjectsOnMap(0, Difficulty / 3 + 2, BO_80_CAULDRON);
		}
		if( IsQuestOnLevel( Q_20_SUNLESS_SEA ) ){
			uchar* island = (uchar*) LoadFile( "levels\\L3data\\islandentry.dun" );
			LoadMapObjects( island, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1 );
			FreeMem( island );
			SetupAngels();
		}
		
		//if( QuestPattern* qp = GetQuestPattern(Q_36_CLEAR_WATER) ){
		if( IsQuestOnLevel(Q_36_CLEAR_WATER) && SetCurPattern(Q_36_CLEAR_WATER) ){
			uchar* swamp = (uchar*)LoadFile("levels\\L3data\\swampy_cavern_entry.dun");
			//LoadMapObjects(swamp, 2 * qp->row + 1, 2 * qp->col + 1);
			LoadMapObjects(swamp, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1);
			FreeMem(swamp);
			RestoreCurPattern();
		}
		if( IsQuestOnLevel(Q_29_IRON_FIGURINE)) {
			uchar* swamp = (uchar*)LoadFile("levels\\L3data\\swampy_cavern_entry.dun", 0);
			LoadMapObjects(swamp, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1);
			FreeMem(swamp);
		}
		if (IsQuestOnLevel(Q_30_DEMON_PLAGUE)) {
			uchar* grotto = (uchar*)LoadFile("levels\\L3data\\mossy_grotto_entry.dun");
			LoadMapObjects(grotto, 2 * CurPattern.row + 1, 2 * CurPattern.col + 1);
			FreeMem(grotto);
		}
		AddSomeBarrelsOnLevel();
	}
	if( Dungeon->genType == DT_4_HELL ){
		if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) ){
			switch( Players[CurrentPlayerIndex].ClassID ){
			case PC_0_WARRIOR: 
			case PC_5_SAVAGE: speech = 238; break;
			case PC_1_ARCHER: speech = 246; break;
			case PC_2_MAGE:   speech = 242; break;
			case PC_3_MONK:   speech = 262; break;
			case PC_4_ROGUE:  speech = 266; break;
			}

			Quests[Q_11_WARLORD_OF_BLOOD].speechIndex = speech;
			AddQuestBookThatChangeMap(112, 112, CurPattern.row, CurPattern.col, CurPattern.row + CurPattern.sizeRow, CurPattern.col + CurPattern.sizeCol, speech);
			uchar* warlord = (uchar*) LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\Warlord_CL.DUN" : "Levels\\L4Data\\Warlord.DUN");
			LoadMapObjects(warlord, 2 * CurPattern.row, 2 * CurPattern.col);
			FreeMem(warlord);
		}
		if( IsQuestOnLevel(Q_15_ARCHBISHOP_LAZARUS) ){
			if( MaxCountOfPlayersInGame == 1 ){
				AddLazarusVileStand();
			}
		}
		if( IsQuestOnLevel(Q_21_THE_CURSED_BLADE) ){
			if( auto* qp = GetQuestPattern(Q_21_THE_CURSED_BLADE); qp && qp->dun ){
				LoadMapObjects((uchar*)qp->dun, 2 * qp->row + 1, 2 * qp->col);
			}
		}
		AddSomeBarrelsOnLevel();
		AddHellDecor();
	}
	AddRandFromRangeObjectsOnMap(5, 10, BO_5_SMALL_CHEST);
	AddRandFromRangeObjectsOnMap(3, 6, BO_6_CHEST);
	AddRandFromRangeObjectsOnMap(1, 5, BO_7_LARGE_CHEST);
	if( Dungeon->genType != DT_4_HELL ){
		TrapingObjectsWithWallTraps();
	}
	if( Dungeon->level > 4 ){ // TODO: check for crypt dun type ?
		TrapingChestsWithContactTrap();
	}
	InitObjFlag = 0;
}

// TODO: косячит
//----- (0044C607) --------------------------------------------------------
void __fastcall AddSomeTwoTilesSizeObjectOnMap( int minCount, int maxCount, BASE_OBJECT baseObjectIndex )
{
	int v3; // esi@1
	int v4; // edx@1
	//int v5; // ecx@1
	int v6; // eax@2
	//int v7; // ecx@2
	int v8; // edi@2
	int v9; // eax@2
	int v10; // esi@2
	int v11; // ebx@2
	int v12; // [sp+4h] [bp-8h]@1
	int v13; // [sp+8h] [bp-4h]@1

	v3 = minCount;
	v4 = maxCount - minCount;
	v13 = 0;
	v12 = v3 + RNG(v4);
	if( v12 > 0 ){
		while( 1 ){
			do{
				v6 = RNG(80);
				v8 = v6 + 16;
				v9 = RNG(80);
				v10 = v9 + 16;
				v11 = v9 + 14;
			}while( !IsCellFree( v8 - 1, v9 + 14 ) );
			if( IsCellFree( v8, v11 ) ){
				if( IsCellFree( v8 + 1, v11 ) ){
					if( IsCellFree( v8 - 1, v10 - 1 ) ){
						if( IsCellFree( v8, v10 - 1 ) ){
							if( IsCellFree( v8 + 1, v10 - 1 ) ){
								if( IsCellFree( v8 - 1, v10 ) ){
									if( IsCellFree( v8, v10 ) ){
										if( IsCellFree( v8 + 1, v10 ) ){
											if( IsCellFree( v8 - 1, v10 + 1 ) ){
												if( IsCellFree( v8, v10 + 1 ) ){
													if( IsCellFree( v8 + 1, v10 + 1 ) ){
														AddObject( baseObjectIndex, v8, v10 );
														if( ++v13 >= v12 ){
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//----- (0044C730) --------------------------------------------------------
void __fastcall AddRandFromRangeCountOfObjects( int minCount, int maxCount, BASE_OBJECT baseObjectIndex )
{
	int objectsCount = minCount + RNG(maxCount - minCount);
	for( int currentIndex = 0; currentIndex < objectsCount; currentIndex++ ){
		for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
			bool coordinateIsCorrect = true;
			int row112 = RNG(80) + 16;
			int col112 = RNG(80) + 16;

			for( int colOffset = -2; colOffset <= 2; colOffset++ ){
				for( int rowOffset = -2; rowOffset <= 2; rowOffset++ ){
					if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
						coordinateIsCorrect = false;
					}
				}
			}

			if( coordinateIsCorrect ){
				AddObject(baseObjectIndex, row112, col112);
				break;
			}
		}
	}
}

//----- (0044C7E2) --------------------------------------------------------
void ClearAllObjects()
{
	memzero(Objects);
	ObjectsCount = 0;
	for( int i = 0; i < MaxObjects_127; i++ ){ ObjectAvailable[i] = i; }
	memzero(ObjectActive);
	FlameGroupIndex = 1;
	FlameOrientation = 0;
	LeverId = 1;
}

//----- (0044C83A) --------------------------------------------------------
int AddButcherObjects()
{
	int v0;     // esi@1
	int v1;     // edi@2
	int result; // eax@5
	int* v3;    // [sp+Ch] [bp-4h]@2
	v0 = 0;
	do{
		v1 = 2;
		v3 = &FineMap[ 0 ][ v0 ];
		do{
			if( *v3 == 367 ){
				AddObject(BO_36_TSOUL, v1 - 2, v0 + 1);
				AddObject(BO_38_TSOUL, v1, v0 - 1);
				AddObject(BO_37_TSOUL, v1 - 2, v0 + 3);
				AddObject(BO_39_TSOUL, v1 + 2, v0 - 1);
				AddObject(BO_40_TSOUL, v1 - 2, v0 + 5);
				AddObject(BO_33_TNUDEW, v1 - 1, v0 + 3);
				AddObject(BO_30_TNUDEM, v1 + 2, v0 + 5);
				AddObject(BO_31_TNUDEM, v1, v0);
				AddObject(BO_32_TNUDEM, v1 + 1, v0 + 2);
				AddObject(BO_33_TNUDEW, v1, v0 + 4);
				AddObject(BO_34_TNUDEW, v1, v0 + 1);
				AddObject(BO_35_TNUDEW, v1 + 2, v0 + 2);
			}
			v3 += 112;
			++v1;
			result = v1 - 2;
		}while( v1 - 2 < 112 );
		++v0;
	}while( v0 < 112 );
	return result;
}

//----- (0044C931) --------------------------------------------------------
void AddSomeObjectsOnMapWithPoisonWaterQuest()
{
	Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	AddObject(BO_87_ON_FLOOR_TORCH, poisonWaterQuest.row - 2, poisonWaterQuest.col + 1);
	AddObject(BO_87_ON_FLOOR_TORCH, poisonWaterQuest.row + 3, poisonWaterQuest.col + 1);
	AddObject(BO_87_ON_FLOOR_TORCH, poisonWaterQuest.row - 1, poisonWaterQuest.col + 2);
	AddObject(BO_87_ON_FLOOR_TORCH, poisonWaterQuest.row + 2, poisonWaterQuest.col + 2);
}

//----- (0044C97A) --------------------------------------------------------
char __stdcall AddQuestBookThatChangeMap(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	int v7;              // ebx@2
	int col;              // edi@2
	int row;              // esi@2
	char result;         // al@4
	int objectIndex;             // esi@17
	int v12;             // [sp+Ch] [bp-Ch]@1
	unsigned __int8 v13; // [sp+17h] [bp-1h]@2
	int v14;             // [sp+10h] [bp-8h]@3
	v12 = 0;
	do{
		v13 = 1;
		row = RNG(80) + 16;
		col = RNG(80) + 16;
		v7 = -2;
		do{
			v14 = -2;
			do{
				result = IsCellFree(v14 + row, v7 + col);
				if( !result )
					v13 &= result;
				++v14;
			}while( v14 <= 2 );
			++v7;
		}while( v7 <= 2 );
		if( v13 )
			break;
		++v12;
		if( v12 > 20000 )
			return result;
	}while( !v13 );
	if( IsQuestOnLevel(Q_8_HALLS_OF_THE_BLIND) )
		AddObject(BO_71_BOOK_OF_THE_BLIND, row, col);
	if( IsQuestOnLevel(Q_11_WARLORD_OF_BLOOD) )
		AddObject(BO_88_STEEL_TOME, row, col);
	if( IsQuestOnLevel(Q_9_VALOR) ){
		row = 2 * CurPattern.row + 25;
		col = 2 * CurPattern.col + 40;
		AddObject(BO_72_BOOK_OF_BLOOD, 2 * CurPattern.row + 25, 2 * CurPattern.col + 40);
	}
	objectIndex = ObjectsMap[ row ][ col ] - 1;
	SetChangeRect(objectIndex, a3, a4, a5, a6, LeverId);
	SetObjectSpeechIndex(objectIndex, a7);
	++LeverId;
	result = 120 * (uchar)objectIndex;
	Objects[objectIndex].maybeFrameIndex = Objects[objectIndex].FrameIndex + 1;
	return result;
}

//----- (0044CA98) --------------------------------------------------------
int AddSomeBarrelsOnLevel()
{
	int result; // eax@1
	int i;      // ebp@1
	int v2;     // edi@2
	int v3;     // esi@2
	int v4;     // edx@3
	char v5;    // bl@3
	int v6;     // eax@2
	int v7;     // eax@3
	int v8;     // eax@8
	char v9;    // al@8
	int v10;    // eax@10
	int v11;    // [sp+Ch] [bp-4h]@1
	int v12;    // [sp+8h] [bp-8h]@3
	int v13;    // [sp+4h] [bp-Ch]@6
	result = RNG(5);
	v11 = 0;
	for( i = result + 3; v11 < i; ++v11 ){
		do{
			v3 = RNG(80) + 16;
			v6 = RNG(80);
			v2 = v6 + 16;
		}while( !IsCellFree(v3, v6 + 16) );
		v7 = RNG(4);
		AddObject((BASE_OBJECT)(BO_58_POD_URN_BARREL_EXPLOSIVE - (v7 != 0)), v3, v2);
		v5 = 1;
		v4 = 0;
		v12 = 1;
		while( 1 ){
			result = RNG(v4);
			if( result )
				break;
			if( !v5 )
				break;
			v13 &= result;
			v5 = 0;
			do{
				if( v13 >= 3 )
					break;
				v8 = 4 * RNG(8);
				v3 += *(int*)((char*)&YArrayForRND[0] + v8);
				v2 += *(int*)((char*)&XArrayForRND[0] + v8);
				v9 = IsCellFree(v3, v2);
				++v13;
				v5 = v9;
			}while( !v9 );
			if( v5 ){
				v10 = RNG(5);
				AddObject((BASE_OBJECT)(BO_58_POD_URN_BARREL_EXPLOSIVE - (v10 != 0)), v3, v2);
				++v12;
			}
			v4 = v12 >> 1;
		}
	}
	return result;
}

//----- (0044CB96) --------------------------------------------------------
int __fastcall AddCaveDoors(int a1, int a2, int a3, int a4)
{
	int v4;     // edi@2
	int* v5;    // esi@3
	int v6;     // ebx@4
	int result; // eax@9
	int aRow;   // [sp+0h] [bp-8h]@1
	int aCol;   // [sp+4h] [bp-4h]@1
	aRow = a1;
	for( aCol = a2; aCol < a4; result = aCol ){
		v4 = aRow;
		if( aRow < a3 ){
			v5 = &FineMap[ aRow ][ aCol ];
			do{
				v6 = *v5;
				if( *v5 == 531 )
					AddObject(BO_74_CAVE_DOOR_MAIN_DIAGONAL, v4, aCol);
				if( v6 == 534 )
					AddObject(BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL, v4, aCol);
				++v4;
				v5 += 112;
			}while( v4 < a3 );
		}
		++aCol;
	}
	return result;
}

//----- (0044CC0A) --------------------------------------------------------
void AddCatacombTorches()
{
	int v0; // esi@1
	int v1; // edi@2
	char *v2; // ebx@2
	int( *v3 )[ 112 ]; // [sp+Ch] [bp-Ch]@2
	int v4; // [sp+10h] [bp-8h]@4
	int( *v5 )[ 112 ]; // [sp+14h] [bp-4h]@1

	v0 = 0;
	v5 = FineMap;
	do{
		v1 = 0;
		v2 = (char *) ObjectsMap + v0;
		v3 = v5;
		do{
			if( CheckCatacombForTorch( v1, v0 ) ){
				v4 = (*v3)[ 0 ];
				if( (*v3)[ 0 ] == 1 && !RNG(3) ){
					AddObject( BO_46_WTORCH1, v1, v0 );
				}
				if( v4 == 5 && !RNG(3) ){
					AddObject( BO_47_WTORCH2, v1, v0 );
				}
				if( v4 == 37 && !RNG(10) && !*(v2 - 112) ){
					AddObject( BO_44_WTORCH4, v1 - 1, v0 );
				}
				if( v4 == 41 && !RNG(10) && !*(v2 - 1) ){
					AddObject( BO_45_WTORCH3, v1, v0 - 1 );
				}
			}
			++v3;
			++v1;
			v2 += 112;
		} while( v1 < 112 );
		v5 = (int( *)[ 112 ])((char *) v5 + 4);
		++v0;
	}while( (uint) v5 < (uint) FineMap[ 1 ] );
}

//----- (0044CCF7) --------------------------------------------------------
char __fastcall CheckCatacombForTorch(int a1, int a2)
{
	return ( ~ FlagMap[ a1 ][ a2 ] >> 3 ) & CF_1_SPELL_CAST;
}

//----- (0044CD09) --------------------------------------------------------
void TrapingObjectsWithWallTraps()
{
	#ifdef USE_IDA_CODE
	int objectCol; // edi@9
	char *objectsMapOfs; // esi@10
	int objectIndex; // ebx@13
	int trapRow; // esi@15
	char *j; // eax@15
	char v5; // al@20
	int trapCol; // esi@21
	char *i; // eax@21
	int v8; // eax@27
	char *objectsMapOfs_1; // [sp+Ch] [bp-10h]@10
	int percentOfTrappedObjects; // [sp+10h] [bp-Ch]@0
	int rowOfs; // [sp+14h] [bp-8h]@10
	int objectRow; // [sp+18h] [bp-4h]@10

	if( Dungeon->level == 1 ){
		percentOfTrappedObjects = 10;
	}
	if( Dungeon->level >= 2 ){
		percentOfTrappedObjects = 15;
	}

	if( Dungeon->level >= 5 ){
		percentOfTrappedObjects = 20;
	}
	if( Dungeon->level >= 7 ){
		percentOfTrappedObjects = 25;
	}
	objectCol = 0;
	do{
		objectRow = 0;
		rowOfs = 0;
		objectsMapOfs = (char *) ObjectsMap + objectCol;
		objectsMapOfs_1 = (char *) ObjectsMap + objectCol;
		do{
			if( *objectsMapOfs > 0 && RNG(100) < percentOfTrappedObjects ){
				objectIndex = (char) (*objectsMapOfs - 1);
				if( BaseObjects[ Objects[ objectIndex ].BaseObjectIndex ].CanBeTrappedWithWallTrap ){
					if( RNG(2) ){
						trapCol = objectCol - 1;
						for( i = (char *) FineMap + 4 * (objectCol - 1 + rowOfs); !TileBlockWalking[ *(DWORD *) i ]; i -= 4 ){
							--trapCol;
						}
						if( IsGoodPlaceForWallTrap( objectRow, trapCol ) && objectCol - trapCol > 1 ){
							AddObject( BO_54_WALL_TRAP_FIRES_IN_COL, objectRow, trapCol );
							v5 = *(&ObjectsMap[ 0 ][ trapCol ] + rowOfs);
							goto LABEL_27;
						}
					}else{
						trapRow = objectRow - 1;
						for( j = (char *) FineMap + 4 * (objectCol + 112 * (objectRow - 1)); !TileBlockWalking[ *(DWORD *) j ]; j -= 448 ){
							--trapRow;
						}
						if( IsGoodPlaceForWallTrap( trapRow, objectCol ) && objectRow - trapRow > 1 ){
							AddObject( BO_53_WALL_TRAP_FIRES_IN_ROW, trapRow, objectCol );
							v5 = ObjectsMap[ trapRow ][ objectCol ];
						LABEL_27:
							v8 = (char) (v5 - 1);
							Objects[ v8 ].TrapedObjectRow = objectRow;
							Objects[ v8 ].TrapedObjectCol = objectCol;
							Objects[ objectIndex ].IsTrapped = 1;
							goto LABEL_28;
						}
					}
				}
			}
		LABEL_28:
			rowOfs += 112;
			++objectRow;
			objectsMapOfs = objectsMapOfs_1 + 112;
			objectsMapOfs_1 += 112;
		}while( rowOfs < 12544 );
		++objectCol;
	}while( objectCol < 112 );
	#else // #ifdef REFACTORED_BUT_WRONG // may be right now
	int percentOfTrappedObjects = 0;
	if( Dungeon->level == 1 ){
		percentOfTrappedObjects = 10;
	} 
	if( Dungeon->level >= 2 ){
		percentOfTrappedObjects = 15;
	}
	if( Dungeon->level >= 5 ){
		percentOfTrappedObjects = 20;
	}
	if( Dungeon->level >= 7 ){
		percentOfTrappedObjects = 25;
	}
	for( int objectCol = 0; objectCol < 112; ++objectCol ){
		for( int objectRow = 0; objectRow < 112; ++objectRow ){
			int objectNum = ObjectsMap[ objectRow ][ objectCol ];
			if( objectNum <= 0 ){
				continue;
			}
			int objectIndex = objectNum - 1;
			Object& object = Objects[objectIndex];
			if( RNG(100) >= percentOfTrappedObjects ){
				continue;
			}
			int baseObjectIndex = object.BaseObjectIndex;
			if( BaseObjects[baseObjectIndex].CanBeTrappedWithWallTrap == 0 ){
				continue;
			}
			uint trapCol;
			uint trapRow;
			BASE_OBJECT trapType;
			if( RNG(2) ){
				trapCol = objectCol - 1;
				trapRow = objectRow;
				trapType = BO_54_WALL_TRAP_FIRES_IN_COL;
				while( !TileBlockWalking[FineMap[ trapRow ][ trapCol ]] ){
					--trapCol;
				}
				if( ! IsGoodPlaceForWallTrap(trapRow, trapCol) || objectCol - trapCol <= 1 ){
					continue;
				}
			}else{
				trapCol = objectCol;
				trapRow = objectRow - 1;
				trapType = BO_53_WALL_TRAP_FIRES_IN_ROW;
				while( !TileBlockWalking[FineMap[ trapRow ][ trapCol ]] ){
					--trapRow;
				}
				if( ! IsGoodPlaceForWallTrap(trapRow, trapCol) || objectRow - trapRow <= 1 ){
					continue;
				}
			}
			AddObject(trapType, trapRow, trapCol);
			uint trapIndex = ObjectsMap[ trapRow ][ trapCol ] - 1;
			Object& trap = Objects[trapIndex];
			trap.TrapedObjectRow = objectRow;
			trap.TrapedObjectCol = objectCol;
			object.IsTrapped = true;
		}
	}
	#endif
}

//----- (0044CE97) --------------------------------------------------------
bool __fastcall IsGoodPlaceForWallTrap(int row, int col)
{
	int cell = 112 * row + col;
	if( FlagMap[ 0 ][ cell ] & CF_8_FORBIDDEN ){
		return false;
	}else{
		return IsWall[FineMap[ 0 ][ cell ]] != 0;
	}
}

//----- (0044CEBB) --------------------------------------------------------
void TrapingChestsWithContactTrap()
{
	for( int objectCol = 0; objectCol < 112; objectCol++ ){
		for( int objectRow = 0; objectRow < 112; objectRow++ ){

			int objectNum = ObjectsMap[ objectRow ][ objectCol ];
			if( objectNum <= 0 ){
				continue;
			}
			int objectIndex = objectNum - 1;
			Object& object = Objects[objectIndex];
			if( object.BaseObjectIndex >= BO_5_SMALL_CHEST && object.BaseObjectIndex <= BO_7_LARGE_CHEST && !object.IsTrapped && RNG(100) < 10 ){
				(int&)object.BaseObjectIndex += 63;
				object.IsTrapped = true;
				uchar rndFactor;
				if( Dungeon->genType == DT_2_CATACOMB ){
					rndFactor = 2;
				}else{
					rndFactor = 6;
				}
				object.OpenState = RNG(rndFactor);
			}
		}
	}
}

//----- (0044CF4F) --------------------------------------------------------
void __fastcall LoadMapObjects(uchar* dun, int startRow, int startCol)
{
	InitObjFlag = TRUE;

	uchar* data = dun;
	int width = *data;
	data += 2;
	int height = *data;
	// Skip map + height word
	int mapoff = ((width * height) << 1) + 2;
	// Convert to index mini tile level instead of mega
	width = width << 1;
	height = height << 1;
	// Skip treasure and monster map
	mapoff += ((width * height) << 2);
	data += mapoff;

	for( int colOfs = 0; colOfs < height; ++colOfs) {
		for( int rowOfs = 0; rowOfs < width; ++rowOfs) {
			if( *data != 0 ){
				int objectType = *data;
				int row = rowOfs + MapDirty_16 + startRow;
				int col = colOfs + MapDirty_16 + startCol;
				AddObject(BaseObjectsTableIndexes[objectType], row, col);
			}
			data += 2;
		}
	}

	InitObjFlag = FALSE;
}

//----- (0044CFD6) --------------------------------------------------------
void AddDiabloLevelObjects() // AddDiabObjs
{
	int v0;   // eax@1
	char* v1; // esi@1
	int v2;   // eax@1
	char* v3; // esi@1
	int v4;   // eax@1
	char* v5; // esi@1
	v0 = (int)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab1_cl.DUN" : "Levels\\L4Data\\diab1.DUN");
	v1 = (char*)v0;
	AddDiabloDunObject(v0, 2 * DiabloQuad1row, 2 * DiabloQuad1col, DiabloQuad2row, DiabloQuad2col, 11, 12, 1);
	FreeMem(v1);
	v2 = (int)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab2a_cl.DUN" : "Levels\\L4Data\\diab2a.DUN");
	v3 = (char*)v2;
	AddDiabloDunObject(v2, 2 * DiabloQuad2row, 2 * DiabloQuad2col, DiabloQuad3row, DiabloQuad3col, 11, 11, 2);
	FreeMem(v3);
	v4 = (int)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L4Data\\diab3a_cl.DUN" : "Levels\\L4Data\\diab3a.DUN");
	v5 = (char*)v4;
	AddDiabloDunObject(v4, 2 * DiabloQuad3row, 2 * DiabloQuad3col, DiabloQuad4row, DiabloQuad4col, 9, 9, 3);
	FreeMem(v5);
}

//----- (0044D093) --------------------------------------------------------
int __fastcall AddDiabloDunObject(int a1, int a2, int aCol, int a4, int a5, int a6, int a7, int a8)
{
	int result; // eax@1
	int v9;     // edx@1
	int v10;    // ecx@1
	int v11;    // esi@1
	int v12;    // ebx@2
	int i;      // edi@3
	int v14;    // edx@1
	int v15;    // ecx@1
	int v16;    // eax@1
	int v17;    // eax@5
	int v18;    // [sp+10h] [bp-Ch]@1
	int v19;    // [sp+Ch] [bp-10h]@1
	int v20;    // [sp+8h] [bp-14h]@1
	int v21;    // [sp+14h] [bp-8h]@1
	int v22;    // [sp+18h] [bp-4h]@2
	v18 = a2;
	InitObjFlag = 1;
	v14 = *(uchar*)a1;
	v15 = a1 + 2;
	v11 = 2 * *(uchar*)v15;
	v16 = v14 * *(uchar*)v15;
	v9 = 2 * v14;
	v19 = v9;
	v20 = v11;
	result = 2 * v16 + 2 + 4 * v9 * v11;
	v10 = result + v15;
	v21 = v10;
	if( v11 > 0 ){
		v22 = -16 - aCol;
		v12 = a5;
		aCol += 16;
		do{
			for( i = 0; i < v9; v21 = v10 ){
				if( *(uchar*)v10 ){
					AddObject(BaseObjectsTableIndexes[*(uchar*)v10], i + v18 + 16, aCol);
					v17 = ObjIndex(i + v18 + 16, aCol);
					SetChangeRect(v17, a4, v12, a4 + a6, v12 + a7, a8);
					v10 = v21;
					v9 = v19;
					v11 = v20;
				}
				v10 += 2;
				++i;
			}
			++aCol;
			result = aCol + v22;
		}while( aCol + v22 < v11 );
	}
	InitObjFlag = 0;
	return result;
}

//----- (0044D16B) --------------------------------------------------------
void __fastcall PutCryptQuestBookOnMap( int bookIndex )
{
	for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
		bool coordinateIsCorrect = true;
		int row112 = RNG(80) + 16;
		int col112 = RNG(80) + 16;

		for( int colOffset = -3; colOffset <= 3; colOffset++ ){
			for( int rowOffset = -3; rowOffset <= 3; rowOffset++ ){
				if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
					coordinateIsCorrect = false;
				}
			}
		}

		if( coordinateIsCorrect ){
			AddQuestBook86(BO_86_QUEST_BOOK, bookIndex, row112, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 2, col112 + 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 2, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 1, col112 - 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 1, col112 - 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 2, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 2, col112 + 1);
			break;
		}
	}
}

//----- (0044D249) --------------------------------------------------------
void PutQuestBookOnMap()
{
	for( int currentTryIndex = 0; currentTryIndex < 20000; currentTryIndex++ ){
		bool coordinateIsCorrect = true;
		int row112 = RNG(80) + 16;
		int col112 = RNG(80) + 16;

		for( int colOffset = -2; colOffset <= 2; colOffset++ ){
			for( int rowOffset = -3; rowOffset <= 3; rowOffset++ ){
				if( !IsCellFree(rowOffset + row112, colOffset + col112) ){
					coordinateIsCorrect = false;
				}
			}
		}

		if( coordinateIsCorrect ){
			AddObject(BO_86_QUEST_BOOK, row112, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 2, col112 + 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 2, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 - 1, col112 - 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 1, col112 - 1);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 2, col112);
			AddObject(BO_87_ON_FLOOR_TORCH, row112 + 2, col112 + 1);
			break;
		}
	}
}

//----- (0044D322) --------------------------------------------------------
void AddHellDecor()
{
	AddHellArch(6);
	AddRandFromRangeObjectsOnMap(2, 6, BO_29_BURNCROS);
	AddRandFromRangeObjectsOnMap(2, 6, BO_30_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_31_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_32_TNUDEM);
	AddRandFromRangeObjectsOnMap(2, 6, BO_33_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_34_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_35_TNUDEW);
	AddRandFromRangeObjectsOnMap(2, 6, BO_67_DECAPITATED_BODY);
	AddRandFromRangeObjectsOnMap(1, 3, BO_80_CAULDRON);
}

//----- (0044D399) --------------------------------------------------------
void __fastcall AddHellArch(int a1)
{
	int col; // ebx@1
	char *v2; // esi@2
	int row; // edi@2
	int v4; // eax@9
	int v5; // eax@10
	int v6; // eax@17
	int range; // [sp+Ch] [bp-Ch]@1
	int row_2; // [sp+10h] [bp-8h]@2
	int col_2; // [sp+14h] [bp-4h]@1

	col_2 = 0;
	range = a1;
	col = 16;
	do{
		row_2 = 0;
		v2 = (char *) Map + col_2;
		row = 17;
		do{
			if( (*v2 == 1 || *v2 == 2) && !RNG(range) && IsCoordinateNotNearAloneRoom( row_2, col_2 ) ){
				if( *v2 != 1 || v2[ 40 ] != 6 ){
					if( *v2 == 2 && v2[ 1 ] == 6 ){
						v6 = RNG(2);
						if( v6 ){
							if( v6 == 1 ){
								AddObject( BO_39_TSOUL, row - 1, col );
							}
						}else{
							AddObject( BO_38_TSOUL, row - 1, col );
						}
					}
				}else{
					v4 = RNG(3);
					if( v4 ){
						v5 = v4 - 1;
						if( v5 ){
							if( v5 == 1 ){
								AddObject( BO_40_TSOUL, row, col );
							}
						}else{
							AddObject( BO_37_TSOUL, row, col );
						}
					}else{
						AddObject( BO_36_TSOUL, row, col );
					}
				}
			}
			++row_2;
			row += 2;
			v2 += 40;
		} while( row < 97 );
		++col_2;
		col += 2;
	} while( col < 96 );
}

//----- (0044D471) --------------------------------------------------------
void AddLazarusVileStand()
{
	int row; // edi@2
	int col; // esi@2
	int v2; // ebx@2
	int v3; // edi@11
	int v4; // [sp+Ch] [bp-Ch]@1
	int v5; // [sp+10h] [bp-8h]@3
	char v6; // [sp+17h] [bp-1h]@2

	v4 = 0;
	do{
		v6 = 1;
		row = RNG(80) + 16;
		col = RNG(80) + 16;
		v2 = -3;
		do{
			v5 = -2;
			do{
				if( !IsCellFree( v5 + row, v2 + col ) ){
					v6 = 0;
				}
				++v5;
			} while( v5 <= 3 );
			++v2;
		} while( v2 <= 3 );
		if( v6 ){
			AddObject( BO_95_VILE_STAND, row, col );
			AddObject( BO_30_TNUDEM, row, col + 2 );
			AddObject( BO_87_ON_FLOOR_TORCH, row + 1, col + 2 );
			AddObject( BO_31_TNUDEM, row + 2, col + 2 );
			AddObject( BO_33_TNUDEW, row, col - 2 );
			AddObject( BO_87_ON_FLOOR_TORCH, row + 1, col - 2 );
			AddObject( BO_34_TNUDEW, row + 2, col - 2 );
			v3 = row - 1;
			AddObject( BO_87_ON_FLOOR_TORCH, v3, col - 1 );
			AddObject( BO_35_TNUDEW, v3, col );
			AddObject( BO_87_ON_FLOOR_TORCH, v3, col + 1 );
			return;
		}
		++v4;
	} while( v4 <= 10000 );
	AddRandFromRangeObjectsOnMap( 1, 1, BO_95_VILE_STAND );
}

//----- (0044D57A) --------------------------------------------------------
void __fastcall SetMapObjects(int dunData, int startRow, int startCol)
{
	int dirtyBorder = Dungeon->genType != DT_0_TOWN ? 16 : 0;
	int v3; // esi@1
	char v4; // al@1
	uchar* v5; // ecx@2
	int v6; // ebx@7
	int v7; // eax@7
	int v8; // esi@7
	int v9; // ecx@7
	int v10; // esi@7
	int v11; // ecx@7
	int v12; // ebx@7
	uchar* v13; // eax@7
	int v14; // edx@8
	int v15; // edi@10
	char *v16; // ST0A_4@17
	char *v17; // eax@17
	int v18; // ecx@17
	char **v19; // edi@18
	int i; // edi@21
	char v21[ 57 ]; // [sp+Dh] [bp-65h]@1
	char arglist[ 32 ]; // [sp+46h] [bp-2Ch]@17
	int v23; // [sp+66h] [bp-Ch]@1
	uchar* v24; // [sp+6Ah] [bp-8h]@7
	int v25; // [sp+6Eh] [bp-4h]@15
	int col; // [sp+7Ah] [bp+8h]@20

	v23 = startRow;
	v3 = dunData;
	ClearAllObjects();
	InitObjFlag = 1;
	memset( v21, 0, sizeof(v21) );
	v4 = BaseObjects[ 0 ].RegularObject;
	if( BaseObjects[ 0 ].RegularObject != -1 ){
		v5 = (uchar*)&BaseObjects[ 0 ].SpriteIndex;
		do{
			if( true /*v4 == 1 && DungeonType == v5[ 3 ]*/ ){
				v21[ *v5 ] = 1;
			}
			v4 = v5[ 43 ];
			v5 += 44;
		} while( v4 != -1 );
	}
	v6 = *(uchar*) v3;
	v7 = v3 + 2;
	v8 = *(uchar*) (v3 + 2);
	v9 = v8;
	v10 = 2 * v8;
	v11 = v6 * v9;
	v12 = 2 * v6;
	v13 = (uchar*) (2 * v11 + 2 + 4 * v12 * v10 + v7);
	v24 = v13;
	if( v10 > 0 ){
		v14 = v10;
		do{
			if( v12 > 0 ){
				v15 = v12;
				do{
					if( *v13 ){
						v21[ BaseObjects[ BaseObjectsTableIndexes[ *v13 ] ].SpriteIndex ] = 1;
					}
					v13 += 2;
					--v15;
				} while( v15 );
			}
			--v14;
		} while( v14 );
	}
	v25 = 0;
	switch( Dungeon->graphType ){
	case DT_6_ABYSS: v19 = FloorObjectsNamesAbyss; break;
	case DT_5_CRYPT: v19 = FloorObjectsNamesCrypt; break;
	default: v19 = FloorObjectsNamesOriginal; break;
	}
	do{
		if( true /*v21[ v25 ]*/ ){
			v16 = *v19;
			ObjectSpriteIndexes[ ObjectsSpritesCount ] = v25;
			sprintf( arglist, "Objects\\%s.CEL", v16 );
			v17 = (char *) LoadFile( arglist );
			v18 = ObjectsSpritesCount++;
			ObjectsSprites[ v18 ] = v17;
		}
		++v25;
		++v19;
	} while( v25 < 57 );
	v25 = (int) v24;
	if( v10 > 0 ){
		col = startCol + dirtyBorder;
		do{
			for( i = 0; i < v12; ++i ){
				if( *(uchar*) v25 ){
					AddObject( BaseObjectsTableIndexes[ *(uchar*) v25 ], i + v23 + dirtyBorder, col );
				}
				v25 += 2;
			}
			++col;
			--v10;
		} while( v10 );
	}
	InitObjFlag = 0;
}

//----- (0044D6E2) --------------------------------------------------------
void __fastcall SetChangeRect( int objectIndex, int startRow40, int startCol40, int endRow40, int endCol40, int bookIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = startRow40;
	object.startCol40 = startCol40;
	object.endRow40 = endRow40;
	object.endCol40 = endCol40;
	object.bookIndex = bookIndex;
}

//----- (0044D716) --------------------------------------------------------
void __fastcall SetObjectSpeechIndex( int objectIndex, int speechIndex )
{
	Objects[objectIndex].SpeechIndex = speechIndex;
}

//----- (0044D720) --------------------------------------------------------
void __fastcall AddAncientTomeOrBookOfVileness( int objectIndex )
{
	if( Dungeon->isQuest && Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
		Objects[objectIndex].FrameIndex = 4;
	}
}

//----- (0044D740) --------------------------------------------------------
void __fastcall AddMcirl( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = GetRndSeed();
	object.InAction = 1;
	object.maybeFrameIndex = 0;
	object.readyState = 1;
}

//----- (0044D769) --------------------------------------------------------
void __fastcall SetObjectSeed2( int objectIndex )
{
	Objects[objectIndex].Seed = GetRndSeed();
}

//----- (0044D77C) --------------------------------------------------------
void __fastcall AddPedestalOfBlood( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = CurPattern.row;
	object.startCol40 = CurPattern.col;
	object.endRow40 = CurPattern.sizeRow + CurPattern.row;
	object.endCol40 = CurPattern.sizeCol + CurPattern.col;
}

//----- (0044D7B4) --------------------------------------------------------
void AddMushroomPathObject()
{
	if( ObjectsCount >= MaxObjects_127 ){
		return;
	}
	char lastObjectIndex = ObjectAvailable[0];
	char negativeObjectNumber = -(lastObjectIndex + 1);
	int row, col;
	FindFreeMapRegion(5, row, col);
	ObjectsMap[ row+1 ][ col+1 ] = negativeObjectNumber;
	ObjectsMap[ row+2 ][ col+1 ] = negativeObjectNumber;
	ObjectsMap[ row+1 ][ col+2 ] = negativeObjectNumber;
	AddObject(BO_94_MUSHROOM_PATCH, row + 2, col + 2);
}

//----- (0044D810) --------------------------------------------------------
void __fastcall FindFreeMapRegion( int radius, int& row, int& col )
{
	if( !radius ){
		return;
	}
	int tryesCount = 0;
	bool isCellBusy;
	do{
		while( true ){
			if( ++tryesCount > 1000 && radius > 1 ){
				radius--;
			}
			
			row = RNG(112);
			col = RNG(112);
			int rowOffset = 0;
			isCellBusy = false;

			if( radius <= 0 ){
				break;
			}

			while( !isCellBusy ){
				for( int colOffset = 0; colOffset < radius && !isCellBusy; colOffset++ ){
					isCellBusy = !IsCellFree(row + rowOffset, col + colOffset);
				}
				if( ++rowOffset >= radius ){
					goto LABEL_12;
				}
			}
		}
LABEL_12:
		;
	}while( isCellBusy );
}

//----- (0044D89A) --------------------------------------------------------
void AddSlainHeroOnMap()
{
	int row, col;
	FindFreeMapRegion(5, row, col);
	AddObject(BO_96_SLAIN_HERO, row + 2, col + 2);
}

//----- (0044D8C5) --------------------------------------------------------
void __fastcall AddQuestBook86( BASE_OBJECT objectType, int bookIndex, int row, int col )
{
	if( ObjectsCount < MaxObjects_127 ){
		int objectIndex = ObjectAvailable[0];
		ObjectActive[ObjectsCount] = objectIndex;
		ObjectAvailable[0] = ObjectAvailable[ MaxObjects_127 - 1 - ObjectsCount ];
		ObjectsMap[ row ][ col ] = objectIndex + 1;
		SetupObject(objectIndex, row, col, objectType);
		MaybeInitQuestBook(objectIndex, bookIndex);
		ObjectsCount++;
		Objects[objectIndex].OffsetX =  (Objects[objectIndex].Width - 64) >> 1;
	}
}

//----- (0044D946) --------------------------------------------------------
void __fastcall SetupObject(int objectIndex, int row, int col, BASE_OBJECT baseObjectIndex)
{
	Object& object = Objects[objectIndex];
	BaseObject& baseObject = BaseObjects[baseObjectIndex];
	object.BaseObjectIndex = baseObjectIndex;
	object.Row = row;
	object.Col = col;
	int index;
	for( index = 0; index < countof(ObjectSpriteIndexes) && ObjectSpriteIndexes[index] != baseObject.SpriteIndex; index++ );
	if( index == countof(ObjectSpriteIndexes) ) LoadOneObjectSprite(baseObject.SpriteIndex);
	for( index = 0; ObjectSpriteIndexes[index] != baseObject.SpriteIndex; index++ );
	object.CelFilePtr = ObjectsSprites[index]; 
	object.playAnimation = baseObject.hasAnimation;
	int frameIndex;
	if( baseObject.hasAnimation ){
		object.AnimationDelayFrameCount = baseObject.AnimationDelayFrameCount;
		object.AnimationDelayIndex = RNG(baseObject.AnimationDelayFrameCount);
		object.FramesCount = baseObject.FramesCount;
		frameIndex = RNG(baseObject.FramesCount - 1) + 1;
	}else{
		object.AnimationDelayFrameCount = 1000;
		object.AnimationDelayIndex = 0;
		object.FramesCount = baseObject.FramesCount;
		frameIndex = baseObject.AnimationDelayFrameCount;
	}
	object.FrameIndex = frameIndex;
	object.Width = baseObject.Width;
	object.isBlockWalking = baseObject.isBlockWalking;
	object.objectAllowWalking = baseObject.objectAllowWalking;
	object.field_C = baseObject.field_20;
	object.deletedObject = 0;
	object.destructable = baseObject.destructable;
	object.selectable = baseObject.selectable;
	object.InAction = 0;
	object.IsTrapped = 0;
	object.isDoor = 0;
}

//----- (0044DA68) --------------------------------------------------------
void __fastcall MaybeInitQuestBook( int objectIndex, int bookIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	Object& object = Objects[objectIndex];

	if( bookIndex <= 5 ){
		object.bookIndex = 0;
		object.MaybeSpeechIndex = bookIndex + SP_316;
		object.BookNameIndex = bookIndex + 9;
	}else{
		if( bookIndex == 6 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:		object.MaybeSpeechIndex = SP_323;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_332;		break;

			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_329;		break;

			case PC_3_MONK:				object.MaybeSpeechIndex = SP_326;		break;

			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_335;		break;
			}
		}else if( bookIndex == 7 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:			object.MaybeSpeechIndex = SP_324;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_333;		break;

			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_330;		break;

			case PC_3_MONK:				object.MaybeSpeechIndex = SP_327;		break;

			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_336;		break;
			}
		}else if( bookIndex == 8 ){
			switch (player.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:			object.MaybeSpeechIndex = SP_325;		break;

			case PC_1_ARCHER:			object.MaybeSpeechIndex = SP_334;		break;
			case PC_2_MAGE:				object.MaybeSpeechIndex = SP_331;		break;
			case PC_3_MONK:				object.MaybeSpeechIndex = SP_328;		break;
			case PC_4_ROGUE:			object.MaybeSpeechIndex = SP_337;		break;
			}
		}
		object.bookIndex = bookIndex;
		object.BookNameIndex = 15;
	}
	object.ShrineIndex = 1;
	object.FrameIndex = 5 - 2 * object.ShrineIndex;
	object.OpenState = 5 - 2 * object.ShrineIndex + 1;
}

//----- (0044DC75) --------------------------------------------------------
void __fastcall AddObject(BASE_OBJECT baseObject, int row, int col)
{
	if( ObjectsCount >= MaxObjects_127 ){
		return;
	}
	int objectIndex = ObjectAvailable[0]; /////////////////////////////

	int temp = ObjectAvailable[MaxObjects_127 - 1 - ObjectsCount];
	ObjectActive[ObjectsCount] = ObjectAvailable[0];
	ObjectAvailable[0] = temp;


	ObjectsMap[ row ][ col ] = objectIndex + 1;
	SetupObject( objectIndex, row, col, baseObject );
	switch( baseObject ){
	case BO_87_ON_FLOOR_TORCH:					PutSomeLightSorce(objectIndex, 3);					break;
	case BO_44_WTORCH4:
	case BO_45_WTORCH3:
	case BO_46_WTORCH1:
	case BO_47_WTORCH2:					PutSomeLightSorce(objectIndex, 8);					break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:					AddCatacombDoor(objectIndex, row, col, baseObject);					break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:					AddCaveDoor(objectIndex, row, col, baseObject);					break;
	case BO_41_MYTHICAL_BOOK:					AddMythicalBook(objectIndex);					break;
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST:
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:				AddChest(objectIndex, baseObject);					break;
	case BO_48_SARCOPHAGUS:					AddSarcofagusOnMap(objectIndex);					break;
	case BO_49_FLAME1:					AddFlameOnMap(objectIndex);					break;
	case BO_50_LEVER_2:					AddLever2OnMap(objectIndex);					break;
	case BO_51_MINIWATR:					Objects[objectIndex].FrameIndex = 1;					break;
	case BO_53_WALL_TRAP_FIRES_IN_ROW:
	case BO_54_WALL_TRAP_FIRES_IN_COL:					AddTrapHoleOnMap(objectIndex);					break;
	case BO_57_POD_URN_BARREL_WITH_DROP:
	case BO_58_POD_URN_BARREL_EXPLOSIVE:					AddBarrelOnMap(objectIndex);					break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:					AddShrineOnMap(objectIndex);					break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:					AddBookCaseOnMap(objectIndex);					break;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:
	case BO_79_GOAT_SHRINE:
	case BO_80_CAULDRON:
	case BO_82_FOUNTAIN_OF_TEARS:
	case BO_66_BLOOD_FOUNTAIN:					SetObjectSeed(objectIndex);					break;
	case BO_67_DECAPITATED_BODY:					AddDecapitatedBody(objectIndex);					break;
	case BO_76_PURIFYING_SPRING:					AddFontain(objectIndex);					break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:					AddArmorRack(objectIndex);					break;
	case BO_81_MURKY_POOL:					AddFontain(objectIndex);					break;
	case BO_84_MCIRL:
	case BO_85_MCIRL:					AddMcirl(objectIndex);					break;
	case BO_86_QUEST_BOOK:					AddQuestBook(objectIndex);					break;
	case BO_91_BURNCROS:				
	case BO_26_BURNCROS:
		SetObjectSeed2(objectIndex);					
		PutSomeLightSorce(objectIndex, 5);
		break;
	case BO_73_PEDESTAL_OF_BLOOD:					AddPedestalOfBlood(objectIndex);					break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:					AddWeaponRack(objectIndex);					break;
	case BO_30_TNUDEM:					AddTNude(objectIndex);					break;
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:					AddAncientTomeOrBookOfVileness(objectIndex);					break;
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:					
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:					AddChurchDoor(objectIndex, row, col, baseObject);					break;
	case BO_0_L1BRAZ:
	case BO_3_TORCH:
	case BO_8_L1BRAZ:
	case BO_65_CANDLE2:
	case BO_9_CANDLE2:					PutSomeLightSorce(objectIndex, 5);				break;
	default:																break;
	}
	ObjectsCount++;
	Objects[objectIndex].OffsetX = (Objects[objectIndex].Width - 64) / 2;
}

//----- (0044DF7B) --------------------------------------------------------
void __fastcall AddChurchDoor( int objectIndex, int row, int col, int baseObjectIndex )
{
	Object& object = Objects[objectIndex];
	int cell = 112*row + col;
	object.isDoor = 1;
	if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
		object.MaybeSpeechIndex = FineMap[ 0 ][ cell - 1 ];
	}else{
		object.MaybeSpeechIndex = FineMap[ 0 ][ cell - 112 ];
	}
	object.ShrineIndex = FineMap[ 0 ][ cell ];
	object.OpenState = OS_0_CLOSED;
}

//----- (0044DFDA) --------------------------------------------------------
void __fastcall AddMythicalBook( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.startRow40 = CurPattern.row;
	object.startCol40 = CurPattern.col;
	object.endRow40 = CurPattern.sizeRow + CurPattern.row + 1;
	object.endCol40 = CurPattern.sizeCol + CurPattern.col + 1;
	object.maybeFrameIndex = object.FrameIndex + 1;
}

//----- (0044E023) --------------------------------------------------------
void __fastcall AddChest( int objectIndex, int baseObject )
{
	Object& object = Objects[objectIndex];
	if( !RNG(2) ){
		object.FrameIndex += 3;
	}
	object.Seed = GetRndSeed();
	if( baseObject == BO_5_SMALL_CHEST || baseObject == BO_68_TRAPED_SMALL_CHEST ){
		if( Dungeon->isQuest ){
			object.ShrineIndex = 1;
		}else{
			object.ShrineIndex = RNG(2);
		}
	}else if( baseObject == BO_6_CHEST || baseObject == BO_69_TRAPED_CHEST ){
		if( Dungeon->isQuest ){
			object.ShrineIndex = 2;
		}else{
			object.ShrineIndex = RNG(3);
		}
	}else if( baseObject == BO_7_LARGE_CHEST || baseObject == BO_70_TRAPED_LARGE_CHEST ){
		if( Dungeon->isQuest ){
			object.ShrineIndex = 3;
		}else{
			object.ShrineIndex = RNG(4);
		}
	}
	object.MaybeSpeechIndex = RNG(8);
}

//----- (0044E0D9) --------------------------------------------------------
void __fastcall AddCatacombDoor( int objectIndex, int row, int col, int baseObject )
{
	Object& object = Objects[objectIndex];
	object.isDoor = 1;
	int dungeonMapNumber;
	if( baseObject == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
		dungeonMapNumber = 538;
	}else{
		dungeonMapNumber = 540;
	}
	SetDungeonMapNumber(row, col, dungeonMapNumber);
	object.OpenState = 0;

}

//----- (0044E114) --------------------------------------------------------
void __fastcall AddCaveDoor( int objectIndex, int row, int col, int baseObject )
{
	Object& object = Objects[objectIndex];
	object.isDoor = 1;
	int dungeonMapNumber;
	if( baseObject == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
		dungeonMapNumber = 531;
	}else{
		dungeonMapNumber = 534;
	}
	SetDungeonMapNumber(row, col, dungeonMapNumber);
	object.OpenState = 0;
}

//----- (0044E14F) --------------------------------------------------------
void __fastcall AddSarcofagusOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	ObjectsMap[ object.Row ][ object.Col - 1 ] = -1 - objectIndex;
	object.ShrineIndex = RNG(10);
	object.Seed = GetRndSeed();
	if( object.ShrineIndex >= 8 ){
		object.MaybeSpeechIndex = AddAmbushMonster();
	}
}

//----- (0044E1A0) --------------------------------------------------------
void __fastcall AddFlameOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.ShrineIndex = FlameGroupIndex;
	object.MaybeSpeechIndex = 0;
	object.BookNameIndex = FlameOrientation;
	object.OpenState = 0;
}

//----- (0044E1C8) --------------------------------------------------------
void __fastcall AddLever2OnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.ShrineIndex = FlameGroupIndex;
	object.MaybeSpeechIndex = 49;
}

//----- (0044E1E1) --------------------------------------------------------
void __fastcall AddTrapHoleOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	int dungeonLevel = Dungeon->level;

	if( dungeonLevel > 16 && dungeonLevel < 21 ){
		dungeonLevel -= 4;
	}else if( dungeonLevel >= 21 ){
		dungeonLevel -= 8;
	}
	int spellIndex = MI_0_NONE_ARROW;
	switch( RNG(dungeonLevel / 3 + 1) ){
	case 0:	spellIndex = MI_0_NONE_ARROW; break;
	case 1:	spellIndex = MI_1_FIREBOLT;   break;
	case 2:	spellIndex = MI_7_LIGHTNING;  break;
	}
	object.TrapSpellIndex = spellIndex;
	object.OpenState = 0;
}

//----- (0044E25F) --------------------------------------------------------
void __fastcall PutSomeLightSorce( int objectIndex, int lightLevel )
{
	Object& object = Objects[objectIndex];
	if( InitObjFlag ){
		DoLighting(object.Row, object.Col, lightLevel, -1);// и у всех объектов лайт индекс будет -1?
		object.ShrineIndex = -1;
	}else{
		object.ShrineIndex = 0;
	}
}

//----- (0044E296) --------------------------------------------------------
void __fastcall AddBarrelOnMap( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.ShrineIndex = 0;
	object.Seed = GetRndSeed();
	object.MaybeSpeechIndex = RNG(10);;
	object.BookNameIndex = RNG(3);
	if( object.MaybeSpeechIndex >= 8 ){
		object.OpenState = AddAmbushMonster();
	}
}

//----- (0044E2E4) --------------------------------------------------------
void __fastcall AddShrineOnMap( int objectIndex )
{
	uchar validShrineMask[36];
	Object& object = Objects[objectIndex];
	object.InAction = true;

	for( int shrineIndex = 0; shrineIndex < 34; shrineIndex++ ){
		if( Dungeon->level >= MinShrineDungeonLevels[shrineIndex] && Dungeon->level <= MaxShrineDungeonLevels[shrineIndex] ){
			validShrineMask[shrineIndex] = true;
		}else{
			validShrineMask[shrineIndex] = false;
		}
		// 0044E331
		if( MaxCountOfPlayersInGame == 1 ){
			// 0044E344
			if( ShrineAppearsMode[shrineIndex] == 2 ){
				validShrineMask[shrineIndex] = false;
			}
		}else{
			// 0044E335
			if( ShrineAppearsMode[shrineIndex] == 1 ){
				validShrineMask[shrineIndex] = false;
			}
		}

	}

	int shrineIndex;
	do{
		shrineIndex = RNG(34);
	}while( !validShrineMask[shrineIndex] );

	object.ShrineIndex = shrineIndex;
	if( RNG(2) ){
		object.FrameIndex = 12;
		object.FramesCount = 22;
	}
}

//----- (0044E394) --------------------------------------------------------
void __fastcall AddBookCaseOnMap( int objectIndex )
{
	Objects[objectIndex].Seed = GetRndSeed();
	Objects[objectIndex].InAction = 1;
}

//----- (0044E3B1) --------------------------------------------------------
void __fastcall SetObjectSeed( int objectIndex )
{
	Objects[objectIndex].Seed = GetRndSeed();
}

//----- (0044E3C4) --------------------------------------------------------
void __fastcall AddArmorRack( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !IsNotArmorPlaced && !(Dungeon->isQuest && is(Dungeon, DUN_41_FLESHDOOM, DUN_42_WIELDER, DUN_43_HEPHASTO, DUN_83_BLOOD_KEEP, DUN_84_ARMORIES_OF_HELL )) ){
		object.playAnimation = 2;
		object.selectable = 0;
	}
	object.Seed = GetRndSeed();
}

//----- (0044E3F6) --------------------------------------------------------
void __fastcall AddFontain( int objectIndex )
{
	Object& object = Objects[objectIndex];
	int row = object.Row;
	int col = object.Col;
	char negativeObjectNumber = -((char)objectIndex + 1);
	ObjectsMap[ row ][ col-1 ] = negativeObjectNumber;
	ObjectsMap[ row-1 ][ col ] = negativeObjectNumber;
	ObjectsMap[ row-1 ][ col-1 ] = negativeObjectNumber;
	object.Seed = GetRndSeed();
}

//----- (0044E42F) --------------------------------------------------------
void __fastcall AddDecapitatedBody( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = GetRndSeed();
	object.FrameIndex = RNG(8) + 1;
	object.InAction = 1;
}

//----- (0044E45D) --------------------------------------------------------
void __fastcall AddQuestBook( int objectIndex )
{
	int objectIndex_1; // esi@1
	int rng_0_2; // eax@1
	int v4; // ecx@1
	int v6; // eax@2
	int v7; // edx@8

	objectIndex_1 = objectIndex;
	SetRndSeed( DungeonRandSeed[ DUN_16_HELL_4 ] );
	rng_0_2 = RNG(3);
	v4 = objectIndex_1;
	Objects[ objectIndex_1 ].TrapedObjectRow = rng_0_2;
	if( Dungeon == DUN_4_CHURCH_4 ){
		v6 = QuestBooksSpeeches[ 3 * rng_0_2 ];
	LABEL_7:
		Objects[ v4 ].TrapedObjectCol = v6;
		goto LABEL_8;
	}
	if( Dungeon == DUN_8_CATACOMB_4 ){
		v6 = QuestBooksSpeeches[ 3 * rng_0_2 + 1 ];
		goto LABEL_7;
	}
	if( Dungeon == DUN_12_CAVE_4 ){
		v6 = QuestBooksSpeeches[ 3 * rng_0_2 + 2 ];
		goto LABEL_7;
	}
LABEL_8:
	Objects[ v4 ].TrapSpellIndex = Dungeon->level / 4 + 3 * Objects[ v4 ].TrapedObjectRow - 1;
	v7 = 2 * Objects[ v4 ].TrapedObjectRow;
	Objects[ v4 ].FrameIndex = 5 - v7;
	Objects[ v4 ].OpenState = 5 - v7 + 1;

	#ifdef REFACKTOR
	Object& object = Objects[ objectIndex ];
	SetRndSeed(DungeonRandSeed[DUN_16_HELL_4]);
	int rng_0_2 = RNG(3);
	object.ShrineIndex = rng_0_2;
	object.MaybeSpeechIndex = QuestBooksSpeeches[3 * rng_0_2 + (Dungeon->level / 4) - 1];
	object.BookNameIndex = (Dungeon->level / 4) + 3 * rng_0_2 - 1;
	object.FrameIndex = 5 - 2 * rng_0_2;
	object.OpenState = 5 - 2 * rng_0_2 + 1;
	#endif
}

//----- (0044E4F2) --------------------------------------------------------
void __fastcall AddWeaponRack( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !IsWeapondStandNotPlaced && !(Dungeon->isQuest && is(Dungeon, DUN_41_FLESHDOOM, DUN_42_WIELDER, DUN_43_HEPHASTO, DUN_83_BLOOD_KEEP, DUN_84_ARMORIES_OF_HELL)) ){
		object.playAnimation = 2;
		object.selectable = 0;
	}
	object.Seed = GetRndSeed();
}

//----- (0044E524) --------------------------------------------------------
void __fastcall AddTNude( int objectIndex )
{
	Object& object = Objects[objectIndex];
	object.Seed = GetRndSeed();
	object.FrameIndex = RNG(4) + 1;
	object.InAction = 1;
}

//----- (0044E552) --------------------------------------------------------
void __fastcall LightObjectAction( int objectIndex, int lightRadius )
{
	Object& object = Objects[objectIndex];

	if( object.startRow40 == -1 ){// источник света выключен
		return;
	}
	int needOnLight = 0;
	int lightRadiusPlusTen = lightRadius + 10;
	if( !FullLighting ){
		for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
			Player& player = Players[playerIndex];
			if( player.IsExists && Dungeon == player.dungeon ){
				int deltaRow = abs(player.Row - object.Row);
				int deltaCol = abs(player.Col - object.Col);
				if( deltaRow < lightRadiusPlusTen && deltaCol < lightRadiusPlusTen ){
					needOnLight = 1;
					break;
				}
			}
		}
	}
	if( needOnLight ){
		if( !object.startRow40  ){
			object.lightIndex = AddLight(object.Row, object.Col, lightRadius);
		}
		object.startRow40 = 1;
	}else{
		if( object.startRow40 == 1 ){
			LightOff(object.lightIndex);
		}
		object.startRow40 = 0;// источник света далеко от игрока, нет смысла использовать
	}
}

//----- (0044E642) --------------------------------------------------------
void __fastcall ReadBookInLazarusAltar(int objectIndex)
{
	Object& object = Objects[ objectIndex ];
	Player& player = Players[ CurrentPlayerIndex ];

	if( player.Row != object.Row || player.Col != object.Col ){
		if( object.BaseObjectIndex == BO_84_MCIRL ){
			object.FrameIndex = 1;
		}else if( object.BaseObjectIndex == BO_85_MCIRL ){
			object.FrameIndex = 3;
		}
		object.maybeFrameIndex = 0;
	}else{
		if( object.BaseObjectIndex == BO_84_MCIRL ){
			object.FrameIndex = 2;
		}else if( object.BaseObjectIndex == BO_85_MCIRL ){
			object.FrameIndex = 4;
		}
		if( object.Row != 45 || object.Col != 47 ){
			object.maybeFrameIndex = object.Row == 26 && object.Col == 46;
		}else{
			object.maybeFrameIndex = 2;
		}

		auto teleportPad = [&](int landRow, int landCol) {
			CastMissile(player.Row, player.Col, landRow, landCol, player.dir, MI_3_PHASING, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0);
			TrackInit(false);
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
			ClearPlayerPath(CurrentPlayerIndex);
			StartStand(CurrentPlayerIndex, 0);
			};

		if( Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR && object.Row == 35 && object.Col == 36 && object.readyState == 3 && Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 < LQS_4_READ_BOOK_IN_UNHOLY_ALTAR ){
			object.maybeFrameIndex = 4;
			ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			if( Quests[ Q_15_ARCHBISHOP_LAZARUS ].status == QS_2_IN_PROGRESS ){
				Quests[ Q_15_ARCHBISHOP_LAZARUS ].status2 = LQS_4_READ_BOOK_IN_UNHOLY_ALTAR;
			}
			CastMissile( player.Row, player.Col, 35, 46, player.dir, MI_3_PHASING, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
            TrackInit( false );
			MouseButtonPressed = MBP_0_NONE; OldMouseButtonPressed = MBP_0_NONE;
			ReleaseCapture();
			ClearPlayerPath( CurrentPlayerIndex );
			StartStand( CurrentPlayerIndex, 0 );
		}
		else if (Dungeon == DUN_103_ALTAR_OF_IMMOLATION) {
			//1
			if (object.Row == 58 && object.Col == 56) { // teleport from...
				teleportPad(52, 57);
			}
			else if (object.Row == 51 && object.Col == 56) { // teleport from...
				teleportPad(59, 57);
			}
			//2
			else if (object.Row == 18 && object.Col == 89) { // teleport from...
				teleportPad(90, 50);
			}
			else if (object.Row == 89 && object.Col == 49) { // teleport from...
				teleportPad(19, 90);
			}
			//3
			else if (object.Row == 58 && object.Col == 18) { // teleport from...
				teleportPad(51, 49);
			}
			else if (object.Row == 50 && object.Col == 48) { // teleport from...
				teleportPad(59, 19);
			}
		}
		else if (Dungeon == DUN_104_NETHERWORLD_GATES) {
			if (object.Row == 32 && object.Col == 22) { // teleport from...
				teleportPad(66, 55);
			}
			else if (object.Row == 65 && object.Col == 54) { // teleport from...
				teleportPad(33, 23);
			}
			
		}
		else if (Dungeon == DUN_113_THE_MAZE) {
			if (object.Row == 40 && object.Col == 42) { // teleport from...
				teleportPad(48, 55);// to... start
			}
			else if (object.Row == 42 && object.Col == 42) { // teleport from...
				teleportPad(40, 70);// 2nd room start
			}
			else if (object.Row == 40 && object.Col == 36) { // teleport from...
				teleportPad(48, 55);
			}
			else if (object.Row == 40 && object.Col == 34) { // teleport from...
				teleportPad(78, 44);// 3rd room start
			}
			else if (object.Row == 40 && object.Col == 78) { // teleport from...
				teleportPad(48, 55);
			}
			else if (object.Row == 40 && object.Col == 81) { // teleport from...
				teleportPad(88, 44);// 4th room
			}
			else if (object.Row == 18 && object.Col == 90) { // teleport from...
				teleportPad(34, 72);// 5th room
			}
			else if (object.Row == 18 && object.Col == 78) { // teleport from...
				teleportPad(48, 55);// start
			}
			else if (object.Row == 34 && object.Col == 70) { // teleport from...
				teleportPad(32, 63);
			}
			else if (object.Row == 34 && object.Col == 63) { // teleport from...
				teleportPad(48, 55);// start
			}
			else if (object.Row == 46 && object.Col == 26) { // teleport from...
				teleportPad(54, 18);// room final
			}
			else if (object.Row == 18 && object.Col == 18) { // teleport from...
				teleportPad(40, 70);			
			}
			else if (object.Row == 78 && object.Col == 36) { // teleport from...finish
				teleportPad(48, 55);// to start
			}
			else if (object.Row == 46 && object.Col == 20) { // teleport from...
				teleportPad(78, 44);// 3rd room start
			}
			else if (object.Row == 72 && object.Col == 72) { // teleport from...
				teleportPad(40, 70);
			}
			else if (object.Row == 18 && object.Col == 26) { // teleport from...
				teleportPad(18, 54);
			}
			else if (object.Row == 46 && object.Col == 23) { // teleport from...
				teleportPad(32, 63);
			}
		}
	}
}

//----- (0044E7BC) --------------------------------------------------------
void __fastcall StopAnimationAfterComplete( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.FrameIndex == object.FramesCount ){
		object.AnimationDelayIndex = 0;
		object.AnimationDelayFrameCount = 1000;
	}
}

//----- (0044E7DF) --------------------------------------------------------
void __fastcall DoorAction( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.OpenState ){
		int cell = object.Col + 112 * object.Row;
		if( !MonsterMap[ 0 ][ cell ] && !ItemMap[0][cell] && !DeathMonstersMap[0][cell] && !PlayerMap[0][cell] ){
			object.OpenState = OS_1_OPEN;
			object.selectable = 2;
		}else{
			object.OpenState = OS_2_BLOCKED;
			object.selectable = 0;
		}
		object.objectAllowWalking = 1;
	}else{
		object.selectable = 3;
		if( object.BaseObjectIndex != BO_74_CAVE_DOOR_MAIN_DIAGONAL && object.BaseObjectIndex != BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			object.objectAllowWalking = 0;
		}
	}
}

//----- (0044E870) --------------------------------------------------------
void __fastcall StopAnimationAfterComplete2( int objectIndex )
{
	Object& object = Objects[ objectIndex ];
	if( object.FrameIndex == object.FramesCount ){
		object.playAnimation = 0;
	}
}

//----- (0044E889) --------------------------------------------------------
void __fastcall FireAllSomeFlames( int baseObjectIndex, int flameGroupIndex )
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectActive[objectIndexIndex];
		Object& object = Objects[objectIndex];
		if( object.BaseObjectIndex == baseObjectIndex && object.ShrineIndex == flameGroupIndex ){
			object.OpenState = 1;
			object.playAnimation = 1;
			object.AnimationDelayFrameCount = 1;
			object.lightIndex = AddLight(object.Row, object.Col, 1);
		}
	}
}

//----- (0044E908) --------------------------------------------------------
void __fastcall ActivateFlame( int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.MaybeSpeechIndex ){
		if( !object.OpenState ){
			return;
		}
		object.FrameIndex--;
		if( object.FrameIndex == 1 ){
			object.OpenState = 0;
			LightOff(object.lightIndex);
			return;
		}
		if( object.FrameIndex <= 4 ){
			ChangeLightRadius(object.lightIndex, object.FrameIndex);
		}
		return;
	}

	if( object.OpenState ){
		if( object.FrameIndex == object.FramesCount ){
			object.FrameIndex = 11;
		}
		if( object.FrameIndex <= 5 ){
			ChangeLightRadius(object.lightIndex, object.FrameIndex);
		}
		return;
	}

	if( object.BookNameIndex == 2 ){
		for( int rowOffset = -2; rowOffset <=2; rowOffset++ ){
			int cell = 112 * (object.Row+rowOffset) + object.Col;
			if( PlayerMap[ 0 ][ cell ] /*|| MonsterMap[ 0 ][ cell ]*/ ){
				object.OpenState = 1;
				break;
			}
		}
	}else{
		for( int colOffset = -2; colOffset <=2; colOffset++ ){
			int cell = 112 * object.Row + (object.Col+colOffset);
			if( PlayerMap[ 0 ][ cell ] /*|| MonsterMap[ 0 ][ cell ]*/ ){
				object.OpenState = 1;
				break;
			}
		}
	}

	if( object.OpenState ){
		FireAllSomeFlames(object.BaseObjectIndex, object.ShrineIndex);
	}
}

//----- (0044EA33) --------------------------------------------------------
void __fastcall ActivateWallTrap( int objectIndex )
{
	#ifndef REFACTOR
	int v1; // edi@1
	char v2; // dl@1
	int trapped; // esi@2
	int baseObjectIndex; // eax@2
	int targetCol; // ebx@18
	int targetRow; // ecx@18
	int v7; // eax@18
	int v8; // ecx@20
	char *i; // edx@21
	int orient; // eax@29
	int v11; // [sp+Ch] [bp-1Ch]@18
	int v12; // [sp+14h] [bp-14h]@19
	int trapRow; // [sp+18h] [bp-10h]@18
	int trapCol; // [sp+1Ch] [bp-Ch]@18
	int v15; // [sp+20h] [bp-8h]@19
	int targetRow_1; // [sp+24h] [bp-4h]@18

	v1 = objectIndex;
	v2 = 0;
	if( Objects[ objectIndex ].OpenState ){
		return;
	}
	trapped = ObjectsMap[ Objects[ v1 ].TrapedObjectRow ][ Objects[ v1 ].TrapedObjectCol ] - 1;
	baseObjectIndex = Objects[ trapped ].BaseObjectIndex;
	if( baseObjectIndex <= 7 ){
		if( baseObjectIndex < 4 ){
			if( baseObjectIndex <= 0 || baseObjectIndex > 2 ){
				goto LABEL_17;
			}
			goto LABEL_13;
		}
	LABEL_15:
		if( Objects[ trapped ].selectable ){
			goto LABEL_17;
		}
		goto LABEL_16;
	}
	if( baseObjectIndex == 28 ){
		goto LABEL_15;
	}
	if( baseObjectIndex <= 41 ){
		goto LABEL_17;
	}
	if( baseObjectIndex <= 43 ){
		goto LABEL_13;
	}
	if( baseObjectIndex == 48 ){
		goto LABEL_15;
	}
	if( baseObjectIndex > 73 && baseObjectIndex <= 75 ){
	LABEL_13:
		if( !Objects[ trapped ].OpenState ){
			goto LABEL_17;
		}
	LABEL_16:
		v2 = 1;
	}
LABEL_17:
	if( v2 ){
		Objects[ v1 ].OpenState = 1;
		targetCol = Objects[ trapped ].Col;
		targetRow = Objects[ trapped ].Row;
		trapRow = Objects[ v1 ].Row;
		trapCol = Objects[ v1 ].Col;
		v7 = targetCol - 1;
		targetRow_1 = Objects[ trapped ].Row;
		v11 = targetCol + 1;
		if( targetCol - 1 <= targetCol + 1 ){
			v12 = targetRow - 1;
			v15 = targetRow + 1;
			do{
				v8 = v12;
				if( v12 <= v15 ){
					for( i = &PlayerMap[ v12 ][ v7 ]; ; i += 112 ){
						if( *i ){
							targetRow_1 = v8;
							targetCol = v7;
						}
						if( ++v8 > v15 ){
							break;
						}
					}
				}
				++v7;
			}while( v7 <= v11 );
			targetRow = targetRow_1;
		}
		if( !deltaload ){
			orient = OrientationToTarget( trapRow, trapCol, targetRow, targetCol );
			CastMissile( trapRow, trapCol, targetRow_1, targetCol, orient, Objects[ v1 ].TrapSpellIndex, CT_1_MONSTER_AND_TRAP, -1, 0, 0, 0 );
			PlayLocalSound( 78, Objects[ trapped ].Row, Objects[ trapped ].Col );
		}
		Objects[ trapped ].IsTrapped = 0;
	}
	#else
	Object& trap = Objects[objectIndex];

	if( trap.OpenState ){
		return;
	}
	int trappedObjectIndex = ObjectsMap[ trap.TrapedObjectRow ][ trap.TrapedObjectCol ] - 1;
	//if( trappedObjectIndex == -1 ){
	//	return;
	//}
	Object& trappedObject = Objects[trappedObjectIndex];
	int baseObjectIndex = trappedObject.BaseObjectIndex;
	if( baseObjectIndex != BO_1_CHURCH_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL &&
		baseObjectIndex != BO_42_CATACOMB_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL &&
		baseObjectIndex != BO_74_CAVE_DOOR_MAIN_DIAGONAL &&
		baseObjectIndex != BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			if( trappedObject.selectable ){
				return;
			}
	} 
	if( !trappedObject.OpenState ){
		trap.OpenState = 1;
	}
	int targetCol = trappedObject.Col;
	int targetRow = trappedObject.Row;
	for( int colOffset = -1; colOffset <=1; colOffset++ ){
		for( int rowOffset = -1; rowOffset <=1; rowOffset++ ){
			int cell = 112 * (targetRow+rowOffset) + (targetCol+colOffset);
			if( PlayerMap[ 0 ][ cell ] ){
				targetCol += colOffset;
				targetRow += rowOffset;
				break;
			}
		}
	}
	if( !deltaload ){
		int orient = OrientationToTarget(trap.Row, trap.Col, targetRow, targetCol);
		CastMissile(trap.Row, trap.Col, targetRow, targetCol, orient, trap.TrapSpellIndex, CT_1_MONSTER_AND_TRAP, -1, 0, 0, missile.spellIndex );
		PlayLocalSound(S_78_I_TRAP, trappedObject.Row, trappedObject.Col);
	}
	trappedObject.IsTrapped = 0;
	#endif
}

//----- (0044EB90) --------------------------------------------------------
void ProcessObjects()
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectActive[objectIndexIndex];
		Object& object = Objects[objectIndex];
		int baseObjectIndex = object.BaseObjectIndex;

		switch (object.BaseObjectIndex ){
		case BO_26_BURNCROS:
		case BO_29_BURNCROS:
		case BO_91_BURNCROS:
			LightObjectAction(objectIndex, 10);
			BurnCrossDamage(objectIndex);
			break;
		case BO_20_CRUCIFIED_SKELETON:
		case BO_21_CRUCIFIED_SKELETON_2:
		case BO_22_CRUCIFIED_SKELETON_3:
		case BO_57_POD_URN_BARREL_WITH_DROP:
		case BO_58_POD_URN_BARREL_EXPLOSIVE:
		case BO_59_SHRINE:
		case BO_60_SHRINE_2:
			StopAnimationAfterComplete(objectIndex);
			break;
		case BO_54_WALL_TRAP_FIRES_IN_COL:
		case BO_53_WALL_TRAP_FIRES_IN_ROW:
			ActivateWallTrap(objectIndex);
			break;
		case BO_49_FLAME1:
			ActivateFlame(objectIndex);
			break;
		case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
		case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
		case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
		case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
		case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
			DoorAction(objectIndex);
			break;
		case BO_84_MCIRL:
		case BO_85_MCIRL:
			ReadBookInLazarusAltar(objectIndex);
			break;
		case BO_48_SARCOPHAGUS:
			StopAnimationAfterComplete2(objectIndex);
			break;
		case BO_0_L1BRAZ:
			LightObjectAction(objectIndex, 10);
			break;
		case BO_44_WTORCH4:
		case BO_45_WTORCH3:
		case BO_46_WTORCH1:
		case BO_47_WTORCH2:
			LightObjectAction(objectIndex, 8);
			break;
		case BO_3_TORCH:
		case BO_9_CANDLE2:
		case BO_65_CANDLE2:
			LightObjectAction(objectIndex, 5);
			break;
		case BO_87_ON_FLOOR_TORCH:
			LightObjectAction(objectIndex, 3);
			break;
		}

		//if( object.playAnimation ){// old check, without battle trance
		if (object.playAnimation && (!BattleTrance || MainDrawCount & 1)) {
			object.AnimationDelayIndex++;
			if (object.AnimationDelayIndex >= object.AnimationDelayFrameCount) {  // TODO: change to > and fix the BaseObjects animDelay data
				object.AnimationDelayIndex = 0;
				object.FrameIndex++;
				if (object.FrameIndex > object.FramesCount) {
					object.FrameIndex = 1;
				}
			}
		}
	}

	int objectIndex2 = 0;
	while( objectIndex2 < ObjectsCount ){
		int objectIndex = ObjectActive[objectIndex2];
		if( Objects[objectIndex].deletedObject ){
			DeleteObjectOnMap(objectIndex, objectIndex2);
			objectIndex2 = 0;
		}else{
			objectIndex2++;
		}
	}
}

//----- (0044ED4F) --------------------------------------------------------
void __fastcall DeleteObjectOnMap( int deletedObject, int a2 )
{
	int result;         // eax@1
	unsigned __int8 v3; // zf@1
	char v4;            // sf@1
	ObjectsMap[ Objects[deletedObject].Row ][ Objects[deletedObject].Col ] = 0;
	result = ObjectsCount - 1;
	v3 = ObjectsCount == 1;
	v4 = ObjectsCount < 1;
	ObjectsCount--;
	ObjectAvailable[MaxObjects_127 - 1 - ObjectsCount] = deletedObject;
	if( !(v4 | v3) ){
		if( a2 != result ){
			ObjectActive[a2] = ObjectActive[result];
		}
	}
}

//----- (0044ED9F) --------------------------------------------------------
void __fastcall BurnCrossDamage(int objectIndex)
{
	Player *player_1; // eax@6
	char classId; // cl@8
	int objectIndex_1; // [sp+1Ch] [bp-8h]@1

	objectIndex_1 = objectIndex;
	Player* player = &Players[ CurrentPlayerIndex ];
	if( player->CurAction != PCA_8_DEATH && !player->lvlChanging ){
		int damage = 16 * hack_ModifyTrapDamage();
		double fireResistancePercent = player->resistPercent[ET_1_FIRE];
		if( fireResistancePercent > 0 ){
			damage -= int(fireResistancePercent * damage);
		}
		if( player->Row == Objects[ objectIndex_1 ].Row && player->Col == Objects[ objectIndex_1 ].Col - 1 ){
			if(is(GameMode, GM_EASY/*, GM_CLASSIC*/)){
				damage = damage * 80 / 100; // -20% easy mode
				if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_4_EFFORTLESS) && MaxCountOfPlayersInGame == 1) {
					damage /= 2;
				}
			}
			else if (GameMode == GM_HARD) {
				damage = damage * 120 / 100; // +20% hard mode
				if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_6_IMPOSSIBLE)) {
					damage *= 2;
				}
			}
			player->CurLife -= damage;
			Players[ CurrentPlayerIndex ].BaseLife -= damage;
			player_1 = &Players[ CurrentPlayerIndex ];
			if( (player_1->CurLife & ~63) <= 0 ){
				TryToDie( CurrentPlayerIndex, 0 );
			LABEL_18:
				return;
			}
			classId = player_1->ClassID;
			if( classId ){
				if( classId == 1 ){
					goto LABEL_25;
				}
				if( classId == 2 ){
					PlayLocalSound( 585, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId == 3 ){
					PlayLocalSound( 905, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId == 4 ){
				LABEL_25:
					PlayLocalSound( 688, player_1->Row, player_1->Col );
					goto LABEL_18;
				}
				if( classId != 5 ){
					goto LABEL_18;
				}
			}
			PlayLocalSound( 797, player_1->Row, player_1->Col );
			goto LABEL_18;
		}
	}
}

//----- (0044EDF1) --------------------------------------------------------
int hack_ModifyTrapDamage()
{
	int result = Dungeon->level * (Difficulty + 1);
	return result;
}

//----- (0044EEFB) --------------------------------------------------------
void __fastcall SetDungeonMapNumber( int row, int col, int DungeonMapNumber )
{
	int decodedNumber;
	FineMap[ row ][ col ] = DungeonMapNumber;
	uint dungeonMapIndex = DungeonMapNumber - 1;
	uint cell = IsometricMapOffset(row, col);
	ushort* minPtr = (ushort*)CurrentLevelMINFile;

	if( Dungeon->genType == DT_4_HELL ){
		for( int i = 0; i < 16; i++ ){
			decodedNumber = (i & 1) - (i & 14) + 14;
			TileMap[cell].tile[i] = minPtr[16 * dungeonMapIndex + decodedNumber];
		}
	}else{
		for( int i = 0; i < 10; i++ ){
			decodedNumber = (i & 1) - (i & 14) + 8;
			TileMap[cell].tile[i] = minPtr[10 * dungeonMapIndex + decodedNumber];
		}
	}
}

//----- (0044EF7E) --------------------------------------------------------
void ChangeAllPlayersVisibility()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && player.dungeon == Dungeon ){
			ChangeVision(player.lightRadiusIdentificator, player.Row, player.Col);
		}
	}
}

//----- (0044EFBB) --------------------------------------------------------
void __fastcall ActivateChurchDoorCollateralDiagonal(int playerIndex, int objectIndex, char isCurrentPlayer)
{
	int objectIndex_1; // esi@1
	int openState; // eax@1
	int row; // edi@4
	int col; // ebx@4
	int cell; // eax@25
	bool monster_item; // cl@27
	bool monster_item_death; // al@31
	int mayBeObjectType; // eax@37
	int playerIndex_1; // [sp+Ch] [bp-Ch]@1
	int objectIndex_2; // [sp+14h] [bp-4h]@1

	objectIndex_1 = objectIndex;
	objectIndex_2 = objectIndex;
	playerIndex_1 = playerIndex;
	openState = Objects[ objectIndex ].OpenState;
	if( openState != 2 ){
		row = Objects[ objectIndex_1 ].Row;
		col = Objects[ objectIndex_1 ].Col;
		if( !openState ){
			if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
                NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectIndex );
			}
			if( !deltaload ) PlayLocalSound( Dungeon->graphType == DT_5_CRYPT ? S_1020_I_CROPEN : S_26_I_DOOROPEN, Objects[ objectIndex_1 ].Row, Objects[ objectIndex_1 ].Col );
			SetDungeonMapNumber( row, col, Dungeon->graphType == DT_5_CRYPT ? 209 : 395 );
			ArchGraphicsMap[ row ][ col ] = Dungeon->graphType >= DT_5_CRYPT ? 2 : 8;
			ApplyChangeFromDunMapToTileMap( row, col - 1 );
			Objects[ objectIndex_1 ].FrameIndex += 2;
			Objects[ objectIndex_1 ].InAction = 1;
			ChangeDungeonMapWithActivateDoor( objectIndex_2, row - 1, col );
			Objects[ objectIndex_1 ].OpenState = 1;
			Objects[ objectIndex_1 ].selectable = 2;
			goto LABEL_48;
		}
		PlayLocalSound( Dungeon->graphType == DT_5_CRYPT ? S_1021_I_CRCLOS : S_25_I_DOORCLOS, row, Objects[ objectIndex_1 ].Col );
		cell = col + 112 * row;
		monster_item = !MonsterMap[ 0 ][ cell ] && !ItemMap[ 0 ][ cell ];
		monster_item_death = monster_item && !DeathMonstersMap[ 0 ][ cell ];
		if( !monster_item_death ){
			Objects[ objectIndex_1 ].OpenState = 2;
			return;
		}
		if( playerIndex_1 == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, 44, objectIndex_2 );
		}
		Objects[ objectIndex_1 ].OpenState = 0;
		Objects[ objectIndex_1 ].selectable = 3;
		SetDungeonMapNumber( row, col, Objects[ objectIndex_1 ].TrapedObjectRow );
		mayBeObjectType = Objects[ objectIndex_1 ].TrapedObjectCol;
		if( Dungeon->graphType >= DT_5_CRYPT ){
			if( mayBeObjectType == 86 ){
				if( FineMap[ 0 ][ cell - 112 ] == 210 ){
					SetDungeonMapNumber( row - 1, col, 232 );
				}else{
					SetDungeonMapNumber( row - 1, col, 86 );
				}
				goto LABEL_47;
			}
		}else if( mayBeObjectType == 50 ){
			if( FineMap[ 0 ][ cell - 112 ] == 396 ){
				SetDungeonMapNumber( row - 1, col, 411 );
			}else{
				SetDungeonMapNumber( row - 1, col, 50 );
			}
			goto LABEL_47;
		}
		SetDungeonMapNumber( row - 1, col, Objects[ objectIndex_1 ].TrapedObjectCol );
	LABEL_47:
		Objects[ objectIndex_1 ].FrameIndex -= 2;
		Objects[ objectIndex_1 ].InAction = 0;
	LABEL_48:
		ChangeAllPlayersVisibility();
		return;
	}
	if( !deltaload ){
		PlayLocalSound( S_25_I_DOORCLOS, Objects[ objectIndex_1 ].Row, Objects[ objectIndex_1 ].Col );
	}
}

//----- (0044F21B) --------------------------------------------------------
void __fastcall ApplyChangeFromDunMapToTileMap( int row, int col )
{
	int dungeonMapIndex = FineMap[ row ][ col ] - 1;
	TileMap[IsometricMapOffset(row, col)].Layer[0].Left = *(ushort*)&CurrentLevelMINFile[20 * dungeonMapIndex + 16];
	TileMap[IsometricMapOffset(row, col)].Layer[0].Right = *(ushort*)&CurrentLevelMINFile[20 * dungeonMapIndex + 18];
}

//----- (0044F293) --------------------------------------------------------
void __fastcall ChangeDungeonMapWithActivateDoor( int objectIndex, int row, int col )
{
	int dungeonMap = FineMap[ row ][ col ];
	Object& object = Objects[objectIndex];
	uint newDungeonMapNumber = NULL;

	if( Dungeon->graphType >= DT_5_CRYPT ){
		switch (dungeonMap ){
			case 75:				newDungeonMapNumber = 204;			break;
			case 79:				newDungeonMapNumber = 208;			break;
			case 86:
				if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					newDungeonMapNumber = 232;
				}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					newDungeonMapNumber = 234;
				}
				break;
			case 91:				newDungeonMapNumber = 215;			break;
			case 93:				newDungeonMapNumber = 218;			break;
			case 99:				newDungeonMapNumber = 220;			break;
			case 111:				newDungeonMapNumber = 222;			break;
			case 113:				newDungeonMapNumber = 224;			break;
			case 115:				newDungeonMapNumber = 226;			break;
			case 117:				newDungeonMapNumber = 228;			break;
			case 119:				newDungeonMapNumber = 230;			break;
			case 232:				newDungeonMapNumber = 212;			break;
			case 234:				newDungeonMapNumber = 212;			break;
		}
	}else{
		switch (dungeonMap ){
			case 43:				newDungeonMapNumber = 392;			break;
			case 45:				newDungeonMapNumber = 394;			break;
			case 50:
				if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					newDungeonMapNumber = 411;
				}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					newDungeonMapNumber = 412;
				}
				break;
			case 54:				newDungeonMapNumber = 397;			break;
			case 55:				newDungeonMapNumber = 398;			break;
			case 61:				newDungeonMapNumber = 399;			break;
			case 67:				newDungeonMapNumber = 400;			break;
			case 68:				newDungeonMapNumber = 401;			break;
			case 69:				newDungeonMapNumber = 403;			break;
			case 70:				newDungeonMapNumber = 404;			break;
			case 72:				newDungeonMapNumber = 406;			break;
			case 212:				newDungeonMapNumber = 407;			break;
			case 354:				newDungeonMapNumber = 409;			break;
			case 355:				newDungeonMapNumber = 410;			break;
			case 411:				newDungeonMapNumber = 396;			break;
			case 412:				newDungeonMapNumber = 396;			break;
		}
	}
	if( newDungeonMapNumber ){
		SetDungeonMapNumber(row, col, newDungeonMapNumber);
	}
}

//----- (0044F54A) --------------------------------------------------------
void __fastcall ActivateChurchDoorMainDiagonal( int playerIndex, int objectOnMapIndex, char isCurrentPlayer )
{
	Object& objectOnMap = Objects[objectOnMapIndex];
	int objectRow = objectOnMap.Row;
	int objectCol = objectOnMap.Col;

	if( objectOnMap.OpenState == OS_2_BLOCKED ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS, objectRow, objectCol);
		}
		return;
	}

	if( objectOnMap.OpenState == OS_0_CLOSED ){
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectOnMapIndex );
		}
		if( !deltaload ){
			PlayLocalSound(Dungeon->graphType == DT_5_CRYPT ? S_1020_I_CROPEN : S_26_I_DOOROPEN, objectOnMap.Row, objectOnMap.Col);
		}
		int dunMapNum;
		if( Dungeon->graphType == DT_5_CRYPT ){
			dunMapNum = 206;
		}else if( objectOnMap.ShrineIndex == 214 ){
			dunMapNum = 408;
		}else{
			dunMapNum = 393;
		}
		SetDungeonMapNumber(objectRow, objectCol, dunMapNum);
		ArchGraphicsMap[ objectRow ][ objectCol ] = Dungeon->graphType >= DT_5_CRYPT ? 1 : 7;
		ApplyChangeFromDunMapToTileMap(objectRow - 1, objectCol);
		objectOnMap.FrameIndex += 2;
		objectOnMap.InAction = 1;
		ChangeDungeonMapWithActivateDoor(objectOnMapIndex, objectRow, objectCol - 1);
		objectOnMap.OpenState = OS_1_OPEN;
		objectOnMap.selectable = 2;
		ChangeAllPlayersVisibility();
		return;
	}
	// OPEN_STATE_OPEN
	if( !deltaload ){
		PlayLocalSound(Dungeon->graphType == DT_5_CRYPT ? S_1021_I_CRCLOS : S_25_I_DOORCLOS, objectOnMap.Row, objectOnMap.Col);
	}
	if( MonsterMap[ objectRow ][ objectCol ] || ItemMap[objectRow][objectCol] || DeathMonstersMap[objectRow][objectCol] ){
		objectOnMap.OpenState = OS_2_BLOCKED;
		return;
	}
	if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
        NetSendCmdParam1( 1, CMD_44_CLOSEDOOR, objectOnMapIndex );
	}
	objectOnMap.OpenState = OS_0_CLOSED;
	objectOnMap.selectable = 3;
	SetDungeonMapNumber(objectRow, objectCol, objectOnMap.ShrineIndex);
	int v18;
	if( Dungeon->graphType >= DT_5_CRYPT ){
		if( objectOnMap.MaybeSpeechIndex == 86 ){
			if( FineMap[ objectRow ][ objectCol - 1 ] == 210 ){///////////////////////////////
				v18 = 234;
			}else{
				v18 = 86;
			}
			SetDungeonMapNumber(objectRow, objectCol - 1, v18);
			objectOnMap.FrameIndex -= 2;
			objectOnMap.InAction = 0;
			ChangeAllPlayersVisibility();
			return;
		}
	}else{
		if( objectOnMap.MaybeSpeechIndex == 50 ){
			if( FineMap[ objectRow ][ objectCol - 1 ] == 396 ){ //////////////////////////////////
				v18 = 412;
			}else{
				v18 = 50;
			}
			SetDungeonMapNumber(objectRow, objectCol - 1, v18);
			objectOnMap.FrameIndex -= 2;
			objectOnMap.InAction = 0;
			ChangeAllPlayersVisibility();
			return;
		}
	}
	SetDungeonMapNumber(objectRow, objectCol - 1, objectOnMap.MaybeSpeechIndex);
	objectOnMap.FrameIndex -= 2;
	objectOnMap.InAction = 0;
	ChangeAllPlayersVisibility();
}

//----- (0044F7BF) --------------------------------------------------------
void __fastcall ActivateCatacombDoorMainDiagonal( int playerIndex, int objectIndex, int isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_44_CLOSEDOOR, objectIndex );
		}
		SetDungeonMapNumber(object.Row, object.Col, 538);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectIndex );
		}
		if( !deltaload ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 13);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044F934) --------------------------------------------------------
void __fastcall MonstersInteractWithDoors( int monsterIndex )
{
	if( monsterIndex < SummonMonsters_Count ) return; // Summons prohibited from interacting with objects
	Monster& monster = Monsters[monsterIndex];
	int row = monster.Row;
	int col = monster.Col;
	Cell o(ObjectsMap, row, col);
	if( !o(-1,-1) && !o(0,-1) && !o(1,-1) && !o(-1,0) && !o(1,0) && !o(-1,1) && !o(0,1) && !o(1,1) ){
		return; // если вокруг монстра нет объектов
	}
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		int objectIndex = ObjectActive[objectIndexIndex];
		Object& object = Objects[objectIndex];
		int baseObjectIndex = object.BaseObjectIndex;
		int deltaRow = abs(object.Row - row);
		int deltaCol = abs(object.Col - col);

		if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
					ActivateChurchDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
					ActivateChurchDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}else if( baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
					ActivateCatacombDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
					ActivateCatacombDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}else if( baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL || baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){// тут подозрительное место
			if( object.OpenState == OS_0_CLOSED ){
				if( deltaRow == 1 && deltaCol <= 1 && baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
					ActivateCaveDoorCollateralDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
				if( deltaRow <= 1 && deltaCol == 1 && baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
					ActivateCaveDoorMainDiagonal(CurrentPlayerIndex, objectIndex, 1);
				}
			}
		}
		if (GameMode == GM_HARD && (baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP || baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE) && monster.SpritePtr->baseMonsterIndex != BM_253_EXPLODER) {
			int deltaOrient = abs(monster.Orientation - OrientationToTarget(monster.Row, monster.Col, object.Row, object.Col));
			if ((deltaRow == 1 && deltaCol <= 1) && (deltaOrient < 2 || deltaOrient > 5)) {
				if (object.selectable && monster.CurAction != A_4_MELEE) {
					monster.TargetObjectIndex = objectIndex;
					StartMonsterMeleeAttack(monsterIndex, OrientationToTarget(monster.Row, monster.Col, object.Row, object.Col));
				}
			}
		}
	}
}

//----- (0044FB64) --------------------------------------------------------
void __fastcall ActivateCatacombDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_44_CLOSEDOOR, objectIndex );
		}
		SetDungeonMapNumber(object.Row, object.Col, 540);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectIndex );
		}
		if( !deltaload ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 17);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FCD9) --------------------------------------------------------
void __fastcall ActivateCaveDoorCollateralDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_44_CLOSEDOOR, objectIndex );
		}
		SetDungeonMapNumber(object.Row, object.Col, 534);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectIndex );
		}
		if( !deltaload ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 541);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FE51) --------------------------------------------------------
void __fastcall ActivateCaveDoorMainDiagonal( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_2_BLOCKED ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS,object.Row, object.Col);
		}
		return;
	}
	if( object.OpenState == OS_1_OPEN ){
		if( !deltaload ){
			PlayLocalSound(S_25_I_DOORCLOS, object.Row, object.Col);
		}
		int cell = object.Col + 112 * object.Row;
		if( MonsterMap[ 0 ][ cell ] || ItemMap[0][cell] || DeathMonstersMap[0][cell] ){
			object.OpenState = OS_2_BLOCKED;
			return;
		}
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_44_CLOSEDOOR, objectIndex );
		}
		SetDungeonMapNumber(object.Row, object.Col, 531);
		object.FrameIndex -= 2;
		object.InAction = 0;
		object.OpenState = OS_0_CLOSED;
		object.selectable = 3;
	}else{
		if( playerIndex == CurrentPlayerIndex && isCurrentPlayer ){
            NetSendCmdParam1( 1, CMD_43_OPENDOOR, objectIndex );
		}
		if( !deltaload ){
			PlayLocalSound(S_26_I_DOOROPEN, object.Row, object.Col);
		}
		SetDungeonMapNumber(object.Row, object.Col, 538);
		object.FrameIndex += 2;
		object.InAction = 1;
		object.OpenState = OS_1_OPEN;
		object.selectable = 2;
	}
	ChangeAllPlayersVisibility();
}

//----- (0044FFC9) --------------------------------------------------------
void __fastcall ChangeMapRectAfterScriptEvent( int startRow40, int startCol40, int endRow40, int endCol40 )
{
	uint minRow112 = Convert40to112(startRow40);
	uint minCol112 = Convert40to112(startCol40);
	uint maxRow112 = Convert40to112(endRow40) + 1;
	uint maxCol112 = Convert40to112(endCol40) + 1;

	if( Dungeon == DUN_4_CHURCH_4 && Quests[Q_7_OGDENS_SIGN].status > 0 ){ //Snot door fix
		for( int i = 0; i < MaxObjects_127; i++ ){
			if( is(Objects[i].BaseObjectIndex, BO_1_CHURCH_DOOR_MAIN_DIAGONAL, BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL)
			 && Objects[i].Row >= (int)minRow112 && Objects[i].Row <= (int)maxRow112
			 && Objects[i].Col >= (int)minCol112 && Objects[i].Col <= (int)maxCol112 ){
				DeleteObjectOnMap(i, 0);
			}
		}
	}

	for( int col40 = startCol40; col40 <= endCol40; col40++ ){
		for( int row40 = startRow40; row40 <= endRow40; row40++ ){
			uint cell40 = 40 * row40 + col40;
			SetNewTileNumber(row40, col40, AltMap[ 0 ][ cell40 ]);
			Map[ 0 ][ cell40 ] = AltMap[ 0 ][ cell40 ];
		}
	}
	
	if( Dungeon->graphType == DT_1_CHURCH ){
		ChangeArchGraphicsIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112);
		AddDoorsAndLibrazObjectInChurch(minRow112, minCol112, maxRow112, maxCol112);
	}
	if( Dungeon->genType == DT_2_CATACOMB ){
		ChangeArchGraphicsIn112RectFromDungeonMapCatacomb(minRow112, minCol112, maxRow112, maxCol112);
		AddDoorsObjectInCatacomb(minRow112, minCol112, maxRow112, maxCol112);
	}
}

//----- (004500B3) --------------------------------------------------------
void __fastcall SetNewTileNumber( int row40, int col40, uint someTileNumber )
{
	uint someTileIndex = someTileNumber - 1;
	TileSquare& tileSquare = *(TileSquare*)&CurrentLevelTilesFile[8 * someTileIndex];
	uint row112 = 2 * row40 + 16;
	uint col112 = 2 * col40 + 16;
	SetDungeonMapNumber(row112, col112, tileSquare.up + 1);
	SetDungeonMapNumber(row112 + 1, col112, tileSquare.left + 1);
	SetDungeonMapNumber(row112, col112 + 1, tileSquare.right + 1);
	SetDungeonMapNumber(row112 + 1, col112 + 1, tileSquare.down + 1);
}

//----- (0045012B) --------------------------------------------------------
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapChurch( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col = minCol112; col <= maxCol112; col++ ){
		for( int row = minRow112; row <= maxRow112; row++ ){
			uint newArchGraphics = 0;
			switch (FineMap[ row ][ col ] ){
			case  12: newArchGraphics = 1; break;
			case  11: newArchGraphics = 2; break;
			case  71: newArchGraphics = 1; break;
			case 259: newArchGraphics = 5; break;
			case 249: newArchGraphics = 2; break;
			case 325: newArchGraphics = 2; break;
			case 321: newArchGraphics = 1; break;
			case 255: newArchGraphics = 4; break;
			case 211: newArchGraphics = 1; break;
			case 344: newArchGraphics = 2; break;
			case 341: newArchGraphics = 1; break;
			case 331: newArchGraphics = 2; break;
			case 418: newArchGraphics = 1; break;
			case 421: newArchGraphics = 2; break;
			}
			ArchGraphicsMap[ row ][ col ] = newArchGraphics;
		}
	}
}

//----- (00450217) --------------------------------------------------------
void __fastcall ChangeArchGraphicsIn112RectFromDungeonMapCatacomb( int minRow112, int minCol112, int maxRow112, int maxCol112 )
{
	for( int col112 = minCol112; col112 <= maxCol112; col112++ ){
		for( int row112 = minRow112; row112 <= maxRow112; row112++ ){
			uint cell112 = 112 * row112 + col112;
			uint newArchGraphics = 0;
			switch (FineMap[ 0 ][ cell112 ] ){
				case 541:	newArchGraphics = 5;	break;
				case 178:	newArchGraphics = 5;	break;
				case 551:	newArchGraphics = 5;	break;
				case 542:	newArchGraphics = 6;	break;
				case 553:	newArchGraphics = 6;	break;
				case 13:	newArchGraphics = 5;	break;
				case 17:	newArchGraphics = 6;	break;
			}
			ArchGraphicsMap[ 0 ][ cell112 ] = newArchGraphics;
		}
	}
	for( int col112 = minCol112; col112 <= maxCol112; col112++ ){
		for( int row112 = minRow112; row112 <= maxRow112; row112++ ){
			uint cell112 = 112 * row112 + col112;
			switch (FineMap[ 0 ][ cell112 ] ){// Ворота?
				case 132:
					ArchGraphicsMap[ row112 ][ col112 + 1 ] = 2;
					ArchGraphicsMap[ row112 ][ col112 + 2 ] = 1;
					break;
				case 135:
				case 139:
					ArchGraphicsMap[ row112 + 1 ][ col112 ] = 3;
					ArchGraphicsMap[ row112 + 2 ][ col112 ] = 4;
					break;
			}
		}
	}
}

//----- (00450313) --------------------------------------------------------
void __fastcall ChangeMapRectAfterScriptEvent2( int startRow40, int startCol40, int endRow40, int endCol40 )
{
	for( int col40 = startCol40; col40 <= endCol40; col40++ ){
		for( int row40 = startRow40; row40 <= endRow40; row40++ ){
			uint cell40 = 40 * row40 + col40;
			SetNewTileNumber(row40, col40, AltMap[ 0 ][ cell40 ]);
			Map[ 0 ][ cell40 ] = AltMap[ 0 ][ cell40 ];
		}
	}
	uint minRow112 = Convert40to112(startRow40);
	uint minCol112 = Convert40to112(startCol40);
	uint maxRow112 = Convert40to112(endRow40) + 1;
	uint maxCol112 = Convert40to112(endCol40) + 1;
	if( Dungeon->graphType == DT_1_CHURCH ){
		ChangeArchGraphicsIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112);
		//AddObjectOnMapIn112RectFromDungeonMapChurch(minRow112, minCol112, maxRow112, maxCol112); только этим и отличается от ChangeMapRectAfterScriptEvent
	}
	if( Dungeon->genType == DT_2_CATACOMB ){
		ChangeArchGraphicsIn112RectFromDungeonMapCatacomb(minRow112, minCol112, maxRow112, maxCol112);
		//ProcessMap2(minRow112, minCol112, maxRow112, maxCol112);
	}
}

//----- (004503D1) --------------------------------------------------------
void DisarmTrap( int playerIndex, int objectIndex )
{
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}

	Player& player = Players[playerIndex];
	Object& object = Objects[objectIndex];

	if( !object.IsTrapped ){
		return;
	}

	// проверяем все объекты. Если нашли тот, 
	for( int i = 0; i < ObjectsCount; i++ ){
		int currentObjectIndex = ObjectActive[i];
		Object& currentObject = Objects[currentObjectIndex];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex != BO_53_WALL_TRAP_FIRES_IN_ROW && baseObjectIndex != BO_54_WALL_TRAP_FIRES_IN_COL ){
			continue;
		}
		int trappenRow = currentObject.ShrineIndex;
		int trappenCol = currentObject.MaybeSpeechIndex;
		if( ObjectsMap[ trappenRow ][ trappenCol ] - 1 == objectIndex ){
			currentObject.OpenState = OS_1_OPEN;
			object.IsTrapped = false;
			return;
		}
	}
}

i64 GetXpForTrapDisarm(int playerIndex)
{
	const Player& player = Players[playerIndex];
	i64 xp_per_disarm;
	switch (Difficulty) {
	default:				xp_per_disarm = Dungeon->level * 200;							break;
	case DL_1_PURGATORY:	xp_per_disarm = Dungeon->level * 833        +         9'166;	break;
	case DL_2_DOOM:			xp_per_disarm = Dungeon->level * 10'000     +        20'000;	break; //     30'000  >     260'000
	case DL_3_CRUCIBLE:		xp_per_disarm = Dungeon->level * 73'913     +       200'000;	break; //    300'000  >   2'000'000
	case DL_4_TORMENT:		xp_per_disarm = Dungeon->level * 173'913    +     1'826'087;	break; //  2'000'000  >   6'000'000
	case DL_5_AGONY:		xp_per_disarm = Dungeon->level * 391'304    +     5'608'696;	break; //  6'000'000  >  15'000'000
	case DL_6_TERROR:		xp_per_disarm = Dungeon->level * 521'739    +    12'478'261;	break; // 13'000'000  >  25'000'000
	case DL_7_DESTRUCTION:	xp_per_disarm = Dungeon->level * 1'000'000  +    24'000'000;	break; // 25'000'000  >  48'000'000
	case DL_8_INFERNO:		xp_per_disarm = Dungeon->level * 1'521'739  +    43'478'261;	break; // 45'000'000  >  80'000'000
	}
	return xp_per_disarm;
}

//----- (00711720) --------------------------------------------------------
void __fastcall DisarmTrapFix( int playerIndex, int objectIndex )
{
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics( CM_1_NORMAL_HAND );
	}

	Player& player = Players[ playerIndex ];
	Object& object = Objects[ objectIndex ];

	if( !object.IsTrapped ){
		return;
	}

	// проверяем все объекты. Если нашли тот, 
	for( int i = 0; i < ObjectsCount; i++ ){
		int currentObjectIndex = ObjectActive[ i ];
		Object& currentObject = Objects[ currentObjectIndex ];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex != BO_53_WALL_TRAP_FIRES_IN_ROW && baseObjectIndex != BO_54_WALL_TRAP_FIRES_IN_COL ){
			continue;
		}
		int trappenRow = currentObject.ShrineIndex;
		int trappenCol = currentObject.MaybeSpeechIndex;
		if( ObjectsMap[ trappenRow ][ trappenCol ] - 1 == objectIndex ){
			currentObject.OpenState = OS_1_OPEN;
			object.IsTrapped = false;
			AddPlayerExperience(playerIndex, GetXpForTrapDisarm(playerIndex), player.Row, player.Col);
			return;
		}
	}
	if( object.BaseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && object.BaseObjectIndex <= BO_70_TRAPED_LARGE_CHEST ){
		(int&) object.BaseObjectIndex -= (BO_68_TRAPED_SMALL_CHEST - BO_5_SMALL_CHEST);
		object.IsTrapped = false;
		AddPlayerExperience(playerIndex, GetXpForTrapDisarm(playerIndex), player.Row, player.Col);
	}
}

//----- (0045049A) --------------------------------------------------------
BASE_ITEM __fastcall FindBaseItemWithMagicCode( int findableMagicCode )
{
	uint baseItemIndex;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( baseItemIndex = 0; baseItemIndex < count_BaseItems; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( baseItem.mode + 1 & mode && baseItem.ChanceToDrop > 0 && baseItem.MagicCode == findableMagicCode ){
			break;
		}
	}
	return (BASE_ITEM)baseItemIndex;
}

//----- (fleshdooom) --------------------------------------------------------
void __fastcall ActivateAnvil(int playerIndex, int objectIndex)
{
	if (deltaload) {//Mark Anvil as unselectable if already touched by some player.
		Object& anvil = Objects[objectIndex];
		Player& player = Players[playerIndex];
		anvil.selectable = 0;
		anvil.objectAllowWalking = 0;
		return;
	}

	bool noTPmode = isTownPortalForbidden();
	if (Dungeon == DUN_43_HEPHASTO) {
		if ((Quests[Q_21_THE_CURSED_BLADE].status == QS_2_IN_PROGRESS && Quests[Q_21_THE_CURSED_BLADE].status2 == 8) || (Quests[Q_21_THE_CURSED_BLADE].status != QS_3_COMPLETE && noTPmode)) {
			int item;
			if (FindItemInInventory(playerIndex, BI_399_SHADOWFANG, item)) {
				ClearInventorySlot(playerIndex, item);
			}
			Object& anvil = Objects[objectIndex];
			Player& player = Players[playerIndex];
			anvil.selectable = 0;
			anvil.objectAllowWalking = 0;
			if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
				int missileIndex = CastMissile(anvil.Row - 1, anvil.Col - 1, anvil.Row, anvil.Col, 0, MI_5_FIRE_WALL_SEGMENT, 1, playerIndex, 0, 0, 0);
			}
			else {
				int missileIndex = CastMissile(anvil.Row, anvil.Col - 1, anvil.Row, anvil.Col, 0, MI_6_FIREBLAST, 1, playerIndex, 0, 0, 0);
			}
			//if( missileIndex != -1 ){
			//	SetMissileSprite( missileIndex, MIS_19_BIGEXP );
			//	Missiles[missileIndex].TimeToLive = Missiles[missileIndex].framesCount - 1;
			//}
			//CastMissile( anvil.Row, anvil.Col, player.Row, player.Col, 0, MI_72_FIRE_NOVA, 1, playerIndex, 0, 0 , missile.spellIndex );
			//CastMissile( anvil.Row, anvil.Col, player.Row, player.Col, 0, MI_66_MONSTER_APOC_EXPLODE, -1, playerIndex, 10000, 0 , missile.spellIndex );
			PlayLocalSound(S_1669_shadowfang_destroy_harp, anvil.Row, anvil.Col);// was 1084 (dim vision)
			//Quests[ Q_21_FLESHDOOM ].status2 = 9;
			//ActivateSlainHero_TH( playerIndex, objectIndex );
			Quests[Q_21_THE_CURSED_BLADE].status = QS_3_COMPLETE;
			int oil = by(Difficulty, MC_36_BLACKSMITH_OIL, MC_40_OIL_OF_HARDENING, MC_37_OIL_OF_FORTITUDE);
			for (int i = 10; i--; ) CreateTypeItem(anvil.Row, anvil.Col, 0, IC_0_OTHER, oil, 0, 1);
			if (playerIndex == CurrentPlayerIndex) {//If NOT deltaload, mark object for deltaload sync.
				//				NetSendCmdQuest(1, Q_21_FLESHDOOM);//Reward is only available for players on the same map when Hell Forge is destroyed.  No, this doesn't work as you would expect.  Syncing quest status will make Hell Forge not drop anything for other players.
				NetSendCmdParam1(0, CMD_45_OPERATEOBJ, objectIndex);
			}
		}
	}
}

//----- (004504B2) --------------------------------------------------------
void __fastcall OperateObject(int playerIndex, int objectIndex, int telekines)
{
	int isCurrentPlayer = playerIndex == CurrentPlayerIndex;
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	bool ActivatedIronmanObject = IsIronmanObject(Objects[objectIndex]);
	switch( baseObjectIndex ){
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:		ActivateAncientTomeOrBookOfVileness(playerIndex, objectIndex);		break;
	case BO_48_SARCOPHAGUS:		ActivateSarcofagus(playerIndex, objectIndex, isCurrentPlayer, telekines);		break;
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
				ActivateChurchDoorMainDiagonal(playerIndex, objectIndex, 1);
			}else if( baseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
				ActivateChurchDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateChurchDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
				ActivateCatacombDoorMainDiagonal(playerIndex, objectIndex, 1);
			}
			if( baseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCatacombDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateCatacombDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
		if( telekines ){
			if( baseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
				ActivateCaveDoorMainDiagonal(playerIndex, objectIndex, 1);
			}else if( baseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCaveDoorCollateralDiagonal(playerIndex, objectIndex, 1);
			}
		}else if( playerIndex == CurrentPlayerIndex ){
			ActivateCaveDoor(playerIndex, objectIndex, 1);
		}
		break;
	case BO_4_LEVER:
	case BO_28_SKULL_LEVER:		ActivateLever(playerIndex, objectIndex);		break;
	case BO_41_MYTHICAL_BOOK:		ActivateMythicalBook(playerIndex, objectIndex);		break;
	case BO_50_LEVER_2:		ActivateLever2(objectIndex);		break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:		ApplyShrineEffect(playerIndex, objectIndex, S_69_I_MAGIC);		break;
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST: OperateChest(playerIndex, objectIndex, isCurrentPlayer, telekines); break;
	case BO_71_BOOK_OF_THE_BLIND:
	case BO_72_BOOK_OF_BLOOD:
	case BO_88_STEEL_TOME:		ActivateQuestBook(playerIndex, objectIndex);		break;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:		ActivateLibraryBook(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:		ActivateBookcase(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_67_DECAPITATED_BODY:		ActivateDecapitatedBody(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:		ActivateArmorRack(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_79_GOAT_SHRINE:		ActivateGoatShrine(playerIndex, objectIndex, S_118_PORTAL);		break;
	case BO_80_CAULDRON:		ActivateCauldron(playerIndex, objectIndex, S_101_CALDRON);		break;
	case BO_66_BLOOD_FOUNTAIN:
	case BO_76_PURIFYING_SPRING:
	case BO_81_MURKY_POOL:
	case BO_82_FOUNTAIN_OF_TEARS:		ActivateFountain(playerIndex, objectIndex);		break;
	case BO_86_QUEST_BOOK:		ActivateQuestBook2(playerIndex, objectIndex);		break;
	case BO_73_PEDESTAL_OF_BLOOD:		ActivatePedestalOfBlood(playerIndex, objectIndex);		break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:		ActivateWeaponRack(playerIndex, objectIndex, isCurrentPlayer);		break;
	case BO_94_MUSHROOM_PATCH:		ActivateMushroomPatch(playerIndex, objectIndex);		break;
	case BO_95_VILE_STAND:		ActivateVileStand(playerIndex, objectIndex);		break;
	case BO_96_SLAIN_HERO:		ActivateSlainHero_TH(playerIndex, objectIndex);		break; // ActivateSlainHero
	case BO_97_OGDEN_CHEST:		ActivateLargeOgdenQuestChest( playerIndex, objectIndex );		break;
	case BO_24_ANGEL:		ActivateAngels( playerIndex, objectIndex );		break;
	case BO_98_HELLFORGE:	ActivateAnvil( playerIndex, objectIndex );		break; // fleshdoom patch (1)
	}

	if( GameModeType() == GAME_MODE_TYPE::IRON && ActivatedIronmanObject && GetNumberOfMonstersInDungeon() == 0 && GetNumberOfObjectsInDungeon() <= MaxIronmanObjects && Dungeon != DUN_0_TOWN ){
		IronmanLevelCleared();
	}
}

//----- (004507F7) --------------------------------------------------------
void __fastcall ActivateChurchDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			ActivateChurchDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			ActivateChurchDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (0045088E) --------------------------------------------------------
void __fastcall ActivateLever( int playerIndex, int objectsOnMapIndex )
{
	Object& object = Objects[objectsOnMapIndex];

	if( object.selectable == false ){
		return;
	}

	if( Dungeon->isQuest && Dungeon == DUN_34_THE_PIT ){
		if( !LeverClick() ){
			object.selectable = false;
		}
		return;
	}

	if( !deltaload ){
		PlayLocalSound(S_68_I_LEVER, object.Row, object.Col);
	}

	object.selectable = false;
	object.FrameIndex++;
	bool needChangeMap = true;
	if( Dungeon == DUN_16_HELL_4 ){
		for( int currentObjectOnMapIndex = 0; currentObjectOnMapIndex < ObjectsCount; currentObjectOnMapIndex++ ){
			Object& currentObjectOnMap = Objects[ObjectActive[currentObjectOnMapIndex]];
			if( currentObjectOnMap.BaseObjectIndex == BO_28_SKULL_LEVER 
			 && object.bookIndex == currentObjectOnMap.bookIndex 
			 && currentObjectOnMap.selectable ){
				needChangeMap = false;
			}
		}
	}
	#if comment
	if( Dungeon == DUN_24_CRYPT_4 ){
		OpenUberDiabloRoom();
		IsUberLeverActivated = 1;
		needChangeMap = false;
		Quests[21].status = QS_3_COMPLETE;
	}
	#endif
	if( Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS ){
		if( !Objects[ ObjIndex(44, 24) ].selectable && !Objects[ ObjIndex(44, 26) ].selectable ){
			OpenNakrul(/*isWeak*/ false);
			if( playerIndex == CurrentPlayerIndex ){
				NetSendCmdQuest(1, Q_16_HORAZON_S_DEMONS);
			}
			needChangeMap = false;
		}
	}
	if( needChangeMap ){
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
	}
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectsOnMapIndex );
	}
}

//----- (00450982) --------------------------------------------------------
void __fastcall ActivateAncientTomeOrBookOfVileness( int playerIndex, int objectIndex ) // #Chamber of Bone reward, #Chamber Hydra, #Chamber Fury, #COB Fury
{
	Object& object = Objects[objectIndex];

	if( !object.selectable ){
		return;
	}
	if( !Dungeon->isQuest ){
		object.selectable = 0;
		object.FrameIndex++;
		return;
	}

	Player& player = Players[playerIndex];
	Player& currentPlayer = Players[CurrentPlayerIndex];

	if (Dungeon == DUN_26_BONE_CHAMBER) {// сделано исключительно для сингла. Хоть и передаётся playerIndex всё равно работа ведётся с текущим
		object.selectable = 0;
		object.FrameIndex++;

		if (deltaload) {//Must need this so player will not automatically get another slvl upon re-entry.
			return;
		}

		int spellindex;

		switch (currentPlayer.fullClassId) {
		case    PFC_WARRIOR:
		case	PFC_INQUISITOR:
		case	PFC_GUARDIAN:
		case	PFC_TEMPLAR:
		case	PFC_ARCHER:
		case	PFC_SCOUT:
		case	PFC_SHARPSHOOTER:
		case	PFC_MONK:
		case	PFC_KENSEI:
		case	PFC_SHUGOKI:
		case	PFC_SHINOBI:
		case	PFC_ROGUE:
		case	PFC_ASSASSIN:
		case	PFC_IRON_MAIDEN:
		case	PFC_SAVAGE:
		case	PFC_BERSERKER:
		case	PFC_EXECUTIONER:
		case	PFC_THRAEX:
		case	PFC_MURMILLO:
		case	PFC_DIMACHAERUS:
		case	PFC_DRUID:
		case	PFC_SECUTOR: spellindex = PS_22_FURY; break;

		case	PFC_BOMBARDIER:
		case	PFC_TRAPPER:
		case	PFC_MAGE:
		case	PFC_ELEMENTALIST:
		case	PFC_DEMONOLOGIST:
		case	PFC_NECROMANCER:
		case	PFC_BEASTMASTER:
		case	PFC_WARLOCK:
		default: spellindex = PS_13_HYDRA; break;
		}

		//Mamluk, exception:
		if (HasTrait(playerIndex, TraitId::Mamluk)) {
			spellindex = PS_22_FURY;
		}
		if (GameMode == GM_CLASSIC) { spellindex = PS_13_HYDRA; }

		currentPlayer.AvailableSpellMask |= 1i64 << (spellindex - 1);
		if( currentPlayer.SpellLevels[spellindex] < SpellLevelBookMax() ){
			currentPlayer.SpellLevels[spellindex]++;
		}
		Quests[Q_14_THE_CHAMBER_OF_BONE].status = QS_3_COMPLETE;
		if( !deltaload ){
			PlayLocalSound(S_14_QUESTDON, object.Row, object.Col);
		}
		AddOnScreenMessage(OM_43_Arcane_knowledge);

		switch (spellindex) {//The missile might desync but that's no big deal.
			case PS_22_FURY: CastMissile(currentPlayer.Row, currentPlayer.Col, object.Row, object.Col, currentPlayer.dir, MI_43_FURY, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, PS_22_FURY); break;
			case PS_13_HYDRA:
			default: CastMissile(currentPlayer.Row, currentPlayer.Col, object.Row - 2, object.Col - 4, currentPlayer.dir, MI_2_HYDRA, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, PS_13_HYDRA); break;
		}

		if (playerIndex == CurrentPlayerIndex) {
			NetSendCmdParam1(0, CMD_45_OPERATEOBJ, objectIndex);
		}
	}

	if( Dungeon != DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
		return;
	}

	int needPhasePlayer = 0;// алгоритм для перемещения по квестовым телепортам в комнате лазаруса
	int needChangeMap = 0;

	for( int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++ ){
		Object& object = Objects[ ObjectActive[objectIndex] ];
		int destCol = 0;
		int destRow = 0;
		if( object.BaseObjectIndex == BO_85_MCIRL && object.maybeFrameIndex == 1 ){
			destCol = 29;
			destRow = 27;
			object.maybeFrameIndex = 4;
			needPhasePlayer = 1;
		}
		if( object.BaseObjectIndex == BO_85_MCIRL && object.maybeFrameIndex == 2 ){
			destCol = 29;
			destRow = 43;
			object.maybeFrameIndex = 4;
			needPhasePlayer = 1;
		}
		if( needPhasePlayer ){
			Objects[ObjectsMap[ 35 ][ 36 ] - 1].readyState++;// состояние круга телепорта у лазаруса
			CastMissile(player.Row, player.Col, destRow, destCol, player.dir, MI_3_PHASING, CT_0_PLAYER, playerIndex, 0, 0, PS_10_PHASING );
			needPhasePlayer = 0;
			needChangeMap = 1;
		}
	}

	if( needChangeMap ){
		object.selectable = 0;
		object.FrameIndex++;
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		for( int i = 0; i < ObjectsCount; i++ ){
			SetDunTileForObject(ObjectActive[i]);
		}
	}
}

//----- (00450BE2) --------------------------------------------------------
void __fastcall ActivateQuestBook( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	if( Speech_IsPanelVisible ){
		return;
	}
	int baseObjectIndex = object.BaseObjectIndex;
	if( baseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
		Quest& quest = Quests[Q_8_HALLS_OF_THE_BLIND];
		if( quest.status2 == 0 ){
			for (int i = 0; i < MonstersCount; i++)
			{
				if (Monsters[i].newBossId - 1 == UM_676_Hazeweaver || Monsters[i].newBossId - 1 == UM_C_871_HAZEWEAVER ) {
					ChangeLightPos(Monsters[i].LightIndex, Monsters[i].Row, Monsters[i].Col);
				}
			}
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
		}
	}
	if( baseObjectIndex == BO_72_BOOK_OF_BLOOD ){
		Quest& quest = Quests[Q_9_VALOR];
		if( quest.status2 == 0 ){
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
			SpawnSomething(BI_289_BLOOD_STONE, Convert40to112(CurPattern.row) + 3, Convert40to112(CurPattern.col) + 10, 0, 0, 1, 1);
			SpawnSomething(BI_289_BLOOD_STONE, Convert40to112(CurPattern.row) + 15, Convert40to112(CurPattern.col) + 10, 0, 0, 1, 1);
			SpawnSomething(BI_289_BLOOD_STONE, Convert40to112(CurPattern.row) + 9, Convert40to112(CurPattern.col) + 17, 0, 0, 1, 1);
		}
	}
	if( baseObjectIndex == BO_88_STEEL_TOME ){
		Quest& quest = Quests[Q_11_WARLORD_OF_BLOOD];
		if( quest.status2 == 0 ){
			quest.status = QS_2_IN_PROGRESS;
			quest.talkToFlag = true;
			quest.status2 = 1;
		}
	}
	if( object.FrameIndex != object.maybeFrameIndex ){
		if( baseObjectIndex != BO_72_BOOK_OF_BLOOD ){
			ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		}
		if( baseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
			int currentBlindReward = blindRewards[Difficulty];
			if (GameMode == GM_CLASSIC) { currentBlindReward = 4680 + Difficulty; }
			SpawnUnique(currentBlindReward, Convert40to112(CurPattern.row) + 5, Convert40to112(CurPattern.col) + 5);
			int wallTransparentIndex = TransValue;
			TransValue = 9;
			SetAreaTransparencyByIndex_40(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			TransValue = wallTransparentIndex;
		}
	}
	object.FrameIndex = object.maybeFrameIndex;
	StartSpeech(object.SpeechIndex);
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
	}
}

//----- (00450D94) --------------------------------------------------------
void __fastcall ActivateMythicalBook( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	Quest& quest = Quests[Q_14_THE_CHAMBER_OF_BONE];
	if (!object.selectable || Speech_IsPanelVisible) {
		return;
	}

	if( object.FrameIndex != object.maybeFrameIndex ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			int objectIndex = ObjectActive[objectIndexIndex];
			SetDunTileForObject(objectIndex);
		}
	}

	object.FrameIndex = object.maybeFrameIndex;

	if( quest.status == QS_1_ACCEPT ){
		quest.status = QS_2_IN_PROGRESS;
		quest.talkToFlag = true;
		if (MaxCountOfPlayersInGame > 1) {
			NetSendCmdQuest(true, Q_14_THE_CHAMBER_OF_BONE);
		}
	}
	if( playerIndex != CurrentPlayerIndex ){//This already takes care of deltaload, as it assigns a negative playerIndex.
		return;
	}
	int speechIndex;
	switch (Players[CurrentPlayerIndex].ClassID ){
	case PC_0_WARRIOR:		
	case PC_5_SAVAGE:		speechIndex = SP_235;		break;
	case PC_1_ARCHER:		speechIndex = SP_243;		break;
	case PC_4_ROGUE:		speechIndex = SP_263;		break;
	case PC_3_MONK:			speechIndex = SP_259;		break;
	case PC_2_MAGE:			speechIndex = SP_239;		break;
	}
	quest.speechIndex = speechIndex;
	StartSpeech(speechIndex);
	if (playerIndex == CurrentPlayerIndex) {
		NetSendCmdParam1(0, CMD_45_OPERATEOBJ, objectIndex);
	}
}

i64 GetXpForOpeningChest ( int playerIndex )
{
	const Player& player = Players[playerIndex]; 
	i64 xp_per_disarm;
	switch (Difficulty) {
	default:				xp_per_disarm = Dungeon->level * 200;							break;
	case DL_1_PURGATORY:	xp_per_disarm = Dungeon->level * 833 + 9'166;	break;
	case DL_2_DOOM:			xp_per_disarm = Dungeon->level * 10'000 + 20'000;	break; //     30'000  >     260'000
	case DL_3_CRUCIBLE:		xp_per_disarm = Dungeon->level * 73'913 + 200'000;	break; //    300'000  >   2'000'000
	case DL_4_TORMENT:		xp_per_disarm = Dungeon->level * 173'913 + 1'826'087;	break; //  2'000'000  >   6'000'000
	case DL_5_AGONY:		xp_per_disarm = Dungeon->level * 391'304 + 5'608'696;	break; //  6'000'000  >  15'000'000
	case DL_6_TERROR:		xp_per_disarm = Dungeon->level * 521'739 + 12'478'261;	break; // 13'000'000  >  25'000'000
	case DL_7_DESTRUCTION:	xp_per_disarm = Dungeon->level * 1'000'000 + 24'000'000;	break; // 25'000'000  >  48'000'000
	case DL_8_INFERNO:		xp_per_disarm = Dungeon->level * 1'521'739 + 43'478'261;	break; // 45'000'000  >  80'000'000
	}
	return xp_per_disarm / 4;
}

//----- (00450E94) --------------------------------------------------------
void __fastcall ActivateChest( int playerIndex, int objectIndex, int isCurrentIndex )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];

	if( object.selectable == false ){
		return;
	}
	
	if( !deltaload ){
		PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
	}
	object.selectable = false;
	object.FrameIndex += 2;
	if( deltaload ){
		return;
	}
	if (isCurrentIndex && GameMode != GM_CLASSIC) {
		AddPlayerExperience(playerIndex, GetXpForOpeningChest(playerIndex), player.Row, player.Col);
	}
	SetRndSeed(object.Seed);
	if( Dungeon->isQuest ){
		for( int i = 0; i < object.ShrineIndex; i++ ){
			CreateRndItem(object.Row, object.Col, 1, isCurrentIndex, 0, 1<<playerIndex ); // normal chest drop 
		}
	}else{
		for( int i = 0; i < object.ShrineIndex; i++ ){
			if( object.MaybeSpeechIndex ){
				CreateRndItem( object.Row, object.Col, 0, isCurrentIndex, 0, 1<<playerIndex);
			}else{
				CreateRndUseful(playerIndex, object.Row, object.Col, isCurrentIndex);
			}
		}
	}
	TryToDropGem( object.Row, object.Col );
	
	int baseObjectIndex = object.BaseObjectIndex;
	if( object.IsTrapped && baseObjectIndex >= BO_68_TRAPED_SMALL_CHEST && baseObjectIndex <= BO_70_TRAPED_LARGE_CHEST ){
		int orientation = OrientationToTarget(object.Row, object.Col, player.Row, player.Col);
		int effectindex;
		switch (object.OpenState){
		case 0:	effectindex = MI_0_NONE_ARROW;		break;
		case 1:	effectindex = MI_27_FIRE_ARROW;		break;
		case 2:	effectindex = MI_82_STEAL_BOTTLES_FORM_BELT;				break;
		case 3:	effectindex = MI_27_FIRE_ARROW;	break;
		case 4:	effectindex = MI_42_LIGHTNING_NOVA;	break;
		case 5:	effectindex = MI_83_MANA_TRAP;				break;
		}
		CastMissile(object.Row, object.Col, player.Row, player.Col, orientation, effectindex, CT_1_MONSTER_AND_TRAP, -1, 0, 0, 0 );
		object.IsTrapped = false;
	}
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam2( 0, CMD_46_PLROPOBJ, playerIndex, objectIndex ); // TODO: check for multiply activation in net game, possible elixir duping
	}
}

//----- (00450E94) --------------------------------------------------------
void __fastcall OperateChest(int playerIndex, int objectIndex, int isCurrentIndex, int telekines)
{
	Player& player = Players[playerIndex];
	Object& chest = Objects[objectIndex];
	if (telekines) {
		if (CheckCastInSight(player.Row, player.Col, chest.Row, chest.Col))
			ActivateChest(playerIndex, objectIndex, isCurrentIndex);
	}
	else {
		ActivateChest(playerIndex, objectIndex, isCurrentIndex);
		
	}
}

//----- (00451041) --------------------------------------------------------
void __fastcall ActivateMushroomPatch( int playerIndex, int objectIndex )
{
	if( Quests[Q_1_BLACK_MUSHROOM].status == QS_2_IN_PROGRESS && Quests[Q_1_BLACK_MUSHROOM].status2 >= 2 ){
		Object& object = Objects[objectIndex];
		if( object.selectable ){
			if( !deltaload ){
				PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
			}
			object.selectable = 0;
			object.FrameIndex ++;
			if( !deltaload ){
				SpawnSomething(BI_330_BLACK_MUSHROOM, object.Row, object.Col, 0, 0, 0, 1);
				Quests[Q_1_BLACK_MUSHROOM].status2 = 3;
			}
		}
	}else if( !deltaload && playerIndex == CurrentPlayerIndex ){
		int soundIndex = 0;
		switch (Players[CurrentPlayerIndex].ClassID ){
		case PC_0_WARRIOR:		
		case PC_5_SAVAGE:		soundIndex = S_736_WAR_13;		break;
		case PC_1_ARCHER:		
		case PC_4_ROGUE:		soundIndex = S_633_ROG_13;		break;
		case PC_3_MONK:			soundIndex = S_850_MONK_13;		break;
		case PC_2_MAGE:			soundIndex = S_530_SOR_13;		break;
		}
		PlayGlobalSound(soundIndex);
	}
}

//----- (00451138) --------------------------------------------------------
void __fastcall ActivateLargeOgdenQuestChest( int playerIndex, int objectIndex )
{
	if( Quests[Q_7_OGDENS_SIGN].status2 == 2 ){
		Object& object = Objects[objectIndex];
		if( object.selectable ){
			if( !deltaload ){
				PlayLocalSound(S_24_I_CHEST, object.Row, object.Col);
			}
			object.selectable = 0;
			object.FrameIndex += 2;
			if( !deltaload ){
				SpawnSomething(BI_327_TAVERN_SIGN, object.Row, object.Col, 0, 0, 0, 1);
			}
		}
	}else if( !deltaload && playerIndex == CurrentPlayerIndex ){
		int soundIndex = 0;
		switch (Players[CurrentPlayerIndex].ClassID ){
		case PC_0_WARRIOR:		
		case PC_5_SAVAGE:		soundIndex = S_753_WAR_24;		break;
		case PC_1_ARCHER:		
		case PC_4_ROGUE:		soundIndex = S_644_ROG_24;		break;
		case PC_3_MONK:			soundIndex = S_861_MONK_24;		break;
		case PC_2_MAGE:			soundIndex = S_541_SOR_24;		break;
		}
		PlayGlobalSound(soundIndex);
	}
}

//----- (0045121C) --------------------------------------------------------
void __fastcall ActivateSlainHero(int playerIndex, int objectIndex)
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		object.selectable = 0;
		if( !deltaload ){
			CreateSpellBook(object.Row, object.Col, PS_7_TOWN_PORTAL, 0, 1);
			if( playerIndex == CurrentPlayerIndex ){
                NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
			}
		}
	}
}

// slain hero
//----- (00706CE0) --------------------------------------------------------
void __fastcall ActivateSlainHero_TH( int playerIndex, int objectIndex )
{
	int itemGfxId = 0, bookSpell = 0, itemCode = 0;
	int sound = 0;
	if( Objects[ objectIndex ].selectable ){
		Objects[ objectIndex ].selectable = 0;
		if( deltaload == 0 ){
			switch( Players[ playerIndex ].fullClassId ){ // this will need adjustments during item system upgrade <<<<<<<<<<<<<<<
			case PFC_WARRIOR: itemGfxId = by(Difficulty, 70, 220, 24); sound = S_732_WAR_09; itemCode = IC_4_MACE;   break;
			case PFC_INQUISITOR: itemGfxId = by(Difficulty, 70, 220, 24); sound = S_732_WAR_09; itemCode = IC_4_MACE;   break;
			case PFC_GUARDIAN: itemGfxId = by(Difficulty, 70, 220, 24); sound = S_732_WAR_09; itemCode = IC_4_MACE;   break;
			case PFC_TEMPLAR: itemGfxId = by(Difficulty, 70, 220, 24); sound = S_732_WAR_09; itemCode = IC_5_SHIELD;   break;
			case PFC_ARCHER: itemGfxId = by(Difficulty, 219, 108, 1500); sound = S_629_ROG_09; itemCode = /*IC_3_BOW*/IC_3_BOW;    break;
			case PFC_SCOUT: itemGfxId = by(Difficulty, 219, 108, 1500); sound = S_629_ROG_09; itemCode = /*IC_3_BOW*/IC_3_BOW;    break;
			case PFC_SHARPSHOOTER: itemGfxId = by(Difficulty, 219, 108, 1500); sound = S_629_ROG_09; itemCode = /*IC_3_BOW*/IC_3_BOW;    break;
			case PFC_TRAPPER: itemGfxId = by(Difficulty, 219, 108, 1500); sound = S_629_ROG_09; itemCode = /*IC_3_BOW*/IC_19_TRAP;    break;
			case PFC_MAGE: bookSpell = by(Difficulty, PS_35_ARCANE_STAR, PS_21_GOLEM, PS_23_TELEPORT); sound = S_526_SOR_09; break;
			case PFC_ELEMENTALIST:
				if (HasTrait(playerIndex, TraitId::Mamluk)) {
					itemGfxId = by(Difficulty, 70, 220, 24); sound = S_526_SOR_09; itemCode = IC_4_MACE;   break;
				}
				else {
					bookSpell = by(Difficulty, PS_35_ARCANE_STAR, PS_21_GOLEM, PS_23_TELEPORT); sound = S_526_SOR_09; break;
				}
			case PFC_DEMONOLOGIST: bookSpell = by(Difficulty, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON, PS_23_TELEPORT); sound = S_526_SOR_09; break;
			case PFC_NECROMANCER: bookSpell = by(Difficulty, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON, PS_23_TELEPORT); sound = S_526_SOR_09; break;
			case PFC_BEASTMASTER: bookSpell = by(Difficulty, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON, PS_23_TELEPORT); sound = S_526_SOR_09; break;
			case PFC_WARLOCK: bookSpell = by(Difficulty, PS_35_ARCANE_STAR, PS_21_GOLEM, PS_23_TELEPORT); sound = S_526_SOR_09; break;
			case PFC_MONK: itemGfxId = by(Difficulty, 1101, 390, 166); sound = S_904_; itemCode = IC_17_BELT; break;
			case PFC_KENSEI: itemGfxId = by(Difficulty, 1101, 390, 166); sound = S_904_; itemCode = IC_17_BELT; break;
			case PFC_SHUGOKI: itemGfxId = by(Difficulty, 1101, 390, 166); sound = S_904_; itemCode = IC_17_BELT; break;
			case PFC_SHINOBI: itemGfxId = by(Difficulty, 1101, 390, 166); sound = S_904_; itemCode = IC_17_BELT; break;
			case PFC_ROGUE: itemGfxId = by(Difficulty, 1367, 912, 357); sound = S_629_ROG_09; itemCode = IC_16_BOOTS;  break;
			case PFC_ASSASSIN: itemGfxId = by(Difficulty, 1367, 912, 357); sound = S_629_ROG_09; itemCode = IC_20_CLAW;  break;
			case PFC_IRON_MAIDEN: itemGfxId = by(Difficulty, 1367, 912, 357); sound = S_629_ROG_09; itemCode = IC_16_BOOTS;  break;
			case PFC_BOMBARDIER: itemGfxId = by(Difficulty, 1367, 912, 357); sound = S_629_ROG_09; itemCode = IC_18_FLASK;  break;
			case PFC_SAVAGE: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_15_GLOVE;    break;
			case PFC_BERSERKER: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_2_AXE;    break;
			case PFC_EXECUTIONER: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_2_AXE;    break;
			case PFC_THRAEX: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_1_SWORD;    break;
			case PFC_MURMILLO: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_5_SHIELD;    break;
			case PFC_DIMACHAERUS: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_15_GLOVE;    break;
			case PFC_SECUTOR: itemGfxId = by(Difficulty, 104, 211, 142); sound = S_732_WAR_09; itemCode = IC_7_HELM;    break;
			case PFC_DRUID: bookSpell = by(Difficulty, PS_2_HEALING, PS_12_FIREBLAST, PS_14_BALL_LIGHTNING, PS_35_ARCANE_STAR, PS_23_TELEPORT, PS_21_GOLEM, PS_21_GOLEM, PS_21_GOLEM, PS_21_GOLEM); sound = S_732_WAR_09; /*itemCode = IC_7_HELM;*/    break;
			}
			if( !bookSpell ){
			    DropItemFromSlain( Objects[ objectIndex ].Row, Objects[ objectIndex ].Col, 1, CurrentPlayerIndex == playerIndex, 0, 1 << playerIndex, itemCode);
				//GotItemFromSlainHero( Objects[ objectIndex ].Row, Objects[ objectIndex ].Col, itemCode, itemGfxId, 0, (int) Players );
			}else{
				CreateSpellBook( Objects[ objectIndex ].Row, Objects[ objectIndex ].Col, bookSpell, 0, (int) Players );
			}
			PlayLocalSound( sound, Players[ CurrentPlayerIndex ].Row, Players[ CurrentPlayerIndex ].Col );
			if( playerIndex == CurrentPlayerIndex ){
                NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
			}
		}
	}
}

//----- (0071CC00) --------------------------------------------------------
void __fastcall ActivateAngels( int playerIndex, int objectIndex )
{
	int pi; // ebx@1
	int oi; // esi@1
	int angelClick; // eax@3
	Quest *islandQuest; // eax@5

	pi = playerIndex;
	oi = objectIndex;
	if( Objects[ objectIndex ].selectable ){
		if( IsQuestOnLevel( Q_20_SUNLESS_SEA ) ){
			angelClick = CheckAngelClick( Objects[ oi ].bookIndex );
			if( angelClick ){
				if( angelClick == 1 ){
					CastMissile( Objects[ oi ].Row, Objects[ oi ].Col, Players[ pi ].Row, Players[ pi ].Col, 0, 11, 1, pi, 0, 0, 0 );
					CastMissile( Objects[ oi ].Row, Objects[ oi ].Col, Players[ pi ].Row, Players[ pi ].Col, 0, 12, 1, pi, 0, 0, 0 );
				}else{
					islandQuest = &Quests[ 20 ];
					islandQuest->status2 = 1;
					islandQuest->status3 = 1;
					PlayLocalSound( 1037, Objects[ oi ].Row, Objects[ oi ].Col );
				}
			}else{
				PlayLocalSound( 1036, Objects[ oi ].Row, Objects[ oi ].Col );
			}
		}
	}
}

//----- (004513A6) --------------------------------------------------------
void __fastcall ActivateLever2( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.FrameIndex == 1 ){
		object.FrameIndex = 2;
		for( int objectIndexIndex2 = 0; objectIndexIndex2 < ObjectsCount; objectIndexIndex2++ ){
			int objectIndex2 = ObjectActive[objectIndexIndex2];
			Object& object2 = Objects[objectIndex2];
			if( object2.BaseObjectIndex == object.MaybeSpeechIndex && object2.ShrineIndex == object.ShrineIndex ){
				object2.MaybeSpeechIndex = 1;
				object2.playAnimation = 0;
			}
		}

	}else{
		object.FrameIndex--;
		for( int objectIndexIndex2 = 0; objectIndexIndex2 < ObjectsCount; objectIndexIndex2++ ){
			int objectIndex2 = ObjectActive[objectIndexIndex2];
			Object& object2 = Objects[objectIndex2];
			if( object2.BaseObjectIndex == object.MaybeSpeechIndex && object2.ShrineIndex == object.ShrineIndex ){
				object2.MaybeSpeechIndex = 0;
				if( object2.OpenState ){
					object2.playAnimation = 1;
				}
			}
		}
	}
}

//----- (00451465) --------------------------------------------------------
void __fastcall ActivateSarcofagus( int playerIndex, int objectIndex, unsigned __int8 a3, int telekines )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];

	if (telekines && !CheckCastInSight(player.Row, player.Col, object.Row, object.Col))
		return;

	if( !LOBYTE(object.selectable) ){
		return;
	}
	if( !deltaload ){
		PlayLocalSound(S_72_I_SARC, object.Row, object.Col);
	}
	*(uchar*)&object.selectable = 0;
	if( !deltaload ){
		object.playAnimation = 1;
		object.AnimationDelayFrameCount = 3;
		SetRndSeed(object.Seed);
		if( object.ShrineIndex <= 2 ){
			CreateRndItem(object.Row, object.Col, 0, a3, 0, 1<<playerIndex);
			TryToDropGem( object.Row, object.Col );
		}
		if( object.ShrineIndex >= 8 ){// добавляем монстра
			ActivateAmbushMonster(object.MaybeSpeechIndex, object.Row, object.Col);
		}
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
		}
	}else{
		object.FrameIndex = object.FramesCount;
	}
}

//----- (0045153C) --------------------------------------------------------
void __fastcall ActivateCatacombDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
			ActivateCatacombDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
			ActivateCatacombDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (004515D6) --------------------------------------------------------
void __fastcall ActivateCaveDoor( int playerIndex, int objectIndex, char isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];
	int deltaRow = abs(object.Row - player.Row);
	int deltaCol = abs(object.Col - player.Col);
	if( deltaRow == 1 && deltaCol <= 1 ){
		if( object.BaseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
			ActivateCaveDoorCollateralDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
	if( deltaRow <= 1 && deltaCol == 1 ){
		if( object.BaseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
			ActivateCaveDoorMainDiagonal(playerIndex, objectIndex, isCurrentPlayer);
		}
	}
}

//----- (0045166E) --------------------------------------------------------
void __fastcall ActivatePedestalOfBlood( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.maybeFrameIndex == 3 ){
		return;
	}
	int inventoryIndex;
	if( FindItemInInventory(playerIndex, BI_289_BLOOD_STONE, inventoryIndex) ){
		ClearInventorySlot(playerIndex, inventoryIndex);
		object.FrameIndex++;
		object.maybeFrameIndex++;
	}
	if( object.maybeFrameIndex == 1 ){
		if( !deltaload ){
			PlayLocalSound(S_132_PUDDLE, object.Row, object.Col);
		}
		ChangeMapRectAfterScriptEvent(CurPattern.row, CurPattern.col + 3, CurPattern.row + 2, CurPattern.col + 7);
	}
	if( object.maybeFrameIndex == 2 ){
		if( !deltaload ){
			PlayLocalSound(S_132_PUDDLE, object.Row, object.Col);
		}
		ChangeMapRectAfterScriptEvent(CurPattern.row + 6, CurPattern.col + 3, CurPattern.row + CurPattern.sizeRow, CurPattern.col + 7);
	}
	if( object.maybeFrameIndex == 3 ){
		if( !deltaload ){
			PlayLocalSound(S_97_BLODSTAR, object.Row, object.Col);
		}

		//(object.startRow40, object.startCol40, object.endRow40, object.endCol40)
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);

		for (int i = 0; i < MonstersCount; i++)
		{
			int bossy = GameMode == GM_CLASSIC ? UM_C_902_RAGEHORN : UM_677_Guardian_of_Valor;
			if (Monsters[i].newBossId - 1 == bossy) {
				ChangeLightPos(Monsters[i].LightIndex, Monsters[i].Row, Monsters[i].Col);
			}
		}

		uchar* dunFilePtr = (uchar*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blood2_cls.DUN" : "Levels\\L2Data\\Blood2.DUN");
		for (int r = 0; r < 112; r++)
		{
			for (int c = 0; c < 112; c++)
			{
				LightMap[r][c] = StaticLightMap[r][c];
			}
		}
		LoadMapObjects(dunFilePtr, 2 * CurPattern.row, 2 * CurPattern.col);
		FreeMem(dunFilePtr);
		for (int col = Convert40to112(object.startCol40); col < Convert40to112(object.endCol40); col++) {
			for (int row = Convert40to112(object.startRow40); row < Convert40to112(object.endRow40); row++) {
				StaticLightMap[row][col] = LightMap[row][col]; //save valor torches light
			}
		}
				
		/*Player& player = Players[CurrentPlayerIndex];
		int valorRewardUniqueItemID = valorRewards[player.ClassID][Difficulty];
		if (HasTrait(playerIndex, TraitId::Mamluk)) {
			valorRewardUniqueItemID = by(Difficulty, 178, 179, 180, 139, 140, 141, 142, 143, 144); 
		}
		SpawnUnique(valorRewardUniqueItemID, 2 * StartPatternRow40 + 25, 2 * StartPatternCol40 + 19);*/
		object.selectable = 0;
	}
}

//----- (004517D4) --------------------------------------------------------
void __fastcall ApplyShrineEffect( int playerIndex, int objectIndex, int soundIndex )
{
	Player& player = Players[playerIndex];
	IsGoldSplitPanelVisible = false;

	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	//object.ShrineIndex = 3; Exe::Objects[objectIndex].ShrineIndex = object.ShrineIndex; // сверка действий алтарей (шрайнов)
	SetRndSeed(object.Seed);// 00451811

	object.selectable = 0;

	if( deltaload ){
		object.FrameIndex = object.FramesCount;
		object.playAnimation = 0;
	}else{
		PlayLocalSound(soundIndex, object.Row, object.Col);
		object.playAnimation = 1;
		object.AnimationDelayFrameCount = 1;
	}
	// 00451864
	SHRINE shrineIndex = (SHRINE)object.ShrineIndex;

	const int templar_faith_bonus = 0;// player.fullClassId == PFC_TEMPLAR ? 1 : 0; // caused crashes in MP - find out why..

	switch (shrineIndex) {
	case SH_0_Mysterious:// 0045187A update
		if (deltaload) {
			return;
		}
		{
			Player& currentPlayer = Players[CurrentPlayerIndex];
			if (!currentPlayer.BaseStrength
				|| !currentPlayer.BaseDexterity
				|| !currentPlayer.BaseMagic
				|| !currentPlayer.BaseVitality) {
				AddOnScreenMessage(OM_12_One_can_learn);
				return;
			}
		}
		// 00451882
		if (playerIndex != CurrentPlayerIndex) {
			return;
		}
		{
			ModifyPlayerStrength(playerIndex, -1);
			ModifyPlayerMagic(playerIndex, -1);
			ModifyPlayerDexterity(playerIndex, -1);
			ModifyPlayerVitality(playerIndex, -1);
			int addLevelPoints = LevelPointsToMax(playerIndex);
			if (player.fullClassId == PFC_TEMPLAR) {
				if (addLevelPoints > 5) {
					addLevelPoints = 5;
				}
			}
			else {
				if (addLevelPoints > 4) {
					addLevelPoints = 4;
				}
			}
			player.AvailableLvlPoints += addLevelPoints;
			LimitPlayerStats(playerIndex);
			AddOnScreenMessage(OM_12_One_can_learn);
		}
		break;
	case SH_1_Hidden:// 0045190C
	{
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		int onBodyItemsCount = 0;
		for (int slotIndex = 0; slotIndex < IS_Inventory; slotIndex++) {
			Item& item = player.OnBodySlots[slotIndex];
			if (item.ItemCode != IC_M1_NONE) {
				onBodyItemsCount++;
			}
		}
		if (onBodyItemsCount) {
			for (int slotIndex = 0; slotIndex < IS_Inventory; slotIndex++) {
				Item& item = player.OnBodySlots[slotIndex];
				if (item.ItemCode != IC_M1_NONE
					&& item.BaseDurability != ITEM_DUR_INDESTRUBLE
					&& item.BaseDurability)
				{
					++item.CurDurability;
					AddBaseDurWithLimit(&item);
					if (item.CurDurability > item.BaseDurability) {
						item.CurDurability = item.BaseDurability;
					}
				}
			}
			int hasDestructable = 0;
			for (int slotIndex = 0; slotIndex < IS_Inventory; slotIndex++) {
				Item& item = player.OnBodySlots[slotIndex];
				hasDestructable = CountDestructible(&item, hasDestructable);
			}
			if (hasDestructable) {
				int brokeThis;
				while (1) {
					brokeThis = RNG(IS_Inventory);
					Item& item = player.OnBodySlots[brokeThis];
					if (item.ItemCode != IC_M1_NONE
						&& item.BaseDurability != ITEM_DUR_INDESTRUBLE
						&& item.BaseDurability)
					{
						break;
					}
				}
				player.OnBodySlots[brokeThis].BaseDurability -= 2;
				player.OnBodySlots[brokeThis].CurDurability -= 2;
				if (player.OnBodySlots[brokeThis].BaseDurability <= 0) {
					player.OnBodySlots[brokeThis].BaseDurability = 1;
				}
				if (player.OnBodySlots[brokeThis].CurDurability <= 0) {
					player.OnBodySlots[brokeThis].CurDurability = 1;
				}
			}
		}
		AddOnScreenMessage(OM_13_New_strength);
		break;
	}
	case SH_2_Gloomy:// 00451A7D
	{
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) {
#ifndef OLD_GLOOMY
			player.activeBuffFlag |= BF_4_ARMOR_BLESS;
#else
			int incACparam = Difficulty + 1;
			// 00451AA8
			Item* currentItem = &player.OnBodySlots[IS_Head];
			if (currentItem->ItemCode != IC_M1_NONE) {
				currentItem->ArmorClass += incACparam;
			}
			// 00451AB6
			currentItem = &player.OnBodySlots[IS_Torso];
			if (currentItem->ItemCode != IS_Torso) {
				currentItem->ArmorClass += incACparam;
			}
			// 00451ACC
			currentItem = &player.OnBodySlots[IS_LeftHand];
			if (currentItem->ItemCode != IC_M1_NONE) {
				if (currentItem->ItemCode == IC_5_SHIELD) {
					currentItem->ArmorClass += incACparam;
				}
				else {
					currentItem->MaxDamage--;
					LimitToMin(currentItem->MaxDamage, currentItem->MinDamage);
				}
			}
			// 00451B0C
			currentItem = &player.OnBodySlots[IS_RightHand];
			if (currentItem->ItemCode != IC_M1_NONE) {
				if (currentItem->ItemCode == IC_5_SHIELD) {
					currentItem->ArmorClass += incACparam;
				}
				else {
					currentItem->MaxDamage--;
					LimitToMin(currentItem->MaxDamage, currentItem->MinDamage);
				}
			}
			// 00451B4C
			for (int inventoryItemIndex = 0; inventoryItemIndex < player.InvItemCount; inventoryItemIndex++) {
				currentItem = &player.InventorySlots[inventoryItemIndex];
				bool isWeapon = currentItem->ItemCode >= IC_1_SWORD && currentItem->ItemCode <= IC_4_MACE || currentItem->ItemCode == IC_10_STAFF;
				bool isArmor = currentItem->ItemCode >= IC_5_SHIELD && currentItem->ItemCode <= IC_9_HEAVY_ARMOR;
				if (isArmor) {
					// 00451B90
					currentItem->ArmorClass += 2;
				}
				else if (isWeapon) {
					// 00451B99
					currentItem->MaxDamage--;
					LimitToMin(currentItem->MaxDamage, currentItem->MinDamage);
				}
			}
#endif
			AddOnScreenMessage(OM_14_Those_who_defend);
		}
		break;
	}
	case SH_3_Weird:// 00451BD9
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) {
#ifndef OLD_WEIRD
			player.activeBuffFlag |= BF_5_DAMAGE_BLESS;
#else
			Item& weaponLeft = player.OnBodySlots[IS_LeftHand];
			if (weaponLeft.ItemCode != IC_M1_NONE && weaponLeft.ItemCode != IC_5_SHIELD) {
				weaponLeft.MaxDamage++;
			}
			// 00451C1C
			Item& weaponRight = player.OnBodySlots[IS_RightHand];
			if (weaponRight.ItemCode != IC_M1_NONE && weaponRight.ItemCode != IC_5_SHIELD) {
				weaponRight.MaxDamage++;
			}
			// 00451C3B
			for (int itemIndex = 0; itemIndex < IS_70_40_Inv_Count; itemIndex++) {
				Item& item = player.InventorySlots[itemIndex];
				int itemCode = item.ItemCode;
				if (itemCode == IC_1_SWORD
					|| itemCode == IC_4_MACE
					|| itemCode == IC_3_BOW
					|| itemCode == IC_2_AXE
					|| itemCode == IC_10_STAFF) {
					item.MaxDamage++;
				}
			}
#endif
			AddOnScreenMessage(OM_15_The_sword_of);
		}
		break;
	case SH_4_Moonlit:
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		player.activeBuffFlag |= BF_14_ACCU_BLESS;
		AddOnScreenMessage(OM_110_Powers_and_Treasure);
		break;
	case SH_11_Magical:// 00451C8B
		if (deltaload) {
			return;
		}
		CastMissile(player.Row, player.Col, player.Row, player.Col, player.dir, MI_13_MANA_SHIELD, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * Dungeon->genType, PS_11_MANA_SHIELD);
		if (playerIndex != CurrentPlayerIndex) {
			return;
		}
		AddOnScreenMessage(OM_16_While_the_spirit);
		break;
	case SH_5_Stone:// 00451CDB
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) { // issue #146 here, also note: jewelry cannot have charges and must NOT be recharged by this shrine
			//for (int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++) {
			//	Item& item = player.OnBodySlots[itemIndex];
			//	if (item.ItemCode == IC_10_STAFF) {
			//		item.CurCharges = item.BaseCharges;
			//	}
			//}
			//// 00451D2C
			//for (int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++) {
			//	Item& item = player.InventorySlots[itemIndex];
			//	if (item.ItemCode == IC_10_STAFF) {
			//		item.CurCharges = item.BaseCharges;
			//	}
			//}
			//// 00451D63
			//for (int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++) {
			//	Item& item = player.BeltInventory[itemIndex];
			//	if (item.ItemCode == IC_10_STAFF) {// Посох на поясе?
			//		item.CurCharges = item.BaseCharges;
			//	}
			//}
			VisualTrade_ForEachPlayerItem([](Item* item)
				{
					item->CurCharges = item->BaseCharges;
				});
			AddOnScreenMessage(OM_17_The_powers_of);
		}
		break;
	case SH_6_Religious:// 00451D90
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) {
			/*for (int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++) {
				Item& item = player.OnBodySlots[itemIndex];
				item.CurDurability = item.BaseDurability;
			}

			for (int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++) {
				Item& item = player.InventorySlots[itemIndex];
				item.CurDurability = item.BaseDurability;
			}

			for (int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++) {
				Item& item = player.BeltInventory[itemIndex];
				item.CurDurability = item.BaseDurability;
			}*/
			VisualTrade_ForEachPlayerItem([](Item* item)
				{
					item->CurDurability = item->BaseDurability;
				});
			AddOnScreenMessage(OM_18_Time_cannot);
		}
		break;
	case SH_7_Enchanted:// 00451E2C
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_7_EXP_BLESS;
		//end code here
		AddOnScreenMessage(OM_19_Magic_is_not);
		break;

		#if 0
		{
			// Вычисляем количество доступных спэлов
			int spellBitMaskPosition = 1i64;
			uint availableSpellsCount = 0;
			for( int spellIndex = 0; spellIndex < PLAYER_SPELLS_AMOUNT; spellIndex++ ){
				if( player.AvailableSpellMask & (spellBitMaskPosition << spellIndex) ){
					availableSpellsCount++;
				}
			}

			// 00451E9D
			if( availableSpellsCount > 1 ){// меньше чем с двумя так не выйдет
				// Выбираем какой спэл будем уменьшать
				uint redusedSpellIndex;
				do{
					redusedSpellIndex = RNG(PLAYER_SPELLS_AMOUNT);
					if( player.AvailableSpellMask & (spellBitMaskPosition << redusedSpellIndex) ){
						if( player.SpellLevels[redusedSpellIndex] > 0 ){
							break;
						}
					}
				}while( 1 );

				// все кроме выбранного увеличиваем а выбранный уменьшаем.
				// в оригинале вначале все увеличиваем потом случайный уменьшаем
				for( int spellIndex = 0; spellIndex < PLAYER_SPELLS_AMOUNT; spellIndex++ ){
					if( player.AvailableSpellMask & (spellBitMaskPosition << spellIndex) ){
						if( spellIndex == redusedSpellIndex ){
							player.SpellLevels[spellIndex]--;
						}else{
							if( player.SpellLevels[spellIndex] < MaxSpellLevel_200 ){
								player.SpellLevels[spellIndex]++;
							}
						}
					}
				}
			}
		}
		#endif

	case SH_8_Thaumaturgic:// 00451F57
		for (int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++) {
			Object& object = Objects[ObjectActive[objectIndex]];
			int baseObjectIndex = object.BaseObjectIndex;
			if (baseObjectIndex == BO_5_SMALL_CHEST || baseObjectIndex == BO_6_CHEST || baseObjectIndex == BO_7_LARGE_CHEST) {
				if (!object.selectable) {
					object.Seed = GetRndSeed();
					object.selectable = 1;
					object.FrameIndex -= 2;
				}
			}
		}
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) {
			AddOnScreenMessage(OM_20_What_once_was);
		}
		break;

	case SH_9_Fascinating:// 00451FDB
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_8_GOLD_BLESS;
		//end code here
		AddOnScreenMessage(OM_21_Wealth_obtain);
		break;
		#if 0
		{
			int spellIndex = PS_1_FIREBOLT;
			player.AvailableSpellMask |= (1I64 << (spellIndex - 1));
			player.SpellLevels[spellIndex] += 2;
			if( player.SpellLevels[spellIndex] > MaxSpellLevel_200 ){
				player.SpellLevels[spellIndex] = MaxSpellLevel_200;
			}
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana -= player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}
			AddOnScreenMessage(OM_21_Wealth_obtain);
		}
		#endif
	case SH_10_Cryptic:// 0045210F
		if (deltaload) {
			return;
		}
		CastMissile(player.Row, player.Col, player.Row, player.Col, player.dir, MI_42_LIGHTNING_NOVA, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * Dungeon->genType, PS_18_LIGHTNING_NOVA);
		if (playerIndex != CurrentPlayerIndex) {
			return;
		}
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		AddOnScreenMessage(OM_22_Arcane_power);
		break;
	case SH_12_Eldritch:// 00452185
		if (deltaload) {
			return;
		}
		if (playerIndex == CurrentPlayerIndex) {
			for (int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++) {
				Item& item = player.InventorySlots[itemIndex];
				if (item.ItemCode != IC_0_OTHER) {
					continue;
				}
				int magicCode = item.MagicCode;
				uint newMagicCode;
				if (magicCode == MC_3_POTION_OF_HEALING || magicCode == MC_6_POTION_OF_MANA) {
					// 004521E0
					if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {// Night Kin can't use Holy Water potions, so he gets no effect here
						continue;
					}
					else{
						newMagicCode = MC_18_POTION_OF_REJUVENATION;
					}
				}
				else if (magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_7_POTION_OF_FULL_MANA) {
					// 00452250
					if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
						continue;
					}
					else{
						newMagicCode = MC_19_POTION_OF_FULL_REJUVENATION;
					}
				}
				else {
					continue;
				}
				auto seed = item.seed;
				SetPlayerHandItem(item, FindBaseItemWithMagicCode(newMagicCode));
				item.SetSeed(seed);
				item.IsReqMet = true;
			}
			// 004522C6
			for (int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++) {
				Item& item = player.BeltInventory[itemIndex];
				if (item.ItemCode != IC_0_OTHER) {
					continue;
				}
				int magicCode = item.MagicCode;
				uint newMagicCode;
				if (magicCode == MC_3_POTION_OF_HEALING || magicCode == MC_6_POTION_OF_MANA) {
					// 004521E0
					if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {// Night Kin can't use Holy Water potions, so he gets no effect here
						continue;
					}
					else {
						newMagicCode = MC_18_POTION_OF_REJUVENATION;
					}
				}
				else if (magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_7_POTION_OF_FULL_MANA) {
					// 00452250
					if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
						continue;
					}
					else {
						newMagicCode = MC_19_POTION_OF_FULL_REJUVENATION;
					}
				}
				else {
					continue;
				}
				auto seed = item.seed;
				SetPlayerHandItem(item, FindBaseItemWithMagicCode(newMagicCode));
				item.SetSeed(seed);
				item.IsReqMet = true;
			}
			AddOnScreenMessage(OM_24_Bright_and_Dark);
		}
		break;
	case SH_13_Eerie:// 004523D5
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		if (GameMode == GM_NIGHTMARE) {
			if (RNG(2) == 1) {
				ModifyPlayerMagic(playerIndex, 1 + templar_faith_bonus + RNG(3));
			}
			else {
				ModifyPlayerMagic(playerIndex, -1);
			}
		}
		else if (GameMode == GM_SPEEDRUN){
			ModifyPlayerMagic(playerIndex, 2 + templar_faith_bonus);
		}
		else if (GameModeType() == GAME_MODE_TYPE::IRON) {
			ModifyPlayerMagic(playerIndex, RNG(2) + 1 + templar_faith_bonus);
		}
		else {
			ModifyPlayerMagic(playerIndex, 1 + templar_faith_bonus);
		}
		LimitPlayerStats(playerIndex);
		AddOnScreenMessage(OM_25_Wisdom_may_come);
		break;
	case SH_14_Divine:// 00452404
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if( 2 * Dungeon->level >= 9 && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))){
			// 0045244A
			CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 0, 1); 
			CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 0, 1); 
		}else{
			// 0045242E
			CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_7_POTION_OF_FULL_MANA, 0, 1);
			CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_2_POTION_OF_FULL_HEALING, 0, 1);
		}
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		player.CurLife = player.MaxCurLife;
		player.BaseLife = player.MaxBaseLife;
		AddOnScreenMessage(OM_26_Drink_and_be_ref);
		break;
	case SH_15_Holy:// 004524D7
		if( deltaload ){
			return;
		}
		{
			int row, col, cell;
			for( int tryesCount = 1; tryesCount < 112 * 112; tryesCount++ ){
				row = RNG(112);
				col = RNG(112);
				cell = col + 112 * row;
				if( !TileBlockWalking[FineMap[ 0 ][ cell ]] && !ObjectsMap[ 0 ][ cell ] && !MonsterMap[ 0 ][ cell ] ){
					break;
				}
			}
			CastMissile(player.Row, player.Col, row, col, player.dir, MI_34_ETHEREAL, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * Dungeon->genType, PS_25_ETHEREAL );
			if( playerIndex != CurrentPlayerIndex ){
				return;
			}
			AddOnScreenMessage(OM_27_Magical_damage);
			break;
		}
	case SH_16_Sacred:// 0045259D
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_9_MAGIC_FIND;
		//end code here
		AddOnScreenMessage(OM_28_Energy_comes);
		break;
		#if 0
		{
			int spellIndex = PS_30_CHARGED_BOLT;
			player.AvailableSpellMask |= (1i64 << (spellIndex - 1));
			int newSpellLevel = player.SpellLevels[spellIndex] + 2;
			if( newSpellLevel < MaxSpellLevel_200 ) ++player.SpellLevels[ spellIndex ];
			if( newSpellLevel < MaxSpellLevel_200 ) ++player.SpellLevels[ spellIndex ]; // в 1.216 на два уровня увеличивается
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana = player.BaseMana - player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}

			AddOnScreenMessage(OM_28_Energy_comes);
			break;
		}
		#endif
	case SH_17_Spiritual:// 004526D1
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN) {
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
			AddOnScreenMessage(OM_69_Portal_Shrine_Nightmare);
		}
		else {
			for( int r = 0; r < InvRows; ++r ) for( int c = 0; c < player.invCols; ++c ){
				int invIndex = r * InvCols + c;
				if (player.InvUsed[invIndex]) {
					continue;
				}
				int goldCount = Dungeon->level + (Difficulty * 24);// 1-24, 25-48, 49-72, 73-96, 97-120, 121-144, 145-168, 169-192, 193-216
				goldCount = goldCount + RNG(goldCount + 1);
				goldCount += goldCount * player.goldFind / 100;
				LimitToMin( goldCount, 1 );
				int itemIndex = player.InvItemCount;
				Item& item = player.InventorySlots[itemIndex];
				memcpy(&item, &ItemGoldBuffer, sizeof( Item ));
				item.MakeSeed();
				player.InvItemCount++;
				player.InvUsed[invIndex] = player.InvItemCount;
				item.amount = goldCount;
				player.TotalGold += goldCount;
				ChangeGoldGraphicOnQuantity(playerIndex, player.InvItemCount - 1);
			}
			AddOnScreenMessage(OM_29_Riches_abound);
		}
		break;
	case SH_18_Spooky:// 004527D9
		if( deltaload ){
			return;
		}
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_30_Where_avarice);
		}else{
			AddOnScreenMessage(OM_31_Blessed_by);
			Player& currentPlayer = Players[CurrentPlayerIndex];
			currentPlayer.CurLife = currentPlayer.MaxCurLife;
			currentPlayer.BaseLife = currentPlayer.MaxBaseLife;
			currentPlayer.CurMana = currentPlayer.MaxCurMana;
			currentPlayer.BaseMana = currentPlayer.MaxBaseMana;
		}
		break;
	case SH_19_Abandoned:// 00452867
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (GameMode == GM_NIGHTMARE) {
			if (RNG(2) == 1) {
				ModifyPlayerDexterity(playerIndex, 1 + templar_faith_bonus + RNG(3));
			}
			else {
				ModifyPlayerDexterity(playerIndex, -1);
			}
		}
		else if (GameMode == GM_SPEEDRUN) {
			ModifyPlayerDexterity(playerIndex, 2 + templar_faith_bonus);
		}
		else if (GameModeType() == GAME_MODE_TYPE::IRON) {
			ModifyPlayerDexterity(playerIndex, RNG(2) + 1 + templar_faith_bonus);
		}
		else {
			ModifyPlayerDexterity(playerIndex, 1 + templar_faith_bonus);
		}
		LimitPlayerStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_32_The_hands_of_men);
		}
		break;
	case SH_20_Creepy:// 004528A2
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (GameMode == GM_NIGHTMARE) {
			if (RNG(2) == 1) {
				ModifyPlayerStrength(playerIndex, 1 + templar_faith_bonus + RNG(3));
			}
			else {
				ModifyPlayerStrength(playerIndex, -1);
			}
		}
		else if (GameMode == GM_SPEEDRUN) {
			ModifyPlayerStrength(playerIndex, 2 + templar_faith_bonus);
		}
		else if (GameModeType() == GAME_MODE_TYPE::IRON) {
			ModifyPlayerStrength(playerIndex, RNG(2) + 1 + templar_faith_bonus);
		}
		else {
			ModifyPlayerStrength(playerIndex, 1 + templar_faith_bonus);
		}
		LimitPlayerStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_33_Strength_may_be);
		}
		break;
	case SH_21_Quiet:// 004528DD
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (GameMode == GM_NIGHTMARE) {
			if (RNG(2) == 1) {
				ModifyPlayerVitality(playerIndex, 1 + templar_faith_bonus + RNG(3));
			}
			else {
				ModifyPlayerVitality(playerIndex, -1);
			}
		}
		else if (GameMode == GM_SPEEDRUN) {
			ModifyPlayerVitality(playerIndex, 2 + templar_faith_bonus);
		}
		else if (GameModeType() == GAME_MODE_TYPE::IRON) {
			ModifyPlayerVitality(playerIndex, RNG(2) + 1 + templar_faith_bonus);
		}
		else {
			ModifyPlayerVitality(playerIndex, 1 + templar_faith_bonus);
		}
		LimitPlayerStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			AddOnScreenMessage(OM_34_Power_may_come);
		}
		break;
	case SH_22_Secluded:// 00452918
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_10_REGEN_BLESS;
		//end code here
		AddOnScreenMessage(OM_35_Thy_corse);
		break;
		/*if( playerIndex == CurrentPlayerIndex ){
			for( int cell = 0; cell < 40*40; cell++ ){
				AutoMap[ 0 ][ cell ] = 1;
			}
			AddOnScreenMessage(OM_35_The_way_is_made);
		}*/
	case SH_23_Ornate:// 00452950
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_11_SPELL_BLESS;
		//end code here
		AddOnScreenMessage(OM_36_Salvation_comes);
		break;

		/*{
			int spellIndex = PS_31_HOLY_BOLT;
			player.AvailableSpellMask |= (1I64 << (spellIndex - 1));
			int newSpellLevel = player.SpellLevels[spellIndex] + 2;
			if( newSpellLevel < MaxSpellLevel_200 ) ++player.SpellLevels[ spellIndex ];
			if( newSpellLevel < MaxSpellLevel_200 ) ++player.SpellLevels[ spellIndex ];
			int newMaxCurMana = player.MaxCurMana - player.MaxBaseMana;
			int newCurMana = player.CurMana - player.BaseMana;
			player.BaseMana = player.BaseMana - player.MaxBaseMana / 10;
			player.CurMana -= player.MaxBaseMana / 10;
			player.MaxCurMana -= player.MaxBaseMana / 10;
			player.MaxBaseMana -= player.MaxBaseMana / 10;
			if( (player.CurMana & (-64)) <= 0 ){
				player.CurMana = newCurMana;
				player.BaseMana = 0;
			}
			if( (player.MaxCurMana & (-64)) <= 0 ){
				player.MaxCurMana = newMaxCurMana;
				player.MaxBaseMana = 0;
			}
			AddOnScreenMessage(OM_36_Salvation_comes);
			break;
		}*/
	case SH_24_Glimmering:// 00452A84
    {
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		/*auto identifyItemIfNeed = []( Item& item )
            {
                if( item.MagicLevel && !item.Identified ){
                    item.Identified = 1;
                    UpdateOneItem( &item );
                }
            };*/
		
		/*for( int itemIndex = 0; itemIndex < IS_10_7_Inventory; itemIndex++ ){
			Item& item = player.OnBodySlots[itemIndex];
			identifyItemIfNeed( item );
		}
		for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
			Item& item = player.InventorySlots[itemIndex];
			identifyItemIfNeed( item );
		}
		for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
			Item& item = player.BeltInventory[itemIndex];
			identifyItemIfNeed( item );
		}*/
		VisualTrade_ForEachPlayerItem([](Item* item)
			{
				if (item->MagicLevel && !item->Identified) {
					item->Identified = 1;
					UpdateItem(*item);
				}
			});
		AddOnScreenMessage(OM_37_Mysteries_are);
		break;
    }
	case SH_25_Tainted:// 00452B4C
		if( deltaload ){
			return;
		}
		//put new code here
		
		
		//end code here
		if (MaxCountOfPlayersInGame == 1) {
			player.activeBuffFlag |= BF_12_LIGHT_CURSE;
			AddOnScreenMessage(OM_38_Fate_doth_take);
		}
		else {
			if (playerIndex == CurrentPlayerIndex) {
				AddOnScreenMessage(OM_38_Fate_doth_take);
			}
			else {
				/*AddOnScreenMessage(OM_39_No_good_deed);
				int strModifier =
				    magModifier =
				    dexModifier =
				    vitModifier = RNG(11) < 5 ? -1 : 1;
				ModifyPlayerStrength(CurrentPlayerIndex, strModifier);
				ModifyPlayerMagic(CurrentPlayerIndex, magModifier);
				ModifyPlayerDexterity(CurrentPlayerIndex, dexModifier);
				ModifyPlayerVitality(CurrentPlayerIndex, vitModifier);
				CheckStatsOnShrine(CurrentPlayerIndex);*/
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 1);
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 1);
				AddOnScreenMessage(OM_38_Fate_doth_take);
			}
		}
		break;
	case SH_26_Oily:// 00452BEA
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_44_That_not_kill);
		// Oily shrine gives stats according to class
		if (player.ClassID == PC_0_WARRIOR) {
			ModifyPlayerStrength(CurrentPlayerIndex, 2 + templar_faith_bonus);
		}
		else if (player.ClassID == PC_1_ARCHER) {
			ModifyPlayerDexterity(CurrentPlayerIndex, 2);
		}
		else if (player.ClassID == PC_2_MAGE) {
			if (HasTrait(playerIndex, TraitId::Mamluk)) {// Elementalist with Mamluk trait turns into a Str/Dex fighter
				ModifyPlayerStrength(CurrentPlayerIndex, 1);
				ModifyPlayerDexterity(CurrentPlayerIndex, 1);
			}
			else { ModifyPlayerMagic(CurrentPlayerIndex, 2); }// standard caster Mag gains
		}
		else if (player.ClassID == PC_3_MONK) {
			ModifyPlayerStrength(CurrentPlayerIndex, 1);
			ModifyPlayerDexterity(CurrentPlayerIndex, 1);
		}
		else if (player.ClassID == PC_4_ROGUE) {
			ModifyPlayerDexterity(CurrentPlayerIndex, 1);
			ModifyPlayerMagic(CurrentPlayerIndex, 1);
		}
		else { // SAVAGE
			ModifyPlayerVitality(CurrentPlayerIndex, 2); 
		}
		LimitPlayerStats(playerIndex);
		CastMissile(object.Row, object.Col, player.Row, player.Col, player.dir, MI_5_FIRE_WALL_SEGMENT, CT_1_MONSTER_AND_TRAP, 0, 2 * Dungeon->level + 2 + Difficulty * 45, 0, 0);
		break;
	case SH_27_Glowing:// 00452CB6
		// Removed because 5% xp is too high on high levels
		//put new code here
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		if (MaxCountOfPlayersInGame == 1) { //Single player drops 2 random oils
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_31_OIL_OF_REPAIR_I, 1);
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_31_OIL_OF_REPAIR_I, 1);
			AddOnScreenMessage(OM_76_FX_of_time);
		}
		else { //Multiplayer drops
			if( Dungeon->level < 13 && Difficulty == DL_0_HORROR ){ //2 holy waters if horror 1-12 lvls
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 1);
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 1);
			}
			else{ //2 random elixirs if after horror 12
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
				CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
				AddOnScreenMessage(OM_69_Portal_Shrine_Nightmare);
			}			
		}
		break;
	case SH_28_Mendicant_s:// 00452D9F
		if (deltaload || playerIndex != CurrentPlayerIndex) {
			return;
		}
		//put new code here
		player.activeBuffFlag |= BF_13_CRIT_BLESS;
		//end code here
		AddOnScreenMessage(OM_73_Kill);
		break;

#ifdef comment // бывшый 28
		if( LoadingOtherPlayers || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			AddOnScreenMessage(OM_46_Give_and_you);
			int price = player.TotalGold / 2;
			AddPlayerExperience(CurrentPlayerIndex, player.CharLevel, price);
			ClearGoldByInventoryAsPrice(price);
			LimitPlayerStats(playerIndex);
		}
		break;
#endif
	case SH_29_Sparkling: // this will definitely require revision because of new XP table!
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_47_Some_experience);
		{
			int dlvl = Dungeon->level, mplier = by(Difficulty, 250, 250, 250, 484, 1000, 2000, 3500, 5500, 7500), 
				addz = by(Difficulty, 0, 20, 40, 60, 80, 80, 80, 80, 80), gg = Difficulty == DL_0_HORROR ? 250 : 0;
			i64 chunk = mplier * ((dlvl + addz) * (dlvl + addz));
			// HOR:		from 500 - 750 on DLVL - 1			to 100k - 200k on DLVL - 20;
			// PRG:		from 110k - 220k on DLVL - 1		to 400k - 800k on DLVL - 20;
			// DOO:		from 420k - 840k on DLVL - 1		to 900k - 1800K on DLVL - 20;
			// CRC:		from 1.8m - 3.6m on DLVL - 1		to 3.1m - 6.2m on DLVL - 20;
			// TRM:		from 6.56m - 13.12m on DLVL - 1		to 10m - 20m on DLVL - 20;
			// AGO:		from 13m - 26m on DLVL - 1			to 20m - 40m on DLVL - 20;
			// TER:		from 23m - 46m on DLVL - 1			to 35m - 70m on DLVL - 20;
			// DST:		from 36m - 72m on DLVL - 1			to 55m - 110m on DLVL - 20;
			// INF:		from 49m - 98m on DLVL - 1			to 75m - 150m on DLVL - 20;
			i64 formula = chunk + Rng32to64(chunk + 1) + gg; 
			AddPlayerExperience(CurrentPlayerIndex, formula, player.Row, player.Col);
		}
		CastMissile(object.Row, object.Col, player.Row, player.Col, player.dir, MI_11_FLASH_FR, CT_1_MONSTER_AND_TRAP, 0, Difficulty * 60 + 3 * Dungeon->level + 2, 0, 0 );
		LimitPlayerStats(playerIndex);
		break;
	case SH_30_Town:// 00452E34
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if (GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN) {
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
			CreateItemIgnoringDungeonLevel(object.Row, object.Col, 0, IC_0_OTHER, MC_10_ELIXIR_OF_STRENGTH + RNG(4), 1);
			AddOnScreenMessage(OM_69_Portal_Shrine_Nightmare);
		}
		else if (GameModeType() == GAME_MODE_TYPE::IRON) {
			if (2 * Dungeon->level >= 9 && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))) {
				// 0045244A
				CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 0, 1);
				CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 0, 1);
			}
			else {
				// 0045242E
				CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_7_POTION_OF_FULL_MANA, 0, 1);
				CreateTypeItem(object.Row, object.Col, 0, IC_0_OTHER, MC_2_POTION_OF_FULL_HEALING, 0, 1);
			}
			player.CurMana = player.MaxCurMana;
			player.BaseMana = player.MaxBaseMana;
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
			AddOnScreenMessage(OM_26_Drink_and_be_ref);
		}
		else {
			AddOnScreenMessage(OM_48_Theres_no_place);
			CastMissile(object.Row, object.Col, player.Row, player.Col, player.dir, MI_10_TOWN_PORTAL, CT_1_MONSTER_AND_TRAP, 0, 0, 0, 0 );
		}
		break;
	case SH_31_Shimmering:// 00452E8D
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		AddOnScreenMessage(OM_49_Spiritual_energy);
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
		break;
	case SH_32_Solar:// 00452EE1
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			time_t curTime = time(0);
			int hour = localtime(&curTime)->tm_hour;
			if( hour > 20 || hour < 4 ){ // ночь
				AddOnScreenMessage(OM_53_You_refreshed);
				ModifyPlayerVitality(CurrentPlayerIndex, 1 + templar_faith_bonus);
			}else if( hour >= 4 && hour <= 12 ){ // утро
				AddOnScreenMessage(OM_50_You_agile);
				ModifyPlayerDexterity(CurrentPlayerIndex, 1 + templar_faith_bonus);
			}else if( hour > 12 && hour <= 18 ){ // день
				AddOnScreenMessage(OM_51_You_stronger);
				ModifyPlayerStrength(CurrentPlayerIndex, 1 + templar_faith_bonus);
			}else if( hour > 18 && hour <= 20 ){// > 18 <=20  // вечер
				AddOnScreenMessage(OM_52_You_wiser);
				ModifyPlayerMagic(CurrentPlayerIndex, 1 + templar_faith_bonus);
			}
		}
		LimitPlayerStats(playerIndex);
		break;
	case SH_33_Murphy_s:// 00452F8E
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		{
			AddOnScreenMessage(OM_54_That_can_break);
			bool isItemBroken = 0;
			for( int inventoryIndex = 0; inventoryIndex < IS_Inventory; inventoryIndex++ ){
				Item& item = player.OnBodySlots[inventoryIndex];
				if( item.ItemCode != -1 && !RNG(10) && item.CurDurability != ITEM_DUR_INDESTRUBLE && item.CurDurability ){
					item.CurDurability /= 2;
					isItemBroken = true;
					break;
				}
			}
			if( !isItemBroken ){
				//if (RNG(100) >= player.CharLevel / 3 + 40) { // chance removed. now it works always
					int gold_removed = player.TotalGold / (player.CharLevel * 10);
					LimitToMin(gold_removed, 1);
					ClearGoldByInventoryAsPrice(gold_removed);
				//}
			}
		}
		break;
	}
	// 00453040
	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam2( 0, CMD_46_PLROPOBJ, playerIndex, objectIndex );
	}
}

//----- (004530FB) --------------------------------------------------------
void __fastcall ActivateLibraryBook( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		if( !deltaload ){
			PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);
		}
		object.selectable = 0;
		object.FrameIndex += 2;
		if( !deltaload ){
			SetRndSeed(object.Seed);
			int magicCode;
			if( RNG(5) ){
				magicCode = MC_21_RELIC_NEED_NO_TARGET;
			}else{
				magicCode = MC_24_BOOKS;
			}
			CreateTypeItem(object.Row, object.Col, 0, 0, magicCode, isCurrentPlayer, 0);
			if( playerIndex == CurrentPlayerIndex ){
                NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
			}
		}
	}
}

//----- (0045319B) --------------------------------------------------------
void __fastcall ActivateBookcase( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];

	if( !object.selectable ){
		return;
	}
	if( !deltaload ){
		PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);
	}
	object.selectable = 0;
	object.FrameIndex -= 2;
	if( deltaload ){
		return;
	}
	SetRndSeed(object.Seed);
	CreateTypeItem(object.Row, object.Col, 0, 0, MC_24_BOOKS, isCurrentPlayer, 0);
	if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
		Monster& monster = Monsters[SummonMonsters_Count];
		if( strcmp(monster.Name, UniqueMonsters[GameMode == GM_CLASSIC ? UM_C_870_ZHAR_THE_MAD : UM_2_Zhar_the_Mad].NamePtr) == 0 ){
			if( monster.ActivationCounter == 255 && monster.CurrentLife > 0){
				monster.speechIndex = SP_149;
				FixMonsterPosition(0, monster.Orientation);
				monster.goal = MG_5_ATTACK2;
				monster.CurAction = A_17_TALK;
			}
		}
	}
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
	}
}

//----- (0045327D) --------------------------------------------------------
void __fastcall ActivateDecapitatedBody( int playerIndex, int objectIndex, uchar isCurrentPlayer )
{
	Object& object = Objects[objectIndex];
	if( object.selectable ){
		object.selectable = 0;
		if( !deltaload ){
			SetRndSeed(object.Seed);
			CreateRndItem(object.Row, object.Col, 0, isCurrentPlayer, 0, 1<<playerIndex);
			TryToDropGem( object.Row, object.Col );
			if( playerIndex == CurrentPlayerIndex ){
                NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
			}
		}
	}
}

//----- (004532E1) --------------------------------------------------------
void __fastcall ActivateArmorRack( int playerIndex, int objectIndex, uchar isCurrentPlayer )// #armor rack
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		return;
	}
	object.selectable = 0;
	++object.FrameIndex;
	if( deltaload ){
		return;
	}
	SetRndSeed(object.Seed);
	int mayBe = RNG(2);
	int itemCode;
	int flag = 0;
	#ifdef OLD_CODE
	int magicAffixFlag;
	if( Dungeon->level <= 5 ){
		itemCode = IC_6_LIGHT_ARMOR; // Light armor
		magicAffixFlag = 1;
	}else if( Dungeon->level >= 6 && Dungeon->level <= 9 ){
		itemCode = IC_8_MID_ARMOR; // Medium armor
		magicAffixFlag = mayBe;
	}else if( Dungeon->level >= 10 && Dungeon->level <= 12 ){
		itemCode = IC_9_HEAVY_ARMOR; // Plate armor
		magicAffixFlag = 0;
	}else if( (Dungeon->level >= 13 && Dungeon->level <= 16) || Dungeon->level >= 17 ){
		itemCode = IC_9_HEAVY_ARMOR; // Plate armor
		magicAffixFlag = 1;
	}
	#else
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS) 
		|| Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN)) {// add night kin check here
		itemCode = IC_11_GOLD;
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
		itemCode = RngFrom(IC_7_HELM, IC_15_GLOVE, IC_16_BOOTS, IC_17_BELT, IC_5_SHIELD);
	}
	else {
		if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && Players[CurrentPlayerIndex].fullClassId == PFC_ELEMENTALIST) {
			switch (Difficulty) {
			default:
			case 0:		itemCode = IC_6_LIGHT_ARMOR; flag |= IF_MAMLUK_HARNESS;	break;
			case 1:		itemCode = IC_8_MID_ARMOR;	 flag |= IF_MAMLUK_HARNESS;	break;
			case 2:		itemCode = IC_9_HEAVY_ARMOR; flag |= IF_MAMLUK_HARNESS;	break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		itemCode = RngFrom(IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR);  flag |= IF_MAMLUK_HARNESS; break;
			}
		}
		else if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {// EXCLUDING CASTER ARMORS
			switch (Difficulty) {
			default:
			case 0:		itemCode = IC_6_LIGHT_ARMOR; flag |= IF_CASTER_HARNESS;	break;
			case 1:		itemCode = IC_8_MID_ARMOR;	 flag |= IF_CASTER_HARNESS;	break;
			case 2:		itemCode = IC_9_HEAVY_ARMOR; flag |= IF_CASTER_HARNESS;	break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		itemCode = RngFrom(IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR);  flag |= IF_CASTER_HARNESS; break;
			}
		}
		else if (Players[CurrentPlayerIndex].fullClassId == PFC_IRON_MAIDEN) {// EXCLUDING NON THORNS ARMORS
			switch (Difficulty) {
			default:
			case 0:		itemCode = IC_6_LIGHT_ARMOR; flag |= IF_NON_THORNS_HARNESS;	break;
			case 1:		itemCode = IC_8_MID_ARMOR;	 flag |= IF_NON_THORNS_HARNESS;	break;
			case 2:		itemCode = IC_9_HEAVY_ARMOR; flag |= IF_NON_THORNS_HARNESS;	break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		itemCode = RngFrom(IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR);  flag |= IF_NON_THORNS_HARNESS; break;
			}
		}
		else if (GameMode == GM_CLASSIC){
			if (Difficulty < 1) {
				if (Dungeon->level < 6) { itemCode = IC_6_LIGHT_ARMOR; }
				else if (Dungeon->level < 10) { itemCode = IC_8_MID_ARMOR; }
				else { itemCode = IC_9_HEAVY_ARMOR; }
			}
			else { 
				itemCode = IC_9_HEAVY_ARMOR; 
			}
			
		}
		else {
			switch (Difficulty) {
			default:
			case 0:		itemCode = IC_6_LIGHT_ARMOR;	break;
			case 1:		itemCode = IC_8_MID_ARMOR;		break;
			case 2:		itemCode = IC_9_HEAVY_ARMOR;	break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:		itemCode = RngFrom(IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR); break;
			}
		}
	}
	#endif
	CreateTypeItem(object.Row, object.Col, 1 /*magicAffixFlag*/, itemCode, 0, isCurrentPlayer, 0, flag, true); // min qlvl check for rack
	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex ); // rack activating message
	}
}

//----- (004533B8) --------------------------------------------------------
void __fastcall ActivateGoatShrine( int playerIndex, int objectIndex, int soundIndex )
{
	Object& object = Objects[objectIndex];
	SetRndSeed(object.Seed);
	object.ShrineIndex = GetRandomValidShrineEffect();
	ApplyShrineEffect(playerIndex, objectIndex, soundIndex);
	object.AnimationDelayFrameCount = 2;
}

//----- (00453403) --------------------------------------------------------
int GetRandomValidShrineEffect()
{
	int shrineIndex;
	bool isFound = false;
	do{
		isFound = false;
		do{
			shrineIndex = RNG(34);
			if( Dungeon->level >= MinShrineDungeonLevels[shrineIndex]
			 && Dungeon->level <= MaxShrineDungeonLevels[shrineIndex] ){
				if( shrineIndex != 8 ){
					isFound = true;
				}
			}
		}while( !isFound );

		if( MaxCountOfPlayersInGame == 1 ){
			if( ShrineAppearsMode[shrineIndex] == 2 ){
				isFound = false;
			}
		}else{
			if( ShrineAppearsMode[shrineIndex] == 1 ){
				isFound = false;
			}
		}

	}while( !isFound );
	return shrineIndex;
}

//----- (00453466) --------------------------------------------------------
void __fastcall ActivateCauldron( int playerIndex, int objectIndex, int soundIndex )
{
	Object& object = Objects[objectIndex];
	SetRndSeed(object.Seed);
	object.ShrineIndex = GetRandomValidShrineEffect();
	ApplyShrineEffect(playerIndex, objectIndex, soundIndex);
	object.FrameIndex = 3;
	object.playAnimation = 0;
}

//----- (004534B8) --------------------------------------------------------
void __fastcall ActivateFountain( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	Player& player = Players[playerIndex];

	SetRndSeed(object.Seed);
	if( object.BaseObjectIndex == BO_66_BLOOD_FOUNTAIN ){
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		if( player.CurLife < player.MaxCurLife ){
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
			if (GameMode == GM_CLASSIC){
				if (player.CurLife < player.MaxCurLife) {
					player.BaseLife += 64;
					player.CurLife += 64;
				}
				if (player.CurLife > player.MaxCurLife) {
					player.CurLife = player.MaxCurLife;
					player.BaseLife = player.MaxBaseLife;
				}
			}
			else {
				int curLife = player.CurLife;
				int lifeDelta = uint(player.MaxCurLife - curLife) >> 5; // возможно ошибка, зарегистрирован отрицательный BaseLife
				if (lifeDelta < 64) lifeDelta = 64;
				player.CurLife = lifeDelta + curLife;
				player.BaseLife += lifeDelta;
				if (player.CurLife > player.MaxCurLife) {
					player.CurLife = player.MaxCurLife;
					player.BaseLife = player.MaxBaseLife;
				}
			}
		}else{
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
		}
	}else if( object.BaseObjectIndex == BO_76_PURIFYING_SPRING ){
		if( deltaload || playerIndex != CurrentPlayerIndex ){
			return;
		}
		Quest& q = Quests[ Q_36_CLEAR_WATER];
		if( Dungeon == q.questDungeon && q.status == QS_3_COMPLETE ){
			int emptyContainer = 0;
			if( FindItemInInventory(playerIndex, BI_2511_MT_CONTAINERS, emptyContainer) ){
				ClearInventorySlot(playerIndex, emptyContainer);
				SpawnSomething(BI_2512_FULL_CONTAINERS, player.Row, player.Col, 1);
				PlayLocalSound(S_14_QUESTDON, object.Row, object.Col);
			}				
		}
		if( player.CurMana < player.MaxCurMana ){
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
			if (GameMode == GM_CLASSIC) {
				if (player.CurMana < player.MaxCurMana) {
					player.BaseMana += 64;
					player.CurMana += 64;
				}
				if (player.CurMana > player.MaxCurMana) {
					player.CurMana = player.MaxCurMana;
					player.BaseMana = player.MaxBaseMana;
				}
			}
			else {
				int curMana = player.CurMana;
				int manaDelta = uint(player.MaxCurMana - curMana) >> 5;
				if (manaDelta < 64) manaDelta = 64;
				player.CurMana = manaDelta + curMana;
				player.BaseMana += manaDelta;
				if (player.CurMana > player.MaxCurMana) {
					player.CurMana = player.MaxCurMana;
					player.BaseMana = player.MaxBaseMana;
				}
			}
		}else{
			PlayLocalSound( S_115_FOUNTAIN, object.Row, object.Col );
		}
	}else if( object.BaseObjectIndex == BO_81_MURKY_POOL ){
		if( !deltaload ){
			CastMissile(player.Row, player.Col, player.Row, player.Col, player.dir, MI_39_INFRAVISION, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * Dungeon->genType, 0 );
		}
	}else if( object.BaseObjectIndex == BO_82_FOUNTAIN_OF_TEARS ){
		if( !deltaload && playerIndex == CurrentPlayerIndex ){
			//CastMissile(player.Row, player.Col, player.Row, player.Col, player.dir, MI_79_REFLECT, CT_M1_MAP_OBJECT, playerIndex, 0, 2 * DungeonType );
			player.activeBuffFlag |= BF_6_RESIST_BLESS;
			PlayLocalSound(S_1215_FOUNTAIN_OF_TEARS, object.Row, object.Col);
		}
	}
	return;
}

//----- (004537C5) --------------------------------------------------------
void __fastcall ActivateWeaponRack( int playerIndex, int objectIndex, int isCurrentPlayer )// #racks, #rack drops, #drop from weapon racks
{
	Object& object = Objects[objectIndex];
	
	if( object.selectable ){
		object.selectable = 0;
		object.FrameIndex++;
		SetRndSeed(object.Seed);
		int itemCode = IC_1_SWORD;
		int flag = 0;
		// if( MaxCountOfPlayersInGame == 1) // MP smart drop
		if( deltaload ) return;
		const Player& player = Players[CurrentPlayerIndex];
		if (GameMode == GM_CLASSIC) { itemCode = RngFrom(IC_1_SWORD, IC_4_MACE, IC_10_STAFF, IC_2_AXE, IC_3_BOW); }
		else if (player.gameChanger & BIT(GC_14_CLEAN_HANDS) || player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
			itemCode = IC_11_GOLD;
		}
		else if (is(player.fullClassId, PFC_WARRIOR, PFC_INQUISITOR)) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Ranger)) {
				itemCode = IC_3_BOW;
				flag |= IF_NO_CROSSBOW;
			}
			else {
				switch (RNG(4)) {
				case 0: itemCode = IC_1_SWORD;		break;
				case 1: itemCode = IC_3_BOW; flag |= IF_NO_BOW; break;
				case 2: itemCode = IC_4_MACE;		break;
				case 3: itemCode = HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister) ? IC_1_SWORD : IC_5_SHIELD;		break;
				}
			}
		}
		else if (player.fullClassId == PFC_TEMPLAR) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
				itemCode = RngFrom(IC_1_SWORD, IC_4_MACE);
			}
			else {
				itemCode = RngFrom(IC_1_SWORD, IC_4_MACE, IC_22_MALLET);
			}
		}
		else if (player.fullClassId == PFC_GUARDIAN) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
				itemCode = IC_23_PISTOL;
			}
			else {
				flag |= SD_1_TWO_HANDS_ONLY;
				itemCode = RngFrom(IC_1_SWORD, IC_1_SWORD, IC_4_MACE, IC_4_MACE, IC_3_BOW);
			}
		}
		else if (player.fullClassId == PFC_SAVAGE) {
			flag |= SD_1_TWO_HANDS_ONLY;
			itemCode = RngFrom(IC_1_SWORD, IC_1_SWORD, IC_4_MACE, IC_4_MACE, IC_3_BOW);
		}
		else if (player.fullClassId == PFC_ARCHER
			|| player.fullClassId == PFC_SCOUT
			|| player.fullClassId == PFC_SHARPSHOOTER) {
			itemCode = IC_3_BOW;
		}
		else if (player.fullClassId == PFC_TRAPPER) {
			itemCode = RngFrom(IC_19_TRAP, IC_5_SHIELD);
		}
		else if (player.fullClassId == PFC_MAGE
			|| player.fullClassId == PFC_ELEMENTALIST
			|| player.fullClassId == PFC_WARLOCK
			|| player.fullClassId == PFC_DEMONOLOGIST
			|| player.fullClassId == PFC_NECROMANCER) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				itemCode = RngFrom(IC_3_BOW, IC_1_SWORD);
			}
			else {
				itemCode = RngFrom(IC_10_STAFF, IC_5_SHIELD);
			}
		}
		else if (player.fullClassId == PFC_BEASTMASTER) {
			itemCode = RngFrom(IC_1_SWORD, IC_5_SHIELD);
		}
		else if (player.fullClassId == PFC_MONK) {
			itemCode = RngFrom(IC_3_BOW, IC_10_STAFF, IC_10_STAFF);
		}
		else if (player.fullClassId == PFC_KENSEI) {
			switch (RNG(5)) {
			case 3:
			case 0: itemCode = IC_1_SWORD;		break;
			case 1: itemCode = IC_3_BOW;		break;
			case 4:
			case 2: itemCode = IC_4_MACE;		break;
			}
		}
		else if (player.fullClassId == PFC_SHUGOKI) {
			switch (RNG(3)) {
			case 1:  itemCode = IC_3_BOW;		break;
			default: itemCode = IC_2_AXE;		break;// double chance for axe
			}
		}
		else if (player.fullClassId == PFC_SHINOBI) {
			itemCode = RngFrom(IC_1_SWORD, IC_1_SWORD, IC_4_MACE, IC_4_MACE, IC_5_SHIELD, IC_21_KNIFE);
		}
		else if (player.fullClassId == PFC_ROGUE) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
				itemCode = IC_2_AXE;
			}
			else {
				itemCode = RngFrom(IC_1_SWORD, IC_4_MACE);
			}
		}
		else if (player.fullClassId == PFC_ASSASSIN) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
				itemCode = RngFrom(IC_20_CLAW, IC_5_SHIELD);
			}
			else {
				itemCode = RngFrom(IC_20_CLAW, IC_5_SHIELD, IC_21_KNIFE);
			}
		}
		else if (player.fullClassId == PFC_IRON_MAIDEN) {
			switch (RNG(4)) {
			case 0: itemCode = HasTrait(CurrentPlayerIndex, TraitId::TwoTowers) ? IC_5_SHIELD : IC_1_SWORD;	break;
			case 1: itemCode = IC_3_BOW;		break;
			case 2: itemCode = HasTrait(CurrentPlayerIndex, TraitId::TwoTowers) ? IC_5_SHIELD : IC_4_MACE;		break;
			case 3: itemCode = IC_5_SHIELD;		break;
			}
		}
		else if (player.fullClassId == PFC_BOMBARDIER) {
			itemCode = RngFrom(IC_18_FLASK, IC_5_SHIELD);
		}
		else if (player.fullClassId == PFC_THRAEX
			|| player.fullClassId == PFC_SECUTOR
			|| player.fullClassId == PFC_MURMILLO) {
			switch (RNG(6)) {
			case 0:
			case 1: itemCode = IC_1_SWORD;		break;
			case 2: itemCode = IC_3_BOW;		break;
			case 3:
			case 4: itemCode = IC_4_MACE;		break;
			case 5: itemCode = IC_5_SHIELD;		break;
			}
		}
		else if (player.fullClassId == PFC_BERSERKER
			|| player.fullClassId == PFC_EXECUTIONER) {
			switch (RNG(3)) {
			case 0: itemCode = IC_3_BOW;		break;
			case 1:
			case 2: itemCode = IC_2_AXE;		break;
			}
		}
		else if (player.fullClassId == PFC_DIMACHAERUS) {
			switch (RNG(5)) {
			case 0:
			case 1: itemCode = IC_1_SWORD;		break;
			case 2: itemCode = IC_3_BOW;		break;
			case 3:
			case 4: itemCode = IC_4_MACE;		break;
			}
		}
		else if (player.fullClassId == PFC_DRUID) {
			itemCode = IC_2_AXE;
		}
		else {
			itemCode = IC_11_GOLD;
		}
		// macicCode = -1 means ignoring magicCode when selecting base item
		// also could be a preset magic code for flask/staff/etc subtype, if would need
		int magicCode = MC_0_NONE;
		switch( itemCode ){
		case IC_10_STAFF: magicCode = -1; break; // ignore
		case IC_18_FLASK: magicCode = -1; break; // ignore
		case IC_19_TRAP : magicCode = -1; break; // ignore
		//case IC_20_CLAW: magicCode = -1; break; // ignore CLAWS? Is it required?
		}
		int isGoodItem = Dungeon->genType > DT_1_CHURCH;
		CreateTypeItem(object.Row, object.Col, isGoodItem, itemCode, magicCode, isCurrentPlayer, 0, flag, true); // min qlvl check for rack
		if( playerIndex == CurrentPlayerIndex ){
            NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
		}
	}
}

//----- (00453871) --------------------------------------------------------
void __fastcall ActivateQuestBook2( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( MaxCountOfPlayersInGame == 1 ){ //Original SP logic
		if( !object.selectable || deltaload || Speech_IsPanelVisible || playerIndex != CurrentPlayerIndex ){
			return;
		}
	}else if( Dungeon->isQuest && Dungeon == DUN_34_THE_PIT && object.Row == 48 ){ // New MP logic.  Make sure GrimSpike quest books are deactivated once and for all, so they won't reset .status3 upon re-entry and reading again.
		Objects[ObjIndex(48, 28)].selectable = 0;
		Objects[ObjIndex(48, 38)].selectable = 0;
		Objects[ObjIndex(28, 34)].selectable = 0;
		object.FrameIndex = object.OpenState;
		if( deltaload || Speech_IsPanelVisible || playerIndex != CurrentPlayerIndex ){
			return;
		}
	}else if( Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS && (object.bookIndex == 1 || object.bookIndex == 2 || object.bookIndex == 3) && object.selectable ){ // For MP Na-Krul door sync and book status update.
		object.FrameIndex = object.OpenState;
		if( playerIndex != CurrentPlayerIndex && !deltaload ){ // Na-Krul door sync for other players on the same map.  deltaload == 1 should have playerIndex == -1.
			if( Quests[Q_16_HORAZON_S_DEMONS].status2 == 2 ){
				OpenNakrul(/*isWeak*/true);
				return;
			}
			return;
		}else if( Quests[Q_16_HORAZON_S_DEMONS].status2 >= 2 ){ // If door is already opened, make sure books and levers are untouchable after deltaload.
			Objects[ObjIndex(43, 19)].selectable = 0;//Books
			Objects[ObjIndex(43, 20)].selectable = 0;
			Objects[ObjIndex(43, 21)].selectable = 0;
			Objects[ObjIndex(44, 24)].selectable = 0;//Levers
			Objects[ObjIndex(44, 26)].selectable = 0;
			return;
		}else if( !object.selectable || deltaload || Speech_IsPanelVisible || playerIndex != CurrentPlayerIndex ){ // For current player who reads the book or is loading map.  This check is needed, otherwise the player who reads book will end up in an infinite loop.
			return;
		}
	}else{ // All other quest books in MP
		object.FrameIndex = object.OpenState;
		if( ! object.selectable || deltaload || Speech_IsPanelVisible || playerIndex != CurrentPlayerIndex ){
			return;
		}
	}
	object.FrameIndex = object.OpenState;
	PlayLocalSound(S_63_I_INVSCROL, object.Row, object.Col);

	if( IsQuestOnLevel(Q_16_HORAZON_S_DEMONS) ){
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		if( horazonQuest.status == QS_1_ACCEPT ){
			horazonQuest.status = QS_2_IN_PROGRESS;
		}
		horazonQuest.talkToFlag = true;
	}

	if( Dungeon->isQuest && Dungeon == DUN_34_THE_PIT && object.Row == 48 ){
		// убираем выделение
		Objects[ObjIndex(48, 28)].selectable = 0;
		Objects[ObjIndex(48, 38)].selectable = 0;
		Objects[ObjIndex(28, 34)].selectable = 0;
		Quest& horazonQuest = Quests[Q_16_HORAZON_S_DEMONS];
		horazonQuest.status3 = 2;// книга прочитана
		if( MaxCountOfPlayersInGame == 1 ){
			HorazonGrimspikeDelay = 180;
		}else{
			HorazonGrimspikeDelay = 0; // Since we are NOT making a new Net CMD to sync this timer in MP (other players have this timer = 0 after quest status sync), we immediately open the door to ensure GrimSpike spawns at the same time for all players!
		}
		if( playerIndex == CurrentPlayerIndex ){
			NetSendCmdQuest(1, Q_16_HORAZON_S_DEMONS);
		}
	}

	if( Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS && object.bookIndex ){
		if( MaxCountOfPlayersInGame == 1 ){ // Original SP logic
			if( ReadUberBook(object.bookIndex) ){
				OpenNakrul(/*isWeak*/true);
				return;
			}
		}else{ // In MP, book reading global vars are local to the current player.  This block of code should only be reached by the player who reads the book.  Door sync is done at the beginning of the current func.
			if( ReadUberBook(object.bookIndex) ){
				OpenNakrul(/*isWeak*/true);
				if( playerIndex == CurrentPlayerIndex ){
					NetSendCmdQuest(1, Q_16_HORAZON_S_DEMONS); // Player who solves the puzzle must update quest status for other players, so they will have boss door open.
					NetSendCmdParam1(0, CMD_45_OPERATEOBJ, objectIndex); // Early return case should not skip object sync.
				}
				return;
			}
		}
	}
	#if comment
	DungeonQuest& horazonQuest = DungeonQuests[Q_16_HORAZON_S_DEMONS];
	if( object.bookIndex && Dungeon->isQuest && Dungeon == DUN_33_SUMMONING_ROOMS / *&& Dungeon == DUN_24_CRYPT_4 * / ){
		if( horazonQuest.Status2 < 1/ *открыт босс 1* / && ReadUberBook(object.bookIndex) ){
			horazonQuest.Status2 = 1;
			// открыть комнату босса и ещё какие то действия выполнить
			return;
		}
	}
	DungeonQuest& uberDiabloQuest = DungeonQuests[Q_21_UBER_DIABLO];
	if( Dungeon >= DUN_21_CRYPT_1 && Dungeon <= DUN_24_CRYPT_4 ){
		uberDiabloQuest.Status = QS_2_IN_PROGRESS;
		uberDiabloQuest.TalkToFlag = 1;
		uberDiabloQuest.SpeechIndex = object.MaybeSpeechIndex;
	}
	#endif
	StartSpeech(object.MaybeSpeechIndex);
	if (playerIndex == CurrentPlayerIndex) {
		NetSendCmdParam1( 0, CMD_45_OPERATEOBJ, objectIndex );
	}
}

//----- (0045393E) --------------------------------------------------------
void __fastcall ActivateVileStand( int playerIndex, int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( object.selectable && !deltaload && !Speech_IsPanelVisible && playerIndex == CurrentPlayerIndex ){
		object.FrameIndex++;
		object.selectable = 0;
		SpawnSomething(BI_33_STAFF_OF_LAZARUS, object.Row, object.Col, 0, 0, 0, 1);
	}
}

//----- (004539A3) --------------------------------------------------------
void __fastcall SyncOpObject( int playerIndex, int netCommand, int objectIndex )
{
	int baseObjectIndex;          // eax@1
	baseObjectIndex = Objects[objectIndex].BaseObjectIndex;

	switch (baseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:		ActivateChurcheDoorFromNetwork(playerIndex, netCommand, objectIndex);	return;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:	ActivateCatacombDoorFromNetwork(playerIndex, netCommand, objectIndex);	return;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:		ActivateCaveDoorFromNetwork(playerIndex, netCommand, objectIndex);		return;
	case BO_28_SKULL_LEVER:
	case BO_4_LEVER:								ActivateLever(playerIndex, objectIndex);								return;
	case BO_48_SARCOPHAGUS:							ActivateSarcofagus(playerIndex, objectIndex, 0);						return;
	case BO_86_QUEST_BOOK:							ActivateQuestBook2(playerIndex, objectIndex);							return;
	case BO_80_CAULDRON:							ActivateCauldron(playerIndex, objectIndex, S_101_CALDRON);				return;
	case BO_81_MURKY_POOL:
	case BO_82_FOUNTAIN_OF_TEARS:					ActivateFountain(playerIndex, objectIndex);								return;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:						ActivateWeaponRack(playerIndex, objectIndex, 0);						return;
	case BO_94_MUSHROOM_PATCH:						ActivateMushroomPatch(playerIndex, objectIndex);						return;
	case BO_96_SLAIN_HERO:							ActivateSlainHero_TH(playerIndex, objectIndex);							return;
	case BO_97_OGDEN_CHEST:							ActivateLargeOgdenQuestChest(playerIndex, objectIndex);					return;
	case BO_73_PEDESTAL_OF_BLOOD:					ActivatePedestalOfBlood(playerIndex, objectIndex);						return;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:								ActivateArmorRack(playerIndex, objectIndex, 0);							return;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:							ActivateBookcase(playerIndex, objectIndex, 0);							return;
	case BO_61_ANCIENT_TOME:
	case BO_64_LIBRARY_BOOK:						ActivateLibraryBook(playerIndex, objectIndex, 0);						return;
	case BO_67_DECAPITATED_BODY:					ActivateDecapitatedBody(playerIndex, objectIndex, 0);					return;
	case BO_79_GOAT_SHRINE:							ActivateGoatShrine(playerIndex, objectIndex, S_118_PORTAL);				return;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2:							ApplyShrineEffect(playerIndex, objectIndex, S_69_I_MAGIC);				return;
	case BO_71_BOOK_OF_THE_BLIND:
	case BO_72_BOOK_OF_BLOOD:						
	case BO_88_STEEL_TOME:							ActivateQuestBook(playerIndex, objectIndex);							return;
	case BO_5_SMALL_CHEST:
	case BO_6_CHEST:
	case BO_7_LARGE_CHEST:
	case BO_68_TRAPED_SMALL_CHEST:
	case BO_69_TRAPED_CHEST:
	case BO_70_TRAPED_LARGE_CHEST:					ActivateChest(playerIndex, objectIndex, 0);								return;
	case BO_98_HELLFORGE:							ActivateAnvil(playerIndex, objectIndex);								return;
	case BO_41_MYTHICAL_BOOK:						ActivateMythicalBook(playerIndex, objectIndex);							return;//COB entry
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:	ActivateAncientTomeOrBookOfVileness(playerIndex, objectIndex);			return;//COB reward book
	}
}

//----- (00453B78) --------------------------------------------------------
void __fastcall ActivateChurcheDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == CMD_43_OPENDOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == CMD_44_CLOSEDOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
				ActivateChurchDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
				ActivateChurchDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453BEC) --------------------------------------------------------
void __fastcall ActivateCatacombDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == CMD_43_OPENDOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == CMD_44_CLOSEDOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_42_CATACOMB_DOOR_MAIN_DIAGONAL ){
				ActivateCatacombDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCatacombDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453C60) --------------------------------------------------------
void __fastcall ActivateCaveDoorFromNetwork( int playerIndex, int netCommand, int objectIndex )
{
	Object& objectOnMap = Objects[objectIndex];
	if( playerIndex != CurrentPlayerIndex ){
		bool someFlag = false;
		if( netCommand == CMD_43_OPENDOOR ){
			if( objectOnMap.OpenState == OS_0_CLOSED ){
				someFlag = true;
			}
		}
		if( netCommand == CMD_44_CLOSEDOOR ){
			if( objectOnMap.OpenState == OS_1_OPEN ){
				someFlag = true;
			}
		}
		if( someFlag ){
			if( objectOnMap.BaseObjectIndex == BO_74_CAVE_DOOR_MAIN_DIAGONAL ){
				ActivateCaveDoorMainDiagonal(-1, objectIndex, 0);
			}
			if( objectOnMap.BaseObjectIndex == BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL ){
				ActivateCaveDoorCollateralDiagonal(-1, objectIndex, 0);
			}
		}
	}
}

//----- (00453CD4) --------------------------------------------------------
void __fastcall DamageDestroyableOnMapObject( int playerIndex, int objectIndex )
{
	Player& player = Players[playerIndex];
	int damage;
	if( playerIndex == -1 ){
		damage = 10;
	}else{
		int damageFromItem = RngFromRange( player.MinDamageFromItem, player.MaxDamageFromItem);
		damage = player.BaseDamage + player.ItemsAddDamage + damageFromItem * player.ItemsAddDamagePercents / 100 + damageFromItem;
		LimitToMin(damage, 1);
	}
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	if( baseObjectIndex >= BO_20_CRUCIFIED_SKELETON && baseObjectIndex <= BO_22_CRUCIFIED_SKELETON_3 ){
		InteractWithCrucifiedSkeleton(playerIndex, objectIndex);
	}else if( baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP || baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		InteractWithBarrel(playerIndex, objectIndex, damage, 0, 1);
	}
}

//----- (00453D77) --------------------------------------------------------
void __fastcall InteractWithCrucifiedSkeleton(int playerIndex, int objectIndex)
{
	Object& object = Objects[objectIndex];

	if (object.selectable == 0) {//To avoid infinite loop after net message is sent.
		return;
	}

	object.playAnimation = 1;
	object.FrameIndex = 1;
	object.AnimationDelayFrameCount = 1;
	object.isBlockWalking = 1;
	object.objectAllowWalking = 1;
	object.destructable = -1;
	object.selectable = 0;

	if (deltaload) {//Copied from InteractWithBarrel(), not 100% sure about these fields...
		object.FrameIndex = object.FramesCount;
		object.AnimationDelayIndex = 0;
		object.AnimationDelayFrameCount = 1000;
		return;
	}

	for( int objectIndex = 0; objectIndex < ObjectsCount; objectIndex++ ){//If hit by missile
		Object& currentObject = Objects[ObjectActive[objectIndex]];
		int baseObjectIndex = currentObject.BaseObjectIndex;
		if( baseObjectIndex == BO_20_CRUCIFIED_SKELETON 
	     || baseObjectIndex == BO_21_CRUCIFIED_SKELETON_2 
	     || baseObjectIndex == BO_22_CRUCIFIED_SKELETON_3 ){
			if( object.bookIndex == currentObject.bookIndex && currentObject.destructable != -1 ){
				return;
			}
		}
	}

	if( !deltaload ){
		PlayLocalSound(S_68_I_LEVER, object.Row, object.Col);
	}
	ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);

	if (playerIndex == CurrentPlayerIndex || playerIndex == -1) {//Hit by player/monster missile means playerIndex = -1
		NetSendCmdParam2(0, CMD_47_BREAKOBJ, playerIndex, objectIndex);
	}
}

// Drop from barrel 
//----- (00453E3A) --------------------------------------------------------
void __fastcall InteractWithBarrel( int playerIndex, int objectIndex, int damage, int oneHitDestruction, int a5 )
{
	uchar isBlocked; // in original (uchar*)&oneHitDestruction + 3 
	Object& object = Objects[objectIndex];
	if( object.selectable == 0 ){
		return;
	}
	if( oneHitDestruction ){// уничтожить сразу
		object.ShrineIndex = 0;
	}else{
		object.ShrineIndex -= damage;// hp бочки
		if( playerIndex != CurrentPlayerIndex && object.ShrineIndex <= 0 ){
			object.ShrineIndex = 1;
		}
	}
	if( object.ShrineIndex > 0 ){
		if( !deltaload ){
			PlayLocalSound(S_54_I_INVBOW, object.Row, object.Col);
		}
		return;
	}
	object.ShrineIndex = 0;
	object.playAnimation = 1;
	object.FrameIndex = 1;
	object.AnimationDelayFrameCount = 1;
	object.isBlockWalking = 0;
	object.objectAllowWalking = 1;
	object.destructable = -1;
	object.selectable = 0;
	object.InAction = 1;
	if( deltaload ){
		object.FrameIndex = object.FramesCount;
		object.AnimationDelayIndex = 0;
		object.AnimationDelayFrameCount = 1000;
		return;
	}
	if( object.BaseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		int soundIndex = 0;
		switch( Dungeon->graphType ){
		case DT_6_ABYSS: soundIndex = S_18_I_PODPOP8; break;
		case DT_5_CRYPT: soundIndex = S_20_I_URNPOP3; break;
		default: soundIndex = S_16_I_BARLFIRE; break;
		}
		PlayLocalSound(soundIndex, object.Row, object.Col);
		for( int colDelta = -1; colDelta <= 1; ++colDelta ){
			for( int rowDelta = -1; rowDelta <= 1; ++rowDelta ){
				int row = object.Row + rowDelta;
				int col = object.Col + colDelta;
				int monsterNum = MonsterMap[ row ][ col ];
				if( monsterNum > 0 ){
					MvM_Ranged(monsterNum - 1, M1_TRAP, 1, 4, 0, MI_1_FIREBOLT, 0);
				}
				int playerNum = PlayerMap[ row ][ col ];
				if( playerNum > 0 ){
					int damage = ModifyBarrelExplodeDamage() + 16;
					MvP_Ranged(playerNum - 1, M1_TRAP, 0, damage/2, damage, MI_1_FIREBOLT, 0, 0, &isBlocked);
				}
				int objectNum = ObjectsMap[ row ][ col ];
				if( objectNum > 0 ){
					Object& object2 = Objects[objectNum - 1];
					if( object2.BaseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE && object2.destructable != -1 ){
						InteractWithBarrel(playerIndex, objectNum - 1, damage, 1, a5);
					}
				}
			}
		}
		if( Dungeon->isQuest && Dungeon == DUN_35_PASSAGE_OF_FIRE ){
			Quests[Q_17_FALLEN_ANGEL].status2 = 1;
			Quests[Q_17_FALLEN_ANGEL].status3 = 1;
			if (MaxCountOfPlayersInGame != 1) {
				NetSendCmdQuest(true, Q_17_FALLEN_ANGEL);//.status2 is used for spawning Red TP.  Must sync quest status when a player hits the last Barrel, so another player on a different map can see the Red TP after he loads into Passage of Fire in the current game.
			}
		}
	}else{
		int soundIndex = 0;
		switch( Dungeon->graphType ){
		case DT_6_ABYSS: soundIndex = S_19_I_PODPOP5; break;
		case DT_5_CRYPT: soundIndex = S_21_I_URNPOP2; break;
		default: soundIndex = S_17_I_BARREL; break;
		}
		PlayLocalSound(soundIndex, object.Row, object.Col);
		SetRndSeed(object.Seed);
		if( object.MaybeSpeechIndex <= 1 ){
			if( object.BookNameIndex ){
				CreateRndItem(object.Row, object.Col, 0, a5, 0, 1<<playerIndex ); // выбор дропа из бочек
			}else{
				CreateRndUseful(playerIndex, object.Row, object.Col, a5);
			}
			TryToDropGem( object.Row, object.Col );
		}
		if( object.MaybeSpeechIndex >= 8 ){
			ActivateAmbushMonster(object.OpenState, object.Row, object.Col);
		}
	}

	if( playerIndex == CurrentPlayerIndex ){
        NetSendCmdParam2( 0, CMD_47_BREAKOBJ, playerIndex, objectIndex );
	}
}

//----- (00454118) --------------------------------------------------------
void __fastcall SyncBreakObj( int playerIndex, int objectOnMapIndex )
{
	int baseObjectIndex = Objects[objectOnMapIndex].BaseObjectIndex;
	if( baseObjectIndex == BO_57_POD_URN_BARREL_WITH_DROP || baseObjectIndex == BO_58_POD_URN_BARREL_EXPLOSIVE ){
		InteractWithBarrel(playerIndex, objectOnMapIndex, 0, 1, 0);
	} else if (baseObjectIndex >= BO_20_CRUCIFIED_SKELETON && baseObjectIndex <= BO_22_CRUCIFIED_SKELETON_3) {
		InteractWithCrucifiedSkeleton(playerIndex, objectOnMapIndex);
	}
}

//----- (00454139) --------------------------------------------------------
void __fastcall SetDunTileForObject(int objectIndex)
{
	Object& object = Objects[objectIndex];
	BaseObject& baseObject = BaseObjects[object.BaseObjectIndex];
	int index = 0;
	for( index = 0; ObjectSpriteIndexes[ index ] != baseObject.SpriteIndex && index < countof(ObjectSpriteIndexes); ++index );
	if( index == countof(ObjectSpriteIndexes) ){
		LoadOneObjectSprite(baseObject.SpriteIndex);
	}
	for( index = 0; ObjectSpriteIndexes[ index ] != baseObject.SpriteIndex && index < countof(ObjectSpriteIndexes); ++index );
	if( index == countof(ObjectSpriteIndexes) ){
		TerminateWithError( "Wrong dungeon object index" );
	}
	object.CelFilePtr = ObjectsSprites[index]; 
	switch (object.BaseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:		WithDoors_1				(objectIndex);	break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:													
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:	WithDoors_2				(objectIndex);	break;
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:														
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:		WithDoors_3				(objectIndex);	break;
	case BO_28_SKULL_LEVER:																	
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:											
	case BO_4_LEVER:								WithLever				(objectIndex);	break;
	case BO_20_CRUCIFIED_SKELETON:															
	case BO_21_CRUCIFIED_SKELETON_2:														
	case BO_22_CRUCIFIED_SKELETON_3:				WithCrucifiedSkeleton	(objectIndex);	break;
	case BO_71_BOOK_OF_THE_BLIND:															
	case BO_88_STEEL_TOME:																	
	case BO_41_MYTHICAL_BOOK:						WithQuestBooks			(objectIndex);	break;
	case BO_73_PEDESTAL_OF_BLOOD:					WithPedestalOfBlood		(objectIndex);	break;
	}
}

//----- (004541FB) --------------------------------------------------------
void __fastcall WithDoors_1( int objectIndex )
{
	Object& object = Objects[objectIndex];

	if( object.OpenState == OS_0_CLOSED ){
		object.objectAllowWalking = 0;
		return;
	}
	object.objectAllowWalking = 1;
	int row = object.Row;
	int col = object.Col;
	object.selectable = 2;
	if( Dungeon->graphType <= DT_4_HELL ){
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			int dungeonMapNumber;
			if( object.ShrineIndex == 214 ){
				dungeonMapNumber = 408;
			}else{
				dungeonMapNumber = 393;
			}
			SetDungeonMapNumber(row, col, dungeonMapNumber);
			ArchGraphicsMap[ row ][ col ] = 7;
			ApplyChangeFromDunMapToTileMap(row - 1, col--);
		}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			SetDungeonMapNumber(row, col, 395);
			ArchGraphicsMap[ row ][ col ] = 8;
			ApplyChangeFromDunMapToTileMap(row--, col - 1);
		}
	}else{
		if( object.BaseObjectIndex == BO_1_CHURCH_DOOR_MAIN_DIAGONAL ){
			SetDungeonMapNumber(row, col, 206);
			ArchGraphicsMap[ row ][ col ] = 1;
			ApplyChangeFromDunMapToTileMap(row - 1, col--);
		}else if( object.BaseObjectIndex == BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL ){
			SetDungeonMapNumber(row, col, 209);
			ArchGraphicsMap[ row ][ col ] = 2;
			ApplyChangeFromDunMapToTileMap(row--, col - 1);
		}
	}
	ChangeDungeonMapWithActivateDoor(objectIndex, row, col);
}

//----- (00454301) --------------------------------------------------------
void __fastcall WithCrucifiedSkeleton(int objectIndex)
{
	char notCrucified; // bl@1
	int *nextObjectIndex; // esi@2
	int objectCount; // edi@2
	int curObjectIndex; // eax@3
	int baseObjectIndex; // edx@3

	notCrucified = 1;
	if( ObjectsCount > 0 ){
		nextObjectIndex = ObjectActive;
		objectCount = ObjectsCount;
		do{
			curObjectIndex = *nextObjectIndex;
			baseObjectIndex = Objects[ curObjectIndex ].BaseObjectIndex;
			if( (baseObjectIndex == BO_20_CRUCIFIED_SKELETON
			 || baseObjectIndex == BO_21_CRUCIFIED_SKELETON_2
			 || baseObjectIndex == BO_22_CRUCIFIED_SKELETON_3 )
			 && Objects[ objectIndex ].bookIndex == Objects[ curObjectIndex ].bookIndex
			 && Objects[ curObjectIndex ].destructable != -1 ){
				notCrucified = 0;
			}
			++nextObjectIndex;
			--objectCount;
		} while( objectCount );
	}
	if( notCrucified ){
		ChangeMapRectAfterScriptEvent( Objects[ objectIndex ].TrapedObjectRow, Objects[ objectIndex ].TrapedObjectCol, Objects[ objectIndex ].TrapSpellIndex, Objects[ objectIndex ].OpenState );
	}
}

//----- (0045437E) --------------------------------------------------------
void __fastcall WithLever( int objectIndex )
{
	Object& object = Objects[objectIndex];
	if( !object.selectable ){
		ChangeMapRectAfterScriptEvent(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
	}
}

//----- (004543A8) --------------------------------------------------------
void __fastcall WithQuestBooks(int objectIndex)
{
	Object& object = Objects[objectIndex];

	if( object.FrameIndex == object.maybeFrameIndex ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		if( object.BaseObjectIndex == BO_71_BOOK_OF_THE_BLIND ){
			int wallTransparentIndex = TransValue;
			TransValue = 9;
			SetAreaTransparencyByIndex_40(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
			TransValue = wallTransparentIndex;
		}
	}
}

//----- (00454417) --------------------------------------------------------
void __fastcall WithPedestalOfBlood(int objectIndex)
{
	Object& object = Objects[objectIndex];
	if( Objects[objectIndex].maybeFrameIndex == 1 )
		ChangeMapRectAfterScriptEvent2(CurPattern.row, CurPattern.col + 3, CurPattern.row + 2, CurPattern.col + 7);
	if( object.maybeFrameIndex == 2 ){
		ChangeMapRectAfterScriptEvent2(CurPattern.row, CurPattern.col + 3, CurPattern.row + 2, CurPattern.col + 7);
		ChangeMapRectAfterScriptEvent2(CurPattern.row + 6, CurPattern.col + 3, CurPattern.row + CurPattern.sizeRow, CurPattern.col + 7);
	}
	if( object.maybeFrameIndex == 3 ){
		ChangeMapRectAfterScriptEvent2(object.startRow40, object.startCol40, object.endRow40, object.endCol40);
		uchar* blood = (uchar*)LoadFile(GameMode == GM_CLASSIC ? "Levels\\L2Data\\Blood2_cls.DUN" : "Levels\\L2Data\\Blood2.DUN");
		LoadMapObjects(blood, 2 * CurPattern.row, 2 * CurPattern.col);
		FreeMem(blood);
	}
}

//----- (004544DC) --------------------------------------------------------
void __fastcall WithDoors_2(int objectIndex)
{
	Object& object = Objects[objectIndex];

	if( object.OpenState ){
		object.objectAllowWalking = 1;
	}else{
		object.objectAllowWalking = 0;
	}
	uint row = object.Row;
	uint col = object.Col;
	object.selectable = 2;
	uint newDungeonMapNumber = NULL;
	if( object.BaseObjectIndex == 42 ){
		if( object.OpenState  == OS_0_CLOSED ){
			newDungeonMapNumber = 538;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 13;
		}
	}else if( object.BaseObjectIndex == 43 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 540;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 17;
		}
	}
	if( newDungeonMapNumber ){
		SetDungeonMapNumber(row, col, newDungeonMapNumber);
	}
}

//----- (00454579) --------------------------------------------------------
void __fastcall WithDoors_3(int objectIndex)
{
	Object& object = Objects[objectIndex];
	object.objectAllowWalking = 1;
	uint row = object.Row;
	uint col = object.Col;
	object.selectable = 2;
	uint newDungeonMapNumber = NULL;
	if( object.BaseObjectIndex == 74 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 531;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 538;
		}
	}else if( object.BaseObjectIndex == 75 ){
		if( object.OpenState == OS_0_CLOSED ){
			newDungeonMapNumber = 534;
		}else if( object.OpenState == OS_1_OPEN || object.OpenState == OS_2_BLOCKED ){
			newDungeonMapNumber = 541;
		}
	}
	SetDungeonMapNumber(row, col, newDungeonMapNumber);
}

//----- (0045460B) --------------------------------------------------------
void __fastcall IdentifyObjectOnMapInFocus(int objectIndex)
{
	int baseObjectIndex = Objects[objectIndex].BaseObjectIndex;
	Object& objectOnMap = Objects[objectIndex];
	switch (baseObjectIndex ){
	case BO_1_CHURCH_DOOR_MAIN_DIAGONAL:
	case BO_2_CHURCH_DOOR_COLLATERAL_DIAGONAL:
		switch (objectOnMap.OpenState ){
			case OS_0_CLOSED:		strcpy(InfoPanelHeader, "Closed Door");		break;
			case OS_1_OPEN:			strcpy(InfoPanelHeader, "Open Door");		break;
			case OS_2_BLOCKED:		strcpy(InfoPanelHeader, "Blocked Door");	break;
		}
		break;
	case BO_4_LEVER:				strcpy(InfoPanelHeader, "Lever");			break;
	case BO_20_CRUCIFIED_SKELETON:
	case BO_21_CRUCIFIED_SKELETON_2:
	case BO_22_CRUCIFIED_SKELETON_3:strcpy(InfoPanelHeader, "Crucified Skeleton");break;
	case BO_5_SMALL_CHEST:			strcpy(InfoPanelHeader, "Small Chest");		break;
	case BO_6_CHEST:				strcpy(InfoPanelHeader, "Chest");			break;
	case BO_7_LARGE_CHEST:			strcpy(InfoPanelHeader, "Large Chest");		break;
	case BO_28_SKULL_LEVER:			strcpy(InfoPanelHeader, "Skull Lever");		break;
	case BO_25_ANCIENT_TOME_OR_BOOK_OF_VILENESS:
		if( Dungeon->isQuest ){
			if( Dungeon == DUN_26_BONE_CHAMBER ){
				strcpy(InfoPanelHeader, "Ancient Tome");
			}else if( Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR ){
				strcpy(InfoPanelHeader, "Book of Vileness");
			}
		}
		break;
	case BO_24_ANGEL:				strcpy(InfoPanelHeader, "Statue of an Angel"); break;
	case BO_50_LEVER_2:				strcpy(InfoPanelHeader, "Lever");			break;
	case BO_42_CATACOMB_DOOR_MAIN_DIAGONAL:
	case BO_43_CATACOMB_DOOR_COLLATERAL_DIAGONAL:
	case BO_74_CAVE_DOOR_MAIN_DIAGONAL:
	case BO_75_CAVE_DOOR_COLLATERAL_DIAGONAL:
		switch (objectOnMap.OpenState ){
			case OS_0_CLOSED:		strcpy(InfoPanelHeader, "Closed Door");		break;
			case OS_1_OPEN:			strcpy(InfoPanelHeader, "Open Door");		break;
			case OS_2_BLOCKED:		strcpy(InfoPanelHeader, "Blocked Door");	break;
		}
		break;
	case BO_41_MYTHICAL_BOOK:		strcpy(InfoPanelHeader, "Mythical Book");	break;
	case BO_68_TRAPED_SMALL_CHEST:	strcpy(InfoPanelHeader, "Small Chest");		break;
	case BO_69_TRAPED_CHEST:		strcpy(InfoPanelHeader, "Chest");			break;
	case BO_70_TRAPED_LARGE_CHEST:
	case BO_97_OGDEN_CHEST:			strcpy(InfoPanelHeader, "Large Chest");		break;
	case BO_48_SARCOPHAGUS:			strcpy(InfoPanelHeader, "Sarcophagus");		break;
	case BO_55_BOOKSHELF:			strcpy(InfoPanelHeader, "Bookshelf");		break;
	case BO_62_BOOKCASE:
	case BO_63_BOOKCASE_2:			strcpy(InfoPanelHeader, "Bookcase");		break;
	case BO_57_POD_URN_BARREL_WITH_DROP:
	case BO_58_POD_URN_BARREL_EXPLOSIVE:
		switch( Dungeon->graphType ){
		case DT_6_ABYSS: strcpy(InfoPanelHeader, "Pod"); break;
		case DT_5_CRYPT: strcpy(InfoPanelHeader, "Urn"); break;
		default:         strcpy(InfoPanelHeader, "Barrel"); break;
		}
		break;
	case BO_59_SHRINE:
	case BO_60_SHRINE_2: // имена шрайнов скрываются в игровом режиме
		if( (DevelopMode && IsGodMode) || GameMode == GM_CLASSIC ) sprintf(InfoPanelHeader, "%s Shrine", ShrineNamesPtr[objectOnMap.ShrineIndex]); else
		strcpy(InfoPanelHeader, "Shrine");
		break;
	case BO_61_ANCIENT_TOME:		strcpy(InfoPanelHeader, "Ancient Tome");		break;
	case BO_64_LIBRARY_BOOK:		strcpy(InfoPanelHeader, "Library Book");		break;
	case BO_66_BLOOD_FOUNTAIN:		strcpy(InfoPanelHeader, "Blood Fountain");		break;
	case BO_67_DECAPITATED_BODY:	strcpy(InfoPanelHeader, "Decapitated Body");	break;
	case BO_71_BOOK_OF_THE_BLIND:	strcpy(InfoPanelHeader, "Book of the Blind");	break;
	case BO_88_STEEL_TOME:			strcpy(InfoPanelHeader, "Steel Tome");			break;
	case BO_72_BOOK_OF_BLOOD:		strcpy(InfoPanelHeader, "Book of Blood");		break;
	case BO_76_PURIFYING_SPRING:	strcpy(InfoPanelHeader, "Purifying Spring");	break;
	case BO_77_ARMOR:
	case BO_89_ARMOR_2:				strcpy(InfoPanelHeader, "Armor Rack");				break;
	case BO_79_GOAT_SHRINE:			strcpy(InfoPanelHeader, "Goat Shrine");			break;
	case BO_80_CAULDRON:			strcpy(InfoPanelHeader, "Cauldron");			break;
	case BO_81_MURKY_POOL:			strcpy(InfoPanelHeader, "Murky Pool");			break;
	case BO_82_FOUNTAIN_OF_TEARS:	strcpy(InfoPanelHeader, "Fountain of Tears");	break;
	case BO_73_PEDESTAL_OF_BLOOD:	strcpy(InfoPanelHeader, "Pedestal of Blood");	break;
	case BO_86_QUEST_BOOK:			strcpy(InfoPanelHeader, BookNamePtrTable[objectOnMap.BookNameIndex]);			break;
	case BO_90_WEAPON_RACK:
	case BO_92_WEAPON_RACK_2:		strcpy(InfoPanelHeader, "Weapon Rack");			break;
	case BO_94_MUSHROOM_PATCH:		strcpy(InfoPanelHeader, "Mushroom Patch");		break;
	case BO_95_VILE_STAND:			strcpy(InfoPanelHeader, "Vile Stand");			break;
	case BO_96_SLAIN_HERO:			strcpy(InfoPanelHeader, "Slain Hero");			break;
	case BO_98_HELLFORGE:			strcpy(InfoPanelHeader, "Anvil of Hellforge");	break; // fleshdoom patch (3) did
	default:			break;
	}

	if( (/*Players[CurrentPlayerIndex].Infravision || */PerkValue(PERK_AWARENESS, CurrentPlayerIndex) > 0) && objectOnMap.IsTrapped){
		sprintf(InfoPanelBuffer, "Trapped %s", InfoPanelHeader);
		strcpy(InfoPanelHeader, InfoPanelBuffer);
		CurFontColor = 2;
	}
	
	//sprintf(InfoPanelHeader, "Object field_38 %d", objectOnMap.isBlockWalking);
	//sprintf(InfoPanelHeader, "Object Type %d", baseObjectIndex);
	//InfoPanel_AddLine(InfoPanelHeader);
}


