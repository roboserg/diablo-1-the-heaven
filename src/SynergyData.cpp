#include "stdafx.h"

Perk SynergyPerks[] = {

	{ SYNERGY_INTREPIDITY, {"    life increased by %i"}, "Intrepidity", {// expanded
		{  4, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  2, PERK_FORTITUDE,  1}/*perk*/, {}/*spell*/, 0,0,0, 20/*sdmv*/,  30 },
		{ 10, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  5, PERK_FORTITUDE,  5}/*perk*/, {}/*spell*/, 0,0,0, 40/*sdmv*/,  60 },
		{ 25, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 10, PERK_FORTITUDE, 10}/*perk*/, {}/*spell*/, 0,0,0, 80/*sdmv*/, 100 },
		{ 40, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 15, PERK_FORTITUDE, 15}/*perk*/, {}/*spell*/, 0,0,0,120/*sdmv*/, 150 },
		{ 55, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 20, PERK_FORTITUDE, 20}/*perk*/, {}/*spell*/, 0,0,0,160/*sdmv*/, 200 },
	} },

	{ SYNERGY_SAPIENCE, {"    mana increased by %i"}, "Sapience", {// expanded
		{  4, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  2, PERK_FOCUS,  1}/*perk*/, {}/*spell*/, 0,0, 20,0/*sdmv*/,  30 },
		{ 10, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  5, PERK_FOCUS,  5}/*perk*/, {}/*spell*/, 0,0, 40,0/*sdmv*/,  60 },
		{ 25, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 10, PERK_FOCUS, 10}/*perk*/, {}/*spell*/, 0,0, 80,0/*sdmv*/, 100 },
		{ 40, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 15, PERK_FOCUS, 15}/*perk*/, {}/*spell*/, 0,0,120,0/*sdmv*/, 150 },
		{ 55, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 20, PERK_FOCUS, 20}/*perk*/, {}/*spell*/, 0,0,160,0/*sdmv*/, 200 },
	} },

	{ SYNERGY_WILL_TO_ENDURE, {"affixes that increase::Vitality and Magic::    add +%i%% to life & mana"}, "Will to Endure", {// expanded
		{ 11, IPCM_ALL_CLASSES, {PERK_ENDURANCE,  5, PERK_WILLPOWER,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  50 },
		{ 21, IPCM_ALL_CLASSES, {PERK_ENDURANCE, 10, PERK_WILLPOWER, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 100 },
	} },

	{ SYNERGY_CYCLE_OF_FURY, {"Fury cooldown gets::    decreased by %i seconds"}, "Cycle of Fury", {// ok
		{ 14, IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_ARCHER | IPCM_SCOUT | IPCM_SHARPSHOOTER | IPCM_KENSEI | IPCM_SHUGOKI | IPCM_ANY_SAVAGE & ~IPCM_BERSERKER, {PERK_RAMPAGE, 3, PERK_REHABILITATION, 3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  15 },			
	} },

	{ SYNERGY_SPELL_POWER, {"damage of all spell types::    gets increased by %i%%"}, "Spell Power", {// ok
		{  6, IPCM_MAGE | IPCM_WARLOCK | IPCM_ROGUE | IPCM_ASSASSIN, {PERK_GAIN_MAGIC,  2, PERK_MASTER_CASTER,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 13, IPCM_MAGE | IPCM_WARLOCK | IPCM_ROGUE | IPCM_ASSASSIN, {PERK_GAIN_MAGIC,  5, PERK_MASTER_CASTER,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 21, IPCM_MAGE | IPCM_WARLOCK | IPCM_ROGUE | IPCM_ASSASSIN, {PERK_GAIN_MAGIC,  7, PERK_MASTER_CASTER,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 28, IPCM_MAGE | IPCM_WARLOCK | IPCM_ROGUE | IPCM_ASSASSIN, {PERK_GAIN_MAGIC, 10, PERK_MASTER_CASTER, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
	} },

	{ SYNERGY_MINION_OFFENSE, {"damage of all minions::    gets increased by %i points"}, "Minion Offense", {//?
		{  7, IPCM_ANY_SUMMONER, {PERK_RESISTANCE,  2, PERK_GAIN_STRENGTH, 1}/*perk*/, {}/*spell*/, 0,0, 50,0/*sdmv*/,  5 },
		{ 13, IPCM_ANY_SUMMONER, {PERK_RESISTANCE,  4, PERK_GAIN_STRENGTH, 2}/*perk*/, {}/*spell*/, 0,0, 80,0/*sdmv*/, 10 },
		{ 19, IPCM_ANY_SUMMONER, {PERK_RESISTANCE,  6, PERK_GAIN_STRENGTH, 3}/*perk*/, {}/*spell*/, 0,0,110,0/*sdmv*/, 15 },
		{ 25, IPCM_ANY_SUMMONER, {PERK_RESISTANCE,  8, PERK_GAIN_STRENGTH, 4}/*perk*/, {}/*spell*/, 0,0,140,0/*sdmv*/, 20 },
		{ 31, IPCM_ANY_SUMMONER, {PERK_RESISTANCE, 10, PERK_GAIN_STRENGTH, 5}/*perk*/, {}/*spell*/, 0,0,170,0/*sdmv*/, 25 },
	} },

	{ SYNERGY_IMPEDANCE, {"    resist all: +%i","    maximum resistance: +%i%%"}, "Impedance", {//?
		{ 12, IPCM_ALL_CLASSES, {PERK_RESISTANCE,  5, PERK_STEEL_SKIN,  5}/*perk*/, {}/*spell*/, 0,0,0,50/*sdmv*/, 3, 3 },
		{ 22, IPCM_ALL_CLASSES, {PERK_RESISTANCE, 10, PERK_STEEL_SKIN, 10}/*perk*/, {}/*spell*/, 0,0,0,75/*sdmv*/, 5, 5 },
	} },
	
	{ SYNERGY_STAR_ALIGNMENT, {"    all attributes: +%i points"}, "Star Alignment", {// expanded
		{ 11, IPCM_ALL_CLASSES, {PERK_GAIN_STRENGTH,  5, PERK_GAIN_MAGIC,  5, PERK_GAIN_DEXTERITY,  5, PERK_GAIN_VITALITY,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 22, IPCM_ALL_CLASSES, {PERK_GAIN_STRENGTH, 10, PERK_GAIN_MAGIC, 10, PERK_GAIN_DEXTERITY, 10, PERK_GAIN_VITALITY, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_BLOOD_THIEVERY, {"hitting monsters in melee::additionally steals::    %i points of life","    and %i points of mana"}, "Blood Thievery", {//ok
		{ 10, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_AUGMENTED_HEALING, 1, PERK_REHABILITATION,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2, 1 },
		{ 17, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_AUGMENTED_HEALING, 2, PERK_REHABILITATION,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4, 2 },
		{ 24, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_AUGMENTED_HEALING, 3, PERK_REHABILITATION,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 6, 3 },
		{ 31, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_AUGMENTED_HEALING, 4, PERK_REHABILITATION,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 7, 4 },
		{ 38, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_AUGMENTED_HEALING, 5, PERK_REHABILITATION, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 8, 5 },
	} },

	{ SYNERGY_IMPROVED_EVASION, {"chances of getting hit::in melee combat::    get decreased by %i%%"}, "Improved Evasion", {//ok
		{ 20, IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ANY_SAVAGE,   {PERK_DODGER,  5, PERK_TOUGHNESS,  5, PERK_IRONCLAD,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		{ 40, IPCM_GUARDIAN,										{PERK_DODGER, 10, PERK_TOUGHNESS, 10, PERK_IRONCLAD, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
	} },

	{ SYNERGY_EXTRA_AVOIDANCE, {"chances of getting hit::in ranged combat::    get decreased by %i%%"}, "Extra Avoidance", {//ok
		{ 20, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ASSASSIN | IPCM_ANY_MONK & ~IPCM_SHUGOKI | IPCM_DIMACHAERUS, {PERK_DODGER, 5, PERK_STEEL_SKIN, 5, PERK_IRONCLAD, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
	} },

	{ SYNERGY_RESIST_CHAOS, {"damage from physical spells::    gets reduced by %i%%"}, "Resist Chaos", {//ok
		{ 15, IPCM_ALL_CLASSES, {PERK_RESISTANCE,  5, PERK_TOUGHNESS, 5, PERK_IRONCLAD,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 30, IPCM_ALL_CLASSES, {PERK_RESISTANCE, 10, PERK_TOUGHNESS, 5, PERK_IRONCLAD, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
	} },

	{ SYNERGY_ENERGY_FIELD, {"excessive mana shield::damage gets reduced::    additionally by %i%%"}, "Energy Field", {//ok
		{ 30, IPCM_ANY_MAGE, {PERK_MIND_OVER_MATTER, 5, PERK_IRONCLAD,  5}/*perk*/, { PS_11_MANA_SHIELD, 8 }/*spell*/, 0,0,0,0/*sdmv*/, 5 },
		{ 44, IPCM_WARLOCK,	 {PERK_MIND_OVER_MATTER, 7, PERK_IRONCLAD, 10}/*perk*/, { PS_11_MANA_SHIELD, 14 }/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_INEXORABILITY, {"increases stats (based on clvl)::    accuracy: +clvl/%i","    melee damage: +clvl/%i","    armor class: +clvl/%i"}, "Inexorability", {//ok
		{ 10, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_STRONG_ARM, 5, PERK_EAGLE_EYE, 5}/*perk*/, {}/*spell*/, 60,0,0,0/*sdmv*/, 7, 6, 7 },
		{ 20, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_STRONG_ARM, 10, PERK_EAGLE_EYE, 10}/*perk*/, {}/*spell*/, 110,0,0,0/*sdmv*/, 6, 5, 6 },
		{ 35, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_STRONG_ARM, 15, PERK_EAGLE_EYE, 15}/*perk*/, {}/*spell*/, 160,0,0,0/*sdmv*/, 5, 4, 5 },
		{ 45, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_STRONG_ARM, 20, PERK_EAGLE_EYE, 20}/*perk*/, {}/*spell*/, 250,0,0,0/*sdmv*/, 4, 3, 4 },
		{ 60, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK & ~IPCM_SHINOBI | IPCM_ANY_SAVAGE, {PERK_STRONG_ARM, 30, PERK_EAGLE_EYE, 25}/*perk*/, {}/*spell*/, 350,0,0,0/*sdmv*/, 3, 2, 3 },
		{ 80, IPCM_SAVAGE | IPCM_EXECUTIONER, {PERK_STRONG_ARM, 40, PERK_EAGLE_EYE, 30}/*perk*/, {}/*spell*/, 600,0,0,0/*sdmv*/, 2, 1, 2 },
	} },

	{ SYNERGY_IRON_BULWARK, {"    +%i layers of reflect"}, "Iron Bulwark", {//ok
		{ 11, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_UNSTOPPABLE,  5, PERK_CONSERVATOR, 4}/*perk*/, {}/*spell*/, 0,0,0, 60/*sdmv*/, 1 },
		{ 21, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_UNSTOPPABLE, 10, PERK_CONSERVATOR, 8}/*perk*/, {}/*spell*/, 0,0,0,100/*sdmv*/, 2 },
	} },

	{ SYNERGY_ONSET, {"    accuracy: +%i","    ranged attack damage: +%i"}, "Onset", {//ok
		{ 10, IPCM_TEMPLAR | IPCM_ARCHER | IPCM_SHARPSHOOTER | IPCM_ANY_MONK | IPCM_ASSASSIN | IPCM_IRON_MAIDEN | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_ANY_GLADIATOR, {PERK_EAGLE_EYE, 5, PERK_OVERDRAW, 5}/*perk*/, {}/*spell*/, 40,75,0,0/*sdmv*/, 5, 7 },
		{ 20, IPCM_TEMPLAR | IPCM_ARCHER | IPCM_SHARPSHOOTER | IPCM_ANY_MONK | IPCM_ASSASSIN | IPCM_IRON_MAIDEN | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_ANY_GLADIATOR, {PERK_EAGLE_EYE, 10, PERK_OVERDRAW, 10}/*perk*/, {}/*spell*/, 60,125,0,0/*sdmv*/, 10, 14 },
		{ 35, IPCM_SHARPSHOOTER | IPCM_KENSEI | IPCM_SHINOBI | IPCM_ASSASSIN | IPCM_SAVAGE | IPCM_BERSERKER | /*IPCM_EXECUTIONER |*/ IPCM_THRAEX, {PERK_EAGLE_EYE, 15, PERK_OVERDRAW, 15}/*perk*/, {}/*spell*/, 80,175,0,0/*sdmv*/, 20, 25},
		{ 50, IPCM_SHARPSHOOTER | IPCM_ASSASSIN, {PERK_EAGLE_EYE, 20, PERK_OVERDRAW, 20}/*perk*/, {}/*spell*/, 100,250,0,0/*sdmv*/, 30, 50},
		{ 65, IPCM_SHARPSHOOTER, {PERK_EAGLE_EYE, 25, PERK_OVERDRAW, 25}/*perk*/, {}/*spell*/, 150,300,0,0/*sdmv*/, 40, 75},
		{ 80, IPCM_SHARPSHOOTER, {PERK_EAGLE_EYE, 30, PERK_OVERDRAW, 30}/*perk*/, {}/*spell*/, 200,400,0,0/*sdmv*/, 50, 100},
	} },
	//needs revision to description
	{ SYNERGY_HARDENED_STEEL, {"maximum melee and arrow::damage resistance::    increased by %i%%,","minimum chance to be hit::with melee hits and arrows::    gets increased by %i%%"}, "Hardened Steel", {//ok
		{ 35, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_TOUGHNESS, 5, PERK_STEEL_SKIN, 5, PERK_ADVANCED_USER, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 5, 2 },
	} },

	{ SYNERGY_STOICISM, {"knockback resistance chance::    gets increased by %i%%"}, "Stoicism", {//ok
		{ 30, IPCM_DIMACHAERUS, {PERK_STONEWALL, 3, PERK_ACROBATICS, 2}/*perk*/, {}/*spell*/, 0,0,0,100/*sdmv*/, 10 },
	} },

	{ SYNERGY_RESUMPTION, {"reflect cooldown::    reduced by %i seconds"}, "Resumption", {//ok
		{ 14, IPCM_ALL_CLASSES, {PERK_REHABILITATION, 3, PERK_TOUGHNESS, 3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 22, IPCM_ALL_CLASSES, {PERK_REHABILITATION, 6, PERK_TOUGHNESS, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
	} },

	{ SYNERGY_REBOUND, {"reflect arrows back::at attacking monsters,::    rebound chance: %i%%,","    rebound damage: +%i%%","    armor reduced by %i"}, "Rebound", {//ok
		{  5, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN,  2, PERK_ANTISOCIAL,  2}/*perk*/, {}/*spell*/,  40, 40,0,0/*sdmv*/, 30,  150,  4},
		{  9, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN,  4, PERK_ANTISOCIAL,  4}/*perk*/, {}/*spell*/,  55, 55,0,0/*sdmv*/, 35,  300,  8},
		{ 13, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN,  6, PERK_ANTISOCIAL,  6}/*perk*/, {}/*spell*/,  70, 70,0,0/*sdmv*/, 40,  450, 12},
		{ 17, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN,  8, PERK_ANTISOCIAL,  8}/*perk*/, {}/*spell*/,  85, 85,0,0/*sdmv*/, 45,  600, 16},
		{ 21, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 10, PERK_ANTISOCIAL, 10}/*perk*/, {}/*spell*/, 100,100,0,0/*sdmv*/, 50,  750, 20},
		{ 26, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 11, PERK_ANTISOCIAL, 12}/*perk*/, {}/*spell*/, 125,150,0,0/*sdmv*/, 55,  900, 24},
		{ 32, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 12, PERK_ANTISOCIAL, 14}/*perk*/, {}/*spell*/, 150,200,0,0/*sdmv*/, 60, 1050, 28},
		{ 39, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 13, PERK_ANTISOCIAL, 16}/*perk*/, {}/*spell*/, 175,250,0,0/*sdmv*/, 65, 1200, 32},
		{ 47, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 14, PERK_ANTISOCIAL, 18}/*perk*/, {}/*spell*/, 200,300,0,0/*sdmv*/, 70, 1350, 36},
		{ 56, IPCM_IRON_MAIDEN, {PERK_STEEL_SKIN, 15, PERK_ANTISOCIAL, 20}/*perk*/, {}/*spell*/, 250,400,0,0/*sdmv*/, 75, 1500, 40},
	} },

	{ SYNERGY_AGGRESSION, {"maximum and current::    block chance: -%i%%","    melee base damage: +%i%%"}, "Aggression", {//ok
		{ 12, IPCM_TEMPLAR | IPCM_MURMILLO, {PERK_TUCK_TAILS, 1, PERK_STRONG_ARM,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10, 15 },
		{ 18, IPCM_TEMPLAR | IPCM_MURMILLO, {PERK_TUCK_TAILS, 2, PERK_STRONG_ARM,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15, 21 },
		{ 24, IPCM_TEMPLAR | IPCM_MURMILLO, {PERK_TUCK_TAILS, 3, PERK_STRONG_ARM, 11}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25, 28 },
		{ 30, IPCM_TEMPLAR | IPCM_MURMILLO, {PERK_TUCK_TAILS, 4, PERK_STRONG_ARM, 15}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35, 35 },
	} },

	{ SYNERGY_HEAVY_HITTER, {"Your attacks may lack finesse::but you do more melee damage::    accuracy: -%i","    melee damage: +%i"}, "Heavy Hitter", {//ok
		{  4, IPCM_GUARDIAN | IPCM_IRON_MAIDEN | IPCM_EXECUTIONER | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_MURMILLO, {PERK_TOUGHNESS,  2, PERK_ENDURANCE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  1,  4 },
		{  8, IPCM_GUARDIAN | IPCM_IRON_MAIDEN | IPCM_EXECUTIONER | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_MURMILLO, {PERK_TOUGHNESS,  4, PERK_ENDURANCE,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  3, 12 },
		{ 12, IPCM_GUARDIAN | IPCM_IRON_MAIDEN | IPCM_EXECUTIONER | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_MURMILLO, {PERK_TOUGHNESS,  6, PERK_ENDURANCE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  6, 24 },
		{ 16, IPCM_GUARDIAN | IPCM_IRON_MAIDEN | IPCM_EXECUTIONER | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_MURMILLO, {PERK_TOUGHNESS,  8, PERK_ENDURANCE,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10, 40 },
		{ 20, IPCM_GUARDIAN | IPCM_IRON_MAIDEN | IPCM_EXECUTIONER | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_MURMILLO, {PERK_TOUGHNESS, 10, PERK_ENDURANCE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15, 60 },
	} },

	{ SYNERGY_IMPATIENCE, {"cooldown for nova spells::    reduced by %i seconds"}, "Impatience", {//ok
		{ 24, IPCM_MAGE | IPCM_WARLOCK, {PERK_UNSTOPPABLE, 6, PERK_MASTER_CASTER, 3}/*perk*/, {}/*spell*/, 0,0,120,0/*sdmv*/, 1 },
		{ 31, IPCM_MAGE | IPCM_WARLOCK, {PERK_UNSTOPPABLE, 8, PERK_MASTER_CASTER, 4}/*perk*/, {}/*spell*/, 0,0,170,0/*sdmv*/, 2 },
		{ 37, IPCM_MAGE | IPCM_WARLOCK, {PERK_UNSTOPPABLE, 10, PERK_MASTER_CASTER, 5}/*perk*/, {}/*spell*/, 0,0,220,0/*sdmv*/, 3 },
		{ 43, IPCM_WARLOCK, {PERK_UNSTOPPABLE, 11, PERK_MASTER_CASTER, 7}/*perk*/, {}/*spell*/, 0,0,270,0/*sdmv*/, 4 },
		{ 49, IPCM_WARLOCK, {PERK_UNSTOPPABLE, 13, PERK_MASTER_CASTER, 9}/*perk*/, {}/*spell*/, 0,0,320,0/*sdmv*/, 5 },
		{ 55, IPCM_WARLOCK, {PERK_UNSTOPPABLE, 15, PERK_MASTER_CASTER, 10}/*perk*/, {}/*spell*/, 0,0,370,0/*sdmv*/, 6 },
	} },

	{ SYNERGY_GLAD_DISCIPLINE, {"    weapon damage: +%i%%","    stun threshold: +%i","    armor class: -%i"}, "Gladiatorial Discipline", {//ok
		{ 13, IPCM_THRAEX | IPCM_DIMACHAERUS | IPCM_SECUTOR, {PERK_FOCUS, 2, PERK_SLAYER, 3}/*perk*/, {}/*spell*/, 45,45,0,0/*sdmv*/,  50, 1, 4 },
		{ 19, IPCM_THRAEX | IPCM_DIMACHAERUS | IPCM_SECUTOR, {PERK_FOCUS, 4, PERK_SLAYER, 5}/*perk*/, {}/*spell*/, 60,60,0,0/*sdmv*/, 100, 2, 8 },
	} },

	{ SYNERGY_RETRIBUTION, {"    weapon damage: +%i%%","    accuracy: +%i"}, "Retribution", {//ok
		{ 10, IPCM_TEMPLAR, {PERK_STRONG_ARM,  2, PERK_OBSESSION,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  60,  20 },
		{ 18, IPCM_TEMPLAR, {PERK_STRONG_ARM,  4, PERK_OBSESSION,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 120,  40 },
		{ 25, IPCM_TEMPLAR, {PERK_STRONG_ARM,  6, PERK_OBSESSION,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 180,  60 },
		{ 29, IPCM_TEMPLAR, {PERK_STRONG_ARM,  8, PERK_OBSESSION,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 240,  80 },
		{ 32, IPCM_TEMPLAR, {PERK_STRONG_ARM, 10, PERK_OBSESSION, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 300, 100 },
	} },

	{ SYNERGY_STRENGTH_N_HONOR, {"    weapon damage: +%i%%","    accuracy: +%i"}, "Strength and Honor", {//?
		{  8, IPCM_GUARDIAN, {PERK_STEEL_SKIN,  2, PERK_TOUGHNESS,  2, PERK_IRONCLAD,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  35,  7 },
		{ 15, IPCM_GUARDIAN, {PERK_STEEL_SKIN,  4, PERK_TOUGHNESS,  4, PERK_IRONCLAD,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  70, 14 },
		{ 22, IPCM_GUARDIAN, {PERK_STEEL_SKIN,  6, PERK_TOUGHNESS,  6, PERK_IRONCLAD,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 105, 21 },
		{ 29, IPCM_GUARDIAN, {PERK_STEEL_SKIN,  8, PERK_TOUGHNESS,  8, PERK_IRONCLAD,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 140, 28 },
		{ 36, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 10, PERK_TOUGHNESS, 10, PERK_IRONCLAD, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 175, 35 },
		{ 43, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 12, PERK_TOUGHNESS, 12, PERK_IRONCLAD, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 210, 42 },
		{ 50, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 14, PERK_TOUGHNESS, 14, PERK_IRONCLAD, 14}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 245, 49 },
		{ 57, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 16, PERK_TOUGHNESS, 16, PERK_IRONCLAD, 16}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 280, 56 },
		{ 64, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 18, PERK_TOUGHNESS, 18, PERK_IRONCLAD, 18}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 315, 63 },
		{ 71, IPCM_GUARDIAN, {PERK_STEEL_SKIN, 20, PERK_TOUGHNESS, 20, PERK_IRONCLAD, 20}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 350, 70 },
	} },

	{ SYNERGY_LIGHT_FINGERED, {"    -%i%% damage from traps,","amount of gold found::    gets increased by %i%%"}, "Light-Fingered", {//ok
		{ 19, IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_GLADIATOR, {PERK_AWARENESS, 1, PERK_SIXTH_SENSE, 3, PERK_GOLD_FEVER,  5}/*perk*/, {}/*spell*/, 0, 70,0,0/*sdmv*/,  80, 25 },
		{ 26, IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_GLADIATOR & ~IPCM_MURMILLO, {PERK_AWARENESS, 1, PERK_SIXTH_SENSE, 3, PERK_GOLD_FEVER, 10}/*perk*/, {}/*spell*/, 0, 80,0,0/*sdmv*/,  85, 50 },
		{ 32, IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_GLADIATOR & ~IPCM_MURMILLO, {PERK_AWARENESS, 1, PERK_SIXTH_SENSE, 3, PERK_GOLD_FEVER, 15}/*perk*/, {}/*spell*/, 0, 90,0,0/*sdmv*/,  90, 75 },
		{ 37, IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_SHUGOKI | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_GLADIATOR & ~IPCM_MURMILLO, {PERK_AWARENESS, 1, PERK_SIXTH_SENSE, 3, PERK_GOLD_FEVER, 20}/*perk*/, {}/*spell*/, 0,100,0,0/*sdmv*/,  95, 100 },
	} },

	{ SYNERGY_SILVER_TONGUE, {"your skill at trading::gets you more discounts!::    extra %i%% decrease in prices::when purchasing from traders"}, "Silver Tongue", {//ok
		{  4, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_HAGGLER, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 45, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_HAGGLER, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_BASTION_OF_ELEMENTS, {"elemental aura radius::    further increased by %i%%"}, "Bastion of Elements", {//ok
		{ 27, IPCM_INQUISITOR, {PERK_RESISTANCE,  5, PERK_AURA_RADIUS, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 31, IPCM_INQUISITOR, {PERK_RESISTANCE, 10, PERK_AURA_RADIUS, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
	} },

	{ SYNERGY_VALOR, {"    +%i to melee damage,","    +%i to accuracy,","    -%i to maximum mana"}, "Valor", {//ok
		{  4, IPCM_TEMPLAR, {PERK_FORTITUDE,  2, PERK_FOCUS, 1}/*perk*/, {}/*spell*/, 30,0,10,0/*sdmv*/,  10,  5,  20 },
		{ 10, IPCM_TEMPLAR, {PERK_FORTITUDE,  4, PERK_FOCUS, 2}/*perk*/, {}/*spell*/, 40,0,20,0/*sdmv*/,  25, 11,  40 },
		{ 16, IPCM_TEMPLAR, {PERK_FORTITUDE,  6, PERK_FOCUS, 3}/*perk*/, {}/*spell*/, 50,0,30,0/*sdmv*/,  45, 18,  60 },
		{ 22, IPCM_TEMPLAR, {PERK_FORTITUDE,  8, PERK_FOCUS, 4}/*perk*/, {}/*spell*/, 60,0,40,0/*sdmv*/,  70, 26,  80 },
		{ 28, IPCM_TEMPLAR, {PERK_FORTITUDE, 10, PERK_FOCUS, 5}/*perk*/, {}/*spell*/, 70,0,50,0/*sdmv*/, 100, 35, 100 },
	} },

	{ SYNERGY_ABJURATION, {"Walls and Rings::of Fire and Lightning::    last %i seconds longer,","holy spell damage::    gets increased by %i%%"}, "Abjuration", {//ok
		{  11, IPCM_TEMPLAR, {PERK_PATH_OF_MANA, 1, PERK_SPELL_DEFLECTION, 1}/*perk*/, {}/*spell*/,  50,0, 40,0/*sdmv*/, 10,  5 },
		{  21, IPCM_TEMPLAR, {PERK_PATH_OF_MANA, 2, PERK_SPELL_DEFLECTION, 2}/*perk*/, {}/*spell*/,  90,0, 80,0/*sdmv*/, 20, 10 },
		{  28, IPCM_TEMPLAR, {PERK_PATH_OF_MANA, 3, PERK_SPELL_DEFLECTION, 3}/*perk*/, {}/*spell*/, 130,0,120,0/*sdmv*/, 30, 15 },
	} },

	{ SYNERGY_THAUMATURGY, {"base Magic cap::    gets increased by %i"}, "Thaumaturgy", {// ?
		{  8, IPCM_TEMPLAR, {PERK_MIGHT_N_MAGIC,  2, PERK_SANCTITY, 1}/*perk*/, {}/*spell*/,  50,0, 30,0/*sdmv*/,  20 },
		{ 16, IPCM_TEMPLAR, {PERK_MIGHT_N_MAGIC,  4, PERK_SANCTITY, 2}/*perk*/, {}/*spell*/,  70,0, 55,0/*sdmv*/,  40 },
		{ 24, IPCM_TEMPLAR, {PERK_MIGHT_N_MAGIC,  6, PERK_SANCTITY, 3}/*perk*/, {}/*spell*/,  90,0, 80,0/*sdmv*/,  60 },
		{ 32, IPCM_TEMPLAR, {PERK_MIGHT_N_MAGIC,  8, PERK_SANCTITY, 4}/*perk*/, {}/*spell*/, 110,0,105,0/*sdmv*/,  80 },
		{ 40, IPCM_TEMPLAR, {PERK_MIGHT_N_MAGIC, 10, PERK_SANCTITY, 5}/*perk*/, {}/*spell*/, 130,0,130,0/*sdmv*/, 100 },
	} },

	{ SYNERGY_CAREFUL_MENDING, {"    %i%% chance to avoid::losing item durability::when repairing with skill"}, "Careful Mending", {
		{  5, IPCM_WARRIOR, {PERK_UNSTOPPABLE, 1, PERK_CONSERVATOR, 1}/*perk*/, {}/*spell*/,  0,0,0,0/*sdmv*/,  10 },
		{  9, IPCM_WARRIOR, {PERK_UNSTOPPABLE, 2, PERK_CONSERVATOR, 2}/*perk*/, {}/*spell*/,  0,0,0,0/*sdmv*/,  19 },
		{ 13, IPCM_WARRIOR, {PERK_UNSTOPPABLE, 3, PERK_CONSERVATOR, 3}/*perk*/, {}/*spell*/,  0,0,0,0/*sdmv*/,  27 },
		{ 17, IPCM_WARRIOR, {PERK_UNSTOPPABLE, 4, PERK_CONSERVATOR, 4}/*perk*/, {}/*spell*/,  0,0,0,0/*sdmv*/,  34 },
		{ 21, IPCM_WARRIOR, {PERK_UNSTOPPABLE, 5, PERK_CONSERVATOR, 5}/*perk*/, {}/*spell*/,  0,0,0,0/*sdmv*/,  40 },
	} },

	{ SYNERGY_RESTORATION, {"regeneration of life and mana::    gets increased by %i"}, "Restoration", {//ok
		{  4, IPCM_ALL_CLASSES, {PERK_FORTITUDE, 1, PERK_FOCUS, 1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		{  8, IPCM_ALL_CLASSES, {PERK_FORTITUDE, 2, PERK_FOCUS, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
		{ 12, IPCM_ALL_CLASSES, {PERK_FORTITUDE, 3, PERK_FOCUS, 3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 3 },
		{ 16, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER | IPCM_THRAEX | IPCM_DIMACHAERUS | IPCM_SECUTOR, {PERK_FORTITUDE, 4, PERK_FOCUS, 4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4 },
		{ 20, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER | IPCM_THRAEX | IPCM_SECUTOR, {PERK_FORTITUDE, 5, PERK_FOCUS, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 5 },
	} },

	{ SYNERGY_GOLDEN_SUNS, {"amount of gold found::    increased by %i pieces"}, "Golden Suns", {// ?
		{  7, IPCM_ALL_CLASSES, {PERK_GOLD_FEVER, 2, PERK_OBSESSION, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 11, IPCM_ALL_CLASSES, {PERK_GOLD_FEVER, 4, PERK_OBSESSION, 4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35 },
		{ 15, IPCM_ALL_CLASSES, {PERK_GOLD_FEVER, 6, PERK_OBSESSION, 6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 60 },
	} },

	{ SYNERGY_HIGHER_EDUCATION, {"    gain %i more stat point::per level up to distribute"}, "Higher Education", {// ?
		{ 45, IPCM_ALL_CLASSES, {PERK_EDUCATED, 5, PERK_HEAVY_DRINKER, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
	} },

	{ SYNERGY_FATAL_IMPACTS, {"    +%i%% critical damage,","    +%i to accuracy"}, "Fatal Impacts", {// ?
		{  8, IPCM_ANY_WARRIOR | IPCM_ARCHER | IPCM_SHARPSHOOTER | IPCM_ANY_MONK | (IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER) | IPCM_ANY_SAVAGE & ~IPCM_SAVAGE, {PERK_SLAYER, 3, PERK_WILLPOWER, 3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20, 10 },
		{ 18, IPCM_WARRIOR | IPCM_ARCHER | IPCM_SHARPSHOOTER | IPCM_ANY_MONK | (IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER) | IPCM_EXECUTIONER | IPCM_DIMACHAERUS, {PERK_SLAYER, 6, PERK_WILLPOWER, 6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40, 20 },
		{ 28, IPCM_ARCHER | IPCM_SHARPSHOOTER | IPCM_ANY_MONK | (IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER) | IPCM_DIMACHAERUS, {PERK_SLAYER, 9, PERK_WILLPOWER, 9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 60, 30 },
	} },

	{ SYNERGY_REJUVENATION, {"    Life increased by %i,","Life regeneration::    increased by %i"}, "Rejuvenation", {// ?
		{ 20, IPCM_GUARDIAN | IPCM_KENSEI | IPCM_SAVAGE | IPCM_EXECUTIONER, {PERK_BLOODSURGE, 5, PERK_FORTITUDE, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25, 5 },
		{ 35, IPCM_EXECUTIONER, {PERK_BLOODSURGE, 10, PERK_FORTITUDE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 75, 10 },
	} },

	{ SYNERGY_MANA_FLOW, {"    mana increased by %i,","mana regeneration::    increased by %i"}, "Mana Flow", {// ?
		{ 19, IPCM_GUARDIAN | IPCM_ASSASSIN | IPCM_SHUGOKI, {PERK_VAMPYRISM, 5, PERK_FOCUS, 1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20, 3 },
		{ 35, IPCM_SHUGOKI, {PERK_VAMPYRISM, 10, PERK_FOCUS, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 45, 6 },
	} },

	{ SYNERGY_BUTCHERY, {"Melee damage resistance::    increased by %i"}, "Butchery", {// ok
		{ 16, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_GAIN_STRENGTH,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 22, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_GAIN_STRENGTH, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_GRACE, {"    +%i to armor class"}, "Grace", {// ok
		{ 16, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_GAIN_DEXTERITY,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 22, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_GAIN_DEXTERITY, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
	} },

	{ SYNERGY_NEGATION, {"    +%i to all resistance"}, "Negation", {// ok
		{ 16, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_GAIN_MAGIC,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 22, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_GAIN_MAGIC, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_BOLSTER, {"    %i damage from enemies"}, "Bolster", {// ok
		{ 16, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_GAIN_VITALITY,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, -4 },
		{ 22, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_GAIN_VITALITY, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, -8 },
	} },

	{ SYNERGY_ACUITY, {"    +%i to accuracy,","    +%i points to damage,","    +%i to armor class"}, "Acuity", {// ?
		{  8, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS,  3, PERK_EAGLE_EYE,  3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  15,  10,  7 },
		{ 16, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS,  6, PERK_EAGLE_EYE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  30,  20, 14 },
		{ 24, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS,  9, PERK_EAGLE_EYE,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  45,  30, 21 },
		{ 32, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 12, PERK_EAGLE_EYE, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  60,  40, 28 },
		{ 40, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 15, PERK_EAGLE_EYE, 15}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  75,  50, 35 },
		{ 49, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 18, PERK_EAGLE_EYE, 18}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  90,  60, 42 },
		{ 58, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 21, PERK_EAGLE_EYE, 21}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 105,  70, 49 },
		{ 67, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 24, PERK_EAGLE_EYE, 24}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 120,  80, 56 },
		{ 76, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 27, PERK_EAGLE_EYE, 27}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 135,  90, 63 },
		{ 85, IPCM_WARRIOR | IPCM_GUARDIAN, {PERK_FOCUS, 30, PERK_EAGLE_EYE, 30}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 150, 100, 70 },
	} },

	{ SYNERGY_EXCEPTIONAL_FERVOUR, {"    +%i%% crit chance,","   +%i%% critical damage,","melee damage resistance::   increased by %i,","arrow damage resistance::   increased by %i"}, "Exceptional Fervour", {// ?
		{ 20, IPCM_SHINOBI, {PERK_DODGER,  5, PERK_EAGLE_EYE,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2, 20, 2, 2 },
		{ 40, IPCM_SHINOBI, {PERK_DODGER, 10, PERK_EAGLE_EYE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4, 40, 4, 4 },
		{ 45, IPCM_SHINOBI, {PERK_DODGER, 15, PERK_EAGLE_EYE, 15}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 6, 60, 6, 6 },
	} },

	{ SYNERGY_MORTAL_STRIKE, {"    +%i%% crit chance"}, "Mortal Strike", {//ok
		{ 14, IPCM_WARRIOR, {PERK_FOCUS,  2, PERK_EAGLE_EYE,  2}/*perk*/, {}/*spell*/, 0, 50,0,0/*sdmv*/,  3 },
		{ 21, IPCM_WARRIOR, {PERK_FOCUS,  4, PERK_EAGLE_EYE,  4}/*perk*/, {}/*spell*/, 0, 80,0,0/*sdmv*/,  6 },
		{ 32, IPCM_WARRIOR, {PERK_FOCUS,  6, PERK_EAGLE_EYE,  6}/*perk*/, {}/*spell*/, 0,130,0,0/*sdmv*/,  9 },
		{ 48, IPCM_WARRIOR, {PERK_FOCUS,  8, PERK_EAGLE_EYE,  8}/*perk*/, {}/*spell*/, 0,220,0,0/*sdmv*/, 12 },
		{ 60, IPCM_WARRIOR, {PERK_FOCUS, 10, PERK_EAGLE_EYE, 10}/*perk*/, {}/*spell*/, 0,350,0,0/*sdmv*/, 15 },
	} },

	{ SYNERGY_SEEKER_ARROWS, {"   +%i%% critical hit damage"}, "Seeker Arrows", {//ok
		{ 15, IPCM_WARRIOR, {PERK_EAGLE_EYE,  2, PERK_BODKIN_ARROWS,  2}/*perk*/, {}/*spell*/, 0, 40,0,0/*sdmv*/,  60 },
		{ 21, IPCM_WARRIOR, {PERK_EAGLE_EYE,  4, PERK_BODKIN_ARROWS,  4}/*perk*/, {}/*spell*/, 0, 60,0,0/*sdmv*/, 120 },
		{ 27, IPCM_WARRIOR, {PERK_EAGLE_EYE,  6, PERK_BODKIN_ARROWS,  6}/*perk*/, {}/*spell*/, 0, 80,0,0/*sdmv*/, 180 },
		{ 33, IPCM_WARRIOR, {PERK_EAGLE_EYE,  8, PERK_BODKIN_ARROWS,  8}/*perk*/, {}/*spell*/, 0,100,0,0/*sdmv*/, 240 },
		{ 39, IPCM_WARRIOR, {PERK_EAGLE_EYE, 10, PERK_BODKIN_ARROWS, 10}/*perk*/, {}/*spell*/, 0,120,0,0/*sdmv*/, 300 },
	} },

	{ SYNERGY_AUXILIARY_ARMOR, {"    %i%% chance to reduce","    damage received by %i%%"}, "Auxiliary Armour", {//ok
		{  6, IPCM_GUARDIAN | IPCM_SHUGOKI | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER,	{PERK_TOUGHNESS, 1, PERK_IRONCLAD,  2, PERK_UNSTOPPABLE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10, 60 },
		{ 12, IPCM_GUARDIAN | IPCM_SHUGOKI | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER,	{PERK_TOUGHNESS, 2, PERK_IRONCLAD,  4, PERK_UNSTOPPABLE,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15, 65 },
		{ 18, IPCM_GUARDIAN | IPCM_SHUGOKI | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER,	{PERK_TOUGHNESS, 3, PERK_IRONCLAD,  6, PERK_UNSTOPPABLE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20, 70 },
		{ 24, IPCM_GUARDIAN | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_EXECUTIONER,					{PERK_TOUGHNESS, 4, PERK_IRONCLAD,  8, PERK_UNSTOPPABLE,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25, 75 },
		{ 29, IPCM_SAVAGE | IPCM_EXECUTIONER,													{PERK_TOUGHNESS, 5, PERK_IRONCLAD, 10, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30, 80 },
		{ 34, IPCM_EXECUTIONER,																	{PERK_TOUGHNESS, 6, PERK_IRONCLAD, 12, PERK_UNSTOPPABLE, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35, 85 },
		{ 39, IPCM_EXECUTIONER,																	{PERK_TOUGHNESS, 7, PERK_IRONCLAD, 14, PERK_UNSTOPPABLE, 14}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40, 90 },
	} },

	{ SYNERGY_DEATH_DENIER, {"melee and ranged autohit::    improved by %i%%"}, "Death Denier", {//ok
		{ 15, IPCM_EXECUTIONER, {PERK_TOUGHNESS,  5, PERK_UNSTOPPABLE,  5}/*perk*/, {}/*spell*/,  70,0,0,0/*sdmv*/, 1 },
		{ 22, IPCM_EXECUTIONER, {PERK_TOUGHNESS, 10, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 120,0,0,0/*sdmv*/, 2 },
	} },

	{ SYNERGY_VERSATILITY, {"    damage increased by %i"}, "Versatility", {//ok
		{ 30, IPCM_ARCHER, {PERK_LETHALITY, 5, PERK_PURGE, 5, PERK_EXORCISM, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 50 },
	} },

	{ SYNERGY_INFERNAL_BARGAIN, {"    get %i more perk points,","life & mana regeneration::    decreased by %i,","    all stats decreased by %i"}, "Infernal Bargain", {// ?
		{ 13, IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE & ~IPCM_WARLOCK | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_SAVAGE & ~IPCM_EXECUTIONER,											{PERK_GAIN_MAGIC, 1, PERK_GAIN_VITALITY, 1}/*perk*/, {}/*spell*/, 0,0,0,30/*sdmv*/,  5,  2,  2 },
		{ 21, IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE & ~IPCM_WARLOCK | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_SAVAGE & ~IPCM_EXECUTIONER,											{PERK_GAIN_MAGIC, 2, PERK_GAIN_VITALITY, 2}/*perk*/, {}/*spell*/, 0,0,0,40/*sdmv*/, 10,  4,  4 },
		{ 29, IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE & ~IPCM_WARLOCK | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_ANY_SAVAGE & ~IPCM_EXECUTIONER,											{PERK_GAIN_MAGIC, 3, PERK_GAIN_VITALITY, 3}/*perk*/, {}/*spell*/, 0,0,0,50/*sdmv*/, 15,  6,  6 },
		{ 34, IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE & ~IPCM_WARLOCK | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_THRAEX | IPCM_DIMACHAERUS | IPCM_SECUTOR,	{PERK_GAIN_MAGIC, 4, PERK_GAIN_VITALITY, 4}/*perk*/, {}/*spell*/, 0,0,0,60/*sdmv*/, 20,  8,  8 },
		{ 38, IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE & ~IPCM_WARLOCK | IPCM_ANY_MONK | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_BERSERKER | IPCM_THRAEX | IPCM_SECUTOR,						{PERK_GAIN_MAGIC, 5, PERK_GAIN_VITALITY, 5}/*perk*/, {}/*spell*/, 0,0,0,70/*sdmv*/, 25, 10, 10 },
	} },

	{ SYNERGY_PERSEVERANCE, {"    extra %i%% chance to::avoid getting stunned"}, "Perseverance", {//ok
		{ 17, IPCM_ELEMENTALIST, {PERK_UNSTOPPABLE, 2, PERK_FOOTWORK,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 32, IPCM_ELEMENTALIST, {PERK_UNSTOPPABLE, 4, PERK_FOOTWORK, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
	} },

	{ SYNERGY_DEEP_WOUNDS, {"    +%i%% critical damage"}, "Deep Wounds", {//ok
		{ 10, IPCM_ELEMENTALIST, {PERK_GUIDED_HAND,  3, PERK_CRIPPLING_STRIKES,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  50 },
		{ 18, IPCM_ELEMENTALIST, {PERK_GUIDED_HAND,  6, PERK_CRIPPLING_STRIKES,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 100 },
		{ 26, IPCM_ELEMENTALIST, {PERK_GUIDED_HAND,  9, PERK_CRIPPLING_STRIKES,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 150 },
		{ 34, IPCM_ELEMENTALIST, {PERK_GUIDED_HAND, 12, PERK_CRIPPLING_STRIKES,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 200 },
		{ 42, IPCM_ELEMENTALIST, {PERK_GUIDED_HAND, 15, PERK_CRIPPLING_STRIKES, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 250 },
	} },

	{ SYNERGY_MESSENGER_OF_DEATH, {"    +%i damage with swords,","    +%i damage with bows"}, "Messenger of Death", {// ready!
		{  14, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD,  4, PERK_TEMPEST,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,   33,  15},
		{  25, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD,  6, PERK_TEMPEST,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,   66,  30 },
		{  36, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD,  8, PERK_TEMPEST,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  100,  45 },
		{  50, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 10, PERK_TEMPEST, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  150,  65 },
		{  60, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 12, PERK_TEMPEST, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  200,  90 },
		{  70, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 14, PERK_TEMPEST, 14}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  250, 120 },
		{  80, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 16, PERK_TEMPEST, 16}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  300, 150 },
		{  90, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 18, PERK_TEMPEST, 18}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  400, 185 },
		{ 100, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 20, PERK_TEMPEST, 20}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  500, 225 },
		{ 110, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 22, PERK_TEMPEST, 22}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  600, 270 },
		{ 120, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 24, PERK_TEMPEST, 24}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  700, 320 },
		{ 130, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 26, PERK_TEMPEST, 26}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  800, 375 },
		{ 140, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 28, PERK_TEMPEST, 28}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  900, 435 },
		{ 150, IPCM_ELEMENTALIST, {PERK_SHARPENED_SWORD, 30, PERK_TEMPEST, 30}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1000, 500 },
	} },

	{ SYNERGY_TRADER, {"    +%i to maximum level::of items sold in shops"}, "Trader", {// expanded
		{  20, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER,  5, PERK_GOLD_FEVER,  5 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{  40, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER,  6, PERK_GOLD_FEVER,  6 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{  60, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER,  7, PERK_GOLD_FEVER,  7 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{  80, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER,  8, PERK_GOLD_FEVER,  8 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 100, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER,  9, PERK_GOLD_FEVER,  9 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
		{ 120, IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_KENSEI | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR, {PERK_HAGGLER, 10, PERK_GOLD_FEVER, 10 }/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
	} },

	{ SYNERGY_OSTENTATION, {"all base attribute limits::    raised by %i points"}, "Ostentation", {
		{ 60, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_OBSESSION,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  25 },
		{ 70, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_OBSESSION,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  50 },
		{ 80, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 15, PERK_OBSESSION,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  75 },
		{ 90, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 20, PERK_OBSESSION, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 100 },
	} },

	{ SYNERGY_START_EARLY, {"minimal hit points::needed for targets::to get improved damage::    increased by %i%%"}, "Start Early", {
		{ 20, IPCM_SHINOBI, {PERK_STRONG_ARM,  4, PERK_FINAL_JUDGEMENT,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 35, IPCM_SHINOBI, {PERK_STRONG_ARM,  8, PERK_FINAL_JUDGEMENT,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 50, IPCM_SHINOBI, {PERK_STRONG_ARM, 12, PERK_FINAL_JUDGEMENT,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 65, IPCM_SHINOBI, {PERK_STRONG_ARM, 16, PERK_FINAL_JUDGEMENT,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 80, IPCM_SHINOBI, {PERK_STRONG_ARM, 20, PERK_FINAL_JUDGEMENT, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
	} },

	{ SYNERGY_CRIMSON_TIDE, {"chance for critical hit::    further increased by %i%%,","critical hit damage::    further increased by %i%%"}, "Crimson Tide", {
		{ 51, IPCM_SHUGOKI, {PERK_FOCUS,  2, PERK_UNSTOPPABLE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  2,  25 },
		{ 60, IPCM_SHUGOKI, {PERK_FOCUS,  4, PERK_UNSTOPPABLE,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  4,  50 },
		{ 69, IPCM_SHUGOKI, {PERK_FOCUS,  6, PERK_UNSTOPPABLE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  6,  75 },
		{ 78, IPCM_SHUGOKI, {PERK_FOCUS,  8, PERK_UNSTOPPABLE,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  8, 100 },
		{ 87, IPCM_SHUGOKI, {PERK_FOCUS, 10, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10, 125 },
	} },

	{ SYNERGY_SNOWBALL_EFFECT, {"maximal hit points::needed for monsters::to get increased damage::    reduced further by %i%%"}, "Snowball Effect", {
		{ 25, IPCM_SAVAGE, {PERK_BLOODSURGE, 1, PERK_CRUSHING_BLOWS,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{ 41, IPCM_SAVAGE, {PERK_BLOODSURGE, 2, PERK_CRUSHING_BLOWS,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{ 57, IPCM_SAVAGE, {PERK_BLOODSURGE, 3, PERK_CRUSHING_BLOWS,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{ 73, IPCM_SAVAGE, {PERK_BLOODSURGE, 4, PERK_CRUSHING_BLOWS,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{ 89, IPCM_SAVAGE, {PERK_BLOODSURGE, 5, PERK_CRUSHING_BLOWS, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
	} },

	{ SYNERGY_DAZZLING_LIGHT, {"melee and ranged autohit::    improved by %i%%"}, "Dazzling Light", {
		{ 56, IPCM_TEMPLAR, {PERK_HOLY_CAUSE, 5, PERK_CONSERVATOR, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
	} },

	{ SYNERGY_PICK_UP_THE_PACE, {"    +%i%% to experience::points gained additionally"}, "Pick up the Pace", {
		{ 45, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_REHABILITATION,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 100 },
		{ 50, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 15, PERK_REHABILITATION,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 200 },
		{ 55, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 20, PERK_REHABILITATION,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 300 },
		{ 60, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 25, PERK_REHABILITATION,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 400 },
		{ 65, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 30, PERK_REHABILITATION,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 500 },
		{ 70, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 35, PERK_REHABILITATION,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 600 },
		{ 75, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 40, PERK_REHABILITATION, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 700 },
	} },

	{ SYNERGY_CORNERED_BEAST, {"if current hit points::are reduced below 40 percent,::critical hit chance will be::    increased by %i%%"}, "Cornered Beast", {
		{  20, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE,  5, PERK_TOUGHNESS,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		{  30, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 10, PERK_TOUGHNESS,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		{  40, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 14, PERK_TOUGHNESS,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		{  55, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 18, PERK_TOUGHNESS,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		{  70, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 22, PERK_TOUGHNESS,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
		{  85, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 26, PERK_TOUGHNESS,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
		{ 100, IPCM_ANY_ROGUE & ~IPCM_BOMBARDIER, {PERK_FORTITUDE, 30, PERK_TOUGHNESS, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35 },
	} },

	{ SYNERGY_DURABLE, {"melee & ranged damage::resistance caps get::    improved by %i%%"}, "Durable", {
		{ 25, IPCM_THRAEX, {PERK_ENDURANCE,  3, PERK_IRONCLAD,  2}/*perk*/, {}/*spell*/, 0,0,0,100/*sdmv*/, 1 },
		{ 35, IPCM_THRAEX, {PERK_ENDURANCE,  6, PERK_IRONCLAD,  4}/*perk*/, {}/*spell*/, 0,0,0,150/*sdmv*/, 2 },
		{ 45, IPCM_THRAEX, {PERK_ENDURANCE,  9, PERK_IRONCLAD,  6}/*perk*/, {}/*spell*/, 0,0,0,200/*sdmv*/, 3 },
		{ 55, IPCM_THRAEX, {PERK_ENDURANCE, 12, PERK_IRONCLAD,  8}/*perk*/, {}/*spell*/, 0,0,0,250/*sdmv*/, 4 },
		{ 65, IPCM_THRAEX, {PERK_ENDURANCE, 15, PERK_IRONCLAD, 10}/*perk*/, {}/*spell*/, 0,0,0,300/*sdmv*/, 5 },
	} },

	{ SYNERGY_ELEM_BARRIER, {"elemental resistance caps::    further increased by %i%%"}, "Elemental Barrier", {
		{ 26, IPCM_EXECUTIONER, {PERK_WILLPOWER,  2, PERK_UNSTOPPABLE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		{ 35, IPCM_EXECUTIONER, {PERK_WILLPOWER,  4, PERK_UNSTOPPABLE,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
		{ 44, IPCM_EXECUTIONER, {PERK_WILLPOWER,  6, PERK_UNSTOPPABLE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 3 },
		{ 53, IPCM_EXECUTIONER, {PERK_WILLPOWER,  8, PERK_UNSTOPPABLE,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4 },
		{ 62, IPCM_EXECUTIONER, {PERK_WILLPOWER, 10, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 5 },
	} },

	{ SYNERGY_ANCHOR, {"    +%i%% chance to resist::knockback effect"}, "Anchor", {
		/* 0*/ { 18, IPCM_IRON_MAIDEN, {PERK_CONSERVATOR,  3, PERK_UNSTOPPABLE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		/* 1*/ { 26, IPCM_IRON_MAIDEN, {PERK_CONSERVATOR,  5, PERK_UNSTOPPABLE,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		/* 2*/ { 34, IPCM_IRON_MAIDEN, {PERK_CONSERVATOR,  8, PERK_UNSTOPPABLE,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
		/* 3*/ { 42, IPCM_IRON_MAIDEN, {PERK_CONSERVATOR, 10, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40 },
	} },

	{ SYNERGY_OVERWHELM, {"    +%i stat point on level-up"}, "Overwhelm", {
		/* 0*/ { 15, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER,  5, PERK_ADVANCED_USER, 1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		//* 1*/ { 25, IPCM_ALL_CLASSES, {PERK_SWIFT_LEARNER, 10, PERK_ADVANCED_USER, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
	} },

	{ SYNERGY_PAINKILLER, {"    +%i to life regeneration::when in battle"}, "Painkiller", {
		/* 0*/ {  9, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_FORTITUDE, 3, PERK_REHABILITATION, 1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		/* 1*/ { 15, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_FORTITUDE, 6, PERK_REHABILITATION, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
	} },

	{ SYNERGY_GUNDOWN, {"Shoot to kill!::    +%i accuracy,","    +%i bullet damage"}, "Gundown", {
		/* 0*/ {   5, IPCM_GUARDIAN, {PERK_OVERDRAW,  2, PERK_STOPPING_POWER,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5,   5 },
		/* 1*/ {  11, IPCM_GUARDIAN, {PERK_OVERDRAW,  5, PERK_STOPPING_POWER,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10,  10 },
		/* 2*/ {  22, IPCM_GUARDIAN, {PERK_OVERDRAW,  9, PERK_STOPPING_POWER,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15,  20 },
		/* 3*/ {  37, IPCM_GUARDIAN, {PERK_OVERDRAW, 13, PERK_STOPPING_POWER,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20,  40 },
		/* 4*/ {  56, IPCM_GUARDIAN, {PERK_OVERDRAW, 17, PERK_STOPPING_POWER, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25,  80 },
		/* 5*/ {  79, IPCM_GUARDIAN, {PERK_OVERDRAW, 21, PERK_STOPPING_POWER, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30, 160 },
		/* 6*/ { 106, IPCM_GUARDIAN, {PERK_OVERDRAW, 25, PERK_STOPPING_POWER, 14}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35, 320 },
	} },

	{ SYNERGY_BETTER_CRITS, {"    +%i%% critical hit chance,","    +%i%% critical damage"}, "Better Criticals", {
		/* 0*/ {  17, IPCM_GUARDIAN, {PERK_MARKSMAN,  2, PERK_PIERCING_BULLETS,  2}/*perk*/, {}/*spell*/, 0, 60,0,0/*sdmv*/, 1,  25 },
		/* 1*/ {  24, IPCM_GUARDIAN, {PERK_MARKSMAN,  4, PERK_PIERCING_BULLETS,  4}/*perk*/, {}/*spell*/, 0, 80,0,0/*sdmv*/, 2,  50 },
		/* 2*/ {  31, IPCM_GUARDIAN, {PERK_MARKSMAN,  6, PERK_PIERCING_BULLETS,  6}/*perk*/, {}/*spell*/, 0,100,0,0/*sdmv*/, 3,  75 },
		/* 3*/ {  39, IPCM_GUARDIAN, {PERK_MARKSMAN,  8, PERK_PIERCING_BULLETS,  8}/*perk*/, {}/*spell*/, 0,140,0,0/*sdmv*/, 4, 100 },
		/* 4*/ {  47, IPCM_GUARDIAN, {PERK_MARKSMAN, 10, PERK_PIERCING_BULLETS, 10}/*perk*/, {}/*spell*/, 0,180,0,0/*sdmv*/, 5, 125 },
	} },

	{ SYNERGY_LONGEVITY, {"    +%i life,","    +%i life regeneration"}, "Longevity", {
		/* 0*/ {  65, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  2, PERK_FORTITUDE,  2, PERK_GAIN_VITALITY,  1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  40,  4 },
		/* 1*/ {  69, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  4, PERK_FORTITUDE,  4, PERK_GAIN_VITALITY,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  80,  8 },
		/* 2*/ {  73, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  6, PERK_FORTITUDE,  6, PERK_GAIN_VITALITY,  3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 120, 12 },
		/* 3*/ {  77, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE,  8, PERK_FORTITUDE,  8, PERK_GAIN_VITALITY,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 160, 16 },
		/* 4*/ {  81, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 10, PERK_FORTITUDE, 10, PERK_GAIN_VITALITY,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 200, 20 },
		/* 5*/ {  85, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 12, PERK_FORTITUDE, 12, PERK_GAIN_VITALITY,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 240, 24 },
		/* 6*/ {  89, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 14, PERK_FORTITUDE, 14, PERK_GAIN_VITALITY,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 280, 28 },
		/* 7*/ {  93, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 16, PERK_FORTITUDE, 16, PERK_GAIN_VITALITY,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 320, 32 },
		/* 8*/ {  97, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 18, PERK_FORTITUDE, 18, PERK_GAIN_VITALITY,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 360, 36 },
		/* 9*/ { 101, IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE, {PERK_ENDURANCE, 20, PERK_FORTITUDE, 20, PERK_GAIN_VITALITY, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 400, 40 },
	} },

	{ SYNERGY_AWAKENING, {"    +%i mana,","    +%i mana regeneration"}, "Awakening", {
		/* 0*/ {  65, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  2, PERK_FOCUS,  2, PERK_GAIN_MAGIC,  1}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  40,  4 },
		/* 1*/ {  69, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  4, PERK_FOCUS,  4, PERK_GAIN_MAGIC,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  80,  8 },
		/* 2*/ {  73, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  6, PERK_FOCUS,  6, PERK_GAIN_MAGIC,  3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 120, 12 },
		/* 3*/ {  77, IPCM_ALL_CLASSES, {PERK_WILLPOWER,  8, PERK_FOCUS,  8, PERK_GAIN_MAGIC,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 160, 16 },
		/* 4*/ {  81, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 10, PERK_FOCUS, 10, PERK_GAIN_MAGIC,  5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 200, 20 },
		/* 5*/ {  85, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 12, PERK_FOCUS, 12, PERK_GAIN_MAGIC,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 240, 24 },
		/* 6*/ {  89, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 14, PERK_FOCUS, 14, PERK_GAIN_MAGIC,  7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 280, 28 },
		/* 7*/ {  93, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 16, PERK_FOCUS, 16, PERK_GAIN_MAGIC,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 320, 32 },
		/* 8*/ {  97, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 18, PERK_FOCUS, 18, PERK_GAIN_MAGIC,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 360, 36 },
		/* 9*/ { 101, IPCM_ALL_CLASSES, {PERK_WILLPOWER, 20, PERK_FOCUS, 20, PERK_GAIN_MAGIC, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 400, 40 },
	} },

	{ SYNERGY_AURA_MASTERY, {"    +%i%% all aura damage"}, "Aura Mastery", {
		/* 0*/ {  14, IPCM_INQUISITOR, {PERK_SEARING_AURA,  3, PERK_STATIC_AURA,  3, PERK_THEURGIC_AURA,  3, PERK_TOXIC_AURA,  3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/,  5 },
		/* 1*/ {  27, IPCM_INQUISITOR, {PERK_SEARING_AURA,  6, PERK_STATIC_AURA,  6, PERK_THEURGIC_AURA,  6, PERK_TOXIC_AURA,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 10 },
		/* 2*/ {  40, IPCM_INQUISITOR, {PERK_SEARING_AURA,  9, PERK_STATIC_AURA,  9, PERK_THEURGIC_AURA,  9, PERK_TOXIC_AURA,  9}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 15 },
		/* 3*/ {  53, IPCM_INQUISITOR, {PERK_SEARING_AURA, 12, PERK_STATIC_AURA, 12, PERK_THEURGIC_AURA, 12, PERK_TOXIC_AURA, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20 },
		/* 4*/ {  66, IPCM_INQUISITOR, {PERK_SEARING_AURA, 15, PERK_STATIC_AURA, 15, PERK_THEURGIC_AURA, 15, PERK_TOXIC_AURA, 15}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25 },
		/* 5*/ {  79, IPCM_INQUISITOR, {PERK_SEARING_AURA, 18, PERK_STATIC_AURA, 18, PERK_THEURGIC_AURA, 18, PERK_TOXIC_AURA, 18}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 30 },
		/* 6*/ {  92, IPCM_INQUISITOR, {PERK_SEARING_AURA, 21, PERK_STATIC_AURA, 21, PERK_THEURGIC_AURA, 21, PERK_TOXIC_AURA, 21}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35 },
		/* 7*/ { 105, IPCM_INQUISITOR, {PERK_SEARING_AURA, 24, PERK_STATIC_AURA, 24, PERK_THEURGIC_AURA, 24, PERK_TOXIC_AURA, 24}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 40 },
		/* 8*/ { 118, IPCM_INQUISITOR, {PERK_SEARING_AURA, 27, PERK_STATIC_AURA, 27, PERK_THEURGIC_AURA, 27, PERK_TOXIC_AURA, 27}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 45 },
		/* 9*/ { 131, IPCM_INQUISITOR, {PERK_SEARING_AURA, 30, PERK_STATIC_AURA, 30, PERK_THEURGIC_AURA, 30, PERK_TOXIC_AURA, 30}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 50 },
	} },

	{ SYNERGY_INVINCIBLE, {"when under Fury,::    resist %i%% damage::(of all types)"}, "Invincible", {
		/* 0*/ { 25, IPCM_BERSERKER, {PERK_FEEL_NO_PAIN,  2, PERK_RESISTANCE,  3}/*perk*/, {}/*spell*/, 0,0,0,100/*sdmv*/, 10 },
		/* 1*/ { 34, IPCM_BERSERKER, {PERK_FEEL_NO_PAIN,  4, PERK_RESISTANCE,  6}/*perk*/, {}/*spell*/, 0,0,0,150/*sdmv*/, 19 },
		/* 2*/ { 43, IPCM_BERSERKER, {PERK_FEEL_NO_PAIN,  6, PERK_RESISTANCE,  9}/*perk*/, {}/*spell*/, 0,0,0,200/*sdmv*/, 27 },
		/* 3*/ { 52, IPCM_BERSERKER, {PERK_FEEL_NO_PAIN,  8, PERK_RESISTANCE, 12}/*perk*/, {}/*spell*/, 0,0,0,250/*sdmv*/, 34 },
		/* 4*/ { 61, IPCM_BERSERKER, {PERK_FEEL_NO_PAIN, 10, PERK_RESISTANCE, 15}/*perk*/, {}/*spell*/, 0,0,0,300/*sdmv*/, 40 },
	} }, 
	
	{ SYNERGY_STRONGER_QUENCHING, {"increase quench grade limit::    by %i grade(s)"}, "Stronger Quenching", {
		/* 0*/ { 70, IPCM_ALL_CLASSES, {PERK_HAGGLER,  2, PERK_CONSERVATOR, 2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 1 },
		/* 1*/ { 75, IPCM_ALL_CLASSES, {PERK_HAGGLER,  4, PERK_CONSERVATOR, 3}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 2 },
		/* 2*/ { 80, IPCM_ALL_CLASSES, {PERK_HAGGLER,  6, PERK_CONSERVATOR, 4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 3 },
		/* 3*/ { 85, IPCM_ALL_CLASSES, {PERK_HAGGLER,  8, PERK_CONSERVATOR, 5}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 4 },
		/* 4*/ { 90, IPCM_ALL_CLASSES, {PERK_HAGGLER,  9, PERK_CONSERVATOR, 6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 5 },
		/* 5*/ { 95, IPCM_ALL_CLASSES, {PERK_HAGGLER, 10, PERK_CONSERVATOR, 7}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 6 },
	} },

	{ SYNERGY_BOARDING_TECHNIQUE, {"    %i%% of Dexterity::is added to melee damage"}, "Boarding Technique", {
		/* 0*/ { 11, IPCM_ROGUE, {PERK_GAIN_DEXTERITY,  2, PERK_EAGLE_EYE,  3}/*perk*/, {}/*spell*/, 0, 60,0,0/*sdmv*/, 15 },
		/* 1*/ { 20, IPCM_ROGUE, {PERK_GAIN_DEXTERITY,  4, PERK_EAGLE_EYE,  6}/*perk*/, {}/*spell*/, 0, 95,0,0/*sdmv*/, 30 },
		/* 2*/ { 29, IPCM_ROGUE, {PERK_GAIN_DEXTERITY,  6, PERK_EAGLE_EYE,  9}/*perk*/, {}/*spell*/, 0,130,0,0/*sdmv*/, 45 },
		/* 3*/ { 38, IPCM_ROGUE, {PERK_GAIN_DEXTERITY,  8, PERK_EAGLE_EYE, 12}/*perk*/, {}/*spell*/, 0,165,0,0/*sdmv*/, 60 },
		/* 4*/ { 47, IPCM_ROGUE, {PERK_GAIN_DEXTERITY, 10, PERK_EAGLE_EYE, 15}/*perk*/, {}/*spell*/, 0,200,0,0/*sdmv*/, 75 },
	} },

	{ SYNERGY_LAST_STAND, {"    %i%% chance to reduce","    damage received by %i%%","    if life is below %i%%"}, "Last Stand", {
		/* 0*/ {  6, IPCM_THRAEX,	{PERK_IRONCLAD,  1, PERK_UNSTOPPABLE,  2}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 20, 45, 33 },
		/* 1*/ { 13, IPCM_THRAEX,	{PERK_IRONCLAD,  2, PERK_UNSTOPPABLE,  4}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 25, 50, 36 },
		/* 2*/ { 20, IPCM_THRAEX,	{PERK_IRONCLAD,  3, PERK_UNSTOPPABLE,  6}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 29, 55, 39 },
		/* 3*/ { 27, IPCM_THRAEX,	{PERK_IRONCLAD,  4, PERK_UNSTOPPABLE,  8}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 32, 60, 42 },
		/* 4*/ { 34, IPCM_THRAEX,	{PERK_IRONCLAD,  5, PERK_UNSTOPPABLE, 10}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 35, 65, 45 },
		/* 5*/ { 41, IPCM_THRAEX,	{PERK_IRONCLAD,  6, PERK_UNSTOPPABLE, 12}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 38, 70, 48 },
		/* 6*/ { 48, IPCM_THRAEX,	{PERK_IRONCLAD,  7, PERK_UNSTOPPABLE, 14}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 41, 75, 51 },
		/* 7*/ { 55, IPCM_THRAEX,	{PERK_IRONCLAD,  8, PERK_UNSTOPPABLE, 16}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 44, 80, 54 },
		/* 8*/ { 62, IPCM_THRAEX,	{PERK_IRONCLAD,  9, PERK_UNSTOPPABLE, 18}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 47, 85, 57 },
		/* 9*/ { 69, IPCM_THRAEX,	{PERK_IRONCLAD, 10, PERK_UNSTOPPABLE, 20}/*perk*/, {}/*spell*/, 0,0,0,0/*sdmv*/, 50, 90, 60 },
	} },

	{ SYNERGY_RIGIDITY, {"melee & arrow damage::    resistance increased by %i"}, "Rigidity", {
		/* 0*/ { 16, IPCM_MURMILLO, {PERK_IRONCLAD,  2, PERK_UNSTOPPABLE,  4}/*perk*/, {}/*spell*/, 0,0,0, 75/*sdmv*/,  5 },
		/* 1*/ { 27, IPCM_MURMILLO, {PERK_IRONCLAD,  4, PERK_UNSTOPPABLE,  8}/*perk*/, {}/*spell*/, 0,0,0,115/*sdmv*/, 10 },
		/* 2*/ { 38, IPCM_MURMILLO, {PERK_IRONCLAD,  6, PERK_UNSTOPPABLE, 12}/*perk*/, {}/*spell*/, 0,0,0,155/*sdmv*/, 15 },
		/* 3*/ { 49, IPCM_MURMILLO, {PERK_IRONCLAD,  8, PERK_UNSTOPPABLE, 16}/*perk*/, {}/*spell*/, 0,0,0,195/*sdmv*/, 20 },
		/* 4*/ { 60, IPCM_MURMILLO, {PERK_IRONCLAD, 10, PERK_UNSTOPPABLE, 20}/*perk*/, {}/*spell*/, 0,0,0,235/*sdmv*/, 25 },
	} },

	{ SYNERGY_STRONG_HEART, {"gain +1 melee damage::    for each %i points in life"}, "Strong Heart", {
		/* 0*/ { 11, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_ENDURANCE,  5, PERK_STRONG_ARM,  5}/*perk*/, {}/*spell*/,  70,0,0,0/*sdmv*/, 25 },
		/* 1*/ { 21, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_ENDURANCE, 10, PERK_STRONG_ARM, 10}/*perk*/, {}/*spell*/, 120,0,0,0/*sdmv*/, 20 },
		/* 2*/ { 31, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_ENDURANCE, 15, PERK_STRONG_ARM, 15}/*perk*/, {}/*spell*/, 170,0,0,0/*sdmv*/, 15 },
		/* 3*/ { 41, IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE, {PERK_ENDURANCE, 20, PERK_STRONG_ARM, 20}/*perk*/, {}/*spell*/, 220,0,0,0/*sdmv*/, 10 },
	} },

};
