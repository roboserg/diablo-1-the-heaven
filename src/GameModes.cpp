#include "stdafx.h"

GAME_MODE GameMode = GM_NORMAL;
int GameModeFilter = -1;

struct Mode
{
	GAME_MODE id;
	const char* name;
	const char* desc;
};

Mode Modes[GM_count] = {
	{ GM_NORMAL,    "Normal",    "Normal mode is a classic set of rules since the original The Hell mod. This is the default difficulty and the intended way to play. For more info, see README file." },
	{ GM_HARDCORE,	"Hardcore",  "In Hardcore mode death is permanent. If you die, your character file is erased. Level of difficulty is average." },
	{ GM_IRONMAN,	"Ironman",   "Ironman rules, bosses drop Identify relicts, repair oils, magic find is +100%, increased experience, saving allowed after clearing dungeon levels 4/8/12/16/20/24." },
	{ GM_NIGHTMARE,	"Nightmare", "Nightmare: saving allowed only after beating level 24, death is permanent. Magic find is +100%, bosses drop 2 ID relicts and one oil, increased experience." },
	{ GM_EASY,		"Easy",      "Easy is a variation of Normal. Without features like battle lockdowns and inescapable quest locations. Monsters are less aggressive. Recommended for new players." },
	{ GM_SURVIVAL,	"Survival",  "Survival is a Normal mode variation, no town backtracking, saving is allowed once every level. Battle pause is allowed. +50% MF, +(50+CLVL)% XP." },
	{ GM_HARD,		"Hard",      "Hard mode is the most difficult challenge of this mod. Monsters are aggressive and dangerous. Prepare to get brutalized. Level of difficulty is extreme." },
	{ GM_SPEEDRUN,	"Speedrun",  "Nightmare variation. Saving allowed only after beating level 24, no Vise, no pause, 200% MF, increased experience, and a timer which stops with your death or His." },
	{ GM_COLISEUM,	"Coliseum",  "Coliseum mode" },
	{ GM_CLASSIC,	"Classic",   "Classic mode" },
};

int GameModeTitleList[ 2 ] = { ID_CAPTION, 0 };
int GameModeOkCancelList[ 3 ] = { ID_OK_BUTTON, ID_CANCEL_BUTTON, 0 };

int GameModeButtonList[] = {
	ID_GAME_MODE_1,
	ID_GAME_MODE_2,
	ID_GAME_MODE_3,
	ID_GAME_MODE_4,
	ID_GAME_MODE_5,
	ID_GAME_MODE_6,
	0 };

constexpr std::array nextGameModeOrder = {
	ID_GAME_MODE_2,
	ID_GAME_MODE_3,
	ID_GAME_MODE_4,
	ID_GAME_MODE_5,
	ID_GAME_MODE_6,
	ID_GAME_MODE_1,
};
constexpr std::array prevGameModeOrder = {
	ID_GAME_MODE_6,
	ID_GAME_MODE_1,
	ID_GAME_MODE_2,
	ID_GAME_MODE_3,
	ID_GAME_MODE_4,
	ID_GAME_MODE_5,
};

// import
void __fastcall ResetButtonText( HWND hdlg, int *elemList, int fontIndex, int limitWidth );
void __fastcall WriteTextToElemData( TextWin *elemData, const char *srcText );
void __cdecl PlaySoundTitleMove();
void __fastcall PaintElem( HWND elem );
int __stdcall SDlgDefDialogProc_( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
int __stdcall CallWindowProcA_( WNDPROC lpPrevWndFunc, HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
int __fastcall AnimateSelectionPentagram( HWND hdlg, HWND curButton );
int __fastcall CheckScrollPressAndRelease( HWND scroll );
int __fastcall CheckCursorOnButton( HWND hdlg, HWND button, int cursorX, int cursorY );
int __stdcall DefWindowProcA_( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
void __cdecl PlaySoundTitleSelect();
int __fastcall GetScrollAction( HWND scroll, int cursorX, int cursorY );
void __fastcall SetScrollOnElem( HWND hDlg, int nIDDlgItem, int scrollLineCount, int elemIndex );
void __fastcall InitTextElemList( HWND hWnd, int *elemList, int fontIndex );
int __fastcall InitButtonListText( HWND hdlg, int *elemList, int fontIndex, int limitWidth );
void __fastcall InitSelectionAndLoadPentagram( char *pentagramName );
void __cdecl DeletePentagram();
void __fastcall InitScroll( HWND hDlg, int nIDDlgItem );
void __fastcall DeleteScroll( HWND hDlg, int nIDDlgItem );
void __fastcall DeleteElementListData( HWND hDlg, int *elementList );
void __fastcall WriteAndDrawDialogTitle( HWND hdlg, char *text );
void __fastcall LeaveButton( HWND hdlg, HWND button );
void __fastcall SelectButton( HWND button );
void ShowWindowList( HWND hDlg, int* list, int nCmdShow );
void __fastcall AdjustScrollRect( RECT *rect, int newLeft, int newTop );
void __cdecl CheckEnter(uint& msg, WPARAM& wParam, LPARAM& lParam);
void __fastcall InitButtonListProc( HWND hwnd, int *elemList );
LPARAM IsMultiplayer();

extern CharSaveInfo NewSaveInfo;
extern int CharParamNameList[];
extern int CharParamValueList[];
extern int ClassDescriptionList[];

//----- (th3) ---------------------------------------------------------------
Portrait getGameModePortrait( GAME_MODE gameMode )
{
    Portrait portrait;
    switch( gameMode ){
        case GM_NORMAL:     portrait = PcxHeros[56]; break;
        case GM_HARDCORE:	portrait = PcxHeros[ 8]; break;
        case GM_IRONMAN:	portrait = PcxHeros[80]; break;
        case GM_NIGHTMARE:	portrait = PcxHeros[ 4]; break;
        case GM_EASY:	    portrait = PcxHeros[78]; break;
        case GM_SURVIVAL:	portrait = PcxHeros[54]; break;
        case GM_HARD:	    portrait = PcxHeros[55]; break;
		case GM_SPEEDRUN:   portrait = PcxHeros[53]; break;
		case GM_COLISEUM:   portrait = PcxHeros[56]; break;
		case GM_CLASSIC:    portrait = PcxHeros[ 2]; break;
    }
    return portrait;
}

//----- (th3) ---------------------------------------------------------------
const char* getGameModeDescription( GAME_MODE gameMode )
{
	if( gameMode >= GM_count ) return "Unknown mode description";
	else return Modes[gameMode].desc;
}

//----- (th3) ---------------------------------------------------------------
const char* getGameModeName( GAME_MODE gameMode, GAME_CHANGER GameChanger/* = GC_0_CONTINUE*/ )
{
	if( gameMode >= GM_count ) return "Unknown mode name";
    if( gameMode == GM_IRONMAN && GameChanger & BIT(GC_24_RUTHLESS) ) return "Ruthless Iron";
	else return Modes[gameMode].name;
}

//----- (th3) ---------------------------------------------------------------
int GetSuperGameMode(char*) // TODO: check wtf is this
{
	return GM_NORMAL;
}

//----- (th3) ---------------------------------------------------------------
vector<GAME_MODE> GetAvailableGameModes()
{
	vector<GAME_MODE> modes;
	modes.reserve(GM_count);
	modes.push_back(GM_EASY);
	modes.push_back(GM_NORMAL);
	if( IsMultiplayer() ){ // multi
		if( ! ColiseumInMainMenu ) modes.push_back(GM_COLISEUM);
	}else{ // single
		if( ! ClassicInMainMenu ) modes.push_back(GM_CLASSIC);
	}
	modes.push_back(GM_HARD);
	modes.push_back(GM_SURVIVAL);
	modes.push_back(GM_HARDCORE);
	modes.push_back(GM_IRONMAN);
	modes.push_back(GM_SPEEDRUN);
	modes.push_back(GM_NIGHTMARE);
	return modes;
}

//----- (th3) ---------------------------------------------------------------
void __fastcall SetGameModeInfoText( HWND parent, const GAME_MODE gameMode )
{
    ShowWindowList( parent, CharParamNameList, SW_HIDE );
    ShowWindowList( parent, CharParamValueList, SW_HIDE );
    ShowWindowList( parent, ClassDescriptionList, SW_SHOW );
    
    Portrait portrait = getGameModePortrait( gameMode );
    const char* description = getGameModeDescription( gameMode );
    
    SetWindowTextA( GetDlgItem( parent, ID_CLASS_DESCRIPTION ), description );

    DrawTextToElemList( parent, ClassDescriptionList, 1 );
    
    const HWND portraitWin = GetDlgItem( parent, ID_PLAYER_PORTRAIT );
    struct tagRECT rect;
    InvalidateRect( portraitWin, 0, 0 );
    GetClientRect( portraitWin, &rect );
    AdjustScrollRect( &rect, 0, rect.bottom * portrait.id );
	SDlgSetBitmap( portraitWin, 0, "Static", -1, 1, portrait.pcx->data, &rect, portrait.pcx->size.cx, portrait.pcx->size.cy, -1 );
}

//----- (th3) ---------------------------------------------------------------
void __fastcall ResetGameModeButtons( HWND hdlg, const int topIndex )
{
    for( int i = 0; GameModeButtonList[i] != 0; ++i ){
        int saveButtonId = GameModeButtonList[i];
        HWND saveButton = GetDlgItem( hdlg, saveButtonId );
        if( saveButton ){
            int currentGameModeIndex = topIndex + i;
			auto modes = GetAvailableGameModes();
            if( currentGameModeIndex < (int)modes.size() ){
                EnableWindow( saveButton, true );
                if( auto gameModeWin = (GameModeWin*) GetWindowLongA( saveButton, GWL_USERDATA ); gameModeWin ){
                    WriteTextToElemData( (TextWin *) gameModeWin, getGameModeName( modes[currentGameModeIndex] ) );
                    gameModeWin->gameModeIndex = (void*)currentGameModeIndex;
                }
            }else{
                EnableWindow( saveButton, false );
            }
        }
    }
    ResetButtonText( hdlg, GameModeButtonList, 2, 1 );
}

//----- (th3) ---------------------------------------------------------------
void GameModeRefreshCurrentGameModeInfo( HWND hdlg, int button )
{
    if( const HWND activeElem = GetDlgItem( hdlg, button ); activeElem ){
        if( const auto gameModeWin = (GameModeWin *)GetWindowLongA( activeElem, GWL_USERDATA ); gameModeWin ){
            GameMode = GetAvailableGameModes()[(int)gameModeWin->gameModeIndex];
            SetGameModeInfoText( GetParent( hdlg ), GameMode );
        }
    }
}

//----- (th3) ---------------------------------------------------------------
int __fastcall GetGameModeElemIndex( HWND elem )
{
    int result = 0;
    if( elem ){
        if( auto* gameModeWin = (GameModeWin *) GetWindowLongA( elem, GWL_USERDATA ); gameModeWin ){
            return (int)gameModeWin->gameModeIndex;
        }
    }
    return result;
}

//----- (th3) ---------------------------------------------------------------
void GameModeRefreshScrollState( const HWND hdlg )
{
    SetScrollOnElem( hdlg, ID_SCROLL, GetAvailableGameModes().size(), GetGameModeElemIndex( GetFocus() ) );
}

//----- (th3) ---------------------------------------------------------------
void GameModeSelectNextGameMode( const HWND hdlg, const HWND button, int order )
{
    const auto& buttonsOrder = ( order > 0 ) ? nextGameModeOrder : prevGameModeOrder;
    HWND currentButton = button;
    for( auto i{ 0u }, ie{ buttonsOrder.size() }; i < ie; ++i ){
        currentButton = GetDlgItem( hdlg, buttonsOrder[ GetWindowLongA( currentButton, GWL_ID ) - ID_GAME_MODE_1 ] );
        if( IsWindowEnabled( currentButton ) ){
            SetFocus( currentButton );
            break;
        }
    }
}

//----- (th3) ---------------------------------------------------------------
void __fastcall GameModePageDown( const HWND button )
{
    const HWND hdlg = GetParent( button );
    if( !hdlg ){
        return;
    }
    
    const HWND firstBut = GetDlgItem( hdlg, ID_GAME_MODE_1 );
    if( !firstBut ){
        return;
    }
    
    auto gameModeWin = (GameModeWin *) GetWindowLongA( GetDlgItem( hdlg, ID_GAME_MODE_6 ), GWL_USERDATA );
    if( !gameModeWin ){
        return;
    }
	auto modes = GetAvailableGameModes();
    if( auto gameModeIndex = (int)gameModeWin->gameModeIndex; gameModeIndex + 1 < (int)modes.size() ){
        int topSaveIndex = GetGameModeElemIndex( firstBut );
        int nextTopGameModeIndex = std::min( topSaveIndex + 6, (int)modes.size() - 6 );
        PlaySoundTitleMove();
        ResetGameModeButtons( hdlg, nextTopGameModeIndex );
        GameModeRefreshCurrentGameModeInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
        GameModeRefreshScrollState( hdlg );
    }else{
        GameModeSelectNextGameMode( hdlg, firstBut, -1 );
    }
}

//----- (th3) ---------------------------------------------------------------
void __fastcall GameModePageUp( const HWND button )
{
    const auto hdlg = GetParent( button );
    if( !hdlg ){
        return;
    }
    
    const HWND firstBut = GetDlgItem( hdlg, ID_GAME_MODE_1 );
    if( !firstBut ){
        return;
    }
    
    const auto gameModeWin = (const GameModeWin *) GetWindowLongA( firstBut, GWL_USERDATA );
    if( !gameModeWin ){
        return;
    }
    
    if( const auto gameModeIndex = (int)gameModeWin->gameModeIndex; gameModeIndex == 0 ){
        const auto lastBut = GetDlgItem( hdlg, ID_GAME_MODE_6 );
        GameModeSelectNextGameMode( hdlg, lastBut, 1 );
    }else{
        const int topSaveIndex = GetGameModeElemIndex( firstBut );
        const int nextTopGameModeIndex = std::max( topSaveIndex - 6, 0 );
        PlaySoundTitleMove();
        ResetGameModeButtons( hdlg, nextTopGameModeIndex );
        GameModeRefreshCurrentGameModeInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
        GameModeRefreshScrollState( hdlg );
    }
}

//----- (th3) ---------------------------------------------------------------
void __fastcall GameModeDown( const HWND button )
{
    const auto hdlg = GetParent( button );
    
    if( auto gameModeWin = (GameModeWin *) GetWindowLongA( button, GWL_USERDATA ); gameModeWin ){
        if( auto gameModeIndex = (int)gameModeWin->gameModeIndex; gameModeIndex + 1 < (int)GetAvailableGameModes().size() ){
            if( GetWindowLongA( button, GWL_ID ) < ID_GAME_MODE_6 ){
                GameModeSelectNextGameMode( hdlg, button, 1 );
            }else{
                if( auto newBut = GetDlgItem( hdlg, ID_GAME_MODE_2 ); newBut ){
                    if( auto newWin = (GameModeWin*)GetWindowLongA( newBut, GWL_USERDATA ); newWin ){
                        const auto nextTopGameModeIndex = (int)newWin->gameModeIndex;
                        PlaySoundTitleMove();
                        ResetGameModeButtons( hdlg, nextTopGameModeIndex );
                        GameModeRefreshCurrentGameModeInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
                        GameModeRefreshScrollState( hdlg );
                    }
                }
            }
        }
    }
}

//----- (th3) ---------------------------------------------------------------
void __fastcall GameModeUp( const HWND button )
{
    const auto hdlg = GetParent( button );
    
    if( GetWindowLongA( button, GWL_ID ) > ID_GAME_MODE_1 ){
        GameModeSelectNextGameMode( hdlg, button, -1 );
        return;
    }else{
        if( auto gameModeWin = (GameModeWin*)GetWindowLongA( button, GWL_USERDATA ); gameModeWin ){
            if( auto gameModeIndex = (int)gameModeWin->gameModeIndex; gameModeIndex > 0 ){
                const auto nextTopGameModeIndex = gameModeIndex - 1;
                PlaySoundTitleMove();
                ResetGameModeButtons( hdlg, nextTopGameModeIndex );
                GameModeRefreshCurrentGameModeInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
                GameModeRefreshScrollState( hdlg );
            }
        }
    }
}

//----- (th3) ---------------------------------------------------------------
LRESULT __stdcall GameModeSelectButtonProc( const HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{

    XinputVirtualKeyboard();

    LRESULT result = 0;
    auto oldProc = (WNDPROC)GetPropA( button, "UIOLDPROC" );
    const auto hdlg = GetParent( button );
    
    switch( Msg ){
        case WM_GETDLGCODE:
            result = 4;
            break;
        case WM_KEYFIRST:
            switch( wParam ){
                case VK_RETURN:
                case VK_SPACE:
                    SendMessageA( hdlg, 273, 1, 0 );
                    break;
                case VK_ESCAPE:
                    SendMessageA( hdlg, 273, 2, 0 );
                    break;
                case VK_TAB:
                    GameModeSelectNextGameMode( hdlg, button, ( GetKeyState( VK_SHIFT ) >= 0 ) ? 1 : -1 );
                    break;
                case VK_LEFT:
                case VK_UP:
                    GameModeUp( button );
                    break;
                case VK_RIGHT:
                case VK_DOWN:
                    GameModeDown( button );
                    break;
                case VK_PRIOR:
                    GameModePageUp( button );
                    break;
                case VK_NEXT:
                    GameModePageDown( button );
                    break;
                case VK_DELETE:
                    SendMessageA( hdlg, Msg, wParam, lParam );
                    break;
                default:
                    break;
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

//----- (th3) ---------------------------------------------------------------
int __stdcall DialogSelectGameMode( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
    XinputVirtualKeyboard();

    int result;
    bool needToCallDefDialogProc = true;
    
    switch( Msg ){
        case WM_INITDIALOG:
        {
	        if( GameModeFilter > 0 && GameModeFilter & B(GameMode) ){
				SDlgEndDialog( hdlg, lParam == 1 ? -1 : 0 );
                return SDlgDefDialogProc_(hdlg, Msg, wParam, lParam);
            }
            for( int i = 0; GameModeButtonList[i] != 0; ++i ){
                int saveButtonId = GameModeButtonList[i];
                HWND saveButton = GetDlgItem( hdlg, saveButtonId );
                if( saveButton ){
                    SetPropA( saveButton, "UIOLDPROC", (HANDLE)GetWindowLongA( saveButton, GWL_WNDPROC ) );
                    SetWindowLongA( saveButton, GWL_WNDPROC, (LONG)GameModeSelectButtonProc );
                }
            }
            
            char tempBuf[32];
			LoadStringA( HInstance, NewHeroTitle(), tempBuf, 31 );
			WriteAndDrawDialogTitle( GetParent( hdlg ), tempBuf );
            SetWindowLongA( hdlg, GWL_USERDATA, GetWindowLongA( GetParent( hdlg ), GWL_USERDATA ) );
            InitTextElemList( hdlg, GameModeTitleList, 5 );
            InitButtonListText( hdlg, GameModeOkCancelList, 4, 0 );
            InitButtonListText( hdlg, GameModeButtonList, 2, 1 );
            ResetGameModeButtons( hdlg, 0 );
            InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
            SDlgSetTimer( hdlg, 1, 55, 0 );
            InitScroll( hdlg, ID_SCROLL );
            if( GetAvailableGameModes().size() <= 6 ){
                ShowWindow( GetDlgItem( hdlg, ID_SCROLL ), 0 );
            }
            needToCallDefDialogProc = false;
            result = 0;
            break;
        }
        case WM_DESTROY:
        {
            DeleteScroll( hdlg, ID_SCROLL );
            DeletePentagram();
            DeleteElementListData( hdlg, GameModeButtonList );
            DeleteElementListData( hdlg, GameModeOkCancelList );
            DeleteElementListData( hdlg, GameModeTitleList );
            WriteAndDrawDialogTitle( GetParent( hdlg ), 0 );
            break;
        }
        case WM_COMMAND:
        {
            if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
                LeaveButton( hdlg, (HWND) lParam );
            }else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
                InvalidateRect( GetParent( hdlg ), NULL, NULL ); //removes menu freezes // character selection menu
                UpdateWindow( GetParent( hdlg ) ); //removes menu freezes // character selection menu
                SelectButton( (HWND) lParam );
                AnimateSelectionPentagram( hdlg, (HWND) lParam );
                GameModeRefreshCurrentGameModeInfo( hdlg, (unsigned __int16) wParam );
                GameModeRefreshScrollState( hdlg );
            }else if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){ // Enter
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg, 1 );
            }else if( (ushort)wParam == 2 ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg, 2 );
            }
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        {
            int cursorX = (unsigned __int16) lParam;
            int cursorY = (unsigned int) lParam >> 16;
            if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_OK_BUTTON ), cursorX, cursorY ) ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg,  1 );
            }else if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_CANCEL_BUTTON ), cursorX, cursorY ) ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg,  2 );
            }else if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_SCROLL ), cursorX, cursorY ) ){
                switch( GetScrollAction( GetDlgItem( hdlg, ID_SCROLL ), cursorX, cursorY ) ){
                    case 1:
                        GameModeUp( GetFocus() );
                        break;
                    case 2:
                        GameModeDown( GetFocus() );
                        break;
                    case 3:
                        GameModePageUp( GetFocus() );
                        break;
                    case 4:
                        GameModePageDown( GetFocus() );
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            if( CheckScrollPressAndRelease( GetDlgItem( hdlg, ID_SCROLL ) ) ){
                GameModeRefreshScrollState( hdlg );
            }
            break;
        }
        case WM_TIMER:
            AnimateSelectionPentagram( hdlg, GetFocus() );
            needToCallDefDialogProc = false;
            result = 0;
            break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            SendMessageA( SDrawGetFrameWindow( nullptr ), Msg, wParam, lParam );
            break;
    }
    
    if( needToCallDefDialogProc ){
        result = SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
    }
    
    return result;
} 