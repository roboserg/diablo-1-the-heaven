#include "stdafx.h"

int Difficulty;
int SelectedDifficulty;

struct {
	const char* name;
	const char* desc1;
	const char* desc2;
} DifficultyText[DL_COUNT][2] = {
	{ { "Horror", "\nThis is where a starting character should begin the quest to defeat the Lord of Terror.", "\n\nThis is recommended for starting characters." },
	  { "Normal", "\nThis is where a starting character should begin the quest to defeat the Lord of Terror.", "\n\nThis is recommended for starting characters." } }, // classic
	{ { "Purgatory", "\nThe denizens of the Labyrinth have been bolstered and will prove to be a greater challenge. ", "\n\nRecommended for characters that completed previous difficulty. Level 20+" },
	  { "Nightmare", "\nThe denizens of the Labyrinth have been bolstered and will prove to be a greater challenge. ", "\n\nRecommended for characters that completed previous difficulty. Level 20+" } }, // classic
	{ { "Doom", "\nMonsters become tougher, stronger and faster. Welcome to your doom.", "\n\nRecommended for characters that completed previous difficulty. Level 35+"},
	  { "Hell", "\nThe most powerful of the Underworld's creatures lurk at the gateway into Hell.", "\n\nRecommended for characters that completed previous difficulty. Level 30+"} }, // classic
	{ { "Crucible", "\nEvil grows stronger than ever. Magical and mysterious creatures of Hell await inside the Labyrinth.", "\n\nRecommended for characters that completed previous difficulty. Level 43+"}, {"","",""} },
	{ { "Torment", "\nGreater riches await ahead, but peril is around every corner.", "\n\nRecommended for characters that completed previous difficulty. Level 51+"}, {"","",""} },
	{ { "Agony", "\nPain and suffering is what awaits you from now on. Embrace them, there will be plenty.", "\n\nRecommended for characters that completed previous difficulty. Level 59+"}, {"","",""} },
	{ { "Terror", "\nYou are about to transcend into the world of madness. True Hell begins here.", "\n\nRecommended for characters that completed previous difficulty. Level 67+"}, {"","",""} },
	{ { "Destruction", "\nThe most powerful of the underworld's creatures lurk at the gateway into the ancient Crypt.", "\n\nRecommended for characters that completed previous difficulty. Level 75+"}, {"","",""} },
	{ { "Inferno", "\nThe army of darkness at its ultimate power. Blood, fire and ecstasy. Demons' ecstasy, not yours.", "\n\nRecommended for characters that completed previous difficulty. Level 83+"}, {"","",""} },
};

// variable win list for hellfrui universal list funcs, TODO: check support for non-continues ids
int DifButtonList[ DL_COUNT + 1 ] = {
	ID_SELECT_DIF_1,
	ID_SELECT_DIF_2,
	ID_SELECT_DIF_3,
	ID_SELECT_DIF_4,
	ID_SELECT_DIF_5,
	ID_SELECT_DIF_6,
	0
};

int DifficultyTitleList[ 3 ] = { ID_CAPTION, ID_SUBCAPTION, 0 };
int DifficultyCtextList[ 2 ] = { ID_SOME_LABEL, 0 };
int DifficultyLtextList[ 2 ] = { ID_DIFFICULTY_DESCRIPTION_2, 0 };
int DifficultyOkCancelList[ 3 ] = { ID_OK_BUTTON, ID_CANCEL_BUTTON, 0 };

//----- (2000D140) --------------------------------------------------------
void __fastcall SetSelectedDifficulty( int difficulty )
{
	SelectedDifficulty = difficulty;
}

//----- (th3) -------------------------------------------------------------
const char* DifName(int dif /*= -1*/)
{
	if( dif == -1 ) dif = Difficulty;
	static char name[DL_COUNT][32] = {0};
	if( ! *name[dif] ){ sprintf(name[dif], "%d. %s", dif + 1, DifficultyText[dif][GameMode == GM_CLASSIC].name); }
	return name[dif];
}

//----- (th3) -------------------------------------------------------------
const char* DifDesc1(int dif /*= -1*/)
{
	if( dif == -1 ) dif = Difficulty;
	static char desc1[DL_COUNT][128] = {0};
	auto& difText = DifficultyText[dif][GameMode == GM_CLASSIC];
	if( ! *desc1[dif] ){ sprintf(desc1[dif], "%s difficulty\n%s", difText.name, difText.desc1); }
	return desc1[dif];
}

//----- (th3) -------------------------------------------------------------
const char* DifDesc2(int dif /*= -1*/)
{
	if( dif == -1 ) dif = Difficulty;
	static char desc2[DL_COUNT][256] = {0};
	auto& difText = DifficultyText[dif][GameMode == GM_CLASSIC];
	if( ! *desc2[dif] ){ sprintf(desc2[dif], "%s difficulty\n%s%s", difText.name, difText.desc1, difText.desc2); }
	return desc2[dif];
}

//----- (th3) -------------------------------------------------------------
void DifChange( HWND hdlg, HWND button, int shift )
{
	for( int i = 0; i < MaxScrollCount_6; ++i ){
		button = GetDlgItem( hdlg, ID_SELECT_DIF_1 + (GetWindowLongA( button, GWL_ID ) - ID_SELECT_DIF_1 + shift + MaxScrollCount_6) % MaxScrollCount_6 );
		if( IsWindowEnabled( button ) ){
			SetFocus( button );
			break;
		}
	}
}

//----- (th3) -------------------------------------------------------------
int DifCount()
{
	int dlCount = GameMode == GM_CLASSIC ? 3 : DL_COUNT;
	#if CHEATS
	return dlCount;
	#else
	CharSaveInfo* curCharSaveInfo = &NewSaveInfo;
	CharSaveInfo multiCharInfo;
	if( CurCharSaveInfo ){
		curCharSaveInfo = CurCharSaveInfo;
	}else if( UserInfo_1 && UserInfo_1->playerDescription ){
		GetCharSaveInfo( UserInfo_1->playerDescription, &multiCharInfo );
		curCharSaveInfo = &multiCharInfo;
	}
	if( IsMultiplayer() ){
		return dlCount;
	}else{
		return std::min(1 + curCharSaveInfo->allowedDifficulty, (int)dlCount);
	}
	#endif
}

//----- (th3) -------------------------------------------------------------
void UpdateAllowedDifficulty()
{
	Player& player = Players[CurrentPlayerIndex];
	if( player.allowedDifficulty < Difficulty + 1 ){
		player.allowedDifficulty = Difficulty + 1;

		// load saved player info
		uint slotIndex = GetSlotByHeroName(player.playerName);
		HANDLE archiveHandle = OpenArchive(false, slotIndex);
		if( !archiveHandle ){
			TerminateWithError("Unable to open archive");
		}
		uchar* heroData = ReadPlayerInfo(archiveHandle);
		if( !heroData ){
			TerminateWithError("Unable to load character");
		}
		LoadPlayerInfo(heroData, PlayersMax_4, false);
		FreeMemZero(heroData);
		CloseArchiveRead(archiveHandle);

		// write updated allowed difficulty to save
		BackupSaveIfNeed(slotIndex);
		LastPlayerInfo playerInfo;
		if( OpenArchiveWrite(true, slotIndex) ){
			SavePlayerInfo(&playerInfo, PlayersMax_4);
			playerInfo.allowedDifficulty = player.allowedDifficulty; // change only allowed difficulty
			SaveCharacter(&playerInfo);
			CloseArchiveWrite(MaxCountOfPlayersInGame == 1, slotIndex);
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall ResetDifButtons( HWND hdlg, int topDif )
{
	for( int i = 0; DifButtonList[i] != 0; ++i ){
		if( HWND difButton = GetDlgItem( hdlg, DifButtonList[i] ); difButton ){
			int dif = topDif + i;
			if( dif < DifCount() ){
				EnableWindow( difButton, true );
				if( DifWin* difWin = (DifWin*) GetWindowLongA( difButton, GWL_USERDATA ); difWin ){
					WriteTextToElemData( (TextWin *) difWin, DifName(dif) );
					difWin->val = dif;
				}
			}else{
				EnableWindow( difButton, false );
			}
		}
	}
	ResetButtonText( hdlg, DifButtonList, 2, 1 );
}

#if 0
//----- (th4) -------------------------------------------------------------
void __fastcall ResetDifClassicButtons(HWND hdlg, int topDif)
{
	for (int i = 0; DifClassicButtonList[i] != 0; ++i) {
		if (HWND difButton = GetDlgItem(hdlg, DifClassicButtonList[i]); difButton) {
			int dif = topDif + i;
			if (dif < DifCount()) {
				EnableWindow(difButton, true);
				if (DifWin* difWin = (DifWin*)GetWindowLongA(difButton, GWL_USERDATA); difWin) {
					WriteTextToElemData((TextWin*)difWin, DifClassicName(dif));
					difWin->val = dif;
				}
			}
			else {
				EnableWindow(difButton, false);
			}
		}
	}
	ResetButtonText(hdlg, DifClassicButtonList, 2, 1);
}
#endif

//----- (th3) ------------------------------------------------------------
void __fastcall SetDifInfoText( HWND hdlg, int dif )
{
	WriteTextToElemData( (TextWin *) GetWindowLongA( GetDlgItem( hdlg, ID_SOME_LABEL ), GWL_USERDATA ), DifficultyText[dif][GameMode == GM_CLASSIC].name );
	DrawTextToElemList( hdlg, DifficultyCtextList, 5 );
	WriteTextToElemData( (TextWin *) GetWindowLongA( GetDlgItem( hdlg, ID_DIFFICULTY_DESCRIPTION_2 ), GWL_USERDATA ), DifDesc2(dif) );
	DrawTextToElemList( hdlg, DifficultyLtextList, 1 );
}


//----- (th3) -------------------------------------------------------------
void DifRefreshSelection( HWND hdlg )
{
	SelectedDifficulty = DifWin::Val(GetFocus());
	SetDifInfoText( hdlg, SelectedDifficulty );
}

//----- (th3) -------------------------------------------------------------
void DifRefreshScroll( const HWND hdlg )
{
	SetScrollOnElem( hdlg, ID_SCROLL, DifCount(), DifWin::Val( GetFocus() ) );
}

//----- (th3) -------------------------------------------------------------
void __fastcall DifPageDown( HWND button )
{
	HWND hdlg = GetParent( button );
	HWND topBut = GetDlgItem( hdlg, ID_SELECT_DIF_1 );
	int dif = DifWin::Val( topBut ) + MaxScrollCount_6;
	if( dif < DifCount() ){
		LimitToMax( dif, DifCount() - MaxScrollCount_6 );
		PlaySoundTitleMove();
		ResetDifButtons( hdlg, dif ); // scroll selection down
		DifRefreshSelection( hdlg );
		DifRefreshScroll( hdlg );
	}else{
		DifChange( hdlg, topBut, -1 ); // select bottom available (wrap above top)
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall DifPageUp( HWND button )
{ 
	HWND hdlg = GetParent( button );
	HWND topBut = GetDlgItem( hdlg, ID_SELECT_DIF_1 );
	int dif = DifWin::Val(topBut);
	if( dif > 0 ){
		dif -= MaxScrollCount_6;
		LimitToMin( dif, 0 );
		PlaySoundTitleMove();
		ResetDifButtons( hdlg, dif ); // scroll selection up
		DifRefreshSelection( hdlg );
		DifRefreshScroll( hdlg );
	}else{
		DifChange( hdlg, topBut, 0 );  // select top
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall DifDown( HWND button )
{
	HWND hdlg = GetParent( button );
	int dif = DifWin::Val( button ) + 1;
	if( dif >= DifCount() ) return;
	if( GetWindowLongA( button, GWL_ID ) < ID_SELECT_DIF_1 + MaxScrollCount_6 - 1 ){
		DifChange( hdlg, button, 1 ); // move selection down
	}else{
		PlaySoundTitleMove();
		ResetDifButtons( hdlg, dif - MaxScrollCount_6 + 1); // scroll selection down
		DifRefreshSelection( hdlg );
		DifRefreshScroll( hdlg );
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall DifUp( HWND button )
{
	HWND hdlg = GetParent( button );
	int dif = DifWin::Val( button ) - 1;
	if( GetWindowLongA( button, GWL_ID ) > ID_SELECT_DIF_1 ){
		DifChange( hdlg, button, -1 ); // move selection up
	}else if( dif >= 0 ){
		PlaySoundTitleMove();
		ResetDifButtons( hdlg, dif ); // scroll selection up
		DifRefreshSelection( hdlg );
		DifRefreshScroll( hdlg );
	}
}

//----- (th3) -------------------------------------------------------------
LRESULT __stdcall DifSelectButtonProc( const HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = 0;
	auto oldProc = (WNDPROC)GetPropA( button, "UIOLDPROC" );
	const auto hdlg = GetParent( button );

	switch( Msg ){
	case WM_GETDLGCODE: result = 4; break;
	case WM_KEYFIRST:
		switch( wParam ){
		case VK_RETURN or VK_SPACE: SendMessageA( hdlg, 273, 1, 0 ); break;
		case VK_ESCAPE: SendMessageA( hdlg, 273, 2, 0 ); break;
		case VK_TAB: DifChange( hdlg, button, ( GetKeyState( VK_SHIFT ) >= 0 ) ? 1 : -1 ); break;
		case VK_LEFT or VK_UP: DifUp( button ); break;
		case VK_RIGHT or VK_DOWN: DifDown( button ); break;
		case VK_PRIOR: DifPageUp( button ); break;
		case VK_NEXT: DifPageDown( button ); break;
		case VK_DELETE: SendMessageA( hdlg, Msg, wParam, lParam ); break;
		}
		break;
	case WM_DESTROY:
		RemovePropA( button, "UIOLDPROC" );
		if( oldProc ){
			SetWindowLongA( button, GWL_WNDPROC, (LONG)oldProc );
		}
		[[fallthrough]];
	default:
		if( oldProc ){
			result = CallWindowProcA_( oldProc, button, Msg, wParam, lParam );
		}else{
			result = DefWindowProcA_( button, Msg, wParam, lParam );
		}
		break;
	case WM_PAINT:
		PaintElem( button );
		break;
	}
	return result;
}

//----- (20003EB0) --------------------------------------------------------
void __fastcall CreateNewGame_2( HWND hdlg, int difficulty = -1 )
{
	HWND hdlg_1; // ebp@1
	HWND activeButton; // esi@1
	client_info *clientInfo; // eax@2
	GameTemplate *gameTemplate_1; // edi@3
	GameTemplate gameTemplate; // [sp+20h] [bp-248h]@2
	char errorText[ 128 ]; // [sp+28h] [bp-240h]@8
	char errorBuf[ 192 ]; // [sp+A8h] [bp-1C0h]@7
	char pszGameStatString[ 256 ]; // [sp+168h] [bp-100h]@2

	hdlg_1 = hdlg;
	activeButton = GetFocus();
	if( hdlg_1 == GetParent( activeButton ) || difficulty > -1 ){
		gameTemplate.difficulty = difficulty > -1 ? difficulty : DifWin::Val(activeButton);
		SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, pszGameStatString, 256 );
		clientInfo = ClientInfo_1;
		if( ClientInfo_1->gameTemplateSize >= 8u ){
			gameTemplate_1 = ClientInfo_1->gameTemplate;
			gameTemplate_1->difficulty = difficulty > -1 ? difficulty : DifWin::Val(activeButton);
			clientInfo = ClientInfo_1;
		}
		if( SNetCreateGame(
			GameName_1,
			0,
			pszGameStatString,
			0,
			(char *) clientInfo->gameTemplate,
			clientInfo->gameTemplateSize,
			ProviderInfo_1->maxPlayers,
			UserInfo_1->PlayerName,
			0,
			PlayerId ) ){
			PlaySoundTitleSelect();
			SDlgKillTimer( hdlg_1, 1 );
			SDlgEndDialog( hdlg_1,  1 );
		}else{
			ErrorCode = GetLastError();
			if( ErrorCode == ERROR_ALREADY_EXISTS ){
				LoadStringA( HInstance, STR_73_Game_already_exists, errorText, 127 );
				sprintf( errorBuf, errorText, GameName_1 );
			}else{
				LoadStringA( HInstance, STR_38_Unable_create, errorBuf, 191 );
			}
			SelectOk_2( hdlg_1, errorBuf, 0, 0 );
		}
	}
}

//----- (20004010) --------------------------------------------------------
void __fastcall CreateNewGame_3( HWND hdlg )
{
	HWND hdlg_1; // ebp@1
	HWND selectedButton; // eax@1
	HWND selectedButton_2; // edi@3
	client_info *clientInfo; // eax@4
	GameTemplate *gameTemplate_2; // ebx@5
	GameTemplate gameTemplate; // [sp+18h] [bp-3D0h]@1
	GameTemplate gameTemplate_1; // [sp+20h] [bp-3C8h]@4
	char gameAlreadyExists[ 128 ]; // [sp+28h] [bp-3C0h]@10
	char unableCreateGame[ 192 ]; // [sp+A8h] [bp-340h]@9
	char gameStatString[ 256 ]; // [sp+168h] [bp-280h]@1
	char errorTextBuffer[ 256 ]; // [sp+1E8h] [bp-200h]@1
	char pszGameStatString[ 256 ]; // [sp+2E8h] [bp-100h]@4

	hdlg_1 = hdlg;
	selectedButton = GetFocus();
	gameTemplate.difficulty = DifWin::Val(selectedButton);
	SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
	if( UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorTextBuffer, 256 ) ){
		selectedButton_2 = GetFocus();
		if( hdlg_1 == GetParent( selectedButton_2 ) ){
			gameTemplate_1.difficulty = DifWin::Val(selectedButton);
			SetGameStatString( &gameTemplate_1, UserInfo_1->PlayerName, UserInfo_1->playerDescription, pszGameStatString, 256 );
			clientInfo = ClientInfo_1;
			if( ClientInfo_1->gameTemplateSize >= 8u ){
				gameTemplate_2 = ClientInfo_1->gameTemplate;
				gameTemplate_2->difficulty = DifWin::Val(selectedButton);
				clientInfo = ClientInfo_1;
			}
			if( SNetCreateGame(
				GameName_1,
				0,
				pszGameStatString,
				0,
				(char *) clientInfo->gameTemplate,
				clientInfo->gameTemplateSize,
				ProviderInfo_1->maxPlayers,
				UserInfo_1->PlayerName,
				0,
				PlayerId ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg_1, 1 );
				SDlgEndDialog( hdlg_1,  1 );
			}else{
				ErrorCode = GetLastError();
				if( ErrorCode == ERROR_ALREADY_EXISTS ){
					LoadStringA( HInstance, STR_73_Game_already_exists, gameAlreadyExists, 127 );
					sprintf( unableCreateGame, gameAlreadyExists, GameName_1 );
				}else{
					LoadStringA( HInstance, STR_38_Unable_create, unableCreateGame, 191 );
				}
				SelectOk_2( hdlg_1, unableCreateGame, 0, 0 );
			}
		}
	}else{
		SelectOk_2( hdlg_1, errorTextBuffer, 0, 0 );
	}
}

//----- (200038EB) --------------------------------------------------------
int __stdcall DialogSelectDifficulty( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam )
{
	GameTemplate gameTemplate;
	char errorText[ 256 ];
	static int isMultiGame = 1;

	XinputVirtualKeyboard();
	CheckEnter( msg, wParam, lParam );
	switch( msg ){
	case WM_INITDIALOG:
	{
		for( int i = 0; DifButtonList[i] != 0; ++i ){
			int saveButtonId = DifButtonList[i];
			HWND saveButton = GetDlgItem( hDlg, saveButtonId );
			if( saveButton ){
				SetPropA( saveButton, "UIOLDPROC", (HANDLE)GetWindowLongA( saveButton, GWL_WNDPROC ) );
				SetWindowLongA( saveButton, GWL_WNDPROC, (LONG)DifSelectButtonProc );
			}
		}
		isMultiGame = lParam;
		if( HasPentagram ){
			InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
		}else{
			InitSelection();
		}
		SDlgSetTimer( hDlg, 1, 55, 0 );
		TextWin* textWin = AllocTextWin();
		if( textWin ){
			SetWindowLongA( hDlg, GWL_USERDATA, (LONG) textWin );
			LoadDialogBackground( hDlg, 0, "popup", -1, 1, "ui_art\\seldiff.pcx", &textWin->surface, &textWin->size, 0 );
		}
		InitButtonListProc( hDlg, DifButtonList );
		InitTextElemList( hDlg, DifficultyTitleList, 5 );
		InitTextElemList( hDlg, DifficultyCtextList, 5 );
		InitTextElemList( hDlg, DifficultyLtextList, 1 );
		InitButtonListText( hDlg, DifficultyOkCancelList, 4, 0 );
		InitButtonListText( hDlg, DifButtonList, 2, 1 );
		ResetDifButtons( hDlg, 0 );
		InitScroll( hDlg, ID_SCROLL );
		if( DifCount() <= MaxScrollCount_6 ){
			ShowWindow( GetDlgItem( hDlg, ID_SCROLL ), 0 );
		}
		if( GameMode == GM_COLISEUM ){
			CreateNewGame_2( hDlg, DL_0_HORROR );
			break;
		}
		return 0;
	}
	case WM_DESTROY:
	{
		DeleteElementListData( hDlg, DifButtonList );
		DeleteElementListData( hDlg, DifficultyOkCancelList );
		DeleteElementListData( hDlg, DifficultyLtextList );
		DeleteElementListData( hDlg, DifficultyCtextList );
		DeleteElementListData( hDlg, DifficultyTitleList );
		DeleteWinData( (TextWin *) GetWindowLongA( hDlg, GWL_USERDATA ) );
		SetWindowLongA( hDlg, GWL_USERDATA, 0 );
		if( HasPentagram ){ DeletePentagram(); }
		DeleteScroll( hDlg, ID_SCROLL );
		break;
	}
	case WM_COMMAND:
	{
		if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
			LeaveButton( hDlg, (HWND) lParam );
		}else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
			InvalidateRect( hDlg, NULL, NULL );
			UpdateWindow( hDlg );
			SelectButton( (HWND) lParam );
			AnimateSelectionPentagram( hDlg, (HWND) lParam );
			DifRefreshSelection(hDlg);
			DifRefreshScroll(hDlg);
		}else if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED ){
			if( isMultiGame != 1 ){
				SetSelectedDifficulty(DifWin::Val(GetFocus()));
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg,  1 );
			}else{
				gameTemplate.difficulty = DifWin::Val(GetFocus()); // SelectedDifficulty;
				char gameStatString[ 256 ];
				SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
				if( !UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorText, 256 ) ){
					SelectOk_2( hDlg, errorText, 0, 0 );
				}else{
					CreateNewGame_2( hDlg );
				}
			}
		}else if( (ushort) wParam == 1 ){                 // press Enter (see CheckEnter)
			if( isMultiGame != 1 ){
				SetSelectedDifficulty(DifWin::Val(GetFocus()));
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg,  1 );
			}else{
				gameTemplate.difficulty = DifWin::Val(GetFocus()); // SelectedDifficulty;
				char gameStatString[ 256 ];
				SetGameStatString( &gameTemplate, UserInfo_1->PlayerName, UserInfo_1->playerDescription, gameStatString, 128 );
				if( !UiAuthCallback( 2, UserInfo_1->PlayerName, UserInfo_1->playerDescription, 0, gameStatString, errorText, 256 ) ){
					SelectOk_2( hDlg, errorText, 0, 0 );
				}else{
					CreateNewGame_2(hDlg);
				}
			}
		}else if( (ushort) wParam == 2 ){                 // press Esc (see CheckEnter)
			PlaySoundTitleSelect();
			SDlgKillTimer( hDlg, 1 );
			SDlgEndDialog( hDlg,  2 );
		}
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		int cursorX = (ushort) lParam;
		int cursorY = (uint) lParam >> 16;
		if( CheckCursorOnButton( hDlg, GetDlgItem( hDlg, ID_OK_BUTTON ), cursorX, cursorY ) ){
			if( isMultiGame == 1 ){
				CreateNewGame_3( hDlg );
			}else{
				SetSelectedDifficulty(DifWin::Val(GetFocus()));
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg, 1 );
			}
		}else if( CheckCursorOnButton( hDlg, GetDlgItem( hDlg, ID_CANCEL_BUTTON ), cursorX, cursorY ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hDlg, 1 );
				SDlgEndDialog( hDlg, 2 );
		}else if( CheckCursorOnButton( hDlg, GetDlgItem( hDlg, ID_SCROLL ), cursorX, cursorY ) ){
			cursorY += 150;
			switch( GetScrollAction( GetDlgItem( hDlg, ID_SCROLL ), cursorX, cursorY ) ){
			case 1: DifUp( GetFocus() ); break;
			case 2: DifDown( GetFocus() ); break;
			case 3: DifPageUp( GetFocus() ); break;
			case 4: DifPageDown( GetFocus() ); break;
			}
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		if( CheckScrollPressAndRelease( GetDlgItem( hDlg, ID_SCROLL ) ) ){
			DifRefreshScroll( hDlg );
		}
		break;
	}
	case WM_TIMER:
		if( wParam == 1 ){
			AnimateSelectionPentagram( hDlg, GetFocus() );
		}
		break;
	case WM_SYSKEYUP or WM_SYSKEYDOWN:
		SendMessageA( SDrawGetFrameWindow(0), msg, wParam, lParam );
		return 0;
	}
	return SDlgDefDialogProc_( hDlg, msg, wParam, lParam );
}

//----- (200041E0) --------------------------------------------------------
int __fastcall SelectDifficulty( Provider_info *providerInfo, client_info *clientInfo, user_info *userInfo, battle_info *battleInfo, module_info *moduleInfo, int *playerId, int hasPentagram, char *gameName )
{
	ProviderInfo_1 = providerInfo;
	ClientInfo_1 = clientInfo;
	UserInfo_1 = userInfo;
	PlayerId = playerId;
	HasPentagram = hasPentagram;
	GameName_1 = gameName;
	LPARAM isMultiplayer = IsMultiplayer();
	int diffIsSelected = SDlgDialogBoxParam( HInstance, "SELDIFF_DIALOG", battleInfo->FrameWnd, DialogSelectDifficulty, isMultiplayer );
	if( diffIsSelected != 1 ){
		SetLastError( ErrorCode );
		diffIsSelected = 0;
	}
	UserInfo_1 = 0;
	return diffIsSelected;
}

//----- (200055B0) --------------------------------------------------------
int __fastcall SelectDifficultyDirectConnect( HWND hWnd )
{
	HWND hwnd; // ebx@1
	int maxPlayers; // ecx@1
	int difIsSelected; // eax@1
	Provider_info providerInfo; // [sp+Ch] [bp-44h]@1
	battle_info battleInfo; // [sp+1Ch] [bp-34h]@1

	hwnd = hWnd;
	memcpy( &battleInfo, CableBattleInfo, sizeof( battleInfo ) );
	battleInfo.Size = 52;
	battleInfo.FrameWnd = hWnd;
	providerInfo.maxPlayers = 0;
	providerInfo.size = 16;
	providerInfo.field_C = 0;
	providerInfo.provider = 'SCBL';
	maxPlayers = CableClientInfo->MaxPlayers;
	providerInfo.field_C = 0;
	providerInfo.maxPlayers = maxPlayers;
	difIsSelected = SelectDifficulty(&providerInfo, CableClientInfo, CableUserInfo, &battleInfo, CableModuleInfo, CablePlayerIndex, 0, CableUserInfo->PlayerName);
	if( difIsSelected ){
		PlaySoundTitleSelect();
		CheckFadeTimer();
		SDlgKillTimer( hwnd, 3 );
		SDlgKillTimer( hwnd, 2 );
		SDlgKillTimer( hwnd, 1 );
		FadePalette( 10 );
		difIsSelected = SDlgEndDialog( hwnd,  1 );
	}
	return difIsSelected;
}
