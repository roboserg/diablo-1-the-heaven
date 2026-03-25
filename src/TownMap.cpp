#include "stdafx.h"

int CeliaUniqsTable[6][9] = {
	{ 570, 571, 572, 2407, 2408, 2409, 2410, 2411, 2412 }, // PC_0_WARRIOR // exp
	{ 573, 574, 575, 2413, 2414, 2415, 2416, 2417, 2418 }, // PC_1_ARCHER // exp
	{ 175, 176, 177, 2437, 2438, 2439, 2440, 2441, 2442 }, // PC_2_MAGE // exp
	{ 576, 577, 578, 2419, 2420, 2421, 2422, 2423, 2424 }, // PC_3_MONK // exp
	{ 579, 580, 581, 2425, 2426, 2427, 2428, 2429, 2430 }, // PC_4_ROGUE // exp
	{ 582, 583, 584, 2431, 2432, 2433, 2434, 2435, 2436 }, // PC_5_SAVAGE // EXP
};
int AnvilOfFuryUniqsTable[PLAYER_FULL_CLASS::PFC_COUNT][9] = {
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_WARRIOR
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_INQUISITOR
	{	20,		21,		22,		2455,	2456,	2457,	2458,	2459,	2460	}, // PFC_GUARDIAN
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_TEMPLAR

	{	315,	316,	317,	2449,	2450,	2451,	2452,	2453,	2454	}, // PFC_ARCHER
	{	315,	316,	317,	2449,	2450,	2451,	2452,	2453,	2454	}, // PFC_SCOUT
	{	315,	316,	317,	2449,	2450,	2451,	2452,	2453,	2454	}, // PFC_SHARPSHOOTER
	{	17,		18,		19,		2473,	2474,	2475,	2476,	2477,	2478	}, // PFC_TRAPPER

	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_MAGE
	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_ELEMENTALIST
	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_DEMONOLOGIST
	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_NECROMANCER
	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_BEASTMASTER
	{	327,	328,	329,	2461,	2462,	2463,	2464,	2465,	2466	}, // PFC_WARLOCK

	{	318,	319,	320,	2491,	2492,	2493,	2494,	2495,	2496	}, // PFC_MONK
	{	69,		70,		71,		2485,	2486,	2487,	2488,	2489,	2490	}, // PFC_KENSEI
	{	324,	325,	326,	2467,	2468,	2469,	2470,	2471,	2472	}, // PFC_SHUGOKI
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_SHINOBI

	{	321,	322,	323,	2479,	2480,	2481,	2482,	2483,	2484	}, // PFC_ROGUE
	{	17,		18,		19,		2473,	2474,	2475,	2476,	2477,	2478	}, // PFC_ASSASSIN
	{	4665,	4666,	4667,	4668,	4669,	4670,	4671,	4672,	4673	}, // PFC_IRON_MAIDEN
	{	17,		18,		19,		2473,	2474,	2475,	2476,	2477,	2478	}, // PFC_BOMBARDIER

	{	20,		21,		22,		2455,	2456,	2457,	2458,	2459,	2460	}, // PFC_SAVAGE
	{	324,	325,	326,	2467,	2468,	2469,	2470,	2471,	2472	}, // PFC_BERSERKER
	{	324,	325,	326,	2467,	2468,	2469,	2470,	2471,	2472	}, // PFC_EXECUTIONER
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_THRAEX
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_MURMILLO
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_DIMACHAERUS
	{	181,	182,	183,	2443,	2444,	2445,	2446,	2447,	2448	}, // PFC_SECUTOR
	{	324,	325,	326,	2467,	2468,	2469,	2470,	2471,	2472	}, // PFC_DRUID
};


int CowSounds[ 3 ][ 6 ] = {// 00491938
	S_781_WAR_52, S_672_ROG_52, S_569_SOR_52, S_889_MONK_52, S_672_ROG_52, 0, 
	S_778_WAR_49, S_669_ROG_49, S_566_SOR_49, S_886_MONK_49, S_669_ROG_49, 0, 
	S_779_WAR_50, S_670_ROG_50, S_567_SOR_50, S_887_MONK_50, S_670_ROG_50, 0 
};
// специальный порядок анимации для горожан. Чтобы создать иллюзию длинной неповторящейся анимации
char TownerAnimationFramesOrder[ 6 ][ 148 ] = {	// 004CAE70
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4,		-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 3, 2, 1, 20, 19, 19, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, -1,		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 25, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 25, 25, 1, 1, 1, 25, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, -1,		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 2, 1, 16, 15, 14, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, -1,			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 11, 11, 12, 13, 14, 15, 16, 17, 18, 18, 1, 1, 1, 18, 17, 16, 15, 14, 13, 12, 11, 10, 11, 12, 13, 14, 15, 16, 17, 18, 1, 2, 3, 4, 5, 5, 5, 4, 3, 2, -1,			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 4, 4, 5, 6, 6, 6, 5, 4, 15, 14, 13, 13, 13, 14, 15, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 19, 18, 19, 1, 2, 1, 19, 18, 19, 1, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 15, 14, 13, 13, 13, 13, 14, 15, 15, 15, 14, 13, 12, 12, 12, 11, 10, 10, 10, 9, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1, 2, 1, 19, 18, 19, 1, 2, 1, 2, 3, -1,		0, 0, 0, 0, 0, 0
};
int CowsRows[3]	= { 83, 77, 81 };	// 004CB1E8
int CowsCols[3]	= { 46, 50, 53 };	// 004CB1F8
int CowsOrients[3]	= { 1, 3, 4 };	// 004CB208
int CowsDeltaRow[8]	= { -1, 0, -1,-1, -1, 0, -1, -1 };	// 004CB218
int CowsDeltaCol[8]	= { -1,-1, -1, 0, -1, -1, -1, 0 };	// 004CB238

// it is 14 people and 24 quests. index in cross of people and qwest mean index of speech which this man tall in "talk to" dialog 
// 004CB258	hellfire quests
int TownersSpeechAboutQuestArray[TI_15_COUNT][Quest_Count] = {
	{ SP_116_GRISWOLD_ABOUT_MAGIC_ROCK,	SP_126_GRISWOLD_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_77_GRISWOLD_ABOUT_LOGLAN,	SP_M1,						SP_59,	SP_16,	SP_69,	SP_98,	SP_89,	SP_106,	SP_6,	SP_43,	SP_51,	SP_31,	SP_268,		SP_369,	SP_M1,	SP_378, SP_388,	SP_395,	SP_298,	SP_303,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_421,	SP_M1,	SP_427,	SP_M1,	SP_M1,	SP_M1, SP_438,	SP_M1,	SP_M1,	SP_16 },//	Griswold
	{ SP_113_PEPIN_ABOUT_MAGIC_ROCK,	SP_M1,							SP_M1,	SP_M1,	SP_75_PEPIN_ABOUT_LOGLAN,		SP_M1, 						SP_57,	SP_14,	SP_67,	SP_96,	SP_86,	SP_104,	SP_4,	SP_40,	SP_49,	SP_29, 	SP_269,		SP_370,	SP_M1,	SP_282,	SP_386,	SP_397,	SP_299,	SP_302,	SP_M1,											SP_M1,	SP_M1,	SP_416,	SP_M1,	SP_M1,	SP_424,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_441,	SP_M1,	SP_14 },//	Pepin
	{ SP_M1,							SP_M1,							SP_M1,	SP_M1,	SP_M1,							SP_M1,						SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,		SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 },//	Cow
	{ SP_112_OGDEN_ABOUT_MAGIC_ROCK,	SP_122_OGDEN_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_74_OGDEN_ABOUT_LOGLAN,		SP_M1,						SP_56,	SP_M1, 	SP_66,	SP_95,	SP_85,	SP_103,	SP_2,	SP_38,	SP_48,	SP_26,	SP_271,		SP_372, SP_M1,	SP_379,	SP_382,	SP_400,	SP_311,	SP_308,	SP_403_OGDEN_THE_RAMPAGING_DEMON_NOT_FINISHED,	SP_409,	SP_412,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_444,	SP_M1 },//	Ogden
	{ SP_111_CAIN_ABOUT_MAGIC_ROCK,		SP_121_CAIN_ABOUT_HUGE_AXE,		SP_M1,	SP_M1, 	SP_73_CAIN_ABOUT_LOGLAN,		SP_25_KAIN_ABOUT_DIABLO,	SP_55,	SP_11,	SP_65,	SP_94,	SP_84,	SP_102,	SP_0,	SP_37, 	SP_47,	SP_24,	SP_272,		SP_368,	SP_M1,	SP_375,	SP_387,	SP_399,	SP_312,	SP_304,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_11 },//	Cain
	{ SP_118_FARNHAM_ABOUT_MAGIC_ROCK,	SP_127_FARNHAM_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_78_FARNHAM_ABOUT_LOGLAN,		SP_M1,						SP_60,	SP_17,	SP_70,	SP_99, 	SP_91,	SP_107,	SP_7,	SP_44,	SP_52,	SP_32,	SP_362,		SP_373,	SP_M1,	SP_380, SP_M1,	SP_396,	SP_313,	SP_305,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_17 },//	Farnham
	{ SP_119_ADRIA_ABOUT_MAGIC_ROCK,	SP_129_ADRIA_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_79_ADRIA_ABOUT_LOGLAN,		SP_M1,						SP_61,	SP_18,	SP_71,	SP_100,	SP_92,	SP_108,	SP_8,	SP_45,	SP_53,	SP_33,	SP_267,		SP_367,	SP_M1,	SP_377,	SP_385,	SP_401,	SP_315,	SP_306,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_18 },//	Adria
	{ SP_114_GILIAN_ABOUT_MAGIC_ROCK,	SP_125_GILIAN_ABOUT_HUGE_AXE,	SP_M1,	SP_M1,	SP_76_GILIAN_ABOUT_LOGLAN,		SP_M1,						SP_58, 	SP_15,	SP_68,	SP_97,	SP_87,	SP_105,	SP_5,	SP_42,	SP_50,	SP_30,	SP_363,		SP_365,	SP_M1,	SP_274,	SP_383,	SP_394,	SP_297,	SP_309,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_15 },//	Gillian
	{ SP_120_WIRT_ABOUT_MAGIC_ROCK,		SP_133_WIRT_ABOUT_HUGE_AXE,		SP_M1,	SP_M1,	SP_80_WIRT_ABOUT_LOGLAN,		SP_M1,						SP_62,	SP_19,	SP_72,	SP_101,	SP_93,	SP_109,	SP_9,	SP_46,	SP_54,	SP_34,	SP_275,		SP_371,	SP_M1,	SP_376,	SP_384,	SP_398,	SP_314,	SP_307,	SP_M1,											SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_430,	SP_433,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_19 },//	Wirt
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 },//	hellfire citizens
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 },//
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 },//
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 },//
	{ SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1,	SP_M1 }, //
};	
int CowClickPlayerSound = -1;	// 004CB798

char SomeBoolFlag_18;	// 005753D4
char CowQuestOn;	// 005753D8

int NPCMenusEnabled;	// 006FF1E0
int CowClickPlayerSoundIndex;	// 006FF1E4
uchar TownersCount;	// 006FF1E8
int CowClickCount;	// 006FF1EC
int SomeBoolFlag_41;	// 006FF1F0
int WirtIsPlacedOnMap;	// 006FF1F4
char* CowSprite;	// 006FF1F8
Towner Towners[16];	// 006FF200
Sign Signes[] = { // 00B01110 signes patch (1) did
	{ 1, 1, 0, 0, 32, 12, 0, 0, 1, 0, "here lies the programmer\nburied under the code\nfragments of this game\nrest in peace, desert dragon", 0, 0 },
	{ 1, 1, 0, 0, 36, 8, 0, 0, 1, 0, "here lies mordor\nan outsider in life\n...and in afterlife", 0, 0 },
	{ 1, 1, 0, 0, 34, 24, 0, 0, 1, 0, "Here lies Rageweaver\nGod Of The Arena", 0, 0 },
	{ 1, 1, 0, 0, 28, 12, 0, 0, 1, 0, "Here lies a thing\na quiet madness made", 0, 0 },
	{ 1, 1, 0, 0, 34, 30, 0, 0, 1, 0, "Here lies Oracle Raven\nwish", 0, 0 },
	{ -1 },
};

int CurSign = -1; // signes patch (2) did

bool IsShowTileNum;

//----- (00473433) --------------------------------------------------------
void __fastcall DrawTileGrid(uchar* dst, uchar color /*= 194*/)
{
	dst += 2 * (16 - 1);
	for( int i = 1; i <= 16; ++i, dst -= WorkingWidth + 2 ){
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ break; }
		dst[0] = dst[2 * 2 * i - 1] = 194;
	}
}

//----- (00473433) --------------------------------------------------------
void __fastcall DrawTileFill( uchar* dst, uchar color /*= 0*/ )
{
	dst += 2 * (16 - 1);
	for( int i = 1; i <= 16; ++i, dst -= WorkingWidth + 2 ){ // lower triangle
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ break; }
		memset(dst, color, 2 * 2 * i);
	}
	dst += 2 * 2;
	for( int i = 1; i <= 15; ++i, dst -= WorkingWidth - 2 ){ // upper triangle
		if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ break; }
		memset(dst, color, 64 - 2 * 2 * i);
	}
}

//----- (00474322) --------------------------------------------------------
void __fastcall DrawMostTopTile(uchar* dst, int row, int col, int screenCellRow, int tileInCellIndex, int ofsX, int ofsY) // T_DrawEFlag3
{
	TileColumn* tile = &TileMap[IsometricMapOffset(row, col)];
	for( int i = 0; i < 7; ++i ){ // TODO: find out why 7, not 8 (it is original)
		if( screenCellRow >= i ){
			if( CurTileID = tile->Layer[i].Left ; CurTileID ) DrawDunCelTrans(dst);
			if( CurTileID = tile->Layer[i].Right; CurTileID ) DrawDunCelTrans(dst + 32);
		}
		dst -= WorkingWidth * 32;
	}
	DrawDynamicObjectsTown((uchar*)dst, row, col, screenCellRow, tileInCellIndex, ofsX, ofsY, 0);
}

//----- (004743AB) --------------------------------------------------------
void __fastcall DrawDynamicObjectsTown(uchar* surfaceDest, int row, int col, int screenCellRow, int tileInCellIndex, int playerOffsetX, int playerOffsetY, int firstDraw) // T_DrawHTLXsub
{
	if( int itemIndex = ItemMap[row][col] - 1; itemIndex >= 0 ){
		Item& item = Items[itemIndex];
		int outlineColor = IsAltPressed || AltHighlight || itemIndex == Cur.ItemID ? 181 : -1;
		CEL_Draw(playerOffsetX - item.subOffsetX, playerOffsetY, item.ItemCELFilePtr, item.FrameIndex, item.AnimWidth, 0, tileInCellIndex, R_DIRECT, outlineColor);
	}
	if( FlagMap[row][col] & CF_16_MONSTER_HORIZONTAL ){
		if( int townerIndex = -1 - MonsterMap[row][col]; townerIndex >= 0 ){
			Towner& towner = Towners[townerIndex];
			CEL_Draw(playerOffsetX - Towners[townerIndex].offsetX, playerOffsetY, towner.stayAnim, towner.CurFrame, towner.animWidth, 0, tileInCellIndex, R_DIRECT, townerIndex == CurMon ? 166 : -1);
		}
	}
	if( int townerIndex = MonsterMap[row][col] - 1; townerIndex >= 0 ){
		Towner& towner = Towners[townerIndex];
		CEL_Draw(playerOffsetX - towner.offsetX, playerOffsetY, towner.stayAnim, towner.CurFrame, towner.animWidth, 0, tileInCellIndex, R_DIRECT, townerIndex == CurMon ? 166 : -1);
		if( NpcSign && IsTownerHasActiveSpeech( CurrentPlayerIndex, townerIndex ) ){
            Surface_DrawCEL(playerOffsetX + 32/2, playerOffsetY - 88, Data_ExclamCEL, 1, 32);
        }
	}
	if( FlagMap[row][col] & CF_32_PLAYER_HORIZONTAL ){
		if( int playerIndex = -1 - PlayerMap[row][col - 1]; playerIndex >= 0 ){
			Player& player = Players[playerIndex];
			int x = playerOffsetX + player.xOfs - player.animWidth2;
			int y = playerOffsetY + player.yOfs;
			if( playerIndex == Cur.playerIndex ){
				CL2_DrawOutline(165, x, y, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
			}
			CL2_Draw(x, y, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
			if( firstDraw && player.UnderArcType ){
				DrawMostTopTile(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
			}
		}
	}
	if( FlagMap[row][col] & CF_4_DEAD_PLAYER ){
		DrawDiedPlayer(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( int playerIndex = PlayerMap[row][col] - 1; playerIndex >= 0 ){
		Player& player = Players[playerIndex];
		int x = playerOffsetX + player.xOfs - player.animWidth2;
		int y = playerOffsetY + player.yOfs;
		if( playerIndex == Cur.playerIndex ){
			CL2_DrawOutline(165, x, y, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
		}
		CL2_Draw(x, y, player.currentAnimationPointer, player.currentAnimationFrame, player.animWidth, 0, tileInCellIndex);
		if( firstDraw && player.UnderArcType ){
			DrawMostTopTile(surfaceDest - 64, row - 1, col + 1, screenCellRow, tileInCellIndex, playerOffsetX - 64, playerOffsetY);
		}
	}
	if( FlagMap[row][col] & CF_1_SPELL_CAST ){
		DrawMissileInTile(row, col, playerOffsetX, playerOffsetY, 0, tileInCellIndex, 0);
	}
	if( ArchGraphicsMap[row][col] ){
		ActNullSpell((int)surfaceDest);
	}
}

//----- (004746E3) --------------------------------------------------------
void __fastcall DrawSceneTownTop(int row, int col, int offsetX, int offsetY, int horCellDrawCount, int screenCellRow, int halfFlag) // T_DrawHTLX
{
	if( horCellDrawCount <= 0 ) return;
	// row, col - левая клетка рисуемоего по диагонали ряда (враво-вверх по массиву карт)
	TileColumn* tile = &TileMap[IsometricMapOffset(row, col)];
	int tileInCellIndex = std::min(2 * screenCellRow + 2, 8); // 8 == 7 + 1, максимальная высота объекта над тайлом заданная в тайлах ?
	// рисуем средние полные целлы:
	do{
		if( In112(col, row) && (CurTileID = FineMap[row][col], CurTileID) ){
			int yOfs = YOffsetHashTable[offsetY];
			uchar* leftTileDst = &WorkingSurface[offsetX + yOfs];
			if( !IsShowTileNum || CurTileID != CurFineMap ){
				for( int i = 0; i < 8; ++i ){
					if( screenCellRow >= i ){
						// рисуем левые треугольники и стены
						CurTileID = tile->Layer[i].Left;
						if( CurTileID ) DrawDunCelTrans(leftTileDst);
						// рисуем правые треугольники и стены
						CurTileID = tile->Layer[i].Right;
						if( CurTileID ) DrawDunCelTrans(leftTileDst + 32);
					}
					leftTileDst -= 32 * WorkingWidth;
				}
			}else{
				DrawTileFill(&WorkingSurface[ offsetX ] + YOffsetHashTable[ offsetY ]);
			}
			// рисуем остальные объекты над тайлом
			// в т.ч. рисуется объект над ячейкой слева от героя которые ограничивают его движение
			DrawDynamicObjectsTown(&WorkingSurface[offsetX + yOfs], row, col, screenCellRow, tileInCellIndex, offsetX, offsetY, 1);
		}else{
			// эта функция очевидно зарисовывает места где отсутствует карта
			DrawTileFill(&WorkingSurface[offsetX] + YOffsetHashTable[offsetY]);
		}
		++tile;
		++row;
		--col;
		offsetX += 64;
	}while( horCellDrawCount-- != 1 );
}

//----- (00474FB6) --------------------------------------------------------
void InitIsometricTileMap()
{
	for( int col = 0; col < 112; col++ ){
		for( int row = 0; row < 112; row++ ){
			int curFineTile = FineMap[ row ][ col ];
			ushort* tile = TileMap[ IsometricMapOffset(row, col) ].tile;
			if( curFineTile ){
				ushort* mine = (ushort*)&CurrentLevelMINFile[32 * (curFineTile - 1)]; // CurrentLevelMINFile и остальные три - массивы структур?
				for( int i = 0; i < 16; i++ ){
					tile[i] = mine[14 - (i & 14) + (i & 1)]; // big end decode
				}
			}else{
				memset(tile, 0, sizeof( TileColumn ));
			}
		}
	}
}

//----- (0047509F) --------------------------------------------------------
void __fastcall LoadTownSector( TileSquare* tilesFilePtr, char* dunFilePtr, int startRow, int startCol, int rowSize, int colSize )
{
	ushort* dunMap = (ushort*)(dunFilePtr + 4);
	int dunMapIndex = 0;
	for( int col = startCol; col < startCol + 2 * colSize; col += 2 ){
		for( int row = startRow; row < startRow + 2 * rowSize; row += 2 ){
			Cell fineMap(FineMap, row, col);
			ushort mapValue = dunMap[dunMapIndex++];
			int up, left, right, down;
			if( mapValue ){
				TileSquare& tileSquare = tilesFilePtr[mapValue - 1];
				up = tileSquare.up + 1;
				left = tileSquare.left + 1;
				right = tileSquare.right + 1;
				down = tileSquare.down + 1;
			}else{
				up = 0;
				left = 0;
				right = 0;
				down = 0;
			}
			fineMap(0,0) = up;
			fineMap(1,0) = left;
			fineMap(0,1) = right;
			fineMap(1,1) = down;
		}
	}
}

//----- (00475155) --------------------------------------------------------
void __fastcall SetTileSquare(TileSquare* tiles, int row, int col, int tileNumber)
{
	TileSquare& tileSquare = tiles[tileNumber - 1];
	FineMap[ row ][ col ] = tileSquare.up + 1;
	FineMap[ row + 1 ][ col ] = tileSquare.left + 1;
	FineMap[ row ][ col + 1 ] = tileSquare.right + 1;
	FineMap[ row + 1 ][ col + 1 ] = tileSquare.down + 1;
}

//----- (004751C6) --------------------------------------------------------
void FillDMap3OpenedAbyss()
{
	FineMap[ 78 ][ 60 ] = 1162;
	FineMap[ 79 ][ 60 ] = 1163;
	FineMap[ 78 ][ 61 ] = 1164;
	FineMap[ 79 ][ 61 ] = 1294;
	FineMap[ 78 ][ 62 ] = 1262;
	FineMap[ 78 ][ 63 ] = 1264;
	FineMap[ 79 ][ 62 ] = 1296;
	FineMap[ 79 ][ 63 ] = 1297;
	FineMap[ 79 ][ 64 ] = 1298;
	FineMap[ 78 ][ 64 ] = 282;
	FineMap[ 78 ][ 65 ] = 284;
	FineMap[ 79 ][ 65 ] = 285;
	FineMap[ 80 ][ 60 ] = 1299;
	FineMap[ 80 ][ 61 ] = 1301;
	FineMap[ 81 ][ 61 ] = 1302;
	FineMap[ 82 ][ 60 ] = 1303;
	FineMap[ 83 ][ 60 ] = 1304;
	FineMap[ 82 ][ 61 ] = 1305;
	FineMap[ 83 ][ 61 ] = 1306;
	FineMap[ 80 ][ 62 ] = 1307;
	FineMap[ 81 ][ 62 ] = 1308;
	FineMap[ 80 ][ 63 ] = 1309;
	FineMap[ 81 ][ 63 ] = 1310;
	FineMap[ 80 ][ 64 ] = 1311;
	FineMap[ 81 ][ 64 ] = 1312;
	FineMap[ 80 ][ 65 ] = 1313;
	FineMap[ 81 ][ 65 ] = 1314;
	FineMap[ 82 ][ 64 ] = 1319;
	FineMap[ 83 ][ 64 ] = 1320;
	FineMap[ 82 ][ 65 ] = 1321;
	FineMap[ 83 ][ 65 ] = 1322;
	FineMap[ 82 ][ 62 ] = 1315;
	FineMap[ 83 ][ 62 ] = 1316;
	FineMap[ 82 ][ 63 ] = 1317;
	FineMap[ 83 ][ 63 ] = 1318;
	FineMap[ 84 ][ 61 ] = 280;
	FineMap[ 84 ][ 62 ] = 280;
	FineMap[ 84 ][ 63 ] = 280;
	FineMap[ 85 ][ 60 ] = 280;
	FineMap[ 85 ][ 61 ] = 280;
	FineMap[ 85 ][ 63 ] = 8;
	FineMap[ 85 ][ 64 ] = 8;
	FineMap[ 86 ][ 60 ] = 217;
	FineMap[ 86 ][ 61 ] = 24;
	FineMap[ 85 ][ 62 ] = 19;
	FineMap[ 84 ][ 64 ] = 280;
	InitIsometricTileMap();
}

//----- (00475379) --------------------------------------------------------
void FillTownMap1ClosedAbyss()
{
	FineMap[ 78 ][ 60 ] = 1162;
	FineMap[ 79 ][ 60 ] = 1259;
	FineMap[ 78 ][ 61 ] = 1260;
	FineMap[ 79 ][ 61 ] = 1261;
	FineMap[ 78 ][ 62 ] = 1262;
	FineMap[ 79 ][ 62 ] = 1263;
	FineMap[ 78 ][ 63 ] = 1264;
	FineMap[ 79 ][ 63 ] = 1265;
	FineMap[ 78 ][ 64 ] = 1266;
	FineMap[ 79 ][ 64 ] = 1267;
	FineMap[ 78 ][ 65 ] = 1268;
	FineMap[ 80 ][ 60 ] = 1269;
	FineMap[ 81 ][ 60 ] = 1270;
	FineMap[ 80 ][ 61 ] = 1271;
	FineMap[ 81 ][ 61 ] = 1272;
	FineMap[ 82 ][ 60 ] = 1273;
	FineMap[ 83 ][ 60 ] = 1274;
	FineMap[ 82 ][ 61 ] = 1275;
	FineMap[ 83 ][ 61 ] = 1276;
	FineMap[ 80 ][ 62 ] = 1277;
	FineMap[ 81 ][ 62 ] = 1278;
	FineMap[ 80 ][ 63 ] = 1279;
	FineMap[ 81 ][ 63 ] = 1280;
	FineMap[ 80 ][ 64 ] = 1281;
	FineMap[ 81 ][ 64 ] = 1282;
	FineMap[ 80 ][ 65 ] = 1283;
	FineMap[ 81 ][ 65 ] = 1284;
	FineMap[ 82 ][ 64 ] = 1289;
	FineMap[ 83 ][ 64 ] = 1290;
	FineMap[ 82 ][ 65 ] = 1291;
	FineMap[ 83 ][ 65 ] = 1292;
	FineMap[ 82 ][ 62 ] = 1285;
	FineMap[ 83 ][ 62 ] = 1286;
	FineMap[ 82 ][ 63 ] = 1287;
	FineMap[ 83 ][ 63 ] = 1288;
	FineMap[ 84 ][ 61 ] = 280;
	FineMap[ 84 ][ 62 ] = 280;
	FineMap[ 84 ][ 63 ] = 280;
	FineMap[ 85 ][ 60 ] = 280;
	FineMap[ 85 ][ 61 ] = 280;
	FineMap[ 85 ][ 63 ] = 8;
	FineMap[ 85 ][ 64 ] = 8;
	FineMap[ 86 ][ 60 ] = 217;
	FineMap[ 86 ][ 61 ] = 24;
	FineMap[ 85 ][ 62 ] = 19;
	FineMap[ 84 ][ 64 ] = 280;
	InitIsometricTileMap();
}

//----- (0047552C) --------------------------------------------------------
void FillTownMap2ClosedCrypt()
{
	FineMap[ 36 ][ 21 ] = 1323;
	FineMap[ 37 ][ 21 ] = 1324;
	FineMap[ 36 ][ 22 ] = 1325;
	FineMap[ 37 ][ 22 ] = 1326;
	FineMap[ 36 ][ 23 ] = 1327;
	FineMap[ 37 ][ 23 ] = 1328;
	FineMap[ 36 ][ 24 ] = 1329;
	FineMap[ 37 ][ 24 ] = 1330;
	FineMap[ 35 ][ 21 ] = 1339;
	FineMap[ 34 ][ 21 ] = 1340;
	InitIsometricTileMap();
}

//----- (00475595) --------------------------------------------------------
void FillDMap1OpenedCrypt()
{
	FineMap[ 36 ][ 21 ] = 1331;
	FineMap[ 37 ][ 21 ] = 1332;
	FineMap[ 36 ][ 22 ] = 1333;
	FineMap[ 37 ][ 22 ] = 1334;
	FineMap[ 36 ][ 23 ] = 1335;
	FineMap[ 37 ][ 23 ] = 1336;
	FineMap[ 36 ][ 24 ] = 1337;
	FineMap[ 37 ][ 24 ] = 1338;
	FineMap[ 35 ][ 21 ] = 1339;
	FineMap[ 34 ][ 21 ] = 1340;
	InitIsometricTileMap();
}

//----- (0071B300) --------------------------------------------------------
void __fastcall FillDMapClosedDisusedReliquary(int row, int col)
{
	FineMap[ row + 2 ][ col     ] = 1323;
	FineMap[ row + 3 ][ col     ] = 1324;
	FineMap[ row + 2 ][ col + 1 ] = 1325;
	FineMap[ row + 3 ][ col + 1 ] = 1326;
	FineMap[ row + 2 ][ col + 2 ] = 1327;
	FineMap[ row + 3 ][ col + 2 ] = 1328;
	FineMap[ row + 2 ][ col + 3 ] = 1329;
	FineMap[ row + 3 ][ col + 3 ] = 1330;
	FineMap[ row + 1 ][ col     ] = 1339;
	FineMap[ row     ][ col     ] = 1340;
	InitIsometricTileMap();
}

//----- (0071B380) --------------------------------------------------------
void __fastcall FillDMapOpenedDisusedReliquary(int row, int col)
{
	FineMap[ row + 2 ][ col     ] = 1331;
	FineMap[ row + 3 ][ col     ] = 1332;
	FineMap[ row + 2 ][ col + 1 ] = 1333;
	FineMap[ row + 3 ][ col + 1 ] = 1334;
	FineMap[ row + 2 ][ col + 2 ] = 1335;
	FineMap[ row + 3 ][ col + 2 ] = 1336;
	FineMap[ row + 2 ][ col + 3 ] = 1337;
	FineMap[ row + 3 ][ col + 3 ] = 1338;
	FineMap[ row + 1 ][ col     ] = 1339;
	FineMap[ row     ][ col     ] = 1340;
	InitIsometricTileMap();
}

//----- (00716600) --------------------------------------------------------
void __fastcall LoadSPTownMap(TileSquare* townTiles)
{
	Player& player = Players[CurrentPlayerIndex];
	auto& clvlReq = ClvlReqsForEnterInDungeon[ Difficulty ];
	if( !(player.CharLevel >= clvlReq[ DD_1_CATACOMB ] || player.OpenedTownDoorMask & BIT(DD_1_CATACOMB))){ // закрыть катакомбы
		SetTileSquare(townTiles, 48, 20, 320);
	}

	if( !(player.CharLevel >= clvlReq[ DD_2_CAVE ] || player.OpenedTownDoorMask & BIT(DD_2_CAVE))){ // закрыть пещеры
		SetTileSquare(townTiles, 16, 68, 332);
		SetTileSquare(townTiles, 16, 70, 331);
	}

	if( !(player.CharLevel >= clvlReq[ DD_3_HELL ] || player.OpenedTownDoorMask & BIT(DD_3_HELL)) ){ // закрыть ад
		for( int row = 36; row < 46; row++ ){
			SetTileSquare(townTiles, row, 78, RNG(4) + 1);// случайные тайлы травы
		}
	}

	if( player.CharLevel >= clvlReq[ DD_4_ABYSS ] || player.OpenedTownDoorMask & BIT(DD_4_ABYSS) ){
		FillDMap3OpenedAbyss();
	}else{
		FillTownMap1ClosedAbyss();
	}

	if( player.CharLevel >= clvlReq[ DD_5_CRYPT ] || player.OpenedTownDoorMask & BIT(DD_5_CRYPT) ){
		FillDMap1OpenedCrypt();
	}else{
		FillTownMap2ClosedCrypt();
	}
}

//----- (004755FE) --------------------------------------------------------
void LoadTownMap()
{
	for( int col = 0; col < 112; ++col ){
		for( int row = 0; row < 112; ++row ){
			FineMap[ row ][ col ] = 0;
		}
	}

	auto townTiles = (TileSquare*)LoadFile("Levels\\TownData\\Town.TIL");
	
	char* townSector = (char*)LoadFile("Levels\\TownData\\Sector1s.DUN");
	LoadTownSector(townTiles, townSector, 46, 46, 25, 25);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector2s.DUN");
	LoadTownSector(townTiles, townSector, 46, 0, 25, 23);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector3s.DUN");
	LoadTownSector(townTiles, townSector, 0, 46, 23, 25);
	FreeMem(townSector);

	townSector = (char*)LoadFile("Levels\\TownData\\Sector4s.DUN");
	LoadTownSector(townTiles, townSector, 0, 0, 23, 23);
	FreeMem(townSector);

	Player& player = Players[CurrentPlayerIndex];
	if( MaxCountOfPlayersInGame == 1 ){
		LoadSPTownMap(townTiles);
	}else{
		FillDMap3OpenedAbyss();
		FillDMap1OpenedCrypt();
	}

	Quest& poisonedWaterSupplyQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
	int tileNumber;
	if( poisonedWaterSupplyQuest.status != QS_3_COMPLETE && poisonedWaterSupplyQuest.status ){
		tileNumber = 342;
	}else{
		tileNumber = 71;
	}
	SetTileSquare(townTiles, 60, 70, tileNumber);

	Quest& lairQuest = Quests[Q_23_INFESTED_CELLAR];
	if( lairQuest.status > QS_1_ACCEPT || (player.dungeonVisited[DUN_9_CAVE_1] && lairQuest.status) ){
		SetTileSquare(townTiles, 70, 78, 311);
		SetTileSquare(townTiles, 72, 78, 310);
		SetTileSquare(townTiles, 74, 78, 313);
		SetTileSquare(townTiles, 72, 80, 309);
		SetTileSquare(townTiles, 74, 80, 312);
		lairQuest.row = 73;
		lairQuest.col = 80;
	}

	Quest& ditt_Quest = Quests[Q_27_DEMON_IN_THE_TOWN];
	if (ditt_Quest.status > QS_1_ACCEPT || (player.dungeonVisited[DUN_4_CHURCH_4] && ditt_Quest.status)) {
		//SetTileSquare(townTiles, 27, 48, 311);
		//SetTileSquare(townTiles, 29, 48, 310);
		//SetTileSquare(townTiles, 31, 48, 313);
		//SetTileSquare(townTiles, 29, 50, 309);
		//SetTileSquare(townTiles, 31, 50, 312);
		ditt_Quest.row = 31;
		ditt_Quest.col = 50;
	}

	Quest& andiQuest = Quests[Q_22_MAIDEN_OF_ANGUISH];
	if( andiQuest.status ){
		SetTileSquare(townTiles, 76, 12, 64);
		SetTileSquare(townTiles, 78, 12, 65);
		SetTileSquare(townTiles, 80, 12, 66);
		if( andiQuest.status > QS_1_ACCEPT || player.dungeonVisited[DUN_12_CAVE_4] ){
			SetTileSquare(townTiles, 76, 12, 67);
			SetTileSquare(townTiles, 78, 12, 68);
			SetTileSquare(townTiles, 80, 12, 69);
			andiQuest.row = 80;
			andiQuest.col = 14;
		}
	}

	Quest& treasureQuest = Quests[Q_19_LOST_TREASURE];
	if( treasureQuest.status ){
		FillDMapClosedDisusedReliquary (18, 30);
		if( treasureQuest.status2 > 2 ){
			FillDMapOpenedDisusedReliquary (18, 30);
		}
	}

	// Do not need to set stair entrance to Ravenholm since there is already a red portal that disappears after quest is done.
	// Without setting the stair coordinates, it will spawn at the (0,0) position on the map (unintialized variable in Windows).

	FreeMem(townTiles);
}

//----- (0047587C) --------------------------------------------------------
void __fastcall CreateTown( int lvlDir )
{
	MapStartRow = 10;
	MapStartCol = 10;
	MapEndRow = 84;
	MapEndCol = 84;

	if( lvlDir == LVL_0_DOWN ){ // start new game pos
		PlayerRowPos = 56;
		PlayerColPos = 77;
	}else if( lvlDir == LVL_1_UP ){
		PlayerRowPos = 25;
		PlayerColPos = 31;
	}else if( lvlDir == LVL_7_TWARPUP ){
		switch( Dungeon.prev ){
			case DUN_5_CATACOMB_1: PlayerRowPos = 49; PlayerColPos = 22; break;
			case DUN_9_CAVE_1    : PlayerRowPos = 18; PlayerColPos = 69; break;
			case DUN_13_HELL_1   : PlayerRowPos = 41; PlayerColPos = 81; break;
			case DUN_17_ABYSS_1  : PlayerRowPos = 79; PlayerColPos = 62; break;
			case DUN_21_CRYPT_1  : PlayerRowPos = 36; PlayerColPos = 25; break;
		}
		if( Dungeon == DUN_44_RAVENHOLM ){
			PlayerRowPos = 54; PlayerColPos = 63;
		}
	}
	LoadTownMap();
	memzero(LightMap);
	memzero(FlagMap);
	memzero(PlayerMap);
	memzero(MonsterMap);
	memzero(ObjectsMap);
	memzero(ItemMap);
	memzero(ArchGraphicsMap);
	
	for( int i = 0; i < 112 * 112; i++ ){
		char& archGraphicsMap = ArchGraphicsMap[ 0 ][ i ];
		switch( FineMap[ 0 ][ i ] ){
			case 360:			archGraphicsMap = 1;			break;
			case 358:			archGraphicsMap = 2;			break;
			case 129:			archGraphicsMap = 6;			break;
			case 130:			archGraphicsMap = 7;			break;
			case 128:			archGraphicsMap = 8;			break;
			case 117:			archGraphicsMap = 9;			break;
			case 157:			archGraphicsMap = 10;			break;
			case 158:			archGraphicsMap = 11;			break;
			case 156:			archGraphicsMap = 12;			break;
			case 162:			archGraphicsMap = 13;			break;
			case 160:			archGraphicsMap = 14;			break;
			case 214:			archGraphicsMap = 15;			break;
			case 212:			archGraphicsMap = 16;			break;
			case 217:			archGraphicsMap = 17;			break;
			case 216:			archGraphicsMap = 18;			break;
		}
	}
	InitIsometricTileMap();
}

//----- (00475AE0) --------------------------------------------------------
int __fastcall GetTowner( int townerIdentificator )
{
	for( int townerIndex = 0; townerIndex < TownersCount; townerIndex++ ){
		if( Towners[townerIndex].TownerIdentificator == townerIdentificator ){
			return townerIndex;
		}
	}
	return 0; // prevent out of Towner array access, was -1;
}

//----- (00475B06) --------------------------------------------------------
void __fastcall SetupCowAnimation( char* cowCELFile, char** animationArray )
{
	int* animationOffsets = (int*)cowCELFile;
	for( int direction = 0; direction < 8; ++direction ){
		animationArray[direction] = &cowCELFile[animationOffsets[direction]];
	}
}

//----- (00475B40) --------------------------------------------------------
void __fastcall InitTownerAnimation( int townerIndex, char* townerAnimationPtr, int stayFrameCount, int delayFrameCount )
{
	Towner& towner = Towners[townerIndex];
	towner.stayAnim = townerAnimationPtr;
	towner.StayingFrameCount = stayFrameCount;
	towner.CurFrame = 1;
	towner.CurDelayFrame = 0;
	towner.DelayFrameCount = delayFrameCount;
}

//----- (00475B73) --------------------------------------------------------
void __fastcall InitTownerStruct( int townerIndex, int maybePicSize, int maybeHasDialog, int townerIdentificator, int row, int col, char framesOrderTypeIndex, int always10 )
{
	Towner& towner = Towners[townerIndex];
	memset(&towner, 0, sizeof(Towner));
	towner.field_54 = maybeHasDialog;
	towner.animWidth = maybePicSize;
	towner.offsetX = (maybePicSize - 64) / 2;
	towner.IsTownerActionSelected = false;
	towner.TownerIdentificator = townerIdentificator;
	towner.Row = row;
	towner.Col = col;
	MonsterMap[ row ][ col ] = townerIndex + 1;
	towner.FramesOrderTypeIndex = framesOrderTypeIndex;
	towner.reserved = always10;
	towner.seed = GetRndSeed();
}

//----- (00475C05) --------------------------------------------------------
void __fastcall SetTownerQuestInfo(int townerIndex)
{
	return;
}

//----- (00475C52) --------------------------------------------------------
void LoadGriswold()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_0_GRISWOLD, 62, 63, 0, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Smith\\SmithN.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 16;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 16, 3);
	strcpy(towner.Name, "Griswold the Blacksmith");
	TownersCount++;
}

//----- (00475CE9) --------------------------------------------------------
void LoadOgden()
{
	Towner& towner = Towners[TownersCount];
	SomeBoolFlag_41 = 0; // Только присваивается значение, нигде не используется
	InitTownerStruct(TownersCount, 96, 1, TI_3_OGDEN, 55, 62, 3, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TwnF\\TwnFN.CEL");
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 16;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 16, 3);
	strcpy(towner.Name, "Ogden the Tavern owner");
	TownersCount++;
}

//----- (00475D87) --------------------------------------------------------
void LoadWoundedTownsman()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_2_WOUNDED_GUY, 24, 32, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Butch\\Deadguy.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 8;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[4], 8, 6);
	strcpy(towner.Name, "Wounded Townsman");
	TownersCount++;
}

//----- (00475E1F) --------------------------------------------------------
void LoadAdria()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_6_ADRIA, 44, 68, 5, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownWmn1\\Witch.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 19;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 19, 6);
	strcpy(towner.Name, "Adria the Witch");
	TownersCount++;
}

//----- (00475EB6) --------------------------------------------------------
void LoadGilian()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_7_GILLIAN, 55, 44, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownWmn1\\WmnN.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 18;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 18, 6);
	strcpy(towner.Name, "Gillian the Stasher");
	TownersCount++;
}

//----- (00475F4D) --------------------------------------------------------
void LoadWirt()
{
	Towner& towner = Towners[TownersCount];
	WirtIsPlacedOnMap = 1;
	InitTownerStruct(TownersCount, 96, 1, TI_8_WIRT, 56, 58, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\TownBoy\\PegKid1.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 20, 6);
	strcpy(towner.Name, "Wirt the Peg-legged boy");
	TownersCount++;
}

//----- (00475FEB) --------------------------------------------------------
void LoadPepin()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_1_PEPIN, 55, 79, 1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Healer\\Healer.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[7], 20, 6);
	strcpy(towner.Name, "Pepin the Healer");
	TownersCount++;
}

//----- (00476082) --------------------------------------------------------
void LoadCain()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_4_CAIN, 62, 71, 2, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Strytell\\Strytell.cel");
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 25;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 25, 3);
	strcpy(towner.Name, "Cain the Elder");
	TownersCount++;
}

//----- (00476119) --------------------------------------------------------
void LoadFarnham()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_5_FARNHAM, 75, 71, 4, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Drunk\\TwnDrunk.CEL");

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 18;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 18, 3);
	strcpy(towner.Name, "Farnham the Drunk");
	TownersCount++;
}

//----- (004761B0) --------------------------------------------------------
void LoadCows()
{
	CowSprite = (char*)LoadFile("Towners\\Animals\\Cow.CEL");

	for( int cowIndex = 0; cowIndex < 3; ++cowIndex ){
		Towner& cow = Towners[TownersCount];
		int cowRow = CowsRows[cowIndex];
		int cowCol = CowsCols[cowIndex];
		InitTownerStruct(TownersCount, 128, 0, TI_9_COWS, cowRow, cowCol, -1, 10);
		cow.spritePtr = CowSprite;
		SetupCowAnimation(CowSprite, cow.animationFrameArray);
		cow.animationFrameCount = 12;
		InitTownerAnimation(TownersCount, cow.animationFrameArray[CowsOrients[cowIndex]], 12, 3);
		cow.CurFrame = RNG(11) + 1;
		cow.field_54 = 1;

		strcpy(cow.Name, "Cow");

		int someRow = cowRow + CowsDeltaRow[CowsOrients[cowIndex]];
		int someCol = cowCol + CowsDeltaCol[CowsOrients[cowIndex]];

		if( !MonsterMap[ cowRow ][ someCol ] ){
			MonsterMap[ cowRow ][ someCol ] = -1 - TownersCount;
		}
		if( !MonsterMap[ someRow ][ cowCol ] ){
			MonsterMap[ someRow ][ cowCol ] = -1 - TownersCount;
		}
		if( !MonsterMap[ someRow ][ someCol ] ){
			MonsterMap[ someRow ][ someCol ] = -1 - TownersCount;
		}

		TownersCount++;
	}

}

//----- (00476308) --------------------------------------------------------
void LoadLester()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_10_LESTER, 77, 19, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Farmer\\Farmrn2.CEL");
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 15;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 15, 3);
	strcpy(towner.Name, "Lester the farmer");
	TownersCount++;
}

//----- (0047639F) --------------------------------------------------------
void LoadNut()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_12_NUT, 77, 19, -1, 10);
	SetTownerQuestInfo(TownersCount);

	char* fileName;
	if( Quests[Q_23_INFESTED_CELLAR].status == QS_3_COMPLETE ){
		fileName = "Towners\\Farmer\\mfrmrn2.CEL";
	}else{
		fileName = "Towners\\Farmer\\cfrmrn2.CEL";
	}

	towner.spritePtr = (char*)LoadFile(fileName);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 15;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[1], 15, 3);
	strcpy(towner.Name, "Complete Nut");
	TownersCount++;
}

//----- (0047644B) -------------------------------------------------------
void LoadCelia()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_11_CELIA, 64, 42, -1, 10);
	SetTownerQuestInfo(TownersCount);
	
	char* fileName;
	if( Quests[Q_18_LITTLE_GIRL].status == QS_3_COMPLETE ){
		fileName = "Towners\\Girl\\Girls2.CEL";
	}else{
		fileName = "Towners\\Girl\\Girlw2.CEL";
	}
	towner.spritePtr = (char*)LoadFile(fileName);

	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 20, 6);
	strcpy(towner.Name, "Celia the little girl");
	TownersCount++;
}

//----- (0071E500) --------------------------------------------------------
void LoadPriest()
{
	Towner& towner = Towners[TownersCount];
	// tremain для fleshdoom есть всегда, если он "отключен", то ставиться в верхнем углу города (2, 2)
	InitTownerStruct(TownersCount, 96, 1, TI_13_TREMAIN, 55, 68, -1, 10);
	SetTownerQuestInfo(TownersCount);
	towner.spritePtr = (char*)LoadFile("Towners\\Priest\\priest8.cel");
	
	for( int direction = 0; direction < 8; ++direction ){
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 33;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 33, 13);
	strcpy(towner.Name, "Tremain the Priest");
	TownersCount++;
}

void LoadColiseumGirl()
{
	Towner& towner = Towners[TownersCount];
	InitTownerStruct(TownersCount, 96, 1, TI_14_COLISEUM, /*55*/ 53, /*72*/ 50, -1, 10);
	towner.spritePtr = (char*)LoadFile("NPC\\lostnpc_town.cel");

	for (int direction = 0; direction < 8; ++direction) {
		towner.animationFrameArray[direction] = towner.spritePtr;
	}

	towner.animationFrameCount = 20;
	InitTownerAnimation(TownersCount, towner.animationFrameArray[0], 20, 6);
	strcpy(towner.Name, "Sepia the Munerarius");
	TownersCount++;
}

//----- (004764F7) --------------------------------------------------------
void InitTowners()
{
	TownersCount = 0;
	WirtIsPlacedOnMap = 0;
	LoadGriswold();
	LoadPepin();
	uchar butcherQuestStatus = Quests[Q_6_THE_BUTCHER].status;
	if( butcherQuestStatus != QS_0_NO && butcherQuestStatus != QS_3_COMPLETE ){
		LoadWoundedTownsman();
	}
	LoadOgden();
	LoadCain();
	LoadFarnham();
	LoadAdria();
	LoadGilian();
	LoadWirt();
	LoadCows();
	if (GameMode == GM_COLISEUM) {
		LoadColiseumGirl();
	}
	if (CowQuestOn) {
		LoadNut();
		LoadPriest();
//	}else if( MaxCountOfPlayersInGame == 1
//	 && Players[CurrentPlayerIndex].dungeonVisited[ DUN_13_HELL_1 ]
//	 && Quests[ Q_21_FLESHDOOM ].status != QS_0_NO
//	 && Quests[ Q_21_FLESHDOOM ].status != QS_3_COMPLETE
//	 && Quests[ Q_21_FLESHDOOM ].status2 < 7 ){
//		LoadPriest();
//	}
	} else if ((Players[CurrentPlayerIndex].dungeonVisited[DUN_13_HELL_1] || Players[CurrentPlayerIndex].dungeonVisited[DUN_14_HELL_2] || Players[CurrentPlayerIndex].dungeonVisited[DUN_41_FLESHDOOM])
		&& Quests[Q_21_THE_CURSED_BLADE].status != QS_0_NO
		&& Quests[Q_21_THE_CURSED_BLADE].status != QS_3_COMPLETE
		&& Quests[Q_21_THE_CURSED_BLADE].status2 <= 7) { // Was .status2 < 7.  .status2 = 7 while running the InquisitionScript().  This call precedes the script.  In SP, Priest will always be present before burning, so Griswold is safe.  However, these checks will fail in MP if new player just joined game while the script is running, cuz dungeons are not visited, and Griswold will burn instead.  The MP case is handled inside the script, which is a little more risky but gets the job done.
		LoadPriest();
	}
	if( SomeBoolFlag_18 && MaxCountOfPlayersInGame == 1 ){
		if( Players[CurrentPlayerIndex].dungeonVisited[ DUN_17_ABYSS_1 ] ){
			LoadCelia();
		}
	}
}

//----- (0047657E) --------------------------------------------------------
void FreeTowners()
{
	for( int townerIndex = 0; townerIndex < 16; ++townerIndex ){
		Towner& towner = Towners[townerIndex];
		if( towner.spritePtr && towner.spritePtr != CowSprite ){
			FreeMem(towner.spritePtr);
		}
		towner.spritePtr = 0;
	}
	FreeMemZero(CowSprite);
}

//----- (004765C1) --------------------------------------------------------
void __fastcall StopTownerSpeechWhenPlayerGoAway(int townerIndex)
{
	Towner& towner = Towners[townerIndex];
	if( towner.isThisTownerSpeakingNow ){
		Player& player = Players[towner.indexOfPlayerWhichSpeaksTownsman];
		int deltaRow = abs(towner.Row - player.Row);
		int deltaCol = abs(towner.Col - player.Col);
		if( deltaRow >= 2 || deltaCol >= 2 ){
			towner.isThisTownerSpeakingNow = 0;
			Speech_IsPanelVisible = 0;
			StopSpeech();
		}
	}
}

//----- (0047664C) --------------------------------------------------------
void SlainTownsmanAction()
{
	Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
	int townerIndex = GetTowner(TI_2_WOUNDED_GUY);
	Towner& towner = Towners[townerIndex];
	StopTownerSpeechWhenPlayerGoAway(townerIndex);
	if( Speech_IsPanelVisible ){
		if( butcherQuest.status != QS_1_ACCEPT ){
			towner.CurDelayFrame = 0;
		}
	}else if( (butcherQuest.status != QS_2_IN_PROGRESS || butcherQuest.talkToFlag) && butcherQuest.status != QS_1_ACCEPT ){
		towner.DelayFrameCount = 1000;
		towner.CurFrame = 1;
		strcpy(towner.Name, "Slain Townsman");
		towner.CurDelayFrame = 0;
	}
}

//----- (00476754) --------------------------------------------------------
void ProcessTowners()
{
	for( int townerIndex = 0; townerIndex < 16; ++townerIndex ){
		Towner& towner = Towners[townerIndex];
		//if( towner.TownerIdentificator <= 12 ){ // в оригинале нет этой проверки
			switch( towner.TownerIdentificator ){
			case TI_0_GRISWOLD   : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_0_GRISWOLD)); break;
			case TI_3_OGDEN      : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_3_OGDEN));    break;
			case TI_2_WOUNDED_GUY: SlainTownsmanAction();                                      break;
			case TI_1_PEPIN      : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_1_PEPIN));    break;
			case TI_4_CAIN       : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_4_CAIN));     break;
			case TI_5_FARNHAM    : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_5_FARNHAM));  break;
			case TI_8_WIRT       : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_8_WIRT));     break;
			case TI_6_ADRIA      : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_6_ADRIA));    break;
			case TI_7_GILLIAN    : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_7_GILLIAN));  break;
			case TI_9_COWS       : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_9_COWS));     break; // косяк. Находит только первую корову. А говорить может игрок с любой из них
			case TI_10_LESTER    : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_10_LESTER));  break;
			case TI_11_CELIA     : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_11_CELIA));   break;
			case TI_12_NUT       : StopTownerSpeechWhenPlayerGoAway(GetTowner(TI_12_NUT));     break;
			}
		//}
		if( ++towner.CurDelayFrame >= towner.DelayFrameCount ){ // TODO: change to > and fix the Towners animDelay data
			towner.CurDelayFrame = 0;
			if( towner.FramesOrderTypeIndex < 0 ){// последовательная анимация
				if( ++towner.CurFrame > towner.StayingFrameCount ){
					towner.CurFrame = 1;
				}
			}else{// специальная анимация
				towner.FramesOrderCurrentIndex++;
				if( TownerAnimationFramesOrder[ towner.FramesOrderTypeIndex ][ towner.FramesOrderCurrentIndex ] == -1 ){
					towner.FramesOrderCurrentIndex = 0;
				}
				towner.CurFrame = TownerAnimationFramesOrder[ towner.FramesOrderTypeIndex][ towner.FramesOrderCurrentIndex ];
			}
		}
	}
}

//----- (0047685D) --------------------------------------------------------
Item* __fastcall FindItemInInventory(int playerIndex, int baseItemIndex, int& itemInventoryIndex)
{
	Player& player = Players[playerIndex];
	for( int currentItemIndex = 0; currentItemIndex < player.InvItemCount; ++currentItemIndex ){
		Item& item = player.InventorySlots[currentItemIndex];
		if( item.baseItemIndex == baseItemIndex ){
			itemInventoryIndex = currentItemIndex;
			return &item;
		}
	}
	return 0;
}

//----- (004768C1) --------------------------------------------------------
void __fastcall NPCGreeting(int speechIndex)
{
	CowClickCount = 0;
	CowClickPlayerSoundIndex = 0;
	NPCMenusEnabled = true;
	StartSpeech(speechIndex);
}

//!---- (00476D52) --------------------------------------------------------
bool GriswoldGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	
	//if( MaxCountOfPlayersInGame == 1 ){
		Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];
		Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
		if( player.dungeonVisited[DUN_4_CHURCH_4]
			&& magicRockQuest.status 
			&& !magicRockQuest.status3 ){
			    if( !needAction ) { return true; }
				magicRockQuest.status3 = 1;
				magicRockQuest.talkToFlag = true;
				if( magicRockQuest.status == QS_1_ACCEPT ){
					magicRockQuest.status = QS_2_IN_PROGRESS;
					magicRockQuest.status2 = 1;
				}
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_115);
				towner.IsTownerActionSelected = true;
		}

		int questItemInventoryIndex;
		if( !towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, /*GameMode == GM_CLASSIC ? BI(96) :*/ BI_9_MAGIC_ROCK, questItemInventoryIndex) ){
			    if( !needAction ) { return true; }
				magicRockQuest.status = QS_3_COMPLETE;
				magicRockQuest.status2 = 2;
				magicRockQuest.status3 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				int uniqIndex;
				if (GameMode == GM_CLASSIC) { 
					uniqIndex = 4677 + Difficulty; 
				}
				else {
					switch (Difficulty) {
					case DL_0_HORROR:		uniqIndex = 152;	break;// Empyrean Bands, by difficulty
					case DL_1_PURGATORY:	uniqIndex = 153;	break;
					case DL_2_DOOM:			uniqIndex = 154;	break;
					case DL_3_CRUCIBLE:		uniqIndex = 2383;	break;
					case DL_4_TORMENT:		uniqIndex = 2384;	break;
					case DL_5_AGONY:		uniqIndex = 2385;	break;
					case DL_6_TERROR:		uniqIndex = 2386;	break;
					case DL_7_DESTRUCTION:	uniqIndex = 2387;	break;
					case DL_8_INFERNO:		uniqIndex = 2388;	break;
					}
				}
				SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_117);
				towner.IsTownerActionSelected = true;
		}
		
		if( (player.dungeonVisited[DUN_9_CAVE_1] || player.dungeonVisited[DUN_10_CAVE_2]) // Player arriving at Dlvl 10 without going to Dlvl 9 should also trigger quest.
			&& anvilOfFuryQuest.status 
			&& (anvilOfFuryQuest.status == QS_1_ACCEPT || anvilOfFuryQuest.status == QS_2_IN_PROGRESS)
			&& !anvilOfFuryQuest.status3
			&& !towner.IsTownerActionSelected){
				// In MP, the condition below is true even if the Magic Rock quest is disabled, as .status3 is initialized to 2 in ResetQuests().
				// mordor: i have commented out the checks for magic rock quests, (SP) tests showed they were preventing the quest from properly initializing,
				// griswold didnt start his speech, only reacted to player giving him the anvil. if this (for some reason) needs to be re-activated, code is intact (not deleted)
				//if( magicRockQuest.status3 == 2 || 
				//	magicRockQuest.status == QS_2_IN_PROGRESS && magicRockQuest.status3 == 1 ){// TODO: непонятно при чем тут проверка на квест магического камня
					    if( !needAction ) { return true; }
						anvilOfFuryQuest.status3 = 1;
						anvilOfFuryQuest.talkToFlag = true;
						if( anvilOfFuryQuest.status == QS_1_ACCEPT ){
							anvilOfFuryQuest.status = QS_2_IN_PROGRESS;
							anvilOfFuryQuest.status2 = 1;
						}
						NetSendCmdQuest(true, Q_10_ANVIL_OF_FURY);
						towner.isThisTownerSpeakingNow = 150;
						towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
						StartSpeech(SP_88);
						towner.IsTownerActionSelected = true;
				//}
		}
			
		if( !towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_16_ANVIL_OF_FURY, questItemInventoryIndex) ){
			    if( !needAction ) { return true; }
				anvilOfFuryQuest.status = QS_3_COMPLETE;
				anvilOfFuryQuest.status2 = 2;
				anvilOfFuryQuest.status3 = 2;
				NetSendCmdQuest(true, Q_10_ANVIL_OF_FURY); // This currently does nothing in MP, as quest is already completed at this point.
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				PLAYER_FULL_CLASS fullClassId = player.fullClassId;
				if (GameMode == GM_CLASSIC) {
					SpawnUnique(4683 + Difficulty, towner.Row, towner.Col + 1);
					goto SKIPPO;
				}
				if (player.fullClassId == PFC_ROGUE && HasTrait(playerIndex, TraitId::Axepertise)) {
					fullClassId = PFC_SHUGOKI;
				}
				/*else if (player.fullClassId == PFC_IRON_MAIDEN && HasTrait(playerIndex, TraitId::TwoTowers)) {
					fullClassId = PFC_BOMBARDIER;
				}*/
				else if (player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)) {
					fullClassId = PFC_WARRIOR;
				}
				else if (HasTrait(playerIndex, TraitId::Barbarian)) {
					SetPlayerHandItem(player.ItemOnCursor, UniqueItems[AnvilOfFuryUniqsTable[fullClassId][Difficulty]].baseId);
					Item_MakeNonCollisionSeed(CurrentPlayerIndex, player.ItemOnCursor);
					if( DropCursorItemBeforeTrig() ) FixItemCursor(-1);
				}

				if (!(HasTrait(playerIndex, TraitId::Barbarian))) {
					if (HasTrait(playerIndex, TraitId::Ranger)) {
						SpawnUnique(2549 + Difficulty, towner.Row, towner.Col + 1); // first Griswold's Bow (unique for Ranger) is index #2549 in unique table
					}
					else if (HasTrait(playerIndex, TraitId::Pistoleer)) {
						SpawnUnique(2735 + Difficulty, towner.Row, towner.Col + 1); 
					}
					else {
						SpawnUnique(AnvilOfFuryUniqsTable[fullClassId][Difficulty], towner.Row, towner.Col + 1);
					}
				}
				SKIPPO:
				//Item_CreateQuestUnique(anvilOfFuryUniqsTable[ player.ClassID ][ Difficulty ], towner.Row, towner.Col + 1);
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_90);
				towner.IsTownerActionSelected = true;
		}

		Quest& iron_Figurine_Quest = Quests[Q_29_IRON_FIGURINE];
		int questItemInventoryIndex2;
		if ((player.dungeonVisited[DUN_11_CAVE_3] || player.dungeonVisited[DUN_12_CAVE_4] || player.dungeonVisited[DUN_51_FETID_CAVERN])
			&& !towner.IsTownerActionSelected) {
			if (iron_Figurine_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				iron_Figurine_Quest.status = QS_2_IN_PROGRESS;
				iron_Figurine_Quest.talkToFlag = true;
				iron_Figurine_Quest.status2 = 1;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				iron_Figurine_Quest.speechIndex = SP_420;
				StartSpeech(SP_420);
				towner.IsTownerActionSelected = true;
			}
			else if (iron_Figurine_Quest.status == QS_3_COMPLETE
				&& FindItemInInventory(playerIndex, BI_2457_IRON_FIGURINE, questItemInventoryIndex2)
				&& iron_Figurine_Quest.status2 != 2) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				iron_Figurine_Quest.status2 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex2);
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_422);
				int uniqIndex = Difficulty + 2807;
				SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.IsTownerActionSelected = true;
			}
		}

		Quest& good_Deal_Quest = Quests[Q_31_GOOD_DEAL];
		int questItemInventoryIndex4;
		if ((player.dungeonVisited[DUN_7_CATACOMB_3] || player.dungeonVisited[DUN_53_VAULTKEEP])
			&& !towner.IsTownerActionSelected) {
			if (good_Deal_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				good_Deal_Quest.status = QS_2_IN_PROGRESS;
				good_Deal_Quest.talkToFlag = true;
				good_Deal_Quest.status2 = 1;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				good_Deal_Quest.speechIndex = SP_426;
				StartSpeech(SP_426);
				towner.IsTownerActionSelected = true;
			}
			else if (good_Deal_Quest.status == QS_3_COMPLETE
				&& FindItemInInventory(playerIndex, BI_2460_THE_RIGHT_GOODS, questItemInventoryIndex4)
				&& good_Deal_Quest.status2 != 2) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				good_Deal_Quest.status2 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex4);
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_428);
				int goldAmount = 4'000 + 2'000 * Difficulty;
				SpawnSomething(BI_0_GOLD, towner.Row, towner.Col, 0, 0, 0, 0, goldAmount);
				//int uniqIndex = Difficulty + 2825;
				//SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.IsTownerActionSelected = true;
			}
		}

		Quest& iron_Oath_Quest = Quests[Q_35_IRON_OATH];
		int questItemInventoryIndex5;
		if ((player.dungeonVisited[DUN_1_CHURCH_1] /*|| player.dungeonVisited[DUN_111_CHURCH_SACRISTY]*/)
			&& !towner.IsTownerActionSelected) {
			if (iron_Oath_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				iron_Oath_Quest.status = QS_2_IN_PROGRESS;
				iron_Oath_Quest.talkToFlag = true;
				iron_Oath_Quest.status2 = 1;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				iron_Oath_Quest.speechIndex = SP_437;
				StartSpeech(SP_437);
				towner.IsTownerActionSelected = true;
			}
			else if (iron_Oath_Quest.status == QS_3_COMPLETE
				&& FindItemInInventory(playerIndex, BI_2510_DUSTY_CRATE, questItemInventoryIndex5)
				&& iron_Oath_Quest.status2 != 2) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				iron_Oath_Quest.status2 = 2;
				ClearInventorySlot(playerIndex, questItemInventoryIndex5);
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_439);
				int goldAmount = 1'000 + 2'000 * Difficulty;
				SpawnSomething(BI_0_GOLD, towner.Row, towner.Col, 0, 0, 0, 0, goldAmount);
				towner.IsTownerActionSelected = true;
			}
		}
	//}
	
	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Griswold Greeting
		NPCGreeting(SP_188);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_1_BlacksmithMain);
		}
	}
	
	return false;
}

//----- (007154F0) --------------------------------------------------------
void __fastcall DropLairQuestAward (int row, int col)
{
	int qlvl;
	switch (Difficulty ){
		case DL_0_HORROR:		qlvl = 14;	break;
		case DL_1_PURGATORY:	qlvl = 19;	break;
		default:				qlvl = 14 + Difficulty * 5;	break;
	}
	CreatePepinRing(row, col, qlvl, 0, 1);
}

void __fastcall Drop_DITT_QuestAward(int row, int col)
{
	int qlvl;
	switch (Difficulty) {
	case DL_0_HORROR:		qlvl = 10;	break;
	case DL_1_PURGATORY:	qlvl = 15;	break;
	default:				qlvl = 10 + Difficulty * 5;	break;
	}
	CreatePepinRing(row, col, qlvl, 0, 1);
}

//!---- (004771DE) --------------------------------------------------------
bool PepinGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int brainz;
	if( MaxCountOfPlayersInGame >= 1 ){ //This enables Poisoned Water Supply dialogue and quest reward in MP
		// Cellar script
		Quest& lairQuest = Quests[Q_23_INFESTED_CELLAR];
		if( player.dungeonVisited[DUN_9_CAVE_1] || player.dungeonVisited[DUN_27_THE_INFESTED_CELLAR] ){
			if( lairQuest.status == QS_1_ACCEPT ){
				if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

				lairQuest.status = QS_2_IN_PROGRESS;
				lairQuest.talkToFlag = true;
				lairQuest.status2 = 1;
				lairQuest.speechIndex = SP_300;
				StartSpeech(SP_300);
				towner.IsTownerActionSelected = true;
			}else if( lairQuest.status == QS_3_COMPLETE && lairQuest.status2 != 2 ){
				if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

				lairQuest.status2 = 2;
				StartSpeech(SP_301);
				towner.IsTownerActionSelected = true;
				SetRndSeed(towner.seed);
				DropLairQuestAward(towner.Row, towner.Col);
			}
		}

		Quest& demon_Plague_Quest = Quests[Q_30_DEMON_PLAGUE];
		int questItemInventoryIndex3;
		if ((player.dungeonVisited[DUN_8_CATACOMB_4] || player.dungeonVisited[DUN_52_MOSSY_GROTTO])
			&& !towner.IsTownerActionSelected) {
			if (demon_Plague_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				demon_Plague_Quest.status = QS_2_IN_PROGRESS;
				demon_Plague_Quest.talkToFlag = true;
				demon_Plague_Quest.status2 = 1;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				demon_Plague_Quest.speechIndex = SP_423; 
				StartSpeech(SP_423); 
				towner.IsTownerActionSelected = true; 
			}
			else if (demon_Plague_Quest.status == QS_3_COMPLETE 
				&& FindItemInInventory(playerIndex, BI_2459_REAGENTS, questItemInventoryIndex3) 
				&& demon_Plague_Quest.status2 != 2) { 
				if (!needAction) { return true; } 
				towner.isThisTownerSpeakingNow = 150; 
				demon_Plague_Quest.status2 = 2; 
				ClearInventorySlot(playerIndex, questItemInventoryIndex3); 
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex; 
				StartSpeech(SP_425); 
				int uniqIndex = Difficulty + 2816; 
				SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.IsTownerActionSelected = true;
			}
		}

		// DITT script
		Quest& demon_in_the_Town_Quest = Quests[Q_27_DEMON_IN_THE_TOWN];
		if (player.dungeonVisited[DUN_4_CHURCH_4] || player.dungeonVisited[DUN_49_SICK_VILLAGERS_BASEMENT]) {
			if (demon_in_the_Town_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

				demon_in_the_Town_Quest.status = QS_2_IN_PROGRESS;
				demon_in_the_Town_Quest.talkToFlag = true;
				demon_in_the_Town_Quest.status2 = 1;
				demon_in_the_Town_Quest.speechIndex = SP_415;
				StartSpeech(SP_415);
				towner.IsTownerActionSelected = true;
			}
			else if (demon_in_the_Town_Quest.status == QS_3_COMPLETE && demon_in_the_Town_Quest.status2 != 2) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;

				demon_in_the_Town_Quest.status2 = 2;
				StartSpeech(SP_417);
				towner.IsTownerActionSelected = true;
				SetRndSeed(towner.seed);
				Drop_DITT_QuestAward(towner.Row, towner.Col);
			}
		}

		// Clear Water script
		Quest& clW_Quest = Quests[Q_36_CLEAR_WATER];
		if (player.dungeonVisited[DUN_10_CAVE_2] || player.dungeonVisited[DUN_11_CAVE_3] || player.dungeonVisited[DUN_112_STILL_SPRINGS]) {
			int filledContainer = 0;
			if (clW_Quest.status == QS_1_ACCEPT) {
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				clW_Quest.status = QS_2_IN_PROGRESS;
				clW_Quest.talkToFlag = true;
				clW_Quest.speechIndex = SP_440;
				StartSpeech(SP_440);// same
				towner.IsTownerActionSelected = true;
				clW_Quest.status2 = 1; // пустая бутылка выдана
				SetRndSeed(towner.seed);
				SpawnSomething(BI_2511_MT_CONTAINERS, towner.Row, towner.Col, 1);
			}else if (clW_Quest.status == QS_3_COMPLETE && clW_Quest.status2 == 1 && FindItemInInventory(playerIndex, BI_2512_FULL_CONTAINERS, filledContainer)) {// quest complete and reward not given out yet
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_442);
				towner.IsTownerActionSelected = true;
				SetRndSeed(towner.seed);
				if (FindItemInInventory(playerIndex, BI_2512_FULL_CONTAINERS, filledContainer)) {
					ClearInventorySlot(playerIndex, filledContainer);
					SpawnSomething(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), towner.Row, towner.Col);
				}				
			}else if( clW_Quest.status > QS_1_ACCEPT && clW_Quest.status2 == 0 ){ // пустая бутылка не выдана
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_440);
				towner.IsTownerActionSelected = true;
				clW_Quest.status2 = 1; // выдаем пустую бутылку если еще нет
				SetRndSeed(towner.seed);
				SpawnSomething(BI_2511_MT_CONTAINERS, towner.Row, towner.Col, 1);
			}
		}
		//--------------------------
		Quest& poisonWaterQuest = Quests[Q_13_POISONED_WATER_SUPPLY];
		// fix for: P0 (right) opened Dlvl 2 TP for P1 (left) before both players went inside PWS to kill monsters, so P1 never went to Dlvl 1 to satisfy the npc dialogue condition
		if( (player.dungeonVisited[DUN_1_CHURCH_1] || player.dungeonVisited[DUN_5_CATACOMB_1] || player.dungeonVisited[DUN_28_POISONED_WATER_SUPPLY])
			&& !towner.IsTownerActionSelected ){
			if( poisonWaterQuest.status == QS_1_ACCEPT ){
				if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				poisonWaterQuest.status = QS_2_IN_PROGRESS;
				poisonWaterQuest.talkToFlag = true;
				poisonWaterQuest.status2 = 1;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				poisonWaterQuest.speechIndex = SP_39;
				StartSpeech(SP_39);
				towner.IsTownerActionSelected = true;
			}
			else if (poisonWaterQuest.status == QS_3_COMPLETE
				&& poisonWaterQuest.status2 != 2) {// 0047729C
				if (!needAction) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				poisonWaterQuest.status2 = 2;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_41);
				int uniqIndex;
				if (GameMode == GM_CLASSIC) {
					uniqIndex = 4689 + Difficulty;
				}
				else{
					switch (Difficulty) {
					case DL_0_HORROR:		uniqIndex = 149;	break;
					case DL_1_PURGATORY:	uniqIndex = 150;	break;
					case DL_2_DOOM:			uniqIndex = 151;	break;
					case DL_3_CRUCIBLE:		uniqIndex = 2377;	break;
					case DL_4_TORMENT:		uniqIndex = 2378;	break;
					case DL_5_AGONY:		uniqIndex = 2379;	break;
					case DL_6_TERROR:		uniqIndex = 2380;	break;
					case DL_7_DESTRUCTION:	uniqIndex = 2381;	break;
					case DL_8_INFERNO:		uniqIndex = 2382;	break;
					}
				}
				SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
				towner.IsTownerActionSelected = true;
			}
		}

		Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
		if( !towner.IsTownerActionSelected
			&& blackMushroomQuest.status == QS_2_IN_PROGRESS 
			&& blackMushroomQuest.speechIndex == SP_130 
			&& FindItemInInventory(playerIndex, BI_331_BRAIN, brainz) ){
				if( !needAction ) { return true; }
				ClearInventorySlot(playerIndex, brainz);
				SpawnSomething(BI_332_SPECTRAL_ELIXIR, towner.Row, towner.Col + 1, 0, 0, 1);
				StartSpeech(SP_124);
				TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_M1;
				blackMushroomQuest.status2 = 7;
		}
	}
	
	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Pepin Greeting
		NPCGreeting(SP_169);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_14_HealerMain);
		}
	}
	
	return false;
}

//!---- (00476BC4) --------------------------------------------------------
bool WoundedGuyGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	//int questItemInventoryIndex;
	Quest& butcherQuest = Quests[Q_6_THE_BUTCHER];
	if( butcherQuest.status == QS_2_IN_PROGRESS 
		&& butcherQuest.status2 == 1 ){
		    if( !needAction ) { return false; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			butcherQuest.status2 = 1;
			int soundIndex;
			switch (player.ClassID ){
				case PC_0_WARRIOR:
				case PC_5_SAVAGE:		soundIndex = S_731_WAR_47;	break;
				case PC_4_ROGUE:
				case PC_1_ARCHER:		soundIndex = S_628_ROG_47;		break;
				case PC_2_MAGE:			soundIndex = S_525_SOR_47;		break;
				case PC_3_MONK:			soundIndex = S_845_MONK_08;			break;
			}
			if( !SoundIsPlaying(soundIndex) ){
				PlayGlobalSound(soundIndex);
			}
			towner.IsTownerActionSelected = true;
	}
	if( butcherQuest.status == QS_3_COMPLETE 
		&& butcherQuest.status2 == 1 ){
		    if( !needAction ) { return false; }
			towner.isThisTownerSpeakingNow = 150;
			butcherQuest.status2 = 1;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			towner.IsTownerActionSelected = true;
	}
	if( butcherQuest.status == QS_1_ACCEPT 
		|| butcherQuest.status == QS_2_IN_PROGRESS && !butcherQuest.status2 ){
		    if( !needAction ) { return true; }
			towner.isThisTownerSpeakingNow = 50;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			butcherQuest.status = QS_2_IN_PROGRESS;
			butcherQuest.talkToFlag = true;
			butcherQuest.speechIndex = SP_63;
			butcherQuest.status2 = 1;
			towner.field_AC = 3;
			StartSpeech(SP_63);
            NetSendCmdQuest( true, Q_6_THE_BUTCHER );
			towner.IsTownerActionSelected = true;
	}
	
	return false;
}

//!---- (00476996) --------------------------------------------------------
bool OgdenGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	if( !player.dungeonVisited[DUN_0_TOWN] && !towner.IsTownerActionSelected && !player.hasGreetedByOgden && GameMode != GM_COLISEUM ){
	    if( !needAction ) { return true; }
		towner.isThisTownerSpeakingNow = 150;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		StartSpeech(SP_258);
		towner.IsTownerActionSelected = true;
		player.hasGreetedByOgden = true;
	}
	if( player.dungeonVisited[DUN_2_CHURCH_2] || player.dungeonVisited[DUN_4_CHURCH_4] ){
		Quest& kingLeoricQuest = Quests[Q_12_THE_CURSE_OF_KING_LEORIC];																																																				
		if( kingLeoricQuest.status ){
			if( !kingLeoricQuest.status3 
				&& !towner.IsTownerActionSelected ){
				    if( !needAction ) { return true; }
					kingLeoricQuest.status3 = 1;
					kingLeoricQuest.talkToFlag = true;
					if( kingLeoricQuest.status == QS_1_ACCEPT ){
						kingLeoricQuest.status = QS_2_IN_PROGRESS;
						kingLeoricQuest.status2 = true;
					}
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					StartSpeech(SP_1);
					towner.IsTownerActionSelected = true;
                NetSendCmdQuest( true, Q_12_THE_CURSE_OF_KING_LEORIC );
			}
			if( kingLeoricQuest.status == QS_3_COMPLETE 
				&& kingLeoricQuest.status3 == 1 
				&& !towner.IsTownerActionSelected ){
				    if( !needAction ) { return true; }
					towner.isThisTownerSpeakingNow = 150;
					kingLeoricQuest.status2 = 2;
					kingLeoricQuest.status3 = 2;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					StartSpeech(SP_3);
					towner.IsTownerActionSelected = true;
                NetSendCmdQuest( true, Q_12_THE_CURSE_OF_KING_LEORIC );
			}
		}
	}
	//if( MaxCountOfPlayersInGame == 1 ){ // TODO: may be add check for ogden sign mp (QF_MULTI)
		Quest& ogdens_Sign_Quest = Quests[Q_7_OGDENS_SIGN];
		if( player.dungeonVisited[DUN_3_CHURCH_3] 
			&& ogdens_Sign_Quest.status 
			&& (ogdens_Sign_Quest.status == QS_1_ACCEPT || ogdens_Sign_Quest.status == QS_2_IN_PROGRESS) 
			&& !ogdens_Sign_Quest.status3
			&& !towner.IsTownerActionSelected ){
			    if( !needAction ) { return true; }
				ogdens_Sign_Quest.status3 = 1;
				if( ogdens_Sign_Quest.status == QS_1_ACCEPT ){
					ogdens_Sign_Quest.status2 = 1;
					ogdens_Sign_Quest.status = QS_2_IN_PROGRESS;
				}
				towner.isThisTownerSpeakingNow = 150;
				ogdens_Sign_Quest.talkToFlag = true;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_12);
				towner.IsTownerActionSelected = true;
		}
		int questItemInventoryIndex;
		if (!towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_327_TAVERN_SIGN, questItemInventoryIndex)) {
			if (!needAction) { return true; }
			ogdens_Sign_Quest.status = QS_3_COMPLETE;
			ogdens_Sign_Quest.status2 = 3;
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			int uniqIndex;
			if (GameMode == GM_CLASSIC) {
				uniqIndex = Difficulty + 4719;
			}
			else {
				switch (Difficulty) {
				case DL_0_HORROR:		uniqIndex = 160;	break;
				case DL_1_PURGATORY:	uniqIndex = 161;	break;
				case DL_2_DOOM:			uniqIndex = 162;	break;
				case DL_3_CRUCIBLE:		uniqIndex = 2365;	break;
				case DL_4_TORMENT:		uniqIndex = 2366;	break;
				case DL_5_AGONY:		uniqIndex = 2367;	break;
				case DL_6_TERROR:		uniqIndex = 2368;	break;
				case DL_7_DESTRUCTION:	uniqIndex = 2369;	break;
				case DL_8_INFERNO:		uniqIndex = 2370;	break;
				}
			}
			SpawnUnique(uniqIndex, towner.Row, towner.Col);
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_13);
			towner.IsTownerActionSelected = true;
		}

		// ogdens sign MP version
		Quest& ogdens_Sign_MP_Quest = Quests[Q_38_OGDENS_SIGN_MP];
		if (player.dungeonVisited[DUN_3_CHURCH_3]
			&& ogdens_Sign_MP_Quest.status
			&& (ogdens_Sign_MP_Quest.status == QS_1_ACCEPT || ogdens_Sign_MP_Quest.status == QS_2_IN_PROGRESS)
			&& !ogdens_Sign_MP_Quest.status3
			&& !towner.IsTownerActionSelected) {
			if (!needAction) { return true; }
			ogdens_Sign_MP_Quest.status3 = 1;
			if (ogdens_Sign_MP_Quest.status == QS_1_ACCEPT) {
				ogdens_Sign_MP_Quest.status2 = 1;
				ogdens_Sign_MP_Quest.status = QS_2_IN_PROGRESS;
			}
			towner.isThisTownerSpeakingNow = 150;
			ogdens_Sign_MP_Quest.talkToFlag = true;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_12);
			towner.IsTownerActionSelected = true;
		}
		int questItemInventoryIndexMP;
		if (!towner.IsTownerActionSelected
			&& FindItemInInventory(playerIndex, BI_327_TAVERN_SIGN, questItemInventoryIndexMP)) {
			if (!needAction) { return true; }
			ogdens_Sign_MP_Quest.status = QS_3_COMPLETE;
			ogdens_Sign_MP_Quest.status2 = 3;
			ClearInventorySlot(playerIndex, questItemInventoryIndexMP);
			int uniqIndex;
			switch (Difficulty) {
			case DL_0_HORROR:		uniqIndex = 160;	break;
			case DL_1_PURGATORY:	uniqIndex = 161;	break;
			case DL_2_DOOM:			uniqIndex = 162;	break;
			case DL_3_CRUCIBLE:		uniqIndex = 2365;	break;
			case DL_4_TORMENT:		uniqIndex = 2366;	break;
			case DL_5_AGONY:		uniqIndex = 2367;	break;
			case DL_6_TERROR:		uniqIndex = 2368;	break;
			case DL_7_DESTRUCTION:	uniqIndex = 2369;	break;
			case DL_8_INFERNO:		uniqIndex = 2370;	break;
			}
			SpawnUnique(uniqIndex, towner.Row, towner.Col);
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_13);
			towner.IsTownerActionSelected = true;
		}

		// rampaging demon quest
        Quest& theRampagingDemonQuest = Quests[Q_24_THE_RAMPAGING_DEMON];
        if( player.dungeonVisited[DUN_6_CATACOMB_2]
            && theRampagingDemonQuest.status == QS_1_ACCEPT
            && !towner.IsTownerActionSelected ){
				if( !needAction ) { return true; }
                theRampagingDemonQuest.status = QS_2_IN_PROGRESS;
                theRampagingDemonQuest.status2 = 0;
                towner.isThisTownerSpeakingNow = 150;
                theRampagingDemonQuest.talkToFlag = true;
                towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
                StartSpeech(SP_402_OGDEN_THE_RAMPAGING_DEMON_START);
                towner.IsTownerActionSelected = true;
        }
        
        if( !towner.IsTownerActionSelected
            && theRampagingDemonQuest.status == QS_2_IN_PROGRESS
			&& FindItemInInventory(playerIndex, BI_1204_THE_PROOF/*Severed Head*/, questItemInventoryIndex) ){
			if( !needAction ) { return true; }
			theRampagingDemonQuest.status = QS_3_COMPLETE;
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			int goldAmount = 4'000 + 5'000 * Difficulty;
			SpawnSomething(BI_0_GOLD, towner.Row, towner.Col, 0, 0, 0, 0, goldAmount);
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_404_OGDEN_THE_RAMPAGING_DEMON_FINISHED);
			towner.IsTownerActionSelected = true;
		}

		// locked chest scripts
		Quest& lockedChestQuest = Quests[Q_25_THE_LOCKED_CHEST];
		if (player.dungeonVisited[DUN_17_ABYSS_1] 
			&& lockedChestQuest.status == QS_1_ACCEPT
			&& !towner.IsTownerActionSelected ){ 
				if (!needAction) { return true; } 
				lockedChestQuest.status = QS_2_IN_PROGRESS;
				lockedChestQuest.status2 = 0;
				towner.isThisTownerSpeakingNow = 150; 
				lockedChestQuest.talkToFlag = true;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex; 
				StartSpeech(SP_408 ); 
				towner.IsTownerActionSelected = true; 
		} 
		if (!towner.IsTownerActionSelected
			&& lockedChestQuest.status == QS_2_IN_PROGRESS
			&& FindItemInInventory(playerIndex, BI_2456_CHEST_KEY, questItemInventoryIndex)) {
			if (!needAction) { return true; }
			lockedChestQuest.status = QS_3_COMPLETE;
			//ClearInventorySlot(playerIndex, questItemInventoryIndex);// dont take away the key
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_410);
			towner.IsTownerActionSelected = true;
		}

		// Special Weapon script
		Quest& specialWeaponQuest = Quests[Q_26_SPECIAL_WEAPON];
		if ((player.dungeonVisited[DUN_19_ABYSS_3] || player.dungeonVisited[DUN_20_ABYSS_4])
			&& specialWeaponQuest.status == QS_1_ACCEPT
			&& !towner.IsTownerActionSelected) {
			if (!needAction) { return true; }
			specialWeaponQuest.status = QS_2_IN_PROGRESS;
			specialWeaponQuest.status2 = 0;
			towner.isThisTownerSpeakingNow = 150;
			specialWeaponQuest.talkToFlag = true;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_411);
			towner.IsTownerActionSelected = true;
		}
		if (!towner.IsTownerActionSelected
			//&& specialWeaponQuest.status == QS_2_IN_PROGRESS
			&& specialWeaponQuest.status == QS_3_COMPLETE
			&& specialWeaponQuest.status != QS_4_TOOWEAK1
			//&& FindItemInInventory(playerIndex, BI_2457_IRON_FIGURINE/*would-be weapon*/, questItemInventoryIndex)
			) {
			if (!needAction) { return true; }
			specialWeaponQuest.status = QS_4_TOOWEAK1;// quest is really finished, Ogden has given his last speech
			//ClearInventorySlot(playerIndex, questItemInventoryIndex);
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_413);
			towner.IsTownerActionSelected = true;
		}

		// price of hubris scripts
		Quest& hubrisQuest = Quests[Q_37_PRICE_OF_HUBRIS];
		if ((player.dungeonVisited[DUN_12_CAVE_4] || player.dungeonVisited[DUN_13_HELL_1])
			&& hubrisQuest.status == QS_1_ACCEPT
			&& !towner.IsTownerActionSelected) {
			if (!needAction) { return true; }
			hubrisQuest.status = QS_2_IN_PROGRESS;
			hubrisQuest.status2 = 0;
			towner.isThisTownerSpeakingNow = 150;
			hubrisQuest.talkToFlag = true;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_443);
			towner.IsTownerActionSelected = true;
		}
		if (!towner.IsTownerActionSelected
			&& hubrisQuest.status == QS_3_COMPLETE && hubrisQuest.status != QS_4_TOOWEAK1) {
			if (!needAction) { return true; }
			hubrisQuest.status = QS_4_TOOWEAK1;
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_445);
			towner.IsTownerActionSelected = true;
		}
	//}
	
	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Ogden greeting
		NPCGreeting(SP_160);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_21_OgdenMain);
		}
	}
	
	return false;
}

//!---- (0047737B) --------------------------------------------------------
bool CainGreeting(int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int questItemInventoryIndex;
	Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
	Quest& diabloQuest = Quests[Q_5_DIABLO];
	if( MaxCountOfPlayersInGame == 1 ){
		if( lazarusQuest.status == QS_1_ACCEPT 
			&& FindItemInInventory(playerIndex, BI_33_STAFF_OF_LAZARUS, questItemInventoryIndex) ){
			    if( !needAction ) { return true; }
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				towner.isThisTownerSpeakingNow = 150;
				lazarusQuest.status2 = LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_23);
				towner.IsTownerActionSelected = true;
				lazarusQuest.status = QS_2_IN_PROGRESS;
				lazarusQuest.talkToFlag = true;
		}else if( lazarusQuest.status == QS_3_COMPLETE 
			&& lazarusQuest.status2 == LQS_7_LAZARUS_DEAD ){
			    if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				lazarusQuest.status2 = LQS_8;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_25_KAIN_ABOUT_DIABLO);
				towner.IsTownerActionSelected = true;
				diabloQuest.talkToFlag = true;
		}
	}else if( MaxCountOfPlayersInGame != 1 ){// лишнее дублирование, но так виднее что мульти онли
		if( lazarusQuest.status == QS_2_IN_PROGRESS && !lazarusQuest.talkToFlag ){
		    if( !needAction ) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_23);//Was missing this dialogue in MP, Cain had ! overhead but no dialogue when clicked on.
			towner.IsTownerActionSelected = true;
			lazarusQuest.talkToFlag = true;
            NetSendCmdQuest( true, Q_15_ARCHBISHOP_LAZARUS );
		}else if( lazarusQuest.status == QS_3_COMPLETE && lazarusQuest.status2 == LQS_7_LAZARUS_DEAD ){
			if( !needAction ) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			lazarusQuest.status2 = LQS_8;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_25_KAIN_ABOUT_DIABLO);//Was missing this dialogue in MP, Cain had ! overhead but no dialogue when clicked on.
			towner.IsTownerActionSelected = true;
			NetSendCmdQuest( true, Q_15_ARCHBISHOP_LAZARUS );
			diabloQuest.talkToFlag = true;
			NetSendCmdQuest( true, Q_5_DIABLO );
		}
	}
	
	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Elder Kain Greeting
		NPCGreeting(SP_150);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_15_ElderMain);
		}
	}
	
	return false;
}

//!---- (0047719D) --------------------------------------------------------
bool FarnhamGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	
	Quest& islandQuest = Quests[Q_20_SUNLESS_SEA];

	if( player.dungeonVisited[DUN_10_CAVE_2] && islandQuest.status == QS_1_ACCEPT ){
	    if( !needAction ) { return true; }
		islandQuest.status = QS_2_IN_PROGRESS;
		islandQuest.talkToFlag = true;
		towner.isThisTownerSpeakingNow = 150;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		islandQuest.speechIndex = SP_381;
		StartSpeech(SP_381);
		towner.IsTownerActionSelected = true;
	}

    if( !needAction ) { return false; }

	if( !Speech_IsPanelVisible ){
		NPCGreeting(SP_200);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_22_FarnhamMain);
		}
	}
	
	return false;
}

//!---- (00476F65) --------------------------------------------------------
bool AdriaGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int questItemInventoryIndex;
	Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
	if( blackMushroomQuest.status == QS_1_ACCEPT 
		&& FindItemInInventory(playerIndex, BI_19_FUNGAL_TOME, questItemInventoryIndex) ){
		    if( !needAction ) { return true; }
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			towner.isThisTownerSpeakingNow = 150;
			blackMushroomQuest.status = QS_2_IN_PROGRESS;
			blackMushroomQuest.talkToFlag = true;
			blackMushroomQuest.status2 = 2;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_128);
			towner.IsTownerActionSelected = true;
	}else if( blackMushroomQuest.status == QS_2_IN_PROGRESS ){

		if( blackMushroomQuest.status2 >= 2 && blackMushroomQuest.status2 <= 4 ){
			if( FindItemInInventory(playerIndex, BI_330_BLACK_MUSHROOM, questItemInventoryIndex) ){
			    if( !needAction ) { return true; }
				ClearInventorySlot(playerIndex, questItemInventoryIndex);
				TownersSpeechAboutQuestArray[TI_6_ADRIA][Q_1_BLACK_MUSHROOM] = SP_M1;
				towner.isThisTownerSpeakingNow = 150;
				blackMushroomQuest.status2 = 5;
				TownersSpeechAboutQuestArray[TI_1_PEPIN][Q_1_BLACK_MUSHROOM] = SP_123;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				blackMushroomQuest.speechIndex = SP_130;
				StartSpeech(SP_130);
				towner.IsTownerActionSelected = true;
			}else if( blackMushroomQuest.speechIndex != SP_129_ADRIA_ABOUT_HUGE_AXE ){
			    if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				blackMushroomQuest.speechIndex = SP_129_ADRIA_ABOUT_HUGE_AXE;
				StartSpeech(SP_129_ADRIA_ABOUT_HUGE_AXE);
				towner.IsTownerActionSelected = true;
			}
		}else{
			Item* spectralElixir = FindItemInInventory(playerIndex, BI_332_SPECTRAL_ELIXIR, questItemInventoryIndex);
			if( spectralElixir ){
			    if( !needAction ) { return true; }
				towner.isThisTownerSpeakingNow = 150;
				towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
				StartSpeech(SP_132);
				blackMushroomQuest.status = QS_3_COMPLETE;
				BaseItems[spectralElixir->baseItemIndex].UseOnceFlag = true;
			}else if( FindItemInInventory(playerIndex, BI_331_BRAIN, questItemInventoryIndex) 
				&& blackMushroomQuest.status3 != 131 ){
				    if( !needAction ) { return true; }
					towner.isThisTownerSpeakingNow = 150;
					towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
					blackMushroomQuest.status3 = 131;
					StartSpeech(SP_131);
					towner.IsTownerActionSelected = true;
			}
		}

	}
	
	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Adria Greeting
		NPCGreeting(SP_212);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_5_WitchMain);
		}
	}
	
	return false;
}

//----- (0071AC89) --------------------------------------------------------
void AddIzualQuestAward ()
{
	Player& player = Players[CurrentPlayerIndex];
	int xp;
	switch (Difficulty ){
		case DL_0_HORROR:		xp = 14'285;		break;
		case DL_1_PURGATORY:	xp = 500'000;		break;
		case DL_2_DOOM:			xp = 2'000'000;		break;
		case DL_3_CRUCIBLE:		xp = 4'000'000;		break;
		case DL_4_TORMENT:		xp = 8'000'000;		break;
		case DL_5_AGONY:		xp = 16'000'000;	break;
		case DL_6_TERROR:		xp = 32'000'000;	break;
		case DL_7_DESTRUCTION:	xp = 64'000'000;	break;
		case DL_8_INFERNO:		xp = 128'000'000;	break;
	}
	AddPlayerExperience(CurrentPlayerIndex, xp);
}

//----- (00716280) --------------------------------------------------------
void AddAndyQuestAward ()
{
	Player& player = Players[CurrentPlayerIndex];
	PlayGlobalSound(S_1022_LEVELUP);
	player.AvailableLvlPoints += Difficulty + 2;
}

//!---- (00477118) --------------------------------------------------------
bool GillianGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int isScriptActivated = 0;
	
	// Grave Matters
	if ((player.dungeonVisited[DUN_1_CHURCH_1])
		&& !isScriptActivated) {
		Quest& gm_Quest = Quests[Q_34_GRAVE_MATTERS];
		if (gm_Quest.status == QS_1_ACCEPT) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			gm_Quest.status = QS_2_IN_PROGRESS;
			gm_Quest.talkToFlag = 1;
			gm_Quest.status2 = 1;
			StartSpeech(SP_435);
			isScriptActivated = 1;
		}
		else if (gm_Quest.status == QS_3_COMPLETE && gm_Quest.status2 != 2) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			gm_Quest.status2 = 2;
			StartSpeech(SP_436);
			isScriptActivated = 1;
		}
	}
	// andariel
	if( (player.dungeonVisited[DUN_12_CAVE_4] || player.dungeonVisited[DUN_30_HALLS_OF_ANGUISH])
	 && !isScriptActivated ){ // Accepting Izual quest automatically disables Andariel quest, why???
		Quest& andyQuest = Quests[Q_22_MAIDEN_OF_ANGUISH]; // Andariel
		if( andyQuest.status == QS_1_ACCEPT )	{
		    if( !needAction ) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			andyQuest.status = QS_2_IN_PROGRESS;
			andyQuest.talkToFlag = 1;
			andyQuest.status2 = 1;
			
			// в th1 также, похоже на ошибку, отключаем на лету
			//andyQuest.status2 = 40;
			//andyQuest.status3 = 1;
			
			StartSpeech(SP_296);
			isScriptActivated = 1;
		}else if( andyQuest.status == QS_3_COMPLETE && andyQuest.status2 != 2 ){
		    if( !needAction ) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			AddAndyQuestAward();
			andyQuest.status2 = 2;
			StartSpeech(SP_316);
			isScriptActivated = 1;
		}
	}
	// izual
	if (player.dungeonVisited[DUN_17_ABYSS_1] || player.dungeonVisited[DUN_35_PASSAGE_OF_FIRE]) {//If a player skips Dlvl 17 and arrives in Dlvl 19 via another player's TP, he should be able to get Izual quest reward.
		Quest& izualQuest = Quests[Q_17_FALLEN_ANGEL];
		if (izualQuest.status == QS_1_ACCEPT) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			izualQuest.status = QS_2_IN_PROGRESS;
			izualQuest.talkToFlag = 1;
			StartSpeech(SP_364);
			isScriptActivated = 1;
		}
		else if (izualQuest.status == QS_3_COMPLETE && izualQuest.status2 != 3) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			izualQuest.status2 = 3;
			StartSpeech(SP_366);
			AddIzualQuestAward();
			isScriptActivated = 1;
		}
	}
	// visions of victory
	if ((player.dungeonVisited[DUN_21_CRYPT_1] || player.dungeonVisited[DUN_50_TEMPLE_OF_SUNSET])
		&& !isScriptActivated) {
		Quest& vov_Quest = Quests[Q_28_VISIONS_OF_VICTORY];
		if (vov_Quest.status == QS_1_ACCEPT) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			vov_Quest.status = QS_2_IN_PROGRESS;
			vov_Quest.talkToFlag = 1;
			vov_Quest.status2 = 1;

			StartSpeech(SP_418);
			isScriptActivated = 1;
		}
		else if (vov_Quest.status == QS_3_COMPLETE && vov_Quest.status2 != 2) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			towner.indexOfPlayerWhichSpeaksTownsman = CurrentPlayerIndex;
			vov_Quest.status2 = 2;
			StartSpeech(SP_419);
			isScriptActivated = 1;
		}
	}

	if( !needAction ) { return false; }
	
	if( !Speech_IsPanelVisible ){// Gillian Greeting
		NPCGreeting(SP_179);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_23_GilianMain);
		}
	}
	
	return false;
}

//!---- (0047733E) --------------------------------------------------------
bool WirtGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	Quest& lostTreasureQuest = Quests[Q_19_LOST_TREASURE];
	if( lostTreasureQuest.status == QS_1_ACCEPT && player.dungeonVisited[DUN_7_CATACOMB_3] ){
	    if( !needAction ) { return true; }
		towner.isThisTownerSpeakingNow = 150;
		lostTreasureQuest.status = QS_2_IN_PROGRESS;
		lostTreasureQuest.status2 = 1;// только поговорили
		lostTreasureQuest.talkToFlag = 1;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		lostTreasureQuest.speechIndex = SP_374;
		StartSpeech(SP_374);
		SpawnSomething(BI_22_CRYPT_MAP, 69, 49, 1);
	}

	// ---- START OF STRANGE BOOK SCRIPT ------------
	Quest& strange_book_Quest = Quests[Q_33_STRANGE_BOOK];
	if ((player.dungeonVisited[DUN_16_HELL_4] || player.dungeonVisited[DUN_17_ABYSS_1])
		&& strange_book_Quest.status == QS_1_ACCEPT
		&& !towner.IsTownerActionSelected) {
		if (!needAction) { return true; }
		strange_book_Quest.status = QS_2_IN_PROGRESS;
		strange_book_Quest.status2 = 0;
		towner.isThisTownerSpeakingNow = 150;
		strange_book_Quest.talkToFlag = true;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		StartSpeech(SP_432);
		towner.IsTownerActionSelected = true;
	}
	int questItemInventoryIndex7;
	if (!towner.IsTownerActionSelected
		&& strange_book_Quest.status == QS_2_IN_PROGRESS
		&& strange_book_Quest.status != QS_4_TOOWEAK1
		&& FindItemInInventory(playerIndex, BI_2474_TOOL_CHEST, questItemInventoryIndex7)) { 
		if (!needAction) { return true; }
		strange_book_Quest.status = QS_3_COMPLETE;
		ClearInventorySlot(playerIndex, questItemInventoryIndex7); // take away the tool chest
		SetPlayerHandItem(player.ItemOnCursor, BI_2476_THE_STRANGE_BOOK);
		Item_MakeNonCollisionSeed(CurrentPlayerIndex, player.ItemOnCursor);
		if( DropCursorItemBeforeTrig() ) FixItemCursor(-1);
		towner.isThisTownerSpeakingNow = 150;
		towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
		StartSpeech(SP_434);
		towner.IsTownerActionSelected = true;
	}
	// ---- END OF STRANGE BOOK SCRIPT ------------

	Quest& ast_Quest = Quests[Q_32_A_SIMPLE_TASK];
	int questItemInventoryIndex5;
	if ((player.dungeonVisited[DUN_20_ABYSS_4] || player.dungeonVisited[DUN_21_CRYPT_1] || player.dungeonVisited[DUN_54_ABANDONED_FANE])
		&& !towner.IsTownerActionSelected) {
		if (ast_Quest.status == QS_1_ACCEPT) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			ast_Quest.status = QS_2_IN_PROGRESS;
			ast_Quest.talkToFlag = true;
			ast_Quest.status2 = 1;
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			ast_Quest.speechIndex = SP_429;
			StartSpeech(SP_429);
			towner.IsTownerActionSelected = true;
		}
		else if (ast_Quest.status == QS_3_COMPLETE
			&& FindItemInInventory(playerIndex, BI_2461_MUNDANE_INVENTORY, questItemInventoryIndex5)
			&& ast_Quest.status2 != 2) {
			if (!needAction) { return true; }
			towner.isThisTownerSpeakingNow = 150;
			ast_Quest.status2 = 2;
			ClearInventorySlot(playerIndex, questItemInventoryIndex5);
			towner.indexOfPlayerWhichSpeaksTownsman = playerIndex;
			StartSpeech(SP_431);
			int uniqIndex = mundaneInventoryRewards[Difficulty];
			SpawnUnique(uniqIndex, towner.Row, towner.Col + 1);
			towner.IsTownerActionSelected = true;
		}
	}

    if( !needAction ) { return false; }

	if( !Speech_IsPanelVisible ){
		NPCGreeting(SP_224);
		if( NPCMenusEnabled ){
			ChangeTownerDialog(PD_12_WirtMain);
		}
	}
	
	return false;
}

//!---- (004774DD) --------------------------------------------------------
void CowGreeting (int playerIndex, int townerIndex)
{
	if( !Speech_IsPanelVisible ){
		CowClick(playerIndex);// Коровы
	}
}

//!---- (004774FF) --------------------------------------------------------
void LesterGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if( Speech_IsPanelVisible ){
		return;
	}

	int questItemInventoryIndex;
	int speechIndex;
	Quest& farmerOrchardQuest = Quests[Q_17_FALLEN_ANGEL];
	if( farmerOrchardQuest.status == QS_0_NO 
		|| farmerOrchardQuest.status == QS_1_ACCEPT ){
			if( farmerOrchardQuest.status == QS_0_NO ){
				if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
					farmerOrchardQuest.status = QS_2_IN_PROGRESS;
					farmerOrchardQuest.status2 = 1;
					farmerOrchardQuest.talkToFlag = true;
					farmerOrchardQuest.speechIndex = SP_277;
					StartSpeech(SP_278);
					if( MaxCountOfPlayersInGame != 1 ){
                        NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
					}
					return;
				}
				if( !player.dungeonVisited[DUN_9_CAVE_1] && player.CharLevel < 15 ){
					if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
						StartSpeech(SP_278);
					}else{
						StartSpeech(SP_279);
					}
					if( MaxCountOfPlayersInGame != 1 ){
                        NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
					}
					return;
				}

				// иначе дефолтная речь
			}else if( farmerOrchardQuest.status == QS_1_ACCEPT ){
				if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
					farmerOrchardQuest.status = QS_2_IN_PROGRESS;
					farmerOrchardQuest.status2 = 1;
					farmerOrchardQuest.speechIndex = SP_277;
					farmerOrchardQuest.talkToFlag = true;
					StartSpeech(SP_278);
					if( MaxCountOfPlayersInGame != 1 ){
                        NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
					}
					return;
				}

				if( !player.dungeonVisited[DUN_9_CAVE_1] && player.CharLevel < 23 ){
					speechIndex = SP_309;
					if( player.dungeonVisited[DUN_2_CHURCH_2] ){
						speechIndex = SP_281;
					}
					if( player.dungeonVisited[DUN_5_CATACOMB_1] ){
						speechIndex = SP_308;
					}
					if( player.dungeonVisited[DUN_7_CATACOMB_3] ){
						speechIndex = SP_310;
					}
					StartSpeech(speechIndex);
					if( MaxCountOfPlayersInGame != 1 ){
                        NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
					}
					return;
				}

				// иначе дефолтная речь
			}

			// дефолтная речь
			farmerOrchardQuest.status = QS_2_IN_PROGRESS;
			farmerOrchardQuest.status2 = 1;
			farmerOrchardQuest.talkToFlag = true;
			farmerOrchardQuest.speechIndex = SP_277;
			StartSpeech(SP_277);
			if( MaxCountOfPlayersInGame != 1 ){
                NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
			}
	}else if( farmerOrchardQuest.status == QS_2_IN_PROGRESS ){
		if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
			StartSpeech(SP_278);
		}else{ 
			StartSpeech(SP_279);
		}
		if( MaxCountOfPlayersInGame != 1 ){
            NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
		}
		return;
	}
	if( farmerOrchardQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_280;
		SpawnSomething(BI_329_AMBER, towner.Row + 1, towner.Col, 1);
		farmerOrchardQuest.talkToFlag = false;
		farmerOrchardQuest.status = QS_10_REALLYDONE;
	}else if( farmerOrchardQuest.status == QS_10_REALLYDONE ){
		speechIndex = SP_M1;
	}else{
		farmerOrchardQuest.status = QS_0_NO;
		speechIndex = SP_280;
	}
	if( speechIndex != -1 ){
		StartSpeech(speechIndex);
	}
	if( MaxCountOfPlayersInGame != 1 ){
        NetSendCmdQuest( true, Q_17_FALLEN_ANGEL );
	}
}

// знаком //!---- помечены адреса инлайн кусков функций в оригинале
//!---- (004778C3) --------------------------------------------------------
bool CeliaGreeting (int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	int questItemInventoryIndex;
	Quest& littleGirlQuest = Quests[Q_18_LITTLE_GIRL];
	int speechIndex;
	bool isHasSpeech = true;
	if( FindItemInInventory(playerIndex, BI_334_THEODORE, questItemInventoryIndex) 
		&& littleGirlQuest.status != QS_3_COMPLETE ){
		    if( !needAction ) { return true; }
			speechIndex = SP_285;
			ClearInventorySlot(playerIndex, questItemInventoryIndex);
			// DromCeliaAmulet(towner.Row, towner.Col, 13, 0, 1);
			if (HasTrait(playerIndex, TraitId::Mamluk)) {
				SpawnUnique(Difficulty + 2852, towner.Row, towner.Col + 1);
			}
			else {
				SpawnUnique(CeliaUniqsTable[player.ClassID][Difficulty], towner.Row, towner.Col + 1);
			}
			littleGirlQuest.talkToFlag = false;
			littleGirlQuest.status = QS_3_COMPLETE;
	}else if( littleGirlQuest.status == QS_0_NO
		|| littleGirlQuest.status == QS_1_ACCEPT ){
		    if( !needAction ) { return true; }
			speechIndex = SP_283;
			littleGirlQuest.status = QS_2_IN_PROGRESS;
			littleGirlQuest.status2 = 1;
			littleGirlQuest.talkToFlag = true;
			littleGirlQuest.speechIndex = SP_283;
	}else if( littleGirlQuest.status == QS_2_IN_PROGRESS ){
	    if( !needAction ) { return true; }
		speechIndex = SP_284;
	}else if( littleGirlQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_M1;
	}else{
	    if( !needAction ) { return true; }
		littleGirlQuest.status = QS_0_NO;
		speechIndex = SP_282;
		isHasSpeech = false;
	}

    if( !needAction ) { return false; }

	if( speechIndex != -1 ){
		if( isHasSpeech ){
			StartSpeech(speechIndex);
		}else{
			PlayGlobalSound(Speeches[speechIndex].SoundIndex);
		}
	}
	if( MaxCountOfPlayersInGame != 1 ){
        NetSendCmdQuest( 1, Q_18_LITTLE_GIRL );
	}
	
	return false;
}

//!---- (004776E2) --------------------------------------------------------
void NutGreeting (int playerIndex, int townerIndex)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if( Speech_IsPanelVisible ){
		return;
	}

	int questItemInventoryIndex;
	int speechIndex;
	Quest& nutQuest = Quests[Q_23_INFESTED_CELLAR];

	if( FindItemInInventory(playerIndex, BI_47_BLUDGEON, questItemInventoryIndex) ){
		speechIndex = SP_303;
		ClearInventorySlot(playerIndex, questItemInventoryIndex);
	}else if( FindItemInInventory(playerIndex, BI_46_BONE_MACE, questItemInventoryIndex) ){
		SpawnUnique(9, towner.Row + 1, towner.Col); // uniq id = 9 was UID_ARMRCOW, now "Exodus"
		speechIndex = SP_304;
		nutQuest.status = QS_3_COMPLETE;
	}else if( FindItemInInventory(playerIndex, BI_39_RUNE_BOMB_OPENS_ABYSS, questItemInventoryIndex) ){
		speechIndex = SP_301;
		nutQuest.status = QS_2_IN_PROGRESS;
		nutQuest.status2 = 1;
		nutQuest.speechIndex = SP_300;
		nutQuest.talkToFlag = true;
	}else if( nutQuest.status == QS_0_NO || nutQuest.status == QS_1_ACCEPT ){
		speechIndex = SP_297;
		nutQuest.status = QS_7_PASS1;
	}else if( nutQuest.status == QS_2_IN_PROGRESS ){
		speechIndex = SP_301;
	}else if( nutQuest.status == QS_3_COMPLETE ){
		speechIndex = SP_297;
	}else if( nutQuest.status == QS_7_PASS1 ){
		speechIndex = SP_298;
		nutQuest.status = QS_8_PASS2;
	}else if( nutQuest.status == QS_8_PASS2 ){
		speechIndex = SP_299;
		nutQuest.status = QS_9_PASS3;
	}else if( nutQuest.status == QS_9_PASS3 ){
		if( player.dungeonVisited[DUN_9_CAVE_1] || player.CharLevel >= 15 ){
			speechIndex = SP_300;
			nutQuest.status = QS_2_IN_PROGRESS;
			nutQuest.status2 = 1;
			nutQuest.speechIndex = SP_300;
			nutQuest.talkToFlag = true;
			//PutRuneBombOnMap(cityzen.Row + 1, cityzen.Col);
		}else{
			switch (RNG(4) ){
			case 0:		speechIndex = SP_305;		break;
			case 1:		speechIndex = SP_312;		break;
			case 2:		speechIndex = SP_313;		break;
			case 3:		speechIndex = SP_314;		break;
			}
		}
	}else{// -1 ?
		nutQuest.status = QS_0_NO;
		speechIndex = SP_301;
	}
	StartSpeech(speechIndex);
	if( MaxCountOfPlayersInGame != 1 ){
        NetSendCmdQuest( 1, Q_23_INFESTED_CELLAR );
	}
}

//----- (0071F400) --------------------------------------------------------
bool __fastcall TremainGreeting(int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	int questItemInventoryIndex;
	int speechIndex = -1;
//	if( FindItemInInventory(playerIndex, BI_399_SHADOWFANG, questItemInventoryIndex) ){
	if (Quests[Q_21_THE_CURSED_BLADE].status4 == 1 && FindItemInInventory(playerIndex, BI_399_SHADOWFANG, questItemInventoryIndex)) {//Tremain should only take Shadow Fang after Wielder is dead.
	    if( !needAction ) { return true; }
		if( Quests[Q_21_THE_CURSED_BLADE].status2 >= 7 ){
			return false;
		}
		ClearInventorySlot(playerIndex, questItemInventoryIndex);
		//Quests[ Q_21_FLESHDOOM ].speechIndex = speechIndex;
		#ifdef FLESHDOOM_ENABLED
		SpawnUnique(0, towner.Row + 1, towner.Col);
		#endif
		Quests[Q_21_THE_CURSED_BLADE].status2 = 7;// Tremain dead
		Quests[Q_21_THE_CURSED_BLADE].status3 = 1;
//		HorazonGrimspikeDelay = 250;
		FleshDoomDelay = 250;//250 for current player, but 0 for all other players due to quest status sync at the end of this function.  Not a big issue.
		speechIndex = SP_393;
		Quests[Q_21_THE_CURSED_BLADE].speechIndex = speechIndex;
	}else if( Quests[Q_21_THE_CURSED_BLADE].status2 == 0 ){//
	    if( !needAction ) { return true; }
		Quests[Q_21_THE_CURSED_BLADE].status = QS_2_IN_PROGRESS;
		Quests[Q_21_THE_CURSED_BLADE].status2 = 1;// after first speech
		Quests[Q_21_THE_CURSED_BLADE].talkToFlag = 1;
		speechIndex = SP_389;
		Quests[Q_21_THE_CURSED_BLADE].speechIndex = speechIndex;
	}else if( Quests[Q_21_THE_CURSED_BLADE].status2 == 1 ){
	    if( !needAction ) { return true; }
		speechIndex = SP_390;
	}else if( Quests[Q_21_THE_CURSED_BLADE].status2 == 2 ){
	    if( !needAction ) { return true; }
		Quests[Q_21_THE_CURSED_BLADE].status2 = 3;// after second speech
		Quests[Q_21_THE_CURSED_BLADE].talkToFlag = 1;
		speechIndex = SP_391;
		Quests[Q_21_THE_CURSED_BLADE].speechIndex = speechIndex;
	}else if( Quests[Q_21_THE_CURSED_BLADE].status2 == 3 ){
	    if( !needAction ) { return true; }
		speechIndex = SP_392;
	}
	
	if( !needAction ) { return false; }
	
	if( speechIndex != -1 ){
		StartSpeech(speechIndex);
	}
	if( MaxCountOfPlayersInGame != 1 ){
        NetSendCmdQuest( 1, Q_21_THE_CURSED_BLADE );//This quest sync call runs as InquisitionScript() updates quest status.
	}
	
	return false;
}

bool ColiseumGreeting(int playerIndex, int townerIndex, bool needAction = true)
{
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];

	if (!needAction) { return false; }

	if (!Speech_IsPanelVisible) {
		NPCGreeting(SP_405 + (GetRndSeed() % 3));
		if (NPCMenusEnabled) {
			ChangeTownerDialog(PD_28_ColiseumMain);
		}
	}

	return false;
}

//----- (004768DE) --------------------------------------------------------
void __fastcall TalkToTowner(int playerIndex, int townerIndex)
{      
	RNG(3); // TODO: check wtf is this
	RNG(4);
	RNG(5);
	Player& player = Players[playerIndex];
	Towner& towner = Towners[townerIndex];
	int deltaRow = abs(player.Row - towner.Row);
	int deltaCol = abs(player.Col - towner.Col);
	if( deltaRow >= 2 || deltaCol >= 2 || Speech_IsPanelVisible ){
		return;
	}
	if( Speech_IsPanelVisible ){
		return;
	}
	towner.IsTownerActionSelected = false;
	if( Cur.GraphicsID >= CM_12_ITEMS_PICS_START && !DropCursorItemBeforeTrig() ){
		return;
	}
	if (HasTrait(playerIndex, TraitId::Leper) && not(towner.TownerIdentificator, TI_7_GILLIAN, TI_9_COWS)) {
		return;
	}
	switch (towner.TownerIdentificator) {
	case TI_0_GRISWOLD:					GriswoldGreeting(playerIndex, townerIndex);	return;
	case TI_1_PEPIN:					PepinGreeting(playerIndex, townerIndex);		return;
	case TI_2_WOUNDED_GUY:				WoundedGuyGreeting(playerIndex, townerIndex);	return;
	case TI_3_OGDEN:					OgdenGreeting(playerIndex, townerIndex);		return;
	case TI_4_CAIN:						CainGreeting(playerIndex, townerIndex);		return;
	case TI_5_FARNHAM:					FarnhamGreeting(playerIndex, townerIndex);		return;
	case TI_6_ADRIA:					AdriaGreeting(playerIndex, townerIndex);		return;
	case TI_7_GILLIAN:					GillianGreeting(playerIndex, townerIndex);		return;
	case TI_8_WIRT:						WirtGreeting(playerIndex, townerIndex);		return;
	case TI_9_COWS:						CowGreeting(playerIndex, townerIndex);			return;
	case TI_10_LESTER:					LesterGreeting(playerIndex, townerIndex);		return;
	case TI_11_CELIA:					CeliaGreeting(playerIndex, townerIndex);		return;
	case TI_12_NUT:						NutGreeting(playerIndex, townerIndex);			return;
	case TI_13_TREMAIN:					TremainGreeting(playerIndex, townerIndex);		return;
	case TI_14_COLISEUM:				ColiseumGreeting(playerIndex, townerIndex);		return;
	}

}

bool IsTownerHasActiveSpeech( int playerIndex, int townerIndex )
{
    bool result = false;
    Towner& towner = Towners[townerIndex];
    
    if( Speech_IsPanelVisible )
    {
        return false;
    }
    towner.IsTownerActionSelected = false;
    
	if (HasTrait(playerIndex, TraitId::Leper) && not(towner.TownerIdentificator, TI_7_GILLIAN, TI_9_COWS)) {
		return false;
	}

	switch (towner.TownerIdentificator) {
	case TI_0_GRISWOLD:					result = GriswoldGreeting(playerIndex, townerIndex, false);	break;
	case TI_1_PEPIN:					result = PepinGreeting(playerIndex, townerIndex, false);		break;
	case TI_2_WOUNDED_GUY:				result = WoundedGuyGreeting(playerIndex, townerIndex, false);	break;
	case TI_3_OGDEN:					result = OgdenGreeting(playerIndex, townerIndex, false);		break;
	case TI_4_CAIN:						result = CainGreeting(playerIndex, townerIndex, false);		break;
	case TI_5_FARNHAM:					result = FarnhamGreeting(playerIndex, townerIndex, false);	break;
	case TI_6_ADRIA:					result = AdriaGreeting(playerIndex, townerIndex, false);		break;
	case TI_7_GILLIAN:					result = GillianGreeting(playerIndex, townerIndex, false);		break;
	case TI_8_WIRT:						result = WirtGreeting(playerIndex, townerIndex, false);		break;
		//case TI_9_COWS:						result = CowGreeting( playerIndex, townerIndex, false );		break;
		//case TI_10_LESTER:					result = LesterGreeting( playerIndex, townerIndex, false );		break;
	case TI_11_CELIA:					result = CeliaGreeting(playerIndex, townerIndex, false);		break;
		//case TI_12_NUT:						result = NutGreeting( playerIndex, townerIndex, false );		break;
	case TI_13_TREMAIN:					result = TremainGreeting(playerIndex, townerIndex, false);	break;
	}
	return result;
}

//----- (004779D0) --------------------------------------------------------
void __fastcall CowClick(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( CowClickPlayerSound == -1 || !SoundIsPlaying(CowClickPlayerSound) ){
		if( ++CowClickCount < 8 ){
			int soundIndex;
			if( CowClickCount != 4 ){
				soundIndex = S_244_T_COW1;
			}else{
				soundIndex = S_245_T_COW2;
			}
			CowClickPlayerSound = soundIndex;
			PlayLocalSound(soundIndex, player.Row, player.Col);
			return;
		}else{
			PlayLocalSound(S_244_T_COW1, player.Row, player.Col + 5);
			CowClickCount = 4;
			int soundIndex = CowSounds[ CowClickPlayerSoundIndex ][ player.ClassID ];
			CowClickPlayerSound = soundIndex;
			if( CowClickPlayerSoundIndex++ >= 2 ){
				CowClickPlayerSoundIndex = 0;
			}
			PlayLocalSound(soundIndex, player.Row, player.Col);
		}
	}
	
}
