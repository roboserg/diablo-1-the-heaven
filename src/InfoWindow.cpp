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
	// Approach B (A8L8): clear LightSurface so UI sprites render at full brightness.
	const intptr_t lightOfs = LightSurface
		? (intptr_t)LightSurface - (intptr_t)WorkingSurface : 0;

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
			if (lightOfs) memset((char*)((intptr_t)(dst+j) + lightOfs), 0, 127);
			// (th4) Stage 2 + 3 (2026-09-05): per-ink clears for the alpha overlay
			// AND the colored-light tint — the frame art is not a rectangle (the
			// 4 corner decorations stick out), so both follow the LightSurface
			// pattern instead of the bounding-box rect (the rect over-cleared
			// 4 thin strips along the edges).
			AlphaOverlayClearRow((uchar*)dst + j, 127);
			TintClearRow((uchar*)dst + j, 127);
			i += 127;
			j += 127;
		}
		else if (src[i] == -128) j += 128;
		else if (src[i] >= 0) {
			for (k = 0; k < src[i]; ++k) dst[j + k] = src[i + k + 1];
			if (lightOfs && src[i]) memset((char*)((intptr_t)(dst+j) + lightOfs), 0, (unsigned char)src[i]);
			// (th4) Stage 2 + 3: same per-ink clears as the 127-run.
			if (src[i]) AlphaOverlayClearRow((uchar*)dst + j, (unsigned char)src[i]);
			if (src[i]) TintClearRow((uchar*)dst + j, (unsigned char)src[i]);
			j += src[i];
			i += src[i];
		}
		else if (src[i] < 0) j += -src[i];
	}
}

// ---- th2 -------------------------------------------------------------------------------
// DrawBackground_LUT — LUT-optimised transparent background
// ---------------------------------------------------------------------------
// Improved replacement for IW_TransparentBackground.  Builds a 256-entry
// lookup table once per call (negligible cost), then applies it with a
// branch-free inner loop — eliminating ~3x the per-pixel branching overhead
// of the original function-pointer-based implementation.
//
// Dungeon type mapping:
//   DT_0_TOWN:              cutColors=128, low indices -> town shade formula
//   DT_4_HELL:              cutColors=16,  low indices -> 0
//   DT_3_CAVE/5_CRYPT/6_ABYSS: cutColors=32, low indices -> 0
//   DT_1_CHURCH / default:  cutColors=0,   bit-ops only
//
// Note: IW_TransparentBackground has a defect — its cave/crypt/abyss case
//   uses `or` (C++ ||), evaluating to 1 (== DT_1_CHURCH), so those dungeon
//   types fell through to default.  This function uses explicit case labels.
// ---------------------------------------------------------------------------
void __fastcall DrawBackground_LUT(int xPosition, int yPosition, int width, int height)
{
	// --- Build LUT ---
	uchar lut[256];
	uchar cutColors = 0;
	bool  townMode  = false;

	switch (Dungeon->graphType) {
	case DT_0_TOWN:   cutColors = 128u; townMode = true; break;
	case DT_4_HELL:   cutColors =  16u;                  break;
	case DT_3_CAVE:
	case DT_5_CRYPT:
	case DT_6_ABYSS:  cutColors =  32u;                  break;
	case DT_1_CHURCH:
	default:          cutColors =   0u;                  break;
	}

	lut[0] = 0;// pixel index 0 is never modified (mirrors original `if (clrVal)` guard)
	for (int c = 1; c < 256; ++c) {
		uchar u = (uchar)c;
		if (u < cutColors) {
			lut[c] = townMode ? (uchar)(252u + (u >> 5)) : 0u;
		} else if ((u >= 128 && u <= 135) || (u >= 136 && u <= 143) ||
		           (u >= 144 && u <= 151) || (u  > 152 && u  < 159)) {
			lut[c] = (u & 0b11111000u) + ((u & 0b00000111u) >> 2) + 6;
		} else {
			lut[c] = (u & 0b11110000u) + ((u & 0b00001111u) >> 2) + 12;
		}
	}

	// --- Pre-loop bounds clamp ---
	if (yPosition < 0)                      { height += yPosition; yPosition = 0; }
	if (xPosition < 0)                      { width  += xPosition; xPosition = 0; }
	if (yPosition + height > WorkingHeight) { height  = WorkingHeight - yPosition; }
	if (xPosition + width  > WorkingWidth)  { width   = WorkingWidth  - xPosition; }
	if (width <= 0 || height <= 0) return;

	// --- Apply LUT (branch-free inner loop) ---
	for (int row = 0; row < height; ++row) {
		uchar* p = &WorkingSurface[WorkingWidth * (yPosition + row) + xPosition];
		for (int col = 0; col < width; ++col)
			p[col] = lut[p[col]];
		// Approach B (A8L8): clear LightSurface so the background renders at the
		// LUT-transformed palette brightness, without extra dungeon-shadow darkening.
		if (LightSurface) memset(LightSurface + (p - WorkingSurface), 0, width);
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_DrawLongText(int xPos, int yPos, const char* stringPtr, int fontColor)
{
	// (th4) CHI: steps by CHARACTER, not by byte, and never returns a position that
	// splits a GB2312 pair. Two things broke here for Chinese:
	//   1. ++text walked one byte, so the window could stop between a lead and a trail.
	//   2. Chinese has NO spaces, so `last` was never assigned; a line longer than
	//      textSize bytes with no '\n' inside the window returned `text` unchanged.
	//      The caller then did `firstInLine = ++lastInLine`, advancing ONE byte onto a
	//      trail byte -- every character after that decoded from the wrong boundary and
	//      the panel filled with garbage.
	// CJK breaks legally between any two characters, so when no space is available we
	// return the last character boundary that fit. Same rule the speech wrap already
	// uses (Speeches.cpp). Latin behaviour is byte-for-byte unchanged: for ASCII the
	// pair test never fires and `bound` tracks `text` exactly as `++text` used to.
	auto InfoWindow_CropText = [](const char* text, int textSize = 22) {
		// (th4) The original initialised `last = text`, so a line with no space returned
		// its own start: the caller then drew nothing for that line and crawled forward a
		// byte at a time. Starting null and falling back to the last character boundary
		// fixes it for every language -- CJK (which never has spaces) and any Latin word
		// longer than the line. Not CHI-gated: the Latin behaviour was also wrong.
		const char* last = nullptr;		// last space seen (word-break point)
		const char* bound = text;		// last valid character boundary
		for (int i = 0; i < textSize; ++i) {
			if (IsChiLead((uchar)text[0]) && IsChiTrail((uchar)text[1])) {
				text += 2; ++i;			// consume the pair atomically
				bound = text;
				continue;
			}
			if (*text == ' ') {
				last = text;
			}
			else if (*text == '\0' || *text == '\n') {
				return text;
			}
			++text;
			bound = text;
		}
		// Prefer a word break; with no space (i.e. CJK) fall back to the character
		// boundary. Never return the start -- that is what desynced the caller.
		return last ? last : bound;
	};

	int textWidth;
	int LetterID; // (th4) widened: CJK letterIDs reach 8434, a char caps at 127
	int SurfaceOffset;

	const char* firstInLine = stringPtr;
	const char* lastInLine = nullptr;

	for (int i = 0; i < 24; ++i) {
		// (th4) CHI: the old `firstInLine = ++lastInLine` assumed lastInLine always
		// points AT a 1-byte separator (space or '\n') to be consumed. That holds for
		// Latin, but a CJK line has no space to break on, so the crop returns a bare
		// character boundary instead -- and blindly skipping a byte there would eat a
		// GB2312 lead byte and desync the rest of the panel. Only step over an actual
		// separator; a boundary is already the next line's first byte.
		// (th4) MEMORY SAFETY, ALL LANGUAGES -- deliberately not CHI-gated.
		// The original `firstInLine = ++lastInLine` assumed lastInLine always points AT a
		// 1-byte separator to be consumed. It does not: when the crop stops at the end of
		// the string it returns a pointer AT the '\0', and ++ then steps PAST the
		// terminator. The i<24 loop keeps calling the crop, which scans unallocated memory
		// a byte at a time -- measured at ~660 out-of-bounds reads for one ordinary
		// English item string. It has gone unnoticed because the `firstInLine ==
		// lastInLine` guard below suppresses drawing whatever it finds and the bytes after
		// the string are normally zero, but a string near a page boundary would fault.
		// (For CJK it is worse still: skipping a byte at a bare character boundary eats a
		// GB2312 lead byte and desyncs the rest of the panel.)
		// Only step over an ACTUAL separator; a boundary is already the next line's start.
		if (lastInLine) {
			firstInLine = lastInLine;
			if (*firstInLine == ' ' || *firstInLine == '\n') ++firstInLine;
		}
		lastInLine = InfoWindow_CropText(firstInLine, 30);
		if (firstInLine == lastInLine) continue;
		textWidth = 0;

		const char* tmp = firstInLine;
		// (th4) ARABIC: shape the line and measure the GLYPHS. Byte stepping asks
		// ChiLetterID for an id it deliberately returns -1 for, so the width came out
		// wrong and the draw loop below painted nothing at all -- this is why the
		// game-changer info panel was blank.
		static AR_Glyph arG[ 512 ];
		int arN = 0;
		if( GameLanguage == LANG_AR ){
			char arBuf[ 512 ];
			size_t arLen = (size_t)( lastInLine - firstInLine );
			if( arLen > sizeof( arBuf ) - 1 ) arLen = sizeof( arBuf ) - 1;
			memcpy( arBuf, firstInLine, arLen );
			arBuf[ arLen ] = 0;
			arN = AR_Layout( arBuf, arG, 512 );
			for( int k = 0; k < arN; ++k ){
				if( arG[k].isForm )   textWidth += AR_Advance( arG[k].cell, 0 );
				else if( arG[k].raw ) textWidth += FontWidthSmall[ FontIndexSmall[ Codepage[ arG[k].raw ] ] ] + 1;
			}
		}else
		while (tmp != lastInLine){ // (th4) CHI: step by character, not byte
			int w;
			tmp += ChiNextSmall( (const uchar*)tmp, &LetterID, &w );
			textWidth += w + 1;
		}

		SurfaceOffset = YOffsetHashTable[yPos + Screen_TopBorder + 14 * i] + xPos + Screen_LeftBorder - textWidth / 2;
		if( GameLanguage == LANG_AR ){
			for( int k = 0; k < arN; ++k ){
				if( arG[k].isForm ){
					DrawLetter( SurfaceOffset, AR_FIRST_ID + arG[k].cell, fontColor );
					SurfaceOffset += AR_Advance( arG[k].cell, 0 );
				}else if( arG[k].raw > ' ' ){
					const int li = FontIndexSmall[ Codepage[ arG[k].raw ] ];
					if( li ) DrawLetter( SurfaceOffset, li, fontColor );
					SurfaceOffset += FontWidthSmall[ li ] + 1;
				}else if( arG[k].raw ){
					SurfaceOffset += FontWidthSmall[ FontIndexSmall[ Codepage[ arG[k].raw ] ] ] + 1;
				}
			}
			firstInLine = lastInLine;
		}else
		do { // (th4) CHI: step by character; the old (uint8_t) cast truncated CJK letterIDs
			int w;
			firstInLine += ChiNextSmall( (const uchar*)firstInLine, &LetterID, &w );
			if (LetterID)
				DrawLetter(SurfaceOffset, LetterID, fontColor);
			SurfaceOffset += w + 1;
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
		// Approach B (A8L8): clear LightSurface so the rect renders at full brightness.
		if (LightSurface) memset(LightSurface + (dst - WorkingSurface), 0, width);
		// (th4) Stage 2 + 3 (2026-09-05): same per-row clears for the alpha
		// overlay and the colored-light tint — the scroll indicator must never
		// be ghosted or tinted by the world.
		AlphaOverlayClearRow(dst, width);
		TintClearRow(dst, width);
		dst += WorkingWidth;
	}
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall InfoWindow_Draw()
{
	// Background (transparent)
	DrawBackground_LUT(
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
	const char* iwHead0 = LocalizedGCInfoHeader( IW_headTexts[0] ); // (th4) RU in-game GC panel header
	DrawLevelInfoText(-GetTextWidth(iwHead0) / 2 + InfoWindowRect.Left + 165, InfoWindowRect.Top + 44, iwHead0, C_3_Gold);
	// Right main text
	const char* iwHead1 = LocalizedGCInfoHeader( IW_headTexts[1] ); // (th4) RU in-game GC panel header
	DrawLevelInfoText(-GetTextWidth(iwHead1) / 2 + InfoWindowRect.Left + 475, InfoWindowRect.Top + 44, iwHead1, C_3_Gold);

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
	{
		int _gcId = gc_listIndexes[lastSelectedInListIndex + listStartFromIndex];
		textptr = LocalizedGCInfoDesc( _gcId ); // (th4) RU in-game GC description
		if( !textptr ) textptr = GC_Descriptions_InfoWindow[_gcId];
	}
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
		int _gcNameId = gc_listIndexes[index];
		textptr = LocalizedGCInfoName( _gcNameId ); // (th4) RU in-game GC name
		if( !textptr ) textptr = GC_Names_InfoWindow[_gcNameId];
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