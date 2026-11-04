#include "stdafx.h"
#include "MenuPortrait.h"

GAME_CHANGER SelectedGameChanger = GC_0_CONTINUE;

int GameChangerTitleList[2] = { ID_CAPTION, 0 };
int GameChangerOkCancelList[3] = { ID_OK_BUTTON, ID_CANCEL_BUTTON, 0 };

int GameChangerButtonList[] = {
    ID_GAME_CHANGER_1,
    ID_GAME_CHANGER_2,
    ID_GAME_CHANGER_3,
    ID_GAME_CHANGER_4,
    ID_GAME_CHANGER_5,
    ID_GAME_CHANGER_6,
    0 };

constexpr std::array nextGameChangerOrder = {
    ID_GAME_CHANGER_2,
    ID_GAME_CHANGER_3,
    ID_GAME_CHANGER_4,
    ID_GAME_CHANGER_5,
    ID_GAME_CHANGER_6,
    ID_GAME_CHANGER_1,
};
constexpr std::array prevGameChangerOrder = {
    ID_GAME_CHANGER_6,
    ID_GAME_CHANGER_1,
    ID_GAME_CHANGER_2,
    ID_GAME_CHANGER_3,
    ID_GAME_CHANGER_4,
    ID_GAME_CHANGER_5, 
};

const char* GC_Names_InfoWindow[GC_COUNT] = {
    /*GC_0_CONTINUE*/
    "None (skiped)",
    /*GC_1_AUTO_SAVE*/
    "Auto-saving",
    /*GC_2_BT_STRIP*/
    "Bound Gear",
    /*GC_3_DROP_ITMS*/
    "Item Drops",
    /*GC_4_EFFORTLESS*/
    "Effortless",
    /*GC_5*/
    "Live off the Land",
    /*GC_6*/
    "Impossible",
    /*GC_7*/
    "Prohibition",
    /*GC_8*/
    "Full Contact",
    /*GC_9*/
    "Night Kin",
    /*GC_10*/
    "Hack & Slash",
    /*GC_11*/
    "Vaccinated",
    /*GC_12*/
    "That's my Secret",
    /*GC_13*/
    "No regeneration",
    /*GC_14*/
    "Clean Hands",
    /*GC_15*/
    "Ice Age",
    /*GC_16*/
    "Groundhog Day",
    /*GC_17*/
    "Tougher Monsters",
    /*GC_18*/
    "Imperishable Items",
    /*GC_19*/
    "No Map",
    /*GC_20*/
    "Double Damage",
    /*GC_21*/
    "See No Evil",
    /*GC_22*/
    "Black Death",
    /*GC_23*/
    "Tough Luck",
    /*GC_24*/
    "Ruthless Iron",
    /*GC_25*/
    "Acceleration",
    /*GC_26*/
    "Asceticism",
    /*GC_27*/
    "Terminator",
    /*GC_28*/
    "Super Mutant",
    /*GC_29*/
    "Enclave Soldier"
};

const char* GC_Descriptions_InfoWindow[GC_COUNT] = {
    /*GC_0_CONTINUE*/
    "None (skiped)",

    /*GC_1_AUTO_SAVE*/
    "Auto-saving\n\n\n\
Alters playstyle\n\n\
Removes necessity for player to manually save game, saving is done automatically, just like in multiplayer mode.",

    /*GC_2_BT_STRIP*/
    "Bound Gear\n\n\n\
Adds challenge\n\n\
Makes it impossible to equip or unequip items when in battle (except hand slots).",

    /*GC_3_DROP_ITMS*/
    "Item Drops\n\n\n\
Adds challenge\n\n\
Dying makes you drop equipped items.\n\n\
Warning: if the dropped items are not retrieved within the same game session, they will disappear.",

    /*GC_4_EFFORTLESS*/
    "Effortless\n\n\n\
Makes game easier\n\n\
Monsters only deal half damage to you.\n\
Doesn't get easier than that.",

    /*GC_5_LOTL*/
    "Live off the Land\n\n\n\
Adds challenge\n\n\
Towners will not sell you any items.\n\n\
They will not stop providing their service though: gossip, quest dialogues, repair, identify, enchanting, crafting, socketing, recharge.",

    /*GC_6*/
    "Impossible\n\n\n\
Makes game harder\n\n\
Monsters deal double damage to you.\n\
Doesn't get harder than that.",

    /*GC_7*/
    "Prohibition\n\n\n\
Adds challenge\n\n\
Monsters will not drop stat elixirs randomly anymore\n\
Note: Heavy Drinker perk will still give elixirs from unique monsters",

    /*GC_8*/
    "Full Contact\n\n\n\
Makes game easier\n\n\
Monsters with ranged attacks don't retreat.",

    /*GC_9*/
"Night Kin\n\n\n\
Makes you a vampire\n\n\
You live and die by the blood. your hunger is insatiable but you have all the tools to prey on the weak\n\n\
This changes gameplay.\n\
You don't use items and rely on your innate powers.",

    /*GC_10*/
"Hack & Slash\n\n\n\
Makes game easier\n\n\
You no longer have to switch weapon types (sharp or blunt) to adjust to monster type. All weapons do standard damage to all monster species.",

    /*GC_11*/
"Vaccinated\n\n\n\
Makes game easier\n\n\
Monsters lose all immunities, and there is no need to switch spells - every type of spell works.\n\
Stone Curse, Bone Spirit, Elemental and Holy spells are unaffected.",

    /*GC_12*/
"That's my Secret\n\n\n\
Makes game easier\n\n\
Fury doesn't have a cooldown and can be recast as soon as it ends.",

    /*GC_13*/
"No Regeneration\n\n\n\
Adds challenge\n\n\
Character has no basic regeneration of life and mana.",

    /*GC_14*/
"Clean Hands\n\n\n\
Adds challenge\n\n\
Monsters do not drop items that can be equipped by character.\n\
Player is supposed to acquire them through buying and/or enchanting.",

    /*GC_15*/
"Ice Age\n\n\n\
Cosmetic change\n\n\
Dungeon levels look like it is deep deep winter, all is iced and frozen.\n\
Have you ever imagined how Hell would look like if frozen over?",

    /*GC_16*/
"Groundhog Day\n\n\n\
Alters playstyle\n\n\
Same dungeon is generated every time, populated by the same monsters. You can play and improve timing each time you play.",

    /*GC_17*/
"Tougher Monsters\n\n\n\
Makes game harder\n\n\
Monsters have +200% hit points.",

    /*GC_18*/
"Imperishable Items\n\n\n\
Makes game easier\n\n\
Items will not be destroyed if durability drops to zero. Instead, items will become unusable until repaired.",

    /*GC_19*/
"No Map\n\n\n\
Adds challenge\n\n\
Auto-map is unavailable\n\
You will have to find your way around dungeons without relying on it\n\n\
Feel lost already?",

    /*GC_20*/
"Double Damage\n\n\n\
Makes game easier\n\n\
Your attacks deal double damage to monsters\n\n\
Warning: it may become disappointingly easy.\n\
Do not choose this if you want to keep the thrill of battle.",

    /*GC_21*/
"See No Evil\n\n\n\
Adds challenge\n\n\
Seeing does not highlight monsters in the darkness.",

    /*GC_22*/
"Black Death\n\n\n\
You are infected\n\n\
The plague of Black Death has taken over you. It will slowly develop adverse symptoms that will eventually inevitably kill you.\n\n\
You cannot cure it, you cannot heal it, and you cannot stop it. Over time, it will get more and more severe. There is no escape from this deadly illness.",

    /*GC_23*/
"Tough Luck\n\n\n\
Adds challenge\n\n\
Luck tends to run out sooner or later. Yours didn't even start.\n\n\
Start game with all stats equal to 5. Get only 2 stat points on level-up. Magic finding skills, experience gained and gold finding ability reduced by 75%. Start game without any items.\n\n\
No perks to help you with experience, elixir drops, magic item finding and level-up stats.",

    /*GC_24*/
"Ruthless Iron\n\n\n\
Adds challenge\n\n\
Classic Diablo Ironman was different.\n\n\
There was no boost to stats per level-up, no extra experience, no oils and IDs from bosses. No improvement to magicfind either.\n\n\
This GC makes you go back to those roots.",

    /*GC_25*/
"Acceleration\n\n\n\
Makes game easier\n\n\
Experience is increased 2x.",

    /*GC_26*/
"Asceticism\n\n\n\
Adds challenge\n\n\
Game is saved automatically.\n\n\
Items are dropped on death. You can't resurrect in town (have to make a new game) and all dropped items are lost because of that.\n\n\
It's like Hardcore mode, but instead of losing your character, you lose your equipped items.",

    /*GC_27*/
"Terminator\n\n\n\
You are a Cyberdyne Systems series unit. Model 101, infiltrator. CPU read-write mode activated. Sent to the past to execute mission.\n\n\
Main target: Uber Diablo.\n\n\
Secondary target: exterminate monsters.\n\n\
Condition: appear human to native subjects.\n\n\
Basic upgrades installed. Additional upgrades available through self-learning.",

/*GC_28*/
"Super Mutant\n\n\n\
Your frame can haul weapons that would snap a normal man in half.\n\n\
Miniguns.\n\n\
Things that shouldn't exist in this world. You don't cast spells anymore. You don't need to.\n\n\
When you pull the trigger, the burst tears through hellspawn like paper in a storm.",

/*GC_29*/
"Enclave Soldier\n\n\n\
Enclave Armed Forces soldier, trained to exterminate mutants and genetic impurity.\n\n\
Finds himself stranded in a world that has never known nuclear fire, but has filth of a different kind - Hellspawn.\n\n\
The enemy has changed. The purpose has not.\n\n\
Abilities: Wields energy weapons and deploys combat drones. Adept at elemental warfare."

};

std::vector<GAME_CHANGER> GameChangersAvailableList;

Portrait getGameChangerPortrait(GAME_CHANGER gameChanger)
{
    Portrait portrait;
    switch (gameChanger)
    {
    case GC_0_CONTINUE:     portrait = PcxGcs[50]; break;
    case GC_1_AUTO_SAVE:    portrait = PcxGcs[ 0]; break;
    case GC_2_BT_STRIP:     portrait = PcxGcs[ 1]; break;
    case GC_3_DROP_ITMS:    portrait = PcxGcs[ 2]; break;
    case GC_4_EFFORTLESS:   portrait = PcxGcs[14]; break;
    case GC_5_LOTL:         portrait = PcxGcs[ 4]; break;
    case GC_6_IMPOSSIBLE:   portrait = PcxGcs[13]; break;
    case GC_7_PROHIBITION:  portrait = PcxGcs[17]; break;
    case GC_8_FULL_CONTACT: portrait = PcxGcs[ 3]; break;
    case GC_9_NIGHT_KIN:    portrait = PcxGcs[ 5]; break;
    case GC_10_HACKNSLASH:  portrait = PcxGcs[19]; break;
    case GC_11_VACCINATED:  portrait = PcxGcs[12]; break;
    case GC_12_MY_SECRET:   portrait = PcxGcs[ 7]; break;
    case GC_13_NO_REGENS:   portrait = PcxGcs[ 8]; break;
    case GC_14_CLEAN_HANDS: portrait = PcxGcs[ 9]; break;
    case GC_15_ICE_AGE:     portrait = PcxGcs[16]; break;
    case GC_16_GROUNDHOG:   portrait = PcxGcs[15]; break;
    case GC_17_TOUGH_MNSTR: portrait = PcxGcs[10]; break;
    case GC_18_INDESTR_ITM: portrait = PcxGcs[11]; break;
    case GC_19_NO_MAP:      portrait = PcxGcs[22]; break;
    case GC_20_2X_DAMAGE:   portrait = PcxGcs[21]; break;
    case GC_21_C_NO_EVIL:   portrait = PcxGcs[20]; break;
    case GC_22_BLACK_DEATH: portrait = PcxGcs[ 6]; break;
    case GC_23_TOUGH_LUCK:  portrait = PcxGcs[18]; break;
    case GC_24_RUTHLESS:    portrait = PcxGcs[28]; break;
    case GC_25_ACCELERATION:portrait = PcxGcs[25]; break;
    case GC_26_ASCETICISM:  portrait = PcxGcs[27]; break;
    case GC_27_TERMINATOR:  portrait = PcxGcs[26]; break;
    case GC_28_SUPERMUTANT: portrait = PcxGcs[23]; break;
    case GC_29_ENCLAVE_SOLDIER: portrait = PcxGcs[24]; break;
    default:                portrait = PcxGcs[50]; break;
	}
    return portrait;
}

const char* getGameChangerDescription(const GAME_CHANGER gameChanger)
{
    switch (gameChanger)
    {
    case GC_0_CONTINUE:     return LMenu("Game Changers are optional. GCs stay with character forever. GCs can add certain perks and synergies to the pool, or remove some.");
    case GC_1_AUTO_SAVE:    return LMenu("Auto-saving removes necessity for player to manually save game, saving is done automatically, like in multiplayer mode.");
    case GC_2_BT_STRIP:     return LMenu("Makes it impossible to equip or unequip items when in battle (except hand slots).");
    case GC_3_DROP_ITMS:    return LMenu("Dying makes you drop equipped items. Warning: if the dropped items are not retrieved within the same game session, they will disappear.");
    case GC_4_EFFORTLESS:   return LMenu("Makes game easier. Monsters only deal half damage to you. Doesn't get easier than that.");
    case GC_5_LOTL:         return LMenu("Towners will not sell you any items. They will provide their service though: repair, identify, enchanting, etc.");
    case GC_6_IMPOSSIBLE:   return LMenu("Makes game harder. Monsters deal double damage. This is the ultimate difficulty degree you can get.");
    case GC_7_PROHIBITION:  return LMenu("Monsters will not drop stat elixirs randomly anymore. Note: Heavy Drinker perk will still give elixirs from unique monsters.");
    case GC_8_FULL_CONTACT: return LMenu("Makes game easier. Monsters with ranged attacks don't retreat.");
    case GC_9_NIGHT_KIN:    return LMenu("Hungry for blood, you venture deep into the labirynth of Tristram to prey on the weak. This changes gameplay. You don't use items and rely on your innate powers.");
    case GC_10_HACKNSLASH:  return LMenu("Makes game easier. There is no need to switch weapon type anymore to adjust to monster type. All weapons do neutral damage. Just hack and slash.");
    case GC_11_VACCINATED:  return LMenu("Makes game easier. Monsters lose immunities, and there is no need to switch spells. Stone Curse, Bone Spirit, Elemental and Holy spells are unaffected.");
    case GC_12_MY_SECRET:   return LMenu("Makes game easier. Fury doesn't have a cooldown and can be recast as soon as it ends.");
    case GC_13_NO_REGENS:   return LMenu("Adds challenge. Character has no basic regeneration of life and mana.");
    case GC_14_CLEAN_HANDS: return LMenu("Adds challenge. Monsters do not drop items that can be equipped by character. Player is supposed to acquire them through buying and/or enchanting.");
    case GC_15_ICE_AGE:     return LMenu("Cosmetic change. Dungeon levels look like it is deep deep winter, all is iced and frozen. Have you ever imagined how Hell would look like if frozen over?");
    case GC_16_GROUNDHOG:   return LMenu("Alters playstyle. Same dungeon is generated every time, populated by the same monsters. You can play and improve timing each time you play.");
    case GC_17_TOUGH_MNSTR: return LMenu("Makes game harder. Monsters have +200% hit points.");
    case GC_18_INDESTR_ITM: return LMenu("Makes game easier. Items will not be destroyed if durability drops to zero. Instead, items will become unusable until repaired.");
    case GC_19_NO_MAP:      return LMenu("Adds challenge. Auto-map is unavailable. You will have to find your way around dungeons without it. Feel lost already?");
    case GC_20_2X_DAMAGE:   return LMenu("Makes game easier. Your attacks deal double damage to monsters. Warning: it may become disappointingly easy. Don't choose this if you want to keep the thrill of battle.");
    case GC_21_C_NO_EVIL:   return LMenu("Adds challenge. Seeing does not highlight monsters in the darkness.");
    case GC_22_BLACK_DEATH: return LMenu("You are infected. The plague of Black Death has taken over you. It is a curse that develops uncontrollably. How long can you last until it claims your life?");
    case GC_23_TOUGH_LUCK:  return LMenu("You are unlucky. Starting stats = 5. Get 2 stat points on level-up. -75% to MF/GF/XP. No perks for XP, elixirs, magic find and level-up stats.");
    case GC_24_RUTHLESS:    return LMenu("Classic Diablo Ironman conditions: no boost to XP, MF, stats. No oils and IDs from bosses.");
    case GC_25_ACCELERATION:return LMenu("Makes game easier. Experience is increased 2x. Warning: this is not intended game pace. Use it only for testing.");
    case GC_26_ASCETICISM:  return LMenu("Game is saved automatically. You can't resurrect in town (have to make a new game) and all dropped items are lost when you die. It's a hardcore mode for items.");
    case GC_27_TERMINATOR:  return LMenu("You are infiltration unit, model 101. Sent to the past to exterminate monsters. They are about to face evil beyond their comprehension...");
    case GC_28_SUPERMUTANT: return LMenu("You can haul weapons that would snap a normal man in half. Things that shouldn't exist in this world. Pull the trigger - the burst tears through everything.");
    case GC_29_ENCLAVE_SOLDIER: return LMenu("Enclave Armed Forces soldier, trained to exterminate mutants and genetic impurity. Finds himself stranded in a world that has never known nuclear fire.");
    default:                return LMenu("wrong gc index");
    }
}

const char* returnONorOFF(const GAME_CHANGER gameChanger) {
    return NewSaveInfo.GameChanger & BIT(gameChanger) ? LMenu("ON") : LMenu("OFF");
}

const char* getGameChangerName(const GAME_CHANGER gameChanger)
{
	static char text[256];

	switch (gameChanger)
    {
    case GC_0_CONTINUE:     sprintf(text, LMenu("Continue")); break;
    case GC_1_AUTO_SAVE:    sprintf(text, LMenu("Auto-saving: %s"), returnONorOFF(gameChanger)); break;
    case GC_2_BT_STRIP:     sprintf(text, LMenu("Bound Gear: %s"), returnONorOFF(gameChanger)); break;
    case GC_3_DROP_ITMS:    sprintf(text, LMenu("Item Drops: %s"), returnONorOFF(gameChanger)); break;
    case GC_4_EFFORTLESS:   sprintf(text, LMenu("Effortless: %s"), returnONorOFF(gameChanger)); break;
    case GC_5_LOTL:         sprintf(text, LMenu("Live off the Land: %s"), returnONorOFF(gameChanger)); break;
    case GC_6_IMPOSSIBLE:   sprintf(text, LMenu("Impossible: %s"), returnONorOFF(gameChanger)); break;
    case GC_7_PROHIBITION:  sprintf(text, LMenu("Prohibition: %s"), returnONorOFF(gameChanger)); break;
    case GC_8_FULL_CONTACT: sprintf(text, LMenu("Full Contact: %s"), returnONorOFF(gameChanger)); break;
    case GC_9_NIGHT_KIN:    sprintf(text, LMenu("Night Kin: %s"), returnONorOFF(gameChanger)); break;
    case GC_10_HACKNSLASH:  sprintf(text, LMenu("Hack & Slash: %s"), returnONorOFF(gameChanger)); break;
    case GC_11_VACCINATED:  sprintf(text, LMenu("Vaccinated: %s"), returnONorOFF(gameChanger)); break;
    case GC_12_MY_SECRET:   sprintf(text, LMenu("That's My Secret: %s"), returnONorOFF(gameChanger)); break;
    case GC_13_NO_REGENS:   sprintf(text, LMenu("No Regeneration: %s"), returnONorOFF(gameChanger)); break;
    case GC_14_CLEAN_HANDS: sprintf(text, LMenu("Clean Hands: %s"), returnONorOFF(gameChanger)); break;
    case GC_15_ICE_AGE:     sprintf(text, LMenu("Ice Age: %s"), returnONorOFF(gameChanger)); break;
    case GC_16_GROUNDHOG:   sprintf(text, LMenu("Groundhog Day: %s"), returnONorOFF(gameChanger)); break;
    case GC_17_TOUGH_MNSTR: sprintf(text, LMenu("Tougher Monsters: %s"), returnONorOFF(gameChanger)); break;
    case GC_18_INDESTR_ITM: sprintf(text, LMenu("Imperishable Items: %s"), returnONorOFF(gameChanger)); break;
    case GC_19_NO_MAP:      sprintf(text, LMenu("No Map: %s"), returnONorOFF(gameChanger)); break;
    case GC_20_2X_DAMAGE:   sprintf(text, LMenu("Double Damage: %s"), returnONorOFF(gameChanger)); break;
    case GC_21_C_NO_EVIL:   sprintf(text, LMenu("See No Evil: %s"), returnONorOFF(gameChanger)); break;
    case GC_22_BLACK_DEATH: sprintf(text, LMenu("Black Death: %s"), returnONorOFF(gameChanger)); break;
    case GC_23_TOUGH_LUCK:  sprintf(text, LMenu("Tough Luck: %s"), returnONorOFF(gameChanger)); break;
    case GC_24_RUTHLESS:    sprintf(text, LMenu("Ruthless Iron: %s"), returnONorOFF(gameChanger)); break;
    case GC_25_ACCELERATION:sprintf(text, LMenu("Acceleration: %s"), returnONorOFF(gameChanger)); break;
    case GC_26_ASCETICISM:  sprintf(text, LMenu("Asceticism: %s"), returnONorOFF(gameChanger)); break;
    case GC_27_TERMINATOR:  sprintf(text, LMenu("Terminator: %s"), returnONorOFF(gameChanger)); break;
    case GC_28_SUPERMUTANT: sprintf(text, LMenu("Super Mutant: %s"), returnONorOFF(gameChanger)); break;
    case GC_29_ENCLAVE_SOLDIER: sprintf(text, LMenu("Enclave Soldier: %s"), returnONorOFF(gameChanger)); break;
    default:                sprintf(text, LMenu("wrong gc index")); break;
	}
    return text;
}

int GetSuperGameChanger(char*) //?
{
    return GC_0_CONTINUE;
}


// (th4) SCROLLABLE GAME CHANGER DESCRIPTION ---------------------------------------------
//
// Same mechanism as the trait description (see the block above SetTraitInfoText in
// Traits.cpp) -- read that one first, this is a direct copy of it with the GC id as the
// cache key. The short version:
//
// The description field (ID_CLASS_DESCRIPTION on the parent SELHERO dialog) is a fixed
// box and anything past it was silently cut -- and the GC texts are the longest in the
// menu (Black Death, Tough Luck, Terminator all overflow). It now scrolls, with the same
// arrow scrollbar the GC list uses, sitting on its right (ID_DESC_SCROLL). The control
// is on the CHILD dialog, not the parent, because the child is modal and is the one
// receiving the mouse -- both are 256x203 at the same origin, so it lines up.
//
// DrawFontString lays the text out exactly as before but skips the first
// g_MenuTextSkipLines wrapped lines and reports the total in g_MenuTextLineCount. The
// scrollbar's "line count" is the number of scroll POSITIONS (total - visible + 1) and
// its "selected index" is the top line, which is how SetScrollOnElem sizes the thumb.
static int g_GcDescTop  = 0;                 // first visible wrapped line
static int g_GcDescMax  = 0;                 // last legal top (0 when it all fits)
static int g_GcDescPage = 1;                 // lines visible at once

// The scroll follows the text: it resets when the shown GC changes and survives a
// redraw of the same one.
// (th4) Reset the scroll only when the DESCRIPTION TEXT actually changes.
//
// This used to compare an id instead, and that was wrong: SetGameChangerInfoText is also called
// from the BN_SETFOCUS arm of the dialog proc, so simply moving the mouse over the list
// re-runs the whole info draw. On that path the id is re-derived from the freshly
// focused button and can differ for an instant from the one the scroll belongs to, which
// snapped the text back to the top BETWEEN clicks. A debug log of real clicks showed it
// plainly: "ScrollBy: top 1 -> 2" followed by an unprompted "UpdateScroll: top=0" with
// the same totalLines, so every second click appeared to do nothing.
// Comparing the rendered text is immune to that -- it cannot be fooled by an id that is
// recomputed to a different value for the same description, and it still resets properly
// when the player really does move to another entry.
static char g_GcDescShownText[ 512 ] = { 0 };
static void GcDescNoteShown( const char* description )
{
    const char* d = description ? description : "";
    if( strncmp( d, g_GcDescShownText, sizeof( g_GcDescShownText ) - 1 ) != 0 ){
        strncpy( g_GcDescShownText, d, sizeof( g_GcDescShownText ) - 1 );
        g_GcDescShownText[ sizeof( g_GcDescShownText ) - 1 ] = 0;
        g_GcDescTop = 0;
    }
}

// Size and show/hide the scrollbar after a draw. Mirrors the GC list, which hides its
// own scrollbar when there are six entries or fewer.
static void GcDescUpdateScroll( HWND scrollDlg, TextWin* field, int totalLines )
{
    if( !scrollDlg || !field ) return;
    const int fontH = GetCurFontHeight() > 0 ? GetCurFontHeight() : 1;
    g_GcDescPage = field->size.cy / fontH;
    if( g_GcDescPage < 1 ) g_GcDescPage = 1;
    g_GcDescMax = totalLines - g_GcDescPage;
    if( g_GcDescMax < 0 ) g_GcDescMax = 0;
    if( g_GcDescTop > g_GcDescMax ) g_GcDescTop = g_GcDescMax;
    if( g_GcDescTop < 0 ) g_GcDescTop = 0;
    const HWND scroll = GetDlgItem( scrollDlg, ID_DESC_SCROLL );
    if( !scroll ) return;
    SetScrollOnElem( scrollDlg, ID_DESC_SCROLL, g_GcDescMax + 1, g_GcDescTop );
    const bool want = g_GcDescMax > 0;
    if( want != ( IsWindowVisible( scroll ) != 0 ) ){
        ShowWindow( scroll, want ? SW_SHOW : SW_HIDE );
        // (th4) These dialogs are painted onto a surface, not by Windows: hiding a
        // control does NOT clear its pixels, so the bar lingers until something else
        // repaints -- which looks like the bar lagging a click behind. The child is
        // WS_EX_TRANSPARENT, so the parent has to repaint underneath it too.
        const HWND parent = GetParent( scrollDlg );
        if( parent ){ InvalidateRect( parent, NULL, FALSE ); UpdateWindow( parent ); }
        InvalidateRect( scrollDlg, NULL, FALSE ); UpdateWindow( scrollDlg );
    }
}


// (th4) scrollDlg is the CHILD list dialog that owns ID_DESC_SCROLL. Callers that have
// no scrollbar (none today) may omit it; the description then simply does not scroll.
void __fastcall SetGameChangerInfoText(HWND parent, const GAME_CHANGER gameChanger, HWND scrollDlg = nullptr)
{
    ShowWindowList(parent, CharParamNameList, SW_HIDE);
    ShowWindowList(parent, CharParamValueList, SW_HIDE);
    ShowWindowList(parent, ClassDescriptionList, SW_SHOW);

    Portrait portrait = getGameChangerPortrait(gameChanger);
    const char* description = getGameChangerDescription(gameChanger);

    SetWindowTextA(GetDlgItem(parent, ID_CLASS_DESCRIPTION), description);
    GcDescNoteShown( description );   // (th4) reset the scroll only on a real change

    // (th4) The draw is told how many wrapped lines to skip and reports how many the
    // text has in all; the scrollbar is sized from that. Skip is reset straight after
    // so no other menu text inherits it.
    g_MenuTextSkipLines = g_GcDescTop;
    DrawTextToElemList(parent, ClassDescriptionList, is(GameLanguage, LANG_RU, LANG_BG, LANG_UK) ? 0 : 1);
    g_MenuTextSkipLines = 0;
    {
        const HWND field = GetDlgItem( parent, ID_CLASS_DESCRIPTION );
        GcDescUpdateScroll( scrollDlg, field ? (TextWin*) GetWindowLongA( field, GWL_USERDATA ) : nullptr, g_MenuTextLineCount );
    }

    const HWND portraitWin = GetDlgItem(parent, ID_PLAYER_PORTRAIT);
    RECT rect;
    InvalidateRect(portraitWin, 0, 0);
    GetClientRect(portraitWin, &rect);
    AdjustScrollRect(&rect, 0, rect.bottom * portrait.id);
	SDlgSetBitmap(portraitWin, 0, "Static", -1, 1, portrait.pcx->data, &rect, portrait.pcx->size.cx, portrait.pcx->size.cy, -1);
	// (th4) hand the true-colour overlay what is being shown; it re-reads the
	// control each frame, so it stops by itself when the dialog closes.
	MenuPortrait_Set( portrait.pcx ? portrait.pcx->name : 0, portrait.id, portraitWin, portrait.pcx ? portrait.pcx->data : 0, portrait.pcx ? (int) portrait.pcx->size.cx : 0 );
}

void __fastcall ResetGameChangerButtons(HWND hdlg, const int topIndex)
{
    GameChangersAvailableList.clear();
    GameChangersAvailableList.emplace_back(GC_0_CONTINUE);
   
    if (MaxCountOfPlayersInGame == 1
        && (!(NewSaveInfo.GameChanger & BIT(GC_26_ASCETICISM)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        && is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_HARD, GM_HARDCORE)) {
        GameChangersAvailableList.emplace_back(GC_1_AUTO_SAVE);
    }

    GameChangersAvailableList.emplace_back(GC_2_BT_STRIP); // always available

    if ((MaxCountOfPlayersInGame > 1 || NewSaveInfo.GameChanger & BIT(GC_1_AUTO_SAVE)) 
        && is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_HARD)) {
        GameChangersAvailableList.emplace_back(GC_3_DROP_ITMS);
    }

    if (MaxCountOfPlayersInGame == 1 && NewSaveInfo.GameMode == GM_EASY) {
        GameChangersAvailableList.emplace_back(GC_4_EFFORTLESS);
    }

    if (!has(NewSaveInfo.Traits, TraitId::Leper)
        && (!(NewSaveInfo.GameChanger & BIT(GC_14_CLEAN_HANDS)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_26_ASCETICISM)))
        && NewSaveInfo.GameMode != GM_OUTBREAK
        ) {
        GameChangersAvailableList.emplace_back(GC_5_LOTL);
    }

    if (MaxCountOfPlayersInGame == 1 
        && NewSaveInfo.GameMode == GM_HARD) {
        GameChangersAvailableList.emplace_back(GC_6_IMPOSSIBLE);
    }

    if (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)) 
        && NewSaveInfo.GameMode != GM_OUTBREAK
        ) {
        GameChangersAvailableList.emplace_back(GC_7_PROHIBITION);
    }

    if (MaxCountOfPlayersInGame == 1 
        && NewSaveInfo.GameMode != GM_HARD) {
        GameChangersAvailableList.emplace_back(GC_8_FULL_CONTACT);
    }

    if (MaxCountOfPlayersInGame == 1 && (NewSaveInfo.Class == PC_5_SAVAGE && NewSaveInfo.SubClass == PSC_SAVAGE_EXECUTIONER)) {
        if (
            (!has(NewSaveInfo.Traits, TraitId::Leper))
            && (!has(NewSaveInfo.Traits, TraitId::Barbarism))
            && (!has(NewSaveInfo.Traits, TraitId::Weird))
            && (!has(NewSaveInfo.Traits, TraitId::Scrounger))
            && (!has(NewSaveInfo.Traits, TraitId::TreasureHunter))
            && (!has(NewSaveInfo.Traits, TraitId::NastyDisposition))
            && (!has(NewSaveInfo.Traits, TraitId::CrowdSeeker))
            && (!has(NewSaveInfo.Traits, TraitId::LitheBuild))
            //&& (is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_HARD, GM_HARDCORE))
            ) {
            GameChangersAvailableList.emplace_back(GC_9_NIGHT_KIN);
        }
    }

    if (MaxCountOfPlayersInGame == 1 // SP
        && NewSaveInfo.GameMode == GM_EASY // EASY
        && (!is(NewSaveInfo.Class, PC_1_ARCHER, PC_2_MAGE)) // not archers or magi
        && (!has(NewSaveInfo.Traits, TraitId::Axepertise)) // Axe Rogue doesn't need it (axe is neutral damage)
        && (!has(NewSaveInfo.Traits, TraitId::Bestiarius)) // Bestiarius can't have it..
        && (!has(NewSaveInfo.Traits, TraitId::Pistoleer)) 
        && (!has(NewSaveInfo.Traits, TraitId::TwoTowers)) // Maiden with 2 shields does not use weapons
        && (!(NewSaveInfo.Class == PC_0_WARRIOR && NewSaveInfo.SubClass == PSC_WARRIOR_INQUISITOR)) // not Inquisitor
        && (!(NewSaveInfo.Class == PC_3_MONK && NewSaveInfo.SubClass == PSC_MONK_SHUGOKI)) // not Shugoki
        && (!(NewSaveInfo.Class == PC_4_ROGUE && NewSaveInfo.SubClass == PSC_ROGUE_BOMBARDIER)) // not Bombardier
        && (!(NewSaveInfo.Class == PC_5_SAVAGE && NewSaveInfo.SubClass == PSC_SAVAGE_BERSERKER)) // not Berserker
        ) {
        GameChangersAvailableList.emplace_back(GC_10_HACKNSLASH);
    }

    if (MaxCountOfPlayersInGame == 1 // SP
        && NewSaveInfo.GameMode == GM_EASY
        && ((NewSaveInfo.Class == PC_2_MAGE && NewSaveInfo.SubClass == PSC_MAGE_MAGE)
            || (NewSaveInfo.Class == PC_2_MAGE && NewSaveInfo.SubClass == PSC_MAGE_ELEMENTALIST)// mor: Mamluk still has it, might want to remove it from him
            || (NewSaveInfo.Class == PC_2_MAGE && NewSaveInfo.SubClass == PSC_MAGE_WARLOCK)
            //|| (NewSaveInfo.Class == PC_1_ARCHER && NewSaveInfo.SubClass == PSC_ARCHER_SCOUT)
            //|| (NewSaveInfo.Class == PC_0_WARRIOR && NewSaveInfo.SubClass == PSC_WARRIOR_INQUISITOR)
            || (NewSaveInfo.Class == PC_4_ROGUE && NewSaveInfo.SubClass == PSC_ROGUE_BOMBARDIER))
        ) {
        GameChangersAvailableList.emplace_back(GC_11_VACCINATED);
    }

    if (MaxCountOfPlayersInGame == 1 
        && NewSaveInfo.GameMode == GM_EASY
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        ) {
        GameChangersAvailableList.emplace_back(GC_12_MY_SECRET);
    }

    if (
        (!has(NewSaveInfo.Traits, TraitId::Adrenaline))
        && (!has(NewSaveInfo.Traits, TraitId::Necropathy))
        && (!has(NewSaveInfo.Traits, TraitId::GrimDeal))
        && (!has(NewSaveInfo.Traits, TraitId::DarkPact))
        && (!has(NewSaveInfo.Traits, TraitId::Barbarism))
        && (!has(NewSaveInfo.Traits, TraitId::Survivor))
        && (!has(NewSaveInfo.Traits, TraitId::FastMetabolism))
        && (!has(NewSaveInfo.Traits, TraitId::BlisteredSkin))
        && (!has(NewSaveInfo.Traits, TraitId::Black_Witchery))
        && (!has(NewSaveInfo.Traits, TraitId::Psion)) 
        && (!has(NewSaveInfo.Traits, TraitId::Paladin))
        && (!has(NewSaveInfo.Traits, TraitId::Insensitive))
        && (!has(NewSaveInfo.Traits, TraitId::BloodOath))
        && (!has(NewSaveInfo.Traits, TraitId::Warlord))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        ) {
        GameChangersAvailableList.emplace_back(GC_13_NO_REGENS);
    }

    if (
        (!has(NewSaveInfo.Traits, TraitId::Scrounger))
        && (!has(NewSaveInfo.Traits, TraitId::TreasureHunter))
        && (!has(NewSaveInfo.Traits, TraitId::Barbarian))
        && (!has(NewSaveInfo.Traits, TraitId::NastyDisposition))
        && (!(NewSaveInfo.Class == PC_5_SAVAGE && NewSaveInfo.SubClass == PSC_SAVAGE_EXECUTIONER)) // executioner cannot have clean hands
        && (!(NewSaveInfo.GameChanger & BIT( GC_5_LOTL )))
        && (is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_HARD, GM_HARDCORE))
        ) {
        GameChangersAvailableList.emplace_back(GC_14_CLEAN_HANDS);
    }

    if (MaxCountOfPlayersInGame == 1) {
        GameChangersAvailableList.emplace_back(GC_15_ICE_AGE);
    }

    if (MaxCountOfPlayersInGame == 1 
        && NewSaveInfo.GameMode == GM_SPEEDRUN) {
        GameChangersAvailableList.emplace_back(GC_16_GROUNDHOG);
    }

    if (MaxCountOfPlayersInGame == 1 
         && NewSaveInfo.GameMode != GM_EASY) {
        GameChangersAvailableList.emplace_back(GC_17_TOUGH_MNSTR);
    }

    if (
        (is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_SURVIVAL, GM_OUTBREAK))
        && (!(NewSaveInfo.GameChanger & BIT(GC_9_NIGHT_KIN)))
        ) {
        GameChangersAvailableList.emplace_back(GC_18_INDESTR_ITM);
    }

    GameChangersAvailableList.emplace_back(GC_19_NO_MAP);

    if (
        MaxCountOfPlayersInGame == 1
        && NewSaveInfo.GameMode == GM_EASY
        && (!has(NewSaveInfo.Traits, TraitId::Psion))
        && (!has(NewSaveInfo.Traits, TraitId::Devastator))
        && (!has(NewSaveInfo.Traits, TraitId::HolyAura))
        ) {
        GameChangersAvailableList.emplace_back(GC_20_2X_DAMAGE);
    }

    if (NewSaveInfo.GameMode != GM_EASY) {
        GameChangersAvailableList.emplace_back(GC_21_C_NO_EVIL);
    }

    if (
        NewSaveInfo.GameMode != GM_EASY
        && (!has(NewSaveInfo.Traits, TraitId::Sisyphean))
        && (!has(NewSaveInfo.Traits, TraitId::Forgetful))
        && (!has(NewSaveInfo.Traits, TraitId::Adrenaline))
        && (!has(NewSaveInfo.Traits, TraitId::Prodigy)) 
        && (!has(NewSaveInfo.Traits, TraitId::Insensitive))
        && (!has(NewSaveInfo.Traits, TraitId::RawPower))
        && (!has(NewSaveInfo.Traits, TraitId::FearTheReaper))
        && (!(NewSaveInfo.Class == PC_5_SAVAGE && NewSaveInfo.SubClass == PSC_SAVAGE_EXECUTIONER)) // not executioner
        && (!(NewSaveInfo.Class == PC_2_MAGE && NewSaveInfo.SubClass == PSC_MAGE_WARLOCK)) // not warlock
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_29_ENCLAVE_SOLDIER)))
        ) {
        GameChangersAvailableList.emplace_back(GC_22_BLACK_DEATH);
    }

    if (
        (is(NewSaveInfo.GameMode, GM_NORMAL, GM_HARD))
        && (!has(NewSaveInfo.Traits, TraitId::GrimDeal))
        && (!has(NewSaveInfo.Traits, TraitId::Gifted))
        && (!has(NewSaveInfo.Traits, TraitId::Adventurer))
        && (!has(NewSaveInfo.Traits, TraitId::Forgetful))
        && (!has(NewSaveInfo.Traits, TraitId::BlueBlood))
        && (!has(NewSaveInfo.Traits, TraitId::Domesticated))
        && (!has(NewSaveInfo.Traits, TraitId::Rudiarius))
        && (!has(NewSaveInfo.Traits, TraitId::Sisyphean))
        && (!has(NewSaveInfo.Traits, TraitId::FearTheReaper))
        && (!(NewSaveInfo.GameChanger & BIT(GC_22_BLACK_DEATH)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_28_SUPERMUTANT)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_29_ENCLAVE_SOLDIER)))
        ) {
        GameChangersAvailableList.emplace_back(GC_23_TOUGH_LUCK);
    }

    if (
        MaxCountOfPlayersInGame == 1
        && NewSaveInfo.GameMode == GM_IRONMAN
        ) {
        GameChangersAvailableList.emplace_back(GC_24_RUTHLESS);
    }

    if (
        MaxCountOfPlayersInGame == 1
        && NewSaveInfo.GameMode == GM_EASY
        && (!has(NewSaveInfo.Traits, TraitId::Sisyphean))
        ) {
        GameChangersAvailableList.emplace_back(GC_25_ACCELERATION);
    }

    if (
        MaxCountOfPlayersInGame == 1
        && is(NewSaveInfo.GameMode, GM_NORMAL, GM_HARD)
        && (!has(NewSaveInfo.Traits, TraitId::Sisyphean))
        && (!(NewSaveInfo.GameChanger & BIT(GC_22_BLACK_DEATH)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_9_NIGHT_KIN)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_1_AUTO_SAVE)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_3_DROP_ITMS)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_5_LOTL)))
        ) {
        GameChangersAvailableList.emplace_back(GC_26_ASCETICISM);
    }

    if (
        MaxCountOfPlayersInGame == 1
        //&& not(NewSaveInfo.GameMode, GM_HARD)
        && (!has(NewSaveInfo.Traits, TraitId::Sisyphean))
        && (!has(NewSaveInfo.Traits, TraitId::CrossbowTraining))
        && (!has(NewSaveInfo.Traits, TraitId::Juggernaut))
        && (!has(NewSaveInfo.Traits, TraitId::FearTheReaper))
        && (!has(NewSaveInfo.Traits, TraitId::NastyDisposition))
        && (!has(NewSaveInfo.Traits, TraitId::GrimDeal))
        && (!has(NewSaveInfo.Traits, TraitId::Giant))
        && (!has(NewSaveInfo.Traits, TraitId::LitheBuild))
        && (!has(NewSaveInfo.Traits, TraitId::Gifted))
        && (!has(NewSaveInfo.Traits, TraitId::Survivor))
        && (!has(NewSaveInfo.Traits, TraitId::Weird))
        && (!has(NewSaveInfo.Traits, TraitId::Domesticated))
        && (!has(NewSaveInfo.Traits, TraitId::Negotiant))
        && (!has(NewSaveInfo.Traits, TraitId::Adventurer))
        && (!has(NewSaveInfo.Traits, TraitId::RatelHide))
        && (!has(NewSaveInfo.Traits, TraitId::FastMetabolism))
        && (!has(NewSaveInfo.Traits, TraitId::BendTheRules))
        && (!has(NewSaveInfo.Traits, TraitId::BlueBlood))
        && (!has(NewSaveInfo.Traits, TraitId::Coinbound))
        && (has(NewSaveInfo.Traits, TraitId::Pistoleer))
        && (!(NewSaveInfo.GameChanger & BIT(GC_22_BLACK_DEATH)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_1_AUTO_SAVE)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_13_NO_REGENS)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_28_SUPERMUTANT)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_29_ENCLAVE_SOLDIER)))
        ) {
        GameChangersAvailableList.emplace_back(GC_27_TERMINATOR);
    }

    if (
        MaxCountOfPlayersInGame == 1
        && (!has(NewSaveInfo.Traits, TraitId::CrossbowTraining))
        && (!has(NewSaveInfo.Traits, TraitId::GrimDeal))
        && (!has(NewSaveInfo.Traits, TraitId::LitheBuild))
        && (!has(NewSaveInfo.Traits, TraitId::SmallFrame))
        && (!has(NewSaveInfo.Traits, TraitId::Vigorous))
        && (!has(NewSaveInfo.Traits, TraitId::Zealot))
        && (!has(NewSaveInfo.Traits, TraitId::Adventurer))
        && (!has(NewSaveInfo.Traits, TraitId::Scrounger))
        && (!has(NewSaveInfo.Traits, TraitId::Negotiant))
        && (!has(NewSaveInfo.Traits, TraitId::RatelHide))
        && (!has(NewSaveInfo.Traits, TraitId::BendTheRules))
        && (!has(NewSaveInfo.Traits, TraitId::BlueBlood))
        && (!has(NewSaveInfo.Traits, TraitId::Gifted))
        && (has(NewSaveInfo.Traits, TraitId::Pistoleer))
        && (!(NewSaveInfo.GameChanger & BIT(GC_22_BLACK_DEATH)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_29_ENCLAVE_SOLDIER)))
        ) {
        GameChangersAvailableList.emplace_back(GC_28_SUPERMUTANT);
    }

    if (
        MaxCountOfPlayersInGame == 1
        && (!has(NewSaveInfo.Traits, TraitId::CrossbowTraining))
        //&& (!has(NewSaveInfo.Traits, TraitId::Juggernaut))
        //&& (!has(NewSaveInfo.Traits, TraitId::NastyDisposition))
        //&& (!has(NewSaveInfo.Traits, TraitId::GrimDeal))
        //&& (!has(NewSaveInfo.Traits, TraitId::LitheBuild))
        //&& (!has(NewSaveInfo.Traits, TraitId::SmallFrame))
        && (!has(NewSaveInfo.Traits, TraitId::Vigorous))
        && (!has(NewSaveInfo.Traits, TraitId::Zealot))
        && (!has(NewSaveInfo.Traits, TraitId::BendTheRules))
        && (has(NewSaveInfo.Traits, TraitId::Pistoleer))
        && (!(NewSaveInfo.GameChanger & BIT(GC_22_BLACK_DEATH)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_27_TERMINATOR)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
        && (!(NewSaveInfo.GameChanger & BIT(GC_28_SUPERMUTANT)))
        //&& (NewSaveInfo.GameChanger & BIT(GC_9_NIGHT_KIN))// rem this!
        ) {
        GameChangersAvailableList.emplace_back(GC_29_ENCLAVE_SOLDIER);
    }
    // ----- END OF DATA -----

	for (int i = 0; GameChangerButtonList[i] != 0; ++i) {
        int saveButtonId = GameChangerButtonList[i];
        HWND saveButton = GetDlgItem(hdlg, saveButtonId);
        if (saveButton) {
            const uint currentGameChangerIndex = topIndex + i;
            if( currentGameChangerIndex < GameChangersAvailableList.size() ){
                EnableWindow(saveButton, true);
                if (auto gameChangerWin = (GameChangerWin*)GetWindowLongA(saveButton, GWL_USERDATA); gameChangerWin) { //todo
                    WriteTextToElemData((TextWin*)gameChangerWin, getGameChangerName(GameChangersAvailableList[currentGameChangerIndex]));
                    gameChangerWin->gameChangerIndex = currentGameChangerIndex;
                }
            }
            else {
                EnableWindow(saveButton, false);
            }
        }
    }
    ResetButtonText(hdlg, GameChangerButtonList, 2, 1);
}

void GameChangerRefreshCurrentGameChangerInfo(HWND hdlg, int button)
{
    if (const HWND activeElem = GetDlgItem(hdlg, button); activeElem) {
        if (const auto gameChangerWin = (GameChangerWin*)GetWindowLongA(activeElem, GWL_USERDATA); gameChangerWin) { //todo
            SelectedGameChanger = GameChangersAvailableList[gameChangerWin->gameChangerIndex];
            SetGameChangerInfoText(GetParent(hdlg), SelectedGameChanger, hdlg);
        }
    }
}

int __fastcall GetGameChangerElemIndex(HWND elem)
{
    int result = 0;
    if (elem) {
        if (auto* gameChangerWin = (GameChangerWin*)GetWindowLongA(elem, GWL_USERDATA); gameChangerWin) { //todo
            return gameChangerWin->gameChangerIndex;
        }
    }
    return result;
}

void GameChangerRefreshScrollState(const HWND hdlg)
{
    SetScrollOnElem(hdlg, ID_SCROLL, GameChangersAvailableList.size(), GetGameChangerElemIndex(GetFocus()));
}

// (th4) Scroll the description by `delta` wrapped lines and redraw it.
static void GcDescScrollBy( HWND hdlg, int delta )
{
    int top = g_GcDescTop + delta;
    if( top > g_GcDescMax ) top = g_GcDescMax;
    if( top < 0 ) top = 0;
    if( top == g_GcDescTop ) return;      // at an end: no move, no sound -- same as the list
    g_GcDescTop = top;
    PlaySoundTitleMove();                 // (th4) S_75_I_TITLEMOV, what the list scroll plays
    SetGameChangerInfoText( GetParent( hdlg ), SelectedGameChanger, hdlg );
	// (th4) Force the repaint the list scrollbar gets for free.
	// These dialogs are painted onto a surface, not by Windows. SetScrollOnElem writes the
	// new thumb position into the scrollbar's surface, but nothing puts that surface on
	// screen. The LIST scrollbar never noticed because every list scroll calls SetFocus,
	// and the BN_SETFOCUS arm of the dialog proc does exactly this InvalidateRect/
	// UpdateWindow pair ("removes menu freezes"). Scrolling TEXT changes no focus, so
	// nothing repainted and the bar only caught up on the NEXT click -- it looked like it
	// reacted to every other click.
	// Parent first: the child is WS_EX_TRANSPARENT and shows what is underneath it.
	{
		const HWND parentDlg = GetParent( hdlg );
		if( parentDlg ){ InvalidateRect( parentDlg, NULL, FALSE ); UpdateWindow( parentDlg ); }
		InvalidateRect( hdlg, NULL, FALSE ); UpdateWindow( hdlg );
	}
}

// Shared WM_LBUTTONDOWN arm for the description scrollbar. Action codes are the ones
// GetScrollAction hands the GC list: 1 up-arrow, 2 down-arrow, 3 page up, 4 page down.
static bool GcDescHandleScrollClick( HWND hdlg, int cursorX, int cursorY )
{
    const HWND scroll = GetDlgItem( hdlg, ID_DESC_SCROLL );
    if( !scroll || !IsWindowVisible( scroll ) ) return false;
    if( !CheckCursorOnButton( hdlg, scroll, cursorX, cursorY ) ) return false;
    switch( GetScrollAction( scroll, cursorX, cursorY ) ){
        case 1: GcDescScrollBy( hdlg, -1 ); break;
        case 2: GcDescScrollBy( hdlg, +1 ); break;
        case 3: GcDescScrollBy( hdlg, -g_GcDescPage ); break;
        case 4: GcDescScrollBy( hdlg, +g_GcDescPage ); break;
        default: break;
    }
    return true;
}

void GameChangerSelectNextGameChanger(const HWND hdlg, const HWND button, int order)
{
    const auto& buttonsOrder = (order > 0) ? nextGameChangerOrder : prevGameChangerOrder;
    HWND currentButton = button;
    for (auto i{ 0u }, ie{ buttonsOrder.size() }; i < ie; ++i) {
        currentButton = GetDlgItem(hdlg, buttonsOrder[GetWindowLongA(currentButton, GWL_ID) - ID_GAME_CHANGER_1]);
        if (IsWindowEnabled(currentButton)) {
            SetFocus(currentButton);
            break;
        }
    }
}

void __fastcall GameChangerPageDown(const HWND button)
{
    const HWND hdlg = GetParent(button);
    if (!hdlg) {
        return;
    }

    const HWND firstBut = GetDlgItem(hdlg, ID_GAME_CHANGER_1);
    if (!firstBut) {
        return;
    }

    auto gameChangerWin = (GameChangerWin*)GetWindowLongA(GetDlgItem(hdlg, ID_GAME_CHANGER_6), GWL_USERDATA); //todo
    if (!gameChangerWin) {
        return;
    }

    if (uint gameChangerIndex = gameChangerWin->gameChangerIndex; gameChangerIndex + 1 < GameChangersAvailableList.size()) { //todo
        const int topSaveIndex = GetGameChangerElemIndex(firstBut);
        const int nextTopGameChangerIndex = std::min(topSaveIndex + 6, (int)GameChangersAvailableList.size() - 6);
        PlaySoundTitleMove();
        ResetGameChangerButtons(hdlg, nextTopGameChangerIndex);
        GameChangerRefreshCurrentGameChangerInfo(hdlg, GetWindowLongA(button, GWL_ID));
        GameChangerRefreshScrollState(hdlg);
    }
    else {
        GameChangerSelectNextGameChanger(hdlg, firstBut, -1);
    }
}

void __fastcall GameChangerPageUp(const HWND button)
{
    const auto hdlg = GetParent(button);
    if (!hdlg) {
        return;
    }

    const HWND firstBut = GetDlgItem(hdlg, ID_GAME_CHANGER_1);
    if (!firstBut) {
        return;
    }

    const auto gameChangerWin = (const GameChangerWin*)GetWindowLongA(firstBut, GWL_USERDATA); //todo
    if (!gameChangerWin) {
        return;
    }

    if (const auto gameChangerIndex = gameChangerWin->gameChangerIndex; gameChangerIndex == 0) { //todo
        const auto lastBut = GetDlgItem(hdlg, ID_GAME_CHANGER_6);
        GameChangerSelectNextGameChanger(hdlg, lastBut, 1);
    }
    else {
        const int topSaveIndex = GetGameChangerElemIndex(firstBut);
        const int nextTopGameChangerIndex = std::max(topSaveIndex - 6, 0);
        PlaySoundTitleMove();
        ResetGameChangerButtons(hdlg, nextTopGameChangerIndex);
        GameChangerRefreshCurrentGameChangerInfo(hdlg, GetWindowLongA(button, GWL_ID));
        GameChangerRefreshScrollState(hdlg);
    }
}

void __fastcall GameChangerDown(const HWND button)
{
    const auto hdlg = GetParent(button);

    if (auto gameChangerWin = (GameChangerWin*)GetWindowLongA(button, GWL_USERDATA); gameChangerWin) { //todo
        if (auto gameChangerIndex = gameChangerWin->gameChangerIndex; gameChangerIndex + 1 < GameChangersAvailableList.size()) {
            if (GetWindowLongA(button, GWL_ID) < ID_GAME_CHANGER_6) {
                GameChangerSelectNextGameChanger(hdlg, button, 1);
            }
            else {
                if (auto newBut = GetDlgItem(hdlg, ID_GAME_CHANGER_2); newBut) {
                    if (auto newWin = (GameChangerWin*)GetWindowLongA(newBut, GWL_USERDATA); newWin) {//todo
                        const auto nextTopGameChangerIndex = newWin->gameChangerIndex;
                        PlaySoundTitleMove();
                        ResetGameChangerButtons(hdlg, nextTopGameChangerIndex);
                        GameChangerRefreshCurrentGameChangerInfo(hdlg, GetWindowLongA(button, GWL_ID));
                        GameChangerRefreshScrollState(hdlg);
                    }
                }
            }
        }
    }
}

void __fastcall GameChangerUp(const HWND button)
{
    const auto hdlg = GetParent(button);

    if (GetWindowLongA(button, GWL_ID) > ID_GAME_CHANGER_1) {
        GameChangerSelectNextGameChanger(hdlg, button, -1);
        return;
    }
    else {
        if (auto gameChangerWin = (GameChangerWin*)GetWindowLongA(button, GWL_USERDATA); gameChangerWin) {//todo
            if (auto gameChangerIndex = gameChangerWin->gameChangerIndex; gameChangerIndex > 0) {
                const auto nextTopGameChangerIndex = gameChangerIndex - 1;
                PlaySoundTitleMove();
                ResetGameChangerButtons(hdlg, nextTopGameChangerIndex);
                GameChangerRefreshCurrentGameChangerInfo(hdlg, GetWindowLongA(button, GWL_ID));
                GameChangerRefreshScrollState(hdlg);
            }
        }
    }
}

LRESULT __stdcall GameChangerSelectButtonProc(const HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam)
{
    XinputVirtualKeyboard();

    LRESULT result = 0;
    auto oldProc = (WNDPROC)GetPropA(button, "UIOLDPROC");
    const auto hdlg = GetParent(button);

    switch (Msg) {
    case WM_GETDLGCODE:
        result = 4;
        break;
    case WM_KEYFIRST:
        switch (wParam) {
        case VK_RETURN:
        case VK_SPACE:
            SendMessageA(hdlg, 273, 1, 0);
            break;
        case VK_ESCAPE:
            SendMessageA(hdlg, 273, 2, 0);
            break;
        case VK_TAB:
            GameChangerSelectNextGameChanger(hdlg, button, (GetKeyState(VK_SHIFT) >= 0) ? 1 : -1);
            break;
        case VK_LEFT:
        case VK_UP:
            GameChangerUp(button);
            break;
        case VK_RIGHT:
        case VK_DOWN:
            GameChangerDown(button);
            break;
        case VK_PRIOR:
            GameChangerPageUp(button);
            break;
        case VK_NEXT:
            GameChangerPageDown(button);
            break;
        case VK_DELETE:
            SendMessageA(hdlg, Msg, wParam, lParam);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        RemovePropA(button, "UIOLDPROC");
        if (oldProc) {
            SetWindowLongA(button, GWL_WNDPROC, (LONG)oldProc);
        }
        [[fallthrough]];
    default:
        if (oldProc) {
            result = CallWindowProcA_(oldProc, button, Msg, wParam, lParam);
        }
        else {
            result = DefWindowProcA_(button, Msg, wParam, lParam);
        }
        break;
    case WM_PAINT:
        PaintElem(button);
        break;
    }
    return result;
}

int __stdcall DialogSelectGameChangers(HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam)
{
    int result;
    bool needToCallDefDialogProc = true;

    XinputVirtualKeyboard();

    switch (Msg) {
    case WM_INITDIALOG:
    {
		if( GameMode == GM_COLISEUM || GameMode == GM_CLASSIC ){
            SelectedGameChanger = GAME_CHANGER::GC_0_CONTINUE;
            SDlgEndDialog(hdlg, lParam == 1 ? -1 : 0);
            return SDlgDefDialogProc_(hdlg, Msg, wParam, lParam);
        }
        SelectedGameChanger = GAME_CHANGER::GC_0_CONTINUE;

        for (int i = 0; GameChangerButtonList[i] != 0; ++i) {
            int saveButtonId = GameChangerButtonList[i];
            HWND saveButton = GetDlgItem(hdlg, saveButtonId);
            if (saveButton) {
                SetPropA(saveButton, "UIOLDPROC", (HANDLE)GetWindowLongA(saveButton, GWL_WNDPROC));
                SetWindowLongA(saveButton, GWL_WNDPROC, (LONG)GameChangerSelectButtonProc);
            }
        }

        char tempBuf[32];
        LoadStringA(HInstance, NewHeroTitle(), tempBuf, 31);
        // (th4) Widened from LANG_RU to all non-English, matching the already-widened
        // Traits.cpp dialog: LMenu resolves per-language, so ID/PT/ES get their overlay
        // translations here too. English returns the input unchanged.
        // tempBuf is char[32] and strcpy is unbounded, so copy with an explicit bound.
        if( GameLanguage != LANG_EN ){
            // (th4) All six titles, Dark Campaign and the mode variants included.
            if( const char* t = NewHeroTitleText() ){
                strncpy( tempBuf, t, sizeof(tempBuf) - 1 );
                tempBuf[ sizeof(tempBuf) - 1 ] = 0;
            }
            SetWindowTextA( GetDlgItem( hdlg, ID_CAPTION ), LMenu("Select Game Changers") );
            SetWindowTextA( GetDlgItem( hdlg, ID_OK_BUTTON ), LMenu("OK") );
            SetWindowTextA( GetDlgItem( hdlg, ID_CANCEL_BUTTON ), LMenu("Cancel") );
        }
        WriteAndDrawDialogTitle(GetParent(hdlg), tempBuf);
        SetWindowLongA(hdlg, GWL_USERDATA, GetWindowLongA(GetParent(hdlg), GWL_USERDATA));
        InitTextElemList(hdlg, GameChangerTitleList, 5);
        InitButtonListText(hdlg, GameChangerOkCancelList, 4, 0);
        InitButtonListText(hdlg, GameChangerButtonList, 2, 1);
        ResetGameChangerButtons(hdlg, 0);
        InitSelectionAndLoadPentagram("ui_art\\focus16.pcx");
        SDlgSetTimer(hdlg, 1, 55, 0);
        InitScroll(hdlg, ID_DESC_SCROLL);                          // (th4) description scrollbar
        ShowWindow(GetDlgItem(hdlg, ID_DESC_SCROLL), SW_HIDE);     // shown by the first draw if needed
        InitScroll(hdlg, ID_SCROLL);
        if( GameChangersAvailableList.size() <= 6 ){
            ShowWindow(GetDlgItem(hdlg, ID_SCROLL), 0);
        }
        needToCallDefDialogProc = false;
        result = 0;
        break;
    }
    case WM_DESTROY:
    {
        DeleteScroll(hdlg, ID_DESC_SCROLL);                        // (th4)
        DeleteScroll(hdlg, ID_SCROLL);
        DeletePentagram();
        DeleteElementListData(hdlg, GameChangerButtonList);
        DeleteElementListData(hdlg, GameChangerOkCancelList);
        DeleteElementListData(hdlg, GameChangerTitleList);
        WriteAndDrawDialogTitle(GetParent(hdlg), 0);
        break;
    }
    case WM_COMMAND:
    {
        if (HIWORD_IDA(wParam) == BN_KILLFOCUS) {
            LeaveButton(hdlg, (HWND)lParam);
        }
        else if (HIWORD_IDA(wParam) == BN_SETFOCUS) {
            InvalidateRect(GetParent(hdlg), NULL, NULL); //removes menu freezes // character selection menu
            UpdateWindow(GetParent(hdlg)); //removes menu freezes // character selection menu
            SelectButton((HWND)lParam);
            AnimateSelectionPentagram(hdlg, (HWND)lParam);
            GameChangerRefreshCurrentGameChangerInfo(hdlg, (unsigned __int16)wParam);
            GameChangerRefreshScrollState(hdlg);
        }
        else if (HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1) { // Enter
            PlaySoundTitleSelect();
            SDlgKillTimer(hdlg, 1);
            SDlgEndDialog(hdlg, 1);
        }
        else if ((ushort)wParam == 2) {
            PlaySoundTitleSelect();
            SDlgKillTimer(hdlg, 1);
            SDlgEndDialog(hdlg, 2);
        }
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    {
        int cursorX = (unsigned __int16)lParam;
        int cursorY = (unsigned int)lParam >> 16;
        if (CheckCursorOnButton(hdlg, GetDlgItem(hdlg, ID_OK_BUTTON), cursorX, cursorY)) {
            PlaySoundTitleSelect();
            SDlgKillTimer(hdlg, 1);
            SDlgEndDialog(hdlg, 1);
        }
        else if (CheckCursorOnButton(hdlg, GetDlgItem(hdlg, ID_CANCEL_BUTTON), cursorX, cursorY)) {
            PlaySoundTitleSelect();
            SDlgKillTimer(hdlg, 1);
            SDlgEndDialog(hdlg, 2);
        }
        else if (CheckCursorOnButton(hdlg, GetDlgItem(hdlg, ID_SCROLL), cursorX, cursorY)) {
            switch (GetScrollAction(GetDlgItem(hdlg, ID_SCROLL), cursorX, cursorY)) {
            case 1:
                GameChangerUp(GetFocus());
                break;
            case 2:
                GameChangerDown(GetFocus());
                break;
            case 3:
                GameChangerPageUp(GetFocus());
                break;
            case 4:
                GameChangerPageDown(GetFocus());
                break;
            default:
                break;
            }
        }
        else {
            GcDescHandleScrollClick(hdlg, cursorX, cursorY);   // (th4) description scrollbar
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (CheckScrollPressAndRelease(GetDlgItem(hdlg, ID_SCROLL))) {
            GameChangerRefreshScrollState(hdlg);
        }
        if (CheckScrollPressAndRelease(GetDlgItem(hdlg, ID_DESC_SCROLL))) {   // (th4) let go of an arrow: repaint it
            SetScrollOnElem(hdlg, ID_DESC_SCROLL, g_GcDescMax + 1, g_GcDescTop);
        }
        break;
    }
    case WM_TIMER:
        AnimateSelectionPentagram(hdlg, GetFocus());
        needToCallDefDialogProc = false;
        result = 0;
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        SendMessageA(SDrawGetFrameWindow(nullptr), Msg, wParam, lParam);
        break;
    }

    if (needToCallDefDialogProc) {
        result = SDlgDefDialogProc_(hdlg, Msg, wParam, lParam);
    }

    return result;
}
