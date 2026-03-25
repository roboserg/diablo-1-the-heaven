#include "stdafx.h"

//----- (0041DE9D) -------------------------------------------------------- game
bool ProgressUpdate()
{
	static i64 prevTime = 0;
	i64 curTime = 0;
	HandleSystemMessage();
	ProgressBar += 15;
	if( ProgressBar > MAX_PROGRESS ){
		ProgressBar = MAX_PROGRESS;
	}
	if( LoadingScreenCEL ){
		curTime = GetTickCountTh();
		i64 deltaTime = curTime - prevTime;
		if( deltaTime < 30_ms ){
			SleepTh( 30_ms - deltaTime );
		}
		//DirectDrawPtr->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, 0 );
		ShowOperationProgress();
	}
	prevTime = curTime;
	return ProgressBar >= MAX_PROGRESS;
}

//----- (0041DED6) -------------------------------------------------------- interface
void ShowOperationProgress()
{
	static const ScreenPos progressBarPos[ 3 ] = { { 53, 37 }, { 53, 421 }, { 53, 37 } };
	// пока LoadingScreen 640 на 480 рисуем с фиксированным размером в левом верхнем углу и растягиваем как видео
	int Xpos = Screen_LeftBorder + ((ScreenWidth - GUI_Width) / 2); // WorkingWidth / 2 - GUI_Width / 2
	int Ypos = Screen_TopBorder + ((ScreenHeight - GUI_Height) / 2); // Screen_TopBorder + ScreenHeight / 2 - GUI_Height / 2;
	LockSrcSurface();
	Surface_DrawCEL(Xpos, Ypos + 479, LoadingScreenCEL, 1, GUI_Width);
	uint index = 0;
	if( ProgressBar ){
		do{
			DrawProgressBar(progressBarPos[ProgressBarIndex].x + index++ + Xpos, progressBarPos[ProgressBarIndex].y + Ypos, ProgressBarIndex);
			// попиксельная плавность загрузки (тормозит на высоких разрешениях)
			//if( th2 && index & 1 && index > prevCount ) DrawNotGameplayScreen( 0 );
		}while( index < ProgressBar );
	}
	UnlockSrcSurface();
	//if( !th2 )
		DrawNotGameplayScreen(0);
	//prevCount = MAX_PROGRESS;
}

//----- (0041DF4C) -------------------------------------------------------- interface
void __fastcall DrawProgressBar(int x, int y, int progressBarIndex)
{
	static const uchar progressBarColor[ 3 ] = { 138, 43, 254 };
	uchar* dstSurf = &WorkingSurface[x + YOffsetHashTable[y]];
	for( int i = 0; i < 22; i++ ){
		*dstSurf = progressBarColor[progressBarIndex];
		dstSurf += WorkingWidth;
	}
}

//----- (th3) -------------------------------------------------------------
void ChangeDungeon(LEVEL_DIRECTION lvlDir)
{
	if( lvlDir == LVL_2_SET ){
		QuestVideoOnEnter();
		// May wanna call this func for LVL_5_WARP to set proper quest map return coords via TP.
		// I currently blocked player from taking TP if he has never been to the quest map in MP.
		// This is for quest status and quest log checks purpose.
		// Also, without proper return coords set for TP,
		// player taking quest map (if never been there) TP may end up at (0,0) after leaving quest map via stairs.
		SetReturnDungeonPos(Dungeon.next);
	}
	ProgressUpdate();
	if( MaxCountOfPlayersInGame == 1 ){
		SaveLevel();
	}else{
		DeltaSaveLevel();
	}
	FreeDungeonResources();
	if( lvlDir == LVL_5_WARP && Dungeon == DUN_0_TOWN && CurrentPortal == CurrentPlayerIndex ){
		NetSendCmd( 1, CMD_57_DEACTIVATEPORTAL );
		DeactivatePortal( CurrentPortal );
	}
	if( Dungeon.next == DUN_0_TOWN ){
		DungeonRandSeed[DUN_0_TOWN] = GetRndSeed() * ms(GetTickCountTh()); // TODO: check for sync issues
	}
	Dungeon = Dungeon.next;
	ProgressUpdate();
	LoadGameLevel(0, lvlDir);
	ProgressUpdate();

}

//----- (0041DF75) -------------------------------------------------------- game
void __fastcall ShowProgress(uint msg)
{
	CleanAutoMapGraphics();
	gbSomebodyWonGameKludge = 0;
	PlayerMessageHold(true);

	ProcHandler prevHandler = SetWindowProc(DisableInputWindowProc);
	HandleSystemMessage();
	
	// load progress background and fade in
	ClearScreen();
	DrawNotGameplayScreen(1);
	ProgressLoad(msg);
	BlackPalette();
	ShowOperationProgress();
	PaletteFadeIn(8);

	i64 startTick = GetTickCountTh();

	ProgressUpdate();
	LoadClassSoundEffect();
	ProgressUpdate();

	switch( msg ){
	case WM_1035_LOAD_GAME:
		ProgressUpdate();
		LoadGame(1); // load from main menu
		ProgressUpdate();
		XinputFixCursorPosition();
		CurPerkPanel = PERK_TYPE_COMMON;
		memzero(PerkPanelLine);
		if (!AngelSeq[0]) { //if sequence not regenerated
			GenerateRandomSequenceOfAngels();
		}
		break;

	case WM_1034_NEW_GAME:
		NonGameplaySeed = GetTickCount();
		ProgressUpdate();
		FreeDungeonResources();
		ProgressUpdate();
		DestroyTempSaves();
		LoadGameLevel(1, LVL_0_DOWN);
		if( GameMode == GM_NIGHTMARE || GameMode == GM_SPEEDRUN){
			UberDiabloKilled = false;
			ResetSpeedrunTimer();
			bool oldEnforceNoSave = EnforceNoSave;
			EnforceNoSave = false;
			UpdatePlayerFile();
			EnforceNoSave = oldEnforceNoSave;
		}
		ProgressUpdate();
		XinputFixCursorPosition();
		ItemUpdateTimer = 1;
		ValidateCurPlayer(true);//Force item recalc at the start of the game to prevent books morphing. Does not prevent book structs corruption in the first place though...
		CurPerkPanel = PERK_TYPE_COMMON;
		memzero(PerkPanelLine);
		break;

	case WM_1026_NEXT_DUNGEON             : ChangeDungeon(LVL_0_DOWN   ); break;
	case WM_1027_PREV_DUNGEON             : ChangeDungeon(LVL_1_UP     ); break;
	case WM_1029_GOTO_QUEST_DUNGEON       : ChangeDungeon(LVL_2_SET    ); break;
	case WM_1028_RETURN_FROM_QUEST_DUNGEON: ChangeDungeon(LVL_3_RTN    ); break;
	case WM_1030_TOWN_PORTAL              : ChangeDungeon(LVL_5_WARP   ); break;
	case WM_1031_GOTO_DUNGEON             : ChangeDungeon(LVL_6_TWARPDN); break;
	case WM_1032_RETURN_TO_TOWN           : ChangeDungeon(LVL_7_TWARPUP); break;
	case WM_1033_RESTART_IN_TOWN          : ChangeDungeon(LVL_0_DOWN   ); break;
	}

	while( GetTickCountTh() - startTick < 1000_ms ) SleepTh(187); // показываем loading screen минимум 1 секунду
	PaletteFadeOut(8);
	FreeLoadingScreenCEL();
	SetWindowProc(prevHandler);
	NetSendCmdLocParam1( 1, CMD_53_PLAYER_JOINLEVEL, Players[CurrentPlayerIndex].Row, Players[CurrentPlayerIndex].Col, (DUNGEON)Dungeon );
	PlayerMessageHold(false);
	RestoreScreenSurface();
	if( gbSomebodyWonGameKludge && Dungeon == DUN_16_HELL_4 ){
		PrepDoEnding();
	}
	gbSomebodyWonGameKludge = 0;
}

//----- (0041E2C3) -------------------------------------------------------- interface
void FreeLoadingScreenCEL()
{
	FreeMemZero(LoadingScreenCEL);
}

/*__forceinline*/ void SetLoadingScreenImage(char* imageName, char* paletteName, int progressBarIndex)
{
	LoadingScreenCEL = (char*)LoadFile(imageName);
	LoadPalette(paletteName);
	if( progressBarIndex >= 0 ){
		ProgressBarIndex = progressBarIndex;
	}
	ProgressBar = 0;
}

//----- (0041E2D5) --------------------------------------------------------
void __fastcall ProgressLoad( int msg )// #loading screens
{
	DUNGEON dungeon = Dungeon.next;
	switch( msg ){
	case WM_1026_NEXT_DUNGEON:
		if( dungeon == DUN_1_CHURCH_1 ){ 
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				SetLoadingScreenImage("gendata\\Iceagetowncel.cel", "gendata\\Iceagetownpalette.pal", 1);
			}
			else {
				SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
			}
		}
		//break; // break is not allowed here, else it removes loading screens in dungeons when you go down. no idea why this shit has to be so convoluted but that's how it works
	case WM_1027_PREV_DUNGEON
	  or WM_1031_GOTO_DUNGEON
	  or WM_1032_RETURN_TO_TOWN
	  or WM_1033_RESTART_IN_TOWN:
		  switch (Dungeons[dungeon].graphType) {
		  case DT_0_TOWN:
			  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				  SetLoadingScreenImage("gendata\\Iceagetowncel.cel", "gendata\\Iceagetownpalette.pal", 1);
			  }
			  else {
				  if (Dungeon.prev == DUN_0_TOWN) {
					  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
						  SetLoadingScreenImage("gendata\\Iceagetowncel.cel", "gendata\\Iceagetownpalette.pal", 1);
					  }
					  else {
						  SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
					  }
				  }
				  else {
					  SetLoadingScreenImage("Gendata\\Cutstart.CEL", "Gendata\\Cutstart.pal", 1);
				  }
			  }
			  break;
		  case DT_1_CHURCH:
			  if (Dungeon.prev == DUN_5_CATACOMB_1) {
				  char* paloCats = "Gendata\\Cut2.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  paloCats = "Nlevels\\IceAge_Catas1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut2.CEL", paloCats, 2);
			  }
			  else if (Dungeon.prev == DUN_0_TOWN) {  //  <<<   going down into Cathedral from Town...
				   if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					   SetLoadingScreenImage("gendata\\Iceagetowncel.cel", "gendata\\Iceagetownpalette.pal", 1);
				   }
				   else {
					   if (RNG(2)) {// randomized loading screens, currently 2 variations
						   SetLoadingScreenImage("gendata\\CathLSalt01.cel", "gendata\\CathAltLS01.pal", 1);
					   }
					   else {
						   SetLoadingScreenImage("Gendata\\Cuttt.CEL", "Gendata\\Cuttt.pal", 1);
					   }
				   }
			   }
			  else {
				  SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			  }
			  break;
		  case DT_2_CATACOMB:
			  if (Dungeon.prev == DUN_4_CHURCH_4) {
				  SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0);
			  }
			  else if (Dungeon.prev == DUN_9_CAVE_1) {
				  char* pal3 = "Gendata\\Cut3.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal3 = "Nlevels\\IceAge_Cave1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut3.CEL", pal3, 1);
			  }
			  else {
				  char* paloCats = "Gendata\\Cut2.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  paloCats = "Nlevels\\IceAge_Catas1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut2.CEL", paloCats, 2);
			  }
			  break;
		  case DT_3_CAVE:
			  if (Dungeon.prev == DUN_8_CATACOMB_4) {
				  char* paloCats = "Gendata\\Cut2.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  paloCats = "Nlevels\\IceAge_Catas1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut2.CEL", paloCats, 2);
			  }
			  else if (Dungeon.prev == DUN_13_HELL_1) {
				  char* pal4 = "Gendata\\Cut4.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal4 = "Nlevels\\IceAge_Hell1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut4.CEL", pal4, 1);
			  }
			  else {
				  char* pal3 = "Gendata\\Cut3.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal3 = "Nlevels\\IceAge_Cave1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut3.CEL", pal3, 1);
			  }
			  break;
		  case DT_4_HELL:
			  if (dungeon == DUN_16_HELL_4 && Dungeon.prev == DUN_15_HELL_3) {
				  SetLoadingScreenImage("Gendata\\Cutgate.CEL", "Gendata\\Cutgate.pal", 1);
			  }
			  else if (Dungeon.prev == DUN_12_CAVE_4) {
				  char* pal3 = "Gendata\\Cut3.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal3 = "Nlevels\\IceAge_Cave1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut3.CEL", pal3, 1);
			  }
			  else if (Dungeon.prev == DUN_17_ABYSS_1) {
				  SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			  }
			  else {
				  char* pal4 = "Gendata\\Cut4.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal4 = "Nlevels\\IceAge_Hell1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut4.CEL", pal4, 1);
			  }
			  break;
		  case DT_6_ABYSS:
			  if (Dungeon.prev == DUN_21_CRYPT_1) {
				  char* pal6 = "Nlevels\\cutl5.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal6 = "Nlevels\\IceAge_Crypt1.pal";
				  }
				  SetLoadingScreenImage("Nlevels\\cutl5.CEL", pal6, 1);
			  }
			  else if (Dungeon.prev == DUN_16_HELL_4) {
				  char* pal4 = "Gendata\\Cut4.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal4 = "Nlevels\\IceAge_Hell1.pal";
				  }
				  SetLoadingScreenImage("Gendata\\Cut4.CEL", pal4, 1);
			  }
			  else {
				  SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			  }
			  break;
		  case DT_5_CRYPT:
			  if (Dungeon.prev == DUN_20_ABYSS_4) {
				  SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			  }
			  else {
				  char* pal6 = "Nlevels\\cutl5.pal";
				  if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					  pal6 = "Nlevels\\IceAge_Crypt1.pal";
				  }
				  SetLoadingScreenImage("Nlevels\\cutl5.CEL", pal6, 1);
			  }
			  break;
		  default:
			  SetLoadingScreenImage("Gendata\\Cutl1d.CEL", "Gendata\\Cutl1d.pal", 0); 
			  break;
		  }
		  break;
	case WM_1028_RETURN_FROM_QUEST_DUNGEON:
		if (dungeon == DUN_19_ABYSS_3) {
			char* pal3 = "Gendata\\Cut3.pal";
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				pal3 = "Nlevels\\IceAge_Cave1.pal";
			}
			SetLoadingScreenImage("Gendata\\Cut3.CEL", pal3, 1); 
			break;
		}
		else {
			dungeon = Dungeon.prev;
		}
	case WM_1029_GOTO_QUEST_DUNGEON:
		switch( dungeon ){
		case DUN_54_ABANDONED_FANE:
		{
			char* pal_xz = "Nlevels\\cutl5.pal";
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				pal_xz = "Nlevels\\IceAge_Crypt1.pal";
			}
			SetLoadingScreenImage("Nlevels\\cutl5.CEL", pal_xz, 1);
		}
			break;
		case DUN_53_VAULTKEEP or DUN_58_GATEWAY_OF_BLOOD or DUN_59_HALL_OF_FIRE 
		or DUN_66_SEWERS or DUN_67_SEPULCHER or DUN_68_OUBLIETTE or DUN_69_GIBBET 
		or DUN_70_CITADEL or DUN_71_GRIM_VAULTS or DUN_72_NECROPOLIS or DUN_109_HALL_OF_HEROES:
		{
			char* paloCats = "Gendata\\Cut2.pal";
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				paloCats = "Nlevels\\IceAge_Catas1.pal";
			}
			SetLoadingScreenImage("Gendata\\Cut2.CEL", paloCats, 2);
		}
			break;
		case DUN_51_FETID_CAVERN or DUN_52_MOSSY_GROTTO or DUN_112_STILL_SPRINGS or DUN_73_LAVA_PITS or DUN_74_CRATER or DUN_75_RIVER_OF_FIRE
		or DUN_76_DEN_OF_EVIL or DUN_77_VILE_TUNNELS or DUN_78_THE_BRIDGE or DUN_79_COLONNADE or DUN_80_HELL_GATE:// better upgrade grotto
		{
			if(Dungeon.prev == DUN_81_UNDERWORLD){ 
				SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1); 
			}
			else {
				char* pal3 = "Gendata\\Cut3.pal";
				if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
					pal3 = "Nlevels\\IceAge_Cave1.pal";
				}
				SetLoadingScreenImage("Gendata\\Cut3.CEL", pal3, 1);
			}
		}
			break;
		case DUN_50_TEMPLE_OF_SUNSET: 
		{
			char* pal14 = "Nlevels\\cutl5.pal";
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				pal14 = "Nlevels\\IceAge_Crypt1.pal";
			}
			SetLoadingScreenImage("Nlevels\\cutl5.CEL", pal14, 1); 
		}
			break;
		case DUN_44_RAVENHOLM:           SetLoadingScreenImage("Gendata\\raven.cel", "Gendata\\raven.pal", 1); 
			break;
		case DUN_45_THE_DEN:             SetLoadingScreenImage("Gendata\\cerb.cel",  "Gendata\\Cerb.pal",  1); 
			break;
		case DUN_27_THE_INFESTED_CELLAR or DUN_49_SICK_VILLAGERS_BASEMENT: SetLoadingScreenImage("Gendata\\ING.CEL",   "Gendata\\INF.pal",   1); 
			break;
		case DUN_31_ANDARIELS_THRONE:    
			if (RNG(100) < 80 ) {
				SetLoadingScreenImage("Gendata\\AndZ.CEL", "Gendata\\AndZ.pal", 2);
			}
			else {
				SetLoadingScreenImage("Gendata\\AndX.CEL", "Gendata\\AndX.pal", 2);
			}
			break;
		case DUN_30_HALLS_OF_ANGUISH:    SetLoadingScreenImage("Gendata\\Cut4.CEL",  "Gendata\\Cut4.pal",  1); 
			break;
		case DUN_81_UNDERWORLD or DUN_82_HALLS_OF_SUFFERING or DUN_83_BLOOD_KEEP or DUN_84_ARMORIES_OF_HELL or DUN_85_DEMON_STRONGHOLD
			or DUN_86_VALLEY_OF_WRATH or DUN_87_PANDEMONIUM or DUN_88_REALM_OF_TERROR or DUN_89_BEDLAM:
		{
			if(Dungeon.prev == DUN_80_HELL_GATE){ 
				SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1); 
			}
			else if (Dungeon.prev == DUN_90_FIELDS_OF_MADNESS) {
				SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			}
			else{ 
				SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			}
		}
			break;
		case DUN_90_FIELDS_OF_MADNESS or DUN_91_STYGIAN_PASSAGE or DUN_92_FLESH_PIT or DUN_93_BLEEDING_VEINS_PATH or DUN_94_ROT_CHAMBERS
		or DUN_95_MARROW_GARDEN or DUN_96_BONE_MIRE or DUN_97_GLOOMY_DEPTHS or DUN_98_EBON_CHASM:
		{
			if (Dungeon.prev == DUN_89_BEDLAM) {
				SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			}
			else if (Dungeon.prev == DUN_99_ANCIENT_CRYPTS) {
				SetLoadingScreenImage("Nlevels\\cutl5.CEL", "Nlevels\\cutl5.pal", 1);
			}
			else {
				SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			}
		}
			break; 
		case DUN_99_ANCIENT_CRYPTS or DUN_100_HORADRIM_TREASURY or DUN_101_VOID_CHAMBERS or DUN_102_OBLIVION_TRAIL or DUN_103_ALTAR_OF_IMMOLATION
		or DUN_104_NETHERWORLD_GATES or DUN_105_DARK_DESCENT or DUN_106_BASTION_OF_NIGHT or DUN_107_DREAD_SANCTUARY or DUN_108_ARCANE_PRISON:
		{
			if (Dungeon.prev == DUN_98_EBON_CHASM) {
				SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			}
			else {
				SetLoadingScreenImage("Nlevels\\cutl5.CEL", "Nlevels\\cutl5.pal", 1);
			}
		}
			break;
		case DUN_32_HORAZONS_HAVEN
		  or DUN_33_SUMMONING_ROOMS
		  or DUN_34_THE_PIT:           //SetLoadingScreenImage("Gendata\\Horaz.cel",    "Gendata\\Cut2.pal", 1);
			                             SetLoadingScreenImage("Gendata\\Whorazon.cel", "Gendata\\Whorazon.pal", 1); 
										 break;
		case DUN_35_PASSAGE_OF_FIRE:
			//if( Dungeon.prev == DUN_36_VALLEY_OF_DESPAIR ){
			//	SetLoadingScreenImage("Gendata\\Cut3.CEL",  "Gendata\\Cut3.pal",  1); 
			//}else{
				SetLoadingScreenImage("gendata\\Passage1.CEL", "gendata\\passage1.pal", 1); 
				//SetLoadingScreenImage("Nlevels\\Cutl6.CEL", "Nlevels\\Cutl6.pal", 1);
			//}
			break;
		case DUN_47_DARK_NEST or DUN_48_BLOOD_HIVE or DUN_55_DEEP_HOLLOW or DUN_56_WEB_TUNNELS or DUN_57_ARACHNID_LAIR:
			SetLoadingScreenImage("Nlevels\\cutl6.CEL", "Nlevels\\cutl6.pal", 1);
			break;
		case DUN_36_VALLEY_OF_DESPAIR:
			if( Dungeon.prev != DUN_0_TOWN ){
				SetLoadingScreenImage("gendata\\izual.cel",    "Gendata\\Cut2.pal",     1);
			}else{
				SetLoadingScreenImage("Gendata\\Cutportr.CEL", "Gendata\\Cutportr.pal", 1);
			}
			break;
		case DUN_37_DISUSED_RELIQUARY: 
			if (Dungeon.prev == DUN_0_TOWN) {
				SetLoadingScreenImage("gendata\\Reliquary.cel", "gendata\\Reliquary.pal", 1);
			}
			else if (Dungeon.prev == DUN_38_GLACIAL_CAVERNS) {
				SetLoadingScreenImage("gendata\\glacial2.cel", "gendata\\glacial02logo1.pal", 1);
			}
			else {
				SetLoadingScreenImage("Gendata\\Reliquary2Town.CEL", "Gendata\\Reliquary2town.pal", 1);
			}
			break;
		case DUN_38_GLACIAL_CAVERNS
		  or DUN_39_FROZEN_LOCH:       SetLoadingScreenImage("gendata\\glacial2.cel", "gendata\\glacial02logo1.pal", 1); 
			break;
		case DUN_40_ISLAND:            SetLoadingScreenImage("Gendata\\Cutportl.CEL", "gendata\\cutportl.pal",      -1); 
			break; // TODO: check why -1
		case DUN_41_FLESHDOOM
		  or DUN_42_WIELDER
		  or DUN_43_HEPHASTO:
			if( msg == WM_1028_RETURN_FROM_QUEST_DUNGEON ){
				SetLoadingScreenImage( "Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1 );
			}else{
				int fleshdoomIndex = Dungeon.next - DUN_41_FLESHDOOM + 1; // TODO: continue here, check
				sprintf( InfoPanelBuffer, "Gendata\\Fleshdoom%d.cel", fleshdoomIndex );
				sprintf( InfoPanelHeader, "Levels\\l4data\\Fleshdoomlogo%d.pal", fleshdoomIndex );
				SetLoadingScreenImage( InfoPanelBuffer, InfoPanelHeader, 1);
			}
			break;
		case DUN_113_THE_MAZE:            SetLoadingScreenImage("Gendata\\Cut4.CEL", "Gendata\\Cut4.pal", 1);
			break;
		case DUN_26_BONE_CHAMBER:            SetLoadingScreenImage("Gendata\\Chamber1.CEL", "Gendata\\Chamber1.pal", 2);
			break;
		case DUN_28_POISONED_WATER_SUPPLY:   
			if (Dungeon.prev == DUN_2_CHURCH_2) {
				SetLoadingScreenImage("Gendata\\pws.CEL", "Gendata\\pws.pal", 1);
			}
			else {
				SetLoadingScreenImage("Gendata\\Cut3.CEL", "Gendata\\Cut3.pal", 1); 
			}
			break;
		case DUN_25_KING_LEORICS_TOMB:       SetLoadingScreenImage("Gendata\\KLT.CEL",      "Gendata\\KLT.pal",      1); 
			break;
		case DUN_46_COLISEUM:
		case DUN_29_ARCHBISHOP_LAZARUS_LAIR: SetLoadingScreenImage("Gendata\\Cutportr.CEL", "Gendata\\Cutportr.pal", 1); 
			break;
		default:                             SetLoadingScreenImage("Gendata\\Cutl1d.CEL",   "Gendata\\Cutl1d.pal",   0); 
			break;
		}
		break;
	case WM_1030_TOWN_PORTAL:
		if (GameMode == GM_CLASSIC || BlueTownPortal || Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
			SetLoadingScreenImage("Gendata\\Cutportl.CEL", "gendata\\cutportl.pal", 1);
		}
		else{
			SetLoadingScreenImage("Gendata\\Cutportl.CEL", "Gendata\\TownPrtl.pal", 1);
		}
		break;
	case WM_1034_NEW_GAME
	  or WM_1035_LOAD_GAME: SetLoadingScreenImage("Gendata\\Cutstart.CEL", "Gendata\\Cutstart.pal", 1); 
		break;
	default: TerminateWithError( "Unknown progress mode" ); 
		break;
	}
}
