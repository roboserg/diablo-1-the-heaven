#include "stdafx.h"

// Affix array at 0x0048B820
Affix AffixesSigil[] = {
	// Name,	Effects,	type,	minLow,	minHigh,	maxLow,	maxHigh,	chanceLow,	chanceHigh,	QualityLevel,	item,	IS_ALL,	ExcludedCombinations,	DoubleChance,	NotCursed,	MinGoldValue,	MaxGoldValue,	Multiplier, CLVL req min,	CLVL req max



		/* ------------------------------------
		-------------- PREFIXES ---------------
		-------------------------------------*/

		/* ------------------------------------
						TO_HIT
		-------------------------------------*/

		{ 1,	"Copper",		AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  1,	  1,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 2,	"Tin",			AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  1,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 3,	"Brass",		AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  1,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   100,	   100,	 3,	 0,	 0 },
		{ 4,	"Bronze",		AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  1,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   150,	   150,	 4,	 0,	 0 },
		{ 5,	"Iron",			AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  1,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   200,	   200,	 5,	 0,	 0 },
		{ 6,	"Steel",		AE_TO_HIT,	0,	0,	0,	0,	0,	  1,	  2,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   250,	   250,	 6,	 0,	 0 },
		{ 7,	"Silver",		AE_TO_HIT,	0,	0,	0,	0,	0,	  2,	  2,	 46,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   300,	   300,	 7,	 0,	 0 },
		{ 9,	"Platinum",		AE_TO_HIT,	0,	0,	0,	0,	0,	  2,	  3,	 62,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   350,	   350,	 8,	 0,	 0 },
		{ 487,	"Extravagant",	AE_TO_HIT,	0,	0,	0,	0,	0,	  3,	  3,	 92,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   400,	   400,	 9,	 0,	 0 },
		{ 489,	"Meteoric",		AE_TO_HIT,	0,	0,	0,	0,	0,	  3,	  4,	112,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   500,	   500,	10,	 0,	 0 },
		{ 491,	"Superior",		AE_TO_HIT,	0,	0,	0,	0,	0,	  4,	  4,	132,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   600,	   600,	11,	 0,	 0 },
		{ 493,	"Guiding",		AE_TO_HIT,	0,	0,	0,	0,	0,	  4,	  5,	152,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   700,	   700,	12,	 0,	 0 },

		/* ------------------------------------
			DAMAGE PERCENT, ON WEAPONS
		-------------------------------------*/

		{  27,	"Fierce",	AE_PERCENT_DAMAGE,	0,	3,	3,	0,	0,	0,	0,	 40,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	  4,	 0,	 0 },
		{  18,	"Cruel",	AE_PERCENT_DAMAGE,	0,	3,	4,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   400,	  6,	 0,	 0 },
		{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	4,	5,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   500,	   600,	  8,	 0,	 0 },
		{  18,	"Ravaging",	AE_PERCENT_DAMAGE,	0,	5,	5,	0,	0,	0,	0,	160,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   700,	   700,	 10,	 0,	 0 },
				
		/* ------------------------------------
					DFE ON JEWELRY
		-------------------------------------*/

		{  12,	"Neophyte's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 14,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   100,	   100,	  2,	 0,	 0 },
		{  12,	"Neophyte's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 24,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   150,	   150,	  3,	 0,	 0 },
		{  12,	"Neophyte's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 42,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   200,	   200,	  4,	 0,	 0 },
		{  13,	"Warden's",		AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	 80,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   300,	   300,	  5,	 0,	 0 },
		{  13,	"Sentinel's",	AE_DFE,	0,	 -1,	 -1,	0,	0,	0,	0,	140,	IA_RING,	IS_ALL | IS_SUMMONING,	16,	0,	1,	   400,	   400,	  6,	 0,	 0 },
		
		/* ------------------------------------
					VARIOUS
		-------------------------------------*/


		
		/* ------------------------------------
				MAGIC RESISTANCES
		-------------------------------------*/

		// fire
		{ 62,	"Carnelian",		AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 62,	"Rhodochrosite",	AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
		{ 63,	"Clinohumite",		AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 63,	"Vanadinite",		AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 63,	"Jasper",			AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 32,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   175,	   175,	 3,	 0,	 0 },
		{ 64,	"Garnet",			AE_RESIST,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 55,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	 3,	 0,	 0 },
		{ 64,	"Cinnabar",			AE_RESIST,	ET_1_FIRE,	  1,	  2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   250,	   250,	 4,	 0,	 0 },
		{ 64,	"Rubellite",		AE_RESIST,	ET_1_FIRE,	  2,	  2,	0,	0,	0,	0,	 90,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   300,	 5,	 0,	 0 },
		{ 64,	"Hessonite",		AE_RESIST,	ET_1_FIRE,	  2,	  3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   350,	   350,	 6,	 0,	 0 },
		{ 64,	"Spessarite",		AE_RESIST,	ET_1_FIRE,	  2,	  3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   400,	   500,	 7,	 0,	 0 },
		{ 64,	"Ruby",				AE_RESIST,	ET_1_FIRE,	  3,	  3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   600,	   600,	 8,	 0,	 0 },

		// lite
		{ 62,	"Chalcanthite",	AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 62,	"Chalcedony",	AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
		{ 63,	"Benitoite",	AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 63,	"Lazurite",		AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 63,	"Azurite",		AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 32,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   175,	   175,	 3,	 0,	 0 },
		{ 64,	"Lapis",		AE_RESIST,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 55,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	 3,	 0,	 0 },
		{ 64,	"Cobalt",		AE_RESIST,	ET_2_LIGHTNING,	  1,	  2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   250,	   250,	 4,	 0,	 0 },
		{ 64,	"Larimar",		AE_RESIST,	ET_2_LIGHTNING,	  2,	  2,	0,	0,	0,	0,	 90,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   300,	 5,	 0,	 0 },
		{ 64,	"Kyanite",		AE_RESIST,	ET_2_LIGHTNING,	  2,	  3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   350,	   350,	 6,	 0,	 0 },
		{ 64,	"Spectrolite",	AE_RESIST,	ET_2_LIGHTNING,	  2,	  3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   400,	   500,	 7,	 0,	 0 },
		{ 64,	"Sapphire",		AE_RESIST,	ET_2_LIGHTNING,	  3,	  3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   600,	   600,	 8,	 0,	 0 },

		// arcane
		{ 62,	"Poudretteite",	AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 62,	"Lepidolite",	AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
		{ 63,	"Purpurite",	AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 63,	"Taaffeite",	AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 63,	"Pezzottaite",	AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 32,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   175,	   175,	 3,	 0,	 0 },
		{ 64,	"Kunzite",		AE_RESIST,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 55,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	 3,	 0,	 0 },
		{ 64,	"Iolite",		AE_RESIST,	ET_3_ARCAN,	  1,	  2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   250,	   250,	 4,	 0,	 0 },
		{ 64,	"Charoite",		AE_RESIST,	ET_3_ARCAN,	  2,	  2,	0,	0,	0,	0,	 90,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   300,	 5,	 0,	 0 },
		{ 64,	"Sugilite",		AE_RESIST,	ET_3_ARCAN,	  2,	  3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   350,	   350,	 6,	 0,	 0 },
		{ 64,	"Alexandrite",	AE_RESIST,	ET_3_ARCAN,	  2,	  3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   400,	   500,	 7,	 0,	 0 },
		{ 64,	"Amethyst",		AE_RESIST,	ET_3_ARCAN,	  3,	  3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   600,	   600,	 8,	 0,	 0 },

		// acid
		{ 62,	"Prasiolite",	AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 62,	"Olivine",		AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
		{ 63,	"Malachite",	AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 63,	"Fuchsite",		AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 63,	"Beryl",		AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	 32,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   175,	   175,	 3,	 0,	 0 },
		{ 64,	"Turquoise",	AE_RESIST,	ET_4_ACID,	  1,	  1,	0,	0,	0,	0,	 55,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	 3,	 0,	 0 },
		{ 64,	"Aventurine",	AE_RESIST,	ET_4_ACID,	  1,	  2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   250,	   250,	 4,	 0,	 0 },
		{ 64,	"Epidote",		AE_RESIST,	ET_4_ACID,	  2,	  2,	0,	0,	0,	0,	 90,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   300,	 5,	 0,	 0 },
		{ 64,	"Jade",			AE_RESIST,	ET_4_ACID,	  2,	  3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   350,	   350,	 6,	 0,	 0 },
		{ 64,	"Peridot",		AE_RESIST,	ET_4_ACID,	  2,	  3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   400,	   500,	 7,	 0,	 0 },
		{ 64,	"Emerald",		AE_RESIST,	ET_4_ACID,	  3,	  3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   600,	   600,	 8,	 0,	 0 },

		// cold
		{ 62,	"Goshenite",	AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    50,	    50,	 1,	 0,	 0 },
		{ 62,	"Scolecite",	AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    75,	    75,	 1,	 0,	 0 },
		{ 63,	"Selenite",		AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   100,	   100,	 2,	 0,	 0 },
		{ 63,	"Magnesite",	AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   150,	   150,	 2,	 0,	 0 },
		{ 63,	"Ulexite",		AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	 32,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   175,	   175,	 3,	 0,	 0 },
		{ 64,	"Calcite",		AE_RESIST,	ET_6_COLD,	  1,	  1,	0,	0,	0,	0,	 55,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   200,	   200,	 3,	 0,	 0 },
		{ 64,	"Howlite",		AE_RESIST,	ET_6_COLD,	  1,	  2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   250,	   250,	 4,	 0,	 0 },
		{ 64,	"Natrolite",	AE_RESIST,	ET_6_COLD,	  2,	  2,	0,	0,	0,	0,	 90,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   300,	   300,	 5,	 0,	 0 },
		{ 64,	"Moonstone",	AE_RESIST,	ET_6_COLD,	  2,	  3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   350,	   350,	 6,	 0,	 0 },
		{ 64,	"Pearl",		AE_RESIST,	ET_6_COLD,	  2,	  3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   400,	   500,	 7,	 0,	 0 },
		{ 64,	"Diamond",		AE_RESIST,	ET_6_COLD,	  3,	  3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	   600,	   600,	 8,	 0,	 0 },

		// all	
		//{ 62,	"Andratite",	AE_RESIST_All,	0,	 2,	 4,	0,	0,	0,	0,	  7,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	    50,	    100,	 2,	  0,	  0 },
		//{ 62,	"Andalusite",	AE_RESIST_All,	0,	 6,	 8,	0,	0,	0,	0,	 11,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   300,	    600,	 2,	  0,	  0 },
		//{ 63,	"Zoisite",		AE_RESIST_All,	0,	 9,	11,	0,	0,	0,	0,	 17,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  3'000,	 3,	  2,	  3 },
		//{ 63,	"Andesine",		AE_RESIST_All,	0,	12,	15,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 3'000,	  4'000,	 3,	 12,	 15 },
		//{ 63,	"Ametrine",		AE_RESIST_All,	0,	16,	18,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 6'000,	  7'000,	 4,	 24,	 27 },
		//{ 64,	"Ammolite",		AE_RESIST_All,	0,	19,	21,	0,	0,	0,	0,	 61,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 9'000,	 10'000,	 4,	 31,	 35 },
		//{ 64,	"Pietersite",	AE_RESIST_All,	0,	22,	24,	0,	0,	0,	0,	 82,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	12'000,	 13'000,	 5,	 45,	 49 },
		//{ 64,	"Opal",			AE_RESIST_All,	0,	25,	27,	0,	0,	0,	0,	105,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	15'000,	 17'000,	 6,	 59,	 61 },
		//{ 64,	"Agate",		AE_RESIST_All,	0,	28,	30,	0,	0,	0,	0,	125,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	20'000,	 25'000,	 7,	 74,	 80 },
		//{ 64,	"Labradorite",	AE_RESIST_All,	0,	31,	35,	0,	0,	0,	0,	178,	IA_BODY_ARMOR,								IS_ALL,	0,	0,	0,	93'000,	105'000,	14,	104,	118 },


		/* ------------------------------------
					VARIOUS
		-------------------------------------*/

		// life steal
		//{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
		//{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
		//{ 83,	"Hungry",		AE_LIFE_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	52,	58 },

		// mana steal
		//{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	25,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	2'000,	2'000,	3,	14,	17 },
		//{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	48,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	4'000,	4'000,	4,	31,	33 },
		//{ 85,	"Vampiric",		AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0,	94,	IA_JEWEL,	IS_BATTLE,	0,	0,	1,	8'000,	8'000,	6,	52,	58 },

		{ 87,	"Curing",		AE_LIFE_REGEN_PERCENT,	0,	2,	2,	0,	0,	0,	0,	54,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
		{ 87,	"Healing",		AE_LIFE_REGEN_PERCENT,	0,	3,	3,	0,	0,	0,	0,	94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 4,	 0,	 0 },

		{ 88,	"Refilling",	AE_MANA_REGEN_PERCENT,	0,	2,	2,	0,	0,	0,	0,	12,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
		{ 89,	"Replenishing",	AE_MANA_REGEN_PERCENT,	0,	3,	3,	0,	0,	0,	0,	94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 4,	 0,	 0 },

		
	// + damage on jewelry
	{ 92,	"Tearing",		AE_DAMAGE,	0,	1,	 1,	0,	0,	0,	0,	 10,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
	{ 93,	"Pounding",		AE_DAMAGE,	0,	1,	 1,	0,	0,	0,	0,	 20,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 3,	 0,	 0 },
	{ 94,	"Smashing",		AE_DAMAGE,	0,	1,	 1,	0,	0,	0,	0,	 30,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
	{ 95,	"Crushing",		AE_DAMAGE,	0,	1,	 1,	0,	0,	0,	0,	 45,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 4,	 0,	 0 },
	{ 95,	"Shattering",	AE_DAMAGE,	0,	1,	 1,	0,	0,	0,	0,	 65,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
	{ 95,	"Pulverizing",	AE_DAMAGE,	0,	1,	 2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 5,	 0,	 0 },
	{ 95,	"Decimating",	AE_DAMAGE,	0,	2,	 2,	0,	0,	0,	0,	 85,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 6,	 0,	 0 },
	{ 95,	"Breaking",		AE_DAMAGE,	0,	2,	 2,	0,	0,	0,	0,	 95,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 6,	 0,	 0 },
	{ 95,	"Demolishing",	AE_DAMAGE,	0,	2,	 3,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 7,	 0,	 0 },
	{ 95,	"Shattering",	AE_DAMAGE,	0,	2,	 3,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 7,	 0,	 0 },
	{ 95,	"Eliminating",	AE_DAMAGE,	0,	3,	 3,	0,	0,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    750,	    750,	 8,	 0,	 0 },

	// ac in jewelry
	{ 99,	"Shielding",	AE_SPECIAL_AC,	0,	1,	1,	0,	0,	0,	0,	 16,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
	{ 100,	"Covering",		AE_SPECIAL_AC,	0,	1,	1,	0,	0,	0,	0,	 29,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	1,	1,	0,	0,	0,	0,	 50,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	1,	2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	2,	2,	0,	0,	0,	0,	 95,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
	{ 101,	"Armored",		AE_SPECIAL_AC,	0,	2,	2,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


	/* ------------------------------------
		ELEMENTAL DAMAGE ON WEAPONS
	-------------------------------------*/

	//{ 102,	"Warm",			AE_ELEM_HIT,	EA_MELEE_FIRE,	 1,	 2,	 3,	 4,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	//{ 103,	"Melting",		AE_ELEM_HIT,	EA_MELEE_FIRE,	 3,	 4,	 6,	 7,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	//{ 104,	"Hot",			AE_ELEM_HIT,	EA_MELEE_FIRE,	 6,	 8,	12,	14,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	//{ 105,	"Candent",		AE_ELEM_HIT,	EA_MELEE_FIRE,	11,	14,	16,	19,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	//{ 106,	"Burning",		AE_ELEM_HIT,	EA_MELEE_FIRE,	12,	16,	22,	26,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	//{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	24,	32,	44,	52,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	//{ 107,	"Incinerating",	AE_ELEM_HIT,	EA_MELEE_FIRE,	24,	32,	44,	52,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 108,	"Static",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	//{ 109,	"Electric",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 1,	 2,	10,	11,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	//{ 110,	"Charged",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 3,	 5,	19,	21,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	//{ 111,	"Shocking",		AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 5,	 8,	28,	31,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	//{ 112,	"Lightning",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	 7,	11,	37,	41,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	//{ 113,	"Thundering",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	14,	22,	74,	82,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	//{ 113,	"Thundering",	AE_ELEM_HIT,	EA_MELEE_LIGHTNING,	14,	22,	74,	82,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 114,	"Chilling",	AE_ELEM_HIT,	EA_MELEE_COLD,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
	//{ 115,	"Snow",		AE_ELEM_HIT,	EA_MELEE_COLD,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 },
	//{ 116,	"Icy",		AE_ELEM_HIT,	EA_MELEE_COLD,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 },
	//{ 117,	"Winter",	AE_ELEM_HIT,	EA_MELEE_COLD,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 },
	//{ 118,	"Glacial",	AE_ELEM_HIT,	EA_MELEE_COLD,	 9,	13,	35,	39,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 },
	//{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	24,	32,	64,	72,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 },
	//{ 119,	"Freezing",	AE_ELEM_HIT,	EA_MELEE_COLD,	24,	32,	64,	72,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 120,	"Cleansing",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 1,	 1,	 2,	 3,	0,	0,	 1,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 }, // 7
	//{ 121,	"Purifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 24 +100%
	//{ 122,	"Cleric's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 48 + 100%
	//{ 123,	"Priest's",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 72 + 50
	//{ 124,	"Purging",		AE_ELEM_HIT,	EA_MELEE_HOLY,	 9,	13,	35,	39,	0,	0,	55,	IA_STAFF | IA_AXE | IA_MACE,	IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 96 + 33%
	//{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	24,	32,	64,	72,	0,	0,	62,	IA_STAFF | IA_AXE | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 192
	//{ 125,	"Sanctifying",	AE_ELEM_HIT,	EA_MELEE_HOLY,	24,	32,	64,	72,	0,	0,	99,	IA_MACE_1H,					IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 126,	"Voiding",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	 2,	 3,	 9,	10,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 24
	//{ 127,	"Depleting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 4,	 6,	18,	20,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 48
	//{ 128,	"Exhausting",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 6,	 9,	27,	30,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 72
	//{ 129,	"Devastating",	AE_ELEM_HIT,	EA_MELEE_ARCAN,	 9,	12,	36,	39,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,				IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 96
	//{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	21,	27,	53,	61,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 162!
	//{ 130,	"Eldritch",		AE_ELEM_HIT,	EA_MELEE_ARCAN,	21,	27,	53,	61,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,				IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },

	//{ 131,	"Rancid",			AE_ELEM_HIT,	EA_MELEE_ACID,	 2,	 3,	 7,	 8,	0,	0,	 4,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	   400,	   400,	1,	 0,	 0 }, // 20
	//{ 132,	"Toxic",			AE_ELEM_HIT,	EA_MELEE_ACID,	 4,	 6,	15,	17,	0,	0,	10,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 1'000,	 1'000,	2,	 3,	 4 }, // 42
	//{ 133,	"Diluent",			AE_ELEM_HIT,	EA_MELEE_ACID,	 5,	 7,	23,	26,	0,	0,	24,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 2'500,	 2'500,	3,	15,	18 }, // 61
	//{ 134,	"Corrodent",		AE_ELEM_HIT,	EA_MELEE_ACID,	12,	15,	25,	30,	0,	0,	55,	IA_STAFF | IA_MELEE | IA_CLAW,						IS_BATTLE,	0,	0,	1,	 5'000,	 5'000,	4,	30,	36 }, // 82
	//{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	18,	24,	48,	56,	0,	0,	62,	IA_STAFF | IA_AXE | IA_SWORD_2H | IA_MACE_2H,	IS_BATTLE,	0,	0,	1,	 7'000,	 7'000,	6,	37,	39 }, // 146!
	//{ 135,	"Disintegrating",	AE_ELEM_HIT,	EA_MELEE_ACID,	18,	24,	48,	56,	0,	0,	99,	IA_SWORD_1H | IA_MACE_1H,						IS_BATTLE,	0,	0,	1,	13'000,	13'000,	7,	55,	59 },



	/* ------------------------------------
				XP PERCENT
	-------------------------------------*/

	{ 138,	"Curious",			AE_XP_GAIN_PERCENT,	0,	 1,	 1,	 0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
	{ 139,	"Curious",			AE_XP_GAIN_PERCENT,	0,	 1,	 1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
	{ 140,	"Studying",			AE_XP_GAIN_PERCENT,	0,	 1,	 1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
	{ 141,	"Illuminating",		AE_XP_GAIN_PERCENT,	0,	 1,	 1,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 4,	 0,	 0 },
	{ 141,	"Contemplating",	AE_XP_GAIN_PERCENT,	0,	 1,	 2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
	{ 141,	"Clarifying",		AE_XP_GAIN_PERCENT,	0,	 2,	 2,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
	{ 141,	"Clarifying",		AE_XP_GAIN_PERCENT,	0,	 2,	 2,	0,	0,	0,	0,	125,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    750,	    750,	 7,	 0,	 0 },


	/* ------------------------------------
				GOLD FIND PERCENT
	-------------------------------------*/

	{ 142,	"Rapacious",	AE_GOLD_FIND,	0,	1,	1,	0,	0,	0,	0,	 11,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	1,	1,	0,	0,	0,	0,	 24,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	1,	1,	0,	0,	0,	0,	 46,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	1,	2,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    750,	 6,	 0,	 0 },
	{ 143,	"Covetous",		AE_GOLD_FIND,	0,	2,	2,	0,	0,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    850,	    850,	 7,	 0,	 0 },


	/* ------------------------------------
		MELEE & ARROW DAMAGE RESISTANCE
	-------------------------------------*/

	//Melee
{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 11,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 20,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 146,	"Vindicative",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 39,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 73,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 93,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	113,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 147,	"Consuming",	AE_ABSORB_MELEE_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	133,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

	// Arrow

{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 11,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 20,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 38,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 73,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	 93,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	113,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },
{ 148,	"Devouring",	AE_ABSORB_RANGED_DAMAGE,	0,	1,	1,	0,	0,	0,	0,	133,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

/* ------------------------------------
			SUMMON BOOSTS
-------------------------------------*/

//{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 1,	 3,	 1,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	    40,	   120,	1,	 0,	 0 },
//{ 156,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 3,	 4,	 3,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   150,	   200,	1,	 0,	 0 },
//{ 157,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 5,	 8,	 7,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	   500,	   800,	2,	 3,	 5 },
//{ 158,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	 9,	14,	13,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 1'800,	 2'800,	5,	 6,	 8 },
//{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	25,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	 7'500,	10'000,	6,	13,	17 },
//{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	47,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	16'500,	17'000,	7,	23,	28 },
//{ 159,	"Abjurer's",	AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	15,	20,	93,	IA_JEWEL,	IS_SUMMONING,	0,	0,	1,	22'500,	25'000,	8,	49,	55 },

//{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 1,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
//{ 160,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 3,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
//{ 161,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	 7,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
//{ 162,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	12,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 2'000,	 3'500,	4,	 6,	 8 },
//{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	25,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	 7'000,	 8'500,	6,	13,	17 },
//{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	47,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	14'000,	15'500,	7,	23,	28 },
//{ 163,	"Evoker's",		AE_SUMMON_DAMAGE_PERCENT,	0,	20,	25,	0,	0,	0,	0,	93,	IA_STAFF | IA_SHIELD | IA_SWORD_1H,	IS_SUMMONING,	0,	0,	1,	26'000,	27'500,	8,	51,	54 },

//{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 1,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
//{ 164,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	15,	19,	0,	0,	0,	0,	 3,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   150,	   190,	1,	 0,	 0 },
//{ 165,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	   600,	   750,	2,	 3,	 5 },
//{ 166,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	12,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 2'000,	 2'500,	4,	 6,	 8 },
//{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	25,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	 6'000,	 7'500,	6,	13,	17 },
//{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	47,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	11'000,	13'500,	7,	23,	28 },
//{ 167,	"Conjurer's",	AE_SUMMON_HP_PERCENT,	0,	20,	25,	0,	0,	0,	0,	93,	IA_ARMOR,	IS_SUMMONING,	0,	0,	1,	18'000,	19'500,	8,	51,	55 },

//could be used on uniques	{ 0,	"Summon AC",	AE_SUMMON_AC_PERCENT,	0,	5,	20,	0,	0,	0,	0,	25,	IA_ARMOR,	IS_ALL,	0,	0,	1,	100,	100,	5,	5+2 },


	/* ------------------------------------
			+ ARMOR VS SPECIES
	-------------------------------------*/

	//{ 168,	"Shimmering",	AE_SPICIES_AC,	MON_0_UNDEAD,	11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	//{ 169,	"Shining",		AE_SPICIES_AC,	MON_0_UNDEAD,	16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	//{ 170,	"Coruscating",	AE_SPICIES_AC,	MON_0_UNDEAD,	26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	//{ 171,	"Radiant",		AE_SPICIES_AC,	MON_0_UNDEAD,	36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	//{ 171,	"Luminous",		AE_SPICIES_AC,	MON_0_UNDEAD,	46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },

	//{ 172,	"Blessed",		AE_SPICIES_AC,	MON_1_DEMON,		11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	//{ 173,	"Saintly",		AE_SPICIES_AC,	MON_1_DEMON,		16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	//{ 174,	"Divine",		AE_SPICIES_AC,	MON_1_DEMON,		26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	//{ 175,	"Godly",		AE_SPICIES_AC,	MON_1_DEMON,		36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	//{ 175,	"Sanctified",	AE_SPICIES_AC,	MON_1_DEMON,		46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },

	//{ 176,	"Fine",			AE_SPICIES_AC,	MON_2_BEAST,		11,	15,	0,	0,	0,	0,	 7,	IA_ARMOR,	IS_ALL,	0,	0,	1,	   600,	   600,	 2,	 2,	 3 },
	//{ 177,	"Grand",		AE_SPICIES_AC,	MON_2_BEAST,		16,	25,	0,	0,	0,	0,	14,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 1'500,	 1'500,	 4,	 5,	 6 },
	//{ 178,	"Valiant",		AE_SPICIES_AC,	MON_2_BEAST,		26,	35,	0,	0,	0,	0,	28,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 3'000,	 3'000,	 6,	15,	17 },
	//{ 179,	"Glorious",		AE_SPICIES_AC,	MON_2_BEAST,		36,	45,	0,	0,	0,	0,	56,	IA_ARMOR,	IS_ALL,	0,	0,	1,	 6'000,	 6'000,	 8,	29,	33 },
	//{ 179,	"Awesome",		AE_SPICIES_AC,	MON_2_BEAST,		46,	60,	0,	0,	0,	0,	97,	IA_ARMOR,	IS_ALL,	0,	0,	1,	12'000,	12'000,	13,	46,	57 },


	/* ------------------------------------
				THORNS PERCENT
	-------------------------------------*/

	//{ 182,	"Thorned",	AE_THORNS_PERCENT,	0,	 31,	 40,	0,	0,	0,	0,	  2,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	//{ 183,	"Thorned",	AE_THORNS_PERCENT,	0,	 41,	 50,	0,	0,	0,	0,	  6,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   300,	   300,	 2,	  2,	  3 },
	//{ 184,	"Thorned",	AE_THORNS_PERCENT,	0,	 51,	 60,	0,	0,	0,	0,	 12,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   800,	   800,	 3,	  4,	  5 },
	//{ 185,	"Thorned",	AE_THORNS_PERCENT,	0,	 61,	 70,	0,	0,	0,	0,	 24,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'500,	 1'500,	 4,	 14,	 16 },
	//{ 186,	"Thorned",	AE_THORNS_PERCENT,	0,	 71,	 80,	0,	0,	0,	0,	 32,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'100,	 3'100,	 5,	 20,	 22 },
	//{ 187,	"Thorned",	AE_THORNS_PERCENT,	0,	 81,	 90,	0,	0,	0,	0,	 48,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 5'100,	 5'100,	 6,	 26,	 28 },
	//{ 188,	"Thorned",	AE_THORNS_PERCENT,	0,	 91,	100,	0,	0,	0,	0,	 56,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 7'100,	 7'100,	 7,	 32,	 34 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	101,	110,	0,	0,	0,	0,	 62,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	11'100,	11'100,	 8,	 38,	 40 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	111,	120,	0,	0,	0,	0,	 98,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	24'200,	28'000,	 9,	 42,	 46 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	121,	130,	0,	0,	0,	0,	112,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	29'500,	33'000,	10,	 48,	 53 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	131,	140,	0,	0,	0,	0,	130,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	34'500,	38'000,	11,	 55,	 61 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	141,	150,	0,	0,	0,	0,	150,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	39'500,	43'000,	12,	 63,	 70 },
	//{ 189,	"Thorned",	AE_THORNS_PERCENT,	0,	151,	160,	0,	0,	0,	0,	178,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	0,	56'500,	60'000,	16,	100,	111 },

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

				//{ 190,	"Hurting",		AE_DAMAGE,	0,	  1,	  1,	0,	0,	0,	0,	  1,	IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,					IS_BATTLE,	0,	0,	1,	   100,	   100,	1,	 0,	 0 },
				//{ 191,	"Wounding",		AE_DAMAGE,	0,	  2,	  3,	0,	0,	0,	0,	  2,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   200,	   300,	1,	 0,  0 },
				//{ 192,	"Mess",			AE_DAMAGE,	0,	  4,	  5,	0,	0,	0,	0,	  5,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   400,	   800,	2,	 2,	 3 },
				//{ 193,	"Maiming",		AE_DAMAGE,	0,	  6,	  8,	0,	0,	0,	0,	  8,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	   900,	 1'500,	3,	 4,	 6 },
				//{ 194,	"Mangling",		AE_DAMAGE,	0,	  9,	 11,	0,	0,	0,	0,	 11,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 1'600,	 2'200,	3,	 7,	 9 },
				//{ 195,	"Atrocity",		AE_DAMAGE,	0,	 12,	 15,	0,	0,	0,	0,	 14,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 2'300,	 3'000,	4,	11,	13 },
				//{ 196,	"Mayhem",		AE_DAMAGE,	0,	 16,	 20,	0,	0,	0,	0,	 20,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 3'100,	 4'000,	5,	15,	17 },
				//{ 197,	"Slaying",		AE_DAMAGE,	0,	 21,	 30,	0,	0,	0,	0,	 26,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 4'100,	 5'000,	6,	19,	21 },
				//{ 198,	"Gore",			AE_DAMAGE,	0,	 31,	 40,	0,	0,	0,	0,	 32,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 5'100,	 6'500,	6,	23,	26 },
				//{ 199,	"Carnage",		AE_DAMAGE,	0,	 41,	 50,	0,	0,	0,	0,	 37,	IA_RANGE | IA_STAFF | IA_MELEE | IA_CLAW | IA_KNIFE | IA_MALLET | IA_PISTOL,		IS_BATTLE,	0,	0,	1,	 6'600,	 8'000,	7,	26,	28 },


/* ------------------------------------
				STATS
-------------------------------------*/

//STR
{ 279,	"Staunchness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Stoutness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Stalwartness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 279,	"Ruggedness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 280,	"Heartiness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 280,	"Burliness",	AE_STAT,	ST_STRENGTH,	 1,	  1,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 281,	"Solidity",		AE_STAT,	ST_STRENGTH,	 1,	  2,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 282,	"Potency",		AE_STAT,	ST_STRENGTH,	 2,	  2,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 283,	"Tenacity",		AE_STAT,	ST_STRENGTH,	 2,	  3,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 284,	"Strength",		AE_STAT,	ST_STRENGTH,	 3,	  3,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

//DEX
{ 279,	"Aptitude",		AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Talent",		AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 280,	"Agility",		AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 281,	"Skill",		AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 282,	"Exactness",	AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 285,	"Deftness",		AE_STAT,	ST_DEXTERITY,	 1,	  1,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 285,	"Precision",	AE_STAT,	ST_DEXTERITY,	 1,	  2,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Mastery",		AE_STAT,	ST_DEXTERITY,	 2,	  2,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Guile",		AE_STAT,	ST_DEXTERITY,	 2,	  3,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 285,	"Grace",		AE_STAT,	ST_DEXTERITY,	 3,	  3,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

//MAG
{ 279,	"Sanity",		AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Clarity",		AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Insight",		AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 280,	"Reasoning",	AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 281,	"Intellect",	AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 284,	"Knowledge",	AE_STAT,	ST_MAGIC,	 1,	  1,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 285,	"Foresight",	AE_STAT,	ST_MAGIC,	 1,	  2,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Wisdom",		AE_STAT,	ST_MAGIC,	 2,	  2,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Sorcery",		AE_STAT,	ST_MAGIC,	 2,	  3,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 285,	"Wizardry",		AE_STAT,	ST_MAGIC,	 3,	  3,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

//VIT
{ 279,	"Health",		AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Sustenance",	AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 279,	"Hardiness",	AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 280,	"Zest",			AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 281,	"Zeal",			AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 284,	"Vim",			AE_STAT,	ST_VITALITY,	 1,	  1,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 285,	"Vigor",		AE_STAT,	ST_VITALITY,	 1,	  2,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Ardor",		AE_STAT,	ST_VITALITY,	 2,	  2,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 285,	"Fervor",		AE_STAT,	ST_VITALITY,	 2,	  3,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 285,	"Life",			AE_STAT,	ST_VITALITY,	 3,	  3,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

//ALL STATS
//{ 286,	"the Plains",		AE_ALL_STAT,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   150,	    150,	 2,	 0,	  1 },
//{ 286,	"the Hills",		AE_ALL_STAT,	0,	 2,	 2,	0,	0,	0,	0,	  6,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	   500,	    500,	 2,	 0,	  1 },
//{ 287,	"the Mountains",	AE_ALL_STAT,	0,	 3,	 3,	0,	0,	0,	0,	  9,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 1'000,	  1'000,	 3,	 1,	  2 },
//{ 288,	"the Rainbow",		AE_ALL_STAT,	0,	 4,	 4,	0,	0,	0,	0,	 13,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 2'000,	  2'000,	 3,	 7,	  9 },
//{ 289,	"the Lights",		AE_ALL_STAT,	0,	 5,	 6,	0,	0,	0,	0,	 23,	IA_JEWEL | IA_SHIELD | IA_ARMOR | IA_FLASK,	IS_ALL,	0,	0,	1,	 4'000,	  5'000,	 4,	10,	 12 },
//{ 290,	"the Clouds",		AE_ALL_STAT,	0,	 7,	 8,	0,	0,	0,	0,	 33,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	10'000,	 12'000,	 6,	19,	 21 },
//{ 291,	"the Sky",			AE_ALL_STAT,	0,	 9,	10,	0,	0,	0,	0,	 43,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	14'000,	 16'000,	 8,	25,	 28 },
//{ 292,	"the Heavens",		AE_ALL_STAT,	0,	11,	12,	0,	0,	0,	0,	 53,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	27'000,	 30'000,	 9,	32,	 35 },
//{ 293,	"the Moon",			AE_ALL_STAT,	0,	13,	14,	0,	0,	0,	0,	 63,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	35'000,	 45'000,	10,	40,	 44 },
//{ 497,	"the Sun",			AE_ALL_STAT,	0,	15,	16,	0,	0,	0,	0,	 83,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	45'000,	 55'000,	11,	50,	 55 },
//{ 498,	"the Stars",		AE_ALL_STAT,	0,	17,	18,	0,	0,	0,	0,	103,	IA_JEWEL | IA_SHIELD | IA_ARMOR,			IS_ALL,	0,	0,	1,	55'000,	 65'000,	12,	60,	 66 },
//{ 499,	"the Zodiac",		AE_ALL_STAT,	0,	19,	20,	0,	0,	0,	0,	123,	IA_JEWEL | IA_ARMOR,						IS_ALL,	0,	0,	1,	65'000,	 75'000,	13,	70,	 77 },
//{ 500,	"Constellations",	AE_ALL_STAT,	0,	21,	22,	0,	0,	0,	0,	143,	IA_JEWEL,								IS_ALL,	0,	0,	1,	75'000,	 85'000,	14,	80,	 88 },
//{ 501,	"the Galaxies",		AE_ALL_STAT,	0,	23,	25,	0,	0,	0,	0,	163,	IA_JEWEL,								IS_ALL,	0,	0,	1,	85'000,	 95'000,	15,	90,	 99 },
//{ 502,	"the Universe",		AE_ALL_STAT,	0,	26,	30,	0,	0,	0,	0,	178,	IA_JEWEL,								IS_ALL,	0,	0,	0,	96'000,	100'000,	17,	99,	110 },


/* ------------------------------------
				SLVL
-------------------------------------*/

//{ 294,	"the Focus",	AE_SPELLS_LEVEL,	0,	1,	1,	0,	0,	0,	0,	  8,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 2'000,	 2'000,	2,	 0,	  0 },
//{ 295,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0,	 26,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	 6'000,	 6'000,	4,	12,	 18 },
//{ 296,	"Spellcraft",	AE_SPELLS_LEVEL,	0,	3,	3,	0,	0,	0,	0,	 44,	IA_JEWEL | IA_STAFF | IA_SHIELD | IA_ARMOR,	IS_MAGIC,	0,	0,	1,	12'000,	12'000,	8,	19,	 28 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	4,	4,	0,	0,	0,	0,	 64,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	25'000,	25'000,	12,	29,	 34 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	5,	5,	0,	0,	0,	0,	105,	IA_JEWEL | IA_STAFF | IA_SHIELD,			IS_MAGIC,	0,	0,	1,	35'000,	35'000,	13,	37,	 50 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	6,	6,	0,	0,	0,	0,	147,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	45'000,	45'000,	14,	63,	 80 },
//{ 297,	"Diabolism",	AE_SPELLS_LEVEL,	0,	7,	7,	0,	0,	0,	0,	178,	IA_JEWEL | IA_STAFF,						IS_MAGIC,	0,	0,	0,	55'000,	55'000,	15,	99,	119 },


/* ------------------------------------
				MANA
-------------------------------------*/
{ 306,	"the Moth",		AE_MANA,	0,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 307,	"the Spider",	AE_MANA,	0,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 310,	"the Frog",		AE_MANA,	0,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 314,	"the Cat",		AE_MANA,	0,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 313,	"the Vulture",	AE_MANA,	0,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 314,	"the Serpent",	AE_MANA,	0,	 1,	  2,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 314,	"the Viper",	AE_MANA,	0,	 2,	  3,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 314,	"the Wyrm",		AE_MANA,	0,	 3,	  4,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 314,	"the Hydra",	AE_MANA,	0,	 4,	  5,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 314,	"the Drake",	AE_MANA,	0,	 5,	  5,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

/* ------------------------------------
				HP
-------------------------------------*/
{ 307,	"the Squirrel",	AE_HIT_POINTS,	0,	 1,	  1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 306,	"the Rabbit",	AE_HIT_POINTS,	0,	 1,	  1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 307,	"the Jackal",	AE_HIT_POINTS,	0,	 1,	  1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 308,	"the Fox",		AE_HIT_POINTS,	0,	 1,	  1,	0,	0,	0,	0,	 13,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 310,	"the Lynx",		AE_HIT_POINTS,	0,	 1,	  1,	0,	0,	0,	0,	 25,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 311,	"the Elk",		AE_HIT_POINTS,	0,	 1,	  2,	0,	0,	0,	0,	 48,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 313,	"the Wolf",		AE_HIT_POINTS,	0,	 2,	  3,	0,	0,	0,	0,	 74,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 314,	"the Ox",		AE_HIT_POINTS,	0,	 3,	  4,	0,	0,	0,	0,	 94,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 314,	"the Bear",		AE_HIT_POINTS,	0,	 4,	  5,	0,	0,	0,	0,	114,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 314,	"the Whale",	AE_HIT_POINTS,	0,	 5,	  5,	0,	0,	0,	0,	134,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


/* ------------------------------------
	RANGED WEAPONS' ELEMENTAL DAMAGE
-------------------------------------*/
//removed........




/* ------------------------------------
			+ GOLD FLAT
-------------------------------------*/

{ 383,	"Avarice",		AE_GOLD_ABS_FIND,	0,	1,	1,	1,	1,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 384,	"Greed",		AE_GOLD_ABS_FIND,	0,	1,	1,	1,	1,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 3,	 0,	 0 },
{ 385,	"Wealth",		AE_GOLD_ABS_FIND,	0,	1,	1,	1,	1,	0,	0,	 12,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 4,	 0,	 0 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	1,	1,	1,	1,	0,	0,	 22,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	1,	1,	1,	1,	0,	0,	 40,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	1,	1,	2,	2,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	1,	2,	2,	2,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 386,	"Abundance",	AE_GOLD_ABS_FIND,	0,	2,	2,	2,	2,	0,	0,	130,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


/* ------------------------------------
				 STUN
-------------------------------------*/

{ 387,	"Firmness",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	  2,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 388,	"Firmness",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	  4,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 389,	"Stiffness",		AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	  7,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 390,	"Stiffness",		AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	 12,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 391,	"Rigidity",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	 23,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	 45,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 392,	"Rigidity",			AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	120,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 392,	"Inflexibility",	AE_STUN_RESIST,	0,	 1,	 1,	0,	0,	0,	0,	140,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


/* ------------------------------------
				  MF
-------------------------------------*/
{ 393,	"Chance",	AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 394,	"Bounty",	AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	  5,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 395,	"Fortune",	AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 396,	"Luck",		AE_MAGIC_FIND,	0,	 1,	 1,	0,	0,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


//works but unused now	{ 0,	"Spell Level",	AE_SPELL_LEVEL,	PS_1_FIREBOLT,	1,	2,	0,	0,	0,	0,	11,	IA_STAFF,	IS_ALL,	0,	0,	1,	100,	100,	1,	1+2 },
//	{ 0,	"To Spell",	AE_SPELL_PERCENT,	0,	500,	500,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"Add Spell Damage",	AE_SPELL_DAMAGE,	500,	500,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	10,	10,	1,	1+2 },
//	{ 0,	"To Element",	AE_ELEMENT_PERCENT,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
//	{ 0,	"Add Element Damage",	AE_ELEMENT_DAMAGE,	0,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


	/* ------------------------------------
				  FLAT THORNS
	-------------------------------------*/

	//{ 397,	"Burs",		AE_ACTION,	EA_THORNS,	  1,	  2,	  3,	  4,	51,	 60,	  1,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   100,	   100,	 1,	  0,	  0 },
	//{ 398,	"Prickles",	AE_ACTION,	EA_THORNS,	  5,	  6,	  7,	  8,	36,	 45,	  3,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   200,	   200,	 2,	  0,	  0 },
	//{ 399,	"Bramble",	AE_ACTION,	EA_THORNS,	  4,	  5,	  7,	 10,	76,	 90,	  8,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	   500,	   500,	 3,	  2,	  3 },
	//{ 400,	"Teeth",	AE_ACTION,	EA_THORNS,	 27,	 38,	 48,	 56,	21,	 30,	 15,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 1'000,	 1'100,	 4,	  4,	  6 },
	//{ 401,	"Barbs",	AE_ACTION,	EA_THORNS,	 17,	 18,	 19,	 20,	91,	100,	 26,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 2'300,	 2'500,	 5,	  9,	 11 },
	//{ 402,	"Thorns",	AE_ACTION,	EA_THORNS,	 33,	 36,	 40,	 45,	61,	 70,	 34,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 3'300,	 3'500,	 6,	 13,	 15 },
	//{ 403,	"Spikes",	AE_ACTION,	EA_THORNS,	 60,	 70,	 85,	105,	41,	 55,	 42,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	 6'300,	 6'500,	 7,	 17,	 19 },
	//{ 404,	"Razors",	AE_ACTION,	EA_THORNS,	 62,	 72,	 80,	 85,	71,	 80,	 51,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	10'500,	11'500,	 8,	 21,	 23 },
	//{ 405,	"Lances",	AE_ACTION,	EA_THORNS,	515,	550,	650,	750,	11,	 15,	 59,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	21'500,	22'500,	 9,	 25,	 28 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	100,	110,	115,	145,	81,	 90,	 75,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	31'500,	32'500,	10,	 41,	 44 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	250,	270,	280,	340,	41,	 45,	 95,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	42'500,	44'500,	11,	 55,	 60 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	150,	165,	175,	200,	81,	 90,	115,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	53'500,	55'500,	12,	 66,	 69 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	675,	750,	825,	900,	27,	 30,	135,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	58'500,	58'500,	13,	 74,	 81 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	650,	700,	750,	800,	41,	 45,	155,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	63'500,	64'500,	14,	 86,	 93 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	425,	450,	475,	500,	81,	 90,	175,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	1,	68'500,	70'500,	15,	 97,	105 },
	//{ 406,	"Swords",	AE_ACTION,	EA_THORNS,	525,	550,	575,	600,	81,	 90,	178,	IA_BODY_ARMOR | IA_HELM | IA_SHIELD,	IS_THORNS,	0,	0,	0,	75'500,	80'500,	16,	106,	110 },


	//	{ 0,	"To Thorns",	AE_ACTION_PERCENT,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
	//	{ 0,	"Add Thorns",	AE_ACTION_VALUE,	EA_THORNS,	0,	0,	0,	0,	0,	0,	11,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },


		

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
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	  3,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	  5,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 428,	"Strengthening",	AE_SUMMON_DAMAGE,	0,	 1,	 1,	1,	1,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 1,	 1,	0,	0,	0,	0,	  5,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 1,	 1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 1,	 1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 1,	 2,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 2,	 2,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 2,	 3,	0,	0,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 3,	 4,	0,	0,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 5,	 6,	0,	0,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 434,	"Thickening",	AE_SUMMON_HP,	0,	 6,	 7,	0,	0,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	  3,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	  5,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 440,	"Guarding",	AE_SUMMON_AC,	0,	 1,	 1,	0,	0,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


/* ------------------------------------
				CRITS
-------------------------------------*/

// + crit dmg percent
{ 450,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	 1,	 1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 450,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	 1,	 2,	0,	0,	0,	0,	 30,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 450,	"Efficacy",		AE_CRIT_DAMAGE_PERCENT,	0,	 2,	 2,	0,	0,	0,	0,	 60,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	 2,	 3,	0,	0,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	 3,	 3,	0,	0,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	 4,	 4,	0,	0,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 452,	"Efficiency",	AE_CRIT_DAMAGE_PERCENT,	0,	 4,	 5,	0,	0,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

// + crit dmg
{ 456,	"Smite",		AE_CRIT_DAMAGE,	0,	  1,	  1,	 2,	 2,	0,	0,	  3,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 456,	"Smite",		AE_CRIT_DAMAGE,	0,	  1,	  2,	 2,	 3,	0,	0,	  5,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 2,	 0,	 0 },
{ 457,	"Justice",		AE_CRIT_DAMAGE,	0,	  2,	  2,	 3,	 4,	0,	0,	  8,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    250,	    250,	 3,	 0,	 0 },
{ 458,	"Punishment",	AE_CRIT_DAMAGE,	0,	  2,	  3,	 4,	 5,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 3,	 0,	 0 },
{ 459,	"Retaliation",	AE_CRIT_DAMAGE,	0,	  3,	  4,	 5,	 6,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 460,	"Oblation",		AE_CRIT_DAMAGE,	0,	  5,	  6,	 7,	 8,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 461,	"Retribution",	AE_CRIT_DAMAGE,	0,	  9,	 10,	11,	12,	0,	0,	 76,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 461,	"Retribution",	AE_CRIT_DAMAGE,	0,	 13,	 14,	15,	16,	0,	0,	106,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 17,	 18,	19,	20,	0,	0,	121,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 462,	"Obliteration",	AE_CRIT_DAMAGE,	0,	 21,	 22,	23,	24,	0,	0,	142,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


/* ---------------------------------------
				ELEM BOOST
----------------------------------------*/

{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 456,	"Burning",		AE_ELEMENT_PERCENT,	ET_1_FIRE,	  1,	  1,	0,	0,	0,	0,	145,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 456,	"Shocking",		AE_ELEMENT_PERCENT,	ET_2_LIGHTNING,	  1,	  1,	0,	0,	0,	0,	145,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 456,	"Devastation",	AE_ELEMENT_PERCENT,	ET_3_ARCAN,	  1,	  1,	0,	0,	0,	0,	145,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },

{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	 15,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    150,	    150,	 2,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	 28,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    350,	    350,	 4,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	 49,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 4,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	 75,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    450,	    450,	 5,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	105,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 5,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	115,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    550,	    550,	 6,	 0,	 0 },
{ 456,	"Purification",	AE_ELEMENT_PERCENT,	ET_5_HOLY,	  1,	  1,	0,	0,	0,	0,	145,	IA_RING,	IS_ALL | IS_SUMMONING,	0,	0,	1,	    650,	    650,	 6,	 0,	 0 },


		/* ------------------------------------
			   NONSENSE !!! << REWORK!
		-------------------------------------*/

		//{ 469,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
		//{ 470,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
		//{ 471,	"Fire Bolts",		AE_SPELL_DAMAGE,	PS_1_FIREBOLT,		4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

		//{ 472,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
		//{ 473,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
		//{ 474,	"Charged Bolts",	AE_SPELL_DAMAGE,	PS_30_CHARGED_BOLT,	4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

		//{ 475,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		1,	2,	3,	 4,	0,	0,	 1,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	100,	100,	1, 0, 0 },
		//{ 476,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		2,	3,	4,	 5,	0,	0,	 4,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	300,	300,	2, 0, 0 },
		//{ 477,	"Holy Bolts",		AE_SPELL_DAMAGE,	PS_31_HOLY_BOLT,		4,	6,	7,	11,	0,	0,	10,	IA_JEWEL | IA_STAFF | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	900,	900,	3, 3, 4 },

		//{ 478,	"Arcane Stars",		AE_SPELL_DAMAGE,	PS_35_ARCANE_STAR,	4,	8,	12,	16,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
		//{ 479,	"Arcane Stars",		AE_SPELL_DAMAGE,	PS_35_ARCANE_STAR,	8,	16,	24,	32,	0,	0,	18,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 12 },

		//{ 480,	"Lightning",		AE_SPELL_DAMAGE,	PS_3_LIGHTNING,		2,	3,	4,	5,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
		//{ 481,	"Lightning",		AE_SPELL_DAMAGE,	PS_3_LIGHTNING,		4,	8,	10,	12,	0,	0,	18,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 13 },

		//{ 482,	"Incineration",		AE_SPELL_DAMAGE,	PS_20_INCINERATE,	2,	3,	4,	5,	0,	0,	13,	IA_RING | IA_STAFF | IA_SWORD_1H | IA_SHIELD,	IS_MAGIC,	0,	0,	1,	1'400,	1'400,	3, 5,  7 },
		//{ 483,	"Incineration",		AE_SPELL_DAMAGE,	PS_20_INCINERATE,	4,	8,	10,	12,	0,	0,	19,	IA_STAFF | IA_SWORD_1H | IA_SHIELD,				IS_MAGIC,	0,	0,	1,	1'900,	1'900,	4, 8, 13 },

		//	{ 0,	"To Weapon",	AE_WEAPON_PERCENT,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
		//	{ 0,	"To Attack Speed",	AE_ATTACK_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },
		//	{ 0,	"To Cast Speed",	AE_CAST_SPEED,	0,	0,	0,	0,	0,	0,	0,	1,	IA_ALL,	IS_ALL,	0,	0,	1,	1000,	1000,	5,	5+2 },

			{ 0,	"",	AE_NO },
};
// next new affix id = ?
