#include "stdafx.h"

// 0048ACA8
char HelpText[] = 
"$Keyboard Shortcuts:|F1:    Open Help Screen|Esc:   Display Main "\
"Menu|Tab:   Display Auto-map|Space: Hide all info screens|S: Open"\
" Speedbook|B: Open Magicbook|I: Open Inventory screen|C: Open Cha"\
"racter screen|Q: Open Quest log|F: Reduce screen brightness|G: In"\
"crease screen brightness|Z: Zoom Game Screen|+ / -: Zoom Automap|"\
"1 - 8: Use Belt item|F5, F6, F7, F8:     Set hot key for skill or"\
" spell|Shift + Left Click: Attack without moving||$Movement:|If y"\
"ou hold the mouse button down while moving, the character will co"\
"ntinue to move in that direction.||$Combat:|Holding down the shif"\
"t key and then left-clicking allows the character to attack witho"\
"ut moving.||$Auto-map:|To access the auto-map, click the 'MAP'"\
" button on the Information Bar or press 'TAB' on the keyboa"\
"rd. Zooming in and out of the map is done with the + and - keys. "\
"Scrolling the map uses the arrow keys.||$Picking up Objects:|Usea"\
"ble items that are small in size, such as potions or scrolls, are"\
" automatically placed in your 'belt' located at the top of "\
"the Interface bar . When an item is placed in the belt, a small n"\
"umber appears in that box. Items may be used by either pressing t"\
"he corresponding number or right-clicking on the item.||$Gold|You"\
" can select a specific amount of gold to drop by right clicking o"\
"n a pile of gold in your inventory.||$Skills & Spells:|You can ac"\
"cess your list of skills and spells by left-clicking on the 'M"\
"agics' button in the interface bar. Memorized spells and those"\
" available through staffs are listed here. Left-clicking on the s"\
"pell you wish to cast will ready the spell. A readied spell may b"\
"e cast by simply right-clicking in the play area.||$Using the Spe"\
"edbook for Spells|Left-clicking on the 'readied spell' butt"\
"on will open the 'Speedbook' which allows you to select a s"\
"kill or spell for immediate use.  To use a readied skill or spell"\
", simply right-click in the main play area.||$Setting Spell Hotke"\
"ys|You can assign up to four Hot Keys for skills, spells or scrol"\
"ls.  Start by opening the 'speedbook' as described in the s"\
"ection above. Press the F5, F6, F7 or F8 keys after highlighting "\
"the spell you wish to assign.||$Magic Books|Reading more than one"\
" book increases your knowledge of that spell, allowing you to cas"\
"t the spell more effectively.|&";

//----- (0041D0A9) --------------------------------------------------------
void ResetHelp()
{
	IsHELPVisible = false;
	SomeHelpMemory_1 = 0;
	SomeHelpMemory_2 = 0;
}

//----- (0041D0BB) --------------------------------------------------------
void GameHelp()
{
	int v0;   // edi@1
	int v2;   // edx@3
	int v3;   // ecx@3
	char v4;  // al@8
	char* v5; // eax@15
	int v6;   // edx@22
	int v7;   // ecx@22
	char v8;  // al@28
	char v9;  // al@13
	char v11; // al@34
	char v12; // ST10_1@39
	int v13;  // edx@39
	int v14;  // [sp+10h] [bp-4h]@2
	int v15;  // [sp+Ch] [bp-8h]@26
	SetGameHelpBigMenuBox();
	DrawDialogBox( Dialog_591_Width, Dialog_302_Height, TextBoxCel );
	DrawGameDialogTitleText(0, 2, 1, "The Heaven Help", 3, 0);
	DrawDialogLine(5);
	v0 = HelpCurrentPosition;
	uint helpTextIndex = 0;
	if( HelpCurrentPosition > 0 ){
		v14 = HelpCurrentPosition;
		do{
			v2 = 0;
			v3 = 0;
			while( !HelpText[helpTextIndex] ){
				++helpTextIndex;
			}
			if( HelpText[helpTextIndex] == '$' ){
				++helpTextIndex;
			}



			v4 = HelpText[helpTextIndex];
			if( HelpText[helpTextIndex] != '&' ){
				while( v4 != '|' ){
					if( v3 >= 577 ){
						goto LABEL_15;
					}
					if( !v4 ){
						do{
							++helpTextIndex;
						}while( !HelpText[helpTextIndex] );
					}
					v9 = HelpText[helpTextIndex];
					InfoPanelBuffer[v2++] = HelpText[helpTextIndex++];
					v3 += FontWidthSmall[FontIndexSmall[Codepage[v9]]] + 1;
					v4 = HelpText[helpTextIndex];
				}
				if( v3 < 577 )
					goto LABEL_18;
				LABEL_15:
				// Было v2 = &BeforeSomeString2[v2], но v2 согласно алгоритма не может быть тут меньше 1
				v5 = &InfoPanelBuffer[v2 - 1];
				while( *v5 != ' ' ){
					--helpTextIndex;
					--v5;
				}
				LABEL_18:
				if( HelpText[helpTextIndex] == 124 ){
					++helpTextIndex;
				}
			}
		}while( v14-- != 1 );
	}
	v14 = 7;
	do{
		v7 = 0;
		v6 = 0;
		// Мистика. Есть строка HelpText. Есть указатель fn, присваиваем fn адрес HelpText, а fn принимает какое то другое значение. Не адрес HelpText
		// Затем делаю fn индексом HelpText. Читаю 0е значение и опять ошибка при чтении. Причём адрес какой то отличный от адреса HelpText
		// Может всё дело в размере или содержимом HelpText?
		while( !HelpText[helpTextIndex] ){// Break with error - incorrect pointer 0x79654b24
			++helpTextIndex;
		}
		if( HelpText[helpTextIndex] == 36 ){
			++helpTextIndex;
			LOBYTE_IDA(v15) = 2;
		}else{
			LOBYTE_IDA(v15) = 0;
		}
		v8 = HelpText[helpTextIndex];
		if( HelpText[helpTextIndex] == 38 ){
			HelpStringsCount = v0;
		}else{
			while( v8 != 124 ){
				if( v6 >= 577 ){
					goto LABEL_36;
				}
				if( !v8 ){
					do{
						++helpTextIndex;
					}while( !HelpText[helpTextIndex] );
				}
				v11 = HelpText[helpTextIndex];
				InfoPanelBuffer[v7++] = HelpText[helpTextIndex++];
				v6 += FontWidthSmall[FontIndexSmall[Codepage[v11]]] + 1;
				v8 = HelpText[helpTextIndex];
			}
			if( v6 < 577 ){
				goto LABEL_38;
			}
			LABEL_36:
			while( 1 ){
				--v7;
				if( InfoPanelBuffer[v7] == 32 ){
					break;
				}
				--helpTextIndex;
			}
			LABEL_38:
			if( v7 ){
				v12 = v15;
				v13 = v14;
				InfoPanelBuffer[v7] = 0;
				DrawHelpText(0, v13, (int)InfoPanelBuffer, v12);
				v0 = HelpCurrentPosition;
			}
			if( HelpText[helpTextIndex] == 124 ){
				++helpTextIndex;
			}
		}
		++v14;
	}while( v14 < 22 );
	DrawGameDialogTitleText(0, 23, 1, "Press ESC to end or the arrow keys to scroll.", 3, 0);
}

//----- (0041D246) --------------------------------------------------------
char __fastcall DrawHelpText(int a1, int a2, int a3, char aFontSize)
{
	char result; // al@1
	int v5;      // ebx@1
	int v6;      // edi@1
	char v7;     // al@2
	int v8;      // esi@2
	v5 = 0;
	v6 = YOffsetHashTable[StringRowYPosition[a2] + 204] + a1 + 96;
	for( result = *(uchar*)a3; *(uchar*)a3; result = *(uchar*)a3 ){
		++a3;
		v7 = FontIndexSmall[Codepage[result]];
		v8 = (unsigned __int8)v7;
		v5 += FontWidthSmall[v7] + 1;
		if( v7 ){
			if( v5 <= 577 )
				DrawLetter(v6, (unsigned __int8)v7, aFontSize);
		}
		v6 += FontWidthSmall[v8] + 1;
	}
	return result;
}

//----- (0041D2DB) --------------------------------------------------------
void ShiftHelpUp()
{
	if( HelpCurrentPosition > 0 ){// Help button if busy by Fury spell
		--HelpCurrentPosition;
	}
}

//----- (0041D2EB) --------------------------------------------------------
void ShiftHelpDown()
{
	if( HelpCurrentPosition < HelpStringsCount ){// Help button if busy by Fury spell
		++HelpCurrentPosition;
	}
}
