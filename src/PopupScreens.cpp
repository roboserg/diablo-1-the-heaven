#include "stdafx.h"

int DiabloQuestTimer;// 00575400
char PopupScreen[228 * 1024]; // TODO: check wtf is this
int SomeDword_60;// 00575404
char IsPopupScreenOpen;// 005753F8

//----- (0040B95F) --------------------------------------------------------
int GetMapOfTheStarsMinutsPassedCount()
{
	if( DiabloQuestTimer == 36001 ){
		return 31;
	}else{
		return DiabloQuestTimer / 1200;
	}
}

//----- (0040B997) --------------------------------------------------------
void OpenPopupScreen()
{
	SomeDword_60 = (-(GetMapOfTheStarsMinutsPassedCount() != 31)) & 0xE1 + 31;// принимает значения от 31 (при !=31) до 256 (при ==31)
	if( ReadPopupScreen() ){
		IsPopupScreenOpen = true;
	}else{
		ClosePopupScreen();
	}
}

//----- (0040B9EA) --------------------------------------------------------
bool ReadPopupScreen()
{
	bool loadindIsDone = false;
	strcpy(InfoPanelBuffer, "Items\\Map\\MapZtown.CEL");
	if( LoadFileWithMem(InfoPanelBuffer, PopupScreen) ){
		loadindIsDone = true;
	}
	return loadindIsDone;
}

//----- (0040BA18) --------------------------------------------------------
void ClosePopupScreen()
{
	IsPopupScreenOpen = false;
}

//----- (0040BA24) --------------------------------------------------------
void DrawPopupScreen()
{
	if( IsPopupScreenOpen ){
		// 64, 511 в оригинале. не тестировалось
		
		int height = 351;
		Surface_DrawCEL((WorkingWidth - GUI_Width) / 2, (ScreenHeight - GUI_MainPanelHeight - height) / 2 + height + Screen_TopBorder /*511*/, PopupScreen, 1, 640);
	}
}

