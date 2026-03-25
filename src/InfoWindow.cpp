#include "stdafx.h"

extern Portrait getGameChangerPortrait(GAME_CHANGER);

constexpr int IW_width = 640;
constexpr int IW_height = 462;
bool IsInfoWindowVisible = false;
bool IsLeftButtonDown = false;

DisplayObject InfoWindowRect;
constexpr const char* IW_headTexts[] = {
	 /*left*/ "Game Changer Info",
	/*right*/ "List of Game Changers"
};

DisplayObject closeButton;
char closeButtonState = 0;

constexpr size_t rowInList = 20;
DisplayObject textlist[rowInList];
char textliststates[20];
size_t lastSelectedInListIndex = 0;

DisplayObject gc_icon;

DisplayObject longTextMidPos;

size_t listStartFromIndex = 0;
size_t listStartLastIndex = 0;

char* IW_IMG_background;
char* IW_IMG_icons;
char* IW_IMG_buttons;

vector<int> gc_listIndexes;

// ---- th2 -------------------------------------------------------------------------------
void RecalculateRectRightDown(DisplayObject & obj)
{
	obj.Right = obj.Left + obj.Width;
	obj.Down = obj.Top + obj.Heigth;
}

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Init()
{
	if (gc_listIndexes.size()) gc_listIndexes.clear();
	else gc_listIndexes.reserve(128);
	listStartFromIndex = 0;
	listStartLastIndex = 0;
	lastSelectedInListIndex = 0;

	InfoWindowRect.Left = ScreenWidth / 2 - IW_width / 2;
	InfoWindowRect.Top = (ScreenHeight - 132) / 2 - IW_height / 2;
	InfoWindowRect.Width = IW_width;
	InfoWindowRect.Heigth = IW_height;
	RecalculateRectRightDown(InfoWindowRect);

	closeButton.Left = InfoWindowRect.Left + InfoWindowRect.Width - 20 - 33;
	closeButton.Top = InfoWindowRect.Top + InfoWindowRect.Heigth - 20 - 32;
	closeButton.Width = 33;
	closeButton.Heigth = 32;
	RecalculateRectRightDown(closeButton);

	gc_icon.Width = 120;
	gc_icon.Heigth = 76;
	gc_icon.Left = InfoWindowRect.Left - gc_icon.Width / 2 + (315 - 15) / 2 + 15;
	gc_icon.Top = InfoWindowRect.Top + 54;
	RecalculateRectRightDown(gc_icon);

	longTextMidPos.Left = InfoWindowRect.Left + (315 - 15) / 2 + 15;
	longTextMidPos.Top = InfoWindowRect.Top + 150;

	for (size_t i = 0; i < countof(textlist); ++i) {
		textlist[i].Left = InfoWindowRect.Left + 335;
		textlist[i].Top = InfoWindowRect.Top + 56 + 17 * i;
		textlist[i].Width = 280;
		textlist[i].Heigth = 16;
		RecalculateRectRightDown(textlist[i]);
		textliststates[i] = 0;
	}
	textliststates[lastSelectedInListIndex] = 1;

	for (int i = 1; i < GC_COUNT; ++i)
		if (Players[CurrentPlayerIndex].gameChanger & BIT(i))
			gc_listIndexes.emplace_back(i);

	listStartLastIndex = gc_listIndexes.size() <= rowInList ? 0 : gc_listIndexes.size() - rowInList;

	IW_IMG_background = (char*)LoadFile("data\\gc_info\\gc_background.cel");
	IW_IMG_icons = (char*)LoadFile("data\\gc_info\\gc_icons.cel");
	IW_IMG_buttons = (char*)LoadFile("data\\stash_21\\tradewindowbutn.cel");
};

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Free()
{
	gc_listIndexes.clear();
	FreeMemZero(IW_IMG_background);
	FreeMemZero(IW_IMG_icons);
	FreeMemZero(IW_IMG_buttons);
	listStartFromIndex = 0;
	listStartLastIndex = 0;
	lastSelectedInListIndex = 0;
};

// ---- th2 -------------------------------------------------------------------------------
void __fastcall IW_DrawRegularCel(int Xpos, int Ypos, const char* image, int frame, int width)
{
	if (!WorkingSurface || !image) return;
	char* dst = (char*)&WorkingSurface[Xpos] + YOffsetHashTable[Ypos];
	const int* img = reinterpret_cast<const int*>(image);
	int size = img[frame + 1] - img[frame];
	const char* src = image + img[frame];

	// skip header
	if (src[0] == 10 && src[1] == 0) {
		size -= 11;
		src += 11;
	}
	// decode RLE
	for (int i = 0, j = 0, k; i < size; ++i) {
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		if (j >= width) {
			dst -= WorkingWidth;
			j = 0;
		}
		if (src[i] == 127) {
			for (k = 0; k < 127; ++k) dst[j + k] = src[i + k + 1];
			i += 127;
			j += 127;
		}
		else if (src[i] == -128) j += 128;
		else if (src[i] >= 0) {
			for (k = 0; k < src[i]; ++k) dst[j + k] = src[i + k + 1];
			j += src[i];
			i += src[i];
		}
		else if (src[i] < 0) j += -src[i];
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall IW_TransparentBackground(int xPosition, int yPosition, int width, int height)
{
	size_t position = 0;
	uchar cutColors = 0;
	uchar clrVal = 0;

	struct color {
		inline static void town(const int& position) { WorkingSurface[position] = 252 + (WorkingSurface[position] >> 5); };
		inline static void dungeon(const int& position) { WorkingSurface[position] = 0; };
	};

	void (*redrawColor)(const int&) = nullptr;

	switch( Dungeon->graphType ){
	case DT_0_TOWN: cutColors = 128u; redrawColor = color::town;    break;
	case DT_4_HELL: cutColors = 16u;  redrawColor = color::dungeon; break;
	case DT_3_CAVE
	  or DT_5_CRYPT
	  or DT_6_ABYSS: cutColors = 32u; redrawColor = color::dungeon; break;
	case DT_1_CHURCH: break;
	};

	for (int i = 0, j; i < height; ++i) {
		position = WorkingWidth * (yPosition + i) + xPosition;
		for (j = 0; j < width; ++j) {
			clrVal = WorkingSurface[position];
			if (clrVal) {
				if (clrVal < cutColors)
					redrawColor(position);
				else if ((clrVal >= 128 && clrVal <= 135) || (clrVal >= 144 && clrVal <= 151) || (clrVal >= 136 && clrVal <= 143) || (clrVal > 152 && clrVal < 159))
					WorkingSurface[position] = (clrVal & 0b11111000) + ((clrVal & 0b00000111) >> 2) + 6;
				else
					WorkingSurface[position] = (clrVal & 0b11110000) + ((clrVal & 0b00001111) >> 2) + 12;
			}
			++position;
		}
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_DrawLongText(int xPos, int yPos, const char* stringPtr, int fontColor)
{
	auto InfoWindow_CropText = [](const char* text, int textSize = 22) {
		const char* last = text;
		for (int i = 0; i < textSize; ++i) {
			if (*text == ' ') {
				last = text;
			}
			else if (*text == '\0' || *text == '\n') {
				last = text;
				break;
			}
			++text;
		}
		return last;
	};

	int textWidth;
	char LetterID;
	int SurfaceOffset;

	const char* firstInLine = stringPtr;
	const char* lastInLine = nullptr;

	for (int i = 0; i < 24; ++i) {
		if (lastInLine) firstInLine = ++lastInLine;
		lastInLine = InfoWindow_CropText(firstInLine, 30);
		if (firstInLine == lastInLine) continue;
		textWidth = 0;

		const char* tmp = firstInLine;
		while (tmp != lastInLine){
			LetterID = FontIndexSmall[Codepage[*tmp]];
			textWidth += FontWidthSmall[LetterID] + 1;;
			++tmp;
		}

		SurfaceOffset = YOffsetHashTable[yPos + Screen_TopBorder + 14 * i] + xPos + Screen_LeftBorder - textWidth / 2;
		do {
			LetterID = FontIndexSmall[Codepage[*firstInLine]];
			if (LetterID)
				DrawLetter(SurfaceOffset, (uint8_t)LetterID, fontColor);
			SurfaceOffset += FontWidthSmall[LetterID] + 1;;
			++firstInLine;
		} while (firstInLine != lastInLine && *firstInLine);

		if (*firstInLine == '\0') break;
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_DrawRect(int xPosition, int yPosition, int width, int height, uint8_t color)
{
	uchar* dst = &WorkingSurface[WorkingWidth * yPosition + xPosition];
	for (int i = 0; i < height; ++i) {
		memset(dst, color, width);
		dst += WorkingWidth;
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_Draw()
{
	// Background (transparent)
	IW_TransparentBackground(
		InfoWindowRect.Left + Screen_LeftBorder + 16,
		InfoWindowRect.Top + Screen_TopBorder + 16,
		IW_width - 32,
		IW_height - 32
	);
	// Window board (image)
	IW_DrawRegularCel(
		InfoWindowRect.Left + Screen_LeftBorder,
		InfoWindowRect.Top + Screen_TopBorder + IW_height,
		IW_IMG_background,
		1,
		640
	);

	// Left main text
	DrawLevelInfoText(-GetTextWidth(IW_headTexts[0]) / 2 + InfoWindowRect.Left + 165, InfoWindowRect.Top + 44, IW_headTexts[0], C_3_Gold);
	// Right main text
	DrawLevelInfoText(-GetTextWidth(IW_headTexts[1]) / 2 + InfoWindowRect.Left + 475, InfoWindowRect.Top + 44, IW_headTexts[1], C_3_Gold);

	// Close button (image)
	IW_DrawRegularCel(
		Screen_LeftBorder + closeButton.Left,
		Screen_TopBorder + closeButton.Top + closeButton.Heigth,
		IW_IMG_buttons,
		closeButtonState ? 20 : 19,
		closeButton.Width
	);

	if(gc_listIndexes.size() == 0) return;

	// Icon (Game Changer)
	IW_DrawRegularCel(
		Screen_LeftBorder + gc_icon.Left,
		Screen_TopBorder + gc_icon.Top + gc_icon.Heigth,
		IW_IMG_icons,
		getGameChangerPortrait((GAME_CHANGER)gc_listIndexes[lastSelectedInListIndex + listStartFromIndex]).id + 1,
		gc_icon.Width
	);


	const char* textptr = nullptr;
	// Long text
	textptr = GC_Descriptions_InfoWindow[gc_listIndexes[lastSelectedInListIndex + listStartFromIndex]];
	InfoWindow_DrawLongText(
		longTextMidPos.Left,
		longTextMidPos.Top,
		textptr,
		C_0_White
	);

	textptr = nullptr;
	// List
	size_t index;
	for (size_t i = 0; i < countof(textlist) && i < gc_listIndexes.size(); ++i) {
		index = listStartFromIndex + i;
		textptr = GC_Names_InfoWindow[gc_listIndexes[index]];
		DrawLevelInfoText(
			-GetTextWidth(textptr) / 2 + textlist[i].Left + textlist[i].Width / 2,
			textlist[i].Top + 12,
			textptr,
			textliststates[i] ? textliststates[i] == 1 ? C_1_Blue : C_4_Orange : C_0_White
		);
	}

	// Scroll list indicator
	if (true /* && listStartLastIndex*/) {
		InfoWindow_DrawRect(
			Screen_LeftBorder + InfoWindowRect.Left + IW_width - 24,
			Screen_TopBorder + InfoWindowRect.Top + 55,
			1,
			340,
			197
		);
		InfoWindow_DrawRect(
			Screen_LeftBorder + InfoWindowRect.Left + IW_width - 26,
			Screen_TopBorder + InfoWindowRect.Top + 55 + ((listStartFromIndex + lastSelectedInListIndex) * 340 / (gc_listIndexes.size() > 1 ? gc_listIndexes.size() - 1 : 1)), /*division by zero*/
			5,
			1,
			241
		);
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_MouseMove()
{
	// Close button
	if (CursorIntoDisplayObject(closeButton) && IsLeftButtonDown)
		closeButtonState = 1;
	else if (closeButtonState)
		closeButtonState = 0;

	// List
	for (size_t i = 0; i < countof(textlist) && i < gc_listIndexes.size(); ++i) {
		if (CursorIntoDisplayObject(textlist[i])) {
			if (textliststates[i] == 0) textliststates[i] = 2;
		}
		else if (textliststates[i] == 2) textliststates[i] = 0;
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_MouseDown()
{
	IsLeftButtonDown = true;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_MouseUp()
{
	// Close button
	if (CursorIntoDisplayObject(closeButton)) {
		closeButtonState = 0;
		InfoWindow_Close();
		PlayGlobalSound(S_75_I_TITLEMOV);
	}

	// List
	for (size_t i = 0; i < countof(textlist) && i < gc_listIndexes.size(); ++i)
		if (CursorIntoDisplayObject(textlist[i])) {
			if (i == lastSelectedInListIndex) break;
			textliststates[lastSelectedInListIndex] = 0;
			textliststates[i] = 1;
			lastSelectedInListIndex = i;
			PlayGlobalSound(S_75_I_TITLEMOV);
			break;
		}

	if (IsLeftButtonDown) IsLeftButtonDown = false;
}

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Open()
{
	IsInfoWindowVisible = true;
}

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Close()
{
	IsInfoWindowVisible = false;
}

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Next()
{
	if (lastSelectedInListIndex + 1 < rowInList && lastSelectedInListIndex + 1 < gc_listIndexes.size()){
		textliststates[lastSelectedInListIndex] = 0;
		textliststates[++lastSelectedInListIndex] = 1;
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	else if (lastSelectedInListIndex + 1 == rowInList && listStartFromIndex < listStartLastIndex) {
		++listStartFromIndex;
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}

// ---- th2 -------------------------------------------------------------------------------
void InfoWindow_Prev()
{
	if (lastSelectedInListIndex > 0) {
		textliststates[lastSelectedInListIndex] = 0;
		textliststates[--lastSelectedInListIndex] = 1;
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	else if (lastSelectedInListIndex == 0 && listStartFromIndex > 0) {
		--listStartFromIndex;
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
}