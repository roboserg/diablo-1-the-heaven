#include "stdafx.h"

// Affix array at 0x0048B820
Affix AffixesClassic[] = {
	// Name,	Effects,	type,	minLow,	minHigh,	maxLow,	maxHigh,	chanceLow,	chanceHigh,	QualityLevel,	item,	IS_ALL,	ExcludedCombinations,	DoubleChance,	NotCursed,	MinGoldValue,	MaxGoldValue,	Multiplier, CLVL req min,	CLVL req max


		/* ------------------------------------
		-------------- PREFIXES ---------------
		-------------------------------------*/

		/* ------------------------------------
						SLVL
		-------------------------------------*/

{ 294,	"angel's",		AE_SPELLS_LEVEL,	0,	1,	1,	0,	0,	0,	0,	 15,	IA_STAFF,	IS_ALL,	0,	0,	1,	 25000,	 25000,	2,	0,	 0},
{ 295,	"arch-angel's",	AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0,	 25,	IA_STAFF,	IS_ALL,	0,	0,	1,	 50000,	 50000,	3,	0,	 0},
		

		/* ------------------------------------
						MANA
		-------------------------------------*/

{ 306,	"spider's",		AE_MANA,	0,	 10,	 15,	0,	0,	0,	0,	  1,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	   100,	   1000,	 2,	 0,	  0 },
{ 306,	"raven's",		AE_MANA,	0,	 16,	 20,	0,	0,	0,	0,	  5,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	  1100,	   2000,	 3,	 0,	  0 },
{ 306,	"snake's",		AE_MANA,	0,	 21,	 30,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	  2100,	   4000,	 5,	 0,	  0 },
{ 306,	"serpent's",	AE_MANA,	0,	 31,	 40,	0,	0,	0,	0,	 15,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	  4100,	   6000,	 7,	 0,	  0 },
{ 306,	"drake's",		AE_MANA,	0,	 41,	 50,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	  6100,	  10000,	 9,	 0,	  0 },
{ 306,	"dragon's",		AE_MANA,	0,	 51,	 60,	0,	0,	0,	0,	 27,	IA_JEWEL | IA_STAFF,				IS_ALL,		0,	0,	1,	 10100,	  15000,	11,	 0,	  0 },
		// these below are Hellfire-only, I don't add Hellfire stuff
		//{ 306,	"wyrm's",		AE_MANA,	0,	 61,	 80,	0,	0,	0,	0,	 35,	IA_ARMOR,						IS_ALL,		0,	0,	1,	 15100,	  19000,	12,	 0,	  0 },
		//{ 306,	"hydra's",		AE_MANA,	0,	 81,	100,	0,	0,	0,	0,	 60,	IA_ARMOR,						IS_ALL,		0,	0,	1,	 19100,	  30000,	13,	 0,	  0 },


		/* ------------------------------------
					ARMOR PERCENT
		-------------------------------------*/

{ 306,	"fine",			AE_PERCENT_AC,	0,	 50,	 65,	0,	0,	0,	0,	  1,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	    20,	    100,	 2,	 0,	  0 },
{ 306,	"strong",		AE_PERCENT_AC,	0,	 66,	 80,	0,	0,	0,	0,	  3,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   120,	    200,	 3,	 0,	  0 },
{ 306,	"grand",		AE_PERCENT_AC,	0,	 81,	 95,	0,	0,	0,	0,	  6,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   220,	    300,	 5,	 0,	  0 },
{ 306,	"valiant",		AE_PERCENT_AC,	0,	 96,	105,	0,	0,	0,	0,	 10,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   320,	    400,	 7,	 0,	  0 },
{ 306,	"glorious",		AE_PERCENT_AC,	0,	106,	120,	0,	0,	0,	0,	 14,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   420,	    600,	 9,	 0,	  0 },
{ 306,	"blessed",		AE_PERCENT_AC,	0,	121,	135,	0,	0,	0,	0,	 19,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   620,	    800,	11,	 0,	  0 },
{ 306,	"saintly",		AE_PERCENT_AC,	0,	136,	150,	0,	0,	0,	0,	 24,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	   820,	   1200,	13,	 0,	  0 },
{ 306,	"awesome",		AE_PERCENT_AC,	0,	156,	170,	0,	0,	0,	0,	 28,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	  1220,	   2000,	15,	 0,	  0 },
{ 306,	"holy",			AE_PERCENT_AC,	0,	171,	185,	0,	0,	0,	0,	 35,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	  5200,	   6000,	17,	 0,	  0 },
{ 306,	"godly",		AE_PERCENT_AC,	0,	186,	200,	0,	0,	0,	0,	 60,	IA_ARMOR | IA_SHIELD,					IS_ALL,		0,	0,	1,	  6200,	   7000,	20,	 0,	  0 },


		/* ------------------------------------
						TO_HIT
		-------------------------------------*/

{ 1,	"bronze",		AE_TO_HIT,	0,	0,	0,	0,	0,	  6,	 10,	  1,	IA_MELEE | IA_RANGE | IA_JEWEL,			IS_ALL,		0,	0,	1,	   100,	   500,	 2,	  0,	  0 },
{ 1,	"iron",			AE_TO_HIT,	0,	0,	0,	0,	0,	 11,	 15,	  4,	IA_MELEE | IA_RANGE | IA_JEWEL,			IS_ALL,		0,	0,	1,	   600,	  1000,	 3,	  0,	  0 },
{ 1,	"steel",		AE_TO_HIT,	0,	0,	0,	0,	0,	 16,	 20,	  6,	IA_MELEE | IA_RANGE | IA_JEWEL,			IS_ALL,		0,	0,	1,	  1100,	  1500,	 5,	  0,	  0 },
{ 1,	"silver",		AE_TO_HIT,	0,	0,	0,	0,	0,	 21,	 25,	  9,	IA_MELEE | IA_RANGE | IA_JEWEL,			IS_ALL,		0,	0,	1,	  1600,	  2000,	 7,	  0,	  0 },
{ 1,	"gold",			AE_TO_HIT,	0,	0,	0,	0,	0,	 26,	 30,	 12,	IA_MELEE | IA_RANGE | IA_JEWEL,			IS_ALL,		0,	0,	1,	  2100,	  3000,	 9,	  0,	  0 },
{ 1,	"platinum",		AE_TO_HIT,	0,	0,	0,	0,	0,	 31,	 40,	 16,	IA_MELEE | IA_RANGE,					IS_ALL,		0,	0,	1,	  3100,	  4000,	11,	  0,	  0 },
{ 1,	"mithril",		AE_TO_HIT,	0,	0,	0,	0,	0,	 41,	 60,	 20,	IA_MELEE | IA_RANGE,					IS_ALL,		0,	0,	1,	  4100,	  6000,	13,	  0,	  0 },
{ 1,	"meteoric",		AE_TO_HIT,	0,	0,	0,	0,	0,	 61,	 80,	 23,	IA_MELEE | IA_RANGE,					IS_ALL,		0,	0,	1,	  6100,	 10000,	15,	  0,	  0 },
{ 1,	"weird",		AE_TO_HIT,	0,	0,	0,	0,	0,	 81,	100,	 35,	IA_MELEE | IA_RANGE,					IS_ALL,		0,	0,	1,	 10100,	 14000,	17,	  0,	  0 },
{ 1,	"strange",		AE_TO_HIT,	0,	0,	0,	0,	0,	101,	150,	 60,	IA_MELEE | IA_RANGE,					IS_ALL,		0,	0,	1,	 14100,	 20000,	20,	  0,	  0 },

		/* ------------------------------------
			DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

{  19,	"jagged",		AE_PERCENT_DAMAGE,	0,	 40,	 55,	0,	0,	0,	0,	  1,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	   250,	   450,	 3,	0,	0 },
{  19,	"deadly",		AE_PERCENT_DAMAGE,	0,	 66,	 70,	0,	0,	0,	0,	  4,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	   500,	   700,	 4,	0,	0 },
{  19,	"heavy",		AE_PERCENT_DAMAGE,	0,	 71,	 85,	0,	0,	0,	0,	  7,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	   750,	   950,	 5,	0,	0 },
{  19,	"vicious",		AE_PERCENT_DAMAGE,	0,	 86,	100,	0,	0,	0,	0,	 10,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	  1000,	  1450,	 8,	0,	0 },
{  19,	"brutal",		AE_PERCENT_DAMAGE,	0,	101,	115,	0,	0,	0,	0,	 14,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	  1500,	  1950,	10,	0,	0 },
{  19,	"massive",		AE_PERCENT_DAMAGE,	0,	116,	130,	0,	0,	0,	0,	 18,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	  2000,	  2450,	13,	0,	0 },
{  19,	"savage",		AE_PERCENT_DAMAGE,	0,	111,	145,	0,	0,	0,	0,	 23,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	  2500,	  3000,	15,	0,	0 },
{  19,	"ruthless",		AE_PERCENT_DAMAGE,	0,	126,	160,	0,	0,	0,	0,	 35,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	 10100,	 15000,	17,	0,	0 },
{  19,	"merciless",	AE_PERCENT_DAMAGE,	0,	161,	175,	0,	0,	0,	0,	 47,	IA_RANGE | IA_MELEE,	IS_ALL,	0,	0,	1,	 15000,	 20000,	20,	0,	0 },


		/* ------------------------------------
		TO_HIT + DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

{ 45,	"sharp",		AE_TO_HIT_DAMAGE,	0,	 40,	 55,	0,	0,	 6,	 10,	  2,	IA_RANGE | IA_STAFF | IA_MELEE,		IS_ALL,	0,	0,	1,	   350,	   950,	 5,	  0,	  0 },
{ 45,	"fine",			AE_TO_HIT_DAMAGE,	0,	 56,	 70,	0,	0,	11,	 15,	  6,	IA_RANGE | IA_STAFF | IA_MELEE,		IS_ALL,	0,	0,	1,	  1100,	  1700,	 7,	  0,	  0 },
{ 45,	"warrior's",	AE_TO_HIT_DAMAGE,	0,	 71,	 85,	0,	0,	16,	 20,	 10,	IA_RANGE | IA_STAFF | IA_MELEE,		IS_ALL,	0,	0,	1,	  1850,	  2450,	13,	  0,	  0 },
{ 45,	"soldier's",	AE_TO_HIT_DAMAGE,	0,	 86,	100,	0,	0,	21,	 25,	 15,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	  2600,	  3950,	17,	  0,	  0 },
{ 45,	"lord's",		AE_TO_HIT_DAMAGE,	0,	101,	115,	0,	0,	26,	 30,	 19,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	  4100,	  5950,	21,	  0,	  0 },
{ 45,	"knight's",		AE_TO_HIT_DAMAGE,	0,	116,	130,	0,	0,	31,	 40,	 23,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	  6100,	  8450,	26,	  0,	  0 },
{ 45,	"master's",		AE_TO_HIT_DAMAGE,	0,	131,	145,	0,	0,	41,	 50,	 28,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	  8600,	 13000,	30,	  0,	  0 },
{ 45,	"champion's",	AE_TO_HIT_DAMAGE,	0,	146,	160,	0,	0,	51,	 75,	 40,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	 15200,	 24000,	33,	  0,	  0 },
{ 45,	"king's",		AE_TO_HIT_DAMAGE,	0,	161,	175,	0,	0,	76,	100,	 60,	IA_STAFF | IA_MELEE,				IS_ALL,	0,	0,	1,	 24100,	 35000,	38,	  0,	  0 },

		
		/* ------------------------------------
					VARIOUS
		-------------------------------------*/
					
{ 60,	"Plentiful",	AE_EXTRA_CHARGES,	0,	2,	2,	0,	0,	0,	0,	4,	IA_STAFF,	IS_ALL,	0,	0,	1,	2'000,	2'000,	2,	0,	0 },
{ 61,	"Bountiful",	AE_EXTRA_CHARGES,	0,	3,	3,	0,	0,	0,	0,	9,	IA_STAFF,	IS_ALL,	0,	0,	1,	3'000,	3'000,	3,	0,	0 },

		/* ------------------------------------
				MAGIC RESISTANCES
		-------------------------------------*/

	// fire	
{ 62,	"red",		AE_RESIST,	ET_1_FIRE,	 10,	 20,	0,	0,	0,	0,	  4,	IA_ALL,	IS_ALL,	0,	0,	1,	   500,	  1500,	2,	 0,	 0 },
{ 62,	"crunson",	AE_RESIST,	ET_1_FIRE,	 21,	 30,	0,	0,	0,	0,	 10,	IA_ALL,	IS_ALL,	0,	0,	1,	  2100,	  3000,	2,	 0,	 0 },
{ 62,	"crimson",	AE_RESIST,	ET_1_FIRE,	 31,	 40,	0,	0,	0,	0,	 16,	IA_ALL,	IS_ALL,	0,	0,	1,	  3100,	  4000,	2,	 0,	 0 },
{ 62,	"garnet",	AE_RESIST,	ET_1_FIRE,	 41,	 50,	0,	0,	0,	0,	 20,	IA_ALL,	IS_ALL,	0,	0,	1,	  8200,	 12000,	3,	 0,	 0 },
{ 62,	"ruby",		AE_RESIST,	ET_1_FIRE,	 51,	 60,	0,	0,	0,	0,	 26,	IA_ALL,	IS_ALL,	0,	0,	1,	 17100,	 20000,	5,	 0,	 0 },

	// lite
{ 62,	"blue",		AE_RESIST,	ET_2_LIGHTNING,	 10,	 20,	0,	0,	0,	0,	  4,	IA_ALL,	IS_ALL,	0,	0,	1,	   500,	  1500,	2,	 0,	 0 },
{ 62,	"azure",	AE_RESIST,	ET_2_LIGHTNING,	 21,	 30,	0,	0,	0,	0,	 10,	IA_ALL,	IS_ALL,	0,	0,	1,	  2100,	  3000,	2,	 0,	 0 },
{ 62,	"lapis",	AE_RESIST,	ET_2_LIGHTNING,	 31,	 40,	0,	0,	0,	0,	 16,	IA_ALL,	IS_ALL,	0,	0,	1,	  3100,	  4000,	2,	 0,	 0 },
{ 62,	"cobalt",	AE_RESIST,	ET_2_LIGHTNING,	 41,	 50,	0,	0,	0,	0,	 20,	IA_ALL,	IS_ALL,	0,	0,	1,	  8200,	 12000,	3,	 0,	 0 },
{ 62,	"sapphire",	AE_RESIST,	ET_2_LIGHTNING,	 51,	 60,	0,	0,	0,	0,	 26,	IA_ALL,	IS_ALL,	0,	0,	1,	 17100,	 20000,	5,	 0,	 0 },

	// arcane	
{ 62,	"white",	AE_RESIST,	ET_3_ARCAN,	 10,	 20,	0,	0,	0,	0,	  4,	IA_ALL,	IS_ALL,	0,	0,	1,	   500,	  1500,	2,	 0,	 0 },
{ 62,	"pearl",	AE_RESIST,	ET_3_ARCAN,	 21,	 30,	0,	0,	0,	0,	 10,	IA_ALL,	IS_ALL,	0,	0,	1,	  2100,	  3000,	2,	 0,	 0 },
{ 62,	"ivory",	AE_RESIST,	ET_3_ARCAN,	 31,	 40,	0,	0,	0,	0,	 16,	IA_ALL,	IS_ALL,	0,	0,	1,	  3100,	  4000,	2,	 0,	 0 },
{ 62,	"crystal",	AE_RESIST,	ET_3_ARCAN,	 41,	 50,	0,	0,	0,	0,	 20,	IA_ALL,	IS_ALL,	0,	0,	1,	  8200,	 12000,	3,	 0,	 0 },
{ 62,	"diamond",	AE_RESIST,	ET_3_ARCAN,	 51,	 60,	0,	0,	0,	0,	 26,	IA_ALL,	IS_ALL,	0,	0,	1,	 17100,	 20000,	5,	 0,	 0 },

	// all	
{ 62,	"topaz",	AE_RESIST_All,	0,	 10,	 15,	0,	0,	0,	0,	  8,	IA_ALL,									IS_ALL,	0,	0,	1,	  2000,	  5000,	3,	 0,	 0 },
{ 62,	"amber",	AE_RESIST_All,	0,	 16,	 20,	0,	0,	0,	0,	 12,	IA_ALL,									IS_ALL,	0,	0,	1,	  7400,	 10000,	3,	 0,	 0 },
{ 62,	"jade",		AE_RESIST_All,	0,	 21,	 25,	0,	0,	0,	0,	 18,	IA_ALL,									IS_ALL,	0,	0,	1,	 11000,	 15000,	3,	 0,	 0 },
{ 62,	"obsidian",	AE_RESIST_All,	0,	 26,	 30,	0,	0,	0,	0,	 24,	IA_ALL,									IS_ALL,	0,	0,	1,	 24000,	 40000,	4,	 0,	 0 },
{ 62,	"emerald",	AE_RESIST_All,	0,	 31,	 35,	0,	0,	0,	0,	 31,	IA_MELEE|IA_RANGE|IA_SHIELD|IA_STAFF,	IS_ALL,	0,	0,	1,	 61000,	 75000,	7,	 0,	 0 },

	
	/* ------------------------------------
		ELEMENTAL DAMAGE ON WEAPONS
	-------------------------------------*/

{ 102,	"flaming",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 1,	 1,	10,	10,	0,	0,	 7,		IA_STAFF | IA_MELEE,			IS_ALL,	0,	0,	1,	  5000,	  5000,	2,	 0,	 0 },
	
{ 108,	"Static",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 2,	 2,	20,	20,	0,	0,	18,	IA_STAFF | IA_MELEE,			IS_ALL,	0,	0,	1,	 10000,	 10000,	2,	 0,	 0 },
	
	//	{ 0,	"Fire Enchanted",	AE_SPELL_DAMAGE,	PS_1_FIREBOLT,	2,	4,	8,	10,	0,	0,	10,	IA_STAFF|IA_SHIELD,	IS_MAGIC,	0,	0,	1,	900,	900,	3, 0, 0+2 },
	// + damage series needs to be continued
		{ 0,	"",	AE_NO,	1 }, //	type,	minLow,	minHigh,	maxLow,	maxHigh,	chanceLow,	chanceHigh,	QualityLevel,	item,	IS_ALL,	ExcludedCombinations,	DoubleChance,	NotCursed,	MinGoldValue,	MaxGoldValue,	Multiplier

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	/* ------------------------------------
	-------------- SUFFIXES ---------------
	-------------------------------------*/

	/* ------------------------------------
					+ DAMAGE
	-------------------------------------*/
		//1H + bow
{ 190,	"quality",		AE_DAMAGE,	0,	  1,	  2,	0,	0,	0,	0,	  2,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	   100,	   200,	 2,	 0,	 0 },
{ 190,	"maiming",		AE_DAMAGE,	0,	  3,	  5,	0,	0,	0,	0,	  7,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	  1300,	  1500,	 3,	 0,	 0 },
{ 190,	"slaying",		AE_DAMAGE,	0,	  6,	  8,	0,	0,	0,	0,	 15,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	  2600,	  3000,	 5,	 0,	 0 },
{ 190,	"gore",			AE_DAMAGE,	0,	  9,	 12,	0,	0,	0,	0,	 25,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	  4100,	  5000,	 8,	 0,	 0 },
{ 190,	"carnage",		AE_DAMAGE,	0,	 13,	 16,	0,	0,	0,	0,	 35,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	  5100,	 10000,	10,	 0,	 0 },
{ 190,	"slaughter",	AE_DAMAGE,	0,	 17,	 20,	0,	0,	0,	0,	 60,	IA_MACE_1H | IA_SWORD_1H | IA_RANGE,	IS_ALL,	0,	0,	1,	 10100,	 15000,	13,	 0,	 0 },
	//2H
{ 190,	"quality",		AE_DAMAGE,	0,	  2,	  3,	0,	0,	0,	0,	  2,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	   100,	   200,	 2,	 0,	 0 },
{ 190,	"maiming",		AE_DAMAGE,	0,	  4,	  6,	0,	0,	0,	0,	  7,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	  1300,	  1500,	 3,	 0,	 0 },
{ 190,	"slaying",		AE_DAMAGE,	0,	  7,	 10,	0,	0,	0,	0,	 15,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	  2600,	  3000,	 5,	 0,	 0 },
{ 190,	"gore",			AE_DAMAGE,	0,	 11,	 16,	0,	0,	0,	0,	 25,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	  4100,	  5000,	 8,	 0,	 0 },
{ 190,	"carnage",		AE_DAMAGE,	0,	 17,	 23,	0,	0,	0,	0,	 35,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	  5100,	 10000,	10,	 0,	 0 },
{ 190,	"slaughter",	AE_DAMAGE,	0,	 24,	 30,	0,	0,	0,	0,	 60,	IA_MACE_2H | IA_SWORD_2H | IA_AXE,		IS_ALL,	0,	0,	1,	 10100,	 15000,	13,	 0,	 0 },

	/* ------------------------------------
					DFE
	-------------------------------------*/

{ 241,	"health",		AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  2,	IA_SHIELD | IA_ARMOR | IA_JEWEL,	IS_ALL,		0,	0,	1,	   200,	   200,	 2,	  0,	  0 },
{ 241,	"protection",	AE_DFE,	0,	-2,	-2,	0,	0,	0,	0,	  6,	IA_SHIELD | IA_ARMOR,				IS_ALL,		0,	0,	1,	   400,	   400,	 4,	  0,	  0 },
{ 241,	"absoption",	AE_DFE,	0,	-3,	-3,	0,	0,	0,	0,	 12,	IA_SHIELD | IA_ARMOR,				IS_ALL,		0,	0,	1,	   900,	   900,	10,	  0,	  0 },
{ 241,	"deflection",	AE_DFE,	0,	-4,	-4,	0,	0,	0,	0,	 20,	IA_ARMOR,							IS_ALL,		0,	0,	1,	  2500,	  2500,	15,	  0,	  0 },
{ 241,	"osmosis",		AE_DFE,	0,	-5,	-6,	0,	0,	0,	0,	 50,	IA_ARMOR,							IS_ALL,		0,	0,	1,	  7500,	 10000,	20,	  0,	  0 },


/* ------------------------------------
				STATS
-------------------------------------*/

//STR
{ 279,	"Strength",		AE_STAT,	ST_STRENGTH,	 1,	  5,	0,	0,	0,	0,	  1,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	   200,	  1000,	 2,	 0,	  0 },
{ 279,	"Might",		AE_STAT,	ST_STRENGTH,	 6,	 10,	0,	0,	0,	0,	  5,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  1200,	  2000,	 3,	 0,	  0 },
{ 279,	"power",		AE_STAT,	ST_STRENGTH,	11,	 15,	0,	0,	0,	0,	 11,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  2200,	  3000,	 4,	 0,	  0 },
{ 279,	"giants",		AE_STAT,	ST_STRENGTH,	16,	 20,	0,	0,	0,	0,	 17,	IA_ARMOR | IA_MELEE | IA_RANGE | IA_JEWEL,												IS_ALL,		0,	0,	1,	  3200,	  5000,	 7,	 0,	  0 },
{ 279,	"titans",		AE_STAT,	ST_STRENGTH,	21,	 30,	0,	0,	0,	0,	 23,	IA_MELEE | IA_JEWEL,																	IS_ALL,		0,	0,	1,	  5200,	 10000,	10,	 0,	  0 },

//DEX
{ 279,	"dexterity",	AE_STAT,	ST_DEXTERITY,	 1,	  5,	0,	0,	0,	0,	  1,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	   200,	  1000,	 2,	 0,	  0 },
{ 279,	"skill",		AE_STAT,	ST_DEXTERITY,	 6,	 10,	0,	0,	0,	0,	  5,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  1200,	  2000,	 3,	 0,	  0 },
{ 279,	"accuracy",		AE_STAT,	ST_DEXTERITY,	11,	 15,	0,	0,	0,	0,	 11,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  2200,	  3000,	 4,	 0,	  0 },
{ 279,	"precision",	AE_STAT,	ST_DEXTERITY,	16,	 20,	0,	0,	0,	0,	 17,	IA_ARMOR | IA_MELEE | IA_RANGE | IA_JEWEL,												IS_ALL,		0,	0,	1,	  3200,	  5000,	 7,	 0,	  0 },
{ 279,	"perfection",	AE_STAT,	ST_DEXTERITY,	21,	 30,	0,	0,	0,	0,	 23,	IA_RANGE | IA_JEWEL,																	IS_ALL,		0,	0,	1,	  5200,	 10000,	10,	 0,	  0 },

//MAG	
{ 279,	"magic",		AE_STAT,	ST_MAGIC,		 1,	  5,	0,	0,	0,	0,	  1,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_STAFF | IA_RANGE | IA_JEWEL,						IS_ALL,		0,	0,	1,	   200,	  1000,	 2,	 0,	  0 },
{ 279,	"the mind",		AE_STAT,	ST_MAGIC,		 6,	 10,	0,	0,	0,	0,	  5,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_STAFF | IA_RANGE | IA_JEWEL,						IS_ALL,		0,	0,	1,	  1200,	  2000,	 3,	 0,	  0 },
{ 279,	"brilliance",	AE_STAT,	ST_MAGIC,		11,	 15,	0,	0,	0,	0,	 11,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_STAFF | IA_RANGE | IA_JEWEL,						IS_ALL,		0,	0,	1,	  2200,	  3000,	 4,	 0,	  0 },
{ 279,	"sorcery",		AE_STAT,	ST_MAGIC,		16,	 20,	0,	0,	0,	0,	 17,	IA_ARMOR | IA_MELEE | IA_STAFF | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  3200,	  5000,	 7,	 0,	  0 },
{ 279,	"wizardry",		AE_STAT,	ST_MAGIC,		21,	 30,	0,	0,	0,	0,	 23,	IA_STAFF | IA_JEWEL,																	IS_ALL,		0,	0,	1,	  5200,	 10000,	10,	 0,	  0 },

//VIT
{ 279,	"vitality",		AE_STAT,	ST_VITALITY,	 1,	  5,	0,	0,	0,	0,	  1,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	   200,	  1000,	 2,	 0,	  0 },
{ 279,	"zest",			AE_STAT,	ST_VITALITY,	 6,	 10,	0,	0,	0,	0,	  5,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  1200,	  2000,	 3,	 0,	  0 },
{ 279,	"vim",			AE_STAT,	ST_VITALITY,	11,	 15,	0,	0,	0,	0,	 11,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,									IS_ALL,		0,	0,	1,	  2200,	  3000,	 4,	 0,	  0 },
{ 279,	"vigor",		AE_STAT,	ST_VITALITY,	16,	 20,	0,	0,	0,	0,	 17,	IA_ARMOR | IA_MELEE | IA_RANGE | IA_JEWEL,												IS_ALL,		0,	0,	1,	  3200,	  5000,	 7,	 0,	  0 },
{ 279,	"life",			AE_STAT,	ST_VITALITY,	21,	 30,	0,	0,	0,	0,	 23,	IA_JEWEL,																				IS_ALL,		0,	0,	1,	  5200,	 10000,	10,	 0,	  0 },

//ALL STATS
{ 279,	"the sky",		AE_ALL_STAT,	0,	 1,	  3,	0,	0,	0,	0,	  5,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,											IS_ALL,		0,	0,	1,	   800,	  4000,	 5,	 0,	  0 },
{ 279,	"the moon",		AE_ALL_STAT,	0,	 4,	  7,	0,	0,	0,	0,	 11,	IA_ARMOR | IA_SHIELD | IA_MELEE | IA_RANGE | IA_JEWEL,											IS_ALL,		0,	0,	1,	  4800,	  8000,	10,	 0,	  0 },
{ 279,	"the stars",	AE_ALL_STAT,	0,	 8,	 11,	0,	0,	0,	0,	 17,	IA_ARMOR | IA_MELEE | IA_RANGE | IA_JEWEL,														IS_ALL,		0,	0,	1,	  8800,	 12000,	15,	 0,	  0 },
{ 279,	"the heavens",	AE_ALL_STAT,	0,	12,	 15,	0,	0,	0,	0,	 25,	IA_MELEE | IA_RANGE | IA_JEWEL,																	IS_ALL,		0,	0,	1,	 12800,	 20000,	20,	 0,	  0 },
{ 279,	"the zodiac",	AE_ALL_STAT,	0,	16,	 20,	0,	0,	0,	0,	 30,	IA_JEWEL,																						IS_ALL,		0,	0,	1,	 20800,	 40000,	30,	 0,	  0 },


/* ------------------------------------
				HP
-------------------------------------*/

{ 306,	"the fox",		AE_HIT_POINTS,	0,	 10,	 15,	0,	0,	0,	0,	  1,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	   100,	   1000,	 2,	 0,	  0 },
{ 306,	"the jaguar",	AE_HIT_POINTS,	0,	 16,	 20,	0,	0,	0,	0,	  5,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	  1100,	   2000,	 3,	 0,	  0 },
{ 306,	"the eagle",	AE_HIT_POINTS,	0,	 21,	 30,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	  2100,	   4000,	 5,	 0,	  0 },
{ 306,	"the wolf",		AE_HIT_POINTS,	0,	 31,	 40,	0,	0,	0,	0,	 15,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	  4100,	   6000,	 7,	 0,	  0 },
{ 306,	"the tiger",	AE_HIT_POINTS,	0,	 41,	 50,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	  6100,	  10000,	 9,	 0,	  0 },
{ 306,	"the lion",		AE_HIT_POINTS,	0,	 51,	 60,	0,	0,	0,	0,	 27,	IA_JEWEL | IA_ARMOR,							IS_ALL,		0,	0,	1,	 10100,	  15000,	11,	 0,	  0 },
{ 306,	"the mammoth",	AE_HIT_POINTS,	0,	 61,	 80,	0,	0,	0,	0,	 35,	IA_ARMOR,										IS_ALL,		0,	0,	1,	 15100,	  19000,	12,	 0,	  0 },
{ 306,	"the whale",	AE_HIT_POINTS,	0,	 81,	100,	0,	0,	0,	0,	 60,	IA_ARMOR,										IS_ALL,		0,	0,	1,	 19100,	  30000,	13,	 0,	  0 },


/* ------------------------------------
	RANGED WEAPONS' ELEMENTAL DAMAGE
-------------------------------------*/

{ 315,	"flame",			AE_ELEM_HIT,	EA_ARROW_FIRE,	 1,	 1,	 3,	 3,	0,	0,	 1,	IA_RANGE,	IS_ALL,	0,	0,	1,	  2000,	  2000,	2,	 0,	 0 },
{ 315,	"fire",				AE_ELEM_HIT,	EA_ARROW_FIRE,	 1,	 1,	 6,	 6,	0,	0,	11,	IA_RANGE,	IS_ALL,	0,	0,	1,	  4000,	  4000,	4,	 0,	 0 },
{ 315,	"burning",			AE_ELEM_HIT,	EA_ARROW_FIRE,	 1,	 1,	16,	16,	0,	0,	35,	IA_RANGE,	IS_ALL,	0,	0,	1,	  6000,	  6000,	6,	 0,	 0 },

{ 321,	"shock",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	 6,	 6,	0,	0,	13,	IA_RANGE,	IS_BATTLE,	0,	0,	1,	  6000,	  6000,	2,	 0,	 0 },
{ 321,	"lightning",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	10,	10,	0,	0,	21,	IA_RANGE,	IS_BATTLE,	0,	0,	1,	  8000,	  8000,	2,	 0,	 0 },
{ 321,	"thunder",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	20,	20,	0,	0,	60,	IA_RANGE,	IS_BATTLE,	0,	0,	1,	 12000,	 12000,	2,	 0,	 0 },


/* ------------------------------------
				VARIOUS
-------------------------------------*/

{ 356,	"the bat",	AE_MANA_STEAL_PERCENT,	0,	3,	3,	0,	0,	0,	0,	  8,	IA_MELEE,	IS_ALL,	0,	0,	1,	 7'500,	 7'500,	 3,	 0,	 0 },
{ 356,	"vampires",	AE_MANA_STEAL_PERCENT,	0,	5,	5,	0,	0,	0,	0,	 19,	IA_MELEE,	IS_ALL,	0,	0,	1,	14'500,	14'500,	 3,	 0,	 0 },

{ 358,	"The leech",AE_LIFE_STEAL_PERCENT,	0,	3,	3,	0,	0,	0,	0,	  8,	IA_MELEE,	IS_ALL,	0,	0,	1,	 7'500,	 7'500,	 3,	 0,	 0 },
{ 356,	"blood",	AE_LIFE_STEAL_PERCENT,	0,	5,	5,	0,	0,	0,	0,	 19,	IA_MELEE,	IS_ALL,	0,	0,	1,	14'500,	14'500,	 3,	 0,	 0 },

{ 362,	"readiness",	AE_ATTACK_SPEED,	0,	1,	1,	0,	0,	0,	0,	 5,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	 2,	0,	0 },
{ 363,	"swiftness",	AE_ATTACK_SPEED,	0,	2,	2,	0,	0,	0,	0,	16,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	 4,	0,	0 },
{ 364,	"speed",		AE_ATTACK_SPEED,	0,	3,	3,	0,	0,	0,	0,	31,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	0,	0 },
{ 364,	"haste",		AE_ATTACK_SPEED,	0,	4,	4,	0,	0,	0,	0,	57,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_BATTLE,	0,	0,	1,	16'000,	16'000,	16,	0,	0 },

// HIT RECOVERY
{ 365,	"balance",		AE_HIT_RECOVERY,	0,	1,	1,	0,	0,	0,	0,	 1,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	  2000,	  2000,	2,	 0,	 0 },
{ 365,	"stability",	AE_HIT_RECOVERY,	0,	2,	2,	0,	0,	0,	0,	12,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	  4000,	  4000,	4,	 0,	 0 },
{ 365,	"harmony",		AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0,	24,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	  8000,	  8000,	8,	 0,	 0 },

// KB
{ 368,	"the bear",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	 5,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_ALL,	0,	0,	1,	  750,	  750,	2,	 0,	 0 },
{ 369,	"the bear",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	19,	IA_RANGE | IA_STAFF | IA_MELEE,	IS_ALL,	0,	0,	1,	1'500,	1'500,	3,	 0,	 0 },

/* ------------------------------------
				DURABILITY
-------------------------------------*/

{ 180,	"STURDINESS",		AE_HIGH_DURABILITY,	0,	 26,	 75,	0,	0,	0,	0,	 1,	IA_RANGE | IA_MELEE | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	100,	100,	2,	 0,	 0 },
{ 180,	"craftsmanship",	AE_HIGH_DURABILITY,	0,	 51,	100,	0,	0,	0,	0,	 6,	IA_RANGE | IA_MELEE | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	200,	200,	2,	 0,	 0 },
{ 180,	"structure",		AE_HIGH_DURABILITY,	0,	101,	200,	0,	0,	0,	0,	12,	IA_RANGE | IA_MELEE | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	300,	300,	2,	 0,	 0 },


			{ 0,	"",	AE_NO },
};
// next new affix id = ?
