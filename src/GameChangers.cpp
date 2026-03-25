#include "stdafx.h"

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
    "Asceticism"
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
It's like Hardcore mode, but instead of losing your character, you lose your equipped items."

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
    case GC_20_2X_DAMAGE:   portrait = PcxGcs[21]; break; // unused: 18
    case GC_21_C_NO_EVIL:   portrait = PcxGcs[20]; break;
    case GC_22_BLACK_DEATH: portrait = PcxGcs[ 6]; break;
    case GC_23_TOUGH_LUCK:  portrait = PcxGcs[18]; break;
    case GC_24_RUTHLESS:    portrait = PcxGcs[18]; break;
    case GC_25_ACCELERATION:portrait = PcxGcs[15]; break;
    case GC_26_ASCETICISM:  portrait = PcxGcs[15]; break;
    default:                portrait = PcxGcs[50]; break;
	}
    return portrait;
}

const char* getGameChangerDescription(const GAME_CHANGER gameChanger)
{
    switch (gameChanger)
    {
    case GC_0_CONTINUE:     return "Game Changers are optional. GCs stay with character forever. GCs can add certain perks and synergies to the pool, or remove some.";
    case GC_1_AUTO_SAVE:    return "Auto-saving removes necessity for player to manually save game, saving is done automatically, like in multiplayer mode.";
    case GC_2_BT_STRIP:     return "Makes it impossible to equip or unequip items when in battle (except hand slots).";
    case GC_3_DROP_ITMS:    return "Dying makes you drop equipped items. Warning: if the dropped items are not retrieved within the same game session, they will disappear.";
    case GC_4_EFFORTLESS:   return "Makes game easier. Monsters only deal half damage to you. Doesn't get easier than that.";
    case GC_5_LOTL:         return "Towners will not sell you any items. They will provide their service though: repair, identify, enchanting, etc.";
    case GC_6_IMPOSSIBLE:   return "Makes game harder. Monsters deal double damage. This is the ultimate difficulty degree you can get.";
    case GC_7_PROHIBITION:  return "Monsters will not drop stat elixirs randomly anymore. Note: Heavy Drinker perk will still give elixirs from unique monsters.";
    case GC_8_FULL_CONTACT: return "Makes game easier. Monsters with ranged attacks don't retreat.";
    case GC_9_NIGHT_KIN:    return "Hungry for blood, you venture deep into the labirynth of Tristram to prey on the weak. This changes gameplay. You don't use items and rely on your innate powers.";
    case GC_10_HACKNSLASH:  return "Makes game easier. There is no need to switch weapon type anymore to adjust to monster type. All weapons do neutral damage. Just hack and slash.";
    case GC_11_VACCINATED:  return "Makes game easier. Monsters lose immunities, and there is no need to switch spells. Stone Curse, Bone Spirit, Elemental and Holy spells are unaffected.";
    case GC_12_MY_SECRET:   return "Makes game easier. Fury doesn't have a cooldown and can be recast as soon as it ends.";
    case GC_13_NO_REGENS:   return "Adds challenge. Character has no basic regeneration of life and mana.";
    case GC_14_CLEAN_HANDS: return "Adds challenge. Monsters do not drop items that can be equipped by character. Player is supposed to acquire them through buying and/or enchanting.";
    case GC_15_ICE_AGE:     return "Cosmetic change. Dungeon levels look like it is deep deep winter, all is iced and frozen. Have you ever imagined how Hell would look like if frozen over?";
    case GC_16_GROUNDHOG:   return "Alters playstyle. Same dungeon is generated every time, populated by the same monsters. You can play and improve timing each time you play.";
    case GC_17_TOUGH_MNSTR: return "Makes game harder. Monsters have +200% hit points.";
    case GC_18_INDESTR_ITM: return "Makes game easier. Items will not be destroyed if durability drops to zero. Instead, items will become unusable until repaired.";
    case GC_19_NO_MAP:      return "Adds challenge. Auto-map is unavailable. You will have to find your way around dungeons without it. Feel lost already?";
    case GC_20_2X_DAMAGE:   return "Makes game easier. Your attacks deal double damage to monsters. Warning: it may become disappointingly easy. Don't choose this if you want to keep the thrill of battle.";
    case GC_21_C_NO_EVIL:   return "Adds challenge. Seeing does not highlight monsters in the darkness.";
    case GC_22_BLACK_DEATH: return "You are infected. The plague of Black Death has taken over you. It is a curse that develops uncontrollably. How long can you last until it claims your life?";
    case GC_23_TOUGH_LUCK:  return "You are unlucky. Starting stats = 5. Get 2 stat points on level-up. -75% to MF/GF/XP. No perks for XP, elixirs, magic find and level-up stats.";
    case GC_24_RUTHLESS:    return "Classic Diablo Ironman conditions: no boost to XP, MF, stats. No oils and IDs from bosses.";
    case GC_25_ACCELERATION:return "Makes game easier. Experience is increased 2x. Warning: this is not intended game pace. Use it only for testing.";
    case GC_26_ASCETICISM:  return "Game is saved automatically. You can't resurrect in town (have to make a new game) and all dropped items are lost when you die. It's a hardcore mode for items.";
    default:                return "wrong gc index";
    }
}

const char* returnONorOFF(const GAME_CHANGER gameChanger) {
    return NewSaveInfo.GameChanger & BIT(gameChanger) ? "ON" : "OFF";
}

const char* getGameChangerName(const GAME_CHANGER gameChanger)
{
	static char text[256];

	switch (gameChanger)
    {
    case GC_0_CONTINUE:     sprintf(text, "Continue"); break;
    case GC_1_AUTO_SAVE:    sprintf(text, "Auto-saving: %s", returnONorOFF(gameChanger)); break;
    case GC_2_BT_STRIP:     sprintf(text, "Bound Gear: %s", returnONorOFF(gameChanger)); break;
    case GC_3_DROP_ITMS:    sprintf(text, "Item Drops: %s", returnONorOFF(gameChanger)); break;
    case GC_4_EFFORTLESS:   sprintf(text, "Effortless: %s", returnONorOFF(gameChanger)); break;
    case GC_5_LOTL:         sprintf(text, "Live off the Land: %s", returnONorOFF(gameChanger)); break;
    case GC_6_IMPOSSIBLE:   sprintf(text, "Impossible: %s", returnONorOFF(gameChanger)); break;
    case GC_7_PROHIBITION:  sprintf(text, "Prohibition: %s", returnONorOFF(gameChanger)); break;
    case GC_8_FULL_CONTACT: sprintf(text, "Full Contact: %s", returnONorOFF(gameChanger)); break;
    case GC_9_NIGHT_KIN:    sprintf(text, "Night Kin: %s", returnONorOFF(gameChanger)); break;
    case GC_10_HACKNSLASH:  sprintf(text, "Hack & Slash: %s", returnONorOFF(gameChanger)); break;
    case GC_11_VACCINATED:  sprintf(text, "Vaccinated: %s", returnONorOFF(gameChanger)); break;
    case GC_12_MY_SECRET:   sprintf(text, "That's My Secret: %s", returnONorOFF(gameChanger)); break;
    case GC_13_NO_REGENS:   sprintf(text, "No Regeneration: %s", returnONorOFF(gameChanger)); break;
    case GC_14_CLEAN_HANDS: sprintf(text, "Clean Hands: %s", returnONorOFF(gameChanger)); break;
    case GC_15_ICE_AGE:     sprintf(text, "Ice Age: %s", returnONorOFF(gameChanger)); break;
    case GC_16_GROUNDHOG:   sprintf(text, "Groundhog Day: %s", returnONorOFF(gameChanger)); break;
    case GC_17_TOUGH_MNSTR: sprintf(text, "Tougher Monsters: %s", returnONorOFF(gameChanger)); break;
    case GC_18_INDESTR_ITM: sprintf(text, "Imperishable Items: %s", returnONorOFF(gameChanger)); break;
    case GC_19_NO_MAP:      sprintf(text, "No Map: %s", returnONorOFF(gameChanger)); break;
    case GC_20_2X_DAMAGE:   sprintf(text, "Double Damage: %s", returnONorOFF(gameChanger)); break;
    case GC_21_C_NO_EVIL:   sprintf(text, "See No Evil: %s", returnONorOFF(gameChanger)); break;
    case GC_22_BLACK_DEATH: sprintf(text, "Black Death: %s", returnONorOFF(gameChanger)); break;
    case GC_23_TOUGH_LUCK:  sprintf(text, "Tough Luck: %s", returnONorOFF(gameChanger)); break;
    case GC_24_RUTHLESS:    sprintf(text, "Ruthless Iron: %s", returnONorOFF(gameChanger)); break;
    case GC_25_ACCELERATION:sprintf(text, "Acceleration: %s", returnONorOFF(gameChanger)); break;
    case GC_26_ASCETICISM:  sprintf(text, "Asceticism: %s", returnONorOFF(gameChanger)); break;
    default:                sprintf(text, "wrong gc index"); break;
	}
    return text;
}

int GetSuperGameChanger(char*) //?
{
    return GC_0_CONTINUE;
}

void __fastcall SetGameChangerInfoText(HWND parent, const GAME_CHANGER gameChanger)
{
    ShowWindowList(parent, CharParamNameList, SW_HIDE);
    ShowWindowList(parent, CharParamValueList, SW_HIDE);
    ShowWindowList(parent, ClassDescriptionList, SW_SHOW);

    Portrait portrait = getGameChangerPortrait(gameChanger);
    const char* description = getGameChangerDescription(gameChanger);

    SetWindowTextA(GetDlgItem(parent, ID_CLASS_DESCRIPTION), description);

    DrawTextToElemList(parent, ClassDescriptionList, 1);

    const HWND portraitWin = GetDlgItem(parent, ID_PLAYER_PORTRAIT);
    RECT rect;
    InvalidateRect(portraitWin, 0, 0);
    GetClientRect(portraitWin, &rect);
    AdjustScrollRect(&rect, 0, rect.bottom * portrait.id);
	SDlgSetBitmap(portraitWin, 0, "Static", -1, 1, portrait.pcx->data, &rect, portrait.pcx->size.cx, portrait.pcx->size.cy, -1);
}

void __fastcall ResetGameChangerButtons(HWND hdlg, const int topIndex)
{
    GameChangersAvailableList.clear();
    GameChangersAvailableList.emplace_back(GC_0_CONTINUE);
    if (MaxCountOfPlayersInGame == 1
        && (!(NewSaveInfo.GameChanger & BIT(GC_26_ASCETICISM)))
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
        ) {
        GameChangersAvailableList.emplace_back(GC_5_LOTL);
    }

    if (MaxCountOfPlayersInGame == 1 
        && NewSaveInfo.GameMode == GM_HARD) {
        GameChangersAvailableList.emplace_back(GC_6_IMPOSSIBLE);
    }

    GameChangersAvailableList.emplace_back(GC_7_PROHIBITION); // always available

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
        && NewSaveInfo.GameMode == GM_EASY) {
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
        (is(NewSaveInfo.GameMode, GM_EASY, GM_NORMAL, GM_SURVIVAL))
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
        && (!has(NewSaveInfo.Traits, TraitId::FearTheReaper))
        && (!(NewSaveInfo.Class == PC_5_SAVAGE && NewSaveInfo.SubClass == PSC_SAVAGE_EXECUTIONER)) // not executioner
        && (!(NewSaveInfo.Class == PC_2_MAGE && NewSaveInfo.SubClass == PSC_MAGE_WARLOCK)) // not warlock
        && (!(NewSaveInfo.GameChanger & BIT(GC_23_TOUGH_LUCK)))
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
            SetGameChangerInfoText(GetParent(hdlg), SelectedGameChanger);
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
        WriteAndDrawDialogTitle(GetParent(hdlg), tempBuf);
        SetWindowLongA(hdlg, GWL_USERDATA, GetWindowLongA(GetParent(hdlg), GWL_USERDATA));
        InitTextElemList(hdlg, GameChangerTitleList, 5);
        InitButtonListText(hdlg, GameChangerOkCancelList, 4, 0);
        InitButtonListText(hdlg, GameChangerButtonList, 2, 1);
        ResetGameChangerButtons(hdlg, 0);
        InitSelectionAndLoadPentagram("ui_art\\focus16.pcx");
        SDlgSetTimer(hdlg, 1, 55, 0);
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
        break;
    }
    case WM_LBUTTONUP:
    {
        if (CheckScrollPressAndRelease(GetDlgItem(hdlg, ID_SCROLL))) {
            GameChangerRefreshScrollState(hdlg);
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
