#include "stdafx.h"

// Affix array at 0x0048B820
Affix Affixes[] = {
	// Name,	Effects,	type,	minLow,	minHigh,	maxLow,	maxHigh,	chanceLow,	chanceHigh,	QualityLevel,	item,	IS_ALL,	ExcludedCombinations,	DoubleChance,	NotCursed,	MinGoldValue,	MaxGoldValue,	Multiplier, CLVL req min,	CLVL req max
	


		/* ------------------------------------
		-------------- PREFIXES ---------------
		-------------------------------------*/

	/* ------------------------------------
					TO_HIT
	-------------------------------------*/

	{ 1,	"Copper",		AE_TO_HIT,	0,	0,	0,	0,	0,	  4,	  6,	  1,	IA_ALL,																IS_BATTLE,	0,	0,	1,	    50,	   200,	 1,	  0,	  0 },
	{ 2,	"Tin",			AE_TO_HIT,	0,	0,	0,	0,	0,	  7,	 10,	  7,	IA_JEWEL | IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	   525,	   750,	 2,	  0,	  0 },
	{ 3,	"Brass",		AE_TO_HIT,	0,	0,	0,	0,	0,	 11,	 20,	 12,	IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 1'050,	 1'500,	 3,	  2,	  3 },
	{ 4,	"Bronze",		AE_TO_HIT,	0,	0,	0,	0,	0,	 21,	 30,	 19,	IA_RANGE | IA_MELEE | IA_GLOVE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 1'550,	 2'000,	 4,	  4,	  6 },
	{ 5,	"Iron",			AE_TO_HIT,	0,	0,	0,	0,	0,	 31,	 40,	 28,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 2'050,	 2'500,	 5,	  8,	 11 },
	{ 6,	"Steel",		AE_TO_HIT,	0,	0,	0,	0,	0,	 41,	 50,	 37,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 2'550,	 3'000,	 6,	 12,	 16 },
	{ 7,	"Silver",		AE_TO_HIT,	0,	0,	0,	0,	0,	 51,	 60,	 46,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 3'050,	 3'500,	 7,	 17,	 19 },
	{ 8,	"Gold",			AE_TO_HIT,	0,	0,	0,	0,	0,	 61,	 70,	 55,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 3'550,	 4'000,	 8,	 23,	 25 },
	{ 9,	"Platinum",		AE_TO_HIT,	0,	0,	0,	0,	0,	 71,	 80,	 62,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 4'050,	 5'000,	 9,	 30,	 33 },
	{ 485,	"Palladium",	AE_TO_HIT,	0,	0,	0,	0,	0,	 81,	 90,	 72,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 6'000,	 7'000,	10,	 37,	 39 },
	{ 486,	"Palladium",	AE_TO_HIT,	0,	0,	0,	0,	0,	 91,	100,	 82,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,						IS_BATTLE,	16,	0,	1,	 8'000,	 9'000,	11,	 44,	 46 },
	{ 487,	"Extravagant",	AE_TO_HIT,	0,	0,	0,	0,	0,	101,	110,	 92,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	10'000,	11'000,	11,	 51,	 53 },
	{ 488,	"Meteoric",		AE_TO_HIT,	0,	0,	0,	0,	0,	111,	120,	102,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	12'000,	13'000,	12,	 58,	 60 },
	{ 489,	"Meteoric",		AE_TO_HIT,	0,	0,	0,	0,	0,	121,	130,	112,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	14'000,	15'000,	12,	 65,	 67 },
	{ 490,	"Superior",		AE_TO_HIT,	0,	0,	0,	0,	0,	131,	140,	122,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	16'000,	17'000,	13,	 72,	 74 },
	{ 491,	"Superior",		AE_TO_HIT,	0,	0,	0,	0,	0,	141,	150,	132,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	18'000,	19'000,	13,	 76,	 79 },
	{ 492,	"Guiding",		AE_TO_HIT,	0,	0,	0,	0,	0,	151,	160,	142,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	20'000,	21'000,	14,	 81,	 84 },
	{ 493,	"Guiding",		AE_TO_HIT,	0,	0,	0,	0,	0,	161,	170,	152,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	22'000,	23'000,	14,	 86,	 88 },
	{ 494,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	171,	180,	162,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	24'000,	25'000,	15,	 90,	 94 },
	{ 495,	"Weird",		AE_TO_HIT,	0,	0,	0,	0,	0,	181,	190,	172,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	26'000,	27'000,	15,	 96,	 99 },
	{ 496,	"Strange",		AE_TO_HIT,	0,	0,	0,	0,	0,	191,	200,	178,	IA_MELEE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	0,	28'000,	29'000,	16,	100,	116 },
	
	//jewelry
	{  55,	"Superior",		AE_TO_HIT,	0,	0,	0,	0,	0,	11,	15,	 15,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 1'100,	 1'500,	4,	 3,	 4 },
	{  55,	"Guiding",		AE_TO_HIT,	0,	0,	0,	0,	0,	16,	20,	 21,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 2'400,	 3'000,	5,	14,	16 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	21,	25,	 28,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 4'200,	 5'000,	6,	18,	20 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	26,	30,	 36,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 7'800,	 9'000,	6,	23,	26 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	31,	35,	 45,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	12'400,	14'000,	7,	28,	31 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	36,	40,	 57,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	21'600,	24'000,	7,	35,	37 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	41,	45,	 75,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	32'800,	35'000,	8,	45,	47 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	46,	50,	 90,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	36'000,	40'000,	8,	52,	57 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	51,	55,	110,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	41'000,	45'000,	9,	62,	67 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	56,	60,	130,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	46'000,	50'000,	9,	72,	77 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	61,	65,	150,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	51'000,	55'000,	9,	82,	87 },
	{  84,	"Leading",		AE_TO_HIT,	0,	0,	0,	0,	0,	66,	70,	170,	IA_JEWEL,	IS_BATTLE,	0,	0,	0,	56'000,	60'000,	9,	92,	97 },


	/* ------------------------------------
		DAMAGE PERCENT, ON WEAPONS
	-------------------------------------*/

	{  19,	"Fierce",	AE_PERCENT_DAMAGE,	0,	 25,	 40,	0,	0,	0,	0,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   200,	 2,	0,	0 },
	{  20,	"Brutal",	AE_PERCENT_DAMAGE,	0,	 41,	 55,	0,	0,	0,	0,	  5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   357,	   525,	 2,	2,	3 },
	{  21,	"Injuring",	AE_PERCENT_DAMAGE,	0,	 56,	 70,	0,	0,	0,	0,	 10,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   760,	 1'000,	 3,	4,	5 },
	{  22,	"Furious",	AE_PERCENT_DAMAGE,	0,	 71,	 85,	0,	0,	0,	0,	 15,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'111,	 1'375,	 3,	6,	7 },
	{  23,	"Vicious",	AE_PERCENT_DAMAGE,	0,	 86,	100,	0,	0,	0,	0,	 20,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'512,	 1'800,	 4,	8,	10 },
	{  24,	"Severe",	AE_PERCENT_DAMAGE,	0,	101,	115,	0,	0,	0,	0,	 25,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'963,	 2'275,	 4,	12,	17 },
	{  25,	"Cruel",	AE_PERCENT_DAMAGE,	0,	116,	130,	0,	0,	0,	0,	 30,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'464,	 2'800,	 5,	21,	24 },
	{  26,	"Sadistic",	AE_PERCENT_DAMAGE,	0,	131,	145,	0,	0,	0,	0,	 35,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'015,	 3'375,	 5,	26,	28 },
	{  27,	"Merciless",AE_PERCENT_DAMAGE,	0,	146,	160,	0,	0,	0,	0,	 40,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'616,	 4'000,	 6,	29,	32 },
	{  28,	"Insane",	AE_PERCENT_DAMAGE,	0,	161,	175,	0,	0,	0,	0,	 45,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'267,	 4'675,	 6,	33,	35 },
	{  29,	"Deadly",	AE_PERCENT_DAMAGE,	0,	176,	190,	0,	0,	0,	0,	 50,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 7,	36,	38 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	191,	205,	0,	0,	0,	0,	 60,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	13'020,	13'500,	 8,	39,	41 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	206,	220,	0,	0,	0,	0,	 70,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	16'000,	16'000,	 9,	42,	46 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	221,	235,	0,	0,	0,	0,	 85,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	 9,	47,	51 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	236,	250,	0,	0,	0,	0,	100,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	52,	56 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	251,	265,	0,	0,	0,	0,	115,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	22'000,	22'000,	10,	58,	62 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	266,	280,	0,	0,	0,	0,	130,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	11,	64,	72 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	281,	295,	0,	0,	0,	0,	145,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	26'000,	26'000,	11,	73,	78 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	296,	310,	0,	0,	0,	0,	160,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	28'000,	28'000,	12,	82,	87 },
	{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	311,	325,	0,	0,	0,	0,	177,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	30'000,	30'000,	13,	92,	99 },


	/* ------------------------------------
	LOW DURABILITY + HIGH PERCENT DAMAGE
	-------------------------------------*/

	{  14,	"Brash",		AE_LOW_DUR_ADD_DAMAGE,	0,	25,	45,	0,	0,	0,	0,	19,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	   750,	 1'750,	 5,	15,	17 },
	{  15,	"Fragile",		AE_LOW_DUR_ADD_DAMAGE,	0,	65,	75,	0,	0,	0,	0,	32,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	 3'250,	 3'750,	 8,	26,	31 },
	{  16,	"Brittle",		AE_LOW_DUR_ADD_DAMAGE,	0,	85,	90,	0,	0,	0,	0,	57,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	 8'500,	 9'500,	11,	35,	37 },
	{  16,	"Crystalline",	AE_LOW_DUR_ADD_DAMAGE,	0,	90,	95,	0,	0,	0,	0,	97,	IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	17'000,	19'000,	13,	61,	64 },


	/* ------------------------------------
	TO_HIT + DAMAGE PERCENT, ON WEAPONS
	-------------------------------------*/

	{ 45,	"Commoner's",	AE_TO_HIT_DAMAGE,	0,	 16,	 30,	0,	0,	 1,	  5,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	   300,	   600,	 2,	  0,	  0 },
	{ 46,	"Recruit's",	AE_TO_HIT_DAMAGE,	0,	 31,	 45,	0,	0,	 6,	 10,	  8,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	 1'080,	 1'950,	 2,	  2,	  3 },
	{ 47,	"Fighter's",	AE_TO_HIT_DAMAGE,	0,	 41,	 50,	0,	0,	11,	 15,	 14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	 2'040,	 3'200,	 3,	  4,	  5 },
	{ 48,	"Squire's",		AE_TO_HIT_DAMAGE,	0,	 51,	 60,	0,	0,	16,	 20,	 21,	IA_RANGE | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	16,	0,	1,	 3'300,	 4'750,	 4,	 11,	 13 },
	{ 49,	"Soldier's",	AE_TO_HIT_DAMAGE,	0,	 61,	 70,	0,	0,	21,	 25,	 28,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 4'455,	 6'050,	 5,	 16,	 18 },
	{ 50,	"Warrior's",	AE_TO_HIT_DAMAGE,	0,	 71,	 80,	0,	0,	26,	 30,	 34,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 5'760,	 7'500,	 6,	 22,	 24 },
	{ 51,	"Champion's",	AE_TO_HIT_DAMAGE,	0,	 81,	 90,	0,	0,	31,	 35,	 41,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 6'240,	 9'100,	 7,	 25,	 27 },
	{ 52,	"Knight's",		AE_TO_HIT_DAMAGE,	0,	 91,	100,	0,	0,	36,	 40,	 49,	IA_RANGE | IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,							IS_BATTLE,	16,	0,	1,	 7'770,	10'850,	 8,	 28,	 31 },
	{ 53,	"Lord's",		AE_TO_HIT_DAMAGE,	0,	101,	110,	0,	0,	41,	 45,	 57,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	 8'325,	12'750,	 9,	 32,	 34 },
	{ 54,	"Baron's",		AE_TO_HIT_DAMAGE,	0,	111,	120,	0,	0,	46,	 50,	 66,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	10'080,	16'000,	10,	 39,	 42 },
	{ 54,	"Duke's",		AE_TO_HIT_DAMAGE,	0,	121,	130,	0,	0,	51,	 60,	 95,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	25'000,	25'000,	11,	 61,	 66 },
	{ 54,	"King's",		AE_TO_HIT_DAMAGE,	0,	131,	140,	0,	0,	61,	 70,	117,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	35'000,	35'000,	12,	 71,	 73 },
	{ 54,	"King's",		AE_TO_HIT_DAMAGE,	0,	141,	150,	0,	0,	71,	 80,	138,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	43'000,	43'000,	13,	 74,	 77 },
	{ 54,	"King's",		AE_TO_HIT_DAMAGE,	0,	151,	160,	0,	0,	81,	 90,	157,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	1,	51'000,	51'000,	15,	 79,	 83 },
	{ 54,	"Emperor's",	AE_TO_HIT_DAMAGE,	0,	161,	170,	0,	0,	91,	100,	178,	IA_MELEE | IA_KNIFE | IA_MALLET | IA_PISTOL,									IS_BATTLE,	16,	0,	0,	60'000,	60'000,	25,	 91,	106 },


	/* ------------------------------------
		SPECIES DAMAGE PERCENT, ON WEAPONS
	-------------------------------------*/

	//undead
	{ 30,	"Consecrated",		AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	 51,	100,	0,	0,	0,	0,	  3,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
	{ 31,	"Pure",				AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	101,	200,	0,	0,	0,	0,	  8,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
	{ 32,	"Sacred",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	201,	300,	0,	0,	0,	0,	 16,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
	{ 33,	"Hallowed",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	301,	400,	0,	0,	0,	0,	 32,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
	{ 34,	"Holy",				AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	351,	450,	0,	0,	0,	0,	 60,	IA_STAFF | IA_AXE | IA_MACE_2H,					IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
	{ 43,	"Hallowed",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	351,	450,	0,	0,	0,	0,	 62,	IA_MACE_1H | IA_CLAW,							IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
	{ 43,	"Hallowed",			AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	401,	500,	0,	0,	0,	0,	 92,	IA_MACE_1H | IA_CLAW,							IS_BATTLE,	0,	0,	0,	18'000,	18'000,	 8,	53,	58 },
	{ 44,	"Holy",				AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	401,	500,	0,	0,	0,	0,	100,	IA_STAFF | IA_AXE | IA_MACE_2H,					IS_BATTLE,	0,	0,	0,	30'000,	30'000,	10,	55,	61 },

	//demon
	{ 35,	"Lunar",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		 51,	100,	0,	0,	0,	0,	  3,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
	{ 36,	"Arcadian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		101,	200,	0,	0,	0,	0,	  8,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
	{ 37,	"Astral",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		201,	300,	0,	0,	0,	0,	 16,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
	{ 38,	"Elysian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		301,	400,	0,	0,	0,	0,	 32,	IA_STAFF | IA_MELEE,							IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
	{ 39,	"Celestial",		AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		351,	450,	0,	0,	0,	0,	 60,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
	{ 43,	"Elysian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		351,	450,	0,	0,	0,	0,	 62,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
	{ 43,	"Elysian",			AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		401,	500,	0,	0,	0,	0,	 92,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	0,	18'000,	18'000,	 8,	53,	58 },
	{ 44,	"Celestial",		AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,		401,	500,	0,	0,	0,	0,	100,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	30'000,	30'000,	10,	55,	61 },

	//beast
	{ 40,	"Savage",			AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		 51,	100,	0,	0,	0,	0,	  3,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
	{ 41,	"Flaying",			AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		101,	200,	0,	0,	0,	0,	  8,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	 4,	 3,	 4 },
	{ 42,	"Eviscerating",		AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		201,	300,	0,	0,	0,	0,	 16,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 6,	10,	12 },
	{ 43,	"Disemboweling",	AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		301,	400,	0,	0,	0,	0,	 32,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,			IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	 8,	20,	24 },
	{ 44,	"Butchering",		AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		351,	450,	0,	0,	0,	0,	 60,	IA_STAFF | IA_SWORD_2H | IA_AXE,				IS_BATTLE,	0,	0,	1,	20'000,	20'000,	10,	33,	36 },
	{ 43,	"Disemboweling",	AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		351,	450,	0,	0,	0,	0,	 62,	IA_SWORD | IA_CLAW,								IS_BATTLE,	0,	0,	1,	12'000,	12'000,	 8,	34,	37 },
	{ 43,	"Disemboweling",	AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		401,	500,	0,	0,	0,	0,	 92,	IA_SWORD | IA_CLAW,								IS_BATTLE,	0,	0,	0,	18'000,	18'000,	 8,	53,	58 },
	{ 44,	"Butchering",		AE_SPICIES_DAMAGE_PERCENT,	MON_2_BEAST,		401,	500,	0,	0,	0,	0,	100,	IA_STAFF | IA_SWORD_2H | IA_AXE,				IS_BATTLE,	0,	0,	0,	30'000,	30'000,	10,	55,	61 },


	/* ------------------------------------
				DFE ON JEWELRY
	-------------------------------------*/

	{  10,	"Neophyte's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 12,	IA_JEWEL,	IS_ALL,	16,	0,	1,	   500,	   500,	2,	 2,	  3 },
	{  11,	"Warden's",		AE_DFE,	0,	 -2,	 -2,	0,	0,	0,	0,	 24,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 2'000,	 2'000,	3,	 8,	 10 },
	{  12,	"Sentinel's",	AE_DFE,	0,	 -3,	 -3,	0,	0,	0,	0,	 38,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 4'000,	 4'000,	4,	15,	 19 },
	{  13,	"Sentinel's",	AE_DFE,	0,	 -4,	 -4,	0,	0,	0,	0,	 56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 6'000,	 6'000,	5,	23,	 27 },
	{  13,	"Sentinel's",	AE_DFE,	0,	 -5,	 -5,	0,	0,	0,	0,	 80,	IA_JEWEL,	IS_ALL,	16,	0,	1,	 9'000,	 9'000,	5,	33,	 37 },
	{  13,	"Sentinel's",	AE_DFE,	0,	 -6,	 -7,	0,	0,	0,	0,	100,	IA_JEWEL,	IS_ALL,	16,	0,	1,	11'000,	11'000,	6,	48,	 52 },
	{  13,	"Sentinel's",	AE_DFE,	0,	 -8,	 -9,	0,	0,	0,	0,	120,	IA_JEWEL,	IS_ALL,	16,	0,	1,	14'000,	14'000,	6,	63,	 67 },
	{  13,	"Sentinel's",	AE_DFE,	0,	-10,	-11,	0,	0,	0,	0,	140,	IA_JEWEL,	IS_ALL,	16,	0,	1,	17'000,	17'000,	7,	78,	 82 },
	{  13,	"Sentinel's",	AE_DFE,	0,	-12,	-13,	0,	0,	0,	0,	160,	IA_JEWEL,	IS_ALL,	16,	0,	1,	21'000,	21'000,	7,	93,	 97 },
	{  13,	"Sentinel's",	AE_DFE,	0,	-14,	-15,	0,	0,	0,	0,	178,	IA_JEWEL,	IS_ALL,	16,	0,	0,	25'000,	25'000,	8,	99,	107 },
	//expand [wip	]
	//{  13,	"Veteran's",	AE_DFE,	0,	-5,	-5,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
	//{  13,	"Veteran's",	AE_DFE,	0,	-6,	-6,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
	//{  13,	"Veteran's",	AE_DFE,	0,	-7,	-7,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },
	//{  13,	"Veteran's",	AE_DFE,	0,	-8,	-8,	0,	0,	0,	0,	56,	IA_JEWEL,	IS_ALL,	16,	0,	1,	5000,	7000,	5,	23,	23 + 2 },

	
	/* ------------------------------------
				VARIOUS
	-------------------------------------*/
	
	
	{ 57,	"Sturdy",	AE_PERCENT_AC,	0,	25,	30,	0,	0,	0,	0,	33,	IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	0,	2'500,	3'000,	10,	22,	25 }, 

	
	{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	25,	IA_HELM,	IS_ALL,	0,	0,	0,	 3'000,	 3'000,	5,	15,	17 }, // they are cursed now!
	{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	48,	IA_HELM,	IS_ALL,	0,	0,	0,	 7'000,	 7'000,	7,	31,	35 },
	{ 59,	"Gazing",	AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0,	90,	IA_HELM,	IS_ALL,	0,	0,	0,	13'000,	13'000,	9,	58,	65 },

	{ 60,	"Plentiful",	AE_EXTRA_CHARGES,	0,	2,	2,	0,	0,	0,	0,	 6,	IA_STAFF,	IS_MAGIC,	0,	0,	1,	2'000,	2'000,	2,	 0,	 0 },
	{ 61,	"Bountiful",	AE_EXTRA_CHARGES,	0,	3,	3,	0,	0,	0,	0,	15,	IA_STAFF,	IS_MAGIC,	0,	0,	1,	3'000,	3'000,	3,	10,	12 }, 
	
	
	/* ------------------------------------
			MAGIC RESISTANCES
	-------------------------------------*/
	
	// fire	
	{ 62,	"Carnelian",		AE_RESIST,	ET_1_FIRE,	  5,	 10,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
	{ 62,	"Rhodochrosite",	AE_RESIST,	ET_1_FIRE,	 10,	 20,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 63,	"Clinohumite",		AE_RESIST,	ET_1_FIRE,	 20,	 30,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
	{ 63,	"Vanadinite",		AE_RESIST,	ET_1_FIRE,	 30,	 40,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
	{ 63,	"Jasper",			AE_RESIST,	ET_1_FIRE,	 40,	 50,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
	{ 64,	"Garnet",			AE_RESIST,	ET_1_FIRE,	 50,	 60,	0,	0,	0,	0,	 55,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
	{ 64,	"Cinnabar",			AE_RESIST,	ET_1_FIRE,	 60,	 70,	0,	0,	0,	0,	 75,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
	{ 64,	"Rubellite",		AE_RESIST,	ET_1_FIRE,	 70,	 80,	0,	0,	0,	0,	 95,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	59,	61 },
	{ 64,	"Hessonite",		AE_RESIST,	ET_1_FIRE,	 80,	 90,	0,	0,	0,	0,	115,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	67,	71 },
	{ 64,	"Spessarite",		AE_RESIST,	ET_1_FIRE,	 90,	100,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	80,	84 },
	{ 64,	"Ruby",				AE_RESIST,	ET_1_FIRE,	100,	120,	0,	0,	0,	0,	168,	IA_SHIELD | IA_ARMOR,						IS_ALL,	0,	0,	0,	50'000,	60'000,	9,	87,	91 },

	// lite
	{ 62,	"Chalcanthite",	AE_RESIST,	ET_2_LIGHTNING,	  5,	 10,	0,	0,	0,	0,	  3,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
	{ 62,	"Chalcedony",	AE_RESIST,	ET_2_LIGHTNING,	 10,	 20,	0,	0,	0,	0,	  5,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 63,	"Benitoite",	AE_RESIST,	ET_2_LIGHTNING,	 20,	 30,	0,	0,	0,	0,	 12,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
	{ 63,	"Lazurite",		AE_RESIST,	ET_2_LIGHTNING,	 30,	 40,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
	{ 63,	"Azurite",		AE_RESIST,	ET_2_LIGHTNING,	 40,	 50,	0,	0,	0,	0,	 39,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
	{ 64,	"Lapis",		AE_RESIST,	ET_2_LIGHTNING,	 50,	 60,	0,	0,	0,	0,	 56,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
	{ 64,	"Cobalt",		AE_RESIST,	ET_2_LIGHTNING,	 60,	 70,	0,	0,	0,	0,	 76,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
	{ 64,	"Larimar",		AE_RESIST,	ET_2_LIGHTNING,	 70,	 80,	0,	0,	0,	0,	 96,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	59,	61 },
	{ 64,	"Kyanite",		AE_RESIST,	ET_2_LIGHTNING,	 80,	 90,	0,	0,	0,	0,	116,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	67,	71 },
	{ 64,	"Spectrolite",	AE_RESIST,	ET_2_LIGHTNING,	 90,	100,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	80,	84 },
	{ 64,	"Sapphire",		AE_RESIST,	ET_2_LIGHTNING,	100,	120,	0,	0,	0,	0,	168,	IA_SHIELD | IA_ARMOR,						IS_ALL,	0,	0,	0,	50'000,	60'000,	9,	87,	91 },

	// arcane	
	{ 62,	"Poudretteite",	AE_RESIST,	ET_3_ARCAN,	  5,	 10,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
	{ 62,	"Lepidolite",	AE_RESIST,	ET_3_ARCAN,	 10,	 20,	0,	0,	0,	0,	  6,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 63,	"Purpurite",	AE_RESIST,	ET_3_ARCAN,	 20,	 30,	0,	0,	0,	0,	 13,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
	{ 63,	"Taaffeite",	AE_RESIST,	ET_3_ARCAN,	 30,	 40,	0,	0,	0,	0,	 22,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
	{ 63,	"Pezzottaite",	AE_RESIST,	ET_3_ARCAN,	 40,	 50,	0,	0,	0,	0,	 40,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
	{ 64,	"Kunzite",		AE_RESIST,	ET_3_ARCAN,	 50,	 60,	0,	0,	0,	0,	 57,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
	{ 64,	"Iolite",		AE_RESIST,	ET_3_ARCAN,	 60,	 70,	0,	0,	0,	0,	 77,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
	{ 64,	"Charoite",		AE_RESIST,	ET_3_ARCAN,	 70,	 80,	0,	0,	0,	0,	 97,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	59,	61 },
	{ 64,	"Sugilite",		AE_RESIST,	ET_3_ARCAN,	 80,	 90,	0,	0,	0,	0,	117,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	67,	71 },
	{ 64,	"Alexandrite",	AE_RESIST,	ET_3_ARCAN,	 90,	100,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	80,	84 },
	{ 64,	"Amethyst",		AE_RESIST,	ET_3_ARCAN,	100,	120,	0,	0,	0,	0,	168,	IA_SHIELD | IA_ARMOR,						IS_ALL,	0,	0,	0,	50'000,	60'000,	9,	87,	91 },

	// acid
	{ 62,	"Prasiolite",	AE_RESIST,	ET_4_ACID,	  5,	 10,	0,	0,	0,	0,	  5,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
	{ 62,	"Olivine",		AE_RESIST,	ET_4_ACID,	 10,	 20,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 63,	"Malachite",	AE_RESIST,	ET_4_ACID,	 20,	 30,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
	{ 63,	"Fuchsite",		AE_RESIST,	ET_4_ACID,	 30,	 40,	0,	0,	0,	0,	 23,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
	{ 63,	"Beryl",		AE_RESIST,	ET_4_ACID,	 40,	 50,	0,	0,	0,	0,	 41,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
	{ 64,	"Turquoise",	AE_RESIST,	ET_4_ACID,	 50,	 60,	0,	0,	0,	0,	 58,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
	{ 64,	"Aventurine",	AE_RESIST,	ET_4_ACID,	 60,	 70,	0,	0,	0,	0,	 78,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
	{ 64,	"Epidote",		AE_RESIST,	ET_4_ACID,	 70,	 80,	0,	0,	0,	0,	 98,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	59,	61 },
	{ 64,	"Jade",			AE_RESIST,	ET_4_ACID,	 80,	 90,	0,	0,	0,	0,	118,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	68,	72 },
	{ 64,	"Peridot",		AE_RESIST,	ET_4_ACID,	 90,	100,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	80,	84 },
	{ 64,	"Emerald",		AE_RESIST,	ET_4_ACID,	100,	120,	0,	0,	0,	0,	168,	IA_SHIELD | IA_ARMOR,						IS_ALL,	0,	0,	0,	50'000,	60'000,	9,	87,	91 },

	// cold	
	{ 62,	"Goshenite",	AE_RESIST,	ET_6_COLD,	  5,	 10,	0,	0,	0,	0,	  6,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	   100,	2,	 0,	 0 },
	{ 62,	"Scolecite",	AE_RESIST,	ET_6_COLD,	 10,	 20,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 63,	"Selenite",		AE_RESIST,	ET_6_COLD,	 20,	 30,	0,	0,	0,	0,	 15,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'500,	 3'000,	3,	 2,	 3 },
	{ 63,	"Magnesite",	AE_RESIST,	ET_6_COLD,	 30,	 40,	0,	0,	0,	0,	 24,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	 4'000,	3,	12,	15 },
	{ 63,	"Ulexite",		AE_RESIST,	ET_6_COLD,	 40,	 50,	0,	0,	0,	0,	 42,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	 7'000,	4,	24,	27 },
	{ 64,	"Calcite",		AE_RESIST,	ET_6_COLD,	 50,	 60,	0,	0,	0,	0,	 59,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	10'000,	4,	31,	35 },
	{ 64,	"Howlite",		AE_RESIST,	ET_6_COLD,	 60,	 70,	0,	0,	0,	0,	 79,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	13'000,	5,	45,	49 },
	{ 64,	"Natrolite",	AE_RESIST,	ET_6_COLD,	 70,	 80,	0,	0,	0,	0,	 99,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	17'000,	6,	59,	61 },
	{ 64,	"Moonstone",	AE_RESIST,	ET_6_COLD,	 80,	 90,	0,	0,	0,	0,	119,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	25'000,	7,	69,	73 },
	{ 64,	"Pearl",		AE_RESIST,	ET_6_COLD,	 90,	100,	0,	0,	0,	0,	135,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	50'000,	8,	80,	84 },
	{ 64,	"Diamond",		AE_RESIST,	ET_6_COLD,	100,	120,	0,	0,	0,	0,	168,	IA_SHIELD | IA_ARMOR,						IS_ALL,	0,	0,	0,	50'000,	60'000,	9,	87,	91 },

	// all	
	{ 62,	"Andratite",	AE_RESIST_All,	0,	 2,	 4,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	    100,	 2,	  0,	  0 },
	{ 62,	"Andalusite",	AE_RESIST_All,	0,	 6,	 8,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	    600,	 2,	  0,	  0 },
	{ 63,	"Zoisite",		AE_RESIST_All,	0,	 9,	11,	0,	0,	0,	0,	 17,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  3'000,	 3,	  2,	  3 },
	{ 63,	"Andesine",		AE_RESIST_All,	0,	12,	15,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	  4'000,	 3,	 12,	 15 },
	{ 63,	"Ametrine",		AE_RESIST_All,	0,	16,	18,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	  7'000,	 4,	 24,	 27 },
	{ 64,	"Ammolite",		AE_RESIST_All,	0,	19,	21,	0,	0,	0,	0,	 61,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	 10'000,	 4,	 31,	 35 },
	{ 64,	"Pietersite",	AE_RESIST_All,	0,	22,	24,	0,	0,	0,	0,	 82,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	 13'000,	 5,	 45,	 49 },
	{ 64,	"Opal",			AE_RESIST_All,	0,	25,	27,	0,	0,	0,	0,	105,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	 17'000,	 6,	 59,	 61 },
	{ 64,	"Agate",		AE_RESIST_All,	0,	28,	30,	0,	0,	0,	0,	125,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	 25'000,	 7,	 74,	 80 },
	{ 64,	"Labradorite",	AE_RESIST_All,	0,	31,	35,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,								IS_ALL,	0,	0,	0,	93'000,	105'000,	14,	104,	118 },


	/* ------------------------------------
				VARIOUS
	-------------------------------------*/

	// life steal
	{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
	{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
	{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	52,	58 },

	// mana steal
	{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
	{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
	{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	52,	58 },

	{ 86,	"Curing",		AE_LIFE_REGEN_PERCENT,	0,	55,	 75,	0,	0,	0,	0,	12,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_TRAP,	IS_BATTLE,	0,	0,	1,	 1'750,	 2'250,	3,	 0,	 2 },
	{ 87,	"Healing",		AE_LIFE_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	28,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	 5'000,	 7'500,	4,	15,	18 },
	{ 87,	"Healing",		AE_LIFE_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	54,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	 8'000,	10'000,	5,	25,	31 },
	{ 87,	"Healing",		AE_LIFE_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	94,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_BATTLE,	0,	0,	1,	12'000,	15'000,	6,	53,	56 },

	{ 88,	"Refilling",	AE_MANA_REGEN_PERCENT,	0,	55,	 75,	0,	0,	0,	0,	12,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_TRAP,	IS_MAGIC,	0,	0,	1,	1'750,	 2'250,	3,	 0,	 2 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	28,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	5'000,	 7'500,	4,	15,	18 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	54,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	8'000,	10'000,	5,	30,	34 },
	{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	80,	100,	0,	0,	0,	0,	94,	IA_STAFF | IA_JEWEL | IA_SHIELD | IA_ARMOR,				IS_MAGIC,	0,	0,	1,	12'000,	15'000,	6,	52,	58 },

		// what's up with this '2' in curse field???
	{ 90,	"Boar's",		AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	25,	IA_RANGE | IA_PISTOL,	IS_BATTLE,	1,	0,	2,	2'000,	2'000,	 6,	13,	16 },
	{ 90,	"Boar's",		AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	48,	IA_RANGE | IA_PISTOL,	IS_BATTLE,	1,	0,	2,	4'000,	4'000,	 8,	31,	33 },
	{ 90,	"Boar's",		AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	94,	IA_RANGE | IA_PISTOL,	IS_BATTLE,	1,	0,	2,	8'000,	8'000,	11,	52,	58 },

	// + damage on jewelry
	{ 91,	"Damaging",		AE_DAMAGE,	0,	 1,	  1,	0,	0,	0,	0,	  1,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	    50,	    50,	 1,   0,	  0 }, 
	{ 92,	"Tearing",		AE_DAMAGE,	0,	 2,	  2,	0,	0,	0,	0,	  5,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   150,	   150,	 2,	  0,	  2 },
	{ 92,	"Tearing",		AE_DAMAGE,	0,	 3,	  3,	0,	0,	0,	0,	 10,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   450,	   450,	 2,	  0,	  2 },
	{ 92,	"Tearing",		AE_DAMAGE,	0,	 4,	  5,	0,	0,	0,	0,	 15,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	   600,	   750,	 2,	  3,	  5 },
	{ 93,	"Pounding",		AE_DAMAGE,	0,	 6,	 10,	0,	0,	0,	0,	 20,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 1'200,	 2'000,	 3,	  6,	  8 },
	{ 93,	"Battering",	AE_DAMAGE,	0,	11,	 15,	0,	0,	0,	0,	 25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 2'200,	 3'000,	 4,	 11,	 14 },
	{ 94,	"Smashing",		AE_DAMAGE,	0,	16,	 20,	0,	0,	0,	0,	 30,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 3'200,	 4'000,	 5,	 20,	 22 },
	{ 94,	"Hammering",	AE_DAMAGE,	0,	21,	 25,	0,	0,	0,	0,	 35,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 4'200,	 5'000,	 6,	 23,	 25 },
	{ 94,	"Thrashing",	AE_DAMAGE,	0,	26,	 30,	0,	0,	0,	0,	 40,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 5'200,	 6'000,	 6,	 26,	 29 },
	{ 95,	"Crushing",		AE_DAMAGE,	0,	31,	 35,	0,	0,	0,	0,	 45,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 6'200,	 7'000,	 7,	 31,	 33 },
	{ 95,	"Pummeling",	AE_DAMAGE,	0,	36,	 40,	0,	0,	0,	0,	 50,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 7'200,	 8'000,	 8,	 34,	 37 },
	{ 95,	"Razing",		AE_DAMAGE,	0,	41,	 45,	0,	0,	0,	0,	 55,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 8'200,	 9'000,	 9,	 38,	 40 },
	{ 95,	"Crippling",	AE_DAMAGE,	0,	46,	 50,	0,	0,	0,	0,	 60,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	 9'200,	10'000,	10,	 41,	 42 },
	{ 95,	"Shattering",	AE_DAMAGE,	0,	51,	 55,	0,	0,	0,	0,	 65,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	10'200,	11'000,	11,	 45,	 47 },
	{ 95,	"Pulverizing",	AE_DAMAGE,	0,	56,	 60,	0,	0,	0,	0,	 75,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	11'200,	12'000,	11,	 45,	 47 },
	{ 95,	"Decimating",	AE_DAMAGE,	0,	61,	 65,	0,	0,	0,	0,	 85,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	12'200,	13'000,	11,	 55,	 59 },
	{ 95,	"Breaking",		AE_DAMAGE,	0,	66,	 70,	0,	0,	0,	0,	 95,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	13'200,	14'000,	12,	 60,	 63 },
	{ 95,	"Demolishing",	AE_DAMAGE,	0,	71,	 75,	0,	0,	0,	0,	105,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	14'200,	15'000,	13,	 64,	 70 },
	{ 95,	"Shattering",	AE_DAMAGE,	0,	76,	 80,	0,	0,	0,	0,	115,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	15'200,	16'000,	13,	 71,	 75 },
	{ 95,	"Obliterating",	AE_DAMAGE,	0,	81,	 85,	0,	0,	0,	0,	125,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	16'200,	17'000,	14,	 76,	 80 },
	{ 95,	"Eliminating",	AE_DAMAGE,	0,	86,	 90,	0,	0,	0,	0,	135,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	17'200,	18'000,	14,	 81,	 90 },
	{ 95,	"Annihilating",	AE_DAMAGE,	0,	91,	 95,	0,	0,	0,	0,	150,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	18'200,	19'000,	14,	 91,	100 },
	{ 95,	"Apocalyptic",	AE_DAMAGE,	0,	96,	100,	0,	0,	0,	0,	171,	IA_JEWEL,	IS_BATTLE,	0,	0,	0,	19'200,	20'000,	15,	109,	116 },

	// ac in jewelry
	{ 96,	"Protective",	AE_SPECIAL_AC,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_JEWEL,	IS_ALL,	0,	0,	1,	    50,	    50,	1,	 0,	  0 },
	{ 97,	"Protective",	AE_SPECIAL_AC,	0,	 2,	 2,	0,	0,	0,	0,	  3,	IA_JEWEL,	IS_ALL,	0,	0,	1,	   100,	   200,	2,	 0,	  0 },
	{ 98,	"Reliant",		AE_SPECIAL_AC,	0,	 3,	 5,	0,	0,	0,	0,	  8,	IA_JEWEL,	IS_ALL,	0,	0,	1,	   300,	   500,	3,	 2,	  3 },
	{ 99,	"Shielding",	AE_SPECIAL_AC,	0,	 6,	 9,	0,	0,	0,	0,	 16,	IA_JEWEL,	IS_ALL,	0,	0,	1,	   900,	 1'350,	4,	 4,	  6 },
	{ 100,	"Covering",		AE_SPECIAL_AC,	0,	10,	14,	0,	0,	0,	0,	 29,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 2'000,	 2'800,	5,	11,	 13 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	15,	20,	0,	0,	0,	0,	 61,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 4'500,	 6'000,	6,	28,	 33 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	21,	25,	0,	0,	0,	0,	 91,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 8'400,	10'000,	7,	54,	 60 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	26,	30,	0,	0,	0,	0,	133,	IA_JEWEL,	IS_ALL,	0,	0,	1,	13'000,	15'000,	8,	70,	 80 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	31,	35,	0,	0,	0,	0,	177,	IA_JEWEL,	IS_ALL,	0,	0,	0,	31'000,	35'000,	9,	93,	111 },


	/* ------------------------------------
		ELEMENTAL DAMAGE ON WEAPONS
	-------------------------------------*/

	{ 102,	"Warm",			AE_ELEM_HIT,	EA_MELEE_FIRE,	 1,	 2,	 3,	 4,	0,	0,	 1,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 }, 
	{ 103,	"Melting",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 3,	 4,	 6,	 7,	0,	0,	 4,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, 
	{ 104,	"Hot",			AE_ELEM_HIT,	EA_MELEE_FIRE,	 6,	 8,	12,	14,	0,	0,	10,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 105,	"Candent",		AE_ELEM_HIT,	EA_MELEE_FIRE,	11,	14,	16,	19,	0,	0,	24,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, 
	{ 106,	"Burning",		AE_ELEM_HIT,	EA_MELEE_FIRE,	12,	16,	22,	26,	0,	0,	55,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, 
	{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	24,	32,	44,	52,	0,	0,	62,	IA_STAFF|IA_AXE|IA_SWORD_2H|IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, 
	{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	24,	32,	44,	52,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	{ 108,	"Static",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 109,	"Electric",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 2,	10,	11,	0,	0,	 4,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 110,	"Charged",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 3,	 5,	19,	21,	0,	0,	10,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 111,	"Shocking",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 5,	 8,	28,	31,	0,	0,	24,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 112,	"Lightning",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 7,	11,	37,	41,	0,	0,	55,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 113,	"Thundering",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	14,	22,	74,	82,	0,	0,	62,	IA_STAFF|IA_AXE|IA_SWORD_2H|IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 113,	"Thundering",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	14,	22,	74,	82,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	{ 114,	"Chilling",	AE_ELEM_HIT,	EA_MELEE_COLD,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 115,	"Snow",		AE_ELEM_HIT,	EA_MELEE_COLD,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	{ 116,	"Icy",		AE_ELEM_HIT,	EA_MELEE_COLD,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	{ 117,	"Winter",	AE_ELEM_HIT,	EA_MELEE_COLD,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	{ 118,	"Glacial",	AE_ELEM_HIT,	EA_MELEE_COLD,	 9,	13,	35,	39,	0,	0,	55,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	24,	32,	64,	72,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	24,	32,	64,	72,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	{ 120,	"Cleansing",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF|IA_AXE|IA_MACE,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 }, // 7
	{ 121,	"Purifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF|IA_AXE|IA_MACE,	IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 24 +100%
	{ 122,	"Cleric's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF|IA_AXE|IA_MACE,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 48 + 100%
	{ 123,	"Priest's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF|IA_AXE|IA_MACE,	IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 72 + 50
	{ 124,	"Purging",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 9,	13,	35,	39,	0,	0,	55,	IA_STAFF|IA_AXE|IA_MACE,	IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 96 + 33%
	{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	24,	32,	64,	72,	0,	0,	62,	IA_STAFF|IA_AXE|IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 192
	{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	24,	32,	64,	72,	0,	0,	99,	IA_MACE_1H,					IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	{ 126,	"Voiding",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 24
	{ 127,	"Depleting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 48
	{ 128,	"Exhausting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 72
	{ 129,	"Devastating",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 9,	12,	36,	39,	0,	0,	55,	IA_STAFF|IA_MELEE|IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 96
	{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	21,	27,	53,	61,	0,	0,	62,	IA_STAFF|IA_AXE|IA_SWORD_2H|IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 162!
	{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	21,	27,	53,	61,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	{ 131,	"Rancid",			AE_ELEM_HIT,	EA_MELEE_ACID,	 2,	 3,	 7,	 8,	0,	0,	 4,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 20
	{ 132,	"Toxic",			AE_ELEM_HIT,	EA_MELEE_ACID,	 4,	 6,	15,	17,	0,	0,	10,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 42
	{ 133,	"Diluent",			AE_ELEM_HIT,	EA_MELEE_ACID,	 5,	 7,	23,	26,	0,	0,	24,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 61
	{ 134,	"Corrodent",		AE_ELEM_HIT,	EA_MELEE_ACID,	12,	15,	25,	30,	0,	0,	55,	IA_STAFF|IA_MELEE|IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 82
	{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	18,	24,	48,	56,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 146!
	{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	18,	24,	48,	56,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	
	/* ------------------------------------
				VARIOUS
	-------------------------------------*/
	
	{ 136,	"Draining",	AE_FLAG,	EA_LIFE_STEAL_RND,	0,	0,	0,	0,	0,	0,	6,	IA_STAFF | IA_MELEE|IA_CLAW,	IS_BATTLE,	0,	0,	0,	800,	800,	7,	4,	6 },
	
	{ 58,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	12,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_FLASK | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	1'000,	1'000,	 5,	 5,	 7 },
	{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	35,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_FLASK | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	3'000,	3'000,	 7,	17,	19 },
	{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	68,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_FLASK | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	6'000,	6'000,	11,	27,	33 },
	{ 137,	"Dreadful",	AE_FLAG,	EA_RANDOM_DAMAGE,	0,	0,	0,	0,	0,	0,	90,	IA_JEWEL | IA_STAFF | IA_MELEE | IA_TRAP | IA_FLASK | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	9'000,	9'000,	13,	51,	58 },
	
	{ 180,	"Crafted",			AE_HIGH_DURABILITY,	0,	105,	210,	0,	0,	0,	0,	 1,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP | IA_PISTOL,	IS_ALL,	0,	0,	1,	    50,	   100,	1,	 0,	 0 },
	{ 181,	"Reliable",			AE_HIGH_DURABILITY,	0,	100,	200,	0,	0,	0,	0,	 3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP | IA_PISTOL,	IS_ALL,	0,	0,	1,	   200,	   300,	2,	 0,	 0 },
	{ 181,	"Structured",		AE_HIGH_DURABILITY,	0,	 95,	190,	0,	0,	0,	0,	 7,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP | IA_PISTOL,	IS_ALL,	0,	0,	1,	   500,	 1'000,	2,	 0,	 0 },
	{ 181,	"Tempered",			AE_HIGH_DURABILITY,	0,	 90,	180,	0,	0,	0,	0,	13,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP | IA_PISTOL,	IS_ALL,	0,	0,	1,	 2'000,	 4'000,	3,	 3,	 4 },
	//{ 181,	"Indurated",		AE_HIGH_DURABILITY,	0,	 85,	170,	0,	0,	0,	0,	26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	 5'000,	 7'000,	3,	16,	20 },
	//{ 181,	"Strengthened",		AE_HIGH_DURABILITY,	0,	 80,	160,	0,	0,	0,	0,	52,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	 8'000,	12'000,	4,	30,	34 },
	//{ 181,	"Masterwork",		AE_HIGH_DURABILITY,	0,	 75,	150,	0,	0,	0,	0,	87,	IA_RANGE | IA_STAFF | IA_MELEE | IA_SHIELD | IA_ARMOR | IA_CLAW | IA_KNIFE | IA_MALLET | IA_TRAP,	IS_ALL,	0,	0,	1,	16'000,	24'000,	6,	50,	55 },

	
	/* ------------------------------------
				XP PERCENT
	-------------------------------------*/
	
	//{ 138,	"Curious",	AE_XP_GAIN_PERCENT,	0,	2,	 3,	 0,	0,	0,	0,	1,	IA_JEWEL|IA_STAFF|IA_ARMOR|IA_FLASK|IA_TRAP|IA_KNIFE | IA_MALLET,	IS_ALL,	0,	0,	1,	100,	150,	1,	0,	0+2 },
	{ 139,	"Curious",			AE_XP_GAIN_PERCENT,	0,	 6,	 7,	0,	0,	0,	0,	  7,	IA_JEWEL,	IS_ALL,	0,	0,	1,	   300,	   400,	2,	 0,	 0 },
	{ 140,	"Studying",			AE_XP_GAIN_PERCENT,	0,	 8,	 9,	0,	0,	0,	0,	 13,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 1'000,	 1'200,	3,	 3,	 4 },
	{ 141,	"Illuminating",		AE_XP_GAIN_PERCENT,	0,	10,	11,	0,	0,	0,	0,	 25,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 3'500,	 4'000,	4,	15,	16 },
	{ 141,	"Contemplating",	AE_XP_GAIN_PERCENT,	0,	12,	13,	0,	0,	0,	0,	 48,	IA_JEWEL,	IS_ALL,	0,	0,	1,	18'000,	20'000,	5,	33,	36 },
	{ 141,	"Clarifying",		AE_XP_GAIN_PERCENT,	0,	14,	15,	0,	0,	0,	0,	 70,	IA_JEWEL,	IS_ALL,	0,	0,	0,	22'000,	24'000,	5,	41,	46 },
	{ 141,	"Clarifying",		AE_XP_GAIN_PERCENT,	0,	16,	17,	0,	0,	0,	0,	112,	IA_JEWEL,	IS_ALL,	0,	0,	0,	28'000,	30'000,	6,	56,	60 },


	/* ------------------------------------
				GOLD FIND PERCENT
	-------------------------------------*/

	{ 142,	"Rapacious",	AE_GOLD_FIND,	0,	10,	20,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,	0,	0,	1,	   600,	 1'200,	2,	 2,	 3 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	21,	25,	0,	0,	0,	0,	 24,	IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,	0,	0,	1,	 4'200,	 5'000,	3,	15,	18 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	25,	30,	0,	0,	0,	0,	 46,	IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,	0,	0,	1,	10'000,	12'000,	4,	30,	35 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	31,	35,	0,	0,	0,	0,	 91,	IA_JEWEL,							IS_ALL,	0,	0,	0,	31'000,	35'000,	8,	50,	56 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	36,	40,	0,	0,	0,	0,	121,	IA_JEWEL,							IS_ALL,	0,	0,	0,	41'000,	45'000,	9,	62,	64 },

	
	/* ------------------------------------
		MELEE & ARROW DAMAGE RESISTANCE
	-------------------------------------*/
	
		//Melee
	{ 144,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	  0,	  0 },
	{ 145,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 3,	 4,	0,	0,	0,	0,	  4,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   150,	   300,	 2,	  0,	  0 },
	{ 145,	"Negating",		AE_ABSORB_MELEE_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 10,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   500,	   600,	 3,	  2,	  4 },
	{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	 7,	 8,	0,	0,	0,	0,	 19,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	 1'000,	 1'200,	 4,	  5,	  7 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 9,	12,	0,	0,	0,	0,	 27,	IA_BODY_ARMOR | IA_HELM,		IS_ALL,		0,	0,	1,	 4'000,	 4'800,	 6,	 14,	 16 },
	//shields
	{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	 8,	 9,	0,	0,	0,	0,	 38,	IA_SHIELD,						IS_ALL,		0,	0,	1,	 4'000,	 5'200,	 5,	 20,	 25 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	 9,	10,	0,	0,	0,	0,	 73,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	 6,	 45,	 46 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 93,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	11'000,	12'000,	 7,	 60,	 61 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	13,	14,	0,	0,	0,	0,	113,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	13'000,	14'000,	 8,	 75,	 77 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	15,	16,	0,	0,	0,	0,	133,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	15'000,	16'000,	 9,	 83,	 85 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	17,	18,	0,	0,	0,	0,	153,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	17'000,	18'000,	10,	 90,	 93 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	19,	20,	0,	0,	0,	0,	174,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	19'000,	20'000,	11,	103,	105 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	178,	IA_SHIELD,						IS_BATTLE,	0,	0,	0,	21'000,	25'000,	13,	109,	125 },
	//helms
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	10,	13,	0,	0,	0,	0,	 50,	IA_HELM,						IS_ALL,		0,	0,	1,	10'000,	13'000,	 7,	 27,	 31 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	13,	15,	0,	0,	0,	0,	 75,	IA_HELM,						IS_ALL,		0,	0,	1,	13'000,	15'000,	 7,	 46,	 48 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	16,	18,	0,	0,	0,	0,	 95,	IA_HELM,						IS_ALL,		0,	0,	1,	16'000,	18'000,	 8,	 61,	 63 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	19,	21,	0,	0,	0,	0,	115,	IA_HELM,						IS_BATTLE,	0,	0,	1,	19'000,	21'000,	 9,	 76,	 78 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	22,	24,	0,	0,	0,	0,	135,	IA_HELM,						IS_BATTLE,	0,	0,	1,	22'000,	24'000,	10,	 84,	 87 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	25,	27,	0,	0,	0,	0,	155,	IA_HELM,						IS_BATTLE,	0,	0,	1,	25'000,	27'000,	11,	 91,	 94 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	28,	30,	0,	0,	0,	0,	176,	IA_HELM,						IS_BATTLE,	0,	0,	1,	28'000,	30'000,	12,	104,	107 },
	{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	178,	IA_HELM,						IS_BATTLE,	0,	0,	0,	31'000,	35'000,	14,	108,	126 },
	//boots/gloves/belt
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	 9,	10,	0,	0,	0,	0,	 38,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 5'000,	 6'000,	 6,	 28,	 31 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	10,	11,	0,	0,	0,	0,	 59,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 8'000,	 9'000,	 6,	 33,	 38 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 80,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	 47,	 49 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	13,	14,	0,	0,	0,	0,	100,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	13'000,	14'000,	 9,	 62,	 65 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	15,	16,	0,	0,	0,	0,	120,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	15'000,	16'000,	10,	 77,	 80 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	17,	19,	0,	0,	0,	0,	140,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	17'000,	19'000,	11,	 85,	 89 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	20,	22,	0,	0,	0,	0,	160,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	20'000,	22'000,	12,	 93,	 97 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	23,	25,	0,	0,	0,	0,	175,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	23'000,	25'000,	13,	106,	109 },
	{ 148,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	178,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	0,	26'000,	30'000,	15,	111,	129 },
		//harness
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	13,	16,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	10'500,	12'000,	 8,	 33,	 35 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	17,	20,	0,	0,	0,	0,	 62,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	17'500,	20'000,	 8,	 40,	 43 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	 85,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	21'000,	25'000,	 9,	 48,	 52 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	105,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	26'000,	30'000,	10,	 64,	 67 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	125,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	31'000,	36'000,	11,	 79,	 82 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	36,	40,	0,	0,	0,	0,	145,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	36'000,	40'000,	12,	 87,	 90 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	41,	45,	0,	0,	0,	0,	165,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	41'000,	45'000,	13,	 95,	 99 },
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	46,	50,	0,	0,	0,	0,	175,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	46'000,	50'000,	14,	108,	113 },		
	{ 149,	"Devouring",	AE_ABSORB_MELEE_DAMAGE,	0,	51,	60,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	0,	51'000,	60'000,	14,	116,	130 },

		// Arrow
	{ 144,	"Negating",		AE_ABSORB_RANGED_DAMAGE,	0,	 1,	 2,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	  0,	  0 },
	{ 145,	"Negating",		AE_ABSORB_RANGED_DAMAGE,	0,	 3,	 4,	0,	0,	0,	0,	  4,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   150,	   300,	 2,	  0,	  0 },
	{ 145,	"Negating",		AE_ABSORB_RANGED_DAMAGE,	0,	 5,	 6,	0,	0,	0,	0,	 10,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	   500,	   600,	 3,	  2,	  4 },
	{ 146,	"Vindicative",	AE_ABSORB_RANGED_DAMAGE,	0,	 7,	 8,	0,	0,	0,	0,	 19,	IA_SHIELD | IA_ARMOR,			IS_ALL,		0,	0,	1,	 1'000,	 1'200,	 4,	  5,	  7 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 9,	12,	0,	0,	0,	0,	 27,	IA_BODY_ARMOR | IA_HELM,		IS_ALL,		0,	0,	1,	 4'000,	 4'800,	 6,	 14,	 16 },
	//shields
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 8,	 9,	0,	0,	0,	0,	 38,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	 4'000,	 5'000,	 5,	 20,	 24 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	 9,	10,	0,	0,	0,	0,	 73,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	 6,	 45,	 46 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 93,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	11'000,	12'000,	 7,	 60,	 61 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	13,	14,	0,	0,	0,	0,	113,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	13'000,	14'000,	 8,	 75,	 77 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	15,	16,	0,	0,	0,	0,	134,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	15'000,	16'000,	 9,	 83,	 85 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	17,	18,	0,	0,	0,	0,	153,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	17'000,	18'000,	10,	 90,	 93 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	19,	20,	0,	0,	0,	0,	173,	IA_SHIELD,						IS_BATTLE,	0,	0,	1,	19'000,	20'000,	11,	103,	105 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	178,	IA_SHIELD,						IS_BATTLE,	0,	0,	0,	21'000,	25'000,	13,	109,	122 },
	//helms
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	10,	13,	0,	0,	0,	0,	 50,	IA_HELM,						IS_ALL,		0,	0,	1,	10'000,	13'000,	 7,	 27,	 31 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	13,	15,	0,	0,	0,	0,	 75,	IA_HELM,						IS_ALL,		0,	0,	1,	13'000,	15'000,	 7,	 46,	 48 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	16,	18,	0,	0,	0,	0,	 95,	IA_HELM,						IS_ALL,		0,	0,	1,	16'000,	18'000,	 8,	 61,	 63 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	19,	21,	0,	0,	0,	0,	115,	IA_HELM,						IS_BATTLE,	0,	0,	1,	19'000,	21'000,	 9,	 76,	 78 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	22,	24,	0,	0,	0,	0,	136,	IA_HELM,						IS_BATTLE,	0,	0,	1,	22'000,	24'000,	10,	 84,	 87 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	25,	27,	0,	0,	0,	0,	155,	IA_HELM,						IS_BATTLE,	0,	0,	1,	25'000,	27'000,	11,	 91,	 94 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	28,	30,	0,	0,	0,	0,	175,	IA_HELM,						IS_BATTLE,	0,	0,	1,	28'000,	30'000,	12,	104,	107 },
	{ 147,	"Consuming",	AE_ABSORB_RANGED_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	178,	IA_HELM,						IS_BATTLE,	0,	0,	0,	31'000,	35'000,	14,	108,	124 },
	//boots/gloves/belt
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	 9,	10,	0,	0,	0,	0,	 38,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 5'000,	 6'000,	 6,	 28,	 31 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	10,	11,	0,	0,	0,	0,	 59,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	 8'000,	 9'000,	 6,	 33,	 38 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	11,	12,	0,	0,	0,	0,	 80,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	 47,	 49 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	13,	14,	0,	0,	0,	0,	101,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_ALL,		0,	0,	1,	13'000,	14'000,	 9,	 62,	 65 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	15,	16,	0,	0,	0,	0,	120,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	15'000,	16'000,	10,	 77,	 80 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	17,	19,	0,	0,	0,	0,	141,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	17'000,	19'000,	11,	 85,	 89 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	20,	22,	0,	0,	0,	0,	160,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	20'000,	22'000,	12,	 93,	 97 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	23,	25,	0,	0,	0,	0,	175,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	1,	23'000,	25'000,	13,	106,	109 },
	{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	178,	IA_BOOTS | IA_GLOVE | IA_BELT,	IS_BATTLE,	0,	0,	0,	26'000,	30'000,	15,	111,	124 },
	//harness
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	13,	16,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	10'500,	12'000,	 8,	 33,	 35 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	17,	20,	0,	0,	0,	0,	 62,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	17'500,	20'000,	 8,	 40,	 43 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	21,	25,	0,	0,	0,	0,	 84,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	21'000,	25'000,	 9,	 48,	 52 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	26,	30,	0,	0,	0,	0,	105,	IA_BODY_ARMOR,					IS_ALL,		0,	0,	1,	26'000,	30'000,	10,	 64,	 67 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	31,	35,	0,	0,	0,	0,	125,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	31'000,	36'000,	11,	 79,	 82 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	36,	40,	0,	0,	0,	0,	145,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	36'000,	40'000,	12,	 87,	 90 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	41,	45,	0,	0,	0,	0,	164,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	41'000,	45'000,	13,	 95,	 99 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	46,	50,	0,	0,	0,	0,	175,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	46'000,	50'000,	14,	108,	113 },
	{ 149,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	51,	60,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	0,	51'000,	60'000,	14,	116,	128 },

	
	/* ------------------------------------
				SUMMON BOOSTS
	-------------------------------------*/
	
	{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 1,	 3,	 1,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	    40,	   120,	1,	 0,	 0 },
	{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 3,	 4,	 3,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   150,	   200,	1,	 0,	 0 },
	{ 157,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 5,	 8,	 7,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   500,	   800,	2,	 3,	 5 },
	{ 158,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 9,	14,	13,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 1'800,	 2'800,	5,	 6,	 8 },
	{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	25,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 7'500,	10'000,	6,	13,	17 },
	{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	47,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	16'500,	17'000,	7,	23,	28 },
	{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	93,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	22'500,	25'000,	8,	49,	55 },

	{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 1,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
	{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 3,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
	{ 161,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	 7,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
	{ 162,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	12,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 2'000,	 3'500,	4,	 6,	 8 },
	{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	25,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 7'000,	 8'500,	6,	13,	17 },
	{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	47,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	14'000,	15'500,	7,	23,	28 },
	{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	93,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	26'000,	27'500,	8,	51,	54 },

	{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 1,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
	{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 3,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
	{ 165,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
	{ 166,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	12,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 2'000,	 2'500,	4,	 6,	 8 },
	{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	25,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	6,	13,	17 },
	{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	47,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	11'000,	13'500,	7,	23,	28 },
	{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	93,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	18'000,	19'500,	8,	51,	55 },

//could be used on uniques	{ 0,	"Summon AC",	AE_SUMMON_AC_PERCENT,	0,	5,	20,	0,	0,	0,	0,	25,	IA_ARMOR,	IS_ALL,	0,	0,	1,	100,	100,	5,	5+2 },


	/* ------------------------------------
			+ ARMOR VS SPECIES
	-------------------------------------*/

	{ 168,	"Shimmering",	AE_SPICIES_AC,	MON_0_UNDEAD,	11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 169,	"Shining",		AE_SPICIES_AC,	MON_0_UNDEAD,	16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 170,	"Coruscating",	AE_SPICIES_AC,	MON_0_UNDEAD,	26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 171,	"Radiant",		AE_SPICIES_AC,	MON_0_UNDEAD,	36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 171,	"Luminous",		AE_SPICIES_AC,	MON_0_UNDEAD,	46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },

	{ 172,	"Blessed",		AE_SPICIES_AC,	MON_1_DEMON,		11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 173,	"Saintly",		AE_SPICIES_AC,	MON_1_DEMON,		16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 174,	"Divine",		AE_SPICIES_AC,	MON_1_DEMON,		26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 175,	"Godly",		AE_SPICIES_AC,	MON_1_DEMON,		36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 175,	"Sanctified",	AE_SPICIES_AC,	MON_1_DEMON,		46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },

	{ 176,	"Fine",			AE_SPICIES_AC,	MON_2_BEAST,		11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	{ 177,	"Grand",		AE_SPICIES_AC,	MON_2_BEAST,		16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	{ 178,	"Valiant",		AE_SPICIES_AC,	MON_2_BEAST,		26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	{ 179,	"Glorious",		AE_SPICIES_AC,	MON_2_BEAST,		36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	{ 179,	"Awesome",		AE_SPICIES_AC,	MON_2_BEAST,		46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },


	/* ------------------------------------
				THORNS PERCENT
	-------------------------------------*/

	{ 182,	"Thorned",	AE_THORNS_PERCENT,	0,	 31,	 40,	0,	0,	0,	0,	  2,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	{ 183,	"Thorned",	AE_THORNS_PERCENT,	0,	 41,	 50,	0,	0,	0,	0,	  6,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   300,	   300,	 2,	  2,	  3 },
	{ 184,	"Thorned",	AE_THORNS_PERCENT,	0,	 51,	 60,	0,	0,	0,	0,	 12,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   800,	   800,	 3,	  4,	  5 },
	{ 185,	"Thorned",	AE_THORNS_PERCENT,	0,	 61,	 70,	0,	0,	0,	0,	 24,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'500,	 1'500,	 4,	 14,	 16 },
	{ 186,	"Thorned",	AE_THORNS_PERCENT,	0,	 71,	 80,	0,	0,	0,	0,	 32,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'100,	 3'100,	 5,	 20,	 22 },
	{ 187,	"Thorned",	AE_THORNS_PERCENT,	0,	 81,	 90,	0,	0,	0,	0,	 48,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 5'100,	 5'100,	 6,	 26,	 28 },
	{ 188,	"Thorned",	AE_THORNS_PERCENT,	0,	 91,	100,	0,	0,	0,	0,	 56,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 7'100,	 7'100,	 7,	 32,	 34 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	101,	110,	0,	0,	0,	0,	 62,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	11'100,	11'100,	 8,	 38,	 40 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	111,	120,	0,	0,	0,	0,	 98,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	24'200,	28'000,	 9,	 42,	 46 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	121,	130,	0,	0,	0,	0,	112,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	29'500,	33'000,	10,	 48,	 53 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	131,	140,	0,	0,	0,	0,	130,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	34'500,	38'000,	11,	 55,	 61 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	141,	150,	0,	0,	0,	0,	150,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	39'500,	43'000,	12,	 63,	 70 },
	{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	151,	160,	0,	0,	0,	0,	178,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	0,	56'500,	60'000,	16,	100,	111 },

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
	
	{ 190,	"Hurting",		AE_DAMAGE,	0,	  1,	  1,	0,	0,	0,	0,	  1,	IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,					IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 191,	"Wounding",		AE_DAMAGE,	0,	  2,	  3,	0,	0,	0,	0,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   200,	   300,	1,	 0,  0 },
	{ 192,	"Mess",			AE_DAMAGE,	0,	  4,	  5,	0,	0,	0,	0,	  5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   400,	   800,	2,	 2,	 3 },
	{ 193,	"Maiming",		AE_DAMAGE,	0,	  6,	  8,	0,	0,	0,	0,	  8,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   900,	 1'500,	3,	 4,	 6 },
	{ 194,	"Mangling",		AE_DAMAGE,	0,	  9,	 11,	0,	0,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 1'600,	 2'200,	3,	 7,	 9 },
	{ 195,	"Atrocity",		AE_DAMAGE,	0,	 12,	 15,	0,	0,	0,	0,	 14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 2'300,	 3'000,	4,	11,	13 },
	{ 196,	"Mayhem",		AE_DAMAGE,	0,	 16,	 20,	0,	0,	0,	0,	 20,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 3'100,	 4'000,	5,	15,	17 },
	{ 197,	"Slaying",		AE_DAMAGE,	0,	 21,	 30,	0,	0,	0,	0,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 4'100,	 5'000,	6,	19,	21 },
	{ 198,	"Gore",			AE_DAMAGE,	0,	 31,	 40,	0,	0,	0,	0,	 32,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 5'100,	 6'500,	6,	23,	26 },
	{ 199,	"Carnage",		AE_DAMAGE,	0,	 41,	 50,	0,	0,	0,	0,	 37,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 6'600,	 8'000,	7,	26,	28 },
	//1H
	{ 200,	"Slaughter",	AE_DAMAGE,	0,	 51,	 60,	0,	0,	0,	0,	 42,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	 8'100,	10'000,	 7,	 29,	 32 },
	{ 201,	"Hatred",		AE_DAMAGE,	0,	 61,	 70,	0,	0,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	10'200,	12'000,	 8,	 33,	 34 },
	{ 202,	"Massacre",		AE_DAMAGE,	0,	 71,	 85,	0,	0,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	12'600,	15'000,	 8,	 35,	 37 },
	{ 203,	"Death",		AE_DAMAGE,	0,	 86,	100,	0,	0,	0,	0,	 70,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	17'200,	20'000,	 9,	 39,	 43 },
	{ 203,	"Mutilation",	AE_DAMAGE,	0,	 101,	120,	0,	0,	0,	0,	 90,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	20'200,	24'000,	10,	 48,	 55 },
	{ 203,	"Torture",		AE_DAMAGE,	0,	 121,	140,	0,	0,	0,	0,	110,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	27'225,	31'500,	11,	 60,	 70 },
	{ 203,	"Demise",		AE_DAMAGE,	0,	 141,	160,	0,	0,	0,	0,	130,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	35'250,	40'000,	12,	 75,	 85 },
	{ 203,	"Butchery",		AE_DAMAGE,	0,	 161,	180,	0,	0,	0,	0,	155,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	1,	44'275,	49'500,	13,	 90,	100 },
	{ 203,	"Execution",	AE_DAMAGE,	0,	 181,	200,	0,	0,	0,	0,	178,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW | IA_KNIFE | IA_MALLET,	IS_BATTLE,	0,	0,	0,	54'300,	60'000,	15,	101,	120 },
	//2H
	{ 204,	"Slaughter",	AE_DAMAGE,	0,	 51,	 80,	0,	0,	0,	0,	 42,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	 8'100,	10'000,	 7,	 29,	 32 },
	{ 205,	"Hatred",		AE_DAMAGE,	0,	 81,	115,	0,	0,	0,	0,	 46,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	10'200,	12'000,	 8,	 33,	 34 },
	{ 206,	"Massacre",		AE_DAMAGE,	0,	116,	155,	0,	0,	0,	0,	 50,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	12'600,	15'000,	 8,	 35,	 37 },
	{ 203,	"Death",		AE_DAMAGE,	0,	156,	200,	0,	0,	0,	0,	 70,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	17'200,	20'000,	 9,	 39,	 43 },
	{ 203,	"Mutilation",	AE_DAMAGE,	0,	201,	240,	0,	0,	0,	0,	 90,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	20'200,	24'000,	10,	 48,	 55 },
	{ 203,	"Torture",		AE_DAMAGE,	0,	241,	280,	0,	0,	0,	0,	111,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	27'225,	31'500,	11,	 60,	 70 },
	{ 203,	"Demise",		AE_DAMAGE,	0,	281,	320,	0,	0,	0,	0,	130,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	35'250,	40'000,	12,	 75,	 85 },
	{ 203,	"Butchery",		AE_DAMAGE,	0,	321,	360,	0,	0,	0,	0,	155,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	1,	44'275,	49'500,	13,	 90,	100 },
	{ 203,	"Execution",	AE_DAMAGE,	0,	361,	400,	0,	0,	0,	0,	178,	IA_RANGE | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,							IS_BATTLE,	0,	0,	0,	54'300,	60'000,	15,	101,	120 },


	/* ------------------------------------
			+ DAMAGE VS SPECIES
	-------------------------------------*/

		//undead
	{ 208,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 10,	 12,	 14,	 16,	0,	0,	  8,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
	{ 209,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 16,	 20,	 24,	 28,	0,	0,	 14,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
	{ 210,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 28,	 32,	 36,	 40,	0,	0,	 24,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
	{ 211,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 40,	 50,	 60,	 70,	0,	0,	 37,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
	{ 212,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	 70,	 80,	 90,	100,	0,	0,	 42,	IA_STAFF | IA_AXE | IA_MACE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
	//1h
	{ 213,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	110,	120,	130,	140,	0,	0,	 46,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 214,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	140,	150,	160,	170,	0,	0,	 50,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	170,	180,	190,	200,	0,	0,	 57,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	201,	210,	220,	230,	0,	0,	 75,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	231,	240,	250,	260,	0,	0,	 95,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	261,	275,	285,	300,	0,	0,	115,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	301,	315,	330,	350,	0,	0,	135,	IA_STAFF | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 215,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	351,	370,	380,	400,	0,	0,	155,	IA_STAFF | IA_MACE_1H,				IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
	//2h
	{ 216,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	125,	150,	175,	200,	0,	0,	 46,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 217,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	225,	250,	275,	300,	0,	0,	 50,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	325,	350,	375,	400,	0,	0,	 57,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	401,	420,	440,	460,	0,	0,	 75,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	461,	485,	510,	535,	0,	0,	 95,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	536,	560,	580,	600,	0,	0,	115,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	601,	633,	666,	700,	0,	0,	138,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 218,	"Wrecking",		AE_SPICIES_DAMAGE,	MON_0_UNDEAD,	701,	733,	766,	800,	0,	0,	158,	IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },

		//demons
	{ 219,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 10,	 12,	 14,	 16,	0,	0,	  8,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
	{ 220,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 16,	 20,	 24,	 28,	0,	0,	 14,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
	{ 221,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 28,	 32,	 36,	 40,	0,	0,	 24,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
	{ 222,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 40,	 50,	 60,	 70,	0,	0,	 37,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
	{ 223,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		 70,	 80,	 90,	100,	0,	0,	 42,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
	//1h
	{ 224,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		110,	120,	130,	140,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 225,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		140,	150,	160,	170,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 226,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		170,	180,	190,	200,	0,	0,	 57,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 226,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		201,	210,	220,	230,	0,	0,	 75,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 215,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		231,	240,	250,	260,	0,	0,	 95,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 215,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		261,	275,	285,	300,	0,	0,	115,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 215,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		301,	315,	330,	350,	0,	0,	135,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 215,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		351,	370,	380,	400,	0,	0,	154,	IA_STAFF | IA_SWORD_1H | IA_MACE_1H,			IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
	//2h
	{ 227,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		125,	150,	175,	200,	0,	0,	 46,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 228,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		225,	250,	275,	300,	0,	0,	 50,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		325,	350,	375,	400,	0,	0,	 57,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		401,	420,	440,	460,	0,	0,	 75,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		461,	485,	510,	535,	0,	0,	 95,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		536,	560,	580,	600,	0,	0,	115,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		601,	633,	666,	700,	0,	0,	135,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 229,	"Banishment",	AE_SPICIES_DAMAGE,	MON_1_DEMON,		701,	733,	766,	800,	0,	0,	156,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },

		//beasts
	{ 230,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 10,	 12,	 14,	 16,	0,	0,	  8,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  100,	  100,	2,	 0,	 0 },
	{ 231,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 16,	 20,	 24,	 28,	0,	0,	 14,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	  800,	  800,	3,	 3,	 5 },
	{ 232,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 28,	 32,	 36,	 40,	0,	0,	 24,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	1'700,	1'700,	4,	14,	17 },
	{ 233,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 40,	 50,	 60,	 70,	0,	0,	 37,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	3'000,	3'000,	5,	23,	27 },
	{ 234,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		 70,	 80,	 90,	100,	0,	0,	 42,	IA_STAFF | IA_SWORD | IA_AXE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	4'500,	4'500,	6,	28,	30 },
	//1h
	{ 235,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		110,	120,	130,	140,	0,	0,	 46,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 236,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		140,	150,	160,	170,	0,	0,	 50,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		170,	180,	190,	200,	0,	0,	 57,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		201,	210,	220,	230,	0,	0,	 75,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		231,	240,	250,	260,	0,	0,	 95,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		261,	275,	285,	300,	0,	0,	115,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		301,	315,	330,	350,	0,	0,	137,	IA_STAFF | IA_SWORD_1H | IA_CLAW,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 237,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		351,	370,	380,	400,	0,	0,	157,	IA_STAFF | IA_SWORD_1H,				IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },
	//2h
	{ 238,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		125,	150,	175,	200,	0,	0,	 46,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	 6'500,	 6'500,	 7,	31,	35 },
	{ 239,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		225,	250,	275,	300,	0,	0,	 50,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	36,	38 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		325,	350,	375,	400,	0,	0,	 57,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	10,	39,	42 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		401,	420,	440,	460,	0,	0,	 75,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	18'000,	18'000,	11,	44,	47 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		461,	485,	510,	535,	0,	0,	 95,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	24'000,	24'000,	12,	51,	56 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		536,	560,	580,	600,	0,	0,	115,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	1,	31'000,	31'000,	13,	60,	66 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		601,	633,	666,	700,	0,	0,	135,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	0,	39'000,	39'000,	14,	72,	76 },
	{ 240,	"Bloodshed",	AE_SPICIES_DAMAGE,	MON_2_BEAST,		701,	733,	766,	800,	0,	0,	158,	IA_AXE | IA_SWORD_2H,	IS_BATTLE,	0,	0,	0,	48'000,	48'000,	15,	85,	93 },

	
	/* ------------------------------------
					DFE
	-------------------------------------*/

		//shields/belts/boots/gloves
	{ 241,	"Protection",	AE_DFE,	0,	- 1,	- 1,	0,	0,	0,	0,	  3,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	   200,	   200,	 1,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 2,	- 2,	0,	0,	0,	0,	  7,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 3,	- 3,	0,	0,	0,	0,	 12,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 2'000,	 2'000,	 3,	  3,	  4 },
	{ 241,	"Protection",	AE_DFE,	0,	- 4,	- 4,	0,	0,	0,	0,	 18,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 3'000,	 3'000,	 4,	 12,	 15 },
	{ 241,	"Protection",	AE_DFE,	0,	- 5,	- 5,	0,	0,	0,	0,	 25,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 4'500,	 4'500,	 5,	 16,	 18 },
	{ 241,	"Protection",	AE_DFE,	0,	- 6,	- 6,	0,	0,	0,	0,	 35,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 6'500,	 6'500,	 6,	 22,	 25 },
	{ 241,	"Protection",	AE_DFE,	0,	- 7,	- 7,	0,	0,	0,	0,	 45,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	 9'500,	 9'500,	 7,	 26,	 31 },
	{ 241,	"Protection",	AE_DFE,	0,	- 8,	- 8,	0,	0,	0,	0,	 55,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	13'500,	13'500,	 8,	 33,	 36 },
	{ 241,	"Protection",	AE_DFE,	0,	- 9,	- 9,	0,	0,	0,	0,	 65,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	18'500,	18'500,	 9,	 37,	 40 },
	{ 241,	"Protection",	AE_DFE,	0,	-10,	-10,	0,	0,	0,	0,	 75,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_ALL,		0,	0,	1,	21'500,	21'500,	10,	 45,	 47 },
	{ 241,	"Protection",	AE_DFE,	0,	-11,	-12,	0,	0,	0,	0,	 90,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	22'000,	24'000,	11,	 48,	 52 },
	{ 241,	"Protection",	AE_DFE,	0,	-13,	-14,	0,	0,	0,	0,	105,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	26'000,	28'000,	12,	 55,	 60 },
	{ 241,	"Protection",	AE_DFE,	0,	-15,	-16,	0,	0,	0,	0,	125,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	30'000,	32'000,	13,	 65,	 70 },
	{ 241,	"Protection",	AE_DFE,	0,	-17,	-18,	0,	0,	0,	0,	150,	IA_SHIELD | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	34'000,	36'000,	14,	 75,	 80 },
	{ 241,	"Protection",	AE_DFE,	0,	-19,	-20,	0,	0,	0,	0,	180,	IA_SHIELD,									IS_BATTLE,	0,	0,	1,	38'000,	40'000,	15,	 85,	 95 },
	{ 241,	"Protection",	AE_DFE,	0,	-19,	-20,	0,	0,	0,	0,	178,	IA_BELT | IA_BOOTS | IA_GLOVE,				IS_BATTLE,	0,	0,	0,	41'000,	47'000,	16,	 99,	115 },
	{ 241,	"Protection",	AE_DFE,	0,	-21,	-25,	0,	0,	0,	0,	178,	IA_SHIELD,									IS_BATTLE,	0,	0,	0,	42'000,	50'000,	16,	105,	120 },

		//harness
	{ 241,	"Protection",	AE_DFE,	0,	- 1,	- 1,	0,	0,	0,	0,	  2,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	   250,	   250,	 1,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 2,	- 2,	0,	0,	0,	0,	  6,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 3,	- 4,	0,	0,	0,	0,	 11,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 2'000,	 2'500,	 3,	  3,	  4 },
	{ 241,	"Protection",	AE_DFE,	0,	- 5,	- 7,	0,	0,	0,	0,	 17,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 3'000,	 3'500,	 4,	 12,	 15 },
	{ 241,	"Protection",	AE_DFE,	0,	- 8,	-10,	0,	0,	0,	0,	 24,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 4'000,	 4'500,	 5,	 16,	 18 },
	{ 241,	"Protection",	AE_DFE,	0,	-11,	-13,	0,	0,	0,	0,	 34,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 5'500,	 7'500,	 6,	 22,	 25 },
	{ 241,	"Protection",	AE_DFE,	0,	-14,	-16,	0,	0,	0,	0,	 44,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	 8'500,	10'500,	 7,	 26,	 31 },
	{ 241,	"Protection",	AE_DFE,	0,	-17,	-19,	0,	0,	0,	0,	 54,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	12'500,	14'500,	 8,	 33,	 36 },
	{ 241,	"Protection",	AE_DFE,	0,	-20,	-22,	0,	0,	0,	0,	 64,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	18'500,	20'500,	 9,	 37,	 40 },
	{ 241,	"Protection",	AE_DFE,	0,	-23,	-25,	0,	0,	0,	0,	 74,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	23'500,	25'500,	10,	 45,	 47 },
	{ 241,	"Protection",	AE_DFE,	0,	-26,	-28,	0,	0,	0,	0,	 89,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	26'000,	28'000,	11,	 48,	 52 },
	{ 241,	"Protection",	AE_DFE,	0,	-29,	-31,	0,	0,	0,	0,	104,	IA_BODY_ARMOR,	IS_ALL,		0,	0,	1,	29'000,	31'000,	12,	 55,	 60 },
	{ 241,	"Protection",	AE_DFE,	0,	-32,	-34,	0,	0,	0,	0,	124,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	32'000,	34'000,	13,	 65,	 73 },
	{ 241,	"Protection",	AE_DFE,	0,	-35,	-37,	0,	0,	0,	0,	148,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	35'000,	37'000,	14,	 80,	 90 },
	{ 241,	"Protection",	AE_DFE,	0,	-38,	-40,	0,	0,	0,	0,	171,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	1,	38'000,	40'000,	15,	 95,	104 },
	{ 241,	"Protection",	AE_DFE,	0,	-41,	-45,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,	IS_BATTLE,	0,	0,	0,	56'000,	60'000,	16,	105,	120 },
		
		//helm
	{ 241,	"Protection",	AE_DFE,	0,	- 1,	- 1,	0,	0,	0,	0,	  2,	IA_HELM,	IS_ALL,		0,	0,	1,	   250,	   250,	 1,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 2,	- 2,	0,	0,	0,	0,	  6,	IA_HELM,	IS_ALL,		0,	0,	1,	 1'000,	 1'000,	 2,	  0,	  0 },
	{ 241,	"Protection",	AE_DFE,	0,	- 3,	- 3,	0,	0,	0,	0,	 11,	IA_HELM,	IS_ALL,		0,	0,	1,	 2'000,	 2'000,	 3,	  3,	  4 },
	{ 241,	"Protection",	AE_DFE,	0,	- 4,	- 5,	0,	0,	0,	0,	 17,	IA_HELM,	IS_ALL,		0,	0,	1,	 3'000,	 3'500,	 4,	 12,	 15 },
	{ 241,	"Protection",	AE_DFE,	0,	- 6,	- 7,	0,	0,	0,	0,	 24,	IA_HELM,	IS_ALL,		0,	0,	1,	 4'000,	 4'500,	 5,	 16,	 18 },
	{ 241,	"Protection",	AE_DFE,	0,	- 8,	- 9,	0,	0,	0,	0,	 34,	IA_HELM,	IS_ALL,		0,	0,	1,	 5'500,	 6'500,	 6,	 22,	 25 },
	{ 241,	"Protection",	AE_DFE,	0,	-10,	-11,	0,	0,	0,	0,	 44,	IA_HELM,	IS_ALL,		0,	0,	1,	 8'500,	 9'500,	 7,	 26,	 31 },
	{ 241,	"Protection",	AE_DFE,	0,	-12,	-13,	0,	0,	0,	0,	 54,	IA_HELM,	IS_ALL,		0,	0,	1,	12'500,	13'500,	 8,	 33,	 36 },
	{ 241,	"Protection",	AE_DFE,	0,	-14,	-15,	0,	0,	0,	0,	 64,	IA_HELM,	IS_ALL,		0,	0,	1,	14'500,	15'500,	 9,	 37,	 40 },
	{ 241,	"Protection",	AE_DFE,	0,	-16,	-17,	0,	0,	0,	0,	 74,	IA_HELM,	IS_ALL,		0,	0,	1,	16'000,	17'000,	10,	 45,	 47 },
	{ 241,	"Protection",	AE_DFE,	0,	-18,	-19,	0,	0,	0,	0,	 89,	IA_HELM,	IS_ALL,		0,	0,	1,	18'000,	19'000,	11,	 48,	 52 },
	{ 241,	"Protection",	AE_DFE,	0,	-20,	-21,	0,	0,	0,	0,	104,	IA_HELM,	IS_ALL,		0,	0,	1,	20'000,	21'000,	12,	 55,	 60 },
	{ 241,	"Protection",	AE_DFE,	0,	-22,	-24,	0,	0,	0,	0,	124,	IA_HELM,	IS_BATTLE,	0,	0,	1,	22'000,	24'000,	13,	 65,	 70 },
	{ 241,	"Protection",	AE_DFE,	0,	-25,	-27,	0,	0,	0,	0,	149,	IA_HELM,	IS_BATTLE,	0,	0,	1,	25'000,	27'000,	14,	 75,	 80 },
	{ 241,	"Protection",	AE_DFE,	0,	-28,	-30,	0,	0,	0,	0,	172,	IA_HELM,	IS_BATTLE,	0,	0,	1,	31'000,	33'000,	15,	 85,	 95 },
	{ 241,	"Protection",	AE_DFE,	0,	-31,	-35,	0,	0,	0,	0,	178,	IA_HELM,	IS_BATTLE,	0,	0,	0,	41'000,	45'000,	16,	105,	120 },

		//jewelry
	{ 10,	"Security",		AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 10,	IA_JEWEL,	IS_ALL,	1,	0,	1,	   500,	   500,	2,	 2,	  3 },
	{ 11,	"Rescue",		AE_DFE,	0,	 -2,	 -2,	0,	0,	0,	0,	 23,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 2'000,	 2'000,	3,	 8,	 10 },
	{ 12,	"Shelter",		AE_DFE,	0,	 -3,	 -3,	0,	0,	0,	0,	 44,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 4'000,	 4'000,	4,	15,	 19 },
	{ 13,	"Safehaven",	AE_DFE,	0,	 -4,	 -4,	0,	0,	0,	0,	 60,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 6'000,	 6'000,	5,	23,	 27 },
	{ 13,	"Asylum",		AE_DFE,	0,	 -5,	 -5,	0,	0,	0,	0,	 80,	IA_JEWEL,	IS_ALL,	1,	0,	1,	 9'000,	 9'000,	5,	33,	 37 },
	{ 13,	"Deliverance",	AE_DFE,	0,	 -6,	 -7,	0,	0,	0,	0,	100,	IA_JEWEL,	IS_ALL,	1,	0,	1,	11'000,	11'000,	6,	48,	 52 },
	{ 13,	"Refuge",		AE_DFE,	0,	 -8,	 -9,	0,	0,	0,	0,	120,	IA_JEWEL,	IS_ALL,	1,	0,	1,	14'000,	14'000,	6,	63,	 67 },
	{ 13,	"Freedom",		AE_DFE,	0,	-10,	-11,	0,	0,	0,	0,	140,	IA_JEWEL,	IS_ALL,	1,	0,	1,	17'000,	17'000,	7,	78,	 82 },
	{ 13,	"Sanctuary",	AE_DFE,	0,	-12,	-13,	0,	0,	0,	0,	160,	IA_JEWEL,	IS_ALL,	1,	0,	1,	21'000,	21'000,	7,	93,	 97 },
	{ 13,	"Salvation",	AE_DFE,	0,	-14,	-15,	0,	0,	0,	0,	178,	IA_JEWEL,	IS_ALL,	1,	0,	0,	25'000,	25'000,	8,	99,	120 },


	/* ------------------------------------
					STATS
	-------------------------------------*/
	
	//STR
	{ 279,	"Staunchness",	AE_STAT,	ST_STRENGTH,	 1,	  2,	0,	0,	0,	0,	  1,	IA_ALL,																			IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
	{ 279,	"Stoutness",	AE_STAT,	ST_STRENGTH,	 3,	  4,	0,	0,	0,	0,	  2,	IA_ALL,																			IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
	{ 279,	"Stalwartness",	AE_STAT,	ST_STRENGTH,	 5,	  6,	0,	0,	0,	0,	  4,	IA_ALL,																			IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
	{ 279,	"Ruggedness",	AE_STAT,	ST_STRENGTH,	 7,	  8,	0,	0,	0,	0,	  7,	IA_ALL,																			IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
	{ 280,	"Heartiness",	AE_STAT,	ST_STRENGTH,	 9,	 10,	0,	0,	0,	0,	 10,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
	{ 280,	"Burliness",	AE_STAT,	ST_STRENGTH,	11,	 15,	0,	0,	0,	0,	 20,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
	{ 281,	"Solidity",		AE_STAT,	ST_STRENGTH,	16,	 20,	0,	0,	0,	0,	 30,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
	{ 282,	"Potency",		AE_STAT,	ST_STRENGTH,	21,	 25,	0,	0,	0,	0,	 40,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
	{ 283,	"Tenacity",		AE_STAT,	ST_STRENGTH,	26,	 30,	0,	0,	0,	0,	 50,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
	{ 284,	"Strength",		AE_STAT,	ST_STRENGTH,	31,	 40,	0,	0,	0,	0,	 60,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 27 },
	{ 285,	"Force",		AE_STAT,	ST_STRENGTH,	41,	 45,	0,	0,	0,	0,	 70,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	23'500,	24'000,	 6,	27,	 37 },
	{ 285,	"Power",		AE_STAT,	ST_STRENGTH,	46,	 50,	0,	0,	0,	0,	 80,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	27'500,	28'000,	 7,	37,	 47 },
	{ 285,	"Brawn",		AE_STAT,	ST_STRENGTH,	51,	 55,	0,	0,	0,	0,	 90,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	30'500,	35'000,	 8,	47,	 55 },
	{ 285,	"Brute",		AE_STAT,	ST_STRENGTH,	56,	 60,	0,	0,	0,	0,	105,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 65 },
	{ 285,	"Might",		AE_STAT,	ST_STRENGTH,	61,	 70,	0,	0,	0,	0,	125,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 75 },
	{ 285,	"Giants",		AE_STAT,	ST_STRENGTH,	71,	 80,	0,	0,	0,	0,	144,	IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL | IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 85 },
	{ 285,	"Titans",		AE_STAT,	ST_STRENGTH,	81,	 90,	0,	0,	0,	0,	166,	IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_GLOVE,									IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 95 },
	{ 285,	"Goliath",		AE_STAT,	ST_STRENGTH,	91,	100,	0,	0,	0,	0,	186,	IA_AXE | IA_SWORD_2H | IA_MACE_2H,												IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },
		
	//DEX
	{ 279,	"Craftiness",	AE_STAT,	ST_DEXTERITY,	 1,	  2,	0,	0,	0,	0,	  1,	IA_ALL,																IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
	{ 279,	"Aptitude",		AE_STAT,	ST_DEXTERITY,	 3,	  4,	0,	0,	0,	0,	  2,	IA_ALL,																IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
	{ 279,	"Proficiency",	AE_STAT,	ST_DEXTERITY,	 5,	  6,	0,	0,	0,	0,	  4,	IA_ALL,																IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
	{ 279,	"Talent",		AE_STAT,	ST_DEXTERITY,	 7,	  8,	0,	0,	0,	0,	  7,	IA_ALL,																IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
	{ 280,	"Agility",		AE_STAT,	ST_DEXTERITY,	 9,	 10,	0,	0,	0,	0,	 11,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
	{ 280,	"Nimbleness",	AE_STAT,	ST_DEXTERITY,	11,	 15,	0,	0,	0,	0,	 16,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
	{ 281,	"Skill",		AE_STAT,	ST_DEXTERITY,	16,	 20,	0,	0,	0,	0,	 22,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
	{ 282,	"Exactness",	AE_STAT,	ST_DEXTERITY,	21,	 25,	0,	0,	0,	0,	 29,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
	{ 283,	"Dexterity",	AE_STAT,	ST_DEXTERITY,	26,	 30,	0,	0,	0,	0,	 37,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
	{ 284,	"Accuracy",		AE_STAT,	ST_DEXTERITY,	31,	 40,	0,	0,	0,	0,	 46,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 26 },
	{ 285,	"Cunning",		AE_STAT,	ST_DEXTERITY,	41,	 45,	0,	0,	0,	0,	 56,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	18'500,	23'000,	 6,	27,	 36 },
	{ 285,	"Deftness",		AE_STAT,	ST_DEXTERITY,	46,	 50,	0,	0,	0,	0,	 71,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	26'500,	27'000,	 7,	37,	 46 },
	{ 285,	"Expertise",	AE_STAT,	ST_DEXTERITY,	51,	 55,	0,	0,	0,	0,	 86,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_ALL,		0,	0,	1,	30'500,	35'000,	 8,	47,	 56 },
	{ 285,	"Precision",	AE_STAT,	ST_DEXTERITY,	56,	 60,	0,	0,	0,	0,	107,	IA_RANGE | IA_PISTOL | IA_MELEE | IA_JEWEL | IA_ARMOR | IA_SHIELD,	IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 67 },
	{ 285,	"Mastery",		AE_STAT,	ST_DEXTERITY,	61,	 70,	0,	0,	0,	0,	127,	IA_RANGE | IA_PISTOL | IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 75 },
	{ 285,	"Perfection",	AE_STAT,	ST_DEXTERITY,	71,	 80,	0,	0,	0,	0,	146,	IA_RANGE | IA_JEWEL | IA_BODY_ARMOR | IA_BOOTS,						IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 85 },
	{ 285,	"Guile",		AE_STAT,	ST_DEXTERITY,	81,	 90,	0,	0,	0,	0,	168,	IA_RANGE | IA_BOOTS,												IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 95 },
	{ 285,	"Grace",		AE_STAT,	ST_DEXTERITY,	91,	100,	0,	0,	0,	0,	186,	IA_RANGE,															IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },
	
	//MAG	
	{ 279,	"Sanity",		AE_STAT,	ST_MAGIC,	 1,	  2,	0,	0,	0,	0,	  1,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
	{ 279,	"Mind",			AE_STAT,	ST_MAGIC,	 3,	  4,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
	{ 279,	"Clarity",		AE_STAT,	ST_MAGIC,	 5,	  6,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
	{ 279,	"Insight",		AE_STAT,	ST_MAGIC,	 7,	  8,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
	{ 280,	"Reasoning",	AE_STAT,	ST_MAGIC,	 9,	 10,	0,	0,	0,	0,	 10,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
	{ 280,	"Divination",	AE_STAT,	ST_MAGIC,	11,	 15,	0,	0,	0,	0,	 20,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
	{ 281,	"Intellect",	AE_STAT,	ST_MAGIC,	16,	 20,	0,	0,	0,	0,	 30,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	 11 },
	{ 282,	"Discernment",	AE_STAT,	ST_MAGIC,	21,	 25,	0,	0,	0,	0,	 40,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 16 },
	{ 283,	"Magic",		AE_STAT,	ST_MAGIC,	26,	 30,	0,	0,	0,	0,	 50,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 21 },
	{ 284,	"Knowledge",	AE_STAT,	ST_MAGIC,	31,	 40,	0,	0,	0,	0,	 60,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 27 },
	{ 285,	"Erudition",	AE_STAT,	ST_MAGIC,	41,	 45,	0,	0,	0,	0,	 70,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	20'500,	30'000,	 6,	27,	 37 },
	{ 285,	"Foresight",	AE_STAT,	ST_MAGIC,	46,	 50,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	20'500,	30'000,	 7,	37,	 47 },
	{ 285,	"Wisdom",		AE_STAT,	ST_MAGIC,	51,	 55,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	30'500,	35'000,	 8,	47,	 57 },
	{ 285,	"Enlightenment",AE_STAT,	ST_MAGIC,	56,	 60,	0,	0,	0,	0,	105,	IA_JEWEL | IA_STAFF | IA_FLASK | IA_SHIELD | IA_ARMOR,					IS_ALL,		0,	0,	1,	35'500,	40'000,	 9,	57,	 67 },
	{ 285,	"Epiphany",		AE_STAT,	ST_MAGIC,	61,	 70,	0,	0,	0,	0,	125,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_HELM | IA_BODY_ARMOR | IA_GLOVE,	IS_MAGIC,	0,	0,	1,	40'500,	45'000,	10,	67,	 75 },
	{ 285,	"Witchcraft",	AE_STAT,	ST_MAGIC,	71,	 80,	0,	0,	0,	0,	145,	IA_JEWEL | IA_STAFF | IA_SHIELD| IA_HELM,								IS_MAGIC,	0,	0,	1,	45'500,	50'000,	11,	77,	 85 },
	{ 285,	"Sorcery",		AE_STAT,	ST_MAGIC,	81,	 90,	0,	0,	0,	0,	166,	IA_JEWEL | IA_STAFF| IA_SHIELD,											IS_MAGIC,	0,	0,	1,	50'500,	55'000,	12,	87,	 95 },
	{ 285,	"Wizardry",		AE_STAT,	ST_MAGIC,	91,	100,	0,	0,	0,	0,	186,	IA_STAFF,																IS_MAGIC,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },
	
	//VIT
	{ 279,	"Health",		AE_STAT,	ST_VITALITY,	 1,	  2,	0,	0,	0,	0,	  1,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	    25,	    50,	 1,	 0,	  1 },
	{ 279,	"Sustenance",	AE_STAT,	ST_VITALITY,	 3,	  4,	0,	0,	0,	0,	  2,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  1 },
	{ 279,	"Robustness",	AE_STAT,	ST_VITALITY,	 5,	  6,	0,	0,	0,	0,	  4,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	   100,	   200,	 1,	 0,	  1 },
	{ 279,	"Hardiness",	AE_STAT,	ST_VITALITY,	 7,	  8,	0,	0,	0,	0,	  7,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	   300,	   400,	 1,	 1,	  3 },
	{ 280,	"Constitution",	AE_STAT,	ST_VITALITY,	 9,	 10,	0,	0,	0,	0,	 11,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	   600,	 1'200,	 2,	 2,	  4 },
	{ 280,	"Zest",			AE_STAT,	ST_VITALITY,	11,	 15,	0,	0,	0,	0,	 16,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'600,	 2'400,	 2,	 4,	  6 },
	{ 281,	"Zeal",			AE_STAT,	ST_VITALITY,	16,	 20,	0,	0,	0,	0,	 22,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'800,	 3'200,	 2,	 7,	  9 },
	{ 282,	"Verve",		AE_STAT,	ST_VITALITY,	21,	 25,	0,	0,	0,	0,	 29,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	 4'250,	 6'000,	 3,	12,	 14 },
	{ 283,	"Vitality",		AE_STAT,	ST_VITALITY,	26,	 30,	0,	0,	0,	0,	 37,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	 7'500,	 9'600,	 4,	17,	 20 },
	{ 284,	"Vim",			AE_STAT,	ST_VITALITY,	31,	 40,	0,	0,	0,	0,	 46,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_ALL,		0,	0,	1,	13'200,	16'000,	 5,	22,	 25 },
	{ 285,	"Vigor",		AE_STAT,	ST_VITALITY,	41,	 45,	0,	0,	0,	0,	 56,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_BATTLE,	0,	0,	1,	20'500,	30'000,	 6,	27,	 31 },
	{ 285,	"Vivacity",		AE_STAT,	ST_VITALITY,	46,	 50,	0,	0,	0,	0,	 71,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_BATTLE,	0,	0,	1,	20'500,	30'000,	 7,	37,	 42 },
	{ 285,	"Vehemence",	AE_STAT,	ST_VITALITY,	51,	 55,	0,	0,	0,	0,	 86,	IA_JEWEL|IA_SHIELD|IA_ARMOR,	IS_BATTLE,	0,	0,	1,	30'500,	35'000,	 8,	47,	 53 },
	{ 285,	"Ardor",		AE_STAT,	ST_VITALITY,	56,	 60,	0,	0,	0,	0,	106,	IA_SHIELD|IA_ARMOR,				IS_BATTLE,	0,	0,	1,	35'500,	40'000,	 9,	57,	 64 },
	{ 285,	"Fervor",		AE_STAT,	ST_VITALITY,	61,	 70,	0,	0,	0,	0,	126,	IA_ARMOR,						IS_BATTLE,	0,	0,	1,	40'500,	45'000,	10,	67,	 75 },
	{ 285,	"Life",			AE_STAT,	ST_VITALITY,	71,	 80,	0,	0,	0,	0,	146,	IA_BODY_ARMOR|IA_BELT|IA_BOOTS,	IS_BATTLE,	0,	0,	1,	45'500,	50'000,	11,	77,	 86 },
	{ 285,	"Essence",		AE_STAT,	ST_VITALITY,	81,	 90,	0,	0,	0,	0,	167,	IA_BODY_ARMOR|IA_BELT,			IS_BATTLE,	0,	0,	1,	50'500,	55'000,	12,	87,	 96 },
	{ 285,	"Colossus",		AE_STAT,	ST_VITALITY,	91,	100,	0,	0,	0,	0,	186,	IA_BODY_ARMOR,					IS_BATTLE,	0,	0,	1,	55'500,	60'000,	13,	97,	107 },
	
	//ALL STATS
	{ 286,	"the Plains",		AE_ALL_STAT,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_JEWEL|IA_SHIELD|IA_ARMOR|IA_FLASK,	IS_ALL,	0,	0,	1,	   150,	    150,	 2,	 0,	  1 },
	{ 286,	"the Hills",		AE_ALL_STAT,	0,	 2,	 2,	0,	0,	0,	0,	  6,	IA_JEWEL|IA_SHIELD|IA_ARMOR|IA_FLASK,	IS_ALL,	0,	0,	1,	   500,	    500,	 2,	 0,	  1 },
	{ 287,	"the Mountains",	AE_ALL_STAT,	0,	 3,	 3,	0,	0,	0,	0,	  9,	IA_JEWEL|IA_SHIELD|IA_ARMOR|IA_FLASK,	IS_ALL,	0,	0,	1,	 1'000,	  1'000,	 3,	 1,	  2 },
	{ 288,	"the Rainbow",		AE_ALL_STAT,	0,	 4,	 4,	0,	0,	0,	0,	 13,	IA_JEWEL|IA_SHIELD|IA_ARMOR|IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  2'000,	 3,	 7,	  9 },
	{ 289,	"the Lights",		AE_ALL_STAT,	0,	 5,	 6,	0,	0,	0,	0,	 23,	IA_JEWEL|IA_SHIELD|IA_ARMOR|IA_FLASK,	IS_ALL,	0,	0,	1,	 4'000,	  5'000,	 4,	10,	 12 },
	{ 290,	"the Clouds",		AE_ALL_STAT,	0,	 7,	 8,	0,	0,	0,	0,	 33,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	10'000,	 12'000,	 6,	19,	 21 },
	{ 291,	"the Sky",			AE_ALL_STAT,	0,	 9,	10,	0,	0,	0,	0,	 43,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	14'000,	 16'000,	 8,	25,	 28 },
	{ 292,	"the Heavens",		AE_ALL_STAT,	0,	11,	12,	0,	0,	0,	0,	 53,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	27'000,	 30'000,	 9,	32,	 35 },
	{ 293,	"the Moon",			AE_ALL_STAT,	0,	13,	14,	0,	0,	0,	0,	 63,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	35'000,	 45'000,	10,	40,	 44 },
	{ 497,	"the Sun",			AE_ALL_STAT,	0,	15,	16,	0,	0,	0,	0,	 83,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	 55'000,	11,	50,	 55 },
	{ 498,	"the Stars",		AE_ALL_STAT,	0,	17,	18,	0,	0,	0,	0,	103,	IA_JEWEL|IA_SHIELD|IA_ARMOR,			IS_ALL,	0,	0,	1,	55'000,	 65'000,	12,	60,	 66 },
	{ 499,	"the Zodiac",		AE_ALL_STAT,	0,	19,	20,	0,	0,	0,	0,	123,	IA_JEWEL|IA_ARMOR,						IS_ALL,	0,	0,	1,	65'000,	 75'000,	13,	70,	 77 },
	{ 500,	"Constellations",	AE_ALL_STAT,	0,	21,	22,	0,	0,	0,	0,	143,	IA_JEWEL,								IS_ALL,	0,	0,	1,	75'000,	 85'000,	14,	80,	 88 },
	{ 501,	"the Galaxies",		AE_ALL_STAT,	0,	23,	25,	0,	0,	0,	0,	163,	IA_JEWEL,								IS_ALL,	0,	0,	1,	85'000,	 95'000,	15,	90,	 99 },
	{ 502,	"the Universe",		AE_ALL_STAT,	0,	26,	30,	0,	0,	0,	0,	178,	IA_JEWEL,								IS_ALL,	0,	0,	0,	96'000,	100'000,	17,	99,	110 },

	
	/* ------------------------------------
					SLVL
	-------------------------------------*/
	
	{ 294,	"the Focus",	AE_SPELLS_LEVEL,	0,	1,	1,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 2'000,	 2'000,	2,	 0,	  0 },
	{ 295,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 6'000,	 6'000,	4,	12,	 18 },
	{ 296,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	3,	3,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	12'000,	12'000,	8,	19,	 28 },
	{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	4,	4,	0,	0,	0,	0,	 64,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	25'000,	25'000,	12,	29,	 34 },
	{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	5,	5,	0,	0,	0,	0,	105,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	35'000,	35'000,	13,	37,	 50 },
	{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	6,	6,	0,	0,	0,	0,	147,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	45'000,	45'000,	14,	63,	 80 },
	{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	7,	7,	0,	0,	0,	0,	178,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	55'000,	55'000,	15,	99,	119 },
	
	
	/* ------------------------------------
					MANA
	-------------------------------------*/
		
	{ 306,	"the Moth",		AE_MANA,	0,	  5,	 10,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  0 },
	{ 307,	"the Worm",		AE_MANA,	0,	 11,	 15,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	   220,	   300,	 2,	 0,	  0 },
	{ 307,	"the Spider",	AE_MANA,	0,	 16,	 20,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	   480,	   600,	 2,	 1,	  2 },
	{ 308,	"the Wasp",		AE_MANA,	0,	 21,	 25,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 1'100,	 1'500,	 2,	 3,	  5 },
	{ 309,	"the Grig",		AE_MANA,	0,	 26,	 30,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 1'600,	 2'000,	 3,	 6,	  8 },
	{ 310,	"the Frog",		AE_MANA,	0,	 31,	 40,	0,	0,	0,	0,	 29,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_ALL,		0,	0,	1,	 2'100,	 3'000,	 3,	 9,	 12 },
	{ 311,	"the Scorpion",	AE_MANA,	0,	 41,	 50,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 3'100,	 4'000,	 4,	13,	 15 },
	{ 312,	"the Rat",		AE_MANA,	0,	 51,	 60,	0,	0,	0,	0,	 48,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 5'100,	 6'000,	 5,	16,	 20 },
	{ 313,	"the Cat",		AE_MANA,	0,	 61,	 70,	0,	0,	0,	0,	 59,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 6'100,	 7'000,	 6,	21,	 27 },
	{ 314,	"the Vulture",	AE_MANA,	0,	 71,	 80,	0,	0,	0,	0,	 69,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 7'100,	 8'000,	 7,	28,	 35 },
	{ 314,	"the Serpent",	AE_MANA,	0,	 81,	 90,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 8'100,	 9'000,	 7,	35,	 39 },
	{ 314,	"the Serpent",	AE_MANA,	0,	 91,	100,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 9'100,	10'000,	 8,	40,	 44 },
	{ 314,	"the Serpent",	AE_MANA,	0,	101,	110,	0,	0,	0,	0,	100,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	10'100,	11'000,	 8,	45,	 49 },
	{ 314,	"the Serpent",	AE_MANA,	0,	111,	120,	0,	0,	0,	0,	110,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	11'100,	12'000,	 9,	49,	 53 },
	{ 314,	"the Serpent",	AE_MANA,	0,	121,	130,	0,	0,	0,	0,	120,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	12'100,	13'000,	 9,	53,	 57 },
	{ 314,	"the Viper",	AE_MANA,	0,	131,	140,	0,	0,	0,	0,	131,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	13'100,	14'000,	10,	57,	 61 },
	{ 314,	"the Python",	AE_MANA,	0,	141,	150,	0,	0,	0,	0,	140,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,		IS_MAGIC,	0,	0,	1,	14'100,	15'000,	10,	62,	 66 },
	{ 314,	"the Wyrm",		AE_MANA,	0,	151,	160,	0,	0,	0,	0,	151,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR | IA_HELM,	IS_MAGIC,	0,	0,	1,	15'100,	16'000,	11,	67,	 72 },
	{ 314,	"the Hydra",	AE_MANA,	0,	161,	170,	0,	0,	0,	0,	161,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR | IA_HELM,	IS_MAGIC,	0,	0,	1,	16'100,	17'000,	11,	73,	 79 },
	{ 314,	"the Drake",	AE_MANA,	0,	171,	180,	0,	0,	0,	0,	170,	IA_STAFF | IA_SHIELD | IA_BODY_ARMOR,			IS_MAGIC,	0,	0,	1,	17'100,	18'000,	12,	80,	 86 },
	{ 314,	"the Wyvern",	AE_MANA,	0,	181,	190,	0,	0,	0,	0,	180,	IA_STAFF | IA_SHIELD,							IS_MAGIC,	0,	0,	1,	18'100,	19'000,	12,	87,	 95 },
	{ 314,	"the Dragon",	AE_MANA,	0,	191,	200,	0,	0,	0,	0,	186,	IA_STAFF,										IS_MAGIC,	0,	0,	1,	19'100,	20'000,	13,	96,	106 },


	/* ------------------------------------
					HP
	-------------------------------------*/

	{ 306,	"the Squirrel",	AE_HIT_POINTS,	0,	  5,	 10,	0,	0,	0,	0,	  2,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	    50,	   100,	 1,	 0,	  0 },
	{ 307,	"the Rabbit",	AE_HIT_POINTS,	0,	 11,	 15,	0,	0,	0,	0,	  4,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	   220,	   300,	 2,	 0,	  0 },
	{ 307,	"the Jackal",	AE_HIT_POINTS,	0,	 16,	 20,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	   480,	   600,	 2,	 1,	  2 },
	{ 308,	"the Fox",		AE_HIT_POINTS,	0,	 21,	 25,	0,	0,	0,	0,	 14,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 1'100,	 1'500,	 2,	 3,	  5 },
	{ 309,	"the Badger",	AE_HIT_POINTS,	0,	 26,	 30,	0,	0,	0,	0,	 21,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 1'600,	 2'000,	 3,	 6,	  8 },
	{ 310,	"the Lynx",		AE_HIT_POINTS,	0,	 31,	 40,	0,	0,	0,	0,	 29,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_ALL,		0,	0,	1,	 2'100,	 3'000,	 3,	 9,	 12 },
	{ 311,	"the Elk",		AE_HIT_POINTS,	0,	 41,	 50,	0,	0,	0,	0,	 38,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 3'100,	 4'000,	 4,	13,	 15 },
	{ 312,	"the Moose",	AE_HIT_POINTS,	0,	 51,	 60,	0,	0,	0,	0,	 48,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 5'100,	 6'000,	 5,	16,	 20 },
	{ 313,	"the Wolf",		AE_HIT_POINTS,	0,	 61,	 70,	0,	0,	0,	0,	 59,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 6'100,	 7'000,	 6,	21,	 27 },
	{ 314,	"the Panther",	AE_HIT_POINTS,	0,	 71,	 80,	0,	0,	0,	0,	 69,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 7'100,	 8'000,	 7,	28,	 35 },
	{ 314,	"the Jaguar",	AE_HIT_POINTS,	0,	 81,	 90,	0,	0,	0,	0,	 80,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 8'100,	 9'000,	 7,	35,	 39 },
	{ 314,	"the Lion",		AE_HIT_POINTS,	0,	 91,	100,	0,	0,	0,	0,	 90,	IA_JEWEL | IA_ARMOR | IA_SHIELD,				IS_BATTLE,	0,	0,	1,	 9'100,	10'000,	 8,	40,	 44 },
	{ 314,	"the Tiger",	AE_HIT_POINTS,	0,	101,	110,	0,	0,	0,	0,	100,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	10'100,	11'000,	 8,	45,	 49 },
	{ 314,	"the Walrus",	AE_HIT_POINTS,	0,	111,	120,	0,	0,	0,	0,	109,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	11'100,	12'000,	 9,	49,	 53 },
	{ 314,	"the Bull",		AE_HIT_POINTS,	0,	121,	130,	0,	0,	0,	0,	120,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	12'100,	13'000,	 9,	53,	 57 },
	{ 314,	"the Ox",		AE_HIT_POINTS,	0,	131,	140,	0,	0,	0,	0,	130,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	13'100,	14'000,	10,	57,	 61 },
	{ 314,	"the Bear",		AE_HIT_POINTS,	0,	141,	150,	0,	0,	0,	0,	139,	IA_ARMOR | IA_SHIELD,							IS_BATTLE,	0,	0,	1,	14'100,	15'000,	10,	62,	 66 },
	{ 314,	"the Shark",	AE_HIT_POINTS,	0,	151,	160,	0,	0,	0,	0,	150,	IA_ARMOR,										IS_BATTLE,	0,	0,	1,	15'100,	16'000,	11,	67,	 72 },
	{ 314,	"the Whale",	AE_HIT_POINTS,	0,	161,	170,	0,	0,	0,	0,	160,	IA_BODY_ARMOR | IA_BELT | IA_BOOTS | IA_GLOVE,	IS_BATTLE,	0,	0,	1,	16'100,	17'000,	11,	73,	 79 },
	{ 314,	"the Elephant",	AE_HIT_POINTS,	0,	171,	180,	0,	0,	0,	0,	169,	IA_BODY_ARMOR | IA_BELT| IA_BOOTS,				IS_BATTLE,	0,	0,	1,	17'100,	18'000,	12,	80,	 86 },
	{ 314,	"the Mammoth",	AE_HIT_POINTS,	0,	181,	190,	0,	0,	0,	0,	180,	IA_BODY_ARMOR | IA_BELT,						IS_BATTLE,	0,	0,	1,	18'100,	19'000,	12,	87,	 95 },
	{ 314,	"the Whale",	AE_HIT_POINTS,	0,	191,	200,	0,	0,	0,	0,	186,	IA_HEAVY_ARMOR,									IS_BATTLE,	0,	0,	1,	19'100,	20'000,	13,	96,	106 },

	
	/* ------------------------------------
		RANGED WEAPONS' ELEMENTAL DAMAGE
	-------------------------------------*/
	
	{ 315,	"Ashes",			AE_ELEM_HIT,	EA_ARROW_FIRE,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 316,	"Cinder",			AE_ELEM_HIT,	EA_ARROW_FIRE,	 2,	 3,	 5,	 6,	0,	0,	 2,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 317,	"Fire",				AE_ELEM_HIT,	EA_ARROW_FIRE,	 5,	 7,	 8,	12,	0,	0,	 8,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 318,	"Flame",			AE_ELEM_HIT,	EA_ARROW_FIRE,	 9,	11,	12,	16,	0,	0,	18,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 319,	"Searing",			AE_ELEM_HIT,	EA_ARROW_FIRE,	13,	15,	16,	20,	0,	0,	38,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 320,	"Burning",			AE_ELEM_HIT,	EA_ARROW_FIRE,	17,	19,	20,	24,	0,	0,	53,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 320,	"Burning",			AE_ELEM_HIT,	EA_ARROW_FIRE,	17,	19,	20,	24,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	{ 321,	"Sparks",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 322,	"Charges",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	 8,	10,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 323,	"Shock",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	17,	21,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 324,	"Lightning",		AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	26,	32,	0,	0,	22,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 325,	"Thunder",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	35,	43,	0,	0,	39,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 326,	"Electra",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	44,	54,	0,	0,	54,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 326,	"Electra",			AE_ELEM_HIT,	EA_ARROW_LIGHTNING,	1,	1,	44,	54,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	{ 327,	"Cooling",			AE_ELEM_HIT,	EA_ARROW_COLD,	1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 328,	"Snow",				AE_ELEM_HIT,	EA_ARROW_COLD,	2,	 3,	 5,	 6,	0,	0,	 2,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 329,	"Ice",				AE_ELEM_HIT,	EA_ARROW_COLD,	5,	 7,	 8,	12,	0,	0,	 8,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 330,	"Frost",			AE_ELEM_HIT,	EA_ARROW_COLD,	9,	11,	12,	16,	0,	0,	17,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 331,	"Winter",			AE_ELEM_HIT,	EA_ARROW_COLD,	13,	15,	16,	20,	0,	0,	37,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 332,	"Freezing",			AE_ELEM_HIT,	EA_ARROW_COLD,	17,	19,	20,	24,	0,	0,	53,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 332,	"Freezing",			AE_ELEM_HIT,	EA_ARROW_COLD,	17,	19,	20,	24,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	{ 333,	"Cleansing",		AE_ELEM_HIT,	EA_ARROW_HOLY,	1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 334,	"Purification",		AE_ELEM_HIT,	EA_ARROW_HOLY,	2,	 3,	 5,	 6,	0,	0,	 2,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 335,	"Purge",			AE_ELEM_HIT,	EA_ARROW_HOLY,	5,	 7,	 8,	12,	0,	0,	 8,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 336,	"Sanctity",			AE_ELEM_HIT,	EA_ARROW_HOLY,	9,	11,	12,	16,	0,	0,	17,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 337,	"Catharsis",		AE_ELEM_HIT,	EA_ARROW_HOLY,	13,	15,	16,	20,	0,	0,	37,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 338,	"Absolution",		AE_ELEM_HIT,	EA_ARROW_HOLY,	17,	19,	20,	24,	0,	0,	53,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 338,	"Absolution",		AE_ELEM_HIT,	EA_ARROW_HOLY,	17,	19,	20,	24,	0,	0,	95,	IA_RANGE | IA_MALLET | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	{ 339,	"Voiding",			AE_ELEM_HIT,	EA_ARROW_ARCAN,	2,	 3,	 5,	 6,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 340,	"Depletion",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	5,	 7,	 8,	12,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 341,	"Havoc",			AE_ELEM_HIT,	EA_ARROW_ARCAN,	9,	11,	12,	16,	0,	0,	24,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 342,	"Exhausting",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	13,	15,	16,	20,	0,	0,	40,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 343,	"Devastation",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	17,	19,	20,	24,	0,	0,	61,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 343,	"Devastation",		AE_ELEM_HIT,	EA_ARROW_ARCAN,	17,	19,	20,	24,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	{ 344,	"Blight",			AE_ELEM_HIT,	EA_ARROW_ACID,	1,	 1,	 2,	 3,	0,	0,	 1,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	{ 345,	"Acidity",			AE_ELEM_HIT,	EA_ARROW_ACID,	2,	 3,	 5,	 6,	0,	0,	 3,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	   500,	   500,	2,	 0,	 0 },
	{ 346,	"Toxicity",			AE_ELEM_HIT,	EA_ARROW_ACID,	5,	 7,	 8,	12,	0,	0,	10,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 2,	 3 },
	{ 347,	"Corrosion",		AE_ELEM_HIT,	EA_ARROW_ACID,	9,	11,	12,	16,	0,	0,	22,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	 6,	 8 },
	{ 348,	"Dissolution",		AE_ELEM_HIT,	EA_ARROW_ACID,	13,	15,	16,	20,	0,	0,	39,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	6,	20,	24 },
	{ 349,	"Disintegration",	AE_ELEM_HIT,	EA_ARROW_ACID,	17,	19,	20,	24,	0,	0,	59,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	 8'000,	 8'000,	8,	30,	33 },
	{ 349,	"Disintegration",	AE_ELEM_HIT,	EA_ARROW_ACID,	17,	19,	20,	24,	0,	0,	95,	IA_RANGE | IA_KNIFE | IA_PISTOL,				IS_BATTLE,	0,	0,	1,	11'000,	11'000,	8,	50,	58 },

	
	/* ------------------------------------
					VARIOUS
	-------------------------------------*/
	
	{ 350,	"Parrying",	AE_SPECIAL_AC,	0,	1,	3,	0,	0,	0,	0,	3,	IA_STAFF|IA_MELEE|IA_CLAW,	IS_BATTLE,	0,	0,	1,	100,	300,	2,	0,	0 },
	{ 351,	"Fending",	AE_SPECIAL_AC,	0,	4,	5,	0,	0,	0,	0,	8,	IA_STAFF|IA_MELEE|IA_CLAW,	IS_BATTLE,	0,	0,	1,	300,	600,	3,	3,	4 },
	
	{ 356,	"the Bat",	AE_MANA_STEAL_PERCENT,	0,	1,	1,	0,	0,	0,	0,	  8,	IA_JEWEL|IA_STAFF|IA_MELEE|IA_CLAW,	IS_BATTLE,	0,	0,	1,	 1'500,	 1'500,	 2,	 3,	 4 },
	{ 357,	"Vampires",	AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 25,	IA_STAFF|IA_MELEE|IA_CLAW,			IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	 3,	16,	20 },
	{ 357,	"Vampires",	AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 48,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	 4,	21,	30 },
	{ 357,	"Vampires",	AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 94,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 5,	45,	56 },
	{ 357,	"Vampires",	AE_MANA_STEAL_PERCENT,	0,	3,	3,	0,	0,	0,	0,	131,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	0,	39'000,	39'000,	11,	58,	63 },

	{ 358,	"The leech",AE_LIFE_STEAL_PERCENT,	0,	1,	1,	0,	0,	0,	0,	  8,	IA_JEWEL|IA_STAFF|IA_MELEE|IA_CLAW,	IS_BATTLE,	0,	0,	1,	 1'500,	 1'500,	 2,	 3,	 4 },
	{ 359,	"Blood",	AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 25,	IA_STAFF|IA_MELEE|IA_CLAW,			IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	 3,	16,	20 },
	{ 359,	"Blood",	AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 48,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	 4,	21,	30 },
	{ 359,	"Blood",	AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	 94,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 5,	45,	56 },
	{ 359,	"Blood",	AE_LIFE_STEAL_PERCENT,	0,	3,	3,	0,	0,	0,	0,	132,	IA_STAFF | IA_MELEE | IA_CLAW,		IS_BATTLE,	0,	0,	0,	39'000,	39'000,	11,	58,	63 },

	{ 360,	"Piercing",	AE_ARMOR_PIERCING,	0,	3,	3,	0,	0,	0,	0,	 12,	IA_RANGE|IA_STAFF|IA_MELEE|IA_CLAW,			IS_BATTLE,	1,	0,	1,	 1'500,	 1'500,	2,	 4,	 6 },
	{ 361,	"Bashing",	AE_ARMOR_PIERCING,	0,	4,	4,	0,	0,	0,	0,	 27,	IA_RANGE|IA_STAFF|IA_MELEE|IA_CLAW,			IS_BATTLE,	1,	0,	1,	 5'000,	 5'000,	3,	15,	18 },
	{ 361,	"Bashing",	AE_ARMOR_PIERCING,	0,	4,	4,	0,	0,	0,	0,	 50,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	1,	0,	1,	11'000,	11'000,	4,	30,	33 },
	{ 361,	"Bashing",	AE_ARMOR_PIERCING,	0,	5,	5,	0,	0,	0,	0,	 94,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	1,	0,	1,	20'000,	20'000,	5,	45,	56 },
	
	{ 362,	"Alacrity",	AE_ATTACK_SPEED,	0,	1,	1,	0,	0,	0,	0,	 20,	IA_JEWEL|IA_RANGE|IA_STAFF|IA_MELEE|IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 4'000,	 4'000,	 4,	  7,	  9 },
	{ 363,	"Speed",	AE_ATTACK_SPEED,	0,	2,	2,	0,	0,	0,	0,	 40,	IA_JEWEL|IA_RANGE|IA_STAFF|IA_MELEE|IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 9'000,	 9'000,	 8,	 21,	 26 },
	{ 364,	"Haste",	AE_ATTACK_SPEED,	0,	3,	3,	0,	0,	0,	0,	 70,	IA_RANGE|IA_STAFF|IA_MELEE|IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,			IS_BATTLE,	0,	0,	1,	15'000,	15'000,	12,	 42,	 45 },
	{ 364,	"Haste",	AE_ATTACK_SPEED,	0,	3,	3,	0,	0,	0,	0,	110,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	25'000,	25'000,	14,	 68,	 74 },
	{ 364,	"Velocity",	AE_ATTACK_SPEED,	0,	4,	4,	0,	0,	0,	0,	178,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	75'000,	75'000,	25,	120,	130 },
		// jewelry solo fast attack
	{ 364,	"Haste",	AE_ATTACK_SPEED,	0,	2,	2,	0,	0,	0,	0,	 74,	IA_JEWEL,																		IS_BATTLE,	0,	0,	1,	20'000,	20'000,	13,	 44,	 47 },
	{ 364,	"Haste",	AE_ATTACK_SPEED,	0,	2,	2,	0,	0,	0,	0,	108,	IA_JEWEL,																		IS_BATTLE,	0,	0,	1,	26'000,	26'000,	15,	 59,	 63 },

	// HIT RECOVERY
	{ 365,	"Balance",		AE_HIT_RECOVERY,	0,	1,	1,	0,	0,	0,	0,	 2,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	    50,	    50,	2,	 0,	 0 },
	{ 365,	"Balance",		AE_HIT_RECOVERY,	0,	1,	1,	0,	0,	0,	0,	 4,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	    75,	    75,	2,	 0,	 0 },
	{ 365,	"Balance",		AE_HIT_RECOVERY,	0,	1,	1,	0,	0,	0,	0,	 7,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_ALL,	0,	0,	1,	   100,	   100,	3,	 1,	 2 },
	{ 366,	"Stability",	AE_HIT_RECOVERY,	0,	2,	2,	0,	0,	0,	0,	16,	IA_JEWEL | IA_ARMOR,				IS_ALL,	0,	0,	1,	 1'500,	 1'500,	4,	 3,	 4 },
	{ 484,	"Fixity",		AE_HIT_RECOVERY,	0,	2,	2,	0,	0,	0,	0,	31,	IA_JEWEL | IA_ARMOR,				IS_ALL,	0,	0,	1,	 2'500,	 2'500,	5,	 5,	15 },
	{ 484,	"Fixity",		AE_HIT_RECOVERY,	0,	2,	2,	0,	0,	0,	0,	56,	IA_JEWEL | IA_ARMOR,				IS_ALL,	0,	0,	1,	 5'000,	 5'000,	6,	16,	25 },
	{ 484,	"Fixity",		AE_HIT_RECOVERY,	0,	2,	2,	0,	0,	0,	0,	92,	IA_JEWEL | IA_ARMOR,				IS_ALL,	0,	0,	1,	10'000,	10'000,	6,	25,	35 },	
	{ 367,	"Recovery",		AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0,	50,	IA_BOOTS,							IS_ALL,	0,	0,	1,	21'000,	21'000,	7,	25,	30 },
	{ 367,	"Recovery",		AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0,	94,	IA_BOOTS,							IS_ALL,	0,	0,	1,	25'000,	25'000,	8,	45,	56 },

	// KB
	{ 368,	"Rejection",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	  750,	  750,	3,	 0,	 1 },
	{ 369,	"Rejection",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	1'500,	1'500,	5,	 2,	 4 },
	{ 370,	"Rejection",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	32,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	3'000,	3'000,	6,	15,	24 },
	{ 370,	"Rejection",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	60,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	4'000,	4'000,	7,	25,	30 },
	{ 370,	"Rejection",	AE_FLAG,	EA_KNOCK_BACK,	0,	0,	0,	0,	0,	0,	96,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_MALLET | IA_PISTOL,	IS_BATTLE,	16,	0,	1,	5'000,	5'000,	8,	41,	56 },

	{ 371,	"Draining",	AE_FLAG,	EA_LIFE_STEAL_RND,	0,	0,	0,	0,	0,	0,	26,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 2'000,	 2'000,	4,	15,	18 },
	{ 371,	"Draining",	AE_FLAG,	EA_LIFE_STEAL_RND,	0,	0,	0,	0,	0,	0,	50,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	5,	19,	25 },
	{ 371,	"Draining",	AE_FLAG,	EA_LIFE_STEAL_RND,	0,	0,	0,	0,	0,	0,	95,	IA_STAFF | IA_MELEE | IA_CLAW,	IS_BATTLE,	0,	0,	1,	14'000,	14'000,	6,	26,	54 },

	// is fast block even needed? maybe it should be reworked properly... After considering it, it is inappropriate here
	//{ 372,	"Blocking",	AE_FLAG,	EA_FAST_BLOCK,	1,	1,	0,	0,	0,	0,	25,	IA_SHIELD,	IS_BATTLE,	0,	0,	1,	3000,	3000,	4,	11,	11+2 },
	
	{ 373,	"Bravery",	AE_FLAG,	EA_FURY_DURATION,	0,	0,	0,	0,	0,	0,	33,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 5'000,	 5'000,	6,	 6,	12 },
	{ 373,	"Bravery",	AE_FLAG,	EA_FURY_DURATION,	0,	0,	0,	0,	0,	0,	62,	IA_JEWEL,	IS_ALL,	0,	0,	1,	 8'000,	 8'000,	7,	31,	36 },
	{ 373,	"Bravery",	AE_FLAG,	EA_FURY_DURATION,	0,	0,	0,	0,	0,	0,	90,	IA_JEWEL,	IS_ALL,	0,	0,	1,	15'000,	15'000,	8,	50,	57 },

	// MULTISHOT
	{ 374,	"Assault",	AE_FLAG,	EA_MULTISHOT,	0,	0,	0,	0,	0,	0,	25,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	5,	14,	18 },
	{ 374,	"Assault",	AE_FLAG,	EA_MULTISHOT,	0,	0,	0,	0,	0,	0,	48,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	11'000,	11'000,	6,	28,	32 },
	{ 374,	"Assault",	AE_FLAG,	EA_MULTISHOT,	0,	0,	0,	0,	0,	0,	94,	IA_RANGE | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	21'000,	21'000,	7,	45,	49 },
	
		//use profit on uniques...
	//{ 375,	"Profit",	AE_FLAG,	EA_MANACOST,	0,	0,	0,	0,	0,	0,	30,	IA_JEWEL|IA_STAFF,	IS_MAGIC,	0,	0,	1,	9000,	9000,	8,	12,	12+2 },
	
	{ 376,	"Ruin",	AE_FLAG,	EA_MORE_DAMAGE_CHANCE,	0,	0,	0,	0,	0,	0,	30,	IA_JEWEL|IA_RANGE|IA_STAFF|IA_MELEE|IA_FLASK|IA_TRAP|IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,				IS_ALL,	0,	0,	1,	 6'000,	 6'000,	5,	18,	21 },
	{ 376,	"Ruin",	AE_FLAG,	EA_MORE_DAMAGE_CHANCE,	0,	0,	0,	0,	0,	0,	60,	IA_JEWEL | IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	 9'000,	 9'000,	6,	33,	38 },
	{ 376,	"Ruin",	AE_FLAG,	EA_MORE_DAMAGE_CHANCE,	0,	0,	0,	0,	0,	0,	90,	IA_JEWEL | IA_RANGE | IA_STAFF | IA_MELEE | IA_FLASK | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_ALL,	0,	0,	1,	13'000,	13'000,	7,	50,	55 },


	/* ------------------------------------
					  AC
	-------------------------------------*/

	{ 377,	"Thwarting",		AE_AC,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	{ 378,	"Rebuffing",		AE_AC,	0,	 2,	 3,	0,	0,	0,	0,	  3,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   300,	   450,	 2,	  0,	  1 },
	{ 379,	"Safety",			AE_AC,	0,	 4,	 6,	0,	0,	0,	0,	  8,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	   800,	 1'200,	 2,	  1,	  2 },
	{ 380,	"Repulsion",		AE_AC,	0,	 7,	10,	0,	0,	0,	0,	 16,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 1'750,	 2'500,	 3,	  3,	  4 },
	{ 381,	"Deflection",		AE_AC,	0,	11,	15,	0,	0,	0,	0,	 27,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 3'300,	 4'500,	 4,	 16,	 18 },
	{ 382,	"Deflection",		AE_AC,	0,	16,	20,	0,	0,	0,	0,	 43,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 5'600,	 7'000,	 5,	 28,	 33 },
	{ 382,	"Deflection",		AE_AC,	0,	21,	25,	0,	0,	0,	0,	 70,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	 8'200,	 9'000,	 6,	 35,	 38 },
	{ 382,	"Imperviousness",	AE_AC,	0,	26,	30,	0,	0,	0,	0,	100,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	11'000,	15'000,	 7,	 45,	 55 },
	{ 382,	"Imperviousness",	AE_AC,	0,	31,	35,	0,	0,	0,	0,	130,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	21'000,	25'000,	 8,	 60,	 70 },
	{ 382,	"Imperviousness",	AE_AC,	0,	36,	40,	0,	0,	0,	0,	159,	IA_SHIELD | IA_ARMOR,	IS_ALL,		0,	0,	1,	26'000,	30'000,	 9,	 83,	 99 },
	{ 382,	"Imperviousness",	AE_AC,	0,	41,	50,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,			IS_BATTLE,	0,	0,	0,	41'000,	50'000,	15,	100,	120 },

	
	/* ------------------------------------
				+ GOLD FLAT
	-------------------------------------*/
	
	{ 383,	"Avarice",		AE_GOLD_ABS_FIND,	0,	 1,	 2,	 3,	 4,	0,	0,	  2,	IA_ALL,										IS_ALL,	0,	0,	1,	   100,	   400,	2,	 0,	 0 },
	{ 384,	"Greed",		AE_GOLD_ABS_FIND,	0,	 5,	 6,	 7,	 8,	0,	0,	  7,	IA_ALL,										IS_ALL,	0,	0,	1,	   500,	   800,	2,	 0,	 0 },
	{ 385,	"Wealth",		AE_GOLD_ABS_FIND,	0,	 9,	10,	11,	13,	0,	0,	 12,	IA_ALL,										IS_ALL,	0,	0,	1,	 1'350,	 1'800,	3,	 4,	 4 },
	{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	14,	16,	17,	20,	0,	0,	 22,	IA_JEWEL | IA_ARMOR | IA_SHIELD | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'300,	 4'000,	4,	11,	14 },
	{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	21,	25,	26,	30,	0,	0,	 40,	IA_JEWEL | IA_ARMOR | IA_SHIELD | IA_FLASK,	IS_ALL,	0,	0,	1,	 8'400,	12'000,	4,	26,	30 },
	{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	31,	35,	36,	40,	0,	0,	 70,	IA_JEWEL | IA_ARMOR | IA_FLASK,				IS_ALL,	0,	0,	1,	24'800,	32'000,	4,	38,	41 },
	{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	41,	45,	46,	50,	0,	0,	129,	IA_JEWEL | IA_ARMOR | IA_FLASK,				IS_ALL,	0,	0,	1,	32'800,	40'000,	4,	70,	75 },
	{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	51,	60,	71,	80,	0,	0,	174,	IA_JEWEL | IA_ARMOR | IA_FLASK,				IS_ALL,	0,	0,	0,	41'000,	50'000,	5,	76,	90 },

	
	/* ------------------------------------
				  	 STUN
	-------------------------------------*/
	
	{ 387,	"Firmness",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	  1,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	   100,	   100,	 1,	 0,	  0 },
	{ 388,	"Firmness",			AE_STUN_RESIST,	0,	 2,	 2,	0,	0,	0,	0,	  5,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	   500,	   500,	 2,	 1,	  2 },
	{ 389,	"Stiffness",		AE_STUN_RESIST,	0,	 3,	 3,	0,	0,	0,	0,	 12,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	 1'300,	 1'300,	 3,	 4,	  5 },
	{ 390,	"Stiffness",		AE_STUN_RESIST,	0,	 4,	 4,	0,	0,	0,	0,	 20,	IA_SHIELD | IA_ARMOR,														IS_ALL,		0,	0,	1,	 4'000,	 4'000,	 4,	 9,	 11 },
	{ 391,	"Rigidity",			AE_STUN_RESIST,	0,	 5,	 6,	0,	0,	0,	0,	 32,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	 7'500,	 9'000,	 7,	19,	 23 },
	{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	 7,	 8,	0,	0,	0,	0,	 54,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	11'000,	12'000,	 8,	31,	 36 },
	{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	 9,	10,	0,	0,	0,	0,	 81,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	14'000,	16'000,	 9,	44,	 51 },
	{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	11,	12,	0,	0,	0,	0,	100,	IA_SHIELD | IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BELT | IA_BOOTS,	IS_ALL,		0,	0,	1,	17'000,	19'000,	10,	53,	 59 },
	{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	13,	14,	0,	0,	0,	0,	120,	IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM | IA_BOOTS,							IS_BATTLE,	0,	0,	1,	21'000,	22'000,	11,	69,	 76 },
	{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	15,	16,	0,	0,	0,	0,	142,	IA_MID_ARMOR | IA_HEAVY_ARMOR | IA_HELM,									IS_BATTLE,	0,	0,	1,	23'000,	24'000,	12,	79,	 86 },
	{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	17,	20,	0,	0,	0,	0,	178,	IA_MID_ARMOR | IA_HEAVY_ARMOR,												IS_BATTLE,	0,	0,	0,	34'000,	40'000,	15,	99,	113 },

	
	/* ------------------------------------
					  MF
	-------------------------------------*/
	
	{ 393,	"Chance",	AE_MAGIC_FIND,	0,	 5,	10,	0,	0,	0,	0,	  2,	IA_JEWEL|IA_ARMOR|IA_FLASK|IA_TRAP,																IS_ALL,	0,	0,	1,	   100,	   200,	 2,	 0,	 0 },
	{ 394,	"Bounty",	AE_MAGIC_FIND,	0,	10,	15,	0,	0,	0,	0,	  7,	IA_JEWEL|IA_ARMOR|IA_FLASK|IA_TRAP,																IS_ALL,	0,	0,	1,	   400,	   600,	 3,	 0,	 0 },
	{ 395,	"Fortune",	AE_MAGIC_FIND,	0,	15,	20,	0,	0,	0,	0,	 14,	IA_JEWEL|IA_ARMOR|IA_FLASK|IA_TRAP,																IS_ALL,	0,	0,	1,	 1'500,	 2'000,	 4,	 3,	 5 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	20,	25,	0,	0,	0,	0,	 26,	IA_JEWEL|IA_LIGHT_ARMOR|IA_MID_ARMOR|IA_HELM|IA_BELT|IA_GLOVE|IA_BOOTS|IA_FLASK,				IS_ALL,	0,	0,	1,	 4'000,	 5'000,	 5,	12,	15 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	25,	30,	0,	0,	0,	0,	 45,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	 5'000,	 6'000,	 6,	30,	33 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	30,	35,	0,	0,	0,	0,	 65,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	 8'000,	10'000,	 7,	35,	38 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	35,	40,	0,	0,	0,	0,	 85,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	13'000,	16'000,	 8,	45,	48 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	40,	45,	0,	0,	0,	0,	108,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	1,	20'000,	24'000,	 9,	55,	58 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	45,	50,	0,	0,	0,	0,	128,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	0,	30'000,	35'000,	10,	65,	75 },
	{ 396,	"Luck",		AE_MAGIC_FIND,	0,	51,	55,	0,	0,	0,	0,	158,	IA_JEWEL | IA_LIGHT_ARMOR | IA_MID_ARMOR | IA_HELM | IA_BELT | IA_GLOVE | IA_BOOTS | IA_FLASK,	IS_ALL,	0,	0,	0,	37'000,	41'000,	12,	76,	79 },


//works but unused now	{ 0,	"Spell Level",	AE_SPELL_LEVEL,	PS_1_FIREBOLT,	1,	2,	0,	0,	0,	0,	11,	IA_STAFF,	IS_ALL,	0,	0,	1,	100,	100,	1,	1+2 },
//	{ 0,	"To Spell",	AE_SPELL_PERCENT,	0,	500,	500,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"Add Spell Damage",	AE_SPELL_DAMAGE,	500,	500,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"To Element",	AE_ELEMENT_PERCENT,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"Add Element Damage",	AE_ELEMENT_DAMAGE,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


	/* ------------------------------------
				  FLAT THORNS
	-------------------------------------*/

	{ 397,	"Burs",		AE_ACTION,	EA_THORNS,	  1,	  2,	  3,	  4,	51,	 60,	  1,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	{ 398,	"Prickles",	AE_ACTION,	EA_THORNS,	  5,	  6,	  7,	  8,	36,	 45,	  3,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   200,	   200,	 2,	  0,	  0 },
	{ 399,	"Bramble",	AE_ACTION,	EA_THORNS,	  4,	  5,	  7,	 10,	76,	 90,	  8,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   500,	   500,	 3,	  2,	  3 },
	{ 400,	"Teeth",	AE_ACTION,	EA_THORNS,	 27,	 38,	 48,	 56,	21,	 30,	 15,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'000,	 1'100,	 4,	  4,	  6 },
	{ 401,	"Barbs",	AE_ACTION,	EA_THORNS,	 17,	 18,	 19,	 20,	91,	100,	 26,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 2'300,	 2'500,	 5,	  9,	 11 },
	{ 402,	"Thorns",	AE_ACTION,	EA_THORNS,	 33,	 36,	 40,	 45,	61,	 70,	 34,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'300,	 3'500,	 6,	 13,	 15 },
	{ 403,	"Spikes",	AE_ACTION,	EA_THORNS,	 60,	 70,	 85,	105,	41,	 55,	 42,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 6'300,	 6'500,	 7,	 17,	 19 },
	{ 404,	"Razors",	AE_ACTION,	EA_THORNS,	 62,	 72,	 80,	 85,	71,	 80,	 51,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	10'500,	11'500,	 8,	 21,	 23 },
	{ 405,	"Lances",	AE_ACTION,	EA_THORNS,	515,	550,	650,	750,	11,	 15,	 59,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	21'500,	22'500,	 9,	 25,	 28 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	100,	110,	115,	145,	81,	 90,	 75,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	31'500,	32'500,	10,	 41,	 44 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	250,	270,	280,	340,	41,	 45,	 95,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	42'500,	44'500,	11,	 55,	 60 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	150,	165,	175,	200,	81,	 90,	115,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	53'500,	55'500,	12,	 66,	 69 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	675,	750,	825,	900,	27,	 30,	135,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	58'500,	58'500,	13,	 74,	 81 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	650,	700,	750,	800,	41,	 45,	155,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	63'500,	64'500,	14,	 86,	 93 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	425,	450,	475,	500,	81,	 90,	175,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	68'500,	70'500,	15,	 97,	105 },
	{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	525,	550,	575,	600,	81,	 90,	178,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	0,	75'500,	80'500,	16,	106,	110 },


//	{ 0,	"To Thorns",	AE_ACTION_PERCENT,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"Add Thorns",	AE_ACTION_VALUE,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


	/* ------------------------------------
					OTHER
	-------------------------------------*/

	{ 407,	"Apprentice",	AE_CAST_SPEED,	0,	1,	1,	0,	0,	0,	0,	 30,	IA_JEWEL|IA_STAFF,	IS_MAGIC,	0,	0,	1,	 3'000,	 3'000,	 4,	17,	19 },
	{ 408,	"Magus",		AE_CAST_SPEED,	0,	2,	2,	0,	0,	0,	0,	 58,	IA_JEWEL|IA_STAFF,	IS_MAGIC,	0,	0,	1,	 8'000,	 8'000,	 8,	31,	34 },
	{ 408,	"Magus",		AE_CAST_SPEED,	0,	2,	2,	0,	0,	0,	0,	 97,	IA_JEWEL|IA_STAFF,	IS_MAGIC,	0,	0,	1,	10'000,	10'000,	 9,	48,	55 },
	{ 409,	"Dominus",		AE_CAST_SPEED,	0,	3,	3,	0,	0,	0,	0,	114,	IA_JEWEL|IA_STAFF,	IS_MAGIC,	0,	0,	0,	14'000,	14'000,	12,	56,	72 },
	
	{ 410,	"the Raven",	AE_LIFE_REGEN,	0,	 1,	 2,	0,	0,	0,	0,	  3,	IA_JEWEL|IA_SHIELD|IA_ARMOR,		IS_BATTLE,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 411,	"the Owl",		AE_LIFE_REGEN,	0,	 3,	 4,	0,	0,	0,	0,	  9,	IA_JEWEL|IA_SHIELD|IA_ARMOR,		IS_BATTLE,	0,	0,	1,	 1'200,	 1'800,	3,	 2,	 2 },
	{ 412,	"the Hawk",		AE_LIFE_REGEN,	0,	 5,	 6,	0,	0,	0,	0,	 20,	IA_JEWEL|IA_SHIELD|IA_ARMOR,		IS_BATTLE,	0,	0,	1,	 4'000,	 4'800,	4,	 5,	11 },
	{ 413,	"the Falcon",	AE_LIFE_REGEN,	0,	 7,	 8,	0,	0,	0,	0,	 32,	IA_JEWEL|IA_SHIELD|IA_ARMOR,		IS_BATTLE,	0,	0,	1,	 7'000,	 8'000,	5,	13,	19 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	 9,	10,	0,	0,	0,	0,	 49,	IA_JEWEL|IA_SHIELD|IA_ARMOR,		IS_BATTLE,	0,	0,	1,	 9'000,	10'000,	6,	20,	30 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	11,	15,	0,	0,	0,	0,	 87,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	11'000,	15'000,	7,	47,	52 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	16,	20,	0,	0,	0,	0,	121,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	1,	16'000,	20'000,	8,	66,	75 },
	{ 414,	"the Eagle",	AE_LIFE_REGEN,	0,	21,	25,	0,	0,	0,	0,	175,	IA_JEWEL | IA_SHIELD | IA_ARMOR,	IS_BATTLE,	0,	0,	0,	21'000,	25'000,	9,	89,	99 },

	{ 415,	"the Gecko",	AE_MANA_REGEN,	0,	 1,	 2,	0,	0,	0,	0,	  3,	IA_JEWEL|IA_STAFF|IA_SHIELD|IA_ARMOR|IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	   300,	   600,	2,	 0,	 0 },
	{ 416,	"the Lizard",	AE_MANA_REGEN,	0,	 3,	 4,	0,	0,	0,	0,	  9,	IA_JEWEL|IA_STAFF|IA_SHIELD|IA_ARMOR|IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	 1'200,	 1'800,	3,	 2,	 2 },
	{ 417,	"the Turtle",	AE_MANA_REGEN,	0,	 5,	 6,	0,	0,	0,	0,	 20,	IA_JEWEL|IA_STAFF|IA_SHIELD|IA_ARMOR|IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	 4'000,	 4'800,	4,	 5,	11 },
	{ 418,	"the Wyrm",		AE_MANA_REGEN,	0,	 7,	 8,	0,	0,	0,	0,	 32,	IA_JEWEL|IA_STAFF|IA_SHIELD|IA_ARMOR|IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	 7'000,	 8'000,	5,	13,	19 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	 9,	10,	0,	0,	0,	0,	 49,	IA_JEWEL|IA_STAFF|IA_SHIELD|IA_ARMOR|IA_SWORD_1H,			IS_MAGIC,	0,	0,	1,	 9'000,	10'000,	6,	20,	30 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	11,	15,	0,	0,	0,	0,	 87,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	11'000,	15'000,	7,	47,	52 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	16,	20,	0,	0,	0,	0,	121,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	1,	16'000,	20'000,	8,	66,	75 },
	{ 419,	"the Dragon",	AE_MANA_REGEN,	0,	21,	25,	0,	0,	0,	0,	175,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR | IA_SWORD_1H,	IS_MAGIC,	0,	0,	0,	21'000,	25'000,	9,	89,	99 },


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

	{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	 1,	 2,	0,	0,	  1,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	    50,	   200,	 1,	 0,	 0 },
	{ 429,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 2,	 2,	 2,	 3,	0,	0,	  4,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   250,	   500,	 2,	 0,	 0 },
	{ 430,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 3,	 3,	 4,	 5,	0,	0,	 11,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   600,	 1'000,	 3,	 3,	 5 },
	{ 431,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 4,	 5,	 6,	 8,	0,	0,	 25,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 3'500,	 5'000,	 4,	15,	18 },
	{ 432,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 5,	 6,	 7,	10,	0,	0,	 55,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	30,	34 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 8,	10,	12,	14,	0,	0,	 67,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	20'000,	20'000,	 8,	40,	44 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	15,	17,	18,	20,	0,	0,	 92,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	30'000,	33'000,	 9,	51,	54 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	21,	22,	23,	25,	0,	0,	112,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	35'000,	40'000,	10,	58,	63 },
	{ 433,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	26,	27,	28,	30,	0,	0,	132,	IA_STAFF|IA_SHIELD|IA_SWORD_1H,	IS_SUMMONING,	0,	0,	0,	41'000,	45'000,	11,	66,	70 },
		//jewelry
	{ 428,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 1,	 1,	 1,	 1,	0,	0,	  3,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	    150,	   150,	 1,	 0,	 0 },
	{ 428,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 2,	 2,	 2,	 2,	0,	0,	  9,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	    750,	   750,	 3,	 3,	 4 },
		//+magic gives more, so these fall behind at this point
	//{ 431,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 3,	 3,	 3,	 3,	0,	0,	 18,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 3'500,	 5'000,	 4,	15,	18 },
	//{ 432,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 4,	 4,	 5,	 5,	0,	0,	 40,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	30,	34 },
	//{ 433,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 6,	 6,	 7,	 7,	0,	0,	 61,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	20'000,	20'000,	 8,	40,	44 },
	//{ 433,	"the Horde",		AE_SUMMON_DAMAGE,	0,	 8,	 8,	 9,	10,	0,	0,	 79,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	30'000,	33'000,	 9,	51,	54 },
	//{ 433,	"the Horde",		AE_SUMMON_DAMAGE,	0,	10,	11,	11,	12,	0,	0,	102,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	35'000,	40'000,	10,	58,	63 },
	//{ 433,	"the Horde",		AE_SUMMON_DAMAGE,	0,	13,	14,	14,	15,	0,	0,	122,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	41'000,	45'000,	11,	66,	70 },
	//{ 433,	"the Horde",		AE_SUMMON_DAMAGE,	0,	16,	17,	17,	18,	0,	0,	146,	IA_JEWEL,	IS_SUMMONING,	0,	0,	0,	51'000,	55'000,	12,	76,	80 },

	{ 434,	"Thickening",	AE_SUMMON_HP,	0,	  5,	 10,	0,	0,	0,	0,	  1,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	    50,	   200,	 1,	 0,	 0 },
	{ 435,	"Thickening",	AE_SUMMON_HP,	0,	 10,	 20,	0,	0,	0,	0,	  4,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	   250,	   500,	 2,	 0,	 0 },
	{ 436,	"Thickening",	AE_SUMMON_HP,	0,	 20,	 40,	0,	0,	0,	0,	 11,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	   600,	 1'200,	 3,	 4,	 5 },
	{ 437,	"Thickening",	AE_SUMMON_HP,	0,	 50,	 70,	0,	0,	0,	0,	 25,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	 2'500,	 5'000,	 4,	15,	18 },
	{ 438,	"Thickening",	AE_SUMMON_HP,	0,	 80,	100,	0,	0,	0,	0,	 55,	IA_ARMOR,		IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	31,	36 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	120,	140,	0,	0,	0,	0,	 68,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	20'000,	20'000,	 8,	40,	45 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	160,	200,	0,	0,	0,	0,	 92,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	30'000,	30'000,	 9,	51,	55 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	220,	250,	0,	0,	0,	0,	112,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	1,	34'000,	40'000,	10,	58,	63 },
	{ 439,	"Thickening",	AE_SUMMON_HP,	0,	270,	300,	0,	0,	0,	0,	132,	IA_BODY_ARMOR,	IS_SUMMONING,	0,	0,	0,	41'000,	45'000,	11,	66,	70 },

	{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 2,	0,	0,	0,	0,	  1,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	    50,	   100,	 1,	 0,	 0 },
	{ 441,	"Guarding",	AE_SUMMON_AC,	0,	 3,	 5,	0,	0,	0,	0,	  4,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   300,	   500,	 2,	 0,	 0 },
	{ 442,	"Guarding",	AE_SUMMON_AC,	0,	 6,	10,	0,	0,	0,	0,	 11,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   600,	 1'000,	 3,	 3,	 5 },
	{ 443,	"Guarding",	AE_SUMMON_AC,	0,	11,	15,	0,	0,	0,	0,	 25,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 2'200,	 3'000,	 4,	16,	19 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	16,	20,	0,	0,	0,	0,	 55,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	 6,	30,	35 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	21,	25,	0,	0,	0,	0,	 92,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	21'000,	25'000,	 8,	51,	55 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	26,	30,	0,	0,	0,	0,	112,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	30'000,	35'000,	 9,	57,	63 },
	{ 444,	"Guarding",	AE_SUMMON_AC,	0,	31,	35,	0,	0,	0,	0,	132,	IA_ARMOR,	IS_SUMMONING,	0,	0,	0,	36'000,	40'000,	10,	66,	70 },


	/* ------------------------------------
					CRITS
	-------------------------------------*/

	{ 445,	"Supremacy",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 1,	 1,	  1,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
	{ 446,	"Supremacy",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 2,	 3,	  3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   200,	   300,	 1,	 0,	 0 },
	{ 447,	"Expertise",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 4,	 5,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   800,	 1'200,	 2,	 3,	 4 },
	{ 448,	"Expertise",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 6,	 7,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 2'800,	 3'600,	 4,	19,	21 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	 8,	10,	 48,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	10'000,	12'000,	 6,	31,	35 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	11,	12,	 88,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	15'000,	15'000,	 8,	51,	57 },
	{ 449,	"Excellence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	13,	14,	128,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	25'000,	25'000,	11,	70,	79 },
	{ 449,	"Preeminence",	AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	15,	16,	176,	IA_RANGE | IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	30'000,	32'000,	15,	80,	93 },

	{ 450,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	 71,	 90,	0,	0,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 5 },
	{ 451,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	 91,	110,	0,	0,	0,	0, 	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	4,	13,	21 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	111,	130,	0,	0,	0,	0,	 54,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	30,	35 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	131,	150,	0,	0,	0,	0,	 88,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	17'000,	17'000,	7,	50,	57 },
	{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	151,	175,	0,	0,	0,	0,	124,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	27'000,	27'000,	9,	75,	80 },

	{ 453,	"Wrack",		AE_CRIT_PERCENT,	0,	36,	 45,	0,	0,	1,	2,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 5 },
	{ 454,	"Disruption",	AE_CRIT_PERCENT,	0,	46,	 55,	0,	0,	3,	4,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	4,	 9,	11 },
	{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	56,	 65,	0,	0,	5,	6,	 54,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	27,	33 },
	{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	66,	 75,	0,	0,	7,	7,	 95,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	52,	58 },
	{ 455,	"Destruction",	AE_CRIT_PERCENT,	0,	76,	100,	0,	0,	8,	8,	102,	IA_RANGE | IA_STAFF | IA_MELEE | IA_TRAP | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	0,	27'000,	27000,	9,	61,	65 },

	{ 456,	"Smite",		AE_CRIT_DAMAGE,	0,	  10,	  20,	  30,	  40,	0,	0,	  3,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	   100,	   100,	 1,	 0,	 0 },
	{ 457,	"Justice",		AE_CRIT_DAMAGE,	0,	  25,	  35,	  50,	  60,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 1'100,	 1'100,	 2,	 3,	 4 },
	{ 458,	"Punishment",	AE_CRIT_DAMAGE,	0,	  55,	  75,	 110,	 130,	0,	0,	 23,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 3'000,	 3'000,	 4,	15,	19 },
	{ 459,	"Retaliation",	AE_CRIT_DAMAGE,	0,	 130,	 160,	 240,	 270,	0,	0,	 38,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	 6'000,	 6'000,	 6,	27,	29 },
	{ 460,	"Oblation",		AE_CRIT_DAMAGE,	0,	 260,	 320,	 480,	 540,	0,	0,	 53,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	10'000,	10'000,	 8,	31,	34 },
	{ 461,	"Retribution",	AE_CRIT_DAMAGE,	0,	 520,	 580,	 740,	 800,	0,	0,	 60,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,	IS_BATTLE,	0,	0,	1,	16'000,	16'000,	10,	35,	37 },
//1h
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 600,	 700,	 800,	900,	0,	0,	 89,	IA_SWORD_1H | IA_MACE_1H,											IS_BATTLE,	0,	0,	1,	25'000,	25'000,	12,	53,	61 },
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 700,	 800,	 900,	1000,	0,	0,	162,	IA_SWORD_1H | IA_MACE_1H,											IS_BATTLE,	0,	0,	0,	35'000,	35'000,	14,	84,	89 },
//2h
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 700,	 800,	 900,	1000,	0,	0,	 90,	IA_RANGE | IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,			IS_BATTLE,	0,	0,	1,	25'000,	25'000,	12,	51,	58 },
	{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	1000,	1200,	1300,	1500,	0,	0,	163,	IA_RANGE | IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H | IA_PISTOL,			IS_BATTLE,	0,	0,	0,	35'000,	35'000,	14,	85,	98 },

	
	
	/* ------------------------------------
			 LIFE & MANA PERCENT
	-------------------------------------*/
	
	{ 463,	"Vis",			AE_LIFE_PERCENT,	0,	10,	12,	0,	0,	0,	0,	 16,	IA_ARMOR,						IS_BATTLE,	0,	0,	1,	   800,	 1'600,	 3,	 5,	  7 },
	{ 464,	"Prowess",		AE_LIFE_PERCENT,	0,	13,	15,	0,	0,	0,	0,	 27,	IA_ARMOR,						IS_BATTLE,	0,	0,	1,	 2'100,	 2'500,	 5,	15,	 21 },
	{ 465,	"Valour",		AE_LIFE_PERCENT,	0,	16,	20,	0,	0,	0,	0,	 51,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	 5'200,	 6'000,	 7,	24,	 31 },
	{ 465,	"Valour",		AE_LIFE_PERCENT,	0,	16,	20,	0,	0,	0,	0,	 92,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	1,	 8'200,	 9'000,	 9,	46,	 54 },
	{ 465,	"Heroism",		AE_LIFE_PERCENT,	0,	21,	25,	0,	0,	0,	0,	178,	IA_MID_ARMOR | IA_HEAVY_ARMOR,	IS_BATTLE,	0,	0,	0,	21'000,	25'000,	12,	99,	119 },

	{ 466,	"Nous",			AE_MANA_PERCENT,	0,	10,	12,	0,	0,	0,	0,	 16,	IA_ARMOR,		IS_MAGIC,	0,	0,	1,	   800,	 1'600,	 3,	 5,	  7 },
	{ 467,	"Intellect",	AE_MANA_PERCENT,	0,	13,	15,	0,	0,	0,	0,	 27,	IA_ARMOR,		IS_MAGIC,	0,	0,	1,	 2'100,	 2'500,	 5,	15,	 21 },
	{ 468,	"Awakening",	AE_MANA_PERCENT,	0,	16,	20,	0,	0,	0,	0,	 51,	IA_HELM,		IS_MAGIC,	0,	0,	1,	 5'200,	 6'000,	 7,	24,	 31 },
	{ 468,	"Awakening",	AE_MANA_PERCENT,	0,	16,	20,	0,	0,	0,	0,	 92,	IA_HELM,		IS_MAGIC,	0,	0,	1,	 8'200,	 9'000,	 9,	46,	 54 },
	{ 468,	"Afflatus",		AE_MANA_PERCENT,	0,	21,	25,	0,	0,	0,	0,	178,	IA_HELM,		IS_MAGIC,	0,	0,	0,	21'000,	25'000,	12,	99,	120 },

	
	/* ------------------------------------
		   NONSENSE !!! << REWORK!
	-------------------------------------*/
	
	{ 469,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
	{ 470,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
	{ 471,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

	{ 472,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
	{ 473,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
	{ 474,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

	{ 475,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
	{ 476,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
	{ 477,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

	{ 478,	"Arcane Stars",		AE_SPELL_DAMAGE,	PS_35_ARCANE_STAR,	4,	8,	12,	16,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
	{ 479,	"Arcane Stars",		AE_SPELL_DAMAGE,	PS_35_ARCANE_STAR,	8,	16,	24,	32,	0,	0,	18,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 12 },

	{ 480,	"Lightning",		AE_SPELL_DAMAGE,	PS_3_LIGHTNING,		2,	3,	4,	5,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
	{ 481,	"Lightning",		AE_SPELL_DAMAGE,	PS_3_LIGHTNING,		4,	8,	10,	12,	0,	0,	18,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 13 },

	{ 482,	"Incineration",		AE_SPELL_DAMAGE,	PS_20_INCINERATE,	2,	3,	4,	5,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
	{ 483,	"Incineration",		AE_SPELL_DAMAGE,	PS_20_INCINERATE,	4,	8,	10,	12,	0,	0,	19,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 13 },

//	{ 0,	"To Weapon",	AE_WEAPON_PERCENT,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"To Attack Speed",	AE_ATTACK_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"To Cast Speed",	AE_CAST_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
	
	{ 0,	"",	AE_NO }, 
};
// next new affix id = ?
