#include "stdafx.h"

int MenuMusicIndex = 0;

char* MainMenuMusic[] = {
	//"Music\\Dintr1.wav", // reserved for Andariel quest level 1
	"Music\\ambience_gru.ogg",
    "Music\\ambience_gru_v3.ogg",
	//"Music\\Dintr3.wav",
	"Music\\Dintr4.ogg",
    "Music\\ambience_mm_moody.ogg",
};

char* MainMenuOrigMusic[] = {
	"Music\\or\\dintro.ogg",
};

char* MainMenuDoomMusic[] = {
    "Music\\doom\\0_menu.ogg",
    "Music\\doom\\0_1_menu.ogg",
    "Music\\doom\\0_2_menu.ogg",
};

//----- (0042E2ED) --------------------------------------------------------
void StartMenuMusic()
{
   srand(GetTickCount());
   StopAmbient();
   if (Doom_Music) {
       PlayMusic(MainMenuDoomMusic[RandSound(countof(MainMenuDoomMusic))]);
   }
   else if (OriginalMusic) {
       PlayMusic(MainMenuOrigMusic[RandSound(countof(MainMenuOrigMusic))]);
   }
   else {
       PlayMusic(MainMenuMusic[RandSound(countof(MainMenuMusic))]);
   }
}

bool MainMainMenu = false;

//----- (0042E314) --------------------------------------------------------
void DisplayMainMenu()
{
    int v0;  // esi@1
    char v1; // zf@9
    int v2;  // eax@14
    int dialogResult;  // [sp+4h] [bp-4h]@2
    StartMenuMusic();
    v0 = 0;
    do{
        dialogResult = 0;
        //(rt) LoadDragonMenuPalette(); // no longer needed, since they are loaded from the original storm.dll table
        if( LoadOnStart ){ // (rt) load the specified save immediately for a quick comparison
			dialogResult = LoadMulti ? MM_2_MULTI_PLAYER : MM_1_SINGLE_PLAYER;
		}else{
			// (rt) place the cursor in the centre of the screen when the fullscreen main menu starts
			// (rt) because of the temporary meta-menu offset when drawing (until the menu is rewritten from scratch or storm.dll is finished)
			if( IsFullScreen ){
				SetCursorPos( GUI_Width / 2 * WinWidth / ScreenWidth, GUI_Height / 2 * WinHeight / ScreenHeight);
			}
			//EnableWindow( global_hWnd, TRUE );
			MainMainMenu = true;
			if( OriginalMenu && !UiMainMenuDialog(TheHellVersion, &dialogResult, SomeBoolFlag_17, PlaySoundFile, 30) ){
				TerminateWithError("Unable to display mainmenu");
			}
			MainMainMenu = false;
			//EnableWindow( global_hWnd, FALSE );
		}
        //(rt) LoadMainMenuPalette(); // no longer needed, since they are loaded from the original storm.dll table
        if( dialogResult == MM_1_SINGLE_PLAYER || !OriginalMenu ){
			v2 = SinglePlayerGame();
            goto LABEL_16;
        }
        if( dialogResult == MM_2_MULTI_PLAYER ){
            v2 = MultiPlayerGame();
            LABEL_16:
            v1 = v2 == 0;
            LABEL_17:
            if( v1 )
                v0 = 1;
            continue;
        }
        if( dialogResult != MM_3_PLAY_INTRO ){
            if( dialogResult == MM_4_SUPPORT ){
                UiSupportDialog(16);
                continue;
            }
            if( dialogResult == MM_5_CREDITS ){
                UiCreditsDialog(16);
                continue;
            }
            v1 = dialogResult == MM_6_EXIT;
            goto LABEL_17;
        }
        if( IsMainWindowActive )
            ShowVideo();
    }while( !v0 );
    StopMusic();
}

//----- (0042E3A4) --------------------------------------------------------
int SinglePlayerGame()
{
    int singlePlayerSelection;
    do{
        singlePlayerSelection = 0;
        MaxCountOfPlayersInGame = 1;
		#ifndef NO_FASTWALK
		if( !SRegLoadValue("Hellfire", "Fast Walk", 0, &IsFastWalkEnabled) ){
            IsFastWalkEnabled = 1;
        }
		#endif
		if( OriginalMenu ){
			if( LoadOnStart ){ // (rt) load the specified save immediately for a quick comparison
				LoadCharSlots(nullptr);
				singlePlayerSelection = NewGameOnStart ? HM_1_NEW_GAME : HM_2_LOAD_GAME;
				strcpy( HeroName, CharNameSlots[ AutoLoadSlot ] ); // (rt) the current save number is determined somewhere in hell.dll
			}else{
				if( !UiSelHeroSingDialog(LoadCharSlots, UiCreateHero, DeleteCharSlot,	GetStartingStatsByClass, &singlePlayerSelection, HeroName, &Difficulty, Enable5Class, Enable6Class) ){
					TerminateWithError("Unable to display SelHeroSing");
				}
				if( singlePlayerSelection == HM_4_CANCEL ){
					return 1;
				}
			}
        }else{ // (rt) code for use without the original meta menu
            LoadCharSlots(nullptr);
            ShowCursor(1);
            singlePlayerSelection = DialogBox(HInstance, MAKEINTRESOURCE(IDD_SINGLEPLAYER), ghMainWnd, SinglePlayerDialogProc);
            if( singlePlayerSelection == HM_4_CANCEL ){
                return 0;
			}
            ShowCursor(0);
        } // #endif
    }while( DoMenu(singlePlayerSelection) );
    return 0;
}

//----- (0042E439) --------------------------------------------------------
int __fastcall DoMenu(int heroMenuSelection)
{
    int result; // eax@2
    if( heroMenuSelection == HM_4_CANCEL ){
        result = 1;
    }else{
        StopMusic();	// (th4) meta menu <-> game switches music at once, no crossfade (SoundBuffer.cpp MUSIC_FADE_MS)
        gbRunGameResult = StartGame(heroMenuSelection != HM_2_LOAD_GAME, heroMenuSelection != HM_3_NETWORK_GAME );
        if( gbRunGameResult ){
            StartMenuMusic();
        }
        result = gbRunGameResult;
    }
    //(rt) LoadMainMenuPalette(); // no longer needed, since they are loaded from the original storm.dll table
    return result;
}

//----- (0042E46F) --------------------------------------------------------
int MultiPlayerGame()
{
    int multiPlayerSelection; // [sp+0h] [bp-4h]@1
    do{
        multiPlayerSelection = 0;
		if( !SRegLoadValue( "Hellfire", "Fast Walk", 0, &IsFastWalkEnabled ) ){
			IsFastWalkEnabled = 1;
		}
		MaxCountOfPlayersInGame = 4;
		// (rt) auto-creation of a network game for a given save should be added
		// (rt) via the th menu, two player-selection submenus can't be created at once in different threads; not reentrant
		if( LoadOnStart ){ // (rt) load the specified save immediately for a quick comparison
			LoadCharSlots(nullptr);
			multiPlayerSelection = HM_3_NETWORK_GAME;
			strcpy( HeroName, CharNameSlots[ AutoLoadSlot ] );
		}else{
			if( !UiSelHeroMultDialog(LoadCharSlots, UiCreateHero, DeleteCharSlot, GetStartingStatsByClass, &multiPlayerSelection, HeroName, Enable5Class, Enable6Class) )
				TerminateWithError("Can't load multiplayer dialog");
			if( multiPlayerSelection == 4 )
				return 1;
		}
    }while( DoMenu( multiPlayerSelection ) );
    return 0;
}

//----- (0042E4DE) --------------------------------------------------------
void ShowVideo()
{
    StopMusic();
	char* vid[] = { "gendata\\diablo1.smk", "gendata\\doom.smk", "gendata\\fbutch3.smk" };
	PlayVideo( vid[RNG(countof(vid))], 1 );
    StartMenuMusic();
}
