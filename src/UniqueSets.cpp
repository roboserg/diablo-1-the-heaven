#include "stdafx.h"

UniqueSet UniqueSets[] = {

/*0*/	{ "Midas' Finery",        // Name
            6,                  // UniquesCount
           {                    // UniquesList
				1774 /*Midas' Touch - all */,
				1717 /*Midas' Ring - mages only*/,
				1622 /*Midas' Terret - all */,
				1405 /*Midas' Eye - all helm */,
				1508 /* Midas' Goldwrap - belt 4all*/,
				23 /* Midas' Threads - boots */,
           },
           10,                   // EffectCount
           {                    // Effects
				{ 2, { AE_LIFE_REGEN,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_GOLD_FIND,	0,	50,	50,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 3, { AE_GOLD_ABS_FIND,	0,	40,	40,	50,	50,	0,	0 } },
				{ 4, { AE_LIFE_PERCENT,	0,	25,	25,	0,	0,	0,	0 } },
				{ 4, { AE_ALL_STAT,	0,	5,	5,	0,	0,	0,	0 } },
				{ 5, { AE_MANA_PERCENT,	0,	25,	25,	0,	0,	0,	0 } },
				{ 5, { AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0 } },
				{ 6, { AE_RESIST_All,	0,	10,	10,	0,	0,	0,	0 } },
				{ 6, { AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0 } },
           }
		},

/*1*/	{ "Bhaltavir's Battle Steel",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				270 /*Bhaltavir's Garb*/,
				1685 /*Bhaltavir's Rage - berserker only helm*/,
				634 /* "Bhaltavir's Decapitator" - AXE */,
           },
           5,                   // EffectCount
           {                    // Effects
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_RANGED_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_DAMAGE,	0,	40,	40,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-35,	-35,	0,	0,	0,	0 } },
				{ 3, { AE_AC,	0,	30,	30,	0,	0,	0,	0 } },
           }
		},

/*2*/	{ "Civerb's Tools",        // Name
            4,                  // UniquesCount
           {                    // UniquesList
				171 /* Civerb's Club - 2h club - GRD+SVG */,
				1183 /* Civerb's Beard - amulet - 4all */,
				1605 /* Civerb's Vest - warriors + savages */,
				2293 /* Civerb's Mane - helm */,
           },
           9,                   // EffectCount
           {                    // Effects
				{ 2, { AE_AC,	0,	20,	20,	0,	0,	0,	0 } },
				{ 2, { AE_HIT_POINTS,	0,	75,	75,	0,	0,	0,	0 } },
				{ 3, { AE_TO_HIT_DAMAGE,	0,	80,	80,	0,	0,	20,	20 } },
				{ 3, { AE_DAMAGE,	0,	30,	30,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 4, { AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,	150,	150,	0,	0,	0,	0 } },
				{ 4, { AE_SPICIES_AC,	MON_1_DEMON,	40,	40,	0,	0,	0,	0 } },
				{ 4, { AE_SPICIES_DFE,	MON_1_DEMON,	-15,	-15,	0,	0,	0,	0 } },
				{ 4, { AE_ALL_STAT,	0,	5,	5,	0,	0,	0,	0 } },
           }
		},

/*3*/	{ "Immortal King",        // Name
            7,                  // UniquesCount
           {                    // UniquesList
				1907 /* Immortal King's Pillar - boots*/,
				1908 /* Immortal King's Forge - glove */,
				1237 /* Immortal King's Detail - belt */,
				1909 /* Immortal King's Soul Cage - armor */,
				1787 /* Immortal King's Will - helm */,
				1858 /* Immortal King's Stone Crusher - weap */,
				1910 /* Immortal King's Wall - shield */,
           },
           7,                   // EffectCount
           {                    // Effects
				{ 2, { AE_SPELLS_LEVEL,	0,	2,	2,	0,	0,	0,	0 } },
				{ 3, { AE_TO_HIT,	0,	0,	0,	0,	0,	50,	50 } },
				{ 4, { AE_LIFE_REGEN,	0,	10,	10,	0,	0,	0,	0 } },
				{ 5, { AE_RESIST_All,	0,	15,	15,	0,	0,	0,	0 } },
				{ 6, { AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0 } },
				{ 7, { AE_ATTACK_SPEED,	0,	4,	4,	0,	0,	0,	0 } },
				{ 7, { AE_CAST_SPEED,	0,	3,	3,	0,	0,	0,	0 } },
           }
		},

/*4*/	{ "Tal-Rasha's Path of Torment",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				72 /* Tal-Rasha's Duress - armor */,
				973 /* Binding of Tal-Rasha - ring */,
				1371 /* Tal-Rasha's Restraint - belt */,
           },
           6,                   // EffectCount
           {                    // Effects
				{ 2, { AE_MANA,	0,	75,	75,	0,	0,	0,	0 } },
				{ 3, { AE_SPELL_PERCENT,	PS_3_LIGHTNING,	35,	35,	0,	0,	0,	0 } },
				{ 3, { AE_SPELL_PERCENT,	PS_12_FIREBLAST,	35,	35,	0,	0,	0,	0 } },
				{ 3, { AE_SPELL_PERCENT,	PS_20_INCINERATE,	35,	35,	0,	0,	0,	0 } },
				{ 3, { AE_SPELL_PERCENT,	PS_35_ARCANE_STAR,	35,	35,	0,	0,	0,	0 } },
				{ 3, { AE_ELEMENT_PERCENT,	ET_0_PHYSICAL,	20,	20,	0,	0,	0,	0 } },
           }
		},

/*5*/	{ "Renegade's Outfit",        // Name
            4,                  // UniquesCount
           {                    // UniquesList
				645/*helm*/,
				362/*armor*/,
				148/*ammy*/,
				2294/*ring*/,
           },
           5,                   // EffectCount
           {                    // Effects
				{ 2, { AE_DFE,	0,	-10,	-10,	0,	0,	0,	0 } },
				{ 3, { AE_ABSORB_MELEE_DAMAGE,	0,	10,	10,	0,	0,	0,	0 } },
				{ 3, { AE_ABSORB_RANGED_DAMAGE,	0,	10,	10,	0,	0,	0,	0 } },
				{ 4, { AE_RESIST_All,	0,	15,	15,	0,	0,	0,	0 } },
				{ 4, { AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0 } },
           }
		},

/*6*/	{ "Celestial Arsenal",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1915 /* Celestial Visor - helm */,
				1276 /* Celestial Shelter - armor */,
				1914 /* Celestial Guard - shield */,
           },
           6,                   // EffectCount
           {                    // Effects
				{ 2, { AE_DFE,	0,	-10,	-10,	0,	0,	0,	0 } },
				{ 2, { AE_AC,	0,	15,	15,	0,	0,	0,	0 } },
				{ 3, { AE_PERCENT_DAMAGE,	0,	50,	50,	0,	0,	0,	0 } },
				{ 3, { AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	2,	2 } },
				{ 3, { AE_HIT_POINTS,	0,	50,	50,	0,	0,	0,	0 } },
				{ 3, { AE_MANA,	0,	25,	25,	0,	0,	0,	0 } },
           }
		},

/*7*/	{ "Tigris of Gaul",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1283 /* Tigris' Armsteel - gloves */,
				1911 /* Tigris' Visor - helm */,
				1944/* armor */,
           },
           5,                   // EffectCount
           {                    // Effects
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	10,	10,	0,	0,	0,	0 } },
				{ 2, { AE_STUN_RESIST,	0,	10,	10,	0,	0,	0,	0 } },
				{ 3, { AE_DAMAGE,	0,	40,	40,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-10,	-10,	0,	0,	0,	0 } },
				{ 3, { AE_RESIST_All,	0,	15,	15,	0,	0,	0,	0 } },
           }
		},

/*8*/	{ "Sazabi's Grand Tribute",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1912 /* "Sazabi's Mental Sheath" - KNS helm */,
				1913 /* "Sazabi's Ghost Liberator" - armor */,
				1984/*Sazabi's Cobalt Redeemers - boots */,
           },
           7,                   // EffectCount
           {                    // Effects
				{ 2, { AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	100,	100,	0,	0,	0,	0 } },
				{ 2, { AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_ELEM_HIT,	EA_MELEE_HOLY,	15,	15,	25,	25,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 3, { AE_RESIST_All,	0,	10,	10,	0,	0,	0,	0 } },
				{ 3, { AE_MINUS_LIGHT_RADIUS,	0,	1,	1,	0,	0,	0,	0 } },
				{ 3, { AE_MAGIC_FIND,	0,	-30,	-30,	0,	0,	0,	0 } },
           }
		},

/*9*/	{ "Arctic Gear",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1916 /* Arctic helm - SVG */,
				1917 /* Arctic Furs - SVG */,
				1949 /* amulette */,
           },
           8,                   // EffectCount
           {                    // Effects
				{ 2, { AE_AC,	0,	20,	20,	0,	0,	0,	0 } },
				{ 2, { AE_STUN_RESIST,	0,	10,	10,	0,	0,	0,	0 } },
				{ 2, { AE_TO_HIT_DAMAGE,	0,	50,	50,	0,	0,	25,	25 } },
				{ 2, { AE_DFE,	0,	-10,	-10,	0,	0,	0,	0 } },
				{ 3, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_RESIST_All,	0,	15,	15,	0,	0,	0,	0 } },
				{ 3, { AE_LIFE_REGEN,	0,	10,	10,	0,	0,	0,	0 } },
				{ 3, { AE_ELEM_HIT,	EA_MELEE_COLD,	1,	4,	7,	10,	0,	0 } },
           }
		},

/*10*/	{ "Rathma's Armory",        // Name
            6,                  // UniquesCount
           {                    // UniquesList
				1244 /* "Rathma's Death Gaze" */,
				1845 /*"Rathma's Bone Ribcage"*/,
				1919 /* "Rathma's Grips" */,
				1850 /* "Rathma's Girth" */,
				1920 /* "Rathma's Treads" */,
				1918 /* "Rathma's Bonewall" */,
           },
           6,                   // EffectCount
           {                    // Effects
				{ 2, { AE_ABSORB_RANGED_DAMAGE,	0,	15,	15,	0,	0,	0,	0 } },
				{ 3, { AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0 } },
				{ 4, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 5, { AE_RESIST_All,	0,	15,	15,	0,	0,	0,	0 } },
				{ 6, { AE_SPELLS_LEVEL,	0,	3,	3,	0,	0,	0,	0 } },
				{ 6, { AE_SUMMON_DAMAGE_PERCENT,	0,	15,	15,	0,	0,	0,	0 } },
           }
		},

/*11*/	{ "Sigon's Steel",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1921 /* "Sigon's Casque" */,
				1922 /* "Sigon's Shelter" */,
				1997 /* Sigon's Gage - glove */,
           },
           5,                   // EffectCount
           {                    // Effects
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_RANGED_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_AC,	0,	15,	15,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-15,	-15,	0,	0,	0,	0 } },
				{ 3, { AE_STUN_RESIST,	0,	10,	10,	0,	0,	0,	0 } },
           }
		},

/*12*/	{ "Poseidon's Gifts",        // Name
            2,                  // UniquesCount
           {                    // UniquesList
				1923 /* helm - MUR midgame set */,
				1924 /* shld */,
           },
           7,                   // EffectCount
           {                    // Effects
				{ 2, { AE_RESIST,	ET_6_COLD,	70,	70,	0,	0,	0,	0 } },
				{ 2, { AE_STUN_RESIST,	0,	10,	10,	0,	0,	0,	0 } },
				{ 2, { AE_DFE,	0,	-10,	-10,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_RANGED_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_TO_HIT_DAMAGE,	0,	35,	35,	0,	0,	15,	15 } },
				{ 2, { AE_RESIST,	ET_2_LIGHTNING,	-70,	-70,	0,	0,	0,	0 } },
           }
		},

/*13*/	{ "Heavenly Brethren",        // Name
            4,                  // UniquesCount
           {                    // UniquesList
				1925 /* helm - TMP mid game set */,
				1926 /* amo */,
				1927 /* shld */,
				1928 /* swrd */,
           },
           6,                   // EffectCount
           {                    // Effects
				{ 2, { AE_SPICIES_AC,	MON_0_UNDEAD,	40,	40,	0,	0,	0,	0 } },
				{ 2, { AE_SPICIES_AC,	MON_1_DEMON,	40,	40,	0,	0,	0,	0 } },
				{ 3, { AE_SPICIES_DFE,	MON_0_UNDEAD,	-20,	-20,	0,	0,	0,	0 } },
				{ 3, { AE_SPICIES_DFE,	MON_1_DEMON,	-20,	-20,	0,	0,	0,	0 } },
				{ 4, { AE_SPICIES_DAMAGE_PERCENT,	MON_0_UNDEAD,	250,	250,	0,	0,	0,	0 } },
				{ 4, { AE_SPICIES_DAMAGE_PERCENT,	MON_1_DEMON,	150,	150,	0,	0,	0,	0 } },
           }
		},

/*14*/	{ "Nox Eternis",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				0 /* ring */,
				1941 /* amu */,
				1936/* ring */,
           },
           8,                   // EffectCount
           {                    // Effects
				{ 2, { AE_XP_GAIN_PERCENT,	0,	50,	50,	0,	0,	0,	0 } },
				{ 2, { AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	2,	2 } },
				{ 2, { AE_STUN_RESIST,	0,	3,	3,	0,	0,	0,	0 } },
				{ 3, { AE_LIFE_REGEN,	0,	20,	20,	0,	0,	0,	0 } },
				{ 3, { AE_SPECIAL_AC,	0,	7,	7,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 3, { AE_CRIT_DAMAGE_PERCENT,	0,	70,	70,	0,	0,	0,	0 } },
				{ 3, { AE_PERCENT_DAMAGE,	0,	50,	50,	0,	0,	0,	0 } },
           }
		},

/*15*/	{ "Trang-Oul's Powers",        // Name
            6,                  // UniquesCount
           {                    // UniquesList
				1979,
				1980,
				2016 /* gloves */,
				2022 /* feet */,
				2023 /* helm */,
				2024 /* body */,
           },
           10,                   // EffectCount
           {                    // Effects
				{ 2, { AE_SUMMON_HP,	0,	300,	300,	0,	0,	0,	0 } },
				{ 3, { AE_SUMMON_AC,	0,	10,	10,	0,	0,	0,	0 } },
				{ 4, { AE_SUMMON_TO_HIT,	0,	0,	0,	0,	0,	10,	10 } },
				{ 5, { AE_SUMMON_DAMAGE,	0,	5,	5,	10,	10,	0,	0 } },
				{ 6, { AE_MANA_REGEN_PERCENT,	0,	50,	50,	0,	0,	0,	0 } },
				{ 6, { AE_GOLD_FIND,	0,	25,	25,	0,	0,	0,	0 } },
				{ 6, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 6, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 6, { AE_ABSORB_RANGED_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 6, { AE_RESIST_All,	0,	10,	10,	0,	0,	0,	0 } },
           }
		},

/*16*/	{ "Vidala's Rig",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				1981,
				1982,
				1983,
           },
           8,                   // EffectCount
           {                    // Effects
				{ 2, { AE_TO_HIT,	0,	0,	0,	0,	0,	30,	30 } },
				{ 2, { AE_DAMAGE,	0,	25,	25,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_RANGED_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 3, { AE_RESIST_All,	0,	10,	10,	0,	0,	0,	0 } },
				{ 3, { AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0 } },
				{ 3, { AE_MANA_REGEN,	0,	3,	3,	0,	0,	0,	0 } },
           }
		},

/*17*/	{ "Sunset Armory",        // Name
            2,                  // UniquesCount
           {                    // UniquesList
				2041 /* shinguards */,
				2042 /* parma */,
           },
           9,                   // EffectCount
           {                    // Effects
				{ 2, { AE_TO_HIT,	0,	0,	0,	0,	0,	15,	15 } },
				{ 2, { AE_DAMAGE,	0,	30,	30,	0,	0,	0,	0 } },
				{ 2, { AE_AC,	0,	10,	10,	0,	0,	0,	0 } },
				{ 2, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 2, { AE_ABSORB_MELEE_DAMAGE,	0,	5,	5,	0,	0,	0,	0 } },
				{ 2, { AE_BLOCK_CHANCE,	0,	0,	0,	0,	0,	5,	5 } },
				{ 2, { AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	2,	2 } },
				{ 2, { AE_RESIST_All,	0,	10,	10,	0,	0,	0,	0 } },
				{ 2, { AE_STUN_RESIST,	0,	5,	5,	0,	0,	0,	0 } },
           }
		},

/*18*/	{ "Legacy of Sir De'Ragon",        // Name
            3,                  // UniquesCount
           {                    // UniquesList
				2074 /* amulette */,
				2075 /* ring 1 */,
				2076 /* ring 2 */,
           },
           3,                   // EffectCount
           {                    // Effects
				{ 2, { AE_PERCENT_DAMAGE,	0,	100,	100,	0,	0,	0,	0 } },
				{ 3, { AE_DFE,	0,	-5,	-5,	0,	0,	0,	0 } },
				{ 3, { AE_MANA_REGEN,	0,	6,	6,	0,	0,	0,	0 } },
           }
		},

/*19*/	{ "Reign of Blood",        // Name
            7,                  // UniquesCount
           {                    // UniquesList
				4592 /* amulet */,
				4593 /* helm */,
				4594 /* boots */,
				4595 /* gloves */,
				4596 /* red ring */,
				4597 /* blu ring */,
				4598 /* ama */,
           },
           10,                   // EffectCount
           {                    // Effects
				{ 2, { AE_DAMAGE,	0,	50,	50,	0,	0,	0,	0 } },
				{ 3, { AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0 } },
				{ 3, { AE_RESIST_All,	0,	50,	50,	0,	0,	0,	0 } },
				{ 4, { AE_ATTACK_SPEED,	0,	4,	4,	0,	0,	0,	0 } },
				{ 5, { AE_STUN_RESIST,	0,	15,	15,	0,	0,	0,	0 } },
				{ 5, { AE_MANA_STEAL_PERCENT,	0,	2,	2,	0,	0,	0,	0 } },
				{ 6, { AE_ALL_STAT,	0,	20,	20,	0,	0,	0,	0 } },
				{ 6, { AE_AC,	0,	100,	100,	0,	0,	0,	0 } },
				{ 7, { AE_ABSORB_MELEE_DAMAGE,	0,	50,	50,	0,	0,	0,	0 } },
				{ 7, { AE_ABSORB_RANGED_DAMAGE,	0,	50,	50,	0,	0,	0,	0 } },
           }
		},

/*20*/	{ "Traitor's Apparel",        // Name
            7,                  // UniquesCount
           {                    // UniquesList
				4599,
				4600,
				4601,
				4602,
				4603,
				4604,
				4605,
           },
           9,                   // EffectCount
           {                    // Effects
				{ 2, { AE_MANA,	0,	50,	50,	0,	0,	0,	0 } },
				{ 3, { AE_SPELLS_LEVEL,	0,	5,	5,	0,	0,	0,	0 } },
				{ 4, { AE_CAST_SPEED,	0,	3,	3,	0,	0,	0,	0 } },
				{ 5, { AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0 } },
				{ 5, { AE_RESIST_All,	0,	30,	30,	0,	0,	0,	0 } },
				{ 6, { AE_MANA_REGEN,	0,	20,	20,	0,	0,	0,	0 } },
				{ 6, { AE_FLAG,	EA_INFRAVISION,	0,	0,	0,	0,	0,	0 } },
				{ 7, { AE_XP_GAIN_PERCENT,	0,	10,	10,	0,	0,	0,	0 } },
				{ 7, { AE_STAT,	ST_MAGIC,	100,	100,	0,	0,	0,	0 } },
           }
		},

/*21*/	{ "Mordessa's Ensemble",        // Name
            4,                  // UniquesCount
           {                    // UniquesList
				4606,
				4607,
				4608,
				4609,
           },
           4,                   // EffectCount
           {                    // Effects
				{ 2, { AE_PERCENT_DAMAGE,	0,	100,	100,	0,	0,	0,	0 } },
				{ 3, { AE_CRIT_CHANCE,	0,	0,	0,	0,	0,	3,	3 } },
				{ 4, { AE_DFE,	0,	-20,	-20,	0,	0,	0,	0 } },
				{ 4, { AE_AC,	0,	100,	100,	0,	0,	0,	0 } },
           }
		},

/*22*/	{ "The Royal Relics",        // Name
            8,                  // UniquesCount
           {                    // UniquesList
				4610,
				4611,
				4612,
				4613,
				4614,
				4615,
				4616,
				4617,
           },
           9,                   // EffectCount
           {                    // Effects
				{ 2, { AE_CLVL_SUMMON_DAMAGE,	0,	0,	0,	50,	50,	0,	0 } },
				{ 3, { AE_CLVL_SUMMON_HP_PERCENT,	0,	3,	3,	25,	25,	0,	0 } },
				{ 4, { AE_CLVL_SUMMON_AC,	0,	0,	0,	100,	100,	0,	0 } },
				{ 5, { AE_CLVL_STAT_ALL,	0,	0,	0,	50,	50,	0,	0 } },
				{ 6, { AE_HIT_RECOVERY,	0,	3,	3,	0,	0,	0,	0 } },
				{ 7, { AE_CLVL_RESIST_ALL,	0,	0,	0,	70,	70,	0,	0 } },
				{ 7, { AE_CLVL_DFE,	0,	0,	0,	-50,	-50,	0,	0 } },
				{ 8, { AE_CLVL_MELEE_RES,	0,	0,	0,	60,	60,	0,	0 } },
				{ 8, { AE_CLVL_ARROW_RES,	0,	0,	0,	80,	80,	0,	0 } },
           }
		},

/**/	{ "",	-1 /*END_MARKER*/ }
};
const size_t count_UniqueSet = countof(UniqueSets) - 1;

