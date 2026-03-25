#ifndef _difficulty_h_
#define _difficulty_h_
#else
#error reinclude difficulty.h
#endif

enum DIFFICULTY
{
	DL_0_HORROR			= 0,
	DL_1_PURGATORY		= 1,
	DL_2_DOOM			= 2,
	DL_3_CRUCIBLE		= 3,
	DL_4_TORMENT		= 4,
	DL_5_AGONY			= 5,
	DL_6_TERROR			= 6,
	DL_7_DESTRUCTION	= 7,
	DL_8_INFERNO		= 8,
	DL_COUNT
};

using DifWin = Win<int>;

extern int Difficulty;
extern int SelectedDifficulty;

const char* DifName(int dif = -1);
int __stdcall DialogSelectDifficulty( HWND hDlg, unsigned int msg, WPARAM wParam, LPARAM lParam );
int __fastcall SelectDifficultyDirectConnect( HWND hWnd );
void UpdateAllowedDifficulty();
