#include "stdafx.h"

//----- (th3) -------------------------------------------------------------
char* GetSolName()
{
	char* SOLFileName = 0;
	if( Dungeon->genType == DT_0_TOWN ){
		SOLFileName = "NLevels\\TownData\\Town.SOL";
	}else if( Dungeon->genType == DT_1_CHURCH ){
		if( Dungeon ->graphType >= DT_5_CRYPT ){
			SOLFileName = "NLevels\\L5Data\\L5.SOL";
		}else{
			SOLFileName = "Levels\\L1Data\\L1.SOL";
		}
	}else if( Dungeon->genType == DT_2_CATACOMB ){
		SOLFileName = "Levels\\L2Data\\L2.SOL";
	}else if( Dungeon->genType == DT_3_CAVE ){
		if( Dungeon->graphType >= DT_5_CRYPT ){
			SOLFileName = "NLevels\\L6Data\\L6.SOL";
		}else{
			SOLFileName = "Levels\\L3Data\\L3.SOL";
		}
	}else if( Dungeon->genType == DT_4_HELL ){
		SOLFileName = "Levels\\L4Data\\L4.SOL";
	}else{
		TerminateWithError("FillSolidBlockTbls");
	}
	return SOLFileName;
}

//----- (th3) -------------------------------------------------------------
void ClearBlockCell()
{
	memzero(TileBlockSight);
	memzero(TileBlockWalking);
	memzero(CellTransparent);
	memzero(TileBlockMissle);
	memzero(IsWall);
}

//----- (0041B72D) --------------------------------------------------------
void FillSolidBlockTables()
{
	ClearBlockCell();
	char* SOLFileName = GetSolName();
	int size;
	char* sol = (char*)LoadFile(SOLFileName, &size); // SOL: Settings Of Level ?
	for( int i = 1; i <= size; i++ ){
		uchar solFlag = sol[i - 1];
		if( solFlag & BIT(1) ){
			TileBlockWalking[i] = WT_ALL; // Blocks Walking all
			if( Dungeon->genType == DT_3_CAVE ){
				if( Dungeon->graphType >= DT_5_CRYPT ){
					if( is(i, 534, 535, 536, 537, 506, 504, 503, 500, 498) || i >= 386 && i <= 496){ // abyss
						TileBlockWalking[i] = WT_1_WALK; // Blocks Walking non fly
					}
				}else{
					if( i >= 56 && i <= 146 || is(i, 150, 152) || i >= 154 && i <= 160 ){ // cave
						TileBlockWalking[i] = WT_1_WALK; // Blocks Walking non fly
					}
				}
			}
			#if UNIVERSAL_FLYABLE_FLAG_DETECTION
			if( solFlag & (BIT(2)|BIT(3)|BIT(8)) ){
				TileBlockWalking[i] = WT_ALL; // Blocks Walking all
			}else{
				TileBlockWalking[i] = WT_1_WALK; // Blocks Walking non fly
			}
			#endif
		}
		if( solFlag & BIT(2) ) TileBlockSight[i] = 1; // Blocks Sight
		if( solFlag & BIT(3) ) TileBlockMissle[i] = 1; // Blocks Missiles
		if( solFlag & BIT(4) ) CellTransparent[i] = 1; // Transparency
		WallsTransparency[i] = (solFlag >> 4) & 7;
		if( solFlag & BIT(8) ) IsWall[i] = 1; // это стена
	}
	FreeMem( sol );
}

int MaxDarkenTilePixelIndex = 0;
// используется в модуле карт
//----- (0041B869) --------------------------------------------------------
void InitDarkenDungeonTiles()
{
	//return; // временно отключена до сравнения результатов в оригиналом, возможно ошибки в других функциях отрисовки
	memset(TileLightMapTop, 0, sizeof(TileLightMapTop));
	memset(TileLightMapCounter, 0, sizeof(TileLightMapCounter));
	for( int i = 0; i < 2048; i++ ){
		TileLightMapIndex[i] = i;
	}
	if( Dungeon->genType == DT_0_TOWN ) return;
	int activeTiles = Dungeon->genType == DT_4_HELL ? 12 : 10;
	for( int i = 0; i < 112*112; i++ ){
		for( int t = 0; t < activeTiles; t++ ){
			if( ushort tile = TileLightMap[ 0 ][ i].tile[t] ){
				++TileLightMapCounter[tile & 0xFFF ];
				TileLightMapTop[tile & 0xFFF] = tile & 0x7000; // 2048 = 128 * 16
			}
		}
	}
	//VD;
	LevelCELsAmount = *CurrentLevelCELFile & 0xFFFF;
	for( int i = 0; i < LevelCELsAmount; i++ ){
		SizeOfLevelCEL[i] = ushort(CurrentLevelCELFile[i + 1] - CurrentLevelCELFile[i]);
	}
	//VD;
	SizeOfLevelCEL[0] = 0;
	if( Dungeon->genType == DT_4_HELL ){// DRAW_BUG в тх2 нулится больше значений чем нужно
		int size = 0;
		if( LevelCELsAmount > 0 ){
			ushort* tileLightMapTop = TileLightMapTop;
			do{
				int flag = 1;
				if( TileLightMapCounter[ size ] ){
					uchar* pixel;
					int row;
					if( *tileLightMapTop == 0x1000 ){
						pixel = (unsigned __int8 *) CurrentLevelCELFile + CurrentLevelCELFile[ size ];
						row = 32;
						int col;
						int row_1;
						int pixelCount_1;
						int pixelCount;
						uchar pixel_1;
						do{
							row_1 = row;
							col = 32;
							do{
								while( 1 ){
									pixelCount_1 = *pixel++;
									if( (char) pixelCount_1 >= 0 ){
										break;
									}
									LOBYTE_IDA( pixelCount_1 ) = -(char) pixelCount_1;
									col -= pixelCount_1;
									if( !col ){
										goto LABEL_37;
									}
								}
								col -= pixelCount_1;
								pixelCount = pixelCount_1;
								do{
									pixel_1 = *pixel++;
									if( pixel_1 && pixel_1 < 32u ){
										flag = 0;
									}
									--pixelCount;
								}while( pixelCount );
							}while( col );
						LABEL_37:
							row = row_1 - 1;
						}while( row_1 != 1 );
					}else{
						uchar* pixel_3 = (uchar*) CurrentLevelCELFile + CurrentLevelCELFile[ size ];
						for( int i = SizeOfLevelCEL[ size ]; i; --i ){
							uchar pixel_2 = *pixel_3++;
							if( pixel_2 && pixel_2 < 32u ){
								flag = 0;
							}
						}
					}
					if( !flag ){
						TileLightMapCounter[ size ] = 0;
					}
				}
				++tileLightMapTop;
				++size;
			}while( size < LevelCELsAmount );
		}
		#ifdef refucktor
		for( int i = 0; i < LevelCELsAmount; i++ ){
			int flag = 1;
			uchar* cel = (uchar*)CurrentLevelCELFile + CurrentLevelCELFile[i];
			if( TileLightMapCounter[i] ){ // декодирование tile'ов lightmap из CEL ?
				if( TileLightMapTop[i] == 0x1000 ){
					uchar* pixel = cel;
					for( int row = 0; row < 32; row++ ){
						char col = 32;
						do{
							char pixelCount;
							while( (pixelCount = *pixel++) < 0 ){
								pixelCount = -pixelCount;
								if( (col -= pixelCount) != 0 ){
									goto NEXT_ROW;
								}
							}
							col -= pixelCount;
							do{
								if( *pixel != 0 && *pixel < 32 ){
									flag = 0;
								}
								pixel++;
							}while( --pixelCount );
						}while( col );
				NEXT_ROW:;
					}
				}else{
					for( uchar* pixel = cel; pixel < cel + SizeOfLevelCEL[i]; pixel++ ){
						if( *pixel != 0 && *pixel < 32 ){
							flag = 0;
						}
					}
				}
				if( !flag ){
					TileLightMapCounter[i] = 0;
				}
			}
		}
		#endif
	}
	SortTileLightmap(2047);
	int maxLightIndex = 0;
	int darkLevel = Light4Levels ? 3 : 15;
	for( int lastDrawingTilePixel = 0; lastDrawingTilePixel < 1024 * 1024; ){
		lastDrawingTilePixel += (darkLevel - 1) * SizeOfLevelCEL[maxLightIndex++];
	}
	if( --maxLightIndex > 128 ){
		maxLightIndex = 128;
	}
	int curDarkenPixel = 0;
	for( int lightIndex = 0; lightIndex < maxLightIndex; lightIndex++ ){
		DungeonTilesOffset[lightIndex * 16] = TileLightMapIndex[lightIndex];
		if( TileLightMapTop[lightIndex] == 0x1000 ){
			for( int brightness = 1; brightness < darkLevel; ++brightness ){
				DungeonTilesOffset[lightIndex * 16 + brightness] = curDarkenPixel;
				char* curCEL = (char*)CurrentLevelCELFile + CurrentLevelCELFile[TileLightMapIndex[lightIndex]];
				int darkenPixel = curDarkenPixel; // char* darkenPixel = (char*)&DarkenDungeonTiles[curDarkenPixel];
				for( int row = 0; row < 32; row++ ){
					int col = 32;
					do{
						char pixel;
						while( 1 ){
							pixel = *curCEL++;
							DarkenDungeonTiles[darkenPixel++] = pixel;
							if( pixel >= 0 )
								break;
							col += pixel;
							if( col == 0 )
								goto NEXT_row;
						}
						col -= pixel;
						while( pixel-- ){
							DarkenDungeonTiles[darkenPixel++] = ((uchar*)LightTable)[256 * brightness + *(uchar*)curCEL++]; // *(uchar*)darkenPixel++ =
						}
					}while( col );
NEXT_row:
					;
				}
				curDarkenPixel += SizeOfLevelCEL[lightIndex];
			}
		}else{ // TileLightMapTop[lightIndex] != 0x1000
			for( int brightness = 1; brightness < darkLevel; ++brightness ){
				DungeonTilesOffset[lightIndex * 16 + brightness] = curDarkenPixel;
				uchar* curCEL = (uchar*)CurrentLevelCELFile + CurrentLevelCELFile[TileLightMapIndex[lightIndex]];
				for( int pixel = SizeOfLevelCEL[lightIndex]; pixel; --pixel ){
					DarkenDungeonTiles[curDarkenPixel++] = ((uchar*)LightTable)[brightness * 256 + *curCEL++];
					if( curDarkenPixel > MaxDarkenTilePixelIndex ) MaxDarkenTilePixelIndex = curDarkenPixel;
				}
			}
		}
	}
	for( int cell = 0; cell < 112*112; cell++ ){
		if( FineMap[ 0 ][ cell ] ){
			for( ushort* tile = &TileLightMap[ 0 ][ cell ].tile[0]; tile < &TileLightMap[ 0 ][ cell ].tile[activeTiles]; tile++ ){
				if( *tile ){
					for( ushort lightIndex = 0; lightIndex < maxLightIndex; lightIndex++ ){
						if( (*tile & 4095) == TileLightMapIndex[lightIndex] ){
							*tile = lightIndex + TileLightMapTop[lightIndex] + 0x8000;
							break;
						}
					}
				}
			}
		}
	}
}

// bubble sort
//----- (0041BC9F) --------------------------------------------------------
void __fastcall SortTileLightmap(int count)
{
	for( bool prevLower = true; count > 0 && prevLower; count-- ){
		prevLower = false;
		for( int i = 0; i < count; i++ ){
			if( TileLightMapCounter[i] < TileLightMapCounter[i + 1] ){
				SwapTileData(i, i + 1);
				prevLower = true;
			}
		}
	}
}

//----- (0041BCDE) --------------------------------------------------------
void __fastcall SwapTileData(int counter, int nextCounter)
{
	int tmp1 = TileLightMapCounter[counter];
	TileLightMapCounter[counter] = TileLightMapCounter[nextCounter];
	TileLightMapCounter[nextCounter] = tmp1;
	int tmp2 = TileLightMapIndex[counter];
	TileLightMapIndex[counter] = TileLightMapIndex[nextCounter];
	TileLightMapIndex[nextCounter] = tmp2;
	ushort tmp3 = TileLightMapTop[counter];
	TileLightMapTop[counter] = TileLightMapTop[nextCounter];
	TileLightMapTop[nextCounter] = tmp3;
	int tmp4 = SizeOfLevelCEL[counter];
	SizeOfLevelCEL[counter] = SizeOfLevelCEL[nextCounter];
	SizeOfLevelCEL[nextCounter] = tmp4;
}


// изометрическое преобразование координат для вывода тайлов на экран ?
//----- (0041BD6A) --------------------------------------------------------
int __fastcall IsometricMapOffset(int row, int col) // CalcRot
{
	int result;
	if( row >= 112 - col ){
		result = 112 * 112 - 1 - ((111 - col) * (111 - col + 1) + (111 - row) * (111 - row + 2 * (111 - col) + 3)) / 2;
	}else{
		result = (col * (col + 1) + row * (row + 2 * col + 3)) / 2;
	}
	return result;
}

//----- (0041BDB2) --------------------------------------------------------
void InitDrawingTileWithLight()
{
	for( int row = 0; row < 112; row++ ){
		for( int col = 0; col < 112; col++ ){
			memcpy( &TileMap[ IsometricMapOffset( row, col ) ], &TileLightMap[ row ][ col ], sizeof( TileColumn ));
		}
	}
}

//----- (0041BDFB) --------------------------------------------------------
void InitDungeonTiles()
{
	int someCount;
	if( Dungeon->genType == DT_0_TOWN || Dungeon->genType == DT_4_HELL ){
		MaxTilesInCell = 12;
		someCount = 16;
	}else{
		MaxTilesInCell = 10;
		someCount = 10;
	}
	
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			uint dungeonMapNumber = FineMap[ row ][ col ];
			if( dungeonMapNumber ){
				uint dungeonMapIndex = dungeonMapNumber - 1;
				ushort* minPtr = (ushort*)CurrentLevelMINFile;
				for( int i = 0; i < someCount; i++ ){
					int decodedNumber = someCount + (i & 1) - (i & 14) - 2; // big end decode ?
					TileLightMap[ row ][ col ].tile[i] = minPtr[someCount * dungeonMapIndex + decodedNumber];
				}
			}else{
				memset( &TileLightMap[ row ][ col ], 0, 2 * someCount );
			}
		}
	}
	InitDarkenDungeonTiles(); // тут задаются тайлы разной яркости
	InitDrawingTileWithLight();
}

//----- (0041BF44) --------------------------------------------------------
void ClearWallsTransparency()
{
	memset( TransMap, 0, sizeof( TransMap ) );
	memset( TransList, 0, sizeof( TransList ) );
	TransValue = 1;
}

//----- (0041BF74) --------------------------------------------------------
void __fastcall SetAreaTransparencyByIndex_40( int startRow40, int startCol40, int endRow40, int endCol40 )
{
	int endRow112 = 2 * endRow40 + 16;
	int endCol112 = Convert40to112(endCol40);
	int startCol112 = Convert40to112(startCol40) + 1;
	int startRow112 = Convert40to112(startRow40) + 1;
	for( int curCol112 = startCol112; curCol112 <= endCol112; curCol112++ ){
		for( int curRow112 = startRow112; curRow112 <= endRow112; curRow112++ ){
			TransMap[ curRow112 ][ curCol112 ] = TransValue;
		}
	}
	TransValue++;
}

//----- (0041BFCD) --------------------------------------------------------
void __fastcall SetRectTrans( int startRow112, int startCol112, int endRow112, int endCol112 ) // DRLG_RectTrans
{
	for( int col = startCol112; col <= endCol112; col++ ){
		for( int row = startRow112; row <= endRow112; row++ ){
			TransMap[ row ][ col ] = TransValue;
		}
	}
	TransValue++;
}

//----- (0041C012) --------------------------------------------------------
void __fastcall CopyCellTransparency(int rowFrom, int colFrom, int rowTo, int colTo)
{
	TransMap[ rowTo ][ colTo ] = TransMap[ rowFrom ][ colFrom ];
}

//----- (0041C031) --------------------------------------------------------
void __fastcall FillAreasDiffOnMap112x112u(int aAreaNumber, MapArea* aAreaPtr)
{
	MapArea* AreaPtr;       // esi@1
	int AreaCount;          // edi@2
	char StartRow;          // al@3
	char StartCol;          // cl@3
	char* v6;               // esi@3
	char EndRow;            // dl@3
	unsigned __int8 EndCol; // bl@3
	AreaPtr = aAreaPtr;
	if( aAreaNumber > 0 ){
		AreaCount = aAreaNumber;
		do{
			StartRow = AreaPtr->StartRow;
			StartCol = AreaPtr->StartCol;
			v6 = (char*)&AreaPtr->EndRow;
			EndRow = *v6;
			EndCol = *(v6 + 1);
			AreaPtr = (MapArea*)(v6 + 2);
			SetRectTrans((unsigned __int8)StartRow, (unsigned __int8)StartCol, (unsigned __int8)EndRow, EndCol);
			--AreaCount;
		}while( AreaCount );
	}
}

//----- (0041C064) --------------------------------------------------------
void __fastcall FillAreasSomeOnMap112x112u(int AreaNumber, MapArea* aAreaPtr)
{
	MapArea* AreaPtr;       // esi@1
	int AreaCount;          // edi@2
	char StartRow;          // al@3
	char StartCol;          // cl@3
	unsigned __int8* v6;    // esi@3
	unsigned __int8 EndRow; // dl@3
	unsigned __int8 EndCol; // bl@3
	AreaPtr = aAreaPtr;
	if( AreaNumber > 0 ){
		AreaCount = AreaNumber;
		do{
			StartRow = AreaPtr->StartRow;
			StartCol = AreaPtr->StartCol;
			v6 = (unsigned __int8* )&AreaPtr->EndRow;
			EndRow = *v6;
			EndCol = *(v6 + 1);
			AreaPtr = (MapArea*)(v6 + 2);
			SetRectTrans((unsigned __int8)StartRow, (unsigned __int8)StartCol, EndRow, EndCol);
			--TransValue;
			--AreaCount;
		}while( AreaCount );
	}
	++TransValue;
}

//----- (0041C0A3) --------------------------------------------------------
void ClearCurPattern()
{
	CurPattern = {};
}

//----- (0041C0BA) --------------------------------------------------------
void MarkPatternPositionOnMap(QuestPattern& qp)
{
	int startRow = Convert40to112(qp.row);
	int startCol = Convert40to112(qp.col);
	int endRow = startRow + 2 * qp.sizeRow;
	int endCol = startCol + 2 * qp.sizeCol;
	for( int row = startRow; row < endRow; row++ ) for( int col = startCol; col < endCol; col++ ){
		FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
	}
}

//---- (00718B90) -------------------------------------------------------------
void UnMarkPatternPositionOnMap(QuestPattern& qp)
{
	int startRow = Convert40to112(qp.row);
	int startCol = Convert40to112(qp.col);
	int endRow = startRow + 2 * qp.sizeRow;
	int endCol = startCol + 2 * qp.sizeCol;
	for( int row = startRow; row < endRow; row++ ) for( int col = startCol; col < endCol; col++ ){
		FlagMap[ row ][ col ] &= ~CF_8_FORBIDDEN;
	}
}

//---- (th4) ------------------------------------------------------------------
QuestPattern* GetQuestPattern(QUEST qi)
{
	if( IsQuestOnLevel(qi) ){
		for( QuestPattern& qp: QuestPatterns ){
			if( qp.id == qi ) return qp.placed ? &qp : 0;
		}
	}
	return 0;
}

//---- (th4) ------------------------------------------------------------------
bool SetQuestPatternWarp(int row, int col)
{
	for(QuestPattern& qp: QuestPatterns){
		if( qp.id != Q_NO && qp.placed && ! (qp.flag & QP_PORTAL)
		 && abs(row - (Convert40to112(qp.row) + qp.ofsRow)) < 3
		 && abs(col - (Convert40to112(qp.col) + qp.ofsCol)) < 3 ){
			if( DUNGEON destDun = Quests[qp.id].questDungeon; destDun != DUN_NO ){
				AddWarp(row, col, WM_1029_GOTO_QUEST_DUNGEON, destDun);
				return true;
			}
			break;
		}
	}
	return false;
}

//---- (th4) ------------------------------------------------------------------
bool SetCurPattern(QUEST qi)
{
	for( QuestPattern& qp: QuestPatterns ){
		if( qp.id == qi && qp.placed ){
			TmpPattern = CurPattern;
			CurPattern = qp;
			return true;
		}
	}
	return false;
}

//---- (th4) ------------------------------------------------------------------
void RestoreCurPattern()
{
	CurPattern = TmpPattern;
}

//----- (0041C10E) --------------------------------------------------------
void __fastcall MarkPandemoniumPositionOnMap(int startRow40, int startCol40, int rowSize40, int colSize40)
{
	int startRow112 = Convert40to112(startRow40);
	int startCol112 = Convert40to112(startCol40);
	int endRow112 = startRow112 + 2*rowSize40;
	int endCol112 = startCol112 + 2*colSize40;
	for( int col = startCol112; col < endCol112; col++ ){
		for( int row = startRow112; row < endRow112; row++ ){
			FlagMap[ row ][ col ] |= CF_8_FORBIDDEN;
		}
	}
}

//----- (0041C15D) --------------------------------------------------------
bool __fastcall GetMaxSizeForRoom(int floorTilesIndex, int starRow, int startCol, int minSizeOfRooms, int maxSizeOfRooms, int& row40Size, int& col40Size)
{
	if( starRow > Map_40 - maxSizeOfRooms || startCol > Map_40 - maxSizeOfRooms || !IsCoordinateNotNearAloneRoom(starRow, startCol) ){
		return 0;
	}

	// Двигаемся по двум лучам, определяем размер линии пола идущей паралельно лучу.
	uint rowSizesTable[20];
	uint colSizesTable[20];
	memset(rowSizesTable, 0, sizeof( rowSizesTable ));
	memset(colSizesTable, 0, sizeof( colSizesTable ));
	bool isNotSecondBeamBlocked = 1;
	bool isNotFirstBeamBlocked = 1;

	for( int curSizeIndex = 0; curSizeIndex < maxSizeOfRooms; curSizeIndex++ ){

		if( isNotFirstBeamBlocked ){
			int curRowSize = 0;
			for( int curRow = starRow; curRow < starRow + maxSizeOfRooms; curRow++ ){
				if( Map[ curRow ][ curSizeIndex + startCol ] == floorTilesIndex ){
					curRowSize++;
				}else{
					if( curRow >= minSizeOfRooms ){ // непонятное условие
						break;
					}
					isNotFirstBeamBlocked = 0;
				}
			}
			if( isNotFirstBeamBlocked ){
				rowSizesTable[curSizeIndex] = curRowSize;
			}
		}

		if( isNotSecondBeamBlocked ){
			int curColSize = 0;
			for( int curCol = startCol; curCol < startCol + maxSizeOfRooms; curCol++ ){
				if( Map[ starRow + curSizeIndex ][ curCol ] == floorTilesIndex ){
					curColSize++;
				}else{
					if( curCol >= minSizeOfRooms ){
						break;
					}
					isNotSecondBeamBlocked = 0;
				}
			}
			if( isNotSecondBeamBlocked ){
				colSizesTable[curSizeIndex] = curColSize;
			}
		}

	}

	// если даже минимальную комнату не построить, то выходим
	for( int curSizeIndex = 0; curSizeIndex < minSizeOfRooms; curSizeIndex++ ){
		int curRowSize = rowSizesTable[curSizeIndex];
		if( curRowSize < minSizeOfRooms ){
			return 0;
		}
		int curColSize = colSizesTable[curSizeIndex];
		if( curColSize < minSizeOfRooms ){
			return 0;
		}
	}

	// определяем максимально возможные размеры комнаты
	int minimumRowSize = (uint)rowSizesTable;
	int minimumColSize = (uint)colSizesTable;
	for( int curSizeIndex = 0; curSizeIndex < maxSizeOfRooms; curSizeIndex++ ){
		int curRowSize = rowSizesTable[curSizeIndex];
		if( curRowSize < minSizeOfRooms ){
			break;
		}
		int curColSize = colSizesTable[curSizeIndex];
		if( curColSize < minSizeOfRooms ){
			break;
		}
		if( curRowSize < minimumRowSize ){
			minimumRowSize = curRowSize;
		}
		if( curColSize < minimumColSize ){
			minimumColSize = curColSize;
		}
	}
	row40Size = minimumRowSize - 2;
	col40Size = minimumColSize - 2;
	return 1;
}

//----- (0041C2F9) --------------------------------------------------------
void __fastcall SetupRoom(int roomIndex)
{
	Chamber& room = Chambers[roomIndex];

	int startCol = room.startCol40;
	int startRow = room.startRow40;
	int colSize = room.colSize40;
	int rowSize = room.rowSize40;
	int endCol = startCol + colSize;
	int endRow = startRow + rowSize;

	for( int curCol = startCol; curCol < endCol; curCol++ ){
		for( int curRow = startRow; curRow < endRow; curRow++ ){
			uchar& curWallAndFloorValue = Map[ curRow ][ curCol ];
			if( Dungeon->genType == DT_2_CATACOMB ){
				if( curCol == startCol && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = CATA_WAFM_2_RU_WALL;
				}else if( curCol == endCol - 1 && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = CATA_WAFM_2_RU_WALL;
				}else if( curRow == startRow && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = CATA_WAFM_1_LU_WALL;
				}else if( curRow == endRow - 1 && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = CATA_WAFM_1_LU_WALL;
				}else{
					curWallAndFloorValue = CATA_WAFM_3_FLOOR;
				}
			}
			if( Dungeon->genType == DT_3_CAVE ){
				if( curCol == startCol && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = CAVE_WAFM_134_WOOD_WALL_M_MIDDLE;
				}else if( curCol == endCol - 1 && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = CAVE_WAFM_134_WOOD_WALL_M_MIDDLE;
				}else if( curRow == startRow && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = CAVE_WAFM_137_WOOD_WALL_UNM_MIDDLE_BROKEN;
				}else if( curRow == endRow - 1 && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = CAVE_WAFM_137_WOOD_WALL_UNM_MIDDLE_BROKEN;
				}else{
					curWallAndFloorValue = CAVE_WAFM_7_FLOOR;
				}
			}
			if( Dungeon->genType == DT_4_HELL ){
				if( curCol == startCol && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = HELL_WAFM_2;
				}else if( curCol == endCol - 1 && curRow >= startRow && curRow <= endRow ){
					curWallAndFloorValue = HELL_WAFM_2;
				}else if( curRow == startRow && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = HELL_WAFM_1;
				}else if( curRow == endRow - 1 && curCol >= startCol && curCol <= endCol ){
					curWallAndFloorValue = HELL_WAFM_1;
				}else{
					curWallAndFloorValue = HELL_WAFM_6_FLOOR;
				}
			}
		}
	}

	if( Dungeon->genType == DT_2_CATACOMB ){
		Map[ startRow ][ startCol ] = CATA_WAFM_8_CORNER;
		Map[ endRow - 1 ][ startCol ] = CATA_WAFM_7_CORNER;
		Map[ startRow ][ endCol - 1 ] = CATA_WAFM_9_CORNER;
		Map[ endRow - 1 ][ endCol - 1 ] = CATA_WAFM_6_CORNER;
	}
	if( Dungeon->genType == DT_3_CAVE ){
		Map[ startRow ][ startCol ] = CAVE_WAFM_150_WOOD_WALL;
		Map[ endRow - 1 ][ startCol ] = CAVE_WAFM_151_WOOD_WALL;
		Map[ startRow ][ endCol - 1 ] = CAVE_WAFM_152_WOOD_WALL;
		Map[ endRow - 1 ][ endCol - 1 ] = CAVE_WAFM_138_WOOD_WALL_WOOD_CORNER;
	}
	if( Dungeon->genType == DT_4_HELL ){
		Map[ startRow ][ startCol ] = HELL_WAFM_9;
		Map[ endRow - 1 ][ startCol ] = HELL_WAFM_16;
		Map[ startRow ][ endCol - 1 ] = HELL_WAFM_15;
		Map[ endRow - 1 ][ endCol - 1 ] = HELL_WAFM_12;
	}
	if( Dungeon->genType == DT_2_CATACOMB ){
		if( RNG(2) ){
			Map[ rowSize / 2 + startRow ][ endCol - 1 ] = CATA_WAFM_5_RU_ODOOR;
		}else{
			Map[ endRow - 1 ][ colSize / 2 + startCol ] = CATA_WAFM_4_LU_ODOOR;
		}
	}
	if( Dungeon->genType == DT_3_CAVE ){
		if( RNG(2) ){
			Map[ rowSize / 2 + startRow ][ endCol - 1 ] = CAVE_WAFM_146_WOOD_WALL_M_CDOOR;
		}else{
			Map[ endRow - 1 ][ colSize / 2 + startCol ] = CAVE_WAFM_147_WOOD_WALL_UNM_CDOOR;
		}
	}
	if( Dungeon->genType == DT_4_HELL ){
		if( RNG(2) ){
			int cell = (rowSize / 2 + startRow) * 40 + (endCol - 1);
			Map[ 0 ][ cell - 40 ] = HELL_WAFM_57;
			Map[ 0 ][ cell ] = HELL_WAFM_6_FLOOR;
			Map[ 0 ][ cell + 40 ] = HELL_WAFM_56;
			Map[ 0 ][ cell - 1 ] = HELL_WAFM_59;
			Map[ 0 ][ cell - 41 ] = HELL_WAFM_58;
		}else{
			int cell = (endRow - 1) * 40 + colSize / 2 + startCol;
			Map[ 0 ][ cell - 1 ] = HELL_WAFM_53;
			Map[ 0 ][ cell ] = HELL_WAFM_6_FLOOR;
			Map[ 0 ][ cell + 1 ] = HELL_WAFM_52;
			Map[ 0 ][ cell - 41 ] = HELL_WAFM_54;
		}
	}
}

//----- (0041C708) --------------------------------------------------------
void __fastcall CreateRooms(int minSizeOfRooms, int maxSizeOfRooms, int floorTilesIndex, int skipTilesParam, int needSizeRandomization)
{
	ChambersCount = 0;
	memzero(Chambers);

	for( int curCol40 = 0; curCol40 < Map_40; curCol40++ ){
		for( int curRow40 = 0; curRow40 < Map_40; curRow40++ ){
			if( Map[ curRow40 ][ curCol40 ] != floorTilesIndex ){
				continue;
			}
			if( RNG(skipTilesParam) ){
				continue;
			}
			int row40Size, col40Size;
			if( !GetMaxSizeForRoom(floorTilesIndex, curRow40, curCol40, minSizeOfRooms, maxSizeOfRooms, row40Size, col40Size) ){
				continue;
			}
			if( needSizeRandomization ){
				int minSize = minSizeOfRooms - 2;
				int maxSize = maxSizeOfRooms - 2;

				row40Size = minSize + RNG(RNG(row40Size - minSize + 1));
				if( row40Size < minSize || row40Size > maxSize ){
					row40Size = minSize;
				}

				col40Size = minSize + RNG(RNG(col40Size - minSize + 1));
				if( col40Size < minSize || col40Size > maxSize ){
					col40Size = minSize;
				}
			}
			Chamber& aloneRoom = Chambers[ChambersCount];
			aloneRoom.startRow40 = curRow40 + 1;
			aloneRoom.startCol40 = curCol40 + 1;
			aloneRoom.rowSize40 = row40Size;
			aloneRoom.colSize40 = col40Size;
			if( Dungeon->genType == DT_3_CAVE ){
				SetRectTrans( Convert40to112(curRow40 + 2), Convert40to112(curCol40 + 2), Convert40to112(curRow40 + row40Size) - 1, Convert40to112(curCol40 + col40Size) - 1);
			}else{
				SetAreaTransparencyByIndex_40(curRow40 + 1, curCol40 + 1, curRow40 + row40Size, curCol40 + col40Size);
			}
			aloneRoom.transIndex = TransValue - 1;
			SetupRoom(ChambersCount);
			++ChambersCount;
		}
	}
}

// Holds theme rooms created in dungeon types 2, 3 & 4.
//----- (0041C894) --------------------------------------------------------
void SetForbiddenObjectFlagForAloneRooms2()
{
	for( int aloneRoomCoordinatesIndex = 0; aloneRoomCoordinatesIndex < ChambersCount; aloneRoomCoordinatesIndex++ ){
		Chamber& coord = Chambers[aloneRoomCoordinatesIndex];
		int endRow40 = coord.startRow40 + coord.rowSize40 - 1;
		int endCol40 = coord.startCol40 + coord.colSize40 - 1;
		for( int row40 = coord.startRow40; row40 < endRow40; row40++ ){
			for( int col40 = coord.startCol40; col40 < endCol40; col40++ ){
				int cell = 112 * Convert40to112 (row40) + Convert40to112 (col40);
				FlagMap[ 0 ][ cell ] |= CF_8_FORBIDDEN;
				FlagMap[ 0 ][ cell + 112 ] |= CF_8_FORBIDDEN;
				FlagMap[ 0 ][ cell + 1 ] |= CF_8_FORBIDDEN;
				FlagMap[ 0 ][ cell + 113 ] |= CF_8_FORBIDDEN;
			}
		}
	}
}

//----- (0041C92E) --------------------------------------------------------
bool __fastcall IsCoordinateNotNearAloneRoom( int row, int col )
{
	for( int aloneRoomIndex = 0; aloneRoomIndex < ChambersCount; aloneRoomIndex++ ){
		Chamber& room = Chambers[aloneRoomIndex];

		int startRow = room.startRow40 - 2;
		int endRow = room.rowSize40 + 2 + room.startRow40;
		int startCol = room.startCol40 - 2;
		int endCol = room.colSize40 + 2 + room.startCol40;

		if( InSegment(row, startRow, endRow) && InSegment(col, startCol, endCol) ){
			return false;
		}
	}
	return true;
}

//----- (0041C980) --------------------------------------------------------
void _cdecl InitLevels()
{
	if( !DungeonDebug ){
		Dungeon = DUN_0_TOWN;
	}
}
