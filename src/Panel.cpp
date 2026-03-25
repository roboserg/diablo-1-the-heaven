#include "stdafx.h"

const int DAMAGED_ITEMS_PICS_WIDTH = 32;
const int DAMAGED_ITEMS_PICS_HEIGHT = 32;
const int DAMAGED_ITEMS_PICS_INDENT = 8;
const int DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT = 12;

// th2
int InfoLineYOffsets[5][5] = {// переработаный ориганальный массив офсетов. меняется только y
	{0,									0,0,0,0},
	{-12,	12,							0,0,0},
	{-18,	0,		18,					0,0},
	{-22,	-7,		7,		22,			0},
	{-24,	-12,	0,		12,		23,	},
};

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// оптимизация
__forceinline bool CursorIntoSpellIcon()
{
	int spellIconLeft = 565 + (ScreenWidth - GUI_Width) / 2;
	int spellIconUp = 416 + ScreenHeight - GUI_Height;
	int spellIconWidth = 56;
	int spellIconHeight = 56;
	int spellIconRight = spellIconLeft + spellIconWidth;
	int spellIconDown = spellIconUp + spellIconHeight;

	return CursorX >= spellIconLeft && CursorX < spellIconRight && CursorY >= spellIconUp && CursorY < spellIconDown;
}

// оптимизация
bool CursorIntoBelt()
{
	int const beltLeft = 191 + (ScreenWidth - GUI_Width)/2;
	int const beltWidth = 248;
	int const beltTop = 353 + ScreenHeight - GUI_Height;
	int const beltHeight = 31;
	int x = CursorX;
	int y = CursorY;
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		x += (CurCursorWidth / 2); // For items we consider cursor x as item's center
		y += (CurCursorHeight / 2);
	}
	return x >= beltLeft && x <= (beltLeft + beltWidth) && y >= beltTop && y <= (beltTop + beltHeight);
}

bool CursorIntoMainPanelButton(Button& b)
{
	int buttonX = b.x + (ScreenWidth - GUI_Width) / 2;
	int buttonY = b.y + ScreenHeight - GUI_Height;
	int buttonWidth = b.w;
	int buttonHeight = b.h;
	int buttonRight = buttonX + buttonWidth;
	int buttonDown = buttonY + buttonHeight;

	return CursorX >= buttonX && CursorX <= buttonRight && CursorY >= buttonY && CursorY <= buttonDown;
}

// оптимизация
bool CursorIntoLevelUpButton()
{
	int buttonLeft = 40 + (ScreenWidth - GUI_Width) / 2;
	int buttonUp = 313 + ScreenHeight - GUI_Height;
	int buttonWidth = 41;
	int buttonHeight = 22;
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

// оптимизация
__forceinline bool CursorIntoTalkPanelButton(int index)
{
	int buttonLeft = 172 + (ScreenWidth - GUI_Width) / 2;
	int buttonUp = 421 + index * 18 + ScreenHeight - GUI_Height;
	int buttonWidth = 60;
	int buttonHeight = 18;// 54 Это сразу 3. А каждая 18
	int buttonRight = buttonLeft + buttonWidth;
	int buttonDown = buttonUp + buttonHeight;

	return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

// оптимизация
__forceinline void FormatedOutputPlayerSkillString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf( InfoPanelBuffer, "%s Skill", Spells[ currentSpellNumber ].SkillNamePtr );
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf( InfoPanelHeader, "%s Skill", Spells[ currentSpellNumber ].SkillNamePtr );
	}
}

//----- (th2) -------------------------------------------------------------
int FindBodySlotWithSpell(int playerIndex, int spellIndex, int startItem /*= 0*/)
{
	const Player& player = Players[playerIndex];
	int itemWithSpellIndex = -1;
	for( int i = startItem; i < IS_Inventory; ++i ){
		const Item& bodyItem = player.OnBodySlots[i];
		if( bodyItem.SpellIndex == spellIndex
		 && bodyItem.ItemCode != IC_M1_NONE
		 && bodyItem.IsReqMet
		 && bodyItem.BaseCharges ){
			itemWithSpellIndex = i;
			if( bodyItem.CurCharges != 0 ){ // If item with cur charged found, return it
				return i;
			}
		}
	}
	return itemWithSpellIndex; // return Item with spell index but without charges if found
}

//----- (th2) -------------------------------------------------------------
int SumBodySlotSpellCharges(int playerIndex, int spellIndex)
{
	const Player& player = Players[CurrentPlayerIndex];
	int sumCharges = 0, slotIndex = -1;
	do{
		slotIndex = FindBodySlotWithSpell( CurrentPlayerIndex, spellIndex, slotIndex + 1 );
		if( slotIndex != -1 ) sumCharges += player.OnBodySlots[slotIndex].CurCharges;
	}while( slotIndex != -1 );
	return sumCharges;		
}

//----- (th2) -------------------------------------------------------------
void FormatedOutputStaffChargesString( int currentSpellNumber, int help = 0 )
{
	int sumCharges = SumBodySlotSpellCharges( CurrentPlayerIndex, currentSpellNumber );
	if( sumCharges > 0 ){
		strcpy(InfoPanelBuffer, getSpellName(currentSpellNumber));
		InfoPanel_AddLine(InfoPanelBuffer, true);
		sprintf(InfoPanelBuffer, "%i Charge%s", sumCharges, sumCharges == 1 ? "" : "s");
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}
}

// оптимизация
__forceinline void FormatedOutputSpellString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf(InfoPanelBuffer, "%s Spell", getSpellName( currentSpellNumber ));
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf( InfoPanelHeader, "%s Spell", getSpellName(  currentSpellNumber  ) );
	}
	if( SelectedSpellIndex == PS_31_HOLY_BOLT ){
		sprintf( InfoPanelBuffer, "Damages undead only" );
		InfoPanel_AddLine( InfoPanelBuffer, 1 );
	}
	int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, currentSpellNumber);
	if( spellLevel == 0 ){
		sprintf(InfoPanelBuffer, "Spell Level 0 - Unusable");
	}else{
		sprintf(InfoPanelBuffer, "Spell Level %i", spellLevel);
	}
	InfoPanel_AddLine(InfoPanelBuffer, true);
}

// оптимизация
__forceinline void FormatedOutputRelicString(int currentSpellNumber, int help = 0 )
{
	if( help ){
		sprintf(InfoPanelBuffer, RelixToScrolls || GameMode == GM_CLASSIC ? "Scroll of %s" : "Relict of %s", getSpellName( currentSpellNumber ));
		InfoPanel_AddLine(InfoPanelBuffer, true);
	}else{
		sprintf(InfoPanelHeader, RelixToScrolls || GameMode == GM_CLASSIC ? "Scroll of %s" : "Relict of %s", getSpellName(  currentSpellNumber  ) );
	}
	Player& player = Players[CurrentPlayerIndex];
	int numberOfRelicWithCurrentSpell = GetNumberOfRelicWithCurrentSpell(currentSpellNumber);
	if( numberOfRelicWithCurrentSpell == 1 ){
		strcpy(InfoPanelBuffer, RelixToScrolls || GameMode == GM_CLASSIC ? "1 Scroll" : "1 Relict");
	}else{
		sprintf(InfoPanelBuffer, RelixToScrolls || GameMode == GM_CLASSIC ? "%i Scrolls" : "%i Relicts", numberOfRelicWithCurrentSpell);
	}
	InfoPanel_AddLine(InfoPanelBuffer, true);
}

// DrawLetter function draws single letter in dialog windows/barter/infopanel etc...
// The fontColor wasn't size but color... we should rename all function headers using that...
// 
// I've extracted the GetLetterColor function to get proper coloring of the given pixel
// in letter based on the color code. I've also added 2 more just for fun (you can draw
// yellow and orange text ;P). Color is made up from a "grayscale" mask + adequate shift
// in the pallette (white = no shift)
// 
// In the future, there should be a special pallette just for handling text, where we can 
// specify up to 16 (16 * 16 color grades). Now it's using default town pallette (i think)
// and that's kinda messy and only have few "usable" colors 
// (see http://fuller.gjgt.sk/img/th/fontcolor.png)
// Fuco

// оптимизация
__forceinline char GetLetterColor(uchar mask, int colorIndex)
{
	switch (colorIndex) {
	case C_0_White:	break;
	case C_1_Blue:
		if (mask > 253) {
			mask = 191;
		}
		else if (mask >= 240) {
			mask -= 62;
		}
		break;
	case C_2_Red:   if (mask >= 240) { mask -= 16; }	            break; // was 16
	case C_3_Gold:  if (mask >= 240) { 
		if (mask >= 254) {
			mask = 207;
		}
		else {
			mask -= 46;
		}
	}
		break;
	case C_4_Orange:if (mask >= 240) {
		mask -= 32;
	}
					break;
	case C_5_Yellow:if (mask > 251) {
		mask = 151;
	}
					else if (mask >= 240) {
		mask = 145 + ((mask - 240) >> 1);
	}
		break;
	case C_6_Brownish:if (mask >= 240) { mask -= 49; } break; // if (mask >= 240) { mask = 78 + (mask >> 1); } break;
	case C_7_Grey:if (mask >= 240) { mask -= -3; } break; // if (mask >= 240) { mask = 55 + (mask >> 2); } break;
	case C_8_Pink:if (mask >= 240) { mask -= 79; } break;
	case C_9_RedGold:if (mask >= 240) { mask -= 96; } break;
	case C_10_Enchanted: 
		if (mask >= 254) {
			mask = 190;
		}
		else if (mask >= 240) {
			mask -= 65;
		}
		break;
	case C_11_Quest:
		if (mask > 251) {
			mask = 151;
		}
		else if (mask >= 240) {
			mask = 145 + ((mask - 240) >> 1);
		}
		break;
	}
	return mask;
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (00403A2C) --------------------------------------------------------
void __fastcall DrawSpellIcon(int surfaceX, int surfaceY, char* iconCel, int iconIndex, int size, int spellType /* = 0 */ )
{
	//#if CHEATS
	if( GameMode == GM_CLASSIC){
		if(      iconCel == Data_SpelIconCEL && iconIndex > 61 ) iconCel = Data_SpelIconCEL_2;
		else if( iconCel == Data_SpellI2CEL  && iconIndex > 52 ) iconCel = Data_SpellI2CEL_2;
	}
	//#endif
	
	uchar* trans/* = spellType == SO_4_DISABLE ? IconColorsDisabled : IconColors*/;

	switch (spellType) {
		case SO_0_SKILL: trans = IconColorsSkill; break;
		case SO_1_SPELL: trans = IconColorsSpell; break;
		case SO_2_RELIC: trans = IconColorsRelict; break;
		case SO_3_EQUIPED_ITEM: trans = IconColorsCharges; break;
		case SO_4_NONE: trans = IconColorsDisabled; break;
		case SO_5_TRAIT: trans = IconColorsTrait; break;
	}
	
	uchar* dst = &WorkingSurface[ surfaceX ] + YOffsetHashTable[ surfaceY ];
	uint* frame = (uint*)iconCel;
	char* src = iconCel + frame[iconIndex];
	uint celSize = frame[iconIndex + 1] - frame[iconIndex];
	
	// most copy-paste from CopyPCX
	char* cel = src;
	if( *cel == 10 ){
		cel += 11;
	}
	char* end = &src[ celSize ];
	int widLeft = 0, len = 0;
	do{
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
		widLeft = size;
		do{
			while( 1 ){
				len = *cel++;
				if( len > 0 ){
					break;
				}
				len = -len;
				dst += len;
				widLeft -= len;
				if( widLeft <= 0 ){
					goto next_line;
				}
			}
			widLeft -= len;
			while( len-- ){ *dst++ = trans[(uchar)*cel++]; }
		} while( widLeft > 0 );
	next_line: dst -= size - widLeft + WorkingWidth;
	} while( cel < end );
}

//----- (00403AD6) --------------------------------------------------------
void __fastcall DrawSpellColor(int spellType)
{
	int v1; // eax@2
	int v2; // eax@4
	int v3; // eax@10
	unsigned __int8 *v4; // ecx@11
	int v5; // eax@13
	int v6; // eax@16
	unsigned __int8 *v7; // ecx@17
	int v8; // eax@19
	unsigned __int8 *v9; // ecx@20

	if( !spellType ){ // SO_0_SKILL
		v1 = 0;
		do{
			IconColors[ v1 ] = v1;
			++v1;
		}while( v1 < 128 );
	}
	v2 = 128;
	do{
		IconColors[ v2 ] = v2;
		++v2;
	}while( v2 < 256 );
	IconColors[ 255 ] = 0;
	switch( spellType ){
	case SO_1_SPELL:
		IconColors[ 144 ] = 177;
		IconColors[ 145 ] = 179;
		IconColors[ 146 ] = 181;
		v8 = 176;
		do{
			v9 = &IconColors[ v8 + 16 ];
			*(v9 - 32) = v8;
			*v9 = v8;
			v9[ 16 ] = v8++;
		}while( v8 < 192 );
		break;
	case SO_2_RELIC:
		IconColors[ 144 ] = 161;
		IconColors[ 145 ] = 163;
		IconColors[ 146 ] = 165;
		v6 = 160;
		do{
			v7 = &IconColors[ v6 + 48 ];
			*(v7 - 16) = v6;
			*v7 = v6++;
		}while( v6 < 176 );
		break;
	case SO_3_EQUIPED_ITEM:
		IconColors[ 144 ] = 209;
		IconColors[ 145 ] = 211;
		IconColors[ 146 ] = 213;
		v5 = 208;
		do{
			IconColors[ v5 - 48 ] = v5;
			IconColors[ v5 - 16 ] = v5;
			++v5;
		}while( v5 < 224 );
		break;
	case SO_4_NONE:
		IconColors[ 144 ] = 241;
		IconColors[ 145 ] = 243;
		IconColors[ 146 ] = 245;
		v3 = 240;
		do{
			v4 = &IconColors[ v3 ];
			*(v4 - 80) = v3;
			*(v4 - 48) = v3;
			*(v4 - 32) = v3++;
		}while( v3 < 255 );
		IconColors[ 175 ] = 0;
		IconColors[ 207 ] = 0;
		IconColors[ 223 ] = 0;
		break;
	}
}

//----- (00403BF3) --------------------------------------------------------
void DrawSpellButton()
{
	Player& player = Players[CurrentPlayerIndex];
	int spellNum = player.CurrentSpellIndex;
	int spellType = player.SpellType;
	int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, spellNum);
	bool isSpell = spellType == SO_1_SPELL && spellNum != -1;
	bool isSpellNoLevel = isSpell && spellLevel <= 0;
	bool isNegative = spellNum < 0;
	if( isSpell ){
		if( !IsManaEnoughtForSpell(CurrentPlayerIndex, spellNum, 1, 1) ){ // no mana
			spellType = SO_4_NONE;
		}
	}
	if( !IsSpellEnabledHere(spellNum, CurrentPlayerIndex) || isNegative || isSpellNoLevel ){
		spellType = SO_4_NONE;
	}
	DrawSpellColor(spellType);
	int iconIndex;
	if( spellNum == -1 ){
		iconIndex = 27;
	}else{
		iconIndex = getSpellIcon( spellNum );
	}
	DrawSpellIcon(565 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, 471 + Screen_TopBorder + ScreenHeight - GUI_Height, Data_SpelIconCEL, iconIndex, 56, spellType);
}

//----- (00403CCD) --------------------------------------------------------
void DrawSpellsPanel()
{
	Player& player = Players[CurrentPlayerIndex];

	SelectedSpellIndex = -1;
	InfoPanelHeader[0] = '\0';
	int surfaceXMin = Screen_LeftBorder + 20; // th1 = 20, th2 было 56 + 28;
	int surfaceXMax = Screen_LeftBorder + ScreenWidth - 68;

	int surfaceX = surfaceXMax;
	int surfaceY = Screen_TopBorder + 335 + (ScreenHeight - GUI_Height);
	InfoPanel_ClearBody();

	int iconIndex = 0;
	i64 spellMask = 0;
	for( int spellType = 0; spellType < 4; spellType++ ){
		int drawSpellType = spellType;
		switch (spellType ){
		case SO_0_SKILL:        DrawSpellColor(0); iconIndex = 55; spellMask = player.AvailableSkillMask;   break;
		case SO_1_SPELL:                           iconIndex = 56; spellMask = player.AvailableSpellMask;   break;
		case SO_2_RELIC:        DrawSpellColor(2); iconIndex = 53; spellMask = player.AvailableRelictMask;  break;
		case SO_3_EQUIPED_ITEM: DrawSpellColor(3); iconIndex = 54; spellMask = player.AvailableChargesMask; break;
		}
		i64 spellSelector = 1i64;
		for( int spellIndex = 1; spellIndex < PS_COUNT; spellIndex++, spellSelector <<= 1){
			if( !(spellSelector & spellMask) ){
				continue;
			}
			if( spellType == 1 ){
				int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, spellIndex);
				drawSpellType = spellLevel <= 0 ? 4 : 1;
				DrawSpellColor(drawSpellType);
			}
			if( !IsSpellEnabledHere(spellIndex, CurrentPlayerIndex) ){// Если в городе заклинание недоступно и мы находимся в городе меняем цвет
				drawSpellType = SO_4_NONE;
				DrawSpellColor(drawSpellType);
			}
			DrawSpellIcon(surfaceX, surfaceY, Data_SpelIconCEL, getSpellIcon( spellIndex ), SpellSelect_Size_56, drawSpellType);// Рисуем иконку

			if( CursorX >= surfaceX - Screen_LeftBorder && CursorX < surfaceX - Screen_LeftBorder + SpellSelect_Size_56 && CursorY >= surfaceY - 216 && CursorY < surfaceY - 216 + SpellSelect_Size_56 ){
				SelectedSpellType = spellType;
				SelectedSpellIndex = spellIndex;
				if( Players[CurrentPlayerIndex].ClassID == PC_3_MONK && spellIndex == PS_46_RELIC_OF_CONCENTR ){
					SelectedSpellType = 0;
				}
				DrawSpellIcon(surfaceX, surfaceY, Data_SpelIconCEL, iconIndex, SpellSelect_Size_56, drawSpellType);
				switch( SelectedSpellType ){
					case SO_0_SKILL:	FormatedOutputPlayerSkillString (SelectedSpellIndex) ;	break;
					case SO_1_SPELL:	FormatedOutputSpellString (SelectedSpellIndex) ;		break;
					case SO_2_RELIC:	FormatedOutputRelicString (SelectedSpellIndex) ;		break;
					case SO_3_EQUIPED_ITEM:	FormatedOutputStaffChargesString (SelectedSpellIndex) ;	break;
				}

				// Расширенные клавиши переназначения
				if (MoreManualHotkeys) {
					static constexpr char customkeys_charlist[64] = {
						'1','2','3','4','5','6','7','8','-','-','-','-', // 12
						'A','B','C','D','E','F','G','H','I','J','K','L','M',
						'N','O','P','Q','R','S','T','U','V','W','X','Y','Z', // +26
						'L','R', // +2 (CTRL)
						'\0'
					};

					for( int hotKeyIndex = 0; hotKeyIndex < 40; hotKeyIndex++ ){
						if ( SpellHotkeys.hotkey[hotKeyIndex] == SelectedSpellIndex && SpellHotkeys.type[hotKeyIndex] == SelectedSpellType){
							DrawSpellIcon(surfaceX, surfaceY, Data_HotkeyIconsCEL, hotKeyIndex + 1, SpellSelect_Size_56, drawSpellType);
							if (hotKeyIndex < 12) // отображения подсказок на F1-F8
								sprintf(InfoPanelBuffer, "Hotkey #F%c", customkeys_charlist[hotKeyIndex]);
							else if(hotKeyIndex < 38) // A-Z
								sprintf(InfoPanelBuffer, "Hotkey '%c'", customkeys_charlist[hotKeyIndex]);
							else if (hotKeyIndex < 40)  // L.CTRL / R.CTRL
								sprintf(InfoPanelBuffer, "Hotkey '%c.CTRL'", customkeys_charlist[hotKeyIndex]);
								
							InfoPanel_AddLine(InfoPanelBuffer, true);
						}
					}
				}else{
					for (int hotKeyIndex = 0; hotKeyIndex < 4; hotKeyIndex++) {
						if( player.SpellOnFuncKeys[hotKeyIndex] == SelectedSpellIndex && player.SpellTypeOnFuncKeys[hotKeyIndex] == SelectedSpellType ){
							DrawSpellIcon(surfaceX, surfaceY, Data_SpelIconCEL, hotKeyIndex + 57, SpellSelect_Size_56, drawSpellType);
							sprintf(InfoPanelBuffer, "Spell Hot Key #F%i", hotKeyIndex + 5);
							InfoPanel_AddLine(InfoPanelBuffer, true);
						}
					}
				}
			}

			surfaceX -= SpellSelect_Size_56;
			if( surfaceX <= surfaceXMin ){
				surfaceY -= SpellSelect_Size_56;
				surfaceX = surfaceXMax;
			}
		}

		if( spellMask && surfaceX != surfaceXMax ){// Отступ в одну иконку между группами заклинаний
			surfaceX -= SpellSelect_Size_56;
			if( surfaceX <= surfaceXMin ){
				surfaceY -= SpellSelect_Size_56;
				surfaceX = surfaceXMax;
			}
		}
		
	}
}

//----- (00404219) --------------------------------------------------------
void SelectCurrentSpell()
{
	Player& player = Players[CurrentPlayerIndex];
	SelectCurSpellMode = 0;
	if( SelectedSpellIndex != -1 ){
		InfoPanel_ClearBody();
		player.CurrentSpellIndex = SelectedSpellIndex;
		player.SpellType = SelectedSpellType;
	}
}

//----- (00404275) --------------------------------------------------------
void __fastcall AssignNewSpellToKey(int key)
{
	Player& player = Players[CurrentPlayerIndex];
	if( SelectedSpellIndex != -1 ){
		for( int i = 0; i < 4; i++ ){
			if( player.SpellOnFuncKeys[i] == SelectedSpellIndex
			 && player.SpellTypeOnFuncKeys[i] == SelectedSpellType ){
				player.SpellOnFuncKeys[i] = -1;
			}
		}
		player.SpellOnFuncKeys[key] = SelectedSpellIndex;
		player.SpellTypeOnFuncKeys[key] = SelectedSpellType;
	}
}

void __fastcall AssignNewSpellToKeyExpanded(int key)
{
	bool clearedhotkey = true;

	if (SelectedSpellIndex != -1) {
		// ищем уже имеющийся хоткей с выделенным заклинанием для снятия привязки
		for (int i = 0; i < 40; ++i)
			if (SelectedSpellIndex == SpellHotkeys.hotkey[i]
				&& SelectedSpellType == SpellHotkeys.type[i]) {
				if (i == key) clearedhotkey = false;
				SpellHotkeys.hotkey[i] = -1;
				SpellHotkeys.type[i] = 0;
			}
		if (clearedhotkey) {
			SpellHotkeys.hotkey[key] = SelectedSpellIndex;
			SpellHotkeys.type[key] = SelectedSpellType;
			PlayGlobalSound(S_76_I_TITLSLCT); // звук назначения хоткея
		}
		else
		{
			PlayGlobalSound(S_75_I_TITLEMOV); // звук снятия хоткея
		}
	}
}

//----- (00404317) -------------------------------------------------------- player spell
void __fastcall SelectSpellByHotKey(int keyIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	
	int spellNumber = player.SpellOnFuncKeys[keyIndex];
	if( (uint) spellNumber < PS_COUNT ){
		int spellOrigin = player.SpellTypeOnFuncKeys[keyIndex];
		i64 spellMask;
		switch (spellOrigin) {
		case SO_0_SKILL: spellMask = player.AvailableSkillMask; break;
		case SO_1_SPELL: spellMask = player.AvailableSpellMask; break;
		case SO_2_RELIC: spellMask = player.AvailableRelictMask; break;
		case SO_3_EQUIPED_ITEM: spellMask = player.AvailableChargesMask; break;
		default: spellMask = 0;
		}
		if( spellMask & BIT(spellNumber) ){
			player.CurrentSpellIndex = spellNumber;
			player.SpellType = spellOrigin;
		}
	}
}

int __fastcall SelectSpellByHotKeyExpanded(int keyOffset)
{
	Player& player = Players[CurrentPlayerIndex];
	int spellNumber = SpellHotkeys.hotkey[keyOffset];
	int spellOrigin   = SpellHotkeys.type[keyOffset];
	__int64 spellMask;

	if (spellNumber > 0) {
		switch (spellOrigin) {
		case SO_0_SKILL: spellMask = player.AvailableSkillMask; break;
		case SO_1_SPELL: spellMask = player.AvailableSpellMask; break;
		case SO_2_RELIC: spellMask = player.AvailableRelictMask; break;
		case SO_3_EQUIPED_ITEM: spellMask = player.AvailableChargesMask; break;
		default: spellMask = 0;
		}

		if (spellMask & (1i64 << (spellNumber - 1))) {
			player.CurrentSpellIndex = spellNumber;
			player.SpellType = spellOrigin;
		}
		else return 0;
	}
	return 1;
}

typedef void DrawLetterPixelFunc(uchar* dst, char mask, int colorIndex);
//----- (th) --------------------------------------------------------------
void DrawLetterPixel(uchar* dst, char mask, int colorIndex)
{
	*dst = GetLetterColor(mask, colorIndex);
}
//----- (th) --------------------------------------------------------------
void DrawLetterOutline(uchar* dst, char mask, int colorIndex)
{
	dst[-1] = 0;
	dst[ 1] = 0;
	dst[-WorkingWidth] = 0;
	dst[ WorkingWidth] = 0;
}
//----- (th) --------------------------------------------------------------
void DrawLetterPixels(uchar* curPtr, uchar* endPtr, uchar* dst, char fontColor, DrawLetterPixelFunc* drawLetterPixelFunc)
{
	enum { CharLen = 13 };
	int nextLine = 0;
	while( curPtr < endPtr ){
		if( *curPtr > 127 ){
			int lineWidth = 256 - *curPtr++;
			dst += lineWidth;
			nextLine += lineWidth;
			if( nextLine >= CharLen ){
				dst = dst - WorkingWidth - CharLen;
				nextLine -= CharLen;
			}
		}else{
			int lineWidth = *curPtr++;
			while( lineWidth-- ){
				drawLetterPixelFunc(dst++, *curPtr++, fontColor);
				nextLine++;
				if( nextLine >= CharLen ){
					dst = dst - WorkingWidth - CharLen;
					nextLine = 0;
				}
			}
		}
	}
}

//----- (0040440C) --------------------------------------------------------
void __fastcall DrawLetter(int surfaceOffset, int letterID, char fontColor, bool outline /*= false*/)
{
	const LetterStruct* letterPtr = (LetterStruct*)&FontSpriteSmall[4 * letterID];
	uchar* dst = &WorkingSurface[surfaceOffset];
	uchar* curPtr = (unsigned char*)&FontSpriteSmall[letterPtr->StartOffset];
	uchar* endPtr = (unsigned char*)&FontSpriteSmall[letterPtr->EndOffset];
	if( outline ) DrawLetterPixels(curPtr, endPtr, dst, fontColor, DrawLetterOutline);
	DrawLetterPixels(curPtr, endPtr, dst, fontColor, DrawLetterPixel);
}

//----- (0040454A) -------------------------------------------------------- interface
void __fastcall InfoPanel_AddLine(const char *str, int centered, const char* str2 /*=0*/)
{
    if( InfoPanelLinesUsed == 16 ){
        return;
    }
	strcpy(InfoPanel4Lines[InfoPanelLinesUsed], str);
	if( str2 ) strcat( InfoPanel4Lines[InfoPanelLinesUsed], str2 );
	InfoPanelLinesCentered[InfoPanelLinesUsed++] = centered;
}

//----- (0040457C) -------------------------------------------------------- interface
void InfoPanel_ClearBody()
{
	InfoPanelLinesUsed = 0;
	InfoPanelUsed = 0;
}

//----- (0040458B) -------------------------------------------------------- interface
int __fastcall CopyFromMainPanelToWorkingSurface(int SrcX, int SrcY, int Width, int Height, int DstX, int DstY)
{
	DstX += (ScreenWidth - GUI_Width)/2;
	DstY += ScreenHeight - GUI_Height;

	int result = DstX + WorkingWidth * DstY;
	int SrcWidth = GUI_Width;   //MainPanel.GetWidth();
	int SrcHeight = 288;  // temporary
	int DstWidth = WorkingWidth;
	int DstHeight = ScreenHeight + 192; //1300;
	for( int y = 0; (y < Height) && (y + SrcY < SrcHeight) && (y + DstY < DstHeight); y++ ){
		if( y + SrcY >= 0 && y + DstY >= 0 ){
			for( int x = 0; (x < Width) && (x + SrcX < SrcWidth) && (x + DstX < DstWidth); x++ ){
				/*if( MainPanelImage[SrcX + x + SrcWidth * (SrcY + y)] != 0 ){*/ // 0 в главной панели означает чёрный, а не прозрачный
					WorkingSurface[DstX + x + DstWidth * (DstY + y)] = MainPanelImage[SrcX + x + SrcWidth * (SrcY + y)];
				/*}*/
			}
		}
	}
	return result;
}

//----- (004045FC) -------------------------------------------------------- interface
void __fastcall DrawEmptyGlobeBottom(uchar* aLifeShereImage, int aStartRow, int aEndRow, int aStartOffset, int aStartY)
{
	aStartOffset += WorkingWidth * (aStartY + ScreenHeight - GUI_Height) + (ScreenWidth - GUI_Width)/2;
	uchar* srcPtr = &aLifeShereImage[88 * aStartRow];
	uchar* dstPtr = &WorkingSurface[aStartOffset];
	for( int height = aEndRow - aStartRow; height; height-- ){
		memcpy(dstPtr, srcPtr, 0x58u);
		srcPtr += 88;
		dstPtr += WorkingWidth;
	}
}

//----- (0040464D) -------------------------------------------------------- interface
void __fastcall PutWithAlpha(uchar* srcSurface, int srcWidth, int srcOffset, uchar* dstSurface, int dstOffset, int ySize)
{
	uchar* src = srcSurface + srcOffset;
	uchar* dst = dstSurface + dstOffset + WorkingWidth*(ScreenHeight - GUI_Height) + (ScreenWidth - GUI_Width)/2;
	for( int y = 0; y < ySize; y++ ){
		for( int x = 0; x < 59; x++ ){
			if( *src ){// Если пиксель источника ненулевой рисуем его
				*dst = *src; //Dragon оригинал вылетает когда кончается фури, DstSurface выходит за границу
			}
			src++;
			dst++;
		}
		src += srcWidth - 59;
		dst += WorkingWidth - 59;
	}
}

//----- (0040468D) -------------------------------------------------------- interface
void DrawLifeGlobeTop()
{    
	if( !IsInterface ) return;
	Player& player = Players[CurrentPlayerIndex];
	if( player.MaxCurLife > 0 ){
		player.RatioLifeGlobe = ftol(double(player.CurLife) / double(player.MaxCurLife) * 80.);
	}else{
		player.RatioLifeGlobe = 0;
	}
	int height = 80 - player.RatioLifeGlobe;
	if( height > 11 ){
		height = 11;// Магическое число размер на который сфера подымается над панелью
	}
	height += 2;
	PutWithAlpha(LifeShereImage, 88, 277, WorkingSurface, WorkingWidth * 499 + 109 + Screen_LeftBorder, height);// Изображение куска панели и обода сферы
	if( height != 13 ){// Если в верхней части банки осталась кровь
		PutWithAlpha(MainPanelImage, GUI_Width, GUI_Width * (height + 3) + 109, WorkingSurface, WorkingWidth * height + WorkingWidth * 499 + 109 + Screen_LeftBorder, 13 - height);// Изображение собственно крови
	}
}

//----- (00404752) -------------------------------------------------------- interface
void DrawLifeGlobeBottom()
{
	Player& player = Players[CurrentPlayerIndex];
	int lifeOrbHeight;
	if( player.MaxCurLife > 0 ){
		lifeOrbHeight = ftol(double(player.CurLife) / double(player.MaxCurLife) * 80.0);
	}else{
		lifeOrbHeight = 0;
	}
	player.RatioLifeGlobe = lifeOrbHeight;
	if( lifeOrbHeight > 69 ){
		lifeOrbHeight = 69;// Магическое число размер на который сфера опускается под панелью. Надо загнать в константы
	}
	if( lifeOrbHeight != 69 ){
		DrawEmptyGlobeBottom(LifeShereImage, 16, 85 - lifeOrbHeight, 96 + Screen_LeftBorder, 512);
	}
	if( lifeOrbHeight ){
		CopyFromMainPanelToWorkingSurface(96, 85 - lifeOrbHeight, 88, lifeOrbHeight, 96 + Screen_LeftBorder, 581 - lifeOrbHeight);
	}
	if (ShowNumbersOnHealth) {
	    const char* separator = "/";
     
		const int x = ScreenWidth / 2 - 182;
		const int y = ScreenHeight - 89;
		
		DrawLevelInfoText( x, y, separator, C_0_White, OnScreenTextOutline );
		
		sprintf(InfoPanelBuffer, "%i", player.MaxCurLife >> 6);
		DrawLevelInfoText( x + GetTextWidth( separator ), y, InfoPanelBuffer, C_0_White, OnScreenTextOutline );
		
		sprintf(InfoPanelBuffer, "%i", player.CurLife >> 6);
		DrawLevelInfoText( x - GetTextWidth( InfoPanelBuffer ), y, InfoPanelBuffer, C_0_White, OnScreenTextOutline );
	}
}

//----- (004047E7) -------------------------------------------------------- interface
void DrawManaGlobeTop()
{
	if( !IsInterface) return;
	Player& player = Players[CurrentPlayerIndex];
	// в TH 1 в этой функции в отличии от трех остальных глобовский, почему то нет расчета ratio
	// но если это блок убрать, то манасфера отрисовывается без верха, а в TH 1 нормально, концов пока не нашел
	if( player.MaxCurMana > 0 ){
		player.RatioManaGlobe = ftol(double(player.CurMana) / double(player.MaxCurMana) * 80.0 );
	}else{
		player.RatioManaGlobe = 0;
	}
	int height = 80 - player.RatioManaGlobe;// Даже не тут вычисляется
	if( height > 11 ){
		height = 11;
	}
	height += 2;
	PutWithAlpha(ManaShereImage, 88, 277, WorkingSurface, WorkingWidth * 499 + 475 + Screen_LeftBorder, height);
	if( height != 13 ){
		PutWithAlpha(MainPanelImage, GUI_Width, GUI_Width * (height + 3) + 475, WorkingSurface, WorkingWidth * height + WorkingWidth * 499 + 475 + Screen_LeftBorder, 13 - height);
	}
}

//----- (00404867) -------------------------------------------------------- interface
void RecalcLifeManaGlobes()
{
	Player& player = Players[ CurrentPlayerIndex ];
	int maxCurMana = player.MaxCurMana;
	int curMana = player.CurMana;
	if( player.MaxCurMana < 0 ){
		maxCurMana = 0;
	}
	if( player.CurMana < 0 ){
		curMana = 0;
	}
	if( maxCurMana ){
		player.RatioManaGlobe = ftol ((double) curMana / (double) maxCurMana * 80.0);
	}else{
		player.RatioManaGlobe = 0;
	}
	player.RatioLifeGlobe = ftol ((double) player.CurLife / (double) player.MaxCurLife * 80.0);

}

//----- (004048F9) -------------------------------------------------------- interface
void DrawManaGlobeBottom()
{
	Player& player = Players[CurrentPlayerIndex];
	int ratioManaGlobe; // edi@6
	int curMana; // [sp+8h] [bp-8h]@1
	int maxCurMana; // [sp+Ch] [bp-4h]@1

	maxCurMana = Players[ CurrentPlayerIndex ].MaxCurMana;
	curMana = Players[ CurrentPlayerIndex ].CurMana;
	if( Players[ CurrentPlayerIndex ].MaxCurMana < 0 ){
		maxCurMana = 0;
	}
	if( Players[ CurrentPlayerIndex ].CurMana < 0 ){
		curMana = 0;
	}
	if( maxCurMana ){
		ratioManaGlobe = ftol((double) curMana / (double) maxCurMana * 80.0);
	}else{
		ratioManaGlobe = 0;
	}
	Players[ CurrentPlayerIndex ].RatioManaGlobe = ratioManaGlobe;
	if( ratioManaGlobe > 69 ){
		ratioManaGlobe = 69;// Магическое число размер на который сфера опускается под панелью. Надо загнать в константы
	}
	if( ratioManaGlobe != 69 ){
		DrawEmptyGlobeBottom(ManaShereImage, 16, 85 - ratioManaGlobe, 464 + Screen_LeftBorder, 512);
	}
	if( ratioManaGlobe ){
		CopyFromMainPanelToWorkingSurface(464, 85 - ratioManaGlobe, 88, ratioManaGlobe, 464 + Screen_LeftBorder, 581 - ratioManaGlobe);
	}

	if (ShowNumbersOnMana) {
	    const char* separator = "/";
     
		const int x = ScreenWidth / 2 + 182;
		const int y = ScreenHeight - 89;
		
		DrawLevelInfoText( x, y, separator, C_0_White, OnScreenTextOutline );
		
		sprintf(InfoPanelBuffer, "%i", player.MaxCurMana >> 6);
		DrawLevelInfoText( x + GetTextWidth( separator ), y, InfoPanelBuffer, C_0_White, OnScreenTextOutline );
		
		sprintf(InfoPanelBuffer, "%i", player.CurMana >> 6);
		DrawLevelInfoText( x - GetTextWidth( InfoPanelBuffer ), y, InfoPanelBuffer, C_0_White, OnScreenTextOutline );
	}
	DrawSpellButton();
}

uchar CharbutIceage[256];
//----- (004049AE) -------------------------------------------------------- interface
void MayBeViewInit()
{
	int mainPanelSize = 144 * GUI_Width; // 92160
	if( MaxCountOfPlayersInGame != 1 ){ // блокировка талк панели для сингла
		mainPanelSize = 288 * GUI_Width; // 184320
	}
	MainPanelImage = (uchar*) AllocMem( mainPanelSize );
	memset( MainPanelImage, 0, mainPanelSize );
	ManaShereImage = (uchar*)AllocMem(7744); // 88 * 88
	memset( ManaShereImage, 0, 7744);
	LifeShereImage = (uchar*)AllocMem(7744); // 88 * 88
	memset( LifeShereImage, 0, 7744);
	FontSpriteSmall = (char*)LoadFile("CtrlPan\\SmalText.CEL");
	Monster_Bar_Border = (char*)LoadFile("data\\monsterbar\\status_bar_border.CEL");
	char* monsterBarTRNname[3] = { "base.trn", "blue.trn", "gray.trn" };
	for (size_t i = 0; i < countof(monsterBarTRNname); ++i) {
		char fullpath[256] = "data\\monsterbar\\status_bar_border_";
		if(ColoredMonsterBarBorder)
			strcat(fullpath, monsterBarTRNname[i]);
		else
			strcat(fullpath, "base.trn");
		uchar* colorTable = (uchar*)LoadFile(fullpath);
		memcpy(MonsterBarBorderColors[i], colorTable, 256);
		FreeMemZero(colorTable);
	}
	Monster_Bar_MonIcons = (char*)LoadFile("data\\monsterbar\\status_bar_icons.CEL");
	Monster_Bar_ImIcons = (char*)LoadFile("data\\monsterbar\\status_bar_im.CEL");
	char* vulnIconsTRNname[6] = { "fire.trn", "light.trn", "cold.trn", "arcan.trn", "acid.trn", "curse.trn" };
	for (size_t i = 0; i < countof(vulnIconsTRNname); ++i) {
		char fullpath[256] = "data\\monsterbar\\status_bar_icon_";
		strcat(fullpath, vulnIconsTRNname[i]);
		uchar* colorTable = (uchar*)LoadFile(fullpath);
		memcpy(MonsterBarIconColors[i], colorTable, 256);
		FreeMemZero(colorTable);
	}
	Data_CharCEL = (char*)LoadFile("Data\\Char.CEL");
	InfoWindow_Init();
	Data_HotkeyIconsCEL = (char*)LoadFile("Data\\HotkeyIcons.CEL");
	Data_SpelIconCEL   = (char*)LoadFile(GameMode == GM_CLASSIC ? "Data\\SpelIconOL.CEL" : "Data\\SpelIcon.CEL");
	Data_SpelIconCEL_2 = (char*)LoadFile(GameMode != GM_CLASSIC ? "Data\\SpelIconOL.CEL" : "Data\\SpelIcon.CEL");
	DrawSpellColor(0);
	char* currentCELFilePtr = (char*) LoadFile( (char*)("CtrlPan\\"s + (GameMode == GM_CLASSIC ? "panel8" : UserPanelB  ? "front_panel_mana_blue" : "front_panel_mana_black") + ".cel").c_str() );
	ParseCELFile(MainPanelImage, 0, 143, GUI_Width, currentCELFilePtr, 1, GUI_Width );
	FreeMemZero(currentCELFilePtr);
	// Globes
	currentCELFilePtr = (char*)LoadFile("CtrlPan\\P8Bulbs.CEL");
	ParseCELFile(LifeShereImage, 0, 87, 88, currentCELFilePtr, 1, 88);
	ParseCELFile(ManaShereImage, 0, 87, 88, currentCELFilePtr, 2, 88);
	FreeMemZero(currentCELFilePtr);
	TalkPanelMode = 0;
	if( MaxCountOfPlayersInGame != 1 ){ // загружаем второй образ панели, блокировка талк панели для сингла
		currentCELFilePtr = (char*)LoadFile("CtrlPan\\TalkPanl.CEL");
		ParseCELFile(MainPanelImage, 0, 287, GUI_Width, currentCELFilePtr, 1, GUI_Width);
		FreeMemZero(currentCELFilePtr);
		MainPanelSelector = 0;
		MayPlayersTalk[0] = true;
		MayPlayersTalk[1] = true;
		MayPlayersTalk[2] = true;
		MayPlayersTalk[3] = true;
		TalkPanelButtonState[0] = 0;
		TalkPanelButtonState[1] = 0;
		TalkPanelMessage[0] = 0;
		TalkPanelButtonState[2] = 0;
	}
	MaybeNeedDrawInfoPanel = 0;
	IsCursorOnLevelUpButton = false;
    IsCursorOnPerkUpButton = false;
	InitPanelButtons();
	Data_CharButCEL = (char*)LoadFile("Data\\CharBut.CEL");
	AttributeUpAvail[0] = 0;
	AttributeUpAvail[1] = 0;
	AttributeUpAvail[2] = 0;
	ButtonStatDown = 0;
	AttributeUpAvail[3] = 0;
	Items_DurIconsCEL = (char*) LoadFile( "Items\\DurIcons.CEL" );
	Items_DurIconsCEL2 = (char*) LoadFile( "Items\\DurIconsTh2.CEL" );
	#if th2_crucifixion
	CrestImageCEL = LoadFile( "crest.CEL" );// thehell2.mpq
	#endif
	InfoPanelHeader[ 0 ] = '\0';
	InfoPanel_ClearBody();
	Tooltip_Clear();
	IsCHARPanelVisible = false;
	SelectCurSpellMode = 0;
	Data_SpellBkCEL = (char*)LoadFile("Data\\SpellBk_newX.CEL");
	Data_SpellBkBCEL = (char*)LoadFile("Data\\SpellBkB_new.CEL");
	Data_SpellI2CEL   = (char*)LoadFile(GameMode == GM_CLASSIC ? "Data\\Spelli2OL.CEL" : "Data\\SpellI2.CEL");
	Data_SpellI2CEL_2 = (char*)LoadFile(GameMode != GM_CLASSIC ? "Data\\Spelli2OL.CEL" : "Data\\SpellI2.CEL");

	uchar* colorTable = (uchar*) LoadFile(UserPanelB ? "X\\other\\UnActBlue.trn" : "X\\other\\UnActBlack.trn");
	if (GameMode == GM_CLASSIC) { colorTable = (uchar*)LoadFile("X\\other\\UnActBlueCL.trn"); }
	memcpy( IconColorsDisabled, colorTable, 256 );
	FreeMemZero( colorTable );

    colorTable = (uchar*)LoadFile("X\\other\\Spell_Icon_Inactive05.trn");
	if (RedInactiveSpellIcons) memcpy(IconColorsDisabled, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile(UserPanelB ? "X\\other\\ActBlue.trn" : "X\\other\\ActBlack.trn");
	if (GameMode == GM_CLASSIC) { colorTable = (uchar*)LoadFile("X\\other\\ActBlueCL.trn"); }
	memcpy(IconColorsSpell, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile("X\\other\\Crimzon.trn");
	if (GameMode == GM_CLASSIC) { colorTable = (uchar*)LoadFile("X\\other\\CrimzonCL.trn"); }
	memcpy(IconColorsRelict, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile("X\\other\\Beige.trn");
	if (GameMode == GM_CLASSIC) { colorTable = (uchar*)LoadFile("X\\other\\BeigeCL.trn"); }
	memcpy(IconColorsSkill, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile("X\\other\\Orange.trn");
	if (GameMode == GM_CLASSIC) { colorTable = (uchar*)LoadFile("X\\other\\OrangeCL.trn"); }
	memcpy(IconColorsCharges, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile("X\\other\\BrightRed-Orange.trn");
	memcpy(IconColorsTrait, colorTable, 256);
	FreeMemZero(colorTable);

	colorTable = (uchar*)LoadFile("data\\charbut_iceage.trn");
	memcpy(CharbutIceage, colorTable, 256);
	FreeMemZero(colorTable);

	CurrentSpellBookPage = 0;
	IsSpellBookVisible = false;
	Data_QuestCEL = (char*)LoadFile("Data\\Quest_new.CEL");
	Data_ExclamCEL = (char*)LoadFile("Data\\exclam32x32bleak.CEL");
	// Вылет при открытии сплит голд панели когда загрузка файла изображения сплит панели проваливается. 
	CtrlPan_GolddropCEL = (char*)LoadFile("CtrlPan\\Golddrop.cel"); 
	PentSpnCurrentPicNumber = 1;
	IsGoldSplitPanelVisible = false;
	HowMuchGoldYouWantToRemove = 0;
	GoldYouHave = 0;
	CurCellIndex = 0;
	
	SetupDisplayObjects();
}

// вычисление прямоугольников отображаемых вещей, пересчитываются при изменении разрешения
// ---- th2 -------------------------------------------------------------------------------
void SetupDisplayObjects()
{
	SetupInvPanel(CurrentPlayerIndex);

	SetupPanelRect( QuestPanelRect, LeftPanel, GUI_QuestPanelWidth, GUI_QuestPanelHeight );
	const int questPanelTextAreaPadding = 32;
	const int questPanelTextAreaDoublePadding = questPanelTextAreaPadding * 2;
	/*CursorX > 32 && CursorX < 288 && CursorY > 32 && CursorY < 308*/
	SetupDisplayObject( QuestPanelTextAreaRect, QuestPanelRect.Left + questPanelTextAreaPadding
		, QuestPanelRect.Top + questPanelTextAreaPadding
		, QuestPanelRect.Width - questPanelTextAreaDoublePadding
		, QuestPanelRect.Heigth - questPanelTextAreaDoublePadding );
	
	SetupPanelRect( CharPanelRect, LeftPanel, GUI_CharPanelWidth, GUI_CharPanelHeight );

	SetupDisplayObject(CharPanelCurStrRect,      CharPanelRect.Left +   136, CharPanelRect.Top + 137, 44, 26);
	SetupDisplayObject(CharPanelCurMagRect,      CharPanelRect.Left +   136, CharPanelRect.Top + 165, 44, 26);
	SetupDisplayObject(CharPanelCurDexRect,      CharPanelRect.Left +   136, CharPanelRect.Top + 193, 44, 26);
	SetupDisplayObject(CharPanelCurVitRect,      CharPanelRect.Left +   136, CharPanelRect.Top + 221, 44, 26);
	SetupDisplayObject(CharPanelBaseStrRect,     CharPanelRect.Left +    88, CharPanelRect.Top + 137, 44, 26);
	SetupDisplayObject(CharPanelBaseMagRect,     CharPanelRect.Left +    88, CharPanelRect.Top + 165, 44, 26);
	SetupDisplayObject(CharPanelBaseDexRect,     CharPanelRect.Left +    88, CharPanelRect.Top + 193, 44, 26);
	SetupDisplayObject(CharPanelBaseVitRect,     CharPanelRect.Left +    88, CharPanelRect.Top + 221, 44, 26);
	SetupDisplayObject(CharPanelDamageRect,      CharPanelRect.Left +   240, CharPanelRect.Top + 195, 63, 23);
	SetupDisplayObject(CharPanelResistAcid,      CharPanelRect.Left +   250, CharPanelRect.Top + 219, 52, 23);
	SetupDisplayObject(CharPanelResistMagic,     CharPanelRect.Left +   250, CharPanelRect.Top + 243, 52, 23);
	SetupDisplayObject(CharPanelResistFire,      CharPanelRect.Left +   250, CharPanelRect.Top + 267, 52, 23);
	SetupDisplayObject(CharPanelResistCold,      CharPanelRect.Left +   250, CharPanelRect.Top + 291, 52, 23);
	SetupDisplayObject(CharPanelResistLightning, CharPanelRect.Left +   250, CharPanelRect.Top + 315, 52, 23);
	SetupDisplayObject(CharPanelResistArrow,     CharPanelRect.Left +   240, CharPanelRect.Top + 344, 63, 22);
	SetupDisplayObject(CharPanelResistMelee,     CharPanelRect.Left +   240, CharPanelRect.Top + 123, 63, 23);
	SetupDisplayObject(CharPanelXp,              CharPanelRect.Left +   210, CharPanelRect.Top +  53, 93, 23);
	SetupDisplayObject(CharPanelXpNext,          CharPanelRect.Left +   210, CharPanelRect.Top +  82, 93, 23);
	SetupDisplayObject(CharPanelArmorClass,      CharPanelRect.Left +   250, CharPanelRect.Top + 148, 55, 22);
	SetupDisplayObject(charPanelToHit,           CharPanelRect.Left +   250, CharPanelRect.Top + 174, 55, 19);
	SetupDisplayObject(CharPanelLifeRegen,       CharPanelRect.Left +    86, CharPanelRect.Top + 344, 46, 22);
	SetupDisplayObject(CharPanelManaRegen,       CharPanelRect.Left + 86+50, CharPanelRect.Top + 344, 46, 22);
	SetupDisplayObject(CharPanelCritDmg,         CharPanelRect.Left + 86+50, CharPanelRect.Top + 373, 66, 22);
	SetupDisplayObject(CharPanelCritChance,      CharPanelRect.Left +    86, CharPanelRect.Top + 373, 66, 22);

	SetupTradePanel();
	SetupStashPanel();
	SetupCraftPanel();
	SetupEnchantPanel();
	SetupQuenchPanel();
	SetupTransmutePanel();

	SetupDisplayObject(MainPanelRect, (ScreenWidth - GUI_Width) / 2, ScreenHeight - GUI_MainPanelHeight, GUI_Width, GUI_MainPanelHeight);
	SetupPanelRect( InventoryPanelRect, RightPanel, GUI_InventoryPanelWidth, GUI_InventoryPanelHeight );

	const int GUI_GoldSplitPanelLeftPadding = (InventoryPanelRect.Width - GUI_GoldSplitPanelWidth) / 2;
	SetupDisplayObject( GoldSplitPanelRect, InventoryPanelRect.Left + GUI_GoldSplitPanelLeftPadding
		, InventoryPanelRect.Top + GUI_GoldSplitPanelTopPadding
		, GUI_GoldSplitPanelWidth
		, GUI_GoldSplitPanelHeight );

	SetupPanelRect( SpellBookRect, RightPanel, GUI_SpellBookWidth, GUI_SpellBookHeight );

	SetupDisplayObject( SpellBookTextRect, SpellBookRect.Left + 53, SpellBookRect.Top + 18, 257, 296 );

	SetupDisplayObject( SpellBookPageButtonsRect, SpellBookRect.Left + GUI_SpellBook_PageButtonsX
		, SpellBookRect.Top + GUI_SpellBook_PageButtonsY
		, GUI_SpellBook_PageButtonsWidth
		, GUI_SpellBook_PageButtonsHeight );


	SetupDisplayObject( SpellBookSpellButtonsRect, SpellBookRect.Left + GUI_SpellBook_SpellButtonsX
		, SpellBookRect.Top + GUI_SpellBook_SpellButtonsY
		, GUI_SpellBook_SpellButtonsWidth
		, GUI_SpellBook_SpellButtonsHeight );

	SetupPanelRect( PerksPanelPos, CenteredPanel, GUI_PerksPanelWidth, GUI_PerksPanelHeight );
}

//----- (00404D06) -------------------------------------------------------- interface
void DrawMainPanel()
{
	CopyFromMainPanelToWorkingSurface(0, MainPanelSelector + 16, GUI_Width/*ScreenWidth*/, 128, Screen_LeftBorder, 512);
	DrawInfoPanel();
}

//----- (th2) -------------------------------------------------------------
void InitPanelButtons()
{
	CtrlPan_Panel8buCEL   = (char*)LoadFile("CtrlPan\\Panel8bu.CEL");
	CtrlPan_NewButtonsCEL = (char*)LoadFile("CtrlPan\\MainPanelV3_buttons_37x29.CEL");
	CtrlPan_TimeButtonCEL = (char*)LoadFile("data\\timescale_arrow_button.CEL");
	CtrlPan_MainMenuCEL   = (char*)LoadFile("CtrlPan\\MainPanelV3_menuButton_75x29.CEL");
	CtrlPan_P8But2CEL     = (char*)LoadFile("CtrlPan\\P8But2.CEL");
	CtrlPan_TalkButtCEL   = (char*)LoadFile("CtrlPan\\TalkButt.CEL");
	Buttons = ModButtons;
	if( GameMode == GM_CLASSIC ) for( Button& b: Buttons ){
		switch(b.id){
		case B_CHAR   : b.cel = &CtrlPan_Panel8buCEL; b.x = 9; b.y = 361; b.w = 71; b.h = 19; b.pic[0] = -1; b.pic[1] = 1; b.yPicOfs = 0; break;
		case B_QUEST  : b.cel = &CtrlPan_Panel8buCEL; b.x = 9; b.y = 387; b.w = 71; b.h = 19; b.pic[0] = -1; b.pic[1] = 2; b.yPicOfs = 0; break;
		case B_AUTOMAP: b.cel = &CtrlPan_Panel8buCEL; b.x = 9; b.y = 427; b.w = 71; b.h = 19; b.pic[0] = -1; b.pic[1] = 3; b.yPicOfs = 0; break;
		case B_MENU   : b.cel = &CtrlPan_Panel8buCEL; b.x = 9; b.y = 453; b.w = 71; b.h = 19; b.pic[0] = -1; b.pic[1] = 4; b.yPicOfs = 0; break;
		}
	}
	MainPanelButtonFlag   = 0;
	for( auto& b: Buttons ){
		b.pressed = 0;
		b.active = ((MaxCountOfPlayersInGame != 1) + 1) & b.multiPlayer;
		if( !(b.gameMode & B(GameMode)) ) b.active = false;
	}
}

//----- (th2) -------------------------------------------------------------
void FreePanelButtons()
{
	FreeMemZero(CtrlPan_Panel8buCEL);
	FreeMemZero(CtrlPan_NewButtonsCEL);
	FreeMemZero(CtrlPan_TimeButtonCEL);
	FreeMemZero(CtrlPan_MainMenuCEL);
	FreeMemZero(CtrlPan_P8But2CEL);
	FreeMemZero(CtrlPan_TalkButtCEL);
}

//----- (00404D2B) -------------------------------------------------------- interface
void DrawMainButtons()
{
	for( auto& b: Buttons ){
		if( !b.active ) continue;
		if( int pic = b.pic[b.pressed]; pic == -1 ){
			CopyFromMainPanelToWorkingSurface(b.x, b.y - 336, b.w, b.h + 1, b.x + Screen_LeftBorder, b.y + Screen_TopBorder);
		}else{
			if( b.id == B_TIME ){ pic += CurTimeSpeedIndex * 2; }
			Surface_DrawCEL(b.x + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, b.y + 178 + b.yPicOfs + ScreenHeight - GUI_Height, *b.cel, pic, b.w);
		}
	}
}

// при клике на кнопку выбора заклинания надо спозиционировать курсор на текущее заклинание
//----- (00404DE6) -------------------------------------------------------- interface
void SpellPanel_SetupCursorPos()
{
	__int64 spellSelector;                           // qax@4
	__int64 spellMask;
	int surfaceX;                                    // [sp+20h] [bp-4h]@1
	int surfaceY;                                    // [sp+1Ch] [bp-8h]@1
	int newMouseX;                                   // [sp+Ch] [bp-18h]@1
	int newMouseY;                                   // [sp+10h] [bp-14h]@1
	//int spellType;                                   // [sp+18h] [bp-Ch]@2
	Player& player = Players[CurrentPlayerIndex];
	SelectCurSpellMode = 1;                          // запускаем режим выбора заклинания
	int surfaceXMin = 56 + 28;
	int surfaceXMax = ScreenWidth;

	surfaceX = surfaceXMax;// Странный баг иногда случается, когда при первом открытии панели выбора курсор летит незнамо куда...
	surfaceY = 495 + ScreenHeight - GUI_Height;

	newMouseX = surfaceX - 70;// Координаты середины первой иконки то бишь скила героя
	newMouseY = surfaceY - 188;
	if( player.CurrentSpellIndex == -1 ){ //	 если нет текущего заклинания
		ChangeCursorPos(newMouseX - CursorX, newMouseY - CursorY);
		return;
	}

	for( int spellType = 0; spellType < 4; spellType++){
		// выбор типа скила
		switch (spellType){
			case SO_0_SKILL:	spellMask = player.AvailableSkillMask;	break;// индивидуальный скилл героя
			case SO_1_SPELL:	spellMask = player.AvailableSpellMask;	break;//	spell
			case SO_2_RELIC:	spellMask = player.AvailableRelictMask;	break;//	Relict
			case SO_3_EQUIPED_ITEM:	spellMask = player.AvailableChargesMask;	break;//	заклинание вещи
		}
		spellSelector = 1i64;
		for( int spellNumber = 1; spellNumber < PS_COUNT; spellNumber++, spellSelector <<= 1){
			if( !(spellSelector & spellMask) ){// Если закл не выучен
				continue;
			}
			// Если заклинание выучено
			if( spellNumber == player.CurrentSpellIndex && spellType == player.SpellType ){  // если это текущий спелл позиционируем курсор на текущий спелл
				newMouseX = surfaceX - 70;
				newMouseY = surfaceY - 188;
				ChangeCursorPos(newMouseX - CursorX, newMouseY - CursorY);
				return;
			}
			//	передвигаемся по панели выбора спелла
			surfaceX -= SpellSelect_Size_56; // Следующий спэлл находится на 56 пикселей правее
			if( surfaceX <= surfaceXMin ){// Если правее некуда, значит 
				surfaceY -= SpellSelect_Size_56;// на 56 пикселей выше
				surfaceX = surfaceXMax;// а по ширине возвращаемся вправо
			}// 56 скорее всего размер иконки плюс отступ, 20 - отступ слева, а (GUI_Width - 636) это отступ справа
		}

		if( spellMask && surfaceX != surfaceXMax ){// Если хоть одно заклинание из группы имеется И текущая позиция не самая крайняя
			surfaceX -= SpellSelect_Size_56;// Делаем отступ размером в одну иконку по х
			if( surfaceX <= surfaceXMin ){// Если правее некуда, значит 
				surfaceY -= SpellSelect_Size_56;// на 56 пикселей выше
				surfaceX = surfaceXMax;// а по ширине возвращаемся вправо
			}// 56 скорее всего размер иконки плюс отступ, 20 - отступ слева, а (GUI_Width - 636) это отступ справа
		}
	}
	ChangeCursorPos(newMouseX - CursorX, newMouseY - CursorY);
}

//----- (00404F33) -------------------------------------------------------- interface
void CheckMainPanelRegion()
{
	for( auto& b: Buttons ){
		if( b.active && CursorIntoMainPanelButton(b) ){ // Если мышкой попали в кнопку
			ButtonPress(b);// Делаем её нажатой
			return;
		}
	}
	if( !SelectCurSpellMode && CursorIntoSpellIcon() ){// Если навелись на иконку спэла и сейчас не режим выбора заклинания 
		#ifdef AUTO_MOVE_CURSOR
		SpellPanel_SetupCursorPos(); // в TH 1 сейчас отключено
		#else
		SelectCurSpellMode = 1;                          // запускаем режим выбора заклинания
		#endif
		ResetMainMenu();
	}
}

//----- (00404FD5) -------------------------------------------------------- interface
void __fastcall ButtonPress(Button& b)
{
	b.pressed = 1;
	MainPanelButtonFlag = 1;
}

//----- (00404FEA) -------------------------------------------------------- interface
void Check_MenuORSendMessage()
{
	if( auto& b = Buttons[B_MENU]; CursorIntoMainPanelButton(b) ) ButtonPress(b);
	if( auto& b = Buttons[B_TALK]; CursorIntoMainPanelButton(b) ) ButtonPress(b);
}

//----- (0040509A) -------------------------------------------------------- interface
void PopUpHelpOnButton()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();

	for( auto& b: Buttons ){
		if( !b.active || !CursorIntoMainPanelButton(b) ) continue;
		char* header = b.name;
		switch( b.id ){
		case B_PVP: if( IsPlayerFriendly ) header = "Player friendly"; break;
		case B_SYNERGY: if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_22_BLACK_DEATH) ) header = "Symptoms"; break;
		}
		strcpy(InfoPanelHeader, header);
		if( b.keyName ){
			sprintf(InfoPanelBuffer, "Hotkey : %s", b.keyName);
			InfoPanel_AddLine(InfoPanelBuffer, true);
		}
		CurFontColor = 0;
		MaybeNeedDrawInfoPanel = 1;
		InfoPanelUsed = 1;
	}

	if( SelectCurSpellMode || !CursorIntoSpellIcon() ){// Если навелись не на иконку спэла или сейчас режим выбора заклинания 
		if (!TalkPanelMode && CursorIntoBelt()) { // если курсор попал на пояс
			Cur.InventoryCellID = GetSlotMouseAbove_Inv();
			Tooltip_ShowItemInfo(Cur.InventoryCellID);
		}
		return;
	}

	// Если навелись на иконку спэла
	strcpy(InfoPanelHeader, "Select current spell button");
	CurFontColor = 0;
	MaybeNeedDrawInfoPanel = 1;
	InfoPanelUsed = 1;
#ifdef comment
	strcpy(InfoPanelBuffer, "Hotkey : 's'");
	InfoPanel_AddLine(InfoPanelBuffer, true);
#endif
	if( Players[CurrentPlayerIndex].CurrentSpellIndex != -1 ){
		switch (Players[CurrentPlayerIndex].SpellType){
		case SO_0_SKILL: FormatedOutputPlayerSkillString ( Players[CurrentPlayerIndex].CurrentSpellIndex, 1 ); break;
		case SO_1_SPELL: FormatedOutputSpellString		 ( Players[CurrentPlayerIndex].CurrentSpellIndex, 1 ); break;
		case SO_2_RELIC: FormatedOutputRelicString		 ( Players[CurrentPlayerIndex].CurrentSpellIndex, 1 ); break;
		case SO_3_EQUIPED_ITEM: FormatedOutputStaffChargesString( Players[CurrentPlayerIndex].CurrentSpellIndex, 1 ); break;
		}
	}
}

//----- (th2) -------------------------------------------------------------
void ToggleCharPanel()
{
    // Conflicts with
    {
        IsQUESTPanelVisible = false;
        IsPerksPanelVisible = false;
        IsStashPanelVisible = false;
        IsInfoWindowVisible = false;
        VisualTrade_Close();
        Craft_Close();
		Enchant_Close();
		Quench_Close();
    }

    IsCHARPanelVisible = !IsCHARPanelVisible;
	PlayGlobalSound(75);
}

//----- (th2) -------------------------------------------------------------
void ToggleQuestPanel()
{
    { // Conflicts with
        IsCHARPanelVisible = false;
        IsPerksPanelVisible = false;
        IsStashPanelVisible = false;
        IsInfoWindowVisible = false;
        VisualTrade_Close();
        Craft_Close();
		Enchant_Close();
		Quench_Close();
    }
    if( IsQUESTPanelVisible ){
        IsQUESTPanelVisible = false;
    }else{
        QuestPanel_Open();
    }
	PlayGlobalSound(75);
}

//----- (th2) -------------------------------------------------------------
void ToggleInventoryPanel()
{
	if (/*Berserk*/HasTrait(CurrentPlayerIndex, TraitId::Psychotic) && (Players[CurrentPlayerIndex].activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN )
		return;

	IsINVPanelVisible ? CloseInventoryPanel() : OpenInventoryPanel();
	PlayGlobalSound(75);

}

//----- (th2) -------------------------------------------------------------
void OpenPerksPanel()
{
    { // Conflicts with
        IsSpellBookVisible = false;
        IsQUESTPanelVisible = false;
        IsCHARPanelVisible = false;
        IsStashPanelVisible = false;
        VisualTrade_Close();
        Craft_Close();
		Enchant_Close();
		Quench_Close();
		CloseInventoryPanel();
		InfoWindow_Close();
    }
	IsPerksPanelVisible = true;
	if( !RememberPerkSelection ) memzero(PerkPanelLine);
	getAvailablePerksList();
}

//----- (th2) -------------------------------------------------------------
void ClosePerksPanel()
{
    IsPerksPanelVisible = false;
}

//----- (th2) -------------------------------------------------------------
void TogglePerkTypePanel(PERK_TYPE panel)
{
	if (IsPerksPanelVisible) {
		if( CurPerkPanel != panel ){
			CurPerkPanel = panel;
			OpenPerksPanel();
		}else{
			ClosePerksPanel();
		}
	}else{
		CurPerkPanel = panel;
		OpenPerksPanel();
	}
	PlayGlobalSound(75);
}

//----- (th2) -------------------------------------------------------------
void TogglePerksPanel()
{
	if( GameMode == GM_CLASSIC ) return;
	TogglePerkTypePanel(PERK_TYPE_COMMON);
}

//----- (th2) -------------------------------------------------------------
void ToggleSynergyPanel()
{
	if( GameMode == GM_CLASSIC ) return;
	TogglePerkTypePanel((Players[CurrentPlayerIndex].gameChanger & BIT(GC_22_BLACK_DEATH)) ? PERK_TYPE_SYMPTOM : PERK_TYPE_SYNERGY);
}

//----- (th2) -------------------------------------------------------------
void OpenInfoWindow()
{
	IsSpellBookVisible = false;
	IsQUESTPanelVisible = false;
	IsCHARPanelVisible = false;
	IsStashPanelVisible = false;
	VisualTrade_Close();
	Craft_Close();
	Enchant_Close();
	Quench_Close();
	CloseInventoryPanel();
	ClosePerksPanel();

	InfoWindow_Open();
}

//----- (th2) -------------------------------------------------------------
void CloseInfoWindow()
{
	IsInfoWindowVisible = false;
}

//----- (th2) -------------------------------------------------------------
void ToggleInfoWindow()
{
	IsInfoWindowVisible ? CloseInfoWindow() : OpenInfoWindow(); 
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void  TogglePvP()
{
	IsPlayerFriendly = ! IsPlayerFriendly;
	Players[CurrentPlayerIndex].IsPlayerEnemy = !IsPlayerFriendly;//Update current player's PvP state LOCALLY, then force sync elsewhere
	SendCmdPvP(0, CurrentPlayerIndex, Players[CurrentPlayerIndex].IsPlayerEnemy);
	Buttons[B_PVP].pic[0] = IsPlayerFriendly ? 3 : 5;
	Buttons[B_PVP].pic[1] = IsPlayerFriendly ? 4 : 6;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th3) -------------------------------------------------------------
bool PlayerFriendly()
{
	return IsPlayerFriendly && GameMode != GM_COLISEUM;
}

//----- (00405408) -------------------------------------------------------- interface
void MainPanelButtonControl()
{
	bool resetMainMenu = 1;
	MainPanelButtonFlag = 0;
	for( auto& b: Buttons ){
		if( !b.active ) continue;
		if( b.pressed && CursorIntoMainPanelButton(b) ){ // button pressed down and up on it
			b.action();
			if( b.mainMenu ) resetMainMenu = 0;
		}
		b.pressed = 0;
	}
	if( resetMainMenu ){
		ResetMainMenu();
	}
}

//----- (004055AA) -------------------------------------------------------- interface
void FreePanels()
{
	///////////// MainPanel
	FreeMemZero(ManaShereImage);
	FreeMemZero(LifeShereImage);
	FreeMemZero(MainPanelImage);
	FreePanelButtons();
	FreeMemZero(FontSpriteSmall);
	FreeMemZero(Data_CharCEL);
	FreeMemZero(Data_SpelIconCEL_2);
	FreeMemZero(Data_SpelIconCEL);
	FreeMemZero(Data_HotkeyIconsCEL);
	FreeMemZero(Monster_Bar_Border);
	FreeMemZero(Monster_Bar_MonIcons);
	FreeMemZero(Monster_Bar_ImIcons);
	FreeMemZero(Data_CharButCEL);
	FreeMemZero(Items_DurIconsCEL);
	FreeMemZero(Items_DurIconsCEL2);
	#if th2_crucifixion
	FreeMemZero( CrestImageCEL );
	#endif
	FreeMemZero(Data_QuestCEL);
	FreeMemZero(Data_ExclamCEL);
	FreeMemZero(Data_SpellBkCEL);
	FreeMemZero(Data_SpellBkBCEL);
	FreeMemZero(Data_SpellI2CEL_2);
	FreeMemZero(Data_SpellI2CEL);
	FreeMemZero(CtrlPan_GolddropCEL);
	InfoWindow_Free();
}

//----- (004056BF) --------------------------------------------------------
int __fastcall CheckLetterWidth(char* StrPtr)
{
	int letterWidth = 0;
	while( *StrPtr ){
		letterWidth += FontWidthSmall[FontIndexSmall[Codepage[*StrPtr++]]];
		if( letterWidth >= 125 ){
			return 0;
		}
	}
	return 1;
}

//----- (00778A00) --------------------------------------------------------
void ShowSign()
{
	//Tooltip_Clear();
	if( Cur.ItemID == -1 && CurMon == -1
		&& Cur.ObjectIndex == -1 && Cur.playerIndex == -1 && CurSign != -1 ){
		
		if (CurSign < 10 && (Dungeon->genType || Dungeon->isQuest) ) //dont show signs (graves) in dungeons except IronMan Memorial which is displayed on level 2 IM SP only
			return;
#if 0
			switch (CurSign) {
				case TOMB_4: Tooltip_Clear();
						Tooltip_AddLine("Here lies Oracle Raven,");
						Tooltip_AddLine("wishing you luck on your journey");
						break;
				case TOMB_3: Tooltip_Clear();
						Tooltip_AddLine("Here lies a thing");
						Tooltip_AddLine("a quiet madness made");
						break;
				case TOMB_2: Tooltip_Clear();
						Tooltip_AddLine("Here lies Rageweaver,");
						Tooltip_AddLine("the God Of The Arena.");
						break;
				case TOMB_1: Tooltip_Clear();
						Tooltip_AddLine("here lies mordor");
						Tooltip_AddLine("an outsider in life");
						Tooltip_AddLine("...and in afterlife");
						break;
				case TOMB_0:
						Tooltip_Clear(); 
						Tooltip_AddLine("here lies the programmer");
						Tooltip_AddLine("buried under the code");
						Tooltip_AddLine("fragments of this game");
						Tooltip_AddLine("rest in peace, desert dragon");
						break;
				case MEMORIAL_IRONMAN:
						//Tooltip_AddLine("hall of heroes");
						Tooltip_Clear(); 
						Tooltip_AddLine("ironman tournament heroes memorial");
						Tooltip_AddLine("q3'2019: evil_bl");
						Tooltip_AddLine("q4'2019: redear");
						Tooltip_AddLine("q1'2020: thraex_pig");
						Tooltip_AddLine("q2'2020: deaf-dangluh");
						Tooltip_AddLine("q3'2020: thraex_pig");
						Tooltip_AddLine("q4'2020: deaf-dangluh");
						Tooltip_AddLine("q1'2021: redear");
						Tooltip_AddLine("q2'2021: thraex_pig");
						Tooltip_AddLine("q3'2021: thraex_pig");
						Tooltip_AddLine("q1'2022: N1kotin");
						Tooltip_AddLine("q2'2022: deaf-dangluh");
						break;
				case MEMORIAL_SPEEDRUN:
						Tooltip_Clear();
						Tooltip_AddLine("speedrun tournament heroes memorial");
						Tooltip_AddLine("q4'2020: redear");
						Tooltip_AddLine("q1'2021: contrique, secutor, 1:39:24");
						Tooltip_AddLine("q2'2021: redear, sharpshooter, 3:16:25");
						Tooltip_AddLine("q3'2021: alukret, necromancer, 2:28:55");
						Tooltip_AddLine("q1'2022: deaf-dangluh, nightkin, 0:26:16");
						break;

			}
#endif
		}
}

//----- (004056F0) -------------------------------------------------------- interface
void DrawInfoPanel()
{
    bool isCursorOnBelt = Cur.InventoryCellID >= IS_50_47_Belt_Start && Cur.InventoryCellID < IS_50_47_Belt_End;
	if( !IsInterface || Cur.InventoryCellID >= 0 /*&& !isCursorOnBelt*/ ) return;
 
	#ifdef STRANGE_ORIGINAL_CODE
	CopyFromMainPanelToWorkingSurface( 177, 62, 288u, 60u, 241, 558 );
	#endif
	if( !MaybeNeedDrawInfoPanel && !IswarpUnderCursor && Cur.InventoryCellID == -1 ){
		if( SelectCurSpellMode ){
		LABEL_33:
			CurFontColor = C_0_White;
			goto LABEL_34;
		}
		InfoPanelHeader[ 0 ] = 0;
		CurFontColor = C_0_White;
		InfoPanel_ClearBody();
	}
	if( SelectCurSpellMode || IswarpUnderCursor ){
		goto LABEL_33;
	}
	if( Cur.GraphicsID < 12 ){
		if( Cur.ItemID != -1 ){
			Item_ShowInfo( Cur.ItemID );
		}
		if( Cur.ObjectIndex != -1 ){
			IdentifyObjectOnMapInFocus( Cur.ObjectIndex );
		}
		if( CurMon != -1 ){
			if( Dungeon->genType || Dungeon->isQuest ){
				int monsterIndex = CurMon;
				Monster& monster = Monsters[monsterIndex];
				if( monsterIndex < SummonMonsters_Count ){
				    const Player& owner = Players[ monsterIndex / SummonMonstersPerPlayer_Count ];
					CurFontColor = C_3_Gold;
					sprintf( InfoPanelHeader, "%s's %s", owner.playerName, monster.Name );
					InfoPanel_ClearBody();
					sprintf( InfoPanelBuffer, "Hit Points %i of %i", monster.CurrentLife >> 6, monster.BaseLife >> 6 );
					InfoPanel_AddLine( InfoPanelBuffer, 1 );
					sprintf(InfoPanelBuffer, "dmg: %u-%u, ac: %u, hit: %u", monster.MinDamage, monster.MaxDamage, monster.ArmorClass, monster.ToHit);
					InfoPanel_AddLine(InfoPanelBuffer, 1);
					goto LABEL_34;
				}
				if( monster.ChampionNumber ){
					CurFontColor = C_1_Blue;
					sprintf( InfoPanelHeader, ChampionsNameTable[monster.ChampionNumber - 1], monster.Name );
				}else{
					CurFontColor = C_0_White;
					strcpy( InfoPanelHeader, monster.Name );
				}
				InfoPanel_ClearBody();
				if( Monsters[ monsterIndex ].newBossId ){
					CurFontColor = C_3_Gold;
					ShowBossHP();
				}else{
					if( monster.hisBossIndex ){
						sprintf( InfoPanelHeader, "%s (minion)", monster.Name );
					}
					ShowMonsterInfo(monsterIndex);
				}
			}else{
				strcpy( InfoPanelHeader, Towners[ CurMon ].Name );
			}
		}
		if( Cur.playerIndex != -1 ){
			CurFontColor = C_3_Gold;
			strcpy( InfoPanelHeader, Players[ Cur.playerIndex ].playerName );
			InfoPanel_ClearBody();
			sprintf(InfoPanelBuffer, "%s, Level : %i", GetFullClassName(Cur.playerIndex), Players[ Cur.playerIndex ].CharLevel );
			InfoPanel_AddLine( InfoPanelBuffer, 1 );
			sprintf( InfoPanelBuffer, "Hit Points %i of %i", Players[ Cur.playerIndex ].CurLife >> 6, Players[ Cur.playerIndex ].MaxCurLife >> 6 );
			InfoPanel_AddLine( InfoPanelBuffer, 1 );
		}
		ShowSign(); // signes patch (4)
	}else{
		Player& player = Players[ CurrentPlayerIndex ];
		Item& curItem = player.ItemOnCursor;
		if( curItem.ItemCode == IC_11_GOLD ){
			char* goldStr = formatGoldAmount( curItem.amount );
			sprintf( InfoPanelHeader, "%i gold %s", curItem.amount, goldStr );
		}
		else if (curItem.IsReqMet) {
			const bool identified = curItem.Identified || curItem.MagicLevel == ML_0_USUAL;
			strcpy(InfoPanelHeader, identified ? curItem.FullMagicalItemName : curItem.Name);
			CurFontColor = ItemColor(curItem);
		}else{
			InfoPanel_ClearBody();
			InfoPanel_AddLine( "Requirements not met", 1 );
			InfoPanelUsed = 1;
		}
	}
LABEL_34:
	if( InfoPanelHeader[ 0 ] || InfoPanelLinesUsed ){
		InfoPanel_Draw();
	}
}

//----- (004059B8) -------------------------------------------------------- interface
void InfoPanel_Draw()
{
	if( !TalkPanelMode ){
		int startIndex = 0;
		int infoLinesCount = InfoPanelLinesUsed - 1;
		if( InfoPanelHeader[0] != '\0' ){
			++infoLinesCount;
			InfoPanel_DrawLine(0, InfoPanelHeader, true, infoLinesCount);
			startIndex = 1;
		}

		for( int i = 0; i < InfoPanelLinesUsed; ++i ){
			InfoPanel_DrawLine(startIndex + i, (char *)InfoPanel4Lines[i], InfoPanelLinesCentered[i] != 0, infoLinesCount);
		}
	}
}

//----- (00405A2E) --------------------------------------------------------
void __fastcall InfoPanel_DrawLine(int line, char *text, bool centered, int infoLinesCount)
{
	// отступ по высоте основанный на общем числе линий и индексе текущей линии
	int lineYOffset = InfoLineYOffsets[infoLinesCount][line];
	// положение текста x = 241	 y = 594 в оригинале
	int yOffset = Screen_TopBorder + 434 + lineYOffset + (ScreenHeight - GUI_Height); 
	int defaultPos = YOffsetHashTable[yOffset] + Screen_LeftBorder + 177 + (ScreenWidth - GUI_Width)/2;
	int surfaceOffset = defaultPos;

	int offset = 0;
	if( centered ){
		int textLength = 0;
		for( int i = 0; text[i]; i++ ){// Вычисляем длинну строки на экране
			textLength += FontWidthSmall[FontIndexSmall[Codepage[text[i]]]] + 2;
		}
		if( textLength < 288 ){
			offset = (288 - textLength) / 2;
		}
		surfaceOffset += offset;
	}
	for( int i = 0; text[i]; i++ ){
		uchar currentLetter = FontIndexSmall[Codepage[text[i]]];
		offset += FontWidthSmall[currentLetter] + 2;
		if( currentLetter && offset < 288 ){
			DrawLetter(surfaceOffset, currentLetter, CurFontColor);
		}
		surfaceOffset += FontWidthSmall[currentLetter] + 2;
	}
}

// Draw level info on automap (level/gamename)
//----- (00405AEC) --------------------------------------------------------
int __fastcall DrawLevelInfoText(int xPos, int yPos, const char* stringPtr, int fontColor, bool outline /*= false*/)
{
	int textWidth = 0;
	char result;       // al@1
    const char* StringPtr;   // edi@1
	int SurfaceOffset; // esi@1
	char LetterID;     // bl@2
	StringPtr = stringPtr;
	SurfaceOffset = YOffsetHashTable[yPos + Screen_TopBorder] + xPos + Screen_LeftBorder;
	for( result = *stringPtr; *StringPtr; result = *StringPtr ){
		++StringPtr;
		LetterID = FontIndexSmall[Codepage[result]];
		if( LetterID )
			DrawLetter(SurfaceOffset, (unsigned __int8)LetterID, fontColor, outline);
		int letterWidth = FontWidthSmall[LetterID] + 1;
		SurfaceOffset += letterWidth;
		textWidth += letterWidth;
	}
	return textWidth;
}

//----- (004065E2) --------------------------------------------------------
void __fastcall DrawText(int xPos, int yPos, int xPosEnd, const char* text, int fontColor)
{
	int offset = YOffsetHashTable[yPos + Screen_TopBorder] + xPos + Screen_LeftBorder;

	if( xPosEnd > 0 ){
		int maxWidth = xPosEnd - xPos + 1;
		int textWidth = 0; auto t = text;
		while( char c = *t++ ) textWidth += FontWidthSmall[FontIndexSmall[Codepage[c]]] + 1;
		if( textWidth < maxWidth ){ // Center text if necessary
			offset += (maxWidth - textWidth) / 2;
		}
	}

	while( char c = *text++ ){
		char l = FontIndexSmall[Codepage[c]];
		if( l ) DrawLetter(offset, l, fontColor);
		offset += FontWidthSmall[l] + 1;
	}
}

// Draws text for dmg in char window (only use)
// Parameter scale can be 0 or -1 to "pack" letters (if dmg string is too wide ie. xxx-yyy)
//----- (00406699) --------------------------------------------------------
char __fastcall DrawTextWithScale(int xPos, int yPos, int xPosEnd, char* text, char fontSize, int scale)
{
	int textLength = 0; // edx@1
	char* textTmp;      // edi@1
	int maxWidth;       // esi@1
	char currentLetter; // al@8
	int offset;         // [sp+Ch] [bp-4h]@1
	offset = YOffsetHashTable[yPos + Screen_TopBorder] + xPos + Screen_LeftBorder;
	maxWidth = xPosEnd - xPos + 1;
	textTmp = text;
	if( *text ) // get text length
	{
		while( *textTmp ){
			textLength += scale + FontWidthSmall[FontIndexSmall[Codepage[*textTmp]]];
			textTmp++;
		}
	}
	textTmp = text;             // end
	if( textLength < maxWidth ) // Center the text if necessary
	{
		offset += (maxWidth - textLength) >> 1;
	} //end
	while( *textTmp ){
		currentLetter = FontIndexSmall[Codepage[*textTmp]];
		if( currentLetter ){
			DrawLetter(offset, currentLetter, fontSize);
		}
		offset += FontWidthSmall[currentLetter] + scale;
		textTmp++;
	}
	return *(--textTmp);
}

//----- (00406758) -------------------------------------------------------- interface
void CheckLevelUpButton()
{
	if( !IsCursorOnLevelUpButton ){
		IsCursorOnLevelUpButton = CursorIntoLevelUpButton ();
	}
}

//----- (0040678E) -------------------------------------------------------- interface
void LevelUPButton_MouseClick()
{
	if( CursorIntoLevelUpButton() ){
	    if( !IsCHARPanelVisible ){
	        ToggleCharPanel();
        }
    }
	IsCursorOnLevelUpButton = false;
	PlayGlobalSound(75);
}

//----- (th2) -------------------------------------------------------------
void __fastcall DrawPlusCEL(int x, int y, int pic)
{
	x += Screen_LeftBorder;
	y += Screen_TopBorder;
	if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) ){
		DrawRegularCelViaTRN(x, y, Data_CharButCEL, pic, 41, CharbutIceage);
	}else{
		Surface_DrawCEL(x, y, Data_CharButCEL, pic, 41);
	}
}

//----- (004067C2) -------------------------------------------------------- interface
void DrawLevelUpButton()
{
	if( CurrentDialogIndex ){
		return;
	}
	int picNumber;
	if( !IsCursorOnLevelUpButton ){
		picNumber = 2;
	}else{
		picNumber = 3;
	}
	DrawText((ScreenWidth - GUI_Width)/2, 303+ ScreenHeight - GUI_Height, 120 + (ScreenWidth - GUI_Width)/2, "Stats", 0);
	DrawPlusCEL(40 + (ScreenWidth - GUI_Width)/2, 335 + ScreenHeight - GUI_Height, picNumber);
}

const int perksUpXOffset = 530;

//----- (th2) -------------------------------------------------------------
bool CursorIntoPerkUpButton()
{
    int buttonLeft = perksUpXOffset + 40 + (ScreenWidth - GUI_Width) / 2;
    int buttonUp = 313 + ScreenHeight - GUI_Height;
    int buttonWidth = 41;
    int buttonHeight = 22;
    int buttonRight = buttonLeft + buttonWidth;
    int buttonDown = buttonUp + buttonHeight;

    return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

//----- (th2) -------------------------------------------------------------
void CheckPerkUpButton()
{
    if( !IsCursorOnPerkUpButton ){
        IsCursorOnPerkUpButton = CursorIntoPerkUpButton();
    }
}

//----- (th2) -------------------------------------------------------------
void PerkUPButton_MouseClick()
{
    if( CursorIntoPerkUpButton() )
    {
        CurPerkPanel = PERK_TYPE_COMMON;
		OpenPerksPanel();
		PlayGlobalSound(75);
    }
    IsCursorOnPerkUpButton = false;
}

//----- (th2) -------------------------------------------------------------
void DrawPerkUpButton()
{
    if( CurrentDialogIndex ){
        return;
    }
    int picNumber;
    if( !IsCursorOnPerkUpButton ){
        picNumber = 2;
    }else{
        picNumber = 3;
    }

    DrawText(perksUpXOffset + (ScreenWidth - GUI_Width)/2, 303+ ScreenHeight - GUI_Height, perksUpXOffset + 120 + (ScreenWidth - GUI_Width)/2, "Perks", 0);
	DrawPlusCEL(perksUpXOffset + 40 + (ScreenWidth - GUI_Width)/2, 335 + ScreenHeight - GUI_Height, picNumber);
}

//----- (004069B9) --------------------------------------------------------
void DrawAllDamagedItemsPics()
{
	const Player& player = Players[CurrentPlayerIndex];
    
		// расположение на DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT левее края экрана
    int screenX = GUI_Width + Screen_LeftBorder - (DAMAGED_ITEMS_PICS_INDENT_FROM_RIGHT + DAMAGED_ITEMS_PICS_WIDTH) + (ScreenWidth - GUI_Width);

		// screenX меняется внутри функции по ссылке. Сделано для удобства
		DrawOneDamagedItemPic(player.OnBodySlots[IS_Head],			screenX, DIP_4_HELM);
		DrawOneDamagedItemPic(player.OnBodySlots[IS_Torso],			screenX, DIP_3_BODY_ARMOR);
		DrawOneDamagedItemPic(player.OnBodySlots[IS_Feet],			screenX, DIP_20_FEET );
		DrawOneDamagedItemPic(player.OnBodySlots[IS_Waist],			screenX, DIP_18_WAIST );
		DrawOneDamagedItemPic(player.OnBodySlots[IS_Hands],			screenX, DIP_22_HANDS );
		DrawOneDamagedItemPic(player.OnBodySlots[IS_LeftHand],		screenX, DIP_0_UNKNOWN);
		DrawOneDamagedItemPic(player.OnBodySlots[IS_RightHand],		screenX, DIP_0_UNKNOWN);
}

//----- (00406A3F) --------------------------------------------------------
void __fastcall DrawOneDamagedItemPic(const Item& item, int& screenX, int picNumber)
{
	const int LOW_DAMAGED_DURABILITY = 6;
	const int HIGH_DAMAGED_DURABILITY = 1;

	int resolutionDifferenceHeight = ScreenHeight - GUI_Height;
	int screenY = (ScreenHeight - DAMAGED_ITEMS_PICS_HEIGHT) / 2;//495;
	
	int itemCode = item.ItemCode;
	int itemDurability = item.CurDurability;

	// вещи нет или она не сломана настолько чтобы предупреждать
	if( itemCode == IC_M1_NONE || itemDurability > LOW_DAMAGED_DURABILITY  ){
		return;
	}

	// если вещь в руке, то нужно определить её тип для показа нужной картинки
	if( picNumber == DIP_0_UNKNOWN ){
		if( item.TypeID == ITEM_1_WEAPON ){
			switch (itemCode ){
				case IC_1_SWORD:	picNumber = DIP_2_SWORD;	break;
				case IC_2_AXE:		picNumber = DIP_6_AXE;		break;
				case IC_3_BOW:		picNumber = DIP_7_BOW;		break;
				case IC_4_MACE:		picNumber = DIP_5_MACE;		break;
				case IC_10_STAFF:	picNumber = DIP_8_STAFF;	break;
                case IC_18_FLASK:	picNumber = DIP_24_FLASK;	break;
				case IC_19_TRAP:	picNumber = DIP_26_TRAP;	break;
				case IC_20_CLAW:	picNumber = DIP_28_CLAW;	break;
				case IC_21_KNIFE:	picNumber = DIP_30_KNIFE;	break;
				case IC_22_MALLET:	picNumber = DIP_5_MACE;		break;
				case IC_23_PISTOL:	picNumber = DIP_32_PISTOL;	break;
			}
		}else{
			picNumber = DIP_1_SHEELD;
		}
	}

	// если сломаны не критически то меняем цвет картинки на жёлтый (с 9ю по 16)
	if( itemDurability > HIGH_DAMAGED_DURABILITY ){
		if( picNumber > DIP_8_STAFF ){
			--picNumber;
		}else{
			picNumber += DIP_8_NUMBER_OF_DIFFERENT_PICS;
		}
	}

	// рисуем картинку на экран
	if( picNumber > 16 ){
		Surface_DrawCEL( screenX, screenY + resolutionDifferenceHeight, Items_DurIconsCEL2, picNumber, DAMAGED_ITEMS_PICS_WIDTH );
	}else {
		Surface_DrawCEL(screenX, screenY + resolutionDifferenceHeight, Items_DurIconsCEL, picNumber, DAMAGED_ITEMS_PICS_WIDTH);
	}
	// меняем значение по ссылке чтобы следующая картинка рисовалась с отступом в DAMAGED_ITEMS_PICS_INDENT левее текущей
	screenX -= DAMAGED_ITEMS_PICS_INDENT + DAMAGED_ITEMS_PICS_WIDTH;
}

//----- (00406AB8) -------------------------------------------------------- interface
int DrawRedScreen()
{
	int _EAX;          // eax@1
	int v1;            // edx@2
	unsigned char* v3; // edi@2
	int v4;            // ecx@3
	int v5;            // edx@7
	unsigned char* v7; // edi@7
	int v8;            // ecx@8
	int v9;            // eax@1
	int v10;           // [sp+8h] [bp-4h]@1
	v9 = -(Light4Levels != 0);
	LOWORD_IDA(v9) = v9 & 0xF400;
	_EAX = v9 + 4608;
	v10 = _EAX;
	_EAX = 0;
	if( Dungeon->genType == DT_4_HELL ){
		v7 = (unsigned char*)&WorkingSurface[WorkingWidth * Screen_TopBorder + Screen_LeftBorder];
		unsigned int _EBX = (unsigned)&LightTable[v10];
		v5 = ScreenHeight;
		do{
			v8 = ScreenWidth;
			do{
				LOBYTE_IDA(_EAX) = *v7;
				if( (unsigned __int8) * v7 >= 0x20u )
					LOBYTE_IDA(_EAX) = ((uchar*)_EBX)[_EAX&0xFF]; // XLAT
				*v7++ = _EAX;
				--v8;
			}while( v8 );
			v7 += Screen_LeftBorder + Screen_RightBorder;
			--v5;
		}while( v5 );
	}else{
		v3 = (unsigned char*)&WorkingSurface[WorkingWidth * Screen_TopBorder + Screen_LeftBorder];
		unsigned int _EBX = (unsigned)&LightTable[v10];
		v1 = ScreenHeight;
		do{
			v4 = ScreenWidth;
			do{
				LOBYTE_IDA(_EAX) = *v3;
				LOBYTE_IDA(_EAX) = ((uchar*)_EBX)[_EAX&0xFF]; // XLAT
				*v3++ = _EAX;
				--v4;
			}while( v4 );
			v3 += Screen_LeftBorder + Screen_RightBorder;
			--v1;
		}while( v1 );
	}
	return _EAX;
}

//----- (00406B42) -------------------------------------------------------- player spell
int __fastcall GetColorNumberWithSpellBook(int spellNumber, int isSpellNotEnableInTown )
{
	Player& player = Players[CurrentPlayerIndex];
	__int64 spellSelector;
	int spellType = SO_1_SPELL;
	if( player.ClassID == PC_3_MONK && spellNumber == 46 ){
		return SO_0_SKILL;
	}
	spellSelector = 1i64 << (spellNumber - 1);
	if( spellSelector & player.AvailableChargesMask ){
		spellType = SO_3_EQUIPED_ITEM;
	}
	if( spellSelector & player.AvailableSkillMask ){
		spellType = SO_0_SKILL;
	}
	int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, spellNumber);

	bool isCurrentSpellIsSpell = spellType == SO_1_SPELL && spellNumber != -1;
	bool isSpellHasNotPositiveLevel = isCurrentSpellIsSpell && spellLevel <= 0;
	bool isManaNotEnoughtForSpell = isCurrentSpellIsSpell && !IsManaEnoughtForSpell(CurrentPlayerIndex, spellNumber, 1, 1);
	bool isSpellHasNegativeNumber = spellNumber < 0;

	bool isSpellNotEnable = isSpellHasNegativeNumber || isManaNotEnoughtForSpell || isSpellHasNotPositiveLevel;
	if( isSpellNotEnable ){ //!DungeonLevel && !Spells[spellNumber].IsEnabledInTown;){
		spellType = SO_4_NONE;
	}
	if (isSpellNotEnableInTown && !IsSpellEnabledHere(spellNumber, CurrentPlayerIndex)) {
		spellType = SO_4_NONE;
	}
	return spellType;
}

//----- (00714740) --------------------------------------------------------
POINT __fastcall InfoPanelManaHeal( int spellNum )
{
	Player& player = Players[CurrentPlayerIndex];
	int SLVL; // ecx@1
	int minHeal; // ecx@1
	int maxHeal; // edx@1
	unsigned int healMin; // ecx@1
	unsigned int healMax; // edx@1 OVERLAPPED
	int FC; // eax@1
	//unsigned int v8; // eax@5
	POINT result; // edx:ecx@10

	if (spellNum == PS_2_HEALING || spellNum == PS_34_HEAL_OTHER)      // Если Хил
	{

		SLVL = PlayerSpellLevel(CurrentPlayerIndex, spellNum);
		minHeal = player.BaseVitality / 2 + SLVL;
		maxHeal = SLVL * 2 + player.BaseVitality;
		healMin = minHeal + 1;
		healMax = maxHeal + 4;
		FC = player.fullClassId;
		if (is(FC, PFC_MAGE, PFC_ELEMENTALIST, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK)) {
			if (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				healMin /= 2;
				healMax /= 2;
			}
		}
		else if (FC == PFC_EXECUTIONER) {
			healMin += healMin / 2;
			healMax += healMax / 2;
		}
		healMin += healMin * (PerkValue(PERK_AUGMENTED_HEALING, CurrentPlayerIndex) + PerkValue(PERK_HEALING_MASTERY, CurrentPlayerIndex)) / 100;
		healMax += healMax * (PerkValue(PERK_AUGMENTED_HEALING, CurrentPlayerIndex) + PerkValue(PERK_HEALING_MASTERY, CurrentPlayerIndex)) / 100;
		result.y = healMax;
		result.x = healMin;
	}
	else if (spellNum == PS_37_MANA_RECHARGE)   // Если "реликт маны". Взято из: Missile.cpp (функция: CastManaRecharge)
	{
	//  Missile& missile = Missiles[missileIndex];

	//	int manaAddsCount = (RNG(4) + 1) * 64;
		int minManaAddCount = (0 + 1) * 64;
		int maxManaAddCount = (3 + 1) * 64;

		if (player.CharLevel > 0) {
			int i = 0;
			do {
	//			manaAddsCount += (RNG(2) + 1) * 64;
				minManaAddCount += (0 + 1) * 64;
				maxManaAddCount += (1 + 1) * 64;
			} while (++i < player.BaseMagic);
		}
		// 00433F4F
	//	for (int i = 0; i < missile.SpellLevel; i++) {   // ВОЗМОЖЕН БАГ !! (я не смог реализовать этот кусок, но на мой взгляд он и не нужен, так как у реликта маны нету дальности, нужно проверить)
	//		manaAddsCount += (RNG(2) + 1) * 64;                              
	//		minManaAddCount += (0 + 1) * 64;
	//		maxManaAddCount += (1 + 1) * 64;
	//	}
		// 00433F83
		int playerClass = player.ClassID;
		if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) {
			minManaAddCount = 7 * minManaAddCount / 10;
			maxManaAddCount = 7 * maxManaAddCount / 10;
		}
		else if (playerClass == PC_5_SAVAGE && player.fullClassId != PFC_DRUID) {
	//		manaAddsCount /= 9;
			minManaAddCount /= 9;
			maxManaAddCount /= 9;
		}
		else if (playerClass != PC_2_MAGE) {
	//		manaAddsCount /= 5;
			minManaAddCount /= 5;
			maxManaAddCount /= 5;
		}
		else {
	//		manaAddsCount = 4 * manaAddsCount / 10; // mage classes now have less mana recovered from Mana spell
			minManaAddCount = 4 * minManaAddCount / 10;
			maxManaAddCount = 4 * maxManaAddCount / 10;
		}
		result.y = maxManaAddCount / 64;
		result.x = minManaAddCount / 64;
	}
	return result;
}

//----- (004070D8) -------------------------------------------------------- player
char* __fastcall formatGoldAmount(int currentGold)
{
	return currentGold == 1 ? "piece" : "pieces";
}

//----- (0040745A) -------------------------------------------------------- player
void __fastcall SetGoldViewOnCursor(int playerIndex)
{
	Item& cur = Players[playerIndex].ItemOnCursor;
	if( cur.amount < Gold_2500_HiLimit ){
		if( cur.amount > Gold_1000_LowLimit ){
			cur.GraphicValue = Gold_5_MediumPic;
		}else{
			cur.GraphicValue = Gold_4_SmallPic;
		}
	}else{
		cur.GraphicValue = Gold_6_BigPic;
	}
	SetCursorGraphics( cur.GraphicValue + CM_12_ITEMS_PICS_START );
}

//----- (004074B3) -------------------------------------------------------- interface
void DrawTalkPanel()
{
	int v1;   // ebx@1
	int v4;   // esi@4
	char* v5; // eax@6
	int v6;   // esi@6
	int v7;   // esi@11
	char v8;  // bp@14
	int picNumber;   // eax@15
	int v10;  // [sp+4h] [bp-4h]@1
	int v11;  // [sp+0h] [bp-8h]@1
	v10 = 0;
	v11 = 0;
	v1 = 0;
	int playerIndex = 0;

	if( !TalkPanelMode ){
		return;
	}
	int startX = 111 + Screen_LeftBorder; // th 175
	CopyFromMainPanelToWorkingSurface(175, MainPanelSelector + 20, 294, 5, 175 + Screen_LeftBorder, 516); // Самая верхняя линия
	for( int i = 0; i < 10; i++ ){
		CopyFromMainPanelToWorkingSurface((i >> 1) + 175, i + MainPanelSelector + 25, 293 - i, 1, (i >> 1) + 175 + Screen_LeftBorder, i + 521);
	}
	CopyFromMainPanelToWorkingSurface(185, MainPanelSelector + 35, 274, 30, 185 + Screen_LeftBorder, 531);
	CopyFromMainPanelToWorkingSurface(180, MainPanelSelector + 65, 284, 5, 180 + Screen_LeftBorder, 561);
	v4 = 0;
	do{
		CopyFromMainPanelToWorkingSurface(180, v4 + MainPanelSelector + 70, (ushort)v4 + 284, 1u, 180 + Screen_LeftBorder, v4 + 566);
		++v4;
	}while( v4 < 10 );
	CopyFromMainPanelToWorkingSurface(170, MainPanelSelector + 80, 310, 55, 170 + Screen_LeftBorder, 576);
	v5 = TalkPanelMessage;
	v6 = 0;
	do{
		v5 = (char*)DrawTalkText((int)v5, 0, v6, &v10, 0);
		if( !v5 )
			goto LABEL_11;
		v6 += 13;
	}while( v6 < 39 );
	if( v5 )
		*v5 = 0;
LABEL_11:
	Surface_DrawCelTo(&WorkingSurface[v10], PentSpn2CELPtr, PentSpn2PicIndex, 12);// Крутящаяся пентаграмма курсора чата
	v11 = 0;
	v7 = 0;
	if( IsMainDraw ){
		PentSpn2PicIndex = (PentSpn2PicIndex & 7) + 1;
	}
	while( v11 == CurrentPlayerIndex ){
LABEL_22:
		++v11;
		v1 += sizeof(Player);
		playerIndex++;
		if( ++playerIndex > PlayersMax_4 ) return;
	}
	if( MayPlayersTalk[v11] ){
		v8 = 3;
		if( !TalkPanelButtonState[v7] ){
LABEL_19:
			if( Players[playerIndex].IsExists )
				DrawTalkText((int)&Players[playerIndex].playerName, 46, 18*v7 + 60, &v10, v8);
			++v7;
			goto LABEL_22;
		}
		picNumber = (v7 != 0) + 3;
	}else{
		picNumber = (v7 != 0) + 1;
		v8 = 2;
		if( TalkPanelButtonState[v7] )
			picNumber = (v7 != 0) + 5;
	}
	Surface_DrawCEL(172 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2, 18 * v7 + 596 + ScreenHeight - GUI_Height, CtrlPan_TalkButtCEL, picNumber, 61);
	goto LABEL_19;
}

// This is probably function handling rendering text in multiplayer chat
// Guess 2: maybe the "typing" console... looks more probable
// Guess 3: its probably something else coz the color of the font is gold ~~
//----- (004076C2) --------------------------------------------------------
int __fastcall DrawTalkText(int a1, int a2, int a3, int* a4, char fontColor)
{
	char v5;    // al@1
	int v6;     // ecx@1
	int v7;     // ebx@1
	int* v8;    // esi@1
	char v9;    // al@4
	int v10;    // edi@4
	int result; // eax@8
	int v12;    // edx@1
	int v13;    // ecx@4
	v8 = a4;
	v12 = a2 + 200 + Screen_LeftBorder + (ScreenWidth - GUI_Width)/2;
	v7 = a1;
	*a4 = v12 + YOffsetHashTable[a3 + 374 + Screen_TopBorder + ScreenHeight - GUI_Height ];
	v5 = *(uchar*)a1;
	v6 = v12;
	if( v5 ){
		while( 1 ){
			v9 = FontIndexSmall[Codepage[v5]];
			v10 = (unsigned __int8)v9;
			v13 = v6 + FontWidthSmall[v9] + 1;
			a3 = v13;
			if( v13 > 450 + Screen_LeftBorder /*+ 32*/ + (ScreenWidth - GUI_Width)/2 )
				break;
			++v7;
			if( v9 )
				DrawLetter(*v8, (unsigned __int8)v9, fontColor);
			*v8 += FontWidthSmall[v10] + 1;
			v5 = *(uchar*)v7;
			if( !*(uchar*)v7 )
				goto LABEL_8;
			v6 = a3;
		}
		result = v7;
	}else{
		LABEL_8:
		result = 0;
	}
	return result;
}

//----- (00407745) -------------------------------------------------------- interface
bool TalkPanel_CheckMouseClick()
{
	if( !TalkPanelMode ){
		return 0;
	}

	for( int buttonIndex = 0; buttonIndex < 3; buttonIndex++ ){
		if( CursorIntoTalkPanelButton(buttonIndex) ){
			TalkPanelButtonState[0] = buttonIndex == 0;
			TalkPanelButtonState[1] = buttonIndex == 1;
			TalkPanelButtonState[2] = buttonIndex == 2;
		}
	}

	return 0;
}

//----- (004077A0) -------------------------------------------------------- interface
void TalkPanel_LButtonUp()
{
	if( !TalkPanelMode ){
		return;
	}

	TalkPanelButtonState[0] = 0;
	TalkPanelButtonState[1] = 0;
	TalkPanelButtonState[2] = 0;

	if( CursorX >= 172 && CursorY >= 421 && CursorX <= 233 && CursorY <= 475 ){
		int v1 = (CursorY - 421) / 18;
		__int64 v2 = (CursorY - 421) % 18; //  *((_DWORD*)&v2 + 1) 
		
		do{
			if( v1 == -1 ){
				break;
			}
			if( *((uint*)&v2 + 1) != CurrentPlayerIndex ){
				--v1;
			}
			++*((uint*)&v2 + 1);
		}while( *((uint*)&v2 + 1) < 4 );
		
		if( *((uint*)&v2 + 1) <= 4 ){
			//MayPlayersTalk[*((_DWORD*)&v2 + 1)] = MayPlayersTalk[*((_DWORD*)&v2 + 1)] == 0;
			MayPlayersTalk[v2] = (MayPlayersTalk[v2] == false);
		}
	}
}

//----- (00407814) -------------------------------------------------------- interface infopanel
void OpenTalkPanel()
{
	if( MaxCountOfPlayersInGame == 1 ){ // Блокировка талк панели для сингла
		return;
	}
	TalkPanelMessage[0] = 0;
	TalkPanelMode = 1;
	PentSpn2PicIndex = 1;
	TalkPanelButtonState[0] = 0;
	TalkPanelButtonState[1] = 0;
	TalkPanelButtonState[2] = 0;
	MainPanelSelector = GUI_MainPanelHeight;
	CurTalkPanelIndex = TalkPanelIndex;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (0040785B) -------------------------------------------------------- interface infopanel
void CloseTalkPanel()
{
	TalkPanelMode = 0;
	MainPanelSelector = 0;
	PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) -------------------------------------------------------------
void  ToggleTalkPanel()
{
	TalkPanelMode ? CloseTalkPanel() : OpenTalkPanel();
}

//----- (00407874) -------------------------------------------------------- interface infopanel
bool __fastcall PutNewCharInTalkPanelPromt(unsigned int keyCode)
{
	if( TalkPanelMode && keyCode >= 0x20 ){
		size_t len = strlen(TalkPanelMessage);
		if( len < 78 ){
			TalkPanelMessage[len] = keyCode;
			TalkPanelMessage[len + 1] = 0;
		}
		return 1;
	}else{
		return 0;
	}
}

//----- (004078B4) -------------------------------------------------------- interface infopanel
bool __fastcall HandleTalkPanelControlKeys(int Key)
{
	if( !TalkPanelMode ) return 0;
	switch (Key){
	case VK_27_ESC_KEY:			CloseTalkPanel();					return 1;
	case VK_13_RETURN_KEY:		AddNewStringInTalkPanel();			return 1;
	case VK_40_DOWN_ARROW_KEY:	PutSomeSpecialCharInTalkPanel(1);	return 1;
	case VK_38_UP_ARROW_KEY:	PutSomeSpecialCharInTalkPanel(-1);	return 1;
	case VK_8_BACKSPACE_KEY:	if( size_t msgLen = strlen(TalkPanelMessage) ) TalkPanelMessage[msgLen - 1] = 0; return 1;
	default: return 0;
	}
}

//----- (0040791E) -------------------------------------------------------- interface infopanel
void AddNewStringInTalkPanel()
{
	int v0; // eax@2
	int v1; // ecx@2
	int index; // esi@6
	char v4; // al@10
	int v5; // ecx@11

	if( TalkPanelMessage[ 0 ] ){
		v0 = 0;
		v1 = 0;
		do{
			if( MayPlayersTalk[ v1 ] ){
				v0 |= 1 << v1;
			}
			++v1;
		}while( v1 < 4 );
        NetSendString( v0, TalkPanelMessage );
		for( index = 0; index < 8; ++index ){
			if( !strcmp( TalkPanelMessages[index], TalkPanelMessage ) ){
				break;
			}
		}
		if( index < 8 ){
			v4 = TalkPanelIndex;
			v5 = (TalkPanelIndex - 1) & 7;
			if( index != v5 ){
				strcpy( TalkPanelMessages[index], TalkPanelMessages[v5] );
				strcpy( TalkPanelMessages[v5], TalkPanelMessage );
				v4 = TalkPanelIndex;
			}
		}else{
			strcpy( TalkPanelMessages[TalkPanelIndex], TalkPanelMessage );
			v4 = (TalkPanelIndex + 1) & 7;
			TalkPanelIndex = (TalkPanelIndex + 1) & 7;
		}
		TalkPanelMessage[ 0 ] = 0;
		CurTalkPanelIndex = v4;
	}
	CloseTalkPanel();
}

//----- (004079F9) -------------------------------------------------------- interface infopanel
void __fastcall PutSomeSpecialCharInTalkPanel(char a1)
{
	unsigned __int8 v1; // al@1
	int v2; // esi@1

	v1 = CurTalkPanelIndex;
	v2 = 0;
	while( 1 ){
		v1 = (v1 + a1) & 7;
		CurTalkPanelIndex = v1;
		if( TalkPanelMessages[v1] ){
			break;
		}
		if( ++v2 >= 8 ){
			return;
		}
	}
	strcpy( TalkPanelMessage, TalkPanelMessages[v1] );
}

//----- (th3) ------------------------------------------------------------- interface infopanel
void SetupPanelRect(DisplayObject& displayObject, PanelLayout layout, int width, int height)
{
    const int panelsXOffset = 10;

    int x = 0;
    int y = (ScreenHeight - GUI_MainPanelHeight) / 2 - height / 2;
    
    switch(layout){
    case LeftPanel:
        if( PanelsOnCenter ){
            x = ScreenWidth / 2 - width - panelsXOffset;
        }else{
            x = ScreenWidth / 4 - width / 2;
        }
        break;
    case CenteredPanel:
        x = ScreenWidth / 2 - width / 2;
        break;
    case RightPanel:
        if( PanelsOnCenter ){
            x = ScreenWidth / 2 + panelsXOffset;
        }else{
            x = ScreenWidth * 3 / 4 - width / 2;
        }
        break;
    }

    LimitToRange(x, 0, ScreenWidth - width);
    LimitToRange(y, 0, ScreenHeight - height);

	SetupDisplayObject(displayObject, x, y, width, height);
}

//----- (th2) -------------------------------------------------------------
void SetupDisplayObject(DisplayObject& displayObject, int xPos, int yPos, int width, int height)
{
	displayObject.Left = xPos;
	displayObject.Top = yPos;
	displayObject.Width = width;
	displayObject.Heigth = height;
	displayObject.Down = displayObject.Top + displayObject.Heigth;
	displayObject.Right = displayObject.Left + displayObject.Width;
}

//----- (th2) -------------------------------------------------------------
bool CursorIntoDisplayObject(const DisplayObject& button)
{
	int cursorX = CursorX;
	int cursorY = CursorY;

	// For items we consider cursor x as item's center
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
		cursorX += (CurCursorWidth / 2);
		cursorY += (CurCursorHeight / 2);
	}

	if( cursorX >= button.Left && cursorX <= button.Right
		&& cursorY >= button.Top && cursorY <= button.Down ){
		return true;
	}else{
		return false;
	}
}
