#include "stdafx.h"

Aura Auras[] = {
	    //AuraIndex,						 AuraName,			 AuraEffect,				 			 baseMonsterIndex,																								minVal, maxVal, radius
/*0*/	{ AUI_0_NONE, "None",						AUEF_0_NONE,					{-1},																	    0, 0, 0},
/*1*/	{ AUI_1_,	  "Drains Life",				AUEF_1_HP_BLEED,				{ BM_0_BURNED_CORPSE, BM_215_CADAVER, BM_246_GREATER_MUMMY, BM_244_HORADRIM_ANCIENT, BM_120_NECROMORPH, BM_135_GROTESQUE, BM_141_SHAMBLER, BM_142_HELL_MEAT, BM_2_ROTTING_CARCASS, BM_140_GHOUL, BM_169_ZOMBIE, BM_286_BRINE_CORPSE, -1 },			1, 3, 3, S_1517_m_aura_01},
/*2*/	{ AUI_2_,	  "Reduces Accuracy",			AUEF_4_TO_HIT_REDUCE,			{ BM_248_SALAMANDER, -1 },			1, 3, 3, S_1518_m_aura_02},// perplexity
/*3*/	{ AUI_3_,	  "Drains Mana",				AUEF_2_MP_BLEED,				{ BM_1_OBLIVION_KNIGHT, BM_50_SKELETON_LORD, BM_64_DEATH_GUARDIAN, BM_203_DEATH_KNIGHT, BM_260_ARMAGEDDON_WALKER, -1 },						1, 3, 3, S_1519_m_aura_03},
/*4*/	{ AUI_4_,	  "Reduces Armor",				AUEF_3_AC_REDUCE,				{ BM_126_PLAGUE_EATER, BM_221_RAT_MAN, BM_258_RAT_HORDE, BM_274_FLESH_DEVOURER, -1 },		1, 3, 3, S_1520_m_aura_04},
/*5*/	{ AUI_5_,     "Static Field",				AUEF_5_LIGHTNING_RES_REDUCE,	{ BM_249_SCARAB, BM_250_BONE_SCARAB, -1 },												1, 3, 2, S_1521_m_aura_05},
/*6*/	{ AUI_6_,     "Searing Field",				AUEF_6_FIRE_RES_REDUCE,			{ BM_60_HELL_STONE, BM_61_BLOOD_STONE, BM_62_MAULER, BM_63_LAVA_LORD, BM_180_CRYPT_STONE, BM_273_FLAME_FIEND, -1 },							1, 3, 3, S_1522_m_aura_06},
/*7*/	{ AUI_7_,	  "Freezing Field",				AUEF_7_COLD_RES_REDUCE,			{ BM_238_TAINTED, BM_239_AFFLICTED, -1 },											1, 3, 3, S_1523_m_aura_07},
/*8*/	{ AUI_8_,	  "Toxic Field",				AUEF_8_ACID_RES_REDUCE,			{ BM_46_PIT_SPIDER, -1 },							1, 3, 3, S_1524_m_aura_08},
/*9*/	{ AUI_9_,	  "Arcane Field",				AUEF_9_ARCAN_RES_REDUCE,		{ BM_101_DARK_MISTRESS, BM_102_SUCCUBUS, BM_103_BLOOD_WITCH, BM_104_DARK_WITCH, /*BM_133_SOUL_BURNER,*/ BM_191_DARK_MISTRESS, BM_202_SUCCUBUS, BM_284_SIN_SPAWN, -1 },											1, 3, 5, S_1525_m_aura_09},
/*10*/	{ AUI_10_,	  "Lowers Damage Resistance",	AUEF_10_MELEE_RES_REDUCE,		{ BM_87_DARK_REAPER, BM_136_BLOOD_SLAYER, BM_152_BLACK_REAPER, BM_153_DEATH_MINION, BM_154_DEATH_MINION, BM_276_SWORD_SPIDER, -1 },								1, 3, 3, S_1526_m_aura_10_11},
/*11*/	{ AUI_11_,    "Lowers Damage Resistance",	AUEF_11_RANGED_RES_REDUCE,		{ BM_199_HELL_CLAN, BM_255_RAGE_CLAN, BM_256_RAGE_CLAN, -1 },								1, 3, 4, S_1526_m_aura_10_11},
/*12*/	{ AUI_12_,    "Lowers Stun Threshold",		AUEF_12_STUNTRSH_REDUCE,		{ BM_111_OBSIDIAN_GOLEM, BM_125_GOLD_GOLEM, BM_130_BLACK_GOLEM, -1 },											1, 2, 2, S_1527_m_aura_12},
/*13*/	{ AUI_13_,    "Lowers Damage",				AUEF_13_PLAYER_DAMGE_REDUCE, 	{ BM_52_BILE_DEMON, BM_55_GRIM_DEMON, BM_56_PIT_DEMON, BM_93_FLAYED_ONE, BM_178_OVERLORD, BM_179_FILTH_DEMON, -1 },					1, 3, 3, S_1528_m_aura_13},
/*14*/	{ AUI_14_,    "Lowers DFE",					AUEF_14_PLAYER_DFE_REDUCE, 		{ BM_277_SPIDERLING, -1 },					1, 3, 3, S_1798},

///*14*/	{ AUI_14_,	  "Turbo Mana Burn",		 AUEF_2_MP_BLEED,			 { BM_7_SKELETON, /*BM_174_ZHAR_BASEMNSTR,*/ -1 },							2, 100, 7, S_1519_m_aura_03},

};