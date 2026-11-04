#include "stdafx.h"

int UberRow;// 00577360
int UberCol;// 00577364
int SomeDword_64;// 00577368
int IsUberRoomOpened;// 0057736C
int UberLeverRow;// 00577370
int UberLeverCol;// 00577374
int IsUberLeverActivated;// 00577378
int UberDiabloMonsterIndex;// 0057737C

//----- (00454963) --------------------------------------------------------
void Floor24QuestMapObjectsGeneration()
{
	AddQuestLeverOn24Level();
	// (rt) adding 3 books for opening Uber's chamber
	int indexesTable [6][3] = {// (rt) optimization. The original had 6 conditions
		{6, 7, 8},
		{6, 8, 7},
		{7, 6, 8},
		{7, 8, 6},
		{8, 6, 7},
		{8, 7, 6},
	};
	int shuffleIndex = RNG(6);
	indexesTable[shuffleIndex][0];
	AddQuestBookOnMap(indexesTable[shuffleIndex][0], UberRow + 3, UberCol);
	AddQuestBookOnMap(indexesTable[shuffleIndex][1], UberRow + 2, UberCol - 3);
	AddQuestBookOnMap(indexesTable[shuffleIndex][2], UberRow + 2, UberCol + 2);
}

//----- (00454AF0) --------------------------------------------------------
void __fastcall AddQuestBookOnMap( int bookIndex, int row112, int col112 )
{
	AddQuestBook86(BO_86_QUEST_BOOK, bookIndex, row112, col112);
}

// (rt) reworked for use with the books from the Horazon quest. Original values in the comments
//----- (00454B04) --------------------------------------------------------
bool __fastcall ReadUberBook(int bookIndex)
{
	if( bookIndex == 1/*6*/ ){
		UberBookReadParam = 1;
	}else if( bookIndex == 2/*7*/ ){
		if( UberBookReadParam == 1 ){
			UberBookReadParam = 2;
		}else{
			UberBookReadParam = 0;
		}
	}else if( bookIndex == 3/*8*/ ){
		if( UberBookReadParam == 2 ){
			return true;
		}
		UberBookReadParam = 0;
	}
	return false;
}

//----- (00454B46) --------------------------------------------------------
void OpenUberDiabloRoom()
{
	if( Dungeon == DUN_24_CRYPT_4 ){
		PlayLocalSound(S_1020_I_CROPEN, UberRow, UberCol);
		FineMap[ UberRow ][ UberCol ] = 298;
		FineMap[ UberRow ][ UberCol + 1 ] = 299;
		FineMap[ UberRow ][ UberCol -2 ] = 300;
		FineMap[ UberRow ][ UberCol - 1 ] = 301;
		// (rt) activation and sound when Uber is released from the chamber
		Monsters[UberDiabloMonsterIndex].ActivationCounter = 255;
		PlayGlobalSound(S_997_M_UBERGRT);
		MonsterLookAround(UberDiabloMonsterIndex);

		InitDungeonTiles();
	}
}

//----- (00454BA8) --------------------------------------------------------
void LoadOpennedUberRoom()
{
	FineMap[ UberRow ][ UberCol ] = 298;
	FineMap[ UberRow ][ UberCol + 1 ] = 299;
	FineMap[ UberRow ][ UberCol -2 ] = 300;
	FineMap[ UberRow ][ UberCol - 1 ] = 301;
	InitDungeonTiles();
}

//----- (00454BEA) --------------------------------------------------------
void AddQuestLeverOn24Level()
{
	int row; // edi@1
	int col; // eax@1
	int col_16; // esi@1
	int col_15; // ebx@1
	int v4; // esi@7

	while( 1 ){
		#ifndef REFACTOR
		do{
			row = RNG(80) + 16;
			col = RNG(80);
			col_16 = col + 16;
			col_15 = col + 15;
		}while( !IsCellFree( row - 1, col + 15 ) );
		if( IsCellFree( row, col_15 ) ){
			if( IsCellFree( row + 1, col_15 ) ){
				if( IsCellFree( row - 1, col_16 ) ){
					if( IsCellFree( row, col_16 ) ){
						if( IsCellFree( row + 1, col_16 ) ){
							v4 = col_16 + 1;
							if( IsCellFree( row - 1, v4 ) ){
								if( IsCellFree( row, v4 ) && IsCellFree( row + 1, v4 ) ){
									break;
								}
							}
						}
					}
				}
			}
		}
		#else
		int row112 = RNG(80) + 16;
		int col112 = RNG(80) + 16;
		int isValidCol = true;
		for( int rowOffset = -1; rowOffset <= 1; rowOffset++ ){
			for( int colOffset = -1; colOffset <= 1; colOffset++ ){
				if( !IsCellFree(row112 + rowOffset, col112 + colOffset) ){// (rt) optimization, the original had 9 calls
					isValidCol = false;
				}
			}
		}
		if( isValidCol ){
			break;
		}
		#endif
	}
	// (rt) the found row and col value is not used
	// (rt) the coordinate dwords aren't used either
	UberLeverRow = UberRow + 3;
	UberLeverCol = UberCol - 1;
	AddObject(BO_4_LEVER, UberRow + 3, UberCol - 1);
	// (rt) in th, opening is done not by just swapping a pair of tiles but by replacing the whole room with the opened room
	SetChangeRect(
		ObjIndex(UberLeverRow, UberLeverCol),
		CurPattern.row,
		CurPattern.col,
		CurPattern.sizeRow + CurPattern.row,
		CurPattern.sizeCol + CurPattern.col,
		1);
}