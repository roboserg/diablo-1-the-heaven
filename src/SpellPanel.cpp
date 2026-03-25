#include "stdafx.h"


DisplayObject SpellBookRect;
DisplayObject SpellBookTextRect;
DisplayObject SpellBookPageButtonsRect;
DisplayObject SpellBookSpellButtonsRect;


//----- (th2) -------------------------------------------------------------
PLAYER_SPELL LearnedSpells(int page, int line)
{
	static PLAYER_SPELL BasePages[GUI_SpellBook_PagesAmount * GUI_SpellBook_SpellsPerPageAmount] = {
		// page 1
		PS_0_NONE,
		PS_22_FURY,
		PS_7_TOWN_PORTAL,
		PS_16_REFLECT,
		PS_33_TELEKINES,
		PS_11_MANA_SHIELD,
		PS_23_TELEPORT,
		PS_2_HEALING,
		PS_34_HEAL_OTHER,

		// page 2
		PS_1_FIREBOLT,
		PS_20_INCINERATE,
		PS_6_FIRE_WALL,
		PS_12_FIREBLAST,
		PS_19_FLAME_RING,
		PS_41_FIERY_NOVA,
		PS_13_HYDRA,
		PS_M1_NONE,
		PS_M1_NONE,

		// page 3
		PS_30_CHARGED_BOLT,
		PS_3_LIGHTNING,
		PS_40_LIGHTING_WALL,
		PS_14_BALL_LIGHTNING,
		PS_17_LIGHTING_RING,
		PS_18_LIGHTNING_NOVA,
		PS_35_ARCANE_STAR,
		PS_4_FLASH,
		PS_43_ARCANE_NOVA,

		// page 4
		PS_31_HOLY_BOLT,
		PS_39_HOLY_NOVA,
		PS_36_BONE_SPIRIT,
		PS_15_FORCE_WAVE,
		PS_21_GOLEM,
		PS_8_STONE_CURSE,
		PS_29_ELEMENTAL,
		PS_M1_NONE,
		PS_M1_NONE,

		// page 5
		PS_56_ICE_BOLT,
		PS_57_FREEZING_BALL,
		PS_58_FROST_NOVA,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_52_LESSER_SUMMON,
		PS_53_COMMON_SUMMON,
		PS_54_GREATER_SUMMON,

		// page 6
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
		PS_M1_NONE,
	};
	constexpr auto pos = [](int page, int line){ return page * GUI_SpellBook_SpellsPerPageAmount + line; };
	int index = pos(page, line);
	PLAYER_SPELL spell = BasePages[index];
	Player& player = Players[CurrentPlayerIndex];
	switch( index ){
	case pos(0, 0):
		switch( player.ClassID ){
		case PC_0_WARRIOR: spell = PS_26_ITEM_REPAIR; break;
		case PC_1_ARCHER : spell = PS_9_INFRAVISION; break;
		case PC_2_MAGE   : spell = PS_27_STAFF_RECHARGE; break;
		case PC_3_MONK   : spell = PS_33_TELEKINES; break;
		case PC_4_ROGUE  : spell = PS_5_IDENTIFY; break;
		case PC_5_SAVAGE : spell = PS_16_REFLECT; break;
		}
		if( player.fullClassId == PFC_IRON_MAIDEN || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) spell = PS_26_ITEM_REPAIR;
		else if( player.gameChanger & BIT(GC_9_NIGHT_KIN) ) spell = PS_9_INFRAVISION;
		else if ( HasTrait(CurrentPlayerIndex, TraitId::Hydramancer)) spell = PS_13_HYDRA;
		else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin)) spell = PS_11_MANA_SHIELD;
		//else if (player.fullClassId == PFC_WARLOCK) spell = PS_13_HYDRA;
		break;
	case pos(4, 0): if( player.fullClassId == PFC_WARLOCK ) spell = PS_59_RANCID_BOLT; break;
	case pos(4, 1): if( player.fullClassId == PFC_WARLOCK ) spell = PS_60_TOXIC_BALL; break;
	case pos(4, 2): if( player.fullClassId == PFC_WARLOCK ) spell = PS_61_ACID_NOVA; break;
	case pos(1, 6): if( player.fullClassId == PFC_ELEMENTALIST ) spell = PS_M1_NONE; break;
	case pos(4, 3): if( player.fullClassId == PFC_ELEMENTALIST ) spell = PS_13_HYDRA; break;
	}
	return spell;
}

//----- (th2) -------------------------------------------------------------
void WriteSummonSpellBookInfo(PLAYER_SPELL minionTypeSpell, int LINE_HEIGHT, int& lineIndex) {
	
	auto drawLine = [&](const char* text, int baseFontColor = C_0_White)
	{
		DrawTextColored(SpellBookTextRect.Left, SpellBookTextRect.Top + 11 + 8 + lineIndex * LINE_HEIGHT, SpellBookTextRect.Right, text, baseFontColor);
		++lineIndex;
	};

	int minAccuracyFirst, maxAccuracyFirst, minAccuracySecond, maxAccuracySecond, minMinDamageFirst, maxMinDamageFirst, minMaxDamageFirst, maxMaxDamageFirst, minMinDamgeSecond, maxMinDamageSecond, minMaxDamageSecond, maxMaxDamageSecond, minArmor, maxArmor, minLife, maxLife;
	Player& owner = Players[CurrentPlayerIndex];
	int clvl = owner.CharLevel;
	int slvl = PlayerSpellLevel(CurrentPlayerIndex, minionTypeSpell);

	if (minionTypeSpell == PS_21_GOLEM) {
		if (GameMode == GM_CLASSIC) {
			minLife = 2 * owner.MaxCurMana / 3 + ((10 * slvl) << 6);
			maxLife = minLife;
			minArmor = 25;
			maxArmor = minArmor;
			minAccuracyFirst = 40 + 2 * clvl + 5 * slvl;
			maxAccuracyFirst = minAccuracyFirst;
			minMinDamageFirst = 8 + 2 * slvl;
			maxMinDamageFirst = minMinDamageFirst;
			minMaxDamageFirst = 16 + 2 * slvl;
			maxMaxDamageFirst = minMaxDamageFirst;
				// the rest is not used but i'll leave it here just because
			//toHitSecond = slvl + 2 * clvl;
			//minDamageSecond = 2 * (slvl + 1);
			//maxDamageSecond = 4 * (slvl + 10); 
		}
		else {
			minLife = (((owner.CurMagic * clvl) / 10) << 6) + ((clvl * clvl) << 6) + 6400 + owner.MaxCurMana;
			maxLife = minLife + ((10 * clvl) << 6);
			minAccuracyFirst = slvl + 2 * clvl;
			maxAccuracyFirst = minAccuracyFirst;
			minAccuracySecond = slvl + 2 * clvl;
			maxAccuracySecond = minAccuracySecond;
			minMinDamageFirst = owner.CurMagic * clvl / 1000 + clvl / 2 + 1 + slvl;
			maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 2;
			minMaxDamageFirst = owner.CurMagic * clvl / 500 + 3 + clvl / 2 + slvl;
			maxMaxDamageFirst = minMaxDamageFirst + (clvl / 3) + 2;
			minMinDamgeSecond = 2 * (slvl + 1);
			maxMinDamageSecond = minMinDamgeSecond;
			minMaxDamageSecond = 4 * (slvl + 10);
			maxMaxDamageSecond = minMaxDamageSecond;
			minArmor = owner.CurMagic * clvl / 500 + (clvl / 2) + slvl;
			maxArmor = minArmor + (clvl / 10) + 2;
		}

		sprintf(InfoPanelBuffer, "creates a golem");
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, "to serve you");
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, "(does physical damage)");
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);
	}
	int fclass = owner.fullClassId;
	if (is(fclass, PFC_DEMONOLOGIST, PFC_MAGE, PFC_NECROMANCER, PFC_BEASTMASTER)) {
		int summontype = 0;
		switch (minionTypeSpell) {
		case PS_21_GOLEM: summontype = SUM_GOLEM;			break;
		case PS_52_LESSER_SUMMON: summontype = SUM_LESSER;	break;
		case PS_53_COMMON_SUMMON: summontype = SUM_COMMON;	break;
		case PS_54_GREATER_SUMMON: summontype = SUM_GREATER; break;
		}
		sprintf(InfoPanelBuffer, "Max amount: %i", CalculateSummonsMaxAmount(summontype, slvl, CurrentPlayerIndex));
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);
	}
	else if (is(fclass, PFC_ARCHER, PFC_SHARPSHOOTER, PFC_SCOUT)){
		sprintf(InfoPanelBuffer, "Extra resistance to");
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, "elemental damage: %i%%", PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 0));
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);
	}

	switch (owner.fullClassId) {
		case PFC_DEMONOLOGIST:
			switch (minionTypeSpell) {
				case PS_52_LESSER_SUMMON: {
					minLife = (((owner.CurMagic * clvl) / 22) << 6) + ((3 * clvl) << 6) + (owner.MaxCurMana / 3);
					maxLife = minLife + ((clvl + 15) << 6);
					minAccuracyFirst = owner.CurMagic * clvl / 666 + clvl / 2 + slvl / 2 + 12;
					maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
					minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
					maxAccuracySecond = minAccuracySecond;
					minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 8) : (clvl / 4)) + slvl / 2 + 2;
					maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 2;
					minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 8) : (clvl / 4)) + slvl / 2 + (is(GameMode, GM_EASY, GM_CLASSIC) ? 7 : 6);
					maxMaxDamageFirst = minMaxDamageFirst + (clvl / 4) + 3;
					minMinDamgeSecond = owner.CurMagic * clvl / 337 + clvl / 2 + slvl;
					maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2; 
					minMaxDamageSecond = owner.CurMagic * clvl / 250 + clvl / 2 + slvl + 2;
					maxMaxDamageSecond = minMaxDamageSecond + (clvl / 3) + 2;
					minArmor = owner.CurMagic * clvl / 2000 + clvl / 2 + slvl;
					maxArmor = minArmor + (clvl / 10) + 5;

					sprintf(InfoPanelBuffer, "summons a flying imp");
					drawLine(InfoPanelBuffer);
					break;
				}
				case PS_53_COMMON_SUMMON: {
						minLife = (((owner.CurMagic * clvl) / 13) << 6) + (owner.MaxCurMana / 3);
						maxLife = minLife + (((2 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 666 + clvl / 3 + slvl;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 3 + 4;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 3;
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 3 + (is(GameMode, GM_EASY, GM_CLASSIC) ? 15 : 13);
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 260 + clvl / 2 + slvl;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2;
						minMaxDamageSecond = owner.CurMagic * clvl / 208 + clvl / 2 + slvl + 2;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 3) + 2;
						minArmor = owner.CurMagic * clvl / 2000 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons goat archer");
						drawLine(InfoPanelBuffer);
						break;
				}
				case PS_54_GREATER_SUMMON: {
					minLife = ((owner.CurMagic * clvl / 5) << 6) + (owner.MaxCurMana * 2);
						maxLife = minLife + (((10 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 400 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 2 + 4;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 7) + 3;
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 2 + 9;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 260 + clvl / 2 + slvl;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2;
						minMaxDamageSecond = owner.CurMagic * clvl / 208 + clvl / 2 + slvl + 2;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 10) + 5;
						minArmor = owner.CurMagic * clvl / 300 + clvl / 2 + 2 * slvl + 1;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons satyr lord");
						drawLine(InfoPanelBuffer);
						break;
				}
			}	break;
		case PFC_NECROMANCER:
			switch (minionTypeSpell) {
				case PS_52_LESSER_SUMMON: {
						minLife = (((owner.CurMagic * clvl) / 16) << 6) + (owner.MaxCurMana / 5);
						maxLife = minLife + ((clvl + 5) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 500 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 6) : (clvl / 3)) + (slvl / 2) + (is(GameMode, GM_EASY, GM_CLASSIC) ? 6 : 5);
						maxMinDamageFirst = minMinDamageFirst + ((clvl / 10) + 1);
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 6) : (clvl / 3)) + (slvl / 2) + 8;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 7) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 337 + clvl / 2 + slvl;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2;
						minMaxDamageSecond = owner.CurMagic * clvl / 250 + clvl / 2 + slvl + 2;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 3) + 2;
						minArmor = owner.CurMagic * clvl / 2000 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons skeletal archer");
						drawLine(InfoPanelBuffer);
						break;
				}
				case PS_53_COMMON_SUMMON: {
						minLife = (((owner.CurMagic * clvl) / 7) << 6) + (owner.MaxCurMana / 2);
						maxLife = minLife + (((2 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 333 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 666 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2)) + slvl / 2 + (is(GameMode, GM_EASY, GM_CLASSIC) ? 8 : 7);
						maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 5;
						minMaxDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2)) + slvl / 2 + (is(GameMode, GM_EASY, GM_CLASSIC) ? 16 : 14);
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 260 + clvl / 2 + slvl;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2;
						minMaxDamageSecond = owner.CurMagic * clvl / 208 + clvl / 2 + slvl + 2;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 3) + 2;
						minArmor = owner.CurMagic * clvl / 500 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons skeleton warrior");
						drawLine(InfoPanelBuffer);
						break;
				}
				case PS_54_GREATER_SUMMON: {
						minLife = ((owner.CurMagic * clvl / 5) << 6) + (owner.MaxCurMana * 2);
						maxLife = minLife + (((10 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 300 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 2) : (clvl / 2)) + slvl / 2 + 5;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 7) + 5;
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 2) : (clvl / 2)) + slvl / 2 + 20;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 400 + clvl + slvl / 2 + 14;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 10) + 5;
						minMaxDamageSecond = owner.CurMagic * clvl / 300 + clvl + slvl / 2 + 23;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 8) + 5;
						minArmor = owner.CurMagic * clvl / 500 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons skeleton lord");
						drawLine(InfoPanelBuffer);
						break;
				}
			} break;
		case PFC_BEASTMASTER:
			switch (minionTypeSpell) {
				case PS_52_LESSER_SUMMON: {
						minLife = (((owner.CurMagic * clvl) / 17) << 6) + ((5 * clvl) << 6) + (owner.MaxCurMana / 3);
						maxLife = minLife + ((clvl + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 666 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 3)) + slvl / 2 + 5;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 2;
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 3)) + slvl / 2 + 8;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 3) + 3;
						minMinDamgeSecond = owner.CurMagic * clvl / 337 + clvl / 2 + slvl;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 2;
						minMaxDamageSecond = owner.CurMagic * clvl / 250 + clvl / 2 + slvl + 2;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 3) + 2;
						minArmor = owner.CurMagic * clvl / 800 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons a stinger");
						drawLine(InfoPanelBuffer);
						break;
				}
				case PS_53_COMMON_SUMMON: {
						minLife = (((owner.CurMagic * clvl) / 6) << 6) + (owner.MaxCurMana / 2);
						maxLife = minLife + (((2 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 500 + clvl / 2 + slvl / 2 + 12;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 500 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2)) + slvl / 2 + 5;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 4;
						minMaxDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 5) : (clvl / 2)) + slvl / 2 + 14;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 500 + clvl + slvl / 2 + 5;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 5) + 5;
						minMaxDamageSecond = owner.CurMagic * clvl / 400 + clvl + slvl / 2 + 14;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 8) + 5;
						minArmor = owner.CurMagic * clvl / 500 + clvl / 2 + slvl;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons hulking boar");
						drawLine(InfoPanelBuffer);
						break;
				}
				case PS_54_GREATER_SUMMON: {
						minLife = ((owner.CurMagic * clvl / 2) << 6) + (owner.MaxCurMana * 2);
						maxLife = minLife + (((10 * clvl) + 15) << 6);
						minAccuracyFirst = owner.CurMagic * clvl / 266 + clvl / 2 + slvl / 2 + 15;
						maxAccuracyFirst = minAccuracyFirst + (clvl / 10) + 6;
						minAccuracySecond = (3 * slvl / 2) + 3 * clvl / 2;;
						maxAccuracySecond = minAccuracySecond;
						minMinDamageFirst = owner.CurMagic * clvl / 400 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 2 + 7;
						maxMinDamageFirst = minMinDamageFirst + (clvl / 5) + 4;
						minMaxDamageFirst = owner.CurMagic * clvl / 333 + (GameMode == GM_HARD ? (clvl / 4) : (clvl / 2)) + slvl / 2 + 18;
						maxMaxDamageFirst = minMaxDamageFirst + (clvl / 8) + 4;
						minMinDamgeSecond = owner.CurMagic * clvl / 400 + clvl + slvl / 2 + 14;
						maxMinDamageSecond = minMinDamgeSecond + (clvl / 10) + 5;
						minMaxDamageSecond = owner.CurMagic * clvl / 300 + clvl + slvl / 2 + 23;
						maxMaxDamageSecond = minMaxDamageSecond + (clvl / 8) + 5;
						minArmor = owner.CurMagic * clvl / 400 + clvl / 2 + slvl + 1;
						maxArmor = minArmor + (clvl / 10) + 5;

						sprintf(InfoPanelBuffer, "summons greater beast");
						drawLine(InfoPanelBuffer);
						break;
				}
			}
	}
	if (minionTypeSpell != PS_21_GOLEM) {
		sprintf(InfoPanelBuffer, "to fight enemies for you");
		drawLine(InfoPanelBuffer);
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);
	}

	int best_defense_trait_penalty = HasTrait(CurrentPlayerIndex, TraitId::BestDefense) ? +(25 + (owner.CharLevel / 15)) : 0;
	minLife += minLife * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	minLife += ((minLife * owner.summonHpPercent) / 100 + (owner.summonHp << 6)); // hp increase from affixes
	minLife += PerkValue(PERK_TOUGH_MINIONS, CurrentPlayerIndex) << 6; // +hp from perks
	minLife = minLife * (100 - best_defense_trait_penalty) / 100; //added here because it changes TOTAL hit points. including affix and perk buffs
	maxLife += maxLife * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	maxLife += ((maxLife * owner.summonHpPercent) / 100 + (owner.summonHp << 6)); // hp increase from affixes
	maxLife += PerkValue(PERK_TOUGH_MINIONS, CurrentPlayerIndex) << 6; // +hp from perks
	maxLife = maxLife * (100 - best_defense_trait_penalty) / 100; //added here because it changes TOTAL hit points. including affix and perk buffs

	minAccuracyFirst += owner.summonToHit;
	minAccuracyFirst += minAccuracyFirst * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	minAccuracyFirst += PerkValue(PERK_ACCURATE_MINIONS, CurrentPlayerIndex); // +accuracy from perks
	maxAccuracyFirst += owner.summonToHit;
	maxAccuracyFirst += maxAccuracyFirst * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	maxAccuracyFirst += PerkValue(PERK_ACCURATE_MINIONS, CurrentPlayerIndex); // +accuracy from perks

	int best_defense_benefit = HasTrait(CurrentPlayerIndex, TraitId::BestDefense) ? (25 + owner.CharLevel / 15) : 0;
	minMinDamageFirst += minMinDamageFirst * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	minMinDamageFirst += minMinDamageFirst * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMin;
	minMinDamageFirst += PerkValue(PERK_STRONG_MINIONS, CurrentPlayerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, CurrentPlayerIndex); // +damage from perks
	
	maxMaxDamageFirst += maxMaxDamageFirst * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	maxMaxDamageFirst += maxMaxDamageFirst * (owner.summonDamagePercent + best_defense_benefit) / 100 + owner.summonDamageMax;
	maxMaxDamageFirst += PerkValue(PERK_STRONG_MINIONS, CurrentPlayerIndex) + PerkValue(SYNERGY_MINION_OFFENSE, CurrentPlayerIndex); // +damage from perks
	
	if (minionTypeSpell == PS_54_GREATER_SUMMON) {
		switch (owner.fullClassId) {
		case PFC_DEMONOLOGIST: minMinDamageFirst *= 2; maxMaxDamageFirst *= 2; break;
		case PFC_NECROMANCER: minMinDamageFirst *= 2; maxMaxDamageFirst *= 2; break;
		case PFC_BEASTMASTER: minMinDamageFirst = minMinDamageFirst * 5 / 2; maxMaxDamageFirst = maxMaxDamageFirst * 5 / 2;
		}
	}

	minArmor += minArmor * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100; 
	minArmor += (minArmor * owner.summonAcPercent) / 100 + owner.summonAc;
	minArmor += PerkValue(PERK_ARMORED_MINIONS, CurrentPlayerIndex); // +armor from perks
	maxArmor += maxArmor * PerkValue(PERK_GOLEM_MASTERY, CurrentPlayerIndex, 1) / 100;
	maxArmor += (maxArmor * owner.summonAcPercent) / 100 + owner.summonAc;
	maxArmor += PerkValue(PERK_ARMORED_MINIONS, CurrentPlayerIndex); // +armor from perks


	if (minAccuracyFirst == maxAccuracyFirst)
		sprintf(InfoPanelBuffer, "To Hit: %i", minAccuracyFirst);
	else 
		sprintf(InfoPanelBuffer, "To Hit: %i - %i", minAccuracyFirst, maxAccuracyFirst);
	drawLine(InfoPanelBuffer);
	sprintf(InfoPanelBuffer, "Damage: %i - %i", minMinDamageFirst, maxMaxDamageFirst);
	drawLine(InfoPanelBuffer);
	sprintf(InfoPanelBuffer, "Armor Class: %i - %i", minArmor, maxArmor);
	drawLine(InfoPanelBuffer);
	sprintf(InfoPanelBuffer, "Hit Points: %i - %i", minLife>>6, maxLife>>6);
	drawLine(InfoPanelBuffer);

}

//----- (00406C24) -------------------------------------------------------- interface
void DrawSpellBook()
{
    static constexpr int LINE_HEIGHT = 22;

	const int offsetX = SpellBookRect.Left;
	const int offsetY = SpellBookRect.Top;
 
	const int spellBookTextOffset = 66 + Screen_LeftBorder;
	
	// Собственно рисуем картинку
	Surface_DrawCEL(SpellBookRect.Left + Screen_LeftBorder, SpellBookRect.Down + Screen_TopBorder, Data_SpellBkCEL, 1, SpellBookRect.Width);
	// Прорисовка 5 кнопок перехода между страницами
	if( CurrentSpellBookPage < GUI_SpellBook_PagesAmount ){
		Surface_DrawCEL(SpellBookPageButtonsRect.Left + Screen_LeftBorder + GUI_SpellBook_PageButtonWidth * CurrentSpellBookPage, SpellBookPageButtonsRect.Down + Screen_TopBorder, Data_SpellBkBCEL, CurrentSpellBookPage + 1, GUI_SpellBook_PageButtonWidth);// Отрисовка нажатого состояния кнопки номера текущей страницы заклинания
	}
    
    int lineIndex = 0;
    
    auto drawLine = [&]( const char* text, int baseFontColor = C_0_White )
        {
            DrawTextColored( SpellBookTextRect.Left, SpellBookTextRect.Top + 11 + 8 + lineIndex*LINE_HEIGHT, SpellBookTextRect.Right, text, baseFontColor );
            ++lineIndex;
        };
    
	Player& player = Players[CurrentPlayerIndex];
	int surfaceY = 215 + offsetY; // TODO: change 215 to Screen_TopBorder + 55
	const __int64 spellMask = player.AvailableSpellMask | player.AvailableSkillMask | player.AvailableChargesMask;
	
	for( int spellOnPage = 0; spellOnPage < GUI_SpellBook_SpellsPerPageAmount; spellOnPage++, surfaceY += 43){
		const int spellIndex = LearnedSpells(CurrentSpellBookPage, spellOnPage);
		if( spellIndex == -1 || !(spellMask & (1i64 << (spellIndex - 1)) ) ){
			continue;
		}
		// Изображение заклинания
		int spellType = GetColorNumberWithSpellBook(spellIndex, 1);
		DrawSpellColor(spellType);
		DrawSpellIcon(11 + Screen_LeftBorder + offsetX, surfaceY, Data_SpellI2CEL, getSpellIcon( spellIndex ), 37, spellType);
		// Вот то что нам нужно. Отрисовка выделения
		if( spellIndex == player.CurrentSpellIndex && spellType == player.SpellType ){
			DrawSpellColor(0);
			DrawSpellIcon(11 + Screen_LeftBorder + offsetX, surfaceY, Data_SpellI2CEL, 52, 37, 0);
		}
	}
	
	if( player.CurrentSpellIndex == PS_M1_NONE ){
	    drawLine( "No selected spells" );
	}
	else {
		const auto spellIndex = player.CurrentSpellIndex;
		const auto spellType = player.SpellType;

		// 1. Spell name
		drawLine(getSpellName(spellIndex), C_3_Gold);

		// 2. Spell origin
		if (spellType == SO_0_SKILL) {
			drawLine("Skill");
		}
		else if (spellType == SO_1_SPELL) {
			int spellLevel = PlayerSpellLevel(CurrentPlayerIndex, spellIndex);
			if( spellLevel == 0 ){
				drawLine("Spell Level 0 - Unusable", C_2_Red);
			}else{
				sprintf(InfoPanelBuffer, "Spell Level %i", spellLevel);
				drawLine(InfoPanelBuffer);
			}
		}
		else if (spellType == SO_2_RELIC) {
			const int numberOfRelicWithCurrentSpell = GetNumberOfRelicWithCurrentSpell(spellIndex);
			if (numberOfRelicWithCurrentSpell == 1) {
				drawLine("1 Relict");
			}
			else {
				sprintf(InfoPanelBuffer, "%i Relicts", numberOfRelicWithCurrentSpell);
				drawLine(InfoPanelBuffer);
			}
		}
		else if (spellType == SO_3_EQUIPED_ITEM) {
			int sumCharges = SumBodySlotSpellCharges( CurrentPlayerIndex, spellIndex );
			if( sumCharges > 0 ){
				sprintf(InfoPanelBuffer, "%s charges: %i", getSpellName(spellIndex), sumCharges);
				drawLine(InfoPanelBuffer);
			}
			else {
				drawLine("Equipped item");
			}
		}

		// 3. description
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);
		if (spellIndex == PS_22_FURY && player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
			sprintf(InfoPanelBuffer, "Slows time and"); drawLine(InfoPanelBuffer);
		}
		if (spellIndex == PS_22_FURY) { // page 1
			sprintf(InfoPanelBuffer, "improves battle prowess"); drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "for a short time"); drawLine(InfoPanelBuffer);
			//sprintf(InfoPanelBuffer, " "); drawLine(InfoPanelBuffer);
			//sprintf(InfoPanelBuffer, "spell power grows"); drawLine(InfoPanelBuffer);
			//sprintf(InfoPanelBuffer, "with character level"); drawLine(InfoPanelBuffer);
			//sprintf(InfoPanelBuffer, " "); drawLine(InfoPanelBuffer); 
			sprintf(InfoPanelBuffer, " "); drawLine(InfoPanelBuffer);
			int CLVL = player.CharLevel;
			switch (player.fullClassId){
					// WARRIORS
			case PFC_WARRIOR:
				if(HasTrait(CurrentPlayerIndex, TraitId::Ranger)){
					sprintf(InfoPanelBuffer, "triple shot"); drawLine(InfoPanelBuffer);
				}
				else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister)) {
					// show nothing
				}
				else {
					sprintf(InfoPanelBuffer, "block chance: +%i", (CLVL / 10) + 5); drawLine(InfoPanelBuffer);
				}
				sprintf(InfoPanelBuffer, "chance to hit: +%i", (CLVL / 3) + 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", (CLVL / 4) + 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", 7 * CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_INQUISITOR:
				sprintf(InfoPanelBuffer, "chance to hit: +%i", (CLVL / 2) - 3); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", (CLVL / 3) - 1); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", (2 * CLVL) - 1); drawLine(InfoPanelBuffer);
				break;
			case PFC_GUARDIAN:
				sprintf(InfoPanelBuffer, "chance to hit: +%i", CLVL / 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", (4 * CLVL / 5) + 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 10); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist arrow damage: +%i", CLVL / 5); drawLine(InfoPanelBuffer);
				break;
			case PFC_TEMPLAR:
				sprintf(InfoPanelBuffer, "chance to hit: +%i", (CLVL / 5) + 10); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", (CLVL / 9) + 15); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", (2 * CLVL) + 1); drawLine(InfoPanelBuffer);
				break;
					//ARCHERS
			case PFC_ARCHER:
				sprintf(InfoPanelBuffer, "damage: +%i", CLVL + 5); drawLine(InfoPanelBuffer);
				break;
			case PFC_SCOUT:
				sprintf(InfoPanelBuffer, "damage: +%i", (3 * CLVL / 2) + 3); drawLine(InfoPanelBuffer);
				break;
			case PFC_SHARPSHOOTER:
				sprintf(InfoPanelBuffer, "chance to hit: +%i", (CLVL / 3) + 7); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", (2 * CLVL) - 2); drawLine(InfoPanelBuffer);
				break;
			case PFC_TRAPPER:
				sprintf(InfoPanelBuffer, "armor class: +%i", 2 * CLVL / 3); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", 2 * CLVL / 9); drawLine(InfoPanelBuffer);
				break;
					// SUMMONERS
			case PFC_DEMONOLOGIST:
				sprintf(InfoPanelBuffer, "spell levels: +%i", 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "summon armor: +%i", CLVL / 5); drawLine(InfoPanelBuffer);
				break;
			case PFC_NECROMANCER:
				sprintf(InfoPanelBuffer, "spell levels: +%i", 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "summon hp %%: +%i", (CLVL / 10) + 5); drawLine(InfoPanelBuffer);
				break;
			case PFC_BEASTMASTER:
				sprintf(InfoPanelBuffer, "spell levels: +%i", 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "summon damage: +%i", CLVL / 5); drawLine(InfoPanelBuffer);
				break;
					// CASTERS
			case PFC_MAGE:
				sprintf(InfoPanelBuffer, "spell levels: +%i", (CLVL / 15) + 2); drawLine(InfoPanelBuffer);
				break;
			case PFC_ELEMENTALIST:
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					sprintf(InfoPanelBuffer, "chance to hit: +%i", (CLVL / 5) + 10); drawLine(InfoPanelBuffer);
					sprintf(InfoPanelBuffer, "armor class: +%i", (CLVL / 9) + 15); drawLine(InfoPanelBuffer);
					sprintf(InfoPanelBuffer, "damage: +%i", (2 * CLVL) + 1); drawLine(InfoPanelBuffer);
				}				
				else {
					sprintf(InfoPanelBuffer, "spell levels: +%i", (CLVL / 10) + 2); drawLine(InfoPanelBuffer);
				}
				break;
			case PFC_WARLOCK:
				sprintf(InfoPanelBuffer, "spell levels: +%i", 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "mana regeneration: +%i", (CLVL / 2) + 1); drawLine(InfoPanelBuffer);
				break;
					// MONKS
			case PFC_MONK:
				sprintf(InfoPanelBuffer, "DAMAGE: +%i", 2 * CLVL); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_KENSEI:
			{ if (MaxCountOfPlayersInGame != 1)
				sprintf(InfoPanelBuffer, "DAMAGE: +%i", 3 * CLVL / 2); drawLine(InfoPanelBuffer);
			}
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_SHUGOKI:
				sprintf(InfoPanelBuffer, "DAMAGE: +%i", 2 * CLVL); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_SHINOBI:
				sprintf(InfoPanelBuffer, "DAMAGE: +%i", 5 * CLVL / 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
					// ROGUES
			case PFC_ROGUE:
				sprintf(InfoPanelBuffer, "DAMAGE: +%i", 3 * CLVL / 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", 3 * CLVL / 8); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_ASSASSIN:
				sprintf(InfoPanelBuffer, "crit chance: +%i", (CLVL / 10) + 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "crit damage: +%i", CLVL * 7); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", (CLVL / 5) + 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_IRON_MAIDEN:
				sprintf(InfoPanelBuffer, "crit chance: +%i", (CLVL / 25) + 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist melee damage: +%i", CLVL / 5 + 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 11 + 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_BOMBARDIER:
				sprintf(InfoPanelBuffer, "armor class: +%i", CLVL / 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "resist all: +%i", CLVL / 3); drawLine(InfoPanelBuffer);
				break;
					// SAVAGES
			case PFC_SAVAGE:
				sprintf(InfoPanelBuffer, "damage: +%i", 5 * CLVL / 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", CLVL / 3); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "chance to hit: +%i", CLVL / 3); drawLine(InfoPanelBuffer);
				break;
			case PFC_BERSERKER:
				sprintf(InfoPanelBuffer, "accuracy: +%i", 10 + 3 * CLVL / 5 + PerkValue(PERK_BASHING_HITS, CurrentPlayerIndex)); drawLine(InfoPanelBuffer); 
				sprintf(InfoPanelBuffer, "damage: +%i", CLVL * player.BaseStrength / 357 + CLVL / 2 + PerkValue(PERK_BLOODTHIRST, CurrentPlayerIndex)); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", 1 + CLVL / 3 + PerkValue(PERK_LIKE_A_ROCK, CurrentPlayerIndex)); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "dfe: -%i", 1 + CLVL / 5 + PerkValue(PERK_FEEL_NO_PAIN, CurrentPlayerIndex)); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "stun threshold: +%i", 1 + CLVL / 5 + PerkValue(PERK_UNBREAKABLE, CurrentPlayerIndex)); drawLine(InfoPanelBuffer);
				//sprintf(InfoPanelBuffer, "resist melee damage: +%i", 10 + (CLVL / 5)); drawLine(InfoPanelBuffer);
				//sprintf(InfoPanelBuffer, "+(%s)", "perk improvements"); drawLine(InfoPanelBuffer);
				break;
			case PFC_EXECUTIONER:
				sprintf(InfoPanelBuffer, "damage: +%i", 10 + (5 * CLVL / 2)); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", (3 * CLVL / 7) + 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "chance to hit: +%i", CLVL / 3); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "life regeneration: +%i", CLVL + 10); drawLine(InfoPanelBuffer);
				break;
					// GLADIATORS
			case PFC_THRAEX:
				sprintf(InfoPanelBuffer, "damage: +%i", 2 * CLVL); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", 2 * CLVL / 9); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "chance to hit: +%i", 5 * CLVL / 4); drawLine(InfoPanelBuffer);
				break;
			case PFC_DIMACHAERUS:
				sprintf(InfoPanelBuffer, "attack speed: -%i %s", 1, "frame"); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "chance to hit: +%i", CLVL / 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", CLVL / 2); drawLine(InfoPanelBuffer);
				break;
			case PFC_MURMILLO:
				sprintf(InfoPanelBuffer, "damage: +%i", 7 * CLVL / 4); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", 2 * CLVL / 5); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", 3 * CLVL / 4); drawLine(InfoPanelBuffer);
				break;			
			case PFC_SECUTOR:
				sprintf(InfoPanelBuffer, "spell levels: +%i", 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage: +%i", CLVL * 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", (3 * CLVL / 10) - 2); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", CLVL / 10); drawLine(InfoPanelBuffer);
				break;
			case PFC_DRUID:
				sprintf(InfoPanelBuffer, "damage: +%i", 5 + CLVL * 3); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "damage from enemies: -%i", 3 * CLVL / 21 + 1); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "chance to hit: +%i", CLVL / 2 + 10); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "armor class: +%i", 5 + CLVL / 10); drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "stun threshold: +%i", 3 + CLVL / 15); drawLine(InfoPanelBuffer);
				break;
			}
			// Fury duration:
			int additionalFuryDuration = player.effectFlag[EA_FURY_DURATION] ? (player.CharLevel / 6 + 10) : 0;
			int durf = 30 + (CLVL / 5) + /*10 * missile.SpellLevel*/ + additionalFuryDuration + PerkValue(PERK_RAMPAGE, CurrentPlayerIndex);
			sprintf(InfoPanelBuffer, " "); drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "duration: %i seconds", durf); drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_7_TOWN_PORTAL) {
			sprintf(InfoPanelBuffer, "creates a magic portal");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that teleports caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to town and back");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_33_TELEKINES) {
			sprintf(InfoPanelBuffer, "caster uses kinetic powers");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to manipulate objects");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from a distance");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_16_REFLECT) {
			sprintf(InfoPanelBuffer, "gives invulnerability");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to monster melee attacks");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_23_TELEPORT) {
			sprintf(InfoPanelBuffer, "instantly moves caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to selected location");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_11_MANA_SHIELD) {
			sprintf(InfoPanelBuffer, "damage is substracted");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from mana instead of life");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_2_HEALING) {
			drawLine ("partially restores life");
			drawLine("");			
		}
		else if (spellIndex == PS_34_HEAL_OTHER) {
			sprintf(InfoPanelBuffer, "caster restores life");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "for selected player");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_1_FIREBOLT) { // page 2
			sprintf(InfoPanelBuffer, "casts a fiery missile");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_20_INCINERATE) {
			sprintf(InfoPanelBuffer, "breathe a stream of flame");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that scorches enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_12_FIREBLAST) {
			sprintf(InfoPanelBuffer, "casts a powerful fire ball");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that explodes on hit");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "and damages nearby enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_6_FIRE_WALL) {
			sprintf(InfoPanelBuffer, "creates a wall of flames");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that prevents enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "without fire immunity");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from crossing it");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_19_FLAME_RING) {
			sprintf(InfoPanelBuffer, "creates a burning circle");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that prevents enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "without fire immunity");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from crossing it");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_41_FIERY_NOVA) {
			sprintf(InfoPanelBuffer, "creates expanding circle");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of fireballs around caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_13_HYDRA) {
			sprintf(InfoPanelBuffer, "summons a multi-headed");
			drawLine(InfoPanelBuffer);
			if (player.fullClassId == PFC_ELEMENTALIST) {
				sprintf(InfoPanelBuffer, "beast to spit ice bolts");
				drawLine(InfoPanelBuffer);
			}
			else if (HasTrait(CurrentPlayerIndex, TraitId::Hydramancer)) {
				sprintf(InfoPanelBuffer, "beast that throws fire,");
				drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "electric, arcane");
				drawLine(InfoPanelBuffer);
				sprintf(InfoPanelBuffer, "or acid bolts");
				drawLine(InfoPanelBuffer);
			}
			else {
			sprintf(InfoPanelBuffer, "beast that throws fire");
			drawLine(InfoPanelBuffer);
			}
			sprintf(InfoPanelBuffer, "at non-immune enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "Max amount: %i", MaxCountOfHydrasForPlayer(CurrentPlayerIndex));
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_40_LIGHTING_WALL) { // page 3
			sprintf(InfoPanelBuffer, "creates electric wall");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that prevents enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "without lightning immunity");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from crossing it");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_17_LIGHTING_RING) {
			sprintf(InfoPanelBuffer, "creates electric circle");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that prevents enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "without lightning immunity");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "from crossing it");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_30_CHARGED_BOLT) {
			sprintf(InfoPanelBuffer, "creates multiple bolts");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of electrical energy");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_3_LIGHTNING) {
			sprintf(InfoPanelBuffer, "sends a lightning torrent");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that strikes targets");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_14_BALL_LIGHTNING) {
			sprintf(InfoPanelBuffer, "launches massive charged");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "ball of electricity");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_18_LIGHTNING_NOVA) {
			sprintf(InfoPanelBuffer, "casts expanding ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of electric charges");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_35_ARCANE_STAR) {
			sprintf(InfoPanelBuffer, "creates a magic star");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that inflicts arcane damage");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_4_FLASH) {
			sprintf(InfoPanelBuffer, "creates a magical ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of energy around caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to damage nearby targets");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_43_ARCANE_NOVA) {
			sprintf(InfoPanelBuffer, "creates an expanding ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of stars around caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_31_HOLY_BOLT) { // page 4
			sprintf(InfoPanelBuffer, "a bolt of holy energy");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that damages undead");
			drawLine(InfoPanelBuffer);
			if (GameMode != GM_CLASSIC) {
				sprintf(InfoPanelBuffer, "and knocks them back");
				drawLine(InfoPanelBuffer);
			}
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_39_HOLY_NOVA) {
			sprintf(InfoPanelBuffer, "creates a massive ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of expanding holy bolts");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy undead");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_8_STONE_CURSE) {
			sprintf(InfoPanelBuffer, "turns non-immune enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to stone form");
			drawLine(InfoPanelBuffer);
			if (GameMode != GM_CLASSIC) {
				sprintf(InfoPanelBuffer, "making them invulnerable");
				drawLine(InfoPanelBuffer);			
			}
			sprintf(InfoPanelBuffer, "for some time");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_36_BONE_SPIRIT) {
			sprintf(InfoPanelBuffer, "releases a restless soul");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that devours the living");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "for a percentage of life");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_15_FORCE_WAVE) {
			sprintf(InfoPanelBuffer, "sends a moving wall");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of force energy");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to knock back enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "(does physical damage)");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_21_GOLEM) {
			WriteSummonSpellBookInfo(PS_21_GOLEM, LINE_HEIGHT, lineIndex);
		}
		else if (spellIndex == PS_29_ELEMENTAL) {
			sprintf(InfoPanelBuffer, "releases a chaos being");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "that seeks and destroys");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "live creatures");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "(does physical damage)");
			drawLine(InfoPanelBuffer);
			if (GameMode != GM_CLASSIC) {
				sprintf(InfoPanelBuffer, "(knocks target back)");
				drawLine(InfoPanelBuffer);
			}
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_56_ICE_BOLT) { // page 5
			sprintf(InfoPanelBuffer, "casts a shard of ice");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_57_FREEZING_BALL) {
			sprintf(InfoPanelBuffer, "launches a powerful");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "damaging ball of ice");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_58_FROST_NOVA) {
			sprintf(InfoPanelBuffer, "creates an expanding ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of ice around caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_59_RANCID_BOLT) {
			sprintf(InfoPanelBuffer, "casts an acid bolt");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to slowly burn enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_60_TOXIC_BALL) {
			sprintf(InfoPanelBuffer, "launches a powerful");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "damaging ball of acid");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_61_ACID_NOVA) {
			sprintf(InfoPanelBuffer, "creates an expanding ring");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "of acid around caster");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, "to destroy enemies");
			drawLine(InfoPanelBuffer);
			sprintf(InfoPanelBuffer, " ");
			drawLine(InfoPanelBuffer);
		}
		else if (spellIndex == PS_52_LESSER_SUMMON) {
			WriteSummonSpellBookInfo(PS_52_LESSER_SUMMON, LINE_HEIGHT, lineIndex);
		}
		else if (spellIndex == PS_53_COMMON_SUMMON) {
			WriteSummonSpellBookInfo(PS_53_COMMON_SUMMON, LINE_HEIGHT, lineIndex);
		}
		else if (spellIndex == PS_54_GREATER_SUMMON) {
			WriteSummonSpellBookInfo(PS_54_GREATER_SUMMON, LINE_HEIGHT, lineIndex);
		}
		else if (spellIndex == PS_10_PHASING) {
			drawLine("quickly teleports caster");
			drawLine("to nearby random location");
		}
		else if(spellIndex == PS_42_WARP) {
			drawLine("teleports caster");
			drawLine("to nearest stairs");
		}
		else if (spellIndex == PS_24_APOCALYPSE) {
			drawLine("engulfs visible enemies");
			drawLine("in infernal flames");
			drawLine("obliterating everything");
			drawLine("(does physical damage)");
		}
		else if (spellIndex == PS_25_ETHEREAL) {
			drawLine("makes caster invulnerable");
			drawLine("to all magical damage");
			drawLine("for 20 seconds");
		}
		else if (spellIndex == PS_26_ITEM_REPAIR) {
			drawLine("fully repairs item");
			drawLine("but reduces maximum");
			drawLine("durability by 1 point");
		}
		else if (spellIndex == PS_27_STAFF_RECHARGE) {
			drawLine("restores spell charges");
			drawLine("at the cost of item's");			
			drawLine("current durability");
		}
		else if (spellIndex == PS_28_TRAP_DISARM) {
			drawLine("disarms trapped objects");			
		}
		else if (spellIndex == PS_37_MANA_RECHARGE) { 
			drawLine("restores partial mana");	
			drawLine("");
		}
		else if (spellIndex == PS_38_MAGI) {
			drawLine("fully restores mana");			
		}
		//else if (spellIndex == PS_42_WARP) { // somehow this didn't work !!
		//	drawLine("quickly teleport");
		//	drawLine("to nearest stairs");
		//}
		else if (spellIndex == PS_5_IDENTIFY) {
			drawLine("identify selected item");			
		}
		else if (spellIndex == PS_9_INFRAVISION) {
			drawLine("monsters outside of");
			drawLine("caster's line of sight");
			drawLine("and light radius");
			drawLine("are highlighted red");
		}

        // 4. damage, healing or kind of
        int minDamage = 0;
        int maxDamage = 0;
        GetDamageOfPlayerSpell(CurrentPlayerIndex, spellIndex, PlayerSpellLevel(CurrentPlayerIndex, spellIndex), SpellElement(player, spellIndex), &minDamage, &maxDamage);
        
        if( spellIndex == PS_36_BONE_SPIRIT ){
            const int atrophyLevel = PerkValue(PERK_ATROPHY, CurrentPlayerIndex);
            int numerator;
            int denominator;
			if (atrophyLevel >= 3) {
				numerator = 1;
				denominator = 5;
			}else if( atrophyLevel == 2 ){
                numerator =     1;
                denominator =   6;
            }else if( atrophyLevel == 1 ){
                numerator =     1;
                denominator =   7;
            }else{
                numerator =     1;
                denominator =   8;
            }
			denominator += Difficulty;
            sprintf( InfoPanelBuffer, "Damage: %i/%i of target's hp", numerator, denominator );
            drawLine( InfoPanelBuffer );
        }else if( spellIndex == PS_2_HEALING || spellIndex == PS_34_HEAL_OTHER ){   //
            const POINT p = InfoPanelManaHeal( spellIndex );
            sprintf( InfoPanelBuffer, "Healing: %i-%i", p.x, p.y ); // TODO: write new function. And use it here and in healing code
            drawLine( InfoPanelBuffer );
		}else if (spellIndex == PS_37_MANA_RECHARGE) {   //  Реликт маны
			const POINT p = InfoPanelManaHeal(spellIndex);
			sprintf(InfoPanelBuffer, "mana restore: %i-%i",p.x, p.y );
			drawLine(InfoPanelBuffer);
        }else if( spellIndex == PS_11_MANA_SHIELD ){
            sprintf( InfoPanelBuffer, "Damage Received: %i%%", minDamage );
            drawLine( InfoPanelBuffer );
        }else if (spellIndex == PS_16_REFLECT) {
			int reflect_layers = player.BaseVitality / 50 + 1; // that's basic level of Reflect layers
			if (HasTrait(CurrentPlayerIndex, TraitId::Armadillo)) {
				reflect_layers += player.CharLevel / 13 + 2;
			}
			reflect_layers += PerkValue(SYNERGY_IRON_BULWARK, CurrentPlayerIndex);
			if( reflect_layers == 1 ){
                drawLine( "Absorbs 1 hit" );
            }else{
                sprintf( InfoPanelBuffer, "Absorbs %i hits", reflect_layers);
                drawLine( InfoPanelBuffer );
            }
		}else if (spellIndex == PS_6_FIRE_WALL || spellIndex == PS_19_FLAME_RING) {
			sprintf(InfoPanelBuffer, "Damage per second: %i", minDamage);
			drawLine(InfoPanelBuffer, 4);
		}else if (spellIndex == PS_17_LIGHTING_RING || spellIndex == PS_40_LIGHTING_WALL) {
			sprintf(InfoPanelBuffer, "Damage per second: %i", minDamage);
			drawLine(InfoPanelBuffer, 1);
		}else if (spellIndex == PS_4_FLASH) {
			sprintf(InfoPanelBuffer, "Damage per second: %i", minDamage);
			drawLine(InfoPanelBuffer, 8);
		}else if (spellIndex == PS_20_INCINERATE) {
			sprintf(InfoPanelBuffer, "Damage per second: %i", minDamage);
			drawLine(InfoPanelBuffer, 4);
		}else if( minDamage != -1 && !( minDamage == 0 && maxDamage == 0 ) && !is(spellIndex, PS_21_GOLEM, PS_52_LESSER_SUMMON, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON)){
            sprintf( InfoPanelBuffer, "Damage: %i-%i", minDamage, maxDamage );
			int le_color;
			//ELEMENTAL_TYPE damageType;
			switch (spellIndex) {
			case PS_13_HYDRA: player.fullClassId == PFC_ELEMENTALIST ? le_color = 7 /*grey*/ : le_color = 4/*orng*/; break; 
			case PS_12_FIREBLAST:
			//case PS_19_FLAME_RING:
			case PS_1_FIREBOLT:
			//case PS_20_INCINERATE:
			case PS_41_FIERY_NOVA:
			//case PS_6_FIRE_WALL:
			//case PS_45_RING_OF_FIRE:	
										le_color = 4; break; // orange
			//case PS_17_LIGHTING_RING:
			case PS_14_BALL_LIGHTNING:
			case PS_18_LIGHTNING_NOVA:
			case PS_30_CHARGED_BOLT:
			case PS_3_LIGHTNING:
			//case PS_40_LIGHTING_WALL:	
										le_color = 1; break; // blue
			case PS_31_HOLY_BOLT:
			case PS_39_HOLY_NOVA:		le_color = 6; break; // goldish
			case PS_43_ARCANE_NOVA:
			case PS_35_ARCANE_STAR:
			//case PS_4_FLASH:			
										le_color = 8; break; // brownish
			case PS_56_ICE_BOLT:
			case PS_57_FREEZING_BALL:
			case PS_58_FROST_NOVA:		le_color = 7; break; // grey
			case PS_59_RANCID_BOLT:
			case PS_60_TOXIC_BALL:
			case PS_61_ACID_NOVA:		le_color = 5; break; // yellow
			default:					le_color = 0; break; // white - physical
			}

			if( HasTrait(CurrentPlayerIndex, TraitId::Hydramancer) && spellIndex == PS_13_HYDRA ){
				if (PlayerSpellLevel(CurrentPlayerIndex, PS_1_FIREBOLT) > 0) {
					GetDamageOfPlayerSpell(CurrentPlayerIndex, spellIndex, PlayerSpellLevel(CurrentPlayerIndex, spellIndex), ET_1_FIRE, &minDamage, &maxDamage);
					sprintf(InfoPanelBuffer, "Fire Damage: %i-%i", minDamage, maxDamage);
					drawLine(InfoPanelBuffer, C_4_Orange);
				}

				if (PlayerSpellLevel(CurrentPlayerIndex, PS_30_CHARGED_BOLT) > 0) {
					GetDamageOfPlayerSpell(CurrentPlayerIndex, spellIndex, PlayerSpellLevel(CurrentPlayerIndex, spellIndex), ET_2_LIGHTNING, &minDamage, &maxDamage);
					sprintf(InfoPanelBuffer, "Lightning Damage: %i-%i", minDamage, maxDamage);
					drawLine(InfoPanelBuffer, C_1_Blue);
				}

				if (PlayerSpellLevel(CurrentPlayerIndex, PS_35_ARCANE_STAR) > 0) {
					GetDamageOfPlayerSpell(CurrentPlayerIndex, spellIndex, PlayerSpellLevel(CurrentPlayerIndex, spellIndex), ET_3_ARCAN, &minDamage, &maxDamage);
					sprintf(InfoPanelBuffer, "Arcane Damage: %i-%i", minDamage, maxDamage);
					drawLine(InfoPanelBuffer, C_8_Pink);
				}

				if (PlayerSpellLevel(CurrentPlayerIndex, PS_59_RANCID_BOLT) > 0) {
					GetDamageOfPlayerSpell(CurrentPlayerIndex, spellIndex, PlayerSpellLevel(CurrentPlayerIndex, spellIndex), ET_4_ACID, &minDamage, &maxDamage);
					sprintf(InfoPanelBuffer, "Acid Damage: %i-%i", minDamage, maxDamage);
					drawLine(InfoPanelBuffer, C_5_Yellow);
				}
			}
			else {
				drawLine(InfoPanelBuffer, le_color);
			}
        }

		// empty line
		sprintf(InfoPanelBuffer, " ");
		drawLine(InfoPanelBuffer);

		// 5. Range
		if (spellIndex == PS_20_INCINERATE) {
			drawLine("range: 3 tiles");
			drawLine("");
		}
		else if (is(spellIndex, PS_23_TELEPORT, PS_33_TELEKINES)) {
			drawLine("range: 10 tiles");
			drawLine("");
		}

		

        // 7. Mana cost
        if( spellType == SO_1_SPELL ){
            int manaCost = CalculateManaRequiredToCastSpell(CurrentPlayerIndex, spellIndex) / 64;
            sprintf( InfoPanelBuffer, "Mana cost: %i", manaCost ); // TODO: red or blue depending on bonuses and penalties
            drawLine( InfoPanelBuffer );
        }

		const auto [cdType, cooldown] = GetSpellCooldownValue(CurrentPlayerIndex, spellIndex);
		if (cooldown > 0) {
			if (cooldown == 1) {				
				sprintf(InfoPanelBuffer, "Cooldown: 1 second");
			}
			else {				
				sprintf(InfoPanelBuffer, "Cooldown: %i seconds", cooldown);
			}
			drawLine("");
			drawLine(InfoPanelBuffer);
		}
        
    }
}

//----- (00406F90) -------------------------------------------------------- interface
void BookPanel_CkeckLButtonClick()
{
	Player& player = Players[CurrentPlayerIndex];

    if( CursorIntoDisplayObject(SpellBookPageButtonsRect) ){
        CurrentSpellBookPage = (CursorX - SpellBookPageButtonsRect.Left) / GUI_SpellBook_PageButtonWidth;
        LimitToRange( CurrentSpellBookPage, 0, GUI_SpellBook_PagesAmount - 1 );
		PlayGlobalSound(S_75_I_TITLEMOV);
        return;
    }
    
    if( CursorIntoDisplayObject(SpellBookSpellButtonsRect) ){
        i64 spellMask;
        unsigned& maskHi = *((unsigned*)&spellMask + 1);
        unsigned& maskLo = *(unsigned*)&spellMask;
        int spellBookLine = (CursorY - SpellBookSpellButtonsRect.Top) / GUI_SpellBook_SpellButtonHeight;
        int spellNumber = LearnedSpells(CurrentSpellBookPage, spellBookLine);
		spellMask = player.AvailableSkillMask | player.AvailableChargesMask | player.AvailableSpellMask;
        if( spellNumber == -1 ){
            return;
        }
        __int64 spellSelector = 1i64 << (spellNumber - 1);
        if( !( spellSelector & spellMask )){
            return;
        }
        int spellType = SO_1_SPELL;
        if( player.AvailableChargesMask & spellSelector ){
            spellType = SO_3_EQUIPED_ITEM;
        }
        if( player.AvailableSkillMask & spellSelector ){
            spellType = SO_0_SKILL;
        }
        player.CurrentSpellIndex = spellNumber;
        player.SpellType = spellType;
		PlayGlobalSound(S_75_I_TITLEMOV);
    }
}

void ToggleSpellBook()
{
    // Conflicts with
    {
        CloseInventoryPanel();
        IsPerksPanelVisible = false;
        IsInfoWindowVisible = false;
    }

    IsSpellBookVisible = !IsSpellBookVisible;
	PlayGlobalSound(75);
}
