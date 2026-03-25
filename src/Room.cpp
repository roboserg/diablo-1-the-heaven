#include "stdafx.h"

int RoomsCount;// 006FF000
int IsNotArmorPlaced;// 006FF004
int IsWeapondStandNotPlaced;// 006FF018
int IsRoomWithGoldNotPlaced;// 006FF01C
int IsMurkyPoolNotPlaced;// 006FF020
int IsCauldronNotPlaced;// 006FF024
int IsFountainOfTearsNotPlaced;// 006FF028
int ZharTheMadRoomIndex;// 006FF02C
int RoomCursorRow;// 006FF030
int RoomCursorCol;// 006FF034
int RoomCursor_MonsterSprite_Or_Direction;// 006FF038
Room Rooms[RoomsMax];// 006FF040
int IsPuriFyingSpringNotPlaced;// 006FF1D0
int IsBloodFountanIsNotPlaced;// 006FF1D4
int IsBurnCrossAdded;// 006FF1D8

ROOM_TYPE DefaultRoomTypesTable[4] ={ // 004CACA8
	RT_11_GOAT_SHRINE, 
	RT_1_SHRINE, 
	RT_3_ANCIENT_TOMES, 
	RT_5_LIBRYARY 
};
int Offsets5x5_Rows[5][5] ={ // 004CACB8
	{-2, -1, 0, 1, 2,}, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
	{-2, -1, 0, 1, 2, }, 
};
int Offsets5x5_Cols[5][5] ={ // 004CAD20
	{-2, -2, -2, -2, -2, }, 
	{-1, -1, -1, -1, -1, }, 
	{0, 0, 0, 0, 0, }, 
	{1, 1, 1, 1, 1, }, 
	{2, 2, 2, 2, 2, }, 
};
int Offsets3x3_Rows[3][3] ={ // 004CAD88
	{-1, 0, 1, }, 
	{-1, 0, 1, }, 
	{-1, 0, 1, }, 
};
int Offsets3x3_Cols[3][3] ={ // 004CADB0
	{-1, -1, -1, }, 
	{0, 0, 0, }, 
	{1, 1, 1, },  
};

//----- (00471B33) --------------------------------------------------------
bool __fastcall SetRoomCursoreToCenterOfTopWall(int roomIndex)
{
	int hallIndex = Rooms[roomIndex].transIndex; 
	int direction = 0;
	for( int col = 0; col < 112; ++col ){
		for( int row = 0; row < 112; ++row ){
			if( TransMap[ row ][ col ] == hallIndex ){
				if( IsWall[FineMap[ row ][ col - 1 ]]
					&& !TileBlockWalking[FineMap[ row - 1 ][ col ]] 
					&& !TileBlockWalking[FineMap[ row + 1 ][ col ]] 
					&& TransMap[ row - 1 ][ col ] == hallIndex 
					&& TransMap[ row + 1 ][ col ] == hallIndex 
					&& !ObjectsMap[ row - 1 ][ col - 1 ] 
					&& !ObjectsMap[ row + 1 ][ col - 1 ] ){
						direction = 1;
				}else if( IsWall[FineMap[ row - 1 ][ col ]] 
					&& !TileBlockWalking[FineMap[ row ][ col - 1 ]] 
					&& !TileBlockWalking[FineMap[ row ][ col + 1 ]] 
					&& TransMap[ row ][ col - 1 ] == hallIndex 
					&& TransMap[ row ][ col + 1 ] == hallIndex 
					&& !ObjectsMap[ row - 1 ][ col - 1 ] 
					&& !ObjectsMap[ row - 1 ][ col + 1 ] ){
						direction = 2;
				}
				if( direction ){
					RoomCursor_MonsterSprite_Or_Direction = direction;
					RoomCursorRow = row;
					RoomCursorCol = col;
					return true;
				}
			}
		}
	}
	return false;
}

//----- (00471C57) --------------------------------------------------------
bool __fastcall SetRoomCursoreToCenterOfRoom(int roomIndex)
{
	int randChangeFactor = RNG(5) + 1;
	int startRandChangeFactor = randChangeFactor;
	int hallIndex = Rooms[roomIndex].transIndex;
	int row = 0;
	int col = 0;
	// глупый алгоритм. находит n-ое вхождение клетки комнаты удаленной от стен идругих объектов не менее чем на 2 клетки
	// если есть менее чем n вхождений, то проходится ещё раз по имеющимся пока 
	// если нет ни одного вхождения, то выйдем уже после первого прохода по клеткам
	while( true ){
		bool checkResult = false;

		if( TransMap[ row ][ col ] == hallIndex 
	     && !TileBlockWalking[FineMap[ row ][ col ]] ){
				checkResult = true;

				for( int i = 0; i < 5 && checkResult; ++i ){
					for( int j = 0; j < 5 && checkResult; ++j ){
						int checkedRow = row + Offsets5x5_Rows[i][j];
						int checkedCol = col + Offsets5x5_Cols[i][j];
						if( TileBlockWalking[FineMap[ checkedRow ][ checkedCol ]] ){
							checkResult = false;
							break;
						}
						if( TransMap[ checkedRow ][ checkedCol ] != hallIndex ){
							checkResult = false;
							break;
						}
					}
				}

				
		}

		if( checkResult ){
			// возможно оригинальный баг. Финальной будет только первая найденная клетка
			// поскольку найдя её мы ещё randChangeFactor раз проверим её же вместо перехода к следующей
			--randChangeFactor;
			if( randChangeFactor <= 0 ){
				RoomCursorRow = row;
				RoomCursorCol = col;
				return true;
			}
		}else{
			// перебираем всю карту, если дошли до конца, начинаем сначала
			if( ++row == 112 ){
				row = 0;
				if( ++col == 112 ){
					// если прошли всю карту и так ни разу не встретили подходящей клетки
					if( randChangeFactor == startRandChangeFactor ){
						return false;
					}
					col = 0;
				}
			}
		}
	}
}

//----- (00471D35) --------------------------------------------------------
bool __fastcall SelectSceletonAndCenterOfRoom(int roomIndex)
{
	if( Dungeon->genType == DT_1_CHURCH || Dungeon->genType == DT_2_CATACOMB && MonsterSpritesCount > 0 ){
		for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
			if( IsMonsterSummonable(MonsterSprites[monsterSpriteIndex].baseMonsterIndex
			    /*&& MonsterSprites[monsterSpriteIndex].flags & 1*/ ) ){  //  fixing 'no goatshrine rooms' bug
                    RoomCursor_MonsterSprite_Or_Direction = monsterSpriteIndex;
                    return SetRoomCursoreToCenterOfRoom(roomIndex);
			}
		}
	}
	return false;
}

//----- (00471D85) --------------------------------------------------------
bool __fastcall SelectClanAndCenterOfRoom(int roomIndex)
{
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( IsMonsterClan(MonsterSprites[monsterSpriteIndex].baseMonsterIndex
		    /*&& MonsterSprites[monsterSpriteIndex].flags & 1*/ ) ){ //  fixing 'no goatshrine rooms' bug
                RoomCursor_MonsterSprite_Or_Direction = monsterSpriteIndex;
                return SetRoomCursoreToCenterOfRoom(roomIndex);
		}
	}
	return false;
}

//----- (00471DC8) --------------------------------------------------------
bool __fastcall IsFloorAroundCellIsFree( int row, int col, int roomIndex, int successChance )
{
	for( int i = 0; i < 3; ++i ){
		for( int j = 0; j < 3; ++j ){
			int checkedRow = row + Offsets3x3_Rows[i][j];
			int checkedCol = col + Offsets3x3_Cols[i][j];
			if( ! In112(checkedRow, checkedCol) // was potential bug // checkedRow < 0 || checkedCol < 0
			 || TileBlockWalking[FineMap[ checkedRow ][ checkedCol ]]
			 || TransMap[ checkedRow ][ checkedCol ] != Rooms[roomIndex].transIndex
			 || ObjectsMap[ checkedRow ][ checkedCol ]
				// похоже на косяк. Вместо однократной проверки на допустимость, проверка будет произведена до 9 раз
				// соответственно шансы на успех становятся мизерными
			 || successChance != -1 && !RNG(successChance) ){
				return false;
			}
		}
	}
	return true;
}

//----- (00471E47) --------------------------------------------------------
bool __fastcall SetRoomCursorToCellAtDistanceFromWalls( int roomIndex )
{
	char successChanceTable[4] = { 4, 4, 3, 5 };
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( IsFloorAroundCellIsFree(row, col, roomIndex, successChanceTable[Dungeon->genType - 1]) ){
				RoomCursorRow = row;
				RoomCursorCol = col;
				return true;
			}
		}
	}
	return false;
}

//----- (00471EA3) --------------------------------------------------------
bool __fastcall IsRoomTypeSuitableForDLVL( int roomType )
{
	switch( roomType ){
	case RT_0_BARRELS           :
	case RT_2_MAGIC_ITEM        :
	case RT_4_GOLD              :
	case RT_6_TORTURE           :
	case RT_8_DECAPITATED_BODIES:
	case RT_11_GOAT_SHRINE      :
	case RT_15_BURN_CROSSES     : return true;
	case RT_1_SHRINE            :
	case RT_5_LIBRYARY          : return Dungeon->genType != DT_3_CAVE && Dungeon->genType != DT_4_HELL;
	case RT_3_ANCIENT_TOMES     : return Dungeon >= DUN_1_CHURCH_1 && Dungeon <= DUN_4_CHURCH_4;
	case RT_7_BLOOD_FOUNTAN     : return IsBloodFountanIsNotPlaced != 0;
	case RT_9_PURIFYING_SPRING  : return IsPuriFyingSpringNotPlaced != 0;
	case RT_10_ARMOR_STAND      :
	case RT_16_WEAPON_STAND     : return Dungeon->genType != DT_1_CHURCH;
	case RT_12_CAULDRON         : return Dungeon->genType == DT_4_HELL && IsCauldronNotPlaced;
	case RT_13_MURKY_POOL       : return IsMurkyPoolNotPlaced != 0;
	case RT_14_FOUNTAIN_OF_TEARS: return IsFountainOfTearsNotPlaced != 0; 
	default                     : return false;
	}
	return false;
}

//----- (00471F23) --------------------------------------------------------
bool  __fastcall IsRoomTypeSuitable( int roomIndex, int roomType )
{
	if( ! IsRoomTypeSuitableForDLVL(roomType) ) return false;

	switch( roomType ){
	case RT_1_SHRINE:
	case RT_5_LIBRYARY:		return SetRoomCursoreToCenterOfTopWall(roomIndex);
	case RT_3_ANCIENT_TOMES:		return SelectSceletonAndCenterOfRoom(roomIndex);
	case RT_7_BLOOD_FOUNTAN:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsBloodFountanIsNotPlaced = false;
			return true;
		}
		return false;
	case RT_9_PURIFYING_SPRING:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsPuriFyingSpringNotPlaced = false;
			return true;
		}
		return false;
	case RT_13_MURKY_POOL:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsMurkyPoolNotPlaced = false;
			return true;
		}
		return false;
	case RT_14_FOUNTAIN_OF_TEARS:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsFountainOfTearsNotPlaced = false;
			return true;
		}
		return false;
	case RT_12_CAULDRON:
		if( SetRoomCursoreToCenterOfRoom(roomIndex) ){
			IsCauldronNotPlaced = false;
			return true;
		}
		return false;
	case RT_11_GOAT_SHRINE:			return SelectClanAndCenterOfRoom(roomIndex);
	case RT_6_TORTURE:
	case RT_8_DECAPITATED_BODIES:
	case RT_10_ARMOR_STAND:
	case RT_15_BURN_CROSSES:
	case RT_16_WEAPON_STAND:		return SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
	case RT_4_GOLD:
		if( IsRoomWithGoldNotPlaced ){
			IsRoomWithGoldNotPlaced = false;
			return true;
		}
		return false;
	default:
		return true;
	}
	return true;
}

//----- (0047205F) --------------------------------------------------------
bool __fastcall HallIsRoom(int hallIndex)
{
	// холл в котором находится переход не может быть комнатой
	for( auto& warp: Warps ){
		if( TransMap[ warp.Row ][ warp.Col ] == hallIndex){
			return false;
		}
	}
	// измеряем размер холла
	int roomCellsCount = 0;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex ){
				// если клетка уже помечена как относящаяся к комнате,
				// переходу или квестовому шаблону, то это не комната (или уже отмеченная комната)
				if( FlagMap[ row ][ col ] & CF_8_FORBIDDEN ){
					return false;
				}
				roomCellsCount++;
			}
		}
	}
	// лимит размера комнаты для церкви
	if( Dungeon->genType == DT_1_CHURCH 
	 && !(roomCellsCount >= 9 && roomCellsCount <= 100) ){
			return false;
	}
	// ни один из проходимых тилов этого холла не должен соседствовать с проходимыми тилами другого холла
	// т.е. пол комнаты должен быть окружен стенами
	for( int col = 0; col < 112; col ++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[ FineMap[ row ][ col ]] ){
				// как оно интересно работает с краевыми элементами? bug!!
				if( TransMap[row - 1][col    ] != hallIndex && !TileBlockWalking[ FineMap[row - 1][col    ] ]
				 || TransMap[row + 1][col    ] != hallIndex && !TileBlockWalking[ FineMap[row + 1][col    ] ]
				 || TransMap[row    ][col - 1] != hallIndex && !TileBlockWalking[ FineMap[row    ][col - 1] ]
				 || TransMap[row    ][col + 1] != hallIndex && !TileBlockWalking[ FineMap[row    ][col + 1] ] ){
					return false;
				}
			}
		}
	}
	return true;
}

//----- (00472193) --------------------------------------------------------
void InitRooms() // InitThemes
{
	ZharTheMadRoomIndex = -1;
	RoomsCount = 0;
	IsNotArmorPlaced = true;
	IsBloodFountanIsNotPlaced = true;
	IsCauldronNotPlaced = true;
	IsMurkyPoolNotPlaced = true;
	IsPuriFyingSpringNotPlaced = true;
	IsFountainOfTearsNotPlaced = true;
	IsRoomWithGoldNotPlaced = true;
	IsBurnCrossAdded = false;
	IsWeapondStandNotPlaced = true;

	for(auto& r: Rooms){ r.roomType = RT_NONE; r.transIndex = 0; }

	if( Dungeon == DUN_16_HELL_4 ) return;

	switch(Dungeon->genType){
	case DT_1_CHURCH:
		for( int transIndex = 0; transIndex < RoomsMax; ++transIndex ){
			if( HallIsRoom(transIndex) ){ // 12 hallIndex должен дать true
				Rooms[RoomsCount].transIndex = transIndex;
				ROOM_TYPE roomType = DefaultRoomTypesTable[RNG(countof(DefaultRoomTypesTable))];
				while( ! IsRoomTypeSuitable(RoomsCount, roomType) ){
					roomType = (ROOM_TYPE) RNG(RT_count);
				}
				Rooms[RoomsCount++].roomType = roomType;
			}
		}
		break;
	case DT_2_CATACOMB or DT_3_CAVE or DT_4_HELL:
		if( IsQuestOnLevel(Q_3_ZHAR_THE_MAD) ){
			for( int roomIndex = 0; roomIndex < ChambersCount; roomIndex++ ){
				Room& room = Rooms[roomIndex];
				Chamber& chamber = Chambers[roomIndex];
				room.transIndex = chamber.transIndex;
				if( IsRoomTypeSuitable(roomIndex, RT_5_LIBRYARY) ){
					room.roomType = RT_5_LIBRYARY;
					ZharTheMadRoomIndex = roomIndex;
					break;
				}
			}
		}
		for( int roomIndex = 0; roomIndex < ChambersCount; roomIndex++ ){
			Room& room = Rooms[roomIndex];
			Chamber& chamber = Chambers[roomIndex];
			if( room.roomType == -1 ){
				room.transIndex = chamber.transIndex;
				ROOM_TYPE roomType = DefaultRoomTypesTable[RNG(4)];
				while( ! IsRoomTypeSuitable(roomIndex, roomType) ){
					roomType = (ROOM_TYPE) RNG(RT_count);
				}
				room.roomType = roomType;
			}
		}
		RoomsCount = ChambersCount; // was original RoomsCount = ChambersCount, but have no sense
		break;
	}
}

//----- (00472346) --------------------------------------------------------
void HoldThemeRooms()
{
	if( Dungeon == DUN_16_HELL_4 ){
		return;
	}
	if( Dungeon->genType != DT_1_CHURCH ){
		SetForbiddenObjectFlagForAloneRooms2();
	}else{ // DT_1_CHURCH
		for( int roomIndex = 0; roomIndex < RoomsCount; roomIndex++ ){
			int hallIndex = Rooms[roomIndex].transIndex;
			for( int col = 0; col < 112; col++ ){
				for( int row = 0; row < 112; row++ ){
					if( TransMap[ row ][ col ] == hallIndex ){
						FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
					}
				}
			}
		}
	}
}

//----- (004723A0) --------------------------------------------------------
void __fastcall FillRoomWithMonsters(int roomIndex, int successChance)
{
	uint validMonstersTable[222]; // was 138
	int validMonstersCount = 0;
	for( int monsterSpriteIndex = 0; monsterSpriteIndex < MonsterSpritesCount; monsterSpriteIndex++ ){
		if( MonsterSprites[monsterSpriteIndex].placeFlag & MPF_1_SCATTER ){
			validMonstersTable[validMonstersCount++] = monsterSpriteIndex;
		}
	}
	if( ! validMonstersCount ) return;
	int spriteIndex = validMonstersTable[RNG(validMonstersCount)];
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex
				&& !TileBlockWalking[FineMap[ row ][ col ]] 
				&& !ItemMap[ row ][ col ]
				&& !ObjectsMap[ row ][ col ] ){
					if( !RNG(successChance) ){
						AddMonster(row, col, RNG(8), spriteIndex, 1);
					}
			}
		}
	}
}

//----- (00472485) --------------------------------------------------------
void __fastcall MakeAloneRoomType0Barrels( int roomIndex )
{
	char successChanceTable1[4] = { 2, 6, 4, 8 };
	Room& room = Rooms[roomIndex];
	int hallIndex = room.transIndex;

	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex
				&& !TileBlockWalking[ FineMap[ row ][ col ] ] ){
					if( !RNG( successChanceTable1[Dungeon->genType - 1]) ){
						if( !RNG( successChanceTable1[Dungeon->genType - 1]) ){
							AddObject(BO_57_POD_URN_BARREL_WITH_DROP, row, col);
						}else{
							AddObject(BO_58_POD_URN_BARREL_EXPLOSIVE, row, col);
						}
					}
			}
		}
	}
	char monsterPlaceChanceTable[4] = { 5, 7, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (0047255D) --------------------------------------------------------
void __fastcall MakeAloneRoomType1Shrine( int roomIndex )
{
	SetRoomCursoreToCenterOfTopWall(roomIndex);
	if( RoomCursor_MonsterSprite_Or_Direction == 1 ){
		AddObject(BO_9_CANDLE2, RoomCursorRow - 1, RoomCursorCol);
		AddObject(BO_60_SHRINE_2, RoomCursorRow, RoomCursorCol);
		AddObject(BO_9_CANDLE2, RoomCursorRow + 1, RoomCursorCol);
	}else{
		AddObject(BO_9_CANDLE2, RoomCursorRow, RoomCursorCol - 1);
		AddObject(BO_59_SHRINE, RoomCursorRow, RoomCursorCol);
		AddObject(BO_9_CANDLE2, RoomCursorRow, RoomCursorCol + 1);
	}
	int monsterPlaceChanceTable[4] = { 6, 6, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472610) --------------------------------------------------------
void __fastcall MakeAloneRoomType2MagicItem( int roomIndex )
{
	int randChangeFactor = RNG(100) + 1;
	Room& room = Rooms[roomIndex];
	int hallIndex = room.transIndex;
	int row = 0;
	int col = 0;

	while( randChangeFactor > 0 ){
		if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
			--randChangeFactor;
		}
		if( randChangeFactor <= 0 ){
			break;
		}
		if( ++row == 112 ){
			row = 0;
			if( ++col == 112 ){
				col = 0;
			}
		}
	}

	CreateRndItem(row, col, 1, 0, 1);
	SomeActionWithDropItemsInAloneRooms();
	char monsterPlaceChanceTable[4] = { 6, 7, 3, 9 };
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (004726AC) --------------------------------------------------------
void __fastcall MakeAloneRoomType3AncientTomes( int roomIndex )
{
	char monsterPlaceChanceTable[4] = { 6, 7, 3, 9 };
	SelectSceletonAndCenterOfRoom(roomIndex);
	int col = RoomCursorCol;
	int row = RoomCursorRow;
	AddObject(BO_3_TORCH, RoomCursorRow, RoomCursorCol);
	
	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col - 1);
	}else{
		AddObject(BO_11_BANNER, row - 1, col - 1);
	}

	ActivateAmbushMonster(AddAmbushMonster(), row, col - 1);

	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col - 1);
	}else{
		AddObject(BO_13_BANNER, row + 1, col - 1);
	}

	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col);
	}else{
		AddObject(BO_12_BANNER, row - 1, col);
	}

	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col);
	}else{
		AddObject(BO_12_BANNER, row + 1, col);
	}

	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row - 1, col + 1);
	}else{
		AddObject(BO_13_BANNER, row - 1, col + 1);
	}

	ActivateAmbushMonster(AddAmbushMonster(), row, col + 1);

	if( RNG(monsterPlaceChanceTable[Dungeon->genType - 1]) ){
		ActivateAmbushMonster(AddAmbushMonster(), row + 1, col + 1);
	}else{
		AddObject(BO_11_BANNER, row + 1, col + 1);
	}

	if( !ObjectsMap[ row ][ col - 3 ] ){ // проверяется клетка рядом, чтобы стойка с книгой не перекрывала дверь
		AddObject(BO_61_ANCIENT_TOME, row, col - 2);
	}
	if( !ObjectsMap[ row ][ col + 3 ] ){ // проверяется клетка рядом, чтобы стойка с книгой не перекрывала дверь
		AddObject(BO_61_ANCIENT_TOME, row, col + 2);
	}

}

//----- (00472882) --------------------------------------------------------
void __fastcall MakeAloneRoomType4Gold( int roomIndex )
{
	char successChanceTable1[4] = {4, 9, 7, 10}; // берем некий шанс из таблички по актам. И генерим случайное число от 0 до него
	GetRndSeed();
	
    // Prevent random generator from damage
    // With segregated loot, random generator desyncronized after dropping of item
    auto randUsingCounter = RandUsingCounter;
    auto randomSeed = RandomSeed;
    auto lastSeed = LastSeed;
	
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				int randFactor = RNG(successChanceTable1[Dungeon->genType - 1]);
				if( !(2 * RNG(successChanceTable1[ Dungeon->genType - 1 ])) ){// скорее всего ошибка и должно было быть умножение successChanceTable1[DungeonType] на 2
					CreateTypeItem(row, col, 0, IC_11_GOLD, 0, 0, 1);
					SomeActionWithDropItemsInAloneRooms();
				}
				if( !randFactor ){
					CreateRndItem(row, col, 0, 0, 1);
					SomeActionWithDropItemsInAloneRooms();
				}

				int i = successChanceTable1[Dungeon->genType - 1] - 2; // берем некий шанс из таблички. При этом уменьшаем на 2
				if( !randFactor	|| randFactor >= i ){ // С шансом 1/N или если rand(N) >= N-2 (еще сколько-то процентов)
					int itemIndex = SomeActionWithDropItemsInAloneRooms(); // Обращаемся к только что брошенной кучке золота или предмету
					if( randFactor >= i && Dungeon->genType != DT_1_CHURCH ){ // если rand(N) >= N-2 и это НЕ склеп или собор
						Items[itemIndex].amount >>= 1; // Делим QualityLevel пополам (в случае золота, это его количество)
						LimitToMin(Items[itemIndex].amount, 1); // минималочка = 1 (чтоб не было кучек золота в 0 монет, ибо тупость)
					}
				}
			}
		}
	}
	
	RandUsingCounter = randUsingCounter;
    RandomSeed = randomSeed;
    LastSeed = lastSeed;
	
	char monsterPlaceChanceTable[4] = {6, 8, 3, 7};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (g) --------------------------------------------------------
void __fastcall MakeAloneRoomType5Libryary( int roomIndex )
{
	SetRoomCursoreToCenterOfTopWall(roomIndex);

	if( RoomCursor_MonsterSprite_Or_Direction == 1 ){
		AddObject(BO_65_CANDLE2, RoomCursorRow - 1, RoomCursorCol);
		AddObject(BO_63_BOOKCASE_2, RoomCursorRow, RoomCursorCol);
		AddObject(BO_65_CANDLE2, RoomCursorRow + 1, RoomCursorCol);
	}else{
		AddObject(BO_65_CANDLE2, RoomCursorRow, RoomCursorCol - 1);
		AddObject(BO_62_BOOKCASE, RoomCursorRow, RoomCursorCol);
		AddObject(BO_65_CANDLE2, RoomCursorRow, RoomCursorCol + 1);
	}

	char successChanceTable1[4] = {1, 2, 2, 5};
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) && !MonsterMap[ row ][ col ] ){
				if( !RNG(successChanceTable1[Dungeon->genType - 1]) ){
					AddObject(BO_64_LIBRARY_BOOK, row, col);
					if( RNG(2 * successChanceTable1[Dungeon->genType - 1]) ){
						int objectIndex = ObjectsMap[ row ][ col ] - 1;
						// был адский мега баг, в th1 портиться ObjectsIndexInverse
						if( objectIndex >= 0 ){
							Objects[objectIndex].selectable = 0;
							Objects[objectIndex].FrameIndex += 2;
						}
					}
				}
			}
		}
	}

	if( !IsQuestOnLevel(Q_3_ZHAR_THE_MAD) || roomIndex != ZharTheMadRoomIndex ){
		char monsterPlaceChanceTable[4] = {5, 7, 3, 9};
		FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]); // was [DungeonType] - original bug, out of range in HELL type labirint
	}
}

//----- (00472B68) --------------------------------------------------------
void __fastcall MakeAloneRoomType6Tnudems( int roomIndex )
{
	char successChanceTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RNG(successChanceTable1[ Dungeon->genType - 1 ]) ){
						AddObject(BO_30_TNUDEM, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters( roomIndex, monsterPlaceChanceTable[ Dungeon->genType - 1 ] );
}

//----- (00472C3B) --------------------------------------------------------
void __fastcall MakeAloneRoomType7BloodFountan( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObject(BO_66_BLOOD_FOUNTAIN, RoomCursorRow, RoomCursorCol);
	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472C81) --------------------------------------------------------
void __fastcall MakeAloneRoomType8DecapitatedBodies( int roomIndex )
{
	char successChanceTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 1; col < 111; col++ ){
		for( int row = 1; row < 111; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RNG(successChanceTable1[Dungeon->genType - 1]) ){
						AddObject(BO_67_DECAPITATED_BODY, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472D54) --------------------------------------------------------
void __fastcall MakeAloneRoomType9PurifyingSpring( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObject(BO_76_PURIFYING_SPRING, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[5] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472D9A) --------------------------------------------------------
void __fastcall MakeAloneRoomType10ArmorStand( int roomIndex )
{
	if( IsNotArmorPlaced ){
		SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
		AddObject(BO_77_ARMOR, RoomCursorRow, RoomCursorCol);
	}

	char successChanceTable1[4] = {6, 8, 3, 8};
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RNG(successChanceTable1[Dungeon->genType - 1]) ){
						AddObject(BO_78_ARMSTAND, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
	IsNotArmorPlaced = false;
}

//----- (00472E8B) --------------------------------------------------------
void __fastcall MakeAloneRoomType11GoatShrine( int roomIndex )
{
	SelectClanAndCenterOfRoom(roomIndex);
	AddObject(BO_79_GOAT_SHRINE, RoomCursorRow, RoomCursorCol);

	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = RoomCursorCol - 1; col <= RoomCursorCol + 1; col++ ){
		for( int row = RoomCursorRow - 1; row <= RoomCursorRow + 1; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( row != RoomCursorRow || col != RoomCursorCol ){
					AddMonster(row, col, 1, RoomCursor_MonsterSprite_Or_Direction, 1);
				}
			}
		}
	}
}

//----- (00472F59) --------------------------------------------------------
void __fastcall MakeAloneRoomType12Cauldron( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObject(BO_80_CAULDRON, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472F9F) --------------------------------------------------------
void __fastcall MakeAloneRoomType13MurkyPool( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObject(BO_81_MURKY_POOL, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (00472FE5) --------------------------------------------------------
void __fastcall MakeAloneRoomType14FountainOfTears( int roomIndex )
{
	SetRoomCursoreToCenterOfRoom(roomIndex);
	AddObject(BO_82_FOUNTAIN_OF_TEARS, RoomCursorRow, RoomCursorCol);

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
}

//----- (0047302B) --------------------------------------------------------
void __fastcall MakeAloneRoomType15BurnCrosses( int roomIndex )
{
	char successChanceTable1[4] = {5, 7, 3, 8};
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RNG(successChanceTable1[Dungeon->genType - 1]) ){
						AddObject(BO_91_BURNCROS, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 8, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
	IsBurnCrossAdded = true;
}

//----- (004730FE) --------------------------------------------------------
void __fastcall MakeAloneRoomType16WeaponStand( int roomIndex )
{
	if( IsWeapondStandNotPlaced ){
		SetRoomCursorToCellAtDistanceFromWalls(roomIndex);
		AddObject(BO_92_WEAPON_RACK_2, RoomCursorRow, RoomCursorCol);
	}

	char successChanceTable1[4] = {6, 8, 5, 8};
	
	int hallIndex = Rooms[roomIndex].transIndex;
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] == hallIndex && !TileBlockWalking[FineMap[ row ][ col ]] ){
				if( IsFloorAroundCellIsFree(row, col, roomIndex, -1) ){
					if( !RNG(successChanceTable1[Dungeon->genType - 1]) ){
						AddObject(BO_93_WEAPSTND, row, col);
					}
				}
			}
		}
	}

	char monsterPlaceChanceTable[4] = {6, 7, 3, 9};
	FillRoomWithMonsters(roomIndex, monsterPlaceChanceTable[Dungeon->genType - 1]);
	IsWeapondStandNotPlaced = false;
}

//----- (004731EF) --------------------------------------------------------
void UniteAllHalls()
{
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			if( TransMap[ row ][ col ] ){
				TransMap[ row ][ col ] = 1;
			}
		}
	}
}

//----- (0047320F) --------------------------------------------------------
void FillAloneRooms()
{
	if( Dungeon == DUN_16_HELL_4 ){
		return;
	}
	InitObjFlag = true;
	for( int roomIndex = 0; roomIndex < RoomsCount; roomIndex++ ){
		int roomType = Rooms[roomIndex].roomType;
		RoomCursorRow = 0;
		RoomCursorCol = 0;
		switch( roomType ){
			case RT_0_BARRELS:				MakeAloneRoomType0Barrels(roomIndex);					break;
			case RT_1_SHRINE:				MakeAloneRoomType1Shrine(roomIndex);					break;
			case RT_2_MAGIC_ITEM:			MakeAloneRoomType2MagicItem(roomIndex);					break;
			case RT_3_ANCIENT_TOMES:		MakeAloneRoomType3AncientTomes(roomIndex);				break;
			case RT_4_GOLD:					MakeAloneRoomType4Gold(roomIndex);						break;
			case RT_5_LIBRYARY:				MakeAloneRoomType5Libryary(roomIndex);					break;
			case RT_6_TORTURE:				MakeAloneRoomType6Tnudems(roomIndex);					break;
			case RT_7_BLOOD_FOUNTAN:		MakeAloneRoomType7BloodFountan(roomIndex);				break;
			case RT_8_DECAPITATED_BODIES:	MakeAloneRoomType8DecapitatedBodies(roomIndex);			break;
			case RT_9_PURIFYING_SPRING:		MakeAloneRoomType9PurifyingSpring(roomIndex);			break;
			case RT_10_ARMOR_STAND:			MakeAloneRoomType10ArmorStand(roomIndex);				break;
			case RT_11_GOAT_SHRINE:			MakeAloneRoomType11GoatShrine(roomIndex);				break;
			case RT_12_CAULDRON:			MakeAloneRoomType12Cauldron(roomIndex);					break;
			case RT_13_MURKY_POOL:			MakeAloneRoomType13MurkyPool(roomIndex);				break;
			case RT_14_FOUNTAIN_OF_TEARS:	MakeAloneRoomType14FountainOfTears(roomIndex);			break;
			case RT_15_BURN_CROSSES:		MakeAloneRoomType15BurnCrosses(roomIndex);				break;
			case RT_16_WEAPON_STAND:		MakeAloneRoomType16WeaponStand(roomIndex);				break;
		default:																					break;
		}
	}
	InitObjFlag = false;
	if( Dungeon->genType == DT_4_HELL && ChambersCount > 0 ){// в аду большие двери и все комнаты считаются одной
		UniteAllHalls();
	}
}
