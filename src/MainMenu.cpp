#include "stdafx.h"

int MenuMusicIndex = 0;

char* MainMenuMusic[] = {
	//"Music\\Dintr1.wav", // reserved for Andariel quest level 1
	"Music\\ambience_gru.wav",
    "Music\\ambience_gru_v3.wav",
	//"Music\\Dintr3.wav",
	"Music\\Dintr4.wav",
    "Music\\ambience_mm_moody.wav",
};

char* MainMenuOrigMusic[] = {
	"Music\\or\\dintro.wav",
};

char* MainMenuDoomMusic[] = {
    "Music\\doom\\0_menu.wav",
    "Music\\doom\\0_1_menu.wav",
    "Music\\doom\\0_2_menu.wav",
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
        //LoadDragonMenuPalette(); // уже не нужны, так как загружаются из оригинальной таблицы storm.dll
        if( LoadOnStart ){ // сразу загружать заданную запись для быстрой сверки
			dialogResult = LoadMulti ? MM_2_MULTI_PLAYER : MM_1_SINGLE_PLAYER;
		}else{
			// устанавливаем курсор по центру экрана при старте главного меню на весь экран
			// из-за временного сдвига мета-меню при отрисовке (до замены меню с нуля или допиливания storm.dll)
			if( IsFullScreen ){
				SetCursorPos( GUI_Width / 2 * WinWidth / ScreenWidth, GUI_Height / 2 * WinHeight / ScreenHeight);
			}
			//EnableWindow( global_hWnd, TRUE );
			MainMainMenu = true;
			if( OriginalMenu && !UiMainMenuDialog(TheHeavenVersion, &dialogResult, SomeBoolFlag_17, PlaySoundFile, 30) ){
				TerminateWithError("Unable to display mainmenu");
			}
			MainMainMenu = false;
			//EnableWindow( global_hWnd, FALSE );
		}
        //LoadMainMenuPalette(); // уже не нужны, так как загружаются из оригинальной таблицы storm.dll
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
			if( LoadOnStart ){ // сразу загружать заданную запись для быстрой сверки
				LoadCharSlots(nullptr);
				singlePlayerSelection = NewGameOnStart ? HM_1_NEW_GAME : HM_2_LOAD_GAME;
				strcpy( HeroName, CharNameSlots[ AutoLoadSlot ] ); // где то в hell.dll определяется номер текущей записи
			}else{
				if( !UiSelHeroSingDialog(LoadCharSlots, UiCreateHero, DeleteCharSlot,	GetStartingStatsByClass, &singlePlayerSelection, HeroName, &Difficulty, Enable5Class, Enable6Class) ){
					TerminateWithError("Unable to display SelHeroSing");
				}
				if( singlePlayerSelection == HM_4_CANCEL ){
					return 1;
				}
			}
        }else{ // код для использования без оригинального мета меню
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
        StopMusic();
        gbRunGameResult = StartGame(heroMenuSelection != HM_2_LOAD_GAME, heroMenuSelection != HM_3_NETWORK_GAME );
        if( gbRunGameResult ){
            StartMenuMusic();
        }
        result = gbRunGameResult;
    }
    //LoadMainMenuPalette(); // уже не нужны, так как загружаются из оригинальной таблицы storm.dll
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
		// надо добавить авто создание сетевой игры заданного сейва
		// через меню th в разных потоках не может создать одновременно два подменю выбора игрока, не реентарабельно
		if( LoadOnStart ){ // сразу загружать заданную запись для быстрой сверки
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
