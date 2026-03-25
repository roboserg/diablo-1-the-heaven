#include "stdafx.h"

//----- (0045F29F) --------------------------------------------------------
void InitPortals()
{
	for( int playerIndex = 0; playerIndex < 4; playerIndex++ ){
		Portal& entryPoint = Portals[playerIndex];
		if( delta_portal_inited( playerIndex ) ){
			entryPoint.open = 0;
		}
	}
}

//----- (0045F2C5) --------------------------------------------------------
void __fastcall SyncPortal( int playerIndex, int exists, int xPos, int yPos, DUNGEON dungeon )
{
	Portals[playerIndex].open = exists;
	Portals[playerIndex].row = xPos;
	Portals[playerIndex].col = yPos;
	Portals[playerIndex].dungeon = dungeon;
}

//----- (0045F303) --------------------------------------------------------
void __fastcall AddWarpMissile( int playerIndex, int row, int col )
{
	BaseMissile& townPortalEffect = BaseMissiles[MI_10_TOWN_PORTAL];
	townPortalEffect.CastSound = S_M1_NO_SOUND;
	MissilesMap[ row ][ col ] = 0;
	int spellCastIndex = CastMissile(0, 0, row, col, 0, MI_10_TOWN_PORTAL, CT_0_PLAYER, playerIndex, 0, 0, 0 );
	if( spellCastIndex == -1 ){
		return;// если не скастуется, то звук не восстановится?
	}
	SetMissileDirection(spellCastIndex, 1);
	if( Dungeon != DUN_0_TOWN ){
		Missile& spell = Missiles[spellCastIndex];
		spell.LightIndex = AddLight(spell.Row, spell.Col, 15);
	}
	townPortalEffect.CastSound = S_136_SENTINEL;
}

//----- (0045F377) --------------------------------------------------------
void SyncPortals()
{
	// Add the warp fields
	for( int playerIndex = 0; playerIndex < PlayersMax_4; ++playerIndex ){
		Portal& portal = Portals[ playerIndex ];
		if( portal.open ){
			if( Dungeon != DUN_0_TOWN ){
				if( portal.dungeon == Dungeon ){
					AddWarpMissile( playerIndex, portal.row, portal.col );
				}
			}else{
				AddWarpMissile( playerIndex, TownStartingRow[ playerIndex ], TownStartingCol[ playerIndex ] );
			}
		}
	}
	//if( MaxCountOfPlayersInGame == 1 ){ // enabling ravenholm red portals in mp
		Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( is(Dungeon, DUN_0_TOWN, DUN_44_RAVENHOLM) && theRampagingDemonQuest.status == QS_2_IN_PROGRESS && theRampagingDemonQuest.status3 == 1 ){
			int row = 53, col = 63;
			if( Dungeon == DUN_44_RAVENHOLM ){
				row = 93; col = 89;
			}
			//int isSoundEnable = IsSoundEnable; IsSoundEnable = 0;
			CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
			//IsSoundEnable = isSoundEnable;
		}
	//}
}

//----- (0045F3D2) --------------------------------------------------------
void __fastcall AddinTownPortal(int playerIndex)
{
	AddWarpMissile(playerIndex, TownStartingRow[playerIndex], TownStartingCol[playerIndex]);
}

//----- (0045F3E6) --------------------------------------------------------
void __fastcall ActivatePortal( int playerIndex, int row, int col, DUNGEON dungeon )
{
	Portal& portal = Portals[playerIndex];
	portal.open = true;
	if( dungeon != DUN_0_TOWN ){
		portal.row = row;
		portal.col = col;
		portal.dungeon = dungeon;
	}
}

//----- (0045F42B) --------------------------------------------------------
void __fastcall DeactivatePortal(int playerIndex)
{
	Portals[ playerIndex ].open = false;
}

//----- (0045F437) --------------------------------------------------------
int __fastcall PortalOnLevel(int playerIndex)
{
	//TP owner takes Town TP back to quest map, all players will now see this TP correctly disappear.
	if( Portals[playerIndex].dungeon == Dungeon ){
		return true;
	}
	return Dungeon == DUN_0_TOWN;
}

//----- (0045F459) --------------------------------------------------------
void __fastcall ClearAllTownPortalsOwnedThisPlayer( int playerIndex )
{
	for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
		int spellIndex = MissileIndexes[spellIndexIndex];
		Missile& spell = Missiles[spellIndex];
		if( spell.BaseMissileIndex == MI_10_TOWN_PORTAL && spell.CasterIndex == playerIndex && In112(spell.Row, spell.Col) ){
			FlagMap[ spell.Row ][ spell.Col ] &= ~CF_1_SPELL_CAST;
			MissilesMap[ spell.Row ][ spell.Col ] = 0;
			if( Portals[playerIndex].dungeon ){
				LightOff(spell.LightIndex);
			}
			RemoveMissile(spellIndex, spellIndexIndex);
		}
	}
}

//----- (0045F5A4) --------------------------------------------------------
void GetLevelEntryPoint()
{
	if( Dungeon != DUN_0_TOWN ){ // in Dungeon
		PlayerRowPos = Portals[CurrentPortal].row; // установка координат игрока в после портала
		PlayerColPos = Portals[CurrentPortal].col;
		if( CurrentPortal != CurrentPlayerIndex ){
			++PlayerRowPos; // здесь игрок уходит в стену
			++PlayerColPos;
			if( IsCellBlocking(PlayerRowPos, PlayerColPos) ){ // добавляем проверку на стену
				PlayerRowPos -= 2;
				PlayerColPos -= 2;
			}
		}
	}else{ // in town
		PlayerRowPos = TownStartingRow[CurrentPortal] + 1;
		PlayerColPos = TownStartingCol[CurrentPortal] + 1;
	}
}

//----- (th3) -------------------------------------------------------------
int __fastcall CellFreeForEnter(int playerIndex, int row, int col)
{
	if( ! In112(row,col) ) return false;
	int objectNum = ObjectsMap[row][col];
	int objectIndex = abs(objectNum) - 1;

	bool isWarp = false;
	for( auto& warp: Warps ) if( warp.Row == row && warp.Col == col ){ isWarp = true; break; }
	
	return ! isWarp && FineMap[row][col] && ! IsCellBlocking(row, col)
		&& ( ! objectNum || ! Objects[objectIndex].isBlockWalking );
}


//----- (th3) -------------------------------------------------------------
bool RadialsCellFound(int& row, int& col, int playerIndex, int (__fastcall *cellOk)(int,int,int))
{
	for( int cellOfs = 0; cellOfs < FineMap_112; cellOfs++ ){
		for( int colOfs = -cellOfs; colOfs <= cellOfs; colOfs++ ){
			int c = col + colOfs;
			for (int rowOfs = -cellOfs; rowOfs <= cellOfs; rowOfs++) {
				int r = row + rowOfs;
				if( cellOk(playerIndex, r, c) ){ 
					row = r;
					col = c;
					return true;
				}
			}
		}
	}
	return false;
}

//----- (th3) -------------------------------------------------------------
void FixTPandStairEntryPoint(int lvlDir) // Handle player position collision upon taking stairs and TP.  TP has loadingMode = 5.
{
	// Player is no longer put inside stair when loading Crypt levels, which caused screen to shake after moving, and the fix to fail
	if( Dungeon->graphType == DT_5_CRYPT && not(lvlDir, LVL_1_UP, LVL_5_WARP) ){
		PlayerColPos += 2;
	}

	// SP does NOT need the rest of the code
	if (MaxCountOfPlayersInGame > 1){
	
		// When going to Town via stairs (loadingMode == 7, except arriving in Town via Church entrance), each player is initially assigned a unique position based on CurrentPlayerIndex, as Town map is fixed
		if( Dungeon == DUN_0_TOWN && lvlDir != LVL_5_WARP ){
			ShiftPositionStairToTown();
		}

		// This deals with position collision when >= 2 players are taking the same stair at the same time, Dungeon only
		int playerRowPosOri = PlayerRowPos; // Remember the original position in case position shift result is not a good tile.
		int playerColPosOri = PlayerColPos;
		for (int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++) {
			Player& player = Players[playerIndex];
			if( player.IsExists && CurrentPlayerIndex != playerIndex && player.lvlChanging && player.dungeon == Dungeon ){
				if( lvlDir == LVL_5_WARP ){ // Taking TP
					PlayerRowPos += CurrentPlayerIndex / 2 + 1;// 0: (1, 0)   1: (1, 1)   2: (2, 1)   3: (2, 2)
					PlayerColPos += (CurrentPlayerIndex + 1) / 2;
				}else if( Dungeon != DUN_0_TOWN ){ // Taking stairs.  Need to be large enough shift to avoid overlapping in Abyss.  This shift better depends on entrance orientation...
					PlayerRowPos += CurrentPlayerIndex / 2 + 2;// 0: (2, 1)   1: (2, 2)   2: (3, 2)   3: (3, 3)
					PlayerColPos += (CurrentPlayerIndex + 1) / 2 + 1;
				}
				break;
			}
		}

		// Now deal with player position collision when players are loaded at the same / different time.
		// The following code searches nearby tiles in spiral pattern, from center to edge: https://stackoverflow.com/questions/3706219/algorithm-for-iterating-over-an-outward-spiral-on-a-discrete-2d-grid-from-the-or
		int NumGrid = 225;// 225 is a 15x15 square centered on player.  Need to be a large enough region to search for better player placement, and small enough so player position won't potentially shift too much.
		int di = 1;// (di, dj) is a vector - direction in which we move right now
		int dj = 0;
		int segment_length = 1;// Length of current segment

		int NewRow = PlayerRowPos;// Current position (i, j) and how much of current segment we passed
		int NewCol = PlayerColPos;
		int segment_passed = 0;

		bool mayBeGood = false;
		for (int k = 0; k < NumGrid; ++k) {// Make a step, add 'direction' vector (di, dj) to current position (i, j)
			if (!IsStairEntryCellFreeFromPlayer(NewRow, NewCol) || TileBlockWalking[FineMap[NewRow][NewCol]] || IsPointNearWarp(NewRow, NewCol) || IsCellTownPortal(NewRow, NewCol)) {// Additional checks to make sure player won't end up inside a wall / take the same stair back to previous Dlvl.  Note that this check does NOT work if player ends up in stones near Cave entrance in Town, which shouldn't happen anyway.  Note that CellFreeForPlayer() and IsCellBlocking() do NOT work, because PlayerMap[][] and MonsterMap[][] are NOT yet loaded upon taking stairs
				NewRow += di;
				NewCol += dj;
				++segment_passed;

				if (segment_passed == segment_length) {
					segment_passed = 0;// Done with current segment

					int buffer = di;// 'Rotate' directions
					di = -dj;
					dj = buffer;

					if (dj == 0) {// Increase segment length if necessary
						++segment_length;
					}
				}
			} else { // Done if a good tile to place player is found.
				PlayerRowPos = NewRow;
				PlayerColPos = NewCol;
				mayBeGood = true;
				break;
			}
		}
		if( ! mayBeGood ){
			// If fails to find a good tile to place player, PlayerRowPos and PlayerColPos are not modified.
			PlayerRowPos = playerRowPosOri;
			PlayerColPos = playerColPosOri;
		}

		if( ! CellFreeForPlayer(CurrentPlayerIndex, PlayerRowPos, PlayerColPos) ){
			RadialsCellFound(PlayerRowPos, PlayerColPos, CurrentPlayerIndex, CellFreeForEnter);
		}
	}
}

// Does NOT deal with player-monster position collision as neither PlayerMap[][] nor MonsterMap[][] is loaded yet
int __fastcall IsStairEntryCellFreeFromPlayer(int row, int col)
{
	for( int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++ ){
		Player& player = Players[playerIndex];
		if( player.IsExists && CurrentPlayerIndex != playerIndex ){
			if( player.Row == row && player.Col == col && !player.lvlChanging && player.dungeon == Dungeon ){ // Without Crypt stair entrance position adjustment PlayerColPos += 2, it would require player.TargetRow for fix to work
				return false;
			}
		}
	}
	return true;
}

void ShiftPositionStairToTown()// Final position may change slightly due to player collision and nearby warp checks
{
	// 3D array containing CurrentPlayerIndex and desired PlayerRowPos and PlayerColPos info for various dungeon entrances in Town
	// 1st dimension = CurrentPlayerIndex from 0 to 3, 2nd dimension = which 1 of the 6 entrances, 3rd dimension = PlayerRowPos or PlayerColPos
	static const int Offset[4][7][2] = {
		{ {25, 31}, {49, 22}, {18, 69}, {41, 81}, {77, 62}, {36, 25}, {56, 77} },
		{ {26, 31}, {50, 22}, {18, 68}, {42, 81}, {77, 63}, {37, 25}, {57, 77} },
		{ {25, 32}, {49, 23}, {19, 68}, {40, 81}, {77, 64}, {36, 26}, {56, 78} },
		{ {26, 32}, {50, 23}, {20, 68}, {43, 81}, {77, 65}, {37, 26}, {57, 78} }
	};
	int NumTileThreshold = 4;// Large enough to not confuse Hell entry with Crypt
	int EntranceIndex = 0;

	if (abs(PlayerRowPos - 25) <= NumTileThreshold) {// Church Entry in Town
		EntranceIndex = 0;
	} else if (abs(PlayerRowPos - 49) <= NumTileThreshold) {// Catacomb Entry in Town
		EntranceIndex = 1;
	} else if (abs(PlayerRowPos - 18) <= NumTileThreshold) {// Cave Entry in Town
		EntranceIndex = 2;
	} else if (abs(PlayerRowPos - 41) <= NumTileThreshold) {// Hell Entry in Town
		EntranceIndex = 3;
	} else if (abs(PlayerRowPos - 79) <= NumTileThreshold) {// Abyss Entry in Town
		EntranceIndex = 4;
	} else if (abs(PlayerRowPos - 36) <= NumTileThreshold) {// Crypt Entry in Town
		EntranceIndex = 5;
	} else if (abs(PlayerRowPos - 56) <= NumTileThreshold) {// Restart in town
		EntranceIndex = 6;
	}

	PlayerRowPos = Offset[CurrentPlayerIndex][EntranceIndex][0];
	PlayerColPos = Offset[CurrentPlayerIndex][EntranceIndex][1];
}

int __fastcall IsCellTownPortal(int row, int col)// Check if target cell is a TP to avoid sending player right back to where he started.  Missile map check does NOT work because missiles are loaded AFTER player is initialized...
{
//	for (int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++) {
//		int spellIndex = MissileIndexes[spellIndexIndex];
//		Missile& spell = Missiles[spellIndex];
//		if (spell.BaseMissileIndex == MI_10_TOWN_PORTAL && spell.Row == row && spell.Col == col) {
//			return true;
//		}
//	}
	for (int playerIndex = 0; playerIndex < PlayersMax_4; playerIndex++) {
		Player& player = Players[playerIndex];
		if (player.IsExists && Portals[playerIndex].row == row && Portals[playerIndex].col == col) {
			return true;
		}
	}
	return false;
}