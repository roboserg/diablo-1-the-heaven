#include "stdafx.h"

enum Tooltip_Param
{
	TOOLTIP_TEXT_MAX = 256,

	NO_BORDER = 0,
	BACKGROUND_COLOR = 254,
	BORDER_COLOR = 176,
	LINE_HEIGHT = 22,
	PADDING = 6,
};

struct TooltipLine
{
    char text[TOOLTIP_TEXT_MAX];
    int width;
    int color;
};

vector<TooltipLine> TooltipLines;
int TooltipWidth = 0;
vec2 TooltipHotspot {};
vec2 TooltipOffset {};

vector<TooltipLine> TooltipLines_2;
int TooltipWidth_2 = 0;
vec2 TooltipHotspot_2{};
vec2 TooltipOffset_2{};

vector<TooltipLine> TooltipLines_3;
int TooltipWidth_3 = 0;
vec2 TooltipHotspot_3{};
vec2 TooltipOffset_3{};

constexpr vec2 TooltipHotspotLeft   = {  0.0,   0.5 },
               TooltipOffsetLeft    = { -8  ,   0   },
               TooltipHotspotRight  = {  1.0,   0.5 },
               TooltipOffsetRight   = { 32  ,   0   },
               TooltipHotspotTop    = {  0.5,   0.0 },
               TooltipOffsetTop     = {  0  , -32   },
               TooltipHotspotBottom = {  0.5,   1.0 },
               TooltipOffsetBottom  = {  0  ,  32   };

// ---- th2 -------------------------------------------------------------------------------
int __fastcall GetTextWidthColored( const char* s )
{
	int l = 0;
	while( char c = *s++ ){
	    if( c > 0 ){ // ignore color chars (negative value is color of char)
		    l += FontWidthSmall[ FontIndexSmall[ Codepage[ c ] ] ] + 1;
        }
	}
	return l;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall DrawTextColored( int xPos, int yPos, int xPosEnd, const char* text, int baseFontColor )
{
	int offset = YOffsetHashTable[yPos + Screen_TopBorder] + xPos + Screen_LeftBorder;

	// Center text
	int textLength = GetTextWidthColored( text );
	int maxWidth = xPosEnd - xPos + 1;
	if (textLength < maxWidth){
		offset += (maxWidth - textLength) / 2;
	}
	
	// Draw line
    const char* textPtr = text;
	stack<char> colorStack;
    while( *textPtr ){
        if( *textPtr < 0 ){
            char color = *textPtr;
            if( colorStack.empty() ){
                colorStack.push( color );
            }else{
                if( colorStack.top() != color ){
                    colorStack.push( color );
                }else{
                    colorStack.pop();
                }
            }
        }else{
            char currentLetter = FontIndexSmall[Codepage[*textPtr]];
            if( currentLetter ) {
    
                int color;
                if( !colorStack.empty() ){
                    color = colorStack.top() + 128;
                }else{
                    color = baseFontColor;
                }
                DrawLetter( offset, currentLetter, color );
            }
            offset += FontWidthSmall[currentLetter] + 1;
        }
        ++textPtr;
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Clear()
{
    TooltipLines.clear();
    TooltipWidth = 0;

    TooltipLines_2.clear();
    TooltipWidth_2 = 0;

    TooltipLines_3.clear();
    TooltipWidth_3 = 0;

    ProhibitAltHighlight = false;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine( const char* text, int color )
{
    TooltipLine line{};
    strcpy(line.text, text);
    line.width = GetTextWidthColored( line.text );
    line.color = color;

    TooltipLines.push_back( line );
    TooltipWidth = max( TooltipWidth, line.width );
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine_2(const char* text, int color)
{
    TooltipLine line{};
    strcpy(line.text, text);
    line.width = GetTextWidthColored(line.text);
    line.color = color;

    TooltipLines_2.push_back(line);
    TooltipWidth_2 = max(TooltipWidth_2, line.width);
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine_3(const char* text, int color)
{
    TooltipLine line{};
    strcpy(line.text, text);
    line.width = GetTextWidthColored(line.text);
    line.color = color;

    TooltipLines_3.push_back(line);
    TooltipWidth_3 = max(TooltipWidth_3, line.width);
}


// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Draw()
{
    if( TooltipLines.empty() ){
        return;
    }
    
    int width = TooltipWidth + PADDING*2;
    int height = TooltipLines.size()*LINE_HEIGHT - 11 + PADDING*2;
    int x = CursorX + int(width * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int(height * (TooltipHotspot.y - 1.0f));
    
    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);
    
    LimitToRange(x, 0 + ((TooltipWidth_2 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_2 + (PADDING * 2) + 2) : 0) + ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0), ScreenWidth - width - ((TooltipWidth_2 && (is(TooltipHotspot.x, TooltipHotspotRight.x, TooltipHotspotBottom.x))) ? (TooltipWidth_2 + (PADDING * 2) + 2) : 0) - ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotRight.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0));
    LimitToRange(y, 0, ScreenHeight - height);
    
    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);
    
    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);

    for( int i = 0, ie = TooltipLines.size(); i != ie; ++i ){
        const TooltipLine& line = TooltipLines[i];
        DrawTextColored( tooltipRect.Left + PADDING, tooltipRect.Top + i*LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color );
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Draw_2()
{
    if (TooltipLines_2.empty()) {
        return;
    }

    int width = TooltipWidth_2 + PADDING * 2;
    int height = TooltipLines_2.size() * LINE_HEIGHT - 11 + PADDING * 2;
    int x = CursorX + int((width + (TooltipWidth + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int((TooltipLines.size() * LINE_HEIGHT - 11 + PADDING * 2) * (TooltipHotspot.y - 1.0f));

    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);

    if (TooltipHotspot.x == TooltipHotspotRight.x) {
        x += (TooltipWidth + (PADDING * 2));
        x+=2;
    }
    else if (TooltipHotspot.x == TooltipHotspotBottom.x) {
        x -= int((width + (TooltipWidth + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
        x += (TooltipWidth + (PADDING * 2)) / 2;
        x+=2;
    }
    else {
        x-=2;
    }

    

    LimitToRange(x, 0 + ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2 ) : 0) , ScreenWidth - width - ((TooltipWidth_3 && (is (TooltipHotspot.x, TooltipHotspotRight.x, TooltipHotspotBottom.x))) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0));
    LimitToRange(y, 0, ScreenHeight - height);

    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);

    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);

    for (int i = 0, ie = TooltipLines_2.size(); i != ie; ++i) {
        const TooltipLine& line = TooltipLines_2[i];
        DrawTextColored(tooltipRect.Left + PADDING, tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color);
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Draw_3()
{
    if (TooltipLines_3.empty()) {
        return;
    }

    int width = TooltipWidth_3 + PADDING * 2;
    int height = TooltipLines_3.size() * LINE_HEIGHT - 11 + PADDING * 2;
    int x = CursorX + int((width + (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int((TooltipLines.size() * LINE_HEIGHT - 11 + PADDING * 2) * (TooltipHotspot.y - 1.0f));

    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);

    if (TooltipHotspot.x == TooltipHotspotRight.x) {
        x += (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2));
        x += 4;
    }
    else if (TooltipHotspot.x == TooltipHotspotBottom.x) {
        x -= int((width + (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
        x += ((TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) / 2;
        x += 4;
    }
    else {
        x -= 4;
    }

    LimitToRange(x, 0, ScreenWidth - width);
    LimitToRange(y, 0, ScreenHeight - height);

    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);

    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);

    for (int i = 0, ie = TooltipLines_3.size(); i != ie; ++i) {
        const TooltipLine& line = TooltipLines_3[i];
        DrawTextColored(tooltipRect.Left + PADDING, tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color);
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetLeft()
{
    TooltipHotspot = TooltipHotspotLeft;
    TooltipOffset = TooltipOffsetLeft;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetRight()
{
    TooltipHotspot = TooltipHotspotRight;
    TooltipOffset = TooltipOffsetRight;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetTop()
{
    TooltipHotspot = TooltipHotspotTop;
    TooltipOffset = TooltipOffsetTop;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetBottom()
{
    TooltipHotspot = TooltipHotspotBottom;
    TooltipOffset = TooltipOffsetBottom;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo_2(int slot)
{
    if (slot == -1) {
        return;
    }
    //Tooltip_Clear();
    ProhibitAltHighlight = true;
    Tooltip_AddLine("Selected", C_7_Grey);
    Tooltip_AddLine_2("Equipped", C_7_Grey);

    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
    Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[slot];

    if (item.ItemCode == IC_M1_NONE) return;

    BaseItem& baseItem = BaseItems[item.baseItemIndex];

    int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if (identified) {
        namePtr = item.FullMagicalItemName;
    }
    else {
        namePtr = item.Name;
    }

    if (item.ItemCode == IC_11_GOLD) {
        sprintf(InfoPanelBuffer, "%i gold", item.amount);
        namePtr = InfoPanelBuffer;
    }

    // Skip * in set's name
    if (*namePtr == '*') {
        ++namePtr;
    }

    Tooltip_AddLine_2(namePtr, nameColor);

    if (rare) {
        Tooltip_AddLine_2("Rare Item", nameColor);
    }
    else if (item.MagicLevel == ML_2_UNIQUE) {
        Tooltip_AddLine_2("Unique Item", nameColor);
    }
    else if (item.MagicLevel == ML_3_SET) {
        Tooltip_AddLine_2("Set Item", nameColor);
    }
    else if (IsQuestItem(item.baseItemIndex)) {
        Tooltip_AddLine_2("Quest item", nameColor);
    }

    if (item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
        if (item.BaseDurability) {
            if (item.BaseDurability == ITEM_DUR_INDESTRUBLE) {
                Tooltip_AddLine_2("indestructible", C_3_Gold);
            }
            else {
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;

                if (item.CurDurability <= 1) {
                    curDurColor = C_2_Red;
                }
                else if (item.CurDurability >= 2 && item.CurDurability <= 5) {
                    curDurColor = C_5_Yellow;
                }

                if (item.BaseDurability <= 1) {
                    baseDurColor = C_2_Red;
                }
                else if (item.CurDurability <= 1) {
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;

                sprintf(InfoPanelBuffer, "durability: %c%i%c of %c%i%c", curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }
    }

    if (item.BaseCharges) {
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName(item.SpellIndex), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine_2(InfoPanelBuffer);
    }

    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_1;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_1:
            const char* damageName = "damage";
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_4;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = "slow damage";
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "slashing damage";
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "crushing damage";
            }
        MUMZ_4:
            sprintf(InfoPanelBuffer, "%s: ", damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine_2("(+50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_2("(+15% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_2("(+80% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_2("(+30% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_2("(+40% to beasts, -50% to undead)");
                    }
                    else {
                        Tooltip_AddLine_2("(+50% to beasts, -50% to undead)");
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine_2("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_2("(+15% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_2("(+25% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_2("(+30% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_2("(+40% to undead, -50% to beasts)");
                    }
                    else {
                        Tooltip_AddLine_2("(+50% to undead, -50% to beasts)");
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine_2("(+20% to demons, -20% to beasts)");
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine_2("(+20% to beasts, -20% to undead)");
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                BaseItem& baseItem = BaseItems[item.baseItemIndex];
                TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = "increased";
                }
                else {
                    ac = -ac;
                    modificationSrt = "reduced";
                }
                sprintf(InfoPanelBuffer, "armor class %s by: %i", modificationSrt, ac);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }
    }

    if (item.TypeID == ITEM_2_ARMOR) {
        if (item.damageFromEnemies) {
            sprintf(InfoPanelBuffer, "%+i damage from enemies", item.damageFromEnemies);
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.ArmorClass) {
            sprintf(InfoPanelBuffer, "armor class: %i", item.ArmorClass);
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent) {
            sprintf(InfoPanelBuffer, "resist melee damage: ");
            if (item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent) {
            sprintf(InfoPanelBuffer, "resist arrow damage: ");
            if (item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minThorns || item.maxThorns) {
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if (item.minThorns == item.maxThorns) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.MinDamage || item.MaxDamage) {
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if (minDmg > 0) {
                modificationSrt = "increased";
            }
            else {
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = "reduced";
            }
            if (item.MinDamage == item.MaxDamage) {
                sprintf(InfoPanelBuffer, "damage %s by: %i", modificationSrt, minDmg);
            }
            else {
                sprintf(InfoPanelBuffer, "damage %s by: %i-%i", modificationSrt, minDmg, maxDmg);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
    }

	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, "Quenched: %s", InfoPanelBuffer);
					Tooltip_AddLine_2(temp, C_1_Blue);
				}
			}
		}
	}

    for (int i = 0; i < baseItem.EffectCount; ++i) {
        int effectIndex = 20 + i;
		if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, &item, effectIndex) ){
			Tooltip_AddLine_2(InfoPanelBuffer);
		}
    }

    // Effects
    if (identified) {
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
			}
		}
    }else{
        if (item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
            if (item.MagicLevel) {
                Tooltip_AddLine_2("Not Identified", C_2_Red);
            }
        }
    }

    // Usage effects
    {
        MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;

        switch (magicCode) {
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2("recover 50% of your life");
            }
            else {
                Tooltip_AddLine_2("fully recover life");
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
            Tooltip_AddLine_2("recover partial life");
            break;
        case MC_6_POTION_OF_MANA:
            Tooltip_AddLine_2("recover partial mana");
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2("recover 50% of your mana");
            }
            else {
                Tooltip_AddLine_2("fully recover mana");
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine_2("increases strength");
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine_2("increases magic");
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine_2("increases dexterity");
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine_2("increases vitality");
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine_2("increases all stats");
            break;
        case MC_18_POTION_OF_REJUVENATION:
            Tooltip_AddLine_2("recover life and mana");
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2("recover 50% of life and mana");
            }
            else {
                Tooltip_AddLine_2("fully recover life and mana");
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine_2("Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine_2("Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_2("sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex(item.baseItemIndex);
            if (gemInfoOpt) {
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;

                bool isTwoHanded = [&]() {
                    if (isSocketingPanel) {
                        Item& socketedItem = Craft_GetSocketedItem();
                        if (socketedItem.ItemCode != IC_M1_NONE) {
                            return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                        }
                    }
                    return false;
                }();

                if (isSocketingPanel && isCraftSlot) {
                    Item& socketedItem = Craft_GetSocketedItem();
                    if (socketedItem.ItemCode != IC_M1_NONE) {
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer(socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded);
                        Tooltip_AddLine_2(InfoPanelBuffer);

                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice(gemInfoOpt->grade);
                        sprintf(InfoPanelBuffer, "Extraction price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else {
                    Tooltip_AddLine_2("can be inserted into socketed items");

                    PrintGemEffectToStringBuffer(gemInfoOpt->weaponEffect);
                    sprintf(buffer, "weapons: %s", InfoPanelBuffer);
                    if (gemInfoOpt->weaponEffect.type == GemEffectType::Damage) {
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, "%s (2X FOR 2H WEAPONS)", buffer);
                        }
                    }
                    Tooltip_AddLine_2(buffer);

                    PrintGemEffectToStringBuffer(gemInfoOpt->armorEffect);
                    if (gemInfoOpt->type == GEM::Chrysolite) {
                        sprintf(buffer, "body armors: %s", InfoPanelBuffer);
                    }
                    else {
                        sprintf(buffer, "armors: %s", InfoPanelBuffer);
                    }
                    Tooltip_AddLine_2(buffer);

                    if (isSocketingPanel && !isCraftSlot) {
                        if (Craft_Socketing_IsSocketingItemReady()) {
                            if (Craft_Socketing_IsGemSuitableForCurrentItem(*gemInfoOpt)) {
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice(gemInfoOpt->grade);
                                sprintf(InfoPanelBuffer, "Insertion price: %i", price);
                                Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }
                            else {
                                Tooltip_AddLine_2("Not suitable for current item", C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    Tooltip_AddLine_2("Shift click to move to crafting slot");
                }
            }
            break;
        }
        }

        if (restoreDurMin || restoreDurMax) {
            if (restoreDurMin == restoreDurMax) {
                sprintf(InfoPanelBuffer, "restores %i points", restoreDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, "restores %i-%i points", restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            Tooltip_AddLine_2("of current durability");
        }

        if (addDurMin || addDurMax) {
            if (addDurMin == addDurMax) {
                sprintf(InfoPanelBuffer, "+%i to maxdurability", addDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, "+[%i-%i] to max durability", addDurMin, addDurMax);
            }
            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "up to %i points", addDurLimit);
                Tooltip_AddLine_2(InfoPanelBuffer);
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, "(%i for body armor)", elTorso);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }

        if (IsValueStoneItem(item.baseItemIndex)) {
            Tooltip_AddLine_2("Sell in town for gold");
        }

    }

    if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
        sprintf(InfoPanelBuffer, "Socketed [%i]", item.socketsAmount);
        Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
		}
	}

    // Usage hint
    if (!IsVisualTradePanelVisible && !IsStashPanelVisible)
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_2("Right click to use");
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
            Tooltip_AddLine_2("Right click to read");
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine_2("Right-click to read, then");
            Tooltip_AddLine_2("left-click to target");
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine_2("Right click to view");
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, "Level : %i", item.quality);
            Tooltip_AddLine_2(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine_2("Doubles gold capacity");
            break;
        }

        if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
            Tooltip_AddLine_2("Right click to use");
        }
        else if (isTownPortalForbidden()) {
            switch (item.baseItemIndex) {
            case BI_19_FUNGAL_TOME:		 Tooltip_AddLine_2("Right click to transmute into spectral elixir"); break;
            case BI_2512_FULL_CONTAINERS:Tooltip_AddLine_2("Right click to transmute into random elixir"); break;
            case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine_2("Right click to use"); break;
            case BI_16_ANVIL_OF_FURY:
            case BI_9_MAGIC_ROCK:
            case BI_327_TAVERN_SIGN:
            case BI_2457_IRON_FIGURINE:
            case BI_2459_REAGENTS:
            case BI_2460_THE_RIGHT_GOODS: 
            case BI_2510_DUSTY_CRATE:
            case BI_2461_MUNDANE_INVENTORY:
            case BI_334_THEODORE:		 Tooltip_AddLine_2("Right click to transmute into quest reward"); break;
            }
        }
        if (isTownPortalForbidden()) {
            if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
                Tooltip_AddLine_2("Right click to transmute into gold");
            }
        }
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) {
            Tooltip_AddLine_2("Right click to open (requires key)"); 
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine_2("Right click to read (requires crystal eyepiece)");
        }

        if (IsItemAllowedForBelt(item)) {
            if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                Tooltip_AddLine_2("Shift Click to move left");
			}else if( ! IsTransmutePanelVisible ){
                Tooltip_AddLine_2("Shift Click to move to belt");
            }
        }
    }

    // Requirements
    if (item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u) {
        Tooltip_AddLine_2("Spell is maxed", C_3_Gold);
    }
    else if (item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel) {
        strcpy(InfoPanelBuffer, "required:");

        auto appendReqInfo = [&player](const char* name, int req, int current, int magicCode)
        {
            if (req) {
                uchar color;
                if (current >= req) {
                    color = C_0_White + 128;
                }
                else if (name != "lvl" && req <= current + (is(magicCode,
                    MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET,
                    MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                    MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser)) {
                    color = C_1_Blue + 128;
                }
                else {
                    color = C_2_Red + 128;
                }
                sprintf(InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color);
            }
        };

        appendReqInfo("lvl", item.RequiredLevel, player.CharLevel, item.MagicCode);
        appendReqInfo("str", item.RequiredStrength, player.CurStrength, item.MagicCode);
        appendReqInfo("mag", item.RequiredMagic, player.CurMagic, item.MagicCode);
        appendReqInfo("dex", item.RequiredDexterity, player.CurDexterity, item.MagicCode);
        appendReqInfo("vit", item.RequiredVitality, player.CurVitality, item.MagicCode);
        Tooltip_AddLine_2(InfoPanelBuffer);
    }

    uint playerFullClassMask = (1u << static_cast<uint>(player.fullClassId));
    uint requiredClassMask = GetRequiredClassMask(&item);
    if (requiredClassMask != IPCM_ALL_CLASSES) {
        printItemClassReqInfoInBuffer(&item);
        Tooltip_AddLine_2(InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red);
    }

    if ((playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles) {
        Tooltip_AddLine_2("[Can't wear item]", C_2_Red);
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
        if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
            Tooltip_AddLine_2("[Can't wear body armor]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
        if (is(item.ItemCode, IC_5_SHIELD)) {
            Tooltip_AddLine_2("[Can't use shields]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
        if (is(item.ItemCode, IC_21_KNIFE)) {
            Tooltip_AddLine_2("[Can't use throwing knives]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine_2("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine_2("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_2("[Can't use melee weapons]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_2("[Can't use item]", C_2_Red);
        }
    }
    if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
        if (is(item.MagicCode,
            MC_3_POTION_OF_HEALING,
            MC_6_POTION_OF_MANA,
            MC_2_POTION_OF_FULL_HEALING,
            MC_7_POTION_OF_FULL_MANA,
            MC_18_POTION_OF_REJUVENATION,
            MC_19_POTION_OF_FULL_REJUVENATION)) {
            Tooltip_AddLine_2("[Can't use potions]", C_2_Red);
        }
    }
    //----------------------
    if (IsVisualTradePanelVisible) {

        if (isVisualTradeSlot) {
            int price = VisualTrade_GetBuyPrice(&item);
            sprintf(InfoPanelBuffer, "Buy price: %i", price);
            Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
            Tooltip_AddLine_2("Shift click to buy fast");
        }
        else {
            if (Cur.GraphicsID == CM_1_NORMAL_HAND) {
                if (CurrentVisualTradeMode != VTM_Elder) {
                    int price = VisualTrade_GetSellPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Sell price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer);
                        Tooltip_AddLine_2("Shift click to sell fast");
                    }
                    else {
                        Tooltip_AddLine_2("Item cannot be sold", C_2_Red);
                    }
                }
            }
            else if (Cur.IsTradeService) {
                if (Cur.GraphicsID == CM_2_IDENTIFY) {
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_3_REPAIR) {
                    int price = VisualTrade_GetRepairPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_4_STAFF_RECHARGE) {
                    int price = VisualTrade_GetRechargePrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }

    if (IsStashPanelVisible) {
        if (slot >= IS_Inventory) {
            if (!IsItemAllowedForStash(&item)) {
                Tooltip_AddLine_2("Cannot be stashed", C_2_Red);
            }
            else {
                if (isVisualStashSlot) {
                    Tooltip_AddLine_2("Shift Click to move to inventory");
                }
                else {
                    Tooltip_AddLine_2("Shift Click to move to stash");
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ) {
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine_2("Shift Click to move to inventory", C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine_2("Shift Click to transmute panel", C_7_Grey);
		}
	}

    if (identified && item.MagicLevel == ML_3_SET) {
        UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
        UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

        Tooltip_AddLine_2(""); // blank line
        bool hasActiveEffects = false;
        int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
        for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
            auto& e = set.Effects[i];
            if (countOfEquippedItemsFromSet >= e.Threshold) {
				if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine_2(InfoPanelBuffer, C_4_Orange);
				}
				hasActiveEffects = true;
			}
        }
        if (hasActiveEffects) Tooltip_AddLine_2(""); // blank line
        Tooltip_AddLine_2(set.Name, C_3_Gold);
        for (int i = 0; i < set.UniquesCount; ++i)
        {
            int uniqueItemIndex = set.UniquesList[i];
            UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
            Tooltip_AddLine_2(uniq2.Name, SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
        }
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo_3(int slot)
{
    if (slot == -1) {
        return;
    }

    //Tooltip_Clear();

    Tooltip_AddLine_3("Equipped", C_7_Grey);

    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[slot];

	if (item.ItemCode == IC_M1_NONE) return;

    BaseItem& baseItem = BaseItems[item.baseItemIndex];

    int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if (identified) {
        namePtr = item.FullMagicalItemName;
    }
    else {
        namePtr = item.Name;
    }

    if (item.ItemCode == IC_11_GOLD) {
        sprintf(InfoPanelBuffer, "%i gold", item.amount);
        namePtr = InfoPanelBuffer;
    }

    // Skip * in set's name
    if (*namePtr == '*') {
        ++namePtr;
    }

    Tooltip_AddLine_3(namePtr, nameColor);

    if (rare) {
        Tooltip_AddLine_3("Rare Item", nameColor);
    }
    else if (item.MagicLevel == ML_2_UNIQUE) {
        Tooltip_AddLine_3("Unique Item", nameColor);
    }
    else if (item.MagicLevel == ML_3_SET) {
        Tooltip_AddLine_3("Set Item", nameColor);
    }
    else if (IsQuestItem(item.baseItemIndex)) {
        Tooltip_AddLine_3("Quest item", nameColor);
    }

    if (item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
        if (item.BaseDurability) {
            if (item.BaseDurability == ITEM_DUR_INDESTRUBLE) {
                Tooltip_AddLine_3("indestructible", C_3_Gold);
            }
            else {
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;

                if (item.CurDurability <= 1) {
                    curDurColor = C_2_Red;
                }
                else if (item.CurDurability >= 2 && item.CurDurability <= 5) {
                    curDurColor = C_5_Yellow;
                }

                if (item.BaseDurability <= 1) {
                    baseDurColor = C_2_Red;
                }
                else if (item.CurDurability <= 1) {
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;

                sprintf(InfoPanelBuffer, "durability: %c%i%c of %c%i%c", curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }
    }

    if (item.BaseCharges) {
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName(item.SpellIndex), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine_3(InfoPanelBuffer);
    }

    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_2;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_2:
            const char* damageName = "damage";
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_5;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = "slow damage";
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "slashing damage";
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "crushing damage";
            }
            MUMZ_5:
            sprintf(InfoPanelBuffer, "%s: ", damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine_3("(+50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_3("(+15% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_3("(+80% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_3("(+30% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_3("(+40% to beasts, -50% to undead)");
                    }
                    else {
                        Tooltip_AddLine_3("(+50% to beasts, -50% to undead)");
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine_3("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_3("(+15% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_3("(+25% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_3("(+30% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_3("(+40% to undead, -50% to beasts)");
                    }
                    else {
                        Tooltip_AddLine_3("(+50% to undead, -50% to beasts)");
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine_3("(+20% to demons, -20% to beasts)");
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine_3("(+20% to beasts, -20% to undead)");
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                BaseItem& baseItem = BaseItems[item.baseItemIndex];
                TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = "increased";
                }
                else {
                    ac = -ac;
                    modificationSrt = "reduced";
                }
                sprintf(InfoPanelBuffer, "armor class %s by: %i", modificationSrt, ac);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }
    }

    if (item.TypeID == ITEM_2_ARMOR) {
        if (item.damageFromEnemies) {
            sprintf(InfoPanelBuffer, "%+i damage from enemies", item.damageFromEnemies);
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.ArmorClass) {
            sprintf(InfoPanelBuffer, "armor class: %i", item.ArmorClass);
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent) {
            sprintf(InfoPanelBuffer, "resist melee damage: ");
            if (item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent) {
            sprintf(InfoPanelBuffer, "resist arrow damage: ");
            if (item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minThorns || item.maxThorns) {
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if (item.minThorns == item.maxThorns) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.MinDamage || item.MaxDamage) {
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if (minDmg > 0) {
                modificationSrt = "increased";
            }
            else {
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = "reduced";
            }
            if (item.MinDamage == item.MaxDamage) {
                sprintf(InfoPanelBuffer, "damage %s by: %i", modificationSrt, minDmg);
            }
            else {
                sprintf(InfoPanelBuffer, "damage %s by: %i-%i", modificationSrt, minDmg, maxDmg);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
    }

	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, "Quenched: %s", InfoPanelBuffer);
					Tooltip_AddLine_3(temp, C_1_Blue);
				}
			}
		}
	}

    for (int i = 0; i < baseItem.EffectCount; ++i) {
        const int effectIndex = 20 + i;
        if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, const_cast<Item*>(&item), effectIndex) ){
			Tooltip_AddLine_3(InfoPanelBuffer);
		}
    }

    // Effects
    if (identified) {
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
			}
		}
    }else{
        if (item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
            if (item.MagicLevel) {
                Tooltip_AddLine_3("Not Identified", C_2_Red);
            }
        }
    }

    // Usage effects
    {
        MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;

        switch (magicCode) {
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3("recover 50% of life");
            }
            else {
                Tooltip_AddLine_3("fully recover life");
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
            Tooltip_AddLine_3("recover partial life");
            break;
        case MC_6_POTION_OF_MANA:
            Tooltip_AddLine_3("recover partial mana");
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3("recover 50% of mana");
            }
            else {
                Tooltip_AddLine_3("fully recover mana");
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine_3("increases strength");
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine_3("increases magic");
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine_3("increases dexterity");
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine_3("increases vitality");
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine_3("increases all stats");
            break;
        case MC_18_POTION_OF_REJUVENATION:
            Tooltip_AddLine_3("recover life and mana");
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3("recover 50% of life and mana");
            }
            else {
                Tooltip_AddLine_3("fully recover life and mana");
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine_3("Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine_3("Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_3("sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex(item.baseItemIndex);
            if (gemInfoOpt) {
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;

                bool isTwoHanded = [&]() {
                    if (isSocketingPanel) {
                        Item& socketedItem = Craft_GetSocketedItem();
                        if (socketedItem.ItemCode != IC_M1_NONE) {
                            return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                        }
                    }
                    return false;
                }();

                if (isSocketingPanel && isCraftSlot) {
                    Item& socketedItem = Craft_GetSocketedItem();
                    if (socketedItem.ItemCode != IC_M1_NONE) {
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer(socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded);
                        Tooltip_AddLine_3(InfoPanelBuffer);

                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice(gemInfoOpt->grade);
                        sprintf(InfoPanelBuffer, "Extraction price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else {
                    Tooltip_AddLine_3("can be inserted into socketed items");

                    PrintGemEffectToStringBuffer(gemInfoOpt->weaponEffect);
                    sprintf(buffer, "weapons: %s", InfoPanelBuffer);
                    if (gemInfoOpt->weaponEffect.type == GemEffectType::Damage) {
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, "%s (2X FOR 2H WEAPONS)", buffer);
                        }
                    }
                    Tooltip_AddLine_3(buffer);

                    PrintGemEffectToStringBuffer(gemInfoOpt->armorEffect);
                    if (gemInfoOpt->type == GEM::Chrysolite) {
                        sprintf(buffer, "body armors: %s", InfoPanelBuffer);
                    }
                    else {
                        sprintf(buffer, "armors: %s", InfoPanelBuffer);
                    }
                    Tooltip_AddLine_3(buffer);

                    if (isSocketingPanel && !isCraftSlot) {
                        if (Craft_Socketing_IsSocketingItemReady()) {
                            if (Craft_Socketing_IsGemSuitableForCurrentItem(*gemInfoOpt)) {
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice(gemInfoOpt->grade);
                                sprintf(InfoPanelBuffer, "Insertion price: %i", price);
                                Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }
                            else {
                                Tooltip_AddLine_3("Not suitable for current item", C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    Tooltip_AddLine_3("Shift click to move to crafting slot");
                }
            }
            break;
        }
        }

        if (restoreDurMin || restoreDurMax) {
            if (restoreDurMin == restoreDurMax) {
                sprintf(InfoPanelBuffer, "restores %i points", restoreDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, "restores %i-%i points", restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            Tooltip_AddLine_3("of current durability");
        }

        if (addDurMin || addDurMax) {
            if (addDurMin == addDurMax) {
                sprintf(InfoPanelBuffer, "+%i to max durability", addDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, "+[%i-%i] to max durability", addDurMin, addDurMax);
            }
            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "up to %i points", addDurLimit);
                Tooltip_AddLine_3(InfoPanelBuffer);
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, "(%i for body armor)", elTorso);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }

        if (IsValueStoneItem(item.baseItemIndex)) {
            Tooltip_AddLine_3("Sell in town for gold");
        }

    }

	if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
		sprintf(InfoPanelBuffer, "Socketed [%i]", item.socketsAmount);
        Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
		}
	}

    // Usage hint
    if (!IsVisualTradePanelVisible && !IsStashPanelVisible)
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_3("Right click to use");
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
            Tooltip_AddLine_3("Right click to read");
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine_3("Right-click to read, then");
            Tooltip_AddLine_3("left-click to target");
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine_3("Right click to view");
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, "Level : %i", item.quality);
            Tooltip_AddLine_3(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine_3("Doubles gold capacity");
            break;
        }

        if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
            Tooltip_AddLine_3("Right click to use");
        }
        else if (isTownPortalForbidden()) {
            switch (item.baseItemIndex) {
            case BI_19_FUNGAL_TOME:		 Tooltip_AddLine_3("Right click to transmute into spectral elixir"); break;
            case BI_2512_FULL_CONTAINERS:Tooltip_AddLine_3("Right click to transmute into random elixir"); break;
            case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine_3("Right click to use"); break;
            case BI_16_ANVIL_OF_FURY:
            case BI_9_MAGIC_ROCK:
            case BI_327_TAVERN_SIGN:
            case BI_2457_IRON_FIGURINE:
            case BI_2459_REAGENTS:
            case BI_2460_THE_RIGHT_GOODS:
            case BI_2510_DUSTY_CRATE:
            case BI_2461_MUNDANE_INVENTORY:
            case BI_334_THEODORE:		 Tooltip_AddLine_3("Right click to transmute into quest reward"); break;
            }
        }
        if (isTownPortalForbidden()) {
            if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
                Tooltip_AddLine_3("Right click to transmute into gold");
            }
        }
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) { 
            Tooltip_AddLine_3("Right click to open (requires key)");
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine_3("Right click to read (requires crystal eyepiece)");
        }

        if (IsItemAllowedForBelt(item)) {
            if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                Tooltip_AddLine_3("Shift Click to move left");
            }else if( ! IsTransmutePanelVisible ){
                Tooltip_AddLine_3("Shift Click to move to belt");
            }
        }
    }

    // Requirements
    if (item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u) {
        Tooltip_AddLine_3("Spell is maxed", C_3_Gold);
    }
    else if (item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel) {
        strcpy(InfoPanelBuffer, "required:");

        auto appendReqInfo = [&player](const char* name, int req, int current, int magicCode)
        {
            if (req) {
                uchar color;
                if (current >= req) {
                    color = C_0_White + 128;
                }
                else if (name != "lvl" && req <= current + (is(magicCode,
                    MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET,
                    MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                    MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser)) {
                    color = C_1_Blue + 128;
                }
                else {
                    color = C_2_Red + 128;
                }
                sprintf(InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color);
            }
        };

        appendReqInfo("lvl", item.RequiredLevel, player.CharLevel, item.MagicCode);
        appendReqInfo("str", item.RequiredStrength, player.CurStrength, item.MagicCode);
        appendReqInfo("mag", item.RequiredMagic, player.CurMagic, item.MagicCode);
        appendReqInfo("dex", item.RequiredDexterity, player.CurDexterity, item.MagicCode);
        appendReqInfo("vit", item.RequiredVitality, player.CurVitality, item.MagicCode);
        Tooltip_AddLine_3(InfoPanelBuffer);
    }

    uint playerFullClassMask = (1u << static_cast<uint>(player.fullClassId));
    uint requiredClassMask = GetRequiredClassMask(&item);
    if (requiredClassMask != IPCM_ALL_CLASSES) {
        printItemClassReqInfoInBuffer(&item);
        Tooltip_AddLine_3(InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red);
    }

    if ((playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles) {
        Tooltip_AddLine_3("[Can't wear item]", C_2_Red);
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
        if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
            Tooltip_AddLine_3("[Can't wear body armor]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
        if (is(item.ItemCode, IC_5_SHIELD)) {
            Tooltip_AddLine_3("[Can't use shields]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
        if (is(item.ItemCode, IC_21_KNIFE)) {
            Tooltip_AddLine_3("[Can't use throwing knives]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine_3("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine_3("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_3("[Can't use melee weapons]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_3("[Can't use item]", C_2_Red);
        }
    }
    if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
        if (is(item.MagicCode,
            MC_3_POTION_OF_HEALING,
            MC_6_POTION_OF_MANA,
            MC_2_POTION_OF_FULL_HEALING,
            MC_7_POTION_OF_FULL_MANA,
            MC_18_POTION_OF_REJUVENATION,
            MC_19_POTION_OF_FULL_REJUVENATION)) {
            Tooltip_AddLine_3("[Can't use potions]", C_2_Red);
        }
    }
    //----------------------
    if (IsVisualTradePanelVisible) {

        if (isVisualTradeSlot) {
            int price = VisualTrade_GetBuyPrice(&item);
            sprintf(InfoPanelBuffer, "Buy price: %i", price);
            Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
            Tooltip_AddLine_3("Shift click to buy fast");
        }
        else {
            if (Cur.GraphicsID == CM_1_NORMAL_HAND) {
                if (CurrentVisualTradeMode != VTM_Elder) {
                    int price = VisualTrade_GetSellPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Sell price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer);
                        Tooltip_AddLine_3("Shift click to sell fast");
                    }
                    else {
                        Tooltip_AddLine_3("Item cannot be sold", C_2_Red);
                    }
                }
            }
            else if (Cur.IsTradeService) {
                if (Cur.GraphicsID == CM_2_IDENTIFY) {
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_3_REPAIR) {
                    int price = VisualTrade_GetRepairPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_4_STAFF_RECHARGE) {
                    int price = VisualTrade_GetRechargePrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }

    if (IsStashPanelVisible) {
        if (slot >= IS_Inventory) {
            if (!IsItemAllowedForStash(&item)) {
                Tooltip_AddLine_3("Cannot be stashed", C_2_Red);
            }
            else {
                if (isVisualStashSlot) {
                    Tooltip_AddLine_3("Shift Click to move to inventory");
                }
                else {
                    Tooltip_AddLine_3("Shift Click to move to stash");
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ){
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine_3("Shift Click to move to inventory", C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine_3("Shift Click to transmute panel", C_7_Grey);
		}
	}

    if (identified && item.MagicLevel == ML_3_SET) {
        UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
        UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

        Tooltip_AddLine_3(""); // blank line
        bool hasActiveEffects = false;
        int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
        for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
            auto& e = set.Effects[i];
            if (countOfEquippedItemsFromSet >= e.Threshold) {
                if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine_3(InfoPanelBuffer, C_4_Orange);
				}
                hasActiveEffects = true;
            }
        }
        if (hasActiveEffects) Tooltip_AddLine_3(""); // blank line
        Tooltip_AddLine_3(set.Name, C_3_Gold);
        for (int i = 0; i < set.UniquesCount; ++i)
        {
            int uniqueItemIndex = set.UniquesList[i];
            UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
            Tooltip_AddLine_3(uniq2.Name, SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
        }
    }
}

bool ProhibitAltHighlight = false;

// ---- th2 -------------------------------------------------------------------------------
void __fastcall ShowSecondaryTooltip(Item& item)
{
    if( ! SecondaryTooltip || ! IsAltPressed ){
        if (IsController()) {
            if (curXboxController.GetState().Gamepad.bLeftTrigger < 100) {
                return;
            }
        }
        else {
            return;
        }
    }
    Player& player = Players[CurrentPlayerIndex];

    switch (item.ItemCode) {
    case IC_1_SWORD:
    case IC_2_AXE:
    case IC_4_MACE:
    case IC_18_FLASK:
    case IC_20_CLAW:  
    case IC_10_STAFF:
    case IC_19_TRAP:
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP) && is(player.OnBodySlots[IS_RightHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
            Tooltip_ShowItemInfo_3(IS_RightHand);
        } 
        else if (is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_3_BOW: 
    case IC_21_KNIFE:
    case IC_22_MALLET:
    case IC_23_PISTOL:
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_5_SHIELD: 
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode) && is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
            Tooltip_ShowItemInfo_3(IS_RightHand);
        }
        else if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_6_LIGHT_ARMOR:
    case IC_8_MID_ARMOR:
    case IC_9_HEAVY_ARMOR: if (!is(player.OnBodySlots[IS_Torso].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Torso); break;
    case IC_7_HELM: if (!is(player.OnBodySlots[IS_Head].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Head); break;
    case IC_12_RING: 
        if (!is(player.OnBodySlots[IS_LeftRing].ItemCode, IC_M1_NONE) && !is(player.OnBodySlots[IS_RightRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_LeftRing);
            Tooltip_ShowItemInfo_3(IS_RightRing);
        }
        else if (!is(player.OnBodySlots[IS_LeftRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_LeftRing);
        }
        else if (!is(player.OnBodySlots[IS_RightRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_RightRing);
        }
        break;
    case IC_13_AMULET: if (!is(player.OnBodySlots[IS_Neck].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Neck); break;
    case IC_15_GLOVE: if (!is(player.OnBodySlots[IS_Hands].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Hands); break;
    case IC_16_BOOTS: if (!is(player.OnBodySlots[IS_Feet].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Feet); break;
    case IC_17_BELT: if (!is(player.OnBodySlots[IS_Waist].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Waist); break;
    default: return;
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo( int slot )
{
    if( slot == -1 ){
        return;
    }
    XinputAutoTargettingOffTimer = 0;
    Tooltip_Clear();
 
    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
    Player& player = Players[CurrentPlayerIndex];
    Item* itemPtr = nullptr;
	if( slot < IS_50_47_Belt_Start ){ // TODO: change to unified func
	    itemPtr = &player.OnBodySlots[ slot ]; // onBody slots and inventory
	    Tooltip_SetOffsetLeft();
        if (slot > IS_Feet) {
            ShowSecondaryTooltip(*itemPtr);      
        }

    }else if( slot < IS_50_47_Belt_End ){
        itemPtr = &player.BeltInventory[ slot - IS_50_47_Belt_Start ];
        Tooltip_SetOffsetTop();
    }else if( StashPanel_IsStashSlot( slot ) ){
        itemPtr = StashPanel_GetItemBySlotId( slot );
        Tooltip_SetOffsetRight();
        ShowSecondaryTooltip(*itemPtr);
        isVisualStashSlot = true;
    }else if( VisualTrade_IsVisualTradeSlot( slot ) ){
        itemPtr = VisualTrade_GetItemBySlotId( slot );
        Tooltip_SetOffsetRight();
        ShowSecondaryTooltip(*itemPtr);
        isVisualTradeSlot = true;
    }else if( Craft_IsCraftSlot( slot ) ){
        itemPtr = Craft_GetItemBySlotId( slot );
        Tooltip_SetOffsetBottom();
        ShowSecondaryTooltip(*itemPtr);
        isCraftSlot = true;
    }else if (IsEnchantPanelVisible && slot == 101) {
        itemPtr = &EnchantItem;
        Tooltip_SetOffsetBottom();
        ShowSecondaryTooltip(*itemPtr);
	}else if (IsQuenchPanelVisible && slot == 102) {
		itemPtr = &QuenchItem;
		Tooltip_SetOffsetBottom();
		ShowSecondaryTooltip(*itemPtr);
	}else if (IsTransmutePanelVisible) {
		itemPtr = TransmuteSlots_GetItemBySlotId( slot );
		Tooltip_SetOffsetBottom();
		ShowSecondaryTooltip(*itemPtr);
	}else{
        return;
    }
    
    Item& item = *itemPtr;
    BaseItem& baseItem = BaseItems[item.baseItemIndex];
    
	int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if( identified ){
        namePtr = item.FullMagicalItemName;
    }else{
        namePtr = item.Name;
    }
    
    if( item.ItemCode == IC_11_GOLD ){
		sprintf(InfoPanelBuffer, "%i gold", item.amount);
		namePtr = InfoPanelBuffer;
	}
    
    // Skip * in set's name
    if( *namePtr == '*' ){
        ++namePtr;
    }

    Tooltip_AddLine(namePtr, nameColor);
    
    if( rare ){
        Tooltip_AddLine("Rare Item", nameColor);
    }else if( item.MagicLevel == ML_2_UNIQUE ){
        Tooltip_AddLine("Unique Item", nameColor);
    }else if( item.MagicLevel == ML_3_SET ){
        Tooltip_AddLine("Set Item", nameColor);
    }else if( IsQuestItem(item.baseItemIndex) ){
        Tooltip_AddLine("Quest item", nameColor);
    }

    if( item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR ){
        if( item.BaseDurability ){
            if( item.BaseDurability == ITEM_DUR_INDESTRUBLE ){
                Tooltip_AddLine("indestructible", C_3_Gold);
            }else{
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;
                
                if( item.CurDurability <= 1 ){
                    curDurColor = C_2_Red;
                }else if( item.CurDurability >= 2 && item.CurDurability <= 5 ){
                    curDurColor = C_5_Yellow;
                }
                
                if( item.BaseDurability <= 1 ){
                    baseDurColor = C_2_Red;
                }else if( item.CurDurability <= 1 ){
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;
                
                sprintf(InfoPanelBuffer, "durability: %c%i%c of %c%i%c", curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
    }
    
    if( item.BaseCharges ){
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName( item.SpellIndex ), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine(InfoPanelBuffer);
    }
    
    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_3;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_3:
            const char* damageName = "damage";
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_6;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = "slow damage";
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "slashing damage";
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = "crushing damage";
            }
            MUMZ_6:
            sprintf(InfoPanelBuffer, "%s: ", damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine("(+50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine("(+15% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine("(+80% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine("(+30% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine("(+40% to beasts, -50% to undead)");
                    }
                    else {
                        Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine("(+15% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine("(+25% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine("(+30% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine("(+40% to undead, -50% to beasts)");
                    }
                    else {
                        Tooltip_AddLine("(+50% to undead, -50% to beasts)");
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine("(+20% to demons, -20% to beasts)");
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine("(+20% to beasts, -20% to undead)");
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                const BaseItem& baseItem = BaseItems[item.baseItemIndex];
                const TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = "increased";
                }
                else {
                    ac = -ac;
                    modificationSrt = "reduced";
                }
                sprintf(InfoPanelBuffer, "armor class %s by: %i", modificationSrt, ac);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
    }
    
    if( item.TypeID == ITEM_2_ARMOR ){
        if( item.damageFromEnemies ){
            sprintf(InfoPanelBuffer, "%+i damage from enemies", item.damageFromEnemies);
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.ArmorClass ){
            sprintf(InfoPanelBuffer, "armor class: %i", item.ArmorClass);
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent ){
            sprintf(InfoPanelBuffer, "resist melee damage: ");
            if( item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }else{
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent ){
            sprintf(InfoPanelBuffer, "resist arrow damage: ");
            if( item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }else{
                sprintf(InfoPanelBuffer, "%s%i to %i", InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minThorns || item.maxThorns ){
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if( item.minThorns == item.maxThorns ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }else{
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.MinDamage || item.MaxDamage ){
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if( minDmg > 0 ){
                modificationSrt = "increased";
            }else{
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = "reduced";
            }
			if( item.MinDamage == item.MaxDamage ){
				sprintf(InfoPanelBuffer, "damage %s by: %i", modificationSrt, minDmg);
			}else{
				sprintf(InfoPanelBuffer, "damage %s by: %i-%i", modificationSrt, minDmg, maxDmg);
			}
            Tooltip_AddLine(InfoPanelBuffer);
        }
    }
 
	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, "Quenched: %s", InfoPanelBuffer);
					Tooltip_AddLine(temp, C_1_Blue);
				}
			}
		}
	}

    for( int i = 0; i < baseItem.EffectCount; ++i ){
        const int effectIndex = 20 + i;
		if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, const_cast<Item*>(&item), effectIndex) ){
			Tooltip_AddLine(InfoPanelBuffer);
		}
	}
    
    // Effects
	if( identified ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine(InfoPanelBuffer, C_1_Blue);
			}
		}
	}else{
		if( item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR ){
            if( item.MagicLevel ){
                Tooltip_AddLine("Not Identified", C_2_Red);
            }
        }
	}
 
    // Usage effects
    {
        const MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;
        
        switch( magicCode ){
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine("recover 50% of life");
            }
            else {
                Tooltip_AddLine("fully recover life");
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
            Tooltip_AddLine( "recover partial life");
            break;
        case MC_6_POTION_OF_MANA:
            Tooltip_AddLine( "recover partial mana");
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine("recover 50% of mana");
            }
            else {
                Tooltip_AddLine("fully recover mana");
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine( "increases strength");
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine( "increases magic");
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine( "increases dexterity");
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine( "increases vitality");
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine( "increases all stats");
            break;
        case MC_18_POTION_OF_REJUVENATION:
            Tooltip_AddLine( "recover life and mana");
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine("recover 50% of life and mana");
            }
            else {
                Tooltip_AddLine("fully recover life and mana");
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine( "Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine( "Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine( "sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( item.baseItemIndex );
            if( gemInfoOpt ){
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;
                
                bool isTwoHanded = [&](){
                        if( isSocketingPanel ){
                            Item& socketedItem = Craft_GetSocketedItem();
                            if( socketedItem.ItemCode != IC_M1_NONE ){
                                return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                            }
                        }
                        return false;
                    }();
                
                if( isSocketingPanel && isCraftSlot ){
                    Item& socketedItem = Craft_GetSocketedItem();
                    if( socketedItem.ItemCode != IC_M1_NONE ){
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer( socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded );
                        Tooltip_AddLine( InfoPanelBuffer );
                        
                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice( gemInfoOpt->grade );
                        sprintf(InfoPanelBuffer, "Extraction price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else{
                    Tooltip_AddLine( "can be inserted into socketed items");
                    
                    PrintGemEffectToStringBuffer( gemInfoOpt->weaponEffect );
                    sprintf(buffer, "weapons: %s", InfoPanelBuffer);
                    if( gemInfoOpt->weaponEffect.type == GemEffectType::Damage ){
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, "%s (2X FOR 2H WEAPONS)", buffer);
                        }
                    }
                    Tooltip_AddLine( buffer );
                    
                    PrintGemEffectToStringBuffer( gemInfoOpt->armorEffect );
                    if( gemInfoOpt->type == GEM::Chrysolite ){
                        sprintf(buffer, "body armors: %s", InfoPanelBuffer);
                    }else{
                        sprintf(buffer, "armors: %s", InfoPanelBuffer);
                    }
                    Tooltip_AddLine( buffer );
                    
                    if( isSocketingPanel && !isCraftSlot ){
                        if( Craft_Socketing_IsSocketingItemReady() ){
                            if( Craft_Socketing_IsGemSuitableForCurrentItem( *gemInfoOpt ) ){
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice( gemInfoOpt->grade );
                                sprintf(InfoPanelBuffer, "Insertion price: %i", price);
                                Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }else{
                                Tooltip_AddLine("Not suitable for current item", C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    if (IsController() && !XinputNotInUse(true)) {
                        Tooltip_AddLine("Right Trigger + A to move to crafting slot");
                    }
                    else {
                        Tooltip_AddLine("Shift click to move to crafting slot");
                    }
                }
            }
            break;
        }
        }
        
        if( restoreDurMin || restoreDurMax ){
            if( restoreDurMin == restoreDurMax ){
                sprintf(InfoPanelBuffer, "restores %i points", restoreDurMin);
            }else{
                sprintf(InfoPanelBuffer, "restores %i-%i points", restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine( InfoPanelBuffer );
            
            Tooltip_AddLine( "of current durability" );
        }
        
        if( addDurMin || addDurMax ){
            if( addDurMin == addDurMax ){
                sprintf(InfoPanelBuffer, "+%i to max durability", addDurMin);
            }
            else{
                sprintf(InfoPanelBuffer, "+[%i-%i] to max durability", addDurMin, addDurMax);
            }
            if( addDurLimit ){
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine( InfoPanelBuffer );
            
            if( addDurLimit ){
                sprintf(InfoPanelBuffer, "up to %i points", addDurLimit);
                Tooltip_AddLine( InfoPanelBuffer ); 
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, "(%i for body armor)", elTorso);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
        
        if( IsValueStoneItem(item.baseItemIndex) ){
            Tooltip_AddLine("Sell in town for gold");
        }

    }
    
	if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
		sprintf(InfoPanelBuffer, "Socketed [%i]", item.socketsAmount );
        Tooltip_AddLine( InfoPanelBuffer, C_1_Blue );
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine(InfoPanelBuffer, C_1_Blue);
		}
	}
	
    // Usage hint
    if( !IsVisualTradePanelVisible && !IsStashPanelVisible )
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine("Right click to use");
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
            Tooltip_AddLine("Right click to read");
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine("Right-click to read, then");
            Tooltip_AddLine("left-click to target");
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine("Right click to view");
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, "Level : %i", item.quality);
            Tooltip_AddLine(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine("Doubles gold capacity");
            break;
        }
    
		if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
			Tooltip_AddLine("Right click to use");
		}
        else if( isTownPortalForbidden()  ){
			switch (item.baseItemIndex) {
				case BI_19_FUNGAL_TOME:		 Tooltip_AddLine("Right click to transmute into spectral elixir"); break;
                case BI_2512_FULL_CONTAINERS:Tooltip_AddLine("Right click to transmute into random elixir"); break;
                case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine("Right click to use"); break;
				case BI_16_ANVIL_OF_FURY: 
				case BI_9_MAGIC_ROCK:
				case BI_327_TAVERN_SIGN:
                case BI_2457_IRON_FIGURINE:
                case BI_2459_REAGENTS:
                case BI_2460_THE_RIGHT_GOODS:
                case BI_2510_DUSTY_CRATE:
                case BI_2461_MUNDANE_INVENTORY:
                case BI_334_THEODORE:		 Tooltip_AddLine("Right click to transmute into quest reward"); break;
			}
        }
		if (isTownPortalForbidden()) {
			if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
				Tooltip_AddLine("Right click to transmute into gold");
			}
		}
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) { 
            Tooltip_AddLine("Right click to open (requires key)");
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine("Right click to read (requires crystal eyepiece)");
        }

        if( IsItemAllowedForBelt(item) ){
                if (IsController() && !XinputNotInUse(true)) {
                    if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                        Tooltip_AddLine("Right trigger to move left");
                    }
                    else {
                        Tooltip_AddLine("Right trigger to move to belt");
                    }
                }
                else {
                    if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                        Tooltip_AddLine("Shift Click to move left");
					}else if( ! IsTransmutePanelVisible ){
						Tooltip_AddLine("Shift Click to move to belt");
                    }
                }
        }
    }
 
	// Requirements
	if( item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u ){
		Tooltip_AddLine( "Spell is maxed", C_3_Gold );
	}else if( item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel ){
		strcpy(InfoPanelBuffer, "required:");
		
		auto appendReqInfo = [&player]( const char* name, int req, int current, int magicCode )
        {
            if( req ){
				uchar color;
				if( current >= req ){
					color = C_0_White + 128;
                }else if( name != "lvl" && req <= current + ( is(magicCode,
                 MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET, 
                 MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                 MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser) ){
					color = C_1_Blue + 128;
                }else{
					color = C_2_Red + 128;
                }
                sprintf( InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color );
            }
        };
		
		appendReqInfo( "lvl", item.RequiredLevel, player.CharLevel, item.MagicCode);
		appendReqInfo( "str", item.RequiredStrength, player.CurStrength, item.MagicCode);
		appendReqInfo( "mag", item.RequiredMagic, player.CurMagic, item.MagicCode);
		appendReqInfo( "dex", item.RequiredDexterity, player.CurDexterity, item.MagicCode);
		appendReqInfo( "vit", item.RequiredVitality, player.CurVitality, item.MagicCode );
        Tooltip_AddLine(InfoPanelBuffer);
	}
	
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	uint requiredClassMask = GetRequiredClassMask( &item );
	if( requiredClassMask != IPCM_ALL_CLASSES ){
	    printItemClassReqInfoInBuffer( &item );
	    Tooltip_AddLine( InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red );
	}
 
	if( (playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles ){
		Tooltip_AddLine( "[Can't wear item]", C_2_Red );
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
		if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
			Tooltip_AddLine("[Can't wear body armor]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
		if (is(item.ItemCode, IC_5_SHIELD)) {
			Tooltip_AddLine("[Can't use shields]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
		if (is(item.ItemCode, IC_21_KNIFE)) {
			Tooltip_AddLine("[Can't use throwing knives]", C_2_Red);
		}
	}
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
		if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
			Tooltip_AddLine("[Can't use melee weapons]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
		if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
			Tooltip_AddLine("[Can't use item]", C_2_Red);
		}
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
		if (is(item.MagicCode,
			MC_3_POTION_OF_HEALING,
			MC_6_POTION_OF_MANA,
			MC_2_POTION_OF_FULL_HEALING,
			MC_7_POTION_OF_FULL_MANA,
			MC_18_POTION_OF_REJUVENATION,
			MC_19_POTION_OF_FULL_REJUVENATION)) {
			Tooltip_AddLine("[Can't use potions]", C_2_Red);
		}
	}
	//----------------------
	if( IsVisualTradePanelVisible ){
	
	    if( isVisualTradeSlot ){
	        int price = VisualTrade_GetBuyPrice(&item);
                sprintf(InfoPanelBuffer, "Buy price: %i", price);
                Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to buy fast");
                }
                else {
                    Tooltip_AddLine("Shift click to buy fast");
                }
        }else{
            if( Cur.GraphicsID == CM_1_NORMAL_HAND ){
                if( CurrentVisualTradeMode != VTM_Elder ){
                    int price = VisualTrade_GetSellPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Sell price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer);
                        if (IsController() && !XinputNotInUse(true)) {
                            Tooltip_AddLine("Right Trigger + A to sell fast");
                        }
                        else {
                            Tooltip_AddLine("Shift click to sell fast");
                        }
                    }else{
                        Tooltip_AddLine("Item cannot be sold", C_2_Red);
                    }
                }
            }else if( Cur.IsTradeService ){
                if( Cur.GraphicsID == CM_2_IDENTIFY ){
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else if( Cur.GraphicsID == CM_3_REPAIR ){
                    int price = VisualTrade_GetRepairPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else if( Cur.GraphicsID == CM_4_STAFF_RECHARGE ){
                    int price = VisualTrade_GetRechargePrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }
    
    if( IsStashPanelVisible ){
        if( slot >= IS_Inventory ){
            if( !IsItemAllowedForStash(&item) ){
                Tooltip_AddLine("Cannot be stashed", C_2_Red);
            }else{
                if (IsController() && !XinputNotInUse(true)) {
                    if (isVisualStashSlot) {
                        Tooltip_AddLine("Right Trigger + A to move to inventory");
                    }
                    else {
                        Tooltip_AddLine("Right Trigger + A to move to stash");
                    }
                }
                else {
                    if (isVisualStashSlot) {
                        Tooltip_AddLine("Shift Click to move to inventory");
                    }
                    else {
                        Tooltip_AddLine("Shift Click to move to stash");
                    }
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ){
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine("Shift Click to move to inventory", C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine("Shift Click to transmute panel", C_7_Grey);
		}
	}

    if (SecondaryTooltip && !IsAltPressed) {
        if (((slot > IS_Feet && slot < IS_50_47_Belt_Start)
            || StashPanel_IsStashSlot(slot)
            || VisualTrade_IsVisualTradeSlot(slot)
            || Craft_IsCraftSlot(slot)
            || IsEnchantPanelVisible && slot == 101
			|| IsQuenchPanelVisible  && slot == 102)
			&& !is(item.ItemCode, IC_0_OTHER, IC_11_GOLD)
            )
            if (IsController() && !XinputNotInUse(true)) {
                Tooltip_AddLine("Press Left Trigger to compare", C_7_Grey);
            }
            else {
                Tooltip_AddLine("Press ALT to compare", C_7_Grey);
            }
    }

    if (slot <= IS_50_47_Belt_End) {
        if (IsController() && !XinputNotInUse(true)) {
            Tooltip_AddLine("Press X to drop", C_7_Grey);
        } 
        else if (DropItemOnCtrl) {
            Tooltip_AddLine("Ctrl Click to drop", C_7_Grey);
        }
    }

    if (FastInventoryItemSwap)
    {
        if (!IsVisualTradePanelVisible && !IsStashPanelVisible) {
            if (slot <= IS_Feet) {
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to unequip", C_7_Grey);
                }else{
                    Tooltip_AddLine("Shift Click to unequip", C_7_Grey);
                }
            }else if( slot >= IS_Inventory
			 && (is(BaseItems[item.baseItemIndex].ItemType, ITEM_1_WEAPON, ITEM_2_ARMOR) || is(item.ItemCode, IC_12_RING, IC_13_AMULET))
			 && !IsTransmutePanelVisible ){
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to equip", C_7_Grey);
                }else{
                    if( RightSwapAllowed(CurrentPlayerIndex, item) ){
                        Tooltip_AddLine("Shift Click to equip to left slot", C_7_Grey);
                        if (FastSwapRightOnAlt) {
                            Tooltip_AddLine("Alt Click to equip to right slot", C_7_Grey);
                        }
                    }else{
                        Tooltip_AddLine("Shift Click to equip", C_7_Grey);
                    }
                }
            }
        }
    }
    
 	if (identified && item.MagicLevel == ML_3_SET) {
		UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
		UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

		Tooltip_AddLine(""); // blank line
		bool hasActiveEffects = false;
		int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
		for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
			auto& e = set.Effects[i];
			if (countOfEquippedItemsFromSet >= e.Threshold) {
				if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine(InfoPanelBuffer, C_4_Orange);
				}
				hasActiveEffects = true;
			}
		}
		if (hasActiveEffects) Tooltip_AddLine(""); // blank line
		Tooltip_AddLine(set.Name, C_3_Gold);
		for (int i = 0; i < set.UniquesCount; ++i)
		{
			int uniqueItemIndex = set.UniquesList[i];
			UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
			Tooltip_AddLine(uniq2.Name, SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
		}
	}
}
