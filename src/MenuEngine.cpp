#include "stdafx.h"
#include "ItemOverlay.h"	// (th4) the menu must sit above inventory item overlays

//----- (0041C99A) --------------------------------------------------------
void Menu_ShowPause()
{
	if( Dungeon != DUN_0_TOWN ){
		DrawRedScreen();
	}
	if( !CurrentMenuWindowPtr ){
		DarkLevel = 0;
		// (th4) Localized through the Ui table; languages without a "Pause" row fall back to English.
		const char* pauseText = L("Pause");
		Font_DrawText(Screen_LeftBorder + ScreenWidth/2 - GetTextWidthBig(pauseText)/2 , ScreenHeight/2, (char*)pauseText);
	}
}

/////////////////////////////// Menu Font //////////////////////////////////
//----- (0041C9CD) --------------------------------------------------------
char __fastcall Font_DrawText(int aXpos, int aYpos, char* aText)
{
	unsigned char CurChar;   // al@1
	int Ypos;                // ebp@1
	unsigned char* CharPtr;  // edi@1
	int Xpos;                // esi@1
	unsigned char CharIndex; // bl@2
	CharPtr = (unsigned char*)aText;
	Ypos = aYpos;
	Xpos = aXpos;
	// (th4) ARABIC: shape and reorder the whole string, then paint, and return. The
	// loop below walks bytes and would hand ChiLetterID's -1 straight to
	// Font_DrawChar. Big-font cells are fixed width like the CJK path; the engine
	// ring is off here because the Arabic bigtgold already carries a baked rim.
	if( GameLanguage == LANG_AR ){
		static AR_Glyph g[ 512 ];
		const int gn = AR_Layout( aText, g, 512 );
		for( int k = 0; k < gn; ++k ){
			if( g[k].isForm ){
				Font_DrawChar( Xpos, Ypos, (char*)FontSpriteBig, AR_FIRST_ID + g[k].cell, AR_CELL_W_BIG );
				Xpos += AR_Advance( g[k].cell, 2 );
			}else if( g[k].raw ){
				const unsigned char ci = FontIndexBigPtr[ Codepage[ g[k].raw ] ];
				// (th4) whitespace ADVANCES but never paints. FontIndexBigPtr[Codepage[' ']]
				// is not zero, and in the Arabic bigtgold that frame is not blank, so every
				// space came out as a stray dot between the words.
				if( ci && g[k].raw > ' ' ) Font_DrawChar( Xpos, Ypos, (char*)FontSpriteBig, ci, 46 );
				Xpos += FontWidthBigPtr[ ci ] + 2;
			}
		}
		return gn ? (char)1 : (char)0;
	}
	// Loop invariant: at the top of each iteration CharPtr points AT CurChar, so
	// CharPtr[1] is the following byte. (th4) GB2312 is double-byte and cannot be
	// expressed in the 256-entry FontIndex tables, so pairs are detected here and
	// resolved straight to a CJK frame -- the same way Tooltip.cpp does for the small
	// font. IsChiLead() is false for every non-CHI language, so this is a no-op there
	// and the original path below runs byte-for-byte unchanged.
	for( CurChar = *aText; *CharPtr; CurChar = *CharPtr ){
		if( IsChiLead( CurChar ) && IsChiTrail( CharPtr[1] ) ){
			// (th4) JAPANESE AND CHINESE ONLY -- Ibragim's call after looking at all
			// three in game. This branch is shared by CHI, KO and JA (IsChiLead() is
			// true for each), but KOREAN IS DELIBERATELY EXCLUDED: its bigtgold.cel
			// already carries a baked rim that reads correctly on its own, and adding
			// the engine ring on top only thickens text that already looks right.
			// So this is an explicit language test, not a ride on the CJK branch.
			// Latin and Cyrillic never reach this code at all.
			// (th4) HINDI joins JA/CHI here: its bigtgold carries NO baked rim, so the
			// engine draws the ring. Korean stays out because its art already has one.
			const int bigOutline = is( GameLanguage, LANG_JA, LANG_CHI, LANG_HIN ) ? 0 : -1;
			Font_DrawChar(Xpos, Ypos, (char*)FontSpriteBig, ChiLetterID( CurChar, CharPtr[1] ), CHI_CELL_W_BIG, bigOutline);
			Xpos += ( GameLanguage == LANG_HIN   // (th4) see HindiText.cpp
			        ? HIN_Advance( ChiLetterID( CurChar, CharPtr[1] ) - HIN_FIRST_ID, 2 )
			        : CHI_CELL_W_BIG ) + 2;
			CharPtr += 2;
			continue; // `continue` still runs the CurChar = *CharPtr step
		}
		++CharPtr;
		CharIndex = FontIndexBigPtr[Codepage[CurChar]];
		if( CharIndex )
			Font_DrawChar(Xpos, Ypos, (char*)FontSpriteBig, CharIndex, 46);
		Xpos += FontWidthBigPtr[CharIndex] + 2;
	}
	return CurChar;
}

//----- (0041CA26) --------------------------------------------------------
void FreeMenuGraphics()
{
	FreeMemZero(Hf_logo3CELPtr);
	FreeMemZero(FontSpriteBig);
	FreeMemZero(PentSpinCELPtr);
	FreeMemZero(OptionCELPtr);
	FreeMemZero(OptbarCELPtr);
}

//----- (0041CA80) --------------------------------------------------------
void Menu_Init()
{
	PentagramFrameCount = 1;
	LogoFrameCount = 1;
	CurrentMenuWindowPtr = 0;
	CurrentMenuItem = 0;
	MainMenuHandlerPtr = 0;
	CurMenuSize = 0;
	IsMenuVisible = false;
	Hf_logo3CELPtr = (char*)LoadFile("Data\\th_logo2.CEL"); // "Data\\hf_logo3.CEL"
	FontSpriteBig = (char*)LoadFile("Data\\BigTGold.CEL");
	PentSpinCELPtr = (char*)LoadFile("Data\\PentSpin.CEL");
	OptionCELPtr = (char*)LoadFile("Data\\option.CEL");
	OptbarCELPtr = (char*)LoadFile("Data\\optbar.CEL");
}

//----- (0041CAFF) --------------------------------------------------------
bool IsGameMenuActive()
{
	return CurrentMenuWindowPtr != 0;
}

//----- (0041CB0B) --------------------------------------------------------
void __fastcall Menu_SetupMenuWindow(Menu *a1, void (__cdecl *aMenuHandlerPtr)())
{
	Menu* v2; // eax@1
	int v3;         // ecx@3
	int v4;         // edx@4
	int s;          // [sp+0h] [bp+0h]@7

	GamePaused = 0;
	IsMenuVisible = false;
	v2 = a1;
	MainMenuHandlerPtr = aMenuHandlerPtr;
	CurrentMenuWindowPtr = a1;
	if( aMenuHandlerPtr ){
		((int(*)())aMenuHandlerPtr)();
		v2 = CurrentMenuWindowPtr;
	}
	v3 = 0;
	CurMenuSize = 0;
	if( v2 ){
		v4 = (int)&v2->FuncPtr;
		while( *(uint*)v4 ){
			++v3;
			v4 += 12;
			CurMenuSize = v3;
		}
	}
	s = 1;
	CurrentMenuItem = &v2[v3 - 1];
	SelectMenuItem(1);
}

//----- (0041CB65) --------------------------------------------------------
void __fastcall SelectMenuItem(int aDirection)
{
	Menu* SelectedMenuItem; // eax@1
	int v2;                       // edi@2
	int v3;                       // ebx@11
	SelectedMenuItem = CurrentMenuItem;
	if( CurrentMenuItem ){
		IsMenuVisible = false;
		v2 = CurMenuSize - 1;
		if( CurMenuSize ){
			do{
				if( aDirection ){
					++SelectedMenuItem;
					CurrentMenuItem = SelectedMenuItem;
					if( SelectedMenuItem->FuncPtr )
						goto LABEL_10;
					SelectedMenuItem = CurrentMenuWindowPtr;
				}else{
					if( SelectedMenuItem == CurrentMenuWindowPtr )
						SelectedMenuItem = &CurrentMenuWindowPtr[CurMenuSize];
					--SelectedMenuItem;
				}
				CurrentMenuItem = SelectedMenuItem;
LABEL_10:
				if( SelectedMenuItem->Flags & MENU_ENABLED ){
					if( v2 )
						PlayGlobalSound(S_75_I_TITLEMOV);
					return;
				}
				v3 = v2--;
			}while( v3 );
		}
	}
}

//----- (0041CBD2) --------------------------------------------------------
void Menu_Draw()
{
	if( CurrentMenuWindowPtr ){
		// (th4) The menu is 8-bit ink in WorkingSurface; the true-colour item overlay is
		// drawn afterwards, so an item lying in the inventory behind the menu covered the
		// logo, the text and the pentagram.
		//
		// Same policy the tooltips use: claim the band the menu occupies and snapshot it
		// BEFORE drawing into it. Every pixel the menu then changes counts as ink and is
		// re-drawn over the icons. Factor 1.0f, because the menu is not a darkened box --
		// the icons keep their brightness and only the ink lands on top.
		//
		// The item on the CURSOR is exempt: it is drawn after this patch pass, so dragging
		// something over the menu still shows it above the text. See ItemOverlay.cpp.
		{
			// Exactly as wide as the menu actually draws, and no wider: claiming the full
			// screen width would swallow the inventory panel, which is drawn AFTER this
			// point, and its own 8-bit art would then be painted back over the item icons.
			//
			// Widest line wins, and the two pentagrams hang off either end of it -- the left
			// one 54px before the text starts, the right one 4px after it ends, both 48
			// wide. The logo is 430, so that is the floor.
			int widest = 430;
			for( Menu* i = CurrentMenuWindowPtr; i->FuncPtr; ++i ){
				const int w = (int)Menu_GetItemTextWidth(i);
				if( w > widest ) widest = w;
			}
			const int span = widest + 54 + 48 + 4 + 48;
			const int left = ((int)WorkingWidth - widest) / 2 - 54 - (int)Screen_LeftBorder;
			const int top    = 90 + (ScreenHeight - GUI_Height) / 2;
			const int bottom = 160 + 45 * 7 + (ScreenHeight - GUI_Height) / 2;
			ItemOverlay_QueueDimRect(left, top, span, bottom - top, 1.0f);
			ItemOverlay_SnapshotDimRect();
		}
		if( MainMenuHandlerPtr )
			MainMenuHandlerPtr();
		i64 v0 = GetTickCountTh(); // (rt) this timer doesn't affect gameplay, not intercepted
		i64 v1 = v0;
        if( OldTickCount2 == 0 ){
            OldTickCount2 = v0;
        }
		if( IsMainDraw && v0 - OldTickCount2 > 25_ms ){
			++LogoFrameCount;
			if( (unsigned __int8)LogoFrameCount > 16u )
				LogoFrameCount = 1;
			OldTickCount2 = v0;
		}
		Surface_DrawCEL(Screen_LeftBorder + 21 + (ScreenWidth-430)/2, Screen_TopBorder + 102 + (ScreenHeight - GUI_Height)/2, Hf_logo3CELPtr, (unsigned __int8)LogoFrameCount, 430);
		int y = Screen_TopBorder + 160 + (ScreenHeight - GUI_Height)/2; // vertical center
		for( Menu* i = CurrentMenuWindowPtr; i->FuncPtr; y += 45 ){
			Menu_DrawItem(i, y);
			++i;
 		}
        if( OldTickCount3 == 0 ){
            OldTickCount3 = v1;
        }
		if( IsMainDraw && v1 - OldTickCount3 > 25_ms ){
			++PentagramFrameCount;
			if( PentagramFrameCount == 9 )
				PentagramFrameCount = 1;
			OldTickCount3 = v1;
		}
	}
}

//----- (0041CC93) --------------------------------------------------------
void __fastcall Menu_DrawItem(Menu* itemPtr, int itemYPos)
{
	uint itemTextWidth = Menu_GetItemTextWidth((struct Menu*)itemPtr);
	if( itemPtr->Flags & MENU_SLIDER ){ 
		uint HalfAnd80 = itemTextWidth / 2 + Screen_LeftBorder + 16; // (rt) itemTextWidth = 490 here, GUI_Width - 490 = 150; 150/2 = 75 margin on each side
		Surface_DrawCEL(HalfAnd80 + (ScreenWidth-GUI_Width)/2, itemYPos - 10, OptbarCELPtr, 1, 287);
		uint max_val = (itemPtr->Flags >> 12) & 0x00000FFF;
		if( max_val < 2 ){
			max_val = 2;
		}
		uint relative_cur_val = ((itemPtr->Flags & 0x00000FFF) << 8) / max_val;
		uint halfAnd82 = itemTextWidth / 2 + Screen_LeftBorder + 18;
		uint offset = relative_cur_val;
		Menu_DrawItemBackground(halfAnd82+(ScreenWidth-GUI_Width)/2, itemYPos - 12 , relative_cur_val + 13, 28);
		Surface_DrawCEL(offset + 2 + HalfAnd80+(ScreenWidth-GUI_Width)/2, itemYPos - 12, OptionCELPtr, 1, 27);
	}
	int textStartPos = WorkingWidth / 2 - itemTextWidth / 2;
	// Check whether menuitem enabled or not
	int negFlaf;
	if( itemPtr->Flags & MENU_ENABLED || itemPtr->Flags & MENU_STATIC ){
		negFlaf = -1;
		LOBYTE_IDA(negFlaf) = negFlaf & 0xF1;
	}else{
		negFlaf = 0;
	}
	DarkLevel = negFlaf + 0xF;
	Font_DrawText(textStartPos, itemYPos, itemPtr->NamePtr);
	if( itemPtr == CurrentMenuItem ){
		int pentagramYPos = itemYPos + 1;
		Surface_DrawCEL(textStartPos - 54, pentagramYPos, PentSpinCELPtr, (unsigned __int8)PentagramFrameCount, 48);
		Surface_DrawCEL(itemTextWidth + 4 + textStartPos, pentagramYPos, PentSpinCELPtr, (unsigned __int8)PentagramFrameCount, 48);
	}
}

//----- (0041CD87) --------------------------------------------------------
void* __fastcall Menu_DrawItemBackground(int a1, int a2, size_t Size, int a4)
{
	void* result; // eax@1
	void* v5;     // esi@1
	int v6;       // edi@2
	v5 = &WorkingSurface[a1] + YOffsetHashTable[a2];
	result = (void*)(a4 - 1);
	if( a4 ){
		v6 = a4;
		do{
			result = memset(v5, 205, Size);
			// Approach B (A8L8): clear LightSurface so slider bar renders at full brightness.
			if( LightSurface ) memset( LightSurface + ((uchar*)v5 - WorkingSurface), 0, Size );
			// (th4) Stage 2 (2026-09-04): same for the alpha overlay under the
			// slider background row — menu UI must not be ghosted by
			// occluded-sprite RGB.
			AlphaOverlayClearRow((uchar*)v5, (int)Size);
			// (th4) Stage 3 (2026-09-05): same for the colored-light tint — the
			// slider bar row must not be multiplied by the world's light color.
			TintClearRow((uchar*)v5, (int)Size);
			v5 = (char*)v5 - WorkingWidth;
			--v6;
		}while( v6 );
	}
	return result;
}

//----- (0041CDC6) --------------------------------------------------------
int __fastcall Menu_GetItemTextWidth(struct Menu* aMenu)
{
	int result;         // eax@2
	void* v2;           // eax@3
	int v3;             // edx@3
	unsigned __int8 v4; // cl@4
	if( aMenu->Flags & MENU_SLIDER ){
		result = 490;
	}else{
		v2 = aMenu->NamePtr;
		v3 = 0;
		if( GameLanguage == LANG_AR ){
			// (th4) THIS is what threw the in-game menu off centre. With no Arabic arm the
			// loop below charges a full CJK cell per unit while Font_DrawText advances by
			// the PROPORTIONAL AR_Advance, so the width came out several times too large:
			// textStartPos = WorkingWidth/2 - width/2 shoved every row left -- further the
			// longer the row -- and flung the two pentagrams out to the screen edges.
			// Measure exactly what Font_DrawText draws.
			static AR_Glyph mg[ 512 ];
			const int mn = AR_Layout( (const char*)aMenu->NamePtr, mg, 512 );
			for( int k = 0; k < mn; ++k ){
				if( mg[k].isForm )   v3 += AR_Advance( mg[k].cell, 2 );
				else if( mg[k].raw ) v3 += FontWidthBigPtr[ FontIndexBigPtr[ Codepage[ mg[k].raw ] ] ] + 2;
			}
			return v3 > 2 ? (uint)( v3 - 2 ) : 0u;
		}
		while( 1 ){
			v4 = *(uchar*)v2;
			if( !*(uchar*)v2 )
				break;
			if( IsChiLead( v4 ) && IsChiTrail( ((uchar*)v2)[1] ) ){ // (th4) GB2312 pair
				// (th4) read the trail BEFORE v2 advances -- must match Font_DrawText above.
				const uchar hinTrail = ((uchar*)v2)[1];
				v2 = (char*)v2 + 2;
				v3 += ( GameLanguage == LANG_HIN
				      ? HIN_Advance( ChiLetterID( v4, hinTrail ) - HIN_FIRST_ID, 2 )
				      : CHI_CELL_W_BIG ) + 2;
				continue;
			}
			v2 = (char*)v2 + 1;
			v3 += FontWidthBigPtr[FontIndexBigPtr[Codepage[v4]]] + 2;
		}
		result = v3 - 2;
	}
	return result;
}

//----- (0041CE02) --------------------------------------------------------
int __fastcall Menu_DispatchKey(int KeyPressed)
{
	if( !CurrentMenuWindowPtr )
		return 0;

	switch( KeyPressed ){
	case VK_RETURN:
		if( CurrentMenuItem->Flags & MENU_ENABLED ){
			PlayGlobalSound(75);
			((int(*)(uint))CurrentMenuItem->FuncPtr)(1);
		}
		return 1;
	case VK_ESCAPE:
		PlayGlobalSound(75);
		Menu_SetupMenuWindow(0, 0);
		return 1;
    case VK_F12:
        if( CurrentMenuWindowPtr == MurderConfirmationMenu ){
            PlayGlobalSound(S_1216_PIECE_1);
            MurderHeroInGame();
            return 1;
        }
        break;
	case VK_SPACE:
		return 0;
	case VK_LEFT:
	case VK_RIGHT:
		Menu_RegulateItem(KeyPressed == VK_RIGHT ? 1 : 0);
		return 1;
	case VK_UP:
	case VK_DOWN:
		SelectMenuItem(KeyPressed == VK_UP ? 0 : 1);
		return 1;
	}
	return 1;
}

//----- (0041CE7E) --------------------------------------------------------
void __fastcall Menu_RegulateItem(int aDirection)
{
	unsigned int Flags;   // edx@1
	unsigned int cur_old; // eax@2
	unsigned int cur_new; // eax@4
	Flags = CurrentMenuItem->Flags;
	if( CurrentMenuItem->Flags & MENU_SLIDER ){
		cur_old = Flags & 0x00000FFF;
		if( aDirection ){
			if( cur_old == ((Flags >> 12) & 0x00000FFF) ){
				return;
			}
			cur_new = cur_old + 1;
		}else{
			if( !cur_old ){
				return;
			}
			cur_new = cur_old - 1;
		}
		CurrentMenuItem->Flags = (Flags & 0xFFFFF000) | cur_new;
		CurrentMenuItem->FuncPtr(0, CurrentMenuItem->Flags);
	}
}

// (rt) there is a jmp here from the jmp-call function 0040A3F6 j_Menu_ItemMouseClick
// (rt) but for the comparison this doesn't matter, since the entry bytes into the full function are substituted
//----- (0041CECE) --------------------------------------------------------
int __fastcall Menu_ItemMouseClick(int a1)
{
	int result;      // eax@2
	unsigned int v2; // edx@3
	unsigned int v3; // [sp+0h] [bp-4h]@1
	v3 = a1;
	if( IsMenuVisible ){
		Menu_SetNewRegulatorPos(&v3);
		v2 = v3 * ((CurrentMenuItem->Flags >> 12) & 0x00000FFF) % 256;
		v3 = v3 * ((CurrentMenuItem->Flags >> 12) & 0x00000FFF) / 256;
		CurrentMenuItem->Flags &= 0xFFFFF000u;
		CurrentMenuItem->Flags |= v3;
		CurrentMenuItem->FuncPtr(0, v2);
		result = 1;
	}else{
		result = 0;
	}
	return result;
}

//----- (0041CF26) --------------------------------------------------------
bool __fastcall Menu_SetNewRegulatorPos(uint* a1)
{
	const int MAX_REGULATOR_VALUE = 264;
	const int REGULATOR_LEFT = 265;
	const int REGULATOR_WIDTH = 285;

	int menuRegulatorLeft = REGULATOR_LEFT + (ScreenWidth-GUI_Width)/2;
	int menuRegulatorRight = menuRegulatorLeft + REGULATOR_WIDTH;
	int newValue = CursorX - menuRegulatorLeft;
	LimitToRange(newValue, 0, MAX_REGULATOR_VALUE);
	*a1 = newValue;
	if( CursorX >= menuRegulatorLeft && CursorY <= menuRegulatorRight ){
		return true;
	}else{
		return false;
	}
}

//----- (0041CF55) --------------------------------------------------------
int __fastcall Menu_CheckMouseClick(bool mouseButtonState)
{
	if( mouseButtonState == MOUSE_BUTTON_UP ){
		if( IsMenuVisible == false ){
			return false;
		}
		IsMenuVisible = false;
		return true;
	}
	// (rt) everything below runs only on MOUSE_BUTTON_DOWN

	int menuTop = 117 + (ScreenHeight - GUI_Height)/2; // (rt) Possibly the other way round; not certain whether screen height 0 is at the top as usual or at the bottom
	int menuDown = 352 + (ScreenHeight - GUI_Height)/2 + (39 + 45) * (CurrentMenuWindowPtr == OptionsMenu);
	int screenWidthCenter = ScreenWidth/2;
	uint cursorAbsDistanceToWidthCentr = abs(screenWidthCenter - CursorX);
	const uchar menuItemHeight = 45;

	if( !CurrentMenuWindowPtr || CursorY >= menuDown ){
		return false;
	}

	if( CursorY < menuTop ){
		return true;
	}
	uchar menuItemUnderCursorIndex = (CursorY - menuTop) / menuItemHeight;
	if( menuItemUnderCursorIndex >= CurMenuSize ){
		return true;
	}
	Menu& menuItemUnderCursor = CurrentMenuWindowPtr[menuItemUnderCursorIndex];
	if( !(menuItemUnderCursor.Flags & MENU_ENABLED) ){// 1000 0000
		return true;
	}
	if( cursorAbsDistanceToWidthCentr >= ((uint)Menu_GetItemTextWidth(&menuItemUnderCursor) >> 1)){
		return true;
	}
	CurrentMenuItem = &menuItemUnderCursor;
	PlayGlobalSound(75);
	if( menuItemUnderCursor.Flags & MENU_SLIDER ){// 0100 0000
		uint someFlag = 0;
		IsMenuVisible = Menu_SetNewRegulatorPos(&someFlag);
		Menu_ItemMouseClick(someFlag); ////////////////////////
	}else{
		//menuItemUnderCursor.FuncPtr(1);
		((int(*)(int))CurrentMenuItem->FuncPtr)(1); // (rt) The arguments passed are inconsistent
		// (rt) and besides, the arguments aren't used by any function of the menu struct.
		// (rt) at least no such functions were found.
	}
	return true;
}

//----- (0041D007) --------------------------------------------------------
void __fastcall Menu_SetItemAvailability(Menu* menuItem, int isActivated)
{
	if( isActivated ){
		menuItem->Flags |= MENU_ENABLED;
	}else{
		menuItem->Flags &= ~MENU_ENABLED;
	}
}

//----- (0041D015) --------------------------------------------------------
unsigned int __fastcall Menu_SetSlider(Menu* aMenu, int aMin, int aMax, int aCur)
{
	int CurMax = int((aMenu->Flags >> 12) & 0x00000FFF);
	if( CurMax < 2 )
		CurMax = 2;
	aMenu->Flags &= 0xFFFFF000;
	aMenu->Flags |= (CurMax * (aCur - aMin) + (aMax - aMin - 1) / 2) / (aMax - aMin);
	return aMenu->Flags;
}

//----- (0041D057) --------------------------------------------------------
int __fastcall Menu_GetRegulatorPos(Menu* menuItem, int min, int max)
{
	int v3; // ecx@1
	int v4; // esi@1
	int v6; // eax@1
	v6 = (menuItem->Flags >> 12) & 0xFFF;
	v3 = menuItem->Flags & 0xFFF;
	v4 = v6;
	if( v6 < 2 )
		v4 = 2;
	return min + (v3*(max - min) + (v4 - 1) / 2) / v4;
}

//----- (0041D094) --------------------------------------------------------
void __fastcall Menu_SetSliderTicks(Menu* menuItem, __int16 pos)
{

	menuItem->Flags = menuItem->Flags & 0xFF000FFF | ((pos & 0xFFF) << 12);
}
