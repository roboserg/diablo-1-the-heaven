#include "stdafx.h"

// Affix array at 0x0048B820
Affix AffixesRare[] = {
	// Name,	Effects,	type,	minLow,	minHigh,	maxLow,	maxHigh,	chanceLow,	chanceHigh,	QualityLevel,	item,	IS_ALL,	ExcludedCombinations,	DoubleChance,	NotCursed,	MinGoldValue,	MaxGoldValue,	Multiplier
	
	
			/* ------------------------------------
		-------------- PREFIXES ---------------
		-------------------------------------*/

		/* ------------------------------------
						TO_HIT
		-------------------------------------*/

		{ 1,	"Copper",	AE_TO_HIT,	0,	0,	0,	0,	0,	  2,	  3,	  1,	IA_ALL,																IS_BATTLE,	0,	0,	1,	    50,	   200,	 1,	  0,	  0 },
		{ 2,	"Tin",		AE_TO_HIT,	0,	0,	0,	0,	0,	  4,	  5,	  7,	IA_JEWEL | IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	   525,	   750,	 2,	  0,	  0 },
		{ 3,	"Brass",	AE_TO_HIT,	0,	0,	0,	0,	0,	  8,	 10,	 12,	IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 1'050,	 1'500,	 3,	  2,	  3 },
		{ 4,	"Bronze",	AE_TO_HIT,	0,	0,	0,	0,	0,	 13,	 15,	 19,	IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 1'550,	 2'000,	 4,	  4,	  6 },
		{ 5,	"Iron",		AE_TO_HIT,	0,	0,	0,	0,	0,	 18,	 20,	 28,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 2'050,	 2'500,	 5,	  8,	 11 },
		{ 6,	"Steel",	AE_TO_HIT,	0,	0,	0,	0,	0,	 23,	 25,	 37,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 2'550,	 3'000,	 6,	 12,	 16 },
		{ 7,	"Silver",	AE_TO_HIT,	0,	0,	0,	0,	0,	 28,	 30,	 46,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 3'050,	 3'500,	 7,	 17,	 19 },
		{ 8,	"Gold",		AE_TO_HIT,	0,	0,	0,	0,	0,	 33,	 35,	 55,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 3'550,	 4'000,	 8,	 23,	 25 },
		{ 9,	"Platinum",	AE_TO_HIT,	0,	0,	0,	0,	0,	 38,	 40,	 62,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 4'050,	 5'000,	 9,	 30,	 33 },
		{ 485,	"Palladium",AE_TO_HIT,	0,	0,	0,	0,	0,	 43,	 45,	 72,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 6'000,	 7'000,	10,	 37,	 39 },
		{ 486,	"Palladium",AE_TO_HIT,	0,	0,	0,	0,	0,	 48,	 50,	 82,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 8'000,	 9'000,	11,	 44,	 46 },
		{ 487,	"Palladium",AE_TO_HIT,	0,	0,	0,	0,	0,	 53,	 55,	 92,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	10'000,	11'000,	11,	 51,	 53 },
		{ 488,	"Meteoric",	AE_TO_HIT,	0,	0,	0,	0,	0,	 58,	 60,	102,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	12'000,	13'000,	12,	 58,	 60 },
		{ 489,	"Meteoric",	AE_TO_HIT,	0,	0,	0,	0,	0,	 63,	 65,	112,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	14'000,	15'000,	12,	 65,	 67 },
		{ 490,	"Superior",	AE_TO_HIT,	0,	0,	0,	0,	0,	 68,	 70,	122,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	16'000,	17'000,	13,	 72,	 74 },
		{ 491,	"Superior",	AE_TO_HIT,	0,	0,	0,	0,	0,	 73,	 75,	132,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	18'000,	19'000,	13,	 76,	 79 },
		{ 492,	"Guiding",	AE_TO_HIT,	0,	0,	0,	0,	0,	 78,	 80,	142,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	20'000,	21'000,	14,	 81,	 84 },
		{ 493,	"Guiding",	AE_TO_HIT,	0,	0,	0,	0,	0,	 83,	 85,	152,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	22'000,	23'000,	14,	 86,	 88 },
		{ 494,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	 88,	 90,	162,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	24'000,	25'000,	15,	 90,	 94 },
		{ 495,	"Weird",	AE_TO_HIT,	0,	0,	0,	0,	0,	 93,	 95,	172,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	26'000,	27'000,	15,	 96,	 99 },
		
		//jewelry
		{  55,	"Superior",	AE_TO_HIT,	0,	0,	0,	0,	0,	 6,	 7,	 15,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 1'100,	 1'500,	4,	 3,	 4 },
		{  55,	"Guiding",	AE_TO_HIT,	0,	0,	0,	0,	0,	 9,	10,	 21,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 2'400,	 3'000,	5,	14,	16 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	11,	12,	 28,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 4'200,	 5'000,	6,	18,	20 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	13,	15,	 36,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 7'800,	 9'000,	6,	23,	26 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	16,	17,	 45,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	12'400,	14'000,	7,	28,	31 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	18,	20,	 57,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	21'600,	24'000,	7,	35,	37 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	21,	22,	 75,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	32'800,	35'000,	8,	45,	47 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	23,	25,	 90,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	36'000,	40'000,	8,	52,	57 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	26,	27,	110,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	41'000,	45'000,	9,	62,	67 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	28,	30,	130,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	46'000,	50'000,	9,	72,	77 },
		{  84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	31,	32,	150,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	51'000,	55'000,	9,	82,	87 },
		/* cursed */{84,	"Leading",	AE_TO_HIT,	0,	0,	0,	0,	0,	33,	35,	170,	IA_JEWEL,	IS_BATTLE,	0,	0,	0,	56'000,	60'000,	9,	92,	97},
		

		/* ------------------------------------
			DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

		{  19,	"Fierce",	AE_PERCENT_DAMAGE,	0,	 22,	 25,	0,	0,	0,	0,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   200,	2,	0,	0 },
		{  20,	"Brutal",	AE_PERCENT_DAMAGE,	0,	 33,	 37,	0,	0,	0,	0,	  5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   357,	   525,	2,	2,	3 },
		{  21,	"Injuring",	AE_PERCENT_DAMAGE,	0,	 47,	 50,	0,	0,	0,	0,	 10,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   760,	 1'000,	3,	4,	5 },
		{  22,	"Furious",	AE_PERCENT_DAMAGE,	0,	 58,	 62,	0,	0,	0,	0,	 15,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'111,	 1'375,	3,	6,	7 },
		{  23,	"Vicious",	AE_PERCENT_DAMAGE,	0,	 71,	 75,	0,	0,	0,	0,	 20,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'512,	 1'800,	4,	8,	10 },
		{  24,	"Severe",	AE_PERCENT_DAMAGE,	0,	 83,	 87,	0,	0,	0,	0,	 25,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'963,	 2'275,	4,	12,	17 },
		{  25,	"Cruel",	AE_PERCENT_DAMAGE,	0,	 96,	100,	0,	0,	0,	0,	 30,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'464,	 2'800,	5,	21,	24 },
		{  26,	"Sadistic",	AE_PERCENT_DAMAGE,	0,	106,	110,	0,	0,	0,	0,	 35,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'015,	 3'375,	5,	26,	28 },
		{  27,	"Merciless",AE_PERCENT_DAMAGE,	0,	117,	120,	0,	0,	0,	0,	 40,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'616,	 4'000,	6,	29,	32 },
		{  28,	"Insane",	AE_PERCENT_DAMAGE,	0,	128,	130,	0,	0,	0,	0,	 45,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'267,	 4'675,	6,	33,	35 },
		{  29,	"Deadly",	AE_PERCENT_DAMAGE,	0,	138,	140,	0,	0,	0,	0,	 50,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	7,	36,	38 },
		{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	148,	150,	0,	0,	0,	0,	 60,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	13'020,	13'500,	8,	39,	41 },
		{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	151,	155,	0,	0,	0,	0,	110,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	20'000,	20'000,	9,	58,	66 },


		/* ------------------------------------
		TO_HIT + DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

		{ 45,	"Commoner's",	AE_TO_HIT_DAMAGE,	0,	 13,	 15,	0,	0,	 2,	  3,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	   300,	   600,	 2,	  0,	  0 },
		{ 46,	"Recruit's",	AE_TO_HIT_DAMAGE,	0,	 21,	 22,	0,	0,	 4,	  5,	  8,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	 1'080,	 1'950,	 2,	  2,	  3 },
		{ 47,	"Fighter's",	AE_TO_HIT_DAMAGE,	0,	 27,	 30,	0,	0,	 6,	  7,	 14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	 2'040,	 3'200,	 3,	  4,	  5 },
		{ 48,	"Squire's",		AE_TO_HIT_DAMAGE,	0,	 34,	 37,	0,	0,	 8,	 10,	 21,	IA_RANGE | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 3'300,	 4'750,	 4,	 11,	 13 },
		{ 49,	"Soldier's",	AE_TO_HIT_DAMAGE,	0,	 42,	 45,	0,	0,	11,	 12,	 28,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 4'455,	 6'050,	 5,	 16,	 18 },
		{ 50,	"Warrior's",	AE_TO_HIT_DAMAGE,	0,	 47,	 50,	0,	0,	13,	 15,	 34,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 5'760,	 7'500,	 6,	 22,	 24 },
		{ 51,	"Champion's",	AE_TO_HIT_DAMAGE,	0,	 52,	 55,	0,	0,	16,	 17,	 41,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 6'240,	 9'100,	 7,	 25,	 27 },
		{ 52,	"Knight's",		AE_TO_HIT_DAMAGE,	0,	 62,	 65,	0,	0,	18,	 20,	 49,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 7'770,	10'850,	 8,	 28,	 31 },
		{ 53,	"Lord's",		AE_TO_HIT_DAMAGE,	0,	 67,	 70,	0,	0,	21,	 23,	 57,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	 8'325,	12'750,	 9,	 32,	 34 },
		{ 54,	"Baron's",		AE_TO_HIT_DAMAGE,	0,	 72,	 75,	0,	0,	24,	 26,	 66,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	10'080,	16'000,	10,	 39,	 42 },
		{ 54,	"Duke's",		AE_TO_HIT_DAMAGE,	0,	 76,	 79,	0,	0,	28,	 30,	 95,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	25'000,	25'000,	11,	 54,	 60 },
		{ 54,	"King's",		AE_TO_HIT_DAMAGE,	0,	 81,	 85,	0,	0,	34,	 37,	137,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	35'000,	35'000,	13,	 73,	 76 },
		

		/* ------------------------------------
			SPECIES DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

		//undead
		{ 30,	"Consecrated",		AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	 40,	 50,	0,	0,	0,	0,	  3,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 31,	"Pure",				AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	 90,	100,	0,	0,	0,	0,	  8,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
		{ 32,	"Sacred",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	140,	150,	0,	0,	0,	0,	 16,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
		{ 33,	"Hallowed",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	190,	200,	0,	0,	0,	0,	 32,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
		{ 34,	"Holy",				AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	215,	225,	0,	0,	0,	0,	 60,	IA_STAFF | IA_AXE | IA_MACE_2H,					IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
		{ 43,	"Hallowed",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	215,	225,	0,	0,	0,	0,	 62,	IA_MACE_1H | IA_CLAW,							IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
		
		//demon
		{ 35,	"Lunar",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		 40,	 50,	0,	0,	0,	0,	  3,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 36,	"Arcadian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		 90,	100,	0,	0,	0,	0,	  8,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
		{ 37,	"Astral",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		140,	150,	0,	0,	0,	0,	 16,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
		{ 38,	"Elysian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		190,	200,	0,	0,	0,	0,	 32,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
		{ 39,	"Celestial",		AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		215,	225,	0,	0,	0,	0,	 60,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
		{ 43,	"Elysian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		215,	225,	0,	0,	0,	0,	 62,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
		
		//beast
		{ 40,	"Savage",			AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		 40,	 50,	0,	0,	0,	0,	  3,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 41,	"Flaying",			AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		 90,	100,	0,	0,	0,	0,	  8,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
		{ 42,	"Eviscerating",		AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		140,	150,	0,	0,	0,	0,	 16,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
		{ 43,	"Disemboweling",	AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		190,	200,	0,	0,	0,	0,	 32,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
		{ 44,	"Butchering",		AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		215,	225,	0,	0,	0,	0,	 60,	IA_STAFF | IA_SWORD_2H | IA_AXE,				IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
		{ 43,	"Disemboweling",	AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		215,	225,	0,	0,	0,	0,	 62,	IA_SWORD | IA_CLAW,								IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
		

		/* ------------------------------------
					DFE ON JEWELRY
		-------------------------------------*/

		//{  10,	"Neophyte's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 12,	IA_JEWEL,	IS_ALL,	16,	0,	1,	   500,	   500,	2,	 2,	  3 },
		{  11,	"Warden's",		AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 24,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 2'000,	 2'000,	3,	 8,	 10 },
		//{  12,	"Sentinel's",	AE_DFE,	0,	 -3,	 -3,	0,	0,	0,	0,	 38,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 4'000,	 4'000,	4,	15,	 19 },
		{  13,	"Sentinel's",	AE_DFE,	0,	 -2,	 -2,	0,	0,	0,	0,	 56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 6'000,	 6'000,	5,	23,	 27 },
		//{  13,	"Sentinel's",	AE_DFE,	0,	 -5,	 -5,	0,	0,	0,	0,	 80,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 9'000,	 9'000,	5,	33,	 37 },
		{  13,	"Sentinel's",	AE_DFE,	0,	 -3,	 -3,	0,	0,	0,	0,	100,	IA_JEWEL,	IS_ALL,	16,	0,	1,	11'000,	11'000,	6,	48,	 52 },
		//{  13,	"Sentinel's",	AE_DFE,	0,	 -7,	 -7,	0,	0,	0,	0,	120,	IA_JEWEL,	IS_ALL,	16,	0,	1,	14'000,	14'000,	6,	63,	 67 },
		{  13,	"Sentinel's",	AE_DFE,	0,	 -4,	 -5,	0,	0,	0,	0,	130,	IA_JEWEL,	IS_ALL,	16,	0,	1,	17'000,	17'000,	7,	78,	 82 },
		{  13,	"Sentinel's",	AE_DFE,	0,	 -6,	 -7,	0,	0,	0,	0,	160,	IA_JEWEL,	IS_ALL,	16,	0,	1,	21'000,	21'000,	7,	93,	 97 },
		//{  13,	"Sentinel's",	AE_DFE,	0,	-10,	-10,	0,	0,	0,	0,	180,	IA_JEWEL,	IS_ALL,	16,	0,	0,	25'000,	25'000,	8,	99,	120 },
		//expand [wip	]
		//{  13,	"Veteran's",	AE_DFE,	0,	-5,	-5,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
		//{  13,	"Veteran's",	AE_DFE,	0,	-6,	-6,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
		//{  13,	"Veteran's",	AE_DFE,	0,	-7,	-7,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
		//{  13,	"Veteran's",	AE_DFE,	0,	-8,	-8,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },


		/* ------------------------------------
					VARIOUS
		-------------------------------------*/


		{  57,	"Sturdy",	AE_PERCENT_AC,	0,	13,	15,	0,	0,	0,	0,	33,	IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	0,	2'500,	3'000,	10,	22,	25 },

		//{  58,	"Menacing",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	12,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_FLASK,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5,	5 + 2 },

		//{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	25,	IA_HELM,	IS_ALL,	0,	0,	0,	 3'000,	 3'000,	5,	15,	17 }, // they are cursed now!
		//{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	48,	IA_HELM,	IS_ALL,	0,	0,	0,	 7'000,	 7'000,	7,	31,	35 },
		//{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	90,	IA_HELM,	IS_ALL,	0,	0,	0,	13'000,	13'000,	9,	58,	65 },

		//{  60,	"Plentiful",	AE_EXTRA_CHARGES,	0,	2,	2,	0,	0,	0,	0,	 6,	IA_STAFF,	IS_MAGIC,	0,	0,	1,	2'000,	2'000,	2,	 0,	 0 },
		//{  61,	"Bountiful",	AE_EXTRA_CHARGES,	0,	3,	3,	0,	0,	0,	0,	15,	IA_STAFF,	IS_MAGIC,	0,	0,	1,	3'000,	3'000,	3,	10,	12 },


		/* ------------------------------------
				MAGIC RESISTANCES
		-------------------------------------*/

		// fire	
		{ 62,	"Carnelian",		AE_RESIST,	ET_1_FIRE,	  4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_1_FIRE,	  8,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
		{ 63,	"Eudialyte",		AE_RESIST,	ET_1_FIRE,	 13,	 15,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
		{ 63,	"Cardinal",			AE_RESIST,	ET_1_FIRE,	 18,	 20,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
		{ 63,	"Jasper",			AE_RESIST,	ET_1_FIRE,	 23,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
		{ 64,	"Garnet",			AE_RESIST,	ET_1_FIRE,	 28,	 30,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
		{ 64,	"Maroon",			AE_RESIST,	ET_1_FIRE,	 33,	 35,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
		{ 64,	"Rubbelite",		AE_RESIST,	ET_1_FIRE,	 38,	 40,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	51,	59 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_1_FIRE,	 43,	 45,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	61,	67 },
		{ 64,	"Sunstone",			AE_RESIST,	ET_1_FIRE,	 48,	 50,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	72,	79 },
		
		// lite		
		{ 62,	"Carnelian",		AE_RESIST,	ET_2_LIGHTNING,	  4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_2_LIGHTNING,	  8,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
		{ 63,	"Eudialyte",		AE_RESIST,	ET_2_LIGHTNING,	 13,	 15,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
		{ 63,	"Cardinal",			AE_RESIST,	ET_2_LIGHTNING,	 18,	 20,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
		{ 63,	"Jasper",			AE_RESIST,	ET_2_LIGHTNING,	 23,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
		{ 64,	"Garnet",			AE_RESIST,	ET_2_LIGHTNING,	 28,	 30,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
		{ 64,	"Maroon",			AE_RESIST,	ET_2_LIGHTNING,	 33,	 35,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
		{ 64,	"Rubbelite",		AE_RESIST,	ET_2_LIGHTNING,	 38,	 40,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	51,	56 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_2_LIGHTNING,	 43,	 45,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	61,	66 },
		{ 64,	"Sunstone",			AE_RESIST,	ET_2_LIGHTNING,	 48,	 50,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	72,	78 },
		
		// arcane	
		{ 62,	"Carnelian",		AE_RESIST,	ET_3_ARCAN,	  4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_3_ARCAN,	  8,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
		{ 63,	"Eudialyte",		AE_RESIST,	ET_3_ARCAN,	 13,	 15,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
		{ 63,	"Cardinal",			AE_RESIST,	ET_3_ARCAN,	 18,	 20,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
		{ 63,	"Jasper",			AE_RESIST,	ET_3_ARCAN,	 23,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
		{ 64,	"Garnet",			AE_RESIST,	ET_3_ARCAN,	 28,	 30,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
		{ 64,	"Maroon",			AE_RESIST,	ET_3_ARCAN,	 33,	 35,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
		{ 64,	"Rubbelite",		AE_RESIST,	ET_3_ARCAN,	 38,	 40,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	51,	58 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_3_ARCAN,	 43,	 45,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	61,	66 },
		{ 64,	"Sunstone",			AE_RESIST,	ET_3_ARCAN,	 48,	 50,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	72,	77 },
		
		// acid		
		{ 62,	"Carnelian",		AE_RESIST,	ET_4_ACID,	  4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_4_ACID,	  8,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
		{ 63,	"Eudialyte",		AE_RESIST,	ET_4_ACID,	 13,	 15,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
		{ 63,	"Cardinal",			AE_RESIST,	ET_4_ACID,	 18,	 20,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
		{ 63,	"Jasper",			AE_RESIST,	ET_4_ACID,	 23,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
		{ 64,	"Garnet",			AE_RESIST,	ET_4_ACID,	 28,	 30,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
		{ 64,	"Maroon",			AE_RESIST,	ET_4_ACID,	 33,	 35,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
		{ 64,	"Rubbelite",		AE_RESIST,	ET_4_ACID,	 38,	 40,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	51,	58 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_4_ACID,	 43,	 45,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	61,	66 },
		{ 64,	"Sunstone",			AE_RESIST,	ET_4_ACID,	 48,	 50,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	72,	77 },

		// cold	
		{ 62,	"Carnelian",		AE_RESIST,	ET_6_COLD,	  4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_6_COLD,	  8,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
		{ 63,	"Eudialyte",		AE_RESIST,	ET_6_COLD,	 13,	 15,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
		{ 63,	"Cardinal",			AE_RESIST,	ET_6_COLD,	 18,	 20,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
		{ 63,	"Jasper",			AE_RESIST,	ET_6_COLD,	 23,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
		{ 64,	"Garnet",			AE_RESIST,	ET_6_COLD,	 28,	 30,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
		{ 64,	"Maroon",			AE_RESIST,	ET_6_COLD,	 33,	 35,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
		{ 64,	"Rubbelite",		AE_RESIST,	ET_6_COLD,	 38,	 40,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	51,	55 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_6_COLD,	 43,	 45,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	61,	65 },
		{ 64,	"Sunstone",			AE_RESIST,	ET_6_COLD,	 48,	 50,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	70,	76 },
		
		// all	
		{ 62,	"Andratite",	AE_RESIST_All,	0,	 1,	 2,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	    100,	 2,	  0,	  0 },
		{ 62,	"Clinohumite",	AE_RESIST_All,	0,	 3,	 4,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	    600,	 2,	  0,	  0 },
		{ 63,	"Agate",		AE_RESIST_All,	0,	 5,	 6,	0,	0,	0,	0,	 17,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  3'000,	 3,	  2,	  3 },
		{ 63,	"Andesine",		AE_RESIST_All,	0,	 7,	 8,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	  4'000,	 3,	 12,	 15 },
		{ 63,	"Ametrine",		AE_RESIST_All,	0,	 9,	10,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	  7'000,	 4,	 24,	 27 },
		{ 64,	"Topaz",		AE_RESIST_All,	0,	11,	12,	0,	0,	0,	0,	 82,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	 13'000,	 5,	 45,	 49 },
		{ 64,	"Citrine",		AE_RESIST_All,	0,	12,	13,	0,	0,	0,	0,	105,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	 17'000,	 6,	 59,	 61 },
		{ 64,	"Spessarite",	AE_RESIST_All,	0,	14,	15,	0,	0,	0,	0,	125,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	 25'000,	 7,	 65,	 76 },
		

		/* ------------------------------------
					VARIOUS
		-------------------------------------*/

		// life steak
		//{ 83,	"Hungry",	AE_LIFE_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
		//{ 83,	"Hungry",	AE_LIFE_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
		//{ 83,	"Hungry",	AE_LIFE_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	61,	63 },

		// mana steal
		//{ 85,	"Vampiric",	AE_MANA_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
		//{ 85,	"Vampiric",	AE_MANA_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
		//{ 85,	"Vampiric",	AE_MANA_STEAL_1_2,	0,	5,	5,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	61,	63 },

	{ 86,	"Curing",	AE_LIFE_REGEN_PERCENT,	0,	34,	 37,	0,	0,	0,	0,	12,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_TRAP,	IS_BATTLE,	0,	0,	1,	 1'750,	 2'250,	3,	 0,	 2 },
	{ 87,	"Healing",	AE_LIFE_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	28,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	 5'000,	 7'500,	4,	15,	18 },
	{ 87,	"Healing",	AE_LIFE_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	54,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	 8'000,	10'000,	5,	25,	31 },
	{ 87,	"Healing",	AE_LIFE_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	94,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	12'000,	15'000,	6,	53,	56 },

	{ 88,	"Refilling",	AE_MANA_REGEN_PERCENT,	0,	34,	 37,	0,	0,	0,	0,	12,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_TRAP,	IS_MAGIC,	0,	0,	1,	1'750,	 2'250,	3,	 0,	 2 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	28,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	5'000,	 7'500,	4,	15,	18 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	54,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	8'000,	10'000,	5,	30,	34 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	47,	 50,	0,	0,	0,	0,	94,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	12'000,	15'000,	6,	51,	52 },

		// what's up with this '2' in curse field???
	//{ 90,	"Boar's",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	25,	IA_RANGE,	IS_BATTLE,	1,	0,	2,	2'000,	2'000,	 6,	13,	16 },
	//{ 90,	"Boar's",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	48,	IA_RANGE,	IS_BATTLE,	1,	0,	2,	4'000,	4'000,	 8,	31,	33 },
	//{ 90,	"Boar's",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	94,	IA_RANGE,	IS_BATTLE,	1,	0,	2,	8'000,	8'000,	11,	57,	61 },

	// + damage on jewelry
	{ 91,	"Damaging",	AE_DAMAGE,	0,	 1,	  1,	0,	0,	0,	0,	  4,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	    50,	    50,	 1,  0,	 0 },
	{ 92,	"Tearing",	AE_DAMAGE,	0,	 2,	  2,	0,	0,	0,	0,	  8,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 2 },
	{ 92,	"Tearing",	AE_DAMAGE,	0,	 3,	  3,	0,	0,	0,	0,	 12,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   450,	   450,	 2,	 0,	 2 },
	{ 92,	"Tearing",	AE_DAMAGE,	0,	 4,	  4,	0,	0,	0,	0,	 16,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   600,	   750,	 2,	 3,	 5 },
	{ 93,	"Pounding",	AE_DAMAGE,	0,	 5,	  5,	0,	0,	0,	0,	 20,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 1'200,	 2'000,	 3,	 6,	 8 },
	{ 93,	"Spiked",	AE_DAMAGE,	0,	 6,	  7,	0,	0,	0,	0,	 25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 2'200,	 3'000,	 4,	11,	14 },
	{ 94,	"Smashing",	AE_DAMAGE,	0,	 8,	 10,	0,	0,	0,	0,	 30,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 3'200,	 4'000,	 5,	20,	22 },
	{ 94,	"Smashing",	AE_DAMAGE,	0,	11,	 12,	0,	0,	0,	0,	 35,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 4'200,	 5'000,	 6,	23,	25 },
	{ 94,	"Smashing",	AE_DAMAGE,	0,	13,	 15,	0,	0,	0,	0,	 40,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 5'200,	 6'000,	 6,	26,	29 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	16,	 17,	0,	0,	0,	0,	 45,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 6'200,	 7'000,	 7,	31,	33 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	18,	 20,	0,	0,	0,	0,	 50,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 7'200,	 8'000,	 8,	34,	37 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	21,	 22,	0,	0,	0,	0,	 55,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 8'200,	 9'000,	 9,	38,	40 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	23,	 25,	0,	0,	0,	0,	 60,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 9'200,	10'000,	10,	41,	42 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	26,	 27,	0,	0,	0,	0,	 65,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	10'200,	11'000,	11,	45,	47 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	28,	 30,	0,	0,	0,	0,	 75,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	11'200,	12'000,	11,	45,	47 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	31,	 32,	0,	0,	0,	0,	 85,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	12'200,	13'000,	11,	48,	52 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	33,	 35,	0,	0,	0,	0,	 95,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	13'200,	14'000,	12,	53,	56 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	36,	 37,	0,	0,	0,	0,	105,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	14'200,	15'000,	13,	57,	60 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	38,	 40,	0,	0,	0,	0,	115,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	15'200,	16'000,	13,	61,	70 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	41,	 42,	0,	0,	0,	0,	125,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	16'200,	17'000,	14,	71,	75 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	43,	 45,	0,	0,	0,	0,	135,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	17'200,	18'000,	14,	76,	80 },
	{ 95,	"Damaging",	AE_DAMAGE,	0,	46,	 47,	0,	0,	0,	0,	150,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	18'200,	19'000,	14,	87,	95 },
	
	// ac in jewelry
	//{ 96,	"Protective",	AE_SPECIAL_AC,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_JEWEL,	IS_ALL,	0,	0,	1,	    50,	    50,	1,	 0,	  0 },
	{ 97,	"Protective",	AE_SPECIAL_AC,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	   100,	   200,	2,	 0,	  0 },
	{ 98,	"Reliant",		AE_SPECIAL_AC,	0,	 2,	 2,	0,	0,	0,	0,	  8,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	   300,	   500,	3,	 2,	  3 },
	{ 99,	"Shielding",	AE_SPECIAL_AC,	0,	 3,	 4,	0,	0,	0,	0,	 16,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	   900,	 1'350,	4,	 4,	  6 },
	{ 100,	"Covering",		AE_SPECIAL_AC,	0,	 5,	 7,	0,	0,	0,	0,	 29,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	 2'000,	 2'800,	5,	11,	 13 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	 8,	10,	0,	0,	0,	0,	 61,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	 4'500,	 6'000,	6,	28,	 33 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	11,	12,	0,	0,	0,	0,	 91,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	 8'400,	10'000,	7,	50,	 55 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	13,	15,	0,	0,	0,	0,	133,	IA_STAFF | IA_JEWEL,	IS_ALL,	0,	0,	1,	13'000,	15'000,	8,	66,	 75 },
	//{ 101,	"Armored",		AE_SPECIAL_AC,	0,	31,	35,	0,	0,	0,	0,	177,	IA_JEWEL,	IS_ALL,	0,	0,	0,	31'000,	35'000,	9,	93,	111 },


	/* ------------------------------------
		ELEMENTAL DAMAGE ON WEAPONS
	-------------------------------------*/

	//{ 102,	"Warm",			AE_FIRE_HIT_DAMAGE,	0,	 1,	 2,	 3,	 4,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 103,	"Melting",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 1,	 2,	 3,	 4,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 104,	"Hot",			AE_ELEM_HIT,	EA_MELEE_FIRE,	 3,	 4,	 6,	 7,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 105,	"Candent",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 5,	 7,	 8,	10,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 106,	"Burning",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 6,	 8,	11,	13,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	12,	16,	22,	26,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	12,	16,	22,	26,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 108,	"Static",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 109,	"Electric",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 2,	3,	5,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 110,	"Charged",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 3,	9,	10,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 111,	"Shocking",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 2,	 4,	14,	15,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 112,	"Lightning",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 3,	5,	18,	21,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 113,	"Thunder",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	7,	11,	37,	41,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 113,	"Thunder",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	7,	11,	37,	41,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 114,	"Chilling",	AE_ELEM_HIT,	EA_MELEE_COLD,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 115,	"Snow",		AE_ELEM_HIT,	EA_MELEE_COLD,	 1,	 2,	 4,	5,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 116,	"Icy",		AE_ELEM_HIT,	EA_MELEE_COLD,	 2,	 3,	9,	10,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 117,	"Winter",	AE_ELEM_HIT,	EA_MELEE_COLD,	 3,	 4,	13,	15,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 118,	"Glacial",	AE_ELEM_HIT,	EA_MELEE_COLD,	 4,	6,	17,	20,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	12,	16,	32,	36,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	12,	16,	32,	36,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 120,	"Cleansing",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 121,	"Purifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 1,	 2,	 4,	5,	0,	0,	 4,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 122,	"Cleric's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 2,	 3,	9,	10,	0,	0,	10,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 123,	"Priest's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 3,	 4,	14,	15,	0,	0,	24,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 124,	"Purging",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 4,	7,	18,	20,	0,	0,	55,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	12,	16,	32,	36,	0,	0,	62,	IA_STAFF | IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	12,	16,	32,	36,	0,	0,	99,	IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 126,	"Voiding",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	 1,	 1,	 2,	 3,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 127,	"Depleting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 1,	 2,	10,	11,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 128,	"Exhausting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 3,	 4,	14,	15,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 129,	"Devastating",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 5,	 8,	16,	19,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	11,	13,	26,	31,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	11,	13,	26,	31,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 131,	"Rancid",			AE_ELEM_HIT,	EA_MELEE_ACID,	 1,	 1,	 2,	 3,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 132,	"Toxic",			AE_ELEM_HIT,	EA_MELEE_ACID,	 1,	 2,	 8,	10,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 133,	"Diluent",			AE_ELEM_HIT,	EA_MELEE_ACID,	 3,	 5,	11,	12,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 134,	"Corrodent",		AE_ELEM_HIT,	EA_MELEE_ACID,	 6,	 7,	13,	15,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	 9,	12,	24,	28,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	 9,	12,	24,	28,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },


	/* ------------------------------------
				VARIOUS
	-------------------------------------*/

	//{ 136,	"Draining",	AE_LIFE_STEALING_1_2_RND,	0,	0,	0,	0,	0,	0,	0,	6,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	0,	800,	800,	7,	4,	6 },

	//{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	35,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET,			IS_ALL,	0,	0,	1,	3'000,	3'000,	 7,	17,	19 },
	//{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	68,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	6'000,	6'000,	11,	27,	33 },
	//{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	90,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	9'000,	9'000,	13,	66,	77 },

	//{ 180,	"Crafted",			AE_HIGH_DURABILITY,	0,	105,	210,	0,	0,	0,	0,	 1,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	    50,	   100,	1,	 0,	 0 },
	//{ 181,	"Structured",		AE_HIGH_DURABILITY,	0,	100,	200,	0,	0,	0,	0,	 3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	   200,	   300,	2,	 0,	 0 },
	//{ 181,	"Structured",		AE_HIGH_DURABILITY,	0,	 95,	190,	0,	0,	0,	0,	 7,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	   500,	 1'000,	2,	 0,	 0 },
	//{ 181,	"Tempered",			AE_HIGH_DURABILITY,	0,	 90,	180,	0,	0,	0,	0,	13,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	 2'000,	 4'000,	3,	 3,	 4 },
	//{ 181,	"Exquisite",		AE_HIGH_DURABILITY,	0,	 85,	170,	0,	0,	0,	0,	26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	 5'000,	 7'000,	3,	16,	20 },
	//{ 181,	"Strengthened",		AE_HIGH_DURABILITY,	0,	 80,	160,	0,	0,	0,	0,	52,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	 8'000,	12'000,	4,	30,	34 },
	//{ 181,	"Masterwork",		AE_HIGH_DURABILITY,	0,	 75,	150,	0,	0,	0,	0,	87,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	16'000,	24'000,	6,	50,	55 },


	/* ------------------------------------
				XP PERCENT
	-------------------------------------*/

	{ 139,	"Curious",			AE_XP_GAIN_PERCENT,	0,	 3,	 4,	0,	0,	0,	0,	 7,	IA_JEWEL,	IS_ALL,	0,	0,	1,	   300,	   400,	2,	 0,	 0 },
	{ 140,	"Studying",			AE_XP_GAIN_PERCENT,	0,	 5,	 5,	0,	0,	0,	0,	13,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 1'000,	 1'200,	3,	 3,	 4 },
	{ 141,	"Illuminating",		AE_XP_GAIN_PERCENT,	0,	 6,	 6,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 3'500,	 4'000,	4,	15,	16 },
	{ 141,	"Contemplating",	AE_XP_GAIN_PERCENT,	0,	 7,	 7,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_ALL,	0,	0,	1,	18'000,	20'000,	5,	33,	36 },
	//{ 141,	"Clarifying",		AE_XP_GAIN_PERCENT,	0,	11,	12,	0,	0,	0,	0,	70,	IA_JEWEL,	IS_ALL,	0,	0,	0,	22'000,	24'000,	5,	45,	50 },


	/* ------------------------------------
				GOLD FIND PERCENT
	-------------------------------------*/

	{ 142,	"Rapacious",	AE_GOLD_FIND,	0,	 8,	10,	0,	0,	0,	0,	11,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	 1'200,	2,	 2,	 3 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	11,	12,	0,	0,	0,	0,	24,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	 4'200,	 5'000,	3,	15,	18 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	13,	15,	0,	0,	0,	0,	46,	IA_JEWEL | IA_ARMOR,	IS_ALL,	0,	0,	1,	10'000,	12'000,	4,	30,	35 },


	/* ------------------------------------
		MELEE & ARROW DAMAGE RESISTANCE
	-------------------------------------*/

	//MeleeDR
//{ 144,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	  0,	  0 },
{ 145,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  4,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   150,	   300,	 2,	  0,	  0 },
{ 145,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 2,	 3,	0,	0,	0,	0,	 10,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   500,	   600,	 3,	  2,	  3 },
{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	 3,	 4,	0,	0,	0,	0,	 16,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	 1'000,	 1'200,	 4,	  4,	  5 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 27,	IA_BODY_ARMOR | IA_HELM,		IS_ALL,		0,	0,	1,	 4'000,	 4'800,	 6,	 14,	 16 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 4,	 5,	0,	0,	0,	0,	 38,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 5'000,	 6'000,	 6,	 28,	 31 },
//shields
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	4,	5,	0,	0,	0,	0,	 73,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	 6,	 45,	 46 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	5,	6,	0,	0,	0,	0,	 93,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	11'000,	12'000,	 7,	 60,	 61 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	6,	7,	0,	0,	0,	0,	113,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	13'000,	14'000,	 8,	 75,	 77 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	7,	8,	0,	0,	0,	0,	133,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	15'000,	16'000,	 9,	 83,	 85 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	8,	9,	0,	0,	0,	0,	153,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	17'000,	18'000,	10,	 90,	 93 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	9,	10,	0,	0,	0,	0,	174,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	19'000,	20'000,	11,	103,	105 },
//{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	181,	IA_SHIELD,						IS_BATTLE,	0,	0,	0,	21'000,	25'000,	13,	109,	125 },
//helms
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 6,	 7,	0,	0,	0,	0,	 75,	IA_HELM,						IS_ALL,		0,	0,	1,	13'000,	15'000,	 7,	 46,	 48 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 8,	 9,	0,	0,	0,	0,	 95,	IA_HELM,						IS_ALL,		0,	0,	1,	16'000,	18'000,	 8,	 61,	 63 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 9,	10,	0,	0,	0,	0,	115,	IA_HELM,						IS_BATTLE,	0,	0,	1,	19'000,	21'000,	 9,	 76,	 78 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	135,	IA_HELM,						IS_BATTLE,	0,	0,	1,	22'000,	24'000,	10,	 84,	 87 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	12,	13,	0,	0,	0,	0,	155,	IA_HELM,						IS_BATTLE,	0,	0,	1,	25'000,	27'000,	11,	 91,	 94 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	14,	15,	0,	0,	0,	0,	176,	IA_HELM,						IS_BATTLE,	0,	0,	1,	28'000,	30'000,	12,	104,	107 },
//{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	183,	IA_HELM,						IS_BATTLE,	0,	0,	0,	31'000,	35'000,	14,	108,	126 },
//boots/gloves/belt
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 80,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	 47,	 49 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 6,	 7,	0,	0,	0,	0,	100,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	13'000,	14'000,	 9,	 62,	 65 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 7,	 8,	0,	0,	0,	0,	120,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	15'000,	16'000,	10,	 77,	 80 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 8,	 9,	0,	0,	0,	0,	140,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	17'000,	19'000,	11,	 85,	 89 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	10,	11,	0,	0,	0,	0,	160,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	20'000,	22'000,	12,	 93,	 97 },
{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	180,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	23'000,	25'000,	13,	106,	109 },
//{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	182,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	0,	26'000,	30'000,	15,	111,	129 },
//harness
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 6,	 8,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	10'500,	12'000,	 8,	 33,	 35 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 8,	10,	0,	0,	0,	0,	 62,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	17'500,	20'000,	 8,	 40,	 43 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 85,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	21'000,	25'000,	 9,	 48,	 52 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	13,	15,	0,	0,	0,	0,	105,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	26'000,	30'000,	10,	 64,	 67 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	16,	17,	0,	0,	0,	0,	125,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	31'000,	36'000,	11,	 79,	 82 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	18,	20,	0,	0,	0,	0,	145,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	36'000,	40'000,	12,	 87,	 90 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	21,	22,	0,	0,	0,	0,	165,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	41'000,	45'000,	13,	 95,	 99 },
{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	23,	25,	0,	0,	0,	0,	185,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	46'000,	50'000,	14,	108,	113 },
//{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	51,	60,	0,	0,	0,	0,	184,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	0,	51'000,	60'000,	14,	116,	130 },

// ArrowDR
	//{ 144,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	  0,	  0 },
{ 145,	"Negating",		AE_ABSORB_RANGED_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  4,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   150,	   300,	 2,	  0,	  0 },
{ 145,	"Negating",		AE_ABSORB_RANGED_DAMAGE,	0,	 2,	 3,	0,	0,	0,	0,	 10,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   500,	   600,	 3,	  2,	  3 },
{ 146,	"Vindicative",	AE_ABSORB_RANGED_DAMAGE,	0,	 3,	 4,	0,	0,	0,	0,	 16,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	 1'000,	 1'200,	 4,	  4,	  5 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 27,	IA_BODY_ARMOR | IA_HELM,		IS_ALL,		0,	0,	1,	 4'000,	 4'800,	 6,	 14,	 16 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	 4,	 5,	0,	0,	0,	0,	 38,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 5'000,	 6'000,	 6,	 28,	 31 },
	//shields
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 4,	 5,	0,	0,	0,	0,	 73,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	 6,	 45,	 46 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 93,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	11'000,	12'000,	 7,	 53,	 56 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	6,	7,	0,	0,	0,	0,	113,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	13'000,	14'000,	 8,	 60,	 65 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	7,	8,	0,	0,	0,	0,	133,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	15'000,	16'000,	 9,	 71,	 75 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	8,	9,	0,	0,	0,	0,	153,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	17'000,	18'000,	10,	 86,	 93 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	9,	10,	0,	0,	0,	0,	174,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	19'000,	20'000,	11,	100,	105 },
	//{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	181,	IA_SHIELD,						IS_BATTLE,	0,	0,	0,	21'000,	25'000,	13,	109,	125 },
	//helms
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	6,	7,	0,	0,	0,	0,	 75,	IA_HELM,						IS_ALL,		0,	0,	1,	13'000,	15'000,	 7,	 46,	 48 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	8,	9,	0,	0,	0,	0,	 95,	IA_HELM,						IS_ALL,		0,	0,	1,	16'000,	18'000,	 8,	 53,	 56 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	9,	10,	0,	0,	0,	0,	115,	IA_HELM,						IS_BATTLE,	0,	0,	1,	19'000,	21'000,	 9,	 60,	 65 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	135,	IA_HELM,						IS_BATTLE,	0,	0,	1,	22'000,	24'000,	10,	 72,	 76 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	12,	13,	0,	0,	0,	0,	155,	IA_HELM,						IS_BATTLE,	0,	0,	1,	25'000,	27'000,	11,	 87,	 94 },
{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	14,	15,	0,	0,	0,	0,	176,	IA_HELM,						IS_BATTLE,	0,	0,	1,	28'000,	30'000,	12,	104,	107 },
	//{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	183,	IA_HELM,						IS_BATTLE,	0,	0,	0,	31'000,	35'000,	14,	108,	126 },
	//boots/gloves/belt
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	5,	6,	0,	0,	0,	0,	 80,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	 47,	 49 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	6,	7,	0,	0,	0,	0,	100,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	13'000,	14'000,	 9,	 54,	 61 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	7,	8,	0,	0,	0,	0,	120,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	15'000,	16'000,	10,	 70,	 75 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	8,	9,	0,	0,	0,	0,	140,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	17'000,	19'000,	11,	 80,	 85 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	10,	11,	0,	0,	0,	0,	160,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	20'000,	22'000,	12,	 87,	 90 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	180,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	23'000,	25'000,	13,	106,	109 },
	//{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	182,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	0,	26'000,	30'000,	15,	111,	129 },
	//harness
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	 6,	 8,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	10'500,	12'000,	 8,	 33,	 35 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	 8,	10,	0,	0,	0,	0,	 62,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	17'500,	20'000,	 8,	 40,	 43 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 85,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	21'000,	25'000,	 9,	 48,	 52 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	13,	15,	0,	0,	0,	0,	105,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	26'000,	30'000,	10,	 58,	 62 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	16,	17,	0,	0,	0,	0,	125,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	31'000,	36'000,	11,	 77,	 80 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	18,	20,	0,	0,	0,	0,	145,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	36'000,	40'000,	12,	 87,	 90 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	21,	22,	0,	0,	0,	0,	165,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	41'000,	45'000,	13,	 95,	 99 },
{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	23,	25,	0,	0,	0,	0,	185,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	46'000,	50'000,	14,	108,	113 },
	//{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	51,	60,	0,	0,	0,	0,	184,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	0,	51'000,	60'000,	14,	116,	130 },



/* ------------------------------------
			SUMMON BOOSTS
-------------------------------------*/

//{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 1,	 3,	 1,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	    40,	   120,	1,	 0,	 0 },
{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 1,	 2,	 3,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   150,	   200,	1,	 0,	 0 },
{ 157,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 3,	 4,	 7,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   500,	   800,	2,	 3,	 5 },
{ 158,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 5,	7,	13,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 1'800,	 2'800,	5,	 6,	 8 },
{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	8,	10,	25,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 7'500,	10'000,	6,	13,	17 },
{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	8,	10,	47,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	16'500,	17'000,	7,	23,	28 },
{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	8,	10,	93,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	22'500,	25'000,	8,	50,	58 },

{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	7,	9,	0,	0,	0,	0,	 1,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	7,	9,	0,	0,	0,	0,	 3,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
{ 161,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	 7,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
{ 162,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	12,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 2'000,	 3'500,	4,	 6,	 8 },
{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	25,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 7'000,	 8'500,	6,	13,	17 },
{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	47,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	14'000,	15'500,	7,	23,	28 },
{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	93,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	26'000,	27'500,	8,	51,	56 },

{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	7,	9,	0,	0,	0,	0,	 1,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	7,	9,	0,	0,	0,	0,	 3,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
{ 165,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	10,	12,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
{ 166,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	10,	12,	0,	0,	0,	0,	12,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 2'000,	 2'500,	4,	 6,	 8 },
{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	10,	12,	0,	0,	0,	0,	25,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	6,	13,	17 },
{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	10,	12,	0,	0,	0,	0,	47,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	11'000,	13'500,	7,	23,	28 },
{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	10,	12,	0,	0,	0,	0,	93,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	18'000,	19'500,	8,	51,	56 },


	/* ------------------------------------
			+ ARMOR VS SPECIES
	-------------------------------------*/

	{ 168,	"Shimmering",	AE_SPICIES_AC,	MON_0_UNDEAD,	5,	7,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 169,	"Shining",		AE_SPICIES_AC,	MON_0_UNDEAD,	9,	12,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 170,	"Coruscating",	AE_SPICIES_AC,	MON_0_UNDEAD,	13,	17,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 171,	"Radiant",		AE_SPICIES_AC,	MON_0_UNDEAD,	18,	22,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 171,	"Luminous",		AE_SPICIES_AC,	MON_0_UNDEAD,	23,	30,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },

	{ 172,	"Blessed",		AE_SPICIES_AC,	MON_1_DEMON,		5,	7,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 173,	"Saintly",		AE_SPICIES_AC,	MON_1_DEMON,		9,	12,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 174,	"Divine",		AE_SPICIES_AC,	MON_1_DEMON,		13,	17,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 175,	"Godly",		AE_SPICIES_AC,	MON_1_DEMON,		18,	22,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 175,	"Sanctified",	AE_SPICIES_AC,	MON_1_DEMON,		23,	30,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	47,	57 },

	{ 176,	"Fine",			AE_SPICIES_AC,	MON_2_BEAST,		5,	7,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 177,	"Grand",		AE_SPICIES_AC,	MON_2_BEAST,		9,	12,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 178,	"Valiant",		AE_SPICIES_AC,	MON_2_BEAST,		13,	17,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 179,	"Glorious",		AE_SPICIES_AC,	MON_2_BEAST,		16,	22,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 179,	"Awesome",		AE_SPICIES_AC,	MON_2_BEAST,		23,	30,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },


	/* ------------------------------------
				THORNS PERCENT
	-------------------------------------*/

	{ 182,	"Thorned",	AE_THORNS_PERCENT,	0,	 18,	 20,	0,	0,	0,	0,	  2,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	{ 183,	"Thorned",	AE_THORNS_PERCENT,	0,	 22,	 25,	0,	0,	0,	0,	  6,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   300,	   300,	 2,	  2,	  3 },
	{ 184,	"Thorned",	AE_THORNS_PERCENT,	0,	 27,	 30,	0,	0,	0,	0,	 12,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   800,	   800,	 3,	  4,	  5 },
	{ 185,	"Thorned",	AE_THORNS_PERCENT,	0,	 32,	 35,	0,	0,	0,	0,	 24,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'500,	 1'500,	 4,	 14,	 16 },
	{ 186,	"Thorned",	AE_THORNS_PERCENT,	0,	 37,	 40,	0,	0,	0,	0,	 32,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'100,	 3'100,	 5,	 20,	 22 },
	{ 187,	"Thorned",	AE_THORNS_PERCENT,	0,	 42,	 45,	0,	0,	0,	0,	 48,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 5'100,	 5'100,	 6,	 26,	 28 },
	{ 188,	"Thorned",	AE_THORNS_PERCENT,	0,	 47,	 50,	0,	0,	0,	0,	 56,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 7'100,	 7'100,	 7,	 32,	 34 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	 52,	 60,	0,	0,	0,	0,	 62,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	11'100,	11'100,	 8,	 38,	 40 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	 62,	 70,	0,	0,	0,	0,	 98,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	24'200,	28'000,	12,	 55,	 59 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	141,	150,	0,	0,	0,	0,	178,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	0,	56'400,	60'000,	16,	100,	121 },

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

				//{ 190,	"Quality",	AE_DAMAGE,	0,	  1,	  1,	0,	0,	0,	0,	  1,	IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET,					IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
				//{ 191,	"Quality",	AE_DAMAGE,	0,	  2,	  3,	0,	0,	0,	0,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET,		IS_BATTLE,	0,	0,	1,	   200,	   300,	1,	 0,  0 },
				{ 192,	"Mess",		AE_DAMAGE,	0,	  2,	  2,	0,	0,	0,	0,	  5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   400,	   800,	2,	 2,	 3 },
				{ 193,	"Maiming",	AE_DAMAGE,	0,	  3,	  4,	0,	0,	0,	0,	  8,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   900,	 1'500,	3,	 4,	 6 },
				{ 194,	"Atrocity",	AE_DAMAGE,	0,	  4,	 5,	0,	0,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 1'600,	 2'200,	3,	 7,	 9 },
				{ 195,	"Slaying",	AE_DAMAGE,	0,	 6,	 7,	0,	0,	0,	0,	 14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 2'300,	 3'000,	4,	11,	13 },
				{ 196,	"Gore",		AE_DAMAGE,	0,	 8,	 10,	0,	0,	0,	0,	 20,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 3'100,	 4'000,	5,	15,	17 },
				{ 197,	"Carnage",	AE_DAMAGE,	0,	 11,	 15,	0,	0,	0,	0,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 4'100,	 5'000,	6,	19,	21 },
				{ 198,	"Slaughter",AE_DAMAGE,	0,	 16,	 20,	0,	0,	0,	0,	 32,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 5'100,	 6'500,	6,	23,	26 },
				{ 199,	"Massacre",	AE_DAMAGE,	0,	 21,	 25,	0,	0,	0,	0,	 37,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 6'600,	 8'000,	7,	26,	28 },
				//1H
				{ 200,	"Death",	AE_DAMAGE,	0,	 26,	 30,	0,	0,	0,	0,	 42,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	 8'100,	10'000,	 7,	 29,	 32 },
				{ 201,	"Hatred",	AE_DAMAGE,	0,	 31,	 35,	0,	0,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	10'200,	12'000,	 8,	 33,	 34 },
				{ 202,	"Agony",	AE_DAMAGE,	0,	 36,	 40,	0,	0,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	12'600,	15'000,	 8,	 35,	 37 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	 41,	 50,	0,	0,	0,	0,	 70,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	17'200,	20'000,	 9,	 39,	 43 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	 51,	60,	0,	0,	0,	0,	 90,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	20'200,	24'000,	10,	 48,	 55 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	 61,	70,	0,	0,	0,	0,	110,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	27'225,	31'500,	11,	 60,	 70 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	 71,	80,	0,	0,	0,	0,	130,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	35'250,	40'000,	12,	 75,	 85 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	 81,	90,	0,	0,	0,	0,	155,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	44'275,	49'500,	13,	 90,	100 },
				//{ 203,	"Doom",		AE_DAMAGE,	0,	 181,	200,	0,	0,	0,	0,	182,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	0,	54'300,	60'000,	15,	101,	120 },
				//2H
				{ 204,	"Death",	AE_DAMAGE,	0,	 26,	 40,	0,	0,	0,	0,	 42,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	 8'100,	10'000,	 7,	 29,	 32 },
				{ 205,	"Hatred",	AE_DAMAGE,	0,	 41,	57,	0,	0,	0,	0,	 46,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	10'200,	12'000,	 8,	 33,	 34 },
				{ 206,	"Agony",	AE_DAMAGE,	0,	58,	 77,	0,	0,	0,	0,	 50,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	12'600,	15'000,	 8,	 35,	 37 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	78,	 100,	0,	0,	0,	0,	 70,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	17'200,	20'000,	 9,	 39,	 43 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	101,	120,	0,	0,	0,	0,	 90,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	20'200,	24'000,	10,	 48,	 55 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	121,	140,	0,	0,	0,	0,	111,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	27'225,	31'500,	11,	 60,	 70 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	141,	160,	0,	0,	0,	0,	130,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	35'250,	40'000,	12,	 75,	 85 },
				{ 203,	"Doom",		AE_DAMAGE,	0,	161,	180,	0,	0,	0,	0,	155,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	44'275,	49'500,	13,	 90,	100 },
				//{ 203,	"Doom",		AE_DAMAGE,	0,	361,	400,	0,	0,	0,	0,	182,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H,							IS_BATTLE,	0,	0,	0,	54'300,	60'000,	15,	101,	120 },


				/* ------------------------------------
						+ DAMAGE VS SPECIES
				-------------------------------------*/

				//undead
			{ 208,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	  5,	  6,	  7,	  8,	0,	0,	  8,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
			{ 209,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	  8,	 10,	 12,	 14,	0,	0,	 14,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
			{ 210,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 14,	 16,	 18,	 20,	0,	0,	 24,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
			{ 211,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 20,	 25,	 30,	 35,	0,	0,	 37,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
			{ 212,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 35,	 40,	 45,	 50,	0,	0,	 42,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
			//1h
			{ 213,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 55,	 60,	 65,	 70,	0,	0,	 46,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
			{ 214,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 70,	 75,	 80,	 85,	0,	0,	 50,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
			{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 85,	 90,	 95,	100,	0,	0,	 57,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
			{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	101,	105,	110,	115,	0,	0,	 75,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
			{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	115,	120,	125,	130,	0,	0,	 95,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
			{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	121,	137,	142,	150,	0,	0,	115,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
			//{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	151,	157,	165,	175,	0,	0,	135,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
			//{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	176,	185,	190,	200,	0,	0,	155,	IA_STAFF | IA_MACE_1H,				IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
			//2h
			{ 216,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 62,	 75,	 87,	100,	0,	0,	 46,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
			{ 217,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	112,	125,	137,	150,	0,	0,	 50,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
			{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	162,	175,	187,	200,	0,	0,	 57,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
			{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	201,	210,	220,	230,	0,	0,	 75,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
			{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	231,	245,	255,	267,	0,	0,	 95,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
			{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	268,	280,	290,	300,	0,	0,	115,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
			//{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	301,	316,	333,	350,	0,	0,	138,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
			//{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	351,	365,	383,	400,	0,	0,	158,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },

			//demons
		{ 219,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 5,	  6,	  7,	  8,	0,	0,	  8,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
		{ 220,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 8,	 10,	 12,	 14,	0,	0,	 14,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
		{ 221,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		14,	 16,	 18,	 20,	0,	0,	 24,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
		{ 222,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		20,	 25,	 30,	 35,	0,	0,	 37,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
		{ 223,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		35,	 40,	 45,	 50,	0,	0,	 42,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
		//1h
		{ 224,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 55,	 60,	 65,	 70,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
		{ 225,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 70,	 75,	 80,	 85,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
		{ 226,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 85,	 90,	 95,	100,	0,	0,	 57,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
		{ 226,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		101,	105,	110,	115,	0,	0,	 75,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
		{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_1_DEMON,		115,	120,	125,	130,	0,	0,	 95,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
		{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_1_DEMON,		121,	137,	142,	150,	0,	0,	115,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
		//{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_1_DEMON,		301,	315,	330,	350,	0,	0,	135,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
		//{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_1_DEMON,		351,	370,	380,	400,	0,	0,	154,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H,			IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
		//2h
		{ 227,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 62,	 75,	 87,	100,	0,	0,	 46,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
		{ 228,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		112,	125,	137,	150,	0,	0,	 50,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
		{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		162,	175,	187,	200,	0,	0,	 57,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
		{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		201,	210,	220,	230,	0,	0,	 75,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
		{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		231,	245,	255,	267,	0,	0,	 95,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
		{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		268,	280,	290,	300,	0,	0,	115,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
		//{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		601,	633,	666,	700,	0,	0,	135,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
		//{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		701,	733,	766,	800,	0,	0,	156,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },

		//beasts
	{ 230,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		  5,	  6,	  7,	  8,	0,	0,	  8,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
	{ 231,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		  8,	 10,	 12,	 14,	0,	0,	 14,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
	{ 232,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 14,	 16,	 18,	 20,	0,	0,	 24,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
	{ 233,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 20,	 25,	 30,	 35,	0,	0,	 37,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
	{ 234,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 35,	 40,	 45,	 50,	0,	0,	 42,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
	//1h
	{ 235,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 55,	 60,	 65,	 70,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 236,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 70,	 75,	 80,	 85,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 85,	 90,	 95,	100,	0,	0,	 57,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		101,	105,	110,	115,	0,	0,	 75,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		115,	120,	125,	130,	0,	0,	 95,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		121,	137,	142,	150,	0,	0,	115,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	//{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		301,	315,	330,	350,	0,	0,	137,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	//{ 237,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		351,	370,	380,	400,	0,	0,	157,	IA_STAFF | IA_SWORD_1H,				IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
	//2h
	{ 238,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 62,	 75,	 87,	100,	0,	0,	 46,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 239,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		112,	125,	137,	150,	0,	0,	 50,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		162,	175,	187,	200,	0,	0,	 57,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		201,	210,	220,	230,	0,	0,	 75,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		231,	245,	255,	267,	0,	0,	 95,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		268,	280,	290,	300,	0,	0,	115,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	//{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		601,	633,	666,	700,	0,	0,	135,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	//{ 240,	"Mutilation",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		701,	733,	766,	800,	0,	0,	158,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },


	/* ------------------------------------
					DFE
	-------------------------------------*/

	//shields
//{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  3,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	   200,	   200,	 1,	  0,	  0 },
{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  7,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
//{ 241,	"Protection",	AE_DFE,	0,	-3,	-3,	0,	0,	0,	0,	 12,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 2'000,	 2'000,	 3,	  3,	  4 },
{ 241,	"Protection",	AE_DFE,	0,	-2,	-2,	0,	0,	0,	0,	 18,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 3'000,	 3'000,	 4,	 12,	 15 },
//{ 241,	"Protection",	AE_DFE,	0,	-5,	-5,	0,	0,	0,	0,	 25,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 4'500,	 4'500,	 5,	 16,	 18 },
{ 241,	"Protection",	AE_DFE,	0,	-3,	-3,	0,	0,	0,	0,	 35,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 6'500,	 6'500,	 6,	 22,	 25 },
//{ 241,	"Protection",	AE_DFE,	0,	-7,	-7,	0,	0,	0,	0,	 45,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 9'500,	 9'500,	 7,	 26,	 31 },
{ 241,	"Protection",	AE_DFE,	0,	-4,	-4,	0,	0,	0,	0,	 55,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	13'500,	13'500,	 8,	 33,	 36 },
//{ 241,	"Protection",	AE_DFE,	0,	-9,	-9,	0,	0,	0,	0,	 65,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	18'500,	18'500,	 9,	 37,	 40 },
{ 241,	"Protection",	AE_DFE,	0,	 -5,	 -5,	0,	0,	0,	0,	 75,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	21'500,	21'500,	10,	 45,	 47 },
{ 241,	"Protection",	AE_DFE,	0,	- 5,	- 6,	0,	0,	0,	0,	 90,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	22'000,	24'000,	11,	 48,	 52 },
{ 241,	"Protection",	AE_DFE,	0,	- 6,	- 7,	0,	0,	0,	0,	105,	IA_SHIELD,									IS_BATTLE,	0,	0,	1,	26'000,	28'000,	12,	 55,	 60 },
{ 241,	"Protection",	AE_DFE,	0,	- 7,	- 8,	0,	0,	0,	0,	125,	IA_SHIELD,									IS_BATTLE,	0,	0,	1,	30'000,	32'000,	13,	 65,	 70 },
{ 241,	"Protection",	AE_DFE,	0,	- 8,	- 9,	0,	0,	0,	0,	150,	IA_SHIELD,									IS_BATTLE,	0,	0,	1,	34'000,	36'000,	14,	 75,	 80 },
{ 241,	"Protection",	AE_DFE,	0,	- 9,	-10,	0,	0,	0,	0,	180,	IA_SHIELD,									IS_BATTLE,	0,	0,	1,	38'000,	40'000,	15,	 85,	 95 },
//{ 241,	"Protection",	AE_DFE,	0,	-21,	-25,	0,	0,	0,	0,	181,	IA_SHIELD,									IS_BATTLE,	0,	0,	0,	42'000,	50'000,	16,	105,	120 },

//harness
//{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  2,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	   250,	   250,	 1,	  0,	  0 },
{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  6,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
{ 241,	"Protection",	AE_DFE,	0,	-1,	-2,	0,	0,	0,	0,	 11,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 2'000,	 2'500,	 3,	  3,	  4 },
{ 241,	"Protection",	AE_DFE,	0,	-2,	-3,	0,	0,	0,	0,	 17,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 3'000,	 3'500,	 4,	 12,	 15 },
{ 241,	"Protection",	AE_DFE,	0,	-4,	-5,	0,	0,	0,	0,	 24,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 4'000,	 4'500,	 5,	 16,	 18 },
{ 241,	"Protection",	AE_DFE,	0,	-5,	-6,	0,	0,	0,	0,	 34,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 5'500,	 7'500,	 6,	 22,	 25 },
{ 241,	"Protection",	AE_DFE,	0,	-7,	-8,	0,	0,	0,	0,	 44,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 8'500,	10'500,	 7,	 26,	 31 },
{ 241,	"Protection",	AE_DFE,	0,	-8,	-9,	0,	0,	0,	0,	 54,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	12'500,	14'500,	 8,	 33,	 36 },
{ 241,	"Protection",	AE_DFE,	0,	-10,	-11,	0,	0,	0,	0,	 64,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	18'500,	20'500,	 9,	 37,	 40 },
{ 241,	"Protection",	AE_DFE,	0,	-11,	-12,	0,	0,	0,	0,	 74,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	23'500,	25'500,	10,	 45,	 47 },
{ 241,	"Protection",	AE_DFE,	0,	-13,	-14,	0,	0,	0,	0,	 89,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	26'000,	28'000,	11,	 48,	 52 },
{ 241,	"Protection",	AE_DFE,	0,	-15,	-15,	0,	0,	0,	0,	104,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	29'000,	31'000,	12,	 55,	 60 },
{ 241,	"Protection",	AE_DFE,	0,	-16,	-17,	0,	0,	0,	0,	124,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	32'000,	34'000,	13,	 65,	 73 },
{ 241,	"Protection",	AE_DFE,	0,	-17,	-18,	0,	0,	0,	0,	148,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	35'000,	37'000,	14,	 80,	 90 },
{ 241,	"Protection",	AE_DFE,	0,	-19,	-20,	0,	0,	0,	0,	179,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	38'000,	40'000,	15,	 95,	104 },
//{ 241,	"Protection",	AE_DFE,	0,	-41,	-45,	0,	0,	0,	0,	183,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	0,	56'000,	60'000,	16,	105,	120 },

//helm
//{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  2,	IA_HELM,	IS_ALL,		0,	0,	1,	   250,	   250,	 1,	  0,	  0 },
{ 241,	"Protection",	AE_DFE,	0,	-1,	-1,	0,	0,	0,	0,	  6,	IA_HELM,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
//{ 241,	"Protection",	AE_DFE,	0,	-3,	-3,	0,	0,	0,	0,	 11,	IA_HELM,	IS_ALL,		0,	0,	1,	 2'000,	 2'000,	 3,	  3,	  4 },
{ 241,	"Protection",	AE_DFE,	0,	-2,	-2,	0,	0,	0,	0,	 17,	IA_HELM,	IS_ALL,		0,	0,	1,	 3'000,	 3'500,	 4,	 12,	 15 },
{ 241,	"Protection",	AE_DFE,	0,	-3,	-3,	0,	0,	0,	0,	 24,	IA_HELM,	IS_ALL,		0,	0,	1,	 4'000,	 4'500,	 5,	 16,	 18 },
{ 241,	"Protection",	AE_DFE,	0,	-4,	-4,	0,	0,	0,	0,	 34,	IA_HELM,	IS_ALL,		0,	0,	1,	 5'500,	 6'500,	 6,	 22,	 25 },
{ 241,	"Protection",	AE_DFE,	0,	-5,	-5,	0,	0,	0,	0,	 44,	IA_HELM,	IS_ALL,		0,	0,	1,	 8'500,	 9'500,	 7,	 26,	 31 },
{ 241,	"Protection",	AE_DFE,	0,	-6,	-6,	0,	0,	0,	0,	 54,	IA_HELM,	IS_ALL,		0,	0,	1,	12'500,	13'500,	 8,	 33,	 36 },
{ 241,	"Protection",	AE_DFE,	0,	-7,	-7,	0,	0,	0,	0,	 64,	IA_HELM,	IS_ALL,		0,	0,	1,	14'500,	15'500,	 9,	 37,	 40 },
{ 241,	"Protection",	AE_DFE,	0,	-8,	-8,	0,	0,	0,	0,	 74,	IA_HELM,	IS_ALL,		0,	0,	1,	16'000,	17'000,	10,	 45,	 47 },
{ 241,	"Protection",	AE_DFE,	0,	-9,	-9,	0,	0,	0,	0,	 89,	IA_HELM,	IS_ALL,		0,	0,	1,	18'000,	19'000,	11,	 48,	 52 },
{ 241,	"Protection",	AE_DFE,	0,	-10,	-10,	0,	0,	0,	0,	104,	IA_HELM,	IS_ALL,		0,	0,	1,	20'000,	21'000,	12,	 55,	 60 },
{ 241,	"Protection",	AE_DFE,	0,	-11,	-12,	0,	0,	0,	0,	124,	IA_HELM,	IS_BATTLE,	0,	0,	1,	22'000,	24'000,	13,	 65,	 70 },
{ 241,	"Protection",	AE_DFE,	0,	-12,	-13,	0,	0,	0,	0,	149,	IA_HELM,	IS_BATTLE,	0,	0,	1,	25'000,	27'000,	14,	 75,	 80 },
{ 241,	"Protection",	AE_DFE,	0,	-14,	-15,	0,	0,	0,	0,	179,	IA_HELM,	IS_BATTLE,	0,	0,	1,	31'000,	33'000,	15,	 85,	 95 },
//{ 241,	"Protection",	AE_DFE,	0,	-31,	-5,	0,	0,	0,	0,	182,	IA_HELM,	IS_BATTLE,	0,	0,	0,	41'000,	45'000,	16,	105,	120 },

//jewelry
//{ 10,	"Freedom",		AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 10,	IA_JEWEL,	IS_ALL,	1,	0,	1,	   500,	   500,	2,	 2,	  3 },
{ 11,	"Sanctuary",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 23,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 2'000,	 2'000,	3,	 8,	 10 },
{ 12,	"Salvation",	AE_DFE,	0,	 -2,	 -2,	0,	0,	0,	0,	 44,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 4'000,	 4'000,	4,	15,	 19 },
//{ 13,	"Redemption",	AE_DFE,	0,	 -2,	 -2,	0,	0,	0,	0,	 60,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 6'000,	 6'000,	5,	23,	 27 },
{ 13,	"Redemption",	AE_DFE,	0,	 -3,	 -3,	0,	0,	0,	0,	 60,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 9'000,	 9'000,	5,	28,	 33 },
{ 13,	"Redemption",	AE_DFE,	0,	 -4,	 -4,	0,	0,	0,	0,	 85,	IA_JEWEL,	IS_ALL,	1,	0,	1,	11'000,	11'000,	6,	48,	 52 },
{ 13,	"Redemption",	AE_DFE,	0,	 -5,	 -5,	0,	0,	0,	0,	110,	IA_JEWEL,	IS_ALL,	1,	0,	1,	14'000,	14'000,	6,	63,	 67 },
{ 13,	"Redemption",	AE_DFE,	0,	 -6,	 -6,	0,	0,	0,	0,	135,	IA_JEWEL,	IS_ALL,	1,	0,	1,	17'000,	17'000,	7,	74,	 78 },
{ 13,	"Redemption",	AE_DFE,	0,	 -7,	 -7,	0,	0,	0,	0,	160,	IA_JEWEL,	IS_ALL,	1,	0,	1,	21'000,	21'000,	7,	93,	 97 },
//{ 13,	"Redemption",	AE_DFE,	0,	-10,	-10,	0,	0,	0,	0,	180,	IA_JEWEL,	IS_ALL,	1,	0,	0,	25'000,	25'000,	8,	99,	120 },


/* ------------------------------------
				STATS
-------------------------------------*/

//STR
//{ 279,	"Staunchness",	AE_STR,	0,	 1,	  2,	0,	0,	0,	0,	  1,	IA_ALL,																			IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
{ 279,	"Stoutness",	AE_STAT,	ST_STRENGTH,	 1,	  2,	0,	0,	0,	0,	  2,	IA_ALL,																			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
//{ 279,	"Stalwartness",	AE_STR,	0,	 5,	  6,	0,	0,	0,	0,	  4,	IA_ALL,																			IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
{ 279,	"Ruggedness",	AE_STAT,	ST_STRENGTH,	 3,	  4,	0,	0,	0,	0,	  7,	IA_ALL,																			IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
{ 280,	"Heartiness",	AE_STAT,	ST_STRENGTH,	 4,	  5,	0,	0,	0,	0,	 10,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
{ 280,	"Burliness",	AE_STAT,	ST_STRENGTH,	 6,	  7,	0,	0,	0,	0,	 20,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
{ 281,	"Solidity",		AE_STAT,	ST_STRENGTH,	 8,	 10,	0,	0,	0,	0,	 30,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
{ 282,	"Potency",		AE_STAT,	ST_STRENGTH,	11,	 12,	0,	0,	0,	0,	 40,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
{ 283,	"Tenacity",		AE_STAT,	ST_STRENGTH,	13,	 15,	0,	0,	0,	0,	 50,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
{ 284,	"Strength",		AE_STAT,	ST_STRENGTH,	16,	 20,	0,	0,	0,	0,	 60,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 27 },
{ 285,	"Force",		AE_STAT,	ST_STRENGTH,	21,	 22,	0,	0,	0,	0,	 70,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	23'500,	24'000,	 6,	27,	 37 },
{ 285,	"Power",		AE_STAT,	ST_STRENGTH,	23,	 25,	0,	0,	0,	0,	 80,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	27'500,	28'000,	 7,	37,	 47 },
{ 285,	"Brawn",		AE_STAT,	ST_STRENGTH,	26,	 27,	0,	0,	0,	0,	 90,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	30'500,	35'000,	 8,	47,	 57 },
{ 285,	"Brute",		AE_STAT,	ST_STRENGTH,	28,	 30,	0,	0,	0,	0,	105,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 67 },
{ 285,	"Might",		AE_STAT,	ST_STRENGTH,	31,	 35,	0,	0,	0,	0,	125,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 78 },
{ 285,	"Giants",		AE_STAT,	ST_STRENGTH,	36,	 40,	0,	0,	0,	0,	144,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 87 },
{ 285,	"Titans",		AE_STAT,	ST_STRENGTH,	41,	 45,	0,	0,	0,	0,	166,	IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_GLOVE,									IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 97 },
//{ 285,	"Goliath",		AE_STR,	0,	91,	100,	0,	0,	0,	0,	186,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,												IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },

//DEX
//{ 279,	"Craftiness",	AE_STAT,	ST_DEXTERITY,	 1,	  2,	0,	0,	0,	0,	  1,	IA_ALL,													IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
{ 279,	"Aptitude",		AE_STAT,	ST_DEXTERITY,	 1,	  2,	0,	0,	0,	0,	  2,	IA_ALL,													IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
//{ 279,	"Proficiency",	AE_STAT,	ST_DEXTERITY,	 5,	  6,	0,	0,	0,	0,	  4,	IA_ALL,													IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
{ 279,	"Talent",		AE_STAT,	ST_DEXTERITY,	 3,	  4,	0,	0,	0,	0,	  7,	IA_ALL,													IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
{ 280,	"Agility",		AE_STAT,	ST_DEXTERITY,	 4,	  5,	0,	0,	0,	0,	 11,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
{ 280,	"Nimbleness",	AE_STAT,	ST_DEXTERITY,	 6,	  7,	0,	0,	0,	0,	 16,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
{ 281,	"Skill",		AE_STAT,	ST_DEXTERITY,	 8,	 10,	0,	0,	0,	0,	 22,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
{ 282,	"Exactness",	AE_STAT,	ST_DEXTERITY,	11,	 12,	0,	0,	0,	0,	 29,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
{ 283,	"Dexterity",	AE_STAT,	ST_DEXTERITY,	13,	 15,	0,	0,	0,	0,	 37,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
{ 284,	"Accuracy",		AE_STAT,	ST_DEXTERITY,	16,	 20,	0,	0,	0,	0,	 46,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 26 },
{ 285,	"Cunning",		AE_STAT,	ST_DEXTERITY,	21,	 22,	0,	0,	0,	0,	 56,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	18'500,	23'000,	 6,	27,	 36 },
{ 285,	"Deftness",		AE_STAT,	ST_DEXTERITY,	23,	 25,	0,	0,	0,	0,	 71,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	26'500,	27'000,	 7,	37,	 46 },
{ 285,	"Expertise",	AE_STAT,	ST_DEXTERITY,	26,	 27,	0,	0,	0,	0,	 86,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	30'500,	35'000,	 8,	47,	 54 },
{ 285,	"Precision",	AE_STAT,	ST_DEXTERITY,	28,	 30,	0,	0,	0,	0,	107,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 67 },
{ 285,	"Mastery",		AE_STAT,	ST_DEXTERITY,	31,	 35,	0,	0,	0,	0,	127,	IA_RANGE | IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 77 },
{ 285,	"Perfection",	AE_STAT,	ST_DEXTERITY,	36,	 40,	0,	0,	0,	0,	146,	IA_RANGE | IA_JEWEL | IA_BODY_ARMOR | IA_BOOTS,			IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 88 },
{ 285,	"Guile",		AE_STAT,	ST_DEXTERITY,	41,	 45,	0,	0,	0,	0,	168,	IA_RANGE | IA_BOOTS,									IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 98 },
//{ 285,	"Grace",		AE_STAT,	ST_DEXTERITY,	91,	100,	0,	0,	0,	0,	188,	IA_RANGE,												IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },

//MAG	
//{ 279,	"Sanity",		AE_STAT,	ST_MAGIC,	 1,	  2,	0,	0,	0,	0,	  1,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
{ 279,	"Mind",			AE_STAT,	ST_MAGIC,	 1,	  2,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
//{ 279,	"Clarity",		AE_STAT,	ST_MAGIC,	 5,	  6,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
{ 279,	"Insight",		AE_STAT,	ST_MAGIC,	 3,	  4,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
{ 280,	"Reasoning",	AE_STAT,	ST_MAGIC,	 4,	  5,	0,	0,	0,	0,	 10,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
{ 280,	"Divination",	AE_STAT,	ST_MAGIC,	 6,	  7,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
{ 281,	"Intellect",	AE_STAT,	ST_MAGIC,	 8,	 10,	0,	0,	0,	0,	 30,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
{ 282,	"Discernment",	AE_STAT,	ST_MAGIC,	11,	 12,	0,	0,	0,	0,	 40,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
{ 283,	"Magic",		AE_STAT,	ST_MAGIC,	13,	 15,	0,	0,	0,	0,	 50,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
{ 284,	"Knowledge",	AE_STAT,	ST_MAGIC,	16,	 20,	0,	0,	0,	0,	 60,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 27 },
{ 285,	"Erudition",	AE_STAT,	ST_MAGIC,	21,	 22,	0,	0,	0,	0,	 70,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	20'500,	30'000,	 6,	27,	 37 },
{ 285,	"Foresight",	AE_STAT,	ST_MAGIC,	23,	 25,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	20'500,	30'000,	 7,	37,	 47 },
{ 285,	"Wisdom",		AE_STAT,	ST_MAGIC,	26,	 27,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	30'500,	35'000,	 8,	47,	 57 },
{ 285,	"Enlightenment",AE_STAT,	ST_MAGIC,	28,	 30,	0,	0,	0,	0,	105,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	35'500,	40'000,	 9,	57,	 67 },
{ 285,	"Epiphany",		AE_STAT,	ST_MAGIC,	31,	 35,	0,	0,	0,	0,	125,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_HELM | IA_BODY_ARMOR | IA_GLOVE,	IS_MAGIC,	0,	0,	1,	40'500,	45'000,	10,	67,	 78 },
{ 285,	"Witchcraft",	AE_STAT,	ST_MAGIC,	36,	 40,	0,	0,	0,	0,	145,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_HELM,								IS_MAGIC,	0,	0,	1,	45'500,	50'000,	11,	77,	 87 },
{ 285,	"Sorcery",		AE_STAT,	ST_MAGIC,	41,	 45,	0,	0,	0,	0,	166,	IA_JEWEL | IA_STAFF | IA_SHIELD,											IS_MAGIC,	0,	0,	1,	50'500,	55'000,	12,	87,	 97 },
//{ 285,	"Wizardry",		AE_STAT,	ST_MAGIC,	91,	100,	0,	0,	0,	0,	187,	IA_STAFF,																IS_MAGIC,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },

//VIT
//{ 279,	"Health",		AE_STAT,	ST_VITALITY,	 1,	  2,	0,	0,	0,	0,	  1,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
{ 279,	"Sustenance",	AE_STAT,	ST_VITALITY,	 1,	  2,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
//{ 279,	"Robustness",	AE_STAT,	ST_VITALITY,	 5,	  6,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
{ 279,	"Hardiness",	AE_STAT,	ST_VITALITY,	 3,	  4,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
{ 280,	"Constitution",	AE_STAT,	ST_VITALITY,	 4,	  5,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
{ 280,	"Zest",			AE_STAT,	ST_VITALITY,	 6,	  7,	0,	0,	0,	0,	 16,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
{ 281,	"Zeal",			AE_STAT,	ST_VITALITY,	8,	 10,	0,	0,	0,	0,	 22,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	  9 },
{ 282,	"Verve",		AE_STAT,	ST_VITALITY,	11,	 12,	0,	0,	0,	0,	 29,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 14 },
{ 283,	"Vitality",		AE_STAT,	ST_VITALITY,	13,	 15,	0,	0,	0,	0,	 37,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 20 },
{ 284,	"Vim",			AE_STAT,	ST_VITALITY,	16,	 20,	0,	0,	0,	0,	 46,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 25 },
{ 285,	"Vigor",		AE_STAT,	ST_VITALITY,	21,	 22,	0,	0,	0,	0,	 56,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	20'500,	30'000,	 6,	27,	 31 },
{ 285,	"Vivacity",		AE_STAT,	ST_VITALITY,	23,	 25,	0,	0,	0,	0,	 71,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	20'500,	30'000,	 7,	37,	 42 },
{ 285,	"Vehemence",	AE_STAT,	ST_VITALITY,	26,	 27,	0,	0,	0,	0,	 86,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	30'500,	35'000,	 8,	47,	 53 },
{ 285,	"Ardor",		AE_STAT,	ST_VITALITY,	28,	 30,	0,	0,	0,	0,	106,	IA_SHIELD | IA_ARMOR,				IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 64 },
{ 285,	"Fervor",		AE_STAT,	ST_VITALITY,	31,	 35,	0,	0,	0,	0,	126,	IA_ARMOR,							IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 75 },
{ 285,	"Life",			AE_STAT,	ST_VITALITY,	36,	 40,	0,	0,	0,	0,	146,	IA_BODY_ARMOR | IA_BELT | IA_BOOTS,	IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 86 },
{ 285,	"Essence",		AE_STAT,	ST_VITALITY,	41,	 45,	0,	0,	0,	0,	167,	IA_BODY_ARMOR | IA_BELT,			IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 97 },
//{ 285,	"Colossus",		AE_STAT,	ST_VITALITY,	91,	100,	0,	0,	0,	0,	188,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },

//ALL STATS
//{ 286,	"the Plains",		AE_ALL_STAT,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   150,	    150,	 2,	 0,	  1 },
{ 286,	"the Hills",		AE_ALL_STAT,	0,	 1,	 1,	0,	0,	0,	0,	  6,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   500,	    500,	 2,	 0,	  1 },
//{ 287,	"the Mountains",	AE_ALL_STAT,	0,	 3,	 3,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'000,	  1'000,	 3,	 1,	  2 },
{ 288,	"the Rainbow",		AE_ALL_STAT,	0,	 2,	 2,	0,	0,	0,	0,	 13,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  2'000,	 3,	 7,	  9 },
{ 289,	"the Lights",		AE_ALL_STAT,	0,	 3,	 3,	0,	0,	0,	0,	 23,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 4'000,	  5'000,	 4,	10,	 12 },
{ 290,	"the Clouds",		AE_ALL_STAT,	0,	 4,	 4,	0,	0,	0,	0,	 33,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	10'000,	 12'000,	 6,	19,	 21 },
{ 291,	"the Sky",			AE_ALL_STAT,	0,	 5,	 5,	0,	0,	0,	0,	 43,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	14'000,	 16'000,	 8,	25,	 28 },
{ 292,	"the Moon",			AE_ALL_STAT,	0,	 6,	 6,	0,	0,	0,	0,	 53,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	27'000,	 30'000,	 9,	32,	 35 },
{ 293,	"the Sun",			AE_ALL_STAT,	0,	 7,	 7,	0,	0,	0,	0,	 63,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	35'000,	 45'000,	10,	40,	 44 },
{ 497,	"the Stars",		AE_ALL_STAT,	0,	 8,	 8,	0,	0,	0,	0,	 83,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	 55'000,	11,	50,	 55 },
{ 498,	"the Heavens",		AE_ALL_STAT,	0,	 9,	 9,	0,	0,	0,	0,	103,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	55'000,	 65'000,	12,	60,	 64 },
{ 499,	"the Zodiac",		AE_ALL_STAT,	0,	10,	10,	0,	0,	0,	0,	123,	IA_JEWEL | IA_ARMOR,						IS_ALL,	0,	0,	1,	65'000,	 750'00,	13,	70,	 77 },
{ 500,	"the Galaxies",		AE_ALL_STAT,	0,	11,	11,	0,	0,	0,	0,	143,	IA_JEWEL,									IS_ALL,	0,	0,	1,	75'000,	 85'000,	14,	80,	 88 },
{ 501,	"the Angels",		AE_ALL_STAT,	0,	12,	12,	0,	0,	0,	0,	163,	IA_JEWEL,									IS_ALL,	0,	0,	1,	85'000,	 95'000,	15,	90,	 99 },
//{ 502,	"the Arch-Angels",	AE_ALL_STAT,	0,	26,	30,	0,	0,	0,	0,	183,	IA_JEWEL,								IS_ALL,	0,	0,	0,	96'000,	100'000,	17,	99,	110 },


/* ------------------------------------
				SLVL
-------------------------------------*/

//{ 294,	"the Focus",	AE_SPELLS_LEVEL,	0,	1,	1,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 2'000,	 2'000,	2,	 0,	  0 },
{ 295,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	1,	1,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 6'000,	 6'000,	4,	12,	 18 },
//{ 296,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	3,	3,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	12'000,	12'000,	8,	19,	 28 },
{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0,	 64,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	25'000,	25'000,	12,	29,	 34 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	5,	5,	0,	0,	0,	0,	105,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	35'000,	35'000,	13,	35,	 45 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	6,	6,	0,	0,	0,	0,	147,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	45'000,	45'000,	14,	60,	 75 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	7,	7,	0,	0,	0,	0,	184,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	55'000,	55'000,	15,	99,	108 },


/* ------------------------------------
				MANA
-------------------------------------*/

{ 306,	"the Moth",		AE_MANA,	0,	 4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	    80,	   100,	 1,	 0,	  0 },
{ 307,	"the Worm",		AE_MANA,	0,	 6,	  7,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	   220,	   300,	 2,	 0,	  0 },
{ 307,	"the Spider",	AE_MANA,	0,	 9,	 10,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	   480,	   600,	 2,	 1,	  2 },
{ 308,	"the Wasp",		AE_MANA,	0,	11,	 12,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 1'100,	 1'500,	 2,	 3,	  5 },
{ 309,	"the Frog",		AE_MANA,	0,	13,	 15,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 1'600,	 2'000,	 3,	 6,	  8 },
{ 310,	"the Scorpion",	AE_MANA,	0,	16,	 20,	0,	0,	0,	0,	 29,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 2'100,	 3'000,	 3,	 9,	 12 },
{ 311,	"the Rat",		AE_MANA,	0,	21,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 3'100,	 4'000,	 4,	13,	 15 },
{ 312,	"the Cat",		AE_MANA,	0,	26,	 30,	0,	0,	0,	0,	 48,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 5'100,	 6'000,	 5,	16,	 20 },
{ 313,	"the Lizard",	AE_MANA,	0,	31,	 35,	0,	0,	0,	0,	 59,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 6'100,	 7'000,	 6,	21,	 27 },
{ 314,	"the Vulture",	AE_MANA,	0,	36,	 40,	0,	0,	0,	0,	 69,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 7'100,	 8'000,	 7,	28,	 35 },
{ 314,	"the Serpent",	AE_MANA,	0,	41,	 45,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 8'100,	 9'000,	 7,	35,	 39 },
{ 314,	"the Serpent",	AE_MANA,	0,	46,	 50,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 9'100,	10'000,	 8,	40,	 44 },
{ 314,	"the Serpent",	AE_MANA,	0,	51,	 55,	0,	0,	0,	0,	100,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	10'100,	11'000,	 8,	45,	 49 },
{ 314,	"the Serpent",	AE_MANA,	0,	56,	 60,	0,	0,	0,	0,	110,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	11'100,	12'000,	 9,	49,	 53 },
{ 314,	"the Serpent",	AE_MANA,	0,	61,	 65,	0,	0,	0,	0,	120,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	12'100,	13'000,	 9,	53,	 57 },
{ 314,	"the Viper",	AE_MANA,	0,	66,	 70,	0,	0,	0,	0,	131,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	13'100,	14'000,	10,	57,	 61 },
{ 314,	"the Python",	AE_MANA,	0,	71,	 75,	0,	0,	0,	0,	140,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	14'100,	15'000,	10,	62,	 66 },
{ 314,	"the Wyrm",		AE_MANA,	0,	76,	 80,	0,	0,	0,	0,	151,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR | IA_HELM,	IS_MAGIC,	0,	0,	1,	15'100,	16'000,	11,	67,	 72 },
{ 314,	"the Hydra",	AE_MANA,	0,	81,	 85,	0,	0,	0,	0,	161,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR | IA_HELM,	IS_MAGIC,	0,	0,	1,	16'100,	17'000,	11,	73,	 79 },
{ 314,	"the Drake",	AE_MANA,	0,	86,	 90,	0,	0,	0,	0,	170,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR,			IS_MAGIC,	0,	0,	1,	17'100,	18'000,	12,	80,	 86 },
{ 314,	"the Wyvern",	AE_MANA,	0,	91,	 95,	0,	0,	0,	0,	180,	IA_STAFF | IA_SHIELD,							IS_MAGIC,	0,	0,	1,	18'100,	19'000,	12,	87,	 95 },
{ 314,	"the Dragon",	AE_MANA,	0,	96,	100,	0,	0,	0,	0,	186,	IA_STAFF,										IS_MAGIC,	0,	0,	1,	19'100,	20'000,	13,	96,	106 },


/* ------------------------------------
				HP
-------------------------------------*/

{ 306,	"the Squirrel",	AE_HIT_POINTS,	0,	 4,	  5,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  0 },
{ 307,	"the Rabbit",	AE_HIT_POINTS,	0,	 6,	  7,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	   220,	   300,	 2,	 0,	  0 },
{ 307,	"the Jackal",	AE_HIT_POINTS,	0,	 9,	 10,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	   480,	   600,	 2,	 1,	  2 },
{ 308,	"the Fox",		AE_HIT_POINTS,	0,	11,	 12,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 1'100,	 1'500,	 2,	 3,	  5 },
{ 309,	"the Badger",	AE_HIT_POINTS,	0,	13,	 15,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 1'600,	 2'000,	 3,	 6,	  8 },
{ 310,	"the Lynx",		AE_HIT_POINTS,	0,	16,	 20,	0,	0,	0,	0,	 29,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 2'100,	 3'000,	 3,	 9,	 12 },
{ 311,	"the Elk",		AE_HIT_POINTS,	0,	21,	 25,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 3'100,	 4'000,	 4,	13,	 15 },
{ 312,	"the Moose",	AE_HIT_POINTS,	0,	26,	 30,	0,	0,	0,	0,	 48,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 5'100,	 6'000,	 5,	16,	 20 },
{ 313,	"the Wolf",		AE_HIT_POINTS,	0,	31,	 35,	0,	0,	0,	0,	 59,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 6'100,	 7'000,	 6,	21,	 27 },
{ 314,	"the Panther",	AE_HIT_POINTS,	0,	36,	 40,	0,	0,	0,	0,	 69,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 7'100,	 8'000,	 7,	28,	 35 },
{ 314,	"the Jaguar",	AE_HIT_POINTS,	0,	41,	 45,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 8'100,	 9'000,	 7,	35,	 39 },
{ 314,	"the Lion",		AE_HIT_POINTS,	0,	46,	 50,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 9'100,	10'000,	 8,	40,	 44 },
{ 314,	"the Tiger",	AE_HIT_POINTS,	0,	51,	 55,	0,	0,	0,	0,	100,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	10'100,	11'000,	 8,	45,	 49 },
{ 314,	"the Walrus",	AE_HIT_POINTS,	0,	56,	 60,	0,	0,	0,	0,	109,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	11'100,	12'000,	 9,	49,	 53 },
{ 314,	"the Bull",		AE_HIT_POINTS,	0,	61,	 65,	0,	0,	0,	0,	120,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	12'100,	13'000,	 9,	53,	 57 },
{ 314,	"the Ox",		AE_HIT_POINTS,	0,	66,	 70,	0,	0,	0,	0,	130,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	13'100,	14'000,	10,	57,	 61 },
{ 314,	"the Bear",		AE_HIT_POINTS,	0,	71,	 75,	0,	0,	0,	0,	139,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	14'100,	15'000,	10,	62,	 66 },
{ 314,	"the Shark",	AE_HIT_POINTS,	0,	76,	 80,	0,	0,	0,	0,	150,	IA_ARMOR,										IS_BATTLE,	0,	0,	1,	15'100,	16'000,	11,	67,	 72 },
{ 314,	"the Whale",	AE_HIT_POINTS,	0,	81,	 85,	0,	0,	0,	0,	160,	IA_BODY_ARMOR | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	16'100,	17'000,	11,	73,	 79 },
{ 314,	"the Elephant",	AE_HIT_POINTS,	0,	86,	 90,	0,	0,	0,	0,	169,	IA_BODY_ARMOR | IA_BELT | IA_BOOTS,				IS_BATTLE,	0,	0,	1,	17'100,	18'000,	12,	80,	 86 },
{ 314,	"the Mammoth",	AE_HIT_POINTS,	0,	91,	 95,	0,	0,	0,	0,	180,	IA_BODY_ARMOR | IA_BELT,						IS_BATTLE,	0,	0,	1,	18'100,	19'000,	12,	87,	 95 },
{ 314,	"the Whale",	AE_HIT_POINTS,	0,	96,	100,	0,	0,	0,	0,	186,	IA_HEAVY_ARMOR,									IS_BATTLE,	0,	0,	1,	19'100,	20'000,	13,	96,	106 },


/* ------------------------------------
	RANGED WEAPONS' ELEMENTAL DAMAGE
-------------------------------------*/

//{ 315,	"Ashes",	AE_ELEM_HIT,	EA_ARROW_FIRE,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
{ 316,	"Cinder",	AE_ELEM_HIT,	EA_ARROW_FIRE,	1,	1,	2,	 3,	0,	0,	 2,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 317,	"Fire",		AE_ELEM_HIT,	EA_ARROW_FIRE,	2,	3,	4,	 6,	0,	0,	 8,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 318,	"Flame",	AE_ELEM_HIT,	EA_ARROW_FIRE,	4,	5,	6,	 8,	0,	0,	18,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 319,	"Searing",	AE_ELEM_HIT,	EA_ARROW_FIRE,	6,	7,	8,	10,	0,	0,	38,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 320,	"Burning",	AE_ELEM_HIT,	EA_ARROW_FIRE,	8,	9,	10,	12,	0,	0,	53,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 320,	"Burning",	AE_ELEM_HIT,	EA_ARROW_FIRE,	8,	9,	10,	12,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

//{ 321,	"Sparks",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
{ 322,	"Charges",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	 3,	 4,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 323,	"Shock",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	2,	 6,	 9,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 324,	"Lightning",	AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	12,	15,	0,	0,	22,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 325,	"Thunder",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	16,	20,	0,	0,	39,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 326,	"Electra",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	21,	26,	0,	0,	54,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 326,	"Electra",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	21,	26,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

//{ 327,	"Cooling",	AE_ELEM_HIT,	EA_ARROW_COLD,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
{ 328,	"Snow",		AE_ELEM_HIT,	EA_ARROW_COLD,	 1,	 1,	 2,	 3,	0,	0,	 2,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 329,	"Ice",		AE_ELEM_HIT,	EA_ARROW_COLD,	 2,	 3,	 4,	 6,	0,	0,	 8,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 330,	"Frost",	AE_ELEM_HIT,	EA_ARROW_COLD,	 4,	 5,	 6,	 8,	0,	0,	17,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 331,	"Winter",	AE_ELEM_HIT,	EA_ARROW_COLD,	 6,	 7,	 8,	10,	0,	0,	37,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 332,	"Freezing",	AE_ELEM_HIT,	EA_ARROW_COLD,	 8,	 9,	10,	12,	0,	0,	53,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 332,	"Freezing",	AE_ELEM_HIT,	EA_ARROW_COLD,	 8,	 9,	10,	12,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

//{ 333,	"Cleansing",	AE_ELEM_HIT,	EA_ARROW_HOLY,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
{ 334,	"Purification",	AE_ELEM_HIT,	EA_ARROW_HOLY,	 1,	 1,	 2,	 3,	0,	0,	 2,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 335,	"Purge",		AE_ELEM_HIT,	EA_ARROW_HOLY,	 2,	 3,	 4,	 6,	0,	0,	 8,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 336,	"Sanctity",		AE_ELEM_HIT,	EA_ARROW_HOLY,	 4,	 5,	 6,	 8,	0,	0,	17,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 337,	"Catharsis",	AE_ELEM_HIT,	EA_ARROW_HOLY,	 6,	 7,	 8,	10,	0,	0,	37,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 338,	"Absolution",	AE_ELEM_HIT,	EA_ARROW_HOLY,	 8,	 9,	10,	12,	0,	0,	53,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 338,	"Absolution",	AE_ELEM_HIT,	EA_ARROW_HOLY,	 8,	 9,	10,	12,	0,	0,	95,	IA_RANGE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

//{ 339,	"Voiding",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	 1,	 1,	 2,	 3,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 340,	"Depletion",	AE_ELEM_HIT,	EA_ARROW_ARCAN,	 2,	 3,	 4,	 6,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 341,	"Havoc",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	 4,	 5,	 6,	 8,	0,	0,	24,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 342,	"Exhausting",	AE_ELEM_HIT,	EA_ARROW_ARCAN,	 6,	 7,	 8,	10,	0,	0,	40,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 343,	"Devastation",	AE_ELEM_HIT,	EA_ARROW_ARCAN,	 8,	 9,	10,	12,	0,	0,	61,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 343,	"Devastation",	AE_ELEM_HIT,	EA_ARROW_ARCAN,	 8,	 9,	10,	12,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

//{ 344,	"Blight",			AE_ELEM_HIT,	EA_ARROW_ACID,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
{ 345,	"Acidity",			AE_ELEM_HIT,	EA_ARROW_ACID,	 1,	 1,	 2,	 3,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
{ 346,	"Toxicity",			AE_ELEM_HIT,	EA_ARROW_ACID,	 2,	 3,	 4,	 6,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
{ 347,	"Corrosion",		AE_ELEM_HIT,	EA_ARROW_ACID,	 4,	 5,	 6,	 8,	0,	0,	22,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
{ 348,	"Dissolution",		AE_ELEM_HIT,	EA_ARROW_ACID,	 6,	 7,	 8,	10,	0,	0,	39,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
{ 349,	"Disintegration",	AE_ELEM_HIT,	EA_ARROW_ACID,	 8,	 9,	10,	12,	0,	0,	59,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
{ 349,	"Disintegration",	AE_ELEM_HIT,	EA_ARROW_ACID,	 8,	 9,	10,	12,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },


/* ------------------------------------
				VARIOUS
-------------------------------------*/

//{ 350,	"Parrying",	AE_SPECIAL_AC,	0,	1,	3,	0,	0,	0,	0,	3,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	100,	300,	2,	0,	0 },
//{ 351,	"Fending",	AE_SPECIAL_AC,	0,	4,	5,	0,	0,	0,	0,	8,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	300,	600,	3,	3,	4 },


/* ------------------------------------
				  AC
-------------------------------------*/

//{ 377,	"Reliability",		AE_AC,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
{ 378,	"Faith",			AE_AC,	0,	 2,	 2,	0,	0,	0,	0,	  3,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   300,	   450,	 2,	  0,	  1 },
{ 379,	"Safety",			AE_AC,	0,	 3,	 3,	0,	0,	0,	0,	  8,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   800,	 1'200,	 2,	  1,	  2 },
{ 380,	"Repulsion",		AE_AC,	0,	 4,	 5,	0,	0,	0,	0,	 16,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'750,	 2'500,	 3,	  3,	  4 },
{ 381,	"Toughness",		AE_AC,	0,	 6,	 7,	0,	0,	0,	0,	 27,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 3'300,	 4'500,	 4,	 16,	 18 },
{ 382,	"Deflection",		AE_AC,	0,	 8,	10,	0,	0,	0,	0,	 43,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 5'600,	 7'000,	 5,	 28,	 33 },
{ 382,	"Deflection",		AE_AC,	0,	11,	12,	0,	0,	0,	0,	 70,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 8'200,	 9'000,	 6,	 35,	 38 },
{ 382,	"Imperviousness",	AE_AC,	0,	13,	15,	0,	0,	0,	0,	100,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	11'000,	15'000,	 7,	 45,	 55 },
{ 382,	"Imperviousness",	AE_AC,	0,	16,	17,	0,	0,	0,	0,	130,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	21'000,	25'000,	 8,	 60,	 70 },
{ 382,	"Imperviousness",	AE_AC,	0,	18,	20,	0,	0,	0,	0,	159,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	26'000,	30'000,	 9,	 83,	 99 },
//{ 382,	"Imperviousness",	AE_AC,	0,	41,	50,	0,	0,	0,	0,	181,	IA_BODY_ARMOR,			IS_BATTLE,	0,	0,	0,	41'000,	50'000,	15,	100,	120 },


/* ------------------------------------
			+ GOLD FLAT
-------------------------------------*/

//{ 383,	"Avarice",		AE_GOLD_ABS_FIND,	0,	 1,	 2,	 3,	 4,	0,	0,	  2,	IA_ALL,							IS_ALL,	0,	0,	1,	   100,	   400,	2,	 0,	 0 },
{ 384,	"Greed",		AE_GOLD_ABS_FIND,	0,	 2,	 3,	 4,	 4,	0,	0,	  7,	IA_ALL,							IS_ALL,	0,	0,	1,	   500,	   800,	2,	 0,	 0 },
{ 385,	"Wealth",		AE_GOLD_ABS_FIND,	0,	 4,	 5,	 6,	 6,	0,	0,	 12,	IA_ALL,							IS_ALL,	0,	0,	1,	 1'350,	 1'800,	3,	 4,	 4 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	 7,	 8,	 9,	10,	0,	0,	 22,	IA_JEWEL | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'300,	 4'000,	4,	11,	14 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	11,	12,	13,	15,	0,	0,	 40,	IA_JEWEL | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 8'400,	12'000,	4,	26,	30 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	16,	17,	18,	20,	0,	0,	 70,	IA_JEWEL | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	24'800,	32'000,	4,	41,	44 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	21,	22,	23,	25,	0,	0,	129,	IA_JEWEL | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	32'800,	40'000,	4,	70,	75 },


/* ------------------------------------
				 STUN
-------------------------------------*/

//{ 387,	"Firmness",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	   100,	   100,	 1,	 0,	  0 },
{ 388,	"Firmness",			AE_STUN_RESIST,	0,	1,	1,	0,	0,	0,	0,	  5,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	   500,	   500,	 2,	 1,	  2 },
//{ 389,	"Stiffness",	AE_STUN_RESIST,	0,	3,	3,	0,	0,	0,	0,	 12,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	 1'300,	 1'300,	 3,	 4,	  5 },
{ 390,	"Stiffness",		AE_STUN_RESIST,	0,	2,	2,	0,	0,	0,	0,	 20,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	 4'000,	 4'000,	 4,	 9,	 11 },
{ 391,	"Rigidity",			AE_STUN_RESIST,	0,	3,	3,	0,	0,	0,	0,	 32,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	 7'500,	 9'000,	 7,	19,	 23 },
{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	4,	4,	0,	0,	0,	0,	 54,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	31,	 36 },
{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0,	 80,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	14'000,	16'000,	 9,	44,	 51 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	6,	6,	0,	0,	0,	0,	100,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	17'000,	19'000,	10,	57,	 62 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	7,	7,	0,	0,	0,	0,	120,	IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BOOTS,							IS_BATTLE,	0,	0,	1,	21'000,	22'000,	11,	69,	 76 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	8,	8,	0,	0,	0,	0,	142,	IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM,									IS_BATTLE,	0,	0,	1,	23'000,	24'000,	12,	79,	 86 },
//{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	17,	20,	0,	0,	0,	0,	178,	IA_MID_ARMOR | IA_HEAVY_ARMOR,												IS_BATTLE,	0,	0,	0,	34'000,	40'000,	15,	99,	113 },


/* ------------------------------------
				  MF
-------------------------------------*/

//{ 393,	"Chance",	AE_MAGIC_FIND,	0,	 5,	10,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_ARMOR | IA_FLASK | IA_TRAP,																IS_ALL,	0,	0,	1,	   100,	   200,	 2,	 0,	 0 },
{ 394,	"Bounty",	AE_MAGIC_FIND,	0,	 5,	 7,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_ARMOR | IA_FLASK | IA_TRAP,														IS_ALL,	0,	0,	1,	   400,	   600,	 3,	 0,	 0 },
{ 395,	"Fortune",	AE_MAGIC_FIND,	0,	 9,	10,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_ARMOR | IA_FLASK | IA_TRAP,														IS_ALL,	0,	0,	1,	 1'500,	 2'000,	 4,	 3,	 5 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	10,	12,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	 4'000,	 5'000,	 5,	12,	15 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	13,	15,	0,	0,	0,	0,	 45,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	 5'000,	 6'000,	 6,	30,	33 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	15,	17,	0,	0,	0,	0,	 65,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	 8'000,	10'000,	 7,	35,	38 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	17,	20,	0,	0,	0,	0,	 85,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	13'000,	16'000,	 8,	45,	48 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	20,	22,	0,	0,	0,	0,	108,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	20'000,	24'000,	 9,	55,	58 },
//{ 396,	"Luck",		AE_MAGIC_FIND,	0,	45,	50,	0,	0,	0,	0,	128,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	0,	30'000,	35'000,	10,	65,	75 },


//works but unused now	{ 0,	"Spell Level",	AE_SPELL_LEVEL,	PS_1_FIREBOLT,	1,	2,	0,	0,	0,	0,	11,	IA_STAFF,	IS_ALL,	0,	0,	1,	100,	100,	1,	1+2 },
//	{ 0,	"To Spell",	AE_SPELL_PERCENT,	0,	500,	500,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"Add Spell Damage",	AE_SPELL_DAMAGE,	500,	500,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"To Element",	AE_ELEMENT_PERCENT,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"Add Element Damage",	AE_ELEMENT_DAMAGE,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


	/* ------------------------------------
				  FLAT THORNS
	-------------------------------------*/

	//{ 397,	"Burs",		AE_ACTION,	EA_THORNS,	  1,	  2,	  3,	  4,	51,	 60,	  1,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,		IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	 0,	 0 },
	{ 398,	"Prickles",	AE_ACTION,	EA_THORNS,	  2,	  3,	  3,	  4,	36,	 45,	  3,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   200,	   200,	 2,	  0,	  0 },
	{ 399,	"Bramble",	AE_ACTION,	EA_THORNS,	  2,	  2,	  3,	  5,	76,	 90,	  8,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   500,	   500,	 3,	  2,	  3 },
	{ 400,	"Teeth",	AE_ACTION,	EA_THORNS,	 13,	 19,	 24,	 28,	21,	 30,	 15,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'000,	 1'000,	 4,	  4,	  6 },
	{ 401,	"Barbs",	AE_ACTION,	EA_THORNS,	  8,	  9,	  9,	 10,	91,	100,	 26,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 2'500,	 2'500,	 5,	  9,	 11 },
	{ 402,	"Thorns",	AE_ACTION,	EA_THORNS,	 16,	 18,	 20,	 22,	61,	 70,	 34,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'500,	 3'500,	 6,	 13,	 15 },
	{ 403,	"Spikes",	AE_ACTION,	EA_THORNS,	 30,	 35,	 42,	 52,	41,	 55,	 42,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 6'500,	 6'500,	 7,	 17,	 19 },
	{ 404,	"Razors",	AE_ACTION,	EA_THORNS,	 31,	 36,	 40,	 42,	71,	 80,	 51,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	11'500,	11'500,	 8,	 21,	 23 },
	{ 405,	"Lances",	AE_ACTION,	EA_THORNS,	257,	275,	325,	375,	11,	 15,	 59,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	22'500,	22'500,	 9,	 25,	 28 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	 50,	 55,	 57,	 72,	81,	 90,	 75,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	32'500,	32'500,	10,	 41,	 44 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	185,	200,	210,	255,	27,	 30,	 95,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	44'500,	44'500,	11,	 55,	 60 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	150,	165,	175,	200,	40,	 45,	115,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	53'500,	55'500,	12,	 66,	 69 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	675,	750,	825,	900,	13,	 15,	135,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	58'500,	58'500,	13,	 74,	 81 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	650,	700,	750,	800,	20,	 22,	155,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	63'500,	64'500,	14,	 86,	 93 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	425,	450,	475,	500,	40,	 45,	175,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	68'500,	70'500,	15,	 97,	105 },


	//	{ 0,	"To Thorns",	AE_ACTION_PERCENT,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
	//	{ 0,	"Add Thorns",	AE_ACTION_VALUE,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


		/* ------------------------------------
						OTHER
		-------------------------------------*/

	//	{ 410,	"the Raven",	AE_LIFE_REGEN,	0,	 1,	 2,	0,	0,	0,	0,	  3,	IA_JEWEL | IA_SHIELD | IA_ARMOR,		IS_BATTLE,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 411,	"the Owl",		AE_LIFE_REGEN,	0,	1,	 2,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	 1'200,	 1'800,	3,	 2,	 2 },
	{ 412,	"the Hawk",		AE_LIFE_REGEN,	0,	2,	 3,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'800,	4,	 5,	11 },
	{ 413,	"the Falcon",	AE_LIFE_REGEN,	0,	3,	 4,	0,	0,	0,	0,	 32,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	 7'000,	 8'000,	5,	13,	19 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	4,	 5,	0,	0,	0,	0,	 49,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	6,	20,	30 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	5,	 7,	0,	0,	0,	0,	 87,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	11'000,	15'000,	7,	50,	55 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	8,	10,	0,	0,	0,	0,	121,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	16'000,	20'000,	8,	67,	77 },

		//{ 415,	"the Gecko",	AE_MANA_REGEN,	0,	 1,	 2,	0,	0,	0,	0,	  3,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 416,	"the Lizard",	AE_MANA_REGEN,	0,	1,	 2,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	 1'200,	 1'800,	3,	 2,	 2 },
	{ 417,	"the Turtle",	AE_MANA_REGEN,	0,	2,	 3,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	 4'000,	 4'800,	4,	 5,	11 },
	{ 418,	"the Wyrm",		AE_MANA_REGEN,	0,	3,	 4,	0,	0,	0,	0,	 32,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	 7'000,	 8'000,	5,	13,	19 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	4,	 5,	0,	0,	0,	0,	 49,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	 9'000,	10'000,	6,	20,	30 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	5,	 7,	0,	0,	0,	0,	 87,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	11'000,	15'000,	7,	50,	55 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	8,	10,	0,	0,	0,	0,	121,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	16'000,	20'000,	8,	67,	77 },


		// flat xp cancelled for now
	//{ 420,	"Learning",			AE_XP_GAIN,	0,	   5,	  10, 	  15,	  20,	0,	0,	 2,	IA_JEWEL|IA_STAFF|IA_ARMOR|IA_FLASK|IA_TRAP | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	200,	300,	2,	0,	0+2 },
	//{ 421,	"Perspicacity",		AE_XP_GAIN,	0,	  20,	  30,	  40,	  50,	0,	0,	 7,	IA_JEWEL|IA_STAFF|IA_ARMOR|IA_SWORD_1H|IA_FLASK|IA_TRAP | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	360,	540,	3,	2,	2+2 },
	//{ 422,	"Insight",			AE_XP_GAIN,	0,	  50,	  70,	  90,	 110,	0,	0,	13,	IA_JEWEL|IA_STAFF|IA_ARMOR|IA_SWORD_1H|IA_FLASK|IA_TRAP | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	600,	900,	4,	6,	6+2 },
	//{ 423,	"Discernment",		AE_XP_GAIN,	0,	 120,	 160,	 200,	 250,	0,	0,	20,	IA_JEWEL|IA_STAFF|IA_ARMOR|IA_FLASK|IA_TRAP | IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	1200,	1800,	5,	10,	10+2 },
	//{ 424,	"Shrewdness",		AE_XP_GAIN,	0,	 300,	 500,	 600,	 800,	0,	0,	28,	IA_JEWEL|IA_LIGHT_ARMOR|IA_MID_ARMOR|IA_HELM|IA_BELT|IA_GLOVE|IA_BOOTS,	IS_ALL,	0,	0,	1,	1800,	2700,	6,	14,	14+2 },
	//{ 425,	"Sagacity",			AE_XP_GAIN,	0,	1000,	1400,	1600,	2000,	0,	0,	37,	IA_JEWEL|IA_LIGHT_ARMOR|IA_MID_ARMOR|IA_HELM|IA_BELT|IA_GLOVE|IA_BOOTS,	IS_ALL,	0,	0,	1,	2400,	3600,	7,	18,	18+2 },
	//{ 426,	"Acumen",			AE_XP_GAIN,	0,	2000,	2600,	2900,	3500,	0,	0,	47,	IA_JEWEL|IA_LIGHT_ARMOR|IA_MID_ARMOR|IA_HELM|IA_BELT|IA_GLOVE|IA_BOOTS,	IS_ALL,	0,	0,	1,	3200,	4800,	8,	22,	22+2 },
	//{ 427,	"Comprehension",	AE_XP_GAIN,	0,	3500,	4000,	4500,	5000,	0,	0,	58,	IA_JEWEL|IA_LIGHT_ARMOR|IA_MID_ARMOR|IA_HELM|IA_BELT|IA_GLOVE|IA_BOOTS,	IS_ALL,	0,	0,	0,	4000,	6000,	9,	26,	26+2 },
//	{ 0,	"ARD",	AE_ABSORB_RANGED_DAMAGE,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


	/* ------------------------------------
					  SUMMON BOOSTS
	-------------------------------------*/

	//{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	 1,	 2,	0,	0,	 1,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	    50,	   200,	 1,	 0,	 0 },
	{ 429,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	 1,	 1,	0,	0,	  4,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   250,	   500,	 2,	 0,	 0 },
	{ 430,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	 2,	 2,	0,	0,	 11,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   600,	 1'000,	 3,	 3,	 5 },
	{ 431,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 2,	 2,	 3,	 4,	0,	0,	 25,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 3'500,	 5'000,	 4,	15,	18 },
	{ 432,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 2,	 3,	 3,	 5,	0,	0,	 55,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	30,	34 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 4,	 5,	 6,	 7,	0,	0,	 67,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	20'000,	20'000,	 8,	40,	44 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 7,	 8,	 9,	10,	0,	0,	 92,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	30'000,	33'000,	10,	52,	57 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 9,	10,	11,	12,	0,	0,	112,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	35'000,	40'000,	10,	58,	63 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	12,	13,	14,	15,	0,	0,	132,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	41'000,	45'000,	11,	66,	70 },

	{ 434,	"Thickening",	AE_SUMMON_HP,	0,	  2,	  5,	0,	0,	0,	0,	  1,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	    50,	   200,	1,	 0,	 0 },
	{ 435,	"Thickening",	AE_SUMMON_HP,	0,	  5,	 10,	0,	0,	0,	0,	  4,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	   250,	   500,	2,	 0,	 0 },
	{ 436,	"Thickening",	AE_SUMMON_HP,	0,	 10,	 20,	0,	0,	0,	0,	 11,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	   600,	 1'200,	3,	 4,	 5 },
	{ 437,	"Thickening",	AE_SUMMON_HP,	0,	 25,	 35,	0,	0,	0,	0,	 25,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	 2'500,	 5'000,	4,	15,	18 },
	{ 438,	"Thickening",	AE_SUMMON_HP,	0,	 40,	 50,	0,	0,	0,	0,	 55,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	6,	31,	36 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	 60,	 70,	0,	0,	0,	0,	 68,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	20'000,	20'000,	8,	40,	47 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	 80,	100,	0,	0,	0,	0,	 92,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	30'000,	30'000,	8,	51,	56 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	110,	125,	0,	0,	0,	0,	112,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	34'000,	40'000,	10,	58,	63 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	135,	150,	0,	0,	0,	0,	132,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	41'000,	45'000,	11,	66,	70 },

	//{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 2,	0,	0,	0,	0,	 1,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	    50,	   100,	 1,	 0,	 0 },
	{ 441,	"Guarding",	AE_SUMMON_AC,	0,	1,	 2,	0,	0,	0,	0,	  4,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   300,	   500,	 2,	 0,	 0 },
	{ 442,	"Guarding",	AE_SUMMON_AC,	0,	3,	 5,	0,	0,	0,	0,	 11,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   600,	 1'000,	 3,	 3,	 5 },
	{ 443,	"Guarding",	AE_SUMMON_AC,	0,	5,	 7,	0,	0,	0,	0,	 25,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 2'200,	 3'000,	 4,	16,	19 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	8,	10,	0,	0,	0,	0,	 55,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	30,	35 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	11,	12,	0,	0,	0,	0,	 92,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	21'000,	25'000,	 8,	52,	55 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	13,	15,	0,	0,	0,	0,	112,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	30'000,	35'000,	 9,	57,	63 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	16,	17,	0,	0,	0,	0,	132,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	36'000,	40'000,	10,	66,	70 },


	/* ------------------------------------
					CRITS
	-------------------------------------*/

	//{ 445,	"Supremacy",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 1,	 1,	  1,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
	{ 446,	"Supremacy",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 1,	 1,	  3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   200,	   300,	 1,	 0,	 0 },
	{ 447,	"Expertise",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 2,	 2,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   800,	 1'200,	 2,	 3,	 4 },
	{ 448,	"Expertise",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 3,	 3,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'800,	 3'600,	 4,	19,	21 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 4,	 5,	 48,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	10'000,	12'000,	 6,	31,	35 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 5,	 6,	 88,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	15'000,	15'000,	 8,	51,	57 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 7,	 7,	128,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	25'000,	25'000,	11,	70,	79 },

	{ 450,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	35,	45,	0,	0,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 5 },
	{ 451,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	46,	55,	0,	0,	0,	0, 	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	4,	13,	21 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	56,	65,	0,	0,	0,	0,	 54,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	30,	35 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	66,	75,	0,	0,	0,	0,	 88,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	17'000,	17'000,	7,	50,	57 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	76,	85,	0,	0,	0,	0,	124,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	27'000,	27'000,	9,	68,	76 },

	{ 453,	"Wrack",		AE_CRIT_PERCENT,	0,	18,	 22,	0,	0,	1,	1,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 5 },
	{ 454,	"Disruption",	AE_CRIT_PERCENT,	0,	23,	 27,	0,	0,	2,	2,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	4,	 9,	11 },
	{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	28,	 32,	0,	0,	3,	3,	 54,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	27,	33 },
	{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	33,	 37,	0,	0,	4,	4,	 95,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	52,	58 },
	//{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	76,	100,	0,	0,	8,	8,	102,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	0,	27'000,	27000,	9,	77,	80 },

	{ 456,	"Smite",		AE_CRIT_DAMAGE,	0,	   5,	  10,	  15,	  20,	0,	0,	  3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   100,	 1,	 0,	 0 },
	{ 457,	"Justice",		AE_CRIT_DAMAGE,	0,	  12,	  17,	  25,	  30,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'100,	 1'100,	 2,	 3,	 4 },
	{ 458,	"Punishment",	AE_CRIT_DAMAGE,	0,	  27,	  37,	  55,	  65,	0,	0,	 23,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 4,	15,	19 },
	{ 459,	"Retaliation",	AE_CRIT_DAMAGE,	0,	  65,	  80,	 120,	 135,	0,	0,	 38,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 6'000,	 6'000,	 6,	27,	29 },
	{ 460,	"Oblation",		AE_CRIT_DAMAGE,	0,	 130,	 160,	 240,	 270,	0,	0,	 53,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	10'000,	10'000,	 8,	31,	34 },
	{ 461,	"Retribution",	AE_CRIT_DAMAGE,	0,	 260,	 290,	 370,	 400,	0,	0,	 60,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	16'000,	16'000,	10,	35,	37 },
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 300,	 350,	 400,	 450,	0,	0,	 90,	IA_SWORD_1H | IA_MACE_1H,														IS_BATTLE,	0,	0,	1,	25'000,	25'000,	12,	52,	61 },
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 350,	 400,	 450,	 500,	0,	0,	 91,	IA_RANGE | IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,			IS_BATTLE,	0,	0,	1,	25'000,	25'000,	12,	53,	62 },
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 500,	 600,	 650,	 750,	0,	0,	163,	IA_RANGE | IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,			IS_BATTLE,	0,	0,	1,	35'000,	35'000,	14,	85,	90 },


	/* ------------------------------------
			 LIFE & MANA PERCENT
	-------------------------------------*/

	{ 463,	"Vis",			AE_LIFE_PERCENT,	0,	 5,	 6,	0,	0,	0,	0,	 16,	IA_ARMOR,						IS_BATTLE,	0,	0,	1,	   800,	 1'600,	 3,	 5,	  7 },
	{ 464,	"Prowess",		AE_LIFE_PERCENT,	0,	 6,	 7,	0,	0,	0,	0,	 27,	IA_ARMOR,						IS_BATTLE,	0,	0,	1,	 2'100,	 2'500,	 5,	15,	 21 },
	{ 465,	"Valour",		AE_LIFE_PERCENT,	0,	 8,	 9,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	 5'200,	 6'000,	 7,	24,	 31 },
	{ 465,	"Valour",		AE_LIFE_PERCENT,	0,	 9,	10,	0,	0,	0,	0,	 92,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	 8'200,	 9'000,	 9,	55,	 59 },
	{ 465,	"Heroism",		AE_LIFE_PERCENT,	0,	11,	12,	0,	0,	0,	0,	181,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	21'000,	25'000,	12,	99,	111 },

	{ 466,	"Nous",			AE_MANA_PERCENT,	0,	5,	 6,	0,	0,	0,	0,	 16,	IA_ARMOR,	IS_MAGIC,	0,	0,	1,	   800,	 1'600,	 3,	 5,	  7 },
	{ 467,	"Intellect",	AE_MANA_PERCENT,	0,	6,	 7,	0,	0,	0,	0,	 27,	IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 2'100,	 2'500,	 5,	15,	 21 },
	{ 468,	"Awakening",	AE_MANA_PERCENT,	0,	8,	 9,	0,	0,	0,	0,	 51,	IA_HELM,	IS_MAGIC,	0,	0,	1,	 5'200,	 6'000,	 7,	24,	 31 },
	{ 468,	"Awakening",	AE_MANA_PERCENT,	0,	9,	10,	0,	0,	0,	0,	 92,	IA_HELM,	IS_MAGIC,	0,	0,	1,	 8'200,	 9'000,	 9,	55,	 59 },
	{ 468,	"Afflatus",		AE_MANA_PERCENT,	0,	11,	12,	0,	0,	0,	0,	182,	IA_HELM,	IS_MAGIC,	0,	0,	1,	21'000,	25'000,	12,	99,	113 },


	/* ------------------------------------
		   NONSENSE !!! << REWORK!
	-------------------------------------*/

	//	{ 0,	"To Weapon",	AE_WEAPON_PERCENT,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
	//	{ 0,	"To Attack Speed",	AE_ATTACK_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
	//	{ 0,	"To Cast Speed",	AE_CAST_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },

		{ 0,	"",	AE_NO },
};
// next new rare affix id = ?

//----- (th2) -------------------------------------------------------------
char* generator_armour_belts( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* nm2[] = { "Ancestors", "Bloodlust", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom",
		"Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares",
		"Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices",
		"Warlords", "Wars", "Whispers", "Worlds" };
	static const char* nm3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished",
		"Captain's", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation",
		"Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction",
		"Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost",
		"Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering",
		"Incarnated", "Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning",
		"Lonely", "Loyal", "Lusting", "Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's",
		"Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime",
		"Primitive", "Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated",
		"Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing",
		"Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm",
		"Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul",
		"Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged",
		"Warden's", "Warlord's", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian",
		"Silver", "Skeletal", "Steel", "Titanium", "Demon" };
	static const char* nm5_1[] = { "Belt", "Belt", "Girdle", "Links", "Waistband", "Waistguard", "Band", "Chain", "Cord" };
	static const char* nm6[] = { "Absorption", "Heavy Loads", "Carrying", "Heavy Pants", "Perseverance", "Firmness", "Stability", "Steady Hands",
		"Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger",
		"Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults",
		"Auras", "Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes",
		"Blessings", "Blight", "Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Worlds",
		"Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger",
		"Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay", "Deception", "Defiance", "Deflection", "Delirium", "Delusions",
		"Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion", "Diligence", "Discipline", "Dishonor", "Dismay",
		"Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence", "Duels", "Dusk", "Echoes", "Enchantments",
		"Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation", "Eternal Glory", "Eternal Justice",
		"Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories", "Fallen Kings",
		"Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed",
		"Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes",
		"Holy Might", "Honor", "Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials",
		"Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending", "Lifestealing", "Light's Hope", "Limbo", 
		"Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles", "Misery", "Mists", "Moonlight",
		"Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms", "Power", "Prayers",
		"Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands",
		"Silence", "Slaughter", "Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering",
		"Suffering's End", "Sunfire", "Sunlight", "Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders",
		"Titans", "Torment", "Traitors", "Trust", "Truth", "Truths", "Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight",
		"Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War", "Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal",
		"the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast", "the Berserker", "the Blessed", "the Boar",
		"the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion", "the Claw", "the Corrupted",
		"the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths", "the Dragons",
		"the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator",
		"the Gods", "the Harvest", "the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King",
		"the Knight", "the Lasting Night", "the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf",
		"the Lost", "the Mage", "the Moon", "the Moonwalker", "the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker",
		"the North", "the Occult", "the Oracle", "the Phoenix", "the Plague", "the Prince", "the Princess", "the Prisoner", "the Prodigy",
		"the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer", "the Serpent", "the Setting Sun", "the Shadows",
		"the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun", "the Sunwalker", "the Swamp", "the Talon",
		"the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West", "the Whale", "the Whispers",
		"the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };

	static const char* nm4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Rugged Leather", "Scaled", "Silk",
		"Wool", "Embroided" };
	static const char* nm5_2[] = { "Belt", "Sash", "Strap", "Girdle", "Waistband", "Cord", "belt" };

	const char** nm4 = type == 1 ? nm4_2 : nm4_1; int count_nm4 = type == 1 ? countof( nm4_2 ) : countof( nm4_1 );
	const char** nm5 = type == 1 ? nm5_2 : nm5_1; int count_nm5 = type == 1 ? countof( nm5_2 ) : countof( nm5_1 );

	int i = RNG(10);
	{
		int rnd = RNG(countof(nm1));
		int rnd2 = RNG(countof(nm2));
		int rnd3 = RNG(countof(nm3));
		int rnd4 = RNG(count_nm4);
		int rnd5 = RNG(count_nm5);
		int rnd6 = RNG(countof(nm6));
		if( i < 2 ){
			sprintf_s( names, "%s of %s %s", nm5[ rnd5 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 4 ){
			sprintf_s( names, "%s %s of %s %s", nm4[ rnd4 ], nm5[ rnd5 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 7 ){
			sprintf_s( names, "%s %s %s", nm3[ rnd3 ], nm4[ rnd4 ], nm5[ rnd5 ] );
		} else{
			sprintf_s( names, "%s %s of %s", nm3[ rnd3 ], nm5[ rnd5 ], nm6[ rnd6 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_armour_boots( int type )
{
	static char names[ 128 ];	*names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Bitterness", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes",
		"Freedom", "Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery",
		"Nightmares", "Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions",
		"Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted",
		"Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom",
		"Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful",
		"Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost", "Frozen", "Furious", "Fusion",
		"Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's", "Hatred", "Haunted", "Heartless",
		"Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated", "Infused", "Inherited",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare",
		"Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive",
		"Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated",
		"Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing",
		"Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm",
		"Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul",
		"Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged",
		"Warden's", "Warlord's", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* names4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril",
		"Obsidian", "Silver", "Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* names5_1[] = { "Boots", "Greaves", "Stompers", "Greatboots", "Treads", "Spurs", "Warboots", "Sabatons", "Feet",
		"Walkers", "Footguards" };
	static const char* names7[] = { "Absorption", "Steadiness", "Water Walking", "Perseverance", "Firmness", "Stability", "the Elephant",
		"the Mammoth", "Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers",
		"Anger", "Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins",
		"Assaults", "Auras", "Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune",
		"Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams",
		"Broken Limbs", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage",
		"Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay", "Deception", "Defiance",
		"Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion", "Diligence",
		"Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction",
		"Faded Memories", "Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Folly", "Forging",
		"Fortitude", "Fortune", "Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory",
		"Grace", "Greed", "Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment",
		"Heroes", "Holy Might", "Honor", "Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials",
		"Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending", "Lifestealing", "Light's Hope", "Limbo", "Lost Cities", "Lost Hope",
		"Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles", "Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature",
		"Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms", "Power", "Prayers", "Pride", "Pride's Fall", "Prophecies",
		"Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets", "Resilience", "Respect", "Riddles", "Ruins",
		"Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter", "Slaying", "Smite", "Solitude",
		"Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight", "Swordbreaking",
		"Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor",
		"Visions", "War", "Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk",
		"the Bear", "the Beast", "the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun",
		"the Caged Mind", "the Cataclysm", "the Champion", "the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned",
		"the Day", "the Daywalker", "the Dead", "the Depth", "the Depths", "the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East",
		"the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen", "the Falling Sky", "the Flames", "the Forest", "the Forests",
		"the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest", "the Hunter", "the Immortal", "the Immortals",
		"the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night", "the Leviathan", "the Light", "the Lion",
		"the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker", "the Mountain", "the Mountains",
		"the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague", "the Prince",
		"the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner",
		"the Sun", "the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward",
		"the Warrior", "the West", "the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };
		
	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather",
		"Scaled", "Silk", "Wool", "Embroided" };
	static const char* names5_2[] = { "Sprinters", "Sandals", "Boots", "Heels", "Treads", "Feet", "Footguards", "Footpads", "Walkers", "Footsteps" };
	
	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );

	int i = RNG(10);
	{
		if( i < 2 ){
			int rnd0 = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd0 = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names4);
			int rnd3 = RNG(count_names5);
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 7 ){
			int rnd0 = RNG(countof(names3));
			int rnd1 = RNG(count_names4);
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s %s %s", names3[ rnd0 ], names4[ rnd1 ], names5[ rnd2 ] );
		} else{
			int rnd0 = RNG(countof(names3));
			int rnd1 = RNG(count_names5);
			int rnd2 = RNG(countof(names7));
			sprintf_s( names, "%s %s of %s", names3[ rnd0 ], names5[ rnd1 ], names7[ rnd2 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_armour_chests( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned",
		"Dark", "Demonic", "Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed",
		"Haunted", "Hellish", "Holy", "Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered",
		"Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* nm2[] = { "Ancestors", "Bloodlust", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom",
		"Glory", "Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares",
		"Nights", "Power", "Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices",
		"Warlords", "Wars", "Whispers", "Worlds" };
	static const char* nm3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", 
		"Crazed", "Crying", "Cursed", "Defender", "Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", 
		"Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved", "Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful",
		"Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's", "Frenzied", "Frost", "Frozen", "Furious", "Fusion",
		"Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's", "Hatred", "Haunted", "Heartless",
		"Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated", "Infused", "Inherited",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare",
		"Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised",
		"Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution",
		"Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's",
		"Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless", "Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury",
		"Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented",
		"Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful",
		"Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's", "Warped", "Warrior", "Warrior's",
		"Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* nm5_1[] = { "Vest", "Silks", "Armor", "Coat", "Outfit", "Garb", "Guise", "Shell", "Shroud", "Chestpiece", "Robe","Carapace", "Harness" };
	static const char* nm6_1[] = { "Armament", "Scales", "Guard", "Bastion", "Steel", "Defense", "Bane", "Bond", "Boon", "Call", "Champion", "Conqueror", "Crusader",
		"Cry", "Curator", "Dawn", "Fall", "End", "Demise", "Birth", "Death", "Edge", "Blight", "Burden", "Blessing", "Fortune", "Defender", "Defiler", "Deflector",
		"Destroyer", "Emissary", "Ender", "Favor", "Foe", "Gift", "Glory", "Guard", "Guardian", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Last Hope",
		"Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward",
		"Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward", "Warden", "Whisper", "Wit", "Vest", "Chestguard", "Armor", "Tunic", };
	static const char* nm7[] = { "Absorption", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger", "Anguish", "Annihilation",
		"Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras", "Awareness", "Barriers",
		"Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust",
		"Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Cities", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity",
		"Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay",
		"Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion",
		"Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories",
		"Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed", "Grieving Widows",
		"Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor", "Hope", "Horrors",
		"Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending",
		"Lifestealing", "Light's Hope", "Limbo", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles",
		"Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest",
		"the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night",
		"the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker",
		"the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague",
		"the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun",
		"the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West",
		"the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };

	static const char* nm4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled", "Silk",
		"Wool", "Embroided" };
	static const char* nm5_2[] = { "Guise", "Tunic", "Wraps", "Vest", "Vestment", "Robes", "Garb", "Jerkin", "Raiment", "Garments", "Mantle", "Shroud", "Gown",  "Clothing", "Vesture",
		"Attire", "Uniform", "Outfit" };
	static const char* nm6_2[] = { "Armament", "Bane", "Bond", "Boon", "Call", "Champion", "Conqueror", "Crusader", "Cry", "Curator", "Dawn", "Fall", "End", "Demise",
		"Birth", "Death",		"Edge", "Blight", "Burden", "Blessing", "Fortune", "Defender", "Defiler", "Deflector", "Destroyer", "Emissary", "Ender", "Favor",
		"Foe", "Gift", "Glory", "Guard", "Guardian", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Last Hope", "Last Stand", "Legacy", "Memory", "Might",
		"Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance",
		"Vindicator", "Visage", "Voice", "Ward", "Warden", "Whisper", "Wit", "Breastplate", "Tunic", "Wraps", "Vest", "Vestment", "Robes", "Chestguard", "Jerkin",
		"Raiment", "Garments" };
	
	const char** nm4 = type == 1 ? nm4_2 : nm4_1; int count_nm4 = type == 1 ? countof( nm4_2 ) : countof( nm4_1 );
	const char** nm5 = type == 1 ? nm5_2 : nm5_1; int count_nm5 = type == 1 ? countof( nm5_2 ) : countof( nm5_1 );
	const char** nm6 = type == 1 ? nm6_2 : nm6_1; int count_nm6 = type == 1 ? countof( nm6_2 ) : countof( nm6_1 );

	if( type == 1 ){
	}

	int i = RNG(10);
	{
		if( i < 2 ){
			int rnd = RNG(countof(nm1));
			int rnd2 = RNG(countof(nm2));
			int rnd3 = RNG(count_nm5);
			sprintf_s( names, "%s of %s %s", nm5[ rnd3 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 4 ){
			int rnd = RNG(countof(nm1));
			int rnd2 = RNG(countof(nm2));
			int rnd3 = RNG(count_nm4);
			int rnd4 = RNG(count_nm5);
			sprintf_s( names, "%s %s of %s %s", nm4[ rnd3 ], nm5[ rnd4 ], nm1[ rnd ], nm2[ rnd2 ] );
		} else if( i < 6 ){
			int rnd = RNG(countof(nm3));
			int rnd2 = RNG(count_nm4);
			int rnd3 = RNG(count_nm5);
			sprintf_s( names, "%s %s %s", nm3[ rnd ], nm4[ rnd2 ], nm5[ rnd3 ] );
		} else if( i < 8 ){
			int rnd = RNG(countof(nm3));
			int rnd2 = RNG(count_nm5);
			int rnd3 = RNG(countof(nm7));
			sprintf_s( names, "%s %s of %s", nm3[ rnd ], nm5[ rnd2 ], nm7[ rnd3 ] );
		} else{
			int rnd = RNG(count_nm6);
			int rnd2 = RNG(countof(nm7));
			sprintf_s( names, "%s of %s", nm6[ rnd ], nm7[ rnd2 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_armour_gauntlets( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned", "Dark", "Demonic",
		"Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed", "Haunted", "Hellish", "Holy", "Imminent",
		"Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered", "Silent", "Smoldering", "Timeless", "Twisted", "Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom", "Glory", "Hell",
		"Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares", "Nights", "Power", "Powers",
		"Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blood Infused",
		"Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's", "Cataclysm", "Cataclysmic",
		"Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender",
		"Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved",
		"Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated",
		"Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous",
		"Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's",
		"Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged",
		"Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard",
		"Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy",
		"Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's", "Warped",
		"Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* names4_1[] = { "Adamantite", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Mail", "Chain", "Chainmail", "Titanium", "Demon" };
	static const char* names5_1[] = { "Gauntlets", "Grips", "Gloves", "Handguards", "Fists", "Hands", "Grasps", "Warfists" };
	static const char* names7[] = { "Absorption", "Grips", "Tight Grips", "the Archer", "Archer", "Keen Aim", "Precision", "Perseverance", "Firmness", "Stability",
		"Steady Hands", "Fidelity", "Silence", "Muffled Steps", "Fleet Feet", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger",
		"Anguish", "Annihilation", "Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras",
		"Awareness", "Barriers", "Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight",
		"Blood", "Bloodlust", "Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Worlds", "Burdens", "Carnage", "Cataclysms",
		"Chaos", "Clarity", "Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness",
		"Dawn", "Decay", "Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction",
		"Devotion", "Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams",
		"Due Diligence", "Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust",
		"Eternal Damnation", "Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions",
		"Extinction", "Faded Memories", "Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging",
		"Fortitude", "Fortune", "Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed",
		"Grieving Widows", "Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor",
		"Hope", "Horrors", "Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life",
		"Lifemending", "Lifestealing", "Light's Hope", "Limbo", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might",
		"Miracles", "Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods", "the Harvest",
		"the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight", "the Lasting Night",
		"the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage", "the Moon", "the Moonwalker",
		"the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult", "the Oracle", "the Phoenix", "the Plague",
		"the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector", "the Queen", "the Scourge", "the Seer",
		"the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm", "the Summoner", "the Sun",
		"the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward", "the Warrior", "the West",
		"the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wretched" };
	
	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled",
		"Silk", "Wool", "Embroided" };
	static const char* names5_2[] = { "Gauntlets", "Grips", "Gloves", "Handguards", "Fists", "Hands", "Grasps", "Stranglers", "Handwraps", "Touch" };
	
	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );

	int i = RNG(10);
	{
		if( i < 2 ){
			int rnd = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names4);
			int rnd3 = RNG(count_names5);
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd ], names2[ rnd1 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(names3));
			int rnd1 = RNG(count_names4);
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s %s %s", names3[ rnd ], names4[ rnd1 ], names5[ rnd2 ] );
		} else{
			int rnd = RNG(countof(names3));
			int rnd1 = RNG(count_names5);
			int rnd2 = RNG(countof(names7));
			sprintf_s( names, "%s %s of %s", names3[ rnd ], names5[ rnd1 ], names7[ rnd2 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_armour_helmets( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* names1[] = { "Ancient", "Binding", "Blessed", "Blind", "Broken", "Burning", "Condemned", "Conquered", "Cursed", "Damned", "Dark", "Demonic",
		"Distant", "Divine", "Doomed", "Ending", "Endless", "Eternal", "Faded", "Fallen", "Fleeting", "Frozen", "Hallowed", "Haunted", "Hellish", "Holy",
		"Imminent", "Immortal", "Infernal", "Infinite", "Lost", "Ominous", "Relentless", "Sacred", "Shattered", "Silent", "Smoldering", "Timeless", "Twisted",
		"Unholy" };
	static const char* names2[] = { "Ancestors", "Bloodlust", "Damnation", "Dreams", "Fire", "Fires", "Fortune", "Fortunes", "Freedom", "Glory",
		"Hell", "Hells", "Honor", "Hope", "Illusions", "Justice", "Kings", "Lands", "Magic", "Memories", "Might", "Misery", "Nightmares", "Nights", "Power",
		"Powers", "Protection", "Punishment", "Sorrow", "Souls", "Torment", "Trials", "Vengeance", "Visions", "Voices", "Warlords", "Wars", "Whispers", "Worlds" };
	static const char* names3[] = { "Ancient", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blood Infused",
		"Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Brutal", "Brutality", "Burnished", "Captain's", "Cataclysm", "Cataclysmic",
		"Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corrupted", "Crazed", "Crying", "Cursed", "Defender",
		"Defender's", "Defiled", "Demonic", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragon", "Ebon", "Enchanted", "Engraved",
		"Eternal", "Exile", "Extinction", "Faith's", "Faithful", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Forsaken", "Fortune's",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving", "Guard's", "Guardian's",
		"Hatred", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Incarnated",
		"Infused", "Inherited", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lusting",
		"Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous",
		"Peacekeeper", "Peacekeeper's", "Phantom", "Possessed", "Pride's", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's",
		"Reforged", "Reincarnated", "Relentless", "Remorse", "Renewed", "Renovated", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged",
		"Savage", "Sentinel", "Shadow", "Silent", "Singing", "Sinister", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spite", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard",
		"Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead",
		"Unholy", "Vanquisher", "Vengeance", "Vengeful", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "War-Forged", "Warden's", "Warlord's",
		"Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* names4_1[] = { "Adamantite", "Plate", "Scaled", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Golden", "Iron", "Mithril", "Obsidian",
		"Silver", "Skeletal", "Steel", "Titanium", "Demon" };
	static const char* names5_1[] = { "Casque", "Crown", "Faceguard", "Gaze", "Greathelm", "Headcover", "Headguard", "Helm", "Helmet", "Jaws", "Visage",
	"Faceplate", "Visor", "Cap", "Grin", "Nimbus", "Cover" };
	static const char* names6_1[] = { "Armament", "Bane", "Birth", "Blessing", "Blight", "Bond", "Boon", "Burden", "Call", "Casque", "Champion", "Conqueror",
		"Crown", "Crusader", "Cry", "Curator", "Dawn", "Death", "Defender", "Defiler", "Deflector", "Demise", "Destroyer", "Edge", "Emissary", "End", "Ender",
		"Faceguard", "Fall", "Favor", "Foe", "Fortune", "Gaze", "Gift", "Glory", "Greathelm", "Guard", "Guardian", "Headcover", "Headguard", "Heirloom", "Helm",
		"Helmet", "Hero", "Hope", "Incarnation", "Jaws", "Keeper", "Last Hope", "Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise",
		"Protection", "Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward",
		"Warden", "Whisper", "Wit" };
	static const char* names7[] = { "Absorption", "the Phoenix", "Adventure", "Agony", "Ancient Power", "Ancient Powers", "Anger", "Anguish", "Annihilation",
		"Arcane Magic", "Arcane Power", "Arcane Resist", "Archery", "Ashes", "Assassination", "Assassins", "Assaults", "Auras", "Awareness", "Barriers",
		"Beginnings", "Binding", "Black Magic", "Blast Protection", "Blessed Fortune", "Blessed Fortunes", "Blessings", "Blight", "Blood", "Bloodlust",
		"Bloodshed", "Bravery", "Broken Bones", "Broken Dreams", "Broken Worlds", "Burdens", "Carnage", "Cataclysms", "Chaos", "Clarity",
		"Conquered Worlds", "Corruption", "Courage", "Creation", "Cunning", "Danger", "Dark Magic", "Dark Powers", "Dark Souls", "Darkness", "Dawn", "Decay",
		"Deception", "Defiance", "Deflection", "Delirium", "Delusions", "Demon Fire", "Demons", "Denial", "Desecration", "Despair", "Destruction", "Devotion",
		"Diligence", "Discipline", "Dishonor", "Dismay", "Dominance", "Domination", "Doom", "Dragons", "Dragonsouls", "Dread", "Dreams", "Due Diligence",
		"Duels", "Dusk", "Echoes", "Enchantments", "Ended Dreams", "Ending Hope", "Ending Misery", "Ends", "Eternal Bloodlust", "Eternal Damnation",
		"Eternal Glory", "Eternal Justice", "Eternal Rest", "Eternal Sorrow", "Eternal Struggles", "Eternity", "Executions", "Extinction", "Faded Memories",
		"Fallen Kings", "Fallen Souls", "Fire", "Fire Magic", "Fire Power", "Fire Protection", "Fire Resist", "Fools", "Forging", "Fortitude", "Fortune",
		"Frost", "Frost Power", "Frost Resist", "Frozen Hells", "Fury", "Ghosts", "Giants", "Giantslaying", "Glory", "Grace", "Greed", "Grieving Widows",
		"Guardians", "Hate", "Hatred", "Healing", "Hell", "Hell's Games", "Hellfire", "Hellish Torment", "Heroes", "Holy Might", "Honor", "Hope", "Horrors",
		"Ice", "Ice Magic", "Illusions", "Immortality", "Inception", "Infinite Trials", "Infinity", "Insanity", "Justice", "Kings", "Life", "Lifemending",
		"Lifestealing", "Light's Hope", "Limbo", "Lost Hope", "Lost Souls", "Lost Voices", "Lost Worlds", "Mercy", "Might", "Miracles",
		"Misery", "Mists", "Moonlight", "Mysteries", "Mystery", "Nature", "Necromancy", "Nightmares", "Oblivion", "Paradise", "Patience", "Phantoms",
		"Power", "Prayers", "Pride", "Pride's Fall", "Prophecies", "Protection", "Putrefaction", "Reckoning", "Recoil", "Redemption", "Regret", "Regrets",
		"Resilience", "Respect", "Riddles", "Ruins", "Runes", "Salvation", "Secrecy", "Secrets", "Serenity", "Shadows", "Shifting Sands", "Silence", "Slaughter",
		"Slaying", "Smite", "Solitude", "Souls", "Stealth", "Stone", "Storms", "Strength", "Subtlety", "Suffering", "Suffering's End", "Sunfire", "Sunlight",
		"Swordbreaking", "Tears", "Terror", "Terrors", "Thieves", "Thorns", "Thunder", "Thunders", "Titans", "Torment", "Traitors", "Trust", "Truth", "Truths",
		"Twilight", "Twilight's End", "Twisted Visions", "Undoing", "Unholy Blight", "Unholy Might", "Valiance", "Valor", "Vengeance", "Vigor", "Visions", "War",
		"Whispers", "Wisdom", "Woe", "Wonders", "Wraiths", "Zeal", "the Ancients", "the Archer", "the Banished", "the Basilisk", "the Bear", "the Beast",
		"the Berserker", "the Blessed", "the Boar", "the Breaking Storm", "the Brotherhood", "the Burning Sun", "the Caged Mind", "the Cataclysm", "the Champion",
		"the Claw", "the Corrupted", "the Covenant", "the Crown", "the Crusader", "the Damned", "the Day", "the Daywalker", "the Dead", "the Depth", "the Depths",
		"the Dragons", "the Dreadlord", "the Eagle", "the Earth", "the East", "the Eclipse", "the Emperor", "the End", "the Enigma", "the Fallen",
		"the Falling Sky", "the Flames", "the Forest", "the Forests", "the Forgotten", "the Forsaken", "the Gargoyle", "the Gladiator", "the Gods",
		"the Harvest", "the Hunter", "the Immortal", "the Immortals", "the Incoming Storm", "the Insane", "the Isles", "the King", "the Knight",
		"the Lasting Night", "the Leviathan", "the Light", "the Lion", "the Lionheart", "the Lone Victor", "the Lone Wolf", "the Lost", "the Mage",
		"the Moon", "the Moonwalker", "the Mountain", "the Mountains", "the Night", "the Night Sky", "the Nightstalker", "the North", "the Occult",
		"the Oracle", "the Phoenix", "the Plague", "the Prince", "the Princess", "the Prisoner", "the Prodigy", "the Prophecy", "the Prophet", "the Protector",
		"the Queen", "the Scourge", "the Seer", "the Serpent", "the Setting Sun", "the Shadows", "the Sky", "the South", "the Stars", "the Steward", "the Storm",
		"the Summoner", "the Sun", "the Sunwalker", "the Swamp", "the Talon", "the Titans", "the Undying", "the Victor", "the Void", "the Volcano", "the Ward",
		"the Warrior", "the West", "the Whale", "the Whispers", "the Wicked", "the Wind", "the Wolf", "the World", "the Wounded Stars", "the Wretched" };
	static const char* names4_2[] = { "Cloth", "Heavy Hide", "Heavy Leather", "Hide", "Leather", "Linen", "Padded", "Quilted", "Rugged Leather", "Scaled",
		"Silk", "Wool" };
	static const char* names5_2[] = { "Bandana", "Cap", "Coif", "Cowl", "Facemask", "Headguard", "Headpiece", "Helm", "Hood", "Mask" };
	static const char* names6_2[] = { "Armament", "Bandana", "Bane", "Birth", "Blessing", "Blight", "Bond", "Boon", "Burden", "Call", "Cap", "Champion", "Coif",
		"Conqueror", "Cowl", "Crusader", "Cry", "Curator", "Dawn", "Death", "Defender", "Defiler", "Deflector", "Demise", "Destroyer", "Edge", "Emissary", "End",
		"Ender", "Facemask", "Fall", "Favor", "Foe", "Fortune", "Gift", "Glory", "Guard", "Guardian", "Headguard", "Headpiece", "Heirloom", "Helm", "Hero", "Hood",
		"Hope", "Incarnation", "Keeper", "Last Hope", "Last Stand", "Legacy", "Mask", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection",
		"Protector", "Reach", "Shepherd", "Soul", "Steward", "Terror", "Token", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Ward", "Warden",
		"Whisper", "Wit" };

	const char** names4 = type == 2 ? names4_2 : names4_1; int count_names4 = type == 2 ? countof( names4_2 ) : countof( names4_1 );
	const char** names5 = type == 2 ? names5_2 : names5_1; int count_names5 = type == 2 ? countof( names5_2 ) : countof( names5_1 );
	const char** names6 = type == 2 ? names6_2 : names6_1; int count_names6 = type == 2 ? countof( names6_2 ) : countof( names6_1 );

	int i = RNG(10);
	{
		if( i < 2 ){
			int rnd0 = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s of %s %s", names5[ rnd2 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 4 ){
			int rnd0 = RNG(countof(names1));
			int rnd1 = RNG(countof(names2));
			int rnd2 = RNG(count_names4);
			int rnd3 = RNG(count_names5);
			sprintf_s( names, "%s %s of %s %s", names4[ rnd2 ], names5[ rnd3 ], names1[ rnd0 ], names2[ rnd1 ] );
		} else if( i < 6 ){
			int rnd0 = RNG(countof(names3));
			int rnd1 = RNG(count_names4);
			int rnd2 = RNG(count_names5);
			sprintf_s( names, "%s %s %s", names3[ rnd0 ], names4[ rnd1 ], names5[ rnd2 ] );
		} else if( i < 8 ){
			int rnd0 = RNG(countof(names3));
			int rnd1 = RNG(count_names5);
			int rnd2 = RNG(countof(names7));
			sprintf_s( names, "%s %s of %s", names3[ rnd0 ], names5[ rnd1 ], names7[ rnd2 ] );
		} else{
			int rnd0 = RNG(count_names6);
			int rnd1 = RNG(countof(names7));
			sprintf_s( names, "%s of %s", names6[ rnd0 ], names7[ rnd1 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_armour_shields()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Aegeus", "Alpha", "Amnesia", "Anguish", "Apocalypse", "Armageddon", "Barrage", "Bear", "Behemoth", "Berg", "Betrayal",
		"Blackest Heart", "Blackout", "Blazeguard", "Blind Hate", "Blind Hatred", "Bloodlust", "Bombardment", "Broken Promise", "Calamity", "Cataclysm",
		"Catastrophe", "Cerberus", "Chaos", "Chasm", "Coliseum", "Colossus", "Combustion", "Cometfall", "Convergence", "Curator", "Cyclone", "Darkheart",
		"Darkness", "Dawn", "Dawnbreaker", "Dawnguard", "Death's Bargain", "Death's Deflection", "Death's Touch", "Deathgate", "Deluge", "Desolation",
		"Despair", "Destiny's Song", "Destiny's Will", "Devinity", "Divine Light", "Domination", "Doom", "Dragonheart", "Dragonscale", "Due Diligence",
		"Dusk", "Ebony", "Echo", "Eclipse", "Enforcer", "Enigma", "Epilogue", "Eruption", "Eulogy", "Extinction", "Face of Death", "Face of Insanity",
		"Faithkeeper", "Fate", "Final Favor", "Ghostwalker", "Glacier", "Glimmer", "Gloom", "Headache", "Hell's Scream", "Hero's Calling", "Hero's Stand",
		"Hero's Surrender", "Honor's Call", "Honor's End", "Honor's Grasp", "Honor's Guard", "Honor's Pride", "Honor's Will", "Hope", "Interrogator", "Ironbark",
		"Ivory", "Justice", "King's Defender", "King's Honor", "King's Legacy", "Kingdom's Heart", "Kingdom's Honor", "Kingdom's Pride", "Knight's Fall",
		"Knightfall", "Lament", "Last Rites", "Last Words", "Lazarus", "Lightbringer", "Limbo", "Lionheart", "Lost Paradise", "Loyalty", "Maelstrom", "Malice",
		"Mercy", "Misery's End", "Monster", "Moonlight", "Nethersbane", "Night's End", "Night's Fall", "Nightbane", "Nightfall", "Nirvana", "Oathbreaker",
		"Oathkeeper", "Oathsworn", "Oblivion", "Ogre", "Omega", "Overture", "Painkiller", "Path of Exile", "Peace Maker", "Peacekeeper", "Perfect Storm",
		"Permission", "Persuasion", "Prelude", "Pride's Honor", "Prologue", "Purifier", "Rage", "Ragnarok", "Rand", "Reckoning", "Red Dwarf", "Redemption",
		"Reign Breaker", "Reign Maker", "Remorse", "Requiem", "Retirement", "Ruin", "Rumpel Steelskin", "Sanctify", "Scar", "Selfreflection", "Shepherd",
		"Shroud", "Sierra", "Silence", "Silverlight", "Skullcrusher", "Stalker", "Starlight", "Steelskin", "Storm", "Storm Breaker", "Stormbringer", "Stormcaller",
		"Sunward", "Supernova", "Supremacy", "Suspension", "Swan Song", "The Abyss", "The Ambassador", "The Barricade", "The Beast", "The Black Hole",
		"The Blockade", "The Boon", "The Brute", "The Curator", "The End", "The Grand Slam", "The Iron Maid", "The Iron Maiden", "The Keeper", "The Light",
		"The Mirror", "The Mountain", "The Observer", "The Oculus", "The Righteous", "The Sentinel", "The Sentry", "The Shadow", "The Titan", "The Voice",
		"The Void", "Thunder", "Thundercloud", "Torrent", "Tranquility", "Treachery", "Trinity", "Triumph", "Truthbreaker", "Typhoon", "Tyranny", "Vacancy",
		"Vacuum", "Valkyrie", "Vanquisher", "Vengeance", "Vigilant", "Vigilante", "Voice of Honor", "Voice of Insanity", "Voice of Madness", "Voice of Pride",
		"Voice of Reason", "Volcano", "Vortex", "Wall of Honor", "Wall of Insanity", "Wall of Madness", "Wall of Pain", "Wall of Pride", "Wall of Sorrow",
		"Wall of the Dead", "Warcry", "Warmonger", "Warsong", "Willbreaker", "Wisdom's Grasp", "Wisdom's Hold", "Wit's End", "Witherbrand" };
	static const char* nm2[] = { "Ancient", "Antique", "Arcane", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn",
		"Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror", "Conqueror's", "Corroded", "Corrupted",
		"Crazed", "Crying", "Curator", "Cursed", "Curved", "Dancing", "Defender", "Defender's", "Defiled", "Deluded", "Demonic", "Deserted", "Desire's",
		"Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted", "Engraved", "Eternal", "Exiled",
		"Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused", "Firesoul", "Flimsy", "Forsaken", "Fortune's",
		"Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Grieving",
		"Guard's", "Guardian's", "Hailstorm", "Hateful", "Haunted", "Heartless", "Hero", "Hero's", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless",
		"Howling", "Hungering", "Improved", "Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Legionnaire's",
		"Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful", "Lusting", "Malevolent", "Malicious", "Malignant", "Massive", "Mended", "Mercenary",
		"Military", "Misfortune's", "Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed",
		"Pride's", "Prideful", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless",
		"Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Savage", "Sentinel",
		"Shadow", "Silent", "Singing", "Sinister", "Skyfall", "Smooth", "Soldier's", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulless",
		"Spectral", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury",
		"Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical",
		"Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful", "Venom", "Vicious", "Victor", "Vindication", "Vindicator", "Vindictive", "Volcanic", "Vowed",
		"War-Forged", "Warden's", "Warlord's", "Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful",
		"Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Ashwood", "Bone", "Bronze", "Bronzed", "Copper", "Ivory", "Ebon", "Glass", "Golden", "Hardwood", "Iron", "Ironbark",
		"Maple", "Mithril", "Oak", "Obsidian", "Redwood", "Silver", "Skeletal", "Steel", "Titanium", "Warpwood", "Willow", "Yew" };
	static const char* nm4[] = { "Shield", "Bulwark", "Carapace", "Ward", "Wall", "Visage", "Armament", "Barrier", "Greatshield", "Barricade", "Blockade",
		"Kite Shield", "Buckler", "Heater", "Light Shield", "Heavy Shield", "Tower Shield", "Shield Wall", "Buffer", "Guard", "Aegis", "Warden", "Guardian",
		"Keeper", "Defender", "Bastion" };
	static const char* nm5[] = { "Aegis", "Angel", "Armament", "Bane", "Barricade", "Barrier", "Bastion", "Blockade", "Bond", "Boon", "Breaker", "Bringer",
		"Buckler", "Buffer", "Bulwark", "Call", "Caretaker", "Champion", "Conqueror", "Conservator", "Crusader", "Cry", "Cunning", "Curator", "Dawn",
		"Defender", "Defiler", "Deflector", "Destroyer", "Emissary", "Ender", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatshield", "Guard", "Guardian",
		"Heater", "Heavy Shield", "Heirloom", "Hero", "Hope", "Incarnation", "Keeper", "Kite Shield", "Last Hope", "Last Stand", "Legacy", "Light Shield",
		"Memory", "Might", "Oath", "Pact", "Pledge", "Promise", "Protection", "Protector", "Reach", "Savagery", "Secret", "Shepherd", "Shield", "Shield Wall",
		"Soul", "Steward", "Terror", "Token", "Tower Shield", "Tribute", "Vengeance", "Vindicator", "Visage", "Voice", "Wall", "Ward", "Warden", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune",
		"of Blessings", "of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos",
		"of Closing Eyes", "of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay",
		"of Deception", "of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence",
		"of Echoes", "of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory",
		"of Eternal Justice", "of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories",
		"of Fallen Souls", "of Fools", "of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate",
		"of Hatred", "of Hell's Games", "of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors",
		"of Illuminated Dreams", "of Illumination", "of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice",
		"of Light's Hope", "of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains",
		"of Mourning", "of Mystery", "of Necromancy", "of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall",
		"of Putrefaction", "of Reckoning", "of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows",
		"of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering",
		"of Suffering's End", "of Summoning", "of Terror", "of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment",
		"of Traitors", "of Trembling Hands", "of Trials", "of Truth", "of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might",
		"of Vengeance", "of Visions", "of Wasted Time", "of Widows", "of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished",
		"of the Basilisk", "of the Beast", "of the Blessed", "of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind",
		"of the Cataclysm", "of the Champion", "of the Claw", "of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker",
		"of the Dead", "of the Depth", "of the Dreadlord", "of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma",
		"of the Fallen", "of the Falling Sky", "of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest",
		"of the Immortal", "of the Incoming Storm", "of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion",
		"of the Lionheart", "of the Lone Victor", "of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night",
		"of the Nightstalker", "of the North", "of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector",
		"of the Queen", "of the Serpent", "of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner",
		"of the Sun", "of the Sunwalker", "of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked",
		"of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Aegeus", "Alpha", "Amnesia", "Anguish", "Apocalypse", "Armageddon", "Barrage", "Bear", "Behemoth", "Berg", "Betrayal",
		"Blackout", "Blazeguard", "Blind Hate", "Blind Hatred", "Bloodlust", "Bombardment", "Broken Promise", "Calamity", "Cataclysm", "Catastrophe", "Cerberus",
		"Chaos", "Chasm", "Coliseum", "Colossus", "Combustion", "Cometfall", "Convergence", "Curator", "Cyclone", "Darkheart", "Darkness", "Dawn", "Dawnbreaker",
		"Dawnguard", "Death's Bargain", "Deathgate", "Deluge", "Desolation", "Despair", "Destiny's Song", "Destiny's Will", "Devinity", "Divine Light",
		"Domination", "Doom", "Dragonheart", "Dragonscale", "Due Diligence", "Dusk", "Ebony", "Echo", "Eclipse", "Enforcer", "Enigma", "Epilogue", "Eruption",
		"Eulogy", "Extinction", "Faithkeeper", "Fate", "Final Favor", "Ghostwalker", "Glacier", "Glimmer", "Gloom", "Headache", "Hell's Scream", "Hero's Calling",
		"Hero's Stand", "Hero's Surrender", "Honor's Call", "Honor's End", "Honor's Grasp", "Honor's Guard", "Honor's Pride", "Honor's Will", "Hope", "Interrogator",
		"Ironbark", "Ivory", "Justice", "King's Defender", "King's Honor", "King's Legacy", "Kingdom's Heart", "Kingdom's Honor", "Kingdom's Pride", "Knight's Fall",
		"Knightfall", "Lament", "Last Rites", "Last Words", "Lazarus", "Lightbringer", "Limbo", "Lionheart", "Lost Paradise", "Loyalty", "Maelstrom", "Malice",
		"Mercy", "Misery's End", "Monster", "Moonlight", "Nethersbane", "Night's End", "Night's Fall", "Nightbane", "Nightfall", "Nirvana", "Oathbreaker",
		"Oathkeeper", "Oathsworn", "Oblivion", "Ogre", "Omega", "Overture", "Painkiller", "Path of Exile", "Peace Maker", "Peacekeeper", "Perfect Storm",
		"Permission", "Persuasion", "Prelude", "Pride's Honor", "Prologue", "Purifier", "Rage", "Ragnarok", "Rand", "Reckoning", "Red Dwarf", "Redemption",
		"Reign Breaker", "Reign Maker", "Remorse", "Requiem", "Retirement", "Ruin", "Sanctify", "Scar", "Shepherd", "Shroud", "Sierra", "Silence", "Silverlight",
		"Skullcrusher", "Stalker", "Starlight", "Steelskin", "Storm", "Storm Breaker", "Stormbringer", "Stormcaller", "Sunward", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "The Abyss", "The Ambassador", "The Barricade", "The Beast", "The Black Hole", "The Blockade", "The Boon", "The Brute",
		"The Curator", "The End", "The Grand Slam", "The Iron Maid", "The Iron Maiden", "The Keeper", "The Light", "The Mirror", "The Mountain", "The Observer"
		"The Oculus", "The Righteous", "The Sentinel", "The Sentry", "The Shadow", "The Titan", "The Voice", "The Void", "Thunder", "Thundercloud", "Torrent",
		"Tranquility", "Treachery", "Trinity", "Triumph", "Truthbreaker", "Typhoon", "Tyranny", "Vacancy", "Vacuum", "Valkyrie", "Vanquisher", "Vengeance",
		"Vigilant", "Vigilante", "Voice of Honor", "Voice of Insanity", "Voice of Madness", "Voice of Pride", "Voice of Reason", "Volcano", "Vortex", "Warcry",
		"Warmonger", "Warsong", "Willbreaker", "Wisdom's Grasp", "Wisdom's Hold", "Wit's End", "Witherbrand" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_weapons_battle_axes()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abomination", "Agatha", "Allegiance", "Alpha", "Amnesia", "Anduril", "Anger's Edge", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodfury",
		"Bloodmoon", "Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Braindead", "Broken Promise", "Brutality", "Brutalizer", "Cataclysm",
		"Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Convergence", "Corpsemaker", "Corruption", "Crescent Moon",
		"Crimson", "Cutting Response", "Cyclone", "Darkness", "Dawn", "Dawn's Edge", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze", "Deathspade",
		"Decimation", "Desolation", "Despair", "Destiny", "Devastation", "Devotion", "Devourer", "Divine Light", "Dominance", "Dominion", "Doom",
		"Doom's Edge", "Doomblade", "Due Diligence", "Dusk's Edge", "Early Retirement", "Earthquake", "Echo", "Eclipse", "Edge of Death",
		"Edge of Eternity", "Edge of Infinity", "Edge of Life", "Edge of Madness", "Edge of Misery", "Edge of Reason", "Edge of Ruin", "Edge of Time", "Emergency",
		"Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Extermination", "Extinction", "Faithkeeper", "Fate", "Finality", "Fleshrender",
		"Freak's Bite", "Frenzy", "Frostbite", "Frostwind", "Fury", "Fury's Gaze", "Gatecrasher", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer",
		"Grieving Blade", "Guillotine", "Gutrender", "Harbinger", "Harvester", "Hatred", "Hatred's Blade", "Heartseeker", "Hell's Scream", "Hellfire", "Hellreaver",
		"Homage", "Honor's Call", "Hope's Edge", "Hope's End", "Humongous", "Hurricane", "Infamy", "Infinity", "Interrogator", "Justice", "Justifier",
		"King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance", "Last Laugh", "Last Rites",
		"Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Magma", "Malice",
		"Maneater", "Massacre", "Mercy", "Midnight", "Misery's Edge", "Misery's End", "Monsoon", "Mooncleaver", "Mournblade", "Narcoleptic", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightfall", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", "Orbit",
		"Orenmir", "Dream Killer", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety", "Pork-Chopper", "Pride",
		"Prophecy", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement",
		"Righteous Might", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Severance", "Shadow Strike",
		"Shadow's Edge", "Shadowfury", "Shadowmoon", "Shadowsteel", "Silence", "Skullcleaver", "Skullcrusher", "Slice of Life", "Soul Breaker", "Soul Reaper",
		"Spinefall", "Stalker", "Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormrider", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The Decapitator", "The End", "The Facelifter", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Treachery", "Tremor", "Trickster", "Trickster's Edge",
		"Trinity", "Twisted", "Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked",
		"Widow Maker", "Willbreaker", "Windreaper", "Windslicer", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian",
		"Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin",
		"Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defender", "Defender's", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Ferocious",
		"Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Foul", "Fragile", "Frail",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting",
		"Greedy", "Grieving", "Grim", "Guard's", "Guardian's", "Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hero", "Hero's",
		"Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved", "Impure", "Incarnated", "Infused", "Inherited", "Isolated",
		"Jade Infused", "Judgement", "Keeper's", "Knightly", "Knight's", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful", "Lusting",
		"Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primal", "Prime",
		"Primitive", "Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Recruit's", "Refined", "Reforged", "Reincarnated", "Relentless", "Remorseful",
		"Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Runed", "Rusty", "Savage", "Sentinel",
		"Shadow", "Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall", "Smooth", "Soldier's", "Solitude's", "Sorcerer's", "Sorrow's",
		"Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral", "Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury",
		"Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn",
		"Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful",
		"Venom", "Vicious", "Victor", "Vile", "Vindication", "Vindicator", "Vindictive", "Void", "Volcanic", "Vowed", "War", "War-Forged", "Warden's", "Warlord's",
		"Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning",
		"Zealous" };
	static const char* nm3[] = { "Adamantite", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Glass", "Golden", "Iron", "Ironbark", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Titanium" };
	static const char* nm4[] = { "Battle Axe", "Chopper", "Axe", "Edge", "Crescent", "Greataxe", "Dualblade", "Broadaxe", "Reaver", "War Axe", "Ravager",
		"Pollaxe", "hatchet", "slasher" };
	static const char* nm5[] = { "Allegiance", "Annihilation", "Axe", "Battle Axe", "Betrayer", "Bond", "Boon", "Breaker", "Bringer", "Broadaxe", "Butcher",
		"Call", "Carver", "Champion", "Chopper", "Cleaver", "Conqueror", "Crescent", "Crusader", "Cry", "Cunning", "Dark Blade", "Dawn", "Defender", "Defiler",
		"Destroyer", "Doomblade", "Dualblade", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greataxe", "Guardian",
		"Heirloom", "Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand", "Legacy", "Maul", "Memory", "Might", "Oath", "Pact", "Pledge", "Promise",
		"Prophecy", "Protector", "Ravager", "Razor", "Reach", "Reaper", "Reaver", "Runed Blade", "Sculptor", "Secret", "Slayer", "Slicer", "Soul", "Terror",
		"Token", "Tribute", "Vengeance", "Voice", "War Axe", "Warblade", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun",
		"of the Sunwalker", "of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked",
		"of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Abomination", "Agatha", "Allegiance", "Alpha", "Amnesia", "Anduril", "Anger's Edge", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blazeguard", "Blind Justice", "Blinkstrike", "Bloodfury", "Bloodmoon",
		"Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Braindead", "Broken Promise", "Brutality", "Brutalizer", "Cataclysm",
		"Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Convergence", "Corpsemaker", "Corruption", "Crescent Moon",
		"Crimson", "Cyclone", "Darkness", "Dawn", "Dawn's Edge", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze", "Deathspade", "Decimation", "Desolation",
		"Despair", "Destiny", "Devastation", "Devotion", "Devourer", "Divine Light", "Dominance", "Dominion", "Doom", "Doomblade", "Doomchopper",
		"Due Diligence", "Earthquake", "Echo", "Eclipse", "Emergency", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Extermination",
		"Extinction", "Faithkeeper", "Fate", "Finality", "Fleshrender", "Freak's Bite", "Frenzy", "Frostbite", "Frostwind", "Fury", "Fury's Gaze", "Gatecrasher",
		"Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Guillotine", "Gutrender", "Harbinger", "Harvester", "Hatred", "Heartseeker", "Hell's Scream",
		"Hellfire", "Hellreaver", "Homage", "Honor's Call", "Hope's Edge", "Hope's End", "Humongous", "Hurricane", "Infamy", "Infinity", "Interrogator",
		"Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance",
		"Last Laugh", "Last Rites", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning",
		"Limbo", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight", "Misery's Edge", "Misery's End", "Monsoon", "Mooncleaver", "Mournblade",
		"Narcoleptic", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightfall", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion",
		"Old Age", "Omega", "Orbit", "Orenmir", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety",
		"Pork-Chopper", "Pride", "Prophecy", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse",
		"Requiem", "Retirement", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Severance", "Shadow Strike",
		"Shadow's Edge", "Shadowfury", "Shadowmoon", "Shadowsteel", "Silence", "Skullcleaver", "Skullcrusher", "Slice of Life", "Soul Breaker", "Soul Reaper",
		"Spinefall", "Stalker", "Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormrider", "Supernova", "Supremacy",
		"Suspension", "Swan Song", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The Decapitator", "The End", "The Facelifter", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Treachery", "Tremor", "Trickster", "Trinity",
		"Twisted", "Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked",
		"Widow Maker", "Willbreaker", "Windreaper", "Windslicer", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	
	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_weapons_bows()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Hornet's Sting", "Blackened Sky", "Darkened Sky", "Steel Hail", "Archangel", "Archdemon", "Arcus", "Armadillo", "Armageddon",
		"Arrowsong", "Arrowspike", "Avalance", "Back Pain", "Backsnipe", "Ballista", "Barrage", "Beesting", "Betrayal", "Betrayal's Sting", "Bolt", "Bolt Action",
		"Bon Voyage", "Bristleblitz", "Bullseye", "Calamity", "Chimera", "Clutch", "Comet", "Courier", "Crier", "Curvey", "Cyclone", "Dash", "Dead Air",
		"Death's Kiss", "Death's Sigh", "Death's Whisper", "Decimate", "Deliverance", "Deluge", "Destiny's Song", "Devil's Kiss", "Devil's Recurve",
		"Devil's Sting", "Devil's Whisper", "Doomcaster", "Drawback", "Drawling", "Drawstring", "Dream Catcher", "Eagle", "Eagle Strike", "Emily's Curve",
		"Euthanasia", "Eye of Eternity", "Eye of Fidelity", "Eye of Precision", "Eye of Truth", "Falling Star", "Featherdraw", "Final Breath", "Final Voyage",
		"Final Whisper", "Final Whistle", "Firestarter", "Fling", "Flux", "Fury", "Gargoyle", "Graviton", "Hailstorm", "Hamstring", "Hamstrung", "Harbinger",
		"Hatred's Sting", "Hawkeye", "Heartbeat", "Heartpiercer", "Heartstriker", "Heartstring", "Hedgehog", "Hell's Whistle", "High-Strung",
		"Hooty Tooty Aim and Shooty", "Hope's End", "Hornet", "Huntress", "Hurricane", "Hush", "Imminent Doom", "Impending Doom", "Irk", "Jugular",
		"Kiss of Death", "Lash", "Last Kiss", "Last Vision", "Last Whisper", "Lightning", "Lightning Strike", "Long Shot", "Messenger", "Meteor",
		"Meteor Strike", "Midge", "Mirage", "Misery's End", "Molten Fury", "Mosquito", "Needle Shooter", "Needle Threader", "Netherstrand", "Penetrator",
		"Perfidy", "Phantom", "Phantom Strike", "Phoenix", "Pierce", "Pinch", "Pique", "Pluck", "Porcupine", "Precise", "Precision", "Prickle", "Prophet",
		"Puncture", "Quickstrike", "Quillshooter", "Quintain", "Rain Maker", "Razorsong", "Razorwind", "Recurve", "Rigormortis", "Salvation", "Scorpio",
		"Scorpion", "Scorpion Sting", "Shadow Strike", "Shadow's Bane", "Shadow's Strike", "Shatter Storm", "Shooting Star", "Shriek", "Silent Messenger",
		"Silentsong", "Siren's Call", "Siren's Cry", "Siren's Song", "Sky Piercer", "Skyfire", "Snatch", "Snipe", "Snitch", "Soulstring", "Special Delivery",
		"Spitfire", "Splinter", "Splintermark", "Squawk", "Squirm", "Star's Fury", "Starshot", "Starstruck", "Sting", "Stormsong", "Striker's Mark", "Stryker",
		"Sudden Death", "Surprise", "Swiftwind", "Swoosh", "Talonstrike", "Tempest", "The Ambassador", "The Messenger", "Thunder", "Thunderstrike", "Tiebreaker",
		"Tranquility", "Trophy Chord", "Trophy Gatherer", "Trophy Mark", "Truestrike", "Tweak", "Twisted", "Twister", "Twitch", "Typhoon", "Valkyrie",
		"Vehement Chord", "Venomstrike", "Viper", "Vixen", "Vulture", "Warsong", "Wasp", "Wasp's Sting", "Whelm", "Whisper", "Whisperwind", "Windbreaker",
		"Windforce", "Windlass", "Windrunner", "Windstalker", "Windtalker", "WithDraw" };
	static const char* nm2[] = { "Advanced", "Amber Infused", "Ancient", "Anguish'", "Annihilation", "Antique", "Arcane", "Arched", "Archer's", "Assassination",
		"Atuned", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blessed", "Blood Infused", "Bloodcursed", "Bloodied",
		"Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Bowman's", "Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Charmed",
		"Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Defiled", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire",
		"Doom", "Doom's", "Dragon's", "Dragonbreath", "Dreadful", "Ebon", "Eerie", "Enchanted", "Engraved", "Eternal", "Ethereal", "Executing", "Exiled",
		"Expert's", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feather-Wrapped", "Featherdraw", "Feral", "Fierce", "Fiery", "Fine", "Firestorm",
		"Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghostly", "Gladiator", "Gladiator's",
		"Grieving", "Guard's", "Guardian's", "Hateful", "Haunted", "Heavy", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering",
		"Improved", "Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful",
		"Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Massive", "Master Hunter's", "Mended", "Mercenary", "Military", "Misfortune's", "Mourning",
		"Nightmare", "Nightstalker", "Ominous", "Peacekeeper", "Phantom", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Proud",
		"Ranger's", "Recruit's", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring",
		"Savage", "Oathkeeper's", "Shadow", "Shadowleaf", "Shrieking", "Silent", "Singed", "Singing", "Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's",
		"Sorrow's", "Soul", "Soul Infused", "Soulcursed", "Soulless", "Spectral", "Spiteful", "Storm", "Storm's", "Stormfury", "Stormguard", "Stout", "Striker's",
		"Sturdy", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunderfury", "Thunderguard", "Thundersoul", "Timeworn", "Tormented", "Tracking", "Trainee's",
		"Trapper's", "Treachery's", "Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious",
		"Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged", "Warlord's", "Warp", "Warped", "Warsong", "Well Crafted", "Whistling",
		"Wicked", "Wind's", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Yielding", "Zealous" };
	static const char* nm3[] = { "Ashwood", "Bone", "Bronzed", "Driftwood", "Ebon", "Hardwood", "Iron", "Ironbark", "Maple", "Oak", "Redwood", "Skeletal",
		"Steel", "Titanium", "Warpwood", "Willow", "Yew" };
	static const char* nm4[] = { "Branch", "Rain", "Reach", "String", "Launcher", "Repeater", "Shooter", "Crossfire", "Fletch", "Inception",
		"Harp", "Piercer", "Striker", "Shredder", "Chord", "Recurve", "Rift", "Wind", "Ripper", "Nurture", "Spitter", "Curser",
		"Negotiator", "Slayer", "Defiler", "Devastator" };
	static const char* nm5[] = { "Annihilation", "Arbalest", "Arch", "Betrayer", "Bite", "Bolter", "Bond", "Boon", "Bow", "Breaker", "Bringer", "Call", "Champion",
		"Chord", "Composite Bow", "Compound Bow", "Crescent", "Crook", "Crossbow", "Crossfire", "Cry", "Cunning", "Curve", "Dawn", "Defiler", "Destroyer",
		"Eclipse", "Ellipse", "Ender", "Etcher", "Executioner", "Eye", "Favor", "Ferocity", "Flatbow", "Foe", "Gift", "Glory", "Guardian", "Heavy Crossbow",
		"Heirloom", "Hope", "Hunting Bow", "Incarnation", "Kiss", "Last Hope", "Last Stand", "Launcher", "Legacy", "Longbow", "Memory", "Might", "Oath", "Pact",
		"Piercer", "Pique", "Pledge", "Poke", "Prick", "Promise", "Protector", "Ravager", "Reach", "Recurve", "Reflex Bow", "Repeater", "Savagery", "Secret",
		"Self Bow", "Shooter", "Shortbow", "Skewer", "Soul", "Speargun", "Spike", "Spine", "Straight Bow", "Striker", "String", "Terror", "Token", "Tribute",
		"Vengeance", "Voice", "Warbow", "Warp-Bow", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Archangel", "Archdemon", "Arcus", "Armadillo", "Armageddon", "Arrowsong", "Arrowspike", "Avalance", "Back Pain", "Backsnipe",
		"Ballista", "Barrage", "Beesting", "Betrayal", "Betrayal's Sting", "Bolt", "Bon Voyage", "Bristleblitz", "Bullseye", "Calamity", "Chimera", "Clutch",
		"Comet", "Courier", "Crier", "Curvey", "Cyclone", "Dash", "Dead Air", "Death's Kiss", "Death's Sigh", "Death's Whisper", "Decimate", "Deliverance",
		"Deluge", "Destiny's Song", "Devil's Kiss", "Devil's Recurve", "Devil's Sting", "Devil's Whisper", "Doomcaster", "Drawback", "Drawling", "Drawstring",
		"Dream Catcher", "Eagle", "Eagle Strike", "Emily's Curve", "Euthanasia", "Falling Star", "Featherdraw", "Firestarter", "Fling", "Flux", "Fury",
		"Gargoyle", "Graviton", "Hailstorm", "Hamstring", "Hamstrung", "Harbinger", "Hatred's Sting", "Hawkeye", "Heartbeat", "Heartpiercer", "Heartstriker",
		"Heartstring", "Hedgehog", "Hell's Whistle", "High-Strung", "Hope's End", "Hornet", "Huntress", "Hurricane", "Hush", "Irk", "Jugular", "Kiss of Death",
		"Lash", "Lightning", "Long Shot", "Messenger", "Meteor", "Midge", "Mirage", "Misery's End", "Molten Fury", "Mosquito", "Netherstrand", "Penetrator",
		"Perfidy", "Phantom", "Phantom Strike", "Phoenix", "Pierce", "Pinch", "Pique", "Pluck", "Porcupine", "Precise", "Precision", "Prickle", "Prophet",
		"Puncture", "Quickstrike", "Quintain", "Rain Maker", "Razorsong", "Razorwind", "Recurve", "Rigormortis", "Salvation", "Scorpio", "Scorpion",
		"Shadow Strike", "Shooting Star", "Shriek", "Silent Messenger", "Silentsong", "Siren's Call", "Siren's Cry", "Siren's Song", "Sky Piercer", "Skyfire",
		"Snatch", "Snipe", "Snitch", "Soulstring", "Spitfire", "Splinter", "Splintermark", "Squawk", "Squirm", "Star's Fury", "Starshot", "Starstruck", "Sting",
		"Stormsong", "Stryker", "Surprise", "Swiftwind", "Swoosh", "Talonstrike", "Tempest", "The Ambassador", "The Messenger", "Thunder", "Thunderstrike",
		"Tiebreaker", "Tranquility", "Truestrike", "Tweak", "Twisted", "Twister", "Twitch", "Typhoon", "Valkyrie", "Venomstrike", "Viper", "Vixen", "Vulture",
		"Warsong", "Wasp", "Whelm", "Whisper", "Whisperwind", "Windbreaker", "Windforce", "Windlass", "Windrunner", "Windstalker", "Windtalker", "WithDraw" };
	
	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm3));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm3[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_weapons_staves()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abracadaver", "Alakaslam", "Alpha", "Amnesia", "Amnesty", "Anarchy", "Apocalypse", "Apostle", "Armageddon", "Arrogance",
		"Ataraxia", "Attrition", "Balance", "Benediction", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blind Justice", "Branch of Corruption",
		"Branch of Illusions", "Branch of Truth", "Branch of Twilight", "Branch of Wisdom", "Brilliance", "Brilliancy", "Broken Promise", "Brutality",
		"Cataclysm", "Catastrophe", "Celeste", "Chancellor", "Chaos", "Clemency", "Cloudscorcher", "Cometfell", "Consecration", "Convergence", "Corrupted Will",
		"Corruption", "Crucifix", "Damnation", "Dawn", "Deathraze", "Decimation", "Dementia", "Desolation", "Despair", "Devotion", "Divinity", "Doomcaller",
		"Dragonwrath", "Draughtbane", "Dreambender", "Dreambinder", "Dreamcaller", "Dreamwatcher", "Due Diligence", "Dying Hope", "Dying Light", "Earthshaper",
		"Echo", "Eclipse", "Edge of Eternity", "Edge of Insanity", "Edge of Sanity", "Edge of Time", "Enigma", "Enlightenment", "Epilogue", "Erosion",
		"Eternal Will", "Extinction", "Eye of Corruption", "Eye of the Seeker", "Faded Memory", "Faith", "Faithkeeper", "Fate", "Feral Fury", "Firestorm",
		"Fireweaver", "Flamewarden", "Fluke", "Fortuity", "Frenzy", "Frostwarden", "Fury", "Gaze of Arrogance", "Gaze of Corruption", "Gaze of Truth",
		"Gaze of Vanity", "Ghostwalker", "Glimmer", "Gutrender", "Gutwrencher", "Harmony", "Hell's Scream", "Hubris", "Hysteria", "Illumination", "Illusion",
		"Inertia", "Infamy", "Infinity", "Ingenuity", "Insanity", "Insight", "Insolence", "Invocation", "Isolation", "Ivory", "Journey's End", "Judgement",
		"Justice", "Knightfall", "Labyrinth", "Lament", "Lazarus", "Lifebender", "Lifebinder", "Lifemender", "Lightbane", "Limbo", "Lorekeeper", "Maelstrom",
		"Malevolence", "Malice", "Massacre", "Meditation", "Mercy", "Midnight", "Moonlight", "Moonshadow", "Moonshine", "Narcoleptic", "Nemesis", "Nightbane",
		"Nightfall", "Nightfury", "Nightkiss", "Nightstaff", "Nimble", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Omen", "Pansophy",
		"Peacekeeper", "Penance", "Perdition", "Persuasion", "Phantom", "Phantomstrike", "Phobia", "Pilgrimage", "Prudence", "Purgatory", "Pursuit", "Quicksilver",
		"Rage", "Rapture", "Reaper", "Reckoning", "Redemption", "Reflection", "Remorse", "Repentance", "Requiem", "Retribution", "Riddle", "Sacrifice", "Sanguine",
		"Sapience", "Scar", "Scarlet", "Sealed Truth", "Serendipity", "Serenity", "Shadowfall", "Shadowsong", "Silence", "Silverlight", "Sleepwalker", "Solarflare",
		"Soulkeeper", "Spellbinder", "Spire", "Spire of Twilight", "Spiritbinder", "Spiritcaller", "Stalk of Corruption", "Stalk of Illusions", "Stalk of Truth",
		"Stalk of Wisdom", "Starlight", "Starshine", "Stoneshaper", "Storm", "Stormbringer", "Stormcaller", "Stormtip", "Sufferance", "Sun Strike", "Sunflare",
		"Sunshard", "Sunshine", "Supinity", "Suspension", "Swan Song", "The End", "The Prophecy", "The Spire", "The Stake", "The Taker", "The Undertaker",
		"The Unmaker", "The Verdict", "The Void", "Thorn", "Thunder", "Thunderstorm", "Torment", "Tranquility", "Treachery", "Trickster", "Trinity", "Twilight",
		"Twinkle", "Twisted Visage", "Twisted Visions", "Twister", "Twitch", "Vainglorious", "Valkyrie", "Vanity", "Verdict", "Visage", "Visage of Arrogance",
		"Visage of Truth", "Will of Truth", "Will of the Master", "Windcaller", "Windfall", "Windwalker", "Windweaver", "Worldshaper" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Baneful", "Banished", "Barbarian", "Barbaric",
		"Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin", "Burnished",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Ebon", "Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral",
		"Ferocious", "Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Forsaken", "Fortune's", "Foul", "Frenzied", "Frost",
		"Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gleaming", "Glinting", "Greedy", "Grieving", "Grim", "Guardian's",
		"Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hollow", "Holy", "Honed", "Hope's", "Hopeless", "Howling", "Hungering",
		"Improved", "Impure", "Incarnated", "Infused", "Inherited", "Jade Infused", "Judgement", "Liar's", "Lich", "Lightning", "Lonely", "Loyal", "Lustful",
		"Lusting", "Mage's", "Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Necromancer's", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive",
		"Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Refined", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated",
		"Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused", "Rune-Forged", "Runed", "Rusty", "Sage's", "Savage", "Sear's", "Shadow",
		"Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall", "Smooth", "Solitude's", "Sorcerer's", "Sorrow's", "Soul", "Soul Infused",
		"Soul-Forged", "Soulcursed", "Soulless", "Spectral", "Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror",
		"Thirsting", "Thirsty", "Thunder", "Thunder-Forged", "Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Treachery's",
		"Twilight", "Twilight's", "Twisted", "Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vile", "Vindication", "Vindictive",
		"Void", "Volcanic", "Vowed", "War", "War-Forged", "Warlock's", "Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong",
		"Wizard's", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Ashwood", "Bone", "Briarwood", "Bronzed", "Cedarwood", "Cottonwood", "Devilwood", "Driftwood", "Ebon", "Emberwood", "Hardwood",
		"Iron", "Ironbark", "Maple", "Oak", "Oakwood", "Redwood", "Sagewood", "Sandalwood", "Skeletal", "Steel", "Summerwood", "Titanium", "Warpwood", "Willow",
		"Yew" };
	static const char* nm4[] = { "Staff", "Branch", "Cane", "Pole", "Scepter", "Warden Staff", "Grand Staff", "Greatstaff", "War Staff", "Spire", "Spiritstaff",
		"Energy Staff" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Bond", "Boon", "Branch", "Breaker", "Bringer", "Call", "Cane", "Champion", "Conqueror", "Crusader",
		"Cry", "Cunning", "Dawn", "Defender", "Defiler", "Deflector", "Destroyer", "Doom", "Edge", "Ender", "Energy Staff", "Executioner", "Favor", "Ferocity",
		"Foe", "Gift", "Glory", "Grand Staff", "Greatstaff", "Guardian", "Heirloom", "Hope", "Incarnation", "Last Hope", "Last Stand", "Legacy", "Memory",
		"Might", "Oath", "Pact", "Pledge", "Pole", "Promise", "Protector", "Reach", "Reaper", "Savagery", "Secret", "Slayer", "Soul", "Spire", "Spiritstaff",
		"Staff", "Terror", "Token", "Tribute", "Vengeance", "Voice", "War Staff", "Warden Staff", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of Time", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Abracadaver", "Alakaslam", "Alpha", "Amnesia", "Amnesty", "Anarchy", "Apocalypse", "Apostle", "Armageddon", "Arrogance",
		"Ataraxia", "Attrition", "Balance", "Benediction", "Betrayal", "Betrayer", "Blackout", "Blazefury", "Blind Justice", "Brilliance", "Brilliancy",
		"Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chancellor", "Chaos", "Clemency", "Cloudscorcher", "Cometfell", "Consecration",
		"Convergence", "Corrupted Will", "Corruption", "Crucifix", "Damnation", "Dawn", "Deathraze", "Decimation", "Dementia", "Desolation", "Despair",
		"Devotion", "Divinity", "Doomcaller", "Dragonwrath", "Draughtbane", "Dreambender", "Dreambinder", "Dreamcaller", "Dreamwatcher", "Due Diligence",
		"Dying Hope", "Dying Light", "Earthshaper", "Echo", "Eclipse", "Edge of Eternity", "Edge of Insanity", "Edge of Sanity", "Edge of Time", "Enigma",
		"Enlightenment", "Epilogue", "Erosion", "Eternal Will", "Extinction", "Eye of Corruption", "Eye of the Seeker", "Faded Memory", "Faith", "Faithkeeper",
		"Fate", "Feral Fury", "Firestorm", "Fireweaver", "Flamewarden", "Fluke", "Fortuity", "Frenzy", "Frostwarden", "Fury", "Gaze of Arrogance",
		"Gaze of Corruption", "Gaze of Truth", "Gaze of Vanity", "Ghostwalker", "Glimmer", "Gutrender", "Gutwrencher", "Harmony", "Hell's Scream", "Hubris",
		"Hysteria", "Illumination", "Illusion", "Inertia", "Infamy", "Infinity", "Ingenuity", "Insanity", "Insight", "Insolence", "Invocation",
		"Isolation", "Ivory", "Journey's End", "Judgement", "Justice", "Knightfall", "Labyrinth", "Lament", "Lazarus", "Lifebender", "Lifebinder", "Lifemender",
		"Lightbane", "Limbo", "Lorekeeper", "Maelstrom", "Malevolence", "Malice", "Massacre", "Meditation", "Mercy", "Midnight", "Moonlight", "Moonshadow",
		"Moonshine", "Narcoleptic", "Nemesis", "Nightbane", "Nightfall", "Nightfury", "Nightkiss", "Nightstaff", "Nimble", "Nirvana", "Oathbreaker", "Oathkeeper",
		"Oblivion", "Omega", "Omen", "Pansophy", "Peacekeeper", "Penance", "Perdition", "Persuasion", "Phantom", "Phantomstrike", "Phobia", "Pilgrimage",
		"Prudence", "Purgatory", "Pursuit", "Quicksilver", "Rage", "Rapture", "Reaper", "Reckoning", "Redemption", "Reflection", "Remorse", "Repentance",
		"Requiem", "Retribution", "Riddle", "Sacrifice", "Sanguine", "Sapience", "Scar", "Scarlet", "Sealed Truth", "Serendipity", "Serenity", "Shadowfall",
		"Shadowsong", "Silence", "Silverlight", "Sleepwalker", "Solarflare", "Soulkeeper", "Spellbinder", "Spire", "Spire of Twilight", "Spiritbinder",
		"Spiritcaller", "Stalk of Corruption", "Stalk of Illusions", "Stalk of Truth", "Stalk of Wisdom", "Starlight", "Starshine", "Stoneshaper", "Storm",
		"Stormbringer", "Stormcaller", "Stormtip", "Sufferance", "Sun Strike", "Sunflare", "Sunshard", "Sunshine", "Supinity", "Suspension", "Swan Song",
		"The End", "The Prophecy", "The Stake", "The Taker", "The Undertaker", "The Unmaker", "The Verdict", "The Void", "Thorn", "Thunder", "Thunderstorm",
		"Torment", "Tranquility", "Treachery", "Trickster", "Trinity", "Twilight", "Twinkle", "Twisted Visage", "Twisted Visions", "Twister", "Twitch",
		"Vainglorious", "Valkyrie", "Vanity", "Verdict", "Visage", "Visage of Arrogance", "Visage of Truth", "Will of Truth", "Will of the Master", "Windcaller",
		"Windfall", "Windwalker", "Windweaver", "Worldshaper" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_weapons_swords()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Shadowfang", "Azurewrath", "Assurance", "Red Obsidian", "Abyssal Shard", "Aetherius", "Agatha", "Alpha",
		"Amnesia", "Anduril", "Anger's Tear", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer", "Blackest Heart", "Blackout",
		"Blade of a Thousand Cuts", "Blade of the Grave", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodquench",
		"Bloodweep", "Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chaos", "Cometfell", "Convergence", "Corruption", "Darkheart",
		"Darkness", "Dawn", "Dawn of Ruins", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Despair", "Destiny's Song",
		"Devourer", "Dirge", "Divine Light", "Doomblade", "Doomcaller", "Draughtbane", "Due Diligence", "Early Retirement", "Echo", "Piece Maker", "Eclipse",
		"Endbringer", "Epilogue", "Espada", "Eternal Harmony", "Eternal Rest", "Extinction", "Faithkeeper", "Fallen Champion", "Fate", "Final Achievement",
		"Fleshrender", "Florance", "Frenzy", "Fury", "Ghost Reaver", "Ghostwalker", "Gladius", "Glimmer", "Godslayer", "Grasscutter", "Grieving Blade",
		"Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker", "Hell's Scream", "Hellfire", "Hellreaver", "Hollow Silence", "Honor's Call", "Hope's End",
		"Infamy", "Interrogator", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Klinge", "Knight's Fall", "Knightfall", "Lament",
		"Last Rites", "Last Words", "Lazarus", "Life's Limit", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty",
		"Malice", "Mangler", "Massacre", "Mercy", "Misery's End", "Morbid Doom", "Morbid Will", "Mournblade", "Narcoleptic", "Needle", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle", "Nightfall", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir",
		"Peacekeeper", "Perfect Storm", "Persuasion", "Purifier", "Rage", "Ragespike", "Ragnarok", "Reaper", "Reaper's Toll", "Reckoning",
		"Reign of Misery", "Remorse", "Requiem", "Requiem of the Lost", "Retirement", "Righteous Might", "Rigormortis", "Savagery", "Scalpel", "Scar",
		"Seethe", "Severance", "Shadow Strike", "Shadowsteel", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life",
		"Soul Reaper", "Soulblade", "Soulrapier", "Spada", "Spike", "Spineripper", "Spiteblade", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm",
		"Storm Breaker", "Stormbringer", "Stormcaller", "Storm-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Blade",
		"The End", "The Facelifter", "The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Toothpick",
		"Tranquility", "Treachery", "Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank",
		"Warmonger", "Widow Maker", "Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };
	static const char* nm2[] = { "Massive", "Military", "Amber Infused", "Ancient", "Anguish", "Annihilation", "Antique", "Arcane", "Arched", "Assassination",
		"Atuned", "Oathkeeper's", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged",
		"Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Bonecarvin", "Brutal", "Brutality", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Decapitating", "Defiled",
		"Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted",
		"Engraved", "Eternal", "Executing", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused",
		"Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious",
		"Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting", "Greedy", "Grieving", "Guard's", "Guardian's",
		"Hailstorm", "Hateful", "Haunted", "Heartless", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved",
		"Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely",
		"Loyal", "Lustful", "Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Mended", "Mercenary", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Ominous", "Peacekeeper", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Deluded",
		"Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual",
		"Roaring", "Ruby Infused", "Rune-Forged", "Rusty", "Sailor's", "Sapphire Infused", "Savage", "Shadow", "Sharpened", "Silent", "Singed", "Singing",
		"Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged",
		"Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bronze", "Copper", "Diamond", "Glass", "Gold", "Iron", "Mithril", "Obsidian", "Silver", "Skeletal", "Steel" };
	static const char* nm4[] = { "Blade", "Broadsword", "Edge", "Defender", "Deflector", "Doomblade", "Spike", "Guardian", "Slasher", "Steel",
		"Mageblade", "Protector", "Fury", "Needle", "Reaver", "Malice", "Sting", "Pride", "Slicer", "Spellblade", "Sacrifice", "Sword", "Warblade",
		"Skewer", "Carver", "Etcher", "Sculptor", "Razor", "Cutter" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Blade", "Blessed Blade", "Blood Blade", "Bond", "Boon", "Breaker", "Bringer", "Broadsword",
		"Butcher", "Call", "Carver", "Champion", "Claymore", "Conqueror", "Crusader", "Cry", "Cunning", "Dark Blade", "Dawn", "Defender", "Defiler", "Deflector",
		"Destroyer", "Doomblade", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatsword", "Guardian", "Heirloom",
		"Hope", "Incarnation", "Jaws", "Katana", "Last Hope", "Last Stand", "Legacy", "Longblade", "Longsword", "Mageblade", "Memory", "Might", "Oath", "Pact",
		"Pledge", "Promise", "Protector", "Quickblade", "Rapier", "Ravager", "Razor", "Reach", "Reaper", "Reaver", "Runed Blade", "Saber", "Sabre", "Savagery",
		"Scimitar", "Sculptor", "Secret", "Shortsword", "Skewer", "Slayer", "Slicer", "Soul", "Spellblade", "Spine", "Swiftblade", "Sword", "Terror", "Token",
		"Tribute", "Vengeance", "Voice", "Warblade", "Warglaive", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Aetherius", "Agatha", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer",
		"Blackout", "Blazefury", "Blazeguard", "Blinkstrike", "Bloodquench", "Bloodweep", "Brutality", "Celeste", "Chaos", "Cometfell", "Convergence", "Darkheart",
		"Dawn", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Destiny's Song", "Dirge", "Doomblade", "Doomcaller", "Draughtbane",
		"Due Diligence", "Echo", "Eclipse", "Endbringer", "Epilogue", "Espada", "Extinction", "Faithkeeper", "Fate", "Fleshrender", "Florance", "Frenzy",
		"Fury", "Ghost Reaver", "Ghostwalker", "Gladius", "Glimmer", "Godslayer", "Grasscutter", "Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker",
		"Hell's Scream", "Hellfire", "Piece Maker", "Hellreaver", "Honor's Call", "Hope's End", "Infamy", "Interrogator", "Justifier", "Kinslayer", "Klinge",
		"Knightfall", "Lament", "Lazarus", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty", "Malice", "Mangler",
		"Massacre", "Mercy", "Misery", "Mournblade", "Narcoleptic", "Needle", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle",
		"Nightfall", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir", "Peacekeeper", "Persuasion", "Prick", "Purifier", "Rage",
		"Ragespike", "Ragnarok", "Reckoning", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Savagery", "Scalpel", "Scar", "Seethe", "Severance",
		"Shadow Strike", "Shadowsteel", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life", "Soul Reaper", "Soulblade",
		"Soulrapier", "Spada", "Spike", "Spineripper", "Spiteblade", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm", "Storm Breaker", "Stormbringer",
		"Stormcaller", "Story-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Blade", "The End", "The Facelifter",
		"The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Toothpick", "Tranquility", "Treachery",
		"Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank", "Warmonger", "Widow Maker",
		"Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_weapons_war_hammers()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Abomination", "Agatha", "Agony", "Allegiance", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ash",
		"Ashes", "Ashrune", "Back Breaker", "Battlestar", "Betrayal", "Betrayer", "Blackout", "Blazeguard", "Blessing", "Blind Justice", "Bloodfury", "Bloodmoon",
		"Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Bonesnapper", "Braindead", "Broken Promise", "Brutality", "Brutalizer",
		"Cataclysm", "Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Concussion", "Convergence", "Corpsemaker",
		"Corruption", "Cosmos", "Crash", "Crimson", "Crush", "Cryptmaker", "Cyclone", "Darkness", "Dawn", "Dawnbreaker", "Daytime", "Deathbringer", "Deathraze",
		"Decimation", "Demise", "Desolation", "Despair", "Destiny", "Destruction", "Devastation", "Devotion", "Devourer", "Disturbance", "Divine Light",
		"Dominance", "Dominion", "Doom", "Doomhowler", "Dragonfist", "Dragonmaw", "Dragonstrike", "Due Diligence", "Early Retirement", "Earthquake", "Earthshaker",
		"Earthshaper", "Earthwarden", "Echo", "Eclipse", "Emergency", "End of Dreams", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity", "Eveningstar", 
		"Extermination", "Extinction", "Faithkeeper", "Falcon", "Falling Star", "Fate", "Finality", "Frenzy", "Frostwind", "Fury's Gaze", "Galaxy", "Gatecrasher", 
		"Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grace", "Guiding Star", "Harbinger", "Harmony", "Harvester", "Hatred", "Heartcrusher", "Hell's Scream", 
		"Hellfire", "Homage", "Honor's Call", "Hope's End", "Humongous", "Hurricane", "Icebreaker", "Infamy", "Infinity", "Interrogator", "Jackhammer", "Jawbone", 
		"Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor", "Knightfall", "Lament", "Last Chance", "Last Laugh", 
		"Last Rites", "Last Word", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", 
		"Lockjaw", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight", "Misery's End", "Monsoon", "Morningstar", "Narcoleptic", "Nethersbane", 
		"Night's Fall", "Nightbane", "Nightfall", "Nightglow", "Nightmare", "Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", 
		"Orbit", "Orenmir", "Oxheart", "Patience", "Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piece Maker", "Piety", "Pride", 
		"Prophecy", "Punisher", "Purifier", "Rage", "Ragnarok", "Rapture", "Reaper's Toll", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement", 
		"Righteous Might", "Rigormortis", "Rising Tide", "Savagery", "Scar", "Scourgeborne", "Second Chance", "Seism", "Seismic", "Serenity", "Severance", 
		"Shadowfury", "Shadowmoon", "Shatterskull", "Shooting Star", "Silence", "Skullcrusher", "Soul Breaker", "Spinefall", "Stalker", "Stonefist", "Storm", 
		"Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormherald", "Stormrider", "Sucker Punch", "Sundown", "Supernova", "Supremacy", 
		"Suspension", "Swan Song", "Tank", "Tenderiser", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The End", "The Light", "The Minotaur", 
		"The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Trauma", "Treachery", "Tremor", "Trinity", "Twilight", "Typhoon", 
		"Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked", "Widow Maker", "Willbreaker", 
		"Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };
	static const char* nm2[] = { "Ancient", "Antique", "Apocalypse", "Apocalyptic", "Arcane", "Arched", "Atuned", "Bandit's", "Baneful", "Banished", "Barbarian",
		"Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged", "Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bonecarvin",
		"Brutal", "Brutality", "Burnished", "Cataclysm", "Cataclysmic", "Challenger", "Challenger's", "Champion", "Champion's", "Cold-Forged", "Conqueror",
		"Conqueror's", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Dark", "Darkness", "Defender", "Defender's", "Defiled",
		"Defiling", "Deluded", "Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Diabolical", "Dire", "Doom", "Doom's", "Dragon's", "Dragonbreath",
		"Eerie", "Enchanted", "Engraved", "Enlightened", "Eternal", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Ferocious",
		"Fierce", "Fiery", "Fire Infused", "Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Foul", "Fragile", "Frail",
		"Frenzied", "Frost", "Frozen", "Furious", "Fusion", "Ghastly", "Ghost", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting",
		"Greedy", "Grieving", "Grim", "Guard's", "Guardian's", "Hailstorm", "Harmonized", "Hateful", "Haunted", "Heartless", "Heinous", "Hero", "Hero's",
		"Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved", "Impure", "Incarnated", "Infused", "Inherited",
		"Isolated", "Jade Infused", "Judgement", "Keeper's", "Knightly", "Knight's", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely", "Loyal",
		"Lustful", "Lusting", "Malevolent", "Malicious", "Malificent", "Malignant", "Massive", "Mended", "Mercenary", "Military", "Misfortune's", "Misty",
		"Moonlit", "Mourning", "Nightmare", "Oathkeeper's", "Ominous", "Peacekeeper", "Peacekeeper's", "Phantom", "Polished", "Possessed", "Pride's",
		"Prideful", "Primal", "Prime", "Primitive", "Promised", "Protector's", "Proud", "Pure", "Putrid", "Raging", "Recruit's", "Refined", "Reforged",
		"Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual", "Roaring", "Ruby Infused",
		"Rune-Forged", "Runed", "Rusty", "Savage", "Sentinel", "Shadow", "Shamanic", "Sharpened", "Silent", "Singed", "Singing", "Sinister", "Skyfall",
		"Smooth", "Soldier's", "Solitude's", "Sorcerer's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vanquisher", "Vengeance", "Vengeful", "Venom", "Vicious", "Victor", "Vile", "Vindication", "Vindicator", "Vindictive",
		"Void", "Volcanic", "Vowed", "War", "War-Forged", "Warden's", "Warlord's", "Warp", "Warped", "Warrior", "Warrior's", "Whistling", "Wicked", "Wind's",
		"Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bone", "Bronze", "Bronzed", "Ivory", "Ebon", "Glass", "Golden", "Iron", "Ironbark", "Mithril", "Obsidian", "Silver",
		"Skeletal", "Steel", "Titanium" };
	static const char* nm4[] = { "Mace", "Bludgeon", "Maul", "Hammer", "Mallet", "Crusher", "Battlehammer", "Greathammer", "Smasher", "Warmace", "Hammer", "Mace",
		"Scepter", "Pummeler", "Breaker", "Destroyer", "Rhythm" };
	static const char* nm5[] = { "Allegiance", "Annihilation", "Battlehammer", "Betrayer", "Bludgeon", "Bond", "Boon", "Breaker", "Bringer", "Bruiser", "Butcher",
		"Call", "Champion", "Conqueror", "Crusader", "Crusher", "Cry", "Cunning", "Dawn", "Defender", "Defiler", "Destroyer", "Disposer", "Ender", "Executioner",
		"Favor", "Ferocity", "Foe", "Gift", "Glory", "Greathammer", "Guardian", "Hammer", "Heirloom", "Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand",
		"Legacy", "Mace", "Mallet", "Masher", "Maul", "Memory", "Might", "Oath", "Pact", "Pledge", "Pounder", "Promise", "Prophecy", "Protector", "Pummeler",
		"Queller", "Ravager", "Reach", "Reaper", "Scepter", "Sculptor", "Secret", "Slayer", "Smasher", "Soul", "Suppressor", "Terror", "Token", "Tribute",
		"Vengeance", "Voice", "Warmace", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Holy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope", "of Lost Hope",
		"of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy", "of Nightmares",
		"of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning", "of Redemption", "of Regret",
		"of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds", "of Silence", "of Slaughter",
		"of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror", "of Thunder",
		"of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth", "of Twilight's End",
		"of Twisted Visions", "of Unholy Blight", "of Unholy Might", "of Vengeance", "of Visions", "of Wasted Time", "of Widows", "of Wizardry", "of Woe",
		"of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed", "of the Breaking Storm",
		"of the Brotherhood", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw", "of the Corrupted", "of the Covenant",
		"of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord", "of the Earth", "of the East", "of the Emperor",
		"of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky", "of the Flame", "of the Forest", "of the Forgotten",
		"of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm", "of the Insane", "of the King", "of the Lasting Night",
		"of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor", "of the Lone Wolf", "of the Lost", "of the Moon",
		"of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North", "of the Occult", "of the Oracle", "of the Phoenix",
		"of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent", "of the Setting Sun", "of the Shadows", "of the Sky",
		"of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker", "of the Talon", "of the Undying", "of the Victor",
		"of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf", "of the World", "of the Wretched" };
	static const char* nm7[] = { "Abomination", "Agatha", "Agony", "Allegiance", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite",
		"Ash", "Ashes", "Ashrune", "Back Breaker", "Battlestar", "Betrayal", "Betrayer", "Blackout", "Blazeguard", "Blessing", "Blind Justice", "Bloodfury",
		"Bloodmoon", "Bloodquench", "Bloodrage", "Bloodspiller", "Bloodweep", "Bone Warden", "Bonesnapper", "Braindead", "Broken Promise", "Brutality",
		"Brutalizer", "Cataclysm", "Catastrophe", "Celeste", "Chance", "Chaos", "Chasm", "Chieftain", "Comet", "Cometfall", "Concussion", "Convergence",
		"Corpsemaker", "Corruption", "Cosmos", "Crash", "Crimson", "Crush", "Cryptmaker", "Cyclone", "Darkness", "Dawn", "Dawnbreaker", "Daytime", "Deathbringer",
		"Deathraze", "Decimation", "Demise", "Desolation", "Despair", "Destiny", "Destruction", "Devastation", "Devotion", "Devourer", "Disturbance",
		"Divine Light", "Dominance", "Dominion", "Doom", "Doomhowler", "Dragonfist", "Dragonmaw", "Dragonstrike", "Due Diligence", "Earthquake", "Earthshaker",
		"Earthshaper", "Earthwarden", "Echo", "Eclipse", "Emergency", "End of Dreams", "Endbringer", "Enigma", "Epilogue", "Eternal Rest", "Eternity",
		"Eveningstar", "Extermination", "Extinction", "Faithkeeper", "Falcon", "Falling Star", "Fate", "Finality", "Frenzy", "Frostwind", "Fury", "Fury's Gaze",
		"Galaxy", "Gatecrasher", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grace", "Guiding Star", "Harbinger", "Harmony", "Harvester", "Hatred",
		"Heartcrusher", "Hell's Scream", "Hellfire", "Homage", "Honor's Call", "Hope's End", "Humongous", "Hurricane", "Icebreaker", "Infamy", "Infinity",
		"Interrogator", "Jackhammer", "Jawbone", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Knight's Fall", "Knight's Honor",
		"Knightfall", "Lament", "Last Chance", "Last Laugh", "Last Rites", "Last Word", "Last Words", "Lazarus", "Legacy", "Legionaire", "Life's Limit",
		"Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Lockjaw", "Magma", "Malice", "Maneater", "Massacre", "Mercy", "Midnight",
		"Misery's End", "Monsoon", "Morningstar", "Narcoleptic", "Nethersbane", "Night's Fall", "Nightbane", "Nightfall", "Nightglow", "Nightmare",
		"Nighttime", "Nirvana", "Oathbreaker", "Oathkeeper", "Oblivion", "Old Age", "Omega", "Orbit", "Orenmir", "Oxheart", "Patience",
		"Peacekeeper", "Peacemaker", "Pendulum", "Perfect Storm", "Persuasion", "Piety", "Pride", "Prophecy", "Punisher", "Purifier", "Rage", "Ragnarok",
		"Rapture", "Reckoning", "Red Dwarf", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Rising Tide", "Savagery", "Seism", "Seismic",
		"Serenity", "Severance", "Shadowfury", "Shatterskull", "Shooting Star", "Silence", "Skullcrusher", "Soul Breaker", "Spinefall", "Stalker", "Stonefist",
		"Storm", "Storm Breaker", "Storm-Weaver", "Stormbringer", "Stormcaller", "Stormedge", "Stormherald", "Stormrider", "Sucker Punch", "Sundown", "Supernova",
		"Supremacy", "Suspension", "Swan Song", "Tank", "Tenderiser", "Termination", "The Ambassador", "The Chancellor", "The Chief", "The End", "The Light",
		"The Minotaur", "The Oculus", "The Oracle", "The Sundering", "The Void", "The Warden", "Thunder", "Trauma", "Treachery", "Tremor", "Trinity", "Twilight",
		"Typhoon", "Ubiquity", "Undoing", "Vacancy", "Valkyrie", "Vanquisher", "Vengeance", "Warbringer", "Warmonger", "Whirlwind", "Wicked", "Widow Maker",
		"Willbreaker", "Winter's Bite", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldcarver", "Worldslayer" };


	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* generator_miscellaneous_jewelrys( int type )
{
	static char names[ 128 ]; *names = 0;
	static const char* nm3[] = { "Ornament", "Pendant", "Pin", "Brooch", "Sigil", "Gift", "Burden", "Fetters", "Bond",
		"Star", "Talisman", "Ward", "Charm", "Shackles", "Gyve", "Stone", "Curse", "Sign", "Torment", "Mark", "Manacles",
		"Seal"};
	static const char* nm1[] = { "Adored", "Agate", "Aged", "Amber", "Amethyst", "Ancient", "Angel", "Angelic", "Anonymous", "Antique", "Arctic", "Austere",
		"Azure", "Blind", "Blushing", "Brave", "Bright", "Brilliant", "Broken", "Citrine", "Colossal", "Coral", "Crisp", "Crystal", "Curly", "Curvy", "Darling",
		"Dearest", "Defiant", "Devoted", "Diamond", "Diligent", "Earnest", "Elated", "Elegant", "Emerald", "Enchanted", "Enchanting", "Enlightened", "Exalted",
		"Exotic", "Faint", "Fair", "Feline", "Flawless", "Forsaken", "Free", "Frozen", "Garnet", "Gentle", "Gifted", "Glistening", "Graceful", "Gracious", "Grand",
		"Grateful", "Handsome", "Happy", "Harmonious", "Heaven", "Heavenly", "Hematite", "Honest", "Humble", "Idle", "Illustrious", "Impossible", "Infinite",
		"Innocent", "Jade", "Jasper", "Lavish", "Lonely", "Loyal", "Luminous", "Lunar", "Lustrous", "Majestic", "Malachite", "Mellow", "Mysterious", "Obsidian",
		"Ocean", "Onyx", "Parallel", "Peace", "Peaceful", "Pearl", "Perfect", "Pink", "Playful", "Precious", "Pristine", "Proud", "Pure", "Purity", "Quiet",
		"Royal", "Ruby", "Sapphire", "Scented", "Secret", "Serene", "Serpent", "Serpentine", "Shadow", "Silent", "Sinful", "Solar", "Spinel", "Spotless",
		"Stunning", "Sweet", "Tempting", "Tender", "Tinted", "Unmounted", "Velvet", "Vibrant", "Violet", "Virtuous", "Worthy", "Zircon" };
	static const char* nm2[] = { "Ambition", "Aura", "Balance", "Bauble", "Beauty", "Belle", "Blessing", "Bliss", "Blossom", "Bond", "Breath", "Bubble", "Charm",
		"Class", "Clover", "Core", "Crescent", "Crest", "Cross", "Crux", "Desire", "Devotion", "Dewdrop", "Dream", "Drop", "Droplet", "Eye", "Fan", "Favor",
		"Flame", "Flower", "Focus", "Force", "Gift", "Glamour", "Globe", "Grace", "Heart", "Hope", "Hum", "Hymn", "Image", "Leaf", "Life", "Light", "Lily",
		"Love", "Lure", "Mark", "Memorial", "Mind", "Moon", "Oath", "Oculus", "Orb", "Palm", "Panther", "Passion", "Petal", "Pledge", "Poem", "Prayer",
		"Promise", "Prospect", "Rainbow", "Resolve", "Riddle", "Rock", "Root", "Scale", "Seal", "Shield", "Song", "Soul", "Spark", "Spiral", "Spirit", "Star",
		"Stone", "Sun", "Swan", "Tear", "Teardrop", "Tempest", "Tribute", "Trinket", "Twin", "Twins", "Twirl", "Twist", "Valor", "Vigor", "Vision", "Voice",
		"Vow", "Whisper", "Will", "Wing", "Wings", "Wish" };




	int i = RNG(10);
	{
		int rnd = RNG(countof(nm1));
		int rnd2 = RNG(countof(nm2));
		if( i < 5 ){
			sprintf_s( names, "The %s %s", nm1[ rnd ], nm2[ rnd2 ] );
		} else{
			int rnd3 = RNG(countof(nm3));
			sprintf_s( names, "The %s %s %s", nm1[ rnd ], nm2[ rnd2 ], nm3[ rnd3 ] );
		}
		return names;
	}
}


//----- (th2) -------------------------------------------------------------
char* generator_weapons_flask()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Assurance", "Red Obsidian", "Abyssal Shard", "Aetherius", "Agatha", "Alpha",
		"Anduril", "Anger's Tear", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer", "Black Goblet", "Blackout",
		"Chalice of a Thousand Bites", "Flasks of the Grave", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodquench",
		"Bloodweep", "Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chaos", "Cometfell", "Convergence", "Corruption", "Darkheart",
		"Darkness", "Dawn", "Dawn of Ruins", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Delta", "Desolation", "Despair", "Destiny's Song",
		"Devourer", "Dirge", "Divine Light", "Doomflasks", "Draughtbane", "Due Diligence", "Early Retirement", "Echo", "Piece Maker", "Eclipse",
		"Endbringer", "Epilogue", "Espada", "Eternal Harmony", "Eternal Rest", "Extinction", "Faithkeeper", "Fallen Champion", "Fate", "Final Achievement",
		"Fleshrender", "Florance", "Frenzy", "Fury", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grieving Flasks",
		"Gutrender", "Hatred's Bite", "Heartdrinker", "Heartstriker", "Hell's Scream", "Hellfire", "Hellreaver", "Hollow Silence", "Honor's Call", "Hope's End",
		"Infamy", "Interrogator", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Klinge", "Knight's Fall", "Knightfall", "Lament",
		"Last Rites", "Last Words", "Life's Limit", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty",
		"Malice", "Mangler", "Massacre", "Mercy", "Misery's End", "Morbid Doom", "Morbid Will", "Mournflask", "Narcoleptic", "Needle", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle", "Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir",
		"Peacekeeper", "Perfect Storm", "Persuasion", "Purifier", "Rage", "Ragespike", "Reaper", "Reaper's Toll", "Reckoning",
		"Reign of Misery", "Remorse", "Requiem", "Requiem of the Lost", "Retirement", "Righteous Might", "Rigormortis", "Savagery", "Scalpel", "Scar",
		"Seethe", "Severance", "Shadow Strike", "Shadowdrinker", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life",
		"Soul Reaper", "Soulburn", "Spineripper", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm",
		"Storm Breaker", "Stormbringer", "Stormcaller", "Storm-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Eventide",
		"The End", "The Facelifter", "The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder",
		"Tranquility", "Treachery", "Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank",
		"Warmonger", "Widow Maker", "Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };
	static const char* nm2[] = { "Massive", "Military", "Amber Infused", "Ancient", "Anguish", "Annihilation", "Antique", "Arcane", "Arched", "Assassination",
		"Atuned", "Oathkeeper's", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged",
		"Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Brutal", "Brutality", "Burning", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Decapitating", "Defiled",
		"Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire", "Dissolution", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted",
		"Engraved", "Eternal", "Executing", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused",
		"Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious",
		"Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting", "Greedy", "Grieving", "Guard's", "Guardian's",
		"Hailstorm", "Hateful", "Haunted", "Heartless", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved",
		"Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely",
		"Loyal", "Lustful", "Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Mended", "Mercenary", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Ominous", "Peacekeeper", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Deluded",
		"Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual",
		"Roaring", "Ruby Infused", "Rune-Forged", "Rusty", "Sailor's", "Sapphire Infused", "Savage", "Shadow", "Silent", "Singed", "Singing",
		"Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged",
		"Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bronze", "Copper", "Diamond", "Glass", "Gold", "Iron", "Mithril", "Obsidian", "Silver", "Skeletal", "Steel" };
	static const char* nm4[] = { "Goblet", "Potion", "Chalice", "Flasks", "Burner", "Elixir", "Brew", "Concoction", "Tincture", "Bottle", "Canister",
		"Mixture", "Fluid", "Liquor", "Bitters", "Poison", "Medicine", "Amphora", "Vessel", "Can", "Jar", "Jug", "Distillate", "Nectar" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Blessed Potion", "Blood Potion", "Bond", "Boon", "Breaker", "Bringer", 
		"Butcher", "Call", "burner", "Champion",  "Conqueror", "Crusader", "Cry", "Cunning", "Dark Flasks", "Dawn", "Defender", "Defiler", "Deflector",
		"Destroyer", "Doomflask", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatflask", "Guardian", "Heirloom",
		"Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact",
		"Pledge", "Promise", "Protector", "Ravager", "Reach", "Reaper", "Reaver", "Savagery",
		"Sculptor", "Secret", "Slayer", "Soul", "Spine", "Terror", "Token",
		"Tribute", "Vengeance", "Voice", "Warflask", "War canister", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Unholy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Aetherius", "Agatha", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer",
		"Blackout", "Blazefury", "Blazeguard", "Blinkstrike", "Bloodquench", "Bloodweep", "Brutality", "Celeste", "Chaos", "Cometfell", "Convergence", "Darkheart",
		"Dawn", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Destiny's Song", "Dirge", "Doomflask", "Draughtbane",
		"Due Diligence", "Echo", "Eclipse", "Endbringer", "Epilogue", "Blight Flasks", "Extinction", "Faithkeeper", "Fate", "Fleshrender", "Florance", "Frenzy",
		"Fury", "Ghost Reaver", "Ghostwalker", "Burning Glass", "Glimmer", "Godslayer", "Grasscutter", "Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker",
		"Hell's Scream", "Hellfire", "Piece Maker", "Hellreaver", "Honor's Call", "Hope's End", "Infamy", "Interrogator", "Justifier", "Executioner of Kings", "Klinge",
		"pridefall", "Lament", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty", "Malice", "Mangler",
		"Massacre", "Mercy", "Misery", "Mournflask", "Narcoleptic", "Needle", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle",
		"Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir", "firekeeper", "Persuasion", "Purifier", "Rage",
		"Ragespike", "Ragnarok", "Reckoning", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Savagery", "Goblet of Flames", "Scar", "Seethe", "Severance",
		"Shadow Strike", "Shadowblaze", "Silence", "Silencer", "Silverlight", "Skullcrusher", "Slice of Life", "Soul Reaper", "Soulflask",
		"Soulburner", "Spineripper",  "Stalker", "Starshatterer", "Sting", "Stinger", "Storm", "Storm Breaker", "Stormbringer",
		"Stormcaller", "Story-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Flask", "The End", "The Facelifter",
		"The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Tranquility", "Treachery",
		"Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank", "Warmonger", "Widow Maker",
		"Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer", "Warden of Evil" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}


//----- (th2) -------------------------------------------------------------
char* generator_weapons_trap()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Assurance", "Red Obsidian", "Abyssal Shard", "Aetherius", "Agatha", "Alpha",
		"Anduril", "Anger's Tear", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer", "Black Goblet", "Blackout",
		"Chalice of a Thousand Bites", "Flasks of the Grave", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodquench",
		"Bloodweep", "Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chaos", "Cometfell", "Convergence", "Corruption", "Darkheart",
		"Darkness", "Dawn", "Dawn of Ruins", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Delta", "Desolation", "Despair", "Destiny's Song",
		"Devourer", "Dirge", "Divine Light", "Doomflasks", "Draughtbane", "Due Diligence", "Early Retirement", "Echo", "Piece Maker", "Eclipse",
		"Endbringer", "Epilogue", "Espada", "Eternal Harmony", "Eternal Rest", "Extinction", "Faithkeeper", "Fallen Champion", "Fate", "Final Achievement",
		"Fleshrender", "Florance", "Frenzy", "Fury", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grieving Flasks",
		"Gutrender", "Hatred's Bite", "Heartdrinker", "Heartstriker", "Hell's Scream", "Hellfire", "Hellreaver", "Hollow Silence", "Honor's Call", "Hope's End",
		"Infamy", "Interrogator", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Klinge", "Knight's Fall", "Knightfall", "Lament",
		"Last Rites", "Last Words", "Life's Limit", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty",
		"Malice", "Mangler", "Massacre", "Mercy", "Misery's End", "Morbid Doom", "Morbid Will", "Mournflask", "Narcoleptic", "Needle", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle", "Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir",
		"Peacekeeper", "Perfect Storm", "Persuasion", "Purifier", "Rage", "Ragespike", "Reaper", "Reaper's Toll", "Reckoning",
		"Reign of Misery", "Remorse", "Requiem", "Requiem of the Lost", "Retirement", "Righteous Might", "Rigormortis", "Savagery", "Scalpel", "Scar",
		"Seethe", "Severance", "Shadow Strike", "Shadowdrinker", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life",
		"Soul Reaper", "Soulburn", "Spineripper", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm",
		"Storm Breaker", "Stormbringer", "Stormcaller", "Storm-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Eventide",
		"The End", "The Facelifter", "The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder",
		"Tranquility", "Treachery", "Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank",
		"Warmonger", "Widow Maker", "Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };
	static const char* nm2[] = { "Massive", "Military", "Amber Infused", "Ancient", "Anguish", "Annihilation", "Antique", "Arcane", "Arched", "Assassination",
		"Atuned", "Oathkeeper's", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged",
		"Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Brutal", "Brutality", "Burning", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Decapitating", "Defiled",
		"Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire", "Dissolution", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted",
		"Engraved", "Eternal", "Executing", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused",
		"Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious",
		"Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting", "Greedy", "Grieving", "Guard's", "Guardian's",
		"Hailstorm", "Hateful", "Haunted", "Heartless", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved",
		"Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely",
		"Loyal", "Lustful", "Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Mended", "Mercenary", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Ominous", "Peacekeeper", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Deluded",
		"Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual",
		"Roaring", "Ruby Infused", "Rune-Forged", "Rusty", "Sailor's", "Sapphire Infused", "Savage", "Shadow", "Silent", "Singed", "Singing",
		"Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged",
		"Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bronze", "Copper", "Diamond", "Glass", "Gold", "Iron", "Mithril", "Obsidian", "Silver", "Skeletal", "Steel" };
	static const char* nm4[] = { "Rune", "Cube", "Trap", "Machine", "Destroyer", "Mechanism", "Puzzle", "Device", "Dice", "Snare" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Blessed Potion", "Blood Potion", "Bond", "Boon", "Breaker", "Bringer", 
		"Butcher", "Call", "burner", "Champion",  "Conqueror", "Crusader", "Cry", "Cunning", "Dark Flasks", "Dawn", "Defender", "Defiler", "Deflector",
		"Destroyer", "Doomflask", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatflask", "Guardian", "Heirloom",
		"Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact",
		"Pledge", "Promise", "Protector", "Ravager", "Reach", "Reaper", "Reaver", "Savagery",
		"Sculptor", "Secret", "Slayer", "Soul", "Spine", "Terror", "Token",
		"Tribute", "Vengeance", "Voice", "Warflask", "War canister", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Unholy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Aetherius", "Agatha", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer",
		"Blackout", "Blazefury", "Blazeguard", "Blinkstrike", "Bloodquench", "Bloodweep", "Brutality", "Celeste", "Chaos", "Cometfell", "Convergence", "Darkheart",
		"Dawn", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Destiny's Song", "Dirge", "Doomflask", "Draughtbane",
		"Due Diligence", "Echo", "Eclipse", "Endbringer", "Epilogue", "Blight Flasks", "Extinction", "Faithkeeper", "Fate", "Fleshrender", "Florance", "Frenzy",
		"Fury", "Ghost Reaver", "Ghostwalker", "Burning Glass", "Glimmer", "Godslayer", "Grasscutter", "Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker",
		"Hell's Scream", "Hellfire", "Piece Maker", "Hellreaver", "Honor's Call", "Hope's End", "Infamy", "Interrogator", "Justifier", "Executioner of Kings", "Klinge",
		"pridefall", "Lament", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty", "Malice", "Mangler",
		"Massacre", "Mercy", "Misery", "Mournflask", "Narcoleptic", "Needle", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle",
		"Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir", "firekeeper", "Persuasion", "Purifier", "Rage",
		"Ragespike", "Ragnarok", "Reckoning", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Savagery", "Goblet of Flames", "Scar", "Seethe", "Severance",
		"Shadow Strike", "Shadowblaze", "Silence", "Silencer", "Silverlight", "Skullcrusher", "Slice of Life", "Soul Reaper", "Soulflask",
		"Soulburner", "Spineripper",  "Stalker", "Starshatterer", "Sting", "Stinger", "Storm", "Storm Breaker", "Stormbringer",
		"Stormcaller", "Story-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Flask", "The End", "The Facelifter",
		"The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Tranquility", "Treachery",
		"Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank", "Warmonger", "Widow Maker",
		"Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer", "Warden of Evil" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}


//----- (th2) -------------------------------------------------------------
char* generator_weapons_claw()
{
	static char names[ 128 ]; *names = 0;
	static const char* nm1[] = { "Assurance", "Red Obsidian", "Abyssal Shard", "Aetherius", "Agatha", "Alpha",
		"Anduril", "Anger's Tear", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer", "Black Goblet", "Blackout",
		"Chalice of a Thousand Bites", "Flasks of the Grave", "Blazefury", "Blazeguard", "Blight's Plight", "Blind Justice", "Blinkstrike", "Bloodquench",
		"Bloodweep", "Broken Promise", "Brutality", "Cataclysm", "Catastrophe", "Celeste", "Chaos", "Cometfell", "Convergence", "Corruption", "Darkheart",
		"Darkness", "Dawn", "Dawn of Ruins", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Delta", "Desolation", "Despair", "Destiny's Song",
		"Devourer", "Dirge", "Divine Light", "Doomflasks", "Draughtbane", "Due Diligence", "Early Retirement", "Echo", "Piece Maker", "Eclipse",
		"Endbringer", "Epilogue", "Espada", "Eternal Harmony", "Eternal Rest", "Extinction", "Faithkeeper", "Fallen Champion", "Fate", "Final Achievement",
		"Fleshrender", "Florance", "Frenzy", "Fury", "Ghost Reaver", "Ghostwalker", "Glimmer", "Godslayer", "Grieving Flasks",
		"Gutrender", "Hatred's Bite", "Heartdrinker", "Heartstriker", "Hell's Scream", "Hellfire", "Hellreaver", "Hollow Silence", "Honor's Call", "Hope's End",
		"Infamy", "Interrogator", "Justice", "Justifier", "King's Defender", "King's Legacy", "Kinslayer", "Klinge", "Knight's Fall", "Knightfall", "Lament",
		"Last Rites", "Last Words", "Life's Limit", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty",
		"Malice", "Mangler", "Massacre", "Mercy", "Misery's End", "Morbid Doom", "Morbid Will", "Mournflask", "Narcoleptic", "Needle", "Nethersbane",
		"Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle", "Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir",
		"Peacekeeper", "Perfect Storm", "Persuasion", "Purifier", "Rage", "Ragespike", "Reaper", "Reaper's Toll", "Reckoning",
		"Reign of Misery", "Remorse", "Requiem", "Requiem of the Lost", "Retirement", "Righteous Might", "Rigormortis", "Savagery", "Scalpel", "Scar",
		"Seethe", "Severance", "Shadow Strike", "Shadowdrinker", "Silence", "Silencer", "Silver Saber", "Silverlight", "Skullcrusher", "Slice of Life",
		"Soul Reaper", "Soulburn", "Spineripper", "Stalker", "Starshatterer", "Sting", "Stinger", "Storm",
		"Storm Breaker", "Stormbringer", "Stormcaller", "Storm-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Eventide",
		"The End", "The Facelifter", "The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder",
		"Tranquility", "Treachery", "Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank",
		"Warmonger", "Widow Maker", "Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer" };
	static const char* nm2[] = { "Massive", "Military", "Amber Infused", "Ancient", "Anguish", "Annihilation", "Antique", "Arcane", "Arched", "Assassination",
		"Atuned", "Oathkeeper's", "Bandit's", "Baneful", "Banished", "Barbarian", "Barbaric", "Battleworn", "Blazefury", "Blood Infused", "Blood-Forged",
		"Bloodcursed", "Bloodied", "Bloodlord's", "Bloodsurge", "Bloodvenom", "Bone Crushing", "Brutal", "Brutality", "Burning", "Burnished", "Captain's",
		"Cataclysm", "Cataclysmic", "Cold-Forged", "Corroded", "Corrupted", "Crazed", "Crying", "Cursed", "Curved", "Dancing", "Decapitating", "Defiled",
		"Demonic", "Deserted", "Desire's", "Desolation", "Destiny's", "Dire", "Dissolution", "Doom", "Doom's", "Dragon's", "Dragonbreath", "Ebon", "Eerie", "Enchanted",
		"Engraved", "Eternal", "Executing", "Exiled", "Extinction", "Faith's", "Faithful", "Fancy", "Fearful", "Feral", "Fierce", "Fiery", "Fire Infused",
		"Fireguard", "Firesoul", "Firestorm", "Flaming", "Flimsy", "Forsaken", "Fortune's", "Fragile", "Frail", "Frenzied", "Frost", "Frozen", "Furious",
		"Fusion", "Ghastly", "Ghost-Forged", "Ghostly", "Gladiator", "Gladiator's", "Gleaming", "Glinting", "Greedy", "Grieving", "Guard's", "Guardian's",
		"Hailstorm", "Hateful", "Haunted", "Heartless", "Hollow", "Holy", "Honed", "Honor's", "Hope's", "Hopeless", "Howling", "Hungering", "Improved",
		"Incarnated", "Infused", "Inherited", "Isolated", "Jade Infused", "Judgement", "Knightly", "Legionnaire's", "Liar's", "Lich", "Lightning", "Lonely",
		"Loyal", "Lustful", "Lusting", "Mage's", "Malevolent", "Malicious", "Malignant", "Mended", "Mercenary", "Misfortune's", "Misty", "Moonlit", "Mourning",
		"Nightmare", "Ominous", "Peacekeeper", "Phantom", "Polished", "Possessed", "Pride's", "Prideful", "Primitive", "Promised", "Protector's", "Deluded",
		"Proud", "Recruit's", "Reforged", "Reincarnated", "Relentless", "Remorseful", "Renewed", "Renovated", "Replica", "Restored", "Retribution", "Ritual",
		"Roaring", "Ruby Infused", "Rune-Forged", "Rusty", "Sailor's", "Sapphire Infused", "Savage", "Shadow", "Silent", "Singed", "Singing",
		"Sinister", "Skullforge", "Skyfall", "Smooth", "Solitude's", "Sorrow's", "Soul", "Soul Infused", "Soul-Forged", "Soulcursed", "Soulless", "Spectral",
		"Spectral-Forged", "Spiteful", "Storm", "Storm-Forged", "Stormfury", "Stormguard", "Terror", "Thirsting", "Thirsty", "Thunder", "Thunder-Forged",
		"Thunderfury", "Thunderguard", "Thundersoul", "Thunderstorm", "Timeworn", "Tormented", "Trainee's", "Treachery's", "Twilight", "Twilight's", "Twisted",
		"Tyrannical", "Undead", "Unholy", "Vengeance", "Vengeful", "Venom", "Vicious", "Vindication", "Vindictive", "Void", "Volcanic", "Vowed", "War-Forged",
		"Warlord's", "Warp", "Warped", "Whistling", "Wicked", "Wind's", "Wind-Forged", "Windsong", "Woeful", "Wrathful", "Wretched", "Yearning", "Zealous" };
	static const char* nm3[] = { "Adamantite", "Bronze", "Copper", "Diamond", "Glass", "Gold", "Iron", "Mithril", "Obsidian", "Silver", "Skeletal", "Steel" };
	static const char* nm4[] = { "Claws", "Spike", "Hand", "Awl", "Piercer", "Nails", "Razors" };
	static const char* nm5[] = { "Annihilation", "Betrayer", "Blessed Potion", "Blood Potion", "Bond", "Boon", "Breaker", "Bringer", 
		"Butcher", "Call", "burner", "Champion",  "Conqueror", "Crusader", "Cry", "Cunning", "Dark Flasks", "Dawn", "Defender", "Defiler", "Deflector",
		"Destroyer", "Doomflask", "Edge", "Ender", "Etcher", "Executioner", "Favor", "Ferocity", "Foe", "Gift", "Glory", "Greatflask", "Guardian", "Heirloom",
		"Hope", "Incarnation", "Jaws", "Last Hope", "Last Stand", "Legacy", "Memory", "Might", "Oath", "Pact",
		"Pledge", "Promise", "Protector", "Ravager", "Reach", "Reaper", "Reaver", "Savagery",
		"Sculptor", "Secret", "Slayer", "Soul", "Spine", "Terror", "Token",
		"Tribute", "Vengeance", "Voice", "Warflask", "War canister", "Whisper", "Wit" };
	static const char* nm6[] = { "of Agony", "of Ancient Power", "of Anguish", "of Ashes", "of Assassins", "of Black Magic", "of Blessed Fortune", "of Blessings",
		"of Blight", "of Blood", "of Bloodlust", "of Broken Bones", "of Broken Dreams", "of Burdens", "of Chaos", "of Closing Eyes",
		"of Conquered Worlds", "of Corruption", "of Cruelty", "of Cunning", "of Dark Magic", "of Dark Souls", "of Darkness", "of Decay", "of Deception",
		"of Degradation", "of Delusions", "of Denial", "of Desecration", "of Diligence", "of Dismay", "of Dragonsouls", "of Due Diligence", "of Echoes",
		"of Ended Dreams", "of Ending Hope", "of Ending Misery", "of Eternal Bloodlust", "of Eternal Damnation", "of Eternal Glory", "of Eternal Justice",
		"of Eternal Rest", "of Eternal Sorrow", "of Eternal Struggles", "of Eternity", "of Executions", "of Faded Memories", "of Fallen Souls", "of Fools",
		"of Frost", "of Frozen Hells", "of Fury", "of Giants", "of Giantslaying", "of Grace", "of Grieving Widows", "of Hate", "of Hatred", "of Hell's Games",
		"of Hellish Torment", "of Heroes", "of Unholy Might", "of Honor", "of Hope", "of Horrid Dreams", "of Horrors", "of Illuminated Dreams", "of Illumination",
		"of Immortality", "of Inception", "of Infinite Trials", "of Insanity", "of Invocation", "of Justice", "of Light's Hope",
		"of Lost Hope", "of Lost Voices", "of Lost Worlds", "of Magic", "of Mercy", "of Misery", "of Mountains", "of Mourning", "of Mystery", "of Necromancy",
		"of Nightmares", "of Oblivion", "of Perdition", "of Phantoms", "of Power", "of Pride", "of Pride's Fall", "of Putrefaction", "of Reckoning",
		"of Redemption", "of Regret", "of Riddles", "of Secrecy", "of Secrets", "of Shadow Strikes", "of Shadows", "of Shifting Sands", "of Shifting Worlds",
		"of Silence", "of Slaughter", "of Souls", "of Stealth", "of Storms", "of Subtlety", "of Suffering", "of Suffering's End", "of Summoning", "of Terror",
		"of Thunder", "of Time-Lost Memories", "of Timeless Battles", "of Titans", "of Torment", "of Traitors", "of Trembling Hands", "of Trials", "of Truth",
		"of Twilight's End", "of Twisted Visions", "of Unholy Blight", "of Vengeance", "of Visions", "of Wasted Time", "of Widows",
		"of Wizardry", "of Woe", "of Wraiths", "of Zeal", "of the Ancients", "of the Banished", "of the Basilisk", "of the Beast", "of the Blessed",
		"of the Breaking Storm", "of the Burning Sun", "of the Caged Mind", "of the Cataclysm", "of the Champion", "of the Claw",
		"of the Corrupted", "of the Covenant", "of the Crown", "of the Damned", "of the Daywalker", "of the Dead", "of the Depth", "of the Dreadlord",
		"of the Earth", "of the East", "of the Emperor", "of the Empty Void", "of the End", "of the Enigma", "of the Fallen", "of the Falling Sky",
		"of the Flame", "of the Forest", "of the Forgotten", "of the Forsaken", "of the Gladiator", "of the Harvest", "of the Immortal", "of the Incoming Storm",
		"of the Insane", "of the King", "of the Lasting Night", "of the Leviathan", "of the Light", "of the Lion", "of the Lionheart", "of the Lone Victor",
		"of the Lone Wolf", "of the Lost", "of the Moon", "of the Moonwalker", "of the Night Sky", "of the Night", "of the Nightstalker", "of the North",
		"of the Occult", "of the Oracle", "of the Phoenix", "of the Plague", "of the Prince", "of the Protector", "of the Queen", "of the Serpent",
		"of the Setting Sun", "of the Shadows", "of the Sky", "of the South", "of the Stars", "of the Storm", "of the Summoner", "of the Sun", "of the Sunwalker",
		"of the Talon", "of the Undying", "of the Victor", "of the Void", "of the West", "of the Whispers", "of the Wicked", "of the Wind", "of the Wolf",
		"of the World", "of the Wretched" };
	static const char* nm7[] = { "Aetherius", "Agatha", "Alpha", "Amnesia", "Anduril", "Apocalypse", "Armageddon", "Arondite", "Ashrune", "Betrayal", "Betrayer",
		"Blackout", "Blazefury", "Blazeguard", "Blinkstrike", "Bloodquench", "Bloodweep", "Brutality", "Celeste", "Chaos", "Cometfell", "Convergence", "Darkheart",
		"Dawn", "Dawnbreaker", "Deathbringer", "Deathraze", "Decimation", "Desolation", "Destiny's Song", "Dirge", "Doomflask", "Draughtbane",
		"Due Diligence", "Echo", "Eclipse", "Endbringer", "Epilogue", "Blight Flasks", "Extinction", "Faithkeeper", "Fate", "Fleshrender", "Florance", "Frenzy",
		"Fury", "Ghost Reaver", "Ghostwalker", "Burning Glass", "Glimmer", "Godslayer", "Grasscutter", "Gutrender", "Hatred's Bite", "Heartseeker", "Heartstriker",
		"Hell's Scream", "Hellfire", "Piece Maker", "Hellreaver", "Honor's Call", "Hope's End", "Infamy", "Interrogator", "Justifier", "Executioner of Kings", "Klinge",
		"pridefall", "Lament", "Lifedrinker", "Light's Bane", "Lightbane", "Lightbringer", "Lightning", "Limbo", "Loyalty", "Malice", "Mangler",
		"Massacre", "Mercy", "Misery", "Mournflask", "Narcoleptic", "Needle", "Nethersbane", "Night's Edge", "Night's Fall", "Nightbane", "Nightcrackle",
		"Nightfall", "Oathbreaker", "Oathkeeper", "Oblivion", "Omega", "Orenmir", "firekeeper", "Persuasion", "Purifier", "Rage",
		"Ragespike", "Ragnarok", "Reckoning", "Reign", "Remorse", "Requiem", "Retirement", "Rigormortis", "Savagery", "Goblet of Flames", "Scar", "Seethe", "Severance",
		"Shadow Strike", "Shadowblaze", "Silence", "Silencer", "Silverlight", "Skullcrusher", "Slice of Life", "Soul Reaper", "Soulflask",
		"Soulburner", "Spineripper",  "Stalker", "Starshatterer", "Sting", "Stinger", "Storm", "Storm Breaker", "Stormbringer",
		"Stormcaller", "Story-Weaver", "Striker", "Sun Strike", "Suspension", "Swan Song", "The Ambassador", "The Black Flask", "The End", "The Facelifter",
		"The Light", "The Oculus", "The Stake", "The Untamed", "The Unyielding", "The Void", "Thorn", "Thunder", "Tranquility", "Treachery",
		"Trinity", "Tyrhung", "Unending Tyranny", "Unholy Might", "Valkyrie", "Vanquisher", "Vengeance", "Venom", "Venomshank", "Warmonger", "Widow Maker",
		"Willbreaker", "Winterthorn", "Wit's End", "Witherbrand", "Wolf", "Worldbreaker", "Worldslayer", "Warden of Evil" };

	int i = RNG(10);
	{
		if( i < 3 ){
			int rnd = RNG(countof(nm1));
			sprintf_s( names, "%s", nm1[ rnd ] );
		} else if( i < 5 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm4));
			sprintf_s( names, "%s %s", nm2[ rnd ], nm4[ rnd2 ] );
		} else if( i < 7 ){
			int rnd = RNG(countof(nm2));
			int rnd2 = RNG(countof(nm3));
			int rnd3 = RNG(countof(nm4));
			sprintf_s( names, "%s %s %s", nm2[ rnd ], nm3[ rnd2 ], nm4[ rnd3 ] );
		} else{
			int rnd = RNG(countof(nm7));
			int rnd2 = RNG(countof(nm5));
			int rnd3 = RNG(countof(nm6));
			sprintf_s( names, "%s, %s %s", nm7[ rnd ], nm5[ rnd2 ], nm6[ rnd3 ] );
		}
		return names;
	}
}

//----- (th2) -------------------------------------------------------------
char* RareName( ITEM_CODE ic, int type )
{
	switch( ic ){
	case IC_1_SWORD:		return generator_weapons_swords();
	case IC_2_AXE:			return generator_weapons_battle_axes();
		case IC_3_BOW or IC_23_PISTOL:			return generator_weapons_bows();
	case IC_4_MACE:			return generator_weapons_war_hammers();
	case IC_5_SHIELD:		return generator_armour_shields();
	case IC_6_LIGHT_ARMOR:	return generator_armour_chests(1);
	case IC_8_MID_ARMOR:	return generator_armour_chests(0);
	case IC_9_HEAVY_ARMOR:	return generator_armour_chests(0);
	case IC_7_HELM:			return generator_armour_helmets(type);
	case IC_10_STAFF:		return generator_weapons_staves();
	case IC_12_RING:		return generator_miscellaneous_jewelrys(0);
	case IC_13_AMULET:		return generator_miscellaneous_jewelrys(1);
	case IC_15_GLOVE:		return generator_armour_gauntlets(type);
	case IC_16_BOOTS:		return generator_armour_boots(type);
	case IC_17_BELT:		return generator_armour_belts(type);
	case IC_18_FLASK:		return generator_weapons_flask();
	case IC_19_TRAP:		return generator_weapons_trap();
	case IC_20_CLAW:		return generator_weapons_claw();
	case IC_21_KNIFE:		return generator_weapons_claw(); // deal with it
	case IC_22_MALLET:		return generator_weapons_war_hammers();
	default: return "Doomed Soul of Diablo";
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Item_AddAffixesRare( Item& item, int minQLVL, int maxQLVL, int occuranceMask, int specializationMask, int isGoodMagicItem, int rare )
{
	int enablePostfix, disablePrefix;
	uchar prefixExcludedCombinations = 0;
	int prefixes = 0;
	int affixMap[ countof( AffixesRare ) ];
	Affix* prefix;
	Affix* post;
	disablePrefix = 0; // RNG(4);           //	вероятность префикса 0.25
	enablePostfix = 1; // RNG(3);           //	вероятность постфикса 0.66(6)
	if( disablePrefix && !enablePostfix ){ //	если не получилось ни префикса не постфикса то что нибудь всё равно включаем
		if( RNG(2) ){
			enablePostfix = 1;
		} else{
			disablePrefix = 0;
		}
	}
	int prefixIndex = -1;
	int postfixIndex = -1;
	if( !isGoodMagicItem && RNG(3) ){
		isGoodMagicItem = 1;
	}
	int prefixId = 0;
	if( !disablePrefix ){
		prefixes = 0;
		for( ; AffixesRare[ prefixId ].effect.id != AE_NO; ++prefixId ){
			prefix = &AffixesRare[ prefixId ];
			if( (occuranceMask & prefix->item) && (specializationMask & prefix->itemSpecialization) ){
				if( prefix->quality >= minQLVL && prefix->quality <= maxQLVL ){
					if( !isGoodMagicItem || prefix->NotCursed ){
						if( occuranceMask != IA_STAFF /*Staff wo cast*/ || prefix->effect.id != AE_EXTRA_CHARGES ){
							if( is(prefix->effect.fullType(), item.effect[0].fullType(), item.effect[1].fullType(), item.effect[2].fullType(), item.effect[3].fullType()) ) continue;
							affixMap[ prefixes++ ] = prefixId;
							if( prefix->DoubleChance )
								affixMap[ prefixes++ ] = prefixId;
						}
					}
				}
			}
		}
		if( prefixes ){
			prefixIndex = affixMap[ RNG(prefixes) ];
			prefix = &AffixesRare[ prefixIndex ];
			BaseEffect e = prefix->effect;
			Item_ApplyAffix( item, e, prefix->MinGoldValue, prefix->MaxGoldValue, prefix->Multiplier, 0 + rare * 2, prefix->minReqClvl, prefix->maxReqClvl, true );
			prefixExcludedCombinations = prefix->ExcludedCombinations;
		}
	} else{
		for( ; AffixesRare[ prefixId ].effect.id != AE_NO; ++prefixId );
	}
	int firstSuffix = prefixId + 1;
	int suffixes = 0;
	if( enablePostfix ){
		for( int suffixId = 0; AffixesRare[ suffixId + firstSuffix ].effect.id != AE_NO; ++suffixId ){
			post = &AffixesRare[ suffixId + firstSuffix ];
			if( (occuranceMask & post->item) && (specializationMask & post->itemSpecialization) ){
				if( post->quality >= minQLVL && post->quality <= maxQLVL ){
					if( (prefixExcludedCombinations | post->ExcludedCombinations) != (16 | 1) ){
						if( !isGoodMagicItem || post->NotCursed ){
							if( is(post->effect.fullType(), item.effect[0].fullType(), item.effect[1].fullType(), item.effect[2].fullType(), item.effect[3].fullType()) ) continue;
							affixMap[ suffixes++ ] = suffixId;
						}
					}
				}
			}
		}
		if( suffixes ){
			postfixIndex = affixMap[ RNG(suffixes) ];
			post = &AffixesRare[ postfixIndex + firstSuffix ];// original 87
			BaseEffect e = post->effect;
			//if( rareMinus && !*rareMinus ){
			//	if( (e.minLow || e.minHigh || e.maxLow || e.maxHigh || e.chanceLow || e.chanceHigh)
			//		&& not(e.id, AE_LIFE_STEAL_1_2, AE_MANA_STEAL_1_2, AE_FAST_ATTACK_SPEED, AE_FAST_HIT_RECOVERY, AE_FAST_BLOCK) ){
			//		e.minLow = -e.minLow; e.minHigh = -e.minHigh; e.maxLow = -e.maxLow; e.maxHigh = -e.maxHigh;	e.chanceLow = -e.chanceLow;	e.chanceHigh = -e.chanceHigh;
			//		*rareMinus = 1;
			//	}
			//}
			Item_ApplyAffix( item, e, post->MinGoldValue, post->MaxGoldValue, post->Multiplier, 1 + rare * 2, post->minReqClvl, post->maxReqClvl, true );
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall Item_AddPrefixRare( Item& item, int MaxAffixQLVL, int Spell_ID, int IsMagicItem, int rare )
{
	int Affix_ID = -1;
	int affixesCount = 0;
	int AffixMap[ countof( AffixesRare ) ];
	//char Source[128];
	BaseItem& baseItem = BaseItems[ item.baseItemIndex ];
	Affix* pref = 0; // &Affixes[185]; // Инициация нулевым маркером // TODO: find out wtf is this
	if( !RNG(10) || IsMagicItem || rare ){
		while( AffixesRare[ Affix_ID + 1 ].effect.id != AE_NO ){
			pref = &AffixesRare[ ++Affix_ID ];
			if( (pref->item & IA_STAFF) && (pref->itemSpecialization & baseItem.Specialization) && pref->quality <= MaxAffixQLVL ){
				if( !IsMagicItem || pref->NotCursed ){
					if( is(pref->effect.fullType(), item.effect[0].fullType(), item.effect[1].fullType(), item.effect[2].fullType(), item.effect[3].fullType()) ) continue;
					AffixMap[ affixesCount++ ] = Affix_ID;
					if( pref->DoubleChance )
						AffixMap[ affixesCount++ ] = Affix_ID;
				}
			}
		}
		if( affixesCount ){
			Affix_ID = AffixMap[ RNG(affixesCount) ];
			pref = &AffixesRare[ Affix_ID ];
			Item_ApplyAffix( item, pref->effect, pref->MinGoldValue, pref->MaxGoldValue, pref->Multiplier, 0 + 2 * rare, pref->minReqClvl, pref->maxReqClvl, true );
		}
	}
	Item_RecalculatePrice(item);
}

//----- (th2) -------------------------------------------------------------
void InitRare( int itemIndex, int minQuality, int maxQuality, int uniqueChance, int isGoodMagicItem )
{
	Item& item = Items[ itemIndex ];
	BaseItem& baseItem = BaseItems[ item.baseItemIndex ];
	
	Item tmpItem = item;
	Item& it = item.Identified ? item : tmpItem;
	for( int i = 0; i < 2; ++i ){
		//if( minQuality > 25 ) minQuality = 25;
		ITEM_AFFIX itemAffix = ItemAffix(it);
		if( itemAffix == IA_ALL ) return;
		if( it.ItemCode == IC_10_STAFF && baseItem.Specialization & IS_MAGIC && i == 1 ){ //enableSpellCastAbility && (enableSpellCastAbility != 2 || RNG(3) == 1) ){
			Item_AddSpellCastAbility( it, maxQuality, isGoodMagicItem, i );
		} // TODO: check for else need
		Item_AddAffixesRare( it, minQuality, maxQuality, itemAffix, baseItem.Specialization, isGoodMagicItem, i );
	}
	if( ! item.Identified ){
		item.BaseDurability    = tmpItem.BaseDurability;
		item.CurDurability     = tmpItem.CurDurability;
		item.BaseCharges       = tmpItem.BaseCharges;
		item.CurCharges        = tmpItem.CurCharges;
		item.SpellIndex        = tmpItem.SpellIndex;
		item.RequiredStrength  = tmpItem.RequiredStrength;
		item.RequiredMagic     = tmpItem.RequiredMagic;
		item.RequiredDexterity = tmpItem.RequiredDexterity;
		item.RequiredVitality  = tmpItem.RequiredVitality;
		item.RequiredLevel     = tmpItem.RequiredLevel;
		item.fixedReqClvl      = tmpItem.fixedReqClvl;
	}
	item.MagicLevel = ML_2_UNIQUE;
	strcpy_s( item.FullMagicalItemName, RareName( item.ItemCode, BaseItems[item.baseItemIndex].Material ) );
	item.Price = maxQuality * 750;
	item.UniqueItemIndex = 0;
	item.dropType |= D_RARE;
	item.dropType |= D_ALLOW_DUP;
}
